 
/*****************************************************************************
 *  BASS_VST
 *****************************************************************************
 *
 *  File:       bass_vst_impl.cpp
 *  Authors:    Bjoern Petersen
 *  Purpose:    Loading VST plugins
 *
 *	Version History:
 *	22.04.2006	Created in this form (bp)
 *
 *  (C) Bjoern Petersen Software Design and Development
 *
 *****************************************************************************
 *
 *	Plugin Sites:
 *
 *	http://userpage.fu-berlin.de/~thman/start.html
 *  http://www.kvraudio.com
 *  http://www.vst4free.com
 *
 *****************************************************************************
 *
 *  Documentation Sites:
 *
 *	http://www.u-he.com/vstsource/newbie.php?id=4
 *	http://ygrabit.steinberg.de/
 *	http://ygrabit.steinberg.de/~ygrabit/public_html/vstgui/vstsupport.html
 *
 *	VST3:
 *
 *	http://www.heise.de/newsticker/meldung/101994 
 *	http://www.steinberg.de/1504+M54a708de802.html
 *
 *	MIDI RPN (Registered Parametaer Number):
 *
 *	http://206.53.239.180/archives/directmusic/11-2002/msg00015.html
 *  http://www.midiguide.de/nrpnrpn.html
 *
 *****************************************************************************/



#include "bass_vst_impl.h"




/*****************************************************************************
 *  Main()
 *****************************************************************************/


// s_bassfunc is a pointer to the BASS plugin API as defined in api.h
#ifdef _WIN32
	static BASS_FUNCTIONS *s_bassfunc = NULL;
#else
	#ifdef __cplusplus
	  extern "C"
	#endif
	void *_();
	static BASS_FUNCTIONS* s_bassfunc=(BASS_FUNCTIONS*)_;
#endif



// flag to indicate if mainInit was successful or not
static bool s_mainOk = false;



// global initialization
static void mainInit()
{
	// version check:
	// BASS <=2.2 returns the version as 0xjjjjnnnn, newer version numbers come as 0xjjnnrrrr
	// (jj=major, nn=minor, rrrr=revision; beta versions have the revision 0xFFFF
	if( !(s_bassfunc=(BASS_FUNCTIONS*)GetProcAddress(GetModuleHandle("bass"),"_"))
	 || (BASS_GetVersion()!=MAKELONG(2,2) && HIWORD(BASS_GetVersion())!=0x0203 && HIWORD(BASS_GetVersion())!=0x0204) )
	{
		MessageBox(0,"Incorrect BASS.DLL version (2.2 - 2.4 is required)", "BASS_VST" BASS_VST_VERSION_STR,MB_ICONERROR);
		return;
	}

	initHandleHandling();

	InitializeCriticalSection(&s_idleCritical);
	sjhashInit(&s_idleHash, SJHASH_INT, /*keytype*/ 0/*copyKey*/);
	sjhashInit(&s_unloadPendingInstances, SJHASH_INT, /*keytype*/ 0/*copyKey*/);

	s_mainOk = true;
}




// global exit
static void mainExit()
{
	s_mainOk = false;
	s_bassfunc = NULL;

	cleanUpPlugins();

	killIdleTimers();

	exitHandleHandling();			
	
	DeleteCriticalSection(&s_idleCritical);
	sjhashClear(&s_idleHash);
	sjhashClear(&s_unloadPendingInstances);
	
	if( s_hDllKernel32 )
		FreeLibrary(s_hDllKernel32);
}



// window-specific DLL entry point, this calls mainInit() and mainExit()
#ifdef _WIN32
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	// From the LoadLibrary documentation in MSDN:
	// It is not safe to call LoadLibraryEx from DllMain. For more information, see the Remarks section in DllMain. 

    switch( ul_reason_for_call )
	{
		case DLL_PROCESS_ATTACH:
			mainInit();
			if( !s_mainOk )
				return FALSE;
			break;

		case DLL_PROCESS_DETACH:
			mainExit();
			break;

		case DLL_THREAD_ATTACH:
			break;

		case DLL_THREAD_DETACH:
			break;
    }

    return TRUE;
}
#endif


/*****************************************************************************
 *  Some Tools
 *****************************************************************************/



// error/success returns
#define SET_ERROR( errorCode )		{ if( s_bassfunc ) { s_bassfunc->SetError(errorCode); } }
#define RETURN_ERROR( errorCode )	{ if( s_bassfunc ) { s_bassfunc->SetError(errorCode); } return 0; }
#define RETURN_SUCCESS( value )		{ if( s_bassfunc ) { s_bassfunc->SetError(BASS_OK); } return value; }



// type of the plugin's main entry function
typedef AEffect *(*dllMainEntryFuncType) (audioMasterCallback);

// s_inConstructionVstHandle is a little hack as this_ is not yet valid
// when audioMasterCurrentId is called
static DWORD s_inConstructionVstHandle = 0;
static long s_language = kVstLangEnglish;



// just find out the sample rate of the channel
static long getSampleRate(BASS_VST_PLUGIN* this_)
{
	long sampleRate = 44100;
	if( this_ && this_->channelHandle )
	{
		BASS_CHANNELINFO info;
		if( BASS_ChannelGetInfo(this_->channelHandle, &info) )
			sampleRate = info.freq;
	}
	return sampleRate;
}



static void calcVstTimeInfo(BASS_VST_PLUGIN* this_, VstIntPtr toCalc)
{
	this_->vstTimeInfo.flags = kVstTransportPlaying;
	this_->vstTimeInfo.sampleRate = getSampleRate(this_);

	if( toCalc & kVstNanosValid )
	{
		this_->vstTimeInfo.nanoSeconds = (double)timeGetTime() * 1000000.0L;
		this_->vstTimeInfo.flags |= kVstNanosValid;
	}
	
	if( toCalc & kVstTimeSigValid )
	{
		this_->vstTimeInfo.timeSigNumerator   = 4;
		this_->vstTimeInfo.timeSigDenominator = 4;
		this_->vstTimeInfo.flags |= kVstTimeSigValid;
	}

	if( toCalc & (kVstTempoValid|kVstPpqPosValid) )
	{
		this_->vstTimeInfo.tempo = 120;
		this_->vstTimeInfo.flags |= kVstTempoValid;
	}

	if( toCalc & kVstPpqPosValid )
	{
		double dPos = this_->vstTimeInfo.samplePos / this_->vstTimeInfo.sampleRate;
		this_->vstTimeInfo.ppqPos = dPos * this_->vstTimeInfo.tempo / 60.L;
		this_->vstTimeInfo.flags |= kVstPpqPosValid;
	}
	
	if( toCalc & kVstSmpteValid )
	{
		static const double fSmpteDiv[] = {24.f,25.f,24.f,30.f,29.97f,30.f};
		this_->vstTimeInfo.smpteFrameRate = kVstSmpte24fps;

		double dPos = this_->vstTimeInfo.samplePos / this_->vstTimeInfo.sampleRate;
		double dOffsetInSecond = dPos - floor(dPos);
		this_->vstTimeInfo.smpteOffset = (long)(dOffsetInSecond * fSmpteDiv[this_->vstTimeInfo.smpteFrameRate] * 80.L);
		this_->vstTimeInfo.flags |= kVstSmpteValid;
	}

	if( toCalc & kVstCyclePosValid )
	{
		this_->vstTimeInfo.cycleStartPos = 0;
		this_->vstTimeInfo.cycleEndPos = 0;
		this_->vstTimeInfo.flags |= kVstCyclePosValid;
	}
}



/*****************************************************************************
 *  Functions Plugin -> Silverjuke
 *****************************************************************************/



