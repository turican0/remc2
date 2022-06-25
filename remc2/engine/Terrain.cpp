#include "Terrain.h"
#include "engine_support.h"

uint16_t x_WORD_17B4E0; // 34c4e0

uint8_t x_BYTE_10B4E0_terraintype[65536]; // 256x256 //map array1
uint8_t x_BYTE_11B4E0_heightmap[65536]; // 256x256 //map array2 // heightmap
uint8_t x_BYTE_12B4E0_shading[65536]; // 256x256 //map array3
uint8_t x_BYTE_13B4E0_angle[65536]; // 256x256 //map array4 // water
int16_t mapEntityIndex_15B4E0[65536]; // 256x256 //map array5

char x_BYTE_D41B6 = 1; // weak

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
uint8_t unk_D4A30[0x120] = {//terrain gen
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
//int compindexes[100];//compstr[i].index
//int compadresses[100];//compstr[i].adress
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
					comp20 = compare_with_sequence(buffer1, (uint8_t*)x_BYTE_10B4E0_terraintype, 0x2dc4e0, index - skip2, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0, (exitindex != 1000000));
					comp20 = compare_with_sequence(buffer1, (uint8_t*)x_BYTE_11B4E0_heightmap, 0x2dc4e0, index - skip2, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x10000, (exitindex != 1000000));
					comp20 = compare_with_sequence(buffer1, (uint8_t*)x_BYTE_12B4E0_shading, 0x2dc4e0, index - skip2, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x20000, (exitindex != 1000000));
					comp20 = compare_with_sequence(buffer1, (uint8_t*)x_BYTE_13B4E0_angle, 0x2dc4e0, index - skip2, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x30000, (exitindex != 1000000));
					comp20 = compare_with_sequence(buffer1, (uint8_t*)mapEntityIndex_15B4E0, 0x2dc4e0, index - skip2, 0x70000, 0x20000, &origbyte20, &remakebyte20, 0x50000, (exitindex != 1000000));

#ifdef TEST_x64
					type_shadow_D41A0_BYTESTR_0 shadow_D41A0_BYTESTR_0;
					Convert_to_shadow_D41A0_BYTESTR_0(&D41A0_0, &shadow_D41A0_BYTESTR_0);
					comp20 = compare_with_sequence_D41A0(buffer2, (uint8_t*)&shadow_D41A0_BYTESTR_0, 0x356038, index - skip2, 224790, &origbyte20, &remakebyte20, 0, (exitindex != 1000000));
#else
					comp20 = compare_with_sequence_D41A0(buffer2, (uint8_t*)&D41A0_0, 0x356038, index, 224790, &origbyte20, &remakebyte20);
#endif

					comp20 = compare_with_sequence_array_E2A74(buffer3, (uint8_t*)&str_E2A74, 0x2b3a74, index - skip2, 0xc4e, 0xc4e, &origbyte20, &remakebyte20, 0, (exitindex != 1000000));

					//screen
					//comp20 = compare_with_sequence(buffer4, pdwScreenBuffer, 0x3aa0a4, index, 320 * 200, 320 * 200, &origbyte20, &remakebyte20);
				}
				//if(debugcounter_271478>5)
				//comp20 = compare_with_sequence(buffer4, pdwScreenBuffer, 0x3aa0a4, index, 320 * 200, 320 * 200, &origbyte20, &remakebyte20);
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
void GenerateLevelMap_43830(unsigned int a1, type_str_2FECE* a2x)//224830
{
	//uint8_t* v2; // ebx
	//int32_t v3; // ST0C_4
	//test_x_D41A0_BYTEARRAY_0();
	//v2 = a2;
	x_WORD_17B4E0 = a2x->seed_0x2FEE5;
	//*(uint32_t*)(x_D41A0_BYTEARRAY_0 + 8) = *(uint32_t*)(a2 + 23);
	D41A0_0.rand_0x8 = a2x->seed_0x2FEE5;
	memset((void*)mapEntityIndex_15B4E0, 0, 0x20000);
	//v3 = *(uint16_t*)(v2 + 35);
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
	memset((void*)x_BYTE_10B4E0_terraintype, 0, 0x10000);
	sub_43970(a1);//224970 // smooth terrain
	sub_43EE0();//224ee0 // add rivers

	sub_44580();//225580 //set angle of terrain
	if (x_BYTE_D41B6)
		sub_43B40();//224b40 //change angle of terrain
	else
		sub_43D50();//224d50 //change angle of terrain

#ifdef DEBUG_SEQUENCES
	/*uint8_t origbyte20 = 0;
	uint8_t remakebyte20 = 0;
	int comp20;
	//if (debugafterload)
	{
		comp20 = compare_with_sequence((char*)"00224959-002DC4E0", (uint8_t*)x_BYTE_10B4E0_terraintype, 0x2dc4e0, debugcounter_224959, 0x70000, 0x10000, &origbyte20, &remakebyte20);
		comp20 = compare_with_sequence((char*)"00224959-002DC4E0", (uint8_t*)x_BYTE_11B4E0_height, 0x2dc4e0, debugcounter_224959, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x10000);
		comp20 = compare_with_sequence((char*)"00224959-002DC4E0", (uint8_t*)x_BYTE_12B4E0_shading, 0x2dc4e0, debugcounter_224959, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x20000);
		comp20 = compare_with_sequence((char*)"00224959-002DC4E0", (uint8_t*)x_BYTE_13B4E0_angle, 0x2dc4e0, debugcounter_224959, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x30000);
		comp20 = compare_with_sequence((char*)"00224959-002DC4E0", (uint8_t*)x_WORD_15B4E0_source, 0x2dc4e0, debugcounter_224959, 0x70000, 0x20000, &origbyte20, &remakebyte20, 0x50000);

		comp20 = compare_with_sequence_D41A0((char*)"00224959-00356038", (uint8_t*)&D41A0_BYTESTR_0, 0x356038, debugcounter_224959, 224790, &origbyte20, &remakebyte20);

		comp20 = compare_with_sequence_array_E2A74((char*)"00224959-002B3A74", (uint8_t*)& array_E2A74, 0x2b3a74, debugcounter_224959, 0xc4e, 0xc4e, &origbyte20, &remakebyte20);
		//if(debugcounter_271478>5)
		//comp20 = compare_with_sequence((char*)"00224959-003AA0A4", pdwScreenBuffer, 0x3aa0a4, debugcounter_224959, 320 * 200, 320 * 200, &origbyte20, &remakebyte20);

		debugcounter_224959++;
	}*/
#endif //DEBUG_SEQUENCES

	sub_44D00();//225d00
	//debug
		//x_WORD_15B4E0_source 32c4e0
		//x_BYTE_11B4E0_height 2ec4e0
		//x_BYTE_13B4E0_angle_type 30c4e0
		//x_BYTE_10B4E0_terraintype 2dc4e0
		//x_BYTE_12B4E0_shading 2fc4e0
		/*origbyte = 0;
		remakebyte = 0;
		comp1 = compare_with_snapshot((char*)"0160-0022495E", (uint8_t*)x_WORD_15B4E0_source, 0x32c4e0, 0x20000, &origbyte, &remakebyte);
		comp2 = compare_with_snapshot((char*)"0160-0022495E", x_BYTE_11B4E0_height, 0x2ec4e0, 0x10000, &origbyte, &remakebyte);
		comp3 = compare_with_snapshot((char*)"0160-0022495E", x_BYTE_13B4E0_angle_type, 0x30c4e0, 0x10000, &origbyte, &remakebyte);
		comp4 = compare_with_snapshot((char*)"0160-0022495E", x_BYTE_10B4E0_terraintype, 0x2dc4e0, 0x10000, &origbyte, &remakebyte);
		int comp5 = compare_with_snapshot((char*)"0160-0022495E", x_BYTE_12B4E0_shading, 0x2fc4e0, 0x10000, &origbyte, &remakebyte);
		*/
		//	compsize = compare_with_snapshot((char*)"0160-00256200-2", pdwScreenBuffer, 0x3aa0a4, 640 * height, &origbyte, &remakebyte);//4c
	//debug
}
// 10000: using guessed type void /*__noreturn*/ sub_10000();
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41B6: using guessed type char x_BYTE_D41B6;
// 17B4E0: using guessed type __int16 x_WORD_17B4E0;

//----- (000B5E70) --------------------------------------------------------
void /*__spoils<ecx>*/ sub_B5E70_decompress_terrain_map_level(__int16 a1, unsigned __int16 a2, __int16 a3, int32_t a4)//296e70
{
	uint16_t v3; // ebx
	__int16 v4; // cx
	//char v5; // sf
	//char v6; // of
	int8_t v7; // [esp+1h] [ebp-3h]
	int8_t v8; // [esp+1h] [ebp-3h]
	int8_t v9; // [esp+2h] [ebp-2h]
	int8_t v10; // [esp+2h] [ebp-2h]
	int8_t v11; // [esp+3h] [ebp-1h]
	//uint8_t savedregs[20]; // [esp+4h] [ebp+0h]

	mapEntityIndex_15B4E0[a2] = a3;//32c4e0 //first seed
	v11 = 7;
	do
	{
		v3 = a2;
		v4 = (1 << v11);
		v7 = 1 << (7 - v11);
		do
		{
			v9 = 1 << (7 - v11);
			do
			{
				sub_B5EFA(v4, &v3, a4, &a1);//355220
				v9--;
			} while (v9);
			v3 += (v4 + v4) << 8;
			v7--;
		} while (v7);
		v8 = 1 << (7 - v11);
		do
		{
			v10 = 1 << (7 - v11);
			do
			{
				sub_B5F8F(v4, &v3, a4, &a1);
				v10--;
			} while (v10);
			v3 += (v4 + v4) << 8;
			v8--;
		} while (v8);
		v11--;
		//v6 = __OFSUB__(v11, 1);
		//v5 = (v11-- - 1) < 0;
	} while (v11 >= 0/*!(v5 ^ v6)*/);
}

