#pragma once

#ifndef TYPE_E9C38_SMALLTIT_H
#define TYPE_E9C38_SMALLTIT_H

#include "stdint.h"

#pragma pack (1)
typedef struct {//lenght 44
	int32_t x_0;
	int32_t alt_4;//altitude
	int32_t alt2_8;//?
	int32_t y_12;//?
	int32_t pnt1_16;//triangleB
	int32_t pnt2_20;//triangleB
	int32_t pnt3_24;//triangleA
	int32_t pnt4_28;//triangleA
	int32_t pnt5_32;//triangleA and triangleB
	int16_t haveBillboard_36;//? billboard
	uint16_t triangleFeatures_38;//0x1- |/| |\| 0x1000 -flat
	int8_t var_40;
	uint8_t textIndex_41;//texture index
	int8_t textUV_42;//texture direction
	bool textAtyp_43;
}
type_E9C38_smalltit;
#pragma pack (16)

#endif //TYPE_E9C38_SMALLTIT_H