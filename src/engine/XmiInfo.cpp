#include "XmiInfo.h"

// ============================================================================
// Internal helpers — all static, no external dependencies
// ============================================================================

static uint32_t _be32(const uint8_t* p) {
	return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) | (uint32_t)p[3];
}

// Standard MIDI VLQ (used for meta event lengths and Note On duration)
static uint32_t _vlq(const uint8_t*& p, const uint8_t* end) {
	uint32_t v = 0;
	while (p < end) { uint8_t b = *p++; v = (v << 7) | (b & 0x7F); if (!(b & 0x80)) break; }
	return v;
}

// XMI delta-time: sum of bytes with high-bit=0 (NOT concatenated like MIDI VLQ)
static uint32_t _xmi_delta(const uint8_t*& p, const uint8_t* end) {
	uint32_t d = 0;
	while (p < end && !(*p & 0x80)) d += *p++;
	return d;
}

// Locate the first EVNT chunk in the XMI data.
// Size is read directly from the IFF headers — no external size parameter needed.
// Handles FORM XMID, FORM XDIR (with trailing CAT XMID), and direct CAT XMID.
// Returns pointer to EVNT data (past tag+size header), sets *out_evnt_size.
// Returns nullptr if not found or data is malformed.
static const uint8_t* _find_evnt(const uint8_t* data, uint32_t* out_evnt_size)
{
	// Need at least tag(4) + size(4) + type(4)
	const uint8_t* p = data;

	// Determine total file boundary from the outermost IFF chunk size
	uint32_t outer_size = _be32(p + 4);        // size field of FORM/CAT
	const uint8_t* file_end = p + 8 + outer_size;

	const uint8_t* xmid_interior = nullptr;
	uint32_t       xmid_interior_size = 0;

	// Scan a byte range for the first FORM XMID sub-chunk
	auto try_find_xmid = [&](const uint8_t* scan, const uint8_t* scan_end) -> bool {
		while (scan + 8 <= scan_end) {
			uint32_t csz = _be32(scan + 4);
			uint32_t step = csz + (csz & 1);
			if (memcmp(scan, "FORM", 4) == 0 && memcmp(scan + 8, "XMID", 4) == 0) {
				xmid_interior = scan + 12;
				xmid_interior_size = (csz >= 4) ? csz - 4 : 0;
				return true;
			}
			if (scan + 8 + step > scan_end) break;
			scan += 8 + step;
		}
		return false;
		};

	if (memcmp(p, "FORM", 4) == 0 && memcmp(p + 8, "XMID", 4) == 0) {
		// Direct single-track FORM XMID
		xmid_interior = p + 12;
		xmid_interior_size = (outer_size >= 4) ? outer_size - 4 : 0;
	}
	else if (memcmp(p, "FORM", 4) == 0 && memcmp(p + 8, "XDIR", 4) == 0) {
		// FORM XDIR directory wrapper: CAT XMID immediately follows in the file
		const uint8_t* cat = p + 8 + outer_size + (outer_size & 1);
		if (memcmp(cat, "CAT ", 4) == 0) {
			uint32_t csz = _be32(cat + 4);
			try_find_xmid(cat + 12, cat + 8 + csz);
		}
	}
	else if (memcmp(p, "CAT ", 4) == 0 && memcmp(p + 8, "XMID", 4) == 0) {
		// Direct multi-track CAT XMID
		try_find_xmid(p + 12, file_end);
	}

	if (!xmid_interior) return nullptr;

	// Walk chunks inside FORM XMID and find EVNT
	const uint8_t* ip = xmid_interior;
	const uint8_t* iend = xmid_interior + xmid_interior_size;
	while (ip + 8 <= iend) {
		uint32_t csz = _be32(ip + 4);
		if (memcmp(ip, "EVNT", 4) == 0) {
			if (out_evnt_size) *out_evnt_size = csz;
			return ip + 8;
		}
		ip += 8 + csz + (csz & 1);
	}
	return nullptr;
}

// ============================================================================
// Main function
// ============================================================================

