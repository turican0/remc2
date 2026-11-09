#pragma once

#include <cstdint>

// MC1 search pattern: map cells ordered by distance (rings) from data/search.dat

typedef struct { // DOS 6 bytes
	uint8_t* start_0; // 4 byte records: dx, dy, ring
	int16_t count_4;
} Type_Mc1SearchRing;

typedef struct { // DOS 0x18 bytes
	int32_t first_0; // the first ring
	int32_t last_4; // the last ring
	int32_t index_8; // the current ring, -1 free
	int32_t counter_0xC; // the record in the ring
	Type_Mc1SearchRing* ring_0x10;
	uint8_t* data_0x14; // the next record
} Type_Mc1SearchState;

extern Type_Mc1SearchRing searchRings_AD008_ACFF8[32];
extern Type_Mc1SearchState searchStates_AC5F0_AC5E0[101]; // 0 and 100 are never freed

void InitSearchRings_11540_11540();
int32_t StartSearch_11410_11410(int32_t first, int32_t last);
int32_t EndSearch_11490_11490(int32_t search);
int32_t GetNextSearchOffset_114B0_114B0(int32_t search, int32_t* dx, int32_t* dy);
