#include "Basic.h"
#include "engine_support.h"
#include "CommandLineParser.h"

std::string gameDataPath;
std::string cdDataPath;
std::string bigGraphicsPath;

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

std::chrono::time_point<std::chrono::steady_clock> frameStart;
std::chrono::duration<double> timeDelta(0);
int frameCount = 0;
int fps = 0;

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

uint8_t* x_DWORD_180708; // weak
uint8_t* x_DWORD_18070C; // weak
uint8_t* x_DWORD_180714; // weak
uint8_t* x_DWORD_180718; // weak

//basic graphics

char x_BYTE_D41CE = 0; // weak
uint8_t* x_DWORD_F01E4; // weak
uint16_t x_WORD_F01F4; // weak
char x_BYTE_D41C1 = 0; // weak
char* x_DWORD_D41D0 = 0; // weak
__int16 x_WORD_E36D4 = 0; // weak
char x_BYTE_EB3B6; // weak
type_x_DWORD_17DE38str x_DWORD_17DE38str;
uint8_t* x_DWORD_E9C38_smalltit;//*(x_DWORD*)(x_DWORD_E9C38_smalltit + 36964)
type_F2C20ar str_F2C20ar;

int help_VGA_type_resolution = 0;

int16_t x_WORD_180660_VGA_type_resolution; // weak
uint8_t x_BYTE_E88E0x[32];
uint8_t unk_F0A20x[1024];
char isCaveLevel_D41B6 = 1;
__int16 x_WORD_D4B7C = 254; // some key color?
__int16 x_WORD_D4B7E = 0; // some key color?
type_event_0x6E8E* x_DWORD_EA3E4[1001];//2bb3e4

uint8_t x_BYTE_F6EE0_tablesx[83456];// (uint8_t*)&x_BYTE_F6EE0_tablesbuff;//animated sprites
uint8_t* x_BYTE_F6EE0_tablesx_pre = (uint8_t*)x_BYTE_F6EE0_tablesx;
uint8_t* x_BYTE_FAEE0_tablesx_pre = (uint8_t*)&x_BYTE_F6EE0_tablesx[16384];

type_E9C38_smalltit Str_E9C38_smalltit[21 * 40];

uint8_t* ViewPortRenderBufferStart_DE558 = 0;

uint8_t* ViewPortRenderBufferAltStart_DE554 = 0;

//language
char* x_DWORD_E9C4C_langindexbuffer[1000]; // idb


//file

int filearrayindex_POINTERSDATTAB = 0;
int filearrayindex_FONTS0DATTAB = 1;
int filearrayindex_MSPRDDATTAB = 2;
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
posistruct2_t* FONTS0TAB_BEGIN_BUFFER = 0;
posistruct2_t* FONTS0TAB_END_BUFFER = 0;
uint8_t* FONTS1DAT_BEGIN_BUFFER = 0;//2baa94
posistruct2_t* FONTS1TAB_BEGIN_BUFFER = 0;//2bab24
posistruct2_t* FONTS1TAB_END_BUFFER = 0;//2bab04
type_TMAPS00TAB_BEGIN_BUFFER* str_TMAPS00TAB_BEGIN_BUFFER;
//uint8_t* TMAPS00TAB_BEGIN_BUFFER = 0;//2c7ed0
TColor* str_PALDATA_BEGIN_BUFFER = 0;//2bb3c8
TColor* str_PALMEM_BEGIN_BUFFER = 0;//6d654d
uint8_t* POINTERSDAT_BEGIN_BUFFER = 0;//2bc390
posistruct2_t* POINTERSTAB_BEGIN_BUFFER = 0;//2bc394
posistruct2_t* POINTERSTAB_END_BUFFER = 0;//2bc388*/

uint8_t* BUILD00DAT_BEGIN_BUFFER = 0;
posistruct2_t* BUILD00TAB_BEGIN_BUFFER = 0;
posistruct2_t* BUILD00TAB_END_BUFFER = 0;

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
posistruct2_t* MSPRD00TAB_BEGIN_BUFFER = 0;
posistruct2_t* MSPRD00TAB_END_BUFFER = 0;

uint8_t* HSPRD00DAT_BEGIN_BUFFER = 0;
posistruct2_t* HSPRD00TAB_BEGIN_BUFFER = 0;
posistruct2_t* HSPRD00TAB_END_BUFFER = 0;

uint8_t* HFONT3DAT_BEGIN_BUFFER = 0;
posistruct2_t* HFONT3TAB_BEGIN_BUFFER = 0;
posistruct2_t* HFONT3TAB_END_BUFFER = 0;

uint8_t* CLRD0DAT_BEGIN_BUFFER = 0;

posistruct_t* posistruct1;//fix it
posistruct_t* posistruct2;//fix it
posistruct_t* posistruct3;//fix it
posistruct_t* posistruct4;//fix it
posistruct_t* posistruct5;//fix it
posistruct_t* posistruct6;//fix it
posistruct_t* posistruct7;//fix it
posistruct_t* posistruct8;//fix it
posistruct_t* posistruct9;//fix it
posistruct_t* posistruct10;//fix it
posistruct_t* posistruct11;//fix it

uint8_t* ZERO_BUFFER = 0;
posistruct2_t* ZERO_BUFFER_PST2 = 0;

posistruct_t* x_DWORD_EA3D4; // weak

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

filearray_struct x_DWORD_E9B20[4] = { filearray_2aa18c[filearrayindex_FONTS0DATTAB],filearray_2aa18c[filearrayindex_MSPRDDATTAB],
filearray_2aa18c[filearrayindex_HFONT3DATTAB],filearray_2aa18c[filearrayindex_MSPRDDATTAB] };

type_SPELLS_BEGIN_BUFFER_str SPELLS_BEGIN_BUFFER_str[26] =
{ {0x03,0x00,{
{0x000000FA,0x00000064,0x00000000,0x00000000,0x00000000,0x00BA,0x0005,0x00,0x00},
{0x000000A0,0x000000FA,0x00000000,0x00000190,0x00000096,0x00BB,0x000B,0x01,0x01},
{0x00000050,0x000005DC,0x000009C4,0x00001388,0x000002EE,0x00BC,0x000F,0x02,0x00}}},
{0x03,0x00,{
{0x0000000A,0x00000064,0x00000000,0x00000000,0x00000000,0x00BD,0x0003,0x00,0x00},
{0x0000000F,0x000000FA,0x00000000,0x0000012C,0x00000032,0x00BE,0x0029,0x01,0x00},
{0x00000014,0x000003E8,0x000003E8,0x00000384,0x00000096,0x00BF,0x0033,0x02,0x00}}},
{0x03,0x00,{
{0x00000000,0x000003E8,0x00000000,0x00000000,0x00000000,0x00C0,0x0065,0x00,0x00},
{0x00000000,0x000004E2,0x00000000,0x00000019,0x00000004,0x00C1,0x0065,0x01,0x00},
{0x00000001,0x000007D0,0x00000000,0x0000007D,0x00000006,0x00C2,0x0065,0x02,0x00}}},
{0x03,0x04,{
{0x00000002,0x000003E8,0x00000000,0x00000000,0x00000000,0x00C3,0x012D,0x00,0x00},
{0x00000003,0x000009C4,0x00000000,0x0000008C,0x0000000F,0x00C4,0x01C3,0x00,0x00},
{0x00000004,0x00001388,0x00000000,0x00000118,0x0000002D,0x00C5,0x01F5,0x00,0x00}}},
{0x03,0x04,{
{0x00000064,0x00001388,0x00004E20,0x00000000,0x00000000,0x00C6,0x00C9,0x13,0x00},
{0x00000064,0x00001D4C,0x00004E20,0x00000050,0x00000014,0x00C8,0x012D,0x19,0x00},
{0x00000064,0x00003A98,0x000061A8,0x000000F0,0x0000003C,0x00C9,0x01C7,0x10,0x00}}},
{0x03,0x00,{
{0x00000001,0x000001F4,0x00000000,0x00000000,0x00000000,0x00CA,0x000B,0x00,0x00},
{0x00000001,0x000002EE,0x00000000,0x0000012C,0x0000004B,0x00CB,0x0015,0x00,0x00},
{0x0000000A,0x000005DC,0x00000000,0x00000384,0x0000012C,0x00CC,0x0029,0x00,0x00}}},
{0x03,0x04,{
{0x00000064,0x000003E8,0x00000000,0x00000000,0x00000000,0x00CD,0x0065,0x00,0x00},
{0x00000064,0x000005DC,0x00000000,0x00000078,0x0000001E,0x00CE,0x00C9,0x00,0x00},
{0x00000064,0x00002710,0x00000000,0x00000118,0x00000046,0x00CF,0x012D,0x01,0x00}}},
{0x03,0x00,{
{0x000000C8,0x000001F4,0x000002EE,0x00000000,0x00000000,0x00D0,0x0005,0x00,0x01},
{0x000000FA,0x000009C4,0x00001388,0x00000258,0x00000096,0x00D1,0x0023,0x01,0x00},
{0x00000320,0x00002710,0x00004E20,0x00001D4C,0x000001F4,0x00D2,0x002D,0x02,0x00}}},
{0x03,0x04,{
{0x00000000,0x000009C4,0x00001388,0x00000000,0x00000000,0x00D3,0x007D,0x00,0x00},
{0x00000000,0x00000EA6,0x00001D4C,0x00000078,0x0000001E,0x00D4,0x00FB,0x00,0x00},
{0x00000000,0x00001B58,0x00002710,0x000000F0,0x0000003C,0x00D5,0x007D,0x01,0x00}}},
{0x03,0x00,{
{0x000006D6,0x000003E8,0x00001388,0x00000000,0x00000000,0x00D6,0x0009,0x02,0x00},
{0x00000DAC,0x000005DC,0x00002328,0x00000352,0x00000032,0x00D7,0x0011,0x05,0x00},
{0x00001B58,0x000007D0,0x00004650,0x0000092E,0x00000096,0x00D8,0x0019,0x0A,0x00}}},
{0x03,0x00,{
{0x00000000,0x00001388,0x00001388,0x00000000,0x00000000,0x00D9,0x000B,0x00,0x00},
{0x00000000,0x00001770,0x00001388,0x000000C8,0x0000001E,0x00DA,0x000B,0x01,0x00},
{0x00000000,0x00001D4C,0x00001388,0x00000190,0x0000005A,0x00DB,0x0015,0x02,0x00}}},
{0x03,0x04,{
{0x00000000,0x000007D0,0x000007D0,0x00000000,0x00000000,0x00DC,0x00B5,0x01,0x00},
{0x00000000,0x000009C4,0x00000BB8,0x00000064,0x0000001E,0x00DD,0x00B7,0x02,0x00},
{0x00000000,0x00000BB8,0x00000FA0,0x0000012C,0x0000005A,0x00DE,0x00B7,0x03,0x00}}},
{0x03,0x04,{
{0x00000000,0x00000708,0x00000000,0x00000000,0x00000000,0x00DF,0x0097,0x00,0x00},
{0x00000000,0x00000960,0x00002710,0x00000078,0x0000001E,0x00E0,0x0105,0x00,0x00},
{0x00000000,0x00001388,0x00002710,0x00000168,0x0000003C,0x00E1,0x0169,0x00,0x00}}},
{0x03,0x00,{
{0x000007D0,0x000001F4,0x00002710,0x00000000,0x00000000,0x00E2,0x0005,0x00,0x00},
{0x00000FA0,0x000003E8,0x00003A98,0x00000190,0x00000064,0x00E3,0x0007,0x00,0x00},
{0x0000000A,0x000004B0,0x00002EE0,0x00000320,0x000000C8,0x00E4,0x0015,0x01,0x00}}},
{0x03,0x04,{
{0x00001432,0x000009C4,0x00002710,0x00000000,0x00000000,0x00E5,0x00C3,0x00,0x00},
{0x00001E28,0x00000DAC,0x00003A98,0x00000064,0x00000032,0x00E6,0x018B,0x01,0x00},
{0x00001E28,0x00001388,0x00004E20,0x000000C8,0x00000064,0x00E7,0x025B,0x02,0x00}}},
{0x03,0x00,{
{0x00000014,0x00001388,0x00002710,0x00000000,0x00000000,0x00E8,0x001F,0x3C,0x00},
{0x00000028,0x00001D4C,0x00004E20,0x000003E8,0x00000028,0x00E9,0x0029,0x50,0x00},
{0x00000032,0x00002710,0x00009C40,0x000007D0,0x00000064,0x00EA,0x003D,0x78,0x00}}},
{0x03,0x00,{
{0x00000032,0x00001770,0x00003A98,0x00000000,0x00000000,0x00EB,0x001F,0x06,0x00},
{0x00000064,0x00002328,0x00007530,0x000005DC,0x00000028,0x00EC,0x0029,0x0C,0x00},
{0x0000012C,0x00002EE0,0x0000EA60,0x00000BB8,0x00000050,0x00ED,0x0033,0x18,0x00}}},
{0x03,0x00,{
{0x00000050,0x00002710,0x00004650,0x00000000,0x00000000,0x00EE,0x0015,0x10,0x00},
{0x00000078,0x00002EE0,0x00008CA0,0x00000708,0x0000003C,0x00EF,0x001F,0x20,0x00},
{0x00000140,0x00003A98,0x00011940,0x00000E10,0x00000078,0x00F0,0x0029,0x40,0x00}}},
{0x03,0x00,{
{0x00000078,0x00002EE0,0x00004E20,0x00000000,0x00000000,0x00F1,0x003F,0x07,0x00},
{0x000000F0,0x00003A98,0x00009C40,0x000007D0,0x00000028,0x00F2,0x0049,0x09,0x00},
{0x000001E0,0x00004650,0x00013880,0x00000FA0,0x00000050,0x00F3,0x0053,0x0B,0x00}}},
{0x03,0x00,{
{0x00000064,0x00002710,0x00004E20,0x00000000,0x00000000,0x00F4,0x012B,0x13,0x00},
{0x00000064,0x00004E20,0x00009C40,0x00000050,0x00000028,0x00F6,0x018F,0x19,0x00},
{0x00000064,0x00009C40,0x00015F90,0x000000A0,0x00000050,0x00F7,0x01F3,0x10,0x00}}},
{0x03,0x00,{
{0x000002EE,0x00003A98,0x00007530,0x00000000,0x00000000,0x00F8,0x001F,0x10,0x00},
{0x000005DC,0x00004E20,0x0000EA60,0x000009C4,0x00000019,0x00F9,0x0033,0x1A,0x00},
{0x00000BB8,0x00007530,0x00015F90,0x00001388,0x00000032,0x00FA,0x003D,0x28,0x00}}},
{0x03,0x00,{
{0x00000064,0x000007D0,0x00000000,0x00000000,0x00000000,0x00FB,0x001F,0x05,0x00},
{0x00000064,0x000009C4,0x000009C4,0x000000C8,0x00000032,0x00FC,0x0033,0x0A,0x00},
{0x000000C8,0x00001388,0x00002710,0x00000190,0x00000064,0x00FD,0x003D,0x0A,0x00}}},
{0x03,0x00,{
{0x000000FA,0x000003E8,0x00001388,0x00000000,0x00000000,0x00FE,0x001F,0x00,0x00},
{0x000001F4,0x000005DC,0x00002710,0x00000050,0x0000001E,0x00FF,0x001F,0x01,0x00},
{0x000003E8,0x000007D0,0x00002710,0x000000A0,0x0000003C,0x0100,0x001F,0x02,0x00}}},
{0x03,0x00,{
{0x000003E8,0x00002710,0x00004E20,0x00000000,0x00000000,0x0101,0x002F,0x00,0x00},
{0x00001388,0x00002EE0,0x00004E20,0x00000078,0x0000001E,0x0102,0x0043,0x01,0x00},
{0x00002710,0x00005DC0,0x0000EA60,0x000000F0,0x0000003C,0x0103,0x0057,0x02,0x00}}},
{0x03,0x00,{
{0x00000262,0x00002710,0x00004E20,0x00000000,0x00000000,0x0104,0x002B,0x10,0x00},
{0x0000044C,0x00004650,0x00008CA0,0x0000005A,0x0000001E,0x0105,0x0021,0x1A,0x00},
{0x00000A96,0x00007530,0x00015F90,0x00000118,0x00000046,0x0106,0x0021,0x20,0x00}}},
{0x03,0x00,{
{0x00000064,0x00002AF8,0x00004A38,0x00000000,0x00000000,0x0107,0x001F,0x00,0x00},
{0x00000082,0x000032C8,0x00009470,0x000000C8,0x00000028,0x0108,0x0029,0x01,0x00},
{0x00000186,0x00006590,0x0000DAC0,0x00000190,0x00000078,0x0109,0x0033,0x02,0x00}}}
};

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
Pathstruct xabscreen2 = { "*BScreen\0",&x_DWORD_E9C38_smalltit,&BSCREEN2_END_BUFFER,0x11170/*0x011508*/,0 };
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