static VstIntPtr audioMasterCallbackImpl(AEffect* aeffect_, // on load, aeffect_ this may be NULL or incomplete, use this_->aeffeet insteed
										 VstInt32 opcode, VstInt32 index,
						                 VstIntPtr value, void *ptr, float opt)
{
	VstIntPtr ret = 0;

	DWORD vstHandle = (aeffect_ && aeffect_->resvd1)?		// litte bug fix for 2.4.0.2: we also check for aeffect_->resvd1 now, so we will _always_ get a handle as 
			(DWORD)aeffect_->resvd1 : s_inConstructionVstHandle;	// s_inConstructionVstHandle is always valid until aeffect_->resvd1 is set.
	BASS_VST_PLUGIN* this_ = refHandle(vstHandle);
	if( this_ == NULL )
		return 0;

	// check if processing is done by the user
	////////////////////////////////////////////////////////////
	
	if( this_->callback )
	{
		VSTPROC*		callback = this_->callback;
		void*			callbackUserData = this_->callbackUserData;
		BASS_VST_AUDIO_MASTER_PARAM amp;
		amp.aeffect		= this_->aeffect;
		amp.opcode		= opcode;
		amp.index		= index;
		amp.value		= value;
		amp.ptr			= ptr;
		amp.opt			= opt;
		amp.doDefault	= 1;
		unrefHandle(vstHandle);
			ret = callback(vstHandle, BASS_VST_AUDIO_MASTER, (DWORD)&amp, 0, callbackUserData);
			if( amp.doDefault == 0 )
				return ret;
			ret = 0;
		this_ = refHandle(vstHandle); // reallocate the handle
		if( this_ == NULL )
			return 0;
	}


	switch( opcode )
	{
		// VST 1.00 opcodes
		////////////////////////////////////////////////////////////

		case audioMasterAutomate:				// Notifies us a about a parameter change in the editor --
			break;								// however, we do not rely on this as some plugins do not send this message.
												// Instead, we poll for parameter changes in our idle routine.

		case audioMasterVersion:				// VST Version supported (for example 2200 for VST 2.2) --
			ret = kVstVersion;					// 2 for VST 2.00, 2100 for VST 2.1, 2200 for VST 2.2 etc.
			break;								// We use 2.4 although we do not support all features
												// as some plugins relies on this without checking :-(
												// (eg. TripleComp crashes if the version is set to 2)

		case audioMasterCurrentId:				// Returns the unique id of a plug that's currently loading
			ret = vstHandle;
			break;

		case audioMasterIdle:					// Call application idle routine (this will
			idleDo();							// call effEditIdle for all open editors too) 
			break;

		case audioMasterPinConnected:			
			ret = index >= 2? 1 : 0;			// Inquire if an input or output is beeing connected;
			if( this_->channelHandle )			// index enumerates input or output counting from zero,
			{									// value is 0 for input and != 0 otherwise. note: the
				BASS_CHANNELINFO info;			// return value is 0 for <true> such that older versions will always return true.	
				if( BASS_ChannelGetInfo(this_->channelHandle, &info) )
				{
					ret = index >= (long)info.chans? 1 : 0;
				}
			}
			break;

		// VST 2.00 opcodes
		////////////////////////////////////////////////////////////

		case audioMasterUpdateDisplay: // the plug-in reported an update (e.g. after a program load/rename or any other param change)
			if (this_->effStartProcessCalled)
			{
				enterVstCritical(this_);
					int oldParamCount = this_->numLastValues;
					int newParamCount = validateLastValues(this_);
				leaveVstCritical(this_);
				if( this_->callback )
					this_->callback(this_->vstHandle, BASS_VST_PARAM_CHANGED, oldParamCount, newParamCount, this_->callbackUserData);
			}
			break;

		case audioMasterGetTime:
			calcVstTimeInfo(this_, value);
			ret =(VstIntPtr)&this_->vstTimeInfo;// the structure needs to be valid "a moment"; it is overwritten on the next call to audioMasterGetTime
			break;
		
		case __audioMasterNeedIdleDeprecated:	// plug needs idle calls (outside its editor window) (although this is deprecated it is heavily used by VST 2.3 and sooner)
			this_->needsIdle |= NEEDS_IDLE_OUTSIDE_EDIT;
			updateIdleTimers(this_);
			ret = 1;
			break;
			
		case audioMasterSizeWindow:				// index: width, value: height
			if( this_->callback )
			{
				this_->callback(vstHandle, BASS_VST_EDITOR_RESIZED, index, (DWORD)value, this_->callbackUserData);
			}
			break;

		case audioMasterGetSampleRate:
			ret = getSampleRate(this_);
			break;

		case audioMasterGetVendorString:		// fills <ptr> with a string identifying the vendor (max 64 char)
			strcpy((char*)ptr, "Bjoern Petersen Software Design and Development"/*max 64 char!*/);
			ret = true;
			break;

		case audioMasterGetProductString:		// fills <ptr> with a string with product name (max 64 char)
			strcpy((char*)ptr, "BASS_VST @ Silverjuke.Net");
			ret = true;
			break;

		case audioMasterGetVendorVersion:		// returns vendor-specific version
			ret = BASS_VST_VERSION_HEX;
			break;

		case audioMasterCanDo:					// string in ptr, see below
			if( strcasecmp((char*)ptr, "supplyidle")==0
			 || strcasecmp((char*)ptr, "sendvstevents")==0		// we can (and will!) send events to the plugin ...
			 || strcasecmp((char*)ptr, "sendvstmidievent")==0	// ... esp. MIDI event for VSTi
			 || strcasecmp((char*)ptr, "sendvsttimeinfo")==0	// on request, we can send timing information to the plugin (see this_->vstTimeInfo)
			 || strcasecmp((char*)ptr, "openfileselector")==0	// we support audioMasterOpenFileSelector?
			 || strcasecmp((char*)ptr, "closefileselector")==0	// we support audioMasterCloseFileSelector?
			 || strcasecmp((char*)ptr, "sizewindow")==0
			 || strcasecmp((char*)ptr, "startstopprocess")==0 )	// we calls effStartProcess  and effStopProcess
			{
				ret = 1;
			}
			break;

		case audioMasterGetLanguage:			// see enum
			ret = s_language;
			break;

		// VST 2.10 opcodes
		////////////////////////////////////////////////////////////


		case audioMasterOpenFileSelector:		// open a fileselector window with VstFileSelect* in <ptr>
			ret = fileSelOpen(this_, (VstFileSelect*)ptr);
			break;

		// VST 2.20 opcodes
		////////////////////////////////////////////////////////////

		case audioMasterCloseFileSelector:
			fileSelClose(this_, (VstFileSelect*)ptr);
			break;

		// Misc / Debug only
		////////////////////////////////////////////////////////////

		#if defined(_DEBUG) && defined(_WIN32)
		case __audioMasterWantMidiDeprecated:
			OutputDebugString("plug called audioMasterWantMidi\n");
			break;

		default:
			{
			char buf[256]; sprintf(buf, "plug called opcode #%i\n", (int)opcode);
			OutputDebugString(buf);
			}
			break;
		#endif
	}

	// Unknown opcode, always return 0
	////////////////////////////////////////////////////////////

	unrefHandle(vstHandle);
	return ret;
}



/*****************************************************************************
 *  effect creation
 *****************************************************************************/



static void CALLBACK onChannelDestroy(HSYNC handle, DWORD channel, DWORD data, USERPTR vstHandle__)
{
	DWORD vstHandle = (DWORD)vstHandle__;
	BASS_VST_PLUGIN* this_ = refHandle(vstHandle);
	if( this_ )
	{
		this_->channelHandle = 0; // do not unlink from BASS or call any other BASS function!
		this_->dspHandle = 0;	  // BASS has already deleted the channel!

		unrefHandle(vstHandle);	  // first call to free the just allocted pointer
		unrefHandle(vstHandle);   // second call to free the channel at all

		checkForwarding();
	}
}

