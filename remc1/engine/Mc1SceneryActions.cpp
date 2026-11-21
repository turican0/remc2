#include "Mc1SceneryActions.h"

#include <cstring>

#include "Mc1Creatures.h"
#include "Mc1Entity.h"
#include "Mc1Files.h"
#include "Mc1Game.h"
#include "Mc1NotPorted.h"
#include "Mc1Sound.h"
#include "Mc1Scenery.h"
#include "Mc1Search.h"
#include "Mc1Shared.h"
#include "Mc1Spells.h"
#include "Mc1Terrain.h"
#include "Mc1Wizard.h"

static uint16_t Tile(int x, int y)
{
	return (uint16_t)((x & 0xFF) | ((y & 0xFF) << 8));
}

static int16_t EntityIndex(Type_Mc1Entity* entity)
{
	return (int16_t)(entity - mc1Entities);
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
	QueueSound_55370_558A0(EntityIndex(entity), -1, 10);
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
	QueueSound_55370_558A0(EntityIndex(entity), -1, 10);
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
		QueueSound_55370_558A0(EntityIndex(entity), -1, 10);
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
			mc1Entities[entity->word_0x2A_42].word_0x30_48 = 2;
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

static Type_Mc1Entity* EntityAt(uint16_t index)
{
	return &mc1Entities[index];
}

static void HitByWizard200(uint16_t attacker)
{
	// a wizard (model 0 or 1, the class not checked) gets 200 at 210h of its player data
	Type_Mc1Entity* wizard = EntityAt(attacker);
	if (wizard->model_0x41_65 != 0 && wizard->model_0x41_65 != 1)
		return;
	if (wizard->playerData_0xA0_160) // port: the original writes to 210h of a null player data
	{
		const int16_t value = 0xC8;
		memcpy(wizard->playerData_0xA0_160 + 0x210, &value, sizeof(value));
	}
}

// the owner from the mail 1 (the mana and the buildings of a player)
static bool TakeOwner(Type_Mc1Entity* entity)
{
	Type_Mc1Mail& mail = entity->mails_0x5A_90[1];
	const uint16_t owner = mail.sender_4;
	if (!owner)
		return false;
	bool changed = false;
	if (owner != entity->owner_0x90_144)
	{
		entity->owner_0x90_144 = owner;
		QueueSound_55370_558A0((int16_t)owner, -1, 4);
		changed = true;
	}
	mail.sender_4 = 0;
	mail.value_0 = 0;
	return changed;
}

int32_t BuildingDamage_29640_29640(Type_Mc1Entity* entity)
{
	// 0 none, 1 hit, 2 destroyed
	entity->word_0x28_40 = 0;
	if (entity->life_0x0C_12 < 0)
		return 2;
	Type_Mc1Mail& mail = entity->mails_0x5A_90[0];
	if (!mail.sender_4)
		return 0;
	entity->life_0x0C_12 -= mail.value_0;
	if (entity->life_0x0C_12 < 0)
	{
		entity->word_0x26_38 = mail.sender_4;
		return 2;
	}
	entity->word_0x28_40 = mail.sender_4;
	mail.value_0 = 0;
	mail.sender_4 = 0;
	return 1;
}

Type_Mc1Entity* SpawnCreature_28D10_28D10(Type_Mc1Entity* entity, Type_Mc1Position* position)
{
	// a random creature of the building
	(void)position;
	entity->rand_0x04_4 = entity->rand_0x04_4 * 0x24A1 + 0x24DF;
	int32_t model;
	uint8_t action;
	switch (entity->rand_0x04_4 % 12)
	{
	case 0:
	case 1:
		model = 4;
		action = 0x19;
		break;
	case 2:
	case 3:
		model = 0x0E;
		action = 0x55;
		break;
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
		model = 0x0D;
		action = 0x4F;
		break;
	default:
		model = 0x0C;
		action = 0x49;
		break;
	}
	Type_Mc1Entity* creature = CreateEntity_373F0_377B0(&featurePosition_AE454_AE444, 5, model);
	if (creature)
		creature->actionIndex_0x46_70 = action;
	return creature;
}

void NearestBuilding_44C90_44C90(Type_Mc1Entity* entity)
{
	// the distance of the wizard of the current player to the nearest building (190h of its player data)
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	Type_Mc1Entity* wizard = EntityAt(state->players_0x340B_13323[state->currentPlayer_0x08_8].entityIndex_0x0A_10);
	const int32_t distance = (int32_t)Distance3D_42340_42680(&entity->position_0x48_72, &wizard->position_0x48_72);
	int32_t nearest;
	memcpy(&nearest, wizard->playerData_0xA0_160 + 0x190, sizeof(nearest));
	if (distance <= nearest)
		memcpy(wizard->playerData_0xA0_160 + 0x190, &distance, sizeof(distance));
}

void BuildingAction_28DC0_28DC0(Type_Mc1Entity* entity)
{
	// the creature building: hit it lets out a creature, full a random one, destroyed the action 35h
	switch (BuildingDamage_29640_29640(entity))
	{
	case 0:
		break;
	case 1:
		if (entity->word_0x1A_26 > 2)
		{
			entity->word_0x1A_26--;
			featurePosition_AE454_AE444 = entity->position_0x48_72;
			featurePosition_AE454_AE444.x += entity->box_0x4E_78[1];
			if (Type_Mc1Entity* creature = CreateEntity_373F0_377B0(&featurePosition_AE454_AE444, 5, 4))
				creature->actionIndex_0x46_70 = 0x19;
			HitByWizard200(entity->word_0x28_40);
		}
		break;
	case 2:
		entity->actionIndex_0x46_70 = 0x35;
		return;
	default:
		return;
	}
	if (entity->mails_0x5A_90[1].sender_4)
	{
		const uint16_t owner = entity->mails_0x5A_90[1].sender_4;
		if (TakeOwner(entity))
		{
			entity->flags_0x10_16 &= ~1u;
			SetSprite_36FA0_37360(entity, 0xB1);
			int16_t colour;
			memcpy(&colour, EntityAt(owner)->playerData_0xA0_160 + 0x30, sizeof(colour));
			entity->sprite_0x56_86 = (int16_t)(entity->sprite_0x56_86 + colour);
		}
	}
	if (!((uint8_t)entity->index_0x3F_63 % 0x28))
	{
		entity->mana_0x8C_140 = entity->word_0x1A_26 << 8;
		const int16_t capacity = entity->word_0x80_128;
		if (capacity > 5 && capacity == entity->word_0x1A_26)
		{
			entity->rand_0x04_4 = entity->rand_0x04_4 * 0x24A1 + 0x24DF;
			const int32_t roll = (int32_t)(entity->rand_0x04_4 % (uint32_t)(int32_t)capacity);
			if (roll > capacity - (capacity >> 4) - 2)
			{
				featurePosition_AE454_AE444 = entity->position_0x48_72;
				featurePosition_AE454_AE444.x += entity->box_0x4E_78[1];
				SpawnCreature_28D10_28D10(entity, &featurePosition_AE454_AE444);
			}
		}
	}
	NearestBuilding_44C90_44C90(entity);
}

void TerrainGradient_41F50_41F50(const Type_Mc1Position* position, Type_Mc1Position* gradient)
{
	// the slope of the tile: x and y to the lower side (z unchanged)
	const uint8_t x = (uint8_t)((uint16_t)position->x >> 8);
	const uint8_t y = (uint8_t)((uint16_t)position->y >> 8);
	const int32_t h00 = mapHeightmap_DC1E0_DC1D0[(y << 8) | x];
	const int32_t h10 = mapHeightmap_DC1E0_DC1D0[(y << 8) | (uint8_t)(x + 1)];
	const int32_t h11 = mapHeightmap_DC1E0_DC1D0[((uint8_t)(y + 1) << 8) | (uint8_t)(x + 1)];
	const int32_t h01 = mapHeightmap_DC1E0_DC1D0[((uint8_t)(y + 1) << 8) | x];
	gradient->x = (int16_t)(h00 - h10 - h11 + h01);
	gradient->y = (int16_t)(h00 + h10 - h11 - h01);
}

Type_Mc1Entity* FindOverlapping_11D10_11D10(Type_Mc1Entity* entity)
{
	// the first entity of the kind (byte_0x42 class, byte_0x43 model, FFh any) overlapping in the tiles of the radius
	const int32_t x = (entity->position_0x48_72.x + 0x80) >> 8;
	const int32_t y = (entity->position_0x48_72.y + 0x80) >> 8;
	const int32_t search = StartSearch_11410_11410(0, (entity->box_0x4E_78[1] + 0xFF) / 256);
	if (!search)
		return nullptr;
	Type_Mc1Entity* entities = mc1Entities;
	int32_t dx;
	int32_t dy;
	while (GetNextSearchOffset_114B0_114B0(search, &dx, &dy) == 1)
	{
		const uint16_t cell = (uint16_t)((((dy + y) & 0xFF) << 8) + ((x + dx) & 0xFF));
		for (Type_Mc1Entity* other = &entities[(int16_t)mapEntityIndex_10C1E0_10C1D0[cell]]; other != entities; other = &entities[other->mapNext_0x14_20])
		{
			bool kind = (int8_t)entity->byte_0x42_66 == -1;
			if (!kind && entity->byte_0x42_66 == other->class_0x40_64)
				kind = (int8_t)entity->byte_0x43_67 == -1 || entity->byte_0x43_67 == other->model_0x41_65;
			if (!kind || entity->id_0x18_24 == other->id_0x18_24 || !EntitiesOverlap_11950_11950(entity, other))
				continue;
			EndSearch_11490_11490(search);
			return other;
		}
	}
	EndSearch_11490_11490(search);
	return nullptr;
}

void MergeMana_277D0_277D0(Type_Mc1Entity* entity, Type_Mc1Entity* other)
{
	// the other mana ball into the entity: the owner a player over a building (class 10), the higher 88h of two players
	entity->mana_0x8C_140 += other->mana_0x8C_140;
	if (!entity->owner_0x90_144 && !other->owner_0x90_144)
	{
	}
	else if (!entity->owner_0x90_144)
		entity->owner_0x90_144 = other->owner_0x90_144;
	else if (other->owner_0x90_144)
	{
		Type_Mc1Entity* owner = EntityAt(entity->owner_0x90_144);
		Type_Mc1Entity* otherOwner = EntityAt(other->owner_0x90_144);
		if (owner->class_0x40_64 == 0x0A && otherOwner->class_0x40_64 == 0x0A)
		{
		}
		else if (owner->class_0x40_64 == 0x0A)
			entity->owner_0x90_144 = other->owner_0x90_144;
		else if (otherOwner->class_0x40_64 == 0x0A || owner == otherOwner)
		{
		}
		else if (owner->dword_0x88_136 <= otherOwner->dword_0x88_136)
			entity->owner_0x90_144 = other->owner_0x90_144;
	}
	FreeEntity_41E90_421D0(other);
}

void ManaAction_27030_27030(Type_Mc1Entity* entity)
{
	// the mana ball: the owner by the mail 1, pushed by the mail 4, pulled by the wizard (flag 40h), else it falls,
	// bounces, merges and rolls down the slope
	if (entity->mails_0x5A_90[1].sender_4 && TakeOwner(entity))
		entity->flags_0x10_16 &= ~0x40u;
	if (const uint16_t pusher = entity->mails_0x5A_90[4].sender_4)
	{
		entity->word_0x1E_30 = (int16_t)GetYawAngle_42150_42490((axis_3d*)&entity->position_0x48_72, (axis_3d*)&EntityAt(pusher)->position_0x48_72);
		featurePosition_AE454_AE444.x = 0;
		featurePosition_AE454_AE444.y = 0;
		featurePosition_AE454_AE444.z = 0;
		MoveEntity_41EC0_42200((axis_3d*)&featurePosition_AE454_AE444, (uint16_t)entity->word_0x1E_30, 0, 4);
		entity->target_0x96_150.x += featurePosition_AE454_AE444.x;
		entity->mails_0x5A_90[4].sender_4 = 0;
		entity->target_0x96_150.y += featurePosition_AE454_AE444.y;
	}
	if (entity->flags_0x10_16 & 0x40)
	{
		Type_Mc1Entity* wizard = EntityAt(entity->target_0x92_146);
		if (wizard->class_0x40_64 != 3 || wizard->model_0x41_65 != 3)
		{
			entity->flags_0x10_16 &= ~0x40u;
			return;
		}
		entity->word_0x2E_46 = 0x80;
		entity->word_0x1E_30 = (int16_t)GetYawAngle_42150_42490((axis_3d*)&entity->position_0x48_72, (axis_3d*)&wizard->position_0x48_72);
		const int32_t distance = (int32_t)Distance2D_423D0_42710(&entity->position_0x48_72, &wizard->position_0x48_72);
		if (distance > 0x400)
		{
			entity->flags_0x10_16 &= ~0x40u;
			return;
		}
		featurePosition_AE454_AE444 = entity->position_0x48_72;
		if (distance < 0x10)
		{
			featurePosition_AE454_AE444.x = wizard->position_0x48_72.x;
			featurePosition_AE454_AE444.y = wizard->position_0x48_72.y;
			const int16_t z = wizard->position_0x48_72.z;
			if (featurePosition_AE454_AE444.z < z)
				featurePosition_AE454_AE444.z += 0x20;
			else if (featurePosition_AE454_AE444.z > z + 0x200)
				featurePosition_AE454_AE444.z -= 0x20;
		}
		else
			MoveEntity_41EC0_42200((axis_3d*)&featurePosition_AE454_AE444, (uint16_t)entity->word_0x1E_30, 0, 0x10);
		const int16_t ground = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&featurePosition_AE454_AE444);
		if (ground > featurePosition_AE454_AE444.z)
			featurePosition_AE454_AE444.z = ground;
		CopyEntityPosition_41C70_41FB0(entity, &featurePosition_AE454_AE444);
		return;
	}
	if (!entity->byte_0x3A_58)
		return;
	Type_Mc1Position& speed = entity->target_0x96_150;
	if (speed.x < -0x40)
		speed.x = -0x40;
	if (speed.x > 0x40)
		speed.x = 0x40;
	if (speed.y < -0x40)
		speed.y = -0x40;
	if (speed.y > 0x40)
		speed.y = 0x40;
	featurePosition_AE454_AE444 = entity->position_0x48_72;
	featurePosition_AE454_AE444.x = (int16_t)(entity->position_0x48_72.x + speed.x);
	featurePosition_AE454_AE444.y = (int16_t)(entity->position_0x48_72.y + speed.y);
	const int16_t fall = entity->word_0x2E_46;
	featurePosition_AE454_AE444.z = (int16_t)(featurePosition_AE454_AE444.z + fall);
	entity->word_0x2E_46 = (int16_t)(fall - 0x10);
	if (entity->word_0x2E_46 < -0x80)
		entity->word_0x2E_46 = -0x80;
	const int16_t ground = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&featurePosition_AE454_AE444);
	if (ground > featurePosition_AE454_AE444.z)
	{
		// the bounce: a quarter back up, small ones stop
		entity->word_0x2E_46 = (int16_t)-(entity->word_0x2E_46 / 4);
		if (entity->word_0x2E_46 <= 0x10)
			entity->word_0x2E_46 = 0;
		featurePosition_AE454_AE444.z = ground;
	}
	CopyEntityPosition_41C70_41FB0(entity, &featurePosition_AE454_AE444);
	if (ground == featurePosition_AE454_AE444.z)
	{
		if (Type_Mc1Entity* other = FindOverlapping_11D10_11D10(entity))
			MergeMana_277D0_277D0(entity, other);
		TerrainGradient_41F50_41F50(&entity->position_0x48_72, &featurePosition_AE454_AE444);
		speed.x = (int16_t)(speed.x + featurePosition_AE454_AE444.x);
		speed.y = (int16_t)(speed.y + featurePosition_AE454_AE444.y);
		speed.x = (int16_t)(speed.x * 250 / 256);
		speed.y = (int16_t)(speed.y * 250 / 256);
	}
	SetManaSprite_274D0_274D0(entity);
}

