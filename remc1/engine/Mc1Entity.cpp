#include "Mc1Entity.h"

#include <cstdlib>
#include <cstring>

#include "Mc1EntityTables.h"
#include "Mc1Files.h"
#include "Mc1Game.h"
#include "Mc1Shared.h"
#include "Mc1TextureMaps.h"

Type_Mc1Position featurePosition_AE454_AE444;
uint8_t playerData_B7330_B7320[0xF0 * 4];

// 98F38: 30 records of 32 bytes and an empty one, the same in MC1 and HW
uint8_t creatureTypes_98F38_98F38[31][32] = {
	{ 0x00, 0x00, 0x38, 0x00, 0x05, 0x00, 0x16, 0x00, 0x05, 0x00, 0x00, 0x07, 0x00, 0x00, 0xFC, 0xFF, 0x00, 0x01, 0x00, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x0C, 0x32, 0x00, 0x00, 0x10, 0x00, 0x02 },
	{ 0x01, 0x00, 0x16, 0x00, 0x05, 0x00, 0x16, 0x00, 0x05, 0x00, 0x00, 0x07, 0x00, 0x00, 0xFC, 0xFF, 0x00, 0x01, 0x00, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x09, 0x28, 0x00, 0x00, 0x10, 0x00, 0x02 },
	{ 0x02, 0x00, 0x71, 0x00, 0x05, 0x00, 0x71, 0x00, 0x05, 0x00, 0x00, 0x07, 0x00, 0x00, 0xFC, 0xFF, 0x00, 0x01, 0x00, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x09, 0x28, 0x00, 0x00, 0x10, 0x00, 0x02 },
	{ 0x03, 0x00, 0x11, 0x00, 0x05, 0x00, 0x16, 0x00, 0x05, 0x00, 0x00, 0x07, 0x00, 0x00, 0xFC, 0xFF, 0x00, 0x01, 0x00, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x09, 0x28, 0x00, 0x00, 0x10, 0x00, 0x02 },
	{ 0x04, 0x00, 0x0B, 0x00, 0x05, 0x00, 0x16, 0x00, 0x05, 0x00, 0x00, 0x07, 0x00, 0x00, 0xFC, 0xFF, 0x00, 0x01, 0x00, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x09, 0x28, 0x00, 0x00, 0x10, 0x00, 0x02 },
	{ 0x05, 0x00, 0x05, 0x00, 0x05, 0x00, 0x16, 0x00, 0x05, 0x00, 0x00, 0x07, 0x00, 0x00, 0xFC, 0xFF, 0x00, 0x01, 0x00, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x09, 0x28, 0x00, 0x00, 0x10, 0x00, 0x02 },
	{ 0x06, 0x00, 0x00, 0x00, 0x05, 0x00, 0x16, 0x00, 0x05, 0x00, 0x00, 0x07, 0x00, 0x00, 0xFC, 0xFF, 0x00, 0x01, 0x00, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x09, 0x28, 0x00, 0x00, 0x10, 0x00, 0x02 },
	{ 0x07, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04, 0x80, 0x00, 0xFC, 0xFF, 0x00, 0x01, 0x00, 0x02, 0xFF, 0xFE, 0xFF, 0xFF, 0x00, 0x18, 0x28, 0x00, 0x00, 0x20, 0x00, 0x02 },
	{ 0x08, 0x00, 0x00, 0x01, 0x05, 0x00, 0x00, 0x01, 0x05, 0x00, 0x00, 0x03, 0x80, 0x00, 0xFC, 0xFF, 0x00, 0x01, 0x00, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x18, 0x28, 0x00, 0x00, 0x20, 0x00, 0x02 },
	{ 0x09, 0x00, 0x38, 0x00, 0x05, 0x00, 0x16, 0x00, 0x05, 0x00, 0x00, 0x06, 0x00, 0x02, 0xF0, 0xFF, 0x00, 0x01, 0x00, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x0C, 0x32, 0x00, 0x00, 0x10, 0x00, 0x02 },
	{ 0x0A, 0x00, 0x16, 0x00, 0x05, 0x00, 0x16, 0x00, 0x05, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xFF, 0x0F, 0x00, 0x00, 0x02, 0xFE, 0x80, 0xF0, 0xFF, 0x00, 0x09, 0x28, 0x00, 0x00, 0x0C, 0x00, 0x02 },
	{ 0x0B, 0x00, 0x16, 0x00, 0x05, 0x00, 0x16, 0x00, 0x05, 0x00, 0x00, 0x07, 0x00, 0x02, 0xFC, 0xFF, 0x46, 0x00, 0x55, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x0F, 0x28, 0x00, 0x00, 0x0F, 0x00, 0x02 },
	{ 0x0C, 0x00, 0x22, 0x00, 0x05, 0x00, 0x55, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0x1E, 0x00, 0x55, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x14, 0x1E, 0x00, 0x00, 0x14, 0xAA, 0x00 },
	{ 0x0D, 0x00, 0x16, 0x00, 0x05, 0x00, 0x16, 0x00, 0x05, 0x00, 0x00, 0x05, 0x00, 0x02, 0xFC, 0xFF, 0x46, 0x00, 0x00, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x09, 0x1E, 0x00, 0x00, 0x0C, 0x00, 0x02 },
	{ 0x0E, 0x00, 0x71, 0x00, 0x11, 0x00, 0x71, 0x00, 0x11, 0x00, 0x00, 0x07, 0x33, 0x00, 0xE0, 0xFF, 0x14, 0x00, 0x00, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x06, 0x1E, 0x00, 0x00, 0x14, 0xAA, 0x02 },
	{ 0x0F, 0x00, 0x22, 0x00, 0x05, 0x00, 0x55, 0x00, 0x05, 0x00, 0x00, 0x01, 0x00, 0x00, 0xC0, 0xFF, 0x14, 0x00, 0x55, 0x01, 0xFE, 0x80, 0xF0, 0xFF, 0x00, 0x14, 0x1E, 0x00, 0x00, 0x14, 0xAA, 0x00 },
	{ 0x10, 0x00, 0x16, 0x00, 0x05, 0x00, 0x71, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x80, 0xFF, 0x14, 0x00, 0x00, 0x02, 0xFE, 0x80, 0xF0, 0xFF, 0x00, 0x0A, 0x1E, 0x00, 0x00, 0x14, 0x00, 0x02 },
	{ 0x11, 0x00, 0x38, 0x00, 0x05, 0x00, 0x16, 0x00, 0x05, 0x00, 0x00, 0x01, 0x00, 0x00, 0xC0, 0xFF, 0x14, 0x00, 0x00, 0x02, 0xFE, 0x80, 0xF0, 0xFF, 0x00, 0x09, 0x1E, 0x00, 0x00, 0x14, 0x00, 0x02 },
	{ 0x12, 0x00, 0x16, 0x00, 0x05, 0x00, 0x16, 0x00, 0x05, 0x00, 0x00, 0x01, 0x00, 0x00, 0xE0, 0xFF, 0x14, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x09, 0x23, 0x00, 0x00, 0x14, 0xAA, 0x02 },
	{ 0x13, 0x00, 0x16, 0x00, 0x05, 0x00, 0x16, 0x00, 0x05, 0x00, 0x00, 0x01, 0x00, 0x00, 0x80, 0xFF, 0x14, 0x00, 0x00, 0x02, 0xFE, 0x80, 0xF0, 0xFF, 0x00, 0x09, 0x28, 0x00, 0x00, 0x0F, 0x00, 0x02 },
	{ 0x14, 0x00, 0x38, 0x00, 0x05, 0x00, 0x16, 0x00, 0x05, 0x00, 0x00, 0x05, 0x00, 0x00, 0xE7, 0xFF, 0x50, 0x00, 0x00, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x09, 0x28, 0x00, 0x00, 0x19, 0x00, 0x02 },
	{ 0x15, 0x00, 0x16, 0x00, 0x05, 0x00, 0x16, 0x00, 0x05, 0x00, 0x00, 0x01, 0x00, 0x00, 0x80, 0xFF, 0x14, 0x00, 0x00, 0x02, 0xFE, 0x80, 0xF0, 0xFF, 0x00, 0x09, 0x19, 0x00, 0x00, 0x08, 0x00, 0x02 },
	{ 0x16, 0x00, 0x16, 0x00, 0x05, 0x00, 0x16, 0x00, 0x05, 0x00, 0x00, 0x01, 0x00, 0x00, 0x80, 0xFF, 0x14, 0x00, 0x00, 0x02, 0xFE, 0x80, 0xF0, 0xFF, 0x00, 0x09, 0x28, 0x00, 0x00, 0x0C, 0x00, 0x02 },
	{ 0x17, 0x00, 0x2D, 0x00, 0x05, 0x00, 0x16, 0x00, 0x05, 0x00, 0x00, 0x07, 0x00, 0x00, 0xFC, 0xFF, 0x3C, 0x00, 0x00, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x09, 0x0A, 0x00, 0x00, 0x1E, 0xC7, 0x07 },
	{ 0x18, 0x00, 0x16, 0x00, 0x05, 0x00, 0x16, 0x00, 0x05, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xFF, 0x14, 0x00, 0x00, 0x02, 0x00, 0x00, 0x02, 0x00, 0x00, 0x09, 0x0F, 0x00, 0x00, 0x0F, 0x00, 0x02 },
	{ 0x19, 0x00, 0x44, 0x00, 0x05, 0x00, 0x16, 0x00, 0x05, 0x00, 0x00, 0x07, 0x00, 0x01, 0xF8, 0xFF, 0x46, 0x00, 0x00, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x09, 0x28, 0x00, 0x00, 0x12, 0x00, 0x02 },
	{ 0x1A, 0x00, 0x16, 0x00, 0x05, 0x00, 0x16, 0x00, 0x05, 0x00, 0x00, 0x01, 0x00, 0x00, 0x80, 0xFF, 0x14, 0x00, 0x00, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x09, 0x28, 0x00, 0x00, 0x0C, 0x00, 0x02 },
	{ 0x1B, 0x00, 0x16, 0x00, 0x05, 0x00, 0x16, 0x00, 0x05, 0x00, 0x00, 0x01, 0x00, 0x00, 0x80, 0xFF, 0x14, 0x00, 0x00, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x09, 0x28, 0x00, 0x00, 0x0C, 0x00, 0x02 },
	{ 0x1C, 0x00, 0x16, 0x00, 0x05, 0x00, 0x16, 0x00, 0x05, 0x00, 0x00, 0x01, 0x00, 0x00, 0x80, 0xFF, 0x14, 0x00, 0x00, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x09, 0x28, 0x00, 0x00, 0x0C, 0x00, 0x02 },
	{ 0x1D, 0x00, 0x16, 0x00, 0x05, 0x00, 0x16, 0x00, 0x05, 0x00, 0x00, 0x01, 0x00, 0x00, 0x80, 0xFF, 0x14, 0x00, 0x00, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x09, 0x28, 0x00, 0x00, 0x0C, 0x00, 0x02 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
};

// 90AD8
const uint8_t frameCounts_90AD8_90AD8[42] = {
	1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 1, 1, 1, 1,
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 0, 0, 0, 0, 0,
};

static Type_Mc1Entity* Entities()
{
	return mc1State_AE400_AE3F0->entities_0x7463_29795;
}

static uint16_t EntityIndex(Type_Mc1Entity* entity)
{
	return (uint16_t)(entity - Entities());
}

static uint16_t MapCell(const Type_Mc1Position* position)
{
	// the high bytes of x and y
	return (uint16_t)(((position->y >> 8) & 0xFF) << 8 | ((position->x >> 8) & 0xFF));
}

void InitEntities_371C0_37580()
{
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	state->entities_0x7463_29795[0].playerData_0xA0_160 = playerData_B7330_B7320;
	state->freeTop_0x28_40 = -1;
	state->reuseTop_0x11F1_4593 = -1;
	// entity 0 is never used, 1 is taken first
	for (int i = 999; i > 0; i--)
		state->freeEntities_0x251_593[++state->freeTop_0x28_40] = &state->entities_0x7463_29795[i];
}

void RebuildFreeLists_37220_375E0()
{
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	state->freeTop_0x28_40 = -1;
	state->reuseTop_0x11F1_4593 = -1;
	for (int i = 999; i > 0; i--)
	{
		Type_Mc1Entity* entity = &state->entities_0x7463_29795[i];
		if (!entity->class_0x40_64)
			state->freeEntities_0x251_593[++state->freeTop_0x28_40] = entity;
		else if (entity->flags_0x10_16 & 0x20400)
			state->reuseEntities_0x11F5_4597[++state->reuseTop_0x11F1_4593] = entity;
	}
}

Type_Mc1Entity* AllocEntity_372C0_37680()
{
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	Type_Mc1Entity* entity;
	if (state->freeTop_0x28_40 >= 0)
		entity = state->freeEntities_0x251_593[state->freeTop_0x28_40--];
	else
	{
		if (state->reuseTop_0x11F1_4593 < 0)
			return nullptr;
		// a reusable entity is taken, the lists of the kinds are cleared
		Type_Mc1Settings_AE408_AE3F8* settings = mc1Settings_AE408_AE3F8;
		memset(&settings->entityLists_0x8E1A_36378[1], 0, 20 * sizeof(Type_Mc1Entity*));
		settings->entityLists_0x8E1A_36378[22] = nullptr;
		settings->entityLists_0x8E1A_36378[23] = nullptr;
		settings->entityLists_0x8E1A_36378[21] = nullptr;
		settings->entityLists_0x8E1A_36378[24] = nullptr;
		entity = state->reuseEntities_0x11F5_4597[state->reuseTop_0x11F1_4593];
		RemoveFromMap_41DD0_42110(entity);
		entity->class_0x40_64 = 0;
		state->reuseTop_0x11F1_4593--;
	}
	memset(entity, 0, sizeof(Type_Mc1Entity));
	uint16_t index = EntityIndex(entity);
	entity->maxLife_0x08_8 = 300;
	entity->flags_0x10_16 = 8;
	entity->word_0x7E_126 = 0x10;
	entity->word_0x2C_44 = 100;
	entity->id_0x18_24 = index;
	entity->byte_0x42_66 = 0xFF;
	entity->byte_0x43_67 = 0xFF;
	entity->typeParams_0x9C_156 = creatureTypes_98F38_98F38[0];
	entity->playerData_0xA0_160 = playerData_B7330_B7320;
	entity->byte_0x44_68 = 10;
	entity->byte_0x3A_58 = 0xFA;
	entity->rand_0x04_4 = index + state->rand_0x04_4;
	entity->index_0x3F_63 = (uint8_t)index;
	return entity;
}

void SetSprite_36FA0_37360(Type_Mc1Entity* entity, int16_t sprite)
{
	const Type_Mc1SpriteDef_99BA0& def = spriteDefs_99BA0_99BA0[sprite];
	entity->frame_0x58_88 = 0;
	entity->sprite_0x56_86 = sprite;
	entity->frameCount_0x59_89 = frameCounts_90AD8_90AD8[def.kind_0x0C_12];
	entity->box_0x4E_78[0] = (int16_t)(def.height_0x08_8 / 2);
	entity->box_0x4E_78[1] = (int16_t)(def.width_0x06_6 / 2);
	entity->box_0x4E_78[2] = (int16_t)(def.width_0x06_6 / 2);
	entity->box_0x4E_78[3] = (int16_t)(def.height_0x08_8 / 2);
}

void SetSpriteBox_37030_373F0(Type_Mc1Entity* entity, int16_t sprite)
{
	SetSprite_36FA0_37360(entity, sprite);
	const Type_Mc1SpriteDef_99BA0& def = spriteDefs_99BA0_99BA0[sprite];
	entity->box_0x4E_78[1] = (int16_t)(def.width_0x06_6 / 2);
	entity->box_0x4E_78[2] = (int16_t)(def.width_0x06_6 / 2);
	entity->box_0x4E_78[3] = (int16_t)(def.height_0x08_8 / 2);
}

void SetSpriteHalfBox_370E0_374A0(Type_Mc1Entity* entity, int16_t sprite)
{
	SetSprite_36FA0_37360(entity, sprite);
	entity->box_0x4E_78[1] /= 2;
	entity->box_0x4E_78[2] /= 2;
	entity->box_0x4E_78[3] /= 2;
}

void ResetLife_36DE0_371A0(Type_Mc1Entity* entity)
{
	entity->life_0x0C_12 = entity->maxLife_0x08_8;
}

void SetBox_37130_374F0(Type_Mc1Entity* entity, int16_t width, int16_t height)
{
	entity->box_0x4E_78[1] = width;
	entity->box_0x4E_78[2] = width;
	entity->box_0x4E_78[3] = height;
}

int32_t WrapDelta_358D0_35C90(int16_t from, int16_t to, int16_t range)
{
	// the shorter way on the wrapped map
	int16_t delta = (int16_t)(to - from);
	int16_t half = range >> 1;
	if (delta > half)
		return (int16_t)(delta - range);
	if (delta < -half)
		return (int16_t)(delta + range);
	return delta;
}

void SetFeaturePosition_35900_35CC0(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2)
{
	// the higher of the two tiles
	uint8_t height = mapHeightmap_DC1E0_DC1D0[(uint16_t)((x & 0xFF) | (y & 0xFF) << 8)];
	uint8_t height2 = mapHeightmap_DC1E0_DC1D0[(uint16_t)((x2 & 0xFF) | (y2 & 0xFF) << 8)];
	if (height2 > height)
		height = height2;
	featurePosition_AE454_AE444.x = (int16_t)(x << 8);
	featurePosition_AE454_AE444.z = (int16_t)(height << 5);
	featurePosition_AE454_AE444.y = (int16_t)(y << 8);
}

void InitBuildingEntity_36DF0_371B0(Type_Mc1Entity* entity, uint16_t sprite)
{
	// *Build tab: the sizes are doubled in MCGA by the relocation (buildIndex), halved again here
	uint16_t height = buildIndex[sprite].height_5;
	uint16_t width = buildIndex[sprite].width_4;
	entity->word_0x1A_26 = 2;
	entity->word_0x80_128 = (int16_t)((height * width) >> 4);
	if (*videoMode_12F02E_12F01E == 1)
	{
		height >>= 1;
		width >>= 1;
	}
	// to the corner of the tile
	featurePosition_AE454_AE444 = entity->position_0x48_72;
	featurePosition_AE454_AE444.x = (int16_t)((entity->position_0x48_72.x >> 8) << 8);
	featurePosition_AE454_AE444.y = (int16_t)((entity->position_0x48_72.y >> 8) << 8);
	CopyEntityPosition_41C70_41FB0(entity, &featurePosition_AE454_AE444);
	uint8_t x = (uint8_t)((entity->position_0x48_72.x >> 8) - (width >> 1));
	uint8_t y = (uint8_t)((entity->position_0x48_72.y >> 8) - (height >> 1));
	if ((x + y) % 2)
	{
		// one tile right on the odd squares
		featurePosition_AE454_AE444 = entity->position_0x48_72;
		featurePosition_AE454_AE444.x = (int16_t)(featurePosition_AE454_AE444.x + 0x100);
		CopyEntityPosition_41C70_41FB0(entity, &featurePosition_AE454_AE444);
		x++;
	}
	SetBuildingBox_37150_37510(entity, (int16_t)sprite);
	int16_t z = GetTerrainHeightFromSquare_361C0_36580((char)x, (char)y, (char)height, (char)width);
	entity->life_0x0C_12 = 0x1E;
	entity->word_0x2C_44 = 2000;
	entity->position_0x48_72.z = (int16_t)(z << 5);
	entity->mailMask_0x1C_28 |= 2;
	entity->byte_0x47_71 = (uint8_t)sprite;
}

void SetBuildingBox_37150_37510(Type_Mc1Entity* entity, int16_t sprite)
{
	uint32_t height = buildIndex[sprite].height_5;
	uint32_t width = buildIndex[sprite].width_4;
	if (*videoMode_12F02E_12F01E == 1)
	{
		height >>= 1;
		width >>= 1;
	}
	entity->box_0x4E_78[1] = (int16_t)(((width << 8) + 0x500) >> 1);
	entity->box_0x4E_78[0] = (int16_t)0xE000;
	entity->box_0x4E_78[3] = 0x4000;
	entity->box_0x4E_78[2] = (int16_t)(((height << 8) + 0x500) >> 1);
}

Type_Mc1Entity* CreateEntity_373F0_377B0(Type_Mc1Position* position, int32_t entityClass, int32_t model)
{
	const Type_Mc1ClassModel& record = classes_96902_96902[entityClass].models_4[model];
	if (!record.flag_10 || (int16_t)record.index_4 != model)
		return nullptr;
	return record.create_6(position);
}

int32_t CopyEntityPosition_41C70_41FB0(Type_Mc1Entity* entity, Type_Mc1Position* position)
{
	if (MapCell(&entity->position_0x48_72) != MapCell(position))
	{
		RemoveFromMap_41DD0_42110(entity);
		AddToMap_41CF0_42030(entity, position);
		return 1;
	}
	entity->position_0x48_72 = *position;
	return 0;
}

void AddToMap_41CF0_42030(Type_Mc1Entity* entity, Type_Mc1Position* position)
{
	if (entity->flags_0x10_16 & 4)
		return;
	// the first entity of the cell
	entity->mapPrevious_0x16_22 = 0;
	uint16_t cell = MapCell(position);
	uint16_t next = (uint16_t)mapEntityIndex_10C1E0_10C1D0[cell];
	entity->mapNext_0x14_20 = next;
	if (next)
		Entities()[next].mapPrevious_0x16_22 = EntityIndex(entity);
	mapEntityIndex_10C1E0_10C1D0[cell] = (int16_t)EntityIndex(entity);
	entity->position_0x48_72 = *position;
	entity->flags_0x10_16 |= 4;
}

void RemoveFromMap_41DD0_42110(Type_Mc1Entity* entity)
{
	if (!(entity->flags_0x10_16 & 4))
		return;
	if (entity->mapPrevious_0x16_22)
		Entities()[entity->mapPrevious_0x16_22].mapNext_0x14_20 = entity->mapNext_0x14_20;
	else
		mapEntityIndex_10C1E0_10C1D0[MapCell(&entity->position_0x48_72)] = (int16_t)entity->mapNext_0x14_20;
	if (entity->mapNext_0x14_20)
		Entities()[entity->mapNext_0x14_20].mapPrevious_0x16_22 = entity->mapPrevious_0x16_22;
	entity->flags_0x10_16 &= ~4u;
}

void MarkToFree_41E80_421C0(Type_Mc1Entity* entity)
{
	entity->flags_0x10_16 |= 0x400;
}

void FreeEntity_41E90_421D0(Type_Mc1Entity* entity)
{
	RemoveFromMap_41DD0_42110(entity);
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	entity->class_0x40_64 = 0;
	state->freeEntities_0x251_593[++state->freeTop_0x28_40] = entity;
}

uint32_t Distance2D_423D0_42710(Type_Mc1Position* from, Type_Mc1Position* to)
{
	int32_t dy = (int16_t)(to->y - from->y);
	int32_t dx = (int16_t)(to->x - from->x);
	return IntegerSqrt_410CE_4140E(dy * dy + dx * dx);
}

void CreateFeatures_36430_367F0(Type_Mc1Level_2F503* level)
{
	// feature 0 is not used
	for (int i = 1; i < 2000; i++)
	{
		Type_Mc1LevelFeature* feature = &level->features_0x430_1072[i];
		if (feature->word_0x08_8 == 0xFFFF && feature->type_0x00_0 == 10)
		{
			CreateFeature_36480_36840(level, feature);
			feature->type_0x00_0 = 0;
		}
	}
	InitCreatedEntities_36620_369E0();
}

void CreateFeature_36480_36840(Type_Mc1Level_2F503* level, Type_Mc1LevelFeature* feature)
{
	uint16_t model = feature->model_0x02_2;
	bool line = model == 0x1C || model == 0x1D || model == 0x1F || model == 0x32;
	if (line && feature->word_0x0C_12)
	{
		CreateFeatureChain_362C0_36680(level, feature);
		return;
	}
	featurePosition_AE454_AE444.x = (int16_t)(feature->x_0x04_4 << 8);
	featurePosition_AE454_AE444.y = (int16_t)(feature->y_0x06_6 << 8);
	featurePosition_AE454_AE444.z = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&featurePosition_AE454_AE444);
	// 97D12: the models of class 10 by the model number (the flag is not checked here)
	Type_Mc1Entity* (*create)(Type_Mc1Position*) = classes_96902_96902[10].models_4[model].create_6;
	if (!create)
		return;
	Type_Mc1Entity* entity = create(&featurePosition_AE454_AE444);
	// 2Dh: a building, the sprite of the *Build tab
	if (model == 0x2D && entity)
		InitBuildingEntity_36DF0_371B0(entity, (uint16_t)(feature->next_0x0E_14 + 0x10));
}

