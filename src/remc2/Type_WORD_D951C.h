#pragma once
#ifndef TYPE_WORD_D951C_H
#define TYPE_WORD_D951C_H

#include "stdint.h"

#pragma pack (1)
typedef struct {//lenght 14
	uint16_t word_0;
	uint16_t word_2;
	uint16_t word_4;
	uint16_t speed_6;//shift
	uint16_t rotSpeed_8;
	uint8_t byte_10;
	int8_t byte_11;
	uint8_t byte_12;
	uint8_t byte_13;
}
type_WORD_D951C;
#pragma pack (16)

extern type_WORD_D951C str_WORD_D951C[347];

#endif //TYPE_WORD_D951C_H