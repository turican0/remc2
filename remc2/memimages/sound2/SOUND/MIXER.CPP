////////////////////////////////////////////////////////////////////////
// $Source: x:/prj/tech/libsrc/sound/RCS/mixer.cpp $
// $Author: PATMAC $
// $Date: 1998/03/25 16:54:53 $
// $Revision: 1.47 $
//
// (c) 1996 Looking Glass Technologies Inc.
// Pat McElhatton (from JohnB)
//
// Module name: Mixer
// File name: mixer.cpp
//
// Description: Top-level controller of digital sounds
//
////////////////////////////////////////////////////////////////////////

#include <windows.h>

#include <assert.h>

#include <lgsndi.h>

#include <comtools.h>

#include <appagg.h>
#include <wappapi.h>
#include <wdispapi.h>
#include <dispapi.h>

#include <dsnddynf.h>
#include <mprintf.h>
#include <dbg.h>
#include <matrix.h>
#include <math.h>

#include <mixerlck.h>
// @Note (toml 05-17-96):  Sometime relatively soon the mixer should be converted into a true
// member of the application aggregate object


// prototype for internally used callback.  These are cdecl for now.
extern "C" {
static ISndSample * StopSample(ISndMixer *, ISndSample *sample, void *);
};

// this is how often timer interrupts get generated
const int kTmBestResolution = 10;

// if fade timer callback doesn't happen by this time, assume timers are broken
const int kTmFadeStartTimeout = 6000;  // in milliseconds

uint32 cSndMixer::mTimerId = 0;
BOOL cSndMixer::mTimerNeeded = FALSE;
TIMECAPS cSndMixer::mTimerCaps = {0};

#ifdef DO_PERF_METERS
PerfMeter lgsoundThreadPM;
#endif

#define MIXER_MUTEX \
  cMixerAutoLock __mixer_lock__( mMutex )

#define SAMPLE_NAME(SMP)  ((long) (SMP->GetName()))


////////////////////////////////////
// IMPLEMENT_UNAGGREGATABLE
//
// This does all the implementations of IUnknown methods
//

IMPLEMENT_UNAGGREGATABLE_SELF_DELETE(cSndMixer, ISndMixer);

////////////////////////////////////
// cSndMixer contructor
//
// This just sets everything to 0
//

cSndMixer::cSndMixer()
{
   int group;
   sSndVector origin;

   mNumMixerChans = 0;
   fnPriorityCB = NULL;
   mTimerId = 0;
   mpThread = NULL;
   mTimeoutMillisecs = ~0;
   fnFreeChannelCB = NULL;
   fnTraceCB = NULL;
   mpDisplayDevice = NULL;
   mpWinDisplayDevice = NULL;
   mpInaudibleListHead = NULL;
   mpAudibleListHead = NULL;

   m3DMixMethod = kSnd3DMethodNone;
   origin.x = 0.0;
   origin.y = 0.0;
   origin.z = 0.0;
   m3DPosition = origin;
   // default orientation - looking along positive x axis, z is up
   //  rig derived classes so that for this orientation, y axis is out of right ear
   m3DTopVector = origin;
   m3DTopVector.z = 1.0;
   m3DFrontVector = origin;
   m3DFrontVector.x = 1.0;
   m3DVelocity = origin;
   m3DEnvironment.dopplerFactor = 1.0;
   m3DEnvironment.distanceFactor = 1.0;
   m3DEnvironment.rolloffFactor = 1.0;

   for ( group = 0; group < (kSndNumGroups+1); group++ ) {
      mpGroupFades[group] = NULL;
   }
}


////////////////////////////////////
// cSndMixer destructor
//
// Destroy the primary sample buffer
// And free up the sample memory pool

cSndMixer::~cSndMixer()
{
   int blockCookie = BlockDisplay();
   ReleaseDisplay(blockCookie);
   // inform nosy app about death of mixer
   DoTrace( NULL, kSndMixerDestroyed );

   SafeRelease( mpDisplayDevice );
   SafeRelease( mpWinDisplayDevice );
}

void
cSndMixer::Destroy( void )
{
   int group;
   cSndSample *pSample, *pNext;

   // kill the fade timer if needed
   mTimerNeeded = FALSE;
   CheckTimer();

   // kill the mixer update thread
   if ( mpThread != NULL ) {
      mpThread->CallWorker( kThreadExit );
      mpThread->WaitForClose();
      delete mpThread;
   }

   // kill all the group volume fades
   for ( group = 0; group < (kSndNumGroups+1); group++ ) {
      if ( mpGroupFades[group] != NULL ) {
         delete mpGroupFades[group];
      }
   }

   // destroy all samples
   fnFreeChannelCB = NULL;
   pSample = mpAudibleListHead;
   while ( pSample != NULL ) {
      pNext = pSample->Next();
      delete pSample;
      pSample = pNext;
   }
   pSample = mpInaudibleListHead;
   while ( pSample != NULL ) {
      pNext = pSample->Next();
      delete pSample;
      pSample = pNext;
   }

}


////////////////////////////////////
// RegisterPriorityCallback
//
// Store a Prioirty callback.  The callback will get called
// when an application tries to play a sound and there are none available
//

STDMETHODIMP_(void)
cSndMixer::RegisterPriorityCallback( SndPriorityCallback func,
                                     void                *pCBData)
{
   TLOG0( "Mix::RegisterPriorityCallback" );
   fnPriorityCB = func;
   mpPriorityCBData = pCBData;
}