void CreateFeatureChain_362C0_36680(Type_Mc1Level_2F503* level, Type_Mc1LevelFeature* feature)
{
	uint16_t type = feature->type_0x00_0;
	uint16_t model = feature->model_0x02_2;
	void (*line)(int32_t, int32_t, int32_t, int32_t) = nullptr;
	if (type == 10)
	{
		switch (model)
		{
		case 0x1C:
			line = CreateFeatureLine1C_35960_35D20;
			break;
		case 0x1D:
			line = CreateFeatureLine1D_35BF0_35FB0;
			break;
		case 0x1F:
			line = CreateFeatureLine1F_35D30_360F0;
			break;
		case 0x32:
			line = CreateFeatureLine32_35DE0_361A0;
			break;
		}
	}
	if (!line)
		return;
	// back to the first feature of the chain (+0Eh), then along +10h
	Type_Mc1LevelFeature* current = feature;
	while (current->next_0x0E_14)
		current = &level->features_0x430_1072[current->next_0x0E_14];
	while (current && current->type_0x00_0 == type && current->model_0x02_2 == model)
	{
		uint16_t link = current->link_0x10_16;
		current->word_0x0C_12 = 0;
		if (!link)
			break;
		Type_Mc1LevelFeature* next = &level->features_0x430_1072[link];
		line(current->x_0x04_4, current->y_0x06_6, next->x_0x04_4, next->y_0x06_6);
		current = next;
	}
}