//----- (00044DB0) --------------------------------------------------------
void sub_44DB0_truncTerrainHeight()//225db0 // map to heightmap
{
	__int16 v0; // dx
	__int16 v1; // cx
	uint16_t v2; // bx
	__int16 v3; // ax
	int v4; // edx
	uint32_t x; // eax

	v0 = -32000;
	v1 = 32000;
	v2 = 0;
	do
	{
		v3 = mapEntityIndex_15B4E0[v2];
		if (v3 > v0)
			v0 = mapEntityIndex_15B4E0[v2];
		if (v3 < v1)
			v1 = mapEntityIndex_15B4E0[v2];
		v2++;
	} while (v2);//find min and max height
	if (v0)
		v4 = 0xC40000 / v0;
	else
		v4 = 0;
	do
	{
		x = v4 * mapEntityIndex_15B4E0[v2] >> 16;
		mapEntityIndex_15B4E0[v2] = 0;
		if ((x & 0x8000u) != 0)//water level trunc
			x = 0;
		if (x > 196)//trunc max height
			x = 196;
		x_BYTE_11B4E0_heightmap[v2] = x;
		v2++;
	} while (v2);
	//return result;
}

int sub_B5C60_getTerrainAlt2(uint16_t a1, uint16_t a2)//296c60
{
	uint16_t v2; // ebx
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
	v3 = LOBYTE(a2);
	if ((HIBYTE(a1) + HIBYTE(a2)) & 1)
	{
		//if (__CFADD__((x_BYTE)v3, (x_BYTE)a1))
		if (uint8(v3) > uint8(v3 + a1))
		{
			HIBYTE(v2) = HIBYTE(a2) + 1;
			v4 = x_BYTE_11B4E0_heightmap[v2];
			LOBYTE(v2)++;
			v8 = x_BYTE_11B4E0_heightmap[v2];
			LOBYTE(v3) = ~(x_BYTE)a2;
			HIBYTE(v2) = HIBYTE(a2);
			v6 = v3 * (x_BYTE_11B4E0_heightmap[v2] - v8) + (unsigned __int8)a1 * (v8 - v4);
		}
		else
		{
			v4 = x_BYTE_11B4E0_heightmap[v2];
			LOBYTE(v2)++;
			v5 = (unsigned __int8)a1 * (x_BYTE_11B4E0_heightmap[v2] - v4);
			LOBYTE(v2)--;
			HIBYTE(v2) = HIBYTE(a2) + 1;
			v6 = (unsigned __int8)a2 * (x_BYTE_11B4E0_heightmap[v2] - v4) + v5;
		}
		result = (v6 >> 3) + 32 * v4;
	}
	else
	{
		if ((unsigned __int8)a1 <= (unsigned __int8)v3)
		{
			v9 = x_BYTE_11B4E0_heightmap[v2];
			HIBYTE(v2) = HIBYTE(a2) + 1;
			v12 = x_BYTE_11B4E0_heightmap[v2];
			LOBYTE(v2)++;
			v11 = (unsigned __int8)a2 * (v12 - v9) + (unsigned __int8)a1 * (x_BYTE_11B4E0_heightmap[v2] - v12);
		}
		else
		{
			v9 = x_BYTE_11B4E0_heightmap[v2];
			LOBYTE(v2)++;
			v10 = x_BYTE_11B4E0_heightmap[v2];
			HIBYTE(v2) = HIBYTE(a2) + 1;
			v11 = (unsigned __int8)a2 * (x_BYTE_11B4E0_heightmap[v2] - v10) + (unsigned __int8)a1 * (v10 - v9);
		}
		result = (v11 >> 3) + 32 * v9;
	}
	return result;
}

//----- (00044E40) --------------------------------------------------------
void sub_44E40(int a1, uint8_t a2)//225e40
{
	int v2; // edi
	uint16_t v3; // si
	signed int v4; // ecx
	uint16_t v5; // ax
	uaxis_2d v6x; // dx
	uint16_t v7; // eax
	uint16_t v8; // ebx
	//unsigned __int16 i; // ax

	v2 = a1;
	v3 = 0;
	do
	{
		if (x_BYTE_11B4E0_heightmap[v3])
			x_BYTE_13B4E0_angle[v3] = 5;
		else
			x_BYTE_13B4E0_angle[v3] = 0;
		v3++;
	} while (v3);
LABEL_12:
	if (v2 > 0)
	{
		v4 = 1000;
		while (1)
		{
			//v8 = -1;
			v5 = 9377 * x_WORD_17B4E0 + 9439;
			x_WORD_17B4E0 = v5;
			v6x.word = v5 % 0xffffu;
			v7 = v5 % 0xffffu;
			v8 = x_BYTE_11B4E0_heightmap[v7] + 0xff00;
			if (!--v4)
				break;
			if ((v8 & 0xff) > a2 && x_BYTE_13B4E0_angle[v7])
			{
				v2--;
				//adress 225eb1
				//eax e568 ebx ff6f ecx 3e6 edx e568
				sub_44EE0_smooth_tiles(/*v8,*/ v6x);
				goto LABEL_12;
			}
		}
	}
	do
	{
		//i = v3;
		x_BYTE_10B4E0_terraintype[v3++] = -1;
	} while (v3);
}
// 17B4E0: using guessed type __int16 x_WORD_17B4E0;

//----- (00045AA0) --------------------------------------------------------
void sub_45AA0_setMax4Tiles()//226aa0
{
	uaxis_2d indexx; // ax
	char v1; // dh
	//unsigned __int16 v2; // cx
	unsigned __int8 v3; // dl
	unsigned __int8 v4; // bl
	//unsigned __int16 v5; // cx
	//unsigned __int16 v6; // cx
	//unsigned __int16 v7; // cx
	//unsigned __int16 v8; // cx
	char v9; // [esp+0h] [ebp-4h]

	//[*][*]
	//[*][*]

	do
	{
		indexx.word = 0;
		v9 = 0;
		do
		{
			v1 = 0;
			if (!x_BYTE_13B4E0_angle[indexx.word])
				v1 = 1;
			//v2 = index;
			v3 = x_BYTE_11B4E0_heightmap[indexx.word];
			v4 = x_BYTE_11B4E0_heightmap[indexx.word];
			//LOBYTE(index)++;
			indexx._axis_2d.x++;
			if (!x_BYTE_13B4E0_angle[indexx.word])
				v1++;
			if (v3 > x_BYTE_11B4E0_heightmap[indexx.word])
				v3 = x_BYTE_11B4E0_heightmap[indexx.word];
			if (v4 < x_BYTE_11B4E0_heightmap[indexx.word])
				v4 = x_BYTE_11B4E0_heightmap[indexx.word];
			//HIBYTE(index)++;
			indexx._axis_2d.y++;
			if (!x_BYTE_13B4E0_angle[indexx.word])
				v1++;
			if (v3 > x_BYTE_11B4E0_heightmap[indexx.word])
				v3 = x_BYTE_11B4E0_heightmap[indexx.word];
			if (v4 < x_BYTE_11B4E0_heightmap[indexx.word])
				v4 = x_BYTE_11B4E0_heightmap[indexx.word];
			//LOBYTE(index)--;
			indexx._axis_2d.x--;
			if (!x_BYTE_13B4E0_angle[indexx.word])
				v1++;
			if (v3 > x_BYTE_11B4E0_heightmap[indexx.word])
				v3 = x_BYTE_11B4E0_heightmap[indexx.word];
			if (v4 < x_BYTE_11B4E0_heightmap[indexx.word])
				v4 = x_BYTE_11B4E0_heightmap[indexx.word];
			//HIBYTE(index)--;
			indexx._axis_2d.y--;
			if (v4 != v3 && v1 == 4)
			{
				v9 = 1;
				//v5 = index;
				x_BYTE_11B4E0_heightmap[indexx.word] = v3;
				//LOBYTE(index)++;
				indexx._axis_2d.x++;
				//v6 = index;
				x_BYTE_11B4E0_heightmap[indexx.word] = v3;
				//HIBYTE(index)++;
				indexx._axis_2d.y++;
				//v7 = index;
				x_BYTE_11B4E0_heightmap[indexx.word] = v3;
				//LOBYTE(index)--;
				indexx._axis_2d.x--;
				//v8 = index;
				x_BYTE_11B4E0_heightmap[indexx.word] = v3;
				//HIBYTE(index)--;
				indexx._axis_2d.y--;
			}
			indexx.word++;
		} while (indexx.word);
	} while (v9);
}

