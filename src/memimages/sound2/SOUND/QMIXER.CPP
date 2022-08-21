////////////////////////////////////////////////////////////////////////
// $Header: x:/prj/tech/libsrc/sound/RCS/qmixer.cpp 1.1 1998/03/20 12:53:34 PATMAC Exp $
//
// (c) 1998 Looking Glass Technologies Inc.
// Pat McElhatton
//
// Module name: Mixer
// File name: mixer.cpp
//
// Description: Top-level controller of digital sounds
//
////////////////////////////////////////////////////////////////////////

#include <windows.h>

#include <assert.h>

#include <qlgsndi.h>

#include <comtools.h>

#include <appagg.h>
#include <wappapi.h>
#include <wdispapi.h>
#include <dispapi.h>

#include <mprintf.h>
#include <dbg.h>

#include <mixerlck.h>


#define MIXER_MUTEX \
  cMixerAutoLock __mixer_lock__( mMutex )

#define SAMPLE_NAME(SMP)  ((long) (SMP->GetName()))


////////////////////////////////////
// The Factory Method for creating the mixer
//

BOOL SndCreateQSMixer(ISndMixer **ppMixer, IUnknown *pOuter)
{
   *ppMixer = NULL;

   if( pOuter != NULL )
      return FALSE;

   *ppMixer = (ISndMixer *) new cQSndMixer;

   return (*ppMixer != NULL);
}

#ifndef SHIP

#define QSOUND_ERROR_CHECK( res, str )  if ( res ) DisplayQSError( res, str )
#define QSOUND_ERROR( str )  DisplayQSError( 0, str )

static void
DisplayQSError( UINT    res,
                char    *pStr )
{
   char daText[512];

   if ( res ) {
      QSWaveMixGetErrorText( res, daText, sizeof(daText) );
      mprintf( "Mixer error from %s: %s\n", pStr, daText );
   } else {
      mprintf( "Mixer error from %s\n", pStr );
   }
}

#else

#define QSOUND_ERROR_CHECK( res, str )
#define QSOUND_ERROR( str )

#endif

////////////////////////////////////
// IMPLEMENT_UNAGGREGATABLE
//
// This does all the implementations of IUnknown methods
//

IMPLEMENT_UNAGGREGATABLE_SELF_DELETE(cQSndMixer, ISndMixer);

////////////////////////////////////
// cQSndMixer contructor
//
// This just sets everything to 0
//

cQSndMixer::cQSndMixer()
{
   mpMixDevice = NULL;
   mDistanceFactor = 1.0;
}


////////////////////////////////////
// cQSndMixer destructor
//
// Destroy the primary sample buffer
// And free up the sample memory pool

cQSndMixer::~cQSndMixer()
{
   int blockCookie = BlockDisplay();

   Destroy();

   // free the QMixer
   if ( mpMixDevice ) {
      QSWaveMixCloseSession( mpMixDevice );
      mpMixDevice = NULL;
   }

   ReleaseDisplay(blockCookie);
}


////////////////////////////////////
// Init()
//
// How to initialize the mixer.  In this Win32 implementation this creates
// the DirectSound interface.

#ifdef __WATCOMC__
#pragma off(unreferenced)
#endif


