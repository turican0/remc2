#include "Mc1Creatures.h"

#include <cstring>

#include "Mc1Castle.h"
#include "Mc1Entity.h"
#include "Mc1Files.h"
#include "Mc1Game.h"
#include "Mc1NotPorted.h"
#include "Mc1Shared.h"
#include "Mc1Terrain.h"
#include "Mc1Wizard.h"

// the creatures (class 5): the movement, the damage and the actions of the models

static Type_Mc1Entity* Entities()
{
	return mc1State_AE400_AE3F0->entities_0x7463_29795;
}

static uint16_t EntityIndex(const Type_Mc1Entity* entity)
{
	return (uint16_t)(entity - Entities());
}

static Type_Mc1Entity** Lists()
{
	return mc1Settings_AE408_AE3F8->entityLists_0x8E1A_36378;
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

static uint32_t ReadU32(const uint8_t* data)
{
	uint32_t value;
	memcpy(&value, data, sizeof(value));
	return value;
}

static uint32_t NextRand(Type_Mc1Entity* entity)
{
	entity->rand_0x04_4 = entity->rand_0x04_4 * 0x24A1 + 0x24DF;
	return entity->rand_0x04_4;
}

static void WriteS16(uint8_t* data, int16_t value)
{
	memcpy(data, &value, sizeof(value));
}

static uint32_t Distance2DSquared(const Type_Mc1Entity* entity, const Type_Mc1Entity* other)
{
	const int32_t dx = (int16_t)(other->position_0x48_72.x - entity->position_0x48_72.x);
	const int32_t dy = (int16_t)(other->position_0x48_72.y - entity->position_0x48_72.y);
	return (uint32_t)(dx * dx + dy * dy);
}

uint32_t SetAction_424F0_42830(Type_Mc1Entity* entity, uint8_t action)
{
	entity->actionIndex_0x46_70 = action;
	return action;
}

int32_t ClampHeight_42000_42340(Type_Mc1Position* position, int32_t ground, int32_t bottom, int32_t top, int32_t climb)
{
	int32_t result = 0;
	const int32_t z = position->z;
	if (z > top + ground)
	{
		position->z = (int16_t)(position->z + climb);
		result = 1;
	}
	else if (z > ground + bottom)
	{
		position->z = (int16_t)(z + climb * 25 / 100);
		result = 1;
	}
	if (position->z < ground + bottom)
	{
		position->z = (int16_t)(ground + bottom);
		result = 1;
	}
	return result;
}

int32_t GetTileSlope_19650_19650(const Type_Mc1Position* position)
{
	const uint8_t x = (uint8_t)(position->x >> 8);
	const uint8_t y = (uint8_t)(position->y >> 8);
	const int32_t h00 = mapHeightmap_DC1E0_DC1D0[(y << 8) | x];
	const int32_t h01 = mapHeightmap_DC1E0_DC1D0[(y << 8) | (uint8_t)(x + 1)];
	const int32_t h11 = mapHeightmap_DC1E0_DC1D0[((uint8_t)(y + 1) << 8) | (uint8_t)(x + 1)];
	const int32_t h10 = mapHeightmap_DC1E0_DC1D0[((uint8_t)(y + 1) << 8) | x];
	int32_t slopeX = h00 + h10 - h01 - h11;
	int32_t slopeY = h00 + h01 - h10 - h11;
	if (slopeX < 0)
		slopeX = -slopeX;
	if (slopeY < 0)
		slopeY = -slopeY;
	return slopeX > slopeY ? slopeX : slopeY;
}

int32_t CheckPosition_11640_11640(Type_Mc1Entity* entity, Type_Mc1Position* position, uint8_t checks)
{
	// 2 the height over the ground, 1 the kind of the tile, 4 the pitch to the position
	const uint8_t* params = entity->typeParams_0x9C_156;
	if (checks & 2)
	{
		if (position->z < (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)position) + ReadS16(params + 0x0C))
			return 1;
		if (position->z > (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)position) + ReadS16(params + 0x0A))
			return 1;
	}
	if (checks & 1)
	{
		const uint32_t tile = GetTileTypeBit_11810_11810(position) & ~ReadU32(params + 0x14);
		if (tile)
			return (int32_t)tile;
	}
	if (checks & 4)
	{
		const uint16_t pitch = GetPitchAngle_42180_424C0(&entity->position_0x48_72, position);
		const int16_t direction = (int16_t)AngleDirection_42240_42580(0, pitch);
		if (direction == 1)
		{
			if ((uint16_t)AngleDistance_42210_42550(0, pitch) > ReadS16(params + 0x12))
				return 1;
		}
		else if (direction == -1)
		{
			if ((uint16_t)AngleDistance_42210_42550(0, pitch) > ReadS16(params + 0x10))
				return 1;
		}
	}
	return 0;
}

// a step of the creature from its position to featurePosition_AE454 (196E0)
static void CreatureStep(Type_Mc1Entity* entity)
{
	const uint8_t* params = entity->typeParams_0x9C_156;
	featurePosition_AE454_AE444 = entity->position_0x48_72;
	const int16_t ground = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&featurePosition_AE454_AE444);
	ClampHeight_42000_42340(&featurePosition_AE454_AE444, ground, ReadS16(params + 0x0C), ReadS16(params + 0x0A), ReadS16(params + 0x0E));
	MoveEntity_41EC0_42200((axis_3d*)&featurePosition_AE454_AE444, (uint16_t)entity->word_0x1E_30, 0, entity->word_0x7E_126);
}

static bool CreatureStepBlocked(Type_Mc1Entity* entity)
{
	return CheckPosition_11640_11640(entity, &featurePosition_AE454_AE444, 1)
		|| GetTileSlope_19650_19650(&featurePosition_AE454_AE444) >= ReadS16(entity->typeParams_0x9C_156 + 0x10);
}

static int32_t CreatureStepDone(Type_Mc1Entity* entity)
{
	const uint8_t* params = entity->typeParams_0x9C_156;
	CopyEntityPosition_41C70_41FB0(entity, &featurePosition_AE454_AE444);
	const int32_t turn = TurnStep_422A0_425E0((uint16_t)entity->word_0x1E_30, (uint16_t)entity->word_0x22_34, ReadU16(params + 4), ReadU16(params + 2));
	entity->word_0x1E_30 = (int16_t)((entity->word_0x1E_30 + turn) & 0x7FF);
	return 1;
}

int32_t MoveCreature_196E0_196E0(Type_Mc1Entity* entity)
{
	// forward, then turned by 155h both ways and back, else the creature dies
	CreatureStep(entity);
	if (entity->position_0x48_72.x >> 8 == featurePosition_AE454_AE444.x >> 8 && entity->position_0x48_72.y >> 8 == featurePosition_AE454_AE444.y >> 8)
		return CreatureStepDone(entity);
	if (!CreatureStepBlocked(entity))
		return CreatureStepDone(entity);
	const int16_t yaw = entity->word_0x1E_30;
	static const int16_t turns[3] = { 0x155, -0x155, 0x400 };
	for (int i = 0; i < 3; i++)
	{
		entity->word_0x1E_30 = (int16_t)((yaw + turns[i]) & 0x7FF);
		CreatureStep(entity);
		if (!CreatureStepBlocked(entity))
			return CreatureStepDone(entity);
	}
	entity->life_0x0C_12 = -1;
	return 1;
}

// the mail of the damage and the damage of the linked segments: 0 none, 1 hit, 2 dead
static uint32_t CreatureDamage(Type_Mc1Entity* entity)
{
	uint32_t result = 0;
	if (entity->byte_0x3A_58)
	{
		Type_Mc1Mail& mail = entity->mails_0x5A_90[0];
		if (mail.sender_4)
		{
			entity->life_0x0C_12 -= mail.value_0;
			entity->word_0x28_40 = mail.sender_4;
			mail.sender_4 = 0;
			result = 1;
		}
		else
			entity->word_0x28_40 = 0;
		if (entity->word_0x36_54)
		{
			for (Type_Mc1Entity* next = &Entities()[entity->word_0x36_54]; next != Entities(); next = &Entities()[next->word_0x36_54])
			{
				if (next->life_0x0C_12 < entity->life_0x0C_12)
				{
					entity->life_0x0C_12 = next->life_0x0C_12;
					entity->word_0x28_40 = next->word_0x28_40;
					result = 1;
					break;
				}
			}
		}
	}
	if (entity->life_0x0C_12 < 0)
	{
		result = 2;
		entity->word_0x26_38 = entity->word_0x28_40;
	}
	return result;
}

