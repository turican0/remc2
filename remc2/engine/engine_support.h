#include <stdio.h>
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

#include"../portability/port_time.h"

#include "../portability/port_sdl_vga_mouse.h"
#include "../portability/mctypes.h"
//#define USE_DOSBOX

#ifndef ENGINE_SUPPORT_ACTIVE
#define ENGINE_SUPPORT_ACTIVE
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

extern Bit8u* readbuffer;
extern char* printbuffer;//char* buffer; // [esp+0h] [ebp-2h]
extern char* printbuffer2;//char v11; // [esp+40h] [ebp+3Eh]
//extern Bit8u *x_DWORD_E9C38_smalltit; // weak
/*extern Bit32s x_DWORD_D41A4_x6x;
extern Bit32s x_DWORD_D41A4_x8x;
extern Bit32s x_DWORD_D41A4_xAx;
extern Bit32s x_DWORD_D41A4_xBx;
extern Bit32s x_DWORD_D41A4_xCx;
extern Bit32s x_DWORD_D41A4_xDx;
extern char x_DWORD_D41A4_xB6;//2A1644 b6=182
extern char x_DWORD_D41A4_x16x;//2A15A4 16=22
extern Bit8u x_DWORD_D41A4_x17x;
extern Bit8u x_DWORD_D41A4_x18x;
extern char x_DWORD_D41A4_x19x;//2A51BD 19=25
extern Bit8u x_DWORD_D41A4_x1Ex;
extern Bit8u x_DWORD_D41A4_x2Bx;
extern Bit8u x_DWORD_D41A4_x2Dx;
extern Bit32s x_DWORD_D41A4_x33x;
extern Bit32s x_DWORD_D41A4_x59x;
extern Bit32s x_DWORD_D41A4_x79x;
extern Bit32s x_DWORD_D41A4_x7Ax;
extern Bit32s x_DWORD_D41A4_x7Bx;
extern Bit32s x_DWORD_D41A4_x7Cx;
extern Bit32s x_DWORD_D41A4_x7Dx;
extern Bit32s x_DWORD_D41A4_x7Fx;
extern Bit32s x_DWORD_D41A4_xB2;
extern Bit32s x_DWORD_D41A4_xB4;
extern Bit32s x_DWORD_D41A4_xB7;
extern Bit32s x_DWORD_D41A4_xBA;
extern Bit32s x_DWORD_D41A4_xC0;
extern Bit32s x_DWORD_D41A4_xCE;
extern Bit32s x_DWORD_D41A4_xCD;
extern Bit32s x_DWORD_D41A4_xCF;
extern Bit32s x_DWORD_D41A4_xD0;
extern Bit32s x_DWORD_D41A4_xD8;
extern Bit32s x_DWORD_D41A4_xDC;
extern Bit32s x_DWORD_D41A4_xE0;
extern Bit32s x_DWORD_D41A4_xE1;
extern Bit32s x_DWORD_D41A4_xE2;
extern Bit32s x_DWORD_D41A4_xE6;
extern Bit32s x_DWORD_D41A4_xF2;
extern Bit32s x_DWORD_D41A4_xF6;
extern Bit32s x_DWORD_D41A4_x100;
extern Bit32s x_DWORD_D41A4_x235;
extern Bit32s x_DWORD_D41A4_x749;
extern Bit32s x_DWORD_D41A4_x8CF;
extern Bit32s x_DWORD_D41A4_x94C;
extern Bit32s x_DWORD_D41A4_x954;
extern Bit32s x_DWORD_D41A4_x21AA;
extern Bit32s x_DWORD_D41A4_x00;
extern Bit32s x_DWORD_D41A4_x9602;
extern Bit32s x_DWORD_D41A4_x9603;
extern Bit32s x_DWORD_D41A4_x966F;
extern Bit32s x_DWORD_D41A4_x9677;
extern Bit32s x_DWORD_D41A4_x967B;
extern Bit32s x_DWORD_D41A4_x967F;
extern Bit32s x_DWORD_D41A4_x9691;
extern Bit32s x_DWORD_D41A4_x9692;
extern Bit32s x_DWORD_D41A4_x4;
extern Bit32s x_DWORD_D41A4_x965B;
extern Bit32s x_DWORD_D41A4_x9683;
extern Bit32s x_DWORD_D41A4_x96BF;*/

//extern Bit32s x2124_x_DWORD_D41A4_xCx_x_DWORD_D41A0_11234;

//extern Bit32s x_DWORD_D41A4;
//extern Bit8u* x_DWORD_D41A0;

//extern Bit8s x_D41A0_BYTEARRAY[];
//extern Bit8u* x_D41A0_BYTEARRAY_0;
//extern Bit8u* x_D41A0_BYTEARRAY_4;

extern Bit8u* off_D41A8_sky;
extern Bit8u* x_BYTE_14B4E0;

#pragma pack (1)
typedef struct {
	Bit8u byte1;
	Bit8u byte2;
}doublebyte;

typedef struct {
	doublebyte dbyte1_2;
	doublebyte dbyte3_4;
}tetrabytebyte;

typedef struct {
	Bit16u x;
	Bit16u y;
}axis_2du;

typedef struct {
	Bit16s x;
	Bit16s y;
}axis_2d;

typedef struct {
	Bit32s x;
	Bit32s y;
}axis_2d_32;

typedef struct {
	Bit16u x;
	Bit16u y;
	Bit16u z;
}axis_3d;
/*
typedef struct {
	Bit16u x;
	Bit16u y;
	Bit16u z;
}axis_3du;
*/
typedef struct {
	Bit16s aa;
	Bit16s xshift;
	Bit16s yshift;
	Bit16s dd;
}axis_4d;

typedef struct {
	Bit8u x;
	Bit8u y;
} baxis_2d;

typedef union {
	baxis_2d _axis_2d;
	Bit16u word;
}
uaxis_2d;

typedef struct {
	Bit8s str_0x6E3E_byte0;//0x6E3E-28222//D41A0_BYTESTR_0.array_0x6E3E[xx].str_0x6E3E_byte0
	Bit8s str_0x6E3E_byte1;//0x6E3F-28223
	Bit8s str_0x6E3E_byte2;//0x6E40-28224
	Bit8s str_0x6E3E_byte3;//0x6E41-28225
	Bit8s str_0x6E3E_byte4;//0x6E42-28226
	Bit8s str_0x6E3E_byte5;//0x6E43-28227
	Bit16u str_0x6E3E_word6;//0x6E44-28228
	Bit16u str_0x6E3E_word8;//0x6E46-28230
}type_str_0x6E3E;
/*
3-bit-28225
4-bit-28226
5-bit-28227
6-2bit-28228
8-2bit-28230
*/

typedef struct {
	Bit8u byte[0x1a];
	//Bit16u word_2279;
}
type_8_1a;

typedef struct {
	Bit16s word[0x1a];
	//Bit16u word_2279;
}
type_16_1a;

typedef struct {
	Bit32s dword[0x1a];
	//Bit16u word_2279;
}
type_32_1a;

typedef union {
	Bit8u byte[4];
	Bit16u word[2];
	Bit32u dword;
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
	//Bit8s array_0x41D_1053x[0x1a];//size??
	//Bit8s array_0x41E_1054;//array_0x41D_1053x[1]
	//Bit8s array_0x41F_1055;//array_0x41D_1053x[2]
	//Bit8s array_0x420_1056;//array_0x41D_1053x[3]
	//Bit8s array_0x422_1058;//array_0x41D_1053x[5]
	//Bit8s array_0x423_1059;//array_0x41D_1053x[6]
	//Bit8s array_0x424_1060;//array_0x41D_1053x[7]
	//Bit8s array_0x425_1061;//array_0x41D_1053x[8]
	//Bit8s array_0x428_1064;//array_0x41D_1053x[11]
	//Bit8s array_0x433_1075;//array_0x41D_1053x[22]
	//1079 501+611//spell index
	type_8_1a array_0x3CF_975x;//364//byte_0x7B5_2BDE
	type_8_1a array_0x3E9_1001x;//390//size??//array_0x7CF_2BDE_13229
	type_8_1a array_0x403_1027x;//416//size??//byte_0x7E9_2BDE
	type_8_1a array_0x41D_1053z;//442//size??//byte_0x803_2BDE
	type_8_1a array_0x437_1079x;//468//size?? //table of spell indexes//byte_0x81D_2BDE
	Bit16s word_0x451_1105;//494 act left spell index//word_0x837_2BDE
	Bit16s word_0x453_1107;//496 act right spell index//word_0x839_2BDE
	Bit8s byte_0x455_1109;//498
	Bit8s byte_0x456_1110;//499
	Bit8s byte_0x457_1111;//500//byte_0x83D_2BDE //select spell/type spell
	Bit8s byte_0x458_1112;//501//byte_0x83E_2BDE
	Bit8s byte_0x459_1113;//502
	Bit8s byte_0x45A_1114;
	Bit8s byte_0x45B_1115;
}
type_str_611;

/*typedef struct {
	Bit16s x;
	Bit16s y;
}
type_str_164_pos;*/

typedef struct {//lenght 18
	Bit16s word_0;
	Bit16s word_2BDE_12658[8];
}type_str_0x1AC_428;

