#include "Mc1Graphics.h"

#include "Mc1Files.h"
#include "Mc1Game.h"

uint8_t colorLookup_AD168_AD158[16 * 16 * 16];
bitmap_pos_struct_t* currentFont_AE44C_AE43C = nullptr;

uint8_t FindNearestColor_5CC70_5D180(const uint8_t* palette, uint8_t red, uint8_t green, uint8_t blue)
{
	uint16_t count = (videoMode_12F02E_12F01E & 6) ? 16 : 256;
	int32_t bestDistance = 9999;
	//!!!!! the original returns an uninitialized value when no color is closer than 9999
	uint8_t bestIndex = 0;
	const uint8_t* color = palette;
	for (int16_t index = 0; index < count; index++, color += 3)
	{
		int32_t dg = green - color[1];
		int32_t dr = red - color[0];
		int32_t db = blue - color[2];
		int32_t distance = dg * dg + dr * dr + db * db;
		if ((int16_t)distance < (int16_t)bestDistance)
		{
			bestDistance = distance;
			bestIndex = (uint8_t)index;
		}
	}
	return bestIndex;
}

void SetFont_5A3C0_5A8D0(int16_t index)
{
	// the original keeps the relocated tab, the port the index built from it
	bitmap_pos_struct_t* font = fontIndexes[index];
	if (!font)
		font = fontIndexes[0];
	currentFont_AE44C_AE43C = font;
}
