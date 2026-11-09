#include "Mc1Terrain.h"

#include "Mc1Level.h"
#include "Mc1Search.h"
#include "Mc1Shared.h"

int32_t lowRange_90758_90758 = 0;

// 909AC: 8 + 24 (909BC) + 8 (909EC) + 24 (909FC) + 80 (90A2C) pairs, the same in MC1 and HW
const uint8_t terrainTypes_909AC_909AC[144][2] = {
	{ 0x1B, 0x00 }, { 0x1B, 0x50 }, { 0x1B, 0x30 }, { 0x1B, 0x60 }, { 0x1A, 0x00 }, { 0x1A, 0x50 }, { 0x1A, 0x30 }, { 0x1A, 0x60 },
	{ 0x0A, 0x00 }, { 0x0A, 0x50 }, { 0x0A, 0x30 }, { 0x0A, 0x60 }, { 0x0A, 0x00 }, { 0x0A, 0x50 }, { 0x0A, 0x30 }, { 0x0A, 0x60 },
	{ 0x0B, 0x00 }, { 0x0B, 0x50 }, { 0x0B, 0x30 }, { 0x0B, 0x60 }, { 0x0B, 0x00 }, { 0x0B, 0x50 }, { 0x0B, 0x30 }, { 0x0B, 0x60 },
	{ 0x0C, 0x00 }, { 0x0C, 0x50 }, { 0x0C, 0x30 }, { 0x0C, 0x60 }, { 0x0C, 0x00 }, { 0x0C, 0x50 }, { 0x0C, 0x30 }, { 0x0C, 0x60 },
	{ 0x15, 0x00 }, { 0x15, 0x50 }, { 0x15, 0x30 }, { 0x15, 0x60 }, { 0x16, 0x00 }, { 0x16, 0x50 }, { 0x16, 0x30 }, { 0x16, 0x60 },
	{ 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x18, 0x00 }, { 0x18, 0x50 }, { 0x18, 0x30 }, { 0x18, 0x60 },
	{ 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x17, 0x00 }, { 0x17, 0x50 }, { 0x17, 0x30 }, { 0x17, 0x60 },
	{ 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x19, 0x00 }, { 0x19, 0x50 }, { 0x19, 0x30 }, { 0x19, 0x60 },
	{ 0x10, 0x00 }, { 0x10, 0x50 }, { 0x10, 0x30 }, { 0x10, 0x60 }, { 0x0F, 0x00 }, { 0x0F, 0x50 }, { 0x0F, 0x30 }, { 0x0F, 0x60 },
	{ 0x10, 0x00 }, { 0x10, 0x50 }, { 0x10, 0x30 }, { 0x10, 0x60 }, { 0x0F, 0x00 }, { 0x0F, 0x50 }, { 0x0F, 0x30 }, { 0x0F, 0x60 },
	{ 0x1E, 0x00 }, { 0x1E, 0x50 }, { 0x1E, 0x30 }, { 0x1D, 0x60 }, { 0x1F, 0x00 }, { 0x1D, 0x50 }, { 0x1D, 0x30 }, { 0x1D, 0x60 },
	{ 0x1E, 0x00 }, { 0x1E, 0x50 }, { 0x1E, 0x30 }, { 0x1E, 0x60 }, { 0x1D, 0x00 }, { 0x1D, 0x50 }, { 0x1D, 0x30 }, { 0x1D, 0x60 },
	{ 0x21, 0x00 }, { 0x21, 0x50 }, { 0x21, 0x30 }, { 0x21, 0x60 }, { 0x22, 0x00 }, { 0x20, 0x50 }, { 0x20, 0x30 }, { 0x20, 0x60 },
	{ 0x21, 0x00 }, { 0x21, 0x50 }, { 0x21, 0x30 }, { 0x21, 0x60 }, { 0x20, 0x00 }, { 0x20, 0x50 }, { 0x20, 0x30 }, { 0x20, 0x60 },
	{ 0x13, 0x00 }, { 0x13, 0x50 }, { 0x13, 0x30 }, { 0x13, 0x60 }, { 0x14, 0x00 }, { 0x12, 0x50 }, { 0x12, 0x30 }, { 0x12, 0x60 },
	{ 0x13, 0x00 }, { 0x13, 0x50 }, { 0x13, 0x30 }, { 0x13, 0x60 }, { 0x12, 0x00 }, { 0x12, 0x50 }, { 0x12, 0x30 }, { 0x12, 0x60 },
	{ 0x13, 0x00 }, { 0x13, 0x50 }, { 0x13, 0x30 }, { 0x13, 0x60 }, { 0x14, 0x00 }, { 0x12, 0x50 }, { 0x12, 0x30 }, { 0x12, 0x60 },
	{ 0x21, 0x00 }, { 0x21, 0x50 }, { 0x21, 0x30 }, { 0x21, 0x60 }, { 0x20, 0x00 }, { 0x20, 0x50 }, { 0x20, 0x30 }, { 0x20, 0x60 },
};

