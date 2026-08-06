#include "Basic.h"
#include "engine_support.h"
#include "CommandLineParser.h"

std::string gameDataPath;
std::string cdDataPath;
std::string highResGraphicsPath;
std::string fixedMenuGraphicsPath;

//lenght 18
//type_17ECA0 str_17ECA0[256]; // weak
//uint8_t x_DWORD_17ECA0[4608]; // weak

//extern int16_t m_iViewPortX_EA3D0; // weak?x_DWORD_E9C4C_langindexbuffer[481]
//extern uint16_t m_uiViewPortWidth_EA3C4; // weak?x_DWORD_E9C4C_langindexbuffer[478]

type_str_unk_1804B0ar str_unk_1804B0ar;

TColor unk_17D838x[256]; // weak

int readFileStatus_E3E2C = 0; // weak

char x_BYTE_E3766 = 0; // weak

long oldmillis = 0;

std::chrono::time_point<std::chrono::system_clock> m_frameStart;
std::chrono::time_point<std::chrono::system_clock> m_lastFrameEnd;
float m_fTimeElapsed = 0.0f; // The time that has elapsed so far
int m_iFrameCount = 0; // The number of frames that have occurred.
float m_fFps; // The frames rendered per second. Needs to be stored to be shown every frame.

uint8_t* x_DWORD_E9C3C; // weak

uint8_t* x_DWORD_17DB50; // weak

int x_DWORD_E3768 = 0; // weak

int x_DWORD_E3758 = 0; // weak

int x_DWORD_E3890 = 0; // weak

int16_t unk_1806EC; // weak

axis_2d x_WORD_E3760_mouse;

int16_t x_WORD_1806EE; // weak

uint8_t* x_DWORD_1806F0; // weak

__int16 x_WORD_180738; // weak

int x_DWORD_180630_screen_height; // weak
int x_DWORD_180634_screen_width; // weak

int x_DWORD_180644_map_resolution2_y; // weak
int x_DWORD_180648_map_resolution2_x; // weak

int x_DWORD_180650_positiony; // weak

int x_DWORD_18063C_sprite_sizex; // weak

int x_DWORD_180734; // weak
int x_DWORD_18073A; // weak

int16_t x_DWORD_1806F8_mousex; // weak
int16_t x_DWORD_1806F8_mousey; // weak

int x_DWORD_1806F4_mouse_byte_index2; // weak
int16_t x_DWORD_1806FC_mouse_invx; // weak
int16_t x_DWORD_1806FC_mouse_invy; // weak
uint8_t* x_DWORD_180700; // weak
int x_DWORD_180704_mouse_byte_index1; // weak

uint32_t screenHeight_180624; // weak
uint32_t screenWidth_18062C; // weak

int iScreenWidth_DE560 = 0; // weak //screen X // DE560

//std::array<uint8_t*, 256> x_DWORD_DDF50_texture_adresses; /*= { 0,32 }*/ // weak

//inputs
__int16 x_WORD_180744_mouse_right_button; // weak//351744
__int16 x_WORD_180746_mouse_left_button; // weak//351746
__int16 x_WORD_18074A_mouse_right2_button; // weak//35174a
__int16 x_WORD_18074C_mouse_left2_button; // weak//35174c

uint16_t x_WORD_18072C_cursor_sizex; // weak
uint16_t x_WORD_18072E_cursor_sizey; // weak
uint8_t* x_DWORD_180730_cursor_data; // weak//351730

uint8_t* ptrScreenPos_180708; // weak
uint8_t* ptrCursorBitmapPos_18070C; // weak
uint8_t* x_DWORD_180714; // weak
uint8_t* x_DWORD_180718; // weak

extern axis_2d x_WORD_E3760_mouse;

//basic graphics

char SubtitlesOn_D41CE = 0; // weak
uint8_t* x_DWORD_F01E4; // weak
uint16_t x_WORD_F01F4; // weak
char DisplaySubtitles_D41C1 = 0; // weak
char* x_DWORD_D41D0 = 0; // weak
__int16 x_WORD_E36D4 = 0; // weak
char x_BYTE_EB3B6; // weak
type_x_DWORD_17DE38str x_DWORD_17DE38str;
uint8_t* x_DWORD_E9C38_smalltit;//*(x_DWORD*)(x_DWORD_E9C38_smalltit + 36964)
type_F2C20ar str_F2C20ar;

int help_VGA_type_resolution = 0;

int16_t x_WORD_180660_VGA_type_resolution; // weak
uint8_t playersColors_E88E0x[8][3];
uint8_t unk_F0A20x[1024];
uint8_t keyColor1_D4B7C = 0xfe; // some key color?
uint8_t keyColor2_D4B7E = 0x00; // some key color?
type_entity_0x6E8E* Entities_EA3E4[1001];//2bb3e4

uint8_t x_BYTE_F6EE0_tablesx[83456];// (uint8_t*)&x_BYTE_F6EE0_tablesbuff;//animated sprites
uint8_t* x_BYTE_F6EE0_tablesx_pre = (uint8_t*)x_BYTE_F6EE0_tablesx;
uint8_t* x_BYTE_FAEE0_tablesx_pre = (uint8_t*)&x_BYTE_F6EE0_tablesx[16384];

type_E9C38_smalltit Str_E9C38_smalltit[TILE_ROWS_COUNT * TILE_COLUMNS_COUNT];

uint8_t* ViewPortRenderBufferStart_DE558 = 0;

uint8_t* ViewPortRenderBufferAltStart_DE554 = 0;

//language
char* x_DWORD_E9C4C_langindexbuffer[1000]; // idb

volatile int16_t GameTimerTurn_17DB54 = 0;
uint32_t PitFrequency_F4240 = 1193182; //PIT Frequency

int Int386Request_17FF0C;
int MscdexCommand_17FF44;
uint8_t MinTrackIdx_180471;
uint8_t MaxTrackIdx_180472;

//file

int filearrayindex_POINTERSDATTAB = 0;
int filearrayindex_FONTS0DATTAB = 1;
int filearrayindex_FONTS1DATTAB = 2;
int filearrayindex_ZERO1 = 3;
int filearrayindex_MSPRD00DATTAB = 4;
int filearrayindex_ZERO2 = 5;
int filearrayindex_HFONT3DATTAB = 6;
int filearrayindex_ZERO3 = 7;
int filearrayindex_BUILD00DATTAB = 8;
int filearrayindex_ZERO4 = 9;


uint8_t* SEARCH_BEGIN_BUFFER = 0;
uint8_t* SEARCH_END_BUFFER = 0;
uint8_t* FONTS0DAT_BEGIN_BUFFER = 0;
bitmap_pos_struct2_t* FONTS0TAB_BEGIN_BUFFER = 0;
bitmap_pos_struct2_t* FONTS0TAB_END_BUFFER = 0;
uint8_t* FONTS1DAT_BEGIN_BUFFER = 0;//2baa94
bitmap_pos_struct2_t* FONTS1TAB_BEGIN_BUFFER = 0;//2bab24
bitmap_pos_struct2_t* FONTS1TAB_END_BUFFER = 0;//2bab04
type_TMAPS00TAB_BEGIN_BUFFER* str_TMAPS00TAB_BEGIN_BUFFER;
//uint8_t* TMAPS00TAB_BEGIN_BUFFER = 0;//2c7ed0
TColor* str_PALDATA_BEGIN_BUFFER = 0;//2bb3c8
TColor* str_PALMEM_BEGIN_BUFFER = 0;//6d654d
uint8_t* POINTERSDAT_BEGIN_BUFFER = 0;//2bc390
bitmap_pos_struct2_t* POINTERSTAB_BEGIN_BUFFER = 0;//2bc394
bitmap_pos_struct2_t* POINTERSTAB_END_BUFFER = 0;//2bc388*/

uint8_t* BUILD00DAT_BEGIN_BUFFER = 0;
bitmap_pos_struct2_t* BUILD00TAB_BEGIN_BUFFER = 0;
bitmap_pos_struct2_t* BUILD00TAB_END_BUFFER = 0;

uint8_t* ETEXT_BEGIN_BUFFER = 0;
uint8_t* FTEXT_BEGIN_BUFFER = 0;
uint8_t* GTEXT_BEGIN_BUFFER = 0;
uint8_t* ITEXT_BEGIN_BUFFER = 0;

uint8_t* BLOCK16DAT_BEGIN_BUFFER = 0;
uint8_t* BLOCK32DAT_BEGIN_BUFFER = 0;

uint8_t* BSCREEN_BEGIN_BUFFER = 0;
uint8_t* BSCREEN_END_BUFFER = 0;

uint8_t* PALD0DAT_BEGIN_BUFFER = 0;
uint8_t* PALD0DAT2_BEGIN_BUFFER = 0;

uint8_t* WSCREEN_BEGIN_BUFFER = 0;
uint8_t* WSCREEN_END_BUFFER = 0;

//uint8_t* BSCREEN2_BEGIN_BUFFER = 0;
uint8_t* BSCREEN2_END_BUFFER = 0;

uint8_t* MSPRD00DAT_BEGIN_BUFFER = 0;
bitmap_pos_struct2_t* MSPRD00TAB_BEGIN_BUFFER = 0;
bitmap_pos_struct2_t* MSPRD00TAB_END_BUFFER = 0;

uint8_t* HSPRD00DAT_BEGIN_BUFFER = 0;
bitmap_pos_struct2_t* HSPRD00TAB_BEGIN_BUFFER = 0;
bitmap_pos_struct2_t* HSPRD00TAB_END_BUFFER = 0;

uint8_t* HFONT3DAT_BEGIN_BUFFER = 0;
bitmap_pos_struct2_t* HFONT3TAB_BEGIN_BUFFER = 0;
bitmap_pos_struct2_t* HFONT3TAB_END_BUFFER = 0;

uint8_t* CLRD0DAT_BEGIN_BUFFER = 0;

bitmap_pos_struct_t* posistruct1;//fix it
bitmap_pos_struct_t* posistruct2;//fix it
bitmap_pos_struct_t* posistruct3;//fix it
bitmap_pos_struct_t* posistruct4;//fix it
bitmap_pos_struct_t* posistruct5;//fix it
bitmap_pos_struct_t* posistruct6;//fix it
bitmap_pos_struct_t* posistruct7;//fix it
bitmap_pos_struct_t* posistruct8;//fix it
bitmap_pos_struct_t* posistruct9;//fix it
bitmap_pos_struct_t* posistruct10;//fix it
bitmap_pos_struct_t* posistruct11;//fix it

uint8_t* ZERO_BUFFER = 0;
bitmap_pos_struct2_t* ZERO_BUFFER_PST2 = 0;

bitmap_pos_struct_t* x_DWORD_EA3D4; // weak

filearray_struct filearray_2aa18c[] = {
	{ &POINTERSTAB_BEGIN_BUFFER,&POINTERSTAB_END_BUFFER,&POINTERSDAT_BEGIN_BUFFER,&posistruct1 },
{ &FONTS0TAB_BEGIN_BUFFER,&FONTS0TAB_END_BUFFER,&FONTS0DAT_BEGIN_BUFFER,&posistruct2 },
{ &FONTS1TAB_BEGIN_BUFFER,&FONTS1TAB_END_BUFFER,&FONTS1DAT_BEGIN_BUFFER,&posistruct3 },
{ &ZERO_BUFFER_PST2,&ZERO_BUFFER_PST2,&ZERO_BUFFER,&posistruct4 },
{ &MSPRD00TAB_BEGIN_BUFFER,&MSPRD00TAB_END_BUFFER,&MSPRD00DAT_BEGIN_BUFFER,&posistruct5 },
{ &ZERO_BUFFER_PST2,&ZERO_BUFFER_PST2,&ZERO_BUFFER,&posistruct6 },
{ &HFONT3TAB_BEGIN_BUFFER,&HFONT3TAB_END_BUFFER,&HFONT3DAT_BEGIN_BUFFER,&posistruct7 },
{ &ZERO_BUFFER_PST2,&ZERO_BUFFER_PST2,&ZERO_BUFFER,&posistruct8 },
{ &BUILD00TAB_BEGIN_BUFFER,&BUILD00TAB_END_BUFFER,&BUILD00DAT_BEGIN_BUFFER,&posistruct9 },
{ &ZERO_BUFFER_PST2,&ZERO_BUFFER_PST2,&ZERO_BUFFER,&posistruct10 },
{ &ZERO_BUFFER_PST2,&ZERO_BUFFER_PST2,&ZERO_BUFFER,&posistruct11 }
};

filearray_struct x_DWORD_E9B20[4] = { filearray_2aa18c[filearrayindex_FONTS0DATTAB],filearray_2aa18c[filearrayindex_FONTS1DATTAB],
filearray_2aa18c[filearrayindex_HFONT3DATTAB],filearray_2aa18c[filearrayindex_FONTS1DATTAB] };

uint8_t* SPELLS_BEGIN_BUFFER_ptr = (uint8_t*)SPELLS_BEGIN_BUFFER_str;
uint8_t* LANG_BEGIN_BUFFER = 0;

char* x_DWORD_D41BC_langbuffer; // fix it

Pathstruct xasearchd_2bac30 = { "*SearchD\0",(uint8_t**)&SEARCH_BEGIN_BUFFER,&SEARCH_END_BUFFER,0x1000,0 };
//#define psxasearchd_2bac30 0
Pathstruct xafonts0dat = { "DATA/FONT0.DAT\0",&FONTS0DAT_BEGIN_BUFFER,NULL,0,0 };
//#define psxafonts0dat 1
Pathstruct xafonts0tab = { "DATA/FONT0.TAB\0",(uint8_t**)&FONTS0TAB_BEGIN_BUFFER,(uint8_t**)&FONTS0TAB_END_BUFFER,0,0 };
//#define psxafonts0tab 2
Pathstruct xafonts1dat = { "DATA/FONT1.DAT\0",&FONTS1DAT_BEGIN_BUFFER,NULL,0,0 };
//#define psxafonts1dat 3
Pathstruct xafonts1tab = { "DATA/FONT1.TAB\0",(uint8_t**)&FONTS1TAB_BEGIN_BUFFER,(uint8_t**)&FONTS1TAB_END_BUFFER,0,0 };
//#define psxafonts1tab 4
Pathstruct xadatatmaps00tab = { "DATA/TMAPS0-0.TAB",(uint8_t**)&str_TMAPS00TAB_BEGIN_BUFFER,NULL,0,0 };
//#define psxadatatmaps00tab 5
Pathstruct xapaldata = { "*PalData\0",(uint8_t**)&str_PALDATA_BEGIN_BUFFER,NULL,0x400,0 };
//#define psxapaldata 6
Pathstruct xapalmem = { "*PalMem\0",(uint8_t**)&str_PALMEM_BEGIN_BUFFER,NULL,0,0 };
//#define psxapalmem 7
Pathstruct xadatapointersdat = { "DATA/POINTERS.DAT\0",&POINTERSDAT_BEGIN_BUFFER,NULL,0,0 };
//#define psxadatapointersdat 8
Pathstruct xadatapointerstab = { "DATA/POINTERS.TAB\0",(uint8_t**)&POINTERSTAB_BEGIN_BUFFER,(uint8_t**)&POINTERSTAB_END_BUFFER,0,0 };
//#define psxadatapointerstab 9
Pathstruct xazero = { "\0",NULL,NULL,0,0 };
//#define psxazero0 10
Pathstruct xadatabuild00dat = { "DATA/BUILD0-0.DAT\0",&BUILD00DAT_BEGIN_BUFFER,NULL,0,0 };
//#define psxadatabuild00dat 11
Pathstruct xadatabuild00tab = { "DATA/BUILD0-0.TAB\0",(uint8_t**)&BUILD00TAB_BEGIN_BUFFER,(uint8_t**)&BUILD00TAB_END_BUFFER,0,0 };
//#define psxadatabuild00tab 12
//zero
//#define psxazero1 13
Pathstruct xadataetextdat = { "DATA/ETEXT.DAT\0",&ETEXT_BEGIN_BUFFER,NULL,0,0 };
//#define psxadataetextdat 14
//zero
//#define psxazero2 15
Pathstruct xadataftextdat = { "DATA/FTEXT.DAT\0",&FTEXT_BEGIN_BUFFER,NULL,0,0 };
//#define psxadataftextdat 16
//zero
//#define psxazero3 17
Pathstruct xadatagtextdat = { "DATA/GTEXT.DAT\0",&GTEXT_BEGIN_BUFFER,NULL,0,0 };
//#define psxadatagtextdat 18
//zero
//#define psxazero4 19
Pathstruct xadataitextdat = { "DATA/ITEXT.DAT\0",&ITEXT_BEGIN_BUFFER,NULL,0,0 };
//#define psxadataitextdat 20
//zero
//#define psxazero5 21
Pathstruct xadatablock16dat = { "DATA/BLOCK16.DAT\0",&BLOCK16DAT_BEGIN_BUFFER,NULL,0,0 };
//#define psxadatablock16dat 22
Pathstruct xadatablock32dat = { "DATA/BLOCK32.DAT\0",&BLOCK32DAT_BEGIN_BUFFER,NULL,0,0 };
//#define psxadatablock32dat 23
//zero
//#define psxazero6 24
Pathstruct xabscreen = { "*BScreen\0",&BSCREEN_BEGIN_BUFFER,&BSCREEN_END_BUFFER,0x011508,0 };
//#define psxabscreen 25
Pathstruct xadatapald0dat = { "DATA/PALD-0.DAT\0",&PALD0DAT_BEGIN_BUFFER,NULL,0,0 };
//#define psxadatapald0dat 26
//zero
//#define psxazero7 27
Pathstruct xawscreen_351628 = { "*WScreen\0",&WSCREEN_BEGIN_BUFFER,&WSCREEN_END_BUFFER,0x04b100,0 };
//#define psxawscreen_351628 28
Pathstruct xabscreen2 = { "*BScreen\0",&x_DWORD_E9C38_smalltit,&BSCREEN2_END_BUFFER, 0x011508,0 };
//#define psxabscreen2 29

Pathstruct xadatapald0dat2 = { "DATA/PALD-0.DAT\0",&PALD0DAT2_BEGIN_BUFFER,NULL,0,0 };
//#define psxadatapald0dat2 30
//zero
//#define psxazero8 31
Pathstruct xadatamsprd00dat = { "DATA/MSPRD0-0.DAT\0",&MSPRD00DAT_BEGIN_BUFFER,NULL,0,0 };
//#define psxadatamsprd00dat 32
Pathstruct xadatamsprd00tab = { "DATA/MSPRD0-0.TAB\0",(uint8_t**)&MSPRD00TAB_BEGIN_BUFFER,(uint8_t**)&MSPRD00TAB_END_BUFFER,0,0 };
//#define psxadatamsprd00tab 33
//zero
//#define psxazero9 34
Pathstruct xadatahsprd00dat = { "DATA/HSPRD0-0.DAT\0",&HSPRD00DAT_BEGIN_BUFFER,NULL,0,0 };
//#define psxadatahsprd00dat 35
Pathstruct xadatahsprd00tab = { "DATA/HSPRD0-0.TAB\0",(uint8_t**)&HSPRD00TAB_BEGIN_BUFFER,(uint8_t**)&HSPRD00TAB_END_BUFFER,0,0 };
//#define psxadatahsprd00tab 36
Pathstruct xadatahfont3dat = { "DATA/HFONT3.DAT\0",&HFONT3DAT_BEGIN_BUFFER,NULL,0,0 };
//#define psxadatahfont3dat 37
Pathstruct xadatahfont3tab = { "DATA/HFONT3.TAB\0",(uint8_t**)&HFONT3TAB_BEGIN_BUFFER,(uint8_t**)&HFONT3TAB_END_BUFFER,0,0 };
//#define psxadatahfont3tab 38
//zero
//#define psxazero10 39
Pathstruct xadataclrd0dat = { "DATA/CLRD-0.DAT\0",&CLRD0DAT_BEGIN_BUFFER,NULL,0,0 };
//#define psxadataclrd0dat 40
//zero
//#define psxazero11 41
Pathstruct xadataspellsdatx = { "DATA/SPELLS.DAT\0",&SPELLS_BEGIN_BUFFER_ptr,NULL,0,0 };
//#define psxadataspellsdat 42
//zero
//#define psxazero12 43
Pathstruct xadatalang = { "",(uint8_t**)&x_DWORD_D41BC_langbuffer,&LANG_BEGIN_BUFFER,0,0 };
//#define psxadatalangbuffer 44
//zero
//#define psxazero13 45
Pathstruct xadatatables = { "",(uint8_t**)&x_DWORD_D41BC_langbuffer,&LANG_BEGIN_BUFFER,0,0 };
//#define psxadatalangbuffer2 46
//zero
//#define psxazero14 47

