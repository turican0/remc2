#include "Mc1Spells.h"

#include <cstring>

#include "Mc1Entity.h"
#include "Mc1EntityTables.h"
#include "Mc1Game.h"
#include "Mc1Level.h"
#include "Mc1NotPorted.h"
#include "Mc1Sound.h"
#include "Mc1SceneryActions.h"
#include "Mc1Search.h"
#include "Mc1Shared.h"
#include "Mc1Terrain.h"
#include "Mc1Wizard.h"

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

static uint32_t NextRand(Type_Mc1Entity* entity)
{
	entity->rand_0x04_4 = entity->rand_0x04_4 * 0x24A1 + 0x24DF;
	return entity->rand_0x04_4;
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
		QueueSound_55370_558A0(EntityIndex(wizard), -1, 0x12);
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

// the casting of the spells in the hand of a wizard (word_0x2A): word_0x30 counts down from word_0x32, byte_0x3D more
// projectiles in the first frame

int32_t SpellCanCast_55DD0_56300(Type_Mc1Entity* spell, Type_Mc1Entity* wizard)
{
	// the mana of the wizard (and of its castle for the spells with dword_0x84), else the sound 1Dh
	if (wizard->mana_0x8C_140 >= 0 && wizard->life_0x0C_12 >= 0)
	{
		bool castle = true;
		if (const int32_t cost = spell->dword_0x84_132)
		{
			const uint16_t castleIndex = (uint16_t)ReadS16(wizard->playerData_0xA0_160 + 0x32);
			castle = castleIndex && cost <= Entities()[castleIndex].mana_0x8C_140;
		}
		if (castle)
		{
			if (wizard->mana_0x8C_140 >= spell->dword_0x88_136 && spell->word_0x30_48 == spell->word_0x32_50)
				return 1;
			if (spell->word_0x30_48 != spell->word_0x32_50)
				return 1;
		}
	}
	QueueSound_55370_558A0(0, ReadS16(wizard->playerData_0xA0_160 + 0x30), 0x1D);
	return 0;
}

int32_t SpellUseMana_55E80_563B0(Type_Mc1Entity* spell, Type_Mc1Entity* wizard)
{
	// the first frame takes the mana (dword_0x84 of the wizard, the mana taken by the frame), later frames stop the gain
	if (spell->word_0x30_48 == spell->word_0x32_50)
	{
		if (wizard->dword_0x84_132 < 0)
			wizard->dword_0x84_132 -= spell->dword_0x88_136;
		else
			wizard->dword_0x84_132 = -spell->dword_0x88_136;
		return 1;
	}
	if (spell->word_0x30_48 && wizard->dword_0x84_132 > 0)
		wizard->dword_0x84_132 = 0;
	return 0;
}

static void CopyToSegments(Type_Mc1Entity* entity, Type_Mc1Position* position)
{
	for (Type_Mc1Entity* segment = &Entities()[entity->word_0x36_54]; segment != Entities(); segment = &Entities()[segment->word_0x36_54])
		CopyEntityPosition_41C70_41FB0(segment, position);
}

void SpellFromHand_55EF0_56420(Type_Mc1Entity* wizard, Type_Mc1Entity* projectile)
{
	// the projectile from the hand: flag 100h left, 200h right, a tile aside (not into the ground)
	Type_Mc1Position position = projectile->position_0x48_72;
	int32_t side = 0;
	if (wizard->flags_0x10_16 & 0x100)
		side = -0x200;
	else if (wizard->flags_0x10_16 & 0x200)
		side = 0x200;
	if (side)
	{
		MoveEntity_41EC0_42200((axis_3d*)&position, (uint16_t)((wizard->word_0x1E_30 + side) & 0x7FF), 0, 0x100);
		if ((int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&position) > position.z)
			position = projectile->position_0x48_72;
		CopyToSegments(projectile, &position);
	}
	CopyEntityPosition_41C70_41FB0(projectile, &position);
}

// 56090 / 56510: the projectile of class 9 at the wizard flying by its yaw and pitch
static void SpellCastProjectile(Type_Mc1Entity* spell, int32_t model, uint8_t kind, bool fireball)
{
	if (spell->word_0x30_48 <= 0)
		return;
	Type_Mc1Entity* wizard = &Entities()[spell->word_0x2A_42];
	if (spell->word_0x2A_42)
	{
		if (SpellCanCast_55DD0_56300(spell, wizard))
		{
			while ((int8_t)spell->byte_0x3D_61 >= 0)
			{
				Type_Mc1Entity* projectile;
				if (spell->word_0x30_48 == spell->word_0x32_50 && (projectile = CreateEntity_373F0_377B0(&wizard->position_0x48_72, 9, model)))
				{
					projectile->word_0x7E_126 = (int16_t)(projectile->word_0x7E_126 + wizard->word_0x7E_126);
					SpellFromHand_55EF0_56420(wizard, projectile);
					projectile->byte_0x44_68 = 0x0A;
					projectile->byte_0x45_69 = kind;
					projectile->id_0x18_24 = wizard->id_0x18_24;
					if (fireball)
						projectile->word_0x2C_44 = spell->word_0x2C_44;
					projectile->mana_0x8C_140 = spell->mana_0x8C_140;
					projectile->position_0x48_72.z = (int16_t)(projectile->position_0x48_72.z + wizard->box_0x4E_78[3]);
					projectile->word_0x1E_30 = wizard->word_0x1E_30;
					projectile->word_0x20_32 = wizard->word_0x20_32;
					if (fireball)
						projectile->word_0x1A_26 = wizard->playerData_0xA0_160[0x146];
					else
						projectile->word_0x1A_26 = 0xC8;
					wizard->playerData_0xA0_160[0x146] = 0;
					projectile->target_0x96_150 = wizard->position_0x48_72;
					MoveEntity_41EC0_42200((axis_3d*)&projectile->target_0x96_150, (uint16_t)wizard->word_0x1E_30, (uint16_t)wizard->word_0x20_32, fireball ? 0x4000 : 0x2800);
					QueueSound_55370_558A0(EntityIndex(projectile), -1, fireball ? 9 : 0x28);
				}
				SpellUseMana_55E80_563B0(spell, wizard);
				spell->byte_0x3D_61--;
			}
			spell->byte_0x3D_61 = 0;
			spell->word_0x30_48--;
			return;
		}
		spell->word_0x30_48 = 1;
	}
	spell->word_0x30_48--;
}

void SpellFireball_56090_565C0(Type_Mc1Entity* spell)
{
	SpellCastProjectile(spell, 0, 0, true);
}

void SpellModel3_56510_56A40(Type_Mc1Entity* spell)
{
	SpellCastProjectile(spell, 1, 0x0C, false);
}

void SpellModel16_57610_57B40(Type_Mc1Entity* spell)
{
	// the projectile model 0Ah: to the castle of the wizard (kind 2Bh), without one 1000h ahead on the ground (kind 2)
	if (spell->word_0x30_48 <= 0 || !spell->word_0x2A_42)
		return;
	Type_Mc1Entity* wizard = &Entities()[spell->word_0x2A_42];
	if (!SpellCanCast_55DD0_56300(spell, wizard))
	{
		spell->word_0x30_48 = 0;
		return;
	}
	if (spell->word_0x30_48 != spell->word_0x32_50)
		return;
	SpellUseMana_55E80_563B0(spell, wizard);
	Type_Mc1Entity* projectile = CreateEntity_373F0_377B0(&wizard->position_0x48_72, 9, 0x0A);
	if (!projectile)
		return;
	spell->word_0x30_48 = (int16_t)(spell->word_0x32_50 - 1);
	projectile->word_0x7E_126 = (int16_t)(projectile->word_0x7E_126 + wizard->word_0x7E_126);
	SpellFromHand_55EF0_56420(wizard, projectile);
	projectile->word_0x2C_44 = spell->word_0x2C_44;
	projectile->id_0x18_24 = wizard->id_0x18_24;
	projectile->position_0x48_72.z = (int16_t)(projectile->position_0x48_72.z + wizard->box_0x4E_78[3]);
	projectile->mana_0x8C_140 = spell->mana_0x8C_140;
	if (const uint16_t castle = (uint16_t)ReadS16(wizard->playerData_0xA0_160 + 0x32))
	{
		projectile->byte_0x44_68 = 0x0A;
		projectile->byte_0x45_69 = 0x2B;
		projectile->target_0x92_146 = castle;
	}
	else
	{
		projectile->target_0x96_150 = wizard->position_0x48_72;
		MoveEntity_41EC0_42200((axis_3d*)&projectile->target_0x96_150, (uint16_t)wizard->word_0x1E_30, 0, 0x1000);
		projectile->byte_0x44_68 = 3;
		projectile->byte_0x45_69 = 2;
		projectile->target_0x96_150.z = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&projectile->target_0x96_150);
	}
	projectile->word_0x1A_26 = wizard->playerData_0xA0_160[0x146];
	wizard->playerData_0xA0_160[0x146] = 0;
	projectile->word_0x1E_30 = wizard->word_0x1E_30;
	projectile->word_0x20_32 = wizard->word_0x20_32;
	QueueSound_55370_558A0(EntityIndex(projectile), -1, 0x0F);
}

// the projectiles of class 9

void RaiseByBox_524C0_52800(Type_Mc1Entity* entity)
{
	if (entity->model_0x41_65 != 2)
		entity->position_0x48_72.z = (int16_t)(entity->position_0x48_72.z + entity->box_0x4E_78[0]);
}

void CancelCast_46D20_47060(Type_Mc1Entity* projectile, int16_t keep)
{
	// the spell in the hand of the owner (2C4h of its player data): word_0x30 to 0, or one frame before the cast
	const int16_t spellIndex = ReadS16(Entities()[(int16_t)projectile->id_0x18_24].playerData_0xA0_160 + 0x2C4);
	if (!spellIndex)
		return;
	Type_Mc1Entity* spell = &Entities()[spellIndex];
	if (!keep)
		spell->word_0x30_48 = 0;
	else
		spell->word_0x30_48 = (int16_t)(spell->word_0x32_50 - 1);
}

int32_t CastlePlaceFree_12F70_12F70(const Type_Mc1Position* position)
{
	// no castle (class 3 model 2) within 800h over its box and no tile changed by hand in the 8 x 8 tiles before
	bool taken = false;
	for (Type_Mc1Entity* castle = mc1Settings_AE408_AE3F8->entityLists_0x8E1A_36378[21]; castle > Entities() && !taken; castle = castle->next_0x00_0)
	{
		if (castle->model_0x41_65 != 2)
			continue;
		const int32_t dx = (int16_t)(castle->position_0x48_72.x - position->x);
		if ((dx < 0 ? -dx : dx) > castle->box_0x4E_78[1] + 0x800)
			continue;
		const int32_t dy = (int16_t)(castle->position_0x48_72.y - position->y);
		if ((dy < 0 ? -dy : dy) > castle->box_0x4E_78[2] + 0x800)
			continue;
		taken = true;
	}
	const uint8_t x = (uint8_t)((position->x >> 8) - 8);
	uint8_t y = (uint8_t)((position->y >> 8) - 8);
	for (int row = 0; row < 8 && !taken; row++, y++)
	{
		uint8_t column = x;
		for (int i = 0; i < 8 && !taken; i++, column++)
			if (mapAngle_FC1E0_FC1D0[(y << 8) | column] & 0x80)
				taken = true;
	}
	return taken ? 0 : 1;
}