//FILE *out = fopen("bass_vst2.log", "a");
//fprintf(out, "%s %S\r\n", "loadVstLibrary called", dllFile);
//fflush(out);
//fclose(out);

static BOOL loadVstLibrary(BASS_VST_PLUGIN* this_, const void* dllFile, DWORD createFlags)
{
	dllMainEntryFuncType dllMainEntryFuncPtr;

	// init some values
	this_->createFlags						= createFlags;

	// load the library
	if( createFlags & BASS_UNICODE )
		this_->hinst = LoadLibraryW((const LPCWSTR)dllFile);
	else
		this_->hinst = LoadLibraryA((const char*)dllFile);

	if( this_->hinst == NULL )
	{
		SET_ERROR(BASS_ERROR_FILEOPEN);
		return false;
	}

	// get the plugin pointer
	dllMainEntryFuncPtr = (dllMainEntryFuncType)GetProcAddress(this_->hinst, "VSTPluginMain");
	if( dllMainEntryFuncPtr == NULL )
	{
		dllMainEntryFuncPtr = (dllMainEntryFuncType)GetProcAddress(this_->hinst, "main");
		if( dllMainEntryFuncPtr == NULL )
		{
			SET_ERROR(BASS_ERROR_FILEFORM);
			return false;
		}
	}

	// get the aeffect instance
	s_inConstructionVstHandle = this_->vstHandle;
		this_->aeffect = (dllMainEntryFuncPtr)(audioMasterCallbackImpl);
		if(  this_->aeffect == NULL 
		 ||  this_->aeffect->magic != kEffectMagic
	     || (this_->aeffect->__processDeprecated == NULL && this_->aeffect->processReplacing == NULL && !canDoubleReplacing(this_))
		 ||  this_->aeffect->dispatcher == NULL )
		{
			SET_ERROR(BASS_ERROR_FILEFORM);
			s_inConstructionVstHandle = 0;
			return false;
		}
		this_->aeffect->resvd1 = (long)this_->vstHandle;
	s_inConstructionVstHandle = 0;

	// check if there are enough inputs / outputs
	if( this_->type==VSTeffect && this_->aeffect->numInputs <= 0 )
	{
		SET_ERROR(BASS_VST_ERROR_NOINPUTS);
		return false;
	}

	if(  this_->aeffect->numOutputs <= 0 
	 || (this_->type==VSTinstrument && !(this_->aeffect->flags&effFlagsIsSynth)) )
	{
		SET_ERROR(BASS_VST_ERROR_NOOUTPUTS);
		return false;
	}

	// call effOpen - call this before dispatching anything else!
	this_->aeffect->dispatcher(this_->aeffect, effOpen, 0, 0, NULL, 0.0); // effOpen has no return value for errors
	this_->effOpenCalled = true;

	// check of the module supports real time processing -
	// checking for (PluginCanDo("noRealTime")&&PluginCanDo("offline")) is not correct: this only means a realtime plugin can also do offline processing
	long plugCategory = (long)this_->aeffect->dispatcher(this_->aeffect, effGetPlugCategory, 0, 0, NULL, 0.0);
	if( plugCategory == kPlugCategOfflineProcess )
	{
		SET_ERROR(BASS_VST_ERROR_NOREALTIME);
		return false;
	}

	// init the sample rate
	long sampleRate = getSampleRate(this_);
	this_->aeffect->dispatcher(this_->aeffect, effSetSampleRate, 0, 0, NULL, (float)sampleRate);
	// dodgy hack to force some plugins to initialise the sample rate..
	if (this_->aeffect->getParameter && (!(this_->aeffect->flags&effFlagsHasEditor)) && this_->aeffect->numParams > 0)
    {
        const float old = this_->aeffect->getParameter(this_->aeffect, 0);
		this_->aeffect->setParameter(this_->aeffect, 0, (old < 0.5f) ? 1.0f : 0.0f);
		this_->aeffect->setParameter(this_->aeffect, 0, old);
    }

	// this is a safety measure against some plugins that only set their buffers
	// ONCE - this should ensure that they allocate a buffer that's large enough.
	// Normally, the buffer size is set dynamically in PostprocessSamples()
	this_->aeffect->dispatcher(this_->aeffect, effSetBlockSize, 0, sampleRate/*one second*/, NULL, 0.0);
	this_->aeffect->dispatcher(this_->aeffect, effMainsChanged, 0, 1/*resume*/, NULL, 0.0);

	this_->numDefaultValues = 0;
	this_->numLastValues = 0;

	if( this_->aeffect->getParameter )
	{
		// select the first program
		if( this_->aeffect->dispatcher(this_->aeffect, effGetProgram, 0, 0, NULL, 0.0) != 0 )
		{
			this_->aeffect->dispatcher(this_->aeffect, effSetProgram, 0, 0/*select first program*/, NULL, 0.0);
		}

		int paramCount = this_->aeffect->numParams;
		if (paramCount >= 0)
		{
			// remember all defaultValues and init lastValues
			long bytesNeeded = sizeof(float) * paramCount;
			if (bytesNeeded == 0)
				bytesNeeded = sizeof(float) * 24;
			this_->defaultValues = (float*)malloc(bytesNeeded);
			this_->lastValues = (float*)malloc(bytesNeeded);

			if( this_->defaultValues == NULL || this_->lastValues == NULL)
			{
				SET_ERROR(BASS_ERROR_MEM);
				return false;
			}

			memset(this_->defaultValues, 0, bytesNeeded);
			memset(this_->lastValues, 0, bytesNeeded);

			this_->numDefaultValues = paramCount;
			this_->numLastValues = paramCount;

			int paramIndex;
			for( paramIndex = 0; paramIndex < paramCount; paramIndex++ )
			{
				this_->defaultValues[paramIndex] = this_->aeffect->getParameter(this_->aeffect, paramIndex);
			}
		}
	}

	// success
	return true;
}



DWORD BASS_VSTDEF(BASS_VST_ChannelSetDSP)(DWORD channelHandle, const void* dllFile,
										  DWORD createFlags, int priority)
{
	BASS_VST_PLUGIN*		this_ = NULL;

	// attach ok?
	if( !s_mainOk )
	{
		SET_ERROR(BASS_ERROR_UNKNOWN);
		goto Error;
	}

	// get the slot, load the library
	this_ = createHandle(VSTeffect, 0);
	if( this_ == NULL )
	{
		SET_ERROR(BASS_ERROR_MEM);
		goto Error;
	}

	this_->channelHandle = channelHandle;
	if( !loadVstLibrary(this_, dllFile, createFlags) )
	{
		goto Error;
	}

	// okay -- plugin loaded so far: assign it to the given channel -- 
	// after BASS_ChannelSetDSP(), ->dispatcher() etc. should be surrounded by enterVstCritical()/leaveVstCritical()
	if( channelHandle )
	{
		if( !openProcess(this_, this_) )
		{
			SET_ERROR( BASS_ERROR_HANDLE );
			goto Error; // error already logged
		}

		this_->dspHandle = BASS_ChannelSetDSP(channelHandle, doEffectProcess, (USERPTR)this_->vstHandle, priority);
		if( this_->dspHandle == 0 )
		{
			goto Error; // error already logged by BASS
		}

		DWORD syncHandle = BASS_ChannelSetSync(channelHandle, BASS_SYNC_FREE, 0, onChannelDestroy, (USERPTR)this_->vstHandle);
		if( syncHandle == 0 )
		{
			goto Error; // error already logged by BASS
		}
	}


	// success
	checkForwarding();

	RETURN_SUCCESS(this_->vstHandle);

Error:
	// error - error already set by BASS or by using SET_ERROR
	if( this_ )
		unrefHandle(this_->vstHandle);

	return 0;
}