bool IsDefaultResolution320(int width, int height)
{
	return (width == 320 && height == 200);
}

bool IsDefaultResolution640(int width, int height)
{
	return (width == 640 && height == 480);
}

bool IsDefaultResolution(int width, int height)
{
	if ((IsDefaultResolution320(width, height)) ||
		 IsDefaultResolution640(width, height))
		return true;
	return false;
}

bool DefaultResolutions()
{
	if (IsDefaultResolution(screenWidth_18062C, screenHeight_180624))
		return true;
	return false;
}

//----- (00083E80) --------------------------------------------------------
void FreeMem_83E80(uint8_t* ptr)//264e80
{
	/*if (*ptr != NULL)
	{
		free((void*)*ptr);
		*ptr = NULL;
	}
	
	or

	free((void*)ptr);

	may must rewrite with garbage collector
	*/

	//

	//if (*a1)x_free((void*)*a1);
	//*a1 = NULL;
	//return result;
	/*
	uint8_t* v1; // eax
	char v2; // bl
	int* i; // ebx

	if (a1)
	{
		v1 = (uint8_t*)&x_DWORD_17ECA0;
		v2 = 0;
		while (v1)
		{
			if ((int)a1 == v1[0])//fix
			{
				v2 = 1;
				*((x_BYTE*)v1 + 16) = 0;
				break;
			}
			v1 = (uint8_t*)(int*)v1[2];
		}
		if (v2 == 1)
		{
			for (i = (int*)&x_DWORD_17ECA0; i; i = (int*)i[2])
			{
				if (!*((x_BYTE*)i + 16))
					sub_84000((int)i);
			}
		}
		//sub_85350(); //fix
	}*/
}
// 17ECA0: using guessed type int x_DWORD_17ECA0;

x_DWORD x_outp(x_DWORD, char) { stub_fix_it(); return 0; };// weak
x_DWORD x_inp(x_DWORD) { stub_fix_it(); return 0; };// weak
void stub_fix_it() { allert_error(); };

void dbgfprintf(FILE* file, const char* format, ...) {
	//void dbgfprintf(FILE* file,char* str) {
	fprintf(file, "%s", format);
	fprintf(file, "\n");
}

void* Malloc_83CD0(size_t a1)//264cd0
{
	return malloc(a1);
}

void qmemcpy(void* dest, void* src, size_t size) {
	memcpy(dest, src, size);
};

//----- (0009D490) --------------------------------------------------------
int FreeMem_9D490(void* a1, int  a2)//27e490
{
	//fix
	//a2 may be must used
	a2 = 0;//
	//fix
	int result; // eax

	if (a1)
	{
		//result = off_E3E34_freex(a1);
		result = x_free(a1);
	}
	return result;
	//fix it
	//return 0;
}
// E3E34: using guessed type int (*off_E3E34_freex)(int);

int x_free(void* ptr) { free(ptr); return 0; };

void __writegsx_WORD(unsigned long  /*Offset*/, unsigned short  /*Data*/) { stub_fix_it(); };
void __writegsx_DWORD(unsigned long  /*Offset*/, unsigned long  /*Data*/) { stub_fix_it(); };
unsigned long __readgsx_DWORD(unsigned long  /*Offset*/) { stub_fix_it(); return 0; };
unsigned char __readgsx_BYTE(unsigned long  /*Offset*/) { stub_fix_it(); return 0; };
unsigned short __readgsx_WORD(unsigned long  /*Offset*/) { stub_fix_it(); return 0; };
unsigned     int x__readeflags(void) { stub_fix_it(); return 0; };
void x__writeeflags(unsigned  /*Value*/) { stub_fix_it(); };
unsigned int x__getcallerseflags(void) { stub_fix_it(); return 0; };

x_DWORD dos_read(FILE*, char, x_DWORD, x_DWORD, char*) { stub_fix_it(); return 0; };// weak

//----- (0009D770) --------------------------------------------------------
int sub_9D770(char* a1, char a2)//27e770
{
	char v3; // [esp+0h] [ebp-E0h]
	int v4; // [esp+40h] [ebp-A0h]
	unsigned int v5; // [esp+44h] [ebp-9Ch]
	int v6; // [esp+ACh] [ebp-34h]
	int v7; // [esp+C4h] [ebp-1Ch]
	int v8; // [esp+C8h] [ebp-18h]
	unsigned int i; // [esp+CCh] [ebp-14h]
	FILE* v10; // [esp+D0h] [ebp-10h]
	int v11; // [esp+D4h] [ebp-Ch]
	int v12; // [esp+D8h] [ebp-8h]
	int v13; // [esp+DCh] [ebp-4h]

	//fix it
	v4 = 0;
	v5 = 0;
	//fix it

	v13 = 8224;
	v7 = 0;
	if (a2 & 1)
	{
		v10 = (FILE*)a1;
	}
	else
	{
		v10 = x_open(a1, 512);
		if (v10 == NULL)
			return 0;
	}
	v12 = sub_9D380(v10, 60, a2, (uint8_t*)&v8, 4);
	v12 = sub_9D380(v10, v8, a2, (uint8_t*)&v13, 2);
	if (!strcmp((const char*)&v13, "LX"))
	{
		sub_9D380(v10, v8, a2, (uint8_t*)&v3, 172);
		v12 = v4 + v8;
		for (i = 0; i < v5; i++)
		{
			v12 = sub_9D380(v10, v12, a2, (uint8_t*)&v6, 24);
			v7 += v6;
		}
		if (!(a2 & 1))
			DataFileIO::Close((FILE*)v10);
		v7 += 15 * v5;
		v11 = v7;
	}
	else
	{
		DataFileIO::Close((FILE*)v10);
		v11 = 0;
	}
	return v11;
}
// 9A050: using guessed type x_DWORD strcmp(x_DWORD, x_DWORD);
// A0855: using guessed type x_DWORD close(x_DWORD);

//----- (0009DE20) --------------------------------------------------------
long GetFileLenght_9DE20(char* filename)//27ee20
{
	int result;

	readFileStatus_E3E2C = 0;
	if (FILE* file = x_open(filename, 512); file == nullptr)
	{
		readFileStatus_E3E2C = 3;
		result = -1;
	}
	else
	{
		long fileLenght = DataFileIO::FileLengthBytes(file);
		if (fileLenght == -1)
			readFileStatus_E3E2C = 5;
		DataFileIO::Close(file);
		result = fileLenght;
	}
	return result;
}

FILE* x_open(char* path, int  /*pmodex*/) {
	return myopent(path, (char*)"rb");
};

//----- (0009D380) --------------------------------------------------------
int sub_9D380(FILE* a1, int a2, char a3, uint8_t* a4x, int a5)//27e380
{
	if (a3 & 1)
	{
#ifdef _WIN32
		// Windows FILE*
		memcpy((void*)a4x, (const void*)(uintptr_t(a1) + a2), a5);
#else
		// Android/Linux FILE*
		memcpy((void*)a4x, (const void*)((char*)a1 + a2), a5);
#endif
	}
	else
	{
		DataFileIO::Seek(a1, a2, 0);
		DataFileIO::Read(a1, a4x, a5);
	}
	return a5 + a2;
}
// 99DBD: using guessed type x_DWORD memcpy(x_DWORD, x_DWORD, x_DWORD);
// A0863: using guessed type x_DWORD read(x_DWORD, x_DWORD, x_DWORD);
// AA7C0: using guessed type x_DWORD lseek(x_DWORD, x_DWORD, char);

char x_toupper(char inputchar) {
	return toupper(inputchar);
}; //weak

