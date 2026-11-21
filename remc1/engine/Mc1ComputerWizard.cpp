#include "Mc1ComputerWizard.h"

#include <cstring>

#include "Mc1Castle.h"
#include "Mc1Creatures.h"
#include "Mc1Entity.h"
#include "Mc1EntityTables.h"
#include "Mc1Game.h"
#include "Mc1Shared.h"
#include "Mc1Spells.h"
#include "Mc1Wizard.h"

// the wizard played by the computer (class 3 action 1): the state 19Fh of the player data chooses what it does,
// 136C0 chooses the next state; the spells cast by the delays 2D4h + 2 * spell, the opinion 1CCh of each player

Type_Mc1Entity* playerWizard_ACF50_ACF40 = nullptr;

// the delay before the spell is cast again (by the spell)
static const uint16_t spellDelays_90034_90034[24] = {
	2, 1, 0x20, 0x0A, 1, 0, 0, 4, 0x190, 0, 1, 0, 1, 0, 1, 1, 0x28, 0x258, 0, 1, 4, 2, 3, 4,
};

// the C library rand of the original (Watcom: 1 at the start, never seeded by MC1)
static uint32_t crtRandNext = 1;

static int32_t CrtRand()
{
	crtRandNext = crtRandNext * 1103515245 + 12345;
	return (int32_t)((crtRandNext >> 16) & 0x7FFF);
}