// the turn to the wanted yaw / pitch (word_0x22 / word_0x24) by the steps of the type, the speed to word_0x80
static void ProjectileSteer(Type_Mc1Entity* projectile)
{
	const uint8_t* params = projectile->typeParams_0x9C_156;
	projectile->word_0x1E_30 = (int16_t)((projectile->word_0x1E_30 + TurnStep_422A0_425E0((uint16_t)projectile->word_0x1E_30, (uint16_t)projectile->word_0x22_34,
		(uint16_t)ReadS16(params + 4), (uint16_t)ReadS16(params + 2))) & 0x7FF);
	projectile->word_0x20_32 = (int16_t)((projectile->word_0x20_32 + TurnStep_422A0_425E0((uint16_t)projectile->word_0x20_32, (uint16_t)projectile->word_0x24_36,
		(uint16_t)ReadS16(params + 8), (uint16_t)ReadS16(params + 6))) & 0x7FF);
}

static void ProjectileSpeedStep(Type_Mc1Entity* projectile)
{
	const int32_t difference = projectile->word_0x80_128 - projectile->word_0x7E_126;
	const int32_t sign = difference ? difference / (difference < 0 ? -difference : difference) : 0;
	projectile->word_0x7E_126 = (int16_t)(projectile->word_0x7E_126 + sign * 2);
}

static void ProjectileFly(Type_Mc1Entity* projectile, uint16_t yaw)
{
	featurePosition_AE454_AE444 = projectile->position_0x48_72;
	MoveEntity_41EC0_42200((axis_3d*)&featurePosition_AE454_AE444, yaw, (uint16_t)projectile->word_0x20_32, projectile->word_0x7E_126);
	CopyEntityPosition_41C70_41FB0(projectile, &featurePosition_AE454_AE444);
}

void TurnToEntity_52610_52950(Type_Mc1Entity* projectile, Type_Mc1Entity* target)
{
	projectile->word_0x22_34 = (int16_t)GetYawAngle_42150_42490((axis_3d*)&projectile->position_0x48_72, (axis_3d*)&target->position_0x48_72);
	projectile->word_0x24_36 = (int16_t)GetPitchAngle_42180_424C0(&projectile->position_0x48_72, &target->position_0x48_72);
	ProjectileSteer(projectile);
}

void ProjectileToPlace_53B50_53E90(Type_Mc1Entity* projectile)
{
	// to target_0x96 (the ground ahead), there the entity of byte_0x44 / byte_0x45 (the castle); the first frame checks
	// the place, else the cast is cancelled
	if (!(projectile->flags_0x10_16 & 2))
	{
		projectile->flags_0x10_16 |= 2;
		if (!CastlePlaceFree_12F70_12F70(&projectile->position_0x48_72))
		{
			CancelCast_46D20_47060(projectile, 0);
			MarkToFree_41E80_421C0(projectile);
		}
		return;
	}
	projectile->word_0x22_34 = (int16_t)GetYawAngle_42150_42490((axis_3d*)&projectile->position_0x48_72, (axis_3d*)&projectile->target_0x96_150);
	projectile->word_0x24_36 = (int16_t)GetPitchAngle_42180_424C0(&projectile->position_0x48_72, &projectile->target_0x96_150);
	ProjectileSteer(projectile);
	ProjectileSpeedStep(projectile);
	ProjectileFly(projectile, (uint16_t)projectile->word_0x1E_30);
	Type_Mc1Entity* target = &Entities()[projectile->target_0x92_146];
	bool done = false;
	bool blocked = false;
	if (EntitiesOverlap_11950_11950(projectile, target))
	{
		RaiseByBox_524C0_52800(target);
		CopyEntityPosition_41C70_41FB0(projectile, &target->position_0x48_72);
		done = true;
	}
	else
	{
		const int16_t ground = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&projectile->position_0x48_72);
		featurePosition_AE454_AE444.z = ground;
		if (ground > projectile->position_0x48_72.z)
			done = true;
		else if (--projectile->life_0x0C_12 < 0)
			done = true;
		else if (!CastlePlaceFree_12F70_12F70(&projectile->position_0x48_72))
			done = blocked = true;
	}
	if (!done)
		return;
	if (blocked)
		ProjectileFly(projectile, (uint16_t)((projectile->word_0x1E_30 + 0x400) & 0x7FF));
	if (Type_Mc1Entity* created = CreateEntity_373F0_377B0(&projectile->position_0x48_72, (int8_t)projectile->byte_0x44_68, (int8_t)projectile->byte_0x45_69))
	{
		created->id_0x18_24 = projectile->id_0x18_24;
		MarkToFree_41E80_421C0(projectile);
	}
}

void ProjectileToCastle_53980_53CC0(Type_Mc1Entity* projectile)
{
	// to the target (the castle of the wizard), there the entity of byte_0x44 / byte_0x45; without a target 53B50
	if (!projectile->target_0x92_146)
	{
		ProjectileToPlace_53B50_53E90(projectile);
		return;
	}
	Type_Mc1Entity* target = &Entities()[projectile->target_0x92_146];
	TurnToEntity_52610_52950(projectile, target);
	ProjectileSpeedStep(projectile);
	ProjectileFly(projectile, (uint16_t)projectile->word_0x1E_30);
	bool done = false;
	if (EntitiesOverlap_11950_11950(projectile, target))
	{
		CopyEntityPosition_41C70_41FB0(projectile, &target->position_0x48_72);
		done = true;
	}
	else
	{
		const int16_t ground = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&projectile->position_0x48_72);
		featurePosition_AE454_AE444.z = ground;
		if (ground > projectile->position_0x48_72.z || --projectile->life_0x0C_12 < 0)
			done = true;
	}
	if (!done)
		return;
	Type_Mc1Entity* owner = &Entities()[(int16_t)projectile->id_0x18_24];
	if (projectile->byte_0x44_68 == 3 && (uint16_t)ReadS16(owner->playerData_0xA0_160 + 0x32))
	{
		MarkToFree_41E80_421C0(projectile);
		return;
	}
	if (Type_Mc1Entity* created = CreateEntity_373F0_377B0(&projectile->position_0x48_72, (int8_t)projectile->byte_0x44_68, (int8_t)projectile->byte_0x45_69))
	{
		created->id_0x18_24 = projectile->id_0x18_24;
		MarkToFree_41E80_421C0(projectile);
		return;
	}
	CancelCast_46D20_47060(owner, 0);
}

void LowerByBox_524E0_52820(Type_Mc1Entity* entity)
{
	if (entity->model_0x41_65 != 2)
		entity->position_0x48_72.z = (int16_t)(entity->position_0x48_72.z - entity->box_0x4E_78[0]);
}

void AimAt_52500_52840(Type_Mc1Entity* projectile, Type_Mc1Entity* target)
{
	// the wanted yaw and pitch to the middle of the target
	RaiseByBox_524C0_52800(target);
	projectile->word_0x22_34 = (int16_t)GetYawAngle_42150_42490((axis_3d*)&projectile->position_0x48_72, (axis_3d*)&target->position_0x48_72);
	projectile->word_0x24_36 = (int16_t)GetPitchAngle_42180_424C0(&projectile->position_0x48_72, &target->position_0x48_72);
	LowerByBox_524E0_52820(target);
}

void HomeTo_52550_52890(Type_Mc1Entity* projectile, Type_Mc1Entity* target)
{
	RaiseByBox_524C0_52800(target);
	TurnToEntity_52610_52950(projectile, target);
	LowerByBox_524E0_52820(target);
}

//!!!!! MC1 only: HW 52A00 differs, not checked
void CountHit_526C0(Type_Mc1Entity* projectile, Type_Mc1Entity* hit, Type_Mc1Entity* target)
{
	// the shots (157h) and the hits of the target (15Bh) of the player wizard, by the model of the projectile
	const uint8_t model = projectile->model_0x41_65;
	if (!(model <= 1 || model == 3 || (model >= 7 && model <= 9) || model == 0x13))
		return;
	Type_Mc1Entity* owner = &Entities()[(int16_t)projectile->id_0x18_24];
	if (owner->class_0x40_64 != 3 || owner->model_0x41_65 != 0)
		return;
	uint8_t* playerData = owner->playerData_0xA0_160;
	WriteS32(playerData + 0x157, ReadS32(playerData + 0x157) + 1);
	if (!hit || hit <= Entities() || target <= Entities() || hit->id_0x18_24 != target->id_0x18_24 || !playerData)
		return;
	WriteS32(playerData + 0x15B, ReadS32(playerData + 0x15B) + 1);
}

// the aim value: the square of the miss by the angles at the distance
static uint32_t AimMiss(uint16_t yawMiss, uint16_t pitchMiss, int32_t distance)
{
	const int32_t a = (Maths::sin_DB750[512 + yawMiss] * distance) >> 16;
	const int32_t b = (Maths::sin_DB750[yawMiss] * distance * 4) >> 16;
	const int32_t c = (Maths::sin_DB750[512 + pitchMiss] * distance) >> 16;
	const int32_t d = (distance * Maths::sin_DB750[pitchMiss] * 4) >> 16;
	return (uint32_t)(a * a + b * b + c * c + d * d);
}

uint32_t AimValue_54A90_54FC0(Type_Mc1Entity* projectile, Type_Mc1Entity* target, uint16_t maxYaw, uint16_t maxPitch)
{
	// the target in the yaw / pitch limits and 1400h (to its middle), else FFFFFFFFh
	RaiseByBox_524C0_52800(target);
	const uint16_t yawMiss = (uint16_t)AngleDistance_42210_42550((uint16_t)projectile->word_0x1E_30,
		(uint16_t)GetYawAngle_42150_42490((axis_3d*)&projectile->position_0x48_72, (axis_3d*)&target->position_0x48_72));
	if (yawMiss > maxYaw)
	{
		LowerByBox_524E0_52820(target);
		return 0xFFFFFFFF;
	}
	const uint16_t pitchMiss = (uint16_t)AngleDistance_42210_42550((uint16_t)projectile->word_0x20_32,
		GetPitchAngle_42180_424C0(&projectile->position_0x48_72, &target->position_0x48_72));
	if (pitchMiss > maxPitch)
	{
		LowerByBox_524E0_52820(target);
		return 0xFFFFFFFF;
	}
	const int32_t distance = (int32_t)Distance2D_423D0_42710(&projectile->position_0x48_72, &target->position_0x48_72);
	LowerByBox_524E0_52820(target);
	if (distance > 0x1400)
		return 0xFFFFFFFF;
	return AimMiss(yawMiss, pitchMiss, distance);
}

