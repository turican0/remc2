#pragma once
#ifndef ENGINE_SUPPORT_H
#define ENGINE_SUPPORT_H

#include <stdio.h>
#include <cstdint>
#include <assert.h>
#include <sstream>
#include <stddef.h>
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */

#include "png.h"
#pragma comment(lib, "zlib.lib") // must be before libpng!
#ifndef _WIN64
#pragma comment(lib, "libpng15.lib") // must be after zlib!
#else
#endif

#include "../portability/port_time.h"
#include "../portability/port_sdl_vga_mouse.h"

//#define USE_DOSBOX

/*
// sign flag
template<class T> int8 __SETS__x(T x)
{
	if (sizeof(T) == 1)
		return int8(x) < 0;
	if (sizeof(T) == 2)
		return int16(x) < 0;
	if (sizeof(T) == 4)
		return int32(x) < 0;
	//return int64(x) < 0;
}

// overflow flag of addition (x+y)
template<class T, class U> int8 __OFADD__x(T x, U y)
{
	if (sizeof(T) < sizeof(U))
	{
		U x2 = x;
		int8 sx = __SETS__x(x2);
		return ((1 ^ sx) ^ __SETS__x(y)) & (sx ^ __SETS__x(x2 + y));
	}
	else
	{
		T y2 = y;
		int8 sx = __SETS__x(x);
		return ((1 ^ sx) ^ __SETS__x(y2)) & (sx ^ __SETS__x(x + y2));
	}
}
*/
void allert_error();

extern uint8_t* readbuffer;
extern char* printbuffer;//char* buffer; // [esp+0h] [ebp-2h]
extern char* printbuffer2;//char v11; // [esp+40h] [ebp+3Eh]
//extern uint8_t *x_DWORD_E9C38_smalltit; // weak
/*extern int32_t x_DWORD_D41A4_x6x;
extern int32_t x_DWORD_D41A4_x8x;
extern int32_t x_DWORD_D41A4_xAx;
extern int32_t x_DWORD_D41A4_xBx;
extern int32_t x_DWORD_D41A4_xCx;
extern int32_t x_DWORD_D41A4_xDx;
extern char x_DWORD_D41A4_xB6;//2A1644 b6=182
extern char x_DWORD_D41A4_x16x;//2A15A4 16=22
extern uint8_t x_DWORD_D41A4_x17x;
extern uint8_t x_DWORD_D41A4_x18x;
extern char x_DWORD_D41A4_x19x;//2A51BD 19=25
extern uint8_t x_DWORD_D41A4_x1Ex;
extern uint8_t x_DWORD_D41A4_x2Bx;
extern uint8_t x_DWORD_D41A4_x2Dx;
extern int32_t x_DWORD_D41A4_x33x;
extern int32_t x_DWORD_D41A4_x59x;
extern int32_t x_DWORD_D41A4_x79x;
extern int32_t x_DWORD_D41A4_x7Ax;
extern int32_t x_DWORD_D41A4_x7Bx;
extern int32_t x_DWORD_D41A4_x7Cx;
extern int32_t x_DWORD_D41A4_x7Dx;
extern int32_t x_DWORD_D41A4_x7Fx;
extern int32_t x_DWORD_D41A4_xB2;
extern int32_t x_DWORD_D41A4_xB4;
extern int32_t x_DWORD_D41A4_xB7;
extern int32_t x_DWORD_D41A4_xBA;
extern int32_t x_DWORD_D41A4_xC0;
extern int32_t x_DWORD_D41A4_xCE;
extern int32_t x_DWORD_D41A4_xCD;
extern int32_t x_DWORD_D41A4_xCF;
extern int32_t x_DWORD_D41A4_xD0;
extern int32_t x_DWORD_D41A4_xD8;
extern int32_t x_DWORD_D41A4_xDC;
extern int32_t x_DWORD_D41A4_xE0;
extern int32_t x_DWORD_D41A4_xE1;
extern int32_t x_DWORD_D41A4_xE2;
extern int32_t x_DWORD_D41A4_xE6;
extern int32_t x_DWORD_D41A4_xF2;
extern int32_t x_DWORD_D41A4_xF6;
extern int32_t x_DWORD_D41A4_x100;
extern int32_t x_DWORD_D41A4_x235;
extern int32_t x_DWORD_D41A4_x749;
extern int32_t x_DWORD_D41A4_x8CF;
extern int32_t x_DWORD_D41A4_x94C;
extern int32_t x_DWORD_D41A4_x954;
extern int32_t x_DWORD_D41A4_x21AA;
extern int32_t x_DWORD_D41A4_x00;
extern int32_t x_DWORD_D41A4_x9602;
extern int32_t x_DWORD_D41A4_x9603;
extern int32_t x_DWORD_D41A4_x966F;
extern int32_t x_DWORD_D41A4_x9677;
extern int32_t x_DWORD_D41A4_x967B;
extern int32_t x_DWORD_D41A4_x967F;
extern int32_t x_DWORD_D41A4_x9691;
extern int32_t x_DWORD_D41A4_x9692;
extern int32_t x_DWORD_D41A4_x4;
extern int32_t x_DWORD_D41A4_x965B;
extern int32_t x_DWORD_D41A4_x9683;
extern int32_t x_DWORD_D41A4_x96BF;*/

//extern int32_t x2124_x_DWORD_D41A4_xCx_x_DWORD_D41A0_11234;

//extern int32_t x_DWORD_D41A4;
//extern uint8_t* x_DWORD_D41A0;

//extern int8_t x_D41A0_BYTEARRAY[];
//extern uint8_t* x_D41A0_BYTEARRAY_0;
//extern uint8_t* x_D41A0_BYTEARRAY_4;

extern uint8_t* off_D41A8_sky;
extern uint8_t* x_BYTE_14B4E0;

#pragma pack (1)
typedef struct {
	uint8_t byte1;
	uint8_t byte2;
}doublebyte;

typedef struct {
	doublebyte dbyte1_2;
	doublebyte dbyte3_4;
}tetrabytebyte;

typedef struct {
	uint16_t x;
	uint16_t y;
}axis_2du;

typedef struct {
	int16_t x;
	int16_t y;
}axis_2d;

typedef struct {
	int32_t x;
	int32_t y;
}axis_2d_32;

typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t z;
}axis_3d;
/*
typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t z;
}axis_3du;
*/
typedef struct {
	int16_t aa;
	int16_t xshift;
	int16_t yshift;
	int16_t dd;
}axis_4d;

typedef struct {
	uint8_t x;
	uint8_t y;
} baxis_2d;

typedef union {
	baxis_2d _axis_2d;
	uint16_t word;
}
uaxis_2d;

typedef struct {
	int8_t str_0x6E3E_byte0;//0x6E3E-28222//D41A0_BYTESTR_0.array_0x6E3E[xx].str_0x6E3E_byte0
	int8_t str_0x6E3E_byte1;//0x6E3F-28223
	int8_t str_0x6E3E_byte2;//0x6E40-28224
	int8_t roll;//0x6E41-28225
	int8_t pitch;//0x6E42-28226
	int8_t str_0x6E3E_byte5;//0x6E43-28227
	uint16_t str_0x6E3E_word6;//0x6E44-28228
	uint16_t str_0x6E3E_word8;//0x6E46-28230
}type_str_0x6E3E;
/*
3-bit-28225
4-bit-28226
5-bit-28227
6-2bit-28228
8-2bit-28230
*/

typedef struct {
	uint8_t byte[0x1a];
	//uint16_t word_2279;
}
type_8_1a;

typedef struct {
	int16_t word[0x1a];
	//uint16_t word_2279;
}
type_16_1a;

typedef struct {
	int32_t dword[0x1a];
	//uint16_t word_2279;
}
type_32_1a;

typedef union {
	uint8_t byte[4];
	uint16_t word[2];
	uint32_t dword;
}
dw_w_b;
typedef struct {//begin 611//str_611
	type_32_1a array_0x263_611x;//size??//dword_0x649_2BDE
	type_32_1a array_0x2CB_715x;//104//size??
	//array_0x2CB_715[2] 723
	type_16_1a array_0x333_819x;//208//size?? spells//word_0x719_2BDE
	//821//array_0x333_819[1]
	//823//array_0x333_819[2]
	//825//array_0x333_819[3]
	//827//array_0x333_819[4]
	//835//array_0x333_819[8]
	//type_16_1a word_0x34B_843;//232//remove it array_0x333_819x.word[0xc]
	//type_16_1a word_0x34F_847;//remove it array_0x333_819x.word[0xe]
	type_16_1a array_0x367_871x;//260//ok
	//875 -array_0x367_871[4]
	type_8_1a array_0x39B_923x;//312//size??//byte_0x781_2BDE
	type_8_1a array_0x3B5_949x;//338//size??//array_0x79B_2BDE_13177
	//int8_t array_0x41D_1053x[0x1a];//size??
	//int8_t array_0x41E_1054;//array_0x41D_1053x[1]
	//int8_t array_0x41F_1055;//array_0x41D_1053x[2]
	//int8_t array_0x420_1056;//array_0x41D_1053x[3]
	//int8_t array_0x422_1058;//array_0x41D_1053x[5]
	//int8_t array_0x423_1059;//array_0x41D_1053x[6]
	//int8_t array_0x424_1060;//array_0x41D_1053x[7]
	//int8_t array_0x425_1061;//array_0x41D_1053x[8]
	//int8_t array_0x428_1064;//array_0x41D_1053x[11]
	//int8_t array_0x433_1075;//array_0x41D_1053x[22]
	//1079 501+611//spell index
	type_8_1a array_0x3CF_975x;//364//byte_0x7B5_2BDE
	type_8_1a array_0x3E9_1001x;//390//size??//array_0x7CF_2BDE_13229
	type_8_1a array_0x403_1027x;//416//size??//byte_0x7E9_2BDE
	type_8_1a array_0x41D_1053z;//442//size??//byte_0x803_2BDE
	type_8_1a array_0x437_1079x;//468//size?? //table of spell indexes//byte_0x81D_2BDE
	int16_t word_0x451_1105;//494 act left spell index//word_0x837_2BDE
	int16_t word_0x453_1107;//496 act right spell index//word_0x839_2BDE
	int8_t byte_0x455_1109;//498
	int8_t byte_0x456_1110;//499
	int8_t byte_0x457_1111;//500//byte_0x83D_2BDE //select spell/type spell
	int8_t byte_0x458_1112;//501//byte_0x83E_2BDE
	int8_t byte_0x459_1113;//502
	int8_t byte_0x45A_1114;
	int8_t byte_0x45B_1115;
}
type_str_611;

/*typedef struct {
	int16_t x;
	int16_t y;
}
type_str_164_pos;*/

typedef struct {//lenght 18
	int16_t word_0;
	int16_t word_2BDE_12658[8];
}type_str_0x1AC_428;

