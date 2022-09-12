#include <cstdint>
#include <gtest/gtest.h>
#include "../utilities/Math.h"
#include "../comparison/engine/OriginalTerrain.h"
#include "../../remc2/engine/Terrain.h"

TEST(sub_B5C60_getTerrainAlt2, getTerrainAlt) 
{

	for (int i = 0; i < 256 * 256; i++)
		mapHeightmap_11B4E0[i] = pseudoRand() % 256;

	for (int y = 0; y < 256 * 256; y++)
	{
		if ((y > 260) && (y < 256 * 256 - 260))//skip steps for quick compare
			continue;
		for (int x = 0; x < 256 * 256; x++)
		{
			int a = sub_B5C60_getTerrainAlt2(x, y);
			int b = sub_B5C60_getTerrainAlt2_orig(x, y, mapHeightmap_11B4E0);
			EXPECT_EQ(a, b);
		}
	}
}

TEST(sub_44DB0_truncTerrainHeight, truncTerrainHeight)
{
	int16_t* mapEntityIndex1 = (int16_t*)malloc(65536 * sizeof(int16_t));
	int16_t* mapEntityIndex2 = (int16_t*)malloc(65536 * sizeof(int16_t));
	uint8_t* mapHeightmap1 = (uint8_t*)malloc(65536 * sizeof(uint8_t));
	uint8_t* mapHeightmap2 = (uint8_t*)malloc(65536 * sizeof(uint8_t));

	//Generate Data
	for (int i = 0; i < 256 * 256; i++)
	{
		mapEntityIndex1[i] = pseudoRand() % (256 * 256);
		mapEntityIndex2[i] = mapEntityIndex1[i];
	}

	//Run original code
	sub_44DB0_truncTerrainHeight_orig(mapEntityIndex1, mapHeightmap1);

	//Run current code
	sub_44DB0_truncTerrainHeight(mapEntityIndex2, mapHeightmap2);

	//Check results match
	for (int i = 0; i < 256 * 256; i++)
	{
		EXPECT_EQ(mapEntityIndex1[i], mapEntityIndex2[i]);
		EXPECT_EQ(mapHeightmap1[i], mapHeightmap2[i]);
	}
	free(mapEntityIndex1);
	free(mapEntityIndex2);
	free(mapHeightmap1);
	free(mapHeightmap2);
}