// the wander of the creature: the wanted yaw turned by 55h-154h to a random side
static void CreatureWanderTurn(Type_Mc1Entity* entity)
{
	entity->rand_0x04_4 = entity->rand_0x04_4 * 0x24A1 + 0x24DF;
	const int32_t side = (int32_t)(entity->rand_0x04_4 % 0x9D) / 0x4F * 2 - 1;
	entity->rand_0x04_4 = entity->rand_0x04_4 * 0x24A1 + 0x24DF;
	entity->word_0x22_34 = (int16_t)(((uint16_t)entity->word_0x22_34 + side * (int32_t)((entity->rand_0x04_4 & 0xFF) + 0x55)) & 0x7FF);
}

static bool CreatureUpdateFrame(Type_Mc1Entity* entity)
{
	return !((uint8_t)entity->index_0x3F_63 % ReadS16(entity->typeParams_0x9C_156 + 0x1A));
}

// the nearest in the list within the range and the view angle of the creature
static Type_Mc1Entity* NearestInView(Type_Mc1Entity* entity, Type_Mc1Entity* list, bool wizards)
{
	const uint8_t* params = entity->typeParams_0x9C_156;
	const uint32_t range = (uint32_t)(ReadS16(params + 0x1C) * ReadS16(params + 0x1C));
	uint32_t best = 0xFFFFFFFF;
	Type_Mc1Entity* found = nullptr;
	for (Type_Mc1Entity* other = list; other > Entities(); other = other->next_0x00_0)
	{
		if (!wizards && (other->word_0x34_52 || other == entity))
			continue;
		const uint32_t distance = Distance2DSquared(entity, other);
		if (distance > range)
			continue;
		if (wizards && (other->flags_0x10_16 & 0x20))
			continue;
		const int32_t angle = ReadS16(params + 0x1E);
		const uint16_t yaw = (uint16_t)GetYawAngle_42150_42490((axis_3d*)&entity->position_0x48_72, (axis_3d*)&other->position_0x48_72);
		if ((int32_t)(uint16_t)AngleDistance_42210_42550((uint16_t)entity->word_0x1E_30, yaw) >= angle)
			continue;
		if (distance >= best)
			continue;
		found = other;
		best = distance;
	}
	return found;
}

void CreatureFollow_19550_19550(Type_Mc1Entity* entity)
{
	// the segment follows the previous one
	Type_Mc1Entity* leader = &Entities()[entity->word_0x34_52];
	if (leader->class_0x40_64 != 5)
		MarkToFree_41E80_421C0(entity);
	if (entity->byte_0x3A_58)
	{
		entity->word_0x1E_30 = (int16_t)GetYawAngle_42150_42490((axis_3d*)&entity->position_0x48_72, (axis_3d*)&leader->position_0x48_72);
		entity->word_0x20_32 = (int16_t)GetPitchAngle_42180_424C0(&entity->position_0x48_72, &leader->position_0x48_72);
		featurePosition_AE454_AE444 = leader->position_0x48_72;
		MoveEntity_41EC0_42200((axis_3d*)&featurePosition_AE454_AE444, (uint16_t)entity->word_0x1E_30, (uint16_t)entity->word_0x20_32, (int16_t)-entity->word_0x38_56);
		CopyEntityPosition_41C70_41FB0(entity, &featurePosition_AE454_AE444);
		Type_Mc1Mail& mail = entity->mails_0x5A_90[0];
		if (mail.sender_4)
		{
			entity->life_0x0C_12 -= mail.value_0;
			entity->word_0x28_40 = mail.sender_4;
			mail.sender_4 = 0;
		}
		else
			entity->word_0x28_40 = 0;
	}
	else if (!(entity->index_0x3F_63 & 3))
	{
		CopyEntityPosition_41C70_41FB0(entity, &leader->position_0x48_72);
		entity->word_0x1E_30 = leader->word_0x1E_30;
	}
}

void CreatureWander_19D70_19D70(Type_Mc1Entity* entity, uint8_t action)
{
	// action + 2 attack the wizard (hit by it or seen), + 3 follow a creature of the model, + 4 dead
	switch (CreatureDamage(entity))
	{
	case 0:
		break;
	case 1:
		if (Entities()[entity->word_0x28_40].class_0x40_64 == 3)
		{
			entity->target_0x92_146 = entity->word_0x28_40;
			SetAction_424F0_42830(entity, (uint8_t)(action + 2));
		}
		return;
	case 2:
		SetAction_424F0_42830(entity, (uint8_t)(action + 4));
		return;
	default:
		return;
	}
	MoveCreature_196E0_196E0(entity);
	if (!CreatureUpdateFrame(entity))
		return;
	CreatureWanderTurn(entity);
	if (!entity->byte_0x3A_58)
		return;
	if (Type_Mc1Entity* wizard = NearestInView(entity, Lists()[21], true))
	{
		entity->target_0x92_146 = EntityIndex(wizard);
		SetAction_424F0_42830(entity, (uint8_t)(action + 2));
		return;
	}
	if (Type_Mc1Entity* leader = NearestInView(entity, Lists()[1 + (int8_t)entity->model_0x41_65], false))
	{
		entity->word_0x34_52 = EntityIndex(leader);
		SetAction_424F0_42830(entity, (uint8_t)(action + 3));
	}
}

void CreatureWanderMana_1B200_1B200(Type_Mc1Entity* entity)
{
	// wanders, the nearest mana ball (model 28h) in the range is the target
	CreatureWander_19D70_19D70(entity, 6);
	const uint8_t* params = entity->typeParams_0x9C_156;
	if ((uint8_t)entity->index_0x3F_63 % ReadS16(params + 0x1A))
		return;
	const uint32_t range = (uint32_t)(ReadS16(params + 0x1C) * ReadS16(params + 0x1C));
	uint32_t best = 0xFFFFFFFF;
	Type_Mc1Entity* found = nullptr;
	for (Type_Mc1Entity* mana = Lists()[22]; mana > Entities(); mana = mana->next_0x00_0)
	{
		if (mana->model_0x41_65 != 0x28)
			continue;
		const uint32_t distance = Distance2DSquared(entity, mana);
		if (distance > range || distance >= best)
			continue;
		found = mana;
		best = distance;
	}
	if (found)
	{
		entity->target_0x92_146 = EntityIndex(found);
		SetAction_424F0_42830(entity, 6);
	}
}

void CreatureWander12_1B510_1B510(Type_Mc1Entity* entity)
{
	CreatureWander_19D70_19D70(entity, 0x12);
}

// a wizard (model 0 or 1, not checked by the class) that hit the creature gets 200 at 210h of its player data
static void HitByWizard(Type_Mc1Entity* entity)
{
	Type_Mc1Entity* attacker = &Entities()[entity->word_0x28_40];
	if (attacker->model_0x41_65 != 0 && attacker->model_0x41_65 != 1)
		return;
	if (attacker->playerData_0xA0_160) // port: the original writes to 210h of a null player data
		WriteS16(attacker->playerData_0xA0_160 + 0x210, 0xC8);
}

void CreatureWander48_1EED0_1EED0(Type_Mc1Entity* entity)
{
	const uint8_t action = 0x48;
	switch (CreatureDamage(entity))
	{
	case 0:
		break;
	case 1:
		HitByWizard(entity);
		return;
	case 2:
		SetAction_424F0_42830(entity, (uint8_t)(action + 4));
		return;
	default:
		return;
	}
	MoveCreature_196E0_196E0(entity);
	if (!CreatureUpdateFrame(entity))
		return;
	CreatureWanderTurn(entity);
	// a count down in word_0x1A, at 0 the action + 3
	const int16_t count = entity->word_0x1A_26;
	entity->word_0x1A_26 = (int16_t)(count - 1);
	if (count)
		return;
	entity->word_0x1A_26 = 1;
	SetAction_424F0_42830(entity, (uint8_t)(action + 3));
}

