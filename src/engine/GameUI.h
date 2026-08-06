#pragma once

#include <array>
#include <string_view>

#include "global_types.h"
#include "engine_support.h"

using namespace std::string_view_literals;


#pragma pack(push, 1)
typedef struct {
	axis_3d v51y;//v51x_struct v51x; // [esp+A8h] [ebp-12h]//0
	//uint16_t v52xw_4;//__int16 v52; // [esp+ACh] [ebp-Eh]//+4
	uint8_t* v53xdw_6;//uint8_t* v53; // [esp+AEh] [ebp-Ch]//+6
	uint32_t v54xdw_10;//int v54; // [esp+B2h] [ebp-8h]//+10
	uint32_t v55xdw_14;//int v55; // [esp+B6h] [ebp-4h]//+14
	uint32_t v56xdw_18;//int v56; // [esp+BAh] [ebp+0h]//+18
	uint32_t v57xdw_22;//int v57; // [esp+BEh] [ebp+4h]//+22
	uint32_t v58xdw_26;//int v58; // [esp+C2h] [ebp+8h]//+26
	uint32_t v59xdw_30;//int v59; // [esp+C6h] [ebp+Ch]//+30
	uint32_t v60xdw_34;//int v60; // [esp+CAh] [ebp+10h]//+34
	uint32_t v61xdw_38;//int v61; // [esp+CEh] [ebp+14h]//+38
	uint16_t v62xw_42;//__int16 v62; // [esp+D2h] [ebp+18h]//+42
	uint16_t v63xw_44;//__int16 v63; // [esp+D4h] [ebp+1Ah]//+44
	uint8_t v64xb_46;//char v64; // [esp+D6h] [ebp+1Ch]//+46 //color
} v51x_struct;
#pragma pack(pop)


extern char FontType_D419D;
extern char x_BYTE_D47D8;
extern char spellIndex_D94FF[29];
extern int16_t x_WORD_E375C_mouse_position_x;
extern int16_t x_WORD_E375E_mouse_position_y;
extern uint8_t xx_array_E36C4[64];

constexpr std::array WizardsNames_D93A0 { 
    "Zanzamar"sv, "Nyphur"sv, "Rahn"sv, "Belix"sv, "Jark"sv, "Elyssia"sv, "Yragore"sv, "Prish"sv  
};


void DrawLine_2BD10(int16_t posX1, int16_t posY1, int16_t posX2, int16_t posY2, uint8_t color, uint8_t thickness = 1);

void DrawTopStatusBar_2D710(type_entity_0x6E8E* a1, uint8_t scale = 1);
void DrawSpellIcon_2E260(int16_t posX, int16_t posY, type_entity_0x6E8E* playerEvent, bool drawNextBitmap, uint8_t scale = 1);

void ComputeTextboxSizes_89830(Type_TextBox_1804B0* textbox, uint8_t scale = 1);
void ComputeTextboxLine_898A0(Type_TextBox_1804B0* textbox);
void ComputeTextboxSizesFromTextLines_89920(Type_TextBox_1804B0* textbox, int16_t countLines, int16_t* textLines, uint8_t scale = 1);

void GetHelpPopupTextAndCoords_87CF0(uint8_t scale = 1);
int16_t GetHelpPopupIndex_88450();
void GetHintText_89AC0(char* buffer, int helpIndex);

void SetPauseMenuClosed_41B60();
void SetMenuCursorPosition_52E90(type_str_0x2BDE* playStr, uint16_t newMenuStatus, bool useSound, uint8_t scale = 1);
void sub_53120();
void SetHelpPopupTextAndCoords_884D0(int16_t helpIdx, int16_t a2, int16_t* popupSrcPos, char a4, char a5, uint8_t scale = 1);
void SetHelpPopupCoords_87580();
void SetPauseMenuCoordinates_87970(uint8_t scale = 1);
void SetSpellHelpPopupCoordinates_88D40(uint8_t scale = 1);
void SetPlayerScoresHelpPopupTextAndCoords_89360(uint8_t scale = 1);
void SetTextBoxMinMaxSizes_87090();
void SetTextBoxMinMaxForSetResolution();
void SetSoundEffectAndMusicLevelCoordinates_19D60(signed int volume);

