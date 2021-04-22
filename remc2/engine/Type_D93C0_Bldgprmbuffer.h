#pragma once

#ifndef TYPE_D93C0_BLDGPRMBUFFER_H
#define TYPE_D93C0_BLDGPRMBUFFER_H

#include "stdint.h"

#pragma pack (1)
typedef struct {//lenght 4
	uint16_t word_0;
	uint8_t byte_2;
	uint8_t byte_3;
}
type_D93C0_bldgprmbuffer;
#pragma pack (16)

type_D93C0_bldgprmbuffer str_D93C0_bldgprmbuffer[76];
uint8_t str_D94F0_bldgprmbuffer[3][5] = {
	{0xAA,0x00,0x63,0x0D,0x00},
	{0x33,0x00,0x11,0x3B,0x00},
	{0x33,0x00,0x88,0x3B,0x00}
}; // lenght 5*3

#endif //TYPE_D93C0_BLDGPRMBUFFER_H