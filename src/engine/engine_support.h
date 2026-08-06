#pragma once
#ifndef ENGINE_SUPPORT_H
#define ENGINE_SUPPORT_H

#include <stdio.h>
#include <cstdint>
#include <assert.h>
#include <array>
#include <map>
#include <sstream>
#include <stddef.h>
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */

#ifdef _DEBUG
#include "png.h"
#endif

#include "global_types.h"
#include "MapType.h"
#include "portability/port_time.h"
#include "portability/port_sdl_vga_mouse.h"
#include "portability/port_sdl_joystick.h"
#include "portability/port_outputs.h"
#include "portability/port_show_perifery.h"

#include "BasicTerrain.h"
#include "LevelStructs.h"
#include "ScreenBuff.h"

#pragma pack(push, 1)
typedef struct {//lenght 3
	uint8_t red;
	uint8_t green;
	uint8_t blue;
}
TColor;
#pragma pack(pop)

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

extern bool unitTests;
extern std::string unitTestsPath;
extern int* endTestsCode;

extern uint8_t* readbuffer;
extern const int printBufferSize;
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

#pragma pack(push, 1)

typedef struct {
	int32_t dword[26];
}
type_dword_0x649_2BDE;

typedef struct {//size 5152
	//type_particle_str_0 un_0;//lenght?
	uint16_t word_0;
	uint16_t width;
	uint16_t height;
	int8_t textureBuffer[];
}
type_particle_str;

typedef struct {//size 14
	type_particle_str* partstr_0;//type_particle_str?
	int32_t dword_4;
	int16_t word_8;
	int16_t Index;
	int16_t word_12;
}
subtype_x_DWORD_E9C28_str;

typedef struct {//size 4
	int8_t byte_0;
	int8_t byte_1;
	int8_t byte_2;
	int8_t byte_3;
}
subtype_x_DWORD_E9C28_str2;

typedef struct {//size 26
	int32_t dword_0;
	int32_t dword_4;
	subtype_x_DWORD_E9C28_str* str_8_data;
	subtype_x_DWORD_E9C28_str** dword_12x;//uint32_t*
	uint32_t* dword_16x;//uint32_t*
	int16_t word_20;
	int16_t word_22;
	int16_t word_24;
	subtype_x_DWORD_E9C28_str data[];
	//uint32_t dword_26;
} type_x_DWORD_E9C28_str;

typedef struct {//lenght 14
	int8_t stuba[4];
	int32_t dword_4;
	uint32_t dword_8;
	int8_t stubb[2];
} type_index_242ar;