void SmokeAction_25130_25130(Type_Mc1Entity* entity)
{
	// the smoke of the dying wizard: puffs (class 10 model 0) around in the ring word_0x1A, freed after its life
	const int32_t life = entity->life_0x0C_12;
	entity->life_0x0C_12 = life - 1;
	if (life < 0)
	{
		MarkToFree_41E80_421C0(entity);
		return;
	}
	if (!(entity->flags_0x10_16 & 2))
	{
		entity->flags_0x10_16 |= 2;
		QueueSound_55370_558A0(EntityIndex(entity), -1, 3);
	}
	const int32_t search = StartSearch_11410_11410(entity->word_0x1A_26, entity->word_0x1A_26);
	if (!search)
		return;
	int32_t dx;
	int32_t dy;
	while (GetNextSearchOffset_114B0_114B0(search, &dx, &dy) == 1)
	{
		if ((int32_t)(NextRand(entity) % 0x9D) / 0x4F * 2 - 1 <= 0)
			continue;
		Type_Mc1Position position;
		position.x = (int16_t)(dx * 0xC0 + entity->position_0x48_72.x - 0x60 + (int32_t)(NextRand(entity) % 0x81) - 0x40);
		position.y = (int16_t)(dy * 0xC0 + entity->position_0x48_72.y - 0x60 + (int32_t)(NextRand(entity) % 0x81) - 0x40);
		position.z = entity->position_0x48_72.z;
		if (Type_Mc1Entity* puff = CreateEntity_373F0_377B0(&position, 0x0A, 0))
		{
			puff->id_0x18_24 = entity->id_0x18_24;
			puff->word_0x1E_30 = entity->word_0x1E_30;
			puff->flags_0x10_16 |= (entity->flags_0x10_16 & 0x10000) | 0x80;
		}
	}
	EndSearch_11490_11490(search);
}

