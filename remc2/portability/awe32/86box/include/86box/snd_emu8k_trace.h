/* Empty variant of the 86Box tracing hooks.
 *
 * `snd_emu8k.c` in our 86Box tree has added `emu8k_trace_*` calls through
 * which traces get out of the running VM. Our render does not need them -
 * it traces one level up, in `Synth` - but we compile **the same file**, so
 * that the chip is literally the same code in both projects. That is why
 * the calls are empty functions here, which the compiler drops.
 *
 * Should a real body ever be written here, one rule holds: it must not
 * touch the chip state. Once tracing affected the sound, it would no longer
 * hold that our render and 86Box compute the same thing.
 */
#ifndef SND_EMU8K_TRACE_H
#define SND_EMU8K_TRACE_H

#include <stdint.h>

static inline void
emu8k_trace_read(uint16_t addr, uint16_t val)
{
    (void) addr;
    (void) val;
}

static inline void
emu8k_trace_write(uint16_t addr, uint16_t val)
{
    (void) addr;
    (void) val;
}

static inline void
emu8k_trace_advance(int num_samples)
{
    (void) num_samples;
}

static inline void
emu8k_trace_wav(const void *buf, int num_samples)
{
    (void) buf;
    (void) num_samples;
}

#endif /* SND_EMU8K_TRACE_H */