////////////////////////////////////
// SetMasterVolume
//
// Stores the new setting and scales all sound volumes appropriately
// returns the previous setting
//

STDMETHODIMP_(int32) cSndMixer::SetMasterVolume(int32 vol)
{
   uint32 group;
   cSndSample *pSample;

   int32 old = mMasterVolume;
   mMasterVolume = vol;
   TLOG1( "Mix::SetMasterVolume %d", vol );

   // update the group+master volume for all samples
   for ( pSample = mpAudibleListHead; pSample != NULL; pSample = pSample->Next() ) {
      group = pSample->GetGroupQuick();
      pSample->SetGroupVolume( mGroupVolumes[group] + mMasterVolume );
   }

   for ( pSample = mpInaudibleListHead; pSample != NULL; pSample = pSample->Next() ) {
      group = pSample->GetGroupQuick();
      pSample->SetGroupVolume( mGroupVolumes[group] + mMasterVolume );
   }

   return old;
}

////////////////////////////////////
// GetMasterVolume
//
// Retrieve the current master volume setting
//

STDMETHODIMP_(int32) cSndMixer::GetMasterVolume()
{
   return mMasterVolume;
}

////////////////////////////////////
// SetGroupVolume
//
// Set the volume offset for all samples in a group
// returns the previous setting
//

STDMETHODIMP_(int32)
cSndMixer::SetGroupVolume( uint32   group,
                           int32    vol )
{
   cSndSample *pSample;
   int32 old;

   TLOG2( "Mix:SetGroupVolume group %d vol %d", group, vol );

   if ( group == kSndMasterGroup ) {
      return SetMasterVolume ( vol );
   }

   if ( group == kSndAnyGroup ) {
      // set ALL group volumes
      for ( group = 0; group < kSndNumGroups; group++ ) {
         SetGroupVolume( group, vol );
      }
      return 0;
   }

   assert( group < kSndNumGroups );
   old = mGroupVolumes[group];
   mGroupVolumes[group] = vol;

   // pass volume change to all samples which are audible
   for ( pSample = mpAudibleListHead; pSample != NULL; pSample = pSample->Next() ) {
      if ( pSample->GetGroupQuick() == group ) {
         pSample->SetGroupVolume( vol + mMasterVolume );
      }
   }

   return old;
}


////////////////////////////////////
// GetGroupVolume
//
// Retrieve the current volume setting for a group
//

STDMETHODIMP_(int32)
cSndMixer::GetGroupVolume( uint32 group )
{
   if ( group == kSndMasterGroup ) {
      return GetMasterVolume();
   } else {
      assert( group < kSndNumGroups );

      return mGroupVolumes[group];
   }
}


////////////////////////////////////
// FadeGroupVolume
//
// start a fade of volume of a sound group
//
STDMETHODIMP_(void)
cSndMixer::FadeGroupVolume( uint32         group,
                            int32          newVolume,
                            int32          time,
                            eSndFadeFlags  flags )
{
   cSndGroupVolumeFadeTask    *pFade;

   TLOG3( "Mix::FadeGroupVolume group %d volume %d time %d", group, newVolume, time );
   if ( group == kSndMasterGroup ) {
      // the master volume fade is in the last (extra) element of mpGroupFades
      group = kSndNumGroups;
   } else {
      assert( group < kSndNumGroups );
   }

   pFade = mpGroupFades[group];
   if ( pFade == NULL ) {
      // need to create the fade task
      pFade = new cSndGroupVolumeFadeTask( group, this );
      mpGroupFades[group] = pFade;
   }

   pFade->Fade( newVolume, time, flags );
   mGroupFadesActive = TRUE;
   StartTimer();
}


////////////////////////////////////
// RegisterGroupFadeCallback
//
// Store an end-of-group-fade callback & its data
//
STDMETHODIMP_(void)
cSndMixer::RegisterGroupFadeCallback( uint32          group,
                                      SndEndCallback  endFunc,
                                      void            *pCBData )
{
   if ( group == kSndMasterGroup ) {
      // the master volume fade is in the last (extra) element of mpGroupFades
      group = kSndNumGroups;
   } else {
      assert( group < kSndNumGroups );
   }

   if ( mpGroupFades[group] == NULL ) {
      mpGroupFades[group] = new cSndGroupVolumeFadeTask( group, this );
   }
   mpGroupFades[group]->RegisterEndCallback( endFunc, pCBData );
}


////////////////////////////////////
// CreateSampleFromRez - create a sample from a sound resource (memory image of sound file)
//
// Takes:   flags -- describes what kind of sample is desired
//          buf -- a pointer to the sound resource - must be a wave or voc file
//          len -- length of the raw data
// Returns: An ISndSample * that is ready to go.
//

STDMETHODIMP_(ISndSample *)
cSndMixer::CreateSampleFromRez(
                               eSndSampleCreateFlagSet   flags,
                               void                      *pRezData,
                               uint32                    len )
{
   sSndAttribs attribs;
   void *pRawData;
   uint32 rawDataLen;
   uint32 nSamples;

   TLOG0( "Mix::CreateSampleFromRez" );
   if ( SndCrackRezHeader( pRezData, len, &pRawData, &rawDataLen,
                           &nSamples, &attribs )  ) {
      return NULL;
   }
   // create the actual sample...
   return CreateRawSample( flags, pRawData, rawDataLen, nSamples, &attribs );
}

