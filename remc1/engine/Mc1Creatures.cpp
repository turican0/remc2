#include "Mc1Creatures.h"

#include <cstring>

#include "Mc1Entity.h"
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
