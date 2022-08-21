 
/*****************************************************************************
 *  BASS_VST
 *****************************************************************************
 *
 *  File:       bass_vst_handle.cpp
 *  Authors:    Bjoern Petersen
 *  Purpose:    Handle handling
 *
 *	Version History:
 *	22.04.2006	Created in this form (bp)
 *  27.02.2015  Modified by Bernd Niedergesaess
 *              - tempChunkData clean-up
 *              - validateLastValues added
 *
 *  (C) Bjoern Petersen Software Design and Development
 *  
 *****************************************************************************/



#include "bass_vst_impl.h"



/*****************************************************************************
 *  global init / exit
 *****************************************************************************/



static CRITICAL_SECTION		s_handleCritical; // initialized in main()
static sjhash				s_handleHash;

CRITICAL_SECTION			s_forwardCritical;



void initHandleHandling()
{
	InitializeCriticalSection(&s_handleCritical);
	sjhashInit(&s_handleHash, SJHASH_INT, /*keytype*/ 0/*copyKey*/);

	InitializeCriticalSection(&s_forwardCritical);
}



void exitHandleHandling()
{
	DeleteCriticalSection(&s_handleCritical);
	sjhashClear(&s_handleHash);

	DeleteCriticalSection(&s_forwardCritical);
}



/*****************************************************************************
 *  create / destroy handles
 *****************************************************************************/



BASS_VST_PLUGIN* createHandle(DWORD type, DWORD requestedHandleValue)
{
	// allocate memory and init the memory
	BASS_VST_PLUGIN* this_ = (BASS_VST_PLUGIN*)malloc(sizeof(BASS_VST_PLUGIN));
	if( this_ == NULL )
		return NULL;

	memset(this_, 0, sizeof(BASS_VST_PLUGIN));
	this_->type = type;

	// calculate the handle to use - either use the given one or simply the pointer
	if( requestedHandleValue == 0 )
	{
		requestedHandleValue = (DWORD)this_;
	}

	// make sure, the handle value is okay
	EnterCriticalSection(&s_handleCritical);

		if( sjhashFind(&s_handleHash, NULL, /*pKey, not needed*/ (int)requestedHandleValue/*nKey*/) == NULL )
		{
			this_->vstHandle = requestedHandleValue;
			sjhashInsert(&s_handleHash, NULL, /*pKey, not needed*/ (int)this_->vstHandle, /*nKey*/ this_/*pData*/);
		}

	LeaveCriticalSection(&s_handleCritical);

	if( this_->vstHandle == 0 )
	{
		free(this_);
		return NULL;
	}

	// init some basic data
	InitializeCriticalSection(&this_->vstCritical_);
	InitializeCriticalSection(&this_->midiCritical_);

	this_->handleUsage = 1;

	return this_;
}



static void destroyHandle(BASS_VST_PLUGIN* this_)
{
	// this function is only called indirectly if the last reference to a handle is lost

	// unlink from everything ...

	// ... delete any idle
	this_->needsIdle = 0;
	updateIdleTimers(this_);

	// ... remove the plugin from the channel
	if( this_->channelHandle && this_->dspHandle )
		BASS_ChannelRemoveDSP(this_->channelHandle, this_->dspHandle);

	// ... stop process
	if( this_->effStartProcessCalled )
		closeProcess(this_);

	// ... close the editor
	if( this_->editorIsOpen )
	{
		enterVstCritical(this_);
			this_->aeffect->dispatcher(this_->aeffect, effEditClose, 0, 0, NULL, 0.0);
		leaveVstCritical(this_);
	}

	// ... call effClose
	if( this_->effOpenCalled )
	{
		enterVstCritical(this_);
			this_->aeffect->dispatcher(this_->aeffect, effClose, 0, 0, NULL, 0.0);
		leaveVstCritical(this_);
	}

	// unload the library delayed - otherwise we get some curious crashes here and there ...
	// if the library is aleady pending, increase the unload counter
	EnterCriticalSection(&s_idleCritical);
		long oldVal = (long)sjhashFind(&s_unloadPendingInstances, NULL, /*pKey, not needed*/ 
			(int)this_->hinst/*nKey*/);

		sjhashInsert(&s_unloadPendingInstances, NULL, /*pKey, not needed*/ (int)this_->hinst, /*nKey*/ 
			(void*)(oldVal+1)/*pData*/);

		s_unloadPendingCountdown = IDLE_UNLOAD_PENDING_COUNTDOWN;
		createIdleTimers();
	LeaveCriticalSection(&s_idleCritical);

	// delete "easy" data
	DeleteCriticalSection(&this_->vstCritical_);
	DeleteCriticalSection(&this_->midiCritical_);

	if( this_->midiEventsCurr )
	{
		for( int i=0; i<MAX_MIDI_EVENTS; i++ ) { if( this_->midiEventsCurr->events[i] ) free( this_->midiEventsCurr->events[i] ); }
		free(this_->midiEventsCurr);
	}

	if( this_->midiEventsPrev )
	{
		for( int i=0; i<MAX_MIDI_EVENTS; i++ ) { if( this_->midiEventsPrev->events[i] ) free( this_->midiEventsPrev->events[i] ); }
		free(this_->midiEventsPrev);
	}

	if( this_->defaultValues )
		free(this_->defaultValues);

	if( this_->lastValues )
		free(this_->lastValues);

	if( this_->tempProgramValueBuf )
		free(this_->tempProgramValueBuf);

	if (this_->tempChunkData)
		free(this_->tempChunkData);

	freeChansBuffers(this_);
	freeTempBuffer(this_);

	free(this_);
}



