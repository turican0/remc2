#include "Mc1Panel.h"

#include <cstdio>
#include <cstring>

#include "Mc1Blocks.h"
#include "Mc1Files.h"
#include "Mc1Game.h"
#include "Mc1Graphics.h"
#include "Mc1Minimap.h"
#include "Mc1Screen.h"
#include "Mc1Shared.h"

static Type_Mc1Entity* Entities()
{
	return mc1Entities;
}

static int16_t ReadS16(const uint8_t* data)
{
	int16_t value;
	memcpy(&value, data, sizeof(value));
	return value;
}

static uint16_t ReadU16(const uint8_t* data)
{
	uint16_t value;
	memcpy(&value, data, sizeof(value));
	return value;
}

static int32_t ReadS32(const uint8_t* data)
{
	int32_t value;
	memcpy(&value, data, sizeof(value));
	return value;
}

static bool LowRes()
{
	return (*videoMode_12F02E_12F01E & 1) != 0;
}

static void DrawSprite(int32_t x, int32_t y, const bitmap_pos_struct_t& sprite)
{
	if (LowRes())
		DrawBitmap320_60CE0_611F0((int16_t)x, (int16_t)y, sprite, 1);
	else
		DrawBitmap640_60D18_61228((int16_t)x, (int16_t)y, sprite, 1);
}

// 23940 / 23AE0: the sprite blended with the screen (BB934 = B7934 + 4000h, [screen][pixel]); 23AE0 blends the colour
// in the shape of the sprite, without the checks. In 320x200 (videoMode 1) the position and the rows are halved,
// the pixels of the rows are not (the upper half of the sprite)
static void BlendSprite(int32_t x, int32_t y, const bitmap_pos_struct_t& sprite, int32_t color)
{
	const bool low = *videoMode_12F02E_12F01E == 1;
	if (color < 0)
	{
		if ((int16_t)x < 0 || (int16_t)y < 0 || (int16_t)x + sprite.width_4 > 0x280
			|| (int16_t)y + sprite.height_5 > (low ? 0x190 : 0x1E0))
			return;
	}
	const uint32_t pitch = *screenWidth_12EFF0_12EFE0;
	int32_t rows = sprite.height_5;
	uint8_t* row;
	if (low)
	{
		rows /= 2;
		row = *workScreen_12EFF4_12EFE4 + pitch * ((int16_t)y / 2) + (int16_t)x / 2;
	}
	else
		row = *workScreen_12EFF4_12EFE4 + pitch * (int16_t)y + (int16_t)x;
	uint8_t* pixel = row;
	const uint8_t* data = sprite.data;
	while ((int16_t)rows)
	{
		const int8_t code = (int8_t)*data++;
		if (!code)
		{
			row += pitch;
			pixel = row;
			rows--;
		}
		else if (code < 0)
			pixel -= code;
		else
		{
			for (int32_t i = 0; i < code; i++, pixel++)
				*pixel = tables_B7934_B7924.blend_0x4000[*pixel][color < 0 ? data[i] : (uint8_t)color];
			data += code;
		}
	}
}

// 24C20 / 24CA0 (61640 / 616C0): the rectangle within 640x400 or 640x480, at least 2x2; the low resolution halves it.
// The translucent fill (9ADFC & 4) is set only by the texts
static void FillRect(int32_t x, int32_t y, int32_t width, int32_t height, uint8_t color)
{
	const bool low = LowRes();
	if (width < 2 || height < 2 || x < 0 || x + width >= 0x280 || y < 0 || y + height >= (low ? 0x190 : 0x1E0))
		return;
	const uint32_t pitch = *screenWidth_12EFF0_12EFE0; // DOS 140h / 280h
	uint16_t columns = (uint16_t)width;
	uint16_t rows = (uint16_t)height;
	uint8_t* row;
	if (low)
	{
		columns >>= 1;
		rows >>= 1;
		row = *workScreen_12EFF4_12EFE4 + pitch * ((uint16_t)y >> 1) + ((uint16_t)x >> 1);
	}
	else
		row = *workScreen_12EFF4_12EFE4 + pitch * (uint16_t)y + (uint16_t)x;
	for (; rows; rows--, row += pitch)
		memset(row, color, columns);
}

// 22810: the bar of the value up to the width
static void DrawBar(int32_t x, int32_t y, int32_t width, int32_t height, int32_t value, uint8_t color)
{
	if (value >= width)
		value = width;
	if (value < 2 || height < 2)
		return;
	FillRect(x, y, value, height, color);
}

