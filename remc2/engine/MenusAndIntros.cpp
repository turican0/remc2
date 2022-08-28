#include "MenusAndIntros.h"
#include "CommandLineParser.h"


#ifdef __linux__
void _strupr(char* s)
{
	char* p = s;

	while (*p) {
		*p = toupper(*p);
		++p;
	}
}
#else //__linux__
#endif //__linux__

bool first_enter = true;

__int16 x_WORD_17DBC4 = 0; // weak//x_DWORD_17DBB8[3] 34ebc4

char x_BYTE_E1B9C[8] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' }; // idb x_WORD_E1964x[0x238+
char x_BYTE_E1BA4[8] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' }; // idb x_WORD_E1964x[0x240+


#pragma pack (1)
typedef struct {//lenght 7
	Type_SoundEvent_E17CC* pSoundEvent_0;
	uint8_t byte_4;
	uint8_t byte_5;
	uint8_t byte_6;
}
type_E16E0;
#pragma pack (16)

Type_SoundEvent_E17CC str_E1328[28] = {
{0x0000,0x45,0x00000005},
{0x0000,0x56,0x00000000},
{0x0000,0x57,0x00000000},
{0x0000,0x41,0x00000005},
{0x0001,0x4D,0x00000006},
{0x0018,0x53,0x00000002},
{0x0042,0x53,0x00000002},
{0x0046,0x53,0x00000002},
{0x0048,0x53,0x00000002},
{0x005C,0x53,0x00000002},
{0x005E,0x53,0x00000002},
{0x007C,0x57,0x00000000},
{0x007C,0x55,0x0000010A},
{0x007C,0x41,0x0000000A},
{0x007C,0x53,0x00000001},
{0x00D2,0x57,0x00000000},
{0x00D2,0x55,0x0000010B},
{0x0118,0x57,0x00000000},
{0x0118,0x55,0x0000010C},
{0x0154,0x57,0x00000000},
{0x0154,0x55,0x0000010D},
{0x01C2,0x57,0x00000000},
{0x01C2,0x59,0x00000000},
{0x01CC,0x53,0x00000003},
{0x01D6,0x53,0x00000003},
{0x01DA,0x53,0x00000003},
{0x01DE,0x53,0x00000003},
{0xFFFF,0xFF,0x00000001}};

Type_SoundEvent_E17CC str_E13EC[27] = {
{0x0000,0x45,0x00000006},
{0x0000,0x56,0x00000000},
{0x0000,0x57,0x00000000},
{0x0000,0x41,0x00000005},
{0x0001,0x4D,0x00000006},
{0x0018,0x53,0x00000002},
{0x0042,0x53,0x00000002},
{0x0046,0x53,0x00000002},
{0x0048,0x53,0x00000002},
{0x005C,0x53,0x00000002},
{0x005E,0x53,0x00000002},
{0x007C,0x57,0x00000000},
{0x007C,0x55,0x0000010E},
{0x007C,0x41,0x0000000A},
{0x007C,0x53,0x00000001},
{0x00C8,0x57,0x00000000},
{0x00C8,0x55,0x0000010F},
{0x0118,0x57,0x00000000},
{0x0118,0x55,0x00000110},
{0x0154,0x57,0x00000000},
{0x0154,0x55,0x00000111},
{0x01B8,0x53,0x00000003},
{0x01C2,0x59,0x00000000},
{0x01D0,0x53,0x00000003},
{0x01DE,0x53,0x00000003},
{0x0203,0x53,0x00000003},
{0xFFFF,0xFF,0x00000001} };

Type_SoundEvent_E17CC str_E14AC[28] = {
{0x0000,0x45,0x00000007},
{0x0000,0x56,0x00000000},
{0x0000,0x57,0x00000000},
{0x0000,0x41,0x00000005},
{0x0001,0x4D,0x00000006},
{0x0018,0x53,0x00000002},
{0x0042,0x53,0x00000002},
{0x0046,0x53,0x00000002},
{0x0048,0x53,0x00000002},
{0x005C,0x53,0x00000002},
{0x005E,0x53,0x00000002},
{0x007C,0x57,0x00000000},
{0x007C,0x55,0x00000112},
{0x007C,0x41,0x0000000A},
{0x007C,0x53,0x00000001},
{0x00DE,0x57,0x00000000},
{0x00DE,0x55,0x00000113},
{0x015E,0x59,0x00000000},
{0x0162,0x53,0x00000003},
{0x016C,0x53,0x00000003},
{0x0176,0x53,0x00000003},
{0x0180,0x53,0x00000003},
{0x018A,0x53,0x00000003},
{0x0194,0x53,0x00000003},
{0x019E,0x53,0x00000003},
{0x01A8,0x53,0x00000003},
{0x01B2,0x53,0x00000003},
{0xFFFF,0xFF,0x00000001} };

Type_SoundEvent_E17CC str_E1570[28] = {
{0x0000,0x45,0x00000008},
{0x0000,0x56,0x00000000},
{0x0000,0x57,0x00000000},
{0x0000,0x41,0x00000005},
{0x0001,0x4D,0x00000006},
{0x0018,0x53,0x00000002},
{0x0042,0x53,0x00000002},
{0x0046,0x53,0x00000002},
{0x0048,0x53,0x00000002},
{0x005C,0x53,0x00000002},
{0x005E,0x53,0x00000002},
{0x007C,0x57,0x00000000},
{0x007C,0x55,0x00000114},
{0x007C,0x41,0x0000000A},
{0x007C,0x53,0x00000001},
{0x00C8,0x57,0x00000000},
{0x00C8,0x55,0x00000115},
{0x0118,0x57,0x00000000},
{0x0118,0x55,0x00000116},
{0x015E,0x59,0x00000000},
{0x0168,0x53,0x00000003},
{0x016D,0x53,0x00000003},
{0x0172,0x53,0x00000003},
{0x017C,0x53,0x00000003},
{0x0181,0x53,0x00000003},
{0x0186,0x53,0x00000003},
{0x0190,0x53,0x00000003},
{0xFFFF,0xFF,0x00000001} };

Type_SoundEvent_E17CC str_E1634[20] = {
{ 0x0000,0x45,0x00000009 },
{ 0x0000,0x56,0x00000000 },
{ 0x0000,0x57,0x00000000 },
{ 0x0000,0x41,0x00000005 },
{ 0x0001,0x4D,0x00000006 },
{ 0x0018,0x53,0x00000002 },
{ 0x0042,0x53,0x00000002 },
{ 0x0046,0x53,0x00000002 },
{ 0x0048,0x53,0x00000002 },
{ 0x005C,0x53,0x00000002 },
{ 0x005E,0x53,0x00000002 },
{ 0x007C,0x57,0x00000000 },
{ 0x007C,0x55,0x00000117 },
{ 0x007C,0x41,0x0000000A },
{ 0x007C,0x53,0x00000001 },
{ 0x00C8,0x57,0x00000000 },
{ 0x00C8,0x55,0x00000118 },
{ 0xFFFF,0xFF,0x00000001 } };

Type_SoundEvent_E17CC str_E16B4[7] = {
{ 0x0000, 0x45, 0x00000004},
{ 0x0000,0x41,0x0000000A },
{ 0x0000,0x41,0x00000005 },
{ 0x0001,0x4D,0x00000002 },
{ 0x001E,0x53,0x00000009 },
{ 0x0038,0x53,0x0000000A },
{ 0x01FF,0x28,0x05002B23 } };

type_E16E0 str_E16E0[7] = {//cutscenes
{str_E1328,0x05,0x00,0x01},
{str_E13EC,0x09,0x00,0x02},
{str_E14AC,0x0D,0x00,0x03},
{str_E1570,0x11,0x00,0x04},
{str_E1634,0x18,0x00,0x05},
{str_E16B4,0x19,0x00,0x06},
{NULL,0x00,0x00,0x00}
};

type_x_BYTE_E25ED_db_str x_BYTE_E26C8_str[16] = {//set keys
{0x00000000,0x00000000,0x01BF,0x0274,0x0073,0x008A,0x0073,0x0004,0x00,0x01,0x0007,0x0026,0x0002,0x0000},
{0x00000000,0x00000000,0x036C,0x0216,0x0073,0x008A,0x0075,0x0008,0x00,0x01,0x0008,0x0026,0x0002,0x0000},
{0x00000000,0x00000000,0x0221,0x0036,0x0055,0x0055,0x0055,0x0000,0x01,0x00,0xFFFF,0x0000,0x0000,0x0000},
{0x00000000,0x00000000,0x028F,0x003A,0x0055,0x0055,0x0057,0x0000,0x01,0x00,0xFFFF,0x0000,0x0000,0x0000},
{0x00000000,0x00000000,0x0234,0x0058,0x0055,0x0055,0x0058,0x0000,0x01,0x00,0xFFFF,0x0000,0x0000,0x0000},
{0x00000000,0x00000000,0x0266,0x007B,0x0055,0x0055,0x0059,0x0000,0x01,0x00,0xFFFF,0x0000,0x0000,0x0000},
{0x00000000,0x00000000,0x0221,0x0036,0x0056,0x005C,0x0056,0x0008,0x00,0x01,0x0009,0x0017,0x0003,0x0000},
{0x00000000,0x00000000,0x028F,0x003A,0x0056,0x005C,0x0058,0x0004,0x00,0x01,0xFFFF,0x0017,0x0002,0x0000},
{0x00000000,0x00000000,0x0234,0x0058,0x0056,0x005C,0x0059,0x0016,0x00,0x01,0xFFFF,0x0017,0x0003,0x0000},
{0x00000000,0x00000000,0x0266,0x007B,0x0056,0x005C,0x005A,0x0015,0x00,0x01,0xFFFF,0x0017,0x0003,0x0000},
{0x00000000,0x00000000,0x033F,0x00F5,0x002E,0x003A,0x0031,0x0000,0x01,0x00,0x000A,0x0005,0x0002,0x0000},
{0x00000000,0x00000000,0x033F,0x00F5,0x002E,0x003A,0x0031,0x0000,0x01,0x01,0x000A,0x0005,0x0002,0xFFFF},
{0x00000000,0x00000000,0x035F,0x0149,0x002E,0x003A,0x002E,0x0000,0x01,0x00,0xFFFF,0x0005,0x0002,0x0000},
{0x00000000,0x00000000,0x0276,0x025F,0x002E,0x003A,0x0034,0x0000,0x01,0x00,0xFFFF,0x0005,0x0002,0x0000},
{0x00000000,0x00000000,0x00F4,0x0278,0x002E,0x003A,0x0038,0x0000,0x01,0x00,0xFFFF,0x0005,0x0002,0x0000},
{0x00000000,0x00000000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00,0x00,0x0000,0x0000,0x0000,0x0000}
};

type_x_BYTE_E25ED_2BB str_BYTE_E2590[0x3] = {//set joy
{ 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x01B5,0x0000,0x0001 },
{ 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x01B6,0x0000,0x0002 },
{ 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000 }
};

#pragma pack (1)
typedef struct {//lenght 24
uint32_t dword_0;
uint32_t dword_4;
uint32_t dword_8;
uint32_t dword_12;
uint16_t word_16;
uint16_t word_18;
uint16_t word_20;
uint16_t word_22;
}
type_E1BAC_0x3c4;
#pragma pack (16)

//0x2ec
type_E1BAC_0x3c4 str_E1BAC_0x2ec[9] = {
{0x0005BC1E,0x000032D0,0x00076BBD,0x00000300,0x00F4,0x0085,0x0088,0x015C},
{0x0005EEEE,0x00003282,0x00076EBD,0x00000300,0x00D0,0x0081,0x00B4,0x0160},
{0x00062170,0x00003BE3,0x000771BD,0x00000300,0x00E2,0x0090,0x0096,0x0151},
{0x00065D53,0x000042B6,0x000774BD,0x00000300,0x00E0,0x006F,0x00A4,0x0172},
{0x0006A009,0x00003029,0x000777BD,0x00000300,0x00EC,0x0070,0x009C,0x0171},
{0x0006D032,0x00002E89,0x00077ABD,0x00000300,0x00E0,0x0087,0x00B4,0x0160},
{0x0006FEBB,0x00003BDC,0x00077DBD,0x00000300,0x00DD,0x0084,0x00A5,0x015D},
{0x00073A97,0x00003126,0x000780BD,0x00000300,0x00F0,0x008B,0x008C,0x0156},
{0x00000000,0x00000000,0x00000000,0x00000000,0x0000,0x0000,0x0000,0x0000} };

#pragma pack (1)
typedef struct {//lenght 69
	int32_t dword_0;//0
	uint8_t stub_4;//4
	uint8_t stub_5;//5
	uint8_t stub_6;//6
	uint8_t stub_7;//7
	uint8_t stub_8;//8
	uint8_t stub_9;//9
	int16_t word_10;//10	
	int8_t byte_12;//12
	uint8_t stub_13;//13
	int16_t word_14;//14	
	int8_t byte_16;//16
	uint8_t stub_17;//17
	int16_t array_word_18[12];////18
	int8_t byte_42;//42
	int8_t byte_43;//43
	uint8_t byte_44;//44
	int16_t array_word_45[12];//45
}
type_WORD_E20A4;
#pragma pack (16)

type_WORD_E20A4 str_WORD_E20A4[12] = {
{0x00000000,0x00,0x00,0x00,0x00,0x01,0x00,0x0025,0x02,0x00,0x0025,0x03,0x00,{0x0000,0x0000,0x0180,0x0000,0x021F,0x03C0,0x008C,0x0091,0x008C,0x008C,0x0091,0x0032},0x01,0x00,0x03,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}},
{0x00000000,0x00,0x00,0x00,0x00,0x01,0x00,0x0025,0x02,0x00,0x0025,0x03,0x00,{0x0000,0x0000,0x018B,0x000A,0x0226,0x03CA,0x008C,0x0091,0x008C,0x008C,0x0091,0x0032},0x01,0x00,0x03,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}},
{0x00000000,0x00,0x00,0x00,0x00,0x01,0x00,0x0025,0x02,0x00,0x0025,0x03,0x00,{0x0000,0x0000,0x0190,0x0006,0x022B,0x03D4,0x008C,0x0091,0x008C,0x008C,0x0091,0x0032},0x01,0x00,0x03,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}},
{0x00000000,0x00,0x00,0x00,0x00,0x01,0x00,0x0027,0x02,0x00,0x0027,0x03,0x00,{0x0000,0x0000,0x0001,0x00BE,0x04FC,0x0134,0x00CE,0x00D3,0x00CE,0x00CE,0x00D3,0x0028},0x01,0x00,0x03,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}},
{0x00000000,0x00,0x00,0x00,0x00,0x01,0x00,0x0025,0x02,0x00,0x0025,0x03,0x00,{0x0000,0x0000,0x01CB,0x0000,0x04FE,0x03C0,0x0092,0x0097,0x0092,0x0092,0x0097,0x0014},0x01,0x00,0x03,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}},
{0x00000000,0x00,0x00,0x00,0x00,0x01,0x00,0x0009,0x02,0x00,0x0003,0x03,0x00,{0x0000,0x0000,0x0253,0x0000,0x023A,0x027C,0x0115,0x011C,0x0115,0x005D,0x0067,0x0000},0x02,0x00,0x03,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}},
{0x00000000,0x00,0x00,0x00,0x00,0x01,0x00,0x0009,0x02,0x00,0x0003,0x03,0x00,{0x0000,0x0000,0x0262,0x0072,0x0262,0x0072,0x0129,0x0130,0x0129,0x0129,0x0130,0x0000},0x03,0x00,0x18,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}},
{0x00000000,0x00,0x00,0x00,0x00,0x01,0x00,0x0009,0x02,0x00,0x0003,0x03,0x00,{0x0000,0x0000,0x0165,0x0000,0x011F,0x0290,0x006B,0x0072,0x006B,0x005D,0x0067,0x000F},0x01,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}},
{0x00000000,0x00,0x00,0x00,0x00,0x02,0x00,0x0009,0x02,0x00,0x0003,0x03,0x00,{0x0000,0x0000,0x03AB,0x0000,0x036F,0x0266,0x006B,0x0072,0x006B,0x005D,0x0067,0x000F},0x01,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}},
{0x00000000,0x00,0x00,0x00,0x00,0x03,0x00,0x0009,0x02,0x00,0x0003,0x03,0x00,{0x0000,0x0000,0x01AF,0x0000,0x0169,0x0200,0x006B,0x0072,0x006B,0x005D,0x0067,0x000B},0x01,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}},
{0x00000000,0x00,0x00,0x00,0x00,0x04,0x00,0x0009,0x02,0x00,0x0003,0x03,0x00,{0x0000,0x0000,0x01C7,0x0000,0x018B,0x0072,0x006B,0x0072,0x006B,0x005D,0x0067,0x0012},0x01,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}},
{0x00000000,0x00,0x00,0x00,0x00,0x00,0x00,0x0000,0x00,0x00,0x0000,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000},0x00,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}}};

type_E24BCx str_E2516[0x5] = {
{0x01D2,0x012F,0x0169,0x01DC,0x016A,0x0000,0x023C,0x01B8,0x02,0x0B},
{0x0195,0x015E,0x0041,0x0211,0x0052,0x0000,0x0254,0x001C,0x08,0x08},
{0x0196,0x007E,0x0041,0x007B,0x0052,0x0000,0x0037,0x001C,0x08,0x09},
{0x01D3,0x007F,0x0168,0x007B,0x0176,0x0000,0x0034,0x01B0,0x08,0x10},
{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00,0x00} };

Type_SoundEvent_E17CC str_E17CC_0[0x32] = {//anim events
{0x0000,0x45,0x00000004},
{0x0000,0x56,0x00000000},
{0x0000,0x57,0x00000000},
{0x0000,0x55,0x00000010},
{0x0000,0x41,0x0000000A},
{0x0002,0x44,0x00000005},
{0x0005,0x53,0x00000003},
{0x0048,0x57,0x00000000},
{0x0048,0x55,0x00000011},
{0x0087,0x57,0x00000000},
{0x0087,0x55,0x00000012},
{0x00BD,0x41,0x000000F0},
{0x00BE,0x41,0x00000005},
{0x00BE,0x48,0x00000001},
{0x00BE,0x50,0x00000001},
{0x0104,0x53,0x00000002},
{0x015E,0x53,0x0000000B},
{0x016D,0x53,0x0000000D},
{0x0190,0x46,0x00000001},
{0x0194,0x41,0x00000007},
{0x019A,0x53,0x00000006},
{0x019A,0x57,0x00000000},
{0x019A,0x55,0x00000013},
{0x01FE,0x53,0x00000004},
{0x01FE,0x57,0x00000000},
{0x01FE,0x55,0x00000014},
{0x0246,0x53,0x00000009},
{0x024D,0x41,0x00000005},
{0x0267,0x53,0x0000000A},
{0x02BC,0x53,0x00000008},
{0x02BC,0x57,0x00000000},
{0x02BC,0x55,0x00000015},
{0x02CD,0x53,0x0000000C},
{0x0322,0x48,0x0000000E},
{0x0322,0x4F,0x0000000E},
{0x038E,0x41,0x00000009},
{0x03FD,0x41,0x00000005},
{0x03FD,0x46,0x0000000E},
{0x03FF,0x53,0x00000007},
{0x0410,0x57,0x00000000},
{0x0410,0x55,0x00000016},
{0x0417,0x41,0x0000000A},
{0x0417,0x53,0x00000005},
{0x0450,0x41,0x00000005},
{0x0460,0x53,0x0000000F},
{0x04C9,0x53,0x00000010},
{0x04CE,0x53,0x00000011},
{0x04FC,0x41,0x00000003},
{0x04FC,0x59,0x00000000},
{0xFFFF,0xFF,0xFFFFFFFF} };

Type_SoundEvent_E17CC str_E17CC_0x160[10] = {
{0x0000,0x41,0x00000003},
{0x0000,0x59,0x00000000},
{0x0000,0x45,0x00000004},
{0x0001,0x53,0x00000012},
{0x0001,0x58,0x00000005},
{0x000A,0x53,0x00000013},
{0x0060,0x41,0x00000064},
{0x0000,0x22,0x004E01AE},//FIXME
{0x0000,0x00,0x00000000},
{0x9D00,0x01,0x00000000} };

type_WORD_E1F84 str_E23E0[6] = {
{0x0025F620,0x00000000,0x0000,0x0245,0x01AB,0x0028,0x0046,0x0000,0xF7,0xF6,0x0B,0x01,0x00,0x01,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003}},
{0x0025F640,0x00000000,0x0000,0x0006,0x01AB,0x0028,0x0046,0x0000,0xF9,0xF8,0x10,0x01,0x00,0x01,{0x0025,0x015C,0x0000,0x0000,0x003C,0x0000,0x01D3,0x0000,0x0003}},
{0x0025F800,0x00000000,0x0000,0x0000,0x0000,0x0028,0x0046,0x0196,0xFB,0xFA,0x09,0x01,0x00,0x01,{0x001D,0x003C,0x0000,0x0000,0x00C8,0x0000,0x01A6,0x0000,0x0003}},
{0x0025F820,0x00000000,0x0000,0x0245,0x0000,0x0028,0x0046,0x0195,0xFD,0xFC,0x08,0x01,0x00,0x01,{0x01FE,0x003C,0x0000,0x0000,0x00C8,0x0000,0x01A5,0x0000,0x0003}},
{0x00000000,0x00000000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00,0x00,0x00,0x00,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}}};

type_WORD_E1F84 str_WORD_E1F84[3] = {
{0x00000000,0x00000000,0x0000,0x0123,0x00CD,0x003C,0x003C,0x0000,0x24,0x23,0x01,0x01,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003}},
{0x00000000,0x00000000,0x0000,0x0120,0x0063,0x003C,0x003C,0x0000,0x4C,0x4D,0x02,0x01,0x01,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003}},
{0x00000000,0x00000000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00,0x00,0x00,0x00,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}}
};

type_WORD_E1F84 str_E1BAC[10] = {
{0x00258350,0x00000000,0x0000,0x00CE,0x0043,0x0050,0x0050,0x019A,0x3B,0x33,0x06,0x01,0x00,0x00,{0x004E,0x004E,0x0000,0x0000,0x0050,0x0000,0x0000,0x0000,0x0003}},
{0x00259E00,0x00000000,0x0000,0x0119,0x0041,0x0050,0x0050,0x0194,0x3C,0x34,0x0A,0x01,0x00,0x00,{0x0164,0x0070,0x0000,0x0000,0x0050,0x0000,0x01A4,0x0000,0x0003}},
{0x0025EE80,0x00000000,0x0000,0x016A,0x0048,0x0050,0x0050,0x0199,0x3D,0x35,0x07,0x01,0x00,0x00,{0x01AE,0x004E,0x0000,0x0000,0x0050,0x0000,0x019C,0x0000,0x0003}},
{0x00259730,0x00000000,0x0000,0x00C8,0x009D,0x0050,0x0050,0x0196,0x3E,0x36,0x09,0x01,0x00,0x00,{0x004E,0x00A0,0x0000,0x0000,0x00C8,0x0000,0x01A6,0x0000,0x0003}},
{0x0025A610,0x00000000,0x0000,0x0195,0x00E7,0x003C,0x002C,0x01CF,0x6A,0x6A,0x0F,0x01,0x00,0x00,{0x0160,0x00AE,0x0000,0x0000,0x0050,0x0000,0x0000,0x0000,0x0003}},
{0x002590F0,0x00000000,0x0000,0x0187,0x009E,0x0050,0x0050,0x0195,0x3F,0x37,0x08,0x01,0x00,0x00,{0x01C0,0x00A0,0x0000,0x0000,0x00C8,0x0000,0x01A5,0x0000,0x0003}},
{0x00258980,0x00000000,0x0000,0x0126,0x0019,0x0034,0x002C,0x0197,0x40,0x38,0x0B,0x01,0x00,0x00,{0x0160,0x001A,0x0000,0x0000,0x0050,0x0000,0x0197,0x0000,0x0003}},
{0x002589E0,0x00000000,0x0000,0x0121,0x009B,0x003C,0x002C,0x0198,0x41,0x39,0x0C,0x01,0x00,0x00,{0x0160,0x00AE,0x0000,0x0000,0x0050,0x0000,0x0000,0x0000,0x0003}},
{0x0025A160,0x00000000,0x0000,0x00B9,0x00E8,0x003C,0x002C,0x019B,0x42,0x3A,0x0D,0x01,0x00,0x00,{0x004A,0x00F6,0x0000,0x0000,0x0064,0x0000,0x019B,0x0000,0x0003}},
{0x00000000,0x00000000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00,0x00,0x00,0x00,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}}};

uint16_t xx_WORD_E24BE[3][9] = { // 2b34be //only for clock?
	{0x0028,0x0031,0x00C4,0x0026,0x0000,0x011B,0x006A,0x0002,0x01C6},
	{0x012A,0x012C,0x018D,0x012A,0x0000,0x0141,0x00EC,0x0008,0x0000},
	{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x01D1}
};

type_E24BCx str_E24BCx[0x1A]= {
{0x01C7,0x0028,0x0031,0x00C4,0x0026,0x0000,0x011B,0x006A,0x02,0x00},
{0x01C6,0x012A,0x012C,0x018D,0x012A,0x0000,0x0141,0x00EC,0x08,0x00},
{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00,0x00},
{0x01D1,0x0103,0x0320,0x015D,0x0320,0x0000,0x0153,0x0320,0x02,0x0B},
{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00,0x00},
{0x01D2,0x012F,0x0169,0x01DC,0x016A,0x0000,0x023C,0x01B8,0x02,0x0B},
{0x0195,0x015E,0x0041,0x0211,0x0052,0x0000,0x0254,0x001C,0x08,0x08},
{0x0196,0x007E,0x0041,0x007B,0x0052,0x0000,0x0037,0x001C,0x08,0x09},
{0x01D3,0x007F,0x0168,0x007B,0x0176,0x0000,0x0034,0x01B0,0x08,0x10},
{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00,0x00},
{0x01C9,0x0004,0x00B8,0x0042,0x00B6,0x0000,0x007E,0x0082,0x00,0x00},
{0x01CB,0x0005,0x0168,0x004D,0x0168,0x0000,0x0095,0x0130,0x00,0x00},
{0x01CA,0x01BC,0x016A,0x0227,0x016A,0x0000,0x01C6,0x0122,0x00,0x00},
{0x01C8,0x01BC,0x00A8,0x0228,0x00A8,0x0000,0x01E8,0x0074,0x00,0x00},
{0x01CC,0x01A3,0x000D,0x01D7,0x0019,0x0000,0x0148,0x0018,0x08,0x00},
{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00,0x00},
{0x019F,0x0002,0x004D,0x007D,0x005A,0x003B,0x00DE,0x005A,0x02,0x06},
{0x01A0,0x0013,0x0074,0x00AC,0x007C,0x0001,0x0119,0x007C,0x02,0x0A},
{0x0196,0x0011,0x00F6,0x005F,0x00F6,0x003E,0x00C0,0x00AA,0x00,0x09},
{0x019B,0x0011,0x0168,0x005F,0x0168,0x003E,0x00C1,0x011C,0x00,0x0D},
{0x01A3,0x00DC,0x0155,0x013E,0x0155,0x0041,0x013C,0x00D0,0x00,0x0C},
{0x01CF,0x019A,0x0148,0x0203,0x0161,0x003F,0x01B5,0x0122,0x08,0x0F},
{0x01A2,0x0184,0x00ED,0x01E5,0x00EC,0x003F,0x01B0,0x00C4,0x08,0x08},
{0x01A1,0x0196,0x009D,0x01FC,0x00A6,0x003D,0x01A8,0x0062,0x08,0x07},
{0x0197,0x019B,0x001F,0x01E8,0x0029,0x0040,0x015A,0x0028,0x08,0x0B},
{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00,0x00} };

type_x_BYTE_E25ED_2BB str_BYTE_E25ED_2BB[0xb] = {
{ 0x00D4,0x0032,0x0000,0x0000,0x0000,0x0000,0x01BC,0x0001,0x0000 },
{ 0x00D4,0x0053,0x0000,0x0000,0x0000,0x0000,0x01BD,0x0000,0x0000 },
{ 0x00D4,0x0074,0x0000,0x0000,0x0000,0x0000,0x01BE,0x0000,0x0000 },
{ 0x00D4,0x0095,0x0000,0x0000,0x0000,0x0000,0x01BF,0x0000,0x0000 },
{ 0x00D4,0x00B6,0x0000,0x0000,0x0000,0x0000,0x01C0,0x0000,0x0000 },
{ 0x00D4,0x00D7,0x0000,0x0000,0x0000,0x0000,0x01C1,0x0000,0x0000 },
{ 0x00D4,0x00F8,0x0000,0x0000,0x0000,0x0000,0x01C2,0x0000,0x0000 },
{ 0x00D4,0x0119,0x0000,0x0000,0x0000,0x0000,0x01C3,0x0000,0x0000 },
{ 0x00D4,0x013A,0x0000,0x0000,0x0000,0x0000,0x01C4,0x0000,0x0000 },
{ 0x00D4,0x015B,0x0000,0x0000,0x0000,0x0000,0x01C5,0x0000,0x0000 },
{ 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000 } };

type_WORD_E1F84 str_WORD_E2008[3] = {
{0x00000000,0x00000000,0x0000,0x0111,0x000A,0x003C,0x003C,0x0000,0x00,0x6D,0x01,0x01,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003}},
{0x00000000,0x00000000,0x0000,0x011B,0x017D,0x003C,0x003C,0x0000,0x00,0x6C,0x02,0x01,0x01,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003}},
{0x00000000,0x00000000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00,0x00,0x00,0x00,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}}};

type_E24BCx str_E24F2[2] = {
{0x01D1,0x0103,0x0320,0x015D,0x0320,0x0000,0x0153,0x0320,0x02,0x0B},
{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00,0x00} };

type_E24BCx str_E2570[0x6] = {
{0x01C9,0x0004,0x00B8,0x0042,0x00B6,0x0000,0x007E,0x0082,0x00,0x00},
{0x01CB,0x0005,0x0168,0x004D,0x0168,0x0000,0x0095,0x0130,0x00,0x00},
{0x01CA,0x01BC,0x016A,0x0227,0x016A,0x0000,0x01C6,0x0122,0x00,0x00},
{0x01C8,0x01BC,0x00A8,0x0228,0x00A8,0x0000,0x01E8,0x0074,0x00,0x00},
{0x01CC,0x01A3,0x000D,0x01D7,0x0019,0x0000,0x0148,0x0018,0x08,0x00},
{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00,0x00} };

type_E24BCx str_E25DC[0xA] = {
{0x019F,0x0002,0x004D,0x007D,0x005A,0x003B,0x00DE,0x005A,0x02,0x06},
{0x01A0,0x0013,0x0074,0x00AC,0x007C,0x0001,0x0119,0x007C,0x02,0x0A},
{0x0196,0x0011,0x00F6,0x005F,0x00F6,0x003E,0x00C0,0x00AA,0x00,0x09},
{0x019B,0x0011,0x0168,0x005F,0x0168,0x003E,0x00C1,0x011C,0x00,0x0D},
{0x01A3,0x00DC,0x0155,0x013E,0x0155,0x0041,0x013C,0x00D0,0x00,0x0C},
{0x01CF,0x019A,0x0148,0x0203,0x0161,0x003F,0x01B5,0x0122,0x08,0x0F},
{0x01A2,0x0184,0x00ED,0x01E5,0x00EC,0x003F,0x01B0,0x00C4,0x08,0x08},
{0x01A1,0x0196,0x009D,0x01FC,0x00A6,0x003D,0x01A8,0x0062,0x08,0x07},
{0x0197,0x019B,0x001F,0x01E8,0x0029,0x0040,0x015A,0x0028,0x08,0x0B},
{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00,0x00} };

#pragma pack (1)
typedef struct {//lenght 18
	uint8_t byte_0;
	uint8_t stub[17];
}
type_BYTE_E25ED_0x;
#pragma pack (16)

type_BYTE_E25ED_0x str_BYTE_E25ED_0x[0x9] = {//gold selection menu
{0x06,{0xA0,0x01,0x13,0x00,0x74,0x00,0xAC,0x00,0x7C,0x00,0x01,0x00,0x19,0x01,0x7C,0x00,0x02}},
{0x0A,{0x96,0x01,0x11,0x00,0xF6,0x00,0x5F,0x00,0xF6,0x00,0x3E,0x00,0xC0,0x00,0xAA,0x00,0x00}},
{0x09,{0x9B,0x01,0x11,0x00,0x68,0x01,0x5F,0x00,0x68,0x01,0x3E,0x00,0xC1,0x00,0x1C,0x01,0x00}},
{0x0D,{0xA3,0x01,0xDC,0x00,0x55,0x01,0x3E,0x01,0x55,0x01,0x41,0x00,0x3C,0x01,0xD0,0x00,0x00}},
{0x0C,{0xCF,0x01,0x9A,0x01,0x48,0x01,0x03,0x02,0x61,0x01,0x3F,0x00,0xB5,0x01,0x22,0x01,0x08}},
{0x0F,{0xA2,0x01,0x84,0x01,0xED,0x00,0xE5,0x01,0xEC,0x00,0x3F,0x00,0xB0,0x01,0xC4,0x00,0x08}},
{0x08,{0xA1,0x01,0x96,0x01,0x9D,0x00,0xFC,0x01,0xA6,0x00,0x3D,0x00,0xA8,0x01,0x62,0x00,0x08}},
{0x07,{0x97,0x01,0x9B,0x01,0x1F,0x00,0xE8,0x01,0x29,0x00,0x40,0x00,0x5A,0x01,0x28,0x00,0x08}},
{0x0B,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} };

type_WORD_E1F84 str_E1BAC_0x1b8[7] = {//0x1b8
{0x0025DCF0,0x00000000,0x0000,0x011C,0x0002,0x001E,0x000A,0x0000,0x0B,0x04,0x00,0x01,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003}},
{0x0025DD30,0x00000000,0x0000,0x011A,0x0022,0x001E,0x000A,0x0000,0x0C,0x05,0x00,0x01,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003}},
{0x0025DCA0,0x00000000,0x0000,0x006F,0x0065,0x0028,0x0028,0x0000,0x08,0x01,0x00,0x01,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003}},
{0x0025DDA0,0x00000000,0x0000,0x01C1,0x0064,0x0028,0x0028,0x0000,0x09,0x02,0x00,0x01,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003}},
{0x0025DBF0,0x00000000,0x0000,0x009A,0x0103,0x0050,0x0096,0x0000,0x0D,0x06,0x00,0x01,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003}},
{0x0025DC40,0x00000000,0x0000,0x018F,0x0102,0x0050,0x0096,0x0000,0x0E,0x07,0x00,0x01,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003}},
{0x00000000,0x00000000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00,0x00,0x00,0x00,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003}}};


std::array<type_MainMenuAnimations_E1748, 4> MainMenuAnimations_E1748x {{ // menu fire animation positions and sprites
	{0,17, 159, 1, 1,8},   // left fire
	{0,531,156, 9, 9,16},  // right fire
	{0,154,308, 17,17,25}, // left incense stick
	{0,482,308, 26,26,34}, // right incense stick
}};
//unk_E1784x[3] // removed - referenced from menu animations but never in any code path


#pragma pack (1)
struct
{
	uint32_t unk_17DBA8; // weak 40
	uint32_t unk_17DBAC; //1(+4)
	uint32_t unk_17DBB0; //2(+8)
	uint16_t unk_17DBB4; //3(+12)
	uint8_t x_BYTE_17DBB5; //(+14)
	uint8_t x_BYTE_17DBB6; //(+15)
} unk_17DBA8str;//16
#pragma pack (16)

#pragma pack (1)
struct//lenght 13
{
	int32_t x_DWORD_17DE28; // weak x_DWORD_17DE28str
	int32_t x_DWORD_17DE2C; // weak x_DWORD_17DE28str+4
	int16_t x_WORD_17DE30_posx; // weak x_DWORD_17DE28str+8
	int16_t x_WORD_17DE32_posy; // weak x_DWORD_17DE28str+10
	int8_t x_BYTE_17DE34; // weak x_DWORD_17DE28str+12
} x_DWORD_17DE28str;
#pragma pack (16)


char x_BYTE_E29DE = 1; // weak
char x_BYTE_E29E1 = 1; // weak

int16_t x_WORD_17DE26; // weak


long unknown_libname_2_findfirst(char* path, uint16_t  /*a2*/, _finddata_t* c_file) {//findfirst
	long hFile;
	//char path2[2048] = "\0";
	//pathfix(path, path2);//only for DOSBOX version

	//struct _finddata_t c_file;
	if ((hFile = my_findfirst(path, c_file)) == -1L)
		return(-1);//file not found
	//strcmp(filename, c_file.name);
	return(hFile);
	/*
if ((hFile = _findfirst(path2, &c_file)) == -1L)
	printf("file not found");
else
{
	do
	{
		printf("%s\n", c_file.name);
	} while (_findnext(hFile, &c_file) == 0);
		_findclose(hFile);
}
return 0;*/
/*char* v3; // eax //language/l.txt

//ecx dfc628 c3cd34c3cd35c3cd36
//eax //language/l.txt
//edx 78651a 0b0005a1a1a1a10005a1b413aaa100
//mov edx,ebx

int result; // eax

v3 = a1;
//__asm { int     21h; DOS - SET DISK TRANSFER AREA ADDRESS }// fix it
BYTE1(v3) = 78;
//__asm { int     21h; DOS - 2 + -FIND FIRST ASCIZ(FINDFIRST) }// fix it
result = 0;//_doserror(v3, a1); // fix it
sub_9A1B6(result, (void*)a3, (void*)a3);
return result;*/
};// weak

long unknown_libname_3_findnext(_finddata_t* c_file, long hFile) {//258193
	//char path[100];//fix
	//char filename[100];//fix it

	/*char path2[2048] = "\0";
	pathfix(path, path2);//only for DOSBOX version
	*/
	//struct _finddata_t c_file;
	//long hFile;
	if ((hFile = my_findnext(hFile, c_file)) == -1L)
	{
		return(-1);//file not found
	}
	//strcmp(filename, c_file.name);
	//_findclose(hFile);
	return(hFile);
	/*

	if ((hFile = _findfirst(path2, &c_file)) == -1L)
	printf("file not found");
	else
	{
	do
	{
	printf("%s\n", c_file.name);
	} while (_findnext(hFile, &c_file) == 0);
	_findclose(hFile);
	}

	*/
	/*int v2; // eax
	int v3; // eax
	int v4; // edx
	int result; // eax
	void *v6; // edx

	v2 = a2;
	BYTE1(v2) = 26;
	__asm { int     21h; DOS - SET DISK TRANSFER AREA ADDRESS }
	v3 = sub_9A1D9(v2, a2);
	BYTE1(v3) = 79;
	__asm { int     21h; DOS - 2 + -FIND NEXT ASCIZ(FINDNEXT) }
	result = _doserror(v3, v4);
	sub_9A1B6(result, v6, a1);
	return result;*/
	return 0;
};// weak

int unknown_libname_4_find_close(struct _finddata_t*  /*c_file*/, long hFile) {//27b1b3
	my_findclose(hFile);
	return 0;
};// weak

void /*__fastcall*/ _wcpp_1_unwind_leave__131() {//257d00
	stub_fix_it();
}

//----- (0002EB40) --------------------------------------------------------
void sub_2EB40()//20fb40
{
	//int result; // eax

	//BYTE1(result) = 0;
	x_BYTE_D41CE = 0;
	x_BYTE_D41C1 = 0;
	x_DWORD_D41D0 = 0;
	//return result;
}
// D41C1: using guessed type char x_BYTE_D41C1;
// D41CE: using guessed type char x_BYTE_D41CE;
// D41D0: using guessed type int x_DWORD_D41D0;


//----- (00076930) --------------------------------------------------------
void MenusAndIntros_76930(int  /*a2*/, uint16_t a3)//257930
{
	//int v3; // eax
	int v4; // edx

	//fix it
	v4 = 0;
	//fix it

	//x_BYTE_E29DE = x_WORD_180660_VGA_type_resolution;//1 -351660
	x_BYTE_E29DF_skip_screen = x_BYTE_D41AD_skip_screen;
	x_WORD_E29DC = 0;
	if (x_BYTE_D41AD_skip_screen == 1 || x_WORD_E29D8)
	{
		sub_82670();
		LoadAndSetGraphicsAndPalette_7AC00();
	}
	if (x_BYTE_D41AD_skip_screen == 1)
	{
		InitLanguage_76A40();
		x_WORD_E29D8 = 4;
	}
	memset(&x_DWORD_17DE38str, 0, sizeof(type_x_DWORD_17DE38str));
	x_DWORD_17DE38str.x_DWORD_17DEE0_filedesc = NULL;
	sub_7BEC0();//25CEC0 // fix this structure
	SetCenterScreenForFlyAssistant_6EDB0();//24FDB0
	/*v3 = */WriteConfigDat_81DB0();//262DB0

	//test_x_D41A0_BYTEARRAY_0();

	do
	{
		//x_WORD_E29D8 - prvni pruchod -0
		//v3 = x_WORD_E29D8;//2b39d8
		switch (x_WORD_E29D8)
		{
		case 0:
			/*v3 = */InitLanguage_76A40();//257A40 //asi inicializace + rovnou i nastaveni jazyka
			break;
		case 1:
			sub_76CF0();//257cf0 nastavi x_WORD_E29D8 na 3
			break;
		case 2:
			/*v3 = */_wcpp_1_unwind_leave__131(/*v3*/);//257d00 asi konec
			break;
		case 3:
			Intros_76D10(0);//257d10 intro
			x_WORD_E29D8 = 4;
			break;
		case 4:
			MainMenu_76FA0(v4, a3);//257fa0 main menu loop
			break;
		case 5:
			x_WORD_E29DC = 1;
			break;
		case 12:
			LanguageSettingDialog_779E0(0);//2589e0 asi herni smycka
			break;
		default:
			break;
		}
	} while (!x_WORD_E29DC);
	sub_7ADE0(x_BYTE_E29DE);//zase nejaka inicializace
	if (x_BYTE_E29E1)
		x_BYTE_E29E1 = 0;
	WriteConfigDat_81DB0();//neco
}
// 76D00: using guessed type int /*__fastcall*/ _wcpp_1_unwind_leave__131(x_DWORD);
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// D41AD: using guessed type char x_BYTE_D41AD_skip_screen;
// E29D8: using guessed type __int16 x_WORD_E29D8;
// E29DC: using guessed type __int16 x_WORD_E29DC;
// E29DE: using guessed type char x_BYTE_E29DE;
// E29E1: using guessed type char x_BYTE_E29E1;
// 17DE38: using guessed type int x_DWORD_17DE38;
// 17DEE0: using guessed type int x_DWORD_17DEE0_filedesc;
// 180660: using guessed type __int16 x_WORD_180660_VGA_type_resolution;

//----- (00076A40) --------------------------------------------------------
void InitLanguage_76A40()//257A40
{
	FILE* configdatfile;
	FILE* langfile;
	uint32_t filelenght;
	TypeConfigDat configDat;

	memset(printbuffer, 0, 80);
	memset(&x_DWORD_17DE38str, 0, sizeof(type_x_DWORD_17DE38str));
	x_DWORD_17DE38str.x_DWORD_17DEE0_filedesc = NULL;
	memset(&configDat, 0, sizeof(TypeConfigDat));
	sprintf(printbuffer, "%s/%s", gameDataPath.c_str(), "CONFIG.DAT");
	configdatfile = DataFileIO::CreateOrOpenFile(printbuffer, 512);
	if (configdatfile == NULL)//config is not found
	{
		LoadAndSetGraphicsAndPalette_7AC00();//25BC00 //change screen radio
		LanguageSettingDialog_779E0(0);//2589E0 //set language
		sub_7ADE0(1);
	}
	else
	{
		DataFileIO::Read(configdatfile, (uint8_t*)&configDat.configDatSign_0, 4);//first dword
		if (configDat.configDatSign_0 == 0xfffffff7)
		{
			DataFileIO::Read(configdatfile, (uint8_t*)&configDat.langIndex_4, 28);

			x_D41A0_BYTEARRAY_4_struct.configDatSign_0 = configDat.configDatSign_0;
			x_D41A0_BYTEARRAY_4_struct.langIndex_4 = configDat.langIndex_4;
			x_D41A0_BYTEARRAY_4_struct.soundVolume_6 = configDat.soundVolume_6;
			x_D41A0_BYTEARRAY_4_struct.musicVolume_8 = configDat.musicVolume_8;
			x_D41A0_BYTEARRAY_4_struct.byteindex_10 = configDat.byteindex_10;
			x_D41A0_BYTEARRAY_4_struct.brightness_11 = configDat.brightness_11;
			x_D41A0_BYTEARRAY_4_struct.brightness_12 = configDat.brightness_12;
			x_D41A0_BYTEARRAY_4_struct.brightness_13 = configDat.brightness_13;
			x_D41A0_BYTEARRAY_4_struct.wordindex_14 = configDat.wordindex_14;
			x_D41A0_BYTEARRAY_4_struct.dwordindex_16 = configDat.dwordindex_16;
			x_D41A0_BYTEARRAY_4_struct.stubb[0] = configDat.stubb[0];
			x_D41A0_BYTEARRAY_4_struct.stubb[1] = configDat.stubb[1];

			for (int i = 0; i < 10; i++)
				x_BYTE_EB39E_keys[i] = configDat.keys[i];

			DataFileIO::Close(configdatfile);
			sub_8E470_sound_proc17_volume(x_D41A0_BYTEARRAY_4_struct.soundVolume_6);
			sub_8E410_sound_proc16_xmidivolume(x_D41A0_BYTEARRAY_4_struct.musicVolume_8);

			sprintf(printbuffer, "%s/%s/L%d.TXT", cdDataPath.c_str(), "LANGUAGE", x_D41A0_BYTEARRAY_4_struct.langIndex_4);
			for (int i = 0; i < 2; i++)
			{
				x_D41A0_BYTEARRAY_4_struct.SelectedLangIndex = x_D41A0_BYTEARRAY_4_struct.langIndex_4 & 0xff;
				if (x_BYTE_E29E0 || x_DWORD_D41BC_langbuffer)
					FreeMem_83E80((uint8_t*)x_DWORD_D41BC_langbuffer);
				langfile = DataFileIO::CreateOrOpenFile(printbuffer, 512);

				if (langfile != NULL)
				{
					filelenght = DataFileIO::FileLengthBytes(langfile) - 4785;
					x_DWORD_D41BC_langbuffer = (char*)sub_83CD0_malloc2(filelenght);
					if (x_DWORD_D41BC_langbuffer)
					{
						DataFileIO::Seek(langfile, 4785, 0);
						DataFileIO::Read(langfile, (uint8_t*)x_DWORD_D41BC_langbuffer, filelenght);
					}
					DataFileIO::Close(langfile);
					sub_5B870_copy_sentence(x_DWORD_D41BC_langbuffer, x_DWORD_E9C4C_langindexbuffer, 471);//Exit Game
					break;
				}
				sprintf(printbuffer, "%s/%s/L%d.TXT", cdDataPath.c_str(), "LANGUAGE", x_D41A0_BYTEARRAY_4_struct.SelectedLangIndex);
			}
		}
		else
		{
			DataFileIO::Close(configdatfile);
			LoadAndSetGraphicsAndPalette_7AC00();
			LanguageSettingDialog_779E0(0);
			sub_7ADE0(1);
		}
	}
	if (x_D41A0_BYTEARRAY_4_struct.SelectedLangIndex != 2 || !soundActive2_E3798)
		x_BYTE_D41C0 = 1;
	x_WORD_E29D8 = 1;
}

//----- (00076CF0) --------------------------------------------------------
void sub_76CF0()
{
	x_WORD_E29D8 = 3;
}
// E29D8: using guessed type __int16 x_WORD_E29D8;

//----- (00076D10) --------------------------------------------------------
void Intros_76D10(char a1)//257d10
{
	//int v1; // eax
	//signed int v2; // eax
	//signed int v3; // eax
	char dataPath[MAX_PATH];

	//x_DWORD_17DE48c = x_D41A0_BYTEARRAY_4_struct.pointer_0xE2_heapbuffer_226;
	x_DWORD_17DE38str.x_DWORD_17DE54 = &x_D41A0_BYTEARRAY_4_struct.pointer_0xE2_heapbuffer_226[0x49ADB];// 301787;
	x_DWORD_17DE38str.x_DWORD_17DEC0 = (posistruct2_t*)&x_D41A0_BYTEARRAY_4_struct.pointer_0xE2_heapbuffer_226[0x4B52F];//308527;
	x_DWORD_17DE38str.x_DWORD_17DEC4 = (posistruct2_t*)&x_D41A0_BYTEARRAY_4_struct.pointer_0xE2_heapbuffer_226[0x4BB8F];

	sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/SCREENS/HSCREEN0.DAT");
	sub_7AA70_load_and_decompres_dat_file(dataPath, &x_D41A0_BYTEARRAY_4_struct.pointer_0xE2_heapbuffer_226[0x49ADB], 0x164FCD, 0x35C);
	sub_7AA70_load_and_decompres_dat_file(dataPath, (uint8_t*)x_DWORD_17DE38str.x_DWORD_17DEC0, 0x165329, 0x224);
	sub_7AA70_load_and_decompres_dat_file(0, 0, 0, 0);
	if (x_WORD_180660_VGA_type_resolution & 1)
		sub_98709_create_index_dattab_power(x_DWORD_17DE38str.x_DWORD_17DEC0, x_DWORD_17DE38str.x_DWORD_17DEC4, x_DWORD_17DE38str.x_DWORD_17DE54, xy_DWORD_17DEC0_spritestr);
	else
		sub_9874D_create_index_dattab(x_DWORD_17DE38str.x_DWORD_17DEC0, x_DWORD_17DE38str.x_DWORD_17DEC4, x_DWORD_17DE38str.x_DWORD_17DE54, xy_DWORD_17DEC0_spritestr);

	sub_2EB40();
	if (soundActive2_E3798 && x_D41A0_BYTEARRAY_4_struct.SelectedLangIndex == 2)
	{
		x_BYTE_D41C1 = 0;
		x_BYTE_D41C0 = 0;
	}
	else
	{
		x_BYTE_D41C0 = 1;
		x_BYTE_D41C1 = 1;
	}
	sub_8CD27_set_cursor((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]);
	char introPath[MAX_PATH];
	sprintf(introPath, "%s/%s", cdDataPath.c_str(), "INTRO/INTRO.DAT");
	switch (a1)
	{
	case 0:
#ifndef debug_hide_graphics
		ShowWelcomeScreen_83850();//frog logo and wait
#endif
		PlayInfoFmv(1, 1, str_E17CC_0, introPath);//257160 intro .. 2b27cc
		/*v1 = */sub_2EB40();
		x_BYTE_D41C1 = 0;
		x_BYTE_D41C0 = 0;
		while (sub_9A10A_check_keyboard(/*v1*/))
		{
			LastPressedKey_1806E4 = 0;
			/*v1 = */sub_7A060_get_mouse_and_keyboard_events();
		}
		j___delay(50);
		sprintf(introPath, "%s/%s", cdDataPath.c_str(), "INTRO/INTRO2.DAT");
		goto LABEL_17;
	case 1:
		PlayInfoFmv(1, 1, str_E17CC_0, introPath);
		break;
	case 2:
		LastPressedKey_1806E4 = 0;
		x_BYTE_D41C1 = 0;
		x_BYTE_D41C0 = 0;
		sprintf(introPath, "%s/%s", cdDataPath.c_str(), "INTRO/INTRO2.DAT");
	LABEL_17:
		PlayInfoFmv(1, 1, str_E17CC_0x160, introPath);//E192C
		break;
	}
	sub_90B27_VGA_pal_fadein_fadeout(0, 0x10u, 0);
	sub_8D8F0_sound_proc3_endsample();
	StopMusic_8E020();//?ac_sound_stop_music
	sub_7B5D0();
	//v2 = 0;
	x_WORD_E29D8 = 4;
	x_BYTE_D41C0 = 0;
	x_BYTE_D41C1 = 0;
	//v3 = sub_9A10A_check_keyboard(/*v2*/);
	if (sub_9A10A_check_keyboard(/*v2*/))
		/*v3 = */sub_7A060_get_mouse_and_keyboard_events();
	x_DWORD_17DE38str.x_BYTE_17DF11_last_key_status = 0;
	x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode = 0;
	if (!a1)
		/*v3 = */LoadAndSetGraphicsAndPalette_7AC00();
	//return v3;
}
// 8E3D5: using guessed type x_DWORD sprintf(x_DWORD, const char *, ...);
// 9A122: using guessed type x_DWORD j___delay(x_DWORD);
// D41A4: using guessed type int x_DWORD_D41A4;
// D41C0: using guessed type char x_BYTE_D41C0;
// D41C1: using guessed type char x_BYTE_D41C1;
// E29D8: using guessed type __int16 x_WORD_E29D8;
// E3798: using guessed type char x_BYTE_E3798_sound_active2;
// EB394: using guessed type int **filearray_2aa18c[0];
// 17DE48: using guessed type int x_DWORD_17DE48;
// 17DE54: using guessed type int (int)x_DWORD_17DE54;
// 17DEC0: using guessed type int (int)x_DWORD_17DEC0;
// 17DEC4: using guessed type int (int)x_DWORD_17DEC4;
// 17DF10: using guessed type char x_BYTE_17DF10_get_key_scancode;
// 17DF11: using guessed type char x_BYTE_17DF11_last_key_status;
// 180660: using guessed type __int16 x_WORD_180660_VGA_type_resolution;
// 1806E4: using guessed type char x_BYTE_1806E4;

//----- (00076FA0) --------------------------------------------------------
void MainMenu_76FA0(/*int a1, */int  /*a2*/, uint16_t a3x)//257fa0
{
	//void (*v3)(); // eax
	__int16 v4; // dx
	//int result; // eax
	//int v6; // eax
	//int v7; // edx
	//int v8; // edx
	//int v9; // edx
	unsigned __int16 v10; // di
	//int v11; // eax
	unsigned __int16 v12; // si
	int v13; // eax
	//int v14; // edx
	uint8_t* v15; // esi
	//int v16; // edx
	//int v17; // eax
	char v18; // dh
	//__int16 v19; // ax
	//int v20; // edx
	//uint8_t* v21; // eax
	//int v22; // edx
	//int v23; // edx
	//__int16 v24; // [esp+0h] [ebp-24h]
	//__int16 v25; // [esp+Ch] [ebp-18h]
	int v26; // [esp+1Ch] [ebp-8h]
	int v27; // [esp+20h] [ebp-4h]

	//int16_t a3t;

	//fix it
	v4 = 0;
	//fix it

	//fix it
	//v7 = 0;
	//v8 = 0;
	//v9 = 0;
	//v16 = 0;
	//v20 = 0;
	//v22 = 0;
	//v23 = 0;
	//fix it

	//fixed
	//x_WORD_180660_VGA_type_resolution = 8;
	//sub_6EB90(posistruct7);
	//sub_6EBF0(&filearray_2aa18c[filearrayindex_HFONT3DATTAB]);
	help_VGA_type_resolution = 0;
	//fixed

	//memset(pdwScreenBuffer_351628, 0, 640*480);//fixed

	/*memset(&v24, 0, 28);
	v24 = 0x3301;
	v25 = 0;
	int386(0x21, (REGS*)&v24, (REGS*)&v24);//get set ctrl break*/
	sub_86860_speak_Sound(x_WORD_1803EC);//267860
	sub_84300_load_sound(0);//265300
	memset(&x_DWORD_17DBB8, 0, 16);
	x_BYTE_17DBC6 = 2;
	x_DWORD_17DE38str.x_WORD_17DF04 = -1;
	x_DWORD_17DE38str.x_DWORD_17DE44 = x_DWORD_E9C38_smalltit;
	SetCenterScreenForFlyAssistant_6EDB0();
	StopMusic_8E020();//26f020
	StartMusic_8E160(4, 0x7Fu);//26f160
	/*sub_75420();//256420
	v3 = dos_getvect(9);*/
	a3x = 256;
	x_WORD_17DE26 = v4;
	//x_DWORD_17DE22 = v3;
	VGA_cleanKeyBuffer();
	if (x_BYTE_E29E1 || x_D41A0_BYTEARRAY_4_struct.setting_byte1_22 & 0x10 || (NewGameDialog_77350(0), !x_WORD_E29DC))
	{
		x_D41A0_BYTEARRAY_4_struct.setting_byte1_22 &= 0xEFu;
		sub_7A110_load_hscreen(x_WORD_180660_VGA_type_resolution, 4);
		ResetMouse_7B5A0();

		//fix
		//write_pngs();//only for export
		//fix

		sub_8CD27_set_cursor(xy_DWORD_17DED4_spritestr[39]);
		//v6 = x_D41A0_BYTEARRAY_4;
		x_DWORD_17DE38str.x_BYTE_17DF13 = x_D41A0_BYTEARRAY_4_struct.byteindex_10;
		//v6 = x_DWORD_17DE38str.x_BYTE_17DF13;
		x_DWORD_17DBB8[0] = j___clock();
		v26 = j___clock();
		v10 = x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx;
		//v11 = x_BYTE_17DF10_get_key_scancode;
		v12 = x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony;
		v27 = x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode;
		while (!x_WORD_E29DC)
		{
			v13 = j___clock();
			if ((v10 == x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx) && (v12 == x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony) && (x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode == v27))
			{
				if ((v13 - v26) / 0x64u > 0x3C)//after any time run intro
				{
					v15 = x_DWORD_E9C38_smalltit;
					x_DWORD_E9C38_smalltit = x_DWORD_17DE38str.x_DWORD_17DE44;
					PlayIntros_83250(SBYTE1(a3x));
					x_DWORD_E9C38_smalltit = v15;
					BYTE1(a3x) = (BYTE1(a3x) == 1) + 1;
					v12 = x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony;
					v10 = x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx;
					x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode = 0;
					v27 = 0;
					v26 = j___clock();
					StopMusic_8E020();
					LOBYTE(a3x) = 0;
					StartMusic_8E160(4, 0x7Fu);
				}
			}
			else
			{
				v12 = x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony;
				v10 = x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx;
				v27 = x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode;
				v26 = j___clock();
			}
			if (x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode == 59)
			{
				//v17 = x_D41A0_BYTEARRAY_4;
				v18 = x_D41A0_BYTEARRAY_4_struct.byteindex_10;
				x_D41A0_BYTEARRAY_4_struct.byteindex_10 = v18 != 1;
				x_DWORD_17DE38str.x_BYTE_17DF13 = v18 != 1;
				//*(x_BYTE *)(v17 + 38402) = 1;
				x_D41A0_BYTEARRAY_4_struct.setting_38402 = 1;
			}
			if (x_WORD_180660_VGA_type_resolution & 1)
				CopyScreen(x_DWORD_E9C38_smalltit, pdwScreenBuffer_351628, 320, 200);
			else
				CopyScreen(x_DWORD_E9C38_smalltit, pdwScreenBuffer_351628, 640, 480);

			/*v19 = */sub_7C120_draw_bitmap_640(185, 232, xy_DWORD_17DED4_spritestr[66]);//adress 25827a
			//ax,ebx,a3
			//6038,100,4?
			DrawMenuAnimations_7AB00();//25bb00
			if (DrawAndServe_7B250(/*(int)v21, v22*/))//25c250
			{
				v12 = x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony;
				v10 = x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx;
				LOBYTE(a3x) = 0;
				v27 = x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode;
				v26 = j___clock();
			}
			if (LOBYTE(a3x))
			{
				if (x_WORD_180660_VGA_type_resolution & 1)
					sub_90478_VGA_Blit320();
				else
					sub_75200_VGA_Blit640(480);
			}
			else
			{
				LOBYTE(a3x) = 1;
				//34ee38 20 0
				sub_90B27_VGA_pal_fadein_fadeout(x_DWORD_17DE38str.x_DWORD_17DE38x, 0x20u, 0);//tady
			}
			/*v11 = */sub_7A060_get_mouse_and_keyboard_events();

			//test_x_D41A0_BYTEARRAY_0();
		}
		sub_41BC0();

		//x_D41A0_BYTEARRAY_0[8592] = 0;
		D41A0_0.m_GameSettings.m_Display.m_uiScreenSize = 0;

		//dos_setvect(9, x_DWORD_17DE22, x_WORD_17DE26);
		sub_753D0();
		//result = (int)x_DWORD_17DE44;
		x_DWORD_E9C38_smalltit = x_DWORD_17DE38str.x_DWORD_17DE44;
	}
	else
	{
		sub_41BC0();

		//x_D41A0_BYTEARRAY_0[8592] = 0;
		D41A0_0.m_GameSettings.m_Display.m_uiScreenSize = 0;

		//dos_setvect(9, x_DWORD_17DE22, x_WORD_17DE26);
		/*result = */sub_753D0();
	}
	//  return result;
}

//----- (00077350) --------------------------------------------------------
bool NewGameDialog_77350(type_WORD_E1F84* a1x)//258350
{
	unsigned __int16 v1; // bx
	bool result=false; // al

	v1 = 0;
	x_WORD_E29D6_not_movex = 0;
	if (D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dw_w_b_0_2BDE_11230.byte[2] & 2
		&& x_D41A0_BYTEARRAY_4_struct.levelnumber_43w == 24)
		sub_833C0();
	memset((void*)&unk_17DBA8str, 0, 16);
	unk_17DBA8str.x_BYTE_17DBB6 = 2;
	if (x_BYTE_D419C_level_num <= -1)
	{
		x_BYTE_17E09D = 0;
		x_DWORD_17DE38str.unk_17E078x[0xb] = 16;
		x_DWORD_17DE38str.x_WORD_17E06E = 480;
		*(int16_t*)&x_DWORD_17DE38str.unk_17E078x[8] = 13;
		x_DWORD_17DE38str.x_BYTE_17E09C = 0;
		x_DWORD_17DE38str.unk_17E078x[0xa] = 13;
		x_DWORD_17DE38str.x_WORD_17E06C = 0;
		memset((void*)&x_DWORD_17DB70str, 0, 56);
		if (D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dw_w_b_0_2BDE_11230.byte[2] & 2)
			x_DWORD_17DB70str.x_BYTE_17DB8F = 4;
		else
			x_DWORD_17DB70str.x_BYTE_17DB8F = 1;
		sub_7A110_load_hscreen(x_WORD_180660_VGA_type_resolution, 6);//here init sprites
		sub_8CD27_set_cursor((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]);//dword_EB394

		ResetMouse_7B5A0();
		sub_90B27_VGA_pal_fadein_fadeout(0, 0x10u, 0);
		if (x_WORD_180660_VGA_type_resolution & 1)
		{
			ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, getPaletteIndex_5BE80(x_DWORD_17DE38str.x_DWORD_17DE38x, 0, 0, 0));
		}
		else
		{
			ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 640, 480, getPaletteIndex_5BE80(x_DWORD_17DE38str.x_DWORD_17DE38x, 0, 0, 0));
		}
		if (x_WORD_180660_VGA_type_resolution & 1)
			sub_90478_VGA_Blit320();
		else
			sub_75200_VGA_Blit640(480);
		sub_41A90_VGA_Palette_install(x_DWORD_17DE38str.x_DWORD_17DE38x);
		sub_7DD70();
		x_DWORD_17DE38str.x_WORD_17DEEC = 0;
		SetCenterScreenForFlyAssistant_6EDB0();
		sub_8CD27_set_cursor(xy_DWORD_17DED4_spritestr[239]);
		x_DWORD_17DB70str.x_WORD_17DB8A = x_D41A0_BYTEARRAY_4_struct.levelnumber_43w;
		while (!v1)
		{
			if (x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode == 59)
			{
				x_DWORD_17DE38str.x_BYTE_17DF13 = x_D41A0_BYTEARRAY_4_struct.byteindex_10 != 1;
				x_D41A0_BYTEARRAY_4_struct.byteindex_10 = x_D41A0_BYTEARRAY_4_struct.byteindex_10 != 1;
				x_D41A0_BYTEARRAY_4_struct.setting_38402 = 1;
			}
			v1 = NewGameDraw_7EAE0(
				&x_DWORD_17DB70str.unk_17DB76_posx,
				&x_DWORD_17DB70str.unk_17DB78_posy,
				&x_DWORD_17DB70str.unk_17DB7E,
				&x_DWORD_17DB70str.unk_17DB80,
				&x_DWORD_17DB70str.x_BYTE_17DB8F,
				&x_DWORD_17DB70str.unk_17DB90);
			if (CommandLineParams.ModeTestRegressionsGame()) {
				x_DWORD_17DB70str.x_BYTE_17DB8E = 1;
				x_D41A0_BYTEARRAY_4_struct.levelnumber_43w = test_regression_level;
				if (mapScreenPortals_E17CC[test_regression_level].activated_18 == 1)
					x_D41A0_BYTEARRAY_4_struct.setting_38545 |= 4u;
				Type_SecretMapScreenPortals_E2970* v46x = sub_824B0(x_D41A0_BYTEARRAY_4_struct.levelnumber_43w);
				if (v46x && v46x->activated_12 == 2)
					x_D41A0_BYTEARRAY_4_struct.setting_38545 |= 0x10u;
				if (x_D41A0_BYTEARRAY_4_struct.levelnumber_43w == 24)
					x_D41A0_BYTEARRAY_4_struct.setting_38545 |= 0x20u;
				v1 = 1;
			}
			if (x_WORD_180660_VGA_type_resolution & 1)
				sub_90478_VGA_Blit320();
			else
				sub_75200_VGA_Blit640(480);
			sub_7A060_get_mouse_and_keyboard_events();
		}
		sub_86860_speak_Sound(x_WORD_1803EC);
		D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dw_w_b_0_2BDE_11230.byte[2] = 0;
		sub_8D8F0_sound_proc3_endsample();
		x_WORD_E29D6_not_movex = 0;
		if (v1 >= 1u)
		{
			if (v1 <= 1u)
			{
				x_WORD_E29DC = 1;
				if (a1x)
					a1x->dword_4 = 0;
			}
			else if (v1 == 2 && a1x)
			{
				a1x->dword_4 = 1;
			}
		}
		sub_8CD27_set_cursor((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]);
		sub_90B27_VGA_pal_fadein_fadeout(0, 0x10u, 0);
		result = true;
	}
	else
	{
		x_WORD_E29DC = 1;
		x_D41A0_BYTEARRAY_4_struct.levelnumber_43w = x_BYTE_D419C_level_num;
		if (a1x)
			a1x->dword_4 = 0;
		result = true;
	}
	return result;
}

//----- (0007E0E0) --------------------------------------------------------
signed int sub_7E0E0_mouse_events()//25f0e0
{
	int v0; // esi
	//x_WORD *v2; // ebx
	int v3; // edx

	v0 = 0;
	if (x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode == 1)
		return 2;
	//v2 = x_WORD_E1F84;
	//uint32_t i = 0;
	int ix = 0;
	if (!(str_WORD_E1F84[0].xmin_10 + (str_WORD_E1F84[0].ymin_12 << 16)))
		return 0;
	do
	{
		if (!(x_DWORD_17DE38str.x_WORD_17DEEE_mouse_buttons & 1) || v0)
		{//[34eee6] [34eee4] 2b2fb0
			if (str_WORD_E1F84[ix].gold_color_24 && InRegion_7B200(&str_WORD_E1F84[ix], x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx, x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony))//confirm language
				v3 = str_WORD_E1F84[ix].byte_20;
			else
				v3 = str_WORD_E1F84[ix].byte_21;
			/*
			[34eed4]+69+48c73d
			cd
			123
			*/
			//x_D41A0_BYTEARRAY_4_struct.pointer_0xE2_heapbuffer_226[0x4D54A + 164829 - 1]
			//je asi &x_DWORD_17DED4[6 * v3]
			//123 cd
			sub_2BB40_draw_bitmap(str_WORD_E1F84[ix].xmin_10, str_WORD_E1F84[ix].ymin_12, xy_DWORD_17DED4_spritestr[v3]);
		}
		else if (InRegion_7B200(&str_WORD_E1F84[ix], x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx, x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony))//change language
		{
			sub_8F100_sound_proc19(0, 14, 127, 64, 0x64u, 0, 3u);
			sub_2BB40_draw_bitmap(str_WORD_E1F84[ix].xmin_10, str_WORD_E1F84[ix].ymin_12, xy_DWORD_17DED4_spritestr[str_WORD_E1F84[ix].xmin_10 & 0xff]);//asi vykresleni stisknuteho tlacitka
			v0 = str_WORD_E1F84[ix].byte_22;
		}
		//i += 22;
		ix++;
	} while (str_WORD_E1F84[ix].xmin_10);
	return v0;
}
// 17DED4: using guessed type int (int)x_DWORD_17DED4;
// 17DEE4: using guessed type int x_DWORD_17DEE4_mouse_position;
// 17DEEE: using guessed type __int16 x_WORD_17DEEE_mouse_buttons;
// 17DF10: using guessed type char x_BYTE_17DF10_get_key_scancode;

//----- (0007E1F0) --------------------------------------------------------
int16_t TestMouseRegions_7E1F0()//25f1f0
{
	//type_WORD_E1F84* v0x; // ebx
	int v0y;
	int16_t v1; // esi
	//int v3; // ecx
	//int v4; // edx

	//v0 = x_WORD_E2008;
	v0y = 0;
	v1 = 0;
	if (!str_WORD_E2008[0].xmin_10)
		return 0;
	do
	{
		//v3 = xy_DWORD_17DED4_spritestr[0].pointer;
		//v4 = v0[21];
		str_WORD_E2008[v0y].sizex_14 = xy_DWORD_17DED4_spritestr[str_WORD_E2008[v0y].byte_21].width_4;
		str_WORD_E2008[v0y].sizey_16 = xy_DWORD_17DED4_spritestr[str_WORD_E2008[v0y].byte_21].height_5;
		if (!(x_DWORD_17DE38str.x_WORD_17DEEE_mouse_buttons & 1))
		{
			if (InRegion_7B200(&str_WORD_E2008[v0y], x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx, x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony))
				sub_2BB40_draw_bitmap(str_WORD_E2008[v0y].xmin_10, str_WORD_E2008[v0y].ymin_12, xy_DWORD_17DED4_spritestr[str_WORD_E2008[v0y].byte_21]);
			goto LABEL_9;
		}
		if (InRegion_7B200(&str_WORD_E2008[v0y], x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx, x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony))
		{
			sub_8F100_sound_proc19(0, 14, 127, 64, 0x64u, 0, 3u);
			sub_2BB40_draw_bitmap(str_WORD_E2008[v0y].xmin_10, str_WORD_E2008[v0y].ymin_12, xy_DWORD_17DED4_spritestr[str_WORD_E2008[v0y].byte_21]);
			v1 = str_WORD_E2008[v0y].byte_22;
			if (str_WORD_E2008[v0y].byte_22)
				break;
		}
	LABEL_9:
		//v0 += 22;
		v0y++;
	} while (str_WORD_E2008[v0y].xmin_10);
	return v1;
}
// 17DED4: using guessed type int (int)x_DWORD_17DED4;
// 17DEE4: using guessed type int x_DWORD_17DEE4_mouse_position;
// 17DEEE: using guessed type __int16 x_WORD_17DEEE_mouse_buttons;

//----- (000779E0) --------------------------------------------------------
char LanguageSettingDialog_779E0(type_WORD_E1F84* a1y)//2589E0
{
	uint8_t* langBuffer;
	char* langFilename;
	FILE* configFile2;
	char selectLang3;
	FILE* configFile;
	_finddata_t langFileL;
	_finddata_t langFileD;
	type_E24BCx textBoxStr[2];
	int actualTime;
	posistruct2_t* tabBufferEnd;
	posistruct2_t* tabBuffer;

	uint8_t* tempSmalltit = NULL;

	int textIndex = 0;
	int oldTime = 0;

	TypeConfigDat configDat;

	uint8_t selectLang2 = 0;
	configDat.configDatSign_0 = 0;
	int frame = 0;
	configDat.langIndex_4 = 0;
	int mouseClick = 0;

	char configFilePath[MAX_PATH];
	sprintf(configFilePath, "%s/%s", gameDataPath.c_str(), "CONFIG.DAT");

	long langlhandle = 0;
	long langdhandle = 0;

	char codeBranch = 2;
	SetCenterScreenForFlyAssistant_6EDB0();//24fdb0
	x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx = 320;
	x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony = 200;
	if (a1y)//0x0
	{
		sub_7A110_load_hscreen(x_WORD_180660_VGA_type_resolution, 12);//load hscreen 25b110
		langBuffer = &x_DWORD_E9C38_smalltit[307200];//4B000
	}
	else
	{
		tempSmalltit = x_DWORD_E9C38_smalltit;
		x_DWORD_E9C38_smalltit = x_D41A0_BYTEARRAY_4_struct.pointer_0xE2_heapbuffer_226;//[[2a51a4]+e2]
		langBuffer = &x_DWORD_E9C38_smalltit[307200];//406514+4b000
		sub_7A110_load_hscreen(x_WORD_180660_VGA_type_resolution, 14);//25b110
	}
	ResetMouse_7B5A0();//25c5a0  disable //enabl
	sub_8CD27_set_cursor((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]/*filearray_2aa18c[0]*/);//26dd27
	x_DWORD_17DE38str.x_WORD_17DEEC = 0;

	char languagePathL[MAX_PATH];
	char languagePathD[MAX_PATH];
	sprintf(languagePathL, "%s/LANGUAGE/L*.TXT", cdDataPath.c_str());
	langlhandle = unknown_libname_2_findfirst(languagePathL, 0, &langFileL);
	if (langlhandle != 0)// 27B166 - 355088
	{
		sprintf(languagePathD, "%s/LANGUAGE/D*.TXT", cdDataPath.c_str());
		langdhandle = unknown_libname_2_findfirst(languagePathD, 0, &langFileD); //v21=3550b4 3550b4
		if (langlhandle == 0 || langdhandle == 0)
			langFilename = (char*)langFileL.name;//something was not found
		else
			langFilename = (char*)langFileD.name;//both are assigned the case "D2.TXT", address 3550d2

		posistruct_t langDatTab[1000];
		x_D41A0_BYTEARRAY_4_struct.SelectedLangIndex = LoadLanguageFile(&tabBuffer, &tabBufferEnd, langBuffer, langFilename, langDatTab);//2607d0
		unknown_libname_4_find_close(&langFileD, langdhandle);//27b1b3
		if (langlhandle == 0)
		{
			configFile2 = DataFileIO::CreateOrOpenFile(configFilePath, 512);
			if (configFile2 != NULL)
			{
				DataFileIO::Read(configFile2, (uint8_t*)&configDat.configDatSign_0, 4);
				if (configDat.configDatSign_0 == 0xfffffff7)
				{
					DataFileIO::Read(configFile2, (uint8_t*)&configDat.langIndex_4, 2);
					sprintf(printbuffer, "L%d.TXT", configDat.langIndex_4);
					x_D41A0_BYTEARRAY_4_struct.SelectedLangIndex = sub_7F960(tabBuffer, tabBufferEnd, langBuffer, printbuffer, langDatTab);//tady se pak zmeni v1 za v1_langdattab
				}
				DataFileIO::Close(configFile2);
			}
		}
		ResetMouse_7B5A0();//25c5a0
		x_DWORD_17DE38str.x_WORD_17DEEE_mouse_buttons = 0;
		sub_8CD27_set_cursor(xy_DWORD_17DED4_spritestr[0x25]);//26dd27
		while (mouseClick != 2)//adress 258c30
		{
			if (x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode == 59)
			{
				x_D41A0_BYTEARRAY_4_struct.byteindex_10 = x_D41A0_BYTEARRAY_4_struct.byteindex_10 != 1;
				x_D41A0_BYTEARRAY_4_struct.setting_38402 = 1;
			}
			if (x_WORD_180660_VGA_type_resolution & 1)
				CopyScreen(x_DWORD_E9C38_smalltit, pdwScreenBuffer_351628, 320, 200);
			else
				CopyScreen(x_DWORD_E9C38_smalltit, pdwScreenBuffer_351628, 640, 480);//write default screan 27b144  adress 258c99
			mouseClick = sub_7E0E0_mouse_events();//25f0e0 adress 258ca1 - change button, return click
			sub_2BB40_draw_bitmap(263, 134, langDatTab[1]);//20cb40 adress 258cba - change flag
			if (x_D41A0_BYTEARRAY_4_struct.byteindex_10 == 1)//is 1 not zero!
			{
				if (codeBranch == 2)
				{
					actualTime = j___clock();//279786 , adresa 258cd9
					if ((actualTime - oldTime) / 100 > 1)//when run 100ms
					{
						if (!xx_WORD_E24BE[textIndex++][0])//2b34be
							textIndex = 0;
						oldTime = actualTime;
					}
					memset(textBoxStr, 0, 2 * sizeof(type_E24BCx));//26db3a
					textBoxStr[0] = str_E24BCx[textIndex];
					sub_7E840_draw_textbox_with_line(textBoxStr, 83, 100);//25f840 adress 258d6a - add text?
				}
				else if (codeBranch == 3)
				{
					actualTime = j___clock();
					if ((actualTime - oldTime) / 100 > 1)//when run 100ms
						codeBranch = 2;
				}
			}
			if (frame)
			{
				if (x_WORD_180660_VGA_type_resolution & 1)
					sub_90478_VGA_Blit320();
				else
					sub_75200_VGA_Blit640(480);//draw //256200
			}
			else
			{
				sub_90B27_VGA_pal_fadein_fadeout(x_DWORD_17DE38str.x_DWORD_17DE38x, 0x20u, 0);//271b27
				frame++;
			}
			sub_7A060_get_mouse_and_keyboard_events();//25b060 adress 258ddf
			if (mouseClick == 1)
			{
				if (unknown_libname_3_findnext(&langFileL, langlhandle))//258193
				{
					unknown_libname_4_find_close(&langFileL, langlhandle);
					langlhandle = unknown_libname_2_findfirst(languagePathL, 0, &langFileL);
				}
				// L2.TXT
				selectLang2 = sub_7F960(tabBuffer, tabBufferEnd, langBuffer, langFileL.name, langDatTab);//adress 258dec
				if (x_D41A0_BYTEARRAY_4_struct.SelectedLangIndex == selectLang2)
				{
					if (unknown_libname_3_findnext(&langFileL, langlhandle))
					{
						unknown_libname_4_find_close(&langFileL, langlhandle);
						langlhandle = unknown_libname_2_findfirst(languagePathL, 0, &langFileL);
					}
					selectLang3 = sub_7F960(tabBuffer, tabBufferEnd, langBuffer, langFileL.name, langDatTab);
				}
				else
				{
					selectLang3 = selectLang2;
				}
				x_D41A0_BYTEARRAY_4_struct.SelectedLangIndex = selectLang3;
			}
		}
	}
	//adress 258ea1
	unknown_libname_4_find_close(&langFileL, langlhandle);//adress 258EA2
	configFile = DataFileIO::CreateOrOpenFile(configFilePath, 546);
	if (configFile != NULL)
	{
		if (x_D41A0_BYTEARRAY_4_struct.setting_38402 == 1)
			x_D41A0_BYTEARRAY_4_struct.setting_38402 = 0;

		configDat.configDatSign_0 = 0xfffffff7;
		configDat.langIndex_4 = x_D41A0_BYTEARRAY_4_struct.SelectedLangIndex;
		configDat.soundVolume_6 = x_D41A0_BYTEARRAY_4_struct.soundVolume_6;
		configDat.musicVolume_8 = x_D41A0_BYTEARRAY_4_struct.musicVolume_8;
		configDat.byteindex_10 = x_D41A0_BYTEARRAY_4_struct.byteindex_10;
		configDat.brightness_11 = x_D41A0_BYTEARRAY_4_struct.brightness_11;
		configDat.brightness_12 = x_D41A0_BYTEARRAY_4_struct.brightness_12;
		configDat.brightness_13 = x_D41A0_BYTEARRAY_4_struct.brightness_13;
		configDat.wordindex_14 = x_D41A0_BYTEARRAY_4_struct.wordindex_14;
		configDat.dwordindex_16 = x_D41A0_BYTEARRAY_4_struct.dwordindex_16;
		configDat.stubb[0] = x_D41A0_BYTEARRAY_4_struct.stubb[0];
		configDat.stubb[1] = x_D41A0_BYTEARRAY_4_struct.stubb[1];
		for (int i = 0; i < 10; i++)
			configDat.keys[i] = x_BYTE_EB39E_keys[i];

		WriteFile_98CAA(configFile, (uint8_t*)&configDat, sizeof(TypeConfigDat));

		DataFileIO::Close(configFile);
	}
	sub_90B27_VGA_pal_fadein_fadeout(0, 0x10u, 0);
	if (x_WORD_180660_VGA_type_resolution & 1)
	{
		ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, 0);
	}
	else
	{
		ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 640, 480, 0);
	}
	if (x_WORD_180660_VGA_type_resolution & 1)
		ClearGraphicsBuffer_72883((void*)x_DWORD_E9C38_smalltit, 320, 200, 0);
	else
		ClearGraphicsBuffer_72883((void*)x_DWORD_E9C38_smalltit, 640, 480, 0);

	ResetMouse_7B5A0();
	if (x_WORD_180660_VGA_type_resolution & 1)
		sub_90478_VGA_Blit320();
	else
		sub_75200_VGA_Blit640(480);
	if (langlhandle != 0)//3551d4
	{
		char dataPath[MAX_PATH];
		sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/SCREENS/HSCREEN0.DAT");

		sub_7AA70_load_and_decompres_dat_file(dataPath, (uint8_t*)x_DWORD_17DE38str.x_DWORD_17DE38x, 0, 768);
		sub_7AA70_load_and_decompres_dat_file(dataPath, (uint8_t*)x_DWORD_17DE38str.x_DWORD_17DE40, x_DWORD_17DE38str.x_DWORD_17DEDC, 168081);
		sub_41A90_VGA_Palette_install(x_DWORD_17DE38str.x_DWORD_17DE38x);
		sub_8CD27_set_cursor((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]);//maybe xadatapointerstab
		sub_8CD27_set_cursor(xy_DWORD_17DED4_spritestr[39]);
		if (x_WORD_180660_VGA_type_resolution & 1)
			CopyScreen((void*)pdwScreenBuffer_351628, (void*)x_DWORD_E9C38_smalltit, 320, 200);
		else
			CopyScreen((void*)pdwScreenBuffer_351628, (void*)x_DWORD_E9C38_smalltit, 640, 480);
		sub_7C120_draw_bitmap_640(185, 232, xy_DWORD_17DED4_spritestr[66]);
	}
	else
	{
		x_DWORD_E9C38_smalltit = tempSmalltit;
	}
	sub_7AA70_load_and_decompres_dat_file(0, 0, 0, 0);
	return 1;
}

//----- (0007E640) --------------------------------------------------------
signed int sub_7E640(type_WORD_E1F84* a1x)//25f640
{
	signed __int16 v1; // ax
	//x_WORD *i; // eax
	int ix;
	//x_WORD *j; // eax
	signed int result; // eax

	char dataPath[MAX_PATH];
	std::string dataPath2 = GetSubDirectoryPath(cdDataPath.c_str(), "DATA/SCREENS/HSCREEN0.DAT");

	sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/SCREENS/HSCREEN0.DAT");

	if (a1x)
		v1 = DrawScrollDialog_7BF20(&a1x->str_26);
	else
		v1 = 1;
	if (v1 == 1)
	{
		x_D41A0_BYTEARRAY_4_struct.levelnumber_43w = 0;
		memset(x_DWORD_17DBC8x, 0, 500);
		memset(x_DWORD_17DDBCx, 0, 100);
		//for (i = (x_WORD*)unk_E17CC_0x194; i[2]; *(i - 3) = 70)
		for (ix = 0; mapScreenPortals_E17CC[ix].viewPortPosX_4; ix++)
		{
			//*(i - 3) = 70; //6
			mapScreenPortals_E17CC[ix].word_16 = 70;
			//i += 11;
			//*((x_BYTE *)i - 4) = 2; //4
			mapScreenPortals_E17CC[ix].activated_18 = 2;
			//*((x_BYTE *)i - 3) = 0; //3
			mapScreenPortals_E17CC[ix].byte_19 = 0;
		}
		//for (j = x_WORD_E2970; j[6]; *((x_BYTE *)j - 1) = 0)
		for (int jj = 0; secretMapScreenPortals_E2970[jj].activated_12; jj++)
		{
			secretMapScreenPortals_E2970[jj].byte_16 = 0;
			//*((x_BYTE*)j - 1) = 0
			//*(x_WORD *)((char *)j - 5) = 3;
			secretMapScreenPortals_E2970[jj].activated_12 = 3;
			//*(x_WORD *)((char *)j - 3) = 70;
			secretMapScreenPortals_E2970[jj].word_14 = 70;
		}
		memset(&D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dword_0x3E6_2BE4_12228.str_611, 0, sizeof(type_str_611));
		sub_86860_speak_Sound(x_WORD_1803EC);
		sub_7AA70_load_and_decompres_dat_file(dataPath, x_DWORD_17DE38str.x_DWORD_17DE64_game_world_map, 0xB2C44 + 3, 0x87D80 + 3);
		sub_7AA70_load_and_decompres_dat_file(0, 0, 0, 0);
		x_BYTE_17E09D = 0;
		D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dw_w_b_0_2BDE_11230.byte[2] = 0;
		memset(&x_DWORD_17DB70str, 0, 56);
		*(int16_t*)&x_DWORD_17DE38str.unk_17E078x[8] = 13;
		x_DWORD_17DE38str.x_WORD_17E06C = 0;
		x_DWORD_17DE38str.unk_17E078x[0xa] = 13;
		x_DWORD_17DE38str.x_BYTE_17E09C = 0;
		x_DWORD_17DE38str.x_WORD_17E06E = 480;
		x_DWORD_17DE38str.unk_17E078x[0xb] = 16;
		x_DWORD_17DB70str.x_BYTE_17DB8F = 1;
		if (a1x)
		{
			a1x->selected_8 = 0;
			ClearScrollDialogVars_7C020(&a1x->str_26);
		}
		result = 1;
	}
	else if (v1 == 2)
	{
		a1x->selected_8 = 0;
		ClearScrollDialogVars_7C020(&a1x->str_26);
		result = 1;
	}
	else
	{
		result = 0;
	}
	return result;
}

//----- (000780F0) --------------------------------------------------------
char LoadGameDialog_780F0(type_WORD_E1F84* a1x)//0x2590f0
{
	char* save_name; // edi
	FILE* SEARCH_FILE; // esi
	FILE* FILE; // eax
	//int v16x;
	//__int16 v17; // si
	char v18; // cl
	char v26; // ch
	type_WORD_E1F84 v34x; // [esp+50h] [ebp-6h]
	//int v40; // [esp+88h] [ebp+32h]
	int numLevelsCompleted; // [esp+94h] [ebp+3Eh]
	uint32_t dword_0; // [esp+98h] [ebp+42h]
	int v44; // [esp+9Ch] [ebp+46h]
	__int16 v51; // [esp+B8h] [ebp+62h]
	int im; // [esp+C4h] [ebp+6Eh]
	__int16 v55; // [esp+C8h] [ebp+72h]

	//fix it
	//v40 = 0;
	//fix it

	numLevelsCompleted = 0;
	v44 = 0;
	v55 = 0;
	//v1 = a1x->word_26;
	uint8_t pal_selected_text = getPaletteIndex_5BE80(x_DWORD_17DE38str.x_DWORD_17DE38x, 0x3Fu, 0x3Fu, 0x3Fu);
	uint8_t pal_text = getPaletteIndex_5BE80(x_DWORD_17DE38str.x_DWORD_17DE38x, 0x16u, 0x10u, 9u);
	if (x_DWORD_17DE38str.x_WORD_17DF04 == -1)
	{
		for (im = 0; im < 8; im++)
		{
			//v2 = i;
			//v48 = 43 * im;
			//v3 = (char*)x_DWORD_E9C4C_langindexbuffer[414];//(char *)x_DWORD_EA2C4;//2bb2c4 Empty
			save_name = &x_DWORD_17DE38str.xx_BYTE_17DF14[im][0];
			//v46 = &x_BYTE_17DF14[v48];
			strcpy(save_name, x_DWORD_E9C4C_langindexbuffer[414]);//(char *)x_DWORD_EA2C4;//2bb2c4 Empty
			x_DWORD_17DE38str.xx_BYTE_17DF14[im][41] = 0;
			x_DWORD_17DE38str.xx_BYTE_17DF14[im][42] = 0;
			std::string saveGameFilePath = GetSaveGameFile(gameFolder, im + 1);
			SEARCH_FILE = DataFileIO::CreateOrOpenFile(saveGameFilePath.c_str(), 512);
			if (SEARCH_FILE != NULL)
			{
				DataFileIO::Read(SEARCH_FILE, (uint8_t*)&dword_0, 4);
				if (dword_0 == 0xFFFFFFF7u)
				{
					DataFileIO::Read(SEARCH_FILE, (uint8_t*)save_name, 20);
					x_DWORD_17DE38str.xx_BYTE_17DF14[im][41] = 1;
				}
				DataFileIO::Close(SEARCH_FILE);
			}
		}
		x_DWORD_17DE38str.x_WORD_17DF04 = 0;
	}
	v51 = DrawScrollDialog_7BF20(&a1x->str_26);
	if ((x_BYTE)v51)
	{
		ClearScrollDialogVars_7C020(&a1x->str_26);
		if ((x_BYTE)v51 == 1 && x_DWORD_17DE38str.x_WORD_17DF04 > 0)
		{
			//Load Saved Game File
			std::string loadFilePath = GetSaveGameFile(gameFolder, x_DWORD_17DE38str.x_WORD_17DF04);
			FILE = DataFileIO::CreateOrOpenFile(loadFilePath.c_str(), 512);
			//v10 = v9;
			if (FILE != NULL)
			{
				DataFileIO::Read(FILE, (uint8_t*)&dword_0, 4);
				if (dword_0 == 0xFFFFFFF7u)
				{
					if (a1x->byte_25)
						sub_7E640(0);
					DataFileIO::Read(FILE, (uint8_t*)&x_DWORD_17DE38str.xx_BYTE_17DF14[(x_DWORD_17DE38str.x_WORD_17DF04 - 1)][0], 20);
					DataFileIO::Read(FILE, (uint8_t*)x_D41A0_BYTEARRAY_4_struct.player_name_57ar, 32);
					DataFileIO::Read(FILE, (uint8_t*)x_D41A0_BYTEARRAY_4_struct.savestring_89, 32);

					//Load completed Secret Portals
					for (int ii = 0; ii < 6; ii++)
					{
						DataFileIO::Read(FILE, readbuffer, 17);
						secretMapScreenPortals_E2970[ii].activated_12 = *(uint16_t*)(readbuffer + 12);
						if (secretMapScreenPortals_E2970[ii].activated_12 == 1)
							secretMapScreenPortals_E2970[ii].word_14 = 305;
						else
							secretMapScreenPortals_E2970[ii].word_14 = 270;
					}
					DataFileIO::Read(FILE, (uint8_t*)&D41A0_0.m_GameSettings, 16);
					DataFileIO::Read(FILE, (uint8_t*)&numLevelsCompleted, 4);
					DataFileIO::Read(FILE, (uint8_t*)&v44, 4);
					DataFileIO::Read(FILE, (uint8_t*)&D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dword_0x3E6_2BE4_12228.str_611, 505);
					DataFileIO::Read(FILE, (uint8_t*)x_DWORD_17DBC8x, 500);
					DataFileIO::Read(FILE, (uint8_t*)x_DWORD_17DDBCx, 100);
					DataFileIO::Close(FILE);
					//v13 = (x_WORD*)unk_E17CC_0x194;
					D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dw_w_b_0_2BDE_11230.word[1] = 0;

					int i = 0;
					//Reset all Portals to inactive
					while (mapScreenPortals_E17CC[i].viewPortPosX_4)
					{
						//v13 += 11;
						//*((x_BYTE *)v13 - 4) = 2;
						mapScreenPortals_E17CC[i].activated_18 = 2;
						i++;
					}

					i = 0;
					//Load completed Portals
					while (i < numLevelsCompleted && mapScreenPortals_E17CC[i].viewPortPosX_4)
					{
						mapScreenPortals_E17CC[i].activated_18 = 1;
						i++;
					}

					i = 0;
					//Set current level number
					while (mapScreenPortals_E17CC[i].viewPortPosX_4)
					{
						if (mapScreenPortals_E17CC[i].activated_18 == 1)
							x_D41A0_BYTEARRAY_4_struct.levelnumber_43w = i;
						i++;
					}
					x_DWORD_17DB70str.x_BYTE_17DB8F = 1;
					memset(&x_DWORD_17DE28str, 0, 13);
					v18 = a1x->byte_25;
					x_DWORD_17DB70str.x_WORD_17DB8A = -1;
					if (v18)
					{
						NewGameSubdraw_81760(/*(type_mapScreenPortals_E17CC*)a1*/);
					}
					else
					{
						x_DWORD_17DE38str.x_WORD_17DF04 = -1;
						NewGameDialog_77350(a1x);
						a1x->dword_4 = 2;
					}
				}
			}
		}
		x_DWORD_17DE38str.x_WORD_17DF04 = -1;
		v55 = 1;
	}

	for (int jm = 0; jm < 8; jm++)
	{
		int j = jm + 1;
		DrawHelpText_6FC50(1);
		if (a1x->str_26.word_36_5 /**(signed __int16*)(a1 + 36)*/ > 16 * (signed __int16)j + 3 * sub_6FC30_get34_height())
		{
      std::string savegame = std::to_string(j) + ". " + std::string(&x_DWORD_17DE38str.xx_BYTE_17DF14[(j - 1)][0]);
      int16_t savegame_y_pos = a1x->str_26.y1_28_1 + 16 * (j + 1);
      int16_t savegame_x_pos = a1x->str_26.x1_26_0 + 20;
      uint8_t pal_text_color = (j == x_DWORD_17DE38str.x_WORD_17DF04) ? pal_selected_text : pal_text;
      DrawText_2BC10(savegame.c_str(), savegame_x_pos, savegame_y_pos, pal_text_color);
		}
	}
	if (a1x->str_26.word_36_5 >= a1x->str_26.word_34_4)
	{
		//for (k = 1; k < 9; k++)
		for (int km = 0; km < 8; km++)
		{
			int k = km + 1;
			v34x.xmin_10 = a1x->str_26.x1_26_0 + 20;
			//v25 = 16 * k;
			v34x.ymin_12 = 16 * k + a1x->str_26.y1_28_1 + 16;
			v34x.sizex_14 = 100;
			//v45 = k;
			v34x.sizey_16 = 16;
			v26 = x_DWORD_17DE38str.xx_BYTE_17DF14[(k - 1)][41];
			//v49 = 43 * (k - 1);
			if (v26 && InRegion_7B200(&v34x, x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx, x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony))
			{
				if (x_DWORD_17DE38str.x_WORD_17DEEE_mouse_buttons & 1)
				{
					x_DWORD_17DE38str.x_WORD_17DF04 = k;
				}
				else
				{
          std::string savegame = std::to_string(k) + ". " + std::string(&x_DWORD_17DE38str.xx_BYTE_17DF14[(k - 1)][0]);
					int16_t savegame_y_pos = a1x->str_26.y1_28_1 + 16 * (k + 1);
					int16_t savegame_x_pos = a1x->str_26.x1_26_0 + 20;
					DrawText_2BC10(savegame.c_str(), savegame_x_pos, savegame_y_pos, pal_selected_text);
				}
			}
		}
	}
	return v55;
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// 8E3D5: using guessed type x_DWORD sprintf(x_DWORD, const char *, ...);
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// E29D6: using guessed type __int16 x_WORD_E29D6;
// EA2C4: using guessed type int x_DWORD_EA2C4;
// 17DB8A: using guessed type __int16 x_WORD_17DB8A;
// 17DB8F: using guessed type char x_BYTE_17DB8F;
// 17DBC8: using guessed type int x_DWORD_17DBC8[];
// 17DDBC: using guessed type int x_DWORD_17DDBC[];
// 17DE28: using guessed type int x_DWORD_17DE28;
// 17DE38: using guessed type int x_DWORD_17DE38;
// 17DEE4: using guessed type int x_DWORD_17DEE4_mouse_position;
// 17DEEE: using guessed type __int16 x_WORD_17DEEE_mouse_buttons;
// 17DF04: using guessed type __int16 x_WORD_17DF04;

//----- (00079610) --------------------------------------------------------
char SetKeysDialog_79610()//25a610
{
	int v0; // eax
	signed __int16 v1; // si
	//signed __int16* v2; // ebx	
	uint8_t* temp_screen_buffer; // edi
	int v4; // ST0C_4
	int v5; // ST04_4
	char* v6; // ST00_4
	//x_WORD* i; // eax
	//x_WORD* j; // ebx
	//x_WORD* k; // eax
	//x_WORD* l; // eax
	uint8_t* v13; // ebx
	//int16_t* v14; // edi
	uint8_t* m; // edi
	signed __int16 v16; // ax
	int v17; // eax
	char v18; // dl
	uint8_t* n; // eax
	signed __int16 v20; // ax
	int v21; // eax
	char v22; // dl
	uint8_t* ii; // eax
	signed __int16 v24; // dx
	unsigned int v25; // eax
	//int v26; // eax
	char v28[60]; // [esp+0h] [ebp-16h]
	type_WORD_E1F84 v29x; // [esp+3Ch] [ebp+26h]
	//__int16 v30; // [esp+46h] [ebp+30h]
	//__int16 v31; // [esp+48h] [ebp+32h]
	//__int16 v32; // [esp+4Ah] [ebp+34h]
	//__int16 v33; // [esp+4Ch] [ebp+36h]
	int v34; // [esp+68h] [ebp+52h]
	int v35; // [esp+6Ch] [ebp+56h]
	int v36; // [esp+70h] [ebp+5Ah]
	unsigned int v37; // [esp+74h] [ebp+5Eh]
	int v38; // [esp+78h] [ebp+62h]
	int v39; // [esp+7Ch] [ebp+66h]
	int v40; // [esp+80h] [ebp+6Ah]
	int v41; // [esp+84h] [ebp+6Eh]
	int v42; // [esp+88h] [ebp+72h]
	int v43; // [esp+8Ch] [ebp+76h]
	int v44; // [esp+90h] [ebp+7Ah]
	int v45; // [esp+94h] [ebp+7Eh]

	//HIWORD(v0) = HIWORD(x_DWORD_17DED4);
	v1 = 0;
	v39 = 359;
	v43 = 372;
	v42 = 410;
	v44 = 0;
	v41 = 0;
	v0 = xy_DWORD_17DED4_spritestr[107].height_5;
	v45 = 191;
	//LOWORD(v0) = (unsigned __int8)v0;
	v40 = v0;
	v38 = j___clock();
	v35 = v38;
	memset(v28, 0, 60);
	//v2 = (signed __int16 *)&unk_E28A8;	
	sub_7A110_load_hscreen(x_WORD_180660_VGA_type_resolution, 15);
	temp_screen_buffer = pdwScreenBuffer_351628;
	pdwScreenBuffer_351628 = x_DWORD_E9C38_smalltit;
	//v2 = (signed __int16*)x_BYTE_E25ED_2BB;
	for (int v2_int = 0; str_BYTE_E25ED_2BB[v2_int].word_0; v2_int++)
	{
		v4 = str_BYTE_E25ED_2BB[v2_int].word_2;
		v5 = str_BYTE_E25ED_2BB[v2_int].word_0;
		v6 = x_DWORD_E9C4C_langindexbuffer[str_BYTE_E25ED_2BB[v2_int].word_12];
		sub_7FCB0_draw_text_with_border(v6, v5, v39, v4, 4, 0, 0);
	}
	pdwScreenBuffer_351628 = temp_screen_buffer;
	ResetMouse_7B5A0();
	for (int v2_int = 0; str_BYTE_E25ED_2BB[v2_int].word_0; v2_int++)
		str_BYTE_E25ED_2BB[v2_int].word_14 = 0;
	/*for (i = (x_WORD*)x_BYTE_E25ED_2BB; *i; *(i - 2) = 0)//delete
		i += 9;*/
	str_BYTE_E25ED_2BB[0].word_14 = 1;
	//x_BYTE_E25ED_2BB[14] = 1;

	sub_8CD27_set_cursor(xy_DWORD_17DED4_spritestr[110]);
	while (v44 != 2)
	{
		v34 = j___clock();
		v36 = v34;
		if (x_WORD_180660_VGA_type_resolution & 1)
			CopyScreen((void*)x_DWORD_E9C38_smalltit, (void*)pdwScreenBuffer_351628, 320, 200);
		else
			CopyScreen((void*)x_DWORD_E9C38_smalltit, (void*)pdwScreenBuffer_351628, 640, 480);
		//backgroung
		if (x_DWORD_17DE38str.x_WORD_17DEEE_mouse_buttons & 1)//switch blocks
		{
			for (int j_int = 0; str_BYTE_E25ED_2BB[j_int].word_0; j_int++)
				//for (j = (x_WORD*)x_BYTE_E25ED_2BB; *j; j += 9)//?
			{
				v29x.xmin_10 = v45;
				v29x.ymin_12 = str_BYTE_E25ED_2BB[j_int].word_2 - 4;
				v29x.sizex_14 = v42;
				v29x.sizey_16 = v40;
				if (InRegion_7B200(&v29x, x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx, x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony))
				{
					for (int k_int = 0; str_BYTE_E25ED_2BB[k_int].word_0; k_int++)
						str_BYTE_E25ED_2BB[k_int].word_14 = 0;
					str_BYTE_E25ED_2BB[0].word_14 = 1;
					/*for (k = (x_WORD*)x_BYTE_E25ED_2BB; *k; *(k - 2) = 0)
						k += 9;//?*/
						//j[7] = 1;
					v1 = 0;
					break;
				}
			}
		}
		v44 = TestMouseRegions_7E1F0();
		if (v44 == 1)
		{
			x_D41A0_BYTEARRAY_4_struct.setting_38402 = 1;
			sub_5BCC0_set_any_variables1();
			for (int l_int = 0; str_BYTE_E25ED_2BB[l_int].word_0; l_int++)
				str_BYTE_E25ED_2BB[l_int].word_14 = 0;
			str_BYTE_E25ED_2BB[0].word_14 = 1;
			/*
			for (l = (x_WORD*)x_BYTE_E25ED_2BB; *l; *(l - 2) = 0)
				l += 9;
			v1 = 0;
			x_BYTE_E25ED_2BB[14] = 1;
			*/
		}
		v13 = &x_BYTE_EB39E_keys[0];//2bc39e
		//v14 = (int16_t*)x_BYTE_E25ED_2BB;
		int v14_int = 0;
		while (v13 < &x_BYTE_EB39E_keys[9] + 1)//drawing texts
		{
			memset(v28, 0, 60);
			if (!str_BYTE_E25ED_2BB[v14_int].word_14)
			{
				sub_79E10(v28, *v13);
				sub_7FCB0_draw_text_with_border(/*(int)v13,*/ v28, v43, v42, str_BYTE_E25ED_2BB[v14_int].word_2, 4, 0, 0);
			}
			v13++;
			//v14 += 9;
			v14_int++;
		}
		//v2 = (signed __int16*)x_BYTE_E25ED_2BB;
		int v2_int = 0;
		for (m = &x_BYTE_EB39E_keys[0]; m < &x_BYTE_EB39E_keys[9] + 1 && !v1; ++m)
		{
			v16 = str_BYTE_E25ED_2BB[v2_int].word_14;
			if (v16 >= 1u)
			{
				v37 = v34 - v38;
				if (v16 <= 1u)
				{
					memset(v28, 0, 60);
					sub_79E10((char*)v28, *m);
					sub_7FCB0_draw_text_with_border(/*(int)v2,*/ v28, v43, v42, str_BYTE_E25ED_2BB[v2_int].word_2, 4, 0, 0);
					if (v37 > 0x32)
					{
						v17 = v34;
						str_BYTE_E25ED_2BB[v2_int].word_14 = 2;
						v38 = v17;
					}
					sub_2BB40_draw_bitmap(v45, str_BYTE_E25ED_2BB[v2_int].word_2, xy_DWORD_17DED4_spritestr[107]);
					if (x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode && sub_79E10((char*)v28, x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode))
					{
						v18 = 0;
						if (m != &x_BYTE_EB39E_keys[6])
						{
							for (n = &x_BYTE_EB39E_keys[0]; n < &x_BYTE_EB39E_keys[9] + 1; n++)
							{
								if (x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode == *n)
								{
									v18 = 1;
									break;
								}
							}
						}
						if (!v18)
						{
							x_D41A0_BYTEARRAY_4_struct.setting_38402 = 1;
							//v2 += 9;//?
							str_BYTE_E25ED_2BB[v2_int].word_14 = 0;
							v2_int++;
							*m = x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode;
							//*(v2 - 2) = 0;
							v20 = str_BYTE_E25ED_2BB[v2_int].word_0;
							x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode = 0;
							if (v20)
							{
								str_BYTE_E25ED_2BB[v2_int].word_14 = 1;
							}
							else
							{
								v1 = 1;
								v38 = v34;
							}
						}
					}
				}
				else if (v16 == 2)
				{
					if ((v34 - v38) > 0x32)
					{
						v21 = v34;
						str_BYTE_E25ED_2BB[v2_int].word_14 = 1;
						v38 = v21;
					}
					sub_2BB40_draw_bitmap(v45, str_BYTE_E25ED_2BB[v2_int].word_2, xy_DWORD_17DED4_spritestr[107]);
					if (x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode)
					{
						if (sub_79E10((char*)v28, x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode))
						{
							v22 = 0;
							if (m != &x_BYTE_EB39E_keys[9] + 1)
							{
								for (ii = &x_BYTE_EB39E_keys[0]; ii < &x_BYTE_EB39E_keys[9] + 1; ++ii)
								{
									if (x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode == *ii)
									{
										v22 = 1;
										break;
									}
								}
							}
							if (!v22)
							{
								x_D41A0_BYTEARRAY_4_struct.setting_38402 = 1;
								*m = x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode;
								str_BYTE_E25ED_2BB[v2_int].word_14 = 0;
								x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode = 0;
								v2_int++;
								v24 = str_BYTE_E25ED_2BB[v2_int].word_0;
								//v24 = v2[9];
								//v2 += 9;
								if (v24)
									str_BYTE_E25ED_2BB[v2_int].word_14 = 1;
								else
									v1 = 1;
							}
						}
					}
				}
			}
			//v2 += 9;
			v2_int++;
		}
		//?
		if (v1 >= 1u)
		{
			v25 = v34 - v38;
			if (v1 <= 1u)
			{
				if (v25 > 0x32)
				{
					v1 = 2;
					v38 = v34;
				}
				sub_2BB40_draw_bitmap(283, 381, xy_DWORD_17DED4_spritestr[108]);
			}
			else if (v1 == 2 && v25 > 0x32)
			{
				v1 = 1;
				v38 = v34;
			}
		}
		if (x_WORD_180660_VGA_type_resolution & 1)
			sub_90478_VGA_Blit320();
		else
			sub_75200_VGA_Blit640(480);
		if (!v41)
		{
			sub_90B27_VGA_pal_fadein_fadeout(x_DWORD_17DE38str.x_DWORD_17DE38x, 0x20u, 0);
			v41 = 1;
		}
		if ((v36 - v35) > 0xA)
		{
			sub_7A060_get_mouse_and_keyboard_events();
			v35 = v36;
		}
		if (x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode == 1)
			v44 = 2;
		if (x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode == 0x1c)
		{
			if (v1)
				v44 = 2;
		}
	}
	WriteConfigDat_81DB0();
	sub_90B27_VGA_pal_fadein_fadeout(0, 0x10u, 0);
	if (x_WORD_180660_VGA_type_resolution & 1)
	{
		ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, 0);
	}
	else
	{
		ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 640, 480, 0);
	}
	if (x_WORD_180660_VGA_type_resolution & 1)
		ClearGraphicsBuffer_72883((void*)x_DWORD_E9C38_smalltit, 320, 200, 0);
	else
		ClearGraphicsBuffer_72883((void*)x_DWORD_E9C38_smalltit, 640, 480, 0);
	ResetMouse_7B5A0();
	if (x_WORD_180660_VGA_type_resolution & 1)
		sub_90478_VGA_Blit320();
	else
		sub_75200_VGA_Blit640(480);

	char dataPath[MAX_PATH];
	sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/SCREENS/HSCREEN0.DAT");

	sub_7AA70_load_and_decompres_dat_file(dataPath, (uint8_t*)x_DWORD_17DE38str.x_DWORD_17DE38x, 0, 768);
	sub_7AA70_load_and_decompres_dat_file(dataPath, x_DWORD_17DE38str.x_DWORD_17DE40, x_DWORD_17DE38str.x_DWORD_17DEDC, 168081);
	sub_7AA70_load_and_decompres_dat_file(0, 0, 0, 0);
	sub_8CD27_set_cursor((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]);
	sub_41A90_VGA_Palette_install(x_DWORD_17DE38str.x_DWORD_17DE38x);
	sub_8CD27_set_cursor(xy_DWORD_17DED4_spritestr[39]);
	if (x_WORD_180660_VGA_type_resolution & 1)
		CopyScreen((void*)pdwScreenBuffer_351628, (void*)x_DWORD_E9C38_smalltit, 320, 200);
	else
		CopyScreen((void*)pdwScreenBuffer_351628, (void*)x_DWORD_E9C38_smalltit, 640, 480);
	return 1;
}

//----- (00079E10) --------------------------------------------------------
signed int sub_79E10(char* a1, char a2)
{
	signed int result; // eax

	result = 0;
	switch (a2)
	{
	case 12://0c
	case 13://0d
		strcpy(a1, "- ");
		result = 1;
		break;
	case 16://10
		strcpy(a1, "Q ");
		result = 1;
		break;
	case 17://11
		strcpy(a1, "W ");
		result = 1;
		break;
	case 18://12
		strcpy(a1, "E ");
		result = 1;
		break;
	case 20://14
		strcpy(a1, "T ");
		result = 1;
		break;
	case 21://15
		strcpy(a1, "Y ");
		result = 1;
		break;
	case 22://16
		strcpy(a1, "U ");
		result = 1;
		break;
	case 28://1c
		result = 1;
		strcpy(a1, "ENTER");
		break;
	case 29://1d
		result = 1;
		strcpy(a1, "CTRL");
		break;
	case 30://1e
		strcpy(a1, "A ");
		result = 1;
		break;
	case 33://21
		strcpy(a1, "F ");
		result = 1;
		break;
	case 34://22
		strcpy(a1, "G ");
		result = 1;
		break;
	case 35://23
		strcpy(a1, "H ");
		result = 1;
		break;
	case 36://24
		strcpy(a1, "J ");
		result = 1;
		break;
	case 37://25
		strcpy(a1, "K ");
		result = 1;
		break;
	case 39://27
		strcpy(a1, "; ");
		result = 1;
		break;
	case 42://2a
		result = 1;
		strcpy(a1, "Left Shift");
		break;
	case 43://2b
		strcpy(a1, "# ");
		result = 1;
		break;
	case 44://2c
		strcpy(a1, "Z ");
		result = 1;
		break;
	case 45://2d
		strcpy(a1, "X ");
		result = 1;
		break;
	case 48://30
		strcpy(a1, "B ");
		result = 1;
		break;
	case 49://31
		strcpy(a1, "N ");
		result = 1;
		break;
	case 51://33
		strcpy(a1, ", ");
		result = 1;
		break;
	case 52://34
		strcpy(a1, ". ");
		result = 1;
		break;
	case 53://35
		strcpy(a1, "/ ");
		result = 1;
		break;
	case 54://36
		result = 1;
		strcpy(a1, "Right Shift");
		break;
	case 56://38
		strcpy(a1, "Alt");
		result = 1;
		break;
	case 72://48
		result = 1;
		strcpy(a1, "UP");
		break;
	case 75://4b
		result = 1;
		strcpy(a1, "LEFT");
		break;
	case 77://4d
		result = 1;
		strcpy(a1, "RIGHT");
		break;
	case 80://50
		result = 1;
		strcpy(a1, "DOWN");
		break;
	default:
		return result;
	}
	return result;
}
// D1AE4: using guessed type int x_DWORD_D1AE4;
// D1B00: using guessed type __int16 x_WORD_D1B00;
// D1B04: using guessed type __int16 x_WORD_D1B04;
// D1B08: using guessed type __int16 x_WORD_D1B08;
// D1B0C: using guessed type __int16 x_WORD_D1B0C;
// D1B10: using guessed type __int16 x_WORD_D1B10;
// D1B14: using guessed type __int16 x_WORD_D1B14;
// D1B18: using guessed type __int16 x_WORD_D1B18;
// D1B1C: using guessed type __int16 x_WORD_D1B1C;
// D1B20: using guessed type __int16 x_WORD_D1B20;
// D1B24: using guessed type __int16 x_WORD_D1B24;
// D1B28: using guessed type __int16 x_WORD_D1B28;
// D1B2C: using guessed type __int16 x_WORD_D1B2C;
// D1B30: using guessed type __int16 x_WORD_D1B30;
// D1B34: using guessed type __int16 x_WORD_D1B34;
// D1B38: using guessed type __int16 x_WORD_D1B38;
// D1B3C: using guessed type __int16 x_WORD_D1B3C;
// D1B40: using guessed type __int16 x_WORD_D1B40;
// D1B44: using guessed type __int16 x_WORD_D1B44;
// D1B48: using guessed type __int16 x_WORD_D1B48;
// D1B4C: using guessed type __int16 x_WORD_D1B4C;
// D1B50: using guessed type __int16 x_WORD_D1B50;
// D1B54: using guessed type __int16 x_WORD_D1B54;

//----- (0007AB00) --------------------------------------------------------
void DrawMenuAnimations_7AB00()//25bb00
{
	unsigned long now = j___clock();

	// animate fire and incense stick animation
	for (auto& ani: MainMenuAnimations_E1748x)
	{
		sub_2BB40_draw_bitmap(ani.PosX_4, ani.PosY_6, xy_DWORD_17DED4_spritestr[ani.ActSprite_8]);
		if ((now - ani.LastTimeRendered_0) >> 2 >= 1)
		{
			ani.ActSprite_8++;
			if (ani.LastSprite_11 < ani.ActSprite_8)
				ani.ActSprite_8 = ani.FirstSprite_10;
			ani.LastTimeRendered_0 = now;
		}
	}
	// draw selected main menu item
	for (int iy = 0; (str_E1BAC[iy].xmin_10<<16) + str_E1BAC[iy].ymin_12; iy++)
	{
		if (str_E1BAC[iy].canSelect_23 && str_E1BAC[iy].gold_color_24)
		{
			sub_2BB40_draw_bitmap(str_E1BAC[iy].xmin_10, str_E1BAC[iy].ymin_12, xy_DWORD_17DED4_spritestr[str_E1BAC[iy].byte_21]);
		}
	}
}

//----- (0007AC00) --------------------------------------------------------
void LoadAndSetGraphicsAndPalette_7AC00()//25BC00
{
	//char v0; // al
	//unsigned __int8 v1; // al
	//unsigned __int8 v2; // al

	if (!(x_WORD_180660_VGA_type_resolution & 8))
	{
		/*if (x_DWORD_E9C3C)
		{
			sub_83E80_freemem4(x_DWORD_E9C3C);
			x_DWORD_E9C3C = 0;
		}*/
		//fix
		if (pre_x_DWORD_E9C3C)
		{
			FreeMem_83E80(pre_x_DWORD_E9C3C);
			pre_x_DWORD_E9C3C = 0;
			x_DWORD_E9C3C = 0;
		}
		//fix
		sub_54600_mouse_reset();//235600 //mouse reset
		sub_6EBF0(&filearray_2aa18c[filearrayindex_POINTERSDATTAB]);//24FBF0 - 2AA18C//?tab
		if (x_WORD_180660_VGA_type_resolution == 1)
		{
			x_WORD_E29DA_type_resolution = x_WORD_180660_VGA_type_resolution;
			x_WORD_180660_VGA_type_resolution = 8;
			/*
			x_DWORD_E9C3C = (uint8_t*)sub_83CD0_malloc2(307200);
			*/
			//fix
			pre_x_DWORD_E9C3C = (uint8_t*)sub_83CD0_malloc2(3000000);
			x_DWORD_E9C3C = &pre_x_DWORD_E9C3C[200000];
			//fix
			CreateIndexes_6EB90(&filearray_2aa18c[filearrayindex_POINTERSDATTAB]);
			memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 768);
			if (x_WORD_180660_VGA_type_resolution & 1)
			{
				//v0 = sub_5BE80_test_Palette(*xadatapald0dat2.colorPalette_var28, 0, 0, 0);
				//ClearGraphicsBuffer320(/*(void*)pdwScreenBuffer_351628, */(void *)pdwScreenBuffer_351628, 200, sub_5BE80_test_Palette(*xadatapald0dat2.colorPalette_var28, 0, 0, 0));
			}
			else
			{
				//v1 = sub_5BE80_test_Palette(*xadatapald0dat2.colorPalette_var28, 0, 0, 0);
				//ClearGraphicsBuffer640(&pdwScreenBuffer_351628, 480, sub_5BE80_test_Palette(*xadatapald0dat2.colorPalette_var28, 0, 0, 0));//[351628d]3aa0a4,1E0,0
			}
			memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 768);
			sub_41A90_VGA_Palette_install((TColor*)*xadatapald0dat2.colorPalette_var28);
			if (x_WORD_180660_VGA_type_resolution & 1)
				sub_90D6E_VGA_set_video_mode_320x200_and_Palette((TColor*)*xadatapald0dat2.colorPalette_var28);
			else
				sub_90E07_VGA_set_video_mode_640x480_and_Palette((TColor*)*xadatapald0dat2.colorPalette_var28);
			sub_41A90_VGA_Palette_install((TColor*)*xadatapald0dat2.colorPalette_var28);
			uiBackGroundColorIdx_EB3A8 = getPaletteIndex_5BE80((TColor*)*xadatapald0dat2.colorPalette_var28, 0, 0, 0);
			if (x_WORD_180660_VGA_type_resolution & 1)
				ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, uiBackGroundColorIdx_EB3A8);
			else
				ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 640, 480, uiBackGroundColorIdx_EB3A8);

			sub_8CEDF_install_mouse();
			sub_8CD27_set_cursor((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]);
		}
	}
	sub_8CD27_set_cursor((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]);
}

//----- (0007ADE0) --------------------------------------------------------
int sub_7ADE0(char a1)//25bde0
{
	char v1; // al
	unsigned __int8 v2; // al
	unsigned __int8 v3; // al

	if (a1 == 1)
	{
		/*if (x_DWORD_E9C3C)
		{
			sub_83E80_freemem4(x_DWORD_E9C3C);
			x_DWORD_E9C3C = 0;
		}
		*/
		//fix
		if (pre_x_DWORD_E9C3C)
		{
			FreeMem_83E80(pre_x_DWORD_E9C3C);
			pre_x_DWORD_E9C3C = 0;
			x_DWORD_E9C3C = 0;
		}
		//fix
		sub_54600_mouse_reset();//mouse reset
		sub_6EBF0(&filearray_2aa18c[filearrayindex_POINTERSDATTAB]);
		x_WORD_180660_VGA_type_resolution = 1;
		x_WORD_E29DA_type_resolution = 1;
		/*
		x_DWORD_E9C3C = (uint8_t*)sub_83CD0_malloc2(64000);
		*/
		//fix
		pre_x_DWORD_E9C3C = (uint8_t*)sub_83CD0_malloc2(3000000);
		x_DWORD_E9C3C = &pre_x_DWORD_E9C3C[2000000];
		//fix
		//sub_6EB90(&*filearray_2aa18c[0]);
		CreateIndexes_6EB90(&(filearray_2aa18c[filearrayindex_POINTERSDATTAB]));
		memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 768);
		if (x_WORD_180660_VGA_type_resolution & 1)
		{
			v1 = getPaletteIndex_5BE80((TColor*)*xadatapald0dat2.colorPalette_var28, 0, 0, 0);
			ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, v1);
		}
		else
		{
			v2 = getPaletteIndex_5BE80((TColor*)*xadatapald0dat2.colorPalette_var28, 0, 0, 0);
			ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 640, 480, v2);
		}
		memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 768);
		sub_41A90_VGA_Palette_install((TColor*)*xadatapald0dat2.colorPalette_var28);
		if (x_WORD_180660_VGA_type_resolution & 1)
			sub_90D6E_VGA_set_video_mode_320x200_and_Palette((TColor*)*xadatapald0dat2.colorPalette_var28);
		else
			sub_90E07_VGA_set_video_mode_640x480_and_Palette((TColor*)*xadatapald0dat2.colorPalette_var28);
		sub_41A90_VGA_Palette_install((TColor*)*xadatapald0dat2.colorPalette_var28);
		v3 = getPaletteIndex_5BE80((TColor*)*xadatapald0dat2.colorPalette_var28, 0, 0, 0);
		uiBackGroundColorIdx_EB3A8 = v3;
		if (x_WORD_180660_VGA_type_resolution & 1)
			ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, v3);
		else
			ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 640, 480, v3);
		sub_8CEDF_install_mouse();
	}
	sub_8CD27_set_cursor((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]);
	return 0;//fix it sub_8CD27_set_cursor((uint8_t**)**filearray_2aa18c[0]);
}

//----- (0007BEC0) --------------------------------------------------------
void sub_7BEC0()//25cec0
{
	//uint8_t* result; // eax
	int resultx;
	unsigned __int8 v1; // dl

	//for (result = off_E1BAC; *((uint16_t*)result + 5); result += 44)
	for (resultx = 0; str_E1BAC[resultx].xmin_10; resultx++)
	{
		str_E1BAC[resultx].selected_8 = 0;
		v1 = str_E1BAC[resultx].byte_22;
		str_E1BAC[resultx].gold_color_24 = 0;
		if (v1 == 7u)
		{
			str_E1BAC[resultx].canSelect_23 = x_D41A0_BYTEARRAY_4_struct.isNetwork_216w != 0;
		}
		else if (v1 == 13)
		{
			str_E1BAC[resultx].canSelect_23 = 1;
		}		
	}
	//return result;
	/*
	for (uint32_t i=0; *(uint32_t*)(off_E1BAC+i+10); i += 0x2c)//2b2bac
	{
		*(uint32_t*)(off_E1BAC + i + 8) = 0;
		*(off_E1BAC + i + 18) = 0;
		if (*(off_E1BAC + i + 16) >= 7u)
		{
			if (*(off_E1BAC + i + 16) <= 7u)
			{
				*(off_E1BAC + i + 17) = (x_D41A0_BYTEARRAY_4_struct.setting_216 != 0);
			}
			else if (*(off_E1BAC + i + 16) == 13)
			{
				*(off_E1BAC + i + 17) = 1;
			}
		}
	}*/
}
// D41A4: using guessed type int x_DWORD_D41A4;

//----- (0007DD70) --------------------------------------------------------
void sub_7DD70()//25ed70
{
	//int v0; // esi
	int resulty; // eax
	__int16 v2; // dx
	char v3; // bl

	//v0 = (int)x_D41A0_BYTEARRAY_4;
	//resultx = (int)unk_E17CC_0x194;
	resulty = 0;
	v2 = 0;
	while (mapScreenPortals_E17CC[resulty].viewPortPosX_4)
	{
		v3 = mapScreenPortals_E17CC[resulty].activated_18;
		mapScreenPortals_E17CC[resulty].word_20 = 0;
		if (v3 == 2)
			mapScreenPortals_E17CC[resulty].word_16 = 70;
		if (D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dw_w_b_0_2BDE_11230.byte[2] & 2
			&& x_D41A0_BYTEARRAY_4_struct.levelnumber_43w == v2)
		{
			mapScreenPortals_E17CC[resulty].activated_18 = 1;
		}
		if (mapScreenPortals_E17CC[resulty].activated_18 == 1)
			mapScreenPortals_E17CC[resulty].word_16 = 37;
		//resultx += 22;
		mapScreenPortals_E17CC[resulty].byte_19 = 0;
		resulty++;
		v2++;
		//*(x_BYTE *)(resultx - 3) = 0;
	}
	if (x_D41A0_BYTEARRAY_4_struct.levelnumber_43w > 0x18u)
	{
		//for (result = (int)x_WORD_E2970; *(x_WORD *)(result + 12); result += 17)
		for (int ir = 0; secretMapScreenPortals_E2970[ir].activated_12; ir++)
		{
			if (x_D41A0_BYTEARRAY_4_struct.levelnumber_43w == secretMapScreenPortals_E2970[ir].levelNumber_6)
			{
				if (D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dw_w_b_0_2BDE_11230.byte[2] & 2)
				{
					secretMapScreenPortals_E2970[ir].activated_12 = 1;
					secretMapScreenPortals_E2970[ir].word_14 = 305;
				}
				else if (secretMapScreenPortals_E2970[ir].activated_12 == 3)
				{
					secretMapScreenPortals_E2970[ir].activated_12 = 2;
					secretMapScreenPortals_E2970[ir].word_14 = 70;
				}
				mapScreenPortals_E17CC[secretMapScreenPortals_E2970[ir].word_4].activated_18 = 1;
				//result = 22 * secretMapScreenPortals_E2970[ir].word_4;
				mapScreenPortals_E17CC[secretMapScreenPortals_E2970[ir].word_4].word_16 = 37;
				break;
			}
		}
	}
	//x_D41A0_BYTEARRAY_4 = (uint8_t*)v0;
	//return result;
}

int pre_sub_7E320(uint32_t adress, type_WORD_E1F84* a1x)
{
	switch (adress)
	{
	case 0x25f800: {
		return sub_7E800(a1x);//25f800
	}
	case 0x25f820: {
		return sub_7E820(a1x);//25f800
	}
	case 0x25f640: {
		return sub_7E640(a1x);//25f800
	}
	case 0x25f620: {
		return sub_7E620(a1x);//25f800
	}
	}
	mydelay(10000);
	exit(1);
}

//----- (0007E620) --------------------------------------------------------
signed int sub_7E620(type_WORD_E1F84* a1x)//25f620
{
	a1x->selected_8 = 0;
	return 2;
}

//----- (0007DA70) --------------------------------------------------------
void sub_7DA70(__int16 a1, __int16 a2, __int16 a3, __int16 a4, __int16 a5, __int16 a6)//25ea70 //draw player in map menu
{
	int v6; // edx
	int v7; // ebx
	__int16 v8; // ax
	int v9; // edx
	__int16 v10; // ax
	uint8_t* v11; // ebx
	int v12; // edx
	axis_3d v14x; // [esp+0h] [ebp-10h]
	//__int16 v15; // [esp+2h] [ebp-Eh]
	axis_3d v16x; // [esp+8h] [ebp-8h]
	//__int16 v17; // [esp+Ah] [ebp-6h]

	x_DWORD_17DE38str.x_WORD_17E074 = a3;
	v6 = xy_DWORD_17DED4_spritestr[37].width_4;
	x_DWORD_17DE38str.x_WORD_17E06C = a1;
	v7 = v6 / 2;
	v8 = v6 / 2 + a3;
	v9 = xy_DWORD_17DED4_spritestr[37].height_5;
	x_DWORD_17DE38str.x_WORD_17E074 = v8;
	v9 /= 2;
	x_DWORD_17DE38str.x_WORD_17E076 = v9 + a4;
	x_DWORD_17DE38str.x_WORD_17E06C = v7 + a1;
	x_DWORD_17DE38str.x_WORD_17E06E = v9 + a2;
	v16x.x = v8;
	v16x.y = v9 + a4;
	v14x.x = v7 + a1;
	v14x.y = v9 + a2;
	v10 = Maths::sub_581E0_maybe_tan2(&v16x, &v14x);
	if (v10 <= 1024 || v10 >= 1536)
	{
		if (v10 <= 1536 || v10 >= 2048)
		{
			if (v10 <= 2048 && v10 <= 0 || v10 >= 512)
			{
				if (v10 <= 512 || v10 >= 1024)
				{
					if (v10 != 2048 && v10)
					{
						if (v10 == 512)
						{
							if (x_DWORD_17DE38str.unk_17E078x[0xa] != 9)
							{
								*(int16_t*)&x_DWORD_17DE38str.unk_17E078x[8] = 9;
								x_DWORD_17DE38str.unk_17E078x[0xb] = 12;
								x_DWORD_17DE38str.unk_17E078x[0xa] = 9;
							}
						}
						else if (v10 == 1024)
						{
							if (x_DWORD_17DE38str.unk_17E078x[0xa] != 1)
							{
								x_DWORD_17DE38str.unk_17E078x[0xa] = 1;
								*(int16_t*)&x_DWORD_17DE38str.unk_17E078x[8] = 1;
								x_DWORD_17DE38str.unk_17E078x[0xb] = 4;
							}
						}
						else if (v10 == 1536 && x_DWORD_17DE38str.unk_17E078x[0xa] != 25)
						{
							*(int16_t*)&x_DWORD_17DE38str.unk_17E078x[8] = 25;
							x_DWORD_17DE38str.unk_17E078x[0xa] = 25;
							x_DWORD_17DE38str.unk_17E078x[0xb] = 28;
						}
					}
					else if (x_DWORD_17DE38str.unk_17E078x[0xa] != 17)
					{
						x_DWORD_17DE38str.unk_17E078x[0xa] = 17;
						*(int16_t*)&x_DWORD_17DE38str.unk_17E078x[8] = 17;
						x_DWORD_17DE38str.unk_17E078x[0xb] = 20;
					}
				}
				else if (x_DWORD_17DE38str.unk_17E078x[0xa] != 13)
				{
					*(int16_t*)&x_DWORD_17DE38str.unk_17E078x[8] = 13;
					x_DWORD_17DE38str.unk_17E078x[0xa] = 13;
					x_DWORD_17DE38str.unk_17E078x[0xb] = 16;
				}
			}
			else if (x_DWORD_17DE38str.unk_17E078x[0xa] != 5)
			{
				x_DWORD_17DE38str.unk_17E078x[0xa] = 5;
				*(int16_t*)&x_DWORD_17DE38str.unk_17E078x[8] = 5;
				x_DWORD_17DE38str.unk_17E078x[0xb] = 8;
			}
		}
		else if (x_DWORD_17DE38str.unk_17E078x[0xa] != 29)
		{
			*(int16_t*)&x_DWORD_17DE38str.unk_17E078x[8] = 29;
			x_DWORD_17DE38str.unk_17E078x[0xb] = 32;
			x_DWORD_17DE38str.unk_17E078x[0xa] = 29;
		}
	}
	else if (x_DWORD_17DE38str.unk_17E078x[0xa] != 21)
	{
		*(int16_t*)&x_DWORD_17DE38str.unk_17E078x[8] = 21;
		x_DWORD_17DE38str.unk_17E078x[0xa] = 21;
		x_DWORD_17DE38str.unk_17E078x[0xb] = 24;
	}
	*(int16_t*)&x_DWORD_17DE38str.unk_17E078x[4] = x_DWORD_17DE38str.x_WORD_17E06C;
	*(int16_t*)&x_DWORD_17DE38str.unk_17E078x[6] = x_DWORD_17DE38str.x_WORD_17E06E;
	v11 = xy_DWORD_17DED4_spritestr[*(int16_t*)&x_DWORD_17DE38str.unk_17E078x[8]].data;
	v12 = x_DWORD_17DE38str.x_WORD_17E06C - *(unsigned __int8*)(v11 + 4) / 2 - a5;
	sub_81260(
		(signed __int16)v12,
		v12,
		x_DWORD_17DE38str.unk_17E078x,
		x_DWORD_17DE38str.x_WORD_17E06C - xy_DWORD_17DED4_spritestr[*(int16_t*)&x_DWORD_17DE38str.unk_17E078x[8]].width_4 / 2 - a5,
		x_DWORD_17DE38str.x_WORD_17E06E - xy_DWORD_17DED4_spritestr[*(int16_t*)&x_DWORD_17DE38str.unk_17E078x[8]].height_5 / 2 - a6);
}

//----- (0007E320) --------------------------------------------------------
signed int sub_7E320_draw_bitmaps_and_play_sounds(/*__int16 a1, int a2*/)//25f320
{
	int ix;
	int v3x;
	int jx;
	int ky;
	int v9; // edx
	int v11y;
	posistruct_t* v12; // edi
	posistruct2_t* v13x; // esi
	type_E24BCx v14x[2];

	for (ix = 0; str_E23E0[ix].byte_25; ix++)//draw buttons
	{
		if (str_E23E0[ix].canSelect_23)
		{
			//a2 = str_E23E0[ix].byte_21;
			sub_7C120_draw_bitmap_640(str_E23E0[ix].xmin_10, str_E23E0[ix].ymin_12, xy_DWORD_17DED4_spritestr[str_E23E0[ix].byte_21]);
		}
	}
	if (x_DWORD_17DB70str.x_BYTE_17DB8E)
		return 0;
	v3x = 0;
	if (str_E23E0[v3x].byte_25)
	{
		do//adress 25f57c
		{
			if (str_E23E0[v3x].selected_8 && str_E23E0[v3x].dword_0)
			{
				uint8_t v5 = pre_sub_7E320(str_E23E0[v3x].dword_0, &str_E23E0[v3x]);
				if (!v5)
					return 1;
				str_E23E0[v3x].selected_8 = 0;
				ResetMouse_7B5A0();
				return v5;
			}
			v3x++;
		} while (str_E23E0[v3x].byte_25);
		jx = 0;
		if (!str_E23E0[0].byte_25)
		{
			ky = 0;
			goto LABEL_34;
		}
		goto LABEL_17;
	}
	for (jx = 0; str_E23E0[jx].byte_25; jx++ )
	{
	LABEL_17:
		if (str_E23E0[jx].canSelect_23 && str_E23E0[jx].gold_color_24)
			sub_7C120_draw_bitmap_640(str_E23E0[jx].xmin_10, str_E23E0[jx].ymin_12, xy_DWORD_17DED4_spritestr[str_E23E0[jx].byte_20]);
		
		str_E23E0[jx].selected_8 = 0;
		str_E23E0[jx].gold_color_24 = 0;		
	}
	for (ky = 0; ; ky++)
	{
	LABEL_34:
		if (!str_E23E0[ky].byte_25)
		{
			sub_82510();
			return 0;
		}
		if (str_E23E0[ky].canSelect_23)
		{
			v9 = str_E23E0[ky].byte_20;
			str_E23E0[ky].sizex_14 = xy_DWORD_17DED4_spritestr[v9].width_4;
			str_E23E0[ky].sizey_16 = xy_DWORD_17DED4_spritestr[v9].height_5;
			if (InRegion_7B200(&str_E23E0[ky], x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx, x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony))
				break;
		}
	LABEL_33:
		;
	}
	if (x_DWORD_17DE38str.x_WORD_17DEEE_mouse_buttons & 1)
	{
		str_E23E0[ky].selected_8 = 1;
		ResetMouse_7B5A0();
		sub_8F100_sound_proc19(0, 14, 127, 64, 0x64u, 0, 3u);
		goto LABEL_33;
	}
	str_E23E0[ky].gold_color_24 = 1;
	if (x_D41A0_BYTEARRAY_4_struct.byteindex_10 != 1)
		return 0;
	v11y = 0;
	if (!str_E2516[v11y].word_2)
		return 0;
	do//adress 25f4e7
	{
		if (str_E2516[v11y].byte_17 == str_E23E0[ky].byte_22)
		{
			memset(v14x, 0, 36);
			v14x[0] = str_E2516[v11y];
			v12 = xy_DWORD_17DEC0_spritestr;
			v13x = x_DWORD_17DE38str.x_DWORD_17DEC4;

			xy_DWORD_17DEC0_spritestr = xy_DWORD_17DEC8_spritestr;
			x_DWORD_17DE38str.x_DWORD_17DEC4 = x_DWORD_17DE38str.x_DWORD_17DECC;
			sub_7E840_draw_textbox_with_line(v14x, 238, 264);
			xy_DWORD_17DEC0_spritestr = v12;
			x_DWORD_17DE38str.x_DWORD_17DEC4 = v13x;
			return 0;
		}
		v11y++;
	} while (str_E2516[v11y].word_2);
	return 0;
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// D41A4: using guessed type int x_DWORD_D41A4;
// 17DB8E: using guessed type char x_BYTE_17DB8E;
// 17DEC0: using guessed type int (int)x_DWORD_17DEC0;
// 17DEC4: using guessed type int (int)x_DWORD_17DEC4;
// 17DEC8: using guessed type int x_DWORD_17DEC8;
// 17DECC: using guessed type int x_DWORD_17DECC;
// 17DED4: using guessed type int (int)x_DWORD_17DED4;
// 17DEE4: using guessed type int x_DWORD_17DEE4_mouse_position;
// 17DEEE: using guessed type __int16 x_WORD_17DEEE_mouse_buttons;

//----- (0007E800) --------------------------------------------------------
char sub_7E800(type_WORD_E1F84* a1x)//25f800
{
	char result; // al

	result = SaveGameDialog_78730(a1x);
	if (result)
		a1x->dword_4 = 0;
	return result;
}

//----- (00078730) --------------------------------------------------------
char SaveGameDialog_78730(type_WORD_E1F84* a1x)//259730
{
	//signed __int16* v1; // ebx
	int v2; // edx
	//int v3; // eax
	char* v4; // esi
	char* v5; // edi
	//char v6; // al
	//char v7; // al
	FILE* file1; // esi
	char* v9; // edi
	char* v10; // esi
	//char v11; // al
	//char v12; // al
	FILE* file2; // esi
	//x_WORD *v14; // eax
	int v14x;
	signed int j; // esi
	char* v16; // edi
	//unsigned __int8 v17; // ST10_1
	__int16 v18; // ST0C_2
	__int16 v19; // ST08_2
	signed __int16 v20; // ax
	int v21; // eax
	//unsigned __int8 v22; // ST10_1
	//__int16 v23; // ST0C_2
	//__int16 v24; // ST08_2
	__int16 v25; // ax
	char* v26; // esi
	char* v27; // edi
	//char v28; // al
	//char v29; // al
	//int v30; // eax
	//int v31; // edi
	unsigned __int8 v32; // si
	__int16 v33; // ST0C_2
	__int16 v34; // ax
	//int v35; // eax
	//int v36; // eax
	unsigned __int8 v37; // ST10_1
	char* v38; // eax
	char* v39; // ST0C_4
	char* v40; // ST08_4
	__int16 v41; // ST04_2
	__int16 v42; // ax
	int v43; // edx
	char* v45; // [esp-4h] [ebp-46h]
	//char v46; // [esp+0h] [ebp-42h]
	type_WORD_E1F84 v47x; // [esp+50h] [ebp+Eh]
	//__int16 v48; // [esp+5Ah] [ebp+18h]//5
	//__int16 v49; // [esp+5Ch] [ebp+1Ah]//6
	//__int16 v50; // [esp+5Eh] [ebp+1Ch]//7
	//__int16 v51; // [esp+60h] [ebp+1Eh]//8
	//char v52; // [esp+7Ch] [ebp+3Ah]
	int v53; // [esp+80h] [ebp+3Eh]
	int v54; // [esp+84h] [ebp+42h]
	int v55; // [esp+88h] [ebp+46h]
	char* v56; // [esp+8Ch] [ebp+4Ah]
	//int v57; // [esp+90h] [ebp+4Eh]
	//int v58; // [esp+94h] [ebp+52h]
	__int16 v59; // [esp+98h] [ebp+56h]
	__int16 result; // [esp+9Ch] [ebp+5Ah]
	int v61; // [esp+A0h] [ebp+5Eh]
	int v62; // [esp+A4h] [ebp+62h]
	int v63; // [esp+A8h] [ebp+66h]
	int v64; // [esp+ACh] [ebp+6Ah]
	int i; // [esp+B0h] [ebp+6Eh]
	int k; // [esp+B4h] [ebp+72h]
	unsigned __int8 v67; // [esp+B8h] [ebp+76h]
	unsigned __int8 v68; // [esp+BCh] [ebp+7Ah]

	//fix it
	v43 = 0;
	//fix it

	result = 0;
	v68 = getPaletteIndex_5BE80(x_DWORD_17DE38str.x_DWORD_17DE38x, 0x3Fu, 0x3Fu, 0x3Fu);
	v67 = getPaletteIndex_5BE80(x_DWORD_17DE38str.x_DWORD_17DE38x, 0x16u, 0x10u, 9u);
	//v1 = a1x->word_26;//a1 + 13;
	if (x_DWORD_17DE38str.x_WORD_17DF04 == -1)
	{
		for (i = 1; i < 9; i++)
		{
			v2 = i;
			//v3 = 43 * (i - 1);
			v4 = x_DWORD_E9C4C_langindexbuffer[414];//(char *)x_DWORD_EA2C4; //Empty
			v5 = &x_DWORD_17DE38str.xx_BYTE_17DF14[(i - 1)][0];
			v45 = &x_DWORD_17DE38str.xx_BYTE_17DF14[(i - 1)][0];
			strcpy(v5, v4);
			/*do
			{
				v6 = v4[0];
				v5[0] = v4[0];
				if (!v6)
					break;
				v7 = v4[1];
				v4 += 2;
				v5[1] = v7;
				v5 += 2;
			} while (v7);*/
			v56 = v45;
			std::string saveGameFilePath = GetSaveGameFile(gameFolder, v2);
			file1 = DataFileIO::CreateOrOpenFile(saveGameFilePath.c_str(), 512);
			if (file1 != NULL)
			{
				DataFileIO::Read(file1, (uint8_t*)&v54, 4);
				if (v54 == -9)
					DataFileIO::Read(file1, (uint8_t*)v56, 20);
				DataFileIO::Close(file1);
			}
		}
		x_DWORD_17DE38str.x_WORD_17DF04 = 0;
	}
	v59 = DrawScrollDialog_7BF20(&a1x->str_26);
	if (v59)
	{
		ClearScrollDialogVars_7C020(&a1x->str_26);
		if (v59 == 1 && x_DWORD_17DE38str.x_WORD_17DF04 > 0)
		{
			v9 = &x_DWORD_17DE38str.xx_BYTE_17DF14[(x_DWORD_17DE38str.x_WORD_17DF04 - 1)][0];
			v10 = v9 + 20;
			strcpy(v9, v10);
			/*do
			{
				v11 = v10[0];
				v9[0] = v10[0];
				if (!v11)
					break;
				v12 = v10[1];
				v10 += 2;
				v9[1] = v12;
				v9 += 2;
			} while (v12);*/
			std::string saveGameFilePath = GetSaveGameFile(gameFolder, x_DWORD_17DE38str.x_WORD_17DF04);
			file2 = DataFileIO::CreateOrOpenFile(saveGameFilePath.c_str(), 546);
			if (file2 != NULL)
			{
				//v14 = (x_WORD*)unk_E17CC_0x194;
				v14x = 0;
				v54 = -9;
				v55 = 0;
				while (mapScreenPortals_E17CC[v14x].viewPortPosX_4 && mapScreenPortals_E17CC[v14x].activated_18 != 2)
				{
					//v14 += 11;
					v14x++;
					v55++;
				}
				v53 = D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dw_w_b_0_2BDE_11230.byte[2];
				WriteFile_98CAA(file2, (uint8_t*)&v54, 4);
				WriteFile_98CAA(file2, (uint8_t*)&x_DWORD_17DE38str.xx_BYTE_17DF14[(x_DWORD_17DE38str.x_WORD_17DF04 - 1)][0], 20);
				WriteFile_98CAA(file2, (uint8_t*)x_D41A0_BYTEARRAY_4_struct.player_name_57ar, 32);
				WriteFile_98CAA(file2, (uint8_t*)x_D41A0_BYTEARRAY_4_struct.savestring_89, 32);
				WriteFile_98CAA(file2, (uint8_t*)&secretMapScreenPortals_E2970, 102);
				WriteFile_98CAA(file2, (uint8_t*)&D41A0_0.m_GameSettings, 16);
				WriteFile_98CAA(file2, (uint8_t*)&v55, 4);
				WriteFile_98CAA(file2, (uint8_t*)&v53, 4);
				WriteFile_98CAA(file2, (uint8_t*)&D41A0_0.array_0x2BDE[0].dword_0x3E6_2BE4_12228.str_611, 505);
				WriteFile_98CAA(file2, (uint8_t*)x_DWORD_17DBC8x, 500);
				WriteFile_98CAA(file2, (uint8_t*)x_DWORD_17DDBCx, 100);
				DataFileIO::Close(file2);
			}
		}
		x_DWORD_17DE38str.x_WORD_17DF04 = -1;
		result = 1;
	}
	for (j = 1; j < 9; j++)
	{
		DrawHelpText_6FC50(1);
		if (a1x->str_26.word_36_5 > 16 * j + 3 * sub_6FC30_get34_height())//0160:259AB7
		{
			sprintf(printbuffer, "%d.", j);
			v62 = 16 * j;
			v16 = &x_DWORD_17DE38str.xx_BYTE_17DF14[(j - 1)][0];
			if (j == x_DWORD_17DE38str.x_WORD_17DF04)
			{//draw edtited text line loc 78b10
				v20 = a1x->str_26.x1_26_0;
				v21 = 16 * j + 20;
				v63 = v20 + 20;
				LOWORD(v21) = a1x->str_26.y1_28_1;
				//v57 = v68;
				//v58 = (signed __int16)(v21 + 16 + v62);
				v61 = v21 + 16 + v62;
				DrawText_2BC10(printbuffer, v63, v21 + 16 + v62, v68);//draw number
				//v22 = v57;
				//v23 = v58;
				//v24 = v63 + 3 * sub_6FC10_letter_width();
				DrawText_2BC10(v16 + 20, v63 + 3 * sub_6FC10_letter_width(), v21 + 16 + v62, v68);//draw text
			}
			else
			{
				//v57 = v67;
				DrawText_2BC10(printbuffer, a1x->str_26.x1_26_0 + 20, v62 + a1x->str_26.y1_28_1 + 16, v67);//draw number
				//v17 = v67;
				v18 = v62 + a1x->str_26.y1_28_1 + 16;
				v19 = a1x->str_26.x1_26_0 + 20 + 3 * sub_6FC10_letter_width();
				DrawText_2BC10(v16, v19, v18, v67);//draw text
			}
		}
	}
	if (a1x->str_26.word_36_5 >= a1x->str_26.word_34_4)
	{
		for (k = 1; k < 9; k++)
		{
			v47x.xmin_10 = a1x->str_26.x1_26_0 + 20;//x pos
			v25 = a1x->str_26.y1_28_1;
			v64 = 16 * k;
			v47x.sizey_16 = 16;//y size
			v47x.sizex_14 = 90;//x size
			v47x.ymin_12 = v25 + 16 + 16 * k;//y pos
			/*
			v47 =355150
			6801 0000 2000 9ff33c00620041009051100088513500051100000
			*/
			if (InRegion_7B200(&v47x, x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx, x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony))
			{
				if (x_DWORD_17DE38str.x_WORD_17DEEE_mouse_buttons & 1)
				{
					x_DWORD_17DE38str.x_WORD_17DF04 = k;//clicked on first save
					v26 = &x_DWORD_17DE38str.xx_BYTE_17DF14[(k - 1)][0];
					v27 = v26 + 20;
					/*do
					{
					  //v28 = v26[0];
					  v27[0] = v26[0];
					  if ( !v26[0])
						break;
					  v29 = v26[1];
					  v27[1] = v26[1];
					  v26 += 2;
					  v27 += 2;
					}
					while ( v29 );*/
					strcpy((char*)v27, (const char*)v26);
					//v30 = 43 * (x_WORD_17DF04 - 1);
					x_DWORD_17DE38str.xx_BYTE_17DF14[(x_DWORD_17DE38str.x_WORD_17DF04 - 1)][40] = 0;
					//LOWORD(v30) = *v1;
					//v30 += 20;
					v63 = a1x->str_26.x1_26_0 + 20;
					//LOWORD(v30) = a1[14];
					v61 = a1x->str_26.y1_28_1 + 16 + v64;
					x_DWORD_17DE38str.xx_BYTE_17DF14[(x_DWORD_17DE38str.x_WORD_17DF04 - 1)][42] = 0;
				}
				else if (k != x_DWORD_17DE38str.x_WORD_17DF04)
				{//draw text highlight
					//v31 = 43 * (k - 1);
					sprintf(printbuffer, "%d.", k);
					v32 = v68;
					DrawText_2BC10(printbuffer, a1x->str_26.x1_26_0 + 20, v64 + a1x->str_26.y1_28_1 + 16, v68);
					v33 = v64 + a1x->str_26.y1_28_1 + 16;
					v34 = sub_6FC10_letter_width();
					DrawText_2BC10(&x_DWORD_17DE38str.xx_BYTE_17DF14[(k - 1)][0], a1x->str_26.x1_26_0 + 20 + 3 * v34, v33, v32);
				}
			}
		}
		if (x_DWORD_17DE38str.x_WORD_17DF04)
		{
			DrawHelpText_6FC50(1);
			if (x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode != 1 && x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode != 28 && x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode != 14
				|| /*(v35 = 43 * (x_DWORD_17DE38str.x_WORD_17DF04 - 1),*/ x_DWORD_17DE38str.xx_BYTE_17DF14[(x_DWORD_17DE38str.x_WORD_17DF04 - 1)][42]/*)*/)
			{
				if (x_DWORD_17DE38str.x_BYTE_17DF11_last_key_status)
				{
					if (sub_7C200(x_DWORD_17DE38str.x_BYTE_17DF11_last_key_status))
					{
						//v36 = 43 * (x_DWORD_17DE38str.x_WORD_17DF04 - 1);
						if (!x_DWORD_17DE38str.xx_BYTE_17DF14[(x_DWORD_17DE38str.x_WORD_17DF04 - 1)][42])
						{
							x_DWORD_17DE38str.xx_BYTE_17DF14[(x_DWORD_17DE38str.x_WORD_17DF04 - 1)][42] = 1;
							sprintf((char*)&x_DWORD_17DE38str.xx_BYTE_17DF14[(x_DWORD_17DE38str.x_WORD_17DF04 - 1)][20], "%c", x_DWORD_17DE38str.x_BYTE_17DF11_last_key_status);
							x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode = 0;
							x_DWORD_17DE38str.x_BYTE_17DF11_last_key_status = 0;
						}
					}
				}
			}
			else
			{
				x_DWORD_17DE38str.xx_BYTE_17DF14[(x_DWORD_17DE38str.x_WORD_17DF04 - 1)][42] = 1;
			}
			v37 = v68;
			v38 = &x_DWORD_17DE38str.xx_BYTE_17DF14[(x_DWORD_17DE38str.x_WORD_17DF04 - 1)][0];
			v39 = v38 + 40;
			v40 = v38 + 20;
			v41 = v61;
			v42 = sub_6FC10_letter_width();
			sub_7F6A0((signed __int16)(v63 + 3 * v42), v43, v63 + 3 * v42, v41, v40, (int8_t*)v39, v37);
		}
	}
	return result;
}
// 8E3D5: using guessed type x_DWORD sprintf(x_DWORD, const char *, ...);
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA2C4: using guessed type int x_DWORD_EA2C4;
// 17DBC8: using guessed type int x_DWORD_17DBC8[];
// 17DDBC: using guessed type int x_DWORD_17DDBC[];
// 17DE38: using guessed type int x_DWORD_17DE38;
// 17DEE4: using guessed type int x_DWORD_17DEE4_mouse_position;
// 17DEEE: using guessed type __int16 x_WORD_17DEEE_mouse_buttons;
// 17DF04: using guessed type __int16 x_WORD_17DF04;
// 17DF10: using guessed type char x_BYTE_17DF10_get_key_scancode;
// 17DF11: using guessed type char x_BYTE_17DF11_last_key_status;

//----- (0007E820) --------------------------------------------------------
char sub_7E820(type_WORD_E1F84* a1x)//25f820
{
	char result; // al

	result = LoadGameDialog_780F0(a1x);
	if (result)
		a1x->selected_8 = 0;
	return result;
}

int debugcounter_7D400 = 0;
//----- (0007D400) --------------------------------------------------------
void sub_7D400_draw_texts_and_play_sounds(int  /*a1*/, __int16 a2, __int16 a3, char a4)//25e400
{
	//x_WORD *v4; // ebx
	int v4x;
	int v5; // edx
	//__int16 v6; // cx
	//uint8_t* i; // ebx
	int ix;
	unsigned __int8 v8; // al
	char v9; // cl
	//x_WORD *jj; // ebx
	unsigned __int16 v11; // ax
	int v12; // edx
	char v13; // dl
	int v14; // edx
	int v15; // edx
	int16_t v16; // eax
	int v17; // eax
	int v18x; // edi
	//char *v20; // eax
	int v20x;
	//char *v21; // edx
	int v21x;
	//x_WORD *v22; // eax
	int v22x;
	__int16 v23; // bx
	//signed __int16 *k; // eax
	//uint16_t v25[18]; // [esp+0h] [ebp-54h]
	type_E24BCx v25x[2];
	//__int16 v26; // [esp+2h] [ebp-52h] v25[1]
	//__int16 v27; // [esp+4h] [ebp-50h] v25[2]
	//__int16 v28; // [esp+6h] [ebp-4Eh] v25[3]
	//__int16 v29; // [esp+8h] [ebp-4Ch] v25[4]
	//__int16 v30; // [esp+Ch] [ebp-48h] v25[5]
	//__int16 v31; // [esp+Eh] [ebp-46h] v25[6]
	//int16_t v32; // [esp+10h] [ebp-44h]
	//int v33; // [esp+24h] [ebp-30h]//22
	int8_t v33x[22];
	//__int16 v34; // [esp+30h] [ebp-24h]//v33x[0xc]
	//__int16 v35; // [esp+32h] [ebp-22h]//v33x[0xe]
	//int v36; // [esp+34h] [ebp-20h]//v33x[0x10]

	posistruct2_t* v37x; // [esp+3Ch] [ebp-18h]
	//uint8_t* v38; // [esp+40h] [ebp-14h]
	posistruct2_t* v38x; // [esp+40h] [ebp-14h]
	posistruct_t* v38s;
	int v39; // [esp+44h] [ebp-10h]
	//x_WORD *v40; // [esp+48h] [ebp-Ch]
	//char *v41; // [esp+4Ch] [ebp-8h]
	int v40x;
	int v41x;
	//int v42; // [esp+50h] [ebp-4h]

	//fix it
	v5 = 0;
	v12 = 0;
	v14 = 0;
	v15 = 0;
	//fix it

	debugcounter_7D400++;

	//v42 = -1;
	//v4 = (x_WORD*)x_BYTE_E25ED_db;
	v4x = 0;
	v41x = -1;
	v40x = -1;
	v39 = j___clock();
	while (x_BYTE_E26C8_str[v4x].word_8)
	{
		//if (x_WORD_E1964x[0x21E] != 1 || x_BYTE_E25ED_db_str[v4x].word_12 != 85 && x_BYTE_E25ED_db_str[v4x].word_12 != 86)
		if (mapScreenPortals_E17CC[24].activated_18 != 1 || x_BYTE_E26C8_str[v4x].word_12 != 85 && x_BYTE_E26C8_str[v4x].word_12 != 86)
			sub_81CA0(a2, v5, a2, a3, &x_BYTE_E26C8_str[v4x]);
		//v4 += 15;
		v4x++;
	}
	//VGA_Debug_Blit(640, 480, pdwScreenBuffer_351628);
	//for (i = unk_E17CC_0x194; *((x_WORD *)i + 2); i += 22)//draw new game flag
	//mapScreenPortals_E17CC
	for (ix = 0; mapScreenPortals_E17CC[ix].viewPortPosX_4; ix++)//draw new game flag
	{
		v8 = mapScreenPortals_E17CC[ix].activated_18;
		if (v8 >= 1u)
		{
			if (v8 <= 1u)
			{
				//v41 = (char*)i;
				v41x = ix;
				sub_7E5A0_pre_draw(a2, v5, a2, a3, (uint8_t*)&mapScreenPortals_E17CC[ix], 37, 43);
			}
			else if (v8 == 2)
			{
				LOBYTE(v5) = a4;
				if (a4 == 3 || a4 == 5)
				{
					v9 = mapScreenPortals_E17CC[ix].byte_19;
					//v40 = (x_WORD*)i;
					v40x = ix;
					if (v9)
					{
						if (v9 == 1)
						{
							if (sub_7E5A0_pre_draw(a2, v5, a2, a3, (uint8_t*)&mapScreenPortals_E17CC[ix], 70, 83))
							{
								mapScreenPortals_E17CC[ix].word_16 = 33;
								mapScreenPortals_E17CC[ix].byte_19 = 2;
							}
						}
						else
						{
							sub_7E5A0_pre_draw(a2, v5, a2, a3, (uint8_t*)&mapScreenPortals_E17CC[ix], 33, 35);
						}
					}
					else
					{
						//Play Level Portal Opening Sound
						sub_8F100_sound_proc19(0, 41, 127, 64, 0x64u, 0, 2u);
						mapScreenPortals_E17CC[ix].byte_19 = 1;
					}
				}
			}
		}
		if (mapScreenPortals_E17CC[ix].activated_18 == 2)
			break;
	}
	//VGA_Debug_Blit(640, 480, pdwScreenBuffer_351628);
	//for (jj = x_WORD_E2970; jj[6]; jj = (x_WORD *)((char *)jj + 17))
	for (int jjx = 0; secretMapScreenPortals_E2970[jjx].activated_12; jjx++)
	{
		v11 = secretMapScreenPortals_E2970[jjx].activated_12;
		if (v11 < 1u)
			continue;
		if (v11 <= 1u)
		{
			memset(v33x, 0, 22);
			*(int32_t*)v33x = secretMapScreenPortals_E2970[jjx].dword_0;
			LOWORD(*(x_DWORD*)&v33x[0x10]) = secretMapScreenPortals_E2970[jjx].word_14;
			*(int16_t*)&v33x[0xc] = secretMapScreenPortals_E2970[jjx].word_8;
			*(int16_t*)&v33x[0xe] = secretMapScreenPortals_E2970[jjx].word_10;
			sub_7E5A0_pre_draw(a2, v12, a2, a3, (uint8_t*)v33x, 305, 311);
		LABEL_40:
			secretMapScreenPortals_E2970[jjx].dword_0 = *(x_DWORD*)v33x;
			secretMapScreenPortals_E2970[jjx].word_14 = *(x_DWORD*)&v33x[0x10];
			continue;
		}
		if (v11 == 2 && a4 == 3)
		{
			v13 = secretMapScreenPortals_E2970[jjx].byte_16;
			if (!v13)
			{
				sub_8F100_sound_proc19(0, 41, 127, 64, 0x64u, 0, 2u);
				secretMapScreenPortals_E2970[jjx].byte_16 = 1;
				continue;
			}
			if (v13 != 1)
			{
				memset(v33x, 0, 22);
				*(x_DWORD*)v33x = secretMapScreenPortals_E2970[jjx].dword_0;
				LOWORD(*(x_DWORD*)&v33x[0x10]) = secretMapScreenPortals_E2970[jjx].word_14;
				*(int16_t*)&v33x[0xc] = secretMapScreenPortals_E2970[jjx].word_8;
				*(int16_t*)&v33x[0xe] = secretMapScreenPortals_E2970[jjx].word_10;
				sub_7E5A0_pre_draw(a2, v15, a2, a3, (uint8_t*)v33x, 270, 272);
				goto LABEL_40;
			}
			memset(v33x, 0, 22);
			*(x_DWORD*)v33x = secretMapScreenPortals_E2970[jjx].dword_0;
			LOWORD(*(x_DWORD*)&v33x[0x10]) = secretMapScreenPortals_E2970[jjx].word_14;
			*(int16_t*)&v33x[0xc] = secretMapScreenPortals_E2970[jjx].word_8;
			*(int16_t*)&v33x[0xe] = secretMapScreenPortals_E2970[jjx].word_10;
			if (sub_7E5A0_pre_draw(a2, v14, a2, a3, (uint8_t*)v33x, 70, 83))
			{
				secretMapScreenPortals_E2970[jjx].word_14 = 270;
				secretMapScreenPortals_E2970[jjx].byte_16 = 2;
			}
			else
			{
				secretMapScreenPortals_E2970[jjx].word_14 = *(x_DWORD*)&v33x[0x10];
			}
			secretMapScreenPortals_E2970[jjx].dword_0 = *(x_DWORD*)v33x;
		}
	}
	//VGA_Debug_Blit(640, 480, pdwScreenBuffer_351628);
	//LOBYTE(j) = a4;
	if (a4 == 3 && x_D41A0_BYTEARRAY_4_struct.byteindex_10 == 1)
	{
		v38x = x_DWORD_17DE38str.x_DWORD_17DEC0;
		v37x = x_DWORD_17DE38str.x_DWORD_17DEC4;
		x_DWORD_17DE38str.x_DWORD_17DEC0 = x_DWORD_17DE38str.x_DWORD_17DEC8;
		x_DWORD_17DE38str.x_DWORD_17DEC4 = x_DWORD_17DE38str.x_DWORD_17DECC;

		v38s = xy_DWORD_17DEC0_spritestr;//fixed
		xy_DWORD_17DEC0_spritestr = xy_DWORD_17DEC8_spritestr;//fixed
		/*
		posistruct_t* xy_DWORD_17DED4_spritestr;
posistruct_t* xy_DWORD_17DEC0_spritestr;
posistruct_t* xy_DWORD_17DEC8_spritestr;
		*/

		//HIWORD(v16) = HIWORD(x_DWORD_17DECC);
		if (x_DWORD_17DB70str.x_WORD_17DB74 == 0)
		{
			if (!x_DWORD_17DB70str.x_WORD_17DB74)
			{
				v16 = v40x != -1;
				//BYTE1(v16) = 0;
				v17 = v16 + 1;
				x_DWORD_17DB70str.x_WORD_17DB74 = v17;
				x_DWORD_17DB70str.x_DWORD_17DB70 = j___clock();
			}
		}
		else if (x_DWORD_17DB70str.x_WORD_17DB74 == 1u)
		{
			if (v41x != -1)
			{
				if (3 == x_DWORD_17DB70str.x_BYTE_17DB8F)
				{
					memset(v25x, 0, 36);
					//qmemcpy(v25x, unk_E24F2, 0x10u+2u);
					v25x[0]= str_E24F2[0];
					//qmemcpy(&v32, &unk_E24FE, 2u);
					v25x[0].word_2 = mapScreenPortals_E17CC[v41x].portalPosX_12 - 80 - a2;
					v25x[0].word_4 = mapScreenPortals_E17CC[v41x].portalPosY_14 - 60 - a3;
					v25x[0].word_6 = mapScreenPortals_E17CC[v41x].portalPosX_12 + 16 - a2;
					v25x[0].word_8 = mapScreenPortals_E17CC[v41x].portalPosY_14 - 60 - a3;
					v25x[0].word_12 = mapScreenPortals_E17CC[v41x].portalPosX_12 + 16 - a2;
					v25x[0].word_14 = mapScreenPortals_E17CC[v41x].portalPosY_14 - 4 - a3;
					v25x[0].word_0 = 464;
					v18x = v40x;
					sub_7E840_draw_textbox_with_line(v25x, 238, 264);
					if (v18x != -1)
					{
						if ((v39 - x_DWORD_17DB70str.x_DWORD_17DB70) / 0x64u > 8)
						{
							x_DWORD_17DB70str.x_DWORD_17DB70 = v39;
							x_DWORD_17DB70str.x_WORD_17DB74 = 2;
						}
					}
				}
			}
		}
		else if (x_DWORD_17DB70str.x_WORD_17DB74 == 2)
		{
			if (v40x != -1)
			{
				if (x_DWORD_17DB70str.x_BYTE_17DB8F == 3)
				{
					memset(v25x, 0, 36);
					v25x[0].word_2 = mapScreenPortals_E17CC[v40x].portalPosX_12 - 80 - a2;
					v25x[0].word_4 = mapScreenPortals_E17CC[v40x].portalPosY_14 - 60 - a3;
					v25x[0].word_6 = mapScreenPortals_E17CC[v40x].portalPosX_12 + 16 - a2;
					v25x[0].word_8 = mapScreenPortals_E17CC[v40x].portalPosY_14 - 60 - a3;
					v25x[0].word_12 = mapScreenPortals_E17CC[v40x].portalPosX_12 + 16 - a2;
					v25x[0].word_14 = mapScreenPortals_E17CC[v40x].portalPosY_14 - 4 - a3;
					v25x[0].word_0 = 465;
					sub_7E840_draw_textbox_with_line(v25x, 238, 264);
					if (v41x != -1)
					{
						if ((v39 - x_DWORD_17DB70str.x_DWORD_17DB70) / 0x64u > 8)
						{
							x_DWORD_17DB70str.x_DWORD_17DB70 = v39;
							x_DWORD_17DB70str.x_WORD_17DB74 = 1;
						}
					}
				}
			}
		}
		x_DWORD_17DE38str.x_DWORD_17DEC0 = v38x;
		x_DWORD_17DE38str.x_DWORD_17DEC4 = v37x;
		xy_DWORD_17DEC0_spritestr = v38s;
	}
	//VGA_Debug_Blit(640, 480, pdwScreenBuffer_351628);
	if (x_DWORD_17DB70str.x_BYTE_17DB8E)
		return;

	if (x_DWORD_17DB70str.x_WORD_17DB8A != -1)
	{
		//v20 = (char *)unk_E17CC_0x194;
		v20x = 0;
		v21x = -1;
		while (mapScreenPortals_E17CC[v20x].viewPortPosX_4)
		{
			if (mapScreenPortals_E17CC[v20x].activated_18 == 1)
				v21x = v20x;
			//v20 += 22;
			v20x++;
		}
		//v22 = (x_WORD*)unk_E17CC_0x194;
		v22x = 0;
		v23 = 0;
		while (mapScreenPortals_E17CC[v22x].viewPortPosX_4)
		{
			if (v23 == x_DWORD_17DB70str.x_WORD_17DB8A)
			{
				sub_7DA70(mapScreenPortals_E17CC[v22x].portalPosX_12, mapScreenPortals_E17CC[v22x].portalPosY_14, mapScreenPortals_E17CC[v21x].portalPosX_12, mapScreenPortals_E17CC[v21x].portalPosY_14, a2, a3);
				return;
			}
			//v22 += 11;
			v22x++;
			//++v23;
			v23++;
		}
		//for (k = x_WORD_E2970; k[6]; k = (signed __int16 *)((char *)k + 17))
		for (int kk = 0; secretMapScreenPortals_E2970[kk].activated_12; kk++)
		{
			if (x_DWORD_17DB70str.x_WORD_17DB8A == secretMapScreenPortals_E2970[kk].levelNumber_6)
			{
				sub_7DA70(secretMapScreenPortals_E2970[kk].word_8, secretMapScreenPortals_E2970[kk].word_10, mapScreenPortals_E17CC[secretMapScreenPortals_E2970[kk].word_4].portalPosX_12, mapScreenPortals_E17CC[secretMapScreenPortals_E2970[kk].word_4].portalPosY_14, a2, a3);
				return;
			}
		}
		return;
	}
	//VGA_Debug_Blit(640, 480, pdwScreenBuffer_351628);
	if ((v41x != -1) && (v40x != -1))
	{
		sub_7DA70(mapScreenPortals_E17CC[v41x].portalPosX_12, mapScreenPortals_E17CC[v41x].portalPosY_14, mapScreenPortals_E17CC[v40x].portalPosX_12, mapScreenPortals_E17CC[v40x].portalPosY_14, a2, a3);
		return;
	}
	//VGA_Debug_Blit(640, 480, pdwScreenBuffer_351628);
	if ((v41x != -1) && (v40x != -1))
		return;

	if (v41x >= 0) {
		sub_7DA70(mapScreenPortals_E17CC[v41x].portalPosX_12, mapScreenPortals_E17CC[v41x].portalPosY_14, mapScreenPortals_E17CC[23].portalPosX_12, mapScreenPortals_E17CC[23].portalPosY_14, a2, a3);
	}
}

//----- (0007EAE0) --------------------------------------------------------
int NewGameDraw_7EAE0(int16_t* posx, int16_t* posy, __int16* a3, __int16* a4, int8_t* a5, x_WORD* a6)//25fae0 //level select
{
	char v6; // al
	int v7x; // eax
	Type_SecretMapScreenPortals_E2970* v8x; // eax
	int v9x;
	int v12x;
	int v13; // ecx
	__int16 v14; // dx
	int v15x;
	__int16 v16; // cx
	int v18; // ecx
	int jx = 0;
	int v20; // edx
	//int v22; // edx
	__int16 v23; // cx
	//int v25; // ST18_4
	//int v26; // ST14_4
	//int v28; // edx
	//int v32; // edx
	signed int v33; // eax
	__int16 v34; // dx
	char v35; // cl
	__int16 v36; // ax
	//int v37; // edx
	signed int v38; // eax
	signed int v39; // eax
	char v42; // cl
	int v43; // ecx
	int v44x;
	Type_SecretMapScreenPortals_E2970* v46x; // eax
	__int16 v48; // ST10_2
	__int16 v49; // ST0C_2
	__int16 v50; // ST08_2
	__int16 v51; // ax
	__int16 v54; // [esp-Ch] [ebp-68h]
	type_WORD_E1F84 v55x; // [esp+0h] [ebp-5Ch]
	//int v60; // [esp+2Ch] [ebp-30h]
	int v61x;
	int v63x;
	int v64x;
	int v65; // [esp+40h] [ebp-1Ch]
	int v66; // [esp+44h] [ebp-18h]
	int v67; // [esp+48h] [ebp-14h]
	int v68; // [esp+4Ch] [ebp-10h]
	int v69; // [esp+50h] [ebp-Ch]
	int v70x;
	int v71; // [esp+58h] [ebp-4h]

	//fix it
	//v22 = 0;
	//v28 = 0;
	//v32 = 0;
	v33 = 0;
	//v37 = 0;
	v38 = 0;
	v39 = 0;
	//fix it

	v6 = *a5;
	v71 = 0;
	v67 = -1;
	switch (v6)
	{
	case 1://prepare
		v12x = 0;
		v13 = 0;
		v61x = -1;
		while (mapScreenPortals_E17CC[v12x].viewPortPosX_4)
		{
			if (mapScreenPortals_E17CC[v12x].activated_18 == 1)
			{
				*posx = mapScreenPortals_E17CC[v12x].viewPortPosX_4;
				v61x = v12x;
				v14 = mapScreenPortals_E17CC[v12x].viewPortPosY_6;
				v67 = v13;
				*posy = v14;
			}
			v12x++;
			v13++;
		}
		//adress 25fc6f
		if ((v61x != -1) && x_DWORD_17DB70str.x_WORD_17DB8A != -1 && v67 != x_DWORD_17DB70str.x_WORD_17DB8A)
		{
			v15x = 0;
			v16 = 0;
			while (mapScreenPortals_E17CC[v15x].viewPortPosX_4)
			{
				if (v16 == x_DWORD_17DB70str.x_WORD_17DB8A)
				{
					*posx = mapScreenPortals_E17CC[v15x].viewPortPosX_4;
					*posy = mapScreenPortals_E17CC[v15x].viewPortPosY_6;
					*a5 = 3;
					break;
				}
				v15x++;
				v16++;
			}
			if (*a5 != 3)
			{
				for (int ii = 0; secretMapScreenPortals_E2970[ii].activated_12; ii++)
				{
					if (x_DWORD_17DB70str.x_WORD_17DB8A == secretMapScreenPortals_E2970[ii].levelNumber_6)
					{
						*posx = mapScreenPortals_E17CC[secretMapScreenPortals_E2970[ii].word_4].viewPortPosX_4;
						*posy = mapScreenPortals_E17CC[secretMapScreenPortals_E2970[ii].word_4].viewPortPosY_6;
						*a5 = 3;
						break;
					}
				}
			}
		}
		else
		{
			x_DWORD_17DB70str.x_WORD_17DB8A = -1;
		}
		memset(&x_DWORD_17DE28str, 0, 13);
		if (x_DWORD_17DB70str.x_WORD_17DB8A == -1)
		{
			v18 = 0;
			for (jx = 0;mapScreenPortals_E17CC[jx].viewPortPosX_4 != 0; jx++)
			{
				v66 = v18;
				if (mapScreenPortals_E17CC[jx].activated_18 == 2)
				{
					*a3 = mapScreenPortals_E17CC[jx].viewPortPosX_4;
					*a4 = mapScreenPortals_E17CC[jx].viewPortPosY_6;
					if (v66)
					{
						*a5 = 2;
						sub_7E8D0(a6, *a3, *a4, *posx, *posy, 4, 4);
					}
					else
					{
						*posx = mapScreenPortals_E17CC[jx].viewPortPosX_4;
						*posy = mapScreenPortals_E17CC[jx].viewPortPosY_6;
						*a5 = 3;
						if (*posy > 480)
							*posy = 480;
						if (*posx > 640)
							*posx = 640;
						v20 = v66;
						*a5 = 3;
						if (v20 < 24)
						{
							x_DWORD_17DE28str.x_WORD_17DE30_posx = *posx;
							x_DWORD_17DE28str.x_WORD_17DE32_posy = *posy;
							x_DWORD_17DE28str.x_BYTE_17DE34 = ((x_D41A0_BYTEARRAY_4_struct.setting_byte3_24 & 0x40) != 0) + 1;
							x_DWORD_17DE28str.x_DWORD_17DE28 = j___clock();
						}
					}
					break;
				}
				v18 = v66 + 1;
			}
			if (*a5 != 1 || x_DWORD_17DB70str.x_BYTE_17DB8E)
				goto LABEL_66;
		}
		else
		{
			memset(&x_DWORD_17DE28str, 0, 13);
			v23 = 0;
			for (jx = 0; mapScreenPortals_E17CC[jx].viewPortPosX_4 != 0; jx++)
			{
				if (mapScreenPortals_E17CC[jx].activated_18 == 2)
				{
					if (v23 < 24)
					{
						x_DWORD_17DE28str.x_WORD_17DE30_posx = *posx;
						x_DWORD_17DE28str.x_WORD_17DE32_posy = *posy;
						x_DWORD_17DE28str.x_BYTE_17DE34 = ((x_D41A0_BYTEARRAY_4_struct.setting_byte3_24 & 0x40) != 0) + 1;
						x_DWORD_17DE28str.x_DWORD_17DE28 = j___clock();
					}
					break;
				}
				v23++;
			}
		}
		*a5 = 3;
	LABEL_66:
		NewGameSubdraw_81760();//draw cursor
		goto LABEL_92;
	case 2:
		//v25 = *a4;
		//v26 = *a3;
		if (sub_7E9D0(posx, posy, a6))
		{
			*a5 = 3;
			memset(&x_DWORD_17DE28str, 0, 13);
			x_DWORD_17DE28str.x_WORD_17DE30_posx = *posx;
			x_DWORD_17DE28str.x_WORD_17DE32_posy = *posy;
			x_DWORD_17DE28str.x_BYTE_17DE34 = ((x_D41A0_BYTEARRAY_4_struct.setting_byte3_24 & 0x40) != 0) + 1;
			x_DWORD_17DE28str.x_DWORD_17DE28 = j___clock();
		}
		goto LABEL_92;
	case 3:
		if (x_DWORD_17DB70str.x_BYTE_17DB8E)
			goto LABEL_92;
		x_WORD_E29D6_not_movex = 1;
		if (x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx)//mousex>0
		{
			if (x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx < 638)
				goto LABEL_75;
			*posx += x_DWORD_17DB70str.x_WORD_17DB82_shift_step;//shift position by step
		}
		else//mousex==0
		{
			*posx -= x_DWORD_17DB70str.x_WORD_17DB82_shift_step;//shift position by step
		}
		x_WORD_E29D6_not_movex = 0;//add
	LABEL_75:
		if (x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony)//mousey>0
		{
			if (x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony < 478)
				goto LABEL_80;
			*posy += x_DWORD_17DB70str.x_WORD_17DB82_shift_step;//shift position by step
		}
		else//mousey==0
		{
			*posy -= x_DWORD_17DB70str.x_WORD_17DB82_shift_step;//shift position by step
		}
		x_WORD_E29D6_not_movex = 0;//shift right up or down
	LABEL_80:
		if (*posx >= 0)
		{
			if (*posx >= 638)
				*posx = 638;
		}
		else
		{
			*posx = 0;
		}
		if (*posy >= 0)
		{
			if (*posy >= 478)
				*posy = 478;
		}
		else
		{
			*posy = 0;
		}
		if (x_WORD_E29D6_not_movex)
		{
			x_DWORD_17DB70str.x_WORD_17DB82_shift_step = 0;
		}
		else
		{
			x_DWORD_17DB70str.x_WORD_17DB82_shift_step += 4;
			if (x_DWORD_17DB70str.x_WORD_17DB82_shift_step > 24)
				x_DWORD_17DB70str.x_WORD_17DB82_shift_step = 24;
		}
		goto LABEL_92;
	case 4:
		if (x_DWORD_17DB70str.x_WORD_17DB8A > 24)
		{
			v8x = sub_824E0(x_DWORD_17DB70str.x_WORD_17DB8A);
			if (!v8x)
				goto LABEL_10;
			*posx = mapScreenPortals_E17CC[v8x->word_4].viewPortPosX_4;
			v7x = v8x->word_4;
		}
		else
		{
			v7x = x_DWORD_17DB70str.x_WORD_17DB8A;
			*posx = mapScreenPortals_E17CC[v7x].viewPortPosX_4;
		}
		*posy = mapScreenPortals_E17CC[v7x].viewPortPosY_6;
	LABEL_10:
		if (x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode || x_DWORD_17DE38str.x_WORD_17DEEE_mouse_buttons)
		{
			*a5 = 1;
			x_DWORD_17DE38str.x_WORD_17DEEE_mouse_buttons = 0;
			x_DWORD_17DE38str.x_BYTE_17DF11_last_key_status = 0;
			x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode = 0;
		}
		v9x = 0;
		v64x = -1;
		v70x = 0;
		while (mapScreenPortals_E17CC[v9x].viewPortPosX_4)
		{
			if (mapScreenPortals_E17CC[v9x].activated_18 == 2)
			{
				v64x = v9x;
				break;
			}
			v9x++;
			v70x++;
		}
		if (v64x != -1)
		{
			mapScreenPortals_E17CC[v64x - 1].activated_18 = 2;
			v70x = mapScreenPortals_E17CC[v64x].viewPortPosX_4;
			mapScreenPortals_E17CC[v64x].viewPortPosX_4 = 0;
		}
		NewGameSubdraw_81760(/*&mapScreenPortals_E17CC[v9x]*/);
		if (v64x != -1)
		{
			mapScreenPortals_E17CC[v64x - 1].activated_18 = 1;
			mapScreenPortals_E17CC[v64x].viewPortPosX_4 = v70x;
		}
	LABEL_92:
		if (x_WORD_E29D6_not_movex)
		{
			DrawNetGameMapBackground_85C8B(x_DWORD_17DE38str.x_DWORD_17DE64_game_world_map, pdwScreenBuffer_351628, *posx, *posy, 160, 480);//draw game word map
			sub_7D400_draw_texts_and_play_sounds(*posx, *posx, *posy, *a5);//draw helps, cursor, flags
			DrawAndSoundDragonAndFire_81EE0(*posx, *posy); // fair animation
			if (x_DWORD_17DB70str.x_BYTE_17DB8E)
			{
				v38 = sub_80D40_move_graphics_and_play_sounds(*posx, *posy, x_DWORD_17DB70str.x_WORD_17DB84, x_DWORD_17DB70str.x_WORD_17DB86, x_DWORD_17DB70str.x_WORD_17DB88);
				v71 = v38;
				if (v38)
				{
					v38 = 0;
					x_DWORD_17DB70str.x_BYTE_17DB8E = 0;
				}
			}
			sub_85CC3_draw_round_frame((uint16_t*)x_DWORD_17DE38str.x_DWORD_17DE5C_border_bitmap);//maybe round frame
			v34 = *a5;
			if (*a5 == 4)
			{
				v36 = x_DWORD_17DB70str.x_WORD_17DB8A;
				goto LABEL_107;
			}
			if ((x_BYTE)v34 == 5)
			{
				v36 = x_DWORD_17DB70str.x_WORD_17DB8C;
				goto LABEL_107;
			}
		}
		else
		{
			DrawNetGameMapBackground_85C8B(x_DWORD_17DE38str.x_DWORD_17DE64_game_world_map, pdwScreenBuffer_351628, *posx, *posy, 160, 480);
			sub_7D400_draw_texts_and_play_sounds(*posx, *posx, *posy, *a5);//draw helps, cursor, flags
			DrawAndSoundDragonAndFire_81EE0(*posx, *posy);
			if (x_DWORD_17DB70str.x_BYTE_17DB8E)
			{
				v33 = sub_80D40_move_graphics_and_play_sounds(*posx, *posy, x_DWORD_17DB70str.x_WORD_17DB84, x_DWORD_17DB70str.x_WORD_17DB86, x_DWORD_17DB70str.x_WORD_17DB88);
				v71 = v33;
				if (v33)
					x_DWORD_17DB70str.x_BYTE_17DB8E = 0;
			}
			sub_85CC3_draw_round_frame((unsigned __int16*)x_DWORD_17DE38str.x_DWORD_17DE5C_border_bitmap);
			v34 = 1;
			v35 = *a5;
			x_WORD_E29D6_not_movex = 1;
			if (v35 == 4)
			{
				v36 = x_DWORD_17DB70str.x_WORD_17DB8A;
			LABEL_107:
				DrawEndGameTable_82C20(v36);
				goto LABEL_108;
			}
			if (v35 == 5)
			{
				v36 = x_DWORD_17DB70str.x_WORD_17DB8C;
				goto LABEL_107;
			}
		}
	LABEL_108:
		v39 = sub_7E320_draw_bitmaps_and_play_sounds();//four buttons in corners and helps
		v69 = v39;
		if (!x_DWORD_17DB70str.x_BYTE_17DB8E && !v71 && !v39)
		{
			if (*a5 == 3)
			{
				if (x_DWORD_17DE38str.x_BYTE_17DF11_last_key_status == 111 || x_DWORD_17DE38str.x_BYTE_17DF11_last_key_status == 79)
				{
					v42 = x_DWORD_17DE28str.x_BYTE_17DE34;
					if (x_DWORD_17DE28str.x_BYTE_17DE34 == 1)
					{
						v39 = x_DWORD_17DE28str.x_DWORD_17DE2C;
						x_DWORD_17DE28str.x_DWORD_17DE28 = x_DWORD_17DE28str.x_DWORD_17DE2C;
						x_DWORD_17DE28str.x_BYTE_17DE34 = 2;
						x_BYTE_17E09D = v42;
					}
					else if (x_DWORD_17DE28str.x_BYTE_17DE34 == 2)
					{
						v39 = j___clock();
						x_DWORD_17DE28str.x_DWORD_17DE2C = v39;
						x_DWORD_17DE28str.x_BYTE_17DE34 = 1;
						x_DWORD_17DE28str.x_DWORD_17DE28 = v39;
						x_BYTE_17E09D = 0;
					}
				}
				if (x_DWORD_17DE28str.x_BYTE_17DE34 == 1)
				{
					v43 = j___clock();
					x_DWORD_17DE28str.x_DWORD_17DE2C = v43;
					//v60 = 100;
					if ((v43 - x_DWORD_17DE28str.x_DWORD_17DE28) / 0x64u > 0xF)
					{
						x_DWORD_17DE28str.x_DWORD_17DE28 = v43;
						x_DWORD_17DE28str.x_BYTE_17DE34 = 2;
					}
					if (x_DWORD_17DE28str.x_WORD_17DE32_posy < 478)
						v54 = 280;
					else
						v54 = 60;
					//adress 2602a0
					DrawText_80C30(130, v54, 380);//divny text
				}
				else if (x_DWORD_17DE28str.x_BYTE_17DE34 <= 2u || x_DWORD_17DE28str.x_BYTE_17DE34 == 3)
				{
					//adress 2602a7
					DrawText_80C30(0, 0, 0);
				}
			}
		}
		if (v69 == 2 && !v71 && !x_DWORD_17DB70str.x_BYTE_17DB8E)
			v71 = v69;
		if (!v69 && !v71)
		{
			if (*a5 == 3 && !v71)
			{
				if (!x_DWORD_17DB70str.x_BYTE_17DB8E)
				{
					if (x_DWORD_17DE38str.x_WORD_17DEEE_mouse_buttons & 1)
					{
						v44x = 0;
						v68 = 0;
						while (mapScreenPortals_E17CC[v44x].viewPortPosX_4)
						{
							v55x.xmin_10 = mapScreenPortals_E17CC[v44x].portalPosX_12 - *posx;
							v55x.ymin_12 = mapScreenPortals_E17CC[v44x].portalPosY_14 - *posy;
							v55x.sizex_14 = mapScreenPortals_E17CC[v44x].word_8;
							v55x.sizey_16 = mapScreenPortals_E17CC[v44x].word_10;
							if (InRegion_7B200(&v55x, x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx, x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony))
							{
								x_DWORD_17DB70str.x_WORD_17DB84 = mapScreenPortals_E17CC[v44x].portalPosX_12;
								x_DWORD_17DB70str.x_WORD_17DB86 = mapScreenPortals_E17CC[v44x].portalPosY_14;
								x_DWORD_17DB70str.x_WORD_17DB88 = mapScreenPortals_E17CC[v44x].activated_18;
								sub_80D40_move_graphics_and_play_sounds(*posx, *posy, mapScreenPortals_E17CC[v44x].portalPosX_12, mapScreenPortals_E17CC[v44x].portalPosY_14, mapScreenPortals_E17CC[v44x].activated_18);
								x_DWORD_17DB70str.x_BYTE_17DB8E = 1;
								x_D41A0_BYTEARRAY_4_struct.levelnumber_43w = v68;

								if (CommandLineParams.ModeTestRegressionsGame()) {
									//x_D41A0_BYTEARRAY_4_struct.levelnumber_43w = 1;
								}
								if (mapScreenPortals_E17CC[v44x].activated_18 == 1)
									x_D41A0_BYTEARRAY_4_struct.setting_38545 |= 4u;
								v46x = sub_824B0(x_D41A0_BYTEARRAY_4_struct.levelnumber_43w);
								if (v46x && v46x->activated_12 == 2)
									x_D41A0_BYTEARRAY_4_struct.setting_38545 |= 0x10u;
								if (x_D41A0_BYTEARRAY_4_struct.levelnumber_43w == 24)
									x_D41A0_BYTEARRAY_4_struct.setting_38545 |= 0x20u;
								break;
							}
							if (mapScreenPortals_E17CC[v44x].activated_18 == 2)
								break;
							v44x++;// += 22;
							v68++;
						}
						if (!v71 && !x_DWORD_17DB70str.x_BYTE_17DB8E)
						{
							int v47i = 0;
							if (secretMapScreenPortals_E2970[v47i].word_8)
							{
								while (1)
								{
									if (secretMapScreenPortals_E2970[v47i].activated_12 != 3)
									{
										v55x.xmin_10 = secretMapScreenPortals_E2970[v47i].word_8 - *posx;
										v55x.ymin_12 = secretMapScreenPortals_E2970[v47i].word_10 - *posy;
										v55x.sizex_14 = 40;
										v55x.sizey_16 = 40;
										if (InRegion_7B200(&v55x, x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx, x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony))
											break;
									}
									v47i++;
									if (!secretMapScreenPortals_E2970[v47i].word_8)
										goto LABEL_179;
								}
								if (secretMapScreenPortals_E2970[v47i].activated_12 == 1)
									x_D41A0_BYTEARRAY_4_struct.setting_38545 |= 4u;
								x_DWORD_17DB70str.x_WORD_17DB84 = secretMapScreenPortals_E2970[v47i].word_8;
								x_DWORD_17DB70str.x_WORD_17DB86 = secretMapScreenPortals_E2970[v47i].word_10;
								v48 = secretMapScreenPortals_E2970[v47i].word_8;
								v49 = *posy;
								v50 = *posx;
								x_DWORD_17DB70str.x_WORD_17DB88 = 1;
								sub_80D40_move_graphics_and_play_sounds(v50, v49, v48, x_DWORD_17DB70str.x_WORD_17DB86, 1);
								v51 = secretMapScreenPortals_E2970[v47i].levelNumber_6;
								x_DWORD_17DB70str.x_BYTE_17DB8E = 1;
								x_D41A0_BYTEARRAY_4_struct.levelnumber_43w = v51;
							}
						}
					}
					else if (x_DWORD_17DE38str.x_WORD_17DEEE_mouse_buttons & 2)
					{
						LOBYTE(v39) = 0;
						v63x = 0;
						v65 = v39;
						while (mapScreenPortals_E17CC[v63x].viewPortPosX_4)
						{
							v55x.xmin_10 = mapScreenPortals_E17CC[v63x].portalPosX_12 - *posx;
							v55x.ymin_12 = mapScreenPortals_E17CC[v63x].portalPosY_14 - *posy;
							v55x.sizex_14 = mapScreenPortals_E17CC[v63x].word_8;
							v55x.sizey_16 = mapScreenPortals_E17CC[v63x].word_10;
							if (InRegion_7B200(&v55x, x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx, x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony))
							{
								if (mapScreenPortals_E17CC[v63x].activated_18 == 1)
								{
									x_DWORD_17DB70str.x_WORD_17DB8C = v65;
									*a5 = 5;
									x_DWORD_17DE38str.x_WORD_17DEEE_mouse_buttons = 0;
								}
								break;
							}
							v63x++;
							v65++;
						}
						if (*a5 != 5)
						{
							for (int kk = 0; secretMapScreenPortals_E2970[kk].word_8; kk++)
							{
								if (secretMapScreenPortals_E2970[kk].activated_12 != 3)
								{
									v55x.xmin_10 = secretMapScreenPortals_E2970[kk].word_8 - *posx;
									v55x.ymin_12 = secretMapScreenPortals_E2970[kk].word_10 - *posy;
									v55x.sizex_14 = 40;
									v55x.sizey_16 = 40;
									if (InRegion_7B200(&v55x, x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx, x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony))
									{
										if (secretMapScreenPortals_E2970[kk].activated_12 == 1)
										{
											*a5 = 5;
											x_DWORD_17DB70str.x_WORD_17DB8C = secretMapScreenPortals_E2970[kk].levelNumber_6;
										}
										break;
									}
								}
							}
						}
					}
				}
			}
		LABEL_179:
			if (!v71 && x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode == 1)
				v71 = 2;
		}
		return v71;
	case 5:
		if (x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode || x_DWORD_17DE38str.x_WORD_17DEEE_mouse_buttons)
		{
			x_DWORD_17DE38str.x_WORD_17DEEE_mouse_buttons = 0;
			*a5 = 3;
			x_DWORD_17DE38str.x_BYTE_17DF11_last_key_status = 0;
			x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode = 0;
		}
		goto LABEL_92;
	default:
		goto LABEL_92;
	}
}

//----- (0007F7D0) --------------------------------------------------------
int LoadLanguageFile(posistruct2_t** a1x, posistruct2_t** a2x, uint8_t* a3, char* langfilename, posistruct_t* a3dattabindex)//2607d0
{
	//int v4; // edi
	FILE* langfile; // eax
	//FILE* v6; // ebx
	//int v7; // ebx
	//uint8_t* v8; // eax
	//unsigned __int8 v9; // bl
	//char v10; // cl
	//char v12; // [esp+0h] [ebp-5Ch]
	uint16_t langfilelenght; // [esp+50h] [ebp-Ch]
	//FILE* v14; // [esp+54h] [ebp-8h]
	//unsigned __int8 i; // [esp+58h] [ebp-4h]

	//uint8_t a3x[5000];//fix it

	//v4 = 0;
	//fix it
	//v9=0;//fix it
	//fix it
	std::string languageFilePath = GetSubDirectoryFile(cdFolder, "LANGUAGE", langfilename);
	for (uint8_t i = 0; i < 2; i++)//[ebp-4]=354f70
	{
		//v4 = atoi(langfilename +1);
		//v4 = unknown_libname_1_atoi((char*)(a4 + 1));
		langfile = DataFileIO::CreateOrOpenFile(languageFilePath.c_str(), 512);//279f9e
		//v6 = langfile;
		//v14 = langfile;
		if (langfile != NULL)
		{
			langfilelenght = DataFileIO::FileLengthBytes(langfile);
			DataFileIO::Read(langfile, a3, 4773);//2798a7
			DataFileIO::Read(langfile, (a3 + 4773), 12);
			if (x_BYTE_E29E0 || x_DWORD_D41BC_langbuffer)//[2b39e0]00 || [2a51bc]00
				FreeMem_83E80((uint8_t*)x_DWORD_D41BC_langbuffer);
			//v7 = langfilelenght - 4785;
			x_DWORD_D41BC_langbuffer = (char*)sub_83CD0_malloc2(langfilelenght - 4785);
			//x_DWORD_D41BC_langbuffer = (uint8_t*)v8;
			if (x_DWORD_D41BC_langbuffer)
			{
				x_BYTE_E29E0 = 1;
				DataFileIO::Read(langfile, (uint8_t*)x_DWORD_D41BC_langbuffer, langfilelenght - 4785);
			}
			DataFileIO::Close(langfile);
			break;
		}
		//v9 = i;
		char updatedLanguagePath[MAX_PATH];
		sprintf(updatedLanguagePath, "LANGUAGE/%s", langfilename);
		languageFilePath = std::string(updatedLanguagePath);

	}
	*a1x = (posistruct2_t*)(a3 + 4773);//35513c
	//v10 = x_WORD_180660_VGA_type_resolution;
	*a2x = (posistruct2_t*)(a3 + 4785);
	//posistruct_t a3dattab[1000];
	if (x_WORD_180660_VGA_type_resolution & 1)
		sub_98709_create_index_dattab_power(*a1x, (posistruct2_t*)(a3 + 4785), a3, a3dattabindex);
	else
		sub_9874D_create_index_dattab(*a1x, (posistruct2_t*)(a3 + 4785), a3, a3dattabindex);
	sub_5B870_copy_sentence(x_DWORD_D41BC_langbuffer, x_DWORD_E9C4C_langindexbuffer, 471);//fix it //2a51bc //Exit Game
	return atoi(langfilename + 1);
}
// 8E3D5: using guessed type x_DWORD sprintf(x_DWORD, const char *, ...);
// 988DA: using guessed type x_DWORD filelength(x_DWORD);
// 98F9E: using guessed type x_DWORD unknown_libname_1_atoi(x_DWORD);
// D41A4: using guessed type int x_DWORD_D41A4;
// D41BC: using guessed type int x_DWORD_D41BC_langbuffer;
// E29E0: using guessed type char x_BYTE_E29E0;
// 180660: using guessed type __int16 x_WORD_180660_VGA_type_resolution;

//----- (0007F960) --------------------------------------------------------
int sub_7F960(posistruct2_t* a1x, posistruct2_t* a2x, uint8_t* a3, char* langcountstring, posistruct_t* a3dattabindex)
//int sub_7F7D0(uint8_t** a1, uint8_t** a2, uint8_t* a3, char* langfilename, posistruct_t* a3dattabindex)//2607d0
{
	uint16_t langcount; // esi
	FILE* langfile; // eax
	//FILE* v6; // ebx
	//FILE* v7; // edi
	int v8; // ebx
	//uint8_t* v9; // eax
	unsigned __int8 v10; // bl
	//char v12; // [esp+0h] [ebp-58h]
	int v13; // [esp+50h] [ebp-8h]
	unsigned __int8 i; // [esp+54h] [ebp-4h]
	langcount = 0;

	std::string languagePath = GetSubDirectoryFile(cdFolder, "LANGUAGE", langcountstring);
	for (i = 0; i < 2u; i = v10 + 1)
	{
		langcount = atoi((langcountstring + 1));
		langfile = DataFileIO::CreateOrOpenFile(languagePath.c_str(), 512);
		//v6 = v5;
		//v7 = v5;
		if (langfile != NULL)
		{
			v13 = DataFileIO::FileLengthBytes(langfile);
			DataFileIO::Read(langfile, a3, 4773);
			DataFileIO::Read(langfile, a3 + 4773, 12);
			if (x_BYTE_E29E0 || x_DWORD_D41BC_langbuffer)
				FreeMem_83E80((uint8_t*)x_DWORD_D41BC_langbuffer);
			v8 = v13 - 4785;
			//v9 = (uint8_t*)sub_83CD0_malloc2(v13 - 4785);
			x_DWORD_D41BC_langbuffer = (char*)sub_83CD0_malloc2(v13 - 4785);
			if (x_DWORD_D41BC_langbuffer)
			{
				x_BYTE_E29E0 = 1;
				DataFileIO::Read(langfile, (uint8_t*)x_DWORD_D41BC_langbuffer, v8);
			}
			DataFileIO::Close(langfile);
			break;
		}
		v10 = i;
		languagePath = GetSubDirectoryFile(cdFolder, "LANGUAGE", langcountstring);
	}
	if (x_WORD_180660_VGA_type_resolution & 1)
		sub_98709_create_index_dattab_power(a1x, a2x, a3, a3dattabindex);
	else
		sub_9874D_create_index_dattab(a1x, a2x, a3, a3dattabindex);
	sub_5B870_copy_sentence(x_DWORD_D41BC_langbuffer, x_DWORD_E9C4C_langindexbuffer, 471);//Exit Game
	return langcount;
}
// 8E3D5: using guessed type x_DWORD sprintf(x_DWORD, const char *, ...);
// 988DA: using guessed type x_DWORD filelength(x_DWORD);
// 98F9E: using guessed type x_DWORD unknown_libname_1_atoi(x_DWORD);
// D41A4: using guessed type int x_DWORD_D41A4;
// D41BC: using guessed type int x_DWORD_D41BC_langbuffer;
// E29E0: using guessed type char x_BYTE_E29E0;
// 180660: using guessed type __int16 x_WORD_180660_VGA_type_resolution;

//----- (00080C30) --------------------------------------------------------
void DrawText_80C30(__int16 posX, __int16 posY, __int16 a3)//261c30
{
	signed int v3; // ebx
	//char *v4; // eax
	int v4x;
	signed int v5; // edx
	//int16_t* i; // eax
	unsigned __int8 v7; // ST14_1
	int v8; // ST08_4
	__int16 v9; // ax

	v3 = -1;
	//v4 = (char *)unk_E17CC_0x194;
	v4x = 0;
	v5 = 0;
	while (mapScreenPortals_E17CC[v4x].viewPortPosX_4)
	{
		if (mapScreenPortals_E17CC[v4x].activated_18 == 2)
		{
			v3 = v5;
			break;
		}
		//v4 += 22;
		v4x++;
		v5++;
	}
	if (posY + posX > 0)
	{
		//for (i = x_WORD_E2970; *(int32_t*)&i[6]; i = (x_WORD *)((char *)i + 17))
		for (int ii = 0; secretMapScreenPortals_E2970[ii].activated_12; ii++)
		{
			if (secretMapScreenPortals_E2970[ii].activated_12 != 3 && v3 == secretMapScreenPortals_E2970[ii].word_4)
			{
				x_DWORD_17DE28str.x_BYTE_17DE34 = 3;
				return;
			}
		}
		DrawHelpText_6FC50(1);
		v7 = getPaletteIndex_5BE80(x_DWORD_17DE38str.x_DWORD_17DE38x, 0x3Fu, 0x3Fu, 0x3Fu);
		v8 = (signed __int16)(posX + a3 - 3 * sub_6FC10_letter_width());
		v9 = sub_6FC10_letter_width();
		sub_7FCB0_draw_text_with_border(/*v3,*/ x_DWORD_E9C4C_langindexbuffer[23 + v3], (signed __int16)(posX + 4 * v9), v8, posY, 5, v7, 1);
		//"You must explore the outer Netherworlds while you learn its magic. Your first destination is the ancient city of Jahwl."+
	}
	if (x_DWORD_17DE28str.x_BYTE_17DE34 != 3 && x_D41A0_BYTEARRAY_4_struct.setting_byte3_24 & 0x40 && !x_BYTE_17E09D)
	{
		x_BYTE_17E09D = 1;
		if ((signed __int16)v3 != -1)
			sub_86EB0(v3, 0, 0);
	}
}
// D41A4: using guessed type int x_DWORD_D41A4;
// 17DE34: using guessed type char x_BYTE_17DE34;
// 17DE38: using guessed type int x_DWORD_17DE38;
// 17E09D: using guessed type char x_BYTE_17E09D;

//----- (00081760) --------------------------------------------------------
void NewGameSubdraw_81760(/*type_mapScreenPortals_E17CC* a1x*/)//262760
{
	bool v1; // esi
	__int16 v2; // bx
	__int16 v3; // di
	//x_WORD *i; // ebx
	int ix;
	//int v5; // edx
	int v6; // edi
	//char *v7; // ebx
	int v7x;
	signed __int16 v8; // cx
	//char *v9; // edx
	int v9x;
	int k; // eax
	int v11; // edi
	int v12; // ebx
	char v14x[24]; // [esp+0h] [ebp-44h]
	uint8_t* temp_screen_buffer; // [esp+18h] [ebp-2Ch]
	int j; // [esp+1Ch] [ebp-28h]
	//uint8_t* v17; // [esp+20h] [ebp-24h]
	int16_t v17x;
	int16_t v18x; // [esp+24h] [ebp-20h]
	//uint8_t* v19; // [esp+28h] [ebp-1Ch]
	int16_t v19x;
	//int v19y;
	int l; // [esp+2Ch] [ebp-18h]
	//uint8_t* v21; // [esp+30h] [ebp-14h]
	int v21x;
	int16_t v22x; // [esp+34h] [ebp-10h]
	int16_t v23x; // [esp+38h] [ebp-Ch]
	int v24; // [esp+3Ch] [ebp-8h]
	int16_t v25x; // [esp+40h] [ebp-4h]

	// fix it
	//v17x = 0;
	//v18 = 0;
	//v19x = 0;
	// fix it

	v1 = false;
	v24 = 0;
	if (mapScreenPortals_E17CC[24].activated_18 == 1)
	{
		//_disable();
		temp_screen_buffer = pdwScreenBuffer_351628;
		pdwScreenBuffer_351628 = x_DWORD_17DE38str.x_DWORD_17DE64_game_world_map;
		v2 = screenHeight_180624;
		x_DWORD_180644_map_resolution2_y = 960;
		screenHeight_180624 = 960;
		v3 = screenWidth_18062C;
		x_DWORD_180648_map_resolution2_x = 1280;
		screenWidth_18062C = 1280;
		sub_7C120_draw_bitmap_640(518, 17, xy_DWORD_17DED4_spritestr[285]);
		sub_7C120_draw_bitmap_640(583, 17, xy_DWORD_17DED4_spritestr[286]);
		sub_7C120_draw_bitmap_640(657, 17, xy_DWORD_17DED4_spritestr[287]);
		sub_7C120_draw_bitmap_640(696, 17, xy_DWORD_17DED4_spritestr[288]);
		sub_7C120_draw_bitmap_640(518, 88, xy_DWORD_17DED4_spritestr[289]);
		sub_7C120_draw_bitmap_640(574, 88, xy_DWORD_17DED4_spritestr[290]);
		sub_7C120_draw_bitmap_640(657, 88, xy_DWORD_17DED4_spritestr[291]);
		sub_7C120_draw_bitmap_640(706, 88, xy_DWORD_17DED4_spritestr[292]);
		sub_7C120_draw_bitmap_640(518, 156, xy_DWORD_17DED4_spritestr[293]);
		sub_7C120_draw_bitmap_640(582, 156, xy_DWORD_17DED4_spritestr[294]);
		sub_7C120_draw_bitmap_640(657, 156, xy_DWORD_17DED4_spritestr[295]);
		sub_7C120_draw_bitmap_640(703, 156, xy_DWORD_17DED4_spritestr[296]);
		screenWidth_18062C = v3;
		screenHeight_180624 = v2;
		x_DWORD_180648_map_resolution2_x = v3;
		x_DWORD_180644_map_resolution2_y = v2;
		//HIWORD(a1) = HIWORD(v15);
		pdwScreenBuffer_351628 = temp_screen_buffer;
		//_enable();
	}
	//for (i = (x_WORD*)unk_E17CC_0x194; *(int32_t*)&i[2]; i += 11)
	for (ix = 0; mapScreenPortals_E17CC[ix].viewPortPosX_4; ix++)
	{
		if (mapScreenPortals_E17CC[ix].activated_18 == 1 && !v24)
		{
			//LOWORD(a1) = mapScreenPortals_E17CC[ix].word_12;
			v17x = mapScreenPortals_E17CC[ix].portalPosX_12;
			//LOWORD(a1) = mapScreenPortals_E17CC[ix].word_14;
			v19x = mapScreenPortals_E17CC[ix].portalPosY_14;
			//v19y = v19x;
			v17x += xy_DWORD_17DED4_spritestr[37].width_4 / 2;
			v19x += xy_DWORD_17DED4_spritestr[37].height_5 / 2;
			v24 = 1;
			//LABEL_19:
				//v19x = v19y;
			continue;
		}
		if (v24 && mapScreenPortals_E17CC[ix].activated_18 == 1)
		{
			//LOWORD(a1) = mapScreenPortals_E17CC[ix].word_14;
			//v5 = xy_DWORD_17DED4_spritestr[37].width;
			//v21x = mapScreenPortals_E17CC[ix].word_14;
			v6 = xy_DWORD_17DED4_spritestr[37].width_4 / 2 + mapScreenPortals_E17CC[ix].portalPosX_12;
			v21x = xy_DWORD_17DED4_spritestr[37].height_5 / 2 + mapScreenPortals_E17CC[ix].portalPosY_14;
			memset(v14x, 0, 24);
			sub_7E8D0((int16_t*)v14x, v6, v21x, v17x, v19x, 2, 2);
			v25x = v17x;
			v22x = v19x;
			while (!v1)
			{
				for (j = v1; ; j = v1)
				{
					while (j < 3)
					{
						v1 = sub_7E9D0((int16_t*)&v17x, (int16_t*)&v19x, (int16_t*)v14x);
						j++;
					}
					if (abs(v19x - v22x) <= 8
						&& abs(v17x - v25x) <= 8)
					{
						break;
					}
					sub_812D0_drawDotBitmap(v17x, v19x);
					v25x = v17x;
					v22x = v19x;
					if (v1)
						break;
					//goto LABEL_18;
				}
			}
			//LABEL_18:
			v19x = v21x;
			v17x = v6;
			v1 = false;
			//goto LABEL_19;
			//v19x = v19y;
			continue;
		}
	}
	if (x_DWORD_17DB70str.x_WORD_17DB8A != -1)
	{
		//v7 = (char *)unk_E17CC_0x194;
		v7x = 0;
		v8 = 0;
		v9x = -1;
		for (k = 0; mapScreenPortals_E17CC[v7x].viewPortPosX_4; k++)
		{
			if (mapScreenPortals_E17CC[v7x].activated_18 == 1)
			{
				v8 = 1;
				v9x = v7x;
			}
			if (mapScreenPortals_E17CC[v7x].activated_18 == 2)
				break;
			//v7 += 22;
			v7x++;
		}
		if (k < 24 && v8)
		{
			//LOWORD(k) = mapScreenPortals_E17CC[v9x].word_12;
			v17x = mapScreenPortals_E17CC[v9x].portalPosX_12;
			//LOWORD(k) = mapScreenPortals_E17CC[v9x].word_14;
			v19x = mapScreenPortals_E17CC[v9x].portalPosY_14;
			v17x += xy_DWORD_17DED4_spritestr[37].width_4 / 2;
			v19x += xy_DWORD_17DED4_spritestr[37].height_5 / 2;
			v11 = xy_DWORD_17DED4_spritestr[37].width_4 / 2 + mapScreenPortals_E17CC[v7x].portalPosX_12;
			v12 = xy_DWORD_17DED4_spritestr[37].height_5 / 2 + mapScreenPortals_E17CC[v7x].portalPosY_14;
			memset(v14x, 0, 24);
			sub_7E8D0((int16_t*)v14x, v11, v12, v17x, v19x, 2, 2);
			v1 = 0;
			v18x = v17x;
			v23x = v19x;
			while (!v1)
			{
				for (l = v1; ; l = v1)
				{
					while (l < 3)
					{
						v1 = sub_7E9D0((x_WORD*)&v17x, (x_WORD*)&v19x, (x_WORD*)v14x);
						l++;
					}
					if (abs(v19x - v23x) <= 8
						&& abs(v17x - v18x) <= 8)
					{
						break;
					}
					sub_812D0_drawDotBitmap(v17x, v19x);
					v18x = v17x;
					v23x = v19x;
					if (v1)
						return;
				}
			}
		}
	}
}

//----- (00081DB0) --------------------------------------------------------
void WriteConfigDat_81DB0()//262db0
{
	FILE* configDatFile;
	TypeConfigDat configDat;

	if (x_D41A0_BYTEARRAY_4_struct.setting_38402 == 1)
	{
		memset(printbuffer, 0, 80);
		sprintf(printbuffer, "%s/%s", gameDataPath.c_str(), "CONFIG.DAT");
		memset(&configDat, 0, sizeof(TypeConfigDat));
		configDatFile = DataFileIO::CreateOrOpenFile(printbuffer, 546);
		if (configDatFile != NULL)
		{
			configDat.configDatSign_0 = 0xfffffff7;
			configDat.langIndex_4 = x_D41A0_BYTEARRAY_4_struct.SelectedLangIndex;
			configDat.soundVolume_6 = x_D41A0_BYTEARRAY_4_struct.soundVolume_6;
			configDat.musicVolume_8 = x_D41A0_BYTEARRAY_4_struct.musicVolume_8;
			configDat.byteindex_10 = x_D41A0_BYTEARRAY_4_struct.byteindex_10;
			configDat.brightness_11 = x_D41A0_BYTEARRAY_4_struct.brightness_11;
			configDat.brightness_12 = x_D41A0_BYTEARRAY_4_struct.brightness_12;
			configDat.brightness_13 = x_D41A0_BYTEARRAY_4_struct.brightness_13;
			configDat.wordindex_14 = x_D41A0_BYTEARRAY_4_struct.wordindex_14;
			configDat.dwordindex_16 = x_D41A0_BYTEARRAY_4_struct.dwordindex_16;
			configDat.stubb[0] = x_D41A0_BYTEARRAY_4_struct.stubb[0];
			configDat.stubb[1] = x_D41A0_BYTEARRAY_4_struct.stubb[1];

			for (int i = 0; i < 10; i++)
				configDat.keys[i] = x_BYTE_EB39E_keys[i];

			WriteFile_98CAA(configDatFile, (uint8_t*)&configDat, sizeof(TypeConfigDat));
			DataFileIO::Close(configDatFile);
		}
		x_D41A0_BYTEARRAY_4_struct.setting_38402 = 0;
	}
}

//----- (00082510) --------------------------------------------------------
void sub_82510(/*__int16 a1*//*, int *a2*/)//263510
{
	int v2; // eax
	int v3; // edx
	__int16 v4; // dx
	//char* v5; // esi
	int v6y; // edx
	int v7; // eax
	int v8; // ecx
	//int result; // eax
	//char v10[36]; // [esp+0h] [ebp-2Ch]
	type_E24BCx v10x[2];
	//char v11[2]; // [esp+10h] [ebp-1Ch]
	posistruct2_t* v12x; // [esp+24h] [ebp-8h]
	//uint8_t* v13; // [esp+28h] [ebp-4h]
	posistruct2_t* v13x; // [esp+28h] [ebp-4h]
	posistruct_t* v13s;

	uint8_t switchbit = 0;

	v13x = x_DWORD_17DE38str.x_DWORD_17DEC0;
	v12x = x_DWORD_17DE38str.x_DWORD_17DEC4;
	x_DWORD_17DE38str.x_DWORD_17DEC0 = x_DWORD_17DE38str.x_DWORD_17DEC8;
	x_DWORD_17DE38str.x_DWORD_17DEC4 = x_DWORD_17DE38str.x_DWORD_17DECC;

	v13s = xy_DWORD_17DEC0_spritestr;//fixed
	xy_DWORD_17DEC0_spritestr = xy_DWORD_17DEC8_spritestr;//fixed

	if (x_D41A0_BYTEARRAY_4_struct.byteindex_10 == 1)
	{
		switchbit = unk_17DBA8str.x_BYTE_17DBB6;//*((x_BYTE *)a2 + 14);
		if (switchbit == 2)
		{
			v2 = j___clock();
			v3 = unk_17DBA8str.unk_17DBA8;
			unk_17DBA8str.unk_17DBAC = v2;
			if ((v2 - v3) / 0x64u > 1)
			{
				v4 = unk_17DBA8str.unk_17DBB4 + 1;
				unk_17DBA8str.unk_17DBB4 = v4;
				//if (!unk_E2516[9 * v4 + 1])
				if (!str_E2516[v4].word_2)
					unk_17DBA8str.unk_17DBB4 = 0;
				unk_17DBA8str.unk_17DBA8 = unk_17DBA8str.unk_17DBAC;//a2[0] = a2[1];
			}
			memset(v10x, 0, 36);//3550c0
			//v5 = (char*)&unk_E2516[9 * unk_17DBA8str.unk_17DBB4];
			//v6x = str_E23E0;
			v6y = 0;
			v10x[0] = str_E2516[unk_17DBA8str.unk_17DBB4];
			//qmemcpy(v10x, v5, 0x12u);
			//qmemcpy(&v11, v5 + 0x10u, 2u);
			while (str_E23E0[v6y].byte_25)
			{
				//if (str_E23E0[v6y].byte_23 && str_E23E0[v6y].byte_22 == (int)(unk_E2516[8 + 9 * unk_17DBA8str.unk_17DBB4] >> 8))// x_BYTE_E2527[18 * *((signed __int16 *)a2 + 6)] )
				if (str_E23E0[v6y].canSelect_23 && str_E23E0[v6y].byte_22 == str_E2516[unk_17DBA8str.unk_17DBB4].byte_16)// x_BYTE_E2527[18 * *((signed __int16 *)a2 + 6)] )
				{
					str_E23E0[v6y].gold_color_24 = 1;
					break;
				}
				//v6 += 44;
				v6y++;
			}
			sub_7E840_draw_textbox_with_line(v10x, 238, 264);//draw help
		}
		else if (switchbit == 3)
		{
			v7 = j___clock();
			v8 = unk_17DBA8str.unk_17DBA8;
			unk_17DBA8str.unk_17DBAC = v7;
			if ((v7 - v8) / 0x64u > 1)
				unk_17DBA8str.x_BYTE_17DBB5 = 2;
		}
	}
	x_DWORD_17DE38str.x_DWORD_17DEC0 = v13x;
	x_DWORD_17DE38str.x_DWORD_17DEC4 = v12x;

	xy_DWORD_17DEC0_spritestr = v13s;//fixed
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// 98786: using guessed type int /*__fastcall*/ j___clock(x_DWORD, x_DWORD, x_DWORD);
// D41A4: using guessed type int x_DWORD_D41A4;
// E2518: using guessed type __int16 x_WORD_E2518[];
// 17DEC0: using guessed type int (int)x_DWORD_17DEC0;
// 17DEC4: using guessed type int (int)x_DWORD_17DEC4;
// 17DEC8: using guessed type int x_DWORD_17DEC8;
// 17DECC: using guessed type int x_DWORD_17DECC;


//----- (00082670) --------------------------------------------------------
void sub_82670()//263670
{
	__int16 v0; // si
	//x_WORD *v1; // eax
	int16_t v1x;
	uint8_t* v2; // eax
	//int8_t* v3; // ebx
	int v3x;
	__int16 v4; // cx
	Type_SecretMapScreenPortals_E2970* v5x; // edi
	//int v7; // eax
	//char v10; // [esp+0h] [ebp-54h]
	unsigned __int8 v11; // [esp+50h] [ebp-4h]

	//fix it
	//v7 = 0;
	//fix it

	v0 = 0;
	v11 = x_WORD_180660_VGA_type_resolution;

	char dataPath[MAX_PATH];

	sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/SCREENS/HSCREEN0.DAT");

	//LOWORD(v1) = (unsigned __int8)x_WORD_180660_VGA_type_resolution;
	LastPressedKey_1806E4 = 0;
	if (!x_BYTE_E29E1)
	{
		//LOWORD(v1) = (uint16)x_D41A0_BYTEARRAY_4;
		if (!(x_D41A0_BYTEARRAY_4_struct.setting_byte1_22 & 0x10))
		{
			v2 = x_D41A0_BYTEARRAY_4_struct.pointer_0xE2_heapbuffer_226;
			x_BYTE_D41C1 = 0;
			x_DWORD_17DE38str.x_DWORD_17DE48c = v2;
			x_DWORD_17DE38str.x_DWORD_17DE54 = (uint8_t*)v2 + 301787;
			x_DWORD_17DE38str.x_DWORD_17DEC0 = (posistruct2_t*)((uint8_t*)v2 + 308527);
			x_DWORD_17DE38str.x_DWORD_17DEC4 = (posistruct2_t*)((uint8_t*)v2 + 310159);
			sub_7AA70_load_and_decompres_dat_file(dataPath, (uint8_t*)(v2 + 301787), 0x164FCD, 860);
			sub_7AA70_load_and_decompres_dat_file(dataPath, (uint8_t*)x_DWORD_17DE38str.x_DWORD_17DEC0, 0x165329, 548);

			//sub_7AA70_load_and_decompres_dat_file(0, 0, 0, 0);//can remove this?

			//if (*(x_BYTE *)(2124 * D41A0_BYTESTR_0.word_0xc + x_D41A0_BYTEARRAY_0 + 11232) & 2 || x_D41A0_BYTEARRAY_4_struct.levelnumber_43 > 0x18u)
			if (D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dw_w_b_0_2BDE_11230.byte[2] & 2 || x_D41A0_BYTEARRAY_4_struct.levelnumber_43w > 0x18u)
			{
				//v3 = byte_E16E0;//str_E16E0
				v3x = 0;
				//LOWORD(v1) = 1;
				v1x = 1;
				//while (*((x_BYTE*)v3 + 4))
				while (str_E16E0[v3x].byte_4)
				{
					if (x_D41A0_BYTEARRAY_4_struct.levelnumber_43w + 1 == str_E16E0[v3x].byte_4)
					{
						if (!str_E16E0[v3x].byte_5)
						{
							v0 = v1x;
							str_E16E0[v3x].byte_5 = 1;
						}
						break;
					}
					//v3 += 7;
					v3x++;
					v1x++;
				}
				if (!v0)
				{
					//LOWORD(v1) = (uint16)x_D41A0_BYTEARRAY_4;
					v4 = x_D41A0_BYTEARRAY_4_struct.levelnumber_43w;
					if ((unsigned __int16)v4 > 0x18u)
					{
						//v1 = sub_824E0(v4);
						v5x = sub_824E0(v4);
						if (v5x)
						{
							//v3 = byte_E16E0;
							v3x = 0;
							//LOWORD(v1) = 1;
							v1x = 1;
							while (str_E16E0[v3x].byte_4)
							{
								if (v5x->word_4 + 1 == str_E16E0[v3x].byte_4)
								{
									if (!str_E16E0[v3x].byte_5)
									{
										v0 = v1x;
										str_E16E0[v3x].byte_5 = 1;
									}
									break;
								}
								//v3 = (void **)((char *)v3 + 7);
								//v3 += 7;
								v3x++;
								v1x++;
							}
						}
					}
				}
				if (v0)
				{
					if (x_D41A0_BYTEARRAY_4_struct.SelectedLangIndex == 2 && soundActive2_E3798 || v0 >= 6)
					{
						x_BYTE_D41C1 = 0;
						x_BYTE_D41C0 = 0;
					}
					else
					{
						sub_2EB40();
						x_BYTE_D41C1 = 1;
						x_BYTE_D41C0 = 1;
					}
					sub_8CD27_set_cursor((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]);
					memset((void*)pdwScreenBuffer_351628, 0, 640 * 480);
					if (x_WORD_180660_VGA_type_resolution != 1)
					{
						sub_54600_mouse_reset();
						memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 768);
						if (x_WORD_180660_VGA_type_resolution & 1)
							ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, 0);
						else
							ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 640, 480, 0);

						sub_41A90_VGA_Palette_install((TColor*)*xadatapald0dat2.colorPalette_var28);
						x_WORD_180660_VGA_type_resolution = 1;
						sub_90D6E_VGA_set_video_mode_320x200_and_Palette((TColor*)*xadatapald0dat2.colorPalette_var28);
						sub_8CEDF_install_mouse();
						sub_8CD27_set_cursor((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]);
					}
					/*if (x_WORD_180660_VGA_type_resolution & 1)
						sub_98709_create_index_dattab_power(x_DWORD_17DEC0, x_DWORD_17DEC4, x_DWORD_17DE54, (new posistruct_t));
					else
						sub_9874D_create_index_dattab(x_DWORD_17DEC0, x_DWORD_17DEC4, x_DWORD_17DE54, (new posistruct_t));*/

					if (x_WORD_180660_VGA_type_resolution & 1)
					{
						sub_98709_create_index_dattab_power(x_DWORD_17DE38str.x_DWORD_17DEC0, x_DWORD_17DE38str.x_DWORD_17DEC4, x_DWORD_17DE38str.x_DWORD_17DE54, xy_DWORD_17DEC0_spritestr);
					}
					else
					{
						sub_9874D_create_index_dattab(x_DWORD_17DE38str.x_DWORD_17DEC0, x_DWORD_17DE38str.x_DWORD_17DEC4, x_DWORD_17DE38str.x_DWORD_17DE54, xy_DWORD_17DEC0_spritestr);
					}

					char cutScenePath[MAX_PATH];
					sprintf(cutScenePath, "%s/INTRO/CUT%d.DAT", cdDataPath.c_str(), str_E16E0[v3x].byte_6);
					sprintf(printbuffer, "%s", cutScenePath);

					PlayInfoFmv(0, 1, str_E16E0[v3x].pSoundEvent_0, cutScenePath);
					
					/*sprintf(printbuffer, "intro\\cut4.dat");
					sub_76160_play_intro(0, 1, 0 + (uint8_t*)array_E1328);*/

					sub_90B27_VGA_pal_fadein_fadeout(0, 0x10u, 0);
					sub_8D8F0_sound_proc3_endsample();
					StopMusic_8E020();
					sub_7B5D0();
					if (x_WORD_180660_VGA_type_resolution & 1)
						ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, 0);
					else
						ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 640, 480, 0);

					if (x_WORD_180660_VGA_type_resolution & 1)
						sub_90478_VGA_Blit320();
					else
						sub_75200_VGA_Blit640(480);
					//LOWORD(v1) = v11;
					if (v11 != x_WORD_180660_VGA_type_resolution)
					{
						sub_54600_mouse_reset();
						memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 768);
						if (x_WORD_180660_VGA_type_resolution & 1)
							ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, 0);
						else
							ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 640, 480, 0);

						sub_41A90_VGA_Palette_install((TColor*)*xadatapald0dat2.colorPalette_var28);
						x_WORD_180660_VGA_type_resolution = v11;
						if (v11 & 1)
							sub_90D6E_VGA_set_video_mode_320x200_and_Palette((TColor*)*xadatapald0dat2.colorPalette_var28);
						else
							sub_90E07_VGA_set_video_mode_640x480_and_Palette((TColor*)*xadatapald0dat2.colorPalette_var28);
						sub_8CEDF_install_mouse();
						// fix it//LOWORD(v1) = sub_8CD27_set_cursor((uint8_t**)*filearray_2aa18c[0]);
						sub_8CD27_set_cursor((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]);
					}
				}
			}
		}
	}
	x_BYTE_D41C1 = 0;
	//return (signed __int16)v1;
}

//----- (00083250) --------------------------------------------------------
void PlayIntros_83250(char a1)//264250
{
	//int v1; // eax

	sub_8CD27_set_cursor((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]);
	memset((void*)pdwScreenBuffer_351628, 0, 307200);
	if (x_WORD_180660_VGA_type_resolution != 1)
	{
		sub_54600_mouse_reset();
		memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 768);
		x_WORD_180660_VGA_type_resolution = 1;
		sub_90D6E_VGA_set_video_mode_320x200_and_Palette((TColor*)*xadatapald0dat2.colorPalette_var28);
		sub_8CEDF_install_mouse();
		sub_8CD27_set_cursor((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]);
	}
	Intros_76D10(a1);
	sub_54600_mouse_reset();
	memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 768);
	//BYTE1(v1) = x_WORD_180660_VGA_type_resolution;
	if (x_WORD_180660_VGA_type_resolution & 1)
		ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, 0);
	else
		ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 640, 480, 0);

	sub_41A90_VGA_Palette_install((TColor*)*xadatapald0dat2.colorPalette_var28);
	x_WORD_180660_VGA_type_resolution = 8;
	sub_90E07_VGA_set_video_mode_640x480_and_Palette((TColor*)*xadatapald0dat2.colorPalette_var28);
	sub_8CEDF_install_mouse();
	sub_8CD27_set_cursor((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]);
	SetCenterScreenForFlyAssistant_6EDB0();
	sub_7A110_load_hscreen(x_WORD_180660_VGA_type_resolution, 4);
	ResetMouse_7B5A0();
	sub_8CD27_set_cursor(xy_DWORD_17DED4_spritestr[39]);
	x_DWORD_17DE38str.x_WORD_17DEEE_mouse_buttons = 0;
	x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode = 0;
	sub_84300_load_sound(0);
}

//----- (000833C0) --------------------------------------------------------
void sub_833C0()//2643c0
{
	//uint8_t* v0; // eax
	//uint8_t* v1; // eax
	//int v2; // edx
	signed int v3; // eax
	signed int v4; // esi
	int v5; // ebx
	//int v6; // eax
	//int v7; // edx
	int v8; // eax
	unsigned int v9; // eax
	int v10; // edi
	char* v11; // ecx
	int v12; // eax
	//int v13; // edx
	int v14; // eax
	unsigned __int8 v15; // al
	//int v16; // eax
	//int v17; // eax
	int v18; // [esp+0h] [ebp-14h]
	int v19; // [esp+8h] [ebp-Ch]
	signed __int16 v20; // [esp+Ch] [ebp-8h]
	int i; // [esp+10h] [ebp-4h]

	char dataPath[MAX_PATH];

	sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/SCREENS/HSCREEN0.DAT");

	//fix it
	//v2 = 0;
	//v6 = 0;
	//v7 = 0;
	//v13 = 0;
	//fix it

	v19 = 0;
	v20 = 0;
	x_DWORD_17DE38str.x_DWORD_17DE38x = (TColor*)*xadatapald0dat2.colorPalette_var28;
	x_DWORD_17DE38str.x_WORD_17DEEC = 0;
	x_DWORD_17DE38str.x_DWORD_17DE40 = pdwScreenBuffer_351628;
	x_DWORD_17DE38str.x_DWORD_17DEE0_filedesc = NULL;
	//v0 = x_D41A0_BYTEARRAY_4_struct.pointer_0xE2_heapbuffer_226;
	x_DWORD_17DE38str.x_DWORD_17DEDC = 0;
	x_DWORD_17DE38str.x_DWORD_17DE48c = x_D41A0_BYTEARRAY_4_struct.pointer_0xE2_heapbuffer_226;
	sub_7B5D0();

	x_DWORD_17DE38str.x_DWORD_17DE54 = x_DWORD_17DE38str.x_DWORD_17DE48c + 0x49ADB;
	x_DWORD_17DE38str.x_DWORD_17DEC4 = (posistruct2_t*)(x_DWORD_17DE38str.x_DWORD_17DE48c + 0x4D313);
	x_DWORD_17DE38str.x_DWORD_17DE58 = x_DWORD_17DE38str.x_DWORD_17DE48c + 0x4D313;
	x_DWORD_17DE38str.x_DWORD_17DEC8 = (posistruct2_t*)(x_DWORD_17DE38str.x_DWORD_17DE48c + 0x4ECC2);
	x_DWORD_17DE38str.x_DWORD_17DECC = (posistruct2_t*)(x_DWORD_17DE38str.x_DWORD_17DE48c + 0x4F31C);
	x_DWORD_17DE38str.x_DWORD_17DED4 = (posistruct2_t*)(x_DWORD_17DE38str.x_DWORD_17DE48c + 0x4F31C);
	x_DWORD_17DE38str.x_DWORD_17DED8 = (posistruct2_t*)(x_DWORD_17DE38str.x_DWORD_17DE48c + 0x4FA72);
	x_DWORD_17DE38str.x_DWORD_17DE60 = x_DWORD_17DE38str.x_DWORD_17DE44;
	x_DWORD_17DE38str.x_DWORD_17DE64_game_world_map = x_DWORD_17DE38str.x_DWORD_17DE48c + 0x4FA72;
	//v1 = (0x12C000 + x_DWORD_17DE48c + 326258);//fix it
	x_DWORD_17DE38str.x_DWORD_17DE3C = (TcolNext*)(x_DWORD_17DE38str.x_DWORD_17DE48c + 0x17BA72);
	x_DWORD_17DE38str.x_DWORD_17DEC0 = (posistruct2_t*)(x_DWORD_17DE38str.x_DWORD_17DE48c + 0x4CCAD);
	x_DWORD_17DE38str.x_DWORD_17DE5C_border_bitmap = x_DWORD_17DE38str.x_DWORD_17DE48c + 0x17FA72;
	v3 = j___clock();
	v4 = v3;
	while (sub_9A10A_check_keyboard(/*v3*/))
	{
		LastPressedKey_1806E4 = 0;
		/*LOBYTE(v3) = */sub_7A060_get_mouse_and_keyboard_events();
	}
	sub_8CD27_set_cursor((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]);
	sub_7AA70_load_and_decompres_dat_file(dataPath, (uint8_t*)x_DWORD_17DE38str.x_DWORD_17DE54, 0x1641FC, 1214);
	sub_7AA70_load_and_decompres_dat_file(dataPath, (uint8_t*)x_DWORD_17DE38str.x_DWORD_17DEC0, 0x1646BA, 589);
	sub_7AA70_load_and_decompres_dat_file(dataPath, (uint8_t*)x_DWORD_17DE38str.x_DWORD_17DE58, 0x164907, 1191);
	sub_7AA70_load_and_decompres_dat_file(dataPath, (uint8_t*)x_DWORD_17DE38str.x_DWORD_17DEC8, 0x164DAE, 543);
	sub_7AA70_load_and_decompres_dat_file(dataPath, (uint8_t*)x_DWORD_17DE38str.x_DWORD_17DE38x, 0x178B5F, 768);
	sub_7AA70_load_and_decompres_dat_file(dataPath, x_DWORD_17DE38str.x_DWORD_17DE64_game_world_map, 0x16554D, 79378);
	sub_7AA70_load_and_decompres_dat_file(0, 0, 0, 0);
	/*if (x_WORD_180660_VGA_type_resolution & 1)
		sub_98709_create_index_dattab_power((uint8_t*)(int)x_DWORD_17DEC0, (uint8_t*)(int)x_DWORD_17DEC4, (uint8_t*)(int)x_DWORD_17DE54, (new posistruct_t));
	else
		sub_9874D_create_index_dattab((uint8_t*)(int)x_DWORD_17DEC0, (uint8_t*)(int)x_DWORD_17DEC4, (uint8_t*)(int)x_DWORD_17DE54, (new posistruct_t));
	*/
	if (x_WORD_180660_VGA_type_resolution & 1)
	{
		sub_98709_create_index_dattab_power(x_DWORD_17DE38str.x_DWORD_17DEC0, x_DWORD_17DE38str.x_DWORD_17DEC4, x_DWORD_17DE38str.x_DWORD_17DE54, xy_DWORD_17DEC0_spritestr);
	}
	else
	{
		sub_9874D_create_index_dattab(x_DWORD_17DE38str.x_DWORD_17DEC0, x_DWORD_17DE38str.x_DWORD_17DEC4, x_DWORD_17DE38str.x_DWORD_17DE54, xy_DWORD_17DEC0_spritestr);
	}

	/*if (x_WORD_180660_VGA_type_resolution & 1)
		sub_98709_create_index_dattab_power((uint8_t*)x_DWORD_17DEC8, (uint8_t*)x_DWORD_17DECC, (uint8_t*)x_DWORD_17DE58, (new posistruct_t));
	else
		sub_9874D_create_index_dattab((uint8_t*)x_DWORD_17DEC8, (uint8_t*)x_DWORD_17DECC, (uint8_t*)x_DWORD_17DE58, (new posistruct_t));*/
	if (x_WORD_180660_VGA_type_resolution & 1)
	{
		sub_98709_create_index_dattab_power(x_DWORD_17DE38str.x_DWORD_17DEC8, x_DWORD_17DE38str.x_DWORD_17DECC, x_DWORD_17DE38str.x_DWORD_17DE58, xy_DWORD_17DEC8_spritestr);
	}
	else
	{
		sub_9874D_create_index_dattab(x_DWORD_17DE38str.x_DWORD_17DEC8, x_DWORD_17DE38str.x_DWORD_17DECC, x_DWORD_17DE38str.x_DWORD_17DE58, xy_DWORD_17DEC8_spritestr);
	}

	x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode = 0;
	//v5 = (int)x_DWORD_17DEC0 + 6;
	x_DWORD_17DE38str.x_WORD_17DEEE_mouse_buttons = 0;
	//x_DWORD_17DEC0 += 6;

	if (x_WORD_180660_VGA_type_resolution & 1)
	{
		sub_98709_create_index_dattab_power_add((uint8_t*)x_DWORD_17DE38str.x_DWORD_17DEC0, (uint8_t*)x_DWORD_17DE38str.x_DWORD_17DEC4, x_DWORD_17DE38str.x_DWORD_17DE54, xy_DWORD_17DEC0_spritestr, 6);
	}
	else
	{
		sub_9874D_create_index_dattab_add((uint8_t*)x_DWORD_17DE38str.x_DWORD_17DEC0, (uint8_t*)x_DWORD_17DE38str.x_DWORD_17DEC4, x_DWORD_17DE38str.x_DWORD_17DE54, xy_DWORD_17DEC0_spritestr, 6);
	}
	//x_DWORD_17DE38str.x_DWORD_17DEC0 += 6;
	x_DWORD_17DE38str.x_DWORD_17DEC0++;

	while (!x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode && !x_DWORD_17DE38str.x_WORD_17DEEE_mouse_buttons)
	{
		if (x_WORD_180660_VGA_type_resolution & 1)
			CopyScreen((void*)x_DWORD_17DE38str.x_DWORD_17DE64_game_world_map, (void*)pdwScreenBuffer_351628, 320, 200);
		else
			CopyScreen((void*)x_DWORD_17DE38str.x_DWORD_17DE64_game_world_map, (void*)pdwScreenBuffer_351628, 640, 480);
		v8 = j___clock();
		v5 = v19;
		v18 = v8;
		for (i = 0; ; i += xy_DWORD_17DEC0_spritestr[65].height_5 + 2)
		{
			v9 = (v18 - v4) / 0x64u;
			v10 = (signed __int16)v5;
			v11 = (char*)off_DB558[v10];
			if (v11 == NULL)//end of sequence
			{
				if (v9 > 5)
				{
					v12 = j___clock();
					v5 = 0;
					v4 = v12;
					v19 = 0;
				}
				goto LABEL_24;
			}
			if (!_stricmp(v11, "!"))
				break;
			v15 = getPaletteIndex_5BE80(x_DWORD_17DE38str.x_DWORD_17DE38x, 0x3Fu, 0x3Fu, 0x3Fu);
			sub_7FCB0_draw_text_with_border(/*v5,*/ off_DB558[v10], 10, 620, (signed __int16)(i + 200), 5, v15, 0);
			//HIWORD(v16) = HIWORD(x_DWORD_17DEC0);
			//LOWORD(v16) = xy_DWORD_17DEC0_spritestr[65].height;
			v5++;
		}
		if ((v18 - v4) / 0x64u > 2)
		{
			v14 = j___clock();
			v5++;
			v4 = v14;
			v19 = v5;
		}
	LABEL_24:
		sub_7A060_get_mouse_and_keyboard_events();
		if (v20)
		{
			if (x_WORD_180660_VGA_type_resolution & 1)
				sub_90478_VGA_Blit320();
			else
				sub_75200_VGA_Blit640(480);
		}
		else
		{
			sub_90B27_VGA_pal_fadein_fadeout(x_DWORD_17DE38str.x_DWORD_17DE38x, 0x20u, 0);
			v20 = 1;
		}
	}
	sub_90B27_VGA_pal_fadein_fadeout(0, 0x10u, 0);
	/*v17 = (int)*/memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 768);
	//LOBYTE(v17) = x_WORD_180660_VGA_type_resolution;
	if (x_WORD_180660_VGA_type_resolution & 1)
		ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, 0);
	else
		ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 640, 480, 0);
	if (x_WORD_180660_VGA_type_resolution & 1)
		sub_90478_VGA_Blit320();
	else
		sub_75200_VGA_Blit640(480);
	while (x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode || x_DWORD_17DE38str.x_WORD_17DEEE_mouse_buttons)
		sub_7A060_get_mouse_and_keyboard_events();
	x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode = 0;
}

//----- (00083850) --------------------------------------------------------
void ShowWelcomeScreen_83850()//264850
{
	//signed int v0; // eax
	signed int v1; // ebx
	//int v2; // edx
	//int v3; // edx
	int v4; // esi
	int v5; // eax
	//int v6; // eax
	//signed int v7; // eax

	char dataPath[MAX_PATH];

	sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/SCREENS/HSCREEN0.DAT");

	//fix it
	//v2 = 0;
	//v3 = 0;
	//v7 = 0;

	//fix it

	sub_7AA70_load_and_decompres_dat_file(dataPath, x_DWORD_E9C38_smalltit, 0x178E5F, 0x32B9);
	sub_7AA70_load_and_decompres_dat_file(dataPath, *xadatapald0dat2.colorPalette_var28, 0x17C118, 0x300);
	sub_7AA70_load_and_decompres_dat_file(cdDataPath.c_str(), nullptr, 0, 0);
	//v0 = (int)sub_7AA70_load_and_decompres_dat_file(0, 0, 0, 0); //fix it
	//v0 = 0;//fix it
	v1 = 0;
	v5 = j___clock();
	v4 = v5;
	//BYTE1(v5) = 0;
	LastPressedKey_1806E4 = 0;
	while (!LastPressedKey_1806E4 && !x_WORD_180746_mouse_left_button && !x_WORD_180744_mouse_right_button && (j___clock() - v4) / 0x64u <= 0x14)
	{
		if (x_WORD_180660_VGA_type_resolution & 1)
			CopyScreen(x_DWORD_E9C38_smalltit, pdwScreenBuffer_351628, 320, 200);
		else
			CopyScreen(x_DWORD_E9C38_smalltit, pdwScreenBuffer_351628, 640, 480);
		if (v1)
		{
			if (x_WORD_180660_VGA_type_resolution & 1)
				sub_90478_VGA_Blit320();
			else
				sub_75200_VGA_Blit640(480);
		}
		else
		{
			v1 = 1;
			ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 640, 480, 0);//fix
			sub_75200_VGA_Blit640(480);//fix
			sub_90B27_VGA_pal_fadein_fadeout((TColor*)*xadatapald0dat2.colorPalette_var28, 0x20u, 0);
		}
	}
	/*LOWORD(v6) = */sub_90B27_VGA_pal_fadein_fadeout(0, 0x10u, 0);
	if (x_WORD_180660_VGA_type_resolution & 1)
		ClearGraphicsBuffer_72883(pdwScreenBuffer_351628, 320, 200, 0);
	else
		ClearGraphicsBuffer_72883(pdwScreenBuffer_351628, 640, 480, 0);

	if (x_WORD_180660_VGA_type_resolution & 1)
		sub_90478_VGA_Blit320();
	else
		sub_75200_VGA_Blit640(480);
	while (sub_9A10A_check_keyboard(/*v7*/))
	{
		LastPressedKey_1806E4 = 0;
		/*v7 = */sub_7A060_get_mouse_and_keyboard_events();
	}
	j___delay(50);
}

//----- (00081EE0) --------------------------------------------------------
void DrawAndSoundDragonAndFire_81EE0(__int16 a5, __int16 a6)//262ee0
{
	int a4;
	int v6; // eax
	//int16_t* v7; // ebx
	//type_WORD_E20A4* v7x;
	int v7y;
	int v8; // esi
	signed __int16 result; // ax
	unsigned __int8 v10; // al
	unsigned __int8 v11; // cl
	char v12; // ah
	__int16 v13; // ax
	unsigned __int8 v14; // ST18_1
	int v15; // ST08_4
	__int16 v16; // ST04_2
	//int v17; // ST18_4
	//int v18; // ST14_4
	unsigned __int8 v19; // ST18_1
	int v20; // ST08_4
	char v21; // dh
	__int16 v22; // ST04_2
	int v23x; // edx
	//int v24; // eax
	//x_WORD *v25; // edx
	int v25x;
	signed __int16 v26; // ax
	char v27; // al
	__int16 v28; // ax
	//int v29; // ST18_4
	//int v30; // ST14_4
	unsigned __int8 v31; // ST18_1
	int v32; // ST08_4
	char v33; // dl
	__int16 v34; // ST04_2
	//x_WORD *v35; // edx
	int v35x;
	__int16 v36; // ax
	unsigned __int8 v37; // al
	char v38; // ch
	//int v39; // ST18_4
	//int v40; // ST14_4
	__int16 v41; // ST2C_2
	__int16 v42; // ST30_2
	uint8_t* temp_screen_buffer; // ST1C_4
	int v44; // edx
	__int16 v45; // ST14_2
	__int16 v46; // ST10_2
	__int16 k; // [esp+8h] [ebp-14h]
	__int16 i; // [esp+Ch] [ebp-10h]
	__int16 j; // [esp+18h] [ebp-4h]

	v6 = j___clock();
	//v7x = str_WORD_E20A4;
	v7y = 0;
	v8 = v6;
	for (result = str_WORD_E20A4[0].array_word_18[2]; result; result = str_WORD_E20A4[v7y].array_word_18[2])
	{
		//v10 = *((x_BYTE*)v7 + 42);
		v10 = str_WORD_E20A4[v7y].byte_42;
		if (v10 == 1)
		{
			v11 = str_WORD_E20A4[v7y].byte_43;//*((x_BYTE*)v7 + 43);
			switch (v11)
			{
			case 0u:
				v12 = str_WORD_E20A4[v7y].byte_43;//*((x_BYTE*)v7 + 43);
				//*(x_DWORD*)v7 = v8;
				str_WORD_E20A4[v7y].dword_0 = v8;
				str_WORD_E20A4[v7y].byte_43 = v12 + 1;
				break;
			case 1u:
				if ((v8 - str_WORD_E20A4[v7y].dword_0) / 0x64u > str_WORD_E20A4[v7y].array_word_18[11])
					str_WORD_E20A4[v7y].byte_43 = v11 + 1;
				break;
			case 2u:
				str_WORD_E20A4[v7y].array_word_18[0] = str_WORD_E20A4[v7y].array_word_18[2];
				v13 = str_WORD_E20A4[v7y].array_word_18[3];
				str_WORD_E20A4[v7y].array_word_18[1] = v13;
				sub_7E8D0(str_WORD_E20A4[v7y].array_word_45, str_WORD_E20A4[v7y].array_word_18[4], str_WORD_E20A4[v7y].array_word_18[5], str_WORD_E20A4[v7y].array_word_18[2], v13, 2, 2);
				str_WORD_E20A4[v7y].array_word_18[7] = str_WORD_E20A4[v7y].array_word_18[6];
				v14 = str_WORD_E20A4[v7y].byte_12;
				v15 = (unsigned __int8)x_BYTE_E1324;
				v16 = str_WORD_E20A4[v7y].word_10;
				++str_WORD_E20A4[v7y].byte_43;
				sub_8F100_sound_proc19(0, v16, v15, 64, 0x64u, 0, v14);
				break;
			case 3u:
				for (i = 0; i < 4; i++)
				{
					//v17 = str_WORD_E20A4[v7y].array_word_18[5];
					//v18 = str_WORD_E20A4[v7y].array_word_18[4];
					a4 = sub_7E9D0(&str_WORD_E20A4[v7y].array_word_18[0], &str_WORD_E20A4[v7y].array_word_18[1], str_WORD_E20A4[v7y].array_word_45);
				}
				if ((unsigned int)(v8 - str_WORD_E20A4[v7y].dword_0) >> 3 >= 1)
				{
					if (str_WORD_E20A4[v7y].array_word_18[8] <= str_WORD_E20A4[v7y].array_word_18[7] - 1)
						++str_WORD_E20A4[v7y].array_word_18[8];
					else
						str_WORD_E20A4[v7y].array_word_18[8] = str_WORD_E20A4[v7y].array_word_18[6];
					str_WORD_E20A4[v7y].dword_0 = v8;
				}
				sub_7C120_draw_bitmap_640(str_WORD_E20A4[v7y].array_word_18[0] - a5, str_WORD_E20A4[v7y].array_word_18[1] - a6, xy_DWORD_17DED4_spritestr[str_WORD_E20A4[v7y].array_word_18[8]]);
				if (a4)
				{
					str_WORD_E20A4[v7y].array_word_18[8] = str_WORD_E20A4[v7y].array_word_18[9];
					v19 = str_WORD_E20A4[v7y].byte_16;
					v20 = (unsigned __int8)x_BYTE_E1324;
					v21 = str_WORD_E20A4[v7y].byte_43;
					v22 = str_WORD_E20A4[v7y].word_14;
					str_WORD_E20A4[v7y].dword_0 = v8;
					str_WORD_E20A4[v7y].byte_43 = v21 + 1;
					sub_8F100_sound_proc19(0, v22, v20, 64, 0x64u, 0, v19);
				}
				break;
			case 4u:
				a4 = false;
				if ((unsigned int)(v8 - str_WORD_E20A4[v7y].dword_0) >> 3 >= 1)
				{
					if (str_WORD_E20A4[v7y].array_word_18[8] <= str_WORD_E20A4[v7y].array_word_18[10] - 1)
					{
						str_WORD_E20A4[v7y].array_word_18[8]++;
					}
					else
					{
						a4 = true;
						str_WORD_E20A4[v7y].array_word_18[8] = str_WORD_E20A4[v7y].array_word_18[10];
					}
					str_WORD_E20A4[v7y].dword_0 = v8;
				}
				if (!a4)
				{
					//v23 = (int)x_DWORD_17DED4;
					v23x = str_WORD_E20A4[v7y].array_word_18[8];
					goto LABEL_85;
				}
				str_WORD_E20A4[v7y].byte_43 = 0;
				break;
			}
		}
		else if (v10 == 2u)
		{
			if (x_DWORD_17DB70str.x_BYTE_17DB8F != 4)
			{
				//v25 = (x_WORD*)unk_E17CC_0x194;
				v25x = 0;
				v26 = 0;
				while (mapScreenPortals_E17CC[v25x].viewPortPosX_4 && mapScreenPortals_E17CC[v25x].activated_18 != 2)
				{
					//v25 += 11;
					v25x++;
					v26++;
				}
				if (v26 == 25)
					v26 = 24;
				if (v26 == str_WORD_E20A4[v7y].byte_44)
				{
					switch (str_WORD_E20A4[v7y].byte_43)
					{
					case 0:
						v27 = str_WORD_E20A4[v7y].byte_43;
						str_WORD_E20A4[v7y].dword_0 = v8;
						str_WORD_E20A4[v7y].byte_43 = v27 + 1;
						break;
					case 1:
						goto LABEL_42;
					case 2:
						for (j = 0; j < 4; j++)
						{
							//v29 = str_WORD_E20A4[v7y].array_word_18[5];
							//v30 = str_WORD_E20A4[v7y].array_word_18[4];
							a4 = sub_7E9D0(str_WORD_E20A4[v7y].array_word_18, &str_WORD_E20A4[v7y].array_word_18[1], str_WORD_E20A4[v7y].array_word_45);
						}
						if ((unsigned int)(v8 - str_WORD_E20A4[v7y].dword_0) >> 3 >= 1)
						{
							if (str_WORD_E20A4[v7y].array_word_18[8] <= str_WORD_E20A4[v7y].array_word_18[7] - 1)
								++str_WORD_E20A4[v7y].array_word_18[8];
							else
								str_WORD_E20A4[v7y].array_word_18[8] = str_WORD_E20A4[v7y].array_word_18[6];
							str_WORD_E20A4[v7y].dword_0 = v8;
						}
						sub_7C120_draw_bitmap_640(str_WORD_E20A4[v7y].array_word_18[0] - a5, str_WORD_E20A4[v7y].array_word_18[1] - a6, xy_DWORD_17DED4_spritestr[str_WORD_E20A4[v7y].array_word_18[8]]);
						if (a4)
						{
							str_WORD_E20A4[v7y].array_word_18[8] = str_WORD_E20A4[v7y].array_word_18[9];
							v31 = str_WORD_E20A4[v7y].byte_16;
							v32 = (unsigned __int8)x_BYTE_E1324;
							v33 = str_WORD_E20A4[v7y].byte_43;
							v34 = str_WORD_E20A4[v7y].word_14;
							str_WORD_E20A4[v7y].dword_0 = v8;
							str_WORD_E20A4[v7y].byte_43 = v33 + 1;
							sub_8F100_sound_proc19(0, v34, v32, 64, 0x64u, 0, v31);
						}
						break;
					case 3:
						a4 = false;
						if ((unsigned int)(v8 - str_WORD_E20A4[v7y].dword_0) >> 3 >= 1)
						{
							if (str_WORD_E20A4[v7y].array_word_18[8] <= str_WORD_E20A4[v7y].array_word_18[10] - 1)
							{
								++str_WORD_E20A4[v7y].array_word_18[8];
							}
							else
							{
								a4 = true;
								str_WORD_E20A4[v7y].array_word_18[8] = str_WORD_E20A4[v7y].array_word_18[9];
							}
							str_WORD_E20A4[v7y].dword_0 = v8;
						}
						if (!a4)
							goto LABEL_84;
						++str_WORD_E20A4[v7y].byte_43;
						break;
					default:
						break;
					}
				}
				else
				{
					str_WORD_E20A4[v7y].byte_43 = 0;
				}
			}
		}
		else if (v10 == 3)
		{
			//v35 = (x_WORD*)unk_E17CC_0x194;
			v35x = 0;
			v36 = 0;
			while (mapScreenPortals_E17CC[v35x].viewPortPosX_4 && mapScreenPortals_E17CC[v35x].activated_18 != 2)
			{
				//v35 += 11;
				v35x++;
				v36++;
			}
			if (v36)
				v36--;
			if (v36 == str_WORD_E20A4[v7y].byte_44)
			{
				v37 = str_WORD_E20A4[v7y].byte_43;
				if (v37 < 1u)
				{
					if (!v37)
					{
						v38 = str_WORD_E20A4[v7y].byte_43;
						str_WORD_E20A4[v7y].dword_0 = v8;
						str_WORD_E20A4[v7y].byte_43 = v38 + 1;
					}
				}
				else if (v37 <= 1u)
				{
				LABEL_42:
					str_WORD_E20A4[v7y].array_word_18[0] = str_WORD_E20A4[v7y].array_word_18[2];
					v28 = str_WORD_E20A4[v7y].array_word_18[3];
					str_WORD_E20A4[v7y].array_word_18[1] = v28;
					sub_7E8D0(str_WORD_E20A4[v7y].array_word_45, str_WORD_E20A4[v7y].array_word_18[4], str_WORD_E20A4[v7y].array_word_18[5], str_WORD_E20A4[v7y].array_word_18[2], v28, 2, 2);
					str_WORD_E20A4[v7y].array_word_18[8] = str_WORD_E20A4[v7y].array_word_18[6];
					++str_WORD_E20A4[v7y].byte_43;
					sub_8F100_sound_proc19(0, str_WORD_E20A4[v7y].word_10, (unsigned __int8)x_BYTE_E1324, 64, 0x64u, 0, str_WORD_E20A4[v7y].byte_12);
				}
				else if (v37 == 2)
				{
					for (k = 0; k < 4; k++)
					{
						//v39 = str_WORD_E20A4[v7y].array_word_18[5];
						//v40 = str_WORD_E20A4[v7y].array_word_18[4];
						a4 = sub_7E9D0(str_WORD_E20A4[v7y].array_word_18, &str_WORD_E20A4[v7y].array_word_18[1], str_WORD_E20A4[v7y].array_word_45);
					}
					if ((unsigned int)(v8 - str_WORD_E20A4[v7y].dword_0) >> 3 >= 1)
					{
						if (str_WORD_E20A4[v7y].array_word_18[8] <= str_WORD_E20A4[v7y].array_word_18[7] - 1)
							++str_WORD_E20A4[v7y].array_word_18[8];
						else
							str_WORD_E20A4[v7y].array_word_18[8] = str_WORD_E20A4[v7y].array_word_18[6];
						str_WORD_E20A4[v7y].dword_0 = v8;
					}
					if (mapScreenPortals_E17CC[24].activated_18 == 1)
					{
						//_disable();
						v41 = screenWidth_18062C;
						v42 = screenHeight_180624;
						temp_screen_buffer = pdwScreenBuffer_351628;
						pdwScreenBuffer_351628 = x_DWORD_17DE38str.x_DWORD_17DE64_game_world_map;
						v44 = str_WORD_E20A4[v7y].array_word_18[8];
						x_DWORD_180648_map_resolution2_x = 1280;
						screenWidth_18062C = 1280;
						v45 = str_WORD_E20A4[v7y].array_word_18[1];
						v46 = str_WORD_E20A4[v7y].array_word_18[0];
						x_DWORD_180644_map_resolution2_y = 960;
						screenHeight_180624 = 960;
						sub_7C120_draw_bitmap_640(v46, v45, xy_DWORD_17DED4_spritestr[v44]);
						screenWidth_18062C = v41;
						screenHeight_180624 = v42;
						x_DWORD_180648_map_resolution2_x = v41;
						x_DWORD_180644_map_resolution2_y = v42;
						pdwScreenBuffer_351628 = temp_screen_buffer;
						//_enable();
					}
					else
					{
					LABEL_84:
						v23x = str_WORD_E20A4[v7y].array_word_18[8];
						//v24 = (int)x_DWORD_17DED4;
					LABEL_85:
						sub_7C120_draw_bitmap_640(str_WORD_E20A4[v7y].array_word_18[0] - a5, str_WORD_E20A4[v7y].array_word_18[1] - a6, xy_DWORD_17DED4_spritestr[v23x]);
					}
				}
			}
			else
			{
				str_WORD_E20A4[v7y].byte_43 = 0;
			}
		}
		//v7 = (signed __int16*)((char*)v7 + 69);
		v7y++;
	}
	//return result;
}

//----- (00082C20) --------------------------------------------------------
void DrawEndGameTable_82C20(__int16 a1)//263c20
{
	//x_WORD *v1; // edx
	__int16 v2; // si
	//int v3; // ebx
	signed __int16 v4; // bx
	__int16 v5; // si
	int v6; // edx
	signed __int16 v7; // bx
	unsigned int v8; // esi
	int v9; // edx
	//int v10; // eax
	int v11; // esi
	int v12; // edi
	signed __int16 i; // bx
	//int v14; // eax
	x_BYTE* v15; // ebx
	__int16 v16; // ax
	//unsigned __int16 v17; // dx
	signed __int16 j; // bx
	//int v19; // eax
	__int16 v20; // di
	signed int v21; // ebx
	int v22; // edx
	//char v23; // [esp+0h] [ebp-12h]
	char* v24x[6]; // [esp+28h] [ebp+16h] //v24x[0]
	//char* v25_spell_found; // [esp+2Ch] [ebp+1Ah]//v24x[1]
	//char* v26; // [esp+30h] [ebp+1Eh]//v24x[2]
	//char* v27; // [esp+34h] [ebp+22h]//v24x[3]
	//char* v28; // [esp+38h] [ebp+26h]//v24x[4]
	//char* v29; // [esp+3Ch] [ebp+2Ah]//v24x[5]
	int v30x[8]; // [esp+40h] [ebp+2Eh]//v30x[0]
	//int v31x[7]; // [esp+44h] [ebp+32h]//v30x[1]
	//int v32; // [esp+48h] [ebp+36h]//v30x[2]
	//int v33; // [esp+4Ch] [ebp+3Ah]//v30x[3]
	//int v34; // [esp+50h] [ebp+3Eh]//v30x[4]
	//int v35; // [esp+54h] [ebp+42h]//v30x[5]
	//int v36; // [esp+58h] [ebp+46h]//v30x[6]
	//int v37; // [esp+5Ch] [ebp+4Ah]//v30x[7]
	//int v38; // [esp+60h] [ebp+4Eh]
	//int v39; // [esp+64h] [ebp+52h]
	int v40; // [esp+68h] [ebp+56h]
	int v41; // [esp+6Ch] [ebp+5Ah]
	int v42; // [esp+70h] [ebp+5Eh]
	int v43; // [esp+74h] [ebp+62h]
	int v44; // [esp+78h] [ebp+66h]
	int v45; // [esp+7Ch] [ebp+6Ah]
	int v46; // [esp+80h] [ebp+6Eh]
	int v47; // [esp+84h] [ebp+72h]
	int v48; // [esp+88h] [ebp+76h]
	int v49; // [esp+8Ch] [ebp+7Ah]

	v42 = 50;
	v48 = 50;
	if (a1 > -1)
	{
		v24x[0] = (char*)LevelsNames_D9204[a1];
		v24x[1] = x_DWORD_E9C4C_langindexbuffer[386];//Spells found //fix it this variable not used
		v24x[2] = x_DWORD_E9C4C_langindexbuffer[385];//Accuracy
		v24x[3] = x_DWORD_E9C4C_langindexbuffer[384];//Creatures Killed
		v24x[4] = x_DWORD_E9C4C_langindexbuffer[377];//Mana
		v24x[5] = x_DWORD_E9C4C_langindexbuffer[394];//Time Taken
		v30x[0] = 0;
		v30x[1] = 0;
		v30x[7] = 0;
		if (a1 > 24)
		{
			//v1 = x_WORD_E2970;
			//int v1i = 0;
			v2 = 0;
			while (secretMapScreenPortals_E2970[v2].activated_12)
			{
				if (a1 == secretMapScreenPortals_E2970[v2].levelNumber_6)
				{
					//v3 = 5 * v2;
					v30x[2] = x_DWORD_17DDBCx[5 * v2];
					v30x[3] = x_DWORD_17DDBCx[1 + 5 * v2];
					v30x[4] = x_DWORD_17DDBCx[2 + 5 * v2];
					v30x[5] = x_DWORD_17DDBCx[3 + 5 * v2];
					v30x[6] = x_DWORD_17DDBCx[4 + 5 * v2];
				}
				//v1 = (x_WORD *)((char *)v1 + 17);
				//v1i++;
				v2++;
			}
		}
		else
		{
			v30x[2] = x_DWORD_17DBC8x[5 * a1];
			v30x[3] = x_DWORD_17DBC8x[1 + 5 * a1];
			v30x[4] = x_DWORD_17DBC8x[2 + 5 * a1];
			v30x[5] = x_DWORD_17DBC8x[3 + 5 * a1];
			v30x[6] = x_DWORD_17DBC8x[4 + 5 * a1];//time of game
		}
		v4 = 1;
		v5 = strlen(v24x[0]);
		while (v4 < 6)
		{
			v6 = v4;
			if (strlen(v24x[v6]) > v5)
				v5 = strlen(v24x[v6]);
			v4++;
		}
		v7 = 1;
		v8 = strlen(v24x[0]);
		while (v7 < 6)
		{
			v9 = v7;
			if (strlen(v24x[v9]) > v8)
				v8 = strlen(v24x[v9]);
			v7++;
		}
		//HIWORD(v10) = HIWORD(x_DWORD_17DEC0);
		//LOWORD(v10) = xy_DWORD_17DEC0_spritestr[65].width;
		v11 = 2 * xy_DWORD_17DEC0_spritestr[65].width_4 * v8;
		v12 = 320 - (signed __int16)v11 / 2;
		v44 = xy_DWORD_17DED4_spritestr[275].width_4;
		v40 = (unsigned __int8)v44;
		if ((signed __int16)v11 % (signed int)(unsigned __int8)v44)
			v11 = ((signed __int16)v11 / (signed int)(unsigned __int8)v44 + 1) * v44;
		for (i = 0; i < (signed __int16)v11; i += v44)
			sub_7C120_draw_bitmap_640(i + v12, v48, xy_DWORD_17DED4_spritestr[275]);
		//HIWORD(v14) = HIWORD(x_DWORD_17DED4);
		//LOWORD(v14) = xy_DWORD_17DED4_spritestr[275].height;
		v49 = 0;
		v48 += xy_DWORD_17DED4_spritestr[275].height_5;
		do
		{
			v41 = (signed __int16)v12 + xy_DWORD_17DED4_spritestr[274].width_4;
			//v39 = (signed __int16)v48;
			v40 = (signed __int16)v48;
			v15 = (x_BYTE*)(v41 + 640 * (signed __int16)v48 + pdwScreenBuffer_351628);
			sub_7C120_draw_bitmap_640(v12, v48, xy_DWORD_17DED4_spritestr[274]);
			sub_7C120_draw_bitmap_640(v11 + v12 - xy_DWORD_17DED4_spritestr[274].width_4, v40, xy_DWORD_17DED4_spritestr[274]);
			v45 = 0;
			while (xy_DWORD_17DED4_spritestr[274].height_5 > (signed __int16)v45)
			{
				v16 = 0;
				while (v16 < v11 - 2 * xy_DWORD_17DED4_spritestr[274].width_4)
				{
					//HIBYTE(v17) = 15;
					//LOBYTE(v17) = *v15;
					//v41 = v17;
					*v15 = x_DWORD_17DE38str.x_DWORD_17DE3C->data[*v15+0xc00];
					v15++;
					v16++;
				}
				v45++;
				v15 += 640 - v16;
				v48++;
			}
			v49++;
		} while ((signed __int16)v49 < 12);
		for (j = 0; j < (signed __int16)v11; j += v44)
			sub_7C120_draw_bitmap_640(j + v12, v48, xy_DWORD_17DED4_spritestr[275]);
		//HIWORD(v19) = HIWORD(x_DWORD_17DED4);//fix it
		//LOWORD(v19) = xy_DWORD_17DED4_spritestr[274].width;
		v47 = 2 * xy_DWORD_17DED4_spritestr[274].width_4 + v42;
		//LOWORD(v19) = xy_DWORD_17DED4_spritestr[65].width;
		v20 = xy_DWORD_17DEC0_spritestr[65].width_4 + v12;
		sub_7FAE0_draw_text(v24x[0], v20, v11 - xy_DWORD_17DEC0_spritestr[65].width_4 + v20, v47, 0);
		v21 = 1;
		v47 += 5;
		while (1)
		{
			v46 = v47 + 2;
			if ((signed __int16)v21 >= 5)
			{
				sub_7FB90_draw_text(
					v24x[5],
					(signed __int16)(v20 + 2 * xy_DWORD_17DEC0_spritestr[65].width_4),
					//v46 + 5 * *(unsigned __int8 *)((int)x_DWORD_17DEC0 + 395) + *(unsigned __int8 *)((int)x_DWORD_17DEC0 + 395),
					v46 + 6 * xy_DWORD_17DEC0_spritestr[v46].height_5,
					0);
				//v39 = 60;
				v41 = v30x[6] % 3600 / 60;
				//v38 = v30x[6] % 3600 / 60;
				sprintf(printbuffer, "%02d:%02d:%02d", v30x[6] / 3600, v41, v30x[6] % 3600 % 60);
				sub_7FB90_draw_text(
					printbuffer,
					(signed __int16)(v20 + v11 - 14 * xy_DWORD_17DEC0_spritestr[65].width_4),
					//v46 + 5 * *(unsigned __int8 *)((int)x_DWORD_17DEC0 + 395) + *(unsigned __int8 *)((int)x_DWORD_17DEC0 + 395),
					v46 + 6 * xy_DWORD_17DEC0_spritestr[v46].height_5,
					0);
				return;
			}
			v22 = (signed __int16)v21;
			if (v24x[v22])
				break;
		LABEL_47:
			v21++;
		}
		if (x_D41A0_BYTEARRAY_4_struct.SelectedLangIndex != 2)
		{
			v43 = v21 * xy_DWORD_17DEC0_spritestr[65].height_5;
			sub_7FB90_draw_text(v24x[v22], (signed __int16)(v20 + 2 * xy_DWORD_17DEC0_spritestr[65].width_4), v43 + v46, 0);
			goto LABEL_46;
		}
		if ((unsigned __int16)v21 < 2u)
			goto LABEL_44;
		if ((unsigned __int16)v21 <= 2u)
		{
			sub_7FB90_draw_text(
				(char*)"Hit Accuracy",
				(signed __int16)(v20 + 2 * xy_DWORD_17DEC0_spritestr[65].width_4),
				v46 + v21 * xy_DWORD_17DEC0_spritestr[65].height_5,
				0);
		}
		else
		{
			if ((x_WORD)v21 != 4)
			{
			LABEL_44:
				sub_7FB90_draw_text(
					v24x[v21],
					(signed __int16)(v20 + 2 * xy_DWORD_17DEC0_spritestr[65].width_4),
					xy_DWORD_17DEC0_spritestr[65].height_5 * (x_WORD)v21 + v47 + 2,
					0);
				goto LABEL_46;
			}
			sub_7FB90_draw_text(
				(char*)"Mana Collected",
				(signed __int16)(v20 + 2 * xy_DWORD_17DEC0_spritestr[65].width_4),
				v46 + 4 * xy_DWORD_17DEC0_spritestr[65].height_5,
				0);
		}
	LABEL_46:
		sprintf(printbuffer, "%3d%%", v30x[1 + v21]);
		sub_7FB90_draw_text(
			printbuffer,
			(signed __int16)(v20 + v11 - 8 * xy_DWORD_17DEC0_spritestr[65].width_4),
			v47 + 2 + v21 * xy_DWORD_17DEC0_spritestr[65].height_5,
			0);
		goto LABEL_47;
	}
}

//----- (00085C8B) --------------------------------------------------------
void DrawNetGameMapBackground_85C8B(uint8_t* a1, uint8_t* a2, int a3, int a4, int a5, int a6)//266c8b
{
	int v6; // edx
	char* v7; // edi
	char* v8; // esi

	v6 = a6;
	v7 = (char*)a2;
	v8 = (char*)(a3 + 1280 * a4 + a1);
	do
	{
		qmemcpy(v7, v8, 4 * a5);
		v7 += 4 * a5;
		v8 += 4 * a5 + 640;
		--v6;
	} while (v6);
}

bool DrawAndServe_pre_sub_7B250(uint32_t var, type_WORD_E1F84* var2x)
{
	bool callres = true;
	switch (var) {
	case 0x258350: {
		return NewGameDialog_77350(var2x);//258350 - new game
		break;
	}
	case 0x258980: {
		return ExitDialog_77980(var2x);//exit dialog
		break;
	}
	case 0x2589e0: {
		return LanguageSettingDialog_779E0(var2x);
		break;
	}
	case 0x2590f0: {
		return LoadGameDialog_780F0(var2x);//2590f0 - load
		break;
	}
	case 0x259730: {
		return SaveGameDialog_78730(var2x);//259730 -save
		break;
	}
	case 0x259e00: {
		return SetPlayerNameDialog_78E00(var2x);//set user name
		break;
	}
	case 0x25a160: {//set joystick
		return SetJoystickDialog_79160(var2x);
		break;
	}
	case 0x25a610: {//set keys
		return SetKeysDialog_79610();
		break;
	}
	case 0x25dcf0: {
		return sub_7CCF0(/*var*/);
		break;
	}
	case 0x25dd30: {
		return sub_7CD30(/*var*/);
		break;
	}
	case 0x25dca0: {
		return sub_7CCA0(/*var*/);
		break;
	}
	case 0x25dda0: {
		return sub_7CDA0(/*var*/);
		break;
	}
	case 0x25dbf0: {
		return sub_7CBF0(/*var*/);
		break;
	}
	case 0x25dc40: {
		return sub_7CC40(/*var*/);
		break;
	}
	case 0x25ee80: {
		return MultiplayerMenu_7DE80(var2x);
		break;
	}
	}
	return callres;
}

//----- (00077980) --------------------------------------------------------
char ExitDialog_77980(type_WORD_E1F84* a1x)//258980
{
	__int16 v1; // ax
	char v2; // bl
	//a1 ma byt 2b2cb4
	v1 = DrawScrollDialog_7BF20(&a1x->str_26);//draw exit dialog
	v2 = v1;
	if (v1 == 1)
	{
		x_WORD_E29DC = 1;
		D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].byte_0x004_2BE0_11234 = 1;//duplicate command in other structure
		//*(x_BYTE *)(2124 * D41A0_BYTESTR_0.word_0xc + x_D41A0_BYTEARRAY_0 + 11234) = 1;
	}
	else if (v1 == 2)
	{
		v2 = 1;
		ClearScrollDialogVars_7C020(&a1x->str_26);
	}
	return v2;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// E29DC: using guessed type __int16 x_WORD_E29DC;

//----- (00078E00) --------------------------------------------------------
char SetPlayerNameDialog_78E00(/*int a1, int a2, */type_WORD_E1F84* a3x)//259e00
{
	int v3; // eax
	int v4; // esi
	int v5; // esi
	const char* v6; // edi
	char* v7; // edx
	char v8; // al
	__int16 v9; // si
	unsigned __int8 v10; // al
	__int16 v11; // bx
	unsigned __int8 v12; // al
	char* v13; // esi
	char* v14; // edi
	//char v15; // al
	//char v16; // al
	char* v17; // edi
	char* v18; // esi
	//char v19; // al
	//char v20; // al
	char* v21; // edi
	char* v22; // esi
	//char v23; // al
	//char v24; // al
	int v26; // [esp+0h] [ebp-14h]
	int v27; // [esp+4h] [ebp-10h]
	char v28; // [esp+8h] [ebp-Ch]
	int v29; // [esp+Ch] [ebp-8h]
	char v30; // [esp+10h] [ebp-4h]

	v28 = 0;
	v26 = j___clock();
	/*v3 = */DrawHelpText_6FC50(1);
	//LOWORD(v3) = a3[13];
	v29 = a3x->str_26.x1_26_0 + 10;
	//HIWORD(v3) = HIWORD(x_DWORD_17DED4);
	v3 = xy_DWORD_17DED4_spritestr[72].width_4;
	v27 = v29 + v3 - 22;
	v4 = a3x->str_26.y1_28_1 + a3x->str_26.word_34_4 / 2;
	v5 = v4 - sub_6FC30_get34_height();
	v30 = DrawScrollDialog_7BF20(&a3x->str_26);
	if (a3x->str_26.word_36_5 <= a3x->str_26.word_34_4 / 2)
		goto LABEL_15;
	x_DWORD_17DE38str.x_WORD_17DEF0 = strlen((const char*)x_DWORD_17DE38str.x_DWORD_17DE50);
	if ((unsigned __int8)x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode < 0xFu)
	{
		if (x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode != 14)
		{
		LABEL_9:
			if ((unsigned __int16)x_DWORD_17DE38str.x_WORD_17DEF0 < 0xCu && x_DWORD_17DE38str.x_BYTE_17DF11_last_key_status && sub_7C200(x_DWORD_17DE38str.x_BYTE_17DF11_last_key_status))
			{
				v7 = (char*)(x_DWORD_17DE38str.x_DWORD_17DE50 + x_DWORD_17DE38str.x_WORD_17DEF0);
				v8 = x_DWORD_17DE38str.x_BYTE_17DF11_last_key_status;
				v7[1] = 0;
				*v7 = v8;
			}
			goto LABEL_13;
		}
	LABEL_7:
		if (x_DWORD_17DE38str.x_WORD_17DEF0)
		{
			v6 = (const char*)x_DWORD_17DE38str.x_DWORD_17DE50;
			*(x_BYTE*)(strlen((const char*)x_DWORD_17DE38str.x_DWORD_17DE50) + x_DWORD_17DE38str.x_DWORD_17DE50 - 1) = 0;
			x_DWORD_17DE38str.x_WORD_17DEF0 = strlen(v6);
		}
		goto LABEL_13;
	}
	if ((unsigned __int8)x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode > 0xFu)
	{
		if (x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode != 83)
			goto LABEL_9;
		goto LABEL_7;
	}
LABEL_13:
	_strupr(x_DWORD_17DE38str.x_DWORD_17DE50);
	DrawHelpText_6FC50(1);
	v9 = sub_6FC30_get34_height() + v5;
	v10 = getPaletteIndex_5BE80(x_DWORD_17DE38str.x_DWORD_17DE38x, 0x16u, 0x10u, 9u);
	v11 = sub_6FC80_pre_draw_text((char*)x_DWORD_17DE38str.x_DWORD_17DE50, v29, v27, v9, v10);
	if ((v26 - x_DWORD_17DE38str.x_DWORD_17DED0) / 0xCu >= 1)
	{
		v12 = getPaletteIndex_5BE80(x_DWORD_17DE38str.x_DWORD_17DE38x, 0x16u, 0x10u, 9u);
		DrawText_2BC10((char*)"_", v11 + 2, v9, v12);
		x_DWORD_17DE38str.x_DWORD_17DED0 = v26;
	}
LABEL_15:
	if (v30)
	{
		if (v30 == 1)
		{
			v13 = x_DWORD_17DE38str.x_DWORD_17DE50;
			v14 = D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].array_0x39f_2BFA_12157;//wizard name
			strcpy(v14, v13);
			/*do
			{
				v15 = *v13;
				*v14 = *v13;
				if (!v15)
					break;
				v16 = v13[1];
				v13 += 2;
				v14[1] = v16;
				v14 += 2;
			} while (v16);*/
			v17 = x_D41A0_BYTEARRAY_4_struct.player_name_57ar;//38cf89
			v18 = D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].array_0x39f_2BFA_12157;//wizard name
			strcpy(v17, v18);
			/*do
			{
				v19 = *v18;
				*v17 = *v18;
				if (!v19)
					break;
				v20 = v18[1];
				v18 += 2;
				v17[1] = v20;
				v17 += 2;
			} while (v20);*/
		}
		else
		{
			*x_D41A0_BYTEARRAY_4_struct.player_name_57ar = 0;//fixed x_D41A0_BYTEARRAY_4_struct.player_name_57 = 0;
		}
		v21 = x_DWORD_17DE38str.x_DWORD_17DE50;
		v22 = D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].array_0x39f_2BFA_12157;//wizard name
		strcpy(v21, v22);
		/*do
		{
			v23 = *v22;
			*v21 = *v22;
			if (!v23)
				break;
			v24 = v22[1];
			v22 += 2;
			v21[1] = v24;
			v21 += 2;
		} while (v24);*/
		ClearScrollDialogVars_7C020(&a3x->str_26);
		v28 = 1;
	}
	return v28;
}
// 98786: using guessed type int /*__fastcall*/ j___clock(x_DWORD, x_DWORD, x_DWORD);
// 9A20A: using guessed type x_DWORD _strupr_s(x_DWORD);
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// 17DE38: using guessed type int x_DWORD_17DE38;
// 17DE50: using guessed type int (int)x_DWORD_17DE50;
// 17DED0: using guessed type int x_DWORD_17DED0;
// 17DED4: using guessed type int (int)x_DWORD_17DED4;
// 17DEF0: using guessed type __int16 x_WORD_17DEF0;
// 17DF10: using guessed type char x_BYTE_17DF10_get_key_scancode;
// 17DF11: using guessed type char x_BYTE_17DF11_last_key_status;

//----- (0007F6A0) --------------------------------------------------------
int sub_7F6A0(int  /*a1*/, int  /*a2*/, __int16 a3, __int16 a4, char* a5, int8_t* a6, unsigned __int8 a7)//2606a0
{
	uint8_t v7; // al
	__int16 v8; // si
	int v10; // [esp+0h] [ebp-4h]

	v10 = j___clock();
	v7 = x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode;
	*a6 = strlen(a5);
	if (v7 < 0xFu)
	{
		if (v7 < 1u)
			goto LABEL_12;
		if (v7 <= 1u)
			goto LABEL_16;
		if (v7 != 14)
			goto LABEL_12;
	}
	else
	{
		if (v7 <= 0xFu)
			goto LABEL_16;
		if (v7 < 0x1Cu)
			goto LABEL_12;
		if (v7 <= 0x1Cu)
			goto LABEL_16;
		if (v7 != 83)
		{
		LABEL_12:
			if (*a6 < 0xFu && x_DWORD_17DE38str.x_BYTE_17DF11_last_key_status && sub_7C200(x_DWORD_17DE38str.x_BYTE_17DF11_last_key_status))
			{
				a5[(unsigned __int8)*a6] = x_DWORD_17DE38str.x_BYTE_17DF11_last_key_status;
				a5[(unsigned __int8)*a6 + 1] = 0;
			}
			goto LABEL_16;
		}
	}
	if (*a6)
	{
		a5[strlen(a5) - 1] = 0;
		*a6 = strlen(a5);
	}
LABEL_16:
	_strupr(a5);
	DrawText_2BC10(a5, a3, a4, a7);
	if ((v10 - x_DWORD_17DE38str.x_DWORD_17DED0) / 0xCu >= 1)
	{
		v8 = sub_6FC10_letter_width();
		DrawText_2BC10((char*)"_", a3 + v8 * strlen(a5), a4, a7);
		x_DWORD_17DE38str.x_DWORD_17DED0 = v10;
	}
	return 0;
}
// 98786: using guessed type int /*__fastcall*/ j___clock(x_DWORD, x_DWORD, x_DWORD);
// 9A20A: using guessed type x_DWORD _strupr_s(x_DWORD);
// 17DED0: using guessed type int x_DWORD_17DED0;
// 17DF10: using guessed type char x_BYTE_17DF10_get_key_scancode;
// 17DF11: using guessed type char x_BYTE_17DF11_last_key_status;

//----- (00079160) --------------------------------------------------------
char SetJoystickDialog_79160(type_WORD_E1F84* locEvent)//25a160
{
	int locY1;
	int tempJoySetting1;
	unsigned int tempJoySetting2;
	type_WORD_E1F84 locRegion;
	int locX1;
	char result;
	int locX2;
	char scrollState;
	unsigned __int8 color2;
	unsigned __int8 color1;

	int lastSelected = 0;
	result = 0;
	color1 = getPaletteIndex_5BE80(x_DWORD_17DE38str.x_DWORD_17DE38x, 0x3Fu, 0x3Fu, 0x3Fu);
	color2 = getPaletteIndex_5BE80(x_DWORD_17DE38str.x_DWORD_17DE38x, 0, 0, 0);
	switch (locEvent->byte_25)
	{
	case 0:
	{
		locX1 = locEvent->str_26.x1_26_0 + 10;
		locX2 = locX1 + xy_DWORD_17DED4_spritestr[72].width_4 - 22;
		locY1 = locEvent->str_26.y1_28_1 + locEvent->str_26.word_34_4 / 2 - 2 * sub_6FC30_get34_height();
		scrollState = DrawScrollDialog_7BF20(&locEvent->str_26);
		if (locEvent->str_26.word_36_5 >= locEvent->str_26.word_34_4)
		{
			DrawHelpText_6FC50(1);
			for (int i = 0; str_BYTE_E2590[i].word_12; i++)
			{
				if (str_BYTE_E2590[i].word_14)
				{
					DrawHelpText_6FC50(1);
					sub_6FC80_pre_draw_text(x_DWORD_E9C4C_langindexbuffer[str_BYTE_E2590[i].word_12], locX1, locX2, locY1, color1);
					lastSelected = i;
				}
				else
				{
					DrawHelpText_6FC50(1);
					sub_6FC80_pre_draw_text(x_DWORD_E9C4C_langindexbuffer[str_BYTE_E2590[i].word_12], locX1, locX2, locY1, color2);
				}
				locY1 += sub_6FC30_get34_height();
			}
			locY1 = locEvent->str_26.y1_28_1 + locEvent->str_26.word_34_4 / 2 - 2 * sub_6FC30_get34_height();
			locRegion.xmin_10 = locX1;
			locRegion.sizex_14 = xy_DWORD_17DED4_spritestr[72].width_4;
			locRegion.sizey_16 = sub_6FC30_get34_height();
			for (int i = 0; str_BYTE_E2590[i].word_12; i++)
			{
				locRegion.ymin_12 = locY1;
				if (InRegion_7B200(&locRegion, x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx, x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony))
				{
					if ((x_DWORD_17DE38str.x_WORD_17DEEE_mouse_buttons & 1) != 0)
					{
						if (&str_BYTE_E2590[lastSelected].word_0)
							str_BYTE_E2590[lastSelected].word_14 = 0;
						str_BYTE_E2590[i].word_14 = 1;
					}
					DrawHelpText_6FC50(1);
					sub_6FC80_pre_draw_text(x_DWORD_E9C4C_langindexbuffer[str_BYTE_E2590[i].word_12], locX1, locX2, locY1, color1);
				}
				locY1 += sub_6FC30_get34_height();
			}
		}
		if (scrollState)
		{
			for (int i = 0; str_BYTE_E2590[i].word_12; i++)
			{
				if (str_BYTE_E2590[i].word_14)
				{
					lastSelected = i;
					break;
				}
				locY1 += sub_6FC30_get34_height();
			}
			if (scrollState != 1)
			{
				switch (unk_18058Cstr.x_WORD_1805C2_joystick)
				{
				case 4:
				case 11:
					unk_18058Cstr.x_WORD_1805C2_joystick = 7;
					break;
				case 8:
					unk_18058Cstr.x_WORD_1805C2_joystick = 1;
					break;
				case 9:
					unk_18058Cstr.x_WORD_1805C2_joystick = 2;
					break;
				default:
					result = 1;
					ClearScrollDialogVars_7C020(&locEvent->str_26);
					return result;
				}
				result = 1;
				ClearScrollDialogVars_7C020(&locEvent->str_26);
				return result;
			}
			result = 1;
			tempJoySetting1 = unk_18058Cstr.x_WORD_1805C2_joystick;
			if (!str_BYTE_E2590[lastSelected].word_0)
			{
				ClearScrollDialogVars_7C020(&locEvent->str_26);
				return result;
			}
			tempJoySetting2 = unk_18058Cstr.x_WORD_1805C2_joystick - 1;
			if (str_BYTE_E2590[lastSelected].word_16 == 2)
			{
				if (sub_89B60_aplicate_setting(4u) == -1)
				{
					unk_18058Cstr.x_WORD_1805C2_joystick = tempJoySetting1;
				}
				else if (tempJoySetting2 <= 0xC)
				{
					switch (tempJoySetting1)
					{
					case 1:
					case 12:
						unk_18058Cstr.x_WORD_1805C2_joystick = 8;
						break;
					case 2:
					case 13:
						unk_18058Cstr.x_WORD_1805C2_joystick = 9;
						break;
					case 3:
					case 4:
					case 5:
					case 6:
					case 8:
					case 9:
					case 10:
						break;
					case 7:
					case 11:
						unk_18058Cstr.x_WORD_1805C2_joystick = 4;
						break;
					}
				}
				result = 1;
				ClearScrollDialogVars_7C020(&locEvent->str_26);
				return result;
			}
			if (str_BYTE_E2590[lastSelected].word_16 != 1)
			{
				ClearScrollDialogVars_7C020(&locEvent->str_26);
				return result;
			}
			if (sub_89B60_aplicate_setting(0xBu) != -1)
			{
				if (tempJoySetting2 <= 8)
				{
					switch (tempJoySetting1)
					{
					case 1:
					case 8:
						unk_18058Cstr.x_WORD_1805C2_joystick = 12;
						break;
					case 2:
					case 9:
						unk_18058Cstr.x_WORD_1805C2_joystick = 13;
						break;
					case 3:
					case 5:
					case 6:
						break;
					case 4:
					case 7:
						unk_18058Cstr.x_WORD_1805C2_joystick = 11;
						break;
					}
				}
				locEvent->byte_25 = 1;
				result = 0;
				ClearScrollDialogVars_7C020(&locEvent->str_26);
				return result;
			}
			unk_18058Cstr.x_WORD_1805C2_joystick = tempJoySetting1;
			ClearScrollDialogVars_7C020(&locEvent->str_26);
		}
		return result;
	}
	case 1:
	{
		locEvent->str_26.word_38_6 = 439;
		locEvent->str_26.nocl_42_8 = 0;
		break;
	}
	case 2:
	{
		locEvent->str_26.word_38_6 = 439;
		break;
	}
	}
	DrawScrollDialog_7BF20(&locEvent->str_26);
	if (locEvent->str_26.word_36_5 < locEvent->str_26.word_34_4)
		return result;
	if (locEvent->byte_25 < 2u)
	{
		if (locEvent->byte_25 == 1)
		{
			locEvent->byte_25 = 2;
			return result;
		}
		return result;
	}
	if (locEvent->byte_25 > 2u)
	{
		if (locEvent->byte_25 != 3)
			return result;
		locEvent->byte_25 = 0;
		locEvent->str_26.word_38_6 = 411;
		locEvent->str_26.nocl_42_8 = 3;
		ClearScrollDialogVars_7C020(&locEvent->str_26);
		return 1;
	}
	x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode = 0;
	sub_8C0E0((unsigned __int8(*)(signed int))sub_7A060_get_mouse_and_keyboard_events);
	if (x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode == 1)
	{
		if (unk_18058Cstr.x_WORD_1805C2_joystick < 0xCu)
		{
			if (unk_18058Cstr.x_WORD_1805C2_joystick == 11)
				unk_18058Cstr.x_WORD_1805C2_joystick = 7;
		}
		else if (unk_18058Cstr.x_WORD_1805C2_joystick <= 0xCu)
		{
			unk_18058Cstr.x_WORD_1805C2_joystick = 1;
		}
		else if (unk_18058Cstr.x_WORD_1805C2_joystick == 13)
		{
			unk_18058Cstr.x_WORD_1805C2_joystick = 2;
		}
	}
	locEvent->byte_25++;
	return result;
}


//----- (0007CCF0) --------------------------------------------------------
int sub_7CCF0()//25dcf0
{
	if (x_DWORD_17DE38str.array_BYTE_17DE68x[x_DWORD_17DE38str.serverIndex_17DEFC].selectedLevel_10 > 50)
	{
		x_DWORD_17DE38str.array_BYTE_17DE68x[x_DWORD_17DE38str.serverIndex_17DEFC].selectedLevel_10--;
		x_DWORD_17DE38str.array_BYTE_17DE68x[x_DWORD_17DE38str.serverIndex_17DEFC].action_9 = 6;
	}
	return 0;
}

//----- (0007CD30) --------------------------------------------------------
int sub_7CD30()//25dd30
{
	char* v0 = (char*)(LevelsNames_D9204)[1 + x_DWORD_17DE38str.array_BYTE_17DE68x[x_DWORD_17DE38str.serverIndex_17DEFC].selectedLevel_10];
	if (v0[0] && v0[0] != 48)
	{
		x_DWORD_17DE38str.array_BYTE_17DE68x[x_DWORD_17DE38str.serverIndex_17DEFC].selectedLevel_10++;
		x_DWORD_17DE38str.array_BYTE_17DE68x[x_DWORD_17DE38str.serverIndex_17DEFC].action_9 = 6;
	}
	return 0;
}

//----- (0007CCA0) --------------------------------------------------------
int sub_7CCA0()//25dca0
{
	if (x_DWORD_17DE38str.serverIndex_17DEFC == GetIndexNetwork2_74515() && x_DWORD_17DE38str.x_WORD_17DEFE == 1 || x_DWORD_17DE38str.serverIndex_17DEFC != GetIndexNetwork2_74515())
		x_DWORD_17DE38str.array_BYTE_17DE68x[x_DWORD_17DE38str.serverIndex_17DEFC].action_9 = 1;
	return 0;
}

//----- (0007CDA0) --------------------------------------------------------
int sub_7CDA0()//25dda0
{
	x_DWORD_17DE38str.array_BYTE_17DE68x[x_DWORD_17DE38str.serverIndex_17DEFC].action_9 = 5;
	return 0;
}

//----- (0007CBF0) --------------------------------------------------------
char sub_7CBF0()//25dbf0
{
	x_DWORD_17DE38str.x_WORD_17DEF8 = 0;
	x_DWORD_17DE38str.array_BYTE_17DE68x[x_DWORD_17DE38str.serverIndex_17DEFC].action_9 = 3;
	x_DWORD_17DE38str.palMulti_17DF02 = 0;
	x_DWORD_17DE38str.x_WORD_17DEF6 = 2;
	x_DWORD_17DE38str.x_WORD_17DEF2 = x_DWORD_17DE38str.x_WORD_17DEF4;
	return 0;
}

//----- (0007CC40) --------------------------------------------------------
char sub_7CC40()//25dc40
{
	x_DWORD_17DE38str.palMulti_17DF02 = 0;
	x_DWORD_17DE38str.x_WORD_17DEF8 = 1;
	x_DWORD_17DE38str.array_BYTE_17DE68x[x_DWORD_17DE38str.serverIndex_17DEFC].action_9 = 4;
	x_DWORD_17DE38str.x_WORD_17DEF6 = 2;
	x_DWORD_17DE38str.x_WORD_17DEF2 = x_DWORD_17DE38str.x_WORD_17DEF4;
	return 0;
}

//----- (0007DE80) --------------------------------------------------------
char MultiplayerMenu_7DE80(type_WORD_E1F84* a2x)//25ee80
{
	//int v2; // edx
	int v3; // edi
	//int v4; // eax
	//signed __int16* v5; // ebx
	//int v6; // edx
	//int v7; // esi
	int v8; // esi
	//char v9; // al
	//int v10; // edx
	bool v11 = false; // zf
	__int16 v12; // si
	//unsigned __int8 v13; // al
	__int16 v14; // si
	//unsigned __int8 v15; // al
	//int v16; // eax
	//int v17; // edx
	//char v19; // [esp+0h] [ebp-20h]
	int v20; // [esp+Ch] [ebp-14h]
	//__int16 v21; // [esp+10h] [ebp-10h]
	int v22; // [esp+14h] [ebp-Ch]
	char v23; // [esp+18h] [ebp-8h]
	char v24; // [esp+1Ch] [ebp-4h]

	memset(printbuffer, 0, 10);
	a2x->dword_4 = 0;
	v3 = j___clock();
	DrawHelpText_6FC50(1);
	v22 = a2x->str_26.x1_26_0 + 10;
	v20 = v22 + xy_DWORD_17DED4_spritestr[72].width_4 - 22;
	v8 = a2x->str_26.word_34_4 / 2 + a2x->str_26.y1_28_1 - sub_6FC30_get34_height();
	v23 = DrawScrollDialog_7BF20(&a2x->str_26);
	if (a2x->str_26.word_36_5 > a2x->str_26.word_34_4 / 2)
	{
		if ((unsigned __int8)x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode != 0xFu)
		{
			if ((unsigned __int8)x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode < 0xFu)
			{
				v11 = x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode == 14;
			}
			if ((unsigned __int8)x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode > 0xFu)
			{
				v11 = x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode == 83;
			}
			if (!v11
				&& (unsigned __int16)x_DWORD_17DE38str.x_WORD_17DEF0 < 0xCu
				&& (unsigned __int8)x_DWORD_17DE38str.x_BYTE_17DF11_last_key_status >= 0x30u
				&& (unsigned __int8)x_DWORD_17DE38str.x_BYTE_17DF11_last_key_status <= 0x39u)
			{
				x_BYTE_E29DF_skip_screen = x_DWORD_17DE38str.x_BYTE_17DF11_last_key_status - '0';
			}
		}
		sprintf(printbuffer, "%d", (unsigned __int8)x_BYTE_E29DF_skip_screen);
		DrawHelpText_6FC50(1);
		v12 = sub_6FC30_get34_height() + v8;
		v14 = sub_6FC80_pre_draw_text(printbuffer, v22, v20, v12, getPaletteIndex_5BE80(x_DWORD_17DE38str.x_DWORD_17DE38x, 0, 0, 0));
		if ((v3 - x_DWORD_17DE38str.x_DWORD_17DED0) / 0xCu >= 1)
		{
			DrawHelpText_6FC50(1);
			DrawText_2BC10((char*)"_", v14 - 2, v12, getPaletteIndex_5BE80(x_DWORD_17DE38str.x_DWORD_17DE38x, 0, 0, 0));
			x_DWORD_17DE38str.x_DWORD_17DED0 = v3;
		}
	}
	if (v23)
	{
		ClearScrollDialogVars_7C020(&a2x->str_26);
		if (v23 == 1)
		{
			x_WORD_E131A = 0;
			x_DWORD_17DE38str.networkSession_17DEFA = (unsigned __int8)x_BYTE_E29DF_skip_screen;
			ResetMouse_7B5A0();
			if (1 == x_D41A0_BYTEARRAY_4_struct.byteindex_10)
				v24 = x_DWORD_17DE38str.x_BYTE_17DF13;
			sub_8CD27_set_cursor((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]);
			a2x->dword_4 = sub_77680() != 0;
			if (x_D41A0_BYTEARRAY_4_struct.byteindex_10 == 1)
				x_DWORD_17DE38str.x_BYTE_17DF13 = v24;
		}
		else
		{
			x_BYTE_E29DF_skip_screen = x_DWORD_17DE38str.networkSession_17DEFA;
		}
		return 1;
	}
	return 0;
}

//----- (0007BF20) --------------------------------------------------------
int DrawScrollDialog_7BF20(type_str_word_26* a1x)//25cf20
{
	char* v1; // esi
	//int result; // eax
	//int v3; // edx
	signed __int16 v4; // di
	signed __int16 v5; // cx
	//int v6; // eax
	signed __int16 v7; // ax

	v1 = 0;
	//result = 0;
	if (!a1x->x1_26_0)
		return 0;
	if (x_D41A0_BYTEARRAY_4_struct.byteindex_10 == 1)
		x_DWORD_17DE38str.x_BYTE_17DF13 = 0;
	DrawHelpText_6FC50(1);
	if (!a1x->click_40_7)
	{
		v4 = a1x->word_36_5;
		if (v4)
		{
			if (v4 <= a1x->word_34_4 - 1)
				a1x->word_36_5 = v4 + 16;
			else
				a1x->word_36_5 = a1x->word_34_4;
		}
		else
		{
			a1x->word_36_5++;//next char
		}
		goto LABEL_14;
	}
	v5 = a1x->word_36_5 - 16;
	a1x->word_36_5 = v5;
	if (v5 >= 0)
	{
	LABEL_14:
		if (a1x->word_36_5 > sub_6FC30_get34_height() + 10)
			v1 = x_DWORD_E9C4C_langindexbuffer[a1x->word_38_6];
		//160 1a 21 2b2cce? exit game 3
		v7 = DrawScrollDialog2_7B660(a1x->x1_26_0, a1x->y1_28_1, a1x->word_36_5, a1x, v1, a1x->nocl_42_8 & 0xff);
		if (v7)
		{
			if (!a1x->click_40_7)
				a1x->click_40_7 = v7;
		}
		return 0;
	}
	a1x->word_36_5 = 0;
	//v6 = (int)x_D41A0_BYTEARRAY_4;
	int16_t result = a1x->click_40_7;
	a1x->click_40_7 = 0;
	if (x_D41A0_BYTEARRAY_4_struct.byteindex_10 == 1)
		x_DWORD_17DE38str.x_BYTE_17DF13 = 1;
	return result;
}
// D41A4: using guessed type int x_DWORD_D41A4;
// 17DF13: using guessed type char x_BYTE_17DF13;

//----- (0008C0E0) --------------------------------------------------------
void sub_8C0E0(unsigned __int8(/*__fastcall*/ *a1)(signed int))//26d0e0
{
	signed int v1=0; // eax
	signed int v2=0; // eax

	unk_180560x[0x1a] = 0;
	while (!(unk_180560x[0x1a] & 1))
	{
		v1 = sub_8BBE0(unk_180560x);
		if (a1 && a1(v1))
			return;
	}
	while (unk_180560x[0x1a] & 1)
	{
		v2 = sub_8BBE0(unk_180560x);
		if (a1)
		{
			if (a1(v2))
				break;
		}
	}
}
// 180586: using guessed type char x_BYTE_180586;

//----- (0007B660) --------------------------------------------------------
int DrawScrollDialog2_7B660(int a1, int a2, __int16 a3, type_str_word_26* a4x, char* a5, char a6)//25c660
{
	//int v6; // ebx
	unsigned __int8 v7; // al
	//uint8_t* v8; // edx
	//__int16 v9; // ax
	//int v10; // eax
	//int v11; // ecx
	//int v12; // eax
	//uint8_t* v13; // edx
	//int v14; // esi
	//int v15; // esi
	//int v16; // eax
	//uint8_t* v17; // ecx
	//__int16 v18; // bx
	//uint8_t* v19; // edx
	__int16 v20; // si
	__int16 v21; // ax
	__int16 v22; // cx
	__int16 v23; // bx
	int v24x; // edx
	//int v25x; // eax
	int v26; // eax
	__int16 v27; // cx
	//uint8_t* v28; // edx
	//uint8_t* v29; // eax
	int v30; // ebx
	unsigned __int8 v31; // al
	type_WORD_E1F84 v33x; // [esp+0h] [ebp-4Ch]
	//__int16 v34; // [esp+Ah] [ebp-42h]
	//__int16 v35; // [esp+Ch] [ebp-40h]
	//__int16 v36; // [esp+Eh] [ebp-3Eh]
	//__int16 v37; // [esp+10h] [ebp-3Ch]
	//int v38; // [esp+2Ch] [ebp-20h]
	//int v39; // [esp+30h] [ebp-1Ch]
	//int v40; // [esp+34h] [ebp-18h]
	//int v41; // [esp+38h] [ebp-14h]
	int v42; // [esp+3Ch] [ebp-10h]
	//int v43; // [esp+40h] [ebp-Ch]
	unsigned __int8 v44; // [esp+44h] [ebp-8h]
	unsigned __int8 v45; // [esp+48h] [ebp-4h]
	signed __int16 v46; // [esp+64h] [ebp+18h]

	//HIWORD(v6) = HIWORD(a4);
	v44 = 0;
	if (!a3)
	{
		sub_7C120_draw_bitmap_640(a1, a2, xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF06]);//unknown draw
		goto LABEL_31;
	}
	v45 = getPaletteIndex_5BE80(x_DWORD_17DE38str.x_DWORD_17DE38x, 0x25u, 0x1Fu, 0x19u);
	//v38 = (signed __int16)a2;
	//v41 = (signed __int16)a1;
	sub_7C120_draw_bitmap_640(a1, a2, xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF06]);//top scroll border
	v7 = getPaletteIndex_5BE80(x_DWORD_17DE38str.x_DWORD_17DE38x, 0x2Au, 0x24u, 0x1Du);
	sub_7C140_draw_text_background(
		(a1 + 10),
		a2 + xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF06].height_5 - 2,
		xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF06].width_4 - 22,
		a3,
		v7);
	//v39 = v45;
	//v40 = (signed __int16)(a3 + a2);
	sub_2BD10_draw_line(a1 + 10, a2, a1 + 10, a3 + a2, v45);
	//v8 = xy_DWORD_17DED4_spritestr[x_WORD_17DF06].pointer;
	//v9 = a2 + xy_DWORD_17DED4_spritestr[x_WORD_17DF06].sizey - 2;
	sub_2BD10_draw_line(a1 + xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF06].width_4 - 12, a2 + xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF06].height_5 - 2, a1 + xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF06].width_4 - 12, a2 + xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF06].height_5 - 2 + a3, v45); // to je ta linka, kterou je treba opravit
	sub_7C120_draw_bitmap_640(a1, a3 + a2, xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF06]);//bottom scroll border
	if (a4x->word_36_5 >= a4x->word_34_4)
	{
		switch (a6)
		{
		case 3:
			//v10 = a4[0];
			//v43 = v10 + 15;
			//v12 = 6 * x_WORD_17DF0C;
			//v13 = v12 + xy_DWORD_17DED4_spritestr[0].pointer;
			//v11 = a4[4];
			//v12 = a4[1];
			//v14 = xy_DWORD_17DED4_spritestr[x_WORD_17DF0C].sizey;
			//HIWORD(v14) = (signed __int16)a1 >> 15;
			//v15 = v11 + v12 - v14;
			v33x.ymin_12 = a4x->word_34_4 + a4x->y1_28_1 - xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF0C].height_5;//12 - v35
			v33x.xmin_10 = a4x->x1_26_0 + 15;//10 - v34
			v33x.sizex_14 = xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF0C].width_4;//14 adress 25c832
			v33x.sizey_16 = xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF0C].height_5;//16
			if (InRegion_7B200(&v33x, x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx, x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony))//icon OK
			{
				sub_7C120_draw_bitmap_640(a4x->x1_26_0 + 15, v33x.ymin_12, xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF0E]);
				if (x_DWORD_17DE38str.x_WORD_17DEEE_mouse_buttons & 1)
					v44 = 1;
			}
			else
			{
				sub_7C120_draw_bitmap_640(a4x->x1_26_0 + 15, v33x.ymin_12, xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF0C]);
			}
			//v16 = 3 * x_WORD_17DF06;
			//v16 = xy_DWORD_17DED4_spritestr[x_WORD_17DF06].sizex;
			//v6 = a4[0];
			v42 = xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF06].width_4 - 12 + a4x->x1_26_0;
			//v17 = xy_DWORD_17DED4_spritestr[x_WORD_17DF06].pointer;
			//v16 = v42 - xy_DWORD_17DED4_spritestr[x_WORD_17DF08].sizex;
			//v33[6] = v15;
			//v18 = v42 - xy_DWORD_17DED4_spritestr[x_WORD_17DF08].sizex;
			v33x.xmin_10 = v42 - xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF08].width_4;
			v33x.sizex_14 = xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF08].width_4;
			//v39 = x_WORD_17DF0A;
			v33x.sizey_16 = xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF0A].height_5;
			if (InRegion_7B200(&v33x, x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx, x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony))//icon X - cancel
			{
				sub_7C120_draw_bitmap_640(v42 - xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF08].width_4, v33x.ymin_12, xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF0A]);
				if (x_DWORD_17DE38str.x_WORD_17DEEE_mouse_buttons & 1)
					v44 = 2;
			}
			else
			{
				sub_7C120_draw_bitmap_640(v42 - xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF08].width_4, v33x.ymin_12, xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF08]);//zlom
			}
			goto LABEL_23;
		case 1:
			//v19 = xy_DWORD_17DED4_spritestr[x_WORD_17DF0C].pointer;
			v20 = a4x->x1_26_0 + 30;
			v21 = a4x->word_34_4 + a4x->y1_28_1;
			v22 = xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF0C].height_5;
			v33x.xmin_10 = a4x->x1_26_0 + 30;
			v23 = v21 - v22;
			v33x.ymin_12 = v21 - v22;
			v33x.sizex_14 = xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF0C].width_4;
			v33x.sizey_16 = xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF0C].height_5;
			if (InRegion_7B200(&v33x, x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx, x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony))
			{
				sub_7C120_draw_bitmap_640(v20, v23, xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF0E]);
				if (x_DWORD_17DE38str.x_WORD_17DEEE_mouse_buttons & 1)
					v44 = 1;
				goto LABEL_23;
			}
			//v24 = (int)(int)x_DWORD_17DED4;
			v24x = x_DWORD_17DE38str.x_WORD_17DF0E;
			break;
		case 2:
			v26 = 3 * x_DWORD_17DE38str.x_WORD_17DF0C;
			v20 = a4x->x1_26_0 + 40;
			v26 = xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF0C].height_5;
			v27 = a4x->word_34_4;
			v42 = v26;
			v23 = v27 + a4x->y1_28_1 - v26;
			v33x.ymin_12 = v27 + a4x->y1_28_1 - v26;
			//v39 = x_WORD_17DF08;
			//v28 = xy_DWORD_17DED4_spritestr[x_WORD_17DF0C].pointer;
			v33x.xmin_10 = v20;
			v33x.sizex_14 = xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF0C].width_4;
			v33x.sizey_16 = xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF0C].height_5;
			if (InRegion_7B200(&v33x, x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx, x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony))
			{
				sub_7C120_draw_bitmap_640(v20, v23, xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF0A]);
				if (x_DWORD_17DE38str.x_WORD_17DEEE_mouse_buttons & 1)
					v44 = 1;
				goto LABEL_23;
			}
			v24x = x_DWORD_17DE38str.x_WORD_17DF08;
			//v25 = (int)x_DWORD_17DED4;
			break;
		default:
			goto LABEL_23;
		}
		sub_7C120_draw_bitmap_640(v20, v23, xy_DWORD_17DED4_spritestr[v24x]);
	LABEL_23:
		if (!v44)
		{
			if (x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode == 28)
			{
				v44 = 1;
			}
			else if (x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode == 1)
			{
				v44 = 2;
			}
		}
		if (CommandLineParams.ModeTestNetwork()) {
			if (first_enter)
			{
				first_enter = false;
				v44 = 1;
			}
		}
	}
	DrawHelpText_6FC50(1);
	//v29 = xy_DWORD_17DED4_spritestr[x_WORD_17DF06].pointer;
	v30 = a1 + 10 + xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF06].width_4 - 22;
	//v29 = xy_DWORD_17DED4_spritestr[x_WORD_17DF06].sizey;
	v46 = xy_DWORD_17DED4_spritestr[x_DWORD_17DE38str.x_WORD_17DF06].height_5 + 2 + a2;
	if (a5)
	{
		v31 = getPaletteIndex_5BE80(x_DWORD_17DE38str.x_DWORD_17DE38x, 0x16u, 0x10u, 9u);
		//"Exit Game" 16a 1c6 28 1 4c//adress 25cc27
		sub_7FCB0_draw_text_with_border(/*v30,*/ a5, a1 + 10, v30, v46, 1, v31, 0);
	}
LABEL_31:
	if (v44)
		sub_8F100_sound_proc19(0, 14, 127, 64, 0x64u, 0, 3u);
	return v44;
}
// 17DE38: using guessed type int x_DWORD_17DE38;
// 17DED4: using guessed type int (int)x_DWORD_17DED4;
// 17DEE4: using guessed type int x_DWORD_17DEE4_mouse_position;
// 17DEEE: using guessed type __int16 x_WORD_17DEEE_mouse_buttons;
// 17DF06: using guessed type __int16 x_WORD_17DF06;
// 17DF08: using guessed type __int16 x_WORD_17DF08;
// 17DF0A: using guessed type __int16 x_WORD_17DF0A;
// 17DF0C: using guessed type __int16 x_WORD_17DF0C;
// 17DF0E: using guessed type __int16 x_WORD_17DF0E;
// 17DF10: using guessed type char x_BYTE_17DF10_get_key_scancode;

//----- (00077680) --------------------------------------------------------
char /*__fastcall*/ sub_77680()//258680
{
	char result; // al
	//int v4; // eax
	int v5; // edx
	//int v6; // eax
	//__int16 v7; // dx
	//char* v8; // esi
	//char v9; // [esp+0h] [ebp-4h]
	//char v10ar[0x24]; // [esp+50h] [ebp+4Ch]
	type_E24BCx v10arx[2];
	//char v11; // [esp+60h] [ebp+5Ch]//v10ar[0x10]

	int v12[4]; // [esp+74h] [ebp+70h]
	int v13; // [esp+78h] [ebp+74h]
	__int16 v14; // [esp+80h] [ebp+7Ch]
	char v15; // [esp+82h] [ebp+7Eh]

	//fix it
	//v5 = 0;
	v14 = 0;
	//fix it

	timeCounter_E2094 = minTimeCounter_E2096;
	memset(printbuffer, 0, 80);
	memset(v12, 0, 16);
	v15 = 2;
	sprintf(printbuffer, "NETH%d", (unsigned __int16)x_DWORD_17DE38str.networkSession_17DEFA + 20);
	int8_t a3a = 0;
	int8_t a3b = 0;
	x_DWORD_17DE38str.serverIndex_17DEFC = NetworkInitConnection_7308F(printbuffer, 8);//-1 == false
	if (x_DWORD_17DE38str.serverIndex_17DEFC == -1)
	{
		sub_8CD27_set_cursor(xy_DWORD_17DED4_spritestr[39]);
		result = 0;
	}
	else
	{
		sub_7A110_load_hscreen(x_WORD_180660_VGA_type_resolution, 7);
		x_DWORD_17DE38str.x_WORD_17DEFE = 0;
		sub_8CD27_set_cursor((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]);
		x_DWORD_17DE38str.x_DWORD_17DEDC = 0;
		x_DWORD_17DE38str.x_WORD_17DEEE_mouse_buttons = 0;
		ResetMouse_7B5A0();
		memset(x_DWORD_17DE38str.array_BYTE_17DE68x, 0, sizeof(type_BYTE_17DE68x) * 8);
		memset(printbuffer, 0, 80);
		x_DWORD_17DE38str.x_WORD_17DEF6 = 5;
		x_DWORD_17DE38str.x_WORD_17DEEC = 0;
		SetCenterScreenForFlyAssistant_6EDB0();
		x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx = 0x140;
		x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony = 0xf0;//test and fix it

		if (x_D41A0_BYTEARRAY_4_struct.levelnumber_43w >= 0x32u)
			//x_DWORD_17DE38str.x_BYTE_17DE68x[0xa + 11 * x_DWORD_17DE38str.x_WORD_17DEFC] = *(x_BYTE*)(x_D41A0_BYTEARRAY_4_struct.levelnumber_43w);
			x_DWORD_17DE38str.array_BYTE_17DE68x[x_DWORD_17DE38str.serverIndex_17DEFC].selectedLevel_10 = x_D41A0_BYTEARRAY_4_struct.levelnumber_43w;
		else
			//x_DWORD_17DE38str.x_BYTE_17DE68x[0xa + 11 * x_DWORD_17DE38str.x_WORD_17DEFC] = 50;
			x_DWORD_17DE38str.array_BYTE_17DE68x[x_DWORD_17DE38str.serverIndex_17DEFC].selectedLevel_10 = 50;
		//x_DWORD_17DE38str.x_BYTE_17DE68x[0x9 + 11 * x_DWORD_17DE38str.x_WORD_17DEFC] = 2;
		x_DWORD_17DE38str.array_BYTE_17DE68x[x_DWORD_17DE38str.serverIndex_17DEFC].action_9 = 2;
		x_DWORD_17DE38str.x_WORD_17DEEE_mouse_buttons = 0;
		while (!a3a)
		{
			if (x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode == 59)
			{
				//v4 = (uint8)x_D41A0_BYTEARRAY_4;
				x_D41A0_BYTEARRAY_4_struct.byteindex_10 = x_D41A0_BYTEARRAY_4_struct.byteindex_10 != 1;
				x_D41A0_BYTEARRAY_4_struct.setting_38402 = 1;
			}
			a3a = sub_7C390();
			//v6 = (uint8)x_D41A0_BYTEARRAY_4;
			if (x_D41A0_BYTEARRAY_4_struct.byteindex_10 == 1)
			{
				//LOBYTE(v6) = v15;
				if (v15 == 2)
				{
					v13 = j___clock();
					if ((v13 - v12[0]) / 0x64u > 1)
					{
						//v7 = unk_E2516[46 + 9 * (signed __int16)(v14++ + 1)];
						v14++;
						//v7 = str_E2570[v14].word_2;
						if (!str_E2570[v14].word_2)
							v14 = 0;
						v12[0] = v13;
					}
					memset(v10arx, 0, 2 * sizeof(type_E24BCx));
					/*v8x = str_E2570[v14];
					qmemcpy(v10arx, v8, 0x10u+2u);*/
					v10arx[0] = str_E2570[v14];
					//qmemcpy(&v10ar[0x10], v8 + 16, 2u);
					sub_7E840_draw_textbox_with_line(v10arx, 21, 27);
				}
				else if (v15 == 3)
				{
					v13 = j___clock();
					if ((v13 - v12[0]) / 0x64u > 1)
						v15 = 2;
				}
			}
			if (!a3b)
			{
				sub_90B27_VGA_pal_fadein_fadeout(x_DWORD_17DE38str.x_DWORD_17DE38x, 0x20u, 0);
				a3b = 1;
				sub_8CD27_set_cursor(xy_DWORD_17DED4_spritestr[15]);
			}
			if (x_WORD_180660_VGA_type_resolution & 1)
				sub_90478_VGA_Blit320();
			else
				sub_75200_VGA_Blit640(480);
			sub_7A060_get_mouse_and_keyboard_events();
		}
		sub_7B5D0();
		sub_7AA70_load_and_decompres_dat_file(0, 0, 0, 0);
		sub_7C710();
		result = 1;
	}
	return result;
}

//----- (0007B250) --------------------------------------------------------
char DrawAndServe_7B250(/*int a1, int a2*//*, __int16 a3*/)//25c250
{
	//uint8_t* i; // esi
	int iy;
	char result; // al
	//uint8_t* j; // esi
	int jx;
	char v6; // ah
	//__int16 v7; // bx
	//uint8_t* v8; // esi
	//uint8_t* v9; // edi
	int v9y;
	//uint8_t* v10; // edi
	int v10x;
	//int v11; // eax
	//int v12; // edx
	int v13; // eax
	type_E24BCx v14arx[2]; // [esp+0h] [ebp-24h]
	//uint8_t v15[2]; // [esp+10h] [ebp-14h]//v14ar[0x10]

	//fix it
	//v12 = 0;
	//fix it

	if (CommandLineParams.ModeTestRegressionsGame()) {
		str_E1BAC[0].dword_0 = 0x258350;
		str_E1BAC[0].selected_8 = 1;
	}

	if (CommandLineParams.ModeTestNetwork()) {
		if (first_enter)
		{
			//str_E1BAC[2].dword_0 = 0x25EE80;
			str_E1BAC[2].selected_8 = 1;
		}
	}

	//for (i = off_E1BAC; *((int16_t*)i + 5); i += 44)
	for (iy = 0; str_E1BAC[iy].xmin_10; iy++)
	{
		if (str_E1BAC[iy].selected_8 && str_E1BAC[iy].dword_0)
		{
			if (DrawAndServe_pre_sub_7B250(str_E1BAC[iy].dword_0, &str_E1BAC[iy]))
			{
				str_E1BAC[iy].selected_8 = 0;
				ResetMouse_7B5A0();
			}
			if (str_E1BAC[iy].dword_4)
			{
				str_E1BAC[iy].selected_8 = 0;
				SetCenterScreenForFlyAssistant_6EDB0();
				sub_7A110_load_hscreen(x_WORD_180660_VGA_type_resolution, 4);
				ResetMouse_7B5A0();
				sub_8CD27_set_cursor(xy_DWORD_17DED4_spritestr[39]);
				if (str_E1BAC[iy].dword_4 == 2)
					str_E1BAC[iy].dword_4 = 0;
				return 1;
			}
			return 0;
		}
	}
	//for (j = off_E1BAC; *((int16_t*)j + 5); j += 44)//clear/set off_E1BAC
	for (jx = 0; str_E1BAC[jx].xmin_10; jx++)//clear/set off_E1BAC
	{
		str_E1BAC[jx].selected_8 = 0;
		v6 = x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode;
		str_E1BAC[jx].gold_color_24 = 0;
		if (v6 == 1 && str_E1BAC[jx].byte_22 == 11)
			str_E1BAC[jx].selected_8 = 1;
	}
	if (x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode == 1)
	{
		x_DWORD_17DE38str.x_BYTE_17DF10_get_key_scancode = 0;
	}
	else
	{
		//j = str_E1BAC;
		jx = 0;
		if (str_E1BAC[jx].xmin_10)
		{
			while (!InRegion_7B200(&str_E1BAC[jx], x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx, x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony) || !str_E1BAC[jx].canSelect_23)
			{
				//j += 44;
				jx++;
				if (!str_E1BAC[jx].xmin_10)
					goto LABEL_28;
			}
			if (x_DWORD_17DE38str.x_WORD_17DEEE_mouse_buttons & 1)
			{
				sub_8F100_sound_proc19(0, 14, 127, 64, 0x64u, 0, 3u);
				str_E1BAC[jx].selected_8 = 1;
				ResetMouse_7B5A0();
			}
			else
			{
				str_E1BAC[jx].gold_color_24 = 1;
				x_BYTE_17DBC6 = 1;
			}
		}
	}
LABEL_28:
	if (x_D41A0_BYTEARRAY_4_struct.byteindex_10 != 1 || !x_DWORD_17DE38str.x_BYTE_17DF13 || x_WORD_E29DC)
		return 0;
	//a3 = x_BYTE_17DBC6;
	if (x_BYTE_17DBC6 == 2)
	{
		x_DWORD_17DBB8[1] = j___clock();
		if ((x_DWORD_17DBB8[1] - x_DWORD_17DBB8[0]) / 0x64u > 1)
		{
			x_WORD_17DBC4++;
			//v7 = unk_E25DC[2 + 18 * (x_WORD_17DBC4++ + 1)];
			if (!str_E25DC[x_WORD_17DBC4].word_2)
				x_WORD_17DBC4 = 0;
			x_DWORD_17DBB8[0] = x_DWORD_17DBB8[1];
		}
		memset(v14arx, 0, 36);
		/*v8 = &unk_E25DC[18 * x_WORD_17DBC4];
		qmemcpy(v14arx, v8, 0x10u+2u);*/
		v14arx[0] = str_E25DC[x_WORD_17DBC4];
		//qmemcpy(&v14ar[0x10], v8 + 0x10, 2u);
		//v9 = str_E1BAC;
		v9y = 0;
		sub_7E840_draw_textbox_with_line(v14arx, 80, 89);
		//if (!*(int16_t*)&off_E1BAC[10])
		if (!str_E1BAC[0].xmin_10)
			return 0;
		do
		{
			//if (v9[22] == x_BYTE_E25ED_0x[18 * x_WORD_17DBC4])
			if (str_E1BAC[v9y].byte_22 == str_BYTE_E25ED_0x[x_WORD_17DBC4].byte_0)
			{
				str_E1BAC[v9y].gold_color_24 = 1;//turn on gold selection
				return 0;
			}
			//v9 += 44;
			v9y++;
		} while (str_E1BAC[v9y].xmin_10);
		result = 0;
	}
	else
	{
		if (x_BYTE_17DBC6 == 3)
		{
			x_DWORD_17DBB8[1] = j___clock();
			if ((x_DWORD_17DBB8[1] - x_DWORD_17DBB8[0]) / 0x64u > 1)
			{
				x_BYTE_17DBC6 = 2;
				return 0;
			}
			return 0;
		}
		//v10 = (uint8_t*)&unk_E25DC;//2b35dc
		v10x = 0;
		//if (*((int16_t*)&unk_E25DC + 1))
		if (str_E25DC[v10x].word_2)
		{
			while (str_E25DC[v10x].byte_17 != str_E1BAC[jx].byte_22)
			{
				//v10 += 9;
				v10x++;
				if (!str_E25DC[v10x].word_2)
					return 0;
			}
			memset(v14arx, 0, 36);
			//qmemcpy(v14arx, v10, 0x10u+2u);
			v14arx[0] = str_E25DC[v10x];
			//qmemcpy(&v14ar[0x10], v10 + 8, 2u);
			sub_7E840_draw_textbox_with_line(v14arx, 80, 89);
			v13 = j___clock();
			x_DWORD_17DBB8[1] = v13;
			x_DWORD_17DBB8[0] = v13;
			x_BYTE_17DBC6 = 3;
			result = 0;
		}
		else
		{
			result = 0;
		}
	}
	return result;
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// 98786: using guessed type int /*__fastcall*/ j___clock(x_DWORD, x_DWORD, x_DWORD);
// D41A4: using guessed type int x_DWORD_D41A4;
// E25DE: using guessed type __int16 x_WORD_E25DE[];
// E29DC: using guessed type __int16 x_WORD_E29DC;
// 17DBB8: using guessed type int x_DWORD_17DBB8;
// 17DBBC: using guessed type int x_DWORD_17DBBC;
// 17DBC4: using guessed type __int16 x_WORD_17DBC4;
// 17DBC6: using guessed type char x_BYTE_17DBC6;
// 17DED4: using guessed type int (int)x_DWORD_17DED4;
// 17DEE4: using guessed type int x_DWORD_17DEE4_mouse_position;
// 17DEEE: using guessed type __int16 x_WORD_17DEEE_mouse_buttons;
// 17DF10: using guessed type char x_BYTE_17DF10_get_key_scancode;
// 17DF13: using guessed type char x_BYTE_17DF13;
// 180660: using guessed type __int16 x_WORD_180660_VGA_type_resolution;

//----- (0007C390) --------------------------------------------------------
signed int sub_7C390()//25d390
{
	signed int result; // eax
	type_E1BAC_0x3c4* v1x;
	type_WORD_E1F84* v2x;
	int v3; // esi
	signed __int16 v4; // dx
	__int16 i; // ax
	uint8_t* v6x = 0; // [esp+0h] [ebp-8h]

	char dataPath[MAX_PATH];

	sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/SCREENS/HSCREEN0.DAT");

	v6x = x_DWORD_E9C38_smalltit;
	x_DWORD_E9C38_smalltit = x_D41A0_BYTEARRAY_4_struct.pointer_0xE2_heapbuffer_226;
	x_DWORD_17DE38str.x_WORD_17DF00 = x_DWORD_17DE38str.x_WORD_17DEFE;
	qmemcpy(x_BYTE_E1BA4, x_BYTE_E1B9C, sizeof(x_BYTE_E1BA4));
	if (SetMultiplayerColors_7CE50())
	{
		x_DWORD_E9C38_smalltit = v6x;
		result = 1;
	}
	else
	{
		qmemcpy(x_BYTE_E1B9C, &x_DWORD_17DE38str.array_BYTE_17DE68x[GetIndexNetwork2_74515()].arrayColors_1, sizeof(x_BYTE_E1B9C));
		v1x = &str_E1BAC_0x2ec[x_DWORD_17DE38str.x_WORD_17DEF4];
		if (x_WORD_180660_VGA_type_resolution & 1)
			CopyScreen((void*)x_DWORD_E9C38_smalltit, (void*)pdwScreenBuffer_351628, 320, 200);
		else
			CopyScreen((void*)x_DWORD_E9C38_smalltit, (void*)pdwScreenBuffer_351628, 640, 480);
		switch (x_DWORD_17DE38str.x_WORD_17DEF6)
		{
		case 1:
			sub_7AA70_load_and_decompres_dat_file(dataPath, &x_DWORD_E9C38_smalltit[307200], v1x->dword_0, v1x->dword_4);
			sub_7AA70_load_and_decompres_dat_file(dataPath, (uint8_t*)x_DWORD_17DE38str.x_DWORD_17DE3C, v1x->dword_8, v1x->dword_12);
			x_DWORD_17DE38str.palMulti_17DF02 = 0;
			x_DWORD_17DE38str.x_WORD_17DEF6 = 3;
			v2x = str_E1BAC_0x1b8;
			v3 = sub_7CB10();
			break;
		case 2:
			x_DWORD_17DE38str.palMulti_17DF02 += 16;
			PaletteMulti_7C9D0(x_DWORD_17DE38str.palMulti_17DF02);
			if (x_DWORD_17DE38str.palMulti_17DF02 < 0xFFu)
			{
				v2x = str_E1BAC_0x1b8;
				v3 = sub_7CB10();
				break;
			}
			CleanRectByColor_7C7C0(x_DWORD_E9C38_smalltit);
			x_DWORD_17DE38str.x_WORD_17DEF6 = 1;
			v2x = str_E1BAC_0x1b8;
			v3 = sub_7CB10();
			break;
		case 3:
			sub_7C720(x_DWORD_17DE38str.x_WORD_17DEF4, &x_DWORD_E9C38_smalltit[307200]);
			x_DWORD_17DE38str.palMulti_17DF02 += 16;
			PaletteCopy_7C800(x_DWORD_17DE38str.palMulti_17DF02);
			if (x_DWORD_17DE38str.palMulti_17DF02 <= 0xFEu)
			{
				v2x = str_E1BAC_0x1b8;
				v3 = sub_7CB10();
				break;
			}
			x_DWORD_17DE38str.x_WORD_17DEF6 = 4;
			v2x = str_E1BAC_0x1b8;
			v3 = sub_7CB10();
			break;
		case 5:
			CleanRectByColor_7C7C0(x_DWORD_E9C38_smalltit);
			x_DWORD_17DE38str.palMulti_17DF02 = 255;
			PaletteMulti_7C9D0(255);
			x_DWORD_17DE38str.x_WORD_17DEF6 = 1;
			x_DWORD_17DE38str.array_BYTE_17DE68x[x_DWORD_17DE38str.serverIndex_17DEFC].selectedLevel_10 = x_DWORD_17DE38str.array_BYTE_17DE68x[GetIndexNetwork2_74515()].selectedLevel_10;
			v2x = str_E1BAC_0x1b8;
			v3 = sub_7CB10();
			break;
		default:
			v2x = str_E1BAC_0x1b8;
			v3 = sub_7CB10();
			break;
		}
		while (v2x->xmin_10)
		{
			if (v2x->canSelect_23 && v2x->gold_color_24)
				sub_7C120_draw_bitmap_640(v2x->xmin_10, v2x->ymin_12, xy_DWORD_17DED4_spritestr[v2x->byte_21]);
			v2x++;
		}
		sub_7C140_draw_text_background(382, 18, 16, 16, 0);
		sprintf((char* const)x_DWORD_17DE38str.x_DWORD_17DE50, "%d", (unsigned __int16)x_DWORD_17DE38str.networkSession_17DEFA);
		sub_7FAE0_draw_text((char*)x_DWORD_17DE38str.x_DWORD_17DE50, 382, 398, 18, 0);
		DrawNetworkLevelName_7D380();
		SetMultiplayerColors_7D310();
		v4 = 0;
		for (i = 0; i < 8; i++)
		{
			if (x_BYTE_E1BA4[i] != x_BYTE_E1B9C[i])
			{
				v4 = 1;
				break;
			}
		}
		if (v4 || x_DWORD_17DE38str.x_WORD_17DF00 != x_DWORD_17DE38str.x_WORD_17DEFE)
			sub_41A90_VGA_Palette_install(x_DWORD_17DE38str.x_DWORD_17DE38x);
		x_DWORD_E9C38_smalltit = v6x;
		result = v3;
	}
	return result;
}

//----- (0007C710) --------------------------------------------------------
void sub_7C710()//25d710
{
	x_DWORD_17DE38str.palMulti_17DF02 = 0;
}

//----- (0007C720) --------------------------------------------------------
void sub_7C720(unsigned __int8 a1, uint8_t* a2)//25d720
{
	type_E1BAC_0x3c4* v2x;
	__int16 v3; // bx
	__int16 v4; // dx
	__int16 v5; // cx
	__int16 v6; // ax

	v2x = &str_E1BAC_0x2ec[a1];
	v3 = v2x->word_18;// *((x_WORD*)v2 + 9);
	v4 = v2x->word_16;//*((x_WORD*)v2 + 8);
	v5 = v2x->word_20;//*((x_WORD*)v2 + 10);
	v6 = v2x->word_22;//*((x_WORD*)v2 + 11);
	if (v3 + v6 > 480)
		v6 = 480 - v3;
	if (v5 + v4 > 640)
		v4 = v5 + v4 - 640;
	sub_85BF5(a2, x_DWORD_E9C38_smalltit, v4, v3, v5, v6);
}

//----- (0007CB10) --------------------------------------------------------
int sub_7CB10()//25db10
{
	int ix;
	int jx;
	int v3x;

	for (ix = 0; str_E1BAC_0x1b8[ix].xmin_10; ix++)
	{
		if (str_E1BAC_0x1b8[ix].selected_8 && str_E1BAC_0x1b8[ix].dword_0)
		{
			if (DrawAndServe_pre_sub_7B250(str_E1BAC_0x1b8[ix].dword_0, &str_E1BAC_0x1b8[ix]))
			{
				str_E1BAC_0x1b8[ix].selected_8 = 0;
				ResetMouse_7B5A0();
				return 1;
			}
			else
			{
				str_E1BAC_0x1b8[ix].selected_8 = 0;
				return 0;
			}
		}
	}
	for (jx = 0; str_E1BAC_0x1b8[jx].xmin_10; jx++)
	{
		str_E1BAC_0x1b8[jx].selected_8 = 0;
		str_E1BAC_0x1b8[jx].gold_color_24 = 0;
	}
	v3x = 0;
	if (!str_E1BAC_0x1b8[v3x].xmin_10)
		return 0;
	while (!InRegion_7B200(&str_E1BAC_0x1b8[v3x], x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx, x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony) || !str_E1BAC_0x1b8[v3x].canSelect_23)
	{
		v3x++;
		if (!str_E1BAC_0x1b8[v3x].xmin_10)
			return 0;
	}
	if (x_DWORD_17DE38str.x_WORD_17DEEE_mouse_buttons & 1)
	{
		str_E1BAC_0x1b8[v3x].selected_8 = 1;
		ResetMouse_7B5A0();
		sub_8F100_sound_proc19(0, 14, 127, 64, 0x64u, 0, 3u);
	}
	else
	{
		str_E1BAC_0x1b8[v3x].gold_color_24 = 1;
	}
	return 0;
}

//----- (0007B200) --------------------------------------------------------
bool InRegion_7B200(type_WORD_E1F84* a1x, int16_t testx, int16_t testy)//25c200
{
	int16_t minx; // dx
	int16_t miny; // bx
	bool result; // eax

	minx = a1x->xmin_10;
	result = 0;
	if (testx >= minx && testx < minx + a1x->sizex_14)
	{
		miny = a1x->ymin_12;
		if (testy >= miny && testy < miny + a1x->sizey_16)
			result = 1;
	}
	return result;
}

//----- (0007C9D0) --------------------------------------------------------
void PaletteMulti_7C9D0(signed __int16 a1)//25d9d0
{
	signed __int16 v1 = a1;
	if (v1 > 255)
		v1 = 255;
	for (int ic = 159; ic < 255; ic++)
	{
		x_DWORD_17DE38str.x_DWORD_17DE38x[ic].red -= (v1 * x_DWORD_17DE38str.x_DWORD_17DE38x[ic].red) >> 8;
		x_DWORD_17DE38str.x_DWORD_17DE38x[ic].green -= (v1 * x_DWORD_17DE38str.x_DWORD_17DE38x[ic].green) >> 8;
		x_DWORD_17DE38str.x_DWORD_17DE38x[ic].blue -= (v1 * x_DWORD_17DE38str.x_DWORD_17DE38x[ic].blue) >> 8;
	}

	x_DWORD_17DE38str.x_DWORD_17DE38x[149].red -= (v1 * x_DWORD_17DE38str.x_DWORD_17DE38x[149].red) >> 8;
	x_DWORD_17DE38str.x_DWORD_17DE38x[149].green -= (v1 * x_DWORD_17DE38str.x_DWORD_17DE38x[149].green) >> 8;
	x_DWORD_17DE38str.x_DWORD_17DE38x[149].blue -= (v1 * x_DWORD_17DE38str.x_DWORD_17DE38x[149].blue) >> 8;

	x_DWORD_17DE38str.x_DWORD_17DE38x[148].red -= (v1 * x_DWORD_17DE38str.x_DWORD_17DE38x[148].red) >> 8;
	x_DWORD_17DE38str.x_DWORD_17DE38x[148].green -= (v1 * x_DWORD_17DE38str.x_DWORD_17DE38x[148].green) >> 8;
	x_DWORD_17DE38str.x_DWORD_17DE38x[148].blue -= (v1 * x_DWORD_17DE38str.x_DWORD_17DE38x[148].blue) >> 8;
	sub_9A0FC_wait_to_screen_beam();
	sub_41A90_VGA_Palette_install(x_DWORD_17DE38str.x_DWORD_17DE38x);
}

//----- (0007C7C0) --------------------------------------------------------
void CleanRectByColor_7C7C0(uint8_t* a2)//25d7c0
{
	CleanRecByColor_85C42(a2, 208, 111, 200, 369, 0x9Fu);
}

// TODO: write UT
//----- (00085C42) --------------------------------------------------------
void CleanRecByColor_85C42(uint8_t* a1, int a2, int a3, int a4, int a5, unsigned __int8 a6)//266c42
{
	uint8_t* v6; // edi
	int v7; // edx
	int v8; // ecx
	//int v10; // [esp+4h] [ebp-4h]

	//fix it
	//v10 = 0;
	//fix it

	//v6 = (unsigned __int8*)(a2 + 640 * a3 + a1);
	v6 = &a1[a2 + 640 * a3];
	v7 = a5;
	do
	{
		v8 = a4;
		do
		{
			if (v6[0] >= a6)
				v6[0] = 0;
			v6++;
			v8--;
		} while (v8);
		v6 = &v6[640-a4];
		--v7;
	} while (v7);
}

//----- (0007D380) --------------------------------------------------------
void DrawNetworkLevelName_7D380()//25e380
{
	CleanRecByColor_85C42(pdwScreenBuffer_351628, 246, 14, 109, 14, 0x9Fu);
	DrawHelpText_6FC50(1);
	int v0 = x_DWORD_17DE38str.array_BYTE_17DE68x[x_DWORD_17DE38str.serverIndex_17DEFC].selectedLevel_10;
	sprintf(printbuffer, "%d. %s", v0 - 49, LevelsNames_D9204[v0]);
	sub_7FAE0_draw_text(printbuffer, 246, 355, 14, 0);
}

//----- (0007C800) --------------------------------------------------------
void PaletteCopy_7C800(signed __int16 a1)//25d800
{
	signed __int16 v1 = a1;

	if (v1 > 255)
		v1 = 255;
	for (int v3c = 159; v3c < 255; v3c++)
	{
		if (x_DWORD_17DE38str.x_DWORD_17DE38x[v3c].red < x_DWORD_17DE38str.x_DWORD_17DE3C->c[v3c].red)
			x_DWORD_17DE38str.x_DWORD_17DE38x[v3c].red = (x_DWORD_17DE38str.x_DWORD_17DE3C->c[v3c].red * v1) >> 8;
		else
			x_DWORD_17DE38str.x_DWORD_17DE38x[v3c].red = x_DWORD_17DE38str.x_DWORD_17DE3C->c[v3c].red;
		if (x_DWORD_17DE38str.x_DWORD_17DE38x[v3c].green < x_DWORD_17DE38str.x_DWORD_17DE3C->c[v3c].green)
			x_DWORD_17DE38str.x_DWORD_17DE38x[v3c].green = (x_DWORD_17DE38str.x_DWORD_17DE3C->c[v3c].green * v1) >> 8;
		else
			x_DWORD_17DE38str.x_DWORD_17DE38x[v3c].green = x_DWORD_17DE38str.x_DWORD_17DE3C->c[v3c].green;
		if (x_DWORD_17DE38str.x_DWORD_17DE38x[v3c].blue < x_DWORD_17DE38str.x_DWORD_17DE3C->c[v3c].blue)
			x_DWORD_17DE38str.x_DWORD_17DE38x[v3c].blue = (x_DWORD_17DE38str.x_DWORD_17DE3C->c[v3c].blue * v1) >> 8;
		else
			x_DWORD_17DE38str.x_DWORD_17DE38x[v3c].blue = x_DWORD_17DE38str.x_DWORD_17DE3C->c[v3c].blue;
	}
	if (x_DWORD_17DE38str.x_DWORD_17DE38x[148].red < x_DWORD_17DE38str.x_DWORD_17DE3C->c[148].red)
		x_DWORD_17DE38str.x_DWORD_17DE38x[148].red = (x_DWORD_17DE38str.x_DWORD_17DE3C->c[148].red * v1) >> 8;
	else
		x_DWORD_17DE38str.x_DWORD_17DE38x[148].red = x_DWORD_17DE38str.x_DWORD_17DE3C->c[148].red;
	if (x_DWORD_17DE38str.x_DWORD_17DE38x[148].green < x_DWORD_17DE38str.x_DWORD_17DE3C->c[148].green)
		x_DWORD_17DE38str.x_DWORD_17DE38x[148].green = (x_DWORD_17DE38str.x_DWORD_17DE3C->c[148].green * v1) >> 8;
	else
		x_DWORD_17DE38str.x_DWORD_17DE38x[148].green = x_DWORD_17DE38str.x_DWORD_17DE3C->c[148].green;
	if (x_DWORD_17DE38str.x_DWORD_17DE38x[148].blue < x_DWORD_17DE38str.x_DWORD_17DE3C->c[148].blue)
		x_DWORD_17DE38str.x_DWORD_17DE38x[148].blue = (x_DWORD_17DE38str.x_DWORD_17DE3C->c[148].blue * v1) >> 8;
	else
		x_DWORD_17DE38str.x_DWORD_17DE38x[148].blue = x_DWORD_17DE38str.x_DWORD_17DE3C->c[148].blue;

	if (x_DWORD_17DE38str.x_DWORD_17DE38x[149].red < x_DWORD_17DE38str.x_DWORD_17DE3C->c[149].red)
		x_DWORD_17DE38str.x_DWORD_17DE38x[149].red = (x_DWORD_17DE38str.x_DWORD_17DE3C->c[149].red * v1) >> 8;
	else
		x_DWORD_17DE38str.x_DWORD_17DE38x[149].red = x_DWORD_17DE38str.x_DWORD_17DE3C->c[149].red;
	if (x_DWORD_17DE38str.x_DWORD_17DE38x[149].green < x_DWORD_17DE38str.x_DWORD_17DE3C->c[149].green)
		x_DWORD_17DE38str.x_DWORD_17DE38x[149].green = (x_DWORD_17DE38str.x_DWORD_17DE3C->c[149].green * v1) >> 8;
	else
		x_DWORD_17DE38str.x_DWORD_17DE38x[149].green = x_DWORD_17DE38str.x_DWORD_17DE3C->c[149].green;
	if (x_DWORD_17DE38str.x_DWORD_17DE38x[149].blue < x_DWORD_17DE38str.x_DWORD_17DE3C->c[149].blue)
		x_DWORD_17DE38str.x_DWORD_17DE38x[149].blue = (x_DWORD_17DE38str.x_DWORD_17DE3C->c[149].blue * v1) >> 8;
	else
		x_DWORD_17DE38str.x_DWORD_17DE38x[149].blue = x_DWORD_17DE38str.x_DWORD_17DE3C->c[149].blue;
	sub_9A0FC_wait_to_screen_beam();
	sub_41A90_VGA_Palette_install(x_DWORD_17DE38str.x_DWORD_17DE38x);
}

//----- (00085BF5) --------------------------------------------------------
void sub_85BF5(uint8_t* a1, uint8_t* a2, int a3, int a4, int a5, int a6)//266bf5
{
	uint8_t* v6; // esi
	uint8_t* v7; // edi
	int v8; // edx
	int v9; // ecx
	//int v11; // [esp+4h] [ebp-4h]

	//fix it
	//v11 = 0;
	//fix it

	v6 = a1;
	v7 = &a2[a3 + 640 * a4];  // a2 == x_DWORD_E9C38_smalltit
	v8 = a6;
	do
	{
		v9 = a5;
		do
		{
			if (*v6 && *v6 != 0xff)
			{
				*v7++ = *v6++;
			}
			else
			{
				v6++;
				v7++;
			}
			v9--;
		} while (v9);
		v7 = &v7[640-a5];
		v8--;
	} while (v8);
	//return v11;
}
