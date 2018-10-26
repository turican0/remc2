#ifndef PORT_SDL_VGA_MOUSE
#define PORT_SDL_VGA_MOUSE
#include <stdio.h>
#include <assert.h>
#include <sstream>
#include <stddef.h>
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */

#include "mctypes.h"

#include "SDL.h"

#include "port_sdl_sound.h"

typedef struct
{
	Bit32s x;
	Bit32s y;
} POSITION;

void VGA_Init();
void VGA_Init(int width, int height, int bpp, Uint32 flags);
void VGA_Resize(int width, int height);
void VGA_Resize(int width, int height, int bpp, Uint32 flags);
void VGA_Blit(int width, int height, Uint8* buffer);
void VGA_Set_pallette(Uint8* pallettebuffer);
void VGA_Write_basic_pallette(Uint8* pallettebuffer);
Uint8* VGA_Get_pallette();
void VGA_Draw_string(char* wrstring);
//void VGA_Set_Cursor(posistruct posstrcursor,int count);
void VGA_GotoXY(int x, int y);
int VGA_WhereX(); //returns current text cursor (x) coordinate 
int VGA_WhereY(); //returns current text cursor (y) coordinate
POSITION VGA_WhereXY();
void Set_basic_pallette0();
void Set_basic_pallette1();
Bit16s VGA_get_shift_status();
bool VGA_check_standart_input_status();
Bit16u VGA_read_char_from_buffer();

#endif //PORT_SDL_VGA_MOUSE
