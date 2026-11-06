#pragma once

#include <cstdint>

#include "../sub_main.h"
#include "EventsFunctions.h"
#include "TypeConfigDat.h"
#include "Network.h"

#pragma pack(push, 1)
typedef struct {//lenght 7
	Type_SoundEvent_E17CC* pSoundEvent_0;
	uint8_t levelNumber_4;
	uint8_t overplayed_5;
	uint8_t fileIndex_6;
}
Type_CutScene_E16E0;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
	unsigned long LastTimeRendered_0;
	int16_t PosX_4;
	int16_t PosY_6;
	int16_t ActSprite_8;
	int8_t FirstSprite_10;
	int8_t LastSprite_11;
} type_MainMenuAnimations_E1748;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {//lenght 30
	int32_t time1_0;
	int32_t time2_4;
	int16_t x1_8;
	int16_t y1_10;
	int16_t firstFrame_12;
	int16_t lastFrame_14;
	int16_t frameIndex_16;
	int16_t time3_18;
	int8_t state_20;
	int8_t playOnce_21;
	int16_t time4_22;
	int16_t wavIndex_24;
	int16_t playType_26;
	//int8_t stuba[1];
	int16_t loopCount_28;
	//int8_t stubb[1];
} type_x_BYTE_E25ED_db_str;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {//lenght 18
	int16_t textIndex_0;
	int16_t left_2;
	int16_t top_4;
	int16_t minx_6;
	int16_t miny_8;
	int16_t word_10;
	int16_t width_12;
	int16_t maxy_14;
	int8_t mapButtonId_16;
	int8_t buttonId_17;
}
typeTextBoxtextBoxStr_E24BCx;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {//lenght 18
	int16_t posX_0;
	int16_t posY_2;
	int16_t word_4;
	int16_t word_6;
	int16_t word_8;
	int16_t word_10;
	int16_t textIndex_12;
	int16_t state_14;
	int16_t type_16;
}
type_x_BYTE_E25ED_2BB;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {//lenght 16
	int16_t x1_26_0;//0 x
	int16_t y1_28_1;//1 y
	int16_t word_30_2;//2
	int16_t word_32_3;//3
	int16_t height_34_4;//4
	int16_t scrollPos_36_5;//5
	int16_t textIndex_38_6;//6
	int16_t click_40_7;//7
	int16_t nocl_42_8;//8
} type_str_word_26;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {//lenght 44
	uint32_t callback_0;
	//int16_t word_2;
	int32_t state_4;//4 dword
	//int16_t word_6;//remove it
	int16_t selected_8;
	int16_t xmin_10;
	int16_t ymin_12;
	int16_t sizex_14;
	int16_t sizey_16;
	int16_t word_18;
	uint8_t goldSprite_20;//index of gold sprite
	uint8_t graySprite_21;//index of gray sprite
	int8_t id_22;
	int8_t canSelect_23;
	int8_t gold_color_24;
	int8_t active_25;
	//int16_t word_24;//23 and 24 byte
	type_str_word_26 scroll_26;//0
	//int16_t word_28;//1
	//int16_t word_30;//2
	//int16_t word_32;//3
	//int16_t word_34;//4
	//int16_t word_36;//5
	//int16_t word_38;//6
	//int16_t click_40;//7
	//int16_t nocl_42;
}
type_menuButtons_E1F84;
#pragma pack(pop)


// types

extern int16_t MOUSE_MIN;
extern int16_t MOUSE_MAX_X;
extern int16_t MOUSE_MAX_Y;

