////////////////////////////////////////////////////////////////////////
// $Source: x:/prj/tech/libsrc/sound/RCS/sample.cpp $
// $Author: PATMAC $
// $Date: 1998/03/20 13:07:15 $
// $Revision: 1.52 $
//
// (c) 1996 Looking Glass Technologies Inc.
// Pat McElhatton (from JohnB)
//
// Module name: Sample
// File name: sample.cpp
//
// Description: Basic digital sound playback thingie
//
////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <lg.h>
#include <assert.h>

#include <lgsndi.h>
#include <mprintf.h>

#include <mixerlck.h>

uint32 cSndSample::mNextSerialNum = 0;
uint32 cSndSample::mSamplesMade = 0;
uint32 cSndSample::mSamplesDestroyed = 0;
uint32 cSndSample::mNumAudible = 0;
uint32 cSndSample::mNumInaudible = 0;

// This is the logging system interrupt handler mask - it lives here so that the
//  timelog module will only be linked into an app if it actually uses time logging
long logIntMask;

// this is used as a sample ID in logging
#define SAMPLE_ID    ((mGroup << 16) + mSerialNum)

// this is the sample name used in logging
#define SAMPLE_NAME ((long) mpName)

// state flag convenience macros
#define IS_MUTED   (mStateFlags & kSndFlagMuted)
#define IS_PAUSED  (mStateFlags & kSndFlagPaused)
#define IS_STREAM  (mStateFlags & kSndFlagStream)
#define IS_LOOP    (mStateFlags & kSndFlagLooped)
#define IS_AUDIBLE (mStateFlags & kSndFlagAudible)
#define IS_RUNNING (!IS_PAUSED && (mStateFlags == kSndStatePlaying))

// estimated sample position
#define ESTIMATED_POSITION( when ) \
  ( (((float) mFrequency * ((when) - mBaseTime)) / 1000.0 ) + mBasePos )

#define ESTIMATED_POSITION_NOW  ESTIMATED_POSITION( timeGetTime() )

// allow only one thread in critical sections
#define MIXER_MUTEX \
   cMixerAutoLock __mixer_lock__( mpMixer->MutexRef() )

////////////////////////////////////
// IMPLEMENT_UNAGGREGATABLE
//
// This does all the implementations of IUnknown methods
//

IMPLEMENT_UNAGGREGATABLE_SELF_DELETE(cSndSample, ISndSample);

///////////////////////////////////
// cSndSample constructor
//
//  This just sets the state to something safe.
//  Init will tell us if everything is ok.
//

cSndSample::cSndSample( cSndMixer               *pMixer,
                        eSndSampleCreateFlagSet flags )
{
   sSndVector origin;

   mpMixer = pMixer;

   mLastWrite = 0;

   mFlags = flags;
   mAppData = 0;
   fnFillCB = NULL;
   mpFillCBData = NULL;
   fnEndCB = NULL;
   mpEndCBData = NULL;
   fnPreemptCB = NULL;
   mpPreemptCBData = NULL;
   mSerialNum = mNextSerialNum++;
   mDumpFile = NULL;
   mVolume = kSndMaxVolume;
   mGroup = 0;
   mGroupVolume = 0;
   mPan = kSndPanCenter;
   mBaseOffset = 0;
   mBasePos = 0;
   mState = kSndStateCreated;
   mStateFlags = kSndFlagNone;
   mLastRead = 0;
   mInitBuffPos = -1;
   mPriority = 0;

   m3DMethod = kSnd3DMethodNone;
   origin.x = 0.0;
   origin.y = 0.0;
   origin.z = 0.0;
   m3DPosition = origin;
   m3DPosition.x = 4.0;
   m3DVelocity = origin;
   m3DConeOuterAngle = 360;
   m3DConeInnerAngle = 0;
   m3DConeOrientation = origin;
   m3DConeOrientation.x = 1.0;
   m3DMinDistance = 1.0;
   m3DMaxDistance = 4000.0;
   mAmbientVolume = 0;
   m3DMode = kSnd3DModeNone;

   // fill in default name
   mpName = new char[32];
   SetName( "Fishmael" );

   // the fade tasks
   memset(mFadeTasks, 0, sizeof(mFadeTasks));

   mSamplesMade++;
   TLOG3( "Smp::Constructor %s %ld made %ld destroyed", SAMPLE_NAME,
         mSamplesMade, mSamplesDestroyed );

   mpPrev = NULL;
   mpNext = NULL;
   MoveToList( mpMixer->InaudibleHead() );

   mpMixer->Inform(this, kSndSampleCreated);

}


