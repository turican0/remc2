#include <stdio.h>
#include <assert.h>
#include <sstream>
#include <stddef.h>
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */

#include "png.h"
#pragma comment(lib, "zlib.lib") // must be before libpng!
#pragma comment(lib, "libpng15.lib") // must be after zlib!

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

extern Bit8u* off_D41A8;
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
	Bit16u z;
}axis_3d;

typedef struct {
	Bit16s aa;
	Bit16s bb;
	Bit16s cc;
	Bit16s dd;
}axis_4d;

typedef struct _str_0x6E8E {//lenght a8//THING
	struct _str_0x6E8E* next_0;
	Bit32s dword_0x4;//4 //?bitmap//LIVE
	Bit32s dword_0x8;//8 // this is Bit32s ?bitmap//MAX LIVE
	tetrabytebyte struct_byte_0xc_12_15;//12
			            //12//struct_byte_0xc_12_15.dbyte1_2.byte1
	//0xfe - all spell
	//Bit8s byte_0xd_13;//13//struct_byte_0xc_12_15.dbyte1_2.byte2
	//Bit8s byte_0xe_14;//14//struct_byte_0xc_12_15.dbyte3_4.byte1
	//Bit8s byte_0xf_15;//15//struct_byte_0xc_12_15.dbyte3_4.byte2
	Bit32s dword_0x10_16;//16 // index of array
	Bit16u word_0x14_20;//20 //random seed
	Bit16u word_0x16_22;//22
	Bit16u word_0x18_24;//24
	Bit16u word_0x1A_26;//26 // index - owner //ID
	Bit16s word_0x1C_28;//28
	Bit16s word_0x1E_30;//30
	Bit16s word_0x20_32;//32
	//Bit8s byte_0x21_33;//33
	Bit16s word_0x22_34;//34
	Bit16s word_0x24_36;//36 // index subentity
	Bit16s word_0x26_38;//38 // index subentity
	Bit16u word_0x28_40;//40//WHO OWNS ME
	Bit16u word_0x2A_42;//42 //maybe Bit16s
	Bit16s word_0x2A_44;//44 //add to z
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
	Bit8s byte_0x3E_62;//62 
	Bit8s byte_0x3F_63;//63 //type//MODEL
	//11 - spell
	Bit8s byte_0x40_64;//64 //subtype//CLASS
	//0 or 1 - player, 2 - castle,3-ballon
	Bit8s byte_0x41_65;//65 //type subentity
	Bit8s byte_0x42_66;//66 //subtype subentity
	Bit8s byte_0x43_67;//67
	Bit8s byte_0x44_68;//68	
	Bit8u byte_0x45_69;//69 // index of structure with lenght 14//STATE
	Bit8u byte_0x46_70;//70 // index of bitmap
	Bit8s byte_0x47_71_xx;//71
	Bit8s byte_0x48_72;//72
	Bit8s byte_0x49_73;//70
	Bit16s word_0x4A_74;
	axis_3d array_0x4C_76;//position//ACTUAL X Y Z
	axis_4d array_0x54_82;
	Bit16s word_0x5A_90;
	Bit8s byte_0x5C_92;
	Bit8s byte_0x5D_93;
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
	Bit16u word_0x82_130;//130 //add to z//ACTUAL SPEED
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
	Bit32u dword_0xA0_160;//160
	//Bit16u word_0xA2_162;//162
	Bit32s dword_0xA4_164;//100 // adress of xx
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
	Bit8u harddisk_number;

	//Bit8u setting_byte5_19;
	Bit32u dwordindex_0;//x_D41A0_BYTEARRAY_4_struct.dwordindex_0
	Bit8u byteindex_4;//x_D41A0_BYTEARRAY_4_struct.byteindex_4
	Bit16u wordindex_6;//x_D41A0_BYTEARRAY_4_struct.wordindex_6
	Bit16u wordindex_8;//x_D41A0_BYTEARRAY_4_struct.wordindex_8
	Bit8u byteindex_10;//0xa//x_D41A0_BYTEARRAY_4_struct.byteindex_10//show help
	Bit8s byte_brightness_11;//0xb//x_D41A0_BYTEARRAY_4_struct.byteindex_11
	Bit8s byte_brightness_12;//0xc//x_D41A0_BYTEARRAY_4_struct.byteindex_12
	Bit8s byte_brightness_13;//0xd//x_D41A0_BYTEARRAY_4_struct.byteindex_13
	Bit32u dwordindex_16;//0x10//x_D41A0_BYTEARRAY_4_struct.dwordindex_16
	Bit8u setting_byte1_22;//x_D41A0_BYTEARRAY_4_struct.setting_byte1_22
	Bit8s setting_byte2_23;//0x17//x_D41A0_BYTEARRAY_4_struct.setting_byte2_23
	Bit8u setting_byte3_24;//cheats? 0x20==free spell//x_D41A0_BYTEARRAY_4_struct.setting_byte3_24
	//spell on - 0x20
	Bit8u setting_byte4_25;//cheats? 0x1==Invincability
	//invincability - 1;
	Bit8u byteindex_26=0;//x_D41A0_BYTEARRAY_4_struct.byteindex_26
	Bit8u setting_30;//x_D41A0_BYTEARRAY_4_struct.setting_30
	FILE* moviemvidatfile_byte4_33;
	Bit8u byteindex_34;//0x22//x_D41A0_BYTEARRAY_4_struct.byteindex_34
	Bit16u moviemvinumber_byte4_39;//x_D41A0_BYTEARRAY_4_struct.moviemvinumber_byte4_39
	Bit8u levelnumber_43;//x_D41A0_BYTEARRAY_4_struct.levelnumber_43
	Bit8u setting_45;
	Bit8s byteindex_50;//0x32//x_D41A0_BYTEARRAY_4_struct.byteindex_50
	Bit8u byteindex_51;//0x33//x_D41A0_BYTEARRAY_4_struct.byteindex_51
	Bit8u byteindex_52;//0x34//x_D41A0_BYTEARRAY_4_struct.byteindex_52
	Bit8u byteindex_53;//0x35//x_D41A0_BYTEARRAY_4_struct.byteindex_53
	char player_name_57ar[32];
	char* savestring_89;
	Bit8u byteindex_121[16];//0x79//x_D41A0_BYTEARRAY_4_struct.byteindex_121
	//Bit8u byteindex_122;//0x7a//x_D41A0_BYTEARRAY_4_struct.byteindex_121[1]
	//Bit8u byteindex_123;//0x7b//x_D41A0_BYTEARRAY_4_struct.byteindex_121[2]
	//Bit8u byteindex_124;//0x7c//x_D41A0_BYTEARRAY_4_struct.byteindex_121[3]
	//Bit8u byteindex_125;//0x7d//x_D41A0_BYTEARRAY_4_struct.byteindex_121[4]
	//Bit8u byteindex_127;//0x7f//x_D41A0_BYTEARRAY_4_struct.byteindex_121[6]

	char byteindex_145ar[32];//x_D41A0_BYTEARRAY_4_struct.byteindex_145ar

	Bit8u byteindex_177;//0xb1//x_D41A0_BYTEARRAY_4_struct.byteindex_177
	Bit8u byteindex_178;//0xb2//x_D41A0_BYTEARRAY_4_struct.byteindex_178
	Bit8u byteindex_179;
	Bit8u byteindex_180;//0xb4//x_D41A0_BYTEARRAY_4_struct.byteindex_180
	Bit8u byteindex_181;//0xb5//x_D41A0_BYTEARRAY_4_struct.byteindex_181
	Bit8u harddiskchar_byte4_182;
	Bit8u byteindex_183;//0xb7//x_D41A0_BYTEARRAY_4_struct.byteindex_183
	Bit8u byteindex_184;//0xb8//x_D41A0_BYTEARRAY_4_struct.byteindex_184
	Bit8u byteindex_186;//0xba//x_D41A0_BYTEARRAY_4_struct.byteindex_186
	Bit32u dwordindex_188;//x_D41A0_BYTEARRAY_4_struct.byteindex_188
	//Bit8u byteindex_189;//x_D41A0_BYTEARRAY_4_struct.byteindex_189
	//Bit8u byteindex_190;//x_D41A0_BYTEARRAY_4_struct.byteindex_190

	Bit32u dwordindex_192;//0xc0//x_D41A0_BYTEARRAY_4_struct.byteindex_192
	//Bit8u byteindex_193;//0xc1//x_D41A0_BYTEARRAY_4_struct.byteindex_193
	//Bit8u byteindex_194;//0xc2//x_D41A0_BYTEARRAY_4_struct.byteindex_194
	
	Bit8u byteindex_196;//0xc4//x_D41A0_BYTEARRAY_4_struct.byteindex_196
	Bit8u transfer_rate_dwordindex_200;//0xc8//x_D41A0_BYTEARRAY_4_struct.transfer_rate_dwordindex_200
	Bit8u byteindex_204;//0xCC//x_D41A0_BYTEARRAY_4_struct.byteindex_204
	Bit8u byteindex_205;//0xCD//x_D41A0_BYTEARRAY_4_struct.byteindex_205
	Bit8u byteindex_206;//0xce//x_D41A0_BYTEARRAY_4_struct.byteindex_206
	Bit8u byteindex_207;//0xcf//x_D41A0_BYTEARRAY_4_struct.byteindex_207
	Bit8u byteindex_208;//0xd0//x_D41A0_BYTEARRAY_4_struct.byteindex_208
	Bit8u byteindex_210;//0xd2//x_D41A0_BYTEARRAY_4_struct.byteindex_210
	Bit8u byteindex_214;//0xd6//x_D41A0_BYTEARRAY_4_struct.byteindex_214
	Bit8u setting_216;
	Bit8u byteindex_218;//0xda//x_D41A0_BYTEARRAY_4_struct.byteindex_218

	
	Bit8u byteindex_220;//0xdc//x_D41A0_BYTEARRAY_4_struct.byteindex_220
	Bit8u byteindex_221ar[3];//0xe0//x_D41A0_BYTEARRAY_4_struct.byteindex_221ar
	Bit8u byteindex_224;//0xe0//x_D41A0_BYTEARRAY_4_struct.byteindex_224
	Bit8u byteindex_225;//0xe1//x_D41A0_BYTEARRAY_4_struct.byteindex_225
	Bit8u byteindex_242ar[14];//0xf2//x_D41A0_BYTEARRAY_4_struct.byteindex_242ar
	//Bit8u byteindex_246;//0xf6//x_D41A0_BYTEARRAY_4_struct.byteindex_242ar[4]
	//Bit8u byteindex_250;//x_D41A0_BYTEARRAY_4_struct.byteindex_242ar[8]
	
	Bit8u byteindex_256ar[2124];//0x100//x_D41A0_BYTEARRAY_4_struct.byteindex_256ar	
	//280=byteindex_256ar[24];//x_D41A0_BYTEARRAY_4_struct.byteindex_256ar[24]
	//565=byteindex_256ar[309];//0x235//x_D41A0_BYTEARRAY_4_struct.byteindex_256ar[309]
	//1865=byteindex_256ar[1609];//0x749//x_D41A0_BYTEARRAY_4_struct.byteindex_256ar[1609]
	//end 2224

	Bit8u byteindex_2255ar[26];//0x100//x_D41A0_BYTEARRAY_4_struct.byteindex_2255ar
	//end 2281

	Bit32u dwordindex_2380;//0x94c//x_D41A0_BYTEARRAY_4_struct.dwordindex_2380

	Bit32u dwordindex_2388;//0x954//x_D41A0_BYTEARRAY_4_struct.dwordindex_2388
	Bit32u dwordindex_2392;//0x958//x_D41A0_BYTEARRAY_4_struct.dwordindex_2392

	//Bit8u byteindex_8618;//0x21aa//x_D41A0_BYTEARRAY_4_struct.byteindex_8618
	//Bit8u setting_38545;

	type_str_0x6E8E* dwordindex_38396;//0x95FC//x_D41A0_BYTEARRAY_4_struct.dwordindex_38396
	Bit8u byteindex_38400;//0x9600//x_D41A0_BYTEARRAY_4_struct.byteindex_38400
	Bit8u byteindex_38401;//0x9601//x_D41A0_BYTEARRAY_4_struct.byteindex_38401

	Bit8u setting_38402;//0x9602//x_D41A0_BYTEARRAY_4_struct.setting_38402

	type_str_0x6E8E* bytearray_38403x[29];//array 116//0x9603//x_D41A0_BYTEARRAY_4_struct.bytearray_38403	
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
	type_str_0x6E8E* dword_38527;//0x967F//x_D41A0_BYTEARRAY_4_struct.dword_38527
	type_str_0x6E8E* dword_38523;//0x967b//x_D41A0_BYTEARRAY_4_struct.dword_38523
	type_str_0x6E8E* dword_38531;//0x9683//x_D41A0_BYTEARRAY_4_struct.dword_38531
	type_str_0x6E8E* dword_38535;//x_D41A0_BYTEARRAY_4_struct.dword_38535
	Bit8u byte_38544;//x_D41A0_BYTEARRAY_4_struct.byte_38544
	Bit8u setting_38545;//0x9691//x_D41A0_BYTEARRAY_4_struct.setting_38545
	Bit8u byte_38546;//0x9692//x_D41A0_BYTEARRAY_4_struct.byte_38546
	Bit8u byte_38591;//0x96BF//x_D41A0_BYTEARRAY_4_struct.byte_38591

	Bit32u dword_616432;


}type_x_D41A0_BYTEARRAY_4_struct;



