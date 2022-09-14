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
		uint16_t tempx_WORD_17B4E0 = x_WORD_17B4E0;

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

		sub_45DC0_orig(tempa2, tempa3x, tempa4, lowDiffHeightmap_D47DC, isCaveLevel_D41B6, tempx_WORD_17B4E0, x_BYTE_F2CD0x, tempTerrType1, tempHeight1, tempAng1, tempShad1, tempSecHeight1);

		sub_45DC0(tempa2, tempa3x, tempa4);

		for (int i = 0; i < 256 * 256; i++)
		{
			EXPECT_EQ(tempHeight1[i], mapHeightmap_11B4E0[i]);
			EXPECT_EQ(tempTerrType1[i], mapTerrainType_10B4E0[i]);
			EXPECT_EQ(tempShad1[i], mapShading_12B4E0[i]);
			EXPECT_EQ(tempSecHeight1[i], x_BYTE_14B4E0_second_heightmap[i]);
			EXPECT_EQ(tempAng1[i], mapAngle_13B4E0[i]);
		}
		EXPECT_EQ(x_WORD_17B4E0, tempx_WORD_17B4E0);
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
		int tempx_DWORD_D47DC = lowDiffHeightmap_D47DC;

		//Create Test Data
		for (int i = 0; i < 256 * 256; i++)
		{
			mapHeightmap_11B4E0[i] = pseudoRand() % 256;
			tempHeight1[i] = mapHeightmap_11B4E0[i];
		}
		sub_45BE0_orig(testA2, testAxis2D, tempx_DWORD_D47DC, tempHeight1);

		sub_45BE0(testA2, testAxis2D);

		for (int i = 0; i < 256 * 256; i++)
		{
			EXPECT_EQ(tempHeight1[i], mapHeightmap_11B4E0[i]);
		}
		EXPECT_EQ(tempx_DWORD_D47DC, lowDiffHeightmap_D47DC);
	}
	free(tempHeight1);
}

TEST(Terrain, sub_33F70) {
	uint8_t* tempTerrainType1 = (uint8_t*)malloc(65536);
	uint8_t* tempHeightMap1 = (uint8_t*)malloc(65536);

	for (int j = 0; j < 400; j++)
	{
		//Create Test Data
		uaxis_2d testAxis2D;
		testAxis2D.word = pseudoRand() % (256 * 256);
		for (int i = 0; i < 256 * 256; i++)
		{
			mapTerrainType_10B4E0[i] = pseudoRand() % 256;
			mapHeightmap_11B4E0[i] = pseudoRand() % 256;
			tempTerrainType1[i] = mapTerrainType_10B4E0[i];
			tempHeightMap1[i] = mapHeightmap_11B4E0[i];
		}

		sub_33F70_orig(testAxis2D.word, tempTerrainType1, tempHeightMap1);

		sub_33F70(testAxis2D.word);

		for (int i = 0; i < 256 * 256; i++)
		{
			EXPECT_EQ(tempTerrainType1[i], mapTerrainType_10B4E0[i]);
			EXPECT_EQ(tempHeightMap1[i], mapHeightmap_11B4E0[i]);
		}
	}
	free(tempTerrainType1);
	free(tempHeightMap1);
}

TEST(Terrain, sub_1B830) {
	uint8_t* tempHeight1 = (uint8_t*)malloc(65536);

	for (int j = 0; j < 400; j++)
	{
		//Create Test Data
		axis_3d testAxis3D;
		testAxis3D.x = pseudoRand() % (256 * 256);
		testAxis3D.y = pseudoRand() % (256 * 256);
		testAxis3D.z = pseudoRand() % (256 * 256) - (127 * 256);
		for (int i = 0; i < 256 * 256; i++)
		{
			mapHeightmap_11B4E0[i] = pseudoRand() % 256;
			tempHeight1[i] = mapHeightmap_11B4E0[i];
		}

		sub_1B830_orig(&testAxis3D, tempHeight1);

		sub_1B830(&testAxis3D);

		for (int i = 0; i < 256 * 256; i++)
		{
			EXPECT_EQ(tempHeight1[i], mapHeightmap_11B4E0[i]);
		}
	}
	free(tempHeight1);
}

