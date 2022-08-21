#pragma once

#ifndef TYPE_E9C38_SMALLTIT_H
#define TYPE_E9C38_SMALLTIT_H

#include "stdint.h"

#pragma pack (1)
typedef struct {//lenght 44
	int32_t dword0_rot;
	int32_t dword4_height;//Height
	int32_t dword8;//?
	int32_t dword12;//?
	int32_t dword16;//triangleB
	int32_t dword20;//triangleB
	int32_t dword24;//triangleA
	int32_t dword28;//triangleA
	int32_t dword32;//triangleA and triangleB
	int16_t word36;//? billboard
	int16_t word38;
	int8_t byte40;
	uint8_t byte41;//texture index
	int8_t byte42_std;//texture direction
	int8_t byte43;
}
type_E9C38_smalltit;
#pragma pack (16)

#endif //TYPE_E9C38_SMALLTIT_H