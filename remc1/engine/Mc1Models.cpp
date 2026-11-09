#include "Mc1Models.h"

#include <cstring>

#include "Mc1Entity.h"
#include "Mc1Game.h"
#include "Mc1Shared.h"

// the create functions of the models of the classes 2, 3, 5, 11 and 12 (classes_96902_96902), the same in HW

static Type_Mc1Entity* Entities()
{
	return mc1State_AE400_AE3F0->entities_0x7463_29795;
}

static int32_t EntityIndex(const Type_Mc1Entity* entity)
{
	return (int32_t)(entity - Entities());
}

static uint32_t NextRand(Type_Mc1Entity* entity)
{
	entity->rand_0x04_4 = entity->rand_0x04_4 * 0x24A1 + 0x24DF;
	return entity->rand_0x04_4;
}

static Type_Mc1Entity* NewEntity(uint8_t entityClass, uint8_t model, uint8_t action)
{
	Type_Mc1Entity* entity = AllocEntity_372C0_37680();
	if (entity)
	{
		entity->actionIndex_0x46_70 = action;
		entity->class_0x40_64 = entityClass;
		entity->model_0x41_65 = model;
	}
	return entity;
}

// class 2

static Type_Mc1Entity* NewClass2(uint8_t model, uint8_t action, bool keepNew)
{
	Type_Mc1Entity* entity = AllocEntity_372C0_37680();
	if (!entity)
		return nullptr;
	if (!keepNew)
		entity->flags_0x10_16 &= ~8u;
	entity->word_0x1A_26 = (int16_t)(EntityIndex(entity) % 11);
	entity->actionIndex_0x46_70 = action;
	entity->class_0x40_64 = 2;
	entity->model_0x41_65 = model;
	return entity;
}

Type_Mc1Entity* CreateClass02Model00_37BC0_37F80(Type_Mc1Position* position)
{
	// placed up to 20h off, one of two sprites
	Type_Mc1Entity* entity = NewEntity(2, 0, 0);
	if (!entity)
		return nullptr;
	entity->word_0x1A_26 = (int16_t)(EntityIndex(entity) % 11);
	entity->life_0x0C_12 = (int32_t)(NextRand(entity) % 5000 + 2500);
	entity->mailMask_0x1C_28 = 1;
	Type_Mc1Position place = *position;
	place.x = (int16_t)(place.x + (int32_t)(NextRand(entity) & 0x3F) - 0x20);
	place.y = (int16_t)(place.y + (int32_t)(NextRand(entity) & 0x3F) - 0x20);
	AddToMap_41CF0_42030(entity, &place);
	ResetLife_36DE0_371A0(entity);
	SetSpriteBox_37030_373F0(entity, (NextRand(entity) & 1) ? 0x54 : 0x53);
	return entity;
}

static Type_Mc1Entity* CreateClass2(Type_Mc1Position* position, uint8_t model, uint8_t action, int16_t sprite, bool keepNew)
{
	Type_Mc1Entity* entity = NewClass2(model, action, keepNew);
	if (!entity)
		return nullptr;
	AddToMap_41CF0_42030(entity, position);
	ResetLife_36DE0_371A0(entity);
	SetSpriteBox_37030_373F0(entity, sprite);
	return entity;
}

Type_Mc1Entity* CreateClass02Model01_37CF0_38070(Type_Mc1Position* position)
{
	return CreateClass2(position, 1, 3, 0x4F, false);
}

Type_Mc1Entity* CreateClass02Model02_37D70_380F0(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = CreateClass2(position, 2, 6, 0x27, false);
	if (entity)
		SetBox_37130_374F0(entity, 0x400, 0x400);
	return entity;
}

Type_Mc1Entity* CreateClass02Model03_37E00_38180(Type_Mc1Position* position)
{
	return CreateClass2(position, 3, 9, 0x10E, false);
}

Type_Mc1Entity* CreateClass02Model04_37E80_38200(Type_Mc1Position* position)
{
	return CreateClass2(position, 4, 0x0C, 0x30, true);
}

Type_Mc1Entity* CreateClass02Model05_37EF0_38270(Type_Mc1Position* position)
{
	return CreateClass2(position, 5, 0x0F, 0x30, true);
}

// class 3