typedef struct {//size 1136
	Bit32s dword_0x0_0;
	Bit16s word_0x4_4;
	Bit16s word_0x6_6;
	Bit8u stuba[4];
	Bit16s word_0xc_12;//final speed
	Bit16s word_0xe_14;
	Bit16s word_0x10_16;
	//Bit8s byte_0x11_17;
	Bit8u stubb[2];
	axis_2d position_backup_20;//position //mouse backup
	Bit16s word_0x18_24_next_entity;
	Bit16s word_0x1A_26;
	Bit8u stubc[2];
	Bit16s word_0x1E_30;
	Bit16s word_0x20_32;
	Bit16s word_0x22_34;
	Bit16s word_0x24_36;
	Bit16s word_0x26_38[8];
	//Bit16s word_0x2E_46;//word_0x26_38[4]
	Bit16s word_0x36_54;
	Bit16s word_0x38_56;
	Bit16s word_0x3A_58;
	Bit16u array_0x3C_60[16];//size??
	Bit16u array_0x5C_92[16];
	Bit8u stubd[174];
	/*Bit8s byte_0xD3_211;
	Bit8s byte_0xD4_212;
	Bit8s byte_0xD5_213;*/
	Bit32s dword_0x12A_298;
	Bit32s dword_0x12E_302;
	Bit8u stube[10];
	Bit32s dword_0x13C_316;
	Bit8u stubf[2];
	Bit32s dword_0x142_322;
	Bit16s word_0x146_326;
	Bit16s word_0x148_328;
	Bit16s word_0x14A_330;
	Bit8u byte_0x14C_332;
	Bit8s byte_0x14D_333;
	Bit8s byte_0x14E_334;
	Bit8s byte_0x14F_335;
	Bit32s byte_0x150_336;
	Bit8u byte_0x154_340;
	Bit16s word_0x155_341;
	Bit16s word_0x157_343;
	Bit16s word_0x159_345;
	Bit8s array_0x15B_347[8];
	Bit16s word_0x163_355;
	Bit32s dword_0x165_357;
	Bit32s dword_0x169_361;
	Bit32s dword_0x16D_365;
	Bit8u stubg[4];
	Bit32s dword_0x175_373;
	Bit32s dword_0x179_377;
	Bit32s dword_0x17D_381;
	Bit32s dword_0x181_385;
	Bit32s dword_0x185_389;
	Bit32s dword_0x189_393;//clock
	Bit32s dword_0x18D_397;
	Bit8u stubi[4];
	Bit8s byte_0x195_405;
	Bit8s byte_0x196_406;
	Bit8s byte_0x197_407;
	Bit8u stubj[2];
	Bit32s dword_0x19A_410;
	Bit32s dword_0x19E_414;
	Bit16s word_0x1A2_418;
	Bit16s word_0x1A4_420;
	Bit16s word_0x1A6_422;//211 x add
	Bit16s word_0x1A8_424;//212 y add
	Bit16s word_0x1AA_426;//213 z add
	type_str_0x1AC_428 str_0x1AC_428;
	Bit8s byte_0x1BE_446;
	Bit8s byte_0x1BF_447;
	Bit8s byte_0x1C0_448;
	Bit8s byte_0x1C1_449;//switch - event? type?
	Bit16s word_0x1C2_450;
	Bit8u stubl[56];
	//Bit8s array_0x1D4_468[0x1a];
	Bit16u array_0x1FC_508[32];//1ca
	//Bit16s word_0x204_516[32];
	//Bit16s word_0x242_578;//word_0x204_516[31]
	Bit8u stubm[6];
	Bit16s word_0x242_578 = -1;
	Bit16s word_0x244_580;
	Bit16s word_0x246_582;
	Bit16s word_0x248_584;
	Bit16s word_0x24A_586;
	Bit16s word_0x24C_588;
	Bit8s array_0x24E_590[10];//size??
	Bit8u stubn[9];
	Bit8s byte_0x261_609;
	Bit8s byte_0x262_610;
	type_str_611 str_611;

	Bit8s str_611_byte_0x45C_1116;
	Bit8s str_611_byte_0x45D_1117;
	Bit8s str_611_byte_0x45E_1118;
	Bit8s str_611_byte_0x45E_1119;

	//Bit8u stubo[16];
	//611;
}
type_str_164;

typedef struct {//lenght 34
	Bit16s word_160_0x0_0;
	Bit16s word_160_0x2_2;
	Bit16s word_160_0x4_4;
	Bit16s word_160_0x6_6;
	Bit16s word_160_0x8_8;
	Bit16s word_160_0xa_10;
	Bit16s word_160_0xc_12;
	Bit16s word_160_0xe_14;
	Bit16s word_160_0x10_16;//->dword_0xA0_160x->word_160_0x10_16
	Bit16s word_160_0x12_18;
	Bit32s dword_160_0x14_20;
	Bit8u stuba[2];
	Bit16s word_160_0x1a_26;
	Bit16s word_160_0x1c_28;
	Bit16s word_160_0x1e_30;
	Bit8s byte_160_0x20_32;
	Bit8u stubb[1];
}
type_str_160;

//viz. unk_D7BD6

typedef struct
{
	Bit32s dword_0x5E_94;//94 //36 lenght
	Bit16s word_0x62_98;//98 //4 // index subentity
	Bit32s dword_0x64_100;//100 //6
	Bit16s word_0x68_104;//104 //10
	Bit8u stub5b[6];
	Bit32s dword_0x70_112;//112
	Bit16s word_0x74_116;//116
	Bit16s word_0x76_118;//118
	Bit16s word_0x78_120;//120
	Bit16s word_0x7A_122;//122
	Bit16s word_0x7C_124;//124
	Bit8u stub5d[2];
	Bit16u word_0x80_128;//128
} type_str_0x5E_94;

typedef struct _str_0x6E8E {//lenght a8//THING
	struct _str_0x6E8E* next_0;
	Bit32s dword_0x4;//4 //?bitmap//LIVE
	Bit32s dword_0x8;//8 // this is Bit32s ?bitmap//MAX LIVE
	dw_w_b struct_byte_0xc_12_15;//12
						//12//struct_byte_0xc_12_15.dbyte1_2.byte1
	//0xfe - all spell
	//Bit8s byte_0xd_13;//13//struct_byte_0xc_12_15.dbyte1_2.byte2
	//Bit8s byte_0xe_14;//14//struct_byte_0xc_12_15.dbyte3_4.byte1
	//Bit8s byte_0xf_15;//15//struct_byte_0xc_12_15.dbyte3_4.byte2
	Bit32s dword_0x10_16;//16 // index of array
	Bit16u word_0x14_20;//20 //random seed adress
	Bit16u word_0x16_22;//22
	Bit16u word_0x18_24_next_entity;//24 //next entity index
	Bit16u word_0x1A_26;//26 // index - owner //ID last index
	Bit16s word_0x1C_28;//28//rotate1
	Bit16s word_0x1E_30;//30//rotate2
	Bit16s word_0x20_32;//32//rotate3
	//Bit8s byte_0x21_33;//33
	Bit16s word_0x22_34;//34//rotate4
	Bit16s word_0x24_36;//36 // index subentity
	Bit16s word_0x26_38;//38 // index subentity
	Bit16u word_0x28_40;//40//WHO OWNS ME
	Bit16u word_0x2A_42;//42 //maybe Bit16s
	Bit16s word_0x2C_44;//44 //add to z
	Bit16s word_0x2E_46;//46
	Bit16u word_0x30_48;//48
	Bit16u word_0x32_50;//50
	Bit16u word_0x34_52;//52 // index subentity
	Bit16u word_0x36_54;//54
	Bit8s byte_0x38_56;//56
	Bit8s byte_0x39_57;//57
	Bit8s byte_0x3A_58;//58
	Bit8s byte_0x3B_59;//59
	Bit8s byte_0x3C_60;//60
	Bit8s byte_0x3D_61;//61
	Bit8u byte_0x3E_62;//62 //index
	Bit8s byte_0x3F_63;//63 //type//MODEL
	//11 - spell
	Bit8s byte_0x40_64;//64 //subtype//CLASS
	//0 or 1 - player, 2 - castle,3-ballon
	Bit8s byte_0x41_65;//65 //type subentity
	Bit8s byte_0x42_66;//66 //subtype subentity
	Bit8s byte_0x43_67;//67
	Bit8s byte_0x44_68;//68
	Bit8u byte_0x45_69;//69 // index of structure with lenght 14//STATE-
	Bit8s byte_0x46_70;//70 // index of bitmap // index of ending sequence
	Bit8s byte_0x47_71_xx;//71
	Bit8s byte_0x48_72;//72
	Bit8s byte_0x49_73;//70
	Bit16s word_0x4A_74;
	axis_3d array_0x4C_76;//position//ACTUAL X Y Z
	axis_4d array_0x52_82;
	Bit16s word_0x5A_90;
	Bit8s byte_0x5C_92;
	Bit8s byte_0x5D_93;
	type_str_0x5E_94 str_0x5E_94;

	Bit16s word_0x82_130;//130 //add to z//ACTUAL SPEED
	Bit16s word_0x84_132;//132//MIN SPEED
	Bit16s word_0x86_134;//134//MAX SPEED
	Bit32s dword_0x88_136;//136
	Bit32s dword_0x8C_140;//140 //wiz name?//ACTUAL MANA
	Bit32s dword_0x90_144;//144 0-1000//MAX MANA
	Bit16u word_0x94_148;//148
	Bit16s word_0x96_150;//150 // index subentity
	Bit16u word_0x98_152;//152
	axis_3d word_0x9A_154x;//154
	//Bit16s word_0x9E_158;//152
	type_str_160* dword_0xA0_160x;//160 //special settings
	//Bit16u word_0xA2_162;//162
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
	Bit32s dword[0x1a];
}
type_dword_0x649_2BDE;

/*
typedef struct {//begin 0x649 //lenght 0x1F9
	type_dword_0x649_2BDE dword_0x649_2BDE;//1609//0x649+0,lenght 0x68(4x0x1a)
	Bit8u stub1[104];
	Bit16s word_0x719_2BDE[0x1a];// -2xa1 tj 2x 26//1817
	//1817
	Bit8u stub4c[52];
	//Bit16s word_0x731_2BDE;//13071
	Bit8s byte_0x781_2BDE[0x1a];//1921
	type_2255ar array_0x79B_2BDE_13177;//0x649+338,lenght 0x18+2
	Bit8s byte_0x7B5_2BDE[0x1a];//1973
	//Bit8u array_0x7CF_2BDE_13229[0x34];//1999
	//Bit8u array_0x7CF_2BDE_13229[0x18];//1999
	type_2255ar array_0x7CF_2BDE_13229;//0x649+390,lenght 0x18+2
	//Bit8u stub4e[0x1a];
	//24
	//2

	//Bit8s array_0x7E7_2BDE_13253[0x1c];//array_0x7CF_2BDE_13229[0x18]
	type_2255ar byte_0x7E9_2BDE;// -1x26//2026

	type_2255ar byte_0x803_2BDE;// -1x26//2051//0x649+442, lenght 0x18+2

	type_2255ar byte_0x81D_2BDE;// -1x26//2077//0x649+468, lenght 0x18+2
	//Bit8u stub2[1];

	Bit16s word_0x837_2BDE;//2103
	Bit16s word_0x839_2BDE;//2105
	Bit8u stub4[2];
	Bit8s byte_0x83D_2BDE;//2109

	Bit8s byte_0x83E_2BDE;//2110 - 501//spell index

	//Bit8s byte_0x83E_2BDE;//2110 //spell index
	Bit8u stub5[3];
} type_struct_0x649_2BDE_12839;*/

/*typedef struct {//lenght 2124?
	type_str_0x6E3E str_0x256_0;
	Bit8u stub[14];
	Bit32u str0x256_24;//??lenght
	Bit8u stubc[1581];
	//Bit8u str0x256_24[1585];//??lenght
	type_str_611 str0x256_1609;//??lenght

	Bit8s byte_0x45C_1116;
	Bit8s byte_0x45D_1117;
	Bit8s byte_0x45E_1118;
	Bit8s byte_0x45E_1119;

	//0x100//x_D41A0_BYTEARRAY_4_struct.byteindex_256ar
	//280=byteindex_256ar[24];//x_D41A0_BYTEARRAY_4_struct.byteindex_256ar[24]
	//565=byteindex_256ar[309];//0x235//x_D41A0_BYTEARRAY_4_struct.byteindex_256ar[309]
	//1865=byteindex_256ar[1609];//0x749//x_D41A0_BYTEARRAY_4_struct.byteindex_256ar[1609]
	//end 2224
	Bit8u stubb[6];
}
type_str_0x256;*/