typedef struct {//size 1136
	int32_t dword_0x0_0;
	int16_t word_0x4_4;
	int16_t word_0x6_6;
	uint8_t stuba[4];
	int16_t word_0xc_12;//final speed
	int16_t word_0xe_14;
	int16_t word_0x10_16;
	//int8_t byte_0x11_17;
	uint8_t stubb[2];
	axis_2d position_backup_20;//position //mouse backup
	int16_t word_0x18_24_next_entity;
	int16_t word_0x1A_26;
	uint8_t stubc[2];
	int16_t word_0x1E_30;
	int16_t word_0x20_32;
	int16_t word_0x22_34;
	int16_t word_0x24_36;
	int16_t word_0x26_38[8];
	//int16_t word_0x2E_46;//word_0x26_38[4]
	int16_t word_0x36_54;
	int16_t word_0x38_56;
	int16_t word_0x3A_58;
	uint16_t array_0x3C_60[16];//size??
	uint16_t array_0x5C_92[16];
	uint8_t stubd[174];
	/*int8_t byte_0xD3_211;
	int8_t byte_0xD4_212;
	int8_t byte_0xD5_213;*/
	int32_t dword_0x12A_298;
	int32_t dword_0x12E_302;
	uint8_t stube[10];
	int32_t dword_0x13C_316;
	uint8_t stubf[2];
	int32_t dword_0x142_322;
	int16_t word_0x146_326;
	int16_t word_0x148_328;
	int16_t word_0x14A_330;
	uint8_t byte_0x14C_332;
	int8_t byte_0x14D_333;
	int8_t byte_0x14E_334;
	int8_t byte_0x14F_335;
	int32_t byte_0x150_336;
	uint8_t byte_0x154_340;
	int16_t word_0x155_341;
	int16_t word_0x157_343;
	int16_t word_0x159_345;
	int8_t array_0x15B_347[8];
	int16_t word_0x163_355;
	int32_t dword_0x165_357;
	int32_t dword_0x169_361;
	int32_t dword_0x16D_365;
	uint8_t stubg[4];
	int32_t dword_0x175_373;
	int32_t dword_0x179_377;
	int32_t dword_0x17D_381;
	int32_t dword_0x181_385;
	int32_t dword_0x185_389;
	int32_t dword_0x189_393;//clock
	int32_t dword_0x18D_397;
	uint8_t stubi[4];
	int8_t byte_0x195_405;
	int8_t byte_0x196_406;
	int8_t byte_0x197_407;
	uint8_t stubj[2];
	int32_t dword_0x19A_410;
	int32_t dword_0x19E_414;
	int16_t word_0x1A2_418;
	int16_t word_0x1A4_420;
	int16_t word_0x1A6_422;//211 x add
	int16_t word_0x1A8_424;//212 y add
	int16_t word_0x1AA_426;//213 z add
	type_str_0x1AC_428 str_0x1AC_428;
	int8_t byte_0x1BE_446;
	int8_t byte_0x1BF_447;
	int8_t byte_0x1C0_448;
	int8_t byte_0x1C1_449;//switch - event? type?
	int16_t word_0x1C2_450;
	uint8_t stubl[56];
	//int8_t array_0x1D4_468[0x1a];
	uint16_t array_0x1FC_508[32];//1ca
	//int16_t word_0x204_516[32];
	//int16_t word_0x242_578;//word_0x204_516[31]
	uint8_t stubm[6];
	int16_t word_0x242_578 = -1;
	int16_t word_0x244_580;
	int16_t word_0x246_582;
	int16_t word_0x248_584;
	int16_t word_0x24A_586;
	int16_t word_0x24C_588;
	int8_t array_0x24E_590[10];//size??
	uint8_t stubn[9];
	int8_t byte_0x261_609;
	int8_t byte_0x262_610;
	type_str_611 str_611;

	int8_t str_611_byte_0x45C_1116;
	int8_t str_611_byte_0x45D_1117;
	int8_t str_611_byte_0x45E_1118;
	int8_t str_611_byte_0x45E_1119;

	//uint8_t stubo[16];
	//611;
}
type_str_164;

typedef struct {//lenght 34
	int16_t word_160_0x0_0;
	int16_t word_160_0x2_2;
	int16_t word_160_0x4_4;
	int16_t word_160_0x6_6;
	int16_t word_160_0x8_8;
	int16_t word_160_0xa_10;
	int16_t word_160_0xc_12;
	int16_t word_160_0xe_14;
	int16_t word_160_0x10_16;//->dword_0xA0_160x->word_160_0x10_16
	int16_t word_160_0x12_18;
	int32_t dword_160_0x14_20;
	uint8_t stuba[2];
	int16_t word_160_0x1a_26;
	int16_t word_160_0x1c_28;
	int16_t word_160_0x1e_30;
	int8_t byte_160_0x20_32;
	uint8_t stubb[1];
}
type_str_160;

//viz. unk_D7BD6

typedef struct
{
	int32_t dword_0x5E_94;//94 //36 lenght
	int16_t word_0x62_98;//98 //4 // index subentity
	int32_t dword_0x64_100;//100 //6
	int16_t word_0x68_104;//104 //10
	uint8_t stub5b[6];
	int32_t dword_0x70_112;//112
	int16_t word_0x74_116;//116
	int16_t word_0x76_118;//118
	int16_t word_0x78_120;//120
	int16_t word_0x7A_122;//122
	int16_t word_0x7C_124;//124
	uint8_t stub5d[2];
	uint16_t word_0x80_128;//128
} type_str_0x5E_94;

typedef struct _str_0x6E8E {//lenght a8//THING
	struct _str_0x6E8E* next_0;
	int32_t dword_0x4;//4 //?bitmap//LIVE
	int32_t dword_0x8;//8 // this is int32_t ?bitmap//MAX LIVE
	dw_w_b struct_byte_0xc_12_15;//12
						//12//struct_byte_0xc_12_15.dbyte1_2.byte1
	//0xfe - all spell
	//int8_t byte_0xd_13;//13//struct_byte_0xc_12_15.dbyte1_2.byte2
	//int8_t byte_0xe_14;//14//struct_byte_0xc_12_15.dbyte3_4.byte1
	//int8_t byte_0xf_15;//15//struct_byte_0xc_12_15.dbyte3_4.byte2
	int32_t dword_0x10_16;//16 // index of array
	uint16_t word_0x14_20;//20 //random seed adress
	uint16_t word_0x16_22;//22
	uint16_t word_0x18_24_next_entity;//24 //next entity index
	uint16_t word_0x1A_26;//26 // index - owner //ID last index
	int16_t word_0x1C_28;//28//rotate1
	int16_t word_0x1E_30;//30//rotate2
	int16_t word_0x20_32;//32//rotate3
	//int8_t byte_0x21_33;//33
	int16_t word_0x22_34;//34//rotate4
	int16_t word_0x24_36;//36 // index subentity
	int16_t word_0x26_38;//38 // index subentity
	uint16_t word_0x28_40;//40//WHO OWNS ME
	uint16_t word_0x2A_42;//42 //maybe int16_t
	int16_t word_0x2C_44;//44 //add to z
	int16_t word_0x2E_46;//46
	uint16_t word_0x30_48;//48
	uint16_t word_0x32_50;//50
	uint16_t word_0x34_52;//52 // index subentity
	uint16_t word_0x36_54;//54
	int8_t byte_0x38_56;//56
	int8_t byte_0x39_57;//57
	int8_t byte_0x3A_58;//58
	int8_t byte_0x3B_59;//59
	int8_t byte_0x3C_60;//60
	int8_t byte_0x3D_61;//61
	uint8_t byte_0x3E_62;//62 //index
	int8_t byte_0x3F_63;//63 //type//MODEL
	//11 - spell
	int8_t byte_0x40_64;//64 //subtype//CLASS
	//0 or 1 - player, 2 - castle,3-ballon
	int8_t byte_0x41_65;//65 //type subentity
	int8_t byte_0x42_66;//66 //subtype subentity
	int8_t byte_0x43_67;//67
	int8_t byte_0x44_68;//68
	uint8_t byte_0x45_69;//69 // index of structure with lenght 14//STATE-
	int8_t byte_0x46_70;//70 // index of bitmap // index of ending sequence
	int8_t byte_0x47_71_xx;//71
	int8_t byte_0x48_72;//72
	int8_t byte_0x49_73;//70
	int16_t word_0x4A_74;
	axis_3d array_0x4C_76;//position//ACTUAL X Y Z
	axis_4d array_0x52_82;
	int16_t word_0x5A_90;
	int8_t byte_0x5C_92;
	int8_t byte_0x5D_93;
	type_str_0x5E_94 str_0x5E_94;

	int16_t word_0x82_130;//130 //add to z//ACTUAL SPEED
	int16_t word_0x84_132;//132//MIN SPEED
	int16_t word_0x86_134;//134//MAX SPEED
	int32_t dword_0x88_136;//136
	int32_t dword_0x8C_140;//140 //wiz name?//ACTUAL MANA
	int32_t dword_0x90_144;//144 0-1000//MAX MANA
	uint16_t word_0x94_148;//148
	int16_t word_0x96_150;//150 // index subentity
	uint16_t word_0x98_152;//152
	axis_3d word_0x9A_154x;//154
	//int16_t word_0x9E_158;//152
	type_str_160* dword_0xA0_160x;//160 //special settings
	//uint16_t word_0xA2_162;//162
	type_str_164* dword_0xA4_164x;//100 // adress of xx
}
type_str_0x6E8E;
/*
	entite
	8-dead
	26-index
	64-type
		1-player
		2-castle
		3-ballon
	144-mana
*/

typedef struct {
	int32_t dword[0x1a];
}
type_dword_0x649_2BDE;

/*
typedef struct {//begin 0x649 //lenght 0x1F9
	type_dword_0x649_2BDE dword_0x649_2BDE;//1609//0x649+0,lenght 0x68(4x0x1a)
	uint8_t stub1[104];
	int16_t word_0x719_2BDE[0x1a];// -2xa1 tj 2x 26//1817
	//1817
	uint8_t stub4c[52];
	//int16_t word_0x731_2BDE;//13071
	int8_t byte_0x781_2BDE[0x1a];//1921
	type_2255ar array_0x79B_2BDE_13177;//0x649+338,lenght 0x18+2
	int8_t byte_0x7B5_2BDE[0x1a];//1973
	//uint8_t array_0x7CF_2BDE_13229[0x34];//1999
	//uint8_t array_0x7CF_2BDE_13229[0x18];//1999
	type_2255ar array_0x7CF_2BDE_13229;//0x649+390,lenght 0x18+2
	//uint8_t stub4e[0x1a];
	//24
	//2

	//int8_t array_0x7E7_2BDE_13253[0x1c];//array_0x7CF_2BDE_13229[0x18]
	type_2255ar byte_0x7E9_2BDE;// -1x26//2026

	type_2255ar byte_0x803_2BDE;// -1x26//2051//0x649+442, lenght 0x18+2

	type_2255ar byte_0x81D_2BDE;// -1x26//2077//0x649+468, lenght 0x18+2
	//uint8_t stub2[1];

	int16_t word_0x837_2BDE;//2103
	int16_t word_0x839_2BDE;//2105
	uint8_t stub4[2];
	int8_t byte_0x83D_2BDE;//2109

	int8_t byte_0x83E_2BDE;//2110 - 501//spell index

	//int8_t byte_0x83E_2BDE;//2110 //spell index
	uint8_t stub5[3];
} type_struct_0x649_2BDE_12839;*/

/*typedef struct {//lenght 2124?
	type_str_0x6E3E str_0x256_0;
	uint8_t stub[14];
	uint32_t str0x256_24;//??lenght
	uint8_t stubc[1581];
	//uint8_t str0x256_24[1585];//??lenght
	type_str_611 str0x256_1609;//??lenght

	int8_t byte_0x45C_1116;
	int8_t byte_0x45D_1117;
	int8_t byte_0x45E_1118;
	int8_t byte_0x45E_1119;

	//0x100//x_D41A0_BYTEARRAY_4_struct.byteindex_256ar
	//280=byteindex_256ar[24];//x_D41A0_BYTEARRAY_4_struct.byteindex_256ar[24]
	//565=byteindex_256ar[309];//0x235//x_D41A0_BYTEARRAY_4_struct.byteindex_256ar[309]
	//1865=byteindex_256ar[1609];//0x749//x_D41A0_BYTEARRAY_4_struct.byteindex_256ar[1609]
	//end 2224
	uint8_t stubb[6];
}
type_str_0x256;*/

