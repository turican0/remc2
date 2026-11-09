#include "Mc1MainMenu.h"

#include <cstdio>
#include <cstring>
#include <string>
#include <utility>

#include "Mc1Game.h"
#include "Mc1Graphics.h"
#include "Mc1Install.h"
#include "Mc1Menu.h"
#include "Mc1NotPorted.h"
#include "Mc1Shared.h"
#include "Mc1Sound.h"
#include "Mc1System.h"
#include "../../remc2/utilities/DataFileIO.h"

uint8_t newGame_9687C_9687C = 1;
uint8_t networkFound_93958_93958 = 0;
int32_t idleMouseX_968C4_968C4 = 0;
int32_t idleMouseY_968C8_968C8 = 0;
int32_t idleLeftDown_968CC_968CC = 0;
int32_t idleRightDown_968D0_968D0 = 0;
int32_t idleMiddleDown_968D4_968D4 = 0;
uint8_t idleKey_12CBDC_12CBCC = 0;
Type_Mc1Font_96884 mainMenuFont_9688C_9688C = {};

// 967E5: six strings of 20 characters, read from the saved games
static char slotNameTexts_967E5_967E5[6][21] = {
	"Game One            ",
	"Game Two            ",
	"Game Three          ",
	"Game Four           ",
	"Game Five           ",
	"Game Six            ",
};
char* slotNames_96864_96864[6] = {
	slotNameTexts_967E5_967E5[0], slotNameTexts_967E5_967E5[1], slotNameTexts_967E5_967E5[2],
	slotNameTexts_967E5_967E5[3], slotNameTexts_967E5_967E5[4], slotNameTexts_967E5_967E5[5],
};

const Type_Mc1SlotWindow slotWindows_4A7EC_4AB2C[6] = {
	{ 170, 0, 150, 36 },
	{ 161, 37, 159, 35 },
	{ 191, 73, 129, 32 },
	{ 172, 104, 148, 27 },
	{ 186, 133, 134, 31 },
	{ 173, 165, 147, 28 },
};

// indexed by the area of mmmask.dat
const Type_Mc1MainMenuArea mainMenuAreas_4A12C_4A46C[13] = {
	{ nullptr, 0 },
	{ NewGame_4FF30_50270, 0 },
	{ sub_50030_50370, 0 },
	{ NetworkGame_4FEB0_501F0, 0 },
	{ QuitGame_4FD20_50060, 0 },
	{ nullptr, 1 },
	{ nullptr, 2 },
	{ nullptr, 3 },
	{ nullptr, 4 },
	{ nullptr, 5 },
	{ nullptr, 6 },
	{ ContinueGame_4FF00_50240, 0 },
	{ nullptr, 0 },
};

// intro\scroll.dat; the second record never matches
const Type_Mc1MovieEvent scrollEvents_4A17C_4A4BC[2] = {
	{ 0, 'A', 1, 0 },
	{ 0xFFFF, 0, 0, 0 },
};

uint8_t* mainMenuSpritesTab_12CB04_12CAF4 = nullptr;
uint8_t* mainMenuSpritesTabEnd_12CB08_12CAF8 = nullptr;
uint8_t* mainMenuSpritesData_12CB0C_12CAFC = nullptr;
uint8_t* mainMenuFontTab_12CB34_12CB24 = nullptr;
uint8_t* mainMenuFontTabEnd_12CB38_12CB28 = nullptr;
uint8_t* mainMenuFontData_12CB3C_12CB2C = nullptr;
uint8_t* globeAnimation_12CB72_12CB62 = nullptr;
uint8_t* globeLoop_12CB76_12CB66 = nullptr;
uint8_t* globeFrame_12CB7A_12CB6A = nullptr;
int16_t globeFrameIndex_12CB7E_12CB6E = 0;
uint8_t* timerAnimation_12CB80_12CB70 = nullptr;
uint8_t* timerLoop_12CB84_12CB74 = nullptr;
uint8_t* timerFrame_12CB88_12CB78 = nullptr;
int16_t timerFrameIndex_12CB8C_12CB7C = 0;
uint8_t* mainMenuMask_12CBA0_12CB90 = nullptr;
uint8_t* brightTable_12CBAC_12CB9C = nullptr;
int16_t lastMouseY_12CBD8_12CBC8 = 0;
int16_t lastMouseX_12CBDA_12CBCA = 0;
uint32_t idleStart_12CBB4_12CBA4 = 0;
uint32_t demoLevel_12CBB8_12CBA8 = 0;
uint8_t demoProgress_12CA60_12CA50[0x18];

