#ifndef PORT_SDL_VGA_MOUSE
#define PORT_SDL_VGA_MOUSE
#include <stdio.h>
#include <assert.h>
#include <sstream>
#include <stddef.h>
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */

#include "SDL.h"

#include "mctypes.h"

void VGA_Init();
void VGA_Init(int width, int height, int bpp, Uint32 flags);
void VGA_Resize(int width, int height);
void VGA_Resize(int width, int height, int bpp, Uint32 flags);
void VGA_Blit(int width, int height, Uint8* buffer);
void VGA_Set_pallette(Uint8* pallettebuffer);
void VGA_Write_basic_pallette(Uint8* pallettebuffer);
Uint8* VGA_Get_pallette();
void VGA_Draw_string(char* wrstring);
void VGA_Set_Cursor(posistruct posstrcursor,int count);
void VGA_GotoXY(int x, int y);
int VGA_WhereX(); //returns current text cursor (x) coordinate 
int VGA_WhereY(); //returns current text cursor (y) coordinate 
void Set_basic_pallette();

#endif //PORT_SDL_VGA_MOUSE