void InitCreatedEntities_36620_369E0()
{
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	state->rand_0x04_4 = state->rand_0x04_4 * 0x24A1 + 0x24DF;
	bool again;
	do
	{
		again = false;
		for (int i = 1; i < 1000; i++)
		{
			Type_Mc1Entity* entity = &state->entities_0x7463_29795[i];
			if (!entity->class_0x40_64)
				continue;
			// class 10: the models 9-0Bh, 1Bh-20h, 32h-33h and 2Dh with the action 33h run their action
			bool action = false;
			bool keep = false;
			if (entity->class_0x40_64 == 10)
			{
				uint8_t model = entity->model_0x41_65;
				if ((model >= 9 && model <= 0x0B) || (model >= 0x1B && model <= 0x20) || model == 0x32 || model == 0x33)
					action = true;
				else if (model == 0x2D)
				{
					action = entity->actionIndex_0x46_70 == 0x33;
					keep = !action;
				}
			}
			if (action)
			{
				const Type_Mc1ClassAction& record = classes_96902_96902[entity->class_0x40_64].actions_0[entity->actionIndex_0x46_70];
				again = true;
				if (record.action_6 && record.flag_10)
					record.action_6(entity);
			}
			else if (!keep)
				MarkToFree_41E80_421C0(entity);
			if (entity->flags_0x10_16 & 0x400)
				FreeEntity_41E90_421D0(entity);
		}
	} while (again);
}

