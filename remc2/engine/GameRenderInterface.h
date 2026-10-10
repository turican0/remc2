#pragma once

#ifndef GAME_RENDER_INTERFACE
#define GAME_RENDER_INTERFACE

#include <cstdint>
#include "engine_support.h"
#include "ViewPort.h"

inline uint8_t ClampReflectionTexel(int texel, const uint8_t* tile)
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