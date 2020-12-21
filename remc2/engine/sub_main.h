/*#include <stdio.h>
#include <assert.h>
#include <sstream>
#include <stddef.h>

#include "dosbox.h"
#include "engine.h"
#include "mem.h"

#include "cpu.h"
#include "memory.h"
#include "debug.h"
#include "mapper.h"
#include "setup.h"
#include "programs.h"
#include "paging.h"
#include "callback.h"
#include "cpu/lazyflags.h"
#include "support.h"
#include "control.h"

//#include "engine.h"

#include "inout.h"
#include "callback.h"
#include "pic.h"
#include "fpu.h"
#include "mmx.h"

#include "debug.h"

*/

//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <direct.h>  
//#include <dir.h>
#include <io.h>  

//#include <vld.h>//only for debug

#include "../portability/port_outputs.h"

//#include <sys/statvfs.h>
#include <windows.h>
#ifdef _MSC_VER
#include "../portability/dirent-x.h"
#else
#include "dirent.h"
#endif
#include "../portability/port_time.h"
#include "../portability/port_filesystem.h"
#include "defs.h"
#define __CFSHL__(x, y) (x<<y)
//#define __CFSHL__(x, y) 1
/*Bit32s __CFSHL__(Bit32u x, Bit32u y) {
	return 1;
}*/
//myprintf("invalid_operation:__CFSHL__(x, y)")
#define __RCR__(x, y) printf("invalid_operation:__RCR__(x, y)")
#define __RCL__(x, y) printf("invalid_operation:__RCL__(x, y)")
#define __ROL4__(x, y) __ROL__(x, y)       // Rotate left
#define __ROR4__(x, y) __ROR__(x, y)       // Rotate left

#include "sub_main_mouse.h"

#include "ail_sound.h"

/* This file has been generated by the Hex-Rays decompiler.
Copyright (c) 2007-2017 Hex-Rays <info@hex-rays.com>

Detected compiler: Watcom C++
*/


/*int sub_main(int argc, char **argv, char **envp);
int sub_97F90(x_DWORD *a1);
int sub_980D0(x_DWORD *a1, int a2);
int sub_98170(x_DWORD *a1, int a2, int a3);
bool sub_9EDD0();
int sub_A158B(int a1, __int16 a2, x_WORD *a3, x_WORD *a4);
bool sub_A4CB0();
int sub_A4F10(int a1, char a2, char a3, char a4);
x_DWORD * sub_A5850(int a1, char a2, unsigned int a3, signed int a4, int a5);*/

#ifndef SUB_MAIN_ACTIVE
#define SUB_MAIN_ACTIVE


#define O_RDONLY        0x00    /* read only */
#define O_WRONLY        0x01    /* write only */
#define O_RDWR          0x02    /* read and write */
#define O_BINARY        0x10    /* file contains - binary data */
#define O_TEXT          0x20    /* - text (\n terminated records) */
#define O_CREAT         0x200   /* replace, or create t */
#define O_TRUNC         0x400   /* open with truncation */


#include "engine_support.h"

#include "read_config.h"

#ifdef _MSC_VER

#include <stdlib.h>
#define bswap_16(x) _byteswap_ushort(x)
#define bswap_32(x) _byteswap_ulong(x)
#define bswap_64(x) _byteswap_uint64(x)

#elif defined(__APPLE__)

// Mac OS X / Darwin features
#include <libkern/OSByteOrder.h>
#define bswap_16(x) OSSwapInt16(x)
#define bswap_32(x) OSSwapInt32(x)
#define bswap_64(x) OSSwapInt64(x)

#elif defined(__sun) || defined(sun)

#include <sys/byteorder.h>
#define bswap_16(x) BSWAP_16(x)
#define bswap_32(x) BSWAP_32(x)
#define bswap_64(x) BSWAP_64(x)

#elif defined(__FreeBSD__)

#include <sys/endian.h>
#define bswap_16(x) bswap16(x)
#define bswap_32(x) bswap32(x)
#define bswap_64(x) bswap64(x)

#elif defined(__OpenBSD__)

#include <sys/types.h>
#define bswap_16(x) swap16(x)
#define bswap_32(x) swap32(x)
#define bswap_64(x) swap64(x)

