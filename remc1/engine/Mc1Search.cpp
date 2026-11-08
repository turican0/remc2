#include "Mc1Search.h"

#include "Mc1Files.h"

Type_Mc1SearchRing searchRings_AD008_ACFF8[32];
Type_Mc1Unknown_AC5E0 unknownTable_AC5E0_AC5D0[101];

void InitSearchRings_11540_11540()
{
	// the 32x32 ring map is loaded into the work screen, the records go to the *SearchD buffer
	uint8_t* grid = *workScreen_12EFF4_12EFE4;
	ReadPackedFile_3EEA0_3F1E0("data/search.dat", grid);
	for (int32_t i = 2; i <= 100; i++)
		unknownTable_AC5E0_AC5D0[i].value_0 = -1;

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