TEST(Terrain, sub_1B7A0) {
	uint8_t* tempHeight1 = (uint8_t*)malloc(65536);

	for (int j = 0; j < 400; j++)
	{
		//Create Test Data
		axis_3d testAxis3D;
		testAxis3D.x = pseudoRand() % (256 * 256);
		testAxis3D.y = pseudoRand() % (256 * 256);
		testAxis3D.z = pseudoRand() % (256 * 256) - (127 * 256);
		for (int i = 0; i < 256 * 256; i++)
		{
			mapHeightmap_11B4E0[i] = pseudoRand() % 256;
			tempHeight1[i] = mapHeightmap_11B4E0[i];
		}
		sub_1B7A0_tile_compare_orig(&testAxis3D, tempHeight1);

		sub_1B7A0_tile_compare(&testAxis3D);
		for (int i = 0; i < 256 * 256; i++)
		{
			EXPECT_EQ(tempHeight1[i], mapHeightmap_11B4E0[i]);
		}
	}
	free(tempHeight1);
}

TEST(Terrain, sub_43D50) {

	uint8_t* tempTerrainType1 = (uint8_t*)malloc(65536);
	uint8_t* tempAngle1 = (uint8_t*)malloc(65536);
	uint8_t* tempHeightmap1 = (uint8_t*)malloc(65536);

	//Create Test Data
	for (int i = 0; i < 256 * 256; i++)
	{
		mapTerrainType_10B4E0[i] = pseudoRand() % (256 * 256);
		mapHeightmap_11B4E0[i] = pseudoRand() % (256 * 256);
		mapAngle_13B4E0[i] = pseudoRand() % (256 * 256);
		tempTerrainType1[i] = mapTerrainType_10B4E0[i];
		tempHeightmap1[i] = mapHeightmap_11B4E0[i];
		tempAngle1[i] = mapAngle_13B4E0[i];
	}

	sub_43D50_orig(tempTerrainType1, tempHeightmap1, tempAngle1);

	sub_43D50();

	for (int i = 0; i < 256 * 256; i++)
	{
		EXPECT_EQ(tempTerrainType1[i], mapTerrainType_10B4E0[i]);
		EXPECT_EQ(tempHeightmap1[i], mapHeightmap_11B4E0[i]);
		EXPECT_EQ(tempAngle1[i], mapAngle_13B4E0[i]);
	}

	free(tempAngle1);
	free(tempTerrainType1);
	free(tempHeightmap1);
}

TEST(Terrain, sub_44D00) {

	uint8_t* tempShading1 = (uint8_t*)malloc(65536);
	uint8_t* tempHeightmap1 = (uint8_t*)malloc(65536);

	for (int j = 0; j < 40; j++)
	{
		//Create Test Data
		switch (pseudoRand() % 3)
		{
		case 0:
			D41A0_0.terrain_2FECE.MapType = MapType_t::Day;
			break;
		case 1:
			D41A0_0.terrain_2FECE.MapType = MapType_t::Night;
			break;
		case 2:
			D41A0_0.terrain_2FECE.MapType = MapType_t::Cave;
			break;
		}
		for (int i = 0; i < 256 * 256; i++)
		{
			mapHeightmap_11B4E0[i] = pseudoRand() % (256 * 256);
			mapShading_12B4E0[i] = pseudoRand() % (256 * 256);
			tempHeightmap1[i] = mapHeightmap_11B4E0[i];
			tempShading1[i] = mapShading_12B4E0[i];
		}

		sub_44D00_orig(D41A0_0.terrain_2FECE.MapType, tempHeightmap1, tempShading1);

		sub_44D00();

		for (int i = 0; i < 256 * 256; i++)
		{
			EXPECT_EQ(tempHeightmap1[i], mapHeightmap_11B4E0[i]);
			EXPECT_EQ(tempShading1[i], mapShading_12B4E0[i]);
		}
	}

	free(tempShading1);
	free(tempHeightmap1);
}

TEST(Terrain, sub_B5E70_decompress_terrain_map_level) {

	int16_t* tempEnt1 = (int16_t*)malloc(65536 * sizeof(int16_t));

	for (int j = 0; j < 1000; j++)
	{
		//Create Test Data
		__int16 a1 = (pseudoRand() % (256 * 256)) - 128 * 256;
		unsigned __int16 a2 = pseudoRand() % (256 * 256);
		__int16 a3 = (pseudoRand() % (256 * 256)) - 128 * 256;
		int32_t a4 = (pseudoRand() % (256 * 256)) - 128 * 256;
		for (int i = 0; i < 256 * 256; i++)
		{
			mapEntityIndex_15B4E0[i] = pseudoRand() % (256 * 256);
			tempEnt1[i] = mapEntityIndex_15B4E0[i];
		}

		sub_B5E70_decompress_terrain_map_level_orig(a1, a2, a3, a4, tempEnt1);

		sub_B5E70_decompress_terrain_map_level(a1, a2, a3, a4);

		for (int i = 0; i < 256 * 256; i++)
		{
			EXPECT_EQ(tempEnt1[i], mapEntityIndex_15B4E0[i]);
		}
	}
	free(tempEnt1);
}

