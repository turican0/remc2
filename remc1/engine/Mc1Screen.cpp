#include "Mc1Screen.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Mc1Files.h"
#include "Mc1Game.h"
#include "Mc1NotPorted.h"
#include "Mc1Sound.h"
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
		SetStereoScreen_31600_31640();
	savedScreenSize_90754_90754 = 0;
}

int32_t GetVesaInfo_61AB0_61FC0()
{
	// int 31h 300h (int 10h 4F00h into the real mode buffer) is gone in the port, as int 10h in MC2: the buffer never
	// gets the signature
	return strncmp((const char*)vesaInfo_12F080_12F070, "VESA", 4);
}

int32_t HasVesaMode_30D30_30D70(uint16_t mode)
{
	// the modes are listed at the real mode far pointer of the VESA info (+0Eh), only read with its signature
	(void)mode;
	if (GetVesaInfo_61AB0_61FC0())
		return 0;
	return 0;
}

void ClearScreenAndBlitVfx_5B500_5BA10()
{
	// MC2 75C50 with 2 / 10h to the ports 302h / 303h (VFX1) instead of 2 / 1, both removed in the port
	memset(*workScreen_12EFF4_12EFE4, 0, *screenWidth_12EFF0_12EFE0 * *screenHeight_12EFF8_12EFE8);
	if (*videoMode_12F02E_12F01E & 1)
		Blit320_5CDA0_5D2B0(0);
	else
		Blit640_599B0_59EC0(480, 0);
}

// the red-blue tables: by the pixel of a 2x2 block the red of the second eye (+3, +1, -1, -3) and the green and blue
// of the first one (the other way)
uint8_t stereoColors_B32A0_B3290[8][256]; // B32A0, B33A0 ... B39A0
uint16_t hiColors_B3AA0_B3A90[2][256]; // hicolor: the red, the green and blue

static uint8_t StereoComponent(uint8_t color, int8_t shift)
{
	// 31720...: the 6-bit component moved and kept in 0-3Fh
	int8_t value = (int8_t)(color + shift);
	if (shift > 0 && value > 0x3F)
		value = 0x3F;
	if (shift < 0 && value < 0)
		value = 0;
	return (uint8_t)value;
}

static void MakeStereoTables(uint8_t* red, uint8_t* greenBlue, int8_t shift)
{
	const uint8_t* color = palette_AE428_AE418;
	for (int i = 0; i < 256; i++, color += 3)
	{
		red[i] = (uint8_t)((StereoComponent(color[0], shift) & 0x38) << 2);
		greenBlue[i] = (uint8_t)(((StereoComponent(color[1], -shift) & 0x38) >> 1) + ((StereoComponent(color[2], shift) & 0x30) >> 4));
	}
}

void SetStereoScreen_31600_31640()
{
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	if (state->screenSize_0x219B_8603 && *renderWidth_93AD4_93AD4 == 640)
		ClearScreenAndBlitVfx_5B500_5BA10();
	if (!*blurBuffer_AE404_AE3F4 || state->vfx_0x219E_8606)
		return;
	const uint16_t mode = *screenWidth_12EFF0_12EFE0 == 320 ? 0x10E : 0x111;
	if (HasVesaMode_30D30_30D70(mode))
	{
		// 619B8 sets the VESA mode (MC2 994BA, removed in remc2)
		highResScreen_90708_90708 = 1;
		*screenWidth_12EFF0_12EFE0 = *renderWidth_93AD4_93AD4;
		const uint8_t* color = palette_AE428_AE418;
		for (int i = 0; i < 256; i++, color += 3)
		{
			hiColors_B3AA0_B3A90[0][i] = (uint16_t)((color[0] & 0x3E) << 10);
			hiColors_B3AA0_B3A90[1][i] = (uint16_t)((((color[1] * 3 / 4) & 0x3F) << 5) + (((color[2] * 3 / 4) & 0x3E) >> 1));
		}
		return;
	}
	MakeStereoTables(stereoColors_B32A0_B3290[5], stereoColors_B32A0_B3290[4], 3); // B37A0, B36A0
	MakeStereoTables(stereoColors_B32A0_B3290[3], stereoColors_B32A0_B3290[6], 1); // B35A0, B38A0
	MakeStereoTables(stereoColors_B32A0_B3290[2], stereoColors_B32A0_B3290[7], -1); // B34A0, B39A0
	MakeStereoTables(stereoColors_B32A0_B3290[0], stereoColors_B32A0_B3290[1], -3); // B32A0, B33A0
	// the palette: 8 reds, 8 greens, 4 blues
	uint8_t palette[0x300];
	uint8_t* entry = palette;
	for (int r = 0; r < 8; r++)
		for (int g = 0; g < 8; g++)
			for (int b = 0; b < 4; b++)
			{
				*entry++ = (uint8_t)(r * 9);
				*entry++ = (uint8_t)(g * 7);
				*entry++ = (uint8_t)(b * 14);
			}
	PaletteInstall_319A0_319E0((TColor*)palette);
}

// one row of the red-blue picture: the red of the second eye (the blur buffer) and the green and blue of the first one
static void StereoRow(uint8_t* screen, const uint8_t* eye, int groups, const uint8_t* redEven, const uint8_t* greenBlueEven,
	const uint8_t* redOdd, const uint8_t* greenBlueOdd)
{
	for (; groups; groups--, screen += 4, eye += 4)
	{
		screen[0] = (uint8_t)(redEven[eye[0]] + greenBlueEven[screen[0]]);
		screen[1] = (uint8_t)(redOdd[eye[1]] + greenBlueOdd[screen[1]]);
		screen[2] = (uint8_t)(redEven[eye[2]] + greenBlueEven[screen[2]]);
		screen[3] = (uint8_t)(redOdd[eye[3]] + greenBlueOdd[screen[3]]);
	}
}

void BlendAndBlit_30B30_30B70()
{
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	if (state->screenSize_0x219B_8603 == 1 && !highResScreen_90708_90708)
	{
		const int groups = (uint16_t)viewPort.Width_DE564 >> 2;
		uint8_t* screen = ViewPortRenderBufferStart_DE558;
		const uint8_t* eye = *blurBuffer_AE404_AE3F4;
		const int pitch = *renderWidth_93AD4_93AD4;
		auto& tables = stereoColors_B32A0_B3290;
		for (int rows = (uint16_t)viewPort.Height_DE568 / 2; rows; rows--)
		{
			StereoRow(screen, eye, groups, tables[5], tables[4], tables[2], tables[7]);
			screen += pitch;
			eye += pitch;
			StereoRow(screen, eye, groups, tables[0], tables[1], tables[3], tables[6]);
			screen += pitch;
			eye += pitch;
		}
	}
	if (*videoMode_12F02E_12F01E & 1)
	{
		if (highResScreen_90708_90708)
			BlitHiColorBanks_79A1B_79F2B((uint8_t*)hiColors_B3AA0_B3A90);
		else
			Blit320_5CDA0_5D2B0(maxGameFps);
	}
	else if (state->vfx_0x219E_8606 && state->screenSize_0x219B_8603)
		BlitVfx_79B2D_7A03D();
	else if (highResScreen_90708_90708)
		BlitHiColor_79906_79E16((uint8_t*)hiColors_B3AA0_B3A90);
	else if (*videoMode_12F02E_12F01E & 1)
		Blit320_5CDA0_5D2B0(maxGameFps);
	else
		Blit640_599B0_59EC0(480, maxGameFps);
}
