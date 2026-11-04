#pragma once

#ifndef TYPE_D93C0_BLDGPRMBUFFER_H
#define TYPE_D93C0_BLDGPRMBUFFER_H

#include "stdint.h"
#include <array>

#pragma pack (push,1)
typedef struct {//lenght 4
	uint16_t subSpellIndex_0;
	uint8_t flags_2;
	uint8_t nextIndex_3;
}
type_D93C0_bldgprmbuffer;
#pragma pack (pop)

extern std::array<type_D93C0_bldgprmbuffer, 77> buildingParams_D93C0; // size? at least index 76 is accessed

#endif //TYPE_D93C0_BLDGPRMBUFFER_H