TEST(Terrain, sub_439A0) {

	uint8_t* tempAng1 = (uint8_t*)malloc(65536);
	uint8_t* tempHeight1 = (uint8_t*)malloc(65536);

	for (int j = 0; j < 256 * 256; j++)
	{
		if ((j > 260) && (j < 256 * 256 - 260))//skip steps for quick compare
			continue;

		//Create Test Data
		for (int i = 0; i < 256 * 256; i++)
		{
			mapAngle_13B4E0[i] = pseudoRand() % 256;
			mapHeightmap_11B4E0[i] = pseudoRand() % 256;
			tempAng1[i] = mapAngle_13B4E0[i];
			tempHeight1[i] = mapHeightmap_11B4E0[i];
		}

		sub_439A0_orig(j, tempHeight1, tempAng1);

		sub_439A0(j);

		for (int i = 0; i < 256 * 256; i++)
		{
			EXPECT_EQ(tempHeight1[i], mapHeightmap_11B4E0[i]);
			EXPECT_EQ(tempAng1[i], mapAngle_13B4E0[i]);
		}
	}
	free(tempAng1);
	free(tempHeight1);
}

TEST(Terrain, sub_43B40) {

	uint8_t* tempMapHeightmap1 = (uint8_t*)malloc(65536);
	uint8_t* tempSecondHeightmap1 = (uint8_t*)malloc(65536);
	uint8_t* tempAng1 = (uint8_t*)malloc(65536);
	x_BYTE_14B4E0_second_heightmap = (uint8_t*)malloc(65536);
	uint16_t temp_x_BYTE_D41B7_1;

	//Create Test Data
	for (int i = 0; i < 256 * 256; i++)
	{
		mapAngle_13B4E0[i] = pseudoRand() % 256;
		mapHeightmap_11B4E0[i] = pseudoRand() % 256;
		x_BYTE_14B4E0_second_heightmap[i] = pseudoRand() % 256;
		tempAng1[i] = mapAngle_13B4E0[i];
		tempMapHeightmap1[i] = mapHeightmap_11B4E0[i];
		tempSecondHeightmap1[i] = x_BYTE_14B4E0_second_heightmap[i];
	}
	x_BYTE_D41B7 = pseudoRand() % 256;
	temp_x_BYTE_D41B7_1 = x_BYTE_D41B7;

	sub_43B40_orig(temp_x_BYTE_D41B7_1, tempMapHeightmap1, tempAng1, tempSecondHeightmap1);

	sub_43B40();

	EXPECT_EQ(temp_x_BYTE_D41B7_1, x_BYTE_D41B7);

	for (int i = 0; i < 256 * 256; i++)
	{
		EXPECT_EQ(tempAng1[i], mapAngle_13B4E0[i]);
		EXPECT_EQ(tempMapHeightmap1[i], mapHeightmap_11B4E0[i]);
		EXPECT_EQ(tempSecondHeightmap1[i], x_BYTE_14B4E0_second_heightmap[i]);
	}

	free(tempMapHeightmap1);
	free(tempSecondHeightmap1);
	free(tempAng1);
	free(x_BYTE_14B4E0_second_heightmap);
}

