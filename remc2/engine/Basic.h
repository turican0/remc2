#pragma once

#include "BasicMini.h"
#include "Spells.h"
#include "LevelStructs.h"

#ifdef _MSC_VER
#include <direct.h>  
#include <io.h>  
#include "../portability/dirent-x.h"
#else
#include "dirent.h"
#endif

#include "../portability/port_outputs.h"
#include "../portability/port_time.h"
#include "../portability/port_filesystem.h"
#include "engine_support.h"
#include "read_config.h"

#include "../utilities/DataFileIO.h"
#include "../engine/ViewPort.h"
#include "../engine/GameBitmap.h"
#include "Type_E9C38_Smalltit.h"
#include "Type_F2C20ar.h"

#if defined(__LP64__) || defined(_WIN64) || (defined(__x86_64__) &&     !defined(__ILP32__) ) || defined(_M_X64) || defined(__ia64) || defined (_M_IA64) || defined(__aarch64__) || defined(__powerpc64__) || defined(__x86_64__) || (UINT_MAX >= 0xffffffffffffffff) || defined (COMPILE_FOR_64BIT)
#define x64_BIT_ENVIRONMENT 1
#else
#define x32_BIT_ENVIRONMENT 1
#endif

#define __CFSHL__(x, y) (x<<y)


#pragma pack (push,1)
typedef struct//lenght 11
{
	int8_t makeUpdate_0;
	uint8_t playerIndex_1[8];
	int8_t action_9;
	int8_t selectedLevel_10;
} type_BYTE_17DE68x;

typedef struct {//lenght 18
	int32_t dword_0;
}type_17ECA0;
#pragma pack (pop)

typedef struct {
	TColor c[256];
	uint8_t data[];
}TcolNext;

#pragma pack(push, 1)
typedef struct //lenght 24
{
	int32_t field_0;
	int16_t field_4;
	int16_t field_6;
	int16_t spriteIndex_8;
	int8_t firstSpriteIndex_10;
	int8_t lastSpriteIndex_11;
} type_animStruct3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct //lenght 24
{
	int16_t diffX_0;
	int16_t diffY_1;
	int16_t absDiffX2_2;
	int16_t absDiffY2_3;
	int16_t maxX_4;
	int16_t maxY_5;
	int16_t beginX_6;
	int16_t beginY_7;
	int16_t secondAdd_8;
	int16_t field_9;
	int16_t stateX_10;
	int16_t stateY_11;
} type_animStruct;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct //lenght 24
{
	int16_t actX_0;
	int16_t actY_1;
	int16_t beginX_2;
	int16_t beginY_3;
	int16_t maxX_4;
	int16_t maxY_5;
	int16_t begin_6;
	int16_t act_7;
	int16_t act2_8;
	int16_t begin2_9;
	int16_t stateX_10;
	int16_t stateY_11;
} type_animStruct2;
#pragma pack(pop)