//----- (00083D70) --------------------------------------------------------
void* Malloc_83D70(int a1)//264d70
{
	/*int *v1; // eax
	unsigned int v2; // edi
	int *v3; // ebx
	unsigned int v4; // esi
	unsigned int v5; // edx

	sub_85070();
	v1 = (int*)&x_DWORD_17ECA0;
	v2 = -1;
	v4 = a1 + 271;
	v3 = 0;
	LOWORD(v4) = (a1 + 271) & 0xFFF0;
	while ( v1 )
	{
	  v5 = v1[1];
	  if ( v4 <= v5 && v2 > v5 && !*((x_BYTE *)v1 + 16) && x_DWORD_17E0A4[3 * *((unsigned __int8 *)v1 + 17)] )
	  {
		v3 = v1;
		v2 = v1[1];
	  }
	  v1 = (int *)v1[2];
	}
	if ( !v3 || !sub_83E00((int)v3, v4) )
	  return 0;
	sub_85350();
	memset((void*)*v3, 0, v4);//problÃ©m
	return (void*)*v3;*/
	return malloc(a1);
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// 17E0A4: using guessed type int x_DWORD_17E0A4[];
// 17ECA0: using guessed type int x_DWORD_17ECA0;

x_DWORD x_tolower(x_DWORD) { stub_fix_it(); return 0; };// weak

void sub_2EC30_clear_img_mem()//20fc30
{
	if (SubtitlesOn_D41CE)
		memset((void*)x_DWORD_F01E4, 0, x_WORD_F01F4);
}

//----- (0002EB60) --------------------------------------------------------
void StartSubtitles_2EB60()//20fb60
{
	//int result; // eax

	if (!SubtitlesOn_D41CE)
	{
		SubtitlesOn_D41CE = 1;
		DisplaySubtitles_D41C1 = 1;
		x_DWORD_F01E4 = &pdwScreenBuffer_351628[64320];//line 320x201 - after normal image
		x_WORD_F01F4 = 0x3200;//320*40
		sub_2EC30_clear_img_mem();
		x_DWORD_D41D0 = 0;
	}
}

//----- (0002EBB0) --------------------------------------------------------
void sub_2EBB0_draw_text_with_border_630x340(char* textString)//20fbb0
{
	if (SubtitlesOn_D41CE)
	{
		x_DWORD_D41D0 = textString;
		x_WORD_E36D4 = 64;
		pdwScreenBuffer_351628 += 0x26C0;
		/*result = */sub_7FCB0_draw_text_with_border(/*64,*/ textString, 0, 630, 340, 5, x_BYTE_EB3B6, 0);
		x_WORD_E36D4 = 0;
		pdwScreenBuffer_351628 -= 0x26C0;
	}
	//return result;
}

//----- (0007FCB0) --------------------------------------------------------
void sub_7FCB0_draw_text_with_border(char* textString, int32_t a3, int32_t a4, int a5, uint8_t a6, unsigned __int8 a7, uint32_t a8)//260cb0
{
	int v8; // esi
	signed __int16 j; // di
	uint8_t v10; // eax
	int v11; // ebx
	uint8_t v12; // al
	int v13; // edi
	__int16 v14; // ST0C_2
	int v15; // eax
	int v16; // eax
	signed __int16 i; // di
	uint8_t v18; // eax
	uint8_t* v19; // ecx
	__int16 v20; // di
	uint8_t v21; // eax
	char v22; // al
	char v23; // ah
	char v24; // cl
	__int16 v25; // ax
	int v26; // esi
	unsigned __int8 v27; // al
	int v28; // esi
	__int16 v29; // ST0C_2
	int v30; // eax
	uint8_t* v31; // ecx
	signed __int16 v32; // ax
	__int16 v33; // si
	int v34; // eax
	__int16 v35; // ST0C_2
	__int16 v36; // ST08_2
	signed __int16 v37; // di
	uint8_t* v38; // ecx
	__int16 v39; // si
	unsigned __int16 v40; // ax
	__int16 v41; // di
	int v42; // eax
	int v43; // eax
	int v44; // eax
	unsigned __int8 v45; // al
	int v46; // esi
	__int16 v47; // ST0C_2
	int v48; // edi
	__int16 v49; // ST0C_2
	int v50; // eax
	int v51; // ebx
	unsigned __int8 v52; // al
	__int16 v53; // ST0C_2
	int v54; // ST08_4
	int v55; // eax
	signed __int16 v56; // si
	signed __int16 v57; // si
	uint8_t* v58x; // ecx
	int v59y; // eax
	__int16 v60; // ax
	int v61; // edi
	int v62; // eax
	int v63; // ebx
	uint8_t* v64; // ecx
	signed __int16 v65; // di
	__int16 v66; // si
	int v67; // eax
	signed __int16 v68; // si
	__int16 v69; // ST0C_2
	__int16 v70; // ST08_2
	int v71; // eax
	x_BYTE* v72; // ecx
	signed __int16 l; // si
	__int16 v74; // ax
	int v75; // esi
	int v76; // eax
	int v77; // ebx
	uint8_t* v78; // ecx
	signed __int16 v79; // si
	__int16 v80; // ax
	int v81; // eax
	signed __int16 v82; // si
	__int16 v83; // ST08_2
	int v84; // eax
	uint8_t v86; // [esp-4h] [ebp-FCh]
	char* v86b; // [esp-4h] [ebp-FCh]
	char v87[180]; // [esp+0h] [ebp-F8h]
	int v88; // [esp+B4h] [ebp-44h]
	int v89; // [esp+B8h] [ebp-40h]
	int v90; // [esp+BCh] [ebp-3Ch]
	int v91; // [esp+C0h] [ebp-38h]
	//int v92; // [esp+C4h] [ebp-34h]
	int32_t v93; // [esp+C8h] [ebp-30h]
	int32_t v94; // [esp+CCh] [ebp-2Ch]
	int v95; // [esp+D0h] [ebp-28h]
	int v96; // [esp+D4h] [ebp-24h]
	unsigned __int16 v97; // [esp+D8h] [ebp-20h]
	int v98; // [esp+DCh] [ebp-1Ch]
	int v99; // [esp+E0h] [ebp-18h]
	int32_t k; // [esp+E4h] [ebp-14h]
	int32_t v101; // [esp+E8h] [ebp-10h]
	int v102; // [esp+ECh] [ebp-Ch]
	int v103; // [esp+F0h] [ebp-8h]
	int32_t v104; // [esp+F4h] [ebp-4h]

	int a1 = 0;//fix

	//debug
	//loadfromsnapshot((char*)"0160-00260CB0", (uint8_t*)&v87, 0x354e80,180);//zbytecne
	/*loadfromsnapshot((char*)"0160-00260CB0", (uint8_t*)&a6, 0x354f6c, 1);
	loadfromsnapshot((char*)"0160-00260CB0", (uint8_t*)&a4, 0x354f64, 4);
	loadfromsnapshot((char*)"0160-00260CB0", (uint8_t*)&a3, 0x354f60, 4);
	loadfromsnapshot((char*)"0160-00260CB0", (uint8_t*)&a8, 0x354f74, 4);*/
	/*loadfromsnapshot2((char*)"0160-00260CB0", (uint8_t*)x_DWORD_17DED4, 0x34eed4, 1000);
	a2 = (uint8_t*)malloc(0x10000);
	loadfromsnapshot((char*)"0160-00260CB0", (uint8_t*)a2, 0x354f5c, 0x10000);*/
	//a5 = 0x12c;

	//debug

	//fix it
	v86 = a7;
	//fix it

	v8 = a5;
	v98 = a5;
	v102 = a5;
	v95 = 0;
	v103 = 0;
	v104 = 0;
	memset(v87, 0, 180);
	if (a6)
	{
		if (a6 == 2 || a6 == 5)
		{
			v104 = a4 - a3;
			if (a8)
			{
				v103 = xy_DWORD_17DED4_spritestr[275].width_4;
				v90 = xy_DWORD_17DED4_spritestr[275].width_4;
				v94 = v104 / v90;
				if (v104 % v90)
				{
					v16 = v103 * (v94++ + 1);
					v104 = v16;
					a4 = v16 + a3 - xy_DWORD_17DED4_spritestr[274].width_4;
				}
				for (i = 0; i < v104; i += v103)
					sub_7C120_draw_bitmap_640(i + a3, a5, xy_DWORD_17DED4_spritestr[275]);
				//HIWORD(v18) = HIWORD(xy_DWORD_17DED4_spritestr);
				v18 = xy_DWORD_17DED4_spritestr[275].height_5;
				v8 = v18 + a5;
				v19 = (pdwScreenBuffer_351628 + a3 + 640 * (v18 + a5));
				for (v93 = 0; v93 < xy_DWORD_17DED4_spritestr[274].height_5; v93++)
				{
					for (v20 = 0; v20 < a4 - (v90 + a3); v20++)
					{
						v90 = xy_DWORD_17DED4_spritestr[274].width_4;
						HIBYTE(v97) = 15;
						LOBYTE(v97) = *v19;
						v19++;
						*(v19 - 1) = ((uint8_t*)x_DWORD_17DE38str.x_DWORD_17DE3C)[v97];
					}
					v19 += 640 - v20;
				}
				sub_7C120_draw_bitmap_640(a3, v8, xy_DWORD_17DED4_spritestr[274]);
				sub_7C120_draw_bitmap_640(a4 - xy_DWORD_17DED4_spritestr[274].width_4, v8, xy_DWORD_17DED4_spritestr[274]);
			}
			if (a8)
			{
				//HIWORD(v21) = HIWORD(xy_DWORD_17DED4_spritestr);
				v21 = xy_DWORD_17DED4_spritestr[274].height_5;
				a1 = v21 + v8;
				v98 += v21;
			}
			else
			{
				v98 = v8;
			}
		}
	}
	else
	{//ramecek s textem
		v104 = a4 - a3;//adress 260cf1
		v88 = a8;
		//v92 = (int)xy_DWORD_17DED4_spritestr;
		v103 = xy_DWORD_17DED4_spritestr[a8].width_4;//10
		if ((a4 - a3) % v103)
		{
			v104 = ((a4 - a3) / v103 + 1) * v103;
			a4 = v104 + a3 - xy_DWORD_17DED4_spritestr[(v88 - 1)].width_4;
		}
		for (j = 0; j < v104; j += v103)//ramecek - horni vodorovna linka
			sub_7C120_draw_bitmap_640(j + a3, a5, xy_DWORD_17DED4_spritestr[a8]);//25d120

//HIWORD(v10) = HIWORD(xy_DWORD_17DED4_spritestr);
		v10 = xy_DWORD_17DED4_spritestr[a8].height_5;//adress 260da7
		v11 = v10 + a5;
		v98 += v10;
		v12 = getPaletteIndex_5BE80(x_DWORD_17DE38str.palette_17DE38x, 0, 0, 0/*v86*/);
		v86 = v12;
		v13 = 6 * (a8 - 1);
		v14 = xy_DWORD_17DED4_spritestr[v13 / 6].height_5;
		v91 = v11;
		v90 = a3;
		sub_7C140_draw_text_background(a3, v11, v104, v14, v12);//prvni cerna linka
		sub_7C120_draw_bitmap_640(v90, v91, xy_DWORD_17DED4_spritestr[v13 / 6]);//prvni levy svisly dilek
		sub_7C120_draw_bitmap_640(a4, v91, xy_DWORD_17DED4_spritestr[v13 / 6]);//prvni pravy svisly dilek
		//HIWORD(v15) = HIWORD(xy_DWORD_17DED4_spritestr);
		v15 = xy_DWORD_17DED4_spritestr[v13 / 6].height_5;
		a1 = v15 + v11;
		v98 += v15;
	}
	v99 = a3;
	if (!a6)//adress 261023
		v98 = v8 + 12;
	v96 = 0;
	v101 = 0;
	for (k = 0; ; k++)
	{
		v22 = 0;
		if (a6)
		{
			v24 = textString[k];
			if (v24 == ' ' || v24 == 0)
			{
			LABEL_38:
				v22 = 1;
				goto LABEL_39;
			}
		}
		else
		{
			v23 = textString[k];
			if (v23 == ' ' || v23 == 0 || v23 == ',' || v23 == '-' || v23 == '.')
				goto LABEL_38;
		}
	LABEL_39:
		if (v22)//space in text adress 26107c
		{
			if (v101)
			{
				if (a6 && a6 != 4 && a6 != 5)//adress 261091
				{
					GetFont_6FC50(1/*v86*/);
					v25 = GetLetterWidth_6FC10();
				}
				else
				{
					v25 = xy_DWORD_17DEC0_spritestr[65].width_4;
				}
				v89 = k;
				v26 = v99;
				if (v99 + v25 * (k - v96) <= a4 - 3 * v25)//adress 2610c2
				{
					v86b = &v87[strlen(v87)];
					qmemcpy(v86b, &textString[v101 + 1], v89 - v101);
				}
				else//width is higher then line size
				{
					if (a6 && a6 != 4)
					{
						if (a6 == 2)
						{
							GetFont_6FC50(1/*v86*/);
							v31 = &pdwScreenBuffer_351628[v26 + 640 * a1];
							for (v32 = 0; v32 < xy_DWORD_17DED4_spritestr[274].height_5; v32++)
							{
								for (v33 = 0; v33 < a4 - (v99 - xy_DWORD_17DED4_spritestr[274].width_4); v33++)
								{
									HIBYTE(v97) = 15;
									LOBYTE(v97) = *v31;
									v31++;
									*(v31 - 1) = ((uint8_t*)x_DWORD_17DE38str.x_DWORD_17DE3C)[v97];
								}
								v31 += 640 - v33;
							}
							sub_7C120_draw_bitmap_640(v99, a1, xy_DWORD_17DED4_spritestr[274]);
							sub_7C120_draw_bitmap_640(a4, a1, xy_DWORD_17DED4_spritestr[274]);
							//HIWORD(v34) = HIWORD(xy_DWORD_17DED4_spritestr);
							v34 = xy_DWORD_17DED4_spritestr[274].height_5;
							a1 += v34;
							v86 = a7;
							v35 = v98;
							v36 = a4 - 2 * GetLetterWidth_6FC10();
							sub_6FC80_pre_draw_text(v87, v99 + xy_DWORD_17DED4_spritestr[275].width_4, v36, v35, a7);
						}
						else if (a6 == 5)
						{
							if (a8)
							{
								v38 = &pdwScreenBuffer_351628[v99 + 640 * a1];
								for (v37 = 0; v37 < xy_DWORD_17DED4_spritestr[274].height_5; v37++)
								{
									for (v39 = 0; v39 < a4 - (xy_DWORD_17DED4_spritestr[274].width_4 + v99); v39++)
									{
										HIBYTE(v40) = 15;
										LOBYTE(v40) = *v38;
										v38++;
										*(v38 - 1) = ((uint8_t*)x_DWORD_17DE38str.x_DWORD_17DE3C)[v40];
									}
									v38 += 640 - v39;
								}
								v41 = v99;
								sub_7C120_draw_bitmap_640(v99, a1, xy_DWORD_17DED4_spritestr[274]);
								v86 = *xy_DWORD_17DED4_spritestr[274].data;
								sub_7C120_draw_bitmap_640(a4 - xy_DWORD_17DED4_spritestr[274].width_4, a1, xy_DWORD_17DED4_spritestr[274]);
								//HIWORD(v42) = HIWORD(xy_DWORD_17DED4_spritestr);
								v42 = xy_DWORD_17DED4_spritestr[274].height_5;
								a1 += v42;
								//2613b3
								DrawText_7FAE0(v87, v41, a4, v98, 0/*v86*/);
							}
							else
							{
								DrawText_7FAE0(v87, v99, a4, v98, v86);
							}
						}
						else
						{
							sub_6FC80_pre_draw_text(v87, v99, a4, v98, a7);
						}
					}
					else
					{
						if (!a6)
						{
							v27 = getPaletteIndex_5BE80(x_DWORD_17DE38str.palette_17DE38x, 0, 0, 0/*v86*/);
							v86 = v27;
							v28 = 6 * (a8 - 1);
							v29 = xy_DWORD_17DED4_spritestr[v28 / 6].height_5;
							v90 = v99;
							sub_7C140_draw_text_background(v99, a1, v104, v29, v27);
							sub_7C120_draw_bitmap_640(v90, a1, xy_DWORD_17DED4_spritestr[v28 / 6]);
							sub_7C120_draw_bitmap_640(a4, a1, xy_DWORD_17DED4_spritestr[v28 / 6]);
							//HIWORD(v30) = HIWORD(xy_DWORD_17DED4_spritestr);
							v30 = xy_DWORD_17DED4_spritestr[v28 / 6].height_5;
							a1 += v30;
						}
						//"click here" 12a 1e2 138 00
						DrawText_7FAE0(v87, v99, a4, v98, 0/*v86*/);//adress 261197 (80197)
					}
					if (a6 && a6 != 4 && a6 != 5)
					{
						LOWORD(v44) = GetLetterHeight_6FC30();
						v98 += v44 + 2;
					}
					else
					{
						//HIWORD(v43) = HIWORD(x_DWORD_17DEC0);
						v43 = xy_DWORD_17DEC0_spritestr[65].height_5;
						v98 += v43;
					}
					memset(v87, 0, 180);
					v86b = v87;
					qmemcpy(v87, &textString[v101 + 1], k - v101);
					v96 = v101;
				}
			}
			else
			{
				v86b = v87;
				qmemcpy(v87, textString, k + 1);//copy first text word
			}
			v101 = k;
		}
		if (!textString[k])
			break;
	}
	if (!v95)//adress 2614e2 (804e2) discoverwhich
	{
		if (a6 && a6 != 4)
		{
			GetFont_6FC50(1/*v86*/);
			if (a6 == 2)
			{
				GetFont_6FC50(1/*v86*/);
				v58x = &pdwScreenBuffer_351628[v99 + 640 * a1];
				v59y = 0;
				for (v57 = 0; v57 < xy_DWORD_17DED4_spritestr[274].height_5; v57++)
				{
					v58x += v59y;
					for (v60 = 0; v60 < a4 - (v99 - v91); v60++)
					{
						v91 = xy_DWORD_17DED4_spritestr[274].width_4;
						HIBYTE(v97) = 15;
						LOBYTE(v97) = *v58x;
						*v58x = ((uint8_t*)x_DWORD_17DE38str.x_DWORD_17DE3C)[v97];
						v58x++;
					}
					v59y = 640 - v60;
				}
				v61 = v99;
				sub_7C120_draw_bitmap_640(v99, a1, xy_DWORD_17DED4_spritestr[274]);
				sub_7C120_draw_bitmap_640(a4, a1, xy_DWORD_17DED4_spritestr[274]);
				//HIWORD(v62) = HIWORD(xy_DWORD_17DED4_spritestr);
				v62 = xy_DWORD_17DED4_spritestr[274].height_5;
				v63 = v62 + a1;
				GetFont_6FC50(1/*v86*/);
				v64 = &pdwScreenBuffer_351628[640 * v63 + v61];
				for (v65 = 0; v65 < xy_DWORD_17DED4_spritestr[274].height_5; v65++)
				{
					for (v66 = 0; v66 < a4 - (v99 - v91); v66++)
					{
						v91 = xy_DWORD_17DED4_spritestr[274].width_4;
						if (v66 >= a4 - (v99 - v91))
							break;
						HIBYTE(v97) = 15;
						LOBYTE(v97) = *v64;
						v64++;
						*(v64 - 1) = ((uint8_t*)x_DWORD_17DE38str.x_DWORD_17DE3C)[v97];
					}
					v64 += 640 - v66;
				}
				sub_7C120_draw_bitmap_640(v99, v63, xy_DWORD_17DED4_spritestr[274]);
				sub_7C120_draw_bitmap_640(a4, v63, xy_DWORD_17DED4_spritestr[274]);
				//HIWORD(v67) = HIWORD(xy_DWORD_17DED4_spritestr);
				v67 = xy_DWORD_17DED4_spritestr[274].height_5;
				v68 = 0;
				a1 = v67 + v63;
				while (v68 < v104)
				{
					sub_7C120_draw_bitmap_640(v68 + a3, a1, xy_DWORD_17DED4_spritestr[275]);
					v68 += v103;
				}
				v86 = a7;
				v69 = v98;
				v70 = a4 - 2 * GetLetterWidth_6FC10();
				sub_6FC80_pre_draw_text(v87, v99 + xy_DWORD_17DED4_spritestr[275].width_4, v70, v69, a7);
			}
			else if (a6 == 5)
			{
				if (a8)
				{
					v71 = 640 * a1 + v99;
					v72 = (x_BYTE*)pdwScreenBuffer_351628;
					for (l = 0; l < xy_DWORD_17DED4_spritestr[274].height_5; l++)
					{
						v72 += v71;
						v74 = 0;
						for (v74 = 0; v74 < a4 - (xy_DWORD_17DED4_spritestr[274].width_4 + v99); v74++)
						{
							HIBYTE(v97) = 15;
							LOBYTE(v97) = *v72;
							v72++;
							*(v72 - 1) = ((uint8_t*)x_DWORD_17DE38str.x_DWORD_17DE3C)[v97];
						}
						v71 = 640 - v74;
					}
					v75 = v99;
					sub_7C120_draw_bitmap_640(v99, a1, xy_DWORD_17DED4_spritestr[274]);
					v86 = *xy_DWORD_17DED4_spritestr[274].data;
					sub_7C120_draw_bitmap_640(a4 - xy_DWORD_17DED4_spritestr[274].width_4, a1, xy_DWORD_17DED4_spritestr[274]);
					//HIWORD(v76) = HIWORD(xy_DWORD_17DED4_spritestr);
					v76 = xy_DWORD_17DED4_spritestr[274].height_5;
					v77 = v76 + a1;
					v78 = &pdwScreenBuffer_351628[640 * v77 + v75];
					v79 = 0;
					for (v79 = 0; v79 < xy_DWORD_17DED4_spritestr[274].height_5; v79++)
					{
						v80 = 0;
						for (v80 = 0; v80 < a4 - (xy_DWORD_17DED4_spritestr[274].width_4 + v99); v80++)
						{
							HIBYTE(v97) = 15;
							LOBYTE(v97) = *v78;
							v78++;
							*(v78 - 1) = ((uint8_t*)x_DWORD_17DE38str.x_DWORD_17DE3C)[v97];
						}
						v78 += 640 - v80;
					}
					sub_7C120_draw_bitmap_640(v99, v77, xy_DWORD_17DED4_spritestr[274]);
					v86 = *xy_DWORD_17DED4_spritestr[274].data;
					sub_7C120_draw_bitmap_640(a4 - xy_DWORD_17DED4_spritestr[274].width_4, v77, xy_DWORD_17DED4_spritestr[274]);
					//HIWORD(v81) = HIWORD(xy_DWORD_17DED4_spritestr);
					v81 = xy_DWORD_17DED4_spritestr[274].height_5;

					a1 = v81 + v77;
					for (v82 = 0; v82 < v104; v82 += v103)
					{
						v83 = v82 + a3;
						sub_7C120_draw_bitmap_640(v83, a1, xy_DWORD_17DED4_spritestr[275]);
					}
					DrawText_7FAE0(v87, v99 + xy_DWORD_17DED4_spritestr[275].width_4, a4, v98, 0);
				}
				else
				{
					DrawText_7FAE0(v87, v99, a4, v98, v86);
				}
			}
			else
			{
				sub_6FC80_pre_draw_text(v87, v99, a4, v98, a7);
			}
		}
		else
		{
			if (!a6)
			{
				v45 = getPaletteIndex_5BE80(x_DWORD_17DE38str.palette_17DE38x, 0, 0, 0/*v86*/);
				v86 = v45;
				v46 = 6 * (a8 - 1);
				v47 = xy_DWORD_17DED4_spritestr[v46 / 6].height_5;
				v91 = v104;
				v48 = v99;
				v90 = a1;
				sub_7C140_draw_text_background(v99, a1, v104, v47, v45);//draw next black row
				sub_7C120_draw_bitmap_640(v48, v90, xy_DWORD_17DED4_spritestr[v46 / 6]);//draw next left column
				v49 = v90;
				v90 = a4;
				sub_7C120_draw_bitmap_640(a4, v49, xy_DWORD_17DED4_spritestr[v46 / 6]);//draw next right column
				//HIWORD(v50) = HIWORD(xy_DWORD_17DED4_spritestr);
				v50 = xy_DWORD_17DED4_spritestr[v46 / 6].height_5;
				v51 = v50 + a1;
				v52 = getPaletteIndex_5BE80(x_DWORD_17DE38str.palette_17DE38x, 0, 0, 0/*v86*/);//?
				v86 = v52;
				v53 = xy_DWORD_17DED4_spritestr[v46 / 6].height_5;
				v54 = v91;
				v91 = v51;
				sub_7C140_draw_text_background(v48, v51, v54, v53, v52);//draw next black row
				sub_7C120_draw_bitmap_640(v48, v91, xy_DWORD_17DED4_spritestr[v46 / 6]);//draw next left column
				sub_7C120_draw_bitmap_640(v90, v91, xy_DWORD_17DED4_spritestr[v46 / 6]);//draw next right column
				//HIWORD(v55) = HIWORD(xy_DWORD_17DED4_spritestr);
				v55 = xy_DWORD_17DED4_spritestr[v46 / 6].height_5;
				v56 = 0;
				a1 = v55 + v51;
				while (v56 < v104)
				{
					sub_7C120_draw_bitmap_640(v56 + v99, a1, xy_DWORD_17DED4_spritestr[a8]);//draw bottom row
					v56 += v103;
				}
			}
			DrawText_7FAE0(v87, v99, a4, v98, 0/*v86*/);//draw text(with palette?) 
		}
	}
	//if (a6)
	//	return;
	//HIWORD(v84) = HIWORD(xy_DWORD_17DED4_spritestr);
	//v84 = xy_DWORD_17DED4_spritestr[(a8 + 2)].height_5;

	//debug
	//compare_with_snapshot((char*)"0x45678", (uint8_t*)&v8, 0x4589, 4);
	//debug

}

//----- (0007C120) --------------------------------------------------------
void sub_7C120_draw_bitmap_640(int16_t posx, int16_t posy, bitmap_pos_struct_t tempstr)//25d120
{
	DrawBitmap_2BB40(posx, posy, tempstr);//ebp
}

//----- (0005BE80) --------------------------------------------------------
uint8_t getPaletteIndex_5BE80(TColor* Palettex, uint8_t red_color, uint8_t green_color, uint8_t blue_color)//23ce80
{
	uint16_t count_of_colors; // edx
	int16_t oldPalAmbient; // ecx
	int16_t newPalAmbient; // esi
	uint8_t result=0; // [esp+4h] [ebp-4h]
	uint16_t Palette_index = 0;
	if (x_WORD_180660_VGA_type_resolution & 6)
		count_of_colors = 16;
	else
		count_of_colors = 256;
	oldPalAmbient = 9999;
	for (uint16_t i = 0; i < count_of_colors; i++)
	{
		newPalAmbient = (red_color - Palettex[Palette_index].red) * (red_color - Palettex[Palette_index].red) +
			(green_color - Palettex[Palette_index].green)* (green_color - Palettex[Palette_index].green)+
			(blue_color - Palettex[Palette_index].blue)* (blue_color - Palettex[Palette_index].blue);
		if (newPalAmbient < oldPalAmbient)
		{
			oldPalAmbient = newPalAmbient;
			result = i;
		}
		Palette_index++;
	}
	return result;
}

//----- (0007C140) --------------------------------------------------------
void sub_7C140_draw_text_background(int16_t x1, int16_t y1, int16_t x2, int16_t y2, unsigned __int8 a5)//25d140
{//black line in text
	int16_t x1d; // si
	int16_t y1d; // bx
	int16_t x2d; // eax
	int16_t y2d; // dx
	//int16_t v9; // cx

	x1d = x1;
	y1d = y1;
	x2d = x2;
	y2d = y2;
	if (x1 < 640 && y1 < 480)//neni vetsi nez obrazovka
	{
		if (y1 + y2 > 480)
			y2d = 480 - y1;
		if (x1 + x2 > 640)
			x2d = 640 - x1;
		if (x2d >= 1 && y2d >= 1)
		{
			if (y1 < 0)
			{
				y2d += y1;
				x1d = 0;
			}
			if ((x1 & 0x8000u) != 0)
			{
				x2d += x1;
				x1d = 0;
			}
			//v9 = x2d;
			//x2d += x1d;
			if (x2d + x1d >= 1)
			{
				//x2d = y1d;
				if (y1d + y2d >= 1)
					DrawLine_2BC80(x1d, y1d, x2d, y2d, a5);
			}
		}
	}
}

//----- (0006FC50) --------------------------------------------------------
void GetFont_6FC50(__int16 a1)//250c50 //font and graphics init
{
	//int result; // eax
	bitmap_pos_struct_t* v2; // edx

	//result = a1;
	v2 = *x_DWORD_E9B20[a1].posistruct;
	if (!v2->data)
		v2 = *x_DWORD_E9B20[0].posistruct;
	x_DWORD_EA3D4 = v2;
	//return result * 4;
}
// E9B20: using guessed type int x_DWORD_E9B20[];
// EA3D4: using guessed type int x_DWORD_EA3D4;

//----- (0006FC10) --------------------------------------------------------
uint8_t GetLetterWidth_6FC10()//250c10
{
	if (help_VGA_type_resolution != 8)//fixed
		return x_DWORD_EA3D4[33].width_4;
	else
		return posistruct7[33].width_4;
}
// EA3D4: using guessed type int x_DWORD_EA3D4;

//----- (0006FC80) --------------------------------------------------------
unsigned int sub_6FC80_pre_draw_text(char* a1, __int16 a2, __int16 a3, __int16 a4, unsigned __int8 a5)//00250C80
{
	unsigned int v5; // kr04_4
	int v6; // esi
	int v7; // eax
	__int16 v9; // [esp+0h] [ebp-4h]

	v9 = a3 - a2;
	v5 = strlen(a1) + 1;
	if (v5 == 1)
	{
		v6 = a2 + v9 / 2;
	}
	else
	{
		v6 = a2 + v9 / 2 - (v5 - 1) * GetLetterWidth_6FC10() / 2;
		DrawText_2BC10(a1, v6, a4, a5);
	}
	v7 = GetLetterWidth_6FC10();
	return v6 + v7 * strlen(a1);
}

//----- (000417A0) --------------------------------------------------------
void sub_417A0_install_pal_and_mouse_minmax()//2227a0
{
	//sub_90810();
	sub_41A90_VGA_Palette_install((TColor*)*xadatapald0dat2.colorPalette_var28);
	sub_6EF10_set_mouse_minmax(0, 640, 0, 400);
}
// EA3D8: using guessed type int *xadatapald0dat2.colorPalette_var28;

//----- (0002EC90) --------------------------------------------------------
void sub_2EC90(char a1)//20fc90
{
	//char result; // al

	if (SubtitlesOn_D41CE)
	{
		//result = a1;
		x_BYTE_EB3B6 = a1;
		if (x_DWORD_D41D0)
			/*result = */sub_2EBB0_draw_text_with_border_630x340(x_DWORD_D41D0);
	}
	//return result;
}
// D41CE: using guessed type char SubtitlesOn_D41CE;
// D41D0: using guessed type int x_DWORD_D41D0;
// EB3B6: using guessed type char x_BYTE_EB3B6;

//----- (0007FAE0) --------------------------------------------------------
uint32_t DrawText_7FAE0(char* text, int16_t x, int16_t width, int16_t posy, uint8_t color)//260ae0
{
	int32_t posx;
	uint32_t helpstrlen = strlen(text);
	int16_t revWidth = width - x;
	if (helpstrlen == 1)
	{
		posx = x + revWidth / 2;
	}
	else
	{
		posx = revWidth / 2 + x - xy_DWORD_17DEC0_spritestr[65].width_4 * helpstrlen / 2;
		DrawText_7FB90(text, posx, posy, color); //"click here to" 13d 138 0
	}
	return posx + xy_DWORD_17DEC0_spritestr[65].width_4 * strlen(text);
}

//int debugcounter_271478 = 0;
//----- (00090478) --------------------------------------------------------
void sub_90478_VGA_Blit320(uint8_t maxFps)//271478
{
	if (CommandLineParams.DoDebugSequences()) {
		/* uint8_t origbyte20 = 0;
		uint8_t remakebyte20 = 0;
		int comp20;
		if (debugafterload)
		{
			//comp20 = compare_with_sequence((char*)"0022860F-002DC4E0", (uint8_t*)x_BYTE_10B4E0_terraintype, 0x2dc4e0, debugcounter_47560, 0x70000, 0x10000, &origbyte20, &remakebyte20);
			//comp20 = compare_with_sequence((char*)"0022860F-002DC4E0", (uint8_t*)x_BYTE_11B4E0_height, 0x2dc4e0, debugcounter_47560, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x10000);
			//comp20 = compare_with_sequence((char*)"0022860F-002DC4E0", (uint8_t*)x_BYTE_12B4E0_shading, 0x2dc4e0, debugcounter_47560, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x20000);
			//comp20 = compare_with_sequence((char*)"0022860F-002DC4E0", (uint8_t*)x_BYTE_13B4E0_angle, 0x2dc4e0, debugcounter_47560, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x30000);
			//comp20 = compare_with_sequence((char*)"0022860F-002DC4E0", (uint8_t*)x_WORD_15B4E0_source, 0x2dc4e0, debugcounter_47560, 0x70000, 0x20000, &origbyte20, &remakebyte20, 0x50000);

			//comp20 = compare_with_sequence_D41A0((char*)"0022860F-00356038", (uint8_t*)&D41A0_BYTESTR_0, 0x356038, debugcounter_271478, 224790, &origbyte20, &remakebyte20);

			//comp20 = compare_with_sequence_array_E2A74((char*)"0022860F-002B3A74", (uint8_t*)&array_E2A74, 0x2b3a74, debugcounter_271478, 0xc4e, 0xc4e, &origbyte20, &remakebyte20);
			//if(debugcounter_271478>5)
			//comp20 = compare_with_sequence((char*)"0022860F-003AA0A4", pdwScreenBuffer_351628, 0x3aa0a4, debugcounter_271478, 320 * 200, 320 * 200, &origbyte20, &remakebyte20);

			debugcounter_271478++;
		} */
	}
	if (!x_BYTE_E3766)
		sub_8CACD_draw_cursor2();

	std::chrono::duration<double, std::milli> timeDelta = CalculateTimeDelta();

#ifndef debug_hide_graphics
#if _DEBUG
	VGA_CalculateAndPrintFps(0, 0, timeDelta.count());
#endif
	VGA_Blit(pdwScreenBuffer_351628);
#endif
	//if(dos_key_vect_9)dos_key_vect_9();
	//VGA_mouse_clear_keys();

	//set speed
	LockFps(maxFps);
	//set speed
}

int debugcounter_258350 = 0;
//----- (00075200) --------------------------------------------------------
//long sub_75200_VGA_Blit640_index= 0;
int debugcounter_256200 = 0;
//long oldmillis = 0;
void sub_75200_VGA_Blit640(uint16_t height, uint8_t maxFps)//256200
{
	/*if (debugcounter_258350 > 0)
	{
		uint8_t origbyte20 = 0;
		uint8_t remakebyte20 = 0;
		int debugcounter11 = 0;
		int comp20;

		//comp20 = compare_with_sequence((char*)"00256200-003AA0A4", pdwScreenBuffer_351628, 0x3aa0a4, debugcounter_256200, 320 * 200, 320 * 200, &origbyte20, &remakebyte20);

		debugcounter_256200++;
	}*/

	if (!x_BYTE_E3766)
		sub_8CACD_draw_cursor2();//26dacd

	std::chrono::duration<double, std::milli> timeDelta = CalculateTimeDelta();
#if _DEBUG
	VGA_CalculateAndPrintFps(0, 0, timeDelta.count());
#endif
	VGA_Blit(pdwScreenBuffer_351628);

	//set speed
	LockFps(maxFps);
	//set speed
}

void VGA_BlitAny(uint8_t maxFps)//256200
{
	if (!x_BYTE_E3766)
		sub_8CACD_draw_cursor2();

	std::chrono::duration<double, std::milli> timeDelta = CalculateTimeDelta();
#if _DEBUG
	VGA_CalculateAndPrintFps(0, 0, timeDelta.count());
	VGA_DrawPlayerCoordData(0, 16);
#endif
	VGA_Blit(pdwScreenBuffer_351628);

	//set speed
	LockFps(maxFps);
	//set speed
}

//----- (0006FC30) --------------------------------------------------------
uint8_t GetLetterHeight_6FC30()//250c30
{
	if (help_VGA_type_resolution != 8)//fixed
		return x_DWORD_EA3D4[34].height_5;
	else
		return posistruct7[34].height_5;
}
// EA3D4: using guessed type int x_DWORD_EA3D4;

//----- (0002BB40) --------------------------------------------------------
void DrawBitmap_2BB40(int16_t posx, int16_t posy, bitmap_pos_struct_t tempposstr, uint8_t scale)//20cb40
{
	uint8_t* temp_screen_buffer;
	if (x_WORD_180660_VGA_type_resolution & 1)
		drawBitmap320_8F8B0(posx, posy, tempposstr);
	else
		drawBitmap640_8F8E8(posx, posy, tempposstr, scale);
	if (D41A0_0.m_GameSettings.m_Display.m_uiScreenSize == 1)
	{
		temp_screen_buffer = pdwScreenBuffer_351628;
		pdwScreenBuffer_351628 = x_DWORD_E9C3C;
		if (x_WORD_180660_VGA_type_resolution & 1)
			drawBitmap320_8F8B0(posx, posy, tempposstr);
		else
			drawBitmap640_8F8E8(posx, posy, tempposstr, scale);
		pdwScreenBuffer_351628 = temp_screen_buffer;
	}
}

//----- (00075D70) --------------------------------------------------------
void CopyAndShiftFrom17DB50_75D70(uint8_t* dest, uint32_t count)//256d70
{
	if (dest)
		qmemcpy(dest, (void*)x_DWORD_17DB50, count);
	x_DWORD_17DB50 += count;
}

//----- (0002BC80) --------------------------------------------------------
void DrawLine_2BC80(int16_t posStartX, int16_t posStartY, int16_t posEndX, int16_t posEndY, uint8_t colorIdx)//20cc80
{
	uint8_t* temp_screen_buffer; // ST14_4

	if (x_WORD_180660_VGA_type_resolution & 1)
		DrawLineLowRes_90164(posStartX, posStartY, posEndX, posEndY, colorIdx);
	else
		DrawLineHighRes_901E4(posStartX, posStartY, posEndX, posEndY, colorIdx);

	if (D41A0_0.m_GameSettings.m_Display.m_uiScreenSize == 1)
	{
		temp_screen_buffer = pdwScreenBuffer_351628;
		pdwScreenBuffer_351628 = x_DWORD_E9C3C;
		if (x_WORD_180660_VGA_type_resolution & 1)
			DrawLineLowRes_90164(posStartX, posStartY, posEndX, posEndY, colorIdx);
		else
			DrawLineHighRes_901E4(posStartX, posStartY, posEndX, posEndY, colorIdx);

		pdwScreenBuffer_351628 = temp_screen_buffer;
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// E9C3C: using guessed type int x_DWORD_E9C3C;
// 180628: using guessed type int pdwScreenBuffer_351628;
// 180660: using guessed type __int16 x_WORD_180660_VGA_type_resolution;

//----- (0002BC10) --------------------------------------------------------
void DrawText_2BC10(const char* textbuffer, int16_t posx, int16_t posy, uint8_t color, uint8_t scale)//20cc10
{
	sub_6F940_sub_draw_text(textbuffer, posx, posy, color, scale);
	if (D41A0_0.m_GameSettings.m_Display.m_uiScreenSize == 1)//shifted graphics
	{
		uint8_t* temp_screen_buffer = pdwScreenBuffer_351628;
		pdwScreenBuffer_351628 = x_DWORD_E9C3C;
		sub_6F940_sub_draw_text(textbuffer, posx, posy, color, scale);
		pdwScreenBuffer_351628 = temp_screen_buffer;
	}	
}

void SetFrameStart(std::chrono::system_clock::time_point frameStart)
{
	m_frameStart = frameStart;
}

std::chrono::duration<double, std::milli> CalculateTimeDelta()
{
	std::chrono::system_clock::time_point currTime = std::chrono::system_clock::now();
	std::chrono::duration<double, std::milli> timeDelta = (currTime - m_lastFrameEnd) * 0.001f;
	m_lastFrameEnd = currTime;
	return timeDelta;
}

void VGA_CalculateAndPrintFps(int x, int y, float timeDelta)
{
	//Caculate FPS
	m_iFrameCount++;
	m_fTimeElapsed += timeDelta;

	if (m_fTimeElapsed >= 1.0f)
	{
		m_fFps = (float)m_iFrameCount / m_fTimeElapsed;
		m_fTimeElapsed = 0.0f;
		m_iFrameCount = 0;
	}

	VGA_GotoXY(x, y);
	std::string fpsStr = "FPS: ";
	fpsStr.append(std::to_string(std::round(m_fFps*10)/10).substr(0,5));

	VGA_Draw_stringXYtoBuffer(fpsStr.c_str(), x, y, pdwScreenBuffer_351628);
}

void VGA_DrawPlayerCoordData(int x, int y)
{
	if (Entities_EA3E4 != nullptr && Entities_EA3E4[D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240] != nullptr) {

		int16_t index = D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].ActPlayerIndex_0x00e_2BDE_11244 + 1;
		axis_3d axisData = D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].struct_0x1d1_2BDE_11695[index].axis_2BDE_11695;
		axis_4d rotData = D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].struct_0x1d1_2BDE_11695[index].rotation__2BDE_11701;

		std::string playerCoordStr = "X: " + std::to_string(axisData.x) +
			" Y: " + std::to_string(axisData.y) +
			" Z: " + std::to_string(axisData.z);

		VGA_Draw_stringXYtoBuffer(playerCoordStr.c_str(), x, y, pdwScreenBuffer_351628, 'S');

		std::string playerRotationStr = "Pitch: " + std::to_string(rotData.pitch) +
			" Roll: " + std::to_string(rotData.roll) +
			" Yaw: " + std::to_string(rotData.yaw);

		VGA_Draw_stringXYtoBuffer(playerRotationStr.c_str(), x, y + 8, pdwScreenBuffer_351628, 'S');
#if 0
		axis_2d mouseData = x_WORD_E3760_mouse;
		std::string mouseCoordStr = "x: " + std::to_string(mouseData.x) + " y: " + std::to_string(mouseData.y);
		VGA_Draw_stringXYtoBuffer(mouseCoordStr.c_str(), x, y + 16, pdwScreenBuffer_351628, 'S');
#endif
	}
}