uint32_t AimValueCastle_54BD0_55100(Type_Mc1Entity* projectile, Type_Mc1Entity* target, uint16_t maxYaw, uint16_t maxPitch)
{
	// as 54A90 to the position of the castle
	const uint16_t yawMiss = (uint16_t)AngleDistance_42210_42550((uint16_t)projectile->word_0x1E_30,
		(uint16_t)GetYawAngle_42150_42490((axis_3d*)&projectile->position_0x48_72, (axis_3d*)&target->position_0x48_72));
	if (yawMiss > maxYaw)
		return 0xFFFFFFFF;
	const uint16_t pitchMiss = (uint16_t)AngleDistance_42210_42550((uint16_t)projectile->word_0x20_32,
		GetPitchAngle_42180_424C0(&projectile->position_0x48_72, &target->position_0x48_72));
	if (pitchMiss > maxPitch)
		return 0xFFFFFFFF;
	const int32_t distance = (int32_t)Distance2D_423D0_42710(&projectile->position_0x48_72, &target->position_0x48_72);
	if (distance > 0x1400)
		return 0xFFFFFFFF;
	return AimMiss(yawMiss, pitchMiss, distance);
}

// the best aimed entity of the list (the lower value) of the ones the filter takes
template <typename Filter, typename Value> static void BestAimed(Type_Mc1Entity* list, uint32_t& best, Type_Mc1Entity*& found, Filter filter, Value value)
{
	for (Type_Mc1Entity* entity = list; entity > Entities(); entity = entity->next_0x00_0)
	{
		if (!filter(entity))
			continue;
		const uint32_t aim = value(entity);
		if (aim < best)
		{
			found = entity;
			best = aim;
		}
	}
}

int32_t FindTarget_54520_548B0(Type_Mc1Entity* projectile)
{
	// the target of the projectile by its model: wizards and castles in the range, creatures (byte_0x3A), mana, buildings
	// HW 548B0: the model 10h has its own case, the yaw up to 100h
	const uint16_t maxYaw = mc1Variant == MC1_VARIANT_HW && projectile->model_0x41_65 == 0x10 ? 0x100 : 0x71;
	if (projectile->word_0x1A_26 > 0x10)
		projectile->word_0x1A_26 = 0x10;
	Type_Mc1Entity** lists = mc1Settings_AE408_AE3F8->entityLists_0x8E1A_36378;
	uint32_t best = 0xFFFFFFFF;
	Type_Mc1Entity* found = nullptr;
	bool alertWizard = false;
	const auto inRange = [projectile](Type_Mc1Entity* wizard, uint32_t range) {
		return wizard->id_0x18_24 != projectile->id_0x18_24 && !(wizard->flags_0x10_16 & 0x20)
			&& Distance3D_42340_42680(&wizard->position_0x48_72, &projectile->position_0x48_72) <= range;
	};
	const auto creature = [projectile](Type_Mc1Entity* entity) { return entity->id_0x18_24 != projectile->id_0x18_24 && entity->byte_0x3A_58; };
	const auto notOwn = [projectile](Type_Mc1Entity* entity) {
		return (int32_t)entity->owner_0x90_144 != (int32_t)(int16_t)projectile->id_0x18_24 && entity->byte_0x3A_58;
	};
	const auto aimWizard = [projectile, maxYaw](Type_Mc1Entity* wizard) {
		return wizard->model_0x41_65 == 2 ? AimValueCastle_54BD0_55100(projectile, wizard, maxYaw, 0x71) : AimValue_54A90_54FC0(projectile, wizard, maxYaw, 0x71);
	};
	const auto aim = [projectile, maxYaw](Type_Mc1Entity* entity) { return AimValue_54A90_54FC0(projectile, entity, maxYaw, 0x71); };
	const auto aimLow = [projectile](Type_Mc1Entity* entity) { return AimValue_54A90_54FC0(projectile, entity, 0x71, 0x200); };
	const uint32_t ownerRange = (uint32_t)(int32_t)ReadS16(Entities()[(int16_t)projectile->id_0x18_24].typeParams_0x9C_156 + 0x1C);
	switch (projectile->model_0x41_65)
	{
	case 0:
	case 3:
	case 4:
	case 0x10:
	case 0x12:
	case 0x13:
		BestAimed(lists[21], best, found, [&](Type_Mc1Entity* wizard) { return inRange(wizard, ownerRange); }, aimWizard);
		for (int list = 1; list <= 20; list++)
			BestAimed(lists[list], best, found, creature, aim);
		alertWizard = true;
		break;
	case 1:
		BestAimed(lists[22], best, found, notOwn, aim);
		BestAimed(lists[23], best, found, notOwn, aim);
		break;
	case 7:
	case 8:
	case 0x0B:
	case 0x0C:
		BestAimed(lists[21], best, found, [&](Type_Mc1Entity* wizard) { return inRange(wizard, ownerRange); }, aim);
		alertWizard = true;
		break;
	case 9:
	{
		const uint32_t range = (uint32_t)(projectile->word_0x80_128 * projectile->maxLife_0x08_8);
		BestAimed(lists[21], best, found, [&](Type_Mc1Entity* wizard) { return inRange(wizard, range); }, aimWizard);
		for (int list = 1; list <= 20; list++)
			BestAimed(lists[list], best, found, creature, aimLow);
		break;
	}
	case 0x11:
		BestAimed(lists[22], best, found, [](Type_Mc1Entity* mana) { return mana->byte_0x3A_58 != 0; }, aim);
		break;
	default:
		return 0;
	}
	if (!found)
		return 0;
	projectile->target_0x92_146 = EntityIndex(found);
	AimAt_52500_52840(projectile, found);
	if (alertWizard && found->class_0x40_64 == 3 && found->model_0x41_65 == 0)
		ResetWizardTimer_46520_46860(found);
	return 1;
}

// the first entity with the flag 8 overlapping the projectile in the tiles of its radius that the filter takes
template <typename Filter> static Type_Mc1Entity* FindOverlappingHit(Type_Mc1Entity* projectile, Filter filter)
{
	const int32_t x = (projectile->position_0x48_72.x + 0x80) >> 8;
	const int32_t y = (projectile->position_0x48_72.y + 0x80) >> 8;
	const int32_t search = StartSearch_11410_11410(0, (projectile->box_0x4E_78[1] + 0xFF) / 256);
	if (!search)
		return nullptr;
	Type_Mc1Entity* entities = Entities();
	int32_t dx;
	int32_t dy;
	while (GetNextSearchOffset_114B0_114B0(search, &dx, &dy) == 1)
	{
		const uint16_t cell = (uint16_t)((((dy + y) & 0xFF) << 8) + ((x + dx) & 0xFF));
		for (Type_Mc1Entity* other = &entities[(int16_t)mapEntityIndex_10C1E0_10C1D0[cell]]; other != entities; other = &entities[other->mapNext_0x14_20])
		{
			if (!(other->flags_0x10_16 & 8) || !filter(other) || !EntitiesOverlap_11950_11950(projectile, other))
				continue;
			EndSearch_11490_11490(search);
			return other;
		}
	}
	EndSearch_11490_11490(search);
	return nullptr;
}

Type_Mc1Entity* FindHit_11980_11980(Type_Mc1Entity* projectile)
{
	// of the kind byte_0x42 / byte_0x43 (FFh any), not of the id
	return FindOverlappingHit(projectile, [projectile](Type_Mc1Entity* other) {
		bool kind = (int8_t)projectile->byte_0x42_66 == -1;
		if (!kind && projectile->byte_0x42_66 == other->class_0x40_64)
			kind = (int8_t)projectile->byte_0x43_67 == -1 || projectile->byte_0x43_67 == other->model_0x41_65;
		return kind && projectile->id_0x18_24 != other->id_0x18_24;
	});
}

Type_Mc1Entity* FindHitMana_11AC0_11AC0(Type_Mc1Entity* projectile)
{
	// a mana ball or a building (class 10 models 27h, 28h, 2Dh) not of the id and not owned by it
	return FindOverlappingHit(projectile, [projectile](Type_Mc1Entity* other) {
		return other->class_0x40_64 == 0x0A && (other->model_0x41_65 == 0x27 || other->model_0x41_65 == 0x28 || other->model_0x41_65 == 0x2D)
			&& projectile->id_0x18_24 != other->id_0x18_24 && (int32_t)(int16_t)projectile->id_0x18_24 != (int32_t)other->owner_0x90_144;
	});
}

static void ProjectileEnd(Type_Mc1Entity* projectile, Type_Mc1Entity* hit, Type_Mc1Entity* target)
{
	// the entity of byte_0x44 / byte_0x45 (the explosion) in the place of the projectile
	Type_Mc1Entity* created = CreateEntity_373F0_377B0(&projectile->position_0x48_72, (int8_t)projectile->byte_0x44_68, (int8_t)projectile->byte_0x45_69);
	if (!created)
		return;
	CountHit_526C0(projectile, hit, target);
	created->id_0x18_24 = projectile->id_0x18_24;
	created->word_0x1E_30 = projectile->word_0x1E_30;
	created->word_0x20_32 = projectile->word_0x20_32;
	MarkToFree_41E80_421C0(projectile);
}

