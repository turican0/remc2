#include "Terrain.h"
#include "EventsFunctions.h"
#include "CommandLineParser.h"
#include "GameUI.h"
#include "Graphics.h"
#include "MenusAndIntros.h"
#include "Network.h"
#include "Player.h"
#include "PlayerInput.h"
#include "Events.h"
#include "Level.h"
#include "../utilities/StateMonitor.h"
#include "../utilities/RendererTests.h"
#include "ConvertMapInfo.h"
#include "LevelInit.h"
#include "DatTabIndexes.h"
#include "MouseOnOff.h"
#include "PrintMessages.h"
#include "ReadAndDecompress.h"
#include "Type_DB080_CdTrack.h"


#define FIX_SetHeightmapByBuilding_48B90// - when set game have error, but compactible with original

uint16_t rand2_17B4E0; // 34c4e0
bool lowDiffHeightmap_D47DC = true;

uint8_t mapTerrainType_10B4E0[65536]; // 256x256 //map array1
uint8_t mapHeightmap_11B4E0[65536]; // 256x256 //map array2 // heightmap
uint8_t mapShading_12B4E0[65536]; // 256x256 //map array3
uint8_t mapAngle_13B4E0[65536]; // 256x256 //map array4 // water //!!! find all comparation, change to int8_t and test it !!!
int16_t mapEntityIndex_15B4E0[65536]; // 256x256 //map array5

char building_F2CD0x[2800][2]; // 233cd0//4802 //4816 // size has to be 2 * (7^4 + 7^3 + 7^2 + 7^1) = 2 * (2401 + 343 + 49 + 7) = 2*2800 as seen in sub_462A0_orig
//0-terrainTileType, 1-rotationTile

uint8_t MapBasicHeight_D41B7 = 44; // weak

int debugcounter_224959 = 0;
//----- (00043830) --------------------------------------------------------
void GenerateLevelMap_43830(Type_Level_2FECE* a2x)//224830
{
	rand2_17B4E0 = a2x->seed_0x2FEE5;
	world_D41A0.rand_0x8 = a2x->seed_0x2FEE5;
	memset((void*)mapEntityIndex_15B4E0, 0, 0x20000);
	DecompressTerrainMapLevel_B5E70(rand2_17B4E0, a2x->offset_0x2FEE9, a2x->raise_0x2FEED, a2x->gnarl_0x2FEF1);

	TruncTerrainHeight_44DB0(mapEntityIndex_15B4E0, mapHeightmap_11B4E0);//225db0 //trunc and create

	memset((void*)mapEntityIndex_15B4E0, 0, 0x20000);
	MarkLandAndRivers_44E40(a2x->river_0x2FEF5, a2x->lriver_0x2FEF9);//225e40 //add any fields

	SetMax4Tiles_45AA0();//226aa0

	ApplySnowLine_440D0(a2x->snLin_0x2FF01);//2250d0

	MarkFlatLowland_45060(a2x->snFlt_0x2FF05, a2x->bhLin_0x2FF09);//226060

	SpreadTerrainClass4_44320();//225320

	MarkFlatLowlandClass2_45210(a2x->snFlt_0x2FF05, a2x->bhLin_0x2FF09);//226210

	MarkFlatHighland_454F0(a2x->source_0x2FEFD, a2x->rkSte_0x2FF11);//2264f0

	MarkSteepTerrain_45600(a2x->bhFlt_0x2FF0D);//226600

	RemoveIsolatedTerrainClasses_43FC0();//224fc0
	memset((void*)mapTerrainType_10B4E0, 0, 0x10000);
	SmoothTerrain_43970();//224970 // smooth terrain
	AddRivers_43EE0();//224ee0 // add rivers

	SetTerrainAngles_44580();//225580 //set angle of terrain
	if (isCaveLevel_D41B6)
		GenerateCaveCeiling_43B40();//224b40 //change angle of terrain
	else
		MarkWaterEdges_43D50();//224d50 //change angle of terrain
	ComputeTerrainShading_44D00();//225d00
}

//----- (000B5E70) --------------------------------------------------------
void DecompressTerrainMapLevel_B5E70(__int16 a1, unsigned __int16 a2, __int16 a3, int32_t a4)//296e70
{
	uaxis_2d sumEnt;
	mapEntityIndex_15B4E0[a2] = a3;//32c4e0 //first seed
	for(int i = 7; i >= 0 ; i--)
	{
		sumEnt.word = a2;
		for (int j = 1 << (7 - i); j > 0; j--)
		{
			for (int k = 1 << (7 - i); k > 0; k--)
			{
				TerrainDiamondStep_B5EFA(1 << i, &sumEnt, a4, &a1);//355220
			}
			sumEnt.word += (2 * (1 << i)) << 8;
		}
		for (int j = 1 << (7 - i); j > 0; j--)
		{
			for (int k = 1 << (7 - i); k > 0; k--)
			{
				TerrainSquareStep_B5F8F(1 << i, &sumEnt, a4, &a1);
			}
			sumEnt.word += (2 * (1 << i)) << 8;
		}
	}
}

//----- (00044DB0) --------------------------------------------------------
void TruncTerrainHeight_44DB0(int16_t mapEntityIndex_15B4E0[], uint8_t mapHeightmap_11B4E0[])//225db0 // map to heightmap
{
	int revMaxEnt = 0;
	uint32_t weightedVar;
	int maxEnt = -32000;
	int minEnt = 32000;
	for(int i = 0; i< 256 * 256; i++)
	{
		if (mapEntityIndex_15B4E0[i] > maxEnt)
			maxEnt = mapEntityIndex_15B4E0[i];
		if (mapEntityIndex_15B4E0[i] < minEnt)
			minEnt = mapEntityIndex_15B4E0[i];
	}
	if (maxEnt)
		revMaxEnt = 0xC40000 / maxEnt;
	for (int i = 0; i < 256 * 256; i++)
	{
		weightedVar = revMaxEnt * mapEntityIndex_15B4E0[i] >> 16;
		mapEntityIndex_15B4E0[i] = 0;
		if ((weightedVar & 0x8000u) != 0)//water level trunc
			weightedVar = 0;
		if (weightedVar > 196)//trunc max height
			weightedVar = 196;
		mapHeightmap_11B4E0[i] = weightedVar;
	}
}

int GetTerrainAlt2_B5C60(uint16_t axisX, uint16_t axisY)//296c60
{
	int point1;
	int diaPoint;
	int result;
	int point2;
	
	uaxis_2d locX;
	uaxis_2d locY;
	uaxis_2d locV2;

	locX.word = axisX;
	locY.word = axisY;

	locV2._axis_2d.x = locX._axis_2d.y;
	locV2._axis_2d.y = locY._axis_2d.y;

	if ((locV2._axis_2d.x + locV2._axis_2d.y) & 1)
	{
		if (locY._axis_2d.x > (locY._axis_2d.x + locX._axis_2d.x) % 256)
		{
			locV2._axis_2d.y++;
			point1 = mapHeightmap_11B4E0[locV2.word];
			locV2._axis_2d.x++;
			point2 = mapHeightmap_11B4E0[locV2.word];
			locV2._axis_2d.y--;
			diaPoint = (255 - locY._axis_2d.x) * (mapHeightmap_11B4E0[locV2.word] - point2) + locX._axis_2d.x * (point2 - point1);
		}
		else
		{
			point1 = mapHeightmap_11B4E0[locV2.word];
			locV2._axis_2d.x++;
			point2 = locX._axis_2d.x * (mapHeightmap_11B4E0[locV2.word] - point1);
			locV2._axis_2d.x--;
			locV2._axis_2d.y++;
			diaPoint = locY._axis_2d.x * (mapHeightmap_11B4E0[locV2.word] - point1) + point2;
		}
		result = (diaPoint >> 3) + 32 * point1;
	}
	else
	{
		if (locX._axis_2d.x <= locY._axis_2d.x)
		{
			point1 = mapHeightmap_11B4E0[locV2.word];
			locV2._axis_2d.y++;
			point2 = mapHeightmap_11B4E0[locV2.word];
			locV2._axis_2d.x++;
			diaPoint = locY._axis_2d.x * (point2 - point1) + locX._axis_2d.x * (mapHeightmap_11B4E0[locV2.word] - point2);
		}
		else
		{
			point1 = mapHeightmap_11B4E0[locV2.word];
			locV2._axis_2d.x++;
			point2 = mapHeightmap_11B4E0[locV2.word];
			locV2._axis_2d.y++;
			diaPoint = locY._axis_2d.x * (mapHeightmap_11B4E0[locV2.word] - point2) + locX._axis_2d.x * (point2 - point1);
		}
		result = (diaPoint >> 3) + 32 * point1;
	}
	return result;
}

//----- (00044E40) --------------------------------------------------------
void MarkLandAndRivers_44E40(int count, uint8_t minSmooth)//225e40
{
	uaxis_2d index;
	int locCount = count;
	for (int i = 0; i < 256 * 256; i++)
	{
		if (mapHeightmap_11B4E0[i])
			mapAngle_13B4E0[i] = 5;
		else
			mapAngle_13B4E0[i] = 0;
	}
	int i = 0;
	while ((locCount > 0) && (i < 1000))
	{
		for (i = 0; i < 1000; i++)
		{
			rand2_17B4E0 = 9377 * rand2_17B4E0 + 9439;
			index.word = rand2_17B4E0 % 0xffffu;
			if ((mapHeightmap_11B4E0[index.word] > minSmooth) && mapAngle_13B4E0[index.word])
			{
				SmoothTiles_44EE0(index);
				locCount--;
				break;
			}
		}
	}
	for (int i = 0; i < 256 * 256; i++)
	{
		mapTerrainType_10B4E0[i] = 255;
	}
}

//----- (00045AA0) --------------------------------------------------------
void SetMax4Tiles_45AA0()//226aa0
{
	//  X-X
	//  | |
	//  B-X

	uaxis_2d indexx;
	char angleIndex;
	uint8_t minHeight;
	uint8_t maxHeight;
	bool runAgain;

	do
	{
		runAgain = false;
		for (int i = 0; i < 256 * 256; i++)
		{
			indexx.word = i;
			angleIndex = 0;
			if (!mapAngle_13B4E0[indexx.word])
				angleIndex = 1;
			minHeight = mapHeightmap_11B4E0[indexx.word];
			maxHeight = minHeight;
			indexx._axis_2d.x++;
			if (!mapAngle_13B4E0[indexx.word])
				angleIndex++;
			if (minHeight > mapHeightmap_11B4E0[indexx.word])
				minHeight = mapHeightmap_11B4E0[indexx.word];
			if (maxHeight < mapHeightmap_11B4E0[indexx.word])
				maxHeight = mapHeightmap_11B4E0[indexx.word];
			indexx._axis_2d.y++;
			if (!mapAngle_13B4E0[indexx.word])
				angleIndex++;
			if (minHeight > mapHeightmap_11B4E0[indexx.word])
				minHeight = mapHeightmap_11B4E0[indexx.word];
			if (maxHeight < mapHeightmap_11B4E0[indexx.word])
				maxHeight = mapHeightmap_11B4E0[indexx.word];
			indexx._axis_2d.x--;
			if (!mapAngle_13B4E0[indexx.word])
				angleIndex++;
			if (minHeight > mapHeightmap_11B4E0[indexx.word])
				minHeight = mapHeightmap_11B4E0[indexx.word];
			if (maxHeight < mapHeightmap_11B4E0[indexx.word])
				maxHeight = mapHeightmap_11B4E0[indexx.word];
			indexx._axis_2d.y--;
			if (maxHeight != minHeight && angleIndex == 4)
			{
				runAgain = true;
				mapHeightmap_11B4E0[indexx.word] = minHeight;
				indexx._axis_2d.x++;
				mapHeightmap_11B4E0[indexx.word] = minHeight;
				indexx._axis_2d.y++;
				mapHeightmap_11B4E0[indexx.word] = minHeight;
				indexx._axis_2d.x--;
				mapHeightmap_11B4E0[indexx.word] = minHeight;
				indexx._axis_2d.y--;
			}
		}
	} while (runAgain);
}

//----- (000440D0) --------------------------------------------------------
void ApplySnowLine_440D0(unsigned __int16 a1)//2250d0
{
	//    X
	//   / \
	//  X B X
	//   \|/
	//    X

	uint8_t maxHeight;
	uint8_t minHeight;
	int diffHeight;
	char ang3;
	char ang2;
	char ang5;
	uaxis_2d index;

	for (int i = 0; i < 256 * 256; i++)
	{
		index.word = i;
		if (mapAngle_13B4E0[index.word] == 5)
		{
			maxHeight = 0;
			minHeight = 255;
			if (mapHeightmap_11B4E0[index.word])
				maxHeight = mapHeightmap_11B4E0[index.word];
			if (mapHeightmap_11B4E0[index.word] < 255)
				minHeight = mapHeightmap_11B4E0[index.word];
			index._axis_2d.y--;
			if (maxHeight < mapHeightmap_11B4E0[index.word])
				maxHeight = mapHeightmap_11B4E0[index.word];
			if (minHeight > mapHeightmap_11B4E0[index.word])
				minHeight = mapHeightmap_11B4E0[index.word];
			index._axis_2d.x++;
			index._axis_2d.y++;
			if (maxHeight < mapHeightmap_11B4E0[index.word])
				maxHeight = mapHeightmap_11B4E0[index.word];
			if (minHeight > mapHeightmap_11B4E0[index.word])
				minHeight = mapHeightmap_11B4E0[index.word];
			index._axis_2d.x--;
			index._axis_2d.y++;
			if (maxHeight < mapHeightmap_11B4E0[index.word])
				maxHeight = mapHeightmap_11B4E0[index.word];
			if (minHeight > mapHeightmap_11B4E0[index.word])
				minHeight = mapHeightmap_11B4E0[index.word];
			index._axis_2d.x--;
			index._axis_2d.y--;
			if (maxHeight < mapHeightmap_11B4E0[index.word])
				maxHeight = mapHeightmap_11B4E0[index.word];
			if (minHeight > mapHeightmap_11B4E0[index.word])
				minHeight = mapHeightmap_11B4E0[index.word];
			diffHeight = maxHeight - minHeight;
			index._axis_2d.x++;
			if (diffHeight <= a1)
			{
				if (diffHeight == a1)
					mapAngle_13B4E0[index.word] = 4;
				else
					mapAngle_13B4E0[index.word] = 3;
			}
		}
	}

	//  X-X
	//  | |
	//  B-X

	for (int i = 0; i < 256 * 256; i++)	
	{
		index.word = i;
		ang3 = 0;
		ang2 = 0;
		ang5 = 0;
		if (mapAngle_13B4E0[index.word] == 3)
			ang3 = 1;
		if (mapAngle_13B4E0[index.word] == 2)
			ang2 = 1;
		if (mapAngle_13B4E0[index.word] == 5)
			ang5 = 1;
		index._axis_2d.x++;
		if (mapAngle_13B4E0[index.word] == 3)
			ang3++;
		if (mapAngle_13B4E0[index.word] == 2)
			ang2++;
		if (mapAngle_13B4E0[index.word] == 5)
			ang5++;
		index._axis_2d.y++;
		if (mapAngle_13B4E0[index.word] == 3)
			ang3++;
		if (mapAngle_13B4E0[index.word] == 2)
			ang2++;
		if (mapAngle_13B4E0[index.word] == 5)
			ang5++;
		index._axis_2d.x--;
		if (mapAngle_13B4E0[index.word] == 3)
			ang3++;
		if (mapAngle_13B4E0[index.word] == 2)
			ang2++;
		if (mapAngle_13B4E0[index.word] == 5)
			ang5++;
		index._axis_2d.y--;
		if (!ang2 && ang3 && ang5)
		{
			if (mapAngle_13B4E0[index.word] == 3)
				mapAngle_13B4E0[index.word] = 4;
			index._axis_2d.x++;
			if (mapAngle_13B4E0[index.word] == 3)
				mapAngle_13B4E0[index.word] = 4;
			index._axis_2d.y++;
			if (mapAngle_13B4E0[index.word] == 3)
				mapAngle_13B4E0[index.word] = 4;
			index._axis_2d.x--;
			if (mapAngle_13B4E0[index.word] == 3)
				mapAngle_13B4E0[index.word] = 4;
			index._axis_2d.y--;			
		}
	}
}

//----- (00045060) --------------------------------------------------------
void MarkFlatLowland_45060(uint8_t maxHeightCut, uint8_t maxHeightDiffCut)//226060
{
	//  X-X-X
	//  |   |
	//  X B X
	//  |/| |
	//  X X-X
	
	uint8_t maxHeight;
	uint8_t minHeight;
	uaxis_2d index;
	qmemcpy(mapTerrainType_10B4E0, mapAngle_13B4E0, 0x10000);
	for (int i = 0; i < 256 * 256; i++)
	{
		index.word = i;
		maxHeight = 0;
		minHeight = 255;
		if (mapHeightmap_11B4E0[index.word])
			maxHeight = mapHeightmap_11B4E0[index.word];
		if (mapHeightmap_11B4E0[index.word] < 255)
			minHeight = mapHeightmap_11B4E0[index.word];
		index._axis_2d.y--;
		if (maxHeight < mapHeightmap_11B4E0[index.word])
			maxHeight = mapHeightmap_11B4E0[index.word];
		if (minHeight > mapHeightmap_11B4E0[index.word])
			minHeight = mapHeightmap_11B4E0[index.word];
		index._axis_2d.x++;
		if (maxHeight < mapHeightmap_11B4E0[index.word])
			maxHeight = mapHeightmap_11B4E0[index.word];
		if (minHeight > mapHeightmap_11B4E0[index.word])
			minHeight = mapHeightmap_11B4E0[index.word];
		index._axis_2d.y++;
		if (maxHeight < mapHeightmap_11B4E0[index.word])
			maxHeight = mapHeightmap_11B4E0[index.word];
		if (minHeight > mapHeightmap_11B4E0[index.word])
			minHeight = mapHeightmap_11B4E0[index.word];
		index._axis_2d.y++;
		if (maxHeight < mapHeightmap_11B4E0[index.word])
			maxHeight = mapHeightmap_11B4E0[index.word];
		if (minHeight > mapHeightmap_11B4E0[index.word])
			minHeight = mapHeightmap_11B4E0[index.word];
		index._axis_2d.x--;
		if (maxHeight < mapHeightmap_11B4E0[index.word])
			maxHeight = mapHeightmap_11B4E0[index.word];
		if (minHeight > mapHeightmap_11B4E0[index.word])
			minHeight = mapHeightmap_11B4E0[index.word];
		index._axis_2d.x--;
		if (maxHeight < mapHeightmap_11B4E0[index.word])
			maxHeight = mapHeightmap_11B4E0[index.word];
		if (minHeight > mapHeightmap_11B4E0[index.word])
			minHeight = mapHeightmap_11B4E0[index.word];
		index._axis_2d.y--;
		if (maxHeight < mapHeightmap_11B4E0[index.word])
			maxHeight = mapHeightmap_11B4E0[index.word];
		if (minHeight > mapHeightmap_11B4E0[index.word])
			minHeight = mapHeightmap_11B4E0[index.word];
		index._axis_2d.y--;
		if (maxHeight < mapHeightmap_11B4E0[index.word])
			maxHeight = mapHeightmap_11B4E0[index.word];
		if (minHeight > mapHeightmap_11B4E0[index.word])
			minHeight = mapHeightmap_11B4E0[index.word];
		index._axis_2d.x++;
		index._axis_2d.y++;
		if (maxHeight < maxHeightCut && maxHeight - minHeight <= maxHeightDiffCut)
		{
			if (mapAngle_13B4E0[index.word])
				mapAngle_13B4E0[index.word] = 5;
		}
	}
}

