#ifndef PORT_SDL_VGA_MOUSE
#define PORT_SDL_VGA_MOUSE
#include <stdio.h>
#include <assert.h>
#include <sstream>
#include <stddef.h>
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */

#include "mctypes.h"

#ifdef _MSC_VER
	#include "SDL.h"
#else
    #include "SDL2/SDL.h"
#endif

#include "port_sdl_sound.h"
#include "port_filesystem.h"

typedef struct
{
	Bit32s x;
	Bit32s y;
} POSITION;

void VGA_Init();
void VGA_Init(Uint32 flags);
void VGA_Resize(int width, int height);
void VGA_Resize(int width, int height, int bpp, Uint32 flags);
void VGA_Blit(int width, int height, Uint8* buffer);
void VGA_Debug_Blit(int width, int height, Uint8* buffer);
void VGA_Set_pallette(Uint8* pallettebuffer);
void VGA_Set_pallette2(Uint8* pallettebuffer);
void VGA_Write_basic_pallette(Uint8* pallettebuffer);
Uint8* VGA_Get_pallette();
void VGA_Draw_string(char* wrstring);
void VGA_Draw_stringXYtoBuffer(char* wrstring, int x, int y,Bit8u* buffer);
//void VGA_Set_Cursor(posistruct posstrcursor,int count);
void VGA_GotoXY(int x, int y);
int VGA_WhereX(); //returns current text cursor (x) coordinate 
int VGA_WhereY(); //returns current text cursor (y) coordinate
POSITION VGA_WhereXY();
void Set_basic_pallette0();
void Set_basic_pallette1();
void VGA_Set_file_pallette(char* filename);
Bit16s VGA_get_shift_status();
bool VGA_check_standart_input_status();
Bit16u VGA_read_char_from_buffer();
void VGA_Set_mouse(Bit16s a1, Bit16s a2);
void setPress(bool locpressed, Bit16u loclastchar);

void VGA_mouse_clear_keys();
void VGA_cleanKeyBuffer();

extern Bit8u x_BYTE_1806E4; // weak//3516e4
extern Bit8s x_BYTE_180664[128]; // idb

#endif //PORT_SDL_VGA_MOUSE