void CreateFeatureLine1C_35960_35D20(int32_t x, int32_t y, int32_t x2, int32_t y2)
{
	int32_t dx = WrapDelta_358D0_35C90((int16_t)x, (int16_t)x2, 0x100);
	int32_t dy = WrapDelta_358D0_35C90((int16_t)y, (int16_t)y2, 0x100);
	if (!dx && !dy)
		return;
	if (dx < 0)
	{
		// always from the left
		dx = -dx;
		dy = -dy;
		int32_t t = x;
		x = x2;
		x2 = t;
		t = y;
		y = y2;
		y2 = t;
	}
	int32_t steps;
	int32_t stepX, restX, stepY, restY;
	int32_t ay = dy < 0 ? -dy : dy;
	if (ay < dx)
	{
		steps = dx / 10 + 1;
		stepX = dx / steps;
		restX = dx - stepX * steps;
		stepY = dy / steps;
		restY = dy - stepY * steps;
		for (; steps; steps--)
		{
			SetFeaturePosition_35900_35CC0((uint16_t)x, (uint16_t)y, (uint16_t)x2, (uint16_t)y2);
			Type_Mc1Entity* entity = CreateEntity_373F0_377B0(&featurePosition_AE454_AE444, 10, 0x1B);
			entity->word_0x1A_26 = (int16_t)(stepX + restX);
			x += stepX + restX;
			entity->actionIndex_0x46_70 = 0x1D;
			SetFeaturePosition_35900_35CC0((uint16_t)x, (uint16_t)y, (uint16_t)x2, (uint16_t)y2);
			entity = CreateEntity_373F0_377B0(&featurePosition_AE454_AE444, 10, 0x1B);
			if (stepY < 0)
			{
				entity->actionIndex_0x46_70 = 0x1B;
				entity->word_0x1A_26 = (int16_t)(-stepY - restY);
			}
			else
			{
				entity->actionIndex_0x46_70 = 0x1C;
				entity->word_0x1A_26 = (int16_t)(stepY + restY);
			}
			y += restY + stepY;
			restX = 0;
			restY = 0;
		}
	}
	else
	{
		steps = dy / 10;
		steps = (steps < 0 ? -steps : steps) + 1;
		stepY = dy / steps;
		restY = dy - stepY * steps;
		stepX = dx / steps;
		restX = dx - stepX * steps;
		for (; steps; steps--)
		{
			SetFeaturePosition_35900_35CC0((uint16_t)x, (uint16_t)y, (uint16_t)x2, (uint16_t)y2);
			Type_Mc1Entity* entity = CreateEntity_373F0_377B0(&featurePosition_AE454_AE444, 10, 0x1B);
			if (stepY < 0)
			{
				entity->actionIndex_0x46_70 = 0x1B;
				entity->word_0x1A_26 = (int16_t)(-stepY - restY);
			}
			else
			{
				entity->actionIndex_0x46_70 = 0x1C;
				entity->word_0x1A_26 = (int16_t)(stepY + restY);
			}
			y += restY + stepY;
			SetFeaturePosition_35900_35CC0((uint16_t)x, (uint16_t)y, (uint16_t)x2, (uint16_t)y2);
			entity = CreateEntity_373F0_377B0(&featurePosition_AE454_AE444, 10, 0x1B);
			restY = 0;
			entity->actionIndex_0x46_70 = 0x1D;
			int32_t moveX = stepX + restX;
			restX = 0;
			x += moveX;
			entity->word_0x1A_26 = (int16_t)moveX;
		}
	}
}