//----- (00044320) --------------------------------------------------------
void SpreadTerrainClass4_44320()//225320
{
	//  X-X
	//  | |
	//  B-X

	char ang0;
	char ang3;
	char ang5;

	uaxis_2d index;
	for (int i = 0; i < 256 * 256; i++)
	{
		index.word = i;
		ang0 = 0;
		ang3 = 0;
		ang5 = 0;
		if (!mapAngle_13B4E0[index.word])
			ang0 = 1;
		if (mapAngle_13B4E0[index.word] == 5)
			ang5 = 1;
		if (mapAngle_13B4E0[index.word] == 3)
			ang3 = 1;
		index._axis_2d.x++;
		if (!mapAngle_13B4E0[index.word])
			ang0++;
		if (mapAngle_13B4E0[index.word] == 5)
			ang5++;
		if (mapAngle_13B4E0[index.word] == 3)
			ang3++;
		index._axis_2d.y++;
		if (!mapAngle_13B4E0[index.word])
			ang0++;
		if (mapAngle_13B4E0[index.word] == 5)
			ang5++;
		if (mapAngle_13B4E0[index.word] == 3)
			ang3++;
		index._axis_2d.x--;
		if (!mapAngle_13B4E0[index.word])
			ang0++;
		if (mapAngle_13B4E0[index.word] == 5)
			ang5++;
		if (mapAngle_13B4E0[index.word] == 3)
			ang3++;
		index._axis_2d.y--;
		if (ang3 && ang5)
		{
			if (mapAngle_13B4E0[index.word] == 5)
				mapAngle_13B4E0[index.word] = 4;
			index._axis_2d.x++;
			if (mapAngle_13B4E0[index.word] == 5)
				mapAngle_13B4E0[index.word] = 4;
			index._axis_2d.y++;
			if (mapAngle_13B4E0[index.word] == 5)
				mapAngle_13B4E0[index.word] = 4;
			index._axis_2d.x--;
			if (mapAngle_13B4E0[index.word] == 5)
				mapAngle_13B4E0[index.word] = 4;
			index._axis_2d.y--;
		}
		if (ang3 && ang0)
		{
			if (mapAngle_13B4E0[index.word] == 3)
				mapAngle_13B4E0[index.word] = 4;
			index._axis_2d.x++;
			if (mapAngle_13B4E0[index.word] == 3)
				mapAngle_13B4E0[index.word] = 4;
			index._axis_2d.y++;
			if (mapAngle_13B4E0[index.word] == 3)
				mapAngle_13B4E0[index.word] = 4;
			index._axis_2d.x--;
			if (mapAngle_13B4E0[index.word] == 3)
				mapAngle_13B4E0[index.word] = 4;
			index._axis_2d.y--;
		}
		if (ang0 && ang5)
		{
			if (mapAngle_13B4E0[index.word])
				mapAngle_13B4E0[index.word] = 4;
			index._axis_2d.x++;
			if (mapAngle_13B4E0[index.word])
				mapAngle_13B4E0[index.word] = 4;
			index._axis_2d.y++;
			if (mapAngle_13B4E0[index.word])
				mapAngle_13B4E0[index.word] = 4;
			index._axis_2d.x--;
			if (mapAngle_13B4E0[index.word])
				mapAngle_13B4E0[index.word] = 4;
			index._axis_2d.y--;
		}
	}
}

//----- (00045210) --------------------------------------------------------
void MarkFlatLowlandClass2_45210(uint8_t maxHeightCut, uint8_t maxHeightDiffCut)//226210
{
	//  X-X-X
	//  |   |
	//  X B X
	//  |/| |
	//  X X-X

	uint8_t ang2;
	uint8_t ang5;
	uint8_t maxHeight;
	uint8_t minHeight;

	qmemcpy(mapTerrainType_10B4E0, mapAngle_13B4E0, 0x10000);
	uaxis_2d index;
	for (int i = 0; i < 256 * 256; i++)
	{
		index.word = i;
		minHeight = 255;
		maxHeight = 0;
		ang2 = 0;
		ang5 = 0;
		if (mapHeightmap_11B4E0[index.word] > 0)
			maxHeight = mapHeightmap_11B4E0[index.word];
		if (mapHeightmap_11B4E0[index.word] < 255)
			minHeight = mapHeightmap_11B4E0[index.word];
		index._axis_2d.y--;
		if (maxHeight < mapHeightmap_11B4E0[index.word])
			maxHeight = mapHeightmap_11B4E0[index.word];
		if (minHeight > mapHeightmap_11B4E0[index.word])
			minHeight = mapHeightmap_11B4E0[index.word];
		if (mapAngle_13B4E0[index.word] == 5)
			ang5 = 1;
		if (mapAngle_13B4E0[index.word] == 2)
			ang2 = 1;
		index._axis_2d.x++;
		if (maxHeight < mapHeightmap_11B4E0[index.word])
			maxHeight = mapHeightmap_11B4E0[index.word];
		if (minHeight > mapHeightmap_11B4E0[index.word])
			minHeight = mapHeightmap_11B4E0[index.word];
		if (mapAngle_13B4E0[index.word] == 5)
			ang5++;
		if (mapAngle_13B4E0[index.word] == 2)
			ang2++;
		index._axis_2d.y++;
		if (maxHeight < mapHeightmap_11B4E0[index.word])
			maxHeight = mapHeightmap_11B4E0[index.word];
		if (minHeight > mapHeightmap_11B4E0[index.word])
			minHeight = mapHeightmap_11B4E0[index.word];
		if (mapAngle_13B4E0[index.word] == 5)
			ang5++;
		if (mapAngle_13B4E0[index.word] == 2)
			ang2++;
		index._axis_2d.y++;
		if (maxHeight < mapHeightmap_11B4E0[index.word])
			maxHeight = mapHeightmap_11B4E0[index.word];
		if (minHeight > mapHeightmap_11B4E0[index.word])
			minHeight = mapHeightmap_11B4E0[index.word];
		if (mapAngle_13B4E0[index.word] == 5)
			ang5++;
		if (mapAngle_13B4E0[index.word] == 2)
			ang2++;
		index._axis_2d.x--;
		if (maxHeight < mapHeightmap_11B4E0[index.word])
			maxHeight = mapHeightmap_11B4E0[index.word];
		if (minHeight > mapHeightmap_11B4E0[index.word])
			minHeight = mapHeightmap_11B4E0[index.word];
		if (mapAngle_13B4E0[index.word] == 5)
			ang5++;
		if (mapAngle_13B4E0[index.word] == 2)
			ang2++;
		index._axis_2d.x--;
		if (maxHeight < mapHeightmap_11B4E0[index.word])
			maxHeight = mapHeightmap_11B4E0[index.word];
		if (minHeight > mapHeightmap_11B4E0[index.word])
			minHeight = mapHeightmap_11B4E0[index.word];
		if (mapAngle_13B4E0[index.word] == 5)
			ang5++;
		if (mapAngle_13B4E0[index.word] == 2)
			ang2++;
		index._axis_2d.y--;
		if (maxHeight < mapHeightmap_11B4E0[index.word])
			maxHeight = mapHeightmap_11B4E0[index.word];
		if (minHeight > mapHeightmap_11B4E0[index.word])
			minHeight = mapHeightmap_11B4E0[index.word];
		if (mapAngle_13B4E0[index.word] == 5)
			ang5++;
		if (mapAngle_13B4E0[index.word] == 2)
			ang2++;
		index._axis_2d.y--;
		if (maxHeight < mapHeightmap_11B4E0[index.word])
			maxHeight = mapHeightmap_11B4E0[index.word];
		if (minHeight > mapHeightmap_11B4E0[index.word])
			minHeight = mapHeightmap_11B4E0[index.word];
		if (mapAngle_13B4E0[index.word] == 5)
			ang5++;
		if (mapAngle_13B4E0[index.word] == 2)
			ang2++;
		index._axis_2d.x++;
		index._axis_2d.y++;
		if (maxHeight < maxHeightCut)
		{
			if (maxHeight - minHeight <= maxHeightDiffCut && mapAngle_13B4E0[index.word] == 5)
			{
				if (ang5 + ang2 == 8)
					mapAngle_13B4E0[index.word] = 2;
			}
		}
	}
}

//----- (000454F0) --------------------------------------------------------
void MarkFlatHighland_454F0(uint8_t maxHeightCut, uint8_t maxHeightDiffCut)//2264f0
{
	//    X
	//	 / \
	//  X-B X
	//    |/
	//    X

	uint8_t maxHeight;
	uint8_t minHeight;
	uaxis_2d index;
	for (int i = 0; i < 256 * 256; i++)
	{
		index.word = i;
		if (mapHeightmap_11B4E0[index.word] > maxHeightCut)
		{
			maxHeight = 0;
			minHeight = 255;
			if (mapHeightmap_11B4E0[index.word])
				maxHeight = mapHeightmap_11B4E0[index.word];
			if (mapHeightmap_11B4E0[index.word] < 255)
				minHeight = mapHeightmap_11B4E0[index.word];
			index._axis_2d.y--;
			if (maxHeight < mapHeightmap_11B4E0[index.word])
				maxHeight = mapHeightmap_11B4E0[index.word];
			if (minHeight > mapHeightmap_11B4E0[index.word])
				minHeight = mapHeightmap_11B4E0[index.word];
			index._axis_2d.x++;
			index._axis_2d.y++;
			if (maxHeight < mapHeightmap_11B4E0[index.word])
				maxHeight = mapHeightmap_11B4E0[index.word];
			if (minHeight > mapHeightmap_11B4E0[index.word])
				minHeight = mapHeightmap_11B4E0[index.word];
			index._axis_2d.x--;
			index._axis_2d.y++;
			if (maxHeight < mapHeightmap_11B4E0[index.word])
				maxHeight = mapHeightmap_11B4E0[index.word];
			if (minHeight > mapHeightmap_11B4E0[index.word])
				minHeight = mapHeightmap_11B4E0[index.word];
			index._axis_2d.x--;
			index._axis_2d.y--;
			if (maxHeight < mapHeightmap_11B4E0[index.word])
				maxHeight = mapHeightmap_11B4E0[index.word];
			if (minHeight > mapHeightmap_11B4E0[index.word])
				minHeight = mapHeightmap_11B4E0[index.word];
			index._axis_2d.x++;
			if (mapAngle_13B4E0[index.word])
			{
				if (maxHeight - minHeight < maxHeightDiffCut)
					mapAngle_13B4E0[index.word] = 6;
			}
		}
	}
}

//----- (00045600) --------------------------------------------------------
void MarkSteepTerrain_45600(uint8_t a1)//226600
{
	char ang4;
	char ang2;
	char ang3;
	char ang5;
	uint8_t maxHeight;
	uint8_t minHeight;

	qmemcpy(mapTerrainType_10B4E0, mapAngle_13B4E0, 0x10000);
	uaxis_2d index;

	//    X
	//	 / \
	//  X-B X
	//    |/
	//    X

	for (int i = 0; i < 256 * 256; i++)
	{
		index.word = i;
		maxHeight = 0;
		minHeight = 255;
		if (mapHeightmap_11B4E0[index.word])
			maxHeight = mapHeightmap_11B4E0[index.word];
		if (mapHeightmap_11B4E0[index.word] < 255)
			minHeight = mapHeightmap_11B4E0[index.word];
		index._axis_2d.y--;
		if (maxHeight < mapHeightmap_11B4E0[index.word])
			maxHeight = mapHeightmap_11B4E0[index.word];
		if (minHeight > mapHeightmap_11B4E0[index.word])
			minHeight = mapHeightmap_11B4E0[index.word];
		index._axis_2d.x++;
		index._axis_2d.y++;
		if (maxHeight < mapHeightmap_11B4E0[index.word])
			maxHeight = mapHeightmap_11B4E0[index.word];
		if (minHeight > mapHeightmap_11B4E0[index.word])
			minHeight = mapHeightmap_11B4E0[index.word];
		index._axis_2d.x--;
		index._axis_2d.y++;
		if (maxHeight < mapHeightmap_11B4E0[index.word])
			maxHeight = mapHeightmap_11B4E0[index.word];
		if (minHeight > mapHeightmap_11B4E0[index.word])
			minHeight = mapHeightmap_11B4E0[index.word];
		index._axis_2d.x--;
		index._axis_2d.y--;
		if (maxHeight < mapHeightmap_11B4E0[index.word])
			maxHeight = mapHeightmap_11B4E0[index.word];
		if (minHeight > mapHeightmap_11B4E0[index.word])
			minHeight = mapHeightmap_11B4E0[index.word];
		index._axis_2d.x++;
		if (mapAngle_13B4E0[index.word] && maxHeight - minHeight >= a1)
			mapAngle_13B4E0[index.word] = 1;
	}

	//  X-X-X
	//  |   |
	//  X B X
	//  |/| |
	//  X X-X

	for (int i = 0; i < 256 * 256; i++)
	{
		index.word = i;
		if (mapAngle_13B4E0[index.word] == 6)
		{
			ang4 = 0;
			ang2 = 0;
			ang3 = 0;
			ang5 = 0;
			index._axis_2d.y--;
			if (mapAngle_13B4E0[index.word] == 3)
				ang3 = 1;
			if (mapAngle_13B4E0[index.word] == 2)
				ang2 = 1;
			if (mapAngle_13B4E0[index.word] == 5)
				ang5 = 1;
			if (mapAngle_13B4E0[index.word] == 4)
				ang4 = 1;
			index._axis_2d.x++;
			if (mapAngle_13B4E0[index.word] == 3)
				ang3++;
			if (mapAngle_13B4E0[index.word] == 2)
				ang2++;
			if (mapAngle_13B4E0[index.word] == 5)
				ang5++;
			if (mapAngle_13B4E0[index.word] == 4)
				ang4++;
			index._axis_2d.y++;
			if (mapAngle_13B4E0[index.word] == 3)
				ang3++;
			if (mapAngle_13B4E0[index.word] == 2)
				ang2++;
			if (mapAngle_13B4E0[index.word] == 5)
				ang5++;
			if (mapAngle_13B4E0[index.word] == 4)
				ang4++;
			index._axis_2d.y++;
			if (mapAngle_13B4E0[index.word] == 3)
				ang3++;
			if (mapAngle_13B4E0[index.word] == 2)
				ang2++;
			if (mapAngle_13B4E0[index.word] == 5)
				ang5++;
			if (mapAngle_13B4E0[index.word] == 4)
				ang4++;
			index._axis_2d.x--;
			if (mapAngle_13B4E0[index.word] == 3)
				ang3++;
			if (mapAngle_13B4E0[index.word] == 2)
				ang2++;
			if (mapAngle_13B4E0[index.word] == 5)
				ang5++;
			if (mapAngle_13B4E0[index.word] == 4)
				ang4++;
			index._axis_2d.x--;
			if (mapAngle_13B4E0[index.word] == 3)
				ang3++;
			if (mapAngle_13B4E0[index.word] == 2)
				ang2++;
			if (mapAngle_13B4E0[index.word] == 5)
				ang5++;
			if (mapAngle_13B4E0[index.word] == 4)
				ang4++;
			index._axis_2d.y--;
			if (mapAngle_13B4E0[index.word] == 3)
				ang3++;
			if (mapAngle_13B4E0[index.word] == 2)
				ang2++;
			if (mapAngle_13B4E0[index.word] == 5)
				ang5++;
			if (mapAngle_13B4E0[index.word] == 4)
				ang4++;
			index._axis_2d.y--;
			if (mapAngle_13B4E0[index.word] == 3)
				ang3++;
			if (mapAngle_13B4E0[index.word] == 2)
				ang2++;
			if (mapAngle_13B4E0[index.word] == 5)
				ang5++;
			if (mapAngle_13B4E0[index.word] == 4)
				ang4++;
			index._axis_2d.x++;
			index._axis_2d.y++;
			if (ang3)
			{
				if (ang2 || ang5 || ang4)
					mapAngle_13B4E0[index.word] = 1;
			}
			else if (ang2 || (ang5 && ang4))
			{
				mapAngle_13B4E0[index.word] = 1;
			}
		}
	}
}

//----- (00043FC0) --------------------------------------------------------
void RemoveIsolatedTerrainClasses_43FC0()//224fc0
{
	int sameAngle;
	char centerAngle;

	//  X-X-X
	//  |   |
	//  X B X
	//  |/| |
	//  X X-X

	uaxis_2d index;
	for (int i = 0; i < 256 * 256; i++)
	{
		index.word = i;
		index._axis_2d.y--;
		centerAngle = mapAngle_13B4E0[index.word];
		index._axis_2d.x++;
		sameAngle = (centerAngle == mapAngle_13B4E0[index.word]);
		index._axis_2d.y++;
		sameAngle += (centerAngle == mapAngle_13B4E0[index.word]);
		index._axis_2d.y++;
		sameAngle += (centerAngle == mapAngle_13B4E0[index.word]);
		index._axis_2d.x--;
		sameAngle += (centerAngle == mapAngle_13B4E0[index.word]);
		index._axis_2d.x--;
		sameAngle += (centerAngle == mapAngle_13B4E0[index.word]);
		index._axis_2d.y--;
		sameAngle += (centerAngle == mapAngle_13B4E0[index.word]);
		index._axis_2d.y--;
		sameAngle += (centerAngle == mapAngle_13B4E0[index.word]);
		index._axis_2d.x++;
		index._axis_2d.y++;
		if (centerAngle)
		{
			if (sameAngle == 7)
				mapAngle_13B4E0[index.word] = centerAngle;
		}
	}
}

//----- (00043970) --------------------------------------------------------
void SmoothTerrain_43970()//224970
{
	uaxis_2d index;
	for (int i = 0; i < 256 * 256; i++)
	{
		index.word = i;
		mapHeightmap_11B4E0[index.word] = GetSmoothedHeight_439A0(index.word);
	}
}