#elif defined(__NetBSD__)

#include <sys/types.h>
#include <machine/bswap.h>
#if defined(__BSWAP_RENAME) && !defined(__bswap_32)
#define bswap_16(x) bswap16(x)
#define bswap_32(x) bswap32(x)
#define bswap_64(x) bswap64(x)
#endif

#else

#define bswap_16(x) ((Bit16u) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8)))
#define bswap_32(x)					\
  ((((x) & 0xff000000u) >> 24) | (((x) & 0x00ff0000u) >> 8)	\
   | (((x) & 0x0000ff00u) << 8) | (((x) & 0x000000ffu) << 24))
#define bswap_64(x)			\
  ((((x) & 0xff00000000000000ull) >> 56)	\
   | (((x) & 0x00ff000000000000ull) >> 40)	\
   | (((x) & 0x0000ff0000000000ull) >> 24)	\
   | (((x) & 0x000000ff00000000ull) >> 8)	\
   | (((x) & 0x00000000ff000000ull) << 8)	\
   | (((x) & 0x0000000000ff0000ull) << 24)	\
   | (((x) & 0x000000000000ff00ull) << 40)	\
   | (((x) & 0x00000000000000ffull) << 56))


#endif

#ifndef _countof
#define _countof(arr) sizeof(arr) / sizeof(arr[0])
//dirty
#endif

//#if !defined(_M_I86) && !defined(__WINDOWS_386__)

struct SREGS {
    unsigned short es;
    unsigned short ds;
    unsigned short fs;
    unsigned short gs;
    unsigned short cs;
    unsigned short ss;
};

/* dword registers */

struct DWORDREGS {
    unsigned int eax;
    unsigned int ebx;
    unsigned int ecx;
    unsigned int edx;
    unsigned int esi;
    unsigned int edi;
    unsigned int cflag;
};

//#endif
/*
// word registers 

struct WORDREGS {
    unsigned short ax;  //__FILLER(_1)
        unsigned short bx;  //__FILLER(_2)
        unsigned short cx;  //__FILLER(_3)
        unsigned short dx;  //__FILLER(_4)
        unsigned short si;  //__FILLER(_5)
        unsigned short di;  //__FILLER(_6)
#if defined(__WINDOWS_386__)
        unsigned short cflag;
#else
        unsigned int cflag;
#endif
};

// byte registers 

struct BYTEREGS {
    unsigned char al, ah;  //__FILLER(_1)
        unsigned char bl, bh;  //__FILLER(_2)
        unsigned char cl, ch;  //__FILLER(_3)
        unsigned char dl, dh;  //__FILLER(_4)
};

union REGS {
#if defined(_M_I86) || defined(__WINDOWS_386__)
    struct WORDREGS  x;
#else
    struct DWORDREGS x;
#endif
    struct WORDREGS  w;
    struct BYTEREGS  h;
};
*/
//#define REGS x_DWORD[6]

struct REGS {
    uint32 eax;
    uint32 ebx;
    uint32 ecx;
    uint32 edx;
    uint32 esi;
    uint32 edi;
    uint32 cflag;
};

/*extern x_WORD __CS__;
extern x_WORD __GS__;
extern x_WORD __DS__;
extern x_WORD __ES__;
extern x_WORD __FS__;
extern x_WORD __SS__;*/

extern char IsTable[];

#pragma pack (1)
typedef struct {
	Bit32s dword_0;
	Bit16s word_4;
	Bit16s word_6;
	Bit16s word_8;//act sprite
	Bit8s byte_10;//first sprite
	Bit8s byte_11;//last sprite
}type_str_unk_E1748;

typedef struct {//lenght 30
	Bit32s dword_0;
	Bit32s dword_4;
	Bit16s word_8;
	Bit16s word_10;
	Bit16s word_12;
	Bit16s word_14;
	Bit16s word_16;
	Bit16s word_18;
	Bit8s byte_20;
	Bit8s byte_21;
	Bit16s word_22;
	Bit16s word_24;
	Bit16s word_26;
	//Bit8s stuba[1];
	Bit16s word_28;
	//Bit8s stubb[1];
} type_x_BYTE_E25ED_db_str;
#pragma pack (16)