STDMETHODIMP_(eSndError)
cQSndMixer::Init( sSndSetup    *setup,
                  uint32       numChannels,
                  sSndAttribs  *pAttribs )
{
   HWND                 window;
   int                  i;
   HRESULT              res;
   BOOL                 howDegrading = FALSE;
   QMIXCONFIG           qMixerConfig;

   if(mpMixDevice != NULL)
      return kSndDeviceAlready;

   // Declare a self-releasing pointer to the IWinApp member of the application
   // aggregate and use it to get the main window (toml 05-17-96)
   AutoAppIPtr(WinApp);
   window = pWinApp->GetMainWnd();

   mpAudibleListHead = NULL;
   mpInaudibleListHead = NULL;

   mNumMixerChans = numChannels;
   mNumFreeMixerChans = numChannels;

   mpIterNext = NULL;
   mIterGroup = 1;

   if ( pAttribs == NULL ) {
      // user didn't specify a set of sound attribs for
      //  the primary sample - pick a default
      mPrimaryAttribs.sampleRate = 22050;
      mPrimaryAttribs.bitsPerSample = 16;
      mPrimaryAttribs.nChannels = 2;
   } else {
      // use the attribs user supplied
      mPrimaryAttribs = *pAttribs;
   }

   TLOG3( "\nMix::Init rate %d bits %d DSound channels %d\n", mPrimaryAttribs.sampleRate,
         mPrimaryAttribs.bitsPerSample, numChannels );

   // fill in the primary attribs which user can't control
   mPrimaryAttribs.dataType = kSndDataPCM;
   mPrimaryAttribs.bytesPerBlock = (mPrimaryAttribs.bitsPerSample
                                    * mPrimaryAttribs.nChannels) / 8;
   mPrimaryAttribs.samplesPerBlock = 1;
   mPrimaryAttribs.numSamples = ~0;

   // create the main mixer object
   memset( &qMixerConfig, 0, sizeof(qMixerConfig) );
   qMixerConfig.dwSize = sizeof( qMixerConfig );
   qMixerConfig.dwFlags = QMIX_CONFIG_USEPRIMARY | QMIX_CONFIG_NOTHREAD
      | QMIX_CONFIG_PLAYSILENCE;
   qMixerConfig.dwSamplingRate = mPrimaryAttribs.sampleRate;
   qMixerConfig.iOutput = QMIX_CONFIG_OUTPUT_DETECT;
   qMixerConfig.iChannels = numChannels;
   qMixerConfig.iMath = QMIX_CONFIG_MATH_DETECT;
   qMixerConfig.iLatency = 100;  // 0.1 seconds
   qMixerConfig.lpIDirectSound = NULL;
   //TBD: should we make our own direct sound object and let
   // qmixer use that??
   mpMixDevice = QSWaveMixInitEx( &qMixerConfig );
   if ( mpMixDevice == NULL ) {
      QSOUND_ERROR( "Init / MixInit" );
   } else {
      res = QSWaveMixActivate( mpMixDevice, TRUE );
      QSOUND_ERROR_CHECK( res, "Init / Activate" );
      if ( m3DMixMethod != kSnd3DMethodNone ) {
         Set3DPosition( &m3DPosition );
         Set3DOrientation( &m3DFrontVector, &m3DTopVector );
         Set3DVelocity( &m3DVelocity );
         Set3DEnvironment( &m3DEnvironment );
      }
   }
                               
   for ( i = 0; i < (kSndNumGroups+1); i++ ) {
      // set all group volumes to 0
      mGroupVolumes[i] = 0;
      mpGroupFades[i] = NULL;
   }
   mGroupFadesActive = FALSE;

   mMasterVolume = 0;

   mTimerState = eSndTimerUnknown;
   
   mpDisplayDevice = AppGetObj(IDisplayDevice);
   mpWinDisplayDevice = AppGetObj(IWinDisplayDevice);

   // set time for pan/volume changes to 50 milliseconds
   res = QSWaveMixSetPanRate( mpMixDevice, 0, QMIX_ALL, 50 );
   QSOUND_ERROR_CHECK( res, "Init / SetPanRate" );

   return kSndOk;
}

#ifdef __WATCOMC__
#pragma on(unreferenced)
#endif

////////////////////////////////////
// Pause
//
// This will suspend all sound output until resume is called
//

STDMETHODIMP_(void) cQSndMixer::Pause()
{
   UINT     res;

   res = QSWaveMixActivate( mpMixDevice, FALSE );
   QSOUND_ERROR_CHECK( res, "Pause" );
}

////////////////////////////////////
// Resume
//
// This will restart all output if  paused
//

STDMETHODIMP_(void) cQSndMixer::Resume()
{
   UINT     res;

   res = QSWaveMixActivate( mpMixDevice, TRUE );
   QSOUND_ERROR_CHECK( res, "Resume" );
}


////////////////////////////////////
// CreateRawSample
//
// Takes:   flags -- describes what kind of sample is desired
//          buf -- a pointer to the raw data
//          format -- enumerated formats (wave, voc, lg etc)
//          channels, bits, sampleRate - format specifiers
// Returns: An ISndSample * that is ready to go.
//