void DrawPauseMenuPopUps_87860();
void sub_87C10();
void DrawBottomSpellsMenu_2ECC0();
void ColorizeScreen_2E790(int posX, int posY, int width, int height, uint8_t color);
void DrawChatMenu_2F6B0();
void DrawPopupTextBox_87610();
void DrawHelpPopUps_871F0();
void DrawTextPauseEndOfLevel_2CE30(int16_t posX, int16_t posY, uint8_t scale = 1);
void DrawCurrentObjectiveTextbox_30630(uint8_t scale = 1);
void GetFont_6FC50(int16_t a1);
void DrawInGameOptionsMenu_30050(uint8_t scale = 1);
char sub_595C0(int16_t a1);
char sub_59610(type_entity_0x6E8E** a1, int16_t a2);
bool sub_596C0(axis_3d* a1, int16_t a2);
char sub_63570(type_entity_0x6E8E* a1, type_entity_0x6E8E* a2);
void DrawMinimap_63600(int16_t x, int16_t y, int16_t posX, int16_t posY, uint16_t width, uint16_t height, int16_t yaw, int16_t scaling, int a10);
void DrawMinimapEntites_61880(int16_t x, int16_t y, int16_t posX, int16_t posY, uint16_t width, uint16_t height, int16_t yaw, int16_t scaling, uint8_t scale);
int TransformPlayerColorIndex_616D0(int index);
void DrawMinimapMarks_644F0(int16_t x, int16_t y, int16_t posX, int16_t posY, uint16_t width, uint16_t height, int16_t yaw, int16_t scaling, uint8_t scale);
char DrawObjectiveRectangle_64CE0(v51x_struct* a1, uint8_t scale = 1);
void DrawOkCancelMenu_30A60(int16_t posTextX, int16_t posTextY, uint8_t scale = 1);
void DrawPauseMenu_2FD90(uint8_t scale = 1);
void DrawSorcererScores_2D1D0(uint8_t scale = 1);
void DrawTextboxLine_89A30(const Type_TextBox_1804B0* textbox);
void DrawVolumeSettings_303D0(uint8_t scale = 1);

void sub_63670_draw_minimap_a(int16_t x, int16_t y, int16_t posX, int16_t posY, uint16_t width, uint16_t height, int16_t yaw, int16_t scaling, int a10);
void sub_63C90_draw_minimap_b(int16_t x, int16_t y, int16_t posX, int16_t posY, uint16_t width, uint16_t height, int16_t yaw, int16_t scaling, int a10);
void sub_627F0_draw_minimap_entites_a(int16_t x, int16_t y, int16_t posX, int16_t posY, uint16_t width, uint16_t height, int16_t yaw, int16_t scaling, uint8_t scale);
void DrawMinimapEntities_B_61A00(int16_t x, int16_t y, int16_t posX, int16_t posY, uint16_t width, uint16_t height, int16_t yaw, int16_t scaling, uint8_t scale);
uint8_t CalculateScaleOffset(uint8_t scale);
void DrawRectangle(uint8_t* ptrImageBufferStart, int posX, int poxY, int width, int height, uint8_t lineThickness, uint32_t pitch, uint8_t colour);

void sub_885E0(type_entity_0x6E8E* a1, int16_t posX, int16_t posY, uint16_t a4);

void ComputeTextboxSizesFromTextWords_89420(Type_TextBox_1804B0* textbox, const char* text, uint8_t scale = 1);
void ConstrainTextboxSizes_89520(Type_TextBox_1804B0* textbox, uint8_t scale = 1);
void DrawTextboxText_895D0(Type_TextBox_1804B0* textbox, const char* text, uint8_t scale = 1);
void DrawTextboxFrame_89690(Type_TextBox_1804B0* textbox, uint8_t scale = 1);
void ComputeFrameSizes_89980(Type_TextBox_1804B0* textbox, uint8_t scale = 1);

void DrawLine_BD524(int16_t posX1, int16_t posY1, int16_t posX2, int16_t posY2, uint8_t color);
void DrawLine_BD542(uint16_t posX1, uint16_t posY1, uint16_t posX2, uint16_t posY2, uint8_t color);
void sub_548F0(type_str_0x2BDE* a1);
void sub_1A280();
int sub_1A4A0();
void SetOkayCancelButtonsCursorPosition_1A030();
void MoveCursorToSelectedSpell_6D200(type_str_0x2BDE* a1);
void SetMousePositionInMemory_5BDC0(int16_t posX, int16_t posY);
void sub_1A5B0_getLangStrings(int a1, int* a2, int* a3);
void GetOkayCancelButtonPositions_30BE0(int16_t* a1, int16_t* a2, uint8_t scale = 1);
void ShowMessage_52D70(uint16_t playerIndex, char* cheatMessage);
