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
#include "Vorbis\vorbisfile.h"

#define NUMBUFFERS              (4)
#define	SERVICE_UPDATE_PERIOD	(20)

#define	TEST_OGGVORBIS_FILE		"stereo.ogg"

// Ogg Vorbis

// Ogg Voribis DLL Handle
HINSTANCE g_hVorbisFileDLL = NULL;

// Functions
void InitVorbisFile();
void ShutdownVorbisFile();
unsigned long DecodeOggVorbis(OggVorbis_File *psOggVorbisFile, char *pDecodeBuffer, unsigned long ulBufferSize, unsigned long ulChannels);
void Swap(short &s1, short &s2);

size_t ov_read_func(void *ptr, size_t size, size_t nmemb, void *datasource);
int ov_seek_func(void *datasource, ogg_int64_t offset, int whence);
int ov_close_func(void *datasource);
long ov_tell_func(void *datasource);

// Function pointers
typedef int (*LPOVCLEAR)(OggVorbis_File *vf);
typedef long (*LPOVREAD)(OggVorbis_File *vf,char *buffer,int length,int bigendianp,int word,int sgned,int *bitstream);
typedef ogg_int64_t (*LPOVPCMTOTAL)(OggVorbis_File *vf,int i);
typedef vorbis_info * (*LPOVINFO)(OggVorbis_File *vf,int link);
typedef vorbis_comment * (*LPOVCOMMENT)(OggVorbis_File *vf,int link);
typedef int (*LPOVOPENCALLBACKS)(void *datasource, OggVorbis_File *vf,char *initial, long ibytes, ov_callbacks callbacks);

// Variables
LPOVCLEAR			fn_ov_clear = NULL;
LPOVREAD			fn_ov_read = NULL;
LPOVPCMTOTAL		fn_ov_pcm_total = NULL;
LPOVINFO			fn_ov_info = NULL;
LPOVCOMMENT			fn_ov_comment = NULL;
LPOVOPENCALLBACKS	fn_ov_open_callbacks = NULL;

bool g_bVorbisInit = false;

