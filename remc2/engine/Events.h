#pragma once

#include "global_types.h"
#include "Level.h"
#include "CompareMaps.h"

#pragma pack(push, 1)
typedef struct {//lenght 14
	uint32_t dword_0;//ok
	int16_t index_4;//ok
	uint32_t address_6;
	int32_t enabled_10;//ok
}
type_D4C52ar2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {//lenght 18
	uint32_t address_0;
	uint32_t classIndex_4;
	int16_t word_8;
	type_D4C52ar2* actions_10;
	type_D4C52ar2* models_14;
}
type_D4C52ar;
#pragma pack(pop)


#pragma pack(push, 1)
typedef struct {//lenght 6
	int8_t* bitmapData_0;
	int16_t bitmapIndex_4;
}
sub2x_BYTE_E7EE0x;
typedef struct {//lenght 24
	int32_t dword_0;
	int32_t lastBitmapIndex_4;
	int32_t bitmapIndex_8;
	int32_t frameCounter_12;
	sub2x_BYTE_E7EE0x* bitmapsStr_16;
	int8_t* bitmapsData_20xx;
}
subx_BYTE_E7EE0x;
#pragma pack(pop)


extern subx_BYTE_E7EE0x str_E7EE0x[700];
extern sub2x_BYTE_E7EE0x bitmaps_E9980x[69];


unsigned int AddE7EE0x_10080(int32_t a1, int32_t a2);
void ResetEvent08_10100(int32_t a1);
void GenerateEvents_49290(Type_Level_2FECE* terrain, char a2, uint16_t width, uint16_t height);
void CopyMaxLifeToLife_49A20(type_entity_0x6E8E* event);
type_entity_0x6E8E* NewEvent_4A050();
void CallUpdateFunction_4A190(uint32_t adress, type_entity_0x6E8E* a1);//pre 22b190
type_entity_0x6E8E* CallCreateFunction_4A190(uint32_t adress, axis_3d* a1);//pre 22b190
type_entity_0x6E8E* CreateEntity_4A190(axis_3d* a1, int a2, int a3);
void SetMapEntity_57E50(type_entity_0x6E8E* entity);
void ReleaseEntity_57F20(type_entity_0x6E8E* entity);
signed int UpdateSpellPickup_69250(type_entity_0x6E8E* a1);

extern std::array<type_D4C52ar, 17> entityTypeTables_D4C48;
extern type_D4C52ar2 class0ECreateTable_D7B74[7];
extern char spriteKindFrames_D8A2E[38];
int CheckPositionCollision_102D0(type_entity_0x6E8E* a1, axis_3d* a2, char a3);
bool BoundsOverlap_10630(axis_3d* a1, axis_4d* a2, axis_3d* a3, axis_4d* a4);
bool EntitiesOverlap_106C0(type_entity_0x6E8E* a1, type_entity_0x6E8E* a2);
bool CompareAxisWithShift_106F0(axis_3d* a1, axis_4d* a2, axis_3d* a3, axis_4d* a4);
bool CompareAxisWithShift_10750(type_entity_0x6E8E* a1, type_entity_0x6E8E* a2);
type_entity_0x6E8E* FindOverlappingSelectableEntity_10780(type_entity_0x6E8E* a1);
type_entity_0x6E8E* FindOverlappingHostileEntity_108B0(type_entity_0x6E8E* a1);
type_entity_0x6E8E* FindOverlappingEntityOfType_10A50(type_entity_0x6E8E* a1);
signed int IsEntityOfModelNearby_10B70(axis_3d* a1, char a2, char a3);
int SendMailToOverlappingEntities_10C80(type_entity_0x6E8E* entity, unsigned __int8 channel, unsigned __int16 amount);
void SendMailToNearbyEntities_112D0(type_entity_0x6E8E* a1, unsigned __int16 a2);
void SendMailToOverlappingEntities2_11400(type_entity_0x6E8E* a1, char a2, unsigned __int16 a3);
int SendMailToOverlappingEntities3_116A0(type_entity_0x6E8E* a1, char a2, unsigned __int16 a3);
int SendMailToEntity_11900(type_entity_0x6E8E* a1, type_entity_0x6E8E* a2, unsigned __int8 a3, unsigned __int16 a4);
void KillEntitiesInArea_11960(type_entity_0x6E8E* a1);
char IsTilePassable_11C80(uaxis_2d a1);
bool IsNearPlayer_11CB0(axis_3d* a1);
bool IsNearCeiling_11E20(type_entity_0x6E8E* a1, axis_3d* a2);
void PreKillEntity_1C890(type_entity_0x6E8E* entity, char state);
void KillEntity_1C930(type_entity_0x6E8E* entity);
void SetEntitySpriteAndLoad_221F0(type_entity_0x6E8E* a1, __int16 a2);
void TurnTowardsEntity_254E0(type_entity_0x6E8E* a1, type_entity_0x6E8E* a2, unsigned __int8 a3);
void SetEntityActionState_27FE0(type_entity_0x6E8E* a1, char a2, char a3, int a4);
char MoveTowardsEntity_28390(type_entity_0x6E8E* a1, type_entity_0x6E8E* a2);
void SetEntitySpriteIfChanged_2AED0(type_entity_0x6E8E* a1, int a2);
void SetEvent144_49C70(type_entity_0x6E8E* event);
void SetEntityIndex_49C90(type_entity_0x6E8E* event, int16_t entityIndex);
void SetEntityIndexAndRot_49CD0(type_entity_0x6E8E* event, int16_t entityIndex);
void SetEntitySprite_49D50(type_entity_0x6E8E* event, int16_t entityIndex);
void SetHalfSpeedEntity_49DA0(type_entity_0x6E8E* event, int16_t entityIndex);
void SetEntitySpriteDoubleSize_49E10(type_entity_0x6E8E* event, int16_t entityIndex);
void SetEntityShiftRot_49EA0(type_entity_0x6E8E* event, int16_t shift, int16_t rotation);
void SetShiftByCastle_49EC0(type_entity_0x6E8E* event, int16_t a2);
void ResetEntityLists_49F30();
void test_pre_sub_4a190(uint32_t adress);
int GetFreeEntityCount_4A810();
char SpawnAroundEntity_572C0(type_entity_0x6E8E* a1, __int16 a2, __int16 a3, __int16 a4, char a5);
void UpdateEntities_57730();
void CopyEntityPosition_57CF0(type_entity_0x6E8E* entity, axis_3d* position);
void MoveEntityOnMap_57D40(type_entity_0x6E8E* entity, axis_3d* position);
void AddEventToMap_57D70(type_entity_0x6E8E* entity, axis_3d* position);
void DisableEntityDrawing04_57F10(type_entity_0x6E8E* entity);
void AdvanceAnimationFrame_585A0(type_entity_0x6E8E* a1);
void SnapEntityToTerrain_59C40(type_entity_0x6E8E* a1);
int SnapEntityToTerrain2_59C60(type_entity_0x6E8E* a1);
char CheckParentAlive_59DC0(type_entity_0x6E8E* a1);
type_entity_0x6E8E* FindEntityOnTile_5B070(type_entity_0x6E8E* a1);
void InitEntityPointers_5C330();
void DisableEntitesDrawing_5E660(type_entity_0x6E8E* entity);
void ClearEntityVisibleFlag_5F7E0(type_entity_0x6E8E* a1, type_entity_0x6E8E* a2);
void RaiseToEntityCentre_65580(type_entity_0x6E8E* a1);
void LowerFromEntityCentre_655A0(type_entity_0x6E8E* a1);
