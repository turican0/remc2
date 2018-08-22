#include <stdio.h>
#include <assert.h>
#include <sstream>
#include <stddef.h>
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */

#include "sdl/graphics_sdl.h"
//#define USE_DOSBOX

#ifdef USE_DOSBOX
	#include "dosbox.h"
	#include "logging.h"
	#include "dos_system.h"
#else
	#include "../dosbox_files/config.h"
#endif //USE_DOSBOX


#ifndef ENGINE_SUPPORT_ACTIVE
#define ENGINE_SUPPORT_ACTIVE

typedef          char   int8;
typedef   signed char   sint8;
typedef unsigned char   uint8;
typedef          short  int16;
typedef   signed short  sint16;
typedef unsigned short  uint16;
typedef          int    int32;
typedef   signed int    sint32;
typedef unsigned int    uint32;

#define x_BYTE int8
#define x_WORD int16
#define x_DWORD int32
#define x_LONG int32

extern Bit8u* readbuffer;
extern char* printbuffer;//char* buffer; // [esp+0h] [ebp-2h]
extern char* printbuffer2;//char v11; // [esp+40h] [ebp+3Eh]
extern Bit8u *x_DWORD_E9C38_smalltit; // weak
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
extern int x_DWORD_D41A0;

//extern Bit8s x_D41A0_BYTEARRAY[];
extern Bit32s* x_D41A0_BYTEARRAY_0;
extern Bit8u* x_D41A0_BYTEARRAY_4;

typedef struct {
	Bit8u harddisk_number;

	//Bit8u setting_byte5_19;
	Bit32u dwordindex_0;
	Bit8u byteindex_4;
	Bit32u dwordindex_6;
	Bit32u dwordindex_8;
	Bit32u dwordindex_12;
	Bit8u setting_byte1_22;
	Bit8u setting_byte2_23;
	Bit8u setting_byte3_24;
	Bit8u setting_byte4_25;
	Bit8u setting_30;
	FILE* moviemvidatfile_byte4_33;
	Bit16u moviemvinumber_byte4_39;
	Bit8u levelnumber_43;
	Bit8u setting_45;
	Bit8u byteindex_179;
	Bit8u harddiskchar_byte4_182;
	Bit8u setting_216;
	Bit8u setting_38545;
	Bit8u setting_38402;

	
}type_x_D41A0_BYTEARRAY_4_struct;

extern type_x_D41A0_BYTEARRAY_4_struct x_D41A0_BYTEARRAY_4_struct;
extern Bit16s x_D41A0_WORDARRAY[];

extern Bit32u x_D41A0_BYTEARRAY_4_0xE6_heapsize;
extern Bit8u* x_D41A0_BYTEARRAY_4_0xE2_heapbuffer;
//extern Bit8u* x_D41A0_BYTEARRAY_4_0xDE_heapbuffer;
//extern Bit32u* off_D918C[0x7c];

extern Bit8u* dword_E9C30[]; // weak


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

extern Bit8u* x_DWORD_180628b;
//extern char* char_355198;
void myWriteOut(const char * format, ...);
int myprintf(const char * format, ...);
void pathfix(char* path, char* path2);
void support_begin();
void support_end();

void loadfromsnapshot(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u size);
void loadfromsnapshot2(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u size);
void compare_with_snapshot(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u size);

#endif //ENGINE_SUPPORT_ACTIVE