// 61594 / 615D4: a pixel within 640x400 or 640x480
static void PutPixel(int32_t x, int32_t y, uint8_t color)
{
	const uint32_t pitch = *screenWidth_12EFF0_12EFE0;
	if (LowRes())
	{
		if ((uint16_t)x >= 0x280 || (uint16_t)y >= 0x190)
			return;
		(*workScreen_12EFF4_12EFE4)[pitch * ((uint16_t)y >> 1) + ((uint16_t)x >> 1)] = color;
	}
	else
	{
		if ((uint16_t)x >= 0x280 || (uint16_t)y >= 0x1E0)
			return;
		(*workScreen_12EFF4_12EFE4)[pitch * (uint16_t)y + (uint16_t)x] = color;
	}
}

// 247C0: the rectangle faded by the table (B7934, [fade][screen])
static void FadeRect(int32_t x, int32_t y, int32_t width, int32_t height, uint8_t fade)
{
	if (*videoMode_12F02E_12F01E == 1)
	{
		x = (int16_t)x / 2;
		y = (int16_t)y / 2;
		width = (int16_t)width / 2;
		height = (int16_t)height / 2;
	}
	const uint32_t pitch = *screenWidth_12EFF0_12EFE0;
	for (int16_t row = (int16_t)y; row < (int16_t)y + (int16_t)height; row++)
		for (int16_t column = (int16_t)x; column < (int16_t)x + (int16_t)width; column++)
		{
			uint8_t& pixel = (*workScreen_12EFF4_12EFE4)[pitch * row + column];
			pixel = tables_B7934_B7924.fades_0x0000[fade][pixel];
		}
}

