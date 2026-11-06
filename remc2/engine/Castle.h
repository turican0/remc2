#pragma once

#include "Basic.h"

extern int speedIcrement_D4B84;
extern int wizardSpeed2_D4B8C;
char CanPlaceCastleArea_11A10(type_entity_0x6E8E* a1);
int UpdateHouse_38330(type_entity_0x6E8E* a1);
void RemoveCastleStage_385C0(type_entity_0x6E8E* event);
void CastleDefendEvent_3AF00(type_entity_0x6E8E* a1);
void InitBuildingEntity_49A30(type_entity_0x6E8E* event, unsigned __int16 a2);
type_entity_0x6E8E* AddCastle_4AA40(axis_3d* a1);
type_entity_0x6E8E* AddCastleBalloon_4ABA0(axis_3d* a1);
type_entity_0x6E8E* CastleDefendCreate_508E0(axis_3d* a1);
void UpdateCastle_59F60(type_entity_0x6E8E* a1);
void UpdateCastleBuilding_5B100(type_entity_0x6E8E* locEvent);
signed int UpdateCastleTarget_5EFA0(type_entity_0x6E8E* a1);
void UpdateWizardSpeed_5F380(type_entity_0x6E8E* a1);
char CastleAttackEntity_5F660(type_entity_0x6E8E* a1, type_entity_0x6E8E* a2, int a3);
void SetCastleCooldown_5F7B0(type_entity_0x6E8E* a1, type_entity_0x6E8E* a2, int a3);
type_entity_0x6E8E* FindNearestEnemyBalloon_5F810(type_entity_0x6E8E* a1, type_entity_0x6E8E* a2, type_entity_0x6E8E* a3);
void UpdateCastleLevel_5F890(type_entity_0x6E8E* a1, __int16 a2);
void EndOfCastleProjectile_5F8F0(type_entity_0x6E8E* a1);
void BeginOfCastleCreation_5FA70(type_entity_0x6E8E* locEvent);
void SpawnCastleEffect42_5FBD0(type_entity_0x6E8E* a1);
type_entity_0x6E8E* SpawnCastleEffect41_5FC40(type_entity_0x6E8E* a1);
void DestroyCastleLevel_5FCA0(type_entity_0x6E8E* a1);
void TransferCastleMana_5FD00(type_entity_0x6E8E* a1);
void UpdateCastleStructure_5FF50(type_entity_0x6E8E* a1);
x_WORD* GetCastleLevelSize_60400(__int16 a1, x_WORD* a2, x_WORD* a3);
void SpawnCastleBuildEffect_60480(type_entity_0x6E8E* a1);
void ShrinkCastleMana_605E0(type_entity_0x6E8E* a1);
void SetCastleLifeAndMana_60780(type_entity_0x6E8E* a1, type_entity_0x6E8E* a2, int a3, int a4);
void GrowCastle_60810(type_entity_0x6E8E* locEvent);
int CheckCastleDestroyed_609E0(type_entity_0x6E8E* locEvent);
void UpdateCastleBalloon_60AB0(type_entity_0x6E8E* a1);
void SpawnCastleRubble_60D50(axis_3d* a1, type_entity_0x6E8E* a2);
void ApplyCastleDamage_60EA0(type_entity_0x6E8E* a1);
void UpdateCastles_60F00();
type_entity_0x6E8E* AddBalloonManaToCastle_61000(type_entity_0x6E8E* a1);
type_entity_0x6E8E* UpdateBalloonCollection_61050(type_entity_0x6E8E* a1);
void UpdateBalloonChain_613D0(type_entity_0x6E8E* a1);
char FindNearestManaForBalloon_61620(type_entity_0x6E8E* a1, type_entity_0x6E8E* a2);
signed int IsBalloonNearCastle_61810(type_entity_0x6E8E* a1, type_entity_0x6E8E* a2);
