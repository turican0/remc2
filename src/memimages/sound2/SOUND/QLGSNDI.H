////////////////////////////////////////////////////////////////////////
// $Header: x:/prj/tech/libsrc/sound/RCS/qlgsndi.h 1.1 1998/03/20 12:52:48 PATMAC Exp $
//
// (c) 1997 Looking Glass Technologies Inc.
// Pat McElhatton
//
// Module name: Looking Glass Sound Library internal defs, QMixer version
// File name: qlgsndi.h
//
// Description: Internal interface definition for sound library
//
////////////////////////////////////////////////////////////////////////

#include <windows.h>

#ifndef _LG_SOUND_H
#include <lgsound.h>
#endif
#include <lgsndi.h>
#include <stdio.h>

#include <lgassert.h>
#include <thrdtool.h>

#include <timelog.h>
#include <qmixer.h>

//-------------------------------------------------------------------------------
// the cQSndDevice class abstracts the lower layer of sound subsystems that we use.
// for DOS this means AIL and for Win95 this means DirectSound.
//-------------------------------------------------------------------------------

class cQSndSample;

class cQSndMixer : public cSndMixer
{
	DECLARE_UNAGGREGATABLE();

public:
   cQSndMixer();
	virtual ~cQSndMixer();

	STDMETHOD_(eSndError,         Init)(sSndSetup *setup, uint32 numChannels,
                                       sSndAttribs *attr);

	STDMETHOD_(ISndSample *,		CreateRawSample)(eSndSampleCreateFlagSet flags, void *pd,
                                                  uint32 dlen, uint32 nsamps, sSndAttribs *attribs );

	STDMETHOD_(void,					Update)(void);

	STDMETHOD_(void,					Pause)(void);
	STDMETHOD_(void,					Resume)(void);

   STDMETHOD_(void,     Set3DPosition)(sSndVector *pPosition);
   STDMETHOD_(void,     Set3DOrientation)(sSndVector *pFront, sSndVector *pTop);
   STDMETHOD_(void,     Set3DVelocity)(sSndVector *pVel);
   STDMETHOD_(void,     Set3DEnvironment)(sSndEnvironment *pParams);
   STDMETHOD_(void,     Set3DMethod)(eSnd3DMethod method);
   STDMETHOD_(void,     Get3DMethodCapabilities)(uint32 *pMethods);

   HQMIXER GetQMixer( void );
   float GetDistanceFactor( void );

private:

   HQMIXER                 mpMixDevice;
   float                   mDistanceFactor;
};

inline HQMIXER
cQSndMixer::GetQMixer( void )
{
   return mpMixDevice;
}

inline float
cQSndMixer::GetDistanceFactor( void )
{
   return mDistanceFactor;
}


//-------------------------------------------------------------------------------
// Here is a sample. this contains all information to get a particular chunk
// of Sound data to play through the mixer.  What you do is request a free sample
// from the mixer.  If none are available it will call your prioirity function.
// If that can free something up it will.  Once you have a sample you can Initialize it
// to the type of sample you want and then do all the fun stuff, most notibly play it.
//-------------------------------------------------------------------------------


class cQSndSample : public cSndSample
{
	DECLARE_UNAGGREGATABLE();

public:

	cQSndSample(cQSndMixer *mixer, eSndSampleCreateFlagSet flags );
	virtual ~cQSndSample();

   STDMETHOD_(void,	   SetPan)(int32 pan);
   STDMETHOD_(void,	   SetFrequency)(uint32 freq);

   STDMETHOD_(uint32,   AvailToWrite)(void);
   STDMETHOD_(void,     CheckStream)(void);

   STDMETHOD_(BOOL,     BufferReady)(uint32 len);
   STDMETHOD_(eSndError,LoadBuffer)(uint8 *data, uint32 len);
   STDMETHOD_(eSndError,LoadBufferIndirect)(SndLoadFunction funk, void *pData, uint32 len);

   STDMETHOD_(void,	   SilenceFill)(uint32 nBytes);

   STDMETHOD_(void,     Set3DPosition)(sSndVector *pPosition);
   STDMETHOD_(void,     Set3DVelocity)(sSndVector *pVelocity);
   STDMETHOD_(void,     Set3DConeAngles)(uint32 inside, uint32 outside);
   STDMETHOD_(void,     Set3DConeOrientation)(sSndVector *pOrient);
   STDMETHOD_(void,     Set3DDistanceRange)(float dmin, float dmax);
   STDMETHOD_(void,     Set3DMode)(eSnd3DMode mode);
   STDMETHOD_(void,     SetAmbientVolume)(int32 vol);
   STDMETHOD_(void,     Set3DMethod)(eSnd3DMethod method);

   // Here is some of the nasty stuff to let things happen 
   // between cQSndMixer and cQSndSample
   virtual BOOL        IsPlaying(void);
   virtual BOOL        MakeAudible();
   virtual BOOL        InvokeRefillCB( void *pBuffer, uint32 nBytes );


protected:

   virtual void         LLStop();
   virtual void         LLRelease();
   virtual void         LLInit();
   virtual HRESULT      LLStart();
   virtual void         LLPause();
   virtual void         LLResume();
   virtual void         LLUnMute();
   virtual void         LLSetPosition( uint32 pos );
   virtual uint32       LLGetPosition();
   virtual void         LLSetVolume( int32 vol );

	virtual BOOL      LockBuffer(void **p1, uint32 *sz1, void **p2, uint32 *sz2, uint32 len);
	virtual void      UnlockBuffer(void *p1, uint32 sz1, void *p2, uint32 sz2);

   int            mChannel;
   HQMIXER        mpMixDevice;
   LPMIXWAVE      mpWave;
   void           *mpAudioData;

   void                 *mpRefillDest;

   float                mAmbientVolumeDB;    // volume outside the outer cone
   float                mRolloff;            // distance volume rolloff factor

	friend class cQSndMixer;
	friend class cQSndFadeTask;
};

