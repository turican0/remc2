#include "Mc1World.h"

#include <cstring>

#include "Mc1Entity.h"
#include "Mc1EntityTables.h"
#include "Mc1Game.h"
#include "Mc1Level.h"
#include "Mc1NotPorted.h"
#include "Mc1Shared.h"
#include "Mc1Screen.h"
#include "Mc1Sound.h"

// MC1 game loop: the frames and the tick of the world (the actions of the entities), the same in HW

static Type_Mc1Entity* Entities()
{
	return mc1State_AE400_AE3F0->entities_0x7463_29795;
}

static bool InList(const Type_Mc1Entity* entity)
{
	// the lists end with the address of entity 0
	return entity && entity > Entities();
}

static Type_Mc1Player_340B& CurrentPlayer()
{
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	return state->players_0x340B_13323[state->currentPlayer_0x08_8];
}

static uint16_t ReadU16(const uint8_t* data)
{
	uint16_t value;
	memcpy(&value, data, sizeof(value));
	return value;
}

static int16_t ReadS16(const uint8_t* data)
{
	int16_t value;
	memcpy(&value, data, sizeof(value));
	return value;
}

static void WriteU16(uint8_t* data, uint16_t value)
{
	memcpy(data, &value, sizeof(value));
}

void GameLoop_34610_349D0()
{
	mc1Settings_AE408_AE3F8->flag_0x17_23 = 0;
	CurrentPlayer().levelResult_0x02_2 = 0;
	while (!CurrentPlayer().exitGame_0x04_4 && !(CurrentPlayer().levelResult_0x02_2 & 8))
		GameFrame_34530_348F0();
}

void GameFrame_34530_348F0()
{
	Type_Mc1Settings_AE408_AE3F8* settings = mc1Settings_AE408_AE3F8;
	if (!mc1State_AE400_AE3F0->screenSize_0x219B_8603)
		sub_34690_34A50();
	if (!(settings->flags_0x00_0 & 0x10000))
		sub_590D0_595E0();
	if (!(settings->flags_0x00_0 & 4))
		sub_17C20_17C20();
	sub_3C9D0_3CD10();
	if (!(settings->flags_0x00_0 & 0x10000))
		sub_415C0_41900();
	// the speed: 1, 4 or 16 ticks of the world in a frame
	switch (settings->speedIndex_0x96_150)
	{
	case 0:
		WorldTick_41780_41AC0();
		break;
	case 1:
		for (int i = 0; i < 4; i++)
			WorldTick_41780_41AC0();
		break;
	case 2:
		for (int i = 0; i < 16; i++)
			WorldTick_41780_41AC0();
		break;
	}
	sub_55100_55630();
	sub_20FB0_20FB0();
	// the ticks of the frame for 5A560, then the time of the frame
	settings->frameTicks_0x99_153 = timerTicks_AC5D4_AC5C4 - settings->frameTicks_0x99_153;
	sub_5A560_5AA70();
	settings->frameTicks_0x99_153 = timerTicks_AC5D4_AC5C4;
	BlendAndBlit_30B30_30B70();
}

// a list of the settings: the previous entity links to the next one, the last to entity 0
static void AddToList(Type_Mc1Entity*& last, int list, Type_Mc1Entity* entity)
{
	if (last)
		last->next_0x00_0 = entity;
	else
		mc1Settings_AE408_AE3F8->entityLists_0x8E1A_36378[list] = entity;
	entity->next_0x00_0 = Entities();
	last = entity;
}

