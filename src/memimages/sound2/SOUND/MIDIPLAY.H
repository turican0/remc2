////////////////////////////////////////////////////////////////////////
//
// (c) 1996 Looking Glass Technologies Inc.
// Pat McElhatton
//
// Module name: Midiplay
// File name: midiplay.h
//
// Description: Interface definition for player for XMIDI resources
//
////////////////////////////////////////////////////////////////////////

#ifndef _MIDI_PLAY_H
#define _MIDI_PLAY_H

#include <comtools.h>
#ifndef _LGSNDID_H
#include <lgsndid.h>
#endif


F_DECLARE_INTERFACE(IMidiSequencer);
F_DECLARE_INTERFACE(IMidiSequence);

#ifndef Id
// a little bit of evil - avoids including res.h
typedef ushort Id;			// ID of a resource
#endif

#ifdef __cplusplus
extern "C" {
#endif

// create the global sequencer
BOOL MidiCreateSequencer( IMidiSequencer **ppSequencer, IUnknown *pOuter );

#ifdef __cplusplus
const int32 kMidiMaxVolume = 127;
const int32 kMidiMinVolume = 0;
#else
#define kMidiMaxVolume 127
#define kMidiMinVolume 0
#endif

// status of a MidiSequence
typedef enum _eMidiSequenceStatus {
   kMidiSequenceDone,
   kMidiSequencePlaying,
   kMidiSequenceStopped
} eMidiSequenceStatus;

// when to switch between 2 MidiSequences
typedef enum _eMidiSwitchTime {
   kMidiSwitchNone,
   kMidiSwitchNow,
   kMidiSwitchAtBeat,
   kMidiSwitchAtMeasure,
   kMidiSwitchAtMarker,
   kMidiSwitchAtEnd
} eMidiSwitchTime;

// Midi device types
typedef enum {
   kMidiDeviceNone,
   kMidiDeviceFM,
   kMidiDeviceGM,
   kMidiDeviceUnknown
} eMidiDeviceType;

// optional actions when fade ends
typedef enum {
   kMidiFadeNone,
   kMidiFadeStop
} eMidiFadeFlags;

#ifdef _WIN32
typedef struct _sMidiSetup {
	void *hwnd;
} sMidiSetup, *pMidiSetup;
#endif

typedef void			(*MidiEndCallback)(IMidiSequence *, void *);


#undef INTERFACE
#define INTERFACE IMidiSequencer

DECLARE_INTERFACE_(IMidiSequencer, IUnknown)
{
   // Comtools macro to declare IUnknown methods
   DECLARE_UNKNOWN_PURE();

   STDMETHOD_(BOOL,              Init) ( THIS_ sMidiSetup *setup, int32 maxSequences ) PURE;
   STDMETHOD_(IMidiSequence *,   CreateSequence) ( THIS ) PURE;
   STDMETHOD_(void,              StopAll) ( THIS ) PURE;
   STDMETHOD_(void,              SetVolume) ( THIS_ int32 v ) PURE;
   STDMETHOD_(void,              Update) ( THIS ) PURE;
   STDMETHOD_(eMidiDeviceType,   GetDeviceType) ( THIS ) PURE;
};
#define IMidiSequencer_Init(p, a, b)            COMCall2(p, Init, a, b)
#define IMidiSequencer_CreateSequence(p)        COMCall0(p, CreateSequence)
#define IMidiSequencer_StopAll(p)               COMCall0(p, StopAll)
#define IMidiSequencer_FadeVolume(p,a,b)        COMCall2(p, FadeVolume, a, b)
#define IMidiSequencer_SetVolume(p,a)           COMCall1(p, SetVolume, a)
#define IMidiSequencer_Update(p)                COMCall0(p, Update)
#define IMidiSequencer_Release(p)               COMCall0(p, Release)
#define IMidiSequencer_GetDeviceType(p)         COMCall0(p, GetDeviceType)

#undef INTERFACE
#define INTERFACE IMidiSequence

DECLARE_INTERFACE_(IMidiSequence, IUnknown)
{
   // Comtools macro to declare IUnknown methods
   DECLARE_UNKNOWN_PURE();

   STDMETHOD_(BOOL,  Init) ( THIS_ Id seqId, int32 seqNum ) PURE;
   STDMETHOD_(void,  Start) ( THIS ) PURE;
   STDMETHOD_(void,  Stop) ( THIS ) PURE;
   STDMETHOD_(void,  Pause) ( THIS ) PURE;
   STDMETHOD_(void,  Resume) ( THIS ) PURE;
   STDMETHOD_(void,  FadeVolume) ( THIS_ int32 v, int32 msecs, eMidiFadeFlags flags ) PURE;
   STDMETHOD_(void,  SetVolume) ( THIS_ int32 v ) PURE;
   STDMETHOD_(void,  FadeTempoPercent) ( THIS_ int32 tempo, int32 msecs ) PURE;
   STDMETHOD_(void,  SetTempoPercent) ( THIS_ int32 tempo ) PURE;
   STDMETHOD_(void,  SetLoopCount) ( THIS_ int32 count ) PURE;
   STDMETHOD_(BOOL,  Switch) ( THIS_ IMidiSequence *n, eMidiSwitchTime when ) PURE;
   STDMETHOD_(void,  GetPosition) ( THIS_ int32 *pBeat, int32 *pMeasure ) PURE;
   STDMETHOD_(void,  RegisterEndCallback)(THIS_ MidiEndCallback cBack, void *p) PURE;
   STDMETHOD_(eMidiSequenceStatus,  Status) ( THIS ) PURE;
   STDMETHOD_(int32, GetVolume) (THIS) PURE;
};

#define IMidiSequence_Init(p, a, b)            COMCall2(p, Init, a, b)
#define IMidiSequence_Start(p)                 COMCall0(p, Start)
#define IMidiSequence_Stop(p)                  COMCall0(p, Stop)
#define IMidiSequence_Pause(p)                 COMCall0(p, Pause)
#define IMidiSequence_Resume(p)                COMCall0(p, Resume)
#define IMidiSequence_FadeVolume(p,a,b,c)      COMCall3(p, FadeVolume, a, b, c)
#define IMidiSequence_SetVolume(p,a)           COMCall1(p, SetVolume, a)
#define IMidiSequence_FadeTempoPercent(p,a,b)  COMCall2(p, FadeTempoPercent, a, b)
#define IMidiSequence_SetTempoPercent(p,a)     COMCall1(p, SetTempoPercent, a)
#define IMidiSequence_SetLoopCount(p,a)        COMCall1(p, SetLoopCount, a)
#define IMidiSequence_Switch(p,a,b)            COMCall2(p, Switch, a, b)
#define IMidiSequence_GetPosition(p,a,b)       COMCall2(p, GetPosition, a, b)
#define IMidiSequence_RegisterEndCallback(p,a,b) COMCall2(p, RegisterEndCallback, a, b)
#define IMidiSequence_Status(p)                COMCall0(p, Status)
#define IMidiSequence_GetVolume(p)             COMCall0(p, GetVolume)
#define IMidiSequence_Release(p)               COMCall0(p, Release)

#ifdef __cplusplus
	};
#endif

#endif
