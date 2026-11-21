#include "Mc1Castle.h"

#include <cstring>

#include "Mc1Creatures.h"
#include "Mc1Entity.h"
#include "Mc1Files.h"
#include "Mc1Game.h"
#include "Mc1NotPorted.h"
#include "Mc1PlayerEvents.h"
#include "Mc1Scenery.h"
#include "Mc1Shared.h"
#include "Mc1Spells.h"
#include "Mc1SceneryActions.h"
#include "Mc1Terrain.h"

// the castle of a wizard (class 3 model 2): the mana in it, its size (word_0x1A), the balloons and the guards

static Type_Mc1Entity* Entities()
{
	return mc1State_AE400_AE3F0->entities_0x7463_29795;
}

static uint16_t EntityIndex(const Type_Mc1Entity* entity)
{
	return (uint16_t)(entity - Entities());
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

static void WriteU16(uint8_t* data, uint16_t value)
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

static uint32_t NextRand(Type_Mc1Entity* entity)
{
	entity->rand_0x04_4 = entity->rand_0x04_4 * 0x24A1 + 0x24DF;
	return entity->rand_0x04_4;
}

// the owner of the castle (its id is the entity of the wizard)
static Type_Mc1Entity* Owner(const Type_Mc1Entity* castle)
{
	return &Entities()[(int16_t)castle->id_0x18_24];
}

uint32_t Distance3DSquared_42390_426D0(const Type_Mc1Position* from, const Type_Mc1Position* to)
{
	const int32_t dy = (int16_t)(to->y - from->y);
	const int32_t dx = (int16_t)(to->x - from->x);
	const int32_t dz = (int16_t)(to->z - from->z);
	return (uint32_t)(dy * dy + dx * dx + dz * dz);
}

Type_Mc1Entity* NearestOwnMana_46CA0_46FE0(Type_Mc1Entity* entity, Type_Mc1Entity* except, Type_Mc1Entity* except2)
{
	// the nearest mana ball (model 27h) owned by the id of the entity, not the two given
	uint32_t best = 0xFFFFFFFF;
	Type_Mc1Entity* found = nullptr;
	for (Type_Mc1Entity* mana = mc1Settings_AE408_AE3F8->entityLists_0x8E1A_36378[22]; mana > Entities(); mana = mana->next_0x00_0)
	{
		if (mana->model_0x41_65 != 0x27 || mana->owner_0x90_144 != (uint32_t)(int32_t)(int16_t)entity->id_0x18_24 || mana == except || mana == except2)
			continue;
		const uint32_t distance = Distance3DSquared_42390_426D0(&entity->position_0x48_72, &mana->position_0x48_72);
		if (distance < best)
		{
			found = mana;
			best = distance;
		}
	}
	return found;
}

void DropMana_27690_27690(Type_Mc1Entity* entity)
{
	// the mana of the entity (a balloon) falls out as a mana ball
	if (entity->mana_0x8C_140 <= 0)
		return;
	NextRand(entity);
	if (Type_Mc1Entity* mana = CreateEntity_373F0_377B0(&entity->position_0x48_72, 0x0A, 0x27))
	{
		mana->mana_0x8C_140 = entity->mana_0x8C_140;
		mana->owner_0x90_144 = entity->owner_0x90_144;
		mana->word_0x1E_30 = (int16_t)(((uint16_t)entity->word_0x1E_30 + NextRand(mana) % 0x71 - 0x38) & 0x7FF);
		mana->word_0x7E_126 = (int16_t)(NextRand(mana) % 0x30 + 0x10);
		mana->target_0x96_150.x = 0;
		mana->target_0x96_150.y = 0;
		const int16_t ground = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&entity->position_0x48_72);
		mana->word_0x2E_46 = (int16_t)((0x400 - (entity->position_0x48_72.z - ground)) / 8);
		MoveEntity_41EC0_42200((axis_3d*)&mana->target_0x96_150, (uint16_t)mana->word_0x1E_30, 0, mana->word_0x7E_126);
	}
	entity->owner_0x90_144 = 0;
}

int32_t CastleDamage_47EC0_48200(Type_Mc1Entity* castle)
{
	// 0 none, 1 hit (the owner gets 4 at 187h), 2 destroyed; the mail 5 of the owner below the size 7: grow (flag 40h)
	int32_t result = 0;
	if (castle->life_0x0C_12 < 0)
		return 2;
	Type_Mc1Mail& mail = castle->mails_0x5A_90[0];
	if (mail.sender_4)
	{
		castle->life_0x0C_12 -= mail.value_0;
		if (castle->life_0x0C_12 < 0)
		{
			castle->word_0x26_38 = mail.sender_4;
			mail.sender_4 = 0;
			return 2;
		}
		mail.sender_4 = 0;
		mail.value_0 = 0;
		Owner(castle)->playerData_0xA0_160[0x187] = 4;
		result = 1;
	}
	Type_Mc1Mail& grow = castle->mails_0x5A_90[5];
	if (grow.sender_4 == (uint32_t)(int32_t)(int16_t)castle->id_0x18_24)
	{
		if (castle->word_0x1A_26 < 7)
			castle->flags_0x10_16 |= 0x40;
		grow.sender_4 = 0;
	}
	return result;
}