void WorldTick_41780_41AC0()
{
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	Type_Mc1Settings_AE408_AE3F8* settings = mc1Settings_AE408_AE3F8;
	state->rand_0x04_4 = state->rand_0x04_4 * 0x24A1 + 0x24DF;
	if (settings->flags_0x00_0 & 0x10000)
		return;
	for (int i = 1; i < 1000; i++)
	{
		Type_Mc1Entity* entity = &Entities()[i];
		if (entity->class_0x40_64 && (entity->flags_0x10_16 & 0x400))
			FreeEntity_41E90_421D0(entity);
	}
	// the lists again: 1-20 the creatures by the model, 21 class 3, 22 class 10 models 27h-28h, 23 model 2Dh, 24 class 9
	Type_Mc1Entity* lastCreature[20] = {};
	memset(&settings->entityLists_0x8E1A_36378[1], 0, 20 * sizeof(Type_Mc1Entity*));
	settings->entityLists_0x8E1A_36378[22] = nullptr;
	settings->entityLists_0x8E1A_36378[23] = nullptr;
	settings->entityLists_0x8E1A_36378[21] = nullptr;
	settings->entityLists_0x8E1A_36378[24] = nullptr;
	Type_Mc1Entity* lastClass3 = nullptr;
	Type_Mc1Entity* lastClass9 = nullptr;
	Type_Mc1Entity* lastMana = nullptr;
	Type_Mc1Entity* lastBuilding = nullptr;
	for (int i = 1; i < 1000; i++)
	{
		Type_Mc1Entity* entity = &Entities()[i];
		switch (entity->class_0x40_64)
		{
		case 3:
			if (entity->life_0x0C_12 >= 0 && !(entity->flags_0x10_16 & 0x10))
				AddToList(lastClass3, 21, entity);
			break;
		case 5:
			if (entity->life_0x0C_12 >= 0 && entity->actionIndex_0x46_70 != 0x78)
			{
				int8_t model = (int8_t)entity->model_0x41_65;
				AddToList(lastCreature[model], 1 + model, entity);
			}
			break;
		case 9:
			AddToList(lastClass9, 24, entity);
			break;
		case 0x0A:
			if (entity->model_0x41_65 == 0x27 || entity->model_0x41_65 == 0x28)
				AddToList(lastMana, 22, entity);
			else if (entity->model_0x41_65 == 0x2D)
				AddToList(lastBuilding, 23, entity);
			break;
		}
	}
	if (!(settings->flags_0x00_0 & 0x10))
		UpdateCreatureAlerts_54F00_55430();
	UpdateAttackRecords_16540_16540();
	SumLevelMana_48230_48570(&Entities()[CurrentPlayer().entityIndex_0x0A_10]);
	for (int i = 1; i < 1000; i++)
	{
		Type_Mc1Entity* entity = &Entities()[i];
		if (!entity->class_0x40_64)
			continue;
		const Type_Mc1ClassAction& record = classes_96902_96902[(int8_t)entity->class_0x40_64].actions_0[(int8_t)entity->actionIndex_0x46_70];
		if ((int16_t)record.index_4 != (int16_t)(int8_t)entity->actionIndex_0x46_70)
		{
			// "STATE-ID does not match. CLASS %d, MODEL %d, ..." is formatted but not shown
			MarkToFree_41E80_421C0(entity);
			continue;
		}
		if (record.flag_10)
		{
			record.action_6(entity);
			entity->index_0x3F_63++;
		}
	}
}

void UpdateCreatureAlerts_54F00_55430()
{
	// the creatures of the lists 1-20 and the class 10 models 27h-28h
	Type_Mc1Settings_AE408_AE3F8* settings = mc1Settings_AE408_AE3F8;
	for (int16_t model = 0; model < 20; model++)
		for (Type_Mc1Entity* entity = settings->entityLists_0x8E1A_36378[1 + model]; InList(entity); entity = entity->next_0x00_0)
		{
			if (entity->life_0x0C_12 < 0)
			{
				entity->byte_0x3A_58 = 0xFA;
				entity->byte_0x3B_59 = 0;
			}
			else
				UpdateCreatureAlert_54F80_554B0(entity);
		}
	for (Type_Mc1Entity* entity = settings->entityLists_0x8E1A_36378[22]; InList(entity); entity = entity->next_0x00_0)
		UpdateCreatureAlert_54F80_554B0(entity);
}

int32_t UpdateCreatureAlert_54F80_554B0(Type_Mc1Entity* entity)
{
	// byte_0x3A counts down and is passed to the segments (word_0x36), byte_0x3B waits; near the player it starts at 10h
	if (entity->byte_0x3A_58)
	{
		entity->byte_0x3A_58--;
		for (uint16_t next = entity->word_0x36_54; next; next = Entities()[next].word_0x36_54)
			Entities()[next].byte_0x3A_58 = entity->byte_0x3A_58;
		return 0;
	}
	if (entity->byte_0x3B_59)
	{
		entity->byte_0x3B_59--;
		return 0;
	}
	Type_Mc1Entity* player = &Entities()[CurrentPlayer().entityIndex_0x0A_10];
	int32_t dy = (int16_t)(player->position_0x48_72.y - entity->position_0x48_72.y);
	int32_t dx = (int16_t)(player->position_0x48_72.x - entity->position_0x48_72.x);
	if ((int32_t)((uint32_t)(dx * dx) + (uint32_t)(dy * dy)) < 0x2400000) // wraps like the original
	{
		// the original passes edx of the distance, dy * dy
		entity->word_0x30_48 = (int16_t)IntegerSqrt_410CE_4140E(dy * dy);
		entity->byte_0x3A_58 = 0x10;
		for (uint16_t next = entity->word_0x36_54; next; next = Entities()[next].word_0x36_54)
			Entities()[next].byte_0x3A_58 = (uint8_t)(entity->byte_0x3A_58 + 2);
	}
	entity->byte_0x3B_59 = 0;
	return 0;
}

