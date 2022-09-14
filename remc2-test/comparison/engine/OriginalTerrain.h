#pragma once

#include <functional>
#include "../../remc2/engine/defs.h"
#include "../../remc2/portability/mctypes.h"
#include "../../remc2/engine/global_types.h"
#include "../../remc2/engine/Unk_D4A30.h"
#include "../../remc2/engine/Unk_D47E0.h"
#include "../../remc2/engine/MapType.h"

int sub_B5C60_getTerrainAlt2_orig(uint16_t a1, uint16_t a2, uint8_t mapHeightmap_11B4E0[]);
void sub_44DB0_truncTerrainHeight_orig(int16_t mapEntityIndex_15B4E0[], uint8_t mapHeightmap_11B4E0[]);
void sub_44EE0_smooth_tiles_orig(uaxis_2d a2x, uint8_t mapTerrainType_10B4E0[], uint8_t mapHeightmap_11B4E0[], uint8_t mapAngle_13B4E0[]);
void sub_44E40_orig(int a1, uint8_t a2, uint16_t x_WORD_17B4E0, uint8_t mapTerrainType_10B4E0[], uint8_t mapHeightmap_11B4E0[], uint8_t mapAngle_13B4E0[]);
int sub_B5D68_orig(__int16 a1, __int16 a2, uint8_t* x_BYTE_14B4E0_second_heightmap);
void sub_45DC0_orig(uint8_t a2, uaxis_2d a3x, unsigned __int8 a4, bool lowDiffHeightmap_D47DC, char isCaveLevel_D41B6, uint16_t x_WORD_17B4E0, char x_BYTE_F2CD0x[][2], uint8_t mapTerrainType_10B4E0[], uint8_t mapHeightmap_11B4E0[], uint8_t mapAngle_13B4E0[], uint8_t mapShading_12B4E0[], uint8_t* x_BYTE_14B4E0_second_heightmap);
void sub_462A0_orig(uaxis_2d a1x, uaxis_2d a2x, char isCaveLevel_D41B6, MapType_t mapType, uint16_t x_WORD_17B4E0, char x_BYTE_F2CD0x[][2], uint8_t mapTerrainType_10B4E0[], uint8_t mapHeightmap_11B4E0[], uint8_t mapAngle_13B4E0[], uint8_t mapShading_12B4E0[], uint8_t* x_BYTE_14B4E0_second_heightmap);
uint8_t sub_45BE0_orig(uint8_t a2, uaxis_2d a3x, bool lowDiffHeightmap_D47DC, uint8_t mapHeightmap_11B4E0[]);
bool sub_33F70_orig(unsigned __int16 a1, uint8_t mapTerrainType_10B4E0[], uint8_t mapHeightmap_11B4E0[]);
int sub_1B830_orig(axis_3d* a1, uint8_t mapHeightmap_11B4E0[]);
int sub_1B7A0_tile_compare_orig(axis_3d* a1, uint8_t mapHeightmap_11B4E0[]);
void sub_43D50_orig(uint8_t mapTerrainType_10B4E0[], uint8_t mapHeightmap_11B4E0[], uint8_t mapAngle_13B4E0[]);
void sub_44D00_orig(MapType_t mapType, uint8_t mapHeightmap_11B4E0[], uint8_t mapShading_12B4E0[]);
void sub_B5E70_decompress_terrain_map_level_orig(__int16 a1, unsigned __int16 a2, __int16 a3, int32_t a4, int16_t mapEntityIndex_15B4E0[]);
void sub_B5EFA_orig(__int16 a1, uint16_t* a2, int32_t a3, __int16* a4, int16_t mapEntityIndex_15B4E0[]);
void sub_B5F8F_orig(__int16 a1, uint16_t* a2, int32_t a3, __int16* a4, int16_t mapEntityIndex_15B4E0[]);
unsigned int sub_439A0_orig(unsigned __int16 a2, uint8_t mapHeightmap_11B4E0[], uint8_t mapAngle_13B4E0[]);
void sub_43B40_orig(uint8_t x_BYTE_D41B7, uint8_t mapHeightmap_11B4E0[], uint8_t mapAngle_13B4E0[], uint8_t* x_BYTE_14B4E0_second_heightmap);
void sub_43BB0_orig(uint8_t mapHeightmap_11B4E0[], uint8_t mapAngle_13B4E0[], uint8_t* x_BYTE_14B4E0_second_heightmap);
void sub_44580_orig(uint8_t* pdwScreenBuffer_351628, uint16_t &x_WORD_17B4E0, char x_BYTE_F2CD0x[][2], uint8_t mapTerrainType_10B4E0[], uint8_t mapAngle_13B4E0[]);
void sub_43EE0_orig(uint8_t mapHeightmap_11B4E0[], uint8_t mapAngle_13B4E0[]);
void sub_43FC0_orig(uint8_t mapAngle_13B4E0[]);
void sub_45600_orig(uint8_t a1, uint8_t mapTerrainType_10B4E0[], uint8_t mapHeightmap_11B4E0[], uint8_t mapAngle_13B4E0[]);