extern int filearrayindex_POINTERSDATTAB;
extern int filearrayindex_FONTS0DATTAB;
extern int filearrayindex_MSPRDDATTAB;
extern int filearrayindex_ZERO1;
extern int filearrayindex_MSPRD00DATTAB;
extern int filearrayindex_ZERO2;
extern int filearrayindex_HFONT3DATTAB;
extern int filearrayindex_ZERO3;
extern int filearrayindex_BUILD00DATTAB;
extern int filearrayindex_ZERO4;

#pragma pack (1)
typedef struct {
	Bit8u** begin_buffer;
	Bit8u** end_buffer;
	Bit8u** dat_buffer;
	posistruct** posistruct;
} filearray_struct;
#pragma pack (16)

extern filearray_struct filearray_2aa18c[];
extern filearray_struct x_DWORD_E9B20[4];

extern __int16 x_WORD_E29DA_type_resolution; // weak
extern __int16 x_WORD_180660_VGA_type_resolution; // weak

#pragma pack (1)
typedef struct//lenght 613 // end 17E09D
{
	Bit8u* x_DWORD_17DE38; // weak
	Bit8u* x_DWORD_17DE3C; // weak//1
	Bit8u* x_DWORD_17DE40; // weak//2
	Bit8u* x_DWORD_17DE44; // weak//3
	Bit8u* x_DWORD_17DE48c; // weak//4
	Bit8u* x_DWORD_17DE4C; // weak
	char* x_DWORD_17DE50; // weak
	Bit8u* x_DWORD_17DE54; // weak
	Bit8u* x_DWORD_17DE58; // weak
	Bit8u* x_DWORD_17DE5C_border_bitmap; // weak
	Bit8u* x_DWORD_17DE60; // weak
	Bit8u* x_DWORD_17DE64_game_world_map; // weak
	Bit8u x_BYTE_17DE68x[88]; // fix it -  weak
	//char x_BYTE_17DE69[1]; //x_BYTE_17DE68x[0x1] fix it -  weak
	//char x_BYTE_17DE71[1]; //x_BYTE_17DE68x[0x9] fix it -  weak
	//char x_BYTE_17DE72[78]; //x_BYTE_17DE68x[0xa] idb
	Bit8u* x_DWORD_17DEC0; // weak
	Bit8u* x_DWORD_17DEC4; // weak
	Bit8u* x_DWORD_17DEC8; // weak
	Bit8u* x_DWORD_17DECC; // weak
	int x_DWORD_17DED0; // weak
	Bit8u* x_DWORD_17DED4; // weak
	Bit8u* x_DWORD_17DED8; // weak
	int x_DWORD_17DEDC; // weak
	FILE* x_DWORD_17DEE0_filedesc; // weak
	Bit16s x_DWORD_17DEE4_mouse_positionx; // weak
	char stub0[2];
	Bit16s x_DWORD_17DEE8_mouse_positiony; // weak
	char stub1[2];
	__int16 x_WORD_17DEEC; // weak
	__int16 x_WORD_17DEEE_mouse_buttons; // weak
	__int16 x_WORD_17DEF0; // weak
	__int16 x_WORD_17DEF2; // weak
	__int16 x_WORD_17DEF4; // weak
	__int16 x_WORD_17DEF6; // weak
	__int16 x_WORD_17DEF8; // weak
	__int16 x_WORD_17DEFA; // weak
	__int16 x_WORD_17DEFC; // weak
	__int16 x_WORD_17DEFE; // weak
	__int16 x_WORD_17DF00; // weak
	__int16 x_WORD_17DF02; // weak
	__int16 x_WORD_17DF04; // weak
	__int16 x_WORD_17DF06; // weak
	__int16 x_WORD_17DF08; // weak
	__int16 x_WORD_17DF0A; // weak
	__int16 x_WORD_17DF0C; // weak
	__int16 x_WORD_17DF0E; // weak
	char x_BYTE_17DF10_get_key_scancode; // weak
	char x_BYTE_17DF11_last_key_status; // weak
	char stuba[1];
	char x_BYTE_17DF13; // weak
	char x_BYTE_17DF14[344]; // idb// 34*8
	//char x_BYTE_17DF3C[400]; // fix it -  weak//&x_BYTE_17DF14[40]
	//char x_BYTE_17DF3D[400]; // fix it -  weak//&x_BYTE_17DF14[41]
	//char x_BYTE_17DF3E[400]; // idb//&x_BYTE_17DF14[42]
	__int16 x_WORD_17E06C; // weak
	__int16 x_WORD_17E06E; // weak
	__int16 x_WORD_17E070; // weak
	__int16 x_WORD_17E072; // weak
	__int16 x_WORD_17E074; // weak
	__int16 x_WORD_17E076; // weak
	Bit8s unk_17E078x[0xc]; // weak [11]
	//__int16 x_WORD_17E07C; // weak unk_17E078x[4]
	//__int16 x_WORD_17E07E; // weak unk_17E078x[6]
	//__int16 x_WORD_17E080; // weak unk_17E078x[8]
	//char x_BYTE_17E082; // weak unk_17E078x[0xa]
	//char x_BYTE_17E083; // weak unk_17E078x[0xb]
	Bit16s unk_17E084x[0xc]; // weak [11]
	char x_BYTE_17E09C; // weak
} type_x_DWORD_17DE38str;
#pragma pack (16)

