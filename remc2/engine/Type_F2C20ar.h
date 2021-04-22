#pragma once

#ifndef TYPE_F2C20_AR_H
#define TYPE_F2C20_AR_H

#include "stdint.h"
#include "engine_support.h"

#pragma pack (1)
typedef struct {
	int32_t dword0x00;
	int32_t dword0x01_rotIdx;
	int8_t* dword0x02_data;
	int32_t dword0x03_screenX;
	int32_t dword0x04_screenY;
	int32_t dword0x05;
	int32_t dword0x06_height;
	int32_t dword0x07;
	int32_t dword0x08_width;
	int32_t dword0x09_realWidth;
	int32_t dword0x0a_actIdx;
	int32_t dword0x0b;
	int32_t dword0x0c_realHeight;
	int32_t dword0x0d;
	int32_t dword0x0e_ptrScreenRenderBufferStart;
	int32_t dword0x0f;
	int32_t dword0x10;
	int32_t dword0x11;
	int32_t dword0x12;
	int32_t dword0x13;
	type_event_0x6E8E* dword0x14x;
	int32_t dword0x15;
	int32_t dword0x16;
	int32_t dword0x17;
	int32_t dword0x18;
	int32_t Height_0x19;
	uint8_t* pbyte0x1a;
	int32_t dword0x1b;
	int32_t dword0x1c;
	int32_t dword0x1d;
	int32_t dword0x1e;
	int32_t dword0x1f;
	int32_t dword0x20;
	int32_t dword0x21;
	int32_t dword0x22;
	int32_t dword0x23;
	int32_t dword0x24;
	int32_t width0x25;
	int32_t height0x26;
	int32_t dword0x27;
}type_F2C20ar;
#pragma pack (16)

#endif //TYPE_F2C20_AR_H