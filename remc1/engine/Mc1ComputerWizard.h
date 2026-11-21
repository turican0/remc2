#pragma once

#include <cstdint>

#include "Mc1GameState.h"

// the wizard played by the computer (class 3 action 1)

extern Type_Mc1Entity* playerWizard_ACF50_ACF40; // the last wizard of the model 0 (16990)

Type_Mc1Entity* SpellOf_14E60_14E60(Type_Mc1Entity* wizard, uint8_t spell);
void FindPlayerWizard_16990_16990();
int16_t TargetKind_15420_15420(Type_Mc1Entity* entity);
int32_t IsTargetKind_15440_15440(Type_Mc1Entity* wizard, Type_Mc1Entity* entity);
int32_t SpellCasting_15E60_15E60(Type_Mc1Entity* wizard, uint8_t spell);
int32_t SpellCheaper_15E90_15E90(Type_Mc1Entity* wizard, uint8_t spell);
Type_Mc1Entity* SpellActive_16000_16000(Type_Mc1Entity* wizard, uint8_t spell);
int8_t HasAttackSpell_16920_16920(Type_Mc1Entity* wizard);
int32_t CanCastSpell_15A00_15A00(Type_Mc1Entity* wizard, uint8_t spell);
int32_t CastSpell_155F0_155F0(Type_Mc1Entity* wizard, uint8_t spell);
int32_t ApproachTarget_15470_15470(Type_Mc1Entity* wizard, Type_Mc1Entity* target, int32_t nearDistance, int32_t farDistance);
int32_t AttackWithSpell10_13800_13800(Type_Mc1Entity* wizard);
int32_t NoState_138E0_138E0(Type_Mc1Entity* );
int32_t BuildCastle_138F0_138F0(Type_Mc1Entity* wizard);
int32_t GoToTarget_13990_13990(Type_Mc1Entity* wizard);
int32_t Wander_13A10_13A10(Type_Mc1Entity* wizard);
int32_t GoHome_13A70_13A70(Type_Mc1Entity* wizard);
int32_t Possess_13BA0_13BA0(Type_Mc1Entity* wizard);
int8_t ChooseAttackSpell_16030_16030(Type_Mc1Entity* wizard);
int8_t ChooseSpell_16310_16310(Type_Mc1Entity* wizard);
int32_t AttackThing_13CA0_13CA0(Type_Mc1Entity* wizard);
int32_t AttackWizard_13DD0_13DD0(Type_Mc1Entity* wizard);
int32_t Attack_13DC0_13DC0(Type_Mc1Entity* wizard);
Type_Mc1Entity* NearestWizardOfModel_15260_15260(Type_Mc1Entity* entity, uint8_t model);
int32_t FindCastlePlace_13F00_13F00(Type_Mc1Entity* wizard);
int32_t ShouldGrowCastle_14120_14120(Type_Mc1Entity* wizard);
Type_Mc1Entity* NearestOtherWizard_15340_15340(Type_Mc1Entity* entity, Type_Mc1Entity* wizard);
Type_Mc1Entity* NearestOtherCastle_153B0_153B0(Type_Mc1Entity* entity, Type_Mc1Entity* castle);
Type_Mc1Entity* FindMana_15080_15080(Type_Mc1Entity* wizard);
int32_t ShouldPossessMana_14230_14230(Type_Mc1Entity* wizard);
int32_t ShouldGoHome_14310_14310(Type_Mc1Entity* wizard);
int32_t ShouldAttackCastle_143A0_143A0(Type_Mc1Entity* wizard);
int32_t ShouldAttackWizard_145B0_145B0(Type_Mc1Entity* wizard);
int32_t ShouldAttackBalloon_147E0_147E0(Type_Mc1Entity* wizard);
int32_t ShouldAttackCreature_14B10_14B10(Type_Mc1Entity* wizard);
int32_t StateGrowCastle_14C40_14C40(Type_Mc1Entity* wizard);
int32_t StateBuildCastle_14C80_14C80(Type_Mc1Entity* wizard);
int32_t StatePossess_14CE0_14CE0(Type_Mc1Entity* wizard);
int32_t StateAttackCastle_14D00_14D00(Type_Mc1Entity* wizard);
int32_t StateAttackCreature_14D20_14D20(Type_Mc1Entity* wizard);
int32_t StateAttackBalloon_14D40_14D40(Type_Mc1Entity* wizard);
int32_t StateAttackWizard_14D60_14D60(Type_Mc1Entity* wizard);
int32_t StateGoHome_14DA0_14DA0(Type_Mc1Entity* wizard);
int32_t StateIdle_14DC0_14DC0(Type_Mc1Entity* wizard);
int32_t ChooseState_136C0_136C0(Type_Mc1Entity* wizard);
int32_t MoveComputerWizard_14EB0_14EB0(Type_Mc1Entity* wizard);
void SpellsOfPlayer_15EC0_15EC0(Type_Mc1Entity* wizard);
Type_Mc1Entity* NearestOwnShot_16800_16800(Type_Mc1Entity* wizard);
void Dodge_16870_16870(Type_Mc1Entity* wizard, Type_Mc1Entity* );
void DefendFromShot_16890_16890(Type_Mc1Entity* wizard, Type_Mc1Entity* shot);
int32_t UpdateComputerWizard_132B0_132B0(Type_Mc1Entity* wizard);
void ComputerWizard_13170_13170(Type_Mc1Entity* wizard);