static Type_Mc1Entity* CreateClass3Ball(Type_Mc1Position* position, uint8_t model, uint8_t typeIndex)
{
	Type_Mc1Entity* entity = NewEntity(3, model, model);
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = 10000;
	entity->word_0x80_128 = 0x50;
	entity->typeParams_0x9C_156 = creatureTypes_98F38_98F38[typeIndex];
	entity->mailMask_0x1C_28 = 0x1D;
	entity->id_0x18_24 = (uint16_t)EntityIndex(entity);
	AddToMap_41CF0_42030(entity, position);
	SetSprite_36FA0_37360(entity, 0x2C);
	ResetLife_36DE0_371A0(entity);
	return entity;
}

Type_Mc1Entity* CreateClass03Model00_37820_37BE0(Type_Mc1Position* position)
{
	return CreateClass3Ball(position, 0, 7);
}

Type_Mc1Entity* CreateClass03Model01_378A0_37C60(Type_Mc1Position* position)
{
	return CreateClass3Ball(position, 1, 8);
}

Type_Mc1Entity* CreateClass03Model02_37920_37CE0(Type_Mc1Position* position)
{
	// on the even tile of the position, the target is the same
	featurePosition_AE454_AE444 = *position;
	Type_Mc1Entity* entity = NewEntity(3, 2, 5);
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = 40000;
	entity->word_0x1A_26 = 0;
	entity->mailMask_0x1C_28 = 0x21;
	Type_Mc1Position place = featurePosition_AE454_AE444;
	place.x = (uint8_t)(featurePosition_AE454_AE444.x >> 8);
	place.y = (uint8_t)(featurePosition_AE454_AE444.y >> 8);
	place.z = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&featurePosition_AE454_AE444);
	if ((place.y + place.x) % 2)
		place.x++;
	place.x = (int16_t)(place.x << 8);
	place.y = (int16_t)(place.y << 8);
	entity->target_0x96_150 = place;
	AddToMap_41CF0_42030(entity, &place);
	ResetLife_36DE0_371A0(entity);
	SetSprite_36FA0_37360(entity, 0xB1);
	return entity;
}

Type_Mc1Entity* CreateClass03Model03_37A00_37DC0(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewEntity(3, 3, 7);
	if (!entity)
		return nullptr;
	entity->maxLife_0x08_8 = 10000;
	entity->word_0x7E_126 = 0x30;
	entity->dword_0x88_136 = 10000;
	entity->mana_0x8C_140 = 0;
	entity->mailMask_0x1C_28 = 1;
	entity->typeParams_0x9C_156 = creatureTypes_98F38_98F38[9];
	AddToMap_41CF0_42030(entity, position);
	ResetLife_36DE0_371A0(entity);
	SetSprite_36FA0_37360(entity, 0xA9);
	return entity;
}

// the models 4-11: the start positions of the players, no entity
static Type_Mc1Entity* SetPlayerStart(Type_Mc1Position* position, int player)
{
	mc1State_AE400_AE3F0->playerStarts_0x23D9_9177[player] = *position;
	return nullptr;
}

Type_Mc1Entity* CreateClass03Model04_37720_37AE0(Type_Mc1Position* position)
{
	return SetPlayerStart(position, 0);
}

Type_Mc1Entity* CreateClass03Model05_37740_37B00(Type_Mc1Position* position)
{
	return SetPlayerStart(position, 1);
}

Type_Mc1Entity* CreateClass03Model06_37760_37B20(Type_Mc1Position* position)
{
	return SetPlayerStart(position, 2);
}

Type_Mc1Entity* CreateClass03Model07_37780_37B40(Type_Mc1Position* position)
{
	return SetPlayerStart(position, 3);
}

Type_Mc1Entity* CreateClass03Model08_377A0_37B60(Type_Mc1Position* position)
{
	return SetPlayerStart(position, 4);
}

Type_Mc1Entity* CreateClass03Model09_377C0_37B80(Type_Mc1Position* position)
{
	return SetPlayerStart(position, 5);
}

Type_Mc1Entity* CreateClass03Model10_377E0_37BA0(Type_Mc1Position* position)
{
	return SetPlayerStart(position, 6);
}

Type_Mc1Entity* CreateClass03Model11_37800_37BC0(Type_Mc1Position* position)
{
	return SetPlayerStart(position, 7);
}

// class 5

static void NewCreature(Type_Mc1Entity* entity, int16_t speed, int16_t word82)
{
	entity->word_0x80_128 = speed;
	entity->word_0x82_130 = word82;
}

static void SetRandomYaw(Type_Mc1Entity* entity)
{
	int16_t yaw = (int16_t)((NextRand(entity) & 0x7FF) - 1);
	entity->word_0x22_34 = yaw;
	entity->word_0x1E_30 = yaw;
	entity->word_0x20_32 = yaw;
}

