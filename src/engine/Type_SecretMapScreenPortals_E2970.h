#pragma once

#ifndef TYPE_SECRETMAPSCREENPORTALS_E2970_H
#define TYPE_SECRETMAPSCREENPORTALS_E2970_H

#include "stdint.h"

#pragma pack(push, 1)
typedef struct {//lenght 17
	int32_t time_0;
	uint16_t index_4;
	uint16_t levelNumber_6;//level number
	uint16_t posX_8;
	uint16_t posY_10;
	uint16_t activated_12; //3-hidden 1-activated
	uint16_t spriteIndex_14;
	uint8_t byte_16;
}Type_SecretMapScreenPortals_E2970;
#pragma pack(pop)

extern Type_SecretMapScreenPortals_E2970 secretMapScreenPortals_E2970[6];

#endif //TYPE_SECRETMAPSCREENPORTALS_E2970_H