BOOL BASS_VSTDEF(BASS_VST_ChannelRemoveDSP)(DWORD channelHandle, DWORD vstHandle)
{
	if( !unrefHandle(vstHandle) )
		RETURN_ERROR( BASS_ERROR_HANDLE );

	checkForwarding();

	RETURN_SUCCESS( true );
}



/*****************************************************************************
 *  instrument creation
 *****************************************************************************/



DWORD BASS_VSTDEF(BASS_VST_ChannelCreate)(DWORD freq, DWORD chans, const void* dllFile, DWORD createFlags)
{
	BASS_VST_PLUGIN*		this_ = NULL;

	// attach ok?
	if( !s_mainOk )
	{
		SET_ERROR(BASS_ERROR_UNKNOWN);
		goto Error;
	}

	// get the slot, load the library
	{
		DWORD vstHandle = BASS_StreamCreate(freq, chans, createFlags, doInstrumentProcess, 0);
		if( vstHandle == 0 )
			goto Error; // error already logged by BASS
	
		this_ = createHandle(VSTinstrument, vstHandle);
		if( this_ == NULL )
		{
			SET_ERROR(BASS_ERROR_MEM);
			goto Error;
		}
	}

	this_->channelHandle = this_->vstHandle;
	if( !loadVstLibrary(this_, dllFile, createFlags) )
	{
		goto Error; // error already logged by loadVstLibrary()
	}

	// okay -- plugin loaded so far: start process	
	if( !openProcess(this_, this_) )
	{
		SET_ERROR( BASS_ERROR_HANDLE );
		goto Error; // error already logged
	}

	// success -- checkForwarding() is not needed as forwarding only affects the VSTeffects
	RETURN_SUCCESS(this_->vstHandle);

Error:
	// error - error already set by BASS or by using SET_ERROR
	if( this_ )
		unrefHandle(this_->vstHandle);
	return 0;
}



BOOL BASS_VSTDEF(BASS_VST_ChannelFree)(DWORD vstHandle)
{
	if( !unrefHandle(vstHandle) )
		RETURN_ERROR( BASS_ERROR_HANDLE );

	if( !BASS_StreamFree(vstHandle) )
		return false; // error already set

	// success -- checkForwarding() is not needed as forwarding only affects the VSTeffects
	RETURN_SUCCESS(true);
}



/*****************************************************************************
 *  parameter handling
 *****************************************************************************/


static BASS_VST_PLUGIN* refHandle_checkParamIndex(DWORD vstHandle, int paramIndex)
{
	BASS_VST_PLUGIN* this_ = refHandle(vstHandle);
	if( this_ == NULL )
		return NULL;

	if( paramIndex < 0 
	 || paramIndex >= this_->aeffect->numParams
	 || this_->aeffect->getParameter == NULL
	 || this_->aeffect->setParameter == NULL )
	{
		unrefHandle(vstHandle);
		return NULL;
	}

	return this_;
}



int BASS_VSTDEF(BASS_VST_GetParamCount)(DWORD vstHandle)
{
	BASS_VST_PLUGIN* this_ = refHandle(vstHandle);
	if( this_ == NULL )
		RETURN_ERROR( BASS_ERROR_HANDLE );

	int paramCount = this_->aeffect->numParams;

	unrefHandle(vstHandle);

	RETURN_SUCCESS( paramCount );
}



BOOL BASS_VSTDEF(BASS_VST_GetParamInfo)(DWORD vstHandle, int paramIndex, BASS_VST_PARAM_INFO* info)
{
	char largeBuf[512];
	
	// init the info return structure
	if( info == NULL )
		RETURN_ERROR( BASS_ERROR_ILLPARAM );

	// get the VST pointer
	BASS_VST_PLUGIN* this_ = refHandle_checkParamIndex(vstHandle, paramIndex);
	if( this_ == NULL )
		RETURN_ERROR( BASS_ERROR_HANDLE );

	// do what to do
	enterVstCritical(this_);

		#if 0	
			// i have not found any plugin that supports the VstParameterProperties API ... 
			// seems to be very uncommon ... if really wanted by the user, he can use the aeffect
			// pointer returned by BASS_VST_GetInfo()
			VstParameterProperties prop;
			memset(&prop, 0, sizeof(VstParameterProperties));
			this_->aeffect->dispatcher(this_->aeffect, effGetParameterProperties, paramIndex, 0, &prop, 0.0);
		#endif

		largeBuf[0] = 0;
		this_->aeffect->dispatcher(this_->aeffect, effGetParamLabel, paramIndex, 0, (void*)largeBuf, 0.0);
		strncpy(info->unit, largeBuf, 24);
		info->unit[kVstMaxParamStrLen - 1] = 0;

		largeBuf[0] = 0;
		this_->aeffect->dispatcher(this_->aeffect, effGetParamDisplay, paramIndex, 0, (void*)largeBuf, 0.0);
		strncpy(info->display, largeBuf, 24);
		info->display[kVstMaxParamStrLen - 1] = 0;

		largeBuf[0] = 0;
		this_->aeffect->dispatcher(this_->aeffect, effGetParamName, paramIndex, 0, (void*)largeBuf, 0.0);
		strncpy(info->name, largeBuf, 24);
		info->name[kVstMaxParamStrLen - 1] = 0;

		if (paramIndex < this_->numDefaultValues)
			info->defaultValue = this_->defaultValues[paramIndex];
		else
			info->defaultValue = 0.0; // we don't know better (as we only cached the default values initially)

	leaveVstCritical(this_);

	unrefHandle(vstHandle);

	RETURN_SUCCESS( true );
}



float BASS_VSTDEF(BASS_VST_GetParam)(DWORD vstHandle, int paramIndex)
{
	BASS_VST_PLUGIN* this_ = refHandle_checkParamIndex(vstHandle, paramIndex);
	if( this_ == NULL )
		RETURN_ERROR( BASS_ERROR_HANDLE );

	enterVstCritical(this_);
		float param = this_->aeffect->getParameter(this_->aeffect, paramIndex);
	leaveVstCritical(this_);

	unrefHandle(vstHandle);

	RETURN_SUCCESS( param );
}


    
BOOL BASS_VSTDEF(BASS_VST_SetParam)(DWORD vstHandle, int paramIndex, float value)
{
	bool leaveIdleCritical = false;

	BASS_VST_PLUGIN* this_ = refHandle_checkParamIndex(vstHandle, paramIndex);
	if( this_ == NULL )
		RETURN_ERROR( BASS_ERROR_HANDLE );

	if( this_->editorIsOpen && paramIndex < this_->numLastValues)
	{
		EnterCriticalSection(&s_idleCritical);
		this_->lastValues[paramIndex] = value;
		leaveIdleCritical = true;
	}

	enterVstCritical(this_);
		this_->aeffect->setParameter(this_->aeffect, paramIndex, value);
	leaveVstCritical(this_);

	if( leaveIdleCritical )
	{
		LeaveCriticalSection(&s_idleCritical);
	}

	unrefHandle(vstHandle);

	RETURN_SUCCESS( true );
}



char* BASS_VSTDEF(BASS_VST_GetChunk)(DWORD vstHandle, BOOL isPreset, DWORD* length)
{
	*length = 0;
	BASS_VST_PLUGIN* this_ = refHandle(vstHandle);
	if( this_ == NULL )
		RETURN_ERROR( BASS_ERROR_HANDLE );

	if (!(this_->aeffect->flags&effFlagsProgramChunks))
		RETURN_ERROR( BASS_ERROR_NOTAVAIL );

	enterVstCritical(this_);

		void* data = 0;
		int size = (int)this_->aeffect->dispatcher(this_->aeffect, effGetChunk, isPreset ? 1 : 0, 0, &data, 0.0f);
		if (data != 0 && size > 0)
        {
			// alloc our temp buffer
			this_->tempChunkData = (char*)realloc(this_->tempChunkData, size );
            memcpy(this_->tempChunkData, data, size);
			*length = size;
        }
		else
		{
			if (this_->tempChunkData)
			{
				free(this_->tempChunkData);
				this_->tempChunkData = NULL;
			}
		}

	leaveVstCritical(this_);

	unrefHandle(vstHandle);

	RETURN_SUCCESS( this_->tempChunkData );
}