/*****************************************************************************
 *  ref / unref handles
 *****************************************************************************/



BASS_VST_PLUGIN* refHandle(DWORD handle)
{
	BASS_VST_PLUGIN* this_ = NULL;
	EnterCriticalSection(&s_handleCritical);
		this_ = (BASS_VST_PLUGIN*)sjhashFind(&s_handleHash, NULL, /*pKey, not needed*/ (int)handle/*nKey*/);
		if( this_ )
			this_->handleUsage++;
	LeaveCriticalSection(&s_handleCritical);

	return this_;
}



BOOL unrefHandle(DWORD handle)
{
	bool ret = false;

	BASS_VST_PLUGIN* ptrToDestroy = NULL;

	EnterCriticalSection(&s_handleCritical);
		BASS_VST_PLUGIN* this_ = (BASS_VST_PLUGIN*)sjhashFind(&s_handleHash, NULL, /*pKey, not needed*/ (int)handle/*nKey*/);
		if( this_ )
		{
			ret = true;
			this_->handleUsage--;
			if( this_->handleUsage <= 0 )
			{
				sjhashInsert(&s_handleHash, NULL, /*pKey, not needed*/ (int)this_->vstHandle, /*nKey*/ 0/*pData = remove*/);
				ptrToDestroy = this_;
			}
		}
	LeaveCriticalSection(&s_handleCritical);

	if( ptrToDestroy )
	{
		destroyHandle(ptrToDestroy);
	}

	return ret;
}



HMODULE				 s_hDllKernel32 = NULL;
static BOOL (WINAPI *m_pfnTryEnterCriticalSection)(CRITICAL_SECTION*) = NULL;
static BOOL          m_pfnTryEnterCriticalSection_loadingTried = false;
BOOL tryEnterVstCritical(BASS_VST_PLUGIN* this_)
{
	// note that this function will wait if trying is impossible!
	if( !m_pfnTryEnterCriticalSection_loadingTried )
	{
		m_pfnTryEnterCriticalSection_loadingTried = true;
		OSVERSIONINFO stVersionInfo;
		stVersionInfo.dwOSVersionInfoSize = sizeof(stVersionInfo);
		if(GetVersionEx(&stVersionInfo) && (stVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT))
		{
			if( s_hDllKernel32 == NULL )
			{
				s_hDllKernel32 = LoadLibrary("kernel32.dll");
			}

			if( s_hDllKernel32 )
			{
				m_pfnTryEnterCriticalSection = (BOOL (WINAPI *)(CRITICAL_SECTION*))GetProcAddress(s_hDllKernel32, "TryEnterCriticalSection");
			}
		}
	}

	if( m_pfnTryEnterCriticalSection )
	{
		return m_pfnTryEnterCriticalSection(&this_->vstCritical_);
	}
	else
	{
		EnterCriticalSection(&this_->vstCritical_);
		return TRUE;
	}
}



void enterVstCritical(BASS_VST_PLUGIN* this_)
{
	EnterCriticalSection(&this_->vstCritical_);
}



void leaveVstCritical(BASS_VST_PLUGIN* this_)
{
	LeaveCriticalSection(&this_->vstCritical_);
}

/*****************************************************************************
 *  lastValues checking and dynamic resizing
 *****************************************************************************/