void WizardManaAction_275C0_275C0(Type_Mc1Entity* entity)
{
	// the mana of the dead wizard on the ground: taken by a wizard (the mail 1) with the mana balls it owned
	entity->position_0x48_72.z = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&entity->position_0x48_72);
	const uint16_t taker = entity->mails_0x5A_90[1].sender_4;
	if (!taker)
		return;
	if (EntityAt(taker)->class_0x40_64 == 3 && !entity->owner_0x90_144)
	{
		Type_Mc1Entity* entities = mc1Entities;
		for (int i = 1; i < mc1EntityCount; i++)
		{
			if (entities[i].owner_0x90_144 == (uint16_t)EntityIndex(entity))
				entities[i].owner_0x90_144 = taker;
		}
	}
	entity->mails_0x5A_90[1].sender_4 = 0;
	MarkToFree_41E80_421C0(entity);
}

int32_t NextFrame_42510_42850(Type_Mc1Entity* entity)
{
	// 1 at the last frame of the sprite
	if (entity->frame_0x58_88 >= entity->frameCount_0x59_89)
		return 1;
	entity->frame_0x58_88++;
	return 0;
}

void PuffAction_24F60_24F60(Type_Mc1Entity* entity)
{
	// a puff of the smoke: the first time it hurts around, burns the tile and starts to rise, then it animates
	if (entity->word_0x1A_26 & 3)
	{
		entity->word_0x1A_26--;
		return;
	}
	const int32_t life = entity->life_0x0C_12;
	entity->life_0x0C_12 = life - 1;
	if (life < 0)
	{
		MarkToFree_41E80_421C0(entity);
		return;
	}
	entity->flags_0x10_16 &= ~1u;
	const int16_t ground = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&entity->position_0x48_72);
	if (!(entity->flags_0x10_16 & 2))
	{
		const uint16_t tile = Tile((entity->position_0x48_72.x + 0x80) >> 8, (entity->position_0x48_72.y + 0x80) >> 8);
		// 33800 gets cl and dl as the registers are left: cl the flags byte 2 (dl after 11F50 not known: 0), after
		// 120B0 cl 0 (its last cell row) and dl the radius
		uint8_t callerCl = (uint8_t)(entity->flags_0x10_16 >> 16);
		uint8_t callerDl = 0;
		if (!(entity->flags_0x10_16 & 0x10000))
		{
			SendMailAround_120B0_120B0(entity, 0, (uint16_t)entity->word_0x2C_44);
			callerCl = 0;
			callerDl = (uint8_t)((entity->box_0x4E_78[1] + 0xFF) / 256);
		}
		const uint8_t type = mapTerrainType_CC1E0_CC1D0[tile];
		if (type == 0x1A)
			SetTileTerrainType_33800_33BF0(callerCl, tile, 0x14, callerDl);
		else if (type == 0x0A)
			SetTileTerrainType_33800_33BF0(callerCl, tile, 0x15, callerDl);
		else if (type == 0x0B)
			SetTileTerrainType_33800_33BF0(callerCl, tile, 0x16, callerDl);
		else if (type && (type < 6 || type > 0x22) && (mapAngle_FC1E0_FC1D0[tile] & 7) != 1
			&& entity->position_0x48_72.z - ground <= 0x80 && !OnFlatWater(entity))
			ChangeHeightsAround_40D30_41070(entity, 0, 0, (int16_t)-(int32_t)(NextRand(entity) % 7), 1);
		entity->flags_0x10_16 |= 2;
		entity->word_0x2E_46 = (int16_t)(NextRand(entity) % 0x41 - 0x20);
		QueueSound_55370_558A0(EntityIndex(entity), -1, 3);
	}
	ClampHeight_42000_42340(&entity->position_0x48_72, ground, 0, 0, entity->word_0x2E_46);
	NextFrame_42510_42850(entity);
}

void ExplosionAction_25760_25760(Type_Mc1Entity* entity)
{
	// the explosion: the damage word_0x2C as the mail 1 around while it animates
	entity->word_0x1A_26++;
	const int32_t life = entity->life_0x0C_12;
	entity->life_0x0C_12 = life - 1;
	if (life < 0)
	{
		MarkToFree_41E80_421C0(entity);
		return;
	}
	NextFrame_42510_42850(entity);
	SendMailAround_120B0_120B0(entity, 1, (uint16_t)entity->word_0x2C_44);
}

