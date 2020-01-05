/*
 * Copyright (c) 2006, Creative Labs Inc.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided
 * that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright notice, this list of conditions and
 * 	     the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions
 * 	     and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *     * Neither the name of Creative Labs Inc. nor the names of its contributors may be used to endorse or
 * 	     promote products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "Framework.h"
#include "EFX-Util.h"
#include <math.h>

#define	TEST_WAVE_FILE		"Footsteps.wav"

ALboolean CreateAuxEffectSlot(ALuint *puiAuxEffectSlot);
ALboolean CreateEffect(ALuint *puiEffect, ALenum eEffectType);
ALvoid PlaySource(ALuint uiSource);
ALboolean SetEFXEAXReverbProperties(EFXEAXREVERBPROPERTIES *pEFXEAXReverb, ALuint uiEffect);

EAXREVERBPROPERTIES eaxBathroom = REVERB_PRESET_BATHROOM;
EAXREVERBPROPERTIES eaxHangar = REVERB_PRESET_HANGAR;

int main()
{
	EFXEAXREVERBPROPERTIES efxReverb;
	ALuint		uiEffectSlot, uiEffect;
	ALuint		uiSource, uiBuffer;
	ALboolean	bEffectCreated = AL_FALSE;

	// Initialize Framework
	ALFWInit();

	ALFWprintf("EFX Reverb Application\n");

	if (!ALFWInitOpenAL())
	{
		ALFWprintf("Failed to initialize OpenAL\n");
		ALFWShutdown();
		return 0;
	}

	// Clear AL Error State
	alGetError();

	// Generate a Buffer
	alGenBuffers(1, &uiBuffer);

	// Load Wave file into OpenAL Buffer
	if (!ALFWLoadWaveToBuffer((char*)ALFWaddMediaPath(TEST_WAVE_FILE), uiBuffer))
	{
		ALFWprintf("Failed to load %s\n", ALFWaddMediaPath(TEST_WAVE_FILE));
		alDeleteBuffers(1, &uiBuffer);
		ALFWShutdownOpenAL();
		ALFWShutdown();
		return 0;
	}

	// Generate a Source
	alGenSources(1, &uiSource);

	// Attach Buffer to Source
	alSourcei(uiSource, AL_BUFFER, uiBuffer);

	// Check for EFX Extension
	if (ALFWIsEFXSupported())
	{
		// The EFX Extension includes support for global effects, such as Reverb.  To use a global effect,
		// you need to create an Auxiliary Effect Slot to store the Effect ...
		if (CreateAuxEffectSlot(&uiEffectSlot))
		{
			// Once we have an Auxiliary Effect Slot, we can generate an Effect Object, set its Type
			// and Parameter Values, and then load the Effect into the Auxiliary Effect Slot ...
			if (CreateEffect(&uiEffect, AL_EFFECT_EAXREVERB))
			{
				bEffectCreated = AL_TRUE;
			}
			else
			{
				ALFWprintf("Failed to Create an EAX Reverb Effect\n");
			}
		}
		else
		{
			ALFWprintf("Failed to generate an Auxilary Effect Slot\n");
		}

		ALFWprintf("Playing Source without Effects\n");
		PlaySource(uiSource);

		if (bEffectCreated)
		{
			ALFWprintf("Playing Source with Send to 'Hangar' Reverb Effect\n");
			
			// Reverb Preset is stored in legacy format, use helper function to convert to EFX EAX Reverb
			ConvertReverbParameters(&eaxHangar, &efxReverb);
				
			// Set the Effect parameters
			if (!SetEFXEAXReverbProperties(&efxReverb, uiEffect))
				ALFWprintf("Failed to set Reverb Parameters\n");

			// Load Effect into Auxiliary Effect Slot
			alAuxiliaryEffectSloti(uiEffectSlot, AL_EFFECTSLOT_EFFECT, uiEffect);

			// Enable (non-filtered) Send from Source to Auxiliary Effect Slot
			alSource3i(uiSource, AL_AUXILIARY_SEND_FILTER, uiEffectSlot, 0, AL_FILTER_NULL);

			// Play Source again
			PlaySource(uiSource);
			
			ALFWprintf("Playing Source with Send to 'Bathroom' Reverb Effect\n");

			// To change one (or more) of the Reverb parameters, update the Effect Object
			// and re-attach to the Auxiliary Effect Slot ...

			// Reverb Preset is stored in legacy format, use helper function to convert to EFX EAX Reverb
			ConvertReverbParameters(&eaxBathroom, &efxReverb);

			// Set the Effect parameters
			if (!SetEFXEAXReverbProperties(&efxReverb, uiEffect))
				ALFWprintf("Failed to set Reverb Parameters\n");

			// Load Effect into Auxiliary Effect Slot
			alAuxiliaryEffectSloti(uiEffectSlot, AL_EFFECTSLOT_EFFECT, uiEffect);

			// Play Source again
			PlaySource(uiSource);

			// Clean-up ...

			// Remove Effect Send from Source
			alSource3i(uiSource, AL_AUXILIARY_SEND_FILTER, AL_EFFECTSLOT_NULL, 0, AL_FILTER_NULL);

			// Load NULL Effect into Effect Slot
			alAuxiliaryEffectSloti(uiEffectSlot, AL_EFFECTSLOT_EFFECT, AL_EFFECT_NULL);
			
			// Delete Effect
			alDeleteEffects(1, &uiEffect);
			
			// Delete Auxiliary Effect Slot
			alDeleteAuxiliaryEffectSlots(1, &uiEffectSlot);
		}

		// Delete Source and Buffer
		alSourceStop(uiSource);
		alDeleteSources(1, &uiSource);
		alDeleteBuffers(1, &uiBuffer);
	}
	else
	{
		ALFWprintf("AL Device does not support EFX\n");
	}

	ALFWShutdownOpenAL();

	ALFWShutdown();

    return 0;
}

ALboolean CreateAuxEffectSlot(ALuint *puiAuxEffectSlot)
{
	ALboolean bReturn = AL_FALSE;

	// Clear AL Error state
	alGetError();

	// Generate an Auxiliary Effect Slot
	alGenAuxiliaryEffectSlots(1, puiAuxEffectSlot);
	if (alGetError() == AL_NO_ERROR)
		bReturn = AL_TRUE;

	return bReturn;
}

ALboolean CreateEffect(ALuint *puiEffect, ALenum eEffectType)
{
	ALboolean bReturn = AL_FALSE;

	if (puiEffect)
	{
		// Clear AL Error State
		alGetError();

		// Generate an Effect
		alGenEffects(1, puiEffect);
		if (alGetError() == AL_NO_ERROR)
		{
			// Set the Effect Type
			alEffecti(*puiEffect, AL_EFFECT_TYPE, eEffectType);
			if (alGetError() == AL_NO_ERROR)
				bReturn = AL_TRUE;
			else
				alDeleteEffects(1, puiEffect);
		}
	}

	return bReturn;
}

ALvoid PlaySource(ALuint uiSource)
{
	ALint iState;

	// Play Source
	alSourcePlay(uiSource);
	do
	{
		alGetSourcei(uiSource, AL_SOURCE_STATE, &iState);
		Sleep(10);
	} while (iState == AL_PLAYING);
}

ALboolean SetEFXEAXReverbProperties(EFXEAXREVERBPROPERTIES *pEFXEAXReverb, ALuint uiEffect)
{
	ALboolean bReturn = AL_FALSE;

	if (pEFXEAXReverb)
	{
		// Clear AL Error code
		alGetError();

		alEffectf(uiEffect, AL_EAXREVERB_DENSITY, pEFXEAXReverb->flDensity);
		alEffectf(uiEffect, AL_EAXREVERB_DIFFUSION, pEFXEAXReverb->flDiffusion);
		alEffectf(uiEffect, AL_EAXREVERB_GAIN, pEFXEAXReverb->flGain);
		alEffectf(uiEffect, AL_EAXREVERB_GAINHF, pEFXEAXReverb->flGainHF);
		alEffectf(uiEffect, AL_EAXREVERB_GAINLF, pEFXEAXReverb->flGainLF);
		alEffectf(uiEffect, AL_EAXREVERB_DECAY_TIME, pEFXEAXReverb->flDecayTime);
		alEffectf(uiEffect, AL_EAXREVERB_DECAY_HFRATIO, pEFXEAXReverb->flDecayHFRatio);
		alEffectf(uiEffect, AL_EAXREVERB_DECAY_LFRATIO, pEFXEAXReverb->flDecayLFRatio);
		alEffectf(uiEffect, AL_EAXREVERB_REFLECTIONS_GAIN, pEFXEAXReverb->flReflectionsGain);
		alEffectf(uiEffect, AL_EAXREVERB_REFLECTIONS_DELAY, pEFXEAXReverb->flReflectionsDelay);
		alEffectfv(uiEffect, AL_EAXREVERB_REFLECTIONS_PAN, pEFXEAXReverb->flReflectionsPan);
		alEffectf(uiEffect, AL_EAXREVERB_LATE_REVERB_GAIN, pEFXEAXReverb->flLateReverbGain);
		alEffectf(uiEffect, AL_EAXREVERB_LATE_REVERB_DELAY, pEFXEAXReverb->flLateReverbDelay);
		alEffectfv(uiEffect, AL_EAXREVERB_LATE_REVERB_PAN, pEFXEAXReverb->flLateReverbPan);
		alEffectf(uiEffect, AL_EAXREVERB_ECHO_TIME, pEFXEAXReverb->flEchoTime);
		alEffectf(uiEffect, AL_EAXREVERB_ECHO_DEPTH, pEFXEAXReverb->flEchoDepth);
		alEffectf(uiEffect, AL_EAXREVERB_MODULATION_TIME, pEFXEAXReverb->flModulationTime);
		alEffectf(uiEffect, AL_EAXREVERB_MODULATION_DEPTH, pEFXEAXReverb->flModulationDepth);
		alEffectf(uiEffect, AL_EAXREVERB_AIR_ABSORPTION_GAINHF, pEFXEAXReverb->flAirAbsorptionGainHF);
		alEffectf(uiEffect, AL_EAXREVERB_HFREFERENCE, pEFXEAXReverb->flHFReference);
		alEffectf(uiEffect, AL_EAXREVERB_LFREFERENCE, pEFXEAXReverb->flLFReference);
		alEffectf(uiEffect, AL_EAXREVERB_ROOM_ROLLOFF_FACTOR, pEFXEAXReverb->flRoomRolloffFactor);
		alEffecti(uiEffect, AL_EAXREVERB_DECAY_HFLIMIT, pEFXEAXReverb->iDecayHFLimit);

		if (alGetError() == AL_NO_ERROR)
			bReturn = AL_TRUE;
	}

	return bReturn;
}