extern type_x_DWORD_17DE38str x_DWORD_17DE38str;
/*
extern Bit16s x_DWORD_17DEE4_mouse_positionx; // weak
extern Bit16s x_DWORD_17DEE4_mouse_positiony; // weak
extern __int16 x_WORD_17DEEC; // weak
extern __int16 x_WORD_17DEEE; // weak
extern __int16 x_WORD_17DEF0; // weak
extern __int16 x_WORD_17DEF2; // weak
extern __int16 x_WORD_17DEF4; // weak
extern __int16 x_WORD_17DEF6; // weak
extern __int16 x_WORD_17DEF8; // weak
extern __int16 x_WORD_17DEFA; // weak
extern __int16 x_WORD_17DEFC; // weak
extern __int16 x_WORD_17DEFE; // weak
extern __int16 x_WORD_17DF00; // weak
extern __int16 x_WORD_17DF02; // weak
extern __int16 x_WORD_17DF04; // weak
extern __int16 x_WORD_17DF06; // weak
extern __int16 x_WORD_17DF08; // weak
extern __int16 x_WORD_17DF0A; // weak
extern __int16 x_WORD_17DF0C; // weak
extern __int16 x_WORD_17DF0E; // weak
extern char x_BYTE_17DF10_get_key_scancode; // weak
extern char x_BYTE_17DF11_last_key_status; // weak
*/

extern Bit8u x_BYTE_10B4E0_terraintype[0x10000]; // idb// x_BYTE_10B1E0[0x300]//2DC4E0 //map array1
extern Bit8u x_BYTE_11B4E0_height[0x10000]; // idb		//2EC4E0    	//map array2 // heightmap
extern Bit8u x_BYTE_12B4E0_shading[0x10000]; // fix it -  weak	//2FC4E0    //map array3
extern Bit8u x_BYTE_13B4E0_angle[0x10000]; // idb//30C4E0	//map array4 // water
extern __int16 x_WORD_15B4E0_source[0x10000]; // idb//32C4E0	//map array5

extern Bit16u x_WORD_17B4E0; // 34c4e0

extern char x_BYTE_D41B6;
#pragma pack (1)
typedef struct {//lenght 28
	Bit32s dword_0;
	Bit32s dword_4;
	Bit32s dword_8;
	Bit16s word_12;
	Bit16u word_14;
	Bit16s word_16_maxframes;
	Bit16s word_18;
	Bit16s word_20;
	Bit16s word_22;//anim frame index
	Bit16s word_24;
	Bit16s word_26;
}
type_animations1;

