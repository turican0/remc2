////////////////////////////////////////////////////////////////////////
// $Header: x:/prj/tech/libsrc/sound/RCS/qsample.cpp 1.1 1998/03/20 12:54:01 PATMAC Exp $
//
// (c) 1998 Looking Glass Technologies Inc.
// Pat McElhatton
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

#include <qlgsndi.h>
#include <mprintf.h>

#include <mixerlck.h>

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

// internal set volume & pan routines
UINT InternalSetVolume( HQMIXER pMixer, int chan, int32 mBVolume );
UINT InternalSetPan( HQMIXER pMixer, int chan, int32 pan );

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
      mprintf( "Sample error from %s: %s\n", pStr, daText );
   } else {
      mprintf( "Sample error from %s\n", pStr );
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

IMPLEMENT_UNAGGREGATABLE_SELF_DELETE(cQSndSample, ISndSample);

///////////////////////////////////
// cQSndSample constructor
//
//  This just sets the state to something safe.
//  Init will tell us if everything is ok.
//

cQSndSample::cQSndSample( cQSndMixer               *pMixer,
                          eSndSampleCreateFlagSet flags )
   : cSndSample( (cSndMixer *) pMixer, flags )
{
   
   mChannel = -1;
   mpWave = NULL;
   mpMixer = pMixer;
   mpMixDevice = pMixer->GetQMixer();

   mAmbientVolumeDB = 0.0;
   mRolloff = 1.0;
   mpAudioData = NULL;

}

///////////////////////////////////
// cQSndSample destructor
//
// make sure that the implementor is gone
//


cQSndSample::~cQSndSample( void )
{
   UINT res;
   MIXER_MUTEX;

   if ( mChannel >= 0 ) {
      mpMixer->DoTrace( (void *) mBufferLen, kSndBufferFree );
      QSWaveMixCloseChannel( mpMixDevice, mChannel, 0 );
      // tell the mixer we're gone & a channel is free
      mpMixer->FreeChannel();

      if ( mpWave != NULL ) {
         res = QSWaveMixFreeWave( mpMixDevice, mpWave );
         QSOUND_ERROR_CHECK( res, "~Sample / FreeWave" );
// TBD: print error message if mpWave == NULL?
      }
   }
}

///////////////////////////////////
//
// This stump is called from the QMixer and passes the call along to the
//   app supplied refill callback
//
static BOOL CALLBACK
QStreamRefillCB( void   *pBuffer,
                 DWORD  nBytes,
                 void   *pUserCBData )
{
   cQSndSample  *pSample = (cQSndSample *) pUserCBData;

   return pSample->InvokeRefillCB( pBuffer, nBytes );
}


///////////////////////////////////
//
// InvokeRefillCB - invoke user stream refill callback, called
//  from QMixer refill callback.  Save away the data destination
//  ptr for LoadBufferIndirect to use later
//
// Return TRUE if stream is still passing data
//
BOOL
cQSndSample::InvokeRefillCB( void      *pBuffer,
                             uint32    nBytes )
{
   mpRefillDest = pBuffer;

   assert( fnFillCB != NULL );

   (fnFillCB) ( this, mpFillCBData, nBytes );

   return ( (GetFlags() & kSndFlagEndOfData) == 0 );
}


///////////////////////////////////
//
// MakeAudible - allocate and init a low-level channel
// Return TRUE if it succeeded
//
BOOL
cQSndSample::MakeAudible()
{
   WAVEFORMATEX         audioFormat;
   QMIXWAVEPARAMS       waveInfo;
   UINT                 res;

   MIXER_MUTEX;

   TLOG3( "Smp::MakeAudible %s %ld audible %ld inaudible",
         SAMPLE_NAME, mNumAudible, mNumInaudible );

   if ( IS_AUDIBLE ) {
      return TRUE;
   }

   ///////////////////////////////////////////////
   // 
   // allocate a QMixer channel to this sample
   //
   if ( mpMixer->AllocChannel( this ) == FALSE ) {
      // can't allocate a channel!
      return FALSE;
   }

   // there should be an available QMixer channel, really try to get it
   mChannel = QSWaveMixOpenChannel( mpMixDevice, 1, QMIX_OPENAVAILABLE );
   if ( mChannel < 0 ) {
      // couldn't open a free channel
      return FALSE;
   }

   // set the data format
   audioFormat.wFormatTag = WAVE_FORMAT_PCM;
   audioFormat.nChannels = (WORD) mAttribs.nChannels;
   audioFormat.nSamplesPerSec = mAttribs.sampleRate;
   // the following assumes an integral bytes/sample (PCM, not ADPCM)
   audioFormat.nBlockAlign = (WORD) ((mAttribs.nChannels
                                         * mAttribs.bitsPerSample) / 8);
   audioFormat.nAvgBytesPerSec = mAttribs.sampleRate * audioFormat.nBlockAlign;
   audioFormat.wBitsPerSample = (WORD) mAttribs.bitsPerSample;
   audioFormat.cbSize = 0;

   switch ( mFlags ) {

      case kSndSampleStream:
         ///////////////////////////////////////////////
         //
         // create a stream wave to play sound with
         //
         waveInfo.Stream.Format = &audioFormat;
         waveInfo.Stream.BlockBytes = 4096;
         waveInfo.Stream.Callback = QStreamRefillCB;
         waveInfo.Stream.User = (void *) this;
         waveInfo.Stream.Tag = (WORD) mSerialNum;

         // open the stream
         mpWave = QSWaveMixOpenWaveEx( mpMixDevice, &waveInfo,
                                       QMIX_INPUTSTREAM );

         break;

      case kSndSampleNormal:
         ///////////////////////////////////////////////
         //
         // create a mem-resident wave to play sound with
         //

         // if no sound buffer has been allocated, do it now
         //   and load it with the oneshot/loop data
         if ( mpAudioData == NULL ) {
            mpAudioData = Malloc( mBufferLen );
            InvokeRefillCB( mpAudioData, mBufferLen );
         }

         // fill in the memory-resident sound descriptor
         waveInfo.Resident.Format = &audioFormat;
         waveInfo.Resident.Data = (HPSTR) mpAudioData;
         waveInfo.Resident.Bytes = mBufferLen;
         waveInfo.Resident.Samples = mNumSamples;
         waveInfo.Resident.Tag = (WORD) mSerialNum;

         // make the memory-resident wave
         mpWave = QSWaveMixOpenWaveEx( mpMixDevice, &waveInfo, QMIX_RESIDENT );
         break;

   }

   if ( mpWave != NULL ) {
      mpMixer->DoTrace( (void *) mBufferLen, kSndBufferAllocate );
      MoveToList( mpMixer->AudibleHead() );
      SetFlags( kSndFlagAudible );

      // load shadowed state into hardware
      mGroupVolume = mpMixer->GetGroupVolume( mGroup ) + mpMixer->GetMasterVolume();
      res = InternalSetVolume( mpMixDevice, mChannel, mVolume + mGroupVolume );
      QSOUND_ERROR_CHECK( res, "MakeAudible / SetVolume" );
      res = InternalSetPan( mpMixDevice, mChannel, mPan );
      QSOUND_ERROR_CHECK( res, "MakeAudible / SetPan" );
      res = QSWaveMixSetFrequency( mpMixDevice, mChannel, 0, mFrequency );
      QSOUND_ERROR_CHECK( res, "MakeAudible / SetFrequency" );

      // enable 3D processing if needed
      switch ( m3DMethod ) {
         case kSnd3DMethodHardware:    //TBD: how to do HW with QSound?
         case kSnd3DMethodSoftware:
            res = QSWaveMixEnableChannel( mpMixDevice, mChannel, 0, QMIX_CHANNEL_QSOUND );
            break;
         case kSnd3DMethodPanVol:
            res = QSWaveMixEnableChannel( mpMixDevice, mChannel, 0, QMIX_CHANNEL_STEREO );
            break;
      }
      if ( m3DMethod != kSnd3DMethodNone ) {
         Set3DPosition( &m3DPosition );
         Set3DVelocity( &m3DVelocity );
         Set3DConeAngles( m3DConeInnerAngle, m3DConeOuterAngle );
         Set3DConeOrientation( &m3DConeOrientation );
         Set3DDistanceRange( m3DMinDistance, m3DMaxDistance );
         Set3DMode( m3DMode );
         SetAmbientVolume( mAmbientVolume );
      }
      QSOUND_ERROR_CHECK( res, "MakeAudible / Set 3D or Stereo" );

   } else {
      QSOUND_ERROR( "MakeAudible / CreateSoundBuffer" );
      return FALSE;
   }

   mLastWrite = 0;

   return TRUE;
}


void
cQSndSample::LLStop( void )
{
   UINT res;

   res = QSWaveMixStopChannel( mpMixDevice, mChannel, 0 );
   QSOUND_ERROR_CHECK( res, "LLStop" );
}

void
cQSndSample::LLRelease( void )
{
   UINT res;

   res = QSWaveMixFreeWave( mpMixDevice, mpWave );
   QSOUND_ERROR_CHECK( res, "LLRelease" );
   mpWave = NULL;
}


void
cQSndSample::LLInit( void )
{
   if ( mFlags == kSndSampleNormal ) {
      // if audio data is supplied, allocate a buffer & copy data to it
      if ( mpBuffer != NULL ) {
         mpAudioData = Malloc( mBufferLen );
         memcpy( mpAudioData, mpBuffer, mBufferLen );
      }
   }
}

static void CALLBACK
QSEndCB( int,
         LPMIXWAVE,
         DWORD       userData )
{
   cQSndSample    *pSample;

   pSample = (cQSndSample *) userData;

   // this will cause sample to stop on next Mixer::Update
   pSample->SetFlags( kSndFlagPleaseStop );

}


HRESULT
cQSndSample::LLStart( void )
{
   UINT res;
   int nLoops;
   QMIXPLAYPARAMS playParams;

   assert(mpWave != NULL);

   // setup play parameters
   playParams.dwSize = sizeof(playParams);
   playParams.lpImage = NULL;
   playParams.hwndNotify = 0;
   playParams.callback = QSEndCB;
   playParams.dwUser = (DWORD) this;
   playParams.lStart = 0;
   playParams.lEnd = 0;
   playParams.lStartLoop = 0;
   playParams.lEndLoop = 0;
   if ( mInitBuffPos != -1 ) {
      // user specified to start play at position in mInitBuffPos
      playParams.lStart = mInitBuffPos * mBytesPerSample;
      mInitBuffPos = -1;
   }
            
   nLoops = (mStateFlags & kSndFlagLooped) ? -1 : 0;

   // play the sound on the assigned channel
   res = QSWaveMixPlayEx( mpMixDevice, mChannel,
                          QMIX_PLAY_NOTIFYSTOP, mpWave,
                          nLoops, &playParams );
   QSOUND_ERROR_CHECK( res, "Start / Play" );
   
   return (res) ? 1 : DS_OK;
}

void
cQSndSample::LLPause( void )
{
   UINT res;

   assert(mpWave != NULL);
   res = QSWaveMixPauseChannel( mpMixDevice, mChannel, 0 );
   QSOUND_ERROR_CHECK( res, "LLPause" );
   SetFlags( kSndFlagReallyPaused );
}

void
cQSndSample::LLResume( void )
{
   UINT res;

   if ( mStateFlags & kSndFlagReallyPaused ) {

      res = QSWaveMixRestartChannel( mpMixDevice, mChannel, 0 );
      QSOUND_ERROR_CHECK( res, "LLResume" );
      ClearFlags( kSndFlagReallyPaused );

   } else {
      Start();
   }
}


void
cQSndSample::LLUnMute( void )
{
   MakeAudible();
}


BOOL
cQSndSample::IsPlaying()
{
   BOOL isDone;

   if ( mChannel >= 0 ) {
       isDone = QSWaveMixIsChannelDone( mpMixDevice, mChannel );
       if ( !isDone ) {
          return TRUE;
       }
   }

   return FALSE;
}

//
// Internal set volume routine
// 
static UINT
InternalSetVolume( HQMIXER pMixer,
                   int     channel,
                   int32   milliBelVolume )
{
   int      qVolume, powerOfTwo;
   // approximate conversion from exponential volume to linear using a 30-step
   //  per power of two table.
   static int mbToLin[] = {
      32767, 32018, 31287, 30572, 29874, 29192, 28525, 27873, 27237, 26615,
      26007, 25413, 24832, 24265, 23711, 23169, 22640, 22123, 21618, 21124,
      20641, 20170, 19709, 19259, 18819, 18389, 17969, 17559, 17158, 16766 };

   // convert from millibels -10000...-1 to linear volume 0...32767
   if ( milliBelVolume >= 0 ) {
      qVolume = 32767;
   } else if (milliBelVolume <= -9000) {
      qVolume = 0;
   } else {

      qVolume = ((-milliBelVolume) % 600) / 20;
      // tmp should now be in the range 0 .. 29
      qVolume = mbToLin[qVolume];
      // tmp is now the linear volume, ignoring the power of two

      powerOfTwo = milliBelVolume / -600;
      if ( powerOfTwo != 0 ) {
         qVolume >>= powerOfTwo;
      }
   }
   return QSWaveMixSetVolume( pMixer, channel, 0, qVolume );
}

// DirectSound pan values are actually in -10K ... 10K, but usually
//   only a portion of that range is used
#define PAN_LIMIT 3000
// TBD: see if PAN_LIMIT and 1 meter range work OK

//
// Internal set pan routine
//  QMixer doesn't have a pan setting, we fake it
//  by setting the position in polar coords
//
static UINT
InternalSetPan( HQMIXER pMixer,
                int     channel,
                int32   pan )
{
   UINT res;
   QSPOLAR polarPos;

   if ( pan > PAN_LIMIT ) {
      pan = PAN_LIMIT;
   } else if ( pan < (-PAN_LIMIT) ) {
      pan = (-PAN_LIMIT);
   }
   polarPos.azimuth = 90.0 * ( (float) pan / (float) PAN_LIMIT );
   polarPos.range = 1.0;            // 1 meter
   polarPos.elevation = 0.0;
   res = QSWaveMixSetPolarPosition( pMixer, channel, 0, &polarPos );

   return res;
}


////////////////////////////////////
//
// SetVolume - set the sample volume
//

void
cQSndSample::LLSetVolume( int32 vol )
{
   HRESULT res;

   TLOG3( "Smp::SetVolume %s %d + group %d", SAMPLE_NAME, mVolume, mGroupVolume );
   if ( mpWave != NULL ) {
      // negative values are used directly as milliBels (DirectSound units)
      if ( vol < kSndMinVolume ) vol = kSndMinVolume;
      if ( vol > kSndMaxVolume ) vol = kSndMaxVolume;
      res = InternalSetVolume( mpMixDevice, mChannel, vol );
      QSOUND_ERROR_CHECK( res, "SetVolume / SetVolume" );
   }

}


////////////////////////////////////
//
// SetPan - Set the pan position of the sample -10000 (left) ... 10000 (right)
//

STDMETHODIMP_(void) cQSndSample::SetPan(int32 pan)
{
   UINT res;

   mPan = pan;

   TLOG2( "Smp::SetPan %s %d", SAMPLE_NAME, mPan );
   if ( mpWave != NULL ) {
      if ( pan < kSndPanLeft ) {
         pan = kSndPanLeft;
      } else if ( pan > kSndPanRight ) {
         pan = kSndPanRight;
      }
      res = InternalSetPan( mpMixDevice, mChannel, pan );
      QSOUND_ERROR_CHECK( res, "SetPan / SetPan" );
   }
}

////////////////////////////////////
//
// SetFrequency - Set the frequency of the sample
//

STDMETHODIMP_(void) cQSndSample::SetFrequency(uint32 freq)
{
   DWORD now;
   UINT res;

   TLOG2( "Smp::SetFrequency %s %d", SAMPLE_NAME, freq );
   // clamp frequency to dsound supported range
   if ( freq < kSndMinFrequency ) {
      freq = kSndMinFrequency;
   } else if ( freq > kSndMaxFrequency ) {
      freq = kSndMaxFrequency;
   }
   if ( !IS_AUDIBLE ) {
      if ( IS_RUNNING ) {
         // muted & running - just recalculate current
         //  position before changing frequency
         now = timeGetTime();
         mBasePos = ESTIMATED_POSITION( now );
         mBaseTime = now;
         TLOG2( "Smp::SetFrequency - reset baseTime %ld basePos %ld", mBaseTime, mBasePos );
      }
   } else {
      // pass frequency down to dsound
      assert(mpWave != NULL);
      res = QSWaveMixSetFrequency( mpMixDevice, mChannel, 0, freq);
      QSOUND_ERROR_CHECK( res, "SetFrequency / SetFrequency" );
   }
   mFrequency = freq;
}


void
cQSndSample::LLSetPosition( uint32 )
{
   // set the hardware play position
   // TBD - there is no QMixer equivalent function...
}

///////////////////////////////////
//
// GetPosition - return the play position(in samples)
//

uint32
cQSndSample::LLGetPosition()
{
   DWORD playPos;
   UINT res;
   LPMIXWAVE pWave;

   assert(mpWave != NULL);
   res = QSWaveMixGetPlayPosition( mpMixDevice, mChannel, &pWave,
                                   &playPos, QMIX_POSITION_BYTES );
   QSOUND_ERROR_CHECK( res, "GetPosition / GetCurrentPosition" );
   TLOG3( "Smp::GetPosition %s hwpos = %d baseoff %d", SAMPLE_NAME, playPos, mBaseOffset );

   return playPos / mBytesPerSample;
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

STDMETHODIMP_(eSndError) cQSndSample::LoadBuffer(uint8 *data, uint32 len)
{
   // ALERT! THIS FUNCTION IS NOT SUPPORTED!
   assert( 0 );
   return LoadBufferIndirect( memCopyFunk, data, len );
}

////////////////////////////////////
// LoadBufferIndirect
//
// Attempt to lock the buffer and call app-supplied function to copy memory
// into it.  Give the data and the length to be copied.  This should be used
// for streaming samples
// 
STDMETHODIMP_(eSndError)
cQSndSample::LoadBufferIndirect( SndLoadFunction    funk,
                                void               *pFunkData,
                                uint32             len )
{
   TLOG3( "Smp::LoadBufferIndirect %s, %d byte pos %d", SAMPLE_NAME, len, mLastWrite + mBaseOffset );

   void *pReturnedData;

   if ( IS_STREAM && (len == 0) ) {
      //
      // Our caller has passed len 0, indicating that it is done passing data
      //  to this stream, so start the shutdown of the stream
      //
      //TBD! how do we tell QMixer that stream is shutting down?
      SetFlags( kSndFlagEndOfData );
      return kSndOk;
   }

   pReturnedData = funk( pFunkData, mpRefillDest, len );
   if ( pReturnedData != mpRefillDest ) {
      // getdata function put data somewhere other than mpRefillDest - move data
      memcpy( mpRefillDest, pReturnedData, len );
   }

   if ( mDumpFile != NULL ) {
      // dump the data
      fwrite( mpRefillDest, 1, len, mDumpFile );
   }

   mLastWrite += len;

   return kSndOk;
}

////////////////////////////////////
// BufferReady
//
// Takes: length of buffer
//
// Returns TRUE if buffer ready
//
//

STDMETHODIMP_(BOOL)
cQSndSample::BufferReady( uint32  /* len */ )
{
   // ALERT! THIS FUNCTION IS NOT SUPPORTED!
   assert( 0 );

   return TRUE;
}

////////////////////////////////////
// LockBuffer
//
// Internally used routine to try to lock the buffer

BOOL cQSndSample::LockBuffer( void **, uint32 *, void **, uint32 *, uint32 )
{
   // ALERT! THIS FUNCTION IS NOT SUPPORTED!
   assert( 0 );

   return TRUE;
}

void cQSndSample::UnlockBuffer(void *, uint32, void *, uint32)
{
   // ALERT! THIS FUNCTION IS NOT SUPPORTED!
   assert( 0 );
}

////////////////////////////////////
//
// AvailToWrite - return the number of bytes that can be written
//  to a stream sample's buffer
//
STDMETHODIMP_(uint32) cQSndSample::AvailToWrite( void )
{
   // ALERT! THIS FUNCTION IS NOT SUPPORTED!
   assert( 0 );

   return 0;
}


//
// CheckStream - stop stream if needed
//
STDMETHODIMP_(void)
cQSndSample::CheckStream()
{
   uint32 readPos;

   if ( IS_MUTED ) {
      // handle muted stream - look for end-of-data condition
      if ( IS_RUNNING ) {
         TLOG2( "Smp::CheckStream %s MUTED, readPos %d", SAMPLE_NAME, readPos );
         readPos = ESTIMATED_POSITION_NOW;
         if ( readPos >= mNumSamples ) {
            // stream sample has reached its virtual end-of-data
            DeferredStop();
         }
      }
      return;
   }

   assert( mpWave != NULL );

   switch( mState ) {
      case kSndStatePlaying:
      case kSndStateInited:
         if ( mStateFlags & kSndFlagEndOfData ) {
            // we are waiting for data already in buffer to be played
            //  before we can release sample
            if ( QSWaveMixIsChannelDone( mpMixDevice, mChannel ) ) {
               TLOG1("Smp::CheckStream %s stopping \n", SAMPLE_NAME );
               DeferredStop();
            }
         }
         break;

      case kSndStateCreated:
      case kSndStateDestroyed:
      case kSndStateStopped:
      default:
         break;
   }

}

//
// SilenceFill - fill nBytes with silence (zeroes)
//   advances mLastWrite
//
STDMETHODIMP_(void)
cQSndSample::SilenceFill( uint32 /* nBytes */ )
{
   // ALERT! THIS FUNCTION IS NOT SUPPORTED!
   assert( 0 );
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
cQSndSample::Set3DPosition( sSndVector *pPosition )
{
   UINT res;
   float distanceFactor;
   QSVECTOR qPosition;

   assert( pPosition != NULL );
   m3DPosition = *pPosition;

   if ( mChannel >= 0 ) {
      distanceFactor = ((cQSndMixer *) mpMixer)->GetDistanceFactor();
      qPosition.x = pPosition->x * distanceFactor;
      qPosition.y = (-pPosition->y) * distanceFactor;
      qPosition.z = pPosition->z * distanceFactor;

      res = QSWaveMixSetSourcePosition( mpMixDevice, mChannel, 0, &qPosition );
      QSOUND_ERROR_CHECK( res, "Set3DPosition" );
   }
}


////////////////////////////////////
//
// Set3DVelocity
//

STDMETHODIMP_(void)
cQSndSample::Set3DVelocity( sSndVector *pVelocity )
{
   HRESULT res;
   QSVECTOR vel;

   assert( pVelocity != NULL );
   m3DVelocity = *pVelocity;

   if ( mChannel >= 0 ) {
      vel.x = pVelocity->x;
      vel.y = (-pVelocity->y);
      vel.z = pVelocity->z;

      res = QSWaveMixSetSourceVelocity( mpMixDevice, mChannel, 0, &vel );
      QSOUND_ERROR_CHECK( res, "Set3DVelocity" );
   }
}


////////////////////////////////////
//
// Set3DConeAngles
//

STDMETHODIMP_(void)
cQSndSample::Set3DConeAngles( uint32   inside,
                              uint32   outside )
{
   HRESULT res;
   QSVECTOR cone;

   m3DConeInnerAngle = inside;
   m3DConeOuterAngle = outside;

   // TBD - the QMixer model only has 1 angle
   if ( mChannel >= 0 ) {
      cone.x = m3DConeOrientation.x;
      cone.y = (-m3DConeOrientation.y);
      cone.z = m3DConeOrientation.z;

      res = QSWaveMixSetSourceCone( mpMixDevice, mChannel, 0, &cone,
                                    m3DConeInnerAngle, mAmbientVolumeDB );
      QSOUND_ERROR_CHECK( res, "Set3DConeAngles" );
   }
}


////////////////////////////////////
//
// Set3DOrientation
//

STDMETHODIMP_(void)
cQSndSample::Set3DConeOrientation( sSndVector *pOrientation )
{
   HRESULT res;
   QSVECTOR cone;

   assert( pOrientation != NULL );
   m3DConeOrientation = *pOrientation;

   if ( mChannel >= 0 ) {
      cone.x = m3DConeOrientation.x;
      cone.y = (-m3DConeOrientation.y);
      cone.z = m3DConeOrientation.z;

      res = QSWaveMixSetSourceCone( mpMixDevice, mChannel, 0, &cone,
                                    m3DConeInnerAngle, mAmbientVolumeDB );
      QSOUND_ERROR_CHECK( res, "Set3DConeOrientation" );
   }
}


////////////////////////////////////
//
// Set3DDistanceRange
//

STDMETHODIMP_(void)
cQSndSample::Set3DDistanceRange( float dmin, float dmax )
{
   HRESULT res;
   QMIX_DISTANCES distances;

   m3DMinDistance = dmin;
   m3DMaxDistance = dmax;

   if ( mChannel >= 0 ) {
      distances.cbSize = sizeof( distances );
      distances.minDistance = dmin;
      distances.maxDistance = dmax;
      distances.scale = mRolloff;

      res = QSWaveMixSetDistanceMapping( mpMixDevice, mChannel, 0, &distances );
      QSOUND_ERROR_CHECK( res, "Set3DConeOrientation" );
   }
}


////////////////////////////////////
//
// Set3DMode
//

STDMETHODIMP_(void)
cQSndSample::Set3DMode( eSnd3DMode dmode )
{
   int mode;
   HRESULT res;

   m3DMode = dmode;

   if ( mChannel >= 0 ) {
      switch( dmode ) {
         default:
         case kSnd3DModeNormal:
            mode = QMIX_CHANNEL_QSOUND;
            break;
         case kSnd3DModeNone:
         case kSnd3DModeHeadRelative:
            mode = QMIX_CHANNEL_STEREO;
            break;
      }

      res = QSWaveMixEnableChannel( mpMixDevice, mChannel, 0, mode );
      QSOUND_ERROR_CHECK( res, "Set3DMode" );
   }
}


////////////////////////////////////
//
// SetAmbientVolume
//

STDMETHODIMP_(void)
cQSndSample::SetAmbientVolume( int32 vol )
{
   HRESULT res;
   QSVECTOR cone;

   mAmbientVolume = vol;
   mAmbientVolumeDB = vol / 100.0;
   if ( mChannel >= 0 ) {
      //TBD: should ambientDBs be negated??
      cone.x = m3DConeOrientation.x;
      cone.y = (-m3DConeOrientation.y);
      cone.z = m3DConeOrientation.z;

      res = QSWaveMixSetSourceCone( mpMixDevice, mChannel, 0, &cone,
                                    m3DConeInnerAngle, mAmbientVolumeDB );
      QSOUND_ERROR_CHECK( res, "SetAmbientVolume" );
   }
}

////////////////////////////////////
//
// Set3DMethod
//

STDMETHODIMP_(void)
cQSndSample::Set3DMethod( eSnd3DMethod method )
{
   m3DMethod = method;
}


