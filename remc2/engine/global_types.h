#pragma once

#include <array>
#include <cstdint>

typedef union {
	uint8_t byte[4];
	uint16_t word[2];
	uint32_t dword;
}
dw_w_b;

#pragma pack (push)
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
	int16_t z;
}axis_3d;

typedef struct {
	int16_t x;
	int16_t y;
	int16_t z;
}axis_3ds;
/*
typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t z;
}axis_3du;
*/
typedef struct {
	int16_t yaw;
	int16_t pitch;
	int16_t roll;
	int16_t fov;
}axis_4d;

typedef struct {
	uint8_t x;
	uint8_t y;
} baxis_2d;

typedef struct {
	int8_t x;
	int8_t y;
} bsaxis_2d;

typedef union {
	baxis_2d _axis_2d;
	uint16_t word;
}
uaxis_2d;

typedef struct {//lenght 34
	int16_t type_160_0x0_0;
	int16_t subtype_160_0x2_2;
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

typedef struct {//lenght 18
	int16_t word_0;
	int16_t word_2BDE_12658[8];
}type_str_0x1AC_428;

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
/*typedef struct {
	int16_t x;
	int16_t y;
}
type_str_164_pos;*/

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

typedef struct Type_str_164 {//size 1136
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
	std::array<uint16_t, 103> array_0x5C_92; // size?? at least index 33 is accessed in level 19
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
	int8_t array_0x15B_347[8] = {0,0,0,0,0,0,0,0};
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
	int32_t maxDistance_0x19E_414;
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
	std::array<int8_t, 19> array_0x24E_590; // size?? -> at least 12 in level 19. using the whole space of stubn now.
	//uint8_t stubn[8];
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
	uint16_t rand_0x14_20;//20 //random seed adress
	uint16_t oldMapEntity_0x16_22;//22
	uint16_t nextEntity_0x18_24;//24 //next entity index
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
	uint8_t type_0x3F_63;//63 //type//MODEL
	//11 - spell
	uint8_t subtype_0x40_64;//64 //subtype//CLASS
	//0 or 1 - player, 2 - castle,3-ballon
	int8_t xtype_0x41_65;//65 //type subentity
	int8_t xsubtype_0x42_66;//66 //subtype subentity
	int8_t byte_0x43_67;//67
	int8_t byte_0x44_68;//68
	uint8_t byte_0x45_69;//69 // index of structure with lenght 14//STATE-
	int8_t byte_0x46_70;//70 // index of bitmap // index of ending sequence
	int8_t byte_0x47_71_xx;//71
	int8_t StageVar1_0x48_72;//72
	int8_t StageVar2_0x49_73;//70
	int16_t word_0x4A_74;
	axis_3d axis_0x4C_76;//position//ACTUAL X Y Z
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
type_event_0x6E8E;

#pragma pack (pop)
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