//----- (000440D0) --------------------------------------------------------
void sub_440D0(unsigned __int16 a1)//2250d0
{
	unsigned __int16 i; // ax
	unsigned __int8 v2; // dh
	unsigned __int8 v3; // dl
	//unsigned __int16 v4; // ax
	//unsigned __int16 v5; // ax
	int v6; // ebx
	char v7; // bl
	char v8; // dl
	char v9; // dh

	// fix if begin
	//v4 = 0;
	//v5 = 0;
	// end

	i = 0;
	do
	{
		if (x_BYTE_13B4E0_angle[i] == 5)
		{
			v2 = 0;
			v3 = 0xFFu;
			if (x_BYTE_11B4E0_heightmap[i])
				v2 = x_BYTE_11B4E0_heightmap[i];
			if (x_BYTE_11B4E0_heightmap[i] < 0xFFu)
				v3 = x_BYTE_11B4E0_heightmap[i];
			HIBYTE(i)--;
			if (v2 < x_BYTE_11B4E0_heightmap[i])
				v2 = x_BYTE_11B4E0_heightmap[i];
			if (v3 > x_BYTE_11B4E0_heightmap[i])
				v3 = x_BYTE_11B4E0_heightmap[i];
			LOBYTE(i)++;
			HIBYTE(i)++;
			if (v2 < x_BYTE_11B4E0_heightmap[i])
				v2 = x_BYTE_11B4E0_heightmap[i];
			if (v3 > x_BYTE_11B4E0_heightmap[i])
				v3 = x_BYTE_11B4E0_heightmap[i];
			LOBYTE(i)--;
			HIBYTE(i)++;
			if (v2 < x_BYTE_11B4E0_heightmap[i])
				v2 = x_BYTE_11B4E0_heightmap[i];
			if (v3 > x_BYTE_11B4E0_heightmap[i])
				v3 = x_BYTE_11B4E0_heightmap[i];
			LOBYTE(i)--;
			HIBYTE(i)--;
			if (v2 < x_BYTE_11B4E0_heightmap[i])
				v2 = x_BYTE_11B4E0_heightmap[i];
			if (v3 > x_BYTE_11B4E0_heightmap[i])
				v3 = x_BYTE_11B4E0_heightmap[i];
			v6 = v2 - v3;
			LOBYTE(i)++;
			if (v6 <= a1)
			{
				if (v6 == a1)
					x_BYTE_13B4E0_angle[i] = 4;
				else
					x_BYTE_13B4E0_angle[i] = 3;
			}
		}
		i++;
	} while (i);
	do
	{
		v7 = 0;
		v8 = 0;
		v9 = 0;
		if (x_BYTE_13B4E0_angle[i] == 3)
			v7 = 1;
		if (x_BYTE_13B4E0_angle[i] == 2)
			v8 = 1;
		if (x_BYTE_13B4E0_angle[i] == 5)
			v9 = 1;
		LOBYTE(i)++;
		if (x_BYTE_13B4E0_angle[i] == 3)
			v7++;
		if (x_BYTE_13B4E0_angle[i] == 2)
			v8++;
		if (x_BYTE_13B4E0_angle[i] == 5)
			v9++;
		HIBYTE(i)++;
		if (x_BYTE_13B4E0_angle[i] == 3)
			v7++;
		if (x_BYTE_13B4E0_angle[i] == 2)
			v8++;
		if (x_BYTE_13B4E0_angle[i] == 5)
			v9++;
		LOBYTE(i)--;
		if (x_BYTE_13B4E0_angle[i] == 3)
			v7++;
		if (x_BYTE_13B4E0_angle[i] == 2)
			v8++;
		if (x_BYTE_13B4E0_angle[i] == 5)
			v9++;
		HIBYTE(i)--;
		if (!v8 && v7 && v9)
		{
			if (x_BYTE_13B4E0_angle[i] == 3)
				x_BYTE_13B4E0_angle[i] = 4;
			LOBYTE(i)++;
			if (x_BYTE_13B4E0_angle[i] == 3)
				x_BYTE_13B4E0_angle[i] = 4;
			HIBYTE(i)++;
			if (x_BYTE_13B4E0_angle[i] == 3)
				x_BYTE_13B4E0_angle[i] = 4;
			LOBYTE(i)--;
			if (x_BYTE_13B4E0_angle[i] == 3)
				x_BYTE_13B4E0_angle[i] = 4;
			HIBYTE(i)--;
		}
		i++;
	} while (i);
	//  return result;
}

//----- (00045060) --------------------------------------------------------
void sub_45060(uint8_t a1, uint8_t a2)//226060
{
	uint16_t v2; // dx
	uint8_t index; // al
	uint8_t v4; // ah
	//unsigned __int16 v5; // dx

	qmemcpy(x_BYTE_10B4E0_terraintype, x_BYTE_13B4E0_angle, 0x10000);
	v2 = 0;
	do
	{
		index = 0;
		v4 = -1;
		if (x_BYTE_11B4E0_heightmap[v2])
			index = x_BYTE_11B4E0_heightmap[v2];
		if (x_BYTE_11B4E0_heightmap[v2] < 0xFFu)
			v4 = x_BYTE_11B4E0_heightmap[v2];
		HIBYTE(v2)--;
		if (index < x_BYTE_11B4E0_heightmap[v2])
			index = x_BYTE_11B4E0_heightmap[v2];
		if (v4 > x_BYTE_11B4E0_heightmap[v2])
			v4 = x_BYTE_11B4E0_heightmap[v2];
		LOBYTE(v2)++;
		if (index < x_BYTE_11B4E0_heightmap[v2])
			index = x_BYTE_11B4E0_heightmap[v2];
		if (v4 > x_BYTE_11B4E0_heightmap[v2])
			v4 = x_BYTE_11B4E0_heightmap[v2];
		HIBYTE(v2)++;
		if (index < x_BYTE_11B4E0_heightmap[v2])
			index = x_BYTE_11B4E0_heightmap[v2];
		if (v4 > x_BYTE_11B4E0_heightmap[v2])
			v4 = x_BYTE_11B4E0_heightmap[v2];
		HIBYTE(v2)++;
		if (index < x_BYTE_11B4E0_heightmap[v2])
			index = x_BYTE_11B4E0_heightmap[v2];
		if (v4 > x_BYTE_11B4E0_heightmap[v2])
			v4 = x_BYTE_11B4E0_heightmap[v2];
		LOBYTE(v2)--;
		if (index < x_BYTE_11B4E0_heightmap[v2])
			index = x_BYTE_11B4E0_heightmap[v2];
		if (v4 > x_BYTE_11B4E0_heightmap[v2])
			v4 = x_BYTE_11B4E0_heightmap[v2];
		LOBYTE(v2)--;
		if (index < x_BYTE_11B4E0_heightmap[v2])
			index = x_BYTE_11B4E0_heightmap[v2];
		if (v4 > x_BYTE_11B4E0_heightmap[v2])
			v4 = x_BYTE_11B4E0_heightmap[v2];
		HIBYTE(v2)--;
		if (index < x_BYTE_11B4E0_heightmap[v2])
			index = x_BYTE_11B4E0_heightmap[v2];
		if (v4 > x_BYTE_11B4E0_heightmap[v2])
			v4 = x_BYTE_11B4E0_heightmap[v2];
		HIBYTE(v2)--;
		if (index < x_BYTE_11B4E0_heightmap[v2])
			index = x_BYTE_11B4E0_heightmap[v2];
		if (v4 > x_BYTE_11B4E0_heightmap[v2])
			v4 = x_BYTE_11B4E0_heightmap[v2];
		LOBYTE(v2)++;
		HIBYTE(v2)++;
		if (index < a1 && index - v4 <= a2)
		{
			if (x_BYTE_13B4E0_angle[v2])
				x_BYTE_13B4E0_angle[v2] = 5;
		}
		v2++;
	} while (v2);
}
// 10000: using guessed type void /*__noreturn*/ sub_10000();

//----- (00044320) --------------------------------------------------------
void sub_44320()//225320
{
	uint16_t index; // ax
	char v1; // bl
	char v2; // dh
	char v3; // dl

	index = 0;
	do
	{
		v1 = 0;
		v2 = 0;
		v3 = 0;
		if (!x_BYTE_13B4E0_angle[index])
			v1 = 1;
		if (x_BYTE_13B4E0_angle[index] == 5)
			v3 = 1;
		if (x_BYTE_13B4E0_angle[index] == 3)
			v2 = 1;
		LOBYTE(index)++;
		if (!x_BYTE_13B4E0_angle[index])
			v1++;
		if (x_BYTE_13B4E0_angle[index] == 5)
			v3++;
		if (x_BYTE_13B4E0_angle[index] == 3)
			v2++;
		HIBYTE(index)++;
		if (!x_BYTE_13B4E0_angle[index])
			v1++;
		if (x_BYTE_13B4E0_angle[index] == 5)
			v3++;
		if (x_BYTE_13B4E0_angle[index] == 3)
			v2++;
		LOBYTE(index)--;
		if (!x_BYTE_13B4E0_angle[index])
			v1++;
		if (x_BYTE_13B4E0_angle[index] == 5)
			v3++;
		if (x_BYTE_13B4E0_angle[index] == 3)
			v2++;
		HIBYTE(index)--;
		if (v2 && v3)
		{
			if (x_BYTE_13B4E0_angle[index] == 5)
				x_BYTE_13B4E0_angle[index] = 4;
			LOBYTE(index)++;
			if (x_BYTE_13B4E0_angle[index] == 5)
				x_BYTE_13B4E0_angle[index] = 4;
			HIBYTE(index)++;
			if (x_BYTE_13B4E0_angle[index] == 5)
				x_BYTE_13B4E0_angle[index] = 4;
			LOBYTE(index)--;
			if (x_BYTE_13B4E0_angle[index] == 5)
				x_BYTE_13B4E0_angle[index] = 4;
			HIBYTE(index)--;
		}
		if (v2 && v1)
		{
			if (x_BYTE_13B4E0_angle[index] == 3)
				x_BYTE_13B4E0_angle[index] = 4;
			LOBYTE(index)++;
			if (x_BYTE_13B4E0_angle[index] == 3)
				x_BYTE_13B4E0_angle[index] = 4;
			HIBYTE(index)++;
			if (x_BYTE_13B4E0_angle[index] == 3)
				x_BYTE_13B4E0_angle[index] = 4;
			LOBYTE(index)--;
			if (x_BYTE_13B4E0_angle[index] == 3)
				x_BYTE_13B4E0_angle[index] = 4;
			HIBYTE(index)--;
		}
		if (v1 && v3)
		{
			if (x_BYTE_13B4E0_angle[index])
				x_BYTE_13B4E0_angle[index] = 4;
			LOBYTE(index)++;
			if (x_BYTE_13B4E0_angle[index])
				x_BYTE_13B4E0_angle[index] = 4;
			HIBYTE(index)++;
			if (x_BYTE_13B4E0_angle[index])
				x_BYTE_13B4E0_angle[index] = 4;
			LOBYTE(index)--;
			if (x_BYTE_13B4E0_angle[index])
				x_BYTE_13B4E0_angle[index] = 4;
			HIBYTE(index)--;
		}
		index++;
	} while (index);
}