#pragma pack(push, 1)
typedef struct // size 16
{
	uint32_t helpTextTime_17DBA8; // weak 40
	uint32_t currentTime_17DBAC; //1(+4)
	uint32_t unk_17DBB0; //2(+8)
	uint16_t helpTextIndex_17DBB4; //3(+12)
	uint8_t x_BYTE_17DBB5; //(+14)
	uint8_t helpMode_17DBB6; //(+15)
} Type_unk_17DBA8str; //16
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct //lenght 56
{
	uint32_t time_17DB70; // weak
	int16_t portalAnimState_17DB74; // weak 4
	int16_t posX_17DB76; // weak 6
	int16_t posY_17DB78; // weak 8
	int16_t fill1; // test 10
	int16_t fill2; // test 12
	int16_t portPosX_17DB7E; // weak 14
	int16_t portPosY_17DB80; // weak 16
	int16_t shiftStep_17DB82; // weak 18
	int16_t portalPosX_17DB84; // weak 20
	int16_t portalPosY_17DB86; // weak 22
	int16_t portalActivated_17DB88; // weak 24
	int16_t selectedLevel_17DB8A; // weak 26
	int16_t targetLevel_17DB8C; // weak 28
	int8_t levelChosen_17DB8E; // weak 30
	int8_t mapState_17DB8F; // weak 31
	type_animStruct anim_17DB90; // weak 32
	//std::array<uint8_t, 6> unk_17DB92; // weak 32
	//std::array<uint8_t, 16> fill3; // fill
} type_x_DWORD_17DB70str;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct //lenght 13
{
	int32_t time_17DE28; // weak levelDescription_17DE28
	int32_t time2_17DE2C; // weak levelDescription_17DE28+4
	int16_t posX_17DE30; // weak levelDescription_17DE28+8
	int16_t posY_17DE32; // weak levelDescription_17DE28+10
	int8_t DisplayLevelDescriptionText_17DE34; // weak levelDescription_17DE28+12
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
	int16_t sample1_10;//10	
	int8_t sample1PlayType_12;//12
	uint8_t stub_13;//13
	int16_t sample2_14;//14	
	int8_t sample2PlayType_16;//16
	uint8_t stub_17;//17
	type_animStruct2 anim_18;////18
	int8_t animKind_42;//42
	int8_t animState_43;//43
	uint8_t portalIndex_44;//44
	type_animStruct animObject_45;//45
}
type_WORD_E20A4;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {//lenght 24
	uint32_t dataOffset_0;
	uint32_t dataSize_4;
	uint32_t paletteOffset_8;
	uint32_t paletteSize_12;
	uint16_t posX_16;
	uint16_t posY_18;
	uint16_t width_20;
	uint16_t height_22;
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
	uint8_t buttonId_0;
	uint8_t stub[17];
}
type_BYTE_E25ED_0x;
#pragma pack(pop)

// from sub_main
extern int8_t LoadLevelNumber_D419C;
extern char skipScreen_D41AD;
extern const char* LevelsNames_D9204[61];
extern __int16 x_WORD_E131A;
extern char menuSoundVolume_E1324;
extern type_menuButtons_E1F84 mainMenuButtons_E1BAC[10];
extern char minTimeCounter_E2096;
extern type_x_BYTE_E25ED_db_str x_BYTE_E25ED_db_str[16];
extern MenuItem nextMenu_E29D8;
extern int16_t m_ExitMenuLoop_E29DC;
extern char selectedSession_E29DF;
extern char langLoaded_E29E0;
extern uint8_t* memoryBufferAlloc_E9C3C;
extern int32_t times_17DBB8[4];
extern char mainMenuState_17DBC6;
extern uint32_t levelScores_17DBC8[125];
extern uint32_t secretLevelScores_17DDBC[26];
extern char IsPlayingCDTrack_17E09D;

