#pragma once

#ifndef TYPE_SECRETMAPSCREENPORTALS_E2970_H
#define TYPE_SECRETMAPSCREENPORTALS_E2970_H

#include "stdint.h"

#pragma pack (1)
typedef struct {//lenght 17
	int32_t dword_0;
	uint16_t word_4;
	uint16_t levelNumber_6;//level number
	uint16_t word_8;
	uint16_t word_10;
	uint16_t activated_12; //3-hidden 1-activated
	uint16_t word_14;
	uint8_t byte_16;
}Type_SecretMapScreenPortals_E2970;
#pragma pack (16)

extern Type_SecretMapScreenPortals_E2970 secretMapScreenPortals_E2970[6];

#endif //TYPE_SECRETMAPSCREENPORTALS_E2970_H