typedef struct Type_x_D41A0_BYTEARRAY_4_struct {
	//uint8_t harddisk_number;

	//uint8_t setting_byte5_19;
	uint32_t configDatSign_0;//x_D41A0_BYTEARRAY_4_struct.dwordindex_0
	uint16_t langIndex_4;//x_D41A0_BYTEARRAY_4_struct.byteindex_4
	uint16_t soundVolume_6;//x_D41A0_BYTEARRAY_4_struct.wordindex_6
	uint16_t musicVolume_8;//x_D41A0_BYTEARRAY_4_struct.wordindex_8
	uint8_t showHelp_10;//0xa//x_D41A0_BYTEARRAY_4_struct.byteindex_10//show help
	int8_t brightness_11;//0xb//x_D41A0_BYTEARRAY_4_struct.byteindex_11
	int8_t brightness_12;//0xc//x_D41A0_BYTEARRAY_4_struct.byteindex_12
	int8_t brightness_13;//0xd//x_D41A0_BYTEARRAY_4_struct.byteindex_13
	uint16_t wordindex_14;
	uint32_t dwordindex_16;//0x10//x_D41A0_BYTEARRAY_4_struct.dwordindex_16
	uint8_t stubb[2];
	uint8_t setting_byte1_22;//x_D41A0_BYTEARRAY_4_struct.setting_byte1_22
	int8_t setting_byte2_23;//0x17//x_D41A0_BYTEARRAY_4_struct.setting_byte2_23
	uint8_t OptionsSettingFlag_24;//cheats? 0x20==free spell//x_D41A0_BYTEARRAY_4_struct.OptionsSettingFlag_24
	//spell on - 0x20
	uint8_t setting_byte4_25;//cheats? 0x1==Invincability 0x8==tester 0x40==music
	//invincability - 1;
	uint8_t FrameTimingIndex_26 = 0;//Used to time flashing indicators
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
	int8_t spellOnCursor_50;//0x32//x_D41A0_BYTEARRAY_4_struct.byteindex_50
	uint8_t paletteMod_51;//0x33//x_D41A0_BYTEARRAY_4_struct.byteindex_51
	uint8_t byteindex_52;//0x34//x_D41A0_BYTEARRAY_4_struct.byteindex_52
	uint8_t byteindex_53;//0x35//x_D41A0_BYTEARRAY_4_struct.byteindex_53
	uint8_t stubh[3];
	char player_name_57ar[32];
	char savestring_89[32];
	uint8_t colorIndex_121[16];//0x79//x_D41A0_BYTEARRAY_4_struct.byteindex_121
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
	uint8_t paletteSubMod_180;//0xb4//x_D41A0_BYTEARRAY_4_struct.byteindex_180
	uint8_t moveSpeedFlag_181;//0xb5//x_D41A0_BYTEARRAY_4_struct.byteindex_181
	uint8_t byteindex_183;//0xb7//x_D41A0_BYTEARRAY_4_struct.byteindex_183
	uint16_t paletteCount_184w;//0xb8//x_D41A0_BYTEARRAY_4_struct.byteindex_184
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
	uint16_t isNetwork_216w;
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
	type_index_242ar str_index_242ar;//0xf2//x_D41A0_BYTEARRAY_4_struct.byteindex_242ar
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
	type_entity_0x6E8E* dwordindex_38396;//0x95FC//x_D41A0_BYTEARRAY_4_struct.dwordindex_38396
	uint8_t leftSpellPlayerIndex_38400;//0x9600//x_D41A0_BYTEARRAY_4_struct.leftSpellPlayerIndex_38400
	uint8_t rightSpellPlayerIndex_38401;//0x9601//x_D41A0_BYTEARRAY_4_struct.rightSpellPlayerIndex_38401

	uint8_t setting_38402;//0x9602//x_D41A0_BYTEARRAY_4_struct.setting_38402

	type_entity_0x6E8E* bytearray_38403x[29];//array 116//0x9603//x_D41A0_BYTEARRAY_4_struct.bytearray_38403
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
	type_entity_0x6E8E* dword_38519;//0x9677//x_D41A0_BYTEARRAY_4_struct.dword_38519
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
	type_entity_0x6E8E* dword_38523;//0x967b//x_D41A0_BYTEARRAY_4_struct.dword_38523
	type_entity_0x6E8E* dword_38527;//0x967F//x_D41A0_BYTEARRAY_4_struct.dword_38527
	type_entity_0x6E8E* dword_38531;//0x9683//x_D41A0_BYTEARRAY_4_struct.dword_38531
	type_entity_0x6E8E* dword_38535;//x_D41A0_BYTEARRAY_4_struct.dword_38535
	uint8_t stubv[5];
	uint8_t byte_38544;//x_D41A0_BYTEARRAY_4_struct.byte_38544
	uint8_t setting_38545;//0x9691//x_D41A0_BYTEARRAY_4_struct.setting_38545
	uint8_t SelectedMenuItem_38546;//0x9692//x_D41A0_BYTEARRAY_4_struct.byte_38546
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
//extern uint32_t* off_D918C[124];

//xx extern uint8_t* dword_E9C30[]; // weak

extern bitmap_pos_struct_t* xy_DWORD_17DED4_spritestr;
extern bitmap_pos_struct_t* xy_DWORD_17DEC0_spritestr;
extern bitmap_pos_struct_t* xy_DWORD_17DEC8_spritestr;

extern bitmap_pos_struct_t* x_DWORD_D4188t_spritestr;

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

typedef struct {//lenght 30
	int16_t word_0;//str_E2A74[].word_0
	std::array<int16_t, 5> axis_2; // 1=textIndex 3=lineX1, 4=lineY1
	//int16_t axis_4[3];//?? str_E2A74[].axis_2[0]
	//int16_t stuba;
	type_entity_0x6E8E* dword_12;//str_E2A74[].dword_12
	int32_t dword_16;
	int32_t dword_20;//str_E2A74[].dword_20
	int32_t dword_24;//str_E2A74[].dword_24
	//int8_t byte_25;
	int8_t byte_28;
	int8_t byte_29;
}
type_str_E2A74;

typedef struct {//lenght 30
	int16_t word_0;//str_E2A74[].word_0
	int16_t axis_2[5]; //str_E2A74[].axis_2[0]
	//int16_t axis_4[3];//?? str_E2A74[].axis_2[0]
	//int16_t stuba;
	int32_t dword_12;//str_E2A74[].dword_12
	int32_t dword_16;
	int32_t dword_20;//str_E2A74[].dword_20
	int32_t dword_24;//str_E2A74[].dword_24
	//int8_t byte_25;
	int8_t byte_28;
	int8_t byte_29;
}
type_shadow_str_E2A74;

typedef std::array<type_str_E2A74, 0x69> type_array_str_E2A74;

extern type_array_str_E2A74 str_E2A74;

//extern uint8_t test_str_E2A74[];

//extern char* char_355198;
//void pathfix(char* path, char* path2);

class thread_exit_exception : public std::exception {};

void End_thread(int backCode);

void support_begin();
void support_end();

void loadfromsnapshot(const char* filename, uint8_t* adress, uint32_t adressdos, uint32_t size);
void loadfromsnapshot2(const char* filename, uint8_t* adress, uint32_t adressdos, uint32_t size);
uint32_t compare_with_snapshot(const char* filename, uint8_t* adress, uint32_t adressdos, uint32_t size, uint8_t* origbyte, uint8_t* copybyte);
uint32_t compare_with_sequence(const char* filename, const uint8_t* adress, uint32_t adressdos, long count, long size1, uint32_t size2, uint8_t* origbyte, uint8_t* copybyte, long offset = 0, bool regressions=false);
uint32_t compare_with_sequence_E7EE0(const char* filename, uint8_t* adress, uint32_t adressdos, uint32_t count, uint32_t size1, uint32_t size2, uint8_t* origbyte, uint8_t* copybyte, long offset = 0);
uint32_t compare_with_snapshot_D41A0(const char* filename, uint8_t* adress, uint32_t adressdos, uint32_t size, uint8_t* origbyte, uint8_t* copybyte);
uint32_t compare_with_sequence_D41A0(const char* filename, uint8_t* adress, uint32_t adressdos, uint32_t count, uint32_t size, uint8_t* origbyte, uint8_t* copybyte, long offset = 0, bool regressions = false);
uint32_t compare_with_sequence_array_E2A74(const char* filename, uint8_t* adress, uint32_t adressdos, uint32_t count, uint32_t size1, uint32_t size2, uint8_t* origbyte, uint8_t* copybyte, long offset = 0, bool regressions = false);
uint32_t compare_with_sequence_x_DWORD_F2C20ar(const char* filename, uint8_t* adress, uint32_t adressdos, uint32_t count, uint32_t size, uint8_t* origbyte, uint8_t* copybyte, int* posdiff);
uint32_t compare_with_sequence_array_222BD3(const char* filename, uint8_t* adress, uint32_t adressdos, uint32_t count, uint32_t size, uint8_t* origbyte, uint8_t* copybyte, int* posdiff);
uint32_t compare_with_sequence_D41A0_4(const char* filename, uint8_t* adress, uint32_t adressdos, uint32_t count, uint32_t size, uint8_t* origbyte, uint8_t* copybyte, long offset = 0);
uint32_t compare_with_sequence_EA3E4(const char* filename, type_entity_0x6E8E** adress, uint32_t count, uint32_t size, uint8_t* origbyte, uint8_t* copybyte);
uint32_t compare_0x6E8E(const char* filename, uint8_t* adress, uint32_t count, uint32_t size, uint8_t* origbyte, uint8_t* copybyte, long offset = 0);
void writehex(uint8_t* buffer, uint32_t count);

void mine_texts(const char* filename, uint32_t adressdos, uint32_t count, char* outfilename);
#ifdef _DEBUG
int writeImage(const char* filename, int width, int height, uint8_t* buffer, char* title);
#endif
void writeImageBMP(const char* imageFileName, int width, int height, uint8_t* image);

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
	int32_t dword_0x649_2BDE[51];//1609
	uint8_t stub1[2];
	int16_t word_0x717_2BDE[26];// -2xa1 tj 2x 26//1815
	//1817
	uint8_t stub4c[54];
	int16_t word_0x731_2BDE;//13071
	int8_t byte_0x781_2BDE[26];//1921
	uint8_t stub4d[26];
	int8_t byte_0x7B5_2BDE[26];//1973
	//uint8_t array_0x7CF_2BDE_13229[52];//1999
	//uint8_t array_0x7CF_2BDE_13229[24];//1999
	type_2255ar array_0x7CF_2BDE_13229;
	uint8_t stub4e[26];
	//24
	//2

	//int8_t array_0x7E7_2BDE_13253[28];//array_0x7CF_2BDE_13229[24]
	//int8_t byte_0x7E8_2BDE[26];// -1x26//2024

	int8_t byte_0x803_2BDE[25];// -1x26//2051

	int8_t byte_0x81C_2BDE[26];// -1x26//2076
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
	uint8_t stuba[25];
	uint8_t byte_0x36115;
	//0x36141;//70
	//0x36141[8];??
	uint8_t stubb[109];
}
type_str_0x360FB;*/

