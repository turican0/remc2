#include <al.h>
#include <alc.h>
#include <efx.h>
#include <efx-creative.h>
#include "ALOutput.h"
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <windows.h>

#define NUM_STREAMING_BUFFERS		4
#define MAX_NUM_EFFECT_SLOTS		4

void OutputDebugMessage(const char *szOutput,...)
{
#ifdef _DEBUG
	static char szString[1024];
	va_list args;
	va_start(args, szOutput);
	vsprintf_s(szString, 1024, szOutput, args);
	va_end(args);
	OutputDebugString(szString);
#endif
}

ALCdevice *g_pDevice = 0;
ALCcontext *g_pContext = 0;
ALuint g_uiSources[MAX_NUM_EFFECT_SLOTS + 1] = { 0 };
ALuint g_uiBuffers[(MAX_NUM_EFFECT_SLOTS + 1) * NUM_STREAMING_BUFFERS] = { 0 };
ALenum g_eFormat = 0;
ALuint g_uiOutputChannels = 0;
ALuint g_uiNumSamplesPerBuffer = 0;
ALuint g_uiFrequency = 0;
ALuint g_uiPlayCounter = 0;
ALuint g_uiMuted;
ALchar *g_pData = 0;

// Effect Extension Stuff
LPALGENEFFECTS alGenEffects = 0;
LPALDELETEEFFECTS alDeleteEffects = 0;
LPALISEFFECT alIsEffect = 0;
LPALEFFECTI alEffecti = 0;
LPALEFFECTF alEffectf = 0;
LPALEFFECTFV alEffectfv = 0;

LPALGENFILTERS alGenFilters = 0;
LPALDELETEFILTERS alDeleteFilters = 0;
LPALISFILTER alIsFilter = 0;
LPALFILTERI alFilteri = 0;
LPALFILTERF alFilterf = 0;

LPALGENAUXILIARYEFFECTSLOTS alGenAuxiliaryEffectSlots = 0;
LPALDELETEAUXILIARYEFFECTSLOTS alDeleteAuxiliaryEffectSlots = 0;
LPALISAUXILIARYEFFECTSLOT alIsAuxiliaryEffectSlot = 0;
LPALAUXILIARYEFFECTSLOTI alAuxiliaryEffectSloti = 0;

ALuint g_uiNumEffectSlots = 0;
ALuint g_uiEffects[MAX_NUM_EFFECT_SLOTS] = { 0 };
ALuint g_uiEffectSlots[MAX_NUM_EFFECT_SLOTS] = { 0 };

ALboolean g_bALInit = AL_FALSE;

void InitEFX(unsigned long ulNumEffects);
void ShutdownEFX();

