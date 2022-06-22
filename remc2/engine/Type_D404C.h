#pragma once

#ifndef TYPE_D404C_H
#define TYPE_D404C_H

#include "stdint.h"

#pragma pack (1)
typedef struct {//lenght 22
	int16_t word_0;
	int16_t word_2;
	int16_t word_4;
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
#pragma pack (16)

extern type_D404C str_D404C[5];

#endif //TYPE_D404C_H