#include "MenusAndIntros.h"

#include <algorithm>

#include "../utilities/StateMonitor.h"
#include "../shared/EventDispatcher.h"
#include "CommandLineParser.h"
#include "GameUI.h"
#include "Level.h"
#include "PlayerInput.h"
#include "DatTabIndexes.h"
#include "EventsFunctions.h"
#include "Graphics.h"
#include "Network.h"
#include "Player.h"
#include "Events.h"
#include "../utilities/RendererTests.h"
#include "ConvertMapInfo.h"
#include "LevelInit.h"
#include "MouseOnOff.h"
#include "PrintMessages.h"
#include "ReadAndDecompress.h"
#include "Type_DB080_CdTrack.h"


int test_regression_level = 1;

const char* LevelsNames_D9204[61] = {
	"1. Jahwl",
	"2. Kopahk",
	"3. Myrnan Gor",
	"4. Arachnium",
	"5. T'Klom",
	"6. Phyrydia",
	"7. Perilium",
	"8. Ul Buthnen",
	"9. Evirith Gor",
	"10. Cymmeria ",
	"11. Tropolos ",
	"12. Jaleen ",
	"13. Galiphur ",
	"14. Tunuk ",
	"15. Zyggogg ",
	"16. Darklava ",
	"17. C'lannesh ",
	"18. Gleph ",
	"19. Baraghan ",
	"20. Ammyridia ",
	"21. Cresidan ",
	"22. Hodor ",
	"23. Jathnar ",
	"24. Malak ",
	"25. Uluth ",
	"26. ",
	"27. ",
	"28. ",
	"29. ",
	"30. ",
	"Karakir ",
	"Ymbul",
	"Pav Durivium",
	"Beleem",
	"Ommosyth",
	"36. ",
	"37. ",
	"38. ",
	"39. ",
	"40. ",
	"41. ",
	"42. ",
	"43. ",
	"44. ",
	"45. ",
	"46. ",
	"47. ",
	"48. ",
	"49. ",
	"50. ",
	"Thrull",
	"Keevur",
	"Braak",
	"Trapox",
	"Hibren Zhor",
	"Jinople",
	"Dethrem",
	"Canquin",
	"Zephulum",
	"Verune",
	"0" };

char menuSoundVolume_E1324 = 50; // weak

char minTimeCounter_E2096 = 40; // weak

MenuItem nextMenu_E29D8 = MenuItem::InitLanguage; // weak//2b39d8

__int16 menuVideoMode_E29DA = 0; // weak

int16_t m_ExitMenuLoop_E29DC = 0; // weak

char selectedSession_E29DF = 0; // weak

char langLoaded_E29E0 = 1; // weak

int32_t times_17DBB8[4]; // weak//times_17DBB8[0]

char mainMenuState_17DBC6; // weak

uint32_t levelScores_17DBC8[125];

uint32_t secretLevelScores_17DDBC[26];

int16_t MOUSE_MIN = 0;
int16_t MOUSE_MAX_X = 638;
int16_t MOUSE_MAX_Y = 478;

#if defined(__linux__) || defined(__APPLE__)
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

// Unattended network testing (--auto_test): drives the menus so a test script can run a
// multiplayer session without anyone clicking through the front end.
extern bool Iam_server;

bool first_enter = true;
char skipScreen_D41AD = 0; // weak
int8_t LoadLevelNumber_D419C = -1; // weak
char IsPlayingCDTrack_17E09D; // weak

int16_t mainMenuTextIndex_17DBC4 = 0; // weak//times_17DBB8[3] 34ebc4
uint8_t* memoryBufferAlloc_E9C3C;

char netPlayerColors_E1B9C[8] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' }; // idb x_WORD_E1964x[0x238+
char oldNetPlayerColors_E1BA4[8] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' }; // idb x_WORD_E1964x[0x240+

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

Type_CutScene_E16E0 cutScene_E16E0[7] = {//cutscenes
{str_E1328,0x05,0x00,0x01},
{str_E13EC,0x09,0x00,0x02},
{str_E14AC,0x0D,0x00,0x03},
{str_E1570,0x11,0x00,0x04},
{str_E1634,0x18,0x00,0x05},
{str_E16B4,0x19,0x00,0x06},
{NULL,0x00,0x00,0x00}
};

type_x_BYTE_E25ED_db_str mapAnimSprites_E26C8[16] = {//set keys
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

type_x_BYTE_E25ED_2BB joystickDialogItems_E2590[3] = {//set joy
{ 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x01B5,0x0000,0x0001 },
{ 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x01B6,0x0000,0x0002 },
{ 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000 }
};

//0x2ec
type_E1BAC_0x3c4 colourMenuBackgrounds_E1E98[9] = {
{0x0005BC1E,0x000032D0,0x00076BBD,0x00000300,0x00F4,0x0085,0x0088,0x015C},
{0x0005EEEE,0x00003282,0x00076EBD,0x00000300,0x00D0,0x0081,0x00B4,0x0160},
{0x00062170,0x00003BE3,0x000771BD,0x00000300,0x00E2,0x0090,0x0096,0x0151},
{0x00065D53,0x000042B6,0x000774BD,0x00000300,0x00E0,0x006F,0x00A4,0x0172},
{0x0006A009,0x00003029,0x000777BD,0x00000300,0x00EC,0x0070,0x009C,0x0171},
{0x0006D032,0x00002E89,0x00077ABD,0x00000300,0x00E0,0x0087,0x00B4,0x0160},
{0x0006FEBB,0x00003BDC,0x00077DBD,0x00000300,0x00DD,0x0084,0x00A5,0x015D},
{0x00073A97,0x00003126,0x000780BD,0x00000300,0x00F0,0x008B,0x008C,0x0156},
{0x00000000,0x00000000,0x00000000,0x00000000,0x0000,0x0000,0x0000,0x0000} };

type_WORD_E20A4 dragonAnims_E20A4[12] = {
{0x00000000,0x00,0x00,0x00,0x00,0x01,0x00,0x0025,0x02,0x00,0x0025,0x03,0x00,{0x0000,0x0000,0x0180,0x0000,0x021F,0x03C0,0x008C,0x0091,0x008C,0x008C,0x0091,0x0032},0x01,0x00,0x03,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}},//dragon
{0x00000000,0x00,0x00,0x00,0x00,0x01,0x00,0x0025,0x02,0x00,0x0025,0x03,0x00,{0x0000,0x0000,0x018B,0x000A,0x0226,0x03CA,0x008C,0x0091,0x008C,0x008C,0x0091,0x0032},0x01,0x00,0x03,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}},//dragon
{0x00000000,0x00,0x00,0x00,0x00,0x01,0x00,0x0025,0x02,0x00,0x0025,0x03,0x00,{0x0000,0x0000,0x0190,0x0006,0x022B,0x03D4,0x008C,0x0091,0x008C,0x008C,0x0091,0x0032},0x01,0x00,0x03,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}},//dragon
{0x00000000,0x00,0x00,0x00,0x00,0x01,0x00,0x0027,0x02,0x00,0x0027,0x03,0x00,{0x0000,0x0000,0x0001,0x00BE,0x04FC,0x0134,0x00CE,0x00D3,0x00CE,0x00CE,0x00D3,0x0028},0x01,0x00,0x03,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}},//dragon2
{0x00000000,0x00,0x00,0x00,0x00,0x01,0x00,0x0025,0x02,0x00,0x0025,0x03,0x00,{0x0000,0x0000,0x01CB,0x0000,0x04FE,0x03C0,0x0092,0x0097,0x0092,0x0092,0x0097,0x0014},0x01,0x00,0x03,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}},//dragon3
{0x00000000,0x00,0x00,0x00,0x00,0x01,0x00,0x0009,0x02,0x00,0x0003,0x03,0x00,{0x0000,0x0000,0x0253,0x0000,0x023A,0x027C,0x0115,0x011C,0x0115,0x005D,0x0067,0x0000},0x02,0x00,0x03,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}},//sea beast
{0x00000000,0x00,0x00,0x00,0x00,0x01,0x00,0x0009,0x02,0x00,0x0003,0x03,0x00,{0x0000,0x0000,0x0262,0x0072,0x0262,0x0072,0x0129,0x0130,0x0129,0x0129,0x0130,0x0000},0x03,0x00,0x18,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}},//vulcan
{0x00000000,0x00,0x00,0x00,0x00,0x01,0x00,0x0009,0x02,0x00,0x0003,0x03,0x00,{0x0000,0x0000,0x0165,0x0000,0x011F,0x0290,0x006B,0x0072,0x006B,0x005D,0x0067,0x000F},0x01,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}},//explosion
{0x00000000,0x00,0x00,0x00,0x00,0x02,0x00,0x0009,0x02,0x00,0x0003,0x03,0x00,{0x0000,0x0000,0x03AB,0x0000,0x036F,0x0266,0x006B,0x0072,0x006B,0x005D,0x0067,0x000F},0x01,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}},//explosion
{0x00000000,0x00,0x00,0x00,0x00,0x03,0x00,0x0009,0x02,0x00,0x0003,0x03,0x00,{0x0000,0x0000,0x01AF,0x0000,0x0169,0x0200,0x006B,0x0072,0x006B,0x005D,0x0067,0x000B},0x01,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}},//explosion
{0x00000000,0x00,0x00,0x00,0x00,0x04,0x00,0x0009,0x02,0x00,0x0003,0x03,0x00,{0x0000,0x0000,0x01C7,0x0000,0x018B,0x0072,0x006B,0x0072,0x006B,0x005D,0x0067,0x0012},0x01,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}},//explosion
{0x00000000,0x00,0x00,0x00,0x00,0x00,0x00,0x0000,0x00,0x00,0x0000,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000},0x00,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}}};

typeTextBoxtextBoxStr_E24BCx textBoxStr_E2516[5] = {
{0x01D2,0x012F,0x0169,0x01DC,0x016A,0x0000,0x023C,0x01B8,0x02,0x0B},
{0x0195,0x015E,0x0041,0x0211,0x0052,0x0000,0x0254,0x001C,0x08,0x08},
{0x0196,0x007E,0x0041,0x007B,0x0052,0x0000,0x0037,0x001C,0x08,0x09},
{0x01D3,0x007F,0x0168,0x007B,0x0176,0x0000,0x0034,0x01B0,0x08,0x10},
{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00,0x00} };

Type_SoundEvent_E17CC introSoundEvents_E17CC[50] = {//anim events
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

Type_SoundEvent_E17CC intro2SoundEvents_E192C[10] = {
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

type_menuButtons_E1F84 mapMenuButtons_E23E0[6] = {
{0x0025F620,0x00000000,0x0000,0x0245,0x01AB,0x0028,0x0046,0x0000,0xF7,0xF6,0x0B,0x01,0x00,0x01,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003}},
{0x0025F640,0x00000000,0x0000,0x0000,0x01AB,0x0028,0x0046,0x0000,0xF9,0xF8,0x10,0x01,0x00,0x01,{0x0025,0x015C,0x0000,0x0000,0x003C,0x0000,0x01D3,0x0000,0x0003}},
{0x0025F800,0x00000000,0x0000,0x0000,0x0000,0x0028,0x0046,0x0196,0xFB,0xFA,0x09,0x01,0x00,0x01,{0x001D,0x003C,0x0000,0x0000,0x00C8,0x0000,0x01A6,0x0000,0x0003}},
{0x0025F820,0x00000000,0x0000,0x0245,0x0000,0x0028,0x0046,0x0195,0xFD,0xFC,0x08,0x01,0x00,0x01,{0x01FE,0x003C,0x0000,0x0000,0x00C8,0x0000,0x01A5,0x0000,0x0003}},
{0x00000000,0x00000000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00,0x00,0x00,0x00,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}}};

type_menuButtons_E1F84 languageButtons_E1F84[3] = {
{0x00000000,0x00000000,0x0000,0x0123,0x00CD,0x003C,0x003C,0x0000,0x24,0x23,0x01,0x01,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003}},
{0x00000000,0x00000000,0x0000,0x0120,0x0063,0x003C,0x003C,0x0000,0x4C,0x4D,0x02,0x01,0x01,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003}},
{0x00000000,0x00000000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00,0x00,0x00,0x00,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}}
};

type_menuButtons_E1F84 mainMenuButtons_E1BAC[10] = {
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

uint16_t languageTextTable_E24BE[3][9] = { // 2b34be //only for clock?
	{0x0028,0x0031,0x00C4,0x0026,0x0000,0x011B,0x006A,0x0002,0x01C6},
	{0x012A,0x012C,0x018D,0x012A,0x0000,0x0141,0x00EC,0x0008,0x0000},
	{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x01D1}
};

typeTextBoxtextBoxStr_E24BCx textBoxStr_E24BCx[26]= {
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

type_x_BYTE_E25ED_2BB keysDialogItems_E28A8[11] = {
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

type_menuButtons_E1F84 regionButtons_E2008[3] = {
{0x00000000,0x00000000,0x0000,0x0111,0x000A,0x003C,0x003C,0x0000,0x00,0x6D,0x01,0x01,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003}},
{0x00000000,0x00000000,0x0000,0x011B,0x017D,0x003C,0x003C,0x0000,0x00,0x6C,0x02,0x01,0x01,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003}},
{0x00000000,0x00000000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00,0x00,0x00,0x00,0x00,0x00,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}}};

typeTextBoxtextBoxStr_E24BCx textBoxStr_E24F2[2] = {
{0x01D1,0x0103,0x0320,0x015D,0x0320,0x0000,0x0153,0x0320,0x02,0x0B},
{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00,0x00} };

typeTextBoxtextBoxStr_E24BCx textBoxStr_E2570[6] = {
{0x01C9,0x0004,0x00B8,0x0042,0x00B6,0x0000,0x007E,0x0082,0x00,0x00},
{0x01CB,0x0005,0x0168,0x004D,0x0168,0x0000,0x0095,0x0130,0x00,0x00},
{0x01CA,0x01BC,0x016A,0x0227,0x016A,0x0000,0x01C6,0x0122,0x00,0x00},
{0x01C8,0x01BC,0x00A8,0x0228,0x00A8,0x0000,0x01E8,0x0074,0x00,0x00},
{0x01CC,0x01A3,0x000D,0x01D7,0x0019,0x0000,0x0148,0x0018,0x08,0x00},
{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00,0x00} };

typeTextBoxtextBoxStr_E24BCx textBoxStr_E25DC[10] = {
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

type_BYTE_E25ED_0x goldMenuItems_E25ED[9] = {//gold selection menu
{0x06,{0xA0,0x01,0x13,0x00,0x74,0x00,0xAC,0x00,0x7C,0x00,0x01,0x00,0x19,0x01,0x7C,0x00,0x02}},
{0x0A,{0x96,0x01,0x11,0x00,0xF6,0x00,0x5F,0x00,0xF6,0x00,0x3E,0x00,0xC0,0x00,0xAA,0x00,0x00}},
{0x09,{0x9B,0x01,0x11,0x00,0x68,0x01,0x5F,0x00,0x68,0x01,0x3E,0x00,0xC1,0x00,0x1C,0x01,0x00}},
{0x0D,{0xA3,0x01,0xDC,0x00,0x55,0x01,0x3E,0x01,0x55,0x01,0x41,0x00,0x3C,0x01,0xD0,0x00,0x00}},
{0x0C,{0xCF,0x01,0x9A,0x01,0x48,0x01,0x03,0x02,0x61,0x01,0x3F,0x00,0xB5,0x01,0x22,0x01,0x08}},
{0x0F,{0xA2,0x01,0x84,0x01,0xED,0x00,0xE5,0x01,0xEC,0x00,0x3F,0x00,0xB0,0x01,0xC4,0x00,0x08}},
{0x08,{0xA1,0x01,0x96,0x01,0x9D,0x00,0xFC,0x01,0xA6,0x00,0x3D,0x00,0xA8,0x01,0x62,0x00,0x08}},
{0x07,{0x97,0x01,0x9B,0x01,0x1F,0x00,0xE8,0x01,0x29,0x00,0x40,0x00,0x5A,0x01,0x28,0x00,0x08}},
{0x0B,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} };

type_menuButtons_E1F84 multiplayerButtons_E1D64[7] = {//0x1b8
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

type_x_DWORD_17DB70str mapMenuState_17DB70;

Type_DWORD_17DE28str levelDescription_17DE28;

Type_unk_17DBA8str mapHelpText_17DBA8;

bool mapNotMoving_E29D6 = false; // used to set/reset map move acceleration
char initialMenuMode_E29DE = 1; // weak
char directStart_E29E1 = 1; // weak

int16_t x_WORD_17DE26; // weak

std::string g_matchResultPending;

intptr_t unknown_libname_2_findfirst(char* path, uint16_t  /*a2*/, _finddata_t* c_file) {//findfirst
	intptr_t hFile;
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
CopyDriverInfoIfType9_9A1B6(result, (void*)a3, (void*)a3);
return result;*/
};// weak

intptr_t unknown_libname_3_findnext(_finddata_t* c_file, intptr_t hFile) {//258193
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
	CopyDriverInfoIfType9_9A1B6(result, v6, a1);
	return result;*/
	return 0;
};// weak

int unknown_libname_4_find_close(struct _finddata_t*  /*c_file*/, intptr_t hFile) {//27b1b3
	my_findclose(hFile);
	return 0;
};// weak

void /*__fastcall*/ _wcpp_1_unwind_leave__131() {//257d00
	stub_fix_it();
}

//----- (0002EB40) --------------------------------------------------------
void StopSubtitles_2EB40()//20fb40
{
	//int result; // eax

	//BYTE1(result) = 0;
	SubtitlesOn_D41CE = 0;
	DisplaySubtitles_D41C1 = 0;
	subtitleText_D41D0 = 0;
	//return result;
}
// D41C1: using guessed type char DisplaySubtitles_D41C1;
// D41CE: using guessed type char SubtitlesOn_D41CE;
// D41D0: using guessed type int subtitleText_D41D0;


//----- (00076930) --------------------------------------------------------
void MenusAndIntros_76930(bool skipMenus)//257930
{
	//1 -351660
	selectedSession_E29DF = skipScreen_D41AD;
	// Skip the intro animations so an unattended test reaches the menu straight away.
	// Set after E29DF has been read, so the network session number stays untouched.
	if (CommandLineParams.AutoTest() && !skipMenus)
		skipScreen_D41AD = 1;
	if (skipMenus)
	{
		skipScreen_D41AD = 1;
		m_ExitMenuLoop_E29DC = 1;
	}
	else
	{
		m_ExitMenuLoop_E29DC = 0;
	}

	if (skipScreen_D41AD == 1 || (nextMenu_E29D8!= MenuItem::InitLanguage))
	{
		PlayInGameFmv_82670();
		LoadAndSetGraphicsAndPalette_7AC00();
	}
	if (skipScreen_D41AD == 1)
	{
		InitLanguage_76A40();
		nextMenu_E29D8 = MenuItem::MainMenu;
	}
	memset(&menuState_17DE38, 0, sizeof(type_x_DWORD_17DE38str));
	menuState_17DE38.fileHandle_17DEE0 = NULL;
	ResetMainMenuButtons_7BEC0();//25CEC0
	SetCenterScreenForFlyAssistant_6EDB0();//24FDB0
	WriteConfigDat_81DB0();//262DB0
	do
	{
		g_state_monitor.Update();
		//2b39d8
		switch (nextMenu_E29D8)
		{
		case MenuItem::InitLanguage:
			InitLanguage_76A40();//257A40
			break;
		case MenuItem::SetToIntro:
			SetToIntro_76CF0();//257cf0 nastavi x_WORD_E29D8 na 3
			break;
		case MenuItem::LeaveX:
			_wcpp_1_unwind_leave__131();//257d00 asi konec
			break;
		case MenuItem::Intros:
			Intros_76D10(0);//257d10
			nextMenu_E29D8 = MenuItem::MainMenu;
			break;
		case MenuItem::MainMenu:
			MainMenu_76FA0();//257fa0
			break;
		case MenuItem::Exit:
			m_ExitMenuLoop_E29DC = 1;
			break;
		case MenuItem::LangSettings:
			LanguageSettingDialog_779E0(0);//2589e0
			break;
		default:
			break;
		}
	} while (!m_ExitMenuLoop_E29DC);
	SetMenuVideoMode_7ADE0(initialMenuMode_E29DE);
	if (directStart_E29E1)
		directStart_E29E1 = 0;
	WriteConfigDat_81DB0();
}

//----- (00076A40) --------------------------------------------------------
void InitLanguage_76A40()//257A40
{
	FILE* configdatfile;
	FILE* langfile;
	uint32_t filelenght;
	TypeConfigDat configDat;

	memset(printbuffer, 0, 80);
	memset(&menuState_17DE38, 0, sizeof(type_x_DWORD_17DE38str));
	menuState_17DE38.fileHandle_17DEE0 = NULL;
	memset(&configDat, 0, sizeof(TypeConfigDat));
	sprintf(printbuffer, "%s/%s", gameDataPath.c_str(), "CONFIG.DAT");
	configdatfile = DataFileIO::CreateOrOpenFile(printbuffer, 512);
	if (configdatfile == NULL && PackedGameData())//regression tests without the game: no dialog, CONFIG.DAT defaults, empty texts
	{
		gameState_D41A4.configDatSign_0 = 0xfffffff7;
		gameState_D41A4.soundVolume_6 = 0x7f;
		gameState_D41A4.musicVolume_8 = 0x7f;
		gameState_D41A4.showHelp_10 = 1;
		if (!langBuffer_D41BC)
		{
			langBuffer_D41BC = (char*)Malloc_83CD0(471);
			memset(langBuffer_D41BC, 0, 471);
		}
		CopySentence_5B870(langBuffer_D41BC, langStrings_E9C4C, 471);
	}
	else if (configdatfile == NULL)//config is not found
	{
		LoadAndSetGraphicsAndPalette_7AC00();//25BC00 //change screen radio
		LanguageSettingDialog_779E0(0);//2589E0 //set language
		SetMenuVideoMode_7ADE0(1);
	}
	else
	{
		DataFileIO::Read(configdatfile, (uint8_t*)&configDat.configDatSign_0, 4);//first dword
		if (configDat.configDatSign_0 == 0xfffffff7)
		{
			DataFileIO::Read(configdatfile, (uint8_t*)&configDat.langIndex_4, 28);

			gameState_D41A4.configDatSign_0 = configDat.configDatSign_0;
			gameState_D41A4.langIndex_4 = configDat.langIndex_4;
			gameState_D41A4.soundVolume_6 = configDat.soundVolume_6;
			gameState_D41A4.musicVolume_8 = configDat.musicVolume_8;
			gameState_D41A4.showHelp_10 = configDat.showHelp_10;
			gameState_D41A4.brightness_11 = configDat.brightness_11;
			gameState_D41A4.brightness_12 = configDat.brightness_12;
			gameState_D41A4.brightness_13 = configDat.brightness_13;
			gameState_D41A4.wordindex_14 = configDat.wordindex_14;
			gameState_D41A4.dwordindex_16 = configDat.dwordindex_16;
			gameState_D41A4.stubb[0] = configDat.stubb[0];
			gameState_D41A4.stubb[1] = configDat.stubb[1];

			for (int i = 0; i < 10; i++)
				keys_EB39E[i] = configDat.keys[i];

			DataFileIO::Close(configdatfile);
			SetSoundVolume_8E470(gameState_D41A4.soundVolume_6);
			SetMusicVolume_8E410(gameState_D41A4.musicVolume_8);

			sprintf(printbuffer, "%s/%s/L%d.TXT", cdDataPath.c_str(), "LANGUAGE", gameState_D41A4.langIndex_4);
			for (int i = 0; i < 2; i++)
			{
				gameState_D41A4.SelectedLangIndex = gameState_D41A4.langIndex_4 & 0xff;
				if (langLoaded_E29E0 || langBuffer_D41BC)
					FreeMem_83E80((uint8_t*)langBuffer_D41BC);
				langfile = DataFileIO::CreateOrOpenFile(printbuffer, 512);

				if (langfile != NULL)
				{
					filelenght = DataFileIO::FileLengthBytes(langfile) - 4785;
					langBuffer_D41BC = (char*)Malloc_83CD0(filelenght);
					if (langBuffer_D41BC)
					{
						DataFileIO::Seek(langfile, 4785, 0);
						DataFileIO::Read(langfile, (uint8_t*)langBuffer_D41BC, filelenght);
					}
					DataFileIO::Close(langfile);
					CopySentence_5B870(langBuffer_D41BC, langStrings_E9C4C, 471);//Exit Game
					break;
				}
				sprintf(printbuffer, "%s/%s/L%d.TXT", cdDataPath.c_str(), "LANGUAGE", gameState_D41A4.SelectedLangIndex);
			}
		}
		else
		{
			DataFileIO::Close(configdatfile);
			LoadAndSetGraphicsAndPalette_7AC00();
			LanguageSettingDialog_779E0(nullptr);
			SetMenuVideoMode_7ADE0(1);
		}
	}
	if (gameState_D41A4.SelectedLangIndex != 2 || !soundAble_E3798)
		DisplaySubtitles_D41C0 = 1;
	nextMenu_E29D8 = MenuItem::SetToIntro;
}

//----- (00076CF0) --------------------------------------------------------
void SetToIntro_76CF0()
{
	nextMenu_E29D8 = MenuItem::Intros;
}

//----- (00076D10) --------------------------------------------------------
void Intros_76D10(char introType)//257d10
{
	char dataPath[MAX_PATH];
	menuState_17DE38.fontData_17DE54 = &gameState_D41A4.pointer_0xE2_heapbuffer_226[301787];
	menuState_17DE38.fontTab_17DEC0 = (bitmap_pos_struct2_t*)&gameState_D41A4.pointer_0xE2_heapbuffer_226[308527];
	menuState_17DE38.fontTabEnd_17DEC4 = (bitmap_pos_struct2_t*)&gameState_D41A4.pointer_0xE2_heapbuffer_226[310159];
	sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/SCREENS/HSCREEN0.DAT");
	LoadAndDecompressDatFile_7AA70(dataPath, &gameState_D41A4.pointer_0xE2_heapbuffer_226[301787], 0x164FCD, 0x35C);
	LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.fontTab_17DEC0, 0x165329, 0x224);
	LoadAndDecompressDatFile_7AA70(0, 0, 0, 0);
	if (videoMode_180660 & 1)
		CreateIndexDatTabDoubled_98709(menuState_17DE38.fontTab_17DEC0, menuState_17DE38.fontTabEnd_17DEC4, menuState_17DE38.fontData_17DE54, fontSprites_17DEC0);
	else
		CreateIndexDatTab_9874D(menuState_17DE38.fontTab_17DEC0, menuState_17DE38.fontTabEnd_17DEC4, menuState_17DE38.fontData_17DE54, fontSprites_17DEC0);

	//LoadExternalFonts("",fontSprites_17DEC0);
	if (extendedFonts)
		LoadFixedFonts(0,(char*)"intro");

	StopSubtitles_2EB40();
	if (soundAble_E3798 && gameState_D41A4.SelectedLangIndex == 2)
	{
		DisplaySubtitles_D41C1 = 0;
		DisplaySubtitles_D41C0 = 0;
	}
	else
	{
		DisplaySubtitles_D41C0 = 1;
		DisplaySubtitles_D41C1 = 1;
	}
	SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]);  //Set cursor to Null (Don't Draw)
	char introPath[MAX_PATH];
	switch (introType)
	{
	case 0:
#ifndef debug_hide_graphics
		ShowWelcomeScreen_83850();//frog logo and wait
#endif
		sprintf(introPath, "%s/%s", cdDataPath.c_str(), "INTRO/INTRO.DAT");
		PlayInfoFmv(1, 1, introSoundEvents_E17CC, introPath);//257160 intro .. 2b27cc
		StopSubtitles_2EB40();
		DisplaySubtitles_D41C1 = 0;
		DisplaySubtitles_D41C0 = 0;
		while (CheckKeyboard_9A10A())
		{
			LastPressedKey_1806E4 = 0;
			GetMouseAndKeyboardEvents_7A060();
		}
		j___delay(50);
		sprintf(introPath, "%s/%s", cdDataPath.c_str(), "INTRO/INTRO2.DAT");
		PlayInfoFmv(1, 1, intro2SoundEvents_E192C, introPath);//E192C
		break;
	case 1:
		sprintf(introPath, "%s/%s", cdDataPath.c_str(), "INTRO/INTRO.DAT");
		PlayInfoFmv(1, 1, introSoundEvents_E17CC, introPath);
		break;
	case 2:
		LastPressedKey_1806E4 = 0;
		DisplaySubtitles_D41C1 = 0;
		DisplaySubtitles_D41C0 = 0;
		sprintf(introPath, "%s/%s", cdDataPath.c_str(), "INTRO/INTRO2.DAT");
		PlayInfoFmv(1, 1, intro2SoundEvents_E192C, introPath);//E192C
		break;
	}
	VgaPalFadeInFadeOut_90B27(0, 0x10u, 0);
	EndSample_8D8F0();
	StopMusic_8E020();
	FadeClearBlit_7B5D0();
	nextMenu_E29D8 = MenuItem::MainMenu;
	DisplaySubtitles_D41C0 = 0;
	DisplaySubtitles_D41C1 = 0;
	if (CheckKeyboard_9A10A())
		GetMouseAndKeyboardEvents_7A060();
	menuState_17DE38.lastKeyChar_17DF11 = 0;
	menuState_17DE38.keyScancode_17DF10 = 0;
	if (!introType)
		LoadAndSetGraphicsAndPalette_7AC00();
}

//----- (00076FA0) --------------------------------------------------------
void MainMenu_76FA0()//257fa0
{
	//fixed
	help_VGA_type_resolution = 0;
	//fixed
	bool onlyBlit = false;
	uint16_t introIndex = 1;

	StopCdPlayback_86860(cdDrive_1803EC);//267860
	LoadSounds_84300(0);//265300
	memset(&times_17DBB8, 0, 16);
	mainMenuState_17DBC6 = 2;
	menuState_17DE38.savedGameIndex_17DF04 = -1;
	menuState_17DE38.savedSmalltit_17DE44 = smalltitBuffer_E9C38;
	SetCenterScreenForFlyAssistant_6EDB0();
	StopMusic_8E020();//26f020
	StartMusic_8E160(4, 0x7Fu);//26f160
	x_WORD_17DE26 = 0;

	if (!g_matchResultPending.empty())
	{
		if (CommandLineParams.DoNetworkDebug())
			debug_net_printf("MENU: showing result %s\n", g_matchResultPending.c_str());
		// The scores screen waits for a keypress, and the automated tests press nothing:
		// a run would stop here after the first match, which is reported as "host played
		// 1 of 2 matches" with the transport perfectly healthy.
		if (!CommandLineParams.AutoTest())
			ShowScores();
	}

	VGA_cleanKeyBuffer();
	if (directStart_E29E1 || gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE || (NewGameDialog_77350(0), !m_ExitMenuLoop_E29DC))
	{
		gameState_D41A4.setting_byte1_22 &= 0xEFu;
		LoadHscreen_7A110(videoMode_180660, 4);
		ResetMouse_7B5A0();

		//fix
		//write_pngs();//only for export
		//fix

		SetCursor_8CD27(menuSprites_17DED4[39]);
		menuState_17DE38.showHelp_17DF13 = gameState_D41A4.showHelp_10;
		times_17DBB8[0] = j___clock();
		int lastTime = j___clock();
		int16_t tempMousePosX = menuState_17DE38.mouseX_17DEE4;
		int16_t tempMousePosY = menuState_17DE38.mouseY_17DEE6;
		int scanCode = menuState_17DE38.keyScancode_17DF10;

		while (!m_ExitMenuLoop_E29DC)
		{
			g_state_monitor.Update();

			SetFrameStart(std::chrono::system_clock::now());
			//v13 = j___clock();
			if ((tempMousePosX == menuState_17DE38.mouseX_17DEE4) && (tempMousePosY == menuState_17DE38.mouseY_17DEE6) && (menuState_17DE38.keyScancode_17DF10 == scanCode))
			{
				if ((j___clock() - lastTime) / 100 > 60)//after 1 min run intro
				{
					uint8_t* tempSmalltit = smalltitBuffer_E9C38;
					smalltitBuffer_E9C38 = menuState_17DE38.savedSmalltit_17DE44;
					PlayIntros_83250(introIndex);
					smalltitBuffer_E9C38 = tempSmalltit;
					introIndex = (introIndex == 1) + 1;//alternate 1 and 2
					tempMousePosY = menuState_17DE38.mouseY_17DEE6;
					tempMousePosX = menuState_17DE38.mouseX_17DEE4;
					menuState_17DE38.keyScancode_17DF10 = 0;
					scanCode = 0;
					lastTime = j___clock();
					StopMusic_8E020();
					onlyBlit = 0;
					StartMusic_8E160(4, 0x7Fu);
				}
			}
			else
			{
				tempMousePosY = menuState_17DE38.mouseY_17DEE6;
				tempMousePosX = menuState_17DE38.mouseX_17DEE4;
				scanCode = menuState_17DE38.keyScancode_17DF10;
				lastTime = j___clock();
			}
			if (menuState_17DE38.keyScancode_17DF10 == 59)
			{
				gameState_D41A4.showHelp_10 = gameState_D41A4.showHelp_10 != 1;
				menuState_17DE38.showHelp_17DF13 = gameState_D41A4.showHelp_10;
				gameState_D41A4.setting_38402 = 1;
			}

			//Draw Backround
			if (videoMode_180660 & 1)
				CopyScreen(smalltitBuffer_E9C38, pdwScreenBuffer_351628, 320, 200);
			else
				CopyScreen(smalltitBuffer_E9C38, pdwScreenBuffer_351628, 640, 480);

			DrawBitmap640_7C120(185, 232, menuSprites_17DED4[66]);//adress 25827a
			DrawMenuAnimations_7AB00();//25bb00

			DrawText_7FB90((char*)VersionNumber, 10, 465, GetPaletteIndex_5BE80(menuState_17DE38.palette_17DE38x, 255, 0, 0));

			if (DrawAndServe_7B250())//25c250
			{
				tempMousePosY = menuState_17DE38.mouseY_17DEE6;
				tempMousePosX = menuState_17DE38.mouseX_17DEE4;
				onlyBlit = false;
				scanCode = menuState_17DE38.keyScancode_17DF10;
				lastTime = j___clock();
			}
			if (onlyBlit)
			{
				if (videoMode_180660 & 1)
					VgaBlit320_90478(menuFps);
				else
					VgaBlit640_75200(480, menuFps);
			}
			else
			{
				onlyBlit = true;
				//34ee38 20 0
				VgaPalFadeInFadeOut_90B27(menuState_17DE38.palette_17DE38x, 0x20u, 0);//tady
			}
			GetMouseAndKeyboardEvents_7A060();
		}

		ClearPauseMenuState_41BC0();
		world_D41A0.m_GameSettings.m_Display.m_uiScreenSize = 0;
		ClearPressedKeys_753D0();
		smalltitBuffer_E9C38 = menuState_17DE38.savedSmalltit_17DE44;
	}
	else
	{
		ClearPauseMenuState_41BC0();
		world_D41A0.m_GameSettings.m_Display.m_uiScreenSize = 0;
		ClearPressedKeys_753D0();
	}
}

//----- (00077350) --------------------------------------------------------
bool NewGameDialog_77350(type_menuButtons_E1F84* a1x)//258350
{
	bool result=false;

	int endAction = 0;
	mapNotMoving_E29D6 = false;
	if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] & 2 && gameState_D41A4.levelnumber_43w == 24) {
		// show credits after finishing the last level
		ShowEndCredits_833C0();
	}
	memset((void*)&mapHelpText_17DBA8, 0, sizeof(mapHelpText_17DBA8));
	mapHelpText_17DBA8.helpMode_17DBB6 = 2;
	if (LoadLevelNumber_D419C <= -1)
	{
		IsPlayingCDTrack_17E09D = 0;
		menuState_17DE38.mapAnimation_17E078x.lastSpriteIndex_11 = 16;
		menuState_17DE38.y_17E06E = 480;
		menuState_17DE38.mapAnimation_17E078x.spriteIndex_8 = 13;
		menuState_17DE38.mapAnimState_17E09C = 0;
		menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 = 13;
		menuState_17DE38.x_17E06C = 0;
		memset((void*)&mapMenuState_17DB70, 0, sizeof(type_x_DWORD_17DB70str));
		if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] & 2)
			mapMenuState_17DB70.mapState_17DB8F = 4;
		else
			mapMenuState_17DB70.mapState_17DB8F = 1;
		LoadHscreen_7A110(videoMode_180660, 6);//here init sprites
		SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]); //Set cursor to Null (Don't Draw)

		ResetMouse_7B5A0();
		VgaPalFadeInFadeOut_90B27(0, 0x10u, 0);
		if (videoMode_180660 & 1)
		{
			ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, GetPaletteIndex_5BE80(menuState_17DE38.palette_17DE38x, 0, 0, 0));
			ClearScreenAlphaBuffer(320, 200, 255);
		}
		else
		{
			ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 640, 480, GetPaletteIndex_5BE80(menuState_17DE38.palette_17DE38x, 0, 0, 0));
			ClearScreenAlphaBuffer(640, 480, 255);
		}
		if (videoMode_180660 & 1)
			VgaBlit320_90478();
		else
			VgaBlit640_75200(480);
		VgaPaletteInstall_41A90(menuState_17DE38.palette_17DE38x);
		PortalsUpdate_7DD70();
		menuState_17DE38.x_WORD_17DEEC = 0;
		SetCenterScreenForFlyAssistant_6EDB0();
		SetCursor_8CD27(menuSprites_17DED4[239]);
		mapMenuState_17DB70.selectedLevel_17DB8A = gameState_D41A4.levelnumber_43w;
		while (!endAction)
		{
			g_state_monitor.Update();

			SetFrameStart(std::chrono::system_clock::now());
			if (menuState_17DE38.keyScancode_17DF10 == 59)
			{
				menuState_17DE38.showHelp_17DF13 = gameState_D41A4.showHelp_10 != 1;
				gameState_D41A4.showHelp_10 = gameState_D41A4.showHelp_10 != 1;
				gameState_D41A4.setting_38402 = 1;
			}
			endAction = NewGameDraw_7EAE0(
				&mapMenuState_17DB70.posX_17DB76,
				&mapMenuState_17DB70.posY_17DB78,
				&mapMenuState_17DB70.portPosX_17DB7E,
				&mapMenuState_17DB70.portPosY_17DB80,
				&mapMenuState_17DB70.mapState_17DB8F,
				&mapMenuState_17DB70.anim_17DB90);
			if (CommandLineParams.ModeRegressionsTestType()!=-1) {
				endAction = 1;
			}
			if (CommandLineParams.ModeRegressionsTestType()==0) {
				mapMenuState_17DB70.levelChosen_17DB8E = 1;
				gameState_D41A4.levelnumber_43w = test_regression_level;
				if (mapScreenPortals_E17CC[test_regression_level].activated_18 == 1)
					gameState_D41A4.setting_38545 |= 4u;
				Type_SecretMapScreenPortals_E2970* v46x = GetSecretAndActivedPortal_824B0(gameState_D41A4.levelnumber_43w);
				if (v46x && v46x->activated_12 == 2)
					gameState_D41A4.setting_38545 |= 0x10u;
				if (gameState_D41A4.levelnumber_43w == 24)
					gameState_D41A4.setting_38545 |= 0x20u;
				endAction = 1;
			}
			if (videoMode_180660 & 1)
				VgaBlit320_90478(menuFps);
			else
				VgaBlit640_75200(480, menuFps);
			GetMouseAndKeyboardEvents_7A060();
		}
		StopCdPlayback_86860(cdDrive_1803EC);
		world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] = 0;
		EndSample_8D8F0();
		mapNotMoving_E29D6 = false;
		if (endAction == 1)
		{
			m_ExitMenuLoop_E29DC = 1;
			if (a1x)
				a1x->state_4 = 0;
		}
		else if (endAction == 2 && a1x)
		{
			a1x->state_4 = 1;
		}
		SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]);
		VgaPalFadeInFadeOut_90B27(0, 0x10u, 0);
		result = true;
	}
	else
	{
		m_ExitMenuLoop_E29DC = 1;
		gameState_D41A4.levelnumber_43w = LoadLevelNumber_D419C;
		if (a1x)
			a1x->state_4 = 0;
		result = true;
	}
	return result;
}