// the creature goes to a building (class 10 model 2Dh) and in (1F640 / 1FAC0)
static void CreatureToBuilding(Type_Mc1Entity* entity, uint8_t action, bool farOnly)
{
	const uint8_t enter = (uint8_t)(action + 4);
	switch (CreatureDamage(entity))
	{
	case 0:
		break;
	case 1:
		HitByWizard(entity);
		return;
	case 2:
		SetAction_424F0_42830(entity, enter);
		return;
	default:
		return;
	}
	MoveCreature_196E0_196E0(entity);
	if (!CreatureUpdateFrame(entity))
		return;
	if (!entity->target_0x92_146)
	{
		CreatureWanderTurn(entity);
		if (!CreatureUpdateFrame(entity))
			return;
		uint32_t best = 0xFFFFFFFF;
		Type_Mc1Entity* found = nullptr;
		for (Type_Mc1Entity* building = Lists()[23]; building > Entities(); building = building->next_0x00_0)
		{
			const uint32_t distance = Distance2DSquared(entity, building);
			if (distance >= best)
				continue;
			if (farOnly && distance <= 0x0E100000)
				continue;
			found = building;
			best = distance;
		}
		if (found)
		{
			entity->target_0x92_146 = EntityIndex(found);
			entity->word_0x7E_126 = entity->word_0x80_128;
		}
		return;
	}
	Type_Mc1Entity* building = &Entities()[entity->target_0x92_146];
	if (building->class_0x40_64 == 0x0A && building->model_0x41_65 == 0x2D)
	{
		if (Distance3D_42340_42680(&entity->position_0x48_72, &building->position_0x48_72) > 0x800)
		{
			entity->word_0x22_34 = (int16_t)GetYawAngle_42150_42490((axis_3d*)&entity->position_0x48_72, (axis_3d*)&building->position_0x48_72);
			return;
		}
		if (building->word_0x80_128 > building->word_0x1A_26)
		{
			SetAction_424F0_42830(entity, enter);
			entity->word_0x1A_26 = 1;
			building->word_0x1A_26++;
			return;
		}
	}
	entity->target_0x92_146 = 0;
	entity->word_0x7E_126 = entity->word_0x82_130;
}

void CreatureToBuilding_1F640_1F640(Type_Mc1Entity* entity)
{
	CreatureToBuilding(entity, 0x4E, false);
}

void CreatureToFarBuilding_1FAC0_1FAC0(Type_Mc1Entity* entity)
{
	CreatureToBuilding(entity, 0x54, true);
}

void SendMail_12B50_12B50(Type_Mc1Entity* from, Type_Mc1Entity* to, uint8_t kind, uint16_t value)
{
	// an unread mail of the kind is replaced, a read one gets the value added
	Type_Mc1Mail& mail = to->mails_0x5A_90[kind];
	if (mail.sender_4)
		mail.value_0 = value;
	else
		mail.value_0 += value;
	mail.sender_4 = from->id_0x18_24;
}

void CreatureFollowLeader_1A390_1A390(Type_Mc1Entity* entity, uint8_t action)
{
	// action + 1 alone, + 2 attack the wizard with the leader, + 4 dead
	if (!entity->word_0x34_52)
	{
		SetAction_424F0_42830(entity, (uint8_t)(action + 1));
		return;
	}
	Type_Mc1Entity* leader = &Entities()[entity->word_0x34_52];
	const uint8_t attack = (uint8_t)(action + 2);
	switch (CreatureDamage(entity))
	{
	case 0:
		break;
	case 1:
		if (Entities()[entity->word_0x28_40].class_0x40_64 == 3)
		{
			leader->word_0x34_52 = 0;
			leader->target_0x92_146 = entity->word_0x28_40;
			SetAction_424F0_42830(leader, attack);
			entity->word_0x34_52 = 0;
			entity->target_0x92_146 = entity->word_0x28_40;
			SetAction_424F0_42830(entity, attack);
		}
		return;
	case 2:
		leader->target_0x92_146 = entity->word_0x28_40;
		leader->word_0x34_52 = 0;
		SetAction_424F0_42830(leader, attack);
		SetAction_424F0_42830(entity, (uint8_t)(action + 4));
		return;
	default:
		return;
	}
	MoveCreature_196E0_196E0(entity);
	if (!CreatureUpdateFrame(entity))
		return;
	switch ((uint32_t)((int8_t)leader->actionIndex_0x46_70 - action))
	{
	case 3: // the leader follows: its leader
		entity->word_0x34_52 = leader->word_0x34_52;
		[[fallthrough]];
	case 0:
	case 1:
	{
		Type_Mc1Entity* target = &Entities()[entity->word_0x34_52];
		entity->word_0x22_34 = (int16_t)GetYawAngle_42150_42490((axis_3d*)&entity->position_0x48_72, (axis_3d*)&target->position_0x48_72);
		// away from the first other creature of the model closer than a tile
		for (Type_Mc1Entity* other = Lists()[1 + (int8_t)entity->model_0x41_65]; other > Entities(); other = other->next_0x00_0)
		{
			if (other->id_0x18_24 == entity->id_0x18_24)
				continue;
			int32_t dx = entity->position_0x48_72.x - other->position_0x48_72.x;
			if ((dx < 0 ? -dx : dx) >= 0x100)
				continue;
			int32_t dy = entity->position_0x48_72.y - other->position_0x48_72.y;
			if ((dy < 0 ? -dy : dy) >= 0x100)
				continue;
			entity->word_0x22_34 = (int16_t)GetYawAngle_42150_42490((axis_3d*)&other->position_0x48_72, (axis_3d*)&entity->position_0x48_72);
			break;
		}
		entity->word_0x7E_126 = (int16_t)(leader->word_0x7E_126 + leader->word_0x82_130);
		return;
	}
	case 2: // the leader attacks
		entity->target_0x92_146 = leader->target_0x92_146;
		entity->word_0x34_52 = 0;
		SetAction_424F0_42830(entity, attack);
		return;
	default:
		entity->word_0x34_52 = 0;
		SetAction_424F0_42830(entity, (uint8_t)(action + 1));
		return;
	}
}

int32_t CreatureAttack_1A120_1A120(Type_Mc1Entity* entity, uint8_t action, int32_t (*attack)(Type_Mc1Entity* entity, Type_Mc1Entity* target))
{
	// goes to the target, attacks it in the range; action + 1 the target lost, + 4 dead
	switch (CreatureDamage(entity))
	{
	case 0:
		break;
	case 1:
		if (Entities()[entity->word_0x28_40].class_0x40_64 == 3)
			entity->target_0x92_146 = entity->word_0x28_40;
		return 0;
	case 2:
		SetAction_424F0_42830(entity, (uint8_t)(action + 4));
		return 0;
	default:
		return 0;
	}
	MoveCreature_196E0_196E0(entity);
	Type_Mc1Entity* target = &Entities()[entity->target_0x92_146];
	if (!(entity->index_0x3F_63 & 3))
		entity->word_0x22_34 = (int16_t)GetYawAngle_42150_42490((axis_3d*)&entity->position_0x48_72, (axis_3d*)&target->position_0x48_72);
	if (target->life_0x0C_12 < 0 || (target->flags_0x10_16 & 0x400))
	{
		SetAction_424F0_42830(entity, (uint8_t)(action + 1));
		return 0;
	}
	const uint8_t* params = entity->typeParams_0x9C_156;
	if ((uint8_t)entity->index_0x3F_63 % ReadS16(params + 0x1A))
		return 0;
	if (Distance3D_42340_42680(&entity->position_0x48_72, &target->position_0x48_72) >= (uint32_t)(int32_t)ReadS16(params + 0x1C))
	{
		SetAction_424F0_42830(entity, (uint8_t)(action + 1));
		return 0;
	}
	return (int16_t)attack(entity, target) ? 1 : 0;
}

int32_t CreatureBite_1AB10_1AB10(Type_Mc1Entity* entity, Type_Mc1Entity* target)
{
	if ((int32_t)Distance3D_42340_42680(&entity->position_0x48_72, &target->position_0x48_72) >= 0x400)
		return 0;
	SendMail_12B50_12B50(entity, target, 0, (uint16_t)entity->word_0x2C_44);
	return 1;
}