typedef struct { // 479 // size 14 tj 0xe
	axis_3d axis_2BDE_11695;//11709 0 2 4
	//Bit32s dword_0x1d1_2BDE_11695; //465 //0
	//Bit32s dword_0x1d5_2BDE_11699; //469 //4
	//Bit32s dword_0x1d9_2BDE_11703; //473 //8
	axis_4d rotation__2BDE_11701;//_2BDE_11715 6 8 10 12 //aa
	//Bit16s word_0x1dd_2BDE_11707; //477 //12 //dd
	/*
	Bit32s dword_0x1df_2BDE_11709; //479
	Bit32s dword_0x1e3_2BDE_11713; //483
	Bit32s dword_0x1e7_2BDE_11717; //487
	Bit16s word_0x1eb_2BDE_11721; //491*/
} type_struct_0x1d1_2BDE_11695;

typedef struct {//lenght 2124=0x84C
	dw_w_b dw_w_b_0_2BDE_11230;
	//Bit8u byte_0x000_2BDE_11230;//0	//11230 - byte? - ne zacatek struktury
	//Bit8u byte_0x001_2BDF_11231_xx;
	//Bit8u byte_0x002_2BE0_11232;//2	//11232 - byte //2, maybe word
	//Bit8u byte_0x003_2BE0_11233_xx;
	Bit8u byte_0x004_2BE0_11234;//2	//11234 - byte //4
	Bit8u byte_0x005_2BE0_11235;//5
	Bit8u byte_0x006_2BE4_11236;//6	//11236 - byte //6
	Bit16s word_0x007_2BE4_11237;//index of player
	Bit8s byte_0x009_2BE4_11239;
	Bit16u word_0x00a_2BE4_11240;//10 //11240 - word - index z EA3E4 //0xa
	Bit8u byte_0x00c_2BE0_11242_xx;
	Bit8u byte_0x00d_2BE0_11243_xx;
	Bit16s word_0x00e_2BDE_11244; //14
	Bit16s word_0x010_2BDE_11246; //16
	Bit32s dword_0x012_2BE0_11248;//18 ??
	//Bit8u byte_0x013_2BE0_11249_xx;
	//Bit8u byte_0x014_2BE0_11250_xx;
	//Bit8u byte_0x015_2BE0_11251_xx;

	Bit8u byte_0x016_2BE0_11252_xx;//22
	Bit8u byte_0x017_2BE0_11253_xx;//23
	Bit32s dword_0x018_2BDE_11254; //24 //pointer
	char array_0x01c_2BFA_11258[49];//28//11258 - byte* jmeno2?
	Bit16s word_0x04d_2C2B_11307;//77 //11307 - word
	Bit16u word_0x04f_2C2D_11309;//79 //11309 - word
	char names_81[8][48];//81 ?8
	//Bit8u stub2[384];

	type_struct_0x1d1_2BDE_11695 struct_0x1d1_2BDE_11695[0x21];//465
	/*Bit32s dword_0x1d1_2BDE_11695; //465
	Bit32s dword_0x1d5_2BDE_11699; //465
	Bit32s dword_0x1d9_2BDE_11703; //465
	Bit16s word_0x1dd_2BDE_11707; //465*/
	/*array 0x1d1 // 479 // size 14 tj 0xe
	{
		Bit32s dword_0x1d1_2BDE_11695; //465
		Bit32s dword_0x1d5_2BDE_11699; //465
		Bit32s dword_0x1d9_2BDE_11703; //465
		Bit16s word_0x1dd_2BDE_11707; //465

		//Bit32s dword_0x1df_2BDE_11709; //479
		//Bit32s dword_0x1e3_2BDE_11713; //483
		//Bit32s dword_0x1e7_2BDE_11717; //487
		//Bit16s word_0x1eb_2BDE_11721; //491
	}*/
	//Bit8u stub2b[308];

	char array_0x39f_2BFA_12157[0x40];//927//12157 - byte(11230+927) 100% name
	Bit8u byte_0x3DF_2BE4_12221;//991//12221 - byte
	Bit8u byte_0x3E0_2BE4_12222;//992//12222 - byte
	Bit8u byte_0x3E1_2BE4_12223;//993//12223 - byte - ok
	Bit8u byte_0x3E2_2BE4_12224;//994//12224 - byte
	Bit8u byte_0x3E3_2BE4_12225;//995
	Bit8u byte_0x3E4_2BE4_12226;//996
	Bit8u stub3[1];
	type_str_164 dword_0x3E6_2BE4_12228;//998//pointer or other
	//Bit8u stub3[61];
	//Bit16s word_0x3EA_2BDE;//1002 //dword_0x3E6_2BE4_12228.word_0x4_4
	//Bit8u stub3b[14];
	//Bit16s word_0x3FA_2BDE;//1018 mouse_x 20 dword_0x3E6_2BE4_12228.position_20.x
	//Bit16s word_0x3FC_2BDE;//1020 mouse_y 22 dword_0x3E6_2BE4_12228.position_20.y
	//Bit8u stub3c[32];
	//Bit16s word_0x420_2BE4_12284;//1054 //dword_0x3E6_2BE4_12228.word_0x38_56
	//Bit8u byte_0x420_2BE4_12286;//1056//12286 - byte //dword_0x3E6_2BE4_12228.word_0x3A_58
	//Bit8u stub4[282];
	//Bit16u word_0x53B_2BDE;// 1339 //dword_0x3E6_2BE4_12228.word_0x155_341
	//Bit8u stub4b[30];
	//Bit32u dword_0x55B_2BDE;//1371 dword//12601//dword_0x3E6_2BE4_12228.dword_0x175_373
	//Bit32u dword_0x55F_2BDE;//1375 dword//12605//dword_0x3E6_2BE4_12228.dword_0x179_377
	//Bit32u dword_0x563_2BDE;//1379 dword//12609//dword_0x3E6_2BE4_12228.dword_0x17D_381
	//Bit32u dword_0x567_2BDE;//1383 dword//12613//dword_0x3E6_2BE4_12228.dword_0x181_385
	//Bit32u dword_0x56B_2BDE;//1387 dword//12617//dword_0x3E6_2BE4_12228.dword_0x185_389
	//Bit32u dword_0x56F_2BDE;//1391 dword//12621//dword_0x3E6_2BE4_12228.dword_0x189_393
	//Bit32u dword_2BDE_12621;//1391
	//Bit8u stub4d[33];
	//Bit16s word_2BDE_12658[8];//1428//430 dword_0x3E6_2BE4_12228.str_0x1AC_428.word_2BDE_12658
	//Bit8u stub4e[2];
	//Bit8s byte_0x5a6_2BE4_12676;//1446//dword_0x3E6_2BE4_12228.byte_0x1C0_448
	//Bit8u stub4c[162];

	//type_struct_0x649_2BDE_12839 struct_0x649_2BDE_12839;//min 0x1ee//1609//611
	//dword_0x3E6_2BE4_12228.str_611
	/*Bit8u stub4b[228];
	Bit8u array_0x7CF_2BDE_13229[24];
	Bit16s word_0x7E7_2BDE_13253;*/
	//Bit8u stub4f[4];

	Bit8s byte_0x846_2BDE;//2118
	Bit8s byte_0x847_2BDE;//2119
	Bit16s word_0x848_2BDE;//2120
	Bit16s word_0x84A_2BDE;//2122
	//Bit8u stub5[17];
} type_str_0x2BDE;//11230 // 84c lenght

typedef struct {//size 26
	Bit32s dword_0;
	Bit32s dword_4;
	Bit8u* dword_8_data;
	Bit32u* dword_12x;//Bit32u*
	Bit32u* dword_16x;//Bit32u*
	Bit16s word_20;
	Bit16s word_22;
	Bit16s word_24;
	Bit8u data[];
	//Bit32u dword_26;
} type_x_DWORD_E9C28_str;

