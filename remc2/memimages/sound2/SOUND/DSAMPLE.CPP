////////////////////////////////////////////////////////////////////////
// $Header: x:/prj/tech/libsrc/sound/RCS/dsample.cpp 1.1 1998/03/20 11:45:53 PATMAC Exp $
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

#include <dlgsndi.h>
#include <mprintf.h>

#include <mixerlck.h>

#include <matrix.h>

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

IMPLEMENT_UNAGGREGATABLE_SELF_DELETE(cDSndSample, ISndSample);

///////////////////////////////////
// cDSndSample constructor
//
//  This just sets the state to something safe.
//  Init will tell us if everything is ok.
//

cDSndSample::cDSndSample( cDSndMixer              *pMixer,
                          eSndSampleCreateFlagSet flags )
   : cSndSample( (cSndMixer *) pMixer, flags )
{
   // DirectSound stuff
   mpSampleImp = NULL;
   mp3DBuffer = NULL;
   // End DirectSound stuff
   m3DMethod = kSnd3DMethodNone;
}


///////////////////////////////////
// cDSndSample destructor
//
// make sure that the implementor is gone
//

cDSndSample::~cDSndSample()
{
   MIXER_MUTEX;

   if ( mpSampleImp != NULL ) {
      mpMixer->DoTrace( (void *) mBufferLen, kSndBufferFree );
      mpSampleImp->Release();
      mpSampleImp = NULL;
      // tell the mixer we're gone & a channel is free
      mpMixer->FreeChannel();
   }
   if ( mp3DBuffer != NULL ) {
      mp3DBuffer->Release();
      mp3DBuffer = NULL;
   }
}

///////////////////////////////////
//
// MakeAudible - allocate and init a direct sound buffer
// Return TRUE if it succeeded
//
BOOL
cDSndSample::MakeAudible()
{
   DSBUFFERDESC         db;
   WAVEFORMATEX         fmt;
   IDirectSoundBuffer   *pDSB;
   HRESULT               res;
   MIXER_MUTEX;
   cDSndMixer           *pMixer = (cDSndMixer *) mpMixer;

   TLOG3( "Smp::MakeAudible %s %ld audible %ld inaudible",
         SAMPLE_NAME, mNumAudible, mNumInaudible );

   if ( IS_AUDIBLE ) {
      return TRUE;
   }

   // allocate a DirectSoundBuffer to this sample
   if ( mpMixer->AllocChannel( this ) == FALSE ) {
      // couldn't allocate a channel!
      return FALSE;
   }

   // init the DirectSound buffer descriptor
   memset( &db, 0, sizeof(db) );       // zero all unused/reserved fields
   db.dwBufferBytes = mBufferLen;
   db.dwSize = sizeof(db);
   if ( (m3DMethod == kSnd3DMethodSoftware) || (m3DMethod == kSnd3DMethodHardware) ) {
      // a 3D sample with volume & frequency control (no pan!)
      db.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
      if ( m3DMethod == kSnd3DMethodHardware ) {
         db.dwFlags |= DSBCAPS_LOCHARDWARE;
      }
   } else {
      // a non-3D sample with volume, frequency & pan control
      db.dwFlags = DSBCAPS_CTRLDEFAULT;
   }
   db.lpwfxFormat = &fmt;

   // init the wave format info
   memset( &fmt, 0, sizeof(fmt) );     // zero all unused/reserved fields
   fmt.wFormatTag = WAVE_FORMAT_PCM;
   fmt.nChannels = (WORD) mAttribs.nChannels;
   fmt.nSamplesPerSec = mAttribs.sampleRate;
   fmt.wBitsPerSample = (WORD) mAttribs.bitsPerSample;
   // the following assumes an integral bytes/sample (PCM, not ADPCM)
   fmt.nBlockAlign = (WORD) ((mAttribs.nChannels
                             * mAttribs.bitsPerSample) / 8);
   fmt.nAvgBytesPerSec = mAttribs.sampleRate * fmt.nBlockAlign;

   // create the DirectSoundBuffer
   res = pMixer->GetDevice()->CreateSoundBuffer( &db, &pDSB, NULL);
   if ( res == DS_OK ) {
      mpMixer->DoTrace( (void *) mBufferLen, kSndBufferAllocate );
      mpSampleImp = pDSB;
      MoveToList( mpMixer->AudibleHead() );
      SetFlags( kSndFlagAudible );

      // load shadowed state into hardware
      mGroupVolume = mpMixer->GetGroupVolume( mGroup ) + mpMixer->GetMasterVolume();
      switch ( m3DMethod ) {
         case kSnd3DMethodPanVol:
            SetPanVol3D();
            // tell mixer that pan/volume is in use
            ((cDSndMixer *) mpMixer)->UsePanVol();
            break;

         case kSnd3DMethodSoftware:
         case kSnd3DMethodHardware:
            // request DirectSound 3D support
            res = mpSampleImp->QueryInterface( IID_IDirectSound3DBuffer,
                                               (void **) &mp3DBuffer );
            //TBD: check res
            if ( res != DS_OK ) {
               // note that this masks kSndOnly8Bit
               return kSnd3DInitFailure;
            }
            Set3DPosition( &m3DPosition );
            Set3DVelocity( &m3DVelocity );
            Set3DConeAngles( m3DConeInnerAngle, m3DConeOuterAngle );
            Set3DConeOrientation( &m3DConeOrientation );
            Set3DDistanceRange( m3DMinDistance, m3DMaxDistance );
            Set3DMode( m3DMode );
            SetAmbientVolume( mAmbientVolume );
            // fall thru to set regular pan & volume below

         case kSnd3DMethodNone:
            SetVolume( mVolume );
            mpSampleImp->SetPan( mPan );
            break;
      }
      mpSampleImp->SetFrequency( mFrequency );

   } else {
      DSOUND_ERROR( res, "MakeAudible / CreateSoundBuffer" );
      return FALSE;
   }

   mLastWrite = 0;

   return TRUE;
}