bool InitializeOpenAL(char *szALDevice, unsigned long ulNumOutputChannels, unsigned long ulNumEffects,
					  unsigned long ulNumSamples, unsigned long ulFrequency)
{
	ALboolean bEFX = AL_FALSE;
	bool bReturn = false;
	char *szDeviceName = 0;

	// Open default OpenAL device
	g_pDevice = alcOpenDevice(szALDevice);
	if (g_pDevice)
	{
		OutputDebugMessage("ALOutput : Using OpenAL Device '%s'\n", alcGetString(g_pDevice, ALC_DEVICE_SPECIFIER));

		// Check for EFX Extension if Effects are being requested
		if (ulNumEffects)
			bEFX = alcIsExtensionPresent(g_pDevice, ALC_EXT_EFX_NAME);

		// Create a Context
		g_pContext = alcCreateContext(g_pDevice, 0);
		if (g_pContext)
		{
			// Make the Context Current (active)
			alcMakeContextCurrent(g_pContext);
			if (alcGetError(g_pDevice) == ALC_NO_ERROR)
			{
				// Get AL Format Tag for the dry-mix output
				switch (ulNumOutputChannels)
				{
				case 1:
					g_eFormat = AL_FORMAT_MONO16;
					g_uiOutputChannels = 1;
					break;
				case 2:
					g_eFormat = AL_FORMAT_STEREO16;
					g_uiOutputChannels = 2;
					break;
				case 4:
					g_eFormat = alGetEnumValue("AL_FORMAT_QUAD16");
					g_uiOutputChannels = 4;
					break;
				case 6:
					g_eFormat = alGetEnumValue("AL_FORMAT_51CHN16");
					g_uiOutputChannels = 6;
					break;
				case 8:
					g_eFormat = alGetEnumValue("AL_FORMAT_71CHN16");
					g_uiOutputChannels = 8;
					break;
				default:
					break;
				}

				// Default to Stereo if we didn't get a valid enum
				if (g_eFormat == 0)
				{
					g_eFormat = AL_FORMAT_STEREO16;
					g_uiOutputChannels = 2;
				}

				g_uiNumSamplesPerBuffer = ulNumSamples;
				g_uiFrequency = ulFrequency;
				// Allocate memory to store one Block of audio data
				g_pData = (ALchar*)malloc(g_uiNumSamplesPerBuffer * g_uiOutputChannels * 2);
				if (g_pData)
				{
					// Generate a Source and some Buffers for the dry-mix output
					alGetError();
					alGenSources(1, &g_uiSources[0]);
					alGenBuffers(NUM_STREAMING_BUFFERS, &g_uiBuffers[0]);
					if (alGetError() == AL_NO_ERROR)
					{
						OutputDebugMessage("ALOutput : OpenAL successfully initialized\n");
						g_bALInit = AL_TRUE;

						// Initialize EFX if requested
						if (bEFX)
							InitEFX(ulNumEffects);

						bReturn = true;
					}
					else
						OutputDebugMessage("ALOutput : Failed to generate Source and / or Buffers\n");
				}
				else
					OutputDebugMessage("ALOutput : Out of memory\n");
			}
			else
				OutputDebugMessage("ALOutput : Failed to make OpenAL context current\n");
		}
		else
			OutputDebugMessage("ALOutput : Failed to create OpenAL context\n");
	}
	else
		OutputDebugMessage("ALOutput : Failed to open default OpenAL device\n");

	if (g_bALInit == AL_FALSE)
		ShutdownOpenAL();

	return bReturn;
}

bool ShutdownOpenAL()
{
	ALuint uiLoop;

	if (g_pContext)
	{
		// Stop and delete Sources and Buffers
		if (alIsSource(g_uiSources[0]))
		{
			alSourceStop(g_uiSources[0]);
			alSourcei(g_uiSources[0], AL_BUFFER, 0);
			alDeleteSources(1, &g_uiSources[0]);
			g_uiSources[0] = 0;
		}

		for (uiLoop = 0; uiLoop < NUM_STREAMING_BUFFERS; uiLoop++)
		{
			if (alIsBuffer(g_uiBuffers[uiLoop]))
			{
				alDeleteBuffers(1, &g_uiBuffers[uiLoop]);
				g_uiBuffers[uiLoop] = 0;
			}
		}

		ShutdownEFX();

		alcMakeContextCurrent(0);
		alcDestroyContext(g_pContext);
		g_pContext = 0;
	}

	// Close the AL device
	if (g_pDevice)
	{
		alcCloseDevice(g_pDevice);
		g_pDevice = 0;
	}

	// Release memory
	if (g_pData)
	{
		free(g_pData);
		g_pData = 0;
	}

	g_eFormat = 0;
	g_uiOutputChannels = 0;
	g_uiNumSamplesPerBuffer = 0;
	g_uiFrequency = 0;
	g_uiPlayCounter = 0;
	g_bALInit = AL_FALSE;

	return true;
}

unsigned long GetNumOutputChannels()
{
	return g_uiOutputChannels;
}

unsigned long GetNumEffects()
{
	return g_uiNumEffectSlots;
}

unsigned long GetNumSamplesPerBuffer()
{
	return g_uiNumSamplesPerBuffer;
}