TEST(Terrain, sub_44580) {

	pdwScreenBuffer_351628 = (uint8_t*)malloc(0x961u * 25);

	uint8_t* tempScreenBuffer1 = (uint8_t*)malloc(0x961u * 25);
	uint8_t* tempAng1 = (uint8_t*)malloc(65536);
	uint8_t* tempTerrainType1 = (uint8_t*)malloc(65536);
	uint16_t temp_x_WORD_17B4E0_1;
	char tempx_BYTE_F2CD0x1[7 * 7 * 7 * 7][2];

	//Create Test Data
	for (int i = 0; i < 256 * 256; i++)
	{
		mapAngle_13B4E0[i] = pseudoRand() % 256;
		mapTerrainType_10B4E0[i] = pseudoRand() % 256;
		tempAng1[i] = mapAngle_13B4E0[i];
		tempTerrainType1[i] = mapTerrainType_10B4E0[i];
	}
	for (int i = 0; i < 7 * 7 * 7 * 7; i++)
		for (int m = 0; m < 2; m++)
		{
			x_BYTE_F2CD0x[i][m] = pseudoRand() % 256;
			tempx_BYTE_F2CD0x1[i][m] = x_BYTE_F2CD0x[i][m];
		}
	for (int i = 0; i < 0x961u * 25; i++)
	{
		pdwScreenBuffer_351628[i] = pseudoRand() % 256;
		tempScreenBuffer1[i] = pdwScreenBuffer_351628[i];
	}
	x_WORD_17B4E0 = pseudoRand() % (256 * 256);
	temp_x_WORD_17B4E0_1 = x_WORD_17B4E0;

	sub_44580_orig(tempScreenBuffer1, temp_x_WORD_17B4E0_1, tempx_BYTE_F2CD0x1, tempTerrainType1, tempAng1);

	sub_44580();

	for (int i = 0; i < 7 * 7 * 7 * 7; i++)
		for (int m = 0; m < 2; m++)
		{
			EXPECT_EQ(tempx_BYTE_F2CD0x1[i][m], x_BYTE_F2CD0x[i][m]);
		}

	EXPECT_EQ(temp_x_WORD_17B4E0_1, x_WORD_17B4E0);

	for (int i = 0; i < 0x961u * 25; i++)
	{
		EXPECT_EQ(tempScreenBuffer1[i], pdwScreenBuffer_351628[i]);
	}
	for (int i = 0; i < 256 * 256; i++)
	{
		EXPECT_EQ(tempAng1[i], mapAngle_13B4E0[i]);
		EXPECT_EQ(tempTerrainType1[i], mapTerrainType_10B4E0[i]);
	}
	free(tempAng1);
	free(tempTerrainType1);
	free(tempScreenBuffer1);
	free(pdwScreenBuffer_351628);
}

TEST(Terrain, sub_43EE0) {

	uint8_t* tempAng1 = (uint8_t*)malloc(65536);
	uint8_t* tempHeight1 = (uint8_t*)malloc(65536);

	//Create Test Data
	for (int i = 0; i < 256 * 256; i++)
	{
		mapAngle_13B4E0[i] = pseudoRand() % 256;
		mapHeightmap_11B4E0[i] = pseudoRand() % 256;
		tempAng1[i] = mapAngle_13B4E0[i];
		tempHeight1[i] = mapHeightmap_11B4E0[i];
	}

	sub_43EE0_orig(tempHeight1, tempAng1);

	sub_43EE0();

	for (int i = 0; i < 256 * 256; i++)
	{
		EXPECT_EQ(tempAng1[i], mapAngle_13B4E0[i]);
		EXPECT_EQ(tempHeight1[i], mapHeightmap_11B4E0[i]);
	}
	free(tempAng1);
	free(tempHeight1);
}

TEST(Terrain, sub_43FC0) {

	uint8_t* tempAng1 = (uint8_t*)malloc(65536);

	//Create Test Data
	for (int i = 0; i < 256 * 256; i++)
	{
		mapAngle_13B4E0[i] = pseudoRand() % 256;
		tempAng1[i] = mapAngle_13B4E0[i];
	}

	sub_43FC0_orig(tempAng1);

	sub_43FC0();

	for (int i = 0; i < 256 * 256; i++)
	{
		EXPECT_EQ(tempAng1[i], mapAngle_13B4E0[i]);
	}
	free(tempAng1);
}

TEST(Terrain, sub_45600) {

	uint8_t* tempTerrainType1 = (uint8_t*)malloc(65536);
	uint8_t* tempAng1 = (uint8_t*)malloc(65536);
	uint8_t* tempHeight1 = (uint8_t*)malloc(65536);

	for (int j = 0; j < 256; j++)
	{
		//Create Test Data
		for (int i = 0; i < 256 * 256; i++)
		{
			mapTerrainType_10B4E0[i] = pseudoRand() % (256 * 256);
			mapAngle_13B4E0[i] = pseudoRand() % 256;
			mapHeightmap_11B4E0[i] = pseudoRand() % 256;
			tempTerrainType1[i] = mapTerrainType_10B4E0[i];
			tempAng1[i] = mapAngle_13B4E0[i];
			tempHeight1[i] = mapHeightmap_11B4E0[i];
		}

		sub_45600_orig(j, tempTerrainType1, tempHeight1, tempAng1);

		sub_45600(j);

		for (int i = 0; i < 256 * 256; i++)
		{
			EXPECT_EQ(tempTerrainType1[i], mapTerrainType_10B4E0[i]);
			EXPECT_EQ(tempAng1[i], mapAngle_13B4E0[i]);
			EXPECT_EQ(tempHeight1[i], mapHeightmap_11B4E0[i]);
		}
	}
	free(tempTerrainType1);
	free(tempAng1);
	free(tempHeight1);
}