//----- (0007E0E0) --------------------------------------------------------
signed int MouseEvents_7E0E0()//25f0e0
{
	int spriteIndex; // edx
	int result = 0;
	if (menuState_17DE38.keyScancode_17DF10 == 1)
		return 2;
	int ix = 0;
	if (!(languageButtons_E1F84[0].xmin_10 + (languageButtons_E1F84[0].ymin_12 << 16)))
		return 0;
	do
	{
		if (!(menuState_17DE38.mouseButtons_17DEEE & 1) || result)
		{
			if (languageButtons_E1F84[ix].gold_color_24 && InRegion_7B200(&languageButtons_E1F84[ix], menuState_17DE38.mouseX_17DEE4, menuState_17DE38.mouseY_17DEE6))//confirm language
				spriteIndex = languageButtons_E1F84[ix].goldSprite_20;
			else
				spriteIndex = languageButtons_E1F84[ix].graySprite_21;
			DrawBitmap_2BB40(languageButtons_E1F84[ix].xmin_10, languageButtons_E1F84[ix].ymin_12, menuSprites_17DED4[spriteIndex]);
		}
		else if (InRegion_7B200(&languageButtons_E1F84[ix], menuState_17DE38.mouseX_17DEE4, menuState_17DE38.mouseY_17DEE6))//change language
		{
			PlaySample_8F100(0, 14, 127, 64, 0x64u, 0, 3u);
			DrawBitmap_2BB40(languageButtons_E1F84[ix].xmin_10, languageButtons_E1F84[ix].ymin_12, menuSprites_17DED4[languageButtons_E1F84[ix].goldSprite_20]);//drawing pushed button (gold sprite)
			result = languageButtons_E1F84[ix].id_22;
		}
		ix++;
	} while (languageButtons_E1F84[ix].xmin_10);
	return result;
}

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
	if (!regionButtons_E2008[0].xmin_10)
		return 0;
	do
	{
		//v3 = menuSprites_17DED4[0].pointer;
		//v4 = v0[21];
		regionButtons_E2008[v0y].sizex_14 = menuSprites_17DED4[regionButtons_E2008[v0y].graySprite_21].width_4;
		regionButtons_E2008[v0y].sizey_16 = menuSprites_17DED4[regionButtons_E2008[v0y].graySprite_21].height_5;
		if (!(menuState_17DE38.mouseButtons_17DEEE & 1))
		{
			if (InRegion_7B200(&regionButtons_E2008[v0y], menuState_17DE38.mouseX_17DEE4, menuState_17DE38.mouseY_17DEE6))
				DrawBitmap_2BB40(regionButtons_E2008[v0y].xmin_10, regionButtons_E2008[v0y].ymin_12, menuSprites_17DED4[regionButtons_E2008[v0y].graySprite_21]);
			goto LABEL_9;
		}
		if (InRegion_7B200(&regionButtons_E2008[v0y], menuState_17DE38.mouseX_17DEE4, menuState_17DE38.mouseY_17DEE6))
		{
			PlaySample_8F100(0, 14, 127, 64, 0x64u, 0, 3u);
			DrawBitmap_2BB40(regionButtons_E2008[v0y].xmin_10, regionButtons_E2008[v0y].ymin_12, menuSprites_17DED4[regionButtons_E2008[v0y].graySprite_21]);
			v1 = regionButtons_E2008[v0y].id_22;
			if (regionButtons_E2008[v0y].id_22)
				break;
		}
	LABEL_9:
		//v0 += 22;
		v0y++;
	} while (regionButtons_E2008[v0y].xmin_10);
	return v1;
}
// 17DED4: using guessed type int (int)x_DWORD_17DED4;
// 17DEE4: using guessed type int x_DWORD_17DEE4_mouse_position;
// 17DEEE: using guessed type __int16 x_WORD_17DEEE_mouse_buttons;

//----- (000779E0) --------------------------------------------------------
char LanguageSettingDialog_779E0(type_menuButtons_E1F84* a1y)//2589E0
{
	uint8_t* langBuffer;
	char* langFilename;
	FILE* configFile2;
	char selectLang3;
	FILE* configFile;
	_finddata_t langFileL;
	_finddata_t langFileD;
	typeTextBoxtextBoxStr_E24BCx textBoxStr[2];
	int actualTime;
	bitmap_pos_struct2_t* tabBufferEnd;
	bitmap_pos_struct2_t* tabBuffer;

	uint8_t* tempSmalltit = nullptr;

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

	intptr_t langlhandle = 0;
	intptr_t langdhandle = 0;

	char codeBranch = 2;
	SetCenterScreenForFlyAssistant_6EDB0();//24fdb0
	menuState_17DE38.mouseX_17DEE4 = 320;
	menuState_17DE38.mouseY_17DEE6 = 200;
	if (a1y)//0x0
	{
		LoadHscreen_7A110(videoMode_180660, 12);//load hscreen 25b110
		langBuffer = &smalltitBuffer_E9C38[307200];//4B000
	}
	else
	{
		tempSmalltit = smalltitBuffer_E9C38;
		smalltitBuffer_E9C38 = gameState_D41A4.pointer_0xE2_heapbuffer_226;//[[2a51a4]+e2]
		langBuffer = &smalltitBuffer_E9C38[307200];//406514+4b000
		LoadHscreen_7A110(videoMode_180660, 14);//25b110
	}
	ResetMouse_7B5A0();//25c5a0  disable //enabl
	SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]); //Set cursor to Null (Don't Draw)
	menuState_17DE38.x_WORD_17DEEC = 0;

	if (extendedFonts)
		LoadFixedFonts(0, (char*)"4b");

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

		bitmap_pos_struct_t langDatTab[1000];
		gameState_D41A4.SelectedLangIndex = LoadLanguageFile(&tabBuffer, &tabBufferEnd, langBuffer, langFilename, langDatTab);//2607d0
		unknown_libname_4_find_close(&langFileD, langdhandle);//27b1b3
		if (langlhandle == 0)
		{
			configFile2 = DataFileIO::CreateOrOpenFile(configFilePath, 512);
			if (configFile2 != nullptr)
			{
				DataFileIO::Read(configFile2, (uint8_t*)&configDat.configDatSign_0, 4);
				if (configDat.configDatSign_0 == 0xfffffff7)
				{
					DataFileIO::Read(configFile2, (uint8_t*)&configDat.langIndex_4, 2);
					sprintf(printbuffer, "L%d.TXT", configDat.langIndex_4);
					gameState_D41A4.SelectedLangIndex = LoadLanguageStrings_7F960(tabBuffer, tabBufferEnd, langBuffer, printbuffer, langDatTab);//tady se pak zmeni v1 za v1_langdattab
				}
				DataFileIO::Close(configFile2);
			}
		}
		ResetMouse_7B5A0();//25c5a0
		menuState_17DE38.mouseButtons_17DEEE = 0;
		SetCursor_8CD27(menuSprites_17DED4[37]);//26dd27
		while (mouseClick != 2)//adress 258c30
		{
			SetFrameStart(std::chrono::system_clock::now());
			if (menuState_17DE38.keyScancode_17DF10 == 59)
			{
				gameState_D41A4.showHelp_10 = gameState_D41A4.showHelp_10 != 1;
				gameState_D41A4.setting_38402 = 1;
			}
			if (videoMode_180660 & 1)
				CopyScreen(smalltitBuffer_E9C38, pdwScreenBuffer_351628, 320, 200);
			else
				CopyScreen(smalltitBuffer_E9C38, pdwScreenBuffer_351628, 640, 480);//write default screan 27b144  adress 258c99
			mouseClick = MouseEvents_7E0E0();//25f0e0 adress 258ca1 - change button, return click
			DrawBitmap_2BB40(263, 134, langDatTab[1]);//20cb40 adress 258cba - change flag
			if (gameState_D41A4.showHelp_10 == 1)//is 1 not zero!
			{
				if (codeBranch == 2)
				{
					actualTime = j___clock();//279786 , adresa 258cd9
					if ((actualTime - oldTime) / 100 > 1)//when run 100ms
					{
						if (!languageTextTable_E24BE[textIndex++][0])//2b34be
							textIndex = 0;
						oldTime = actualTime;
					}
					memset(textBoxStr, 0, 2 * sizeof(typeTextBoxtextBoxStr_E24BCx));//26db3a
					textBoxStr[0] = textBoxStr_E24BCx[textIndex];
					DrawTextBoxWithLine_7E840(textBoxStr, 83, 100);//25f840 adress 258d6a - add text?
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
				if (videoMode_180660 & 1)
					VgaBlit320_90478(menuFps);
				else
					VgaBlit640_75200(480, menuFps);//draw //256200
			}
			else
			{
				VgaPalFadeInFadeOut_90B27(menuState_17DE38.palette_17DE38x, 0x20u, 0);//271b27
				frame++;
			}
			GetMouseAndKeyboardEvents_7A060();//25b060 adress 258ddf
			if (mouseClick == 1)
			{
				if (unknown_libname_3_findnext(&langFileL, langlhandle))//258193
				{
					unknown_libname_4_find_close(&langFileL, langlhandle);
					langlhandle = unknown_libname_2_findfirst(languagePathL, 0, &langFileL);
				}
				// L2.TXT
				selectLang2 = LoadLanguageStrings_7F960(tabBuffer, tabBufferEnd, langBuffer, langFileL.name, langDatTab);//adress 258dec
				if (gameState_D41A4.SelectedLangIndex == selectLang2)
				{
					if (unknown_libname_3_findnext(&langFileL, langlhandle))
					{
						unknown_libname_4_find_close(&langFileL, langlhandle);
						langlhandle = unknown_libname_2_findfirst(languagePathL, 0, &langFileL);
					}
					selectLang3 = LoadLanguageStrings_7F960(tabBuffer, tabBufferEnd, langBuffer, langFileL.name, langDatTab);
				}
				else
				{
					selectLang3 = selectLang2;
				}
				gameState_D41A4.SelectedLangIndex = selectLang3;
			}
		}
	}
	//adress 258ea1
	unknown_libname_4_find_close(&langFileL, langlhandle);//adress 258EA2
	configFile = DataFileIO::CreateOrOpenFile(configFilePath, 546);
	if (configFile != nullptr)
	{
		if (gameState_D41A4.setting_38402 == 1)
			gameState_D41A4.setting_38402 = 0;

		configDat.configDatSign_0 = 0xfffffff7;
		configDat.langIndex_4 = gameState_D41A4.SelectedLangIndex;
		configDat.soundVolume_6 = gameState_D41A4.soundVolume_6;
		configDat.musicVolume_8 = gameState_D41A4.musicVolume_8;
		configDat.showHelp_10 = gameState_D41A4.showHelp_10;
		configDat.brightness_11 = gameState_D41A4.brightness_11;
		configDat.brightness_12 = gameState_D41A4.brightness_12;
		configDat.brightness_13 = gameState_D41A4.brightness_13;
		configDat.wordindex_14 = gameState_D41A4.wordindex_14;
		configDat.dwordindex_16 = gameState_D41A4.dwordindex_16;
		configDat.stubb[0] = gameState_D41A4.stubb[0];
		configDat.stubb[1] = gameState_D41A4.stubb[1];
		for (int i = 0; i < 10; i++)
			configDat.keys[i] = keys_EB39E[i];

		DataFileIO::WriteFile_98CAA(configFile, (uint8_t*)&configDat, sizeof(TypeConfigDat));

		DataFileIO::Close(configFile);
	}
	VgaPalFadeInFadeOut_90B27(nullptr, 0x10u, 0);
	if (videoMode_180660 & 1)
	{
		ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, 0);
		ClearScreenAlphaBuffer(320, 200, 255);
	}
	else
	{
		ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 640, 480, 0);
		ClearScreenAlphaBuffer(640, 480, 255);
	}
	if (videoMode_180660 & 1)
		ClearGraphicsBuffer_72883((void*)smalltitBuffer_E9C38, 320, 200, 0);
	else
		ClearGraphicsBuffer_72883((void*)smalltitBuffer_E9C38, 640, 480, 0);

	ResetMouse_7B5A0();
	if (videoMode_180660 & 1)
		VgaBlit320_90478();
	else
		VgaBlit640_75200(480);
	if (langlhandle != 0)//3551d4
	{
		char dataPath[MAX_PATH];
		sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/SCREENS/HSCREEN0.DAT");

		LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.palette_17DE38x, 0, 768);
		LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.screenBuffer_17DE40, menuState_17DE38.fileOffset_17DEDC, 168081);
		VgaPaletteInstall_41A90(menuState_17DE38.palette_17DE38x);
		SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]);//maybe xadatapointerstab
		SetCursor_8CD27(menuSprites_17DED4[39]);
		if (videoMode_180660 & 1)
			CopyScreen((void*)pdwScreenBuffer_351628, (void*)smalltitBuffer_E9C38, 320, 200);
		else
			CopyScreen((void*)pdwScreenBuffer_351628, (void*)smalltitBuffer_E9C38, 640, 480);
		DrawBitmap640_7C120(185, 232, menuSprites_17DED4[66]);
	}
	else
	{
		smalltitBuffer_E9C38 = tempSmalltit;
	}
	LoadAndDecompressDatFile_7AA70(0, 0, 0, 0);
	return 1;
}

//----- (0007E640) --------------------------------------------------------
signed int NewGameDialog_7E640(type_menuButtons_E1F84* a1x)//25f640
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
		v1 = DrawScrollDialog_7BF20(&a1x->scroll_26);
	else
		v1 = 1;
	if (v1 == 1)
	{
		gameState_D41A4.levelnumber_43w = 0;
		memset(levelScores_17DBC8, 0, 500);
		memset(secretLevelScores_17DDBC, 0, 100);
		//for (i = (x_WORD*)unk_E17CC_0x194; i[2]; *(i - 3) = 70)
		for (ix = 0; mapScreenPortals_E17CC[ix].viewPortPosX_4; ix++)
		{
			//*(i - 3) = 70; //6
			mapScreenPortals_E17CC[ix].spriteIndex_16 = 70;
			//i += 11;
			//*((x_BYTE *)i - 4) = 2; //4
			mapScreenPortals_E17CC[ix].activated_18 = 2;
			//*((x_BYTE *)i - 3) = 0; //3
			mapScreenPortals_E17CC[ix].state_19 = 0;
		}
		//for (j = x_WORD_E2970; j[6]; *((x_BYTE *)j - 1) = 0)
		for (int jj = 0; secretMapScreenPortals_E2970[jj].activated_12; jj++)
		{
			secretMapScreenPortals_E2970[jj].state_16 = 0;
			//*((x_BYTE*)j - 1) = 0
			//*(x_WORD *)((char *)j - 5) = 3;
			secretMapScreenPortals_E2970[jj].activated_12 = 3;
			//*(x_WORD *)((char *)j - 3) = 70;
			secretMapScreenPortals_E2970[jj].spriteIndex_14 = 70;
		}
		memset(&world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.str_611, 0, sizeof(type_str_611));
		StopCdPlayback_86860(cdDrive_1803EC);
		LoadAndDecompressDatFile_7AA70(dataPath, menuState_17DE38.gameWorldMap_17DE64, 0xB2C44 + 3, 0x87D80 + 3);
		LoadAndDecompressDatFile_7AA70(0, 0, 0, 0);
		IsPlayingCDTrack_17E09D = 0;
		world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] = 0;
		memset(&mapMenuState_17DB70, 0, 56);
		menuState_17DE38.mapAnimation_17E078x.spriteIndex_8 = 13;
		menuState_17DE38.x_17E06C = 0;
		menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 = 13;
		menuState_17DE38.mapAnimState_17E09C = 0;
		menuState_17DE38.y_17E06E = 480;
		menuState_17DE38.mapAnimation_17E078x.lastSpriteIndex_11 = 16;
		mapMenuState_17DB70.mapState_17DB8F = 1;
		if (a1x)
		{
			a1x->selected_8 = 0;
			ClearScrollDialogVars_7C020(&a1x->scroll_26);
		}
		result = 1;
	}
	else if (v1 == 2)
	{
		a1x->selected_8 = 0;
		ClearScrollDialogVars_7C020(&a1x->scroll_26);
		result = 1;
	}
	else
	{
		result = 0;
	}
	return result;
}

//----- (000780F0) --------------------------------------------------------
bool LoadGameDialog_780F0(type_menuButtons_E1F84* menuButtons) //0x2590f0
{
	char* save_name;
	FILE* SEARCH_FILE;
	FILE* FILE;
	type_menuButtons_E1F84 menuButtons2;
	int numLevelsCompleted = 0;
	uint32_t signature;
	int32_t some_var = 0;
	bool result = false;
	int drawScrollResult = 0;

	uint8_t pal_selected_text = GetPaletteIndex_5BE80(menuState_17DE38.palette_17DE38x, 0x3Fu, 0x3Fu, 0x3Fu);
	uint8_t pal_text = GetPaletteIndex_5BE80(menuState_17DE38.palette_17DE38x, 0x16u, 0x10u, 9u);
	if (menuState_17DE38.savedGameIndex_17DF04 == -1)
	{
		for (int index = 0; index < 8; index++)
		{
			save_name = &menuState_17DE38.xx_BYTE_17DF14[index][0];
			strcpy(save_name, langStrings_E9C4C[414]);
			menuState_17DE38.xx_BYTE_17DF14[index][41] = 0;
			menuState_17DE38.xx_BYTE_17DF14[index][42] = 0;
			std::string saveGameFilePath = GetSaveGameFile(gameFolder.c_str(), index + 1);
			SEARCH_FILE = DataFileIO::CreateOrOpenFile(saveGameFilePath.c_str(), 512);
			if (SEARCH_FILE != NULL)
			{
				DataFileIO::Read(SEARCH_FILE, (uint8_t*)&signature, 4);
				if (signature == 0xFFFFFFF7u)
				{
					DataFileIO::Read(SEARCH_FILE, (uint8_t*)save_name, 20);
					menuState_17DE38.xx_BYTE_17DF14[index][41] = 1;
				}
				DataFileIO::Close(SEARCH_FILE);
			}
		}
		menuState_17DE38.savedGameIndex_17DF04 = 0;
	}
	drawScrollResult = DrawScrollDialog_7BF20(&menuButtons->scroll_26);
	if (drawScrollResult)
	{
		ClearScrollDialogVars_7C020(&menuButtons->scroll_26);
		if (drawScrollResult == 1 && menuState_17DE38.savedGameIndex_17DF04 > 0)
		{
			//Load Saved Game File
			std::string loadFilePath = GetSaveGameFile(gameFolder.c_str(), menuState_17DE38.savedGameIndex_17DF04);
			FILE = DataFileIO::CreateOrOpenFile(loadFilePath.c_str(), 512);
			if (FILE != NULL)
			{
				DataFileIO::Read(FILE, (uint8_t*)&signature, 4);
				if (signature == 0xFFFFFFF7u)
				{
					if (menuButtons->active_25)
						NewGameDialog_7E640(0);
					DataFileIO::Read(FILE, (uint8_t*)&menuState_17DE38.xx_BYTE_17DF14[(menuState_17DE38.savedGameIndex_17DF04 - 1)][0], 20);
					DataFileIO::Read(FILE, (uint8_t*)gameState_D41A4.player_name_57ar, 32);
					DataFileIO::Read(FILE, (uint8_t*)gameState_D41A4.savestring_89, 32);

					//Load completed Secret Portals
					for (int ii = 0; ii < 6; ii++)
					{
						DataFileIO::Read(FILE, readbuffer, 17);
						secretMapScreenPortals_E2970[ii].activated_12 = *(uint16_t*)(readbuffer + 12);
						if (secretMapScreenPortals_E2970[ii].activated_12 == 1)
							secretMapScreenPortals_E2970[ii].spriteIndex_14 = 305;
						else
							secretMapScreenPortals_E2970[ii].spriteIndex_14 = 270;
					}
					DataFileIO::Read(FILE, (uint8_t*)&world_D41A0.m_GameSettings, 16);
					DataFileIO::Read(FILE, (uint8_t*)&numLevelsCompleted, 4);
					DataFileIO::Read(FILE, (uint8_t*)&some_var, 4);
					DataFileIO::Read(FILE, (uint8_t*)&world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.str_611, 505);
					DataFileIO::Read(FILE, (uint8_t*)levelScores_17DBC8, 500);
					DataFileIO::Read(FILE, (uint8_t*)secretLevelScores_17DDBC, 100);
					DataFileIO::Close(FILE);
					world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.word[1] = 0;

					int i = 0;
					//Reset all Portals to inactive
					while (mapScreenPortals_E17CC[i].viewPortPosX_4)
					{
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
							gameState_D41A4.levelnumber_43w = i;
						i++;
					}
					mapMenuState_17DB70.mapState_17DB8F = 1;
					memset(&levelDescription_17DE28, 0, 13);
					mapMenuState_17DB70.selectedLevel_17DB8A = -1;
					if (menuButtons->active_25)
					{
						MapMenuPortalsDraw_81760();
					}
					else
					{
						menuState_17DE38.savedGameIndex_17DF04 = -1;
						NewGameDialog_77350(menuButtons);
						menuButtons->state_4 = 2;
					}
				}
			}
		}
		menuState_17DE38.savedGameIndex_17DF04 = -1;
		result = true;
	}

	for (int jm = 0; jm < 8; jm++)
	{
		int j = jm + 1;
		GetFont_6FC50(1);
		if (menuButtons->scroll_26.scrollPos_36_5 > 16 * (signed __int16)j + 3 * GetLetterHeight_6FC30())
		{
			std::string savegame = std::to_string(j) + ". " + std::string(&menuState_17DE38.xx_BYTE_17DF14[(j - 1)][0]);
			int16_t savegame_y_pos = menuButtons->scroll_26.y1_28_1 + 16 * (j + 1);
			int16_t savegame_x_pos = menuButtons->scroll_26.x1_26_0 + 20;
			uint8_t pal_text_color = (j == menuState_17DE38.savedGameIndex_17DF04) ? pal_selected_text : pal_text;
			DrawText_2BC10(savegame.c_str(), savegame_x_pos, savegame_y_pos, pal_text_color);
		}
	}
	if (menuButtons->scroll_26.scrollPos_36_5 >= menuButtons->scroll_26.height_34_4)
	{
		for (int km = 0; km < 8; km++)
		{
			int k = km + 1;
			menuButtons2.xmin_10 = menuButtons->scroll_26.x1_26_0 + 20;
			menuButtons2.ymin_12 = 16 * k + menuButtons->scroll_26.y1_28_1 + 16;
			menuButtons2.sizex_14 = 100;
			menuButtons2.sizey_16 = 16;
			if (menuState_17DE38.xx_BYTE_17DF14[(k - 1)][41] && InRegion_7B200(&menuButtons2, menuState_17DE38.mouseX_17DEE4, menuState_17DE38.mouseY_17DEE6))
			{
				if (menuState_17DE38.mouseButtons_17DEEE & 1)
				{
					menuState_17DE38.savedGameIndex_17DF04 = k;
				}
				else
				{
					std::string savegame = std::to_string(k) + ". " + std::string(&menuState_17DE38.xx_BYTE_17DF14[(k - 1)][0]);
					int16_t savegame_y_pos = menuButtons->scroll_26.y1_28_1 + 16 * (k + 1);
					int16_t savegame_x_pos = menuButtons->scroll_26.x1_26_0 + 20;
					DrawText_2BC10(savegame.c_str(), savegame_x_pos, savegame_y_pos, pal_selected_text);
				}
			}
		}
	}
	return result;
}

//----- (00079610) --------------------------------------------------------
char SetKeysDialog_79610()//25a610
{
	uint8_t* temp_screen_buffer;
	char textBuff[60];
	int allKeysDone = 0;
	int v39 = 359;
	int keyNameX = 372;
	int buttonAreaWidth = 410;
	int exitFlag = 0;
	int fadeInDone = 0;
	int leftEdge = 191;
	int spriteHeightCopy = menuSprites_17DED4[107].height_5;
	int clock3 = j___clock();
	int clock2 = clock3;
	memset(textBuff, 0, 60);
	LoadHscreen_7A110(videoMode_180660, 15);
	temp_screen_buffer = pdwScreenBuffer_351628;
	pdwScreenBuffer_351628 = smalltitBuffer_E9C38;
	for (int v2_int = 0; keysDialogItems_E28A8[v2_int].posX_0; v2_int++)
		DrawTextWithBoarder_7FCB0(langStrings_E9C4C[keysDialogItems_E28A8[v2_int].textIndex_12], keysDialogItems_E28A8[v2_int].posX_0, v39, keysDialogItems_E28A8[v2_int].posY_2, 4, 0, 0);
	pdwScreenBuffer_351628 = temp_screen_buffer;
	ResetMouse_7B5A0();
	for (int v2_int = 0; keysDialogItems_E28A8[v2_int].posX_0; v2_int++)
		keysDialogItems_E28A8[v2_int].state_14 = 0;
	keysDialogItems_E28A8[0].state_14 = 1;

	SetCursor_8CD27(menuSprites_17DED4[110]);
	while (exitFlag != 2)
	{
		SetFrameStart(std::chrono::system_clock::now());
		int clockNow = j___clock();
		if (videoMode_180660 & 1)
			CopyScreen((void*)smalltitBuffer_E9C38, (void*)pdwScreenBuffer_351628, 320, 200);
		else
			CopyScreen((void*)smalltitBuffer_E9C38, (void*)pdwScreenBuffer_351628, 640, 480);
		//backgroung
		if (menuState_17DE38.mouseButtons_17DEEE & 1)//switch blocks
		{
			for (int j_int = 0; keysDialogItems_E28A8[j_int].posX_0; j_int++)
			{
				type_menuButtons_E1F84 box;
				box.xmin_10 = leftEdge;
				box.ymin_12 = keysDialogItems_E28A8[j_int].posY_2 - 4;
				box.sizex_14 = buttonAreaWidth;
				box.sizey_16 = spriteHeightCopy;
				if (InRegion_7B200(&box, menuState_17DE38.mouseX_17DEE4, menuState_17DE38.mouseY_17DEE6))
				{
					for (int k_int = 0; keysDialogItems_E28A8[k_int].posX_0; k_int++)
						keysDialogItems_E28A8[k_int].state_14 = 0;
					keysDialogItems_E28A8[0].state_14 = 1;
					allKeysDone = 0;
					break;
				}
			}
		}
		exitFlag = TestMouseRegions_7E1F0();
		if (exitFlag == 1)
		{
			gameState_D41A4.setting_38402 = 1;
			SetDefaultKeys_5BCC0();
			for (int l_int = 0; keysDialogItems_E28A8[l_int].posX_0; l_int++)
				keysDialogItems_E28A8[l_int].state_14 = 0;
			keysDialogItems_E28A8[0].state_14 = 1;
		}
		uint8_t* keyIter = &keys_EB39E[0]; //2bc39e
		int v14_int = 0;
		while (keyIter < &keys_EB39E[9] + 1) //drawing texts
		{
			memset(textBuff, 0, 60);
			if (!keysDialogItems_E28A8[v14_int].state_14)
			{
				GetKeyName_79E10(textBuff, *keyIter);
				DrawTextWithBoarder_7FCB0(textBuff, keyNameX, buttonAreaWidth, keysDialogItems_E28A8[v14_int].posY_2, 4, 0, 0);
			}
			keyIter++;
			v14_int++;
		}
		int v2_int = 0;
		for (uint8_t* keyIter2 = &keys_EB39E[0]; keyIter2 < &keys_EB39E[9] + 1 && !allKeysDone; ++keyIter2)
		{
			int keySlotState = keysDialogItems_E28A8[v2_int].state_14;
			if (keySlotState >= 1u)
			{
				unsigned int elapsedTime = clockNow - clock3;
				if (keySlotState <= 1u)
				{
					memset(textBuff, 0, 60);
					GetKeyName_79E10(textBuff, *keyIter2);
					DrawTextWithBoarder_7FCB0(textBuff, keyNameX, buttonAreaWidth, keysDialogItems_E28A8[v2_int].posY_2, 4, 0, 0);
					if (elapsedTime > 0x32)
					{
						keysDialogItems_E28A8[v2_int].state_14 = 2;
						clock3 = clockNow;
					}
					DrawBitmap_2BB40(leftEdge, keysDialogItems_E28A8[v2_int].posY_2, menuSprites_17DED4[107]);
					if (menuState_17DE38.keyScancode_17DF10 && GetKeyName_79E10(textBuff, menuState_17DE38.keyScancode_17DF10))
					{
						bool isScancode = false;
						if (keyIter2 != &keys_EB39E[6])
						{
							for (uint8_t* keyIter3 = &keys_EB39E[0]; keyIter3 < &keys_EB39E[9] + 1; keyIter3++)
							{
								if (menuState_17DE38.keyScancode_17DF10 == *keyIter3)
								{
									isScancode = true;
									break;
								}
							}
						}
						if (!isScancode)
						{
							gameState_D41A4.setting_38402 = 1;
							keysDialogItems_E28A8[v2_int].state_14 = 0;
							v2_int++;
							*keyIter2 = menuState_17DE38.keyScancode_17DF10;
							menuState_17DE38.keyScancode_17DF10 = 0;
							if (keysDialogItems_E28A8[v2_int].posX_0)
							{
								keysDialogItems_E28A8[v2_int].state_14 = 1;
							}
							else
							{
								allKeysDone = 1;
								clock3 = clockNow;
							}
						}
					}
				}
				else if (keySlotState == 2)
				{
					if ((clockNow - clock3) > 0x32)
					{
						keysDialogItems_E28A8[v2_int].state_14 = 1;
						clock3 = clockNow;
					}
					DrawBitmap_2BB40(leftEdge, keysDialogItems_E28A8[v2_int].posY_2, menuSprites_17DED4[107]);
					if (menuState_17DE38.keyScancode_17DF10)
					{
						if (GetKeyName_79E10(textBuff, menuState_17DE38.keyScancode_17DF10))
						{
							bool isScancode2 = false;
							if (keyIter2 != &keys_EB39E[9] + 1)
							{
								for (uint8_t* ii = &keys_EB39E[0]; ii < &keys_EB39E[9] + 1; ++ii)
								{
									if (menuState_17DE38.keyScancode_17DF10 == *ii)
									{
										isScancode2 = true;
										break;
									}
								}
							}
							if (!isScancode2)
							{
								gameState_D41A4.setting_38402 = 1;
								*keyIter2 = menuState_17DE38.keyScancode_17DF10;
								keysDialogItems_E28A8[v2_int].state_14 = 0;
								menuState_17DE38.keyScancode_17DF10 = 0;
								v2_int++;
								if (keysDialogItems_E28A8[v2_int].posX_0)
									keysDialogItems_E28A8[v2_int].state_14 = 1;
								else
									allKeysDone = 1;
							}
						}
					}
				}
			}
			v2_int++;
		}
		if (allKeysDone >= 1u)
		{
			unsigned int stepsElapsed = clockNow - clock3;
			if (allKeysDone <= 1u)
			{
				if (stepsElapsed > 0x32)
				{
					allKeysDone = 2;
					clock3 = clockNow;
				}
				DrawBitmap_2BB40(283, 381, menuSprites_17DED4[108]);
			}
			else if (allKeysDone == 2 && stepsElapsed > 0x32)
			{
				allKeysDone = 1;
				clock3 = clockNow;
			}
		}
		if (videoMode_180660 & 1)
			VgaBlit320_90478(menuFps);
		else
			VgaBlit640_75200(480, menuFps);
		if (!fadeInDone)
		{
			VgaPalFadeInFadeOut_90B27(menuState_17DE38.palette_17DE38x, 0x20u, 0);
			fadeInDone = 1;
		}
		if ((clockNow - clock2) > 0xA)
		{
			GetMouseAndKeyboardEvents_7A060();
			clock2 = clockNow;
		}
		if (menuState_17DE38.keyScancode_17DF10 == 1)
			exitFlag = 2;
		if (menuState_17DE38.keyScancode_17DF10 == 0x1c)
		{
			if (allKeysDone)
				exitFlag = 2;
		}
	}
	WriteConfigDat_81DB0();
	VgaPalFadeInFadeOut_90B27(0, 0x10u, 0);
	if (videoMode_180660 & 1)
	{
		ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, 0);
		ClearScreenAlphaBuffer(320, 200, 255);
	}
	else
	{
		ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 640, 480, 0);
		ClearScreenAlphaBuffer(640, 480, 255);
	}
	if (videoMode_180660 & 1)
	{
		ClearGraphicsBuffer_72883((void*)smalltitBuffer_E9C38, 320, 200, 0);
	}
	else
	{
		ClearGraphicsBuffer_72883((void*)smalltitBuffer_E9C38, 640, 480, 0);
	}
	ResetMouse_7B5A0();
	if (videoMode_180660 & 1)
		VgaBlit320_90478();
	else
		VgaBlit640_75200(480);

	char dataPath[MAX_PATH];
	sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/SCREENS/HSCREEN0.DAT");

	LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.palette_17DE38x, 0, 768);
	LoadAndDecompressDatFile_7AA70(dataPath, menuState_17DE38.screenBuffer_17DE40, menuState_17DE38.fileOffset_17DEDC, 168081);
	LoadAndDecompressDatFile_7AA70(0, 0, 0, 0);
	SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]);
	VgaPaletteInstall_41A90(menuState_17DE38.palette_17DE38x);
	SetCursor_8CD27(menuSprites_17DED4[39]);
	if (videoMode_180660 & 1)
		CopyScreen((void*)pdwScreenBuffer_351628, (void*)smalltitBuffer_E9C38, 320, 200);
	else
		CopyScreen((void*)pdwScreenBuffer_351628, (void*)smalltitBuffer_E9C38, 640, 480);
	return 1;
}

//----- (00079E10) --------------------------------------------------------
signed int GetKeyName_79E10(char* a1, char a2)
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
		DrawBitmap_2BB40(ani.PosX_4, ani.PosY_6, menuSprites_17DED4[ani.ActSprite_8]);
		if ((now - ani.LastTimeRendered_0) >> 2 >= 1)
		{
			ani.ActSprite_8++;
			if (ani.LastSprite_11 < ani.ActSprite_8)
				ani.ActSprite_8 = ani.FirstSprite_10;
			ani.LastTimeRendered_0 = now;
		}
	}
	// draw selected main menu item
	for (int iy = 0; (mainMenuButtons_E1BAC[iy].xmin_10<<16) + mainMenuButtons_E1BAC[iy].ymin_12; iy++)
	{
		if (mainMenuButtons_E1BAC[iy].canSelect_23 && mainMenuButtons_E1BAC[iy].gold_color_24)
		{
			DrawBitmap_2BB40(mainMenuButtons_E1BAC[iy].xmin_10, mainMenuButtons_E1BAC[iy].ymin_12, menuSprites_17DED4[mainMenuButtons_E1BAC[iy].graySprite_21]);
		}
	}
}

//----- (0007AC00) --------------------------------------------------------
void LoadAndSetGraphicsAndPalette_7AC00()//25BC00
{
	//char v0; // al
	//unsigned __int8 v1; // al
	//unsigned __int8 v2; // al

	if (!(videoMode_180660 & 8))
	{
		/*if (ptrMemoryBuffer_E9C3C)
		{
			sub_83E80_freemem4(ptrMemoryBuffer_E9C3C);
			ptrMemoryBuffer_E9C3C = 0;
		}*/
		//fix
		if (memoryBufferAlloc_E9C3C)
		{
			FreeMem_83E80(memoryBufferAlloc_E9C3C);
			memoryBufferAlloc_E9C3C = 0;
			ptrMemoryBuffer_E9C3C = 0;
		}
		//fix
		MouseReset_54600();//235600 //mouse reset
		CreateFileArrayIndexes_6EBF0(&filearray_2aa18c[filearrayindex_POINTERSDATTAB]);//24FBF0 - 2AA18C//?tab
		if (videoMode_180660 == 1)
		{
			menuVideoMode_E29DA = videoMode_180660;
			videoMode_180660 = 8;
			/*
			ptrMemoryBuffer_E9C3C = (uint8_t*)sub_83CD0_malloc2(307200);
			*/
			//fix
			memoryBufferAlloc_E9C3C = (uint8_t*)Malloc_83CD0(3000000);
			ptrMemoryBuffer_E9C3C = &memoryBufferAlloc_E9C3C[200000];
			//fix
			CreateIndexes_6EB90(&filearray_2aa18c[filearrayindex_POINTERSDATTAB]);
			memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 768);
			if (videoMode_180660 & 1)
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
			VgaPaletteInstall_41A90((TColor*)*xadatapald0dat2.colorPalette_var28);
			if (videoMode_180660 & 1)
				VgaSetVideoMode320x200AndPalette_90D6E((TColor*)*xadatapald0dat2.colorPalette_var28);
			else
				VgaSetVideoMode640x480AndPalette_90E07((TColor*)*xadatapald0dat2.colorPalette_var28);
			VgaPaletteInstall_41A90((TColor*)*xadatapald0dat2.colorPalette_var28);
			uiBackGroundColorIdx_EB3A8 = GetPaletteIndex_5BE80((TColor*)*xadatapald0dat2.colorPalette_var28, 0, 0, 0);
			if (videoMode_180660 & 1)
			{
				ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, uiBackGroundColorIdx_EB3A8);
				ClearScreenAlphaBuffer(320, 200, 255);
			}
			else
			{
				ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 640, 480, uiBackGroundColorIdx_EB3A8);
				ClearScreenAlphaBuffer(640, 480, 255);
			}

			InstallMouse_8CEDF();
			SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]); //Set cursor to Null (Don't Draw)
		}
	}
	SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]); //Set cursor to Null (Don't Draw)
}

//----- (0007ADE0) --------------------------------------------------------
void SetMenuVideoMode_7ADE0(char a1)//25bde0
{
	char v1; // al
	unsigned __int8 v2; // al
	unsigned __int8 v3; // al

	if (a1 == 1)
	{
		//fix
		if (memoryBufferAlloc_E9C3C)
		{
			FreeMem_83E80(memoryBufferAlloc_E9C3C);
			memoryBufferAlloc_E9C3C = 0;
			ptrMemoryBuffer_E9C3C = 0;
		}
		//fix
		MouseReset_54600();//mouse reset
		CreateFileArrayIndexes_6EBF0(&filearray_2aa18c[filearrayindex_POINTERSDATTAB]);
		videoMode_180660 = 1;
		menuVideoMode_E29DA = 1;
		//fix
		memoryBufferAlloc_E9C3C = (uint8_t*)Malloc_83CD0(3000000);
		ptrMemoryBuffer_E9C3C = &memoryBufferAlloc_E9C3C[2000000];
		//fix
		CreateIndexes_6EB90(&(filearray_2aa18c[filearrayindex_POINTERSDATTAB]));
		memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 768);
		if (videoMode_180660 & 1)
		{
			v1 = GetPaletteIndex_5BE80((TColor*)*xadatapald0dat2.colorPalette_var28, 0, 0, 0);
			ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, v1);
			ClearScreenAlphaBuffer(320, 200, 255);
		}
		else
		{
			v2 = GetPaletteIndex_5BE80((TColor*)*xadatapald0dat2.colorPalette_var28, 0, 0, 0);
			ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 640, 480, v2);
			ClearScreenAlphaBuffer(640, 480, 255);
		}
		memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 768);
		VgaPaletteInstall_41A90((TColor*)*xadatapald0dat2.colorPalette_var28);
		if (videoMode_180660 & 1)
			VgaSetVideoMode320x200AndPalette_90D6E((TColor*)*xadatapald0dat2.colorPalette_var28);
		else
			VgaSetVideoMode640x480AndPalette_90E07((TColor*)*xadatapald0dat2.colorPalette_var28);
		VgaPaletteInstall_41A90((TColor*)*xadatapald0dat2.colorPalette_var28);
		v3 = GetPaletteIndex_5BE80((TColor*)*xadatapald0dat2.colorPalette_var28, 0, 0, 0);
		uiBackGroundColorIdx_EB3A8 = v3;
		if (videoMode_180660 & 1)
		{
			ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, v3);
			ClearScreenAlphaBuffer(320, 200, 255);
		}
		else
		{
			ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 640, 480, v3);
			ClearScreenAlphaBuffer(640, 480, 255);
		}
		InstallMouse_8CEDF();
	}
	SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]); //Set cursor to Null (Don't Draw)
}

