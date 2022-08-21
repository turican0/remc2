 
/*****************************************************************************
 *  BASS_VST
 *****************************************************************************
 *
 *  File:       bass_vst_impl.c
 *  Authors:    Bjoern Petersen
 *  Purpose:    All defines and headers needed for the BASS_VST implementation,
 *				this is an internal header!
 *
 *	Version History:
 *	22.04.2006	Created in this form (bp)
 *
 *  (C) Bjoern Petersen Software Design and Development
 *  
 *****************************************************************************/



#ifndef __BASS_VST_IMPL_H__
#define __BASS_VST_IMPL_H__



/*****************************************************************************
 *  Common Stuff and includes
 *****************************************************************************/


// windows includes
#include <windows.h>
#include <assert.h>
#include <crtdbg.h>
#include <stdio.h>
#include <math.h>
#define strcasecmp _stricmp /* strcasecmp() is ANSI, stricmp() not ... fuck Microsoft */
#define strncasecmp _strnicmp


// BASS includes
#define BASSDEF(f) (WINAPI f)	
#define BASSSCOPE
#include "bass.h"
#include "bass_addon.h"
#include "bassmidi.h"

// BASS VST includes
#include "bass_vst.h"

// VST DSK includes
#include "aeffectx.h"


// internal includes
#include "sjhash.h"
#include "bass_vst_version.h"

// in BASS 2.4 the user pointer are void*, in older versions, DWORD was used
#if BASSVERSION >= 0x204
#define USERPTR void*
#else
#define USERPTR DWORD
#endif


/*****************************************************************************
 *  Plugins
 *****************************************************************************/

typedef struct
{
	// are we an effect or an instrument?
	DWORD				type;
	#define				VSTeffect		0
	#define				VSTinstrument	1

	// the vstHandle as returned from BASS_VST_ChannelSetDSP() or BASS_VST_ChannelCreate().
	DWORD				vstHandle;

	// the underlying channel as given to BASS_VST_ChannelSetDSP() or created by BASS_VST_ChannelCreate().
	// for instruments created by the latter, this value is equal to vstHandle!
	DWORD				channelHandle;

	// effect-only stuff: dspHandle is the handle returned from BASS_ChannelSetDSP.  0 for
	// unchanneled effect and for VST instruments.
	HDSP				dspHandle;

	// the underlying DLL
	DWORD				createFlags;
	HINSTANCE			hinst;

	// the underlying VST object
	AEffect*			aeffect;
	#define				canDoubleReplacing(a) ( ((a)->aeffect->flags&effFlagsCanDoubleReplacing)!=0 && (a)->aeffect->processDoubleReplacing!=NULL )

	// handing parameters and programs
	int 				numDefaultValues;  // we cache this value as some plugins change aeffect->numParams :-(
	float*				defaultValues;     // only set at loading time caching the initial param values
	int 				numLastValues;
	float*				lastValues;
	float*				tempProgramValueBuf;
	char				tempProgramNameBuf[128]; // normally kVstMaxProgNameLen+1 (=24+1) should be enough, be a little safer
	char*				tempChunkData;

	// process handling
	#define				MAX_CHANS 32
	float*				buffersIn[MAX_CHANS];
	float*				buffersOut[MAX_CHANS];
	long				bytesPerInOutBuffer;

	float*				bufferTemp;
	long				bytesTempBuffer;

	bool				effOpenCalled;
	bool				effStartProcessCalled;

	long				effBlockSize;
	
	// bypass handling
	BOOL				doBypass;

	// idle stuff
	#define				NEEDS_EDIT_IDLE			0x01
	#define				NEEDS_IDLE_OUTSIDE_EDIT 0x02
	int					needsIdle;

	// editor stuff
	bool				editorIsOpen;
	DWORD				editorScope;

	// callbacks
	VSTPROC*			callback;
	void*				callbackUserData;

	// pending MIDI events, they're sended just before processReplacing is called
	#define				MAX_MIDI_EVENTS 2048
	VstEvents*			midiEventsCurr;
	VstEvents*			midiEventsPrev;
	CRITICAL_SECTION	midiCritical_;

	
	// static vstTimeInfo structre, "static" as the pointer may be needed "a little bit longer"
	VstTimeInfo			vstTimeInfo;

	// misc
	#define				MAX_FWD 128
	DWORD				forwardDataToOtherVstHandles[MAX_FWD];
	int					forwardDataToOtherCnt;

	CRITICAL_SECTION	vstCritical_;

	// do not use directly! always use the BASS_VST_LOCKER!
	long				handleUsage;

} BASS_VST_PLUGIN;



// handle stuff
void				initHandleHandling();
void				exitHandleHandling();

BASS_VST_PLUGIN*	createHandle(DWORD type, DWORD handle); // initalized the reference counting to 1; if handle is 0, a new handle is created
BASS_VST_PLUGIN*	refHandle(DWORD handle);
BOOL				unrefHandle(DWORD handle);	// if a handle has no more references, it is destroyed!

BOOL				tryEnterVstCritical(BASS_VST_PLUGIN*);
void				enterVstCritical(BASS_VST_PLUGIN*);
void				leaveVstCritical(BASS_VST_PLUGIN*);

// idle stuff
extern sjhash			s_idleHash;
extern CRITICAL_SECTION	s_idleCritical;
extern sjhash			s_unloadPendingInstances;
extern long				s_unloadPendingCountdown;
void					idleDo();
void					cleanUpPlugins(); //Falcosoft: in case of dynamic load cleanup loaded plugins when unload.
void					updateIdleTimers(BASS_VST_PLUGIN*); // call this if needsIdle has changed
void					createIdleTimers();
void					killIdleTimers();

#define					IDLE_FREQ 50 /*ms = 20Hz*/
#define					IDLE_UNLOAD_PENDING_COUNTDOWN (5000/*5 seconds*/ / IDLE_FREQ) //Falcosoft: some plugins require complete unload and 10 sec wait is too much...



// buffers
void					freeChansBuffers(BASS_VST_PLUGIN*);
void					freeTempBuffer(BASS_VST_PLUGIN*);

bool					openProcess(BASS_VST_PLUGIN*, BASS_VST_PLUGIN* info_);
bool					closeProcess(BASS_VST_PLUGIN*);
void CALLBACK			doEffectProcess(HDSP handle, DWORD channel, void* buffer, DWORD length, USERPTR user);
DWORD CALLBACK			doInstrumentProcess(HSTREAM vstHandle, void* buffer, DWORD length, USERPTR user);

int						validateLastValues(BASS_VST_PLUGIN*);


// forwarding
void					checkForwarding();
extern CRITICAL_SECTION	s_forwardCritical;

// misc
extern HMODULE			s_hDllKernel32;
void					callMainsChanged(BASS_VST_PLUGIN* this_, long blockSize);
long					fileSelOpen(BASS_VST_PLUGIN* this_, VstFileSelect* vstFs);
void					fileSelClose(BASS_VST_PLUGIN* this_, VstFileSelect* vstFs);


#endif /* __BASS_VST_IMPL_H__ */