///////////////////////////////////
// cSndSample destructor
//
// make sure that the implementor is gone
//

cSndSample::~cSndSample()
{
   mSamplesDestroyed++;
   mState = kSndStateDestroyed;
   TLOG3( "Smp::Destructor %s %ld made %ld destroyed", SAMPLE_NAME,
         mSamplesMade, mSamplesDestroyed );

   // tell the mixer we're gone...
   mpMixer->Inform(this, kSndSampleDestroyed);

   // if data dump is still on, turn it off
   if ( mDumpFile != NULL ) {
      fclose( mDumpFile );
   }

   // release all allocated faders
   for ( int fader = 0; fader < kSndNumFaders; fader++ ) {
      cSndFadeTask *pFade = mFadeTasks[fader];
      if ( pFade != NULL ) {
         // release the fade
         delete pFade;
         mFadeTasks[fader] = (cSndFadeTask *) NULL;
      }
   }        

   RemoveFromList();

   // delete the name string, unless time logging is enabled, since
   //  logging uses the names at dump time, which is long after the
   //  sample is destroyed
#ifndef DO_TIME_LOGGING
   delete mpName;
#endif
}

///////////////////////////////////
//
// MakeInaudible - deallocate direct sound buffer
//  moves sample to inaudible list
//
void
cSndSample::MakeInaudible()
{
   MIXER_MUTEX;

   TLOG3( "Smp::MakeInaudible %s %ld audible %ld inaudible",
         SAMPLE_NAME, mNumAudible, mNumInaudible );

   if ( IS_AUDIBLE ) {
      LLStop();
      mpMixer->DoTrace( (void *) mBufferLen, kSndBufferFree );
      LLRelease();
      MoveToList( mpMixer->InaudibleHead() );
      ClearFlags( kSndFlagAudible );
      mpMixer->FreeChannel();
   }
}


///////////////////////////////////
// Init
//
// Gets called by the mixer to do the nasty so we dont really have to reach
// in and grab that sampleImp thing

BOOL
cSndSample::Init( sSndAttribs    *pAttribs,
                  void           *pBuffer,
                  uint32         bufferLen,
                  uint32         numSamples )
{
   TLOG3( "Smp::Init %s nBytes %d nSamples %d", SAMPLE_NAME, bufferLen, numSamples );
   mAttribs = *pAttribs;
   mBytesPerSample = (mAttribs.bitsPerSample / 8) * mAttribs.nChannels;

   mpBuffer = pBuffer;
   mBufferLen = bufferLen;
   mNumSamples = numSamples;

   mFrequency = pAttribs->sampleRate;

   if ( mFlags == kSndSampleStream ) {
      SetFlags( kSndFlagStream );
   }
   LLInit();

   mState = kSndStateInited;

   mBaseOffset = 0;

   // samples start out inaudible
   MoveToList( mpMixer->InaudibleHead() );

   return TRUE;
}

////////////////////////////////////
// Start
//
// Internal version of play, called by Play, Resume, Loop & UnMute
// NOTE: mInitBuffPos is sort of a secret parameter - it is a sample
//   position which is set after the buffer is loaded but before the
//   DirectSound Play is done

HRESULT
cSndSample::Start( void )
{
   HRESULT res;
   MIXER_MUTEX;

   mState = kSndStatePlaying;
   res = DS_OK;

   if ( !IS_PAUSED ) {

      if ( IS_MUTED ) {
         //
         // muted - just pretend that play started now
         //
         mBaseTime = timeGetTime();
         mBaseOffset = mBytesPerSample * mBasePos;
         TLOG2( "Smp::Start - reset baseTime %ld basePos %ld", mBaseTime, mBasePos );
      } else {

         // unpaused, unmuted - just play it!
         if ( !IS_AUDIBLE ) {
            //
            // try to make it audible
            //

            // calculate buffer size, base position and base offset
            if ( !IS_STREAM ) {
               //
               // for oneshots, the base position is always 0
               //
               if ( mBasePos >= mNumSamples ) {
                  if ( IS_LOOP ) {
                     mInitBuffPos = mBasePos % mNumSamples;
                  } else {
                     mInitBuffPos = mNumSamples;
                  }
               } else {
                  mInitBuffPos = mBasePos;
               }
               // Smp::Start will set the position to mInitBuffPos
               mBasePos = 0;
               mBaseOffset = 0;
            } else {
               //
               // for streams, the caller has set mBasePos to the
               //    play start position (#samples into source)
               //
               if ( mBasePos > mNumSamples ) {
                  mBasePos = mNumSamples;
               }
               // set base offset to position play should start at
               mBaseOffset = mBytesPerSample * mBasePos;
            }

            if ( mBufferLen != 0 ) {
               MakeAudible();
            }
         }

         if ( IS_AUDIBLE ) {
            res = LLStart();
         }

      }
   }

   if ( SUCCEEDED(res) ) {
      mpMixer->Inform(this, kSndSampleStarted);
   }

   return res;
}