void CreatureFollowLeader6_1B320_1B320(Type_Mc1Entity* entity)
{
	CreatureFollowLeader_1A390_1A390(entity, 6);
}

void CreatureFollowLeader12_1B570_1B570(Type_Mc1Entity* entity)
{
	CreatureFollowLeader_1A390_1A390(entity, 0x12);
}

void CreatureAttack6_1B2D0_1B2D0(Type_Mc1Entity* entity)
{
	if (CreatureAttack_1A120_1A120(entity, 6, CreatureBite_1AB10_1AB10))
		sub_55370_558A0((int16_t)EntityIndex(entity), -1, 7);
}

void CreatureFindBuilding_1F390_1F390(Type_Mc1Entity* entity)
{
	// the nearest building: action + 2 go to it, else + 1
	const uint8_t action = 0x48;
	switch (CreatureDamage(entity))
	{
	case 0:
		break;
	case 1:
		HitByWizard(entity);
		return;
	case 2:
		SetAction_424F0_42830(entity, (uint8_t)(action + 4));
		return;
	default:
		return;
	}
	uint32_t best = 0xFFFFFFFF;
	Type_Mc1Entity* found = nullptr;
	for (Type_Mc1Entity* building = Lists()[23]; building > Entities(); building = building->next_0x00_0)
	{
		const uint32_t distance = Distance3D_42340_42680(&entity->position_0x48_72, &building->position_0x48_72);
		if (distance && distance < best)
		{
			found = building;
			best = distance;
		}
	}
	if (found)
	{
		entity->target_0x92_146 = EntityIndex(found);
		entity->word_0x1A_26 = 10;
		SetAction_424F0_42830(entity, (uint8_t)(action + 2));
	}
	else
	{
		entity->word_0x1A_26 = 5;
		SetAction_424F0_42830(entity, (uint8_t)(action + 1));
	}
}

void CreatureGoToBuilding_1F120_1F120(Type_Mc1Entity* entity)
{
	// to the target building: at it the action, after word_0x1A steps or without it the action + 1
	const uint8_t action = 0x48;
	switch (CreatureDamage(entity))
	{
	case 0:
		break;
	case 1:
		HitByWizard(entity);
		return;
	case 2:
		SetAction_424F0_42830(entity, (uint8_t)(action + 4));
		return;
	default:
		return;
	}
	MoveCreature_196E0_196E0(entity);
	if (((uint8_t)entity->index_0x3F_63 % ReadS16(entity->typeParams_0x9C_156 + 0x1A)) / 2)
		return;
	Type_Mc1Entity* building = &Entities()[entity->target_0x92_146];
	const int16_t steps = entity->word_0x1A_26;
	entity->word_0x1A_26 = (int16_t)(steps - 1);
	if (!steps || !building->class_0x40_64)
	{
		entity->word_0x1A_26 = 5;
		SetAction_424F0_42830(entity, (uint8_t)(action + 1));
	}
	entity->word_0x22_34 = (int16_t)GetYawAngle_42150_42490((axis_3d*)&entity->position_0x48_72, (axis_3d*)&building->position_0x48_72);
	if (Distance3D_42340_42680(&entity->position_0x48_72, &building->position_0x48_72) < 0xA00)
	{
		entity->word_0x1A_26 = 0;
		SetAction_424F0_42830(entity, action);
	}
}

int32_t CornerHeightRange_35EA0_36260(uint8_t x, uint8_t y, uint8_t height, uint8_t width)
{
	// the highest minus the lowest of the corners of the area
	const uint8_t xs[4] = { x, (uint8_t)(x + width), (uint8_t)(x + width), x };
	const uint8_t ys[4] = { y, y, (uint8_t)(y + height), (uint8_t)(y + height) };
	int32_t high = mapHeightmap_DC1E0_DC1D0[(y << 8) | x];
	int32_t low = high;
	for (int i = 1; i < 4; i++)
	{
		const int32_t corner = mapHeightmap_DC1E0_DC1D0[(ys[i] << 8) | xs[i]];
		if (corner > high)
			high = corner;
		else if (corner < low)
			low = corner;
	}
	return high - low;
}

uint8_t BuildingPlaceFlat_1E920_1E920(const Type_Mc1Position* position, uint16_t width, uint16_t height, uint16_t limit)
{
	// the corner of the building as InitBuildingEntity_36DF0 places it, the ground flat enough
	uint8_t x = (uint8_t)((uint8_t)(position->x >> 8) - (width >> 1));
	const uint8_t y = (uint8_t)((uint8_t)(position->y >> 8) - (height >> 1));
	if ((x + y) % 2)
		x++;
	return (int16_t)CornerHeightRange_35EA0_36260(x, y, (uint8_t)height, (uint8_t)width) < limit;
}

void BuildingSize_1E9B0_1E9B0(uint16_t sprite, uint16_t* width, uint16_t* height)
{
	// the half size of the sprite of the building in the world + 300h
	uint16_t spriteHeight = buildIndex[sprite].height_5;
	uint16_t spriteWidth = buildIndex[sprite].width_4;
	if (*videoMode_12F02E_12F01E == 1)
	{
		spriteHeight >>= 1;
		spriteWidth >>= 1;
	}
	*height = (uint16_t)((spriteHeight << 8) / 2 + 0x300);
	*width = (uint16_t)((spriteWidth << 8) / 2 + 0x300);
}

// a building or a wizard model 2 in the area (the distances to the half sizes of both)
static bool AreaTaken(Type_Mc1Entity* list, bool wizards, int16_t x, int16_t y, uint16_t width, uint16_t height)
{
	for (Type_Mc1Entity* other = list; other > Entities(); other = other->next_0x00_0)
	{
		if (wizards && other->model_0x41_65 != 2)
			continue;
		const int32_t dx = (int16_t)(other->position_0x48_72.x - x);
		if ((dx < 0 ? -dx : dx) > width + other->box_0x4E_78[1])
			continue;
		const int32_t dy = (int16_t)(other->position_0x48_72.y - y);
		if ((dy < 0 ? -dy : dy) > height + other->box_0x4E_78[2])
			continue;
		return true;
	}
	return false;
}

void CreatureBuild_1EA40_1EA40(Type_Mc1Entity* entity)
{
	// a new building next to the target building, the side by word_0x1A; action + 1 when it cannot
	const uint8_t action = 0x48;
	Type_Mc1Entity* building = &Entities()[entity->target_0x92_146];
	if (!building->class_0x40_64 || building->model_0x41_65 != 0x2D)
		entity->word_0x1A_26 = 5;
	else
	{
		featurePosition_AE454_AE444 = building->position_0x48_72;
		const int16_t side = entity->word_0x1A_26;
		entity->word_0x1A_26 = (int16_t)(side + 1);
		if (side >= 4)
			entity->word_0x1A_26 = 1;
		else
		{
			const uint8_t sprite = (uint8_t)((NextRand(entity) & 7) + 0x19);
			uint16_t width;
			uint16_t height;
			BuildingSize_1E9B0_1E9B0(sprite, &width, &height);
			Type_Mc1Position& place = featurePosition_AE454_AE444;
			switch ((uint16_t)(entity->word_0x1A_26 - 1))
			{
			case 0: // right
			{
				const int32_t step = (int32_t)(NextRand(entity) % 3) << 8;
				place.x = (int16_t)(place.x + width + building->box_0x4E_78[1] + step + 0x100);
				place.y = (int16_t)(place.y + ((int32_t)(NextRand(entity) % 3) << 8) - 0x500);
				break;
			}
			case 1: // left
			{
				const int32_t step = (int32_t)(NextRand(entity) % 3) << 8;
				place.x = (int16_t)(place.x - (width + building->box_0x4E_78[1] + step + 0x100));
				place.y = (int16_t)(place.y + ((int32_t)(NextRand(entity) % 3) << 8) - 0x500);
				break;
			}
			case 2: // down
			{
				place.x = (int16_t)(place.x + ((int32_t)(NextRand(entity) % 3) << 8) - 0x500);
				const int32_t step = (int32_t)(NextRand(entity) % 3) << 8;
				place.y = (int16_t)(place.y + height + building->box_0x4E_78[2] + step + 0x100);
				break;
			}
			case 3: // up
			{
				place.x = (int16_t)(place.x + ((int32_t)(NextRand(entity) % 3) << 8) - 0x500);
				const int32_t step = (int32_t)(NextRand(entity) % 3) << 8;
				place.y = (int16_t)(place.y - (building->box_0x4E_78[2] + height + step + 0x100));
				break;
			}
			}
			const int16_t x = place.x;
			const int16_t y = place.y;
			if (GetTileTypeBit_11810_11810(&place) == 1)
				entity->word_0x1A_26 = 2;
			else
			{
				const uint16_t flatness = (uint16_t)((((int32_t)width >> 7) + ((int32_t)height >> 7) > 4 ? 1 : 0) + 0x0F);
				if (!BuildingPlaceFlat_1E920_1E920(&place, (uint16_t)(width >> 8), (uint16_t)(height >> 8), flatness))
					return;
				if (AreaTaken(Lists()[23], false, x, y, width, height) || AreaTaken(Lists()[21], true, x, y, width, height))
					return;
				if (Type_Mc1Entity* created = CreateEntity_373F0_377B0(&place, 0x0A, 0x2D))
				{
					sub_55370_558A0((int16_t)EntityIndex(entity), -1, 0x0A);
					InitBuildingEntity_36DF0_371B0(created, sprite);
					created->actionIndex_0x46_70 = 0x33;
				}
				entity->target_0x92_146 = 0;
				SetAction_424F0_42830(entity, 0x4F);
				return;
			}
		}
	}
	entity->target_0x92_146 = 0;
	SetAction_424F0_42830(entity, (uint8_t)(action + 1));
}

