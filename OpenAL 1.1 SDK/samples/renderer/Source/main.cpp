/*
	OpenAL4SWMixers

	PURPOSE : Use OpenAL to render pre-mixed audio data from a S/W mixer.  Optionally
	allow pre-mixed 'wet' buffers to feed hardware reverb effects (up to 4).

	BENEFITS : Compared to WASAPI, a native OpenAL device will communicate
	directly with audio drivers to by-pass the entire Vista audio system (Local
	APOs, mixer, Global APOs, etc ...).  This approach is more efficient and
	allows for much lower latency due to improved playback position monitoring.
	The hardware EAX reverb effects are efficient and very high quality.
*/

#include "ALOutput.h"
#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include "CWaves.h"

void MixData(unsigned long ulNumSamples, unsigned long ulOutputChannels, void *pOutput, unsigned long ulNumEffects, void **pEffectOutput);

// Some variables to control the data that is mixed into the buffers feeding OpenAL
CWaves *g_pWaveLoader = NULL;
WAVEID g_WaveID = 0;
bool g_bPlayTestWaveFile = false;
float g_flEffectSends[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

void main()
{
	unsigned long ulOutputChannels = 0;
	unsigned long ulNumEffects = 0;
	unsigned long ulNumSamples = 0;
	unsigned long ulBuffersProcessed = 0;
	void *pOutput = 0;
	void *pEffectOutput[4] = { 0 };
	char ch;

	// Initialize OpenAL
	//	   0 (NULL) device (opens default OpenAL device)
	//	   8 (7.1) Channel Output for dry-mix
	//	   2 Effect Busses (mono)
	//  1024 Samples per mixing block (OpenAL Buffer)
	// 48000 Output frequency
	if (InitializeOpenAL(NULL, 8, 2, 1024, 48000))
	{
		// See what we got ...
		ulOutputChannels = GetNumOutputChannels();
		ulNumEffects = GetNumEffects();
		ulNumSamples = GetNumSamplesPerBuffer();

		printf("OpenAL initialized\nRendering to ");
		switch(ulOutputChannels)
		{
		case 2:
		case 4:
			printf("%d.0 ", ulOutputChannels);
			break;
		case 6:
		case 7:
		case 8:
			printf("%d.1 ", ulOutputChannels - 1);
			break;
		default:
			printf("?.? ");
		}
		printf("with %d Effect Send Buffers\n", ulNumEffects);

		// Allocate memory for the dry mix and the effect mixes
		bool bOutOfMemory = false;
		pOutput = malloc(ulOutputChannels * ulNumSamples * 2);
		for (unsigned long ulLoop = 0; ulLoop < ulNumEffects; ulLoop++)
		{
			pEffectOutput[ulLoop] = malloc(ulNumSamples * 2);
			if (pEffectOutput[ulLoop] == NULL)
				bOutOfMemory = true;
		}

		// Open test wave file for streaming
		g_pWaveLoader = new CWaves();

		if (g_pWaveLoader && pOutput && !bOutOfMemory)
		{
			// Load in a wave file
			if (SUCCEEDED(g_pWaveLoader->OpenWaveFile("..\\..\\Media\\sevenptone.wav", &g_WaveID)))
			{
				// Configure the hardware reverbs
				EFXEAXREVERBPROPERTIES EFXEAXReverb[2] = { EFX_EAX_REVERB_HANGAR, EFX_EAX_REVERB_BATHROOM };

				SetEFXEAXReverbProperties(&EFXEAXReverb[0], 0);
				SetEFXEAXReverbProperties(&EFXEAXReverb[1], 1);

				// Start playback
				Start();

				printf("Press 1 to play test wave file\n");
				printf("Press 2 to stop test wave file\n");
				printf("Press 3 to increase Effect Send 1 (to Hangar Reverb)\n");
				printf("Press 4 to decrease Effect Send 1\n");
				printf("Press 5 to increase Effect Send 2 (to Bathroom Reverb)\n");
				printf("Press 6 to decrease Effect Send 2\n");
				printf("Press Q to quit\n");

				do
				{
					Sleep(10);

					if (_kbhit())
					{
						ch = _getch();
						switch (ch)
						{
						case '1':
							if (g_bPlayTestWaveFile)
								g_pWaveLoader->SetWaveDataOffset(g_WaveID, 0);
							g_bPlayTestWaveFile = true;
							break;
						case '2':
							g_bPlayTestWaveFile = false;
							break;
						case '3':
							g_flEffectSends[0] += 0.125f;
							if (g_flEffectSends[0] > 1.0f)
								g_flEffectSends[0] = 1.0f;
							break;
						case '4':
							g_flEffectSends[0] -= 0.125f;
							if (g_flEffectSends[0] < 0.0f)
								g_flEffectSends[0] = 0.0f;
							break;
						case '5':
							g_flEffectSends[1] += 0.125f;
							if (g_flEffectSends[1] > 1.0f)
								g_flEffectSends[1] = 1.0f;
							break;
						case '6':
							g_flEffectSends[1] -= 0.125f;
							if (g_flEffectSends[1] < 0.0f)
								g_flEffectSends[1] = 0.0f;
							break;
						}
						printf("Effect Send 1 %.3f, Effect Send 2 %.3f\r",g_flEffectSends[0],g_flEffectSends[1]);
					}

					// Periodically request the number of 'processed' Buffers (i.e. number of blocks of 
					// requested number of samples (1024 in this example))
					// For each processed buffer ... 
					//     Fill the Data Buffers with new audio data (MixData function)
					//     Call CommitData to copy data to OpenAL streams
					ulBuffersProcessed = GetBuffersProcessed();
					while (ulBuffersProcessed)
					{
						MixData(ulNumSamples, ulOutputChannels, pOutput, ulNumEffects, &pEffectOutput[0]);
						CommitData(pOutput, &pEffectOutput[0]);
						ulBuffersProcessed--;
					}
				} while ((ch != 'Q') && (ch != 'q'));

				// Stop playback
				Stop();
			}
			else
				printf("Failed to load sevenptone.wav\n");
		}
		else
			printf("Out of memory\n");

		// Shutdown OpenAL
		ShutdownOpenAL();
	}

	// Clean-up ...

	if (pOutput)
		free(pOutput);

	for (unsigned long ulLoop = 0; ulLoop < ulNumEffects; ulLoop++)
	{
		if (pEffectOutput[ulLoop])
			free(pEffectOutput[ulLoop]);
	}

	if (g_pWaveLoader)
	{
		if (g_pWaveLoader->IsWaveID(g_WaveID))
			g_pWaveLoader->DeleteWaveFile(g_WaveID);
		delete g_pWaveLoader;
	}

	printf("\nPress a key to quit\n");
	_getch();
}

// Hypothetical Software Mixer
//  ulNumSamples		: Number of samples requested
//	ulOutputChannels	: Number of output channels (dry-mix)
//	pOutput				: Destination address for dry-mix (expects 'ulNumSamples' 16bit Samples for 'ulOutputChannels' Channels
//	ulNumEffects		: Number of effect outputs (wet-mixes)
//	pEffectOutput		: Array of desintation address for the wet-mixes (expect 'ulNumSamples' 16bit Samples for 1 output channel (i.e. mono))
void MixData(unsigned long ulNumSamples, unsigned long ulOutputChannels, void *pOutput, unsigned long ulNumEffects, void **pEffectOutput)
{
	unsigned long ulRequestedBytes;
	unsigned long ulBytesWritten = 0;
	unsigned long ulSample;
	unsigned long ulChannel;

	// Fill Dry Buffer with audio
	ulRequestedBytes = ulNumSamples * ulOutputChannels * 2;
	while (ulRequestedBytes)
	{
		if (g_bPlayTestWaveFile)
		{
			// Data for Dry Buffer
			g_pWaveLoader->ReadWaveData(g_WaveID, (char*)pOutput + ulBytesWritten, ulRequestedBytes, &ulBytesWritten);
			ulRequestedBytes -= ulBytesWritten;
			if (ulRequestedBytes)
				g_pWaveLoader->SetWaveDataOffset(g_WaveID, 0);
		}
		else
		{
			// Silence for Dry Buffer
			memset(pOutput, 0, ulRequestedBytes);
			ulRequestedBytes = 0;
		}
	}

	// Fill Effect Buffers with audio
	ulRequestedBytes = ulNumSamples * 2;
	for (unsigned long ulEffectBuffers = 0; ulEffectBuffers < ulNumEffects; ulEffectBuffers++)
	{
		if (g_bPlayTestWaveFile && (g_flEffectSends[ulEffectBuffers] != 0.0f))
		{
			for (ulSample = 0; ulSample < ulNumSamples; ulSample++)
			{
				float flMixSample = 0.0f;
				for (ulChannel = 0; ulChannel < ulOutputChannels; ulChannel++)
					flMixSample += ((short*)(pOutput))[(ulSample * ulOutputChannels) + ulChannel];

				if (flMixSample < -32768.0f)
					flMixSample = -32768.0f;
				else if (flMixSample > 32767.0f)
					flMixSample = 32767.0f;

				((short*)(pEffectOutput[ulEffectBuffers]))[ulSample] = (short)(flMixSample * g_flEffectSends[ulEffectBuffers]);
			}
		}
		else
		{
			// Silence for Effect Buffers
			memset(pEffectOutput[ulEffectBuffers], 0, ulRequestedBytes);
		}
	}
}