////////////////////////////////////
//
// Play
//
// Play a sound.  It better be ready to go...

STDMETHODIMP_(void) cSndSample::Play()
{
   TLOG2( "\nSmp::Play %s flags 0x%x\n", SAMPLE_NAME, mFlags );

   if ( IsPlaying() ) {
      return;
   }

   //assert( mState == kSndStateInited );
   Start();
}


////////////////////////////////////
//
// Stop
//
// Stops the sound, destroys the buffer, informs the mixer
// and does the callback

STDMETHODIMP_(void) cSndSample::Stop()
{
   TLOG1( "\nSmp::Stop %s\n", SAMPLE_NAME );

   if ( (mStateFlags & (kSndFlagMuted | kSndFlagPaused)) ==  kSndFlagMuted ) {
      // muted & unpaused - estimate stop position
      mBasePos = ESTIMATED_POSITION_NOW;
   }

   if ( IsPlaying() ) {
      LLStop();
   }

   mpMixer->Inform(this, kSndSampleStopped);

   // insure that callback doesn't delete us
   AddRef();

   // call the end-of-sample callback
   if ( fnEndCB != NULL ) {
      (fnEndCB) ( this, mpEndCBData );
   }

   mState = kSndStateStopped;
   ClearFlags( kSndFlagPleaseStop );

   Release();
}


////////////////////////////////////
//
// DeferredStop
//
// Stops the sound at the hardware level
//   the Stop will be completed by the next cSndMixer::Update call
// This is called from asynchronous things, where invoking the end
// callback immediately is not a good idea

void
cSndSample::DeferredStop()
{
   TLOG1( "Smp::DeferredStop %s", SAMPLE_NAME );

   if( IsPlaying() ) {
      LLStop();
   }
   // this will cause sample to stop on next Mixer::Update
   SetFlags( kSndFlagPleaseStop );
}


////////////////////////////////////
//
// Pause
//
// Stops the sound from playing without telling the mixer.
// This will have several side effects:
//    1. DirectSound will now report that the sound is not playing,
//       therefore it wont show up in the iterator
//    2. If it is never told to resume it will never go away

STDMETHODIMP_(void) cSndSample::Pause()
{

   TLOG1( "Smp::Pause %s", SAMPLE_NAME );
   if ( IS_PAUSED ) {
      return;
   }

   if ( IS_MUTED ) {
      // calculate stop position from current time
      mBasePos = ESTIMATED_POSITION_NOW;
      if ( IS_LOOP ) {
         mBasePos = mBasePos % mNumSamples;
      }
   } else {
      LLPause();
      SetFlags( kSndFlagReallyPaused );
   }

   SetFlags( kSndFlagPaused );
}


////////////////////////////////////
//
// Resume
//
// This will continue a sound that was paused or otherwise
// have the same effect as play

STDMETHODIMP_(void) cSndSample::Resume()
{
   TLOG1( "Smp::Resume %s", SAMPLE_NAME );
   if ( !IS_PAUSED ) {
      return;
   }

   ClearFlags( kSndFlagPaused );

   if ( IS_MUTED ) {
      mBaseTime = timeGetTime();
      TLOG1( "Smp::Resume - reset baseTime %ld", mBaseTime );
   } else {
      LLResume();
   }
}


////////////////////////////////////
//
// Loop - start playback in loop mode
// this will cause that sound to play forever.
//

STDMETHODIMP_(void) cSndSample::Loop()
{
   TLOG2( "\nSmp::Loop %s flags 0x%x\n", SAMPLE_NAME, mFlags );

   if ( IsPlaying() ) {
      return;
   }

   SetFlags( kSndFlagLooped );

   //assert( mState == kSndStateInited );
   Start();
}


////////////////////////////////////
//
// Mute - Make a sound inaudible - this
//  frees the associated dsound object
//