////////////////////////////////////
// AllocChannel
//
// Allocate a DirectSound channel, call priority func
// to free one if none available
// Returns TRUE if allocation succeeds
//

BOOL
cSndMixer::AllocChannel( ISndSample    *pSample )
{
   if ( mNumFreeMixerChans == 0 ) {

      // no channels so see if there is a priority function
      if ( fnPriorityCB == NULL ) {
         TLOG1( "Mix::AllocChannel %s, no priority CB!", SAMPLE_NAME( pSample ) );
         return FALSE;
      }
      if ( (*fnPriorityCB)( this, pSample, mpPriorityCBData ) == FALSE ) {
         // couldn't free a channel
         TLOG1( "Mix::AllocChannel %s, priority CB failed!", SAMPLE_NAME( pSample ) );
         return FALSE;
      }
   }

   mNumFreeMixerChans--;
   TLOG2( "Mix::AllocChannel %s, %d left", SAMPLE_NAME( pSample ), mNumFreeMixerChans );
   return TRUE;
}


////////////////////////////////////
// FreeChannel
//
// Increment the free channel count
//

void
cSndMixer::FreeChannel()
{
   mNumFreeMixerChans++;
   TLOG1( "Mix::FreeChannel %d left", mNumFreeMixerChans );
   if ( fnFreeChannelCB ) {
      // tell app that there is a free channel
      (*fnFreeChannelCB)( this, mpFreeChannelCBData );
   }
}


////////////////////////////////////
// FreeChannelCount
//
// How many channels are now available for allocation
//

STDMETHODIMP_(uint32) cSndMixer::FreeChannelCount()
{
   return mNumFreeMixerChans;
}


////////////////////////////////////
// Update
//
// How to keep everything moving along.  This is how we deal
// with things like finding samples that were playing but aren't anymore.
//

STDMETHODIMP_(void) cSndMixer::Update()
{
   TLOG0( "Mix::Update" );

   DoTrace( NULL, kSndMixerUpdateStart );
   // find out who is done
   {
      MIXER_MUTEX;
      CleanUpFinishedSamples();
   }

   // refill stream ring buffers
   if ( mpThread != NULL ) {
      // tell stream refill thread to do stream update
#ifndef SHIP
      if ( mpThread->CallWorker( kThreadUpdate )==E_FAIL)
         mprintf("App called update in a mixer mutex, kThreadUpdate failed\n");
#else
      mpThread->CallWorker( kThreadUpdate );
#endif      
   } else {
      // threading is off - do stream update ourselves
      CheckStreams();
   }

   // update timer (for crossfades & streams)
   CheckTimer();
   if ( mTimerNeeded && (mTimerState == eSndTimerBroken) ) {
      //
      // do a fake mixer timer callback, which updates faders
      //
      TimerCallback( 0, 0, (DWORD) this, 0, 0 );
      // TimerCallback will set timer state to working
      mTimerState = eSndTimerBroken;
   }
   DoTrace( NULL, kSndMixerUpdateEnd );
   TLOG0( "Mix::Update - done" );
}

////////////////////////////////////
// StopSample
//
// Internally used callback for StopAllSamples
//

#ifdef __WATCOMC__
#pragma off(unreferenced)
#endif

ISndSample *
StopSample( ISndMixer   *mixer,
            ISndSample  *sample,
            void        *pCBD )
{
   sample->Stop();
   return NULL;
}

#ifdef __WATCOMC__
#pragma on(unreferenced)
#endif

/////////////////////////////////////
// StopAllSamples
//
// Stops all currently playing samples
// uses our interator mechanism.

STDMETHODIMP_(void) cSndMixer::StopAllSamples()
{
   TLOG0( "Mix::StopAllSamples" );
   DoForAllSamples( StopSample, NULL );
}


/////////////////////////////////////
//
//   Inform
//
//    This is a way for cSndSamples to report to their cSndMixer as to
//  what action they are taking.  We add or remove them to the play list
//  and perform callbacks if needed
//

void
cSndMixer::Inform( cSndSample       *pSample,
                   eSndTraceEvent action )
{
   DoTrace( pSample, action );
}

/////////////////////////////////////
//
//   DoForAllSamples
//
//   A general purpose iterator for all currently playing sounds.
// Takes a callback function and a callback data ptr
//

STDMETHODIMP_(void)
cSndMixer::DoForAllSamples( SndMixerCallback    func,
                            void                *pCBData )
{
   cSndSample *pSample, *pNextSample;

   // TBD: do both audible & inaudible list, do not do for state == stopped?
   TLOG0( "Mix::DoForAllSamples - audible list" );
   pSample = mpAudibleListHead;
   while ( pSample != NULL ) {
      pNextSample = pSample->Next();
      (*func)(this, pSample, pCBData);
      pSample = pNextSample;
   }

   TLOG0( "Mix::DoForAllSamples - inaudible list" );
   pSample = mpInaudibleListHead;
   while ( pSample != NULL ) {
      pNextSample = pSample->Next();
      (*func)(this, pSample, pCBData);
      pSample = pNextSample;
   }
   TLOG0( "Mix::DoForAllSamples - done" );
}