#pragma pack (push,1)
typedef struct//lenght 613 // end 17E09D
{
	TColor* palette_17DE38x; // weak
	TcolNext* x_DWORD_17DE3C; // weak//1
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
	//char x_BYTE_17DE69[1]; //x_BYTE_17DE68x[1] fix it -  weak
	//char x_BYTE_17DE71[1]; //x_BYTE_17DE68x[9] fix it -  weak
	//char x_BYTE_17DE72[78]; //x_BYTE_17DE68x[10] idb
	bitmap_pos_struct2_t* x_DWORD_17DEC0; // weak
	bitmap_pos_struct2_t* x_DWORD_17DEC4; // weak
	bitmap_pos_struct2_t* x_DWORD_17DEC8; // weak
	bitmap_pos_struct2_t* x_DWORD_17DECC; // weak
	int x_DWORD_17DED0; // weak
	bitmap_pos_struct2_t* x_DWORD_17DED4; // weak
	bitmap_pos_struct2_t* x_DWORD_17DED8; // weak
	int x_DWORD_17DEDC; // weak
	FILE* x_DWORD_17DEE0_filedesc; // weak
	int16_t x_DWORD_17DEE4_mouse_positionx; // weak
	int16_t x_DWORD_17DEE6_mouse_positiony; // weak
	char stub0[2];
	char stub1[2];
	__int16 x_WORD_17DEEC; // weak
	__int16 x_WORD_17DEEE_mouse_buttons; // weak
	__int16 x_WORD_17DEF0; // weak
	__int16 x_WORD_17DEF2; // weak
	__int16 x_WORD_17DEF4; // weak
	__int16 x_WORD_17DEF6; // weak
	__int16 x_WORD_17DEF8; // weak
	__int16 networkSession_17DEFA; // weak
	__int16 serverIndex_17DEFC; // weak
	__int16 x_WORD_17DEFE; // weak
	__int16 x_WORD_17DF00; // weak
	uint16_t palMulti_17DF02; // weak
	int16_t savedGameIndex_17DF04; // selected save game number to load/save (set in load/save_game_dialog)
	__int16 x_WORD_17DF06; // weak
	__int16 x_WORD_17DF08; // weak
	__int16 x_WORD_17DF0A; // weak
	__int16 x_WORD_17DF0C; // weak
	__int16 x_WORD_17DF0E; // weak
	char x_BYTE_17DF10_get_key_scancode; // weak
	char x_BYTE_17DF11_last_key_status; // weak
	char stuba[1];
	char showHelp_17DF13; // weak
	char xx_BYTE_17DF14[8][43]; // 43*8, used for load_dialog_dialog
	//char x_BYTE_17DF3C[400]; // fix it -  weak//&x_BYTE_17DF14[40]
	//char x_BYTE_17DF3D[400]; // fix it -  weak//&x_BYTE_17DF14[41]
	//char x_BYTE_17DF3E[400]; // idb//&x_BYTE_17DF14[42]
	__int16 x_17E06C; // weak
	__int16 y_17E06E; // weak
	__int16 x_WORD_17E070; // weak
	__int16 x_WORD_17E072; // weak
	__int16 x2_17E074; // weak
	__int16 y2_17E076; // weak
	type_animStruct3 unk_17E078x; // weak [11]
	//__int16 x_WORD_17E07C; // weak unk_17E078x[4]
	//__int16 x_WORD_17E07E; // weak unk_17E078x[6]
	//__int16 x_WORD_17E080; // weak unk_17E078x[8]
	//char x_BYTE_17E082; // weak unk_17E078x[10]
	//char x_BYTE_17E083; // weak unk_17E078x[11]
	type_animStruct unk_17E084x; // weak [11]
	char x_BYTE_17E09C; // weak
} type_x_DWORD_17DE38str;
#pragma pack (pop)

typedef struct {
	std::string Message;
	int16_t Duration;
}
Type_Message;

//extern int16_t m_iViewPortX_EA3D0; // weak?x_DWORD_E9C4C_langindexbuffer[481]
//extern uint16_t m_uiViewPortWidth_EA3C4; // weak?x_DWORD_E9C4C_langindexbuffer[478]

extern type_str_unk_1804B0ar str_unk_1804B0ar;
extern __int16 x_WORD_180660_VGA_type_resolution;
extern uint8_t playersColors_E88E0x[8][3];
extern uint8_t unk_F0A20x[1024];//2c1a20
extern uint8_t keyColor1_D4B7C; // weak
extern uint8_t keyColor2_D4B7E; // weak
extern type_entity_0x6E8E* Entities_EA3E4[1001];//2bb3e4

extern uint8_t ColourLookupTable_F6EE0[83456];// (uint8_t*)&x_BYTE_F6EE0_tablesbuff;//animated sprites
const uint32_t COLOUR_BLEND_LOOKUP_OFFSET = 0x4000;
const uint32_t MAP_COLOUR_BLEND_LOOKUP_OFFSET = 0x14000;