void Fireball_52B30_52E70(Type_Mc1Entity* projectile)
{
	// to the target (54520 in the first frame turns it at most 22h), a hit or the ground ends it; a shield (flag 8000h)
	// with the mana of a quarter of the projectile throws it back to the owner
	Type_Mc1Entity* target = &Entities()[projectile->target_0x92_146];
	if (projectile->target_0x92_146)
		HomeTo_52550_52890(projectile, target);
	else if (!(projectile->flags_0x10_16 & 2))
	{
		projectile->flags_0x10_16 |= 2;
		if (FindTarget_54520_548B0(projectile))
		{
			int32_t turn = AngleDistance_42210_42550((uint16_t)projectile->word_0x1E_30, (uint16_t)projectile->word_0x22_34);
			if ((int16_t)turn < 0)
				turn = 0;
			if ((int16_t)turn > 0x22)
				turn = 0x22;
			const int32_t direction = AngleDirection_42240_42580((uint16_t)projectile->word_0x1E_30, (uint16_t)projectile->word_0x22_34);
			projectile->word_0x1E_30 = (int16_t)(projectile->word_0x1E_30 + direction * turn);
			projectile->word_0x20_32 = projectile->word_0x24_36;
		}
		else
		{
			projectile->word_0x22_34 = projectile->word_0x1E_30;
			projectile->word_0x24_36 = projectile->word_0x20_32;
		}
	}
	Type_Mc1Position previous = projectile->position_0x48_72;
	ProjectileFly(projectile, (uint16_t)projectile->word_0x1E_30);
	Type_Mc1Entity* hit = FindHit_11980_11980(projectile);
	bool done = false;
	if (hit)
	{
		if (hit->flags_0x10_16 & 0x8000)
		{
			if (projectile->mana_0x8C_140 / 4 > hit->mana_0x8C_140)
				return;
			QueueSound_55370_558A0((int16_t)EntityIndex(hit), -1, 0x1C);
			Type_Mc1Entity* owner = &Entities()[(int16_t)projectile->id_0x18_24];
			hit->mana_0x8C_140 -= projectile->mana_0x8C_140 / 4;
			projectile->word_0x22_34 = (int16_t)((projectile->word_0x1E_30 + 0x400) & 0x7FF);
			const int32_t direction = AngleDirection_42240_42580(0, (uint16_t)projectile->word_0x20_32);
			const int32_t pitch = AngleDistance_42210_42550(0, (uint16_t)projectile->word_0x20_32);
			projectile->word_0x24_36 = (int16_t)(-(pitch * direction) & 0x7FF);
			projectile->word_0x20_32 = projectile->word_0x24_36;
			projectile->word_0x1E_30 = (int16_t)((uint16_t)projectile->word_0x22_34 + NextRand(projectile) % 0x5B - 0x2D);
			projectile->target_0x92_146 = EntityIndex(owner);
			projectile->id_0x18_24 = hit->id_0x18_24;
			projectile->life_0x0C_12 = projectile->maxLife_0x08_8;
			featurePosition_AE454_AE444 = hit->position_0x48_72;
			featurePosition_AE454_AE444.z = (int16_t)(featurePosition_AE454_AE444.z + hit->box_0x4E_78[3]);
			CopyEntityPosition_41C70_41FB0(projectile, &featurePosition_AE454_AE444);
			return;
		}
		RaiseByBox_524C0_52800(hit);
		CopyEntityPosition_41C70_41FB0(projectile, &hit->position_0x48_72);
		LowerByBox_524E0_52820(hit);
		done = true;
	}
	else
	{
		const int16_t ground = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&projectile->position_0x48_72);
		featurePosition_AE454_AE444.z = ground;
		if (ground > projectile->position_0x48_72.z)
		{
			CopyEntityPosition_41C70_41FB0(projectile, &previous);
			if (projectile->model_0x41_65 != 4 && GetTileTypeBit_11810_11810(&projectile->position_0x48_72) == 1)
			{
				// into the water: a splash
				if (Type_Mc1Entity* splash = CreateEntity_373F0_377B0(&projectile->position_0x48_72, 0x0A, 5))
					splash->id_0x18_24 = projectile->id_0x18_24;
				MarkToFree_41E80_421C0(projectile);
				return;
			}
			done = true;
		}
		else if (--projectile->life_0x0C_12 < 0)
			done = true;
	}
	if (done)
		ProjectileEnd(projectile, hit, target);
}

void Possession_52ED0_53210(Type_Mc1Entity* projectile)
{
	// to the target (a mana ball or a building not of the owner), a hit or the ground ends it
	Type_Mc1Entity* target = &Entities()[projectile->target_0x92_146];
	if (projectile->target_0x92_146)
		HomeTo_52550_52890(projectile, target);
	else if (!(projectile->flags_0x10_16 & 2))
	{
		projectile->flags_0x10_16 |= 2;
		if (FindTarget_54520_548B0(projectile))
		{
			projectile->word_0x1E_30 = projectile->word_0x22_34;
			projectile->word_0x20_32 = projectile->word_0x24_36;
		}
	}
	featurePosition_AE454_AE444 = projectile->position_0x48_72;
	MoveEntity_41EC0_42200((axis_3d*)&featurePosition_AE454_AE444, (uint16_t)projectile->word_0x1E_30, (uint16_t)projectile->word_0x20_32, projectile->word_0x7E_126);
	const int16_t ground = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&featurePosition_AE454_AE444);
	if (ground > featurePosition_AE454_AE444.z)
		featurePosition_AE454_AE444.z = ground;
	CopyEntityPosition_41C70_41FB0(projectile, &featurePosition_AE454_AE444);
	Type_Mc1Entity* hit = FindHitMana_11AC0_11AC0(projectile);
	bool done = false;
	if (hit)
	{
		RaiseByBox_524C0_52800(hit);
		CopyEntityPosition_41C70_41FB0(projectile, &hit->position_0x48_72);
		LowerByBox_524E0_52820(hit);
		done = true;
	}
	else
	{
		const int16_t height = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&projectile->position_0x48_72);
		featurePosition_AE454_AE444.z = height;
		if (height > projectile->position_0x48_72.z || --projectile->life_0x0C_12 < 0)
			done = true;
	}
	if (done)
		ProjectileEnd(projectile, hit, target);
}

void GuardShot_54180_544D0(Type_Mc1Entity* projectile)
{
	// the shot of a guard: a hit (checked before the step), the ground or the end of the life: the damage word_0x2C as
	// the mail 0 around and gone
	if (!(projectile->flags_0x10_16 & 2))
	{
		const int16_t sound = (int16_t)((NextRand(projectile) & 3) + 0x21);
		QueueSound_55370_558A0((int16_t)EntityIndex(projectile), -1, sound);
		projectile->flags_0x10_16 |= 2;
	}
	featurePosition_AE454_AE444 = projectile->position_0x48_72;
	MoveEntity_41EC0_42200((axis_3d*)&featurePosition_AE454_AE444, (uint16_t)projectile->word_0x1E_30, (uint16_t)projectile->word_0x20_32, projectile->word_0x7E_126);
	Type_Mc1Entity* hit = FindHit_11980_11980(projectile);
	const int16_t ground = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&featurePosition_AE454_AE444);
	bool end = ground > featurePosition_AE454_AE444.z;
	if (!end)
		end = projectile->life_0x0C_12-- == 0 || hit;
	if (!end)
	{
		CopyEntityPosition_41C70_41FB0(projectile, &featurePosition_AE454_AE444);
		return;
	}
	if (hit)
	{
		RaiseByBox_524C0_52800(hit);
		CopyEntityPosition_41C70_41FB0(projectile, &hit->position_0x48_72);
		LowerByBox_524E0_52820(hit);
	}
	SendMailAround_120B0_120B0(projectile, 0, (uint16_t)projectile->word_0x2C_44);
	MarkToFree_41E80_421C0(projectile);
}

// the casting of the spells (class 12): the countdown word_0x30 from word_0x32, the wizard of word_0x2A

static Type_Mc1Entity* SpellWizard(Type_Mc1Entity* spell)
{
	Type_Mc1Entity* wizard = &Entities()[spell->word_0x2A_42];
	return wizard > Entities() ? wizard : nullptr;
}

// the projectile of the spell from the hand of the wizard: the kind, the id, the height, the angles, the mana,
// the damage word_0x2C, the charge 146h of the player data
static Type_Mc1Entity* SpellProjectile(Type_Mc1Entity* spell, Type_Mc1Entity* wizard, int32_t model, uint8_t kind, bool addSpeed)
{
	Type_Mc1Entity* projectile = CreateEntity_373F0_377B0(&wizard->position_0x48_72, 9, model);
	if (!projectile)
		return nullptr;
	if (addSpeed)
		projectile->word_0x7E_126 = (int16_t)(projectile->word_0x7E_126 + wizard->word_0x7E_126);
	SpellFromHand_55EF0_56420(wizard, projectile);
	projectile->byte_0x44_68 = 0x0A;
	projectile->byte_0x45_69 = kind;
	projectile->id_0x18_24 = wizard->id_0x18_24;
	projectile->position_0x48_72.z = (int16_t)(projectile->position_0x48_72.z + wizard->box_0x4E_78[3]);
	projectile->word_0x1E_30 = wizard->word_0x1E_30;
	projectile->word_0x20_32 = wizard->word_0x20_32;
	projectile->mana_0x8C_140 = spell->mana_0x8C_140;
	projectile->word_0x2C_44 = spell->word_0x2C_44;
	return projectile;
}

static void TakeCharge(Type_Mc1Entity* projectile, Type_Mc1Entity* wizard)
{
	projectile->word_0x1A_26 = wizard->playerData_0xA0_160[0x146];
	wizard->playerData_0xA0_160[0x146] = 0;
}

// target_0x96 ahead of the wizard (by its pitch or level), on the ground or not
static void TargetAhead(Type_Mc1Entity* projectile, Type_Mc1Entity* wizard, int16_t distance, bool pitch, bool ground)
{
	projectile->target_0x96_150 = wizard->position_0x48_72;
	MoveEntity_41EC0_42200((axis_3d*)&projectile->target_0x96_150, (uint16_t)wizard->word_0x1E_30, pitch ? (uint16_t)wizard->word_0x20_32 : 0, distance);
	if (ground)
		projectile->target_0x96_150.z = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&projectile->target_0x96_150);
}

// the cast of SpellCastProjectile with the damage and the charge, 4000h ahead: per charge byte_0x3D (sound -1 none)
static void SpellCastCharged(Type_Mc1Entity* spell, int32_t model, uint8_t kind, int32_t sound)
{
	if (spell->word_0x30_48 <= 0)
		return;
	Type_Mc1Entity* wizard = SpellWizard(spell);
	if (wizard && SpellCanCast_55DD0_56300(spell, wizard))
	{
		while ((int8_t)spell->byte_0x3D_61 >= 0)
		{
			if (spell->word_0x30_48 == spell->word_0x32_50)
			{
				if (Type_Mc1Entity* projectile = SpellProjectile(spell, wizard, model, kind, true))
				{
					TakeCharge(projectile, wizard);
					TargetAhead(projectile, wizard, 0x4000, true, false);
					if (sound >= 0)
						QueueSound_55370_558A0(EntityIndex(projectile), -1, (uint8_t)sound);
				}
			}
			SpellUseMana_55E80_563B0(spell, wizard);
			spell->byte_0x3D_61--;
		}
		spell->byte_0x3D_61 = 0;
		spell->word_0x30_48--;
		return;
	}
	if (wizard)
		spell->word_0x30_48 = 1;
	spell->word_0x30_48--;
}

// one projectile at the start of the countdown; the mana used every step (or only at the start)
template <typename Init>
static void SpellCastOnce(Type_Mc1Entity* spell, bool manaOnlyAtStart, Init init)
{
	if (spell->word_0x30_48 <= 0)
		return;
	Type_Mc1Entity* wizard = SpellWizard(spell);
	if (wizard && SpellCanCast_55DD0_56300(spell, wizard))
	{
		if (spell->word_0x30_48 == spell->word_0x32_50)
			init(wizard);
		else if (manaOnlyAtStart)
		{
			spell->word_0x30_48--;
			return;
		}
		SpellUseMana_55E80_563B0(spell, wizard);
		spell->word_0x30_48--;
		return;
	}
	if (wizard)
		spell->word_0x30_48 = 1;
	spell->word_0x30_48--;
}