void CastleOverflow_47130_47470(Type_Mc1Entity* castle)
{
	// the mana over the capacity (all of it without a castle size) out as mana balls (1000 a ball, 1-32) and 4 smokes
	uint8_t* playerData = Owner(castle)->playerData_0xA0_160;
	int32_t excess = 0;
	if (castle->mana_0x8C_140 + ReadS32(playerData + 0x134) > castle->dword_0x88_136)
		excess = castle->mana_0x8C_140 - castle->dword_0x88_136;
	if (!castle->word_0x1A_26)
		excess = castle->mana_0x8C_140;
	if (excess <= 0)
		return;
	int32_t balls = excess / 1000;
	int32_t free = GetFreeEntityCount_37710_37AD0();
	if (!(int16_t)free)
	{
		RebuildFreeLists_37220_375E0();
		free = GetFreeEntityCount_37710_37AD0();
		balls = 8;
		mc1State_AE400_AE3F0->reuseTop_0x11F1_4593 = -1;
		if (!(int16_t)free)
			return;
	}
	if (balls < 1)
		balls = 1;
	if (balls > 0x20)
		balls = 0x20;
	if ((int16_t)free < 0)
		free = 0;
	if ((int16_t)free > balls)
		free = balls;
	const int16_t count = (int16_t)free;
	int32_t each = excess / count;
	for (int32_t i = 0; count > i; i++)
	{
		featurePosition_AE454_AE444 = castle->position_0x48_72;
		Type_Mc1Entity* mana = CreateEntity_373F0_377B0(&featurePosition_AE454_AE444, 0x0A, 0x27);
		if (!mana)
			continue;
		mana->mana_0x8C_140 = each;
		mana->owner_0x90_144 = castle->id_0x18_24;
		mana->word_0x7E_126 = (int16_t)(NextRand(mana) % 0x30 + 0x10);
		mana->target_0x96_150.x = 0;
		mana->target_0x96_150.y = 0;
		const int16_t ground = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&castle->position_0x48_72);
		mana->word_0x2E_46 = (int16_t)((0x400 - (castle->position_0x48_72.z - ground)) / 8);
		const int16_t distance = (int16_t)(NextRand(castle) % 0x1400 + 0xF00);
		const uint16_t yaw = (uint16_t)(NextRand(castle) & 0x7FF);
		MoveEntity_41EC0_42200((axis_3d*)&featurePosition_AE454_AE444, yaw, 0, distance);
		CopyEntityPosition_41C70_41FB0(mana, &featurePosition_AE454_AE444);
		castle->mana_0x8C_140 -= mana->mana_0x8C_140;
		excess -= mana->mana_0x8C_140;
		if (excess < each)
			each = excess;
	}
	for (int i = 0; i < 4; i++)
	{
		featurePosition_AE454_AE444 = castle->position_0x48_72;
		Type_Mc1Entity* smoke = CreateEntity_373F0_377B0(&featurePosition_AE454_AE444, 0x0A, 0x36);
		if (!smoke)
			continue;
		smoke->id_0x18_24 = castle->id_0x18_24;
		MoveEntity_41EC0_42200((axis_3d*)&featurePosition_AE454_AE444, (uint16_t)(NextRand(castle) & 0x7FF), 0, 0x1900);
		CopyEntityPosition_41C70_41FB0(smoke, &featurePosition_AE454_AE444);
	}
}

static void FreeBalloon(uint8_t* slot)
{
	Type_Mc1Entity* balloon = &Entities()[ReadU16(slot)];
	DropMana_27690_27690(balloon);
	MarkToFree_41E80_421C0(balloon);
	WriteU16(slot, 0);
}

void CastleBalloonsAndGuards_47400_47740(Type_Mc1Entity* castle)
{
	// by the size: the balloons (34h of the player data, up to 3) sent to the mana, the guards (54h) at the castle
	static const int16_t counts[8][2] = { { 0, 0 }, { 1, 0 }, { 1, 0 }, { 1, 4 }, { 2, 6 }, { 2, 0x0E }, { 3, 0x12 }, { 3, 0x22 } };
	uint8_t* playerData = Owner(castle)->playerData_0xA0_160;
	const uint16_t size = (uint16_t)castle->word_0x1A_26;
	const int16_t balloons = size <= 7 ? counts[size][0] : 0;
	const int16_t guards = size <= 7 ? counts[size][1] : 0;
	int16_t i = 0;
	for (; i < balloons; i++)
	{
		WriteS32(playerData + 0x126, 0);
		uint8_t* slot = playerData + 0x34 + i * 2;
		if (!ReadU16(slot))
		{
			Type_Mc1Entity* balloon = CreateEntity_373F0_377B0(&castle->position_0x48_72, 3, 3);
			if (!balloon)
				continue;
			balloon->id_0x18_24 = castle->id_0x18_24;
			balloon->sprite_0x56_86 = (int16_t)(balloon->sprite_0x56_86 + ReadS16(playerData + 0x30));
			balloon->owner_0x90_144 = castle->id_0x18_24;
			WriteU16(slot, EntityIndex(balloon));
			balloon->actionIndex_0x46_70 = 9;
			continue;
		}
		Type_Mc1Entity* balloon = &Entities()[ReadU16(slot)];
		if (balloon->life_0x0C_12 < 0)
		{
			FreeBalloon(slot);
			continue;
		}
		const uint16_t castleIndex = EntityIndex(castle);
		if (castle->mana_0x8C_140 + ReadS32(playerData + 0x134) >= castle->dword_0x88_136)
			balloon->target_0x92_146 = castleIndex; // full: home
		else if (!((uint8_t)castle->index_0x3F_63 % balloons) && balloon->actionIndex_0x46_70 == 9)
		{
			balloon->target_0x92_146 = castleIndex;
			if (balloon->dword_0x88_136 > balloon->mana_0x8C_140)
			{
				// the nearest mana not taken by the other balloons
				Entities()[0].target_0x92_146 = 0;
				Type_Mc1Entity* other2 = &Entities()[Entities()[ReadU16(playerData + 0x34 + ((i + 2) % 3) * 2)].target_0x92_146];
				Type_Mc1Entity* other = &Entities()[Entities()[ReadU16(playerData + 0x34 + ((i + 1) % 3) * 2)].target_0x92_146];
				if (Type_Mc1Entity* mana = NearestOwnMana_46CA0_46FE0(balloon, other, other2))
					balloon->target_0x92_146 = EntityIndex(mana);
			}
		}
		WriteS32(playerData + 0x126, ReadS32(playerData + 0x126) + balloon->mana_0x8C_140);
		WriteS32(playerData + 0x122, ReadS32(playerData + 0x122) + balloon->dword_0x88_136);
	}
	for (; i < 3; i++)
		if (ReadU16(playerData + 0x34 + i * 2))
			FreeBalloon(playerData + 0x34 + i * 2);
	if (castle->word_0x2E_46 > 0)
		castle->word_0x2E_46--;
	for (int16_t j = 0; j < guards; j++)
	{
		uint8_t* slot = playerData + 0x54 + j * 2;
		if (ReadU16(slot))
		{
			const Type_Mc1Entity& guard = Entities()[ReadU16(slot)];
			if (guard.class_0x40_64 != 5 || guard.model_0x41_65 != 0x0F || guard.actionIndex_0x46_70 == 0x5F)
			{
				WriteU16(slot, 0);
				castle->word_0x2E_46 = 0x10;
			}
			continue;
		}
		if (castle->word_0x2E_46)
			continue;
		Type_Mc1Entity* guard = CreateEntity_373F0_377B0(&castle->position_0x48_72, 5, 0x0F);
		if (!guard)
			continue;
		castle->word_0x2E_46 = 0x10;
		guard->id_0x18_24 = castle->id_0x18_24;
		guard->owner_0x90_144 = castle->id_0x18_24;
		WriteU16(slot, EntityIndex(guard));
		guard->word_0x1E_30 = 0x200;
		guard->word_0x22_34 = 0x200;
		featurePosition_AE454_AE444 = guard->position_0x48_72;
		featurePosition_AE454_AE444.x = (int16_t)(featurePosition_AE454_AE444.x + 0x80);
		featurePosition_AE454_AE444.y = (int16_t)(featurePosition_AE454_AE444.y + 0x280);
		featurePosition_AE454_AE444.z = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&featurePosition_AE454_AE444);
		CopyEntityPosition_41C70_41FB0(guard, &featurePosition_AE454_AE444);
	}
}