//----- (0007BEC0) --------------------------------------------------------
void ResetMainMenuButtons_7BEC0()//25cec0
{
	//uint8_t* result; // eax
	int resultx;
	unsigned __int8 v1; // dl

	//for (result = off_E1BAC; *((uint16_t*)result + 5); result += 44)
	for (resultx = 0; mainMenuButtons_E1BAC[resultx].xmin_10; resultx++)
	{
		mainMenuButtons_E1BAC[resultx].selected_8 = 0;
		v1 = mainMenuButtons_E1BAC[resultx].id_22;
		mainMenuButtons_E1BAC[resultx].gold_color_24 = 0;
		if (v1 == 7u)
		{
			mainMenuButtons_E1BAC[resultx].canSelect_23 = gameState_D41A4.isNetwork_216w != 0;
		}
		else if (v1 == 13)
		{
			mainMenuButtons_E1BAC[resultx].canSelect_23 = 1;
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
				*(off_E1BAC + i + 17) = (gameState_D41A4.setting_216 != 0);
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
void PortalsUpdate_7DD70()//25ed70
{
	for (int levelIndex = 0; mapScreenPortals_E17CC[levelIndex].viewPortPosX_4; levelIndex++)
	{
		mapScreenPortals_E17CC[levelIndex].word_20 = 0;
		if (mapScreenPortals_E17CC[levelIndex].activated_18 == 2)
			mapScreenPortals_E17CC[levelIndex].spriteIndex_16 = 70;
		if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] & 2
			&& gameState_D41A4.levelnumber_43w == levelIndex)
		{
			mapScreenPortals_E17CC[levelIndex].activated_18 = 1;
		}
		if (mapScreenPortals_E17CC[levelIndex].activated_18 == 1)
			mapScreenPortals_E17CC[levelIndex].spriteIndex_16 = 37;
		mapScreenPortals_E17CC[levelIndex].state_19 = 0;
	}
	if (gameState_D41A4.levelnumber_43w > 0x18u)
	{
		for (int ir = 0; secretMapScreenPortals_E2970[ir].activated_12; ir++)
		{
			if (gameState_D41A4.levelnumber_43w == secretMapScreenPortals_E2970[ir].levelNumber_6)
			{
				if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] & 2)
				{
					secretMapScreenPortals_E2970[ir].activated_12 = 1;
					secretMapScreenPortals_E2970[ir].spriteIndex_14 = 305;
				}
				else if (secretMapScreenPortals_E2970[ir].activated_12 == 3)
				{
					secretMapScreenPortals_E2970[ir].activated_12 = 2;
					secretMapScreenPortals_E2970[ir].spriteIndex_14 = 70;
				}
				mapScreenPortals_E17CC[secretMapScreenPortals_E2970[ir].index_4].activated_18 = 1;
				mapScreenPortals_E17CC[secretMapScreenPortals_E2970[ir].index_4].spriteIndex_16 = 37;
				break;
			}
		}
	}
}

int GetMapMenuDialogIndex_7E320(uint32_t adress, type_menuButtons_E1F84* a1x)
{
	switch (adress)
	{
	case 0x25f800: {
		return SaveGameDialog_7E800(a1x);//25f800
	}
	case 0x25f820: {
		return LoadGameDialog_7E820(a1x);//25f800
	}
	case 0x25f640: {
		return NewGameDialog_7E640(a1x);//25f800
	}
	case 0x25f620: {
		return DeselectMenuButton_7E620(a1x);//25f800
	}
	}
	mydelay(10000);
	exit(1);
}

//----- (0007E620) --------------------------------------------------------
signed int DeselectMenuButton_7E620(type_menuButtons_E1F84* a1x)//25f620
{
	a1x->selected_8 = 0;
	return 2;
}

//----- (0007DA70) --------------------------------------------------------
void SetAnimationVariables_7DA70(__int16 x1, __int16 y1, __int16 x2, __int16 y2, __int16 a5, __int16 a6)//25ea70 //draw player in map menu
{
	axis_3d axis1;
	axis_3d axis2;
	menuState_17DE38.x2_17E074 = x2;
	menuState_17DE38.x_17E06C = x1;
	menuState_17DE38.x2_17E074 = menuSprites_17DED4[37].width_4 / 2 + x2;
	menuState_17DE38.y2_17E076 = menuSprites_17DED4[37].height_5 / 2 + y2;
	menuState_17DE38.x_17E06C = menuSprites_17DED4[37].width_4 / 2 + x1;
	menuState_17DE38.y_17E06E = menuSprites_17DED4[37].height_5 / 2 + y1;
	axis1.x = menuSprites_17DED4[37].width_4 / 2 + x2;
	axis1.y = menuSprites_17DED4[37].height_5 / 2 + y2;
	axis2.x = menuSprites_17DED4[37].width_4 / 2 + x1;
	axis2.y = menuSprites_17DED4[37].height_5 / 2 + y1;
	int atan2 = Maths::GetYawAngle_581E0(&axis1, &axis2);
	if (atan2 <= 1024 || atan2 >= 1536)
	{
		if (atan2 <= 1536 || atan2 >= 2048)
		{
			if (atan2 <= 2048 && atan2 <= 0 || atan2 >= 512)
			{
				if (atan2 <= 512 || atan2 >= 1024)
				{
					if (atan2 != 2048 && atan2)
					{
						if (atan2 == 512)
						{
							if (menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 != 9)
							{
								menuState_17DE38.mapAnimation_17E078x.spriteIndex_8 = 9;
								menuState_17DE38.mapAnimation_17E078x.lastSpriteIndex_11 = 12;
								menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 = 9;
							}
						}
						else if (atan2 == 1024)
						{
							if (menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 != 1)
							{
								menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 = 1;
								menuState_17DE38.mapAnimation_17E078x.spriteIndex_8 = 1;
								menuState_17DE38.mapAnimation_17E078x.lastSpriteIndex_11 = 4;
							}
						}
						else if (atan2 == 1536 && menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 != 25)
						{
							menuState_17DE38.mapAnimation_17E078x.spriteIndex_8 = 25;
							menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 = 25;
							menuState_17DE38.mapAnimation_17E078x.lastSpriteIndex_11 = 28;
						}
					}
					else if (menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 != 17)
					{
						menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 = 17;
						menuState_17DE38.mapAnimation_17E078x.spriteIndex_8 = 17;
						menuState_17DE38.mapAnimation_17E078x.lastSpriteIndex_11 = 20;
					}
				}
				else if (menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 != 13)
				{
					menuState_17DE38.mapAnimation_17E078x.spriteIndex_8 = 13;
					menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 = 13;
					menuState_17DE38.mapAnimation_17E078x.lastSpriteIndex_11 = 16;
				}
			}
			else if (menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 != 5)
			{
				menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 = 5;
				menuState_17DE38.mapAnimation_17E078x.spriteIndex_8 = 5;
				menuState_17DE38.mapAnimation_17E078x.lastSpriteIndex_11 = 8;
			}
		}
		else if (menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 != 29)
		{
			menuState_17DE38.mapAnimation_17E078x.spriteIndex_8 = 29;
			menuState_17DE38.mapAnimation_17E078x.lastSpriteIndex_11 = 32;
			menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 = 29;
		}
	}
	else if (menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 != 21)
	{
		menuState_17DE38.mapAnimation_17E078x.spriteIndex_8 = 21;
		menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 = 21;
		menuState_17DE38.mapAnimation_17E078x.lastSpriteIndex_11 = 24;
	}
	menuState_17DE38.mapAnimation_17E078x.posX_4 = menuState_17DE38.x_17E06C;
	menuState_17DE38.mapAnimation_17E078x.posY_6 = menuState_17DE38.y_17E06E;
	MoveAnimIndex_81260(
		&menuState_17DE38.mapAnimation_17E078x,
		menuState_17DE38.x_17E06C - menuSprites_17DED4[menuState_17DE38.mapAnimation_17E078x.spriteIndex_8].width_4 / 2 - a5,
		menuState_17DE38.y_17E06E - menuSprites_17DED4[menuState_17DE38.mapAnimation_17E078x.spriteIndex_8].height_5 / 2 - a6);
}

//----- (0007E320) --------------------------------------------------------
signed int DrawBitmapAndPlaySound_7E320()//25f320
{
	typeTextBoxtextBoxStr_E24BCx textBoxStr[2];
	for (int i = 0; mapMenuButtons_E23E0[i].active_25; i++) //draw buttons
	{
		if (mapMenuButtons_E23E0[i].canSelect_23) {
			DrawBitmap640_7C120(mapMenuButtons_E23E0[i].xmin_10, mapMenuButtons_E23E0[i].ymin_12, menuSprites_17DED4[mapMenuButtons_E23E0[i].graySprite_21]);
		}
	}
	if (mapMenuState_17DB70.levelChosen_17DB8E)
		return 0;
	int index = 0;
	if (mapMenuButtons_E23E0[index].active_25) {
		do //adress 25f57c
		{
			if (mapMenuButtons_E23E0[index].selected_8 && mapMenuButtons_E23E0[index].callback_0) {
				uint8_t dialogIndex = GetMapMenuDialogIndex_7E320(mapMenuButtons_E23E0[index].callback_0, &mapMenuButtons_E23E0[index]);
				if (!dialogIndex)//
					return 1;
				mapMenuButtons_E23E0[index].selected_8 = 0;
				ResetMouse_7B5A0();
				return dialogIndex;
			}
			index++;
		} while (mapMenuButtons_E23E0[index].active_25);
	}
	for (int i = 0; mapMenuButtons_E23E0[i].active_25; i++) {
		if (mapMenuButtons_E23E0[i].canSelect_23 && mapMenuButtons_E23E0[i].gold_color_24)
			DrawBitmap640_7C120(mapMenuButtons_E23E0[i].xmin_10, mapMenuButtons_E23E0[i].ymin_12, menuSprites_17DED4[mapMenuButtons_E23E0[i].goldSprite_20]);
		mapMenuButtons_E23E0[i].selected_8 = 0;
		mapMenuButtons_E23E0[i].gold_color_24 = 0;
	}
	for (int i = 0;; i++) {
		if (!mapMenuButtons_E23E0[i].active_25) {
			DrawMapMenuHelpText_82510();
			return 0;
		}
		if (mapMenuButtons_E23E0[i].canSelect_23) {
			mapMenuButtons_E23E0[i].sizex_14 = menuSprites_17DED4[mapMenuButtons_E23E0[i].goldSprite_20].width_4;
			mapMenuButtons_E23E0[i].sizey_16 = menuSprites_17DED4[mapMenuButtons_E23E0[i].goldSprite_20].height_5;
			if (InRegion_7B200(&mapMenuButtons_E23E0[i], menuState_17DE38.mouseX_17DEE4, menuState_17DE38.mouseY_17DEE6)) {
				if (menuState_17DE38.mouseButtons_17DEEE & 1) {
					mapMenuButtons_E23E0[i].selected_8 = 1;
					ResetMouse_7B5A0();
					PlaySample_8F100(0, 14, 127, 64, 0x64u, 0, 3u);
				}
				else {
					mapMenuButtons_E23E0[i].gold_color_24 = 1;
					if (gameState_D41A4.showHelp_10 != 1)
						return 0;
					int index2 = 0;
					if (!textBoxStr_E2516[index2].left_2)
						return 0;
					do //adress 25f4e7
					{
						if (textBoxStr_E2516[index2].buttonId_17 == mapMenuButtons_E23E0[i].id_22) {
							memset(textBoxStr, 0, 36);
							textBoxStr[0] = textBoxStr_E2516[index2];
							bitmap_pos_struct_t* tempSpriteStr = fontSprites_17DEC0;
							bitmap_pos_struct2_t* tempx_DWORD_17DEC4 = menuState_17DE38.fontTabEnd_17DEC4;
							fontSprites_17DEC0 = font2Sprites_17DEC8;
							menuState_17DE38.fontTabEnd_17DEC4 = menuState_17DE38.font2TabEnd_17DECC;
							DrawTextBoxWithLine_7E840(textBoxStr, 238, 264);
							fontSprites_17DEC0 = tempSpriteStr;
							menuState_17DE38.fontTabEnd_17DEC4 = tempx_DWORD_17DEC4;
							return 0;
						}
						index2++;
					} while (textBoxStr_E2516[index2].left_2);
					return 0;
				}
			}
		}
	}
}

//----- (0007E800) --------------------------------------------------------
char SaveGameDialog_7E800(type_menuButtons_E1F84* a1x)//25f800
{
	char result; // al

	result = SaveGameDialog_78730(a1x);
	if (result)
		a1x->state_4 = 0;
	return result;
}

//----- (00078730) --------------------------------------------------------
char SaveGameDialog_78730(type_menuButtons_E1F84* a1x)//259730
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
	type_menuButtons_E1F84 v47x; // [esp+50h] [ebp+Eh]
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
	v68 = GetPaletteIndex_5BE80(menuState_17DE38.palette_17DE38x, 0x3Fu, 0x3Fu, 0x3Fu);
	v67 = GetPaletteIndex_5BE80(menuState_17DE38.palette_17DE38x, 0x16u, 0x10u, 9u);
	//v1 = a1x->word_26;//a1 + 13;
	if (menuState_17DE38.savedGameIndex_17DF04 == -1)
	{
		for (i = 1; i < 9; i++)
		{
			v2 = i;
			//v3 = 43 * (i - 1);
			v4 = langStrings_E9C4C[414];//(char *)x_DWORD_EA2C4; //Empty
			v5 = &menuState_17DE38.xx_BYTE_17DF14[(i - 1)][0];
			v45 = &menuState_17DE38.xx_BYTE_17DF14[(i - 1)][0];
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
			std::string saveGameFilePath = GetSaveGameFile(gameFolder.c_str(), v2);
			file1 = DataFileIO::CreateOrOpenFile(saveGameFilePath.c_str(), 512);
			if (file1 != NULL)
			{
				DataFileIO::Read(file1, (uint8_t*)&v54, 4);
				if (v54 == -9)
					DataFileIO::Read(file1, (uint8_t*)v56, 20);
				DataFileIO::Close(file1);
			}
		}
		menuState_17DE38.savedGameIndex_17DF04 = 0;
	}
	v59 = DrawScrollDialog_7BF20(&a1x->scroll_26);
	if (v59)
	{
		ClearScrollDialogVars_7C020(&a1x->scroll_26);
		if (v59 == 1 && menuState_17DE38.savedGameIndex_17DF04 > 0)
		{
			v9 = &menuState_17DE38.xx_BYTE_17DF14[(menuState_17DE38.savedGameIndex_17DF04 - 1)][0];
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
			std::string saveGameFilePath = GetSaveGameFile(gameFolder.c_str(), menuState_17DE38.savedGameIndex_17DF04);
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
				v53 = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2];
				DataFileIO::WriteFile_98CAA(file2, (uint8_t*)&v54, 4);
				DataFileIO::WriteFile_98CAA(file2, (uint8_t*)&menuState_17DE38.xx_BYTE_17DF14[menuState_17DE38.savedGameIndex_17DF04 - 1][0], 20);
				DataFileIO::WriteFile_98CAA(file2, (uint8_t*)gameState_D41A4.player_name_57ar, 32);
				DataFileIO::WriteFile_98CAA(file2, (uint8_t*)gameState_D41A4.savestring_89, 32);
				DataFileIO::WriteFile_98CAA(file2, (uint8_t*)&secretMapScreenPortals_E2970, 102);
				DataFileIO::WriteFile_98CAA(file2, (uint8_t*)&world_D41A0.m_GameSettings, 16);
				DataFileIO::WriteFile_98CAA(file2, (uint8_t*)&v55, 4);
				DataFileIO::WriteFile_98CAA(file2, (uint8_t*)&v53, 4);
				DataFileIO::WriteFile_98CAA(file2, (uint8_t*)&world_D41A0.players_0x2BDE[0].playerData_0x3E6_2BE4_12228.str_611, 505);
				DataFileIO::WriteFile_98CAA(file2, (uint8_t*)levelScores_17DBC8, 500);
				DataFileIO::WriteFile_98CAA(file2, (uint8_t*)secretLevelScores_17DDBC, 100);
				DataFileIO::Close(file2);
			}
		}
		menuState_17DE38.savedGameIndex_17DF04 = -1;
		result = 1;
	}
	for (j = 1; j < 9; j++)
	{
		GetFont_6FC50(1);
		if (a1x->scroll_26.scrollPos_36_5 > 16 * j + 3 * GetLetterHeight_6FC30())//0160:259AB7
		{
			sprintf(printbuffer, "%d.", j);
			v62 = 16 * j;
			v16 = &menuState_17DE38.xx_BYTE_17DF14[(j - 1)][0];
			if (j == menuState_17DE38.savedGameIndex_17DF04)
			{//draw edtited text line loc 78b10
				v20 = a1x->scroll_26.x1_26_0;
				v21 = 16 * j + 20;
				v63 = v20 + 20;
				LOWORD(v21) = a1x->scroll_26.y1_28_1;
				//v57 = v68;
				//v58 = (signed __int16)(v21 + 16 + v62);
				v61 = v21 + 16 + v62;
				DrawText_2BC10(printbuffer, v63, v21 + 16 + v62, v68);//draw number
				//v22 = v57;
				//v23 = v58;
				//v24 = v63 + 3 * GetLetterWidth_6FC10();
				DrawText_2BC10(v16 + 20, v63 + 3 * GetLetterWidth_6FC10(), v21 + 16 + v62, v68);//draw text
			}
			else
			{
				//v57 = v67;
				DrawText_2BC10(printbuffer, a1x->scroll_26.x1_26_0 + 20, v62 + a1x->scroll_26.y1_28_1 + 16, v67);//draw number
				//v17 = v67;
				v18 = v62 + a1x->scroll_26.y1_28_1 + 16;
				v19 = a1x->scroll_26.x1_26_0 + 20 + 3 * GetLetterWidth_6FC10();
				DrawText_2BC10(v16, v19, v18, v67);//draw text
			}
		}
	}
	if (a1x->scroll_26.scrollPos_36_5 >= a1x->scroll_26.height_34_4)
	{
		for (k = 1; k < 9; k++)
		{
			v47x.xmin_10 = a1x->scroll_26.x1_26_0 + 20;//x pos
			v25 = a1x->scroll_26.y1_28_1;
			v64 = 16 * k;
			v47x.sizey_16 = 16;//y size
			v47x.sizex_14 = 90;//x size
			v47x.ymin_12 = v25 + 16 + 16 * k;//y pos
			/*
			v47 =355150
			6801 0000 2000 9ff33c00620041009051100088513500051100000
			*/
			if (InRegion_7B200(&v47x, menuState_17DE38.mouseX_17DEE4, menuState_17DE38.mouseY_17DEE6))
			{
				if (menuState_17DE38.mouseButtons_17DEEE & 1)
				{
					menuState_17DE38.savedGameIndex_17DF04 = k;//clicked on first save
					v26 = &menuState_17DE38.xx_BYTE_17DF14[(k - 1)][0];
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
					menuState_17DE38.xx_BYTE_17DF14[(menuState_17DE38.savedGameIndex_17DF04 - 1)][40] = 0;
					//LOWORD(v30) = *v1;
					//v30 += 20;
					v63 = a1x->scroll_26.x1_26_0 + 20;
					//LOWORD(v30) = a1[14];
					v61 = a1x->scroll_26.y1_28_1 + 16 + v64;
					menuState_17DE38.xx_BYTE_17DF14[(menuState_17DE38.savedGameIndex_17DF04 - 1)][42] = 0;
				}
				else if (k != menuState_17DE38.savedGameIndex_17DF04)
				{//draw text highlight
					//v31 = 43 * (k - 1);
					sprintf(printbuffer, "%d.", k);
					v32 = v68;
					DrawText_2BC10(printbuffer, a1x->scroll_26.x1_26_0 + 20, v64 + a1x->scroll_26.y1_28_1 + 16, v68);
					v33 = v64 + a1x->scroll_26.y1_28_1 + 16;
					v34 = GetLetterWidth_6FC10();
					DrawText_2BC10(&menuState_17DE38.xx_BYTE_17DF14[(k - 1)][0], a1x->scroll_26.x1_26_0 + 20 + 3 * v34, v33, v32);
				}
			}
		}
		if (menuState_17DE38.savedGameIndex_17DF04)
		{
			GetFont_6FC50(1);
			if (menuState_17DE38.keyScancode_17DF10 != 1 && menuState_17DE38.keyScancode_17DF10 != 28 && menuState_17DE38.keyScancode_17DF10 != 14
				|| /*(v35 = 43 * (menuState_17DE38.x_WORD_17DF04 - 1),*/ menuState_17DE38.xx_BYTE_17DF14[(menuState_17DE38.savedGameIndex_17DF04 - 1)][42]/*)*/)
			{
				if (menuState_17DE38.lastKeyChar_17DF11)
				{
					if (IsAlphanumericOrSpace_7C200(menuState_17DE38.lastKeyChar_17DF11))
					{
						//v36 = 43 * (menuState_17DE38.x_WORD_17DF04 - 1);
						if (!menuState_17DE38.xx_BYTE_17DF14[(menuState_17DE38.savedGameIndex_17DF04 - 1)][42])
						{
							menuState_17DE38.xx_BYTE_17DF14[(menuState_17DE38.savedGameIndex_17DF04 - 1)][42] = 1;
							sprintf((char*)&menuState_17DE38.xx_BYTE_17DF14[(menuState_17DE38.savedGameIndex_17DF04 - 1)][20], "%c", menuState_17DE38.lastKeyChar_17DF11);
							menuState_17DE38.keyScancode_17DF10 = 0;
							menuState_17DE38.lastKeyChar_17DF11 = 0;
						}
					}
				}
			}
			else
			{
				menuState_17DE38.xx_BYTE_17DF14[(menuState_17DE38.savedGameIndex_17DF04 - 1)][42] = 1;
			}
			v37 = v68;
			v38 = &menuState_17DE38.xx_BYTE_17DF14[(menuState_17DE38.savedGameIndex_17DF04 - 1)][0];
			v39 = v38 + 40;
			v40 = v38 + 20;
			v41 = v61;
			v42 = GetLetterWidth_6FC10();
			EditTextField_7F6A0((signed __int16)(v63 + 3 * v42), v43, v63 + 3 * v42, v41, v40, (int8_t*)v39, v37);
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
char LoadGameDialog_7E820(type_menuButtons_E1F84* a1x)//25f820
{
	char result; // al

	result = LoadGameDialog_780F0(a1x);
	if (result)
		a1x->selected_8 = 0;
	return result;
}

int debugcounter_7D400 = 0;
//----- (0007D400) --------------------------------------------------------
void DrawAnimTextsAndPlaySounds_7D400(__int16 posx, __int16 posy, char a4)//25e400
{
	typeTextBoxtextBoxStr_E24BCx textBoxStr[2];
	Type_MapScreenPortals_E17CC mapPortal;

	debugcounter_7D400++;

	int index2 = -1;
	int index3 = -1;
	int time = j___clock();
	for (int i = 0; mapAnimSprites_E26C8[i].x1_8; i++)
	{
		if (mapScreenPortals_E17CC[24].activated_18 != 1 || mapAnimSprites_E26C8[i].firstFrame_12 != 85 && mapAnimSprites_E26C8[i].firstFrame_12 != 86)
			DrawAnimSprite_81CA0(posx, posy, &mapAnimSprites_E26C8[i]);		
	}
	//VGA_Debug_Blit(640, 480, pdwScreenBuffer_351628);
	for (int i = 0; mapScreenPortals_E17CC[i].viewPortPosX_4; i++)//draw new game flag
	{
		if (mapScreenPortals_E17CC[i].activated_18 == 1)
		{
			index2 = i;
			DrawFrameAnim_7E5A0(posx, posy, &mapScreenPortals_E17CC[i], 37, 43);
		}
		else if (mapScreenPortals_E17CC[i].activated_18 == 2)
		{
			if (a4 == 3 || a4 == 5)
			{
				index3 = i;
				if (mapScreenPortals_E17CC[i].state_19)
				{
					if (mapScreenPortals_E17CC[i].state_19 == 1)
					{
						if (DrawFrameAnim_7E5A0(posx, posy, &mapScreenPortals_E17CC[i], 70, 83))
						{
							mapScreenPortals_E17CC[i].spriteIndex_16 = 33;
							mapScreenPortals_E17CC[i].state_19 = 2;
						}
					}
					else
					{
						DrawFrameAnim_7E5A0(posx, posy, &mapScreenPortals_E17CC[i], 33, 35);
					}
				}
				else
				{
					//Play Level Portal Opening Sound
					PlaySample_8F100(0, 41, 127, 64, 0x64u, 0, IfNotPlayingPlaySample);
					mapScreenPortals_E17CC[i].state_19 = 1;
				}
			}
		}
		if (mapScreenPortals_E17CC[i].activated_18 == 2)
			break;
	}
	for (int i = 0; secretMapScreenPortals_E2970[i].activated_12; i++)
	{
		if (secretMapScreenPortals_E2970[i].activated_12 < 1u)
			continue;
		if (secretMapScreenPortals_E2970[i].activated_12 == 1u)
		{
			memset(&mapPortal, 0, 22);
			mapPortal.time_0 = secretMapScreenPortals_E2970[i].time_0;
			mapPortal.spriteIndex_16 = secretMapScreenPortals_E2970[i].spriteIndex_14;
			mapPortal.portalPosX_12 = secretMapScreenPortals_E2970[i].posX_8;
			mapPortal.portalPosY_14 = secretMapScreenPortals_E2970[i].posY_10;
			DrawFrameAnim_7E5A0(posx, posy, &mapPortal, 305, 311);
			secretMapScreenPortals_E2970[i].time_0 = mapPortal.time_0;
			secretMapScreenPortals_E2970[i].spriteIndex_14 = mapPortal.spriteIndex_16;
			continue;
		}
		if (secretMapScreenPortals_E2970[i].activated_12 == 2 && a4 == 3)
		{
			if (!secretMapScreenPortals_E2970[i].state_16)
			{
				PlaySample_8F100(0, 41, 127, 64, 0x64u, 0, IfNotPlayingPlaySample);
				secretMapScreenPortals_E2970[i].state_16 = 1;
				continue;
			}
			if (secretMapScreenPortals_E2970[i].state_16 != 1)
			{
				memset(&mapPortal, 0, 22);
				mapPortal.time_0 = secretMapScreenPortals_E2970[i].time_0;
				mapPortal.spriteIndex_16 = secretMapScreenPortals_E2970[i].spriteIndex_14;
				mapPortal.portalPosX_12 = secretMapScreenPortals_E2970[i].posX_8;
				mapPortal.portalPosY_14 = secretMapScreenPortals_E2970[i].posY_10;
				DrawFrameAnim_7E5A0(posx, posy, &mapPortal, 270, 272);
				secretMapScreenPortals_E2970[i].time_0 = mapPortal.time_0;
				secretMapScreenPortals_E2970[i].spriteIndex_14 = mapPortal.spriteIndex_16;
				continue;
			}
			memset(&mapPortal, 0, 22);
			mapPortal.time_0 = secretMapScreenPortals_E2970[i].time_0;
			mapPortal.spriteIndex_16 = secretMapScreenPortals_E2970[i].spriteIndex_14;
			mapPortal.portalPosX_12 = secretMapScreenPortals_E2970[i].posX_8;
			mapPortal.portalPosY_14 = secretMapScreenPortals_E2970[i].posY_10;
			if (DrawFrameAnim_7E5A0(posx, posy, &mapPortal, 70, 83))
			{
				secretMapScreenPortals_E2970[i].spriteIndex_14 = 270;
				secretMapScreenPortals_E2970[i].state_16 = 2;
			}
			else
			{
				secretMapScreenPortals_E2970[i].spriteIndex_14 = mapPortal.spriteIndex_16;
			}
			secretMapScreenPortals_E2970[i].time_0 = mapPortal.time_0;
		}
	}
	if (a4 == 3 && gameState_D41A4.showHelp_10 == 1)
	{
		bitmap_pos_struct2_t* tempPosStr1 = menuState_17DE38.fontTab_17DEC0;
		bitmap_pos_struct2_t* tempPosStr2 = menuState_17DE38.fontTabEnd_17DEC4;
		menuState_17DE38.fontTab_17DEC0 = menuState_17DE38.font2Tab_17DEC8;
		menuState_17DE38.fontTabEnd_17DEC4 = menuState_17DE38.font2TabEnd_17DECC;
		bitmap_pos_struct_t* tempSpriteStr = fontSprites_17DEC0;
		fontSprites_17DEC0 = font2Sprites_17DEC8;
		if (mapMenuState_17DB70.portalAnimState_17DB74 == 0)
		{
			if (!mapMenuState_17DB70.portalAnimState_17DB74)
			{
				mapMenuState_17DB70.portalAnimState_17DB74 = (index3 != -1)+1;
				mapMenuState_17DB70.time_17DB70 = j___clock();
			}
		}
		else if (mapMenuState_17DB70.portalAnimState_17DB74 == 1u)
		{
			if (index2 != -1)
			{
				if (3 == mapMenuState_17DB70.mapState_17DB8F)
				{
					memset(textBoxStr, 0, 36);
					textBoxStr[0] = textBoxStr_E24F2[0];
					textBoxStr[0].left_2 = mapScreenPortals_E17CC[index2].portalPosX_12 - 80 - posx;
					textBoxStr[0].top_4 = mapScreenPortals_E17CC[index2].portalPosY_14 - 60 - posy;
					textBoxStr[0].minx_6 = mapScreenPortals_E17CC[index2].portalPosX_12 + 16 - posx;
					textBoxStr[0].miny_8 = mapScreenPortals_E17CC[index2].portalPosY_14 - 60 - posy;
					textBoxStr[0].width_12 = mapScreenPortals_E17CC[index2].portalPosX_12 + 16 - posx;
					textBoxStr[0].maxy_14 = mapScreenPortals_E17CC[index2].portalPosY_14 - 4 - posy;
					textBoxStr[0].textIndex_0 = 464;
					DrawTextBoxWithLine_7E840(textBoxStr, 238, 264);
					if (index3 != -1)
					{
						if ((time - mapMenuState_17DB70.time_17DB70) / 0x64u > 8)
						{
							mapMenuState_17DB70.time_17DB70 = time;
							mapMenuState_17DB70.portalAnimState_17DB74 = 2;
						}
					}
				}
			}
		}
		else if (mapMenuState_17DB70.portalAnimState_17DB74 == 2)
		{
			if (index3 != -1)
			{
				if (mapMenuState_17DB70.mapState_17DB8F == 3)
				{
					memset(textBoxStr, 0, 36);
					textBoxStr[0].left_2 = mapScreenPortals_E17CC[index3].portalPosX_12 - 80 - posx;
					textBoxStr[0].top_4 = mapScreenPortals_E17CC[index3].portalPosY_14 - 60 - posy;
					textBoxStr[0].minx_6 = mapScreenPortals_E17CC[index3].portalPosX_12 + 16 - posx;
					textBoxStr[0].miny_8 = mapScreenPortals_E17CC[index3].portalPosY_14 - 60 - posy;
					textBoxStr[0].width_12 = mapScreenPortals_E17CC[index3].portalPosX_12 + 16 - posx;
					textBoxStr[0].maxy_14 = mapScreenPortals_E17CC[index3].portalPosY_14 - 4 - posy;
					textBoxStr[0].textIndex_0 = 465;
					DrawTextBoxWithLine_7E840(textBoxStr, 238, 264);
					if (index2 != -1)
					{
						if ((time - mapMenuState_17DB70.time_17DB70) / 0x64u > 8)
						{
							mapMenuState_17DB70.time_17DB70 = time;
							mapMenuState_17DB70.portalAnimState_17DB74 = 1;
						}
					}
				}
			}
		}
		menuState_17DE38.fontTab_17DEC0 = tempPosStr1;
		menuState_17DE38.fontTabEnd_17DEC4 = tempPosStr2;
		fontSprites_17DEC0 = tempSpriteStr;
	}
	if (mapMenuState_17DB70.levelChosen_17DB8E)
		return;

	if (mapMenuState_17DB70.selectedLevel_17DB8A != -1)
	{
		int index4 = -1;
		for (int i = 0; mapScreenPortals_E17CC[i].viewPortPosX_4; i++)
		{
			if (mapScreenPortals_E17CC[i].activated_18 == 1)
				index4 = i;
		}		
		for (int i = 0; mapScreenPortals_E17CC[i].viewPortPosX_4;i++)
		{
			if (i == mapMenuState_17DB70.selectedLevel_17DB8A)
			{
				SetAnimationVariables_7DA70(mapScreenPortals_E17CC[i].portalPosX_12, mapScreenPortals_E17CC[i].portalPosY_14, mapScreenPortals_E17CC[index4].portalPosX_12, mapScreenPortals_E17CC[index4].portalPosY_14, posx, posy);
				return;
			}
		}
		for (int i = 0; secretMapScreenPortals_E2970[i].activated_12; i++)
		{
			if (mapMenuState_17DB70.selectedLevel_17DB8A == secretMapScreenPortals_E2970[i].levelNumber_6)
			{
				SetAnimationVariables_7DA70(secretMapScreenPortals_E2970[i].posX_8, secretMapScreenPortals_E2970[i].posY_10, mapScreenPortals_E17CC[secretMapScreenPortals_E2970[i].index_4].portalPosX_12, mapScreenPortals_E17CC[secretMapScreenPortals_E2970[i].index_4].portalPosY_14, posx, posy);
				return;
			}
		}
		return;
	}
	if ((index2 != -1) && (index3 != -1))
	{
		SetAnimationVariables_7DA70(mapScreenPortals_E17CC[index2].portalPosX_12, mapScreenPortals_E17CC[index2].portalPosY_14, mapScreenPortals_E17CC[index3].portalPosX_12, mapScreenPortals_E17CC[index3].portalPosY_14, posx, posy);
		return;
	}
	if ((index2 != -1) && (index3 != -1))
		return;
	if (index2 >= 0) {
		SetAnimationVariables_7DA70(mapScreenPortals_E17CC[index2].portalPosX_12, mapScreenPortals_E17CC[index2].portalPosY_14, mapScreenPortals_E17CC[23].portalPosX_12, mapScreenPortals_E17CC[23].portalPosY_14, posx, posy);
	}
}

//----- (0007EAE0) --------------------------------------------------------
int NewGameDraw_7EAE0(int16_t* posx, int16_t* posy, __int16* portPosX, __int16* portPosY, int8_t* a5, type_animStruct* animStruct)//25fae0
{
	type_menuButtons_E1F84 selectVar;
	int index2;
	int index3 = -1;
	int result2;
	int result = 0;
	int index4;

	switch (*a5)
	{
	case 1:
		index2 = -1;
		for (int i = 0; mapScreenPortals_E17CC[i].viewPortPosX_4; i++)
		{
			if (mapScreenPortals_E17CC[i].activated_18 == 1)
			{
				*posx = mapScreenPortals_E17CC[i].viewPortPosX_4;
				*posy = mapScreenPortals_E17CC[i].viewPortPosY_6;
				index3 = i;
				index2 = i;
			}
		}
		if ((index2 != -1) && mapMenuState_17DB70.selectedLevel_17DB8A != -1 && index3 != mapMenuState_17DB70.selectedLevel_17DB8A)
		{
			for (int i = 0; mapScreenPortals_E17CC[i].viewPortPosX_4; i++)
			{
				if (i == mapMenuState_17DB70.selectedLevel_17DB8A)
				{
					*posx = mapScreenPortals_E17CC[i].viewPortPosX_4;
					*posy = mapScreenPortals_E17CC[i].viewPortPosY_6;
					*a5 = 3;
					break;
				}
			}
			if (*a5 != 3)
			{
				for (int ii = 0; secretMapScreenPortals_E2970[ii].activated_12; ii++)
				{
					if (mapMenuState_17DB70.selectedLevel_17DB8A == secretMapScreenPortals_E2970[ii].levelNumber_6)
					{
						*posx = mapScreenPortals_E17CC[secretMapScreenPortals_E2970[ii].index_4].viewPortPosX_4;
						*posy = mapScreenPortals_E17CC[secretMapScreenPortals_E2970[ii].index_4].viewPortPosY_6;
						*a5 = 3;
						break;
					}
				}
			}
		}
		else
		{
			mapMenuState_17DB70.selectedLevel_17DB8A = -1;
		}
		memset(&levelDescription_17DE28, 0, 13);
		if (mapMenuState_17DB70.selectedLevel_17DB8A == -1)
		{
			for (int jx = 0; mapScreenPortals_E17CC[jx].viewPortPosX_4 != 0; jx++)
			{
				if (mapScreenPortals_E17CC[jx].activated_18 == 2)
				{
					*portPosX = mapScreenPortals_E17CC[jx].viewPortPosX_4;
					*portPosY = mapScreenPortals_E17CC[jx].viewPortPosY_6;
					if (jx)
					{
						*a5 = 2;
						CreateAnimObject_7E8D0(animStruct, *portPosX, *portPosY, *posx, *posy, 4, 4);
					}
					else
					{
						*posx = mapScreenPortals_E17CC[jx].viewPortPosX_4;
						*posy = mapScreenPortals_E17CC[jx].viewPortPosY_6;
						if (*posy > 480) *posy = 480;
						if (*posx > 640) *posx = 640;
						*a5 = 3;
						if (jx < 24)
						{
							levelDescription_17DE28.posX_17DE30 = *posx;
							levelDescription_17DE28.posY_17DE32 = *posy;
							levelDescription_17DE28.DisplayLevelDescriptionText_17DE34 = ((gameState_D41A4.OptionsSettingFlag_24 & SPEECH_ENABLED) != 0) + 1;
							if (autoShowObjectivesForForeignLanguages &&
								gameState_D41A4.SelectedLangIndex != 2 &&
								levelDescription_17DE28.DisplayLevelDescriptionText_17DE34 == 2)
								levelDescription_17DE28.DisplayLevelDescriptionText_17DE34 -= 1;
							levelDescription_17DE28.time_17DE28 = j___clock();
						}
					}
					break;
				}
			}
			if (*a5 != 1 || mapMenuState_17DB70.levelChosen_17DB8E)
				MapMenuPortalsDraw_81760();
			else
			{
				*a5 = 3;
				MapMenuPortalsDraw_81760();
			}
		}
		else
		{
			for (int jx = 0; mapScreenPortals_E17CC[jx].viewPortPosX_4 != 0; jx++)
			{
				if (mapScreenPortals_E17CC[jx].activated_18 == 2)
				{
					if (jx < 24)
					{
						levelDescription_17DE28.posX_17DE30 = *posx;
						levelDescription_17DE28.posY_17DE32 = *posy;
						levelDescription_17DE28.DisplayLevelDescriptionText_17DE34 = ((gameState_D41A4.OptionsSettingFlag_24 & SPEECH_ENABLED) != 0) + 1;
						if (autoShowObjectivesForForeignLanguages &&
							gameState_D41A4.SelectedLangIndex != 2 &&
							levelDescription_17DE28.DisplayLevelDescriptionText_17DE34 == 2)
							levelDescription_17DE28.DisplayLevelDescriptionText_17DE34 -= 1;
						levelDescription_17DE28.time_17DE28 = j___clock();
					}
					break;
				}
			}
			*a5 = 3;
			MapMenuPortalsDraw_81760();
		}
		break;
	case 2:
		if (MoveAnimObject_7E9D0(posx, posy, animStruct))
		{
			*a5 = 3;
			memset(&levelDescription_17DE28, 0, 13);
			levelDescription_17DE28.posX_17DE30 = *posx;
			levelDescription_17DE28.posY_17DE32 = *posy;
			levelDescription_17DE28.DisplayLevelDescriptionText_17DE34 = ((gameState_D41A4.OptionsSettingFlag_24 & SPEECH_ENABLED) != 0) + 1;
			if (autoShowObjectivesForForeignLanguages &&
				gameState_D41A4.SelectedLangIndex != 2 &&
				levelDescription_17DE28.DisplayLevelDescriptionText_17DE34 == 2)
				levelDescription_17DE28.DisplayLevelDescriptionText_17DE34 -= 1;
			levelDescription_17DE28.time_17DE28 = j___clock();
		}
		break;
	case 3:
		if (!mapMenuState_17DB70.levelChosen_17DB8E)
		{
			mapNotMoving_E29D6 = true;
			if (menuState_17DE38.mouseX_17DEE4)
			{
				if (menuState_17DE38.mouseX_17DEE4 >= MOUSE_MAX_X)
				{
					*posx += mapMenuState_17DB70.shiftStep_17DB82;
					mapNotMoving_E29D6 = false;
				}
			}
			else
			{
				*posx -= mapMenuState_17DB70.shiftStep_17DB82;
				mapNotMoving_E29D6 = false;
			}

			if (menuState_17DE38.mouseY_17DEE6)
			{
				if (menuState_17DE38.mouseY_17DEE6 >= MOUSE_MAX_Y)
				{
					*posy += mapMenuState_17DB70.shiftStep_17DB82;
					mapNotMoving_E29D6 = false;
				}
			}
			else
			{
				*posy -= mapMenuState_17DB70.shiftStep_17DB82;
				mapNotMoving_E29D6 = false;
			}
			*posx = std::clamp(*posx, MOUSE_MIN, MOUSE_MAX_X);
			*posy = std::clamp(*posy, MOUSE_MIN, MOUSE_MAX_Y);
			if (mapNotMoving_E29D6)
			{
				mapMenuState_17DB70.shiftStep_17DB82 = 0;
			}
			else
			{
				mapMenuState_17DB70.shiftStep_17DB82 += 4;
				if (mapMenuState_17DB70.shiftStep_17DB82 > 24)
					mapMenuState_17DB70.shiftStep_17DB82 = 24;
			}
		}
		break;
	case 4:
		if (mapMenuState_17DB70.selectedLevel_17DB8A > 24)
		{
			Type_SecretMapScreenPortals_E2970* tempPortal = GetSecretAndActivedPortal2_824E0(mapMenuState_17DB70.selectedLevel_17DB8A);
			if (tempPortal)
			{
				*posx = mapScreenPortals_E17CC[tempPortal->index_4].viewPortPosX_4;
				*posy = mapScreenPortals_E17CC[tempPortal->index_4].viewPortPosY_6;
			}
		}
		else
		{
			*posx = mapScreenPortals_E17CC[mapMenuState_17DB70.selectedLevel_17DB8A].viewPortPosX_4;
			*posy = mapScreenPortals_E17CC[mapMenuState_17DB70.selectedLevel_17DB8A].viewPortPosY_6;
		}

		if (menuState_17DE38.keyScancode_17DF10 || menuState_17DE38.mouseButtons_17DEEE)
		{
			*a5 = 1;
			menuState_17DE38.mouseButtons_17DEEE = 0;
			menuState_17DE38.lastKeyChar_17DF11 = 0;
			menuState_17DE38.keyScancode_17DF10 = 0;
		}
		index4 = -1;
		for (int i = 0; mapScreenPortals_E17CC[i].viewPortPosX_4;i++)
		{
			if (mapScreenPortals_E17CC[i].activated_18 == 2)
			{
				index4 = i;
				break;
			}
		}
		int tempPosX;
		if ((index4 != -1) && (index4 != 0))
		{
			mapScreenPortals_E17CC[index4 - 1].activated_18 = 2;
			tempPosX = mapScreenPortals_E17CC[index4].viewPortPosX_4;
			mapScreenPortals_E17CC[index4].viewPortPosX_4 = 0;
		}
		MapMenuPortalsDraw_81760();
		if ((index4 != -1) && (index4 != 0))
		{
			mapScreenPortals_E17CC[index4 - 1].activated_18 = 1;
			mapScreenPortals_E17CC[index4].viewPortPosX_4 = tempPosX;
		}
		break;
	case 5:
		if (menuState_17DE38.keyScancode_17DF10 || menuState_17DE38.mouseButtons_17DEEE)
		{
			menuState_17DE38.mouseButtons_17DEEE = 0;
			*a5 = 3;
			menuState_17DE38.lastKeyChar_17DF11 = 0;
			menuState_17DE38.keyScancode_17DF10 = 0;
		}
		break;
	default:
		break;
	}
	//if (*a5 != 1 || mapMenuState_17DB70.x_BYTE_17DB8E)
	{
		if (mapNotMoving_E29D6)
		{
			DrawNetGameMapBackground_85C8B(menuState_17DE38.gameWorldMap_17DE64, pdwScreenBuffer_351628, *posx, *posy, 160, 480);
			DrawAnimTextsAndPlaySounds_7D400(*posx, *posy, *a5);
			DrawAndSoundDragonAndFire_81EE0(*posx, *posy);
			if (mapMenuState_17DB70.levelChosen_17DB8E)
			{
				result = MoveGraphicsAndPlaySounds_80D40(*posx, *posy, mapMenuState_17DB70.portalPosX_17DB84, mapMenuState_17DB70.portalPosY_17DB86, mapMenuState_17DB70.portalActivated_17DB88);
				if (result)
				{
					mapMenuState_17DB70.levelChosen_17DB8E = 0;
				}
			}
			DrawRoundFrame_85CC3((uint16_t*)menuState_17DE38.borderBitmap_17DE5C);
			if (*a5 == 4)
			{
				DrawEndGameTable_82C20(mapMenuState_17DB70.selectedLevel_17DB8A);
			}
			else if (*a5 == 5)
			{
				DrawEndGameTable_82C20(mapMenuState_17DB70.targetLevel_17DB8C);
			}
		}
		else
		{
			DrawNetGameMapBackground_85C8B(menuState_17DE38.gameWorldMap_17DE64, pdwScreenBuffer_351628, *posx, *posy, 160, 480);
			DrawAnimTextsAndPlaySounds_7D400(*posx, *posy, *a5);
			DrawAndSoundDragonAndFire_81EE0(*posx, *posy);
			if (mapMenuState_17DB70.levelChosen_17DB8E)
			{
				result = MoveGraphicsAndPlaySounds_80D40(*posx, *posy, mapMenuState_17DB70.portalPosX_17DB84, mapMenuState_17DB70.portalPosY_17DB86, mapMenuState_17DB70.portalActivated_17DB88);
				if (result)
					mapMenuState_17DB70.levelChosen_17DB8E = 0;
			}
			DrawRoundFrame_85CC3((unsigned __int16*)menuState_17DE38.borderBitmap_17DE5C);
			mapNotMoving_E29D6 = true;
			if (*a5 == 4)
			{
				DrawEndGameTable_82C20(mapMenuState_17DB70.selectedLevel_17DB8A);
			}
			else if (*a5 == 5)
			{
				DrawEndGameTable_82C20(mapMenuState_17DB70.targetLevel_17DB8C);
			}
		}
		result2 = DrawBitmapAndPlaySound_7E320();
		if (!mapMenuState_17DB70.levelChosen_17DB8E && !result && !result2)
		{
			if (*a5 == 3)
			{
				if (menuState_17DE38.lastKeyChar_17DF11 == 111 || menuState_17DE38.lastKeyChar_17DF11 == 79)
				{
					if (levelDescription_17DE28.DisplayLevelDescriptionText_17DE34 == 1)
					{
						levelDescription_17DE28.time_17DE28 = levelDescription_17DE28.time2_17DE2C;
						levelDescription_17DE28.DisplayLevelDescriptionText_17DE34 = 2;
						IsPlayingCDTrack_17E09D = 1;
					}
					else if (levelDescription_17DE28.DisplayLevelDescriptionText_17DE34 == 2)
					{
						int time = j___clock();
						levelDescription_17DE28.time2_17DE2C = time;
						levelDescription_17DE28.DisplayLevelDescriptionText_17DE34 = 1;
						levelDescription_17DE28.time_17DE28 = time;
						IsPlayingCDTrack_17E09D = 0;
					}
				}
				if (levelDescription_17DE28.DisplayLevelDescriptionText_17DE34 == 1)
				{
					int time = j___clock();
					levelDescription_17DE28.time2_17DE2C = time;
					if ((time - levelDescription_17DE28.time_17DE28) / 0x64u > 0xF)
					{
						levelDescription_17DE28.time_17DE28 = time;
						levelDescription_17DE28.DisplayLevelDescriptionText_17DE34 = 2;
					}
					int textIndex = (levelDescription_17DE28.posY_17DE32 < MOUSE_MAX_Y) ? 280 : 60;
					PresentLevelDescription_80C30(130, textIndex, 380);
				}
				else if (levelDescription_17DE28.DisplayLevelDescriptionText_17DE34 <= 3)
				{
					//adress 2602a7
					PresentLevelDescription_80C30(0, 0, 0);
				}
			}
		}
		if (result2 == 2 && !result && !mapMenuState_17DB70.levelChosen_17DB8E)
			result = result2;

		if (!result2 && !result)
		{
			if (*a5 == 3 && !mapMenuState_17DB70.levelChosen_17DB8E)
			{
				if (menuState_17DE38.mouseButtons_17DEEE & 1)
				{
					bool found = false;
					for (int i=0;mapScreenPortals_E17CC[i].viewPortPosX_4;i++)
					{
						selectVar.xmin_10 = mapScreenPortals_E17CC[i].portalPosX_12 - *posx;
						selectVar.ymin_12 = mapScreenPortals_E17CC[i].portalPosY_14 - *posy;
						selectVar.sizex_14 = mapScreenPortals_E17CC[i].sizeX_8;
						selectVar.sizey_16 = mapScreenPortals_E17CC[i].sizeY_10;
						if (InRegion_7B200(&selectVar, menuState_17DE38.mouseX_17DEE4, menuState_17DE38.mouseY_17DEE6))
						{
							mapMenuState_17DB70.portalPosX_17DB84 = mapScreenPortals_E17CC[i].portalPosX_12;
							mapMenuState_17DB70.portalPosY_17DB86 = mapScreenPortals_E17CC[i].portalPosY_14;
							mapMenuState_17DB70.portalActivated_17DB88 = mapScreenPortals_E17CC[i].activated_18;
							MoveGraphicsAndPlaySounds_80D40(*posx, *posy, mapScreenPortals_E17CC[i].portalPosX_12, mapScreenPortals_E17CC[i].portalPosY_14, mapScreenPortals_E17CC[i].activated_18);
							mapMenuState_17DB70.levelChosen_17DB8E = 1;
							gameState_D41A4.levelnumber_43w = i;
							if (mapScreenPortals_E17CC[i].activated_18 == 1)
								gameState_D41A4.setting_38545 |= 4u;
							Type_SecretMapScreenPortals_E2970* tempPortal = GetSecretAndActivedPortal_824B0(gameState_D41A4.levelnumber_43w);
							if (tempPortal && tempPortal->activated_12 == 2)
								gameState_D41A4.setting_38545 |= 0x10u;
							if (gameState_D41A4.levelnumber_43w == 24)
								gameState_D41A4.setting_38545 |= 0x20u;
							found = true;
							break;
						}
						if (mapScreenPortals_E17CC[i].activated_18 == 2) break;
					}
					if (!found && !mapMenuState_17DB70.levelChosen_17DB8E)
					{
						for (int i=0;secretMapScreenPortals_E2970[i].posX_8;i++)
						{
							if (secretMapScreenPortals_E2970[i].activated_12 != 3)
							{
								selectVar.xmin_10 = secretMapScreenPortals_E2970[i].posX_8 - *posx;
								selectVar.ymin_12 = secretMapScreenPortals_E2970[i].posY_10 - *posy;
								selectVar.sizex_14 = 40;
								selectVar.sizey_16 = 40;
								if (InRegion_7B200(&selectVar, menuState_17DE38.mouseX_17DEE4, menuState_17DE38.mouseY_17DEE6))
								{
									if (secretMapScreenPortals_E2970[i].activated_12 == 1)
										gameState_D41A4.setting_38545 |= 4u;
									mapMenuState_17DB70.portalPosX_17DB84 = secretMapScreenPortals_E2970[i].posX_8;
									mapMenuState_17DB70.portalPosY_17DB86 = secretMapScreenPortals_E2970[i].posY_10;
									mapMenuState_17DB70.portalActivated_17DB88 = 1;
									MoveGraphicsAndPlaySounds_80D40(*posx, *posy, secretMapScreenPortals_E2970[i].posX_8, secretMapScreenPortals_E2970[i].posY_10, 1);
									gameState_D41A4.levelnumber_43w = secretMapScreenPortals_E2970[i].levelNumber_6;
									mapMenuState_17DB70.levelChosen_17DB8E = 1;
									break;
								}
							}
						}
					}
				}
				else if (menuState_17DE38.mouseButtons_17DEEE & 2)
				{
					bool found_r = false;
					for (int i=0; mapScreenPortals_E17CC[i].viewPortPosX_4; i++)
					{
						selectVar.xmin_10 = mapScreenPortals_E17CC[i].portalPosX_12 - *posx;
						selectVar.ymin_12 = mapScreenPortals_E17CC[i].portalPosY_14 - *posy;
						selectVar.sizex_14 = mapScreenPortals_E17CC[i].sizeX_8;
						selectVar.sizey_16 = mapScreenPortals_E17CC[i].sizeY_10;
						if (InRegion_7B200(&selectVar, menuState_17DE38.mouseX_17DEE4, menuState_17DE38.mouseY_17DEE6))
						{
							if (mapScreenPortals_E17CC[i].activated_18 == 1)
							{
								mapMenuState_17DB70.targetLevel_17DB8C = i;
								*a5 = 5;
								menuState_17DE38.mouseButtons_17DEEE = 0;
							}
							found_r = true;
							break;
						}
					}
					if (!found_r && *a5 != 5)
					{
						for (int kk = 0; secretMapScreenPortals_E2970[kk].posX_8; kk++)
						{
							if (secretMapScreenPortals_E2970[kk].activated_12 != 3)
							{
								selectVar.xmin_10 = secretMapScreenPortals_E2970[kk].posX_8 - *posx;
								selectVar.ymin_12 = secretMapScreenPortals_E2970[kk].posY_10 - *posy;
								selectVar.sizex_14 = 40;
								selectVar.sizey_16 = 40;
								if (InRegion_7B200(&selectVar, menuState_17DE38.mouseX_17DEE4, menuState_17DE38.mouseY_17DEE6))
								{
									if (secretMapScreenPortals_E2970[kk].activated_12 == 1)
									{
										*a5 = 5;
										mapMenuState_17DB70.targetLevel_17DB8C = secretMapScreenPortals_E2970[kk].levelNumber_6;
									}
									break;
								}
							}
						}
					}
				}
			}
			if (!result && menuState_17DE38.keyScancode_17DF10 == 1)
				result = 2;
		}
	}
	return result;
}

//----- (0007F7D0) --------------------------------------------------------
int LoadLanguageFile(bitmap_pos_struct2_t** a1x, bitmap_pos_struct2_t** a2x, uint8_t* a3, char* langfilename, bitmap_pos_struct_t* a3dattabindex)//2607d0
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
	std::string languageFilePath = GetSubDirectoryFile(cdFolder.c_str(), "LANGUAGE", langfilename);
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
			if (langLoaded_E29E0 || langBuffer_D41BC)//[2b39e0]00 || [2a51bc]00
				FreeMem_83E80((uint8_t*)langBuffer_D41BC);
			//v7 = langfilelenght - 4785;
			langBuffer_D41BC = (char*)Malloc_83CD0(langfilelenght - 4785);
			//langBuffer_D41BC = (uint8_t*)v8;
			if (langBuffer_D41BC)
			{
				langLoaded_E29E0 = 1;
				DataFileIO::Read(langfile, (uint8_t*)langBuffer_D41BC, langfilelenght - 4785);
			}
			DataFileIO::Close(langfile);
			break;
		}
		//v9 = i;
		char updatedLanguagePath[MAX_PATH];
		sprintf(updatedLanguagePath, "LANGUAGE/%s", langfilename);
		languageFilePath = std::string(updatedLanguagePath);

	}
	*a1x = (bitmap_pos_struct2_t*)(a3 + 4773);//35513c
	//v10 = videoMode_180660;
	*a2x = (bitmap_pos_struct2_t*)(a3 + 4785);
	//bitmap_pos_struct_t a3dattab[1000];
	if (videoMode_180660 & 1)
		CreateIndexDatTabDoubled_98709(*a1x, (bitmap_pos_struct2_t*)(a3 + 4785), a3, a3dattabindex);
	else
		CreateIndexDatTab_9874D(*a1x, (bitmap_pos_struct2_t*)(a3 + 4785), a3, a3dattabindex);
	CopySentence_5B870(langBuffer_D41BC, langStrings_E9C4C, 471);//fix it //2a51bc //Exit Game
	return atoi(langfilename + 1);
}
// 8E3D5: using guessed type x_DWORD sprintf(x_DWORD, const char *, ...);
// 988DA: using guessed type x_DWORD filelength(x_DWORD);
// 98F9E: using guessed type x_DWORD unknown_libname_1_atoi(x_DWORD);
// D41A4: using guessed type int x_DWORD_D41A4;
// D41BC: using guessed type int langBuffer_D41BC;
// E29E0: using guessed type char langLoaded_E29E0;
// 180660: using guessed type __int16 videoMode_180660;