void UpdateAttackRecords_16540_16540()
{
	// the class 9 entities (once): their owner (class 3) is recorded by the colour in the player data of the target
	Type_Mc1Entity* entities = Entities();
	for (Type_Mc1Entity* entity = mc1Settings_AE408_AE3F8->entityLists_0x8E1A_36378[24]; InList(entity); entity = entity->next_0x00_0)
	{
		if (entity->flags_0x10_16 & 0x2000)
			continue;
		Type_Mc1Entity* caster = &entities[(int16_t)entity->id_0x18_24];
		if (caster <= entities || caster->class_0x40_64 != 3)
			continue;
		Type_Mc1Entity* target = &entities[entity->target_0x92_146];
		if (target <= entities)
			continue;
		entity->flags_0x10_16 |= 0x2000;
		int16_t colour = ReadS16(caster->playerData_0xA0_160 + 0x30);
		uint8_t model = entity->model_0x41_65;
		if (target->class_0x40_64 == 3)
		{
			bool big = model == 3 || model == 4 || model == 0x0B || model == 0x10;
			int32_t add = model == 0x0A ? 0 : big ? (target->model_0x41_65 == 2 ? 0x1388 : 0xBB8) : (target->model_0x41_65 == 2 ? 0x3E8 : 0x1F4);
			uint8_t* record = entities[(int16_t)target->id_0x18_24].playerData_0xA0_160 + 0x1CC + colour * 8;
			int32_t value = ReadU16(record) + add;
			if (value < 0)
				value = 0;
			if (value > 0xFFFF)
				value = 0xFFFF;
			WriteU16(record, (uint16_t)value);
			if (target->model_0x41_65 == 2)
			{
				int32_t limit = 0xC350 - ReadS16(target->playerData_0xA0_160 + 0x20A) * (caster->dword_0x88_136 / 10) / 0xFF;
				if (value > limit)
					WriteU16(record + 2, 1);
			}
		}
		else if (target->class_0x40_64 == 0x0A)
		{
			// a model 1 hitting the mana (27h) of a player
			if (model != 1 || target->model_0x41_65 != 0x27)
				continue;
			Type_Mc1Entity* owner = &entities[target->owner_0x90_144];
			if (owner <= entities || owner->class_0x40_64 != 3)
				continue;
			uint8_t* record = owner->playerData_0xA0_160 + 0x1CC + colour * 8;
			int32_t value = ReadU16(record) + target->mana_0x8C_140 / 4;
			if (value < 0)
				value = 0;
			if (value > 0xFFFF)
				value = 0xFFFF;
			WriteU16(record, (uint16_t)value);
		}
	}
}

// the camera of the player: the last record from its wizard (3C9D0 for every player in a frame)
uint8_t SetPlayerCamera_41B20_41E60(Type_Mc1Player_340B* player, Type_Mc1Entity* entity)
{
	if (mc1Settings_AE408_AE3F8->flags_0x00_0 & 0x10000)
		return 1;
	Type_Mc1PlayerRecord& record = player->records_0x24A_586[(uint16_t)(player->recordCount_0x10_16 - 1)];
	record.x_0x00_0 = entity->position_0x48_72.x;
	record.y_0x02_2 = entity->position_0x48_72.y;
	GetTerrainAlt_11F50_11F50((axis_3d*)&entity->position_0x48_72); // the result is not used
	record.z_0x04_4 = entity->position_0x48_72.z;
	record.yaw_0x06_6 = entity->word_0x1E_30;
	record.word_0x0C_12 = player->recordTemplate_0x23C_572.word_0x0C_12;
	const uint8_t* playerData = entity->playerData_0xA0_160;
	record.roll_0x0A_10 = ReadS16(playerData + 0x147);
	// the pitch: +16h / 16 times the flag 5Eh of the settings, + 149h / 2, - 16h / 8
	const int32_t tilt = ReadS16(playerData + 0x16);
	int32_t pitch = mc1Settings_AE408_AE3F8->colorIndex_0x5D_93[1] * (tilt / 16);
	pitch += ReadS16(playerData + 0x149) / 2;
	pitch -= tilt / 8;
	record.pitch_0x08_8 = (int16_t)pitch;
	return 1;
}