void SpellHeal_56270_567A0(Type_Mc1Entity* spell)
{
	// the wizard healed by 5 % a step while its mana lasts (the sound 19h at the start)
	if (spell->word_0x30_48 <= 0)
		return;
	Type_Mc1Entity* wizard = SpellWizard(spell);
	if (wizard)
	{
		if (SpellCanCast_55DD0_56300(spell, wizard) && wizard->life_0x0C_12 < wizard->maxLife_0x08_8 && wizard->mana_0x8C_140 >= spell->dword_0x88_136)
		{
			if (spell->word_0x30_48 == spell->word_0x32_50)
				QueueSound_55370_558A0(EntityIndex(wizard), -1, 0x19);
			wizard->life_0x0C_12 += wizard->maxLife_0x08_8 * 5 / 100;
			if (wizard->life_0x0C_12 > wizard->maxLife_0x08_8)
				wizard->life_0x0C_12 = wizard->maxLife_0x08_8;
			if (wizard->dword_0x84_132 < 0)
				wizard->dword_0x84_132 -= spell->dword_0x88_136;
			else
				wizard->dword_0x84_132 = -spell->dword_0x88_136;
			spell->word_0x30_48--;
			return;
		}
		spell->word_0x30_48 = 1;
	}
	spell->word_0x30_48--;
}

// the speed of the wizard 0Ch of the player data: 3 (2) times word_0x80 by the factor, smoke every 4th step
static void SpellSpeed(Type_Mc1Entity* spell, int32_t factor)
{
	if (spell->word_0x30_48 <= 0)
		return;
	Type_Mc1Entity* wizard = SpellWizard(spell);
	if (!wizard)
		return;
	uint8_t* playerData = wizard->playerData_0xA0_160;
	if (SpellCanCast_55DD0_56300(spell, wizard) && !ReadS16(playerData + 0x0E))
	{
		if (spell->word_0x30_48 == spell->word_0x32_50 && !(spell->flags_0x10_16 & 0x80))
		{
			spell->flags_0x10_16 |= 0x80;
			QueueSound_55370_558A0(EntityIndex(wizard), -1, 0x13);
		}
		if (spell->word_0x30_48 == spell->word_0x32_50 - 2 && (spell->flags_0x10_16 & 0x80))
			spell->flags_0x10_16 &= ~0x80u;
		if (spell->word_0x30_48 == spell->word_0x32_50)
			WriteS16(playerData + 0x0C, (int16_t)(factor * wizard->word_0x80_128 * 3));
		else
			WriteS16(playerData + 0x0C, (int16_t)(factor * wizard->word_0x80_128 * 2));
		wizard->word_0x7E_126 = ReadS16(playerData + 0x0C);
		if (!(spell->index_0x3F_63 & 3))
		{
			if (Type_Mc1Entity* smoke = CreateEntity_373F0_377B0(&wizard->position_0x48_72, 0x0A, 2))
			{
				smoke->id_0x18_24 = wizard->id_0x18_24;
				smoke->life_0x0C_12 <<= 2;
			}
		}
		SpellUseMana_55E80_563B0(spell, wizard);
	}
	else if (ReadS16(playerData + 0x0E))
		spell->word_0x30_48 = 1;
	if (--spell->word_0x30_48 == 0)
	{
		WriteS16(playerData + 0x0C, (int16_t)(factor * wizard->word_0x80_128));
		wizard->word_0x7E_126 = ReadS16(playerData + 0x0C);
		spell->flags_0x10_16 &= ~0x80u;
	}
}

void SpellSpeed_56380_568B0(Type_Mc1Entity* spell)
{
	SpellSpeed(spell, 1);
}

void SpellSlow_57F00_58410(Type_Mc1Entity* spell)
{
	// the speed backwards
	SpellSpeed(spell, -1);
}

// the flag of the wizard while the spell lasts
static void SpellWizardFlag(Type_Mc1Entity* spell, uint32_t flag)
{
	Type_Mc1Entity* wizard = SpellWizard(spell);
	if (!wizard || spell->word_0x30_48 <= 0)
		return;
	if (SpellCanCast_55DD0_56300(spell, wizard))
	{
		wizard->flags_0x10_16 |= flag;
		SpellUseMana_55E80_563B0(spell, wizard);
	}
	else
		spell->word_0x30_48 = 1;
	spell->word_0x30_48--;
}

void SpellShield_566C0_56BF0(Type_Mc1Entity* spell)
{
	SpellWizardFlag(spell, 0x4000);
}

void SpellManaOnly_56730_56C60(Type_Mc1Entity* spell)
{
	if (spell->word_0x30_48 <= 0)
		return;
	Type_Mc1Entity* wizard = SpellWizard(spell);
	if (wizard)
	{
		if (SpellCanCast_55DD0_56300(spell, wizard))
		{
			SpellUseMana_55E80_563B0(spell, wizard);
			spell->word_0x30_48--;
			return;
		}
		spell->word_0x30_48 = 1;
	}
	spell->word_0x30_48--;
}

void SpellModel2_567A0_56CD0(Type_Mc1Entity* spell)
{
	// the projectile model 2 (kind 0Fh) to the ground 1000h ahead; the mana only at the start
	SpellCastOnce(spell, true, [spell](Type_Mc1Entity* wizard) {
		if (Type_Mc1Entity* projectile = SpellProjectile(spell, wizard, 2, 0x0F, true))
		{
			TakeCharge(projectile, wizard);
			TargetAhead(projectile, wizard, 0x1000, false, true);
			QueueSound_55370_558A0(EntityIndex(projectile), -1, 9);
		}
	});
}

void SpellModel3Pitch_56950_56E80(Type_Mc1Entity* spell)
{
	// the projectile model 3 (kind 11h) 2800h ahead by the pitch
	SpellCastOnce(spell, false, [spell](Type_Mc1Entity* wizard) {
		if (Type_Mc1Entity* projectile = SpellProjectile(spell, wizard, 3, 0x11, true))
		{
			TakeCharge(projectile, wizard);
			TargetAhead(projectile, wizard, 0x2800, true, false);
			QueueSound_55370_558A0(EntityIndex(projectile), -1, 0x0F);
		}
	});
}

void SpellModel4_56AF0_57020(Type_Mc1Entity* spell)
{
	// the projectile model 4 (kind 9) to the ground 1000h ahead
	SpellCastOnce(spell, false, [spell](Type_Mc1Entity* wizard) {
		if (Type_Mc1Entity* projectile = SpellProjectile(spell, wizard, 4, 9, true))
		{
			TakeCharge(projectile, wizard);
			TargetAhead(projectile, wizard, 0x1000, false, true);
			QueueSound_55370_558A0(EntityIndex(projectile), -1, 0x0F);
		}
	});
}

void SpellModel5_56CA0_571D0(Type_Mc1Entity* spell)
{
	// the projectile model 5 (kind 0Bh) to the ground 1000h ahead
	SpellCastOnce(spell, false, [spell](Type_Mc1Entity* wizard) {
		if (Type_Mc1Entity* projectile = SpellProjectile(spell, wizard, 5, 0x0B, true))
		{
			TakeCharge(projectile, wizard);
			TargetAhead(projectile, wizard, 0x1000, false, true);
			QueueSound_55370_558A0(EntityIndex(projectile), -1, 0x0F);
		}
	});
}

void SpellTeleport_56E50_57380(Type_Mc1Entity* spell)
{
	// to the castle and back to the place of the cast (target_0x96, z 0 none), without a castle 4000h to a random yaw
	if (spell->word_0x30_48 <= 0)
		return;
	Type_Mc1Entity* wizard = SpellWizard(spell);
	if (!wizard)
		return;
	if (SpellCanCast_55DD0_56300(spell, wizard))
	{
		if (spell->word_0x30_48 == spell->word_0x32_50)
		{
			Type_Mc1Entity* castle = &Entities()[(uint16_t)ReadS16(wizard->playerData_0xA0_160 + 0x32)];
			if (castle > Entities())
			{
				if (!spell->target_0x96_150.z)
				{
					spell->target_0x96_150 = wizard->position_0x48_72;
					CopyEntityPosition_41C70_41FB0(wizard, &castle->position_0x48_72);
				}
				else
				{
					CopyEntityPosition_41C70_41FB0(wizard, &spell->target_0x96_150);
					spell->target_0x96_150.z = 0;
				}
			}
			else
			{
				spell->target_0x96_150 = wizard->position_0x48_72;
				NextRand(spell);
				MoveEntity_41EC0_42200((axis_3d*)&spell->target_0x96_150, (uint16_t)(spell->rand_0x04_4 & 0x7FF), 0, 0x4000);
				CopyEntityPosition_41C70_41FB0(wizard, &spell->target_0x96_150);
				spell->target_0x96_150.z = 0;
			}
			WriteS16(wizard->playerData_0xA0_160 + 0x0C, 0);
			QueueSound_55370_558A0(EntityIndex(wizard), -1, 0x16);
		}
		SpellUseMana_55E80_563B0(spell, wizard);
	}
	else
		spell->word_0x30_48 = 1;
	if (--spell->word_0x30_48 == 0)
		WriteS16(wizard->playerData_0xA0_160 + 0x0C, 0);
}

void SpellModel7_57040_57570(Type_Mc1Entity* spell)
{
	// the projectile model 7 (kind 1Ah) 2800h ahead by the pitch, its own speed
	SpellCastOnce(spell, false, [spell](Type_Mc1Entity* wizard) {
		if (Type_Mc1Entity* projectile = SpellProjectile(spell, wizard, 7, 0x1A, false))
		{
			TargetAhead(projectile, wizard, 0x2800, true, false);
			QueueSound_55370_558A0(EntityIndex(projectile), -1, 9);
		}
	});
}

void SpellInvisible_571B0_576E0(Type_Mc1Entity* spell)
{
	// the wizard hidden (flag 20h) while the spell lasts
	if (spell->word_0x30_48 <= 0)
		return;
	Type_Mc1Entity* wizard = &Entities()[spell->word_0x2A_42];
	if (wizard > Entities())
	{
		if (SpellCanCast_55DD0_56300(spell, wizard))
		{
			if (spell->word_0x30_48 == spell->word_0x32_50)
			{
				if (spell->playerData_0xA0_160) // port: null player data
					WriteS16(spell->playerData_0xA0_160 + 0x14B, 0);
				wizard->flags_0x10_16 |= 0x20;
			}
			else if (!(wizard->flags_0x10_16 & 0x20))
				spell->word_0x30_48 = 1;
			SpellUseMana_55E80_563B0(spell, wizard);
		}
		else
			spell->word_0x30_48 = 1;
	}
	if (--spell->word_0x30_48 == 0)
		wizard->flags_0x10_16 &= ~0x20u;
}

void SpellModel8_57250_57780(Type_Mc1Entity* spell)
{
	// the projectile model 8 (kind 19h, damage 7D0h) 4000h ahead by the pitch
	SpellCastOnce(spell, false, [spell](Type_Mc1Entity* wizard) {
		if (Type_Mc1Entity* projectile = SpellProjectile(spell, wizard, 8, 0x19, true))
		{
			projectile->word_0x2C_44 = 0x7D0;
			TakeCharge(projectile, wizard);
			TargetAhead(projectile, wizard, 0x4000, true, false);
			QueueSound_55370_558A0(EntityIndex(projectile), -1, 9);
		}
	});
}