static void CountModel(Type_Mc1Entity* entity)
{
	// the number of the model so far, kept in index_0x3F
	uint8_t& counter = mc1State_AE400_AE3F0->modelCounters_0x0C_12[(int8_t)entity->model_0x41_65];
	entity->index_0x3F_63 = counter++;
}

static int16_t TypeParam1A(const Type_Mc1Entity* entity)
{
	int16_t value;
	memcpy(&value, entity->typeParams_0x9C_156 + 0x1A, sizeof(value));
	return value;
}

static uint8_t SpreadByIndex(const Type_Mc1Entity* entity)
{
	int32_t period = TypeParam1A(entity);
	return (uint8_t)(period - entity->index_0x3F_63 % period + 4);
}

// the common part: the yaw, word_0x1A, the counter of the model, the type
static void InitCreature(Type_Mc1Entity* entity, uint8_t typeIndex, int16_t word2C, bool setWord2C)
{
	entity->word_0x1A_26 = (int16_t)(EntityIndex(entity) % 100);
	entity->word_0x24_36 = 0;
	if (setWord2C)
		entity->word_0x2C_44 = word2C;
	entity->mailMask_0x1C_28 = 1;
	CountModel(entity);
	entity->typeParams_0x9C_156 = creatureTypes_98F38_98F38[typeIndex];
}

// the models 0 and 3: a head and 16 segments linked by word_0x34 / word_0x36
static Type_Mc1Entity* CreateLongCreature(Type_Mc1Position* position, uint8_t model, uint8_t action, int16_t speed,
	uint8_t typeIndex, int16_t firstSprite, int16_t headSprite, bool segmentMana)
{
	Type_Mc1Entity* head = NewEntity(5, model, action);
	if (!head)
		return nullptr;
	NewCreature(head, speed, 0x10);
	head->word_0x7E_126 = 0x1E;
	head->maxLife_0x08_8 = 9000;
	SetHalfMana_36F90_37350(head);
	head->dword_0x88_136 = head->mana_0x8C_140;
	head->mana_0x8C_140 = head->mana_0x8C_140 / 2;
	SetRandomYaw(head);
	InitCreature(head, typeIndex, 0, false);
	head->word_0x38_56 = 0x60;
	head->byte_0x3A_58 = SpreadByIndex(head);
	head->byte_0x42_66 = 3;
	Type_Mc1Entity* previous = head;
	for (int32_t i = 0; i <= 15; i++)
	{
		Type_Mc1Entity* segment = AllocEntity_372C0_37680();
		if (segment)
		{
			*segment = *head;
			segment->word_0x34_52 = (uint16_t)EntityIndex(previous);
			if (previous) // the original writes to the address 36h
				previous->word_0x36_54 = (uint16_t)EntityIndex(segment);
			segment->word_0x36_54 = 0;
			segment->actionIndex_0x46_70 = 0x78;
			// model 0 sets it on the head, model 3 on the segment
			(segmentMana ? segment : head)->mana_0x8C_140 = head->dword_0x88_136 / 32;
			segment->index_0x3F_63 = (uint8_t)i;
			SetSprite_36FA0_37360(segment, (int16_t)(firstSprite + i));
			segment->word_0x38_56 = segment->box_0x4E_78[1];
			AddToMap_41CF0_42030(segment, position);
			ResetLife_36DE0_371A0(segment);
		}
		previous = segment;
	}
	AddToMap_41CF0_42030(head, position);
	ResetLife_36DE0_371A0(head);
	SetSprite_36FA0_37360(head, headSprite);
	return head;
}

Type_Mc1Entity* CreateClass05Model00_38030_383B0(Type_Mc1Position* position)
{
	if (GetFreeEntityCount_37710_37AD0() < 16)
		return nullptr;
	return CreateLongCreature(position, 0, 1, 0x50, 12, 0x13, 0x28, false);
}

Type_Mc1Entity* CreateClass05Model01_38270_385F0(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewEntity(5, 1, 7);
	if (!entity)
		return nullptr;
	NewCreature(entity, 100, 0x10);
	entity->maxLife_0x08_8 = 2000;
	entity->word_0x7E_126 = (int16_t)(entity->word_0x80_128 / 2);
	SetHalfMana_36F90_37350(entity);
	entity->word_0x22_34 = 0;
	entity->word_0x1E_30 = 0;
	entity->word_0x20_32 = 0;
	InitCreature(entity, 13, 0, false);
	entity->byte_0x3A_58 = (uint8_t)(entity->typeParams_0x9C_156[0x1A] + 1);
	entity->byte_0x42_66 = 3;
	AddToMap_41CF0_42030(entity, position);
	ResetLife_36DE0_371A0(entity);
	SetSprite_36FA0_37360(entity, 0x56);
	return entity;
}

