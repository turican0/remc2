#pragma once

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

// ============================================================================
// Result structure — one loop event
// ============================================================================

struct XmiLoopEvent
{
	uint32_t    evt_index;      // sequential event number in the EVNT stream
	uint32_t    ticks;          // absolute tick time
	uint32_t    evnt_offset;    // byte offset inside the EVNT chunk
	uint8_t     channel;        // MIDI channel (0-15)
	uint8_t     cc;             // controller: 116 = FOR Start, 117 = FOR Next
	uint8_t     val;            // controller value
	int         loop_depth;     // nesting depth at the time of this event
	char        detail[128];    // human-readable description, e.g.
	// "XMI FOR Loop Start  -> infinite loop (depth 1)"
};

int XMI_FindLoopEvents(const char* xmi_data, XmiLoopEvent out[], int max_out);