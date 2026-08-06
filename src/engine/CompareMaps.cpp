#include "CompareMaps.h"

//for debuging
int countcompindexes = 0;

type_compstr compstr[100];
int getcompstrindex(uint32_t address) {
	int findindex = -1;
	for (int i = 0; i < countcompindexes; i++)
	{
		if (compstr[i].adress == address)
		{
			findindex = i;
			break;
		}
	}
	return findindex;
};

int getcompindex(uint32_t adress) {

	int findindex = getcompstrindex(adress);

	if (findindex > -1)
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
				{
					int i = getcompstrindex(adress);
					if (i > -1)
					{
						compstr[i].index = 0;
					}
					End_thread(20);
				}
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

void add_compare2(uint32_t adress, uint8_t* memadress, uint32_t dosmemadress, uint32_t size, bool debugafterload, int stopstep, bool skip, int exitindex) {
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
