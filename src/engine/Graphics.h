#pragma once

#include <cstdint>
#include "engine_support.h"
#include "ViewPort.h"
#include "GameRenderInterface.h"

extern GameRenderInterface* m_ptrGameRender;

extern uint8_t loc_A0000_vga_buffer[307200];
extern TColor tempPalette_EA3B8x[256];
extern int16_t x_WORD_180662_graphics_handle;
extern bitmap_pos_struct2_t* x_DWORD_D4188;
extern bitmap_pos_struct2_t* x_DWORD_D418C;
extern bitmap_pos_struct2_t* x_DWORD_D4190;

void sub_A0D50_set_viewport(uint16_t posX, uint16_t posY, uint16_t width, uint16_t height);
