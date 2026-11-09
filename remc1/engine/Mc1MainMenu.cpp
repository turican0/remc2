#include "Mc1MainMenu.h"

#include <cstdio>
#include <cstring>
#include <string>
#include <utility>

#include "Mc1Game.h"
#include "Mc1Graphics.h"
#include "Mc1Install.h"
#include "Mc1Menu.h"
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
	{ EnterNameAndNickname_50030_50370, 0 },
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
					LoadGame_51AF0_51E30((uint8_t)area.type_4);
				redrawn = true;
				RedrawMainMenu();
				break;
			case 2:
				if (area.type_4 >= 1 && area.type_4 <= 6 && SaveDialog_4CCA0_4CFE0())
					SaveGame_51C90_51FD0((uint8_t)area.type_4);
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

// scan code -> allowed in the nickname (12CBD5 & 20h), without / with Shift
const uint8_t nicknameKeys_4A84B_4AB8B[0x3A] = {
	0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const uint8_t nicknameKeysShift_4A884_4ABC4[0x3A] = {
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
};
// scan code -> character, without / with Shift (UK keyboard); each table ends with the first byte of the next
const uint8_t keyChars_4A905_4AC45[0x3A] = {
	0x00, 0x00, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x2D, 0x3D, 0x00, 0x00,
	0x71, 0x77, 0x65, 0x72, 0x74, 0x79, 0x75, 0x69, 0x6F, 0x70, 0x5B, 0x5D, 0x00, 0x00, 0x61, 0x73,
	0x64, 0x66, 0x67, 0x68, 0x6A, 0x6B, 0x6C, 0x3B, 0x27, 0x60, 0x00, 0x23, 0x7A, 0x78, 0x63, 0x76,
	0x62, 0x6E, 0x6D, 0x2C, 0x2E, 0x2F, 0x00, 0x00, 0x00, 0x20,
};
const uint8_t keyCharsShift_4A93E_4AC7E[0x3A] = {
	0x20, 0x00, 0x21, 0x22, 0x5C, 0x24, 0x25, 0x5E, 0x26, 0x2A, 0x28, 0x29, 0x5F, 0x2B, 0x00, 0x00,
	0x51, 0x57, 0x45, 0x52, 0x54, 0x59, 0x55, 0x49, 0x4F, 0x50, 0x7B, 0x7D, 0x00, 0x00, 0x41, 0x53,
	0x44, 0x46, 0x47, 0x48, 0x4A, 0x4B, 0x4C, 0x3A, 0x40, 0x7E, 0x00, 0x7C, 0x5A, 0x58, 0x43, 0x56,
	0x42, 0x4E, 0x4D, 0x3C, 0x3E, 0x3F, 0x00, 0x00, 0x00, 0x20,
};

static void DrawPrompt(const char* text)
{
	Type_Mc1TextWindow_12C970 window;
	GetTextWindow_513A0_516E0(&window);
	CopyFullScreen(backScreen_AE3FC_AE3EC, *workScreen_12EFF4_12EFE4);
	int32_t width = TextWidth_51420_51760(&mainMenuFont_9688C_9688C, text);
	DrawText_51560_518A0(&mainMenuFont_9688C_9688C, (int16_t)((window.width_16 - width) / 2), 0x0A, text);
}

// the scroll unrolls over the menu (4F8D0, 50030, 4CCA0)
static void UnrollScroll()
{
	CopyFullScreen(backScreen_AE3FC_AE3EC, screenBackup_12CBA4_12CB94);
	if (!inputDevices_90B23_90B23)
		SetCursor_5C05C_5C56C(bitmap_pos_struct_t{});
	BlitScreen();
	sprintf(moviePath_9ADC8_9ADC8, "%s", "intro/scroll.dat");
	frameCallback_AC5C4_AC5B4 = ScrollFrame_50470_507B0;
	skipOnInput_90026_90026 = 0;
	PlayMovie_107C0_107C0(0, 0, scrollEvents_4A17C_4A4BC);
	CopyFullScreen(*workScreen_12EFF4_12EFE4, backScreen_AE3FC_AE3EC);
	frameCallback_AC5C4_AC5B4 = nullptr;
	if (!inputDevices_90B23_90B23)
		SetCursor_5C05C_5C56C(menuSpritesIndex[mainMenuItem_12CBCE_12CBBE]);
}

void EnterNameAndNickname_50030_50370()
{
	int8_t* keys = *pressedKeys_12EEF0_12EEE0;
	UnrollScroll();
	SaveTextWindow_513E0_51720();
	SetTextWindow_51360_516A0(0x46, 0x49, 0xB3, 0x30);
	TextInput_50D40_51080(8, 0x19, 0x1E, nullptr, mc1Settings_AE408_AE3F8->playerName_0x1D_29, 0xFF, 0,
		DrawNamePrompt_50350_50690, 1, &mainMenuFont_9688C_9688C);
	// the nickname only of the letters and digits of nicknameKeys_4A84B_4AB8B
	menuFlags_12CBD5_12CBC5 |= 0x20;
	keys[0x1C] = 0;
	inputFlags_12CAE4_12CAD4[0] &= 0xFE;
	TextInput_50D40_51080(8, 0x19, 8, nullptr, mc1Settings_AE408_AE3F8->nickname_0x3D_61, 0xFF, 0,
		DrawNicknamePrompt_503E0_50720, 1, &mainMenuFont_9688C_9688C);
	keys[0x1C] = 0;
	menuFlags_12CBD5_12CBC5 &= 0xDF;
	inputFlags_12CAE4_12CAD4[0] &= 0xFE;
	CopyFullScreen(screenBackup_12CBA4_12CB94, backScreen_AE3FC_AE3EC);
	mainMenuItem_12CBCE_12CBBE = 2;
	DrawMainMenuIntoBackScreen();
	// the drawn menu is covered again, the next frame of the menu draws it
	CopyFullScreen(screenBackup_12CBA4_12CB94, backScreen_AE3FC_AE3EC);
	CopyFullScreen(backScreen_AE3FC_AE3EC, *workScreen_12EFF4_12EFE4);
	BlitScreen();
	RestoreTextWindow_51400_51740();
}

void DrawNamePrompt_50350_50690()
{
	// AE2C0: text 34
	DrawPrompt((const char*)textPointers_AE238_AE228[34]);
}

void DrawNicknamePrompt_503E0_50720()
{
	// AE2C4: text 35
	DrawPrompt((const char*)textPointers_AE238_AE228[35]);
}

int32_t TextInput_50D40_51080(int32_t x, int16_t y, int16_t maxLength, const char* prompt, char* text, uint8_t color,
	int32_t /*unused*/, void (*draw)(), uint8_t align, Type_Mc1Font_96884* font)
{
	char* cursor = text;
	char* end = text + strlen(text);
	char* limit = text + maxLength;
	const bool insert = true; // never changed, Insert does nothing
	int32_t textX = 0;
	int32_t result = 0; // 1 Enter / click, 2 Esc
	*mouseRightPressed_12EFDC_12EFCC = 0;
	*mouseLeftPressed_12EFDE_12EFCE = 0;
	Type_Mc1TextWindow_12C970 window;
	GetTextWindow_513A0_516E0(&window);
	if (prompt)
		textX = TextWidth_51420_51760(font, prompt);
	textX += x;
	do
	{
		if (*mouseLeftPressed_12EFDE_12EFCE)
			result = 1;
		uint8_t key = *lastKey_12EF70_12EF60;
		switch (key)
		{
		case 0x0F: // Tab
		case 0x2A: // Shift
		case 0x36:
		case 0x52: // Insert
			break;
		case 0x01: // Esc
			result = 2;
			break;
		case 0x1C: // Enter
			result = 1;
			break;
		case 0x0E: // Backspace
			if (cursor > text)
			{
				cursor--;
				end--;
				DeleteText_51330_51670(cursor, 1);
			}
			break;
		case 0x53: // Delete
			if (cursor < end)
			{
				end--;
				DeleteText_51330_51670(cursor, 1);
			}
			break;
		case 0x4B: // Left
			if (cursor > text)
				cursor--;
			break;
		case 0x4D: // Right
			if (cursor < end)
				cursor++;
			break;
		case 0x47: // Home
			cursor = text;
			break;
		case 0x4F: // End
			cursor = end;
			break;
		default:
		{
			bool shift = (*pressedKeys_12EEF0_12EEE0)[0x2A] || (*pressedKeys_12EEF0_12EEE0)[0x36];
			// port: the keys over 39h are tested first, the original reads its tables beyond their end for them
			if (!key || key > 0x39)
				break;
			if ((menuFlags_12CBD5_12CBC5 & 0x20) && !(shift ? nicknameKeysShift_4A884_4ABC4 : nicknameKeys_4A84B_4AB8B)[key])
				break;
			if (!((insert && end < limit) || (!insert && cursor < limit)))
				break;
			char typed[2] = { (char)(shift ? keyCharsShift_4A93E_4AC7E : keyChars_4A905_4AC45)[key], 0 };
			if (insert)
			{
				InsertText_512D0_51610(cursor, typed);
				if (cursor < limit)
				{
					cursor++;
					end++;
				}
			}
			else
			{
				*cursor = typed[0];
				if (cursor < limit)
					cursor++;
				if (cursor > end)
				{
					end = cursor;
					*cursor = 0;
				}
			}
			// 50FE0: the character that does not fit the window is removed again
			int32_t room = align == 1 ? window.width_16 - 2 * (int16_t)x : window.width_16 - (int16_t)x;
			if (TextWidth_51420_51760(font, text) >= room && insert && cursor > text)
			{
				cursor--;
				end--;
				DeleteText_51330_51670(cursor, 1);
			}
			break;
		}
		}
		*lastKey_12EF70_12EF60 = 0;
		draw();
		font->color1_0x06_6 = color;
		DrawText_51560_518A0(font, (int16_t)x, y, prompt);
		if (align == 0)
			DrawText_51560_518A0(font, (int16_t)textX, y, text);
		else if (align == 1)
			DrawText_51560_518A0(font, (int16_t)((window.width_16 - TextWidth_51420_51760(font, text)) / 2), y, text);
		// 5110F: the cursor under the character at the cursor (a space at the end)
		char before[0x90];
		int16_t index = (int16_t)(cursor - text);
		strcpy(before, text);
		uint8_t under = (uint8_t)before[index];
		if (!under)
			under = ' ';
		before[index] = 0;
		const bitmap_pos_struct_t& glyph = font->glyphs_0x00_0[under - 0x20];
		int32_t cursorX;
		if (align == 0)
			cursorX = TextWidth_51420_51760(font, before) + (int16_t)textX + window.left_0;
		else
			cursorX = (window.width_16 - TextWidth_51420_51760(font, text)) / 2 + TextWidth_51420_51760(font, before)
				+ window.left_0;
		FillRect_51ECC_5220C(cursorX, y + glyph.height_5 - 2 + window.top_8, glyph.width_4, 2, 0xFF, 0);
		BlitScreen();
	} while (!(int16_t)result);
	*mouseRightPressed_12EFDC_12EFCC = 0;
	*mouseLeftPressed_12EFDE_12EFCE = 0;
	*lastKey_12EF70_12EF60 = 0;
	(*pressedKeys_12EEF0_12EEE0)[0x01] = 0;
	(*pressedKeys_12EEF0_12EEE0)[0x1C] = 0;
	return (int16_t)result == 1;
}

char* InsertText_512D0_51610(char* dest, const char* text)
{
	int16_t length = (int16_t)strlen(text);
	memmove(dest + length, dest, strlen(dest) + 1);
	memcpy(dest, text, length);
	return dest;
}

void DeleteText_51330_51670(char* dest, int16_t count)
{
	memmove(dest, dest + count, strlen(dest + count) + 1);
}

void FillRect_51ECC_5220C(int32_t x, int32_t y, int32_t width, int32_t height, uint8_t color, uint8_t mode)
{
	// in the viewport of SetViewport_65EB0_663C0
	if (y >= *viewportBottom_12F010_12F000)
		return;
	if (y < 0)
	{
		height += y;
		y = 0;
	}
	if (y + height > *viewportHeight_12F028_12F018)
		height = *viewportHeight_12F028_12F018 - y;
	if (height <= 0)
		return;
	if (x >= *viewportRight_12F024_12F014)
		return;
	if (x < 0)
	{
		width += x;
		x = 0;
	}
	if (x + width > *viewportWidth_12F000_12EFF0)
		width = *viewportWidth_12F000_12EFF0 - x;
	if (width <= 0)
		return;
	// 51F68: modes 1-3 draw nothing
	if ((mode & 3) != 0)
		return;
	uint8_t* dest = *workScreen_12EFF4_12EFE4 + (y + *viewportY_12F018_12F008) * (int32_t)*screenWidth_12EFF0_12EFE0 + x
		+ *viewportX_12F008_12EFF8;
	for (int32_t row = 0; row < height; row++, dest += *screenWidth_12EFF0_12EFE0)
		memset(dest, color, width);
}

int32_t SaveDialog_4CCA0_4CFE0()
{
	int8_t* keys = *pressedKeys_12EEF0_12EEE0;
	bool done = false;
	int32_t result = 0;
	menuFlags_12CBD5_12CBC5 &= 0xBF;
	UnrollScroll();
	SaveTextWindow_513E0_51720();
	SetTextWindow_51360_516A0(0x50, 0x49, 0x9F, 0x30);
	// an empty slot asks for the name first
	if (!strcmp(slotNames_96864_96864[mainMenuAreas_4A12C_4A46C[mainMenuArea_12CBC6_12CBB6].type_4 - 1], "--"))
		EditSlotName_4D0F0_4D430();
	for (uint32_t frame = 0; !done; frame++)
	{
		if (!inputDevices_90B23_90B23 || ((inputDevices_90B23_90B23 & 0x23) && !(frame & 3)))
			UpdateMenuInput_50770_50AB0();
		if (keys[0x1C])
		{
			done = true;
			result = 1;
			*mouseLeftPressed_12EFDE_12EFCE = 0;
			inputFlags_12CAE4_12CAD4[0] &= 0xFE;
			keys[0x1C] = 0;
			keys[0x01] = 0;
		}
		if (keys[0x01])
		{
			done = true;
			result = 0;
			*mouseLeftPressed_12EFDE_12EFCE = 0;
			keys[0x1C] = 0;
			inputFlags_12CAE4_12CAD4[0] &= 0xFE;
			keys[0x01] = 0;
		}
		if (inputFlags_12CAE4_12CAD4[0] & 1)
		{
			int16_t x = clickX_12CAE0_12CAD0 >> 1;
			int16_t y = clickY_12CAE2_12CAD2 >> 1;
			if (x >= 0x44 && x <= 0x51 && y >= 0x6A && y <= 0x74)
			{
				done = true;
				result = 1;
			}
			if (x >= 0xF0 && x <= 0xFA && y >= 0x69 && y <= 0x73)
			{
				done = true;
				result = 0;
			}
			// the name
			if (x >= 0x6E && x <= 0xE6 && y >= 0x55 && y <= 0x5F)
			{
				EditSlotName_4D0F0_4D430();
				keys[0x1C] = 0;
				keys[0x01] = 0;
			}
			*mouseLeftPressed_12EFDE_12EFCE = 0;
			inputFlags_12CAE4_12CAD4[0] &= 0xFE;
			keys[0x1C] = 0;
			keys[0x01] = 0;
		}
		DrawSaveDialog_4D190_4D4D0();
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

void EditSlotName_4D0F0_4D430()
{
	int8_t* keys = *pressedKeys_12EEF0_12EEE0;
	menuFlags_12CBD5_12CBC5 |= 0x40;
	keys[0x1C] = 0;
	*mouseRightPressed_12EFDC_12EFCC = 0;
	keys[0x01] = 0;
	*mouseMiddlePressed_12EFDA_12EFCA = 0;
	*mouseLeftPressed_12EFDE_12EFCE = 0;
	TextInput_50D40_51080(0x1E, 0x0C, 0x14, nullptr,
		slotNames_96864_96864[mainMenuAreas_4A12C_4A46C[mainMenuArea_12CBC6_12CBB6].type_4 - 1], 0xFF, 0,
		DrawSaveDialog_4D190_4D4D0, 0, &mainMenuFont_9688C_9688C);
	menuFlags_12CBD5_12CBC5 &= 0xBF;
}

void DrawSaveDialog_4D190_4D4D0()
{
	CopyFullScreen(backScreen_AE3FC_AE3EC, *workScreen_12EFF4_12EFE4);
	SetViewport_65EB0_663C0(0x41, 0x4B, 0xBD, 0x2C);
	DrawSprite(0x18, 8, mainMenuSpritesIndex[2]);
	SetViewport_65EB0_663C0(0, 0, 0x140, 0xC8);
	// 12CBD5 & 40h: the name is being edited (TextInput_50D40_51080 draws it)
	if (menuFlags_12CBD5_12CBC5 & 0x40)
		return;
	DrawText_51560_518A0(&mainMenuFont_9688C_9688C, 0x1E, 0x0C,
		slotNames_96864_96864[mainMenuAreas_4A12C_4A46C[mainMenuArea_12CBC6_12CBB6].type_4 - 1]);
	DrawSprite(0x88, 0xD2, mainMenuSpritesIndex[5]);
	DrawSprite(0x1E0, 0xD2, mainMenuSpritesIndex[6]);
}

// the saved game C:\carpet.cd\save\carpdd%02X.gam: only bytes, no pointers (the save in the game needs shadow
// structures as remc2 has them)
static std::string SavePath(uint8_t slot)
{
	char name[64];
	sprintf(name, "/save/carpdd%02X.gam", slot);
	return Mc1InstallPath() + name;
}

int32_t LoadGame_51AF0_51E30(uint8_t slot)
{
	int32_t version = 4;
	int32_t result = 0;
	uint8_t index = slot - 1;
	// the level is written as (level + 12CBD0 + 12CBD1) * 4
	int32_t level = (mc1Settings_AE408_AE3F8->levelIndex_0x11_17 + byte_12CBD0_12CBC0 + byte_12CBD1_12CBC1) * 4;
	FILE* file = DataFileIO::CreateOrOpenFile(SavePath(index).c_str(), 0x200);
	if (!file)
		return 0;
	DataFileIO::Read(file, (uint8_t*)&version, 4);
	if (version == 4)
	{
		DataFileIO::Read(file, (uint8_t*)slotNames_96864_96864[index], 0x14);
		DataFileIO::Read(file, (uint8_t*)mc1Settings_AE408_AE3F8->playerName_0x1D_29, 0x20);
		DataFileIO::Read(file, (uint8_t*)mc1Settings_AE408_AE3F8->nickname_0x3D_61, 0x20);
		DataFileIO::Read(file, &mc1State_AE400_AE3F0->reflections_0x2195_8597, 0x0C);
		DataFileIO::Read(file, (uint8_t*)&level, 4);
		DataFileIO::Read(file, mc1State_AE400_AE3F0->players_0x340B_13323[0].progress_0x7CB_1995, 0x18);
		DataFileIO::Read(file, &byte_12CBD0_12CBC0, 1);
		DataFileIO::Read(file, &byte_12CBD1_12CBC1, 1);
		DataFileIO::Read(file, &mc1State_AE400_AE3F0->reflections_0x2195_8597, 0x0C);
		newGame_9687C_9687C = 0;
		mc1Settings_AE408_AE3F8->levelIndex_0x11_17 = (uint16_t)(level / version - byte_12CBD0_12CBC0 - byte_12CBD1_12CBC1);
		result = 1;
	}
	DataFileIO::Close(file);
	return result;
}

int32_t SaveGame_51C90_51FD0(uint8_t slot)
{
	int32_t version = 4;
	uint8_t index = slot - 1;
	int32_t level = (mc1Settings_AE408_AE3F8->levelIndex_0x11_17 + byte_12CBD0_12CBC0 + byte_12CBD1_12CBC1) * 4;
	FILE* file = DataFileIO::CreateOrOpenFile(SavePath(index).c_str(), 0x222);
	if (!file)
		return 0;
	WriteFile_62ED0_633E0(file, (uint8_t*)&version, 4);
	WriteFile_62ED0_633E0(file, (uint8_t*)slotNames_96864_96864[index], 0x14);
	WriteFile_62ED0_633E0(file, (uint8_t*)mc1Settings_AE408_AE3F8->playerName_0x1D_29, 0x20);
	WriteFile_62ED0_633E0(file, (uint8_t*)mc1Settings_AE408_AE3F8->nickname_0x3D_61, 0x20);
	WriteFile_62ED0_633E0(file, &mc1State_AE400_AE3F0->reflections_0x2195_8597, 0x0C);
	WriteFile_62ED0_633E0(file, (uint8_t*)&level, 4);
	WriteFile_62ED0_633E0(file, mc1State_AE400_AE3F0->players_0x340B_13323[0].progress_0x7CB_1995, 0x18);
	WriteFile_62ED0_633E0(file, &byte_12CBD0_12CBC0, 1);
	WriteFile_62ED0_633E0(file, &byte_12CBD1_12CBC1, 1);
	WriteFile_62ED0_633E0(file, &mc1State_AE400_AE3F0->reflections_0x2195_8597, 0x0C);
	DataFileIO::Close(file);
	return 1;
}