TEST(Terrain, sub_454F0) {

	uint8_t* tempHeight1 = (uint8_t*)malloc(65536);
	uint8_t* tempAng1 = (uint8_t*)malloc(65536);

	for (int y = 0; y < 256; y++)
	{
		if ((y > 3) && (y < 256 - 3))//skip steps for quick compare
			continue;
		for (int x = 0; x < 256; x++)
		{
			//Create Test Data
			for (int i = 0; i < 256 * 256; i++)
			{
				mapAngle_13B4E0[i] = pseudoRand() % 256;
				mapHeightmap_11B4E0[i] = pseudoRand() % 256;
				tempAng1[i] = mapAngle_13B4E0[i];
				tempHeight1[i] = mapHeightmap_11B4E0[i];
			}

			sub_454F0_orig(x, y, tempHeight1, tempAng1);

			sub_454F0(x, y);

			for (int i = 0; i < 256 * 256; i++)
			{
				EXPECT_EQ(tempAng1[i], mapAngle_13B4E0[i]);
				EXPECT_EQ(tempHeight1[i], mapHeightmap_11B4E0[i]);
			}
		}
	}
	free(tempHeight1);
	free(tempAng1);
}

TEST(Terrain, sub_45210) {

	uint8_t* tempTerrainType1 = (uint8_t*)malloc(65536);
	uint8_t* tempAng1 = (uint8_t*)malloc(65536);
	uint8_t* tempHeight1 = (uint8_t*)malloc(65536);

	for (int y = 0; y < 256; y++)
	{
		if ((y > 3) && (y < 256 - 3))//skip steps for quick compare
			continue;
		for (int x = 0; x < 256; x++)
		{
			//Create Test Data
			for (int i = 0; i < 256 * 256; i++)
			{
				mapTerrainType_10B4E0[i] = pseudoRand() % (256 * 256);
				mapAngle_13B4E0[i] = pseudoRand() % 256;
				mapHeightmap_11B4E0[i] = pseudoRand() % 256;
				tempTerrainType1[i] = mapTerrainType_10B4E0[i];
				tempAng1[i] = mapAngle_13B4E0[i];
				tempHeight1[i] = mapHeightmap_11B4E0[i];
			}
			sub_45210_orig(x, y, tempTerrainType1, tempHeight1, tempAng1);

			sub_45210(x, y);

			for (int i = 0; i < 256 * 256; i++)
			{
				EXPECT_EQ(tempTerrainType1[i], mapTerrainType_10B4E0[i]);
				EXPECT_EQ(tempAng1[i], mapAngle_13B4E0[i]);
				EXPECT_EQ(tempHeight1[i], mapHeightmap_11B4E0[i]);
			}
		}
	}
	free(tempTerrainType1);
	free(tempAng1);
	free(tempHeight1);
}

TEST(Terrain, sub_45060) {

	uint8_t* tempTerrainType1 = (uint8_t*)malloc(65536);
	uint8_t* tempAng1 = (uint8_t*)malloc(65536);
	uint8_t* tempHeight1 = (uint8_t*)malloc(65536);

	for (int y = 0; y < 256; y++)
	{
		if ((y > 3) && (y < 256 - 3))//skip steps for quick compare
			continue;
		for (int x = 0; x < 256; x++)
		{
			//Create Test Data
			for (int i = 0; i < 256 * 256; i++)
			{
				mapTerrainType_10B4E0[i] = pseudoRand() % (256 * 256);
				mapAngle_13B4E0[i] = pseudoRand() % 256;
				mapHeightmap_11B4E0[i] = pseudoRand() % 256;
				tempTerrainType1[i] = mapTerrainType_10B4E0[i];
				tempAng1[i] = mapAngle_13B4E0[i];
				tempHeight1[i] = mapHeightmap_11B4E0[i];
			}

			sub_45060_orig(x, y, mapTerrainType_10B4E0, mapHeightmap_11B4E0, mapAngle_13B4E0);
			
			sub_45060(x, y);

			for (int i = 0; i < 256 * 256; i++)
			{
				EXPECT_EQ(tempTerrainType1[i], mapTerrainType_10B4E0[i]);
				EXPECT_EQ(tempAng1[i], mapAngle_13B4E0[i]);
				EXPECT_EQ(tempHeight1[i], mapHeightmap_11B4E0[i]);
			}
		}
	}
	free(tempTerrainType1);
	free(tempAng1);
	free(tempHeight1);
}