bool DefaultResolutions()
{
	if ((screenWidth_18062C <= 640) && (screenHeight_180624 <= 480))
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

//----- (00084000) --------------------------------------------------------
int sub_84000(int a1)//265000
{
	int result; // eax
	int v2; // edx
	int v3; // ebx

	result = a1;
	if (*(x_DWORD*)(a1 + 12))
	{
		*(x_BYTE*)(a1 + 16) = 0;
		v2 = *(x_DWORD*)(a1 + 12);
		if (*(x_BYTE*)(a1 + 17) == *(x_BYTE*)(v2 + 17) && !*(x_BYTE*)(v2 + 16))
		{
			v3 = *(x_DWORD*)(a1 + 8);
			if (v3)
				*(x_DWORD*)(v3 + 12) = v2;
			*(x_DWORD*)(*(x_DWORD*)(a1 + 12) + 8) = *(x_DWORD*)(a1 + 8);
			*(x_DWORD*)(*(x_DWORD*)(a1 + 12) + 4) += *(x_DWORD*)(a1 + 4);
			*(x_DWORD*)(a1 + 4) = 0;
		}
	}
	return result;
}

x_DWORD x_outp(x_DWORD, char) { stub_fix_it(); return 0; };// weak
x_DWORD x_inp(x_DWORD) { stub_fix_it(); return 0; };// weak
void stub_fix_it() { allert_error(); };

void dbgfprintf(FILE* file, const char* format, ...) {
	//void dbgfprintf(FILE* file,char* str) {
	fprintf(file, format);
	fprintf(file, "\n");
}

void* Malloc_83CD0(size_t a1)//264cd0
{
	return malloc(a1);
}

void qmemcpy(void* a, void* b, size_t c) {
	memcpy(a, b, c);
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
signed int GetFileLenght_9DE20(char* a1)//27ee20
{
	signed int v2; // [esp+0h] [ebp-Ch]
	signed int v3; // [esp+4h] [ebp-8h]
	FILE* v4; // [esp+8h] [ebp-4h]

	readFileStatus_E3E2C = 0;
	v4 = x_open(a1, 512);
	if (v4 == NULL)
	{
		readFileStatus_E3E2C = 3;
		v2 = -1;
	}
	else
	{
		v3 = DataFileIO::FileLengthBytes(v4);
		if (v3 == -1)
			readFileStatus_E3E2C = 5;
		DataFileIO::Close(v4);
		v2 = v3;
	}
	return v2;
}
// 988DA: using guessed type x_DWORD filelength(x_DWORD);
// A0855: using guessed type x_DWORD close(x_DWORD);
// E3E2C: using guessed type int x_DWORD_E3E2C;

FILE* x_open(char* path, int  /*pmodex*/) {
	return myopent(path, (char*)"rb");
};

//----- (0009D380) --------------------------------------------------------
int sub_9D380(FILE* a1, int a2, char a3, uint8_t* a4x, int a5)//27e380
{
	if (a3 & 1)
	{
		memcpy((void*)a4x, (const void*)(a2 + a1), a5);
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
	if (x_BYTE_D41CE)
		memset((void*)x_DWORD_F01E4, 0, x_WORD_F01F4);
}

//----- (0002EB60) --------------------------------------------------------
void sub_2EB60()//20fb60
{
	//int result; // eax

	if (!x_BYTE_D41CE)
	{
		x_BYTE_D41CE = 1;
		x_BYTE_D41C1 = 1;
		x_DWORD_F01E4 = &pdwScreenBuffer_351628[64320];//line 320x201 - after normal image
		x_WORD_F01F4 = 0x3200;//320*40
		sub_2EC30_clear_img_mem();
		x_DWORD_D41D0 = 0;
	}
}

//----- (0002EBB0) --------------------------------------------------------
void sub_2EBB0_draw_text_with_border_630x340(char* textString)//20fbb0
{
	if (x_BYTE_D41CE)
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
int sub_7FCB0_draw_text_with_border(/*int a1,*/ char* textString, int32_t a3, int32_t a4, int a5, uint8_t a6, unsigned __int8 a7, uint32_t a8)//260cb0
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
		v12 = getPaletteIndex_5BE80(x_DWORD_17DE38str.x_DWORD_17DE38x, 0, 0, 0/*v86*/);
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
					DrawHelpText_6FC50(1/*v86*/);
					v25 = sub_6FC10_letter_width();
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
							DrawHelpText_6FC50(1/*v86*/);
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
							v36 = a4 - 2 * sub_6FC10_letter_width();
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
								sub_7FAE0_draw_text(v87, v41, a4, v98, 0/*v86*/);
							}
							else
							{
								sub_7FAE0_draw_text(v87, v99, a4, v98, v86);
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
							v27 = getPaletteIndex_5BE80(x_DWORD_17DE38str.x_DWORD_17DE38x, 0, 0, 0/*v86*/);
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
						sub_7FAE0_draw_text(v87, v99, a4, v98, 0/*v86*/);//adress 261197 (80197)
					}
					if (a6 && a6 != 4 && a6 != 5)
					{
						LOWORD(v44) = sub_6FC30_get34_height();
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
			DrawHelpText_6FC50(1/*v86*/);
			if (a6 == 2)
			{
				DrawHelpText_6FC50(1/*v86*/);
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
				DrawHelpText_6FC50(1/*v86*/);
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
				v70 = a4 - 2 * sub_6FC10_letter_width();
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
					sub_7FAE0_draw_text(v87, v99 + xy_DWORD_17DED4_spritestr[275].width_4, a4, v98, 0);
				}
				else
				{
					sub_7FAE0_draw_text(v87, v99, a4, v98, v86);
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
				v45 = getPaletteIndex_5BE80(x_DWORD_17DE38str.x_DWORD_17DE38x, 0, 0, 0/*v86*/);
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
				v52 = getPaletteIndex_5BE80(x_DWORD_17DE38str.x_DWORD_17DE38x, 0, 0, 0/*v86*/);//?
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
			sub_7FAE0_draw_text(v87, v99, a4, v98, 0/*v86*/);//draw text(with palette?) 
		}
	}
	if (a6)
		return v98 - v102;
	//HIWORD(v84) = HIWORD(xy_DWORD_17DED4_spritestr);
	v84 = xy_DWORD_17DED4_spritestr[(a8 + 2)].height_5;

	//debug
	//compare_with_snapshot((char*)"0x45678", (uint8_t*)&v8, 0x4589, 4);
	//debug

	return a1 - v102 + v84;
}

//----- (0007C120) --------------------------------------------------------
void sub_7C120_draw_bitmap_640(int16_t posx, int16_t posy, posistruct_t tempstr)//25d120
{
	sub_2BB40_draw_bitmap(posx, posy, tempstr);//ebp
}

//----- (00076260) --------------------------------------------------------
void sub_76260_read_intro_Palette(uint8_t  /*a1*/)
{
	TColor* v0x; // ebx
	int v1; // esi
	//unsigned int result; // eax
	unsigned __int16 v3; // di
	uint16_t v4; // [esp+0h] [ebp-Ch]
	int32_t v5; // [esp+4h] [ebp-8h]
	unsigned __int8 v6; // [esp+8h] [ebp-4h]

	v0x = unk_17D838x;
	v1 = 0;
	sub_75D70((uint8_t*)&v4, 2u);
	if (v4 > 0u)
	{
		do
		{
			sub_75D70((uint8_t*)&v6, 1u);
			//v0 += 3 * v6;
			v0x += v6;
			v5 = 0;
			sub_75D70((uint8_t*)&v5, 1u);
			if (!v5)
				v5 = 256;
			for (v3 = 0; v3 < v5; v3++)//mybe read Palette
			{
				sub_75D70((uint8_t*)v0x, 3u);
				//v0 += 3;
				v0x++;
			}
			v1++;
		} while (v1 < v4);
	}
	//return result;
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
void DrawHelpText_6FC50(__int16 a1)//250c50 //font and graphics init
{
	//int result; // eax
	posistruct_t* v2; // edx

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
uint8_t sub_6FC10_letter_width()//250c10
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
		v6 = a2 + v9 / 2 - (v5 - 1) * sub_6FC10_letter_width() / 2;
		DrawText_2BC10(a1, v6, a4, a5);
	}
	v7 = sub_6FC10_letter_width();
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

	if (x_BYTE_D41CE)
	{
		//result = a1;
		x_BYTE_EB3B6 = a1;
		if (x_DWORD_D41D0)
			/*result = */sub_2EBB0_draw_text_with_border_630x340(x_DWORD_D41D0);
	}
	//return result;
}
// D41CE: using guessed type char x_BYTE_D41CE;
// D41D0: using guessed type int x_DWORD_D41D0;
// EB3B6: using guessed type char x_BYTE_EB3B6;

//----- (0007FAE0) --------------------------------------------------------
uint32_t sub_7FAE0_draw_text(char* text, int16_t a2, int16_t a3, int16_t posy, uint8_t a5)//260ae0
{
	uint32_t helpstrlen; // kr04_4
	int16_t v6; // bx
	int32_t posx; // ebx

	helpstrlen = strlen(text);
	v6 = a3 - a2;//ebx[ebx+1c] - 1e2 edx[ebp+18] - 12a
	if (helpstrlen == 1)
	{
		posx = a2 + v6 / 2;
	}
	else
	{
		posx = v6 / 2 + a2 - xy_DWORD_17DEC0_spritestr[65].width_4 * helpstrlen / 2;
		sub_7FB90_draw_text(text, posx, posy, a5);//"clisk here to" 13d 138 0
	}
	return posx + xy_DWORD_17DEC0_spritestr[65].width_4 * strlen(text);
}
// 17DEC0: using guessed type int (int)x_DWORD_17DEC0;

//int debugcounter_271478 = 0;
//----- (00090478) --------------------------------------------------------
void sub_90478_VGA_Blit320()//271478
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
#ifndef debug_hide_graphics
#if _DEBUG
	VGA_CalculateAndPrintFPS(0, 0);
#endif
	VGA_Blit(pdwScreenBuffer_351628);
#endif
	//if(dos_key_vect_9)dos_key_vect_9();
	//VGA_mouse_clear_keys();

	//set speed
	long actmillis = mygetthousandths();
	long newdelay = speedGame - (actmillis - oldmillis);//max millis is 20 millis
	if (newdelay < 0)newdelay = 0;
	if (newdelay > speedGame)newdelay = speedGame;
	mydelay(newdelay);//set speed
	oldmillis = actmillis;
	//set speed
}

int debugcounter_258350 = 0;
//----- (00075200) --------------------------------------------------------
//long sub_75200_VGA_Blit640_index= 0;
int debugcounter_256200 = 0;
//long oldmillis = 0;
void sub_75200_VGA_Blit640(uint16_t height)//256200
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

#if _DEBUG
	VGA_CalculateAndPrintFPS(0, 0);
#endif
	VGA_Blit(pdwScreenBuffer_351628);

	//set speed
	long actmillis = mygetthousandths();
	long newdelay = speedGame - (actmillis - oldmillis);//max millis is 20 millis
	if (newdelay < 0)newdelay = 0;
	if (newdelay > speedGame)newdelay = speedGame;
	mydelay(newdelay);//set speed
	oldmillis = actmillis;
	//set speed
}