typedef struct {//lenght 7
	int8_t byte_0;//25972
	int8_t byte_1;//25973
	uint8_t stub[5];
}
type_str_0x6574;

struct {//lenght 0x33
	int16_t maxTextboxWidth_0x0;//0 //0
	int16_t maxTextboxHeight_0x2;//2 //1
	int16_t textBoxWidth_0x4;//4 //2
	int16_t textboxHeight_0x6;//6 //3
	int16_t textboxPosX_0x8;//8 //4
	int16_t textboxPosY_0xa;//a //5
	int16_t maxTextboxWidth2_0xc;//c-3f //6
	int16_t maxTextboxHeight2_0xe;//e //7
	int16_t charWidth_0x10;//10 //8
	int16_t charHeight_0x12;//12 //9
	int16_t minPosX_0x14;//14 //10
	int16_t maxPosX_0x16;//16 //11
	int16_t minPosY_0x18;//18 //12
	int16_t maxPosY_0x1a;//1a //13
	int16_t lineSrcX_0x1c;//1c-4f //14
	int16_t lineSrcY_0x1e;//1e-51 //15
	int16_t lineDestX_0x20;//20 //16
	int16_t lineDestY_0x22;//22 //17
	int16_t framePosX_0x24;//24-57 //width 18
	int16_t framePosY_0x26;//26-59 //19
	int16_t frameWidth_0x28;//28-5b //20
	int16_t frameHeight_0x2a;//2a-5d//21
	int8_t stub[4];
	int8_t color1_0x30;//30-63 - color1 //24
	int8_t color2_0x31;//31-64 - color2
	int8_t color3_0x32;//32-65 - color2
}
typedef Type_TextBox_1804B0;

