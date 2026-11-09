#include "Mc1Scenery.h"

#include <cstring>

#include "Mc1Entity.h"
#include "Mc1Game.h"
#include "Mc1NotPorted.h"
#include "Mc1Shared.h"

// the models of class 10 (the features of the level), the create functions of classes_96902_96902[10]

static Type_Mc1Entity* NewScenery(uint8_t model, uint8_t action)
{
	Type_Mc1Entity* entity = AllocEntity_372C0_37680();
	if (entity)
	{
		entity->actionIndex_0x46_70 = action;
		entity->class_0x40_64 = 10;
		entity->model_0x41_65 = model;
	}
	return entity;
}

static uint32_t NextRand(Type_Mc1Entity* entity)
{
	entity->rand_0x04_4 = entity->rand_0x04_4 * 0x24A1 + 0x24DF;
	return entity->rand_0x04_4;
}

// and 0FFFDFFF7h, or byte +12h 2
static void SetStaticFlags(Type_Mc1Entity* entity)
{
	entity->flags_0x10_16 = (entity->flags_0x10_16 & 0xFFFDFFF7) | 0x20000;
}

static void ClearNewFlag(Type_Mc1Entity* entity)
{
	entity->flags_0x10_16 &= ~8u;
}

static void SetGroundZ(Type_Mc1Entity* entity, Type_Mc1Position* position, int16_t add)
{
	entity->position_0x48_72.z = (int16_t)(GetTerrainAlt_11F50_11F50((axis_3d*)position) + add);
}

Type_Mc1Entity* CreateScenery00_3A490_3A810(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = AllocEntity_372C0_37680();
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = 8;
	entity->actionIndex_0x46_70 = 0;
	entity->class_0x40_64 = 10;
	entity->model_0x41_65 = 0;
	entity->word_0x2C_44 = 0x190;
	entity->mailMask_0x1C_28 = 0;
	SetStaticFlags(entity);
	AddToMap_41CF0_42030(entity, position);
	ResetLife_36DE0_371A0(entity);
	SetSprite_36FA0_37360(entity, 7);
	SetBox_37130_374F0(entity, 0x80, 0x80);
	return entity;
}

Type_Mc1Entity* CreateScenery01_3A510_3A890(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewScenery(1, 1);
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = 1;
	entity->word_0x2C_44 = 0x190;
	SetStaticFlags(entity);
	AddToMap_41CF0_42030(entity, position);
	ResetLife_36DE0_371A0(entity);
	SetSprite_36FA0_37360(entity, 0x29);
	return entity;
}

Type_Mc1Entity* CreateScenery02_3A570_3A8F0(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = AllocEntity_372C0_37680();
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = 8;
	entity->actionIndex_0x46_70 = 2;
	entity->class_0x40_64 = 10;
	entity->model_0x41_65 = 2;
	entity->position_0x48_72 = *position;
	entity->word_0x1A_26 = 0;
	entity->flags_0x10_16 = (entity->flags_0x10_16 & 0xFFFDFFF6) | 0x20001;
	ResetLife_36DE0_371A0(entity);
	return entity;
}

Type_Mc1Entity* CreateScenery03_3A5D0_3A950(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewScenery(3, 3);
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = 7;
	entity->word_0x2C_44 = 0;
	SetStaticFlags(entity);
	entity->word_0x1A_26 = 0;
	AddToMap_41CF0_42030(entity, position);
	ResetLife_36DE0_371A0(entity);
	SetSprite_36FA0_37360(entity, 0x24);
	return entity;
}

Type_Mc1Entity* CreateScenery04_3A640_3A9C0(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewScenery(4, 4);
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = 0x64;
	entity->word_0x2C_44 = 0;
	SetStaticFlags(entity);
	entity->position_0x48_72 = *position;
	entity->word_0x1A_26 = 0;
	SetGroundZ(entity, &entity->position_0x48_72, 0);
	ResetLife_36DE0_371A0(entity);
	return entity;
}

Type_Mc1Entity* CreateScenery05_3A6B0_3AA30(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewScenery(5, 5);
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = 8;
	entity->word_0x2C_44 = 0;
	SetStaticFlags(entity);
	entity->word_0x1A_26 = 0;
	AddToMap_41CF0_42030(entity, position);
	SetGroundZ(entity, &entity->position_0x48_72, 0);
	ResetLife_36DE0_371A0(entity);
	SetSprite_36FA0_37360(entity, 0xF4);
	return entity;
}