void CastleAction_46DB0_470F0(Type_Mc1Entity* castle)
{
	// the castle standing: destroyed -> 6, growing -> 5; word_0x32 counts the frames of its building spell
	const uint16_t building = (uint16_t)castle->word_0x32_50;
	if (building == 1)
	{
		castle->actionIndex_0x46_70 = 5;
		castle->word_0x30_48 = 3;
		castle->word_0x32_50 = 0;
		return;
	}
	if (building)
	{
		castle->word_0x32_50 = (int16_t)(building - 1);
		CancelCast_46D20_47060(castle, 1);
		castle->position_0x48_72.z = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&castle->position_0x48_72);
		return;
	}
	if (CastleDamage_47EC0_48200(castle) == 2)
		castle->actionIndex_0x46_70 = 6;
	else if (castle->flags_0x10_16 & 0x40)
	{
		castle->word_0x30_48 = 0;
		castle->actionIndex_0x46_70 = 5;
	}
	castle->position_0x48_72.z = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&castle->position_0x48_72);
	castle->owner_0x90_144 = castle->id_0x18_24;
	if (castle->index_0x3F_63 & 1)
		return;
	CastleOverflow_47130_47470(castle);
	SetBuildingBox_37150_37510(castle, castle->word_0x1A_26);
	CastleBalloonsAndGuards_47400_47740(castle);
	if (castle->mana_0x8C_140 >= castle->dword_0x88_136)
		return;
	// a mana ball of the owner at the castle gets in
	for (Type_Mc1Entity* mana = mc1Settings_AE408_AE3F8->entityLists_0x8E1A_36378[22]; mana > Entities(); mana = mana->next_0x00_0)
	{
		if (mana->model_0x41_65 == 0x27 && mana->owner_0x90_144 == (uint32_t)(int32_t)(int16_t)castle->id_0x18_24 && EntitiesOverlap_11950_11950(castle, mana))
		{
			castle->mana_0x8C_140 += mana->mana_0x8C_140;
			MarkToFree_41E80_421C0(mana);
			return;
		}
	}
}

void ClearBuildingsAround_12C50_12C50(Type_Mc1Entity* castle)
{
	// the buildings (class 10 model 2Dh) under the next size of the castle are destroyed
	SetBuildingBox_37150_37510(castle, (int16_t)(castle->word_0x1A_26 + 1));
	const int32_t width = castle->box_0x4E_78[1] + 0x100;
	const int32_t height = castle->box_0x4E_78[2] + 0x100;
	for (Type_Mc1Entity* building = mc1Settings_AE408_AE3F8->entityLists_0x8E1A_36378[23]; building > Entities(); building = building->next_0x00_0)
	{
		const int32_t dx = (int16_t)(building->position_0x48_72.x - castle->position_0x48_72.x);
		if ((dx < 0 ? -dx : dx) > width + building->box_0x4E_78[1])
			continue;
		const int32_t dy = (int16_t)(building->position_0x48_72.y - castle->position_0x48_72.y);
		if ((dy < 0 ? -dy : dy) > height + building->box_0x4E_78[2])
			continue;
		building->life_0x0C_12 = -1;
	}
	SetBuildingBox_37150_37510(castle, castle->word_0x1A_26);
}

// a band of tiles: a tile changed by hand (angle bit 80h) stops the growth; the rows after the first start at rowX (the
// original returns to the corner of the area, also in the right band)
static bool BandChanged(uint8_t x, uint8_t y, uint16_t rows, uint16_t columns, uint8_t rowX)
{
	uint8_t column = x;
	for (; rows; rows--, y++, column = rowX)
	{
		for (uint16_t i = columns; i; i--, column++)
			if (mapAngle_FC1E0_FC1D0[(y << 8) | column] & 0x80)
				return true;
	}
	return false;
}

