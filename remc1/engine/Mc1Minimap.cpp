#include "Mc1Minimap.h"

#include <cstring>

#include "Mc1Blocks.h"
#include "Mc1Files.h"
#include "Mc1Game.h"
#include "Mc1Graphics.h"
#include "Mc1Shared.h"

int16_t minimapRows_12C1F0_12C1E0[0x1E0][2];

// 99B58: two palette colours of each player (the dots and the names)
const uint8_t playerColors_99B58_99B58[8][2] = {
	{ 0xB7, 0x71 }, { 0x7D, 0x7A }, { 0x9D, 0x9A }, { 0x07, 0x5A }, { 0x1D, 0x1B }, { 0xDD, 0xDA }, { 0x3C, 0x39 }, { 0x10, 0x0E },
};

static Type_Mc1Entity* Entities()
{
	return mc1State_AE400_AE3F0->entities_0x7463_29795;
}

static int16_t ReadS16(const uint8_t* data)
{
	int16_t value;
	memcpy(&value, data, sizeof(value));
	return value;
}

// the player of a wizard (playerData + 30h)
static int16_t PlayerOf(const Type_Mc1Entity& wizard)
{
	return ReadS16(wizard.playerData_0xA0_160 + 0x30);
}

static const uint8_t* PlayerColor(int16_t player, int index)
{
	return &playerColors_99B58_99B58[0][0] + player * 2 + index;
}

void DrawMinimapTerrain_49300_49640(int32_t screenX, int32_t screenY, int32_t worldX, int32_t worldY, int32_t width, int32_t height,
	int32_t yaw, int32_t scale, int32_t /*unused*/, int32_t mode)
{
	if (*videoMode_12F02E_12F01E & 1)
	{
		screenX >>= 1;
		screenY >>= 1;
		width >>= 1;
		height >>= 1;
		scale *= 2;
	}
	uint8_t* screen = *workScreen_12EFF4_12EFE4 + screenX + screenY * *screenWidth_12EFF0_12EFE0;
	if (mode)
	{
		for (int32_t y = 0; y < height; y++)
		{
			minimapRows_12C1F0_12C1E0[y][0] = (int16_t)width;
			minimapRows_12C1F0_12C1E0[y][1] = 0;
		}
	}
	else
	{
		// the circle from the middle row up and down: sqrt(1 - y * y) of the distances table
		const int32_t half = height / 2;
		const int32_t step = 0x1000000 / (height >> 1);
		const int32_t radius = width >> 1;
		int32_t t = 0;
		for (int32_t i = 0; i < half; i++, t += step)
		{
			const int32_t v = tables_B7934_B7924.distances_0x14300[t >> 16] * radius >> 8;
			minimapRows_12C1F0_12C1E0[half + i][0] = minimapRows_12C1F0_12C1E0[half - 1 - i][0] = (int16_t)(radius + v);
			minimapRows_12C1F0_12C1E0[half + i][1] = minimapRows_12C1F0_12C1E0[half - 1 - i][1] = (int16_t)(radius - v);
		}
	}
	yaw &= 0x7FF;
	// the steps of the map (8.8) along a row and from row to row
	const int32_t sinS = Maths::sin_DB750[yaw] * scale >> 16;
	const int32_t cosS = Maths::sin_DB750[512 + yaw] * scale >> 16;
	const int32_t stepY = sinS * height / width;
	const int32_t stepX = cosS * height / width;
	int32_t rowX = worldX - (stepX * width - sinS * height) / 2;
	int32_t rowY = worldY - (stepY * width + cosS * height) / 2;
	const uint8_t* fades = &tables_B7934_B7924.fades_0x0000[0][0];
	for (int32_t y = 0; y < height; y++, screen += *screenWidth_12EFF0_12EFE0, rowX -= sinS, rowY += cosS)
	{
		const int32_t start = minimapRows_12C1F0_12C1E0[y][1];
		const int32_t end = minimapRows_12C1F0_12C1E0[y][0];
		uint32_t mapX = stepX * start + rowX;
		uint32_t mapY = stepY * start + rowY;
		uint8_t* pixel = screen + start;
		uint32_t count = end - start;
		do
		{
			const uint16_t tile = (uint16_t)(((mapX >> 8) & 0xFF) | (mapY & 0xFF00));
			const uint8_t color = fades[mapShading_EC1E0_EC1D0[tile] << 8 | tables_B7934_B7924.blockColors_0x14000[mapTerrainType_CC1E0_CC1D0[tile]]];
			*pixel = mode ? color : tables_B7934_B7924.blend_0x4000[*pixel][color];
			pixel++;
			mapX += stepX;
			mapY += stepY;
		} while (--count);
	}
}