void
cDSndSample::LLStop( void )
{
   HRESULT res;

   assert(mpSampleImp != NULL);
   res = mpSampleImp->Stop();
   DSOUND_ERROR_CHECK( res, "LLStop" );
}


void
cDSndSample::LLRelease( void )
{
   mpSampleImp->Release();
   mpSampleImp = NULL;
   if ( mp3DBuffer != NULL ) {
      mp3DBuffer->Release();
      mp3DBuffer = NULL;
   }
}

void
cDSndSample::LLInit( void )
{
}


HRESULT
cDSndSample::LLStart( void )
{
   HRESULT res;
   int flags;
   uint32 avail;

   if ( mStateFlags & (kSndFlagStream | kSndFlagLooped) ) {
      // resume in loop mode
      flags = DSBPLAY_LOOPING;
   } else {
      // resume one-shot
      flags = 0;
   }

   // the stream is audible, so preload it
   if ( fnFillCB != NULL ) {
      // if buffer has not been filled, do so now
      avail = AvailToWrite();
      if ( avail != 0 ) {
         (fnFillCB)( this, mpFillCBData, avail );
         ClearFlags( kSndFlagResyncNeeded );
      }
   }
   assert(mpSampleImp != NULL);
   if ( mInitBuffPos != -1 ) {
      // user specified to start play at position in mInitBuffPos
      SetPosition( mInitBuffPos );
      mInitBuffPos = -1;
   }
   res = mpSampleImp->Play(0,0,flags);
   DSOUND_ERROR_CHECK( res, "LLStart" );

   return res;
}

void
cDSndSample::LLPause( void )
{
   HRESULT res;

   assert(mpSampleImp != NULL);
   res = mpSampleImp->Stop();
   DSOUND_ERROR_CHECK( res, "LLPause" );
}

void
cDSndSample::LLResume( void )
{
   Start();
}

void
cDSndSample::LLUnMute( void )
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


BOOL
cDSndSample::IsPlaying()
{
   DWORD stat;
   if ( mpSampleImp == NULL ) {
      return FALSE;
   } else {
      mpSampleImp->GetStatus( &stat );
      return stat & DSBSTATUS_PLAYING;
   }
}

////////////////////////////////////
//
// SetVolume - set the sample volume
//

void
cDSndSample::LLSetVolume( int32 vol )
{
   HRESULT res;

   TLOG3( "Smp::SetVolume %s %d + group %d", SAMPLE_NAME, mVolume, mGroupVolume );
   if ( mpSampleImp != NULL ) {
 
      if ( m3DMethod == kSnd3DMethodPanVol ) {
         SetPanVol3D();
      } else {
         if ( vol < kSndMinVolume ) vol = kSndMinVolume;
         if ( vol > kSndMaxVolume ) vol = kSndMaxVolume;
         res = mpSampleImp->SetVolume(vol);
         DSOUND_ERROR_CHECK( res, "SetVolume / SetVolume" );
      }
   }

}