// 49D64 / 4A0A4 of the code segment
Type_Mc1FileEntry mainMenuFiles_49D64_4A0A4[9] = {
	{ { "data/screens/mmspr.dat", "data/screens/mmspr.dat" }, &mainMenuSpritesData_12CB0C_12CAFC, nullptr, 0, 0 },
	{ { "data/screens/mmspr.tab", "data/screens/mmspr.tab" }, &mainMenuSpritesTab_12CB04_12CAF4, &mainMenuSpritesTabEnd_12CB08_12CAF8, 0, 0 },
	{ { "data/screens/mmmask.dat", "data/screens/mmmask.dat" }, &mainMenuMask_12CBA0_12CB90, nullptr, 0, 0 },
	{ { "data/screens/sfont1.dat", "data/screens/sfont1.dat" }, &mainMenuFontData_12CB3C_12CB2C, nullptr, 0, 0 },
	{ { "data/screens/sfont1.tab", "data/screens/sfont1.tab" }, &mainMenuFontTab_12CB34_12CB24, &mainMenuFontTabEnd_12CB38_12CB28, 0, 0 },
	{ { "data/screens/globe.dat", "data/screens/globe.dat" }, &globeAnimation_12CB72_12CB62, nullptr, 0, 0 },
	{ { "data/screens/timer.dat", "data/screens/timer.dat" }, &timerAnimation_12CB80_12CB70, nullptr, 0, 0 },
	{ { "*BRIGHT TABLE", "*BRIGHT TABLE" }, &brightTable_12CBAC_12CB9C, nullptr, 0x100, 0 },
	{ { "", "" }, nullptr, nullptr, 0, 0 },
};

// port: the tabs of mmspr.tab and sfont1.tab as indexes with real pointers
static bitmap_pos_struct_t* mainMenuSpritesIndex = nullptr;
static bitmap_pos_struct_t* mainMenuFontIndex = nullptr;

static void CopyFullScreen(uint8_t* source, uint8_t* dest)
{
	if (*videoMode_12F02E_12F01E & 1)
		CopyScreen320_62FA8_634B8(source, dest, 320, 200);
	else
		CopyScreen640_62FC4_634D4(source, dest, 640, 480);
}

static void DrawSprite(int16_t x, int16_t y, const bitmap_pos_struct_t& sprite)
{
	if (*videoMode_12F02E_12F01E & 1)
		DrawBitmap320_60CE0_611F0(x, y, sprite, 1);
	else
		DrawBitmap640_60D18_61228(x, y, sprite, 1);
}

// the original swaps *WScreen and *BScreen (xor) to draw the menu into the back screen
static void DrawMainMenuIntoBackScreen()
{
	std::swap(*workScreen_12EFF4_12EFE4, backScreen_AE3FC_AE3EC);
	DrawMainMenu_4CB70_4CEB0();
	std::swap(*workScreen_12EFF4_12EFE4, backScreen_AE3FC_AE3EC);
}

static void RedrawMainMenu()
{
	CopyFullScreen(screenBackup2_12CBA8_12CB98, backScreen_AE3FC_AE3EC);
	DrawMainMenuIntoBackScreen();
}

static uint8_t AreaAt(int16_t x, int16_t y)
{
	return mainMenuMask_12CBA0_12CB90[y * (int32_t)*screenWidth_12EFF0_12EFE0 + x];
}

