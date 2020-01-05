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
#include "CWaves.h"

#define NUMBUFFERS              (4)
#define	SERVICE_UPDATE_PERIOD	(20)

#define	TEST_WAVE_FILE		"FivePtOne.wav"

int main()
{
	ALuint		    uiBuffers[NUMBUFFERS];
	ALuint		    uiSource;
	ALuint			uiBuffer;
	ALint			iState;
	CWaves *		pWaveLoader = NULL;
	WAVEID			WaveID;
	ALint			iLoop;
	ALint			iBuffersProcessed, iTotalBuffersProcessed, iQueuedBuffers;
	WAVEFORMATEX	wfex;
	unsigned long	ulDataSize = 0;
	unsigned long	ulFrequency = 0;
	unsigned long	ulFormat = 0;
	unsigned long	ulBufferSize;
	unsigned long	ulBytesWritten;
	void *			pData = NULL;
	ALenum			eBufferFormat = 0;

	// Initialize Framework
	ALFWInit();

	ALFWprintf("PlayMultiChannel Test Application\n");

	if (!ALFWInitOpenAL())
	{
		ALFWprintf("Failed to initialize OpenAL\n");
		ALFWShutdown();
		return 0;
	}

	// OpenAL implementations are only guaranteed to support MONO and STEREO Buffers, so we need
	// to check if this implementation supports multi-channel formats.
	// This is done by making alGetEnumValue calls to request the value of the Buffer Format Tag Enum (that will be
	// passed to an alBufferData call).   Enum Values are retrieved by string names.  The following names are defined
	// for multi-channel wave formats ...
	// "AL_FORMAT_QUAD16"	: 4 Channel,   16 bit data
	// "AL_FORMAT_51CHN16"	: 5.1 Channel, 16 bit data
	// "AL_FORMAT_61CHN16"	: 6.1 Channel, 16 bit data
	// "AL_FORMAT_71CHN16"	: 7.1 Channel, 16 bit data

	// For this example, we are going to play a 5.1 test file ...
	eBufferFormat = alGetEnumValue("AL_FORMAT_51CHN16");
	if (!eBufferFormat)
	{
		ALFWprintf("No support for 5.1 playback!\n");
		ALFWShutdownOpenAL();
		ALFWShutdown();
		return 0;
	}

    // Generate some AL Buffers for streaming
	alGenBuffers( NUMBUFFERS, uiBuffers );

	// Generate a Source to playback the Buffers
    alGenSources( 1, &uiSource );

	// Create instance of WaveLoader class
	pWaveLoader = new CWaves();
	if ((pWaveLoader) && (SUCCEEDED(pWaveLoader->OpenWaveFile(ALFWaddMediaPath(TEST_WAVE_FILE), &WaveID))))
	{
		pWaveLoader->GetWaveSize(WaveID, &ulDataSize);
		pWaveLoader->GetWaveFrequency(WaveID, &ulFrequency);
		// NOTE : The WaveLoader class will use alGetEnumValue to find the appropriate format tag (if supported)
		pWaveLoader->GetWaveALBufferFormat(WaveID, &alGetEnumValue, &ulFormat);

		// Queue 250ms of audio data
		pWaveLoader->GetWaveFormatExHeader(WaveID, &wfex);
		ulBufferSize = wfex.nAvgBytesPerSec >> 2;

		// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
		ulBufferSize -= (ulBufferSize % wfex.nBlockAlign);

		if (ulFormat != 0)
		{
			pData = malloc(ulBufferSize);
			if (pData)
			{
				// Set read position to start of audio data
				pWaveLoader->SetWaveDataOffset(WaveID, 0);

				// Clear AL Error
				alGetError();

				// Fill all the Buffers with audio data from the wavefile
				for (iLoop = 0; iLoop < NUMBUFFERS; iLoop++)
				{
					if (SUCCEEDED(pWaveLoader->ReadWaveData(WaveID, pData, ulBufferSize, &ulBytesWritten)))
					{
						alBufferData(uiBuffers[iLoop], ulFormat, pData, ulBytesWritten, ulFrequency);
						alSourceQueueBuffers(uiSource, 1, &uiBuffers[iLoop]);
						if (alGetError() != AL_NO_ERROR)
						{
							// When a multi-channel buffer is attached to a Source (either directly using AL_BUFFER) or
							// via Source Queuing ... the call will fail if the playback device does not have enough
							// resources to play the buffer.  (On H/W devices each channel of a Buffer requires a H/W voice,
							// so to play a 5.1 Buffer on a Source requires 6 H/W Voices).
							break;
						}
					}
				}

				if (iLoop != NUMBUFFERS)
				{
					ALFWprintf("Failed to attach MultiChannel buffer to Source\n");
				}
				else
				{
					// Start playing source
					alSourcePlay(uiSource);

					iTotalBuffersProcessed = 0;

					while (!ALFWKeyPress())
					{
						Sleep( SERVICE_UPDATE_PERIOD );

						// Request the number of OpenAL Buffers have been processed (played) on the Source
						iBuffersProcessed = 0;
						alGetSourcei(uiSource, AL_BUFFERS_PROCESSED, &iBuffersProcessed);
						
						// Keep a running count of number of buffers processed (for logging purposes only)
						iTotalBuffersProcessed += iBuffersProcessed;
						ALFWprintf("Buffers Processed %d\r", iTotalBuffersProcessed);

						// For each processed buffer, remove it from the Source Queue, read next chunk of audio
						// data from disk, fill buffer with new data, and add it to the Source Queue
						while (iBuffersProcessed)
						{
							// Remove the Buffer from the Queue.  (uiBuffer contains the Buffer ID for the unqueued Buffer)
							uiBuffer = 0;
							alSourceUnqueueBuffers(uiSource, 1, &uiBuffer);

							// Read more audio data (if there is any)
							pWaveLoader->ReadWaveData(WaveID, pData, ulBufferSize, &ulBytesWritten);
							if (ulBytesWritten)
							{
								// Copy audio data to Buffer
								alBufferData(uiBuffer, ulFormat, pData, ulBytesWritten, ulFrequency);
								// Queue Buffer on the Source
								alSourceQueueBuffers(uiSource, 1, &uiBuffer);
							}

							iBuffersProcessed--;
						}

						// Check the status of the Source.  If it is not playing, then playback was either completed,
						// or the Source was starved of audio data and needs to be restarted.
						alGetSourcei(uiSource, AL_SOURCE_STATE, &iState);
						if (iState != AL_PLAYING)
						{
							// If there are Buffers in the Source Queue then the Source was starved of audio
							// data, so needs to be restarted (because there is more audio data to play)
							alGetSourcei(uiSource, AL_BUFFERS_QUEUED, &iQueuedBuffers);
							if (iQueuedBuffers)
							{
								alSourcePlay(uiSource);
							}
							else
							{
								// Finished playing
								break;
							}
						}
					}
				}

				// Stop the Source and clear the Queue
				alSourceStop(uiSource);
				alSourcei(uiSource, AL_BUFFER, 0);

				// Release temporary storage
				free(pData);
				pData = NULL;
			}
			else
			{
				ALFWprintf("Out of memory\n");
			}
		}
		else
		{
			ALFWprintf("Unknown Audio Buffer format\n");
		}

		// Close Wave Handle
		pWaveLoader->DeleteWaveFile(WaveID);
	}
	else
	{
		ALFWprintf("Failed to load %s\n", ALFWaddMediaPath(TEST_WAVE_FILE));
	}

    // Clean up buffers and sources
	alDeleteSources( 1, &uiSource );
	alDeleteBuffers( NUMBUFFERS, uiBuffers );

	if (pWaveLoader)
		delete pWaveLoader;

	ALFWShutdownOpenAL();

	ALFWShutdown();

    return 0;
}