//----- (00045210) --------------------------------------------------------
void sub_45210(uint8_t a1, uint8_t a2)//226210
{
	uint16_t v2; // dx
	uint16_t v3; // eax
	//char v4; // t1
	uint8_t v5; // bl
	uint8_t v6; // bh
	//unsigned __int16 v7; // dx
	int v8; // ecx

	//fix
	v3 = 0;
	//fix

	qmemcpy(x_BYTE_10B4E0_terraintype, x_BYTE_13B4E0_angle, 0x10000);
	v2 = 0;
	do
	{
		BYTE1(v3) = 0xffu;
		//v4 = x_BYTE_13B4E0_angle[v2];
		v5 = 0;
		v6 = 0;
		if (x_BYTE_11B4E0_heightmap[v2] > 0u)
			LOBYTE(v3) = x_BYTE_11B4E0_heightmap[v2];
		if (x_BYTE_11B4E0_heightmap[v2] < 0xFFu)
			BYTE1(v3) = x_BYTE_11B4E0_heightmap[v2];
		HIBYTE(v2)--;
		if (LOBYTE(v3) < x_BYTE_11B4E0_heightmap[v2])
			LOBYTE(v3) = x_BYTE_11B4E0_heightmap[v2];
		if (BYTE1(v3) > x_BYTE_11B4E0_heightmap[v2])
			BYTE1(v3) = x_BYTE_11B4E0_heightmap[v2];
		if (x_BYTE_13B4E0_angle[v2] == 5)
			v6 = 1;
		if (x_BYTE_13B4E0_angle[v2] == 2)
			v5 = 1;
		LOBYTE(v2)++;
		if (LOBYTE(v3) < x_BYTE_11B4E0_heightmap[v2])
			LOBYTE(v3) = x_BYTE_11B4E0_heightmap[v2];
		if (BYTE1(v3) > x_BYTE_11B4E0_heightmap[v2])
			BYTE1(v3) = x_BYTE_11B4E0_heightmap[v2];
		if (x_BYTE_13B4E0_angle[v2] == 5)
			v6++;
		if (x_BYTE_13B4E0_angle[v2] == 2)
			v5++;
		HIBYTE(v2)++;
		if (LOBYTE(v3) < x_BYTE_11B4E0_heightmap[v2])
			LOBYTE(v3) = x_BYTE_11B4E0_heightmap[v2];
		if (BYTE1(v3) > x_BYTE_11B4E0_heightmap[v2])
			BYTE1(v3) = x_BYTE_11B4E0_heightmap[v2];
		if (x_BYTE_13B4E0_angle[v2] == 5)
			v6++;
		if (x_BYTE_13B4E0_angle[v2] == 2)
			v5++;
		HIBYTE(v2)++;
		if (LOBYTE(v3) < x_BYTE_11B4E0_heightmap[v2])
			LOBYTE(v3) = x_BYTE_11B4E0_heightmap[v2];
		if (BYTE1(v3) > x_BYTE_11B4E0_heightmap[v2])
			BYTE1(v3) = x_BYTE_11B4E0_heightmap[v2];
		if (x_BYTE_13B4E0_angle[v2] == 5)
			v6++;
		if (x_BYTE_13B4E0_angle[v2] == 2)
			v5++;
		LOBYTE(v2)--;
		if (LOBYTE(v3) < x_BYTE_11B4E0_heightmap[v2])
			LOBYTE(v3) = x_BYTE_11B4E0_heightmap[v2];
		if (BYTE1(v3) > x_BYTE_11B4E0_heightmap[v2])
			BYTE1(v3) = x_BYTE_11B4E0_heightmap[v2];
		if (x_BYTE_13B4E0_angle[v2] == 5)
			v6++;
		if (x_BYTE_13B4E0_angle[v2] == 2)
			v5++;
		LOBYTE(v2)--;
		if (LOBYTE(v3) < x_BYTE_11B4E0_heightmap[v2])
			LOBYTE(v3) = x_BYTE_11B4E0_heightmap[v2];
		if (BYTE1(v3) > x_BYTE_11B4E0_heightmap[v2])
			BYTE1(v3) = x_BYTE_11B4E0_heightmap[v2];
		if (x_BYTE_13B4E0_angle[v2] == 5)
			v6++;
		if (x_BYTE_13B4E0_angle[v2] == 2)
			v5++;
		HIBYTE(v2)--;
		if (LOBYTE(v3) < x_BYTE_11B4E0_heightmap[v2])
			LOBYTE(v3) = x_BYTE_11B4E0_heightmap[v2];
		if (BYTE1(v3) > x_BYTE_11B4E0_heightmap[v2])
			BYTE1(v3) = x_BYTE_11B4E0_heightmap[v2];
		if (x_BYTE_13B4E0_angle[v2] == 5)
			v6++;
		if (x_BYTE_13B4E0_angle[v2] == 2)
			v5++;
		HIBYTE(v2)--;
		if (LOBYTE(v3) < x_BYTE_11B4E0_heightmap[v2])
			LOBYTE(v3) = x_BYTE_11B4E0_heightmap[v2];
		if (BYTE1(v3) > x_BYTE_11B4E0_heightmap[v2])
			BYTE1(v3) = x_BYTE_11B4E0_heightmap[v2];
		if (x_BYTE_13B4E0_angle[v2] == 5)
			v6++;
		if (x_BYTE_13B4E0_angle[v2] == 2)
			v5++;
		LOBYTE(v2)++;
		HIBYTE(v2)++;
		if (LOBYTE(v3) < a1)
		{
			v8 = LOBYTE(v3) - BYTE1(v3);
			LOBYTE(v3) = a2;
			if (v8 <= a2 && x_BYTE_13B4E0_angle[v2] == 5)
			{
				v3 = v6 + v5;
				if (v3 == 8)
					x_BYTE_13B4E0_angle[v2] = 2;
			}
		}
		v2++;
	} while (v2);
}
// 10000: using guessed type void /*__noreturn*/ sub_10000();

//----- (000454F0) --------------------------------------------------------
void sub_454F0(uint8_t a1, uint8_t a2)//2264f0
{
	uint16_t index; // ax
	uint8_t v3; // dh
	uint8_t v4; // dl
	//unsigned __int16 v5; // ax
	//uint16_t v6; // ax

	// fix if begin
	//v5 = 0;
	//v6 = 0;
	// end

	index = 0;
	do
	{
		if (x_BYTE_11B4E0_heightmap[index] > a1)
		{
			v3 = 0;
			v4 = -1;
			if (x_BYTE_11B4E0_heightmap[index])
				v3 = x_BYTE_11B4E0_heightmap[index];
			if (x_BYTE_11B4E0_heightmap[index] < 0xFFu)
				v4 = x_BYTE_11B4E0_heightmap[index];
			HIBYTE(index)--;
			if (v3 < x_BYTE_11B4E0_heightmap[index])
				v3 = x_BYTE_11B4E0_heightmap[index];
			if (v4 > x_BYTE_11B4E0_heightmap[index])
				v4 = x_BYTE_11B4E0_heightmap[index];
			LOBYTE(index)++;
			HIBYTE(index)++;
			if (v3 < x_BYTE_11B4E0_heightmap[index])
				v3 = x_BYTE_11B4E0_heightmap[index];
			if (v4 > x_BYTE_11B4E0_heightmap[index])
				v4 = x_BYTE_11B4E0_heightmap[index];
			HIBYTE(index)++;
			LOBYTE(index)--;
			if (v3 < x_BYTE_11B4E0_heightmap[index])
				v3 = x_BYTE_11B4E0_heightmap[index];
			if (v4 > x_BYTE_11B4E0_heightmap[index])
				v4 = x_BYTE_11B4E0_heightmap[index];
			LOBYTE(index)--;
			HIBYTE(index)--;
			if (v3 < x_BYTE_11B4E0_heightmap[index])
				v3 = x_BYTE_11B4E0_heightmap[index];
			if (v4 > x_BYTE_11B4E0_heightmap[index])
				v4 = x_BYTE_11B4E0_heightmap[index];
			LOBYTE(index)++;
			if (x_BYTE_13B4E0_angle[index])
			{
				if (v3 - v4 < a2)
					x_BYTE_13B4E0_angle[index] = 6;
			}
		}
		index++;
	} while (index);
}