static uint16_t Tile(int x, int y)
{
	return (uint16_t)((x & 0xFF) | ((y & 0xFF) << 8));
}

uint8_t GetTileHeightType_33640_33A30(uint8_t callerCl, uint16_t tile, uint8_t callerDl)
{
	const uint8_t* height = mapHeightmap_DC1E0_DC1D0;
	int x = tile & 0xFF;
	int y = tile >> 8;
	// the highest corner of the tile (x, y), (x+1, y), (x+1, y+1), (x, y+1)
	uint8_t lowest = 0xFF;
	uint8_t highest = 0;
	uint8_t corner = callerDl;
	static const int8_t corners[4][2] = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };
	for (uint8_t i = 0; i < 4; i++)
	{
		uint8_t h = height[Tile(x + corners[i][0], y + corners[i][1])];
		if (h > highest)
		{
			highest = h;
			corner = i;
		}
		if (h < lowest)
			lowest = h;
	}
	// the second highest corner
	uint8_t second = 0;
	uint8_t secondCorner = callerCl;
	for (uint8_t i = 0; i < 4; i++)
	{
		if (corner == i)
			continue;
		uint8_t h = height[Tile(x + corners[i][0], y + corners[i][1])];
		if (h > second)
		{
			second = h;
			secondCorner = i;
		}
	}
	lowRange_90758_90758 = highest - lowest <= 8 ? 1 : 0;
	if (highest - second >= 8)
		return corner;
	switch (corner)
	{
	case 0:
		return secondCorner == 1 ? 4 : 7;
	case 1:
		return secondCorner == 2 ? 5 : 4;
	case 2:
		return secondCorner == 3 ? 6 : 5;
	case 3:
		return secondCorner == 0 ? 7 : 6;
	}
	return 0;
}

void SetTileTerrainType_33800_33BF0(uint8_t callerCl, uint16_t tile, uint8_t type, uint8_t callerDl)
{
	uint8_t* terrain = mapTerrainType_CC1E0_CC1D0;
	uint8_t* angle = mapAngle_FC1E0_FC1D0;
	int x = tile & 0xFF;
	int y = tile >> 8;
	uint8_t parity = (uint8_t)(((x + y) & 1) << 3);
	if (type < 8)
	{
		angle[tile] = (angle[tile] & 0xF0) | type;
		UpdateTerrainTiles_33B90_33F80(tile, tile);
		return;
	}
	int entry = -1;
	switch (type)
	{
	case 8:
		terrain[tile] = 8;
		break;
	case 9:
		terrain[tile] = 9;
		break;
	case 15:
		terrain[tile] = 0x0B;
		break;
	case 16:
	{
		uint8_t old = terrain[tile];
		if (old != 0x0A && old != 0x0B && old != 0x0C)
			entry = GetTileHeightType_33640_33A30(callerCl, tile, old);
		break;
	}
	case 17:
		entry = 32 + GetTileHeightType_33640_33A30(callerCl, tile, callerDl);
		break;
	case 18:
		entry = 40 + parity + GetTileHeightType_33640_33A30(callerCl, tile, callerDl);
		break;
	case 19:
		entry = 48 + parity + GetTileHeightType_33640_33A30(callerCl, tile, callerDl);
		break;
	case 20:
		entry = 8 + GetTileHeightType_33640_33A30(callerCl, tile, callerDl);
		break;
	case 21:
		entry = 16 + GetTileHeightType_33640_33A30(callerCl, tile, callerDl);
		break;
	case 22:
		entry = 24 + GetTileHeightType_33640_33A30(callerCl, tile, callerDl);
		break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	{
		// 90A2C: 16 per type, + 8 when the heights differ little
		uint8_t index = (uint8_t)(GetTileHeightType_33640_33A30(callerCl, tile, callerDl) + (type - 10) * 0x10);
		if (lowRange_90758_90758)
			index += 8;
		entry = 64 + index;
		break;
	}
	}
	if (entry >= 0)
	{
		terrain[tile] = terrainTypes_909AC_909AC[entry][0];
		angle[tile] = terrainTypes_909AC_909AC[entry][1] | (angle[tile] & 0x8F);
	}
	// 33A92: changed by hand, the tiles of the corners are not flat water any more
	angle[tile] = (angle[tile] | 0x80) & 0xF7;
	angle[Tile(x + 1, y)] &= 0xF7;
	angle[Tile(x + 1, y + 1)] &= 0xF7;
	angle[Tile(x, y + 1)] &= 0xF7;
}