void MainMenu_4BD60_4C0A0()
{
	int8_t* keys = *pressedKeys_12EEF0_12EEE0;
	bool exitMenu = false;
	bool redrawn = false;
	bool demo = false;
	if (!(menuFlags_12CBD5_12CBC5 & 1))
		InitMainMenu_4C7E0_4CB20();
	AnimateMainMenu_4C6F0_4CA30();
	CopyFullScreen(screenBackup2_12CBA8_12CB98, backScreen_AE3FC_AE3EC);
	DrawMainMenuIntoBackScreen();
	// Tab, with Shift backwards
	if (keys[0x0F])
	{
		keys[0x0F] = 0;
		if (keys[0x2A] || keys[0x36])
			SelectPreviousArea_4C640_4C980();
		else
			SelectNextArea_4C670_4C9B0();
	}
	int16_t x = mouseX_12CADC_12CACC >> 1;
	int16_t y = mouseY_12CADE_12CACE >> 1;
	if ((x != lastMouseX_12CBDA_12CBCA || y != lastMouseY_12CBD8_12CBC8) && x < 320 && y < 200)
	{
		lastMouseX_12CBDA_12CBCA = x;
		lastMouseY_12CBD8_12CBC8 = y;
		mainMenuArea_12CBC6_12CBB6 = AreaAt(x, y);
		if (!IsAreaEnabled_4C6A0_4C9E0())
			mainMenuArea_12CBC6_12CBB6 = 0;
	}
	// Enter clicks
	if (keys[0x1C])
	{
		keys[0x1C] = 0;
		inputFlags_12CAE4_12CAD4[0] |= 1;
	}
	// the right button or Esc leave the load / save mode
	if ((inputFlags_12CAE4_12CAD4[0] & 0x40) || *lastKey_12EF70_12EF60 == 1)
	{
		*lastKey_12EF70_12EF60 = 0;
		inputFlags_12CAE4_12CAD4[0] &= 0xBF;
		if (mainMenuMode_12CBCD_12CBBD)
			mainMenuMode_12CBCD_12CBBD = 0;
		mainMenuArea_12CBC6_12CBB6 = AreaAt(lastMouseX_12CBDA_12CBCA, lastMouseY_12CBD8_12CBC8);
		if (!IsAreaEnabled_4C6A0_4C9E0())
			mainMenuArea_12CBC6_12CBB6 = 0;
	}
	if ((inputFlags_12CAE4_12CAD4[0] & 1) && !(mainMenuArea_12CBC6_12CBB6 == 11 && newGame_9687C_9687C))
	{
		inputFlags_12CAE4_12CAD4[0] &= 0xFE;
		const Type_Mc1MainMenuArea& area = mainMenuAreas_4A12C_4A46C[mainMenuArea_12CBC6_12CBB6];
		if (area.action_0)
		{
			area.action_0();
			if (menuFlags_12CBD5_12CBC5 & 8)
			{
				menuFlags_12CBD5_12CBC5 &= 0xF7;
				exitMenu = true;
			}
		}
		else if (area.type_4)
		{
			switch (mainMenuMode_12CBCD_12CBBD)
			{
			case 0:
				if (area.type_4 == 3)
				{
					mainMenuItem_12CBCE_12CBBE = 1;
					exitMenu = true;
				}
				break;
			case 1:
				if (area.type_4 >= 1 && area.type_4 <= 6 && Confirm_4F8D0_4FC10(DrawLoadConfirm_4CC00_4CF40))
					LoadGame_51AF0_51E30(area.type_4);
				redrawn = true;
				RedrawMainMenu();
				break;
			case 2:
				if (area.type_4 >= 1 && area.type_4 <= 6 && SaveDialog_4CCA0_4CFE0())
					SaveGame_51C90_51FD0(area.type_4);
				redrawn = true;
				RedrawMainMenu();
				break;
			}
			if (redrawn)
				mainMenuMode_12CBCD_12CBBD = 0;
			else if (!mainMenuMode_12CBCD_12CBBD && (area.type_4 == 1 || area.type_4 == 2))
			{
				// 4C277: the load or save mode shows the slots
				mainMenuMode_12CBCD_12CBBD = (uint8_t)area.type_4;
				RedrawMainMenu();
			}
		}
	}
	else if (inputFlags_12CAE4_12CAD4[0] & 1)
		inputFlags_12CAE4_12CAD4[0] &= 0xFE;
	// 4C316: the area under the cursor brighter
	uint8_t highlight = mainMenuArea_12CBC6_12CBB6;
	if ((highlight == 11 && newGame_9687C_9687C) || !highlight)
		highlight = 0xFF;
	HighlightArea_51E84_521C4(backScreen_AE3FC_AE3EC, *workScreen_12EFF4_12EFE4, mainMenuMask_12CBA0_12CB90,
		brightTable_12CBAC_12CB9C, highlight);
	if (inputDevices_90B23_90B23 & 0x23)
		DrawSprite(mousePos_9AD90_9AD90->x, mousePos_9AD90_9AD90->y, menuSpritesIndex[mainMenuItem_12CBCE_12CBBE]);
	BlitScreen();
	if (IsIdle_4C580_4C8C0())
	{
		// 40 s without input: the intro, the title and a demo in turn
		if (idleStart_12CBB4_12CBA4 + 0x12C0 <= timerTicks_AC5D4_AC5C4)
		{
			idleStart_12CBB4_12CBA4 = timerTicks_AC5D4_AC5C4;
			switch (attractStep_12CBD4_12CBC4)
			{
			case 0:
				mainMenuItem_12CBCE_12CBBE = 0;
				attractFlags_12CBD6_12CBC6 |= 1;
				StopMusic_20E60_20E60();
				break;
			case 1:
				mainMenuItem_12CBCE_12CBBE = 8;
				attractFlags_12CBD6_12CBC6 |= 2;
				StopMusic_20E60_20E60();
				break;
			case 2:
				attractFlags_12CBD6_12CBC6 |= 4;
				demoLevel_12CBB8_12CBA8 = mc1Settings_AE408_AE3F8->levelIndex_0x11_17;
				demo = true;
				menuDone_96880_96880 = 1;
				memcpy(demoProgress_12CA60_12CA50, mc1State_AE400_AE3F0->players_0x340B_13323[0].progress_0x7CB_1995, 0x18);
				mc1Settings_AE408_AE3F8->movie_0x0D_13 = 0;
				mc1Settings_AE408_AE3F8->unknown_0xA1_161 = 3;
				mc1Settings_AE408_AE3F8->unknown_0xA2_162 = 200;
				mc1Settings_AE408_AE3F8->flags_0x00_0 |= 0x24; // 4 no exit screen, 0x20 roll
				break;
			}
			attractStep_12CBD4_12CBC4++;
			if (attractStep_12CBD4_12CBC4 == 3)
				attractStep_12CBD4_12CBC4 = 0;
			exitMenu = true;
		}
	}
	else
		idleStart_12CBB4_12CBA4 = timerTicks_AC5D4_AC5C4;
	if (exitMenu)
	{
		ExitMainMenu_4CAC0_4CE00();
		if (demo)
			mainMenuItem_12CBCE_12CBBE = 2;
	}
}