typedef struct { // 479 // size 14 tj 0xe
	axis_3d axis_2BDE_11695;//11709 0 2 4
	//int32_t dword_0x1d1_2BDE_11695; //465 //0
	//int32_t dword_0x1d5_2BDE_11699; //469 //4
	//int32_t dword_0x1d9_2BDE_11703; //473 //8
	axis_4d rotation__2BDE_11701;//_2BDE_11715 6 8 10 12 //aa
	//int16_t word_0x1dd_2BDE_11707; //477 //12 //dd
	/*
	int32_t dword_0x1df_2BDE_11709; //479
	int32_t dword_0x1e3_2BDE_11713; //483
	int32_t dword_0x1e7_2BDE_11717; //487
	int16_t word_0x1eb_2BDE_11721; //491*/
} type_struct_0x1d1_2BDE_11695;

typedef struct {//lenght 2124=0x84C
	dw_w_b dw_w_b_0_2BDE_11230;
	//uint8_t byte_0x000_2BDE_11230;//0	//11230 - byte? - ne zacatek struktury
	//uint8_t byte_0x001_2BDF_11231_xx;
	//uint8_t byte_0x002_2BE0_11232;//2	//11232 - byte //2, maybe word
	//uint8_t byte_0x003_2BE0_11233_xx;
	uint8_t byte_0x004_2BE0_11234;//2	//11234 - byte //4
	uint8_t byte_0x005_2BE0_11235;//5
	uint8_t byte_0x006_2BE4_11236;//6	//11236 - byte //6
	int16_t word_0x007_2BE4_11237;//index of player
	int8_t byte_0x009_2BE4_11239;
	uint16_t word_0x00a_2BE4_11240;//10 //11240 - word - index z EA3E4 //0xa
	uint8_t byte_0x00c_2BE0_11242_xx;
	uint8_t byte_0x00d_2BE0_11243_xx;
	int16_t word_0x00e_2BDE_11244; //14
	int16_t word_0x010_2BDE_11246; //16
	int32_t dword_0x012_2BE0_11248;//18 ??
	//uint8_t byte_0x013_2BE0_11249_xx;
	//uint8_t byte_0x014_2BE0_11250_xx;
	//uint8_t byte_0x015_2BE0_11251_xx;

	uint8_t byte_0x016_2BE0_11252_xx;//22
	uint8_t byte_0x017_2BE0_11253_xx;//23
	int32_t dword_0x018_2BDE_11254; //24 //pointer
	char array_0x01c_2BFA_11258[49];//28//11258 - byte* jmeno2?
	int16_t word_0x04d_2C2B_11307;//77 //11307 - word
	uint16_t word_0x04f_2C2D_11309;//79 //11309 - word
	char names_81[8][48];//81 ?8
	//uint8_t stub2[384];

	type_struct_0x1d1_2BDE_11695 struct_0x1d1_2BDE_11695[0x21];//465
	/*int32_t dword_0x1d1_2BDE_11695; //465
	int32_t dword_0x1d5_2BDE_11699; //465
	int32_t dword_0x1d9_2BDE_11703; //465
	int16_t word_0x1dd_2BDE_11707; //465*/
	/*array 0x1d1 // 479 // size 14 tj 0xe
	{
		int32_t dword_0x1d1_2BDE_11695; //465
		int32_t dword_0x1d5_2BDE_11699; //465
		int32_t dword_0x1d9_2BDE_11703; //465
		int16_t word_0x1dd_2BDE_11707; //465

		//int32_t dword_0x1df_2BDE_11709; //479
		//int32_t dword_0x1e3_2BDE_11713; //483
		//int32_t dword_0x1e7_2BDE_11717; //487
		//int16_t word_0x1eb_2BDE_11721; //491
	}*/
	//uint8_t stub2b[308];

	char array_0x39f_2BFA_12157[0x40];//927//12157 - byte(11230+927) 100% name
	uint8_t byte_0x3DF_2BE4_12221;//991//12221 - byte
	uint8_t byte_0x3E0_2BE4_12222;//992//12222 - byte
	uint8_t byte_0x3E1_2BE4_12223;//993//12223 - byte - ok
	uint8_t byte_0x3E2_2BE4_12224;//994//12224 - byte
	uint8_t byte_0x3E3_2BE4_12225;//995
	uint8_t byte_0x3E4_2BE4_12226;//996
	uint8_t stub3[1];
	type_str_164 dword_0x3E6_2BE4_12228;//998//pointer or other
	//uint8_t stub3[61];
	//int16_t word_0x3EA_2BDE;//1002 //dword_0x3E6_2BE4_12228.word_0x4_4
	//uint8_t stub3b[14];
	//int16_t word_0x3FA_2BDE;//1018 mouse_x 20 dword_0x3E6_2BE4_12228.position_20.x
	//int16_t word_0x3FC_2BDE;//1020 mouse_y 22 dword_0x3E6_2BE4_12228.position_20.y
	//uint8_t stub3c[32];
	//int16_t word_0x420_2BE4_12284;//1054 //dword_0x3E6_2BE4_12228.word_0x38_56
	//uint8_t byte_0x420_2BE4_12286;//1056//12286 - byte //dword_0x3E6_2BE4_12228.word_0x3A_58
	//uint8_t stub4[282];
	//uint16_t word_0x53B_2BDE;// 1339 //dword_0x3E6_2BE4_12228.word_0x155_341
	//uint8_t stub4b[30];
	//uint32_t dword_0x55B_2BDE;//1371 dword//12601//dword_0x3E6_2BE4_12228.dword_0x175_373
	//uint32_t dword_0x55F_2BDE;//1375 dword//12605//dword_0x3E6_2BE4_12228.dword_0x179_377
	//uint32_t dword_0x563_2BDE;//1379 dword//12609//dword_0x3E6_2BE4_12228.dword_0x17D_381
	//uint32_t dword_0x567_2BDE;//1383 dword//12613//dword_0x3E6_2BE4_12228.dword_0x181_385
	//uint32_t dword_0x56B_2BDE;//1387 dword//12617//dword_0x3E6_2BE4_12228.dword_0x185_389
	//uint32_t dword_0x56F_2BDE;//1391 dword//12621//dword_0x3E6_2BE4_12228.dword_0x189_393
	//uint32_t dword_2BDE_12621;//1391
	//uint8_t stub4d[33];
	//int16_t word_2BDE_12658[8];//1428//430 dword_0x3E6_2BE4_12228.str_0x1AC_428.word_2BDE_12658
	//uint8_t stub4e[2];
	//int8_t byte_0x5a6_2BE4_12676;//1446//dword_0x3E6_2BE4_12228.byte_0x1C0_448
	//uint8_t stub4c[162];

	//type_struct_0x649_2BDE_12839 struct_0x649_2BDE_12839;//min 0x1ee//1609//611
	//dword_0x3E6_2BE4_12228.str_611
	/*uint8_t stub4b[228];
	uint8_t array_0x7CF_2BDE_13229[24];
	int16_t word_0x7E7_2BDE_13253;*/
	//uint8_t stub4f[4];

	int8_t byte_0x846_2BDE;//2118
	int8_t byte_0x847_2BDE;//2119
	int16_t word_0x848_2BDE;//2120
	int16_t word_0x84A_2BDE;//2122
	//uint8_t stub5[17];
} type_str_0x2BDE;//11230 // 84c lenght

typedef struct {//size 26
	int32_t dword_0;
	int32_t dword_4;
	uint8_t* dword_8_data;
	uint32_t* dword_12x;//uint32_t*
	uint32_t* dword_16x;//uint32_t*
	int16_t word_20;
	int16_t word_22;
	int16_t word_24;
	uint8_t data[];
	//uint32_t dword_26;
} type_x_DWORD_E9C28_str;

