using namespace std;

#include <stdio.h>
#include <cstdint>
#include <assert.h>
#include <sstream>
#include <stddef.h>
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */

#include "png.h"
#pragma comment(lib, "zlib.lib") // must be before libpng!
#ifndef _WIN64
#pragma comment(lib, "libpng15.lib") // must be after zlib!
#else
#endif

//#include "../portability/port_time.h"
//#include "../portability/port_sdl_vga_mouse.h"

#include "SDL.h"
#include "editor.h"

int main(int argc, char* argv[])
//int SDLmain(int argc, char** argv)
{	
    editor_run();
    return 0;
}