void LockFps(uint8_t maxFps)
{
	if (maxFps <= 0)
		return;

	std::chrono::duration<double, std::milli> renderTimeMS = std::chrono::system_clock::now() - m_frameStart;

	//lock to fps
	float frameTimeMS = 1000.0f / maxFps;
	if (renderTimeMS.count() > 0 && renderTimeMS.count() < frameTimeMS)
	{
		mydelay(frameTimeMS - renderTimeMS.count());
	}
}

// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// E9C3C: using guessed type int x_DWORD_E9C3C;
// 180628: using guessed type int pdwScreenBuffer_351628;

//----- (0006EF10) --------------------------------------------------------
void sub_6EF10_set_mouse_minmax(__int16 a1, signed __int16 a2, __int16 a3, signed __int16 a4)//24ff10
{
	signed __int16 v4; // si
	signed __int16 v5; // bx
	//__int16 result; // ax
	char v7ar[28]; // [esp+0h] [ebp-38h]
	__int16 v8ar[14]; // [esp+1Ch] [ebp-1Ch]
	//__int16 v9; // [esp+24h] [ebp-14h]//v8ar[4]
	//__int16 v10; // [esp+28h] [ebp-10h]//v8ar[6]

	v4 = a2;
	v5 = a4;
	memset(v8ar, 0, 28);
	memset(v7ar, 0, 28);
	if (a2 > 638)
		v4 = 638;
	//result = x_WORD_180660_VGA_type_resolution;
	if ((unsigned __int16)x_WORD_180660_VGA_type_resolution >= 1u)
	{
		if ((unsigned __int16)x_WORD_180660_VGA_type_resolution <= 1u)
		{
			v8ar[4] = a1;
			v8ar[6] = v4;
			v8ar[0] = 7;
//removed int386(0x33, (REGS*)v8ar, (REGS*)v7ar);//set mouse min,max see: https://github.com/open-watcom/open-watcom-v2/blob/master/bld/src/goodies/mouse.c
			v8ar[0] = 8;
			v8ar[4] = a3;
			if (a4 > 398)
				v5 = 398;
		}
		else
		{
			if (x_WORD_180660_VGA_type_resolution != 8)
				return;
			v8ar[0] = 7;
			v8ar[4] = 8 * a1;
			v8ar[6] = 8 * v4;
//removed int386(0x33, (REGS*)v8ar, (REGS*)v7ar);//set mouse min,max see: https://github.com/open-watcom/open-watcom-v2/blob/master/bld/src/goodies/mouse.c
			v8ar[0] = 8;
			if (a4 > 478)
				v5 = 478;
			v8ar[4] = 8 * a3;
			v5 *= 8;
		}
		v8ar[6] = v5;
//removed result = int386(0x33, (REGS*)v8ar, (REGS*)v7ar);//set mouse min,max see: https://github.com/open-watcom/open-watcom-v2/blob/master/bld/src/goodies/mouse.c
	}
	//return;
}

//----- (0007FB90) --------------------------------------------------------
void DrawText_7FB90(char* textbuffer, int16_t posx, int16_t posy, uint8_t color)//260b90
{
	uint8_t* temptextbuffer = (uint8_t*)textbuffer;
	int16_t tempposx = posx;
	int16_t tempposy = posy;
	if (color)
		x_WORD_E36D4 = 64;
	while (temptextbuffer[0] && tempposx < 640)
	{
		if (temptextbuffer[0] < 0xAu) //formating char
		{
			if (!temptextbuffer[0])
			{
				temptextbuffer++;
				continue;
			}
			if (temptextbuffer[0] == 0x9u) //tab char
			{
				tempposx += xy_DWORD_17DEC0_spritestr[65].width_4;
				temptextbuffer++;
				continue;
			}
			if (temptextbuffer[0])//not end char
			{
				if (color)
					sub_72C40_draw_bitmap_640_setcolor(tempposx, tempposy, xy_DWORD_17DEC0_spritestr[temptextbuffer[0]], color);
				else
					sub_7C120_draw_bitmap_640(tempposx, tempposy, xy_DWORD_17DEC0_spritestr[temptextbuffer[0]]);
			}
			tempposx += xy_DWORD_17DEC0_spritestr[65].width_4;
			temptextbuffer++;
			continue;
		}
		if (temptextbuffer[0] <= 0xAu) // formating char
		{
			tempposx = posx;
			tempposy += xy_DWORD_17DEC0_spritestr[65].height_5;
			{
				temptextbuffer++;
				continue;
			}
		}
		if (temptextbuffer[0] == 0xDu) // formating char cariage return
		{
			temptextbuffer++;
			continue;
		}
		if ((temptextbuffer[0] < 0xDu) || (temptextbuffer[0] != 32)) // formating char
		{
			if (temptextbuffer[0])//not end char
			{
				if (color)
					sub_72C40_draw_bitmap_640_setcolor(tempposx, tempposy, xy_DWORD_17DEC0_spritestr[temptextbuffer[0]], color);
				else
					sub_7C120_draw_bitmap_640(tempposx, tempposy, xy_DWORD_17DEC0_spritestr[temptextbuffer[0]]);
			}
			tempposx += xy_DWORD_17DEC0_spritestr[65].width_4;
			temptextbuffer++;
			continue;
		}
		tempposx += xy_DWORD_17DEC0_spritestr[65].width_4;
		temptextbuffer++;
	}
	x_WORD_E36D4 = 0;
}

//----- (0008CACD) --------------------------------------------------------
void sub_8CACD_draw_cursor2()//26dacd
{
	if (x_DWORD_E3768)
	{
		x_DWORD_E3758 = 1;
		DrawCursor_8C635();
	}
}
// E3758: using guessed type int x_DWORD_E3758;
// E3768: using guessed type int x_DWORD_E3768;

//----- (0008F8B0) --------------------------------------------------------
//void drawBitmap320_8F8B0(__int16 a1, __int16 a2, Pathstruct a3)//2708B0
void drawBitmap320_8F8B0(int16_t posx, int16_t posy, bitmap_pos_struct_t temppstr, uint8_t scale)//2708B0
{
	//int v4; // [esp+4h] [ebp-4h]

	//fix it
	//v4 = 0;
	//fix it

	sub_8F935_bitmap_draw_final(temppstr.width_4, temppstr.height_5, posy, posx, temppstr.data, 0, 0, 1);//270935 // a2 je 86
	//return v4;
}
// 180628: using guessed type int pdwScreenBuffer_351628;

