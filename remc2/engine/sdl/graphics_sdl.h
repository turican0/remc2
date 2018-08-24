#include <stdio.h>
#include <assert.h>
#include <sstream>
#include <stddef.h>
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */

#include "SDL.h"

void VGA_Init();
void VGA_Init(int width, int height, int bpp, Uint32 flags);
void VGA_Resize(int width, int height);
void VGA_Resize(int width, int height, int bpp, Uint32 flags);
void VGA_Blit(int width, int height, Uint8* buffer);