//----- (0007F960) --------------------------------------------------------
int LoadLanguageStrings_7F960(bitmap_pos_struct2_t* a1x, bitmap_pos_struct2_t* a2x, uint8_t* a3, char* langcountstring, bitmap_pos_struct_t* a3dattabindex)
//int sub_7F7D0(uint8_t** a1, uint8_t** a2, uint8_t* a3, char* langfilename, bitmap_pos_struct_t* a3dattabindex)//2607d0
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

	std::string languagePath = GetSubDirectoryFile(cdFolder.c_str(), "LANGUAGE", langcountstring);
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
			if (langLoaded_E29E0 || langBuffer_D41BC)
				FreeMem_83E80((uint8_t*)langBuffer_D41BC);
			v8 = v13 - 4785;
			//v9 = (uint8_t*)sub_83CD0_malloc2(v13 - 4785);
			langBuffer_D41BC = (char*)Malloc_83CD0(v13 - 4785);
			if (langBuffer_D41BC)
			{
				langLoaded_E29E0 = 1;
				DataFileIO::Read(langfile, (uint8_t*)langBuffer_D41BC, v8);
			}
			DataFileIO::Close(langfile);
			break;
		}
		v10 = i;
		languagePath = GetSubDirectoryFile(cdFolder.c_str(), "LANGUAGE", langcountstring);
	}
	if (videoMode_180660 & 1)
		CreateIndexDatTabDoubled_98709(a1x, a2x, a3, a3dattabindex);
	else
		CreateIndexDatTab_9874D(a1x, a2x, a3, a3dattabindex);
	CopySentence_5B870(langBuffer_D41BC, langStrings_E9C4C, 471);//Exit Game
	return langcount;
}
// 8E3D5: using guessed type x_DWORD sprintf(x_DWORD, const char *, ...);
// 988DA: using guessed type x_DWORD filelength(x_DWORD);
// 98F9E: using guessed type x_DWORD unknown_libname_1_atoi(x_DWORD);
// D41A4: using guessed type int x_DWORD_D41A4;
// D41BC: using guessed type int langBuffer_D41BC;
// E29E0: using guessed type char langLoaded_E29E0;
// 180660: using guessed type __int16 videoMode_180660;

//----- (00080C30) --------------------------------------------------------
void PresentLevelDescription_80C30(__int16 posX, __int16 posY, __int16 addWidth)//261c30
{
	int levelIdx_v3 = -1;
	for (int i=0; mapScreenPortals_E17CC[i].viewPortPosX_4;i++)
	{
		if (mapScreenPortals_E17CC[i].activated_18 == 2)
		{
			levelIdx_v3 = i;
			break;
		}
	}
	if (posY + posX > 0)
	{
		for (int ii = 0; secretMapScreenPortals_E2970[ii].activated_12; ii++)
		{
			if (secretMapScreenPortals_E2970[ii].activated_12 != 3 && levelIdx_v3 == secretMapScreenPortals_E2970[ii].index_4)
			{
				levelDescription_17DE28.DisplayLevelDescriptionText_17DE34 = 3;
				return;
			}
		}
		GetFont_6FC50(1);
		uint8_t colorIndex = GetPaletteIndex_5BE80(menuState_17DE38.palette_17DE38x, 0x3Fu, 0x3Fu, 0x3Fu);
		DrawTextWithBoarder_7FCB0(langStrings_E9C4C[23 + levelIdx_v3], posX + 4 * GetLetterWidth_6FC10(), posX + addWidth - 3 * GetLetterWidth_6FC10(), posY, 5, colorIndex, 1);
		//"You must explore the outer Netherworlds while you learn its magic. Your first destination is the ancient city of Jahwl."+
	}
	if (levelDescription_17DE28.DisplayLevelDescriptionText_17DE34 != 3 && gameState_D41A4.OptionsSettingFlag_24 & 0x40 && !IsPlayingCDTrack_17E09D)
	{
		IsPlayingCDTrack_17E09D = 1;
		if ((signed __int16)levelIdx_v3 != -1)
			PlayCDTrackSegmentNumber_86EB0(levelIdx_v3, 0, 0);
	}
}

//----- (00080D40) --------------------------------------------------------
bool MoveGraphicsAndPlaySounds_80D40(__int16 a2, __int16 a3, __int16 a4, __int16 a5, char a6)//261d40
{
	int16_t a1 = 1;
	bool animBegin = true;
	int v7x;
	char v8; // dl
	int v10; // edx
	int v11; // ebx
	__int16 v12; // ax
	int v13; // edx
	int v14; // edx
	//__int16 v15; // si
	//__int16 v16; // ax
	char v17; // dh
	signed __int16 v18; // cx
	int v20; // edx
	axis_3d axis2; // [esp+0h] [ebp-10h]
	axis_3d axis1; // [esp+8h] [ebp-8h]

	if (menuState_17DE38.mapAnimState_17E09C >= 1u)
	{
		if (menuState_17DE38.mapAnimState_17E09C > 2u)
			return animBegin;
		for (int i = 0; i < 3; i++)
		{			
			animBegin = MoveAnimObject_7E9D0(&menuState_17DE38.x_17E06C, &menuState_17DE38.y_17E06E, &menuState_17DE38.mapAnimObject_17E084x);
		}
		axis1.x = menuState_17DE38.x2_17E074;
		axis1.y = menuState_17DE38.y2_17E076;
		axis2.x = menuState_17DE38.x_17E06C;
		axis2.y = menuState_17DE38.y_17E06E;
		__int16 tan2 = Maths::GetYawAngle_581E0(&axis1, &axis2);
		bool should_update_animation = false;
		if (tan2 > 1024 && tan2 < 1536)
		{
			if (menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 != 21)
			{
				menuState_17DE38.mapAnimation_17E078x.spriteIndex_8 = 21;
				menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 = 21;
				menuState_17DE38.mapAnimation_17E078x.lastSpriteIndex_11 = 24;
			}
		}
		else if (tan2 <= 1536 || tan2 >= 2048)
		{
			if ((tan2 > 2048 || tan2 > 0) && tan2 < 512)
			{
				if (menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 != 5)
				{
					menuState_17DE38.mapAnimation_17E078x.spriteIndex_8 = 5;
					menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 = 5;
					menuState_17DE38.mapAnimation_17E078x.lastSpriteIndex_11 = 8;
				}
			}
			else if (tan2 <= 512 || tan2 >= 1024)
			{
				switch (tan2)
				{
				case 2048:
				case 0:
					if (menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 != 17)
					{
						menuState_17DE38.mapAnimation_17E078x.spriteIndex_8 = 17;
						menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 = 17;
						menuState_17DE38.mapAnimation_17E078x.lastSpriteIndex_11 = 20;
					}
					break;
				case 512:
					if (menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 != 9)
					{
						menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 = 9;
						v17 = 12;
						menuState_17DE38.mapAnimation_17E078x.spriteIndex_8 = 9;
						menuState_17DE38.mapAnimation_17E078x.lastSpriteIndex_11 = v17;
					}
					break;
				case 1024:
					if (menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 != 1)
					{
						menuState_17DE38.mapAnimation_17E078x.spriteIndex_8 = 1;
						menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 = 1;
						menuState_17DE38.mapAnimation_17E078x.lastSpriteIndex_11 = 4;
					}
					break;
				default:
					if (tan2 == 1536 && menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 != 25)
					{
						v18 = 25;
						menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 = 25;
						v17 = 28;
						menuState_17DE38.mapAnimation_17E078x.spriteIndex_8 = v18;
						menuState_17DE38.mapAnimation_17E078x.lastSpriteIndex_11 = v17;
					}
					break;
				}
			}
			else
			{
				if (menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 != 13)
				{
					v18 = 13;
					menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 = 13;
					v17 = 16;
					menuState_17DE38.mapAnimation_17E078x.spriteIndex_8 = v18;
					menuState_17DE38.mapAnimation_17E078x.lastSpriteIndex_11 = v17;
				}
			}
		}
		else
		{
			if (menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 != 29)
			{
				menuState_17DE38.mapAnimation_17E078x.firstSpriteIndex_10 = 29;
				v17 = 32;
				menuState_17DE38.mapAnimation_17E078x.spriteIndex_8 = 29;
				menuState_17DE38.mapAnimation_17E078x.lastSpriteIndex_11 = v17;
			}
		}
		menuState_17DE38.mapAnimation_17E078x.posX_4 = menuState_17DE38.x_17E06C;
		menuState_17DE38.mapAnimation_17E078x.posY_6 = menuState_17DE38.y_17E06E;
		if (a6 != 1
			&& menuState_17DE38.mapAnimState_17E09C != 2
			&& !animBegin
			&& (abs(menuState_17DE38.y_17E06E - menuState_17DE38.lastY_17E072) > 8 || abs(menuState_17DE38.x_17E06C - menuState_17DE38.lastX_17E070) > 8))
		{
			if (mapMenuState_17DB70.selectedLevel_17DB8A == -1)
				DrawMapObject_812D0(menuState_17DE38.mapAnimation_17E078x.posX_4, menuState_17DE38.mapAnimation_17E078x.posY_6);
			menuState_17DE38.lastX_17E070 = menuState_17DE38.x_17E06C;
			menuState_17DE38.lastY_17E072 = menuState_17DE38.y_17E06E;
		}
		v20 = menuState_17DE38.x_17E06C - menuSprites_17DED4[menuState_17DE38.mapAnimation_17E078x.spriteIndex_8].width_4 / 2 - a2;
		MoveAnimIndex_81260(
			&menuState_17DE38.mapAnimation_17E078x,
			menuState_17DE38.x_17E06C - menuSprites_17DED4[menuState_17DE38.mapAnimation_17E078x.spriteIndex_8].width_4 / 2 - a2,
			menuState_17DE38.y_17E06E - menuSprites_17DED4[menuState_17DE38.mapAnimation_17E078x.spriteIndex_8].height_5 / 2 - a3);
		if (animBegin)
			menuState_17DE38.mapAnimState_17E09C = 3;
		return animBegin;
	}

	if (!menuState_17DE38.mapAnimState_17E09C)
	{
		if (mapMenuState_17DB70.selectedLevel_17DB8A == -1)
		{
			v7x = 0;
			a1 = 0;
			menuState_17DE38.x_17E06C = 0;
			menuState_17DE38.y_17E06E = 0;
			while (mapScreenPortals_E17CC[v7x].viewPortPosX_4)
			{
				v8 = mapScreenPortals_E17CC[v7x].activated_18;
				if (v8 == 1)
				{
					menuState_17DE38.x_17E06C = mapScreenPortals_E17CC[v7x].portalPosX_12;
					menuState_17DE38.y_17E06E = mapScreenPortals_E17CC[v7x].portalPosY_14;
				}
				else if (v8 == 2 && !a1)
				{
					return 1;
				}
				v7x++;
				a1++;
			}
		}
		menuState_17DE38.x2_17E074 = a4;
		v10 = menuSprites_17DED4[37].width_4;
		menuState_17DE38.y2_17E076 = a5;
		v11 = v10 / 2;
		v12 = v10 / 2 + a4;
		v13 = menuSprites_17DED4[37].height_5;
		menuState_17DE38.x2_17E074 = v12;
		v14 = v13 / 2;
		menuState_17DE38.y2_17E076 = v14 + a5;
		if (mapMenuState_17DB70.selectedLevel_17DB8A == -1)
		{
			menuState_17DE38.x_17E06C += v11;
			menuState_17DE38.y_17E06E += v14;
		}
		CreateAnimObject_7E8D0(&menuState_17DE38.mapAnimObject_17E084x, menuState_17DE38.x2_17E074, menuState_17DE38.y2_17E076, menuState_17DE38.x_17E06C, menuState_17DE38.y_17E06E, 2, 2);
		menuState_17DE38.mapAnimState_17E09C = (a1 == 0) + 1;
		menuState_17DE38.lastX_17E070 = menuState_17DE38.x_17E06C;
		menuState_17DE38.lastY_17E072 = menuState_17DE38.y_17E06E;
		animBegin = false;
		if (abs(menuState_17DE38.x2_17E074 - menuState_17DE38.x_17E06C) > 5 && abs(menuState_17DE38.y2_17E076 - menuState_17DE38.y_17E06E) > 6)
		{
			PlaySample_8F100(0, 19, (unsigned __int8)menuSoundVolume_E1324, 64, 0x64u, 0, 2u);
			return 0;
		}
	}
	return animBegin;
}
//----- (00081760) --------------------------------------------------------
void MapMenuPortalsDraw_81760()//262760
{
	uint8_t* temp_screen_buffer;
	type_animStruct animStruct;
	bool inArea = false;
	bool firstPortal = false;
	if (mapScreenPortals_E17CC[24].activated_18 == 1)//end game vulcan
	{
		temp_screen_buffer = pdwScreenBuffer_351628;
		pdwScreenBuffer_351628 = menuState_17DE38.gameWorldMap_17DE64;
		__int16 temoScreenHeight = screenHeight_180624;
		clipHeight_180644 = 960;
		screenHeight_180624 = 960;
		__int16 temoScreenWidth = screenWidth_18062C;
		clipWidth_180648 = 1280;
		screenWidth_18062C = 1280;
		DrawBitmap640_7C120(518, 17, menuSprites_17DED4[285]);
		DrawBitmap640_7C120(583, 17, menuSprites_17DED4[286]);
		DrawBitmap640_7C120(657, 17, menuSprites_17DED4[287]);
		DrawBitmap640_7C120(696, 17, menuSprites_17DED4[288]);
		DrawBitmap640_7C120(518, 88, menuSprites_17DED4[289]);
		DrawBitmap640_7C120(574, 88, menuSprites_17DED4[290]);
		DrawBitmap640_7C120(657, 88, menuSprites_17DED4[291]);
		DrawBitmap640_7C120(706, 88, menuSprites_17DED4[292]);
		DrawBitmap640_7C120(518, 156, menuSprites_17DED4[293]);
		DrawBitmap640_7C120(582, 156, menuSprites_17DED4[294]);
		DrawBitmap640_7C120(657, 156, menuSprites_17DED4[295]);
		DrawBitmap640_7C120(703, 156, menuSprites_17DED4[296]);
		screenWidth_18062C = temoScreenWidth;
		screenHeight_180624 = temoScreenHeight;
		clipWidth_180648 = temoScreenWidth;
		clipHeight_180644 = temoScreenHeight;
		pdwScreenBuffer_351628 = temp_screen_buffer;
	}
	int16_t tempPortalPosX;
	int16_t tempPortalPosY;
	int16_t tempPortalPosX2;
	int16_t tempPortalPosY2;
	int16_t tempPortalPosX3;
	int16_t tempPortalPosY3;
	int16_t tempPortalPosX4;
	int16_t tempPortalPosY4;
	int16_t tempPortalPosX5;
	int16_t tempPortalPosY5;
	for (int ix = 0; mapScreenPortals_E17CC[ix].viewPortPosX_4; ix++)
	{
		if (mapScreenPortals_E17CC[ix].activated_18 == 1 && !firstPortal)
		{
			tempPortalPosX = mapScreenPortals_E17CC[ix].portalPosX_12 + menuSprites_17DED4[37].width_4 / 2;
			tempPortalPosY = mapScreenPortals_E17CC[ix].portalPosY_14 + menuSprites_17DED4[37].height_5 / 2;
			firstPortal = true;
			continue;
		}
		if (firstPortal && mapScreenPortals_E17CC[ix].activated_18 == 1)
		{
			tempPortalPosX2 = menuSprites_17DED4[37].width_4 / 2 + mapScreenPortals_E17CC[ix].portalPosX_12;
			tempPortalPosY2 = menuSprites_17DED4[37].height_5 / 2 + mapScreenPortals_E17CC[ix].portalPosY_14;
			memset(&animStruct, 0, 24);
			CreateAnimObject_7E8D0(&animStruct, tempPortalPosX2, tempPortalPosY2, tempPortalPosX, tempPortalPosY, 2, 2);
			tempPortalPosX3 = tempPortalPosX;
			tempPortalPosY3 = tempPortalPosY;
			while (!inArea)
			{
				for (int i = 0; i < 3; ++i) {
					inArea = MoveAnimObject_7E9D0(&tempPortalPosX, &tempPortalPosY, &animStruct);
				}
				if (abs(tempPortalPosY - tempPortalPosY3) > 8 || abs(tempPortalPosX - tempPortalPosX3) > 8)
				{
					DrawMapObject_812D0(tempPortalPosX, tempPortalPosY);
					tempPortalPosX3 = tempPortalPosX;
					tempPortalPosY3 = tempPortalPosY;
				}
			}
			tempPortalPosY = tempPortalPosY2;
			tempPortalPosX = tempPortalPosX2;
			inArea = false;
			continue;
		}
	}
	if (mapMenuState_17DB70.selectedLevel_17DB8A != -1)
	{
		int activated1 = false;
		int activatedIndex = -1;
		int k;
		for (k = 0; mapScreenPortals_E17CC[k].viewPortPosX_4; k++)
		{
			if (mapScreenPortals_E17CC[k].activated_18 == 1)
			{
				activated1 = true;
				activatedIndex = k;
			}
			if (mapScreenPortals_E17CC[k].activated_18 == 2)
				break;
		}
		if (k < 24 && activated1)
		{
			tempPortalPosX = mapScreenPortals_E17CC[activatedIndex].portalPosX_12 + menuSprites_17DED4[37].width_4 / 2;
			tempPortalPosY = mapScreenPortals_E17CC[activatedIndex].portalPosY_14 + menuSprites_17DED4[37].height_5 / 2;
			tempPortalPosX4 = menuSprites_17DED4[37].width_4 / 2 + mapScreenPortals_E17CC[k].portalPosX_12;
			tempPortalPosY4 = menuSprites_17DED4[37].height_5 / 2 + mapScreenPortals_E17CC[k].portalPosY_14;
			memset(&animStruct, 0, 24);
			CreateAnimObject_7E8D0(&animStruct, tempPortalPosX4, tempPortalPosY4, tempPortalPosX, tempPortalPosY, 2, 2);
			inArea = false;
			tempPortalPosX5 = tempPortalPosX;
			tempPortalPosY5 = tempPortalPosY;
			while (!inArea) {
				for (int i = 0; i < 3; ++i) {
					inArea = MoveAnimObject_7E9D0(&tempPortalPosX, &tempPortalPosY, &animStruct);
				}
				if (inArea) return;
				if (abs(tempPortalPosY - tempPortalPosY5) > 8 || abs(tempPortalPosX - tempPortalPosX5) > 8) {
					DrawMapObject_812D0(tempPortalPosX, tempPortalPosY);
					tempPortalPosX5 = tempPortalPosX;
					tempPortalPosY5 = tempPortalPosY;
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

	if (gameState_D41A4.setting_38402 == 1)
	{
		memset(printbuffer, 0, 80);
		sprintf(printbuffer, "%s/%s", gameDataPath.c_str(), "CONFIG.DAT");
		memset(&configDat, 0, sizeof(TypeConfigDat));
		configDatFile = DataFileIO::CreateOrOpenFile(printbuffer, 546);
		if (configDatFile != nullptr)
		{
			configDat.configDatSign_0 = 0xfffffff7;
			configDat.langIndex_4 = gameState_D41A4.SelectedLangIndex;
			configDat.soundVolume_6 = gameState_D41A4.soundVolume_6;
			configDat.musicVolume_8 = gameState_D41A4.musicVolume_8;
			configDat.showHelp_10 = gameState_D41A4.showHelp_10;
			configDat.brightness_11 = gameState_D41A4.brightness_11;
			configDat.brightness_12 = gameState_D41A4.brightness_12;
			configDat.brightness_13 = gameState_D41A4.brightness_13;
			configDat.wordindex_14 = gameState_D41A4.wordindex_14;
			configDat.dwordindex_16 = gameState_D41A4.dwordindex_16;
			configDat.stubb[0] = gameState_D41A4.stubb[0];
			configDat.stubb[1] = gameState_D41A4.stubb[1];

			for (int i = 0; i < 10; i++)
				configDat.keys[i] = keys_EB39E[i];

			DataFileIO::WriteFile_98CAA(configDatFile, (uint8_t*)&configDat, sizeof(TypeConfigDat));
			DataFileIO::Close(configDatFile);
		}
		gameState_D41A4.setting_38402 = 0;
	}
}

//----- (00082510) --------------------------------------------------------
void DrawMapMenuHelpText_82510(/*__int16 a1*//*, int *a2*/)//263510
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
	typeTextBoxtextBoxStr_E24BCx v10x[2];
	//char v11[2]; // [esp+10h] [ebp-1Ch]
	bitmap_pos_struct2_t* v12x; // [esp+24h] [ebp-8h]
	//uint8_t* v13; // [esp+28h] [ebp-4h]
	bitmap_pos_struct2_t* v13x; // [esp+28h] [ebp-4h]
	bitmap_pos_struct_t* v13s;

	uint8_t switchbit = 0;

	v13x = menuState_17DE38.fontTab_17DEC0;
	v12x = menuState_17DE38.fontTabEnd_17DEC4;
	menuState_17DE38.fontTab_17DEC0 = menuState_17DE38.font2Tab_17DEC8;
	menuState_17DE38.fontTabEnd_17DEC4 = menuState_17DE38.font2TabEnd_17DECC;

	v13s = fontSprites_17DEC0;//fixed
	fontSprites_17DEC0 = font2Sprites_17DEC8;//fixed

	if (gameState_D41A4.showHelp_10 == 1)
	{
		switchbit = mapHelpText_17DBA8.helpMode_17DBB6;//*((x_BYTE *)a2 + 14);
		if (switchbit == 2)
		{
			v2 = j___clock();
			v3 = mapHelpText_17DBA8.helpTextTime_17DBA8;
			mapHelpText_17DBA8.currentTime_17DBAC = v2;
			if ((v2 - v3) / 0x64u > 1)
			{
				v4 = mapHelpText_17DBA8.helpTextIndex_17DBB4 + 1;
				mapHelpText_17DBA8.helpTextIndex_17DBB4 = v4;
				//if (!unk_E2516[9 * v4 + 1])
				if (!textBoxStr_E2516[v4].left_2)
					mapHelpText_17DBA8.helpTextIndex_17DBB4 = 0;
				mapHelpText_17DBA8.helpTextTime_17DBA8 = mapHelpText_17DBA8.currentTime_17DBAC;//a2[0] = a2[1];
			}
			memset(v10x, 0, 36);//3550c0
			//v5 = (char*)&unk_E2516[9 * mapHelpText_17DBA8.unk_17DBB4];
			//v6x = str_E23E0;
			v6y = 0;
			v10x[0] = textBoxStr_E2516[mapHelpText_17DBA8.helpTextIndex_17DBB4];
			//qmemcpy(v10x, v5, 0x12u);
			//qmemcpy(&v11, v5 + 0x10u, 2u);
			while (mapMenuButtons_E23E0[v6y].active_25)
			{
				//if (str_E23E0[v6y].byte_23 && str_E23E0[v6y].byte_22 == (int)(unk_E2516[8 + 9 * mapHelpText_17DBA8.unk_17DBB4] >> 8))// x_BYTE_E2527[18 * *((signed __int16 *)a2 + 6)] )
				if (mapMenuButtons_E23E0[v6y].canSelect_23 && mapMenuButtons_E23E0[v6y].id_22 == textBoxStr_E2516[mapHelpText_17DBA8.helpTextIndex_17DBB4].mapButtonId_16)// x_BYTE_E2527[18 * *((signed __int16 *)a2 + 6)] )
				{
					mapMenuButtons_E23E0[v6y].gold_color_24 = 1;
					break;
				}
				//v6 += 44;
				v6y++;
			}
			DrawTextBoxWithLine_7E840(v10x, 238, 264);//draw help
		}
		else if (switchbit == 3)
		{
			v7 = j___clock();
			v8 = mapHelpText_17DBA8.helpTextTime_17DBA8;
			mapHelpText_17DBA8.currentTime_17DBAC = v7;
			if ((v7 - v8) / 0x64u > 1)
				mapHelpText_17DBA8.x_BYTE_17DBB5 = 2;
		}
	}
	menuState_17DE38.fontTab_17DEC0 = v13x;
	menuState_17DE38.fontTabEnd_17DEC4 = v12x;

	fontSprites_17DEC0 = v13s;//fixed
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
void PlayInGameFmv_82670()//263670
{
	uint8_t tempTypeResolution = videoMode_180660;
	char dataPath[MAX_PATH];
	sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/SCREENS/HSCREEN0.DAT");
	LastPressedKey_1806E4 = 0;
	if (!directStart_E29E1)
	{
		if (!(gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE))
		{
			DisplaySubtitles_D41C1 = 0;
			menuState_17DE38.menuSpritesData_17DE48c = &gameState_D41A4.pointer_0xE2_heapbuffer_226[0];
			menuState_17DE38.fontData_17DE54 = &gameState_D41A4.pointer_0xE2_heapbuffer_226[301787];
			menuState_17DE38.fontTab_17DEC0 = (bitmap_pos_struct2_t*)(&gameState_D41A4.pointer_0xE2_heapbuffer_226[308527]);
			menuState_17DE38.fontTabEnd_17DEC4 = (bitmap_pos_struct2_t*)(&gameState_D41A4.pointer_0xE2_heapbuffer_226[310159]);
			LoadAndDecompressDatFile_7AA70(dataPath, &gameState_D41A4.pointer_0xE2_heapbuffer_226[301787], 0x164FCD, 860);
			LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.fontTab_17DEC0, 0x165329, 548);
			if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] & 2 || gameState_D41A4.levelnumber_43w > 0x18u)
			{
				int16_t finalCutSceneIndex = 0;
				int cutSceneIndex = 0;
				while (cutScene_E16E0[cutSceneIndex].levelNumber_4)
				{
					if (gameState_D41A4.levelnumber_43w + 1 == cutScene_E16E0[cutSceneIndex].levelNumber_4)
					{
						if (!cutScene_E16E0[cutSceneIndex].overplayed_5)
						{
							finalCutSceneIndex = cutSceneIndex + 1;
							cutScene_E16E0[cutSceneIndex].overplayed_5 = 1;
						}
						break;
					}
					cutSceneIndex++;
				}
				if (!finalCutSceneIndex)
				{
					uint16_t levelNumber = gameState_D41A4.levelnumber_43w;
					if (levelNumber > 0x18u)
					{
						Type_SecretMapScreenPortals_E2970* secretLevel = GetSecretAndActivedPortal2_824E0(levelNumber);
						if (secretLevel)
						{
							cutSceneIndex = 0;
							while (cutScene_E16E0[cutSceneIndex].levelNumber_4)
							{
								if (secretLevel->index_4 + 1 == cutScene_E16E0[cutSceneIndex].levelNumber_4)
								{
									if (!cutScene_E16E0[cutSceneIndex].overplayed_5)
									{
										finalCutSceneIndex = cutSceneIndex + 1;
										cutScene_E16E0[cutSceneIndex].overplayed_5 = 1;
									}
									break;
								}
								cutSceneIndex++;
							}
						}
					}
				}
				if (finalCutSceneIndex)
				{
					if (gameState_D41A4.SelectedLangIndex == 2 && soundAble_E3798 || finalCutSceneIndex >= 6)
					{
						DisplaySubtitles_D41C1 = 0;
						DisplaySubtitles_D41C0 = 0;
					}
					else
					{
						StopSubtitles_2EB40();
						DisplaySubtitles_D41C1 = 1;
						DisplaySubtitles_D41C0 = 1;
					}
					SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]); //Set cursor to Null (Don't Draw)
					memset((void*)pdwScreenBuffer_351628, 0, 640 * 480);
					if (videoMode_180660 != 1 || screenWidth_18062C != 320 || screenHeight_180624 != 200)
					{
						MouseReset_54600();
						memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 768);
						if (videoMode_180660 & 1)
						{
							ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, 0);
							ClearScreenAlphaBuffer(320, 200, 255);
						}
						else
						{
							ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 640, 480, 0);
							ClearScreenAlphaBuffer(640, 480, 255);
						}
						VgaPaletteInstall_41A90((TColor*)*xadatapald0dat2.colorPalette_var28);
						videoMode_180660 = 1;
						VgaSetVideoMode320x200AndPalette_90D6E((TColor*)*xadatapald0dat2.colorPalette_var28);
						InstallMouse_8CEDF();
						SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]); //Set cursor to Null (Don't Draw)
					}
					if (videoMode_180660 & 1)
						CreateIndexDatTabDoubled_98709(menuState_17DE38.fontTab_17DEC0, menuState_17DE38.fontTabEnd_17DEC4, menuState_17DE38.fontData_17DE54, fontSprites_17DEC0);
					else
						CreateIndexDatTab_9874D(menuState_17DE38.fontTab_17DEC0, menuState_17DE38.fontTabEnd_17DEC4, menuState_17DE38.fontData_17DE54, fontSprites_17DEC0);
					char cutScenePath[MAX_PATH];
					sprintf(cutScenePath, "%s/INTRO/CUT%d.DAT", cdDataPath.c_str(), cutScene_E16E0[cutSceneIndex].fileIndex_6);
					sprintf(printbuffer, "%s", cutScenePath);

					if (extendedFonts)
						LoadFixedFonts(0, (char*)"intro");

					PlayInfoFmv(0, 1, cutScene_E16E0[cutSceneIndex].pSoundEvent_0, cutScenePath);
					VgaPalFadeInFadeOut_90B27(0, 0x10u, 0);
					EndSample_8D8F0();
					StopMusic_8E020();
					FadeClearBlit_7B5D0();
					if (videoMode_180660 & 1)
					{
						ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, 0);
						ClearScreenAlphaBuffer(320, 200, 255);
					}
					else
					{
						ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 640, 480, 0);
						ClearScreenAlphaBuffer(640, 480, 255);
					}

					if (videoMode_180660 & 1)
						VgaBlit320_90478();
					else
						VgaBlit640_75200(480);
					if (tempTypeResolution != videoMode_180660)
					{
						MouseReset_54600();
						memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 768);
						if (videoMode_180660 & 1)
						{
							ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, 0);
							ClearScreenAlphaBuffer(320, 200, 254);
						}
						else
						{
							ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 640, 480, 0);
							ClearScreenAlphaBuffer(640, 480, 254);
						}
						VgaPaletteInstall_41A90((TColor*)*xadatapald0dat2.colorPalette_var28);
						videoMode_180660 = tempTypeResolution;
						if (videoMode_180660 & 1)
							VgaSetVideoMode320x200AndPalette_90D6E((TColor*)*xadatapald0dat2.colorPalette_var28);
						else
							VgaSetVideoMode640x480AndPalette_90E07((TColor*)*xadatapald0dat2.colorPalette_var28);
						InstallMouse_8CEDF();
						SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]); //Set cursor to Null (Don't Draw)
					}
				}
			}
		}
	}
	DisplaySubtitles_D41C1 = 0;
}

