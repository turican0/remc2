#pragma once
#ifndef ENGINE_TYPES_H
#define ENGINE_TYPES_H

#include <string>
#include <cstdint>

#ifdef USE_DOSBOX
#include "logging.h"
#include "dos_system.h"
#else
#include "../dosbox_files/config.h"
#endif //USE_DOSBOX

typedef          char   int8;
typedef   signed char   sint8;
typedef unsigned char   uint8;
typedef          short  int16;
typedef   signed short  sint16;
typedef unsigned short  uint16;
typedef          int    int32;
typedef   signed int    sint32;
typedef unsigned int    uint32;

#define x_BYTE int8
#define x_WORD int16
#define x_DWORD int32
#define x_LONG int32

typedef struct {
	uint8_t* data;
	uint8_t width;
	uint8_t height;
} posistruct_t;

#endif //ENGINE_TYPES_H