//----- (0008F8E8) --------------------------------------------------------
//void drawBitmap640_8F8E8(__int16 a1, __int16 a2, uint8_t* a3)//2708e8
void drawBitmap640_8F8E8(int16_t posx, int16_t posy, bitmap_pos_struct_t temppstr, uint8_t scale)//2708e8
{
	//int v4; // [esp+4h] [ebp-4h]

	//fix it
	//v4 = 0;
	//fix it

	  //esi=48c995 //buffer
	  //ebx=182
	  //ecx=121
	  //edi=3aa0a4
	  //dx=09 0b
	  //esiÂ´48913e
	  //0
	  //0

	  //123 cd 48c80f
	  //2708e8 354f68
	  /*
	  esi=[ebp+10]//48c80f
	  ebx=[ebp+8]//123 - nic
	  ecx=[ebp+c]//cd - a2?
	  edi=[351628]//3aa0a4
	  dl=[esi+4]//26
	  dh=[esi+5]//33
	  esi=[esi]//47ae48
	  */
	  //x_DWORD_180628b
	  //xasearchd_2bac30.colorPalette_var28
	  //sub_8F935_bitmap_draw_final(doublebyte_conv(xasearchd_2bac30.colorPalette_var28, a2, a1, a3, 0, 0);//270935
	sub_8F935_bitmap_draw_final(temppstr.width_4, temppstr.height_5, posy, posx, temppstr.data, 0, 0, scale);//270935
  //return v4;
}
// 180628: using guessed type int pdwScreenBuffer_351628;

//----- (00090164) --------------------------------------------------------
void DrawLineLowRes_90164(int16_t posStartX, int16_t posStartY, int16_t posEndX, int16_t posEndY, uint8_t colorIdx)
{
	uint8_t* pixel; // edi
	uint16_t v6; // dx
	uint8_t v7; // ebx
	int v8; // esi
	int v9; // ecx
	uint16_t v10; // [esp+0h] [ebp-4h]
	uint16_t v11; // [esp+14h] [ebp+10h]

	pixel = &pdwScreenBuffer_351628[320 * (posStartY >> 1) + (posStartX >> 1)];
	v6 = posEndY >> 1;
	v11 = posEndX >> 1;
	v10 = (320 - v11);
	if (x_WORD_E36D4 & 4)
	{
		v7 = colorIdx;
		v8 = x_DWORD_E3890;
		do
		{
			v9 = v11;
			do
			{
				v7 = pixel[0];
				*pixel++ = *(uint8_t*)(v8 + v7);
				--v9;
			} while (v9);
			pixel += v10;
			--v6;
		} while (v6);
	}
	else
	{
		do
		{
			memset(pixel, colorIdx, v11);
			pixel += v10 + v11;
			--v6;
		} while (v6);
	}
}
// E36D4: using guessed type __int16 x_WORD_E36D4;
// E3890: using guessed type int x_DWORD_E3890;
// 180628: using guessed type int pdwScreenBuffer_351628;

//----- (000901E4) --------------------------------------------------------
void DrawLineHighRes_901E4(int16_t posStartX, int16_t posStartY, int16_t posEndX, int16_t posEndY, uint8_t colorIdx)//2711e4
{
	x_BYTE* v5; // edi
	__int16 v6; // dx
	int v7; // ebx
	int v8; // esi
	int v9; // ecx
	int v10; // [esp+0h] [ebp-4h]

	int helpWidth = 640;
	if (x_WORD_180660_VGA_type_resolution != 1)
		if (!DefaultResolutions())
			helpWidth = screenWidth_18062C;

	v5 = (x_BYTE*)(helpWidth * posStartY + pdwScreenBuffer_351628 + posStartX);
	v6 = posEndY;
	v10 = (unsigned __int16)(helpWidth - posEndX);
	if (x_WORD_E36D4 & 4)
	{
		v7 = colorIdx;
		v8 = x_DWORD_E3890;
		do
		{
			v9 = posEndX;
			do
			{
				BYTE1(v7) = *v5;
				*v5++ = *(x_BYTE*)(v7 + v8);
				v9--;
			} while (v9);
			v5 += v10;
			v6--;
		} while (v6);
	}
	else
	{
		do
		{
			memset(v5, colorIdx, posEndX);
			v5 += v10 + posEndX;
			v6--;
		} while (v6);
	}
}
// E36D4: using guessed type __int16 x_WORD_E36D4;
// E3890: using guessed type int x_DWORD_E3890;
// 180628: using guessed type int pdwScreenBuffer_351628;

//----- (0006F940) --------------------------------------------------------
void sub_6F940_sub_draw_text(const char* textbuffer, int posx, int posy, uint8_t color, uint8_t scale)//250940
{
	uint8_t* v4; // esi
	int v5; // ebx
	unsigned __int16 v6; // ax
	int v7; // eax
	//int v8; // edi
	int v9; // eax
	//int result; // eax

	int helpWidth = 640;

	if (x_WORD_180660_VGA_type_resolution != 1)
		if (!DefaultResolutions())
			helpWidth = screenWidth_18062C;

	v4 = (uint8_t*)const_cast<char*>(textbuffer); // FIXME: temporary const cast hack
	v5 = posx;
	x_WORD_E36D4 = 64;
	while (*v4 && v5 < helpWidth)
	{
		v6 = (unsigned __int8)*v4;
		if (v6 < 0xAu)
		{
			if (!v4[0])
				goto LABEL_16;
			if (v6 == 9)
				goto LABEL_14;
		}
		else
		{
			if (v6 <= 0xAu)
			{
				LOWORD(v7) = GetLetterHeight_6FC30() * scale;
				v5 = posx;
				posy += v7;
				goto LABEL_16;
			}
			if (v6 >= 0xDu)
			{
				if (v6 <= 0xDu)
					goto LABEL_16;
				if (v6 == 32)
				{
				LABEL_14:
					LOWORD(v9) = GetLetterWidth_6FC10() * scale;
					goto LABEL_15;
				}
			}
		}
		if (!v4[0])
			goto LABEL_14;
		//v8 = 6 * (v4[0] + 1);
		//similar as:sub_7C120_draw_bitmap_640(tempposx, tempposy, xy_DWORD_17DEC0_spritestr[temptextbuffer[0]]);
		//bitmap_pos_struct_t tempposistruct;//fixed
		//tempposistruct.pointer = new uint8_t[1];//fixed
		/*
		19f990->59361a00 0407 70361a00 04 07 85361a00
		*/
		/*tempposistruct.pointer= &x_DWORD_EA3D4[v8];//fixed
		tempposistruct.sizex = 0;
		tempposistruct.sizey = 0;*/
		if (help_VGA_type_resolution != 8)//fixed
		{
			sub_72C40_draw_bitmap_640_setcolor(v5, posy, x_DWORD_EA3D4[v4[0] + 1], color, scale);//fixed 2509d1
			v9 = x_DWORD_EA3D4[v4[0] + 1].width_4 * scale;
		}
		else
		{
			sub_72C40_draw_bitmap_640_setcolor(v5, posy, posistruct7[v4[0] + 1], color, scale);//fixed 2509d1
			v9 = posistruct7[v4[0] + 1].width_4 * scale;
		}
		//sub_72C40_draw_bitmap_640_setcolor(v5, a3, *(new bitmap_pos_struct_t)/*(uint8_t*)(v8 + x_DWORD_EA3D4)*/, a4);

	LABEL_15:
		v5 += v9;
	LABEL_16:
		v4++;
	}
	//result = v5;
	x_WORD_E36D4 = 0;
}
// E36D4: using guessed type __int16 x_WORD_E36D4;
// EA3D4: using guessed type int x_DWORD_EA3D4;

//----- (00072C40) --------------------------------------------------------
void sub_72C40_draw_bitmap_640_setcolor(int16_t posx, int16_t posy, bitmap_pos_struct_t a3, uint8_t color, uint8_t scale)//253c40
{
	//uint16_t v4; // dx
	//uint8_t* v5; // esi
	//int result; // eax
	//int v7; // [esp+4h] [ebp-4h]

	//fix it
	//v7 = 0;
	//fix it

	//v4 = *(x_WORD *)(a3 + 4);
	//v5 = *(uint8_t **)a3;
	if (x_WORD_180660_VGA_type_resolution & 1)
	{
		sub_8F920(a3.width_4, a3.height_5, posy, posx, a3.data, color, 0);
		//result = v7;
	}
	else
	{
		if (x_WORD_180660_VGA_type_resolution & 8)
		{
			/*doublebyte tempdblbyte;
			tempdblbyte.byte1 = a3.sizex;
			tempdblbyte.byte2 = a3.sizey;*/
			sub_8F935_bitmap_draw_final(a3.width_4, a3.height_5, posy, posx, a3.data, color, 0, scale);
		}
		//result = v7;
	}
	//return result;
}
// 180628: using guessed type int pdwScreenBuffer_351628;
// 180660: using guessed type __int16 x_WORD_180660_VGA_type_resolution;

//----- (0008C635) --------------------------------------------------------
void DrawCursor_8C635()//26d635
{
	uint8_t scale = 1;
	uint8_t* v1; // eax
	uint8_t* v2; // edx


	if (x_WORD_180660_VGA_type_resolution != 1)
	{
		if (!DefaultResolutions())
		{
			scale = gameUiScale;
		}
	}

	x_DWORD_1806F8_mousex = x_WORD_E3760_mouse.x;
	//x_DWORD_1806E8_mousex = x_WORD_E3760_mouse.x;
	x_DWORD_1806F8_mousey = x_WORD_E3760_mouse.y;
	//x_DWORD_1806E8_mousey = x_WORD_E3760_mouse.y;
	if (x_WORD_180660_VGA_type_resolution & 1)
	{
		x_DWORD_1806F8_mousex >>= 1;
		x_DWORD_1806F8_mousey >>= 1;
	}
	x_DWORD_180704_mouse_byte_index1 = x_DWORD_1806F8_mousex + x_DWORD_1806F8_mousey * screenWidth_18062C;
	x_DWORD_1806F4_mouse_byte_index2 = x_DWORD_180704_mouse_byte_index1;
	x_DWORD_1806FC_mouse_invx = screenWidth_18062C - x_DWORD_1806F8_mousex;
	if (x_DWORD_1806FC_mouse_invx > x_WORD_18072C_cursor_sizex)//35172c je to 13001500!
		x_DWORD_1806FC_mouse_invx = x_WORD_18072C_cursor_sizex;
	unk_1806EC = x_DWORD_1806FC_mouse_invx;
	x_DWORD_1806FC_mouse_invy = screenHeight_180624 - x_DWORD_1806F8_mousey;
	if (x_DWORD_1806FC_mouse_invy > x_WORD_18072E_cursor_sizey)
		x_DWORD_1806FC_mouse_invy = x_WORD_18072E_cursor_sizey;
	x_WORD_1806EE = x_DWORD_1806FC_mouse_invy;
	x_DWORD_180714 = x_DWORD_180700;
	x_DWORD_180718 = x_DWORD_1806F0;
	ptrScreenPos_180708 = x_DWORD_180704_mouse_byte_index1 + pdwScreenBuffer_351628;
	ptrCursorBitmapPos_18070C = x_DWORD_180730_cursor_data;

	int lineStartBytes = 0;
	int countBytes = 0;
	int scaledLinesDrawn = 0;

	for (x_WORD_180738 = 0; x_WORD_180738 < x_DWORD_1806FC_mouse_invy; x_WORD_180738++)
	{
		for (x_DWORD_18073A = 0; x_DWORD_18073A < x_DWORD_1806FC_mouse_invx; x_DWORD_18073A++)
		{
			v1 = x_DWORD_180714++;//address1
			v1[0] = ptrScreenPos_180708[0];//save to address
			v2 = x_DWORD_180718++;//address2
			v2[0] = ptrScreenPos_180708[0];//save to address

			for (int s = 0; s < scale; s++)
			{
				if (ptrCursorBitmapPos_18070C[0] != 0xfe)
					ptrScreenPos_180708[0] = ptrCursorBitmapPos_18070C[0];

				ptrScreenPos_180708++;
			}

			ptrCursorBitmapPos_18070C++;
			countBytes++;
		}
		//move row
		if (scaledLinesDrawn < scale - 1)
		{
			int lineLengthBytes = countBytes - lineStartBytes;
			countBytes -= lineLengthBytes;
			scaledLinesDrawn++;
			ptrCursorBitmapPos_18070C -= lineLengthBytes;
			x_WORD_180738--;
		}
		else
		{
			scaledLinesDrawn = 0;
			lineStartBytes = countBytes;
			ptrCursorBitmapPos_18070C += x_DWORD_180734 - x_DWORD_1806FC_mouse_invx;
		}
		ptrScreenPos_180708 += screenWidth_18062C - (x_DWORD_1806FC_mouse_invx * scale);
	}
}
// E3760: using guessed type int x_DWORD_E3760;
// 180624: using guessed type int screenHeight_180624;
// 180628: using guessed type int pdwScreenBuffer_351628;
// 18062C: using guessed type int screenWidth_18062C;
// 180660: using guessed type __int16 x_WORD_180660_VGA_type_resolution;
// 1806E8: using guessed type int;
// 1806EE: using guessed type __int16 x_WORD_1806EE;
// 1806F0: using guessed type int x_DWORD_1806F0;
// 1806F4: using guessed type int x_DWORD_1806F4_mouse_byte_index2;
// 1806F8: using guessed type int x_DWORD_1806F8;
// 1806FC: using guessed type int x_DWORD_1806FC_mouse_inv_x;
// 180700: using guessed type int x_DWORD_180700;
// 180704: using guessed type int x_DWORD_180704_mouse_byte_index1;
// 180708: using guessed type int x_DWORD_180708;
// 18070C: using guessed type int ptrCursorBitmapPos_18070C;
// 180714: using guessed type int x_DWORD_180714;
// 180718: using guessed type int x_DWORD_180718;
// 18072C: using guessed type __int16 x_WORD_18072C_cursor_sizex;
// 18072E: using guessed type __int16 x_WORD_18072E_cursor_sizey;
// 180730: using guessed type int x_DWORD_180730_cursor_data;
// 180738: using guessed type __int16 x_WORD_180738;
// 18073A: using guessed type int x_DWORD_18073A;

