 
/*****************************************************************************
 *  BASS_VST
 *****************************************************************************
 *
 *  File:       bass_vst_idle.cpp
 *  Authors:    Bjoern Petersen
 *  Purpose:    Idle handling
 *
 *	Version History:
 *	22.04.2006	Created in this form (bp)
 *
 *  (C) Bjoern Petersen Software Design and Development
 *  
 *****************************************************************************/



#include "bass_vst_impl.h"



sjhash				s_idleHash;
CRITICAL_SECTION	s_idleCritical;
static UINT			s_idleTimerHandle = 0;

sjhash				s_unloadPendingInstances;
long				s_unloadPendingCountdown = 0;


static void checkForChangedParam(BASS_VST_PLUGIN* this_)
{
	// check if any parameters have beed changed ...
	assert( _CrtCheckMemory() );

	bool paramChanged = false;
	int  oldParamCount = this_->numLastValues;
	int  newParamCount = oldParamCount;

	if( this_->aeffect->getParameter )
	{
		enterVstCritical(this_);

			newParamCount = validateLastValues(this_);
			float param, *params = this_->lastValues;
			for( int paramIndex = 0; paramIndex < newParamCount; paramIndex++ )
			{
				param = this_->aeffect->getParameter(this_->aeffect, paramIndex);
				if( param != params[paramIndex] )
				{
					params[paramIndex] = param;
					paramChanged = true;
				}
			}
		leaveVstCritical(this_);
	}

	// ... sth. changed?
	if( paramChanged )
	{
		// inform the user
		if( this_->callback )
			this_->callback(this_->vstHandle, BASS_VST_PARAM_CHANGED, oldParamCount, newParamCount, this_->callbackUserData);
	}
}

void cleanUpPlugins()
{
	sjhashElem *elem = sjhashFirst(&s_idleHash);
	elem = sjhashFirst(&s_unloadPendingInstances);

	while( elem )
	{
		HINSTANCE inst = (HINSTANCE)sjhashKeysize(elem);
		long      unloadCount = (long)sjhashData(elem);

		while( unloadCount > 0 )
		{
			FreeLibrary(inst);
			unloadCount--;
		}

		elem = sjhashNext(elem);
	}

}

void idleDo()
{	
	static bool s_inHere = false;
	if( !s_inHere )
	{
		s_inHere = true;
		EnterCriticalSection(&s_idleCritical);

			assert( _CrtCheckMemory() );
			BASS_VST_PLUGIN* this_;
			DWORD vstHandle;
			bool removeElem;
			sjhashElem *elem = sjhashFirst(&s_idleHash), *next;
			while( elem )
			{
				next = sjhashNext(elem);
				removeElem = false;

				vstHandle = (DWORD)sjhashKeysize(elem);
				this_ = refHandle(vstHandle);
				if( this_ )
				{
					if( this_->needsIdle & NEEDS_IDLE_OUTSIDE_EDIT )
					{
						// idle outside edit
						enterVstCritical(this_);
							if( this_->aeffect->dispatcher(this_->aeffect, __effIdleDeprecated, 0, 0, NULL, 0.0) == 0 )
								this_->needsIdle &= ~NEEDS_IDLE_OUTSIDE_EDIT;
						leaveVstCritical(this_);
					}

					if( this_->needsIdle & NEEDS_EDIT_IDLE )
					{
						// call the effect idle routine
						enterVstCritical(this_);
							this_->aeffect->dispatcher(this_->aeffect, effEditIdle, 0, 0, NULL, 0.0);
						leaveVstCritical(this_);

						checkForChangedParam(this_);
					}

					if( this_->needsIdle == 0 )
						removeElem = true;

					unrefHandle(vstHandle);
				}
				else
				{
					assert( _CrtCheckMemory() );
					removeElem = true;
				}

				// any more idle needed for this effect?
				if( removeElem )
				{
					sjhashInsert(&s_idleHash, NULL, /*pKey, not needed*/ (int)vstHandle, /*nKey (keySize)*/ 
						(void*)0/*pData - 0 = remove*/);
				}

				elem = next;
			}
			
			// unload pending instances
			if( sjhashCount(&s_unloadPendingInstances) )
			{
				s_unloadPendingCountdown--;
				if( s_unloadPendingCountdown <= 0 )
				{
					elem = sjhashFirst(&s_unloadPendingInstances);
					while( elem )
					{
						HINSTANCE inst = (HINSTANCE)sjhashKeysize(elem);
						long      unloadCount = (long)sjhashData(elem);

						while( unloadCount > 0 )
						{
							FreeLibrary(inst);
							unloadCount--;
						}

						elem = sjhashNext(elem);
					}
					sjhashClear(&s_unloadPendingInstances);
				}
			}

			// kill the timer?
			if( sjhashCount(&s_idleHash) == 0
			 && sjhashCount(&s_unloadPendingInstances) == 0 )
			{
				killIdleTimers();
			}			

		LeaveCriticalSection(&s_idleCritical);
		s_inHere = false;
	}
}



void updateIdleTimers(BASS_VST_PLUGIN* this_)
{
	EnterCriticalSection(&s_idleCritical);

		sjhashInsert(&s_idleHash, NULL, /*pKey, not needed*/ (int)this_->vstHandle, /*nKey*/ 
			(void*)this_->needsIdle/*pData - 0 = remove*/);

		if( sjhashCount(&s_idleHash) || sjhashCount(&s_unloadPendingInstances) )
		{
			// add the idle timer, if not yet done (this is checked in createIdleTimers())
			createIdleTimers();
		}
		else
		{
			// remove the idle timer, if it exists (this is checked in killIdleTimers())
			killIdleTimers();
		}

	LeaveCriticalSection(&s_idleCritical);
}



/*****************************************************************************
 *  low-level OS-based timers
 *****************************************************************************/



static VOID CALLBACK idleTimerProc(HWND,UINT,UINT,DWORD) // Falcosoft: VC6 requires UINT vs UINT_PTR
{
	idleDo();
}



void createIdleTimers()
{
	// called from updateIdleTimers() where the critical section is already allocated
	if( s_idleTimerHandle == 0 )
	{
		s_idleTimerHandle = (UINT)SetTimer(0, 0, IDLE_FREQ, idleTimerProc);
	}
}



void killIdleTimers()
{
	// called on shutdown - where checking the critical section does not make much sense -
	// and from updateIdleTimers() where the criticcal section is already allocated
	if( s_idleTimerHandle )
	{
		KillTimer(NULL, s_idleTimerHandle);
		s_idleTimerHandle = 0;
	}
}


