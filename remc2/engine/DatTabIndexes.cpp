#include "DatTabIndexes.h"
//----- (00098709) --------------------------------------------------------
void CreateIndexDatTabDoubled_98709(bitmap_pos_struct2_t* tabbuffer, bitmap_pos_struct2_t* tabbufferend, uint8_t* datbuffer, bitmap_pos_struct_t* dattabindex)//279709
{
	//int length = (tabbufferend - ((uint8_t*)tabbuffer)) / 6;
	//int length = tabbufferend - tabbuffer;
	for (int i = 0; i < tabbufferend - tabbuffer; i++)
	{
		//int index = tabbuffer[i].data;
		dattabindex[i].data = (datbuffer + tabbuffer[i].data_0);
		dattabindex[i].width_4 = tabbuffer[i].width_4 * 2;
		dattabindex[i].height_5 = tabbuffer[i].height_5 * 2;
	}
}

void CreateIndexDatTabDoubledWithOffset_98709(uint8_t* tabbuffer, uint8_t* tabbufferend, uint8_t* datbuffer, bitmap_pos_struct_t* dattabindex, int add)//279709
{
	for (uint32_t i = 0; i < (tabbufferend - (tabbuffer + add)) / 6; i++)
	{
#ifdef x32_BIT_ENVIRONMENT
		dattabindex[i].data = (uint8_t*)(*(uint32_t*)((tabbuffer + add) + 6 * i)) + reinterpret_cast<int32_t>(datbuffer);
#endif //x32_BIT_ENVIRONMENT
#ifdef x64_BIT_ENVIRONMENT
		dattabindex[i].data = (uint8_t*)(*(uint32_t*)((tabbuffer + add) + 6 * i)) + (int32_t)reinterpret_cast<int64_t>(datbuffer);
#endif //x64_BIT_ENVIRONMENT		
		dattabindex[i].width_4 = (tabbuffer + add)[6 * i + 4] * 2;
		dattabindex[i].height_5 = (tabbuffer + add)[6 * i + 5] * 2;
	}
}

//----- (0009874D) --------------------------------------------------------
void CreateIndexDatTab_9874D(bitmap_pos_struct2_t* tabbuffer, bitmap_pos_struct2_t* tabbufferend, uint8_t* datbuffer, bitmap_pos_struct_t* dattabindex)//27974d
{
	for (uint32_t i = 0; i < tabbufferend - tabbuffer; i++)
	{
		dattabindex[i].data = (datbuffer + tabbuffer[i].data_0);
		dattabindex[i].width_4 = tabbuffer[i].width_4;
		dattabindex[i].height_5 = tabbuffer[i].height_5;
	}
}

void CreateIndexDatTabWithOffset_9874D(uint8_t* tabbuffer, uint8_t* tabbufferend, uint8_t* datbuffer, bitmap_pos_struct_t* dattabindex, int add)//27974d
{
	uint8_t* start_ptr = tabbuffer + add;
	for (uint32_t i = 0; i < (tabbufferend - (tabbuffer + add)) / 6; i++)
	{
		uint32_t file_offset = *(uint32_t*)(start_ptr + (i * 6));
		dattabindex[i].data = datbuffer + file_offset;
		dattabindex[i].width_4 = (tabbuffer + add)[6 * i + 4];
		dattabindex[i].height_5 = (tabbuffer + add)[6 * i + 5];
	}
}

//----- (00099A77) --------------------------------------------------------
void CreateIndexDatTabHalved_99A77(uint8_t* tabbuffer, uint8_t* tabbufferend, uint8_t*  /*datbuffer*/, bitmap_pos_struct_t* dattabindex)//280a77
{
	//uint32_t testadr = *(uint32_t*)tabbuffer;
	/*if (testadr == 0x9999)
	{
		for (uint32_t i = 0;i < (tabbufferend - tabbuffer) / 6;i++)
		{
			dattabindex[i].data -= (int32_t)datbuffer;
			dattabindex[i].width /= 2;
			dattabindex[i].height /= 2;
		}
	}
	else*/
	{
		for (uint32_t i = 0; i < (tabbufferend - tabbuffer) / 6; i++)
		{
			dattabindex[i].data = (uint8_t*)(*(uint32_t*)(tabbuffer + 6 * i))/* + (int32_t)datbuffer*/;//fixed
			dattabindex[i].width_4 = tabbuffer[6 * i + 4] / 2;
			dattabindex[i].height_5 = tabbuffer[6 * i + 5] / 2;
		}
		//testadr = 0x9999;
		//memcpy(tabbuffer, &testadr, 4);
	}
}

//----- (00099AEB) --------------------------------------------------------
void CreateIndexDatTabRelative_99AEB(uint8_t* tabbuffer, uint8_t* tabbufferend, uint8_t*  /*datbuffer*/, bitmap_pos_struct_t* dattabindex)//280aeb
{
	//uint32_t testadr = *(uint32_t*)tabbuffer;
	/*if (testadr == 0x9999)
	{
		for (uint32_t i = 0;i < (tabbufferend - tabbuffer) / 6;i++)
		{
			dattabindex[i].data -= (int32_t)datbuffer;
		}
	}
	else*/
	{
		for (uint32_t i = 0; i < (tabbufferend - tabbuffer) / 6; i++)
		{
			dattabindex[i].data = (uint8_t*)(*(uint32_t*)(tabbuffer + 6 * i))/* + (int32_t)datbuffer*/;//fixed
			dattabindex[i].width_4 = tabbuffer[6 * i + 4];
			dattabindex[i].height_5 = tabbuffer[6 * i + 5];
		}
		//testadr = 0x9999;
		//memcpy(tabbuffer, &testadr, 4);
	}
}

//----- (0006EB90) --------------------------------------------------------
void CreateIndexes_6EB90(filearray_struct* a1)//24fb90
{
	for (uint32_t i = 0; (a1[i].begin_buffer) != &ZERO_BUFFER_PST2; i++)
	{
		if (x_WORD_180660_VGA_type_resolution & 1)
			CreateIndexDatTabDoubled_98709(*a1[i].begin_buffer, *a1[i].end_buffer, *a1[i].dat_buffer, *a1[i].posistruct);//279709 //1a6f44,1a6f86,1a6578
		else
			CreateIndexDatTab_9874D(*a1[i].begin_buffer, *a1[i].end_buffer, *a1[i].dat_buffer, *a1[i].posistruct);
	}
}