void PushManaAction_29920_29960(Type_Mc1Entity* entity)
{
	// the mana balls (model 27h) nearer than C40000h (squared) get the mail 4 (pushed away) while it lives
	const int32_t life = entity->life_0x0C_12;
	entity->life_0x0C_12 = life - 1;
	if (life < 0)
	{
		MarkToFree_41E80_421C0(entity);
		return;
	}
	for (Type_Mc1Entity* mana = mc1Settings_AE408_AE3F8->entityLists_0x8E1A_36378[22]; mana > mc1Entities; mana = mana->next_0x00_0)
	{
		if (mana->model_0x41_65 != 0x27 || (int32_t)Distance2DSquared_42410_42750(&entity->position_0x48_72, &mana->position_0x48_72) >= 0xC40000)
			continue;
		mana->mails_0x5A_90[4].value_0 = 100;
		mana->mails_0x5A_90[4].sender_4 = (uint16_t)EntityIndex(entity);
	}
}

void NearestFire_44C10_44F50(Type_Mc1Entity* entity)
{
	// the distance of the wizard of the current player to the nearest fire (18Ch of its player data)
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	Type_Mc1Entity* wizard = EntityAt(state->players_0x340B_13323[state->currentPlayer_0x08_8].entityIndex_0x0A_10);
	const int32_t distance = (int32_t)Distance3D_42340_42680(&entity->position_0x48_72, &wizard->position_0x48_72);
	int32_t nearest;
	memcpy(&nearest, wizard->playerData_0xA0_160 + 0x18C, sizeof(nearest));
	if (distance <= nearest)
		memcpy(wizard->playerData_0xA0_160 + 0x18C, &distance, sizeof(distance));
}

void FireAction_252D0_252D0(Type_Mc1Entity* entity)
{
	// a fire: grows (word_0x1A to 7, the sprite with it) while the life is over 0Ch, then shrinks and sometimes leaves a
	// smoke (model 0Dh); burns around by 124F0 (without the flag 10000h); in the water gone
	const int32_t life = entity->life_0x0C_12;
	entity->life_0x0C_12 = life - 1;
	if (life >= 0)
	{
		NearestFire_44C10_44F50(entity);
		if (entity->life_0x0C_12 >= 0x0C)
		{
			if (entity->word_0x1A_26 <= 6)
			{
				entity->sprite_0x56_86++;
				entity->word_0x1A_26++;
			}
		}
		else if (entity->word_0x1A_26 > 0)
		{
			entity->sprite_0x56_86--;
			entity->word_0x1A_26--;
			if (!(entity->flags_0x10_16 & 0x80) && !(NextRand(entity) % 7))
			{
				if (Type_Mc1Entity* smoke = CreateEntity_373F0_377B0(&entity->position_0x48_72, 0x0A, 0x0D))
				{
					smoke->word_0x1A_26 = 0x64;
					smoke->life_0x0C_12 = 0x0F;
					smoke->id_0x18_24 = entity->id_0x18_24;
					smoke->sprite_0x56_86 = (int16_t)(smoke->sprite_0x56_86 + 2);
				}
			}
		}
		entity->position_0x48_72.z = (int16_t)(GetTerrainAlt_11F50_11F50((axis_3d*)&entity->position_0x48_72) + entity->word_0x2E_46);
		if (GetTileTypeBit_11810_11810(&entity->position_0x48_72) == 1)
			MarkToFree_41E80_421C0(entity);
	}
	else
		MarkToFree_41E80_421C0(entity);
	if (!(entity->flags_0x10_16 & 0x10000))
		SendFireMail_124F0_124F0(entity, 0, (uint16_t)entity->word_0x2C_44);
}

void FireSmokeAction_257B0_257B0(Type_Mc1Entity* entity)
{
	// the smoke of a fire: rises by word_0x7E (slowing 4 to 40h-80h), drifts 1Eh by its yaw for 16 frames
	const int32_t life = entity->life_0x0C_12;
	entity->life_0x0C_12 = life - 1;
	if (life < 0)
	{
		MarkToFree_41E80_421C0(entity);
		return;
	}
	featurePosition_AE454_AE444 = entity->position_0x48_72;
	entity->word_0x7E_126 = (int16_t)(entity->word_0x7E_126 - 4);
	if (entity->word_0x7E_126 < 0x40)
		entity->word_0x7E_126 = 0x40;
	if (entity->word_0x7E_126 > 0x80)
		entity->word_0x7E_126 = 0x80;
	featurePosition_AE454_AE444.z = (int16_t)(featurePosition_AE454_AE444.z + entity->word_0x7E_126);
	const int16_t ground = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&entity->position_0x48_72);
	if (featurePosition_AE454_AE444.z < ground)
		featurePosition_AE454_AE444.z = ground;
	entity->word_0x1A_26++;
	if (entity->word_0x1A_26 < 0x10)
	{
		MoveEntity_41EC0_42200((axis_3d*)&featurePosition_AE454_AE444, (uint16_t)entity->word_0x1E_30, 0, 0x1E);
		if (!(entity->word_0x1A_26 & 1))
			entity->sprite_0x56_86++;
	}
	if (entity->life_0x0C_12 < 6 && entity->sprite_0x56_86 > 0x43)
		entity->sprite_0x56_86--;
	CopyEntityPosition_41C70_41FB0(entity, &featurePosition_AE454_AE444);
}

void BuildingFallAction_25410_25410(Type_Mc1Entity* entity)
{
	// the destroyed building animates (the sound 1Bh at the start) and is gone at the end of its life
	const int32_t life = entity->life_0x0C_12;
	entity->life_0x0C_12 = life - 1;
	if (life < 0)
	{
		MarkToFree_41E80_421C0(entity);
		return;
	}
	NextFrame_42510_42850(entity);
	if (entity->flags_0x10_16 & 2)
		return;
	entity->flags_0x10_16 |= 2;
	QueueSound_55370_558A0(EntityIndex(entity), -1, 0x1B);
}

// the life counted down, gone below 0
static bool LifeEnded(Type_Mc1Entity* entity)
{
	const int32_t life = entity->life_0x0C_12;
	entity->life_0x0C_12 = life - 1;
	if (life >= 0)
		return false;
	MarkToFree_41E80_421C0(entity);
	return true;
}

static void QueueEntitySound(Type_Mc1Entity* entity, uint8_t sound)
{
	QueueSound_55370_558A0(EntityIndex(entity), -1, sound);
}

static Type_Mc1Entity* Entities()
{
	return mc1Entities;
}

Type_Mc1Entity* FindOverlappingMana_11C00_11C00(Type_Mc1Entity* entity)
{
	// a new mana pile (class 10 model 27h) overlapping in the tiles of the radius
	const int32_t x = (entity->position_0x48_72.x + 0x80) >> 8;
	const int32_t y = (entity->position_0x48_72.y + 0x80) >> 8;
	const int32_t search = StartSearch_11410_11410(0, (entity->box_0x4E_78[1] + 0xFF) / 256);
	if (!search)
		return nullptr;
	int32_t dx;
	int32_t dy;
	while (GetNextSearchOffset_114B0_114B0(search, &dx, &dy) == 1)
	{
		const uint16_t cell = (uint16_t)((((dy + y) & 0xFF) << 8) + ((x + dx) & 0xFF));
		for (Type_Mc1Entity* other = &Entities()[(int16_t)mapEntityIndex_10C1E0_10C1D0[cell]]; other != Entities(); other = &Entities()[other->mapNext_0x14_20])
		{
			if (!(other->flags_0x10_16 & 8) || other->class_0x40_64 != 0x0A || other->model_0x41_65 != 0x27 || !EntitiesOverlap_11950_11950(entity, other))
				continue;
			EndSearch_11490_11490(search);
			return other;
		}
	}
	EndSearch_11490_11490(search);
	return nullptr;
}