// Scans an XMI buffer and writes the first max_out loop events (CC#116/117)
// into out[].  Returns the number of entries written (0 = no loops or parse error).
//
// Parameters:
//   xmi_data  - loaded contents of a .xmi file (size derived from IFF headers)
//   out       - output array of XmiLoopEvent structs
//   max_out   - capacity of the output array (typically 10)
//
int XMI_FindLoopEvents(const char* xmi_data, XmiLoopEvent out[], int max_out)
{
	if (!xmi_data || !out || max_out <= 0) return 0;

	// Locate the EVNT chunk — size comes from the IFF headers inside the data
	uint32_t evnt_size = 0;
	const uint8_t* evnt = _find_evnt((const uint8_t*)xmi_data, &evnt_size);
	if (!evnt) return 0;

	const uint8_t* p = evnt;
	const uint8_t* end = evnt + evnt_size;

	uint32_t ticks = 0;
	uint32_t ev_index = 0;
	int      loop_depth = 0;
	int      found = 0;

	while (p < end && found < max_out)
	{
		// Accumulate XMI delta time
		ticks += _xmi_delta(p, end);
		if (p >= end) break;

		uint32_t ev_offset = (uint32_t)(p - evnt);
		uint8_t  status = *p++;
		uint8_t  type = status & 0xF0;
		uint8_t  ch = status & 0x0F;

		// ---- Meta event ----
		if (status == 0xFF) {
			if (p >= end) break;
			p++;                        // meta type byte
			uint32_t mlen = _vlq(p, end);
			p += mlen;
			ev_index++;
			continue;
		}
		// ---- SysEx ----
		if (status == 0xF0 || status == 0xF7) {
			uint32_t slen = _vlq(p, end);
			p += slen;
			ev_index++;
			continue;
		}
		// ---- Note Off (0x80) ----
		if (type == 0x80) { p += 2; ev_index++; continue; }
		// ---- Note On (0x90) — XMI stores duration VLQ after velocity ----
		if (type == 0x90) {
			if (p + 1 > end) break;
			p += 2;             // note + velocity
			_vlq(p, end);       // duration (XMI-specific)
			ev_index++;
			continue;
		}
		// ---- Poly Pressure (0xA0) ----
		if (type == 0xA0) { p += 2; ev_index++; continue; }
		// ---- Control Change (0xB0) ----
		if (type == 0xB0) {
			if (p + 1 > end) break;
			uint8_t cc = *p++;
			uint8_t val = *p++;

			// Only interested in XMI loop controllers
			if (cc == 116 || cc == 117) {
				XmiLoopEvent& e = out[found++];
				e.evt_index = ev_index;
				e.ticks = ticks;
				e.evnt_offset = ev_offset;
				e.channel = ch;
				e.cc = cc;
				e.val = val;

				if (cc == 116) {    // FOR Loop Start — saves EVNT_ptr + repeat count
					loop_depth++;
					e.loop_depth = loop_depth;
					if (val == 0)
						snprintf(e.detail, sizeof(e.detail),
							"XMI FOR Loop Start  -> infinite loop (depth %d)", loop_depth);
					else
						snprintf(e.detail, sizeof(e.detail),
							"XMI FOR Loop Start  -> repeat %d times (depth %d)", val, loop_depth);
				}
				else {              // FOR Loop Next (cc == 117) — jumps back or exits
					e.loop_depth = loop_depth;
					if (val >= 64)
						snprintf(e.detail, sizeof(e.detail),
							"XMI FOR Loop Next  -> conditional jump back (depth %d)", loop_depth);
					else
						snprintf(e.detail, sizeof(e.detail),
							"XMI FOR Loop Next  -> unconditional, ignored (depth %d)", loop_depth);
					if (loop_depth > 0) loop_depth--;
				}
			}
			ev_index++;
			continue;
		}
		// ---- Program Change (0xC0) ----
		if (type == 0xC0) { p++; ev_index++; continue; }
		// ---- Channel Pressure (0xD0) ----
		if (type == 0xD0) { p++; ev_index++; continue; }
		// ---- Pitch Bend (0xE0) ----
		if (type == 0xE0) { p += 2; ev_index++; continue; }

		// Unknown status byte — stop to avoid corrupt output
		break;
	}

	return found;
}