#pragma once

#ifndef GAME_RENDER_INTERFACE
#define GAME_RENDER_INTERFACE

#include <cstdint>
#include "engine_support.h"
#include "ViewPort.h"

// Texel of a reflection (DrawTriangleInProjectionSpace_B6253, x_BYTE_E126D 0x1A), texel = v * 256 + u in a tile of the BLOCK atlas.
// Why: at the edge of a triangle with v = 0 the interpolated v steps to -1 and its low byte 0xFF made the read
// [tile + 0xFF00 + u], 255 rows (~64 KB) further. For the tiles in the last rows of the atlas that is past its end:
// Level 25 (record6) at turn 9694 reads 24367 B past the 155648 B of BLOCK32 and crashed with an access violation
// where nothing was mapped behind the allocation (it depends on the heap layout: build, debugger, resolution, ...).
// Before: the read was *(x_BYTE*)(v1047 + v1055), as the original 297253 / IDA BCA2D mov al,[ebx+esi] does.
// The original reads the same address (checked in DOSBox, frames 9694 and 9697) and draws a stray texel taken from
// the DOS memory behind the atlas. Now a negative row is clamped to row 0 of the tile.
// The other way, keeping the original read: allocate the atlas 0x10000 B larger and zeroed, e.g. in
// DataFileIO::UnpackAndLoadMemoryFromPath when path.colorPalette_var28 == &BLOCK32DAT_BEGIN_BUFFER
// (Malloc_83D70(path.var36_size_buffer + 0x10000) + memset of the same size); the texel is then 0 instead of a stray one.
// GameRenderHD does not need it: it stops the line when the texel is past (texture size << 8).
inline uint8_t ReflectionTexel(int texel, const uint8_t* tile)
{
	if (texel & 0x8000)//row -1 (0xFF..) and lower
		texel &= 0xFF;
	return tile[texel];
}

class GameRenderInterface
{
public:
	virtual ~GameRenderInterface() {}
	virtual void DrawWorld_411A0(int posX, int posY, int16_t yaw, int16_t posZ, int16_t pitch, int16_t roll, int16_t fov) = 0;
};

#endif //GAME_RENDER_INTERFACE