//----- (00043EE0) --------------------------------------------------------
void AddRivers_43EE0()//224ee0
{
	//  X-X
	//  | |
	//  B-X

	char ang0;
	char ang4;
	uint8_t heightM1;
	uint8_t heightM2;
	char angleM1;
	char angleM2;
	char angleM3;
	uaxis_2d index;
	for (int i = 0; i < 256 * 256; i++)
	{
		ang4 = 0;
		ang0 = 0;
		index.word = i;
		heightM1 = mapHeightmap_11B4E0[index.word];
		index._axis_2d.x++;
		angleM1 = mapAngle_13B4E0[index.word];
		if (angleM1)
		{
			if (angleM1 == 4)
				ang4 = 1;
		}
		else
		{
			heightM2 = mapHeightmap_11B4E0[index.word];
			ang0 = 1;
			if (heightM2 < heightM1)
				heightM1 = heightM2;
		}
		index._axis_2d.y++;
		angleM2 = mapAngle_13B4E0[index.word];
		if (angleM2)
		{
			if (angleM2 == 4)
				ang4++;
		}
		else
		{
			ang0++;
			if (mapHeightmap_11B4E0[index.word] < heightM1)
				heightM1 = mapHeightmap_11B4E0[index.word];
		}
		index._axis_2d.x--;
		angleM3 = mapAngle_13B4E0[index.word];
		if (angleM3)
		{
			if (angleM3 == 4)
				ang4++;
		}
		else
		{
			ang0++;
			if (mapHeightmap_11B4E0[index.word] < heightM1)
				heightM1 = mapHeightmap_11B4E0[index.word];
		}
		index._axis_2d.y--;
		if (ang4 && ang0 && !heightM1)
		{
			mapHeightmap_11B4E0[index.word] = 0;
			index._axis_2d.x++;
			mapHeightmap_11B4E0[index.word] = 0;
			index._axis_2d.y++;
			mapHeightmap_11B4E0[index.word] = 0;
			index._axis_2d.x--;
			mapHeightmap_11B4E0[index.word] = 0;
			index._axis_2d.y--;
		}
	};
}

//----- (00044580) --------------------------------------------------------
void SetTerrainAngles_44580()//225580
{
	uint8_t* actBufPos;
	char point1;
	char point2;
	char point3;
	char point4;
	uint8_t actBufEnt;

	for (int i = 0; i < 2401 * 25; i += 25)
		pdwScreenBuffer_351628[i] = 0;
	for (int i = 0; i < 148; i++)
	{
		if (unk_D47E0[4 * i + 0] >= 0)
		{
			if (unk_D47E0[4 * i + 1] >= 0)
			{
				if (unk_D47E0[4 * i + 2] >= 0)
				{
					if (unk_D47E0[4 * i + 3] >= 0)
					{
						actBufPos = &pdwScreenBuffer_351628[25 * (49 * unk_D47E0[4 * i + 1] + 7 * unk_D47E0[4 * i + 2] + unk_D47E0[4 * i + 3] + 343 * unk_D47E0[4 * i + 0])];
						if (*actBufPos < 12)
						{
							actBufPos[*actBufPos + 13] = 0;
							actBufPos[*actBufPos + 1] = i;
							(*actBufPos)++;
						}
						actBufPos = &pdwScreenBuffer_351628[25 * (49 * unk_D47E0[4 * i + 0] + unk_D47E0[4 * i + 2] + 7 * unk_D47E0[4 * i + 3] + 343 * unk_D47E0[4 * i + 1])];
						if (*actBufPos < 12)
						{							
							actBufPos[*actBufPos + 13] = 16;
							actBufPos[*actBufPos + 1] = i;
							(*actBufPos)++;
						}
						actBufPos = &pdwScreenBuffer_351628[25 * (49 * unk_D47E0[4 * i + 3] + unk_D47E0[4 * i + 1] + 7 * unk_D47E0[4 * i + 0] + 343 * unk_D47E0[4 * i + 2])];
						if (*actBufPos < 12)
						{
							actBufPos[*actBufPos + 13] = 48;
							actBufPos[*actBufPos + 1] = i;
							(*actBufPos)++;
						}
						actBufPos = &pdwScreenBuffer_351628[25 * (49 * unk_D47E0[4 * i + 2] + unk_D47E0[4 * i + 0] + 7 * unk_D47E0[4 * i + 1] + 343 * unk_D47E0[4 * i + 3])];
						if (*actBufPos < 12)
						{
							actBufPos[*actBufPos + 13] = 32;
							actBufPos[*actBufPos + 1] = i;
							(*actBufPos)++;
						}
						actBufPos = &pdwScreenBuffer_351628[25 * (49 * unk_D47E0[4 * i + 2] + 8 * unk_D47E0[4 * i + 3] - unk_D47E0[4 * i + 3] + unk_D47E0[4 * i + 0] + 343 * unk_D47E0[4 * i + 1])];
						if (*actBufPos < 12)
						{
							actBufPos[*actBufPos + 13] = 96;
							actBufPos[*actBufPos + 1] = i;
							(*actBufPos)++;
						}
						actBufPos = &pdwScreenBuffer_351628[25 * (49 * unk_D47E0[4 * i + 1] + 8 * unk_D47E0[4 * i + 0] - unk_D47E0[4 * i + 0] + unk_D47E0[4 * i + 3] + 343 * unk_D47E0[4 * i + 2])];
						if (*actBufPos < 12)
						{
							actBufPos[*actBufPos + 13] = 112;
							actBufPos[*actBufPos + 1] = i;
							(*actBufPos)++;
						}
						actBufPos = &pdwScreenBuffer_351628[25 * (49 * unk_D47E0[4 * i + 0] + 7 * unk_D47E0[4 * i + 1] + unk_D47E0[4 * i + 2] + 343 * unk_D47E0[4 * i + 3])];
						if (*actBufPos < 12)
						{
							actBufPos[*actBufPos + 13] = 80;
							actBufPos[*actBufPos + 1] = i;
							(*actBufPos)++;
						}
						actBufPos = &pdwScreenBuffer_351628[25 * (343 * unk_D47E0[4 * i + 0] + 7 * unk_D47E0[4 * i + 2] + unk_D47E0[4 * i + 1] + 49 * unk_D47E0[4 * i + 3])];
						if (*actBufPos < 12)
						{
							actBufPos[*actBufPos + 13] = 64;
							actBufPos[*actBufPos + 1] = i;
							(*actBufPos)++;
						}
					}
				}
			}
		}
	}
	int index = 0;
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			for (int k = 0; k < 7; k++)
			{
				for (int l = 0; l < 7; l++)
				{
					actBufPos = &pdwScreenBuffer_351628[25 * (49 * j + 7 * k + l + 343 * i)];
					if (*actBufPos)
					{
						building_F2CD0x[index][0] = actBufPos[1];
						building_F2CD0x[index][1] = actBufPos[13];
					}
					else
					{
						building_F2CD0x[index][0] = 1;
						building_F2CD0x[index][1] = 0;
					}
					index++;
				}
			}
		}
	}
	//adress 225bbd

	//  X-X
	//  | |
	//  B-X

	uaxis_2d uindex;
	for (int i = 0; i < 256 * 256; i++)
	{
		uindex.word = i;
		if (!mapTerrainType_10B4E0[uindex.word])
		{
			point1 = mapAngle_13B4E0[uindex.word] & 7;
			uindex._axis_2d.x++;
			point2 = mapAngle_13B4E0[uindex.word] & 7;
			uindex._axis_2d.y++;
			point3 = mapAngle_13B4E0[uindex.word] & 7;
			uindex._axis_2d.x--;
			point4 = mapAngle_13B4E0[uindex.word] & 7;
			uindex._axis_2d.y--;
			actBufEnt = pdwScreenBuffer_351628[25 * (343 * point1 + 49 * point2 + point4 + 7 * point3)];
			if (actBufEnt)
			{
				rand2_17B4E0 = 9377 * rand2_17B4E0 + 9439;
				if (rand2_17B4E0 % (actBufEnt + 1) >= actBufEnt)
					actBufPos = &pdwScreenBuffer_351628[25 * (343 * point1 + 49 * point2 + point4 + 7 * point3)];
				else
					actBufPos = &pdwScreenBuffer_351628[rand2_17B4E0 % (actBufEnt + 1) + 25 * (343 * point1 + 49 * point2 + point4 + 7 * point3)];
				mapTerrainType_10B4E0[uindex.word] = actBufPos[1];
				mapAngle_13B4E0[uindex.word] = (mapAngle_13B4E0[uindex.word] & 7) + actBufPos[13];
			}
			else
			{
				mapTerrainType_10B4E0[uindex.word] = 1;
			}
		}
	}
}

//----- (00043B40) --------------------------------------------------------
void GenerateCaveCeiling_43B40()//224b40
{
	uint8_t locHeight;
	uaxis_2d index;
	for (int i = 0; i < 256 * 256; i++)
	{
		index.word = i;
		locHeight = mapHeightmap_11B4E0[index.word];
		if (locHeight > MapBasicHeight_D41B7)
			locHeight = MapBasicHeight_D41B7;
		secondHeightmap_14B4E0[index.word] = MapBasicHeight_D41B7 - locHeight;
		if (MapBasicHeight_D41B7 - locHeight > mapHeightmap_11B4E0[index.word])
		{
			mapAngle_13B4E0[index.word] &= 0xF7u;
		}
		else
		{
			secondHeightmap_14B4E0[index.word] = mapHeightmap_11B4E0[index.word] - 1;
			mapAngle_13B4E0[index.word] |= 8;
		}
	}
	RandomiseCeilingHeight_43BB0();
}

//----- (00043D50) --------------------------------------------------------
void MarkWaterEdges_43D50()//224d50
{

	//  X-X-X
	//  |   |
	//  X B X
	//  |/| |
	//  X X-X

	uint8_t point1;
	uint8_t point2;
	uaxis_2d index;
	for (int i = 0; i < 256 * 256; i++)
	{
		index.word = i;
		mapAngle_13B4E0[index.word] &= 0xF7u;
		if (!mapHeightmap_11B4E0[index.word])
		{
			index._axis_2d.y--;
			point1 = (mapHeightmap_11B4E0[index.word] != 0);
			index._axis_2d.x++;
			point1 += (mapHeightmap_11B4E0[index.word] != 0);
			index._axis_2d.y++;
			point1 += (mapHeightmap_11B4E0[index.word] != 0);
			index._axis_2d.y++;
			point1 += (mapHeightmap_11B4E0[index.word] != 0);
			index._axis_2d.x--;
			point1 += (mapHeightmap_11B4E0[index.word] != 0);
			index._axis_2d.x--;
			point1 += (mapHeightmap_11B4E0[index.word] != 0);
			index._axis_2d.y--;
			point1 += (mapHeightmap_11B4E0[index.word] != 0);
			index._axis_2d.y--;
			point2 = mapHeightmap_11B4E0[index.word] != 0;
			index._axis_2d.x++;
			index._axis_2d.y++;

			//  X-X
			//  | |
			//  B-X

			if (!(point2 + point1))
			{
				point1 = (mapTerrainType_10B4E0[index.word] != 0);
				index._axis_2d.x--;
				point1 += (mapTerrainType_10B4E0[index.word] != 0);
				index._axis_2d.y--;
				point1 += (mapTerrainType_10B4E0[index.word] != 0);
				index._axis_2d.x++;
				point1 += (mapTerrainType_10B4E0[index.word] != 0);
				index._axis_2d.y++;
				if (!point1)
					mapAngle_13B4E0[index.word] |= 8u;
			}
		}
	}
}

//----- (00044D00) --------------------------------------------------------
void ComputeTerrainShading_44D00()//225d00
{

	//     X
	//    /
	//   B
	//  /
	// X

	uaxis_2d tempIndex;
	uaxis_2d index;
	rand2_17B4E0 = 0;
	for (int i = 0; i < 256 * 256; i++)
	{
		index.word = i;
		index._axis_2d.x++;
		index._axis_2d.y++;
		tempIndex.word = index.word;
		index._axis_2d.x -= 2;
		index._axis_2d.y -= 2;
		tempIndex._axis_2d.x = mapHeightmap_11B4E0[index.word] - mapHeightmap_11B4E0[tempIndex.word] + 32;
		index._axis_2d.x++;
		index._axis_2d.y++;
		if (tempIndex._axis_2d.x == 32)
		{
			tempIndex.word = 9377 * rand2_17B4E0 + 9439;
			rand2_17B4E0 = tempIndex.word;
			tempIndex._axis_2d.y = (rand2_17B4E0 / 9u) >> 8;
			tempIndex._axis_2d.x = rand2_17B4E0 % 9 + 28;
		}
		else if ((int8_t)tempIndex._axis_2d.x >= 28)
		{
			if ((int8_t)tempIndex._axis_2d.x > 40)
				tempIndex._axis_2d.x = (tempIndex._axis_2d.x & 7) + 40;
		}
		else
		{
			tempIndex._axis_2d.x = (tempIndex._axis_2d.x & 3) + 28;
		}
		if (world_D41A0.terrain_2FECE.MapType != MapType_t::Day)
		{
			mapShading_12B4E0[index.word] = (64 - tempIndex._axis_2d.x);
		}
		else
		{
			mapShading_12B4E0[index.word] = tempIndex._axis_2d.x;
		}
	}
}

//----- (000B5EFA) --------------------------------------------------------
void TerrainDiamondStep_B5EFA(__int16 a1, uaxis_2d* indexx, int32_t a3, int16_t* nextRand)//296EFA
{
	//  X-.-X
	//   \  |
	//    E .
	//      |
	//  B---X

	int16_t sumEnt;
	uint16_t srandNumber;

	sumEnt = mapEntityIndex_15B4E0[indexx->word];
	indexx->_axis_2d.x += 2 * a1;
	sumEnt += mapEntityIndex_15B4E0[indexx->word];
	indexx->_axis_2d.y += 2 * a1;
	sumEnt += mapEntityIndex_15B4E0[indexx->word];
	indexx->_axis_2d.x -= 2 * a1;
	sumEnt += mapEntityIndex_15B4E0[indexx->word];
	indexx->_axis_2d.x += a1;
	indexx->_axis_2d.y -= a1;
	srandNumber = 9377 * *nextRand + 9439;
	*nextRand = srandNumber;
	if (!mapEntityIndex_15B4E0[indexx->word])
		mapEntityIndex_15B4E0[indexx->word] = srandNumber % (uint16_t)(2 * a3 + 1)
		+ srandNumber % (uint16_t)((a1 << 6) + 1) + (sumEnt >> 2) - 32 * a1 - a3;
	indexx->_axis_2d.x += a1;
	indexx->_axis_2d.y -= a1;
}

//----- (000B5F8F) --------------------------------------------------------
void TerrainSquareStep_B5F8F(__int16 a1, uaxis_2d* indexx, int32_t a3, int16_t* nextRand)//296f8f
{

	//   X
	//   |\
	// B E X
	//  \ /
	//   X

	int16_t sumEnt;
	int16_t sumEnt2;
	uint16_t srandNumber;

	sumEnt = mapEntityIndex_15B4E0[indexx->word];
	sumEnt2 = sumEnt;
	indexx->_axis_2d.x += a1;
	indexx->_axis_2d.y -= a1;
	sumEnt += mapEntityIndex_15B4E0[indexx->word];
	indexx->_axis_2d.x += a1;
	indexx->_axis_2d.y += a1;
	sumEnt += mapEntityIndex_15B4E0[indexx->word];
	indexx->_axis_2d.x -= a1;
	indexx->_axis_2d.y += a1;
	sumEnt += mapEntityIndex_15B4E0[indexx->word];
	srandNumber = 9377 * *nextRand + 9439;
	sumEnt2 += mapEntityIndex_15B4E0[indexx->word];
	indexx->_axis_2d.y -= a1;
	if (!mapEntityIndex_15B4E0[indexx->word])
		mapEntityIndex_15B4E0[indexx->word] = srandNumber % (uint16_t)(2 * a3 + 1)
		+ srandNumber % (uint16_t)((a1 << 6) + 1) + (uint16_t)(sumEnt >> 2) - 32 * a1 - a3;
	
	//   X
	//  /|
	// X E-.
	//  \   \
	//   .-B R

	indexx->_axis_2d.x -= 2 * a1;
	indexx->_axis_2d.y += a1;
	sumEnt2 += mapEntityIndex_15B4E0[indexx->word];
	indexx->_axis_2d.x += a1;
	indexx->_axis_2d.y += a1;
	sumEnt2 += mapEntityIndex_15B4E0[indexx->word];
	indexx->_axis_2d.y -= a1;
	srandNumber = 9377 * srandNumber + 9439;
	*nextRand = srandNumber;
	if (!mapEntityIndex_15B4E0[indexx->word])
		mapEntityIndex_15B4E0[indexx->word] = srandNumber % (uint16_t)(2 * a3 + 1)
		+ srandNumber % (uint16_t)((a1 << 6) + 1) + (uint16_t)(sumEnt2 >> 2) - 32 * a1 - a3;
	indexx->_axis_2d.x += 2 * a1;
	indexx->_axis_2d.y -= a1;
}

//----- (00044EE0) --------------------------------------------------------
void SmoothTiles_44EE0(uaxis_2d axis)//225ee0
{
	//  X-X-X
	//  |   |
	//  X B X
	//  | | |
	//  X X-X

	uaxis_2d tempAxis2;
	uaxis_2d tempAxis1;
	uint8_t centralHeight;
	uint8_t minHeight;

	tempAxis1.word = axis.word;
	for (int i = 0; i < 256 * 256; i++)
		mapTerrainType_10B4E0[i] = 3;
	centralHeight = mapHeightmap_11B4E0[axis.word];
	do
	{
		mapTerrainType_10B4E0[tempAxis1.word] = 0;
		tempAxis1._axis_2d.y--;
		minHeight = 255;
		if (mapTerrainType_10B4E0[tempAxis1.word] && mapHeightmap_11B4E0[tempAxis1.word] < 255)
		{
			minHeight = mapHeightmap_11B4E0[tempAxis1.word];
			tempAxis2.word = tempAxis1.word;
		}
		tempAxis1._axis_2d.x++;
		if (mapTerrainType_10B4E0[tempAxis1.word] && minHeight > mapHeightmap_11B4E0[tempAxis1.word])
		{
			minHeight = mapHeightmap_11B4E0[tempAxis1.word];
			tempAxis2.word = tempAxis1.word;
		}
		tempAxis1._axis_2d.y++;
		if (mapTerrainType_10B4E0[tempAxis1.word] && minHeight > mapHeightmap_11B4E0[tempAxis1.word])
		{
			minHeight = mapHeightmap_11B4E0[tempAxis1.word];
			tempAxis2.word = tempAxis1.word;
		}
		tempAxis1._axis_2d.y++;
		if (mapTerrainType_10B4E0[tempAxis1.word] && minHeight > mapHeightmap_11B4E0[tempAxis1.word])
		{
			minHeight = mapHeightmap_11B4E0[tempAxis1.word];
			tempAxis2.word = tempAxis1.word;
		}
		tempAxis1._axis_2d.x--;
		if (mapTerrainType_10B4E0[tempAxis1.word] && minHeight > mapHeightmap_11B4E0[tempAxis1.word])
		{
			minHeight = mapHeightmap_11B4E0[tempAxis1.word];
			tempAxis2.word = tempAxis1.word;
		}
		tempAxis1._axis_2d.x--;
		if (mapTerrainType_10B4E0[tempAxis1.word] && minHeight > mapHeightmap_11B4E0[tempAxis1.word])
		{
			minHeight = mapHeightmap_11B4E0[tempAxis1.word];
			tempAxis2.word = tempAxis1.word;
		}
		tempAxis1._axis_2d.y--;
		if (mapTerrainType_10B4E0[tempAxis1.word] && minHeight > mapHeightmap_11B4E0[tempAxis1.word])
		{
			minHeight = mapHeightmap_11B4E0[tempAxis1.word];
			tempAxis2.word = tempAxis1.word;
		}
		tempAxis1._axis_2d.y--;
		if (mapTerrainType_10B4E0[tempAxis1.word] && minHeight > mapHeightmap_11B4E0[tempAxis1.word])
		{
			minHeight = mapHeightmap_11B4E0[tempAxis1.word];
			tempAxis2.word = tempAxis1.word;
		}
		if (!mapAngle_13B4E0[tempAxis2.word] || minHeight == 255)
			break;
		if (minHeight > centralHeight)//if near tile is higger then central tile set central as near tile
			mapHeightmap_11B4E0[tempAxis2.word] = centralHeight;
		centralHeight = mapHeightmap_11B4E0[tempAxis2.word];
		tempAxis1.word = tempAxis2.word;
	} while (centralHeight);
	for (int i = 0; i < 256 * 256; i++)
		if (!mapTerrainType_10B4E0[i])
			mapAngle_13B4E0[i] = 0;
}