STDMETHODIMP_(void)
cSndSample::Mute()
{
   TLOG1( "Smp::Mute %s", SAMPLE_NAME );

   if ( IS_MUTED ) {
      return;
   }

   if ( IS_AUDIBLE ) {
      // switch from position being based on where DSound play cursor is
      //  to position being estimated based on time
      mBasePos = GetPosition();
   }
   // TBD: should the next line be inside the above if?
   mBaseTime = timeGetTime();
   SetFlags( kSndFlagMuted );
   TLOG2( "Smp::Mute - reset baseTime %ld basePos %ld", mBaseTime, mBasePos );
   // release the low-level sound object
   if ( IS_AUDIBLE ) {
      MakeInaudible();
   }
}


////////////////////////////////////
//
// UnMute - make a muted sound audible - this actually reallocates
// a dsound object and resyncs/reloads the source stream
//
STDMETHODIMP_(void)
cSndSample::UnMute()
{
   TLOG2( "\nSmp::UnMute %s flags 0x%x\n", SAMPLE_NAME, mFlags );

   if ( !IS_MUTED ) {
      return;
   }
   ClearFlags( kSndFlagMuted );
   if ( mState != kSndStatePlaying ) {
      // mute & unmute before play start just set/clear the mute flag
      return;
   }

   // must have a fill callback to unmute
   assert( fnFillCB != NULL );

   SetFlags( kSndFlagResyncNeeded );
   if ( IS_PAUSED ) {

      //  paused - just reload ring buffer so it is ready
      //   to play when a Resume happens
      // alloc & init direct sound buffer
      LLUnMute();

   } else {

      mBasePos = ESTIMATED_POSITION_NOW;
      Start();
   }
   ClearFlags( kSndFlagResyncNeeded );

}


void
cSndSample::LLUnMute( void )
{
   uint32 playPos, avail;

   if ( MakeAudible() ) {
      if ( IS_STREAM ) {
         avail = AvailToWrite();
         (fnFillCB)( this, mpFillCBData, avail );
      } else {
         // for one-shots, we must fill the entire buffer, so set the player position
         //  to 0 so it will start filling there
         playPos = mBasePos;
         mBasePos = 0;
         (fnFillCB)( this, mpFillCBData, mBufferLen );
         SetPosition( playPos );
      }            
   }
}


////////////////////////////////////
//
// ResyncNeeded - Return TRUE if fill callback must do a seek within its
//   source stream.  Return position to start at in pStartPos (in samples)
//

STDMETHODIMP_(BOOL)
cSndSample::ResyncNeeded( uint32 *pStartPos )
{
   if ( mStateFlags & kSndFlagResyncNeeded ) {
      if ( pStartPos != NULL ) {
         *pStartPos = mBasePos;
      }
      TLOG2( "Smp::ResyncNeeded %s at pos %ld", SAMPLE_NAME, *pStartPos );
      return TRUE;
   } else {
      return FALSE;
   }
}


////////////////////////////////////
//
// Preempt - Preempt a sound - just call the preempt callback if one is
//   registered, else do a Stop
//
STDMETHODIMP_(void)
cSndSample::Preempt()
{
   TLOG1( "\nSmp::Preempt %s\n", SAMPLE_NAME );

   SetFlags( kSndFlagPreempted );
   if ( fnPreemptCB != NULL ) {

      // preempt CB registered - let it do Mute or Stop
      (fnPreemptCB) ( this, mpPreemptCBData );

   } else {

      // no preempt CB, just do a Stop
      Stop();
   }
   ClearFlags( kSndFlagPreempted );
}


////////////////////////////////////
//
// SetGroupVolume - set the group volume
//

STDMETHODIMP_(void) cSndSample::SetGroupVolume(int32 vol)
{
   mGroupVolume = vol;

   // if sample is audible, change volume immediately
   if ( IS_AUDIBLE ) {
      SetVolume( mVolume );
   }
}


////////////////////////////////////
//
// SetVolume - set the sample volume
//

STDMETHODIMP_(void) cSndSample::SetVolume(int32 vol)
{
   TLOG3( "Smp::SetVolume %s %d + group %d", SAMPLE_NAME, mVolume, mGroupVolume );

   if ( vol > 0 ) {
      vol = 0;
   }
   mVolume = vol;
   LLSetVolume( vol + mGroupVolume );
}


////////////////////////////////////
//
// SetPosition - Set the position of the sample
//  For streams, this should really only be done while paused or not yet playing
//

