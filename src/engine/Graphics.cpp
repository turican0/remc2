#include "Graphics.h"

#include "Basic.h"

GameRenderInterface* m_ptrGameRender;

uint8_t loc_A0000_vga_buffer[307200];
TColor tempPalette_EA3B8x[256]; // weak?x_DWORD_E9C4C_langindexbuffer[475]
int16_t x_WORD_180662_graphics_handle; // weak
bitmap_pos_struct2_t* x_DWORD_D4188 = 0; // weak
bitmap_pos_struct2_t* x_DWORD_D418C = 0; // weak
bitmap_pos_struct2_t* x_DWORD_D4190 = 0; // weak


//----- (000A0D50) --------------------------------------------------------
void sub_A0D50_set_viewport(uint16_t posX, uint16_t posY, uint16_t width, uint16_t height)//281d50
{
	x_DWORD_18063C_sprite_sizex = posX;
	x_DWORD_180650_positiony = posY;
	x_DWORD_180648_map_resolution2_x = width;
	x_DWORD_180644_map_resolution2_y = height;
	x_DWORD_180634_screen_width = width + posX;
	x_DWORD_180630_screen_height = height + posY;
}