bool Start()
{
	ALuint uiLoop, uiInnerLoop;
	bool bReturn = false;

	if (g_bALInit)
	{
		alGetError();

		// We are going to fill all the streaming Buffers of all the Sources
		// with silence to kick things off
		memset(g_pData, 0, g_uiNumSamplesPerBuffer * 2 * g_uiOutputChannels);

		// Fill direct path Buffers with silence
		for (uiLoop = 0; uiLoop < NUM_STREAMING_BUFFERS; uiLoop++)
		{
			alBufferData(g_uiBuffers[uiLoop], g_eFormat, g_pData, g_uiNumSamplesPerBuffer * 2 * g_uiOutputChannels, g_uiFrequency);
			alSourceQueueBuffers(g_uiSources[0], 1, &g_uiBuffers[uiLoop]);
		}

		// Fill Effect Buffers with silence
		for (uiLoop = 1; uiLoop < (g_uiNumEffectSlots + 1); uiLoop++)
		{
			for (uiInnerLoop = 0; uiInnerLoop < NUM_STREAMING_BUFFERS; uiInnerLoop++)
			{
				alBufferData(g_uiBuffers[(uiLoop * NUM_STREAMING_BUFFERS) + uiInnerLoop], AL_FORMAT_MONO16, g_pData, g_uiNumSamplesPerBuffer * 2, g_uiFrequency);
				alSourceQueueBuffers(g_uiSources[uiLoop], 1, &g_uiBuffers[(uiLoop * NUM_STREAMING_BUFFERS) + uiInnerLoop]);
			}
		}

		// Vector Play call
		alSourcePlayv(g_uiNumEffectSlots + 1, g_uiSources);

		if (alGetError() == AL_NO_ERROR)
			bReturn = true;
	}

	return bReturn;
}

bool Stop()
{
	ALuint uiLoop;
	bool bReturn = false;

	if (g_bALInit)
	{
		alGetError();

		// Stop Sources
		alSourceStopv(g_uiNumEffectSlots + 1, g_uiSources);

		// Clear the queues
		for (uiLoop = 0; uiLoop < (g_uiNumEffectSlots + 1); uiLoop++)
			alSourcei(g_uiSources[uiLoop], AL_BUFFER, 0);

		if (alGetError() == AL_NO_ERROR)
			bReturn = true;
	}

	return bReturn;
}

unsigned long GetBuffersProcessed()
{
	ALint iProcessed;
	ALuint uiMinProcessed = -1;
	ALuint uiLoop;
	ALint iState;

	alGetError();

	// Keep things in sync. by returning the minimum number of
	// Buffers processed on each of the playing Sources
	for (uiLoop = 0; uiLoop < (g_uiNumEffectSlots + 1); uiLoop++)
	{
		alGetSourcei(g_uiSources[uiLoop], AL_BUFFERS_PROCESSED, &iProcessed);
		uiMinProcessed = __min(uiMinProcessed, (ALuint)iProcessed);
		if (uiMinProcessed == 0)
			break;
	}

	// Check if all the Sources are still Playing
	for (uiLoop = 0; uiLoop < (g_uiNumEffectSlots + 1); uiLoop++)
	{
		alGetSourcei(g_uiSources[uiLoop], AL_SOURCE_STATE, &iState);
		if (iState != AL_PLAYING)
			break;
	}

	// If any of them have stopped, we need to stop them all
	if (iState != AL_PLAYING)
	{
		alSourceStopv(g_uiNumEffectSlots + 1, g_uiSources);
		// It is possible that a buffer was queued *after* the Source stopped,
		// so we need to check how many Buffers have been processed
		for (uiLoop = 0; uiLoop < (g_uiNumEffectSlots + 1); uiLoop++)
		{
			alGetSourcei(g_uiSources[uiLoop], AL_BUFFERS_PROCESSED, &iProcessed);
			uiMinProcessed = __min(uiMinProcessed, (ALuint)iProcessed);
		}
		g_uiPlayCounter = uiMinProcessed;
		OutputDebugMessage("ALOutput : One or more Sources were starved, stopping all Sources (%d Buffers Processed)\n", uiMinProcessed);
	}
	
	if (alGetError() != AL_NO_ERROR)
		OutputDebugMessage("ALOutput : GetBuffersProcessed set an AL error\n");

	return uiMinProcessed;
}

