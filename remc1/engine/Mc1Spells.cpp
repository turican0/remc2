#include "Mc1Spells.h"

#include <cstring>

#include "Mc1Entity.h"
#include "Mc1EntityTables.h"
#include "Mc1Game.h"
#include "Mc1NotPorted.h"
#include "Mc1Shared.h"

// the spells of class 12 lying in the world: picked up by a wizard (55A40)

static uint8_t byte_968E4_968E4 = 0; // only cleared (55D30)

static Type_Mc1Entity* Entities()
{
	return mc1State_AE400_AE3F0->entities_0x7463_29795;
}

static int16_t EntityIndex(const Type_Mc1Entity* entity)
{
	return (int16_t)(entity - Entities());
}

static int16_t ReadS16(const uint8_t* data)
{
	int16_t value;
	memcpy(&value, data, sizeof(value));
	return value;
}

static void WriteS16(uint8_t* data, int16_t value)
{
	memcpy(data, &value, sizeof(value));
}

static int32_t ReadS32(const uint8_t* data)
{
	int32_t value;
	memcpy(&value, data, sizeof(value));
	return value;
}

static void WriteS32(uint8_t* data, int32_t value)
{
	memcpy(data, &value, sizeof(value));
}

int32_t FallToGround_42090_423D0(Type_Mc1Position* position, int32_t ground, int32_t bottom, int32_t top, int32_t fall)
{
	int32_t result = 0;
	if ((GetTileAngleBit_11760_11760((axis_3d*)position) & 1) && !ground)
		bottom = -0x300; // the water: sinks
	const int32_t z = position->z;
	if (z > top + ground)
	{
		position->z = (int16_t)(position->z + fall);
		result = 1;
	}
	else if (z > bottom + ground)
	{
		position->z = (int16_t)(z + fall * 25 / 100);
		result = 1;
	}
	if (position->z < bottom + ground)
	{
		position->z = (int16_t)(bottom + ground);
		result = 1;
	}
	if (position->z == -0x300)
		result = -1;
	return result;
}

uint32_t Distance2DSquared_42410_42750(const Type_Mc1Position* from, const Type_Mc1Position* to)
{
	const int32_t dy = (int16_t)(to->y - from->y);
	const int32_t dx = (int16_t)(to->x - from->x);
	return (uint32_t)(dx * dx + dy * dy);
}

int32_t SpellPickUp_55A40_55F70(Type_Mc1Entity* entity, int8_t spell, int8_t action)
{
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	if (entity->life_0x0C_12)
	{
		entity->life_0x0C_12--;
		if (!entity->life_0x0C_12)
		{
			MarkToFree_41E80_421C0(entity);
			return 0;
		}
	}
	const int16_t ground = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&entity->position_0x48_72);
	if (FallToGround_42090_423D0(&entity->position_0x48_72, ground, 0, 0, -0x80) == -1)
	{
		MarkToFree_41E80_421C0(entity);
		return 0;
	}
	if (entity->index_0x3F_63 & 3)
		return 0;
	Type_Mc1Entity** lists = mc1Settings_AE408_AE3F8->entityLists_0x8E1A_36378;
	for (Type_Mc1Entity* wizard = lists[21]; wizard > Entities(); wizard = wizard->next_0x00_0)
	{
		if (wizard->model_0x41_65 || wizard->life_0x0C_12 < 0)
			continue;
		uint8_t* playerData = wizard->playerData_0xA0_160;
		// the spell of the current player is shown when it already has it
		if (state->currentPlayer_0x08_8 == ReadS16(playerData + 0x30) && !(entity->flags_0x10_16 & 1)
			&& ReadS16(playerData + 0x2A4 + (int8_t)entity->model_0x41_65 * 2))
			entity->flags_0x10_16 |= 1;
		if (!EntitiesOverlap_11950_11950(wizard, entity))
			continue;
		// the computer wizards that may have the spell want it
		for (Type_Mc1Entity* other = lists[21]; other > Entities(); other = other->next_0x00_0)
		{
			if (other->model_0x41_65 != 1)
				continue;
			const int32_t model = (int8_t)entity->model_0x41_65;
			uint8_t* otherData = other->playerData_0xA0_160;
			if (!ReadS16(otherData + 0x2A4 + model * 2) && !ReadS16(otherData + 0x274 + model * 2) && otherData[0x31C + model] == 1)
				WriteS16(otherData + 0x274 + model * 2, 0xC8);
		}
		int8_t freeSlot = -1;
		bool has = false;
		for (int i = 0; i < 24; i++)
		{
			const int32_t index = ReadS32(playerData + 0x214 + i * 4);
			if (index > 0)
			{
				const Type_Mc1Entity& had = Entities()[index];
				if (had.class_0x40_64 == 0x0C && (uint8_t)spell == had.model_0x41_65)
				{
					has = true;
					break;
				}
			}
			else if (freeSlot == -1)
				freeSlot = (int8_t)i;
		}
		if (has || freeSlot == -1)
			continue;
		if (entity->flags_0x10_16 & 0x40000)
			entity->dword_0x84_132 = 0;
		sub_55370_558A0(EntityIndex(wizard), -1, 0x12);
		entity->flags_0x10_16 |= 1;
		entity->word_0x2A_42 = (uint16_t)EntityIndex(wizard);
		entity->actionIndex_0x46_70 = (uint8_t)action;
		WriteS32(playerData + 0x214 + freeSlot * 4, EntityIndex(entity));
		WriteS16(playerData + 0x3AC, freeSlot);
		for (int i = 0; i < 10; i++)
		{
			if ((int8_t)playerData[0x304 + i] == -1)
			{
				playerData[0x304 + i] = (uint8_t)freeSlot;
				break;
			}
		}
		return 1;
	}
	return 0;
}

void SpellPickUpCopy_55D30_56260(Type_Mc1Entity* entity)
{
	// picked up: a new spell lies in its place
	byte_968E4_968E4 = 0;
	if (!SpellPickUp_55A40_55F70(entity, (int8_t)entity->model_0x41_65, (int8_t)(entity->actionIndex_0x46_70 - 2)))
		return;
	Type_Mc1Entity* copy = classes_96902_96902[12].models_4[(int8_t)entity->model_0x41_65].create_6(&entity->position_0x48_72);
	if (!copy)
		return;
	copy->actionIndex_0x46_70 += 2;
	if (byte_968E4_968E4)
		copy->flags_0x10_16 |= (entity->flags_0x10_16 & 0x40000) | 1;
	if (copy->flags_0x10_16 & 0x40000)
		copy->sprite_0x56_86 = 0x118;
}

void SpellPickUp_55DB0_562E0(Type_Mc1Entity* entity)
{
	SpellPickUp_55A40_55F70(entity, (int8_t)entity->model_0x41_65, (int8_t)(entity->actionIndex_0x46_70 - 1));
}