DWORD BASS_VSTDEF(BASS_VST_SetChunk)(DWORD vstHandle, BOOL isPreset, const char* chunk, DWORD length)
{
	BASS_VST_PLUGIN* this_ = refHandle(vstHandle);
	if( this_ == NULL )
		RETURN_ERROR( BASS_ERROR_HANDLE );

	if (!(this_->aeffect->flags&effFlagsProgramChunks))
		RETURN_ERROR( BASS_ERROR_NOTAVAIL );

	if(length <= 0)
		RETURN_ERROR( BASS_ERROR_ILLPARAM );

	enterVstCritical(this_);

		int size = (int)this_->aeffect->dispatcher(this_->aeffect, effSetChunk, isPreset ? 1 : 0, length, (void*)chunk, 0.0f);

	leaveVstCritical(this_);

	unrefHandle(vstHandle);

	RETURN_SUCCESS( size );
}



/*****************************************************************************
 *  program handling
 *****************************************************************************/



static BASS_VST_PLUGIN* refHandle_checkProgramIndex(DWORD vstHandle, int programIndex)
{
	BASS_VST_PLUGIN* this_ = refHandle(vstHandle);
	if( this_ == NULL )
		return NULL;

	if( programIndex < 0 
	 || programIndex >= this_->aeffect->numPrograms
	 || this_->aeffect->getParameter == NULL
	 || this_->aeffect->setParameter == NULL )
	{
		unrefHandle(vstHandle);
		return NULL;
	}

	return this_;
}



int BASS_VSTDEF(BASS_VST_GetProgramCount)(DWORD vstHandle)
{
	assert( _CrtCheckMemory() );

	BASS_VST_PLUGIN* this_ = refHandle(vstHandle);
	if( this_ == NULL )
		RETURN_ERROR( BASS_ERROR_HANDLE );

	int programCount = this_->aeffect->numPrograms;

	unrefHandle(vstHandle);

	assert( _CrtCheckMemory() );

	RETURN_SUCCESS( programCount );
}



int BASS_VSTDEF(BASS_VST_GetProgram)(DWORD vstHandle)
{
	assert( _CrtCheckMemory() );

	BASS_VST_PLUGIN* this_ = refHandle(vstHandle);
	if( this_ == NULL )
		RETURN_ERROR( BASS_ERROR_HANDLE );

	enterVstCritical(this_);
		int program = (int)this_->aeffect->dispatcher(this_->aeffect, effGetProgram, 0, 0, NULL, 0.0);
	leaveVstCritical(this_);

	unrefHandle(vstHandle);

	assert( _CrtCheckMemory() );

	RETURN_SUCCESS( program )
}



const char* BASS_VSTDEF(BASS_VST_GetProgramName)(DWORD vstHandle, int programIndex)
{
	assert( _CrtCheckMemory() );

	BASS_VST_PLUGIN* this_ = refHandle_checkProgramIndex(vstHandle, programIndex);
	if( this_ == NULL )
		RETURN_ERROR( BASS_ERROR_HANDLE );

	char* programName = this_->tempProgramNameBuf;

	enterVstCritical(this_);

		if( this_->aeffect->dispatcher(this_->aeffect, effGetProgramNameIndexed, programIndex, 
						0, programName, 0.0) == 0 )
		{
			int orgProgramIndex = (int)this_->aeffect->dispatcher(this_->aeffect, effGetProgram, 0, 0, NULL, 0.0);
			if( orgProgramIndex != programIndex )
			{
				this_->aeffect->dispatcher(this_->aeffect, effSetProgram, 0, programIndex, NULL, 0.0);
			}

			this_->aeffect->dispatcher(this_->aeffect, effGetProgramName, 0, 0, programName, 0.0);

			if( orgProgramIndex != programIndex )
			{
				this_->aeffect->dispatcher(this_->aeffect, effSetProgram, 0, orgProgramIndex, NULL, 0.0);
			}
		}

	leaveVstCritical(this_);

	programName[kVstMaxProgNameLen] = 0;

	unrefHandle(vstHandle);

	assert( _CrtCheckMemory() );

	RETURN_SUCCESS( programName );
}



const float* BASS_VSTDEF(BASS_VST_GetProgramParam)(DWORD vstHandle, int programIndex, DWORD* length)
{
	assert( _CrtCheckMemory() );

	*length = 0;
	BASS_VST_PLUGIN* this_ = refHandle_checkProgramIndex(vstHandle, programIndex);
	if( this_ == NULL )
		RETURN_ERROR( BASS_ERROR_HANDLE );

	enterVstCritical(this_);

		int orgProgramIndex = (int)this_->aeffect->dispatcher(this_->aeffect, effGetProgram, 0, 0, NULL, 0.0);
		if( orgProgramIndex != programIndex )
		{
			this_->aeffect->dispatcher(this_->aeffect, effSetProgram, 0, programIndex, NULL, 0.0);
		}

		int numParams = this_->aeffect->numParams;
		// create the temp value buffer on-the-fly (as numParams might be dynamic)
		this_->tempProgramValueBuf = (float*)realloc(this_->tempProgramValueBuf, sizeof(float) * numParams );
		float* param = this_->tempProgramValueBuf;

		if (this_->tempProgramValueBuf != NULL)
		{
			*length = numParams;
			for( int i = 0; i < numParams; i++ )
			{
				param[i] = this_->aeffect->getParameter(this_->aeffect, i);
			}
		}

		if( orgProgramIndex != programIndex )
		{
			this_->aeffect->dispatcher(this_->aeffect, effSetProgram, 0, orgProgramIndex, NULL, 0.0);
		}

	leaveVstCritical(this_);

	unrefHandle(vstHandle);
	
	assert( _CrtCheckMemory() );

	RETURN_SUCCESS( param );
}



BOOL BASS_VSTDEF(BASS_VST_SetProgram)(DWORD vstHandle, int programIndex)
{
	BASS_VST_PLUGIN* this_ = refHandle_checkProgramIndex(vstHandle, programIndex);
	if( this_ == NULL )
		RETURN_ERROR( BASS_ERROR_HANDLE );

	enterVstCritical(this_);
		if (programIndex != this_->aeffect->dispatcher(this_->aeffect, effGetProgram, 0, 0, NULL, 0.0))
			this_->aeffect->dispatcher(this_->aeffect, effSetProgram, 0, programIndex, NULL, 0.0);
	leaveVstCritical(this_);

	unrefHandle(vstHandle);

	RETURN_SUCCESS( true );
}



BOOL BASS_VSTDEF(BASS_VST_SetProgramName)(DWORD vstHandle, int programIndex, const char* newName__)
{
	if( newName__ == NULL )
		RETURN_ERROR( BASS_ERROR_ILLPARAM );

	BASS_VST_PLUGIN* this_ = refHandle_checkProgramIndex(vstHandle, programIndex);
	if( this_ == NULL )
		RETURN_ERROR( BASS_ERROR_HANDLE );

	enterVstCritical(this_);
		int orgProgramIndex = (int)this_->aeffect->dispatcher(this_->aeffect, effGetProgram, 0, 0, NULL, 0.0);
		if( orgProgramIndex != programIndex )
		{
			this_->aeffect->dispatcher(this_->aeffect, effSetProgram, 0, programIndex, NULL, 0.0);
		}

		char newName[128];
		strncpy(newName, newName__, kVstMaxProgNameLen);
		newName[kVstMaxProgNameLen] = 0;

		this_->aeffect->dispatcher(this_->aeffect, effSetProgramName, 0, 0, newName, 0.0);

		if( orgProgramIndex != programIndex )
		{
			this_->aeffect->dispatcher(this_->aeffect, effSetProgram, 0, orgProgramIndex, NULL, 0.0);
		}
	leaveVstCritical(this_);

	unrefHandle(vstHandle);

	RETURN_SUCCESS( true );
}


