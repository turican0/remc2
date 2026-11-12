#pragma once

#include <cstdint>

// MC1 screen, mouse and keyboard setup; the VGA layer itself (mode, palette, fades, cursor) is shared with remc2

extern uint8_t graphicsInitialized_93A84_93A84;
extern uint8_t highResScreen_90708_90708;
extern uint8_t savedScreenSize_90754_90754;
extern uint8_t stereoColors_B32A0_B3290[8][256];
extern uint16_t hiColors_B3AA0_B3A90[2][256];

void InitGraphics_44730_44A70();
void ShutdownGraphics_44840_44B80();
void ResetMouse_3F370_3F6B0();
void FreeCursorBuffers();
void SetScreenModeAndPalette_315C0_31600();
void SetFullScreen_31A00_31A40();
void RestoreScreenSize_31A60_31AA0();
int32_t GetVesaInfo_61AB0_61FC0();
int32_t HasVesaMode_30D30_30D70(uint16_t mode);
void ClearScreenAndBlitVfx_5B500_5BA10();
// screen size 1: the red-blue stereo (the palette and the tables of 30B30), hicolor when there is VESA
void SetStereoScreen_31600_31640();
// the frame to the screen: red-blue of the two eyes with screen size 1, then the blit of the mode
void BlendAndBlit_30B30_30B70();