typedef struct {
	//uint8_t harddisk_number;

	//uint8_t setting_byte5_19;
	uint32_t dwordindex_0;//x_D41A0_BYTEARRAY_4_struct.dwordindex_0
	uint16_t wordindex_4;//x_D41A0_BYTEARRAY_4_struct.byteindex_4
	uint16_t wordindex_6;//x_D41A0_BYTEARRAY_4_struct.wordindex_6
	uint16_t wordindex_8;//x_D41A0_BYTEARRAY_4_struct.wordindex_8
	uint8_t byteindex_10;//0xa//x_D41A0_BYTEARRAY_4_struct.byteindex_10//show help
	int8_t byte_brightness_11;//0xb//x_D41A0_BYTEARRAY_4_struct.byteindex_11
	int8_t byte_brightness_12;//0xc//x_D41A0_BYTEARRAY_4_struct.byteindex_12
	int8_t byte_brightness_13;//0xd//x_D41A0_BYTEARRAY_4_struct.byteindex_13
	uint16_t wordindex_14;
	uint32_t dwordindex_16;//0x10//x_D41A0_BYTEARRAY_4_struct.dwordindex_16
	uint8_t stubb[2];
	uint8_t setting_byte1_22;//x_D41A0_BYTEARRAY_4_struct.setting_byte1_22
	int8_t setting_byte2_23;//0x17//x_D41A0_BYTEARRAY_4_struct.setting_byte2_23
	uint8_t setting_byte3_24;//cheats? 0x20==free spell//x_D41A0_BYTEARRAY_4_struct.setting_byte3_24
	//spell on - 0x20
	uint8_t setting_byte4_25;//cheats? 0x1==Invincability
	//invincability - 1;
	uint8_t byteindex_26 = 0;//x_D41A0_BYTEARRAY_4_struct.byteindex_26
	uint8_t stubc[3];
	uint8_t setting_30;//x_D41A0_BYTEARRAY_4_struct.setting_30
	uint8_t stubd[3];
	//FILE* moviemvidatfile_byte4_33;
	uint8_t m_wHighSpeedSystem;
	FILE* moviemvidatfile_byte4_35;
	uint16_t moviemvinumber_byte4_39;//x_D41A0_BYTEARRAY_4_struct.moviemvinumber_byte4_39
	uint8_t stube[2];
	uint16_t levelnumber_43w;//x_D41A0_BYTEARRAY_4_struct.levelnumber_43
	//uint8_t stubf[1];
	uint16_t setting_45w;
	uint8_t stubg[3];
	int8_t byteindex_50;//0x32//x_D41A0_BYTEARRAY_4_struct.byteindex_50
	uint8_t byteindex_51;//0x33//x_D41A0_BYTEARRAY_4_struct.byteindex_51
	uint8_t byteindex_52;//0x34//x_D41A0_BYTEARRAY_4_struct.byteindex_52
	uint8_t byteindex_53;//0x35//x_D41A0_BYTEARRAY_4_struct.byteindex_53
	uint8_t stubh[3];
	char player_name_57ar[32];
	char savestring_89[32];
	uint8_t byteindex_121[16];//0x79//x_D41A0_BYTEARRAY_4_struct.byteindex_121
	//uint8_t byteindex_122;//0x7a//x_D41A0_BYTEARRAY_4_struct.byteindex_121[1]
	//uint8_t byteindex_123;//0x7b//x_D41A0_BYTEARRAY_4_struct.byteindex_121[2]
	//uint8_t byteindex_124;//0x7c//x_D41A0_BYTEARRAY_4_struct.byteindex_121[3]
	//uint8_t byteindex_125;//0x7d//x_D41A0_BYTEARRAY_4_struct.byteindex_121[4]
	//uint8_t byteindex_127;//0x7f//x_D41A0_BYTEARRAY_4_struct.byteindex_121[6]
	uint8_t stubi[8];
	char byteindex_145ar[32];//x_D41A0_BYTEARRAY_4_struct.byteindex_145ar

	uint8_t byteindex_177;//0xb1//x_D41A0_BYTEARRAY_4_struct.byteindex_177
	uint8_t speedIndex;//0xb2//x_D41A0_BYTEARRAY_4_struct.byteindex_178
	uint8_t SelectedLangIndex;
	uint8_t byteindex_180;//0xb4//x_D41A0_BYTEARRAY_4_struct.byteindex_180
	uint8_t byteindex_181;//0xb5//x_D41A0_BYTEARRAY_4_struct.byteindex_181
	uint8_t byteindex_183;//0xb7//x_D41A0_BYTEARRAY_4_struct.byteindex_183
	uint16_t byteindex_184w;//0xb8//x_D41A0_BYTEARRAY_4_struct.byteindex_184
	//uint8_t stubj[1];
	uint8_t byteindex_186;//0xba//x_D41A0_BYTEARRAY_4_struct.byteindex_186
	uint8_t stubk[1];
	uint32_t dwordindex_188;//x_D41A0_BYTEARRAY_4_struct.byteindex_188
	//uint8_t byteindex_189;//x_D41A0_BYTEARRAY_4_struct.byteindex_189
	//uint8_t byteindex_190;//x_D41A0_BYTEARRAY_4_struct.byteindex_190

	//1(0x1)-help//2(0x2)-sound//3(0x4)-music//4(0x8)-speek//5(0x10)-fly//6(0x20)-bright//7(0x40)speed//8(0x80)screen
	//9(0x100)-reflection//10(0x200)-sky//11(0x400)-shadows//12(0x800)-light/13(0x1000)-icons//14(0x2000)-transparency//15(0x4000)-flat	//16(0x8000)-resolution
	//17(0x10000)-names

	uint32_t dwordindex_192;//0xc0//x_D41A0_BYTEARRAY_4_struct.byteindex_192
	//uint8_t byteindex_193;//0xc1//x_D41A0_BYTEARRAY_4_struct.byteindex_193
	//uint8_t byteindex_194;//0xc2//x_D41A0_BYTEARRAY_4_struct.byteindex_194

	//1(0x1)-help//2(0x2)-sound//3(0x4)-music//4(0x8)-speek//5(0x10)-fly//6(0x20)-bright//7(0x40)speed//8(0x80)screen
	//9(0x100)-reflection//10(0x200)-sky//11(0x400)-shadows//12(0x800)-light/13(0x1000)-icons//14(0x2000)-transparency//15(0x4000)-flat	//16(0x8000)-resolution
	//17(0x10000)-names

	uint8_t byteindex_196;//0xc4//x_D41A0_BYTEARRAY_4_struct.byteindex_196
	uint8_t stubl[3];
	uint8_t transfer_rate_dwordindex_200;//0xc8//x_D41A0_BYTEARRAY_4_struct.transfer_rate_dwordindex_200
	uint8_t stubm[3];
	uint8_t byteindex_204;//0xCC//x_D41A0_BYTEARRAY_4_struct.byteindex_204
	uint8_t byteindex_205;//0xCD//x_D41A0_BYTEARRAY_4_struct.byteindex_205
	uint8_t byteindex_206;//0xce//x_D41A0_BYTEARRAY_4_struct.byteindex_206
	uint8_t byteindex_207;//0xcf//x_D41A0_BYTEARRAY_4_struct.byteindex_207
	uint8_t byteindex_208;//0xd0//x_D41A0_BYTEARRAY_4_struct.byteindex_208
	uint8_t stubmm[1];
	uint32_t byteindex_210d;//0xd2//x_D41A0_BYTEARRAY_4_struct.byteindex_210
	//uint8_t stubn[3];
	uint16_t byteindex_214w;//0xd6//x_D41A0_BYTEARRAY_4_struct.byteindex_214
	//uint8_t stubo[1];
	uint16_t setting_216w;
	//uint8_t stubp[1];
	uint16_t byteindex_218w;//0xda//x_D41A0_BYTEARRAY_4_struct.byteindex_218
	//uint8_t stubq[1];

	uint8_t byteindex_220;//0xdc//x_D41A0_BYTEARRAY_4_struct.byteindex_220
	uint8_t byteindex_221ar[3];//0xe0//x_D41A0_BYTEARRAY_4_struct.byteindex_221ar
	uint8_t byteindex_224;//0xe0//x_D41A0_BYTEARRAY_4_struct.byteindex_224
	uint8_t byteindex_225;//0xe1//x_D41A0_BYTEARRAY_4_struct.byteindex_225
	uint8_t* pointer_0xE2_heapbuffer_226;//x_D41A0_BYTEARRAY_4_struct.pointer_0xE2_heapbuffer_226
	uint32_t dword_0xE6_heapsize_230;//x_D41A0_BYTEARRAY_4_struct.dword_0xE6_heapsize_230
	uint8_t stubr[8];
	uint8_t byteindex_242ar[14];//0xf2//x_D41A0_BYTEARRAY_4_struct.byteindex_242ar
	//uint8_t byteindex_246;//0xf6//x_D41A0_BYTEARRAY_4_struct.byteindex_242ar[4]
	//uint8_t byteindex_250;//x_D41A0_BYTEARRAY_4_struct.byteindex_242ar[8]

	type_str_0x2BDE byteindex_256ar;//0x100//x_D41A0_BYTEARRAY_4_struct.byteindex_256ar
	//280=byteindex_256ar[24];//x_D41A0_BYTEARRAY_4_struct.byteindex_256ar[24]
	//565=byteindex_256ar[309];//0x235//x_D41A0_BYTEARRAY_4_struct.byteindex_256ar[309]
	//1865=byteindex_256ar[1609];//0x749//x_D41A0_BYTEARRAY_4_struct.byteindex_256ar[1609]
	//end 2224

	//uint8_t byteindex_2255ar[26];//0x100//x_D41A0_BYTEARRAY_4_struct.byteindex_2255ar
	//type_2255ar str_2255ar;
	//end 2281
	//uint8_t stubs[9];
	uint32_t dwordindex_2380;//0x94c//x_D41A0_BYTEARRAY_4_struct.dwordindex_2380
	uint8_t stubt[4];
	uint32_t dwordindex_2388;//0x954//x_D41A0_BYTEARRAY_4_struct.dwordindex_2388
	uint32_t dwordindex_2392;//0x958//x_D41A0_BYTEARRAY_4_struct.dwordindex_2392

	//uint8_t byteindex_8618;//0x21aa//x_D41A0_BYTEARRAY_4_struct.byteindex_8618
	//uint8_t setting_38545;
	uint8_t stubu[36000];
	type_str_0x6E8E* dwordindex_38396;//0x95FC//x_D41A0_BYTEARRAY_4_struct.dwordindex_38396
	uint8_t byteindex_38400;//0x9600//x_D41A0_BYTEARRAY_4_struct.byteindex_38400
	uint8_t byteindex_38401;//0x9601//x_D41A0_BYTEARRAY_4_struct.byteindex_38401

	uint8_t setting_38402;//0x9602//x_D41A0_BYTEARRAY_4_struct.setting_38402

	type_str_0x6E8E* bytearray_38403x[30];//array 116//0x9603//x_D41A0_BYTEARRAY_4_struct.bytearray_38403
	/*
	creatures
		8-dead
		69-dead
	*/
	//bytearray_38419=bytearray_38403[16]//0x965b//x_D41A0_BYTEARRAY_4_struct.bytearray_38403[16]
	//bytearray_38439=bytearray_38403[36]//0x965b//x_D41A0_BYTEARRAY_4_struct.bytearray_38403[36]
	//bytearray_38403[88]//uint32_t dword_38491;//0x965b//x_D41A0_BYTEARRAY_4_struct.bytearray_38403[88]

	//bytearray_38451=bytearray_38403[48]//0x965b//x_D41A0_BYTEARRAY_4_struct.bytearray_38403[48]
	//bytearray_38455=bytearray_38403[52]//0x965b//x_D41A0_BYTEARRAY_4_struct.bytearray_38403[52]

	//bytearray_38511=bytearray_38403[108]//0x965b//x_D41A0_BYTEARRAY_4_struct.bytearray_38403[108]
	//bytearray_38515=bytearray_38403[112]//0x965b//x_D41A0_BYTEARRAY_4_struct.bytearray_38403[112]

	//bytearray_38535=bytearray_38403[132]//0x965b//x_D41A0_BYTEARRAY_4_struct.bytearray_38403[132]

	//endarray - dword_38519
	type_str_0x6E8E* dword_38519;//0x9677//x_D41A0_BYTEARRAY_4_struct.dword_38519
	/*
	entite
	8-dead
	26-index
	64-type
		1-player
		2-castle
		3-ballon
	144-mana
	*/
	type_str_0x6E8E* dword_38523;//0x967b//x_D41A0_BYTEARRAY_4_struct.dword_38523
	type_str_0x6E8E* dword_38527;//0x967F//x_D41A0_BYTEARRAY_4_struct.dword_38527
	type_str_0x6E8E* dword_38531;//0x9683//x_D41A0_BYTEARRAY_4_struct.dword_38531
	type_str_0x6E8E* dword_38535;//x_D41A0_BYTEARRAY_4_struct.dword_38535
	uint8_t stubv[5];
	uint8_t byte_38544;//x_D41A0_BYTEARRAY_4_struct.byte_38544
	uint8_t setting_38545;//0x9691//x_D41A0_BYTEARRAY_4_struct.setting_38545
	uint8_t byte_38546;//0x9692//x_D41A0_BYTEARRAY_4_struct.byte_38546
	uint8_t stubw[44];
	uint8_t byte_38591;//0x96BF//x_D41A0_BYTEARRAY_4_struct.byte_38591

	//uint32_t dword_616432;
}type_x_D41A0_BYTEARRAY_4_struct;

doublebyte doublebyte_conv(uint16_t a2);

extern type_x_D41A0_BYTEARRAY_4_struct x_D41A0_BYTEARRAY_4_struct;
extern int16_t x_D41A0_WORDARRAY[];

//extern uint32_t x_D41A0_BYTEARRAY_4_0xE6_heapsize;
//extern uint8_t* x_D41A0_BYTEARRAY_4_0xE2_heapbuffer;
//extern uint8_t* x_D41A0_BYTEARRAY_4_0xDE_heapbuffer;
//extern uint32_t* off_D918C[0x7c];

//xx extern uint8_t* dword_E9C30[]; // weak

extern posistruct_t* xy_DWORD_17DED4_spritestr;
extern posistruct_t* xy_DWORD_17DEC0_spritestr;
extern posistruct_t* xy_DWORD_17DEC8_spritestr;

extern posistruct_t* x_DWORD_D4188t_spritestr;

/*extern x_DWORD x_DWORD_355208;//3551CE+3A DWORD
extern x_BYTE x_BYTE_355234_hardisknumber;//harddrive//3551CE+66 BYTE
extern x_BYTE x_BYTE_35520C;//3551CE+3e BYTE
extern x_BYTE x_BYTE_355210;//3551CE+42 BYTE
extern x_BYTE x_BYTE_355230;//3551CE+62 BYTE
extern x_BYTE x_BYTE_355218;//3551CE+4a BYTE
extern x_BYTE x_BYTE_355244;//aSpellsedit//3551CE+76 BYTE
extern x_BYTE x_BYTE_355240;//3551CE+72 BYTE
extern x_BYTE x_BYTE_35522C;//nocd//3551CE+5e BYTE
extern x_BYTE x_BYTE_355224;//Showversion//3551CE+56 BYTE
extern x_BYTE x_BYTE_355228;//Showversion2//3551CE+5a BYTE
extern x_BYTE x_BYTE_355214;//3551CE+46 BYTE
extern x_BYTE x_BYTE_35521C;//3551CE+4e BYTE
extern x_BYTE x_BYTE_35523C;//Detectoff//3551CE+6e BYTE
extern x_BYTE x_BYTE_355220;//Langcheck//3551CE+52 BYTE
extern x_BYTE x_BYTE_355238;//aMusic2//3551CE+6a BYTE*/

extern uint8_t* pdwScreenBuffer;