//----- (0008F935) --------------------------------------------------------
void sub_8F935_bitmap_draw_final(uint8_t width, uint8_t height, uint16_t tiley, int tilex, uint8_t* texture, uint8_t setbyte, char a6, uint8_t scale)//270935
{
	uint8_t* ptrScreenBuffer;
	uint8_t* ptrScreenBufferLineStart;
	uint8_t* pixel_buffer_index; // edi
	int32_t startOffsetX;
	int32_t posWidth;
	uint8_t pixel; // al

	char v12; // al
	char v13; // al
	int v15; // ecx
	char v18; // al
	char v19; // al
	char v20; // al
	unsigned int v29; // ecx
	uint8_t* v32; // edi
	char v33; // al
	char* v34; // esi
	int v35; // ebp
	int v37; // ecx
	int v40; // eax
	int v42; // ecx
	unsigned __int8 i; // dl
	char v44; // al
	char v45; // al
	unsigned __int8 v46; // of
	char v47; // dl
	unsigned __int8 v48; // al
	char v50; // al
	char v51; // dl
	char v52; // al
	char v53; // dl
	unsigned int v54; // ebx
	const void* v55; // esi
	unsigned int v62; // ecx
	uint8_t* v65; // edi
	char v66; // al
	char* v67; // esi
	uint8_t v69l;
	uint8_t v69h;
	int8_t v72l; // ecx
	char v73; // al
	char v74; // al
	bool v75; // zf
	bool v76; // sf
	char v77; // al
	char v78; // al
	int v79; // ebx
	int v85; // ebx
	x_BYTE* v86; // edi
	int v87; // ecx
	int v92; // ecx
	int v93; // ebx
	char v96; // al
	int v106; // ecx
	int v107; // ebx
	char v110; // al
	int v112; // ecx
	int v113; // ebx
	char v116; // al
	unsigned int v117; // ebx
	x_BYTE* v118; // edi
	x_BYTE* v119; // edx
	int v120; // ecx
	char v121; // al
	char v122; // al
	char v123; // al
	char v124; // al
	int v125; // eax
	int v126; // edi
	int v128; // eax
	int v129; // eax
	int v130; // eax
	char* v131; // [esp-4h] [ebp-Ch]
	uint8_t v132; // [esp+2h] [ebp-6h]
	unsigned __int8 v133; // [esp+2h] [ebp-6h]
	unsigned __int8 v134; // [esp+2h] [ebp-6h]
	unsigned __int8 v135; // [esp+2h] [ebp-6h]
	unsigned __int8 v137; // [esp+2h] [ebp-6h]
	unsigned __int8 v138; // [esp+2h] [ebp-6h]
	unsigned __int8 v139; // [esp+2h] [ebp-6h]
	char v140; // [esp+3h] [ebp-5h]
	char v141; // [esp+3h] [ebp-5h]

	if (!(height))//453558
		return;
	pixel_buffer_index = pdwScreenBuffer_351628 + x_DWORD_18063C_sprite_sizex + screenWidth_18062C * x_DWORD_180650_positiony;
	if (x_WORD_180660_VGA_type_resolution & 1)//if 320x200 is resolved, the value is halved
	{
		width /= 2;
		height /= 2;
		tilex /= 2;
		tiley /= 2;
	}
	if (tiley < 0)
	{
		if (x_WORD_E36D4 & 2)
		{
			v128 = tiley + height;
			v46 = __OFADD__(1, v128);
			v129 = v128 + 1;
			if (((v129 < 0) ^ v46) | (v129 == 0))
				return;
			tiley = -1;
			height = v129;
		}
		else
		{
			v130 = tiley + height;
			if (((tiley + height < 0) ^ __OFADD__(tiley, height)) | (tiley + height == 0))
				return;
			height += tiley;
			v130 = 0;
			do
			{
				while (1)
				{
					v130 = *texture++;
					if (!v130)
						break;
					if ((v130 & 0x80u) == 0)
						texture += v130;
				}
				tiley++;
			} while (tiley);
		}
	}
	else if (tiley + height >= x_DWORD_180644_map_resolution2_y)
	{
		if (x_WORD_E36D4 & 2)
		{
			if (tiley + 1 >= x_DWORD_180644_map_resolution2_y)
				return;
			v125 = height + tiley + 1 - x_DWORD_180644_map_resolution2_y;
			height = x_DWORD_180644_map_resolution2_y - (tiley + 1);
			v126 = v125;
			startOffsetX = 0;
			do
			{
				while (1)
				{
					startOffsetX = *texture++;
					if (!startOffsetX)
						break;
					if ((startOffsetX & 0x80u) == 0)
						texture += startOffsetX;
				}
				--v126;
			} while (v126);
			pixel_buffer_index = pdwScreenBuffer_351628;
		}
		else
		{
			if (x_DWORD_180644_map_resolution2_y <= tiley)
				return;
			height = x_DWORD_180644_map_resolution2_y - tiley;
		}
	}
	if (tilex >= 0)
	{
		if (tilex + width >= x_DWORD_180648_map_resolution2_x)
		{
			if (x_WORD_E36D4)
			{
				if (x_WORD_E36D4 & 1)
				{
					if (x_WORD_E36D4 & 2)
					{
						if (x_DWORD_180634_screen_width - tilex >= 0)
						{
							v79 = width + tilex;
							ptrScreenBuffer = (screenWidth_18062C * (height + tiley) + v79 + pixel_buffer_index);
							posWidth = 0;
							v79 = x_DWORD_180634_screen_width - v79 - 2;
							width = v79;
							v133 = v79;
							startOffsetX = -1;
							ptrScreenBufferLineStart = ptrScreenBuffer;
							do
							{
								while (1)
								{
									while (1)
									{
										startOffsetX = *texture++;
										if ((startOffsetX & 0x80u) == 0)
											break;
										ptrScreenBuffer += startOffsetX;
										width = width - startOffsetX;
									}
									if (!startOffsetX)
										break;
									posWidth = startOffsetX;
									do
									{
										pixel = *texture++;
										width = width + 1;
										if ((width & 0x80u) == 0)
											*ptrScreenBuffer = pixel;
										--ptrScreenBuffer;
										--posWidth;
									} while (posWidth);
								}
								ptrScreenBufferLineStart -= screenWidth_18062C;
								ptrScreenBuffer = ptrScreenBufferLineStart;
								width = __PAIR__(height, v133) - 256;
							} while (height);
						}
					}
					else if (x_DWORD_180634_screen_width - tilex >= 0)
					{
						v85 = width + tilex;
						v86 = (x_BYTE*)(screenWidth_18062C * tiley + v85 + pixel_buffer_index);
						v87 = 0;
						v85 = x_DWORD_180634_screen_width - v85 - 2;
						width = v85;
						v134 = v85;
						GameBitmap::DrawBitmap(texture, pixel_buffer_index, screenWidth_18062C, v85, tiley, width, height, v134);
					}
				}
				else if (x_WORD_E36D4 & 2)
				{
					ptrScreenBuffer = (screenWidth_18062C * (height + tiley) + tilex + pixel_buffer_index);
					v92 = 0;
					v93 = x_DWORD_180634_screen_width - tilex;
					if (v93 >= 0)
					{
						width = v93;
						v135 = v93;
						startOffsetX = -1;
						ptrScreenBufferLineStart = ptrScreenBuffer;
						do
						{
							while (1)
							{
								while (1)
								{
									startOffsetX = *texture++;
									if ((startOffsetX & 0x80u) == 0)
										break;
									ptrScreenBuffer -= startOffsetX;
									width = startOffsetX + width;
								}
								if (!startOffsetX)
									break;
								v92 = startOffsetX;
								do
								{
									v96 = *texture++;
									width = width - 1;
									if ((width & 0x80u) == 0)
										*ptrScreenBuffer++ = v96;
									--v92;
								} while (v92);
							}
							ptrScreenBufferLineStart -= screenWidth_18062C;
							ptrScreenBuffer = ptrScreenBufferLineStart;
							width = __PAIR__(height, v135) - 256;
						} while (width);
					}
				}
			}
			else
			{
				if (x_DWORD_180634_screen_width - tilex >= 0)
				{
					ptrScreenBuffer = (screenWidth_18062C * tiley + tilex + pixel_buffer_index);
					v69l = x_DWORD_180634_screen_width - tilex;
					v69h = height;//ebx
					v132 = v69l;//ebp-6
					ptrScreenBufferLineStart = ptrScreenBuffer;//edx edi
					v72l = 0;//ecx
					do
					{
						while (1)
						{
							while (1)
							{
								v73 = *texture++;
								if (v73 >= 0)
									break;
								v46 = __OFADD__(v73, v69l);//fix
								v75 = v73 + v69l == 0;//fix
								v76 = (char)(v73 + v69l) < 0;//fix
								v69l = v73 + v69l;//fix
								if (!((v76 ^ v46) | v75))
								{
									ptrScreenBuffer -= v73;
									v77 = *texture++;
									v72l = v77;
									do
									{
										v78 = *texture++;
										v69l--;
										if ((v69l & 0x80u) == 0)//fix
											*ptrScreenBuffer++ = v78;
										v72l--;
									} while (v72l);
								}
							}
							if (!v73)
								break;
							v72l = v73;
							do
							{
								v74 = *texture++;
								v69l--;
								if ((v69l & 0x80u) == 0)//fix
									*ptrScreenBuffer++ = v74;
								v72l--;
							} while (v72l);
						}
						ptrScreenBufferLineStart += screenWidth_18062C;
						ptrScreenBuffer = ptrScreenBufferLineStart;

						v69l = /*(v69l&0xff00)+*/v132;//fix
						v69h--;
					} while (v69h);//fix
				}
			}
		}
		else if (x_WORD_E36D4 & 1)
		{
			if (x_WORD_E36D4 & 2)
			{
				ptrScreenBuffer = (screenWidth_18062C * (height + tiley) + width + tilex + pixel_buffer_index);
				posWidth = 0;
				startOffsetX = -1;
				ptrScreenBufferLineStart = ptrScreenBuffer;
				do
				{
					while (1)
					{
						while (1)
						{
							startOffsetX = *texture++;

							//Is width byte
							if ((startOffsetX & 0x80u) == 0)
							{
								//Start Drawing
								break;
							}
							ptrScreenBuffer += startOffsetX;
							v12 = *texture++;
							posWidth = v12;
							do
							{
								v13 = *texture++;
								*ptrScreenBuffer-- = v13;
								--posWidth;
							} while (posWidth);
						}
						if (!startOffsetX)
							break;
						posWidth = startOffsetX;
						do
						{
							pixel = *texture++;
							*ptrScreenBuffer-- = pixel;
							--posWidth;
						} while (posWidth);
					}
					ptrScreenBufferLineStart -= screenWidth_18062C;
					ptrScreenBuffer = ptrScreenBufferLineStart;
					--height;
				} while (height);
			}
			else
			{
				ptrScreenBuffer = (screenWidth_18062C * tiley + width + tilex + pixel_buffer_index);
				v15 = 0;
				startOffsetX = -1;
				ptrScreenBufferLineStart = ptrScreenBuffer;
				do
				{
					while (1)
					{
						while (1)
						{
							startOffsetX = *texture++;
							if ((startOffsetX & 0x80u) == 0)
							{
								//Start Drawing
								break;
							}
							ptrScreenBuffer += startOffsetX;
							v19 = *texture++;
							v15 = v19;
							do
							{
								v20 = *texture++;
								*ptrScreenBuffer-- = v20;
								--v15;
							} while (v15);
						}
						if (!startOffsetX)
							break;
						v15 = startOffsetX;
						do
						{
							v18 = *texture++;
							*ptrScreenBuffer-- = v18;
							--v15;
						} while (v15);
					}
					ptrScreenBufferLineStart += screenWidth_18062C;
					ptrScreenBuffer = ptrScreenBufferLineStart;
					--height;
				} while (height);
			}
		}
		else if (x_WORD_E36D4)
		{
			if (x_WORD_E36D4 & 2)
			{
				ptrScreenBuffer = (screenWidth_18062C * (height + tiley) + tilex + pixel_buffer_index);
				v29 = 0;
				startOffsetX = -1;
				ptrScreenBufferLineStart = ptrScreenBuffer;
				do
				{
					while (1)
					{
						while (1)
						{
							startOffsetX = *texture++;
							if ((startOffsetX & 0x80u) == 0)
							{
								//Start Drawing
								break;
							}
							v32 = &ptrScreenBuffer[-startOffsetX];
							v33 = *texture;
							v34 = (char*)(texture + 1);
							v29 = v33;
							qmemcpy(v32, v34, v29);
							texture = (uint8_t*)&v34[v29];
							ptrScreenBuffer = &v32[v29];
							v29 = 0;
						}
						if (!startOffsetX)
							break;
						v29 = startOffsetX;
						qmemcpy(ptrScreenBuffer, texture, v29);
						texture += v29;
						ptrScreenBuffer += v29;
						v29 = 0;
					}
					ptrScreenBufferLineStart -= screenWidth_18062C;
					ptrScreenBuffer = ptrScreenBufferLineStart;
					--height;
				} while (height);
			}
			else if (x_WORD_E36D4 & 4)
			{
				v35 = x_DWORD_E3890;
				ptrScreenBuffer = (screenWidth_18062C * tiley + tilex + pixel_buffer_index);
				v37 = 0;
				ptrScreenBufferLineStart = ptrScreenBuffer;
				do
				{
					while (1)
					{
						while (1)
						{
							startOffsetX = *texture++;
							if ((startOffsetX & 0x80u) == 0)
							{
								//Start Drawing
								break;
							}
							ptrScreenBuffer -= (char)startOffsetX;
						}
						if (!startOffsetX)
							break;
						v37 = startOffsetX;
						v40 = startOffsetX;
						do
						{
							v40 = *texture++;
							v40 = *ptrScreenBuffer;
							*ptrScreenBuffer++ = *(x_BYTE*)(v40 + v35);
							--v37;
						} while (v37);
					}
					ptrScreenBufferLineStart += screenWidth_18062C;
					ptrScreenBuffer = ptrScreenBufferLineStart;
					--height;
				} while (height);
			}
			else if (x_WORD_E36D4 & 8)
			{
				ptrScreenBuffer = (screenWidth_18062C * tiley + tilex + pixel_buffer_index);
				v42 = 0;
				ptrScreenBufferLineStart = ptrScreenBuffer;
				v140 = height;
				for (i = setbyte; ; i = setbyte)
				{
					while (1)
					{
						while (1)
						{
							v44 = *texture++;
							if (v44 >= 0)
								break;
							v42 = (unsigned __int8)-v44;
							do
							{
								v46 = __OFSUB__(i--, 1);
								if (((i & 0x80u) != 0) ^ v46)
								{
									i = setbyte;
									++ptrScreenBuffer;
								}
								--v42;
							} while (v42);
						}
						if (!v44)
							break;
						LOBYTE(v42) = v44;
						do
						{
							v45 = *texture++;
							v46 = __OFSUB__(i--, 1);
							if (((i & 0x80u) != 0) ^ v46)
							{
								i = setbyte;
								*ptrScreenBuffer++ = v45;
							}
							--v42;
						} while (v42);
					}
					if (!--v140)
						break;
					v47 = a6;
					while (--v47 >= 0)
					{
						while (1)
						{
							do
								v48 = *texture++;
							while ((v48 & 0x80u) != 0);
							if (!v48)
								break;
							texture += v48;
						}
					}
					ptrScreenBufferLineStart += screenWidth_18062C;
					ptrScreenBuffer = ptrScreenBufferLineStart;
				}
			}
			else if (x_WORD_E36D4 & 0x20)
			{
				ptrScreenBuffer = (screenWidth_18062C * tiley + tilex + pixel_buffer_index);
				ptrScreenBufferLineStart = ptrScreenBuffer;
				v141 = height;
				do
				{
					while (1)
					{
						while (1)
						{
							v50 = *texture++;
							if (v50 >= 0)
								break;
							ptrScreenBuffer += (setbyte * -v50);
						}
						if (!v50)
							break;
						v51 = v50;
						do
						{
							v52 = *texture++;
							memset(ptrScreenBuffer, v52, setbyte);
							ptrScreenBuffer += setbyte;
							v46 = __OFSUB__(v51--, 1);
						} while (!(((v51 < 0) ^ v46) | (v51 == 0)));
					}
					v53 = a6 - 1;
					if (a6 > 1)
					{
						v131 = (char*)texture;
						v54 = ptrScreenBuffer - ptrScreenBufferLineStart;
						do
						{
							v55 = ptrScreenBufferLineStart;
							ptrScreenBufferLineStart += screenWidth_18062C;
							qmemcpy(ptrScreenBufferLineStart, (void*)v55, v54);
							v46 = __OFSUB__(v53--, 1);
						} while (!(((v53 < 0) ^ v46) | (v53 == 0)));
						texture = (uint8_t*)v131;
					}
					ptrScreenBufferLineStart += screenWidth_18062C;
					ptrScreenBuffer = ptrScreenBufferLineStart;
					--v141;
				} while (v141);
			}
			else if (x_WORD_E36D4 & 0x40)
			{
				GameBitmap::DrawColourizedBitmap(texture, setbyte, pixel_buffer_index, screenWidth_18062C, tilex, tiley, height, scale);
			}
			else
			{
				ptrScreenBuffer = (screenWidth_18062C * tiley + tilex + pixel_buffer_index);
				v62 = 0;
				startOffsetX = -1;
				ptrScreenBufferLineStart = ptrScreenBuffer;
				do
				{
					while (1)
					{
						while (1)
						{
							startOffsetX = *texture++;
							if ((startOffsetX & 0x80u) == 0)
							{
								//Start Drawing
								break;
							}
							v65 = &ptrScreenBuffer[-startOffsetX];
							v66 = *texture;
							v67 = (char*)(texture + 1);
							v62 = v66;
							qmemcpy(v65, v67, v62);
							texture = (uint8_t*)&v67[v62];
							ptrScreenBuffer = &v65[v62];
							v62 = 0;
						}
						if (!startOffsetX)
							break;
						v62 = startOffsetX;
						qmemcpy(ptrScreenBuffer, texture, v62);
						texture += v62;
						ptrScreenBuffer += v62;
						v62 = 0;
					}
					ptrScreenBufferLineStart += screenWidth_18062C;
					ptrScreenBuffer = ptrScreenBufferLineStart;
					--height;
				} while (height);
			}
		}
		else
		{
			GameBitmap::DrawBitmap(texture, pixel_buffer_index, screenWidth_18062C, tilex, tiley, height, scale);
		}
		return;
	}
	if (!(x_WORD_E36D4 & 1))
	{
		if (!(x_WORD_E36D4 & 2))
			return;
		v117 = -tilex;
		if (width <= v117)
			return;
		v118 = (x_BYTE*)(screenWidth_18062C * (height + tiley) + pixel_buffer_index);
		v117 = height;
		v119 = v118;
		v139 = v117;
		v120 = 0;
		while (1)
		{
			while (1)
			{
				while (1)
				{
					v121 = *texture++;
					if (v121 < 0)
						break;
					if (v121)
					{
						v120 = v121;
						do
						{
							v122 = *texture++;
							v117 = v117 - 1;
							if ((v117 & 0x80u) != 0)
								*v118++ = v122;
							v120--;
						} while (v120);
					}
					else
					{
						v119 -= screenWidth_18062C;
						v118 = v119;
						v117 = __PAIR__(v117, v139) - 256;
						if (!v117)
							return;
					}
				}
				if ((char)v117 > 0)
					break;
			LABEL_225:
				v118 -= v121;
				v123 = *texture++;
				v120 = v123;
				do
				{
					v124 = *texture++;
					v117 = v117 - 1;
					if ((v117 & 0x80u) != 0)
						*v118++ = v124;
					v120--;
				} while (v120);
			}
			v117 = v121 + v117;
			if ((v117 & 0x80u) != 0)
			{
				v121 = v117;
				goto LABEL_225;
			}
		}
	}
	if (x_WORD_E36D4 & 2)
	{
		ptrScreenBuffer = (screenWidth_18062C * (height + tiley) + width + tilex + pixel_buffer_index);
		v106 = 0;
		v107 = width + tilex + 1;
		if (v107 >= 0)
		{
			width = v107;
			v137 = v107;
			ptrScreenBufferLineStart = ptrScreenBuffer;
			startOffsetX = -1;
			do
			{
				while (1)
				{
					while (1)
					{
						startOffsetX = *texture++;
						if ((startOffsetX & 0x80u) == 0)
						{
							//Start Drawing
							break;
						}
						ptrScreenBuffer += startOffsetX;
						width = startOffsetX + width;
					}
					if (!startOffsetX)
						break;
					v106 = startOffsetX;
					do
					{
						v110 = *texture++;
						width = width - 1;
						if ((width & 0x80u) == 0)
							*ptrScreenBuffer = v110;
						ptrScreenBuffer--;
						v106--;
					} while (v106);
				}
				ptrScreenBufferLineStart -= screenWidth_18062C;
				ptrScreenBuffer = ptrScreenBufferLineStart;
				width = __PAIR__(height, v137) - 256;
			} while (height);
		}
	}
	else
	{
		ptrScreenBuffer = (screenWidth_18062C * tiley + width + tilex + pixel_buffer_index);
		v112 = 0;
		v113 = width + tilex + 1;
		if (v113 >= 0)
		{
			width = v113;
			v138 = v113;
			ptrScreenBufferLineStart = ptrScreenBuffer;
			startOffsetX = -1;
			do
			{
				while (1)
				{
					while (1)
					{
						startOffsetX = *texture++;
						if ((startOffsetX & 0x80u) == 0)
						{
							//Start Drawing
							break;
						}
						ptrScreenBuffer += startOffsetX;
						width = startOffsetX + width;
					}
					if (!startOffsetX)
						break;
					posWidth = startOffsetX;
					do
					{
						v116 = *texture++;
						width = width - 1;
						if ((width & 0x80u) == 0)
							*ptrScreenBuffer = v116;
						ptrScreenBuffer--;
						posWidth--;
					} while (posWidth);
				}
				ptrScreenBufferLineStart += screenWidth_18062C;
				ptrScreenBuffer = ptrScreenBufferLineStart;
				width = __PAIR__(width, v138) - 256;
			} while (height);
		}
	}
}

// E36D4: using guessed type __int16 x_WORD_E36D4;
// E3890: using guessed type int x_DWORD_E3890;
// 180628: using guessed type int pdwScreenBuffer_351628;
// 18062C: using guessed type int screenWidth_18062C;
// 180634: using guessed type int x_DWORD_180634_screen_width;
// 18063C: using guessed type int x_DWORD_18063C_sprite_sizex;
// 180644: using guessed type int x_DWORD_180644_map_resolution2_y;
// 180648: using guessed type int x_DWORD_180648_map_resolution2_x;
// 180650: using guessed type int x_DWORD_180650_positiony;
// 180660: using guessed type __int16 x_WORD_180660_VGA_type_resolution;

//----- (0008F920) --------------------------------------------------------
void sub_8F920(uint8_t a1byte1, uint8_t a1byte2, int16_t posx, int16_t posy, uint8_t* a4, unsigned __int8 a5, char a6)//270920
{
	sub_8F935_bitmap_draw_final(a1byte1, a1byte2, posx, posy, a4, a5, a6, 1);//270935
}

