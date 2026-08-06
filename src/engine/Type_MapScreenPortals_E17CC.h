#pragma once

#ifndef TYPE_MAPSCREENPORTALS_E17CC_H
#define TYPE_MAPSCREENPORTALS_E17CC_H

#include "stdint.h"

#pragma pack(push, 1)
typedef struct {//lenght 22
	int32_t time_0;
	int16_t viewPortPosX_4;
	int16_t viewPortPosY_6;
	int16_t word_8;
	int16_t word_10;
	int16_t portalPosX_12; //X
	int16_t portalPosY_14; //Y
	int16_t spriteIndex_16;
	int8_t activated_18;//2-hidden 1-activated
	int8_t byte_19;
	int16_t word_20;
}Type_MapScreenPortals_E17CC;
#pragma pack(pop)

extern Type_MapScreenPortals_E17CC mapScreenPortals_E17CC[26];

#endif //TYPE_MAPSCREENPORTALS_E17CC_H