extern int test_regression_level;
extern Type_SoundEvent_E17CC introSoundEvents_E17CC[];
extern bool mapNotMoving_E29D6;
extern Type_unk_17DBA8str mapHelpText_17DBA8;
extern type_x_DWORD_17DB70str mapMenuState_17DB70;
extern type_x_BYTE_E25ED_db_str mapAnimSprites_E26C8[];
extern typeTextBoxtextBoxStr_E24BCx textBoxStr_E24F2[];
extern Type_DWORD_17DE28str levelDescription_17DE28;
extern type_WORD_E20A4 dragonAnims_E20A4[];
extern type_menuButtons_E1F84 mapMenuButtons_E23E0[];
extern typeTextBoxtextBoxStr_E24BCx textBoxStr_E2516[];
extern Type_SoundEvent_E17CC intro2SoundEvents_E192C[];
extern int16_t x_WORD_17DE26;
extern char initialMenuMode_E29DE;
extern char directStart_E29E1;
extern bool first_enter;
extern int16_t mainMenuTextIndex_17DBC4;
extern typeTextBoxtextBoxStr_E24BCx textBoxStr_E25DC[];
extern type_BYTE_E25ED_0x goldMenuItems_E25ED[];
extern type_x_BYTE_E25ED_2BB keysDialogItems_E28A8[];
extern type_E1BAC_0x3c4 colourMenuBackgrounds_E1E98[];

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
signed int GetKeyName_79E10(char* a1, char a2);
void DrawMenuAnimations_7AB00();
void LoadAndSetGraphicsAndPalette_7AC00();
void SetMenuVideoMode_7ADE0(char a1);
void ResetMainMenuButtons_7BEC0();
void SetAnimationVariables_7DA70(__int16 a1, __int16 a2, __int16 a3, __int16 a4, __int16 a5, __int16 a6);
void PortalsUpdate_7DD70();
int16_t TestMouseRegions_7E1F0();
signed int DeselectMenuButton_7E620(type_menuButtons_E1F84* a1x);
char SaveGameDialog_7E800(type_menuButtons_E1F84* a1x);
char LoadGameDialog_7E820(type_menuButtons_E1F84* a1x);
int NewGameDraw_7EAE0(__int16* posx, __int16* posy, __int16* a3, __int16* a4, int8_t* a5, type_animStruct* animStruct);
int LoadLanguageFile(bitmap_pos_struct2_t** a1x, bitmap_pos_struct2_t** a2x, uint8_t* a3, char* langfilename, bitmap_pos_struct_t* a3dattabindex);
int LoadLanguageStrings_7F960(bitmap_pos_struct2_t* a1x, bitmap_pos_struct2_t* a2x, uint8_t* a3, char* langcountstring, bitmap_pos_struct_t* a3dattabindex);
void PresentLevelDescription_80C30(__int16 posX, __int16 posY, __int16 a3);
bool MoveGraphicsAndPlaySounds_80D40(__int16 a2, __int16 a3, __int16 a4, __int16 a5, char a6);
void MapMenuPortalsDraw_81760();
void WriteConfigDat_81DB0();
void DrawAndSoundDragonAndFire_81EE0(__int16 a5, __int16 a6);
Type_SecretMapScreenPortals_E2970* GetSecretAndActivedPortal2_824E0(__int16 a1);
void DrawMapMenuHelpText_82510(/*__int16 a1*//*, int *a2*/);
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
int EditTextField_7F6A0(int a1, int a2, __int16 a3, __int16 a4, char* a5, int8_t* a6, unsigned __int8 a7);
char SetJoystickDialog_79160(type_menuButtons_E1F84* a1x);
int MultiplayerPreviousLevel_7CCF0();
int MultiplayerNextLevel_7CD30();
int MultiplayerExit_7CCA0();
int MultiplayerPlayLevel_7CDA0();
char MultiplayerPreviousWizard_7CBF0();
char MultiplayerNextWizard_7CC40();
char MultiplayerMenu_7DE80(type_menuButtons_E1F84* a2x);
int DrawScrollDialog_7BF20(type_str_word_26* a1x);
void WaitForButtonPressAndRelease_8C0E0(unsigned __int8(/*__fastcall*/ *a1)(signed int));
int DrawScrollDialog2_7B660(int a1, int a2, __int16 a3, type_str_word_26* a4x, char* a5, char a6);
char /*__fastcall*/ NetworkSessionLobby_77680();
bool DrawAndServe_7B250();
signed int MultiplayerColourMenu_7C390();
void ResetMultiplayerPalette_7C710();
void SetMultiplayerColors_7D310();
bool SetMultiplayerColors_7CE50();
void RestoreMenuButtonBackground_7C720(unsigned __int8 a1, uint8_t* a2);
int HandleMultiplayerMenuButtons_7CB10();
bool InRegion_7B200(type_menuButtons_E1F84* a1x, int16_t testx, int16_t testy);
void PaletteMulti_7C9D0(signed __int16 a1);
void CleanRectByColor_7C7C0(uint8_t* a2);
void CleanRecByColor_85C42(uint8_t* a1, int a2, int a3, int a4, int a5, unsigned __int8 a6);
void DrawNetworkLevelName_7D380();
void PaletteCopy_7C800(signed __int16 a1);
void BlitTransparent640_85BF5(uint8_t* a1, uint8_t* a2, int a3, int a4, int a5, int a6);
void ClearPauseMenuState_41BC0();
void StopSubtitles_2EB40();
void DrawAnimTextsAndPlaySounds_7D400(__int16 posx, __int16 posy, char a4);
signed int DrawBitmapAndPlaySound_7E320();
int GetMapMenuDialogIndex_7E320(uint32_t adress, type_menuButtons_E1F84* a1x);
signed int NewGameDialog_7E640(type_menuButtons_E1F84* a1x);

extern __int16 menuVideoMode_E29DA; // weak
void LoadHscreen_7A110(char a1, char a2);
void LoadAndDecompressDatFile_7AA70(const char* a1, uint8_t* a2, int a3, int a4);
void ClearScrollDialogVars_7C020(type_str_word_26* a1x);
signed int ReadKeyboardKeysInMenu_7C050();
char IsAlphanumericOrSpace_7C200(uint8_t a1);
bool DrawFrameAnim_7E5A0(__int16 posx, __int16 posy, Type_MapScreenPortals_E17CC* mapPortal, __int16 beginIndex, __int16 endIndex);
void DrawTextBox(std::string message, uint16_t left, uint16_t right, uint16_t top, int16_t borderColour = 80);
void DrawTextBoxWithLine_7E840(typeTextBoxtextBoxStr_E24BCx* testBoxStr, __int16 borderColor, __int16 lineColor);
int DrawMapObject_812D0(__int16 a1, __int16 a2);
void DrawBitmapLine_81360(int16_t a1, int16_t a2, int16_t a3, int16_t a4, __int16 a5);
void DrawAnimSprite_81CA0(__int16 a3, __int16 a4, type_x_BYTE_E25ED_db_str* a5);
Type_SecretMapScreenPortals_E2970* GetSecretAndActivedPortal_824B0(__int16 actLevel);
void ActivateSecretPortal_82AB0(unsigned __int8 a1);
void DrawRoundFrame_85CC3(/*unsigned int a1, int a2, */unsigned __int16* a3);
extern Type_CutScene_E16E0 cutScene_E16E0[];