Type_Mc1Entity* CreateClass05Model02_38370_386F0(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewEntity(5, 2, 0x0D);
	if (!entity)
		return nullptr;
	NewCreature(entity, 0x46, 0x1E);
	entity->maxLife_0x08_8 = 3000;
	entity->word_0x7E_126 = (int16_t)(entity->word_0x80_128 / 2);
	SetHalfMana_36F90_37350(entity);
	SetRandomYaw(entity);
	InitCreature(entity, 14, 0x15E, true);
	entity->byte_0x42_66 = 3;
	entity->byte_0x43_67 = 0;
	entity->byte_0x3A_58 = SpreadByIndex(entity);
	AddToMap_41CF0_42030(entity, position);
	ResetLife_36DE0_371A0(entity);
	SetSprite_36FA0_37360(entity, 3);
	SetBox_37130_374F0(entity, 0x80, 0x80);
	return entity;
}

Type_Mc1Entity* CreateClass05Model03_384B0_38830(Type_Mc1Position* position)
{
	return CreateLongCreature(position, 3, 0x13, 0x40, 15, 0x59, 0x58, true);
}

// the creatures with a random yaw and a box of 80h
static Type_Mc1Entity* CreateSimpleCreature(Type_Mc1Position* position, uint8_t model, uint8_t action, int16_t speed, int16_t word82,
	int32_t maxLife, uint8_t typeIndex, int16_t word2C, int16_t sprite)
{
	Type_Mc1Entity* entity = NewEntity(5, model, action);
	if (!entity)
		return nullptr;
	NewCreature(entity, speed, word82);
	entity->maxLife_0x08_8 = maxLife;
	entity->word_0x7E_126 = entity->word_0x80_128;
	SetHalfMana_36F90_37350(entity);
	SetRandomYaw(entity);
	InitCreature(entity, typeIndex, word2C, true);
	entity->byte_0x3A_58 = 0x40;
	entity->byte_0x42_66 = 3;
	AddToMap_41CF0_42030(entity, position);
	ResetLife_36DE0_371A0(entity);
	SetSprite_36FA0_37360(entity, sprite);
	SetBox_37130_374F0(entity, 0x80, 0x80);
	return entity;
}

Type_Mc1Entity* CreateClass05Model04_386E0_38A60(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewEntity(5, 4, 0x19);
	if (!entity)
		return nullptr;
	NewCreature(entity, 0x1E, 0);
	entity->maxLife_0x08_8 = 1000;
	entity->word_0x7E_126 = entity->word_0x80_128;
	SetHalfMana_36F90_37350(entity);
	SetRandomYaw(entity);
	InitCreature(entity, 16, 0x1F4, true);
	entity->byte_0x42_66 = 3;
	entity->byte_0x3A_58 = SpreadByIndex(entity);
	AddToMap_41CF0_42030(entity, position);
	ResetLife_36DE0_371A0(entity);
	SetSprite_36FA0_37360(entity, 0);
	SetBox_37130_374F0(entity, 0x80, 0x80);
	return entity;
}

Type_Mc1Entity* CreateClass05Model05_38890_38C10(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = AllocEntity_372C0_37680();
	if (!entity)
		return nullptr;
	NewCreature(entity, 0x1E, 3);
	entity->word_0x7E_126 = entity->word_0x80_128;
	entity->actionIndex_0x46_70 = 0x1F;
	entity->class_0x40_64 = 5;
	entity->model_0x41_65 = 5;
	entity->maxLife_0x08_8 = 5000;
	SetRandomYaw(entity);
	entity->mana_0x8C_140 = 0x1F4;
	entity->dword_0x88_136 = 0x2EE0;
	InitCreature(entity, 17, 0x1F4, true);
	entity->byte_0x42_66 = 3;
	entity->byte_0x3A_58 = SpreadByIndex(entity);
	AddToMap_41CF0_42030(entity, position);
	ResetLife_36DE0_371A0(entity);
	SetSprite_36FA0_37360(entity, 0xB9);
	SetBox_37130_374F0(entity, 0x80, 0x80);
	entity->life_0x0C_12 = 5000;
	return entity;
}