void CommitData(void *pOutput, void **pEffectOutputs)
{
	ALuint uiLoop;
	ALuint uiBufferID;
	void *pData = 0;

	for (uiLoop = 0; uiLoop < (g_uiNumEffectSlots + 1); uiLoop++)
	{
		alGetError();
		alSourceUnqueueBuffers(g_uiSources[uiLoop], 1, &uiBufferID);
		if (alGetError() == AL_NO_ERROR)
		{
			if (uiLoop == 0) // Dry Mix
			{
				pData = pOutput ? pOutput : g_pData;
				alBufferData(uiBufferID, g_eFormat, pData, g_uiNumSamplesPerBuffer * 2 * g_uiOutputChannels, g_uiFrequency);
			}
			else
			{
				pData = pEffectOutputs[uiLoop - 1] ? pEffectOutputs[uiLoop - 1] : g_pData;
				alBufferData(uiBufferID, AL_FORMAT_MONO16, pData, g_uiNumSamplesPerBuffer * 2, g_uiFrequency);
			}

			if (alGetError() == AL_NO_ERROR)
				alSourceQueueBuffers(g_uiSources[uiLoop], 1, &uiBufferID);
			else
				OutputDebugMessage("ALOutput : Failed alBufferData\n");
		}
		else
			OutputDebugMessage("ALOutput : Failed unSourceUnQueue\n");
	}

	// Do we need to re-start the Sources?
	if (g_uiPlayCounter)
	{
		g_uiPlayCounter--;
		if (g_uiPlayCounter == 0)
		{
			alSourcePlayv(g_uiNumEffectSlots + 1, g_uiSources);
			OutputDebugMessage("ALOutput : Re-starting Sources\n");
		}
	}
}