void CreateFeatureLine1D_35BF0_35FB0(int32_t x, int32_t y, int32_t x2, int32_t y2)
{
	int32_t dx = WrapDelta_358D0_35C90((int16_t)x, (int16_t)x2, 0x100);
	int32_t dy = WrapDelta_358D0_35C90((int16_t)y, (int16_t)y2, 0x100);
	int32_t signX = dx ? (dx > 0 ? 1 : -1) : 0;
	int32_t signY = dy ? (dy > 0 ? 1 : -1) : 0;
	int32_t ax = dx < 0 ? -dx : dx;
	int32_t ay = dy < 0 ? -dy : dy;
	int32_t diagonal = ax;
	int32_t straight = ay - ax < 0 ? ax - ay : ay - ax;
	int32_t straightX, straightY;
	if (ax > ay)
	{
		diagonal = ay;
		straightX = signX;
		straightY = 0;
	}
	else
	{
		straightY = signY;
		straightX = 0;
	}
	// the diagonal part, then the straight part (z stays from the last feature)
	featurePosition_AE454_AE444.x = (int16_t)(x << 8);
	featurePosition_AE454_AE444.y = (int16_t)(y << 8);
	Type_Mc1Entity* entity = CreateEntity_373F0_377B0(&featurePosition_AE454_AE444, 10, 0x1E);
	if (entity)
	{
		entity->word_0x1E_30 = (int16_t)signX;
		entity->word_0x20_32 = (int16_t)signY;
		entity->word_0x1A_26 = (int16_t)diagonal;
	}
	x += signX * diagonal;
	y += signY * diagonal;
	featurePosition_AE454_AE444.x = (int16_t)(x << 8);
	featurePosition_AE454_AE444.y = (int16_t)(y << 8);
	entity = CreateEntity_373F0_377B0(&featurePosition_AE454_AE444, 10, 0x1E);
	if (entity)
	{
		entity->word_0x1E_30 = (int16_t)straightX;
		entity->word_0x1A_26 = (int16_t)straight;
		entity->word_0x20_32 = (int16_t)straightY;
	}
}

