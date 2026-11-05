#pragma once
#ifndef MAIN_SPELLS
#define MAIN_SPELLS
#include <cstdint>
#include "BasicMini.h"
#include "global_types.h"

#pragma pack (push,1)
typedef struct {//lenght 26
	int32_t subSpellIndex_2;
	int32_t manaCost_6;
	int32_t maxManaLimit_A;
	int32_t xpos1_E;
	int32_t xpos2_0x12;
	int16_t hintText_0x16x;
	int16_t maxStage_0x18;
	int8_t life_0x1A;
	uint8_t fontType_0x1B;
}
type_SPELLS_BEGIN_BUFFER_str_sub;

typedef struct {//lenght 80
	int8_t levelCount_0;
	uint8_t isEnabled_1;
	type_SPELLS_BEGIN_BUFFER_str_sub subspell[3];
}
type_SPELLS_BEGIN_BUFFER_str;
#pragma pack (pop)

/* ???
Entity Sub-Type - Spell
00 : Fireball
01 : Heal
02 : Speed Up
03 : Posession
04 : Shield
05 : Beyond Sight
06 : Earthquake
07 : Meteor
08 : Volcano
09 : Crater
0A : Teleport
0B : Rubber Band
0C : Invisible
0D : Steal Mana
0E : Rebound
0F : Lightning
10 : Castle
11 : Skeleton
12 : Thunderbolt
13 : Mana Magnet
14 : Fire Wall
15 : Reverse Speed
16 : Smart Bomb
17 : Mini Fireball
*/

void SetDefaultSpells_5C0A0();