uint8_t CastleCanGrow_12D10_12D10(Type_Mc1Entity* castle)
{
	// the next size: no other castle overlapping, no tile changed by hand in the new bands (as the original checks them)
	const int16_t oldWidth = (int16_t)(castle->box_0x4E_78[1] >> 8);
	const int16_t oldHeight = (int16_t)(castle->box_0x4E_78[2] >> 8);
	SetBuildingBox_37150_37510(castle, (int16_t)(castle->word_0x1A_26 + 1));
	for (Type_Mc1Entity* other = mc1Settings_AE408_AE3F8->entityLists_0x8E1A_36378[21]; other > Entities(); other = other->next_0x00_0)
	{
		if (other->model_0x41_65 == 2 && other != castle && EntitiesOverlap_11950_11950(other, castle))
		{
			SetBuildingBox_37150_37510(castle, castle->word_0x1A_26);
			return 0;
		}
	}
	const uint8_t centerX = (uint8_t)((castle->position_0x48_72.x + 0x80) >> 8);
	const uint8_t centerY = (uint8_t)((castle->position_0x48_72.y + 0x80) >> 8);
	const int16_t width = (int16_t)(castle->box_0x4E_78[1] >> 8);
	const int16_t height = (int16_t)(castle->box_0x4E_78[2] >> 8);
	const uint8_t x = (uint8_t)(centerX - (uint8_t)width);
	const uint8_t y = (uint8_t)(centerY - (uint8_t)height);
	const uint16_t addWidth = (uint16_t)(width - oldWidth);
	const uint16_t addHeight = (uint16_t)(height - oldHeight);
	const bool changed = BandChanged(x, y, addHeight, (uint16_t)(width * 2), x)
		|| BandChanged(x, (uint8_t)((uint8_t)height * 2 + y - (uint8_t)addHeight), addHeight, (uint16_t)(width * 2), x)
		|| BandChanged(x, (uint8_t)(y + (uint8_t)addHeight), addHeight, addWidth, x)
		|| BandChanged((uint8_t)(x + (uint8_t)width - (uint8_t)addWidth), (uint8_t)(y + (uint8_t)addHeight), addHeight, addWidth, x);
	SetBuildingBox_37150_37510(castle, castle->word_0x1A_26);
	return changed ? 0 : 1;
}

static Type_Mc1Entity* CreateCastleEffect(Type_Mc1Entity* castle, int32_t model, int16_t next)
{
	Type_Mc1Entity* effect = CreateEntity_373F0_377B0(&castle->target_0x96_150, 0x0A, model);
	if (effect)
	{
		effect->byte_0x47_71 = (uint8_t)castle->word_0x1A_26;
		effect->id_0x18_24 = castle->id_0x18_24;
		effect->word_0x2A_42 = EntityIndex(castle);
		castle->word_0x30_48 = next;
	}
	return effect;
}

void CastleEffect2A_47020_47360(Type_Mc1Entity* castle)
{
	CreateCastleEffect(castle, 0x2A, 4);
}

void CastleEffect29_47080_473C0(Type_Mc1Entity* castle)
{
	CreateCastleEffect(castle, 0x29, 6);
}

void CastleGrown_47960_47CA0(Type_Mc1Entity* castle)
{
	// the next size: the effect 42 at target_0x96, the box, the stamp of the castle, 32h / 1A0h of the owner
	Type_Mc1Entity* effect = CreateScenery42_3B7B0_3BB30(&castle->target_0x96_150);
	if (!effect)
		return;
	sub_55370_558A0((int16_t)EntityIndex(castle), -1, 0x0A);
	castle->flags_0x10_16 &= ~0x40u;
	castle->word_0x1A_26++;
	castle->actionIndex_0x46_70 = 5;
	castle->word_0x30_48 = 4;
	SetBuildingBox_37150_37510(castle, castle->word_0x1A_26);
	GrowCastle_47C60_47FA0(castle);
	uint8_t* playerData = Owner(castle)->playerData_0xA0_160;
	WriteU16(playerData + 0x32, EntityIndex(castle));
	WriteU16(playerData + 0x1A0, (uint16_t)castle->word_0x1A_26);
	effect->word_0x2A_42 = EntityIndex(castle);
	effect->id_0x18_24 = castle->id_0x18_24;
	effect->byte_0x3C_60 = 0;
	effect->byte_0x47_71 = (uint8_t)castle->word_0x1A_26;
	effect->flags_0x10_16 |= 0x10000;
}

void CastleGrowAction_46F10_47250(Type_Mc1Entity* castle)
{
	// the growth by word_0x30: 0 grow (or 2 when it cannot), 2 stop, 3 / 5 the effects 2Ah / 29h, 1 / 4 / 6 wait
	switch ((uint16_t)castle->word_0x30_48)
	{
	case 0:
		ClearBuildingsAround_12C50_12C50(castle);
		if (castle->word_0x1A_26 && !CastleCanGrow_12D10_12D10(castle))
		{
			castle->word_0x30_48 = 2;
			castle->flags_0x10_16 &= ~0x40u;
			return;
		}
		if (!(castle->flags_0x10_16 & 2))
		{
			castle->flags_0x10_16 |= 2;
			castle->sprite_0x56_86 = (int16_t)(castle->sprite_0x56_86 + ReadS16(Owner(castle)->playerData_0xA0_160 + 0x30));
		}
		CastleGrown_47960_47CA0(castle);
		return;
	case 2:
		castle->actionIndex_0x46_70 = 4;
		CancelCast_46D20_47060(castle, 0);
		castle->word_0x30_48 = 0;
		return;
	case 3:
		CancelCast_46D20_47060(castle, 1);
		CastleEffect2A_47020_47360(castle);
		return;
	case 5:
		CancelCast_46D20_47060(castle, 1);
		castle->position_0x48_72.z = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&castle->position_0x48_72);
		CastleEffect29_47080_473C0(castle);
		return;
	case 1:
	case 4:
	case 6:
		castle->position_0x48_72.z = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&castle->position_0x48_72);
		return;
	}
}

void DestroyAtTile_40E20_41160(uint16_t tile, uint16_t id)
{
	// the entities of the tile not of the id: class 2 freed, class 5 (not the models 6, 8, 10h) killed by the id
	Type_Mc1Entity* entities = Entities();
	for (Type_Mc1Entity* other = &entities[(int16_t)mapEntityIndex_10C1E0_10C1D0[tile]]; other != entities; other = &entities[other->mapNext_0x14_20])
	{
		if ((int32_t)(int16_t)other->id_0x18_24 == (int32_t)id)
			continue;
		if (other->class_0x40_64 == 2)
			MarkToFree_41E80_421C0(other);
		else if (other->class_0x40_64 == 5 && other->model_0x41_65 != 0x10 && other->model_0x41_65 != 6 && other->model_0x41_65 != 8)
		{
			other->life_0x0C_12 = -1;
			other->word_0x26_38 = id;
			other->word_0x28_40 = id;
		}
	}
}