int validateLastValues(BASS_VST_PLUGIN* this_)
{
	if( this_->aeffect->getParameter )
	{
		int paramCount = this_->aeffect->numParams;
		// check, if the param count has changed
		if (paramCount != this_->numLastValues)
		{
			if (paramCount > this_->numLastValues)
			{
				// increase the lastValues array
				long bytesNeeded = sizeof(float) * paramCount;
				if( (this_->lastValues = (float*)realloc(this_->lastValues, bytesNeeded)) !=  NULL )
				{
					// success, so init the newly added values
					int paramIndex = this_->numLastValues;
					for( ; paramIndex < paramCount; paramIndex++ )
					{
						this_->lastValues[paramIndex] = this_->aeffect->getParameter(this_->aeffect, paramIndex);
					}
					// set the new number
					this_->numLastValues = paramCount;
				}   // else lastValues and numLastValues are left unchanged (should never happen)
			}
			else if (paramCount > 0)
			{
				// the new paramCount is smaller than the old paramCount
				// lastValues is left unchanged (but numLastValues is set to the new value)
				this_->numLastValues = paramCount;
			}
			else
			{
				// lastValues is left unchanged (but numLastValues is set to 0)
				this_->numLastValues = 0;
			}
		}
	}

	return this_->numLastValues;
}



/*****************************************************************************
 *  handle forwarding
 *****************************************************************************/


void checkForwarding()
{
	sjhash oldForwardReceivers;
	sjhashInit(&oldForwardReceivers, SJHASH_INT, /*keytype*/ 0/*copyKey*/);

	EnterCriticalSection(&s_forwardCritical);
	EnterCriticalSection(&s_handleCritical);

		// collect all "old" forward receivers
		BASS_VST_PLUGIN* this_;
		sjhashElem *elemThis = sjhashFirst(&s_handleHash), *elemOther;
		while( elemThis )
		{
			this_ = (BASS_VST_PLUGIN*)sjhashData(elemThis);

			// reset all forwardigs
			this_->forwardDataToOtherCnt = 0;

			// effStartProcessCalled set without an own channel handle?
			// -> this is an old forwarding -> mark this, maybe we will remove it later
			if( this_->type==VSTeffect && this_->channelHandle==0 && this_->effStartProcessCalled )
			{
				sjhashInsert(&oldForwardReceivers, NULL, /*pKey, not needed*/ (int)this_, /*nKey*/ 
					(void*)1/*pData - 0 = remove*/);
			}

			elemThis = sjhashNext(elemThis);
		}

		// search for vst handles with no channel but with the editor opened
		elemThis = sjhashFirst(&s_handleHash);
		while( elemThis )
		{
			this_ = (BASS_VST_PLUGIN*)sjhashData(elemThis);

			if( this_->type==VSTeffect && this_->channelHandle==0 && this_->editorIsOpen )
			{
				elemOther = sjhashFirst(&s_handleHash);
				while( elemOther )
				{
					BASS_VST_PLUGIN* other_ = (BASS_VST_PLUGIN*)sjhashData(elemOther);

					if( other_->channelHandle != 0
					 && other_->editorScope == this_->editorScope )
					{
						other_->forwardDataToOtherVstHandles[other_->forwardDataToOtherCnt] = this_->vstHandle;
						if( other_->forwardDataToOtherCnt < MAX_FWD-1 ) // if the buffer (128(!) handles) is full, this will only result in display problems of VU meters, nothing really to worry about ...
							other_->forwardDataToOtherCnt++;

						if( sjhashInsert(&oldForwardReceivers, NULL, /*pKey, not needed*/ (int)this_, /*nKey*/ 
							(void*)0/*pData - 0 = remove*/) == 0 )
						{
							openProcess(this_, other_);
						}
						else if( other_->effBlockSize )
						{
							callMainsChanged(this_, other_->effBlockSize);
						}
					}

					elemOther = sjhashNext(elemOther);
				}
			}

			elemThis = sjhashNext(elemThis);
		}

		// close the rest of the forwarding handles
		elemThis = sjhashFirst(&oldForwardReceivers);
		while( elemThis )
		{
			this_ = (BASS_VST_PLUGIN*)sjhashKeysize(elemThis);
			closeProcess(this_);

			elemThis = sjhashNext(elemThis);
		}


	LeaveCriticalSection(&s_handleCritical);
	LeaveCriticalSection(&s_forwardCritical);

	sjhashClear(&oldForwardReceivers);
}
