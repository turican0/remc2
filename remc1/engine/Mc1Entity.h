#pragma once

#include <cstdint>

#include "Mc1GameState.h"

// MC1 entities: the free lists, the map cells, the creation by class / model and the features of the level

extern Type_Mc1Position featurePosition_AE454_AE444; // the position a feature / chain creates its entities at
extern uint8_t creatureTypes_98F38_98F38[31][32]; // the records of typeParams_0x9C (MC2 typeParams_0xA0)
extern uint8_t playerData_B7330_B7320[0xF0 * 4]; // playerData_0xA0 of the new entities
extern const uint8_t frameCounts_90AD8_90AD8[42]; // by the kind of the sprite

void InitEntities_371C0_37580();
void RebuildFreeLists_37220_375E0();
Type_Mc1Entity* AllocEntity_372C0_37680();
void SetSprite_36FA0_37360(Type_Mc1Entity* entity, int16_t sprite);
void SetSpriteBox_37030_373F0(Type_Mc1Entity* entity, int16_t sprite);
void SetSpriteHalfBox_370E0_374A0(Type_Mc1Entity* entity, int16_t sprite);
void ResetLife_36DE0_371A0(Type_Mc1Entity* entity);
void SetBox_37130_374F0(Type_Mc1Entity* entity, int16_t width, int16_t height);
int32_t WrapDelta_358D0_35C90(int16_t from, int16_t to, int16_t range);
void SetFeaturePosition_35900_35CC0(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2);
void InitBuildingEntity_36DF0_371B0(Type_Mc1Entity* entity, uint16_t sprite);
void SetBuildingBox_37150_37510(Type_Mc1Entity* entity, int16_t sprite);
Type_Mc1Entity* CreateEntity_373F0_377B0(Type_Mc1Position* position, int32_t entityClass, int32_t model);
int32_t CopyEntityPosition_41C70_41FB0(Type_Mc1Entity* entity, Type_Mc1Position* position);
void AddToMap_41CF0_42030(Type_Mc1Entity* entity, Type_Mc1Position* position);
void RemoveFromMap_41DD0_42110(Type_Mc1Entity* entity);
void MarkToFree_41E80_421C0(Type_Mc1Entity* entity);
void FreeEntity_41E90_421D0(Type_Mc1Entity* entity);
uint32_t Distance2D_423D0_42710(Type_Mc1Position* from, Type_Mc1Position* to);
void CreateFeatures_36430_367F0(Type_Mc1Level_2F503* level);
void CreateFeature_36480_36840(Type_Mc1Level_2F503* level, Type_Mc1LevelFeature* feature);
void CreateFeatureChain_362C0_36680(Type_Mc1Level_2F503* level, Type_Mc1LevelFeature* feature);
void InitCreatedEntities_36620_369E0();
void CreateFeatureLine1C_35960_35D20(int32_t x, int32_t y, int32_t x2, int32_t y2);
void CreateFeatureLine1D_35BF0_35FB0(int32_t x, int32_t y, int32_t x2, int32_t y2);
void CreateFeatureLine1F_35D30_360F0(int32_t x, int32_t y, int32_t x2, int32_t y2);
void CreateFeatureLine32_35DE0_361A0(int32_t x, int32_t y, int32_t x2, int32_t y2);
int32_t BoundsOverlap_118C0_118C0(const Type_Mc1Position* position, const int16_t* box, const Type_Mc1Position* position2, const int16_t* box2);
int32_t EntitiesOverlap_11950_11950(Type_Mc1Entity* entity, Type_Mc1Entity* entity2);
void SendMailToOverlappingEntities_127E0_127E0(Type_Mc1Entity* entity, uint8_t kind, int32_t value); // only the kind 0
void SendMailAround_120B0_120B0(Type_Mc1Entity* entity, uint8_t kind, int32_t value);
void SetHalfMana_36F90_37350(Type_Mc1Entity* entity); // mana = maxLife / 2
int32_t GetFreeEntityCount_37710_37AD0();
int32_t MoveOnMap_41CC0_42000(Type_Mc1Entity* entity, Type_Mc1Position* position); // out of the map and in again
