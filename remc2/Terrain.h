#ifndef MAIN_TERRAIN
#define MAIN_TERRAIN

#include "Basic.h"

extern uint8_t unk_D4A30[];//terrain gen
extern char x_BYTE_F2CD0x[7 * 7 * 7 * 7][2]; // 233cd0//4802 //4816
extern uint8_t x_BYTE_D41B7; // weak

void sub_43830_generate_level_map(unsigned int a1, type_str_2FECE* a2x);//224830
void sub_B5E70_decompress_terrain_map_level(__int16 a1, unsigned __int16 a2, __int16 a3, int32_t a4);//296e70
void sub_44DB0_truncTerrainHeight();//225db0 // map to heightmap
void sub_44E40(int a1, uint8_t a2);//225e40
void sub_45AA0_setMax4Tiles();//226aa0
void sub_440D0(unsigned __int16 a1);//2250d0
void sub_45060(uint8_t a1, uint8_t a2);//226060
void sub_44320();//225320
void sub_45210(uint8_t a1, uint8_t a2);//226210
void sub_454F0(uint8_t a1, uint8_t a2);//2264f0
void sub_45600(uint8_t a1);//226600
void sub_43FC0();//224fc0
void sub_43970(uint32_t a1);//224970
void sub_43EE0();//224ee0
void sub_44580();//225580
void sub_43B40();//224b40
void sub_43D50();//224d50
void sub_44D00();//225d00
void sub_B5EFA(__int16 a1, uint16_t* a2, int32_t a3, __int16* a4);//296EFA
void sub_B5F8F(__int16 a1, uint16_t* a2, int32_t a3, __int16* a4);//296f8f
void sub_44EE0_smooth_tiles(/*int a1,*/ uaxis_2d a2x);//225ee0
unsigned int sub_439A0(unsigned int a1, unsigned __int16 a2);//2249a0
void sub_43BB0();//224bb0


#endif //MAIN_TERRAIN