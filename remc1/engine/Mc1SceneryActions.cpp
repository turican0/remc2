#include "Mc1SceneryActions.h"

#include "Mc1Entity.h"
#include "Mc1Files.h"
#include "Mc1Game.h"
#include "Mc1NotPorted.h"
#include "Mc1Shared.h"
#include "Mc1Terrain.h"

static uint16_t Tile(int x, int y)
{
	return (uint16_t)((x & 0xFF) | ((y & 0xFF) << 8));
}

static int16_t EntityIndex(Type_Mc1Entity* entity)
{
	return (int16_t)(entity - mc1State_AE400_AE3F0->entities_0x7463_29795);
}

static uint8_t TileX(const Type_Mc1Entity* entity)
{
	return (uint8_t)((entity->position_0x48_72.x + 0x80) >> 8);
}

static uint8_t TileY(const Type_Mc1Entity* entity)
{
	return (uint8_t)((entity->position_0x48_72.y + 0x80) >> 8);
}

static bool OnFlatWater(Type_Mc1Entity* entity)
{
	// GetTileAngleBit & 1: the angle bits of the tile are 0
	return GetTileAngleBit_11760_11760((axis_3d*)&entity->position_0x48_72) & 1;
}

static uint32_t NextRand(Type_Mc1Entity* entity)
{
	entity->rand_0x04_4 = entity->rand_0x04_4 * 0x24A1 + 0x24DF;
	return entity->rand_0x04_4;
}

void FreeSceneryAction_253E0_253E0(Type_Mc1Entity* entity)
{
	MarkToFree_41E80_421C0(entity);
}

void RaiseTerrainAction_25470_25470(Type_Mc1Entity* entity)
{
	// grows a ring more every 6 steps, at the end a pit in the centre and the model 12h
	entity->word_0x1A_26++;
	bool done = entity->life_0x0C_12-- < 0;
	if (!done)
	{
		int16_t add = (int16_t)(NextRand(entity) % 9);
		done = ChangeHeightsAround_40D30_41070(entity, 0, entity->word_0x1A_26 / 6, add, 0) != 0;
	}
	if (done)
	{
		ChangeHeightsAround_40D30_41070(entity, 0, 0, -40, 0);
		featurePosition_AE454_AE444 = entity->position_0x48_72;
		featurePosition_AE454_AE444.z = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&featurePosition_AE454_AE444);
		Type_Mc1Entity* created = CreateEntity_373F0_377B0(&featurePosition_AE454_AE444, 10, 0x12);
		if (created)
			created->id_0x18_24 = entity->id_0x18_24;
		MarkToFree_41E80_421C0(entity);
		return;
	}
	SendMailToOverlappingEntities_127E0_127E0(entity, 0, (uint16_t)entity->word_0x2C_44);
	sub_55370_558A0(EntityIndex(entity), -1, 10);
}

void LowerTerrainOnceAction_25570_25570(Type_Mc1Entity* entity)
{
	if (!OnFlatWater(entity))
	{
		int16_t add = (int16_t)-(int16_t)(NextRand(entity) % 7);
		ChangeHeightsAround_40D30_41070(entity, 0, (int16_t)(entity->box_0x4E_78[1] / 256), add, 1);
	}
	MarkToFree_41E80_421C0(entity);
}

void SinkTerrainAction_25670_25670(Type_Mc1Entity* entity)
{
	// a ring more every 3rd entity index, the outer ring once when the radius is reached
	if (!(entity->index_0x3F_63 % 3))
		entity->word_0x1A_26++;
	if (entity->life_0x0C_12-- < 0 || OnFlatWater(entity))
	{
		MarkToFree_41E80_421C0(entity);
		return;
	}
	uint16_t value = (uint16_t)entity->word_0x2C_44;
	SendMailToOverlappingEntities_127E0_127E0(entity, 0, (entity->flags_0x10_16 & 2) ? value / 25 : value);
	int32_t radius = entity->box_0x4E_78[1] / 256 - 1;
	int16_t last = entity->word_0x1A_26;
	if (last > radius)
	{
		last = (int16_t)radius;
		if (!(entity->flags_0x10_16 & 2))
			LowerHeightsAround_255D0_255D0(entity, (int16_t)(radius + 1), (int16_t)(radius + 1));
	}
	entity->flags_0x10_16 |= 2;
	LowerHeightsAround_255D0_255D0(entity, 0, last);
	sub_55370_558A0(EntityIndex(entity), -1, 10);
}

static void RaiseWallTile(uint16_t tile)
{
	if (mapTerrainType_CC1E0_CC1D0[tile] != 8 || CheckTileType8Neighbours_264D0_264D0(tile))
		mapHeightmap_DC1E0_DC1D0[tile] += 0x30;
	SetTerrainTypeAround_33AE0_33ED0(tile, 8);
}

