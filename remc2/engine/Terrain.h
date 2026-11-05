#pragma once

#ifndef MAIN_TERRAIN
#define MAIN_TERRAIN

#include "Basic.h"
#include "BasicMini.h"
#include "BasicTerrain.h"
#include "Unk_D4A30.h"
#include "Unk_D47E0.h"

#include "LevelStructs.h"

extern char building_F2CD0x[2800][2]; // 233cd0//4802 //4816 // size has to be 2 * (7^4 + 7^3 + 7^2 + 7^1) = 2 * (2401 + 343 + 49 + 7) = 2*2800 as seen in sub_462A0_orig
extern uint8_t MapBasicHeight_D41B7;

extern uint16_t rand2_17B4E0;

extern uint8_t mapTerrainType_10B4E0[65536];
extern uint8_t mapHeightmap_11B4E0[65536];
extern uint8_t mapShading_12B4E0[65536];
extern uint8_t mapAngle_13B4E0[65536];
extern __int16 mapEntityIndex_15B4E0[65536];

extern bool lowDiffHeightmap_D47DC;

void GenerateLevelMap_43830(Type_Level_2FECE* a2x);//224830
void DecompressTerrainMapLevel_B5E70(__int16 a1, unsigned __int16 a2, __int16 a3, int32_t a4);//296e70
void TruncTerrainHeight_44DB0(int16_t mapEntityIndex_15B4E0[], uint8_t mapHeightmap_11B4E0[]);//225db0 // map to heightmap
int GetTerrainAlt2_B5C60(uint16_t a1, uint16_t a2);//296c60
void MarkLandAndRivers_44E40(int a1, uint8_t a2);//225e40
void SetMax4Tiles_45AA0();//226aa0
void ApplySnowLine_440D0(unsigned __int16 a1);//2250d0
void MarkFlatLowland_45060(uint8_t a1, uint8_t a2);//226060
void SpreadTerrainClass4_44320();//225320
void MarkFlatLowlandClass2_45210(uint8_t a1, uint8_t a2);//226210
void MarkFlatHighland_454F0(uint8_t a1, uint8_t a2);//2264f0
void MarkSteepTerrain_45600(uint8_t a1);//226600
void RemoveIsolatedTerrainClasses_43FC0();//224fc0
void SmoothTerrain_43970();//224970
void AddRivers_43EE0();//224ee0
void SetTerrainAngles_44580();//225580
void GenerateCaveCeiling_43B40();//224b40
void MarkWaterEdges_43D50();//224d50
void ComputeTerrainShading_44D00();//225d00
void TerrainDiamondStep_B5EFA(__int16 a1, uaxis_2d* a2, int32_t a3, int16_t* nextRand);//296EFA
void TerrainSquareStep_B5F8F(__int16 a1, uaxis_2d* a2, int32_t a3, int16_t* nextRand);//296f8f
void SmoothTiles_44EE0(/*int a1,*/ uaxis_2d a2x);//225ee0
unsigned int GetSmoothedHeight_439A0(uint16_t index);//2249a0
void RandomiseCeilingHeight_43BB0();//224bb0

int GetTileSlope_1B7A0(axis_3d* a1);
int GetTileSlopeWide_1B830(axis_3d* a1);
uint8_t GetTileHeightType_45BE0(uint8_t a2, uaxis_2d a3x, uint8_t type);//226be0
bool CheckTileType8Neighbours_33F70(uint16_t inAxis);

void SetTileTerrainType_45DC0(uint8_t a2, uaxis_2d a3, unsigned __int8 a4, uint8_t type = 0);//type: dl of the caller
void UpdateTerrainTiles_462A0(uaxis_2d a1x, uaxis_2d a2x);

signed int GetTileAngleBit_104A0(axis_3d* a1);
uint32_t TerrainTileIsWater_104D0(axis_3d* a1);
uint32_t TerrainTileType_10590(char a1);
int GetTerrainAlt_10C40(axis_3d* a1);
bool IsTouchingCeiling_11E70(type_entity_0x6E8E* a1, axis_3d* a2);
int GetCeilingAlt_10C60(axis_3d* a1);
int GetCeilingAlt2_B5D68(uint16_t a1, uint16_t a2);

typedef struct {
	int index;
	uint32_t adress;
} type_compstr;

extern type_compstr compstr[100];

#endif //MAIN_TERRAIN