Type_Mc1Entity* CreateScenery06_3A730_3AAB0(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewScenery(6, 6);
	if (!entity)
		return nullptr;
	entity->word_0x2C_44 = 0x32;
	entity->maxLife_0x08_8 = 0xF0;
	entity->word_0x2E_46 = 0;
	SetStaticFlags(entity);
	AddToMap_41CF0_42030(entity, position);
	SetGroundZ(entity, position, 0);
	ResetLife_36DE0_371A0(entity);
	SetSprite_36FA0_37360(entity, 0xE4);
	SetBox_37130_374F0(entity, 0x110, 0x600);
	entity->word_0x1A_26 = 0;
	return entity;
}

Type_Mc1Entity* CreateScenery07_3A7D0_3AB50(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewScenery(7, 7);
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = 0x0C;
	SetStaticFlags(entity);
	entity->word_0x7E_126 = (int16_t)(NextRand(entity) % 20 + 20);
	entity->byte_0x42_66 = 0x0A;
	entity->byte_0x43_67 = 7;
	AddToMap_41CF0_42030(entity, position);
	SetGroundZ(entity, position, 0);
	SetSpriteBox_37030_373F0(entity, 0x4E);
	ResetLife_36DE0_371A0(entity);
	entity->flags_0x10_16 |= 1;
	return entity;
}

Type_Mc1Entity* CreateScenery08_3A870_3ABF0(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewScenery(8, 8);
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = 8;
	entity->position_0x48_72 = *position;
	ClearNewFlag(entity);
	entity->word_0x2C_44 = 0x64;
	ResetLife_36DE0_371A0(entity);
	SetBox_37130_374F0(entity, 0x200, 0x200);
	return entity;
}

Type_Mc1Entity* CreateScenery09_3A8D0_3AC50(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewScenery(9, 9);
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = 0x11;
	entity->position_0x48_72 = *position;
	entity->word_0x2C_44 = 0x7D0;
	ClearNewFlag(entity);
	ResetLife_36DE0_371A0(entity);
	SetBox_37130_374F0(entity, 0x300, 0x2000);
	return entity;
}

Type_Mc1Entity* CreateScenery10_3A930_3ACB0(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewScenery(0x0A, 0x0A);
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = 1;
	entity->position_0x48_72 = *position;
	entity->word_0x2C_44 = 0x64;
	SetStaticFlags(entity);
	ResetLife_36DE0_371A0(entity);
	SetBox_37130_374F0(entity, 0x80, 0x80);
	return entity;
}

Type_Mc1Entity* CreateScenery11_3A9A0_3AD20(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewScenery(0x0B, 0x0B);
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = 0x28;
	entity->position_0x48_72 = *position;
	entity->word_0x2C_44 = 0xC8;
	SetStaticFlags(entity);
	ResetLife_36DE0_371A0(entity);
	SetBox_37130_374F0(entity, 0x900, 0x2000);
	return entity;
}

Type_Mc1Entity* CreateScenery12_3AA10_3AD90(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewScenery(0x0C, 0x0C);
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = 8;
	entity->position_0x48_72 = *position;
	entity->word_0x2C_44 = (int16_t)0xFA00;
	entity->flags_0x10_16 = (entity->flags_0x10_16 & ~9u) | 1;
	AddToMap_41CF0_42030(entity, position);
	ResetLife_36DE0_371A0(entity);
	SetSprite_36FA0_37360(entity, 0x29);
	entity->flags_0x10_16 |= 1;
	SetBox_37130_374F0(entity, 0x200, 0x200);
	return entity;
}

// 13 / 14: random life and 7Eh
static Type_Mc1Entity* CreateRandomScenery(Type_Mc1Position* position, uint8_t model, uint32_t lifeRange, int32_t lifeBase, int16_t sprite)
{
	Type_Mc1Entity* entity = NewScenery(model, model);
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = (int32_t)(NextRand(entity) % lifeRange + lifeBase);
	entity->flags_0x10_16 &= 0xFFFDFFF7;
	entity->word_0x7E_126 = (int16_t)(NextRand(entity) % 0x35 + 0x33);
	entity->byte_0x42_66 = 0x0A;
	entity->byte_0x43_67 = model;
	entity->flags_0x10_16 |= 0x20000;
	AddToMap_41CF0_42030(entity, position);
	SetSpriteBox_37030_373F0(entity, sprite);
	ResetLife_36DE0_371A0(entity);
	return entity;
}

Type_Mc1Entity* CreateScenery13_3AAA0_3AE20(Type_Mc1Position* position)
{
	return CreateRandomScenery(position, 0x0D, 0x17, 0x11, 0x43);
}

