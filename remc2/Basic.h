#ifndef MAIN_BASIC
#define MAIN_BASIC

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cstdint>
#include <functional>

#include "stdint.h"

#ifdef _MSC_VER
#include <direct.h>  
#include <io.h>  
#include <windows.h>
#include "portability/dirent-x.h"
#else
#include "dirent.h"
#endif

#include "portability/port_outputs.h"
#include "portability/port_time.h"
#include "portability/port_filesystem.h"
#include "engine/engine_support.h"
#include "engine/read_config.h"
#include "engine/defs.h"

#include "utilities/DataFileIO.h"

#define __CFSHL__(x, y) (x<<y)

#pragma pack (1)
typedef struct//lenght 11
{
	int8_t byte_0;
	uint8_t array_byte_1[8];
	int8_t byte_9;
	int8_t byte_10;
} type_BYTE_17DE68x;
#pragma pack (16)

#pragma pack (1)
typedef struct//lenght 613 // end 17E09D
{
	uint8_t* x_DWORD_17DE38; // weak
	uint8_t* x_DWORD_17DE3C; // weak//1
	uint8_t* x_DWORD_17DE40; // weak//2
	uint8_t* x_DWORD_17DE44; // weak//3
	uint8_t* x_DWORD_17DE48c; // weak//4
	uint8_t* x_DWORD_17DE4C; // weak
	char* x_DWORD_17DE50; // weak
	uint8_t* x_DWORD_17DE54; // weak
	uint8_t* x_DWORD_17DE58; // weak
	uint8_t* x_DWORD_17DE5C_border_bitmap; // weak
	uint8_t* x_DWORD_17DE60; // weak
	uint8_t* x_DWORD_17DE64_game_world_map; // weak

	/*
	int8_t byte_0
	int8_t array_byte_1[8];
	int8_t byte_9;
	int8_t byte_10;
	*/
	type_BYTE_17DE68x array_BYTE_17DE68x[8];
	//uint8_t x_BYTE_17DE68x[88]; // fix it -  weak //8*11
	//char x_BYTE_17DE69[1]; //x_BYTE_17DE68x[0x1] fix it -  weak
	//char x_BYTE_17DE71[1]; //x_BYTE_17DE68x[0x9] fix it -  weak
	//char x_BYTE_17DE72[78]; //x_BYTE_17DE68x[0xa] idb
	uint8_t* x_DWORD_17DEC0; // weak
	uint8_t* x_DWORD_17DEC4; // weak
	uint8_t* x_DWORD_17DEC8; // weak
	uint8_t* x_DWORD_17DECC; // weak
	int x_DWORD_17DED0; // weak
	uint8_t* x_DWORD_17DED4; // weak
	uint8_t* x_DWORD_17DED8; // weak
	int x_DWORD_17DEDC; // weak
	FILE* x_DWORD_17DEE0_filedesc; // weak
	int16_t x_DWORD_17DEE4_mouse_positionx; // weak
	char stub0[2];
	int16_t x_DWORD_17DEE8_mouse_positiony; // weak
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
	int16_t x_WORD_17DF04; // selected save game number to load/save (set in load/save_game_dialog)
	__int16 x_WORD_17DF06; // weak
	__int16 x_WORD_17DF08; // weak
	__int16 x_WORD_17DF0A; // weak
	__int16 x_WORD_17DF0C; // weak
	__int16 x_WORD_17DF0E; // weak
	char x_BYTE_17DF10_get_key_scancode; // weak
	char x_BYTE_17DF11_last_key_status; // weak
	char stuba[1];
	char x_BYTE_17DF13; // weak
	char xx_BYTE_17DF14[8][43]; // 43*8, used for load_dialog_dialog
	//char x_BYTE_17DF3C[400]; // fix it -  weak//&x_BYTE_17DF14[40]
	//char x_BYTE_17DF3D[400]; // fix it -  weak//&x_BYTE_17DF14[41]
	//char x_BYTE_17DF3E[400]; // idb//&x_BYTE_17DF14[42]
	__int16 x_WORD_17E06C; // weak
	__int16 x_WORD_17E06E; // weak
	__int16 x_WORD_17E070; // weak
	__int16 x_WORD_17E072; // weak
	__int16 x_WORD_17E074; // weak
	__int16 x_WORD_17E076; // weak
	int8_t unk_17E078x[0xc]; // weak [11]
	//__int16 x_WORD_17E07C; // weak unk_17E078x[4]
	//__int16 x_WORD_17E07E; // weak unk_17E078x[6]
	//__int16 x_WORD_17E080; // weak unk_17E078x[8]
	//char x_BYTE_17E082; // weak unk_17E078x[0xa]
	//char x_BYTE_17E083; // weak unk_17E078x[0xb]
	int16_t unk_17E084x[0xc]; // weak [11]
	char x_BYTE_17E09C; // weak
} type_x_DWORD_17DE38str;
#pragma pack (16)

