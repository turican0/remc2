#pragma once

#ifndef TYPE_F2C20_AR_H
#define TYPE_F2C20_AR_H

#include "stdint.h"

#include "global_types.h"

#pragma pack (push,1)
typedef struct {
	int32_t fogShade_0x00;
	int32_t visibility_0x01;
	int8_t* texture_0x02;
	int32_t screenX_0x03;
	int32_t screenY_0x04;
	int32_t signedWidth_0x05;
	int32_t height_0x06;
	int32_t tint_0x07;
	int32_t width_0x08;
	int32_t realWidth_0x09;
	int32_t rowIndex_0x0a;
	int32_t columnIndex_0x0b;
	int32_t realHeight_0x0c;
	int32_t sin_0x0d;
	int32_t renderBufferStart_0x0e;
	int32_t cos2_0x0f;
	int32_t centerY_0x10;
	int32_t cos_0x11;
	int32_t fogThickness_0x12;
	int32_t fogStart_0x13;
	type_entity_0x6E8E* entity_0x14x;
	int32_t tileCutOffDistance_0x15;
	int32_t fogEnd_0x16;
	int32_t sin2_0x17;
	int32_t projectionScale_0x18;
	int32_t Height_0x19;
	uint8_t* pbyte0x1a;
	int32_t cos_0x1b;
	int32_t maxColumns_0x1c;
	int32_t rows_0x1d;
	int32_t octant_0x1e;
	int32_t skew_0x1f;
	int32_t cameraZ_0x20;
	int32_t minScreenX_0x21;
	int32_t horizonOffset_0x22;
	int32_t stride_0x23;
	int32_t centerX_0x24;
	int32_t width0x25;
	int32_t height0x26;
	int32_t sin_0x27;
}type_F2C20ar;
#pragma pack (pop)

#endif //TYPE_F2C20_AR_H