int32_t IsIdle_4C580_4C8C0()
{
	if (mousePos_9AD90_9AD90->x == idleMouseX_968C4_968C4 && mousePos_9AD90_9AD90->y == idleMouseY_968C8_968C8
		&& *lastKey_12EF70_12EF60 == idleKey_12CBDC_12CBCC && *mouseLeftDown_12EFE4_12EFD4 == idleLeftDown_968CC_968CC
		&& *mouseRightDown_12EFE2_12EFD2 == idleRightDown_968D0_968D0
		&& (int16_t)*mouseMiddleDown_12EFE0_12EFD0 == idleMiddleDown_968D4_968D4)
		return 1;
	idleMouseX_968C4_968C4 = mousePos_9AD90_9AD90->x;
	idleMouseY_968C8_968C8 = mousePos_9AD90_9AD90->y;
	idleLeftDown_968CC_968CC = *mouseLeftDown_12EFE4_12EFD4;
	idleRightDown_968D0_968D0 = *mouseRightDown_12EFE2_12EFD2;
	idleMiddleDown_968D4_968D4 = (int16_t)*mouseMiddleDown_12EFE0_12EFD0;
	idleKey_12CBDC_12CBCC = *lastKey_12EF70_12EF60;
	return 0;
}

void SelectPreviousArea_4C640_4C980()
{
	do
	{
		mainMenuArea_12CBC6_12CBB6--;
		if (mainMenuArea_12CBC6_12CBB6 < 1)
			mainMenuArea_12CBC6_12CBB6 = 11;
	} while (!IsAreaEnabled_4C6A0_4C9E0());
}

void SelectNextArea_4C670_4C9B0()
{
	do
	{
		mainMenuArea_12CBC6_12CBB6++;
		if (mainMenuArea_12CBC6_12CBB6 > 11)
			mainMenuArea_12CBC6_12CBB6 = 1;
	} while (!IsAreaEnabled_4C6A0_4C9E0());
}

int32_t IsAreaEnabled_4C6A0_4C9E0()
{
	uint8_t area = mainMenuArea_12CBC6_12CBB6;
	if (area > 7)
	{
		// 11 continue only after a game, 8-10 the slots 4-6 only in the load / save mode
		if (area == 11)
			return !newGame_9687C_9687C;
		return mainMenuMode_12CBCD_12CBBD != 0;
	}
	if (area == 3)
		return networkFound_93958_93958 == 1;
	if (area == 7)
		return mainMenuMode_12CBCD_12CBBD != 0;
	return 1;
}