typedef struct {//lenght 30
	int16_t word_0;//str_E2A74[].word_0
	int16_t axis_2[5]; //str_E2A74[].axis_2[0]
	//int16_t axis_4[3];//?? str_E2A74[].axis_2[0]
	//int16_t stuba;
	type_str_0x6E8E* dword_12;//str_E2A74[].dword_12
	int32_t dword_16;
	int32_t dword_20;//str_E2A74[].dword_20
	int32_t dword_24;//str_E2A74[].dword_24
	//int8_t byte_25;
	int8_t byte_28;
	int8_t byte_29;
}
type_str_E2A74;

extern type_str_E2A74 str_E2A74[0x69];

//extern uint8_t test_str_E2A74[];

//extern char* char_355198;
//void pathfix(char* path, char* path2);
void support_begin();
void support_end();

void loadfromsnapshot(char* filename, uint8_t* adress, uint32_t adressdos, uint32_t size);
void loadfromsnapshot2(char* filename, uint8_t* adress, uint32_t adressdos, uint32_t size);
uint32_t compare_with_snapshot(char* filename, uint8_t* adress, uint32_t adressdos, uint32_t size, uint8_t* origbyte, uint8_t* copybyte);
uint32_t compare_with_sequence(char* filename, uint8_t* adress, uint32_t adressdos, long count, long size1, uint32_t size2, uint8_t* origbyte, uint8_t* copybyte, long offset = 0);
uint32_t compare_with_sequence_E7EE0(char* filename, uint8_t* adress, uint32_t adressdos, uint32_t count, uint32_t size1, uint32_t size2, uint8_t* origbyte, uint8_t* copybyte, long offset = 0);
uint32_t compare_with_snapshot_D41A0(char* filename, uint8_t* adress, uint32_t adressdos, uint32_t size, uint8_t* origbyte, uint8_t* copybyte);
uint32_t compare_with_sequence_D41A0(char* filename, uint8_t* adress, uint32_t adressdos, uint32_t count, uint32_t size, uint8_t* origbyte, uint8_t* copybyte, long offset = 0);
uint32_t compare_with_sequence_array_E2A74(char* filename, uint8_t* adress, uint32_t adressdos, uint32_t count, uint32_t size1, uint32_t size2, uint8_t* origbyte, uint8_t* copybyte, long offset = 0);
uint32_t compare_with_sequence_x_DWORD_F2C20ar(char* filename, uint8_t* adress, uint32_t adressdos, uint32_t count, uint32_t size, uint8_t* origbyte, uint8_t* copybyte, int* posdiff);
uint32_t compare_with_sequence_array_222BD3(char* filename, uint8_t* adress, uint32_t adressdos, uint32_t count, uint32_t size, uint8_t* origbyte, uint8_t* copybyte, int* posdiff);
uint32_t compare_with_sequence_D41A0_4(char* filename, uint8_t* adress, uint32_t adressdos, uint32_t count, uint32_t size, uint8_t* origbyte, uint8_t* copybyte, long offset = 0);
uint32_t compare_with_sequence_EA3E4(char* filename, type_str_0x6E8E** adress, uint32_t count, uint32_t size, uint8_t* origbyte, uint8_t* copybyte);
uint32_t compare_0x6E8E(char* filename, uint8_t* adress, uint32_t count, uint32_t size, uint8_t* origbyte, uint8_t* copybyte, long offset = 0);
void add_compare(uint32_t adress, bool debugafterload, int stopstep = -1, bool skip = false);

void writehex(uint8_t* buffer, uint32_t count);

void mine_texts(char* filename, uint32_t adressdos, uint32_t count, char* outfilename);
int writeImage(char* filename, int width, int height, uint8_t* buffer, char* title);
void writeImageBMP(char* imageFileName, int width, int height, uint8_t* image);

/*
typedef struct {//lenght 8
	uint8_t byte_0;//array_0x365F4
	uint8_t byte_1;//array_0x365F5
	uint8_t byte_2;//array_0x365F6
	uint8_t byte_3;//array_0x365F7
	int16_t word_4;//array_0x365F8
	int16_t word_6;//array_0x365FC
} type_str_0x365F4;
*/
/*
typedef struct {
	uint16_t word0;
	uint16_t word1;
} type_str_double_word;
*/
/*
typedef struct {//lenght 8
	uint8_t str_0x3647A_byte_0;//array_0x3647a
	uint8_t str_0x3647A_byte_1;//array_0x3647b
	int16_t str_0x3647A_word_2;//array_0x3647c
	axis_2d str_0x365F8_axis;//
} type_str_0x365F4;
*/

typedef union {
	axis_2du axis;
	type_str_0x6E8E* pointer_0x6E8E;
	//uint32_t dword;
}
un1;

typedef struct {//lenght 8
	//uint8_t byte_0;//array_0x36479
	int8_t str_0x3647A_byte_0;//array_0x3647a
	int8_t str_0x3647A_byte_1;//array_0x3647b
	uaxis_2d str_0x3647A_2;//array_0x3647c//str_0x3647A_2.word
	un1 str_0x3647C_4;//array_0x3647e
	/*
	axis_2d axis;//array_0x3647e
	type_str_0x6E8E* pointer_0x6E8E;//36484
	*/

	//type_str_0x6E8E* str_0x3647C;//
} type_str_0x3647Ac;

typedef struct {//lenght 8
	//uint8_t byte_0;//array_0x36479
	uint8_t str_0x3647A_byte_0;//array_0x3647a
	uint8_t str_0x3647A_byte_1;//array_0x3647b
	int16_t str_0x3647A_word_2;//array_0x3647c
	uint32_t str_0x3647A_dword_4;//array_0x3647e
	//int16_t str_0x3647A_word_6;//array_0x36480
} type_str_0x3647Ab;
/*
typedef struct {//lenght 20
	uint8_t byte_0;//array_0x3030E//0x30322
	uint8_t byte_1;//array_0x3650F//0x30323
	uint8_t byte_2;//array_0x36510//0x30324
	uint8_t byte_3;//array_0x36511//0x30325
	uint8_t byte_4;//array_0x36512//0x30326
	int16_t word_5;//array_0x36513//0x30327
	uint8_t stuba[8];//array_0x36515//0x30329
	uint8_t* anystruct;//array_0x3651d//0x30331
	uint8_t stubb[1];
} type_str_0x3030E;*/

/*typedef struct {//lenght 20
	uint8_t byte_0;//type_str_0x30310//type_str_0x30324
	uint8_t str_0x30310_word_1;//type_str_0x30311//type_str_0x30325
	uint8_t byte_2;//type_str_0x30312//type_str_0x30326
	int16_t str_0x30310_word_3;//type_str_0x30313//type_str_0x30327
	//uint8_t byte_4;//type_str_0x30314//type_str_0x30328
	uint8_t byte_5;//type_str_0x30315//type_str_0x30329
	uint8_t byte_6;//type_str_0x30316//type_str_0x3032a
	uint8_t byte_7;//type_str_0x30317//type_str_0x3032b
	uint8_t byte_8;//type_str_0x30318//type_str_0x3032c
	uint8_t byte_9;//type_str_0x30319//type_str_0x3032d
	uint8_t byte_10;//type_str_0x3031a//type_str_0x3032e
	uint8_t byte_11;//type_str_0x3031b//type_str_0x3032f
	uint8_t byte_12;//type_str_0x3031c//type_str_0x30330
	uint8_t str_0x30310_byte_13;//type_str_0x3031d//type_str_0x30331
	uint8_t byte_14;//type_str_0x3031e//type_str_0x30332
	uint8_t byte_15;//type_str_0x3031f//type_str_0x30333
	uint8_t byte_16;//type_str_0x30320//type_str_0x30334
	uint8_t byte_17;//type_str_0x30321//type_str_0x30335
	uint8_t byte_18;//type_str_0x30322//type_str_0x30336
	uint8_t byte_19;//type_str_0x30323//type_str_0x30337
}
type_str_0x30310;*/
typedef struct {//lenght 20
	//uint8_t byte_0;//type_str_0x30310//type_str_0x30324
	uint16_t str_0x30311_type;//type_str_0x30311//type_str_0x30325//1091
	//uint8_t byte_1;//type_str_0x30312//type_str_0x30326
	int16_t str_0x30311_subtype;//type_str_0x30313//type_str_0x30327//1091
	//uint8_t byte_4;//type_str_0x30314//type_str_0x30328
	//axis_3d axis3d_4;//type_str_0x30315//type_str_0x30329
	axis_2du axis2d_4;
	uint16_t DisId;
	//uint8_t byte_5;//type_str_0x30316//type_str_0x3032a
	//uint16_t word_6;//type_str_0x30317//type_str_0x3032b
	//uint8_t byte_7;//type_str_0x30318//type_str_0x3032c
	//uint16_t word_8;//type_str_0x30319//type_str_0x3032d
	uint16_t word_10;//type_str_0x3031b//type_str_0x3032f
	//uint8_t byte_11;//type_str_0x3031c//type_str_0x30330
	uint16_t word_12;//type_str_0x3031d//type_str_0x30331
	//uint8_t byte_13;//type_str_0x3031e//type_str_0x30332
	uint16_t word_14;//type_str_0x3031f//type_str_0x30333//1105
	//uint8_t byte_15;//type_str_0x30320//type_str_0x30334
	uint16_t word_16;//type_str_0x30321//type_str_0x30335
	//uint8_t byte_17;//type_str_0x30322//type_str_0x30336
	uint16_t word_18;//type_str_0x30323//type_str_0x30337
	//uint8_t byte_19;//type_str_0x30324//type_str_0x30338
}
type_str_0x30311;

/*
typedef struct {//lenght 20
	uint8_t byte_0;//array_0x3030E
	uint8_t byte_1;//array_0x3650F
	uint8_t byte_2;//array_0x36510
	uint8_t byte_3;//array_0x36511
	uint8_t byte_4;//array_0x36512
	int16_t word_5;//array_0x36513
	uint8_t stub[13];//array_0x36515
} type_str_0x30325;
*/
/*
typedef struct {//begin 0x649 //lenght min 0x1ee
	int32_t dword_0x649_2BDE[0x33];//1609
	uint8_t stub1[2];
	int16_t word_0x717_2BDE[0x1a];// -2xa1 tj 2x 26//1815
	//1817
	uint8_t stub4c[54];
	int16_t word_0x731_2BDE;//13071
	int8_t byte_0x781_2BDE[0x1a];//1921
	uint8_t stub4d[0x1a];
	int8_t byte_0x7B5_2BDE[0x1a];//1973
	//uint8_t array_0x7CF_2BDE_13229[0x34];//1999
	//uint8_t array_0x7CF_2BDE_13229[0x18];//1999
	type_2255ar array_0x7CF_2BDE_13229;
	uint8_t stub4e[0x1a];
	//24
	//2

	//int8_t array_0x7E7_2BDE_13253[0x1c];//array_0x7CF_2BDE_13229[0x18]
	//int8_t byte_0x7E8_2BDE[0x1a];// -1x26//2024

	int8_t byte_0x803_2BDE[0x19];// -1x26//2051

	int8_t byte_0x81C_2BDE[0x1a];// -1x26//2076
	uint8_t stub2[1];
} type_struct_0x649_2BDE_12839;
*/

/*
typedef struct {//lenght 8
	int8_t str_0x65ac_byte_0;//0x65ac
	int8_t str_0x65ac_byte_1;//0x65ad
	int16_t str_0x65ac_word_2;//0x65ae
	//int8_t str_0x65ac_byte_3;//0x65af
	int16_t str_0x65ac_word_4;//0x65b0
	int16_t str_0x65ac_word_6;//0x65b2
}
type_str_0x65ac;
*/
//11230 - byte? - ne zacatek struktury
//11232 - byte //2
//11236 - byte //6
//11240 - word - index z EA3E4 //0xa
//11258 - byte* jmeno?
//11307 - word
//11309 - word
//12157 - byte(11230+927) 100% jmeno
//12221 - byte
//12223 - byte
//12224 - byte
//12286 - byte