// the size of a sprite of the buildings (halved in MCGA as the relocation doubled it)
static uint8_t BuildByte(int sprite, bool height)
{
	const uint8_t value = height ? buildIndex[sprite].height_5 : buildIndex[sprite].width_4;
	return *videoMode_12F02E_12F01E == 1 ? (uint8_t)(value >> 1) : value;
}

static uint16_t Tile(uint8_t x, uint8_t y)
{
	return (uint16_t)(x | (y << 8));
}

// a pixel of the castle sprite: the terrain type of the tile (as 33800 gets it with cl the pixel, dl as left)
static void CastleTerrainType(uint8_t pixel, uint16_t tile)
{
	const uint32_t high = pixel / 16;
	if (!high)
	{
		const uint8_t part = (uint8_t)(pixel % 7);
		if (part)
			SetTileTerrainType_33800_33BF0(pixel, tile, (uint8_t)(part - 1), part);
	}
	else if (high <= 2)
		SetTileTerrainType_33800_33BF0(pixel, tile, (uint8_t)(high + 7), 0);
	else if (high == 3)
	{
		const uint8_t low = (uint8_t)(pixel % 16);
		SetTileTerrainType_33800_33BF0(pixel, tile, (uint8_t)(low / 3 + 0x0A), (uint8_t)(low % 3));
	}
	else
		SetTileTerrainType_33800_33BF0(pixel, tile, (uint8_t)(high + 0x0B), 0);
}

void CastleRaise_285C0_285C0(Type_Mc1Entity* effect)
{
	// the castle of the size byte_0x47 raised into the terrain in 19 steps (word_0x1A) by the build sprites 1..size,
	// the heights wanted per tile in the scratch AE3FC; at the end the hand changed flag back and the castle goes on
	int16_t* wanted = (int16_t*)backScreen_AE3FC_AE3EC;
	if (!(effect->flags_0x10_16 & 2))
	{
		effect->word_0x1A_26 = 0x13;
		effect->flags_0x10_16 |= 2;
	}
	const int16_t steps = effect->word_0x1A_26;
	const uint8_t centerX = (uint8_t)((effect->position_0x48_72.x + 0x80) >> 8);
	const uint8_t centerY = (uint8_t)((effect->position_0x48_72.y + 0x80) >> 8);
	const int size = (int8_t)effect->byte_0x47_71;
	if (steps <= 0)
	{
		effect->word_0x1A_26 = (int16_t)(steps + 1);
		if (effect->word_0x1A_26)
			return;
		const uint32_t width = BuildByte(size, false);
		const uint32_t height = BuildByte(size, true);
		const uint8_t x = (uint8_t)(centerX - (width >> 1));
		uint8_t y = (uint8_t)(centerY - (height >> 1));
		for (uint32_t row = 0; row < height; row++, y++)
		{
			uint8_t column = x;
			for (uint32_t i = 0; i < width; i++, column++)
			{
				uint8_t& angle = mapAngle_FC1E0_FC1D0[Tile(column, y)];
				if (angle & 8)
					angle = (uint8_t)((angle | 0x80) & 0xF7);
			}
		}
		Entities()[effect->word_0x2A_42].word_0x30_48 = 5;
		MarkToFree_41E80_421C0(effect);
		return;
	}
	effect->word_0x1A_26 = (int16_t)(steps - 1);
	if (!effect->word_0x1A_26)
	{
		effect->word_0x1A_26 = effect->byte_0x3C_60 ? (int16_t)-0x19 : (int16_t)-1;
		return;
	}
	if (Entities()[effect->word_0x2A_42].word_0x32_50)
		return;
	const int32_t top = effect->position_0x48_72.z >> 5;
	const uint32_t width = BuildByte(size, false);
	const uint32_t height = BuildByte(size, true);
	memset(wanted, 0, width * height * 2);
	const uint8_t halfWidth = (uint8_t)(width >> 1);
	const uint8_t halfHeight = (uint8_t)(height >> 1);
	const uint8_t x0 = (uint8_t)(centerX - halfWidth);
	const uint8_t y0 = (uint8_t)(centerY - halfHeight);
	for (uint8_t sprite = 1; (int32_t)sprite <= size; sprite++)
	{
		// the rows of the sprite (byte 4), the columns (byte 5): 0 the end of a row, -n skip, n pixels
		uint32_t rows = BuildByte(sprite, false);
		const uint32_t columns = BuildByte(sprite, true);
		const uint8_t* data = buildIndex[sprite].data;
		uint8_t rowY = (uint8_t)(centerY - (rows >> 1));
		const uint8_t rowX = (uint8_t)(centerX - (columns >> 1));
		const int32_t columnOffset = (int32_t)halfWidth - (int32_t)(columns >> 1);
		int32_t rowOffset = ((int32_t)halfHeight - (int32_t)(rows >> 1)) * (int32_t)width;
		uint16_t tile = Tile(rowX, rowY);
		int32_t index = columnOffset + rowOffset;
		uint32_t at = 0;
		while (rows)
		{
			const int8_t code = (int8_t)data[at++];
			if (!code)
			{
				rowY++;
				tile = Tile(rowX, rowY);
				rowOffset += width;
				rows--;
				index = columnOffset + rowOffset;
				continue;
			}
			if (code < 0)
			{
				tile = (uint16_t)((tile & 0xFF00) | (uint8_t)((tile & 0xFF) - code));
				index -= code;
				continue;
			}
			for (int32_t count = code; count; count--)
			{
				if (effect->flags_0x10_16 & 0x10000)
					DestroyAtTile_40E20_41160(tile, (uint16_t)(int16_t)effect->id_0x18_24);
				const uint8_t pixel = data[at++];
				const int32_t ground = mapHeightmap_DC1E0_DC1D0[tile];
				if (pixel >= 0x0F)
				{
					const uint8_t level = (uint8_t)(pixel % 16);
					if (level)
						wanted[index] = (int16_t)(top + (uint8_t)(level - 1) * 4 - ground);
				}
				else if (pixel > 6)
					wanted[index] = (int16_t)(top - ground);
				if (!(effect->word_0x1A_26 % 7) || effect->word_0x1A_26 == 1)
					CastleTerrainType(pixel, tile);
				index++;
				tile = (uint16_t)((tile & 0xFF00) | (uint8_t)((tile & 0xFF) + 1));
			}
		}
	}
	// a step of the heights to the wanted ones
	uint32_t at = 0;
	uint8_t y = y0;
	for (uint32_t row = 0; row < height; row++, y++)
	{
		uint8_t x = x0;
		for (uint32_t column = 0; column < width; column++, x++, at++)
		{
			const uint16_t tile = Tile(x, y);
			if (const int16_t add = wanted[at])
			{
				if (!mapHeightmap_DC1E0_DC1D0[tile])
				{
					mapAngle_FC1E0_FC1D0[tile] = (uint8_t)((mapAngle_FC1E0_FC1D0[tile] & 0xF8) | 1);
					RebuildTerrainTiles_33E10_34200(tile, tile);
				}
				mapHeightmap_DC1E0_DC1D0[tile] = (uint8_t)(mapHeightmap_DC1E0_DC1D0[tile] + add / effect->word_0x1A_26);
				if (effect->word_0x1A_26 == 1 && (mapAngle_FC1E0_FC1D0[tile] & 0x80))
					mapAngle_FC1E0_FC1D0[tile] = (uint8_t)((mapAngle_FC1E0_FC1D0[tile] & 0x77) | 8);
			}
			if (effect->word_0x1A_26 == 2)
				mapAngle_FC1E0_FC1D0[tile] &= 0xF7;
		}
	}
}

