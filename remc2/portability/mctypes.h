#pragma once
#ifndef ENGINE_TYPES_H
#define ENGINE_TYPES_H

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

#pragma pack (1)
typedef struct {
	uint8_t* data;
	uint8_t width;
	uint8_t height;
} posistruct_t;

typedef struct {
	uint32_t data;
	uint8_t width;
	uint8_t height;
} posistruct2_t;
#pragma pack (16)

#endif //ENGINE_TYPES_H