void SpellFlag8000_573F0_57920(Type_Mc1Entity* spell)
{
	// the flag 8000h of the wizard while the spell lasts, cleared when the countdown ends
	Type_Mc1Entity* wizard = SpellWizard(spell);
	if (!wizard)
		return;
	if (spell->word_0x30_48 <= 0)
	{
		wizard->flags_0x10_16 &= ~0x8000u;
		return;
	}
	if (SpellCanCast_55DD0_56300(spell, wizard))
	{
		wizard->flags_0x10_16 |= 0x8000;
		SpellUseMana_55E80_563B0(spell, wizard);
	}
	else
		spell->word_0x30_48 = 1;
	spell->word_0x30_48--;
}

void SpellModel9_57470_579A0(Type_Mc1Entity* spell)
{
	// the projectile model 9 (kind 17h) 4000h ahead by the pitch
	SpellCastOnce(spell, false, [spell](Type_Mc1Entity* wizard) {
		if (Type_Mc1Entity* projectile = SpellProjectile(spell, wizard, 9, 0x17, true))
		{
			TakeCharge(projectile, wizard);
			TargetAhead(projectile, wizard, 0x4000, true, false);
			QueueSound_55370_558A0(EntityIndex(projectile), -1, 0x17);
		}
	});
}

void SpellModelB_57800_57D30(Type_Mc1Entity* spell)
{
	// the projectile model 0Bh (kind 24h): its damage 88h of the spell when the castle has as much in 88h, else 1
	SpellCastOnce(spell, false, [spell](Type_Mc1Entity* wizard) {
		if (Type_Mc1Entity* projectile = SpellProjectile(spell, wizard, 0x0B, 0x24, true))
		{
			projectile->word_0x2C_44 = 1;
			if (const uint16_t castle = (uint16_t)ReadS16(wizard->playerData_0xA0_160 + 0x32))
				if (Entities()[castle].dword_0x88_136 >= spell->dword_0x88_136)
					projectile->word_0x2C_44 = (int16_t)spell->dword_0x88_136;
			TakeCharge(projectile, wizard);
			TargetAhead(projectile, wizard, 0x4000, true, false);
			QueueSound_55370_558A0(EntityIndex(projectile), -1, 9);
		}
	});
}

void SpellModelC_579D0_57F00(Type_Mc1Entity* spell)
{
	// the projectile model 0Ch (byte_0x44 9, kind 9) 4000h ahead by the pitch
	SpellCastOnce(spell, false, [spell](Type_Mc1Entity* wizard) {
		if (Type_Mc1Entity* projectile = SpellProjectile(spell, wizard, 0x0C, 9, true))
		{
			projectile->byte_0x44_68 = 9;
			TakeCharge(projectile, wizard);
			TargetAhead(projectile, wizard, 0x4000, true, false);
			QueueSound_55370_558A0(EntityIndex(projectile), -1, 9);
		}
	});
}

void SpellModel11_57B80_580B0(Type_Mc1Entity* spell)
{
	SpellCastCharged(spell, 0x11, 0x36, 0x28);
}

void SpellModel10_57D40(Type_Mc1Entity* spell)
{
	// MC1: per charge
	SpellCastCharged(spell, 0x10, 0x35, 9);
}

void SpellModel10Hw_58270(Type_Mc1Entity* spell)
{
	// HW: one projectile 2800h ahead by the pitch
	SpellCastOnce(spell, false, [spell](Type_Mc1Entity* wizard) {
		if (Type_Mc1Entity* projectile = SpellProjectile(spell, wizard, 0x10, 0x35, true))
		{
			TakeCharge(projectile, wizard);
			TargetAhead(projectile, wizard, 0x2800, true, false);
			QueueSound_55370_558A0(EntityIndex(projectile), -1, 0x0F);
		}
	});
}

void SpellModel12_580A0_585B0(Type_Mc1Entity* spell)
{
	SpellCastCharged(spell, 0x12, 0x37, -1);
}

void SpellModel0_58240_58750(Type_Mc1Entity* spell)
{
	SpellCastCharged(spell, 0, 0, 9);
}

// the projectiles of class 9 flying by their speed to the target

// the entity index the original computes for a missing entity: (0 - entities) / A4h with the entities at DOS 26AF3h
static uint16_t IndexOf(const Type_Mc1Entity* entity)
{
	//!!!!! the original stores this garbage index for a missing hit (MC1 address, HW not checked)
	if (!entity)
		return (uint16_t)(int16_t)(-0x26AF3 / 0xA4);
	return (uint16_t)EntityIndex(entity);
}

// the first frame: the target found by 54520 / 548B0 (its yaw and pitch taken) or the own angles kept as the wanted ones
static void ProjectileStart(Type_Mc1Entity* projectile, bool keepOwn)
{
	if (projectile->flags_0x10_16 & 2)
		return;
	projectile->flags_0x10_16 |= 2;
	if (FindTarget_54520_548B0(projectile))
	{
		projectile->word_0x1E_30 = projectile->word_0x22_34;
		projectile->word_0x20_32 = projectile->word_0x24_36;
	}
	else if (keepOwn)
	{
		projectile->word_0x22_34 = projectile->word_0x1E_30;
		projectile->word_0x24_36 = projectile->word_0x20_32;
	}
}

enum class ProjectileEndKind { Explosion, WizardsOnly, Model26 };

// 52770 / 530C0 / 53DC0: homing (or 54520 in the first frame), the speed to word_0x80 by 2, a hit or the ground ends it;
// a shield (flag 8000h) with a quarter of the mana throws the kinds 1 / 11h (HW also 35h) back to the caster
static void ProjectileFlight(Type_Mc1Entity* projectile, ProjectileEndKind endKind, bool hw)
{
	bool done = false;
	Type_Mc1Entity* target = &Entities()[projectile->target_0x92_146];
	if (target > Entities())
		HomeTo_52550_52890(projectile, target);
	else
		ProjectileStart(projectile, true);
	const int32_t speedDiff = projectile->word_0x80_128 - projectile->word_0x7E_126;
	const int32_t sign = speedDiff ? speedDiff / (speedDiff < 0 ? -speedDiff : speedDiff) : 0;
	projectile->word_0x7E_126 = (int16_t)(projectile->word_0x7E_126 + sign * 2);
	featurePosition_AE454_AE444 = projectile->position_0x48_72;
	MoveEntity_41EC0_42200((axis_3d*)&featurePosition_AE454_AE444, (uint16_t)projectile->word_0x1E_30, (uint16_t)projectile->word_0x20_32, projectile->word_0x7E_126);
	CopyEntityPosition_41C70_41FB0(projectile, &featurePosition_AE454_AE444);
	Type_Mc1Entity* hit = FindHit_11980_11980(projectile);
	if (hit)
	{
		const uint8_t kind = projectile->byte_0x45_69;
		if ((hit->flags_0x10_16 & 0x8000) && projectile->mana_0x8C_140 / 4 <= hit->mana_0x8C_140 && projectile->byte_0x44_68 == 0x0A
			&& (kind == 1 || kind == 0x11 || (hw && kind == 0x35)))
		{
			QueueSound_55370_558A0(EntityIndex(hit), -1, 0x1C);
			Type_Mc1Entity* caster = &Entities()[(int16_t)projectile->id_0x18_24];
			hit->mana_0x8C_140 -= projectile->mana_0x8C_140 / 4;
			projectile->word_0x22_34 = (int16_t)((projectile->word_0x1E_30 + 0x400) & 0x7FF);
			const int32_t direction = AngleDirection_42240_42580(0, (uint16_t)projectile->word_0x20_32);
			const int32_t pitch = AngleDistance_42210_42550(0, (uint16_t)projectile->word_0x20_32);
			projectile->word_0x24_36 = (int16_t)(-(pitch * direction) & 0x7FF);
			projectile->word_0x20_32 = projectile->word_0x24_36;
			NextRand(projectile);
			projectile->word_0x1E_30 = (int16_t)((uint16_t)projectile->word_0x22_34 + projectile->rand_0x04_4 % 0x2D - 0x16);
			projectile->target_0x92_146 = (uint16_t)EntityIndex(caster);
			projectile->id_0x18_24 = hit->id_0x18_24;
			projectile->life_0x0C_12 = projectile->maxLife_0x08_8;
			featurePosition_AE454_AE444 = hit->position_0x48_72;
			featurePosition_AE454_AE444.z = (int16_t)(featurePosition_AE454_AE444.z + hit->box_0x4E_78[3]);
			CopyEntityPosition_41C70_41FB0(projectile, &featurePosition_AE454_AE444);
			return;
		}
		RaiseByBox_524C0_52800(hit);
		CopyEntityPosition_41C70_41FB0(projectile, &hit->position_0x48_72);
		LowerByBox_524E0_52820(hit);
		done = true;
	}
	else
	{
		const int16_t ground = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&projectile->position_0x48_72);
		featurePosition_AE454_AE444.z = ground;
		if (ground > projectile->position_0x48_72.z)
		{
			if (projectile->model_0x41_65 != 4 && GetTileTypeBit_11810_11810(&projectile->position_0x48_72) == 1)
			{
				if (Type_Mc1Entity* splash = CreateEntity_373F0_377B0(&projectile->position_0x48_72, 0x0A, 5))
					splash->id_0x18_24 = projectile->id_0x18_24;
				MarkToFree_41E80_421C0(projectile);
				return;
			}
			done = true;
		}
		else if (--projectile->life_0x0C_12 < 0)
			done = true;
	}
	if (!done)
		return;
	if (endKind == ProjectileEndKind::WizardsOnly && !(hit && hit->class_0x40_64 == 3 && (hit->model_0x41_65 == 0 || hit->model_0x41_65 == 1)))
	{
		CountHit_526C0(projectile, nullptr, target);
		MarkToFree_41E80_421C0(projectile);
		return;
	}
	Type_Mc1Entity* created = endKind == ProjectileEndKind::Model26
		? CreateEntity_373F0_377B0(&projectile->position_0x48_72, 0x0A, 0x26)
		: CreateEntity_373F0_377B0(&projectile->position_0x48_72, (int8_t)projectile->byte_0x44_68, (int8_t)projectile->byte_0x45_69);
	if (!created)
		return;
	CountHit_526C0(projectile, hit, target);
	if (projectile->byte_0x45_69 == 0x22)
		created->life_0x0C_12 = (uint16_t)projectile->word_0x2C_44;
	created->id_0x18_24 = projectile->id_0x18_24;
	created->word_0x1E_30 = projectile->word_0x1E_30;
	created->word_0x20_32 = projectile->word_0x20_32;
	created->target_0x92_146 = IndexOf(hit);
	created->word_0x2C_44 = projectile->word_0x2C_44;
	if (endKind == ProjectileEndKind::Model26)
	{
		created->byte_0x44_68 = projectile->byte_0x44_68;
		created->byte_0x45_69 = projectile->byte_0x45_69;
	}
	MarkToFree_41E80_421C0(projectile);
}

