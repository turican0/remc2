#include "port_outputs.h"
#include "port_sdl_vga_mouse.h"

void myWriteOut(const char * format, ...) {
#ifdef USE_DOSBOX
	DEBUG_ShowMsg(format);
#else
	printf("%s", format);
	VGA_Draw_string((char*)format);
#endif //USE_DOSBOX

	/*uint16_t sz = 1;
	unsigned char b = 'a';
	DOS_CON->Write(&b, &sz);*/
}

void myprintf(const char * format, ...) {
	char prbuffer[1024];
	va_list arg;
	int done;
	va_start(arg, format);
	done = vsprintf(prbuffer, format, arg);
	va_end(arg);

#ifdef USE_DOSBOX
	DEBUG_ShowMsg(prbuffer);
#else
	printf("%s", prbuffer);
	VGA_Draw_string(prbuffer);
#endif //USE_DOSBOX

	//return strlen(prbuffer);
	//return done;
	/*uint16_t sz = 1;
	unsigned char b = 'a';
	DOS_CON->Write(&b, &sz);*/
}