//----- (000439A0) --------------------------------------------------------
unsigned int GetSmoothedHeight_439A0(uint16_t index)//2249a0
{
	//    X
	//    |
	//  X-B-X
	//    |
	//    X

	uint8_t maxHeight;
	uint8_t minHeight;
	uint8_t centerPoint;
	unsigned int modSumaPoint;
	int sumaPoint = 0;
	unsigned int result = mapHeightmap_11B4E0[index];
	uaxis_2d uindex;
	uindex.word = index;
	if (mapAngle_13B4E0[uindex.word] & 7)
	{
		maxHeight = mapHeightmap_11B4E0[uindex.word];
		minHeight = maxHeight;
		centerPoint = mapHeightmap_11B4E0[uindex.word];
		uindex._axis_2d.y--;
		sumaPoint = mapHeightmap_11B4E0[uindex.word];
		if (mapHeightmap_11B4E0[uindex.word] > maxHeight)
			maxHeight = mapHeightmap_11B4E0[uindex.word];
		if (mapHeightmap_11B4E0[uindex.word] < minHeight)
			minHeight = mapHeightmap_11B4E0[uindex.word];
		uindex._axis_2d.x++;
		sumaPoint += mapHeightmap_11B4E0[uindex.word];
		if (mapHeightmap_11B4E0[uindex.word] > maxHeight)
			maxHeight = mapHeightmap_11B4E0[uindex.word];
		if (mapHeightmap_11B4E0[uindex.word] < minHeight)
			minHeight = mapHeightmap_11B4E0[uindex.word];
		uindex._axis_2d.y++;
		sumaPoint += mapHeightmap_11B4E0[uindex.word];
		if (mapHeightmap_11B4E0[uindex.word] > maxHeight)
			maxHeight = mapHeightmap_11B4E0[uindex.word];
		if (mapHeightmap_11B4E0[uindex.word] < minHeight)
			minHeight = mapHeightmap_11B4E0[uindex.word];
		uindex._axis_2d.y++;
		sumaPoint += mapHeightmap_11B4E0[uindex.word];
		if (mapHeightmap_11B4E0[uindex.word] > maxHeight)
			maxHeight = mapHeightmap_11B4E0[uindex.word];
		if (mapHeightmap_11B4E0[uindex.word] < minHeight)
			minHeight = mapHeightmap_11B4E0[uindex.word];
		uindex._axis_2d.x--;
		sumaPoint += mapHeightmap_11B4E0[uindex.word];
		if (mapHeightmap_11B4E0[uindex.word] > maxHeight)
			maxHeight = mapHeightmap_11B4E0[uindex.word];
		if (mapHeightmap_11B4E0[uindex.word] < minHeight)
			minHeight = mapHeightmap_11B4E0[uindex.word];
		uindex._axis_2d.x--;
		sumaPoint += mapHeightmap_11B4E0[uindex.word];
		if (mapHeightmap_11B4E0[uindex.word] > maxHeight)
			maxHeight = mapHeightmap_11B4E0[uindex.word];
		if (mapHeightmap_11B4E0[uindex.word] < minHeight)
			minHeight = mapHeightmap_11B4E0[uindex.word];
		uindex._axis_2d.y--;
		sumaPoint += mapHeightmap_11B4E0[uindex.word];
		if (mapHeightmap_11B4E0[uindex.word] > maxHeight)
			maxHeight = mapHeightmap_11B4E0[uindex.word];
		if (mapHeightmap_11B4E0[uindex.word] < minHeight)
			minHeight = mapHeightmap_11B4E0[uindex.word];
		uindex._axis_2d.y--;
		sumaPoint += mapHeightmap_11B4E0[uindex.word];
		if (mapHeightmap_11B4E0[uindex.word] > maxHeight)
			maxHeight = mapHeightmap_11B4E0[uindex.word];
		if (mapHeightmap_11B4E0[uindex.word] < minHeight)
			minHeight = mapHeightmap_11B4E0[uindex.word];
		modSumaPoint = sumaPoint >> 3;
		if ((uint8_t)(centerPoint - minHeight) <= 4)
		{
			if ((uint8_t)(maxHeight - centerPoint) <= 4)
				return result;
			if ((uint8_t)(maxHeight - centerPoint) <= 10)
				modSumaPoint = (centerPoint + modSumaPoint) >> 1;
		}
		else if ((uint8_t)(centerPoint - minHeight) <= 10)
		{
			return (modSumaPoint + centerPoint) >> 1;
		}
		result = modSumaPoint;
	}
	return result;
}

//----- (00043BB0) --------------------------------------------------------
void RandomiseCeilingHeight_43BB0()//224bb0
{
	int fuzzyHeight;
	unsigned int randSeed = 37487429;
	for (int i = 0; i < 256 * 256; i++)
	{
		if (!(mapAngle_13B4E0[i] & 8))
		{
			randSeed = 9377 * randSeed + 9439;
			fuzzyHeight = randSeed % 7 - 3 + secondHeightmap_14B4E0[i];
			if (fuzzyHeight < 0)
				fuzzyHeight = 0;
			if (fuzzyHeight > 254)
				fuzzyHeight = 254;
			secondHeightmap_14B4E0[i] = fuzzyHeight;
		}
	}
	for (int i = 0; i < 256 * 256; i++)
	{
		if (secondHeightmap_14B4E0[i] > mapHeightmap_11B4E0[i])
		{
			mapAngle_13B4E0[i] &= 0xF7u;
		}
		else
		{
			secondHeightmap_14B4E0[i] = mapHeightmap_11B4E0[i] - 1;
			mapAngle_13B4E0[i] |= 8;
		}
	}
}

//----- (0001B7A0) --------------------------------------------------------
int GetTileSlope_1B7A0(axis_3d* a1)//1fc7a0
{

	//  X-X
	//    |
	//  B-X

	uaxis_2d uindex;
	uindex._axis_2d.x = a1->x >> 8;
	uindex._axis_2d.y = a1->y >> 8;
	uint8_t point1 = mapHeightmap_11B4E0[uindex.word];
	uindex._axis_2d.x++;
	uint8_t point2 = mapHeightmap_11B4E0[uindex.word];
	uindex._axis_2d.y++;
	uint8_t point3 = mapHeightmap_11B4E0[uindex.word];
	uindex._axis_2d.x--;
	uint8_t point4 = mapHeightmap_11B4E0[uindex.word];
	int result1 = abs(point1 + point4 - point2 - point3);
	int result2 = abs(point1 + point2 - point4 - point3);
	if (result1 > result2)
		return result1;
	return result2;
}

//----- (0001B830) --------------------------------------------------------
int GetTileSlopeWide_1B830(axis_3d* a1)//1fc830
{

	//  X-.-X
	//      |
	//      .
	//      |
	//  B-.-X

	uaxis_2d uindex;
	uindex._axis_2d.x = (a1->x >> 8) - 1;
	uindex._axis_2d.y = (a1->y >> 8) - 1;
	uint8_t point1 = mapHeightmap_11B4E0[uindex.word];
	uindex._axis_2d.x += 2;
	uint8_t point2 = mapHeightmap_11B4E0[uindex.word];
	uindex._axis_2d.y += 2;
	uint8_t point3 = mapHeightmap_11B4E0[uindex.word];
	uindex._axis_2d.x -= 2;//0x1FC86C sub al, 2
	uint8_t point4 = mapHeightmap_11B4E0[uindex.word];
	int result1 = abs(point1 + point4 - point2 - point3);
	int result2 = abs(point1 + point2 - point4 - point3);
	if (result1 > result2)
		return result1;
	return result2;
}

//----- (00045BE0) --------------------------------------------------------
uint8_t GetTileHeightType_45BE0(uint8_t inType, uaxis_2d axis2d, uint8_t type)//226be0 type: dl, inType: cl of the caller
{
	//  X-X
	//  | |
	//  B-X

	uint8_t result;
	uint8_t minHeight = 255;
	uint8_t maxHeight = 0;
	if (mapHeightmap_11B4E0[axis2d.word])
	{
		maxHeight = mapHeightmap_11B4E0[axis2d.word];
		type = 0;
	}
	if (mapHeightmap_11B4E0[axis2d.word] < 255)
		minHeight = mapHeightmap_11B4E0[axis2d.word];
	axis2d._axis_2d.x++;
	if (mapHeightmap_11B4E0[axis2d.word] > maxHeight)
	{
		maxHeight = mapHeightmap_11B4E0[axis2d.word];
		type = 1;
	}
	if (mapHeightmap_11B4E0[axis2d.word] < minHeight)
		minHeight = mapHeightmap_11B4E0[axis2d.word];
	axis2d._axis_2d.y++;
	if (mapHeightmap_11B4E0[axis2d.word] > maxHeight)
	{
		maxHeight = mapHeightmap_11B4E0[axis2d.word];
		type = 2;
	}
	if (mapHeightmap_11B4E0[axis2d.word] < minHeight)
		minHeight = mapHeightmap_11B4E0[axis2d.word];
	axis2d._axis_2d.x--;
	if (mapHeightmap_11B4E0[axis2d.word] > maxHeight)
	{
		maxHeight = mapHeightmap_11B4E0[axis2d.word];
		type = 3;
	}
	if (mapHeightmap_11B4E0[axis2d.word] < minHeight)
		minHeight = mapHeightmap_11B4E0[axis2d.word];
	axis2d._axis_2d.y--;
	uint8_t maxHeight2 = 0;
	if (type && mapHeightmap_11B4E0[axis2d.word])
	{
		maxHeight2 = mapHeightmap_11B4E0[axis2d.word];
		inType = 0;
	}
	axis2d._axis_2d.x++;
	if (type != 1 && mapHeightmap_11B4E0[axis2d.word] > maxHeight2)
	{
		maxHeight2 = mapHeightmap_11B4E0[axis2d.word];
		inType = 1;
	}
	axis2d._axis_2d.y++;
	if (type != 2 && mapHeightmap_11B4E0[axis2d.word] > maxHeight2)
	{
		maxHeight2 = mapHeightmap_11B4E0[axis2d.word];
		inType = 2;
	}
	axis2d._axis_2d.x--;
	if (type != 3)
	{
		uint8_t minHeight2 = mapHeightmap_11B4E0[axis2d.word];
		if (minHeight2 > maxHeight2)
		{
			inType = 3;
			maxHeight2 = minHeight2;
		}
	}
	bool lowDiff = false;
	if (maxHeight - minHeight <= 8)
		lowDiff = true;
	if (maxHeight - maxHeight2 >= 8)
	{
		result = type;
		lowDiffHeightmap_D47DC = lowDiff;
		return result;
	}
	switch (type)
	{
	case 0:
		if (inType != 1)
			result = 7;
		else
			result = 4;
		break;
	case 1:
		if (inType == 2)
			result = 5;
		else
			result = 4;
		break;
	case 2:
		if (inType == 3)
			result = 6;
		else
			result = 5;
		break;
	case 3:
		if (inType)
			result = 6;
		else
			result = 7;
		break;
	default:
		result = 0;
		break;
	}
	lowDiffHeightmap_D47DC = lowDiff;
	return result;
}

//----- (00033F70) --------------------------------------------------------
bool CheckTileType8Neighbours_33F70(uint16_t inAxis)//214f70
{
	//    X
	//    |
	//  X-B-X
	//    |
	//    X

	int compHeight;
	bool result;
	uaxis_2d axis2d;
	uaxis_2d tempAxis2d;
	axis2d.word = inAxis;
	tempAxis2d.word = axis2d.word;
	tempAxis2d._axis_2d.x--;//x-1,y
	result = 1;
	if (mapTerrainType_10B4E0[tempAxis2d.word] == 8)
	{
		compHeight = mapHeightmap_11B4E0[axis2d.word] + 30;
		if (mapHeightmap_11B4E0[tempAxis2d.word] <= compHeight)
		{
			tempAxis2d._axis_2d.x += 2;//x+1,y
			if (mapHeightmap_11B4E0[axis2d.word] <= compHeight)
			{
				tempAxis2d._axis_2d.x--;
				tempAxis2d._axis_2d.y++;//x,y+1
				if (mapHeightmap_11B4E0[tempAxis2d.word] <= compHeight)
				{
					tempAxis2d._axis_2d.y -= 2;//x,y-1
					if (mapHeightmap_11B4E0[tempAxis2d.word] <= compHeight)
						result = 0;
				}
			}
		}
	}
	return result;
}

//----- (00045DC0) --------------------------------------------------------
void SetTileTerrainType_45DC0(uint8_t inType, uaxis_2d axis2d, uint8_t type2, uint8_t type)//226dc0
{
	char heightType;
	if (type2 >= 8u)
	{
		switch (type2)
		{
		case 8u:
			mapTerrainType_10B4E0[axis2d.word] = 8;
			break;
		case 9u:
			mapTerrainType_10B4E0[axis2d.word] = 9;
			break;
		case 0xAu:
			heightType = GetTileHeightType_45BE0(inType, axis2d, type);
			if (lowDiffHeightmap_D47DC)
				heightType += 8;
			mapTerrainType_10B4E0[axis2d.word] = unk_D4A30[0x80 + 2 * heightType];
			mapAngle_13B4E0[axis2d.word] = mapAngle_13B4E0[axis2d.word] & 0x8F | unk_D4A30[0x80 + 2 * heightType + 1];
			break;
		case 0xBu:
			heightType = GetTileHeightType_45BE0(inType, axis2d, type) + 0x10;
			if (lowDiffHeightmap_D47DC)
				heightType += 8;
			mapTerrainType_10B4E0[axis2d.word] = unk_D4A30[0x80 + 2 * heightType];
			mapAngle_13B4E0[axis2d.word] = mapAngle_13B4E0[axis2d.word] & 0x8F | unk_D4A30[0x80 + 2 * heightType + 1];
			break;
		case 0xCu:
			heightType = GetTileHeightType_45BE0(inType, axis2d, type) + 0x20;
			if (lowDiffHeightmap_D47DC)
				heightType += 8;
			mapTerrainType_10B4E0[axis2d.word] = unk_D4A30[0x80 + 2 * heightType];
			mapAngle_13B4E0[axis2d.word] = mapAngle_13B4E0[axis2d.word] & 0x8F | unk_D4A30[0x80 + 2 * heightType + 1];
			break;
		case 0xDu:
			heightType = GetTileHeightType_45BE0(inType, axis2d, type) + 0x30;
			if (lowDiffHeightmap_D47DC)
				heightType += 8;
			mapTerrainType_10B4E0[axis2d.word] = unk_D4A30[0x80 + 2 * heightType];
			mapAngle_13B4E0[axis2d.word] = mapAngle_13B4E0[axis2d.word] & 0x8F | unk_D4A30[0x80 + 2 * heightType + 1];
			break;
		case 0xEu:
			heightType = GetTileHeightType_45BE0(inType, axis2d, type) + 0x40;
			if (lowDiffHeightmap_D47DC)
				heightType += 8;
			mapTerrainType_10B4E0[axis2d.word] = unk_D4A30[0x80 + 2 * heightType];
			mapAngle_13B4E0[axis2d.word] = mapAngle_13B4E0[axis2d.word] & 0x8F | unk_D4A30[0x80 + 2 * heightType + 1];
			break;
		case 0xFu:
			mapTerrainType_10B4E0[axis2d.word] = 11;
			break;
		case 0x10u:
			if (mapTerrainType_10B4E0[axis2d.word] == 10 || mapTerrainType_10B4E0[axis2d.word] == 11 || mapTerrainType_10B4E0[axis2d.word] == 12)
				break;
			heightType = GetTileHeightType_45BE0(inType, axis2d, type);
			mapTerrainType_10B4E0[axis2d.word] = unk_D4A30[2 * heightType];
			mapAngle_13B4E0[axis2d.word] = mapAngle_13B4E0[axis2d.word] & 0x8F | unk_D4A30[2 * heightType + 1];
			break;
		case 0x11u:
			heightType = GetTileHeightType_45BE0(inType, axis2d, type);
			mapTerrainType_10B4E0[axis2d.word] = unk_D4A30[0x40 + 2 * heightType];
			mapAngle_13B4E0[axis2d.word] = mapAngle_13B4E0[axis2d.word] & 0x8F | unk_D4A30[0x40 + 2 * heightType + 1];
			break;
		case 0x12u:
			heightType = (8 * ((axis2d._axis_2d.y + axis2d._axis_2d.x) & 1) + GetTileHeightType_45BE0(inType, axis2d, type));
			mapTerrainType_10B4E0[axis2d.word] = unk_D4A30[0x50 + 2 * heightType];
			mapAngle_13B4E0[axis2d.word] = mapAngle_13B4E0[axis2d.word] & 0x8F | unk_D4A30[0x50 + 2 * heightType + 1];
			break;
		case 0x13u:
			heightType = (8 * ((axis2d._axis_2d.y + axis2d._axis_2d.x) & 1) + GetTileHeightType_45BE0(inType, axis2d, type));
			mapTerrainType_10B4E0[axis2d.word] = unk_D4A30[0x60 + 2 * heightType];
			mapAngle_13B4E0[axis2d.word] = mapAngle_13B4E0[axis2d.word] & 0x8F | unk_D4A30[0x60 + 2 * heightType + 1];
			break;
		case 0x14u:
			heightType = GetTileHeightType_45BE0(inType, axis2d, type);
			mapTerrainType_10B4E0[axis2d.word] = unk_D4A30[0x10 + 2 * heightType];
			mapAngle_13B4E0[axis2d.word] = mapAngle_13B4E0[axis2d.word] & 0x8F | unk_D4A30[0x10 + 2 * heightType + 1];
			break;
		case 0x15u:
			heightType = GetTileHeightType_45BE0(inType, axis2d, type);
			mapTerrainType_10B4E0[axis2d.word] = unk_D4A30[0x20 + 2 * heightType];
			mapAngle_13B4E0[axis2d.word] = mapAngle_13B4E0[axis2d.word] & 0x8F | unk_D4A30[0x20 + 2 * heightType + 1];
			break;
		case 0x16u:
			heightType = GetTileHeightType_45BE0(inType, axis2d, type);
			mapTerrainType_10B4E0[axis2d.word] = unk_D4A30[0x30 + 2 * heightType];
			mapAngle_13B4E0[axis2d.word] = mapAngle_13B4E0[axis2d.word] & 0x8F | unk_D4A30[0x30 + 2 * heightType + 1];
			break;
		default:
			break;
		}
		mapAngle_13B4E0[axis2d.word] |= 0x80;
		if (isCaveLevel_D41B6)
		{
			if (secondHeightmap_14B4E0[axis2d.word] > mapHeightmap_11B4E0[axis2d.word])
			{
				mapAngle_13B4E0[axis2d.word] &= 0xF7;
			}
			else
			{
				mapAngle_13B4E0[axis2d.word] |= 8;
				secondHeightmap_14B4E0[axis2d.word] = mapHeightmap_11B4E0[axis2d.word] - 1;
			}
			axis2d._axis_2d.x++;
			if (secondHeightmap_14B4E0[axis2d.word] > mapHeightmap_11B4E0[axis2d.word])
			{
				mapAngle_13B4E0[axis2d.word] &= 0xF7u;
			}
			else
			{
				secondHeightmap_14B4E0[axis2d.word] = mapHeightmap_11B4E0[axis2d.word] - 1;
				mapAngle_13B4E0[axis2d.word] |= 8;
			}
			axis2d._axis_2d.y++;
			if (secondHeightmap_14B4E0[axis2d.word] > mapHeightmap_11B4E0[axis2d.word])
			{
				mapAngle_13B4E0[axis2d.word] &= 0xF7u;
			}
			else
			{
				secondHeightmap_14B4E0[axis2d.word] = mapHeightmap_11B4E0[axis2d.word] - 1;
				mapAngle_13B4E0[axis2d.word] |= 8;
			}
			axis2d._axis_2d.x--;
			if (secondHeightmap_14B4E0[axis2d.word] <= mapHeightmap_11B4E0[axis2d.word])
			{
				secondHeightmap_14B4E0[axis2d.word] = mapHeightmap_11B4E0[axis2d.word] - 1;
				mapAngle_13B4E0[axis2d.word] |= 8;
				return;
			}
		}
		else
		{
			mapAngle_13B4E0[axis2d.word] &= 0xF7;
			axis2d._axis_2d.x++;
			mapAngle_13B4E0[axis2d.word] &= 0xF7;
			axis2d._axis_2d.y++;
			mapAngle_13B4E0[axis2d.word] &= 0xF7;
			axis2d._axis_2d.x--;
		}
		mapAngle_13B4E0[axis2d.word] &= 0xF7u;
		return;
	}
	mapAngle_13B4E0[axis2d.word] = type2 | mapAngle_13B4E0[axis2d.word] & 0xF0;
	UpdateTerrainTiles_462A0(axis2d, axis2d);
}

