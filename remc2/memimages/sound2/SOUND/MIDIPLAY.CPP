////////////////////////////////////////////////////////////////////////
// $Source: x:/prj/tech/libsrc/sound/RCS/midiplay.cpp $
// $Author: PATMAC $
// $Date: 1996/11/12 18:02:10 $
// $Revision: 1.11 $
//
// (c) 1996 Looking Glass Technologies Inc.
// Pat McElhatton
//
// Module name: Midiplay
// File name: midiplay.cpp
//
// Description: Player for XMIDI resources (a thin layer on top of WAIL)
//
////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <lg.h>
#include <mmsystem.h>
#include <wail.h>
#include <midplayi.h>
#include <waildynf.h>

// this is the WAIL sequence user data slot which is used
//  to point to the corresponding cMidiSequence object
typedef enum {
   kSeqDataThis            // ptr to cMidiSequence object
} eSeqUserDataEntries;

// TBDs:
// ? support XMIDI branch controllers
// - support indirect controller array
//
// call this to initially create the sequencer
//  returns FALSE for failure
//
BOOL
MidiCreateSequencer(IMidiSequencer  **ppSequencer,
                    IUnknown        *pOuter  )
{
   *ppSequencer = NULL;

	if( pOuter != NULL )
		return FALSE;

	*ppSequencer = new cMidiSequencer;

	return (*ppSequencer != NULL);
}


/*=========================================================
 *
 * cMidiSequence - a single midi sequence
 *
 *========================================================*/

////////////////////////////////////
// IMPLEMENT_UNAGGREGATABLE
//
// This does all the implementations of IUnknown methods
//

IMPLEMENT_UNAGGREGATABLE_SELF_DELETE(cMidiSequencer, IMidiSequencer);

cMidiSequencer::cMidiSequencer()
{
   mMidiDriverHandle = NULL;
   mpSequences = NULL;
}


cMidiSequencer::~cMidiSequencer()
{
   int i;
   cMidiSequence *pSeq;

   if ( mMidiDriverHandle != NULL ) {
      // close the midi driver
      DAIL_midiOutClose( mMidiDriverHandle );
      mMidiDriverHandle = NULL;
   }

   if ( mpSequences != NULL ) {
      // delete the active sequence table
      for ( i = 0; i < mMaxSequence; i++ ) {
         pSeq = mpSequences[i];
         if ( pSeq != NULL ) {
            if ( pSeq->Status() == SEQ_PLAYING ) {
               pSeq->Stop();
            }
            pSeq->Release();
         }
      }
      delete [] mpSequences;
      mpSequences = NULL;
   }

   // shutdown WAIL if the DLL is or can be loaded
   if ( LoadWail() ) {
      DAIL_shutdown();
   }
}


#ifdef __WATCOMC__
#pragma off(unreferenced)
#endif


//
// initialize the sequencer
//  return TRUE if failure occurs
//
STDMETHODIMP_(BOOL)
cMidiSequencer::Init(sMidiSetup  *pSetup,
                     int32       maxSequences )
{
//   cMidiSequence *pSeq;
   int i;
   S32 res;

   // see if we can load wail dll
   if ( LoadWail() == FALSE ) {
      // nope, couldn't load dll
      return TRUE;
   }

   // open a midi driver
   res = DAIL_startup();
   res = DAIL_set_preference( MDI_SEQUENCES, maxSequences );

   res = DAIL_midiOutOpen( &mMidiDriverHandle, &mpMidiOut, MIDI_MAPPER );
   //res = DAIL_midiOutOpen( &mMidiDriverHandle, NULL, 0 );
   if ( res != 0 ) {
      // mprintf( "cMidiSequencer failed to open midi out device!\n" );
      return TRUE;
   }

   // allocate our sequences
   mpSequences = new cMidiSequence *[ maxSequences ];
   mMaxSequence = maxSequences;
   for ( i = 0; i < mMaxSequence; i++ ) {
      mpSequences[i] = NULL;
   }

   SetVolume( kMidiMaxVolume );
   mbNeedFadeChecking = FALSE;

   return FALSE;
}
#ifdef __WATCOMC__
#pragma on(unreferenced)
#endif



//
// create a midi sequence
//
STDMETHODIMP_(IMidiSequence *)
cMidiSequencer::CreateSequence()
{
   int i;
   cMidiSequence *pSeq;

   pSeq = NULL;
   for ( i = 0; i < mMaxSequence; i++ ) {
      if ( mpSequences[i] == NULL ) {
         pSeq = new cMidiSequence( this, mMidiDriverHandle );
         mpSequences[i] = pSeq;
         break;
      }
   }

   return pSeq;
}