void InitEFX(unsigned long ulNumEffects)
{
	ALuint uiLoop;

	// Clamp number of Effect Slots
	if (ulNumEffects > MAX_NUM_EFFECT_SLOTS)
		ulNumEffects = MAX_NUM_EFFECT_SLOTS;

	// Get pointers to the EFX Extension functions we need
	alGenAuxiliaryEffectSlots = (LPALGENAUXILIARYEFFECTSLOTS)alGetProcAddress("alGenAuxiliaryEffectSlots");
	alDeleteAuxiliaryEffectSlots = (LPALDELETEAUXILIARYEFFECTSLOTS)alGetProcAddress("alDeleteAuxiliaryEffectSlots");
	alIsAuxiliaryEffectSlot = (LPALISAUXILIARYEFFECTSLOT)alGetProcAddress("alIsAuxiliaryEffectSlot");
	alAuxiliaryEffectSloti = (LPALAUXILIARYEFFECTSLOTI)alGetProcAddress("alAuxiliaryEffectSloti");

	alGenEffects = (LPALGENEFFECTS)alGetProcAddress("alGenEffects");
	alDeleteEffects = (LPALDELETEEFFECTS)alGetProcAddress("alDeleteEffects");
	alIsEffect = (LPALISEFFECT)alGetProcAddress("alIsEffect");
	alEffecti = (LPALEFFECTI)alGetProcAddress("alEffecti");
	alEffectf = (LPALEFFECTF)alGetProcAddress("alEffectf");
	alEffectfv = (LPALEFFECTFV)alGetProcAddress("alEffectfv");

	alGenFilters = (LPALGENFILTERS)alGetProcAddress("alGenFilters");
	alDeleteFilters = (LPALDELETEFILTERS)alGetProcAddress("alDeleteFilters");
	alIsFilter = (LPALISFILTER)alGetProcAddress("alIsFilter");
	alFilteri = (LPALFILTERI)alGetProcAddress("alFilteri");
	alFilterf = (LPALFILTERF)alGetProcAddress("alFilterf");

	// Default Reverb Preset (muted)
	EFXEAXREVERBPROPERTIES EFXEAXReverb;

	EFXEAXReverb.flDensity = 1.0f;
	EFXEAXReverb.flDiffusion = 1.0f;
	EFXEAXReverb.flGain = 0.0f;
	EFXEAXReverb.flGainHF = 0.89f;
	EFXEAXReverb.flGainLF = 1.0;
	EFXEAXReverb.flDecayTime = 1.49f;
	EFXEAXReverb.flDecayHFRatio = 0.83f;
	EFXEAXReverb.flDecayLFRatio = 1.0f;
	EFXEAXReverb.flReflectionsGain = 0.05f;
	EFXEAXReverb.flReflectionsDelay = 0.007f;
	EFXEAXReverb.flReflectionsPan[0] = 0.0f;
	EFXEAXReverb.flReflectionsPan[1] = 0.0f;
	EFXEAXReverb.flReflectionsPan[2] = 0.0f;
	EFXEAXReverb.flLateReverbGain = 1.26f;
	EFXEAXReverb.flLateReverbDelay = 0.011f;
	EFXEAXReverb.flLateReverbPan[0] = 0.0f;
	EFXEAXReverb.flLateReverbPan[1] = 0.0f;
	EFXEAXReverb.flLateReverbPan[2] = 0.0f;
	EFXEAXReverb.flEchoTime = 0.25f;
	EFXEAXReverb.flEchoDepth = 0.0f;
	EFXEAXReverb.flModulationTime = 0.25f;
	EFXEAXReverb.flModulationDepth = 0.0f;
	EFXEAXReverb.flAirAbsorptionGainHF = 0.994f;
	EFXEAXReverb.flHFReference = 5000.0f;
	EFXEAXReverb.flLFReference = 250.0f;
	EFXEAXReverb.flRoomRolloffFactor = 0.0f;
	EFXEAXReverb.iDecayHFLimit = 1;

	if (alGenAuxiliaryEffectSlots && alDeleteAuxiliaryEffectSlots &&
		alIsAuxiliaryEffectSlot && alAuxiliaryEffectSloti &&
		alGenEffects && alDeleteEffects && alIsEffect && alEffecti && alEffectf && alEffectfv &&
		alGenFilters && alDeleteFilters && alIsFilter && alFilteri && alFilterf)
	{
		// Generate Effect Slots and Effects
		for (uiLoop = 0; uiLoop < ulNumEffects; uiLoop++)
		{
			alGenAuxiliaryEffectSlots(1, &g_uiEffectSlots[uiLoop]);
			if (alGetError() != AL_NO_ERROR)
				break;

			alGenEffects(1, &g_uiEffects[uiLoop]);
			if (alGetError() != AL_NO_ERROR)
				break;

			alEffecti(g_uiEffects[uiLoop], AL_EFFECT_TYPE, AL_EFFECT_EAXREVERB);
			if (alGetError() != AL_NO_ERROR)
				break;

			g_uiNumEffectSlots++;
		}

		if (g_uiNumEffectSlots)
		{
			// Generate a Filter to be used to mute the dry-path on the wet-mix Sources
			alGetError();
			alGenFilters(1, &g_uiMuted);
			alFilteri(g_uiMuted, AL_FILTER_TYPE, AL_FILTER_LOWPASS);
			alFilterf(g_uiMuted, AL_LOWPASS_GAIN, 0.0f);
			alFilterf(g_uiMuted, AL_LOWPASS_GAINHF, 0.0f);
			if (alGetError() == AL_NO_ERROR)
			{
				// Generate Source(s) and Buffers for the wet-mix(es) output
				for (uiLoop = 0; uiLoop < g_uiNumEffectSlots; uiLoop++)
				{
					alGetError();
					alGenSources(1, &g_uiSources[uiLoop + 1]);
					// Mute dry-path
					alSourcei(g_uiSources[uiLoop + 1], AL_DIRECT_FILTER, g_uiMuted);
					// Configure Effect Send
					alSource3i(g_uiSources[uiLoop + 1], AL_AUXILIARY_SEND_FILTER, g_uiEffectSlots[uiLoop], 0, AL_FILTER_NULL);
					// Generate Buffers for streaming
					alGenBuffers(NUM_STREAMING_BUFFERS, &g_uiBuffers[(uiLoop + 1) * NUM_STREAMING_BUFFERS]);
					if (alGetError() != AL_NO_ERROR)
						break;
					// Apply muted Generic Reverb preset to the Effect Slot
					SetEFXEAXReverbProperties(&EFXEAXReverb, uiLoop);
				}

				if (uiLoop == g_uiNumEffectSlots)
				{
					OutputDebugMessage("ALOutput : EFX successfully initialized\n");
				}
				else
				{
					OutputDebugMessage("ALOutput : EFX Unavailable\n");
					ShutdownEFX();
				}
			}
			else
			{
				OutputDebugMessage("ALOutput : EFX Support not found (failed to create filter)\n");
				ShutdownEFX();
			}
		}
		else
		{
			OutputDebugMessage("ALOutput : EFX Support not found (failed to create objects)\n");
			ShutdownEFX();
		}
	}
	else
		OutputDebugMessage("ALOutput : EFX Support not found (missing function pointers)\n");
}