static Type_Mc1Entity* Entities()
{
	return mc1Entities;
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

static uint8_t* Data(Type_Mc1Entity* wizard)
{
	return wizard->playerData_0xA0_160;
}

static Type_Mc1Entity* Castle(Type_Mc1Entity* wizard)
{
	return &Entities()[(uint16_t)ReadS16(Data(wizard) + 0x32)];
}

static int32_t Sign(int32_t value)
{
	return value ? value / (value < 0 ? -value : value) : 0;
}

// the spell delay of the spell in the player data
static uint8_t* SpellDelay(Type_Mc1Entity* wizard, uint8_t spell)
{
	return Data(wizard) + 0x2D4 + spell * 2;
}

// the opinion of the wizard about the player of the colour (+0 the hate, +2 attacked)
static uint8_t* Opinion(Type_Mc1Entity* wizard, int16_t colour)
{
	return Data(wizard) + 0x1CC + colour * 8;
}

// the hate needed to attack the entity of the owner by its mana limit and the aggression 20Ah
static int32_t HateLimit(Type_Mc1Entity* wizard, Type_Mc1Entity* owner)
{
	return 0xC350 - owner->dword_0x88_136 / 10 * ReadS16(Data(wizard) + 0x20A) / 0xFF;
}

static int32_t FramesOfThought(Type_Mc1Entity* wizard)
{
	return 0x40 - ReadS16(Data(wizard) + 0x20E) / 4;
}

Type_Mc1Entity* SpellOf_14E60_14E60(Type_Mc1Entity* wizard, uint8_t spell)
{
	//!!!!! the original reads the spells of a target without player data from DOS memory 2A4h+
	if (!Data(wizard)) // port: no player data
		return nullptr;
	Type_Mc1Entity* entity = &Entities()[ReadS16(Data(wizard) + 0x2A4 + spell * 2)];
	return entity > Entities() ? entity : nullptr;
}

void FindPlayerWizard_16990_16990()
{
	// the last wizard of the model 0 (the player) in the list
	for (Type_Mc1Entity* wizard = Lists()[21]; wizard > Entities(); wizard = wizard->next_0x00_0)
		if (!wizard->model_0x41_65)
			playerWizard_ACF50_ACF40 = wizard;
}

int16_t TargetKind_15420_15420(Type_Mc1Entity* entity)
{
	return (int16_t)((((int16_t)(int8_t)entity->class_0x40_64) << 7) + (int8_t)entity->model_0x41_65 + entity->id_0x18_24);
}

int32_t IsTargetKind_15440_15440(Type_Mc1Entity* wizard, Type_Mc1Entity* entity)
{
	return (uint16_t)TargetKind_15420_15420(entity) == wizard->word_0x94_148;
}

static void SetTarget(Type_Mc1Entity* wizard, Type_Mc1Entity* target)
{
	wizard->target_0x92_146 = EntityIndex(target);
	wizard->word_0x94_148 = (uint16_t)TargetKind_15420_15420(target);
}

int32_t SpellCasting_15E60_15E60(Type_Mc1Entity* wizard, uint8_t spell)
{
	if (spell >= 0x18)
		return 0;
	Type_Mc1Entity* entity = SpellOf_14E60_14E60(wizard, spell);
	return entity && entity->word_0x30_48 > 0;
}

int32_t SpellCheaper_15E90_15E90(Type_Mc1Entity* wizard, uint8_t spell)
{
	//!!!!! the original reads 88h of a missing spell (DOS memory 88h)
	Type_Mc1Entity* entity = SpellOf_14E60_14E60(wizard, spell);
	if (!entity) // port: no spell entity
		return 0;
	return entity->dword_0x88_136 <= wizard->dword_0x88_136;
}

Type_Mc1Entity* SpellActive_16000_16000(Type_Mc1Entity* wizard, uint8_t spell)
{
	Type_Mc1Entity* entity = SpellOf_14E60_14E60(wizard, spell);
	return entity && entity->word_0x30_48 > 0 ? entity : nullptr;
}

int8_t HasAttackSpell_16920_16920(Type_Mc1Entity* wizard)
{
	static const uint8_t spells[6] = { 0, 0x0F, 8, 0x11, 0x14, 7 };
	for (uint8_t spell : spells)
		if (SpellOf_14E60_14E60(wizard, spell))
			return 1;
	return 0;
}

// the spell aimed within the turn limit by the skill 20Ch
static bool AimedWell(Type_Mc1Entity* wizard)
{
	const int32_t limit = ((0xFF - ReadS16(Data(wizard) + 0x20C)) / 4 + 0x14) * 0x800 / 0x168;
	return (int32_t)(uint16_t)AngleDistance_42210_42550((uint16_t)wizard->word_0x1E_30, (uint16_t)wizard->word_0x22_34) < limit;
}

int32_t CanCastSpell_15A00_15A00(Type_Mc1Entity* wizard, uint8_t spell)
{
	// the spell got, enough mana; by the spell: not casting, the delay over, aimed, the castle can grow
	Type_Mc1Entity* entity = SpellOf_14E60_14E60(wizard, spell);
	switch (spell)
	{
	case 2:
		return entity && wizard->mana_0x8C_140 >= entity->dword_0x88_136;
	case 4:
	case 12:
	case 14:
		return entity && !entity->word_0x30_48 && !ReadS16(SpellDelay(wizard, spell)) && wizard->mana_0x8C_140 >= entity->dword_0x88_136;
	case 3:
	case 7:
	case 8:
	case 17:
	case 20:
		return entity && !entity->word_0x30_48 && !ReadS16(SpellDelay(wizard, spell)) && wizard->mana_0x8C_140 >= entity->dword_0x88_136 && AimedWell(wizard);
	case 16:
		if (!entity)
			return 0;
		if (!ReadS16(Data(wizard) + 0x32))
			return !ReadS16(SpellDelay(wizard, spell)) && wizard->mana_0x8C_140 >= entity->dword_0x88_136;
		return !entity->word_0x30_48 && !ReadS16(SpellDelay(wizard, spell)) && CastleCanGrow_12D10_12D10(Castle(wizard))
			&& wizard->mana_0x8C_140 >= entity->dword_0x88_136 && AimedWell(wizard);
	case 0:
	case 11:
	case 13:
	case 15:
		return entity && !ReadS16(SpellDelay(wizard, spell)) && wizard->mana_0x8C_140 >= entity->dword_0x88_136 && AimedWell(wizard);
	default:
		if (spell >= 0x18)
			return 0;
		return entity && !ReadS16(SpellDelay(wizard, spell)) && wizard->mana_0x8C_140 >= entity->dword_0x88_136;
	}
}

static void StartSpell(Type_Mc1Entity* spell)
{
	spell->word_0x30_48 = spell->word_0x32_50;
}

int32_t CastSpell_155F0_155F0(Type_Mc1Entity* wizard, uint8_t spell)
{
	// the cast (word_0x30 from word_0x32) and the delay of the spell
	if (!CanCastSpell_15A00_15A00(wizard, spell))
		return 0;
	wizard->flags_0x10_16 &= ~0x100u;
	if (spell > 0x14)
		return 0;
	Type_Mc1Entity* entity = SpellOf_14E60_14E60(wizard, spell);
	uint8_t* playerData = Data(wizard);
	switch (spell)
	{
	case 0:
	case 15:
	{
		// the fireballs: a burst of at most 8, then a pause by the skill 20Eh
		if (!entity || ReadS16(playerData + 0x194) < 0 || wizard->mana_0x8C_140 < entity->dword_0x88_136)
			return 0;
		if ((uint16_t)AngleDistance_42210_42550((uint16_t)wizard->word_0x1E_30, (uint16_t)wizard->word_0x22_34) >= 0xAA)
			return 0;
		WriteS16(SpellDelay(wizard, spell), (int16_t)spellDelays_90034_90034[spell]);
		wizard->word_0x20_32 = (int16_t)GetPitchAngle_42180_424C0(&wizard->position_0x48_72, &Entities()[wizard->target_0x92_146].position_0x48_72);
		StartSpell(entity);
		WriteS16(playerData + 0x194, (int16_t)(ReadS16(playerData + 0x194) + 1));
		if (ReadS16(playerData + 0x194) >= 8)
			WriteS16(playerData + 0x194, (int16_t)((ReadS16(playerData + 0x20E) - 0xFF) / 8 - 1));
		return 1;
	}
	case 3:
	case 7:
	case 8:
	case 11:
	case 13:
	case 17:
	case 20:
		if (!entity || wizard->mana_0x8C_140 < entity->dword_0x88_136)
			return 0;
		if ((uint16_t)AngleDistance_42210_42550((uint16_t)wizard->word_0x1E_30, (uint16_t)wizard->word_0x22_34) >= 0xE3)
			return 0;
		WriteS16(SpellDelay(wizard, spell), (int16_t)spellDelays_90034_90034[spell]);
		wizard->word_0x20_32 = (int16_t)GetPitchAngle_42180_424C0(&wizard->position_0x48_72, &Entities()[wizard->target_0x92_146].position_0x48_72);
		StartSpell(entity);
		return 1;
	case 16:
		// the castle: grown, without one a new castle at target_0x96
		if (!entity || entity->word_0x30_48 || wizard->mana_0x8C_140 < entity->dword_0x88_136)
			return 0;
		if (ReadS16(playerData + 0x32))
		{
			StartSpell(entity);
			WriteS16(SpellDelay(wizard, spell), (int16_t)spellDelays_90034_90034[spell]);
			return 1;
		}
		if (Type_Mc1Entity* castle = CreateEntity_373F0_377B0(&wizard->target_0x96_150, 3, 2))
		{
			castle->id_0x18_24 = wizard->id_0x18_24;
			WriteS16(playerData + 0x32, (int16_t)EntityIndex(castle));
		}
		return 1;
	case 2:
		if (!entity || entity->word_0x30_48 || wizard->mana_0x8C_140 < entity->dword_0x88_136)
			return 0;
		StartSpell(entity);
		WriteS16(SpellDelay(wizard, spell), (int16_t)spellDelays_90034_90034[spell]);
		return 1;
	case 1:
	case 4:
	case 5:
	case 14:
	case 6:
	case 9:
	case 10:
	case 12:
		if (!entity || wizard->mana_0x8C_140 < entity->dword_0x88_136)
			return 0;
		StartSpell(entity);
		WriteS16(SpellDelay(wizard, spell), (int16_t)spellDelays_90034_90034[spell]);
		return 1;
	default:
		return 0;
	}
}

// to the target within the range (far than the speed of 15E60 / 155F0 by 2, near: stopped, 0Eh 1)
int32_t ApproachTarget_15470_15470(Type_Mc1Entity* wizard, Type_Mc1Entity* target, int32_t nearDistance, int32_t farDistance)
{
	uint8_t* playerData = Data(wizard);
	WriteS16(playerData + 0x0E, 0);
	int32_t distance = (int32_t)Distance3D_42340_42680(&wizard->position_0x48_72, target ? &target->position_0x48_72 : &wizard->position_0x48_72);
	if (!target)
		distance = (int32_t)Distance2D_423D0_42710(&wizard->position_0x48_72, &wizard->target_0x96_150);
	if (distance <= nearDistance)
	{
		WriteS16(playerData + 0x0C, 0);
		WriteS16(playerData + 0x0E, 1);
		return 1;
	}
	if (SpellCasting_15E60_15E60(wizard, 2))
		return 0;
	if (distance > farDistance && CanCastSpell_15A00_15A00(wizard, 2))
	{
		CastSpell_155F0_155F0(wizard, 2);
		return 0;
	}
	WriteS16(playerData + 0x0C, wizard->word_0x80_128);
	WriteS16(playerData + 0x0E, 1);
	return 0;
}

// the height kept 200h over the target by the climb of the type
static void KeepHeight(Type_Mc1Entity* wizard, int16_t targetZ)
{
	const int32_t sign = Sign(wizard->position_0x48_72.z - (targetZ + 0x200));
	wizard->position_0x48_72.z = (int16_t)(wizard->position_0x48_72.z + ReadS16(wizard->typeParams_0x9C_156 + 0x0E) * sign);
}

static Type_Mc1Entity* Target(Type_Mc1Entity* wizard)
{
	return &Entities()[wizard->target_0x92_146];
}

int32_t AttackWithSpell10_13800_13800(Type_Mc1Entity* wizard)
{
	Type_Mc1Entity* target = Target(wizard);
	if (!IsTargetKind_15440_15440(wizard, target))
		return 0;
	wizard->word_0x22_34 = (int16_t)GetYawAngle_42150_42490((axis_3d*)&wizard->position_0x48_72, (axis_3d*)&target->position_0x48_72);
	if (!ApproachTarget_15470_15470(wizard, target, 0x200, 0x800))
		return 1;
	if (CastSpell_155F0_155F0(wizard, 0x10))
		return 0;
	KeepHeight(wizard, target->position_0x48_72.z);
	return 1;
}

int32_t NoState_138E0_138E0(Type_Mc1Entity* /*wizard*/)
{
	return 0;
}

int32_t BuildCastle_138F0_138F0(Type_Mc1Entity* wizard)
{
	// to the place target_0x96 and the castle spell there
	wizard->word_0x22_34 = (int16_t)GetYawAngle_42150_42490((axis_3d*)&wizard->position_0x48_72, (axis_3d*)&wizard->target_0x96_150);
	if (!ApproachTarget_15470_15470(wizard, nullptr, 0x800, 0xC00))
		return 0;
	if (CastSpell_155F0_155F0(wizard, 0x10))
		return 0;
	KeepHeight(wizard, wizard->target_0x96_150.z);
	return 1;
}

int32_t GoToTarget_13990_13990(Type_Mc1Entity* wizard)
{
	Type_Mc1Entity* target = Target(wizard);
	if (!IsTargetKind_15440_15440(wizard, target))
		return 0;
	wizard->word_0x22_34 = (int16_t)GetYawAngle_42150_42490((axis_3d*)&wizard->position_0x48_72, (axis_3d*)&target->position_0x48_72);
	return ApproachTarget_15470_15470(wizard, target, 0x100, 0x800) ? 0 : 1;
}

static void SpeedSpellOrStop(Type_Mc1Entity* wizard)
{
	if (SpellCasting_15E60_15E60(wizard, 2))
		return;
	if (CanCastSpell_15A00_15A00(wizard, 2))
	{
		CastSpell_155F0_155F0(wizard, 2);
		return;
	}
	WriteS16(Data(wizard) + 0x0C, wizard->word_0x80_128);
	WriteS16(Data(wizard) + 0x0E, 1);
}

int32_t Wander_13A10_13A10(Type_Mc1Entity* wizard)
{
	SpeedSpellOrStop(wizard);
	return 0;
}

int32_t GoHome_13A70_13A70(Type_Mc1Entity* wizard)
{
	// to the castle (the spell 13h when far), healed by the spell 0Ch on the way
	Type_Mc1Entity* castle = Castle(wizard);
	if (castle > Entities())
	{
		if ((int32_t)Distance2DSquared_42410_42750(&wizard->position_0x48_72, &castle->position_0x48_72) > 0x6400000 && CastSpell_155F0_155F0(wizard, 0x13))
			return 1;
		CastSpell_155F0_155F0(wizard, 0x0C);
		if (!IsTargetKind_15440_15440(wizard, castle))
			return 0;
		wizard->word_0x22_34 = (int16_t)GetYawAngle_42150_42490((axis_3d*)&wizard->position_0x48_72, (axis_3d*)&castle->position_0x48_72);
		return ApproachTarget_15470_15470(wizard, castle, 0x100, 0x800) ? 0 : 1;
	}
	if (CanCastSpell_15A00_15A00(wizard, 0x0C))
		CastSpell_155F0_155F0(wizard, 0x0C);
	SpeedSpellOrStop(wizard);
	return 0;
}

int32_t Possess_13BA0_13BA0(Type_Mc1Entity* wizard)
{
	// the spell 3 at the mana ball, facing it the ball is owned
	Type_Mc1Entity* target = Target(wizard);
	if (!IsTargetKind_15440_15440(wizard, target))
		return 0;
	wizard->word_0x22_34 = (int16_t)GetYawAngle_42150_42490((axis_3d*)&wizard->position_0x48_72, (axis_3d*)&target->position_0x48_72);
	if (!ApproachTarget_15470_15470(wizard, target, 0x400, 0xC00))
		return 1;
	if (CastSpell_155F0_155F0(wizard, 3))
	{
		const uint16_t yaw = (uint16_t)GetYawAngle_42150_42490((axis_3d*)&wizard->position_0x48_72, (axis_3d*)&target->position_0x48_72);
		if ((uint16_t)AngleDistance_42210_42550((uint16_t)wizard->word_0x1E_30, yaw) < 0x1C)
			target->owner_0x90_144 = wizard->id_0x18_24;
	}
	KeepHeight(wizard, target->position_0x48_72.z);
	return 1;
}

// the attack spells chosen in this order (-1 none): low on mana (88h / 4) it waits for half of its limit
static int8_t ChooseAttackSpell(Type_Mc1Entity* wizard, bool spell0E)
{
	uint8_t* playerData = Data(wizard);
	int32_t quarter = wizard->dword_0x88_136 / 4;
	if (quarter > wizard->mana_0x8C_140)
		WriteS16(playerData + 0x196, 1);
	else if (ReadS16(playerData + 0x196))
	{
		quarter += 0x1770;
		if (quarter < wizard->dword_0x88_136)
		{
			if (quarter <= wizard->mana_0x8C_140)
				WriteS16(playerData + 0x196, 0);
		}
		else if (wizard->dword_0x88_136 / 2 <= wizard->mana_0x8C_140)
			WriteS16(playerData + 0x196, 0);
	}
	if (ReadS16(playerData + 0x196))
		return -1;
	// a castable spell is chosen; a cheaper one with its delay running ends the choice
	auto tryCast = [wizard](uint8_t spell, int8_t* chosen) {
		if (!SpellOf_14E60_14E60(wizard, spell))
			return false;
		if (CanCastSpell_15A00_15A00(wizard, spell))
		{
			*chosen = (int8_t)spell;
			return true;
		}
		if (SpellCheaper_15E90_15E90(wizard, spell) && !ReadS16(SpellDelay(wizard, spell)))
		{
			*chosen = -1;
			return true;
		}
		return false;
	};
	int8_t chosen = -1;
	if (tryCast(0x11, &chosen) || tryCast(8, &chosen))
		return chosen;
	if (spell0E && SpellActive_16000_16000(Target(wizard), 0x0E) && CrtRand() % 0xFF < ReadS16(playerData + 0x20C))
	{
		if (SpellOf_14E60_14E60(wizard, 0x0F) && CanCastSpell_15A00_15A00(wizard, 0x0F))
			return 0x0F;
		return -1;
	}
	if (tryCast(7, &chosen) || tryCast(0x14, &chosen) || tryCast(0, &chosen))
		return chosen;
	if (SpellOf_14E60_14E60(wizard, 0x0F) && CanCastSpell_15A00_15A00(wizard, 0x0F))
		return 0x0F;
	return -1;
}

int8_t ChooseAttackSpell_16030_16030(Type_Mc1Entity* wizard)
{
	return ChooseAttackSpell(wizard, true);
}

int8_t ChooseSpell_16310_16310(Type_Mc1Entity* wizard)
{
	return ChooseAttackSpell(wizard, false);
}

int32_t AttackThing_13CA0_13CA0(Type_Mc1Entity* wizard)
{
	// an attack spell at the target every frame of thought
	Type_Mc1Entity* target = Target(wizard);
	if (!IsTargetKind_15440_15440(wizard, target))
		return 0;
	wizard->word_0x22_34 = (int16_t)GetYawAngle_42150_42490((axis_3d*)&wizard->position_0x48_72, (axis_3d*)&target->position_0x48_72);
	if (!ApproachTarget_15470_15470(wizard, target, 0x800, 0xE00))
		return 1;
	if ((uint8_t)wizard->index_0x3F_63 % FramesOfThought(wizard))
		return 1;
	const int8_t spell = ChooseSpell_16310_16310(wizard);
	if (spell != -1 && CastSpell_155F0_155F0(wizard, (uint8_t)spell))
		return 0;
	KeepHeight(wizard, target->position_0x48_72.z);
	return 1;
}

int32_t AttackWizard_13DD0_13DD0(Type_Mc1Entity* wizard)
{
	// an attack spell at the target; a hit wizard is not attacked anymore (+2 of the opinion cleared)
	Type_Mc1Entity* target = Target(wizard);
	if (!IsTargetKind_15440_15440(wizard, target))
		return 0;
	wizard->word_0x22_34 = (int16_t)GetYawAngle_42150_42490((axis_3d*)&wizard->position_0x48_72, (axis_3d*)&target->position_0x48_72);
	if (!ApproachTarget_15470_15470(wizard, target, 0xC00, 0x1000))
		return 1;
	if (ReadS16(Data(wizard) + 0x194) < 0)
		return 1;
	const int8_t spell = ChooseAttackSpell_16030_16030(wizard);
	if (spell != -1 && CanCastSpell_15A00_15A00(wizard, (uint8_t)spell) && CastSpell_155F0_155F0(wizard, (uint8_t)spell))
	{
		if ((target->model_0x41_65 == 0 || target->model_0x41_65 == 1) && target->playerData_0xA0_160) // port: null player data
			WriteS16(Opinion(wizard, ReadS16(target->playerData_0xA0_160 + 0x30)) + 2, 0);
		return 0;
	}
	KeepHeight(wizard, target->position_0x48_72.z);
	return 1;
}

int32_t Attack_13DC0_13DC0(Type_Mc1Entity* wizard)
{
	return AttackWizard_13DD0_13DD0(wizard);
}

Type_Mc1Entity* NearestWizardOfModel_15260_15260(Type_Mc1Entity* entity, uint8_t model)
{
	// a wizard of another id of the model (FFh any, 1 and 4-FEh none)
	if (model != 0 && model != 2 && model != 3 && model != 0xFF)
		return nullptr;
	uint32_t best = 0xFFFFFFFF;
	Type_Mc1Entity* found = nullptr;
	for (Type_Mc1Entity* other = Lists()[21]; other > Entities(); other = other->next_0x00_0)
	{
		if (other->id_0x18_24 == entity->id_0x18_24)
			continue;
		if (model != 0xFF && (int8_t)other->model_0x41_65 != model)
			continue;
		const uint32_t distance = Distance2DSquared_42410_42750(&entity->position_0x48_72, &other->position_0x48_72);
		if (distance < best)
		{
			found = other;
			best = distance;
		}
	}
	return found;
}

int32_t FindCastlePlace_13F00_13F00(Type_Mc1Entity* wizard)
{
	// without a castle: the first place of the 4x4 grid of 4000h tiles (or its corner 1F00h in) with no castle
	// (model 2) within 3000h, in target_0x96; the entity 0 is the probe
	Type_Mc1Entity* probe = Entities();
	if (ReadS16(Data(wizard) + 0x32) || !SpellOf_14E60_14E60(wizard, 0x10) || !SpellCheaper_15E90_15E90(wizard, 0x10))
		return 0;
	probe->id_0x18_24 = wizard->id_0x18_24;
	const int32_t startX = wizard->position_0x48_72.x / 0x4000;
	const int32_t startY = wizard->position_0x48_72.y / 0x4000;
	for (int32_t y = startY; (int16_t)y < (int16_t)(startY + 4); y++)
	{
		for (int32_t x = startX; (int16_t)x < (int16_t)(startX + 4); x++)
		{
			const int32_t gridX = x & 3;
			const int32_t gridY = y & 3;
			probe->position_0x48_72.x = (int16_t)(gridX << 14);
			probe->position_0x48_72.y = (int16_t)(gridY << 14);
			Type_Mc1Entity* castle = NearestWizardOfModel_15260_15260(probe, 2);
			if (!castle || TileDistance_42300_42640(&castle->position_0x48_72, &probe->position_0x48_72) > 0x3000)
			{
				wizard->target_0x96_150 = probe->position_0x48_72;
				return 1;
			}
			probe->position_0x48_72.x = (int16_t)(((gridX << 6) + 0x1F) << 8);
			probe->position_0x48_72.y = (int16_t)(((gridY << 6) + 0x1F) << 8);
			castle = NearestWizardOfModel_15260_15260(probe, 2);
			if (!castle || TileDistance_42300_42640(&castle->position_0x48_72, &probe->position_0x48_72) > 0x3000)
			{
				wizard->target_0x96_150 = probe->position_0x48_72;
				return 1;
			}
		}
	}
	return 0;
}

int32_t ShouldGrowCastle_14120_14120(Type_Mc1Entity* wizard)
{
	Type_Mc1Entity* castle = Castle(wizard);
	if (castle <= Entities())
		return 0;
	Type_Mc1Entity* spell = SpellOf_14E60_14E60(wizard, 0x10);
	if (!spell || spell->word_0x30_48 || ReadS16(Data(wizard) + 0x2F4) || !CastleCanGrow_12D10_12D10(castle))
		return 0;
	if (wizard->dword_0x88_136 < spell->dword_0x88_136 || castle->actionIndex_0x46_70 != 4)
		return 0;
	SetTarget(wizard, castle);
	return 1;
}

Type_Mc1Entity* NearestOtherWizard_15340_15340(Type_Mc1Entity* entity, Type_Mc1Entity* wizard)
{
	// a wizard of the model 0 or 1 of neither id
	uint32_t best = 0xFFFFFFFF;
	Type_Mc1Entity* found = nullptr;
	for (Type_Mc1Entity* other = Lists()[21]; other > Entities(); other = other->next_0x00_0)
	{
		if (other->id_0x18_24 == entity->id_0x18_24 || other->id_0x18_24 == wizard->id_0x18_24)
			continue;
		if (other->model_0x41_65 != 0 && other->model_0x41_65 != 1)
			continue;
		const uint32_t distance = Distance2DSquared_42410_42750(&entity->position_0x48_72, &other->position_0x48_72);
		if (distance < best)
		{
			found = other;
			best = distance;
		}
	}
	return found;
}

Type_Mc1Entity* NearestOtherCastle_153B0_153B0(Type_Mc1Entity* entity, Type_Mc1Entity* castle)
{
	// a castle (model 2) of neither id
	uint32_t best = 0xFFFFFFFF;
	Type_Mc1Entity* found = nullptr;
	for (Type_Mc1Entity* other = Lists()[21]; other > Entities(); other = other->next_0x00_0)
	{
		if (other->id_0x18_24 == entity->id_0x18_24 || other->id_0x18_24 == castle->id_0x18_24 || other->model_0x41_65 != 2)
			continue;
		const uint32_t distance = Distance2DSquared_42410_42750(&entity->position_0x48_72, &other->position_0x48_72);
		if (distance < best)
		{
			found = other;
			best = distance;
		}
	}
	return found;
}

Type_Mc1Entity* FindMana_15080_15080(Type_Mc1Entity* wizard)
{
	// the nearest mana ball (list 22) worth taking: of a hated owner near the castle, or not guarded by another
	// wizard or castle, or of no wizard
	Type_Mc1Entity* castle = Castle(wizard);
	uint32_t best = 0xFFFFFFFF;
	Type_Mc1Entity* found = nullptr;
	for (Type_Mc1Entity* mana = Lists()[22]; mana > Entities(); mana = mana->next_0x00_0)
	{
		Type_Mc1Entity* owner = &Entities()[mana->owner_0x90_144];
		uint32_t distance;
		if (owner->class_0x40_64 == 3)
		{
			if (mana->owner_0x90_144 == (int32_t)(int16_t)wizard->id_0x18_24)
				continue;
			const int32_t hate = (uint16_t)ReadS16(Opinion(wizard, ReadS16(owner->playerData_0xA0_160 + 0x30)));
			if (HateLimit(wizard, owner) < hate)
				distance = Distance2DSquared_42410_42750(&castle->position_0x48_72, &mana->position_0x48_72);
			else
			{
				Type_Mc1Entity* guard = NearestOtherWizard_15340_15340(mana, wizard);
				if (!guard)
					continue;
				Type_Mc1Entity* otherCastle = castle > Entities() ? NearestOtherCastle_153B0_153B0(mana, castle) : NearestWizardOfModel_15260_15260(mana, 2);
				if (wizard->id_0x18_24 != guard->id_0x18_24 && (int32_t)Distance2DSquared_42410_42750(&mana->position_0x48_72, &guard->position_0x48_72) <= 0x1900000)
					continue;
				if (otherCastle && EntitiesOverlap_11950_11950(mana, otherCastle))
					continue;
				distance = Distance2DSquared_42410_42750(&wizard->position_0x48_72, &mana->position_0x48_72);
			}
		}
		else
			distance = Distance2DSquared_42410_42750(&wizard->position_0x48_72, &mana->position_0x48_72);
		if (distance < best)
		{
			found = mana;
			best = distance;
		}
	}
	return found;
}

int32_t ShouldPossessMana_14230_14230(Type_Mc1Entity* wizard)
{
	if (!SpellOf_14E60_14E60(wizard, 3))
		return 0;
	Type_Mc1Entity* castleSpell = SpellOf_14E60_14E60(wizard, 0x10);
	if (castleSpell && wizard->dword_0x88_136 > castleSpell->dword_0x88_136)
		return 0;
	Type_Mc1Entity* mana = FindMana_15080_15080(wizard);
	if (!mana)
		return 0;
	SetTarget(wizard, mana);
	return 1;
}

int32_t ShouldGoHome_14310_14310(Type_Mc1Entity* wizard)
{
	// below half of its life to the castle
	if (wizard->maxLife_0x08_8 / 2 <= wizard->life_0x0C_12)
		return 0;
	Type_Mc1Entity* castle = Castle(wizard);
	if (castle <= Entities())
		return 0;
	SetTarget(wizard, castle);
	return 1;
}

int32_t ShouldAttackCastle_143A0_143A0(Type_Mc1Entity* wizard)
{
	// the castle (model 2) of a hated owner away from its owner, or with less mana than the own castle
	if (!HasAttackSpell_16920_16920(wizard))
		return 0;
	if (!ReadS16(Data(wizard) + 0x32) && SpellOf_14E60_14E60(wizard, 0x10))
		return 0;
	uint32_t best = 0xFFFFFFFF;
	Type_Mc1Entity* found = nullptr;
	for (Type_Mc1Entity* other = Lists()[21]; other > Entities(); other = other->next_0x00_0)
	{
		if (other->id_0x18_24 == wizard->id_0x18_24 || other->model_0x41_65 != 2)
			continue;
		Type_Mc1Entity* owner = &Entities()[(int16_t)other->id_0x18_24];
		const int32_t hate = (uint16_t)ReadS16(Opinion(wizard, ReadS16(owner->playerData_0xA0_160 + 0x30)));
		bool candidate = false;
		if (HateLimit(wizard, owner) < hate && (int32_t)Distance2DSquared_42410_42750(&owner->position_0x48_72, &other->position_0x48_72) > 0x3840000
			&& !EntitiesOverlap_11950_11950(owner, other))
			candidate = true;
		if (!candidate)
		{
			Type_Mc1Entity* castle = Castle(wizard);
			candidate = (0xFF - ReadS16(Data(wizard) + 0x20A)) * 0x280 + other->mana_0x8C_140 < castle->mana_0x8C_140;
		}
		if (!candidate)
			continue;
		const uint32_t distance = Distance2DSquared_42410_42750(&wizard->position_0x48_72, &other->position_0x48_72);
		if (distance < best)
		{
			found = other;
			best = distance;
		}
	}
	if (!found)
		return 0;
	const int32_t range = ReadS16(wizard->typeParams_0x9C_156 + 0x1C) * ReadS16(wizard->typeParams_0x9C_156 + 0x1C);
	if ((int32_t)Distance2DSquared_42410_42750(&found->position_0x48_72, &wizard->position_0x48_72) >= range)
		return 0;
	SetTarget(wizard, found);
	return 1;
}

int32_t ShouldAttackWizard_145B0_145B0(Type_Mc1Entity* wizard)
{
	// a wizard (model 0 / 1) without the spell 0Ch running: attacked by it, hated, or without a castle and poorer
	if (!HasAttackSpell_16920_16920(wizard))
		return 0;
	if (!ReadS16(Data(wizard) + 0x32) && SpellOf_14E60_14E60(wizard, 0x10))
		return 0;
	uint32_t best = 0xFFFFFFFF;
	Type_Mc1Entity* found = nullptr;
	for (Type_Mc1Entity* other = Lists()[21]; other > Entities(); other = other->next_0x00_0)
	{
		if (other->id_0x18_24 == wizard->id_0x18_24 || (other->model_0x41_65 != 0 && other->model_0x41_65 != 1))
			continue;
		if (SpellActive_16000_16000(other, 0x0C))
			continue;
		uint8_t* otherData = other->playerData_0xA0_160;
		uint8_t* opinion = Opinion(wizard, ReadS16(otherData + 0x30));
		if (ReadS16(opinion + 2) == 1)
		{
			SetTarget(wizard, other);
			return 1;
		}
		if (HateLimit(wizard, other) > (int32_t)(uint16_t)ReadS16(opinion))
		{
			if (ReadS16(otherData + 0x32) || !SpellOf_14E60_14E60(other, 0x10))
				continue;
			if ((0xFF - ReadS16(Data(wizard) + 0x20A)) * 0x20 + other->mana_0x8C_140 >= wizard->mana_0x8C_140)
				continue;
		}
		const uint32_t distance = Distance2DSquared_42410_42750(&wizard->position_0x48_72, &other->position_0x48_72);
		if (distance < best)
		{
			found = other;
			best = distance;
		}
	}
	if (!found)
		return 0;
	const int32_t reach = ReadS16(wizard->typeParams_0x9C_156 + 0x1C) + 0x0A;
	if ((int32_t)Distance2DSquared_42410_42750(&found->position_0x48_72, &wizard->position_0x48_72) >= reach * reach)
		return 0;
	SetTarget(wizard, found);
	return 1;
}

int32_t ShouldAttackBalloon_147E0_147E0(Type_Mc1Entity* wizard)
{
	// a balloon (model 3) of a hated owner carrying mana, not at its castle
	if (!HasAttackSpell_16920_16920(wizard))
		return 0;
	uint32_t best = 0xFFFFFFFF;
	Type_Mc1Entity* found = nullptr;
	for (Type_Mc1Entity* other = Lists()[21]; other > Entities(); other = other->next_0x00_0)
	{
		if (other->id_0x18_24 == wizard->id_0x18_24 || other->model_0x41_65 != 3)
			continue;
		Type_Mc1Entity* owner = &Entities()[(int16_t)other->id_0x18_24];
		uint8_t* ownerData = owner->playerData_0xA0_160;
		const int32_t hate = (uint16_t)ReadS16(Opinion(wizard, ReadS16(ownerData + 0x30)));
		if (HateLimit(wizard, owner) >= hate)
			continue;
		if ((0x113 - ReadS16(Data(wizard) + 0x20A)) * 0x0A >= other->mana_0x8C_140)
			continue;
		if (EntitiesOverlap_11950_11950(other, &Entities()[(uint16_t)ReadS16(ownerData + 0x32)]))
			continue;
		const uint32_t distance = Distance2DSquared_42410_42750(&wizard->position_0x48_72, &other->position_0x48_72);
		if (distance < best)
		{
			found = other;
			best = distance;
		}
	}
	if (!found)
		return 0;
	const int32_t range = ReadS16(wizard->typeParams_0x9C_156 + 0x1C) * ReadS16(wizard->typeParams_0x9C_156 + 0x1C);
	if ((int32_t)Distance2DSquared_42410_42750(&found->position_0x48_72, &wizard->position_0x48_72) >= range)
		return 0;
	SetTarget(wizard, found);
	return 1;
}

int32_t ShouldAttackCreature_14B10_14B10(Type_Mc1Entity* wizard)
{
	// the creature (lists 1-20) with mana nearest to the castle (or the wizard)
	if (!HasAttackSpell_16920_16920(wizard))
		return 0;
	Type_Mc1Entity* castle = Castle(wizard);
	Type_Mc1Entity* from = castle != Entities() ? castle : wizard;
	uint32_t best = 0xFFFFFFFF;
	Type_Mc1Entity* found = nullptr;
	for (int32_t list = 0; list < 0x14; list++)
	{
		for (Type_Mc1Entity* other = Lists()[1 + list]; other > Entities(); other = other->next_0x00_0)
		{
			if (other->id_0x18_24 == wizard->id_0x18_24 || other->mana_0x8C_140 <= 0)
				continue;
			const uint32_t distance = Distance2DSquared_42410_42750(&from->position_0x48_72, &other->position_0x48_72);
			if (distance < best)
			{
				found = other;
				best = distance;
			}
		}
	}
	if (!found)
		return 0;
	SetTarget(wizard, found);
	return 1;
}

static int32_t SetState(Type_Mc1Entity* wizard, uint8_t state)
{
	Data(wizard)[0x19F] = state;
	return 1;
}

int32_t StateGrowCastle_14C40_14C40(Type_Mc1Entity* wizard)
{
	return SetState(wizard, 1);
}

int32_t StateBuildCastle_14C80_14C80(Type_Mc1Entity* wizard)
{
	return SetState(wizard, 3);
}

int32_t StatePossess_14CE0_14CE0(Type_Mc1Entity* wizard)
{
	return SetState(wizard, 6);
}

int32_t StateAttackCastle_14D00_14D00(Type_Mc1Entity* wizard)
{
	return SetState(wizard, 7);
}

int32_t StateAttackCreature_14D20_14D20(Type_Mc1Entity* wizard)
{
	return SetState(wizard, 0x0D);
}

int32_t StateAttackBalloon_14D40_14D40(Type_Mc1Entity* wizard)
{
	return SetState(wizard, 9);
}

int32_t StateAttackWizard_14D60_14D60(Type_Mc1Entity* wizard)
{
	return SetState(wizard, 8);
}

int32_t StateGoHome_14DA0_14DA0(Type_Mc1Entity* wizard)
{
	return SetState(wizard, 0x0B);
}

int32_t StateIdle_14DC0_14DC0(Type_Mc1Entity* wizard)
{
	// hurt with a castle: home, else wander
	Type_Mc1Entity* castle = Castle(wizard);
	if (wizard->life_0x0C_12 < wizard->maxLife_0x08_8 && castle > Entities())
	{
		SetTarget(wizard, castle);
		Data(wizard)[0x19F] = 0x0B;
	}
	else
		Data(wizard)[0x19F] = 0x0C;
	return 1;
}

int32_t ChooseState_136C0_136C0(Type_Mc1Entity* wizard)
{
	// the castle place, home when hurt; every frame of thought: grow the castle, attack, possess, wander
	if (FindCastlePlace_13F00_13F00(wizard))
		return StateBuildCastle_14C80_14C80(wizard);
	if (ShouldGoHome_14310_14310(wizard))
		return StateGoHome_14DA0_14DA0(wizard);
	if ((uint8_t)wizard->index_0x3F_63 % FramesOfThought(wizard))
		return 1;
	if (ShouldGrowCastle_14120_14120(wizard))
		return StateGrowCastle_14C40_14C40(wizard);
	if (ShouldAttackCastle_143A0_143A0(wizard))
		return StateAttackCastle_14D00_14D00(wizard);
	if (ShouldAttackWizard_145B0_145B0(wizard))
		return StateAttackWizard_14D60_14D60(wizard);
	if (ShouldAttackBalloon_147E0_147E0(wizard))
		return StateAttackBalloon_14D40_14D40(wizard);
	if (ShouldPossessMana_14230_14230(wizard))
		return StatePossess_14CE0_14CE0(wizard);
	if (ShouldAttackCreature_14B10_14B10(wizard))
		return StateAttackCreature_14D20_14D20(wizard);
	return StateIdle_14DC0_14DC0(wizard);
}

int32_t MoveComputerWizard_14EB0_14EB0(Type_Mc1Entity* wizard)
{
	// the height by the type, forward by word_0x7E and sideways by 10h of the player data (slowing by 4),
	// the speed to 0Ch by 16, the turn to word_0x22 by the reaction 20Eh within the limits of the type
	uint8_t* playerData = Data(wizard);
	const uint8_t* params = wizard->typeParams_0x9C_156;
	featurePosition_AE454_AE444 = wizard->position_0x48_72;
	ClampHeight_42000_42340(&featurePosition_AE454_AE444, (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&featurePosition_AE454_AE444),
		ReadS16(params + 0x0C), ReadS16(params + 0x0A), ReadS16(params + 0x0E));
	MoveEntity_41EC0_42200((axis_3d*)&featurePosition_AE454_AE444, (uint16_t)wizard->word_0x1E_30, 0, wizard->word_0x7E_126);
	MoveEntity_41EC0_42200((axis_3d*)&featurePosition_AE454_AE444, (uint16_t)(wizard->word_0x1E_30 + 0x200), 0, ReadS16(playerData + 0x10));
	WriteS16(playerData + 0x10, (int16_t)(ReadS16(playerData + 0x10) - Sign(ReadS16(playerData + 0x10)) * 4));
	CopyEntityPosition_41C70_41FB0(wizard, &featurePosition_AE454_AE444);
	wizard->word_0x7E_126 = (int16_t)(wizard->word_0x7E_126 + Sign(ReadS16(playerData + 0x0C) - wizard->word_0x7E_126) * 16);
	const uint16_t distance = (uint16_t)AngleDistance_42210_42550((uint16_t)wizard->word_0x1E_30, (uint16_t)wizard->word_0x22_34 & 0x7FF);
	const uint16_t divisor = (uint16_t)((0xFF - ReadS16(playerData + 0x20E)) / 16 + 8);
	int16_t turn = (int16_t)(distance / divisor);
	if (turn > ReadS16(params + 2))
		turn = ReadS16(params + 2);
	else if (turn < ReadS16(params + 4))
		turn = ReadS16(params + 4);
	const uint32_t yaw = (uint16_t)wizard->word_0x1E_30;
	const uint16_t wanted = (uint16_t)wizard->word_0x22_34;
	wizard->word_0x1E_30 = (int16_t)((turn * AngleDirection_42240_42580((uint16_t)wizard->word_0x1E_30, wanted) + yaw) & 0x7FF);
	if (yaw < wanted)
	{
		if ((uint16_t)wizard->word_0x1E_30 > wanted)
			wizard->word_0x1E_30 = (int16_t)wanted;
	}
	else if (yaw > wanted && wanted > (uint16_t)wizard->word_0x1E_30)
		wizard->word_0x1E_30 = (int16_t)wanted;
	return 1;
}

void SpellsOfPlayer_15EC0_15EC0(Type_Mc1Entity* wizard)
{
	// the spells the computer gets at the counts 274h (per spell of the player wizard): put in its free slot
	for (Type_Mc1Entity* player = Lists()[21]; player > Entities(); player = player->next_0x00_0)
	{
		if (player->model_0x41_65)
			continue;
		for (int32_t spell = 0; spell < 24; spell++)
		{
			uint8_t* data = Data(wizard) + spell * 2;
			if (ReadS16(data + 0x2A4) || ReadS16(data + 0x274) <= 0)
				continue;
			WriteS16(data + 0x274, (int16_t)(ReadS16(data + 0x274) - 1));
			if (ReadS16(data + 0x274))
				continue;
			Type_Mc1Entity* created = classes_96902_96902[12].models_4[spell].create_6(&wizard->position_0x48_72);
			if (!created)
				continue;
			for (int32_t slot = 0; slot < 24; slot++)
			{
				uint8_t* slotData = Data(wizard) + 0x214 + slot * 4;
				if (ReadS32(slotData))
					continue;
				created->flags_0x10_16 |= 1;
				created->word_0x2A_42 = EntityIndex(wizard);
				WriteS32(slotData, EntityIndex(created));
				break;
			}
		}
	}
}

Type_Mc1Entity* NearestOwnShot_16800_16800(Type_Mc1Entity* wizard)
{
	// the nearest class 9 entity (list 24) aimed at it within 1400h
	uint32_t best = 0xFFFFFFFF;
	Type_Mc1Entity* found = nullptr;
	for (Type_Mc1Entity* other = Lists()[24]; other > Entities(); other = other->next_0x00_0)
	{
		if (other->target_0x92_146 != (int32_t)(int16_t)wizard->id_0x18_24)
			continue;
		const uint32_t distance = Distance2DSquared_42410_42750(&wizard->position_0x48_72, &other->position_0x48_72);
		if (distance < best)
		{
			best = distance;
			found = other;
		}
	}
	return best < 0x1900000 ? found : nullptr;
}

void Dodge_16870_16870(Type_Mc1Entity* wizard, Type_Mc1Entity* /*shot*/)
{
	WriteS16(Data(wizard) + 0x10, 0x50);
}

void DefendFromShot_16890_16890(Type_Mc1Entity* wizard, Type_Mc1Entity* shot)
{
	// near (100000h): the shield (0Eh) or 4 against the models 0, 3, 10h; 4 against 4 and 9
	if ((int32_t)Distance2DSquared_42410_42750(&wizard->position_0x48_72, &shot->position_0x48_72) >= 0x100000)
		return;
	const uint8_t model = shot->model_0x41_65;
	if (model == 0 || model == 3 || model == 0x10)
	{
		if (CanCastSpell_15A00_15A00(wizard, 0x0E))
		{
			CastSpell_155F0_155F0(wizard, 0x0E);
			return;
		}
		if (CanCastSpell_15A00_15A00(wizard, 4))
			CastSpell_155F0_155F0(wizard, 4);
		return;
	}
	if ((model == 4 || model == 9) && CanCastSpell_15A00_15A00(wizard, 4))
		CastSpell_155F0_155F0(wizard, 4);
}

int32_t UpdateComputerWizard_132B0_132B0(Type_Mc1Entity* wizard)
{
	// the delays, the opinions, the spells, the damage (0 dead: action 2), the moves, the life and the mana
	uint8_t* playerData = Data(wizard);
	bool inCastle = false;
	FindPlayerWizard_16990_16990();
	if (ReadS16(playerData + 0x194) < 0)
		WriteS16(playerData + 0x194, (int16_t)(ReadS16(playerData + 0x194) + 1));
	for (uint8_t spell = 0; spell < 0x18; spell++)
	{
		uint8_t* delay = SpellDelay(wizard, spell);
		if (ReadS16(delay) > 0)
			WriteS16(delay, (int16_t)(ReadS16(delay) - 1));
	}
	for (int16_t colour = 0; colour < 8; colour++)
	{
		uint8_t* opinion = Opinion(wizard, colour);
		uint16_t hate = (uint16_t)ReadS16(opinion);
		if (hate < 0x601F)
		{
			hate = (uint16_t)(hate + ReadS16(playerData + 0x20A) + 1);
			if (hate > 0x601F)
				hate = 0x601F;
			WriteS16(opinion, (int16_t)hate);
		}
		if ((uint16_t)ReadS16(opinion) > 0x601F)
		{
			if (!ReadS16(opinion + 2))
				WriteS16(opinion, (int16_t)(ReadS16(opinion) - (0x100 - ReadS16(playerData + 0x20A))));
			if ((uint16_t)ReadS16(opinion) < 0x601F)
				WriteS16(opinion, 0x601F);
		}
	}
	UpdateSpellEntities_45C10_45F50(wizard);
	if (ReadS16(playerData + 0x32) && EntitiesOverlap_11950_11950(wizard, Castle(wizard)))
		inCastle = true;
	if (inCastle)
		WriteS16(playerData + 0x14B, 2);
	if (!ReadS16(playerData + 0x14B))
	{
		if (TakeWizardMails_46540_46880(wizard) == 2)
		{
			wizard->actionIndex_0x46_70 = 2;
			return 0;
		}
	}
	else
	{
		memset(wizard->mails_0x5A_90, 0, 0x24);
		WriteS16(playerData + 0x14B, (int16_t)(ReadS16(playerData + 0x14B) - 1));
	}
	MoveComputerWizard_14EB0_14EB0(wizard);
	if (playerData[0x146] < 0xC8)
		playerData[0x146]++;
	wizard->mana_0x8C_140 += wizard->dword_0x84_132;
	wizard->life_0x0C_12 += ReadS16(playerData + 0x155);
	if (wizard->life_0x0C_12 < -1)
		wizard->life_0x0C_12 = -1;
	if (wizard->life_0x0C_12 > wizard->maxLife_0x08_8)
		wizard->life_0x0C_12 = wizard->maxLife_0x08_8;
	if (const int32_t timer = ReadS32(playerData + 0x15F))
		WriteS32(playerData + 0x15F, timer - 1);
	if (inCastle || (wizard->flags_0x10_16 & 0x1000))
	{
		wizard->dword_0x84_132 = wizard->dword_0x88_136 / 200;
		WriteS16(playerData + 0x155, (int16_t)(wizard->maxLife_0x08_8 / 200));
		if (wizard->dword_0x84_132 < 1000)
			wizard->dword_0x84_132 = 1000;
		wizard->flags_0x10_16 &= ~0x1000u;
	}
	else
	{
		wizard->dword_0x84_132 = wizard->dword_0x88_136 / 2000;
		WriteS16(playerData + 0x155, (int16_t)(wizard->maxLife_0x08_8 / 500));
		if (wizard->dword_0x84_132 < 100)
			wizard->dword_0x84_132 = 100;
	}
	if (wizard->mana_0x8C_140 < 0)
		wizard->mana_0x8C_140 = 0;
	if (wizard->mana_0x8C_140 > wizard->dword_0x88_136)
		wizard->mana_0x8C_140 = wizard->dword_0x88_136;
	SpellsOfPlayer_15EC0_15EC0(wizard);
	if (!((uint8_t)wizard->index_0x3F_63 % FramesOfThought(wizard)))
	{
		if (Type_Mc1Entity* shot = NearestOwnShot_16800_16800(wizard))
		{
			Dodge_16870_16870(wizard, shot);
			DefendFromShot_16890_16890(wizard, shot);
		}
		if (wizard->life_0x0C_12 < wizard->maxLife_0x08_8)
			CastSpell_155F0_155F0(wizard, 1);
	}
	const int16_t ground = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&wizard->position_0x48_72);
	const uint8_t* params = wizard->typeParams_0x9C_156;
	if (wizard->position_0x48_72.z > ground + ReadS16(params + 0x0A))
		wizard->position_0x48_72.z = (int16_t)(ground + ReadS16(params + 0x0A));
	if (wizard->position_0x48_72.z < ground + ReadS16(params + 0x0C))
		wizard->position_0x48_72.z = (int16_t)(ground + ReadS16(params + 0x0C));
	return 1;
}

void ComputerWizard_13170_13170(Type_Mc1Entity* wizard)
{
	// the update, the state 19Fh, then the next state chosen
	UpdateComputerWizard_132B0_132B0(wizard);
	switch (Data(wizard)[0x19F])
	{
	case 0:
		ChooseState_136C0_136C0(wizard);
		break;
	case 1:
		AttackWithSpell10_13800_13800(wizard);
		break;
	case 2:
	case 5:
		NoState_138E0_138E0(wizard);
		break;
	case 3:
		BuildCastle_138F0_138F0(wizard);
		break;
	case 4:
		GoToTarget_13990_13990(wizard);
		break;
	case 6:
		Possess_13BA0_13BA0(wizard);
		break;
	case 7:
		AttackThing_13CA0_13CA0(wizard);
		break;
	case 8:
	case 9:
	case 13:
		Attack_13DC0_13DC0(wizard);
		break;
	case 11:
		GoHome_13A70_13A70(wizard);
		break;
	case 12:
		Wander_13A10_13A10(wizard);
		break;
	default:
		break;
	}
	ChooseState_136C0_136C0(wizard);
}