void CreatureLookForLeader_19B10_19B10(Type_Mc1Entity* entity, uint8_t action)
{
	// action + 2 attack the wizard that hit it, + 3 follow a creature of the model, + 4 dead
	switch (CreatureDamage(entity))
	{
	case 0:
		break;
	case 1:
		if (Entities()[entity->word_0x28_40].class_0x40_64 == 3)
		{
			entity->target_0x92_146 = entity->word_0x28_40;
			SetAction_424F0_42830(entity, (uint8_t)(action + 2));
		}
		return;
	case 2:
		SetAction_424F0_42830(entity, (uint8_t)(action + 4));
		return;
	default:
		return;
	}
	if (!CreatureUpdateFrame(entity))
		return;
	if (Type_Mc1Entity* leader = NearestInView(entity, Lists()[1 + (int8_t)entity->model_0x41_65], false))
	{
		entity->word_0x34_52 = EntityIndex(leader);
		SetAction_424F0_42830(entity, (uint8_t)(action + 3));
	}
}

void CreatureToTarget6_1B160_1B160(Type_Mc1Entity* entity)
{
	// to the target (the mana ball of 1B200), without it the action 7
	CreatureLookForLeader_19B10_19B10(entity, 6);
	MoveCreature_196E0_196E0(entity);
	if (entity->actionIndex_0x46_70 != 6 || !CreatureUpdateFrame(entity))
		return;
	Type_Mc1Entity* target = &Entities()[entity->target_0x92_146];
	if (target->class_0x40_64)
	{
		entity->word_0x22_34 = (int16_t)GetYawAngle_42150_42490((axis_3d*)&entity->position_0x48_72, (axis_3d*)&target->position_0x48_72);
		return;
	}
	entity->target_0x92_146 = 0;
	SetAction_424F0_42830(entity, 7);
}

void CreatureKilled_1A6C0_1A6C0(Type_Mc1Entity* entity, uint8_t action)
{
	// the creature and its segments to action + 5; a kill by the player wizard (model 0) is counted (167h)
	const uint8_t dead = (uint8_t)(action + 5);
	for (Type_Mc1Entity* segment = &Entities()[entity->word_0x36_54]; segment != Entities(); segment = &Entities()[segment->word_0x36_54])
	{
		SetAction_424F0_42830(segment, dead);
		if (segment->word_0x26_38)
			entity->word_0x26_38 = segment->word_0x26_38;
	}
	Type_Mc1Entity* killer = &Entities()[entity->word_0x26_38];
	if (entity->word_0x26_38 && killer->class_0x40_64 == 3 && killer->model_0x41_65 == 0
		&& !(entity->model_0x41_65 == 9 && entity->owner_0x90_144) && (int16_t)entity->id_0x18_24 == (int16_t)EntityIndex(entity))
	{
		const uint8_t model = entity->model_0x41_65;
		if (model != 0x0C && model != 0x0D && model != 0x0E && model != 0x0F && model != 9)
		{
			uint8_t* kills = killer->playerData_0xA0_160 + 0x167;
			int32_t count;
			memcpy(&count, kills, sizeof(count));
			count++;
			memcpy(kills, &count, sizeof(count));
		}
	}
	SetAction_424F0_42830(entity, dead);
}

void CreatureInBuilding_1FA00_1FA00(Type_Mc1Entity* entity)
{
	// in the building (word_0x1A set by 1F640): gone, else killed
	if (entity->word_0x1A_26)
	{
		MarkToFree_41E80_421C0(entity);
		return;
	}
	Type_Mc1Entity* attacker = &Entities()[entity->word_0x26_38];
	if ((attacker->model_0x41_65 == 0 || attacker->model_0x41_65 == 1) && attacker->playerData_0xA0_160) // port: null player data
		WriteS16(attacker->playerData_0xA0_160 + 0x210, 0xC8);
	CreatureKilled_1A6C0_1A6C0(entity, 0x4E);
}

int32_t CreatureFireball_1A8E0_1A8E0(Type_Mc1Entity* entity, Type_Mc1Entity* target)
{
	// a fireball (class 9 model 0) of the creature to its target
	Type_Mc1Entity* fireball = CreateEntity_373F0_377B0(&entity->position_0x48_72, 9, 0);
	if (!fireball)
		return 0;
	fireball->byte_0x44_68 = 0x0A;
	fireball->byte_0x45_69 = 0;
	fireball->id_0x18_24 = entity->id_0x18_24;
	fireball->word_0x1E_30 = (int16_t)GetYawAngle_42150_42490((axis_3d*)&entity->position_0x48_72, (axis_3d*)&target->position_0x48_72);
	fireball->word_0x20_32 = (int16_t)GetPitchAngle_42180_424C0(&entity->position_0x48_72, &target->position_0x48_72);
	fireball->position_0x48_72.z = (int16_t)(fireball->position_0x48_72.z + entity->box_0x4E_78[3]);
	fireball->target_0x92_146 = entity->target_0x92_146;
	fireball->typeParams_0x9C_156 = creatureTypes_98F38_98F38[6];
	fireball->byte_0x43_67 = entity->byte_0x43_67;
	fireball->word_0x2C_44 = 0x1F4;
	fireball->byte_0x42_66 = entity->byte_0x42_66;
	return 1;
}

void CreatureAttack12_1B520_1B520(Type_Mc1Entity* entity)
{
	if (CreatureAttack_1A120_1A120(entity, 0x12, CreatureFireball_1A8E0_1A8E0))
		sub_55370_558A0((int16_t)EntityIndex(entity), -1, 8);
}

void CreatureKilled12_1B580_1B580(Type_Mc1Entity* entity)
{
	CreatureKilled_1A6C0_1A6C0(entity, 0x12);
}

void CreatureRemains_1A800_1A800(Type_Mc1Entity* entity, uint8_t /*action*/)
{
	// the dead creature every 8th frame: its mana (5000 in the demo) as a mana ball, a smoke, gone
	if (entity->index_0x3F_63 & 7)
		return;
	if (mc1Settings_AE408_AE3F8->flags_0x00_0 & 0x200)
		entity->mana_0x8C_140 = 0x1388;
	DropMana_27690_27690(entity);
	if (Type_Mc1Entity* smoke = CreateEntity_373F0_377B0(&entity->position_0x48_72, 0x0A, 1))
		smoke->id_0x18_24 = entity->id_0x18_24;
	MarkToFree_41E80_421C0(entity);
}

void CreatureRemains12_1B590_1B590(Type_Mc1Entity* entity)
{
	CreatureRemains_1A800_1A800(entity, 0x12);
}

// the actions of the models: the shared ones with the action base of the model