int32_t EntityNear_11E50_11E50(Type_Mc1Position* position, uint8_t entityClass, uint8_t model)
{
	// an entity of the class and model within 80h in the 2x2 tiles at the position
	const int16_t x = (int16_t)((position->x - 0x80) >> 8);
	const int16_t y = (int16_t)((position->y - 0x80) >> 8);
	for (int16_t dy = 0; dy < 2; dy++)
	{
		for (int16_t dx = 0; dx < 2; dx++)
		{
			const uint16_t cell = (uint16_t)((((y + dy) & 0xFF) << 8) + ((x + dx) & 0xFF));
			for (Type_Mc1Entity* other = &Entities()[(int16_t)mapEntityIndex_10C1E0_10C1D0[cell]]; other > Entities(); other = &Entities()[other->mapNext_0x14_20])
			{
				if (other->class_0x40_64 == entityClass && other->model_0x41_65 == model && Distance3D_42340_42680(position, &other->position_0x48_72) <= 0x80)
					return 1;
			}
		}
	}
	return 0;
}

void LifeAction_252B0_252B0(Type_Mc1Entity* entity)
{
	LifeEnded(entity);
}

void LifeAction_253F0_253F0(Type_Mc1Entity* entity)
{
	LifeEnded(entity);
}

void RisingSmokeAction_258A0_258A0(Type_Mc1Entity* entity)
{
	// rises by word_0x7E (slowing 4 to 40h-80h), drifts 1Eh by its yaw for 16 frames, smaller at the end
	if (LifeEnded(entity))
		return;
	featurePosition_AE454_AE444 = entity->position_0x48_72;
	entity->word_0x7E_126 = (int16_t)(entity->word_0x7E_126 - 4);
	if (entity->word_0x7E_126 < 0x40)
		entity->word_0x7E_126 = 0x40;
	if (entity->word_0x7E_126 > 0x80)
		entity->word_0x7E_126 = 0x80;
	featurePosition_AE454_AE444.z = (int16_t)(featurePosition_AE454_AE444.z + entity->word_0x7E_126);
	const int16_t ground = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&entity->position_0x48_72);
	if (featurePosition_AE454_AE444.z < ground)
		featurePosition_AE454_AE444.z = ground;
	entity->word_0x1A_26++;
	if (entity->word_0x1A_26 < 0x10)
	{
		MoveEntity_41EC0_42200((axis_3d*)&featurePosition_AE454_AE444, (uint16_t)entity->word_0x1E_30, 0, 0x1E);
		if (!(entity->word_0x1A_26 & 1))
			entity->sprite_0x56_86++;
	}
	if (entity->life_0x0C_12 < 6)
		entity->sprite_0x56_86--;
	CopyEntityPosition_41C70_41FB0(entity, &featurePosition_AE454_AE444);
}

void WaterTrailAction_25990_25990(Type_Mc1Entity* entity)
{
	// wanders over the water (gone after 8 tiles off it) leaving the model 0Bh behind
	if (GetTileAngleBit_11760_11760((axis_3d*)&entity->position_0x48_72) & 1)
		entity->word_0x1A_26++;
	else if (entity->word_0x1A_26 > 0)
		entity->word_0x1A_26--;
	const int32_t life = entity->life_0x0C_12;
	entity->life_0x0C_12 = life - 1;
	if (life < 0 || entity->word_0x1A_26 > 8)
	{
		MarkToFree_41E80_421C0(entity);
		return;
	}
	NextRand(entity);
	entity->word_0x1E_30 = (int16_t)(((uint16_t)entity->word_0x1E_30 + entity->rand_0x04_4 % 0x5B - 0x2D) & 0x7FF);
	MoveEntity_41EC0_42200((axis_3d*)&entity->position_0x48_72, (uint16_t)entity->word_0x1E_30, 0, 0x100);
	if (Type_Mc1Entity* trail = CreateEntity_373F0_377B0(&entity->position_0x48_72, 0x0A, 0x0B))
	{
		trail->box_0x4E_78[1] = entity->box_0x4E_78[1];
		trail->box_0x4E_78[2] = entity->box_0x4E_78[2];
		trail->box_0x4E_78[3] = entity->box_0x4E_78[3];
		trail->life_0x0C_12 = 0x0A;
		trail->id_0x18_24 = entity->id_0x18_24;
	}
}

static int16_t Clamp16(int16_t value, int16_t low, int16_t high)
{
	if (value < low)
		value = low;
	if (value > high)
		value = high;
	return value;
}

void BouncingRockAction_25A60_25A60(Type_Mc1Entity* entity)
{
	// bounces by target_0x96 (the speed) and word_0x2E (the vertical speed): a splash (model 5) on water, else fire
	if (LifeEnded(entity))
		return;
	if (!(entity->flags_0x10_16 & 2))
		entity->flags_0x10_16 |= 2;
	entity->target_0x96_150.x = Clamp16(entity->target_0x96_150.x, -0x50, 0x50);
	entity->target_0x96_150.y = Clamp16(entity->target_0x96_150.y, -0x50, 0x50);
	featurePosition_AE454_AE444 = entity->position_0x48_72;
	featurePosition_AE454_AE444.x = (int16_t)(entity->position_0x48_72.x + entity->target_0x96_150.x);
	featurePosition_AE454_AE444.y = (int16_t)(entity->position_0x48_72.y + entity->target_0x96_150.y);
	featurePosition_AE454_AE444.z = (int16_t)(featurePosition_AE454_AE444.z + entity->word_0x2E_46);
	entity->word_0x2E_46 = Clamp16((int16_t)(entity->word_0x2E_46 - 0x1C), -0x180, 0x100);
	const int16_t ground = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&featurePosition_AE454_AE444);
	if (ground > featurePosition_AE454_AE444.z)
	{
		featurePosition_AE454_AE444.z = ground;
		entity->word_0x2E_46 = (int16_t)-(entity->word_0x2E_46 / 4);
		if (GetTileTypeBit_11810_11810(&entity->position_0x48_72) == 1)
		{
			if (Type_Mc1Entity* splash = CreateEntity_373F0_377B0(&featurePosition_AE454_AE444, 0x0A, 5))
			{
				splash->id_0x18_24 = entity->id_0x18_24;
				MarkToFree_41E80_421C0(entity);
			}
		}
		else
		{
			if (!EntityNear_11E50_11E50(&featurePosition_AE454_AE444, 0x0A, 6))
			{
				if (Type_Mc1Entity* fire = CreateEntity_373F0_377B0(&featurePosition_AE454_AE444, 0x0A, 6))
				{
					fire->id_0x18_24 = entity->id_0x18_24;
					fire->life_0x0C_12 = 0x1E;
					fire->word_0x2C_44 = (int16_t)(fire->word_0x2C_44 * 3);
					entity->word_0x1A_26 = 0;
				}
			}
			if (entity->word_0x2E_46 <= 0x1C)
				entity->word_0x2E_46 = 0;
		}
	}
	entity->word_0x1A_26++;
	CopyEntityPosition_41C70_41FB0(entity, &featurePosition_AE454_AE444);
	if (ground != featurePosition_AE454_AE444.z)
		return;
	TerrainGradient_41F50_41F50(&entity->position_0x48_72, &featurePosition_AE454_AE444);
	entity->target_0x96_150.x = (int16_t)(entity->target_0x96_150.x + featurePosition_AE454_AE444.x);
	entity->target_0x96_150.y = (int16_t)(entity->target_0x96_150.y + featurePosition_AE454_AE444.y);
	entity->target_0x96_150.x = (int16_t)(entity->target_0x96_150.x * 250 / 256);
	entity->target_0x96_150.y = (int16_t)(entity->target_0x96_150.y * 250 / 256);
}

