////////////////////////////////////////////////////////////////////////
// $Source: x:/prj/tech/libsrc/sound/RCS/lgsndi.h $
// $Author: PATMAC $
// $Date: 1998/03/20 13:05:16 $
// $Revision: 1.32 $
//
// (c) 1996 Looking Glass Technologies Inc.
// Pat McElhatton (from JohnB)
//
// Module name: Looking Glass Sound
// File name: lgsndi.h
//
// Description: Internal interface definition for sound library
//
////////////////////////////////////////////////////////////////////////

#include <dsound.h>

#ifndef _LG_SOUND_H
#include <lgsound.h>
#endif
#include <stdio.h>

#include <lgassert.h>
#include <thrdtool.h>

#include <timelog.h>

F_DECLARE_INTERFACE(IDisplayDevice);
F_DECLARE_INTERFACE(IWinDisplayDevice);

// sound flag bits 16 through 31 are for internal use
enum {
   kSndFlagPleaseStop   = (1<<16),  // sample should be stopped at next mixer update
   kSndFlagResyncNeeded = (1<<17),  // sample needs to be resynced
   kSndFlagOnList       = (1<<18),  // sample is on audible or inaudible list
   kSndFlagReallyPaused = (1<<19)   // sample has been paused since made audible
};


// squelch state machine states
typedef enum _eSndSquelchState {
   eSndSquelchOff,
   eSndSquelchOn,
   eSndSquelchCompleted
} eSndSquelchState;


// timer states
typedef enum _eSndTimerState {
   eSndTimerUnknown,
   eSndTimerTesting,
   eSndTimerWorking,
   eSndTimerBroken
} eSndTimerState;

// commands to stream update thread
enum {
   kThreadExit,
   kThreadUpdate,
   kThreadNewStream
};


class cSndSample;
class cSndGroupVolumeFadeTask;
class cMixerThread;

const int kFadeGranularity = 33;


class cSndMixer : public ISndMixer
{
	DECLARE_UNAGGREGATABLE();

public:
   cSndMixer();
	virtual ~cSndMixer();

	STDMETHOD_(eSndError,         Init)(sSndSetup *setup, uint32 numChannels,
                                       sSndAttribs *attr) = 0;

	STDMETHOD_(ISndSample *,		CreateSampleFromRez)(eSndSampleCreateFlagSet flags, void *dat, uint32 len );
	STDMETHOD_(ISndSample *,		CreateRawSample)(eSndSampleCreateFlagSet flags, void *pd,
                                                  uint32 dlen, uint32 nsamps, sSndAttribs *attribs ) = 0;

	STDMETHOD_(uint32,		  		FreeChannelCount)(void);

	STDMETHOD_(void,              RegisterPriorityCallback)(SndPriorityCallback f, void *pCBData);

	STDMETHOD_(void,					DoForAllSamples)(SndMixerCallback f, void *pCBData);
	STDMETHOD_(void,					DoForGroupSamples)(uint32 group, SndMixerCallback f, void *pCBData);
	STDMETHOD_(void,					Update)(void);

	STDMETHOD_(int32,					SetMasterVolume)(int32 vol);
	STDMETHOD_(int32,					GetMasterVolume)(void);

	STDMETHOD_(int32, 	         SetGroupVolume)(uint32 group, int32 vol);
	STDMETHOD_(int32, 	         GetGroupVolume)(uint32 group);
   STDMETHOD_(void,	            FadeGroupVolume)(uint32 group, int32 vol, int32 time, eSndFadeFlags flags);
   STDMETHOD_(void,              RegisterGroupFadeCallback)(uint32 group, SndEndCallback cBack, void *p);
   
	STDMETHOD_(void,					StopAllSamples)(void);

	STDMETHOD_(ISndSample *,      First)(uint32 group, eSndFlagSet tflags, eSndFlagSet fflags);
	STDMETHOD_(ISndSample *,      Next)(void);

	STDMETHOD_(void,					Pause)(void) = 0;
	STDMETHOD_(void,					Resume)(void) = 0;