typedef union {//lenght 12
	int32_t dword[3];
	int16_t word[6];
}
type_uni_0x8a;

struct {//0xb0 nebo spis 171(0xab)
	Type_TextBox_1804B0 type_sub_0[2];
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
	int16_t Index_0x86;//86
	int16_t word_0x88;//88
	type_uni_0x8a uni_0x8a;
	//int32_t dword_0x8a;//8a
	//int16_t array_0x8e[4];//8e //text lines
	int16_t word_0x96;//96
	int16_t word_0x98;//98
	int16_t word_0x9a;//9A
	int16_t word_0x9c;//9C
	int8_t PopupStatusByte_0x9e;//9e
	int8_t byte_0x9f;//9f
	int8_t byte_0xa0;//a0
	int8_t byte_0xa1;//a1
	int8_t byte_0xa2;//a2
	int8_t drawTextType_0xa3;//a3
	int8_t byte_0xa4;//a4
	int8_t byte_0xa5;//a5
	int8_t byte_0xa6;//a6
	int8_t showPlayerScores_0xa7;//a7
	int8_t byte_0xa8;//a8 - wizard number
	int8_t byte_0xa9;//a9
	int8_t byte_0xaa;//aa
	//int8_t stubb[1];
}
typedef type_str_unk_1804B0ar;
#pragma pack(pop)

/*
	memset((void*)(&x_D41A0_BYTEARRAY_0[16]), 0, 0x1d);
	memset((void*)(&x_D41A0_BYTEARRAY_0[45]), 0, 4);
	memset((void*)(&x_D41A0_BYTEARRAY_0[49]), 0, 2);
	memset((void*)(&x_D41A0_BYTEARRAY_0[51]), 0, 2);
	memset((void*)(&x_D41A0_BYTEARRAY_0[53]), 0, 4);
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

/*void x_D41A0_BYTEARRAY_0_to_x_D41A0_BYTESTR_0();
void x_D41A0_BYTESTR_0_to_x_D41A0_BYTEARRAY_0();*/
void write_bitmap_pos_struct_to_png(uint8_t* buffer, int width, int height, const char* filename);

int my_sign32(int32_t var);
int my_sign16(int16_t var);

#endif //ENGINE_SUPPORT_H