//----- (00083250) --------------------------------------------------------
void PlayIntros_83250(char a1)//264250
{
	SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]);
	memset((void*)pdwScreenBuffer_351628, 0, 307200);
	if (videoMode_180660 != 1)
	{
		MouseReset_54600();
		memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 768);
		videoMode_180660 = 1;
		VgaSetVideoMode320x200AndPalette_90D6E((TColor*)*xadatapald0dat2.colorPalette_var28);
		InstallMouse_8CEDF();
		SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]); //Set cursor to Null (Don't Draw)
	}
	Intros_76D10(a1);
	MouseReset_54600();
	memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 768);
	if (videoMode_180660 & 1)
	{
		ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, 0);
		ClearScreenAlphaBuffer(320, 200, 255);
	}
	else
	{
		ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 640, 480, 0);
		ClearScreenAlphaBuffer(640, 480, 255);
	}

	VgaPaletteInstall_41A90((TColor*)*xadatapald0dat2.colorPalette_var28);
	videoMode_180660 = 8;
	VgaSetVideoMode640x480AndPalette_90E07((TColor*)*xadatapald0dat2.colorPalette_var28);
	InstallMouse_8CEDF();
	SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]); //Set cursor to Null (Don't Draw)
	SetCenterScreenForFlyAssistant_6EDB0();
	LoadHscreen_7A110(videoMode_180660, 4);
	ResetMouse_7B5A0();
	SetCursor_8CD27(menuSprites_17DED4[39]);
	menuState_17DE38.mouseButtons_17DEEE = 0;
	menuState_17DE38.keyScancode_17DF10 = 0;
	LoadSounds_84300(0);
}

//----- (000833C0) --------------------------------------------------------
void ShowEndCredits_833C0()//2643c0
{
	char dataPath[MAX_PATH];

	sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/SCREENS/HSCREEN0.DAT");

	int index2 = 0;
	bool noFade = false;
	menuState_17DE38.palette_17DE38x = (TColor*)*xadatapald0dat2.colorPalette_var28;
	menuState_17DE38.x_WORD_17DEEC = 0;
	menuState_17DE38.screenBuffer_17DE40 = pdwScreenBuffer_351628;
	menuState_17DE38.fileHandle_17DEE0 = NULL;
	menuState_17DE38.fileOffset_17DEDC = 0;
	menuState_17DE38.menuSpritesData_17DE48c = gameState_D41A4.pointer_0xE2_heapbuffer_226;
	FadeClearBlit_7B5D0();
	menuState_17DE38.fontData_17DE54 = menuState_17DE38.menuSpritesData_17DE48c + 0x49ADB;
	menuState_17DE38.fontTabEnd_17DEC4 = (bitmap_pos_struct2_t*)(menuState_17DE38.menuSpritesData_17DE48c + 0x4D313);
	menuState_17DE38.font2Data_17DE58 = menuState_17DE38.menuSpritesData_17DE48c + 0x4D313;
	menuState_17DE38.font2Tab_17DEC8 = (bitmap_pos_struct2_t*)(menuState_17DE38.menuSpritesData_17DE48c + 0x4ECC2);
	menuState_17DE38.font2TabEnd_17DECC = (bitmap_pos_struct2_t*)(menuState_17DE38.menuSpritesData_17DE48c + 0x4F31C);
	menuState_17DE38.menuSpritesTab_17DED4 = (bitmap_pos_struct2_t*)(menuState_17DE38.menuSpritesData_17DE48c + 0x4F31C);
	menuState_17DE38.menuSpritesTabEnd_17DED8 = (bitmap_pos_struct2_t*)(menuState_17DE38.menuSpritesData_17DE48c + 0x4FA72);
	menuState_17DE38.loadBuffer_17DE60 = menuState_17DE38.savedSmalltit_17DE44;
	menuState_17DE38.gameWorldMap_17DE64 = menuState_17DE38.menuSpritesData_17DE48c + 0x4FA72;
	menuState_17DE38.colorNextTable_17DE3C = (TcolNext*)(menuState_17DE38.menuSpritesData_17DE48c + 0x17BA72);
	menuState_17DE38.fontTab_17DEC0 = (bitmap_pos_struct2_t*)(menuState_17DE38.menuSpritesData_17DE48c + 0x4CCAD);
	menuState_17DE38.borderBitmap_17DE5C = menuState_17DE38.menuSpritesData_17DE48c + 0x17FA72;
	int time = j___clock();
	while (CheckKeyboard_9A10A())
	{
		LastPressedKey_1806E4 = 0;
		GetMouseAndKeyboardEvents_7A060();
	}
	SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]); //Set cursor to Null (Don't Draw)
	LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.fontData_17DE54, 0x1641FC, 1214);
	LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.fontTab_17DEC0, 0x1646BA, 589);
	LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.font2Data_17DE58, 0x164907, 1191);
	LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.font2Tab_17DEC8, 0x164DAE, 543);
	LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.palette_17DE38x, 0x178B5F, 768);
	LoadAndDecompressDatFile_7AA70(dataPath, menuState_17DE38.gameWorldMap_17DE64, 0x16554D, 79378);
	LoadAndDecompressDatFile_7AA70(0, 0, 0, 0);
	if (videoMode_180660 & 1)
	{
		CreateIndexDatTabDoubled_98709(menuState_17DE38.fontTab_17DEC0, menuState_17DE38.fontTabEnd_17DEC4, menuState_17DE38.fontData_17DE54, fontSprites_17DEC0);
	}
	else
	{
		CreateIndexDatTab_9874D(menuState_17DE38.fontTab_17DEC0, menuState_17DE38.fontTabEnd_17DEC4, menuState_17DE38.fontData_17DE54, fontSprites_17DEC0);
	}
	if (videoMode_180660 & 1)
	{
		CreateIndexDatTabDoubled_98709(menuState_17DE38.font2Tab_17DEC8, menuState_17DE38.font2TabEnd_17DECC, menuState_17DE38.font2Data_17DE58, font2Sprites_17DEC8);
	}
	else
	{
		CreateIndexDatTab_9874D(menuState_17DE38.font2Tab_17DEC8, menuState_17DE38.font2TabEnd_17DECC, menuState_17DE38.font2Data_17DE58, font2Sprites_17DEC8);
	}
	menuState_17DE38.keyScancode_17DF10 = 0;
	menuState_17DE38.mouseButtons_17DEEE = 0;
	if (videoMode_180660 & 1)
	{
		CreateIndexDatTabDoubledWithOffset_98709((uint8_t*)menuState_17DE38.fontTab_17DEC0, (uint8_t*)menuState_17DE38.fontTabEnd_17DEC4, menuState_17DE38.fontData_17DE54, fontSprites_17DEC0, 6);
	}
	else
	{
		CreateIndexDatTabWithOffset_9874D((uint8_t*)menuState_17DE38.fontTab_17DEC0, (uint8_t*)menuState_17DE38.fontTabEnd_17DEC4, menuState_17DE38.fontData_17DE54, fontSprites_17DEC0, 6);
	}
	menuState_17DE38.fontTab_17DEC0++;
	while (!menuState_17DE38.keyScancode_17DF10 && !menuState_17DE38.mouseButtons_17DEEE)
	{
		if (videoMode_180660 & 1)
			CopyScreen((void*)menuState_17DE38.gameWorldMap_17DE64, (void*)pdwScreenBuffer_351628, 320, 200);
		else
			CopyScreen((void*)menuState_17DE38.gameWorldMap_17DE64, (void*)pdwScreenBuffer_351628, 640, 480);
		int time2 = j___clock();
		int index1;
		index1 = index2;
		bool reachedEnd = false;
		for (int i = 0; ; i += fontSprites_17DEC0[65].height_5 + 2)
		{
			int timeDiff = (time2 - time) / 100;
			int index3 = index1;
			if (off_DB558[index3] == NULL)//end of sequence
			{
				if (timeDiff > 5)
				{
					time = j___clock();
					index2 = 0;
				}
				reachedEnd = true;
				break;
			}
			if (!_stricmp(off_DB558[index3], "!"))
				break;
			uint8_t colorIndex = GetPaletteIndex_5BE80(menuState_17DE38.palette_17DE38x, 0x3Fu, 0x3Fu, 0x3Fu);
			DrawTextWithBoarder_7FCB0(off_DB558[index3], 10, 620, (signed __int16)(i + 200), 5, colorIndex, 0);
			index1++;
		}
		if (!reachedEnd && (time2 - time) / 100 > 2)
		{
			index1++;
			time = j___clock();
			index2 = index1;
		}

		GetMouseAndKeyboardEvents_7A060();
		if (noFade)
		{
			if (videoMode_180660 & 1)
				VgaBlit320_90478();
			else
				VgaBlit640_75200(480);
		}
		else
		{
			VgaPalFadeInFadeOut_90B27(menuState_17DE38.palette_17DE38x, 0x20u, 0);
			noFade = true;
		}
	}
	VgaPalFadeInFadeOut_90B27(0, 0x10u, 0);
	memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 768);
	if (videoMode_180660 & 1)
	{
		ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, 0);
		ClearScreenAlphaBuffer(320, 200, 255);
	}
	else
	{
		ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 640, 480, 0);
		ClearScreenAlphaBuffer(640, 480, 255);
	}
	if (videoMode_180660 & 1)
		VgaBlit320_90478();
	else
		VgaBlit640_75200(480);
	while (menuState_17DE38.keyScancode_17DF10 || menuState_17DE38.mouseButtons_17DEEE)
		GetMouseAndKeyboardEvents_7A060();
	menuState_17DE38.keyScancode_17DF10 = 0;
}

void ShowScores()
{
	int time = j___clock();

	char dataPath[MAX_PATH];

	sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/SCREENS/HSCREEN0.DAT");

	bool noFade = false;
	menuState_17DE38.palette_17DE38x = (TColor*)*xadatapald0dat2.colorPalette_var28;
	menuState_17DE38.x_WORD_17DEEC = 0;
	menuState_17DE38.screenBuffer_17DE40 = pdwScreenBuffer_351628;
	menuState_17DE38.fileHandle_17DEE0 = NULL;
	menuState_17DE38.fileOffset_17DEDC = 0;
	menuState_17DE38.menuSpritesData_17DE48c = gameState_D41A4.pointer_0xE2_heapbuffer_226;
	FadeClearBlit_7B5D0();
	menuState_17DE38.fontData_17DE54 = menuState_17DE38.menuSpritesData_17DE48c + 0x49ADB;
	menuState_17DE38.fontTabEnd_17DEC4 = (bitmap_pos_struct2_t*)(menuState_17DE38.menuSpritesData_17DE48c + 0x4D313);
	menuState_17DE38.font2Data_17DE58 = menuState_17DE38.menuSpritesData_17DE48c + 0x4D313;
	menuState_17DE38.font2Tab_17DEC8 = (bitmap_pos_struct2_t*)(menuState_17DE38.menuSpritesData_17DE48c + 0x4ECC2);
	menuState_17DE38.font2TabEnd_17DECC = (bitmap_pos_struct2_t*)(menuState_17DE38.menuSpritesData_17DE48c + 0x4F31C);
	menuState_17DE38.menuSpritesTab_17DED4 = (bitmap_pos_struct2_t*)(menuState_17DE38.menuSpritesData_17DE48c + 0x4F31C);
	menuState_17DE38.menuSpritesTabEnd_17DED8 = (bitmap_pos_struct2_t*)(menuState_17DE38.menuSpritesData_17DE48c + 0x4FA72);
	menuState_17DE38.loadBuffer_17DE60 = menuState_17DE38.savedSmalltit_17DE44;
	menuState_17DE38.gameWorldMap_17DE64 = menuState_17DE38.menuSpritesData_17DE48c + 0x4FA72;
	menuState_17DE38.colorNextTable_17DE3C = (TcolNext*)(menuState_17DE38.menuSpritesData_17DE48c + 0x17BA72);
	menuState_17DE38.fontTab_17DEC0 = (bitmap_pos_struct2_t*)(menuState_17DE38.menuSpritesData_17DE48c + 0x4CCAD);
	menuState_17DE38.borderBitmap_17DE5C = menuState_17DE38.menuSpritesData_17DE48c + 0x17FA72;

	while (CheckKeyboard_9A10A())
	{
		LastPressedKey_1806E4 = 0;
		GetMouseAndKeyboardEvents_7A060();
	}

	SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]); //Set cursor to Null (Don't Draw)
	LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.fontData_17DE54, 0x1641FC, 1214);
	LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.fontTab_17DEC0, 0x1646BA, 589);
	LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.font2Data_17DE58, 0x164907, 1191);
	LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.font2Tab_17DEC8, 0x164DAE, 543);
	LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.palette_17DE38x, 0x178B5F, 768);
	LoadAndDecompressDatFile_7AA70(dataPath, menuState_17DE38.gameWorldMap_17DE64, 0x16554D, 79378);
	LoadAndDecompressDatFile_7AA70(0, 0, 0, 0);
	if (videoMode_180660 & 1)
	{
		CreateIndexDatTabDoubled_98709(menuState_17DE38.fontTab_17DEC0, menuState_17DE38.fontTabEnd_17DEC4, menuState_17DE38.fontData_17DE54, fontSprites_17DEC0);
	}
	else
	{
		CreateIndexDatTab_9874D(menuState_17DE38.fontTab_17DEC0, menuState_17DE38.fontTabEnd_17DEC4, menuState_17DE38.fontData_17DE54, fontSprites_17DEC0);
	}
	if (videoMode_180660 & 1)
	{
		CreateIndexDatTabDoubled_98709(menuState_17DE38.font2Tab_17DEC8, menuState_17DE38.font2TabEnd_17DECC, menuState_17DE38.font2Data_17DE58, font2Sprites_17DEC8);
	}
	else
	{
		CreateIndexDatTab_9874D(menuState_17DE38.font2Tab_17DEC8, menuState_17DE38.font2TabEnd_17DECC, menuState_17DE38.font2Data_17DE58, font2Sprites_17DEC8);
	}
	menuState_17DE38.keyScancode_17DF10 = 0;
	menuState_17DE38.mouseButtons_17DEEE = 0;
	if (videoMode_180660 & 1)
	{
		CreateIndexDatTabDoubledWithOffset_98709((uint8_t*)menuState_17DE38.fontTab_17DEC0, (uint8_t*)menuState_17DE38.fontTabEnd_17DEC4, menuState_17DE38.fontData_17DE54, fontSprites_17DEC0, 6);
	}
	else
	{
		CreateIndexDatTabWithOffset_9874D((uint8_t*)menuState_17DE38.fontTab_17DEC0, (uint8_t*)menuState_17DE38.fontTabEnd_17DEC4, menuState_17DE38.fontData_17DE54, fontSprites_17DEC0, 6);
	}
	menuState_17DE38.fontTab_17DEC0++;

	auto lines = SplitLines(g_matchResultPending);

	while (!menuState_17DE38.keyScancode_17DF10 && !menuState_17DE38.mouseButtons_17DEEE)
	{
		int timeDiff = (j___clock() - time) / 100;

		if (videoMode_180660 & 1)
			CopyScreen((void*)menuState_17DE38.gameWorldMap_17DE64, (void*)pdwScreenBuffer_351628, 320, 200);
		else
			CopyScreen((void*)menuState_17DE38.gameWorldMap_17DE64, (void*)pdwScreenBuffer_351628, 640, 480);

		int i = 0;
		for (const std::string& line : lines)
		{
			i += fontSprites_17DEC0[65].height_5 + 2;
			uint8_t colorIndex = GetPaletteIndex_5BE80(menuState_17DE38.palette_17DE38x, 0x3Fu, 0x3Fu, 0x3Fu);
			DrawTextWithBoarder_7FCB0((char*)line.c_str(), 10, 620, (signed __int16)(i + 200), 5, colorIndex, 0);
		}

		if (timeDiff > 2)
			GetMouseAndKeyboardEvents_7A060();

		if (noFade)
		{
			if (videoMode_180660 & 1)
				VgaBlit320_90478();
			else
				VgaBlit640_75200(480);
		}
		else
		{
			VgaPalFadeInFadeOut_90B27(menuState_17DE38.palette_17DE38x, 0x20u, 0);
			noFade = true;
		}
	}
	VgaPalFadeInFadeOut_90B27(0, 0x10u, 0);
	memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 768);
	if (videoMode_180660 & 1)
	{
		ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, 0);
		ClearScreenAlphaBuffer(320, 200, 255);
	}
	else
	{
		ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 640, 480, 0);
		ClearScreenAlphaBuffer(640, 480, 255);
	}
	if (videoMode_180660 & 1)
		VgaBlit320_90478();
	else
		VgaBlit640_75200(480);
	while (menuState_17DE38.keyScancode_17DF10 || menuState_17DE38.mouseButtons_17DEEE)
		GetMouseAndKeyboardEvents_7A060();
	menuState_17DE38.keyScancode_17DF10 = 0;

	g_matchResultPending.clear();
}

std::vector<std::string> SplitLines(const std::string& text) {
	std::vector<std::string> lines;
	size_t start = 0;
	size_t end;
	while ((end = text.find('\n', start)) != std::string::npos) {
		std::string line = text.substr(start, end - start);
		if (!line.empty() && line.back() == '\r') {
			line.pop_back();
		}
		lines.push_back(line);
		start = end + 1;
	}
	// last line (if there's no trailing newline)
	if (start < text.size()) {
		lines.push_back(text.substr(start));
	}
	return lines;
}

//----- (00083850) --------------------------------------------------------
void ShowWelcomeScreen_83850()//264850
{
	char dataPath[MAX_PATH];
	sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/SCREENS/HSCREEN0.DAT");
	LoadAndDecompressDatFile_7AA70(dataPath, smalltitBuffer_E9C38, 0x178E5F, 0x32B9);
	LoadAndDecompressDatFile_7AA70(dataPath, *xadatapald0dat2.colorPalette_var28, 0x17C118, 0x300);
	LoadAndDecompressDatFile_7AA70(cdDataPath.c_str(), nullptr, 0, 0);
	bool afterFade = false;
	int time = j___clock();
	LastPressedKey_1806E4 = 0;
	while (!LastPressedKey_1806E4 && !mouseLeftButton_180746 && !mouseRightButton_180744 && (j___clock() - time) / 100 <= 20)
	{
		SetFrameStart(std::chrono::system_clock::now());
		if (videoMode_180660 & 1)
			CopyScreen(smalltitBuffer_E9C38, pdwScreenBuffer_351628, 320, 200);
		else
			CopyScreen(smalltitBuffer_E9C38, pdwScreenBuffer_351628, 640, 480);
		if (afterFade)
		{
			if (videoMode_180660 & 1)
				VgaBlit320_90478(menuFps);
			else
				VgaBlit640_75200(480, menuFps);
		}
		else
		{
			afterFade = true;
			ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 640, 480, 0);
			ClearScreenAlphaBuffer(640, 480, 255);
			VgaBlit640_75200(480, menuFps);
			VgaPalFadeInFadeOut_90B27((TColor*)*xadatapald0dat2.colorPalette_var28, 0x20u, 0);
		}
	}
	VgaPalFadeInFadeOut_90B27(0, 0x10u, 0);
	if (videoMode_180660 & 1)
	{
		ClearGraphicsBuffer_72883(pdwScreenBuffer_351628, 320, 200, 0);
		ClearScreenAlphaBuffer(320, 200, 255);
	}
	else
	{
		ClearGraphicsBuffer_72883(pdwScreenBuffer_351628, 640, 480, 0);
		ClearScreenAlphaBuffer(640, 480, 255);
	}
	if (videoMode_180660 & 1)
		VgaBlit320_90478(menuFps);
	else
		VgaBlit640_75200(480, menuFps);
	while (CheckKeyboard_9A10A())
	{
		LastPressedKey_1806E4 = 0;
		GetMouseAndKeyboardEvents_7A060();
	}
	j___delay(50);
}

//----- (00081EE0) --------------------------------------------------------
void DrawAndSoundDragonAndFire_81EE0(__int16 a5, __int16 a6) //262ee0
{
	bool objectResult = false;
	signed __int16 result; // ax
	uint8_t* temp_screen_buffer; // ST1C_4

	int time = j___clock();
	int index = 0;
	for (result = dragonAnims_E20A4[0].anim_18.beginX_2; result; result = dragonAnims_E20A4[index].anim_18.beginX_2) {
		if (dragonAnims_E20A4[index].animKind_42 == 1) {
			switch (dragonAnims_E20A4[index].animState_43) {
			case 0u:
				dragonAnims_E20A4[index].time_0 = time;
				dragonAnims_E20A4[index].animState_43++;
				break;
			case 1u:
				if ((time - dragonAnims_E20A4[index].time_0) / 0x64u > dragonAnims_E20A4[index].anim_18.stateY_11)
					dragonAnims_E20A4[index].animState_43++;
				break;
			case 2u:
				dragonAnims_E20A4[index].anim_18.actX_0 = dragonAnims_E20A4[index].anim_18.beginX_2;
				dragonAnims_E20A4[index].anim_18.actY_1 = dragonAnims_E20A4[index].anim_18.beginY_3;
				CreateAnimObject_7E8D0(&dragonAnims_E20A4[index].animObject_45, dragonAnims_E20A4[index].anim_18.maxX_4, dragonAnims_E20A4[index].anim_18.maxY_5, dragonAnims_E20A4[index].anim_18.beginX_2, dragonAnims_E20A4[index].anim_18.beginY_3, 2, 2);
				dragonAnims_E20A4[index].anim_18.act2_8 = dragonAnims_E20A4[index].anim_18.begin_6;
				dragonAnims_E20A4[index].animState_43++;
				PlaySample_8F100(0, dragonAnims_E20A4[index].sample1_10, menuSoundVolume_E1324, 64, 0x64u, 0, dragonAnims_E20A4[index].sample1PlayType_12);
				break;
			case 3u:
				for (int i = 0; i < 4; i++) {
					objectResult = MoveAnimObject_7E9D0(&dragonAnims_E20A4[index].anim_18.actX_0, &dragonAnims_E20A4[index].anim_18.actY_1, &dragonAnims_E20A4[index].animObject_45);
				}
				if (time - dragonAnims_E20A4[index].time_0 >= 8) {
					if (dragonAnims_E20A4[index].anim_18.act2_8 <= dragonAnims_E20A4[index].anim_18.act_7 - 1)
						dragonAnims_E20A4[index].anim_18.act2_8++;
					else
						dragonAnims_E20A4[index].anim_18.act2_8 = dragonAnims_E20A4[index].anim_18.begin_6;
					dragonAnims_E20A4[index].time_0 = time;
				}
				DrawBitmap640_7C120(dragonAnims_E20A4[index].anim_18.actX_0 - a5, dragonAnims_E20A4[index].anim_18.actY_1 - a6, menuSprites_17DED4[dragonAnims_E20A4[index].anim_18.act2_8]);
				if (objectResult) {
					dragonAnims_E20A4[index].anim_18.act2_8 = dragonAnims_E20A4[index].anim_18.begin2_9;
					dragonAnims_E20A4[index].time_0 = time;
					dragonAnims_E20A4[index].animState_43++;
					PlaySample_8F100(0, dragonAnims_E20A4[index].sample2_14, menuSoundVolume_E1324, 64, 0x64u, 0, dragonAnims_E20A4[index].sample2PlayType_16);
				}
				break;
			case 4u:
				objectResult = false;
				if (time - dragonAnims_E20A4[index].time_0 >= 8) {
					if (dragonAnims_E20A4[index].anim_18.act2_8 <= dragonAnims_E20A4[index].anim_18.stateX_10 - 1) {
						dragonAnims_E20A4[index].anim_18.act2_8++;
					}
					else {
						objectResult = true;
						dragonAnims_E20A4[index].anim_18.act2_8 = dragonAnims_E20A4[index].anim_18.stateX_10;
					}
					dragonAnims_E20A4[index].time_0 = time;
				}
				if (!objectResult) {
					DrawBitmap640_7C120(dragonAnims_E20A4[index].anim_18.actX_0 - a5, dragonAnims_E20A4[index].anim_18.actY_1 - a6, menuSprites_17DED4[dragonAnims_E20A4[index].anim_18.act2_8]);
				}
				else {
					dragonAnims_E20A4[index].animState_43 = 0;
				}
				break;
			}
		}
		else if (dragonAnims_E20A4[index].animKind_42 == 2u) {
			if (mapMenuState_17DB70.mapState_17DB8F != 4) {
				int i = 0;
				for (i = 0; mapScreenPortals_E17CC[i].viewPortPosX_4 && mapScreenPortals_E17CC[i].activated_18 != 2; i++)
					;
				if (i == 25)
					i = 24;
				if (i == dragonAnims_E20A4[index].portalIndex_44) {
					switch (dragonAnims_E20A4[index].animState_43) {
					case 0:
						dragonAnims_E20A4[index].time_0 = time;
						dragonAnims_E20A4[index].animState_43++;
						break;
					case 1:
						dragonAnims_E20A4[index].anim_18.actX_0 = dragonAnims_E20A4[index].anim_18.beginX_2;
						dragonAnims_E20A4[index].anim_18.actY_1 = dragonAnims_E20A4[index].anim_18.beginY_3;
						CreateAnimObject_7E8D0(&dragonAnims_E20A4[index].animObject_45, dragonAnims_E20A4[index].anim_18.maxX_4, dragonAnims_E20A4[index].anim_18.maxY_5, dragonAnims_E20A4[index].anim_18.beginX_2, dragonAnims_E20A4[index].anim_18.beginY_3, 2, 2);
						dragonAnims_E20A4[index].anim_18.act2_8 = dragonAnims_E20A4[index].anim_18.begin_6;
						dragonAnims_E20A4[index].animState_43++;
						PlaySample_8F100(0, dragonAnims_E20A4[index].sample1_10, menuSoundVolume_E1324, 64, 0x64u, 0, dragonAnims_E20A4[index].sample1PlayType_12);
						break;
					case 2:
						for (int j = 0; j < 4; j++) {
							objectResult = MoveAnimObject_7E9D0(&dragonAnims_E20A4[index].anim_18.actX_0, &dragonAnims_E20A4[index].anim_18.actY_1, &dragonAnims_E20A4[index].animObject_45);
						}
						if (time - dragonAnims_E20A4[index].time_0 >= 8) {
							if (dragonAnims_E20A4[index].anim_18.act2_8 <= dragonAnims_E20A4[index].anim_18.act_7 - 1)
								dragonAnims_E20A4[index].anim_18.act2_8++;
							else
								dragonAnims_E20A4[index].anim_18.act2_8 = dragonAnims_E20A4[index].anim_18.begin_6;
							dragonAnims_E20A4[index].time_0 = time;
						}
						DrawBitmap640_7C120(dragonAnims_E20A4[index].anim_18.actX_0 - a5, dragonAnims_E20A4[index].anim_18.actY_1 - a6, menuSprites_17DED4[dragonAnims_E20A4[index].anim_18.act2_8]);
						if (objectResult) {
							dragonAnims_E20A4[index].anim_18.act2_8 = dragonAnims_E20A4[index].anim_18.begin2_9;
							dragonAnims_E20A4[index].time_0 = time;
							dragonAnims_E20A4[index].animState_43++;
							PlaySample_8F100(0, dragonAnims_E20A4[index].sample2_14, menuSoundVolume_E1324, 64, 0x64u, 0, dragonAnims_E20A4[index].sample2PlayType_16);
						}
						break;
					case 3:
						objectResult = false;
						if (time - dragonAnims_E20A4[index].time_0 >= 8) {
							if (dragonAnims_E20A4[index].anim_18.act2_8 <= dragonAnims_E20A4[index].anim_18.stateX_10 - 1) {
								dragonAnims_E20A4[index].anim_18.act2_8++;
							}
							else {
								objectResult = true;
								dragonAnims_E20A4[index].anim_18.act2_8 = dragonAnims_E20A4[index].anim_18.begin2_9;
							}
							dragonAnims_E20A4[index].time_0 = time;
						}
						if (!objectResult) {
							DrawBitmap640_7C120(dragonAnims_E20A4[index].anim_18.actX_0 - a5, dragonAnims_E20A4[index].anim_18.actY_1 - a6, menuSprites_17DED4[dragonAnims_E20A4[index].anim_18.act2_8]);
						}
						else {
							dragonAnims_E20A4[index].animState_43++;
						}
						break;
					default:
						break;
					}
				}
				else {
					dragonAnims_E20A4[index].animState_43 = 0;
				}
			}
		}
		else if (dragonAnims_E20A4[index].animKind_42 == 3) {
			int i = 0;
			for (i = 0; mapScreenPortals_E17CC[i].viewPortPosX_4 && mapScreenPortals_E17CC[i].activated_18 != 2; i++)
				;
			if (i)
				i--;
			if (i == dragonAnims_E20A4[index].portalIndex_44) {
				if (dragonAnims_E20A4[index].animState_43 < 1u) {
					if (!dragonAnims_E20A4[index].animState_43) {
						dragonAnims_E20A4[index].time_0 = time;
						dragonAnims_E20A4[index].animState_43++;
					}
				}
				else if (dragonAnims_E20A4[index].animState_43 <= 1u) {
					dragonAnims_E20A4[index].anim_18.actX_0 = dragonAnims_E20A4[index].anim_18.beginX_2;
					dragonAnims_E20A4[index].anim_18.actY_1 = dragonAnims_E20A4[index].anim_18.beginY_3;
					CreateAnimObject_7E8D0(&dragonAnims_E20A4[index].animObject_45, dragonAnims_E20A4[index].anim_18.maxX_4, dragonAnims_E20A4[index].anim_18.maxY_5, dragonAnims_E20A4[index].anim_18.beginX_2, dragonAnims_E20A4[index].anim_18.beginY_3, 2, 2);
					dragonAnims_E20A4[index].anim_18.act2_8 = dragonAnims_E20A4[index].anim_18.begin_6;
					dragonAnims_E20A4[index].animState_43++;
					PlaySample_8F100(0, dragonAnims_E20A4[index].sample1_10, (unsigned __int8)menuSoundVolume_E1324, 64, 0x64u, 0, dragonAnims_E20A4[index].sample1PlayType_12);
				}
				else if (dragonAnims_E20A4[index].animState_43 == 2) {
					for (int k = 0; k < 4; k++) {
						objectResult = MoveAnimObject_7E9D0(&dragonAnims_E20A4[index].anim_18.actX_0, &dragonAnims_E20A4[index].anim_18.actY_1, &dragonAnims_E20A4[index].animObject_45);
					}
					if (time - dragonAnims_E20A4[index].time_0 >= 8) {
						if (dragonAnims_E20A4[index].anim_18.act2_8 <= dragonAnims_E20A4[index].anim_18.act_7 - 1)
							++dragonAnims_E20A4[index].anim_18.act2_8;
						else
							dragonAnims_E20A4[index].anim_18.act2_8 = dragonAnims_E20A4[index].anim_18.begin_6;
						dragonAnims_E20A4[index].time_0 = time;
					}
					if (mapScreenPortals_E17CC[24].activated_18 == 1) {
						int tempWidth = screenWidth_18062C;
						int tempHeight = screenHeight_180624;
						temp_screen_buffer = pdwScreenBuffer_351628;
						pdwScreenBuffer_351628 = menuState_17DE38.gameWorldMap_17DE64;
						clipWidth_180648 = 1280;
						screenWidth_18062C = 1280;
						clipHeight_180644 = 960;
						screenHeight_180624 = 960;
						DrawBitmap640_7C120(dragonAnims_E20A4[index].anim_18.actX_0, dragonAnims_E20A4[index].anim_18.actY_1, menuSprites_17DED4[dragonAnims_E20A4[index].anim_18.act2_8]);
						screenWidth_18062C = tempWidth;
						screenHeight_180624 = tempHeight;
						clipWidth_180648 = tempWidth;
						clipHeight_180644 = tempHeight;
						pdwScreenBuffer_351628 = temp_screen_buffer;
					}
					else {
						DrawBitmap640_7C120(dragonAnims_E20A4[index].anim_18.actX_0 - a5, dragonAnims_E20A4[index].anim_18.actY_1 - a6, menuSprites_17DED4[dragonAnims_E20A4[index].anim_18.act2_8]);
					}
				}
			}
			else {
				dragonAnims_E20A4[index].animState_43 = 0;
			}
		}
		index++;
	}
}