void AnimateMainMenu_4C6F0_4CA30()
{
	if ((dword_12CBB0_12CBA0 & 1) != 1 || mainMenuItem_12CBCE_12CBBE != 2)
		return;
	// data\screens\globe.dat, frames 1-30 loop
	globeFrame_12CB7A_12CB6A = (uint8_t*)DecodeAnimationFrame_1002D_1002D((int8_t*)globeFrame_12CB7A_12CB6A,
		(int8_t*)screenBackup2_12CBA8_12CB98);
	globeFrameIndex_12CB7E_12CB6E++;
	if (globeFrameIndex_12CB7E_12CB6E == 0x1F)
	{
		globeFrameIndex_12CB7E_12CB6E = 1;
		globeFrame_12CB7A_12CB6A = globeLoop_12CB76_12CB66;
	}
	else if (globeFrameIndex_12CB7E_12CB6E == 1 && !globeLoop_12CB76_12CB66)
		globeLoop_12CB76_12CB66 = globeFrame_12CB7A_12CB6A;
	if (newGame_9687C_9687C)
		return;
	// data\screens\timer.dat (the continue item), frames 1-3 loop
	timerFrame_12CB88_12CB78 = (uint8_t*)DecodeAnimationFrame_1002D_1002D((int8_t*)timerFrame_12CB88_12CB78,
		(int8_t*)screenBackup2_12CBA8_12CB98);
	timerFrameIndex_12CB8C_12CB7C++;
	if (timerFrameIndex_12CB8C_12CB7C == 4)
	{
		timerFrameIndex_12CB8C_12CB7C = 1;
		timerFrame_12CB88_12CB78 = timerLoop_12CB84_12CB74;
	}
	else if (timerFrameIndex_12CB8C_12CB7C == 1 && !timerLoop_12CB84_12CB74)
		timerLoop_12CB84_12CB74 = timerFrame_12CB88_12CB78;
}

void InitMainMenu_4C7E0_4CB20()
{
	SetAnimationPosition_10010_10010(0x140, 0);
	FadeInOut_61CC0_621D0(nullptr, 0x10, false, 10);
	LoadSoundBank_5D070_5D580(13);
	LoadMusicBank_5CEF0_5D400(0);
	LoadFileGroup_40440_40780(mainMenuFiles_49D64_4A0A4);
	BuildSpritesIndex(&mainMenuSpritesIndex, mainMenuSpritesTab_12CB04_12CAF4, mainMenuSpritesTabEnd_12CB08_12CAF8,
		mainMenuSpritesData_12CB0C_12CAFC, true);
	// 519E0 relocates sfont1.tab in place (not doubled)
	BuildSpritesIndex(&mainMenuFontIndex, mainMenuFontTab_12CB34_12CB24, mainMenuFontTabEnd_12CB38_12CB28,
		mainMenuFontData_12CB3C_12CB2C, false);
	InitFont_51480_517C0(&mainMenuFont_9688C_9688C, mainMenuFontIndex, menuPalette_12CB9C_12CB8C);
	mainMenuFont_9688C_9688C.flags_0x04_4 &= 0xFD;
	ReadPackedFile_3EEA0_3F1E0("data/screens/mainmenu.dat", backScreen_AE3FC_AE3EC);
	CopyFullScreen(backScreen_AE3FC_AE3EC, screenBackup2_12CBA8_12CB98);
	ReadPackedFile_3EEA0_3F1E0("data/screens/mainmenu.pal", menuPalette_12CB9C_12CB8C);
	BuildBrightTable_504A0_507E0();
	ReadSlotNames_51A10_51D50();
	DrawMainMenuIntoBackScreen();
	CopyFullScreen(backScreen_AE3FC_AE3EC, *workScreen_12EFF4_12EFE4);
	BlitScreen();
	if (!inputDevices_90B23_90B23)
		SetCursor_5C05C_5C56C(menuSpritesIndex[mainMenuItem_12CBCE_12CBBE]);
	FadeInOut_61CC0_621D0((TColor*)menuPalette_12CB9C_12CB8C, 0x20, false, 10);
	globeLoop_12CB76_12CB66 = nullptr;
	timerLoop_12CB84_12CB74 = nullptr;
	timerFrameIndex_12CB8C_12CB7C = 0;
	menuFlags_12CBD5_12CBC5 |= 1;
	mainMenuArea_12CBC6_12CBB6 = 0;
	globeFrame_12CB7A_12CB6A = globeAnimation_12CB72_12CB62;
	globeFrameIndex_12CB7E_12CB6E = 0;
	timerFrame_12CB88_12CB78 = timerAnimation_12CB80_12CB70;
	*animDecodeMode_967E4_967E4 = 1;
	inputFlags_12CAE4_12CAD4[0] &= 0xBE;
	idleStart_12CBB4_12CBA4 = timerTicks_AC5D4_AC5C4;
	InitInputChange_356E0_35AA0();
	PlayMusic_5D290_5D7A0(4);
	if (attractFlags_12CBD6_12CBC6 & 4)
	{
		// back from the demo
		mc1Settings_AE408_AE3F8->levelIndex_0x11_17 = (uint16_t)demoLevel_12CBB8_12CBA8;
		memcpy(mc1State_AE400_AE3F0->players_0x340B_13323[0].progress_0x7CB_1995, demoProgress_12CA60_12CA50, 0x18);
		attractFlags_12CBD6_12CBC6 &= 0xFB;
		mc1Settings_AE408_AE3F8->flags_0x00_0 &= ~0x24u;
	}
}