//----- (000462A0) --------------------------------------------------------
void UpdateTerrainTiles_462A0(uaxis_2d inAxis2dA, uaxis_2d inAxis2dB)//2272a0
{
	uaxis_2d tempAxis;
	uint8_t nextAngle;
	int terModIndex;
	char tempShad;
	uint8_t yAdd;
	uint8_t xAdd;
	uint8_t point1;
	uint8_t point2;
	uint8_t point3;
	uint8_t point4;

	tempAxis.word = inAxis2dA.word;
	for (uint8_t indexY = inAxis2dB._axis_2d.y - inAxis2dA._axis_2d.y + 1; indexY != 0; indexY--)//2272B9 inc ah ... dec ah, jnz: byte counters, the area can wrap over the map edge
	{
		for (uint8_t indexX = inAxis2dB._axis_2d.x - inAxis2dA._axis_2d.x + 1; indexX != 0; indexX--)
		{
			if ((int8_t)mapAngle_13B4E0[tempAxis.word] >= 0)
				mapTerrainType_10B4E0[tempAxis.word] = 1;
			tempAxis._axis_2d.x--;
			if ((int8_t)mapAngle_13B4E0[tempAxis.word] >= 0)
				mapTerrainType_10B4E0[tempAxis.word] = 1;
			tempAxis._axis_2d.y--;
			if ((int8_t)mapAngle_13B4E0[tempAxis.word] >= 0)
				mapTerrainType_10B4E0[tempAxis.word] = 1;
			tempAxis._axis_2d.x++;
			if ((int8_t)mapAngle_13B4E0[tempAxis.word] >= 0)
				mapTerrainType_10B4E0[tempAxis.word] = 1;
			tempAxis._axis_2d.x++;
			tempAxis._axis_2d.y++;			
		}
		tempAxis._axis_2d.x -= inAxis2dB._axis_2d.x - inAxis2dA._axis_2d.x + 1;
		tempAxis._axis_2d.y++;
	}
	xAdd = inAxis2dB._axis_2d.x - inAxis2dA._axis_2d.x + 2;
	yAdd = inAxis2dB._axis_2d.y - inAxis2dA._axis_2d.y + 2;	
	inAxis2dA._axis_2d.x--;
	inAxis2dA._axis_2d.y--;
	tempAxis.word = inAxis2dA.word;
	for (uint8_t indexY = yAdd; indexY != 0; indexY--)
	{
		for (uint8_t indexX = xAdd; indexX != 0; indexX--)
		{
			if (mapTerrainType_10B4E0[tempAxis.word] == 1)
			{
				point1 = mapAngle_13B4E0[tempAxis.word];
				tempAxis._axis_2d.x++;
				point2 = mapAngle_13B4E0[tempAxis.word];
				tempAxis._axis_2d.y++;
				point3 = mapAngle_13B4E0[tempAxis.word];
				tempAxis._axis_2d.x--;
				point4 = mapAngle_13B4E0[tempAxis.word];
				tempAxis._axis_2d.y--;
				terModIndex = (point4 & 7) + 7 * (point3 & 7) + 49 * (point2 & 7) + 343 * (point1 & 7);
				mapTerrainType_10B4E0[tempAxis.word] = building_F2CD0x[terModIndex][0];
				if (building_F2CD0x[terModIndex][0] >= 8u)
				{
					nextAngle = building_F2CD0x[terModIndex][1] + (mapAngle_13B4E0[tempAxis.word] & 0x87);
				}
				else
				{
					rand2_17B4E0 = 9377 * rand2_17B4E0 + 9439;
					nextAngle = (mapAngle_13B4E0[tempAxis.word] & 0x87) + 16 * (rand2_17B4E0 % 7u);
				}
				mapAngle_13B4E0[tempAxis.word] = nextAngle;
			}
			tempAxis._axis_2d.x++;
		}
		tempAxis._axis_2d.x -= xAdd;
		tempAxis._axis_2d.y++;
	}
	xAdd++;
	yAdd++;
	tempAxis.word = inAxis2dA.word;
	if (yAdd != 0)
	{
		for (uint8_t indexY = yAdd; indexY != 0; indexY--)
		{
			for (uint8_t indexX = xAdd; indexX != 0; indexX--)
			{
				tempAxis._axis_2d.x++;
				tempAxis._axis_2d.y++;
				point1 = mapHeightmap_11B4E0[tempAxis.word];
				tempAxis._axis_2d.x -= 2;
				tempAxis._axis_2d.y -= 2;
				point2 = mapHeightmap_11B4E0[tempAxis.word];
				tempAxis._axis_2d.x++;
				tempShad = point2 - point1 + 32;
				tempAxis._axis_2d.y++;
				if (tempShad >= 28)
				{
					if (tempShad > 40)
						tempShad = (tempShad & 7) + 40;
				}
				else
				{
					tempShad = (tempShad & 3) + 28;
				}
				if (world_D41A0.terrain_2FECE.MapType != MapType_t::Day)
					mapShading_12B4E0[tempAxis.word] = 32 - tempShad + 32;
				else
					mapShading_12B4E0[tempAxis.word] = tempShad;
				if (isCaveLevel_D41B6 && (secondHeightmap_14B4E0[tempAxis.word] <= mapHeightmap_11B4E0[tempAxis.word]))
				{
					secondHeightmap_14B4E0[tempAxis.word] = mapHeightmap_11B4E0[tempAxis.word] - 1;
					mapAngle_13B4E0[tempAxis.word] |= 8u;
				}
				else
				{
					mapAngle_13B4E0[tempAxis.word] &= 0xF7u;
				}
				tempAxis._axis_2d.x++;
			}
			tempAxis._axis_2d.x -= xAdd;
			tempAxis._axis_2d.y++;
		}
	}
}

//----- (000104A0) --------------------------------------------------------
signed int GetTileAngleBit_104A0(axis_3d* axis3d)//1f14a0
{
	return 1 << (mapAngle_13B4E0[(axis3d->x >> 8) + ((axis3d->y >> 8) << 8)] & 0xF);
}

//----- (000104D0) --------------------------------------------------------
uint32_t TerrainTileIsWater_104D0(axis_3d* axis3d)//1f14d0
{
	uaxis_2d axis2d;
	axis2d._axis_2d.x = axis3d->x >> 8;
	axis2d._axis_2d.y = axis3d->y >> 8;
	return TerrainTileType_10590(mapTerrainType_10B4E0[axis2d.word]);
}

//----- (00010590) --------------------------------------------------------
uint32_t TerrainTileType_10590(char tileType)//1f1590
{
	uint32_t result = 0;
	switch (tileType)
	{
	case 0:
		result = 1;
		break;
	case 1:
		result = 2;
		break;
	case 2:
		result = 4;
		break;
	case 3:
		result = 8;
		break;
	case 4:
		result = 0x10;
		break;
	case 5:
		result = 0x20;
		break;
	case 8:
		result = 0x100;
		break;
	case 9:
		result = 0x200;
		break;
	case 10:
		result = 0x100000;
		break;
	case 11:
		result = 0x200000;
		break;
	case 12:
		result = 0x400000;
		break;
	case 13:
	case 14:
		return result;
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
		result = 0x400;
		break;
	case 21:
	case 22:
	case 24:
		result = 0x20000;
		break;
	case 23:
		result = 0x40000;
		break;
	case 25:
	case 27:
		result = 0x80000;
		break;
	case 26:
		result = 0x10000;
		break;
	default:
		result = 0x800000;
		break;
	}
	return result;
}

//----- (00010C40) --------------------------------------------------------
int GetTerrainAlt_10C40(axis_3d* axis3d)//1f1c40
{
	return GetTerrainAlt2_B5C60(axis3d->x, axis3d->y);
}

//----- (00011E70) --------------------------------------------------------
bool IsTouchingCeiling_11E70(type_entity_0x6E8E* event, axis_3d* axis3d)//1f2e70
{
	return event->params_0x52_82.fov + GetTerrainAlt_10C40(axis3d) + event->typeParams_0xA0_160x->minHeight_160_0xc_12 > GetCeilingAlt_10C60(axis3d);
}

//----- (00010C60) --------------------------------------------------------
int GetCeilingAlt_10C60(axis_3d* axis3d)//1f1c60
{
	return GetCeilingAlt2_B5D68(axis3d->x, axis3d->y);
}

//----- (000B5D68) --------------------------------------------------------
int GetCeilingAlt2_B5D68(uint16_t inX, uint16_t inY)//296d68
{
	uint8_t point1;
	uint8_t point2;
	int finalComp;
	int result;

	uaxis_2d tempAxis;
	uaxis_2d tempInX;
	uaxis_2d tempInY;
	tempInX.word = inX;
	tempInY.word = inY;
	tempAxis._axis_2d.x = tempInX._axis_2d.y;
	tempAxis._axis_2d.y = tempInY._axis_2d.y;

	if ((tempInX._axis_2d.y + tempInY._axis_2d.y) & 1)
	{
		if (tempInY._axis_2d.x > (tempInY._axis_2d.x + tempInX._axis_2d.x) % 256)
		{
			tempAxis._axis_2d.y++;
			point1 = secondHeightmap_14B4E0[tempAxis.word];
			tempAxis._axis_2d.x++;
			point2 = secondHeightmap_14B4E0[tempAxis.word];
			tempInY._axis_2d.x = 255 - tempInY._axis_2d.x;
			tempAxis._axis_2d.y--;
			finalComp = tempInY._axis_2d.x * (secondHeightmap_14B4E0[tempAxis.word] - point2) + tempInX._axis_2d.x * (point2 - point1);
		}
		else
		{
			point1 = secondHeightmap_14B4E0[tempAxis.word];
			tempAxis._axis_2d.x++;
			point2 = secondHeightmap_14B4E0[tempAxis.word];
			tempAxis._axis_2d.x--;
			tempAxis._axis_2d.y++;
			finalComp = tempInY._axis_2d.x * (secondHeightmap_14B4E0[tempAxis.word] - point1) + tempInX._axis_2d.x * (point2 - point1);
		}
		result = (finalComp >> 3) + 32 * point1;
	}
	else
	{
		if (tempInX._axis_2d.x <= tempInY._axis_2d.x)
		{
			point1 = secondHeightmap_14B4E0[tempAxis.word];
			tempAxis._axis_2d.y++;
			point2 = secondHeightmap_14B4E0[tempAxis.word];
			tempAxis._axis_2d.x++;
			finalComp = tempInY._axis_2d.x * (point2 - point1) + tempInX._axis_2d.x * (secondHeightmap_14B4E0[tempAxis.word] - point2);
		}
		else
		{
			point1 = secondHeightmap_14B4E0[tempAxis.word];
			tempAxis._axis_2d.x++;
			point2 = secondHeightmap_14B4E0[tempAxis.word];
			tempAxis._axis_2d.y++;
			finalComp = tempInY._axis_2d.x * (secondHeightmap_14B4E0[tempAxis.word] - point2) + tempInX._axis_2d.x * (point2 - point1);
		}
		result = (finalComp >> 3) + 32 * point1;
	}
	return result;
}

//----- (00041A90) --------------------------------------------------------
/*char sub_41A90_VGA_Palette_install_orig(unsigned __int8 *a1)//insgtalace palety viz:http://www.hoschl.cz/cyril/voxel5.c
{
  char result; // al
  unsigned __int8 *v2; // ebx
  unsigned __int16 v3; // cx
  unsigned __int8 v4; // al
  unsigned __int8 *v5; // ebx
  unsigned __int8 v6; // al

  result = -1;
  v2 = a1;
  __outx_BYTE(0x3C6u, 0xFFu);
  v3 = 0;
  while ( v3 < 0x100u )
  {
	__outx_BYTE(0x3C8u, v3);
	v4 = *v2;
	v5 = v2 + 1;
	__outx_BYTE(0x3C9u, v4);
	v6 = *v5++;
	__outx_BYTE(0x3C9u, v6);
	++v3;
	result = *v5;
	v2 = v5 + 1;
	__outx_BYTE(0x3C9u, result);
  }
  if ( vrDeviceActive_E12EC )
	result = ReadVrPort_75A10(2, a1);//256a10
  return result;
}
// E12EC: using guessed type char vrDeviceActive_E12EC;*/

/*
void debug_testdraw_terrain(uint8_t* buffer) {
	uint8_t temppal[768];
	for (int i = 0; i < 256; i++)
	{
		temppal[i * 3] = i;
		temppal[i * 3 + 1] = i;
		temppal[i * 3 + 2] = i;
	}
	VGA_Set_Palette2(temppal);
	int k = 0;
	for (int i = 0; i < 256; i++)
		for (int j = 0; j < 256; j++)
		{
			pdwScreenBuffer_351628[j * 320 + i] = buffer[k];
			k++;
		}
	VGA_Debug_Blit(640, 480, pdwScreenBuffer_351628);
}*/


//----- (00043C60) --------------------------------------------------------
void RandomiseTerrainArea_43C60(unsigned __int8 a1, char a2, int a3, int a4)//224c60
{
	unsigned int v4; // ecx
	int v5; // esi
	unsigned __int16 v6; // bx
	int v7; // edi
	int v8; // ecx
	unsigned __int16 v9; // bx
	int v10; // esi
	unsigned __int8 v11; // dh

	v4 = 37487429;
	LOBYTE(v6) = a1;
	v5 = a3;
	HIBYTE(v6) = a2;
	while ((x_WORD)--v5 != -1)
	{
		v7 = a4;
		while ((x_WORD)--v7 != -1)
		{
			if (!(mapAngle_13B4E0[v6] & 8))
			{
				v4 = 9377 * v4 + 9439;
				secondHeightmap_14B4E0[v6] += v4 % 7 - 3;
			}
			LOBYTE(v6) = v6 + 1;
		}
		v6 = __PAIR__(HIBYTE(v6), a1) + 256;
	}
	LOBYTE(v9) = a1;
	v8 = a3;
	HIBYTE(v9) = a2;
	while ((x_WORD)--v8 != -1)
	{
		v10 = a4;
		while ((x_WORD)--v10 != -1)
		{
			v11 = mapHeightmap_11B4E0[v9];
			if ((unsigned __int8)secondHeightmap_14B4E0[v9] > v11)
			{
				mapAngle_13B4E0[v9] &= 0xF7u;
			}
			else
			{
				secondHeightmap_14B4E0[v9] = v11 - 1;
				mapAngle_13B4E0[v9] |= 8u;
			}
			LOBYTE(v9) = v9 + 1;
		}
		v9 = __PAIR__(HIBYTE(v9), a1) + 256;
	}
}

//----- (00046180) --------------------------------------------------------
unsigned __int16 SetTerrainTypeAndNeighbours_46180(unsigned __int16 a1, char a2)//227180
{
	//unsigned __int16 v2; // ax
	unsigned __int16 v3; // bx
	unsigned __int16 v4; // bx
	unsigned __int16 v5; // bx
	unsigned __int16 result; // ax
	char v7; // bl
	char i; // bh
	unsigned __int16 v9; // si
	int v10; // edx

	//HIBYTE(v2) = HIBYTE(a1);
	//LOBYTE(v2) = a1 - 1;
	HIBYTE(result) = HIBYTE(a1);
	LOBYTE(result) = a1 - 1;

	mapTerrainType_10B4E0[a1] = a2;
	//v3 = v2;
	v3 = result;
	HIBYTE(result) = HIBYTE(a1) - 1;
	mapTerrainType_10B4E0[v3] = a2;
	v4 = result;
	LOBYTE(result) = a1;
	mapTerrainType_10B4E0[v4] = a2;
	v5 = result;
	LOBYTE(result) = a1 - 1;
	mapTerrainType_10B4E0[v5] = a2;
	v7 = 3;
	do
	{
		for (i = 3; i; i--)
		{
			LOBYTE(result) = result + 1;
			++HIBYTE(result);
			LOWORD(v10) = result;
			LOBYTE(result) = result - 2;
			HIBYTE(result) -= 2;
			v9 = result;
			LOBYTE(result) = result + 1;
			LOBYTE(v10) = mapHeightmap_11B4E0[v9] - mapHeightmap_11B4E0[(unsigned __int16)v10] + 32;
			++HIBYTE(result);
			if ((char)v10 >= 28)
			{
				if ((char)v10 > 40)
					LOBYTE(v10) = (v10 & 7) + 40;
			}
			else
			{
				LOBYTE(v10) = (v10 & 3) + 28;
			}
			if (world_D41A0.terrain_2FECE.MapType != MapType_t::Day)
				v10 = 32 - (char)v10 + 32;
			mapShading_12B4E0[result] = v10;
			if (isCaveLevel_D41B6 && (unsigned __int8)secondHeightmap_14B4E0[result] <= mapHeightmap_11B4E0[result])
			{
				secondHeightmap_14B4E0[result] = mapHeightmap_11B4E0[result] - 1;
				mapAngle_13B4E0[result] |= 8u;
			}
			else
			{
				mapAngle_13B4E0[result] &= 0xF7u;
			}
			LOBYTE(result) = result + 1;
		}
		LOBYTE(result) = result - 3;
		--v7;
		++HIBYTE(result);
	} while (v7);
	return result;
}

