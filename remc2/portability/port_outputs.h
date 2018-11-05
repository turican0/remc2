#ifndef PORT_OUTPUTS
#define PORT_OUTPUTS

#include <stdio.h>
//#include <assert.h>
//#include <sstream>
//#include <stddef.h>
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */

//#include"../portability/port_time.h"

//#include "../portability/port_sdl_vga_mouse.h"
//#include "../portability/mctypes.h"

void myWriteOut(const char * format, ...);
void myprintf(const char * format, ...);

#endif //PORT_OUTPUTS