void DrawPlayerPanel_22E50_22E50(Type_Mc1Entity* wizard)
{
	const intptr_t index = wizard - Entities();
	if (!index || index >= mc1EntityCount)
		return;
	Type_Mc1Settings_AE408_AE3F8* settings = mc1Settings_AE408_AE3F8;
	uint8_t* playerData = wizard->playerData_0xA0_160;
	const int16_t player = ReadS16(playerData + 0x30);
	const uint8_t mana = playerColors_99B58_99B58[player][0];
	const uint8_t maxMana = playerColors_99B58_99B58[player][1];
	const uint8_t mark = (&playerColors_99B58_99B58[0][0])[settings->colorIndex_0x5D_93[2] + player * 2];
	const uint8_t life = 0x7B;
	const int32_t y = 2;
	// the frame of the panel, then the cells of the castle, the balloons and the wizard
	BlendSprite(2, y, spritesIndex[40], -1);
	int32_t x = spritesIndex[40].width_4 + 2;
	// the mana percent to win (word at 9790h of the level, 415C0)
	const uint16_t targetPercent = ReadU16((const uint8_t*)&mc1State_AE400_AE3F0->level_0x2F503_193795 + 0x9790);
	Type_Mc1Entity* castle = &Entities()[ReadU16(playerData + 0x32)];
	if (castle > Entities() && castle->word_0x1A_26 > 0)
	{
		if ((int8_t)playerData[0x187] > 0 && settings->colorIndex_0x5D_93[1])
		{
			BlendSprite(x, y, spritesIndex[55], -1);
			playerData[0x187]--;
		}
		else
			BlendSprite(x, y, spritesIndex[41], -1);
		DrawSprite(x + 2, y, spritesIndex[castle->word_0x1A_26 + 0x2B]);
		DrawSprite(x + 0x26, y, spritesIndex[42]);
		DrawBar(x + 0x3A, y + 8, 0x40, 10, castle->life_0x0C_12 * 64 / castle->maxLife_0x08_8, life);
		// the mana of the castle and of the balloons (pd+134h) of the mana of the level; blinking when full
		const int32_t total = settings->manaStats_0xB8_184.total_0x04;
		const int32_t held = castle->mana_0x8C_140 + ReadS32(playerData + 0x134);
		if (held == castle->dword_0x88_136)
		{
			if (settings->colorIndex_0x5D_93[2] == 1)
				DrawBar(x + 0x3A, y + 0x1A, 0x40, 10, castle->dword_0x88_136 * 64 / total, maxMana);
			else
				DrawBar(x + 0x3A, y + 0x1A, 0x40, 10, held * 64 / total, mana);
		}
		else
		{
			DrawBar(x + 0x3A, y + 0x1A, 0x40, 10, castle->dword_0x88_136 * 64 / total, maxMana);
			DrawBar(x + 0x3A, y + 0x1A, 0x40, 10, held * 64 / total, mana);
		}
		if (!(settings->flags_0x00_0 & 0x10))
		{
			FillRect(x + 0x3A + targetPercent * 0x40 / 100, y + 0x18, 2, 2, mark);
			FillRect(x + 0x3A + targetPercent * 0x40 / 100, y + 0x1A + 10, 2, 2, mark);
		}
	}
	else
		BlendSprite(x, y, spritesIndex[54], -1);
	x += spritesIndex[41].width_4;
	if (castle > Entities())
	{
		if ((int8_t)playerData[0x189] > 0 && settings->colorIndex_0x5D_93[1])
		{
			BlendSprite(x, y, spritesIndex[55], -1);
			playerData[0x189]--;
		}
		else
			BlendSprite(x, y, spritesIndex[41], -1);
		// the balloons by the size of the castle
		int16_t balloons;
		switch ((uint16_t)castle->word_0x1A_26)
		{
		case 1: case 2: case 3:
			balloons = 1;
			break;
		case 4: case 5:
			balloons = 2;
			break;
		case 6: case 7:
			balloons = 3;
			break;
		default:
			balloons = 0;
			break;
		}
		if (balloons)
		{
			DrawSprite(x + 2, y, spritesIndex[balloons + 0x32]);
			DrawSprite(x + 0x26, y, spritesIndex[42]);
		}
		for (int16_t i = 0; i < balloons; i++)
		{
			Type_Mc1Entity* balloon = &Entities()[ReadU16(playerData + 0x34 + i * 2)];
			if (balloon <= Entities())
				continue;
			if (balloon->life_0x0C_12 >= 0)
				DrawBar(x + 0x3A, y + 0x0A + i * 2, 0x40, 2, balloon->life_0x0C_12 * 0x40 / balloon->maxLife_0x08_8, life);
			DrawBar(x + 0x3A, y + 0x1C + i * 2, 0x40, 2, balloon->mana_0x8C_140 * 0x40 / balloon->dword_0x88_136, mana);
		}
	}
	else
		BlendSprite(x, y, spritesIndex[54], -1);
	x += spritesIndex[41].width_4;
	if ((int8_t)playerData[0x188] > 0 && settings->colorIndex_0x5D_93[1])
	{
		BlendSprite(x, y, spritesIndex[55], -1);
		playerData[0x188]--;
	}
	else
		BlendSprite(x, y, spritesIndex[41], -1);
	// the wizard: blinking while hit (flags 30h) or pd+14Bh
	if (((wizard->flags_0x10_16 & 0x30) || ReadS16(playerData + 0x14B)) && settings->colorIndex_0x5D_93[1])
	{
		BlendSprite(x + 2, y, spritesIndex[43], -1);
		BlendSprite(x + 0x26, y, spritesIndex[42], -1);
	}
	else
	{
		DrawSprite(x + 2, y, spritesIndex[43]);
		DrawSprite(x + 0x26, y, spritesIndex[42]);
	}
	DrawBar(x + 0x3A, y + 8, 0x40, 10, wizard->life_0x0C_12 * 64 / wizard->maxLife_0x08_8, life);
	DrawBar(x + 0x3A, y + 0x1A, 0x40, 10, wizard->dword_0x88_136 * 64 / settings->manaStats_0xB8_184.total_0x04, maxMana);
	DrawBar(x + 0x3A, y + 0x1A, 0x40, 10, wizard->mana_0x8C_140 * 64 / settings->manaStats_0xB8_184.total_0x04, mana);
	if (!(settings->flags_0x00_0 & 0x10))
	{
		FillRect(x + 0x3A + targetPercent * 0x40 / 100, y + 0x18, 2, 2, mark);
		FillRect(x + 0x3A + targetPercent * 0x40 / 100, y + 0x1A + 10, 2, 2, mark);
	}
}