//----- (00046570) --------------------------------------------------------
void AddBuildingToTerrain_46570(uaxis_2d axis1, uaxis_2d axis2)//227570
{
	uaxis_2d locAxis1;
	uaxis_2d locAxis2;

	if (CommandLineParams.DoDebugSequences()) {
		//add_compare(0x227573,CommandLineParams.DoDebugafterload());
	}

	locAxis1._axis_2d.x = axis2._axis_2d.x - axis1._axis_2d.x + 1;
	locAxis1._axis_2d.y = axis2._axis_2d.y - axis1._axis_2d.y + 1;
	locAxis2.word = axis1.word;
	char xAdd = axis2._axis_2d.x - axis1._axis_2d.x + 1;
	bool yIsZero = locAxis1._axis_2d.y == 0;
	char yAdd = axis2._axis_2d.y - axis1._axis_2d.y + 1;

	while (!yIsZero)
	{
		locAxis1._axis_2d.x = axis2._axis_2d.x - axis1._axis_2d.x + 1;
		while (locAxis1._axis_2d.x)
		{
			mapTerrainType_10B4E0[locAxis2.word] = 1;
			locAxis2._axis_2d.x--;
			mapTerrainType_10B4E0[locAxis2.word] = 1;
			locAxis2._axis_2d.y--;
			mapTerrainType_10B4E0[locAxis2.word] = 1;
			locAxis2._axis_2d.x++;			
			mapTerrainType_10B4E0[locAxis2.word] = 1;
			locAxis2._axis_2d.x++;
			locAxis2._axis_2d.y++;
			locAxis1._axis_2d.x--;
		}
		locAxis2._axis_2d.x -= xAdd;
		locAxis2._axis_2d.y++;
		yIsZero = --locAxis1._axis_2d.y == 0;
	}
	axis1._axis_2d.x--;
	axis1._axis_2d.y--;
	char yAddPlus = yAdd + 1;
	char yIndex = yAddPlus;
	axis2.word = axis1.word;
	char xAddPlus = xAdd + 1;
	//adress 2275eb
	while (yIndex)
	{
		if (CommandLineParams.DoDebugSequences()) {
			//add_compare(0x227607, CommandLineParams.DoDebugafterload());
		}
		locAxis1._axis_2d.x = xAddPlus;
		for (int i = xAddPlus; i; i--)
		{
			if (mapTerrainType_10B4E0[axis2.word] == 1)
			{
				int buildingIndex = 343 * (mapAngle_13B4E0[axis2.word] & 7);
				axis2._axis_2d.x++;
				buildingIndex += 49 * (mapAngle_13B4E0[axis2.word] & 7);
				axis2._axis_2d.y++;
				buildingIndex += 7 * (mapAngle_13B4E0[axis2.word] & 7);
				axis2._axis_2d.x--;
				buildingIndex += mapAngle_13B4E0[axis2.word] & 7;
				axis2._axis_2d.y--;
				mapTerrainType_10B4E0[axis2.word] = building_F2CD0x[buildingIndex][0];
				if (mapTerrainType_10B4E0[axis2.word] >= 8u)
				{
					mapAngle_13B4E0[axis2.word] = building_F2CD0x[buildingIndex][1] + (mapAngle_13B4E0[axis2.word] & 0x87);
				}
				else
				{
					rand2_17B4E0 = 9377 * rand2_17B4E0 + 9439;
					mapAngle_13B4E0[axis2.word] = (mapAngle_13B4E0[axis2.word] & 0x87) + 16 * ((unsigned __int16)rand2_17B4E0 % 7u);
				}
			}
			axis2._axis_2d.x++;
		}
		axis2._axis_2d.x -= xAddPlus;
		axis2._axis_2d.y++;
		yIndex--;
	}
	char xIndex = xAddPlus + 1;
	axis2.word = axis1.word;
	yIndex = yAddPlus + 1;
	//adress 227737
	if (yAddPlus != -1)
	{
		do
		{
			for (int j = xIndex; j; j--)
			{
				axis2._axis_2d.x++;
				axis2._axis_2d.y++;
				char zTemp = -mapHeightmap_11B4E0[axis2.word] + 32;
				axis2._axis_2d.x -= 2;
				axis2._axis_2d.y -= 2;
				zTemp += mapHeightmap_11B4E0[axis2.word];
				axis2._axis_2d.x++;
				axis2._axis_2d.y++;
				if (zTemp >= 28)
				{
					if (zTemp > 40)
						zTemp = (zTemp & 7) + 40;
				}
				else
				{
					zTemp = (zTemp & 3) + 28;
				}
				if (world_D41A0.terrain_2FECE.MapType != MapType_t::Day)
					mapShading_12B4E0[axis2.word] = 32 - zTemp + 32;
				else
					mapShading_12B4E0[axis2.word] = zTemp;
				locAxis1.word = axis2.word;
				if (isCaveLevel_D41B6 && secondHeightmap_14B4E0[axis2.word] <= mapHeightmap_11B4E0[axis2.word])
				{
					secondHeightmap_14B4E0[axis2.word] = mapHeightmap_11B4E0[axis2.word] - 1;
					mapAngle_13B4E0[axis2.word] |= 8u;
				}
				else
				{
					mapAngle_13B4E0[axis2.word] &= 0xF7u;
				}
				axis2._axis_2d.x++;
			}
			axis2._axis_2d.x -= xIndex;
			yIndex--;
			axis2._axis_2d.y++;
		} while (yIndex);
	}
}

//----- (00048990) --------------------------------------------------------
int GetAreaHeightRange_48990(char a1, char a2, char a3, char a4)//229990
{
	unsigned __int16 v4; // ax
	unsigned __int16 v5; // dx
	unsigned int v6; // eax
	unsigned int v7; // ebx
	unsigned int v8; // ecx
	unsigned int v9; // ecx
	unsigned int v10; // ecx

	LOBYTE(v4) = a1;
	HIBYTE(v4) = a2;
	HIBYTE(v5) = a2;
	LOBYTE(v5) = a4 + a1;
	v6 = mapHeightmap_11B4E0[v4];
	v7 = v6;
	v8 = mapHeightmap_11B4E0[v5];
	if (v8 <= v6)
	{
		if (v8 < v6)
			v7 = mapHeightmap_11B4E0[v5];
	}
	else
	{
		v6 = mapHeightmap_11B4E0[v5];
	}
	HIBYTE(v5) = a3 + a2;
	v9 = mapHeightmap_11B4E0[v5];
	if (v9 <= v6)
	{
		if (v9 < v7)
			v7 = mapHeightmap_11B4E0[v5];
	}
	else
	{
		v6 = mapHeightmap_11B4E0[v5];
	}
	LOBYTE(v5) = a1;
	v10 = mapHeightmap_11B4E0[v5];
	if (v10 > v6)
		return v10 - v7;
	if (v10 < v7)
		v7 = mapHeightmap_11B4E0[v5];
	return v6 - v7;
}

//----- (00048A20) --------------------------------------------------------
void DrawTerrainLine_48A20(int a1, char a2, char a3, int a4, int a5, unsigned __int8 a6)//229a20
{
	int v6; // eax
	int v7; // esi
	uaxis_2d v8x; // bx
	uaxis_2d v9x; // bx
	int v10; // eax
	int v11; // edi
	uint8_t v13; // [esp+0h] [ebp-8h]
	uint8_t v14; // [esp+1h] [ebp-7h]
	//__int16 v15; // [esp+2h] [ebp-6h]
	uaxis_2d v16x; // [esp+4h] [ebp-4h]
	uaxis_2d v17x; // [esp+4h] [ebp-4h]

	// fix if begin
	//v15 = 0;
	// end
	v6 = 0;

	v13 = a2 - a5;
	v14 = a3 - a4;
	HIWORD(v6) = v13 + (v14 << 8);//v15;//eax
	//esi=a1
	//[ebp-4]=HIWORD(v6)
	//ebx= HIWORD(v6)
	//HIBYTE(v16) = a3 - a4;
	//HIBYTE(v8) = a3 - a4;
	v16x._axis_2d.y = a3 - a4;
	v8x._axis_2d.y = a3 - a4;
	v7 = 2 * a4;
	//LOBYTE(v8) = a2 - a5 - a6;
	//LOBYTE(v16) = 2 * a5 + a2 - a5;
	v8x._axis_2d.x = a2 - a5 - a6;
	v16x._axis_2d.x = 2 * a5 + a2 - a5;
	//dh=a6
	//bl=v13
	while ((x_WORD)--v7 != -1)
	{
		LOWORD(a1) = a6;
		a1++;
		while ((x_WORD)--a1 != -1)
		{
			SetHeightmapByBuilding_48B90(v8x);
			SetHeightmapByBuilding_48B90(v16x);
			//LOBYTE(v8)++;
			//LOBYTE(v16)++;
			v8x._axis_2d.x++;
			v16x._axis_2d.x++;
		}
		//LOBYTE(v16) = 2 * a5 + v13;
		//LOBYTE(v8) = v13 - a6;
		v16x._axis_2d.x = 2 * a5 + v13;
		v8x._axis_2d.x = v13 - a6;
		//HIBYTE(v8)++;
		//HIBYTE(v16)++;
		v8x._axis_2d.y++;
		v16x._axis_2d.y++;
	}
	//LOBYTE(v9) = v13 - a6;
	//LOBYTE(v17) = v13 - a6;
	v9x._axis_2d.x = v13 - a6;
	v17x._axis_2d.x = v13 - a6;

	//HIBYTE(v9) = v14 - a6;
	//HIBYTE(v17) = 2 * a4 + v14;
	v9x._axis_2d.y = v14 - a6;
	v17x._axis_2d.y = 2 * a4 + v14;
	LOWORD(v6) = a6;
	v10 = 2 * v6;
	v11 = v10 + 2 * a5;
	while ((x_WORD)--v11 != -1)
	{
		LOWORD(v7) = a6;
		v7++;
		while ((x_WORD)--v7 != -1)
		{
			SetHeightmapByBuilding_48B90(v9x);
			SetHeightmapByBuilding_48B90(v17x);
			//HIBYTE(v9)++;
			//HIBYTE(v17)++;
			v9x._axis_2d.y++;
			v17x._axis_2d.y++;
		}
		LOBYTE(v10) = 2 * a4;
		//HIBYTE(v9) = v14 - a6;
		//LOBYTE(v9) = v9 + 1;
		v9x._axis_2d.y = v14 - a6;
		v9x._axis_2d.x++;
		BYTE1(v10) = 2 * a4 + v14;
		//HIBYTE(v17) = 2 * a4 + v14;
		//LOBYTE(v17) = v17 + 1;
		v17x._axis_2d.y = 2 * a4 + v14;
		v17x._axis_2d.x++;
	}
	//return v10;
}

//----- (00048B50) --------------------------------------------------------
void SetHeightmapByBuildingArea_48B50(uint8 x, uint8 y, int height, int width)//229b50
{
	uaxis_2d axis;
	axis._axis_2d.x = x;
	axis._axis_2d.y = y;
	int indexH = height;
	while (--indexH != -1)
	{
		int indexW = width;
		while (--indexW != -1)
		{
			SetHeightmapByBuilding_48B90(axis);
			axis._axis_2d.x++;
		}
		axis._axis_2d.y++;
		axis._axis_2d.x = x;
	}
}

uint8_t fix_10B4E0_terraintype(int index) {
#ifdef FIX_SetHeightmapByBuilding_48B90
	if (index < 0)
		return 0;
#endif
	return mapTerrainType_10B4E0[index];
};

int debugcounter_229b90 = 0;
//----- (00048B90) --------------------------------------------------------
void SetHeightmapByBuilding_48B90(uaxis_2d axis)//229b90
{
	/*if (debugcounter2 >= 0x2450)//0x234c//0x057e
	{
		origbyte = 0;
		remakebyte = 0;
		comp1 = compare_with_snapshot((char*)"0160-00229B94", (uint8_t*)x_WORD_15B4E0_source, 0x32c4e0, 0x20000, &origbyte, &remakebyte);
		comp2 = compare_with_snapshot((char*)"0160-00229B94", x_BYTE_11B4E0_height, 0x2ec4e0, 0x10000, &origbyte, &remakebyte);
		comp3 = compare_with_snapshot((char*)"0160-00229B94", x_BYTE_13B4E0_angle, 0x30c4e0, 0x10000, &origbyte, &remakebyte);
		comp4 = compare_with_snapshot((char*)"0160-00229B94", fix_10B4E0_terraintype, 0x2dc4e0, 0x10000, &origbyte, &remakebyte);
		comp5 = compare_with_snapshot((char*)"0160-00229B94", x_BYTE_12B4E0_shading, 0x2fc4e0, 0x10000, &origbyte, &remakebyte);

		//myprintf("i:%d,", debugcounter2);
	}
	debugcounter2++;*/
	if (CommandLineParams.DoDebugSequences()) {
		//if (debugcounter_229b90 >= 0x00001C43) {
		if (debugcounter_229b90 >= 0x00001d5d) {
			debugcounter_229b90++;
			debugcounter_229b90--;
		}
		debugcounter_229b90++;
		//add_compare(0x229B94, CommandLineParams.DoDebugafterload());
	}

	uint32 heightSum = 0;
	uint32 heightCount = 0;
	if (mapAngle_13B4E0[axis.word] & 7
		&& mapHeightmap_11B4E0[axis.word]
		&& (fix_10B4E0_terraintype(-0x101 + axis.word) <= 5u
			|| fix_10B4E0_terraintype(-0x101 + axis.word) > 0x22u)
		&& (fix_10B4E0_terraintype(-0x100 + axis.word) <= 5u
			|| fix_10B4E0_terraintype(-0x100 + axis.word) > 0x22u)
		&& (fix_10B4E0_terraintype(-0x1 + axis.word) <= 5u
			|| fix_10B4E0_terraintype(-0x1 + axis.word) > 0x22u)
		&& (fix_10B4E0_terraintype(axis.word) <= 5u
			|| fix_10B4E0_terraintype(axis.word) > 0x22u))
	{
		char indexB = 3;
		for (uint16 i = axis.word - 0x101; --indexB != -1; i += 0xfd)
		{
			char indexC = 3;
			while (--indexC != -1)
			{
				if (fix_10B4E0_terraintype(i) <= 5u
					|| fix_10B4E0_terraintype(i) > 0x22u)
				{
					heightCount++;
					heightSum += mapHeightmap_11B4E0[i];
				}
				i++;
			}
		}
		if (heightCount)
		{
			mapHeightmap_11B4E0[axis.word] = heightSum / heightCount;
			if (isCaveLevel_D41B6)
			{
				if (secondHeightmap_14B4E0[axis.word] > mapHeightmap_11B4E0[axis.word])
				{
					mapAngle_13B4E0[axis.word] &= 0xF7u;
				}
				else
				{
					secondHeightmap_14B4E0[axis.word] = mapHeightmap_11B4E0[axis.word] - 1;
					mapAngle_13B4E0[axis.word] |= 8;
				}
			}
		}
	}
}

//----- (00048D20) --------------------------------------------------------
__int16 FindCeilingHeight_48D20(int a1, unsigned __int16 a2)//229d20
{
	unsigned int v2; // esi
	unsigned int v3; // ecx
	signed int v4; // eax
	int v5; // ebx
	unsigned __int8 v6; // bl
	unsigned __int8 v7; // dh
	char v9; // [esp+8h] [ebp-8h]
	char v10; // [esp+Ch] [ebp-4h]

	v2 = 0;
	v3 = 0;
	LOBYTE(v4) = secondHeightmap_14B4E0[(unsigned __int16)a1];
	BYTE1(v4) = 3;
	v5 = a1 - 257;
	v9 = 3;
	while (--v9 != -1)
	{
		v10 = 3;
		while (1)
		{
			BYTE1(v4) = v10 - 1;
			v10 = BYTE1(v4);
			if (BYTE1(v4) == -1)
				break;
			v4 = abs((unsigned __int8)secondHeightmap_14B4E0[(unsigned __int16)v5] - (unsigned __int8)secondHeightmap_14B4E0[(unsigned __int16)a1]);
			if (v4 > a2)
			{
				v4 = (unsigned __int8)secondHeightmap_14B4E0[(unsigned __int16)v5];
				++v3;
				v2 += v4;
			}
			++v5;
		}
		v5 += 253;
	}
	if (v3)
	{
		v6 = v2 / v3;
		v4 = (unsigned __int16)a1;
		v7 = mapHeightmap_11B4E0[(unsigned __int16)a1];
		secondHeightmap_14B4E0[(unsigned __int16)a1] = v6;
		if (v6 > v7)
		{
			mapAngle_13B4E0[(unsigned __int16)a1] &= 0xF7u;
		}
		else
		{
			secondHeightmap_14B4E0[(unsigned __int16)a1] = v7 - 1;
			mapAngle_13B4E0[(unsigned __int16)a1] |= 8u;
		}
	}
	return v4;
}

//----- (00048DF0) --------------------------------------------------------
__int16 GetTerrainHeightFromSquare_48DF0(char x, char y, char height, char width)//229df0
{
	uaxis_2d locAxis1;
	uaxis_2d locAxis2;
	uaxis_2d locAxis3;
	uaxis_2d locAxis4;
	locAxis1._axis_2d.x = x;
	locAxis1._axis_2d.y = y;
	locAxis2._axis_2d.x = x + width;
	locAxis2._axis_2d.y = y;
	locAxis3._axis_2d.x = x + width;
	locAxis3._axis_2d.y = y + height;
	locAxis4._axis_2d.x = x;
	locAxis4._axis_2d.y = y + height;
	return (mapHeightmap_11B4E0[locAxis1.word] + mapHeightmap_11B4E0[locAxis2.word] + mapHeightmap_11B4E0[locAxis3.word] + mapHeightmap_11B4E0[locAxis4.word]) >> 2;
}

//----- (00048E60) --------------------------------------------------------
signed int GetMinHeightInArea_48E60(__int16 a1, __int16 a2, __int16 a3, unsigned __int16 a4)//229e60
{
	return GetMinHeightmapInArea_48F20(a1, a2, a3, a4, mapHeightmap_11B4E0);
}

//----- (00048E90) --------------------------------------------------------
signed int GetMaxHeightInArea_48E90(__int16 a1, __int16 a2, __int16 a3, unsigned __int16 a4)//229e90
{
	return GetMaxHeightmapInArea_48FD0(a1, a2, a3, a4, mapHeightmap_11B4E0);
}

//----- (00048EC0) --------------------------------------------------------
signed int GetMinCeilingInArea_48EC0(__int16 a1, __int16 a2, __int16 a3, unsigned __int16 a4)//229ec0
{
	return GetMinHeightmapInArea_48F20(a1, a2, a3, a4, secondHeightmap_14B4E0);
}