STDMETHODIMP_(void) cSndSample::SetPosition(uint32 pos)
{
   TLOG2( "Smp::SetPosition %s %d", SAMPLE_NAME, pos );

   if ( pos >= mNumSamples ) {
      if ( IS_LOOP ) {
         pos = pos % mNumSamples;
      } else {
         pos = mNumSamples - 1;
      }
   }

   if ( IS_STREAM ) {
      //
      // set flag so that when data is loaded, the fill callback
      // first does a seek to the specified spot in the source
      //
      SetFlags( kSndFlagResyncNeeded );
   }

   if ( IS_AUDIBLE ) {
      if ( IS_STREAM ) {
         //
         // setup so that base of buffer corresponds to pos, 
         // and so playback will start at base of buffer
         //
         mBasePos = pos;
         pos = 0;
         // setup any buffer filling to start at base of buffer
         mLastWrite = 0;
      }

      LLSetPosition( pos * mBytesPerSample );
   } else {
      // since we're inaudible, just change the estimated time base
      mBaseTime = timeGetTime();
      mBasePos = pos;
   }
}

///////////////////////////////////
//
// SetData - Set the app data field
//

STDMETHODIMP_(void) cSndSample::SetData(int32 data)
{
   TLOG2( "Smp::SetData %s %d", SAMPLE_NAME, data );
   mAppData = data;
}


///////////////////////////////////
//
// SetPriority - Set the app priority field
//

STDMETHODIMP_(void) cSndSample::SetPriority(int32 pri)
{
   TLOG2( "Smp::SetPriority %s %d", SAMPLE_NAME, pri );
   mPriority = pri;
}


///////////////////////////////////
//
// SetSuperInfo -  set superclass info field - this is support for sound utils
//  layered on top of ISndSample
//

STDMETHODIMP_(void)
cSndSample::SetSuperInfo(void *pSuperInfo)
{
   TLOG2( "Smp::SetSuperInfo %s 0x%lx", SAMPLE_NAME, ((long) pSuperInfo) );
   mpSuperInfo = pSuperInfo;
}


////////////////////////////////////
//
// SetGroup - Set the group
//

STDMETHODIMP_(void) cSndSample::SetGroup(uint32 group)
{
   assert( group < kSndNumGroups );

   TLOG2( "Smp::SetGroup %s, group %d", SAMPLE_NAME, group );
   mGroup = group;
   SetGroupVolume( mpMixer->GetGroupVolume( group )
                   + mpMixer->GetMasterVolume() );
}


///////////////////////////////////
//
// GetVolume - return the volume setting
//

STDMETHODIMP_(int32) cSndSample::GetVolume()
{
   return mVolume;
}


///////////////////////////////////
//
// GetPan - return the pan setting
//

STDMETHODIMP_(int32) cSndSample::GetPan()
{
   return mPan;
}


///////////////////////////////////
//
// GetFrequency - return the playback frequency
//

STDMETHODIMP_(uint32) cSndSample::GetFrequency()
{
   return mFrequency;
}


///////////////////////////////////
//
// GetData - return the app data field
//

STDMETHODIMP_(int32) cSndSample::GetData()
{
   return mAppData;
}


///////////////////////////////////
//
// GetPriority - return the app priority field
//

STDMETHODIMP_(int32) cSndSample::GetPriority()
{
   return mPriority;
}


///////////////////////////////////
//
// GetSuperInfo - return super-class info pointer
//  (support for sound utilities layered on top of cSndSample)
//

STDMETHODIMP_(void *)
cSndSample::GetSuperInfo()
{
   return mpSuperInfo;
}


///////////////////////////////////
//
// GetStatus - return sample state & flags
//

STDMETHODIMP_(void)
cSndSample::GetStatus( eSndState    *pState,
                       eSndFlagSet  *pFlags )
{
   *pState = mState;
   *pFlags = mStateFlags;
   TLOG3( "Smp::GetStatus %s state %d flags 0x%lx", SAMPLE_NAME, *pState, *pFlags );
}


///////////////////////////////////
//
// GetPosition - return the play position(in samples)
//

STDMETHODIMP_(uint32)
cSndSample::GetPosition()
{
   uint32 playPos;

   if ( IS_MUTED ) {

      if ( !IS_RUNNING ) {

         // paused & muted stream - pos is just mBasePos
         playPos = mBasePos;

      } else {

         playPos = ESTIMATED_POSITION_NOW;
         if ( IS_LOOP ) {

            playPos = playPos % mNumSamples;

         } else {

            if ( playPos > mNumSamples ) {
               // TBD: should we do a deferred stop here?
               playPos = mNumSamples;
            }

         }

      }

   } else {

      playPos = LLGetPosition();

   }

   TLOG2( "Smp::GetPosition %s %d", SAMPLE_NAME, playPos );
   return playPos;
}

