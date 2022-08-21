////////////////////////////////////////////////////////////////////////
// $Source: x:/prj/tech/libsrc/sound/RCS/fade.cpp $
// $Author: PATMAC $
// $Date: 1998/03/20 12:55:28 $
// $Revision: 1.11 $
//
// (c) 1996 Looking Glass Technologies Inc.
// Pat McElhatton (from JohnB)
//
// Module name: faders
// File name: fade.cpp
//
// Description: Implementation of volume, pan and frequency fades
//
////////////////////////////////////////////////////////////////////////
//
// all fade times are in MILLISECONDS (I theenk)
//

#include <windows.h>
#include <lg.h>
#include <mmsystem.h>
#include <dsound.h>

#include <lgsndi.h>

cSndFadeTask::cSndFadeTask(cSndSample *sample)
{
	mDestination = mOriginal = mCurrent = mTimeLeft = mTime = 0;
	subject = sample;
   mEndCallback = NULL;
}

cSndFadeTask::~cSndFadeTask()
{

}


BOOL
cSndFadeTask::Active()
{
   return mTimeLeft != 0;
	// return mTime != 0;
}


//
// return false if fade should not be done (IE sample isn't playing)
//
BOOL cSndFadeTask::IsFadeable( void )
{
   return subject->IsPlaying();
}


void cSndFadeTask::Fade(int32 dest, uint32 time, eSndFadeFlags flags)
{
	mDestination = dest;
	mTimeLeft = mTime = time;
	mFlags = flags;
	mOriginal = mCurrent = GetCurrentValue();
	
   if ( IsFadeable() ) {
      TLOG3("Fade: dest val %ld start val %ld time %ld",
           dest, mOriginal, time );
      if( mDestination == mOriginal ) {
         TLOG0("Fade: dest == start" );
         // destination == original, no fade needed
         mTimeLeft = 0;
         if ( flags == kSndFadeStop ) {
            // since the fade is over, do the requested stop
            FadeEndStop();
         }
         if ( mEndCallback != NULL ) {
            // call end of fade callback
            mEndCallback( subject, mpEndCBData );
         }
      }
	} else {
		mTimeLeft = 0;
	}				
}


void cSndVolumeFadeTask::SetNextValue(int32 val)
{
   if ( val < kSndMinVolume ) {
      val = kSndMinVolume;
   } else if ( val > kSndMaxVolume ) {
      val = kSndMaxVolume;
   }
	subject->SetVolume(val);
}

int32 cSndVolumeFadeTask::GetCurrentValue()
{
	return subject->GetVolume();
}


void cSndPanFadeTask::SetNextValue(int32 val)
{
   subject->SetPan(val);
}

int32 cSndPanFadeTask::GetCurrentValue()
{
   return subject->GetPan();
}

void cSndFrequencyFadeTask::SetNextValue(int32 val)
{
   subject->SetFrequency(val);
}

int32 cSndFrequencyFadeTask::GetCurrentValue()
{
   return subject->GetFrequency();
}



//
// do a stop for a fade
//
void
cSndFadeTask::FadeEndStop( void )
{
   // let sample-end-callback know stop is due to end of fade
   subject->SetFlags( kSndFadeStop );
   subject->DeferredStop();
}


//
// return TRUE if this fade is still active
//
BOOL
cSndFadeTask::DoFadeStep( void )
{
   BOOL stillActive = FALSE;

   if ( Active() ) {
      int32 v = (int32) mCurrent;
      int32 d = (int32) mDestination;
  
      if ( mTimeLeft > kFadeGranularity ) {
         mTimeLeft -= kFadeGranularity;
         v += ((d - v) * kFadeGranularity) / (int32) mTimeLeft;
      } else {
         mTimeLeft = 0;
         v = d;
      }

      // have to determine direction to tell if we are really done
      TLOG3( "DoFadeStep orig %d dest %d cur %d", mOriginal, d, v );
      if( (mOriginal > d && v <= d) || (mOriginal < d && v >= d) ) {
         //
         // fade is done!
         //
         SetNextValue(d);
         mCurrent = d;
         mTimeLeft = 0;

         // Deal With Flags
         switch( mFlags ) {
            case kSndFadeStop:
               FadeEndStop();
               break;
            case kSndFadeReverse:
               mTimeLeft = mTime;
               mDestination = mOriginal;
               mOriginal = mCurrent = d;
               mFlags = kSndFadeStop; // so it will stop the next time
               stillActive = TRUE;
               break;
            default:
               break;  
         }
         if ( mEndCallback != NULL ) {
            // call end of fade callback
            mEndCallback( subject, mpEndCBData );
         }
      } else {
         //
         // fade is not done
         //
         SetNextValue(v);
         mCurrent = v;
         stillActive = TRUE;
      }
   }

   return stillActive;
}


//
// register callback which should be called when fade ends
//
void
cSndFadeTask::RegisterEndCallback( SndEndCallback  endFunc,
                                   void            *pEndCBData )
{
   mEndCallback = endFunc;
   mpEndCBData = pEndCBData;
}


//
// below is the support for group volume fades
//
cSndGroupVolumeFadeTask::cSndGroupVolumeFadeTask( uint32    group,
                                                  cSndMixer *pMixer)
   :cSndFadeTask( NULL )   // invoke our base class constructor
{
	mGroup = group;
   mpMixer = pMixer;
}

void cSndGroupVolumeFadeTask::SetNextValue(int32 val)
{
   if ( mGroup == kSndNumGroups ) {
      // this is really the master volume fade
      mpMixer->SetMasterVolume( val );
   } else {
      mpMixer->SetGroupVolume( mGroup, val );
   }
}


int32 cSndGroupVolumeFadeTask::GetCurrentValue()
{
   if ( mGroup == kSndNumGroups ) {
      // this is really the master volume fade
      return mpMixer->GetMasterVolume();
   } else {
      return mpMixer->GetGroupVolume( mGroup );
   }
}



//
// group iterator callback which does a deferred stop
//
static ISndSample *
fadeStop( ISndMixer     * /* pMixer */,
          ISndSample    *pISample,
          void          * /* pCBData */ )
{
   cSndSample  *pSample;

   pSample = (cSndSample *) pISample;
   pSample->DeferredStop();

   return pISample;
}


//
// do a stop for a fade
//
void
cSndGroupVolumeFadeTask::FadeEndStop( void )
{
   // since the fade is over, do the requested stop on all
   //  samples which have the same group as the fade
   if ( mGroup == kSndNumGroups ) {
      // this is really the master volume fade, stop ALL samples
      mpMixer->DoForGroupSamples( kSndAnyGroup, fadeStop, this );
   } else {
      mpMixer->DoForGroupSamples( mGroup, fadeStop, this );
   }
}


//
// return false if fade should not be done (never in our case)
//
BOOL cSndGroupVolumeFadeTask::IsFadeable( void )
{
   return TRUE;
}


