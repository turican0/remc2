#pragma once
#include <cstdint>

#pragma pack (1)
typedef struct {//32
	uint32_t configDatSign_0;
	uint16_t langIndex_4;
	uint16_t soundVolume_6;
	uint16_t musicVolume_8;
	uint8_t byteindex_10;
	int8_t brightness_11;
	int8_t brightness_12;
	int8_t brightness_13;
	uint16_t wordindex_14;
	uint32_t dwordindex_16;
	uint8_t stubb[2];
	uint8_t keys[10];
} TypeConfigDat;
#pragma pack (16)