//----- (00045600) --------------------------------------------------------
void sub_45600(uint8_t a1)//226600
{
	uint16_t index; // ax
	unsigned __int8 v2; // dh
	unsigned __int8 v3; // dl
	//unsigned __int16 v4; // ax
	//unsigned __int16 v5; // ax
	//unsigned __int16 v6; // ax
	__int16 v7; // bx
	char v8; // dh
	char v9; // dl
	//char v10; // t0
	//char v11; // t0
	//char v12; // t0
	//char v13; // t0
	//char v14; // t0
	//char v15; // t0
	//char v16; // t0
	//char v17; // t0
	//char v18; // t0
	//char v19; // t0
	//char v20; // t0
	//char v21; // t0
	//char v22; // t0
	//char v23; // t0
	//char v24; // t0
	//char v25; // t0
	//char v26; // t0
	//char v27; // t0
	//char v28; // t0
	//char v29; // t0
	//char v30; // t0
	//char v31; // t0
	//char v32; // t0
	//char v33; // t0

	// fix if begin
	//v4 = 0;
	//v5 = 0;
	//v6 = 0;
	// end

	qmemcpy(x_BYTE_10B4E0_terraintype, x_BYTE_13B4E0_angle, 0x10000);
	index = 0;
	do
	{
		v2 = 0;
		v3 = 0xffu;
		if (x_BYTE_11B4E0_heightmap[index])
			v2 = x_BYTE_11B4E0_heightmap[index];
		if (x_BYTE_11B4E0_heightmap[index] < 0xFFu)
			v3 = x_BYTE_11B4E0_heightmap[index];
		HIBYTE(index)--;
		if (v2 < x_BYTE_11B4E0_heightmap[index])
			v2 = x_BYTE_11B4E0_heightmap[index];
		if (v3 > x_BYTE_11B4E0_heightmap[index])
			v3 = x_BYTE_11B4E0_heightmap[index];
		LOBYTE(index)++;
		HIBYTE(index)++;
		if (v2 < x_BYTE_11B4E0_heightmap[index])
			v2 = x_BYTE_11B4E0_heightmap[index];
		if (v3 > x_BYTE_11B4E0_heightmap[index])
			v3 = x_BYTE_11B4E0_heightmap[index];
		HIBYTE(index)++;
		LOBYTE(index)--;
		if (v2 < x_BYTE_11B4E0_heightmap[index])
			v2 = x_BYTE_11B4E0_heightmap[index];
		if (v3 > x_BYTE_11B4E0_heightmap[index])
			v3 = x_BYTE_11B4E0_heightmap[index];
		LOBYTE(index)--;
		HIBYTE(index)--;
		if (v2 < x_BYTE_11B4E0_heightmap[index])
			v2 = x_BYTE_11B4E0_heightmap[index];
		if (v3 > x_BYTE_11B4E0_heightmap[index])
			v3 = x_BYTE_11B4E0_heightmap[index];
		LOBYTE(index)++;
		if (x_BYTE_13B4E0_angle[index] && v2 - v3 >= a1)
			x_BYTE_13B4E0_angle[index] = 1;
		index++;
	} while (index);
	do
	{
		if (x_BYTE_13B4E0_angle[index] != 6)
			goto LABEL_100;
		v7 = 0;
		HIBYTE(index)--;
		v8 = 0;
		v9 = 0;
		if (x_BYTE_13B4E0_angle[index] == 3)
			HIBYTE(v7) = 1;
		if (x_BYTE_13B4E0_angle[index] == 2)
			v9 = 1;
		if (x_BYTE_13B4E0_angle[index] == 5)
			LOBYTE(v7) = 1;
		//v10 = x_BYTE_13B4E0_angle[index];
		//v11 = x_BYTE_13B4E0_angle[index];
		if (x_BYTE_13B4E0_angle[index] == 4)
			v8 = 1;
		//v12 = x_BYTE_13B4E0_angle[index];
		LOBYTE(index)++;
		if (x_BYTE_13B4E0_angle[index] == 3)
			HIBYTE(v7)++;
		if (x_BYTE_13B4E0_angle[index] == 2)
			v9++;
		if (x_BYTE_13B4E0_angle[index] == 5)
			LOBYTE(v7)++;
		//v13 = x_BYTE_13B4E0_angle[index];
		//v14 = x_BYTE_13B4E0_angle[index];
		if (x_BYTE_13B4E0_angle[index] == 4)
			v8++;
		//v15 = x_BYTE_13B4E0_angle[index];
		HIBYTE(index)++;
		if (x_BYTE_13B4E0_angle[index] == 3)
			HIBYTE(v7)++;
		if (x_BYTE_13B4E0_angle[index] == 2)
			v9++;
		if (x_BYTE_13B4E0_angle[index] == 5)
			LOBYTE(v7)++;
		//v16 = x_BYTE_13B4E0_angle[index];
		//v17 = x_BYTE_13B4E0_angle[index];
		if (x_BYTE_13B4E0_angle[index] == 4)
			v8++;
		//v18 = x_BYTE_13B4E0_angle[index];
		HIBYTE(index)++;
		if (x_BYTE_13B4E0_angle[index] == 3)
			HIBYTE(v7)++;
		if (x_BYTE_13B4E0_angle[index] == 2)
			v9++;
		if (x_BYTE_13B4E0_angle[index] == 5)
			LOBYTE(v7)++;
		//v19 = x_BYTE_13B4E0_angle[index];
		//v20 = x_BYTE_13B4E0_angle[index];
		if (x_BYTE_13B4E0_angle[index] == 4)
			v8++;
		//v21 = x_BYTE_13B4E0_angle[index];
		LOBYTE(index) = index - 1;
		if (x_BYTE_13B4E0_angle[index] == 3)
			HIBYTE(v7)++;
		if (x_BYTE_13B4E0_angle[index] == 2)
			v9++;
		if (x_BYTE_13B4E0_angle[index] == 5)
			LOBYTE(v7)++;
		//v22 = x_BYTE_13B4E0_angle[index];
		//v23 = x_BYTE_13B4E0_angle[index];
		if (x_BYTE_13B4E0_angle[index] == 4)
			v8++;
		//v24 = x_BYTE_13B4E0_angle[index];
		LOBYTE(index)--;
		if (x_BYTE_13B4E0_angle[index] == 3)
			HIBYTE(v7)++;
		if (x_BYTE_13B4E0_angle[index] == 2)
			v9++;
		if (x_BYTE_13B4E0_angle[index] == 5)
			LOBYTE(v7)++;
		//v25 = x_BYTE_13B4E0_angle[index];
		//v26 = x_BYTE_13B4E0_angle[index];
		if (x_BYTE_13B4E0_angle[index] == 4)
			v8++;
		//v27 = x_BYTE_13B4E0_angle[index];
		HIBYTE(index)--;
		if (x_BYTE_13B4E0_angle[index] == 3)
			HIBYTE(v7)++;
		if (x_BYTE_13B4E0_angle[index] == 2)
			v9++;
		if (x_BYTE_13B4E0_angle[index] == 5)
			LOBYTE(v7)++;
		//v28 = x_BYTE_13B4E0_angle[index];
		//v29 = x_BYTE_13B4E0_angle[index];
		if (x_BYTE_13B4E0_angle[index] == 4)
			v8++;
		//v30 = x_BYTE_13B4E0_angle[index];
		HIBYTE(index)--;
		if (x_BYTE_13B4E0_angle[index] == 3)
			HIBYTE(v7)++;
		if (x_BYTE_13B4E0_angle[index] == 2)
			v9++;
		if (x_BYTE_13B4E0_angle[index] == 5)
			LOBYTE(v7)++;
		//v31 = x_BYTE_13B4E0_angle[index];
		//v32 = x_BYTE_13B4E0_angle[index];
		if (x_BYTE_13B4E0_angle[index] == 4)
			v8++;
		//v33 = x_BYTE_13B4E0_angle[index];
		LOBYTE(index)++;
		HIBYTE(index)++;
		if (HIBYTE(v7))
		{
			if (v9 || LOBYTE(v7) || v8)
				goto LABEL_99;
		}
		else if (v9 || (LOBYTE(v7) && v8))
		{
		LABEL_99:
			x_BYTE_13B4E0_angle[index] = 1;
			goto LABEL_100;
		}
	LABEL_100:
		index++;
	} while (index);
}
// 10000: using guessed type void /*__noreturn*/ sub_10000();

//----- (00043FC0) --------------------------------------------------------
void sub_43FC0()//224fc0
{
	uint16_t index; // ax
	uint16_t v1; // dx
	char v2; // dh
	unsigned __int16 v3; // bx
	unsigned __int16 v4; // bx
	unsigned __int16 v5; // bx
	unsigned __int16 v6; // bx
	unsigned __int16 v7; // bx
	char v8; // bl
	//unsigned __int16 v9; // ax

	index = 0;
	do
	{
		HIBYTE(index)--;
		v1 = index;
		LOBYTE(index)++;
		v2 = x_BYTE_13B4E0_angle[v1];
		LOBYTE(v1) = v2 == x_BYTE_13B4E0_angle[index];
		HIBYTE(index)++;
		v3 = index;
		HIBYTE(index)++;
		LOBYTE(v1) = (v2 == x_BYTE_13B4E0_angle[v3]) + v1;
		v4 = index;
		LOBYTE(index)--;
		LOBYTE(v1) = (v2 == x_BYTE_13B4E0_angle[v4]) + v1;
		v5 = index;
		LOBYTE(index)--;
		LOBYTE(v1) = (v2 == x_BYTE_13B4E0_angle[v5]) + v1;
		v6 = index;
		HIBYTE(index)--;
		LOBYTE(v1) = (v2 == x_BYTE_13B4E0_angle[v6]) + v1;
		v7 = index;
		HIBYTE(index)--;
		v8 = (v2 == x_BYTE_13B4E0_angle[index]) + (v2 == x_BYTE_13B4E0_angle[v7]) + v1;
		LOBYTE(index)++;
		HIBYTE(index)++;
		if (v2)
		{
			if (v8 == 7)
				x_BYTE_13B4E0_angle[index] = v2;
		}
		index++;
	} while (index);
}

//----- (00043970) --------------------------------------------------------
void sub_43970(uint32_t a1)//224970
{
	uint16_t v1; // bx
	//uint32_t result; // eax

	v1 = 0;
	do
	{
		//result = sub_439A0(a1, v1);
		x_BYTE_11B4E0_heightmap[v1] = sub_439A0(a1, v1);
		v1++;
	} while (v1);
	//return result;
}

//----- (00043EE0) --------------------------------------------------------
void sub_43EE0()//224ee0
{
	unsigned __int16 index; // ax
	unsigned __int16 v1; // cx
	char v2; // dh
	unsigned __int8 v3; // dl
	int v4; // ecx
	char v5; // bl
	char v6; // bh
	unsigned __int8 v7; // cl
	char v8; // bh
	char v9; // bh
	unsigned __int16 v10; // cx
	unsigned __int16 v11; // cx
	unsigned __int16 v12; // cx
	unsigned __int16 v13; // cx

	index = 0;
	do
	{
		v1 = index;
		v2 = 0;
		LOBYTE(index)++;
		v3 = x_BYTE_11B4E0_heightmap[v1];
		v4 = v1;
		LOWORD(v4) = index;
		v5 = 0;
		v6 = x_BYTE_13B4E0_angle[index];
		if (v6)
		{
			if (v6 == 4)
				v2 = 1;
		}
		else
		{
			v7 = x_BYTE_11B4E0_heightmap[v4];
			v5 = 1;
			if (v7 < v3)
				v3 = v7;
		}
		HIBYTE(index)++;
		v8 = x_BYTE_13B4E0_angle[index];
		if (v8)
		{
			if (v8 == 4)
				v2++;
		}
		else
		{
			v5++;
			if (x_BYTE_11B4E0_heightmap[index] < v3)
				v3 = x_BYTE_11B4E0_heightmap[index];
		}
		LOBYTE(index)--;
		v9 = x_BYTE_13B4E0_angle[index];
		if (v9)
		{
			if (v9 == 4)
				v2++;
		}
		else
		{
			v5++;
			if (x_BYTE_11B4E0_heightmap[index] < v3)
				v3 = x_BYTE_11B4E0_heightmap[index];
		}
		HIBYTE(index)--;
		if (v2 && v5 && !v3)
		{
			v10 = index;
			LOBYTE(index)++;
			x_BYTE_11B4E0_heightmap[v10] = 0;
			v11 = index;
			HIBYTE(index)++;
			x_BYTE_11B4E0_heightmap[v11] = 0;
			v12 = index;
			LOBYTE(index)--;
			x_BYTE_11B4E0_heightmap[v12] = 0;
			v13 = index;
			HIBYTE(index)--;
			x_BYTE_11B4E0_heightmap[v13] = 0;
		}
		index++;
	} while (index);
}