void ExitMainMenu_4CAC0_4CE00()
{
	if (mainMenuItem_12CBCE_12CBBE != 5)
	{
		FadeInOut_61CC0_621D0(nullptr, 0x10, false, 10);
		if (!inputDevices_90B23_90B23)
			SetCursor_5C05C_5C56C(bitmap_pos_struct_t{});
		if (*videoMode_12F02E_12F01E & 1)
			ClearScreen320_411D7_41517(*workScreen_12EFF4_12EFE4, 320, 200, 0);
		else
			ClearScreen640_411FD_4153D(*workScreen_12EFF4_12EFE4, 640, 480, 0);
		BlitScreen();
	}
	FreeFileGroup_61610_61B20(mainMenuFiles_49D64_4A0A4);
	FreeSpritesIndex(&mainMenuSpritesIndex);
	FreeSpritesIndex(&mainMenuFontIndex);
	*animDecodeMode_967E4_967E4 = 0;
	menuFlags_12CBD5_12CBC5 &= 0xFE;
}

void DrawMainMenu_4CB70_4CEB0()
{
	switch (mainMenuMode_12CBCD_12CBBD)
	{
	case 0:
		DrawSprite(0x166, 0x0A, mainMenuSpritesIndex[1]);
		DrawSprite(0x150, 0x56, mainMenuSpritesIndex[2]);
		break;
	case 1:
	case 2:
		DrawSlotNames_4D300_4D640();
		break;
	}
}

void DrawLoadConfirm_4CC00_4CF40()
{
	SetViewport_65EB0_663C0(0x41, 0x4B, 0xBD, 0x2C);
	DrawSprite(0x18, 8, mainMenuSpritesIndex[1]);
	// dword_96860[type]: the name of the slot
	DrawText_51560_518A0(&mainMenuFont_9688C_9688C, 0x1E, 0x0C,
		slotNames_96864_96864[mainMenuAreas_4A12C_4A46C[mainMenuArea_12CBC6_12CBB6].type_4 - 1]);
	SetViewport_65EB0_663C0(0, 0, 0x140, 0xC8);
}

void DrawSlotNames_4D300_4D640()
{
	mainMenuFont_9688C_9688C.color1_0x06_6 = FindNearestColor_5CC70_5D180(menuPalette_12CB9C_12CB8C, 0x3F, 0x3F, 0);
	SaveTextWindow_513E0_51720();
	for (int i = 0; i < 6; i++)
	{
		const Type_Mc1SlotWindow& window = slotWindows_4A7EC_4AB2C[i];
		SetTextWindow_51360_516A0(window.x_0, window.y_4, window.width_8, window.height_12);
		int16_t y = (int16_t)((window.height_12 - mainMenuFont_9688C_9688C.glyphs_0x00_0[0].height_5) / 2);
		DrawText_51560_518A0(&mainMenuFont_9688C_9688C, 4, y, slotNames_96864_96864[i]);
	}
	RestoreTextWindow_51400_51740();
}