void CreatureLookForLeader0_1B060_1B060(Type_Mc1Entity* entity)
{
	CreatureLookForLeader_19B10_19B10(entity, 0x0);
}

void CreatureKilled0_1B100_1B100(Type_Mc1Entity* entity)
{
	CreatureKilled_1A6C0_1A6C0(entity, 0x0);
}

void CreatureRemains0_1B110_1B110(Type_Mc1Entity* entity)
{
	CreatureRemains_1A800_1A800(entity, 0x0);
}

void CreatureKilled6_1B330_1B330(Type_Mc1Entity* entity)
{
	CreatureKilled_1A6C0_1A6C0(entity, 0x6);
}

void CreatureRemains6_1B340_1B340(Type_Mc1Entity* entity)
{
	CreatureRemains_1A800_1A800(entity, 0x6);
}

void CreatureKilledC_1B4E0_1B4E0(Type_Mc1Entity* entity)
{
	CreatureKilled_1A6C0_1A6C0(entity, 0xC);
}

void CreatureRemainsC_1B4F0_1B4F0(Type_Mc1Entity* entity)
{
	CreatureRemains_1A800_1A800(entity, 0xC);
}

void CreatureLookForLeader12_1B500_1B500(Type_Mc1Entity* entity)
{
	CreatureLookForLeader_19B10_19B10(entity, 0x12);
}

void CreatureRemains18_1BC40_1BC40(Type_Mc1Entity* entity)
{
	CreatureRemains_1A800_1A800(entity, 0x18);
}

void CreatureKilled1E_1C3C0_1C3C0(Type_Mc1Entity* entity)
{
	CreatureKilled_1A6C0_1A6C0(entity, 0x1E);
}

void CreatureRemains1E_1C3D0_1C3D0(Type_Mc1Entity* entity)
{
	CreatureRemains_1A800_1A800(entity, 0x1E);
}

void CreatureLookForLeader24_1C490_1C490(Type_Mc1Entity* entity)
{
	CreatureLookForLeader_19B10_19B10(entity, 0x24);
}

void CreatureKilled24_1C8D0_1C8D0(Type_Mc1Entity* entity)
{
	CreatureKilled_1A6C0_1A6C0(entity, 0x24);
}

void CreatureRemains24_1C8E0_1C8E0(Type_Mc1Entity* entity)
{
	CreatureRemains_1A800_1A800(entity, 0x24);
}

void CreatureLookForLeader2A_1C8F0_1C8F0(Type_Mc1Entity* entity)
{
	CreatureLookForLeader_19B10_19B10(entity, 0x2A);
}

void CreatureKilled2A_1CA20_1CA20(Type_Mc1Entity* entity)
{
	CreatureKilled_1A6C0_1A6C0(entity, 0x2A);
}

void CreatureRemains2A_1CA30_1CA30(Type_Mc1Entity* entity)
{
	CreatureRemains_1A800_1A800(entity, 0x2A);
}

void CreatureLookForLeader30_1CA40_1CA40(Type_Mc1Entity* entity)
{
	CreatureLookForLeader_19B10_19B10(entity, 0x30);
}

void CreatureFollowLeader30_1CF50_1CF50(Type_Mc1Entity* entity)
{
	CreatureFollowLeader_1A390_1A390(entity, 0x30);
}

void CreatureRemains30_1CFE0_1CFE0(Type_Mc1Entity* entity)
{
	CreatureRemains_1A800_1A800(entity, 0x30);
}

void CreatureKilled36_1DCB0_1DCB0(Type_Mc1Entity* entity)
{
	CreatureKilled_1A6C0_1A6C0(entity, 0x36);
}

void CreatureRemains36_1DCC0_1DCC0(Type_Mc1Entity* entity)
{
	CreatureRemains_1A800_1A800(entity, 0x36);
}

void CreatureLookForLeader3C_1DDD0_1DDD0(Type_Mc1Entity* entity)
{
	CreatureLookForLeader_19B10_19B10(entity, 0x3C);
}

void CreatureWander3C_1DDE0_1DDE0(Type_Mc1Entity* entity)
{
	CreatureWander_19D70_19D70(entity, 0x3C);
}

void CreatureFollowLeader3C_1DE10_1DE10(Type_Mc1Entity* entity)
{
	CreatureFollowLeader_1A390_1A390(entity, 0x3C);
}

void CreatureKilled3C_1DE20_1DE20(Type_Mc1Entity* entity)
{
	CreatureKilled_1A6C0_1A6C0(entity, 0x3C);
}

void CreatureRemains3C_1DE30_1DE30(Type_Mc1Entity* entity)
{
	CreatureRemains_1A800_1A800(entity, 0x3C);
}

void CreatureFollowLeader42_1E6F0_1E6F0(Type_Mc1Entity* entity)
{
	CreatureFollowLeader_1A390_1A390(entity, 0x42);
}

void CreatureKilled42_1E700_1E700(Type_Mc1Entity* entity)
{
	CreatureKilled_1A6C0_1A6C0(entity, 0x42);
}

void CreatureRemains42_1E710_1E710(Type_Mc1Entity* entity)
{
	CreatureRemains_1A800_1A800(entity, 0x42);
}

void CreatureKilled48_1F5A0_1F5A0(Type_Mc1Entity* entity)
{
	CreatureKilled_1A6C0_1A6C0(entity, 0x48);
}

void CreatureRemains4E_1FAA0_1FAA0(Type_Mc1Entity* entity)
{
	CreatureRemains_1A800_1A800(entity, 0x4E);
}

void CreatureFollowLeader5A_203E0_203E0(Type_Mc1Entity* entity)
{
	CreatureFollowLeader_1A390_1A390(entity, 0x5A);
}

void CreatureKilled5A_203F0_203F0(Type_Mc1Entity* entity)
{
	CreatureKilled_1A6C0_1A6C0(entity, 0x5A);
}

void CreatureRemains5A_20400_20400(Type_Mc1Entity* entity)
{
	CreatureRemains_1A800_1A800(entity, 0x5A);
}

void CreatureLookForLeader60_20700_20700(Type_Mc1Entity* entity)
{
	CreatureLookForLeader_19B10_19B10(entity, 0x60);
}

void CreatureFollowLeader60_20B60_20B60(Type_Mc1Entity* entity)
{
	CreatureFollowLeader_1A390_1A390(entity, 0x60);
}

void CreatureKilled60_20B70_20B70(Type_Mc1Entity* entity)
{
	CreatureKilled_1A6C0_1A6C0(entity, 0x60);
}

void CreatureRemains60_20B80_20B80(Type_Mc1Entity* entity)
{
	CreatureRemains_1A800_1A800(entity, 0x60);
}

void CreatureBob_1B120_1B120(Type_Mc1Entity* entity)
{
	// the flying creature (model 0) bobs: up by word_0x1A, falling by 5, again 96h under 100h over the ground
	entity->position_0x48_72.z = (int16_t)(entity->position_0x48_72.z + entity->word_0x1A_26);
	const int16_t ground = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&entity->position_0x48_72);
	entity->word_0x1A_26 = (int16_t)(entity->word_0x1A_26 - 5);
	if (entity->position_0x48_72.z < ground + 0x100)
		entity->word_0x1A_26 = 0x96;
}

void CreatureWanderBob0_1B070_1B070(Type_Mc1Entity* entity)
{
	CreatureWander_19D70_19D70(entity, 0);
	CreatureBob_1B120_1B120(entity);
}

void CreatureFollowLeaderBob0_1B0E0_1B0E0(Type_Mc1Entity* entity)
{
	CreatureFollowLeader_1A390_1A390(entity, 0);
	CreatureBob_1B120_1B120(entity);
}

void CreatureLookForLeaderC_1B350_1B350(Type_Mc1Entity* entity)
{
	CreatureLookForLeader_19B10_19B10(entity, 0x0C);
	if (entity->actionIndex_0x46_70 == 0x0E)
		entity->word_0x1A_26 = 1;
}

// in a building (word_0x1A set) gone, else killed
static void CreatureGoneOrKilled(Type_Mc1Entity* entity, uint8_t action)
{
	if (entity->word_0x1A_26)
		MarkToFree_41E80_421C0(entity);
	else
		CreatureKilled_1A6C0_1A6C0(entity, action);
}