////////////////////////////////////
//
// GetPositions - return the play & write positions
//

STDMETHODIMP_(void) cSndSample::GetPositions( uint32 *pWrite,
                                              uint32 *pPlay )
{
   *pWrite = (mLastWrite + mBaseOffset) / mBytesPerSample;
   *pPlay = GetPosition();
   TLOG3( "Smp::GetPositions %s rd %d wr %d", SAMPLE_NAME, *pPlay, *pWrite );
}


////////////////////////////////////
//
// GetSerialNumber - return samples serial number
//

STDMETHODIMP_(uint32) cSndSample::GetSerialNumber()
{
   return mSerialNum;
}


////////////////////////////////////
//
// GetGroup - return the samples group number
// 

STDMETHODIMP_(uint32) cSndSample::GetGroup()
{
   return mGroup;
}


////////////////////////////////////
//
// GetAttribs - return the samples attributes
//

STDMETHODIMP_(void) cSndSample::GetAttribs( sSndAttribs *pAttribs )
{
   assert( pAttribs != NULL );

   *pAttribs = mAttribs;
}


//
// helper for LoadBuffer which just copies memory
//
static void
*memCopyFunk( void    *pSrcData,
              void    *pDstData,
              uint32  numBytes )
{
   memcpy( pDstData, pSrcData, numBytes );

   return pDstData;
}


////////////////////////////////////
// LoadBuffer
//
// Attempt to lock the buffer and copy memory into it.  Give the data and the
// length to be copied.  This should be used for double buffered samples.

STDMETHODIMP_(eSndError) cSndSample::LoadBuffer(uint8 *data, uint32 len)
{
   return LoadBufferIndirect( memCopyFunk, data, len );
}

////////////////////////////////////
// Squelch
//
// Stop streaming a sound when its absolute level drops below
//  sqLevel for at least sqCount samples.  sqLevel is relative to
//  a 16-bit sample, even if source is 8-bit (I.E. if squelch level
//  should be 25%, sqLevel must be 32768 * .25 = 8192)
//
STDMETHODIMP_(void)
cSndSample::Squelch( int32, uint32, eSndSquelchMode )
{
   // ALERT! THIS FUNCTION IS NOT SUPPORTED!
   assert( 0 );
}


////////////////////////////////////
// SamplesToTime
//
// Return the # of milliseconds given a sample count
//
STDMETHODIMP_(uint32)
cSndSample::SamplesToTime( uint32      samples )
{
   float t;
   uint32 milliSeconds;

   t = ((float) samples) / ((float) mAttribs.sampleRate);
   t *= 1000.0;
   milliSeconds = t;

   return milliSeconds;
}


////////////////////////////////////
// TimeToSamples
//
// Return the # of samples given a time in milliseconds
//
STDMETHODIMP_(uint32)
cSndSample::TimeToSamples( uint32      milliSeconds )
{
   float t;
   uint32 nSamples;

   t = ((float) milliSeconds) / 1000.0;
   t *= ((float) mAttribs.sampleRate);
   nSamples = t;

   return nSamples;
}


#ifdef __WATCOMC__
#pragma off(unreferenced)
#endif


//
// return a fade task, allocating it if necessary
//
cSndFadeTask
*cSndSample::GetFader( eSndFaders whichFader )
{
   cSndFadeTask *pFader = mFadeTasks[whichFader];

   if( pFader == NULL ) {
      switch( whichFader ) {
         case kSndFadeVolume:    pFader = new cSndVolumeFadeTask(this);    break;
         case kSndFadePan:       pFader = new cSndPanFadeTask(this);       break;
         case kSndFadeFrequency: pFader = new cSndFrequencyFadeTask(this); break;
      }
      mFadeTasks[whichFader] = pFader;
   }
   return pFader;
}


////////////////////////////////////
//
// FadeActive - return true if the specified fade task is in progress
//

STDMETHODIMP_(BOOL) cSndSample::FadeActive( eSndFaders whichFader )
{
   if ( mFadeTasks[whichFader] == NULL ) {
      // it can't be active if it doesn't exist
      return FALSE;
   }
   cSndFadeTask* fader = GetFader( whichFader );
   return fader->Active();
}


