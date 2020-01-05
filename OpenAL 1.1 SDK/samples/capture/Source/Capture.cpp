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

#define	OUTPUT_WAVE_FILE		"Capture.wav"
#define BUFFERSIZE				4410

#pragma pack (push,1)
typedef struct
{
	char			szRIFF[4];
	long			lRIFFSize;
	char			szWave[4];
	char			szFmt[4];
	long			lFmtSize;
	WAVEFORMATEX	wfex;
	char			szData[4];
	long			lDataSize;
} WAVEHEADER;
#pragma pack (pop)

int main()
{
	ALCdevice		*pDevice;
	ALCcontext		*pContext;
	ALCdevice		*pCaptureDevice;
	const ALCchar	*szDefaultCaptureDevice;
	ALint			iSamplesAvailable;
	FILE			*pFile;
	ALchar			Buffer[BUFFERSIZE];
	WAVEHEADER		sWaveHeader;
	ALint			iDataSize = 0;
	ALint			iSize;

	// NOTE : This code does NOT setup the Wave Device's Audio Mixer to select a recording input
	// or a recording level.

	// Initialize Framework
	ALFWInit();

	ALFWprintf("Capture Application\n");

	if (!ALFWInitOpenAL())
	{
		ALFWprintf("Failed to initialize OpenAL\n");
		ALFWShutdown();
		return 0;
	}

	// Check for Capture Extension support
	pContext = alcGetCurrentContext();
	pDevice = alcGetContextsDevice(pContext);
	if (alcIsExtensionPresent(pDevice, "ALC_EXT_CAPTURE") == AL_FALSE)
	{
		ALFWprintf("Failed to detect Capture Extension\n");
		ALFWShutdownOpenAL();
		ALFWShutdown();
		return 0;
	}

	// Get list of available Capture Devices
	const ALchar *pDeviceList = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER);
	if (pDeviceList)
	{
		ALFWprintf("\nAvailable Capture Devices are:-\n");

		while (*pDeviceList)
		{
			ALFWprintf("%s\n", pDeviceList);
			pDeviceList += strlen(pDeviceList) + 1;
		}
	}

	// Get the name of the 'default' capture device
	szDefaultCaptureDevice = alcGetString(NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER);
	ALFWprintf("\nDefault Capture Device is '%s'\n\n", szDefaultCaptureDevice);

	// Open the default Capture device to record a 22050Hz 16bit Mono Stream using an internal buffer
	// of BUFFERSIZE Samples (== BUFFERSIZE * 2 bytes)
	pCaptureDevice = alcCaptureOpenDevice(szDefaultCaptureDevice, 22050, AL_FORMAT_MONO16, BUFFERSIZE);
	if (pCaptureDevice)
	{
		ALFWprintf("Opened '%s' Capture Device\n\n", alcGetString(pCaptureDevice, ALC_CAPTURE_DEVICE_SPECIFIER));

		// Create / open a file for the captured data
		pFile = fopen(OUTPUT_WAVE_FILE, "wb");

		// Prepare a WAVE file header for the captured data
		sprintf(sWaveHeader.szRIFF, "RIFF");
		sWaveHeader.lRIFFSize = 0;
		sprintf(sWaveHeader.szWave, "WAVE");
		sprintf(sWaveHeader.szFmt, "fmt ");
		sWaveHeader.lFmtSize = sizeof(WAVEFORMATEX);		
		sWaveHeader.wfex.nChannels = 1;
		sWaveHeader.wfex.wBitsPerSample = 16;
		sWaveHeader.wfex.wFormatTag = WAVE_FORMAT_PCM;
		sWaveHeader.wfex.nSamplesPerSec = 22050;
		sWaveHeader.wfex.nBlockAlign = sWaveHeader.wfex.nChannels * sWaveHeader.wfex.wBitsPerSample / 8;
		sWaveHeader.wfex.nAvgBytesPerSec = sWaveHeader.wfex.nSamplesPerSec * sWaveHeader.wfex.nBlockAlign;
		sWaveHeader.wfex.cbSize = 0;
		sprintf(sWaveHeader.szData, "data");
		sWaveHeader.lDataSize = 0;

		fwrite(&sWaveHeader, sizeof(WAVEHEADER), 1, pFile);

		// Start audio capture
		alcCaptureStart(pCaptureDevice);

		// Record for two seconds or until a key is pressed
		DWORD dwStartTime = timeGetTime();
		while (!ALFWKeyPress() && (timeGetTime() <= (dwStartTime + 2000)))
		{
			// Release some CPU time ...
			Sleep(1);

			// Find out how many samples have been captured
			alcGetIntegerv(pCaptureDevice, ALC_CAPTURE_SAMPLES, 1, &iSamplesAvailable);

			ALFWprintf("Samples available : %d\r", iSamplesAvailable);

			// When we have enough data to fill our BUFFERSIZE byte buffer, grab the samples
			if (iSamplesAvailable > (BUFFERSIZE / sWaveHeader.wfex.nBlockAlign))
			{
				// Consume Samples
				alcCaptureSamples(pCaptureDevice, Buffer, BUFFERSIZE / sWaveHeader.wfex.nBlockAlign);

				// Write the audio data to a file
				fwrite(Buffer, BUFFERSIZE, 1, pFile);

				// Record total amount of data recorded
				iDataSize += BUFFERSIZE;
			}
		}

		// Stop capture
		alcCaptureStop(pCaptureDevice);

		// Check if any Samples haven't been consumed yet
		alcGetIntegerv(pCaptureDevice, ALC_CAPTURE_SAMPLES, 1, &iSamplesAvailable);
		while (iSamplesAvailable)
		{
			if (iSamplesAvailable > (BUFFERSIZE / sWaveHeader.wfex.nBlockAlign))
			{
				alcCaptureSamples(pCaptureDevice, Buffer, BUFFERSIZE / sWaveHeader.wfex.nBlockAlign);
				fwrite(Buffer, BUFFERSIZE, 1, pFile);
				iSamplesAvailable -= (BUFFERSIZE / sWaveHeader.wfex.nBlockAlign);
				iDataSize += BUFFERSIZE;
			}
			else
			{
				alcCaptureSamples(pCaptureDevice, Buffer, iSamplesAvailable);
				fwrite(Buffer, iSamplesAvailable * sWaveHeader.wfex.nBlockAlign, 1, pFile);
				iDataSize += iSamplesAvailable * sWaveHeader.wfex.nBlockAlign;
				iSamplesAvailable = 0;
			}
		}

		// Fill in Size information in Wave Header
		fseek(pFile, 4, SEEK_SET);
		iSize = iDataSize + sizeof(WAVEHEADER) - 8;
		fwrite(&iSize, 4, 1, pFile);
		fseek(pFile, 42, SEEK_SET);
		fwrite(&iDataSize, 4, 1, pFile);

		fclose(pFile);

		ALFWprintf("\nSaved captured audio data to '%s'\n", OUTPUT_WAVE_FILE);

		// Close the Capture Device
		alcCaptureCloseDevice(pCaptureDevice);
	}

	// Close down OpenAL
	ALFWShutdownOpenAL();

	// Close down the Framework
	ALFWShutdown();

	return 0;
}