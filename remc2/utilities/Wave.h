#pragma once

#ifndef WAVE
#define WAVE

#include <cstdint>

#pragma pack (1)
typedef struct {
	char isRiff[4];
	int32_t fileSizeBytes;
	char fileType[4];
	char formatChunkMarker[4];
	int32_t formatLength;
	int16_t audioFormat;
	int16_t numChannels;
	int32_t sampleRate;
	int32_t byteRate;
	int16_t blockAlign;
	int16_t bytesPerSample;
	char dataMarker[4];
	int32_t dataSizeBytes;
	byte data[];
}wav_t;
#pragma pack (16)

const int WAVE_HEADER_SIZE_BYTES = 44;

#endif //WAVE