////////////////////////////////////
//
// Fade - start the specified parameter fade
//

STDMETHODIMP_(void) cSndSample::Fade(eSndFaders whichFader, int32 destVal, int32 time, eSndFadeFlags flags)
{
   TLOG3( "Smp::Fade %s destval %d, time %d", SAMPLE_NAME, destVal, time );
#ifdef JUST_DO_IT
   // for debugging, to see if timer stuff is causing crashes
   switch( whichFader ) {
      case kSndFadeVolume:    SetVolume( destVal );      break;
      case kSndFadePan:       SetPan( destVal );         break;
      case kSndFadeFrequency: SetFrequency( destVal );   break;
   }
#else
   cSndFadeTask* fader = GetFader( whichFader );

   fader->Fade(destVal, time, flags);
   // start the mixer timer, since it will prod the faders to update
   mpMixer->StartTimer();
#endif
}


#ifdef __WATCOMC__
#pragma on(unreferenced)
#endif


//
// Add ourselves to the head of indicated list, after
//  removing ourselves from previous list (if any)
//
void
cSndSample::MoveToList( cSndSample **ppHead )
{
   if ( ppHead == mpMixer->AudibleHead() ) {
      mNumAudible++;
   } else {
      mNumInaudible++;
   }
   TLOG3("Smp::MoveToList %s %d audible %d inaudible",
        SAMPLE_NAME, mNumAudible, mNumInaudible );

   // if we were on a list, remove ourselves from it
   RemoveFromList();

   // insert ourselves at head of playlist
   mpPrev = NULL;               // we have no predecessor
   mpNext = *ppHead;
   if( mpNext ) {
      // we are predecessor of former play list head
      mpNext->mpPrev = this;
   }
   *ppHead = this;
   SetFlags( kSndFlagOnList );
}

//
// Remove ourselves from whatever list we are in
// NOTE: kSndFlagAudible must be set telling which list sample is in
//
void
cSndSample::RemoveFromList()
{
   cSndSample **ppHead;

   // bail if not on any list
   if ( (mStateFlags & kSndFlagOnList) == 0 ) {
      return;
   }
   ClearFlags( kSndFlagOnList );

   if ( IS_AUDIBLE ) {
      ppHead = mpMixer->AudibleHead();
      mNumAudible--;
   } else {
      ppHead = mpMixer->InaudibleHead();
      mNumInaudible--;
   }
   TLOG3("Smp::RemoveFromList %s %d audible %d inaudible",
        SAMPLE_NAME, mNumAudible, mNumInaudible );

   if ( *ppHead == this ) {
      // we were the playlist head - make our successor the new head
      *ppHead = mpNext;
   } else {
      // unlink ourselves from the successor chain
      if ( mpPrev != NULL ) {
         mpPrev->mpNext = mpNext;
      }
   }

   if ( mpNext != NULL ) {
      // unlink ourselves from the predecessor chain
      mpNext->mpPrev = mpPrev;
   }

   // prevent problems with repeated calls to RemoveFromList
   mpPrev = NULL;
   mpNext = NULL;
}


//
// RegisterFillCallback - register callback app uses to refill double buffer
//
STDMETHODIMP_(void) cSndSample::RegisterFillCallback( SndFillCallback newCB, void *pCBData )
{
   fnFillCB = newCB;
   mpFillCBData = pCBData;
}


//
// RegisterEndCallback - register callback which is called when the sample
//   either finishes playing on its own, or is explicitly stopped.
//
STDMETHODIMP_(void) cSndSample::RegisterEndCallback( SndEndCallback newCB, void *pCBData )
{
   fnEndCB = newCB;
   mpEndCBData = pCBData;
}


//
// RegisterPreemptCallback - register callback which is called when
//   a sample is Preempt()ed - this callback must either Mute or Stop
//   the sample - it must somehow free its DirectSoundBuffer
//
STDMETHODIMP_(void)
cSndSample::RegisterPreemptCallback( SndEndCallback   newCB,
                                     void             *pCBData )
{
   fnPreemptCB = newCB;
   mpPreemptCBData = pCBData;
}


//
// RegisterFadeCallback - register callback which is called when the specified
//   fade finishes
//
STDMETHODIMP_(void) 
cSndSample::RegisterFadeCallback( eSndFaders whichFader, SndEndCallback newCB, void *pCBData )
{
   cSndFadeTask* pFader = GetFader( whichFader );
   pFader->RegisterEndCallback( newCB, pCBData );
}


