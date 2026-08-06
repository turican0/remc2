#pragma once
#ifndef BITMAP_POS_STRUCT_H
#define BITMAP_POS_STRUCT_H

#include <string>
#include <cstdint>

#if defined(WIN32) || defined(_WIN64)
# pragma warning(disable:4996)
#endif

#ifdef USE_DOSBOX
#include "logging.h"
#include "dos_system.h"
#endif //USE_DOSBOX

#define x_BYTE char
#define x_WORD short
#define x_DWORD int

#pragma pack(push, 1)
typedef struct {
	uint8_t* data;
	uint8_t width_4;
	uint8_t height_5;
} bitmap_pos_struct_t;

typedef struct {
	uint32_t data_0;
	uint8_t width_4;
	uint8_t height_5;
} bitmap_pos_struct2_t;
#pragma pack (pop)

#endif //BITMAP_POS_STRUCT_H