void ProjectileFlight_52770(Type_Mc1Entity* projectile)
{
	ProjectileFlight(projectile, ProjectileEndKind::Explosion, false);
}

void ProjectileFlightHw_52AB0(Type_Mc1Entity* projectile)
{
	ProjectileFlight(projectile, ProjectileEndKind::Explosion, true);
}

void ProjectileToWizard_530C0(Type_Mc1Entity* projectile)
{
	ProjectileFlight(projectile, ProjectileEndKind::WizardsOnly, false);
}

void ProjectileToWizardHw_53400(Type_Mc1Entity* projectile)
{
	ProjectileFlight(projectile, ProjectileEndKind::WizardsOnly, true);
}

void ProjectileModel26_53DC0(Type_Mc1Entity* projectile)
{
	ProjectileFlight(projectile, ProjectileEndKind::Model26, false);
}

void ProjectileModel26Hw_54100(Type_Mc1Entity* projectile)
{
	ProjectileFlight(projectile, ProjectileEndKind::Model26, true);
}

static void ProjectileFlightOfGame(Type_Mc1Entity* projectile)
{
	if (mc1Variant == MC1_VARIANT_HW)
		ProjectileFlightHw_52AB0(projectile);
	else
		ProjectileFlight_52770(projectile);
}

void ProjectileAction_53060_533A0(Type_Mc1Entity* projectile)
{
	ProjectileFlightOfGame(projectile);
}

void ProjectileSmoke_53070_533B0(Type_Mc1Entity* projectile)
{
	// a smoke behind it while it flies
	ProjectileFlightOfGame(projectile);
	if (!projectile->class_0x40_64)
		return;
	if (Type_Mc1Entity* smoke = CreateEntity_373F0_377B0(&projectile->position_0x48_72, 0x0A, 1))
	{
		smoke->flags_0x10_16 |= 0x80;
		smoke->id_0x18_24 = projectile->id_0x18_24;
		smoke->flags_0x10_16 |= 0x10000;
	}
}

void ProjectileFireSmokeHw_54600(Type_Mc1Entity* projectile)
{
	// HW action 17: a fire smoke (model 0) behind it while it flies
	ProjectileFlightHw_52AB0(projectile);
	if (!projectile->class_0x40_64)
		return;
	if (Type_Mc1Entity* smoke = CreateEntity_373F0_377B0(&projectile->position_0x48_72, 0x0A, 0))
	{
		smoke->flags_0x10_16 |= 0x80;
		smoke->id_0x18_24 = projectile->id_0x18_24;
		smoke->flags_0x10_16 |= 0x10000;
	}
}

void ProjectileToWizardAction_530B0_533F0(Type_Mc1Entity* projectile)
{
	if (mc1Variant == MC1_VARIANT_HW)
		ProjectileToWizardHw_53400(projectile);
	else
		ProjectileToWizard_530C0(projectile);
}

int32_t TileDistance_42300_42640(const Type_Mc1Position* from, const Type_Mc1Position* to)
{
	// the bigger of |dx| and |dy|
	int32_t dx = (int16_t)(to->x - from->x);
	int32_t dy = (int16_t)(to->y - from->y);
	if (dx < 0)
		dx = -dx;
	if (dy < 0)
		dy = -dy;
	return dx < dy ? dy : dx;
}

void ProjectileLife_54290_545E0(Type_Mc1Entity* projectile)
{
	const int32_t life = projectile->life_0x0C_12;
	projectile->life_0x0C_12 = life - 1;
	if (life < 0)
		MarkToFree_41E80_421C0(projectile);
}

void LightningStep_534C0_53800(Type_Mc1Entity* projectile)
{
	// one step of the lightning (removed from the map): straight on, a hit or the ground ends it
	bool done = false;
	if (&Entities()[projectile->target_0x92_146] <= Entities())
		ProjectileStart(projectile, true);
	featurePosition_AE454_AE444 = projectile->position_0x48_72;
	MoveEntity_41EC0_42200((axis_3d*)&featurePosition_AE454_AE444, (uint16_t)projectile->word_0x1E_30, (uint16_t)projectile->word_0x20_32, projectile->word_0x7E_126);
	projectile->position_0x48_72 = featurePosition_AE454_AE444;
	if (Type_Mc1Entity* hit = FindHit_11980_11980(projectile))
	{
		projectile->position_0x48_72 = hit->position_0x48_72;
		done = true;
	}
	else
	{
		const int16_t ground = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&projectile->position_0x48_72);
		featurePosition_AE454_AE444.z = ground;
		if (ground > projectile->position_0x48_72.z || --projectile->life_0x0C_12 < 0)
			done = true;
	}
	if (done)
		MarkToFree_41E80_421C0(projectile);
}

static int32_t RandomSide(Type_Mc1Entity* entity)
{
	NextRand(entity);
	return (int32_t)(entity->rand_0x04_4 % 0x9D) / 0x4F * 2 - 1;
}

// the zigzag of the lightning: one step back towards the limit, else a random side
static int16_t Zigzag(Type_Mc1Entity* entity, int16_t offset, int16_t limit)
{
	if (limit < offset)
		return (int16_t)(offset - 1);
	if (offset < -limit)
		return (int16_t)(offset + 1);
	return (int16_t)(offset + RandomSide(entity));
}

void Lightning_535E0_53920(Type_Mc1Entity* projectile)
{
	// the whole path flown at once by 534C0, then drawn by sprites D8h zigzagging sideways, the explosion at its end;
	// a shield (flag 8000h) of a wizard with a quarter of the mana takes a quarter of the damage
	const Type_Mc1Position start = projectile->position_0x48_72;
	projectile->word_0x7E_126 = projectile->word_0x80_128;
	RemoveFromMap_41DD0_42110(projectile);
	LightningStep_534C0_53800(projectile);
	int32_t steps = 1;
	const int16_t yaw = projectile->word_0x1E_30;
	const int16_t pitch = projectile->word_0x20_32;
	while (!(projectile->flags_0x10_16 & 0x400))
	{
		LightningStep_534C0_53800(projectile);
		steps++;
	}
	projectile->word_0x1E_30 = yaw;
	projectile->word_0x20_32 = pitch;
	Type_Mc1Position step = { 0, 0, 0 };
	int32_t count = steps * 8;
	const int32_t speed = projectile->word_0x7E_126 / 8;
	MoveEntity_41EC0_42200((axis_3d*)&step, (uint16_t)projectile->word_0x1E_30, (uint16_t)projectile->word_0x20_32, (int16_t)speed);
	int16_t sideX = 0;
	int16_t sideY = 0;
	featurePosition_AE454_AE444 = start;
	Type_Mc1Position position = start;
	while ((int16_t)count >= 0)
	{
		if (Type_Mc1Entity* spark = AllocEntity_372C0_37680())
		{
			spark->actionIndex_0x46_70 = 0x0E;
			spark->class_0x40_64 = 9;
			spark->model_0x41_65 = 9;
			spark->id_0x18_24 = projectile->id_0x18_24;
			spark->maxLife_0x08_8 = spark >= projectile ? 0 : -1;
			SetSprite_36FA0_37360(spark, 0xD8);
			AddToMap_41CF0_42030(spark, &position);
			ResetLife_36DE0_371A0(spark);
		}
		int16_t limit = (int16_t)((int16_t)count / 2);
		if (limit < 0)
			limit = 0;
		if (limit > 8)
			limit = 8;
		sideY = Zigzag(projectile, sideY, limit);
		sideX = Zigzag(projectile, sideX, limit); // computed, not used by the original
		featurePosition_AE454_AE444.x = (int16_t)(featurePosition_AE454_AE444.x + step.x);
		featurePosition_AE454_AE444.y = (int16_t)(featurePosition_AE454_AE444.y + step.y);
		featurePosition_AE454_AE444.z = (int16_t)(featurePosition_AE454_AE444.z + step.z);
		const int16_t side = (int16_t)((int16_t)speed / 4 * sideY);
		position = featurePosition_AE454_AE444;
		position.z = (int16_t)(featurePosition_AE454_AE444.z + side);
		MoveEntity_41EC0_42200((axis_3d*)&position, (uint16_t)((projectile->word_0x1E_30 + 0x200) & 0x7FF), 0, side);
		count--;
	}
	(void)sideX;
	Type_Mc1Entity* hit = FindHit_11980_11980(projectile);
	Type_Mc1Entity* target = &Entities()[projectile->target_0x92_146];
	Type_Mc1Entity* created = CreateEntity_373F0_377B0(&position, (int8_t)projectile->byte_0x44_68, (int8_t)projectile->byte_0x45_69);
	if (!created)
		return;
	CountHit_526C0(projectile, hit, target);
	created->id_0x18_24 = projectile->id_0x18_24;
	created->word_0x1E_30 = projectile->word_0x1E_30;
	created->word_0x20_32 = projectile->word_0x20_32;
	created->target_0x92_146 = IndexOf(hit);
	if (hit && (hit->flags_0x10_16 & 0x8000) && hit->class_0x40_64 == 3 && projectile->mana_0x8C_140 / 4 <= hit->mana_0x8C_140)
		created->word_0x2C_44 = (int16_t)((uint16_t)projectile->word_0x2C_44 / 4);
	else
		created->word_0x2C_44 = projectile->word_0x2C_44;
}

void ManaCollector_542B0_54640(Type_Mc1Entity* projectile)
{
	// to a new mana pile it touches: a sparkle (model 0Ch) and the entity of its kind there
	bool done = false;
	Type_Mc1Entity* target = &Entities()[projectile->target_0x92_146];
	if (target > Entities())
		HomeTo_52550_52890(projectile, target);
	else
		ProjectileStart(projectile, false);
	featurePosition_AE454_AE444 = projectile->position_0x48_72;
	MoveEntity_41EC0_42200((axis_3d*)&featurePosition_AE454_AE444, (uint16_t)projectile->word_0x1E_30, (uint16_t)projectile->word_0x20_32, projectile->word_0x7E_126);
	const int16_t ground = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&featurePosition_AE454_AE444);
	if (ground > featurePosition_AE454_AE444.z)
		featurePosition_AE454_AE444.z = ground;
	CopyEntityPosition_41C70_41FB0(projectile, &featurePosition_AE454_AE444);
	Type_Mc1Entity* mana = FindOverlappingMana_11C00_11C00(projectile);
	if (mana)
	{
		RaiseByBox_524C0_52800(mana);
		CopyEntityPosition_41C70_41FB0(projectile, &mana->position_0x48_72);
		LowerByBox_524E0_52820(mana);
		done = true;
	}
	else
	{
		const int16_t z = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&projectile->position_0x48_72);
		featurePosition_AE454_AE444.z = z;
		if (z > projectile->position_0x48_72.z || --projectile->life_0x0C_12 < 0)
			done = true;
	}
	if (!done)
		return;
	if (Type_Mc1Entity* sparkle = CreateEntity_373F0_377B0(&projectile->position_0x48_72, 0x0A, 0x0C))
	{
		CountHit_526C0(projectile, mana, target);
		sparkle->id_0x18_24 = projectile->id_0x18_24;
		sparkle->word_0x1E_30 = projectile->word_0x1E_30;
		sparkle->word_0x20_32 = projectile->word_0x20_32;
	}
	if (Type_Mc1Entity* created = CreateEntity_373F0_377B0(&projectile->position_0x48_72, (int8_t)projectile->byte_0x44_68, (int8_t)projectile->byte_0x45_69))
	{
		CountHit_526C0(projectile, mana, target);
		created->id_0x18_24 = projectile->id_0x18_24;
		created->word_0x1E_30 = projectile->word_0x1E_30;
		created->word_0x20_32 = projectile->word_0x20_32;
		MarkToFree_41E80_421C0(projectile);
	}
}