// the rows of wall tiles (life wide) go along y, the next tile of a row is the next word (wraps to the next row),
// the tiles before and after a row are marked as set by hand (80h)
static void RaiseWallRow(uint8_t x, uint8_t y, uint16_t width)
{
	uint8_t* angle = mapAngle_FC1E0_FC1D0;
	angle[Tile(x - 1, y)] |= 0x80;
	uint16_t tile = Tile(x, y);
	for (uint16_t i = width; i; i--, tile++)
		RaiseWallTile(tile);
	angle[tile] |= 0x80;
}

void RaiseWallYPlusAction_26560_26560(Type_Mc1Entity* entity)
{
	uint8_t x = TileX(entity);
	uint8_t y = TileY(entity);
	if (Tile(x, y) % 2)
		x++;
	x = (uint8_t)(x - (uint8_t)entity->life_0x0C_12 + 1);
	uint16_t width = (uint16_t)entity->life_0x0C_12;
	for (uint16_t rows = (uint16_t)(entity->word_0x1A_26 + width); rows; rows--, y++)
		RaiseWallRow(x, y, width);
	MarkToFree_41E80_421C0(entity);
}

void RaiseWallYMinusAction_26670_26670(Type_Mc1Entity* entity)
{
	uint8_t x = TileX(entity);
	uint8_t y = (uint8_t)(TileY(entity) + 2);
	uint16_t width = (uint16_t)entity->life_0x0C_12;
	for (uint16_t rows = (uint16_t)(entity->word_0x1A_26 + width); rows; rows--, y--)
		RaiseWallRow(x, y, width);
	MarkToFree_41E80_421C0(entity);
}

void RaiseWallXAction_26760_26760(Type_Mc1Entity* entity)
{
	// word_0x1A wide along x, life rows; the rows above and below are marked as set by hand (80h)
	uint8_t* angle = mapAngle_FC1E0_FC1D0;
	uint8_t x = TileX(entity);
	uint8_t y = TileY(entity);
	if ((x + y) % 2)
		x++;
	uint16_t width = (uint16_t)entity->word_0x1A_26;
	uint16_t tile = Tile(x, y - 1);
	for (uint16_t i = width; i; i--, tile++)
		angle[tile] |= 0x80;
	for (uint16_t rows = (uint16_t)entity->life_0x0C_12; rows; rows--, y++)
	{
		tile = Tile(x, y);
		for (uint16_t i = width; i; i--, tile++)
			RaiseWallTile(tile);
	}
	tile = Tile(x, y);
	for (uint16_t i = width; i; i--, tile++)
		angle[tile] |= 0x80;
	MarkToFree_41E80_421C0(entity);
}

void ApplyPathAction_26890_26890(Type_Mc1Entity* entity)
{
	// word_0x1A tiles by the step word_0x1E, word_0x20 get the angle type 1
	uint8_t* angle = mapAngle_FC1E0_FC1D0;
	uint8_t x = TileX(entity);
	uint8_t y = TileY(entity);
	uint8_t stepX = (uint8_t)entity->word_0x1E_30;
	uint8_t stepY = (uint8_t)entity->word_0x20_32;
	for (uint16_t i = (uint16_t)entity->word_0x1A_26; i; i--, x += stepX, y += stepY)
	{
		uint16_t tile = Tile(x, y);
		angle[tile] = (angle[tile] & 0xF0) | 1;
		UpdateTerrainTiles_33B90_33F80(tile, tile);
	}
	MarkToFree_41E80_421C0(entity);
}

void LayTrailAction_26920_26920(Type_Mc1Entity* entity)
{
	// the model 0Bh on every step forward
	if (entity->life_0x0C_12-- < 0 || OnFlatWater(entity))
	{
		MarkToFree_41E80_421C0(entity);
		return;
	}
	Type_Mc1Entity* created = CreateEntity_373F0_377B0(&entity->position_0x48_72, 10, 0x0B);
	if (created)
	{
		created->life_0x0C_12 = 2;
		created->box_0x4E_78[3] = entity->box_0x4E_78[3];
		created->id_0x18_24 = entity->id_0x18_24;
	}
	MoveEntity_41EC0_42200((axis_3d*)&entity->position_0x48_72, (uint16_t)entity->word_0x1E_30, 0, entity->word_0x7E_126);
}

void RaiseRidgeAction_269A0_269A0(Type_Mc1Entity* entity)
{
	// raises the tiles under it while moving forward until the water
	if (entity->life_0x0C_12-- < 0 || OnFlatWater(entity))
	{
		MarkToFree_41E80_421C0(entity);
		return;
	}
	int16_t add = (int16_t)(NextRand(entity) % 15 + 10);
	if (!ChangeHeightsAround_40D30_41070(entity, 0, 0x400, add, 0))
	{
		SendMailAround_120B0_120B0(entity, 0, (uint16_t)entity->word_0x2C_44);
		sub_55370_558A0(EntityIndex(entity), -1, 10);
	}
	MoveEntity_41EC0_42200((axis_3d*)&entity->position_0x48_72, (uint16_t)entity->word_0x1E_30, 0, entity->word_0x7E_126);
}