void CreatureGoneOrKilled18_1BC10_1BC10(Type_Mc1Entity* entity)
{
	CreatureGoneOrKilled(entity, 0x18);
}

void CreatureGoneOrKilled54_1FE90_1FE90(Type_Mc1Entity* entity)
{
	CreatureGoneOrKilled(entity, 0x54);
}

void CreatureStopAtTarget_1BC50_1BC50(Type_Mc1Entity* entity)
{
	// stopped, one of two sprites, the kind of the target as the kind it hits (byte_0x42 / byte_0x43)
	const uint32_t roll = NextRand(entity) % 0x14;
	entity->word_0x7E_126 = 0;
	SetSprite_36FA0_37360(entity, roll > 10 ? 1 : 0xCE);
	const Type_Mc1Entity& target = Entities()[entity->target_0x92_146];
	entity->byte_0x42_66 = target.class_0x40_64;
	entity->byte_0x43_67 = target.model_0x41_65;
}

void CreatureLookForLeader18_1B5A0_1B5A0(Type_Mc1Entity* entity)
{
	CreatureLookForLeader_19B10_19B10(entity, 0x18);
	if (entity->actionIndex_0x46_70 == 0x1A)
		CreatureStopAtTarget_1BC50_1BC50(entity);
}

void CreatureFollowLeader18_1BBE0_1BBE0(Type_Mc1Entity* entity)
{
	CreatureFollowLeader_1A390_1A390(entity, 0x18);
	if (entity->actionIndex_0x46_70 == 0x1A)
		CreatureStopAtTarget_1BC50_1BC50(entity);
}

void GuardStop_20410_20410(Type_Mc1Entity* entity)
{
	// stopped, one of two sprites
	const uint32_t roll = NextRand(entity) % 0x14;
	entity->word_0x7E_126 = 0;
	SetSprite_36FA0_37360(entity, roll > 10 ? 1 : 0xCE);
}

void GuardWalk_20480_20480(Type_Mc1Entity* entity)
{
	// the guard of a castle walks the tiles: every 8th frame the best free one of the 4 directions (random weights 1F40h
	// 7000 / 10), every 16th frame to the middle of the tile across, away from a guard on the same tile
	static const uint16_t weights_1FF40_1FF40[4] = { 0x1B58, 0x1B58, 0x0A, 0x1B58 };
	const uint8_t* params = entity->typeParams_0x9C_156;
	if (!((uint8_t)entity->index_0x3F_63 % 8))
	{
		if (GetTileTypeBit_11810_11810(&entity->position_0x48_72) & ~ReadU32(params + 0x14))
		{
			entity->actionIndex_0x46_70 = 0x5E;
			return;
		}
		uint32_t best = 1;
		uint16_t yaw = (uint16_t)entity->word_0x1E_30;
		for (int i = 0; i < 4; i++)
		{
			featurePosition_AE454_AE444 = entity->position_0x48_72;
			MoveEntity_41EC0_42200((axis_3d*)&featurePosition_AE454_AE444, yaw, 0, 0x100);
			const uint32_t roll = NextRand(entity) % weights_1FF40_1FF40[i];
			const uint32_t free = CheckPosition_11640_11640(entity, &featurePosition_AE454_AE444, 1) ? 0 : 1;
			const uint32_t score = free * (roll + 2);
			if ((uint16_t)score > (uint16_t)best)
			{
				best = score;
				entity->word_0x1E_30 = (int16_t)yaw;
			}
			yaw = (uint16_t)((yaw + 0x200) & 0x7FF);
		}
	}
	featurePosition_AE454_AE444 = entity->position_0x48_72;
	if (!((uint8_t)entity->index_0x3F_63 % 0x10))
	{
		if ((((int32_t)(uint16_t)entity->word_0x1E_30 - 0x100) >> 9) & 1)
			featurePosition_AE454_AE444.x = (int16_t)(((featurePosition_AE454_AE444.x >> 8) << 8) + 0x80);
		else
			featurePosition_AE454_AE444.y = (int16_t)(((featurePosition_AE454_AE444.y >> 8) << 8) + 0x80);
	}
	for (Type_Mc1Entity* other = Lists()[1 + (int8_t)entity->model_0x41_65]; other > Entities(); other = other->next_0x00_0)
	{
		if (other->id_0x18_24 == entity->id_0x18_24)
			continue;
		const int32_t dx = entity->position_0x48_72.x - other->position_0x48_72.x;
		if ((dx < 0 ? -dx : dx) >= 0x100)
			continue;
		const int32_t dy = entity->position_0x48_72.y - other->position_0x48_72.y;
		if ((dy < 0 ? -dy : dy) >= 0x100)
			continue;
		entity->word_0x22_34 = (int16_t)GetYawAngle_42150_42490((axis_3d*)&other->position_0x48_72, (axis_3d*)&entity->position_0x48_72);
		break;
	}
	if (entity->word_0x22_34 != entity->word_0x1E_30 && NextRand(entity) % 0x14 > 10)
		return;
	MoveEntity_41EC0_42200((axis_3d*)&featurePosition_AE454_AE444, (uint16_t)entity->word_0x1E_30, 0, entity->word_0x7E_126);
	const int16_t ground = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&featurePosition_AE454_AE444);
	ClampHeight_42000_42340(&featurePosition_AE454_AE444, ground, ReadS16(params + 0x0C), ReadS16(params + 0x0A), ReadS16(params + 0x0E));
	CopyEntityPosition_41C70_41FB0(entity, &featurePosition_AE454_AE444);
}

void GuardAction_1FF60_1FF60(Type_Mc1Entity* entity)
{
	// the guard (action 5Ah + 1): walks, attacks a wizard (not of its castle) that hit it or that it sees (5Ch), dead 5Eh
	switch (CreatureDamage(entity))
	{
	case 0:
	{
		GuardWalk_20480_20480(entity);
		if (!CreatureUpdateFrame(entity) || !entity->byte_0x3A_58)
			break;
		const uint8_t* params = entity->typeParams_0x9C_156;
		const uint32_t range = (uint32_t)(ReadS16(params + 0x1C) * ReadS16(params + 0x1C));
		uint32_t best = 0xFFFFFFFF;
		Type_Mc1Entity* found = nullptr;
		for (Type_Mc1Entity* wizard = Lists()[21]; wizard > Entities(); wizard = wizard->next_0x00_0)
		{
			if (wizard->id_0x18_24 == entity->id_0x18_24)
				continue;
			const uint32_t distance = Distance2DSquared(entity, wizard);
			if (distance > range || (wizard->flags_0x10_16 & 0x20))
				continue;
			const uint16_t yaw = (uint16_t)GetYawAngle_42150_42490((axis_3d*)&entity->position_0x48_72, (axis_3d*)&wizard->position_0x48_72);
			if ((int32_t)(uint16_t)AngleDistance_42210_42550((uint16_t)entity->word_0x1E_30, yaw) >= ReadS16(params + 0x1E) || distance >= best)
				continue;
			found = wizard;
			best = distance;
		}
		if (found)
		{
			entity->target_0x92_146 = EntityIndex(found);
			SetAction_424F0_42830(entity, 0x5C);
		}
		break;
	}
	case 1:
	{
		const Type_Mc1Entity& attacker = Entities()[entity->word_0x28_40];
		if (attacker.class_0x40_64 == 3 && attacker.id_0x18_24 != entity->id_0x18_24)
		{
			entity->target_0x92_146 = entity->word_0x28_40;
			SetAction_424F0_42830(entity, 0x5C);
		}
		break;
	}
	case 2:
		SetAction_424F0_42830(entity, 0x5E);
		break;
	}
	if (entity->actionIndex_0x46_70 == 0x5C)
		GuardStop_20410_20410(entity);
}

void GuardResume_20450_20450(Type_Mc1Entity* entity)
{
	entity->word_0x7E_126 = entity->word_0x80_128;
	SetSprite_36FA0_37360(entity, 0);
}

