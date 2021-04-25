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

extern type_D93C0_bldgprmbuffer str_D93C0_bldgprmbuffer[76];

#endif //TYPE_D93C0_BLDGPRMBUFFER_H