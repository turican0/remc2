#include <stdio.h>
#include <assert.h>
#include <sstream>
#include <stddef.h>
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */

#include"../portability/port_time.h"

#include "../portability/port_sdl_vga_mouse.h"
#include "../portability/mctypes.h"
//#define USE_DOSBOX




#ifndef ENGINE_SUPPORT_ACTIVE
#define ENGINE_SUPPORT_ACTIVE

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
extern Bit8u* x_D41A0_BYTEARRAY_0;
//extern Bit8u* x_D41A0_BYTEARRAY_4;

extern Bit8u* off_D41A8;
extern Bit8u* x_BYTE_14B4E0;

typedef struct {
	Bit8u harddisk_number;

	//Bit8u setting_byte5_19;
	Bit32u dwordindex_0;//x_D41A0_BYTEARRAY_4_struct.dwordindex_0
	Bit8u byteindex_4;//x_D41A0_BYTEARRAY_4_struct.byteindex_4
	Bit16u wordindex_6;//x_D41A0_BYTEARRAY_4_struct.wordindex_6
	Bit16u wordindex_8;//x_D41A0_BYTEARRAY_4_struct.wordindex_8
	Bit8u byteindex_10;//0xa//x_D41A0_BYTEARRAY_4_struct.byteindex_10
	Bit8u byteindex_11;//0xb//x_D41A0_BYTEARRAY_4_struct.byteindex_11
	Bit8u byteindex_12;//0xc//x_D41A0_BYTEARRAY_4_struct.byteindex_12
	Bit8u byteindex_13;//0xd//x_D41A0_BYTEARRAY_4_struct.byteindex_13
	Bit8u setting_byte1_22;//x_D41A0_BYTEARRAY_4_struct.setting_byte1_22
	Bit8u setting_byte2_23;//0x17//x_D41A0_BYTEARRAY_4_struct.setting_byte2_23
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
	Bit8u byteindex_50;//0x32//x_D41A0_BYTEARRAY_4_struct.byteindex_50
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
	Bit8u byteindex_188;//x_D41A0_BYTEARRAY_4_struct.byteindex_188
	Bit8u byteindex_189;//x_D41A0_BYTEARRAY_4_struct.byteindex_189
	Bit8u byteindex_190;//x_D41A0_BYTEARRAY_4_struct.byteindex_190
	Bit8u byteindex_192;//0xc0//x_D41A0_BYTEARRAY_4_struct.byteindex_192
	Bit8u byteindex_193;//0xc1//x_D41A0_BYTEARRAY_4_struct.byteindex_193
	Bit8u byteindex_194;//0xc2//x_D41A0_BYTEARRAY_4_struct.byteindex_194
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

	Bit8u byteindex_8618;//0x21aa//x_D41A0_BYTEARRAY_4_struct.byteindex_8618
	//Bit8u setting_38545;

	Bit8u dwordindex_38396;//0x95FC//x_D41A0_BYTEARRAY_4_struct.dwordindex_38396
	Bit8u byteindex_38400;//0x9600//x_D41A0_BYTEARRAY_4_struct.byteindex_38400
	Bit8u byteindex_38401;//0x9601//x_D41A0_BYTEARRAY_4_struct.byteindex_38401

	Bit8u setting_38402;//0x9602//x_D41A0_BYTEARRAY_4_struct.setting_38402

	Bit8u bytearray_38403[116];//array 116//0x9603//x_D41A0_BYTEARRAY_4_struct.bytearray_38403	
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
	Bit32u dword_38519;//0x9677//x_D41A0_BYTEARRAY_4_struct.dword_38519
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
	Bit32u dword_38527;//0x967F//x_D41A0_BYTEARRAY_4_struct.dword_38527
	Bit32u dword_38523;//0x967b//x_D41A0_BYTEARRAY_4_struct.dword_38523
	Bit32u dword_38531;//0x9683//x_D41A0_BYTEARRAY_4_struct.dword_38531
	Bit32u dword_38535;//x_D41A0_BYTEARRAY_4_struct.dword_38535
	Bit8u byte_38544;//x_D41A0_BYTEARRAY_4_struct.byte_38544
	Bit8u setting_38545;//0x9691//x_D41A0_BYTEARRAY_4_struct.setting_38545
	Bit8u byte_38546;//0x9692//x_D41A0_BYTEARRAY_4_struct.byte_38546
	Bit8u byte_38591;//0x96BF//x_D41A0_BYTEARRAY_4_struct.byte_38591

	Bit32u dword_616432;


}type_x_D41A0_BYTEARRAY_4_struct;

typedef struct {
	Bit8u byte1;
	Bit8u byte2;
}doublebyte;

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

void writehex(Bit8u* buffer,Bit32u count);

void mine_texts(char* filename, Bit32u adressdos, Bit32u count, char* outfilename);

#endif //ENGINE_SUPPORT_ACTIVE
