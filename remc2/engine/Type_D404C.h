#pragma once

#ifndef TYPE_D404C_H
#define TYPE_D404C_H

#include "stdint.h"

#pragma pack (push,1)
typedef struct {//lenght 22
	int16_t distance_0;
	int16_t angle_2;
	int16_t height_4;
	int16_t pitch_6;
	int16_t word_8;
	int16_t zOffset_10;
	int16_t yawOffset_12;
	int16_t fov_14;
	int16_t depthOffset1_16;
	int16_t depthOffset2_18;
	int16_t depthOffset3_20;
}
type_D404C;
#pragma pack (pop)

extern type_D404C str_D404C[5];

#endif //TYPE_D404C_H