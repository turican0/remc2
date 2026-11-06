#pragma once

#include <string>

#include "global_types.h"
#include "LevelStructs.h"

void MoveEntity_57FA0(axis_3d* position, uint16_t yaw, int16_t pitch, int16_t speed);

void SetPlayerColoursForMapType_48120();
type_entity_0x6E8E* SetPlayerStart0_4A820(axis_3d* a1);
type_entity_0x6E8E* SetPlayerStart1_4A840(axis_3d* a1);
type_entity_0x6E8E* SetPlayerStart2_4A860(axis_3d* a1);
type_entity_0x6E8E* SetPlayerStart3_4A880(axis_3d* a1);
type_entity_0x6E8E* SetPlayerStart4_4A8A0(axis_3d* a1);
type_entity_0x6E8E* SetPlayerStart5_4A8C0(axis_3d* a1);
type_entity_0x6E8E* SetPlayerStart6_4A8E0(axis_3d* a1);
type_entity_0x6E8E* SetPlayerStart7_4A900(axis_3d* a1);
type_entity_0x6E8E* AddPlayer_4A920(axis_3d* a1);
type_entity_0x6E8E* AddWizard_4A9C0(axis_3d* a1);
void DestroyPlayerCastle(int player);
void MatchScoreReset();
void MatchScoreMarkPlaying(int player);
void MatchScoreRecordKnockOut(int victim, type_entity_0x6E8E* victimEntity);
std::string MatchScoreResultLine();
void PlayerEvents_51BB0();
void ApplyPlayerOptions_55C60(type_str_0x2BDE* loc0x2BDE);
void CopyPlayerPositionAndRotation_57B20(type_str_0x2BDE* a1, type_entity_0x6E8E* a2);
void UpdateMultiplayerRandom_58630();
void UpdatePlayerWizard_5C950(type_str_0x2BDE* a1, type_entity_0x6E8E* a2);
void CreditKillToWizard_686D0(type_entity_0x6E8E* a1, type_entity_0x6E8E* a2);
void UpdatePlayerSpellCharges_5CF40(type_entity_0x6E8E* a1, char a2);
bool MoveTest_5D0A0(type_entity_0x6E8E* a1);
void MovePlayerWizard_5D530(type_entity_0x6E8E* a1);
void UpdateWizardWaterEffect_5DD50(type_entity_0x6E8E* a1);
void UpdateWizardMount_5DE30(type_entity_0x6E8E* a1);
void UpdatePlayer_5E010(type_entity_0x6E8E* a1);
void MultiplayerTestDie_5E310(type_entity_0x6E8E* a1);
__int16 CheckWizardKiller_5E6C0(type_entity_0x6E8E* a1);
void MultiplayerTestBanished_5E7C0(type_entity_0x6E8E* a1);
signed int EndGameSequence_5E8C0(type_entity_0x6E8E* a1);
type_entity_0x6E8E* SetWizardHitTimer_5EF70(type_entity_0x6E8E* a1);
