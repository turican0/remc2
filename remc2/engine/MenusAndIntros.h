#pragma once

#include <cstdint>

#include "../sub_main.h"
#include "EventsFunctions.h"
#include "TypeConfigDat.h"
#include "Network.h"

// types

extern int16_t MOUSE_MIN;
extern int16_t MOUSE_MAX_X;
extern int16_t MOUSE_MAX_Y;

#pragma pack(push, 1)
typedef struct // size 16
{
	uint32_t unk_17DBA8; // weak 40
	uint32_t unk_17DBAC; //1(+4)
	uint32_t unk_17DBB0; //2(+8)
	uint16_t unk_17DBB4; //3(+12)
	uint8_t x_BYTE_17DBB5; //(+14)
	uint8_t x_BYTE_17DBB6; //(+15)
} Type_unk_17DBA8str; //16
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct //lenght 56
{
	uint32_t time_17DB70; // weak
	int16_t x_WORD_17DB74; // weak 4
	int16_t unk_17DB76_posx; // weak 6
	int16_t unk_17DB78_posy; // weak 8
	int16_t fill1; // test 10
	int16_t fill2; // test 12
	int16_t unk_17DB7E; // weak 14
	int16_t unk_17DB80; // weak 16
	int16_t x_WORD_17DB82_shift_step; // weak 18
	int16_t x_WORD_17DB84; // weak 20
	int16_t x_WORD_17DB86; // weak 22
	int16_t x_WORD_17DB88; // weak 24
	int16_t x_WORD_17DB8A; // weak 26
	int16_t x_WORD_17DB8C; // weak 28
	int8_t x_BYTE_17DB8E; // weak 30
	int8_t x_BYTE_17DB8F; // weak 31
	type_animStruct unk_17DB90; // weak 32
	//std::array<uint8_t, 6> unk_17DB92; // weak 32
	//std::array<uint8_t, 16> fill3; // fill
} type_x_DWORD_17DB70str;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct //lenght 13
{
	int32_t time_17DE28; // weak x_DWORD_17DE28str
	int32_t time2_17DE2C; // weak x_DWORD_17DE28str+4
	int16_t x_WORD_17DE30_posx; // weak x_DWORD_17DE28str+8
	int16_t x_WORD_17DE32_posy; // weak x_DWORD_17DE28str+10
	int8_t DisplayLevelDescriptionText_17DE34; // weak x_DWORD_17DE28str+12
} Type_DWORD_17DE28str;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {//lenght 69
	int32_t time_0;//0
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
	type_animStruct2 array_word_18;////18
	int8_t byte_42;//42
	int8_t byte_43;//43
	uint8_t byte_44;//44
	type_animStruct array_word_45;//45
}
type_WORD_E20A4;
#pragma pack(pop)

#pragma pack(push, 1)
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
#pragma pack(pop)

enum class MenuItem : int {
	InitLanguage = 0,
	SetToIntro = 1,
	LeaveX = 2,
	Intros = 3,
	MainMenu = 4,
	Exit = 5,
	LangSettings = 12
};

#pragma pack(push, 1)
typedef struct {//lenght 18
	uint8_t byte_0;
	uint8_t stub[17];
}
type_BYTE_E25ED_0x;
#pragma pack(pop)