// the tiles of the castle area (the size of the sprite byte_0x47) from the corner
template <typename F> static void ForCastleTiles(uint8_t x, uint8_t y, uint32_t width, uint32_t height, F f)
{
	for (uint32_t row = 0; row < height; row++, y++)
	{
		uint8_t column = x;
		for (uint32_t i = 0; i < width; i++, column++)
			f(Tile(column, y));
	}
}

void CastleFlatten_28200_28200(Type_Mc1Entity* effect)
{
	// the ground of the castle to one height (word_0x2C, from the square, at most DCh) in word_0x1A steps
	const uint8_t centerX = (uint8_t)((effect->position_0x48_72.x + 0x80) >> 8);
	const uint8_t centerY = (uint8_t)((effect->position_0x48_72.y + 0x80) >> 8);
	const int size = (int8_t)effect->byte_0x47_71;
	uint32_t width = BuildByte(size, false);
	uint32_t height = BuildByte(size, true);
	const uint8_t x = (uint8_t)(centerX - (width >> 1));
	const uint8_t y = (uint8_t)(centerY - (height >> 1));
	if (!(effect->flags_0x10_16 & 2))
	{
		width += 2;
		height += 2;
		effect->flags_0x10_16 |= 2;
		effect->word_0x1A_26 = 10;
		effect->word_0x30_48 = (int16_t)(effect->position_0x48_72.z >> 5);
		effect->word_0x2C_44 = (int16_t)GetTerrainHeightFromSquare_361C0_36580((char)(uint8_t)(x - 1), (char)(uint8_t)(y - 1), (char)height, (char)width);
		if ((uint16_t)effect->word_0x2C_44 > 0xDC)
			effect->word_0x2C_44 = 0xDC;
		if ((int32_t)effect->word_0x30_48 == (int32_t)(uint16_t)effect->word_0x2C_44)
			effect->word_0x1A_26 = 0;
		return;
	}
	Type_Mc1Entity* castle = &Entities()[(uint16_t)effect->word_0x2A_42];
	if (castle->word_0x32_50 || !effect->word_0x1A_26)
	{
		castle->word_0x30_48 = 2;
		castle->target_0x96_150.z = (int16_t)((uint16_t)effect->word_0x30_48 << 5);
		SmoothBuildingBorder_35F30_362F0(centerX, centerY, (int32_t)(height >> 1), (int32_t)(width >> 1), 3);
		MarkToFree_41E80_421C0(effect);
		return;
	}
	const int32_t step = ((int32_t)(uint16_t)effect->word_0x2C_44 - effect->word_0x30_48) / effect->word_0x1A_26;
	effect->word_0x30_48 = (int16_t)(effect->word_0x30_48 + step);
	if (effect->word_0x1A_26 == 1)
	{
		ForCastleTiles(x, y, width, height, [step](uint16_t tile) {
			uint8_t& angle = mapAngle_FC1E0_FC1D0[tile];
			if (angle & 0x80)
				angle = (uint8_t)((angle & 0x77) | 8);
			mapHeightmap_DC1E0_DC1D0[tile] = (uint8_t)(mapHeightmap_DC1E0_DC1D0[tile] + (uint8_t)step);
		});
		effect->word_0x1A_26 = -10;
		return;
	}
	if (effect->word_0x1A_26 == -1)
	{
		ForCastleTiles(x, y, width, height, [](uint16_t tile) {
			uint8_t& angle = mapAngle_FC1E0_FC1D0[tile];
			if (angle & 8)
				angle = (uint8_t)((angle | 0x80) & 0xF7);
		});
		effect->word_0x1A_26++;
		return;
	}
	if (effect->word_0x1A_26 > 0)
	{
		ForCastleTiles(x, y, width, height, [step](uint16_t tile) {
			mapHeightmap_DC1E0_DC1D0[tile] = (uint8_t)(mapHeightmap_DC1E0_DC1D0[tile] + (uint8_t)step);
		});
		effect->word_0x1A_26--;
		return;
	}
	effect->word_0x1A_26++;
}

