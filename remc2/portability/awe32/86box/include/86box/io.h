#ifndef EMU_IO_H
#define EMU_IO_H
#include <stdint.h>
/* Harness stubs: signatures identical to upstream src/include/86box/io.h.
   The harness calls emu8k_outw/emu8k_inw directly, so these do nothing. */
extern void io_sethandler(uint16_t base, uint16_t size,
                          uint8_t (*inb)(uint16_t port, void *priv),
                          uint16_t (*inw)(uint16_t port, void *priv),
                          uint32_t (*inl)(uint16_t port, void *priv),
                          void (*outb)(uint16_t port, uint8_t val, void *priv),
                          void (*outw)(uint16_t port, uint16_t val, void *priv),
                          void (*outl)(uint16_t port, uint32_t val, void *priv),
                          void *priv);
extern void io_removehandler(uint16_t base, uint16_t size,
                             uint8_t (*inb)(uint16_t port, void *priv),
                             uint16_t (*inw)(uint16_t port, void *priv),
                             uint32_t (*inl)(uint16_t port, void *priv),
                             void (*outb)(uint16_t port, uint8_t val, void *priv),
                             void (*outw)(uint16_t port, uint16_t val, void *priv),
                             void (*outl)(uint16_t port, uint32_t val, void *priv),
                             void *priv);
#endif
