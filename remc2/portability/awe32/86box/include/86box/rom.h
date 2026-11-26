#ifndef EMU_ROM_H
#define EMU_ROM_H
#include <stdio.h>
/* Harness stub: signature identical to upstream src/include/86box/rom.h. */
extern FILE *rom_fopen(const char *fn, char *mode);
/* remc2: copies the wave ROM image (unpacked) to buf, returns the bytes copied;
 * emu8k_init uses it instead of rom_fopen (EMU8K_ROM_READ). */
#define EMU8K_ROM_READ
extern size_t rom_read(void *buf, size_t size);
#endif
