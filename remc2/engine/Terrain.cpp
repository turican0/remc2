#include "Terrain.h"
#include "engine_support.h"
#include "CommandLineParser.h"

uint16_t x_WORD_17B4E0; // 34c4e0

uint8_t mapTerrainType_10B4E0[65536]; // 256x256 //map array1
uint8_t mapHeightmap_11B4E0[65536]; // 256x256 //map array2 // heightmap
uint8_t mapShading_12B4E0[65536]; // 256x256 //map array3
uint8_t mapAngle_13B4E0[65536]; // 256x256 //map array4 // water
int16_t mapEntityIndex_15B4E0[65536]; // 256x256 //map array5

char x_BYTE_F2CD0x[7 * 7 * 7 * 7][2]; // 233cd0//4802 //4816

uint8_t x_BYTE_D41B7 = 44; // weak

int8_t unk_D47E0[592] = {//terrain gen
0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x02,0x02,0x02,0x02,0x03,0x03,0x03,0x03,
0x04,0x04,0x04,0x04,0x05,0x05,0x05,0x05,0x06,0x06,0x06,0x06,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x06,0x00,0x01,0x04,
0x01,0x01,0x00,0x00,0x01,0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x00,0x01,0x01,0x01,
0x06,0x06,0x04,0x04,0x06,0x04,0x06,0x04,0x06,0x04,0x06,0x06,0x04,0x06,0x04,0x04,
0x04,0x04,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x04,0x04,0x04,0x00,0x04,0x00,0x04,
0x01,0x03,0x03,0x03,0x01,0x03,0x01,0x03,0x03,0x01,0x01,0x01,0x01,0x01,0x03,0x03,
0x05,0x01,0x01,0x01,0x01,0x01,0x05,0x05,0x01,0x05,0x01,0x05,0x01,0x05,0x05,0x05,
0x02,0x05,0x02,0x05,0x05,0x02,0x02,0x02,0x02,0x05,0x05,0x05,0x05,0x05,0x02,0x02,
0x04,0x04,0x03,0x03,0x04,0x03,0x03,0x03,0x03,0x04,0x03,0x04,0x03,0x04,0x04,0x04,
0x04,0x05,0x05,0x05,0x05,0x04,0x04,0x04,0x05,0x04,0x05,0x04,0x04,0x04,0x05,0x05,
0x01,0x02,0x01,0x02,0x02,0x01,0x01,0x01,0x01,0x02,0x02,0x02,0x01,0x01,0x02,0x02,
0x04,0x01,0x01,0x01,0x01,0x04,0x01,0x04,0x01,0x04,0x04,0x04,0x01,0x01,0x04,0x04,
0x01,0x06,0x01,0x01,0x06,0x06,0x01,0x01,0x06,0x01,0x06,0x01,0x06,0x01,0x06,0x06,
0x06,0x06,0x00,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x06,0x00,0x06,0x00,0x00,
0x02,0x01,0x05,0x01,0x01,0x01,0x05,0x02,0x05,0x01,0x05,0x02,0x02,0x01,0x02,0x05,
0x02,0x02,0x01,0x05,0x05,0x05,0x01,0x02,0x03,0x03,0x04,0x01,0x04,0x03,0x04,0x01,
0x01,0x01,0x04,0x03,0x01,0x04,0x04,0x03,0x03,0x04,0x03,0x01,0x01,0x03,0x01,0x04,
0x01,0x06,0x04,0x06,0x01,0x06,0x01,0x04,0x01,0x06,0x06,0x04,0x01,0x04,0x06,0x04,
0x01,0x06,0x04,0x01,0x01,0x06,0x04,0x04,0x06,0x04,0x00,0x04,0x00,0x04,0x06,0x06,
0x00,0x04,0x00,0x06,0x00,0x00,0x04,0x06,0x00,0x06,0x04,0x04,0x06,0x00,0x06,0x04,
0x06,0x00,0x06,0x01,0x01,0x00,0x06,0x00,0x01,0x06,0x00,0x00,0x01,0x06,0x06,0x00,
0x01,0x06,0x01,0x00,0x01,0x01,0x00,0x06,0x01,0x00,0x04,0x00,0x01,0x04,0x00,0x04,
0x01,0x04,0x00,0x00,0x01,0x01,0x04,0x00,0x04,0x01,0x00,0x04,0x01,0x04,0x01,0x00,
0x01,0x05,0x05,0x04,0x04,0x05,0x04,0x01,0x01,0x01,0x04,0x05,0x01,0x05,0x04,0x05,
0x01,0x04,0x01,0x05,0x01,0x04,0x04,0x05,0x01,0x06,0x00,0x04,0x06,0x01,0x00,0x04,
0x06,0x06,0x05,0x05,0x06,0x05,0x06,0x05,0x06,0x05,0x06,0x06,0x05,0x06,0x05,0x05,
0x06,0x06,0x03,0x03,0x06,0x03,0x06,0x03,0x06,0x03,0x06,0x06,0x03,0x06,0x03,0x03,
0x01,0x05,0x05,0x06,0x06,0x05,0x06,0x01,0x01,0x01,0x06,0x05,0x01,0x05,0x06,0x05,
0x01,0x06,0x01,0x05,0x01,0x06,0x06,0x05,0x01,0x03,0x03,0x06,0x06,0x03,0x06,0x01,
0x01,0x01,0x06,0x03,0x01,0x03,0x06,0x03,0x01,0x06,0x01,0x03,0x01,0x06,0x06,0x03
}; // weak
uint8_t unk_D4A30[288] = {//terrain gen
0x1B,0x00,0x1B,0x50,0x1B,0x30,0x1B,0x60,0x1A,0x00,0x1A,0x50,0x1A,0x30,0x1A,0x60,
0x0A,0x00,0x0A,0x50,0x0A,0x30,0x0A,0x60,0x0A,0x00,0x0A,0x50,0x0A,0x30,0x0A,0x60,
0x0B,0x00,0x0B,0x50,0x0B,0x30,0x0B,0x60,0x0B,0x00,0x0B,0x50,0x0B,0x30,0x0B,0x60,
0x0C,0x00,0x0C,0x50,0x0C,0x30,0x0C,0x60,0x0C,0x00,0x0C,0x50,0x0C,0x30,0x0C,0x60,
0x15,0x00,0x15,0x50,0x15,0x30,0x15,0x60,0x16,0x00,0x16,0x50,0x16,0x30,0x16,0x60,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x18,0x50,0x18,0x30,0x18,0x60,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x17,0x00,0x17,0x50,0x17,0x30,0x17,0x60,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x19,0x00,0x19,0x50,0x19,0x30,0x19,0x60,
0x10,0x00,0x10,0x50,0x10,0x30,0x10,0x60,0x0F,0x00,0x0F,0x50,0x0F,0x30,0x0F,0x60,
0x10,0x00,0x10,0x50,0x10,0x30,0x10,0x60,0x0F,0x00,0x0F,0x50,0x0F,0x30,0x0F,0x60,
0x1E,0x00,0x1E,0x50,0x1E,0x30,0x1D,0x60,0x1F,0x00,0x1D,0x50,0x1D,0x30,0x1D,0x60,
0x1E,0x00,0x1E,0x50,0x1E,0x30,0x1E,0x60,0x1D,0x00,0x1D,0x50,0x1D,0x30,0x1D,0x60,
0x21,0x00,0x21,0x50,0x21,0x30,0x21,0x60,0x22,0x00,0x20,0x50,0x20,0x30,0x20,0x60,
0x21,0x00,0x21,0x50,0x21,0x30,0x21,0x60,0x20,0x00,0x20,0x50,0x20,0x30,0x20,0x60,
0x13,0x00,0x13,0x50,0x13,0x30,0x13,0x60,0x14,0x00,0x12,0x50,0x12,0x30,0x12,0x60,
0x13,0x00,0x13,0x50,0x13,0x30,0x13,0x60,0x12,0x00,0x12,0x50,0x12,0x30,0x12,0x60,
0x13,0x00,0x13,0x50,0x13,0x30,0x13,0x60,0x14,0x00,0x12,0x50,0x12,0x30,0x12,0x60,
0x21,0x00,0x21,0x50,0x21,0x30,0x21,0x60,0x20,0x00,0x20,0x50,0x20,0x30,0x20,0x60 }; // weak


