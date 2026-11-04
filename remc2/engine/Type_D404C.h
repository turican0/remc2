#pragma once

#ifndef TYPE_D404C_H
#define TYPE_D404C_H

#include "stdint.h"

#pragma pack (push,1)
typedef struct {//lenght 22
	int16_t distance_0;
	int16_t angle_2;
	int16_t height_4;
	int16_t word_6;
	int16_t word_8;
	int16_t word_10;
	int16_t word_12;
	int16_t word_14;
	int16_t word_16;
	int16_t word_18;
	int16_t word_20;
}
type_D404C;
#pragma pack (pop)

extern type_D404C str_D404C[5];

#endif //TYPE_D404C_H