int main()
{
	ALuint		    uiBuffers[NUMBUFFERS];
	ALuint		    uiSource;
	ALuint			uiBuffer;
	ALint			iState;
	ALint			iLoop;
	ALint			iBuffersProcessed, iTotalBuffersProcessed, iQueuedBuffers;
	unsigned long	ulFrequency = 0;
	unsigned long	ulFormat = 0;
	unsigned long	ulChannels = 0;
	unsigned long	ulBufferSize;
	unsigned long	ulBytesWritten;
	char			*pDecodeBuffer;

	// Initialize Framework
	ALFWInit();

	ALFWprintf("PlayOggVorbis Test Application\n");

	// Initialize OggVorbis libary
	InitVorbisFile();
	if (!g_bVorbisInit)
	{
		ALFWprintf("Failed to find OggVorbis DLLs (vorbisfile.dll, ogg.dll, or vorbis.dll)\n");
		ALFWShutdown();
		return 0;
	}

	// Initialise OpenAL
	if (!ALFWInitOpenAL())
	{
		ALFWprintf("Failed to initialize OpenAL\n");
		ALFWShutdown();
		return 0;
	}

	// Open Ogg Stream
	ov_callbacks	sCallbacks;
	OggVorbis_File	sOggVorbisFile;
	vorbis_info		*psVorbisInfo;

	sCallbacks.read_func = ov_read_func;
	sCallbacks.seek_func = ov_seek_func;
	sCallbacks.close_func = ov_close_func;
	sCallbacks.tell_func = ov_tell_func;

	// Open the OggVorbis file
	FILE *pOggVorbisFile = fopen(ALFWaddMediaPath(TEST_OGGVORBIS_FILE), "rb");
	if (!pOggVorbisFile)
	{
		ALFWprintf("Could not find %s\n", ALFWaddMediaPath(TEST_OGGVORBIS_FILE));
		ShutdownVorbisFile();
		ALFWShutdownOpenAL();
		ALFWShutdown();
		return 0;
	}

	// Create an OggVorbis file stream
	if (fn_ov_open_callbacks(pOggVorbisFile, &sOggVorbisFile, NULL, 0, sCallbacks) == 0)
	{
		// Get some information about the file (Channels, Format, and Frequency)
		psVorbisInfo = fn_ov_info(&sOggVorbisFile, -1);
		if (psVorbisInfo)
		{
			ulFrequency = psVorbisInfo->rate;
			ulChannels = psVorbisInfo->channels;
			if (psVorbisInfo->channels == 1)
			{
				ulFormat = AL_FORMAT_MONO16;
				// Set BufferSize to 250ms (Frequency * 2 (16bit) divided by 4 (quarter of a second))
				ulBufferSize = ulFrequency >> 1;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				ulBufferSize -= (ulBufferSize % 2);
			}
			else if (psVorbisInfo->channels == 2)
			{
				ulFormat = AL_FORMAT_STEREO16;
				// Set BufferSize to 250ms (Frequency * 4 (16bit stereo) divided by 4 (quarter of a second))
				ulBufferSize = ulFrequency;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				ulBufferSize -= (ulBufferSize % 4);
			}
			else if (psVorbisInfo->channels == 4)
			{
				ulFormat = alGetEnumValue("AL_FORMAT_QUAD16");
				// Set BufferSize to 250ms (Frequency * 8 (16bit 4-channel) divided by 4 (quarter of a second))
				ulBufferSize = ulFrequency * 2;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				ulBufferSize -= (ulBufferSize % 8);
			}
			else if (psVorbisInfo->channels == 6)
			{
				ulFormat = alGetEnumValue("AL_FORMAT_51CHN16");
				// Set BufferSize to 250ms (Frequency * 12 (16bit 6-channel) divided by 4 (quarter of a second))
				ulBufferSize = ulFrequency * 3;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				ulBufferSize -= (ulBufferSize % 12);
			}
		}

		if (ulFormat != 0)
		{
			// Allocate a buffer to be used to store decoded data for all Buffers
			pDecodeBuffer = (char*)malloc(ulBufferSize);
			if (!pDecodeBuffer)
			{
				ALFWprintf("Failed to allocate memory for decoded OggVorbis data\n");
				fn_ov_clear(&sOggVorbisFile);
				ShutdownVorbisFile();
				ALFWShutdownOpenAL();
				ALFWShutdown();
			}

			// Generate some AL Buffers for streaming
			alGenBuffers( NUMBUFFERS, uiBuffers );

			// Generate a Source to playback the Buffers
			alGenSources( 1, &uiSource );

			// Fill all the Buffers with decoded audio data from the OggVorbis file
			for (iLoop = 0; iLoop < NUMBUFFERS; iLoop++)
			{
				ulBytesWritten = DecodeOggVorbis(&sOggVorbisFile, pDecodeBuffer, ulBufferSize, ulChannels);
				if (ulBytesWritten)
				{
					alBufferData(uiBuffers[iLoop], ulFormat, pDecodeBuffer, ulBytesWritten, ulFrequency);
					alSourceQueueBuffers(uiSource, 1, &uiBuffers[iLoop]);
				}
			}

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
					ulBytesWritten = DecodeOggVorbis(&sOggVorbisFile, pDecodeBuffer, ulBufferSize, ulChannels);
					if (ulBytesWritten)
					{
						alBufferData(uiBuffer, ulFormat, pDecodeBuffer, ulBytesWritten, ulFrequency);
						alSourceQueueBuffers(uiSource, 1, &uiBuffer);
					}

					iBuffersProcessed--;
				}

				// Check the status of the Source.  If it is not playing, then playback was completed,
				// or the Source was starved of audio data, and needs to be restarted.
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

			// Stop the Source and clear the Queue
			alSourceStop(uiSource);
			alSourcei(uiSource, AL_BUFFER, 0);

			if (pDecodeBuffer)
			{
				free(pDecodeBuffer);
				pDecodeBuffer = NULL;
			}

			// Clean up buffers and sources
			alDeleteSources( 1, &uiSource );
			alDeleteBuffers( NUMBUFFERS, uiBuffers );
		}
		else
		{
			ALFWprintf("Failed to find format information, or unsupported format\n");
		}

		// Close OggVorbis stream
		fn_ov_clear(&sOggVorbisFile);
	}

	// Shutdown VorbisFile Library
	ShutdownVorbisFile();

	// Shutdown AL
	ALFWShutdownOpenAL();

	// Shutdown Framework
	ALFWShutdown();

    return 0;
}