Type_Mc1Entity* CreateClass05Model06_389E0_38D60(Type_Mc1Position* position)
{
	// a head and 2 segments
	if (GetFreeEntityCount_37710_37AD0() < 16)
		return nullptr;
	Type_Mc1Entity* head = NewEntity(5, 6, 0x25);
	if (!head)
		return nullptr;
	NewCreature(head, 0x50, 0x10);
	head->word_0x7E_126 = 0x1E;
	head->maxLife_0x08_8 = 9000;
	SetHalfMana_36F90_37350(head);
	head->dword_0x88_136 = head->mana_0x8C_140;
	head->mana_0x8C_140 = head->mana_0x8C_140 / 3;
	SetRandomYaw(head);
	InitCreature(head, 18, 0, false);
	head->word_0x38_56 = 0x60;
	head->byte_0x3A_58 = 0x40;
	head->byte_0x42_66 = 3;
	Type_Mc1Entity* previous = head;
	for (int32_t i = 0; i <= 1; i++)
	{
		Type_Mc1Entity* segment = AllocEntity_372C0_37680();
		if (segment)
		{
			*segment = *head;
			segment->word_0x34_52 = (uint16_t)EntityIndex(previous);
			if (previous) // the original writes to the address 36h
				previous->word_0x36_54 = (uint16_t)EntityIndex(segment);
			segment->word_0x36_54 = 0;
			segment->actionIndex_0x46_70 = 0x78;
			segment->mana_0x8C_140 = head->dword_0x88_136 / 3;
			segment->index_0x3F_63 = (uint8_t)i;
			SetSprite_36FA0_37360(segment, i ? 0xC1 : 0x32);
			segment->word_0x38_56 = (int16_t)(segment->box_0x4E_78[1] << 2);
			AddToMap_41CF0_42030(segment, position);
			ResetLife_36DE0_371A0(segment);
		}
		previous = segment;
	}
	AddToMap_41CF0_42030(head, position);
	ResetLife_36DE0_371A0(head);
	SetSprite_36FA0_37360(head, 0x31);
	return head;
}

static void SetCreature7Kind_38C00_38F80(Type_Mc1Entity* entity)
{
	// the odd and the even ones differ
	if (entity->index_0x3F_63 % 2)
	{
		SetSprite_36FA0_37360(entity, 0x55);
		entity->byte_0x47_71 = 1;
		entity->maxLife_0x08_8 = 4000;
	}
	else
	{
		SetSprite_36FA0_37360(entity, 0xC7);
		entity->byte_0x47_71 = 2;
		entity->maxLife_0x08_8 = 2000;
	}
	SetHalfMana_36F90_37350(entity);
	entity->life_0x0C_12 = entity->maxLife_0x08_8;
}

Type_Mc1Entity* CreateClass05Model07_38C60_38FE0(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewEntity(5, 7, 0x2B);
	if (!entity)
		return nullptr;
	NewCreature(entity, 0x14, 3);
	entity->word_0x7E_126 = entity->word_0x80_128;
	SetRandomYaw(entity);
	InitCreature(entity, 19, 0x1F4, true);
	entity->byte_0x3A_58 = 0x40;
	entity->byte_0x42_66 = 3;
	AddToMap_41CF0_42030(entity, position);
	SetCreature7Kind_38C00_38F80(entity);
	SetBox_37130_374F0(entity, 0x80, 0x80);
	return entity;
}

Type_Mc1Entity* CreateClass05Model08_38D60_390E0(Type_Mc1Position* position)
{
	return CreateSimpleCreature(position, 8, 0x31, 0x28, 0x14, 10000, 20, 0x3E8, 0x2F);
}

Type_Mc1Entity* CreateClass05Model09_38E70_391F0(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewEntity(5, 9, 0x36);
	if (!entity)
		return nullptr;
	NewCreature(entity, 0x14, 0);
	entity->maxLife_0x08_8 = 1000;
	entity->word_0x7E_126 = entity->word_0x80_128;
	SetHalfMana_36F90_37350(entity);
	int16_t yaw = (int16_t)(NextRand(entity) % 0x832 - 1);
	entity->word_0x24_36 = 0;
	entity->word_0x2C_44 = 0x1F4;
	entity->mailMask_0x1C_28 = 1;
	entity->word_0x22_34 = yaw;
	entity->word_0x1E_30 = yaw;
	entity->word_0x20_32 = yaw;
	CountModel(entity);
	entity->typeParams_0x9C_156 = creatureTypes_98F38_98F38[21];
	entity->byte_0x3A_58 = SpreadByIndex(entity);
	entity->byte_0x42_66 = 3;
	entity->word_0x1A_26 = (int16_t)(EntityIndex(entity) % 10 + 0x1D);
	AddToMap_41CF0_42030(entity, position);
	entity->position_0x48_72.z = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&entity->position_0x48_72);
	ResetLife_36DE0_371A0(entity);
	SetSprite_36FA0_37360(entity, 0xDC);
	SetBox_37130_374F0(entity, 0x80, 0x80);
	return entity;
}