	STDMETHOD_(uint32, 	         SetTimeout)(uint32 milliSecs);

   STDMETHOD_(void,              RegisterFreeChannelCallback)(SndFreeChannelCallback f, void *pCBData);
   STDMETHOD_(void,              RegisterTraceCallback)(SndTraceCallback f);
   STDMETHOD_(void,	            GetAttribs)(sSndAttribs *pAttribs);
   
   STDMETHOD_(void,     Set3DPosition)(sSndVector *pPosition) = 0;
   STDMETHOD_(void,     Get3DPosition)(sSndVector *pPosition);
   STDMETHOD_(void,     Set3DOrientation)(sSndVector *pFront, sSndVector *pTop) = 0;
   STDMETHOD_(void,     Get3DOrientation)(sSndVector *pFront, sSndVector *pTop);
   STDMETHOD_(void,     Set3DVelocity)(sSndVector *pVel) = 0;
   STDMETHOD_(void,     Get3DVelocity)(sSndVector *pVel);
   STDMETHOD_(void,     Set3DEnvironment)(sSndEnvironment *pParams) = 0;
   STDMETHOD_(void,     Get3DEnvironment)(sSndEnvironment *pParams);
   STDMETHOD_(void,     Set3DMethod)(eSnd3DMethod method) = 0;
   STDMETHOD_(void,     Get3DMethodCapabilities)(uint32 *pMethods) = 0;
   STDMETHOD_(int32,    Get3DDistanceVolume)(THIS_ float dist);
   STDMETHOD_(float,    Get3DDistanceFromVolume)(THIS_ int32 vol);
   STDMETHOD_(int32,    Get3DPositionVolume)(THIS_ sSndVector *pSrcPos);
   STDMETHOD_(void,     Get3DPositionPanVolume)(THIS_ sSndVector *pSrcPos, int32 *pPan, int32 *pVol);
   STDMETHOD_(void,     Set3DDeferMode)(THIS_ BOOL deferOn);

	// this is how the samples tell the mixer what they're up to
	void Inform(cSndSample *sample, eSndTraceEvent action);
	int32 NumberOfChannels();

   static void CALLBACK TimerCallback( UINT tid, UINT r1, DWORD duser, DWORD r2, DWORD r3 );
   void CheckTimer( void );
   void StartTimer( void );

   BOOL  AllocChannel( ISndSample *pSample );
   void  FreeChannel( void );

   // allow cSndSample to wank on our sample lists
   cSndSample ** AudibleHead( void );
   cSndSample ** InaudibleHead( void );

   cThreadMutex & MutexRef( void );
	BOOL CheckStreams(void);

   void DoFreeChannelCB(void);

   void DoTrace(void *pData,  eSndTraceEvent traceEvent );

   uint32 Get3DDeferFlag(void);

   int BlockDisplay();
   void ReleaseDisplay(int cookie);
   void TBD( char *message );

protected:
   virtual void Destroy( void );

	void CleanUpFinishedSamples(void);

protected:
   sSndAttribs mPrimaryAttribs;              // attribs of primary DirectSound obj

   int32 mGroupVolumes[kSndNumGroups+1];                    // volumes of sample groups
   int32 mMasterVolume;                                     // mixer master volume
   cSndGroupVolumeFadeTask *mpGroupFades[kSndNumGroups+1];  // group volume fade tasks
   // NOTE: the last extra element in mpGroupFades is the master group fade
   BOOL mGroupFadesActive;                                  // false IFF no group volume fades in progress

	static BOOL mTimerNeeded;                 // should timer be started/stopped?
	static uint32 mTimerId;
	static TIMECAPS mTimerCaps;

   // sample allocation stuff

	int32 mNumMixerChans;						      // total DirectSoundBuffers available
   int32 mNumFreeMixerChans;                  // number of free DirectSoundBuffers

   // the audible list holds samples which have a DirectSoundBuffer,
   // the inaudible list holds samples which don't.
	// all samples are in one of these two lists
	cSndSample  *mpAudibleListHead;
	cSndSample  *mpInaudibleListHead;

