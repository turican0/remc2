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

TEST(sub_44EE0_smooth_tiles, smooth_tiles) {
	uint8_t* tempTerr1 = (uint8_t*)malloc(65536);
	uint8_t* tempAng1 = (uint8_t*)malloc(65536);
	uint8_t* tempHeight1 = (uint8_t*)malloc(65536);

	uint8_t* tempTerr2 = (uint8_t*)malloc(65536);
	uint8_t* tempAng2 = (uint8_t*)malloc(65536);
	uint8_t* tempHeight2 = (uint8_t*)malloc(65536);

	for (int j = 0; j < 256 * 256; j++)
	{
		if ((j > 260) && (j < 256 * 256 - 260))//skip steps for quick compare
			continue;

		//Create Test Data
		for (int i = 0; i < 256 * 256; i++)
		{
			mapTerrainType_10B4E0[i] = pseudoRand() % 256;
			mapAngle_13B4E0[i] = pseudoRand() % 256;
			mapHeightmap_11B4E0[i] = pseudoRand() % 256;
			tempTerr1[i] = mapTerrainType_10B4E0[i];
			tempAng1[i] = mapAngle_13B4E0[i];
			tempHeight1[i] = mapHeightmap_11B4E0[i];
		}

		uaxis_2d axis;
		axis.word = j;
		sub_44EE0_smooth_tiles_orig(axis, tempTerr1, tempHeight1, tempAng1);

		sub_44EE0_smooth_tiles(axis);

		for (int i = 0; i < 256 * 256; i++)
		{
			EXPECT_EQ(tempTerr1[i], mapTerrainType_10B4E0[i]);
			EXPECT_EQ(tempAng1[i], mapAngle_13B4E0[i]);
			EXPECT_EQ(tempHeight1[i], mapHeightmap_11B4E0[i]);
		}
	}

	free(tempTerr1);
	free(tempAng1);
	free(tempHeight1);
}

TEST(sub_44E40_orig, sub_44E40) {
	uint8_t* tempTerrainType1 = (uint8_t*)malloc(65536);
	uint8_t* tempAngle1 = (uint8_t*)malloc(65536);
	uint8_t* tempHeightmap1 = (uint8_t*)malloc(65536);

	for (int j = 0; j < 400; j++)
	{
		x_WORD_17B4E0 = pseudoRand() % (256 * 256);
		int a1 = pseudoRand() % 256;
		uint8_t a2 = pseudoRand() % 256;

		//Create Test Data
		for (int i = 0; i < 256 * 256; i++)
		{
			mapTerrainType_10B4E0[i] = pseudoRand() % (256 * 256);
			tempTerrainType1[i] = mapTerrainType_10B4E0[i];
			mapAngle_13B4E0[i] = pseudoRand() % (256 * 256);
			tempAngle1[i] = mapAngle_13B4E0[i];
			mapHeightmap_11B4E0[i] = pseudoRand() % (256 * 256);
			tempHeightmap1[i] = mapHeightmap_11B4E0[i];
		}
		sub_44E40_orig(a1, a2, x_WORD_17B4E0, tempTerrainType1, tempHeightmap1, tempAngle1);

		sub_44E40(a1, a2);

		for (int i = 0; i < 256 * 256; i++)
		{
			EXPECT_EQ(tempTerrainType1[i], mapTerrainType_10B4E0[i]);
			EXPECT_EQ(tempAngle1[i], mapAngle_13B4E0[i]);
			EXPECT_EQ(tempHeightmap1[i], mapHeightmap_11B4E0[i]);
		}
	}
	free(tempAngle1);
	free(tempTerrainType1);
	free(tempHeightmap1);
}