Type_Mc1Entity* CreateScenery14_3AB40_3AEC0(Type_Mc1Position* position)
{
	return CreateRandomScenery(position, 0x0E, 0x21, 0x1C, 9);
}

// 15, 53-56: a random direction in 1Eh
static Type_Mc1Entity* CreateTurningScenery(Type_Mc1Position* position, uint8_t model, uint8_t action, int32_t life, int16_t word1A, bool flag1)
{
	Type_Mc1Entity* entity = NewScenery(model, action);
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = life;
	entity->word_0x7E_126 = 0x100;
	ClearNewFlag(entity);
	uint32_t rand = NextRand(entity);
	entity->word_0x2C_44 = 0x64;
	entity->word_0x1A_26 = word1A;
	entity->word_0x1E_30 = (int16_t)(rand & 0x7FF);
	entity->position_0x48_72 = *position;
	if (flag1)
		entity->flags_0x10_16 |= 1;
	ResetLife_36DE0_371A0(entity);
	SetBox_37130_374F0(entity, 0x400, 0x4000);
	return entity;
}

Type_Mc1Entity* CreateScenery15_3ABE0_3AF60(Type_Mc1Position* position)
{
	return CreateTurningScenery(position, 0x0F, 0x0F, 0x80, 0, false);
}

Type_Mc1Entity* CreateScenery16_3ACC0_3B040(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewScenery(0x10, 0x10);
	if (!entity)
		return nullptr;
	entity->word_0x2C_44 = 0xC8;
	entity->flags_0x10_16 &= 0xFFFDFFF7;
	entity->maxLife_0x08_8 = (int32_t)(NextRand(entity) % 100 + 100);
	uint32_t speed = NextRand(entity) % 0x32;
	entity->word_0x2E_46 = 0x100;
	entity->word_0x1E_30 = (int16_t)(NextRand(entity) & 0x7FF);
	entity->word_0x7E_126 = (int16_t)(speed + 0x34);
	entity->flags_0x10_16 |= 0x20000;
	AddToMap_41CF0_42030(entity, position);
	SetGroundZ(entity, position, 0x40);
	MoveEntity_41EC0_42200((axis_3d*)&entity->target_0x96_150, (uint16_t)entity->word_0x1E_30, 0, entity->word_0x7E_126);
	ResetLife_36DE0_371A0(entity);
	SetSprite_36FA0_37360(entity, 0xD2);
	return entity;
}

Type_Mc1Entity* CreateScenery17_3AC70_3AFF0(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewScenery(0x11, 0x11);
	if (!entity)
		return nullptr;
	entity->position_0x48_72 = *position;
	entity->maxLife_0x08_8 = 0x0A;
	entity->word_0x2C_44 = 0xBB8;
	ClearNewFlag(entity);
	ResetLife_36DE0_371A0(entity);
	return entity;
}

Type_Mc1Entity* CreateScenery18_3ADB0_3B130(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewScenery(0x12, 0x12);
	if (!entity)
		return nullptr;
	entity->word_0x2C_44 = 0xC8;
	entity->word_0x1A_26 = 0;
	entity->maxLife_0x08_8 = 0x2710;
	ClearNewFlag(entity);
	entity->position_0x48_72 = *position;
	ResetLife_36DE0_371A0(entity);
	return entity;
}

Type_Mc1Entity* CreateScenery19_3AE00_3B180(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewScenery(0x13, 0x13);
	if (!entity)
		return nullptr;
	entity->word_0x2C_44 = 0xC8;
	entity->maxLife_0x08_8 = 0xF0;
	SetStaticFlags(entity);
	AddToMap_41CF0_42030(entity, position);
	entity->flags_0x10_16 |= 1;
	ResetLife_36DE0_371A0(entity);
	SetSprite_36FA0_37360(entity, 0xE4);
	SetBox_37130_374F0(entity, 0x200, 0x200);
	return entity;
}

Type_Mc1Entity* CreateScenery23_3AE80_3B200(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = AllocEntity_372C0_37680();
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = 8;
	entity->actionIndex_0x46_70 = 0x17;
	entity->class_0x40_64 = 10;
	entity->model_0x41_65 = 0x17;
	entity->word_0x2C_44 = 0x19;
	SetStaticFlags(entity);
	AddToMap_41CF0_42030(entity, position);
	ResetLife_36DE0_371A0(entity);
	SetSprite_36FA0_37360(entity, 7);
	SetBox_37130_374F0(entity, 0xC8, 0xC8);
	entity->flags_0x10_16 |= 1;
	return entity;
}