//
// do fake 3d localization by setting volume & pan, based on
//  the sample volume & 3D position
//
void
cDSndSample::SetPanVol3D( void )
{
   int32 vol, pan;
   HRESULT res;

   if ( mpSampleImp != NULL ) {
      mpMixer->Get3DPositionPanVolume( &m3DPosition, &pan, &vol );
      vol += (mVolume + mGroupVolume);
      TLOG3( "Smp::SetPanVol3D %s pan %d vol %d", SAMPLE_NAME, pan, vol );

      if ( vol < kSndMinVolume ) vol = kSndMinVolume;
      if ( vol > kSndMaxVolume ) vol = kSndMaxVolume;
      res = mpSampleImp->SetVolume(vol);
      DSOUND_ERROR_CHECK( res, "SetPanVol3D / SetVolume" );

      if ( pan < kSndPanLeft ) {
         pan = kSndPanLeft;
      } else if ( pan > kSndPanRight ) {
         pan = kSndPanRight;
      }
      res = mpSampleImp->SetPan( pan );
      DSOUND_ERROR_CHECK( res, "SetPanVol3D / SetPan" );
   }

}


////////////////////////////////////
//
// SetPan - Set the pan position of the sample -10000 (left) ... 10000 (right)
//

STDMETHODIMP_(void) cDSndSample::SetPan(int32 pan)
{
   HRESULT res;

   mPan = pan;

   TLOG2( "Smp::SetPan %s %d", SAMPLE_NAME, mPan );
   if ( mpSampleImp != NULL ) {
      if ( pan < kSndPanLeft ) {
         pan = kSndPanLeft;
      } else if ( pan > kSndPanRight ) {
         pan = kSndPanRight;
      }
      res = mpSampleImp->SetPan( pan );
      DSOUND_ERROR_CHECK( res, "SetPan / SetPan" );
   }
}

////////////////////////////////////
//
// SetFrequency - Set the frequency of the sample
//

STDMETHODIMP_(void) cDSndSample::SetFrequency(uint32 freq)
{
   DWORD now;
   HRESULT res;

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
      assert(mpSampleImp != NULL);
      res = mpSampleImp->SetFrequency(freq);
      DSOUND_ERROR_CHECK( res, "SetFrequency / SetFrequency" );
   }
   mFrequency = freq;
}


void
cDSndSample::LLSetPosition( uint32   pos )
{
   HRESULT res;

   // set the hardware play position
   res = mpSampleImp->SetCurrentPosition( pos * mBytesPerSample );
   DSOUND_ERROR_CHECK( res, "SetPosition / SetCurrentPosition" );
}


uint32
cDSndSample::LLGetPosition( void )
{
   uint32 writePos, playPos;
   HRESULT res;

   // get position in DirectSoundBuffer
   assert(mpSampleImp != NULL);
   res = mpSampleImp->GetCurrentPosition( &playPos, &writePos );
   DSOUND_ERROR_CHECK( res, "GetPosition / GetCurrentPosition" );
   TLOG3( "Smp::GetPosition %s hwpos = %d baseoff %d", SAMPLE_NAME, playPos, mBaseOffset );

   // for streams, handle fact that play is looping on buffer
   if ( playPos > mLastWrite ) {
      // play position after write position means that play
      //  position base is one buffer length behind write
      //  position base
      playPos += (mBaseOffset - mBufferLen);
   } else {
      playPos += mBaseOffset;
   }

   return playPos / mBytesPerSample;
}


