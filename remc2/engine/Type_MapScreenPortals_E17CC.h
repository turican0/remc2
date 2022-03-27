#pragma once

#ifndef TYPE_MAPSCREENPORTALS_E17CC_H
#define TYPE_MAPSCREENPORTALS_E17CC_H

#include "stdint.h"

#pragma pack (1)
typedef struct {//lenght 22
	int16_t word_0;
	int16_t word_2;
	int16_t word_4;
	int16_t word_6;
	int16_t word_8;
	int16_t word_10;
	int16_t portalPosX_12; //X
	int16_t portalPosY_14; //Y
	int16_t word_16;
	int8_t activated_18;//2-hidden 1-activated
	int8_t byte_19;
	int16_t word_20;
}Type_MapScreenPortals_E17CC;
#pragma pack (16)

extern Type_MapScreenPortals_E17CC mapScreenPortals_E17CC[26];

#endif //TYPE_MAPSCREENPORTALS_E17CC_H