extern uint8_t* x_BYTE_F6EE0_tablesx_pre;
extern uint8_t* x_BYTE_FAEE0_tablesx_pre;

extern std::array<uint8_t*, 256> x_DWORD_DDF50_texture_adresses; /*= { 0,32 }*/ // weak

const uint16_t TILE_ROWS_COUNT = 21;
const uint16_t TILE_COLUMNS_COUNT = 40;
extern type_E9C38_smalltit Str_E9C38_smalltit[TILE_ROWS_COUNT * TILE_COLUMNS_COUNT];

extern type_F2C20ar str_F2C20ar;

extern std::string gameDataPath;
extern std::string cdDataPath;
extern std::string highResGraphicsPath;
extern std::string fixedMenuGraphicsPath;

//extern type_17ECA0 str_17ECA0[];
//extern uint8_t x_DWORD_17ECA0[];

extern long oldmillis;

extern std::chrono::time_point<std::chrono::system_clock> m_frameStart;
extern std::chrono::time_point<std::chrono::system_clock> m_lastFrameEnd;
extern float m_fTimeElapsed; // The time that has elapsed so far
extern int m_iFrameCount; // The number of frames that have occurred.
extern float m_fFps; // The frames rendered per second. Needs to be stored to be shown every frame.

extern int readFileStatus_E3E2C;
extern char DisplaySubtitles_D41C1; // weak
extern char SubtitlesOn_D41CE; // weak
extern char* x_DWORD_D41D0; // weak
extern uint8_t* x_DWORD_17DB50; // weak
extern int x_DWORD_E3890;
extern int x_DWORD_E3768; // weak
extern char x_BYTE_E3766; // weak
extern int16_t unk_1806EC; // weak
extern uint16_t x_WORD_18072C_cursor_sizex; // weak
extern uint16_t x_WORD_18072E_cursor_sizey; // weak
extern uint8_t* x_DWORD_180730_cursor_data; // weak//351730
extern char* x_DWORD_D41BC_langbuffer; // fix it
extern char* x_DWORD_D41BC_langbuffer; // fix it
extern int x_DWORD_18073A; // weak

//inputs
extern __int16 x_WORD_180744_mouse_right_button; // weak//351744
extern __int16 x_WORD_180746_mouse_left_button; // weak//351746
extern __int16 x_WORD_18074A_mouse_right2_button; // weak//35174a
extern __int16 x_WORD_18074C_mouse_left2_button; // weak//35174c

//basic graphics
extern type_x_DWORD_17DE38str x_DWORD_17DE38str;
extern uint8_t* x_DWORD_E9C38_smalltit;

extern int Int386Request_17FF0C;
extern int MscdexCommand_17FF44;
extern uint8_t MinTrackIdx_180471;
extern uint8_t MaxTrackIdx_180472;

extern TColor unk_17D838x[]; // weak
extern uint8_t x_BYTE_D41B5_texture_size;

#pragma pack (push,1)
typedef struct {
	bitmap_pos_struct2_t** begin_buffer;
	bitmap_pos_struct2_t** end_buffer;
	uint8_t** dat_buffer;
	bitmap_pos_struct_t** posistruct;
} filearray_struct;

typedef struct {//lenght 10
	int32_t dword_0;
	int32_t dword_4;
	int16_t word_8;
}
type_TMAPS00TAB_BEGIN_BUFFER;
#pragma pack (pop)

extern int filearrayindex_POINTERSDATTAB;
extern int filearrayindex_FONTS0DATTAB;
extern int filearrayindex_FONTS1DATTAB;
extern int filearrayindex_ZERO1;
extern int filearrayindex_MSPRD00DATTAB;
extern int filearrayindex_ZERO2;
extern int filearrayindex_HFONT3DATTAB;
extern int filearrayindex_ZERO3;
extern int filearrayindex_BUILD00DATTAB;
extern int filearrayindex_ZERO4;

extern uint8_t* ZERO_BUFFER;
extern bitmap_pos_struct2_t* ZERO_BUFFER_PST2;