Type_Mc1Entity* CreateClass05Model10_38FC0_39340(Type_Mc1Position* position)
{
	return CreateSimpleCreature(position, 10, 0x3D, 0x3C, 0x14, 2000, 22, 0x1F4, 0xD0);
}

Type_Mc1Entity* CreateClass05Model11_390D0_39450(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewEntity(5, 11, 0x42);
	if (!entity)
		return nullptr;
	NewCreature(entity, 0x3C, 0x14);
	entity->maxLife_0x08_8 = 20000;
	entity->word_0x7E_126 = entity->word_0x80_128;
	SetHalfMana_36F90_37350(entity);
	entity->dword_0x88_136 = entity->mana_0x8C_140 * 2;
	SetRandomYaw(entity);
	InitCreature(entity, 23, 0x1F4, true);
	entity->byte_0x3A_58 = 0x40;
	entity->byte_0x42_66 = 3;
	entity->word_0x1A_26 = 0;
	AddToMap_41CF0_42030(entity, position);
	ResetLife_36DE0_371A0(entity);
	SetSprite_36FA0_37360(entity, 0xC8);
	SetBox_37130_374F0(entity, 0x80, 0x80);
	return entity;
}

// the models 12-14: no mana, word_0x1A 2
static Type_Mc1Entity* CreateManaLessCreature(Type_Mc1Position* position, uint8_t model, uint8_t action)
{
	Type_Mc1Entity* entity = NewEntity(5, model, action);
	if (!entity)
		return nullptr;
	NewCreature(entity, 0x28, 0x14);
	entity->word_0x7E_126 = entity->word_0x80_128;
	SetRandomYaw(entity);
	entity->maxLife_0x08_8 = 1000;
	entity->mana_0x8C_140 = 0;
	InitCreature(entity, 10, 0x1F4, true);
	entity->byte_0x3A_58 = 0x40;
	entity->byte_0x42_66 = 3;
	entity->word_0x1A_26 = 2;
	AddToMap_41CF0_42030(entity, position);
	ResetLife_36DE0_371A0(entity);
	return entity;
}

Type_Mc1Entity* CreateClass05Model12_39200_39580(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = CreateManaLessCreature(position, 12, 0x49);
	if (!entity)
		return nullptr;
	SetSprite_36FA0_37360(entity, 0xDD);
	SetBox_37130_374F0(entity, 0x80, 0x80);
	return entity;
}

Type_Mc1Entity* CreateClass05Model13_39340_396C0(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = CreateManaLessCreature(position, 13, 0x4F);
	if (!entity)
		return nullptr;
	SetSprite_36FA0_37360(entity, NextRand(entity) % 7 < 4 ? 0xD9 : 0xDA);
	SetBox_37130_374F0(entity, 0x80, 0x80);
	return entity;
}

Type_Mc1Entity* CreateClass05Model14_39490_39810(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = CreateManaLessCreature(position, 14, 0x55);
	if (!entity)
		return nullptr;
	SetSprite_36FA0_37360(entity, 0xDB);
	SetBox_37130_374F0(entity, 0x80, 0x80);
	return entity;
}

Type_Mc1Entity* CreateClass05Model15_395B0_39930(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewEntity(5, 15, 0x5B);
	if (!entity)
		return nullptr;
	NewCreature(entity, 0x1E, 0);
	entity->maxLife_0x08_8 = 1000;
	entity->word_0x22_34 = 0;
	entity->word_0x7E_126 = entity->word_0x80_128;
	entity->word_0x1E_30 = 0;
	entity->word_0x20_32 = 0;
	entity->mana_0x8C_140 = 0;
	InitCreature(entity, 24, 0x1F4, true);
	entity->byte_0x42_66 = 3;
	entity->flags_0x10_16 |= 0x20000;
	entity->byte_0x3A_58 = SpreadByIndex(entity);
	AddToMap_41CF0_42030(entity, position);
	ResetLife_36DE0_371A0(entity);
	SetSprite_36FA0_37360(entity, 0);
	SetBox_37130_374F0(entity, 0x80, 0x80);
	return entity;
}

