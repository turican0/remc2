#pragma once

#ifndef TYPE_ANIMATIONS_H
#define TYPE_ANIMATIONS_H

#include "stdint.h"
#include "engine_support.h"

#pragma pack (1)
typedef struct {//lenght 28
	int32_t dword_0;
	type_particle_str* Particles_4;//mayby type_particle_str*
	int32_t dword_8;
	int16_t word_12;
	uint16_t word_14;
	int16_t CountOfFrames_16;
	int16_t Width_18;//width
	int16_t Height_20;//height
	int16_t FrameIndex_22;//anim frame index
	int16_t word_24;
	int16_t word_26;
}
type_animations1;

typedef struct {//lenght 6
	int16_t word_0;
	type_animations1* dword_2;
}
type_E9C08;
#pragma pack (16)

#endif //TYPE_ANIMATIONS_H