extern uint8_t* SEARCH_BEGIN_BUFFER;
extern uint8_t* SEARCH_END_BUFFER;
extern uint8_t* FONTS0DAT_BEGIN_BUFFER;
extern bitmap_pos_struct2_t* FONTS0TAB_BEGIN_BUFFER;
extern bitmap_pos_struct2_t* FONTS0TAB_END_BUFFER;
extern uint8_t* FONTS1DAT_BEGIN_BUFFER;//2baa94
extern bitmap_pos_struct2_t* FONTS1TAB_BEGIN_BUFFER;//2bab24
extern bitmap_pos_struct2_t* FONTS1TAB_END_BUFFER;//2bab04
extern type_TMAPS00TAB_BEGIN_BUFFER* str_TMAPS00TAB_BEGIN_BUFFER;
//uint8_t* TMAPS00TAB_BEGIN_BUFFER;//2c7ed0
extern TColor* str_PALDATA_BEGIN_BUFFER;//2bb3c8
extern TColor* str_PALMEM_BEGIN_BUFFER;//6d654d
extern uint8_t* POINTERSDAT_BEGIN_BUFFER;//2bc390
extern bitmap_pos_struct2_t* POINTERSTAB_BEGIN_BUFFER;//2bc394
extern bitmap_pos_struct2_t* POINTERSTAB_END_BUFFER;//2bc388

extern uint8_t* BUILD00DAT_BEGIN_BUFFER;
extern bitmap_pos_struct2_t* BUILD00TAB_BEGIN_BUFFER;
extern bitmap_pos_struct2_t* BUILD00TAB_END_BUFFER;

extern uint8_t* ETEXT_BEGIN_BUFFER;
extern uint8_t* FTEXT_BEGIN_BUFFER;
extern uint8_t* GTEXT_BEGIN_BUFFER;
extern uint8_t* ITEXT_BEGIN_BUFFER;

extern uint8_t* BLOCK16DAT_BEGIN_BUFFER;
extern uint8_t* BLOCK32DAT_BEGIN_BUFFER;

extern uint8_t* BSCREEN_BEGIN_BUFFER;
extern uint8_t* BSCREEN_END_BUFFER;

extern uint8_t* PALD0DAT_BEGIN_BUFFER;
extern uint8_t* PALD0DAT2_BEGIN_BUFFER;

extern uint8_t* WSCREEN_BEGIN_BUFFER;
extern uint8_t* WSCREEN_END_BUFFER;

//uint8_t* BSCREEN2_BEGIN_BUFFER;
extern uint8_t* BSCREEN2_END_BUFFER;

extern uint8_t* MSPRD00DAT_BEGIN_BUFFER;
extern bitmap_pos_struct2_t* MSPRD00TAB_BEGIN_BUFFER;
extern bitmap_pos_struct2_t* MSPRD00TAB_END_BUFFER;

extern uint8_t* HSPRD00DAT_BEGIN_BUFFER;
extern bitmap_pos_struct2_t* HSPRD00TAB_BEGIN_BUFFER;
extern bitmap_pos_struct2_t* HSPRD00TAB_END_BUFFER;

extern uint8_t* HFONT3DAT_BEGIN_BUFFER;
extern bitmap_pos_struct2_t* HFONT3TAB_BEGIN_BUFFER;
extern bitmap_pos_struct2_t* HFONT3TAB_END_BUFFER;

extern uint8_t* CLRD0DAT_BEGIN_BUFFER;

extern int x_DWORD_1806F4_mouse_byte_index2; // weak

extern int help_VGA_type_resolution;

extern filearray_struct filearray_2aa18c[];