BOOL BASS_VSTDEF(BASS_VST_SetEditKnobMode)(DWORD vstHandle, int knobMode)
{
	
	BASS_VST_PLUGIN* this_ = refHandle(vstHandle);
	if( this_ == NULL )
		RETURN_ERROR( BASS_ERROR_HANDLE );

	enterVstCritical(this_);

		this_->aeffect->dispatcher(this_->aeffect, effSetEditKnobMode, 0, knobMode, NULL, 0.0);

	leaveVstCritical(this_);
	
	unrefHandle(vstHandle);
	
	RETURN_SUCCESS( true );
}


BOOL BASS_VSTDEF(BASS_VST_SetProgramParam)(DWORD vstHandle, int programIndex, const float* param, DWORD length)
{
	if( param == NULL )
		RETURN_ERROR( BASS_ERROR_ILLPARAM );

	BASS_VST_PLUGIN* this_ = refHandle_checkProgramIndex(vstHandle, programIndex);
	if( this_ == NULL )
		RETURN_ERROR( BASS_ERROR_HANDLE );

	enterVstCritical(this_);

		int orgProgramIndex = (int)this_->aeffect->dispatcher(this_->aeffect, effGetProgram, 0, 0, NULL, 0.0), i;
		if( orgProgramIndex != programIndex )
		{
			this_->aeffect->dispatcher(this_->aeffect, effSetProgram, 0, programIndex, NULL, 0.0);
		}

		long numParams = this_->aeffect->numParams;
		for( i = 0; i < numParams; i++ )
		{
			if (i < (int)length)
				this_->aeffect->setParameter(this_->aeffect, i, param[i]);
		}

		if( orgProgramIndex != programIndex )
		{
			this_->aeffect->dispatcher(this_->aeffect, effSetProgram, 0, orgProgramIndex, NULL, 0.0);
		}
		
	leaveVstCritical(this_);
	
	unrefHandle(vstHandle);

	RETURN_SUCCESS( true );
}




/*****************************************************************************
 *  Get Common Information
 *****************************************************************************/



BOOL BASS_VSTDEF(BASS_VST_GetInfo)(DWORD vstHandle, BASS_VST_INFO* info)
{
	// init the info return structure
	if( info == NULL )
		RETURN_ERROR( BASS_ERROR_ILLPARAM );

	// get the VST pointer
	BASS_VST_PLUGIN* this_ = refHandle(vstHandle);
	if( this_ == NULL )
		RETURN_ERROR( BASS_ERROR_HANDLE );

	enterVstCritical(this_);

		// fill the return structure
		
		info->effectName[0] = 0;
		this_->aeffect->dispatcher(this_->aeffect, effGetEffectName, 0, 0, (void*)info->effectName, 0.0);

		info->productName[0] = 0;
		this_->aeffect->dispatcher(this_->aeffect, effGetProductString, 0, 0, (void*)info->productName, 0.0);

		info->vendorName[0] = 0;
		this_->aeffect->dispatcher(this_->aeffect, effGetVendorString, 0, 0, (void*)info->vendorName, 0.0);

		info->channelHandle		= this_->channelHandle;
		info->uniqueID			= this_->aeffect->uniqueID;
		info->effectVersion		= this_->aeffect->version;
		info->vendorVersion		= (DWORD)this_->aeffect->dispatcher(this_->aeffect, effGetVendorVersion, 0, 0, NULL, 0.0);
		info->effectVstVersion	= (DWORD)this_->aeffect->dispatcher(this_->aeffect, effGetVstVersion, 0, 0, NULL, 0.0);
		info->hostVstVersion	= kVstVersion;
		info->chansIn			= this_->aeffect->numInputs;
		info->chansOut			= this_->aeffect->numOutputs;
		info->initialDelay		= this_->aeffect->initialDelay;
		info->aeffect			= this_->aeffect;
		info->isInstrument		= this_->type==VSTinstrument? 1 : 0;
		info->dspHandle			= this_->dspHandle;

		// editor stuff
		info->hasEditor	= this_->aeffect->flags&effFlagsHasEditor? 1 : 0;
		if( info->hasEditor )
		{
			ERect* rect = NULL;
			this_->aeffect->dispatcher(this_->aeffect, effEditGetRect, 0, 0, &rect, 0.0);
			if( rect )
			{
				info->editorWidth  = rect->right-rect->left;
				info->editorHeight = rect->bottom-rect->top;
				if( info->editorWidth  < 0 ) info->editorWidth  = 0; // unknown - some plugins tell their size only after creation of the editor window
				if( info->editorHeight < 0 ) info->editorHeight = 0; //              - " -
			}
			else
			{
				info->hasEditor = 0;
			}
		}

	leaveVstCritical(this_);

	unrefHandle(vstHandle);

	RETURN_SUCCESS( true );
}



/*****************************************************************************
 *  The Editor, Misc.
 *****************************************************************************/



BOOL BASS_VSTDEF(BASS_VST_EmbedEditor)
#ifdef _WIN32
    (DWORD vstHandle, HWND parentWindow)
#else
    (DWORD vstHandle, void* parentWindow)
#endif
{
	BASS_VST_PLUGIN* this_ = refHandle(vstHandle);
	if( this_ == NULL )
		RETURN_ERROR( BASS_ERROR_HANDLE );

	if( parentWindow )
	{
		// embed / open the editor
		if( this_->editorIsOpen )
		{
			unrefHandle(vstHandle);
			RETURN_ERROR( BASS_ERROR_ALREADY );
		}

		// has the plugin an editor?
		BASS_VST_INFO info;
		if( !BASS_VST_GetInfo(vstHandle, &info) )
		{
			unrefHandle(vstHandle);
			return false; // error alreaedy logged by BASS_VST_GetInfo()
		}

		if( info.hasEditor == 0 )
		{
			unrefHandle(vstHandle);
			RETURN_ERROR( BASS_ERROR_NOTAVAIL );
		}

		enterVstCritical(this_);

			// remember the current parameters as we need to poll them to call BASS_VST_PARAM_CHANGED
			if( this_->aeffect->getParameter )
			{
				int numParam = validateLastValues(this_);
				for( int paramIndex = 0; paramIndex < numParam; paramIndex++ )
				{
					this_->lastValues[paramIndex] = this_->aeffect->getParameter(this_->aeffect, paramIndex);
				}
			}

			// do it!
			this_->aeffect->dispatcher(this_->aeffect, effEditOpen, 0, 0, (void*)parentWindow, 0.0);
			this_->editorIsOpen = true;

		leaveVstCritical(this_);

		this_->needsIdle |= NEEDS_EDIT_IDLE;
		updateIdleTimers(this_);
		checkForwarding();
	}
	else
	{
		// unembed / close the editor
		if( !this_->editorIsOpen )
		{
			unrefHandle(vstHandle);
			RETURN_ERROR( BASS_ERROR_ALREADY );
		}

		// do it!
		this_->needsIdle &= ~NEEDS_EDIT_IDLE;
		updateIdleTimers(this_);

		enterVstCritical(this_);

			this_->aeffect->dispatcher(this_->aeffect, effEditClose, 0, 0, NULL, 0.0);
			this_->editorIsOpen = false;

		leaveVstCritical(this_);

		checkForwarding();
	}

	unrefHandle(vstHandle);

	RETURN_SUCCESS( true );
}



