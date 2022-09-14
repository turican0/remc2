#include <cstdint>
#include <gtest/gtest.h>
#include "../utilities/Math.h"
#include "../comparison/engine/OriginalTerrain.h"
#include "../../remc2/engine/Terrain.h"

TEST(Terrain, sub_B5C60_getTerrainAlt2)
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

TEST(Terrain, sub_44DB0_truncTerrainHeight)
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

TEST(Terrain, sub_44EE0_smooth_tiles) {
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

TEST(Terrain, sub_44E40) {
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

TEST(Terrain, sub_B5D68) {

	x_BYTE_14B4E0_second_heightmap = (uint8_t*)malloc(65536);
	uint8_t* x_BYTE_14B4E0_second_heightmap2 = (uint8_t*)malloc(65536);

	//Create Test Data
	for (int i = 0; i < 256 * 256; i++)
	{
		x_BYTE_14B4E0_second_heightmap[i] = pseudoRand() % 256;
		x_BYTE_14B4E0_second_heightmap2[i] = x_BYTE_14B4E0_second_heightmap[i];
	}

	for (int j = 0; j < 400; j++)
	{
		__int16 tempa1 = pseudoRand() % (256 * 256) - 127 * 256;
		__int16 tempa2 = pseudoRand() % (256 * 256) - 127 * 256;
		EXPECT_EQ(sub_B5D68_orig(tempa1, tempa2, x_BYTE_14B4E0_second_heightmap2), sub_B5D68(tempa1, tempa2));
	}

	free(x_BYTE_14B4E0_second_heightmap);
	free(x_BYTE_14B4E0_second_heightmap2);
}

TEST(Terrain, sub_45DC0) {
	uint8_t* tempTerrType1 = (uint8_t*)malloc(65536);
	uint8_t* tempHeight1 = (uint8_t*)malloc(65536);
	uint8_t* tempSecHeight1 = (uint8_t*)malloc(65536);
	uint8_t* tempAng1 = (uint8_t*)malloc(65536);
	uint8_t* tempShad1 = (uint8_t*)malloc(65536);
	x_BYTE_14B4E0_second_heightmap = (uint8_t*)malloc(65536);

	for (int j = 0; j < 400; j++)
	{
		uint8_t tempa2 = pseudoRand() % 256;
		uaxis_2d tempa3x;
		tempa3x.word = pseudoRand() % (256 * 256);
		uint8_t tempa4 = pseudoRand() % 256;
		lowDiffHeightmap_D47DC = true;
		if (pseudoRand() % 2)
			lowDiffHeightmap_D47DC = false;
		switch (pseudoRand() % 3)
		{
		case 0:
			D41A0_0.terrain_2FECE.MapType = MapType_t::Day;
			isCaveLevel_D41B6 = false;
			break;
		case 1:
			D41A0_0.terrain_2FECE.MapType = MapType_t::Night;
			isCaveLevel_D41B6 = false;
			break;
		case 2:
			D41A0_0.terrain_2FECE.MapType = MapType_t::Cave;
			isCaveLevel_D41B6 = true;
			break;
		}
		for (int i = 0; i < 7 * 7 * 7 * 7; i++)
			for (int m = 0; m < 2; m++)
			{
				x_BYTE_F2CD0x[i][m] = pseudoRand() % 256;
			}
		x_WORD_17B4E0 = pseudoRand() % (256 * 256);
		uint16_t tempx_WORD_17B4E01 = x_WORD_17B4E0;

		//Create Test Data
		for (int i = 0; i < 256 * 256; i++)
		{
			mapTerrainType_10B4E0[i] = pseudoRand() % 256;
			mapHeightmap_11B4E0[i] = pseudoRand() % 256;
			mapAngle_13B4E0[i] = pseudoRand() % 256;
			x_BYTE_14B4E0_second_heightmap[i] = pseudoRand() % 256;
			mapShading_12B4E0[i] = pseudoRand() % 256;

			tempTerrType1[i] = mapTerrainType_10B4E0[i];
			tempShad1[i] = mapShading_12B4E0[i];
			tempSecHeight1[i] = x_BYTE_14B4E0_second_heightmap[i];
			tempAng1[i] = mapAngle_13B4E0[i];
			tempHeight1[i] = mapHeightmap_11B4E0[i];
		}

		sub_45DC0_orig(tempa2, tempa3x, tempa4, lowDiffHeightmap_D47DC, isCaveLevel_D41B6, x_WORD_17B4E0, x_BYTE_F2CD0x, tempTerrType1, tempHeight1, tempAng1, tempShad1, tempSecHeight1);

		uint16_t tempx_WORD_17B4E02 = x_WORD_17B4E0;
		x_WORD_17B4E0 = tempx_WORD_17B4E01;

		sub_45DC0(tempa2, tempa3x, tempa4);

		for (int i = 0; i < 256 * 256; i++)
		{
			EXPECT_EQ(tempHeight1[i], mapHeightmap_11B4E0[i]);
			EXPECT_EQ(tempTerrType1[i], mapTerrainType_10B4E0[i]);
			EXPECT_EQ(tempShad1[i], mapShading_12B4E0[i]);
			EXPECT_EQ(tempSecHeight1[i], x_BYTE_14B4E0_second_heightmap[i]);
			EXPECT_EQ(tempAng1[i], mapAngle_13B4E0[i]);
		}
		EXPECT_EQ(x_WORD_17B4E0, tempx_WORD_17B4E02);
	}
	free(x_BYTE_14B4E0_second_heightmap);
	free(tempTerrType1);
	free(tempHeight1);
	free(tempSecHeight1);
	free(tempAng1);
	free(tempShad1);
}

TEST(Terrain, sub_45BE0) {

	uint8_t* tempHeight1 = (uint8_t*)malloc(65536);

	for (int j = 0; j < 400; j++)
	{
		uaxis_2d testAxis2D;
		testAxis2D.word = pseudoRand() % (256 * 256);
		uint8_t testA2 = pseudoRand() % 256;

		//Create Test Data
		for (int i = 0; i < 256 * 256; i++)
		{
			mapHeightmap_11B4E0[i] = pseudoRand() % 256;
			tempHeight1[i] = mapHeightmap_11B4E0[i];
		}
		sub_45BE0_orig(testA2, testAxis2D, lowDiffHeightmap_D47DC, tempHeight1);
		int tempx_DWORD_D47DC = lowDiffHeightmap_D47DC;

		sub_45BE0(testA2, testAxis2D);

		for (int i = 0; i < 256 * 256; i++)
		{
			EXPECT_EQ(tempHeight1[i], mapHeightmap_11B4E0[i]);
		}
		EXPECT_EQ(tempx_DWORD_D47DC, lowDiffHeightmap_D47DC);
	}
	free(tempHeight1);
}