//for debuging
int countcompindexes = 0;
typedef struct {
	int index;
	uint32_t adress;
} type_compstr;

type_compstr compstr[100];
int getcompindex(uint32_t adress) {
	bool finded = false;
	int findindex = 0;
	for (int i = 0; i < countcompindexes; i++)
	{
		if (compstr[i].adress == adress)
		{
			finded = true;
			findindex = i;
			break;
		}
	}
	if (finded)
	{
		compstr[findindex].index++;
		return compstr[findindex].index;
	}
	else
	{
		compstr[countcompindexes].adress = adress;
		countcompindexes++;
		return 0;
	}
};


type_compstr lastcompstr;
void add_compare(uint32_t adress, bool debugafterload, int stopstep, bool skip, int exitindex, int skip2) {
	uint8_t origbyte20 = 0;
	uint8_t remakebyte20 = 0;
	int comp20;

	char buffer1[500];
	sprintf(buffer1, "%08X-002DC4E0", adress);
	char buffer2[500];
	sprintf(buffer2, "%08X-00356038", adress);
	char buffer3[500];
	sprintf(buffer3, "%08X-002B3A74", adress);
	char buffer4[500];
	sprintf(buffer4, "%08X-003AA0A4", adress);

	if (debugafterload)
	{
		int index = getcompindex(adress);
		if (index >= skip2)
		{
			if (index >= stopstep)
			{
				if (index >= exitindex)
					exit(exitindex);
				if (!skip)
				{
					comp20 = compare_with_sequence(buffer1, (uint8_t*)mapTerrainType_10B4E0, 0x2dc4e0, index - skip2, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0, (exitindex != 1000000));
					comp20 = compare_with_sequence(buffer1, (uint8_t*)mapHeightmap_11B4E0, 0x2dc4e0, index - skip2, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x10000, (exitindex != 1000000));
					comp20 = compare_with_sequence(buffer1, (uint8_t*)mapShading_12B4E0, 0x2dc4e0, index - skip2, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x20000, (exitindex != 1000000));
					comp20 = compare_with_sequence(buffer1, (uint8_t*)mapAngle_13B4E0, 0x2dc4e0, index - skip2, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x30000, (exitindex != 1000000));
					comp20 = compare_with_sequence(buffer1, (uint8_t*)mapEntityIndex_15B4E0, 0x2dc4e0, index - skip2, 0x70000, 0x20000, &origbyte20, &remakebyte20, 0x50000, (exitindex != 1000000));

#ifdef TEST_x64
					type_shadow_D41A0_BYTESTR_0 shadow_D41A0_BYTESTR_0;
					Convert_to_shadow_D41A0_BYTESTR_0(&D41A0_0, &shadow_D41A0_BYTESTR_0);
					comp20 = compare_with_sequence_D41A0(buffer2, (uint8_t*)&shadow_D41A0_BYTESTR_0, 0x356038, index - skip2, 224790, &origbyte20, &remakebyte20, 0, (exitindex != 1000000));
#else
					comp20 = compare_with_sequence_D41A0(buffer2, (uint8_t*)&D41A0_0, 0x356038, index, 224790, &origbyte20, &remakebyte20, 0, (exitindex != 1000000));
#endif

					// FIXME: skip the test of str_E2A74 for the moment as there are differences with the memimage that need to be clarified
					//type_shadow_str_E2A74 shadow_str_E2A74[0x69];
					//Convert_to_shadow_str_E2A74(str_E2A74, shadow_str_E2A74);
					//comp20 = compare_with_sequence_array_E2A74(buffer3, (uint8_t*)&shadow_str_E2A74, 0x2b3a74, index - skip2, 0xc4e, 0xc4e, &origbyte20, &remakebyte20, 0, (exitindex != 1000000));

					//screen
					//comp20 = compare_with_sequence(buffer4, pdwScreenBuffer_351628, 0x3aa0a4, index, 320 * 200, 320 * 200, &origbyte20, &remakebyte20);
				}
				//if(debugcounter_271478>5)
				//comp20 = compare_with_sequence(buffer4, pdwScreenBuffer_351628, 0x3aa0a4, index, 320 * 200, 320 * 200, &origbyte20, &remakebyte20);
				if (stopstep > -1)
				{
					comp20 = index;
				}

				lastcompstr.index = index;
				lastcompstr.adress = adress;
			}
		}
	}
};
//for debuging

