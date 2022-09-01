#pragma once

#ifndef TYPE_D93C0_BLDGPRMBUFFER_H
#define TYPE_D93C0_BLDGPRMBUFFER_H

#include "stdint.h"
#include <array>

#pragma pack (1)
typedef struct {//lenght 4
	uint16_t word_0;
	uint8_t byte_2;
	uint8_t byte_3;
}
type_D93C0_bldgprmbuffer;
#pragma pack (16)

extern std::array<type_D93C0_bldgprmbuffer, 77> str_D93C0_bldgprmbuffer; // size? at least index 76 is accessed

#endif //TYPE_D93C0_BLDGPRMBUFFER_H