doublebyte doublebyte_conv(Bit16u a2);

extern type_x_D41A0_BYTEARRAY_4_struct x_D41A0_BYTEARRAY_4_struct;
extern Bit16s x_D41A0_WORDARRAY[];

extern Bit32u x_D41A0_BYTEARRAY_4_0xE6_heapsize;
extern Bit8u* x_D41A0_BYTEARRAY_4_0xE2_heapbuffer;
//extern Bit8u* x_D41A0_BYTEARRAY_4_0xDE_heapbuffer;
//extern Bit32u* off_D918C[0x7c];

//xx extern Bit8u* dword_E9C30[]; // weak

extern posistruct* xy_DWORD_17DED4_spritestr;
extern posistruct* xy_DWORD_17DEC0_spritestr;
extern posistruct* xy_DWORD_17DEC8_spritestr;

extern posistruct* x_DWORD_D4188t_spritestr;


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

extern Bit8u* x_DWORD_180628b_screen_buffer;

//extern char* char_355198;
//void pathfix(char* path, char* path2);
void support_begin();
void support_end();

void loadfromsnapshot(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u size);
void loadfromsnapshot2(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u size);
Bit32u compare_with_snapshot(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u size, Bit8u* origbyte, Bit8u* copybyte);
Bit32u compare_with_sequence(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u count, Bit32u size, Bit8u* origbyte, Bit8u* copybyte);
Bit32u compare_with_snapshot_D41A0(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u size, Bit8u* origbyte, Bit8u* copybyte);
Bit32u compare_with_sequence_D41A0(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u count, Bit32u size, Bit8u* origbyte, Bit8u* copybyte);
Bit32u compare_with_sequence_array_E2A74(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u count, Bit32u size, Bit8u* origbyte, Bit8u* copybyte, int* posdiff);
Bit32u compare_with_sequence_x_DWORD_F2C20ar(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u count, Bit32u size, Bit8u* origbyte, Bit8u* copybyte, int* posdiff);
Bit32u compare_with_sequence_array_222BD3(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u count, Bit32u size, Bit8u* origbyte, Bit8u* copybyte, int* posdiff);