extern bitmap_pos_struct_t* posistruct1;//fix it
extern bitmap_pos_struct_t* posistruct2;//fix it
extern bitmap_pos_struct_t* posistruct3;//fix it
extern bitmap_pos_struct_t* posistruct4;//fix it
extern bitmap_pos_struct_t* posistruct5;//fix it
extern bitmap_pos_struct_t* posistruct6;//fix it
extern bitmap_pos_struct_t* posistruct7;//fix it
extern bitmap_pos_struct_t* posistruct8;//fix it
extern bitmap_pos_struct_t* posistruct9;//fix it
extern bitmap_pos_struct_t* posistruct10;//fix it
extern bitmap_pos_struct_t* posistruct11;//fix it

extern bitmap_pos_struct_t* x_DWORD_EA3D4; // weak

extern Pathstruct xasearchd_2bac30;
#define psxasearchd_2bac30 0
extern Pathstruct xafonts0dat;
#define psxafonts0dat 1
extern Pathstruct xafonts0tab;
#define psxafonts0tab 2
extern Pathstruct xafonts1dat;
#define psxafonts1dat 3
extern Pathstruct xafonts1tab;
#define psxafonts1tab 4
extern Pathstruct xadatatmaps00tab;
#define psxadatatmaps00tab 5
extern Pathstruct xapaldata;
#define psxapaldata 6
extern Pathstruct xapalmem;
#define psxapalmem 7
extern Pathstruct xadatapointersdat;
#define psxadatapointersdat 8
extern Pathstruct xadatapointerstab;
#define psxadatapointerstab 9
extern Pathstruct xazero;
#define psxazero0 10
extern Pathstruct xadatabuild00dat;
#define psxadatabuild00dat 11
extern Pathstruct xadatabuild00tab;
#define psxadatabuild00tab 12
//zero
#define psxazero1 13
extern Pathstruct xadataetextdat;
#define psxadataetextdat 14
//zero
#define psxazero2 15
extern Pathstruct xadataftextdat;
#define psxadataftextdat 16
//zero
#define psxazero3 17
extern Pathstruct xadatagtextdat;
#define psxadatagtextdat 18
//zero
#define psxazero4 19
extern Pathstruct xadataitextdat;
#define psxadataitextdat 20
//zero
#define psxazero5 21
extern Pathstruct xadatablock16dat;
#define psxadatablock16dat 22
extern Pathstruct xadatablock32dat;
#define psxadatablock32dat 23
//zero
#define psxazero6 24
extern Pathstruct xabscreen;
#define psxabscreen 25
extern Pathstruct xadatapald0dat;
#define psxadatapald0dat 26
//zero
#define psxazero7 27
extern Pathstruct xawscreen_351628;
#define psxawscreen_351628 28
extern Pathstruct xabscreen2;
#define psxabscreen2 29

extern Pathstruct xadatapald0dat2;
#define psxadatapald0dat2 30
//zero
#define psxazero8 31
extern Pathstruct xadatamsprd00dat;
#define psxadatamsprd00dat 32
extern Pathstruct xadatamsprd00tab;
#define psxadatamsprd00tab 33
//zero
#define psxazero9 34
extern Pathstruct xadatahsprd00dat;
#define psxadatahsprd00dat 35
extern Pathstruct xadatahsprd00tab;
#define psxadatahsprd00tab 36
extern Pathstruct xadatahfont3dat;
#define psxadatahfont3dat 37
extern Pathstruct xadatahfont3tab;
#define psxadatahfont3tab 38
//zero
#define psxazero10 39
extern Pathstruct xadataclrd0dat;
#define psxadataclrd0dat 40
//zero
#define psxazero11 41
extern Pathstruct xadataspellsdatx;
#define psxadataspellsdat 42
//zero
#define psxazero12 43
extern Pathstruct xadatalang;
#define psxadatalangbuffer 44
//zero
#define psxazero13 45
extern Pathstruct xadatatables;
#define psxadatalangbuffer2 46
//zero
#define psxazero14 47

extern axis_2d x_WORD_E3760_mouse;
extern uint8_t* ptrMemoryBuffer_E9C3C; // Looks like a generic buffer, used for video and sound operations
extern type_SPELLS_BEGIN_BUFFER_str SPELLS_BEGIN_BUFFER_str[];