char CanSpellAffectEntity_39FA0(type_entity_0x6E8E* a1, type_entity_0x6E8E* a2);
void ApplySpellToEntity_3A200(type_entity_0x6E8E* a1, type_entity_0x6E8E* a2);
char IsCreatureAffectable_3A7F0(type_entity_0x6E8E* a1);
void UpdatePossessedCreature_3A8B0(type_entity_0x6E8E* entity);
void InitialiseSpells_54A50(int playerIndex2, int playerIndex);
char IsSpellCastable_57450(unsigned __int8 a1);
void ResetLevelSpells_574A0();
int UpdateScroll_59C80(type_entity_0x6E8E* entity);
void UpdateSpellAvailability_6D9C0(type_str_611* a1, type_SPELLS_BEGIN_BUFFER_str* a2, __int16 a3, char a4, char a5);
void UpdateSpellExperience_6DAD0(type_str_611* a1, type_SPELLS_BEGIN_BUFFER_str* a2, __int16 a3);
void GrantSpellToPlayer_6DB50(char a1, char a2);
void GrantAllSpellsMultiplayer_6DBD0();
void ImproveAbility_6DC40(unsigned __int8 a1);
type_entity_0x6E8E* CastSpell_6DCA0(type_entity_0x6E8E* a1, axis_3d* a2, unsigned __int16 a3, type_SPELLS_BEGIN_BUFFER_str_sub* a4, __int16 a5, char a6);
int GetSpellIndex_6E020(int entitySubtype);
void UpdateExperience_6E090(type_str_611* spells, int countXP);
void EnableAllSpells_6E0D0();
type_entity_0x6E8E* AddSpellPickup_51120(axis_3d* a1, char a2, char a3);
type_entity_0x6E8E* AddFireballSpell_511A0(axis_3d* a1);
type_entity_0x6E8E* AddPossessSpell_511C0(axis_3d* a1);
type_entity_0x6E8E* AddCastleSpell_511E0(axis_3d* a1);
type_entity_0x6E8E* AddSpeedUpSpell_51200(axis_3d* a1);
type_entity_0x6E8E* AddMorphSpell_51220(axis_3d* a1);
type_entity_0x6E8E* AddHealSpell_51240(axis_3d* a1);
type_entity_0x6E8E* AddShieldSpell_51260(axis_3d* a1);
type_entity_0x6E8E* AddLightningSpell_51280(axis_3d* a1);
type_entity_0x6E8E* AddReboundSpell_512A0(axis_3d* a1);
type_entity_0x6E8E* AddMeteorSpell_512C0(axis_3d* a1);
type_entity_0x6E8E* AddTeleportSpell_512E0(axis_3d* a1);
type_entity_0x6E8E* AddInvisibleSpell_51300(axis_3d* a1);
type_entity_0x6E8E* AddBeyondSightSpell_51320(axis_3d* a1);
type_entity_0x6E8E* AddStealManaSpell_51340(axis_3d* a1);
type_entity_0x6E8E* AddDuelSpell_51360(axis_3d* a1);
type_entity_0x6E8E* AddTremorSpell_51380(axis_3d* a1);
type_entity_0x6E8E* AddCraterSpell_513A0(axis_3d* a1);
type_entity_0x6E8E* AddEarthquakeSpell_513C0(axis_3d* a1);
type_entity_0x6E8E* AddVolcanoSpell_513E0(axis_3d* a1);
type_entity_0x6E8E* AddSummonArmySpell_51400(axis_3d* a1);
type_entity_0x6E8E* AddGravityWellSpell_51420(axis_3d* a1);
type_entity_0x6E8E* AddWhirlwindSpell_51440(axis_3d* a1);
type_entity_0x6E8E* AddFoolsManaSpell_51460(axis_3d* a1);
type_entity_0x6E8E* AddMagicMineSpell_51480(axis_3d* a1);
type_entity_0x6E8E* AddAllianceSpell_514A0(axis_3d* a1);
type_entity_0x6E8E* AddCaveInSpell_514C0(axis_3d* a1);
bool CanSpellOwnerCast_68D50(type_entity_0x6E8E* locEvent1, type_entity_0x6E8E* locEvent2);
signed int RechargeSpell_68DE0(type_entity_0x6E8E* a1, type_entity_0x6E8E* a2);
signed int LaunchSpellProjectile_68E50(type_entity_0x6E8E* a1, type_entity_0x6E8E* a2, type_entity_0x6E8E* a3);
signed int UpdateSpellPickupCommon_68FF0(type_entity_0x6E8E* a1, char a2, char a3);
signed int UpdateSpellPickupFloating_692A0(type_entity_0x6E8E* a1);
char ResetSpellToPickup_692C0(type_entity_0x6E8E* a1);
void SpellHitByEntity_69300(type_entity_0x6E8E* a1, type_entity_0x6E8E* a2);
void SpellFireballActive_693F0(type_entity_0x6E8E* a1);
signed int SpellFireballPickup_69600(type_entity_0x6E8E* a1);
signed int SpellFireballPickupUpgrade_69620(type_entity_0x6E8E* a1);
void SpellPossessionActive_69640(type_entity_0x6E8E* a1);
void CastPossessionProjectile_69900(type_entity_0x6E8E* a1, type_entity_0x6E8E* a2);
signed int SpellPossessionPickup_69A70(type_entity_0x6E8E* a1);
signed int SpellPossessionPickupUpgrade_69A90(type_entity_0x6E8E* a1);
type_entity_0x6E8E* SpellCastleActive_69AB0(type_entity_0x6E8E* a1);
signed int SpellCastlePickup_69D70(type_entity_0x6E8E* a1);
signed int SpellCastlePickupUpgrade_69D90(type_entity_0x6E8E* a1);
void GetScroll_69DB0(type_entity_0x6E8E* a1);
signed int SpellSpeedUpPickupUpgrade_6A010(type_entity_0x6E8E* a1);
void SpellMetamorphActive_6A030(type_entity_0x6E8E* a1);
signed int SpellMetamorphPickup_6A2C0(type_entity_0x6E8E* a1);
signed int SpellMetamorphPickupUpgrade_6A2E0(type_entity_0x6E8E* a1);
void SpellHealActive_6A300(type_entity_0x6E8E* a1);
signed int SpellHealPickup_6A440(type_entity_0x6E8E* a1);
signed int SpellHealPickupUpgrade_6A460(type_entity_0x6E8E* a1);
void SpellShieldActive_6A480(type_entity_0x6E8E* a1);
signed int SpellShieldPickup_6A580(type_entity_0x6E8E* a1);
signed int SpellShieldPickupUpgrade_6A5A0(type_entity_0x6E8E* a1);
void SpellLightningActive_6A5C0(type_entity_0x6E8E* a1);
signed int SpellLightningPickup_6A9C0(type_entity_0x6E8E* a1);
signed int SpellLightningPickupUpgrade_6A9E0(type_entity_0x6E8E* a1);
__int16 SpellReboundActive_6AA00(type_entity_0x6E8E* a1);
signed int SpellReboundPickup_6AAC0(type_entity_0x6E8E* a1);
signed int SpellReboundPickupUpgrade_6AAE0(type_entity_0x6E8E* a1);
void SpellMeteorActive_6AB00(type_entity_0x6E8E* a1);
signed int SpellMeteorPickup_6AD00(type_entity_0x6E8E* a1);
signed int SpellMeteorPickupUpgrade_6AD20(type_entity_0x6E8E* a1);
void SpellTeleportActive_6AD60(type_entity_0x6E8E* a1);
signed int SpellTeleportPickup_6B180(type_entity_0x6E8E* a1);
signed int SpellTeleportPickupUpgrade_6B1A0(type_entity_0x6E8E* a1);
void SpellInvisibleActive_6B1C0(type_entity_0x6E8E* a1);
signed int SpellInvisiblePickup_6B2D0(type_entity_0x6E8E* a1);
signed int SpellInvisiblePickupUpgrade_6B2F0(type_entity_0x6E8E* a1);
__int16 SpellBeyondSightActive_6B310(type_entity_0x6E8E* a1);
signed int SpellBeyondSightPickup_6B3A0(type_entity_0x6E8E* a1);
signed int SpellBeyondSightPickupUpgrade_6B3C0(type_entity_0x6E8E* a1);
void SpellStealManaActive_6B3E0(type_entity_0x6E8E* a1);
signed int SpellStealManaPickup_6B5D0(type_entity_0x6E8E* a1);
signed int SpellStealManaPickupUpgrade_6B5F0(type_entity_0x6E8E* a1);
__int16 SpellDuelActive_6B610(type_entity_0x6E8E* a1);
signed int SpellDuelPickup_6B830(type_entity_0x6E8E* a1);
signed int SpellDuelPickupUpgrade_6B850(type_entity_0x6E8E* a1);
void SpellTremorActive_6B870(type_entity_0x6E8E* a1);
signed int SpellTremorPickup_6BA70(type_entity_0x6E8E* a1);
signed int SpellTremorPickupUpgrade_6BA90(type_entity_0x6E8E* a1);
void SpellCraterActive_6BAB0(type_entity_0x6E8E* a1);
signed int SpellCraterPickup_6BCB0(type_entity_0x6E8E* a1);
signed int SpellCraterPickupUpgrade_6BCD0(type_entity_0x6E8E* a1);
void SpellEarthquakeActive_6BCF0(type_entity_0x6E8E* a1);
signed int SpellEarthquakePickup_6BEF0(type_entity_0x6E8E* a1);
signed int SpellEarthquakePickupUpgrade_6BF10(type_entity_0x6E8E* a1);
void SpellVolcanoActive_6BF30(type_entity_0x6E8E* a1);
signed int SpellVolcanoPickup_6C130(type_entity_0x6E8E* a1);
signed int SpellVolcanoPickupUpgrade_6C150(type_entity_0x6E8E* a1);
void SpellSummonArmyActive_6C170(type_entity_0x6E8E* a1);
signed int SpellSummonArmyPickup_6C3A0(type_entity_0x6E8E* a1);
signed int SpellSummonArmyPickupUpgrade_6C3C0(type_entity_0x6E8E* a1);
void SpellGravityWellActive_6C3E0(type_entity_0x6E8E* a1);
signed int SpellGravityWellPickup_6C5E0(type_entity_0x6E8E* a1);
signed int SpellGravityWellPickupUpgrade_6C600(type_entity_0x6E8E* a1);
void SpellWhirlwindActive_6C620(type_entity_0x6E8E* a1);
signed int SpellWhirlwindPickup_6C830(type_entity_0x6E8E* a1);
signed int SpellWhirlwindPickupUpgrade_6C850(type_entity_0x6E8E* a1);
void SpellFoolsManaActive_6C870(type_entity_0x6E8E* a1);
signed int SpellFoolsManaPickup_6CA80(type_entity_0x6E8E* a1);
signed int SpellFoolsManaPickupUpgrade_6CAA0(type_entity_0x6E8E* a1);
void SpellMagicMineActive_6CAC0(type_entity_0x6E8E* a1);
signed int SpellMagicMinePickup_6CCE0(type_entity_0x6E8E* a1);
signed int SpellMagicMinePickupUpgrade_6CD00(type_entity_0x6E8E* a1);
void SpellAllianceActive_6CD20(type_entity_0x6E8E* a1);
signed int SpellAlliancePickup_6CF60(type_entity_0x6E8E* a1);
signed int SpellAlliancePickupUpgrade_6CF80(type_entity_0x6E8E* a1);
void SpellCaveInActive_6CFA0(type_entity_0x6E8E* a1);
signed int SpellCaveInPickup_6D1C0(type_entity_0x6E8E* a1);
signed int SpellCaveInPickupUpgrade_6D1E0(type_entity_0x6E8E* a1);
void CopyAxisForSpellWithLife_6D830(type_entity_0x6E8E* entity, int spellId);
void RestorePreviousSpell_6D880(type_entity_0x6E8E* a1);
void SendSpellMessage_6D8B0(unsigned __int16 a1, unsigned __int16 a2, __int16 a3);

#endif //MAIN_SPELLS