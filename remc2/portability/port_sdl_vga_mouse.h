#pragma once
#ifndef PORT_SDL_VGA_MOUSE_H
#define PORT_SDL_VGA_MOUSE_H

#include <cstdint>
#include <stdio.h>
#include <assert.h>
#include <sstream>
#include <stddef.h>
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */

#ifdef _MSC_VER
	#include "SDL.h"
#else
    #include "SDL2/SDL.h"
#endif

#include "port_sdl_sound.h"
#include "port_filesystem.h"

typedef struct
{
	int32_t x;
	int32_t y;
} POSITION;

void VGA_Init(int width, int height, bool maintainAspectRatio);
void VGA_Init(Uint32 flags, int width, int height, bool maintainAspectRatio);
bool VGA_LoadFont();
void VGA_close();
void VGA_Resize(int width, int height);
void VGA_Blit(uint16_t width, uint16_t height, Uint8* srcBuffer);
void SubBlit(uint16_t originalResWidth, uint16_t originalResHeight);
void VGA_Debug_Blit(int width, int height, Uint8* srcBuffer);
void VGA_Set_pallette(Uint8* pallettebuffer);
void VGA_Set_pallette2(Uint8* pallettebuffer);
void VGA_Write_basic_pallette(Uint8* pallettebuffer);
Uint8* VGA_Get_pallette();
void VGA_Draw_string(char* wrstring);
void VGA_Draw_stringXYtoBuffer(char* wrstring, int x, int y, uint8_t* buffer);
//void VGA_Set_Cursor(posistruct posstrcursor,int count);
void VGA_GotoXY(int x, int y);
int VGA_WhereX(); //returns current text cursor (x) coordinate
int VGA_WhereY(); //returns current text cursor (y) coordinate
POSITION VGA_WhereXY();
void Set_basic_pallette0();
void Set_basic_pallette1();
void VGA_Set_file_pallette(char* filename);
int16_t VGA_get_shift_status();
bool VGA_check_standart_input_status();
uint16_t VGA_read_char_from_buffer();
void VGA_Set_mouse(int16_t a1, int16_t a2);
void setPress(bool locpressed, uint16_t loclastchar);

void VGA_mouse_clear_keys();
void VGA_cleanKeyBuffer();
void Draw_debug_matrix1();

extern uint8_t LastPressedKey_1806E4; // weak//3516e4
extern int8_t pressedKeys_180664[128]; // idb

#endif //PORT_SDL_VGA_MOUSE