// from sub_main
extern int8_t LoadLevelNumber_D419C;
extern char x_BYTE_D41AD_skip_screen;
extern const char* LevelsNames_D9204[61];
extern char* off_DB558[];
extern __int16 x_WORD_E131A;
extern char x_BYTE_E1324;
extern type_menuButtons_E1F84 mainMenuButtons_E1BAC[10];
extern long lastTime_E208C;
extern __int16 timeCounter_E2094;
extern char minTimeCounter_E2096;
extern char maxTimeCounter_E2097;
extern type_x_BYTE_E25ED_db_str x_BYTE_E25ED_db_str[16];
extern MenuItem nextMenu_E29D8;
extern int16_t m_ExitMenuLoop_E29DC;
extern char x_BYTE_E29DF_skip_screen;
extern char x_BYTE_E29E0;
extern uint8_t* pre_ptrMemoryBuffer_E9C3C;
extern uint8_t x_BYTE_EB39E_keys[10];
extern uint8_t uiBackGroundColorIdx_EB3A8;
extern int32_t times_17DBB8[4];
extern char x_BYTE_17DBC6;
extern uint32_t x_DWORD_17DBC8x[125];
extern uint32_t x_DWORD_17DDBCx[26];
extern char IsPlayingCDTrack_17E09D;
extern int16_t x_WORD_1803EC;
extern uint8_t unk_180560x[44];
extern type_unk_18058Cstr unk_18058Cstr;

extern int test_regression_level;
extern Type_SoundEvent_E17CC str_E17CC_0[];
extern bool map_not_moving_WORD_E29D6;
extern Type_unk_17DBA8str unk_17DBA8str;
extern type_x_DWORD_17DB70str x_DWORD_17DB70str;
extern type_x_BYTE_E25ED_db_str x_BYTE_E26C8_str[];
extern typeTextBoxtextBoxStr_E24BCx textBoxStr_E24F2[];
extern Type_DWORD_17DE28str x_DWORD_17DE28str;
extern type_WORD_E20A4 str_WORD_E20A4[];
extern type_menuButtons_E1F84 mapMenuButtons_E23E0[];
extern typeTextBoxtextBoxStr_E24BCx textBoxStr_E2516[];
extern Type_SoundEvent_E17CC str_E17CC_0x160[];
extern int16_t x_WORD_17DE26;
extern char x_BYTE_E29DE;
extern char x_BYTE_E29E1;
extern bool first_enter;
extern int16_t x_WORD_17DBC4;
extern typeTextBoxtextBoxStr_E24BCx textBoxStr_E25DC[];
extern type_BYTE_E25ED_0x str_BYTE_E25ED_0x[];
extern type_x_BYTE_E25ED_2BB str_BYTE_E25ED_2BB[];
extern type_E1BAC_0x3c4 str_E1BAC_0x2ec[];

// Result of the multiplayer game that has just finished, waiting to be shown.  It is set as
// the level is torn down and displayed after the exit of the level
extern std::string g_matchResultPending;