//----- (00048EF0) --------------------------------------------------------
signed int GetMaxCeilingInArea_48EF0(__int16 a1, __int16 a2, __int16 a3, unsigned __int16 a4)//229ef0
{
	return GetMaxHeightmapInArea_48FD0(a1, a2, a3, a4, secondHeightmap_14B4E0);
}

//----- (00048F20) --------------------------------------------------------
signed int GetMinHeightmapInArea_48F20(char a1, char a2, __int16 a3, unsigned __int16 a4, uint8_t* a5x)//229f20
{
	signed int result; // eax
	unsigned __int16 v6; // dx
	int v7; // esi
	unsigned __int16 v8; // ebx
	signed int v9; // ebx
	unsigned __int16 v10; // ebx
	signed int v11; // ebx

	LOBYTE(v6) = a1;
	result = 250;
	HIBYTE(v6) = a2;
	v7 = a4;
	if (a4)
	{
		do
		{
			LOBYTE(v8) = v6;
			if (result > a5x[v6])
				result = a5x[v6];
			HIBYTE(v8) = a3 + a2;
			v9 = a5x[v8];
			if (v9 < result)
				result = v9;
			LOBYTE(v6)++;
			v7--;
		} while (v7);
		LOWORD(v7) = a3;
		if (a3)
			goto LABEL_9;
	}
	else
	{
		LOWORD(v7) = a3;
		while (v7)
		{
		LABEL_9:
			HIBYTE(v10) = HIBYTE(v6);
			if (result > a5x[v6])
				result = a5x[v6];
			LOBYTE(v10) = v6 - a4;
			v11 = a5x[v10];
			if (v11 < result)
				result = v11;
			HIBYTE(v6)++;
			v7--;
		}
	}
	return result;
}

//----- (00048FD0) --------------------------------------------------------
signed int GetMaxHeightmapInArea_48FD0(char a1, char a2, __int16 a3, unsigned __int16 a4, uint8_t* a5x)//229fd0
{
	signed int result; // eax
	unsigned __int16 v6; // dx
	int v7; // esi
	unsigned __int16 v8; // ebx
	signed int v9; // ebx
	unsigned __int16 v10; // ebx
	signed int v11; // ebx

	LOBYTE(v6) = a1;
	result = 0;
	HIBYTE(v6) = a2;
	v7 = a4;
	if (a4)
	{
		do
		{
			LOBYTE(v8) = v6;
			if (result < a5x[v6])
				result = a5x[v6];
			BYTE1(v8) = a3 + a2;
			v9 = a5x[v8];
			if (v9 > result)
				result = v9;
			LOBYTE(v6) = v6 + 1;
			--v7;
		} while (v7);
		LOWORD(v7) = a3;
		if (a3)
			goto LABEL_9;
	}
	else
	{
		LOWORD(v7) = a3;
		while (v7)
		{
		LABEL_9:
			BYTE1(v10) = HIBYTE(v6);
			if (result < a5x[v6])
				result = a5x[v6];
			LOBYTE(v10) = v6 - a4;
			v11 = a5x[v10];
			if (v11 > result)
				result = v11;
			++HIBYTE(v6);
			--v7;
		}
	}
	return result;
}

//----- (00056EE0) --------------------------------------------------------
bool IsTileFreeOfFeatures_56EE0(uaxis_2d a1x)//237ee0
{
	char v1; // al

	v1 = mapAngle_13B4E0[a1x.word] & 7;
	return v1 != 5 && v1 != 2 && v1 != 3;
}

int debugcounter_237f10 = 0;
//----- (00056F10) --------------------------------------------------------
char FindFreeTileNear_56F10(__int16 a1, __int16 a2, __int16 a3, char a4)//237f10
{
	signed __int16 v4; // bx
	char v5; // dh
	int v6; // edx
	uaxis_2d v7x; // eax
	char v8; // dl
	uaxis_2d v9x; // bx
	char v11; // [esp+0h] [ebp-8h]
	uaxis_2d v12x; // [esp+4h] [ebp-4h]

	if (CommandLineParams.DoDebugSequences()) {
		//add_compare(0x237f12,CommandLineParams.DoDebugafterload(),1);
	}

	//LOBYTE(v12) = a1;
	//HIBYTE(v12) = a2;
	v12x._axis_2d.x = a1;
	v12x._axis_2d.y = a2;
	v11 = 0;
	v4 = a3 + mapHeightmap_11B4E0[v12x.word];
	if (v4 > 200)
	{
		v4 = 200;
		if (!a1 && !a2)
			v11 = 1;
	}
	if (v4 < 0)
	{
		v4 = 0;
		if (!a1 && !a2)
			v11 = 1;
	}
	if (a4 && (mapAngle_13B4E0[v12x.word] & 0x80))
		return 1;
	v5 = isCaveLevel_D41B6;
	mapHeightmap_11B4E0[v12x.word] = v4;
	if (v5)
	{
		v6 = (unsigned __int8)secondHeightmap_14B4E0[v12x.word] - a3;
		if (v6 >= 255)
			secondHeightmap_14B4E0[v12x.word] = -1;
		else
			secondHeightmap_14B4E0[v12x.word] = v6;
	}
	if (v4)
	{
		v7x.word = v12x.word;
		v8 = mapAngle_13B4E0[v12x.word] & 0xF8 | 1;
	}
	else
	{
		//LOBYTE(v9) = a1 - 1;
		//HIBYTE(v9) = a2 - 1;
		v9x._axis_2d.x = a1 - 1;
		v9x._axis_2d.y = a2 - 1;
		if (!IsTileFreeOfFeatures_56EE0(v9x))
			goto LABEL_28;
		//LOBYTE(v9) = v9 + 1;
		v9x._axis_2d.x++;
		if (!IsTileFreeOfFeatures_56EE0(v9x))
			goto LABEL_28;
		//LOBYTE(v9) = v9 + 1;
		v9x._axis_2d.x++;
		if (!IsTileFreeOfFeatures_56EE0(v9x))
			goto LABEL_28;
		//HIBYTE(v9) = a2;
		v9x._axis_2d.y = a2;
		if (!IsTileFreeOfFeatures_56EE0(v9x))
			goto LABEL_28;
		//LOBYTE(v9) = v9 - 2;
		v9x._axis_2d.x -= 2;
		if (!IsTileFreeOfFeatures_56EE0(v9x))
			goto LABEL_28;
		//HIBYTE(v9) = a2 + 1;
		v9x._axis_2d.y = a2 + 1;
		if (!IsTileFreeOfFeatures_56EE0(v9x))
			goto LABEL_28;
		//LOBYTE(v9) = v9 + 1;
		v9x._axis_2d.x++;
		if (!IsTileFreeOfFeatures_56EE0(v9x))
			goto LABEL_28;
		//LOBYTE(v9) = v9 + 1;
		v9x._axis_2d.x++;
		if (!IsTileFreeOfFeatures_56EE0(v9x))
			goto LABEL_28;
		v7x.word = v12x.word;
		v8 = mapAngle_13B4E0[v12x.word] & 0xF0;
	}
	mapAngle_13B4E0[v7x.word] = v8;
LABEL_28:
	if (a4)
	{
		UpdateTerrainTiles_462A0(v12x, v12x);
	}
	else
		AddBuildingToTerrain_46570(v12x, v12x);
	return v11;
}
// D41B6: using guessed type char x_BYTE_D41B6;

int debugcounter_2380f0 = 0;
int debugcounter_2380f0_2 = 0;
//----- (000570F0) --------------------------------------------------------
char FindFreeTileInArea_570F0(__int16 a1, __int16 a2, signed __int16 a3, char a4, char a5, char a6)//2380f0
{
	uaxis_2d v6x; // bx
	char v8; // [esp+0h] [ebp-8h]
	uaxis_2d v9x; // [esp+4h] [ebp-4h]

	if (CommandLineParams.DoDebugSequences()) {
		/*uint8_t origbyte20 = 0;
		uint8_t remakebyte20 = 0;
		int comp20;
		if (debugcounter_2380f0_2 >= 0xce70)
		{
			debugcounter_2380f0_2++;
			debugcounter_2380f0_2--;
		}
		debugcounter_2380f0_2++;
		if (debugafter_215540)
		{
			if (debugcounter_2380f0 >= 0x1000)
			{
				comp20 = compare_with_sequence((char*)"002380f0-002DC4E0", (uint8_t*)x_BYTE_10B4E0_terraintype, 0x2dc4e0, debugcounter_2380f0, 0x70000, 0x10000, &origbyte20, &remakebyte20);
				comp20 = compare_with_sequence((char*)"002380f0-002DC4E0", (uint8_t*)x_BYTE_11B4E0_height, 0x2dc4e0, debugcounter_2380f0, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x10000);
				comp20 = compare_with_sequence((char*)"002380f0-002DC4E0", (uint8_t*)x_BYTE_12B4E0_shading, 0x2dc4e0, debugcounter_2380f0, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x20000);
				comp20 = compare_with_sequence((char*)"002380f0-002DC4E0", (uint8_t*)x_BYTE_13B4E0_angle, 0x2dc4e0, debugcounter_2380f0, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x30000);
				comp20 = compare_with_sequence((char*)"002380f0-002DC4E0", (uint8_t*)x_WORD_15B4E0_source, 0x2dc4e0, debugcounter_2380f0, 0x70000, 0x20000, &origbyte20, &remakebyte20, 0x50000);
				comp20 = compare_with_sequence_D41A0((char*)"002380f0-00356038", (uint8_t*)& D41A0_BYTESTR_0, 0x356038, debugcounter_2380f0, 224790, &origbyte20, &remakebyte20);
			}
			debugcounter_2380f0++;
		}*/
	}

	//LOBYTE(v9) = a1;
	//HIBYTE(v9) = a2;
	v9x._axis_2d.x = a1;
	v9x._axis_2d.y = a2;
	v8 = 0;
	if (a3 > 255)
	{
		a3 = 255;
		if (!a1 && !a2)
			v8 = 1;
	}
	if (a3 < 0)
	{
		a3 = 0;
		if (!a1 && !a2)
			v8 = 1;
	}
	if (a4 && mapAngle_13B4E0[v9x.word] < 0)
		return 1;
	mapHeightmap_11B4E0[v9x.word] = a3;
	if (a5 || IsSpellCastable_57450(mapTerrainType_10B4E0[v9x.word]))
		mapAngle_13B4E0[v9x.word] = mapAngle_13B4E0[v9x.word] & 0xF8 | 1;
	if (!a3)
	{
		if (!a6)
			goto LABEL_32;
		//LOBYTE(v6) = a1 - 1;
		//HIBYTE(v6) = a2 - 1;
		v6x._axis_2d.x = a1 - 1;
		v6x._axis_2d.y = a2 - 1;
		if (IsTileFreeOfFeatures_56EE0(v6x))
		{
			//LOBYTE(v6) = v6 + 1;
			v6x._axis_2d.x++;
			if (IsTileFreeOfFeatures_56EE0(v6x))
			{
				//LOBYTE(v6) = v6 + 1;
				v6x._axis_2d.x++;
				if (IsTileFreeOfFeatures_56EE0(v6x))
				{
					//HIBYTE(v6) = a2;
					v6x._axis_2d.y = a2;
					if (IsTileFreeOfFeatures_56EE0(v6x))
					{
						//LOBYTE(v6) = v6 - 2;
						v6x._axis_2d.x -= 2;
						if (IsTileFreeOfFeatures_56EE0(v6x))
						{
							//HIBYTE(v6) = a2 + 1;
							v6x._axis_2d.y = a2 + 1;
							if (IsTileFreeOfFeatures_56EE0(v6x))
							{
								//LOBYTE(v6) = v6 + 1;
								v6x._axis_2d.x++;
								if (IsTileFreeOfFeatures_56EE0(v6x))
								{
									//LOBYTE(v6) = v6 + 1;
									v6x._axis_2d.x++;
									if (IsTileFreeOfFeatures_56EE0(v6x))
										LABEL_32:
									mapAngle_13B4E0[v9x.word] &= 0xF0u;
								}
							}
						}
					}
				}
			}
		}
	}
	if (a4)
	{
		UpdateTerrainTiles_462A0(v9x, v9x);
	}
	else
		AddBuildingToTerrain_46570(v9x, v9x);
	return v8;
}

//----- (00057390) --------------------------------------------------------
void DamageSceneryOnTile_57390(uaxis_2d axis, uint16_t id)//238390
{
	for (int i = mapEntityIndex_15B4E0[axis.word]; Entities_EA3E4[i] != Entities_EA3E4[0]; i = Entities_EA3E4[i]->oldMapEntity_0x16_22)
	{
		type_entity_0x6E8E* ix = Entities_EA3E4[i];
		if (ix->id_0x1A_26 != id)
		{
			if (ix->class_0x3F_63 == 2)
			{
				DisableEntityDrawing04_57F10(ix);
			}
			else if (ix->class_0x3F_63 == 5
				&& ix->model_0x40_64 != 6
				&& ix->model_0x40_64 != 8
				&& ix->model_0x40_64 != 10
				&& ix->model_0x40_64 != 0x10
				&& ix->model_0x40_64 != 0x16
				&& ix->model_0x40_64 != 0x17
				&& ix->model_0x40_64 != 27
				&& (ix->model_0x40_64 != 0x19 || ix->actionIndex_0x45_69 != 0xC8))
			{
				ix->life_0x8 = -1;
				ix->ownerPlayerId_0x24_36 = id;
				ix->hitById_0x26_38 = id;
			}
		}
	}
}

int debugcounter29x = 0;

//----- (00058030) --------------------------------------------------------
void GetTerrainHeightAt_58030(axis_3d* a1x, axis_3d* a2)//239030
{
	axis_3d* result; // eax
	uaxis_2d v3x; // dx
	unsigned __int8 v4; // bl
	unsigned __int8 v5; // bl
	unsigned __int8 v6; // bl

	result = a2;
	v3x._axis_2d.x = a1x->x >> 8;
	v3x._axis_2d.y = a1x->y >> 8;
	v4 = mapHeightmap_11B4E0[v3x.word];
	a2->x = v4;
	//LOBYTE(v3) = v3 + 1;
	v3x._axis_2d.x++;
	a2->y = v4;
	a2->x -= mapHeightmap_11B4E0[v3x.word];
	v5 = mapHeightmap_11B4E0[v3x.word];
	//++HIBYTE(v3);
	v3x._axis_2d.y++;
	result->y += v5;
	a2->x -= mapHeightmap_11B4E0[v3x.word];
	v6 = mapHeightmap_11B4E0[v3x.word];
	//LOBYTE(v3) = v3 - 1;
	v3x._axis_2d.x--;
	result->y -= v6;
	result->x += mapHeightmap_11B4E0[v3x.word];
	a2->y -= mapHeightmap_11B4E0[v3x.word];
	//return result;
}

//----- (000580E0) --------------------------------------------------------
void ClampAboveTerrain_580E0(axis_3d* a1x, signed int a2, int a3, int  /*a4*/, __int16 a5)//2390e0
{
	//signed int result; // eax

	//result = 0;
	if (a1x->z > a2)
	{
		//result = 1;
		a1x->z += a5;
	}
	if ((int16_t)a1x->z <= a2 + a3)
	{
		//result = 3;
		a1x->z = a3 + a2;
	}
	//return result;
}

// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// D41A4: using guessed type int x_DWORD_D41A4;
// E29EC: using guessed type char x_BYTE_E29EC;
// E29ED: using guessed type char x_BYTE_E29ED;
// E29EE: using guessed type char x_BYTE_E29EE;
// E29EF: using guessed type char x_BYTE_E29EF;
// E29F0: using guessed type char x_BYTE_E29F0;

//----- (00084790) --------------------------------------------------------
void ClearDynamicLights_84790()//265790
{
	memset(&world_D41A0.dynamicLights_0x3664C, 0, sizeof(type_str_0x3664C) * 50);//39x50
	world_D41A0.activeLights_0x36DFA = 0;
	world_D41A0.lightRandSeed_0x36DF2 = 0x3A013;
}

