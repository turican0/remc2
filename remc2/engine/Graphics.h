#pragma once

#include <cstdint>
#include "engine_support.h"
#include "ViewPort.h"
#include "GameRenderInterface.h"

extern GameRenderInterface* m_ptrGameRender;

extern uint8_t vgaBuffer_A0000[307200];
extern TColor tempPalette_EA3B8x[256];
extern int16_t videoModeNumber_180662;
extern bitmap_pos_struct2_t* webSpritesData_D4188;
extern bitmap_pos_struct2_t* webSpritesTab_D418C;
extern bitmap_pos_struct2_t* webSpritesTabEnd_D4190;

void SetViewport_A0D50(uint16_t posX, uint16_t posY, uint16_t width, uint16_t height);
