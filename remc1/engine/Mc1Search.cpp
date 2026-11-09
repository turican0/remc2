#include "Mc1Search.h"

#include "Mc1Files.h"

Type_Mc1SearchRing searchRings_AD008_ACFF8[32];
Type_Mc1SearchState searchStates_AC5F0_AC5E0[101];

void InitSearchRings_11540_11540()
{
	// the 32x32 ring map is loaded into the work screen, the records go to the *SearchD buffer
	uint8_t* grid = *workScreen_12EFF4_12EFE4;
	ReadPackedFile_3EEA0_3F1E0("data/search.dat", grid);
	for (int32_t i = 1; i < 100; i++)
		searchStates_AC5F0_AC5E0[i].index_8 = -1;

	uint8_t* record = searchData_AE3F4_AE3E4;
	int16_t originX = 0; // set by the first cell of ring 0 (the centre), uninitialized before in the original
	int16_t originY = 0;
	for (uint8_t ring = 0; ring < 32; ring++)
	{
		searchRings_AD008_ACFF8[ring].start_0 = record;
		searchRings_AD008_ACFF8[ring].count_4 = 0;
		for (int16_t y = 0; y < 32; y++)
		{
			for (int16_t x = 0; x < 32; x++)
			{
				uint8_t value = grid[y * 32 + x];
				if (value != ring)
					continue;
				if (value == 0 && record == searchData_AE3F4_AE3E4)
				{
					originX = x;
					originY = y;
				}
				record[0] = (uint8_t)(x - originX);
				record[1] = (uint8_t)(y - originY);
				record[2] = ring;
				searchRings_AD008_ACFF8[ring].count_4++;
				record += 4;
			}
		}
	}
}

int32_t StartSearch_11410_11410(int32_t first, int32_t last)
{
	// the first free state, 100 when none is free
	int32_t search = 1;
	while (search < 100 && searchStates_AC5F0_AC5E0[search].index_8 != -1)
		search++;
	Type_Mc1SearchState& state = searchStates_AC5F0_AC5E0[search];
	state.first_0 = first;
	state.index_8 = first;
	state.counter_0xC = 0;
	state.last_4 = last;
	state.ring_0x10 = &searchRings_AD008_ACFF8[state.index_8];
	state.data_0x14 = state.ring_0x10->start_0;
	return search;
}

int32_t EndSearch_11490_11490(int32_t search)
{
	searchStates_AC5F0_AC5E0[search].index_8 = -1;
	return 1;
}

int32_t GetNextSearchOffset_114B0_114B0(int32_t search, int32_t* dx, int32_t* dy)
{
	// 1 an offset, 2 after the last ring
	Type_Mc1SearchState& state = searchStates_AC5F0_AC5E0[search];
	*dx = (int8_t)state.data_0x14[0];
	*dy = (int8_t)state.data_0x14[1];
	state.data_0x14 += 4;
	state.counter_0xC++;
	if ((uint16_t)state.ring_0x10->count_4 > state.counter_0xC)
		return 1;
	state.counter_0xC = 0;
	state.ring_0x10++;
	state.index_8++;
	if (state.index_8 > state.last_4)
		return 2;
	return 1;
}
