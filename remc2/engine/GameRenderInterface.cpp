#include "GameRenderInterface.h"
#include "LevelStructs.h"
#include "Terrain.h"
#include "../utilities/Maths.h"

// MC1 helpers shared by all renderers

RenderGame renderGame = RenderGame::MC2;

// 30D90: the original takes the noise of the stereogram from DOS memory (linear 24080h, off_9359C), the port from
// this generator; dosbox-x-remc1 writes the same there (REMC1_NOISE) to compare
static const uint8_t* Mc1StereogramNoise()
{
	static uint8_t noise[0x2100];
	static bool ready = false;
	if (!ready)
	{
		uint16_t seed = 0x1A2B;
		for (uint8_t& value : noise)
		{
			seed = (uint16_t)(seed * 0x24A1 + 0x24DF);
			value = (uint8_t)(seed >> 8);
		}
		ready = true;
	}
	return noise;
}

int Mc1ProjectionScale(uint16_t width, uint16_t height, int fov)
{
	return (int)((uint32_t)(Maths::IntegerSqrt_7277A(width * width + height * height) * fov) >> 8);
}

bool Mc1TerrainTypeFlat(uint8_t terrainType)
{
	return terrainType == 8 || terrainType == 9;
}

bool Mc1StereogramMode()
{
	return RenderMc1() && world_D41A0.m_GameSettings.m_Display.m_uiScreenSize == 2 && !world_D41A0.m_GameSettings.graphics_0x2192.xxxx_0x2193;
}

void Mc1DrawStereogram(uint8_t* screen, const uint8_t* depth, uint16_t width, uint16_t height, int pitch)
{
	const uint8_t* noiseBase = Mc1StereogramNoise();
	int half = width / 2;
	// 64 pixels of noise in the middle of every row
	uint8_t* row = screen + half - 0x40;
	for (int line = height; line; line--)
	{
		rand2_17B4E0 = (uint16_t)(rand2_17B4E0 * 0x24A1 + 0x24DF);
		const uint8_t* source = noiseBase + ((line & 0x1F) << 8) + rand2_17B4E0 % 7;
		for (int i = 0; i < 0x40; i++)
			row[i] = source[i];
		row += pitch;
	}
	// to the right: the pixel the depth shifts back
	uint8_t* target = screen + half;
	const uint8_t* shift = depth + half;
	for (int line = height; line; line--)
	{
		for (int i = 0; i < half; i++)
			target[i] = target[i - shift[i]];
		target += pitch;
		shift += pitch;
	}
	// to the left: from the pixel the depth shifts forward, starting the depth of the middle back
	target = screen + half;
	shift = depth + half;
	for (int line = height; line; line--)
	{
		uint8_t middle = shift[0];
		uint8_t* pixel = target - middle;
		const uint8_t* pixelShift = shift;
		for (int i = half - middle; i; i--)
		{
			pixel--;
			*pixel = pixel[pixelShift[-1]];
			pixelShift--;
		}
		target += pitch;
		shift += pitch;
	}
}