typedef struct {//lenght 9377
	uint8_t stub[9377];
} type_str_0x24DF;

/*
typedef struct {//lenght 110
	uint8_t byte_0x360FB;
	uint8_t byte_0x360E1;
	uint8_t stuba[0x19];
	uint8_t byte_0x36115;
	//0x36141;//70
	//0x36141[8];??
	uint8_t stubb[109];
}
type_str_0x360FB;*/

typedef struct {//lenght 110 //word_0x360D2
	uint8_t stuba[3];
	int16_t word_0x360D5;
	uint8_t stubb[2];
	int16_t word_0x360D9;
	uint8_t stubc[2];
	int16_t word_0x360DD;
	uint8_t stubd[2];
	uint8_t byte_0x360E1x[26];
	uint8_t byte_0x360FBx[26];
	//uint8_t byte_0x36112;
	//uint8_t stubb[0x2];
	uint8_t byte_0x36115x[26];
	//uint8_t stube[25];
	int16_t word_0x3612F;
	//uint8_t byte_0x0x36140;
	uint8_t stubf[15];
}
type_str_0x360D2;

//objective type
//0 - collect mana
//1 - kill creature2 - must fix entites
//2 -
//3 - kill enemy player
//4 -
//5 - release point
//6 -
//7 - kill creature - must fix entites
//8 - kill all players
//9 - destroy building - must fix entites

typedef struct {
	int8_t byte_0;
	int16_t word_1;//25973//word_0
	axis_2d _axis_2d;
	//int16_t word_3;//25975//word_2
	//int16_t word_5; //25977//word_4
	/*
	int16_t word_0;//25973
	int16_t word_2;//25975
	int16_t word_4; //25977
	uint8_t stub[1];*/
}
type_str_0x36442;

typedef struct {//lenght 0x6604u
		//podpolozky
	//type_str_0x6E8E* dword_2FECE;//D41A0_BYTESTR_0.str_2FECE.dword_2FECE
	uint16_t word_2FECE;
	uint16_t word_2FED0;
	uint8_t byte_0x2FED2;//x_D41A0_BYTEARRAY_0[196306] // type of level graphics
	uint8_t byte_0x2FED3;
	uint8_t MapType;//x_D41A0_BYTEARRAY_0[196308]//GraphicsType
	int16_t word_0x2FED5;
	int16_t word_0x2FED7;
	int8_t array_0x2FED9[8];
	uint8_t stubb[4];
	uint16_t word_0x2FEE5;//2FEE5//23
	uint8_t stubb2[2];
	uint16_t word_0x2FEE9;//2FEE9//27 //first seed position
	uint8_t stubc[2];
	uint16_t word_0x2FEED;//2FEED//31 //first seed height
	uint8_t stubc2[2];
	uint16_t word_0x2FEF1;//2FEF1//35 //random seed
	uint8_t stubc3[2];
	uint32_t word_0x2FEF5;//2FEF5//39
	//uint8_t stubd[2];
	uint16_t word_0x2FEF9;//2FEF9//43
	uint8_t stube[2];
	uint16_t word_0x2FEFD;//2FEFD//47
	uint8_t stubf[2];
	uint16_t word_0x2FF01;//2FF01//51
	uint8_t stubg[2];
	uint16_t word_0x2FF05;//2FF05//55
	uint8_t stubh[2];
	uint16_t word_0x2FF09;//2FF09//59
	uint8_t stubi[2];
	uint16_t word_0x2FF0D;//2FF0D//63
	uint8_t stubj[2];
	uint16_t word_0x2FF11;//2FF11//67
	//0x30331 - end of structure lenght 20
	/*197413 - 0x30324*/
	/*221393 - 0x360D1*/
	uint8_t stubk[1022];
	type_str_0x30311 array_0x30311[0x4b0];//end(next entity) - 0x360d1
	uint8_t next_0x360D1;

	type_str_0x360D2 next_0x360D2[8];//lenght 110  /spells?
	//mozna by to melo zacit az 0x30311

	//type_str_0x3030E array_0x3030E[0xb];//fix array lenght;
	//0x3030e
	//0x30322
	//0x30336
	//type_str_0x30325 array_0x30325[0x4af];

	////0x30325 0x360d1 tj rozdil 23980=1199*20

	//36479
	//type_str_0x360FB str_0x360FB[8];//asi zacina o 40 driv
	type_str_0x36442 str_0x36442[8];//stages(checkpoints)
	//0x36443 - 70 + 110 * 7
	type_str_0x3647Ac array_0x3647A[0xb];//8x11
	//konec 6604
} type_str_2FECE;//compress level

typedef union {
	int32_t dword;
	uint16_t* ptr16u;
	type_str_0x30311* ptr0x30311;
	type_str_0x6E8E* ptr0x6E8E;
}
un_str_36552;
typedef struct {//size 10 count 8
	int8_t str_3654C_byte0;//222540
	int8_t str_3654D_byte1;
	axis_2d str_3654E_axis;
	//int16_t str_3654E_word2;//objective parametres
	//int8_t str_3654F_byte3;
	//int16_t str_36550_word4;
	//int8_t str_36551_byte5;
	un_str_36552 str_36552_un; //pointer sometimes
	//int8_t str_36553_byte7;
	//int8_t str_36554_byte8;
	//int8_t str_36555_byte9;
} type_str_3654C;

typedef struct {//size 11 count 6
	uint8_t array_0x3659C_byte[11];//objectives 1-active 2-done
	/*int8_t str_3659C_byte0;array_0x3659C_byte[0]
	int8_t str_3659D_byte1;array_0x3659C_byte[1]
	int8_t str_3659E_byte2;array_0x3659C_byte[2]
	int8_t str_3659F_byte3;array_0x3659C_byte[3]
	int8_t str_365A0_byte4;
	int8_t str_365A1_byte5;
	int8_t str_365A2_byte6;
	int8_t str_365A3_byte7;
	int8_t str_365A4_byte8;
	int8_t str_365A5_byte9;
	int8_t str_365A6_byte10;*/
} type_str_3659C;
/*
typedef struct {
	int32_t dw;
	int16_t w;
} dw_ws;
*/
/*typedef struct {
	int8_t Bit_0;
	int8_t Bit_1;
	int8_t Bit_2;
	int8_t Bit_3;
	int8_t Bit_4;
	int8_t Bit_5;
	int8_t Bit_6;
	int8_t Bit_7;
	int8_t Bit_8;
	int8_t Bit_9;
}
typedef_str_0x6E3E;*/

typedef struct {
	uint8_t m_wReflections;
	uint8_t m_wShadows;
	uint8_t m_wSky;
	uint8_t m_wViewPortSize;
}
GraphicsStruct_t;

/*typedef union {
	type_str_0x8586 str;
	uint32_t dword;
}
type_uni_0x8586;*/

typedef struct {
	uint8_t m_wMiniMap;//8590 icons//str_0x218E.str.icons_0x218E
	uint8_t m_wTopBar;//8591 ??//str_0x218E.str.xxxx_0x218F
	uint8_t m_uiScreenSize;//8592 screen size?//str_0x218E.str.scr_size_0x2190
	uint8_t xxxx_0x2191;//8593//str_0x218E.str.xxxx_0x2191
}
DisplayStruct_t;

/*typedef union {
	type_str_0x218E str;
	uint32_t dword;
}
type_uni_0x218E;*/

typedef struct {
	uint8_t xxxx_0x2192;//8594 ??//str_0x2192.str.xxxx_0x2192
	uint8_t xxxx_0x2193;//8595 ??//str_0x2192.str.xxxx_0x2193
	uint8_t m_wResolution;//8596 resolution//str_0x2192.str.resolution_0x2194
	uint8_t shadows_0x2195;//8593 shadows//str_0x2192.str.shadows_0x2195
}
type_str_0x2192;

/*typedef union {
	type_str_0x2192 str;
	uint32_t dword;
}
type_uni_0x2192;*/

typedef struct {
	uint8_t m_wDynamicLighting;//8598 lights//str_0x2196.str.lights_0x2196
	uint8_t setting_0x2197;//8599 setting//str_0x2196.str.setting_0x2197
	uint8_t transparency_0x2198;//8600 transparency//str_0x2196.str.transparency_0x2198
	uint8_t flat_0x2199;//8601 flat//str_0x2196.str.flat_0x2199
}
type_str_0x2196;

/*typedef union {
	type_str_0x2196 str;
	uint32_t dword;
}
type_uni_0x2196;*/

typedef struct {
	uint8_t creflections_0x21AA;//uint8_t byte_0x21AA;//8618 can reflections//str_0x21AA.str.creflections_0x21AA
	uint8_t cshadows_0x21AB;//uint8_t byte_0x21AB;//8619 can shadows//str_0x21AA.str.cshadows_0x21AB
	uint8_t csky_0x21AC;//uint8_t byte_0x21AC;//8620 can sky//str_0x21AA.str.csky_0x21AC
	uint8_t calter_0x21AD;//uint8_t byte_0x21AD;//8621//str_0x21AA.str.calter_0x21AD
}
type_str_0x21AA;

/*typedef union {
	type_str_0x21AA str;
	uint32_t dword;
}
type_uni_0x21AA;*/

typedef struct {
	uint8_t xxxx_0x21AE;//uint8_t byte_0x21AE;//str_0x21AE.str.xxxx_0x21AE
	uint8_t xxxx_0x21AF;//uint8_t byte_0x21AF;//str_0x21AE.str.xxxx_0x21AF
	uint8_t xxxx_0x21B0;//uint8_t byte_0x21B0;//str_0x21AE.str.xxxx_0x21B0
	uint8_t xxxx_0x21B1;//uint8_t byte_0x21B1;//str_0x21AE.str.xxxx_0x21B1
}
type_str_0x21AE;

/*typedef union {
	type_str_0x21AE str;
	uint32_t dword;
}
type_uni_0x21AE;*/

typedef struct {
	uint8_t xxxx_0x21B2;//uint8_t byte_0x21B2;//str_0x21B2.str.xxxx_0x21B2
	uint8_t xxxx_0x21B3;//uint8_t byte_0x21B3;//str_0x21B2.str.xxxx_0x21B3
	uint8_t cresolution_0x21B4;//uint8_t byte_0x21B4;//str_0x21B2.str.cresolution_0x21B4
	uint8_t xxxx_0x21B5;//uint8_t byte_0x21B5;//str_0x21B2.str.xxxx_0x21B5
}
type_str_0x21B2;

/*typedef union {
	type_str_0x21B2 str;
	uint32_t dword;
}
type_uni_0x21B2;*/

typedef struct {
	uint8_t clights_0x21B6;//uint8_t byte_0x21B6;//str_0x21B6.str.clights_0x21B6
	uint8_t csetting_0x21B7;//uint8_t byte_0x21B7;//str_0x21B6.str.csetting_0x21B7
	uint8_t ctransparency_0x21B8;//uint8_t byte_0x21B8;//str_0x21B6.str.ctransparency_0x21B8
	uint8_t cflat_0x21B9;//uint8_t byte_0x21B9;//str_0x21B6.str.cflat_0x21B9
}
type_str_0x21B6;

/*typedef union {
	type_str_0x21B6 str;
	uint32_t dword;
}
type_uni_0x21B6;*/
typedef struct {//lenght 39
	int8_t byte_0;
	int8_t byte_1;
	uint8_t byte_2;
	int8_t byte_3;
	axis_3d axis3d_4;
	/*int16_t word_6;
	int16_t word_8;*/
	type_str_0x6E8E* dword_A;
	uint8_t array_E[25];
	//int8_t stub[38];
}
type_str_0x3664C;

