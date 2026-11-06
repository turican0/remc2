#pragma once

#include <cstdint>

// MC1 search pattern: map cells ordered by distance (rings) from data/search.dat

typedef struct { // DOS 6 bytes
	uint8_t* start_0; // 4 byte records: dx, dy, ring
	int16_t count_4;
} Type_Mc1SearchRing;

typedef struct { // size 0x18
	int32_t value_0;
	uint8_t unknown_0x04_4[0x14];
} Type_Mc1Unknown_AC5E0;

extern Type_Mc1SearchRing searchRings_AD008_ACFF8[32];
extern Type_Mc1Unknown_AC5E0 unknownTable_AC5E0_AC5D0[101];

void InitSearchRings_11540_11540();