signed int GetTrueWizardNumber_61790(signed int inputnumber)//242790
{
	signed int outputNumber = inputnumber;
	if (x_D41A0_BYTEARRAY_4_struct.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
	{
		switch (D41A0_0.array_0x2BDE[inputnumber].dword_0x3E6_2BE4_12228.byte_0x1C0_448)
		{
		case 0:
			outputNumber = 0;
			break;
		case 1:
			outputNumber = 2;
			break;
		case 2:
			outputNumber = 1;
			break;
		case 3:
			outputNumber = 4;
			break;
		case 4:
			outputNumber = 5;
			break;
		case 5:
			outputNumber = 6;
			break;
		case 6:
			outputNumber = 3;
			break;
		case 7:
			outputNumber = 7;
			break;
		default:
			return outputNumber;
		}
	}
	return outputNumber;
}

void Convert_from_shadow_D41A0_BYTESTR_0(type_shadow_D41A0_BYTESTR_0* from, type_D41A0_BYTESTR_0* to) {
	for (int i = 0; i < 4; i++)to->stub0[i] = from->stub0[i];
	to->dword_0x4 = from->dword_0x4;
	to->rand_0x8 = from->dword_0x8;
	to->LevelIndex_0xc = from->word_0xc;
	to->NumberOfPlayers_0xe = from->NumberOfPlayers_0xe;
	for (int i = 0; i < 0x1d; i++)to->array_0x10[i] = from->array_0x10[i];
	to->dword_0x2d = from->dword_0x2d;
	to->word_0x31 = from->word_0x31;
	to->word_0x33 = from->word_0x33;
	to->dword_0x35 = from->dword_0x35;
	for (int i = 0; i < 508; i++)to->array_0x39[i] = from->array_0x39[i];
	to->maptypeMusic_0x235 = from->dword_0x235;
	to->byte_0x239 = from->byte_0x239;
	to->dword_0x23a = from->dword_0x23a;
	to->dword_0x23e = from->dword_0x23e;
	to->dword_0x242 = from->dword_0x242;
	for (int i = 0; i < 0x3e8; i++)to->pointers_0x246[i] = (type_entity_0x6E8E*)from->pointers_0x246[i];
	to->dword_0x11e6 = from->dword_0x11e6;
	for (int i = 0; i < 0x3e8; i++)to->dword_0x11EA[i] = (type_entity_0x6E8E*)from->dword_0x11EA[i];
	to->m_GameSettings = from->m_GameSettings;
	to->dword_0x219A = from->dword_0x219A;
	to->dword_0x219E = from->dword_0x219E;
	to->dword_0x21A2 = from->dword_0x21A2;
	to->dword_0x21A6 = from->dword_0x21A6;
	to->str_0x21AA = from->str_0x21AA;
	to->str_0x21AE = from->str_0x21AE;
	to->str_0x21B2 = from->str_0x21B2;
	to->str_0x21B6 = from->str_0x21B6;
	for (int i = 0; i < 0x1a8; i++)to->stub3b[i] = from->stub3b[i];
	for (int i = 0; i < 8; i++)to->array_0x2362[i] = from->array_0x2362[i];
	for (int i = 0; i < 0x14d; i++)to->stub3c[i] = from->stub3c[i];
	for (int i = 0; i < 0x6ff; i++)to->stub3d[i] = from->stub3d[i];
	for (int i = 0; i < 8; i++)to->array_0x2BDE[i] = from->array_0x2BDE[i];
	for (int i = 0; i < 8; i++)to->playerInputs_0x6E3E[i] = from->playerInputs_0x6E3E[i];
	for (int i = 0; i < 0x3e8; i++) {
		to->struct_0x6E8E[i].next_0 = (_str_0x6E8E*)from->struct_0x6E8E[i].next_0;
		to->struct_0x6E8E[i].maxLife_0x4 = from->struct_0x6E8E[i].dword_0x4;
		to->struct_0x6E8E[i].life_0x8 = from->struct_0x6E8E[i].dword_0x8;
		to->struct_0x6E8E[i].struct_byte_0xc_12_15 = from->struct_0x6E8E[i].struct_byte_0xc_12_15;
		to->struct_0x6E8E[i].dword_0x10_16 = from->struct_0x6E8E[i].dword_0x10_16;
		to->struct_0x6E8E[i].rand_0x14_20 = from->struct_0x6E8E[i].word_0x14_20;
		to->struct_0x6E8E[i].oldMapEntity_0x16_22 = from->struct_0x6E8E[i].word_0x16_22;
		to->struct_0x6E8E[i].nextEntity_0x18_24 = from->struct_0x6E8E[i].word_0x18_24_next_entity;
		to->struct_0x6E8E[i].id_0x1A_26 = from->struct_0x6E8E[i].word_0x1A_26;
		to->struct_0x6E8E[i].yaw_0x1C_28 = from->struct_0x6E8E[i].word_0x1C_28;
		to->struct_0x6E8E[i].pitch_0x1E_30 = from->struct_0x6E8E[i].word_0x1E_30;
		to->struct_0x6E8E[i].roll_0x20_32 = from->struct_0x6E8E[i].word_0x20_32;
		to->struct_0x6E8E[i].fov_0x22_34 = from->struct_0x6E8E[i].word_0x22_34;
		to->struct_0x6E8E[i].word_0x24_36 = from->struct_0x6E8E[i].word_0x24_36;
		to->struct_0x6E8E[i].word_0x26_38 = from->struct_0x6E8E[i].word_0x26_38;
		to->struct_0x6E8E[i].parentId_0x28_40 = from->struct_0x6E8E[i].word_0x28_40;
		to->struct_0x6E8E[i].subSpellIndex_0x2A_42 = from->struct_0x6E8E[i].word_0x2A_42;
		to->struct_0x6E8E[i].word_0x2C_44 = from->struct_0x6E8E[i].word_0x2C_44;
		to->struct_0x6E8E[i].word_0x2E_46 = from->struct_0x6E8E[i].word_0x2E_46;
		to->struct_0x6E8E[i].word_0x30_48 = from->struct_0x6E8E[i].word_0x30_48;
		to->struct_0x6E8E[i].word_0x32_50 = from->struct_0x6E8E[i].word_0x32_50;
		to->struct_0x6E8E[i].word_0x34_52 = from->struct_0x6E8E[i].word_0x34_52;
		to->struct_0x6E8E[i].word_0x36_54 = from->struct_0x6E8E[i].word_0x36_54;
		to->struct_0x6E8E[i].byte_0x38_56 = from->struct_0x6E8E[i].byte_0x38_56;
		to->struct_0x6E8E[i].byte_0x39_57 = from->struct_0x6E8E[i].byte_0x39_57;
		to->struct_0x6E8E[i].byte_0x3A_58 = from->struct_0x6E8E[i].byte_0x3A_58;
		to->struct_0x6E8E[i].byte_0x3B_59 = from->struct_0x6E8E[i].byte_0x3B_59;
		to->struct_0x6E8E[i].byte_0x3C_60 = from->struct_0x6E8E[i].byte_0x3C_60;
		to->struct_0x6E8E[i].fontTypeIndex_0x3D_61 = from->struct_0x6E8E[i].byte_0x3D_61;
		to->struct_0x6E8E[i].byte_0x3E_62 = from->struct_0x6E8E[i].byte_0x3E_62;
		to->struct_0x6E8E[i].class_0x3F_63 = from->struct_0x6E8E[i].type_0x3F_63;
		to->struct_0x6E8E[i].model_0x40_64 = from->struct_0x6E8E[i].subtype_0x40_64;
		to->struct_0x6E8E[i].xtype_0x41_65 = from->struct_0x6E8E[i].byte_0x41_65;
		to->struct_0x6E8E[i].xsubtype_0x42_66 = from->struct_0x6E8E[i].byte_0x42_66;
		to->struct_0x6E8E[i].byte_0x43_67 = from->struct_0x6E8E[i].byte_0x43_67;
		to->struct_0x6E8E[i].byte_0x44_68 = from->struct_0x6E8E[i].byte_0x44_68;
		to->struct_0x6E8E[i].actionIndex_0x45_69 = from->struct_0x6E8E[i].byte_0x45_69;
		to->struct_0x6E8E[i].byte_0x46_70 = from->struct_0x6E8E[i].byte_0x46_70;
		to->struct_0x6E8E[i].byte_0x47_71_xx = from->struct_0x6E8E[i].byte_0x47_71_xx;
		to->struct_0x6E8E[i].StageVar1_0x48_72 = from->struct_0x6E8E[i].byte_0x48_72;
		to->struct_0x6E8E[i].StageVar2_0x49_73 = from->struct_0x6E8E[i].byte_0x49_73;
		to->struct_0x6E8E[i].word_0x4A_74 = from->struct_0x6E8E[i].word_0x4A_74;
		to->struct_0x6E8E[i].position_0x4C_76 = from->struct_0x6E8E[i].array_0x4C_76;
		to->struct_0x6E8E[i].array_0x52_82 = from->struct_0x6E8E[i].array_0x52_82;
		to->struct_0x6E8E[i].word_0x5A_90 = from->struct_0x6E8E[i].word_0x5A_90;
		to->struct_0x6E8E[i].animationFrame_0x5C_92 = from->struct_0x6E8E[i].byte_0x5C_92;
		to->struct_0x6E8E[i].byte_0x5D_93 = from->struct_0x6E8E[i].byte_0x5D_93;
		to->struct_0x6E8E[i].str_0x5E_94 = from->struct_0x6E8E[i].str_0x5E_94;
		to->struct_0x6E8E[i].actSpeed_0x82_130 = from->struct_0x6E8E[i].word_0x82_130;
		to->struct_0x6E8E[i].minSpeed_0x84_132 = from->struct_0x6E8E[i].word_0x84_132;
		to->struct_0x6E8E[i].maxSpeed_0x86_134 = from->struct_0x6E8E[i].word_0x86_134;
		to->struct_0x6E8E[i].manaRegen_0x88_136 = from->struct_0x6E8E[i].dword_0x88_136;
		to->struct_0x6E8E[i].maxMana_0x8C_140 = from->struct_0x6E8E[i].dword_0x8C_140;
		to->struct_0x6E8E[i].mana_0x90_144 = from->struct_0x6E8E[i].dword_0x90_144;
		to->struct_0x6E8E[i].playerEntityIndex_0x94_148 = from->struct_0x6E8E[i].playerEntityIndex_0x94_148;
		to->struct_0x6E8E[i].word_0x96_150 = from->struct_0x6E8E[i].word_0x96_150;
		to->struct_0x6E8E[i].word_0x98_152 = from->struct_0x6E8E[i].word_0x98_152;
		to->struct_0x6E8E[i].axis_0x9A_154x = from->struct_0x6E8E[i].axis_0x9A_154x;
		to->struct_0x6E8E[i].dword_0xA0_160x = (type_str_160*)from->struct_0x6E8E[i].dword_0xA0_160x;
		to->struct_0x6E8E[i].dword_0xA4_164x = (type_str_164*)from->struct_0x6E8E[i].dword_0xA4_164x;
	}

	to->terrain_2FECE.word_2FECE = from->str_2FECE.word_2FECE;
	to->terrain_2FECE.levelID_2FED0 = from->str_2FECE.levelID_2FED0;
	to->terrain_2FECE.byte_0x2FED2 = from->str_2FECE.byte_0x2FED2;
	to->terrain_2FECE.byte_0x2FED3 = from->str_2FECE.byte_0x2FED3;
	to->terrain_2FECE.MapType = (from->str_2FECE.MapType == 2) ? MapType_t::Cave : (from->str_2FECE.MapType == 1) ? MapType_t::Night : MapType_t::Day;
	to->terrain_2FECE.word_0x2FED5 = from->str_2FECE.word_0x2FED5;
	to->terrain_2FECE.word_0x2FED7 = from->str_2FECE.word_0x2FED7;
	for (int i = 0; i < 8; i++)to->terrain_2FECE.player_0x2FED9[i] = from->str_2FECE.array_0x2FED9[i];
	for (int i = 0; i < 4; i++)to->terrain_2FECE.stubb[i] = from->str_2FECE.stubb[i];
	to->terrain_2FECE.seed_0x2FEE5 = from->str_2FECE.word_0x2FEE5;
	for (int i = 0; i < 2; i++)to->terrain_2FECE.stubb2[i] = from->str_2FECE.stubb2[i];
	to->terrain_2FECE.offset_0x2FEE9 = from->str_2FECE.word_0x2FEE9;
	for (int i = 0; i < 2; i++)to->terrain_2FECE.stubc[i] = from->str_2FECE.stubc[i];
	to->terrain_2FECE.raise_0x2FEED = from->str_2FECE.word_0x2FEED;
	for (int i = 0; i < 2; i++)to->terrain_2FECE.stubc2[i] = from->str_2FECE.stubc2[i];
	to->terrain_2FECE.gnarl_0x2FEF1 = from->str_2FECE.word_0x2FEF1;
	for (int i = 0; i < 2; i++)to->terrain_2FECE.stubc3[i] = from->str_2FECE.stubc3[i];
	to->terrain_2FECE.river_0x2FEF5 = from->str_2FECE.word_0x2FEF5;
	to->terrain_2FECE.lriver_0x2FEF9 = from->str_2FECE.word_0x2FEF9;
	for (int i = 0; i < 2; i++)to->terrain_2FECE.stube[i] = from->str_2FECE.stube[i];
	to->terrain_2FECE.source_0x2FEFD = from->str_2FECE.word_0x2FEFD;
	for (int i = 0; i < 2; i++)to->terrain_2FECE.stubf[i] = from->str_2FECE.stubf[i];
	to->terrain_2FECE.snLin_0x2FF01 = from->str_2FECE.word_0x2FF01;
	for (int i = 0; i < 2; i++)to->terrain_2FECE.stubg[i] = from->str_2FECE.stubg[i];
	to->terrain_2FECE.snFlt_0x2FF05 = from->str_2FECE.word_0x2FF05;
	for (int i = 0; i < 2; i++)to->terrain_2FECE.stubh[i] = from->str_2FECE.stubh[i];
	to->terrain_2FECE.bhLin_0x2FF09 = from->str_2FECE.word_0x2FF09;
	for (int i = 0; i < 2; i++)to->terrain_2FECE.stubi[i] = from->str_2FECE.stubi[i];
	to->terrain_2FECE.bhFlt_0x2FF0D = from->str_2FECE.word_0x2FF0D;
	for (int i = 0; i < 2; i++)to->terrain_2FECE.stubj[i] = from->str_2FECE.stubj[i];
	to->terrain_2FECE.rkSte_0x2FF11 = from->str_2FECE.word_0x2FF11;
	for (int i = 0; i < 1022; i++)to->terrain_2FECE.stubk[i] = from->str_2FECE.stubk[i];
	for (int i = 0; i < 1200; i++)to->terrain_2FECE.entity_0x30311[i] = from->str_2FECE.array_0x30311[i];
	to->terrain_2FECE.next_0x360D1 = from->str_2FECE.next_0x360D1;
	for (int i = 0; i < 8; i++)to->terrain_2FECE.WizardMapSettings_0x360D2[i] = from->str_2FECE.WizardMapSettings_0x360D2[i];
	for (int i = 0; i < 8; i++)to->terrain_2FECE.stages_0x36442[i] = from->str_2FECE.str_0x36442[i];
	for (int i = 0; i < 11; i++)
	{
		to->terrain_2FECE.StageVars_0x3647A[i].index_0x3647A_0 = from->str_2FECE.array_0x3647A[i].str_0x3647A_byte_0;
		to->terrain_2FECE.StageVars_0x3647A[i].stage_0x3647A_1 = from->str_2FECE.array_0x3647A[i].str_0x3647A_byte_1;
		to->terrain_2FECE.StageVars_0x3647A[i].str_0x3647A_2 = from->str_2FECE.array_0x3647A[i].str_0x3647A_2;
		to->terrain_2FECE.StageVars_0x3647A[i].str_0x3647C_4.axis = from->str_2FECE.array_0x3647A[i].str_0x3647C_4.axis;
	}
	to->str_0x364D2 = from->str_0x364D2;
	to->word_0x3653E = from->word_0x3653E;
	to->word_0x36540 = from->word_0x36540;
	to->word_0x36542 = from->word_0x36542;
	to->word_0x36544 = from->word_0x36544;
	to->word_0x36546 = from->word_0x36546;
	to->word_0x36548 = from->word_0x36548;
	to->word_0x3654A = from->word_0x3654A;
	for (int i = 0; i < 8; i++) {
		to->stages_0x3654C[i].stages_3654C_byte0 = from->struct_0x3654C[i].str_3654C_byte0;
		to->stages_0x3654C[i].str_3654D_byte1 = from->struct_0x3654C[i].str_3654D_byte1;
		to->stages_0x3654C[i].str_3654E_axis = from->struct_0x3654C[i].str_3654E_axis;
		to->stages_0x3654C[i].str_36552_un.dword = from->struct_0x3654C[i].str_36552_un.dword;
	}
	for (int i = 0; i < 8; i++)to->struct_0x3659C[i] = from->struct_0x3659C[i];
	for (int i = 0; i < 11; i++) {
		to->StageVars2_0x365F4[i].index_0x3647A_0 = from->array_0x365F4[i].str_0x3647A_byte_0;
		to->StageVars2_0x365F4[i].stage_0x3647A_1 = from->array_0x365F4[i].str_0x3647A_byte_1;
		to->StageVars2_0x365F4[i].str_0x3647A_2 = from->array_0x365F4[i].str_0x3647A_2;
		to->StageVars2_0x365F4[i].str_0x3647C_4.axis = from->array_0x365F4[i].str_0x3647C_4.axis;
	}
	for (int i = 0; i < 0x32; i++) {
		to->str_0x3664C[i].byte_0 = from->str_0x3664C[i].byte_0;
		to->str_0x3664C[i].byte_1 = from->str_0x3664C[i].byte_1;
		to->str_0x3664C[i].byte_2 = from->str_0x3664C[i].byte_2;
		to->str_0x3664C[i].byte_3 = from->str_0x3664C[i].byte_3;
		to->str_0x3664C[i].axis3d_4 = from->str_0x3664C[i].axis3d_4;
		to->str_0x3664C[i].event_A.pointer_0x6E8E = (type_entity_0x6E8E*)from->str_0x3664C[i].dword_A;
		for (int j = 0; j < 25; j++)to->str_0x3664C[i].array_E[j] = from->str_0x3664C[i].array_E[j];
	}
	to->byte_0x36DEA_fly_asistant = from->byte_0x36DEA_fly_asistant;
	to->byte_0x36DEB_xx = from->byte_0x36DEB_xx;
	to->word_0x36DEC_mousex = from->word_0x36DEC_mousex;
	to->word_0x36DEE_mousey = from->word_0x36DEE_mousey;
	to->word_0x36DF0_mousexx = from->word_0x36DF0_mousexx;
	to->dword_0x36DF2 = from->dword_0x36DF2;
	to->dword_0x36DF6 = (type_str_160*)from->dword_0x36DF6;
	to->word_0x36DFA = from->word_0x36DFA;
	to->word_0x36DFC = from->word_0x36DFC;
	to->word_0x36DFE = from->word_0x36DFE;
	to->countStageVars_0x36E00 = from->byte_0x36E00;
	to->stageIndex_0x36E01 = from->byte_0x36E01;
	to->byte_0x36E02 = from->byte_0x36E02;
	to->byte_0x36E03 = from->byte_0x36E03;
	to->byte_counter_current_objective_box_0x36E04 = from->byte_counter_current_objective_box_0x36E04;
	for (int i = 0; i < 6; i++)to->stub3k[i] = from->stub3k[i];
	to->byte_0x36E0B = from->byte_0x36E0B;
	for (int i = 0; i < 11; i++)to->stubend[i] = from->stubend[i];
}

uint8_t* Zero_pointer = NULL;

void Convert_to_shadow_D41A0_BYTESTR_0(type_D41A0_BYTESTR_0* from, type_shadow_D41A0_BYTESTR_0* to) {
	for (int i = 0; i < 4; i++)to->stub0[i] = from->stub0[i];
	to->dword_0x4 = from->dword_0x4;
	to->dword_0x8 = from->rand_0x8;
	to->word_0xc = from->LevelIndex_0xc;
	to->NumberOfPlayers_0xe = from->NumberOfPlayers_0xe;
	for (int i = 0; i < 0x1d; i++)to->array_0x10[i] = from->array_0x10[i];
	to->dword_0x2d = from->dword_0x2d;
	to->word_0x31 = from->word_0x31;
	to->word_0x33 = from->word_0x33;
	to->dword_0x35 = from->dword_0x35;
	for (int i = 0; i < 508; i++)to->array_0x39[i] = from->array_0x39[i];
	to->dword_0x235 = from->maptypeMusic_0x235;
	to->byte_0x239 = from->byte_0x239;
	to->dword_0x23a = from->dword_0x23a;
	to->dword_0x23e = from->dword_0x23e;
	to->dword_0x242 = from->dword_0x242;
	for (int i = 0; i < 0x3e8; i++)to->pointers_0x246[i] = ((uint8_t*)from->pointers_0x246[i]- Zero_pointer);
	to->dword_0x11e6 = from->dword_0x11e6;
	for (int i = 0; i < 0x3e8; i++)to->dword_0x11EA[i] = ((uint8_t*)from->dword_0x11EA[i] - Zero_pointer);
	to->m_GameSettings = from->m_GameSettings;
	to->dword_0x219A = from->dword_0x219A;
	to->dword_0x219E = from->dword_0x219E;
	to->dword_0x21A2 = from->dword_0x21A2;
	to->dword_0x21A6 = from->dword_0x21A6;
	to->str_0x21AA = from->str_0x21AA;
	to->str_0x21AE = from->str_0x21AE;
	to->str_0x21B2 = from->str_0x21B2;
	to->str_0x21B6 = from->str_0x21B6;
	for (int i = 0; i < 0x1a8; i++)to->stub3b[i] = from->stub3b[i];
	for (int i = 0; i < 8; i++)to->array_0x2362[i] = from->array_0x2362[i];
	for (int i = 0; i < 0x14d; i++)to->stub3c[i] = from->stub3c[i];
	for (int i = 0; i < 0x6ff; i++)to->stub3d[i] = from->stub3d[i];
	for (int i = 0; i < 8; i++)to->array_0x2BDE[i] = from->array_0x2BDE[i];
	for (int i = 0; i < 8; i++)to->playerInputs_0x6E3E[i] = from->playerInputs_0x6E3E[i];
	for (int i = 0; i < 0x3e8; i++) {
		to->struct_0x6E8E[i].next_0 = ((uint8_t*)from->struct_0x6E8E[i].next_0 - Zero_pointer);
		to->struct_0x6E8E[i].dword_0x4 = from->struct_0x6E8E[i].maxLife_0x4;
		to->struct_0x6E8E[i].dword_0x8 = from->struct_0x6E8E[i].life_0x8;
		to->struct_0x6E8E[i].struct_byte_0xc_12_15 = from->struct_0x6E8E[i].struct_byte_0xc_12_15;
		to->struct_0x6E8E[i].dword_0x10_16 = from->struct_0x6E8E[i].dword_0x10_16;
		to->struct_0x6E8E[i].word_0x14_20 = from->struct_0x6E8E[i].rand_0x14_20;
		to->struct_0x6E8E[i].word_0x16_22 = from->struct_0x6E8E[i].oldMapEntity_0x16_22;
		to->struct_0x6E8E[i].word_0x18_24_next_entity = from->struct_0x6E8E[i].nextEntity_0x18_24;
		to->struct_0x6E8E[i].word_0x1A_26 = from->struct_0x6E8E[i].id_0x1A_26;
		to->struct_0x6E8E[i].word_0x1C_28 = from->struct_0x6E8E[i].yaw_0x1C_28;
		to->struct_0x6E8E[i].word_0x1E_30 = from->struct_0x6E8E[i].pitch_0x1E_30;
		to->struct_0x6E8E[i].word_0x20_32 = from->struct_0x6E8E[i].roll_0x20_32;
		to->struct_0x6E8E[i].word_0x22_34 = from->struct_0x6E8E[i].fov_0x22_34;
		to->struct_0x6E8E[i].word_0x24_36 = from->struct_0x6E8E[i].word_0x24_36;
		to->struct_0x6E8E[i].word_0x26_38 = from->struct_0x6E8E[i].word_0x26_38;
		to->struct_0x6E8E[i].word_0x28_40 = from->struct_0x6E8E[i].parentId_0x28_40;
		to->struct_0x6E8E[i].word_0x2A_42 = from->struct_0x6E8E[i].subSpellIndex_0x2A_42;
		to->struct_0x6E8E[i].word_0x2C_44 = from->struct_0x6E8E[i].word_0x2C_44;
		to->struct_0x6E8E[i].word_0x2E_46 = from->struct_0x6E8E[i].word_0x2E_46;
		to->struct_0x6E8E[i].word_0x30_48 = from->struct_0x6E8E[i].word_0x30_48;
		to->struct_0x6E8E[i].word_0x32_50 = from->struct_0x6E8E[i].word_0x32_50;
		to->struct_0x6E8E[i].word_0x34_52 = from->struct_0x6E8E[i].word_0x34_52;
		to->struct_0x6E8E[i].word_0x36_54 = from->struct_0x6E8E[i].word_0x36_54;
		to->struct_0x6E8E[i].byte_0x38_56 = from->struct_0x6E8E[i].byte_0x38_56;
		to->struct_0x6E8E[i].byte_0x39_57 = from->struct_0x6E8E[i].byte_0x39_57;
		to->struct_0x6E8E[i].byte_0x3A_58 = from->struct_0x6E8E[i].byte_0x3A_58;
		to->struct_0x6E8E[i].byte_0x3B_59 = from->struct_0x6E8E[i].byte_0x3B_59;
		to->struct_0x6E8E[i].byte_0x3C_60 = from->struct_0x6E8E[i].byte_0x3C_60;
		to->struct_0x6E8E[i].byte_0x3D_61 = from->struct_0x6E8E[i].fontTypeIndex_0x3D_61;
		to->struct_0x6E8E[i].byte_0x3E_62 = from->struct_0x6E8E[i].byte_0x3E_62;
		to->struct_0x6E8E[i].type_0x3F_63 = from->struct_0x6E8E[i].class_0x3F_63;
		to->struct_0x6E8E[i].subtype_0x40_64 = from->struct_0x6E8E[i].model_0x40_64;
		to->struct_0x6E8E[i].byte_0x41_65 = from->struct_0x6E8E[i].xtype_0x41_65;
		to->struct_0x6E8E[i].byte_0x42_66 = from->struct_0x6E8E[i].xsubtype_0x42_66;
		to->struct_0x6E8E[i].byte_0x43_67 = from->struct_0x6E8E[i].byte_0x43_67;
		to->struct_0x6E8E[i].byte_0x44_68 = from->struct_0x6E8E[i].byte_0x44_68;
		to->struct_0x6E8E[i].byte_0x45_69 = from->struct_0x6E8E[i].actionIndex_0x45_69;
		to->struct_0x6E8E[i].byte_0x46_70 = from->struct_0x6E8E[i].byte_0x46_70;
		to->struct_0x6E8E[i].byte_0x47_71_xx = from->struct_0x6E8E[i].byte_0x47_71_xx;
		to->struct_0x6E8E[i].byte_0x48_72 = from->struct_0x6E8E[i].StageVar1_0x48_72;
		to->struct_0x6E8E[i].byte_0x49_73 = from->struct_0x6E8E[i].StageVar2_0x49_73;
		to->struct_0x6E8E[i].word_0x4A_74 = from->struct_0x6E8E[i].word_0x4A_74;
		to->struct_0x6E8E[i].array_0x4C_76 = from->struct_0x6E8E[i].position_0x4C_76;
		to->struct_0x6E8E[i].array_0x52_82 = from->struct_0x6E8E[i].array_0x52_82;
		to->struct_0x6E8E[i].word_0x5A_90 = from->struct_0x6E8E[i].word_0x5A_90;
		to->struct_0x6E8E[i].byte_0x5C_92 = from->struct_0x6E8E[i].animationFrame_0x5C_92;
		to->struct_0x6E8E[i].byte_0x5D_93 = from->struct_0x6E8E[i].byte_0x5D_93;
		to->struct_0x6E8E[i].str_0x5E_94 = from->struct_0x6E8E[i].str_0x5E_94;
		to->struct_0x6E8E[i].word_0x82_130 = from->struct_0x6E8E[i].actSpeed_0x82_130;
		to->struct_0x6E8E[i].word_0x84_132 = from->struct_0x6E8E[i].minSpeed_0x84_132;
		to->struct_0x6E8E[i].word_0x86_134 = from->struct_0x6E8E[i].maxSpeed_0x86_134;
		to->struct_0x6E8E[i].dword_0x88_136 = from->struct_0x6E8E[i].manaRegen_0x88_136;
		to->struct_0x6E8E[i].dword_0x8C_140 = from->struct_0x6E8E[i].maxMana_0x8C_140;
		to->struct_0x6E8E[i].dword_0x90_144 = from->struct_0x6E8E[i].mana_0x90_144;
		to->struct_0x6E8E[i].playerEntityIndex_0x94_148 = from->struct_0x6E8E[i].playerEntityIndex_0x94_148;
		to->struct_0x6E8E[i].word_0x96_150 = from->struct_0x6E8E[i].word_0x96_150;
		to->struct_0x6E8E[i].word_0x98_152 = from->struct_0x6E8E[i].word_0x98_152;
		to->struct_0x6E8E[i].axis_0x9A_154x = from->struct_0x6E8E[i].axis_0x9A_154x;
		to->struct_0x6E8E[i].dword_0xA0_160x = ((uint8_t*)from->struct_0x6E8E[i].dword_0xA0_160x - Zero_pointer);
		to->struct_0x6E8E[i].dword_0xA4_164x = ((uint8_t*)from->struct_0x6E8E[i].dword_0xA4_164x - Zero_pointer);
	}

	to->str_2FECE.word_2FECE = from->terrain_2FECE.word_2FECE;
	to->str_2FECE.levelID_2FED0 = from->terrain_2FECE.levelID_2FED0;
	to->str_2FECE.byte_0x2FED2 = from->terrain_2FECE.byte_0x2FED2;
	to->str_2FECE.byte_0x2FED3 = from->terrain_2FECE.byte_0x2FED3;
	to->str_2FECE.MapType = static_cast<std::underlying_type<MapType_t>::type>(from->terrain_2FECE.MapType);
	to->str_2FECE.word_0x2FED5 = from->terrain_2FECE.word_0x2FED5;
	to->str_2FECE.word_0x2FED7 = from->terrain_2FECE.word_0x2FED7;
	for (int i = 0; i < 8; i++)to->str_2FECE.array_0x2FED9[i] = from->terrain_2FECE.player_0x2FED9[i];
	for (int i = 0; i < 4; i++)to->str_2FECE.stubb[i] = from->terrain_2FECE.stubb[i];
	to->str_2FECE.word_0x2FEE5 = from->terrain_2FECE.seed_0x2FEE5;
	for (int i = 0; i < 2; i++)to->str_2FECE.stubb2[i] = from->terrain_2FECE.stubb2[i];
	to->str_2FECE.word_0x2FEE9 = from->terrain_2FECE.offset_0x2FEE9;
	for (int i = 0; i < 2; i++)to->str_2FECE.stubc[i] = from->terrain_2FECE.stubc[i];
	to->str_2FECE.word_0x2FEED = from->terrain_2FECE.raise_0x2FEED;
	for (int i = 0; i < 2; i++)to->str_2FECE.stubc2[i] = from->terrain_2FECE.stubc2[i];
	to->str_2FECE.word_0x2FEF1 = from->terrain_2FECE.gnarl_0x2FEF1;
	for (int i = 0; i < 2; i++)to->str_2FECE.stubc3[i] = from->terrain_2FECE.stubc3[i];
	to->str_2FECE.word_0x2FEF5 = from->terrain_2FECE.river_0x2FEF5;
	to->str_2FECE.word_0x2FEF9 = from->terrain_2FECE.lriver_0x2FEF9;
	for (int i = 0; i < 2; i++)to->str_2FECE.stube[i] = from->terrain_2FECE.stube[i];
	to->str_2FECE.word_0x2FEFD = from->terrain_2FECE.source_0x2FEFD;
	for (int i = 0; i < 2; i++)to->str_2FECE.stubf[i] = from->terrain_2FECE.stubf[i];
	to->str_2FECE.word_0x2FF01 = from->terrain_2FECE.snLin_0x2FF01;
	for (int i = 0; i < 2; i++)to->str_2FECE.stubg[i] = from->terrain_2FECE.stubg[i];
	to->str_2FECE.word_0x2FF05 = from->terrain_2FECE.snFlt_0x2FF05;
	for (int i = 0; i < 2; i++)to->str_2FECE.stubh[i] = from->terrain_2FECE.stubh[i];
	to->str_2FECE.word_0x2FF09 = from->terrain_2FECE.bhLin_0x2FF09;
	for (int i = 0; i < 2; i++)to->str_2FECE.stubi[i] = from->terrain_2FECE.stubi[i];
	to->str_2FECE.word_0x2FF0D = from->terrain_2FECE.bhFlt_0x2FF0D;
	for (int i = 0; i < 2; i++)to->str_2FECE.stubj[i] = from->terrain_2FECE.stubj[i];
	to->str_2FECE.word_0x2FF11 = from->terrain_2FECE.rkSte_0x2FF11;
	for (int i = 0; i < 1022; i++)to->str_2FECE.stubk[i] = from->terrain_2FECE.stubk[i];
	for (int i = 0; i < 1200; i++)to->str_2FECE.array_0x30311[i] = from->terrain_2FECE.entity_0x30311[i];
	to->str_2FECE.next_0x360D1 = from->terrain_2FECE.next_0x360D1;
	for (int i = 0; i < 8; i++)to->str_2FECE.WizardMapSettings_0x360D2[i] = from->terrain_2FECE.WizardMapSettings_0x360D2[i];
	for (int i = 0; i < 8; i++)to->str_2FECE.str_0x36442[i] = from->terrain_2FECE.stages_0x36442[i];
	for (int i = 0; i < 11; i++)
	{
		to->str_2FECE.array_0x3647A[i].str_0x3647A_byte_0 = from->terrain_2FECE.StageVars_0x3647A[i].index_0x3647A_0;
		to->str_2FECE.array_0x3647A[i].str_0x3647A_byte_1 = from->terrain_2FECE.StageVars_0x3647A[i].stage_0x3647A_1;
		to->str_2FECE.array_0x3647A[i].str_0x3647A_2 = from->terrain_2FECE.StageVars_0x3647A[i].str_0x3647A_2;
		to->str_2FECE.array_0x3647A[i].str_0x3647C_4.axis = from->terrain_2FECE.StageVars_0x3647A[i].str_0x3647C_4.axis;
	}
	to->str_0x364D2 = from->str_0x364D2;
	to->word_0x3653E = from->word_0x3653E;
	to->word_0x36540 = from->word_0x36540;
	to->word_0x36542 = from->word_0x36542;
	to->word_0x36544 = from->word_0x36544;
	to->word_0x36546 = from->word_0x36546;
	to->word_0x36548 = from->word_0x36548;
	to->word_0x3654A = from->word_0x3654A;
	for (int i = 0; i < 8; i++) {
		to->struct_0x3654C[i].str_3654C_byte0 = from->stages_0x3654C[i].stages_3654C_byte0;
		to->struct_0x3654C[i].str_3654D_byte1 = from->stages_0x3654C[i].str_3654D_byte1;
		to->struct_0x3654C[i].str_3654E_axis = from->stages_0x3654C[i].str_3654E_axis;
		to->struct_0x3654C[i].str_36552_un.dword = from->stages_0x3654C[i].str_36552_un.dword;
	}
	for (int i = 0; i < 8; i++)to->struct_0x3659C[i] = from->struct_0x3659C[i];
	for (int i = 0; i < 11; i++) {
		to->array_0x365F4[i].str_0x3647A_byte_0 = from->StageVars2_0x365F4[i].index_0x3647A_0;
		to->array_0x365F4[i].str_0x3647A_byte_1 = from->StageVars2_0x365F4[i].stage_0x3647A_1;
		to->array_0x365F4[i].str_0x3647A_2 = from->StageVars2_0x365F4[i].str_0x3647A_2;
		to->array_0x365F4[i].str_0x3647C_4.axis = from->StageVars2_0x365F4[i].str_0x3647C_4.axis;
	}
	for (int i = 0; i < 0x32; i++) {
		to->str_0x3664C[i].byte_0 = from->str_0x3664C[i].byte_0;
		to->str_0x3664C[i].byte_1 = from->str_0x3664C[i].byte_1;
		to->str_0x3664C[i].byte_2 = from->str_0x3664C[i].byte_2;
		to->str_0x3664C[i].byte_3 = from->str_0x3664C[i].byte_3;
		to->str_0x3664C[i].axis3d_4 = from->str_0x3664C[i].axis3d_4;
		to->str_0x3664C[i].dword_A = ((uint8_t*)from->str_0x3664C[i].event_A.pointer_0x6E8E - Zero_pointer);
		for (int j = 0; j < 25; j++)to->str_0x3664C[i].array_E[j] = from->str_0x3664C[i].array_E[j];
	}
	to->byte_0x36DEA_fly_asistant = from->byte_0x36DEA_fly_asistant;
	to->byte_0x36DEB_xx = from->byte_0x36DEB_xx;
	to->word_0x36DEC_mousex = from->word_0x36DEC_mousex;
	to->word_0x36DEE_mousey = from->word_0x36DEE_mousey;
	to->word_0x36DF0_mousexx = from->word_0x36DF0_mousexx;
	to->dword_0x36DF2 = from->dword_0x36DF2;
	to->dword_0x36DF6 = ((uint8_t*)from->dword_0x36DF6 - Zero_pointer);
	to->word_0x36DFA = from->word_0x36DFA;
	to->word_0x36DFC = from->word_0x36DFC;
	to->word_0x36DFE = from->word_0x36DFE;
	to->byte_0x36E00 = from->countStageVars_0x36E00;
	to->byte_0x36E01 = from->stageIndex_0x36E01;
	to->byte_0x36E02 = from->byte_0x36E02;
	to->byte_0x36E03 = from->byte_0x36E03;
	to->byte_counter_current_objective_box_0x36E04 = from->byte_counter_current_objective_box_0x36E04;
	for (int i = 0; i < 6; i++)to->stub3k[i] = from->stub3k[i];
	to->byte_0x36E0B = from->byte_0x36E0B;
	for (int i = 0; i < 11; i++)to->stubend[i] = from->stubend[i];
}

void Convert_to_shadow_str_E2A74(const type_array_str_E2A74 &from, type_shadow_str_E2A74* to)
{
	for (int j = 0; j < from.size(); ++j) {
		to[j].word_0 = from[j].word_0;
		for (int i = 0; i < from[j].axis_2.size(); i++)
			to[j].axis_2[i] = from[j].axis_2[i];
		to[j].dword_12 = ((uint8_t*)from[j].dword_12 - Zero_pointer);
		to[j].dword_16 = from[j].dword_16;
		to[j].dword_20 = from[j].dword_20;
		to[j].dword_24 = from[j].dword_24;
		to[j].byte_28 = from[j].byte_28;
		to[j].byte_29 = from[j].byte_29;
	}
}

unsigned long clock_value()
{
	return mygethundredths();
}
unsigned long _clock()
{
	return clock_value();
}
unsigned long j___clock() {
	return _clock();
}; // weak

void SetPlayerLocation(uint16_t x, uint16_t y, int16_t z, int16_t pitch, int16_t roll, int16_t yaw)
{
	if (Entities_EA3E4[D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240] != nullptr)
	{
		auto ptrPlayerEntity = Entities_EA3E4[D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
		ptrPlayerEntity->position_0x4C_76.x = x;
		ptrPlayerEntity->position_0x4C_76.y = y;
		ptrPlayerEntity->position_0x4C_76.z = z;

		ptrPlayerEntity->array_0x52_82.pitch = pitch;
		ptrPlayerEntity->array_0x52_82.roll = roll;
		ptrPlayerEntity->array_0x52_82.yaw = yaw;
	}
}

void fix_sub_9A0FC_wait_to_screen_beam(int32_t delay)//27B0fc
{
	VGA_Blit(nullptr);
	if (delay > 0)
		mydelay(delay);
}