/////////////////////////////////////
//
//   DoForGroupSamples
//
//   A general purpose iterator for all samples in a group.
// Takes a callback function a sample type (or kSndAnyType for all types)
//

STDMETHODIMP_(void)
cSndMixer::DoForGroupSamples( uint32            group,
                              SndMixerCallback  func,
                              void              *pCBData )
{
   cSndSample *pSample, *pNextSample;
   BOOL doAllSamples;

   // TBD: do both audible & inaudible list, do not do for state == stopped?
   TLOG1( "Mix::DoForGroupSamples group %d - audible list", group );
   doAllSamples = ( (group == kSndAnyGroup) || (group == kSndMasterGroup) );
   pSample = mpAudibleListHead;
   while ( pSample != NULL ) {
      pNextSample = pSample->Next();
      if ( doAllSamples || (pSample->GetGroupQuick() == group) ) {
         (*func)(this, pSample, pCBData);
      }
      pSample = pNextSample;
   }

   TLOG1( "Mix::DoForGroupSamples group %d - inaudible list", group );
   pSample = mpInaudibleListHead;
   while ( pSample != NULL ) {
      pNextSample = pSample->Next();
      if ( doAllSamples || (pSample->GetGroupQuick() == group) ) {
         (*func)(this, pSample, pCBData);
      }
      pSample = pNextSample;
   }

   TLOG1( "Mix::DoForGroupSamples group %d - done", group );
}

/////////////////////////////////////
//
// First
//
// Returns the first sample of a given type that is playing.
// Order is arbitrary.  This is used for general iterations.

STDMETHODIMP_(ISndSample *)
cSndMixer::First( uint32         group,
                  eSndFlagSet    tFlags,
                  eSndFlagSet    fFlags )
{
   TLOG3( "Mix::First group %d trueFlags 0x%x falseFlags 0x%x",
         group, tFlags, fFlags );

   if ( group == kSndMasterGroup ) {
      mIterGroup = kSndAnyGroup;
   } else {
      mIterGroup = group;
   }
   mIterTFlags = tFlags;
   mIterFFlags = fFlags;
   if ( mIterFFlags & kSndFlagAudible ) {
      // user has specified to look only for inaudible samples
      mIterWhichList = 1;
      mpIterNext = mpInaudibleListHead;
   } else {
      // else start the search in the audible list
      mIterWhichList = 0;
      mpIterNext = mpAudibleListHead;
   }
   return Next();
}

/////////////////////////////////////
//
// Next
//
// Returns the next sample of the given type that matches
//  conditions set by First call
//

STDMETHODIMP_(ISndSample *)
cSndMixer::Next()
{
   cSndSample     *pSample;
   eSndFlagSet    flags;

   //
   // find next sample in list
   //
   pSample = mpIterNext;
   while ( mIterWhichList < 2 ) {

      if ( pSample != NULL ) {

         if ( (mIterGroup == kSndAnyGroup)
              || (mIterGroup == pSample->GetGroupQuick()) ) {
            flags = pSample->GetFlags();
            if ( ((flags & mIterTFlags) == mIterTFlags)
                 && ((flags & mIterFFlags) == 0) ) {
               //
               // we have found the chosen one, rejoice!
               //
               mpIterNext = pSample->Next();
               TLOG1( "Mix::Next %s", SAMPLE_NAME( pSample ) );
               return pSample;
            }
         }
         pSample = pSample->Next();

      } else {

         // the current list (audible or inaudible) is done, switch to next
         mIterWhichList++;
         if ( (mIterWhichList == 1)
              && ((mIterTFlags & kSndFlagAudible) == 0) ) {
            // done with audible list, switch to inaudible list
            pSample = mpInaudibleListHead;
         }

      }

   }

   TLOG0( "Mix::Next - done" );
   mpIterNext = NULL;

   return pSample;
}


/////////////////////////////////////
//
// CleanUpFinishedSamples.
//
// This gets called from inside cSndMixer::Update() to
// iterate throught the play list and find samples that arent really playing anymore.
// Calling stop in turn calls our inform which does the callback.

void cSndMixer::CleanUpFinishedSamples()
{
   cSndSample *pSample = mpAudibleListHead;
   cSndSample *pNextSample;
   eSndFlagSet    flags;

   while ( pSample != NULL ) {

      // get next sample ptr now, because sample may be destroyed
      //  inside Stop() end-of-sample callback by app
      pNextSample = pSample->Next();
      flags = pSample->GetFlags();
      // stop sample if requested, or if sample is done playing
      if ( (flags & kSndFlagPleaseStop) || 
           (!pSample->IsPlaying() && ((flags & kSndFlagPaused) == 0))  ) {

         pSample->Stop();

      }

      pSample = pNextSample;
   }
}

/////////////////////////////////////
//
// CheckStreams
//
// This gets called from Update to check the buffers of double buffered
// samples and see if they should be done
//
// returns TRUE if any streams were updated, FALSE if there are no streams

BOOL
cSndMixer::CheckStreams(void)
{
   cSndSample *pSample;
   BOOL  foundStream = FALSE;

   for( pSample = mpAudibleListHead; pSample != NULL; pSample = pSample->Next() ) {
      if( pSample->IsStream() ) {
         pSample->CheckStream();
         foundStream = TRUE;
      }
   }

   return foundStream;
}


////////////////////////////////////
// RegisterFreeChannelCallback
//
// Register a FreeChannel callback.  The callback will get called
// when a DirectSound channel is freed.
//