void GuardAttack_201D0_201D0(Type_Mc1Entity* entity)
{
	// the guard shoots (class 9 model 0Dh) at its target in the range, else back to walking (5Bh); dead 5Eh
	if (CreatureDamage(entity) == 2)
		entity->actionIndex_0x46_70 = 0x5E;
	else
	{
		Type_Mc1Entity* target = &Entities()[entity->target_0x92_146];
		if (!(entity->index_0x3F_63 & 3))
			entity->word_0x22_34 = (int16_t)GetYawAngle_42150_42490((axis_3d*)&entity->position_0x48_72, (axis_3d*)&target->position_0x48_72);
		const uint8_t* params = entity->typeParams_0x9C_156;
		if (target->life_0x0C_12 < 0 || (target->flags_0x10_16 & 0x400))
			SetAction_424F0_42830(entity, 0x5B);
		else if (!((uint8_t)entity->index_0x3F_63 % ReadS16(params + 0x1A)))
		{
			if (Distance3D_42340_42680(&entity->position_0x48_72, &target->position_0x48_72) >= (uint32_t)(int32_t)ReadS16(params + 0x1C))
				SetAction_424F0_42830(entity, 0x5B);
			else if (Type_Mc1Entity* shot = CreateEntity_373F0_377B0(&entity->position_0x48_72, 9, 0x0D))
			{
				shot->id_0x18_24 = entity->id_0x18_24;
				shot->word_0x1E_30 = (int16_t)GetYawAngle_42150_42490((axis_3d*)&entity->position_0x48_72, (axis_3d*)&target->position_0x48_72);
				shot->word_0x20_32 = (int16_t)GetPitchAngle_42180_424C0(&entity->position_0x48_72, &target->position_0x48_72);
				shot->position_0x48_72.z = (int16_t)(shot->position_0x48_72.z + entity->box_0x4E_78[3]);
				shot->target_0x92_146 = entity->target_0x92_146;
				shot->byte_0x43_67 = entity->byte_0x43_67;
				shot->byte_0x42_66 = entity->byte_0x42_66;
			}
		}
	}
	if (entity->actionIndex_0x46_70 != 0x5C)
		GuardResume_20450_20450(entity);
}

void CreatureRemainsHit48_1F5B0_1F5B0(Type_Mc1Entity* entity)
{
	// the killer wizard (word_0x26, model 0 or 1) gets 200 at 210h, then the remains
	Type_Mc1Entity* killer = &Entities()[entity->word_0x26_38];
	if ((killer->model_0x41_65 == 0 || killer->model_0x41_65 == 1) && killer->playerData_0xA0_160) // port: null player data
		WriteS16(killer->playerData_0xA0_160 + 0x210, 0xC8);
	CreatureRemains_1A800_1A800(entity, 0x48);
}

void CreatureModel4_1B5D0_1B5D0(Type_Mc1Entity* entity)
{
	// the creature from a building (action 18h + 1): wanders; attacks a wizard that hit creatures lately (210h) or the
	// nearest creature of the model 9, else follows one of its model; with a building as the target it goes in
	const uint8_t action = 0x18;
	entity->word_0x1A_26 = 0;
	switch (CreatureDamage(entity))
	{
	case 0:
	{
		MoveCreature_196E0_196E0(entity);
		if (!CreatureUpdateFrame(entity))
			break;
		if (entity->target_0x92_146)
		{
			Type_Mc1Entity* building = &Entities()[entity->target_0x92_146];
			if (building->class_0x40_64 != 0x0A || building->model_0x41_65 != 0x2D)
			{
				entity->target_0x92_146 = 0;
				break;
			}
			if (Distance3D_42340_42680(&entity->position_0x48_72, &building->position_0x48_72) > 0x1000)
				entity->word_0x22_34 = (int16_t)GetYawAngle_42150_42490((axis_3d*)&entity->position_0x48_72, (axis_3d*)&building->position_0x48_72);
			else
			{
				SetAction_424F0_42830(entity, (uint8_t)(action + 4));
				entity->word_0x1A_26 = 1;
				building->word_0x1A_26++;
			}
			break;
		}
		CreatureWanderTurn(entity);
		const uint8_t* params = entity->typeParams_0x9C_156;
		if ((uint8_t)entity->index_0x3F_63 % (ReadS16(params + 0x1A) * 4))
			break;
		Type_Mc1Entity* found = NearestInView(entity, Lists()[21], true);
		if (found && !((found->model_0x41_65 == 0 || found->model_0x41_65 == 1) && ReadS16(found->playerData_0xA0_160 + 0x210)))
			found = nullptr;
		if (!found)
		{
			const uint32_t range = (uint32_t)(ReadS16(params + 0x1C) * ReadS16(params + 0x1C));
			uint32_t best = 0xFFFFFFFF;
			for (Type_Mc1Entity* other = Lists()[10]; other > Entities(); other = other->next_0x00_0)
			{
				const uint32_t distance = Distance2DSquared(entity, other);
				if (distance > range || distance >= best)
					continue;
				found = other;
				best = distance;
			}
		}
		if (found)
		{
			if (found->class_0x40_64 == 0x0A && found->model_0x41_65 == 0x2D)
				break;
			entity->target_0x92_146 = EntityIndex(found);
			SetAction_424F0_42830(entity, (uint8_t)(action + 2));
			break;
		}
		if (Type_Mc1Entity* leader = NearestInView(entity, Lists()[1 + (int8_t)entity->model_0x41_65], false))
		{
			entity->word_0x34_52 = EntityIndex(leader);
			SetAction_424F0_42830(entity, (uint8_t)(action + 3));
		}
		break;
	}
	case 1:
		if (Entities()[entity->word_0x28_40].class_0x40_64 == 3)
		{
			entity->target_0x92_146 = entity->word_0x28_40;
			SetAction_424F0_42830(entity, (uint8_t)(action + 2));
		}
		break;
	case 2:
		SetAction_424F0_42830(entity, (uint8_t)(action + 4));
		break;
	}
	if (entity->actionIndex_0x46_70 == 0x1A)
		CreatureStopAtTarget_1BC50_1BC50(entity);
}

int32_t CreatureShot_1A990_1A990(Type_Mc1Entity* entity, Type_Mc1Entity* target)
{
	// a shot (class 9 model 0Dh, the damage FAh) of the creature to its target
	Type_Mc1Entity* shot = CreateEntity_373F0_377B0(&entity->position_0x48_72, 9, 0x0D);
	if (!shot)
		return 0;
	shot->id_0x18_24 = entity->id_0x18_24;
	shot->word_0x1E_30 = (int16_t)GetYawAngle_42150_42490((axis_3d*)&entity->position_0x48_72, (axis_3d*)&target->position_0x48_72);
	shot->word_0x20_32 = (int16_t)GetPitchAngle_42180_424C0(&entity->position_0x48_72, &target->position_0x48_72);
	shot->position_0x48_72.z = (int16_t)(shot->position_0x48_72.z + entity->box_0x4E_78[3]);
	shot->target_0x92_146 = entity->target_0x92_146;
	shot->byte_0x43_67 = entity->byte_0x43_67;
	shot->word_0x2C_44 = 0xFA;
	shot->byte_0x42_66 = entity->byte_0x42_66;
	SetSpriteDoubleBox_370A0_37460(shot, 0xC3);
	return 1;
}

void CreatureResume_1BCE0_1BCE0(Type_Mc1Entity* entity)
{
	entity->word_0x7E_126 = entity->word_0x80_128;
	SetSprite_36FA0_37360(entity, 0);
	entity->byte_0x42_66 = 3;
	entity->byte_0x43_67 = 0xFF;
}

void CreatureAttack18_1BB20_1BB20(Type_Mc1Entity* entity)
{
	// the model 4 shoots at its target; a wizard target gets 200 at 210h
	CreatureAttack_1A120_1A120(entity, 0x18, CreatureShot_1A990_1A990);
	if (entity->actionIndex_0x46_70 != 0x1A)
	{
		CreatureResume_1BCE0_1BCE0(entity);
		return;
	}
	if (!CreatureUpdateFrame(entity))
		return;
	Type_Mc1Entity* target = &Entities()[entity->target_0x92_146];
	if ((target->model_0x41_65 == 0 || target->model_0x41_65 == 1) && target->playerData_0xA0_160) // port: null player data
		WriteS16(target->playerData_0xA0_160 + 0x210, 0xC8);
}