void SetTerrainTypeAround_33AE0_33ED0(uint16_t tile, uint8_t type)
{
	int x = tile & 0xFF;
	int y = tile >> 8;
	mapTerrainType_CC1E0_CC1D0[Tile(x, y)] = type;
	mapTerrainType_CC1E0_CC1D0[Tile(x - 1, y)] = type;
	mapTerrainType_CC1E0_CC1D0[Tile(x - 1, y - 1)] = type;
	mapTerrainType_CC1E0_CC1D0[Tile(x, y - 1)] = type;
	// the shading of the 3x3 tiles around, flat is 20h
	const uint8_t* height = mapHeightmap_DC1E0_DC1D0;
	for (int row = 0; row < 3; row++)
		for (int column = 0; column < 3; column++)
		{
			int cx = x - 1 + column;
			int cy = y - 1 + row;
			uint8_t shade = (uint8_t)(height[Tile(cx - 1, cy - 1)] - height[Tile(cx + 1, cy + 1)] + 0x20);
			if ((int8_t)shade < 0x20)
				shade = 0x20;
			else if ((int8_t)shade > 0x28)
				shade = (shade & 7) + 0x28;
			mapShading_EC1E0_EC1D0[Tile(cx, cy)] = shade;
			mapAngle_FC1E0_FC1D0[Tile(cx, cy)] &= 0xF7;
		}
}

// 33B90 / 33E10: the types of the rectangle again from the corner heights (angleTypes_B5D40), then the shading
static void RecomputeTerrainTiles(uint16_t from, uint16_t to, bool keepFixed)
{
	uint8_t* terrain = mapTerrainType_CC1E0_CC1D0;
	uint8_t* angle = mapAngle_FC1E0_FC1D0;
	int fx = from & 0xFF;
	int fy = from >> 8;
	uint8_t width = (uint8_t)((to & 0xFF) - fx + 1);
	uint8_t height = (uint8_t)((to >> 8) - fy + 1);
	// 1: the tiles touching the rectangle (33B90 not the ones set by hand, 80h)
	for (int row = 0; height && row < height; row++)
		for (int column = 0; width && column < width; column++)
		{
			int x = fx + column;
			int y = fy + row;
			uint16_t tiles[4] = { Tile(x, y), Tile(x - 1, y), Tile(x - 1, y - 1), Tile(x, y - 1) };
			for (uint16_t t : tiles)
				if (!keepFixed || !(angle[t] & 0x80))
					terrain[t] = 1;
		}
	// 2: one tile more up / left
	fx--;
	fy--;
	width++;
	height++;
	uint16_t* seed = randSeed_12C1E0_12C1D0;
	for (int row = 0; height && row < height; row++)
		for (int column = 0; width && column < width; column++)
		{
			int x = fx + column;
			int y = fy + row;
			uint16_t t = Tile(x, y);
			if (terrain[t] != 1)
				continue;
			int key = (angle[t] & 7) * 343 + (angle[Tile(x + 1, y)] & 7) * 49 + (angle[Tile(x + 1, y + 1)] & 7) * 7
				+ (angle[Tile(x, y + 1)] & 7);
			terrain[t] = angleTypes_B5D40_B5D30[key][0];
			if (terrain[t] < 8)
			{
				uint8_t kept = angle[t] & 0x87;
				*seed = (uint16_t)(*seed * 0x24A1 + 0x24DF);
				angle[t] = (uint8_t)(((*seed % 7) << 4) + kept);
			}
			else
				angle[t] = (uint8_t)((angle[t] & 0x87) + angleTypes_B5D40_B5D30[key][1]);
		}
	// 3: the shading, one tile more
	width++;
	height++;
	const uint8_t* heights = mapHeightmap_DC1E0_DC1D0;
	for (int row = 0; height && row < height; row++)
		for (int column = 0; width && column < width; column++)
		{
			int x = fx + column;
			int y = fy + row;
			uint8_t shade = (uint8_t)(heights[Tile(x - 1, y - 1)] - heights[Tile(x + 1, y + 1)] + 0x20);
			if ((int8_t)shade < 0x1C)
				shade = (shade & 3) + 0x1C;
			else if ((int8_t)shade > 0x28)
				shade = (shade & 7) + 0x28;
			mapShading_EC1E0_EC1D0[Tile(x, y)] = shade;
			angle[Tile(x, y)] &= 0xF7;
		}
}