STDMETHODIMP_(void)
cSndMixer::RegisterFreeChannelCallback( SndFreeChannelCallback func,
                                        void                   *pCBData)
{
   fnFreeChannelCB = func;
   mpFreeChannelCBData = pCBData;
}


////////////////////////////////////
// RegisterTraceCallback
//
// Register a callback which is called when something important
//  happens to a sample, or when the mixer is destroyed.
//

STDMETHODIMP_(void)
cSndMixer::RegisterTraceCallback( SndTraceCallback       func )
{
   fnTraceCB = func;
}


////////////////////////////////////
//
// GetAttribs - return the samples attributes
//

STDMETHODIMP_(void)
cSndMixer::GetAttribs( sSndAttribs *pAttribs )
{
   assert( pAttribs != NULL );

   *pAttribs = mPrimaryAttribs;
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
cSndMixer::Get3DPosition( sSndVector *pPosition )
{
   assert( pPosition != NULL );
   *pPosition = m3DPosition;
}


////////////////////////////////////
//
// Get3DOrientation
//

STDMETHODIMP_(void)
cSndMixer::Get3DOrientation( sSndVector *pFront, sSndVector *pTop )
{
   assert( pFront != NULL );
   assert( pTop != NULL );
   *pFront = m3DFrontVector;
   *pTop = m3DTopVector;
}


////////////////////////////////////
//
// Get3DVelocity
//

STDMETHODIMP_(void)
cSndMixer::Get3DVelocity( sSndVector *pVelocity )
{
   assert( pVelocity != NULL );
   *pVelocity = m3DVelocity;
}


////////////////////////////////////
//
// Get3DEnvironment
//

STDMETHODIMP_(void)
cSndMixer::Get3DEnvironment( sSndEnvironment *pEnv )
{
   assert( pEnv != NULL );
   *pEnv = m3DEnvironment;
}


uint32
cSndMixer::Get3DDeferFlag()
{
   return m3DDeferFlag;
}


// stuff for converting distance to volume offset
static int32 milliBels[] =
{
   0,    0,    600,  950,  1200, 1393, 1550, 1684,
   1800, 1901, 1993, 2075, 2150, 2220, 2284, 2344,
   2400, 2452, 2501, 2548, 2593, 2635, 2675, 2714,
   2750, 2786, 2820, 2852, 2884, 2914, 2944, 2972,
   3000, 3026, 3052, 3077, 3101, 3125, 3148, 3171,
   3193, 3214, 3235, 3255, 3275, 3295, 3314, 3332,
   3350, 3368, 3386, 3403, 3420, 3436, 3452, 3468,
   3484, 3499, 3514, 3529, 3544, 3558, 3572, 3586,
   3600
};

// this constant is used to approximately match the volume level picked by
//  our panvol calculation to that of DS3D for the same src-listener distance
#define FUNKY_3D_VOL_OFFSET 780

//
// convert distance to volume offset
//  distance units are arbitrary - pick your unit such
//  that for a source at 1 unit from listener, volume offset is 0 dB
//
static int32
DistanceToMilliBels(int32 distance)
{
   int mB;

   mB = 0;
   // scale distance to be <= 64
   //  subtract 6 dB for each doubling of distance
   while ( distance > 64 ) {
      if ( distance > 256 ) {
         if ( distance > 512 ) {
            distance >>= 4;
            mB -= 2400;
         } else {
            distance >>= 3;
            mB -= 1800;
         }
      } else {
         if ( distance > 128 ) {
            distance >>= 2;
            mB -= 1200;
         } else {
            distance >>= 1;
            mB -= 600;
         }
      }
   }
   // now just lookup milliBel offset for scaled distance
   mB -= milliBels[ distance ];

   return (mB - FUNKY_3D_VOL_OFFSET);
}

#ifndef PI
#define PI 3.14159265359
#endif

//
// this is the volume offset for the left ear, at the compass points N, E, S, W
//  where N represents a source directly in front of listener, W is a source
//  which is aligned with the left ear
static float volOff[] = { 0.0, -1530.0, -370.0, 320.0 };

////////////////////////////////////
//
// Get3DDistanceVolume - return volume offset for given distance
//

STDMETHODIMP_(int32)
cSndMixer::Get3DDistanceVolume( float range )
{
   int32 dist, vol;

   dist = range * m3DEnvironment.distanceFactor;

   // return volume offset for range
   // note: returned values do not take into account group & sample volume
   vol = DistanceToMilliBels( dist );
   TLOG2( "Mix::Get3DDistanceVolume distance %d volume %d", dist, vol );

   return vol;
}


////////////////////////////////////
//
// Get3DDistanceFromVolume - return distance from listener for given volume offset
//

STDMETHODIMP_(float)
cSndMixer::Get3DDistanceFromVolume( int32 vol )
{
   float dist;

   dist = pow( 2.0, ((float)-vol) / 600.0);
   if ( m3DEnvironment.distanceFactor != 0.0 ) {
      dist /= m3DEnvironment.distanceFactor;
   }

   TLOG2( "Mix::Get3DDistanceFromVolume distance %d volume %d", dist, vol );
   // note: returned values do not take into account group & sample volume
   return dist;
}


////////////////////////////////////
//
// Get3DPositionVolume - return volume offset for given position
//

STDMETHODIMP_(int32)
cSndMixer::Get3DPositionVolume( sSndVector  *pSrcPos )
{
   float range;
   mxs_vector delta;
   int32 vol;


   mx_sub_vec( &delta, (mxs_vector *) pSrcPos, (mxs_vector *) &m3DPosition );
   range = mx_mag_vec( &delta ) * m3DEnvironment.distanceFactor;

   // return volume offset for range
   // note: returned values do not take into account group & sample volume
   vol = DistanceToMilliBels( range );
   TLOG1( "Mix::Get3DPositionVolume volume %d", vol );

   return vol;
}


////////////////////////////////////
//
// Get3DPositionPanVolume - return pan and volume offset for given position
//

STDMETHODIMP_(void)
cSndMixer::Get3DPositionPanVolume( sSndVector  *pSrcPos,
                                   int32       *pPan,
                                   int32       *pVol )
{
   float sinHeading, cosHeading, range, a, heading;
   mxs_vector delta, crossProd;
   int32 leftVolume, rightVolume;

   // to find the relative heading to the source from the listener orientation,
   // we get the cosine and sine of the heading angle from the dot & cross products
   // and use arctangent to convert this into an angle

   //mprintf("src: %g %g %g\n", pSrcPos->x, pSrcPos->y, pSrcPos->z );
   //mprintf("listener orientation: %g %g %g\n",
   //        m3DFrontVector.x, m3DFrontVector.y, m3DFrontVector.z );

   mx_sub_vec( &delta, (mxs_vector *) pSrcPos, (mxs_vector *) &m3DPosition );
   TLOG3( "Mix::Get3DPositionPanVolume dx %d dy %d dz %d",
          delta.x, delta.y, delta.z );
   range = mx_mag_vec( &delta ) * m3DEnvironment.distanceFactor;
   mx_normeq_vec( &delta );

   cosHeading = mx_dot_vec( &delta, (mxs_vector *) &m3DFrontVector );
   mx_cross_vec( &crossProd, &delta, (mxs_vector *) &m3DFrontVector );
   sinHeading = mx_mag_vec( &crossProd );
   if ( crossProd.z < 0.0 ) {
      // the magnitude of the cross product will always be positive, look at
      //  the z coordinate to give us full range of sine
      // NOTE: this is wrong in many cases, but will work if the source and
      //  listener are basically in the same plane... I think...
      sinHeading = (-sinHeading);
   }

   heading = atan2( sinHeading, cosHeading ) / PI;
   //mprintf("heading %g sin %g cos %g\n", heading, sinHeading, cosHeading);
   // compute volume offset at left & right ear based on heading
   // get value by linear interpolating from table of volume offsets
   //   for source at the 4 compass points
   if ( heading >= 0.0 ) {
      if ( heading < 0.5 ) {
         a = heading / 0.5;
         leftVolume = (a * volOff[1]) + ((1.0 - a) * volOff[0]);
         rightVolume = (a * volOff[3]) + ((1.0 - a) * volOff[0]);
      } else {
         a = (heading / 0.5) - 1.0;
         leftVolume = (a * volOff[2]) + ((1.0 - a) * volOff[1]);
         rightVolume = (a * volOff[2]) + ((1.0 - a) * volOff[3]);
      }
   } else {
      heading = (-heading);
      if ( heading < 0.5 ) {
         a = heading / 0.5;
         leftVolume = (a * volOff[3]) + ((1.0 - a) * volOff[0]);
         rightVolume = (a * volOff[1]) + ((1.0 - a) * volOff[0]);
      } else {
         a = (heading / 0.5) - 1.0;
         leftVolume = (a * volOff[2]) + ((1.0 - a) * volOff[3]);
         rightVolume = (a * volOff[2]) + ((1.0 - a) * volOff[1]);
      }
   }

   // add in volume offset for range
   *pVol = DistanceToMilliBels( range );

   // TBD: calculate left & right volumes
   if ( leftVolume > rightVolume ) {
      *pVol += leftVolume;
   } else {
      *pVol += rightVolume;
   }
   *pPan = rightVolume - leftVolume;

   //mprintf("leftVol %d rightVol %d\n", leftVolume, rightVolume);
   //mprintf("pan %d vol %d range %g\n", *pPan, *pVol, range );
   TLOG3( "Mix::Get3DPositionPanVolume pan %d volume %d dist %d", *pPan, *pVol, range );

   // note: returned values do not take into account group & sample volume
}


////////////////////////////////////
//
// Set3DDeferMode - set deferred mode
//

STDMETHODIMP_(void)
cSndMixer::Set3DDeferMode( BOOL  deferOn )
{
   m3DDeferFlag = deferOn;
}


/************************************************************
 *
 *   T  I  M  E  R      S  T  U  F  F
 *
 *
 */

#pragma off(unreferenced)
//
// mixer timer callback, which updates faders
//
void CALLBACK
cSndMixer::TimerCallback( UINT nTimerID,
                          UINT uReserved,
                          DWORD dwUser,
                          DWORD dwReserved1,
                          DWORD dwReserved2 )
{

   TLOG_INT_START(1);
   cSndMixer *mixer = (cSndMixer *)dwUser;      // ??????????????????????????
   cSndSample *pSample;
   uint32 group;
   cSndGroupVolumeFadeTask *pFade;
   BOOL fadeActive;
   BOOL onAudibleList;
                             // wait!!! - is this mixer (the cast dwUser) == this?
   cMixerAutoLock __mixer_lock__( mixer->mMutex );

   mixer->mTimerState = eSndTimerWorking;
   mixer->mTimerLastTick = timeGetTime();
   mTimerNeeded = FALSE;

   // TBD: replace all this with scheme where there is a linked list of
   //   fades

   // do group volume fades
   if ( mixer->mGroupFadesActive ) {
      for ( group = 0; group < (kSndNumGroups+1); group++ ) {
         pFade = mixer->mpGroupFades[group];
         if ( pFade != NULL ) {
            if ( pFade->Active() ) {
               // found a live one
               mTimerNeeded = TRUE;
               pFade->DoFadeStep();
            } else {
               // fade was active, but not anymore
               delete pFade;
               mixer->mpGroupFades[group] = NULL;
            }
         }
      } // for all groups
      mixer->mGroupFadesActive = mTimerNeeded;
   }

   // do sample fades (volume, pan & frequency)
   // TBD: reimplement so that entire sample list isn't scanned (big job!)
   onAudibleList = TRUE;
   pSample =  mixer->mpAudibleListHead;
   while ( TRUE ) {
      if ( pSample == NULL ) {
         if ( onAudibleList ) {
            // switch from audible list to inaudible list
            pSample = mixer->mpInaudibleListHead;
            onAudibleList = FALSE;
            if ( pSample == NULL ) {
               // inaudible list is empty, quit
               break;
            }
         } else {
            // done both lists, quit
            break;
         }
      }

      // handle fades
      for( int i = 0; i < kSndNumFaders; i++ ) {
         cSndFadeTask *task = pSample->mFadeTasks[i];

         if ( task != NULL ) {
            fadeActive = task->DoFadeStep();
            if ( fadeActive ) {
               mTimerNeeded = TRUE;
            }
            TLOG2( "Mix::Timer fade %s active %d", ((uint32) pSample->GetName()), fadeActive );
         }
      }

      pSample = pSample->Next();

   }
   TLOG_INT_END(1);
}


#pragma on(unreferenced)

//
// check if timer needs to be started or stopped
//
void
cSndMixer::CheckTimer( void )
{
   int32 resolution;
   AutoAppIPtr(DisplayDevice);
   UINT result;
   BOOL failed = FALSE;

   if ( mTimerNeeded == FALSE ) {


      //
      // timer is not needed
      //
      if ( mTimerId != 0 ) {
         TLOG0( "Mix:CheckTimer off" );
         // timer is active & not needed - kill it
         result = timeKillEvent(mTimerId);
#ifndef SHIP
         if ( result ) {
            mprintf( "!!! timeKillEvent returns %d\n", result );
         }
#endif

         // stop the timer
         result = timeEndPeriod(kFadeGranularity);
#ifndef SHIP
         if ( result ) {
            mprintf( "!!! timeEndPeriod returns %d\n", result );
         }
#endif
         mTimerId = 0;
         mTimerState = eSndTimerUnknown;
      }


   } else {


      //
      // timer is needed
      //
      if ( (mTimerId == 0) && (mTimerState != eSndTimerBroken) ) {
         // timer is inactive & needed - start it
         mTimerState = eSndTimerBroken;
         result = timeGetDevCaps(&mTimerCaps, sizeof(TIMECAPS));
         if ( result ) {
#ifndef SHIP
            mprintf( "!!! timeGetDevCaps returns %d\n", result );
#endif
            failed = TRUE;
         }
 
         // get resolution from desired resolution & system limits
         resolution = 
            min(max(mTimerCaps.wPeriodMin, kTmBestResolution), 
                mTimerCaps.wPeriodMax);

         // break the DirectX display lock to avoid hang in timer calls
         int blockCookie = BlockDisplay();

         // start the timer
         result = timeBeginPeriod(kFadeGranularity);
         if ( result ) {
#ifndef SHIP
            mprintf( "!!! timeBeginPeriod returns %d\n", result );
#endif
            failed = TRUE;
         }

         //
         // first argument is how often timer callbacks occur (roughly) in
         //   milliseconds, second argument is how accurately they can
         //   be scheduled in milliseconds (how often timer interrupt occurs)
         //
         TLOG1( "Mix:CheckTimer on resolution %d", resolution );
         mTimerId = timeSetEvent(kFadeGranularity, resolution,
                                 TimerCallback,
                                 (DWORD)this, TIME_PERIODIC );

         if ( mTimerId == NULL ) {
#ifndef SHIP
            mprintf( "!!! timeSetEvent returns NULL\n" );
#endif
            failed = TRUE;
         }

         // restore the DirectX display lock
         ReleaseDisplay(blockCookie);

         if ( failed == FALSE ) {
            // the timer SHOULD be working, setup stuff which CheckTimer will
            // use to detect dead timer
            mTimerState = eSndTimerUnknown;
            mTimerLastTick = timeGetTime();
         }
      } else {
         //
         // timer is needed & has been started - see if it is really ticking
         //
         if ( mTimerState == eSndTimerUnknown ) {
            if ( (timeGetTime() - mTimerLastTick) > kTmFadeStartTimeout ) {
               // timer was started over a second ago, no ticks - assume broken
               Warning( ("Sound fade timer broken, resorting to manual fades\n") );
               mTimerState = eSndTimerBroken;
            }
         }
      }  // end if timer not started (id 0) && not broken



   } // end if timer needed
}


//
// inform mixer that timer may be needed
//
void
cSndMixer::StartTimer( void )
{
   TLOG0( "Mix:StartTimer" );
   mTimerNeeded = TRUE;
   CheckTimer();
}


//
// SetTimeout - set stream-refill timeout
//  setting timeout to 0xFFFFFFFF will turn off threading
//  returns previous timeout value
//
STDMETHODIMP_(uint32)
cSndMixer::SetTimeout( uint32       milliSecs )
{
   int      masterPriority;
   uint32   oldTimeout = mTimeoutMillisecs;

   mTimeoutMillisecs = milliSecs;
   if ( mpThread != NULL ) {
      //
      // threading is on
      //
      if ( milliSecs == ~0 ) {
         // turn threading off
         mpThread->CallWorker( kThreadExit );
         mpThread->WaitForClose();
         delete mpThread;
         mpThread = NULL;
      } else {
         // just change timeout value
         mpThread->SetTimeout( milliSecs );
      }
   } else {
      //
      // threading is off
      //
      if ( milliSecs != ~0 ) {
         // start threading & set timeout value
         mpThread = new cMixerThread( this );
         if ( !mpThread->Create() ) {
            // can't create mixer thread!
            delete mpThread;
            mpThread = NULL;
         } else {
            // set thread priority to one above main thread priority
            masterPriority = GetThreadPriority(GetCurrentThread());
            if ( masterPriority < THREAD_PRIORITY_HIGHEST ) {
               masterPriority++;
            }
            mpThread->SetPriority( masterPriority );
            mpThread->SetTimeout( mTimeoutMillisecs );
         }
      }
   }
   return oldTimeout;
}
   

//
// Block display use, if there is a display
//
int cSndMixer::BlockDisplay()
{
   if (mpDisplayDevice)
   {
      mpWinDisplayDevice->WaitForMutex();
      return mpDisplayDevice->BreakLock();
   }
   return 0;
}

//
// Release a previous display block
//
void cSndMixer::ReleaseDisplay(int cookie)
{
   if (mpDisplayDevice)
   {
      mpDisplayDevice->RestoreLock(cookie);
      mpWinDisplayDevice->ReleaseMutex();
   }
}

/************************************************************
 *
 *   M I X E R      T H R E A D
 *
 *  This thread handles refilling stream buffers
 *
 */

/*
 * Commands to stream thread:
 *  - do update
 *  - new stream - in case thread had shut itself due to no streams
 *  - shut down
 */


//
// thread which updates all streams
//
DWORD
cMixerThread::ThreadProc( void )
{
   DWORD          fWaitResult;
   BOOL           fExit = FALSE;
   HANDLE         waitHandle;
   cThreadMutex   &mixLock = mpMixer->MutexRef();
   BOOL           streamsActive = TRUE;

   waitHandle = GetCallHandle();
   // possible priorities: NORMAL, ABOVE_NORMAL, HIGHEST, TIME_CRITICAL
   SetPriority( THREAD_PRIORITY_HIGHEST );
   //SetTimeout( INFINITE );
   do {
// TBD: allow app to set timeout
      fWaitResult = WaitForSingleObject( waitHandle, 
                                         streamsActive ? mTimeoutMillisecs : INFINITE );

      TLOG_INT_START( 2 );
      switch ( fWaitResult ) {
         case WAIT_TIMEOUT:
            // timeout occured, so do update of streams
            TLOG0( "MixThread: CheckStreams timeout" );
            if (mixLock.Wait(0))
            {
               //mprintf("WOOOHOOO!!!!!!!!\n");
               PERF_ENTER( lgsoundThreadPM );
               mpMixer->DoTrace( NULL, kSndThreadUpdateStart );
               streamsActive = mpMixer->CheckStreams();
               PERF_EXIT( lgsoundThreadPM );
#ifdef DO_PERF_METERS
               TLOG1( "MixThread: CheckStreams took %d millisecs",
                     lgsoundThreadPM.deltaTime );
#endif
               mpMixer->DoTrace( NULL, kSndThreadUpdateEnd );
               mixLock.Release();
            }
            break;
         case WAIT_OBJECT_0:
            // It's a call from the master thread...
            switch ( GetCallParam() ){
               case kThreadExit:
                  Reply(S_OK);
                  fExit = TRUE;
                  break;

               case kThreadUpdate:
                  if (mixLock.Wait(0))
                  {
                     streamsActive = mpMixer->CheckStreams();
                     mixLock.Release();
                     Reply(S_OK);
                  }
                  else
                  {
                     Reply(E_FAIL);
                  }
                  break;

               case kThreadNewStream:
                  // a stream has been created - allow wakeup on timeout
                  streamsActive = TRUE;
                  Reply(S_OK);
                  break;

               default:
                  CriticalMsg("Unknown call to Mixer Stream-refill Thread");
                  Reply(E_FAIL);
                  break;
            }
            break;
      }
      TLOG_INT_END( 2 );
   } while ( !fExit && (fWaitResult != WAIT_FAILED) );

   return S_OK;
}


//
// debug aid
//
void
cSndMixer::TBD( char *message )
{
   TLOG1( "Mixer TBD: %s\n", ((long) message) );
#ifndef SHIP   
   mprintf( "Mixer TBD: %s\n", message );
#endif   
}