Type_Mc1Entity* CreateClass05Model16_396E0_39A60(Type_Mc1Position* position)
{
	Type_Mc1Entity* entity = NewEntity(5, 16, 0x61);
	if (!entity)
		return nullptr;
	NewCreature(entity, 0x3C, 0x14);
	entity->maxLife_0x08_8 = 100000;
	entity->word_0x7E_126 = entity->word_0x80_128;
	SetHalfMana_36F90_37350(entity);
	SetRandomYaw(entity);
	InitCreature(entity, 25, 0x1F4, true);
	entity->byte_0x3A_58 = 0x40;
	entity->byte_0x42_66 = 3;
	entity->word_0x1A_26 = 0;
	AddToMap_41CF0_42030(entity, position);
	ResetLife_36DE0_371A0(entity);
	SetSprite_36FA0_37360(entity, 0xCF);
	SetBox_37130_374F0(entity, 0x80, 0x80);
	return entity;
}

// class 11: the model is the action, the position is copied (not on the map)

Type_Mc1Entity* InitClass11_3BB20_3BE60(Type_Mc1Position* position, uint8_t model, uint8_t action)
{
	Type_Mc1Entity* entity = AllocEntity_372C0_37680();
	if (!entity)
		return nullptr;
	entity->class_0x40_64 = 0x0B;
	entity->model_0x41_65 = model;
	entity->actionIndex_0x46_70 = action;
	entity->flags_0x10_16 &= ~9u;
	entity->word_0x1A_26 = 0;
	entity->flags_0x10_16 |= 1;
	entity->position_0x48_72 = *position;
	ResetLife_36DE0_371A0(entity);
	return entity;
}

#define CLASS11(n, mc1, hw) \
	Type_Mc1Entity* CreateClass11Model##n##_##mc1##_##hw(Type_Mc1Position* position) \
	{ \
		return InitClass11_3BB20_3BE60(position, n, n); \
	}

CLASS11(0, 3BB70, 3BEB0)
CLASS11(1, 3BB90, 3BED0)
CLASS11(2, 3BBB0, 3BEF0)
CLASS11(3, 3BBD0, 3BF10)
CLASS11(4, 3BBF0, 3BF30)
CLASS11(5, 3BC10, 3BF50)
CLASS11(6, 3BC30, 3BF70)
CLASS11(7, 3BC50, 3BF90)
CLASS11(8, 3BC70, 3BFB0)
CLASS11(9, 3BC90, 3BFD0)
CLASS11(10, 3BCB0, 3BFF0)
CLASS11(11, 3BCD0, 3C010)
CLASS11(12, 3BCF0, 3C030)
CLASS11(13, 3BD10, 3C050)
CLASS11(14, 3BD30, 3C070)
CLASS11(15, 3BD50, 3C090)
CLASS11(16, 3BD70, 3C0B0)
CLASS11(17, 3BD90, 3C0D0)
CLASS11(18, 3BDB0, 3C0F0)
CLASS11(19, 3BDD0, 3C110)
CLASS11(20, 3BDF0, 3C130)
CLASS11(21, 3BE10, 3C150)
CLASS11(22, 3BE30, 3C170)
CLASS11(23, 3BE50, 3C190)
CLASS11(24, 3BE70, 3C1B0)
CLASS11(25, 3BE90, 3C1D0)
CLASS11(26, 3BEB0, 3C1F0)
CLASS11(27, 3BED0, 3C210)
CLASS11(28, 3BEF0, 3C230)
CLASS11(29, 3BF10, 3C250)
CLASS11(30, 3BF30, 3C270)
CLASS11(31, 3BF50, 3C290)

// class 12: mana = value / divisor, the sprite 4Dh with a 4 times bigger box