// 25, 26, 36, 43: on the map with a sprite and a box of 200h
static Type_Mc1Entity* CreateBoxScenery(Type_Mc1Position* position, uint8_t model, uint8_t action, int16_t word2C, int16_t sprite)
{
	Type_Mc1Entity* entity = NewScenery(model, action);
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = 8;
	entity->position_0x48_72 = *position;
	entity->word_0x2C_44 = word2C;
	ClearNewFlag(entity);
	AddToMap_41CF0_42030(entity, position);
	ResetLife_36DE0_371A0(entity);
	SetSprite_36FA0_37360(entity, sprite);
	SetBox_37130_374F0(entity, 0x200, 0x200);
	return entity;
}

Type_Mc1Entity* CreateScenery25_3AF00_3B280(Type_Mc1Position* position)
{
	return CreateBoxScenery(position, 0x19, 0x19, 0x7D0, 0x11B);
}

Type_Mc1Entity* CreateScenery26_3AF80_3B300(Type_Mc1Position* position)
{
	return CreateBoxScenery(position, 0x1A, 0x1A, 0xC8, 0x11C);
}

Type_Mc1Entity* CreateScenery27_3B000_3B380(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = AllocEntity_372C0_37680();
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = 2;
	entity->actionIndex_0x46_70 = 0x1B;
	entity->class_0x40_64 = 10;
	entity->model_0x41_65 = 0x1B;
	entity->word_0x2C_44 = (int16_t)((position->z >> 5) + 0x30);
	entity->word_0x1A_26 = 0x0A;
	ClearNewFlag(entity);
	AddToMap_41CF0_42030(entity, position);
	ResetLife_36DE0_371A0(entity);
	return entity;
}

// 28-31, 50: no life, on the map
static Type_Mc1Entity* CreateMarkerScenery(Type_Mc1Position* position, uint8_t model, uint8_t action)
{
	Type_Mc1Entity* entity = AllocEntity_372C0_37680();
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = 0;
	entity->actionIndex_0x46_70 = action;
	entity->class_0x40_64 = 10;
	entity->model_0x41_65 = model;
	entity->position_0x48_72 = *position;
	ClearNewFlag(entity);
	AddToMap_41CF0_42030(entity, position);
	ResetLife_36DE0_371A0(entity);
	return entity;
}

Type_Mc1Entity* CreateScenery28_3B060_3B3E0(Type_Mc1Position* position)
{
	return CreateMarkerScenery(position, 0x1C, 0x1E);
}

Type_Mc1Entity* CreateScenery29_3B120_3B4A0(Type_Mc1Position* position)
{
	return CreateMarkerScenery(position, 0x1D, 0x1F);
}

Type_Mc1Entity* CreateScenery30_3B0C0_3B440(Type_Mc1Position* position)
{
	return CreateMarkerScenery(position, 0x1E, 0x20);
}

Type_Mc1Entity* CreateScenery31_3B1D0_3B550(Type_Mc1Position* position)
{
	return CreateMarkerScenery(position, 0x1F, 0x21);
}

Type_Mc1Entity* CreateScenery50_3B2A0_3B620(Type_Mc1Position* position)
{
	return CreateMarkerScenery(position, 0x32, 0x36);
}

Type_Mc1Entity* CreateScenery32_3B180_3B500(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = AllocEntity_372C0_37680();
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = 0;
	entity->actionIndex_0x46_70 = 0x22;
	entity->class_0x40_64 = 10;
	entity->model_0x41_65 = 0x20;
	entity->position_0x48_72 = *position;
	ClearNewFlag(entity);
	entity->word_0x7E_126 = 0x100;
	ResetLife_36DE0_371A0(entity);
	return entity;
}

Type_Mc1Entity* CreateScenery51_3B230_3B5B0(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = AllocEntity_372C0_37680();
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = 0;
	entity->actionIndex_0x46_70 = 0x37;
	entity->class_0x40_64 = 10;
	entity->model_0x41_65 = 0x33;
	entity->position_0x48_72 = *position;
	entity->word_0x1A_26 = 0x100;
	ClearNewFlag(entity);
	entity->word_0x7E_126 = 0x400;
	SetBox_37130_374F0(entity, 0x300, 0x300);
	ResetLife_36DE0_371A0(entity);
	return entity;
}