typedef struct {
	//Bit8u harddisk_number;

	//Bit8u setting_byte5_19;
	Bit32u dwordindex_0;//x_D41A0_BYTEARRAY_4_struct.dwordindex_0
	Bit16u wordindex_4;//x_D41A0_BYTEARRAY_4_struct.byteindex_4
	Bit16u wordindex_6;//x_D41A0_BYTEARRAY_4_struct.wordindex_6
	Bit16u wordindex_8;//x_D41A0_BYTEARRAY_4_struct.wordindex_8
	Bit8u byteindex_10;//0xa//x_D41A0_BYTEARRAY_4_struct.byteindex_10//show help
	Bit8s byte_brightness_11;//0xb//x_D41A0_BYTEARRAY_4_struct.byteindex_11
	Bit8s byte_brightness_12;//0xc//x_D41A0_BYTEARRAY_4_struct.byteindex_12
	Bit8s byte_brightness_13;//0xd//x_D41A0_BYTEARRAY_4_struct.byteindex_13
	Bit16u wordindex_14;
	Bit32u dwordindex_16;//0x10//x_D41A0_BYTEARRAY_4_struct.dwordindex_16
	Bit8u stubb[2];
	Bit8u setting_byte1_22;//x_D41A0_BYTEARRAY_4_struct.setting_byte1_22
	Bit8s setting_byte2_23;//0x17//x_D41A0_BYTEARRAY_4_struct.setting_byte2_23
	Bit8u setting_byte3_24;//cheats? 0x20==free spell//x_D41A0_BYTEARRAY_4_struct.setting_byte3_24
	//spell on - 0x20
	Bit8u setting_byte4_25;//cheats? 0x1==Invincability
	//invincability - 1;
	Bit8u byteindex_26 = 0;//x_D41A0_BYTEARRAY_4_struct.byteindex_26
	Bit8u stubc[3];
	Bit8u setting_30;//x_D41A0_BYTEARRAY_4_struct.setting_30
	Bit8u stubd[3];
	//FILE* moviemvidatfile_byte4_33;
	Bit8u m_wHighSpeedSystem;
	FILE* moviemvidatfile_byte4_35;
	Bit16u moviemvinumber_byte4_39;//x_D41A0_BYTEARRAY_4_struct.moviemvinumber_byte4_39
	Bit8u stube[2];
	Bit16u levelnumber_43w;//x_D41A0_BYTEARRAY_4_struct.levelnumber_43
	//Bit8u stubf[1];
	Bit16u setting_45w;
	Bit8u stubg[3];
	Bit8s byteindex_50;//0x32//x_D41A0_BYTEARRAY_4_struct.byteindex_50
	Bit8u byteindex_51;//0x33//x_D41A0_BYTEARRAY_4_struct.byteindex_51
	Bit8u byteindex_52;//0x34//x_D41A0_BYTEARRAY_4_struct.byteindex_52
	Bit8u byteindex_53;//0x35//x_D41A0_BYTEARRAY_4_struct.byteindex_53
	Bit8u stubh[3];
	char player_name_57ar[32];
	char savestring_89[32];
	Bit8u byteindex_121[16];//0x79//x_D41A0_BYTEARRAY_4_struct.byteindex_121
	//Bit8u byteindex_122;//0x7a//x_D41A0_BYTEARRAY_4_struct.byteindex_121[1]
	//Bit8u byteindex_123;//0x7b//x_D41A0_BYTEARRAY_4_struct.byteindex_121[2]
	//Bit8u byteindex_124;//0x7c//x_D41A0_BYTEARRAY_4_struct.byteindex_121[3]
	//Bit8u byteindex_125;//0x7d//x_D41A0_BYTEARRAY_4_struct.byteindex_121[4]
	//Bit8u byteindex_127;//0x7f//x_D41A0_BYTEARRAY_4_struct.byteindex_121[6]
	Bit8u stubi[8];
	char byteindex_145ar[32];//x_D41A0_BYTEARRAY_4_struct.byteindex_145ar

	Bit8u byteindex_177;//0xb1//x_D41A0_BYTEARRAY_4_struct.byteindex_177
	Bit8u speedIndex;//0xb2//x_D41A0_BYTEARRAY_4_struct.byteindex_178
	Bit8u SelectedLangIndex;
	Bit8u byteindex_180;//0xb4//x_D41A0_BYTEARRAY_4_struct.byteindex_180
	Bit8u byteindex_181;//0xb5//x_D41A0_BYTEARRAY_4_struct.byteindex_181
	Bit8u byteindex_183;//0xb7//x_D41A0_BYTEARRAY_4_struct.byteindex_183
	Bit16u byteindex_184w;//0xb8//x_D41A0_BYTEARRAY_4_struct.byteindex_184
	//Bit8u stubj[1];
	Bit8u byteindex_186;//0xba//x_D41A0_BYTEARRAY_4_struct.byteindex_186
	Bit8u stubk[1];
	Bit32u dwordindex_188;//x_D41A0_BYTEARRAY_4_struct.byteindex_188
	//Bit8u byteindex_189;//x_D41A0_BYTEARRAY_4_struct.byteindex_189
	//Bit8u byteindex_190;//x_D41A0_BYTEARRAY_4_struct.byteindex_190

	//1(0x1)-help//2(0x2)-sound//3(0x4)-music//4(0x8)-speek//5(0x10)-fly//6(0x20)-bright//7(0x40)speed//8(0x80)screen
	//9(0x100)-reflection//10(0x200)-sky//11(0x400)-shadows//12(0x800)-light/13(0x1000)-icons//14(0x2000)-transparency//15(0x4000)-flat	//16(0x8000)-resolution
	//17(0x10000)-names

	Bit32u dwordindex_192;//0xc0//x_D41A0_BYTEARRAY_4_struct.byteindex_192
	//Bit8u byteindex_193;//0xc1//x_D41A0_BYTEARRAY_4_struct.byteindex_193
	//Bit8u byteindex_194;//0xc2//x_D41A0_BYTEARRAY_4_struct.byteindex_194

	//1(0x1)-help//2(0x2)-sound//3(0x4)-music//4(0x8)-speek//5(0x10)-fly//6(0x20)-bright//7(0x40)speed//8(0x80)screen
	//9(0x100)-reflection//10(0x200)-sky//11(0x400)-shadows//12(0x800)-light/13(0x1000)-icons//14(0x2000)-transparency//15(0x4000)-flat	//16(0x8000)-resolution
	//17(0x10000)-names

	Bit8u byteindex_196;//0xc4//x_D41A0_BYTEARRAY_4_struct.byteindex_196
	Bit8u stubl[3];
	Bit8u transfer_rate_dwordindex_200;//0xc8//x_D41A0_BYTEARRAY_4_struct.transfer_rate_dwordindex_200
	Bit8u stubm[3];
	Bit8u byteindex_204;//0xCC//x_D41A0_BYTEARRAY_4_struct.byteindex_204
	Bit8u byteindex_205;//0xCD//x_D41A0_BYTEARRAY_4_struct.byteindex_205
	Bit8u byteindex_206;//0xce//x_D41A0_BYTEARRAY_4_struct.byteindex_206
	Bit8u byteindex_207;//0xcf//x_D41A0_BYTEARRAY_4_struct.byteindex_207
	Bit8u byteindex_208;//0xd0//x_D41A0_BYTEARRAY_4_struct.byteindex_208
	Bit8u stubmm[1];
	Bit32u byteindex_210d;//0xd2//x_D41A0_BYTEARRAY_4_struct.byteindex_210
	//Bit8u stubn[3];
	Bit16u byteindex_214w;//0xd6//x_D41A0_BYTEARRAY_4_struct.byteindex_214
	//Bit8u stubo[1];
	Bit16u setting_216w;
	//Bit8u stubp[1];
	Bit16u byteindex_218w;//0xda//x_D41A0_BYTEARRAY_4_struct.byteindex_218
	//Bit8u stubq[1];

	Bit8u byteindex_220;//0xdc//x_D41A0_BYTEARRAY_4_struct.byteindex_220
	Bit8u byteindex_221ar[3];//0xe0//x_D41A0_BYTEARRAY_4_struct.byteindex_221ar
	Bit8u byteindex_224;//0xe0//x_D41A0_BYTEARRAY_4_struct.byteindex_224
	Bit8u byteindex_225;//0xe1//x_D41A0_BYTEARRAY_4_struct.byteindex_225
	Bit8u* pointer_0xE2_heapbuffer_226;//x_D41A0_BYTEARRAY_4_struct.pointer_0xE2_heapbuffer_226
	Bit32u dword_0xE6_heapsize_230;//x_D41A0_BYTEARRAY_4_struct.dword_0xE6_heapsize_230
	Bit8u stubr[8];
	Bit8u byteindex_242ar[14];//0xf2//x_D41A0_BYTEARRAY_4_struct.byteindex_242ar
	//Bit8u byteindex_246;//0xf6//x_D41A0_BYTEARRAY_4_struct.byteindex_242ar[4]
	//Bit8u byteindex_250;//x_D41A0_BYTEARRAY_4_struct.byteindex_242ar[8]

	type_str_0x2BDE byteindex_256ar;//0x100//x_D41A0_BYTEARRAY_4_struct.byteindex_256ar
	//280=byteindex_256ar[24];//x_D41A0_BYTEARRAY_4_struct.byteindex_256ar[24]
	//565=byteindex_256ar[309];//0x235//x_D41A0_BYTEARRAY_4_struct.byteindex_256ar[309]
	//1865=byteindex_256ar[1609];//0x749//x_D41A0_BYTEARRAY_4_struct.byteindex_256ar[1609]
	//end 2224

	//Bit8u byteindex_2255ar[26];//0x100//x_D41A0_BYTEARRAY_4_struct.byteindex_2255ar
	//type_2255ar str_2255ar;
	//end 2281
	//Bit8u stubs[9];
	Bit32u dwordindex_2380;//0x94c//x_D41A0_BYTEARRAY_4_struct.dwordindex_2380
	Bit8u stubt[4];
	Bit32u dwordindex_2388;//0x954//x_D41A0_BYTEARRAY_4_struct.dwordindex_2388
	Bit32u dwordindex_2392;//0x958//x_D41A0_BYTEARRAY_4_struct.dwordindex_2392

	//Bit8u byteindex_8618;//0x21aa//x_D41A0_BYTEARRAY_4_struct.byteindex_8618
	//Bit8u setting_38545;
	Bit8u stubu[36000];
	type_str_0x6E8E* dwordindex_38396;//0x95FC//x_D41A0_BYTEARRAY_4_struct.dwordindex_38396
	Bit8u byteindex_38400;//0x9600//x_D41A0_BYTEARRAY_4_struct.byteindex_38400
	Bit8u byteindex_38401;//0x9601//x_D41A0_BYTEARRAY_4_struct.byteindex_38401

	Bit8u setting_38402;//0x9602//x_D41A0_BYTEARRAY_4_struct.setting_38402

	type_str_0x6E8E* bytearray_38403x[30];//array 116//0x9603//x_D41A0_BYTEARRAY_4_struct.bytearray_38403
	/*
	creatures
		8-dead
		69-dead
	*/
	//bytearray_38419=bytearray_38403[16]//0x965b//x_D41A0_BYTEARRAY_4_struct.bytearray_38403[16]
	//bytearray_38439=bytearray_38403[36]//0x965b//x_D41A0_BYTEARRAY_4_struct.bytearray_38403[36]
	//bytearray_38403[88]//Bit32u dword_38491;//0x965b//x_D41A0_BYTEARRAY_4_struct.bytearray_38403[88]

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
	Bit8u stubv[5];
	Bit8u byte_38544;//x_D41A0_BYTEARRAY_4_struct.byte_38544
	Bit8u setting_38545;//0x9691//x_D41A0_BYTEARRAY_4_struct.setting_38545
	Bit8u byte_38546;//0x9692//x_D41A0_BYTEARRAY_4_struct.byte_38546
	Bit8u stubw[44];
	Bit8u byte_38591;//0x96BF//x_D41A0_BYTEARRAY_4_struct.byte_38591

	//Bit32u dword_616432;
}type_x_D41A0_BYTEARRAY_4_struct;

doublebyte doublebyte_conv(Bit16u a2);

extern type_x_D41A0_BYTEARRAY_4_struct x_D41A0_BYTEARRAY_4_struct;
extern Bit16s x_D41A0_WORDARRAY[];

//extern Bit32u x_D41A0_BYTEARRAY_4_0xE6_heapsize;
//extern Bit8u* x_D41A0_BYTEARRAY_4_0xE2_heapbuffer;
//extern Bit8u* x_D41A0_BYTEARRAY_4_0xDE_heapbuffer;
//extern Bit32u* off_D918C[0x7c];

//xx extern Bit8u* dword_E9C30[]; // weak

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

extern Bit8u* pdwScreenBuffer;

typedef struct {//lenght 30
	Bit16s word_0;//str_E2A74[].word_0
	Bit16s axis_2[5]; //str_E2A74[].axis_2[0]
	//Bit16s axis_4[3];//?? str_E2A74[].axis_2[0]
	//Bit16s stuba;
	type_str_0x6E8E* dword_12;//str_E2A74[].dword_12
	Bit32s dword_16;
	Bit32s dword_20;//str_E2A74[].dword_20
	Bit32s dword_24;//str_E2A74[].dword_24
	//Bit8s byte_25;
	Bit8s byte_28;
	Bit8s byte_29;
}
type_str_E2A74;