typedef struct {//lenght 108
	uint32_t dword_0x364D2;//lenght 108
	uint32_t dword_0x364D6[0x1a];// lenght 0x68=104 end 0x3653e
}
type_str_0x364D2;

typedef struct {//lenght 7
	int8_t byte_0;//25972
	int8_t byte_1;//25973
	uint8_t stub[5];
}
type_str_0x6574;

typedef struct {
	GraphicsStruct_t m_Graphics;
	DisplayStruct_t m_Display;
	type_str_0x2192 str_0x2192;
	type_str_0x2196 str_0x2196;
}
GameSettingsStruct_t;

typedef struct {//lenght 224791
	uint8_t stub0[4];
	uint32_t dword_0x4;
	uint32_t dword_0x8;

	int16_t word_0xc;//player_index?
	int16_t word_0xe;
	//word 0xe - 14
	uint8_t array_0x10[0x1d]; //0x10, next 0x2d(45)
	int32_t dword_0x2d;//45
	//uint8_t byte_0x22;
	uint16_t word_0x31;//49
	uint16_t word_0x33;//51
	int32_t dword_0x35;//53//entity counter(max 1000 entity changes per step)
	uint8_t array_0x39[508];//57
	//uint8_t stub2[0x1fc];
	int32_t dword_0x235;//act music
	uint8_t byte_0x239;//569
	int32_t dword_0x23a;
	int32_t dword_0x23e;
	int32_t dword_0x242;
	type_str_0x6E8E* pointers_0x246[0x3e8];//pointers
	//0x81d 2077
	/*
	*(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 570) = 1;//0x23a
	*(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 574) = 0;//0x23e
	*(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 578) = 0;//0x242
	*/
	//uint8_t stub3[0x1f44];
	int32_t dword_0x11e6;//second entity counter
	type_str_0x6E8E* dword_0x11EA[0x3e8];//??4586
	//uint8_t stub3[0xfa0];

	GameSettingsStruct_t m_GameSettings;

	uint32_t dword_0x219A;//8602
	uint32_t dword_0x219E;//8606
	uint32_t dword_0x21A2;//8610
	uint32_t dword_0x21A6;//8614

	type_str_0x21AA str_0x21AA;
	//uint8_t byte_0x21AA;//8618 can reflections
	//uint8_t byte_0x21AB;//8619 can shadows
	//uint8_t byte_0x21AC;//8620 can sky
	//uint8_t byte_0x21AD;//8621
	type_str_0x21AE str_0x21AE;
	//uint8_t byte_0x21AE;//8622
	//uint8_t byte_0x21AF;//8623
	//uint8_t byte_0x21B0;//8624
	//uint8_t byte_0x21B1;//8625
	type_str_0x21B2 str_0x21B2;
	//uint8_t byte_0x21B2;//8626
	//uint8_t byte_0x21B3;//8627
	//uint8_t byte_0x21B4;//resolution//8628
	//uint8_t byte_0x21B5;//8629
	type_str_0x21B6 str_0x21B6;
	//uint8_t byte_0x21B6;//8630 can lights
	//uint8_t byte_0x21B7;//8631
	//uint8_t byte_0x21B8;//8632 can transparency
	//uint8_t byte_0x21B9;//8633 can flat

	//uint8_t byte_0x21BA;//

	uint8_t stub3b[0x1a8];

	axis_3d array_0x2362[8];//0x30
	//int32_t dword_0x2362;
	//int16_t word_0x2366;
	uint8_t stub3c[0x14d];
	///*??*/type_str_0x24DF array_0x24DF[0xb];//?lenght
	uint8_t stub3d[0x6ff];
	//2BEC

	//x_D41A0_BYTEARRAY_0[2124 * D41A0_BYTESTR_0.word_12 + 11234])
	//x_D41A0_BYTEARRAY_0[0x84C * D41A0_BYTESTR_0.word_12 + 0x2BE2])
	type_str_0x2BDE array_0x2BDE[0x8]; //0x84c*0x8 ??
	//0x4260
	//type_str_0x6574 str_0x6574[8];//25972
	//type_str_0x65ac array_0x65ac[0xb];//??
	/*
	3-bit-28225
	4-bit-28226
	5-bit-28227
	6-2bit-28228
	8-2bit-28230
	*/

	type_str_0x6E3E array_0x6E3E[8];//28222	lenght 0xa size 0x8// game events
	type_str_0x6E8E struct_0x6E8E[0x3e8];//28302 a8*3e8
	//1 - 6F36 - 28470
	//2 - 6FDE
	//3 - 7086
	//4 - 712e

	//uint8_t stub3ee[0xa4];
	//uint8_t* pointer_0x6F32;

	//type_str_0x6E8E struct_0x6F36[8];//lenght 168

	//uint8_t stub3e[0x28f98];

	type_str_2FECE str_2FECE;// a1 = &x_D41A0_BYTEARRAY_0[0x2FECE/*196302*/];//fix - size 0x6604u//compress level
	/*//podpolozky
	uint8_t byte_0x2FED4;//x_D41A0_BYTEARRAY_0[196308]
	type_str_0x3030E array_0x3030E[0xb];//fix array lenght;
	type_str_0x30325 array_0x30325[0x4af];
	////0x30325 0x360d1 tj rozdil 23980=1199*20
	//36479
	type_str_0x3647A array_0x3647A[0xb];//8x11
	//konec 6604
	*/

	type_str_0x364D2 str_0x364D2;//lenght 108
	/*uint32_t dword_0x364D2;//lenght 108
	uint8_t array_0x364D6[0x68];// lenght 0x68=104 end 0x3653e*/

	//uint8_t stub3f[0x831];
	int16_t word_0x3653E;//
	int16_t word_0x36540;//
	int16_t word_0x36542;//
	int16_t word_0x36544;//
	int16_t word_0x36546;//
	int16_t word_0x36548;//
	int16_t word_0x3654A;//
	type_str_3654C struct_0x3654C[0x8];//size 10 count 8
	type_str_3659C struct_0x3659C[0x8];//size 11 count 8 C-0 D-1 E-2 F-3

	//int16_t word_0x365E0;
	//uint8_t stub3g[0x12];

	type_str_0x3647Ac array_0x365F4[0xb];//8x11	//set 0x58 // end 0x3664c

	//int8_t byte_0x365FC;//array_0x365F4[1].byte_0
	//int8_t byte_0x365FD;//array_0x365F4[1].byte_1

	type_str_0x3664C str_0x3664C[0x32];// lenght 0x79e//39x50 (end-0x36dea)
	//uint8_t stub3h[0x79e];
	int8_t byte_0x36DEA_fly_asistant;
	int8_t byte_0x36DEB_xx;
	int16_t word_0x36DEC_mousex;
	int16_t word_0x36DEE_mousey;
	int16_t word_0x36DF0_mousexx;
	uint32_t dword_0x36DF2;//index
	type_str_160* dword_0x36DF6;//pointer
	int16_t word_0x36DFA;
	int16_t word_0x36DFC;
	int16_t word_0x36DFE;
	uint8_t byte_0x36E00;
	uint8_t byte_0x36E01;//count objectives
	int8_t byte_0x36E02;//temp objective
	int8_t byte_0x36E03;
	int8_t byte_0x36E04;
	uint8_t stub3k[0x6];
	int8_t byte_0x36E0B;

	//&x_D41A0_BYTEARRAY_0[197413]; ix < &x_D41A0_BYTEARRAY_0[221393]; ix += 20)

	//type_str_0x365F2 array_str_0x365F2[0x10];//fix it
	//*(0x365F2 + x_D41A0_BYTEARRAY_0 + 8 * (signed __int16)result + 2)

	uint8_t stubend[0xb];
} type_D41A0_BYTESTR_0; // level data (gets saved to and loaded from SAVE/SLEVxx.dat) was originally forced to 224791 on load/save which is out of bounds

struct {//lenght 0x33
	int16_t word_0x0;//0 //0
	int16_t word_0x2;//2 //1
	int16_t word_0x4;//4 //2
	int16_t word_0x6;//6 //3
	int16_t word_0x8;//8 //4
	int16_t word_0xa;//a //5
	int16_t word_0xc;//c-3f //6
	int16_t word_0xe;//e //7
	int16_t word_0x10;//10 //8
	int16_t word_0x12;//12 //9
	int16_t word_0x14;//14 //10
	int16_t word_0x16;//16 //11
	int16_t word_0x18;//18 //12
	int16_t word_0x1a;//1a //13
	int16_t word_0x1c;//1c-4f //14
	int16_t word_0x1e;//1e-51 //15
	int16_t word_0x20;//20 //16
	int16_t word_0x22;//22 //17
	int16_t word_0x24;//24-57 //width 18
	int16_t word_0x26;//26-59 //19
	int16_t word_0x28;//28-5b //20
	int16_t word_0x2a;//2a-5d//21
	int8_t stub[4];
	int8_t byte_0x30;//30-63 - color1 //24
	int8_t byte_0x31;//31-64 - color2
	int8_t byte_0x32;//32-65 - color2
}
typedef type_sub_str_unk_1804B0ar;

typedef union {//lenght 12
	int32_t dword[3];
	int16_t word[6];
}
type_uni_0x8a;

struct {//0xb0 nebo spis 171(0xab)
	type_sub_str_unk_1804B0ar type_sub_0[2];
	/*int16_t word_0xe;//e
	int16_t word_0x10;//10
	int16_t word_0x12;//12
	int16_t word_0x14;//14//47=14+33
	int16_t word_0x16;//16//49=16+33
	int16_t word_0x18;//18//4b=18+33
	int16_t word_0x1a;//1a//4d;=1a+33
	//0x33
	//0x66
	*/

	int32_t dword_0x66;//66
	int32_t dword_0x6a;//6a
	int32_t dword_0x6e;//6e
	int32_t dword_0x72;//72
	int32_t dword_0x76;//76
	int32_t dword_0x7a;//7a
	int8_t stub[4];
	int32_t dword_0x82;//82
	int16_t word_0x86;//86
	int16_t word_0x88;//88
	type_uni_0x8a uni_0x8a;
	//int32_t dword_0x8a;//8a
	//int16_t array_0x8e[4];//8e //text lines
	int16_t word_0x96;//96
	int16_t word_0x98;//98
	int16_t word_0x9a;//9A
	int16_t word_0x9c;//9C
	int8_t byte_0x9e;//9e
	int8_t byte_0x9f;//9f
	int8_t byte_0xa0;//a0
	int8_t byte_0xa1;//a1
	int8_t byte_0xa2;//a2
	int8_t byte_0xa3;//a3
	int8_t byte_0xa4;//a4
	int8_t byte_0xa5;//a5
	int8_t byte_0xa6;//a6
	int8_t byte_0xa7;//a7
	int8_t byte_0xa8;//a8
	int8_t byte_0xa9;//a9
	int8_t byte_0xaa;//aa
	//int8_t stubb[1];
}
typedef type_str_unk_1804B0ar;
#pragma pack (16)

/*
	memset((void*)(&x_D41A0_BYTEARRAY_0[0x10]), 0, 0x1d);
	memset((void*)(&x_D41A0_BYTEARRAY_0[0x2d]), 0, 4);
	memset((void*)(&x_D41A0_BYTEARRAY_0[0x31]), 0, 2);
	memset((void*)(&x_D41A0_BYTEARRAY_0[0x33]), 0, 2);
	memset((void*)(&x_D41A0_BYTEARRAY_0[0x35]), 0, 4);
	memset((void*)(&x_D41A0_BYTEARRAY_0[0x239]), 0, 1);
	memset((void*)(&x_D41A0_BYTEARRAY_0[0x364D2]), 0, 0x6c);
	memset((void*)(&x_D41A0_BYTEARRAY_0[0x2362]), 0, 0x30);
	memset((void*)(&x_D41A0_BYTEARRAY_0[0x6E8E]), 0, 0x29040);
*/