extern char gameDataPath[];
extern char cdDataPath[];
extern char bigGraphicsPath[];

extern uint8_t x_DWORD_17ECA0[];

extern int x_DWORD_E3E2C;

extern char x_BYTE_D41C1; // weak

extern char x_BYTE_D41CE; // weak

//inputs
extern __int16 x_WORD_180744_mouse_right_button; // weak//351744
extern __int16 x_WORD_180746_mouse_left_button; // weak//351746

//basic graphics
extern type_x_DWORD_17DE38str x_DWORD_17DE38str;
extern uint8_t* x_DWORD_E9C38_smalltit;


void sub_83E80_freemem4(uint8_t* a1);
int sub_84000(int a1);

x_DWORD x_outp(x_DWORD, char);// weak
x_DWORD x_inp(x_DWORD);// weak
void stub_fix_it();
void* sub_83CD0_malloc2(size_t a1);
void qmemcpy(void* a, void* b, size_t c);
int sub_9D490_free4(void* a1, int a2);
int x_free(void* ptr);
void* sub_83D70_malloc1(int a1);

void __writegsx_WORD(unsigned long Offset, unsigned short Data);
void __writegsx_DWORD(unsigned long Offset, unsigned long Data);
unsigned long __readgsx_DWORD(unsigned long Offset);
unsigned char __readgsx_BYTE(unsigned long Offset);
unsigned short __readgsx_WORD(unsigned long Offset);
unsigned     int x__readeflags(void);
void x__writeeflags(unsigned Value);
unsigned int x__getcallerseflags(void);

//language
extern char* x_DWORD_E9C4C_langindexbuffer[]; // idb

void dbgfprintf(FILE* file, const char* format, ...);

x_DWORD dos_read(FILE*, char, x_DWORD, x_DWORD, x_DWORD);

signed int sub_9DE20_get_file_lenght(char* a1);
uint8_t* sub_9DEA0_read_file(char* a1, uint8_t* a2);

FILE* x_open(char* path, int pmodex);

int sub_9D770(char* a1, char a2);
int sub_9D380(FILE* a1, int a2, char a3, int a4, int a5);//27e380

char x_toupper(char inputchar);
int32_t /*__cdecl*/ x_tolower(int32_t);// weak

//graphics basics - move it later to basic graphics
void sub_2EC30_clear_img_mem();
void sub_2EB60();
void sub_2EBB0_draw_text_with_border_630x340(char* a1);
int sub_7FCB0_draw_text_with_border(/*int a1,*/ char* a2, int32_t a3, int32_t a4, int a5, uint8_t a6, unsigned __int8 a7, uint32_t a8);//560cb0
void sub_7C120_draw_bitmap_640(int16_t posx, int16_t posy, posistruct_t tempstr);
void sub_76260_read_intro_pallette(uint8_t a1);
uint8_t sub_5BE80_test_pallette(uint8_t* a1, uint8_t a2, uint8_t a3, uint8_t a4);
void sub_7C140_draw_text_background(int16_t a1, int16_t a2, int16_t a3, int16_t a4, uint8_t a5);
void sub_41A90_VGA_pallette_install(uint8_t* a1);
void sub_2EC90(char a1);
uint32_t sub_7FAE0_draw_text(char* a1, __int16 a2, __int16 a3, __int16 a4, unsigned __int8 a5);
void sub_90478_VGA_Blit320();

void sub_6FC50(__int16 a1);//250c50
uint8_t sub_6FC10_letter_width();
unsigned int sub_6FC80_pre_draw_text(char* a1, __int16 a2, __int16 a3, __int16 a4, unsigned __int8 a5);

#endif //MAIN_BASIC