//
// this is a back-door interface which should only
//  be called by the cMidiSequence reference
//
STDMETHODIMP_(void)
cMidiSequencer::FreeSequence( cMidiSequence *pSeq )
{
   int i;

   for ( i = 0; i < mMaxSequence; i++ ) {
      if ( pSeq == mpSequences[i] ) {
         mpSequences[i] = NULL;
      }
   }
}


//
// stop all sequences
//
STDMETHODIMP_(void)
cMidiSequencer::StopAll()
{
   int i;

   for ( i = 0; i < mMaxSequence; i++ ) {
      if ( mpSequences[i] != NULL ) {
         mpSequences[i]->Stop();
      }
   }
}


//
// handle anything which requires polling of sequences
//
STDMETHODIMP_(void)
cMidiSequencer::Update()
{
   int i;
   cMidiSequence *pSeq;

   for ( i = 0; i < mMaxSequence; i++ ) {
      pSeq = mpSequences[i];
      if ( pSeq != NULL ) {
         // check each sequence which has a fade in progress, & fade flag stop
         //  to see if it has reached the fade final volume
         if ( pSeq->GetFadeFlags() == kMidiFadeStop ) {
            if ( pSeq->GetVolume() == pSeq->GetFadeEndVolume() ) {
               // fade has reached its destination, splat the sequence
               pSeq->Stop();
            } else {
               // there is at least one sequence which needs checking
               mbNeedFadeChecking = TRUE;
            }
         }

         // do app end-of-sequence callback if needed
         pSeq->CheckStop();

      }
   }
}


// set relative volume of sequence - 0 is full off, 127 is
//   max volume - absolute volume is combination of this
//   volume and the volume commands within the sequence
//
STDMETHODIMP_(void)
cMidiSequencer::SetVolume( int32 volume )
{
   DAIL_set_XMIDI_master_volume( mMidiDriverHandle, volume );
}


//
// return the midi out driver handle
//
STDMETHODIMP_(HMDIDRIVER)
cMidiSequencer::GetDriver( void )
{
   return mMidiDriverHandle;
}


//
// return the midi out driver handle
//
STDMETHODIMP_(eMidiDeviceType)
cMidiSequencer::GetDeviceType( void )
{
#ifdef XXX
   S32   AILType = DAIL_MDI_driver_type( mMidiDriverHandle );

   switch( AILType ) {
      case MDIDRVRTYPE_GM:    return kMidiDeviceGM;
      case MDIDRVRTYPE_FM_2:  return kMidiDeviceFM2Op;
      case MDIDRVRTYPE_FM_4:  return kMidiDeviceFM4Op;
   }

   return kMidiDeviceUnknown;
#else
   MIDIOUTCAPS devCaps;

   UINT result, devID;
   result = midiOutGetID( *mpMidiOut, &devID );
   if ( result != MMSYSERR_NOERROR ) {
      return kMidiDeviceUnknown;
   }
   result = midiOutGetDevCaps( devID, &devCaps, sizeof(devCaps) );
   if ( result != MMSYSERR_NOERROR ) {
      return kMidiDeviceUnknown;
   }

   // this should be replaced with something more specific - for
   //  now, just assume that synth is either FM or GeneralMidi
   if ( devCaps.wTechnology == MOD_FMSYNTH ) {
      return kMidiDeviceFM;
   } else {
      return kMidiDeviceGM;
   }
#endif
}


/*=========================================================
 *
 * cMidiSequence - a single midi sequence
 *
 *========================================================*/

////////////////////////////////////
// IMPLEMENT_UNAGGREGATABLE
//
// This does all the implementations of IUnknown methods
//

IMPLEMENT_UNAGGREGATABLE_SELF_DELETE(cMidiSequence, IMidiSequence);

cMidiSequence::cMidiSequence(cMidiSequencer  *owner,
                             HMDIDRIVER      midiDriverHandle )
{
   mSequence = DAIL_allocate_sequence_handle( midiDriverHandle );
   // save our self reference in a WAIL user data slot for callbacks
   DAIL_set_sequence_user_data( mSequence, kSeqDataThis, (S32) this );

   // no end-sequence callback
   mEndCallback = NULL;
   mpEndCBData = NULL;
   // have AIL call us on end-sequence no matter what
   DAIL_register_sequence_callback( mSequence, EndCallback );

   mpOwner = owner;
   mpSeqMemory = NULL;
   mRezID = 0;
   mpNextSeq = NULL;
   mbPleaseStop = FALSE;
}


