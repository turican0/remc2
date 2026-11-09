#pragma once

#include <cstdint>

#include "Mc1GameState.h"

// MC1 level start: the loading screen, the level data (levels.dat, ddlevels.dat in HW) and the generated map

extern const int8_t anglePatterns_9075C_9075C[148][4]; // the heights of the 4 corners of a tile, -1 unused
extern uint8_t angleTypes_B5D40_B5D30[2401][2]; // terrain type and angle for 4 corner heights 0-6 (7^4)

void LoadingScreen_34460_34820();
void LoadLevel_407A0_40AE0();
void ClearLevel_40550_40890();
uint8_t LoadLevelData_3E100_3E440(int16_t levelIndex, Type_Mc1Level_2F503* level);
void GenerateLevelMap_31AA0_31AE0(Type_Mc1Level_2F503* level);
void SmoothTerrain_31BB0_31FA0();
void MarkFlatHighland_31C10(uint8_t minHeight, uint8_t maxRange);
void MarkWaterEdges_31D40_32130();
void SetTerrainAngles_32560_32950();
void ComputeTerrainShading_329C0_32DB0();
void MarkSteepTerrain_33180_33570(uint8_t minRange);
void CreateLevelModels_37440_37800(int32_t group, uint8_t clear);
void CreateFeatureModel_37560_37920(Type_Mc1LevelFeature* feature);