//----- (000847D0) --------------------------------------------------------
void AddDynamicLight_847D0(type_entity_0x6E8E* event, char a2, char a3, char a4)//2657d0
{
	if (world_D41A0.m_GameSettings.display_0x2196.m_wDynamicLighting)
	{
		if ((world_D41A0.terrain_2FECE.MapType == MapType_t::Night || world_D41A0.terrain_2FECE.MapType == MapType_t::Cave) && world_D41A0.activeLights_0x36DFA < 50)
		{
			int16_t index = 0;
			while (index < 50 && world_D41A0.dynamicLights_0x3664C[index].active_0)
			{
				index++;
			}
			world_D41A0.dynamicLights_0x3664C[index].active_0 = 1;
			world_D41A0.dynamicLights_0x3664C[index].flags_1 = a4;
			world_D41A0.dynamicLights_0x3664C[index].intensity_2 = a2;
			world_D41A0.dynamicLights_0x3664C[index].flickerRange_3 = a3;
			world_D41A0.dynamicLights_0x3664C[index].axis3d_4 = event->position_0x4C_76;
			world_D41A0.dynamicLights_0x3664C[index].event_A.pointer_0x6E8E = event;
			event->flags_0xc_12_15.byte[2] |= 8u;
			memset(world_D41A0.dynamicLights_0x3664C[index].shading_E, 0, 25);
			world_D41A0.activeLights_0x36DFA++;
		}
	}
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (00084880) --------------------------------------------------------
void FreeLightSource_84880(type_str_0x3664C* a1x)//265880
{
	//int result; // eax

	a1x->active_0 = 0;
	//result = (int)x_D41A0_BYTEARRAY_0;
	world_D41A0.activeLights_0x36DFA--;
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//int debugcounter_2658a0 = 0;

//----- (000848A0) --------------------------------------------------------
void UpdateLightSourcesOld_848A0()//2658a0
{
	//uaxis_2d v0x; // ax
	//uint8_t* v1; // edx
	int v1x;
	__int16 v2; // bx
	__int16 v3; // si
	uaxis_2d v4x; // eax
	/*unsigned __int16 v5; // di
	unsigned __int16 v6; // di
	unsigned __int16 v7; // di
	unsigned __int16 v8; // di
	unsigned __int16 v9; // di
	unsigned __int16 v10; // di
	unsigned __int16 v11; // di
	unsigned __int16 v12; // di
	unsigned __int16 v13; // di
	unsigned __int16 v14; // di
	unsigned __int16 v15; // di
	unsigned __int16 v16; // di
	unsigned __int16 v17; // di
	unsigned __int16 v18; // di
	unsigned __int16 v19; // di
	unsigned __int16 v20; // di
	unsigned __int16 v21; // di
	unsigned __int16 v22; // di
	unsigned __int16 v23; // di
	unsigned __int16 v24; // di
	unsigned __int16 v25; // di
	unsigned __int16 v26; // di
	unsigned __int16 v27; // di
	char v28; // ch
	unsigned __int16 v29; // di*/
	__int16 v31; // [esp+4h] [ebp-4h]

	if (CommandLineParams.DoDebugSequences()) {
		//add_compare(0x2658a4, CommandLineParams.DoDebugafterload());
	}

	//LOBYTE(v0) = D41A0_BYTESTR_0.str_2FECE.MapType;
	if (world_D41A0.terrain_2FECE.MapType == MapType_t::Night || world_D41A0.terrain_2FECE.MapType == MapType_t::Cave)
	{
		//v0 = D41A0_BYTESTR_0.word_0x36DFA;
		v31 = world_D41A0.activeLights_0x36DFA;
		if (world_D41A0.activeLights_0x36DFA)
		{
			//D41A0_BYTESTR_0.str_0x3664C[v1x].
			//v1 = x_D41A0_BYTEARRAY_0 + 222796;
			v1x = 0;
			v2 = 0;
			v3 = 0;
			while (v2 < 50 && v3 < v31)
			{
				if (world_D41A0.dynamicLights_0x3664C[v1x].active_0)
				{
					v4x._axis_2d.x = (world_D41A0.dynamicLights_0x3664C[v1x].axis3d_4.x >> 8) - 2;
					v4x._axis_2d.y = (world_D41A0.dynamicLights_0x3664C[v1x].axis3d_4.y >> 8) - 2;
					//v4x._axis_2d.x -=2;
					//v5 = v4;
					//LOBYTE(v4)++;
					mapShading_12B4E0[v4x.word] -= world_D41A0.dynamicLights_0x3664C[v1x].shading_E[0];
					v4x._axis_2d.x++;
					//v6 = v4;
					//LOBYTE(v4)++;
					mapShading_12B4E0[v4x.word] -= world_D41A0.dynamicLights_0x3664C[v1x].shading_E[1];
					v4x._axis_2d.x++;
					//v7 = v4;
					//LOBYTE(v4)++;
					mapShading_12B4E0[v4x.word] -= world_D41A0.dynamicLights_0x3664C[v1x].shading_E[2];
					v4x._axis_2d.x++;
					//v8 = v4;
					//LOBYTE(v4)++;
					mapShading_12B4E0[v4x.word] -= world_D41A0.dynamicLights_0x3664C[v1x].shading_E[3];
					v4x._axis_2d.x++;
					//v9 = v4;
					//HIBYTE(v4)++;
					mapShading_12B4E0[v4x.word] -= world_D41A0.dynamicLights_0x3664C[v1x].shading_E[4];
					v4x._axis_2d.y++;
					//v10 = v4;
					//LOBYTE(v4)--;
					mapShading_12B4E0[v4x.word] -= world_D41A0.dynamicLights_0x3664C[v1x].shading_E[5];
					v4x._axis_2d.x--;
					//v11 = v4;
					//LOBYTE(v4)--;
					mapShading_12B4E0[v4x.word] -= world_D41A0.dynamicLights_0x3664C[v1x].shading_E[6];
					v4x._axis_2d.x--;
					//v12 = v4;
					//LOBYTE(v4)--;
					mapShading_12B4E0[v4x.word] -= world_D41A0.dynamicLights_0x3664C[v1x].shading_E[7];
					v4x._axis_2d.x--;
					//v13 = v4;
					//LOBYTE(v4)--;
					mapShading_12B4E0[v4x.word] -= world_D41A0.dynamicLights_0x3664C[v1x].shading_E[8];
					v4x._axis_2d.x--;
					//v14 = v4;
					//BYTE1(v4)++;
					mapShading_12B4E0[v4x.word] -= world_D41A0.dynamicLights_0x3664C[v1x].shading_E[9];
					v4x._axis_2d.y++;
					//v15 = v4;
					//LOBYTE(v4)++;
					mapShading_12B4E0[v4x.word] -= world_D41A0.dynamicLights_0x3664C[v1x].shading_E[10];
					v4x._axis_2d.x++;
					//v16 = v4;
					//LOBYTE(v4)++;
					mapShading_12B4E0[v4x.word] -= world_D41A0.dynamicLights_0x3664C[v1x].shading_E[11];
					v4x._axis_2d.x++;
					//v17 = v4;
					//LOBYTE(v4)++;
					mapShading_12B4E0[v4x.word] -= world_D41A0.dynamicLights_0x3664C[v1x].shading_E[12];
					v4x._axis_2d.x++;
					//v18 = v4;
					//LOBYTE(v4)++;
					mapShading_12B4E0[v4x.word] -= world_D41A0.dynamicLights_0x3664C[v1x].shading_E[13];
					v4x._axis_2d.x++;
					//v19 = v4;
					//HIBYTE(v4)++;
					mapShading_12B4E0[v4x.word] -= world_D41A0.dynamicLights_0x3664C[v1x].shading_E[14];
					v4x._axis_2d.y++;
					//v20 = v4;
					//LOBYTE(v4)--;
					mapShading_12B4E0[v4x.word] -= world_D41A0.dynamicLights_0x3664C[v1x].shading_E[15];
					v4x._axis_2d.x--;
					v3++;
					mapShading_12B4E0[v4x.word] -= world_D41A0.dynamicLights_0x3664C[v1x].shading_E[16];
					v4x._axis_2d.x--;
					//LOBYTE(v4)--;
					//v21 = v4;
					//LOBYTE(v4)--;
					mapShading_12B4E0[v4x.word] -= world_D41A0.dynamicLights_0x3664C[v1x].shading_E[17];
					v4x._axis_2d.x--;
					//v22 = v4;
					//LOBYTE(v4) = v4 - 1;
					mapShading_12B4E0[v4x.word] -= world_D41A0.dynamicLights_0x3664C[v1x].shading_E[18];//32
					v4x._axis_2d.x--;
					//v23 = v4;
					//HIBYTE(v0)++;
					mapShading_12B4E0[v4x.word] -= world_D41A0.dynamicLights_0x3664C[v1x].shading_E[19];//33
					v4x._axis_2d.y++;
					//v0x.word=v4x.word;
					//v24 = v0;
					//LOBYTE(v0)++;
					mapShading_12B4E0[v4x.word] -= world_D41A0.dynamicLights_0x3664C[v1x].shading_E[20];//34
					v4x._axis_2d.x++;
					//v25 = v0;
					//LOBYTE(v0)++;
					mapShading_12B4E0[v4x.word] -= world_D41A0.dynamicLights_0x3664C[v1x].shading_E[21];//35
					v4x._axis_2d.x++;
					//v26 = v0;
					//LOBYTE(v0)++;
					mapShading_12B4E0[v4x.word] -= world_D41A0.dynamicLights_0x3664C[v1x].shading_E[22];//36
					v4x._axis_2d.x++;
					//v27 = v0;
					//v28 = x_BYTE_12B4E0_shading[v0] - D41A0_BYTESTR_0.str_0x3664C[v1x].array_E[23];
					//LOBYTE(v0)++;
					mapShading_12B4E0[v4x.word] -= world_D41A0.dynamicLights_0x3664C[v1x].shading_E[23];//37
					//v29 = v0;
					v4x._axis_2d.x++;
					//LOBYTE(v0) = D41A0_BYTESTR_0.str_0x3664C[v1x].array_E[24];
					mapShading_12B4E0[v4x.word] -= world_D41A0.dynamicLights_0x3664C[v1x].shading_E[24];//38
				}
				v2++;
				//v1 += 39;
				v1x++;
			}
		}
	}
	//return v0;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//int debugcounter_265b80 = 0;
//----- (00084B80) --------------------------------------------------------
void UpdateLightSources_84B80()//265b80
{
	type_entity_0x6E8E* v2x; // esi
	int v3; // esi
	uaxis_2d v4x; // bx
	int iy;
	int16_t v7x;
	int16_t v9; // [esp+Ch] [ebp-4h]
	type_entity_0x6E8E FIXx;

	if (CommandLineParams.DoDebugSequences()) {
		//add_compare(0x265b84, CommandLineParams.DoDebugafterload());
	}

	if (world_D41A0.terrain_2FECE.MapType == MapType_t::Night || world_D41A0.terrain_2FECE.MapType == MapType_t::Cave)
	{
		v9 = world_D41A0.activeLights_0x36DFA;
		if (world_D41A0.activeLights_0x36DFA)
		{
			v7x = 0;
			for (iy = 0; iy < 50; iy++)
			{
				if (v7x >= v9)
					break;
				if (world_D41A0.dynamicLights_0x3664C[iy].active_0)
				{
					v2x = world_D41A0.dynamicLights_0x3664C[iy].event_A.pointer_0x6E8E;
					v7x++;
					//fix
					if (v2x == 0)
					{
						FIXx.life_0x8 = 0x00700008;
						FIXx.position_0x4C_76.x = 0xe98c;
						FIXx.position_0x4C_76.y = 0xf000;
						FIXx.position_0x4C_76.z = 0xe60c;
						FIXx.flags_0xc_12_15.byte[1] = 0;
						v2x = &FIXx;
					}
					//fix

					if (v2x->life_0x8 < 0 || v2x->flags_0xc_12_15.byte[1] & 4)
					{
						FreeLightSource_84880(&world_D41A0.dynamicLights_0x3664C[iy]);
					}
					else
					{
						world_D41A0.dynamicLights_0x3664C[iy].axis3d_4 = v2x->position_0x4C_76;
						v4x._axis_2d.x = world_D41A0.dynamicLights_0x3664C[iy].axis3d_4.x >> 8;
						v4x._axis_2d.y = world_D41A0.dynamicLights_0x3664C[iy].axis3d_4.y >> 8;
						world_D41A0.lightRandSeed_0x36DF2 = 9377 * world_D41A0.lightRandSeed_0x36DF2 + 9439;
						v3 = world_D41A0.lightRandSeed_0x36DF2 % world_D41A0.dynamicLights_0x3664C[iy].flickerRange_3
							- world_D41A0.dynamicLights_0x3664C[iy].flickerRange_3 / 2;
						v4x._axis_2d.y -= 2;
						v4x._axis_2d.x -= 2;
						//adress 265c5d
						ApplyLightToTile_84EA0(v4x, &world_D41A0.dynamicLights_0x3664C[iy], 0, v3);
						v4x._axis_2d.x++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.dynamicLights_0x3664C[iy], 1, v3);
						v4x._axis_2d.x++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.dynamicLights_0x3664C[iy], 2, v3);
						v4x._axis_2d.x++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.dynamicLights_0x3664C[iy], 3, v3);
						v4x._axis_2d.x++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.dynamicLights_0x3664C[iy], 4, v3);
						v4x._axis_2d.y++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.dynamicLights_0x3664C[iy], 5, v3);
						v4x._axis_2d.x--;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.dynamicLights_0x3664C[iy], 6, v3);
						v4x._axis_2d.x--;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.dynamicLights_0x3664C[iy], 7, v3);
						v4x._axis_2d.x--;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.dynamicLights_0x3664C[iy], 8, v3);
						v4x._axis_2d.x--;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.dynamicLights_0x3664C[iy], 9, v3);
						v4x._axis_2d.y++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.dynamicLights_0x3664C[iy], 10, v3);
						v4x._axis_2d.x++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.dynamicLights_0x3664C[iy], 11, v3);
						v4x._axis_2d.x++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.dynamicLights_0x3664C[iy], 12, v3);
						v4x._axis_2d.x++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.dynamicLights_0x3664C[iy], 13, v3);
						v4x._axis_2d.x++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.dynamicLights_0x3664C[iy], 14, v3);
						v4x._axis_2d.y++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.dynamicLights_0x3664C[iy], 15, v3);
						v4x._axis_2d.x--;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.dynamicLights_0x3664C[iy], 16, v3);
						v4x._axis_2d.x--;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.dynamicLights_0x3664C[iy], 17, v3);
						v4x._axis_2d.x--;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.dynamicLights_0x3664C[iy], 18, v3);
						v4x._axis_2d.x--;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.dynamicLights_0x3664C[iy], 19, v3);
						v4x._axis_2d.y++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.dynamicLights_0x3664C[iy], 20, v3);
						v4x._axis_2d.x++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.dynamicLights_0x3664C[iy], 21, v3);
						v4x._axis_2d.x++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.dynamicLights_0x3664C[iy], 22, v3);
						v4x._axis_2d.x++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.dynamicLights_0x3664C[iy], 23, v3);
						v4x._axis_2d.x++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.dynamicLights_0x3664C[iy], 24, v3);
					}
				}
			}
		}
	}
}

/*
//int debugcounter_265b80 = 0;
//----- (00084B80) --------------------------------------------------------
void UpdateLightSources_84B80()//265b80
{
	type_entity_0x6E8E* v2x; // esi
	int v3; // esi
	uaxis_2d v4x; // bx
	int iy;
	int16_t v7x;
	//__int16 v8; // [esp+8h] [ebp-8h]

	if (CommandLineParams.DoDebugSequences()) {
		//add_compare(0x265b84, CommandLineParams.DoDebugafterload());
	}
	if (world_D41A0.terrain_2FECE.MapType == MapType_t::Night || world_D41A0.terrain_2FECE.MapType == MapType_t::Cave)
	{
		if (world_D41A0.word_0x36DFA)
		{
			v7x = 0;
			for (iy = 0; iy < 50; iy++)
			{
				if (v7x >= world_D41A0.word_0x36DFA)
					break;
				if (world_D41A0.str_0x3664C[iy].byte_0)
				{
					v2x = world_D41A0.str_0x3664C[iy].event_A.pointer_0x6E8E;
					v7x++;

					//fix
					if (v2x == 0)
					{
						type_entity_0x6E8E FIXx;
						FIXx.dword_0x8 = 0x00700008;
						FIXx.position_0x4C_76.x = 0xe98c;
						FIXx.position_0x4C_76.y = 0xf000;
						FIXx.position_0x4C_76.z = 0xe60c;
						FIXx.flags_0xc_12_15.byte[1] = 0;
						v2x = &FIXx;
					}
					//fix

					if (v2x->dword_0x8 < 0 || v2x->flags_0xc_12_15.byte[1] & 4)
					{
						FreeLightSource_84880(&world_D41A0.str_0x3664C[iy]);
					}
					else
					{
						world_D41A0.str_0x3664C[iy].axis3d_4 = v2x->position_0x4C_76;
						v4x._axis_2d.x = world_D41A0.str_0x3664C[iy].axis3d_4.x >> 8;
						v4x._axis_2d.y = world_D41A0.str_0x3664C[iy].axis3d_4.y >> 8;
						world_D41A0.dword_0x36DF2 = 9377 * world_D41A0.dword_0x36DF2 + 9439;
						v3 = world_D41A0.dword_0x36DF2 % world_D41A0.str_0x3664C[iy].byte_3
							- world_D41A0.str_0x3664C[iy].byte_3 / 2;
						v4x._axis_2d.y -= 2;
						v4x._axis_2d.x -= 2;
						//adress 265c5d
						ApplyLightToTile_84EA0(v4x, &world_D41A0.str_0x3664C[iy], 0, v3);
						v4x._axis_2d.x++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.str_0x3664C[iy], 1, v3);
						v4x._axis_2d.x++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.str_0x3664C[iy], 2, v3);
						v4x._axis_2d.x++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.str_0x3664C[iy], 3, v3);
						v4x._axis_2d.x++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.str_0x3664C[iy], 4, v3);
						v4x._axis_2d.y++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.str_0x3664C[iy], 5, v3);
						v4x._axis_2d.x--;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.str_0x3664C[iy], 6, v3);
						v4x._axis_2d.x--;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.str_0x3664C[iy], 7, v3);
						v4x._axis_2d.x--;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.str_0x3664C[iy], 8, v3);
						v4x._axis_2d.x--;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.str_0x3664C[iy], 9, v3);
						v4x._axis_2d.y++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.str_0x3664C[iy], 10, v3);
						v4x._axis_2d.x++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.str_0x3664C[iy], 11, v3);
						v4x._axis_2d.x++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.str_0x3664C[iy], 12, v3);
						v4x._axis_2d.x++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.str_0x3664C[iy], 13, v3);
						v4x._axis_2d.x++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.str_0x3664C[iy], 14, v3);
						v4x._axis_2d.y++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.str_0x3664C[iy], 15, v3);
						v4x._axis_2d.x--;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.str_0x3664C[iy], 16, v3);
						v4x._axis_2d.x--;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.str_0x3664C[iy], 17, v3);
						v4x._axis_2d.x--;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.str_0x3664C[iy], 18, v3);
						v4x._axis_2d.x--;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.str_0x3664C[iy], 19, v3);
						v4x._axis_2d.y++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.str_0x3664C[iy], 20, v3);
						v4x._axis_2d.x++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.str_0x3664C[iy], 21, v3);
						v4x._axis_2d.x++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.str_0x3664C[iy], 22, v3);
						v4x._axis_2d.x++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.str_0x3664C[iy], 23, v3);
						v4x._axis_2d.x++;
						ApplyLightToTile_84EA0(v4x, &world_D41A0.str_0x3664C[iy], 24, v3);
					}
				}
			}
		}
	}
}
*/
int debugcounter_265ea0 = 0;

//----- (00084EA0) --------------------------------------------------------
void ApplyLightToTile_84EA0(uaxis_2d a1x, type_str_0x3664C* a2x, char a3, __int16 a4)//265ea0
{
	signed int v4; // esi
	signed int v5; // edi
	int v6; // ebx
	int v7; // ecx
	//int v8; // edx
	//signed int v9; // ebx
	int v10; // edx
	//int result; // eax
	axis_3d v12; // [esp+0h] [ebp-8h]
	//__int16 v13; // [esp+2h] [ebp-6h]
	//__int16 v14; // [esp+4h] [ebp-4h]

	if (CommandLineParams.DoDebugSequences()) {
		//add_compare(0x265ea4, CommandLineParams.DoDebugafterload(),0x19);
	}

	v4 = 31;
	v5 = (unsigned __int8)mapShading_12B4E0[a1x.word];
	v6 = 0;
	if (v5 < 63)
	{
		v12.x = (a1x._axis_2d.x << 8) + 128;
		v12.y = (a1x._axis_2d.y << 8) + 128;
		v12.z = GetTerrainAlt_10C40(&v12);
		v7 = DistanceSquared3DAlt_84FB0(&a2x->axis3d_4, &v12);//fix v7 = sub_84FB0(a2 + 4, &v12);
		if (v7 < 0x48000)
		{
			if (a2x->flags_1 & 1)
			{
				//a2x->dword_A->dword_0x8
				//v8 = *(x_DWORD *)(a2 + 10);
				//v9 = *(x_DWORD *)(v8 + 8);
				if ((a2x->event_A.pointer_0x6E8E == 0) || a2x->event_A.pointer_0x6E8E->life_0x8 <= 0)
				{
					v4 = 0;
				}
				else if (a2x->event_A.pointer_0x6E8E->life_0x8 < 31)
				{
					v4 = a2x->event_A.pointer_0x6E8E->life_0x8;
				}
			}
			v10 = 31 * (0x48000 - v7) / 0x48000 * a2x->intensity_2;
			//v6 = a4 + ((v10 - (__CFSHL__(v10 >> 31, 7) + (v10 >> 31 << 7))) >> 7);
			v6 = a4 + ((v10 - (my_sign32(v10) << 7) + my_sign32(v10)) >> 7);
			if (v6 > v4)
				v6 = v4;
			if (v5 + v6 > 63)
				v6 = 63 - v5;
			if (v6 < 0)
				v6 = 0;
			mapShading_12B4E0[a1x.word] += v6;
		}
	}
	//result = a2 + a3;
	a2x->shading_E[a3] = v6;
	//*(x_BYTE *)(result + 14) = v6;
	//return result;
}