void ShutdownEFX()
{
	ALuint uiLoop, uiInnerLoop;

	// Stop and delete Effect Sources and Buffers
	for (uiLoop = 1; uiLoop < (g_uiNumEffectSlots + 1); uiLoop++)
	{
		if (alIsSource(g_uiSources[uiLoop]))
		{
			alSourceStop(g_uiSources[uiLoop]);
			alSourcei(g_uiSources[uiLoop], AL_BUFFER, 0);
			alDeleteSources(1, &g_uiSources[uiLoop]);
			g_uiSources[uiLoop] = 0;
		}

		for (uiInnerLoop = 0; uiInnerLoop < NUM_STREAMING_BUFFERS; uiInnerLoop++)
		{
			if (alIsBuffer(g_uiBuffers[(uiLoop*NUM_STREAMING_BUFFERS)+uiInnerLoop]))
			{
				alDeleteBuffers(1, &g_uiBuffers[(uiLoop*NUM_STREAMING_BUFFERS)+uiInnerLoop]);
				g_uiBuffers[(uiLoop*NUM_STREAMING_BUFFERS)+uiInnerLoop] = 0;
			}
		}
	}

	// Delete any generated Aux Effect Slots and Effects
	if (alIsAuxiliaryEffectSlot && alAuxiliaryEffectSloti && alDeleteAuxiliaryEffectSlots &&
		alIsEffect && alEffecti && alDeleteEffects)
	{
		for (uiLoop = 0; uiLoop < g_uiNumEffectSlots; uiLoop++)
		{
			if (alIsAuxiliaryEffectSlot(g_uiEffectSlots[uiLoop]))
			{
				// Unload Reverb Effect
				alAuxiliaryEffectSloti(g_uiEffectSlots[uiLoop], AL_EFFECTSLOT_EFFECT, AL_EFFECT_NULL);
				
				// Delete Auxiliary Effect Slot
				alDeleteAuxiliaryEffectSlots(1, &g_uiEffectSlots[uiLoop]);
				g_uiEffectSlots[uiLoop] = 0;

				// Delete Effect
				if (alIsEffect(g_uiEffects[uiLoop]))
				{
					alEffecti(g_uiEffects[uiLoop], AL_EFFECT_TYPE, AL_EFFECT_NULL);
					alDeleteEffects(1, &g_uiEffects[uiLoop]);
					g_uiEffects[uiLoop] = 0;
				}
			}
		}
	}

	// Delete filter
	if (alIsFilter && alFilteri && alDeleteFilters)
	{
		if (alIsFilter(g_uiMuted))
		{
			alFilteri(g_uiMuted, AL_FILTER_TYPE, AL_FILTER_NULL);
			alDeleteFilters(1, &g_uiMuted);
			g_uiMuted = 0;
		}
	}

	g_uiNumEffectSlots = 0;
}