////////////////////////////////////
// LoadBufferIndirect
//
// Attempt to lock the buffer and call app-supplied function to copy memory
// into it.  Give the data and the length to be copied.  This should be used
// for streaming samples
// 
STDMETHODIMP_(eSndError)
cDSndSample::LoadBufferIndirect( SndLoadFunction    funk,
                                void               *pFunkData,
                                uint32             len )
{
   assert(mpSampleImp != NULL);
   TLOG3( "Smp::LoadBufferIndirect %s, %d byte pos %d", SAMPLE_NAME, len, mLastWrite + mBaseOffset );

   void *p1, *p2, *p11, *p22;
   uint32 sz1, sz2;

   if ( IS_STREAM && (len == 0) ) {
      //
      // Our caller has passed len 0, indicating that it is done passing data
      //  to this stream, so start the shutdown of the stream
      //
      if ( (mState == kSndStatePlaying) || (mState == kSndStateInited) ) {
         // remember the last write point, needed for detecting when
         //  playback really completes.  We can't use mLastWrite for this,
         //  since it will be changed by silence filling
         mFinalWrite = mLastWrite;
         // silence-fill part of buffer holding data which has already been played
         uint32 avail = AvailToWrite();
         TLOG3("Smp:LoadBufferIndirect avail %ld, mLastWrite %ld, mLastRead %ld",
              avail, mLastWrite, mLastRead );
         SilenceFill( avail );
//TBD silence fill (do we need to worry about completely full buffer case?)
      }
      SetFlags( kSndFlagEndOfData );
      return kSndOk;
   }

   if ( LockBuffer( &p1, &sz1, &p2, &sz2, len) ) {
      p11 = funk( pFunkData, p1, sz1 );
      if ( p11 != p1 ) {
         // getdata function put data somewhere other than p1 - move data
         memcpy( p1, p11, sz1 );
      }

      TLOG2( "    section one %d bytes @ 0x%lx", sz1, (long) p1 );
      if ( sz2 ) {
         p22 = funk( pFunkData, p2, sz2 );
         if ( p22 != p2 ) {
            // getdata function put data somewhere other than p2 - move data
            memcpy( p2, p22, sz2 );
         }
         mpWriteBase = (char *) p2;
         TLOG2( "    section two %d bytes @ 0x%lx", sz2, (long) p2 );
      }

      if ( mDumpFile != NULL ) {
         // dump the data
         fwrite( p1, 1, sz1, mDumpFile );
         if ( sz2 != 0 ) {
            fwrite( p2, 1, sz2, mDumpFile );
         }
      }
      UnlockBuffer(p1, sz1, p2, sz2);

      mLastWrite += len;

      if ( IS_STREAM && (mLastWrite >= mBufferLen) ) {
         mLastWrite -= mBufferLen;
         mBaseOffset += mBufferLen;
      }

      if ( (sz1 + sz2) != len ) {
         // DEBUG! spot for a breakpoint...
         sz1 = sz2;
      }
         
      return kSndOk;
   }

   // DEBUG! spot for a breakpoint...
   TBD( "what if LockBuffer fails?" );
   return kSndUnknownError;
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
cDSndSample::BufferReady( uint32  len )
{
   // we're always ready for nothing
   if ( len == 0 ) {
      return TRUE;
   }

   if ( mpSampleImp == NULL ) {
      return FALSE;
   }

   // now see if we're reading where we want to be writing...
   return ( len > AvailToWrite() ) ? FALSE : TRUE;
}

////////////////////////////////////
// LockBuffer
//
// Internally used routine to try to lock the buffer

BOOL cDSndSample::LockBuffer(void **p1, uint32 *sz1, void **p2, uint32 *sz2, uint32 len)
{
   HRESULT res;
   uint32 playPos, writePos, availWrite;

   assert(mpSampleImp != NULL);

   res = mpSampleImp->GetCurrentPosition( &playPos, &writePos );
   DSOUND_ERROR_CHECK( res, "LockBuffer / GetCurrentPosition" );
   TLOG3( "Smp::LockBuffer writePos %d, playPos %d, %d bytes", mLastWrite, playPos, len );

   // see if there is enough room in buffer for data
   availWrite = AvailToWrite( playPos );
   if ( len > availWrite ) {
      TLOG3( "!!Smp::LockBuffer %s avail to write %d, %d bytes!!", SAMPLE_NAME, availWrite, len );
      return FALSE;
   }

   // try to lock the requested range
   res = mpSampleImp->Lock(mLastWrite, len, p1, sz1, p2, sz2, 0);
   DSOUND_ERROR_CHECK( res, "LockBuffer / Lock 2nd try" );

   // this wont happen too often.  I think it happens when
   // we've been suspended or if the hardware is physically removed
   if( res == DSERR_BUFFERLOST ) {
      TLOG1( "!!Smp::LockBuffer %s lost DSound buffer, attempting restore", SAMPLE_NAME );
      // try once to restore
      mpSampleImp->Restore();

      // do the lock from the last known write position
      res = mpSampleImp->Lock(mLastWrite, len, p1, sz1, p2, sz2, 0);
      DSOUND_ERROR_CHECK( res, "LockBuffer / Lock 2nd try" );
   }
   return (SUCCEEDED(res));

}


void
cDSndSample::UnlockBuffer(void *p1, uint32 sz1, void *p2, uint32 sz2)
{
   mpSampleImp->Unlock(p1, sz1, p2, sz2);
}


////////////////////////////////////
//
// AvailToWrite - return the number of bytes that can be written
//  to a stream sample's buffer (internal version)
//

uint32
cDSndSample::AvailToWrite( uint32 readPos )
{
   uint32 avail;
   if ( IS_STREAM ) {
      if ( mLastWrite < readPos ) {
         // readPos is ahead of last write position in buffer, valid data
         // wraps around end of buffer.  Free space starts at mLastWrite
         // and ends at readPos
         avail = readPos - mLastWrite;
      } else {
         // readPos is behind last write position in buffer, valid data does not
         // wrap arounc end of buffer.  Free space starts at mLastWrite, wraps
         // around buffer end, and ends at readPos
         avail = mBufferLen - (mLastWrite - readPos);
      }

      // don't fill the last few bytes of buffer - full buffer is indistinguishable
      //  from empty buffer
      if ( avail > 4 ) {
         avail -= 4;
      } else {
         avail = 0;
      }
   } else {
      // for one-shots, no need to worry about buffer wraparound
      avail = mBufferLen - mLastWrite;
   }

   TLOG3( "Smp::AvailToWrite %s %ld bytes pos %ld", SAMPLE_NAME, avail, readPos );
   return avail;
}


//
// AvailToWrite - return the number of bytes that can be written
//  to a stream sample's buffer
//
STDMETHODIMP_(uint32) cDSndSample::AvailToWrite( void )
{
   uint32 playPos, writePos;
   HRESULT res;

   if ( !IS_AUDIBLE ) {
      // don't have a DirectSound buffer - can't load any data
      return 0;
   }

   res = mpSampleImp->GetCurrentPosition( &playPos, &writePos );
   DSOUND_ERROR_CHECK( res, "AvailToWrite / GetCurrentPosition" );

   return AvailToWrite( playPos );
}


//
// CheckStream - call stream fill callback, stop if needed
//
STDMETHODIMP_(void)
cDSndSample::CheckStream()
{
   uint32 prevReadPos = mLastRead;
   uint32 writePos;
   uint32 readPos;
   BOOL doStop;
   HRESULT res;

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

   assert(mpSampleImp != NULL);

   res = mpSampleImp->GetCurrentPosition( &mLastRead, &writePos );
   DSOUND_ERROR_CHECK( res, "CheckStream / GetCurrentPosition" );
   uint32 avail = AvailToWrite( mLastRead );
   TLOG3( "Smp::CheckStream %s, availToWrite %d, readPos %d", SAMPLE_NAME, avail, mLastRead );

   switch( mState ) {
      case kSndStatePlaying:
      case kSndStateInited:
         if ( mStateFlags & kSndFlagEndOfData ) {
            // we are waiting for data already in buffer to be played
            //  before we can release sample
            doStop = FALSE;
            if ( prevReadPos < mFinalWrite ) {
               // available play data does not wrap around
               if ( (mLastRead > mFinalWrite) || (mLastRead < prevReadPos) ) {
                  // ring buffer is empty
                  TLOG3("Smp::CheckStream %s stopping no-wrap lastWrite %d, prevRead %d\n",
                       SAMPLE_NAME, mFinalWrite, prevReadPos );
                  doStop = TRUE;
               }
            } else {
               // available play data does wrap around
               if ( (mLastRead > mFinalWrite) && (mLastRead < prevReadPos) ) {
                  // ring buffer is empty
                  TLOG3("Smp::CheckStream %s stopping wrap lastWrite %d, prevRead %d\n",
                       SAMPLE_NAME, mFinalWrite, prevReadPos );
                  doStop = TRUE;
               }
            }
            if ( doStop ) {
               DeferredStop();
            } else {
               // fill region which has been played since previous call with silence
               SilenceFill( avail );
            }

         } else {

            // still really playing - let's move some data!
            if ( (fnFillCB != NULL) && (avail != 0) ) {
               // let app add data
               (fnFillCB)( this, mpFillCBData, avail );
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
STDMETHODIMP_(void) cDSndSample::SilenceFill( uint32 nBytes )
{
   void    *p1, *p2;
   uint32  len1, len2;
   uint8 silenceValue;

   assert(mpSampleImp != NULL);
   TLOG3( "Smp::SilenceFill %s, %ld bytes, writePos %ld", SAMPLE_NAME, nBytes, mLastWrite );

   if ( nBytes == 0 ) {
      return;
   }

   // 8-bit samples are unsigned - this will break if we ever have signed 8-bit samples...
   silenceValue = ( mAttribs.bitsPerSample == 8 ) ? 0x80 : 0;

   if ( LockBuffer( &p1, &len1, &p2, &len2, nBytes ) ) {
      memset( p1, silenceValue, len1 );
      if ( len2 != 0 ) {
         memset( p2, silenceValue, len2 );
      }
      if ( mDumpFile != NULL ) {
         // dump the data
         fwrite( p1, 1, len1, mDumpFile );
         if ( len2 != 0 ) {
            fwrite( p2, 1, len2, mDumpFile );
         }
      }
      UnlockBuffer( p1, len1, p2, len2 );
      mLastWrite += nBytes;
      if ( mLastWrite >= mBufferLen ) mLastWrite -= mBufferLen;
   } else {
      TBD( "what if LockBuffer fails? (silence fill)" );
   }
}


/************************************************************
 *
 *   3  D      S  T  U  F  F
 *
 *
 */

// NOTE:  DS3D's coordinate system have Y as up, and X & Z form the ground plane
// while LG's coords have Z as up, and X & Y form the ground plane:
//
//    DS                   LG
//
//    Y                    Z
//    ^   Z                ^   X
//    |  /                 |  /
//    | /                  | /
//    |/                   |/
//    |------>X      Y<----|
//
//    X                   -Y
//    Y                    Z
//    Z                    X


////////////////////////////////////
//
// Set3DPosition
//

STDMETHODIMP_(void)
cDSndSample::Set3DPosition( sSndVector *pPosition )
{
   HRESULT res;

   assert( pPosition != NULL );
   TLOG3( "Smp::Set3DPosition %s %d %d",
          SAMPLE_NAME, pPosition->x, pPosition->y );
   m3DPosition = *pPosition;
   switch( m3DMethod ) {

      case kSnd3DMethodPanVol:
         SetPanVol3D();
         break;

      case kSnd3DMethodNone:
         break;

      default:
         if ( mp3DBuffer != NULL ) {
            res = mp3DBuffer->SetPosition( -(pPosition->y), pPosition->z, pPosition->x,
                                           mpMixer->Get3DDeferFlag() );
            DSOUND_ERROR_CHECK( res, "Smp:Set3DPosition failed" );
         }
         break;
   }
}


////////////////////////////////////
//
// Set3DVelocity
//

STDMETHODIMP_(void)
cDSndSample::Set3DVelocity( sSndVector *pVelocity )
{
   HRESULT res;

   assert( pVelocity != NULL );
   TLOG3( "Smp::Set3DVelocity %d %d %d",
          pVelocity->x, pVelocity->y, pVelocity->z );

   if ( mp3DBuffer != NULL ) {
      res = mp3DBuffer->SetVelocity( -(pVelocity->y), pVelocity->z, pVelocity->x,
                                     mpMixer->Get3DDeferFlag() );
      DSOUND_ERROR_CHECK( res, "Smp:Set3DVelocity failed" );
   }
   m3DVelocity = *pVelocity;
}


////////////////////////////////////
//
// Set3DConeAngles
//

STDMETHODIMP_(void)
cDSndSample::Set3DConeAngles( uint32 inside, uint32 outside )
{
   HRESULT res;

   TLOG3( "Smp::Set3DConeAngles %s inside %d outside %d", SAMPLE_NAME, inside, outside );
   if ( mp3DBuffer != NULL ) {
      res = mp3DBuffer->SetConeAngles( inside, outside,
                                       mpMixer->Get3DDeferFlag() );
      DSOUND_ERROR_CHECK( res, "Smp:Set3DConeAngles failed" );
   }
   m3DConeInnerAngle = inside;
   m3DConeOuterAngle = outside;
}


////////////////////////////////////
//
// Set3DOrientation
//

STDMETHODIMP_(void)
cDSndSample::Set3DConeOrientation( sSndVector *pOrientation )
{
   HRESULT res;

   assert( pOrientation != NULL );
   TLOG3( "Smp::Set3DConeOrientation x100 %d %d %d",
          pOrientation->x * 100, pOrientation->y * 100, pOrientation->z * 100 );
   if ( mp3DBuffer != NULL ) {
      res = mp3DBuffer->SetConeOrientation( -(pOrientation->y), pOrientation->z, pOrientation->x,
                                            mpMixer->Get3DDeferFlag() );
      DSOUND_ERROR_CHECK( res, "Smp:Set3DConeOrientation failed" );
   }
   m3DConeOrientation = *pOrientation;
}


////////////////////////////////////
//
// Set3DDistanceRange
//

STDMETHODIMP_(void)
cDSndSample::Set3DDistanceRange( float dmin, float dmax )
{
   HRESULT res;

   TLOG3( "Smp::Set3DDistanceRange %s min %d max %d", SAMPLE_NAME, dmin, dmax );
   if ( mp3DBuffer != NULL ) {
      res = mp3DBuffer->SetMinDistance( dmin, DS3D_DEFERRED );
      DSOUND_ERROR_CHECK( res, "Smp:Set3DDistanceRange Min failed" );
      res = mp3DBuffer->SetMaxDistance( dmax, mpMixer->Get3DDeferFlag() );
      DSOUND_ERROR_CHECK( res, "Smp:Set3DDistanceRange Max failed" );
   }
   m3DMinDistance = dmin;
   m3DMaxDistance = dmax;
}


////////////////////////////////////
//
// Set3DMode
//

STDMETHODIMP_(void)
cDSndSample::Set3DMode( eSnd3DMode dmode )
{
   uint32 mode;
   HRESULT res;

   TLOG2( "Smp::Set3DMode %s %d", SAMPLE_NAME, dmode );
   switch( dmode ) {
      default:
      case kSnd3DModeNone:          mode = DS3DMODE_DISABLE;         break;
      case kSnd3DModeNormal:        mode = DS3DMODE_NORMAL;          break;
      case kSnd3DModeHeadRelative:  mode = DS3DMODE_HEADRELATIVE;    break;
   }

   if ( mp3DBuffer != NULL ) {
      res = mp3DBuffer->SetMode( mode, mpMixer->Get3DDeferFlag() );
      DSOUND_ERROR_CHECK( res, "Smp:Set3DMode failed" );
   }
      
   m3DMode = dmode;
}


////////////////////////////////////
//
// SetAmbientVolume
//

STDMETHODIMP_(void)
cDSndSample::SetAmbientVolume( int32 vol )
{
   HRESULT res;

   TLOG2( "Smp::SetAmbientVolume %s %d", SAMPLE_NAME, vol );
   if ( mp3DBuffer != NULL ) {
      res = mp3DBuffer->SetConeOutsideVolume( vol, mpMixer->Get3DDeferFlag() );
      DSOUND_ERROR_CHECK( res, "Smp:SetAmbientVolume failed" );
   }
   mAmbientVolume = vol;
}


////////////////////////////////////
//
// Set3DMethod
//

STDMETHODIMP_(void)
cDSndSample::Set3DMethod( eSnd3DMethod method )
{
   // assert if this is called after Init
   assert( mpSampleImp == NULL );
   TLOG2( "Smp::Set3DMethod %s %d", SAMPLE_NAME, method );

   m3DMethod = method;

   if ( m3DMethod == kSnd3DMethodPanVol ) {
      // tell mixer that pan/volume is in use
      ((cDSndMixer *) mpMixer)->UsePanVol();
   }
}


//
// redo pan/volume for audible samples
//  return FALSE if there are no samples using pan/vol for 3D
//
BOOL
cDSndSample::UpdatePanVol3D( void )
{
   cDSndSample *pSample, *pNext;
   BOOL usingPanVol;

   // This is invoked by the mixer when the listener position or orientation changes
   usingPanVol = FALSE;
   // we are the audible list head
   pSample = this;
   while ( pSample != NULL ) {
      pNext = (cDSndSample *) (pSample->mpNext);
      if ( pSample->m3DMethod == kSnd3DMethodPanVol ) {
         pSample->SetPanVol3D();
         // we found at least one audible sample using panvol, so
         // tell mixer to keep updating panvol when listener 3D changes
         usingPanVol = TRUE;
      }
      pSample = pNext;
   }

   return usingPanVol;
}