void VGA_BlitAny()//256200
{
	if (!x_BYTE_E3766)
		sub_8CACD_draw_cursor2();

#if _DEBUG
	VGA_CalculateAndPrintFPS(0, 0);
#endif
	VGA_Blit(pdwScreenBuffer_351628);

	//set speed
	long actmillis = mygetthousandths();
	long newdelay = speedGame - (actmillis - oldmillis);//max millis is 20 millis
	if (newdelay < 0)newdelay = 0;
	if (newdelay > speedGame)newdelay = speedGame;
	mydelay(newdelay);//set speed
	oldmillis = actmillis;
	//set speed
}

//----- (0006FC30) --------------------------------------------------------
uint8_t sub_6FC30_get34_height()//250c30
{
	if (help_VGA_type_resolution != 8)//fixed
		return x_DWORD_EA3D4[34].height_5;
	else
		return posistruct7[34].height_5;
}
// EA3D4: using guessed type int x_DWORD_EA3D4;

//----- (0002BB40) --------------------------------------------------------
void sub_2BB40_draw_bitmap(int16_t posx, int16_t posy, posistruct_t tempposstr)//20cb40
{
	//a3=48c80f-> 48ae47002633

  //void (*v3)(__int16, __int16, Pathstruct); // eax
  //int result; // eax
	uint8_t* temp_screen_buffer; // edi
  //int (*v6)(__int16, __int16, Pathstruct); // [esp+0h] [ebp-4h]
//123 cd 48c80f

  /*Pathstruct tempstruct;
  uint8_t* tempcharstar;
  tempstruct.colorPalette_var28 = &tempcharstar;//this fix
  *tempstruct.colorPalette_var28 = bitmap;//this fix */ //fix this

  //push [ebp+18] -cd- pote ecx
  //push [ebp+14] -123- pote ebx

	if (x_WORD_180660_VGA_type_resolution & 1)
		sub_8F8B0_draw_bitmap320(posx, posy, tempposstr);
	else
		sub_8F8E8_draw_bitmap640(posx, posy, tempposstr);
	//v6 = v3;
	//v3(a1, a2, a3);
	//result = (int)x_D41A0_BYTEARRAY_0;
	if (D41A0_0.m_GameSettings.m_Display.m_uiScreenSize == 1)
	{
		temp_screen_buffer = pdwScreenBuffer_351628;
		pdwScreenBuffer_351628 = x_DWORD_E9C3C;
		if (x_WORD_180660_VGA_type_resolution & 1)
			sub_8F8B0_draw_bitmap320(posx, posy, tempposstr);
		else
			sub_8F8E8_draw_bitmap640(posx, posy, tempposstr);
		pdwScreenBuffer_351628 = temp_screen_buffer;
	}
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// E9C3C: using guessed type int x_DWORD_E9C3C;
// 180628: using guessed type int pdwScreenBuffer_351628;
// 180660: using guessed type __int16 x_WORD_180660_VGA_type_resolution;

//----- (00075D70) --------------------------------------------------------
void sub_75D70(uint8_t* a1y, uint32_t a2)//256d70
{
	//unsigned int result; // eax

	if (a1y)
	{
		//result = a2;
		qmemcpy(a1y, (void*)x_DWORD_17DB50, a2);
		//qmemcpy(a1+a2, (void *)(x_DWORD_17DB50+a2), a2&3);
	}
	x_DWORD_17DB50 += a2;
	//return result;
}
// 17DB50: using guessed type int x_DWORD_17DB50;

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

		pdwScreenBuffer_351628 = (uint8_t*)temp_screen_buffer;
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// E9C3C: using guessed type int x_DWORD_E9C3C;
// 180628: using guessed type int pdwScreenBuffer_351628;
// 180660: using guessed type __int16 x_WORD_180660_VGA_type_resolution;

//----- (0002BC10) --------------------------------------------------------
void DrawText_2BC10(const char* textbuffer, int16_t posx, int16_t posy, uint8_t color)//20cc10
{
	sub_6F940_sub_draw_text(textbuffer, posx, posy, color);
	if (D41A0_0.m_GameSettings.m_Display.m_uiScreenSize == 1)//shifted graphics
	{
		uint8_t* temp_screen_buffer = pdwScreenBuffer_351628;
		pdwScreenBuffer_351628 = x_DWORD_E9C3C;
		sub_6F940_sub_draw_text(textbuffer, posx, posy, color);
		pdwScreenBuffer_351628 = temp_screen_buffer;
	}	
}

void VGA_CalculateAndPrintFPS(int x, int y)
{
	timeDelta += std::chrono::steady_clock::now() - frameStart;
	frameCount++;

	if (timeDelta > std::chrono::duration<double>(1.0))
	{
		fps = frameCount;
		frameCount = 0;
		timeDelta = std::chrono::duration<double>(0);
	}

	VGA_GotoXY(x, y);
	std::string fpsStr = "FPS: ";
	fpsStr.append(std::to_string(fps));

	VGA_Draw_stringXYtoBuffer((char*)fpsStr.c_str(), 0, 0, pdwScreenBuffer_351628);
	//VGA_Draw_string((char*)fpsStr.c_str());
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
void sub_7FB90_draw_text(char* textbuffer, int16_t posx, int16_t posy, uint8_t color)//260b90
{
	uint8_t* temptextbuffer; // esi
	int16_t tempposx; // ebx
	int16_t tempposy; // di
	char v7; // ax
	int v8; // eax
	//int result; // eax
	int v10; // [esp+0h] [ebp-4h]

	temptextbuffer = (uint8_t*)textbuffer;
	tempposx = posx;
	tempposy = posy;
	if (color)
		x_WORD_E36D4 = 64;
	/*if ((posx & 0x8000u) != 0)
		tempposx = 0;*///fix
	v10 = tempposx;
	while (temptextbuffer[0] && tempposx < 640)
	{
		v7 = temptextbuffer[0];
		if (v7 < 0xAu)//formating char
		{
			if (!temptextbuffer[0])
			{
				temptextbuffer++;
				continue;
			}//goto LABEL_21;
			if (v7 == 0x9u)//tab char
			{
				v8 = xy_DWORD_17DEC0_spritestr[65].width_4;
				tempposx += v8;
				temptextbuffer++;
				continue;
			}
			//goto LABEL_20;
	  //LABEL_16:
			if (temptextbuffer[0])//not end char
			{
				if (color)
					sub_72C40_draw_bitmap_640_setcolor(tempposx, tempposy, xy_DWORD_17DEC0_spritestr[temptextbuffer[0]], color);
				else
					sub_7C120_draw_bitmap_640(tempposx, tempposy, xy_DWORD_17DEC0_spritestr[temptextbuffer[0]]);
			}
			v8 = xy_DWORD_17DEC0_spritestr[65].width_4;
			tempposx += v8;
			temptextbuffer++;
			continue;
			//goto LABEL_20;
		}
		if (v7 <= 0xAu) // formating char
		{
			tempposx = v10;
			tempposy += xy_DWORD_17DEC0_spritestr[65].height_5;
			{
				temptextbuffer++;
				continue;
			}//goto LABEL_21;
		}
		if (v7 == 0xDu) // formating char cariage return
		{
			temptextbuffer++;
			continue;
		}//goto LABEL_21;
		if ((v7 < 0xDu) || (v7 != 32)) // formating char
		{
			if (temptextbuffer[0])//not end char
			{
				if (color)
					sub_72C40_draw_bitmap_640_setcolor(tempposx, tempposy, xy_DWORD_17DEC0_spritestr[temptextbuffer[0]], color);
				else
					sub_7C120_draw_bitmap_640(tempposx, tempposy, xy_DWORD_17DEC0_spritestr[temptextbuffer[0]]);
			}
			v8 = xy_DWORD_17DEC0_spritestr[65].width_4;
			tempposx += v8;
			temptextbuffer++;
			continue;
		}//goto LABEL_16;
		/*
		if ( v7 != 32 )//not space
		{
			if (temptextbuffer[0])//not end char
			{
				if (a4)
					sub_72C40_draw_bitmap_640_setcolor(tempposx, tempposy, xy_DWORD_17DEC0_spritestr[temptextbuffer[0]], a4);
				else
					sub_7C120_draw_bitmap_640(tempposx, tempposy, xy_DWORD_17DEC0_spritestr[temptextbuffer[0]]);
			}
			v8 = xy_DWORD_17DEC0_spritestr[65].sizex;
			tempposx += v8;
			temptextbuffer++;
			continue;
		}//goto LABEL_16;*/
		//LABEL_20:
			//HIWORD(v8) = HIWORD(x_DWORD_17DEC0);
		v8 = xy_DWORD_17DEC0_spritestr[65].width_4;
		tempposx += v8;
		//LABEL_21:
		temptextbuffer++;
	}
	//result = v5;
	x_WORD_E36D4 = 0;
	//return result;
}
// E36D4: using guessed type __int16 x_WORD_E36D4;
// 17DEC0: using guessed type int (int)x_DWORD_17DEC0;

//----- (0008CACD) --------------------------------------------------------
void sub_8CACD_draw_cursor2()//26dacd
{
	if (x_DWORD_E3768)
	{
		x_DWORD_E3758 = 1;
		sub_8C635_draw_cursor();
	}
}
// E3758: using guessed type int x_DWORD_E3758;
// E3768: using guessed type int x_DWORD_E3768;

//----- (0008F8B0) --------------------------------------------------------
//void sub_8F8B0_draw_bitmap320(__int16 a1, __int16 a2, Pathstruct a3)//2708B0
void sub_8F8B0_draw_bitmap320(int16_t posx, int16_t posy, posistruct_t temppstr)//2708B0
{
	//int v4; // [esp+4h] [ebp-4h]

	//fix it
	//v4 = 0;
	//fix it

	sub_8F935_bitmap_draw_final(temppstr.width_4, temppstr.height_5, posy, posx, temppstr.data, 0, 0);//270935 // a2 je 86
	//return v4;
}
// 180628: using guessed type int pdwScreenBuffer_351628;

//----- (0008F8E8) --------------------------------------------------------
//void sub_8F8E8_draw_bitmap640(__int16 a1, __int16 a2, uint8_t* a3)//2708e8
void sub_8F8E8_draw_bitmap640(int16_t posx, int16_t posy, posistruct_t temppstr)//2708e8
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
	sub_8F935_bitmap_draw_final(temppstr.width_4, temppstr.height_5, posy, posx, temppstr.data, 0, 0);//270935
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
void sub_6F940_sub_draw_text(const char* textbuffer, int posx, int posy, uint8_t color)//250940
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
				LOWORD(v7) = sub_6FC30_get34_height();
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
					LOWORD(v9) = sub_6FC10_letter_width();
					goto LABEL_15;
				}
			}
		}
		if (!v4[0])
			goto LABEL_14;
		//v8 = 6 * (v4[0] + 1);
		//similar as:sub_7C120_draw_bitmap_640(tempposx, tempposy, xy_DWORD_17DEC0_spritestr[temptextbuffer[0]]);
		//posistruct_t tempposistruct;//fixed
		//tempposistruct.pointer = new uint8_t[1];//fixed
		/*
		19f990->59361a00 0407 70361a00 04 07 85361a00
		*/
		/*tempposistruct.pointer= &x_DWORD_EA3D4[v8];//fixed
		tempposistruct.sizex = 0;
		tempposistruct.sizey = 0;*/
		if (help_VGA_type_resolution != 8)//fixed
		{
			sub_72C40_draw_bitmap_640_setcolor(v5, posy, x_DWORD_EA3D4[v4[0] + 1], color);//fixed 2509d1
			v9 = x_DWORD_EA3D4[v4[0] + 1].width_4;
		}
		else
		{
			sub_72C40_draw_bitmap_640_setcolor(v5, posy, posistruct7[v4[0] + 1], color);//fixed 2509d1
			v9 = posistruct7[v4[0] + 1].width_4;
		}
		//sub_72C40_draw_bitmap_640_setcolor(v5, a3, *(new posistruct_t)/*(uint8_t*)(v8 + x_DWORD_EA3D4)*/, a4);

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
void sub_72C40_draw_bitmap_640_setcolor(int16_t posx, int16_t posy, posistruct_t a3, uint8_t color)//253c40
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
			sub_8F935_bitmap_draw_final(a3.width_4, a3.height_5, posy, posx, a3.data, color, 0);
		}
		//result = v7;
	}
	//return result;
}
// 180628: using guessed type int pdwScreenBuffer_351628;
// 180660: using guessed type __int16 x_WORD_180660_VGA_type_resolution;