//----- (00082C20) --------------------------------------------------------
void DrawEndGameTable_82C20(__int16 a1) //263c20
{
	char* texts[6];
	int v30x[8];
	int x2 = 50;
	int y2 = 50;
	EventDispatcher::I->DispatchEvent(EventType::E_SCENE_CHANGE, Scene::PREAMBLE_MENU);
	if (a1 > -1) {
		texts[0] = (char*)LevelsNames_D9204[a1];
		texts[1] = langStrings_E9C4C[386]; //Spells found
		texts[2] = langStrings_E9C4C[385]; //Accuracy
		texts[3] = langStrings_E9C4C[384]; //Creatures Killed
		texts[4] = langStrings_E9C4C[377]; //Mana
		texts[5] = langStrings_E9C4C[394]; //Time Taken
		v30x[0] = 0;
		v30x[1] = 0;
		v30x[7] = 0;
		if (a1 > 24) {
			int portalIndex = 0;
			while (secretMapScreenPortals_E2970[portalIndex].activated_12) {
				if (a1 == secretMapScreenPortals_E2970[portalIndex].levelNumber_6) {
					v30x[2] = secretLevelScores_17DDBC[0 + 5 * portalIndex]; //Spells found
					v30x[3] = secretLevelScores_17DDBC[1 + 5 * portalIndex]; //Accuracy
					v30x[4] = secretLevelScores_17DDBC[2 + 5 * portalIndex]; //Creatures Killed
					v30x[5] = secretLevelScores_17DDBC[3 + 5 * portalIndex]; //Mana
					v30x[6] = secretLevelScores_17DDBC[4 + 5 * portalIndex]; //Time Taken
				}
				portalIndex++;
			}
		}
		else {
			v30x[2] = levelScores_17DBC8[0 + 5 * a1]; //Spells found
			v30x[3] = levelScores_17DBC8[1 + 5 * a1]; //Accuracy
			v30x[4] = levelScores_17DBC8[2 + 5 * a1]; //Creatures Killed
			v30x[5] = levelScores_17DBC8[3 + 5 * a1]; //Mana
			v30x[6] = levelScores_17DBC8[4 + 5 * a1]; //Time Taken
		}
		int maxstrLen = strlen(texts[0]);
		for (int i = 1; i < 6; i++) {
			unsigned int currentLen = strlen(texts[i]);
			if (currentLen > maxstrLen) {
				maxstrLen = currentLen;
			}
		}
		int frameWidth = 2 * fontSprites_17DEC0[65].width_4 * maxstrLen;
		int beginX = 320 - frameWidth / 2;
		if (frameWidth % menuSprites_17DED4[275].width_4)
			frameWidth = (frameWidth / menuSprites_17DED4[275].width_4 + 1) * menuSprites_17DED4[275].width_4;
		for (int i = 0; i < frameWidth; i += menuSprites_17DED4[275].width_4)
			DrawBitmap640_7C120(i + beginX, y2, menuSprites_17DED4[275]);
		int blockCounter = 0;
		y2 += menuSprites_17DED4[275].height_5;
		do {
			int innerWidth = frameWidth - 2 * menuSprites_17DED4[274].width_4;
			int startX = beginX + menuSprites_17DED4[274].width_4;
			int bufferIndex = startX + 640 * y2;
			DrawBitmap640_7C120(beginX, y2, menuSprites_17DED4[274]);
			DrawBitmap640_7C120(beginX + frameWidth - menuSprites_17DED4[274].width_4, y2, menuSprites_17DED4[274]);
			for (int y = 0; y < menuSprites_17DED4[274].height_5; y++) {
				for (int x = 0; x < frameWidth - 2 * menuSprites_17DED4[274].width_4; x++) {
					pdwScreenBuffer_351628[bufferIndex] = menuState_17DE38.colorNextTable_17DE3C->data[pdwScreenBuffer_351628[bufferIndex] + 0xF00];//fixed, in original code not 0xC00
					bufferIndex++;
				}
				bufferIndex += 640 - innerWidth;
				y2++;
			}
			blockCounter++;
		} while (blockCounter < 12);
		for (int j = 0; j < frameWidth; j += menuSprites_17DED4[275].width_4)
			DrawBitmap640_7C120(j + beginX, y2, menuSprites_17DED4[275]);
		int width4 = 2 * menuSprites_17DED4[274].width_4 + x2;
		int x3 = fontSprites_17DEC0[65].width_4 + beginX;
		DrawText_7FAE0(texts[0], x3, frameWidth - fontSprites_17DEC0[65].width_4 + x3, width4, 0);
		width4 += 5;
		int beginX2;
		for (int k = 1; k < 5; k++) {
			beginX2 = width4 + 2;
			if (texts[k]) {
				if (gameState_D41A4.SelectedLangIndex != 2)
					DrawText_7FB90(texts[k], x3 + 2 * fontSprites_17DEC0[65].width_4, k * fontSprites_17DEC0[65].height_5 + beginX2, 0);
				else {
					if (k == 1 || k == 3)
						DrawText_7FB90(texts[k], x3 + 2 * fontSprites_17DEC0[65].width_4, fontSprites_17DEC0[65].height_5 * k + width4 + 2, 0);
					else if (k == 2)
						DrawText_7FB90((char*)"Hit Accuracy", x3 + 2 * fontSprites_17DEC0[65].width_4, beginX2 + k * fontSprites_17DEC0[65].height_5, 0);
					else if (k == 4)
						DrawText_7FB90((char*)"Mana Collected", x3 + 2 * fontSprites_17DEC0[65].width_4, beginX2 + 4 * fontSprites_17DEC0[65].height_5, 0);
				}
				sprintf(printbuffer, "%3d%%", v30x[1 + k]);
				DrawText_7FB90(printbuffer, x3 + frameWidth - 8 * fontSprites_17DEC0[65].width_4, width4 + 2 + k * fontSprites_17DEC0[65].height_5, 0);
			}
		}
		beginX2 = width4 + 2;
		DrawText_7FB90(texts[5], x3 + 2 * fontSprites_17DEC0[65].width_4, beginX2 + 6 * fontSprites_17DEC0[65].height_5, 0); //fixed 65
		sprintf(printbuffer, "%02d:%02d:%02d", v30x[6] / 3600, v30x[6] % 3600 / 60, v30x[6] % 3600 % 60);
		DrawText_7FB90(printbuffer, x3 + frameWidth - 14 * fontSprites_17DEC0[65].width_4, beginX2 + 6 * fontSprites_17DEC0[65].height_5, 0); //fixed 65
	}
}

//----- (00085C8B) --------------------------------------------------------
void DrawNetGameMapBackground_85C8B(uint8_t* source, uint8_t* dest, int beginX, int beginY, int width, int height)//266c8b
{
	int destIndex = 0;
	int sourceIndex = beginX + 1280 * beginY;
	do
	{
		qmemcpy(&dest[destIndex], &source[sourceIndex], 4 * width);
		destIndex += 4 * width;
		sourceIndex += 4 * width + 640;
		height--;
	} while (height);
}

bool DrawAndServe_pre_sub_7B250(uint32_t var, type_menuButtons_E1F84* var2x)
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
		return MultiplayerPreviousLevel_7CCF0();
		break;
	}
	case 0x25dd30: {
		return MultiplayerNextLevel_7CD30();
		break;
	}
	case 0x25dca0: {
		return MultiplayerExit_7CCA0();
		break;
	}
	case 0x25dda0: {
		return MultiplayerPlayLevel_7CDA0();
		break;
	}
	case 0x25dbf0: {
		return MultiplayerPreviousWizard_7CBF0();
		break;
	}
	case 0x25dc40: {
		return MultiplayerNextWizard_7CC40();
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
bool ExitDialog_77980(type_menuButtons_E1F84* menuButton)//258980
{
	__int16 exitResult = DrawScrollDialog_7BF20(&menuButton->scroll_26);//draw exit dialog
	bool dialogClosed = exitResult;
	if (exitResult == 1)//Exit Game
	{
		m_ExitMenuLoop_E29DC = 1;
		world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].syncReady_0x004_2BE0_11234 = 1;
	}
	else if (exitResult == 2)//no Exit Game
	{
		dialogClosed = true;
		ClearScrollDialogVars_7C020(&menuButton->scroll_26);
	}
	return dialogClosed;
}

//----- (00078E00) --------------------------------------------------------
char SetPlayerNameDialog_78E00(/*int a1, int a2, */type_menuButtons_E1F84* a3x)//259e00
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
	/*v3 = */GetFont_6FC50(1);
	//LOWORD(v3) = a3[13];
	v29 = a3x->scroll_26.x1_26_0 + 10;
	//HIWORD(v3) = HIWORD(x_DWORD_17DED4);
	v3 = menuSprites_17DED4[72].width_4;
	v27 = v29 + v3 - 22;
	v4 = a3x->scroll_26.y1_28_1 + a3x->scroll_26.height_34_4 / 2;
	v5 = v4 - GetLetterHeight_6FC30();
	v30 = DrawScrollDialog_7BF20(&a3x->scroll_26);
	if (a3x->scroll_26.scrollPos_36_5 <= a3x->scroll_26.height_34_4 / 2)
		goto LABEL_15;
	menuState_17DE38.playerNameLength_17DEF0 = strlen((const char*)menuState_17DE38.playerNameText_17DE50);
	if ((unsigned __int8)menuState_17DE38.keyScancode_17DF10 < 0xFu)
	{
		if (menuState_17DE38.keyScancode_17DF10 != 14)
		{
		LABEL_9:
			if ((unsigned __int16)menuState_17DE38.playerNameLength_17DEF0 < 0xCu && menuState_17DE38.lastKeyChar_17DF11 && IsAlphanumericOrSpace_7C200(menuState_17DE38.lastKeyChar_17DF11))
			{
				v7 = (char*)(menuState_17DE38.playerNameText_17DE50 + menuState_17DE38.playerNameLength_17DEF0);
				v8 = menuState_17DE38.lastKeyChar_17DF11;
				v7[1] = 0;
				*v7 = v8;
			}
			goto LABEL_13;
		}
	LABEL_7:
		if (menuState_17DE38.playerNameLength_17DEF0)
		{
			v6 = (const char*)menuState_17DE38.playerNameText_17DE50;
			*(x_BYTE*)(strlen((const char*)menuState_17DE38.playerNameText_17DE50) + menuState_17DE38.playerNameText_17DE50 - 1) = 0;
			menuState_17DE38.playerNameLength_17DEF0 = strlen(v6);
		}
		goto LABEL_13;
	}
	if ((unsigned __int8)menuState_17DE38.keyScancode_17DF10 > 0xFu)
	{
		if (menuState_17DE38.keyScancode_17DF10 != 83)
			goto LABEL_9;
		goto LABEL_7;
	}
LABEL_13:
	_strupr(menuState_17DE38.playerNameText_17DE50);
	GetFont_6FC50(1);
	v9 = GetLetterHeight_6FC30() + v5;
	v10 = GetPaletteIndex_5BE80(menuState_17DE38.palette_17DE38x, 0x16u, 0x10u, 9u);
	v11 = DrawTextCentred_6FC80((char*)menuState_17DE38.playerNameText_17DE50, v29, v27, v9, v10);
	if ((v26 - menuState_17DE38.cursorBlinkTime_17DED0) / 0xCu >= 1)
	{
		v12 = GetPaletteIndex_5BE80(menuState_17DE38.palette_17DE38x, 0x16u, 0x10u, 9u);
		DrawText_2BC10((char*)"_", v11 + 2, v9, v12);
		menuState_17DE38.cursorBlinkTime_17DED0 = v26;
	}
LABEL_15:
	if (v30)
	{
		if (v30 == 1)
		{
			v13 = menuState_17DE38.playerNameText_17DE50;
			v14 = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].WizardName_0x39f_2BFA_12157;
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
			v17 = gameState_D41A4.player_name_57ar;//38cf89
			v18 = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].WizardName_0x39f_2BFA_12157;
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
			*gameState_D41A4.player_name_57ar = 0;//fixed gameState_D41A4.player_name_57 = 0;
		}
		v21 = menuState_17DE38.playerNameText_17DE50;
		v22 = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].WizardName_0x39f_2BFA_12157;
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
		ClearScrollDialogVars_7C020(&a3x->scroll_26);
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
int EditTextField_7F6A0(int  /*a1*/, int  /*a2*/, __int16 a3, __int16 a4, char* a5, int8_t* a6, unsigned __int8 a7)//2606a0
{
	uint8_t v7; // al
	__int16 v8; // si
	int v10; // [esp+0h] [ebp-4h]

	v10 = j___clock();
	v7 = menuState_17DE38.keyScancode_17DF10;
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
			if (*a6 < 0xFu && menuState_17DE38.lastKeyChar_17DF11 && IsAlphanumericOrSpace_7C200(menuState_17DE38.lastKeyChar_17DF11))
			{
				a5[(unsigned __int8)*a6] = menuState_17DE38.lastKeyChar_17DF11;
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
	if ((v10 - menuState_17DE38.cursorBlinkTime_17DED0) / 0xCu >= 1)
	{
		v8 = GetLetterWidth_6FC10();
		DrawText_2BC10((char*)"_", a3 + v8 * strlen(a5), a4, a7);
		menuState_17DE38.cursorBlinkTime_17DED0 = v10;
	}
	return 0;
}
// 98786: using guessed type int /*__fastcall*/ j___clock(x_DWORD, x_DWORD, x_DWORD);
// 9A20A: using guessed type x_DWORD _strupr_s(x_DWORD);
// 17DED0: using guessed type int x_DWORD_17DED0;
// 17DF10: using guessed type char x_BYTE_17DF10_get_key_scancode;
// 17DF11: using guessed type char x_BYTE_17DF11_last_key_status;

//----- (00079160) --------------------------------------------------------
char SetJoystickDialog_79160(type_menuButtons_E1F84* locEvent)//25a160
{
	int locY1;
	int tempJoySetting1;
	unsigned int tempJoySetting2;
	type_menuButtons_E1F84 locRegion;
	int locX1;
	char result;
	int locX2;
	char scrollState;
	unsigned __int8 color2;
	unsigned __int8 color1;

	int lastSelected = 0;
	result = 0;
	color1 = GetPaletteIndex_5BE80(menuState_17DE38.palette_17DE38x, 0x3Fu, 0x3Fu, 0x3Fu);
	color2 = GetPaletteIndex_5BE80(menuState_17DE38.palette_17DE38x, 0, 0, 0);
	switch (locEvent->active_25)
	{
	case 0:
	{
		locX1 = locEvent->scroll_26.x1_26_0 + 10;
		locX2 = locX1 + menuSprites_17DED4[72].width_4 - 22;
		locY1 = locEvent->scroll_26.y1_28_1 + locEvent->scroll_26.height_34_4 / 2 - 2 * GetLetterHeight_6FC30();
		scrollState = DrawScrollDialog_7BF20(&locEvent->scroll_26);
		if (locEvent->scroll_26.scrollPos_36_5 >= locEvent->scroll_26.height_34_4)
		{
			GetFont_6FC50(1);
			for (int i = 0; joystickDialogItems_E2590[i].textIndex_12; i++)
			{
				if (joystickDialogItems_E2590[i].state_14)
				{
					GetFont_6FC50(1);
					DrawTextCentred_6FC80(langStrings_E9C4C[joystickDialogItems_E2590[i].textIndex_12], locX1, locX2, locY1, color1);
					lastSelected = i;
				}
				else
				{
					GetFont_6FC50(1);
					DrawTextCentred_6FC80(langStrings_E9C4C[joystickDialogItems_E2590[i].textIndex_12], locX1, locX2, locY1, color2);
				}
				locY1 += GetLetterHeight_6FC30();
			}
			locY1 = locEvent->scroll_26.y1_28_1 + locEvent->scroll_26.height_34_4 / 2 - 2 * GetLetterHeight_6FC30();
			locRegion.xmin_10 = locX1;
			locRegion.sizex_14 = menuSprites_17DED4[72].width_4;
			locRegion.sizey_16 = GetLetterHeight_6FC30();
			for (int i = 0; joystickDialogItems_E2590[i].textIndex_12; i++)
			{
				locRegion.ymin_12 = locY1;
				if (InRegion_7B200(&locRegion, menuState_17DE38.mouseX_17DEE4, menuState_17DE38.mouseY_17DEE6))
				{
					if ((menuState_17DE38.mouseButtons_17DEEE & 1) != 0)
					{
						if (&joystickDialogItems_E2590[lastSelected].posX_0)
							joystickDialogItems_E2590[lastSelected].state_14 = 0;
						joystickDialogItems_E2590[i].state_14 = 1;
					}
					GetFont_6FC50(1);
					DrawTextCentred_6FC80(langStrings_E9C4C[joystickDialogItems_E2590[i].textIndex_12], locX1, locX2, locY1, color1);
				}
				locY1 += GetLetterHeight_6FC30();
			}
		}
		if (scrollState)
		{
			for (int i = 0; joystickDialogItems_E2590[i].textIndex_12; i++)
			{
				if (joystickDialogItems_E2590[i].state_14)
				{
					lastSelected = i;
					break;
				}
				locY1 += GetLetterHeight_6FC30();
			}
			if (scrollState != 1)
			{
				switch (inputState_18058C.joystickMode_1805C2)
				{
				case 4:
				case 11:
					inputState_18058C.joystickMode_1805C2 = 7;
					break;
				case 8:
					inputState_18058C.joystickMode_1805C2 = 1;
					break;
				case 9:
					inputState_18058C.joystickMode_1805C2 = 2;
					break;
				default:
					result = 1;
					ClearScrollDialogVars_7C020(&locEvent->scroll_26);
					return result;
				}
				result = 1;
				ClearScrollDialogVars_7C020(&locEvent->scroll_26);
				return result;
			}
			result = 1;
			tempJoySetting1 = inputState_18058C.joystickMode_1805C2;
			if (!joystickDialogItems_E2590[lastSelected].posX_0)
			{
				ClearScrollDialogVars_7C020(&locEvent->scroll_26);
				return result;
			}
			tempJoySetting2 = inputState_18058C.joystickMode_1805C2 - 1;
			if (joystickDialogItems_E2590[lastSelected].type_16 == 2)
			{
				if (ApplySetting_89B60(4u) == -1)
				{
					inputState_18058C.joystickMode_1805C2 = tempJoySetting1;
				}
				else if (tempJoySetting2 <= 0xC)
				{
					switch (tempJoySetting1)
					{
					case 1:
					case 12:
						inputState_18058C.joystickMode_1805C2 = 8;
						break;
					case 2:
					case 13:
						inputState_18058C.joystickMode_1805C2 = 9;
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
						inputState_18058C.joystickMode_1805C2 = 4;
						break;
					}
				}
				result = 1;
				ClearScrollDialogVars_7C020(&locEvent->scroll_26);
				return result;
			}
			if (joystickDialogItems_E2590[lastSelected].type_16 != 1)
			{
				ClearScrollDialogVars_7C020(&locEvent->scroll_26);
				return result;
			}
			if (ApplySetting_89B60(0xBu) != -1)
			{
				if (tempJoySetting2 <= 8)
				{
					switch (tempJoySetting1)
					{
					case 1:
					case 8:
						inputState_18058C.joystickMode_1805C2 = 12;
						break;
					case 2:
					case 9:
						inputState_18058C.joystickMode_1805C2 = 13;
						break;
					case 3:
					case 5:
					case 6:
						break;
					case 4:
					case 7:
						inputState_18058C.joystickMode_1805C2 = 11;
						break;
					}
				}
				locEvent->active_25 = 1;
				result = 0;
				ClearScrollDialogVars_7C020(&locEvent->scroll_26);
				return result;
			}
			inputState_18058C.joystickMode_1805C2 = tempJoySetting1;
			ClearScrollDialogVars_7C020(&locEvent->scroll_26);
		}
		return result;
	}
	case 1:
	{
		locEvent->scroll_26.textIndex_38_6 = 439;
		locEvent->scroll_26.nocl_42_8 = 0;
		break;
	}
	case 2:
	{
		locEvent->scroll_26.textIndex_38_6 = 439;
		break;
	}
	}
	DrawScrollDialog_7BF20(&locEvent->scroll_26);
	if (locEvent->scroll_26.scrollPos_36_5 < locEvent->scroll_26.height_34_4)
		return result;
	if (locEvent->active_25 < 2u)
	{
		if (locEvent->active_25 == 1)
		{
			locEvent->active_25 = 2;
			return result;
		}
		return result;
	}
	if (locEvent->active_25 > 2u)
	{
		if (locEvent->active_25 != 3)
			return result;
		locEvent->active_25 = 0;
		locEvent->scroll_26.textIndex_38_6 = 411;
		locEvent->scroll_26.nocl_42_8 = 3;
		ClearScrollDialogVars_7C020(&locEvent->scroll_26);
		return 1;
	}
	menuState_17DE38.keyScancode_17DF10 = 0;
	WaitForButtonPressAndRelease_8C0E0((unsigned __int8(*)(signed int))GetMouseAndKeyboardEvents_7A060);
	if (menuState_17DE38.keyScancode_17DF10 == 1)
	{
		if (inputState_18058C.joystickMode_1805C2 < 0xCu)
		{
			if (inputState_18058C.joystickMode_1805C2 == 11)
				inputState_18058C.joystickMode_1805C2 = 7;
		}
		else if (inputState_18058C.joystickMode_1805C2 <= 0xCu)
		{
			inputState_18058C.joystickMode_1805C2 = 1;
		}
		else if (inputState_18058C.joystickMode_1805C2 == 13)
		{
			inputState_18058C.joystickMode_1805C2 = 2;
		}
	}
	locEvent->active_25++;
	return result;
}


//----- (0007CCF0) --------------------------------------------------------
int MultiplayerPreviousLevel_7CCF0()//25dcf0
{
	if (menuState_17DE38.netSessions_17DE68x[menuState_17DE38.serverIndex_17DEFC].selectedLevel_10 > 50)
	{
		menuState_17DE38.netSessions_17DE68x[menuState_17DE38.serverIndex_17DEFC].selectedLevel_10--;
		menuState_17DE38.netSessions_17DE68x[menuState_17DE38.serverIndex_17DEFC].action_9 = 6;
	}
	return 0;
}

//----- (0007CD30) --------------------------------------------------------
int MultiplayerNextLevel_7CD30()//25dd30
{
	char* v0 = (char*)(LevelsNames_D9204)[1 + menuState_17DE38.netSessions_17DE68x[menuState_17DE38.serverIndex_17DEFC].selectedLevel_10];
	if (v0[0] && v0[0] != 48)
	{
		menuState_17DE38.netSessions_17DE68x[menuState_17DE38.serverIndex_17DEFC].selectedLevel_10++;
		menuState_17DE38.netSessions_17DE68x[menuState_17DE38.serverIndex_17DEFC].action_9 = 6;
	}
	return 0;
}

//----- (0007CCA0) --------------------------------------------------------
int MultiplayerExit_7CCA0()//25dca0
{
	if (menuState_17DE38.serverIndex_17DEFC == GetIndexNetwork2_74515() && menuState_17DE38.netPlayerCount_17DEFE == 1 || menuState_17DE38.serverIndex_17DEFC != GetIndexNetwork2_74515())
		menuState_17DE38.netSessions_17DE68x[menuState_17DE38.serverIndex_17DEFC].action_9 = 1;
	return 0;
}

//----- (0007CDA0) --------------------------------------------------------
int MultiplayerPlayLevel_7CDA0()//25dda0
{
	menuState_17DE38.netSessions_17DE68x[menuState_17DE38.serverIndex_17DEFC].action_9 = 5;
	return 0;
}

//----- (0007CBF0) --------------------------------------------------------
char MultiplayerPreviousWizard_7CBF0()//25dbf0 //change to previous wizard
{
	menuState_17DE38.x_WORD_17DEF8 = 0;
	menuState_17DE38.netSessions_17DE68x[menuState_17DE38.serverIndex_17DEFC].action_9 = 3;
	menuState_17DE38.palMulti_17DF02 = 0;
	menuState_17DE38.colourMenuState_17DEF6 = 2;
	menuState_17DE38.prevWizardIndex_17DEF2 = menuState_17DE38.wizardIndex_17DEF4;
	return 0;
}

//----- (0007CC40) --------------------------------------------------------
char MultiplayerNextWizard_7CC40()//25dc40 //change to next wizard
{
	menuState_17DE38.palMulti_17DF02 = 0;
	menuState_17DE38.x_WORD_17DEF8 = 1;
	menuState_17DE38.netSessions_17DE68x[menuState_17DE38.serverIndex_17DEFC].action_9 = 4;
	menuState_17DE38.colourMenuState_17DEF6 = 2;
	menuState_17DE38.prevWizardIndex_17DEF2 = menuState_17DE38.wizardIndex_17DEF4;
	return 0;
}

//----- (0007DE80) --------------------------------------------------------
char MultiplayerMenu_7DE80(type_menuButtons_E1F84* a2x)//25ee80
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
	a2x->state_4 = 0;
	// Confirm the session-number dialog once and go straight on to the level selection.
	if (CommandLineParams.AutoTest()) {
		static int autoSessionConfirmedFor = -1;
		if (autoSessionConfirmedFor != g_autotest_match) {
			autoSessionConfirmedFor = g_autotest_match;
			// A fixed number, not whatever the dialog happens to be showing.
			//
			// It used to take selectedSession_E29DF, which is not a session number at all:
			// MenusAndIntros.cpp:585 copies it from skipScreen_D41AD, the intro-skip
			// flag, so it holds 0 or 1 depending on how the intros happened to be got rid of.
			// While every instance ended up with the same value that was merely untidy - they
			// all joined session 1 and the tests passed.  Measured when it stopped agreeing:
			// the host confirmed session 0 and the client session 1, so they were in different
			// sessions, neither ever appeared in the other's player list, no CALL was made, and
			// both sat in the lobby to the end of the run reporting "myIdx=0".  Every scenario
			// failed with 0 exchanges and nothing in the transport was wrong.
			//
			// Nothing here depends on WHICH session is used, only on all instances naming the
			// same one, so it is pinned.  Also written back to the byte the dialog prints, so
			// what is on screen matches what was joined.
			const uint8_t autoSession = 0;
			selectedSession_E29DF = (char)autoSession;
			if (CommandLineParams.DoNetworkDebug())
				debug_net_printf("AUTOTEST: confirming session %d (match %d)\n", (int)autoSession, g_autotest_match);
			x_WORD_E131A = 0;
			menuState_17DE38.networkSession_17DEFA = autoSession;
			ResetMouse_7B5A0();
			a2x->state_4 = NetworkSessionLobby_77680() != 0;
			return 1;
		}
	}
	v3 = j___clock();
	GetFont_6FC50(1);
	v22 = a2x->scroll_26.x1_26_0 + 10;
	v20 = v22 + menuSprites_17DED4[72].width_4 - 22;
	v8 = a2x->scroll_26.height_34_4 / 2 + a2x->scroll_26.y1_28_1 - GetLetterHeight_6FC30();
	v23 = DrawScrollDialog_7BF20(&a2x->scroll_26);

	//Skip Game Number entry
	v23 = 1;

	if (a2x->scroll_26.scrollPos_36_5 > a2x->scroll_26.height_34_4 / 2)
	{
		if ((unsigned __int8)menuState_17DE38.keyScancode_17DF10 != 0xFu)
		{
			if ((unsigned __int8)menuState_17DE38.keyScancode_17DF10 < 0xFu)
			{
				v11 = menuState_17DE38.keyScancode_17DF10 == 14;
			}
			if ((unsigned __int8)menuState_17DE38.keyScancode_17DF10 > 0xFu)
			{
				v11 = menuState_17DE38.keyScancode_17DF10 == 83;
			}
			if (!v11
				&& (unsigned __int16)menuState_17DE38.playerNameLength_17DEF0 < 0xCu
				&& (unsigned __int8)menuState_17DE38.lastKeyChar_17DF11 >= 0x30u
				&& (unsigned __int8)menuState_17DE38.lastKeyChar_17DF11 <= 0x39u)
			{
				selectedSession_E29DF = menuState_17DE38.lastKeyChar_17DF11 - '0';
			}
		}
		sprintf(printbuffer, "%d", (unsigned __int8)selectedSession_E29DF);
		GetFont_6FC50(1);
		v12 = GetLetterHeight_6FC30() + v8;
		v14 = DrawTextCentred_6FC80(printbuffer, v22, v20, v12, GetPaletteIndex_5BE80(menuState_17DE38.palette_17DE38x, 0, 0, 0));
		if ((v3 - menuState_17DE38.cursorBlinkTime_17DED0) / 0xCu >= 1)
		{
			GetFont_6FC50(1);
			DrawText_2BC10((char*)"_", v14 - 2, v12, GetPaletteIndex_5BE80(menuState_17DE38.palette_17DE38x, 0, 0, 0));
			menuState_17DE38.cursorBlinkTime_17DED0 = v3;
		}
	}
	if (v23)
	{
		ClearScrollDialogVars_7C020(&a2x->scroll_26);
		if (v23 == 1)
		{
			x_WORD_E131A = 0;
			menuState_17DE38.networkSession_17DEFA = (unsigned __int8)selectedSession_E29DF;
			ResetMouse_7B5A0();
			if (1 == gameState_D41A4.showHelp_10)
				v24 = menuState_17DE38.showHelp_17DF13;
			SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]); //Set cursor to Null (Don't Draw)
			a2x->state_4 = NetworkSessionLobby_77680() != 0;
			if (gameState_D41A4.showHelp_10 == 1)
				menuState_17DE38.showHelp_17DF13 = v24;
		}
		else
		{
			selectedSession_E29DF = menuState_17DE38.networkSession_17DEFA;
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
	if (gameState_D41A4.showHelp_10 == 1)
		menuState_17DE38.showHelp_17DF13 = 0;
	GetFont_6FC50(1);
	if (!a1x->click_40_7)
	{
		v4 = a1x->scrollPos_36_5;
		if (v4)
		{
			if (v4 <= a1x->height_34_4 - 1)
				a1x->scrollPos_36_5 = v4 + 16;
			else
				a1x->scrollPos_36_5 = a1x->height_34_4;
		}
		else
		{
			a1x->scrollPos_36_5++;//next char
		}
		goto LABEL_14;
	}
	v5 = a1x->scrollPos_36_5 - 16;
	a1x->scrollPos_36_5 = v5;
	if (v5 >= 0)
	{
	LABEL_14:
		if (a1x->scrollPos_36_5 > GetLetterHeight_6FC30() + 10)
			v1 = langStrings_E9C4C[a1x->textIndex_38_6];
		//160 1a 21 2b2cce? exit game 3
		v7 = DrawScrollDialog2_7B660(a1x->x1_26_0, a1x->y1_28_1, a1x->scrollPos_36_5, a1x, v1, a1x->nocl_42_8 & 0xff);
		if (v7)
		{
			if (!a1x->click_40_7)
				a1x->click_40_7 = v7;
		}
		return 0;
	}
	a1x->scrollPos_36_5 = 0;
	//v6 = (int)x_D41A0_BYTEARRAY_4;
	int16_t result = a1x->click_40_7;
	a1x->click_40_7 = 0;
	if (gameState_D41A4.showHelp_10 == 1)
		menuState_17DE38.showHelp_17DF13 = 1;
	return result;
}
// D41A4: using guessed type int x_DWORD_D41A4;
// 17DF13: using guessed type char x_BYTE_17DF13;