extern type_str_E2A74 str_E2A74[0x69];

//extern Bit8u test_str_E2A74[];

//extern char* char_355198;
//void pathfix(char* path, char* path2);
void support_begin();
void support_end();

void loadfromsnapshot(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u size);
void loadfromsnapshot2(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u size);
Bit32u compare_with_snapshot(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u size, Bit8u* origbyte, Bit8u* copybyte);
Bit32u compare_with_sequence(char* filename, Bit8u* adress, Bit32u adressdos, long count, long size1, Bit32u size2, Bit8u* origbyte, Bit8u* copybyte, long offset = 0);
Bit32u compare_with_sequence_E7EE0(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u count, Bit32u size1, Bit32u size2, Bit8u* origbyte, Bit8u* copybyte, long offset = 0);
Bit32u compare_with_snapshot_D41A0(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u size, Bit8u* origbyte, Bit8u* copybyte);
Bit32u compare_with_sequence_D41A0(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u count, Bit32u size, Bit8u* origbyte, Bit8u* copybyte, long offset = 0);
Bit32u compare_with_sequence_array_E2A74(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u count, Bit32u size1, Bit32u size2, Bit8u* origbyte, Bit8u* copybyte, long offset = 0);
Bit32u compare_with_sequence_x_DWORD_F2C20ar(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u count, Bit32u size, Bit8u* origbyte, Bit8u* copybyte, int* posdiff);
Bit32u compare_with_sequence_array_222BD3(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u count, Bit32u size, Bit8u* origbyte, Bit8u* copybyte, int* posdiff);
Bit32u compare_with_sequence_D41A0_4(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u count, Bit32u size, Bit8u* origbyte, Bit8u* copybyte, long offset = 0);
Bit32u compare_with_sequence_EA3E4(char* filename, type_str_0x6E8E** adress, Bit32u count, Bit32u size, Bit8u* origbyte, Bit8u* copybyte);
Bit32u compare_0x6E8E(char* filename, Bit8u* adress, Bit32u count, Bit32u size, Bit8u* origbyte, Bit8u* copybyte, long offset = 0);
void add_compare(Bit32u adress, bool debugafterload, int stopstep = -1, bool skip = false);

void writehex(Bit8u* buffer, Bit32u count);

void mine_texts(char* filename, Bit32u adressdos, Bit32u count, char* outfilename);
int writeImage(char* filename, int width, int height, Bit8u* buffer, char* title);
void writeImageBMP(char* imageFileName, int width, int height, Bit8u* image);

/*
typedef struct {//lenght 8
	Bit8u byte_0;//array_0x365F4
	Bit8u byte_1;//array_0x365F5
	Bit8u byte_2;//array_0x365F6
	Bit8u byte_3;//array_0x365F7
	Bit16s word_4;//array_0x365F8
	Bit16s word_6;//array_0x365FC
} type_str_0x365F4;
*/
/*
typedef struct {
	Bit16u word0;
	Bit16u word1;
} type_str_double_word;
*/
/*
typedef struct {//lenght 8
	Bit8u str_0x3647A_byte_0;//array_0x3647a
	Bit8u str_0x3647A_byte_1;//array_0x3647b
	Bit16s str_0x3647A_word_2;//array_0x3647c
	axis_2d str_0x365F8_axis;//
} type_str_0x365F4;
*/

typedef union {
	axis_2du axis;
	type_str_0x6E8E* pointer_0x6E8E;
	//Bit32u dword;
}
un1;

typedef struct {//lenght 8
	//Bit8u byte_0;//array_0x36479
	Bit8s str_0x3647A_byte_0;//array_0x3647a
	Bit8s str_0x3647A_byte_1;//array_0x3647b
	uaxis_2d str_0x3647A_2;//array_0x3647c//str_0x3647A_2.word
	un1 str_0x3647C_4;//array_0x3647e
	/*
	axis_2d axis;//array_0x3647e
	type_str_0x6E8E* pointer_0x6E8E;//36484
	*/

	//type_str_0x6E8E* str_0x3647C;//
} type_str_0x3647Ac;

typedef struct {//lenght 8
	//Bit8u byte_0;//array_0x36479
	Bit8u str_0x3647A_byte_0;//array_0x3647a
	Bit8u str_0x3647A_byte_1;//array_0x3647b
	Bit16s str_0x3647A_word_2;//array_0x3647c
	Bit32u str_0x3647A_dword_4;//array_0x3647e
	//Bit16s str_0x3647A_word_6;//array_0x36480
} type_str_0x3647Ab;
/*
typedef struct {//lenght 20
	Bit8u byte_0;//array_0x3030E//0x30322
	Bit8u byte_1;//array_0x3650F//0x30323
	Bit8u byte_2;//array_0x36510//0x30324
	Bit8u byte_3;//array_0x36511//0x30325
	Bit8u byte_4;//array_0x36512//0x30326
	Bit16s word_5;//array_0x36513//0x30327
	Bit8u stuba[8];//array_0x36515//0x30329
	Bit8u* anystruct;//array_0x3651d//0x30331
	Bit8u stubb[1];
} type_str_0x3030E;*/

/*typedef struct {//lenght 20
	Bit8u byte_0;//type_str_0x30310//type_str_0x30324
	Bit8u str_0x30310_word_1;//type_str_0x30311//type_str_0x30325
	Bit8u byte_2;//type_str_0x30312//type_str_0x30326
	Bit16s str_0x30310_word_3;//type_str_0x30313//type_str_0x30327
	//Bit8u byte_4;//type_str_0x30314//type_str_0x30328
	Bit8u byte_5;//type_str_0x30315//type_str_0x30329
	Bit8u byte_6;//type_str_0x30316//type_str_0x3032a
	Bit8u byte_7;//type_str_0x30317//type_str_0x3032b
	Bit8u byte_8;//type_str_0x30318//type_str_0x3032c
	Bit8u byte_9;//type_str_0x30319//type_str_0x3032d
	Bit8u byte_10;//type_str_0x3031a//type_str_0x3032e
	Bit8u byte_11;//type_str_0x3031b//type_str_0x3032f
	Bit8u byte_12;//type_str_0x3031c//type_str_0x30330
	Bit8u str_0x30310_byte_13;//type_str_0x3031d//type_str_0x30331
	Bit8u byte_14;//type_str_0x3031e//type_str_0x30332
	Bit8u byte_15;//type_str_0x3031f//type_str_0x30333
	Bit8u byte_16;//type_str_0x30320//type_str_0x30334
	Bit8u byte_17;//type_str_0x30321//type_str_0x30335
	Bit8u byte_18;//type_str_0x30322//type_str_0x30336
	Bit8u byte_19;//type_str_0x30323//type_str_0x30337
}
type_str_0x30310;*/
typedef struct {//lenght 20
	//Bit8u byte_0;//type_str_0x30310//type_str_0x30324
	Bit16u str_0x30311_type;//type_str_0x30311//type_str_0x30325//1091
	//Bit8u byte_1;//type_str_0x30312//type_str_0x30326
	Bit16s str_0x30311_subtype;//type_str_0x30313//type_str_0x30327//1091
	//Bit8u byte_4;//type_str_0x30314//type_str_0x30328
	//axis_3d axis3d_4;//type_str_0x30315//type_str_0x30329
	axis_2du axis2d_4;
	Bit16u DisId;
	//Bit8u byte_5;//type_str_0x30316//type_str_0x3032a
	//Bit16u word_6;//type_str_0x30317//type_str_0x3032b
	//Bit8u byte_7;//type_str_0x30318//type_str_0x3032c
	//Bit16u word_8;//type_str_0x30319//type_str_0x3032d
	Bit16u word_10;//type_str_0x3031b//type_str_0x3032f
	//Bit8u byte_11;//type_str_0x3031c//type_str_0x30330
	Bit16u word_12;//type_str_0x3031d//type_str_0x30331
	//Bit8u byte_13;//type_str_0x3031e//type_str_0x30332
	Bit16u word_14;//type_str_0x3031f//type_str_0x30333//1105
	//Bit8u byte_15;//type_str_0x30320//type_str_0x30334
	Bit16u word_16;//type_str_0x30321//type_str_0x30335
	//Bit8u byte_17;//type_str_0x30322//type_str_0x30336
	Bit16u word_18;//type_str_0x30323//type_str_0x30337
	//Bit8u byte_19;//type_str_0x30324//type_str_0x30338
}
type_str_0x30311;

/*
typedef struct {//lenght 20
	Bit8u byte_0;//array_0x3030E
	Bit8u byte_1;//array_0x3650F
	Bit8u byte_2;//array_0x36510
	Bit8u byte_3;//array_0x36511
	Bit8u byte_4;//array_0x36512
	Bit16s word_5;//array_0x36513
	Bit8u stub[13];//array_0x36515
} type_str_0x30325;
*/
/*
typedef struct {//begin 0x649 //lenght min 0x1ee
	Bit32s dword_0x649_2BDE[0x33];//1609
	Bit8u stub1[2];
	Bit16s word_0x717_2BDE[0x1a];// -2xa1 tj 2x 26//1815
	//1817
	Bit8u stub4c[54];
	Bit16s word_0x731_2BDE;//13071
	Bit8s byte_0x781_2BDE[0x1a];//1921
	Bit8u stub4d[0x1a];
	Bit8s byte_0x7B5_2BDE[0x1a];//1973
	//Bit8u array_0x7CF_2BDE_13229[0x34];//1999
	//Bit8u array_0x7CF_2BDE_13229[0x18];//1999
	type_2255ar array_0x7CF_2BDE_13229;
	Bit8u stub4e[0x1a];
	//24
	//2

	//Bit8s array_0x7E7_2BDE_13253[0x1c];//array_0x7CF_2BDE_13229[0x18]
	//Bit8s byte_0x7E8_2BDE[0x1a];// -1x26//2024

	Bit8s byte_0x803_2BDE[0x19];// -1x26//2051

	Bit8s byte_0x81C_2BDE[0x1a];// -1x26//2076
	Bit8u stub2[1];
} type_struct_0x649_2BDE_12839;
*/

/*
typedef struct {//lenght 8
	Bit8s str_0x65ac_byte_0;//0x65ac
	Bit8s str_0x65ac_byte_1;//0x65ad
	Bit16s str_0x65ac_word_2;//0x65ae
	//Bit8s str_0x65ac_byte_3;//0x65af
	Bit16s str_0x65ac_word_4;//0x65b0
	Bit16s str_0x65ac_word_6;//0x65b2
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
	Bit8u stub[9377];
} type_str_0x24DF;

/*
typedef struct {//lenght 110
	Bit8u byte_0x360FB;
	Bit8u byte_0x360E1;
	Bit8u stuba[0x19];
	Bit8u byte_0x36115;
	//0x36141;//70
	//0x36141[8];??
	Bit8u stubb[109];
}
type_str_0x360FB;*/