bool SetEFXEAXReverbProperties(EFXEAXREVERBPROPERTIES *pEFXEAXReverb, unsigned long ulEffectNumber)
{
	bool bReturn = false;

	if ((pEFXEAXReverb) && (ulEffectNumber < g_uiNumEffectSlots))
	{
		// Clear AL Error code
		alGetError();

		alEffectf(g_uiEffects[ulEffectNumber], AL_EAXREVERB_DENSITY, pEFXEAXReverb->flDensity);
		alEffectf(g_uiEffects[ulEffectNumber], AL_EAXREVERB_DIFFUSION, pEFXEAXReverb->flDiffusion);
		alEffectf(g_uiEffects[ulEffectNumber], AL_EAXREVERB_GAIN, pEFXEAXReverb->flGain);
		alEffectf(g_uiEffects[ulEffectNumber], AL_EAXREVERB_GAINHF, pEFXEAXReverb->flGainHF);
		alEffectf(g_uiEffects[ulEffectNumber], AL_EAXREVERB_GAINLF, pEFXEAXReverb->flGainLF);
		alEffectf(g_uiEffects[ulEffectNumber], AL_EAXREVERB_DECAY_TIME, pEFXEAXReverb->flDecayTime);
		alEffectf(g_uiEffects[ulEffectNumber], AL_EAXREVERB_DECAY_HFRATIO, pEFXEAXReverb->flDecayHFRatio);
		alEffectf(g_uiEffects[ulEffectNumber], AL_EAXREVERB_DECAY_LFRATIO, pEFXEAXReverb->flDecayLFRatio);
		alEffectf(g_uiEffects[ulEffectNumber], AL_EAXREVERB_REFLECTIONS_GAIN, pEFXEAXReverb->flReflectionsGain);
		alEffectf(g_uiEffects[ulEffectNumber], AL_EAXREVERB_REFLECTIONS_DELAY, pEFXEAXReverb->flReflectionsDelay);
		alEffectfv(g_uiEffects[ulEffectNumber], AL_EAXREVERB_REFLECTIONS_PAN, pEFXEAXReverb->flReflectionsPan);
		alEffectf(g_uiEffects[ulEffectNumber], AL_EAXREVERB_LATE_REVERB_GAIN, pEFXEAXReverb->flLateReverbGain);
		alEffectf(g_uiEffects[ulEffectNumber], AL_EAXREVERB_LATE_REVERB_DELAY, pEFXEAXReverb->flLateReverbDelay);
		alEffectfv(g_uiEffects[ulEffectNumber], AL_EAXREVERB_LATE_REVERB_PAN, pEFXEAXReverb->flLateReverbPan);
		alEffectf(g_uiEffects[ulEffectNumber], AL_EAXREVERB_ECHO_TIME, pEFXEAXReverb->flEchoTime);
		alEffectf(g_uiEffects[ulEffectNumber], AL_EAXREVERB_ECHO_DEPTH, pEFXEAXReverb->flEchoDepth);
		alEffectf(g_uiEffects[ulEffectNumber], AL_EAXREVERB_MODULATION_TIME, pEFXEAXReverb->flModulationTime);
		alEffectf(g_uiEffects[ulEffectNumber], AL_EAXREVERB_MODULATION_DEPTH, pEFXEAXReverb->flModulationDepth);
		alEffectf(g_uiEffects[ulEffectNumber], AL_EAXREVERB_AIR_ABSORPTION_GAINHF, pEFXEAXReverb->flAirAbsorptionGainHF);
		alEffectf(g_uiEffects[ulEffectNumber], AL_EAXREVERB_HFREFERENCE, pEFXEAXReverb->flHFReference);
		alEffectf(g_uiEffects[ulEffectNumber], AL_EAXREVERB_LFREFERENCE, pEFXEAXReverb->flLFReference);
		alEffectf(g_uiEffects[ulEffectNumber], AL_EAXREVERB_ROOM_ROLLOFF_FACTOR, pEFXEAXReverb->flRoomRolloffFactor);
		alEffecti(g_uiEffects[ulEffectNumber], AL_EAXREVERB_DECAY_HFLIMIT, pEFXEAXReverb->iDecayHFLimit);

		// Apply to Effect Slot
		alAuxiliaryEffectSloti(g_uiEffectSlots[ulEffectNumber], AL_EFFECTSLOT_EFFECT, g_uiEffects[ulEffectNumber]);

		if (alGetError() == AL_NO_ERROR)
		{
			OutputDebugMessage("ALOutput : Set EAX Reverb Preset successfully\n");
			bReturn = true;
		}
		else
			OutputDebugMessage("ALOutput : Failed to set EAX Reverb Preset\n");
	}

	return bReturn;
}