	cSndSample  *mpIterNext;
   uint32      mIterGroup;
   eSndFlagSet mIterTFlags;
   eSndFlagSet mIterFFlags;
   int         mIterWhichList;

//   cThreadLock    mLock;
   cThreadMutex   mMutex;
   cMixerThread   *mpThread;
   uint32         mTimeoutMillisecs;

	SndPriorityCallback     fnPriorityCB;
   void                    *mpPriorityCBData;
   SndFreeChannelCallback  fnFreeChannelCB;
   void                    *mpFreeChannelCBData;
   SndTraceCallback        fnTraceCB;

   eSndTimerState          mTimerState;
   uint32                  mTimerLastTick;

   IDisplayDevice          *mpDisplayDevice;
   IWinDisplayDevice       *mpWinDisplayDevice;

   sSndVector              m3DPosition;
   sSndVector              m3DFrontVector;
   sSndVector              m3DTopVector;
   sSndVector              m3DVelocity;
   sSndEnvironment         m3DEnvironment;
   eSnd3DMethod            m3DMixMethod;
   uint32                  m3DMethodCapabilities;
   uint32                  m3DDeferFlag;
};

inline int32 cSndMixer::NumberOfChannels()
{
	return mNumMixerChans;
}


// provide access to our audible list for cSndSample to change
inline cSndSample **
cSndMixer::AudibleHead()
{
   return &mpAudibleListHead;
}


// provide access to our inaudible list for cSndSample to change
inline cSndSample **
cSndMixer::InaudibleHead()
{
   return &mpInaudibleListHead;
}


// do trace events from outside mixer class
inline void
cSndMixer::DoTrace( void            *pSample,
                    eSndTraceEvent  traceEvent )
{
   if (fnTraceCB != NULL) {
      (*fnTraceCB) ( (ISndSample *) pSample, traceEvent );
   }
}


// provide access to mixer mutex
inline cThreadMutex &
cSndMixer::MutexRef()
{
   return mMutex;
}


class cMixerThread : public cWorkerThread
{
public:
   cMixerThread( cSndMixer *pMixer ) : mpMixer(pMixer), mTimeoutMillisecs( 200 )
   {};
   void        SetTimeout( uint32 ms ) {
      mTimeoutMillisecs = ms;
   };

private:
   virtual DWORD ThreadProc();
   cSndMixer      *mpMixer;
   uint32         mTimeoutMillisecs;
};


//-------------------------------------------------------------------------------
//
// An abstract base class for sample fades.  Each sample will have a fade
// task for each fad-able attribute that can be activated.
//
//-------------------------------------------------------------------------------

class cSndFadeTask
{
public:

   cSndFadeTask(cSndSample *s);
   virtual ~cSndFadeTask();

   void Fade(int32 dst, uint32 tm, eSndFadeFlags flags);
	BOOL Active();
   virtual BOOL DoFadeStep();
   void RegisterEndCallback(SndEndCallback cBack, void *pCBData);


protected:

   virtual void SetNextValue(int32 val) = 0;
   virtual int32 GetCurrentValue() = 0;
   virtual void FadeEndStop();
   virtual BOOL IsFadeable();

   uint32 mTimeLeft, mTime;
   int32 mDestination, mCurrent, mOriginal;
	eSndFadeFlags mFlags;
   cSndSample *subject;
   SndEndCallback mEndCallback;
   void *mpEndCBData;
};


class cSndVolumeFadeTask : public cSndFadeTask
{
public:
   cSndVolumeFadeTask(cSndSample *s) : cSndFadeTask(s) {}
protected:
   void SetNextValue(int32 val);
   int32 GetCurrentValue();

};

class cSndPanFadeTask : public cSndFadeTask
{
public:
   cSndPanFadeTask(cSndSample *s) : cSndFadeTask(s) {}
protected:
   void SetNextValue(int32 val);
   int32 GetCurrentValue();

};