BOOL BASS_VSTDEF(BASS_VST_SetScope)(DWORD vstHandle, DWORD scope)
{
	BASS_VST_PLUGIN* this_ = refHandle(vstHandle);
	if( this_ == NULL )
		RETURN_ERROR( BASS_ERROR_HANDLE );

	this_->editorScope = scope;

	unrefHandle(vstHandle);

	checkForwarding();

	RETURN_SUCCESS( true );
}



BOOL BASS_VSTDEF(BASS_VST_SetCallback)(DWORD vstHandle, VSTPROC* callback, void* userData)
{
	BASS_VST_PLUGIN* this_ = refHandle(vstHandle);
	if( this_ == NULL )
		RETURN_ERROR( BASS_ERROR_HANDLE );

	this_->callback = callback;
	this_->callbackUserData = userData;

	unrefHandle(vstHandle);

	RETURN_SUCCESS( true );
}



BOOL BASS_VSTDEF(BASS_VST_Resume)(DWORD vstHandle)
{
	BASS_VST_PLUGIN* this_ = refHandle(vstHandle);
	if( this_ == NULL )
		RETURN_ERROR( BASS_ERROR_HANDLE );

	if( !this_->effStartProcessCalled )
	{
		unrefHandle(vstHandle);
		RETURN_ERROR( BASS_ERROR_NOPLAY );
	}

	enterVstCritical(this_);
		this_->aeffect->dispatcher(this_->aeffect, effMainsChanged, 0, 0/*suspend*/, NULL, 0.0);
		this_->aeffect->dispatcher(this_->aeffect, effMainsChanged, 0, 1/*resume*/, NULL, 0.0);
	leaveVstCritical(this_);

	unrefHandle(vstHandle);

	RETURN_SUCCESS( true );
}



BOOL BASS_VSTDEF(BASS_VST_SetBypass)(DWORD vstHandle, BOOL newDoBypass)
{
	BASS_VST_PLUGIN* this_ = refHandle(vstHandle);
	if( this_ == NULL )
		RETURN_ERROR( BASS_ERROR_HANDLE );

	if( !this_->effStartProcessCalled )
	{
		unrefHandle(vstHandle);
		RETURN_ERROR( BASS_ERROR_NOPLAY );
	}

	enterVstCritical(this_);

		if( newDoBypass != this_->doBypass )
		{
			if( newDoBypass )
			{	
				this_->doBypass = TRUE;
				this_->aeffect->dispatcher(this_->aeffect, effMainsChanged, 0, 0/*suspend*/, NULL, 0.0);
			}
			else
			{
				this_->aeffect->dispatcher(this_->aeffect, effMainsChanged, 0, 1/*resume*/, NULL, 0.0);
				this_->doBypass = FALSE;
			}
		}

	leaveVstCritical(this_);

	unrefHandle(vstHandle);

	checkForwarding();

	RETURN_SUCCESS( true );
}



BOOL BASS_VSTDEF(BASS_VST_GetBypass)(DWORD vstHandle)
{
	BASS_VST_PLUGIN* this_ = refHandle(vstHandle);
	if( this_ == NULL )
		RETURN_ERROR( BASS_ERROR_HANDLE );

	enterVstCritical(this_);
		BOOL bypass = this_->doBypass;
	leaveVstCritical(this_);

	unrefHandle(vstHandle);

	RETURN_SUCCESS( bypass );
}



BOOL BASS_VSTDEF(BASS_VST_SetLanguage)(const char* lang)
{
	char buffer[16];
	strncpy(buffer, lang, 8);
	buffer[2] = 0;

		 if( strcasecmp(buffer, "de")==0 )	{ s_language = kVstLangGerman;		}
	else if( strcasecmp(buffer, "fr")==0 )	{ s_language = kVstLangFrench;		}
	else if( strcasecmp(buffer, "it")==0 )	{ s_language = kVstLangItalian;		}
	else if( strcasecmp(buffer, "es")==0 )	{ s_language = kVstLangSpanish;		}
	else if( strcasecmp(buffer, "jp")==0 )	{ s_language = kVstLangJapanese;	}
	else /*all other goes to enghlish...*/	{ s_language = kVstLangEnglish;		}

	RETURN_SUCCESS( true );
}



static void queueEventRaw(BASS_VST_PLUGIN* this_, char midi0, char midi1, char midi2, const void* sysexDump, size_t sysexBytes, DWORD* error)
{
	// prepare
	EnterCriticalSection(&this_->midiCritical_);
		
		VstEvent**	eSlot = NULL;
		VstInt32 deltaFrames = 0;

		// initialize MIDI structures
		if( this_->midiEventsCurr == NULL )
		{
			size_t bytesNeeded = sizeof(VstEvents) + MAX_MIDI_EVENTS*sizeof(VstEvent*);
			this_->midiEventsCurr = (VstEvents*)malloc(bytesNeeded);
			if( this_->midiEventsCurr == NULL ) { *error = BASS_ERROR_MEM; goto Error; }
			memset(this_->midiEventsCurr, 0, bytesNeeded);
		}

		// find out the slot for the event
		if( this_->midiEventsCurr->numEvents >= MAX_MIDI_EVENTS )
			{ *error = BASS_ERROR_MEM; goto Error; }

		eSlot = &this_->midiEventsCurr->events[this_->midiEventsCurr->numEvents];
	
		// set up the current event ...
		if( sysexDump )
		{
			// ... SYSEX event: allocate
			size_t bytesNeeded = sizeof(VstMidiSysexEvent) + sysexBytes;
			VstMidiSysexEvent* e = (VstMidiSysexEvent*)malloc(bytesNeeded); 
			if( e == NULL ) { *error = BASS_ERROR_MEM; goto Error; }

			if( *eSlot ) free(*eSlot);
			*eSlot = (VstEvent*)e;

			// ... and setup
			memset(e, 0, bytesNeeded);
			e->type			=	kVstSysExType;
			e->byteSize		=	sizeof(VstMidiSysexEvent) - 8;
			e->deltaFrames	=	deltaFrames;
			e->dumpBytes	=	(VstInt32)sysexBytes;
			e->sysexDump	=	((char*)e) + sizeof(VstMidiSysexEvent);
			memcpy(e->sysexDump, sysexDump, sysexBytes);

			this_->midiEventsCurr->numEvents ++;
		}
		else
		{
			// ... normal MIDI event: allocate
			VstMidiEvent* e = (VstMidiEvent*)*eSlot;
			if( e == NULL )
			{
				assert( sizeof(VstMidiSysexEvent) >= sizeof(VstMidiEvent) ); // this assumption makes it possible to re-use sysex events as midi events
				e = (VstMidiEvent*)malloc(sizeof(VstMidiEvent));
				if( e == NULL ) { *error = BASS_ERROR_MEM; goto Error; }
				*eSlot = (VstEvent*)e;
			}

			// ... and setup
			memset(e, 0, sizeof(VstMidiEvent));
			e->type			=	kVstMidiType;
			e->byteSize		=	sizeof(VstMidiEvent) - 8; // = 24
			e->deltaFrames	=	deltaFrames;
			e->flags		=	kVstMidiEventIsRealtime;
			e->midiData[0]	=	midi0;
			e->midiData[1]	=	midi1;
			e->midiData[2]	=	midi2;

			this_->midiEventsCurr->numEvents ++;
		}

	// unprepare
Error:
	LeaveCriticalSection(&this_->midiCritical_);
}


