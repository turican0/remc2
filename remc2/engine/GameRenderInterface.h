#pragma once

#ifndef GAME_RENDER_INTERFACE
#define GAME_RENDER_INTERFACE

#include <cstdint>
#include "engine_support.h"
#include "ViewPort.h"

enum class SpritePass : uint32_t
{
	Shadow = 0,
	Main = 1,
	Reflection = 2
};

inline uint8_t ClampReflectionTexel(int texel, const uint8_t* tile)
{
	if (texel & 0x8000)//row -1 (0xFF..) and lower
		texel &= 0xFF;
	return tile[texel];
};

// the game the renderers draw: MC1 (Magic Carpet 1 / Hidden Worlds) switches its differences on, MC2 stays as it was
enum class RenderGame : uint8_t
{
	MC2 = 0,
	MC1 = 1
};

extern RenderGame renderGame;

inline bool RenderMc1()
{
	return renderGame != RenderGame::MC2;
}

// MC1 helpers shared by the renderers (GameRenderInterface.cpp)
int Mc1ProjectionScale(uint16_t width, uint16_t height, int fov); // 2A700: isqrt(w*w+h*h)*fov >> 8 (MC2 7*...>>11)
bool Mc1TerrainTypeFlat(uint8_t terrainType); // 90168: the types shaded flat (mode 7), MC2 uses the flat setting
bool Mc1StereogramMode(); // screen size 2 without VFX1: the world is a depth image turned into a random dot stereogram
inline int Mc1StereogramShade(int depth) // 2A700 / 2F170: the depth as the shade of the depth image
{
	return 0x400000 - (0x1400 - depth) * 0x15E;
}
// 30D90: the random dot stereogram from the depth image (width/height of the view port, rows by pitch)
void Mc1DrawStereogram(uint8_t* screen, const uint8_t* depth, uint16_t width, uint16_t height, int pitch);

class GameRenderInterface
{
public:
	virtual ~GameRenderInterface() {}
	virtual void DrawWorld_411A0(int posX, int posY, int16_t yaw, int16_t posZ, int16_t pitch, int16_t roll, int16_t fov) = 0;
};

#endif //GAME_RENDER_INTERFACE