// 23D40 / 24230: the key of the spell, in another mode or while pd+34Ch (by the model) counts down; darkened while the
// castle has less mana than the spell needs (dword_0x84)
static void DrawSpellKey(int32_t x, int32_t y, Type_Mc1Entity* spell, Type_Mc1Entity* owner, const bitmap_pos_struct_t& cell)
{
	uint8_t* playerData = owner->playerData_0xA0_160;
	Type_Mc1Player_340B& ownerPlayer = mc1State_AE400_AE3F0->players_0x340B_13323[ReadS16(playerData + 0x30)];
	if (ownerPlayer.mode_0x44A_1098 || (int8_t)playerData[0x34C + (int8_t)spell->model_0x41_65] > 0)
	{
		playerData[0x34C + (int8_t)spell->model_0x41_65]--;
		for (int32_t key = 0; key < 10; key++)
		{
			const int8_t slot = (int8_t)playerData[0x304 + key];
			if (slot == -1)
				continue;
			if (&Entities()[ReadS32(playerData + 0x214 + slot * 4)] != spell)
				continue;
			BlendSprite(x, y, spritesIndex[key + 0x1E], colorLookup_AD168_AD158[0]);
			break;
		}
	}
	if (spell->dword_0x84_132)
	{
		const uint16_t castle = ReadU16(playerData + 0x32);
		if (!castle || spell->dword_0x84_132 > Entities()[castle].mana_0x8C_140)
			FadeRect(x, y, cell.width_4, cell.height_5, 0x30);
	}
}

void DrawSpellIcon_23D40_23D40(int32_t x, int32_t y, Type_Mc1Entity* spell, int32_t selected)
{
	if (spell <= Entities())
		return;
	SetFont_5A3C0_5A8D0(1);
	Type_Mc1Entity* owner = &Entities()[spell->word_0x2A_42];
	if (owner <= Entities())
		return;
	const int16_t player = ReadS16(owner->playerData_0xA0_160 + 0x30);
	const uint8_t dot = playerColors_99B58_99B58[player][0];
	const uint8_t bar = playerColors_99B58_99B58[player][1];
	// blinking while the spell recovers (word_0x30 below 40h)
	if (spell->word_0x30_48 < 0x40 && spell->word_0x30_48 && spell->word_0x32_50 > 0x40 && mc1Settings_AE408_AE3F8->colorIndex_0x5D_93[1])
		return;
	const bitmap_pos_struct_t& back = spritesIndex[spell->word_0x30_48 ? 2 : 1];
	if (selected)
		DrawSprite(x, y, back);
	else
		BlendSprite(x, y, back, -1);
	DrawSprite(x, y, spritesIndex[(int8_t)spell->model_0x41_65 + 6]);
	// the rest of the mana of the owner to the next cast as the bar, the casts as the dots (two in a column)
	FillRect(x + 4, y + 0x24, owner->mana_0x8C_140 % spell->dword_0x88_136 * 0x38 / spell->dword_0x88_136, 4, bar);
	int32_t casts = owner->mana_0x8C_140 / spell->dword_0x88_136;
	for (int32_t column = 0; column < 0x1B && casts; column++)
		for (int32_t i = 0; i < 2 && casts; i++, casts--)
			PutPixel(x + 4 + column * 2, y + 0x24 + i * 2, dot);
	// the charge of the spell
	if (spell->byte_0x3E_62 && (int8_t)spell->byte_0x3D_61 > 0)
		FillRect(x + 4, y + 0x24, (int8_t)spell->byte_0x3D_61 * 0x37 / (int8_t)spell->byte_0x3E_62, 4, colorLookup_AD168_AD158[0xF0]);
	DrawSpellKey(x, y, spell, owner, spritesIndex[1]);
}

void DrawSpellAbsent_23CF0_23CF0(int32_t x, int32_t y, int32_t model)
{
	BlendSprite(x, y, spritesIndex[3], -1);
	BlendSprite(x, y, spritesIndex[model + 6], 0xA6);
}

void DrawSpellCell_24230_24230(int32_t x, int32_t y, Type_Mc1Entity* spell, int32_t selected)
{
	if (spell <= Entities())
		return;
	SetFont_5A3C0_5A8D0(1);
	Type_Mc1Entity* owner = &Entities()[spell->word_0x2A_42];
	if (owner <= Entities())
		return;
	if (spell->word_0x30_48 < 0x40 && spell->word_0x30_48 && spell->word_0x32_50 > 0x40 && mc1Settings_AE408_AE3F8->colorIndex_0x5D_93[1])
		return;
	const bitmap_pos_struct_t& back = spritesIndex[spell->word_0x30_48 ? 4 : 3];
	if (selected)
		DrawSprite(x, y, back);
	else
		BlendSprite(x, y, back, -1);
	DrawSprite(x, y, spritesIndex[(int8_t)spell->model_0x41_65 + 6]);
	DrawSpellKey(x, y, spell, owner, spritesIndex[3]);
}

