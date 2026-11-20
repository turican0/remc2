#include "Mc1Spells.h"

#include <cstring>

#include "Mc1Entity.h"
#include "Mc1EntityTables.h"
#include "Mc1Game.h"
#include "Mc1NotPorted.h"
#include "Mc1Shared.h"
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
	sub_55370_558A0(0, ReadS16(wizard->playerData_0xA0_160 + 0x30), 0x1D);
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
					sub_55370_558A0(EntityIndex(projectile), -1, fireball ? 9 : 0x28);
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
	sub_55370_558A0(EntityIndex(projectile), -1, 0x0F);
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