extern int16_t x_WORD_1806EE; // weak

extern uint8_t* ptrScreenPos_180708; // weak
extern uint8_t* ptrCursorBitmapPos_18070C; // weak
extern uint8_t* x_DWORD_180714; // weak
extern uint8_t* x_DWORD_180718; // weak

extern uint8_t* x_DWORD_1806F0; // weak

extern __int16 x_WORD_180738; // weak

extern uint8_t* x_DWORD_180700; // weak

extern int x_DWORD_1806F4_mouse_byte_index2; // weak
extern int16_t x_DWORD_1806FC_mouse_invx; // weak
extern int16_t x_DWORD_1806FC_mouse_invy; // weak
extern uint8_t* x_DWORD_180700; // weak
extern int x_DWORD_180704_mouse_byte_index1; // weak

extern int x_DWORD_180644_map_resolution2_y; // weak
extern int x_DWORD_180648_map_resolution2_x; // weak

extern int16_t x_DWORD_1806F8_mousex; // weak
extern int16_t x_DWORD_1806F8_mousey; // weak

extern int x_DWORD_E3758; // weak

extern int x_DWORD_180734; // weak
extern int x_DWORD_18073A; // weak

extern int x_DWORD_180650_positiony; // weak

extern int x_DWORD_180630_screen_height; // weak
extern int x_DWORD_180634_screen_width; // weak

extern int x_DWORD_18063C_sprite_sizex; // weak

extern __int16 x_WORD_E36D4; // weak

extern type_TMAPS00TAB_BEGIN_BUFFER* str_TMAPS00TAB_BEGIN_BUFFER;

extern uint8_t* Zero_pointer;

volatile extern int16_t GameTimerTurn_17DB54; // 34EB54

extern uint32_t PitFrequency_F4240;

extern std::vector<Type_Message*>* m_Messages;

bool DefaultResolutions();
bool IsDefaultResolution(int width, int height);
bool IsDefaultResolution320(int width, int height);
bool IsDefaultResolution640(int width, int height);

void FreeMem_83E80(uint8_t* a1);

x_DWORD x_outp(x_DWORD, char);// weak
x_DWORD x_inp(x_DWORD);// weak
void stub_fix_it();
void* Malloc_83CD0(size_t a1);
void qmemcpy(void* dest, void* src, size_t size);
int FreeMem_9D490(void* a1, int a2);
int x_free(void* ptr);
void* Malloc_83D70(int a1);

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

x_DWORD dos_read(FILE*, char, x_DWORD, x_DWORD, char*);

long GetFileLenght_9DE20(char* filename);
uint8_t* ReadFile_9DEA0(char* filename, uint8_t* buffer);

FILE* x_open(char* path, int pmodex);

int sub_9D770(char* a1, char a2);
int sub_9D380(FILE* a1, int a2, char a3, uint8_t* a4x, int a5);//27e380

char x_toupper(char inputchar);
int32_t /*__cdecl*/ x_tolower(int32_t);// weak

//graphics basics - move it later to basic graphics
void sub_2EC30_clear_img_mem();
void StartSubtitles_2EB60();
void sub_2EBB0_draw_text_with_border_630x340(char* textString);
void DrawTextWithBoarder_7FCB0(char* textString, int32_t a3, int32_t a4, int a5, uint8_t a6, unsigned __int8 a7, uint32_t a8);//560cb0
void sub_7C120_draw_bitmap_640(int16_t posx, int16_t posy, bitmap_pos_struct_t tempstr);
uint8_t getPaletteIndex_5BE80(TColor* a1x, uint8_t a2, uint8_t a3, uint8_t a4);
void sub_7C140_draw_text_background(int16_t a1, int16_t a2, int16_t a3, int16_t a4, uint8_t a5);
void sub_41A90_VGA_Palette_install(TColor* a1x);
void sub_2EC90(char a1);
uint32_t DrawText_7FAE0(char* text, int16_t x, int16_t width, int16_t posy, uint8_t color);
void sub_90478_VGA_Blit320(uint8_t maxFps = 0);
void sub_75200_VGA_Blit640(uint16_t height, uint8_t maxFps = 0);
uint8_t GetLetterHeight_6FC30();
void DrawBitmap_2BB40(int16_t posx, int16_t posy, bitmap_pos_struct_t temposstr, uint8_t scale = 1);