typedef struct {//lenght 110 //word_0x360D2
	Bit8u stuba[3];
	Bit16s word_0x360D5;
	Bit8u stubb[2];
	Bit16s word_0x360D9;
	Bit8u stubc[2];
	Bit16s word_0x360DD;
	Bit8u stubd[2];
	Bit8u byte_0x360E1x[26];
	Bit8u byte_0x360FBx[26];
	//Bit8u byte_0x36112;
	//Bit8u stubb[0x2];
	Bit8u byte_0x36115x[26];
	//Bit8u stube[25];
	Bit16s word_0x3612F;
	//Bit8u byte_0x0x36140;
	Bit8u stubf[15];
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
	Bit8s byte_0;
	Bit16s word_1;//25973//word_0
	axis_2d _axis_2d;
	//Bit16s word_3;//25975//word_2
	//Bit16s word_5; //25977//word_4
	/*
	Bit16s word_0;//25973
	Bit16s word_2;//25975
	Bit16s word_4; //25977
	Bit8u stub[1];*/
}
type_str_0x36442;

typedef struct {//lenght 0x6604u
		//podpolozky
	//type_str_0x6E8E* dword_2FECE;//D41A0_BYTESTR_0.str_2FECE.dword_2FECE
	Bit16u word_2FECE;
	Bit16u word_2FED0;
	Bit8u byte_0x2FED2;//x_D41A0_BYTEARRAY_0[196306] // type of level graphics
	Bit8u byte_0x2FED3;
	Bit8u MapType;//x_D41A0_BYTEARRAY_0[196308]//GraphicsType
	Bit16s word_0x2FED5;
	Bit16s word_0x2FED7;
	Bit8s array_0x2FED9[8];
	Bit8u stubb[4];
	Bit16u word_0x2FEE5;//2FEE5//23
	Bit8u stubb2[2];
	Bit16u word_0x2FEE9;//2FEE9//27 //first seed position
	Bit8u stubc[2];
	Bit16u word_0x2FEED;//2FEED//31 //first seed height
	Bit8u stubc2[2];
	Bit16u word_0x2FEF1;//2FEF1//35 //random seed
	Bit8u stubc3[2];
	Bit32u word_0x2FEF5;//2FEF5//39
	//Bit8u stubd[2];
	Bit16u word_0x2FEF9;//2FEF9//43
	Bit8u stube[2];
	Bit16u word_0x2FEFD;//2FEFD//47
	Bit8u stubf[2];
	Bit16u word_0x2FF01;//2FF01//51
	Bit8u stubg[2];
	Bit16u word_0x2FF05;//2FF05//55
	Bit8u stubh[2];
	Bit16u word_0x2FF09;//2FF09//59
	Bit8u stubi[2];
	Bit16u word_0x2FF0D;//2FF0D//63
	Bit8u stubj[2];
	Bit16u word_0x2FF11;//2FF11//67
	//0x30331 - end of structure lenght 20
	/*197413 - 0x30324*/
	/*221393 - 0x360D1*/
	Bit8u stubk[1022];
	type_str_0x30311 array_0x30311[0x4b0];//end(next entity) - 0x360d1
	Bit8u next_0x360D1;

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
	Bit32s dword;
	Bit16u* ptr16u;
	type_str_0x30311* ptr0x30311;
	type_str_0x6E8E* ptr0x6E8E;
}
un_str_36552;
typedef struct {//size 10 count 8
	Bit8s str_3654C_byte0;//222540
	Bit8s str_3654D_byte1;
	axis_2d str_3654E_axis;
	//Bit16s str_3654E_word2;//objective parametres
	//Bit8s str_3654F_byte3;
	//Bit16s str_36550_word4;
	//Bit8s str_36551_byte5;
	un_str_36552 str_36552_un; //pointer sometimes
	//Bit8s str_36553_byte7;
	//Bit8s str_36554_byte8;
	//Bit8s str_36555_byte9;
} type_str_3654C;

typedef struct {//size 11 count 6
	Bit8u array_0x3659C_byte[11];//objectives 1-active 2-done
	/*Bit8s str_3659C_byte0;array_0x3659C_byte[0]
	Bit8s str_3659D_byte1;array_0x3659C_byte[1]
	Bit8s str_3659E_byte2;array_0x3659C_byte[2]
	Bit8s str_3659F_byte3;array_0x3659C_byte[3]
	Bit8s str_365A0_byte4;
	Bit8s str_365A1_byte5;
	Bit8s str_365A2_byte6;
	Bit8s str_365A3_byte7;
	Bit8s str_365A4_byte8;
	Bit8s str_365A5_byte9;
	Bit8s str_365A6_byte10;*/
} type_str_3659C;
/*
typedef struct {
	Bit32s dw;
	Bit16s w;
} dw_ws;
*/
/*typedef struct {
	Bit8s Bit_0;
	Bit8s Bit_1;
	Bit8s Bit_2;
	Bit8s Bit_3;
	Bit8s Bit_4;
	Bit8s Bit_5;
	Bit8s Bit_6;
	Bit8s Bit_7;
	Bit8s Bit_8;
	Bit8s Bit_9;
}
typedef_str_0x6E3E;*/

typedef struct {
	Bit8u m_wReflections;
	Bit8u m_wShadows;
	Bit8u m_wSky;
	Bit8u m_wViewPortSize;
}
GraphicsStruct_t;

/*typedef union {
	type_str_0x8586 str;
	Bit32u dword;
}
type_uni_0x8586;*/

typedef struct {
	Bit8u m_wMiniMap;//8590 icons//str_0x218E.str.icons_0x218E
	Bit8u m_wTopBar;//8591 ??//str_0x218E.str.xxxx_0x218F
	Bit8u scr_size_0x2190;//8592 screen size?//str_0x218E.str.scr_size_0x2190
	Bit8u xxxx_0x2191;//8593//str_0x218E.str.xxxx_0x2191
}
DisplayStruct_t;

/*typedef union {
	type_str_0x218E str;
	Bit32u dword;
}
type_uni_0x218E;*/

typedef struct {
	Bit8u xxxx_0x2192;//8594 ??//str_0x2192.str.xxxx_0x2192
	Bit8u xxxx_0x2193;//8595 ??//str_0x2192.str.xxxx_0x2193
	Bit8u m_wResolution;//8596 resolution//str_0x2192.str.resolution_0x2194
	Bit8u shadows_0x2195;//8593 shadows//str_0x2192.str.shadows_0x2195
}
type_str_0x2192;

/*typedef union {
	type_str_0x2192 str;
	Bit32u dword;
}
type_uni_0x2192;*/

typedef struct {
	Bit8u m_wDynamicLighting;//8598 lights//str_0x2196.str.lights_0x2196
	Bit8u setting_0x2197;//8599 setting//str_0x2196.str.setting_0x2197
	Bit8u transparency_0x2198;//8600 transparency//str_0x2196.str.transparency_0x2198
	Bit8u flat_0x2199;//8601 flat//str_0x2196.str.flat_0x2199
}
type_str_0x2196;

/*typedef union {
	type_str_0x2196 str;
	Bit32u dword;
}
type_uni_0x2196;*/

typedef struct {
	Bit8u creflections_0x21AA;//Bit8u byte_0x21AA;//8618 can reflections//str_0x21AA.str.creflections_0x21AA
	Bit8u cshadows_0x21AB;//Bit8u byte_0x21AB;//8619 can shadows//str_0x21AA.str.cshadows_0x21AB
	Bit8u csky_0x21AC;//Bit8u byte_0x21AC;//8620 can sky//str_0x21AA.str.csky_0x21AC
	Bit8u calter_0x21AD;//Bit8u byte_0x21AD;//8621//str_0x21AA.str.calter_0x21AD
}
type_str_0x21AA;

/*typedef union {
	type_str_0x21AA str;
	Bit32u dword;
}
type_uni_0x21AA;*/

typedef struct {
	Bit8u xxxx_0x21AE;//Bit8u byte_0x21AE;//str_0x21AE.str.xxxx_0x21AE
	Bit8u xxxx_0x21AF;//Bit8u byte_0x21AF;//str_0x21AE.str.xxxx_0x21AF
	Bit8u xxxx_0x21B0;//Bit8u byte_0x21B0;//str_0x21AE.str.xxxx_0x21B0
	Bit8u xxxx_0x21B1;//Bit8u byte_0x21B1;//str_0x21AE.str.xxxx_0x21B1
}
type_str_0x21AE;

/*typedef union {
	type_str_0x21AE str;
	Bit32u dword;
}
type_uni_0x21AE;*/

typedef struct {
	Bit8u xxxx_0x21B2;//Bit8u byte_0x21B2;//str_0x21B2.str.xxxx_0x21B2
	Bit8u xxxx_0x21B3;//Bit8u byte_0x21B3;//str_0x21B2.str.xxxx_0x21B3
	Bit8u cresolution_0x21B4;//Bit8u byte_0x21B4;//str_0x21B2.str.cresolution_0x21B4
	Bit8u xxxx_0x21B5;//Bit8u byte_0x21B5;//str_0x21B2.str.xxxx_0x21B5
}
type_str_0x21B2;

/*typedef union {
	type_str_0x21B2 str;
	Bit32u dword;
}
type_uni_0x21B2;*/

typedef struct {
	Bit8u clights_0x21B6;//Bit8u byte_0x21B6;//str_0x21B6.str.clights_0x21B6
	Bit8u csetting_0x21B7;//Bit8u byte_0x21B7;//str_0x21B6.str.csetting_0x21B7
	Bit8u ctransparency_0x21B8;//Bit8u byte_0x21B8;//str_0x21B6.str.ctransparency_0x21B8
	Bit8u cflat_0x21B9;//Bit8u byte_0x21B9;//str_0x21B6.str.cflat_0x21B9
}
type_str_0x21B6;

/*typedef union {
	type_str_0x21B6 str;
	Bit32u dword;
}
type_uni_0x21B6;*/
typedef struct {//lenght 39
	Bit8s byte_0;
	Bit8s byte_1;
	Bit8u byte_2;
	Bit8s byte_3;
	axis_3d axis3d_4;
	/*Bit16s word_6;
	Bit16s word_8;*/
	type_str_0x6E8E* dword_A;
	Bit8u array_E[25];
	//Bit8s stub[38];
}
type_str_0x3664C;

typedef struct {//lenght 108
	Bit32u dword_0x364D2;//lenght 108
	Bit32u dword_0x364D6[0x1a];// lenght 0x68=104 end 0x3653e
}
type_str_0x364D2;