void writehex(Bit8u* buffer,Bit32u count);

void mine_texts(char* filename, Bit32u adressdos, Bit32u count, char* outfilename);
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
typedef struct {//lenght 8
	//Bit8u byte_0;//array_0x36479
	Bit8u byte_0;//array_0x3647a
	Bit8u byte_1;//array_0x3647b
	Bit16s word_2;//array_0x3647c
	Bit16s word_4;//array_0x3647e
	Bit16s word_6;//array_0x36480

} type_str_0x3647A;

typedef struct {//lenght 20
	Bit8u byte_0;//array_0x3030E
	Bit8u byte_1;//array_0x3650F
	Bit8u byte_2;//array_0x36510
	Bit8u byte_3;//array_0x36511
	Bit8u byte_4;//array_0x36512
	Bit16s word_5;//array_0x36513
	Bit8u stub[13];//array_0x36515	
} type_str_0x3030E;

typedef struct {//lenght 20
	Bit8u byte_0;//array_0x3030E
	Bit8u byte_1;//array_0x3650F
	Bit8u byte_2;//array_0x36510
	Bit8u byte_3;//array_0x36511
	Bit8u byte_4;//array_0x36512
	Bit16s word_5;//array_0x36513
	Bit8u stub[13];//array_0x36515	
} type_str_0x30325;

