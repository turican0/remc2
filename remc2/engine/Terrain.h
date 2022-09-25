#pragma once

#ifndef MAIN_TERRAIN
#define MAIN_TERRAIN

#include "Basic.h"
#include "Unk_D4A30.h"
#include "Unk_D47E0.h"

extern char x_BYTE_F2CD0x[7 * 7 * 7 * 7][2]; // 233cd0//4802 //4816
extern uint8_t x_BYTE_D41B7;

extern uint16_t x_WORD_17B4E0;

extern uint8_t mapTerrainType_10B4E0[65536];
extern uint8_t mapHeightmap_11B4E0[65536];
extern uint8_t mapShading_12B4E0[65536];
extern uint8_t mapAngle_13B4E0[65536];
extern __int16 mapEntityIndex_15B4E0[65536];

extern bool lowDiffHeightmap_D47DC;

void GenerateLevelMap_43830(type_str_2FECE* a2x);//224830
void sub_B5E70_decompress_terrain_map_level(__int16 a1, unsigned __int16 a2, __int16 a3, int32_t a4);//296e70
void sub_44DB0_truncTerrainHeight(int16_t mapEntityIndex_15B4E0[], uint8_t mapHeightmap_11B4E0[]);//225db0 // map to heightmap
int sub_B5C60_getTerrainAlt2(uint16_t a1, uint16_t a2);//296c60
void sub_44E40(int a1, uint8_t a2);//225e40
void sub_45AA0_setMax4Tiles();//226aa0
void sub_440D0(unsigned __int16 a1);//2250d0
void sub_45060(uint8_t a1, uint8_t a2);//226060
void sub_44320();//225320
void sub_45210(uint8_t a1, uint8_t a2);//226210
void sub_454F0(uint8_t a1, uint8_t a2);//2264f0
void sub_45600(uint8_t a1);//226600
void sub_43FC0();//224fc0
void sub_43970();//224970
void sub_43EE0();//224ee0
void sub_44580();//225580
void sub_43B40();//224b40
void sub_43D50();//224d50
void sub_44D00();//225d00
void sub_B5EFA(__int16 a1, uaxis_2d* a2, int32_t a3, int16_t* nextRand);//296EFA
void sub_B5F8F(__int16 a1, uaxis_2d* a2, int32_t a3, int16_t* nextRand);//296f8f
void sub_44EE0_smooth_tiles(/*int a1,*/ uaxis_2d a2x);//225ee0
unsigned int sub_439A0(uint16_t index);//2249a0
void sub_43BB0();//224bb0

int sub_1B7A0_tile_compare(axis_3d* a1);
int sub_1B830(axis_3d* a1);
uint8_t sub_45BE0(uint8_t a2, uaxis_2d a3x);//226be0
bool sub_33F70(uint16_t inAxis);

void sub_45DC0(/*uint8_t a1,*/ uint8_t a2, uaxis_2d a3, unsigned __int8 a4);
void sub_462A0(uaxis_2d a1x, uaxis_2d a2x);

signed int sub_104A0(axis_3d* a1);
uint32_t sub_104D0_terrain_tile_is_water(axis_3d* a1);
uint32_t sub_10590_terrain_tile_type(char a1);
int getTerrainAlt_10C40(axis_3d* a1);
bool sub_11E70(type_event_0x6E8E* a1, axis_3d* a2);
int sub_10C60(axis_3d* a1);
int sub_B5D68(uint16_t a1, uint16_t a2);

#endif //MAIN_TERRAIN