static void BuildingHeightStep(Type_Mc1Entity* entity, uint16_t tile, int32_t target)
{
	// the height moves toward the target by 1/life, the flat tiles become type 1
	uint8_t* height = mapHeightmap_DC1E0_DC1D0;
	uint8_t* angle = mapAngle_FC1E0_FC1D0;
	height[tile] = (uint8_t)(height[tile] + (target - height[tile]) / entity->life_0x0C_12);
	if (!(angle[tile] & 7))
	{
		angle[tile] = (angle[tile] & 0xF0) | 1;
		UpdateTerrainTiles_33B90_33F80(tile, tile);
	}
}

void BuildBuildingAction_27D30_27D30(Type_Mc1Entity* entity)
{
	// the *Build data of the sprite byte_0x47, rows of RLE records: 0 the next row, < 0 skip, > 0 that many tile codes
	uint8_t centreX = TileX(entity);
	uint8_t centreY = TileY(entity);
	int16_t z = (int16_t)(entity->position_0x48_72.z >> 5);
	const bitmap_pos_struct_t& def = buildIndex[entity->byte_0x47_71];
	const uint8_t* data = def.data;
	uint32_t rows = def.height_5;
	uint32_t width = def.width_4;
	if (*videoMode_12F02E_12F01E == 1)
	{
		rows >>= 1;
		width >>= 1;
	}
	entity->life_0x0C_12--;
	uint32_t halfWidth = width >> 1;
	uint32_t halfHeight = rows >> 1;
	if (!entity->life_0x0C_12)
	{
		UpdateTerrainTiles_33B90_33F80(Tile(centreX - halfWidth, centreY - halfHeight), Tile(centreX + halfWidth, centreY + halfHeight));
		if (entity->actionIndex_0x46_70 == 0x30)
		{
			mc1State_AE400_AE3F0->entities_0x7463_29795[entity->word_0x2A_42].word_0x30_48 = 2;
			MarkToFree_41E80_421C0(entity);
		}
		else if (entity->actionIndex_0x46_70 == 0x33)
		{
			entity->life_0x0C_12 = (uint16_t)entity->word_0x2C_44;
			entity->flags_0x10_16 |= 1;
			entity->actionIndex_0x46_70 = 0x34;
			entity->position_0x48_72.z = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&entity->position_0x48_72);
			SmoothBuildingBorder_35F30_362F0(centreX, centreY, (uint16_t)halfHeight, (uint16_t)halfWidth, 2);
			SmoothBuildingBorder_35F30_362F0(centreX, centreY, (uint16_t)halfHeight, (uint16_t)halfWidth, 5);
		}
		return;
	}
	uint8_t startX = (uint8_t)(centreX - halfWidth);
	uint8_t startY = (uint8_t)(centreY - halfHeight);
	// 1: the heights toward the building
	uint8_t x = startX;
	uint8_t y = startY;
	uint32_t position = 0;
	for (uint32_t left = rows; left;)
	{
		int8_t code = (int8_t)data[position++];
		if (!code)
		{
			y++;
			left--;
			x = startX;
			continue;
		}
		if (code < 0)
		{
			x = (uint8_t)(x - code);
			continue;
		}
		for (int32_t n = code; n; n--, x++)
		{
			uint8_t tileCode = data[position++];
			uint8_t low = tileCode & 0x0F;
			int32_t target;
			if (tileCode < 0x0F)
			{
				if (tileCode <= 6)
					continue;
				target = z;
			}
			else if ((tileCode >> 4) == 3)
			{
				uint8_t part = low % 3;
				if (!part)
					continue;
				target = z + (part == 1 ? 0x0C : 0x10);
			}
			else
			{
				if (!low)
					continue;
				target = z + (low - 1) * 4;
			}
			BuildingHeightStep(entity, Tile(x, y), target);
		}
	}
	// 2: the terrain types on every 5th step and the last one
	if (entity->life_0x0C_12 % 5 && entity->life_0x0C_12 != 1)
		return;
	x = startX;
	y = startY;
	position = 0;
	for (uint32_t left = rows; left;)
	{
		int8_t code = (int8_t)data[position++];
		if (!code)
		{
			y++;
			left--;
			x = startX;
			continue;
		}
		if (code < 0)
		{
			x = (uint8_t)(x - code);
			continue;
		}
		for (int32_t n = code; n; n--, x++)
		{
			uint8_t tileCode = data[position++];
			uint8_t high = tileCode >> 4;
			uint8_t low = tileCode & 0x0F;
			// callerCl / callerDl: what the original leaves in cl / dl
			uint8_t type;
			uint8_t callerCl = tileCode;
			uint8_t callerDl = 0;
			if (high == 0)
			{
				uint8_t part = tileCode % 7;
				if (!part)
					continue;
				type = part - 1;
				callerCl = 7;
				callerDl = part;
			}
			else if (high <= 2)
				type = high + 7;
			else if (high == 3)
			{
				type = low / 3 + 10;
				callerCl = type;
				callerDl = low % 3;
			}
			else
				type = high + 0x0B;
			SetTileTerrainType_33800_33BF0(callerCl, Tile(x, y), type, callerDl);
		}
	}
}