class cSndFrequencyFadeTask : public cSndFadeTask
{
public:
   cSndFrequencyFadeTask(cSndSample *s) : cSndFadeTask(s) {}
protected:
   void SetNextValue(int32 val);
   int32 GetCurrentValue();
};

class cSndGroupVolumeFadeTask : public cSndFadeTask
{
public:
   cSndGroupVolumeFadeTask( uint32 group, cSndMixer *pMixer );
protected:
   void SetNextValue( int32 val );
   int32 GetCurrentValue();
   void FadeEndStop();
   BOOL IsFadeable();

private:
   uint32 mGroup;
   cSndMixer *mpMixer;
};

//-------------------------------------------------------------------------------
// Here is a sample. this contains all information to get a particular chunk
// of Sound data to play through the mixer.  What you do is request a free sample
// from the mixer.  If none are available it will call your prioirity function.
// If that can free something up it will.  Once you have a sample you can Initialize it
// to the type of sample you want and then do all the fun stuff, most notibly play it.
//-------------------------------------------------------------------------------

class cSndSample : public ISndSample
{
	DECLARE_UNAGGREGATABLE();

public:

	cSndSample(cSndMixer *mixer, eSndSampleCreateFlagSet flags );

	virtual ~cSndSample();

	BOOL	Init(sSndAttribs *attribs, void *buf, uint32 len, uint32 nsamps);

	STDMETHOD_(void,     Play)(void);
   STDMETHOD_(void,     Pause)(void);
   STDMETHOD_(void,     Resume)(void);
   STDMETHOD_(void,     Stop)(void);
   STDMETHOD_(void, 		Loop)(void);
   STDMETHOD_(void, 		Mute)(void);
   STDMETHOD_(void, 		UnMute)(void);
   STDMETHOD_(BOOL,     ResyncNeeded)(uint32 *pStartPos);
	STDMETHOD_(void, 	   Preempt)(void);

   STDMETHOD_(void,	   SetVolume)(int32 vol);
   STDMETHOD_(void,	   SetPan)(int32 pan) = 0;
   STDMETHOD_(void,	   SetFrequency)(uint32 freq) = 0;
   STDMETHOD_(void,	   SetData)(int32 data);
   STDMETHOD_(void,	   SetPriority)(int32 data);
   STDMETHOD_(void,     SetSuperInfo)(void *);
   STDMETHOD_(void,	   SetPosition)(uint32 pos);
   STDMETHOD_(void,	   SetGroupVolume)(int32 vol);
	STDMETHOD_(void, 	   SetGroup)(uint32 group);

   STDMETHOD_(int32,	   GetVolume)(void);
   STDMETHOD_(int32,	   GetPan)(void);
   STDMETHOD_(uint32,	GetFrequency)(void);
   STDMETHOD_(int32,	   GetData)(void);
   STDMETHOD_(int32,	   GetPriority)(void);
   STDMETHOD_(void *,	GetSuperInfo)(void);
   STDMETHOD_(uint32,	GetPosition)(void);
   STDMETHOD_(void,	   GetPositions)(uint32 *pW, uint32 *pP);
   STDMETHOD_(void,	   GetAttribs)(sSndAttribs *pAttribs);
   STDMETHOD_(uint32,   GetGroup)(void);

   STDMETHOD_(uint32,	GetSerialNumber)(void);

   STDMETHOD_(void,	   Fade)(eSndFaders t, int32 vol, int32 time, eSndFadeFlags flags);
   STDMETHOD_(BOOL,     FadeActive)(eSndFaders t);

   STDMETHOD_(uint32,   AvailToWrite)(void) = 0;
   STDMETHOD_(void,     CheckStream)(void) = 0;
   STDMETHOD_(void,     GetStatus)(eSndState *pState, uint32 *pFlags);

   STDMETHOD_(BOOL,     BufferReady)(uint32 len) = 0;
   STDMETHOD_(eSndError,LoadBuffer)(uint8 *data, uint32 len);
   STDMETHOD_(eSndError,LoadBufferIndirect)(SndLoadFunction funk, void *pData, uint32 len) = 0;