//----- (0008C0E0) --------------------------------------------------------
void WaitForButtonPressAndRelease_8C0E0(unsigned __int8(/*__fastcall*/ *a1)(signed int))//26d0e0
{
	signed int v1=0; // eax
	signed int v2=0; // eax

	unk_180560x[26] = 0;
	while (!(unk_180560x[26] & 1))
	{
		v1 = ReadJoystick_8BBE0(unk_180560x);
		if (a1 && a1(v1))
			return;
	}
	while (unk_180560x[26] & 1)
	{
		v2 = ReadJoystick_8BBE0(unk_180560x);
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
	type_menuButtons_E1F84 v33x; // [esp+0h] [ebp-4Ch]
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
		DrawBitmap640_7C120(a1, a2, menuSprites_17DED4[menuState_17DE38.scrollBorderSprite_17DF06]);//unknown draw
		goto LABEL_31;
	}
	v45 = GetPaletteIndex_5BE80(menuState_17DE38.palette_17DE38x, 0x25u, 0x1Fu, 0x19u);
	//v38 = (signed __int16)a2;
	//v41 = (signed __int16)a1;
	DrawBitmap640_7C120(a1, a2, menuSprites_17DED4[menuState_17DE38.scrollBorderSprite_17DF06]);//top scroll border
	v7 = GetPaletteIndex_5BE80(menuState_17DE38.palette_17DE38x, 0x2Au, 0x24u, 0x1Du);
	DrawTextBackground_7C140(
		(a1 + 10),
		a2 + menuSprites_17DED4[menuState_17DE38.scrollBorderSprite_17DF06].height_5 - 2,
		menuSprites_17DED4[menuState_17DE38.scrollBorderSprite_17DF06].width_4 - 22,
		a3,
		v7);

	DrawLine_2BD10(a1 + 10, a2, a1 + 10, a3 + a2, v45);
	//v8 = menuSprites_17DED4[x_WORD_17DF06].pointer;
	//v9 = a2 + menuSprites_17DED4[x_WORD_17DF06].sizey - 2;
	DrawLine_2BD10(a1 + menuSprites_17DED4[menuState_17DE38.scrollBorderSprite_17DF06].width_4 - 12, a2 + menuSprites_17DED4[menuState_17DE38.scrollBorderSprite_17DF06].height_5 - 2, a1 + menuSprites_17DED4[menuState_17DE38.scrollBorderSprite_17DF06].width_4 - 12, a2 + menuSprites_17DED4[menuState_17DE38.scrollBorderSprite_17DF06].height_5 - 2 + a3, v45); // to je ta linka, kterou je treba opravit
	DrawBitmap640_7C120(a1, a3 + a2, menuSprites_17DED4[menuState_17DE38.scrollBorderSprite_17DF06]);//bottom scroll border
	if (a4x->scrollPos_36_5 >= a4x->height_34_4)
	{
		switch (a6)
		{
		case 3:
			//v10 = a4[0];
			//v43 = v10 + 15;
			//v12 = 6 * x_WORD_17DF0C;
			//v13 = v12 + menuSprites_17DED4[0].pointer;
			//v11 = a4[4];
			//v12 = a4[1];
			//v14 = menuSprites_17DED4[x_WORD_17DF0C].sizey;
			//HIWORD(v14) = (signed __int16)a1 >> 15;
			//v15 = v11 + v12 - v14;
			v33x.ymin_12 = a4x->height_34_4 + a4x->y1_28_1 - menuSprites_17DED4[menuState_17DE38.okSprite_17DF0C].height_5;//12 - v35
			v33x.xmin_10 = a4x->x1_26_0 + 15;//10 - v34
			v33x.sizex_14 = menuSprites_17DED4[menuState_17DE38.okSprite_17DF0C].width_4;//14 adress 25c832
			v33x.sizey_16 = menuSprites_17DED4[menuState_17DE38.okSprite_17DF0C].height_5;//16
			if (InRegion_7B200(&v33x, menuState_17DE38.mouseX_17DEE4, menuState_17DE38.mouseY_17DEE6))//icon OK
			{
				DrawBitmap640_7C120(a4x->x1_26_0 + 15, v33x.ymin_12, menuSprites_17DED4[menuState_17DE38.okHoverSprite_17DF0E]);
				if (menuState_17DE38.mouseButtons_17DEEE & 1)
					v44 = 1;
			}
			else
			{
				DrawBitmap640_7C120(a4x->x1_26_0 + 15, v33x.ymin_12, menuSprites_17DED4[menuState_17DE38.okSprite_17DF0C]);
			}
			//v16 = 3 * x_WORD_17DF06;
			//v16 = menuSprites_17DED4[x_WORD_17DF06].sizex;
			//v6 = a4[0];
			v42 = menuSprites_17DED4[menuState_17DE38.scrollBorderSprite_17DF06].width_4 - 12 + a4x->x1_26_0;
			//v17 = menuSprites_17DED4[x_WORD_17DF06].pointer;
			//v16 = v42 - menuSprites_17DED4[x_WORD_17DF08].sizex;
			//v33[6] = v15;
			//v18 = v42 - menuSprites_17DED4[x_WORD_17DF08].sizex;
			v33x.xmin_10 = v42 - menuSprites_17DED4[menuState_17DE38.cancelSprite_17DF08].width_4;
			v33x.sizex_14 = menuSprites_17DED4[menuState_17DE38.cancelSprite_17DF08].width_4;
			//v39 = x_WORD_17DF0A;
			v33x.sizey_16 = menuSprites_17DED4[menuState_17DE38.cancelHoverSprite_17DF0A].height_5;
			if (InRegion_7B200(&v33x, menuState_17DE38.mouseX_17DEE4, menuState_17DE38.mouseY_17DEE6))//icon X - cancel
			{
				DrawBitmap640_7C120(v42 - menuSprites_17DED4[menuState_17DE38.cancelSprite_17DF08].width_4, v33x.ymin_12, menuSprites_17DED4[menuState_17DE38.cancelHoverSprite_17DF0A]);
				if (menuState_17DE38.mouseButtons_17DEEE & 1)
					v44 = 2;
			}
			else
			{
				DrawBitmap640_7C120(v42 - menuSprites_17DED4[menuState_17DE38.cancelSprite_17DF08].width_4, v33x.ymin_12, menuSprites_17DED4[menuState_17DE38.cancelSprite_17DF08]);//zlom
			}
			goto LABEL_23;
		case 1:
			//v19 = menuSprites_17DED4[x_WORD_17DF0C].pointer;
			v20 = a4x->x1_26_0 + 30;
			v21 = a4x->height_34_4 + a4x->y1_28_1;
			v22 = menuSprites_17DED4[menuState_17DE38.okSprite_17DF0C].height_5;
			v33x.xmin_10 = a4x->x1_26_0 + 30;
			v23 = v21 - v22;
			v33x.ymin_12 = v21 - v22;
			v33x.sizex_14 = menuSprites_17DED4[menuState_17DE38.okSprite_17DF0C].width_4;
			v33x.sizey_16 = menuSprites_17DED4[menuState_17DE38.okSprite_17DF0C].height_5;
			if (InRegion_7B200(&v33x, menuState_17DE38.mouseX_17DEE4, menuState_17DE38.mouseY_17DEE6))
			{
				DrawBitmap640_7C120(v20, v23, menuSprites_17DED4[menuState_17DE38.okHoverSprite_17DF0E]);
				if (menuState_17DE38.mouseButtons_17DEEE & 1)
					v44 = 1;
				goto LABEL_23;
			}
			//v24 = (int)(int)x_DWORD_17DED4;
			v24x = menuState_17DE38.okHoverSprite_17DF0E;
			break;
		case 2:
			v26 = 3 * menuState_17DE38.okSprite_17DF0C;
			v20 = a4x->x1_26_0 + 40;
			v26 = menuSprites_17DED4[menuState_17DE38.okSprite_17DF0C].height_5;
			v27 = a4x->height_34_4;
			v42 = v26;
			v23 = v27 + a4x->y1_28_1 - v26;
			v33x.ymin_12 = v27 + a4x->y1_28_1 - v26;
			//v39 = x_WORD_17DF08;
			//v28 = menuSprites_17DED4[x_WORD_17DF0C].pointer;
			v33x.xmin_10 = v20;
			v33x.sizex_14 = menuSprites_17DED4[menuState_17DE38.okSprite_17DF0C].width_4;
			v33x.sizey_16 = menuSprites_17DED4[menuState_17DE38.okSprite_17DF0C].height_5;
			if (InRegion_7B200(&v33x, menuState_17DE38.mouseX_17DEE4, menuState_17DE38.mouseY_17DEE6))
			{
				DrawBitmap640_7C120(v20, v23, menuSprites_17DED4[menuState_17DE38.cancelHoverSprite_17DF0A]);
				if (menuState_17DE38.mouseButtons_17DEEE & 1)
					v44 = 1;
				goto LABEL_23;
			}
			v24x = menuState_17DE38.cancelSprite_17DF08;
			//v25 = (int)x_DWORD_17DED4;
			break;
		default:
			goto LABEL_23;
		}
		DrawBitmap640_7C120(v20, v23, menuSprites_17DED4[v24x]);
	LABEL_23:
		if (!v44)
		{
			if (menuState_17DE38.keyScancode_17DF10 == 28)
			{
				v44 = 1;
			}
			else if (menuState_17DE38.keyScancode_17DF10 == 1)
			{
				v44 = 2;
			}
		}
		/*
		if (CommandLineParams.ModeNetwork()) {
			if (first_enter)
			{
				first_enter = false;
				v44 = 1;
			}
		}*/
	}
	GetFont_6FC50(1);
	//v29 = menuSprites_17DED4[x_WORD_17DF06].pointer;
	v30 = a1 + 10 + menuSprites_17DED4[menuState_17DE38.scrollBorderSprite_17DF06].width_4 - 22;
	//v29 = menuSprites_17DED4[x_WORD_17DF06].sizey;
	v46 = menuSprites_17DED4[menuState_17DE38.scrollBorderSprite_17DF06].height_5 + 2 + a2;
	if (a5)
	{
		v31 = GetPaletteIndex_5BE80(menuState_17DE38.palette_17DE38x, 0x16u, 0x10u, 9u);
		//"Exit Game" 16a 1c6 28 1 4c//adress 25cc27
		DrawTextWithBoarder_7FCB0(/*v30,*/ a5, a1 + 10, v30, v46, 1, v31, 0);
	}
LABEL_31:
	if (v44)
		PlaySample_8F100(0, 14, 127, 64, 0x64u, 0, 3u);
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
char NetworkSessionLobby_77680()//258680
{
	char result;
	int oldTimes[4];
	int time;
	int textIndex = 0;
	timeCounter_E2094 = minTimeCounter_E2096;
	memset(printbuffer, 0, 80);
	memset(oldTimes, 0, 4*sizeof(int));
	char timeState = 2;
	sprintf(printbuffer, "NETH%d", (unsigned __int16)menuState_17DE38.networkSession_17DEFA + 20);
	int8_t a3a = 0;
	int8_t a3b = 0;
	menuState_17DE38.serverIndex_17DEFC = NetworkInitConnection_7308F(printbuffer, 8);//-1 == false
	if (menuState_17DE38.serverIndex_17DEFC == -1)
	{
		SetCursor_8CD27(menuSprites_17DED4[39]);
		result = 0;
	}
	else
	{
		LoadHscreen_7A110(videoMode_180660, 7);
		menuState_17DE38.netPlayerCount_17DEFE = 0;
		SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]); //Set cursor to Null (Don't Draw)
		menuState_17DE38.fileOffset_17DEDC = 0;
		menuState_17DE38.mouseButtons_17DEEE = 0;
		ResetMouse_7B5A0();
		memset(menuState_17DE38.netSessions_17DE68x, 0, sizeof(type_BYTE_17DE68x) * 8);
		memset(printbuffer, 0, 80);
		menuState_17DE38.colourMenuState_17DEF6 = 5;
		menuState_17DE38.x_WORD_17DEEC = 0;
		SetCenterScreenForFlyAssistant_6EDB0();
		menuState_17DE38.mouseX_17DEE4 = 320;
		menuState_17DE38.mouseY_17DEE6 = 240;//test and fix it

		if (gameState_D41A4.levelnumber_43w >= 0x32u)
			menuState_17DE38.netSessions_17DE68x[menuState_17DE38.serverIndex_17DEFC].selectedLevel_10 = gameState_D41A4.levelnumber_43w;
		else
			menuState_17DE38.netSessions_17DE68x[menuState_17DE38.serverIndex_17DEFC].selectedLevel_10 = 50;

		// Let an automated run say which multiplayer level to play.  The lobby numbers them from
		// 50 - that is what the test above is about - so the first one a player sees is 50 and the
		// tenth is 59.  Asking for them by that ordinal keeps the command line readable and means
		// a test does not have to know the offset.
		//
		// Without this the run always plays the first level, and a level that has to be reached by
		// hand cannot be part of a repeatable test.  --set_level is no use here: it sets skipMenus
		// and drops the instance straight into a single-player level, past the lobby altogether.
		if (CommandLineParams.AutoTest() && CommandLineParams.AutoTestMpLevel() > 0)
		{
			const int wanted = 50 + CommandLineParams.AutoTestMpLevel() - 1;
			menuState_17DE38.netSessions_17DE68x[menuState_17DE38.serverIndex_17DEFC].selectedLevel_10 = (uint8_t)wanted;
			if (CommandLineParams.DoNetworkDebug())
				debug_net_printf("AUTOTEST: multiplayer level %d selected (level number %d)\n",
					CommandLineParams.AutoTestMpLevel(), wanted);
		}
		menuState_17DE38.netSessions_17DE68x[menuState_17DE38.serverIndex_17DEFC].action_9 = 2;
		menuState_17DE38.mouseButtons_17DEEE = 0;
		if (CommandLineParams.AutoTest())
			debug_net_printf("AUTOTEST: level selection reached, myIdx=%d token=%d server=%d\n",
				(int)menuState_17DE38.serverIndex_17DEFC, (int)GetIndexNetwork2_74515(), (int)Iam_server);
		while (!a3a)
		{
			// Once every player has joined, the host starts the level.
			if (CommandLineParams.AutoTest()) {
				static int   autoFrames = 0;
				static int   autoLevelStartedFor = -1;
				// A second match comes back through this screen, so the settle counters have
				// to start over with it - otherwise the level is rolled on the frame count
				// and connection history left behind by the match before.
				static int   autoMatchSeen = -1;
				if (autoMatchSeen != g_autotest_match) { autoMatchSeen = g_autotest_match; autoFrames = 0; }
				autoFrames++;
				const int autoNeedPlayers = CommandLineParams.AutoTestPlayers() > 0
					? CommandLineParams.AutoTestPlayers() : 2;
				// The peers must be connected, and STAY connected for a while, before the
				// level is rolled.  After a server hand-over the new server reaches its
				// player count at once - the slots are all still there - and a peer that has
				// just died still reads as connected for a few frames, until its NCB is
				// marked closed.  Starting on that broadcasts "start" to nobody: this node
				// goes in-game while the other survivor is left behind in the lobby, and from
				// then on they exchange different-sized records at each other.
				// Watch WHICH peers are connected, not how many: a hand-over swaps the old
				// server for the other survivor and leaves the count unchanged, so a counter
				// alone never dips and the wait it is supposed to impose never happens.
				static int  autoStableFrames = 0;
				static int  autoLastMask = -1;
				static bool autoWasServer = false;
				const int   autoMask = NetworkConnectedMask();
				int         autoConnected = 0;
				for (int b = 0; b < 8; b++) if (autoMask & (1 << b)) autoConnected++;

				// Becoming the server restarts the wait.  At the instant the role is taken
				// over nothing else has moved yet - the dead server still reads as connected
				// for another ~80 ms, so neither the count nor the mask has noticed - and the
				// level would be rolled before the survivors have found each other.
				if (Iam_server != autoWasServer) { autoWasServer = Iam_server; autoStableFrames = 0; }

				if (autoMask != autoLastMask) { autoLastMask = autoMask; autoStableFrames = 0; }
				else if (autoConnected >= autoNeedPlayers) autoStableFrames++;
				else autoStableFrames = 0;

				// ...and the count is not enough on its own.  It includes this node, and a peer
				// that has just been killed goes on being counted until its NCB is marked
				// closed, so "me and the player about to die" reaches the bar just as well as
				// "me and the player that will still be here".  Measured: with three players and
				// a bar of two, the host started 1.2 s after the middle player was killed, on a
				// count made up of itself and that corpse, while the survivor had not finished
				// its LISTEN - which NetworkCancelAll_7449C() then cancelled on the way into the
				// level.  The survivor was stranded in the lobby for the rest of the run.
				//
				// So require the peers by NAME: everybody the transport still lists has to be in
				// a session with us.  The list drops a killed node as soon as its control
				// connection closes, which is immediate, rather than after the five-second data
				// heartbeat - so this says "the players that are really here are all in" without
				// waiting for the corpse to be declared.
				const bool autoPeersAllIn = NetworkAllRosterPeersConnected();

				// Why the level is not being started, reported when any part of the answer changes.
				// Sitting in the lobby looks identical from outside whatever the reason is, and after
				// a hand-over there are four candidates.
				if (CommandLineParams.DoNetworkDebug())
				{
					static int lastWhy = -1;
					static long lastSaid = 0;
					const int why = (Iam_server ? 1 : 0) | (menuState_17DE38.netPlayerCount_17DEFE >= autoNeedPlayers ? 2 : 0)
						| (autoPeersAllIn ? 4 : 0) | (autoStableFrames > 180 ? 8 : 0)
						| (autoLevelStartedFor != g_autotest_match ? 16 : 0);
					// Also every two seconds, not only on change: silence has to mean "this loop is
					// not running" rather than "nothing moved", which is the difference between a
					// gate that refuses and a gate nobody asks.
					if (why != lastWhy || ((long)j___clock() - lastSaid) > 200)
					{
						lastWhy = why;
						lastSaid = (long)j___clock();
						debug_net_printf("GATE: server=%d players=%d(%d) peersAllIn=%d stable=%d notStarted=%d\n",
							(int)Iam_server, (int)menuState_17DE38.netPlayerCount_17DEFE, autoNeedPlayers,
							(int)autoPeersAllIn, (int)(autoStableFrames > 180),
							(int)(autoLevelStartedFor != g_autotest_match));
					}
				}

				if (Iam_server && autoLevelStartedFor != g_autotest_match
					&& menuState_17DE38.netPlayerCount_17DEFE >= autoNeedPlayers
					&& autoPeersAllIn
					&& autoStableFrames > 180 && autoFrames > 180) {
					autoLevelStartedFor = g_autotest_match;
					int me = menuState_17DE38.serverIndex_17DEFC;
					menuState_17DE38.netSessions_17DE68x[me].action_9 = 5;
					menuState_17DE38.netSessions_17DE68x[me].makeUpdate_0 = 1;
					if (CommandLineParams.DoNetworkDebug())
						debug_net_printf("AUTOTEST: host starts the level %d (match %d)\n",
							(int)menuState_17DE38.netSessions_17DE68x[menuState_17DE38.serverIndex_17DEFC].selectedLevel_10,
							g_autotest_match);

					// Which colour each slot ended up with.  Two wizards in one colour is a real
					// failure mode here - the table lives in the token holder's record, so a
					// hand-over can leave it empty and "the first free colour" then answers the same
					// for everybody.  Reported at the start so it can be judged from the log.
					if (CommandLineParams.DoNetworkDebug())
					{
						char colours[64] = { 0 };
						int at = 0;
						for (int c = 0; c < 8 && at < (int)sizeof(colours) - 8; c++)
							if (menuState_17DE38.netSessions_17DE68x[c].makeUpdate_0)
								at += snprintf(colours + at, sizeof(colours) - at, "s%d=c%d ", c,
									(int)menuState_17DE38.netSessions_17DE68x[GetIndexNetwork2_74515()].playerIndex_1[c]);
						debug_net_printf("AUTOTEST: colours %s\n", colours);
					}
				}
			}
			if (menuState_17DE38.keyScancode_17DF10 == 59)
			{
				gameState_D41A4.showHelp_10 = gameState_D41A4.showHelp_10 != 1;
				gameState_D41A4.setting_38402 = 1;
			}
			a3a = MultiplayerColourMenu_7C390();
			if (gameState_D41A4.showHelp_10 == 1)
			{
				if (timeState == 2)
				{
					time = j___clock();
					if ((time - oldTimes[0]) / 0x64u > 1)
					{
						textIndex++;
						if (!textBoxStr_E2570[textIndex].left_2)
							textIndex = 0;
						oldTimes[0] = time;
					}
					typeTextBoxtextBoxStr_E24BCx textBoxStr[2];
					memset(textBoxStr, 0, 2 * sizeof(typeTextBoxtextBoxStr_E24BCx));
					textBoxStr[0] = textBoxStr_E2570[textIndex];
					DrawTextBoxWithLine_7E840(textBoxStr, 21, 27);
				}
				else if (timeState == 3)
				{
					time = j___clock();
					if ((time - oldTimes[0]) / 0x64u > 1)
						timeState = 2;
				}
			}
			if (!a3b)
			{
				VgaPalFadeInFadeOut_90B27(menuState_17DE38.palette_17DE38x, 0x20u, 0);
				a3b = 1;
				SetCursor_8CD27(menuSprites_17DED4[15]);
			}
			UpdateNetInfo();

			if (videoMode_180660 & 1)
				VgaBlit320_90478();
			else
				VgaBlit640_75200(480);
			GetMouseAndKeyboardEvents_7A060();
		}
		FadeClearBlit_7B5D0();
		LoadAndDecompressDatFile_7AA70(0, 0, 0, 0);
		ResetMultiplayerPalette_7C710();
		result = 1;
	}
	return result;
}

//----- (0007B250) --------------------------------------------------------
bool DrawAndServe_7B250()//25c250
{
	typeTextBoxtextBoxStr_E24BCx textBoxStr[2];

	if (CommandLineParams.ModeRegressionsTestType()==0) {
		mainMenuButtons_E1BAC[0].callback_0 = 0x258350;
		mainMenuButtons_E1BAC[0].selected_8 = 1;
	}
	// Pick the network-game button once, on behalf of the tester.
	if (CommandLineParams.AutoTest()) {
		// Once per MATCH, not once per process: a second match comes back to this menu and
		// has to pick the network game again.
		static int autoNetworkPickedFor = -1;
		if (autoNetworkPickedFor != g_autotest_match) {
			autoNetworkPickedFor = g_autotest_match;
			mainMenuButtons_E1BAC[2].selected_8 = 1;
			if (CommandLineParams.DoNetworkDebug())
				debug_net_printf("AUTOTEST: entering network game (match %d)\n", g_autotest_match);
		}
	}

	for (int i = 0; mainMenuButtons_E1BAC[i].xmin_10; i++)
	{
		if (mainMenuButtons_E1BAC[i].selected_8 && mainMenuButtons_E1BAC[i].callback_0)
		{
			if (DrawAndServe_pre_sub_7B250(mainMenuButtons_E1BAC[i].callback_0, &mainMenuButtons_E1BAC[i]))
			{
				mainMenuButtons_E1BAC[i].selected_8 = 0;
				ResetMouse_7B5A0();
			}
			if (mainMenuButtons_E1BAC[i].state_4)
			{
				mainMenuButtons_E1BAC[i].selected_8 = 0;
				SetCenterScreenForFlyAssistant_6EDB0();
				LoadHscreen_7A110(videoMode_180660, 4);
				ResetMouse_7B5A0();
				SetCursor_8CD27(menuSprites_17DED4[39]);
				if (mainMenuButtons_E1BAC[i].state_4 == 2)
					mainMenuButtons_E1BAC[i].state_4 = 0;
				return 1;
			}
			return 0;
		}
	}
	//clear/set off_E1BAC
	int jx;
	for (jx = 0; mainMenuButtons_E1BAC[jx].xmin_10; jx++)//clear/set off_E1BAC
	{
		mainMenuButtons_E1BAC[jx].selected_8 = 0;
		mainMenuButtons_E1BAC[jx].gold_color_24 = 0;
		if (menuState_17DE38.keyScancode_17DF10 == 1 && mainMenuButtons_E1BAC[jx].id_22 == 11)
			mainMenuButtons_E1BAC[jx].selected_8 = 1;
	}
	if (menuState_17DE38.keyScancode_17DF10 == 1)
	{
		menuState_17DE38.keyScancode_17DF10 = 0;
	}
	else
	{
		for (jx = 0; mainMenuButtons_E1BAC[jx].xmin_10; jx++)
		{
			if (InRegion_7B200(&mainMenuButtons_E1BAC[jx], menuState_17DE38.mouseX_17DEE4, menuState_17DE38.mouseY_17DEE6)
				&& mainMenuButtons_E1BAC[jx].canSelect_23)
			{
				if (menuState_17DE38.mouseButtons_17DEEE & 1)
				{
					PlaySample_8F100(0, 14, 127, 64, 0x64u, 0, 3u);
					mainMenuButtons_E1BAC[jx].selected_8 = 1;
					ResetMouse_7B5A0();
				}
				else
				{
					mainMenuButtons_E1BAC[jx].gold_color_24 = 1;
					mainMenuState_17DBC6 = 1;
				}
				break;
			}
		}
	}
	if (gameState_D41A4.showHelp_10 != 1 || !menuState_17DE38.showHelp_17DF13 || m_ExitMenuLoop_E29DC)
		return 0;
	if (mainMenuState_17DBC6 == 2)
	{
		times_17DBB8[1] = j___clock();
		if ((times_17DBB8[1] - times_17DBB8[0]) / 0x64u > 1)
		{
			mainMenuTextIndex_17DBC4++;
			if (!textBoxStr_E25DC[mainMenuTextIndex_17DBC4].left_2)
				mainMenuTextIndex_17DBC4 = 0;
			times_17DBB8[0] = times_17DBB8[1];
		}
		memset(textBoxStr, 0, 36);
		textBoxStr[0] = textBoxStr_E25DC[mainMenuTextIndex_17DBC4];
		int index = 0;
		DrawTextBoxWithLine_7E840(textBoxStr, 80, 89);
		if (!mainMenuButtons_E1BAC[0].xmin_10)
			return 0;
		do
		{
			if (mainMenuButtons_E1BAC[index].id_22 == goldMenuItems_E25ED[mainMenuTextIndex_17DBC4].buttonId_0)
			{
				mainMenuButtons_E1BAC[index].gold_color_24 = 1;//turn on gold selection
				return 0;
			}
			index++;
		} while (mainMenuButtons_E1BAC[index].xmin_10);
		return false;
	}
	else
	{
		if (mainMenuState_17DBC6 == 3)
		{
			times_17DBB8[1] = j___clock();
			if ((times_17DBB8[1] - times_17DBB8[0]) / 0x64u > 1)
			{
				mainMenuState_17DBC6 = 2;
				return 0;
			}
			return 0;
		}
		int index2 = 0;
		if (textBoxStr_E25DC[index2].left_2)
		{
			while (textBoxStr_E25DC[index2].buttonId_17 != mainMenuButtons_E1BAC[jx].id_22)
			{
				index2++;
				if (!textBoxStr_E25DC[index2].left_2)
					return 0;
			}
			memset(textBoxStr, 0, 36);
			textBoxStr[0] = textBoxStr_E25DC[index2];
			DrawTextBoxWithLine_7E840(textBoxStr, 80, 89);
			int time = j___clock();
			times_17DBB8[1] = time;
			times_17DBB8[0] = time;
			mainMenuState_17DBC6 = 3;
			return false;
		}
		else
		{
			return false;
		}
	}
	return false;
}

//----- (0007C390) --------------------------------------------------------
signed int MultiplayerColourMenu_7C390()//25d390
{
	signed int result; // eax
	type_E1BAC_0x3c4* v1x;
	type_menuButtons_E1F84* v2x;
	int v3; // esi
	signed __int16 v4; // dx
	__int16 i; // ax
	uint8_t* v6x = 0; // [esp+0h] [ebp-8h]

	char dataPath[MAX_PATH];

	sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/SCREENS/HSCREEN0.DAT");

	v6x = smalltitBuffer_E9C38;
	smalltitBuffer_E9C38 = gameState_D41A4.pointer_0xE2_heapbuffer_226;
	menuState_17DE38.lastNetPlayerCount_17DF00 = menuState_17DE38.netPlayerCount_17DEFE;
	qmemcpy(oldNetPlayerColors_E1BA4, netPlayerColors_E1B9C, sizeof(oldNetPlayerColors_E1BA4));
	if (SetMultiplayerColors_7CE50())
	{
		smalltitBuffer_E9C38 = v6x;
		result = 1;
	}
	else
	{
		qmemcpy(netPlayerColors_E1B9C, &menuState_17DE38.netSessions_17DE68x[GetIndexNetwork2_74515()].playerIndex_1, sizeof(netPlayerColors_E1B9C));
		v1x = &colourMenuBackgrounds_E1E98[menuState_17DE38.wizardIndex_17DEF4];
		if (videoMode_180660 & 1)
			CopyScreen((void*)smalltitBuffer_E9C38, (void*)pdwScreenBuffer_351628, 320, 200);
		else
			CopyScreen((void*)smalltitBuffer_E9C38, (void*)pdwScreenBuffer_351628, 640, 480);
		switch (menuState_17DE38.colourMenuState_17DEF6)
		{
		case 1://change wizard image
			LoadAndDecompressDatFile_7AA70(dataPath, &smalltitBuffer_E9C38[307200], v1x->dataOffset_0, v1x->dataSize_4);
			LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.colorNextTable_17DE3C, v1x->paletteOffset_8, v1x->paletteSize_12);
			menuState_17DE38.palMulti_17DF02 = 0;
			menuState_17DE38.colourMenuState_17DEF6 = 3;
			v2x = multiplayerButtons_E1D64;
			v3 = HandleMultiplayerMenuButtons_7CB10();
			break;
		case 2://fadeout
			menuState_17DE38.palMulti_17DF02 += 16;
			PaletteMulti_7C9D0(menuState_17DE38.palMulti_17DF02);
			if (menuState_17DE38.palMulti_17DF02 < 0xFFu)
			{
				v2x = multiplayerButtons_E1D64;
				v3 = HandleMultiplayerMenuButtons_7CB10();
				break;
			}
			CleanRectByColor_7C7C0(smalltitBuffer_E9C38);
			menuState_17DE38.colourMenuState_17DEF6 = 1;
			v2x = multiplayerButtons_E1D64;
			v3 = HandleMultiplayerMenuButtons_7CB10();
			break;
		case 3://fadein
			RestoreMenuButtonBackground_7C720(menuState_17DE38.wizardIndex_17DEF4, &smalltitBuffer_E9C38[307200]);
			menuState_17DE38.palMulti_17DF02 += 16;
			PaletteCopy_7C800(menuState_17DE38.palMulti_17DF02);
			if (menuState_17DE38.palMulti_17DF02 <= 0xFEu)
			{
				v2x = multiplayerButtons_E1D64;
				v3 = HandleMultiplayerMenuButtons_7CB10();
				break;
			}
			menuState_17DE38.colourMenuState_17DEF6 = 4;
			v2x = multiplayerButtons_E1D64;
			v3 = HandleMultiplayerMenuButtons_7CB10();
			break;
		case 5://clean wizard and set level
			CleanRectByColor_7C7C0(smalltitBuffer_E9C38);
			menuState_17DE38.palMulti_17DF02 = 255;
			PaletteMulti_7C9D0(255);
			menuState_17DE38.colourMenuState_17DEF6 = 1;
			menuState_17DE38.netSessions_17DE68x[menuState_17DE38.serverIndex_17DEFC].selectedLevel_10 = menuState_17DE38.netSessions_17DE68x[GetIndexNetwork2_74515()].selectedLevel_10;
			v2x = multiplayerButtons_E1D64;
			v3 = HandleMultiplayerMenuButtons_7CB10();
			break;
		default:
			v2x = multiplayerButtons_E1D64;
			v3 = HandleMultiplayerMenuButtons_7CB10();
			break;
		}
		while (v2x->xmin_10)
		{
			if (v2x->canSelect_23 && v2x->gold_color_24)
				DrawBitmap640_7C120(v2x->xmin_10, v2x->ymin_12, menuSprites_17DED4[v2x->graySprite_21]);
			v2x++;
		}
		DrawTextBackground_7C140(382, 18, 16, 16, 0);
		sprintf((char* const)menuState_17DE38.playerNameText_17DE50, "%d", (unsigned __int16)menuState_17DE38.networkSession_17DEFA);
		DrawText_7FAE0((char*)menuState_17DE38.playerNameText_17DE50, 382, 398, 18, 0);
		DrawNetworkLevelName_7D380();
		SetMultiplayerColors_7D310();
		v4 = 0;
		for (i = 0; i < 8; i++)
		{
			if (oldNetPlayerColors_E1BA4[i] != netPlayerColors_E1B9C[i])
			{
				v4 = 1;
				break;
			}
		}
		if (v4 || menuState_17DE38.lastNetPlayerCount_17DF00 != menuState_17DE38.netPlayerCount_17DEFE)
			VgaPaletteInstall_41A90(menuState_17DE38.palette_17DE38x);
		smalltitBuffer_E9C38 = v6x;
		result = v3;
	}
	return result;
}

//----- (0007C710) --------------------------------------------------------
void ResetMultiplayerPalette_7C710()//25d710
{
	menuState_17DE38.palMulti_17DF02 = 0;
}

//----- (0007C720) --------------------------------------------------------
void RestoreMenuButtonBackground_7C720(unsigned __int8 a1, uint8_t* a2)//25d720
{
	type_E1BAC_0x3c4* v2x;
	__int16 v3; // bx
	__int16 v4; // dx
	__int16 v5; // cx
	__int16 v6; // ax

	v2x = &colourMenuBackgrounds_E1E98[a1];
	v3 = v2x->posY_18;// *((x_WORD*)v2 + 9);
	v4 = v2x->posX_16;//*((x_WORD*)v2 + 8);
	v5 = v2x->width_20;//*((x_WORD*)v2 + 10);
	v6 = v2x->height_22;//*((x_WORD*)v2 + 11);
	if (v3 + v6 > 480)
		v6 = 480 - v3;
	if (v5 + v4 > 640)
		v4 = v5 + v4 - 640;
	BlitTransparent640_85BF5(a2, smalltitBuffer_E9C38, v4, v3, v5, v6);
}

//----- (0007CB10) --------------------------------------------------------
int HandleMultiplayerMenuButtons_7CB10()//25db10
{
	int ix;
	int jx;
	int v3x;

	for (ix = 0; multiplayerButtons_E1D64[ix].xmin_10; ix++)
	{
		if (multiplayerButtons_E1D64[ix].selected_8 && multiplayerButtons_E1D64[ix].callback_0)
		{
			if (DrawAndServe_pre_sub_7B250(multiplayerButtons_E1D64[ix].callback_0, &multiplayerButtons_E1D64[ix]))
			{
				multiplayerButtons_E1D64[ix].selected_8 = 0;
				ResetMouse_7B5A0();
				return 1;
			}
			else
			{
				multiplayerButtons_E1D64[ix].selected_8 = 0;
				return 0;
			}
		}
	}
	for (jx = 0; multiplayerButtons_E1D64[jx].xmin_10; jx++)
	{
		multiplayerButtons_E1D64[jx].selected_8 = 0;
		multiplayerButtons_E1D64[jx].gold_color_24 = 0;
	}
	v3x = 0;
	if (!multiplayerButtons_E1D64[v3x].xmin_10)
		return 0;
	while (!InRegion_7B200(&multiplayerButtons_E1D64[v3x], menuState_17DE38.mouseX_17DEE4, menuState_17DE38.mouseY_17DEE6) || !multiplayerButtons_E1D64[v3x].canSelect_23)
	{
		v3x++;
		if (!multiplayerButtons_E1D64[v3x].xmin_10)
			return 0;
	}
	if (menuState_17DE38.mouseButtons_17DEEE & 1)
	{
		multiplayerButtons_E1D64[v3x].selected_8 = 1;
		ResetMouse_7B5A0();
		PlaySample_8F100(0, 14, 127, 64, 0x64u, 0, 3u);
	}
	else
	{
		multiplayerButtons_E1D64[v3x].gold_color_24 = 1;
	}
	return 0;
}

//----- (0007B200) --------------------------------------------------------
bool InRegion_7B200(type_menuButtons_E1F84* a1x, int16_t testx, int16_t testy)//25c200
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
		menuState_17DE38.palette_17DE38x[ic].red -= (v1 * menuState_17DE38.palette_17DE38x[ic].red) >> 8;
		menuState_17DE38.palette_17DE38x[ic].green -= (v1 * menuState_17DE38.palette_17DE38x[ic].green) >> 8;
		menuState_17DE38.palette_17DE38x[ic].blue -= (v1 * menuState_17DE38.palette_17DE38x[ic].blue) >> 8;
	}

	menuState_17DE38.palette_17DE38x[149].red -= (v1 * menuState_17DE38.palette_17DE38x[149].red) >> 8;
	menuState_17DE38.palette_17DE38x[149].green -= (v1 * menuState_17DE38.palette_17DE38x[149].green) >> 8;
	menuState_17DE38.palette_17DE38x[149].blue -= (v1 * menuState_17DE38.palette_17DE38x[149].blue) >> 8;

	menuState_17DE38.palette_17DE38x[148].red -= (v1 * menuState_17DE38.palette_17DE38x[148].red) >> 8;
	menuState_17DE38.palette_17DE38x[148].green -= (v1 * menuState_17DE38.palette_17DE38x[148].green) >> 8;
	menuState_17DE38.palette_17DE38x[148].blue -= (v1 * menuState_17DE38.palette_17DE38x[148].blue) >> 8;
	//WaitToScreenBeam_9A0FC();
	VgaPaletteInstall_41A90(menuState_17DE38.palette_17DE38x);
	fix_sub_9A0FC_wait_to_screen_beam();
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
	GetFont_6FC50(1);
	int v0 = menuState_17DE38.netSessions_17DE68x[menuState_17DE38.serverIndex_17DEFC].selectedLevel_10;
	sprintf(printbuffer, "%d. %s", v0 - 49, LevelsNames_D9204[v0]);
	DrawText_7FAE0(printbuffer, 246, 355, 14, 0);
}

//----- (0007C800) --------------------------------------------------------
void PaletteCopy_7C800(signed __int16 a1)//25d800
{
	signed __int16 v1 = a1;

	if (v1 > 255)
		v1 = 255;
	for (int v3c = 159; v3c < 255; v3c++)
	{
		if (menuState_17DE38.palette_17DE38x[v3c].red < menuState_17DE38.colorNextTable_17DE3C->c[v3c].red)
			menuState_17DE38.palette_17DE38x[v3c].red = (menuState_17DE38.colorNextTable_17DE3C->c[v3c].red * v1) >> 8;
		else
			menuState_17DE38.palette_17DE38x[v3c].red = menuState_17DE38.colorNextTable_17DE3C->c[v3c].red;
		if (menuState_17DE38.palette_17DE38x[v3c].green < menuState_17DE38.colorNextTable_17DE3C->c[v3c].green)
			menuState_17DE38.palette_17DE38x[v3c].green = (menuState_17DE38.colorNextTable_17DE3C->c[v3c].green * v1) >> 8;
		else
			menuState_17DE38.palette_17DE38x[v3c].green = menuState_17DE38.colorNextTable_17DE3C->c[v3c].green;
		if (menuState_17DE38.palette_17DE38x[v3c].blue < menuState_17DE38.colorNextTable_17DE3C->c[v3c].blue)
			menuState_17DE38.palette_17DE38x[v3c].blue = (menuState_17DE38.colorNextTable_17DE3C->c[v3c].blue * v1) >> 8;
		else
			menuState_17DE38.palette_17DE38x[v3c].blue = menuState_17DE38.colorNextTable_17DE3C->c[v3c].blue;
	}
	if (menuState_17DE38.palette_17DE38x[148].red < menuState_17DE38.colorNextTable_17DE3C->c[148].red)
		menuState_17DE38.palette_17DE38x[148].red = (menuState_17DE38.colorNextTable_17DE3C->c[148].red * v1) >> 8;
	else
		menuState_17DE38.palette_17DE38x[148].red = menuState_17DE38.colorNextTable_17DE3C->c[148].red;
	if (menuState_17DE38.palette_17DE38x[148].green < menuState_17DE38.colorNextTable_17DE3C->c[148].green)
		menuState_17DE38.palette_17DE38x[148].green = (menuState_17DE38.colorNextTable_17DE3C->c[148].green * v1) >> 8;
	else
		menuState_17DE38.palette_17DE38x[148].green = menuState_17DE38.colorNextTable_17DE3C->c[148].green;
	if (menuState_17DE38.palette_17DE38x[148].blue < menuState_17DE38.colorNextTable_17DE3C->c[148].blue)
		menuState_17DE38.palette_17DE38x[148].blue = (menuState_17DE38.colorNextTable_17DE3C->c[148].blue * v1) >> 8;
	else
		menuState_17DE38.palette_17DE38x[148].blue = menuState_17DE38.colorNextTable_17DE3C->c[148].blue;

	if (menuState_17DE38.palette_17DE38x[149].red < menuState_17DE38.colorNextTable_17DE3C->c[149].red)
		menuState_17DE38.palette_17DE38x[149].red = (menuState_17DE38.colorNextTable_17DE3C->c[149].red * v1) >> 8;
	else
		menuState_17DE38.palette_17DE38x[149].red = menuState_17DE38.colorNextTable_17DE3C->c[149].red;
	if (menuState_17DE38.palette_17DE38x[149].green < menuState_17DE38.colorNextTable_17DE3C->c[149].green)
		menuState_17DE38.palette_17DE38x[149].green = (menuState_17DE38.colorNextTable_17DE3C->c[149].green * v1) >> 8;
	else
		menuState_17DE38.palette_17DE38x[149].green = menuState_17DE38.colorNextTable_17DE3C->c[149].green;
	if (menuState_17DE38.palette_17DE38x[149].blue < menuState_17DE38.colorNextTable_17DE3C->c[149].blue)
		menuState_17DE38.palette_17DE38x[149].blue = (menuState_17DE38.colorNextTable_17DE3C->c[149].blue * v1) >> 8;
	else
		menuState_17DE38.palette_17DE38x[149].blue = menuState_17DE38.colorNextTable_17DE3C->c[149].blue;
	//WaitToScreenBeam_9A0FC();
	VgaPaletteInstall_41A90(menuState_17DE38.palette_17DE38x);
	fix_sub_9A0FC_wait_to_screen_beam();
}


//----- (000824E0) --------------------------------------------------------
Type_SecretMapScreenPortals_E2970* GetSecretAndActivedPortal2_824E0(__int16 a1)//2634e0
{
	int ri = 0;
	if (!secretMapScreenPortals_E2970[ri].activated_12)
		return 0;
	while (a1 != secretMapScreenPortals_E2970[ri].levelNumber_6)
	{
		ri++;
		if (!secretMapScreenPortals_E2970[ri].activated_12)
			return 0;
	}
	return &secretMapScreenPortals_E2970[ri];
}


//----- (00085BF5) --------------------------------------------------------
void BlitTransparent640_85BF5(uint8_t* a1, uint8_t* a2, int a3, int a4, int a5, int a6)//266bf5
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
	v7 = &a2[a3 + 640 * a4];  // a2 == smalltitBuffer_E9C38
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

//----- (00041BC0) --------------------------------------------------------
void ClearPauseMenuState_41BC0()//222bc0
{
	IsPauseMenuOpen_D47D9 = 0;
	savedScreenSize_D47D8 = 0;
}