//----- (00044580) --------------------------------------------------------
void sub_44580()//225580
{
	uint8_t* v0; // eax
	unsigned __int16 v1; // bx
	unsigned __int16 v2; // bx
	char v3; // al
	char v4; // dl
	char v5; // cl
	char v6; // ah
	int v7; // edi
	uint8_t* v8; // esi
	uint8_t v9; // dh
	uint8_t* v10; // esi
	char v11; // dh
	char v12; // al
	char v13; // dl
	char v14; // dh
	int v15; // ST00_4
	char v16; // ah
	uint8_t* v17; // esi
	char v18; // cl
	unsigned __int8 v19; // dh
	uint8_t* v20; // esi
	char v21; // dh
	char v22; // al
	char v23; // ah
	char v24; // dh
	char v25; // dl
	int v26; // edi
	uint8_t* v27; // esi
	char v28; // cl
	unsigned __int8 v29; // dh
	uint8_t* v30; // esi
	char v31; // dh
	char v32; // al
	char v33; // dl
	char v34; // dh
	int v35; // ST00_4
	char v36; // ah
	uint8_t* v37; // esi
	char v38; // cl
	unsigned __int8 v39; // dh
	uint8_t* v40; // esi
	char v41; // dh
	int v42; // edi
	int v43; // esi
	char v44; // al
	uint8_t* v45; // esi
	char v46; // cl
	unsigned __int8 v47; // dh
	uint8_t* v48; // esi
	char v49; // dh
	char v50; // al
	char v51; // dl
	char v52; // dh
	int v53; // edi
	int v54; // esi
	char v55; // ah
	uint8_t* v56; // esi
	char v57; // cl
	unsigned __int8 v58; // dh
	uint8_t* v59; // esi
	char v60; // dh
	char v61; // al
	char v62; // ah
	char v63; // dh
	char v64; // dl
	int v65; // edi
	uint8_t* v66; // esiv
	char v67; // cl
	unsigned __int8 v68; // dh
	uint8_t* v69; // esi
	uint8_t* v70; // eax
	uint8_t v71; // edx
	//char *v72; // esi
	int v72x;
	char i; // bl
	char j; // ch
	char k; // cl
	char l; // bh
	uint8_t* v77; // eax
	uint16_t v78; // cx
	//uint16_t v79; // eax
	char v80; // ST14_1
	//unsigned __int16 v81; // ax
	char v82; // bh
	unsigned __int16 v83; // ax
	int v84; // esi
	uint8_t v85; // al
	uint8_t v86; // bl
	uint8_t* v87; // edx
	int8_t* v89; // [esp+8h] [ebp-10h]
	uint8_t* v90; // [esp+Ch] [ebp-Ch]

	v0 = pdwScreenBuffer;
	v1 = 0;
	while (v1 < 0x961u)
	{
		v1++;
		v0[0] = 0;
		v0 += 25;
	}
	v2 = 0;
	v90 = pdwScreenBuffer;
	v89 = unk_D47E0;
	while (v2 < 0x94u)
	{
		v3 = v89[0];
		if (v89[0] >= 0)
		{
			v4 = v89[1];
			if (v4 >= 0)
			{
				v5 = v89[2];
				if (v5 >= 0)
				{
					v6 = v89[3];
					if (v6 >= 0)
					{
						v7 = 49 * v4 + 7 * v5 + v6 + 343 * v3;
						v8 = v90 + 25 * v7;
						v9 = v8[0];
						if (v8[0] < 0xCu)
						{
							v8[0] = v9 + 1;
							v10 = &v8[v9];
							v10[13] = 0;
							v10[1] = v2;
						}
						v11 = v3;
						v12 = v4;
						v13 = v11;
						v14 = v6;
						v15 = 7 * v6;
						v16 = v5;
						v17 = v90 + 25 * (49 * v13 + v5 + v15 + 343 * v12);
						v18 = v14;
						v19 = v17[0];
						if (v17[0] < 0xCu)
						{
							v17[0] = v19 + 1;
							v20 = &v17[v19];
							v20[13] = 16;
							v20[1] = v2;
						}
						v21 = v12;
						v22 = v16;
						v23 = v21;
						v24 = v13;
						v25 = v18;
						v26 = 49 * v18 + v23 + 7 * v24 + 343 * v22;
						v27 = v90 + 25 * v26;
						v28 = v24;
						v29 = v27[0];
						if (v27[0] < 0xCu)
						{
							v27[0] = v29 + 1;
							v30 = &v27[v29];
							v30[13] = 48;
							v30[1] = v2;
						}
						v31 = v22;
						v32 = v25;
						v33 = v31;
						v34 = v23;
						v35 = 7 * v23;
						v36 = v28;
						v37 = v90 + 25 * (49 * v33 + v28 + v35 + 343 * v32);
						v38 = v34;
						v39 = *v37;
						if (*v37 < 0xCu)
						{
							*v37 = v39 + 1;
							v40 = &v37[v39];
							v40[13] = 32;
							v40[1] = v2;
						}
						v41 = v32;
						v42 = v32;
						v43 = 8 * v32;
						v44 = v38;
						v45 = v90 + 25 * (49 * v33 + v43 - v42 + v36 + 343 * v38);
						v46 = v41;
						v47 = v45[0];
						if (v45[0] < 0xCu)
						{
							v45[0] = v47 + 1;
							v48 = &v45[v47];
							v48[13] = 96;
							v48[1] = v2;
						}
						v49 = v44;
						v50 = v33;
						v51 = v49;
						v52 = v36;
						v53 = v36;
						v54 = 8 * v36;
						v55 = v46;
						v56 = v90 + 25 * (49 * v51 + v54 - v53 + v46 + 343 * v50);
						v57 = v52;
						v58 = v56[0];
						if (v56[0] < 0xCu)
						{
							v56[0] = v58 + 1;
							v59 = &v56[v58];
							v59[13] = 112;
							v59[1] = v2;
						}
						v60 = v50;
						v61 = v55;
						v62 = v60;
						v63 = v51;
						v64 = v57;
						v65 = 49 * v57 + 7 * v63 + v62 + 343 * v61;
						v66 = v90 + 25 * v65;
						v67 = v63;
						v68 = v66[0];
						if (v66[0] < 0xCu)
						{
							v66[0] = v68 + 1;
							v69 = &v66[v68];
							v69[13] = 80;
							v69[1] = v2;
						}
						v70 = &v90[25 * (343 * v64 + 7 * v62 + v67 + 49 * v61)];
						v71 = v70[0];
						if (v70[0] < 0xCu)
						{
							v70[0] = v71 + 1;
							//v71 = v71&0xff;
							v70[v71 + 13] = 64;
							v70[v71 + 1] = v2;
						}
					}
				}
			}
		}
		v2++;
		v89 += 4;
	}
	//v72 = x_BYTE_F2CD0x;//adress 225afd
	v72x = 0;
	for (i = 0; i < 7; i++)
	{
		for (j = 0; j < 7; j++)
		{
			for (k = 0; k < 7; k++)
			{
				for (l = 0; l < 7; l++)
				{
					v77 = &v90[25 * (49 * j + 7 * k + l + 343 * i)];
					if (v77[0])
					{
						x_BYTE_F2CD0x[v72x][0] = v77[1];
						x_BYTE_F2CD0x[v72x][1] = v77[13];
					}
					else
					{
						x_BYTE_F2CD0x[v72x][0] = 1;
						x_BYTE_F2CD0x[v72x][1] = 0;
					}
					//v72 += 2;
					v72x++;
				}
			}
		}
	}
	v78 = 0;//adress 225bbd
	do
	{
		//v79 = v78;
		if (!x_BYTE_10B4E0_terraintype[v78])
		{
			v80 = x_BYTE_13B4E0_angle[v78] & 7;
			LOBYTE(v78)++;
			//v81 = v78;
			v82 = x_BYTE_13B4E0_angle[v78];
			HIBYTE(v78)++;
			v83 = v78;
			LOBYTE(v78)--;
			v84 = 343 * v80 + 49 * (v82 & 7) + (x_BYTE_13B4E0_angle[v78] & 7) + 7 * (x_BYTE_13B4E0_angle[v83] & 7);
			v85 = v90[25 * v84];
			HIBYTE(v78)--;
			if (v85)
			{
				x_WORD_17B4E0 = 9377 * x_WORD_17B4E0 + 9439;
				v86 = x_WORD_17B4E0 % (v85 + 1);
				if (v86 >= v85)
					v86 = 0;
				v87 = (v86 + 25 * v84 + v90);
				//LOWORD(v79) = v78;
				x_BYTE_10B4E0_terraintype[v78] = v87[1];//problem is here!
				x_BYTE_13B4E0_angle[v78] = (x_BYTE_13B4E0_angle[v78] & 7) + v87[13];
			}
			else
			{
				//LOWORD(v79) = v78;
				x_BYTE_10B4E0_terraintype[v78] = 1;
			}
		}
		v78++;
	} while (v78);
}
// 17B4E0: using guessed type __int16 x_WORD_17B4E0;
// 180628: using guessed type int pdwScreenBuffer;