// the ring of fire growing by 2 tiles (the sound 1Eh at the start): fires (model 0) in it, the damage by word_0x2C
static void FireRing(Type_Mc1Entity* entity, int16_t sizes)
{
	if (LifeEnded(entity))
		return;
	if (!(entity->flags_0x10_16 & 2))
	{
		entity->flags_0x10_16 |= 0x10002;
		QueueEntitySound(entity, 0x1E);
	}
	SetBox_37130_374F0(entity, (int16_t)(entity->word_0x1A_26 * 3 * 256 / 4), 0x200);
	SendMailAround_120B0_120B0(entity, 0, (uint16_t)((int32_t)(uint16_t)entity->word_0x2C_44 / entity->maxLife_0x08_8));
	const int32_t search = StartSearch_11410_11410(entity->word_0x1A_26, entity->word_0x1A_26);
	if (search)
	{
		NextRand(entity);
		int32_t dx;
		int32_t dy;
		while (GetNextSearchOffset_114B0_114B0(search, &dx, &dy) == 1)
		{
			Type_Mc1Position position;
			NextRand(entity);
			position.x = (int16_t)(dx * 0xA0 + entity->position_0x48_72.x - 0x60 + (int32_t)(entity->rand_0x04_4 % 0x81) - 0x40);
			NextRand(entity);
			position.y = (int16_t)(dy * 0xA0 + entity->position_0x48_72.y - 0x60 + (int32_t)(entity->rand_0x04_4 % 0x81) - 0x40);
			position.z = entity->position_0x48_72.z;
			if (Type_Mc1Entity* fire = CreateEntity_373F0_377B0(&position, 0x0A, 0))
			{
				fire->id_0x18_24 = entity->id_0x18_24;
				fire->word_0x1E_30 = entity->word_0x1E_30;
				fire->flags_0x10_16 |= 0x10080;
				SetBox_37130_374F0(fire, 0x200, 0x200);
				fire->word_0x1A_26 = 0;
			}
		}
		EndSearch_11490_11490(search);
	}
	entity->word_0x1A_26 = (int16_t)((entity->word_0x1A_26 + 2) % sizes);
}

void FireRingAction_25CE0_25CE0(Type_Mc1Entity* entity)
{
	FireRing(entity, 0x0B);
}

void VolcanoAction_25EC0_25EC0(Type_Mc1Entity* entity)
{
	// a volcano (one at a time in state word_0x24): lava (model 10h) every step, rocks (class 9 model 0) at the start
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	if (entity->word_0x1A_26 > 0x9C4)
	{
		NextRand(entity);
		if (!(entity->rand_0x04_4 % 0x64) && !state->word_0x24_36)
		{
			const int16_t z = entity->position_0x48_72.z;
			entity->position_0x48_72.z = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&entity->position_0x48_72);
			if (z != entity->position_0x48_72.z)
			{
				MarkToFree_41E80_421C0(entity);
				return;
			}
			entity->word_0x1A_26 = 0;
		}
	}
	bool erupt = false;
	if (entity->word_0x1A_26 < 0x80 && (entity->word_0x1A_26 & 0x0F))
	{
		NextRand(entity);
		erupt = !(entity->rand_0x04_4 % 5);
	}
	if (!erupt && entity->word_0x1A_26)
	{
		entity->word_0x1A_26++;
		return;
	}
	const int16_t z = entity->position_0x48_72.z;
	entity->position_0x48_72.z = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&entity->position_0x48_72);
	if (z != entity->position_0x48_72.z)
	{
		MarkToFree_41E80_421C0(entity);
		return;
	}
	if (!entity->word_0x1A_26)
	{
		Type_Mc1Entity* previous = &Entities()[state->word_0x24_36];
		if (previous > Entities())
			previous->word_0x1A_26 = 0xFA;
		state->word_0x24_36 = (uint16_t)EntityIndex(entity);
		if (Type_Mc1Entity* cloud = CreateEntity_373F0_377B0(&entity->position_0x48_72, 0x0A, 0x13))
		{
			cloud->id_0x18_24 = entity->id_0x18_24;
			Type_Mc1Entity* old = &Entities()[state->word_0x26_38];
			if (old > Entities())
				MarkToFree_41E80_421C0(old);
			state->word_0x26_38 = (uint16_t)EntityIndex(cloud);
		}
	}
	if (Type_Mc1Entity* lava = CreateEntity_373F0_377B0(&entity->position_0x48_72, 0x0A, 0x10))
	{
		lava->id_0x18_24 = entity->id_0x18_24;
		NextRand(entity);
		lava->rand_0x04_4 = entity->rand_0x04_4;
	}
	entity->word_0x1E_30 = (int16_t)(entity->word_0x1E_30 + 0x500); // 25EC0: add byte [ebx+1Fh], 5
	if (!entity->word_0x1A_26)
	{
		if (Type_Mc1Entity* rock = CreateEntity_373F0_377B0(&entity->position_0x48_72, 9, 0))
		{
			rock->id_0x18_24 = entity->id_0x18_24;
			rock->word_0x20_32 = -0x182;
			rock->byte_0x44_68 = 0x0A;
			rock->byte_0x45_69 = 0x11;
			rock->life_0x0C_12 = 1;
			rock->word_0x1E_30 = (int16_t)(entity->word_0x1E_30 & 0x7FF);
			rock->target_0x96_150 = entity->position_0x48_72;
			MoveEntity_41EC0_42200((axis_3d*)&rock->target_0x96_150, (uint16_t)rock->word_0x1E_30, 0, 0x600);
			rock->target_0x96_150.z = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&rock->target_0x96_150);
		}
	}
	if (entity->word_0x1A_26 >= 0x7F)
	{
		MarkToFree_41E80_421C0(entity);
		state->word_0x24_36 = 0;
	}
	entity->word_0x1A_26++;
}

void FireStormAction_26140_26140(Type_Mc1Entity* entity)
{
	// fires (model 0Dh, 4 or 2 yaws) on a random half of the tile every 2nd step
	if (!LifeEnded(entity))
	{
		entity->word_0x1A_26 = 0;
		const int32_t search = StartSearch_11410_11410(0, entity->word_0x1A_26);
		if (search)
		{
			int32_t dx;
			int32_t dy;
			while (GetNextSearchOffset_114B0_114B0(search, &dx, &dy) == 1)
			{
				NextRand(entity);
				if ((int32_t)(entity->rand_0x04_4 % 0x9D) / 0x4F * 2 - 1 <= 0)
					continue;
				Type_Mc1Position position;
				NextRand(entity);
				position.x = (int16_t)(dx * 0xC0 + entity->position_0x48_72.x - 0x60 + (int32_t)(entity->rand_0x04_4 % 0x81) - 0x40);
				NextRand(entity);
				position.y = (int16_t)(dy * 0xC0 + entity->position_0x48_72.y - 0x60 + (int32_t)(entity->rand_0x04_4 % 0x81) - 0x40);
				position.z = entity->position_0x48_72.z;
				if (!(entity->life_0x0C_12 & 1))
					continue;
				for (uint16_t yaw = (uint16_t)((entity->life_0x0C_12 / 2 & 1) << 8); yaw < 0x800; yaw += 0x200)
				{
					if (Type_Mc1Entity* fire = CreateEntity_373F0_377B0(&position, 0x0A, 0x0D))
					{
						fire->id_0x18_24 = entity->id_0x18_24;
						fire->word_0x1E_30 = (int16_t)yaw;
					}
				}
			}
			EndSearch_11490_11490(search);
		}
		entity->position_0x48_72.z = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&entity->position_0x48_72);
	}
	SendMailAround_120B0_120B0(entity, 0, (uint16_t)entity->word_0x2C_44);
}