void add_compare2(uint32_t adress, uint8_t* memadress,uint32_t dosmemadress, uint32_t size,bool debugafterload, int stopstep, bool skip, int exitindex) {
	uint8_t origbyte20 = 0;
	uint8_t remakebyte20 = 0;
	int comp20;

	char buffer1[500];
	sprintf(buffer1, "%08X-08X", adress, dosmemadress);

	if (debugafterload)
	{
		int index = getcompindex(adress);
		if (index >= stopstep)
		{
			if (index >= exitindex)
				exit(exitindex);
			if (!skip)
			{
				comp20 = compare_with_sequence(buffer1, memadress, dosmemadress, index, size, size, &origbyte20, &remakebyte20);
			}
			if (stopstep > -1)
			{
				comp20 = index;
			}

			lastcompstr.index = index;
			lastcompstr.adress = adress;
		}
	}
};
//for debuging


int debugcounter_224959 = 0;
//----- (00043830) --------------------------------------------------------
void GenerateLevelMap_43830(type_str_2FECE* a2x)//224830
{
	x_WORD_17B4E0 = a2x->seed_0x2FEE5;
	D41A0_0.rand_0x8 = a2x->seed_0x2FEE5;
	memset((void*)mapEntityIndex_15B4E0, 0, 0x20000);
	sub_B5E70_decompress_terrain_map_level(x_WORD_17B4E0, a2x->offset_0x2FEE9, a2x->raise_0x2FEED, a2x->gnarl_0x2FEF1);

	sub_44DB0_truncTerrainHeight();//225db0 //trunc and create

	memset((void*)mapEntityIndex_15B4E0, 0, 0x20000);
	sub_44E40(a2x->river_0x2FEF5, a2x->lriver_0x2FEF9);//225e40 //add any fields

	sub_45AA0_setMax4Tiles();//226aa0

	sub_440D0(a2x->snLin_0x2FF01);//2250d0

	sub_45060(a2x->snFlt_0x2FF05, a2x->bhLin_0x2FF09);//226060

	sub_44320();//225320

	sub_45210(a2x->snFlt_0x2FF05, a2x->bhLin_0x2FF09);//226210

	sub_454F0(a2x->source_0x2FEFD, a2x->rkSte_0x2FF11);//2264f0

	sub_45600(a2x->bhFlt_0x2FF0D);//226600

	sub_43FC0();//224fc0
	memset((void*)mapTerrainType_10B4E0, 0, 0x10000);
	sub_43970();//224970 // smooth terrain
	sub_43EE0();//224ee0 // add rivers

	sub_44580();//225580 //set angle of terrain
	if (isCaveLevel_D41B6)
		sub_43B40();//224b40 //change angle of terrain
	else
		sub_43D50();//224d50 //change angle of terrain
	sub_44D00();//225d00
}

//----- (000B5E70) --------------------------------------------------------
void sub_B5E70_decompress_terrain_map_level(__int16 a1, unsigned __int16 a2, __int16 a3, int32_t a4)//296e70
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
				sub_B5EFA(1 << i, &sumEnt, a4, &a1);//355220
			}
			sumEnt.word += (2 * (1 << i)) << 8;
		}
		for (int j = 1 << (7 - i); j > 0; j--)
		{
			for (int k = 1 << (7 - i); k > 0; k--)
			{
				sub_B5F8F(1 << i, &sumEnt, a4, &a1);
			}
			sumEnt.word += (2 * (1 << i)) << 8;
		}
	}
}

//----- (00044DB0) --------------------------------------------------------
void sub_44DB0_truncTerrainHeight()//225db0 // map to heightmap
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