typedef struct {//begin 0x649 //lenght min 0x1ee
	Bit32s dword_0x649_2BDE[0x33];//1609
	Bit8u stub1[2];
	Bit16s word_0x717_2BDE[0x1a];// -2xa1 tj 2x 26//1815	
	//1817
	Bit8u stub4c[54];
	Bit8s byte_0x781_2BDE[0x1a];//1921
	Bit8u stub4d[0x1a];
	Bit8s byte_0x7B5_2BDE[0x1a];//1973
	Bit8u array_0x7CF_2BDE_13229[0x34];//1999
	//Bit8s array_0x7E7_2BDE_13253[0x1c];//array_0x7CF_2BDE_13229[0x18]
	//Bit8s byte_0x7E8_2BDE[0x1a];// -1x26//2024

	Bit8s byte_0x803_2BDE[0x19];// -1x26//2051

	Bit8s byte_0x81C_2BDE[0x1a];// -1x26//2076
	Bit8u stub2[1];
} type_struct_0x649_2BDE_12839;

typedef struct { // 479 // size 14 tj 0xe
	Bit32s dword_0x1d1_2BDE_11695; //465 //0
	Bit32s dword_0x1d5_2BDE_11699; //469 //4
	Bit32s dword_0x1d9_2BDE_11703; //473 //8
	Bit16s word_0x1dd_2BDE_11707; //477 //12
	/*
	Bit32s dword_0x1df_2BDE_11709; //479
	Bit32s dword_0x1e3_2BDE_11713; //483
	Bit32s dword_0x1e7_2BDE_11717; //487
	Bit16s word_0x1eb_2BDE_11721; //491*/
} type_struct_0x1d1_2BDE_11695;