Type_Mc1Entity* CreateScenery34_3B300_3B680(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewScenery(0x22, 0x24);
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = 0;
	entity->byte_0x42_66 = 3;
	entity->byte_0x43_67 = 0xFF;
	ClearNewFlag(entity);
	entity->position_0x48_72 = *position;
	SetSprite_36FA0_37360(entity, 0xDF);
	SetBox_37130_374F0(entity, 0x100, 0x100);
	ResetLife_36DE0_371A0(entity);
	AddToMap_41CF0_42030(entity, position);
	SetGroundZ(entity, &entity->position_0x48_72, 0x280);
	entity->target_0x96_150 = entity->position_0x48_72;
	MoveEntity_41EC0_42200((axis_3d*)&entity->target_0x96_150, (uint16_t)(NextRand(entity) & 0x7FF), 0, (int16_t)0x8000);
	return entity;
}

Type_Mc1Entity* CreateScenery35_3B3D0_3B750(Type_Mc1Position* /*position*/)
{
	// only allocated
	return AllocEntity_372C0_37680();
}

Type_Mc1Entity* CreateScenery36_3B3E0_3B760(Type_Mc1Position* position)
{
	return CreateBoxScenery(position, 0x24, 0x26, (int16_t)0xFA00, 0x29);
}

Type_Mc1Entity* CreateScenery38_3B460_3B7E0(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewScenery(0x26, 0x28);
	if (!entity)
		return nullptr;
	ClearNewFlag(entity);
	entity->maxLife_0x08_8 = 0x20;
	AddToMap_41CF0_42030(entity, position);
	ResetLife_36DE0_371A0(entity);
	SetSprite_36FA0_37360(entity, 0x110);
	SetBox_37130_374F0(entity, 0x200, 0x200);
	return entity;
}

Type_Mc1Entity* CreateScenery33_3B4D0_3B850(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewScenery(0x21, 0x23);
	if (!entity)
		return nullptr;
	ClearNewFlag(entity);
	entity->position_0x48_72 = *position;
	ResetLife_36DE0_371A0(entity);
	SetBox_37130_374F0(entity, 0x200, 0x200);
	return entity;
}

Type_Mc1Entity* CreateScenery39_3B5A0_3B920(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewScenery(0x27, 0x29);
	if (!entity)
		return nullptr;
	entity->byte_0x42_66 = 0x0A;
	entity->byte_0x43_67 = 0x27;
	entity->word_0x2E_46 = 0x80;
	entity->mana_0x8C_140 = 0x200;
	if (mc1Settings_AE408_AE3F8->flags_0x00_0 & 0x200)
		entity->mana_0x8C_140 = 0x9C4;
	entity->word_0x7E_126 = 0x20;
	entity->mailMask_0x1C_28 = 3;
	entity->byte_0x3A_58 = 0x80;
	entity->byte_0x3B_59 = 0;
	AddToMap_41CF0_42030(entity, position);
	ResetLife_36DE0_371A0(entity);
	SetManaSprite_274D0_274D0(entity);
	return entity;
}

Type_Mc1Entity* CreateScenery40_3B620_3B9A0(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewScenery(0x28, 0x2A);
	if (!entity)
		return nullptr;
	entity->word_0x1A_26 = (int16_t)((entity - mc1State_AE400_AE3F0->entities_0x7463_29795) % 11);
	entity->mailMask_0x1C_28 = 2;
	AddToMap_41CF0_42030(entity, position);
	ResetLife_36DE0_371A0(entity);
	SetSpriteBox_37030_373F0(entity, 0x41);
	return entity;
}

Type_Mc1Entity* CreateScenery41_3B760_3BAE0(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewScenery(0x29, 0x2B);
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = 0;
	ClearNewFlag(entity);
	entity->position_0x48_72 = *position;
	ResetLife_36DE0_371A0(entity);
	return entity;
}

Type_Mc1Entity* CreateScenery42_3B7B0_3BB30(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewScenery(0x2A, 0x2C);
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = 0;
	ClearNewFlag(entity);
	entity->position_0x48_72 = *position;
	entity->byte_0x3C_60 = 1;
	ResetLife_36DE0_371A0(entity);
	return entity;
}

Type_Mc1Entity* CreateScenery43_3B6F0_3BA70(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewScenery(0x2B, 0x2D);
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = 8;
	entity->word_0x2C_44 = (int16_t)0xFA00;
	ClearNewFlag(entity);
	AddToMap_41CF0_42030(entity, position);
	ResetLife_36DE0_371A0(entity);
	SetSprite_36FA0_37360(entity, 0x29);
	SetBox_37130_374F0(entity, 0x200, 0x200);
	return entity;
}