//----- (00043B40) --------------------------------------------------------
void sub_43B40()//224b40
{
	uint16_t v0; // dx
	uint8_t v1; // eax
	uint8_t v2; // bl
	uint8_t v3; // bh
	uint8_t v4; // cl

	v0 = 0;
	do
	{
		v1 = x_BYTE_11B4E0_heightmap[v0];
		if (v1 > x_BYTE_D41B7)
			v1 = x_BYTE_D41B7;
		v2 = x_BYTE_D41B7 - v1;
		x_BYTE_14B4E0_second_heightmap[v0] = x_BYTE_D41B7 - v1;
		v3 = x_BYTE_11B4E0_heightmap[v0];
		if (v2 > v3)
		{
			x_BYTE_13B4E0_angle[v0] &= 0xF7u;
		}
		else
		{
			v4 = x_BYTE_13B4E0_angle[v0] | 8;
			x_BYTE_14B4E0_second_heightmap[v0] = v3 - 1;
			x_BYTE_13B4E0_angle[v0] = v4;
		}
		v0++;
	} while (v0);
	sub_43BB0();
}
// D41B7: using guessed type char x_BYTE_D41B7;

//----- (00043D50) --------------------------------------------------------
void sub_43D50()//224d50
{
	uint16_t index; // ax
	uint16_t v1; // dx
	//uint16_t v2; // bx
	//uint16_t v3; // bx
	//uint16_t v4; // bx
	//uint16_t v5; // bx
	//uint16_t v6; // bx
	uint16_t v7; // bx
	uint16_t v8; // dx
	uint16_t v9; // bx
	uint16_t v10; // bx

	index = 0;
	do
	{
		x_BYTE_13B4E0_angle[index] &= 0xF7u;
		if (!x_BYTE_11B4E0_heightmap[index])
		{
			HIBYTE(index)--;
			v1 = index;
			LOBYTE(index)++;
			//v2 = index;
			LOBYTE(v1) = (x_BYTE_11B4E0_heightmap[index] != 0) + (x_BYTE_11B4E0_heightmap[v1] != 0);
			HIBYTE(index)++;
			//v3 = index;
			LOBYTE(v1) = (x_BYTE_11B4E0_heightmap[index] != 0) + v1;
			HIBYTE(index)++;
			//v4 = index;
			LOBYTE(v1) = (x_BYTE_11B4E0_heightmap[index] != 0) + v1;
			LOBYTE(index)--;
			//v5 = index;
			LOBYTE(v1) = (x_BYTE_11B4E0_heightmap[index] != 0) + v1;
			LOBYTE(index)--;
			//v6 = index;
			LOBYTE(v1) = (x_BYTE_11B4E0_heightmap[index] != 0) + v1;
			HIBYTE(index)--;
			v7 = index;
			HIBYTE(index)--;
			LOBYTE(v1) = (x_BYTE_11B4E0_heightmap[v7] != 0) + v1;
			LOBYTE(v7) = x_BYTE_11B4E0_heightmap[index] != 0;
			LOBYTE(index)++;
			HIBYTE(index)++;
			if (!(LOBYTE(v7) + LOBYTE(v1)))
			{
				v8 = index;
				LOBYTE(index)--;
				v9 = index;
				HIBYTE(index)--;
				LOBYTE(v8) = (x_BYTE_10B4E0_terraintype[v9] != 0) + (x_BYTE_10B4E0_terraintype[v8] != 0);
				v10 = index;
				LOBYTE(index)++;
				LOBYTE(v8) = (x_BYTE_10B4E0_terraintype[index] != 0) + (x_BYTE_10B4E0_terraintype[v10] != 0) + v8;
				HIBYTE(index)++;
				if (!(x_BYTE)v8)
					x_BYTE_13B4E0_angle[index] |= 8u;
			}
		}
		index++;
	} while (index);
}