typedef struct {//lenght 2124=0x84C
	Bit8u byte_0x000_2BDE_11230;//0	//11230 - byte? - ne zacatek struktury
	Bit8u byte_0x001_2BDF_11231_xx;
	Bit8u byte_0x002_2BE0_11232;//2	//11232 - byte //2
	Bit8u byte_0x003_2BE0_11233_xx;
	Bit8u byte_0x004_2BE0_11234;//2	//11234 - byte //4
	Bit8u byte_0x005_2BE0_11235_xx;
	Bit8u byte_0x006_2BE4_11236;//6	//11236 - byte //6
	Bit16s word_0x007_2BE4_11237;
	Bit8s byte_0x009_2BE4_11239;
	Bit16u word_0x00a_2BE4_11240;//10 //11240 - word - index z EA3E4 //0xa
	Bit8u byte_0x00c_2BE0_11242_xx;
	Bit8u byte_0x00d_2BE0_11243_xx;
	Bit16s word_0x00e_2BDE_11244; //14
	Bit16s word_0x010_2BDE_11246; //16	
	Bit8u byte_0x012_2BE0_11248_xx;
	Bit8u byte_0x013_2BE0_11249_xx;
	Bit8u byte_0x014_2BE0_11250_xx;
	Bit8u byte_0x015_2BE0_11251_xx;
	Bit8u byte_0x016_2BE0_11252_xx;
	Bit8u byte_0x017_2BE0_11253_xx;	
	Bit32s dword_0x018_2BDE_11254; //24
	Bit8s array_0x01c_2BFA_11258[49];//28//11258 - byte* jmeno2?
	Bit16u word_0x04d_2C2B_11307;//77 //11307 - word
	Bit16u word_0x04f_2C2D_11309;//79 //11309 - word
	Bit8u stub2[384];

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
	
	Bit8s array_0x39f_2BFA_12157[0x40];//927//12157 - byte(11230+927) 100% name
	Bit8u byte_0x3DF_2BE4_12221;//991//12221 - byte
	Bit8u byte_0x3E0_2BE4_12222_xx;
	Bit8u byte_0x3E1_2BE4_12223;//993//12223 - byte - ok
	Bit8u byte_0x3E2_2BE4_12224;//994//12224 - byte
	Bit8u stub3[61];
	Bit8u byte_0x420_2BE4_12286;//1056//12286 - byte
	Bit8u stub4[389];
	Bit8s byte_0x5a6_2BE4_12676;//1446
	Bit8u stub4b[162];
	type_struct_0x649_2BDE_12839 struct_0x649_2BDE_12839;//min 0x1ee
	/*Bit8u stub4b[228];
	Bit8u array_0x7CF_2BDE_13229[24];
	Bit16s word_0x7E7_2BDE_13253;*/
	
	Bit16s word_0x837_2BDE;//2103
	Bit16s word_0x839_2BDE;//2105

	Bit8u stub5[17];
} type_str_0x2BDE;//11230 // 84c lenght