typedef struct {
	Bit16s word_0;
	type_animations1* dword_2;
}
type_E9C08;
#pragma pack (16)
void /*__spoils<ecx>*/ sub_B5E70_decompress_terrain_map_level(__int16 a1, unsigned __int16 a2, __int16 a3, Bit32s a4);
void sub_44DB0_truncTerrainHeight();
void sub_44E40(int a1, unsigned __int8 a2);
void sub_45AA0_setMax4Tiles();
void sub_43B40();
void sub_43BB0();
void sub_43C60(unsigned __int8 a1, char a2, int a3, int a4);
void sub_43D50();
void sub_43EE0();
void sub_43FC0();
void sub_440D0(unsigned __int16 a1);
void sub_44320();
void sub_44580();
void sub_44D00();
void sub_45060(unsigned __int8 a1, unsigned __int8 a2);
void sub_45210(unsigned __int8 a1, unsigned __int8 a2);
void sub_454F0(unsigned __int8 a1, unsigned __int8 a2);
void sub_45600(unsigned __int8 a1);

void sub_49830(type_str_2FECE* a1);
void sub_49290(type_str_2FECE* a1, char a2);

void sub_49F30();


void sub_6FC50(__int16 a1);

void sub_6EBF0(filearray_struct* a1);

char sub_533B0_decompress_levels(__int16 a1, type_str_2FECE* a2);