   STDMETHOD_(void,	   SilenceFill)(uint32 nBytes) = 0;
   STDMETHOD_(void,     RegisterFillCallback)(SndFillCallback cBack, void *p);
   STDMETHOD_(void,     RegisterEndCallback)(SndEndCallback cBack, void *p);
   STDMETHOD_(void,     RegisterPreemptCallback)(SndEndCallback cBack, void *p);
   STDMETHOD_(void,     RegisterFadeCallback)(eSndFaders t, SndEndCallback cBack, void *p);
   STDMETHOD_(void,     DumpData)( BOOL turnOn );  // start dumping data to raw file
   STDMETHOD_(uint32,   PlayTimeRemaining)(void);     // return milliseconds of playtime left
   STDMETHOD_(void,     SetName)(char *pUserName);
   STDMETHOD_(char *,   GetName)(void);
   STDMETHOD_(void,     Squelch)(int32 sqLevel, uint32 sqCount, eSndSquelchMode mode);
   STDMETHOD_(uint32,   TimeToSamples)(uint32 millisecs);
   STDMETHOD_(uint32,   SamplesToTime)(uint32 samples);


   STDMETHOD_(void,     Set3DPosition)(sSndVector *pPosition) = 0;
   STDMETHOD_(void,     Get3DPosition)(sSndVector *pPosition);
   STDMETHOD_(void,     Set3DVelocity)(sSndVector *pVelocity) = 0;
   STDMETHOD_(void,     Get3DVelocity)(sSndVector *pVelocity);
   STDMETHOD_(void,     Set3DConeAngles)(uint32 inside, uint32 outside) = 0;
   STDMETHOD_(void,     Get3DConeAngles)(uint32 *pInside, uint32 *pOutside);
   STDMETHOD_(void,     Set3DConeOrientation)(sSndVector *pOrient) = 0;
   STDMETHOD_(void,     Get3DConeOrientation)(sSndVector *pOrient);
   STDMETHOD_(void,     Set3DDistanceRange)(float dmin, float dmax) = 0;
   STDMETHOD_(void,     Get3DDistanceRange)(float *pMin, float *pMax);
   STDMETHOD_(void,     Set3DMode)(eSnd3DMode mode) = 0;
   STDMETHOD_(void,     Get3DMode)(eSnd3DMode *pMode);
   STDMETHOD_(void,     SetAmbientVolume)(int32 vol) = 0;
   STDMETHOD_(int32,    GetAmbientVolume)(void);
   STDMETHOD_(void,     Set3DMethod)(THIS_ eSnd3DMethod method) = 0;
   STDMETHOD_(eSnd3DMethod, Get3DMethod)(THIS);

   // Here is some of the nasty stuff to let things happen 
   // between cSndMixer and cSndSample
	BOOL        IsStream(void);
   void        DeferredStop(void);
   uint32      GetLogID( void );
   cSndSample  *Next();

   eSndState   GetState();
   eSndFlagSet GetFlags();
   eSndFlagSet SetFlags( eSndFlagSet );
   eSndFlagSet ClearFlags( eSndFlagSet );

   virtual BOOL         IsPlaying(void) = 0;
   virtual BOOL         MakeAudible() = 0;
   virtual void         MakeInaudible();

   void        MoveToList( cSndSample **ppHead );
   void        RemoveFromList();
   uint32      GetGroupQuick( void );
   void        TBD( char *message );

protected:
	virtual HRESULT      Start();
   virtual void         LLStop() = 0;
   virtual void         LLRelease() = 0;
   virtual void         LLInit()  = 0;
   virtual HRESULT      LLStart() = 0;
   virtual void         LLPause() = 0;
   virtual void         LLResume() = 0;
   virtual void         LLUnMute() = 0;
   virtual void         LLSetPosition( uint32 pos ) = 0;
   virtual uint32       LLGetPosition() = 0;
   virtual void         LLSetVolume( int32 vol ) = 0;