//
// start or stop dumping raw data to a file (for debugging)
//
STDMETHODIMP_(void)
cSndSample::DumpData( BOOL turnOn )
{
   char fname[32];

   if ( turnOn ) {
      if ( mDumpFile == NULL ) {
         // open output file
         sprintf( fname, "snd%d.raw", mSerialNum );
         mDumpFile = fopen( fname, "wb" );
      }
   } else {
      if ( mDumpFile != NULL ) {
         // close output file
         fclose( mDumpFile );
         mDumpFile = NULL;
      }
   }
}


//
// return the estimated remaining playtime in milliseconds
//
STDMETHODIMP_(uint32)
cSndSample::PlayTimeRemaining()
{
   uint32 milliSecs, curPos;
   float sampsLeft;

   switch ( mState ) {
      case kSndStatePlaying:
         if ( IS_PAUSED ) {
            milliSecs = ~0;
         } else {
            // estimate time remaining
            curPos = GetPosition();
            if ( curPos >= mNumSamples ) {
               milliSecs = 0;
            } else {
               sampsLeft = mNumSamples - curPos;
               if ( mFrequency > 0 ) {
                  milliSecs = (1000.0 * sampsLeft) / (float) mFrequency;
               } else {
                  milliSecs = ~0;
               }
            }
         }
         break;

      case kSndStateStopped:
         milliSecs = 0;
         break;

      default:
         milliSecs = ~0;
         break;
   }

   return milliSecs;
}


//
// set the sample name (used in logging/debugging)
//
STDMETHODIMP_(void)
cSndSample::SetName( char *pUserName )
{
#ifdef DO_TIME_LOGGING
   sprintf( mpName, "{%s}[%ld:%ld]", pUserName, mGroup, mSerialNum );
#endif
}


//
// return the sample name
// this violates COM to some extent but what the heck
//
STDMETHODIMP_(char *)
cSndSample::GetName( void )
{
   return mpName;
}


/************************************************************
 *
 *   3  D      S  T  U  F  F
 *
 *
 */

////////////////////////////////////
//
// Get3DPosition
//

STDMETHODIMP_(void)
cSndSample::Get3DPosition( sSndVector *pPosition )
{
   assert( pPosition != NULL );
   *pPosition = m3DPosition;
}


////////////////////////////////////
//
// Get3DVelocity
//

STDMETHODIMP_(void)
cSndSample::Get3DVelocity( sSndVector *pVelocity )
{
   assert( pVelocity != NULL );
   *pVelocity = m3DVelocity;
}


////////////////////////////////////
//
// Get3DConeAngles
//

STDMETHODIMP_(void)
cSndSample::Get3DConeAngles( uint32 *pInside, uint32 *pOutside )
{
   assert( pInside != NULL );
   assert( pOutside != NULL );
   *pInside = m3DConeInnerAngle;
   *pOutside = m3DConeOuterAngle;
}


////////////////////////////////////
//
// Get3DOrientation
//

STDMETHODIMP_(void)
cSndSample::Get3DConeOrientation( sSndVector *pOrientation )
{
   assert( pOrientation != NULL );
   *pOrientation = m3DConeOrientation;
}


////////////////////////////////////
//
// Get3DDistanceRange
//

STDMETHODIMP_(void)
cSndSample::Get3DDistanceRange( float *pMin, float *pMax )
{
   assert( pMin != NULL );
   assert( pMax != NULL );
   *pMin = m3DMinDistance;
   *pMax = m3DMaxDistance;
}


////////////////////////////////////
//
// Get3DMode
//

STDMETHODIMP_(void)
cSndSample::Get3DMode( eSnd3DMode *pMode )
{
   assert( pMode != NULL );
   *pMode = m3DMode;
}


////////////////////////////////////
//
// Get3DMethod
//

STDMETHODIMP_( eSnd3DMethod )
cSndSample::Get3DMethod( void )
{
   return m3DMethod;
}


////////////////////////////////////
//
// GetAmbientVolume
// 
//

STDMETHODIMP_(int32)
cSndSample::GetAmbientVolume( void )
{
   return mAmbientVolume;
}


//
// debug aid
//
void
cSndSample::TBD( char *message)
{
   TLOG1( "Sample TBD: %s\n", ((long) message) );
#ifndef SHIP   
   mprintf( "Sample TBD: %s\n", message );
#endif   
}
