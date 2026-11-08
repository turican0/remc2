#include "Mc1Screen.h"

#include <cstdio>
#include <cstdlib>

#include "Mc1Files.h"
#include "Mc1Game.h"
#include "Mc1NotPorted.h"
#include "../../remc2/engine/Basic.h"
#include "../../remc2/engine/Graphics.h"
#include "../../remc2/engine/PlayerInput.h"
#include "../../remc2/engine/read_config.h"
#include "../../remc2/portability/port_sdl_vga_mouse.h"

uint8_t graphicsInitialized_93A84_93A84 = 0;
uint8_t highResScreen_90708_90708 = 0; // 1 after 31600 / 31640 set the mode for screen size 1
uint8_t savedScreenSize_90754_90754 = 0;

static void SetVideoModeAndPalette(uint8_t* palette)
{
	if (*videoMode_12F02E_12F01E & 1)
		SetVideoMode320x200AndPalette_61B90_620A0((TColor*)palette);
	else
		SetVideoMode640x480AndPalette_61C30_62140((TColor*)palette);
}

void InitGraphics_44730_44A70()
{
	if (graphicsInitialized_93A84_93A84)
		return;
	graphicsInitialized_93A84_93A84 = 1;
	// port: opens the window, the first palette call does it in MC2
	VGA_Init(windowResWidth, windowResHeight, gameResWidth, gameResHeight, maintainAspectRatio, displayIndex);
	if (*videoMode_12F02E_12F01E & 1)
		ClearScreen320_411D7_41517(*workScreen_12EFF4_12EFE4, 320, 200, 0);
	else
		ClearScreen640_411FD_4153D(*workScreen_12EFF4_12EFE4, 640, 480, 0);
	if (mc1Settings_AE408_AE3F8->flags_0x00_0 & 0x110)
		SetVideoModeAndPalette(palette_AE428_AE418);
	else
	{
		FadeInOut_61CC0_621D0(nullptr, 0x10, false, 10);
		SetVideoModeAndPalette(*workScreen_12EFF4_12EFE4); // the cleared screen is a black palette
	}
	InstallMouse_5C214_5C724();
	if (!*mouseInstalled_9AD98_9AD98)
	{
		ShutdownGraphics_44840_44B80();
		printf("ERROR : MOUSE DRIVER NOT FOUND.\n");
		exit(1);
	}
	SetCursor_5C05C_5C56C(pointersIndex[0]); // the first record of pointers.tab
	if (!(mc1Settings_AE408_AE3F8->flags_0x00_0 & 8))
		ClearPressedKeys_36D83_37143();
	// with flag 8 the original calls 5B5E0 / 5BAF0, an empty function
}

void ResetMouse_3F370_3F6B0()
{
	// int 33h function 0 (reset)
	*mouseInstalled_9AD98_9AD98 = 0;
}

void ShutdownGraphics_44840_44B80()
{
	if (graphicsInitialized_93A84_93A84 != 1)
		return;
	ResetMouse_3F370_3F6B0();
	// flag 8: 5B5F4 / 5BB04 is empty, otherwise 36DC2 / 37182 restores int 9, nothing to do in the port
	// 5CD70 / 5D280 restores the text mode of the start (MC2 8C21F, removed in remc2); Mc1Main closes the window
	sub_433C0_43700();
	RestoreTimer_5A4EA_5A9FA();
}

void FreeCursorBuffers()
{
	// port: the cursor buffers of InstallMouse_5C214_5C724, remc2 never frees them
	free(*cursorData_12EFC8_12EFB8);
	*cursorData_12EFC8_12EFB8 = nullptr;
	free(*cursorBackground_12EF98_12EF88);
	*cursorBackground_12EF98_12EF88 = nullptr;
	free(*cursorBackground2_12EF88_12EF78);
	*cursorBackground2_12EF88_12EF78 = nullptr;
}

void SetScreenModeAndPalette_315C0_31600()
{
	if (*videoMode_12F02E_12F01E & 1)
		SetVideoMode320x200AndPalette_61B90_620A0((TColor*)palette_AE428_AE418);
	else
		SetVideoMode640x480AndPalette_61C30_62140((TColor*)palette_AE428_AE418);
	highResScreen_90708_90708 = 0;
}

void SetFullScreen_31A00_31A40()
{
	// MC2 SetPausedMenuOpen_41AF0 without the pause counter
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	if (!state->screenSize_0x219B_8603)
		return;
	if (state->vfx_0x219E_8606 && *renderWidth_93AD4_93AD4 == 640)
		ClearScreenAndBlit_5B560_5BA70();
	savedScreenSize_90754_90754 = state->screenSize_0x219B_8603;
	SetScreenModeAndPalette_315C0_31600();
	state->screenSize_0x219B_8603 = 0;
	PaletteInstall_319A0_319E0((TColor*)palette_AE428_AE418);
}

void RestoreScreenSize_31A60_31AA0()
{
	mc1State_AE400_AE3F0->screenSize_0x219B_8603 = savedScreenSize_90754_90754;
	if (savedScreenSize_90754_90754 == 1)
		sub_31600_31640();
	savedScreenSize_90754_90754 = 0;
}