int sub_B5C60_getTerrainAlt2(uint16_t axisX, uint16_t axisY)//296c60
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
void sub_44E40(int count, uint8_t minSmooth)//225e40
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
			x_WORD_17B4E0 = 9377 * x_WORD_17B4E0 + 9439;
			index.word = x_WORD_17B4E0 % 0xffffu;
			if ((mapHeightmap_11B4E0[index.word] > minSmooth) && mapAngle_13B4E0[index.word])
			{
				sub_44EE0_smooth_tiles(index);
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
void sub_45AA0_setMax4Tiles()//226aa0
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
void sub_440D0(unsigned __int16 a1)//2250d0
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
void sub_45060(uint8_t maxHeightCut, uint8_t maxHeightDiffCut)//226060
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
void sub_44320()//225320
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
void sub_45210(uint8_t maxHeightCut, uint8_t maxHeightDiffCut)//226210
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
void sub_454F0(uint8_t maxHeightCut, uint8_t maxHeightDiffCut)//2264f0
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
void sub_45600(uint8_t a1)//226600
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
void sub_43FC0()//224fc0
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
void sub_43970()//224970
{
	uaxis_2d index;
	for (int i = 0; i < 256 * 256; i++)
	{
		index.word = i;
		mapHeightmap_11B4E0[index.word] = sub_439A0(index.word);
	}
}

//----- (00043EE0) --------------------------------------------------------
void sub_43EE0()//224ee0
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
void sub_44580()//225580
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
						x_BYTE_F2CD0x[index][0] = actBufPos[1];
						x_BYTE_F2CD0x[index][1] = actBufPos[13];
					}
					else
					{
						x_BYTE_F2CD0x[index][0] = 1;
						x_BYTE_F2CD0x[index][1] = 0;
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
				x_WORD_17B4E0 = 9377 * x_WORD_17B4E0 + 9439;
				if (x_WORD_17B4E0 % (actBufEnt + 1) >= actBufEnt)
					actBufPos = &pdwScreenBuffer_351628[25 * (343 * point1 + 49 * point2 + point4 + 7 * point3)];
				else
					actBufPos = &pdwScreenBuffer_351628[x_WORD_17B4E0 % (actBufEnt + 1) + 25 * (343 * point1 + 49 * point2 + point4 + 7 * point3)];
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
void sub_43B40()//224b40
{
	uint8_t locHeight;
	uaxis_2d index;
	for (int i = 0; i < 256 * 256; i++)
	{
		index.word = i;
		locHeight = mapHeightmap_11B4E0[index.word];
		if (locHeight > x_BYTE_D41B7)
			locHeight = x_BYTE_D41B7;
		x_BYTE_14B4E0_second_heightmap[index.word] = x_BYTE_D41B7 - locHeight;
		if (x_BYTE_D41B7 - locHeight > mapHeightmap_11B4E0[index.word])
		{
			mapAngle_13B4E0[index.word] &= 0xF7u;
		}
		else
		{
			x_BYTE_14B4E0_second_heightmap[index.word] = mapHeightmap_11B4E0[index.word] - 1;
			mapAngle_13B4E0[index.word] |= 8;
		}
	}
	sub_43BB0();
}

//----- (00043D50) --------------------------------------------------------
void sub_43D50()//224d50
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
void sub_44D00()//225d00
{

	//     X
	//    /
	//   B
	//  /
	// X

	uaxis_2d tempIndex;
	uaxis_2d index;
	x_WORD_17B4E0 = 0;
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
			tempIndex.word = 9377 * x_WORD_17B4E0 + 9439;
			x_WORD_17B4E0 = tempIndex.word;
			tempIndex._axis_2d.y = (x_WORD_17B4E0 / 9u) >> 8;
			tempIndex._axis_2d.x = x_WORD_17B4E0 % 9 + 28;
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
		if (D41A0_0.terrain_2FECE.MapType != MapType_t::Day)
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
void sub_B5EFA(__int16 a1, uaxis_2d* indexx, int32_t a3, int16_t* nextRand)//296EFA
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
void sub_B5F8F(__int16 a1, uaxis_2d* indexx, int32_t a3, int16_t* nextRand)//296f8f
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
void sub_44EE0_smooth_tiles(uaxis_2d axis)//225ee0
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
unsigned int sub_439A0(uint16_t index)//2249a0
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
void sub_43BB0()//224bb0
{
	int fuzzyHeight;
	unsigned int randSeed = 37487429;
	for (int i = 0; i < 256 * 256; i++)
	{
		if (!(mapAngle_13B4E0[i] & 8))
		{
			randSeed = 9377 * randSeed + 9439;
			fuzzyHeight = randSeed % 7 - 3 + x_BYTE_14B4E0_second_heightmap[i];
			if (fuzzyHeight < 0)
				fuzzyHeight = 0;
			if (fuzzyHeight > 254)
				fuzzyHeight = 254;
			x_BYTE_14B4E0_second_heightmap[i] = fuzzyHeight;
		}
	}
	for (int i = 0; i < 256 * 256; i++)
	{
		if (x_BYTE_14B4E0_second_heightmap[i] > mapHeightmap_11B4E0[i])
		{
			mapAngle_13B4E0[i] &= 0xF7u;
		}
		else
		{
			x_BYTE_14B4E0_second_heightmap[i] = mapHeightmap_11B4E0[i] - 1;
			mapAngle_13B4E0[i] |= 8;
		}
	}
}

//----- (000104A0) --------------------------------------------------------
signed int sub_104A0(axis_3d* a1x)//1f14a0
{
	uint16_t v1; // ax

	v1 = (a1x->x >> 8) + ((a1x->y >> 8) << 8);
	return 1 << (mapAngle_13B4E0[v1] & 0xF);
}

//----- (000104D0) --------------------------------------------------------
uint32_t sub_104D0_terrain_tile_is_water(axis_3d* a1)//1f14d0
{
	uaxis_2d v1x; // eax

	v1x._axis_2d.x = a1->x >> 8;
	v1x._axis_2d.y = a1->y >> 8;
	return sub_10590_terrain_tile_type(mapTerrainType_10B4E0[v1x.word]);
}

//----- (00010590) --------------------------------------------------------
uint32_t sub_10590_terrain_tile_type(char a1)//1f1590
{
	uint32_t result; // eax

	result = 0;
	switch (a1)
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
// 10000: using guessed type int sub_10000();

//----- (00010C40) --------------------------------------------------------
int getTerrainAlt_10C40(axis_3d* a1x)//1f1c40
{
	return sub_B5C60_getTerrainAlt2(a1x->x, a1x->y);
}

//----- (00010C60) --------------------------------------------------------
int sub_10C60(axis_3d* a1)//1f1c60
{
	return sub_B5D68(a1->x, a1->y);
}

//----- (00011E70) --------------------------------------------------------
bool sub_11E70(type_event_0x6E8E* a1x, axis_3d* a2)//1f2e70
{
	int v2; // ebx
	int v3; // ebx

	v2 = a1x->dword_0xA0_160x->word_160_0xc_12;
	v3 = a1x->array_0x52_82.fov + (signed __int16)getTerrainAlt_10C40(a2) + v2;
	return v3 > (signed __int16)sub_10C60(a2);
}

//----- (0001B7A0) --------------------------------------------------------
int sub_1B7A0_tile_compare(axis_3d* a1)//1fc7a0
{
	uint8_t v1x[2]; // eax
	//uint8_t v2x[2]; // dx
	//uint8_t v3x[2]; // bx
	unsigned __int8 v4_00; // cl
	unsigned __int8 v4_01; // cl
	unsigned __int8 v4_10; // cl
	unsigned __int8 v4_11; // cl

	//uint8_t v5x[2]; // bx
	//int v6; // esi
	//int v7; // ebx
	int v8; // ecx
	int v9; // ebx

	v1x[0] = a1->x >> 8;
	v1x[1] = a1->y >> 8;

	v4_00 = mapHeightmap_11B4E0[*(uint16_t*)v1x];//0000
	v1x[0]++;
	v4_10 = mapHeightmap_11B4E0[*(uint16_t*)v1x];//+100
	v1x[1]++;
	v4_11 = mapHeightmap_11B4E0[*(uint16_t*)v1x];//+1+1
	v1x[0]--;
	v4_01 = mapHeightmap_11B4E0[*(uint16_t*)v1x];//00+1

	//v7 = v4_00 + v4_10 - v4_01;
	v8 = abs(v4_00 + v4_01 - v4_10 - v4_11);
	v9 = abs(v4_00 + v4_10 - v4_01 - v4_11);
	if (v8 > v9)
		v9 = v8;
	return v9;
}

//----- (0001B830) --------------------------------------------------------
int sub_1B830(axis_3d* a1)//1fc830
{
	unsigned __int16 v1; // ax
	unsigned __int16 v2; // dx
	unsigned __int16 v3; // bx
	unsigned __int8 v4; // cl
	unsigned __int16 v5; // bx
	int v6; // esi
	int v7; // ebx
	int v8; // ecx
	int v9; // ebx

	LOBYTE(v1) = (a1->x >> 8) - 1;
	HIBYTE(v1) = (a1->y >> 8) - 1;
	v2 = v1;
	LOBYTE(v1) = v1 + 2;
	v3 = v1;
	HIBYTE(v1) += 2;
	v4 = mapHeightmap_11B4E0[v3];
	v5 = v1;
	LOBYTE(v2) = mapHeightmap_11B4E0[v2];
	LOBYTE(v1) = v1 - 2;
	HIBYTE(v2) = mapHeightmap_11B4E0[v5];
	v6 = mapHeightmap_11B4E0[v1];
	v7 = (unsigned __int8)v2 + v4 - v6;
	v8 = abs((unsigned __int8)v2 + v6 - v4 - HIBYTE(v2));
	v9 = abs(v7 - HIBYTE(v2));
	if (v8 > v9)
		v9 = v8;
	return v9;
}

//----- (00033F70) --------------------------------------------------------
bool sub_33F70(uint16_t a1)//214f70
{
	unsigned __int16 v1; // ax
	signed int v2; // edx
	unsigned __int16 v3; // ax
	bool result; // al

	HIBYTE(v1) = HIBYTE(a1);
	LOBYTE(v1) = a1 - 1;
	result = 1;
	if (mapTerrainType_10B4E0[v1] == 8)
	{
		v2 = (unsigned __int16)(mapHeightmap_11B4E0[a1] + 30);
		if (mapHeightmap_11B4E0[v1] <= (signed int)(unsigned __int16)v2)
		{
			LOBYTE(v1) = v1 + 2;
			if (mapHeightmap_11B4E0[v1] <= v2)
			{
				LOBYTE(v3) = v1 - 1;
				HIBYTE(v3) = HIBYTE(a1) + 1;
				if (mapHeightmap_11B4E0[v3] <= v2)
				{
					HIBYTE(v3) = HIBYTE(a1) - 1;
					if (mapHeightmap_11B4E0[v3] <= v2)
						result = 0;
				}
			}
		}
	}
	return result;
}

//----- (00045BE0) --------------------------------------------------------
uint8_t sub_45BE0(uint8_t a2, uaxis_2d a3x)//226be0
{
	uaxis_2d v3x; // ax
	uint8_t v4; // bl
	uint8_t v5; // dh
	uint8_t v6; // bh
	uint8_t v7; // al
	uint32_t v8; // edi
	uint8_t result; // al

	uint8_t a1 = 0;

	v3x.word = a3x.word;
	v4 = 0xFFu;
	v5 = 0;
	if (mapHeightmap_11B4E0[a3x.word])
	{
		v5 = mapHeightmap_11B4E0[a3x.word];
		a1 = 0;
	}
	if (mapHeightmap_11B4E0[a3x.word] < 0xFFu)
		v4 = mapHeightmap_11B4E0[a3x.word];
	v3x._axis_2d.x++;
	if (mapHeightmap_11B4E0[v3x.word] > v5)
	{
		v5 = mapHeightmap_11B4E0[v3x.word];
		a1 = 1;
	}
	if (mapHeightmap_11B4E0[v3x.word] < v4)
		v4 = mapHeightmap_11B4E0[v3x.word];
	v3x._axis_2d.y++;
	if (mapHeightmap_11B4E0[v3x.word] > v5)
	{
		v5 = mapHeightmap_11B4E0[v3x.word];
		a1 = 2;
	}
	if (mapHeightmap_11B4E0[v3x.word] < v4)
		v4 = mapHeightmap_11B4E0[v3x.word];
	v3x._axis_2d.x--;
	if (mapHeightmap_11B4E0[v3x.word] > v5)
	{
		v5 = mapHeightmap_11B4E0[v3x.word];
		a1 = 3;
	}
	if (mapHeightmap_11B4E0[v3x.word] < v4)
		v4 = mapHeightmap_11B4E0[v3x.word];
	v3x._axis_2d.y--;
	v6 = 0;
	if (a1 && mapHeightmap_11B4E0[v3x.word])
	{
		v6 = mapHeightmap_11B4E0[v3x.word];
		a2 = 0;
	}
	v3x._axis_2d.x++;
	if (a1 != 1 && mapHeightmap_11B4E0[v3x.word] > v6)
	{
		v6 = mapHeightmap_11B4E0[v3x.word];
		a2 = 1;
	}
	v3x._axis_2d.y++;
	if (a1 != 2 && mapHeightmap_11B4E0[v3x.word] > v6)
	{
		v6 = mapHeightmap_11B4E0[v3x.word];
		a2 = 2;
	}
	v3x._axis_2d.x--;
	if (a1 != 3)
	{
		v7 = mapHeightmap_11B4E0[v3x.word];
		if (v7 > v6)
		{
			a2 = 3;
			v6 = v7;
		}
	}
	v8 = 0;
	if (v5 - v4 <= 8)
		v8 = 1;
	if (v5 - v6 >= 8)
	{
		result = a1;
	LABEL_44:
		lowDiffHeightmap_D47DC = v8;
		return result;
	}
	if (a1 > 3u)
	{
		result = 0;
		goto LABEL_44;
	}
	switch (a1)
	{
	case 0:
		if (a2 != 1)
			goto LABEL_42;
		goto LABEL_36;
	case 1:
		if (a2 == 2)
			goto LABEL_38;
	LABEL_36:
		result = 4;
		lowDiffHeightmap_D47DC = v8;
		return result;
	case 2:
		if (a2 == 3)
			goto LABEL_40;
	LABEL_38:
		result = 5;
		lowDiffHeightmap_D47DC = v8;
		break;
	case 3:
		if (a2)
		{
		LABEL_40:
			result = 6;
			lowDiffHeightmap_D47DC = v8;
		}
		else
		{
		LABEL_42:
			result = 7;
			lowDiffHeightmap_D47DC = v8;
		}
		break;
	}
	return result;
}

//----- (00045DC0) --------------------------------------------------------
void sub_45DC0(uint8_t a2, uaxis_2d a3x, unsigned __int8 a4)//226dc0
{
	uaxis_2d v4x; // bx
	uaxis_2d v5x; // eax
	char v6; // ch
	uint8_t* v7; // eax
	char v8; // al
	char v9; // ah
	unsigned __int8 v10; // al
	char v11; // cl
	unsigned __int8 v12; // dh
	unsigned __int8 v13; // dh
	char v14; // cl
	unsigned __int8 v15; // dh
	char v16; // cl
	unsigned __int8 v17; // dh
	char v18; // bl
	uaxis_2d v19x; // ax
	char v20; // ch
	uaxis_2d v21x; // ax
	char v22; // dl
	unsigned __int8 v24; // [esp+0h] [ebp-4h]

	if (CommandLineParams.DoDebugSequences()) {
		//add_compare(0x226dc4, CommandLineParams.DoDebugafterload());//1cf0
	}

	v4x._axis_2d.y = a3x._axis_2d.y;
	v24 = 8 * ((a3x._axis_2d.y + a3x._axis_2d.x) & 1);
	if (a4 >= 8u)
	{
		switch (a4)
		{
		case 8u:
			mapTerrainType_10B4E0[a3x.word] = 8;
			break;
		case 9u:
			mapTerrainType_10B4E0[a3x.word] = 9;
			break;
		case 0xAu:
			v8 = sub_45BE0(a2, a3x);
			v9 = v8;
			if (lowDiffHeightmap_D47DC)
				v9 = v8 + 8;
			v10 = v9;
			goto LABEL_28;
		case 0xBu:
			v10 = sub_45BE0(a2, a3x) + 16;
			if (lowDiffHeightmap_D47DC)
				v10 += 8;
			goto LABEL_28;
		case 0xCu:
			v10 = sub_45BE0(a2, a3x) + 32;
			if (lowDiffHeightmap_D47DC)
				v10 += 8;
			goto LABEL_28;
		case 0xDu:
			v10 = sub_45BE0(a2, a3x) + 48;
			if (lowDiffHeightmap_D47DC)
				v10 += 8;
			goto LABEL_28;
		case 0xEu:
			v10 = sub_45BE0(a2, a3x) + 64;
			if (lowDiffHeightmap_D47DC)
				v10 += 8;
		LABEL_28:
			v7 = &unk_D4A30[0x80] + 2 * v10;
			goto LABEL_29;
		case 0xFu:
			mapTerrainType_10B4E0[a3x.word] = 11;
			break;
		case 0x10u:
			v6 = mapTerrainType_10B4E0[a3x.word];
			if (v6 == 10 || v6 == 11 || v6 == 12)
				break;
			v7 = unk_D4A30 + 2 * sub_45BE0(a2, a3x);
			goto LABEL_29;
		case 0x11u:
			v7 = &unk_D4A30[64] + 2 * sub_45BE0(a2, a3x);
			goto LABEL_29;
		case 0x12u:
			v7 = &unk_D4A30[80] + 2 * (v24 + sub_45BE0(a2, a3x));
			goto LABEL_29;
		case 0x13u:
			v7 = &unk_D4A30[80] + 2 * (sub_45BE0(a2, a3x) + v24) + 16;
			goto LABEL_29;
		case 0x14u:
			v7 = &unk_D4A30[16] + 2 * sub_45BE0(a2, a3x);
			goto LABEL_29;
		case 0x15u:
			v7 = &unk_D4A30[16] + 2 * sub_45BE0(a2, a3x) + 16;
			goto LABEL_29;
		case 0x16u:
			v7 = &unk_D4A30[16] + 2 * sub_45BE0(a2, a3x) + 32;
		LABEL_29:
			mapTerrainType_10B4E0[a3x.word] = v7[0];
			mapAngle_13B4E0[a3x.word] = mapAngle_13B4E0[a3x.word] & 0x8F | v7[1];
			break;
		default:
			break;
		}
		v11 = mapAngle_13B4E0[a3x.word] | 0x80;
		mapAngle_13B4E0[a3x.word] = v11;
		if (isCaveLevel_D41B6)
		{
			v12 = mapHeightmap_11B4E0[a3x.word];
			if (x_BYTE_14B4E0_second_heightmap[a3x.word] > v12)
			{
				mapAngle_13B4E0[a3x.word] = v11 & 0xF7;
			}
			else
			{
				mapAngle_13B4E0[a3x.word] = v11 | 8;
				x_BYTE_14B4E0_second_heightmap[a3x.word] = v12 - 1;
			}
			v4x._axis_2d.x = a3x._axis_2d.x + 1;
			v13 = mapHeightmap_11B4E0[v4x.word];
			if (x_BYTE_14B4E0_second_heightmap[v4x.word] > v13)
			{
				mapAngle_13B4E0[v4x.word] &= 0xF7u;
			}
			else
			{
				v14 = mapAngle_13B4E0[v4x.word] | 8;
				x_BYTE_14B4E0_second_heightmap[v4x.word] = v13 - 1;
				mapAngle_13B4E0[v4x.word] = v14;
			}
			v4x._axis_2d.y = a3x._axis_2d.y + 1;
			v15 = mapHeightmap_11B4E0[v4x.word];
			if (x_BYTE_14B4E0_second_heightmap[v4x.word] > v15)
			{
				mapAngle_13B4E0[v4x.word] &= 0xF7u;
			}
			else
			{
				v16 = mapAngle_13B4E0[v4x.word] | 8;
				x_BYTE_14B4E0_second_heightmap[v4x.word] = v15 - 1;
				mapAngle_13B4E0[v4x.word] = v16;
			}
			v4x._axis_2d.x = a3x._axis_2d.x;
			v5x.word = v4x.word;
			v17 = mapHeightmap_11B4E0[v4x.word];
			if (x_BYTE_14B4E0_second_heightmap[v4x.word] <= v17)
			{
				v18 = mapAngle_13B4E0[v4x.word] | 8;
				x_BYTE_14B4E0_second_heightmap[v5x.word] = v17 - 1;
				mapAngle_13B4E0[v5x.word] = v18;
				return;
			}
		}
		else
		{
			v4x._axis_2d.x = a3x._axis_2d.x + 1;
			mapAngle_13B4E0[a3x.word] = v11 & 0xF7;
			v19x.word = v4x.word;
			v20 = mapAngle_13B4E0[v4x.word] & 0xF7;
			v4x._axis_2d.y = a3x._axis_2d.y + 1;
			mapAngle_13B4E0[v19x.word] = v20;
			v21x.word = v4x.word;
			v22 = mapAngle_13B4E0[v4x.word] & 0xF7;
			v4x._axis_2d.x = a3x._axis_2d.x;
			mapAngle_13B4E0[v21x.word] = v22;
			v5x.word = v4x.word;
		}
		mapAngle_13B4E0[v5x.word] &= 0xF7u;
		return;
	}
	mapAngle_13B4E0[a3x.word] = a4 | mapAngle_13B4E0[a3x.word] & 0xF0;
	sub_462A0(a3x, a3x);
}

//----- (000462A0) --------------------------------------------------------
void sub_462A0(uaxis_2d a1x, uaxis_2d a2x)//2272a0
{
	uaxis_2d v2x; // cx
	bool v3; // zf
	uaxis_2d v4x; // eax
	uint8_t v4y;
	uaxis_2d v5x; // cx
	int v6; // edx
	int v8; // edx
	int v9; // ebx
	int v10; // eax
	int v11index; // edx
	char v12; // ST10_1
	uaxis_2d v13x; // cx
	char v14; // dh
	char j; // dl
	uaxis_2d v16x; // ax
	uaxis_2d v17x; // bx
	char v18; // al
	char v19; // bl
	char i; // [esp+0h] [ebp-14h]
	char v23; // [esp+4h] [ebp-10h]
	char v24; // [esp+4h] [ebp-10h]
	char v25; // [esp+8h] [ebp-Ch]
	char v26; // [esp+8h] [ebp-Ch]
	char v27; // [esp+8h] [ebp-Ch]
	char v28; // [esp+Ch] [ebp-8h]

	if (CommandLineParams.DoDebugSequences()) {
		//add_compare(0x2272a3, CommandLineParams.DoDebugafterload());
	}
	v4x._axis_2d.x = a2x._axis_2d.x - a1x._axis_2d.x + 1;
	v2x.word = a1x.word;
	v25 = a2x._axis_2d.x - a1x._axis_2d.x + 1;
	v4x._axis_2d.y = a2x._axis_2d.y - a1x._axis_2d.y + 1;
	v3 = v4x._axis_2d.y == 0;
	v23 = a2x._axis_2d.y - a1x._axis_2d.y + 1;
	while (v4x._axis_2d.y != 0)
	{
		v4x._axis_2d.x = a2x._axis_2d.x - a1x._axis_2d.x + 1;
		while (v4x._axis_2d.x)
		{
			if ((int8_t)mapAngle_13B4E0[v2x.word] >= 0)
				mapTerrainType_10B4E0[v2x.word] = 1;
			v2x._axis_2d.x--;
			if ((int8_t)mapAngle_13B4E0[v2x.word] >= 0)
				mapTerrainType_10B4E0[v2x.word] = 1;
			v2x._axis_2d.y--;
			if ((int8_t)mapAngle_13B4E0[v2x.word] >= 0)
				mapTerrainType_10B4E0[v2x.word] = 1;
			v2x._axis_2d.x++;
			if ((int8_t)mapAngle_13B4E0[v2x.word] >= 0)
				mapTerrainType_10B4E0[v2x.word] = 1;
			v2x._axis_2d.y++;
			v4x._axis_2d.x--;
			v2x._axis_2d.x++;
		}
		v2x._axis_2d.x -= v25;
		v2x._axis_2d.y++;
		v3 = --v4x._axis_2d.y == 0;
	}
	a1x._axis_2d.x--;
	a1x._axis_2d.y--;
	v24 = v23 + 1;
	v28 = v24;
	v5x.word = a1x.word;
	v26 = v25 + 1;
	while (v28)
	{
		v4x._axis_2d.x = v26;
		for (i = v26; i; i--)
		{
			v4x.word = v5x.word;
			if (mapTerrainType_10B4E0[v5x.word] == 1)
			{
				v6 = 343 * (mapAngle_13B4E0[v4x.word] & 7);
				v5x._axis_2d.x++;
				v8 = 49 * (mapAngle_13B4E0[v5x.word] & 7) + v6;
				v5x._axis_2d.y++;
				v9 = mapAngle_13B4E0[v5x.word] & 7;
				v5x._axis_2d.x--;
				v10 = mapAngle_13B4E0[v5x.word] & 7;
				v5x._axis_2d.y--;
				v11index = v10 + 7 * v9 + v8;
				mapTerrainType_10B4E0[v5x.word] = x_BYTE_F2CD0x[v11index][0];
				if (x_BYTE_F2CD0x[v11index][0] >= 8u)
				{
					v4y = x_BYTE_F2CD0x[v11index][1] + (mapAngle_13B4E0[v5x.word] & 0x87);
				}
				else
				{
					v12 = mapAngle_13B4E0[v5x.word] & 0x87;
					x_WORD_17B4E0 = 9377 * x_WORD_17B4E0 + 9439;
					v4y = v12 + 16 * (x_WORD_17B4E0 % 7u);
				}
				mapAngle_13B4E0[v5x.word] = v4y;
			}
			v5x._axis_2d.x++;
		}
		v5x._axis_2d.x -= v26;
		v5x._axis_2d.y++;
		v28--;
	}
	v27 = v26 + 1;
	v13x.word = a1x.word;//fixed
	v14 = v24 + 1;
	if (v24 != -1)
	{
		do
		{
			for (j = v27; j; j--)
			{
				v13x._axis_2d.x++;
				v13x._axis_2d.y++;
				v16x.word = v13x.word;
				v13x._axis_2d.x -= 2;
				v13x._axis_2d.y -= 2;
				v17x.word = v13x.word;
				v13x._axis_2d.x++;
				v18 = mapHeightmap_11B4E0[v17x.word] - mapHeightmap_11B4E0[v16x.word] + 32;
				v13x._axis_2d.y++;
				if (v18 >= 28)
				{
					if (v18 > 40)
						v18 = (v18 & 7) + 40;
				}
				else
				{
					v18 = (v18 & 3) + 28;
				}
				if (D41A0_0.terrain_2FECE.MapType != MapType_t::Day)
					v19 = 32 - v18 + 32;
				else
					v19 = v18;
				v4x.word = v13x.word;
				mapShading_12B4E0[v13x.word] = v19;
				if (isCaveLevel_D41B6 && (x_BYTE_14B4E0_second_heightmap[v13x.word] <= mapHeightmap_11B4E0[v13x.word]))
				{
					x_BYTE_14B4E0_second_heightmap[v13x.word] = mapHeightmap_11B4E0[v13x.word] - 1;
					mapAngle_13B4E0[v13x.word] |= 8u;
				}
				else
				{
					mapAngle_13B4E0[v13x.word] &= 0xF7u;
				}
				v13x._axis_2d.x++;
			}
			v13x._axis_2d.x -= v27;
			v14--;
			v13x._axis_2d.y++;
		} while (v14);
	}
}

//----- (000B5D68) --------------------------------------------------------
int sub_B5D68(__int16 a1, __int16 a2)//296d68
{
	int v2; // ebx
	int v3; // edx
	int v4; // eax
	int v5; // esi
	int v6; // esi
	int result; // eax
	int v8; // esi
	int v9; // eax
	int v10; // esi
	int v11; // esi
	int v12; // edi

	v2 = HIBYTE(a1);
	HIBYTE(v2) = HIBYTE(a2);
	v3 = (unsigned __int8)a2;
	if ((HIBYTE(a1) + HIBYTE(a2)) & 1)
	{
		if (__CFADD__((x_BYTE)v3, (x_BYTE)a1))
		{
			HIBYTE(v2) = HIBYTE(a2) + 1;
			v4 = (unsigned __int8)x_BYTE_14B4E0_second_heightmap[v2];
			LOBYTE(v2) = v2 + 1;
			v8 = (unsigned __int8)x_BYTE_14B4E0_second_heightmap[v2];
			LOBYTE(v3) = ~(x_BYTE)a2;
			BYTE1(v2) = HIBYTE(a2);
			v6 = v3 * ((unsigned __int8)x_BYTE_14B4E0_second_heightmap[v2] - v8) + (unsigned __int8)a1 * (v8 - v4);
		}
		else
		{
			v4 = (unsigned __int8)x_BYTE_14B4E0_second_heightmap[v2];
			LOBYTE(v2) = v2 + 1;
			v5 = (unsigned __int8)a1 * ((unsigned __int8)x_BYTE_14B4E0_second_heightmap[v2] - v4);
			LOBYTE(v2) = v2 - 1;
			HIBYTE(v2) = HIBYTE(a2) + 1;
			v6 = (unsigned __int8)a2 * ((unsigned __int8)x_BYTE_14B4E0_second_heightmap[v2] - v4) + v5;
		}
		result = (v6 >> 3) + 32 * v4;
	}
	else
	{
		if ((unsigned __int8)a1 <= (unsigned __int8)v3)
		{
			v9 = (unsigned __int8)x_BYTE_14B4E0_second_heightmap[v2];
			HIBYTE(v2) = HIBYTE(a2) + 1;
			v12 = (unsigned __int8)x_BYTE_14B4E0_second_heightmap[v2];
			LOBYTE(v2)++;
			v11 = (unsigned __int8)a2 * (v12 - v9) + (unsigned __int8)a1 * ((unsigned __int8)x_BYTE_14B4E0_second_heightmap[v2] - v12);
		}
		else
		{
			v9 = (unsigned __int8)x_BYTE_14B4E0_second_heightmap[v2];
			LOBYTE(v2)++;
			v10 = (unsigned __int8)x_BYTE_14B4E0_second_heightmap[v2];
			HIBYTE(v2) = HIBYTE(a2) + 1;
			v11 = (unsigned __int8)a2 * ((unsigned __int8)x_BYTE_14B4E0_second_heightmap[v2] - v10) + (unsigned __int8)a1 * (v10 - v9);
		}
		result = (v11 >> 3) + 32 * v9;
	}
	return result;
}

