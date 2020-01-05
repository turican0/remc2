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

#include"Framework.h"

#define	TEST_WAVE_FILE		"Footsteps.wav"

ALboolean CreateFilter( ALuint *filter, ALenum filterType, ALfloat gain, ALfloat gainHF)
{
	// Clear AL Error
	alGetError();

	alGenFilters( 1, filter);
	if (alGetError() != AL_NO_ERROR)
		return AL_FALSE;

    if( alIsFilter( (*filter) ) )
    {
        alFilteri( (*filter), AL_FILTER_TYPE, filterType );
		if (alGetError() != AL_NO_ERROR)
			return AL_FALSE;
    }

	alFilterf((*filter), AL_LOWPASS_GAIN, gain);
	alFilterf((*filter), AL_LOWPASS_GAINHF, gainHF);

	return AL_TRUE;
}

void Play(ALuint uiSource)
{
	ALint       state;

    // Begin playback
    alSourcePlay( uiSource );

    while( 1  )
    {
	    // Get state
	    alGetSourcei( uiSource, AL_SOURCE_STATE, &state);
	    if( state != AL_PLAYING )    
            break;
    }
}

ALboolean PlayDry(ALuint uiSource)
{
	ALFWprintf("Source played dry\n");
	Play(uiSource);

	return AL_TRUE;
}

ALboolean PlayDirectFilter(ALuint uiSource)
{
	ALuint uiFilter;

	ALFWprintf("Source played through a direct lowpass filter\n");

	if(AL_FALSE == CreateFilter( &uiFilter, AL_FILTER_LOWPASS, 1.0f, 0.5f))
	{
		return AL_FALSE;
	}

	// Assign filter to the source
	alSourcei(uiSource, AL_DIRECT_FILTER, uiFilter);
	if (alGetError() != AL_NO_ERROR)
		return AL_FALSE;

	Play(uiSource);

	// Cleanup
	alSourcei(uiSource, AL_DIRECT_FILTER, AL_FILTER_NULL);
	alDeleteFilters( 1, &uiFilter );

	return AL_TRUE;
}

ALboolean PlayAuxiliaryNoFilter(ALuint uiSource)
{
	ALuint uiEffectSlot;
	ALuint uiEffect;

	ALFWprintf("Source played through an auxiliary reverb without filtering\n");
	alGenAuxiliaryEffectSlots( 1, &uiEffectSlot );

	// Create an effect to be loaded in the auxiliary effect slot
	alGenEffects( 1, &uiEffect );

    // Configure effect to be Reverb
    if( alIsEffect( uiEffect ) )
    {
        alEffecti( uiEffect, AL_EFFECT_TYPE, AL_EFFECT_REVERB );

        // Load effect into slot
        if( alIsAuxiliaryEffectSlot( uiEffectSlot ) )
        {
            alAuxiliaryEffectSloti( uiEffectSlot, AL_EFFECTSLOT_EFFECT, uiEffect );
        }
    }

	// Enable Send 0 from the Source to the Auxiliary Effect Slot without filtering
	alSource3i(uiSource, AL_AUXILIARY_SEND_FILTER, uiEffectSlot, 0, AL_FILTER_NULL);
	if (alGetError() != AL_NO_ERROR)
		return AL_FALSE;

	Play(uiSource);

	// Cleanup
    alSource3i(uiSource, AL_AUXILIARY_SEND_FILTER, AL_EFFECTSLOT_NULL, 0, AL_FILTER_NULL);
    alDeleteAuxiliaryEffectSlots( 1, &uiEffectSlot );
    alDeleteEffects( 1, &uiEffect );

	return AL_TRUE;
}

ALboolean PlayAuxiliaryFilter(ALuint uiSource)
{
	ALuint uiFilter;
	ALuint uiEffectSlot;
	ALuint uiEffect;

	ALFWprintf("Source played through an auxiliary reverb with lowpass filter\n");

	if( AL_FALSE == CreateFilter(&uiFilter, AL_FILTER_LOWPASS, 1.0f, 0.1f))
	{
		return AL_FALSE;
	}

	alGenAuxiliaryEffectSlots( 1, &uiEffectSlot );

	// Create an effect to be loaded in the auxiliary effect slot
	alGenEffects( 1, &uiEffect );

    // Configure effect to be Reverb
    if( alIsEffect( uiEffect ) )
    {
        alEffecti( uiEffect, AL_EFFECT_TYPE, AL_EFFECT_REVERB );

        // Load effect into slot
        if( alIsAuxiliaryEffectSlot( uiEffectSlot ) )
        {
            alAuxiliaryEffectSloti( uiEffectSlot, AL_EFFECTSLOT_EFFECT, uiEffect );
        }
    }

	// Enable Send 0 from the Source to the Auxiliary Effect Slot with filtering
	alSource3i(uiSource, AL_AUXILIARY_SEND_FILTER, uiEffectSlot, 0, uiFilter);
	if (alGetError() != AL_NO_ERROR)
		return AL_FALSE;

	Play(uiSource);

	// Cleanup
    alSource3i(uiSource, AL_AUXILIARY_SEND_FILTER, AL_EFFECTSLOT_NULL, 0, AL_FILTER_NULL);
	alDeleteFilters( 1, &uiFilter );
    alDeleteAuxiliaryEffectSlots( 1, &uiEffectSlot );
    alDeleteEffects( 1, &uiEffect );

	return AL_TRUE;
}

ALboolean playStaticWaveFile( const char *filename )
{
	ALuint      uiSource;
	ALuint      uiBuffer;

	// Check for EFX Extension
	if (!ALFWIsEFXSupported())
	{
		ALFWprintf("Unable to run PlayEFX example without the EFX extension\n");
		return AL_FALSE;
	}

	alGenBuffers( 1, &uiBuffer );
	if (alGetError() != AL_NO_ERROR)
		return AL_FALSE;

	if (!ALFWLoadWaveToBuffer((char*)ALFWaddMediaPath(filename), uiBuffer))
	{
		ALFWprintf("Failed to load or attach %s\n", ALFWaddMediaPath(filename));
		alDeleteBuffers(1, &uiBuffer);
		return AL_FALSE;
	}

    // Generate source object
    alGenSources( 1, &uiSource );

	alSourcei( uiSource, AL_BUFFER, uiBuffer );
	if (alGetError() != AL_NO_ERROR)
		return AL_FALSE;

	// Play the source with no filter
	PlayDry(uiSource);

	// Play the source with a direct filter
	PlayDirectFilter(uiSource);

	// Play the source through an auxiliary reverb with no filter
	PlayAuxiliaryNoFilter(uiSource);

	// Play the source through an auxiliary reverb with an auxiliary filter
	PlayAuxiliaryFilter(uiSource);
	
	return AL_TRUE;
}


int main(void)
{
	ALFWInit();
	if(AL_FALSE == ALFWInitOpenAL())
	{
		return FALSE;
	}
	playStaticWaveFile( TEST_WAVE_FILE );
	ALFWShutdownOpenAL();
	ALFWShutdown();
	return 0;
}