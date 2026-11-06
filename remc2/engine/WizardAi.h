#pragma once

#include "Basic.h"

signed int UpdateWizardAi_12910(type_entity_0x6E8E* a1);
signed int UpdateWizardAiState_12A70(type_entity_0x6E8E* a1);
signed int WizardAiChooseState_12E70(type_entity_0x6E8E* a1);
signed int WizardAiAttackTarget_12FF0(type_entity_0x6E8E* a1);
signed int WizardAiMoveToPosition_13100(type_entity_0x6E8E* a1);
signed int WizardAiFaceTarget_131F0(type_entity_0x6E8E* a1);
signed int WizardAiCastFoolsMana_13270(type_entity_0x6E8E* a1);
signed int WizardAiReturnToCastle_133B0(type_entity_0x6E8E* a1);
signed int WizardAiPossessTarget_135C0(type_entity_0x6E8E* a1);
signed int WizardAiAttackWithSpell_13710(type_entity_0x6E8E* a1);
signed int WizardAiCombat2_13830(type_entity_0x6E8E* a1);
signed int WizardAiCombat3_13850(type_entity_0x6E8E* a1);
int WizardAiCombat4_13870(type_entity_0x6E8E* a1);
signed int WizardAiCombat_13890(type_entity_0x6E8E* a1);
signed int WizardAiTryBuildCastle_13B00(type_entity_0x6E8E* a1);
signed int WizardAiTryGrowCastle_13C50(type_entity_0x6E8E* a1);
type_entity_0x6E8E* WizardAiFindTarget_13CE0(type_entity_0x6E8E* a1);
signed int WizardAiShouldRetreat_13DC0(type_entity_0x6E8E* a1);
signed int WizardAiChooseEnemyWizard_13E40(type_entity_0x6E8E* a1);
signed int WizardAiChooseEnemyCreature_14030(type_entity_0x6E8E* a1);
signed int WizardAiChooseEnemyCastle_14250(type_entity_0x6E8E* a1);
signed int WizardAiChooseAttackTarget_14530(type_entity_0x6E8E* a1);
signed int WizardAiCheckHealth_14630(type_entity_0x6E8E* a1);
type_entity_0x6E8E* GetPlayerSpellEntity_146C0(type_entity_0x6E8E* a1, unsigned __int8 a2);
void UpdateWizardMovement_146F0(type_entity_0x6E8E* a1);
type_entity_0x6E8E* FindNearestOwnBalloon_148E0(type_entity_0x6E8E* a1);
type_entity_0x6E8E* FindWizardTargetBySpell_14B10(type_entity_0x6E8E* a1, unsigned __int8 a2);
type_entity_0x6E8E* FindNearestOtherWizard_14BD0(type_entity_0x6E8E* a1, type_entity_0x6E8E* a2);
__int16 GetEntityKey_14C40(type_entity_0x6E8E* a1);
bool IsTargetStillValid_14C60(type_entity_0x6E8E* a1, type_entity_0x6E8E* a2);
signed int WizardAiMoveTowards_14C90(type_entity_0x6E8E* a1, type_entity_0x6E8E* a2, signed int a3, signed int a4);
signed int WizardAiCastSpell_14E10(type_entity_0x6E8E* a1, unsigned __int8 a2);
type_entity_0x6E8E* WizardCanCastSpell_15170(type_entity_0x6E8E* a1, unsigned __int8 a2);
signed int WizardAiCanGrowCastle_155E0(type_entity_0x6E8E* a1);
bool IsPlayerSpellActive_156F0(type_entity_0x6E8E* a1, unsigned __int8 a2);
bool HasEnoughManaForSpell_15730(type_entity_0x6E8E* a1, unsigned __int8 a2);
type_entity_0x6E8E* GetActivePlayerSpellEntity_15760(type_entity_0x6E8E* a1, unsigned __int8 a2);
char WizardAiChooseAttackSpell_15790(type_entity_0x6E8E* a1);
char WizardAiChooseSpell_15910(type_entity_0x6E8E* a1);
void UpdateWizardAiTargets_159E0();
type_entity_0x6E8E* FindNearestEntityTargeting_15CB0(/*type_str_0x6E8E* a1,*/ type_entity_0x6E8E* a2); // weak
void SetWizardStrafeSpeed_15D20(type_entity_0x6E8E* a1);
char RateWizardTarget_15D40(__int16 a1, type_entity_0x6E8E* a2, type_entity_0x6E8E* a3); // weak
bool HasAttackSpell_15E60(type_entity_0x6E8E* a1);
char FindLocalWizard_15EE0();
signed int WizardTrySelectSpell_15F20(type_entity_0x6E8E* a1, signed __int16 a2, signed int a3);
signed int WizardAiMetamorph_15FC0(type_entity_0x6E8E* a1);
signed int WizardAiAttackMetamorphed_161A0(type_entity_0x6E8E* a1);
bool HasAreaSpell_164B0(type_entity_0x6E8E* a1);
signed int WizardAiSteer_16580(type_entity_0x6E8E* a1);
int16_t FindAdjacentLavaDirection_16730(/*int a1,*/ type_entity_0x6E8E* a2, char a3); // weak
char WizardAiAvoidObstacles_169C0(type_entity_0x6E8E* a1);
int16_t FindAdjacentLavaDirection2_16CA0(baxis_2d* a2, __int16 a3, char a4); // weak
int TraceTileLine_16E70(baxis_2d* a1, baxis_2d* a2);
type_entity_0x6E8E* FindNearestOtherEntity_16FC0(type_entity_0x6E8E* a1, type_entity_0x6E8E* a2);
void CountWizards_223E0();
void UpdateWizardHeights_585D0();
