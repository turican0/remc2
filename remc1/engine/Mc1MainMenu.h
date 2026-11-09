#pragma once

#include <cstdint>

#include "Mc1Files.h"
#include "Mc1MenuText.h"
#include "Mc1Movie.h"

// MC1 main menu (mainMenuItem 2): the areas of data\screens\mmmask.dat select the items, the six save slots
// are shown in the load and save modes

#pragma pack(push, 1)
typedef struct { // 6 bytes
	void (*action_0)();
	uint16_t type_4; // without an action: 1 load, 2 save, 3 sound setup in the main mode; the slot 1-6 in the load and save modes
} Type_Mc1MainMenuArea;

typedef struct { // 16 bytes
	int32_t x_0;
	int32_t y_4;
	int32_t width_8;
	int32_t height_12;
} Type_Mc1SlotWindow;
#pragma pack(pop)

extern uint8_t newGame_9687C_9687C; // 1 until a game was played: no continue, a new game without asking
extern uint8_t networkFound_93958_93958; // set by InitNetwork_43270_435B0
extern int32_t idleMouseX_968C4_968C4;
extern int32_t idleMouseY_968C8_968C8;
extern int32_t idleLeftDown_968CC_968CC;
extern int32_t idleRightDown_968D0_968D0;
extern int32_t idleMiddleDown_968D4_968D4;
extern uint8_t idleKey_12CBDC_12CBCC;
extern Type_Mc1Font_96884 mainMenuFont_9688C_9688C; // data\screens\sfont1
extern char* slotNames_96864_96864[6]; // "Game One" ... or "--", 20 characters
extern const Type_Mc1SlotWindow slotWindows_4A7EC_4AB2C[6];
extern const Type_Mc1MainMenuArea mainMenuAreas_4A12C_4A46C[13];
extern const Type_Mc1MovieEvent scrollEvents_4A17C_4A4BC[2];

extern uint8_t* mainMenuSpritesTab_12CB04_12CAF4;
extern uint8_t* mainMenuSpritesTabEnd_12CB08_12CAF8;
extern uint8_t* mainMenuSpritesData_12CB0C_12CAFC;
extern uint8_t* mainMenuFontTab_12CB34_12CB24;
extern uint8_t* mainMenuFontTabEnd_12CB38_12CB28;
extern uint8_t* mainMenuFontData_12CB3C_12CB2C;
extern uint8_t* globeAnimation_12CB72_12CB62;
extern uint8_t* globeLoop_12CB76_12CB66;
extern uint8_t* globeFrame_12CB7A_12CB6A;
extern int16_t globeFrameIndex_12CB7E_12CB6E;
extern uint8_t* timerAnimation_12CB80_12CB70;
extern uint8_t* timerLoop_12CB84_12CB74;
extern uint8_t* timerFrame_12CB88_12CB78;
extern int16_t timerFrameIndex_12CB8C_12CB7C;
extern uint8_t* mainMenuMask_12CBA0_12CB90; // the area of each pixel
extern uint8_t* brightTable_12CBAC_12CB9C; // each color 30 % brighter, the area under the cursor
extern int16_t lastMouseY_12CBD8_12CBC8;
extern int16_t lastMouseX_12CBDA_12CBCA;
extern uint32_t idleStart_12CBB4_12CBA4; // timer ticks of the last input
extern uint32_t demoLevel_12CBB8_12CBA8; // the level of the settings while the demo plays
extern uint8_t demoProgress_12CA60_12CA50[0x18]; // the progress of player 0 while the demo plays
extern Type_Mc1FileEntry mainMenuFiles_49D64_4A0A4[9];

void MainMenu_4BD60_4C0A0();
int32_t IsIdle_4C580_4C8C0();
void SelectPreviousArea_4C640_4C980();
void SelectNextArea_4C670_4C9B0();
int32_t IsAreaEnabled_4C6A0_4C9E0();
void AnimateMainMenu_4C6F0_4CA30();
void InitMainMenu_4C7E0_4CB20();
void ExitMainMenu_4CAC0_4CE00();
void DrawMainMenu_4CB70_4CEB0();
void DrawLoadConfirm_4CC00_4CF40();
void DrawSlotNames_4D300_4D640();
int32_t Confirm_4F8D0_4FC10(void (*draw)());
void QuitGame_4FD20_50060();
void DrawQuitConfirm_4FE00_50140();
void NetworkGame_4FEB0_501F0();
void ContinueGame_4FF00_50240();
void NewGame_4FF30_50270();
void DrawNewGameConfirm_4FFE0_50320();
void ScrollFrame_50470_507B0();
void BuildBrightTable_504A0_507E0();
void ReadSlotNames_51A10_51D50();
void HighlightArea_51E84_521C4(const uint8_t* source, uint8_t* dest, const uint8_t* mask, const uint8_t* table, uint8_t area);
