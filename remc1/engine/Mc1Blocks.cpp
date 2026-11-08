#include "Mc1Blocks.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "Mc1Files.h"
#include "Mc1Game.h"
#include "Mc1System.h"
#include "../../remc2/engine/EventsFunctions.h"
#include "../../remc2/utilities/DataFileIO.h"
#include "../../remc2/utilities/Maths.h"

uint8_t* blockPointers_9334C_9334C[256];
Type_Mc1Tables_B7934 tables_B7934_B7924;
uint8_t tablesMissing_90B48_90B48 = 0;
// the original walks the heap after each allocation (63338 / 63848) and keeps the largest free block;
// the port is not short of memory, so the largest heap and 32x32 blocks are always chosen
uint32_t largestFreeBlock_1314AC_13149C = 0x1000000;
uint8_t byte_968F0_968F0 = 0;
uint8_t byte_939EC_939EC = 0;

static void NotEnoughMemory()
{
	printf("ERROR : NOT ENOUGH MEMORY.\n");
	exit(1);
}

void InitMemory_41230_41570()
{
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	Type_Mc1Settings_AE408_AE3F8* settings = mc1Settings_AE408_AE3F8;
	if (settings->heap_0xA8_168)
		return;
	void* temp = Malloc_42540_42880(60000);
	byte_968F0_968F0 = 1;
	state->tileTextureSize_0x21A0_8608 = 0x20;
	uint32_t freeBlock = largestFreeBlock_1314AC_13149C;
	if (freeBlock > 0x800000)
	{
		byte_939EC_939EC = 1;
		settings->heapSize_0xAC_172 = 0x400000;
	}
	else if (freeBlock > 0x700000)
	{
		settings->heapSize_0xAC_172 = freeBlock - 0x33DBB0;
		byte_939EC_939EC = 1;
	}
	else if (freeBlock > 0x400000)
	{
		settings->heapSize_0xAC_172 = freeBlock - 0x249970;
		state->options_0x21AD_8621[10] = 0;
		byte_939EC_939EC = 1;
	}
	else if (freeBlock > 0x200000)
	{
		settings->heapSize_0xAC_172 = freeBlock - 0x155730;
		byte_939EC_939EC = 0;
		state->options_0x21AD_8621[10] = 0;
	}
	else if (freeBlock > 0x100000)
	{
		settings->heapSize_0xAC_172 = freeBlock - 0x68FB0;
		state->tileTextureSize_0x21A0_8608 = 0x10;
		byte_939EC_939EC = 3;
		state->options_0x21AD_8621[10] = 0;
	}
	else
		NotEnoughMemory(); // the code after exit in the original is never reached

	settings->heap_0xA8_168 = (uint8_t*)Malloc_42540_42880(settings->heapSize_0xAC_172);
	if (settings->heapSize_0xAC_172 <= 0x96000)
		NotEnoughMemory();
	uint8_t size = (uint8_t)state->tileTextureSize_0x21A0_8608;
	if (size == 0x10)
		LoadFileGroup_40440_40780(blockFiles16_996E0_996E0);
	else if (size == 0x20)
		LoadFileGroup_40440_40780(blockFiles32_99738_99738);
	InitBlockTables_44470_447B0();
	Free_426E0_42A20(temp);
}

// 44547-446E0 / 44887-44A20 of InitBlockTables, a function of its own in the port
static void GenerateTables(Type_Mc1Tables_B7934* tables)
{
	const uint8_t* palette = palette_AE428_AE418;
	MakeBlockColors_72807_72D17(palette, tables->blockColors_0x14000);
	uint16_t factor = 0x100;
	for (uint16_t i = 0; i < 32; i++)
	{
		MakeFadeTable_40EDC_4121C(palette, tables->fades_0x0000[i], palette[0x2FD], palette[0x2FE],
			palette[0x2FF], factor, factor, factor);
		factor -= 8;
	}
	factor = 0;
	for (uint16_t i = 32; i < 64; i++)
	{
		MakeFadeTable_40EDC_4121C(palette, tables->fades_0x0000[i], palette[0], palette[1], palette[2],
			factor, factor, factor);
		factor += 8;
	}
	for (uint16_t i = 0; i < 256; i++)
		MakeFadeTable_40EDC_4121C(palette, tables->blend_0x4000[i], palette[i * 3], palette[i * 3 + 1],
			palette[i * 3 + 2], 0x55, 0x55, 0x55);
	for (uint32_t i = 0; i < 256; i++)
		tables->distances_0x14300[i] = (uint8_t)IntegerSqrt_410CE_4140E(0x10000 - i * i);
	tables->distances_0x14300[0] = 0xFF;
}