void UpdateTerrainTiles_33B90_33F80(uint16_t from, uint16_t to)
{
	RecomputeTerrainTiles(from, to, true);
}

void RebuildTerrainTiles_33E10_34200(uint16_t from, uint16_t to)
{
	RecomputeTerrainTiles(from, to, false);
}

bool CheckTileType8Neighbours_264D0_264D0(uint16_t tile)
{
	int x = tile & 0xFF;
	int y = tile >> 8;
	const uint8_t* height = mapHeightmap_DC1E0_DC1D0;
	int32_t limit = height[tile] + 0x1E;
	if (mapTerrainType_CC1E0_CC1D0[Tile(x - 1, y)] != 8)
		return true;
	if (height[Tile(x - 1, y)] > limit || height[Tile(x + 1, y)] > limit || height[Tile(x, y + 1)] > limit
		|| height[Tile(x, y - 1)] > limit)
		return true;
	return false;
}

uint8_t ChangeTileHeight_40A10_40D50(int32_t x, int32_t y, int32_t add, uint8_t fixed)
{
	// 1: clamped at the tile 0,0 or a tile set by hand (80h) with fixed
	uint16_t tile = Tile(x, y);
	bool origin = !(int16_t)x && !(int16_t)y;
	uint8_t result = 0;
	int16_t height = (int16_t)(mapHeightmap_DC1E0_DC1D0[tile] + add);
	if (height > 0xC8)
	{
		height = 0xC8;
		if (origin)
			result = 1;
	}
	if (height < 0)
	{
		height = 0;
		if (origin)
			result = 1;
	}
	uint8_t* angle = mapAngle_FC1E0_FC1D0;
	if (fixed && (angle[tile] & 0x80))
		return 1;
	mapHeightmap_DC1E0_DC1D0[tile] = (uint8_t)height;
	if (height)
		angle[tile] = (angle[tile] & 0xF8) | 1;
	else
	{
		// flat at 0 when none of the 8 neighbours has the angle types 2, 3, 5
		static const int8_t around[8][2] = { { -1, -1 }, { 0, -1 }, { 1, -1 }, { 1, 0 }, { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 } };
		bool free = true;
		for (int i = 0; i < 8 && free; i++)
		{
			uaxis_2d neighbour;
			neighbour.word = Tile(x + around[i][0], y + around[i][1]);
			free = IsTileFreeOfFeatures_409E0_40D20(neighbour);
		}
		if (free)
			angle[tile] &= 0xF0;
	}
	if (fixed)
		UpdateTerrainTiles_33B90_33F80(tile, tile);
	else
		RebuildTerrainTiles_33E10_34200(tile, tile);
	return result;
}

