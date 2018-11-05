#include "port_outputs.h"
#include "port_sdl_vga_mouse.h"

void myWriteOut(const char * format, ...) {
#ifdef USE_DOSBOX
	DEBUG_ShowMsg(format);
#else
	printf(format);
	VGA_Draw_string((char*)format);
#endif //USE_DOSBOX

	/*Bit16u sz = 1;
	unsigned char b = 'a';
	DOS_CON->Write(&b, &sz);*/
}

void myprintf(const char * format, ...) {
	char prbuffer[1024];
	va_list arg;
	int done;
	va_start(arg, format);
	done = vsprintf_s(prbuffer, format, arg);
	va_end(arg);

#ifdef USE_DOSBOX
	DEBUG_ShowMsg(prbuffer);
#else
	printf(prbuffer);
	VGA_Draw_string(prbuffer);
#endif //USE_DOSBOX

	//return strlen(prbuffer);
	//return done;
	/*Bit16u sz = 1;
	unsigned char b = 'a';
	DOS_CON->Write(&b, &sz);*/
}