cMidiSequence::~cMidiSequence()
{
   DAIL_release_sequence_handle( mSequence );
   // tell sequencer we are dead
   mpOwner->FreeSequence( this );

   // if we still have a sequence in memory, free it
   if ( mpSeqMemory ) {
      DMEM_free_lock( mpSeqMemory );
   }
}


//
// prepare a midi sequence for playback on this player
//  return TRUE if failure occurs
//
STDMETHODIMP_(BOOL)
cMidiSequence::Init( Id    seqID,
                     int32 seqNum )
{
   S32 result;
   int32 seqLen = ResSize( seqID );

   if ( mSequence == NULL ) {
      // allocation wasn't done, or failed
      return TRUE;
   }
   mSeqNum = seqNum;
   mFadeFlags = kMidiFadeNone;

   // if we still have a sequence in memory, free it
   if ( mpSeqMemory ) {
      Stop();
      DMEM_free_lock( mpSeqMemory );
   }
   // now allocate a new memory block & copy sequence there
   // using locked memory is a WAIL requirement
   mpSeqMemory = DMEM_alloc_lock( seqLen );
   // load the XMIDI resource into the locked memory
   ResExtract( seqID, mpSeqMemory );

   result = DAIL_init_sequence( mSequence, mpSeqMemory, mSeqNum );
   if ( result > 0 ) {
      // success - init tempo & volume
      SetTempoPercent( 100 );
      SetVolume( kMidiMaxVolume );
      return FALSE;
   } else {
      // failure occured
      return TRUE;
   }
}


STDMETHODIMP_(void)
cMidiSequence::Start()
{
   DAIL_start_sequence( mSequence );
}


STDMETHODIMP_(void)
cMidiSequence::Stop()
{
   DAIL_end_sequence( mSequence );
   EndCallback( mSequence );
   mFadeFlags = kMidiFadeNone;
}


STDMETHODIMP_(void)
cMidiSequence::Pause()
{
   DAIL_stop_sequence( mSequence );
}


STDMETHODIMP_(void)
cMidiSequence::Resume()
{
   DAIL_resume_sequence( mSequence );
}


//
// set relative volume of sequence - 0 is full off, 127 is
//   max volume - absolute volume is combination of this
//   volume and the volume commands within the sequence
//
STDMETHODIMP_(void)
cMidiSequence::FadeVolume( int32             volume,
                           int32             milliSeconds,
                           eMidiFadeFlags    flags)
{
   DAIL_set_sequence_volume( mSequence, volume, milliSeconds );
   mFadeFlags = flags;
   if ( flags == kMidiFadeStop ) {
      // tell sequencer to check for fade end
      mpOwner->NeedFadeChecking();
   }
   mFadeFinalVol = volume;
}


STDMETHODIMP_(void)
cMidiSequence::SetVolume( int32 volume )
{
   DAIL_set_sequence_volume( mSequence, volume, 0 );
}

//
// specify the tempo multiplier (100 means normal tempo)
//
STDMETHODIMP_(void)
cMidiSequence::SetTempoPercent( int32 tempo )
{
   DAIL_set_sequence_tempo( mSequence, tempo, 0 );
}


//
// make the tempo change over time
//
STDMETHODIMP_(void)
cMidiSequence::FadeTempoPercent( int32  tempo,
                                 int32  milliSeconds )
{
   DAIL_set_sequence_tempo( mSequence, tempo, milliSeconds );
}


//
// set # times sequence will loop (0 means forever)
//
STDMETHODIMP_(void)
cMidiSequence::SetLoopCount( int32 loopCount )
{
   DAIL_set_sequence_loop_count( mSequence, loopCount );
}


//
// stop this midi sequence and simultaneously start another
//   return TRUE if failure occurs
// if pNextSeq == NULL, just stop this sequence at switch time
//
STDMETHODIMP_(BOOL)
cMidiSequence::Switch( IMidiSequence   *pNextSeq,
                       eMidiSwitchTime when )
{
   mSwitchWhen = when;
   mpNextSeq = (cMidiSequence *) pNextSeq;
   switch( when ) {
      case kMidiSwitchNow:
         // just stop this sequence & start next
         Stop();
         mpNextSeq = NULL;
         if ( pNextSeq != NULL ) {
            pNextSeq->Start();
         }
         mSwitchWhen = kMidiSwitchNone;
         break;
      case kMidiSwitchAtBeat:
      case kMidiSwitchAtMeasure:
         // setup to switch on next beat
         DAIL_register_beat_callback( mSequence, BeatCallback );
         break;
      case kMidiSwitchAtMarker:
         // setup to switch on next marker (XMIDI Callback controller in seq)
         DAIL_register_trigger_callback( mSequence, MarkerCallback );
         break;
      case kMidiSwitchAtEnd:
         // setup to switch at end of sequence
         // callback set in constructor
         break;
   }

   return FALSE;
}


