/***********************************************************************************\
*																					*
*	XRAM Example Code																*
*																					*
*	This application demonstrates how to query for the presence of X-RAM and		*
*	shows how to use it to assign storage space for each AL Buffer.					*
*																					*
\***********************************************************************************/

#include "Framework.h"

// Helper function
const ALchar *GetModeString(ALenum eBufferMode);

const ALchar szXRAMAuto[] = "AL_STORAGE_AUTOMATIC";
const ALchar szXRAMHardware[] = "AL_STORAGE_HARDWARE";
const ALchar szXRAMAccessible[] = "AL_STORAGE_ACCESSIBLE";
const ALchar szUnknown[] = "UNKNOWN";

int main()
{
	ALuint			uiBuffers[3] = {0};
	ALuint			uiSources[1] = {0};
	ALboolean		bXRAM;
	ALenum			eBufferMode;
	ALint			i, iState;
	ALint			iLoop;

	// Initialize Framework
	ALFWInit();

	ALFWprintf("X-RAM Demo Application\n");

	if (!ALFWInitOpenAL())
	{
		ALFWprintf("Failed to initialize OpenAL\n");
		ALFWShutdown();
		return 0;
	}

	bXRAM = ALFWIsXRAMSupported();
	if (bXRAM)
	{
		ALFWprintf("\nX-RAM Support found\n\n");

		ALFWprintf("Total X-RAM %dMB, Available X-RAM %dMB\n", alGetInteger(eXRAMSize) / (1024*1024),
			alGetInteger(eXRAMFree) / (1024*1024));
	}
	else
	{
		ALFWprintf("\nX-RAM Support NOT found\n\n");
	}

	// Generate 3 AL Buffers
	alGenBuffers(3, uiBuffers);


	// AUTOMATIC MODE

	// Automatic Mode is the default, so there is no need to do any XRAM calls before
	// loading the data into the Buffer.  If there is available XRAM the audio sample
	// will be uploaded to XRAM, otherwise it will stay in Host RAM.
	
	// Load Wave file into Buffer using default XRAM Mode
	if (ALFWLoadWaveToBuffer(ALFWaddMediaPath("wave1.wav"), uiBuffers[0], 0))
	{
		// Optional Query to find out the location of the Buffer
		if (bXRAM)
		{
			eBufferMode = eaxGetBufferMode(uiBuffers[0], NULL);
			ALFWprintf("Buffer mode is %s\n", GetModeString(eBufferMode));
		}
		else
		{
			ALFWprintf("Buffer loaded in Software\n");
		}
	}
	else
	{
		ALFWprintf( "Failed to load %s\n", ALFWaddMediaPath("wave1.wav") );
		alDeleteBuffers(1, &uiBuffers[0]);
		uiBuffers[0] = 0;
	}


	// HARDWARE MODE

	// Load Wave file into Buffer using HARDWARE XRAM Mode (The ALFWLoadWaveToBuffer function will set the
	// Buffer Mode BEFORE making the alBufferData call)
	if (ALFWLoadWaveToBuffer(ALFWaddMediaPath("wave2.wav"), uiBuffers[1], bXRAM ? eXRAMHardware : 0))
	{
		// Optional Query to find out the location of the Buffer
		if (bXRAM)
		{
			eBufferMode = eaxGetBufferMode(uiBuffers[1], NULL);
			ALFWprintf("Buffer mode is %s\n", GetModeString(eBufferMode));
		}
		else
		{
			ALFWprintf("Buffer loaded in Software\n");
		}
	}
	else
	{
		// Could have failed due to lack of XRAM resources ...
		ALFWprintf( "Failed to load %s (or insufficient XRAM resources)\n", ALFWaddMediaPath("wave2.wav") );
		alDeleteBuffers(1, &uiBuffers[1]);
		uiBuffers[1] = 0;
	}


	// ACCESSIBLE MODE

	// Load Wave file into Buffer using ACCESSIBLE XRAM Mode (The ALFWLoadWaveToBuffer function will set the
	// Buffer Mode BEFORE making the alBufferData call)

	// Use Accessible Mode for streaming buffers as it is more efficient to keep the data on the host
	// because the data will be constantly replaced when servicing the Source's Queue of Buffers 
	if (ALFWLoadWaveToBuffer(ALFWaddMediaPath("wave3.wav"), uiBuffers[2], bXRAM ? eXRAMAccessible : 0))
	{
		// Optional Query to find out the location of the Buffer
		if (bXRAM)
		{
			eBufferMode = eaxGetBufferMode(uiBuffers[2], NULL);
			ALFWprintf("Buffer mode is %s\n", GetModeString(eBufferMode));
		}
		else
		{
			ALFWprintf("Buffer loaded in Software\n");
		}
	}
	else
	{
		ALFWprintf( "Failed to load %s\n", ALFWaddMediaPath("wave3.wav") );
		alDeleteBuffers(1, &uiBuffers[2]);
		uiBuffers[2] = 0;
	}

	// Clear AL Error code
	alGetError();

	// Generate an AL Source
	alGenSources(1, uiSources);

	// Attach each Buffers in turn to the Source and Play them in the normal way !
	for (i = 0; i < 3; i++)
	{
		alSourceStop(uiSources[0]);
		alSourcei(uiSources[0], AL_BUFFER, uiBuffers[i]);
		if (alGetError() == AL_NO_ERROR)
		{
			alSourcePlay(uiSources[0]);

			do
			{
				Sleep(1);
				alGetSourcei(uiSources[0], AL_SOURCE_STATE, &iState);
			} while (iState == AL_PLAYING);
		}
	}

	alSourceStop(uiSources[0]);
	alDeleteSources(1, uiSources);
	for (iLoop = 0; iLoop < 3; iLoop++)
	{
		if (uiBuffers[iLoop])
			alDeleteBuffers(1, &uiBuffers[iLoop]);
	}

	ALFWShutdownOpenAL();

	ALFWShutdown();

	return 0;
}


/*
	Returns string names for the XRAM enums
*/
const ALchar *GetModeString(ALenum eBufferMode)
{
	if (eBufferMode == eXRAMAuto)
		return szXRAMAuto;

	if (eBufferMode == eXRAMHardware)
		return szXRAMHardware;

	if (eBufferMode == eXRAMAccessible)
		return szXRAMAccessible;

	return szUnknown;
}