void GetFont_6FC50(__int16 a1);//250c50
uint8_t GetLetterWidth_6FC10();
unsigned int sub_6FC80_pre_draw_text(char* a1, __int16 a2, __int16 a3, __int16 a4, unsigned __int8 a5);
void CopyAndShiftFrom17DB50_75D70(uint8_t* a1, uint32_t count);
void DrawLine_2BC80(int16_t posStartX, int16_t posStartY, int16_t posEndX, int16_t posEndY, uint8_t colorIdx);
void DrawText_2BC10(const char* textbuffer, int16_t posx, int16_t posy, uint8_t color, uint8_t scale = 1);//20cc10
void SetFrameStart(std::chrono::system_clock::time_point frameStart);
std::chrono::duration<double, std::milli> CalculateTimeDelta();
void VGA_CalculateAndPrintFps(int x, int y, float timeDelta);
void AddMessage(Type_Message* message);
void ClearMessages();
void VGA_DrawMessages();
void VGA_DrawPlayerCoordData(int x, int y);
void VGA_BlitAny(uint8_t maxFps = 0);
void LockFps(uint8_t maxFps);
void sub_6EF10_set_mouse_minmax(__int16 a1, signed __int16 a2, __int16 a3, signed __int16 a4);
void DrawText_7FB90(char* textbuffer, int16_t posx, int16_t posy, uint8_t color);
void sub_8CACD_draw_cursor2();
void drawBitmap320_8F8B0(int16_t posx, int16_t posy, bitmap_pos_struct_t temppstr, uint8_t scale = 1);
void drawBitmap640_8F8E8(int16_t posx, int16_t posy, bitmap_pos_struct_t temppstr, uint8_t scale = 1);
void DrawLineLowRes_90164(int16_t posStartX, int16_t posStartY, int16_t posEndX, int16_t posEndY, uint8_t colorIdx);
void DrawLineHighRes_901E4(int16_t posStartX, int16_t posStartY, int16_t posEndX, int16_t posEndY, uint8_t colorIdx);
void sub_6F940_sub_draw_text(const char* textbuffer, int posx, int posy, uint8_t color, uint8_t scale = 1);//250940
void sub_72C40_draw_bitmap_640_setcolor(int16_t posx, int16_t posy, bitmap_pos_struct_t a3, uint8_t color, uint8_t scale = 1);
void DrawCursor_8C635();
void sub_8F935_bitmap_draw_final(uint8_t width, uint8_t height, uint16_t tiley, int tilex, uint8_t* texture, uint8_t setbyte, char a6, uint8_t scale);
void sub_8F920(uint8_t a1byte1, uint8_t a1byte2, int16_t posx, int16_t posy, uint8_t* a4, unsigned __int8 a5, char a6);

signed int GetTrueWizardNumber_61790(signed int inputnumber);

void Convert_from_shadow_D41A0_BYTESTR_0(type_shadow_D41A0_BYTESTR_0* from, type_D41A0_BYTESTR_0* to);
void Convert_to_shadow_D41A0_BYTESTR_0(type_D41A0_BYTESTR_0* from, type_shadow_D41A0_BYTESTR_0* to);
void Convert_to_shadow_str_E2A74(const type_array_str_E2A74 &from, type_shadow_str_E2A74* to);

void sub_417A0_install_pal_and_mouse_minmax();
unsigned long j___clock();

void SetPlayerLocation(uint16_t x, uint16_t y, int16_t z, int16_t pitch, int16_t roll, int16_t yaw);

void fix_sub_9A0FC_wait_to_screen_beam(int32_t delay);