//
// return the play position of sequence
//
STDMETHODIMP_(void)
cMidiSequence::GetPosition( int32   *pBeat,
                            int32   *pMeasure )
{
   DAIL_sequence_position( mSequence, pBeat, pMeasure );
}


//
// return the playback status of sequence
//
STDMETHODIMP_(eMidiSequenceStatus)
cMidiSequence::Status( void )
{
   U32   AILStatus;
   eMidiSequenceStatus st;

   AILStatus = DAIL_sequence_status( mSequence );
   switch( AILStatus ) {
      case SEQ_FREE:
      case SEQ_DONE:          st = kMidiSequenceDone;          break;
      case SEQ_PLAYING:       st = kMidiSequencePlaying;       break;
      case SEQ_STOPPED:       st = kMidiSequenceStopped;       break;
   }
   return st;
}


//
// get the sequence volume
//
STDMETHODIMP_(int32)
cMidiSequence::GetVolume()
{
   return DAIL_sequence_volume( mSequence );
}

                    
//
// setup app callback which will occur when sequence play ends
//
STDMETHODIMP_(void)
cMidiSequence::RegisterEndCallback( MidiEndCallback   func,
                                    void              *pData )
{
   mEndCallback = func;
   mpEndCBData = pData;
}


// the WAIL callbacks below have several unused params

#ifdef __WATCOMC__
#pragma off(unreferenced)
#endif

//
// callback called every beat, used to switch sequences on beat/measure
//  unfortunately, this callback is shared by ALL sequences so it has
//  to run all the time (for simplicity)
//
void AILEXPORT
cMidiSequence::BeatCallback( HMDIDRIVER   pDriver,
                             HSEQUENCE    pSeq,
                             S32          beat,
                             S32          measure )
{
   cMidiSequence *pThis
      = (cMidiSequence *) DAIL_sequence_user_data( pSeq, kSeqDataThis );

   if ( (pThis->mSwitchWhen == kMidiSwitchAtBeat)
        || ((pThis->mSwitchWhen == kMidiSwitchAtMeasure) && (beat == 0)) ) {
      pThis->Stop();
      if ( pThis->mpNextSeq != NULL ) {
         pThis->mpNextSeq->Start();
      }
      // cancel callback
      DAIL_register_beat_callback( pSeq, NULL );
      pThis->mSwitchWhen = kMidiSwitchNone;
   }
}


//
// callback called when a marker is found in sequence, when
//   switching sequences on next marker
//
void AILEXPORT
cMidiSequence::MarkerCallback( HSEQUENCE  pSeq,
                               S32        markerChannel,
                               S32        markerValue )
{
   cMidiSequence *pThis
      = (cMidiSequence *) DAIL_sequence_user_data( pSeq, kSeqDataThis );

   if ( pThis->mSwitchWhen == kMidiSwitchAtMarker ) {
      pThis->Stop();
      if ( pThis->mpNextSeq != NULL ) {
         pThis->mpNextSeq->Start();
      }
      // cancel the marker callback
      DAIL_register_trigger_callback( pSeq, NULL );
      pThis->mSwitchWhen = kMidiSwitchNone;
   }
}


//
// callback called when sequence ends, do user callback if wanted,
//   switch sequences if needed
//
void AILEXPORT
cMidiSequence::EndCallback( HSEQUENCE  pSeq )
{
   cMidiSequence *pThis
      = (cMidiSequence *) DAIL_sequence_user_data( pSeq, kSeqDataThis );

   // switch to the next sequence if required
   if ( pThis->mSwitchWhen == kMidiSwitchAtEnd ) {
      if ( pThis->mpNextSeq != NULL ) {
         pThis->mpNextSeq->Start();
      }
      pThis->mSwitchWhen = kMidiSwitchNone;
   }

   // tell Update to call end CB next time around
   pThis->mbPleaseStop = TRUE;
}


#ifdef __WATCOMC__
#pragma on(unreferenced)
#endif