bool sub_7B200_in_region(Bit16s* a1, Bit16s testx, Bit16s testy);
void sub_2BC10_draw_text(char* text, __int16 a2, __int16 a3, unsigned __int8 a4);
void sub_6F940_sub_draw_text(char* a1, int a2, int a3, Bit8u color);
//int /*__noreturn*/ sub_10000(); // weak
type_str_0x6E8E* sub_15CB0(/*type_str_0x6E8E* a1,*/ type_str_0x6E8E* a2); // weak
char sub_15D40(__int16 a1, type_str_0x6E8E* a2, type_str_0x6E8E* a3); // weak
Bit16s sub_16730(/*int a1,*/ type_str_0x6E8E* a2, char a3); // weak
Bit16s sub_16CA0(baxis_2d* a2, __int16 a3, char a4); // weak
void sub_17A00(x_BYTE *a1, signed int a2, __int16 a3); // weak
int _wcpp_1_unwind_leave__120(x_DWORD a, x_DWORD b, x_DWORD c);// weak
void sub_1A070(signed int a1, __int16 a2);
void JUMPOUT(int* adr);
void JUMPOUT(x_WORD cs, int* adr);
void JUMPOUT(x_WORD cs, int a, int* adr);
void sub_1F0C0(/*type_str_0x6E8E* a1,*/ type_str_0x6E8E* a2);
int sub_253B0(/*int result, */type_str_0x6E8E* a2, unsigned __int8 a3, unsigned __int8 a4);
signed int sub_369F0(/*signed int a1,*/ __int16 a2);
void qmemcpy(void* a, void* b, size_t c);
void sub_2A340(/*int a1, type_str_0x6E8E* a2, type_str_0x6E8E* a3, int a4, *//*type_str_0x6E8E* a5, */type_str_0x6E8E* a6);
void sub_2AA90(/*type_str_0x6E8E* a1, */type_str_0x6E8E* a2, type_str_0x6E8E* a3);
void /*__spoils<ecx>*/ sub_72883_clear_graphics_320(/*int a1, */void *a2, unsigned __int16 a3, char a4);
void /*__spoils<ecx>*/ sub_728A9_clear_graphics_640(void *a2, unsigned __int16 a3, char a4);
void sub_728A9_clear_graphics_alt(void* a2, unsigned __int16 a3, char a4);
void sub_303D0(/*signed int a1*/);
int _wcpp_1_unwind_leave__62(void); //weak
int sub_7FCB0_draw_text_with_border(int a1, char* a2, Bit32s a3, Bit32s a4, int a5, Bit8u a6, unsigned __int8 a7, Bit32u a8);//560cb0
void sub_45DC0(Bit8u a1, Bit8u a2, uaxis_2d a3, unsigned __int8 a4);
signed int sub_36A50(/*signed int a1, */char a2);
void sub_36920(/*signed __int16 a1, */type_str_0x6E8E* a2);
void sub_48A20(int a1, char a2, char a3, int a4, int a5, unsigned __int8 a6);
unsigned int sub_439A0(unsigned int a1, unsigned __int16 a2);
void sub_3E360_draw_particles(/*int a1x, */int a2x);
unsigned __int16 sub_3FD60(int a2x);
//void __outx_WORD(unsigned short Port, unsigned short Data);
//void __outx_BYTE(unsigned short Port, unsigned char Data);
//unsigned char __inx_BYTE(unsigned short Port);
void sub_43970(unsigned int a1);
void sub_44EE0_smooth_tiles(/*int a1, */uaxis_2d a2);
void sub_56A30_init_game_level(unsigned int a1);
void sub_47320_in_game_loop(signed int a1);
void sub_56D60(unsigned int a1, char a2);
void sub_75200_VGA_Blit640(Bit16u height);
void VGA_BlitAny();
void sub_7A060_get_mouse_and_keyboard_events();
void sub_47560_draw_and_events_in_game(/*Bit8u* a1, int a2, */Bit32u a3, signed int a4, __int16 a5);
void sub_51BB0_game_events(/*Bit8u* a1*/);
//x_DWORD /*__cdecl*/ toupper(x_DWORD); //weak
void sub_55C60(/*int a1, int a2,*/ type_str_0x2BDE* a3);
x_DWORD /*__cdecl*/ fix_filelength(x_DWORD);// weak
//x_DWORD /*__cdecl*/ access(char* path, Bit32u flags);// weak
//x_DWORD /*__cdecl*/ mkdir(char* path);
x_DWORD /*__cdecl*/ settextposition(x_DWORD, x_DWORD);// weak
x_DWORD /*__cdecl*/ outtext(char* text);// weak
//x_DWORD int386(x_DWORD, x_DWORD, x_DWORD);// weak
//int int386(int intno, REGS *inregs, REGS *outregs);
int /*__fastcall*/ gettextposition(x_DWORD, x_DWORD, x_DWORD);// weak
x_DWORD /*__cdecl*/ signal(x_DWORD, x_DWORD);// weak
//int __cdecl unknown_libname_1(char *a1);
//void _disable();
//void _enable();
int /*__fastcall*/ _wcpp_1_unwind_leave__93(x_DWORD, x_DWORD, x_DWORD, x_DWORD, x_DWORD, x_DWORD);// weak
int /*__cdecl*/ _wcpp_1_unwind_leave__100(x_DWORD, x_DWORD);// weak
//long _InterlockedExchange(long volatile * Target, long Value);
int /*__cdecl*/ _wcpp_1_unwind_leave__98(x_DWORD);// weak
//x_DWORD /*__cdecl*/ dos_getvect(x_DWORD);// weak
//void(*dos_getvect(x_DWORD number))();
//x_DWORD /*__cdecl*/ dos_setvect(x_DWORD number, void(*actcall)(), x_DWORD);// weak
int /*__fastcall*/ _wcpp_1_unwind_leave__130(x_DWORD);// weak
//unsigned char _BitScanReverse(unsigned long * Index, unsigned long Mask);
void sub_8F920(Bit8u a1byte1, Bit8u a1byte2, Bit16s posx, Bit16s posy, Bit8u* a4, unsigned __int8 a5, char a6);
void sub_8F935_bitmap_draw_final(Bit8u a1byte1, Bit8u a1byte2, Bit16u a2, int a3, Bit8u* a4, Bit8u setbyte, char a6);
signed int sub_74767(signed __int16 *a1, x_BYTE *a2, Bit8u* a3);
//x_DWORD /*__cdecl*/ segread(SREGS*); //weak
//x_DWORD /*__cdecl*/ int386x(x_DWORD, x_DWORD, x_DWORD, x_DWORD);// weak
//int int386x(int, REGS *, REGS *, struct SREGS *);
unsigned char __readgsx_BYTE(unsigned long Offset);
unsigned short __readgsx_WORD(unsigned long Offset);
//int /*__fastcall*/ _wcpp_1_unwind_leave__131(x_DWORD);// weak
void sub_7AB00_draw_menu_animations(/*__int16 a1,*/ /*int a2,*/ /*type_str_unk_E1748* a3,*/ unsigned __int8 a4);
int sub_7F6A0(int a1, int a2, __int16 a3, __int16 a4, char *a5, x_BYTE *a6, unsigned __int8 a7);
unsigned int sub_81CA0(int a1, int a2, __int16 a3, __int16 a4, type_x_BYTE_E25ED_db_str* a5);
signed int sub_7E5A0_pre_draw(int a1, int a2, __int16 a3, __int16 a4, Bit8u* a5, __int16 a6, __int16 a7);
void sub_81260(int a1, int a2, Bit8s* a3, __int16 a4, __int16 a5);
void sub_82510(/*__int16 a1*//*, int *a2*/);
signed __int16 sub_81EE0(int a1, int a2, int a3, signed __int16 a4, __int16 a5, __int16 a6);
signed int sub_80D40_move_graphics_and_play_sounds(__int16 a1, __int16 a2, __int16 a3, __int16 a4, __int16 a5, char a6);
void sub_85CC3_draw_round_frame(/*unsigned int a1, int a2, */unsigned __int16 *a3);
//void sub_840B8(char a1, bool a2, char a3, char a4);
//unsigned     int __readeflags(void);
__int16 sub_840D3(char _CF, bool _ZF, char _SF, char _OF);
void  sub_841CE(__int16 a1);
//void __writeeflags(unsigned Value);
x_DWORD /*__cdecl*/ gets(x_DWORD);// weak
//x_DWORD /*__cdecl*/ fix_tell(x_DWORD);// weak
x_DWORD /*__cdecl*/ expand(x_DWORD, x_DWORD);// weak
//int /*__fastcall*/ _wcpp_1_unwind_leave__132(x_DWORD);// weak
char sub_8BA10(int a1, int a2, int *a3, char *a4, int a5);
double /*__fastcall*/ _CHP(x_DWORD);// weak
char sub_8B980(int a1, int a2, x_DWORD **a3, int a4);
bool sub_9AE04(/*int eax0, */int edx0, int a3, int a1, int a2);
HDIGDRIVER sub_93330_AIL_install_DIG_driver_file(/*int a1, */char* filename, IO_PARMS* IO);
char sub_9AE90(int eax0, int edx0, int ebx0, int *a1, x_BYTE *a2, int a3, int a4);
int sub_931F0_AIL_install_DIG_INI(/*int a1, */HDIGDRIVER* a2);
HMDIDRIVER sub_95850_AIL_install_MDI_driver_file(char* filename, IO_PARMS* IO);
Bit32s sub_95710_AIL_install_MDI_INI(/*int a1, */HMDIDRIVER *mdi);
Bit32u /*__cdecl*/ x_read(FILE* descriptor, Bit8u* data, Bit32u size);// weak
double IF_DPOW(double, double);// weak
x_DWORD x_dos_setdrive(Bit32s a, Bit32s* b);
//int /*__cdecl*/ x_chdir(const char* path);
//char* /*__cdecl*/ x_getcwd(x_DWORD a, x_DWORD b);// weak
x_DWORD /*__cdecl*/ x_outp(x_DWORD, char);// weak
x_DWORD /*__cdecl*/ x_inp(x_DWORD);// weak
//x_DWORD *sub_9E720(int a1, int a2, int a3);
int sub_A3720_AIL_API_install_DIG_INI(/*int a1, */HDIGDRIVER* dig);
HDIGDRIVER sub_A3600_AIL_API_install_DIG_driver_file(/*int a1, */char* filename, IO_PARMS* IO);
Bit32s sub_A78F0_AIL_API_install_MDI_INI(HMDIDRIVER *mdi);
HMDIDRIVER sub_A77D0_AIL_API_install_MDI_INI(char* filename, IO_PARMS* IO);
FILE* /*__cdecl*/ x_creat(char* path, Bit32u flags);//
x_DWORD /*__cdecl*/ x_setmode(FILE* path, int mode);// weak
FILE* /*__cdecl*/ x_sopen(char* path, int pmode, Bit32u flags);
x_DWORD /*__cdecl*/ x_close(FILE*);// weak
x_DWORD /*__cdecl*/ x_lseek(FILE*, x_DWORD, char);// weak
//Bit32u sub_98AE0(Bit8u *a1);
__int16 sub_98B2C(unsigned __int8 a1, int a2);
//char sub_98BAF(int a1);
__int16 sub_98AE9(__int16 *a1, int a2);
unsigned __CFRCR__(__int16 a, unsigned __int8 b);
unsigned __CFRCL__(__int16 a, unsigned __int8 b);
Bit32u /*__cdecl*/ x_write(FILE* descriptor, Bit8u* buffer, Bit32u size_t);
x_DWORD /*__cdecl*/ x_tolower(x_DWORD);// weak
FILE* x_open(char* path, int pmodex);
void __writegsx_WORD(unsigned long Offset, unsigned short Data);
void __writegsx_DWORD(unsigned long Offset, unsigned long Data);
//x_DWORD /*__cdecl*/ dos_read(x_DWORD, char, x_DWORD, x_DWORD, x_DWORD);// weak
unsigned long __readgsx_DWORD(unsigned long Offset);
void __inx_BYTEstring(unsigned short Port, unsigned char* Buffer, unsigned long Count);
//int* MK_FP(unsigned int segment, unsigned int offset);
//unsigned int __getcallerseflags(void);
int /*__fastcall*/ _hook387(x_DWORD, x_DWORD, x_DWORD);// weak
//void __sidt(void *Destination);
//unsigned long __readcr0(void);
//void __writecr0(unsigned __int32 Data);
int sub_ACE8D(x_WORD *a1, int a2, int *a3);
void sub_AD0E2(x_BYTE *a1, int a2, int *a3, x_BYTE **a4);
void sub_ACF1A(x_BYTE *a1, int a2, int *a3);
int _FDFS(void);// weak
int sub_B33D6(unsigned __int64 a1, int a2, int a3, int a4);
//int _sigfpe_handler();// weak
int sub_B337C(int a1, int a2, int a3);
int sub_B37B0(int a1, int a2, int a3);
int sub_B35DB(int a1, int a2, x_BYTE *a3, int a4);
int sub_B39E0(__int64 a1, int a2, x_BYTE *a3);
int sub_B41F9(int a1, x_BYTE *a2, int a3);
int sub_B3ADF(__int64 *a1, x_BYTE *a2);
int sub_B3605(unsigned __int64 a1, int a2, unsigned int a3, x_BYTE *a4, int a5);
int sub_B37DA(unsigned __int64 a1, unsigned int a2, unsigned int a3, int a4);
__int16 /*__spoils<ecx>*/ sub_B3EAE(int a1, x_BYTE *a2, int a3);
int sub_B4A2A(int *a1, int a2, int a3);
int sub_B4077(__int64 a1, unsigned int a2, unsigned int a3, __int16 a4, __int16 a5);
int sub_B423C(int a1, x_BYTE *a2);
void sub_B49D8(int a1, int a2);
int _DOS4G_hook_init(void);// weak
int _Phar_hook_init(void);// weak
int /*__cdecl*/ _Ergo_hook_init(x_DWORD, x_DWORD);// weak
//int _Intel_hook_init(void);// weak
//int _DOS4G_hook_fini(void);// weak
double sub_B5250(char a1, double *a2, double result);
double sub_B5205(char a1, int a2, __int16 a3, double a4);
__int64 sub_B522B(int a1, __int16 a2, int _ESI);
//int _Intel_hook_fini(void);// weak
//int _Phar_hook_fini(void);// weak
//int /*__cdecl*/ _Ergo_hook_fini(x_DWORD);// weak
void sub_B5F8F(__int16 a1, Bit16u* a2, Bit32s a3, __int16* a4);
int sub_BD320(int result, x_BYTE *a2, x_BYTE *a3, int a4, int a5, int a6);
void sub_B5EFA(__int16 a1, Bit16u* a2, Bit32s a3, __int16* a4);

//type_str_0x6E8E* pre_sub_4A190(Bit32u adress, Bit16s* a1,int type);
type_str_0x6E8E* pre_sub_4A190_axis_3d(Bit32u adress, axis_3d* a1, int type);//pre 22b190


void pre_sub_4A190_0x6E8E(Bit32u adress, type_str_0x6E8E* a1, int type);//pre 22b190

//---------------------------

//char sub_54200();
void sub_71410_process_tmaps();
void sub_5B8D0_initialize();
int /*__cdecl*/ main(int argc, char **argv, char **envp);


#endif //SUB_MAIN_ACTIVE
