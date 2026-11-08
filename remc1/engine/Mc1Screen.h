#pragma once

#include <cstdint>

// MC1 screen, mouse and keyboard setup; the VGA layer itself (mode, palette, fades, cursor) is shared with remc2

extern uint8_t graphicsInitialized_93A84_93A84;
extern uint8_t highResScreen_90708_90708;
extern uint8_t savedScreenSize_90754_90754;

void InitGraphics_44730_44A70();
void ShutdownGraphics_44840_44B80();
void ResetMouse_3F370_3F6B0();
void FreeCursorBuffers();
void SetScreenModeAndPalette_315C0_31600();
void SetFullScreen_31A00_31A40();
void RestoreScreenSize_31A60_31AA0();