void CastleGrowMail_293D0_293D0(Type_Mc1Entity* effect)
{
	// the castle spell at the castle of the owner: the mail 5 (grow), else the cast is cancelled
	effect->word_0x1A_26++;
	const int32_t life = effect->life_0x0C_12;
	effect->life_0x0C_12 = life - 1;
	if (life < 0)
	{
		MarkToFree_41E80_421C0(effect);
		return;
	}
	NextFrame_42510_42850(effect);
	if (effect->flags_0x10_16 & 2)
	{
		MarkToFree_41E80_421C0(effect);
		return;
	}
	effect->flags_0x10_16 |= 2;
	if ((int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&effect->position_0x48_72) > 0xE600)
		CancelCast_46D20_47060(effect, 0);
	Type_Mc1Entity* castle = &Entities()[ReadU16(Owner(effect)->playerData_0xA0_160 + 0x32)];
	if (EntitiesOverlap_11950_11950(effect, castle))
	{
		castle->mails_0x5A_90[5].sender_4 = effect->id_0x18_24;
		castle->mails_0x5A_90[5].value_0 = 10;
		MarkToFree_41E80_421C0(effect);
		return;
	}
	CancelCast_46D20_47060(effect, 0);
	MarkToFree_41E80_421C0(effect);
}

int32_t BalloonDamage_481D0_48510(Type_Mc1Entity* balloon)
{
	// 0 none, 1 hit (4 at 189h of its player data), 2 destroyed
	if (balloon->life_0x0C_12 < 0)
		return 2;
	Type_Mc1Mail& mail = balloon->mails_0x5A_90[0];
	if (!mail.sender_4)
		return 0;
	balloon->life_0x0C_12 -= mail.value_0;
	balloon->playerData_0xA0_160[0x189] = 4;
	if (balloon->life_0x0C_12 < 0)
	{
		balloon->word_0x26_38 = mail.sender_4;
		return 2;
	}
	mail.sender_4 = 0;
	return 1;
}

void BalloonAction_47F90_482D0(Type_Mc1Entity* balloon)
{
	// the balloon of a castle: to its target, a mana ball of the owner (pulled within 400h, taken when touched) or the
	// castle (the mana given when over it)
	if (balloon->target_0x92_146)
	{
		Type_Mc1Entity* target = &Entities()[balloon->target_0x92_146];
		featurePosition_AE454_AE444 = balloon->position_0x48_72;
		balloon->word_0x1E_30 = (int16_t)GetYawAngle_42150_42490((axis_3d*)&balloon->position_0x48_72, (axis_3d*)&target->position_0x48_72);
		const uint8_t* params = balloon->typeParams_0x9C_156;
		bool move = true;
		if (target->class_0x40_64 == 0x0A)
		{
			move = false;
			if ((int32_t)target->owner_0x90_144 == (int32_t)(int16_t)balloon->id_0x18_24)
			{
				const int32_t distance = (int32_t)Distance2D_423D0_42710(&featurePosition_AE454_AE444, &target->position_0x48_72);
				if (distance <= 0x400)
				{
					target->flags_0x10_16 |= 0x40;
					target->target_0x92_146 = EntityIndex(balloon);
					if (EntitiesOverlap_11950_11950(target, balloon))
					{
						balloon->mana_0x8C_140 += target->mana_0x8C_140;
						balloon->owner_0x90_144 = target->owner_0x90_144;
						balloon->target_0x92_146 = 0;
						balloon->life_0x0C_12 = balloon->maxLife_0x08_8;
						MarkToFree_41E80_421C0(target);
					}
				}
				else
					target->flags_0x10_16 &= ~0x40u;
				if (distance > balloon->word_0x7E_126)
					move = true;
				else
				{
					featurePosition_AE454_AE444.x = target->position_0x48_72.x;
					featurePosition_AE454_AE444.y = target->position_0x48_72.y;
				}
			}
		}
		else if (target->class_0x40_64 == 3)
		{
			const uint32_t range = (uint32_t)(balloon->word_0x7E_126 * target->word_0x1A_26);
			if (Distance2D_423D0_42710(&featurePosition_AE454_AE444, &target->position_0x48_72) <= range)
			{
				move = false;
				const int16_t ground = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&featurePosition_AE454_AE444);
				if (featurePosition_AE454_AE444.z <= ground + ReadS16(params + 0x0C) && target->word_0x1A_26 > 0)
				{
					featurePosition_AE454_AE444.x = target->position_0x48_72.x;
					featurePosition_AE454_AE444.y = target->position_0x48_72.y;
					target->mana_0x8C_140 += balloon->mana_0x8C_140;
					balloon->mana_0x8C_140 = 0;
					balloon->owner_0x90_144 = balloon->id_0x18_24;
					balloon->life_0x0C_12 = balloon->maxLife_0x08_8;
				}
			}
		}
		if (move)
			MoveEntity_41EC0_42200((axis_3d*)&featurePosition_AE454_AE444, (uint16_t)balloon->word_0x1E_30, (uint16_t)balloon->word_0x20_32, balloon->word_0x7E_126);
		const int16_t ground = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&featurePosition_AE454_AE444);
		ClampHeight_42000_42340(&featurePosition_AE454_AE444, ground, ReadS16(params + 0x0C), ReadS16(params + 0x0A), ReadS16(params + 0x0E));
		CopyEntityPosition_41C70_41FB0(balloon, &featurePosition_AE454_AE444);
	}
	BalloonDamage_481D0_48510(balloon);
}