typedef struct {//lenght 7
	Bit8s byte_0;//25972
	Bit8s byte_1;//25973
	Bit8u stub[5];
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
	Bit8u stub0[4];
	Bit32u dword_0x4;
	Bit32u dword_0x8;

	Bit16s word_0xc;//player_index?
	Bit16s word_0xe;
	//word 0xe - 14
	Bit8u array_0x10[0x1d]; //0x10, next 0x2d(45)
	Bit32s dword_0x2d;//45
	//Bit8u byte_0x22;
	Bit16u word_0x31;//49
	Bit16u word_0x33;//51
	Bit32s dword_0x35;//53//entity counter(max 1000 entity changes per step)
	Bit8u array_0x39[508];//57
	//Bit8u stub2[0x1fc];
	Bit32s dword_0x235;//act music
	Bit8u byte_0x239;//569
	Bit32s dword_0x23a;
	Bit32s dword_0x23e;
	Bit32s dword_0x242;
	type_str_0x6E8E* pointers_0x246[0x3e8];//pointers
	//0x81d 2077
	/*
	*(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 570) = 1;//0x23a
	*(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 574) = 0;//0x23e
	*(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 578) = 0;//0x242
	*/
	//Bit8u stub3[0x1f44];
	Bit32s dword_0x11e6;//second entity counter
	type_str_0x6E8E* dword_0x11EA[0x3e8];//??4586
	//Bit8u stub3[0xfa0];

	GameSettingsStruct_t m_GameSettings;

	Bit32u dword_0x219A;//8602
	Bit32u dword_0x219E;//8606
	Bit32u dword_0x21A2;//8610
	Bit32u dword_0x21A6;//8614

	type_str_0x21AA str_0x21AA;
	//Bit8u byte_0x21AA;//8618 can reflections
	//Bit8u byte_0x21AB;//8619 can shadows
	//Bit8u byte_0x21AC;//8620 can sky
	//Bit8u byte_0x21AD;//8621
	type_str_0x21AE str_0x21AE;
	//Bit8u byte_0x21AE;//8622
	//Bit8u byte_0x21AF;//8623
	//Bit8u byte_0x21B0;//8624
	//Bit8u byte_0x21B1;//8625
	type_str_0x21B2 str_0x21B2;
	//Bit8u byte_0x21B2;//8626
	//Bit8u byte_0x21B3;//8627
	//Bit8u byte_0x21B4;//resolution//8628
	//Bit8u byte_0x21B5;//8629
	type_str_0x21B6 str_0x21B6;
	//Bit8u byte_0x21B6;//8630 can lights
	//Bit8u byte_0x21B7;//8631
	//Bit8u byte_0x21B8;//8632 can transparency
	//Bit8u byte_0x21B9;//8633 can flat

	//Bit8u byte_0x21BA;//

	Bit8u stub3b[0x1a8];

	axis_3d array_0x2362[8];//0x30
	//Bit32s dword_0x2362;
	//Bit16s word_0x2366;
	Bit8u stub3c[0x14d];
	///*??*/type_str_0x24DF array_0x24DF[0xb];//?lenght
	Bit8u stub3d[0x6ff];
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

	//Bit8u stub3ee[0xa4];
	//Bit8u* pointer_0x6F32;

	//type_str_0x6E8E struct_0x6F36[8];//lenght 168

	//Bit8u stub3e[0x28f98];

	type_str_2FECE str_2FECE;// a1 = &x_D41A0_BYTEARRAY_0[0x2FECE/*196302*/];//fix - size 0x6604u//compress level
	/*//podpolozky
	Bit8u byte_0x2FED4;//x_D41A0_BYTEARRAY_0[196308]
	type_str_0x3030E array_0x3030E[0xb];//fix array lenght;
	type_str_0x30325 array_0x30325[0x4af];
	////0x30325 0x360d1 tj rozdil 23980=1199*20
	//36479
	type_str_0x3647A array_0x3647A[0xb];//8x11
	//konec 6604
	*/

	type_str_0x364D2 str_0x364D2;//lenght 108
	/*Bit32u dword_0x364D2;//lenght 108
	Bit8u array_0x364D6[0x68];// lenght 0x68=104 end 0x3653e*/

	//Bit8u stub3f[0x831];
	Bit16s word_0x3653E;//
	Bit16s word_0x36540;//
	Bit16s word_0x36542;//
	Bit16s word_0x36544;//
	Bit16s word_0x36546;//
	Bit16s word_0x36548;//
	Bit16s word_0x3654A;//
	type_str_3654C struct_0x3654C[0x8];//size 10 count 8
	type_str_3659C struct_0x3659C[0x8];//size 11 count 8 C-0 D-1 E-2 F-3

	//Bit16s word_0x365E0;
	//Bit8u stub3g[0x12];

	type_str_0x3647Ac array_0x365F4[0xb];//8x11	//set 0x58 // end 0x3664c

	//Bit8s byte_0x365FC;//array_0x365F4[1].byte_0
	//Bit8s byte_0x365FD;//array_0x365F4[1].byte_1

	type_str_0x3664C str_0x3664C[0x32];// lenght 0x79e//39x50 (end-0x36dea)
	//Bit8u stub3h[0x79e];
	Bit8s byte_0x36DEA_fly_asistant;
	Bit8s byte_0x36DEB_xx;
	Bit16s word_0x36DEC_mousex;
	Bit16s word_0x36DEE_mousey;
	Bit16s word_0x36DF0_mousexx;
	Bit32u dword_0x36DF2;//index
	type_str_160* dword_0x36DF6;//pointer
	Bit16s word_0x36DFA;
	Bit16s word_0x36DFC;
	Bit16s word_0x36DFE;
	Bit8u byte_0x36E00;
	Bit8u byte_0x36E01;//count objectives
	Bit8s byte_0x36E02;//temp objective
	Bit8s byte_0x36E03;
	Bit8s byte_0x36E04;
	Bit8u stub3k[0x6];
	Bit8s byte_0x36E0B;

	//&x_D41A0_BYTEARRAY_0[197413]; ix < &x_D41A0_BYTEARRAY_0[221393]; ix += 20)

	//type_str_0x365F2 array_str_0x365F2[0x10];//fix it
	//*(0x365F2 + x_D41A0_BYTEARRAY_0 + 8 * (signed __int16)result + 2)

	Bit8u stubend[0xb];
} type_D41A0_BYTESTR_0; // level data (gets saved to and loaded from SAVE/SLEVxx.dat) was originally forced to 224791 on load/save which is out of bounds

struct {//lenght 0x33
	Bit16s word_0x0;//0 //0
	Bit16s word_0x2;//2 //1
	Bit16s word_0x4;//4 //2
	Bit16s word_0x6;//6 //3
	Bit16s word_0x8;//8 //4
	Bit16s word_0xa;//a //5
	Bit16s word_0xc;//c-3f //6
	Bit16s word_0xe;//e //7
	Bit16s word_0x10;//10 //8
	Bit16s word_0x12;//12 //9
	Bit16s word_0x14;//14 //10
	Bit16s word_0x16;//16 //11
	Bit16s word_0x18;//18 //12
	Bit16s word_0x1a;//1a //13
	Bit16s word_0x1c;//1c-4f //14
	Bit16s word_0x1e;//1e-51 //15
	Bit16s word_0x20;//20 //16
	Bit16s word_0x22;//22 //17
	Bit16s word_0x24;//24-57 //width 18
	Bit16s word_0x26;//26-59 //19
	Bit16s word_0x28;//28-5b //20
	Bit16s word_0x2a;//2a-5d//21
	Bit8s stub[4];
	Bit8s byte_0x30;//30-63 - color1 //24
	Bit8s byte_0x31;//31-64 - color2
	Bit8s byte_0x32;//32-65 - color2
}
typedef type_sub_str_unk_1804B0ar;

typedef union {//lenght 12
	Bit32s dword[3];
	Bit16s word[6];
}
type_uni_0x8a;

struct {//0xb0 nebo spis 171(0xab)
	type_sub_str_unk_1804B0ar type_sub_0[2];
	/*Bit16s word_0xe;//e
	Bit16s word_0x10;//10
	Bit16s word_0x12;//12
	Bit16s word_0x14;//14//47=14+33
	Bit16s word_0x16;//16//49=16+33
	Bit16s word_0x18;//18//4b=18+33
	Bit16s word_0x1a;//1a//4d;=1a+33
	//0x33
	//0x66
	*/