// a world position to the minimap, false out of the rows
static bool MinimapPoint(const Type_Mc1Position& position, int32_t worldX, int32_t worldY, int32_t cosK, int32_t sinK, int32_t centreX,
	int32_t centreY, int32_t width, int32_t height, int32_t* x, int32_t* y)
{
	const int32_t dx = (int16_t)(position.x - worldX);
	const int32_t dy = (int16_t)(position.y - worldY);
	*x = ((cosK * dx - dy * sinK) >> 16) + centreX;
	if (*x < 0 || *x >= width)
		return false;
	*y = ((dx * sinK + dy * cosK) >> 16) + centreY;
	if (*y < 0 || *y >= height)
		return false;
	return *x >= minimapRows_12C1F0_12C1E0[*y][1] && *x < minimapRows_12C1F0_12C1E0[*y][0];
}

void DrawMinimapEntities_48710_48A50(int32_t screenX, int32_t screenY, int32_t worldX, int32_t worldY, int32_t width, int32_t height,
	int32_t yaw, int32_t scale, int32_t /*unused*/, int32_t /*mode*/)
{
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	Type_Mc1Settings_AE408_AE3F8* settings = mc1Settings_AE408_AE3F8;
	if (*videoMode_12F02E_12F01E & 1)
	{
		screenX >>= 1;
		screenY >>= 1;
		scale *= 2;
		width >>= 1;
		height >>= 1;
	}
	uint8_t* screen = *workScreen_12EFF4_12EFE4 + screenX + screenY * *screenWidth_12EFF0_12EFE0;
	const int32_t k = 0x10000 / scale;
	const int32_t centreX = width / 2;
	const int32_t centreY = height / 2;
	yaw &= 0x7FF;
	const int32_t sinK = -(Maths::sin_DB750[yaw] * k) >> 16;
	const int32_t cosK = k * Maths::sin_DB750[512 + yaw] >> 16;
	SetFont_5A3C0_5A8D0(1);
	Type_Mc1Entity& wizard = Entities()[state->players_0x340B_13323[state->currentPlayer_0x08_8].entityIndex_0x0A_10];
	// the spell 5 (playerData + 2AEh) shows the names of the players
	int32_t showNames = 0;
	if (const int16_t spell = ReadS16(wizard.playerData_0xA0_160 + 0x2AE))
		showNames = Entities()[spell].word_0x30_48;

	// the dotted line from the wizard to its castle (playerData + 32h), moving with the frames
	if (const uint16_t castle = (uint16_t)ReadS16(wizard.playerData_0xA0_160 + 0x32))
	{
		int32_t dx = (int16_t)(wizard.position_0x48_72.x - worldX);
		int32_t dy = (int16_t)(wizard.position_0x48_72.y - worldY);
		const int32_t fromX = ((cosK * dx - dy * sinK) >> 16) + centreX;
		const int32_t fromY = ((cosK * dy + dx * sinK) >> 16) + centreY;
		const Type_Mc1Position& to = Entities()[castle].position_0x48_72;
		dx = (int16_t)(to.x - worldX);
		dy = (int16_t)(to.y - worldY);
		const int32_t toX = ((cosK * dx - dy * sinK) >> 16) + centreX;
		const int32_t toY = ((cosK * dy + dx * sinK) >> 16) + centreY;
		const int32_t length = IntegerSqrt_410CE_4140E((toX - fromX) * (toX - fromX) + (toY - fromY) * (toY - fromY));
		const uint16_t angle = Atan2_40F87_412C7((int16_t)(toX - fromX), (int16_t)(toY - fromY));
		for (int32_t r = (state->players_0x340B_13323[state->currentPlayer_0x08_8].turn_0x12_18 & 3) + 4; r <= length; r += 4)
		{
			const int32_t x = (Maths::sin_DB750[angle] * r >> 16) + fromX;
			const int32_t y = (Maths::sin_DB750[512 + angle] * -r >> 16) + fromY;
			if (x < 0 || x >= width || y < 0 || y >= height)
				break;
			if (x < minimapRows_12C1F0_12C1E0[y][1] || x >= minimapRows_12C1F0_12C1E0[y][0])
				break;
			uint8_t* pixel = screen + x + y * *screenWidth_12EFF0_12EFE0;
			*pixel = tables_B7934_B7924.blend_0x4000[*pixel][colorLookup_AD168_AD158[0xFFF]];
		}
	}

	// the entities: a dot (1 or 2x2 pixels) of a colour, or a sprite (castles 3Ah / 42h + the player, S and T)
	for (Type_Mc1Entity* entity = &Entities()[1]; entity < &Entities()[1000]; entity++)
	{
		uint8_t sprite = 0;
		uint8_t color = 0;
		int32_t size = 1;
		switch (entity->class_0x40_64)
		{
		case 2:
			switch (entity->model_0x41_65)
			{
			case 0:
				color = 0x1C;
				if (entity->actionIndex_0x46_70 != 2)
					sprite = 1;
				break;
			case 1:
			case 3:
				if (settings->colorIndex_0x5D_93[2])
				{
					color = colorLookup_AD168_AD158[0];
					sprite = 1;
				}
				break;
			default:
				color = 0x1C;
				sprite = 1;
				break;
			}
			break;
		case 3:
			if (entity->model_0x41_65 == 2)
				sprite = (uint8_t)(PlayerOf(Entities()[(int16_t)entity->id_0x18_24]) + 0x3A);
			else if (entity->model_0x41_65 == 3 && (entity->id_0x18_24 == wizard.id_0x18_24 || showNames))
				sprite = (uint8_t)(PlayerOf(Entities()[(int16_t)entity->id_0x18_24]) + 0x42);
			break;
		case 5:
			if (entity->actionIndex_0x46_70 == 0x78 || (entity->flags_0x10_16 & 1))
				break;
			if ((int16_t)entity->id_0x18_24 == (int32_t)(entity - Entities()))
				color = (entity->model_0x41_65 >= 0x0C && entity->model_0x41_65 <= 0x0E) ? colorLookup_AD168_AD158[0x0F] : colorLookup_AD168_AD158[0];
			else
				color = *PlayerColor(PlayerOf(Entities()[(int16_t)entity->id_0x18_24]), 1);
			sprite = 1;
			break;
		case 9:
		case10_owner:
		{
			const Type_Mc1Entity& owner = Entities()[(int16_t)entity->id_0x18_24];
			color = owner.class_0x40_64 == 3 ? *PlayerColor(PlayerOf(owner), 0) : colorLookup_AD168_AD158[0xF0F];
			sprite = 1;
			break;
		}
		case 10:
			switch (entity->model_0x41_65)
			{
			case 0x12:
				break;
			case 0x22:
				size = 2;
				goto case10_owner;
			case 0x27:
				// coloured by the player of the owner (blinking with colorIndex[3]), else E8h
				if (entity->owner_0x90_144 && Entities()[entity->owner_0x90_144].class_0x40_64 == 3)
					color = *PlayerColor(PlayerOf(Entities()[entity->owner_0x90_144]), settings->colorIndex_0x5D_93[3]);
				else
					color = 0xE8;
				sprite = 1;
				break;
			default:
				goto case10_owner;
			}
			break;
		case 11:
			if (entity->model_0x41_65 >= 9 && entity->model_0x41_65 <= 12)
				sprite = 'S';
			else if (entity->model_0x41_65 == 31)
				sprite = 'T';
			break;
		case 12:
			if (!(entity->flags_0x10_16 & 1))
			{
				color = colorLookup_AD168_AD158[0xF00];
				sprite = 1;
			}
			break;
		}
		if (!sprite)
			continue;
		int32_t x, y;
		if (!MinimapPoint(entity->position_0x48_72, worldX, worldY, cosK, sinK, centreX, centreY, width, height, &x, &y))
			continue;
		uint8_t* pixel = screen + x + y * *screenWidth_12EFF0_12EFE0;
		if (sprite <= 1)
		{
			pixel[0] = color;
			if (size > 1)
			{
				pixel[1] = color;
				pixel[*screenWidth_12EFF0_12EFE0] = color;
				pixel[*screenWidth_12EFF0_12EFE0 + 1] = color;
			}
			continue;
		}
		const bitmap_pos_struct_t& bitmap = spritesIndex[sprite];
		auto draw = [&](int32_t drawX, int32_t drawY) {
			if (*videoMode_12F02E_12F01E & 1)
				DrawBitmap320_60CE0_611F0((int16_t)drawX, (int16_t)drawY, bitmap, 1);
			else
				DrawBitmap640_60D18_61228((int16_t)drawX, (int16_t)drawY, bitmap, 1);
		};
		if (*videoMode_12F02E_12F01E == 1)
		{
			// 320: the sprites in the coordinates of 640
			if (sprite >= 0x42 && sprite < 0x4A)
				draw(x * 2 - bitmap.width_4 / 2, y * 2 - bitmap.height_5);
			else if (sprite >= 0x3A && sprite < 0x42)
				draw(x * 2, y * 2 - bitmap.height_5);
			else if (sprite == 'S' || sprite == 'T')
				draw(x * 2 - bitmap.width_4 / 2, y * 2 - bitmap.height_5 / 2);
		}
		else if (sprite < 0x3C)
			draw(x, y - bitmap.height_5);
		else
			draw(x - bitmap.width_4 / 2, y - bitmap.height_5);
	}

	// the names of the other players at their wizards
	if (showNames)
	{
		for (uint16_t i = 0; i < state->playerCount_0x0A_10; i++)
		{
			if (i == (uint16_t)state->currentPlayer_0x08_8)
				continue;
			const Type_Mc1Entity& other = Entities()[state->players_0x340B_13323[i].entityIndex_0x0A_10];
			if (other.life_0x0C_12 < 0)
				continue;
			int32_t x, y;
			if (!MinimapPoint(other.position_0x48_72, worldX, worldY, cosK, sinK, centreX, centreY, width, height, &x, &y))
				continue;
			const char* name = state->players_0x340B_13323[i].name_0x40A_1034;
			if (*videoMode_12F02E_12F01E == 1)
				DrawText_5A180_5A690(name, (int16_t)(x * 2 + 2), (int16_t)(y * 2), *PlayerColor(PlayerOf(other), 1));
			else
				DrawText_5A180_5A690(name, (int16_t)(x + 2), (int16_t)y, *PlayerColor(PlayerOf(other), 1));
		}
	}

	// the cross in the middle, fading out to the ends
	int32_t count = width / 12;
	const int32_t step = 0x800 / count;
	uint8_t* centre = screen + width / 2 + height / 2 * *screenWidth_12EFF0_12EFE0 - 1;
	const uint8_t* tables = (const uint8_t*)&tables_B7934_B7924;
	int32_t fade = 0x2C00;
	*centre = tables[fade + *centre];
	uint8_t* up = centre;
	uint8_t* down = centre;
	uint8_t* right = centre;
	uint8_t* left = centre;
	for (; count; count--)
	{
		up -= *screenWidth_12EFF0_12EFE0;
		down += *screenWidth_12EFF0_12EFE0;
		fade -= step;
		const uint8_t* row = tables + (fade & 0xFF00);
		*up = row[*up];
		right[1] = row[right[1]];
		*down = row[*down];
		left[-1] = row[left[-1]];
		right++;
		left--;
	}
}