uint8_t ChangeHeightsAround_40D30_41070(Type_Mc1Entity* entity, int16_t first, int32_t last, int16_t add, uint8_t fixed)
{
	// the tiles of the rings first..last (at most the radius of the entity), 1 when a tile stopped it
	int32_t x = (entity->position_0x48_72.x + 0x80) >> 8;
	int32_t y = (entity->position_0x48_72.y + 0x80) >> 8;
	int32_t radius = entity->box_0x4E_78[1] / 256;
	if ((int16_t)last > radius)
		last = radius;
	int32_t search = StartSearch_11410_11410(first, (int16_t)last);
	if (!search)
		return 0;
	int32_t dx;
	int32_t dy;
	while (GetNextSearchOffset_114B0_114B0(search, &dx, &dy) == 1)
	{
		if (ChangeTileHeight_40A10_40D50((int16_t)(x + dx), (int16_t)(y + dy), add, fixed))
		{
			EndSearch_11490_11490(search);
			return 1;
		}
	}
	EndSearch_11490_11490(search);
	return 0;
}

void LowerHeightsAround_255D0_255D0(Type_Mc1Entity* entity, int16_t first, int32_t last)
{
	int32_t x = (entity->position_0x48_72.x + 0x80) >> 8;
	int32_t y = (entity->position_0x48_72.y + 0x80) >> 8;
	int32_t radius = entity->box_0x4E_78[1] / 256;
	if ((int16_t)last > radius)
		last = radius;
	int32_t search = StartSearch_11410_11410(first, (int16_t)last);
	if (!search)
		return;
	int32_t dx;
	int32_t dy;
	while (GetNextSearchOffset_114B0_114B0(search, &dx, &dy) == 1)
		ChangeTileHeight_40A10_40D50(x + dx, y + dy, -3, 0);
	EndSearch_11490_11490(search);
}

void SmoothBuildingBorder_35F30_362F0(uint8_t x, uint8_t y, int32_t halfHeight, int32_t halfWidth, uint8_t border)
{
	uint8_t left = (uint8_t)(x - halfWidth);
	uint8_t top = (uint8_t)(y - halfHeight);
	// the left and the right side
	uint8_t row = top;
	for (uint16_t i = (uint16_t)(2 * halfHeight); i; i--, row++)
	{
		uint8_t a = (uint8_t)(left - border);
		uint8_t b = (uint8_t)(left + 2 * halfWidth);
		for (int32_t j = 0; j <= border; j++, a++, b++)
		{
			SmoothTileHeight_360C0_36480(Tile(a, row));
			SmoothTileHeight_360C0_36480(Tile(b, row));
		}
	}
	// the top and the bottom side, one column less on the right
	uint8_t column = (uint8_t)(left - border);
	for (uint16_t i = (uint16_t)(2 * halfWidth + 2 * border); i; i--, column++)
	{
		uint8_t a = (uint8_t)(top - border);
		uint8_t b = (uint8_t)(top + 2 * halfHeight);
		for (int32_t j = 0; j <= border; j++, a++, b++)
		{
			SmoothTileHeight_360C0_36480(Tile(column, a));
			SmoothTileHeight_360C0_36480(Tile(column, b));
		}
	}
}

static bool IsSteepType(uint8_t type)
{
	return type > 5 && type <= 0x22;
}

void SmoothTileHeight_360C0_36480(uint16_t tile)
{
	// the average height of the 3x3 tiles that are not steep
	uint8_t* height = mapHeightmap_DC1E0_DC1D0;
	if (!(mapAngle_FC1E0_FC1D0[tile] & 7) || !height[tile])
		return;
	static const int32_t corners[4] = { -0x101, -0x100, -1, 0 };
	for (int32_t offset : corners)
	{
		// the original reads other globals before the map at the row 0, taken as type 0 like remc2
		int32_t index = tile + offset;
		if (index >= 0 && IsSteepType(mapTerrainType_CC1E0_CC1D0[index]))
			return;
	}
	uint32_t sum = 0;
	uint32_t count = 0;
	uint32_t index = tile - 0x101u;
	for (int row = 0; row < 3; row++, index += 0xFD)
		for (int column = 0; column < 3; column++, index++)
			if (!IsSteepType(mapTerrainType_CC1E0_CC1D0[(uint16_t)index]))
			{
				count++;
				sum += height[(uint16_t)index];
			}
	if (count)
		height[tile] = (uint8_t)(sum / count);
}
