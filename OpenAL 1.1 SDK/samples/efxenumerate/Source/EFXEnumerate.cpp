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

int main()
{
	ALCdevice *	pDevice;
	ALuint		uiEffectSlots[128] = { 0 };
	ALuint		uiEffects[1] = { 0 };
	ALuint		uiFilters[1] = { 0 };
	ALint		iEffectSlotsGenerated;
	ALint		iSends;

	// Initialize Framework
	ALFWInit();

	ALFWprintf("Enumerate EFX Application\n");

	if (!ALFWInitOpenAL())
	{
		ALFWprintf("Failed to initialize OpenAL\n");
		ALFWShutdown();
		return 0;
	}

	// Get the current AL Device
	pDevice = alcGetContextsDevice(alcGetCurrentContext());

	// Check for EFX Extension
	if (ALFWIsEFXSupported())
	{
		// To determine how many Auxiliary Effects Slots are available, create as many as possible (up to 128)
		// until the call fails.
		for (iEffectSlotsGenerated = 0; iEffectSlotsGenerated < 128; iEffectSlotsGenerated++)
		{
			alGenAuxiliaryEffectSlots(1, &uiEffectSlots[iEffectSlotsGenerated]);
			if (alGetError() != AL_NO_ERROR)
				break;
		}

		ALFWprintf("%d Auxiliary Effect Slot%s\n", iEffectSlotsGenerated, (iEffectSlotsGenerated == 1) ? "" : "s");

		// Retrieve the number of Auxiliary Effect Slots Sends available on each Source
		alcGetIntegerv(pDevice, ALC_MAX_AUXILIARY_SENDS, 1, &iSends);
		printf("%d Auxiliary Send%s per Source\n", iSends, (iSends == 1) ? "" : "s");

		// To determine which Effects are supported, generate an Effect Object, and try to set its type to
		// the various Effect enum values
		printf("\nEffects Supported: -\n");
		alGenEffects(1, &uiEffects[0]);
		if (alGetError() == AL_NO_ERROR)
		{
			// Try setting Effect Type to known Effects
			alEffecti(uiEffects[0], AL_EFFECT_TYPE, AL_EFFECT_REVERB);
			printf("'Reverb' Support            %s\n", (alGetError() == AL_NO_ERROR) ? "YES" : "NO");

			alEffecti(uiEffects[0], AL_EFFECT_TYPE, AL_EFFECT_EAXREVERB);
			printf("'EAX Reverb' Support        %s\n", (alGetError() == AL_NO_ERROR) ? "YES" : "NO");

			alEffecti(uiEffects[0], AL_EFFECT_TYPE, AL_EFFECT_CHORUS);
			printf("'Chorus' Support            %s\n", (alGetError() == AL_NO_ERROR) ? "YES" : "NO");

			alEffecti(uiEffects[0], AL_EFFECT_TYPE, AL_EFFECT_DISTORTION);
			printf("'Distortion' Support        %s\n", (alGetError() == AL_NO_ERROR) ? "YES" : "NO");

			alEffecti(uiEffects[0], AL_EFFECT_TYPE, AL_EFFECT_ECHO);
			printf("'Echo' Support              %s\n", (alGetError() == AL_NO_ERROR) ? "YES" : "NO");

			alEffecti(uiEffects[0], AL_EFFECT_TYPE, AL_EFFECT_FLANGER);
			printf("'Flanger' Support           %s\n", (alGetError() == AL_NO_ERROR) ? "YES" : "NO");

			alEffecti(uiEffects[0], AL_EFFECT_TYPE, AL_EFFECT_FREQUENCY_SHIFTER);
			printf("'Frequency Shifter' Support %s\n", (alGetError() == AL_NO_ERROR) ? "YES" : "NO");

			alEffecti(uiEffects[0], AL_EFFECT_TYPE, AL_EFFECT_VOCAL_MORPHER);
			printf("'Vocal Morpher' Support     %s\n", (alGetError() == AL_NO_ERROR) ? "YES" : "NO");

			alEffecti(uiEffects[0], AL_EFFECT_TYPE, AL_EFFECT_PITCH_SHIFTER);
			printf("'Pitch Shifter' Support     %s\n", (alGetError() == AL_NO_ERROR) ? "YES" : "NO");

			alEffecti(uiEffects[0], AL_EFFECT_TYPE, AL_EFFECT_RING_MODULATOR);
			printf("'Ring Modulator' Support    %s\n", (alGetError() == AL_NO_ERROR) ? "YES" : "NO");

			alEffecti(uiEffects[0], AL_EFFECT_TYPE, AL_EFFECT_AUTOWAH);
			printf("'Autowah' Support           %s\n", (alGetError() == AL_NO_ERROR) ? "YES" : "NO");

			alEffecti(uiEffects[0], AL_EFFECT_TYPE, AL_EFFECT_COMPRESSOR);
			printf("'Compressor' Support        %s\n", (alGetError() == AL_NO_ERROR) ? "YES" : "NO");

			alEffecti(uiEffects[0], AL_EFFECT_TYPE, AL_EFFECT_EQUALIZER);
			printf("'Equalizer' Support         %s\n", (alGetError() == AL_NO_ERROR) ? "YES" : "NO");
		}

		// To determine which Filters are supported, generate a Filter Object, and try to set its type to
		// the various Filter enum values
		printf("\nFilters Supported: -\n");
		// Generate a Filter to use to determine what Filter Types are supported
		alGenFilters(1, &uiFilters[0]);
		if (alGetError() == AL_NO_ERROR)
		{
			// Try setting the Filter type to known Filters
			alFilteri(uiFilters[0], AL_FILTER_TYPE, AL_FILTER_LOWPASS);
			printf("'Low Pass'  Support         %s\n", (alGetError() == AL_NO_ERROR) ? "YES" : "NO");

			alFilteri(uiFilters[0], AL_FILTER_TYPE, AL_FILTER_HIGHPASS);
			printf("'High Pass' Support         %s\n", (alGetError() == AL_NO_ERROR) ? "YES" : "NO");

			alFilteri(uiFilters[0], AL_FILTER_TYPE, AL_FILTER_BANDPASS);
			printf("'Band Pass' Support         %s\n", (alGetError() == AL_NO_ERROR) ? "YES" : "NO");
		}

		// Clean-up ...

		// Delete Filter
		alDeleteFilters(1, &uiFilters[0]);

		// Delete Effect
		alDeleteEffects(1, &uiEffects[0]);

		// Delete Auxiliary Effect Slots
		alDeleteAuxiliaryEffectSlots(iEffectSlotsGenerated, uiEffectSlots);
	}
	else
	{
		ALFWprintf("EFX support not found\n");
	}

	ALFWShutdownOpenAL();

	ALFWShutdown();

    return 0;
}