void BlastAction_262D0_262D0(Type_Mc1Entity* entity)
{
	// the damage once (the sound 18h), then gone
	entity->word_0x1A_26++;
	if (LifeEnded(entity))
		return;
	if (entity->flags_0x10_16 & 2)
		return;
	SendMailAround_120B0_120B0(entity, 0, (uint16_t)entity->word_0x2C_44);
	QueueEntitySound(entity, 0x18);
	entity->life_0x0C_12 = 1;
	entity->flags_0x10_16 |= 2;
}

void MailOnceAction_26360_26360(Type_Mc1Entity* entity)
{
	// animated, the mail 3 once
	entity->word_0x1A_26++;
	if (LifeEnded(entity))
		return;
	NextFrame_42510_42850(entity);
	if (entity->flags_0x10_16 & 2)
		return;
	entity->flags_0x10_16 |= 2;
	SendMailAround_120B0_120B0(entity, 3, (uint16_t)entity->word_0x2C_44);
}

void MailAction_263C0_263C0(Type_Mc1Entity* entity)
{
	// animated, the mail 4 every step
	entity->word_0x1A_26++;
	if (LifeEnded(entity))
		return;
	NextFrame_42510_42850(entity);
	SendMailAround_120B0_120B0(entity, 4, (uint16_t)entity->word_0x2C_44);
}

void TeleportAction_26A60_26A60(Type_Mc1Entity* entity)
{
	// a wizard entering it facing it is moved to target_0x96 (the sounds 15h at the start, 16h, 14h at the end)
	if (!(entity->flags_0x10_16 & 2))
	{
		entity->flags_0x10_16 |= 2;
		QueueEntitySound(entity, 0x15);
	}
	if (entity->life_0x0C_12 > 0 && --entity->life_0x0C_12 == 0)
	{
		MarkToFree_41E80_421C0(entity);
		QueueEntitySound(entity, 0x14);
		return;
	}
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	for (uint16_t i = 0; i < state->playerCount_0x0A_10; i++)
	{
		Type_Mc1Entity* wizard = &Entities()[state->players_0x340B_13323[i].entityIndex_0x0A_10];
		if (!EntitiesOverlap_11950_11950(entity, wizard))
			continue;
		const uint16_t yaw = (uint16_t)GetYawAngle_42150_42490((axis_3d*)&wizard->position_0x48_72, (axis_3d*)&entity->position_0x48_72);
		if ((uint16_t)AngleDistance_42210_42550((uint16_t)wizard->word_0x1E_30, yaw) >= 0xAA)
			continue;
		const uint8_t* params = entity->typeParams_0x9C_156;
		int16_t height;
		memcpy(&height, params + 0x0C, sizeof(height));
		entity->target_0x96_150.z = (int16_t)(GetTerrainAlt_11F50_11F50((axis_3d*)&entity->target_0x96_150) + height);
		QueueEntitySound(entity, 0x16);
		CopyEntityPosition_41C70_41FB0(wizard, &entity->target_0x96_150);
		SetFlashOfPlayer_44BE0_44F20(wizard, 6);
	}
	entity->position_0x48_72.z = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&entity->position_0x48_72);
}

void CircleOwnerAction_26C00_26C00(Type_Mc1Entity* entity)
{
	// circles the entity of word_0x2A 100h over the ground, gone when its life is 1
	Type_Mc1Entity* owner = &Entities()[(uint16_t)entity->word_0x2A_42];
	if (owner <= Entities())
		return;
	if (owner->life_0x0C_12 == 1)
	{
		MarkToFree_41E80_421C0(entity);
		return;
	}
	featurePosition_AE454_AE444 = owner->position_0x48_72;
	MoveEntity_41EC0_42200((axis_3d*)&featurePosition_AE454_AE444, (uint16_t)entity->word_0x1E_30, (uint16_t)entity->word_0x20_32, entity->word_0x7E_126);
	entity->word_0x1E_30 = (int16_t)(((uint16_t)entity->word_0x1E_30 + 0x2D) & 0x7FF);
	featurePosition_AE454_AE444.z = (int16_t)(GetTerrainAlt_11F50_11F50((axis_3d*)&featurePosition_AE454_AE444) + 0x100);
	CopyEntityPosition_41C70_41FB0(entity, &featurePosition_AE454_AE444);
}

void FlagLifeAction_26CE0_26CE0(Type_Mc1Entity* entity)
{
	entity->word_0x1A_26++;
	if (LifeEnded(entity))
		return;
	if (!(entity->flags_0x10_16 & 2))
		entity->flags_0x10_16 |= 2;
}

void RisingSplitAction_26D20_26D20(Type_Mc1Entity* entity)
{
	// rises to 400h over the ground, then splits into 2 of its kind (byte_0x44 / 45) to opposite random yaws
	bool moved = false;
	const int16_t ground = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&entity->position_0x48_72);
	if (entity->position_0x48_72.z < ground + 0x400)
	{
		entity->position_0x48_72.z = (int16_t)(entity->position_0x48_72.z + 0x40);
		moved = true;
	}
	if (entity->position_0x48_72.z > ground + 0x400)
	{
		entity->position_0x48_72.z = (int16_t)(ground + 0x400);
		moved = true;
	}
	if (moved || LifeEnded(entity))
		return;
	entity->word_0x20_32 = 0x38;
	NextRand(entity);
	entity->word_0x1E_30 = (int16_t)(entity->rand_0x04_4 & 0x7FF);
	Type_Mc1Entity* part = nullptr;
	for (int32_t i = 0; i < 2; i++)
	{
		entity->word_0x1E_30 = (int16_t)(((uint16_t)entity->word_0x1E_30 + 0x400) & 0x7FF);
		featurePosition_AE454_AE444 = entity->position_0x48_72;
		featurePosition_AE454_AE444.z = (int16_t)(featurePosition_AE454_AE444.z + entity->box_0x4E_78[0]);
		part = CreateEntity_373F0_377B0(&entity->position_0x48_72, (int8_t)entity->byte_0x44_68, (int8_t)entity->byte_0x45_69);
		if (!part)
			continue;
		part->id_0x18_24 = entity->id_0x18_24;
		part->life_0x0C_12 /= 3;
		part->word_0x1E_30 = entity->word_0x1E_30;
		part->word_0x20_32 = entity->word_0x20_32;
		part->byte_0x44_68 = 0x0A;
		part->byte_0x45_69 = 0x17;
		part->word_0x2C_44 = entity->word_0x2C_44;
	}
	//!!!!! the original queues the sound of the last created part, of a wrong index when it was not created
	if (part) // port: no sound of a not created part
		QueueEntitySound(part, 0x17);
}