// 24D20 / 24DA0 (61740 / 61810): the frame of the rectangle within 640x400 or 640x480 by four lines
static void FrameRect(int32_t x, int32_t y, int32_t width, int32_t height, uint16_t color, bool low)
{
	if (width < 2 || height < 2 || x < 0 || x + width >= 0x280 || y < 0 || y + height >= (low ? 0x190 : 0x1E0))
		return;
	auto line = low ? DrawLine320_6B260_6B770 : DrawLine640_6B378_6B888;
	width--;
	height--;
	line((int16_t)x, (int16_t)y, (int16_t)(x + width), (int16_t)y, color, 0);
	line((int16_t)(x + width), (int16_t)y, (int16_t)(x + width), (int16_t)(y + height + 1), color, 0);
	line((int16_t)x, (int16_t)(y + height), (int16_t)(x + width + 1), (int16_t)(y + height), color, 0);
	line((int16_t)x, (int16_t)y, (int16_t)x, (int16_t)(y + height), color, 0);
}

void FrameRect320_24D20_24D20(int32_t x, int32_t y, int32_t width, int32_t height, int32_t color)
{
	FrameRect(x, y, width, height, (uint16_t)color, true);
}

void FrameRect640_24DA0_24DA0(int32_t x, int32_t y, int32_t width, int32_t height, int32_t color)
{
	FrameRect(x, y, width, height, (uint16_t)color, false);
}

void DrawPlayersTable_22880_22880()
{
	// the table of the active players in the middle of the map: a row a player with its name and mana (sprite 85), a
	// column of the kills (pd+1Eh) for each of the 8 players (sprite 86); the width counts only the active players
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	SetFont_5A3C0_5A8D0(1);
	int32_t count = 0;
	for (int32_t i = 0; i < state->playerCount_0x0A_10; i++)
		if (state->players_0x340B_13323[i].active_0x06_6)
			count++;
	const bitmap_pos_struct_t& head = spritesIndex[85];
	const bitmap_pos_struct_t& cell = spritesIndex[86];
	const int32_t x0 = (0x280 - (cell.width_4 * count + head.width_4)) / 2;
	int32_t y = ((*videoMode_12F02E_12F01E == 1 ? 0x190 : 0x1E0) - head.height_5 * count) / 2;
	char text[0x100];
	for (int32_t row = 0; row < state->playerCount_0x0A_10; row++)
	{
		Type_Mc1Player_340B& player = state->players_0x340B_13323[row];
		if (player.active_0x06_6 != 1)
			continue;
		Type_Mc1Entity* wizard = &Entities()[player.entityIndex_0x0A_10];
		int32_t x = x0;
		DrawSprite(x, y, head);
		FillRect(x + 4, y + 4, head.width_4 - 8, head.height_5 - 8, playerColors_99B58_99B58[row][0]);
		DrawText_5A180_5A690(player.name_0x40A_1034, x + 8, (int16_t)(y + 6), playerColors_99B58_99B58[row][1]);
		sprintf(text, "%d", wizard->dword_0x88_136);
		DrawText_5A180_5A690(text, x + 8, (int16_t)(y + 0x14), playerColors_99B58_99B58[row][1]);
		x += head.width_4;
		for (int32_t column = 0; column < 8; column++, x += cell.width_4)
		{
			if (column != row && state->players_0x340B_13323[column].active_0x06_6 == 1)
			{
				DrawSprite(x, y, cell);
				FillRect(x + 4, y + 4, cell.width_4 - 8, cell.height_5 - 8, playerColors_99B58_99B58[column][0]);
				sprintf(text, "%03d", ReadS16(wizard->playerData_0xA0_160 + 0x1E + column * 2));
				DrawText_5A180_5A690(text, x + 8, (int16_t)(y + 0x0A), playerColors_99B58_99B58[column][1]);
			}
			else if (column == row)
			{
				DrawSprite(x, y, cell);
				FillRect(x + 4, y + 4, cell.width_4 - 8, cell.height_5 - 8, colorLookup_AD168_AD158[0]);
			}
		}
		y += head.height_5;
	}
}