//----- (0007A110) --------------------------------------------------------
void LoadHscreen_7A110(char a1, char a2)//25b110
{
	char dataPath[MAX_PATH];
	//int v2; // eax

	menuState_17DE38.palette_17DE38x = (TColor*)*xadatapald0dat2.colorPalette_var28;
	menuState_17DE38.x_WORD_17DEEC = 0;
	menuState_17DE38.screenBuffer_17DE40 = pdwScreenBuffer_351628;
	menuState_17DE38.fileHandle_17DEE0 = NULL;
	menuState_17DE38.fileOffset_17DEDC = 0;
	menuState_17DE38.menuSpritesData_17DE48c = gameState_D41A4.pointer_0xE2_heapbuffer_226;
	FadeClearBlit_7B5D0();//25c5d0

	sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/SCREENS/HSCREEN0.DAT");

	if (a1 == 8)
	{
		switch (a2)
		{
		case 4:
			menuState_17DE38.menuSpritesTabEnd_17DED8 = (bitmap_pos_struct2_t*)&gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A + 0x36f44];//(uint8_t*)&loc_36F44 + x_DWORD_17DE48c;
			menuState_17DE38.menuSpritesTab_17DED4 = (bitmap_pos_struct2_t*)&gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A + 0x36c44];//(uint8_t*)&loc_36CAA + x_DWORD_17DE48c;
			menuState_17DE38.menuData2_17DE4C = &gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A + 0x36f44 + 2];//(uint8_t*)&loc_36F44 + x_DWORD_17DE48c + 2;
			menuState_17DE38.fontData_17DE54 = &gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A + 0x36f44 + 16386];//(uint8_t*)&loc_36F44 + x_DWORD_17DE48c + 16386;
			menuState_17DE38.fontTabEnd_17DEC4 = (bitmap_pos_struct2_t*)&gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A + 0x36f44 + 24631];//(uint8_t*)&loc_36F44 + x_DWORD_17DE48c + 24631;
			menuState_17DE38.playerNameText_17DE50 = (char*)&gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A + 0x36f44 + 24631];//(uint8_t*)&loc_36F44 + x_DWORD_17DE48c + 24631;
			menuState_17DE38.fontTab_17DEC0 = (bitmap_pos_struct2_t*)&gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A + 0x36f44 + 22999];//(uint8_t*)&loc_36F44 + x_DWORD_17DE48c + 22999;
			qmemcpy(&gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A + 0x36f44 + 24631],
				//(char *)&loc_36F44 + x_DWORD_17DE48c + 24631,
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].WizardName_0x39f_2BFA_12157,
				0x20u);
			smalltitBuffer_E9C38 = (uint8_t*)menuState_17DE38.playerNameText_17DE50 + 32;
			LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.palette_17DE38x, 0, 768);
			LoadAndDecompressDatFile_7AA70(dataPath, smalltitBuffer_E9C38, menuState_17DE38.fileOffset_17DEDC, 168081);
			LoadAndDecompressDatFile_7AA70(dataPath, menuState_17DE38.menuSpritesData_17DE48c, menuState_17DE38.fileOffset_17DEDC, 102213);
			LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.menuSpritesTab_17DED4, menuState_17DE38.fileOffset_17DEDC, 411);//19b
			LoadAndDecompressDatFile_7AA70(dataPath, menuState_17DE38.fontData_17DE54, 0x13ACCA, 1226);
			LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.fontTab_17DEC0, 0x13B194, 548);//25ba70
			LoadAndDecompressDatFile_7AA70(0, 0, 0, 0);
			if (videoMode_180660 & 1)
				CreateIndexDatTabDoubled_98709(menuState_17DE38.menuSpritesTab_17DED4, menuState_17DE38.menuSpritesTabEnd_17DED8, menuState_17DE38.menuSpritesData_17DE48c, menuSprites_17DED4);
			else
				CreateIndexDatTab_9874D(menuState_17DE38.menuSpritesTab_17DED4, menuState_17DE38.menuSpritesTabEnd_17DED8, menuState_17DE38.menuSpritesData_17DE48c, menuSprites_17DED4);
			if (videoMode_180660 & 1)
				CreateIndexDatTabDoubled_98709(menuState_17DE38.fontTab_17DEC0, menuState_17DE38.fontTabEnd_17DEC4, menuState_17DE38.fontData_17DE54, fontSprites_17DEC0);
			else
				CreateIndexDatTab_9874D(menuState_17DE38.fontTab_17DEC0, menuState_17DE38.fontTabEnd_17DEC4, menuState_17DE38.fontData_17DE54, fontSprites_17DEC0);
			
			menuState_17DE38.scrollBorderSprite_17DF06 = 72;
			menuState_17DE38.cancelSprite_17DF08 = 68;
			menuState_17DE38.cancelHoverSprite_17DF0A = 69;
			menuState_17DE38.okSprite_17DF0C = 70;
			menuState_17DE38.okHoverSprite_17DF0E = 71;

			if (extendedFonts)
			{
				LoadFixedFonts(0, (char*)"4b");
				LoadFixedFonts(3, (char*)"FONT1");
			}
			break;
		case 6://adress 25b533
			menuState_17DE38.fontData_17DE54 = &gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A + 301787];//(uint8_t*)x_DWORD_17DE48c + 301787;//49adb * //44ffef
			menuState_17DE38.fontTabEnd_17DEC4 = (bitmap_pos_struct2_t*)&gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A + 316179];//(uint8_t*)x_DWORD_17DE48c + 316179;//4d313 //453827
			menuState_17DE38.font2Data_17DE58 = &gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A + 316179];//x_DWORD_17DE48c + 316179; * //453827
			menuState_17DE38.font2Tab_17DEC8 = (bitmap_pos_struct2_t*)&gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A + 322754];//x_DWORD_17DE48c + 322754; * //4551d6
			menuState_17DE38.font2TabEnd_17DECC = (bitmap_pos_struct2_t*)&gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A + 324380];//x_DWORD_17DE48c + 324380; //455830
			menuState_17DE38.menuSpritesTab_17DED4 = (bitmap_pos_struct2_t*)&gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A + 324380];//(uint8_t*)x_DWORD_17DE48c + 324380; *//455830
			menuState_17DE38.menuSpritesTabEnd_17DED8 = (bitmap_pos_struct2_t*)&gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A + 326258];//(uint8_t*)x_DWORD_17DE48c + 326258; //455f86
			menuState_17DE38.loadBuffer_17DE60 = menuState_17DE38.savedSmalltit_17DE44;//* //3f52a4
			menuState_17DE38.gameWorldMap_17DE64 = &gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A + 326258];//x_DWORD_17DE48c + 326258; * //455f86
			menuState_17DE38.colorNextTable_17DE3C = (TcolNext*)&gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A + 0x12C000 + 326258];//(uint8_t*)&unk_12C000 + x_DWORD_17DE48c + 326258;//* //581f86
			menuState_17DE38.fontTab_17DEC0 = (bitmap_pos_struct2_t*)&gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A + 314541];//(uint8_t*)x_DWORD_17DE48c + 314541; * //4531c1
			menuState_17DE38.borderBitmap_17DE5C = &gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A + 0x12C000 + 326258 + 0x4000];//x_DWORD_17DE3C + 0x4000;//ok *//585f86
			LoadAndDecompressDatFile_7AA70(dataPath, menuState_17DE38.fontData_17DE54, 0x1641FC, 1214);//*
			LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.fontTab_17DEC0, 0x1646BA, 589);
			LoadAndDecompressDatFile_7AA70(dataPath, menuState_17DE38.font2Data_17DE58, 0x164907, 1191);
			LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.font2Tab_17DEC8, 0x164DAE, 543);//xx
			LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.palette_17DE38x, 0x13A9CA, 768);//ok
			LoadAndDecompressDatFile_7AA70(dataPath, menuState_17DE38.gameWorldMap_17DE64, 0xB2C47, 0x87D83);
			LoadAndDecompressDatFile_7AA70(dataPath, menuState_17DE38.menuSpritesData_17DE48c, 0x783BD, 103577);//ok
			LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.menuSpritesTab_17DED4, 0x91856, 1027);
			LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.colorNextTable_17DE3C, 0x13B3B8, 6760);
			LoadAndDecompressDatFile_7AA70(dataPath, menuState_17DE38.loadBuffer_17DE60, 0x13CE20, 20581);
			LoadAndDecompressDatFile_7AA70(dataPath, menuState_17DE38.borderBitmap_17DE5C, 0x141E85, 13195);
			LoadAndDecompressDatFile_7AA70(0, 0, 0, 0);
			if (videoMode_180660 & 1)
			{
				CreateIndexDatTabDoubled_98709(menuState_17DE38.menuSpritesTab_17DED4, menuState_17DE38.menuSpritesTabEnd_17DED8, menuState_17DE38.menuSpritesData_17DE48c, menuSprites_17DED4);
				CreateIndexDatTabDoubled_98709(menuState_17DE38.fontTab_17DEC0, menuState_17DE38.fontTabEnd_17DEC4, menuState_17DE38.fontData_17DE54, fontSprites_17DEC0);//here
				CreateIndexDatTabDoubled_98709(menuState_17DE38.font2Tab_17DEC8, menuState_17DE38.font2TabEnd_17DECC, menuState_17DE38.font2Data_17DE58, font2Sprites_17DEC8);
			}
			else
			{
				CreateIndexDatTab_9874D(menuState_17DE38.menuSpritesTab_17DED4, menuState_17DE38.menuSpritesTabEnd_17DED8, menuState_17DE38.menuSpritesData_17DE48c, menuSprites_17DED4);
				CreateIndexDatTab_9874D(menuState_17DE38.fontTab_17DEC0, menuState_17DE38.fontTabEnd_17DEC4, menuState_17DE38.fontData_17DE54, fontSprites_17DEC0);//here
				//34eec0 34eec4 34ee54
				CreateIndexDatTab_9874D(menuState_17DE38.font2Tab_17DEC8, menuState_17DE38.font2TabEnd_17DECC, menuState_17DE38.font2Data_17DE58, font2Sprites_17DEC8);
			}

			menuState_17DE38.scrollBorderSprite_17DF06 = 254;
			menuState_17DE38.cancelSprite_17DF08 = 255;
			menuState_17DE38.cancelHoverSprite_17DF0A = 256;
			menuState_17DE38.okSprite_17DF0C = 257;
			menuState_17DE38.okHoverSprite_17DF0E = 258;
			//menuState_17DE38.x_DWORD_17DEC0 += 6;
			menuState_17DE38.fontTab_17DEC0++;
			fontSprites_17DEC0 = &fontSprites_17DEC0[1];

			if (extendedFonts)
			{
				LoadFixedFonts(0, (char*)"6b");
				LoadFixedFonts(1, (char*)"6c");
				LoadFixedFonts(3, (char*)"FONT1");
			}

			break;
		case 7:
			menuState_17DE38.menuSpritesData_17DE48c += 371200;
			menuState_17DE38.fontData_17DE54 = menuState_17DE38.menuSpritesData_17DE48c + 42428;
			menuState_17DE38.fontTabEnd_17DEC4 = (bitmap_pos_struct2_t*)(menuState_17DE38.menuSpritesData_17DE48c + 50673);
			menuState_17DE38.menuSpritesTab_17DED4 = (bitmap_pos_struct2_t*)(menuState_17DE38.menuSpritesData_17DE48c + 50673);
			menuState_17DE38.menuSpritesTabEnd_17DED8 = (bitmap_pos_struct2_t*)(menuState_17DE38.menuSpritesData_17DE48c + 50871);
			menuState_17DE38.colorNextTable_17DE3C = (TcolNext*)(menuState_17DE38.menuSpritesData_17DE48c + 50871);
			menuState_17DE38.menuData2_17DE4C = menuState_17DE38.menuSpritesData_17DE48c + 51639;
			menuState_17DE38.fontTab_17DEC0 = (bitmap_pos_struct2_t*)(menuState_17DE38.menuSpritesData_17DE48c + 49041);
			menuState_17DE38.playerNameText_17DE50 = (char*)menuState_17DE38.menuSpritesData_17DE48c + 68023;
			LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.palette_17DE38x, 0x42471, 768);
			LoadAndDecompressDatFile_7AA70(dataPath, gameState_D41A4.pointer_0xE2_heapbuffer_226, menuState_17DE38.fileOffset_17DEDC, 82233);
			LoadAndDecompressDatFile_7AA70(dataPath, menuState_17DE38.menuSpritesData_17DE48c, 0x568AA, 21216);
			LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.menuSpritesTab_17DED4, 375690, 148);
			LoadAndDecompressDatFile_7AA70(dataPath, menuState_17DE38.fontData_17DE54, 0x13ACCA, 1226);
			LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.fontTab_17DEC0, 0x13B194, 548);
			if (videoMode_180660 & 1)
			{
				CreateIndexDatTabDoubled_98709(menuState_17DE38.menuSpritesTab_17DED4, menuState_17DE38.menuSpritesTabEnd_17DED8, menuState_17DE38.menuSpritesData_17DE48c, menuSprites_17DED4);
				CreateIndexDatTabDoubled_98709(menuState_17DE38.fontTab_17DEC0, menuState_17DE38.fontTabEnd_17DEC4, menuState_17DE38.fontData_17DE54, fontSprites_17DEC0);
			}
			else
			{
				CreateIndexDatTab_9874D(menuState_17DE38.menuSpritesTab_17DED4, menuState_17DE38.menuSpritesTabEnd_17DED8, menuState_17DE38.menuSpritesData_17DE48c, menuSprites_17DED4);
				CreateIndexDatTab_9874D(menuState_17DE38.fontTab_17DEC0, menuState_17DE38.fontTabEnd_17DEC4, menuState_17DE38.fontData_17DE54, fontSprites_17DEC0);
			}
			if (extendedFonts)
			{
				LoadFixedFonts(0, (char*)"4b");
			}
			break;
		case 12:
			LoadAndDecompressDatFile_7AA70(dataPath, smalltitBuffer_E9C38, 0x91C59, 134382);
			LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.palette_17DE38x, 0xB2947, 768);
			break;
		case 14:
			/*
			34EE48->55CCE4[4D54A]

			*/
			//[gameState_D41A4.pointer_0xE2_heapbuffer_226+0x4D54A]

			menuState_17DE38.fontData_17DE54 = &gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A + 224426];
			menuState_17DE38.fontTabEnd_17DEC4 = (bitmap_pos_struct2_t*)&gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A + 232671];
			menuState_17DE38.menuSpritesTab_17DED4 = (bitmap_pos_struct2_t*)&gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A + 232671];
			menuState_17DE38.menuSpritesTabEnd_17DED8 = (bitmap_pos_struct2_t*)&gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A + 233337];
			menuState_17DE38.menuData2_17DE4C = &gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A + 233339];
			menuState_17DE38.playerNameText_17DE50 = (char*)&gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A + 249723];
			menuState_17DE38.fontTab_17DEC0 = (bitmap_pos_struct2_t*)&gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A + 231039];
			qmemcpy(
				&gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A + 249723],
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].WizardName_0x39f_2BFA_12157,
				0x20u);//fix it
			LoadAndDecompressDatFile_7AA70(dataPath, menuState_17DE38.fontData_17DE54, 0x13ACCA, 1226);
			LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.fontTab_17DEC0, 0x13B194, 548);
			LoadAndDecompressDatFile_7AA70(dataPath, &gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A], 168849, 102213);
			LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.menuSpritesTab_17DED4, 271062, 411);//19b
			LoadAndDecompressDatFile_7AA70(dataPath, gameState_D41A4.pointer_0xE2_heapbuffer_226, 0x91C59, 134382);
			LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.palette_17DE38x, 0xB2947, 768);
			if (videoMode_180660 & 1) {
				CreateIndexDatTabDoubled_98709(menuState_17DE38.menuSpritesTab_17DED4, menuState_17DE38.menuSpritesTabEnd_17DED8, &gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A], menuSprites_17DED4);
				CreateIndexDatTabDoubled_98709(menuState_17DE38.fontTab_17DEC0, menuState_17DE38.fontTabEnd_17DEC4, menuState_17DE38.fontData_17DE54, fontSprites_17DEC0);
			}
			else
			{
				CreateIndexDatTab_9874D(menuState_17DE38.menuSpritesTab_17DED4, menuState_17DE38.menuSpritesTabEnd_17DED8, &gameState_D41A4.pointer_0xE2_heapbuffer_226[0x4D54A], menuSprites_17DED4);
				CreateIndexDatTab_9874D(menuState_17DE38.fontTab_17DEC0, menuState_17DE38.fontTabEnd_17DEC4, menuState_17DE38.fontData_17DE54, fontSprites_17DEC0);
			}

			//menuSprites_17DED4[37]
			//-34eed4
			// 3dc74800d7c94800472c0b00050000004001c800
			// 48c73d - 000000000000020000005d5fc2220000
			//-34eed8
			// d7c94800
			//48c9d7(rozdil 29a) - 000000000000000000000000000000000
			//-34ee48
			//5e3a4500
			//453e5e - 6f005d3f3a13423a13200b

			//pote
			//-34eed4
			// 3dc74800d7c94800472c0b00050000004001c800
			// 48c73d - 5e3a45000000603a45

			//dale [0034eed4]->48c73d->5e3a4500 ->6f005d3f3a13
			//+ d2=48c80f->47ae48 -> 26c2c2c2c2

			//48ae47002633 //47ae48->26c2c2c2c2c2

			/*
			34ee48=354f78->5e3a45
			34eed4=354f70->48c73d->000000000000000000

			*/
			break;
		case 15:
			LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)smalltitBuffer_E9C38, 0x145210, 126188);
			LoadAndDecompressDatFile_7AA70(dataPath, (uint8_t*)menuState_17DE38.palette_17DE38x, 0x163EFC, 0x300);
			break;
		default:
			return;
		}
	}
}

//----- (0007AA70) --------------------------------------------------------
void LoadAndDecompressDatFile_7AA70(const char* path, uint8_t* filebuffer, int position, int lenght)//25ba70
{
	//FILE* result; // eax

	if (menuState_17DE38.fileHandle_17DEE0 != NULL || (path && (menuState_17DE38.fileHandle_17DEE0 = DataFileIO::CreateOrOpenFile(path, 512))))//path 0: only close (a missing file was never opened)
	{
		//result = x_DWORD_17DEE0_filedesc;
		if (menuState_17DE38.fileHandle_17DEE0 != NULL)
		{
			if (filebuffer)
			{
				DataFileIO::Seek(menuState_17DE38.fileHandle_17DEE0, position, 0);
				DataFileIO::Read(menuState_17DE38.fileHandle_17DEE0, filebuffer, lenght);
				FileDecompress_5C3D0(filebuffer, filebuffer);
				menuState_17DE38.fileOffset_17DEDC = lenght + position;//endbuffer
			}
			else
			{
				DataFileIO::Close(menuState_17DE38.fileHandle_17DEE0);
				menuState_17DE38.fileHandle_17DEE0 = NULL;
			}
		}
	}
	//return result;
}
// 17DEDC: using guessed type int x_DWORD_17DEDC;
// 17DEE0: using guessed type int x_DWORD_17DEE0_filedesc;

//----- (0007C020) --------------------------------------------------------
void ClearScrollDialogVars_7C020(type_str_word_26* a1x)//25d020
{
	if (a1x->word_30_2)
	{
		a1x->word_32_3 = 0;
		a1x->word_30_2 = 0;
	}
	a1x->scrollPos_36_5 = 0;
}

//----- (0007C050) --------------------------------------------------------
signed int ReadKeyboardKeysInMenu_7C050()//25d050
{
	uint8_t v2; // bl
	long v3; // eax
	long v5; // esi
	bool ctrl_or_alt_pressed; // zf
	signed int result; // eax

	if (CommandLineParams.DoShowDebugPerifery()) ShowPerifery();

	menuState_17DE38.keyScancode_17DF10 = 0;
	menuState_17DE38.lastKeyChar_17DF11 = 0;
	//a1h = 2;

	//fix - __asm { int     16h; KEYBOARD - GET SHIFT STATUS }
	v2 = VGA_get_shift_status();
	//a1 = 1;
	//v2 = a1;
	if (v2)
	{
		if (v2 & 1)//right_shift
		{
			menuState_17DE38.keyScancode_17DF10 = 54;
		}
		else if (v2 & 2)//left_shift
		{
			menuState_17DE38.keyScancode_17DF10 = 42;
		}
		else if (v2 & 4)//ctrl
		{
			menuState_17DE38.keyScancode_17DF10 = 29;
		}
		else if (v2 & 8)//alt
		{
			menuState_17DE38.keyScancode_17DF10 = 56;
		}
	}
	v3 = j___clock();
	v5 = v3;
	do
	{
		/*ctrl_or_alt_pressed = (v2 & 1) == 0;
		if ( !(v2 & 1) )
		{
		  ctrl_or_alt_pressed = (v2 & 2) == 0;
		  if ( !(v2 & 2) )
		  {
			ctrl_or_alt_pressed = (v2 & 4) == 0;
			if ( !(v2 & 4) )
			{
			  ctrl_or_alt_pressed = (v2 & 8) == 0;
			  if ( !(v2 & 8) )
				break;//if not ctrl or alt end
			}
		  }
		}
		BYTE1(v3) = 2;
		HIBYTE(v4) = 2;
		a1 = VGA_get_shift_status();
		//fix - __asm { int     16h; KEYBOARD - GET SHIFT STATUS }
		if (ctrl_or_alt_pressed)
		  HIBYTE(v4) = 1;
		v2 = v3;
		v3 = j___clock() - v5;*/
		if (!(v2 & 1) && !(v2 & 2) && !(v2 & 4) && !(v2 & 8))
			break;
		//v4h = 2;
		ctrl_or_alt_pressed = VGA_get_shift_status();
		//if (ctrl_or_alt_pressed)
			//v4h = 1;
		//v2 = v3;
		v3 = j___clock() - v5;
	} while (v3 <= 5);
	while (1)
	{
		result = CheckKeyboard_9A10A(/*v3*/);//adress 25d101
		//'a' - 1e61
		//'b' - 3062
		//'UP' - 4800
		//'0' - 5230
		//'ENTER' - 1c0d
		if (!result)
			break;
		//BYTE1(result) = 0;
		result = VGA_read_char_from_buffer();
		// fix - __asm { int     16h; KEYBOARD - READ CHAR FROM BUFFER, WAIT IF EMPTY }
		menuState_17DE38.lastKeyChar_17DF11 = result;
		v3 = result >> 8;
		menuState_17DE38.keyScancode_17DF10 = v3;
	}
	return result;
}
// 98786: using guessed type int /*__fastcall*/ j___clock(x_DWORD, x_DWORD, x_DWORD);
// 17DF10: using guessed type char x_BYTE_17DF10_get_key_scancode;
// 17DF11: using guessed type char x_BYTE_17DF11_last_key_status;

//----- (0007C200) --------------------------------------------------------
char IsAlphanumericOrSpace_7C200(unsigned __int8 a1)//25d200
{
	char result = 0;
	if (a1 == 32 || a1 >= 0x30u && a1 <= 0x39u || a1 >= 0x41u && a1 <= 0x5Au || a1 >= 0x61u && a1 <= 0x7Au)
		result = 1;
	return result;
}

//----- (0007E5A0) --------------------------------------------------------
bool DrawFrameAnim_7E5A0(__int16 posx, __int16 posy, Type_MapScreenPortals_E17CC* mapPortal, __int16 beginIndex, __int16 endIndex)//25f5a0
{
	int actTime = j___clock();
	bool result = false;
	if ((actTime - mapPortal->time_0) >> 3 >= 1)
	{
		if (mapPortal->spriteIndex_16 <= endIndex)
		{
			mapPortal->spriteIndex_16++;
		}
		else
		{
			result = true;
			mapPortal->spriteIndex_16 = beginIndex;
		}
		mapPortal->time_0 = actTime;
	}
	DrawBitmap640_7C120(mapPortal->portalPosX_12 - posx, mapPortal->portalPosY_14 - posy, menuSprites_17DED4[mapPortal->spriteIndex_16]);
	return result;
}

void DrawTextBox(std::string message, uint16_t left, uint16_t right, uint16_t top, int16_t borderColour)
{
	DrawTextWithBoarder_7FCB0((char*)message.c_str(), left, right, top, 0, 0, borderColour);
}

//----- (0007E840) --------------------------------------------------------
void DrawTextBoxWithLine_7E840(typeTextBoxtextBoxStr_E24BCx* testBoxStr, __int16 borderColor, __int16 lineColor)//25f840
{
	int i = 0;
	if (!testBoxStr[i].left_2)
		return;
	do
	{
		if (testBoxStr[i].left_2)
		{
			DrawBitmapLine_81360(testBoxStr[i].minx_6, testBoxStr[i].miny_8, testBoxStr[i].width_12, testBoxStr[i].maxy_14, lineColor);//262360
			DrawTextWithBoarder_7FCB0(langStrings_E9C4C[testBoxStr[i].textIndex_0], testBoxStr[i].left_2, (testBoxStr[i].left_2 + 180), testBoxStr[i].top_4, 0, 0, borderColor);//260cb0
			/*
			Save Current Game
			Exit Game
			Select Language
			Play MultiPlayer Game
			Play Game
			Configure Joystick
			Enter Network Session
			Attempting Connection
			Empty
			Play Game
			Enter Your Player Name
			Play MultiPlayer Game
			Load A Previous Game

			Configure Keyboard controls

			*/
		}
		i++;
	} while (testBoxStr[i].left_2);
}

//----- (000812D0) --------------------------------------------------------
int DrawMapObject_812D0(__int16 a1, __int16 a2)//2622d0
{
	uint8_t* temp_screen_buffer; // edi
	__int16 v3; // si
	__int16 v4; // bx
	int result; // eax

	//_disable();
	temp_screen_buffer = pdwScreenBuffer_351628;
	pdwScreenBuffer_351628 = menuState_17DE38.gameWorldMap_17DE64;
	v3 = screenHeight_180624;
	clipHeight_180644 = 960;
	screenHeight_180624 = 960;
	v4 = screenWidth_18062C;
	clipWidth_180648 = 1280;
	screenWidth_18062C = 1280;
	DrawBitmap640_7C120(a1, a2, menuSprites_17DED4[139]);
	screenWidth_18062C = v4;
	screenHeight_180624 = v3;
	clipWidth_180648 = v4;
	result = v3;
	pdwScreenBuffer_351628 = temp_screen_buffer;
	clipHeight_180644 = v3;
	//_enable();
	return result;
}

//----- (00081360) --------------------------------------------------------
void DrawBitmapLine_81360(int16_t minx, int16_t miny, int16_t maxx, int16_t maxy, __int16 a5)//262360
{
	int32_t v5; // edi
	int32_t v6; // esi
	int16_t v7; // bx
	int16_t v8; // ax
	//int v9; // ecx
	int32_t v10; // eax
	int32_t v11; // eax
	int32_t v12; // eax
	int32_t v13; // eax
	int32_t v14; // eax
	int32_t v15; // edx
	int32_t v17; // edx
	int32_t v18; // eax
	axis_3d v19x; // [esp+0h] [ebp-38h]
	//int16_t v20; // [esp+2h] [ebp-36h]
	axis_3d v21x; // [esp+8h] [ebp-30h]
	//int16_t v22; // [esp+Ah] [ebp-2Eh]
	//int v23; // [esp+10h] [ebp-28h]
	//int32_t v24; // [esp+14h] [ebp-24h]
	int32_t v25; // [esp+18h] [ebp-20h]
	int32_t v26; // [esp+1Ch] [ebp-1Ch]
	int32_t v27; // [esp+20h] [ebp-18h]
	uint32_t v28; // [esp+24h] [ebp-14h]
	int32_t v29; // [esp+28h] [ebp-10h]
	int32_t v30; // [esp+2Ch] [ebp-Ch]
	int32_t v31; // [esp+30h] [ebp-8h]
	int32_t v32; // [esp+34h] [ebp-4h]

	//fix it

	//loadfromsnapshot((char*)"0160-00262360", (uint8_t*)&a6, 0x354f6c, 1);
	  //354f10->35f418 +2->35012a .. 012a
	  //354f14->354f20 +2->350168 .. 0168
	  //0168-18a=3e
	  //354f10->35f418->ec0141 .. 0141
	  //354f14->354f20->012a018d .. 018d
	  //18d-141=4c
	//a3 = 0x00000141;
	//a1 = 0x0000018d;
	//a4 = 0x000000ec;
	//a2 = 0x0000012a;
	//loadfromsnapshot2((char*)"0160-00262360", (uint8_t*)x_DWORD_17DED4, 0x34eed4, 0x300);//3e
	//x_D41A0_BYTEARRAY_4[v9 + 4] = 0x0b;
	//fix it

	v5 = minx/* & 0xffff*/;
	v6 = miny/* & 0xffff*/;
	v21x.x = minx;
	v19x.x = maxx;
	v19x.y = maxy;
	v21x.y = miny/* & 0xffff*/;
	//v20 = maxy/* & 0xffff*/;
	v7 = 0;
	v29 = 0;
	v28 = 0;
	v8 = Maths::GetYawAngle_581E0(&v19x, &v21x);//2391E0
	/*minx &= 0xffff;
	miny &= 0xffff;
	maxx &= 0xffff;
	maxy &= 0xffff;*/
	if (v8 <= 1024 || v8 >= 1536)
	{
		if (v8 <= 1536 || v8 >= 2048)
		{
			if (v8 <= 2048 && v8 <= 0 || v8 >= 512)
			{
				if (v8 <= 512 || v8 >= 1024)
				{
					if (v8 != 2048 && v8)
					{
						switch (v8)
						{
						case 512:
							v7 = a5 - 3;
							break;
						case 1024:
							v7 = a5 - 5;
							break;
						case 1536:
							v7 = a5 + 3;
							break;
						}
					}
					else
					{
						v7 = a5 - 1;
					}
				}
				else
				{
					v7 = a5 - 4;
				}
			}
			else
			{
				v7 = a5 - 2;
			}
		}
		else
		{
			v7 = a5 + 2;
		}
	}
	else
	{
		v7 = a5 + 4;
	}
	//v24 = maxx - minx;
	//v9 = 6 * a5;
	v25 = maxy - miny;
	//v23 = x_DWORD_17DED4[v9 + 4];
	//6*a5
	//dosbox - 258
	//toto - 64
	v31 = 2 * abs((maxx - minx) / menuSprites_17DED4[a5].width_4);//find errors
	//v23 = x_DWORD_17DED4[v9 + 5];
	v30 = 2 * abs((maxy - miny) / menuSprites_17DED4[a5].height_5);
	v10 = maxx - minx;
	if ((maxx - minx) >= 0)
	{
		if (maxx != minx)
		{
			//HIWORD(v10) = HIWORD(x_DWORD_17DED4);
			v10 = menuSprites_17DED4[a5].width_4;
		}
	}
	else
	{
		//HIWORD(v11) = HIWORD(x_DWORD_17DED4);
		v11 = menuSprites_17DED4[a5].width_4;
		v10 = -v11;
	}
	v27 = v10;
	v12 = v25;
	if ((v25 & 0x8000u) == 0)
	{
		if (v25)
		{
			v12 = menuSprites_17DED4[a5].height_5;
		}
	}
	else
	{
		v13 = menuSprites_17DED4[a5].height_5;
		v12 = -v13;
	}
	v26 = v12;
	if (minx >= maxx)
	{
		if (minx > maxx)
			v29 = 1;
	}
	else
	{
		v29 = 2;
	}
	if (miny >= maxy)
	{
		if (miny > maxy)
			v28 = 1;
	}
	else
	{
		v28 = 2;
	}
	if (v31 <= v30)
	{
		v14 = v30;
		v15 = v31;
	}
	else
	{
		v14 = v31;
		v15 = v30;
	}
	v32 = v15 - (v14 >> 1);
	while (1)
	{
		if (v31 <= v30)
		{
			if (v6 == maxy)
			{
				DrawBitmap640_7C120(v5, maxy, menuSprites_17DED4[v7]);
				return;
			}
			if ((v32 & 0x8000u) == 0)
			{
				v5 += v27;
				v32 -= v30;
			}
			v17 = v32;
			v6 += v26;
			v18 = v31;
		}
		else
		{
			if (v5 == maxx)
			{
				DrawBitmap640_7C120(maxx, v6, menuSprites_17DED4[v7]);
				return;
			}
			if ((v32 & 0x8000u) == 0)
			{
				v6 += v26;
				v32 -= v31;
			}
			v17 = v32;
			v5 += v27;
			v18 = v30;
		}
		v32 = v18 + v17;
		if (v29 >= 1u)
		{
			if (v29 <= 1u)
			{
				if (v5 < maxx)
				{
					DrawBitmap640_7C120(maxx, maxy, menuSprites_17DED4[v7]);//draw ending
					//ma byt ec a 141
					return;
				}
			}
			else if (v29 == 2 && v5 > maxx)
			{
				DrawBitmap640_7C120(maxx, maxy, menuSprites_17DED4[v7]);
				return;
			}
		}
		if (v28 < 1u)
			goto LABEL_70;
		if (v28 > 1u)
			break;
		if (v6 < maxy)
		{
			DrawBitmap640_7C120(maxx, maxy, menuSprites_17DED4[v7]);
			return;
		}
	LABEL_70:
		//18d 12a 141 ec - 18d 12a 141 ec -1
		  //c4 26 11b 6a - c4 26 11b 6a -0 - chybi ocasek
		DrawBitmap640_7C120(v5, v6, menuSprites_17DED4[a5]);
	}
	if (v28 != 2 || v6 <= maxy)
		goto LABEL_70;
	DrawBitmap640_7C120(maxx, maxy, menuSprites_17DED4[v7]);//draw ending
}
// 17DED4: using guessed type int (int)x_DWORD_17DED4;

//----- (00081CA0) --------------------------------------------------------
void DrawAnimSprite_81CA0(__int16 posx, __int16 posy, type_x_BYTE_E25ED_db_str* a5x)//262ca0
{
	int time = j___clock();
	if (a5x->state_20 == 0)
	{
		if (!a5x->state_20)
		{
			a5x->state_20 = 2;
			a5x->time1_0 = time;
			a5x->time2_4 = time;
		}
	}
	else if (a5x->state_20 == 1)
	{
		if ((time - a5x->time2_4) >> 3 >= 1)
		{
			if (a5x->frameIndex_16 <= a5x->lastFrame_14 - 2)
			{
				a5x->frameIndex_16++;
			}
			else
			{
				if (a5x->playOnce_21 == 1)
				{
					a5x->state_20 = 0;
				}
				a5x->frameIndex_16 = a5x->firstFrame_12;
			}
			a5x->time2_4 = time;
		}
		DrawBitmap640_7C120(
			a5x->x1_8 - posx,
			a5x->y1_10 - posy,
			menuSprites_17DED4[a5x->frameIndex_16]);
		if (menuState_17DE38.mouseButtons_17DEEE & 1)
			a5x->frameIndex_16++;
	}
	else if (a5x->state_20 == 2)
	{
		time = (time - a5x->time1_0) / 0x64;
		if (time > a5x->time3_18)
		{
			time = a5x->time4_22;
			a5x->state_20 = 1;
			if (time != -1)
				PlaySample_8F100(
					0,
					a5x->wavIndex_24,
					(unsigned __int8)menuSoundVolume_E1324,
					64,
					0x64u,
					a5x->loopCount_28,
					a5x->playType_26);
		}
	}
}

//----- (000824B0) --------------------------------------------------------
Type_SecretMapScreenPortals_E2970* GetSecretAndActivedPortal_824B0(__int16 actLevel)//2634b0
{
	int ri = 0;
	if (!secretMapScreenPortals_E2970[ri].activated_12)
		return 0;
	while (actLevel != secretMapScreenPortals_E2970[ri].index_4)
	{
		ri++;
		if (!secretMapScreenPortals_E2970[ri].activated_12)
			return 0;
	}
	return &secretMapScreenPortals_E2970[ri];
}

//----- (00082AB0) --------------------------------------------------------
void ActivateSecretPortal_82AB0(unsigned __int8 a1)//263ab0
{
	//int v1; // eax
	int v2; // edx
	//int result; // eax
	//x_WORD *v4; // ebx
	__int16 i; // cx
	//int v6; // eax
	int v7; // edx

	if (a1 > 0x18u)
	{
		//v4 = x_WORD_E2970;
		int v4i = 0;
		for (i = 0; secretMapScreenPortals_E2970[v4i].activated_12; i++)
		{
			//result = a1;
			if (a1 == secretMapScreenPortals_E2970[v4i].levelNumber_6)
			{
				//v6 = (int)x_D41A0_BYTEARRAY_0;
				v7 = 5 * i;
				secretLevelScores_17DDBC[v7] = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.spellsCollectedPercent_377;
				secretLevelScores_17DDBC[1 + v7] = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.hitAccuracyPercent_381;
				secretLevelScores_17DDBC[2 + v7] = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.creaturesKilledPercent_373;
				secretLevelScores_17DDBC[3 + v7] = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.manaCollectedPercent_385;
				//result = *(x_DWORD *)(2124 * *(signed __int16 *)(x_D41A0_BYTEARRAY_0 + 12) + x_D41A0_BYTEARRAY_0 + 12621);
				secretLevelScores_17DDBC[4 + v7] = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.time_393;
			}
			v4i++;
			//v4 = (x_WORD *)((char *)v4 + 17);
		}
	}
	else
	{
		//v1 = (int)x_D41A0_BYTEARRAY_0;
		v2 = 5 * a1;
		levelScores_17DBC8[v2] = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.spellsCollectedPercent_377;
		levelScores_17DBC8[1 + v2] = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.hitAccuracyPercent_381;
		levelScores_17DBC8[2 + v2] = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.creaturesKilledPercent_373;
		levelScores_17DBC8[3 + v2] = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.manaCollectedPercent_385;
		//result = *(x_DWORD *)(2124 * *(signed __int16 *)(x_D41A0_BYTEARRAY_0 + 12) + x_D41A0_BYTEARRAY_0 + 12621);
		levelScores_17DBC8[4 + v2] = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.time_393;
	}
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// 17DBC8: using guessed type int x_DWORD_17DBC8[];
// 17DBCC: using guessed type int x_DWORD_17DBCC[];
// 17DBD0: using guessed type int x_DWORD_17DBD0[];
// 17DBD4: using guessed type int x_DWORD_17DBD4[];
// 17DDBC: using guessed type int x_DWORD_17DDBC[];
// 17DDC0: using guessed type int x_DWORD_17DDC0[];
// 17DDC4: using guessed type int x_DWORD_17DDC4[];
// 17DDC8: using guessed type int x_DWORD_17DDC8[];

//----- (00085CC3) --------------------------------------------------------
void DrawRoundFrame_85CC3(uint16_t* buffer)//266cc3
{
	uint16_t* v3; // esi
	uint8_t* v4; // edi
	int16_t v5; // edx
	unsigned __int16* v6; // ST04_4
	int16_t v7; // ST02_2
	uint8_t* v8; // esi
	char v9; // al
	uint8_t* v10; // edx
	unsigned int v11; // ecx
	uint8_t* v12; // edi
	uint8_t* v13; // ebx
	char v14; // al
	int v15; // [esp-1Eh] [ebp-2Eh]
	int16_t v16; // [esp-1Ah] [ebp-2Ah]
	uint8_t* v17; // [esp+0h] [ebp-10h]
	uint8_t* v18; // [esp+4h] [ebp-Ch]
	uint8_t* v19; // [esp+8h] [ebp-8h]
	int v20; // [esp+Ch] [ebp-4h]

	//fix it
	int16_t a1 = 0;
	int a2 = 0;
	//fix it

	v3 = buffer;
	v4 = pdwScreenBuffer_351628;
	v19 = pdwScreenBuffer_351628 + 306560;
	v17 = pdwScreenBuffer_351628 + 640;
	v18 = pdwScreenBuffer_351628 + 307200;
	v20 = 0;
	a2 = 240;
	do
	{
		while (1)
		{
			while (1)
			{
				v5 = *v3;
				v3++;
				if (v5 <= 0)
					break;
				a1 = v5;
				qmemcpy(v4, v3, v5);
				v10 = &v4[v5];
				qmemcpy(v19, v3, a1);
				v19 += a1;
				v16 = a1;
				v15 = a2;
				v11 = a1;
				v12 = v17;
				v13 = v18;
				do
				{
					v14 = *(uint8_t*)v3;
					*v12 = *(uint8_t*)v3;
					*v13 = v14;
					v3 = (uint16_t*)((uint8_t*)v3 + 1);
					v12--;
					v13--;
					v11--;
				} while (v11);
				v17 = v12;
				v18 = v13;
				v4 = v10;
				a2 = v15;
				a1 = v16;
			}
			if ((v5 & 0x8000u) == 0 && !v5)
				break;
			v5 = -v5;
			v4 += v5;
			v20 += a1 + v5;
			v19 += v5;
			v17 -= v5;
			v18 -= v5;
		}
		if (a2 > 221)
		{
			v6 = v3;
			v7 = a1;
			v8 = (uint8_t*)v3 - 3;
			v9 = *v8;
			*v4 = *v8;
			*v19 = v9;
			a1 = v7;
			v3 = v6;
		}
		v4 = &v4[0 - a1 + 640 - v20];
		v19 = &v19[0 - a1 - 640 - v20];
		v17 += v20 + a1 + 640;
		v18 = v20 + a1 + v18 - 640;
		v20 = 0;
		a2 = a2 - 1;
	} while (a2);
}