void CreaturesFromManaAction_26E90_26E90(Type_Mc1Entity* entity)
{
	// up to 8 creatures of the model 9 (64 of the id at most) around it sharing 10000 mana, then gone
	entity->word_0x2C_44 = 0x2710;
	if ((uint16_t)entity->word_0x2C_44)
	{
		int16_t count = (int16_t)GetFreeEntityCount_37710_37AD0();
		if (count < 0)
			count = 0;
		if (count > 8)
			count = 8;
		uint16_t owned = 0;
		for (Type_Mc1Entity* other = mc1Settings_AE408_AE3F8->entityLists_0x8E1A_36378[10]; other > Entities(); other = other->next_0x00_0)
			if (other->owner_0x90_144 == (int32_t)(int16_t)entity->id_0x18_24)
				owned++;
		if (count < 0)
			count = 0;
		if (count > 0x40 - owned)
			count = (int16_t)(0x40 - owned);
		if (count > 0)
		{
			int16_t mana = entity->word_0x2C_44;
			const int16_t share = (int16_t)(mana / count);
			const int32_t step = 0x800 / count;
			int32_t yaw = 0;
			for (int16_t left = count; left > 0; left--, yaw += step)
			{
				featurePosition_AE454_AE444 = entity->position_0x48_72;
				MoveEntity_41EC0_42200((axis_3d*)&featurePosition_AE454_AE444, (uint16_t)yaw, 0, 0x200);
				Type_Mc1Entity* creature = CreateEntity_373F0_377B0(&featurePosition_AE454_AE444, 5, 9);
				if (!creature)
					continue;
				//!!!!! the original stores the remainder of the division, not the share
				creature->mana_0x8C_140 = mana % share;
				creature->owner_0x90_144 = entity->id_0x18_24;
				creature->word_0x22_34 = (int16_t)((yaw + 0x400) & 0x7FF);
				creature->word_0x1E_30 = creature->word_0x22_34;
				mana = (int16_t)(mana - share);
			}
		}
	}
	MarkToFree_41E80_421C0(entity);
}

void SlideAction_29540_29540(Type_Mc1Entity* entity)
{
	// slides down the slope by target_0x96 (at most 80h a step), the damage by word_0x2C
	TerrainGradient_41F50_41F50(&entity->position_0x48_72, &featurePosition_AE454_AE444);
	entity->target_0x96_150.x = (int16_t)(entity->target_0x96_150.x + featurePosition_AE454_AE444.x);
	entity->target_0x96_150.y = (int16_t)(entity->target_0x96_150.y + featurePosition_AE454_AE444.y);
	entity->target_0x96_150.x = Clamp16(entity->target_0x96_150.x, -0x80, 0x80);
	entity->target_0x96_150.y = Clamp16(entity->target_0x96_150.y, -0x80, 0x80);
	featurePosition_AE454_AE444 = entity->position_0x48_72;
	featurePosition_AE454_AE444.x = (int16_t)(featurePosition_AE454_AE444.x + entity->target_0x96_150.x);
	featurePosition_AE454_AE444.y = (int16_t)(featurePosition_AE454_AE444.y + entity->target_0x96_150.y);
	featurePosition_AE454_AE444.z = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&featurePosition_AE454_AE444);
	CopyEntityPosition_41C70_41FB0(entity, &featurePosition_AE454_AE444);
	SendMailAround_120B0_120B0(entity, 0, (uint16_t)entity->word_0x2C_44);
}

void EggAction_296A0_296A0(Type_Mc1Entity* entity)
{
	// on the ground; at the end of the countdown the action 39h and 5000 max life
	entity->position_0x48_72.z = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&entity->position_0x48_72);
	if (LifeEnded(entity))
		return;
	if (entity->word_0x1A_26-- == 0)
	{
		SetAction_424F0_42830(entity, 0x39);
		entity->maxLife_0x08_8 = 0x1388;
	}
}

void HatchAction_29700_29700(Type_Mc1Entity* entity)
{
	// a creature of the model 5 (counted) and a smoke of its id in its place
	entity->position_0x48_72.z = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&entity->position_0x48_72);
	featurePosition_AE454_AE444 = entity->position_0x48_72;
	if (Type_Mc1Entity* creature = CreateEntity_373F0_377B0(&featurePosition_AE454_AE444, 5, 5))
	{
		mc1State_AE400_AE3F0->creatureStats_0x38C9F_232607.creatures_0x00++;
		if (Type_Mc1Entity* smoke = CreateEntity_373F0_377B0(&featurePosition_AE454_AE444, 0x0A, 1))
			smoke->id_0x18_24 = creature->id_0x18_24;
	}
	MarkToFree_41E80_421C0(entity);
}

void FireWallAction_29780(Type_Mc1Entity* entity)
{
	// MC1: 15 rows of fires (model 6) in a 3x3 area, the first ones lasting 14 steps
	if (LifeEnded(entity))
		return;
	SetBox_37130_374F0(entity, 0x200, 0x800);
	SendMailAround_120B0_120B0(entity, 0, (uint16_t)((int32_t)(uint16_t)entity->word_0x2C_44 / entity->maxLife_0x08_8));
	const int32_t search = StartSearch_11410_11410(0, 1);
	if (search)
	{
		NextRand(entity);
		int32_t dx;
		int32_t dy;
		while (GetNextSearchOffset_114B0_114B0(search, &dx, &dy) == 1)
		{
			Type_Mc1Position position;
			NextRand(entity);
			position.x = (int16_t)(dx * 0x70 + entity->position_0x48_72.x - 0x60 + (int32_t)(entity->rand_0x04_4 % 0x81) - 0x40);
			NextRand(entity);
			position.y = (int16_t)(dy * 0x70 + entity->position_0x48_72.y - 0x60 + (int32_t)(entity->rand_0x04_4 % 0x81) - 0x40);
			position.z = entity->position_0x48_72.z;
			Type_Mc1Entity* fire = CreateEntity_373F0_377B0(&position, 0x0A, 6);
			if (!fire)
				continue;
			fire->id_0x18_24 = entity->id_0x18_24;
			fire->word_0x2C_44 = entity->word_0x2C_44;
			fire->life_0x0C_12 = entity->word_0x1A_26 ? 1 : 0x0E;
			fire->flags_0x10_16 |= 0x10080;
			fire->sprite_0x56_86 = (int16_t)(fire->sprite_0x56_86 + 7);
			fire->word_0x2E_46 = (int16_t)(entity->word_0x1A_26 * 256 / 2);
			fire->word_0x1A_26 = 7;
		}
		EndSearch_11490_11490(search);
	}
	if (++entity->word_0x1A_26 > 0x0E)
		MarkToFree_41E80_421C0(entity);
}

void FireWallActionHw_29780(Type_Mc1Entity* entity)
{
	// HW: the ring of fire of 25CE0 with 4 sizes
	FireRing(entity, 7);
}

void ShockwaveAction_299D0_29A10(Type_Mc1Entity* entity)
{
	// the countdown (the sound 2Bh), then within A00h: creatures and class 2 die, wizards get the damage,
	// a class 9 or 10 near ends it; the sound 2Ch, the flash 3 of the caster
	if (entity->word_0x1A_26)
	{
		entity->word_0x1A_26--;
		QueueEntitySound(entity, 0x2B);
		return;
	}
	if (LifeEnded(entity))
		return;
	for (Type_Mc1Entity* other = Entities(); other < Entities() + mc1EntityCount; other++)
	{
		if (!other->class_0x40_64 || other->id_0x18_24 == entity->id_0x18_24)
			continue;
		switch (other->class_0x40_64)
		{
		case 2:
		case 5:
			if (Distance2D_423D0_42710(&entity->position_0x48_72, &other->position_0x48_72) < 0xA00)
				other->life_0x0C_12 = -1;
			break;
		case 3:
			if (Distance2D_423D0_42710(&entity->position_0x48_72, &other->position_0x48_72) < 0xA00)
				SendMail_12B50_12B50(entity, other, 0, (uint16_t)entity->word_0x2C_44);
			break;
		case 9:
		case 0x0A:
			if (Distance2D_423D0_42710(&entity->position_0x48_72, &other->position_0x48_72) < 0xA00)
				entity->life_0x0C_12 = 0;
			break;
		default:
			break;
		}
	}
	QueueEntitySound(entity, 0x2C);
	QueueSound_55370_558A0((int16_t)entity->id_0x18_24, -1, 0x2C);
	SetFlashOfPlayer_44BE0_44F20(&Entities()[(int16_t)entity->id_0x18_24], 3);
	MarkToFree_41E80_421C0(entity);
}