STDMETHODIMP_(ISndSample *)
cQSndMixer::CreateRawSample(
                           eSndSampleCreateFlagSet flags,
                           void                    *pData,
                           uint32                  dataLen,
                           uint32                  nSamples,
                           sSndAttribs             *pAttribs )
{
   BOOL sampleOK;

   //
   // get a sample - steal an old sample if needed
   //
   TLOG3( "Mix::CreateRawSample flags 0x%x data 0x%x #samples %d",
         flags, (long) pData, nSamples );
   cQSndSample *smp = new cQSndSample( this, flags );

   if( smp == NULL )
      return NULL;

   //
   // if all is still well, initialize the sample
   //  this is where a DirectSoundBuffer for the sample actually gets created
   //  this is also where the audio data for one-shot sounds gets passed
   //
   sampleOK = smp->Init( pAttribs, pData, dataLen, nSamples );

   if ( sampleOK ) {

      if ( (flags == kSndSampleStream) &&
           (mpThread != NULL) ) {
         // let stream refill thread know it must reactivate
         mpThread->CallWorker( kThreadNewStream );
      }

      if ( smp != NULL ) {
         smp->SetGroupVolume( mMasterVolume + mGroupVolumes[0] );
      }

   } else {

      TLOG0( "Mix::CreateSampleRaw: failed in Smp::Init" );
      SafeRelease(smp);

   }

   return smp;
}


////////////////////////////////////
// Update
//
// How to keep everything moving along.  This is how we deal
// with things like finding samples that were playing but aren't anymore.
//

STDMETHODIMP_(void) cQSndMixer::Update()
{
   TLOG0( "Mix::Update" );

   DoTrace( NULL, kSndMixerUpdateStart );
   // find out who is done
   {
      MIXER_MUTEX;
      CleanUpFinishedSamples();
   }

   // refill stream ring buffers
   QSWaveMixPump();

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
   //TLOG0( "Mix::Update - done" );
}

/************************************************************
 *
 *   3  D      S  T  U  F  F
 *
 *
 */

////////////////////////////////////
//
// Set3DPosition
//

STDMETHODIMP_(void)
cQSndMixer::Set3DPosition( sSndVector *pPosition )
{
   UINT res;

   assert( pPosition != NULL );
   m3DPosition.x = pPosition->x * mDistanceFactor;
   m3DPosition.y = pPosition->y * mDistanceFactor;
   m3DPosition.z = pPosition->z * mDistanceFactor;

   res = QSWaveMixSetListenerPosition( mpMixDevice, (QSVECTOR *) pPosition, 0 );
   QSOUND_ERROR_CHECK( res, "Set3DPosition" );
}


////////////////////////////////////
//
// Set3DOrientation
//

STDMETHODIMP_(void)
cQSndMixer::Set3DOrientation( sSndVector *pFront, sSndVector *pTop )
{
   UINT res;

   assert( pFront != NULL );
   assert( pTop != NULL );
   m3DFrontVector = *pFront;
   m3DTopVector = *pTop;

   res = QSWaveMixSetListenerOrientation( mpMixDevice, (QSVECTOR *) pFront,
                                          (QSVECTOR *) pTop, 0 );
   QSOUND_ERROR_CHECK( res, "Set3DOrientation" );
}


////////////////////////////////////
//
// Set3DVelocity
//

STDMETHODIMP_(void)
cQSndMixer::Set3DVelocity( sSndVector *pVelocity )
{
   UINT res;

   assert( pVelocity != NULL );
   m3DVelocity = *pVelocity;

   res = QSWaveMixSetListenerVelocity( mpMixDevice, (QSVECTOR *) pVelocity, 0 );
   QSOUND_ERROR_CHECK( res, "Set3DVelocity" );
}


////////////////////////////////////
//
// Set3DEnvironment
//

// default speed of sound in meters per second
#define SPEED_OF_SOUND 331.36

STDMETHODIMP_(void)
cQSndMixer::Set3DEnvironment( sSndEnvironment *pEnv )
{
   UINT res;

   assert( pEnv != NULL );
   m3DEnvironment = *pEnv;
   // TBD: should doppler factor be a multiplier, or divisor, for SOS ?
   res = QSWaveMixSetSpeedOfSound( mpMixDevice,
                                   SPEED_OF_SOUND * pEnv->dopplerFactor, 0 );
   QSOUND_ERROR_CHECK( res, "SetSpeedOfSound" );

   mDistanceFactor = pEnv->distanceFactor;

   // TBD: rolloff factor, rolloffFactor
}


////////////////////////////////////
//
// Set3DMethod
//

STDMETHODIMP_(void)
cQSndMixer::Set3DMethod( eSnd3DMethod method )
{
   // assert if this is called after Init
   m3DMixMethod = method;
}


////////////////////////////////////
//
// Get3DMethodCapabilities - return a bit vector telling which 3D methods
//   are supported
// 
//

STDMETHODIMP_(void)
cQSndMixer::Get3DMethodCapabilities( uint32 *pMethods )
{
   assert( pMethods != NULL );
   // TBD: check for 3D hardware & report if found
   *pMethods = kSnd3DMethodSoftware | kSnd3DMethodPanVol;
}