   cSndFadeTask      *GetFader( eSndFaders whichFader);


private:
protected:

	cSndMixer 	   *mpMixer;
   static uint32 mSamplesMade;
   static uint32 mSamplesDestroyed;

	eSndSampleCreateFlagSet mFlags;

	int32 mVolume;			// we keep this to provide master levels
	int32 mPan;
	int32 mFrequency;
	int32 mAppData;
   void  *mpSuperInfo;

	cSndFadeTask *mFadeTasks[3];  // should be same as number of eSndFadeTasks

	uint32					mLastWrite;
	uint32               mLastRead;
   SndFillCallback      fnFillCB;
   void                 *mpFillCBData;
   uint32               mFinalWrite;
   void                 *mpBuffer;
   uint32               mBufferLen;
   SndEndCallback       fnEndCB;
   void                 *mpEndCBData;
   SndEndCallback       fnPreemptCB;
   void                 *mpPreemptCBData;
   uint32               mSerialNum;
   static uint32        mNextSerialNum;
   sSndAttribs          mAttribs;
   uint32               mNumSamples;
   uint32               mBytesPerSample;
   eSndState            mState;
   eSndFlagSet          mStateFlags;
   char                 *mpNextWrite;
   char                 *mpWriteBase;

   FILE                 *mDumpFile;       // for debugging - write out data to this file
   uint32               mGroup;
   int32                mGroupVolume;

   uint32               mBaseOffset;      // position readback offset

   DWORD                mBaseTime;        // base for muted position estimates
   uint32               mBasePos;

   cSndSample           *mpNext;          // doubley linked list ptrs
   cSndSample           *mpPrev;

   static uint32        mNumAudible;      // #samples in audible list
   static uint32        mNumInaudible;    // #samples in inaudible list
   int32                mInitBuffPos;     // initial DSound buffer position
   int32                mPriority;        // app specified priority level
   char                 *mpName;          // sample name ptr

   eSndSquelchState     mSquelchState;    // are we squelching?
   int32                mSquelchLevel;    // squelch level
   uint32               mSquelchCount;    // current squelch count
   uint32               mSquelchLimit;    // count to complete squelch

   sSndVector           m3DPosition;         // world position
   sSndVector           m3DVelocity;         // velocity
   uint32               m3DConeInnerAngle;   // max volume inside this cone
   uint32               m3DConeOuterAngle;   // min volume outside this cone
   sSndVector           m3DConeOrientation;  // orientation of 3D cone
   float                m3DMinDistance;      // sound cannot get louder inside this distance
   float                m3DMaxDistance;      // sound cannot get softer outside this distance
   eSnd3DMode           m3DMode;             // not-3D, head relative or normal 3D
   int32                mAmbientVolume;      // volume outside the outer cone
   eSnd3DMethod         m3DMethod;           // none, panvol, software, hardware

	friend class cSndMixer;
	friend class cSndFadeTask;
};

inline BOOL cSndSample::IsStream(void)
{
	return (mStateFlags & kSndFlagStream) != 0;
}

inline uint32
cSndSample::GetLogID()
{
   return (mGroup << 16) + mSerialNum;
}

inline eSndState
cSndSample::GetState()
{
   return mState;
}


inline eSndFlagSet
cSndSample::GetFlags()
{
   return mStateFlags;
}


inline eSndFlagSet
cSndSample::SetFlags( eSndFlagSet newFlags )
{
   eSndFlagSet oldFlags = mStateFlags;

   mStateFlags |= newFlags; 
   return oldFlags;
}


inline eSndFlagSet
cSndSample::ClearFlags( eSndFlagSet newFlags )
{
   eSndFlagSet oldFlags = mStateFlags;

   mStateFlags &= (~newFlags); 
   return oldFlags;
}


inline cSndSample *
cSndSample::Next()
{
   return mpNext;
}


inline uint32
cSndSample::GetGroupQuick()
{
   return mGroup;
}


//
// debug aid
//
void
TBD( char *message);