BOOL BASS_VSTDEF(BASS_VST_ProcessEvent)(DWORD vstHandle, DWORD midiCh, DWORD bassEventId, DWORD param)
{
	#define RAWBYTES(a,b,c)		queueEventRaw(this_, (a), (b), (c), 0, 0, &error);
	#define COMMAND(a,b,c)		RAWBYTES((a)+(char)midiCh, (b), (c))
	#define CONTROLLER(b,c)		RAWBYTES(0xB0+(char)midiCh, (b), (c))
	#define RPN(a,b)			CONTROLLER(101,(a)) CONTROLLER(100,(b))
	#define NRPN(a,b)			CONTROLLER(99, (a)) CONTROLLER(98, (b))
	#define DATAENTRY(a)		CONTROLLER(6,  (a))
	#define DATAENTRY_LSB(a)	CONTROLLER(38, (a))
	#define DATAENTRY_FINE(a)	DATAENTRY((char)((a)>>7)&0x7F) DATAENTRY_LSB((char)(a)&0x7F)
	#define RPN_NRPN_RESET		CONTROLLER(101,127) CONTROLLER(100,127)

	BASS_VST_PLUGIN* this_ = refHandle(vstHandle);
	if( this_ == NULL )
		RETURN_ERROR(BASS_ERROR_HANDLE);
	DWORD error = BASS_OK;
	char loparam = LOBYTE(param), hiparam = HIBYTE(param);

	switch( bassEventId )
	{
		case MIDI_EVENT_NOTE:		COMMAND(hiparam? 0x90:0x80, loparam, hiparam? hiparam:64);	break;
		case MIDI_EVENT_PROGRAM:	COMMAND(0xC0, loparam, 0);									break;
		case MIDI_EVENT_CHANPRES:	COMMAND(0xD0, loparam, 0);									break;
		case MIDI_EVENT_PITCH:		COMMAND(0xE0, (char)param&0x7F, (char)(param>>7)&0x7F);		break;
		case MIDI_EVENT_BANK:		CONTROLLER(0, loparam);										break;
		case MIDI_EVENT_MODULATION:	CONTROLLER(1, loparam);										break;
		case MIDI_EVENT_PORTATIME:	CONTROLLER(5, loparam);										break;
		case MIDI_EVENT_VOLUME:		CONTROLLER(7, loparam);										break;
		case MIDI_EVENT_PAN:		CONTROLLER(10, loparam);									break;
		case MIDI_EVENT_EXPRESSION:	CONTROLLER(11, loparam);									break;
		case MIDI_EVENT_SUSTAIN:	CONTROLLER(64, loparam);									break;
		case MIDI_EVENT_PORTAMENTO:	CONTROLLER(65, loparam);									break;
		case MIDI_EVENT_RESONANCE:	CONTROLLER(71, loparam);									break;
		case MIDI_EVENT_CUTOFF:		CONTROLLER(77, loparam);									break;
		case MIDI_EVENT_PORTANOTE:	CONTROLLER(84, loparam);									break;
		case MIDI_EVENT_REVERB:		CONTROLLER(91, loparam);									break;
		case MIDI_EVENT_CHORUS:		CONTROLLER(93, loparam);									break;
		case MIDI_EVENT_SOUNDOFF:	CONTROLLER(120, 0);											break;
		case MIDI_EVENT_RESET:		CONTROLLER(121, 0);											break;
		case MIDI_EVENT_NOTESOFF:	CONTROLLER(123, 0);											break;
		case MIDI_EVENT_MODE:		CONTROLLER(param?126:127, 0);								break;
		case MIDI_EVENT_PITCHRANGE:	RPN(0,0) DATAENTRY(loparam)	RPN_NRPN_RESET					break;
		case MIDI_EVENT_FINETUNE:	RPN(0,1) DATAENTRY_FINE(param) RPN_NRPN_RESET				break;
		case MIDI_EVENT_COARSETUNE:	RPN(0,2) DATAENTRY(loparam)	RPN_NRPN_RESET					break;
		default:					error = BASS_ERROR_ILLPARAM;								break;
		// missing: MIDI_EVENT_DRUMS, MIDI_EVENT_MASTERVOL, MIDI_EVENT_TEMPO, MIDI_EVENT_MIXLEVEL, MIDI_EVENT_TRANSPOSE
		//		MIDI_EVENT_REVERB_*, MIDI_EVENT_CHORUS_*,  MIDI_EVENT_DRUM_*
	}

	unrefHandle(vstHandle);

	if( error == BASS_OK )
		RETURN_SUCCESS( true )
	else
		RETURN_ERROR( error )
}



BOOL BASS_VSTDEF(BASS_VST_ProcessEventRaw)(DWORD vstHandle, const void* bassEventPtr, DWORD param)
{
	BASS_VST_PLUGIN* this_ = refHandle(vstHandle);
	if( this_ == NULL )
		RETURN_ERROR(BASS_ERROR_HANDLE);
	DWORD error = BASS_OK;

	if( param == 0 )
	{
		DWORD bassEventId = ((DWORD)bassEventPtr)&0xFFFFFF;
		queueEventRaw(this_, (char)(bassEventId>>16), (char)((bassEventId>>8)&0xFF), (char)(bassEventId&0xFF), 0, 0, &error);
	}
	else
	{
		queueEventRaw(this_, 0, 0, 0, bassEventPtr, param, &error);
	}

	unrefHandle(vstHandle);

	if( error == BASS_OK )
		RETURN_SUCCESS( true )
	else
		RETURN_ERROR( error )
}


BOOL BASS_VSTDEF(BASS_VST_SupportsRds)(DWORD vstHandle)
{
	BASS_VST_PLUGIN* this_ = refHandle(vstHandle);
	if( this_ == NULL )
		RETURN_ERROR(BASS_ERROR_HANDLE);
	
	DWORD error = BASS_OK;

	error = BASS_ERROR_FILEFORM;

	unrefHandle(vstHandle);

	if( error == BASS_OK )
		RETURN_SUCCESS( true )
	else
		RETURN_ERROR( error )
}

BOOL BASS_VSTDEF(BASS_VST_SetRdsPs)(DWORD vstHandle, char* text, bool now)
{
	BASS_VST_PLUGIN* this_ = refHandle(vstHandle);
	if( this_ == NULL )
		RETURN_ERROR(BASS_ERROR_HANDLE);
	if( text == NULL )
		RETURN_ERROR(BASS_ERROR_ILLPARAM);
	DWORD error = BASS_OK;

	//this_->SetRdsPsPtr(text, now);

	unrefHandle(vstHandle);

	if( error == BASS_OK )
		RETURN_SUCCESS( true )
	else
		RETURN_ERROR( error )
}

BOOL BASS_VSTDEF(BASS_VST_SetRdsRt)(DWORD vstHandle, bool on, char* text, bool now)
{
	BASS_VST_PLUGIN* this_ = refHandle(vstHandle);
	if( this_ == NULL )
		RETURN_ERROR(BASS_ERROR_HANDLE);
	if( text == NULL )
		RETURN_ERROR(BASS_ERROR_ILLPARAM);
	DWORD error = BASS_OK;

	//this_->SetRdsRtPtr(on, text, now);

	unrefHandle(vstHandle);

	if( error == BASS_OK )
		RETURN_SUCCESS( true )
	else
		RETURN_ERROR( error )
}

BOOL BASS_VSTDEF(BASS_VST_SetRdsTa)(DWORD vstHandle, bool ta, bool tp)
{
	BASS_VST_PLUGIN* this_ = refHandle(vstHandle);
	if( this_ == NULL )
		RETURN_ERROR(BASS_ERROR_HANDLE);
	DWORD error = BASS_OK;

	//this_->SetRdsTaPtr(ta, tp);

	unrefHandle(vstHandle);

	if( error == BASS_OK )
		RETURN_SUCCESS( true )
	else
		RETURN_ERROR( error )
}
