////////////////////////////////////////////////////////////////////////
// $Header: x:/prj/tech/libsrc/sound/RCS/dmixer.cpp 1.2 1998/03/27 15:05:22 PATMAC Exp $
//
// (c) 1998 Looking Glass Technologies Inc.
// Pat McElhatton
//
// Module name: Mixer
// File name: dmixer.cpp
//
// Description: Top-level controller of digital sounds
//
////////////////////////////////////////////////////////////////////////

#include <windows.h>

#include <assert.h>

#include <dlgsndi.h>

#include <comtools.h>

#include <appagg.h>
#include <wappapi.h>
#include <wdispapi.h>
#include <dispapi.h>

#include <dsnddynf.h>
#include <mprintf.h>
#include <dbg.h>

#include <mixerlck.h>

#ifdef _MSC_VER
// short-term workaround: the A3D libs/headers don't work for Watcom, so disable
//  A3D acceleration in Watcom builds  - patmc 3-27-98

#define USE_A3D
#endif

#ifdef USE_A3D

#include <ia3d.h>
extern "C" {
//_declspec (dllexport) 
HRESULT WINAPI
A3dCreate(const GUID * lpGUID, LPDIRECTSOUND * ppDS, IUnknown FAR *pUnkOuter );
};

#else
extern "C" {
HRESULT WINAPI
A3dCreate(const GUID * lpGUID, LPDIRECTSOUND * ppDS, IUnknown FAR *pUnkOuter );
}
#endif

// @Note (toml 05-17-96):  Sometime relatively soon the mixer should be converted into a true
// member of the application aggregate object


#define MIXER_MUTEX \
  cMixerAutoLock __mixer_lock__( mMutex )

#define SAMPLE_NAME(SMP)  ((long) (SMP->GetName()))


////////////////////////////////////
// The Factory Method for creating the mixer
//

BOOL SndCreateMixer(ISndMixer **ppMixer, IUnknown *pOuter)
{
   *ppMixer = NULL;

   if( pOuter != NULL )
      return FALSE;

   *ppMixer = new cDSndMixer;

   return (*ppMixer != NULL);
}

BOOL SndCreateA3DMixer(ISndMixer **ppMixer, IUnknown *pOuter)
{
   cDSndMixer *pMixer;
   *ppMixer = NULL;

   if( pOuter != NULL )
      return FALSE;

   pMixer = new cDSndMixer;
   *ppMixer = pMixer;
   if ( pMixer != NULL ) {
      pMixer->SetA3d( TRUE );
   }

   return (pMixer != NULL);
}

void
cDSndMixer::SetA3d( BOOL useA3d )
{
   mbUseA3d = useA3d;
}

////////////////////////////////////
// IMPLEMENT_UNAGGREGATABLE
//
// This does all the implementations of IUnknown methods
//

IMPLEMENT_UNAGGREGATABLE_SELF_DELETE(cDSndMixer, ISndMixer);

////////////////////////////////////
// cDSndMixer contructor
//
// This just sets everything to 0
//

cDSndMixer::cDSndMixer()
{
   // DirectSound Specific stuff
   mSoundDevice = NULL;
   mpPrimaryBuffer = NULL;
   mpListener = NULL;
   mbUseA3d = FALSE;
   m3DDeferFlag = DS3D_IMMEDIATE;
   mbPanVolInUse = FALSE;
   // end DirectSound Specific stuff
}


////////////////////////////////////
// cDSndMixer destructor
//
// Destroy the primary sample buffer
// And free up the sample memory pool