Type_Mc1Entity* CreateScenery44_3B800_3BB80(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewScenery(0x2C, 0x2E);
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = 0x1F4;
	entity->word_0x2C_44 = 0x1F4;
	ClearNewFlag(entity);
	AddToMap_41CF0_42030(entity, position);
	ResetLife_36DE0_371A0(entity);
	SetSprite_36FA0_37360(entity, 8);
	return entity;
}

Type_Mc1Entity* CreateScenery45_3B690_3BA10(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = AllocEntity_372C0_37680();
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = 0x1E;
	entity->actionIndex_0x46_70 = 0x33;
	entity->class_0x40_64 = 10;
	entity->model_0x41_65 = 0x2D;
	entity->word_0x2C_44 = 0x64;
	entity->word_0x1A_26 = 4;
	entity->flags_0x10_16 = 9;
	entity->mailMask_0x1C_28 = 0x21;
	AddToMap_41CF0_42030(entity, position);
	SetSprite_36FA0_37360(entity, 0xB1);
	return entity;
}

Type_Mc1Entity* CreateScenery52_3B860_3BBE0(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewScenery(0x34, 0x38);
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = 0x186A0;
	entity->word_0x2C_44 = 0x1F4;
	entity->word_0x1A_26 = 0x258;
	entity->mana_0x8C_140 = 0x1F4;
	entity->dword_0x88_136 = 0x7D0;
	ClearNewFlag(entity);
	AddToMap_41CF0_42030(entity, position);
	ResetLife_36DE0_371A0(entity);
	SetSprite_36FA0_37360(entity, 0xCD);
	return entity;
}

Type_Mc1Entity* CreateScenery53_3B8E0_3BC60(Type_Mc1Position* position)
{
	if (mc1Variant == MC1_VARIANT_HW)
	{
		// 3BC60: HW has another model 53
		Type_Mc1Entity* entity = NewScenery(0x35, 0x3A);
		if (!entity)
			return nullptr;
		entity->position_0x48_72 = *position;
		entity->maxLife_0x08_8 = 6;
		entity->word_0x2C_44 = 0xBB8;
		ClearNewFlag(entity);
		ResetLife_36DE0_371A0(entity);
		return entity;
	}
	return CreateTurningScenery(position, 0x35, 0x3A, 0x80, 0, true);
}

Type_Mc1Entity* CreateScenery54_3B970_3BCB0(Type_Mc1Position* position)
{
	return CreateTurningScenery(position, 0x36, 0x3B, 0x80, 0, true);
}

Type_Mc1Entity* CreateScenery55_3BA00_3BD40(Type_Mc1Position* position)
{
	return CreateTurningScenery(position, 0x37, 0x3C, 0x13, 0x20, true);
}

Type_Mc1Entity* CreateScenery56_3BA90_3BDD0(Type_Mc1Position* position)
{
	return CreateTurningScenery(position, 0x38, 0x3D, 0x80, 0, true);
}

// 900A4: the mana limits of the sprites
static const int32_t manaSpriteLimits_900A4_900A4[8] = { 0x100, 0x200, 0x400, 0x800, 0x1000, 0x23E8, 0x47D0, 0x8FA0 };

void SetManaSprite_274D0_274D0(Type_Mc1Entity* entity)
{
	// the sprite by the mana from the colour of the owner (class 3), else 34h
	int16_t step = 0;
	while (step < 7 && entity->mana_0x8C_140 > manaSpriteLimits_900A4_900A4[step])
		step++;
	Type_Mc1Entity* owner = &mc1State_AE400_AE3F0->entities_0x7463_29795[entity->owner_0x90_144];
	int16_t first = 0x34;
	if (entity->owner_0x90_144 && owner->class_0x40_64 == 3)
	{
		int16_t colour;
		memcpy(&colour, owner->playerData_0xA0_160 + 0x30, sizeof(colour));
		static const int16_t sprites[9] = { 0x34, 0x69, 0x71, 0x79, 0x81, 0x89, 0x91, 0x99, 0xA1 };
		// other colours keep the caller's cx in the original, not reached
		if (colour >= -1 && colour <= 7)
			first = sprites[colour + 1];
	}
	int16_t sprite = (int16_t)(first + step);
	if (entity->sprite_0x56_86 == sprite)
		return;
	SetSprite_36FA0_37360(entity, sprite);
	if (step)
		SetSpriteHalfBox_370E0_374A0(entity, sprite);
}