void CreateFeatureLine1F_35D30_360F0(int32_t x, int32_t y, int32_t x2, int32_t y2)
{
	Type_Mc1Position from;
	from.x = (int16_t)(x << 8);
	from.y = (int16_t)(y << 8);
	from.z = (int16_t)(mapHeightmap_DC1E0_DC1D0[(uint16_t)x + ((uint32_t)(uint16_t)y << 8)] << 5);
	Type_Mc1Position to;
	to.x = (int16_t)(x2 << 8);
	to.y = (int16_t)(y2 << 8);
	to.z = 0; // not read
	int16_t yaw = GetYawAngle_42150_42490((axis_3d*)&from, (axis_3d*)&to);
	uint32_t distance = Distance2D_423D0_42710(&from, &to);
	Type_Mc1Entity* entity = CreateEntity_373F0_377B0(&from, 10, 0x20);
	if (entity)
	{
		entity->word_0x1E_30 = yaw;
		entity->life_0x0C_12 = (int32_t)((distance & 0xFFFF) >> 8);
	}
}

void CreateFeatureLine32_35DE0_361A0(int32_t x, int32_t y, int32_t x2, int32_t y2)
{
	Type_Mc1Position from;
	from.x = (int16_t)(x << 8);
	from.y = (int16_t)(y << 8);
	from.z = (int16_t)(mapHeightmap_DC1E0_DC1D0[(uint16_t)x + ((uint32_t)(uint16_t)y << 8)] << 4);
	Type_Mc1Position to;
	to.x = (int16_t)(x2 << 8);
	to.y = (int16_t)(y2 << 8);
	to.z = 0; // not read
	int16_t yaw = GetYawAngle_42150_42490((axis_3d*)&from, (axis_3d*)&to);
	uint32_t distance = Distance2D_423D0_42710(&from, &to);
	Type_Mc1Entity* entity = CreateEntity_373F0_377B0(&from, 10, 0x33);
	if (entity)
	{
		entity->word_0x1E_30 = yaw;
		entity->life_0x0C_12 = (int32_t)(distance & 0xFFFF) / entity->word_0x7E_126;
	}
}