void ProjectileBecome_54480_54810(Type_Mc1Entity* projectile)
{
	// at once the entity of its kind (byte_0x44 / 45)
	Type_Mc1Entity* created = CreateEntity_373F0_377B0(&projectile->position_0x48_72, (int8_t)projectile->byte_0x44_68, (int8_t)projectile->byte_0x45_69);
	if (!created)
		return;
	created->id_0x18_24 = projectile->id_0x18_24;
	created->word_0x1E_30 = projectile->word_0x1E_30;
	created->word_0x20_32 = projectile->word_0x20_32;
	created->word_0x2C_44 = projectile->word_0x2C_44;
	MarkToFree_41E80_421C0(projectile);
}

// the triggers of the level (class 11): the models of the group (the id) created by 37440

int32_t TriggerWizardTouch_5A090_5A5A0(Type_Mc1Entity* trigger, int32_t touching)
{
	// every 8th step: a wizard of the model 0 touching it (or not) gets the sound 29h; on the ground
	if (trigger->index_0x3F_63 & 7)
		return 0;
	for (Type_Mc1Entity* wizard = mc1Settings_AE408_AE3F8->entityLists_0x8E1A_36378[21]; wizard > Entities(); wizard = wizard->next_0x00_0)
	{
		if (wizard->model_0x41_65 || EntitiesOverlap_11950_11950(trigger, wizard) != touching)
			continue;
		QueueSound_55370_558A0(EntityIndex(wizard), -1, 0x29);
		return 1;
	}
	trigger->position_0x48_72.z = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&trigger->position_0x48_72);
	return 0;
}

int32_t TriggerPlayerTouch_5A120_5A630(Type_Mc1Entity* trigger, int32_t touching)
{
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	for (uint16_t i = 0; i < state->playerCount_0x0A_10; i++)
		if (EntitiesOverlap_11950_11950(trigger, &Entities()[state->players_0x340B_13323[i].entityIndex_0x0A_10]) == touching)
			return 1;
	return 0;
}

// once when a wizard touches it (or leaves it)
static void TriggerOnce(Type_Mc1Entity* trigger, int32_t touching)
{
	if (!TriggerWizardTouch_5A090_5A5A0(trigger, touching))
		return;
	CreateLevelModels_37440_37800((int16_t)trigger->id_0x18_24, 1);
	MarkToFree_41E80_421C0(trigger);
}

// again after a pause of 10 steps without a player in the state
static void TriggerRepeat(Type_Mc1Entity* trigger, int32_t touching)
{
	if (!trigger->word_0x1A_26)
	{
		if (TriggerWizardTouch_5A090_5A5A0(trigger, touching))
		{
			CreateLevelModels_37440_37800((int16_t)trigger->id_0x18_24, 0);
			trigger->word_0x1A_26 = 0x0A;
		}
		return;
	}
	if (!TriggerPlayerTouch_5A120_5A630(trigger, touching))
		trigger->word_0x1A_26--;
}

static void TriggerCountdown(Type_Mc1Entity* trigger)
{
	if (!trigger->word_0x1A_26)
	{
		trigger->word_0x1A_26 = 0x10;
		return;
	}
	if (trigger->word_0x1A_26 != 1)
	{
		trigger->word_0x1A_26--;
		return;
	}
	QueueSound_55370_558A0(EntityIndex(trigger), -1, 0x29);
	CreateLevelModels_37440_37800((int16_t)trigger->id_0x18_24, 1);
	MarkToFree_41E80_421C0(trigger);
}

void TriggerNoCreatures_59E40_5A350(Type_Mc1Entity* trigger, int32_t model)
{
	// 16 steps after no creature of the model is left (-1: of the models 0-0Bh and 10h)
	Type_Mc1Entity** lists = mc1Settings_AE408_AE3F8->entityLists_0x8E1A_36378;
	if (model != -1)
	{
		if (lists[1 + model])
			return;
	}
	else
	{
		for (int32_t i = 0; i <= 0x10; i++)
			if ((i <= 0x0B || i == 0x10) && lists[1 + i])
				return;
	}
	TriggerCountdown(trigger);
}

void TriggerCastle_59B80_5A090(Type_Mc1Entity* trigger)
{
	// the player 0 with a castle and the flag 2 of its result: the group, the flag cleared, the sound 29h
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	Type_Mc1Entity* wizard = &Entities()[state->players_0x340B_13323[0].entityIndex_0x0A_10];
	uint8_t* playerData = wizard->playerData_0xA0_160;
	if (!ReadS16(playerData + 0x32))
		return;
	Type_Mc1Player_340B& player = state->players_0x340B_13323[ReadS16(playerData + 0x30)];
	if (!(player.levelResult_0x02_2 & 2))
		return;
	CreateLevelModels_37440_37800((int16_t)trigger->id_0x18_24, 0);
	MarkToFree_41E80_421C0(trigger);
	Type_Mc1Player_340B& owner = state->players_0x340B_13323[ReadS16(wizard->playerData_0xA0_160 + 0x30)];
	owner.levelResult_0x02_2 = (uint16_t)((owner.levelResult_0x02_2 & 0xFF00) | (owner.levelResult_0x02_2 & 0xFD));
	QueueSound_55370_558A0(EntityIndex(trigger), -1, 0x29);
}

void TriggerEnter_59A80_59F90(Type_Mc1Entity* trigger)
{
	TriggerOnce(trigger, 1);
}

void TriggerLeave_59AB0_59FC0(Type_Mc1Entity* trigger)
{
	TriggerOnce(trigger, 0);
}

void TriggerEnterRepeat_59AE0_59FF0(Type_Mc1Entity* trigger)
{
	TriggerRepeat(trigger, 1);
}

void TriggerLeaveRepeat_59B30_5A040(Type_Mc1Entity* trigger)
{
	TriggerRepeat(trigger, 0);
}

void TriggerEnter_59C40_5A150(Type_Mc1Entity* trigger)
{
	TriggerOnce(trigger, 1);
}

void TriggerLeave_59C70_5A180(Type_Mc1Entity* trigger)
{
	TriggerOnce(trigger, 0);
}

void TriggerEnterRepeat_59CA0_5A1B0(Type_Mc1Entity* trigger)
{
	TriggerRepeat(trigger, 1);
}

void TriggerLeaveRepeat_59CF0_5A200(Type_Mc1Entity* trigger)
{
	TriggerRepeat(trigger, 0);
}

void TriggerEnter_59D40_5A250(Type_Mc1Entity* trigger)
{
	TriggerOnce(trigger, 1);
}

void TriggerLeave_59D70_5A280(Type_Mc1Entity* trigger)
{
	TriggerOnce(trigger, 0);
}

void TriggerEnterRepeat_59DA0_5A2B0(Type_Mc1Entity* trigger)
{
	TriggerRepeat(trigger, 1);
}

void TriggerLeaveRepeat_59DF0_5A300(Type_Mc1Entity* trigger)
{
	TriggerRepeat(trigger, 0);
}

void TriggerNoCreatures00_59F60_5A470(Type_Mc1Entity* trigger)
{
	TriggerNoCreatures_59E40_5A350(trigger, 0);
}

void TriggerNoCreatures01_59F70_5A480(Type_Mc1Entity* trigger)
{
	TriggerNoCreatures_59E40_5A350(trigger, 1);
}

void TriggerNoCreatures02_59F80_5A490(Type_Mc1Entity* trigger)
{
	TriggerNoCreatures_59E40_5A350(trigger, 2);
}

void TriggerNoCreatures03_59F90_5A4A0(Type_Mc1Entity* trigger)
{
	TriggerNoCreatures_59E40_5A350(trigger, 3);
}

void TriggerNoCreatures04_59FA0_5A4B0(Type_Mc1Entity* trigger)
{
	TriggerNoCreatures_59E40_5A350(trigger, 4);
}

void TriggerNoCreatures05_59FB0_5A4C0(Type_Mc1Entity* trigger)
{
	TriggerNoCreatures_59E40_5A350(trigger, 5);
}

void TriggerNoCreatures06_59FC0_5A4D0(Type_Mc1Entity* trigger)
{
	TriggerNoCreatures_59E40_5A350(trigger, 6);
}

void TriggerNoCreatures07_59FD0_5A4E0(Type_Mc1Entity* trigger)
{
	TriggerNoCreatures_59E40_5A350(trigger, 7);
}

void TriggerNoCreatures08_59FE0_5A4F0(Type_Mc1Entity* trigger)
{
	TriggerNoCreatures_59E40_5A350(trigger, 8);
}

void TriggerNoCreatures09_59FF0_5A500(Type_Mc1Entity* trigger)
{
	TriggerNoCreatures_59E40_5A350(trigger, 9);
}

void TriggerNoCreatures0A_5A000_5A510(Type_Mc1Entity* trigger)
{
	TriggerNoCreatures_59E40_5A350(trigger, 10);
}

void TriggerNoCreatures0B_5A010_5A520(Type_Mc1Entity* trigger)
{
	TriggerNoCreatures_59E40_5A350(trigger, 11);
}

void TriggerNoCreatures0C_5A020_5A530(Type_Mc1Entity* trigger)
{
	TriggerNoCreatures_59E40_5A350(trigger, 12);
}

void TriggerNoCreatures0D_5A030_5A540(Type_Mc1Entity* trigger)
{
	TriggerNoCreatures_59E40_5A350(trigger, 13);
}

void TriggerNoCreatures0E_5A040_5A550(Type_Mc1Entity* trigger)
{
	TriggerNoCreatures_59E40_5A350(trigger, 14);
}

void TriggerNoCreatures0F_5A050_5A560(Type_Mc1Entity* trigger)
{
	TriggerNoCreatures_59E40_5A350(trigger, 15);
}

void TriggerNoCreatures10_5A060_5A570(Type_Mc1Entity* trigger)
{
	TriggerNoCreatures_59E40_5A350(trigger, 16);
}

void TriggerNoCreaturesAny_5A070_5A580(Type_Mc1Entity* trigger)
{
	TriggerNoCreatures_59E40_5A350(trigger, -1);
}
