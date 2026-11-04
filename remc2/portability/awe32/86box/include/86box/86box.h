#ifndef EMU_86BOX_H
#define EMU_86BOX_H
#include <stdarg.h>
#include <stdio.h>
/* Harness stub for 86box/86box.h -- only the logging entry points that
   snd_emu8k.c actually references. */
extern void pclog_ex(const char *fmt, va_list ap);
extern void pclog(const char *fmt, ...);
extern void fatal(const char *fmt, ...);
#endif