	Bit32s dword_0x66;//66
	Bit32s dword_0x6a;//6a
	Bit32s dword_0x6e;//6e
	Bit32s dword_0x72;//72
	Bit32s dword_0x76;//76
	Bit32s dword_0x7a;//7a
	Bit8s stub[4];
	Bit32s dword_0x82;//82
	Bit16s word_0x86;//86
	Bit16s word_0x88;//88
	type_uni_0x8a uni_0x8a;
	//Bit32s dword_0x8a;//8a
	//Bit16s array_0x8e[4];//8e //text lines
	Bit16s word_0x96;//96
	Bit16s word_0x98;//98
	Bit16s word_0x9a;//9A
	Bit16s word_0x9c;//9C
	Bit8s byte_0x9e;//9e
	Bit8s byte_0x9f;//9f
	Bit8s byte_0xa0;//a0
	Bit8s byte_0xa1;//a1
	Bit8s byte_0xa2;//a2
	Bit8s byte_0xa3;//a3
	Bit8s byte_0xa4;//a4
	Bit8s byte_0xa5;//a5
	Bit8s byte_0xa6;//a6
	Bit8s byte_0xa7;//a7
	Bit8s byte_0xa8;//a8
	Bit8s byte_0xa9;//a9
	Bit8s byte_0xaa;//aa
	//Bit8s stubb[1];
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
*(Bit32s*)&x_D41A0_BYTEARRAY_0[0x341c]
-selected bottom spell

x_D41A0_BYTEARRAY_0[28302]-168 arrays
x_D41A0_BYTEARRAY_0[0x6e8e]-0xa8 arrays
*/

/*
Bit32s get_x_D41A0_BYTEARRAY_0_4();
void set_x_D41A0_BYTEARRAY_0_4(Bit32s value);

Bit32s get_x_D41A0_BYTEARRAY_0_8();
void set_x_D41A0_BYTEARRAY_0_8(Bit32s value);

Bit16s get_x_D41A0_BYTEARRAY_0_0xe();
void set_x_D41A0_BYTEARRAY_0_0xe(Bit16s value);

Bit8s get_x_D41A0_BYTEARRAY_0_0x10(int number);
void set_x_D41A0_BYTEARRAY_0_0x10(int number,Bit8s value);
void plus_x_D41A0_BYTEARRAY_0_0x10(int number);

Bit8u get_x_D41A0_BYTEARRAY_0_0x22();
void set_x_D41A0_BYTEARRAY_0_0x22(Bit8u value);

Bit32s get_x_D41A0_BYTEARRAY_0_0x35();
void set_x_D41A0_BYTEARRAY_0_0x35(Bit32s value);
void plus_x_D41A0_BYTEARRAY_0_0x35();
void minus_x_D41A0_BYTEARRAY_0_0x35();

Bit32s get_x_D41A0_BYTEARRAY_0_235();
void set_x_D41A0_BYTEARRAY_0_235(Bit32s value);

Bit32s get_x_D41A0_BYTEARRAY_0_23a();
void set_x_D41A0_BYTEARRAY_0_23a(Bit32s value);

Bit32s get_x_D41A0_BYTEARRAY_0_23e();
void set_x_D41A0_BYTEARRAY_0_23e(Bit32s value);

Bit32s get_x_D41A0_BYTEARRAY_0_242();
void set_x_D41A0_BYTEARRAY_0_242(Bit32s value);

type_str_0x6E8E* get_x_D41A0_BYTEARRAY_0_0x246(int number);
void set_x_D41A0_BYTEARRAY_0_0x246(int number, type_str_0x6E8E* value);

Bit32s get_x_D41A0_BYTEARRAY_0_0x11e6();
void set_x_D41A0_BYTEARRAY_0_0x11e6(Bit32s value);

Bit8u get_x_D41A0_BYTEARRAY_0_0x218a();
void set_x_D41A0_BYTEARRAY_0_0x218a(Bit8u value);
Bit8u get_x_D41A0_BYTEARRAY_0_0x218b();
void set_x_D41A0_BYTEARRAY_0_0x218b(Bit8u value);
Bit8u get_x_D41A0_BYTEARRAY_0_0x218c();
void set_x_D41A0_BYTEARRAY_0_0x218c(Bit8u value);
Bit8u get_x_D41A0_BYTEARRAY_0_0x218d();
void set_x_D41A0_BYTEARRAY_0_0x218d(Bit8u value);
Bit8u get_x_D41A0_BYTEARRAY_0_0x218e();
void set_x_D41A0_BYTEARRAY_0_0x218e(Bit8u value);
Bit8u get_x_D41A0_BYTEARRAY_0_0x218f();
void set_x_D41A0_BYTEARRAY_0_0x218f(Bit8u value);
Bit8u get_x_D41A0_BYTEARRAY_0_0x2190();
void set_x_D41A0_BYTEARRAY_0_0x2190(Bit8u value);
Bit8u get_x_D41A0_BYTEARRAY_0_0x2196();
void set_x_D41A0_BYTEARRAY_0_0x2196(Bit8u value);
Bit8u get_x_D41A0_BYTEARRAY_0_0x2197();
void set_x_D41A0_BYTEARRAY_0_0x2197(Bit8u value);
Bit8u get_x_D41A0_BYTEARRAY_0_0x2198();
void set_x_D41A0_BYTEARRAY_0_0x2198(Bit8u value);
Bit8u get_x_D41A0_BYTEARRAY_0_0x2199();
void set_x_D41A0_BYTEARRAY_0_0x2199(Bit8u value);

Bit8u get_x_D41A0_BYTEARRAY_0_0x21aa();
void set_x_D41A0_BYTEARRAY_0_0x21aa(Bit8u value);
Bit8u get_x_D41A0_BYTEARRAY_0_0x21ab();
void set_x_D41A0_BYTEARRAY_0_0x21ab(Bit8u value);
Bit8u get_x_D41A0_BYTEARRAY_0_0x21ac();
void set_x_D41A0_BYTEARRAY_0_0x21ac(Bit8u value);
Bit8u get_x_D41A0_BYTEARRAY_0_0x21ad();
void set_x_D41A0_BYTEARRAY_0_0x21ad(Bit8u value);
Bit8u get_x_D41A0_BYTEARRAY_0_0x21ae();
void set_x_D41A0_BYTEARRAY_0_0xx21ae(Bit8u value);
Bit8u get_x_D41A0_BYTEARRAY_0_0x21af();
void set_x_D41A0_BYTEARRAY_0_0x21af(Bit8u value);
Bit8u get_x_D41A0_BYTEARRAY_0_0x21b0();
void set_x_D41A0_BYTEARRAY_0_0x21b0(Bit8u value);
Bit8u get_x_D41A0_BYTEARRAY_0_0x21b1();
void set_x_D41A0_BYTEARRAY_0_0x21b1(Bit8u value);
Bit8u get_x_D41A0_BYTEARRAY_0_0x21b2();
void set_x_D41A0_BYTEARRAY_0_0x21b2(Bit8u value);
Bit8u get_x_D41A0_BYTEARRAY_0_0x21b3();
void set_x_D41A0_BYTEARRAY_0_0x21b3(Bit8u value);
Bit8u get_x_D41A0_BYTEARRAY_0_0x21b4();
void set_x_D41A0_BYTEARRAY_0_0x21b4(Bit8u value);
Bit8u get_x_D41A0_BYTEARRAY_0_0x21b5();
void set_x_D41A0_BYTEARRAY_0_0x21b5(Bit8u value);
Bit8u get_x_D41A0_BYTEARRAY_0_0x21b6();
void set_x_D41A0_BYTEARRAY_0_0x21b6(Bit8u value);
Bit8u get_x_D41A0_BYTEARRAY_0_0x21b7();
void set_x_D41A0_BYTEARRAY_0_0x21b7(Bit8u value);
Bit8u get_x_D41A0_BYTEARRAY_0_0x21b8();
void set_x_D41A0_BYTEARRAY_0_0x21b8(Bit8u value);
*/
//axis_3d get_x_D41A0_BYTEARRAY_0_0x2362(int number); //lenght 48 - 6x8
//void set_x_D41A0_BYTEARRAY_0_0x2362(int number, axis_3d value);
/*Bit16s get_x_D41A0_BYTEARRAY_0_0x2366(int number);
void set_x_D41A0_BYTEARRAY_0_0x2366(int number, Bit16s value);*/

//void clean_x_D41A0_BYTEARRAY_0_0x2362();

void clean_x_D41A0_BYTEARRAY_0_0x2BDE(int number);
/*
void copyto_x_D41A0_BYTEARRAY_0_0x2BDE_0x7CF(int number,Bit8u* value);
void copyto_x_D41A0_BYTEARRAY_0_0x2BDE_0x7E7(int number, Bit8u* value);

void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x7(int number, Bit16s value);
Bit8s get_x_D41A0_BYTEARRAY_0_0x2BDE_0x9(int number);
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x9(int number, Bit8s value);
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0xe(int number, Bit16s value);
Bit16s get_x_D41A0_BYTEARRAY_0_0x2BDE_0x10(int number);
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x10(int number, Bit16s value);
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x18(int number, Bit32s value);

void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x3e1(int number, Bit8s value);

Bit32s get_x_D41A0_BYTEARRAY_0_0x2BDE_0x1d1_2BDE_11707(int index, int subindex);//465
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x1d1_2BDE_11707(int index, int subindex, Bit32s value);//465

Bit32s get_x_D41A0_BYTEARRAY_0_0x2BDE_0x1d5_2BDE_11707(int index, int subindex);//469
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x1d5_2BDE_11707(int index, int subindex, Bit32s value);//469

Bit32s get_x_D41A0_BYTEARRAY_0_0x2BDE_0x1d9_2BDE_11707(int index, int subindex);//473
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x1d9_2BDE_11707(int index, int subindex, Bit32s value);//473

Bit16s get_x_D41A0_BYTEARRAY_0_0x2BDE_0x1dd_2BDE_11707(int index, int subindex);//477
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x1dd_2BDE_11707(int index, int subindex, Bit16s value);//477

Bit8s* get_x_D41A0_BYTEARRAY_0_0x2BDE_0x39f_2BFA_12157(int index);

void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x5a6(int number, Bit8s value);

Bit8u* get_x_D41A0_BYTEARRAY_0_0x2BDE_0x649(int number);
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x649(int number,int subindex, Bit32s value);

void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x717(int index, int subindex, Bit16s value);
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x781(int index, int subindex, Bit8s value);
Bit8s get_x_D41A0_BYTEARRAY_0_0x2BDE_0x7B5(int index, int subindex);
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x7B5(int index, int subindex, Bit8s value);
Bit8s get_x_D41A0_BYTEARRAY_0_0x2BDE_0x7CF(int index, int subindex);
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x7CF(int index, int subindex, Bit8s value);
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x7e8(int index, int subindex, Bit8s value);
Bit8s get_x_D41A0_BYTEARRAY_0_0x2BDE_0x803(int index, int subindex);
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x803(int index, int subindex, Bit8s value);
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x81c(int index, int subindex, Bit8s value);

Bit16s get_x_D41A0_BYTEARRAY_0_0x2BDE_0x837(int index);
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x837(int index, Bit16s value);
Bit16s get_x_D41A0_BYTEARRAY_0_0x2BDE_0x839(int index);
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x839(int index, Bit16s value);

Bit8u get_x_D41A0_BYTEARRAY_0_0x2BDE(int number);
void set_x_D41A0_BYTEARRAY_0_0x2BDE(int number,Bit8u value);

Bit8s get_x_D41A0_BYTEARRAY_0_0x6E3E(int index,int subindex);
void set_x_D41A0_BYTEARRAY_0_0x6E3E(int index, int subindex, Bit8s value);

type_str_0x6E8E* get_x_D41A0_BYTEARRAY_0_0x6E8E_adr();
//28302
//((Bit8u*)v1x - (x_D41A0_BYTEARRAY_0 + 28302)) / 168
//ix-get_x_D41A0_BYTEARRAY_0_0x6E8E_adr()
Bit8u* get_x_D41A0_BYTEARRAY_0_0x6F32();
void set_x_D41A0_BYTEARRAY_0_0x6F32(Bit8u* value);

Bit8u get_x_D41A0_BYTEARRAY_0_0x36e0b();
void set_x_D41A0_BYTEARRAY_0_0x36e0b(Bit8u value);
void and_x_D41A0_BYTEARRAY_0_0x36e0b(Bit8u value);

void clean_x_D41A0_BYTEARRAY_0();

void test_x_D41A0_BYTEARRAY_0();
*/

extern type_D41A0_BYTESTR_0 D41A0_BYTESTR_0;

/*void x_D41A0_BYTEARRAY_0_to_x_D41A0_BYTESTR_0();
void x_D41A0_BYTESTR_0_to_x_D41A0_BYTEARRAY_0();*/
void write_posistruct_to_png(Bit8u* buffer, int width, int height, char* filename);

int my_sign32(Bit32s var);
int my_sign16(Bit16s var);

#endif //ENGINE_SUPPORT_ACTIVE