void CastleLower_28FE0_28FE0(Type_Mc1Entity* entity)
{
	// the castle of the size byte_0x47 taken down from the terrain by its sprite (the entity is entity 0 filled by 47A70):
	// the hand changed flag off, the walls lowered, creatures out of it (word_0x1A of them), the tiles smoothed; then the
	// entity is marked to be freed
	const uint8_t centerX = (uint8_t)((entity->position_0x48_72.x + 0x80) >> 8);
	const uint8_t centerY = (uint8_t)((entity->position_0x48_72.y + 0x80) >> 8);
	const uint8_t sprite = entity->byte_0x47_71;
	const uint8_t* data = buildIndex[sprite].data;
	const uint32_t height = BuildByte(sprite, true);
	const uint32_t width = BuildByte(sprite, false);
	const uint8_t x0 = (uint8_t)(centerX - (width >> 1));
	uint8_t y0 = (uint8_t)(centerY - (height >> 1));
	const int32_t base = entity->model_0x41_65
		? GetTerrainHeightFromSquare_361C0_36580((char)x0, (char)y0, (char)height, (char)width)
		: entity->position_0x48_72.z >> 5;
	const int32_t top = base << 5;
	const int32_t low = (base - 10) << 5;
	uint32_t rows = height;
	uint32_t at = 0;
	uint16_t tile = Tile(x0, y0);
	while (rows)
	{
		const int8_t code = (int8_t)data[at++];
		if (!code)
		{
			y0++;
			rows--;
			tile = Tile(x0, y0);
			continue;
		}
		if (code < 0)
		{
			tile = (uint16_t)((tile & 0xFF00) | (uint8_t)((tile & 0xFF) - code));
			continue;
		}
		for (int32_t count = code; count; count--, tile = (uint16_t)((tile & 0xFF00) | (uint8_t)((tile & 0xFF) + 1)))
		{
			const uint8_t pixel = data[at++];
			if (!pixel)
				continue;
			featurePosition_AE454_AE444.x = (int16_t)((tile & 0xFF) << 8);
			featurePosition_AE454_AE444.y = (int16_t)((tile >> 8) << 8);
			featurePosition_AE454_AE444.z = (int16_t)((at & 7) ? top : low);
			if (entity->word_0x1A_26 > 0)
			{
				entity->word_0x1A_26--;
				if (!entity->word_0x1A_26)
				{
					if (Type_Mc1Entity* creature = CreateEntity_373F0_377B0(&featurePosition_AE454_AE444, 5, 0x0C))
						creature->actionIndex_0x46_70 = 0x49;
				}
				else if (entity->word_0x1A_26 < 4)
				{
					if (Type_Mc1Entity* creature = CreateEntity_373F0_377B0(&featurePosition_AE454_AE444, 5, 4))
						creature->actionIndex_0x46_70 = 0x19;
				}
				else
					SpawnCreature_28D10_28D10(entity, &featurePosition_AE454_AE444);
			}
			uint8_t& angle = mapAngle_FC1E0_FC1D0[tile];
			uint8_t& heightAt = mapHeightmap_DC1E0_DC1D0[tile];
			const uint32_t high = pixel / 16;
			if (!high)
				angle &= 0x7F;
			else if (high == 3)
			{
				angle &= 0x7F;
				const uint8_t kind = (uint8_t)((pixel % 16) % 3);
				if (kind == 1 && heightAt > 0x0C)
					heightAt = (uint8_t)(heightAt - 0x0C);
				if ((kind == 1 || kind == 2) && heightAt > 0x10)
					heightAt = (uint8_t)(heightAt - 0x10);
				RebuildTerrainTiles_33E10_34200(tile, tile);
			}
			else
			{
				angle = (uint8_t)((angle & 0x70) | 1);
				RebuildTerrainTiles_33E10_34200(tile, tile);
				const uint8_t level = (uint8_t)(pixel % 16);
				if (level)
				{
					const uint8_t step = (uint8_t)(level - 1);
					const int32_t limit = step * 4;
					if (heightAt > limit)
					{
						if (NextRand(entity) % 0x32 > 0x14)
						{
							const int32_t drop = limit - (int32_t)(NextRand(entity) % 0x14);
							heightAt = (uint8_t)(heightAt - drop);
						}
						else
							heightAt = (uint8_t)(heightAt - (uint8_t)(step << 2));
					}
					else
						heightAt = 0;
				}
			}
		}
	}
	y0 = (uint8_t)(y0 - (uint8_t)height);
	for (uint32_t row = 0; row < height; row++)
		for (uint32_t column = 0; column < width; column++)
			SmoothTileHeight_360C0_36480(Tile((uint8_t)(x0 + column), (uint8_t)(y0 + row)));
	MarkToFree_41E80_421C0(entity);
}

void CastleShrink_47A70_47DB0(Type_Mc1Entity* castle)
{
	// destroyed: a size down (the mana over the smaller capacity out, the walls of the size down by 28FE0 through entity 0),
	// at the size 0 the castle is gone
	if (castle->word_0x1A_26 > 0)
	{
		const int32_t tenth = castle->dword_0x88_136 * 10 / 100;
		castle->dword_0x88_136 -= tenth;
		CastleOverflow_47130_47470(castle);
		castle->dword_0x88_136 += tenth;
		sub_55370_558A0((int16_t)EntityIndex(castle), -1, 0x1E);
		Type_Mc1Entity* temporary = &Entities()[0];
		temporary->position_0x48_72 = castle->target_0x96_150;
		temporary->byte_0x47_71 = (uint8_t)castle->word_0x1A_26;
		temporary->id_0x18_24 = castle->id_0x18_24;
		temporary->model_0x41_65 = 0;
		temporary->word_0x1A_26 = 0;
		temporary->word_0x2A_42 = EntityIndex(castle);
		CastleLower_28FE0_28FE0(temporary);
		castle->word_0x1A_26--;
		SetBuildingBox_37150_37510(castle, castle->word_0x1A_26);
		GrowCastle_47C60_47FA0(castle);
		CancelCast_46D20_47060(castle, 1);
	}
	if (!castle->word_0x1A_26)
	{
		CancelCast_46D20_47060(castle, 0);
		WriteU16(Owner(castle)->playerData_0xA0_160 + 0x32, 0);
		MarkToFree_41E80_421C0(castle);
	}
}

void CastleDestroyed_470E0_47420(Type_Mc1Entity* castle)
{
	// with a free entity: a size down, the mana out, the balloons; back to the action 4
	if (GetFreeEntityCount_37710_37AD0())
	{
		CastleShrink_47A70_47DB0(castle);
		castle->actionIndex_0x46_70 = 4;
		CastleOverflow_47130_47470(castle);
		CastleBalloonsAndGuards_47400_47740(castle);
		castle->word_0x30_48 = 0;
		castle->word_0x32_50 = 5;
	}
	else
		castle->actionIndex_0x46_70 = 4;
}