int32_t BoundsOverlap_118C0_118C0(const Type_Mc1Position* position, const int16_t* box, const Type_Mc1Position* position2, const int16_t* box2)
{
	// box: z offset, half x, half y, half z
	int32_t dx = (int16_t)(position2->x - position->x);
	if (abs(dx) >= box[1] + box2[1])
		return 0;
	int32_t dy = (int16_t)(position2->y - position->y);
	if (abs(dy) >= box[2] + box2[2])
		return 0;
	int32_t dz = (position->z + box[0]) - (position2->z + box2[0]);
	if (abs(dz) >= box[3] + box2[3])
		return 0;
	return 1;
}

int32_t EntitiesOverlap_11950_11950(Type_Mc1Entity* entity, Type_Mc1Entity* entity2)
{
	return BoundsOverlap_118C0_118C0(&entity->position_0x48_72, entity->box_0x4E_78, &entity2->position_0x48_72, entity2->box_0x4E_78);
}

static void AddMail(Type_Mc1Entity* entity, uint8_t kind, uint16_t value, uint16_t sender)
{
	// summed while the sender of the previous one is set
	Type_Mc1Mail& mail = entity->mails_0x5A_90[kind];
	if (mail.sender_4)
		mail.value_0 += value;
	else
		mail.value_0 = value;
	mail.sender_4 = sender;
}