typedef struct {//lenght 8
	Bit8s str_0x65ac_byte_0;//0x65ac
	Bit8s str_0x65ac_byte_1;//0x65ad
	Bit8s str_0x65ac_byte_2;//0x65ae
	Bit8s str_0x65ac_byte_3;//0x65af
	Bit16s str_0x65ac_word_4;//0x65b0
	Bit16s str_0x65ac_word_6;//0x65b2	
}
type_str_0x65ac;

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


typedef struct {//lenght 0x6604u
		//podpolozky
	Bit8u byte_0x2FED4;//x_D41A0_BYTEARRAY_0[196308]
	//0x30331 - end of structure lenght 20
	/*197413 - 0x30324*/
	/*221393 - 0x360D1*/

	type_str_0x3030E array_0x3030E[0xb];//fix array lenght;
	type_str_0x30325 array_0x30325[0x4af];
	////0x30325 0x360d1 tj rozdil 23980=1199*20
	//36479
	type_str_0x3647A array_0x3647A[0xb];//8x11	
	//konec 6604
} type_str_2FECE;//compress level 
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



typedef struct{//lenght 0x36e16	
	Bit8u stub0[4];
	Bit32s dword_0x4;
	Bit32s dword_0x8;

	Bit16s word_0xc;//player_index?
	Bit16s word_0xe;
	//word 0xe - 14
	Bit8u array_0x10[0x12];
	Bit8u byte_0x22;
	Bit8u stub1[0x12];
	Bit32s dword_0x35;
	Bit8u stub2[0x1fc];
	Bit32s dword_0x235;//act music
	Bit8u stub2b[0x1];
	Bit32s dword_0x23a;
	Bit32s dword_0x23e;
	Bit32s dword_0x242;
	type_str_0x6E8E* pointers_0x246[0x3e8];//pointers
	/*
	*(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 570) = 1;//0x23a
	*(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 574) = 0;//0x23e
	*(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 578) = 0;//0x242
	*/
	//Bit8u stub3[0x1f44];
	Bit32s dword_0x11e6;
	Bit8u stub3[0xfa0];

	Bit8u byte_0x218A;//8586 reflections	
	Bit8u byte_0x218B;//8587 shadows
	Bit8u byte_0x218C;//8588 sky
	Bit8u byte_0x218D;//8589 alter screen size
	Bit8u byte_0x218E;//8590 lights
	Bit8u byte_0x218F;//8591 icons
	Bit8u byte_0x2190;//8592 screen size?
	Bit8u byte_0x2191;//8593
	Bit8u byte_0x2192;//8594
	Bit8u byte_0x2193;//8595
	Bit8u byte_0x2194;//8596 resolution
	Bit8u byte_0x2195;//8597 shadows
	Bit8u byte_0x2196;//8598 lights
	Bit8u byte_0x2197;//setting
	Bit8u byte_0x2198;//8600 transparency
	Bit8u byte_0x2199;//8601 flat

	Bit32s dword_0x219A;//8602
	Bit32s dword_0x219E;//8606
	Bit32s dword_0x21A2;//8610
	Bit32s dword_0x21A6;//8614

	Bit8u byte_0x21AA;//8618 can reflections
	Bit8u byte_0x21AB;//8619 can shadows
	Bit8u byte_0x21AC;//8620 can sky
	Bit8u byte_0x21AD;
	Bit8u byte_0x21AE;
	Bit8u byte_0x21AF;
	Bit8u byte_0x21B0;
	Bit8u byte_0x21B1;
	Bit8u byte_0x21B2;
	Bit8u byte_0x21B3;
	Bit8u byte_0x21B4;
	Bit8u byte_0x21B5;
	Bit8u byte_0x21B6;//8630 can lights
	Bit8u byte_0x21B7;//8631
	Bit8u byte_0x21B8;//8632 can transparency
	Bit8u byte_0x21B9;//8633 can flat	

	Bit8u stub3b[0x1a8];

	axis_3d array_0x2362[8];//0x30
	//Bit32s dword_0x2362;
	//Bit16s word_0x2366;
	Bit8u stub3c[0x14d];
	///*??*/type_str_0x24DF array_0x24DF[0xb];//?lenght
	Bit8u stub3d[0x6ff];



	//x_D41A0_BYTEARRAY_0[2124 * D41A0_BYTESTR_0.word_12 + 11234])
	//x_D41A0_BYTEARRAY_0[0x84C * D41A0_BYTESTR_0.word_12 + 0x2BE2])
	type_str_0x2BDE array_0x2BDE[0xb]; //0x84c*0xb ??

	type_str_0x65ac array_0x65ac[0xb];//??

	Bit8s array_0x6E3E[8][0xa];//28222	lenght 0xa size 0x8// game events
	type_str_0x6E8E struct_0x6E8E[0x3e8];//28302
	Bit8u stub3ee[0xa4];
	Bit8u* pointer_0x6F32;

	Bit8u stub3e[0x28f98];

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

	//array 0x364D6 lenght 0x68 end 0x3653e
	Bit8u stub3f[0x831];
	Bit16s word_0x365E0;
	Bit8u stub3g[0x12];
	

	type_str_0x65ac array_0x365F4[0xb];//8x11	//set 0x58 // end 0x3664c

	//Bit8s byte_0x365FC;//array_0x365F4[1].byte_0
	//Bit8s byte_0x365FD;//array_0x365F4[1].byte_1
	

	//array 0x3664C lenght 0x79e (end-0x36dea)
	Bit8u stub3h[0x79e];
	Bit8s byte_0x36DEA_fly_asistant;
	Bit8s byte_0x36DEB_xx;
	Bit16s word_0x36DEC_mousex;
	Bit16s word_0x36DEE_mousey;
	Bit16s word_0x36DF0_mousexx;
	Bit32s dword_0x36DF2;//index
	Bit8u stub3i[0xa];
	Bit8s byte_0x36E00;
	Bit8u stub3j[0xa];
	Bit8u byte_0x36E0B;
	
	
	
	//&x_D41A0_BYTEARRAY_0[197413]; ix < &x_D41A0_BYTEARRAY_0[221393]; ix += 20)

	//type_str_0x365F2 array_str_0x365F2[0x10];//fix it
	//*(0x365F2 + x_D41A0_BYTEARRAY_0 + 8 * (signed __int16)result + 2)

	Bit8u stubend[0xb];

} type_D41A0_BYTESTR_0;
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
axis_3d get_x_D41A0_BYTEARRAY_0_0x2362(int number); //lenght 48 - 6x8
void set_x_D41A0_BYTEARRAY_0_0x2362(int number, axis_3d value);
/*Bit16s get_x_D41A0_BYTEARRAY_0_0x2366(int number);
void set_x_D41A0_BYTEARRAY_0_0x2366(int number, Bit16s value);*/

void clean_x_D41A0_BYTEARRAY_0_0x2362();


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
#endif //ENGINE_SUPPORT_ACTIVE