int32_t Confirm_4F8D0_4FC10(void (*draw)())
{
	int8_t* keys = *pressedKeys_12EEF0_12EEE0;
	bool done = false;
	int32_t result = 0;
	CopyFullScreen(backScreen_AE3FC_AE3EC, screenBackup_12CBA4_12CB94);
	if (!inputDevices_90B23_90B23)
		SetCursor_5C05C_5C56C(bitmap_pos_struct_t{});
	BlitScreen();
	// the scroll unrolls over the menu
	sprintf(moviePath_9ADC8_9ADC8, "%s", "intro/scroll.dat");
	frameCallback_AC5C4_AC5B4 = ScrollFrame_50470_507B0;
	skipOnInput_90026_90026 = 0;
	PlayMovie_107C0_107C0(0, 0, scrollEvents_4A17C_4A4BC);
	CopyFullScreen(*workScreen_12EFF4_12EFE4, backScreen_AE3FC_AE3EC);
	frameCallback_AC5C4_AC5B4 = nullptr;
	if (!inputDevices_90B23_90B23)
		SetCursor_5C05C_5C56C(menuSpritesIndex[mainMenuItem_12CBCE_12CBBE]);
	SaveTextWindow_513E0_51720();
	SetTextWindow_51360_516A0(0x50, 0x49, 0x9F, 0x30);
	for (uint32_t frame = 0; !done; frame++)
	{
		CopyFullScreen(backScreen_AE3FC_AE3EC, *workScreen_12EFF4_12EFE4);
		if (!inputDevices_90B23_90B23 || ((inputDevices_90B23_90B23 & 0x23) && !(frame & 3)))
			UpdateMenuInput_50770_50AB0();
		DrawSprite(0x88, 0xD2, mainMenuSpritesIndex[5]);
		DrawSprite(0x1E0, 0xD2, mainMenuSpritesIndex[6]);
		if (draw)
			draw();
		if ((inputFlags_12CAE4_12CAD4[0] & 1) || keys[0x01] || keys[0x1C])
		{
			int16_t x = clickX_12CAE0_12CAD0 >> 1;
			int16_t y = clickY_12CAE2_12CAD2 >> 1;
			if ((x >= 0x44 && x <= 0x51 && y >= 0x6A && y <= 0x74) || keys[0x1C])
			{
				done = true;
				result = 1;
			}
			else if ((x >= 0xF0 && x <= 0xFA && y >= 0x69 && y <= 0x73) || keys[0x01])
			{
				done = true;
				result = 0;
			}
			*mouseLeftPressed_12EFDE_12EFCE = 0;
			inputFlags_12CAE4_12CAD4[0] &= 0xFE;
		}
		if (inputDevices_90B23_90B23 & 0x23)
			DrawSprite(mousePos_9AD90_9AD90->x, mousePos_9AD90_9AD90->y, menuSpritesIndex[mainMenuItem_12CBCE_12CBBE]);
		BlitScreen();
	}
	RestoreTextWindow_51400_51740();
	CopyFullScreen(screenBackup_12CBA4_12CB94, backScreen_AE3FC_AE3EC);
	CopyFullScreen(backScreen_AE3FC_AE3EC, *workScreen_12EFF4_12EFE4);
	BlitScreen();
	*mouseRightPressed_12EFDC_12EFCC = 0;
	*mouseLeftPressed_12EFDE_12EFCE = 0;
	inputFlags_12CAE4_12CAD4[0] &= 0xBE;
	keys[0x01] = 0;
	keys[0x1C] = 0;
	return result;
}

void QuitGame_4FD20_50060()
{
	if (Confirm_4F8D0_4FC10(DrawQuitConfirm_4FE00_50140))
	{
		ExitMainMenu_4CAC0_4CE00();
		mc1State_AE400_AE3F0->players_0x340B_13323[mc1State_AE400_AE3F0->currentPlayer_0x08_8].exitGame_0x04_4 = 1;
		menuDone_96880_96880 = 1;
		return;
	}
	CopyFullScreen(screenBackup2_12CBA8_12CB98, backScreen_AE3FC_AE3EC);
	mainMenuMode_12CBCD_12CBBD = 0;
	DrawMainMenuIntoBackScreen();
}

void DrawQuitConfirm_4FE00_50140()
{
	SetViewport_65EB0_663C0(0x41, 0x4B, 0xBD, 0x2C);
	const bitmap_pos_struct_t& sprite = mainMenuSpritesIndex[7];
	DrawSprite((int16_t)(0xBD - (sprite.width_4 >> 1)), (int16_t)(0x2C - (sprite.height_5 >> 1)), sprite);
	SetViewport_65EB0_663C0(0, 0, 0x140, 0xC8);
}

void NetworkGame_4FEB0_501F0()
{
	if (!networkFound_93958_93958)
		return;
	mainMenuItem_12CBCE_12CBBE = 4;
	// player 0 kept for the network game (0x801 bytes)
	mc1State_AE400_AE3F0->networkPlayer_0x2409_9225 = mc1State_AE400_AE3F0->players_0x340B_13323[0];
	menuFlags_12CBD5_12CBC5 |= 8;
	mc1Settings_AE408_AE3F8->networkLevel_0x13_19 = mc1Settings_AE408_AE3F8->levelIndex_0x11_17;
}