//----- (0008C635) --------------------------------------------------------
void sub_8C635_draw_cursor()//26d635
{
	//int result; // eax
	uint8_t* v1; // eax
	uint8_t* v2; // edx

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
	x_DWORD_180708 = x_DWORD_180704_mouse_byte_index1 + pdwScreenBuffer_351628;
	x_DWORD_18070C = x_DWORD_180730_cursor_data;
	for (x_WORD_180738 = 0; x_WORD_180738 < x_DWORD_1806FC_mouse_invy; x_WORD_180738++)
	{
		for (x_DWORD_18073A = 0; x_DWORD_18073A < x_DWORD_1806FC_mouse_invx; x_DWORD_18073A++)
		{
			v1 = x_DWORD_180714++;//adresa1
			v1[0] = x_DWORD_180708[0];//uloz na adresu
			v2 = x_DWORD_180718++;//adresa2
			v2[0] = x_DWORD_180708[0];//uloz na adresu
			if (x_DWORD_18070C[0] != 0xfe)
				x_DWORD_180708[0] = x_DWORD_18070C[0];
			x_DWORD_180708++;//351708 - finalni obraz
			x_DWORD_18070C++;//35170c - kurzor
		}
		x_DWORD_180708 += screenWidth_18062C - x_DWORD_1806FC_mouse_invx;
		x_DWORD_18070C += x_DWORD_180734 - x_DWORD_1806FC_mouse_invx;
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
// 18070C: using guessed type int x_DWORD_18070C;
// 180714: using guessed type int x_DWORD_180714;
// 180718: using guessed type int x_DWORD_180718;
// 18072C: using guessed type __int16 x_WORD_18072C_cursor_sizex;
// 18072E: using guessed type __int16 x_WORD_18072E_cursor_sizey;
// 180730: using guessed type int x_DWORD_180730_cursor_data;
// 180738: using guessed type __int16 x_WORD_180738;
// 18073A: using guessed type int x_DWORD_18073A;

//----- (0008F935) --------------------------------------------------------
void sub_8F935_bitmap_draw_final(uint8_t a1byte1, uint8_t a1byte2, uint16_t tiley, int tilex, uint8_t* texture, uint8_t setbyte, char a6)//270935
{
	uint8_t* pixel_buffer_index; // edi
	x_BYTE* v7; // edi
	int v8; // ecx
	signed int v9; // eax
	x_BYTE* v10; // ebx
	char v11; // al
	char v12; // al
	char v13; // al
	x_BYTE* v14; // edi
	int v15; // ecx
	signed int v16; // eax
	x_BYTE* v17; // ebx
	char v18; // al
	char v19; // al
	char v20; // al
	uint8_t* v21_buffer_temp_index1; // edi
	//unsigned int v22; // ecx
	//signed int v23; // eax
	//uint8_t* v24_buffer_temp_index2; // ebx
	//uint8_t* v25_buffer_temp_index3; // edi
	//char v26; // al
	//uint8_t* v27; // esi
	char* v28; // edi
	unsigned int v29; // ecx
	signed int v30; // eax
	char* v31; // ebx
	char* v32; // edi
	char v33; // al
	char* v34; // esi
	int v35; // ebp
	x_BYTE* v36; // edi
	int v37; // ecx
	x_BYTE* v38; // ebx
	unsigned __int8 v39; // al
	int v40; // eax
	x_BYTE* v41; // edi
	int v42; // ecx
	unsigned __int8 i; // dl
	char v44; // al
	char v45; // al
	unsigned __int8 v46; // of
	char v47; // dl
	unsigned __int8 v48; // al
	char* v49; // edi
	char v50; // al
	char v51; // dl
	char v52; // al
	char v53; // dl
	unsigned int v54; // ebx
	const void* v55; // esi
	//char *v56; // edi
	//unsigned int v57; // ecx
	//signed int v58; // eax
	//char *v59; // ebx
	//char *v60; // edi
	char* v61; // edi
	unsigned int v62; // ecx
	signed int v63; // eax
	char* v64; // ebx
	char* v65; // edi
	char v66; // al
	char* v67; // esi
	//uint8_t* v68; // edi
	//int v69; // ebx
	uint8_t v69l;
	uint8_t v69h;
	int8_t* v70; // edi
	int8_t* v71; // edx
	int8_t v72l; // ecx
	char v73; // al
	char v74; // al
	bool v75; // zf
	bool v76; // sf
	char v77; // al
	char v78; // al
	int v79; // ebx
	x_BYTE* v80; // edi
	int v81; // ecx
	signed int v82; // eax
	x_BYTE* v83; // ebx
	char v84; // al
	int v85; // ebx
	x_BYTE* v86; // edi
	int v87; // ecx
	signed int v88; // eax
	x_BYTE* v89; // ebx
	char v90; // al
	x_BYTE* v91; // edi
	int v92; // ecx
	int v93; // ebx
	signed int v94; // eax
	x_BYTE* v95; // ebx
	char v96; // al
	//unsigned int v97; // ebx
	//x_BYTE *v98; // edi
	//x_BYTE *v99; // edx
	/*int v100; // ecx
	char v101; // al
	char v102; // al
	char v103; // al
	char v104; // al*/
	x_BYTE* v105; // edi
	int v106; // ecx
	int v107; // ebx
	x_BYTE* v108; // ebx
	signed int v109; // eax
	char v110; // al
	x_BYTE* v111; // edi
	int v112; // ecx
	int v113; // ebx
	x_BYTE* v114; // ebx
	signed int v115; // eax
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
	int v127; // eax
	int v128; // eax
	int v129; // eax
	int v130; // eax
	char* v131; // [esp-4h] [ebp-Ch]
	uint8_t v132; // [esp+2h] [ebp-6h]
	unsigned __int8 v133; // [esp+2h] [ebp-6h]
	unsigned __int8 v134; // [esp+2h] [ebp-6h]
	unsigned __int8 v135; // [esp+2h] [ebp-6h]
	//unsigned __int8 v136; // [esp+2h] [ebp-6h]
	unsigned __int8 v137; // [esp+2h] [ebp-6h]
	unsigned __int8 v138; // [esp+2h] [ebp-6h]
	unsigned __int8 v139; // [esp+2h] [ebp-6h]
	char v140; // [esp+3h] [ebp-5h]
	char v141; // [esp+3h] [ebp-5h]
	x_BYTE* v142; // [esp+4h] [ebp-4h]
	x_BYTE* v143; // [esp+4h] [ebp-4h]
	//debug
	/*loadfromsnapshot((char*)"0160-00270935-2",texture,0x47be3a,0x400);

	//a1 = 0x1513;
	x_DWORD_180650_positiony = 0;//351650
	screenWidth_18062C = 0x40;//35162c
	x_DWORD_18063C_sprite_sizex = 0;//35163c
	//pdwScreenBuffer_351628=82c714
	x_WORD_180660_VGA_type_resolution = 0x30008;
	tiley = 0;
	a1.byte1 = 0x13;

	a1.byte2 = 0x15;
	//a1.byte2 = 0;

	tilex = 0;
	x_DWORD_180644_map_resolution2_y = 0x40;

	//a4 = 03;*/
	//debug

	//x_DWORD_180650_positiony - 0 35162c
	//screenWidth_18062C - 40 35162c
	//47ae48+1
	//0x47be3a - 03191919
	if (!(a1byte2))//453558
		return;
	pixel_buffer_index = pdwScreenBuffer_351628 + x_DWORD_18063C_sprite_sizex + screenWidth_18062C * x_DWORD_180650_positiony;
	if (x_WORD_180660_VGA_type_resolution & 1)//if 320x200 is resolved, the value is halved
	{
		a1byte1 /= 2;
		a1byte2 /= 2;
		tilex /= 2;
		tiley /= 2;
	}
	if (tiley < 0)
	{
		if (x_WORD_E36D4 & 2)
		{
			v128 = tiley + a1byte2;
			v46 = __OFADD__(1, v128);
			v129 = v128 + 1;
			if (((v129 < 0) ^ v46) | (v129 == 0))
				return;
			tiley = -1;
			a1byte2 = v129;
		}
		else
		{
			v130 = tiley + a1byte2;
			if (((tiley + a1byte2 < 0) ^ __OFADD__(tiley, a1byte2)) | (tiley + a1byte2 == 0))
				return;
			a1byte2 += tiley;
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
	else if (tiley + a1byte2 >= x_DWORD_180644_map_resolution2_y)
	{
		if (x_WORD_E36D4 & 2)
		{
			if (tiley + 1 >= x_DWORD_180644_map_resolution2_y)
				return;
			v125 = a1byte2 + tiley + 1 - x_DWORD_180644_map_resolution2_y;
			a1byte2 = x_DWORD_180644_map_resolution2_y - (tiley + 1);
			v126 = v125;
			v127 = 0;
			do
			{
				while (1)
				{
					v127 = *texture++;
					if (!v127)
						break;
					if ((v127 & 0x80u) == 0)
						texture += v127;
				}
				--v126;
			} while (v126);
			pixel_buffer_index = pdwScreenBuffer_351628;
		}
		else
		{
			if (x_DWORD_180644_map_resolution2_y <= tiley)
				return;
			a1byte2 = x_DWORD_180644_map_resolution2_y - tiley;
		}
	}
	if (tilex >= 0)
	{
		if (tilex + a1byte1 >= x_DWORD_180648_map_resolution2_x)
		{
			if (x_WORD_E36D4)
			{
				if (x_WORD_E36D4 & 1)
				{
					if (x_WORD_E36D4 & 2)
					{
						if (x_DWORD_180634_screen_width - tilex >= 0)
						{
							v79 = a1byte1 + tilex;
							v80 = (x_BYTE*)(screenWidth_18062C * (a1byte2 + tiley) + v79 + pixel_buffer_index);
							v81 = 0;
							v79 = x_DWORD_180634_screen_width - v79 - 2;
							a1byte1 = v79;
							v133 = v79;
							v82 = -1;
							v83 = v80;
							do
							{
								while (1)
								{
									while (1)
									{
										v82 = *texture++;
										if ((v82 & 0x80u) == 0)
											break;
										v80 += v82;
										a1byte1 = a1byte1 - v82;
									}
									if (!v82)
										break;
									v81 = v82;
									do
									{
										v84 = *texture++;
										a1byte1 = a1byte1 + 1;
										if ((a1byte1 & 0x80u) == 0)
											*v80 = v84;
										--v80;
										--v81;
									} while (v81);
								}
								v83 -= screenWidth_18062C;
								v80 = v83;
								a1byte1 = __PAIR__(a1byte2, v133) - 256;
							} while (a1byte2);
						}
					}
					else if (x_DWORD_180634_screen_width - tilex >= 0)
					{
						v85 = a1byte1 + tilex;
						v86 = (x_BYTE*)(screenWidth_18062C * tiley + v85 + pixel_buffer_index);
						v87 = 0;
						v85 = x_DWORD_180634_screen_width - v85 - 2;
						a1byte1 = v85;
						v134 = v85;
						v88 = -1;
						v89 = v86;
						do
						{
							while (1)
							{
								while (1)
								{
									v88 = *texture++;
									if ((v88 & 0x80u) == 0)
										break;
									v86 += v88;
									a1byte1 = a1byte1 - v88;
								}
								if (!v88)
									break;
								v87 = v88;
								do
								{
									v90 = *texture++;
									a1byte1 = a1byte1 + 1;
									if ((a1byte1 & 0x80u) == 0)
										*v86 = v90;
									--v86;
									--v87;
								} while (v87);
							}
							v89 += screenWidth_18062C;
							v86 = v89;
							a1byte1 = __PAIR__(a1byte2, v134) - 256;
						} while (a1byte2);
					}
				}
				else if (x_WORD_E36D4 & 2)
				{
					v91 = (x_BYTE*)(screenWidth_18062C * (a1byte2 + tiley) + tilex + pixel_buffer_index);
					v92 = 0;
					v93 = x_DWORD_180634_screen_width - tilex;
					if (v93 >= 0)
					{
						a1byte1 = v93;
						v135 = v93;
						v94 = -1;
						v95 = v91;
						do
						{
							while (1)
							{
								while (1)
								{
									v94 = *texture++;
									if ((v94 & 0x80u) == 0)
										break;
									v91 -= v94;
									a1byte1 = v94 + a1byte1;
								}
								if (!v94)
									break;
								v92 = v94;
								do
								{
									v96 = *texture++;
									a1byte1 = a1byte1 - 1;
									if ((a1byte1 & 0x80u) == 0)
										*v91++ = v96;
									--v92;
								} while (v92);
							}
							v95 -= screenWidth_18062C;
							v91 = v95;
							a1byte1 = __PAIR__(a1byte2, v135) - 256;
						} while (a1byte1);
					}
				}
			}
			else
			{
				/*for (int ii = 0;ii < a1byte1;ii++)//debug
					for (int jj = 0;jj < a1byte2;jj++)
						pixel_buffer_index[screenWidth_18062C * (tiley + ii) + (tilex + jj)] = 128;
				*/
				//edi - 3c28b7

				  //v68 = tilex + pixel_buffer_index;
				  //v69 = x_DWORD_180634_screen_width - tilex;
				if (x_DWORD_180634_screen_width - tilex >= 0)
				{
					v70 = (int8_t*)(screenWidth_18062C * tiley + tilex + pixel_buffer_index);
					v69l = x_DWORD_180634_screen_width - tilex;
					v69h = a1byte2;//ebx
					v132 = v69l;//ebp-6
					v71 = v70;//edx edi
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
									v70 -= v73;
									v77 = *texture++;
									v72l = v77;
									do
									{
										v78 = *texture++;
										v69l--;
										if ((v69l & 0x80u) == 0)//fix
											*v70++ = v78;
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
									*v70++ = v74;
								v72l--;
							} while (v72l);
						}
						v71 += screenWidth_18062C;
						v70 = v71;

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
				v7 = (x_BYTE*)(screenWidth_18062C * (a1byte2 + tiley) + a1byte1 + tilex + pixel_buffer_index);
				v8 = 0;
				v9 = -1;
				v10 = v7;
				do
				{
					while (1)
					{
						while (1)
						{
							v9 = *texture++;
							if ((v9 & 0x80u) == 0)
								break;
							v7 += v9;
							v12 = *texture++;
							v8 = v12;
							do
							{
								v13 = *texture++;
								*v7-- = v13;
								--v8;
							} while (v8);
						}
						if (!v9)
							break;
						v8 = v9;
						do
						{
							v11 = *texture++;
							*v7-- = v11;
							--v8;
						} while (v8);
					}
					v10 -= screenWidth_18062C;
					v7 = v10;
					--a1byte2;
				} while (a1byte2);
			}
			else
			{
				v14 = (x_BYTE*)(screenWidth_18062C * tiley + a1byte1 + tilex + pixel_buffer_index);
				v15 = 0;
				v16 = -1;
				v17 = v14;
				do
				{
					while (1)
					{
						while (1)
						{
							v16 = *texture++;
							if ((v16 & 0x80u) == 0)
								break;
							v14 += v16;
							v19 = *texture++;
							v15 = v19;
							do
							{
								v20 = *texture++;
								*v14-- = v20;
								--v15;
							} while (v15);
						}
						if (!v16)
							break;
						v15 = v16;
						do
						{
							v18 = *texture++;
							*v14-- = v18;
							--v15;
						} while (v15);
					}
					v17 += screenWidth_18062C;
					v14 = v17;
					--a1byte2;
				} while (a1byte2);
			}
		}
		else if (x_WORD_E36D4)
		{
			if (x_WORD_E36D4 & 2)
			{
				v28 = (char*)(screenWidth_18062C * (a1byte2 + tiley) + tilex + pixel_buffer_index);
				v29 = 0;
				v30 = -1;
				v31 = v28;
				do
				{
					while (1)
					{
						while (1)
						{
							v30 = *texture++;
							if ((v30 & 0x80u) == 0)
								break;
							v32 = &v28[-v30];
							v33 = *texture;
							v34 = (char*)(texture + 1);
							v29 = v33;
							qmemcpy(v32, v34, v29);
							texture = (uint8_t*)&v34[v29];
							v28 = &v32[v29];
							v29 = 0;
						}
						if (!v30)
							break;
						v29 = v30;
						qmemcpy(v28, texture, v29);
						texture += v29;
						v28 += v29;
						v29 = 0;
					}
					v31 -= screenWidth_18062C;
					v28 = v31;
					--a1byte2;
				} while (a1byte2);
			}
			else if (x_WORD_E36D4 & 4)
			{
				v35 = x_DWORD_E3890;
				v36 = (x_BYTE*)(screenWidth_18062C * tiley + tilex + pixel_buffer_index);
				v37 = 0;
				v38 = v36;
				do
				{
					while (1)
					{
						while (1)
						{
							v39 = *texture++;
							if ((v39 & 0x80u) == 0)
								break;
							v36 -= (char)v39;
						}
						if (!v39)
							break;
						v37 = v39;
						v40 = v39;
						do
						{
							v40 = *texture++;
							v40 = *v36;
							*v36++ = *(x_BYTE*)(v40 + v35);
							--v37;
						} while (v37);
					}
					v38 += screenWidth_18062C;
					v36 = v38;
					--a1byte2;
				} while (a1byte2);
			}
			else if (x_WORD_E36D4 & 8)
			{
				v41 = (x_BYTE*)(screenWidth_18062C * tiley + tilex + pixel_buffer_index);
				v42 = 0;
				v142 = v41;
				v140 = a1byte2;
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
									++v41;
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
								*v41++ = v45;
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
					v142 += screenWidth_18062C;
					v41 = v142;
				}
			}
			else if (x_WORD_E36D4 & 0x20)
			{
				v49 = (char*)(screenWidth_18062C * tiley + tilex + pixel_buffer_index);
				v143 = v49;
				v141 = a1byte2;
				do
				{
					while (1)
					{
						while (1)
						{
							v50 = *texture++;
							if (v50 >= 0)
								break;
							v49 += (setbyte * -v50);
						}
						if (!v50)
							break;
						v51 = v50;
						do
						{
							v52 = *texture++;
							memset(v49, v52, setbyte);
							v49 += setbyte;
							v46 = __OFSUB__(v51--, 1);
						} while (!(((v51 < 0) ^ v46) | (v51 == 0)));
					}
					v53 = a6 - 1;
					if (a6 > 1)
					{
						v131 = (char*)texture;
						v54 = v49 - v143;
						do
						{
							v55 = v143;
							v143 += screenWidth_18062C;
							qmemcpy(v143, (void*)v55, v54);
							v46 = __OFSUB__(v53--, 1);
						} while (!(((v53 < 0) ^ v46) | (v53 == 0)));
						texture = (uint8_t*)v131;
					}
					v143 += screenWidth_18062C;
					v49 = v143;
					--v141;
				} while (v141);
			}
			else if (x_WORD_E36D4 & 0x40)
			{
				//v56 = (char *)(dword_18062C * a2 + a3 + v6);
				v21_buffer_temp_index1 = (screenWidth_18062C * tiley + tilex + pixel_buffer_index);
				int8_t v57_loc = 0;
				int8_t v58_loc = -1;
				int8_t* v59_loc = (int8_t*)v21_buffer_temp_index1;
				int8_t* v60loc = 0;
				do
				{
					while (1)
					{
						while (1)
						{
							v58_loc = texture[0];
							texture++;
							if ((v58_loc & 0x80u) == 0)
								break;
							v60loc = (int8_t*)&v21_buffer_temp_index1[-v58_loc];
							v57_loc = texture[0];
							texture += v57_loc + 1;
							memset(v60loc, setbyte, v57_loc);
							v21_buffer_temp_index1 = (uint8_t*)&v60loc[v57_loc];
							v57_loc = 0;
						}
						if (!v58_loc)
							break;
						v57_loc = v58_loc;
						texture += v57_loc;
						memset(v21_buffer_temp_index1, setbyte, v57_loc);
						v21_buffer_temp_index1 += v57_loc;
						v57_loc = 0;
					}
					v59_loc += screenWidth_18062C;
					v21_buffer_temp_index1 = (uint8_t*)v59_loc;
					--a1byte2;
				} while (a1byte2);
				/*
					v21_buffer_temp_index1 = (screenWidth_18062C * tiley + tilex + pixel_buffer_index);
					uint32_t inindex = 0;
					uint32_t outindex = 0;
					int8_t shift = 0;
					int8_t end = 0;
					int8_t count = texture[inindex++];
					memset(&v21_buffer_temp_index1[outindex], setbyte, count);
					//qmemcpy(&v21_buffer_temp_index1[outindex], &texture[inindex], count);
					for (uint32_t y = 1;count != 0x7f;y++)
					{
						memset(&v21_buffer_temp_index1[outindex + shift], setbyte, count);
						//qmemcpy(&v21_buffer_temp_index1[outindex + shift], &texture[inindex], count);
						inindex += count;
						end = texture[inindex++];
						if (end == 0)
						{
							count = texture[inindex++];
							if (count < 0) {
								shift = -count;
								count = texture[inindex++];
							}
							outindex += screenWidth_18062C;
						}
						else {
							shift += count;
							count = end;
							if (count < 0) {
								shift -= count;
								count = texture[inindex++];
							}
						}
					}
					*/
					/*v56 = (char *)(screenWidth_18062C * tiley + tilex + pixel_buffer_index);
					v57 = 0;
					v58 = -1;
					v59 = v56;
					do
					{
					  while ( 1 )
					  {
						while ( 1 )
						{
						  v58 = *texture++;
						  if ( (v58 & 0x80u) == 0 )
							break;
						  v60 = &v56[-v58];
						  v57 = *texture;
						  texture += v57 + 1;
						  memset(v60, a5, v57);
						  v56 = &v60[v57];
						  v57 = 0;
						}
						if ( !v58 )
						  break;
						v57 = v58;
						texture += v57;
						memset(v56, a5, v57);
						v56 += v57;
						v57 = 0;
					  }
					  v59 += screenWidth_18062C;
					  v56 = v59;
					  --a1.byte2;
					}
					while ( a1.byte2 );*/
			}
			else
			{
				v61 = (char*)(screenWidth_18062C * tiley + tilex + pixel_buffer_index);
				v62 = 0;
				v63 = -1;
				v64 = v61;
				do
				{
					while (1)
					{
						while (1)
						{
							v63 = *texture++;
							if ((v63 & 0x80u) == 0)
								break;
							v65 = &v61[-v63];
							v66 = *texture;
							v67 = (char*)(texture + 1);
							v62 = v66;
							qmemcpy(v65, v67, v62);
							texture = (uint8_t*)&v67[v62];
							v61 = &v65[v62];
							v62 = 0;
						}
						if (!v63)
							break;
						v62 = v63;
						qmemcpy(v61, texture, v62);
						texture += v62;
						v61 += v62;
						v62 = 0;
					}
					v64 += screenWidth_18062C;
					v61 = v64;
					--a1byte2;
				} while (a1byte2);
			}
		}
		else
		{
			v21_buffer_temp_index1 = (screenWidth_18062C * tiley + tilex + pixel_buffer_index);

			//v21 = (char *)(dword_18062C * a2 + a3 + v6);
			int8_t v22_loc = 0;
			int8_t v23_loc = -1;
			int8_t* v25_loc = 0;
			int8_t v26_loc = 0;
			int8_t* v27_loc = 0;
			int8_t* v24_loc = (int8_t*)v21_buffer_temp_index1;
			do
			{
				while (1)
				{
					while (1)
					{
						v23_loc = texture[0];
						texture++;
						if ((v23_loc & 0x80u) == 0)
							break;
						v25_loc = (int8_t*)&v21_buffer_temp_index1[-v23_loc];
						v26_loc = texture[0];
						v27_loc = (int8_t*)(texture + 1);
						v22_loc = v26_loc;
						qmemcpy(v25_loc, v27_loc, v22_loc);
						texture = (uint8_t*)&v27_loc[v22_loc];
						v21_buffer_temp_index1 = (uint8_t*)&v25_loc[v22_loc];
						v22_loc = 0;
					}
					if (!v23_loc)
						break;
					v22_loc = v23_loc;
					qmemcpy(v21_buffer_temp_index1, texture, v22_loc);
					texture += v22_loc;
					v21_buffer_temp_index1 += v22_loc;
					v22_loc = 0;
				}
				v24_loc += screenWidth_18062C;
				v21_buffer_temp_index1 = (uint8_t*)v24_loc;
				a1byte2--;
			} while (a1byte2);
			/*

			  v21_buffer_temp_index1 = (screenWidth_18062C * tiley + tilex + pixel_buffer_index);

			  uint32_t inindex = 0;
			  uint32_t outindex = 0;
			  int8_t shift = 0;
			  int8_t end = 0;
			  int8_t count = 0;
			  end = texture[inindex++];
			  if (end == 0)
			  {
				  count = texture[inindex++];
				  outindex ++;
				  while (count == 0) {
					  outindex += screenWidth_18062C+1;
					  count = texture[inindex++];
				  }
				  if (count < 0) {
					  shift = -count;
					  count = texture[inindex++];
				  }
				  outindex += screenWidth_18062C;
			  }
			  else {
				  shift += count;
				  count = end;
				  if (count < 0) {
					  shift -= count;
					  count = texture[inindex++];
				  }
			  }
			  qmemcpy(&v21_buffer_temp_index1[outindex], &texture[inindex], count);
			  for(uint32_t y=1;count!=0x7f;y++)
				  {
					qmemcpy(&v21_buffer_temp_index1[outindex + shift], &texture[inindex], count);
					inindex += count;
					end = texture[inindex++];
					if (end == 0)
					{
						count = texture[inindex++];
						while (count == 0) {
							outindex += screenWidth_18062C+1;
							count = texture[inindex++];
						}
						if (count < 0) {
							shift = -count;
							count = texture[inindex++];
						}
						outindex += screenWidth_18062C;
					}
					else {
						shift += count;
						count = end;
						if (count < 0) {
							shift -= count;
							count = texture[inindex++];
						}
					}
				  }
				  */
				  //v24_buffer_temp_index2 = v21_buffer_temp_index1;
				  /*
				  do
				  {
					while ( 1 )
					{
					  uint32_t index_tab=0;
					  uint32_t index_tab_v23 = 0;
					  uint32_t index_tab_v22 = 0;
					  //v23 = texture[index_tab++];
					  for (uint32_t index_tab = 0;texture[index_tab] != 0; index_tab++)
					  {
						  // 3 19 19
						  v25_buffer_temp_index3 = &v21_buffer_temp_index1[-v23];
						  //v27 = texture + 1;
						  texture[index_tab_v22] = texture[index_tab];
						  qmemcpy(v25_buffer_temp_index3, (void*)texture[index_tab+1], (int)texture[index_tab_v22]);
						  //texture = (uint8_t*)&(texture + 1 + 3);
						  v21_buffer_temp_index1 = &v25_buffer_temp_index3[texture[index_tab_v22]];
						  texture[index_tab_v22] = 0;
					  }
					  if (!v23)
						  break;
					  texture[index_tab_v22] = texture[index_tab_v23];
					  qmemcpy(v21_buffer_temp_index1, texture, texture[index_tab_v22]);
					  texture += texture[index_tab_v22];
					  v21_buffer_temp_index1 += texture[index_tab_v22];
					  texture[index_tab_v22] = 0;

					  v23 = *texture++;
					  for ( ;v23!=0;v23 = *texture++)
					  {
						// 3 19 19
						v25_buffer_temp_index3 = &v21_buffer_temp_index1[-v23];
						//v27 = texture + 1;
						v22 = *texture;
						qmemcpy(v25_buffer_temp_index3, texture + 1, v22);
						texture = (uint8_t*)&(*texture + 1+*v22);
						v21_buffer_temp_index1 = &v25_buffer_temp_index3[v22];
						v22 = 0;
					  }
					  if ( !v23 )
						break;
					  v22 = v23;
					  qmemcpy(v21_buffer_temp_index1, texture, v22);
					  texture += v22;
					  v21_buffer_temp_index1 += v22;
					  v22 = 0;
					}
					v24_buffer_temp_index2 += screenWidth_18062C;
					v21_buffer_temp_index1 = v24_buffer_temp_index2;
					--a1.byte2;
				  }
				  while ( a1.byte2 );*/
		}
		return;
	}
	/*if ( !x_WORD_E36D4 )
	{
	  v97 = -tilex;
	  if (a1byte1 <= v97 )
		return;
	  v98 = (x_BYTE *)(screenWidth_18062C * tiley + pixel_buffer_index);
	  v97 = a1byte2;
	  v99 = v98;
	  v136 = v97;
	  v100 = 0;
	  while ( 1 )
	  {
		while ( 1 )
		{
		  while ( 1 )
		  {
			v101 = *texture++;
			if ( v101 < 0 )
			  break;
			if ( v101 )
			{
			  v100 = v101;
			  do
			  {
				v102 = *texture++;
				v97 = v97 - 1;
				if ( (v97 & 0x80u) != 0 )
				  *v98++ = v102;
				--v100;
			  }
			  while ( v100 );
			}
			else
			{
			  v99 += screenWidth_18062C;
			  v98 = v99;
			  v97 = __PAIR__(v97, v136) - 256;
			  if ( !v97 )
				return;
			}
		  }
		  if ( (char)v97 > 0 )
			break;
  LABEL_179:
		  v98 -= v101;
		  v103 = *texture++;
		  v100 = v103;
		  do
		  {
			v104 = *texture++;
			v97 = v97 - 1;
			if ( (v97 & 0x80u) != 0 )
			  *v98++ = v104;
			--v100;
		  }
		  while ( v100 );
		}
		v97 = v101 + v97;
		if ( (v97 & 0x80u) != 0 )
		{
		  v101 = v97;
		  goto LABEL_179;
		}
	  }
	}*/
	if (!(x_WORD_E36D4 & 1))
	{
		if (!(x_WORD_E36D4 & 2))
			return;
		v117 = -tilex;
		if (a1byte1 <= v117)
			return;
		v118 = (x_BYTE*)(screenWidth_18062C * (a1byte2 + tiley) + pixel_buffer_index);
		v117 = a1byte2;
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
		v105 = (x_BYTE*)(screenWidth_18062C * (a1byte2 + tiley) + a1byte1 + tilex + pixel_buffer_index);
		v106 = 0;
		v107 = a1byte1 + tilex + 1;
		if (v107 >= 0)
		{
			a1byte1 = v107;
			v137 = v107;
			v108 = v105;
			v109 = -1;
			do
			{
				while (1)
				{
					while (1)
					{
						v109 = *texture++;
						if ((v109 & 0x80u) == 0)
							break;
						v105 += v109;
						a1byte1 = v109 + a1byte1;
					}
					if (!v109)
						break;
					v106 = v109;
					do
					{
						v110 = *texture++;
						a1byte1 = a1byte1 - 1;
						if ((a1byte1 & 0x80u) == 0)
							*v105 = v110;
						v105--;
						v106--;
					} while (v106);
				}
				v108 -= screenWidth_18062C;
				v105 = v108;
				a1byte1 = __PAIR__(a1byte2, v137) - 256;
			} while (a1byte2);
		}
	}
	else
	{
		v111 = (x_BYTE*)(screenWidth_18062C * tiley + a1byte1 + tilex + pixel_buffer_index);
		v112 = 0;
		v113 = a1byte1 + tilex + 1;
		if (v113 >= 0)
		{
			a1byte1 = v113;
			v138 = v113;
			v114 = v111;
			v115 = -1;
			do
			{
				while (1)
				{
					while (1)
					{
						v115 = *texture++;
						if ((v115 & 0x80u) == 0)
							break;
						v111 += v115;
						a1byte1 = v115 + a1byte1;
					}
					if (!v115)
						break;
					v112 = v115;
					do
					{
						v116 = *texture++;
						a1byte1 = a1byte1 - 1;
						if ((a1byte1 & 0x80u) == 0)
							*v111 = v116;
						v111--;
						v112--;
					} while (v112);
				}
				v114 += screenWidth_18062C;
				v111 = v114;
				a1byte1 = __PAIR__(a1byte1, v138) - 256;
			} while (a1byte2);
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
	sub_8F935_bitmap_draw_final(a1byte1, a1byte2, posx, posy, a4, a5, a6);//270935
}

//----- (00098709) --------------------------------------------------------
void sub_98709_create_index_dattab_power(posistruct2_t* tabbuffer, posistruct2_t* tabbufferend, uint8_t* datbuffer, posistruct_t* dattabindex)//279709
{
	//int length = (tabbufferend - ((uint8_t*)tabbuffer)) / 6;
	//int length = tabbufferend - tabbuffer;
	for (int i = 0; i < tabbufferend - tabbuffer; i++)
	{
		//int index = tabbuffer[i].data;
		dattabindex[i].data = (datbuffer + tabbuffer[i].data_0);
		dattabindex[i].width_4 = tabbuffer[i].width_4 * 2;
		dattabindex[i].height_5 = tabbuffer[i].height_5 * 2;
	}
}

void sub_98709_create_index_dattab_power_add(uint8_t* tabbuffer, uint8_t* tabbufferend, uint8_t* datbuffer, posistruct_t* dattabindex, int add)//279709
{
	for (uint32_t i = 0; i < (tabbufferend - (tabbuffer + add)) / 6; i++)
	{
#ifdef x32_BIT_ENVIRONMENT
		dattabindex[i].data = (uint8_t*)(*(uint32_t*)((tabbuffer + add) + 6 * i)) + reinterpret_cast<int32_t>(datbuffer);
#endif //x32_BIT_ENVIRONMENT
#ifdef x64_BIT_ENVIRONMENT
		dattabindex[i].data = (uint8_t*)(*(uint32_t*)((tabbuffer + add) + 6 * i)) + (int32_t)reinterpret_cast<int64_t>(datbuffer);
#endif //x64_BIT_ENVIRONMENT		
		dattabindex[i].width_4 = (tabbuffer + add)[6 * i + 4] * 2;
		dattabindex[i].height_5 = (tabbuffer + add)[6 * i + 5] * 2;
	}
}

//----- (0009874D) --------------------------------------------------------
void sub_9874D_create_index_dattab(posistruct2_t* tabbuffer, posistruct2_t* tabbufferend, uint8_t* datbuffer, posistruct_t* dattabindex)//27974d
{
	//uint32_t testadr = *(uint32_t*)tabbuffer;
	/*if (testadr == 0x9999)
	{
		for (uint32_t i = 0;i < (tabbufferend - tabbuffer) / 6;i++)
		{
			dattabindex[i].data += (int32_t)datbuffer;
		}
	}
	else*/
	{
		/*for (uint32_t i = 0; i < (tabbufferend - tabbuffer) / 6; i++)
		{
#ifdef TEST_x64
	allert_error();
#endif
#ifdef COMPILE_FOR_64BIT // FIXME: 64bit
			std::cout << "FIXME: 64bit @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
			dattabindex[i].data = (uint8_t*)(*(uint32_t*)(tabbuffer + 6 * i)) + (int32_t)datbuffer;
#endif
			dattabindex[i].width = tabbuffer[6 * i + 4];
			dattabindex[i].height = tabbuffer[6 * i + 5];
		}*/
		for (uint32_t i = 0; i < tabbufferend - tabbuffer; i++)
		{
			dattabindex[i].data = (datbuffer + tabbuffer[i].data_0);
			dattabindex[i].width_4 = tabbuffer[i].width_4;
			dattabindex[i].height_5 = tabbuffer[i].height_5;
		}

		//testadr = 0x9999;
		//memcpy(tabbuffer, &testadr, 4);
	}
}

void sub_9874D_create_index_dattab_add(uint8_t* tabbuffer, uint8_t* tabbufferend, uint8_t* datbuffer, posistruct_t* dattabindex, int add)//27974d
{
	for (uint32_t i = 0; i < (tabbufferend - (tabbuffer + add)) / 6; i++)
	{
#ifdef x32_BIT_ENVIRONMENT
		dattabindex[i].data = (uint8_t*)(*(uint32_t*)((tabbuffer + add) + 6 * i)) + reinterpret_cast<int32_t>(datbuffer);
#endif //x32_BIT_ENVIRONMENT
#ifdef x64_BIT_ENVIRONMENT
		dattabindex[i].data = (uint8_t*)(*(uint32_t*)((tabbuffer + add) + 6 * i)) + (int32_t)reinterpret_cast<int64_t>(datbuffer);
#endif //x64_BIT_ENVIRONMENT			
		dattabindex[i].width_4 = (tabbuffer + add)[6 * i + 4];
		dattabindex[i].height_5 = (tabbuffer + add)[6 * i + 5];
	}
}

//----- (00099A77) --------------------------------------------------------
void sub_99A77_create_index_dattab_div(uint8_t* tabbuffer, uint8_t* tabbufferend, uint8_t*  /*datbuffer*/, posistruct_t* dattabindex)//280a77
{
	//uint32_t testadr = *(uint32_t*)tabbuffer;
	/*if (testadr == 0x9999)
	{
		for (uint32_t i = 0;i < (tabbufferend - tabbuffer) / 6;i++)
		{
			dattabindex[i].data -= (int32_t)datbuffer;
			dattabindex[i].width /= 2;
			dattabindex[i].height /= 2;
		}
	}
	else*/
	{
		for (uint32_t i = 0; i < (tabbufferend - tabbuffer) / 6; i++)
		{
			dattabindex[i].data = (uint8_t*)(*(uint32_t*)(tabbuffer + 6 * i))/* + (int32_t)datbuffer*/;//fixed
			dattabindex[i].width_4 = tabbuffer[6 * i + 4] / 2;
			dattabindex[i].height_5 = tabbuffer[6 * i + 5] / 2;
		}
		//testadr = 0x9999;
		//memcpy(tabbuffer, &testadr, 4);
	}
}

//----- (00099AEB) --------------------------------------------------------
void sub_99AEB_create_index_dattab_minus(uint8_t* tabbuffer, uint8_t* tabbufferend, uint8_t*  /*datbuffer*/, posistruct_t* dattabindex)//280aeb
{
	//uint32_t testadr = *(uint32_t*)tabbuffer;
	/*if (testadr == 0x9999)
	{
		for (uint32_t i = 0;i < (tabbufferend - tabbuffer) / 6;i++)
		{
			dattabindex[i].data -= (int32_t)datbuffer;
		}
	}
	else*/
	{
		for (uint32_t i = 0; i < (tabbufferend - tabbuffer) / 6; i++)
		{
			dattabindex[i].data = (uint8_t*)(*(uint32_t*)(tabbuffer + 6 * i))/* + (int32_t)datbuffer*/;//fixed
			dattabindex[i].width_4 = tabbuffer[6 * i + 4];
			dattabindex[i].height_5 = tabbuffer[6 * i + 5];
		}
		//testadr = 0x9999;
		//memcpy(tabbuffer, &testadr, 4);
	}
}

signed int GetTrueWizardNumber_61790(signed int inputnumber)//242790
{
	signed int outputNumber = inputnumber;
	if (x_D41A0_BYTEARRAY_4_struct.setting_byte1_22 & 0x10)
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

void Convert_from_shadow_str_2FECE(type_shadow_str_2FECE* from, type_str_2FECE* to) {
	to->word_2FECE = from->word_2FECE;
	to->word_2FED0 = from->word_2FED0;
	to->byte_0x2FED2 = from->byte_0x2FED2;
	to->byte_0x2FED3 = from->byte_0x2FED3;
	to->MapType = (from->MapType == 2) ? MapType_t::Cave : (from->MapType == 1) ? MapType_t::Night : MapType_t::Day;
	to->word_0x2FED5 = from->word_0x2FED5;
	to->word_0x2FED7 = from->word_0x2FED7;
	for (int i = 0; i < 8; i++)to->player_0x2FED9[i] = from->array_0x2FED9[i];
	for (int i = 0; i < 4; i++)to->stubb[i] = from->stubb[i];
	to->seed_0x2FEE5 = from->word_0x2FEE5;
	for (int i = 0; i < 2; i++)to->stubb2[i] = from->stubb2[i];
	to->offset_0x2FEE9 = from->word_0x2FEE9;
	for (int i = 0; i < 2; i++)to->stubc[i] = from->stubc[i];
	to->raise_0x2FEED = from->word_0x2FEED;
	for (int i = 0; i < 2; i++)to->stubc2[i] = from->stubc2[i];
	to->gnarl_0x2FEF1 = from->word_0x2FEF1;
	for (int i = 0; i < 2; i++)to->stubc3[i] = from->stubc3[i];
	to->river_0x2FEF5 = from->word_0x2FEF5;
	to->lriver_0x2FEF9 = from->word_0x2FEF9;
	for (int i = 0; i < 2; i++)to->stube[i] = from->stube[i];
	to->source_0x2FEFD = from->word_0x2FEFD;
	for (int i = 0; i < 2; i++)to->stubf[i] = from->stubf[i];
	to->snLin_0x2FF01 = from->word_0x2FF01;
	for (int i = 0; i < 2; i++)to->stubg[i] = from->stubg[i];
	to->snFlt_0x2FF05 = from->word_0x2FF05;
	for (int i = 0; i < 2; i++)to->stubh[i] = from->stubh[i];
	to->bhLin_0x2FF09 = from->word_0x2FF09;
	for (int i = 0; i < 2; i++)to->stubi[i] = from->stubi[i];
	to->bhFlt_0x2FF0D = from->word_0x2FF0D;
	for (int i = 0; i < 2; i++)to->stubj[i] = from->stubj[i];
	to->rkSte_0x2FF11 = from->word_0x2FF11;
	for (int i = 0; i < 1022; i++)to->stubk[i] = from->stubk[i];
	for (int i = 0; i < 1200; i++)to->entity_0x30311[i] = from->array_0x30311[i];
	to->next_0x360D1 = from->next_0x360D1;
	for (int i = 0; i < 8; i++)to->next_0x360D2[i] = from->next_0x360D2[i];
	for (int i = 0; i < 8; i++)to->stages_0x36442[i] = from->str_0x36442[i];
	for (int i = 0; i < 11; i++)
	{
		to->StageVars_0x3647A[i].index_0x3647A_0 = from->array_0x3647A[i].str_0x3647A_byte_0;
		to->StageVars_0x3647A[i].stage_0x3647A_1 = from->array_0x3647A[i].str_0x3647A_byte_1;
		to->StageVars_0x3647A[i].str_0x3647A_2 = from->array_0x3647A[i].str_0x3647A_2;
		to->StageVars_0x3647A[i].str_0x3647C_4.axis = from->array_0x3647A[i].str_0x3647C_4.axis;
	}
}

void Convert_from_shadow_D41A0_BYTESTR_0(type_shadow_D41A0_BYTESTR_0* from, type_D41A0_BYTESTR_0* to) {
	for (int i = 0; i < 4; i++)to->stub0[i] = from->stub0[i];
	to->dword_0x4 = from->dword_0x4;
	to->rand_0x8 = from->dword_0x8;
	to->LevelIndex_0xc = from->word_0xc;
	to->word_0xe = from->word_0xe;
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
	for (int i = 0; i < 0x3e8; i++)to->pointers_0x246[i] = (type_event_0x6E8E*)from->pointers_0x246[i];
	to->dword_0x11e6 = from->dword_0x11e6;
	for (int i = 0; i < 0x3e8; i++)to->dword_0x11EA[i] = (type_event_0x6E8E*)from->dword_0x11EA[i];
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
	for (int i = 0; i < 8; i++)to->array_0x6E3E[i] = from->array_0x6E3E[i];
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
		to->struct_0x6E8E[i].word_0x1C_28 = from->struct_0x6E8E[i].word_0x1C_28;
		to->struct_0x6E8E[i].word_0x1E_30 = from->struct_0x6E8E[i].word_0x1E_30;
		to->struct_0x6E8E[i].word_0x20_32 = from->struct_0x6E8E[i].word_0x20_32;
		to->struct_0x6E8E[i].word_0x22_34 = from->struct_0x6E8E[i].word_0x22_34;
		to->struct_0x6E8E[i].word_0x24_36 = from->struct_0x6E8E[i].word_0x24_36;
		to->struct_0x6E8E[i].word_0x26_38 = from->struct_0x6E8E[i].word_0x26_38;
		to->struct_0x6E8E[i].parentId_0x28_40 = from->struct_0x6E8E[i].word_0x28_40;
		to->struct_0x6E8E[i].word_0x2A_42 = from->struct_0x6E8E[i].word_0x2A_42;
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
		to->struct_0x6E8E[i].byte_0x3D_61 = from->struct_0x6E8E[i].byte_0x3D_61;
		to->struct_0x6E8E[i].byte_0x3E_62 = from->struct_0x6E8E[i].byte_0x3E_62;
		to->struct_0x6E8E[i].class_0x3F_63 = from->struct_0x6E8E[i].type_0x3F_63;
		to->struct_0x6E8E[i].model_0x40_64 = from->struct_0x6E8E[i].subtype_0x40_64;
		to->struct_0x6E8E[i].xtype_0x41_65 = from->struct_0x6E8E[i].byte_0x41_65;
		to->struct_0x6E8E[i].xsubtype_0x42_66 = from->struct_0x6E8E[i].byte_0x42_66;
		to->struct_0x6E8E[i].byte_0x43_67 = from->struct_0x6E8E[i].byte_0x43_67;
		to->struct_0x6E8E[i].byte_0x44_68 = from->struct_0x6E8E[i].byte_0x44_68;
		to->struct_0x6E8E[i].state_0x45_69 = from->struct_0x6E8E[i].byte_0x45_69;
		to->struct_0x6E8E[i].byte_0x46_70 = from->struct_0x6E8E[i].byte_0x46_70;
		to->struct_0x6E8E[i].byte_0x47_71_xx = from->struct_0x6E8E[i].byte_0x47_71_xx;
		to->struct_0x6E8E[i].StageVar1_0x48_72 = from->struct_0x6E8E[i].byte_0x48_72;
		to->struct_0x6E8E[i].StageVar2_0x49_73 = from->struct_0x6E8E[i].byte_0x49_73;
		to->struct_0x6E8E[i].word_0x4A_74 = from->struct_0x6E8E[i].word_0x4A_74;
		to->struct_0x6E8E[i].axis_0x4C_76 = from->struct_0x6E8E[i].array_0x4C_76;
		to->struct_0x6E8E[i].array_0x52_82 = from->struct_0x6E8E[i].array_0x52_82;
		to->struct_0x6E8E[i].word_0x5A_90 = from->struct_0x6E8E[i].word_0x5A_90;
		to->struct_0x6E8E[i].byte_0x5C_92 = from->struct_0x6E8E[i].byte_0x5C_92;
		to->struct_0x6E8E[i].byte_0x5D_93 = from->struct_0x6E8E[i].byte_0x5D_93;
		to->struct_0x6E8E[i].str_0x5E_94 = from->struct_0x6E8E[i].str_0x5E_94;
		to->struct_0x6E8E[i].actSpeed_0x82_130 = from->struct_0x6E8E[i].word_0x82_130;
		to->struct_0x6E8E[i].minSpeed_0x84_132 = from->struct_0x6E8E[i].word_0x84_132;
		to->struct_0x6E8E[i].maxSpeed_0x86_134 = from->struct_0x6E8E[i].word_0x86_134;
		to->struct_0x6E8E[i].dword_0x88_136 = from->struct_0x6E8E[i].dword_0x88_136;
		to->struct_0x6E8E[i].maxMana_0x8C_140 = from->struct_0x6E8E[i].dword_0x8C_140;
		to->struct_0x6E8E[i].mana_0x90_144 = from->struct_0x6E8E[i].dword_0x90_144;
		to->struct_0x6E8E[i].word_0x94_148 = from->struct_0x6E8E[i].word_0x94_148;
		to->struct_0x6E8E[i].word_0x96_150 = from->struct_0x6E8E[i].word_0x96_150;
		to->struct_0x6E8E[i].word_0x98_152 = from->struct_0x6E8E[i].word_0x98_152;
		to->struct_0x6E8E[i].word_0x9A_154x = from->struct_0x6E8E[i].word_0x9A_154x;
		to->struct_0x6E8E[i].dword_0xA0_160x = (type_str_160*)from->struct_0x6E8E[i].dword_0xA0_160x;
		to->struct_0x6E8E[i].dword_0xA4_164x = (type_str_164*)from->struct_0x6E8E[i].dword_0xA4_164x;
	}

	to->terrain_2FECE.word_2FECE = from->str_2FECE.word_2FECE;
	to->terrain_2FECE.word_2FED0 = from->str_2FECE.word_2FED0;
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
	for (int i = 0; i < 8; i++)to->terrain_2FECE.next_0x360D2[i] = from->str_2FECE.next_0x360D2[i];
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
		to->str_0x3664C[i].event_A.pointer_0x6E8E = (type_event_0x6E8E*)from->str_0x3664C[i].dword_A;
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
	to->byte_0x36E04 = from->byte_0x36E04;
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
	to->word_0xe = from->word_0xe;
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
	for (int i = 0; i < 8; i++)to->array_0x6E3E[i] = from->array_0x6E3E[i];
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
		to->struct_0x6E8E[i].word_0x1C_28 = from->struct_0x6E8E[i].word_0x1C_28;
		to->struct_0x6E8E[i].word_0x1E_30 = from->struct_0x6E8E[i].word_0x1E_30;
		to->struct_0x6E8E[i].word_0x20_32 = from->struct_0x6E8E[i].word_0x20_32;
		to->struct_0x6E8E[i].word_0x22_34 = from->struct_0x6E8E[i].word_0x22_34;
		to->struct_0x6E8E[i].word_0x24_36 = from->struct_0x6E8E[i].word_0x24_36;
		to->struct_0x6E8E[i].word_0x26_38 = from->struct_0x6E8E[i].word_0x26_38;
		to->struct_0x6E8E[i].word_0x28_40 = from->struct_0x6E8E[i].parentId_0x28_40;
		to->struct_0x6E8E[i].word_0x2A_42 = from->struct_0x6E8E[i].word_0x2A_42;
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
		to->struct_0x6E8E[i].byte_0x3D_61 = from->struct_0x6E8E[i].byte_0x3D_61;
		to->struct_0x6E8E[i].byte_0x3E_62 = from->struct_0x6E8E[i].byte_0x3E_62;
		to->struct_0x6E8E[i].type_0x3F_63 = from->struct_0x6E8E[i].class_0x3F_63;
		to->struct_0x6E8E[i].subtype_0x40_64 = from->struct_0x6E8E[i].model_0x40_64;
		to->struct_0x6E8E[i].byte_0x41_65 = from->struct_0x6E8E[i].xtype_0x41_65;
		to->struct_0x6E8E[i].byte_0x42_66 = from->struct_0x6E8E[i].xsubtype_0x42_66;
		to->struct_0x6E8E[i].byte_0x43_67 = from->struct_0x6E8E[i].byte_0x43_67;
		to->struct_0x6E8E[i].byte_0x44_68 = from->struct_0x6E8E[i].byte_0x44_68;
		to->struct_0x6E8E[i].byte_0x45_69 = from->struct_0x6E8E[i].state_0x45_69;
		to->struct_0x6E8E[i].byte_0x46_70 = from->struct_0x6E8E[i].byte_0x46_70;
		to->struct_0x6E8E[i].byte_0x47_71_xx = from->struct_0x6E8E[i].byte_0x47_71_xx;
		to->struct_0x6E8E[i].byte_0x48_72 = from->struct_0x6E8E[i].StageVar1_0x48_72;
		to->struct_0x6E8E[i].byte_0x49_73 = from->struct_0x6E8E[i].StageVar2_0x49_73;
		to->struct_0x6E8E[i].word_0x4A_74 = from->struct_0x6E8E[i].word_0x4A_74;
		to->struct_0x6E8E[i].array_0x4C_76 = from->struct_0x6E8E[i].axis_0x4C_76;
		to->struct_0x6E8E[i].array_0x52_82 = from->struct_0x6E8E[i].array_0x52_82;
		to->struct_0x6E8E[i].word_0x5A_90 = from->struct_0x6E8E[i].word_0x5A_90;
		to->struct_0x6E8E[i].byte_0x5C_92 = from->struct_0x6E8E[i].byte_0x5C_92;
		to->struct_0x6E8E[i].byte_0x5D_93 = from->struct_0x6E8E[i].byte_0x5D_93;
		to->struct_0x6E8E[i].str_0x5E_94 = from->struct_0x6E8E[i].str_0x5E_94;
		to->struct_0x6E8E[i].word_0x82_130 = from->struct_0x6E8E[i].actSpeed_0x82_130;
		to->struct_0x6E8E[i].word_0x84_132 = from->struct_0x6E8E[i].minSpeed_0x84_132;
		to->struct_0x6E8E[i].word_0x86_134 = from->struct_0x6E8E[i].maxSpeed_0x86_134;
		to->struct_0x6E8E[i].dword_0x88_136 = from->struct_0x6E8E[i].dword_0x88_136;
		to->struct_0x6E8E[i].dword_0x8C_140 = from->struct_0x6E8E[i].maxMana_0x8C_140;
		to->struct_0x6E8E[i].dword_0x90_144 = from->struct_0x6E8E[i].mana_0x90_144;
		to->struct_0x6E8E[i].word_0x94_148 = from->struct_0x6E8E[i].word_0x94_148;
		to->struct_0x6E8E[i].word_0x96_150 = from->struct_0x6E8E[i].word_0x96_150;
		to->struct_0x6E8E[i].word_0x98_152 = from->struct_0x6E8E[i].word_0x98_152;
		to->struct_0x6E8E[i].word_0x9A_154x = from->struct_0x6E8E[i].word_0x9A_154x;
		to->struct_0x6E8E[i].dword_0xA0_160x = ((uint8_t*)from->struct_0x6E8E[i].dword_0xA0_160x - Zero_pointer);
		to->struct_0x6E8E[i].dword_0xA4_164x = ((uint8_t*)from->struct_0x6E8E[i].dword_0xA4_164x - Zero_pointer);
	}

	to->str_2FECE.word_2FECE = from->terrain_2FECE.word_2FECE;
	to->str_2FECE.word_2FED0 = from->terrain_2FECE.word_2FED0;
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
	for (int i = 0; i < 8; i++)to->str_2FECE.next_0x360D2[i] = from->terrain_2FECE.next_0x360D2[i];
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
	to->byte_0x36E04 = from->byte_0x36E04;
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