void InitVorbisFile()
{	
	if (g_bVorbisInit)
		return;

	// Try and load Vorbis DLLs (VorbisFile.dll will load ogg.dll and vorbis.dll)
	g_hVorbisFileDLL = LoadLibrary("vorbisfile.dll");
	if (g_hVorbisFileDLL)
	{
		fn_ov_clear = (LPOVCLEAR)GetProcAddress(g_hVorbisFileDLL, "ov_clear");
		fn_ov_read = (LPOVREAD)GetProcAddress(g_hVorbisFileDLL, "ov_read");
		fn_ov_pcm_total = (LPOVPCMTOTAL)GetProcAddress(g_hVorbisFileDLL, "ov_pcm_total");
		fn_ov_info = (LPOVINFO)GetProcAddress(g_hVorbisFileDLL, "ov_info");
		fn_ov_comment = (LPOVCOMMENT)GetProcAddress(g_hVorbisFileDLL, "ov_comment");
		fn_ov_open_callbacks = (LPOVOPENCALLBACKS)GetProcAddress(g_hVorbisFileDLL, "ov_open_callbacks");

		if (fn_ov_clear && fn_ov_read && fn_ov_pcm_total && fn_ov_info &&
			fn_ov_comment && fn_ov_open_callbacks)
		{
			g_bVorbisInit = true;
		}
	}
}

void ShutdownVorbisFile()
{
	if (g_hVorbisFileDLL)
	{
		FreeLibrary(g_hVorbisFileDLL);
		g_hVorbisFileDLL = NULL;
	}
	g_bVorbisInit = false;
}

unsigned long DecodeOggVorbis(OggVorbis_File *psOggVorbisFile, char *pDecodeBuffer, unsigned long ulBufferSize, unsigned long ulChannels)
{
	int current_section;
	long lDecodeSize;
	unsigned long ulSamples;
	short *pSamples;

	unsigned long ulBytesDone = 0;
	while (1)
	{
		lDecodeSize = fn_ov_read(psOggVorbisFile, pDecodeBuffer + ulBytesDone, ulBufferSize - ulBytesDone, 0, 2, 1, &current_section);
		if (lDecodeSize > 0)
		{
			ulBytesDone += lDecodeSize;

			if (ulBytesDone >= ulBufferSize)
				break;
		}
		else
		{
			break;
		}
	}

	// Mono, Stereo and 4-Channel files decode into the same channel order as WAVEFORMATEXTENSIBLE,
	// however 6-Channels files need to be re-ordered
	if (ulChannels == 6)
	{		
		pSamples = (short*)pDecodeBuffer;
		for (ulSamples = 0; ulSamples < (ulBufferSize>>1); ulSamples+=6)
		{
			// WAVEFORMATEXTENSIBLE Order : FL, FR, FC, LFE, RL, RR
			// OggVorbis Order            : FL, FC, FR,  RL, RR, LFE
			Swap(pSamples[ulSamples+1], pSamples[ulSamples+2]);
			Swap(pSamples[ulSamples+3], pSamples[ulSamples+5]);
			Swap(pSamples[ulSamples+4], pSamples[ulSamples+5]);
		}
	}

	return ulBytesDone;
}

void Swap(short &s1, short &s2)
{
	short sTemp = s1;
	s1 = s2;
	s2 = sTemp;
}

size_t ov_read_func(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	return fread(ptr, size, nmemb, (FILE*)datasource);
}

int ov_seek_func(void *datasource, ogg_int64_t offset, int whence)
{
	return fseek((FILE*)datasource, (long)offset, whence);
}

int ov_close_func(void *datasource)
{
   return fclose((FILE*)datasource);
}

long ov_tell_func(void *datasource)
{
	return ftell((FILE*)datasource);
}