void InitBlockTables_44470_447B0()
{
	int32_t size = mc1State_AE400_AE3F0->tileTextureSize_0x21A0_8608;
	FillUVTable_29BA0_29BE0(size);
	uint32_t perRow = 256 / size;
	int32_t rows = (int32_t)(256 / perRow);
	uint8_t** pointer = blockPointers_9334C_9334C;
	for (int32_t row = 0; row < rows; row++)
		for (uint32_t column = 0; column < perRow; column++)
			*pointer++ = blockData_AE3F0_AE3E0 + row * (size << 8) + column * size;

	const char* path = mc1Variant == MC1_VARIANT_HW ? "data/dtables.dat" : "data/tables.dat";
	uint8_t* tables = (uint8_t*)&tables_B7934_B7924;
	if (!tablesMissing_90B48_90B48 && ReadPackedFile_3EEA0_3F1E0(path, tables) <= 0)
		tablesMissing_90B48_90B48 = 1;
	if (!tablesMissing_90B48_90B48)
		return;

	GenerateTables(&tables_B7934_B7924);
	std::string fullPath = Mc1DataFilePath(path);
	OpenWriteClose_62E60_63370(fullPath.c_str(), tables, sizeof(tables_B7934_B7924));
}

static uint8_t FadeComponent(uint8_t color, uint8_t target, int16_t factor)
{
	// 40EF4 / 41234: al = target - color, movsx ax, al, imul factor, color += ah
	int32_t product = (int16_t)(int8_t)(uint8_t)(target - color) * factor;
	return (uint8_t)(color + (uint8_t)(product >> 8));
}

void MakeFadeTable_40EDC_4121C(const uint8_t* palette, uint8_t* table, uint8_t targetRed, uint8_t targetGreen,
	uint8_t targetBlue, int16_t factorRed, int16_t factorGreen, int16_t factorBlue)
{
	const uint8_t* color = palette;
	for (int32_t i = 0; i < 256; i++, color += 3)
	{
		uint8_t red = FadeComponent(color[0], targetRed, factorRed);
		uint8_t green = FadeComponent(color[1], targetGreen, factorGreen);
		uint8_t blue = FadeComponent(color[2], targetBlue, factorBlue);
		// the nearest color, 16 bit sums as in the original (red and green count twice)
		int16_t best = 0x7FFF;
		uint8_t bestIndex = 0; // uninitialized in the original
		const uint8_t* entry = palette;
		for (int32_t j = 0; j < 256; j++, entry += 3)
		{
			int8_t dr = (int8_t)(uint8_t)(entry[0] - red);
			int8_t dg = (int8_t)(uint8_t)(entry[1] - green);
			int8_t db = (int8_t)(uint8_t)(entry[2] - blue);
			int16_t distance = (int16_t)(2 * (dr * dr));
			distance = (int16_t)(distance + (int16_t)(2 * (dg * dg)));
			distance = (int16_t)(distance + (int16_t)(db * db));
			if (distance < best)
			{
				bestIndex = (uint8_t)j;
				best = distance;
			}
		}
		table[i] = bestIndex;
	}
}

static std::vector<uint8_t> DosBlockMemory()
{
	// the original reads 0x40000 bytes from the block data (blk?-1 has only 0x26000): in DOS sky.dat follows
	// directly, then zeros (and 8 bytes of heap headers, not emulated); the port copies that into one image
	std::vector<uint8_t> image(0x40000, 0);
	Type_Mc1FileEntry* group = mc1State_AE400_AE3F0->tileTextureSize_0x21A0_8608 == 0x10 ? blockFiles16_996E0_996E0 : blockFiles32_99738_99738;
	size_t offset = 0;
	for (Type_Mc1FileEntry* entry = group; entry->data_0x1C && offset < image.size(); entry++)
	{
		size_t length = (std::min)((size_t)entry->size_0x24, image.size() - offset);
		if (*entry->data_0x1C)
			memcpy(image.data() + offset, *entry->data_0x1C, length);
		offset += length;
	}
	return image;
}

void MakeBlockColors_72807_72D17(const uint8_t* palette, uint8_t* colors)
{
	std::vector<uint8_t> memory = DosBlockMemory();
	for (int32_t block = 0; block < 256; block++)
	{
		// average of 32x32 pixels whatever the block size, rows 256 bytes apart
		const uint8_t* pixel = memory.data() + (blockPointers_9334C_9334C[block] - blockData_AE3F0_AE3E0);
		uint32_t red = 0;
		uint32_t green = 0;
		uint32_t blue = 0;
		for (int32_t y = 0; y < 32; y++)
		{
			for (int32_t x = 0; x < 32; x++)
			{
				const uint8_t* color = palette + *pixel * 3;
				red += color[0];
				green += color[1];
				blue += color[2];
				pixel++;
			}
			pixel += 0xE0;
		}
		red >>= 10;
		green >>= 10;
		blue >>= 10;
		int16_t best = 0x7FFF;
		uint8_t bestIndex = 0;
		const uint8_t* entry = palette;
		for (int32_t j = 0; j < 256; j++, entry += 3)
		{
			int16_t dr = (int16_t)(entry[0] - (uint16_t)red);
			int16_t dg = (int16_t)(entry[1] - (uint16_t)green);
			int16_t db = (int16_t)(entry[2] - (uint16_t)blue);
			int16_t distance = (int16_t)(dr * dr);
			distance = (int16_t)(distance + (int16_t)(dg * dg));
			distance = (int16_t)(distance + (int16_t)(db * db));
			if (distance < best)
			{
				best = distance;
				bestIndex = (uint8_t)j;
			}
		}
		colors[block] = bestIndex;
		colors[block + 0x80] = bestIndex;
	}
}
