/* Empty variant of the 86Box instruction trace - see snd_emu8k_trace.h.
 *
 * In the 86Box tree, `awe32_trace.c` is a probe that annotates register
 * writes with CPU context (where the driver writes from). Without a running
 * VM it has nothing to report, so in our render it does nothing.
 */
#ifndef AWE32_TRACE_H
#define AWE32_TRACE_H

#include <stdint.h>

static inline void
awe32_trace_ioctx(const char *dir, uint16_t addr, uint16_t val)
{
    (void) dir;
    (void) addr;
    (void) val;
}

#endif /* AWE32_TRACE_H */
