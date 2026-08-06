#pragma once
#ifndef MAIN_BASIC_TERRAIN
#define MAIN_BASIC_TERRAIN
#include "BasicMini.h"
#include "MapType.h"
#pragma pack(push, 1)
typedef struct {//lenght 20
	uint16_t type_0x30311;//type_str_0x30311//type_str_0x30325//1091
	int16_t subtype_0x30311;//type_str_0x30313//type_str_0x30327//1091
	axis_2du axis2d_4;
	int16_t DisId;// must be signed
	uint16_t word_10;//type_str_0x3031b//type_str_0x3032f
	int16_t stageTag_12;//type_str_0x3031d//type_str_0x30331
	uint16_t par1_14;//type_str_0x3031f//type_str_0x30333//1105
	uint16_t par2_16;//type_str_0x30321//type_str_0x30335
	uint16_t par3_18;//type_str_0x30323//type_str_0x30337
}
type_entity_0x30311;

typedef struct {//lenght 110 //word_0x360D2
	uint8_t stuba[3];
	int16_t Aggression_0x360D5;
	uint8_t stubb[2];
	int16_t Reflexes_0x360D9;
	uint8_t stubc[2];
	int16_t Perception_0x360DD;
	uint8_t stubd[2];
	uint8_t StartingSpells_0x360E1x[26];
	uint8_t byte_0x360FBx[26];
	uint8_t BlockedSpells_0x36115x[26];
	int16_t Life_0x3612F;
	uint8_t stubf[15];
}
Type_WizardMapSettings_0x360D2;

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
	int8_t index_0;
	int16_t stage_1;//25973//word_0
	axis_2d _axis_2d;
}
type_str_0x36442;

typedef union {
	uint32_t dword;
	axis_2du axis;
	type_entity_0x6E8E* pointer_0x6E8E;
}
un1;

typedef struct {//lenght 8
	int8_t index_0x3647A_0;//array_0x3647a
	int8_t stage_0x3647A_1;//array_0x3647b
	uaxis_2d str_0x3647A_2;//array_0x3647c//str_0x3647A_2.word
	un1 str_0x3647C_4;//array_0x3647e
} type_str_0x3647Ac;

typedef struct {//lenght 0x6604u
	uint16_t word_2FECE;
	uint16_t levelID_2FED0;
	uint8_t byte_0x2FED2;//x_D41A0_BYTEARRAY_0[196306] // type of level graphics
	uint8_t byte_0x2FED3;
	MapType_t MapType;//x_D41A0_BYTEARRAY_0[196308]//GraphicsType
	int16_t word_0x2FED5;
	int16_t word_0x2FED7;
	int8_t player_0x2FED9[8];
	uint8_t stubb[4];
	uint16_t seed_0x2FEE5;//2FEE5//23
	uint8_t stubb2[2];
	uint16_t offset_0x2FEE9;//2FEE9//27 //first seed position
	uint8_t stubc[2];
	uint16_t raise_0x2FEED;//2FEED//31 //first seed height
	uint8_t stubc2[2];
	uint16_t gnarl_0x2FEF1;//2FEF1//35 //random seed
	uint8_t stubc3[2];
	uint32_t river_0x2FEF5;//2FEF5//39
	uint16_t lriver_0x2FEF9;//2FEF9//43
	uint8_t stube[2];
	uint16_t source_0x2FEFD;//2FEFD//47
	uint8_t stubf[2];
	uint16_t snLin_0x2FF01;//2FF01//51
	uint8_t stubg[2];
	uint16_t snFlt_0x2FF05;//2FF05//55
	uint8_t stubh[2];
	uint16_t bhLin_0x2FF09;//2FF09//59
	uint8_t stubi[2];
	uint16_t bhFlt_0x2FF0D;//2FF0D//63
	uint8_t stubj[2];
	uint16_t rkSte_0x2FF11;//2FF11//67
	uint8_t stubk[1022];
	type_entity_0x30311 entity_0x30311[1200];//end(next entity) - 0x360d1
	uint8_t next_0x360D1;
	Type_WizardMapSettings_0x360D2 WizardMapSettings_0x360D2[8];//lenght 110  /spells?
	type_str_0x36442 stages_0x36442[8];//stages(checkpoints)
	type_str_0x3647Ac StageVars_0x3647A[11];//8x11
} Type_Level_2FECE;//Uncompressed level
#pragma pack(pop)

extern bool isCaveLevel_D41B6;
extern uint8_t* x_BYTE_14B4E0_second_heightmap;

#endif //MAIN_BASIC_TERRAIN