//memset((void*)(x_D41A0_BYTEARRAY_0 + 222796), 0, 1950);
//memset((void*)(x_D41A0_BYTEARRAY_0 + 0x3664C), 0, 0x79E);

/*
*(int32_t*)&x_D41A0_BYTEARRAY_0[0x341c]
-selected bottom spell

x_D41A0_BYTEARRAY_0[28302]-168 arrays
x_D41A0_BYTEARRAY_0[0x6e8e]-0xa8 arrays
*/

/*
int32_t get_x_D41A0_BYTEARRAY_0_4();
void set_x_D41A0_BYTEARRAY_0_4(int32_t value);

int32_t get_x_D41A0_BYTEARRAY_0_8();
void set_x_D41A0_BYTEARRAY_0_8(int32_t value);

int16_t get_x_D41A0_BYTEARRAY_0_0xe();
void set_x_D41A0_BYTEARRAY_0_0xe(int16_t value);

int8_t get_x_D41A0_BYTEARRAY_0_0x10(int number);
void set_x_D41A0_BYTEARRAY_0_0x10(int number,int8_t value);
void plus_x_D41A0_BYTEARRAY_0_0x10(int number);

uint8_t get_x_D41A0_BYTEARRAY_0_0x22();
void set_x_D41A0_BYTEARRAY_0_0x22(uint8_t value);

int32_t get_x_D41A0_BYTEARRAY_0_0x35();
void set_x_D41A0_BYTEARRAY_0_0x35(int32_t value);
void plus_x_D41A0_BYTEARRAY_0_0x35();
void minus_x_D41A0_BYTEARRAY_0_0x35();

int32_t get_x_D41A0_BYTEARRAY_0_235();
void set_x_D41A0_BYTEARRAY_0_235(int32_t value);

int32_t get_x_D41A0_BYTEARRAY_0_23a();
void set_x_D41A0_BYTEARRAY_0_23a(int32_t value);

int32_t get_x_D41A0_BYTEARRAY_0_23e();
void set_x_D41A0_BYTEARRAY_0_23e(int32_t value);

int32_t get_x_D41A0_BYTEARRAY_0_242();
void set_x_D41A0_BYTEARRAY_0_242(int32_t value);

type_str_0x6E8E* get_x_D41A0_BYTEARRAY_0_0x246(int number);
void set_x_D41A0_BYTEARRAY_0_0x246(int number, type_str_0x6E8E* value);

int32_t get_x_D41A0_BYTEARRAY_0_0x11e6();
void set_x_D41A0_BYTEARRAY_0_0x11e6(int32_t value);

uint8_t get_x_D41A0_BYTEARRAY_0_0x218a();
void set_x_D41A0_BYTEARRAY_0_0x218a(uint8_t value);
uint8_t get_x_D41A0_BYTEARRAY_0_0x218b();
void set_x_D41A0_BYTEARRAY_0_0x218b(uint8_t value);
uint8_t get_x_D41A0_BYTEARRAY_0_0x218c();
void set_x_D41A0_BYTEARRAY_0_0x218c(uint8_t value);
uint8_t get_x_D41A0_BYTEARRAY_0_0x218d();
void set_x_D41A0_BYTEARRAY_0_0x218d(uint8_t value);
uint8_t get_x_D41A0_BYTEARRAY_0_0x218e();
void set_x_D41A0_BYTEARRAY_0_0x218e(uint8_t value);
uint8_t get_x_D41A0_BYTEARRAY_0_0x218f();
void set_x_D41A0_BYTEARRAY_0_0x218f(uint8_t value);
uint8_t get_x_D41A0_BYTEARRAY_0_0x2190();
void set_x_D41A0_BYTEARRAY_0_0x2190(uint8_t value);
uint8_t get_x_D41A0_BYTEARRAY_0_0x2196();
void set_x_D41A0_BYTEARRAY_0_0x2196(uint8_t value);
uint8_t get_x_D41A0_BYTEARRAY_0_0x2197();
void set_x_D41A0_BYTEARRAY_0_0x2197(uint8_t value);
uint8_t get_x_D41A0_BYTEARRAY_0_0x2198();
void set_x_D41A0_BYTEARRAY_0_0x2198(uint8_t value);
uint8_t get_x_D41A0_BYTEARRAY_0_0x2199();
void set_x_D41A0_BYTEARRAY_0_0x2199(uint8_t value);

uint8_t get_x_D41A0_BYTEARRAY_0_0x21aa();
void set_x_D41A0_BYTEARRAY_0_0x21aa(uint8_t value);
uint8_t get_x_D41A0_BYTEARRAY_0_0x21ab();
void set_x_D41A0_BYTEARRAY_0_0x21ab(uint8_t value);
uint8_t get_x_D41A0_BYTEARRAY_0_0x21ac();
void set_x_D41A0_BYTEARRAY_0_0x21ac(uint8_t value);
uint8_t get_x_D41A0_BYTEARRAY_0_0x21ad();
void set_x_D41A0_BYTEARRAY_0_0x21ad(uint8_t value);
uint8_t get_x_D41A0_BYTEARRAY_0_0x21ae();
void set_x_D41A0_BYTEARRAY_0_0xx21ae(uint8_t value);
uint8_t get_x_D41A0_BYTEARRAY_0_0x21af();
void set_x_D41A0_BYTEARRAY_0_0x21af(uint8_t value);
uint8_t get_x_D41A0_BYTEARRAY_0_0x21b0();
void set_x_D41A0_BYTEARRAY_0_0x21b0(uint8_t value);
uint8_t get_x_D41A0_BYTEARRAY_0_0x21b1();
void set_x_D41A0_BYTEARRAY_0_0x21b1(uint8_t value);
uint8_t get_x_D41A0_BYTEARRAY_0_0x21b2();
void set_x_D41A0_BYTEARRAY_0_0x21b2(uint8_t value);
uint8_t get_x_D41A0_BYTEARRAY_0_0x21b3();
void set_x_D41A0_BYTEARRAY_0_0x21b3(uint8_t value);
uint8_t get_x_D41A0_BYTEARRAY_0_0x21b4();
void set_x_D41A0_BYTEARRAY_0_0x21b4(uint8_t value);
uint8_t get_x_D41A0_BYTEARRAY_0_0x21b5();
void set_x_D41A0_BYTEARRAY_0_0x21b5(uint8_t value);
uint8_t get_x_D41A0_BYTEARRAY_0_0x21b6();
void set_x_D41A0_BYTEARRAY_0_0x21b6(uint8_t value);
uint8_t get_x_D41A0_BYTEARRAY_0_0x21b7();
void set_x_D41A0_BYTEARRAY_0_0x21b7(uint8_t value);
uint8_t get_x_D41A0_BYTEARRAY_0_0x21b8();
void set_x_D41A0_BYTEARRAY_0_0x21b8(uint8_t value);
*/
//axis_3d get_x_D41A0_BYTEARRAY_0_0x2362(int number); //lenght 48 - 6x8
//void set_x_D41A0_BYTEARRAY_0_0x2362(int number, axis_3d value);
/*int16_t get_x_D41A0_BYTEARRAY_0_0x2366(int number);
void set_x_D41A0_BYTEARRAY_0_0x2366(int number, int16_t value);*/

//void clean_x_D41A0_BYTEARRAY_0_0x2362();

void clean_x_D41A0_BYTEARRAY_0_0x2BDE(int number);
/*
void copyto_x_D41A0_BYTEARRAY_0_0x2BDE_0x7CF(int number,uint8_t* value);
void copyto_x_D41A0_BYTEARRAY_0_0x2BDE_0x7E7(int number, uint8_t* value);

void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x7(int number, int16_t value);
int8_t get_x_D41A0_BYTEARRAY_0_0x2BDE_0x9(int number);
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x9(int number, int8_t value);
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0xe(int number, int16_t value);
int16_t get_x_D41A0_BYTEARRAY_0_0x2BDE_0x10(int number);
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x10(int number, int16_t value);
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x18(int number, int32_t value);

void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x3e1(int number, int8_t value);

int32_t get_x_D41A0_BYTEARRAY_0_0x2BDE_0x1d1_2BDE_11707(int index, int subindex);//465
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x1d1_2BDE_11707(int index, int subindex, int32_t value);//465

int32_t get_x_D41A0_BYTEARRAY_0_0x2BDE_0x1d5_2BDE_11707(int index, int subindex);//469
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x1d5_2BDE_11707(int index, int subindex, int32_t value);//469

int32_t get_x_D41A0_BYTEARRAY_0_0x2BDE_0x1d9_2BDE_11707(int index, int subindex);//473
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x1d9_2BDE_11707(int index, int subindex, int32_t value);//473

int16_t get_x_D41A0_BYTEARRAY_0_0x2BDE_0x1dd_2BDE_11707(int index, int subindex);//477
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x1dd_2BDE_11707(int index, int subindex, int16_t value);//477

int8_t* get_x_D41A0_BYTEARRAY_0_0x2BDE_0x39f_2BFA_12157(int index);

void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x5a6(int number, int8_t value);

uint8_t* get_x_D41A0_BYTEARRAY_0_0x2BDE_0x649(int number);
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x649(int number,int subindex, int32_t value);

void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x717(int index, int subindex, int16_t value);
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x781(int index, int subindex, int8_t value);
int8_t get_x_D41A0_BYTEARRAY_0_0x2BDE_0x7B5(int index, int subindex);
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x7B5(int index, int subindex, int8_t value);
int8_t get_x_D41A0_BYTEARRAY_0_0x2BDE_0x7CF(int index, int subindex);
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x7CF(int index, int subindex, int8_t value);
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x7e8(int index, int subindex, int8_t value);
int8_t get_x_D41A0_BYTEARRAY_0_0x2BDE_0x803(int index, int subindex);
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x803(int index, int subindex, int8_t value);
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x81c(int index, int subindex, int8_t value);

int16_t get_x_D41A0_BYTEARRAY_0_0x2BDE_0x837(int index);
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x837(int index, int16_t value);
int16_t get_x_D41A0_BYTEARRAY_0_0x2BDE_0x839(int index);
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x839(int index, int16_t value);

uint8_t get_x_D41A0_BYTEARRAY_0_0x2BDE(int number);
void set_x_D41A0_BYTEARRAY_0_0x2BDE(int number,uint8_t value);

int8_t get_x_D41A0_BYTEARRAY_0_0x6E3E(int index,int subindex);
void set_x_D41A0_BYTEARRAY_0_0x6E3E(int index, int subindex, int8_t value);

type_str_0x6E8E* get_x_D41A0_BYTEARRAY_0_0x6E8E_adr();
//28302
//((uint8_t*)v1x - (x_D41A0_BYTEARRAY_0 + 28302)) / 168
//ix-get_x_D41A0_BYTEARRAY_0_0x6E8E_adr()
uint8_t* get_x_D41A0_BYTEARRAY_0_0x6F32();
void set_x_D41A0_BYTEARRAY_0_0x6F32(uint8_t* value);

uint8_t get_x_D41A0_BYTEARRAY_0_0x36e0b();
void set_x_D41A0_BYTEARRAY_0_0x36e0b(uint8_t value);
void and_x_D41A0_BYTEARRAY_0_0x36e0b(uint8_t value);

void clean_x_D41A0_BYTEARRAY_0();

void test_x_D41A0_BYTEARRAY_0();
*/

extern type_D41A0_BYTESTR_0 D41A0_BYTESTR_0;

/*void x_D41A0_BYTEARRAY_0_to_x_D41A0_BYTESTR_0();
void x_D41A0_BYTESTR_0_to_x_D41A0_BYTEARRAY_0();*/
void write_posistruct_to_png(uint8_t* buffer, int width, int height, char* filename);

int my_sign32(int32_t var);
int my_sign16(int16_t var);

#endif //ENGINE_SUPPORT_ACTIVE