cDSndMixer::~cDSndMixer()
{
   int blockCookie = BlockDisplay();

   Destroy();

   // free the 3D listener
   SafeRelease( mpListener );

   // free the primary DirectSound buffer
   if ( mpPrimaryBuffer ) {
      mpPrimaryBuffer->Stop();
      SafeRelease( mpPrimaryBuffer );
   }

   // destroy the DirectSound object
   SafeRelease( mSoundDevice );

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
cDSndMixer::Init( sSndSetup    *setup,
                 uint32       numChannels,
                 sSndAttribs  *pAttribs )
{
   DSCAPS               caps;
   HWND                 window;
   int                  i;
   DSBUFFERDESC         db;
   WAVEFORMATEX         fmt;
   HRESULT              res;
   BOOL                 howDegrading = FALSE;

   if(mSoundDevice != NULL)
      return kSndDeviceAlready;

   if ( LoadDirectSound() == FALSE ) {
      // couldn't load the DirectSound dll
      return kSndCantCreateDevice;
   }

   // attemp to create the DirectSound object
#ifdef USE_A3D
   if ( mbUseA3d ) {
      res = A3dCreate(NULL, &mSoundDevice, NULL);
   } else {
      res = DynDirectSoundCreate(NULL, &mSoundDevice, NULL);
   }
#else
   res = DynDirectSoundCreate(NULL, &mSoundDevice, NULL);
#endif
   if ( FAILED(res) ) {
      DSOUND_ERROR( res, "MixInit / DirectSoundCreate" );
      return kSndCantCreateDevice;
   }

   // Declare a self-releasing pointer to the IWinApp member of the application
   // aggregate and use it to get the main window (toml 05-17-96)
   AutoAppIPtr(WinApp);
   window = pWinApp->GetMainWnd();

   res = mSoundDevice->SetCooperativeLevel(window, DSSCL_PRIORITY);

   if ( res != DS_OK ) {
      DSOUND_ERROR( res, "MixInit / SetCooperativeLevel" );
      SafeRelease(mSoundDevice);
      return kSndCantSetupDevice;
   }

   caps.dwSize = sizeof(caps);

//     mSoundDevice->GetCaps(&caps);

//    if(!(caps.dwFlags&DSCAPS_CERTIFIED) || caps.dwFlags&DSCAPS_EMULDRIVER)
//        ;


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

   //
   // create the primary DirectSoundBuffer, which controls the
   //  output side of the DirectSound mixer
   //

   // init the DirectSound buffer descriptor
   memset( &db, 0, sizeof(db) );       // zero all unused/reserved fields
   db.dwBufferBytes = 0;
   db.dwSize = sizeof(db);
   db.dwFlags = DSBCAPS_PRIMARYBUFFER;
   db.lpwfxFormat = NULL;     // app cannot specify primary buffer format

   // init the wave format info
   memset( &fmt, 0, sizeof(fmt) );     // zero all unused/reserved fields
   fmt.wFormatTag = WAVE_FORMAT_PCM;
   fmt.nChannels = (WORD) mPrimaryAttribs.nChannels;
   fmt.nSamplesPerSec = mPrimaryAttribs.sampleRate;
   fmt.wBitsPerSample = (WORD) mPrimaryAttribs.bitsPerSample;
   // the following assumes an integral bytes/sample (PCM, not ADPCM)
   fmt.nBlockAlign = (WORD) ((mPrimaryAttribs.nChannels
                             * mPrimaryAttribs.bitsPerSample) / 8);
   fmt.nAvgBytesPerSec = mPrimaryAttribs.sampleRate * fmt.nBlockAlign;

   switch ( m3DMixMethod ) {
      case kSnd3DMethodSoftware:
      case kSnd3DMethodHardware:
         // request DirectSound 3D support
         db.dwFlags |= DSBCAPS_CTRL3D;
         break;
   }

   // create the primary DirectSoundBuffer
   res = mSoundDevice->CreateSoundBuffer( &db, &mpPrimaryBuffer, NULL);
   DSOUND_ERROR_CHECK( res, "MixInit / CreateSoundBuffer" );

   if ( res == DS_OK ) {
      // set the attributes of the primary buffer
      res = mpPrimaryBuffer->SetFormat( &fmt );
      DSOUND_ERROR_CHECK( res, "MixInit / SetFormat" );
      if ( res != DS_OK ) {
         if ( fmt.wBitsPerSample == 16 ) {
            // failed to init with 16-bit sample size, attempt to
            // degrade gracefully to 8-bit samples
            fmt.wBitsPerSample = 8;
            mPrimaryAttribs.bitsPerSample = 8;
            fmt.nBlockAlign >>= 1;
            fmt.nAvgBytesPerSec >>= 1;
            res = mpPrimaryBuffer->SetFormat( &fmt );
            DSOUND_ERROR_CHECK( res, "MixInit / SetFormat 8-bit" );
            howDegrading = TRUE;
         }
         if ( res != DS_OK ) {
            if ( fmt.nChannels == 2 ) {
               // failed to init with stereo, attempt to use mono
               fmt.nChannels = 1;
               mPrimaryAttribs.nChannels = 1;
               fmt.nBlockAlign >>= 1;
               fmt.nAvgBytesPerSec >>= 1;
               res = mpPrimaryBuffer->SetFormat( &fmt );
               DSOUND_ERROR_CHECK( res, "MixInit / SetFormat 8-bit mono" );
               howDegrading = TRUE;
            }
         }
      }
   }

   if ( (mpPrimaryBuffer == NULL) || (res != DS_OK) ) {
      SafeRelease(mSoundDevice);
      return kSndCantCreateDevice;
   } else {
      res = mpPrimaryBuffer->Play( 0, 0, DSBPLAY_LOOPING );
      DSOUND_ERROR_CHECK( res, "MixInit / PrimaryBuffer Play" );
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

   // try to get the 3D Listener interface
   m3DDeferFlag = DS3D_IMMEDIATE;
   switch ( m3DMixMethod ) {
      case kSnd3DMethodSoftware:
      case kSnd3DMethodHardware:
         // request DirectSound 3D support
         res = mpPrimaryBuffer->QueryInterface( IID_IDirectSound3DListener,
                                                (void **) &mpListener );
         //DSOUND_ERROR_CHECK( res, "MixInit / QueryInterface 3DListener" );
         //TBD: check res
         if ( res != DS_OK ) {
            // note that this masks kSndOnly8Bit
            return kSnd3DInitFailure;
         }
         Set3DPosition( &m3DPosition );
         Set3DOrientation( &m3DFrontVector, &m3DTopVector );
         Set3DVelocity( &m3DVelocity );
         Set3DEnvironment( &m3DEnvironment );
         break;
   }

   return (howDegrading) ? kSndOnly8Bit : kSndOk;
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
cDSndMixer::CreateRawSample(
                            eSndSampleCreateFlagSet flags,
                            void                    *pData,
                            uint32                  dataLen,
                            uint32                  nSamples,
                            sSndAttribs             *pAttribs )
{
   //
   // get a sample - steal an old sample if needed
   //
   TLOG0( "Mix::CreateRawSample" );
   cDSndSample *smp = new cDSndSample(this, flags);

   if( smp == NULL )
      return NULL;

   //
   // if all is still well, initialize the sample
   //  this is where a DirectSoundBuffer for the sample actually gets created
   //  this is also where the audio data for one-shot sounds gets passed
   //
   if(smp->Init( pAttribs, pData, dataLen, nSamples ) == FALSE) {

      SafeRelease(smp);

   } else {
      switch(flags) {
         case kSndSampleStream:
            // let stream refill thread know it must reactivate
            if ( mpThread != NULL ) {
               mpThread->CallWorker( kThreadNewStream );
            }
            break;
         case kSndSampleInternal:
            break;
         default:
            //
            // this is a one-shot sample, if the caller supplied a data buffer address
            //  make it audible & load all its data, otherwise the app must load
            //  the data through a fill callback
            //
            if ( pData != NULL ) {
               if ( smp->MakeAudible() ) {
                  smp->LoadBuffer( (uint8 *) pData, dataLen );
               } else {
                  // couldn't make sample audible
                  SafeRelease( smp );
                  TLOG0( "Mix::CreateSampleRaw: failed to make one-shot sample, no channels avail" );
               }
            }
            break;
      }
      if ( smp != NULL ) {
         smp->SetGroupVolume( mMasterVolume + mGroupVolumes[0] );
      }
   }

   return smp;
}


////////////////////////////////////
// Pause
//
// This will suspend all sound output until resume is called
//

STDMETHODIMP_(void) cDSndMixer::Pause()
{
   HRESULT res;

   TLOG0( "Mix::Pause" );
   res = mpPrimaryBuffer->Stop();
   DSOUND_ERROR_CHECK( res, "MixPause / Stop" );
}

////////////////////////////////////
// Resume
//
// This will restart all output if  paused
//

STDMETHODIMP_(void) cDSndMixer::Resume()
{
   HRESULT res;

   TLOG0( "Mix::Resume" );
   res = mpPrimaryBuffer->Play( 0, 0, DSBPLAY_LOOPING );
   DSOUND_ERROR_CHECK( res, "MixResume / Play" );   
}


/************************************************************
 *
 *   3  D      S  T  U  F  F
 *
 *
 */

#////////////////////////////////////
//
// Set3DPosition
//

STDMETHODIMP_(void)
cDSndMixer::Set3DPosition( sSndVector *pPosition )
{
   HRESULT res;

   assert( pPosition != NULL );
   TLOG3( "Mix::Set3DPosition %d %d %d",
          pPosition->x, pPosition->y, pPosition->z );
   m3DPosition = *pPosition;

   if ( m3DMixMethod != kSnd3DMethodNone ) {

      if ( mpListener != NULL ) {
         res = mpListener->SetPosition( -(pPosition->y), pPosition->z, pPosition->x,
                                        m3DDeferFlag );
         DSOUND_ERROR_CHECK( res, "Mix:Set3DPosition" );
      }

      // redo pan/vol for all audible sources
      if ( mbPanVolInUse && (mpAudibleListHead != NULL) ) {
         mbPanVolInUse = ((cDSndSample *) mpAudibleListHead)->UpdatePanVol3D();
      }
   }
}


////////////////////////////////////
//
// Set3DOrientation
//

STDMETHODIMP_(void)
cDSndMixer::Set3DOrientation( sSndVector *pFront, sSndVector *pTop )
{
   HRESULT res;

   assert( pFront != NULL );
   assert( pTop != NULL );
   TLOG3( "Mix::Set3DOrientation x100 %d %d %d",
          pFront->x * 100, pFront->y * 100, pFront->z * 100 );
   //TBD: what if app is using some panvol & some hardware 3D?
   m3DFrontVector = *pFront;
   m3DTopVector = *pTop;

   if ( m3DMixMethod != kSnd3DMethodNone ) {

      if ( mpListener != NULL ) {
         res = mpListener->SetOrientation( -(pFront->y), pFront->z, pFront->x,
                                           -(pTop->y), pTop->z, pTop->x,
                                           m3DDeferFlag );
         DSOUND_ERROR_CHECK( res, "Mix:Set3DOrientation" );
      }

      // redo pan/vol for all audible sources
      if ( mbPanVolInUse && (mpAudibleListHead != NULL) ) {
         mbPanVolInUse = ((cDSndSample *) mpAudibleListHead)->UpdatePanVol3D();
      }
   }
}


////////////////////////////////////
//
// Set3DVelocity
//

STDMETHODIMP_(void)
cDSndMixer::Set3DVelocity( sSndVector *pVelocity )
{
   HRESULT res;

   assert( pVelocity != NULL );
   TLOG3( "Mix::Set3DVelocity %d %d %d",
          pVelocity->x, pVelocity->y, pVelocity->z );
   if ( mpListener != NULL ) {
      res = mpListener->SetVelocity( -(pVelocity->y), pVelocity->z, pVelocity->x,
                                     m3DDeferFlag );
      DSOUND_ERROR_CHECK( res, "Mix:Set3DVelocity" );
   }
   m3DVelocity = *pVelocity;
}


////////////////////////////////////
//
// Set3DEnvironment
//

STDMETHODIMP_(void)
cDSndMixer::Set3DEnvironment( sSndEnvironment *pEnv )
{
   HRESULT res;

   TLOG3( "Mix::Set3DEnvironment x1000 doppler %d distance %d rolloff %d",
          1000 * pEnv->dopplerFactor, 1000 * pEnv->distanceFactor, 1000 * pEnv->rolloffFactor );
   assert( pEnv != NULL );
   if ( mpListener != NULL ) {
      if ( pEnv->dopplerFactor != m3DEnvironment.dopplerFactor ) {
         res = mpListener->SetDopplerFactor( pEnv->dopplerFactor, m3DDeferFlag );
         DSOUND_ERROR_CHECK( res, "Mix:Set3DEnvironment Doppler" );
      }
      if ( pEnv->distanceFactor != m3DEnvironment.distanceFactor ) {
         res = mpListener->SetDistanceFactor( pEnv->distanceFactor, m3DDeferFlag );
         DSOUND_ERROR_CHECK( res, "Mix:Set3DEnvironment Distance" );
      }
      if ( pEnv->rolloffFactor != m3DEnvironment.rolloffFactor ) {
         res = mpListener->SetRolloffFactor( pEnv->rolloffFactor, m3DDeferFlag );
         DSOUND_ERROR_CHECK( res, "Mix:Set3DEnvironment Rolloff" );
      }
   }
   m3DEnvironment = *pEnv;
}


////////////////////////////////////
//
// Set3DMethod
//

STDMETHODIMP_(void)
cDSndMixer::Set3DMethod( eSnd3DMethod method )
{
   // assert if this is called after Init
   assert( mpPrimaryBuffer == NULL );
   TLOG1( "Mix::Set3DMethod %d", method );
   m3DMixMethod = method;
}


////////////////////////////////////
//
// Get3DMethodCapabilities - return a bit vector telling which 3D methods
//   are supported
// 
//

STDMETHODIMP_(void)
cDSndMixer::Get3DMethodCapabilities( uint32 *pMethods )
{
   assert( pMethods != NULL );
   // TBD: check for 3D hardware & report if found
   if ( mbUseA3d ) {
      *pMethods = kSnd3DMethodSoftware | kSnd3DMethodPanVol | kSnd3DMethodHardware;
   } else {
      *pMethods = kSnd3DMethodSoftware | kSnd3DMethodPanVol;
   }
   TLOG1( "Mix::Get3DMethodCapabilities 0x%x", *pMethods );
}


////////////////////////////////////
//
// Set3DDeferMode - set deferred mode
//

STDMETHODIMP_(void)
cDSndMixer::Set3DDeferMode( BOOL  deferOn )
{
   HRESULT res;

   TLOG1( "Mix::Set3DDeferMode %d", deferOn );
   m3DDeferFlag = (deferOn) ? DS3D_DEFERRED : DS3D_IMMEDIATE;
   if ( (!deferOn) && (mpListener != NULL) ) {
      // they turned deferred mode off, so tell DS3D to do any deferred stuff
      res = mpListener->CommitDeferredSettings();
      DSOUND_ERROR_CHECK( res, "Mix:Set3DDeferMode" );
   }
}


cDSndDevice *
cDSndMixer::GetDevice()
{
	return mSoundDevice;
}