Type_Mc1Entity* InitClass12_3BF70_3C2B0(Type_Mc1Position* position, uint8_t model, uint8_t action, int32_t value, int32_t divisor,
	int32_t byte3C, uint8_t byte3E, int32_t dword84, int32_t word2C)
{
	Type_Mc1Entity* entity = AllocEntity_372C0_37680();
	if (!entity)
		return nullptr;
	entity->class_0x40_64 = 0x0C;
	entity->model_0x41_65 = model;
	entity->actionIndex_0x46_70 = action;
	entity->word_0x2C_44 = (int16_t)word2C;
	entity->word_0x32_50 = (int16_t)divisor;
	entity->byte_0x3E_62 = byte3E;
	if (byte3E)
		byte3C = 0;
	entity->byte_0x3C_60 = (uint8_t)byte3C;
	entity->byte_0x3D_61 = 0;
	entity->maxLife_0x08_8 = 0;
	entity->life_0x0C_12 = 0;
	entity->mana_0x8C_140 = value / entity->word_0x32_50;
	entity->dword_0x84_132 = dword84;
	entity->dword_0x88_136 = value;
	entity->flags_0x10_16 &= ~8u;
	AddToMap_41CF0_42030(entity, position);
	SetSprite_36FA0_37360(entity, 0x4D);
	SetBox_37130_374F0(entity, (int16_t)(entity->box_0x4E_78[1] << 2), (int16_t)(entity->box_0x4E_78[3] << 2));
	ResetLife_36DE0_371A0(entity);
	return entity;
}

#define CLASS12(n, mc1, hw, action, value, divisor, byte3C, byte3E, dword84, word2C) \
	Type_Mc1Entity* CreateClass12Model##n##_##mc1##_##hw(Type_Mc1Position* position) \
	{ \
		return InitClass12_3BF70_3C2B0(position, n, action, value, divisor, byte3C, byte3E, dword84, word2C); \
	}

CLASS12(0, 3C090, 3C3D0, 0, 0xC8, 5, 1, 0, 0, 0x7D)
CLASS12(1, 3C0F0, 3C430, 3, 0x3E8, 0x15, 1, 0, 0, 0x64)
CLASS12(2, 3C0C0, 3C400, 6, 0x3E8, 0xFB, 0, 0, 0, 0x64)
CLASS12(3, 3C040, 3C380, 9, 0x32, 3, 1, 0, 0, 0x64)
CLASS12(4, 3C1B0, 3C4F0, 0x0C, 0x7D0, 0xFB, 1, 0, 0, 0x64)
CLASS12(5, 3C330, 3C670, 0x0F, 0xBB8, 0x65, 1, 0, 0, 0x64)
CLASS12(6, 3C150, 3C490, 0x12, 0x1770, 0x33, 1, 0, 0x1D4C0, 0x1770)
CLASS12(7, 3C1E0, 3C520, 0x15, 0x2710, 0x0B, 1, 0, 0x186A0, 0x2710)
CLASS12(8, 3C390, 3C6D0, 0x18, 0x7530, 0x41, 1, 0, 0x2BF20, 0x3E8)
CLASS12(9, 3C300, 3C640, 0x1B, 0x2EE0, 0x1F, 1, 0, 0x186A0, 0x1770)
CLASS12(10, 3C120, 3C460, 0x1E, 0x1388, 0x33, 1, 0, 0x2710, 0x64)
CLASS12(11, 3C270, 3C5B0, 0x21, 0x9C4, 0x11, 1, 0, 0x3E80, 0x64)
CLASS12(12, 3C2D0, 3C610, 0x24, 0x1388, 0xFB, 1, 0, 0xC350, 0x64)
CLASS12(13, 3C2A0, 3C5E0, 0x27, 0x1F4, 0x0B, 1, 0, 0x4E20, 0x64)
CLASS12(14, 3C210, 3C550, 0x2A, 0x3E8, 0x65, 1, 0, 0x1F40, 0x64)
CLASS12(15, 3C240, 3C580, 0x2D, 0x3E8, 2, 0, 0, 0x61A8, 0x1F4)
CLASS12(16, 3C060, 3C3A0, 0x30, 0x3E8, 0x65, 1, 0, 0, 0x2710)
CLASS12(17, 3C3C0, 3C700, 0x33, 0x32C8, 0x0D, 1, 0, 0x249F0, 0x64)
CLASS12(18, 3C360, 3C6A0, 0x36, 0x4E20, 0x21, 1, 0, 0x15F90, 0x7D0)
CLASS12(19, 3C180, 3C4C0, 0x39, 0xFA0, 0x11, 1, 0, 0x2710, 0x64)
CLASS12(20, 3C3F0, 3C730, 0x3C, 0x1388, 0x33, 1, 0, 0x2EE0, 0x15F90)
CLASS12(21, 3C420, 3C760, 0x3F, 0x3E8, 0xFB, 0, 0, 0, 0x64)
CLASS12(22, 3C450, 3C790, 0x42, 0x124F8, 0x65, 1, 0, 0x30D40, 0x1B58)
CLASS12(23, 3C480, 3C7C0, 0x45, 0x258, 3, 0, 0, 0xC350, 0x32)
