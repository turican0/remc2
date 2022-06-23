////////////////////////////////////////////////////////////////////////
//
// (c) 1996 Looking Glass Technologies Inc.
// Pat McElhatton
//
// Module name: Midplayi
// File name: midplayi.h
//
// Description: Internal interface definition for player for XMIDI resources
//
////////////////////////////////////////////////////////////////////////

#ifndef _MIDI_PLAY_H
#include <midiplay.h>
#endif

extern "C" {
#include <res.h>
};

class cMidiSequence;

class cMidiSequencer : public IMidiSequencer
{
public:
	DECLARE_UNAGGREGATABLE();

public:
   cMidiSequencer();
   virtual ~cMidiSequencer();
   STDMETHOD_(BOOL,           Init) ( sMidiSetup *setup, int32 maxSequences );
   STDMETHOD_(IMidiSequence *,CreateSequence) ( void );
   STDMETHOD_(void,           StopAll) ( void );
   STDMETHOD_(void,           SetVolume) ( int32 v );
   STDMETHOD_(void,           Update) ( void );
   STDMETHOD_(eMidiDeviceType, GetDeviceType) ( void );

   // this should only be called by cMidiSequence::Release!
   STDMETHOD_(void,           FreeSequence) ( cMidiSequence *seq );
   STDMETHOD_(HMDIDRIVER,     GetDriver) ( void );

   void NeedFadeChecking( void );

protected:
   HMDIDRIVER        mMidiDriverHandle;      // midi driver handle
   HMIDIOUT          *mpMidiOut;             // midi out device handle

   int32             mMaxSequence;           // number of sequences in mpSequences array
   cMidiSequence     **mpSequences;          // allocated sequences
   BOOL              mbNeedFadeChecking;     // if true, mixer must check for vol fades ending
};

// cMidiSequence calls this when a fade-with-stop is started
inline void
cMidiSequencer::NeedFadeChecking()
{
   mbNeedFadeChecking = TRUE;
}


class cMidiSequence : public IMidiSequence
{
public:
	DECLARE_UNAGGREGATABLE();
public:
   cMidiSequence( cMidiSequencer *owner, HMDIDRIVER driver );
   virtual ~cMidiSequence();
   STDMETHOD_(BOOL,  Init) ( Id seqID, int32 seqNum );
   STDMETHOD_(void,  Start) ( void );
   STDMETHOD_(void,  Stop) ( void );
   STDMETHOD_(void,  Pause) ( void );
   STDMETHOD_(void,  Resume) ( void );
   STDMETHOD_(void,  FadeVolume) ( int32 v, int32 msecs, eMidiFadeFlags flags );
   STDMETHOD_(void,  SetVolume) ( int32 v );
   STDMETHOD_(void,  FadeTempoPercent) ( int32 tempo, int32 msecs );
   STDMETHOD_(void,  SetTempoPercent) ( int32 tempo );
   STDMETHOD_(void,  SetLoopCount) ( int32 count );
   STDMETHOD_(BOOL,  Switch) ( IMidiSequence *s, eMidiSwitchTime when );
   STDMETHOD_(void,  GetPosition) ( int32 *pBeat, int32 *pMeasure );
   STDMETHOD_(void,  RegisterEndCallback) ( MidiEndCallback func,
                                          void *callbackData );
   STDMETHOD_(eMidiSequenceStatus,  Status) ( void );
   STDMETHOD_(int32, GetVolume) ( void );

   static void AILEXPORT BeatCallback( HMDIDRIVER d, HSEQUENCE seq, S32 b, S32 m );
   static void AILEXPORT MarkerCallback( HSEQUENCE  pSeq, S32 chan, S32 val );
   static void AILEXPORT EndCallback( HSEQUENCE  pSeq );

   eMidiFadeFlags GetFadeFlags();
   int32          GetFadeEndVolume();
   void           CheckStop();

protected:
   HSEQUENCE            mSequence;     // the WAIL sequence handle
   Id                   mRezID;        // resource ID for the sequence
   int32                mSeqNum;       // which subsequence is playing
   cMidiSequencer       *mpOwner;      // the sequencer who owns us
   void                 *mpSeqMemory;  // our copy of the sequence memory
   cMidiSequence        *mpNextSeq;    // sequence to Switch to after this one
   eMidiSwitchTime      mSwitchWhen;   // switch at beat, marker or end
   MidiEndCallback      mEndCallback;  // call this when sequence ends
   void                 *mpEndCBData;  // data for above callback
   eMidiFadeFlags       mFadeFlags;    // flags set by FadeVolume
   int32                mFadeFinalVol; // final volume of fade
   BOOL                 mbPleaseStop;  // TRUE iff Update should call user endCB
};

inline void
cMidiSequence::CheckStop()
{
   if ( mbPleaseStop ) {
      // do the app end-of-sequence callback
      if ( mEndCallback ) {
         (mEndCallback) ( this, mpEndCBData );
      }
   }
}

inline eMidiFadeFlags
cMidiSequence::GetFadeFlags()
{
   return mFadeFlags;
}


inline int32
cMidiSequence::GetFadeEndVolume()
{
   return mFadeFinalVol;
}