int debugcounter_225d00 = 0;
//----- (00044D00) --------------------------------------------------------
void sub_44D00()//225d00
{
	uaxis_2d v0x; // cx
	//uaxis_2d v1x; // dx
	uaxis_2d indexx; // eax
	//unsigned __int16 v3; // cx
	//uaxis_2d v4x; // et2
	char v5; // dl

	v0x.word = 0;
	x_WORD_17B4E0 = 0;
	do
	{
		//adress 225d0f
		if (debugcounter_225d00 >= 0xb03)
		{
			debugcounter_225d00++;
			debugcounter_225d00--;
		}
		debugcounter_225d00++;

		v0x._axis_2d.x++;
		//indexx._axis_2d.y = 0;
		v0x._axis_2d.y++;
		indexx.word = v0x.word;
		v0x._axis_2d.x -= 2;
		v0x._axis_2d.y -= 2;
		//v1x.word = v0x.word;

		indexx._axis_2d.x = x_BYTE_11B4E0_heightmap[v0x.word] - x_BYTE_11B4E0_heightmap[indexx.word] + 32;
		v0x._axis_2d.x++;
		v0x._axis_2d.y++;
		if (indexx._axis_2d.x == 32)
		{
			indexx.word = 9377 * x_WORD_17B4E0 + 9439;
			//LOWORD(index) += 9439;
			x_WORD_17B4E0 = indexx.word;
			//v4x.word = indexx.word;
			indexx._axis_2d.y = (x_WORD_17B4E0 / 9u) >> 8;
			indexx._axis_2d.x = x_WORD_17B4E0 % 9 + 28;
		}
		else if ((int8_t)indexx._axis_2d.x >= 28)
		{
			if ((int8_t)indexx._axis_2d.x > 40)
				indexx._axis_2d.x = (indexx._axis_2d.x & 7) + 40;
		}
		else
		{
			indexx._axis_2d.x = (indexx._axis_2d.x & 3) + 28;
		}
		if (D41A0_0.terrain_2FECE.MapType != MapType_t::Day)
		{
			//index = 32 - (index & 0xff);
			//v5 = (32 - (indexx._axis_2d.x)) + 32;
			v5 = (64 - indexx._axis_2d.x);
		}
		else
		{
			v5 = indexx._axis_2d.x;
		}
		//LOBYTE(index) = v5;
		x_BYTE_12B4E0_shading[v0x.word] = v5;
		v0x.word++;
	} while (v0x.word);
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// 17B4E0: using guessed type __int16 x_WORD_17B4E0;

//----- (000B5EFA) --------------------------------------------------------
void sub_B5EFA(__int16 a1, uint16_t* a2, int32_t a3, __int16* a4)//296EFA
{
	int16_t v4; // di
	//__int16 v5; // di
	//__int16 v6; // di
	uint16_t v7; // si
	uint32_t v8; // di
	//__int16 result; // ax

	v4 = mapEntityIndex_15B4E0[*a2];
	LOBYTE(*a2) += (a1 + a1);
	v4 += mapEntityIndex_15B4E0[*a2];
	*a2 += (a1 + a1) << 8;
	v4 += mapEntityIndex_15B4E0[*a2];
	LOBYTE(*a2) -= (a1 + a1);
	v4 += mapEntityIndex_15B4E0[*a2];
	LOBYTE(*a2) += a1;
	*a2 -= a1 << 8;
	v7 = 9377 * *a4 + 9439;
	*a4 = v7;
	v8 = v7 % (uint16_t)(2 * a3 + 1)
		+ v7 % (uint16_t)((a1 << 6) + 1)
		+ (v4 >> 2)
		- 32 * a1
		- a3;
	//result = v8;
	if (!mapEntityIndex_15B4E0[*a2])
		mapEntityIndex_15B4E0[*a2] = v8;
	LOBYTE(*a2) += a1;
	*a2 -= a1 << 8;
	//return result;
}

//----- (000B5F8F) --------------------------------------------------------
void sub_B5F8F(__int16 a1, uint16_t* a2, int32_t a3, __int16* a4)//296f8f
{
	__int16 v4; // di
	//__int16 v5; // di
	//__int16 v6; // di
	uint16_t v7; // si
	uint16_t v7b; // si
	uint16_t v8; // ax
	int16_t v9; // di
	//__int16 v10; // di
	//__int16 v11; // di
	uint32_t v12; // di
	//__int16 result; // ax
	uint16_t v14; // [esp-2h] [ebp-2h]

	v4 = mapEntityIndex_15B4E0[*a2];
	v14 = v4;
	LOBYTE(*a2) += a1;
	*a2 -= a1 << 8;
	v4 += mapEntityIndex_15B4E0[*a2];
	LOBYTE(*a2) += a1;
	*a2 += a1 << 8;
	v4 += mapEntityIndex_15B4E0[*a2];
	LOBYTE(*a2) -= a1;
	*a2 += a1 << 8;
	v4 += mapEntityIndex_15B4E0[*a2];
	v7 = 9377 * *a4 + 9439;
	//*a4 = v7;
	//2ae9*24a1
	v8 = v7 % (uint16_t)(2 * a3 + 1)
		+ v7 % (uint16_t)((a1 << 6) + 1)
		+ (uint16_t)(v4 >> 2)
		- 32 * a1
		- a3;
	//(uint16_t)(v4 >> 2)-(a1*32)+v7 % (uint16_t)((a1 << 6) + 1)-a3+v7 % (uint16_t)(2 * a3 + 1)
	v9 = mapEntityIndex_15B4E0[*a2];
	*a2 -= a1 << 8;
	if (!mapEntityIndex_15B4E0[*a2])
		mapEntityIndex_15B4E0[*a2] = v8;
	v9 += v14;
	LOBYTE(*a2) -= (a1 + a1);
	*a2 += a1 << 8;
	v9 += mapEntityIndex_15B4E0[*a2];
	LOBYTE(*a2) += a1;
	*a2 += a1 << 8;
	v9 += mapEntityIndex_15B4E0[*a2];
	*a2 -= a1 << 8;
	v7b = 9377 * v7 + 9439;
	*a4 = v7b;
	v12 = v7b % (unsigned __int16)(2 * a3 + 1)
		+ v7b % (unsigned __int16)((a1 << 6) + 1)
		+ (uint16_t)(v9 >> 2)
		- 32 * a1
		- a3;
	//result = v12;
	if (!mapEntityIndex_15B4E0[*a2])
		mapEntityIndex_15B4E0[*a2] = v12;
	*a2 -= a1 << 8;
	LOBYTE(*a2) += (a1 + a1);
	//return result;
}

//----- (00044EE0) --------------------------------------------------------
void sub_44EE0_smooth_tiles(/*int a1,*/ uaxis_2d a2x)//225ee0
{
	uaxis_2d v2x; // eax
	uint16_t v3; // dx
	unsigned __int8 v4; // dh
	//uaxis_2d v5x; // si
	//uaxis_2d v6x; // esi
	unsigned __int8 v7; // dl
	uint16_t index; // ax

	//[*][*][*]
	//[*][X][*]
	//[*][*][*]

	uaxis_2d a1x;

	v2x.word = a2x.word;
	v3 = 0;
	do
		x_BYTE_10B4E0_terraintype[v3++] = 3;
	while (v3);//set all tiles to 3
	v4 = x_BYTE_11B4E0_heightmap[a2x.word];
	do
	{
		//v5x.word = v2x.word;
		x_BYTE_10B4E0_terraintype[v2x.word] = 0;
		//BYTE1(v2)--;
		v2x._axis_2d.y--;

		//v6 = v5;
		//v6 = v2;
		v7 = 0xFFu;
		if (x_BYTE_10B4E0_terraintype[v2x.word] && x_BYTE_11B4E0_heightmap[v2x.word] < 0xFFu)
		{
			v7 = x_BYTE_11B4E0_heightmap[v2x.word];
			a1x.word = v2x.word;
		}
		//LOBYTE(v2)++;
		v2x._axis_2d.x++;
		if (x_BYTE_10B4E0_terraintype[v2x.word] && v7 > x_BYTE_11B4E0_heightmap[v2x.word])
		{
			v7 = x_BYTE_11B4E0_heightmap[v2x.word];
			a1x.word = v2x.word;
		}
		//BYTE1(v2)++;
		v2x._axis_2d.y++;
		if (x_BYTE_10B4E0_terraintype[v2x.word] && v7 > x_BYTE_11B4E0_heightmap[v2x.word])
		{
			v7 = x_BYTE_11B4E0_heightmap[v2x.word];
			a1x.word = v2x.word;
		}
		//BYTE1(v2)++;
		v2x._axis_2d.y++;
		if (x_BYTE_10B4E0_terraintype[v2x.word] && v7 > x_BYTE_11B4E0_heightmap[v2x.word])
		{
			v7 = x_BYTE_11B4E0_heightmap[v2x.word];
			a1x.word = v2x.word;
		}
		//LOBYTE(v2)--;
		v2x._axis_2d.x--;
		if (x_BYTE_10B4E0_terraintype[v2x.word] && v7 > x_BYTE_11B4E0_heightmap[v2x.word])
		{
			v7 = x_BYTE_11B4E0_heightmap[v2x.word];
			a1x.word = v2x.word;
		}
		//LOBYTE(v2)--;
		v2x._axis_2d.x--;
		if (x_BYTE_10B4E0_terraintype[v2x.word] && v7 > x_BYTE_11B4E0_heightmap[v2x.word])
		{
			v7 = x_BYTE_11B4E0_heightmap[v2x.word];
			a1x.word = v2x.word;
		}
		//BYTE1(v2)--;
		v2x._axis_2d.y--;
		if (x_BYTE_10B4E0_terraintype[v2x.word] && v7 > x_BYTE_11B4E0_heightmap[v2x.word])
		{
			v7 = x_BYTE_11B4E0_heightmap[v2x.word];
			a1x.word = v2x.word;
		}
		//BYTE1(v2)--;
		v2x._axis_2d.y--;
		if (x_BYTE_10B4E0_terraintype[v2x.word] && v7 > x_BYTE_11B4E0_heightmap[v2x.word])
		{
			v7 = x_BYTE_11B4E0_heightmap[v2x.word];
			a1x.word = v2x.word;
		}
		if (!x_BYTE_13B4E0_angle[a1x.word] || v7 == 0xFFu)
			break;
		if (v7 > v4)//if near tile is higger then central tile set central as near tile
			x_BYTE_11B4E0_heightmap[a1x.word] = v4;
		v4 = x_BYTE_11B4E0_heightmap[a1x.word];
		v2x.word = a1x.word;
	} while (v4);
	index = 0;
	do
	{
		if (!x_BYTE_10B4E0_terraintype[index])
			x_BYTE_13B4E0_angle[index] = 0;
		index++;
	} while (index);//delete type
}

//----- (000439A0) --------------------------------------------------------
unsigned int sub_439A0(unsigned int a1, unsigned __int16 a2)//2249a0
{
	unsigned __int16 v2; // ax
	unsigned __int8 v3; // dl
	unsigned __int8 v4; // dh
	unsigned __int8 v5; // ch
	unsigned __int8 v6; // cl
	int v7; // ebx
	unsigned __int8 v8; // cl
	int v9; // ebx
	unsigned __int8 v10; // cl
	int v11; // ebx
	unsigned __int8 v12; // cl
	int v13; // ebx
	unsigned __int8 v14; // cl
	int v15; // ebx
	unsigned __int8 v16; // cl
	int v17; // ebx
	unsigned __int8 v18; // cl
	int v19; // ebx
	unsigned __int8 v20; // al
	unsigned int v21; // ebx
	unsigned int v22; // ebx

	LOBYTE(v2) = a2;
	LOWORD(a1) = x_BYTE_11B4E0_heightmap[a2];
	if (x_BYTE_13B4E0_angle[a2] & 7)
	{
		HIBYTE(v2) = HIBYTE(a2) - 1;
		v3 = x_BYTE_11B4E0_heightmap[a2];
		v4 = v3;
		v5 = x_BYTE_11B4E0_heightmap[a2];
		v6 = x_BYTE_11B4E0_heightmap[v2];
		v7 = v6;
		if (v6 > v3)
			v3 = x_BYTE_11B4E0_heightmap[v2];
		if (v6 < v4)
			v4 = x_BYTE_11B4E0_heightmap[v2];
		LOBYTE(v2) = a2 + 1;
		v8 = x_BYTE_11B4E0_heightmap[v2];
		v9 = v8 + v7;
		if (v8 > v3)
			v3 = x_BYTE_11B4E0_heightmap[v2];
		if (v8 < v4)
			v4 = x_BYTE_11B4E0_heightmap[v2];
		HIBYTE(v2) = HIBYTE(a2);
		v10 = x_BYTE_11B4E0_heightmap[v2];
		v11 = v10 + v9;
		if (v10 > v3)
			v3 = x_BYTE_11B4E0_heightmap[v2];
		if (v10 < v4)
			v4 = x_BYTE_11B4E0_heightmap[v2];
		HIBYTE(v2) = HIBYTE(a2) + 1;
		v12 = x_BYTE_11B4E0_heightmap[v2];
		v13 = v12 + v11;
		if (v12 > v3)
			v3 = x_BYTE_11B4E0_heightmap[v2];
		if (v12 < v4)
			v4 = x_BYTE_11B4E0_heightmap[v2];
		LOBYTE(v2) = a2;
		v14 = x_BYTE_11B4E0_heightmap[v2];
		v15 = v14 + v13;
		if (v14 > v3)
			v3 = x_BYTE_11B4E0_heightmap[v2];
		if (v14 < v4)
			v4 = x_BYTE_11B4E0_heightmap[v2];
		LOBYTE(v2) = a2 - 1;
		v16 = x_BYTE_11B4E0_heightmap[v2];
		v17 = v16 + v15;
		if (v16 > v3)
			v3 = x_BYTE_11B4E0_heightmap[v2];
		if (v16 < v4)
			v4 = x_BYTE_11B4E0_heightmap[v2];
		HIBYTE(v2) = HIBYTE(a2);
		v18 = x_BYTE_11B4E0_heightmap[v2];
		v19 = v18 + v17;
		if (v18 > v3)
			v3 = x_BYTE_11B4E0_heightmap[v2];
		if (v18 < v4)
			v4 = x_BYTE_11B4E0_heightmap[v2];
		HIBYTE(v2) = HIBYTE(a2) - 1;
		v20 = x_BYTE_11B4E0_heightmap[v2];
		v21 = v20 + v19;
		if (v20 > v3)
			v3 = v20;
		if (v20 < v4)
			v4 = v20;
		v22 = v21 >> 3;
		if ((unsigned __int8)(v5 - v4) <= 4u)
		{
			if ((unsigned __int8)(v3 - v5) <= 4u)
				return a1;
			if ((unsigned __int8)(v3 - v5) <= 0xAu)
				v22 = (v5 + v22) >> 1;
		}
		else if ((unsigned __int8)(v5 - v4) <= 0xAu)
		{
			return (v22 + v5) >> 1;
		}
		a1 = v22;
	}
	return a1;
}

//----- (00043BB0) --------------------------------------------------------
void sub_43BB0()//224bb0
{
	unsigned int v0; // esi
	unsigned __int16 v1; // cx
	signed int v2; // ebx
	unsigned __int16 v3; // bx
	//int v4; // eax
	unsigned __int8 v5; // dh
	char v6; // cl

	v0 = 37487429;
	v1 = 0;
	do
	{
		if (!(x_BYTE_13B4E0_angle[v1] & 8))
		{
			v0 = 9377 * v0 + 9439;
			v2 = v0 % 7 - 3 + (unsigned __int8)x_BYTE_14B4E0_second_heightmap[v1];
			if (v2 < 0)
				v2 = 0;
			if (v2 > 254)
				LOBYTE(v2) = -2;
			x_BYTE_14B4E0_second_heightmap[v1] = v2;
		}
		v1++;
	} while (v1);
	v3 = 0;
	do
	{
		//v4 = v3;
		v5 = x_BYTE_11B4E0_heightmap[v3];
		if ((unsigned __int8)x_BYTE_14B4E0_second_heightmap[v3] > v5)
		{
			x_BYTE_13B4E0_angle[v3] &= 0xF7u;
		}
		else
		{
			v6 = x_BYTE_13B4E0_angle[v3] | 8;
			x_BYTE_14B4E0_second_heightmap[v3] = v5 - 1;
			x_BYTE_13B4E0_angle[v3] = v6;
		}
		v3++;
	} while (v3);
}