void ContinueGame_4FF00_50240()
{
	menuFlags_12CBD5_12CBC5 |= 8;
	menuDone_96880_96880 = 1;
	newGame_9687C_9687C = 0;
	mainMenuItem_12CBCE_12CBBE = 5;
}

void NewGame_4FF30_50270()
{
	if (newGame_9687C_9687C)
	{
		newGame_9687C_9687C = 0;
		menuFlags_12CBD5_12CBC5 |= 8;
		mainMenuItem_12CBCE_12CBBE = 5;
		menuDone_96880_96880 = 1;
		return;
	}
	if (!Confirm_4F8D0_4FC10(DrawNewGameConfirm_4FFE0_50320))
		return;
	mc1Settings_AE408_AE3F8->levelIndex_0x11_17 = 0;
	mainMenuItem_12CBCE_12CBBE = 5;
	memset(mc1State_AE400_AE3F0->players_0x340B_13323[mc1State_AE400_AE3F0->currentPlayer_0x08_8].progress_0x7CB_1995, 0, 0x18);
	menuDone_96880_96880 = 1;
	menuFlags_12CBD5_12CBC5 |= 8;
}

void DrawNewGameConfirm_4FFE0_50320()
{
	Type_Mc1TextWindow_12C970 window;
	GetTextWindow_513A0_516E0(&window);
	// AE2C8: text 36
	const char* text = (const char*)textPointers_AE238_AE228[36];
	int32_t width = TextWidth_51420_51760(&mainMenuFont_9688C_9688C, text);
	DrawText_51560_518A0(&mainMenuFont_9688C_9688C, (int16_t)((window.width_16 - width) / 2), 0x0C, text);
}

void ScrollFrame_50470_507B0()
{
	// the transparent pixels of the scroll show the menu
	uint8_t* screen = *workScreen_12EFF4_12EFE4;
	const uint8_t* menu = screenBackup_12CBA4_12CB94;
	for (int i = 0; i < 0xFA00; i++)
		if (!screen[i])
			screen[i] = menu[i];
}

void BuildBrightTable_504A0_507E0()
{
	const uint8_t* palette = menuPalette_12CB9C_12CB8C;
	for (int i = 0; i < 0x100; i++)
	{
		uint16_t red = (uint16_t)(palette[i * 3] * 30 / 100 + palette[i * 3]);
		if (red > 0x3F)
			red = 0x3F;
		uint16_t green = (uint16_t)(palette[i * 3 + 1] * 30 / 100 + palette[i * 3 + 1]);
		if (green > 0x3F)
			green = 0x3F;
		uint16_t blue = (uint16_t)(palette[i * 3 + 2] * 30 / 100 + palette[i * 3 + 2]);
		if (blue > 0x3F)
			blue = 0x3F;
		brightTable_12CBAC_12CB9C[i] = FindNearestColor_5CC70_5D180(palette, (uint8_t)red, (uint8_t)green, (uint8_t)blue);
	}
}

void ReadSlotNames_51A10_51D50()
{
	int32_t version = 4;
	for (uint8_t slot = 0; slot < 6; slot++)
	{
		// C:\carpet.cd\save\carpdd%02X.gam
		char name[64];
		sprintf(name, "/save/carpdd%02X.gam", slot);
		std::string path = Mc1InstallPath() + name;
		FILE* file = DataFileIO::CreateOrOpenFile(path.c_str(), 0x200);
		if (!file)
		{
			sprintf(slotNames_96864_96864[slot], "--");
			continue;
		}
		DataFileIO::Read(file, (uint8_t*)&version, 4);
		if (version == 4)
			DataFileIO::Read(file, (uint8_t*)slotNames_96864_96864[slot], 0x14);
		else
			sprintf(slotNames_96864_96864[slot], "--");
		DataFileIO::Close(file);
	}
}

void HighlightArea_51E84_521C4(const uint8_t* source, uint8_t* dest, const uint8_t* mask, const uint8_t* table, uint8_t area)
{
	if (!source || !dest || !table)
		return;
	uint32_t count = *screenHeight_12EFF8_12EFE8 * *screenWidth_12EFF0_12EFE0;
	for (uint32_t i = 0; i < count; i++)
		dest[i] = mask[i] == area ? table[source[i]] : source[i];
}