// functions
void MenusAndIntros_76930(bool skipMenus = false);
void InitLanguage_76A40();
void Intros_76D10(char introType);
void SetToIntro_76CF0();
void MainMenu_76FA0();
bool NewGameDialog_77350(type_menuButtons_E1F84* a1x);
char LanguageSettingDialog_779E0(type_menuButtons_E1F84* a1x);
bool LoadGameDialog_780F0(type_menuButtons_E1F84* menuButtons);
char SaveGameDialog_78730(type_menuButtons_E1F84* a1x);
char SetKeysDialog_79610();
signed int sub_79E10(char* a1, char a2);
void DrawMenuAnimations_7AB00();
void LoadAndSetGraphicsAndPalette_7AC00();
void sub_7ADE0(char a1);
void sub_7BEC0();
void SetAnimationVariables_7DA70(__int16 a1, __int16 a2, __int16 a3, __int16 a4, __int16 a5, __int16 a6);
void PortalsUpdate_7DD70();
int16_t TestMouseRegions_7E1F0();
signed int sub_7E620(type_menuButtons_E1F84* a1x);
char SaveGameDialog_7E800(type_menuButtons_E1F84* a1x);
char LoadGameDialog_7E820(type_menuButtons_E1F84* a1x);
int NewGameDraw_7EAE0(__int16* posx, __int16* posy, __int16* a3, __int16* a4, int8_t* a5, type_animStruct* animStruct);
int LoadLanguageFile(bitmap_pos_struct2_t** a1x, bitmap_pos_struct2_t** a2x, uint8_t* a3, char* langfilename, bitmap_pos_struct_t* a3dattabindex);
int sub_7F960(bitmap_pos_struct2_t* a1x, bitmap_pos_struct2_t* a2x, uint8_t* a3, char* langcountstring, bitmap_pos_struct_t* a3dattabindex);
void PresentLevelDescription_80C30(__int16 posX, __int16 posY, __int16 a3);
bool sub_80D40_move_graphics_and_play_sounds(__int16 a2, __int16 a3, __int16 a4, __int16 a5, char a6);
void MapMenuPortalsDraw_81760();
void WriteConfigDat_81DB0();
void DrawAndSoundDragonAndFire_81EE0(__int16 a5, __int16 a6);
Type_SecretMapScreenPortals_E2970* GetSecretAndActivedPortal2_824E0(__int16 a1);
void sub_82510(/*__int16 a1*//*, int *a2*/);
void PlayInGameFmv_82670();
void DrawEndGameTable_82C20(__int16 a1);
void PlayIntros_83250(char a1);
void ShowEndCredits_833C0();
void ShowScores();
std::vector<std::string> SplitLines(const std::string& text);
void ShowWelcomeScreen_83850();
void DrawNetGameMapBackground_85C8B(uint8_t* a1, uint8_t* a2, int a3, int a4, int a5, int a6);
bool DrawAndServe_pre_sub_7B250(uint32_t var, type_menuButtons_E1F84* var2x);
bool ExitDialog_77980(type_menuButtons_E1F84* a1x);
char SetPlayerNameDialog_78E00(/*int a1, int a2, */type_menuButtons_E1F84* a3x);
int sub_7F6A0(int a1, int a2, __int16 a3, __int16 a4, char* a5, int8_t* a6, unsigned __int8 a7);
char SetJoystickDialog_79160(type_menuButtons_E1F84* a1x);
int MultiplayerPreviousLevel_7CCF0();
int MultiplayerNextLevel_7CD30();
int MultiplayerExit_7CCA0();
int MultiplayerPlayLevel_7CDA0();
char MultiplayerPreviousWizard_7CBF0();
char MultiplayerNextWizard_7CC40();
char MultiplayerMenu_7DE80(type_menuButtons_E1F84* a2x);
int DrawScrollDialog_7BF20(type_str_word_26* a1x);
void sub_8C0E0(unsigned __int8(/*__fastcall*/ *a1)(signed int));
int DrawScrollDialog2_7B660(int a1, int a2, __int16 a3, type_str_word_26* a4x, char* a5, char a6);
char /*__fastcall*/ sub_77680();
bool DrawAndServe_7B250();
signed int sub_7C390();
void sub_7C710();
void SetMultiplayerColors_7D310();
bool SetMultiplayerColors_7CE50();
void sub_7C720(unsigned __int8 a1, uint8_t* a2);
int sub_7CB10();
bool InRegion_7B200(type_menuButtons_E1F84* a1x, int16_t testx, int16_t testy);
void PaletteMulti_7C9D0(signed __int16 a1);
void CleanRectByColor_7C7C0(uint8_t* a2);
void CleanRecByColor_85C42(uint8_t* a1, int a2, int a3, int a4, int a5, unsigned __int8 a6);
void DrawNetworkLevelName_7D380();
void PaletteCopy_7C800(signed __int16 a1);
void sub_85BF5(uint8_t* a1, uint8_t* a2, int a3, int a4, int a5, int a6);
void ClearPauseMenuState_41BC0();
void StopSubtitles_2EB40();
void DrawAnimTextsAndPlaySounds_7D400(__int16 posx, __int16 posy, char a4);
signed int DrawBitmapAndPlaySound_7E320();
int GetMapMenuDialogIndex_7E320(uint32_t adress, type_menuButtons_E1F84* a1x);
signed int sub_7E640(type_menuButtons_E1F84* a1x);