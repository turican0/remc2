#pragma once

#ifndef WAVE
#define WAVE

#include <cstdint>

#pragma pack (1)
typedef struct {
	char isRiff_0[4];
	int32_t fileSizeBytes_4;
	char fileType_8[4];
	char formatChunkMarker_12[4];
	int32_t formatLength_16;
	int16_t audioFormat_20;
	int16_t numChannels_22;
	int32_t sampleRate_24;
	int32_t byteRate_28;
	int16_t blockAlign_32;
	int16_t bytesPerSample_34;
	char dataMarker_36[4];
	int32_t dataSizeBytes_40;
	byte data_44[];
}wav_t;
#pragma pack (16)

const int WAVE_HEADER_SIZE_BYTES = 44;

#endif //WAVE