////////////////////////////////////////////////////////////////////////
// $Source: x:/prj/tech/libsrc/sound/RCS/lgsound.h $
// $Author: PATMAC $
// $Date: 1998/03/20 13:04:42 $
// $Revision: 1.27 $
//
// (c) 1996 Looking Glass Technologies Inc.
// Pat McElhatton (from JohnB)
//
// Module name: Looking Glass Sound
// File name: lgsound.h
//
// Description: External interface definition for sound library
//
////////////////////////////////////////////////////////////////////////

#ifndef _LG_SOUND_H
#define _LG_SOUND_H

#ifndef _INTTYPE_H
//#include <inttype.h>
#endif

#include <comtools.h>

#ifndef _LGSNDID_H
#include <lgsndid.h>
#endif

// forward declarations make life easier
// and reduce dependnecies, except in thiscase we arejust
// doing this cause these calss have a very insestuous relationship

F_DECLARE_INTERFACE(ISndMixer);
F_DECLARE_INTERFACE(ISndSample);

#ifdef __cplusplus
extern "C" {
#endif

// the single most important call here!!
// How to create the mixer in the first place
BOOL SndCreateMixer(ISndMixer **ppMixer, IUnknown *pOuter);
#define SndCreateDSMixer SndCreateMixer
BOOL SndCreateQSMixer(ISndMixer **ppMixer, IUnknown *pOuter);
BOOL SndCreateA3DMixer(ISndMixer **ppMixer, IUnknown *pOuter);

// the errors we want to return
typedef enum _eSndError {
	kSndUnknownError = -1,
	kSndOk,
	kSndCantInit,
	kSndInitAlready,
	kSndNoFreeChannels,
	kSndNoMemory,
	kSndInvalidData,
	kSndDeviceAlready,
	kSndCantCreateDevice,
	kSndCantSetupDevice,
   kSndBadGroup,
   kSndOnly8Bit,
	kUnknownFormat,
   kSnd3DInitFailure
} eSndError;

// major states of a sample
typedef enum {
	kSndStateCreated,          // sample not inited yet
	kSndStateInited,           // sample ready to play
	kSndStatePlaying,          // sample playing (may be paused and/or muted)
	kSndStateStopped,          // sample stopped
	kSndStateDestroyed
} eSndState;

// additional state flags
typedef enum {
   kSndFlagPaused       = 1,        // sample is paused
   kSndFlagMuted        = 2,        // sample is muted
   kSndFlagLooped       = 4,        // sample is looping
   kSndFlagStream       = 8,        // sample is a stream
   kSndFlagAudible      = 16,       // sample is audible (has a low-level channel)
   // the following flags are very short-term - generally are only set during a
   //  single callback
   kSndFlagFadeStop     = 256,      // sample has stopped due to a fade
   kSndFlagPreempted    = 512,      // sample has been preempted
   kSndFlagEndOfData    = 1024,     // sample is at end of data
   // sound flag bits 16 - 31 are reserved for internal use (see lgsndi.h)
   kSndFlagNone         = 0         // initial state, all flags off
} eSndStateFlags;

// a bit set of state flags
typedef uint32 eSndFlagSet;

// what to do when we're all done with a fade
typedef enum _eSndFadeFlags {
	kSndFadeNone,
	kSndFadeStop,
	kSndFadeReverse,
   kSndFadeCallback
} eSndFadeFlags;

// Here are the different types of sounds we can create
// Don't try to create an internal.
typedef enum _eSndSampleCreateFlags {
	kSndSampleInternal   = 1,        // this actually indicates the 'primary' sample
	kSndSampleNormal     = 2,        // "Normal" really means this is a one-shot
	kSndSampleStream     = 4,
	kSndSampleHardware   = 8
} eSndSampleCreateFlags;

typedef uint32 eSndSampleCreateFlagSet;

// The different formats we know about
typedef enum _eSndFormat {
	kSndVocFile,
	kSndWaveFile,
	kSndLgFile,
	kSndPcmFile,
	kSndNoFile        // for streams - can have any data format
} eSndFormat;

typedef enum _eSndFaders {
   kSndFadeVolume,
   kSndFadePan,
   kSndFadeFrequency,
   kSndNumFaders
} eSndFaders;

typedef enum _eSndSquelchMode {
   kSndSquelchStop,
   kSndSquelchMute
} eSndSquelchMode;

typedef enum _eSndDataType {
   kSndDataPCM,
   kSndDataIMAADPCM
} eSndDataType;

//
// significant events in life of a sample (& mixer)
//
typedef enum _eSndTraceEvent {
	kSndSampleStarted,
	kSndSampleStopped,
	kSndSamplePaused,
	kSndSampleResumed,
   kSndSampleDestroyed,
   kSndSampleMuted,
   kSndSampleCreated,
   kSndMixerDestroyed,
   kSndMixerUpdateStart,
   kSndMixerUpdateEnd,
   kSndThreadUpdateStart,
   kSndThreadUpdateEnd,
   kSndBufferAllocate,
   kSndBufferFree
} eSndTraceEvent;

//
// these are the ways of implementing 3d sound
//
typedef enum {
   kSnd3DMethodNone        = 0,
   kSnd3DMethodPanVol      = 1,
   kSnd3DMethodSoftware    = 2,
   kSnd3DMethodHardware    = 4
} eSnd3DMethod;

//
// these are the 3D modes that can be applied on a sample-by-sample basis
//
typedef enum {
   kSnd3DModeNone,
   kSnd3DModeNormal,
   kSnd3DModeHeadRelative
} eSnd3DMode;

//
// these are the attributes of a sound channel
//
typedef struct _sSndAttribs {
   eSndDataType   dataType;
   int32          sampleRate;
   int32          bitsPerSample;
   int32          nChannels;
   int32          bytesPerBlock;
   int32          samplesPerBlock;
   uint32         numSamples;
} sSndAttribs;

typedef struct _sSndSetup {
	void        *hwnd;
} sSndSetup, *pSndSetup;

typedef struct _sSndMixerCaps {
	int32 maxVolume;
	int32 minVolume;
	int32 panLeft;
	int32 panRight;
	int32 maxSampleRate;
   int32 minSampleRate;
} sSndMixerCaps;

// 3D postions, velocities & orientations
//  position units are meters
//  velocity units are a jealously gaurded MSoft secret (furlongs/fortnight?)
//  orientations are arbitrary units
typedef struct _sSndVector {
   float    x;
   float    y;
   float    z;
} sSndVector;

// Global 3D sound environment factors
// 
typedef struct _sSndEnvironment {
   float    dopplerFactor;
   float    distanceFactor;
   float    rolloffFactor;
} sSndEnvironment;

// volume limits in millibels
#define kSndMinVolume -10000
#define kSndMaxVolume -1

// pan limits in millibels
#define kSndPanLeft     -10000
#define kSndPanCenter   0
#define kSndPanRight    10000

// frequency limits in hertz
#define kSndMinFrequency 100
#define kSndMaxFrequency 100000

// number of sample groups
#define kSndNumGroups   16

// for DoForGroup 
#define kSndAnyGroup    (~0)

// to access master volume with group volume methods
#define kSndMasterGroup 65536

// multiplier for Doppler effects - 1.0 is default (real physics)
#define kSndMinDoppler 0.0
#define kSndMaxDoppler 10.0

// multiplier for distance rolloff effects - 1.0 is default (real physics)
#define kSndMinRolloff 0.0
#define kSndMaxRolloff 10.0

// allow apps to supply meaningful names for timelog samples
#ifdef NO_TIME_LOGGING
#define NAME_SAMPLE( pSmp, pName )
#define GET_SAMPLE_NAME
#else
#define NAME_SAMPLE( pSmp, pName ) ISndSample_SetName( pSmp, pName )
#define GET_SAMPLE_NAME  ((long) ISndSample_GetName(
#endif

typedef uint8* SndSampleBuffer;

typedef ISndSample * (*SndMixerCallback)(ISndMixer *, ISndSample *, void *);
typedef BOOL         (*SndPriorityCallback)(ISndMixer *, ISndSample *, void *);
typedef void			(*SndSampleCallback)(ISndSample *);
typedef void			(*SndEndCallback)(ISndSample *, void *);
typedef void			(*SndFreeChannelCallback)(ISndMixer *, void *);
typedef void         (*SndFillCallback)(ISndSample *, void *, uint32);
typedef void *       (*SndLoadFunction)(void *pCBData, void *pDst, uint32);
typedef void			(*SndTraceCallback)(ISndSample *, eSndTraceEvent);

#undef INTERFACE
#define INTERFACE ISndMixer

DECLARE_INTERFACE_(ISndMixer, IUnknown)
{
	// Comtools macro to declare IUnknown methods
	DECLARE_UNKNOWN_PURE();

	// call this bad boy to create a mixer
	STDMETHOD_(eSndError, Init)(THIS_ sSndSetup *setup, uint32 numChannels, sSndAttribs *attr) PURE;

	// how to create a new sample to be played.  flags determines what kind of sample it will be
	STDMETHOD_(ISndSample *, CreateSampleFromRez)(THIS_ eSndSampleCreateFlagSet flags, void *data, uint32 len) PURE;

	STDMETHOD_(ISndSample *, CreateRawSample)(THIS_ eSndSampleCreateFlagSet flags, void *dat, uint32 len,
                                             uint32 nsamps, sSndAttribs *attribs ) PURE;

	STDMETHOD_(uint32, 	FreeChannelCount)(THIS) PURE;

	// some callbacks.  This might get replaced with more elegant designed things
	STDMETHOD_(void,     RegisterPriorityCallback)(THIS_ SndPriorityCallback func, void *pCBData) PURE;

	// a general purpose interator for sound of a certain type
	STDMETHOD_(void, 		DoForAllSamples)(THIS_ SndMixerCallback func, void *pCBData) PURE;
	STDMETHOD_(void, 		DoForGroupSamples)(THIS_ uint32 group, SndMixerCallback func, void *pCBData) PURE;

	// this must get called as ofen as the app needs to have everything run smoothly...
	STDMETHOD_(void, 		Update)(THIS) PURE;

	STDMETHOD_(int32, 	SetMasterVolume)(THIS_  int32 vol) PURE;
	STDMETHOD_(int32, 	GetMasterVolume)(THIS) PURE;

	STDMETHOD_(int32, 	SetGroupVolume)(THIS_  uint32 group, int32 vol) PURE;
	STDMETHOD_(int32, 	GetGroupVolume)(THIS_ uint32 group) PURE;
	STDMETHOD_(void, 		FadeGroupVolume)(THIS_ uint32 group, int32 vol, int32 time, eSndFadeFlags flags) PURE;
   STDMETHOD_(void,     RegisterGroupFadeCallback)(THIS_ uint32 group, SndEndCallback cBack, void *p) PURE;

	STDMETHOD_(void,     StopAllSamples)(THIS) PURE;

	STDMETHOD_(ISndSample *, First)(THIS_ uint32 group, eSndFlagSet tflags, eSndFlagSet fflags) PURE;
	STDMETHOD_(ISndSample *, Next)(THIS) PURE;

	STDMETHOD_(void,     Pause)(THIS) PURE;
	STDMETHOD_(void,     Resume)(THIS) PURE;
   STDMETHOD_(uint32,   SetTimeout)(THIS_ uint32 mSecs ) PURE;
	STDMETHOD_(void,     RegisterFreeChannelCallback)(THIS_ SndFreeChannelCallback func, void *pCBData) PURE;
   STDMETHOD_(void,     RegisterTraceCallback)(THIS_ SndTraceCallback f) PURE;
   STDMETHOD_(void,	   GetAttribs)(THIS_ sSndAttribs *pAttribs) PURE;

   STDMETHOD_(void,     Set3DPosition)(THIS_ sSndVector *pPosition) PURE;
   STDMETHOD_(void,     Get3DPosition)(THIS_ sSndVector *pPosition) PURE;
   STDMETHOD_(void,     Set3DOrientation)(THIS_ sSndVector *pFront, sSndVector *pTop) PURE;
   STDMETHOD_(void,     Get3DOrientation)(THIS_ sSndVector *pFront, sSndVector *pTop) PURE;
   STDMETHOD_(void,     Set3DVelocity)(THIS_ sSndVector *pVel) PURE;
   STDMETHOD_(void,     Get3DVelocity)(THIS_ sSndVector *pVel) PURE;
   STDMETHOD_(void,     Set3DEnvironment)(THIS_ sSndEnvironment *pParams) PURE;
   STDMETHOD_(void,     Get3DEnvironment)(THIS_ sSndEnvironment *pParams) PURE;
   STDMETHOD_(void,     Set3DMethod)(THIS_ eSnd3DMethod method) PURE;
   STDMETHOD_(void,     Get3DMethodCapabilities)(THIS_ uint32 *pMethods) PURE;
   STDMETHOD_(int32,    Get3DDistanceVolume)(THIS_ float dist) PURE;
   STDMETHOD_(float,    Get3DDistanceFromVolume)(THIS_ int32 vol) PURE;
   STDMETHOD_(int32,    Get3DPositionVolume)(THIS_ sSndVector *pSrcPos) PURE;
   STDMETHOD_(void,     Get3DPositionPanVolume)(THIS_ sSndVector *pSrcPos, int32 *pPan, int32 *pVol) PURE;
   STDMETHOD_(void,     Set3DDeferMode)(THIS_ BOOL deferOn) PURE;
};

#define ISndMixer_Init(p, a, b, c)						COMCall3(p, Init, a, b, c)
#define ISndMixer_CreateSampleFromRez(p,a,b,c)     COMCall3(p, CreateSampleFromRez, a, b, c)
#define ISndMixer_CreateRawSample(p,a,b,c,d,e)		COMCall5(p, CreateRawSample, a,b,c,d,e)
#define ISndMixer_FreeChannelCount(p)					COMCall0(p, FreeChannelCount)
#define ISndMixer_RegisterPriorityCallback(p,a,b)  COMCall2(p, RegisterPriorityCallback, a, b)
#define ISndMixer_DoForAllSamples(p, a, b)			COMCall2(p, DoForAllSamples, a, b)
#define ISndMixer_DoForGroupSamples(p,a,b,c)       COMCall3(p, DoForGroupSamples, a, b, c)
#define ISndMixer_Update(p)								COMCall0(p, Update)
#define ISndMixer_SetMasterVolume(p, a)				COMCall1(p, SetMasterVolume, a)
#define ISndMixer_GetMasterVolume(p)					COMCall0(p, GetMasterVolume)
#define ISndMixer_SetGroupVolume(p, a, b)				COMCall2(p, SetGroupVolume, a, b)
#define ISndMixer_GetGroupVolume(p, a)					COMCall1(p, GetGroupVolume, a)
#define ISndMixer_FadeGroupVolume(p,a,b,c,d)       COMCall4(p, FadeGroupVolume,a,b,c,d)
#define ISndMixer_RegisterGroupFadeCallback(p,g,f,a) COMCall3(p, RegisterFadeCallback, g, f, a)
//#define ISndMixer_GroupFadeActive(p,a)             COMCall1(p, GroupFadeActive, a)
#define ISndMixer_StopAllSamples(p)						COMCall0(p, StopAllSamples)
#define ISndMixer_First(p,a)								COMCall1(p, First, a)
#define ISndMixer_Next(p,a)								COMCall1(p, Next, a)
#define ISndMixer_Pause(p)									COMCall0(p, Pause)
#define ISndMixer_Resume(p)								COMCall0(p, Resume)
#define ISndMixer_Release(p)                       COMCall0(p, Release)
#define ISndMixer_SetTimeout(p,a)						COMCall1(p, SetTimeout, a)
#define ISndMixer_RegisterFreeChannelCallback(p,a,b)  COMCall2(p, RegisterFreeChannelCallback, a, b)
#define ISndMixer_RegisterTraceCallback(p,a)       COMCall1(p, RegisterTraceCallback, a)
#define ISndMixer_GetAttribs(p,a)                  COMCall1(p, GetAttribs, a)

#define ISndMixer_Set3DPosition(p, a)              COMCall1(p, Set3DPosition, a)
#define ISndMixer_Get3DPosition(p, a)              COMCall1(p, Get3DPosition, a)
#define ISndMixer_Set3DOrientation(p, a, b)        COMCall2(p, Set3DOrientation, a, b)
#define ISndMixer_Get3DOrientation(p, a, b)        COMCall2(p, Get3DOrientation, a, b)
#define ISndMixer_Set3DVelocity(p, a)              COMCall1(p, Set3DVelocity, a)
#define ISndMixer_Get3DVelocity(p, a)              COMCall1(p, Get3DVelocity, a)
#define ISndMixer_Set3DEnvironment(p, a)           COMCall1(p, Set3DEnvironment, a)
#define ISndMixer_Get3DEnvironment(p, a)           COMCall1(p, Get3DEnvironment, a)
#define ISndMixer_Set3DMethod(p, a)                COMCall1(p, Set3DMethod, a)
#define ISndMixer_Get3DMethodCapabilities(p, a)    COMCall1(p, Get3DMethodCapabilities, a)
#define ISndMixer_Get3DDistanceVolume(p, a)        COMCall1(p, Get3DDistanceVolume, a)
#define ISndMixer_Get3DDistanceFromVolume(p, a)    COMCall1(p, Get3DDistanceFromVolume, a)
#define ISndMixer_Get3DPositionVolume(p, a)        COMCall1(p, Get3DPositionVolume, a)
#define ISndMixer_Get3DPositionPanVolume(p, a, b, c)     COMCall3(p, Get3DPositionPanVolume, a, b, c)
#define ISndMixer_Set3DDeferMode(p, a)				   COMCall1(p, Set3DDeferMode, a)

#undef INTERFACE
#define INTERFACE ISndSample

DECLARE_INTERFACE_(ISndSample, IUnknown)
{
	// Comtools macro to declare IUnknown methods
	DECLARE_UNKNOWN_PURE();

	STDMETHOD_(void, 	Play)(THIS) PURE;
	STDMETHOD_(void, 	Pause)(THIS) PURE;
	STDMETHOD_(void, 	Resume)(THIS) PURE;
	STDMETHOD_(void, 	Stop)(THIS) PURE;
	STDMETHOD_(void, 	Loop)(THIS) PURE;
	STDMETHOD_(void, 	Mute)(THIS) PURE;
	STDMETHOD_(void, 	UnMute)(THIS) PURE;
   STDMETHOD_(BOOL,  ResyncNeeded)(THIS_ uint32 *pStartPos) PURE;
	STDMETHOD_(void, 	Preempt)(THIS) PURE;

	STDMETHOD_(void, 	SetVolume)(THIS_ int32 vol) PURE;
	STDMETHOD_(void, 	SetPan)(THIS_ int32 pan) PURE;
	STDMETHOD_(void, 	SetFrequency)(THIS_ uint32 freq) PURE;
	STDMETHOD_(void, 	SetData)(THIS_ int32 data) PURE;
	STDMETHOD_(void, 	SetPriority)(THIS_ int32 pri) PURE;
   STDMETHOD_(void,  SetSuperInfo)(THIS_ void *pInfo) PURE;
	STDMETHOD_(void, 	SetPosition)(THIS_ uint32 pos) PURE;
	STDMETHOD_(void, 	SetGroup)(THIS_ uint32 group) PURE;

	STDMETHOD_(int32, 	GetVolume)(THIS) PURE;
	STDMETHOD_(int32, 	GetPan)(THIS) PURE;
	STDMETHOD_(uint32, 	GetFrequency)(THIS) PURE;
	STDMETHOD_(int32, 	GetData)(THIS) PURE;
	STDMETHOD_(int32, 	GetPriority)(THIS) PURE;
	STDMETHOD_(void *,   GetSuperInfo)(THIS) PURE;
	STDMETHOD_(uint32, 	GetPosition)(THIS) PURE;
   STDMETHOD_(void,	   GetPositions)(THIS_ uint32 *pW, uint32 *pP) PURE;
   STDMETHOD_(void,	   GetAttribs)(THIS_ sSndAttribs *pAttribs) PURE;
	STDMETHOD_(uint32,   GetGroup)(THIS) PURE;

   STDMETHOD_(uint32, 	GetSerialNumber)(THIS) PURE;

	STDMETHOD_(void, 		Fade)(THIS_ eSndFaders t, int32 vol, int32 time, eSndFadeFlags flags) PURE;
   STDMETHOD_(BOOL,     FadeActive)(THIS_ eSndFaders t) PURE;

   STDMETHOD_(uint32,   AvailToWrite)(THIS) PURE;
   STDMETHOD_(void,     CheckStream)(THIS) PURE;
	STDMETHOD_(void,     GetStatus)(THIS_ eSndState *pState, eSndFlagSet *pFlags) PURE;

	STDMETHOD_(BOOL,     BufferReady)(THIS_ uint32 len) PURE;
	STDMETHOD_(eSndError, LoadBuffer)(THIS_ uint8 * data, uint32 len) PURE;
	STDMETHOD_(eSndError, LoadBufferIndirect)(THIS_ SndLoadFunction funk, void *pFunkData, uint32 len) PURE;

   STDMETHOD_(void,	   SilenceFill)(THIS_ uint32 nBytes) PURE;
   STDMETHOD_(void,     RegisterFillCallback)(THIS_ SndFillCallback cBack, void *p) PURE;
   STDMETHOD_(void,     RegisterEndCallback)(THIS_ SndEndCallback cBack, void *p) PURE;
   STDMETHOD_(void,     RegisterPreemptCallback)(THIS_ SndEndCallback cBack, void *p) PURE;
   STDMETHOD_(void,     RegisterFadeCallback)(THIS_ eSndFaders t, SndEndCallback cBack, void *p) PURE;
   STDMETHOD_(void,     DumpData)(THIS_ BOOL turnOn ) PURE;  // start dumping data to raw file
   STDMETHOD_(uint32,   PlayTimeRemaining)(THIS) PURE;      // return milliseconds of playtime left
   STDMETHOD_(void,     SetName)(THIS_ char *pUserName) PURE;
   STDMETHOD_(char *,   GetName)(THIS) PURE;

   STDMETHOD_(void,     Squelch)(THIS_ int32 sqLevel, uint32 sqCount, eSndSquelchMode mode) PURE;

   STDMETHOD_(uint32,   TimeToSamples)(THIS_ uint32 millisecs) PURE;
   STDMETHOD_(uint32,   SamplesToTime)(THIS_ uint32 samples) PURE;

   STDMETHOD_(void,     Set3DPosition)(THIS_ sSndVector *pPosition) PURE;
   STDMETHOD_(void,     Get3DPosition)(THIS_ sSndVector *pPosition) PURE;
   STDMETHOD_(void,     Set3DVelocity)(THIS_ sSndVector *pVelocity) PURE;
   STDMETHOD_(void,     Get3DVelocity)(THIS_ sSndVector *pVelocity) PURE;
   STDMETHOD_(void,     Set3DConeAngles)(THIS_ uint32 inside, uint32 outside) PURE;
   STDMETHOD_(void,     Get3DConeAngles)(THIS_ uint32 *pInside, uint32 *pOutside) PURE;
   STDMETHOD_(void,     Set3DConeOrientation)(THIS_ sSndVector *pOrient) PURE;
   STDMETHOD_(void,     Get3DConeOrientation)(THIS_ sSndVector *pOrient) PURE;
   STDMETHOD_(void,     Set3DDistanceRange)(THIS_ float dmin, float dmax) PURE;
   STDMETHOD_(void,     Get3DDistanceRange)(THIS_ float *pMin, float *pMax) PURE;
   STDMETHOD_(void,     Set3DMode)(THIS_ eSnd3DMode mode) PURE;
   STDMETHOD_(void,     Get3DMode)(THIS_ eSnd3DMode *pMode) PURE;
   STDMETHOD_(void,     SetAmbientVolume)(THIS_ int32 vol) PURE;
   STDMETHOD_(int32,    GetAmbientVolume)(THIS) PURE;
   STDMETHOD_(void,     Set3DMethod)(THIS_ eSnd3DMethod method) PURE;
   STDMETHOD_(eSnd3DMethod, Get3DMethod)(THIS) PURE;
};

#define	ISndSample_Play(p) 						COMCall0(p, Play)
#define  ISndSample_Pause(p)                 COMCall0(p, Pause)
#define  ISndSample_Resume(p)                COMCall0(p, Resume)
#define  ISndSample_Stop(p)                  COMCall0(p, Stop)
#define  ISndSample_Loop(p)                  COMCall0(p, Loop)
#define  ISndSample_Mute(p)                  COMCall0(p, Mute)
#define  ISndSample_UnMute(p)                COMCall0(p, UnMute)
#define  ISndSample_ResyncNeeded(p,a)        COMCall1(p, ResyncNeeded, a)
#define  ISndSample_Preempt(p)               COMCall0(p, Preempt)

#define  ISndSample_SetVolume(p,a)           COMCall1(p, SetVolume,a)
#define  ISndSample_SetPan(p,a)              COMCall1(p, SetPan,a)
#define  ISndSample_SetFrequency(p,a)        COMCall1(p, SetFrequency,a)
#define  ISndSample_SetData(p,a)             COMCall1(p, SetData,a)
#define  ISndSample_SetPriority(p,a)         COMCall1(p, SetPriority,a)
#define  ISndSample_SetSuperInfo(p,a)        COMCall1(p, SetSuperInfo,a)
#define  ISndSample_SetType(p,a)             COMCall1(p, SetType,a)
#define  ISndSample_SetPosition(p,a)         COMCall1(p, SetPosition,a)
#define  ISndSample_SetGroup(p,a)            COMCall1(p, SetGroup,a)

#define  ISndSample_GetVolume(p)             COMCall0(p, GetVolume)
#define  ISndSample_GetPan(p)                COMCall0(p, GetPan)
#define  ISndSample_GetFrequency(p)          COMCall0(p, GetFrequency)
#define  ISndSample_GetData(p)               COMCall0(p, GetData)
#define  ISndSample_GetPriority(p)           COMCall0(p, GetPriority)
#define  ISndSample_GetSuperInfo(p)          COMCall0(p, GetSuperInfo)
#define  ISndSample_GetType(p)               COMCall0(p, GetType)
#define  ISndSample_GetPosition(p)           COMCall0(p, GetPosition)
#define  ISndSample_GetPositions(p,a,b)      COMCall2(p, GetPositions, a, b)
#define  ISndSample_GetAttribs(p,a)          COMCall1(p, GetAttribs, a)
#define  ISndSample_GetGroup(p)              COMCall0(p, GetGroup)

#define  ISndSample_GetSerialNumber(p)       COMCall0(p, GetSerialNumber)

#define  ISndSample_Fade(p,a,b,c,d)          COMCall4(p, Fade,a,b,c,d)
#define  ISndSample_FadeActive(p,a)          COMCall1(p, FadeActive, a)

#define  ISndSample_AvailToWrite(p)          COMCall0(p, AvailToWrite)
#define  ISndSample_CheckStream(p)           COMCall0(p, CheckStream)
#define  ISndSample_GetStatus(p,a,b)         COMCall0(p, GetStatus,a,b)

#define  ISndSample_BufferReady(p,a)         COMCall1(p, BufferReady,a)
#define  ISndSample_LoadBuffer(p,a,b)        COMCall2(p, LoadBuffer,a,b)
#define  ISndSample_LoadBufferIndirect(p,a,b,c) COMCall3(p, LoadBufferIndirect,a,b,c)

#define  ISndSample_SilenceFill(p, nBytes)   COMCall1(p, SilenceFill, nBytes)
#define  ISndSample_RegisterFillCallback(p,f,a)    COMCall2(p, RegisterFillCallback, f, a)
#define  ISndSample_RegisterEndCallback(p,f,a)     COMCall2(p, RegisterEndCallback, f, a)
#define  ISndSample_RegisterPreemptCallback(p,f,a) COMCall2(p, RegisterEndCallback, f, a)
#define  ISndSample_RegisterFadeCallback(p,g,f,a)  COMCall3(p, RegisterFadeCallback, g, f, a)

#define  ISndSample_Release(p)               COMCall0(p, Release)
#define  ISndSample_DumpData(p,a)            COMCall1(p, DumpData, a)
#define  ISndSample_PlayTimeRemaining(p)     COMCall0(p, PlayTimeRemaining)

#define  ISndSample_SetName(p,a)             COMCall1(p, SetName, a)
#define  ISndSample_GetName(p)               COMCall0(p, GetName)

#define  ISndSample_Squelch(p,a,b,c)         COMCall3(p, Squelch, a, b, c)

#define  ISndSample_TimeToSamples(p,a)       COMCall1(p, TimeToSamples, a)
#define  ISndSample_SamplesToTime(p,a)       COMCall1(p, SamplesToTime, a)

#define ISndSample_Set3DPosition(p, a)       COMCall1(p, Set3DPosition, a)
#define ISndSample_Get3DPosition(p, a)       COMCall1(p, Get3DPosition, a)
#define ISndSample_Set3DVelocity(p, a)       COMCall1(p, Set3DVelocity, a)
#define ISndSample_Get3DVelocity(p, a)       COMCall1(p, Get3DVelocity, a)
#define ISndSample_Set3DConeAngles(p, a, b)  COMCall2(p, Set3DConeAngles, a, b)
#define ISndSample_Get3DConeAngles(p, a, b)  COMCall2(p, Get3DConeAngles, a, b)
#define ISndSample_Set3DConeOrientation(p,a) COMCall1(p, Set3DConeOrientation, a)
#define ISndSample_Get3DConeOrientation(p,a) COMCall1(p, Get3DConeOrientation, a)
#define ISndSample_Set3DDistanceRange(p,a,b) COMCall2(p, Set3DDistanceRange, a, b)
#define ISndSample_Get3DDistanceRange(p,a,b) COMCall2(p, Get3DDistanceRange, a, b)
#define ISndSample_Set3DMode(p, a)           COMCall1(p, Set3DMode, a)
#define ISndSample_Get3DMode(p, a)           COMCall1(p, Get3DMode, a)
#define ISndSample_SetAmbientVolume(p, a)    COMCall1(p, Set3DAmbientVolume, a)
#define ISndSample_GetAmbientVolume(p, a)    COMCall0(p, Get3DAmbientVolume)
#define ISndSample_Set3DMethod(p, a)         COMCall1(p, Set3DMethod, a)
#define ISndSample_Get3DMethod(p)            COMCall0(p, Get3DMethod)


//
// get header info from WAVE or VOC sound file image
// return TRUE if failure occurs
//
extern BOOL
SndCrackRezHeader(
                  void          *pHdr,
                  uint32        len,
                  void          **ppData,
                  uint32        *pDataLen,
                  uint32        *pNumSamples,
                  sSndAttribs   *pAttribs );

#ifdef __cplusplus
	};
#endif

#endif //_LG_SOUND_H
