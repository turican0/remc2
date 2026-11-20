#include "Mc1Graphics.h"

#include "Mc1Files.h"
#include "Mc1Game.h"
#include "Mc1Shared.h"

uint8_t colorLookup_AD168_AD158[16 * 16 * 16];
bitmap_pos_struct_t* currentFont_AE44C_AE43C = nullptr;

uint8_t FindNearestColor_5CC70_5D180(const uint8_t* palette, uint8_t red, uint8_t green, uint8_t blue)
{
	uint16_t count = (*videoMode_12F02E_12F01E & 6) ? 16 : 256;
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

uint16_t GetSpaceWidth_5A3A0_5A8B0()
{
	return currentFont_AE44C_AE43C[33].width_4;
}

uint16_t GetLineHeight_5A3B0_5A8C0()
{
	return currentFont_AE44C_AE43C[33].height_5;
}

int32_t DrawText_5A180_5A690(const char* text, int32_t posX, int32_t posY, uint8_t color)
{
	const uint8_t* c = (const uint8_t*)text;
	int32_t x = posX;
	*drawFlags_9ADFC_9ADFC = 0x40;
	if (*c)
	{
		while ((int16_t)x < 0x280)
		{
			switch (*c)
			{
			case 0x0A:
				posY += GetLineHeight_5A3B0_5A8C0();
				x = posX;
				break;
			case 0x0D:
				break;
			case 0x09:
			case 0x20:
				x += GetSpaceWidth_5A3A0_5A8B0();
				break;
			default:
				DrawGlyph_58768_58C78((int16_t)x, (int16_t)posY, currentFont_AE44C_AE43C[*c + 1], color, 1);
				x += currentFont_AE44C_AE43C[*c + 1].width_4;
				break;
			}
			if (!*++c)
				break;
		}
	}
	*drawFlags_9ADFC_9ADFC = 0;
	return x;
}