static bool TakesMailFrom(const Type_Mc1Entity* sender, const Type_Mc1Entity* entity)
{
	// byte_0x42 / byte_0x43 of the sender: the class / model it sends to, FFh any
	if ((int8_t)sender->byte_0x42_66 == -1)
		return true;
	if (sender->byte_0x42_66 == entity->class_0x40_64 && (int8_t)sender->byte_0x43_67 == -1)
		return true;
	return sender->byte_0x42_66 == entity->class_0x40_64 && sender->byte_0x43_67 == entity->model_0x41_65;
}

static Type_Mc1Entity* FirstInCell(int32_t x, int32_t y)
{
	return &Entities()[mapEntityIndex_10C1E0_10C1D0[(uint16_t)(((y & 0xFF) << 8) + (x & 0xFF))]];
}

static bool InList(const Type_Mc1Entity* entity)
{
	// the lists end with a null pointer, the original with an address not above the entities
	return entity && entity > Entities();
}

// the entities in the map cells around the sender (the kind 0 rules)
static void SendMailToCellsKind0(Type_Mc1Entity* entity, int32_t x, int32_t y, int32_t radius, uint8_t kind, uint16_t value)
{
	uint16_t mask = (uint16_t)(1 << kind);
	for (int32_t dy = -radius; dy <= radius; dy++)
		for (int32_t dx = -radius; dx <= radius; dx++)
			for (Type_Mc1Entity* other = FirstInCell(x + dx, y + dy); other != Entities(); other = &Entities()[other->mapNext_0x14_20])
			{
				if (entity->id_0x18_24 == other->id_0x18_24 || !EntitiesOverlap_11950_11950(entity, other))
					continue;
				if (!(mask & other->mailMask_0x1C_28) || !(other->flags_0x10_16 & 8))
					continue;
				if (other->class_0x40_64 == 3 && other->model_0x41_65 == 2)
					continue;
				if (TakesMailFrom(entity, other))
					AddMail(other, kind, value, entity->id_0x18_24);
			}
}

void SendMailToOverlappingEntities_127E0_127E0(Type_Mc1Entity* entity, uint8_t kind, int32_t value)
{
	if (kind)
		return;
	// the list 21 (model 2): touched are set to 1Eh, the others get the mail
	for (Type_Mc1Entity* other = mc1Settings_AE408_AE3F8->entityLists_0x8E1A_36378[21]; InList(other); other = other->next_0x00_0)
	{
		if (other->model_0x41_65 != 2 || !EntitiesOverlap_11950_11950(entity, other))
			continue;
		other->word_0x32_50 = 0x1E;
		if (other->id_0x18_24 != entity->id_0x18_24)
			AddMail(other, kind, (uint16_t)value, entity->id_0x18_24);
	}
	int32_t x = (entity->position_0x48_72.x - 0x80) / 256;
	int32_t y = (entity->position_0x48_72.y - 0x80) / 256;
	int32_t radius = (entity->box_0x4E_78[1] + 0xFF) / 256;
	SendMailToCellsKind0(entity, x, y, radius, kind, (uint16_t)value);
}

void SendMailAround_120B0_120B0(Type_Mc1Entity* entity, uint8_t kind, int32_t value)
{
	if (!kind)
	{
		for (Type_Mc1Entity* other = mc1Settings_AE408_AE3F8->entityLists_0x8E1A_36378[21]; InList(other); other = other->next_0x00_0)
			if (other->model_0x41_65 == 2 && other->id_0x18_24 != entity->id_0x18_24 && EntitiesOverlap_11950_11950(entity, other))
				AddMail(other, kind, (uint16_t)value, entity->id_0x18_24);
		int32_t x = (entity->position_0x48_72.x - 0x80) / 256;
		int32_t y = (entity->position_0x48_72.y - 0x80) / 256;
		int32_t radius = (entity->box_0x4E_78[1] + 0xFF) / 256;
		SendMailToCellsKind0(entity, x, y, radius, kind, (uint16_t)value);
		return;
	}
	uint16_t mask = (uint16_t)(1 << kind);
	int32_t x = (entity->position_0x48_72.x + 0x80) >> 8;
	int32_t y = (entity->position_0x48_72.y + 0x80) >> 8;
	int32_t radius = (entity->box_0x4E_78[1] + 0xFF) / 256;
	for (int32_t dy = -radius; dy <= radius; dy++)
		for (int32_t dx = -radius; dx <= radius; dx++)
			for (Type_Mc1Entity* other = FirstInCell(x + dx, y + dy); other != Entities(); other = &Entities()[other->mapNext_0x14_20])
			{
				if (entity->id_0x18_24 == other->id_0x18_24 || !other->class_0x40_64 || !(other->flags_0x10_16 & 8))
					continue;
				if (!(mask & other->mailMask_0x1C_28) || !TakesMailFrom(entity, other))
					continue;
				if (EntitiesOverlap_11950_11950(entity, other))
					AddMail(other, kind, (uint16_t)value, entity->id_0x18_24);
			}
}

void SetHalfMana_36F90_37350(Type_Mc1Entity* entity)
{
	entity->mana_0x8C_140 = entity->maxLife_0x08_8 >> 1;
}

int32_t GetFreeEntityCount_37710_37AD0()
{
	return mc1State_AE400_AE3F0->freeTop_0x28_40 + 1;
}

int32_t MoveOnMap_41CC0_42000(Type_Mc1Entity* entity, Type_Mc1Position* position)
{
	RemoveFromMap_41DD0_42110(entity);
	AddToMap_41CF0_42030(entity, position);
	return 1;
}
