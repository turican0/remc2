#include "Mc1Files.h"

#include <cstdio>

#include "Mc1Game.h"
#include "Mc1System.h"
#include "../../remc2/portability/port_filesystem.h"
#include "../../remc2/utilities/DataFileIO.h"
#include "../../remc2/utilities/DataFileRNC.h"
#include "../../remc2/engine/DatTabIndexes.h"

std::string mc1DataPath;

uint8_t* searchData_AE3F4_AE3E4 = nullptr;
uint8_t* searchDataEnd_AE414_AE404 = nullptr;
uint8_t* buildData_AE430_AE420 = nullptr;
uint8_t* buildDataEnd_AE438_AE428 = nullptr;
uint8_t* buildTab_AE440_AE430 = nullptr;
uint8_t* buildTabEnd_AE40C_AE3FC = nullptr;
uint8_t* font0Data_AE378_AE368 = nullptr;
uint8_t* font1Data_AE37C_AE36C = nullptr;
uint8_t* fontTabs_AE3B8_AE3A8[8];
uint8_t* font0TabEnd_AE398_AE388 = nullptr;
uint8_t* font1TabEnd_AE39C_AE38C = nullptr;
uint8_t* tmapsTab_12D744_12D734 = nullptr;
uint8_t* palData_AE424_AE414 = nullptr;
uint8_t* palMem_AE418_AE408 = nullptr;
uint8_t* pointersData_AE448_AE438 = nullptr;
uint8_t* pointersTab_AE450_AE440 = nullptr;
uint8_t* pointersTabEnd_AE43C_AE42C = nullptr;
uint8_t* palette_AE428_AE418 = nullptr;
uint8_t* workScreen_12EFF4_12EFE4 = nullptr;
uint8_t* backScreen_AE3FC_AE3EC = nullptr;
uint8_t* spritesData_AE444_AE434 = nullptr;
uint8_t* spritesTab_AE42C_AE41C = nullptr;
uint8_t* spritesTabEnd_AE434_AE424 = nullptr;
uint8_t* textData_AE41C_AE40C = nullptr;
uint8_t* blockData_AE3F0_AE3E0 = nullptr;
uint8_t* skyData_AE3D8_AE3C8 = nullptr;

bitmap_pos_struct_t* pointersIndex = nullptr;
bitmap_pos_struct_t* fontIndexes[8];
bitmap_pos_struct_t* spritesIndex = nullptr;
bitmap_pos_struct_t* buildIndex = nullptr;

Type_Mc1TabRelocation tabRelocations_99974_99974[6] = {
	{ &pointersTab_AE450_AE440, &pointersTabEnd_AE43C_AE42C, &pointersData_AE448_AE438, &pointersIndex },
	{ &fontTabs_AE3B8_AE3A8[0], &font0TabEnd_AE398_AE388, &font0Data_AE378_AE368, &fontIndexes[0] },
	{ &fontTabs_AE3B8_AE3A8[1], &font1TabEnd_AE39C_AE38C, &font1Data_AE37C_AE36C, &fontIndexes[1] },
	{ &spritesTab_AE42C_AE41C, &spritesTabEnd_AE434_AE424, &spritesData_AE444_AE434, &spritesIndex },
	{ &buildTab_AE440_AE430, &buildTabEnd_AE40C_AE3FC, &buildData_AE430_AE420, &buildIndex },
	{ nullptr, nullptr, nullptr, nullptr },
};

Type_Mc1FileEntry searchDFiles_99318_99318[14] = {
	{ { "*SearchD", "*SearchD" }, &searchData_AE3F4_AE3E4, &searchDataEnd_AE414_AE404, 0x1000, 0 },
	{ { "data/build0-0.dat", "data/build1-0.dat" }, &buildData_AE430_AE420, &buildDataEnd_AE438_AE428, 0, 0 },
	{ { "data/build0-0.tab", "data/build1-0.tab" }, &buildTab_AE440_AE430, &buildTabEnd_AE40C_AE3FC, 0, 0 },
	{ { "data/font0.dat", "data/font0.dat" }, &font0Data_AE378_AE368, nullptr, 0, 0 },
	{ { "data/font0.tab", "data/font0.tab" }, &fontTabs_AE3B8_AE3A8[0], &font0TabEnd_AE398_AE388, 0, 0 },
	{ { "data/font1.dat", "data/font1.dat" }, &font1Data_AE37C_AE36C, nullptr, 0, 0 },
	{ { "data/font1.tab", "data/font1.tab" }, &fontTabs_AE3B8_AE3A8[1], &font1TabEnd_AE39C_AE38C, 0, 0 },
	{ { "data/tmaps0-0.tab", "data/tmaps1-0.tab" }, &tmapsTab_12D744_12D734, nullptr, 0, 0 },
	{ { "*PalData", "*PalData" }, &palData_AE424_AE414, nullptr, 0x400, 0 },
	{ { "*PalMem", "*PalMem" }, &palMem_AE418_AE408, nullptr, 0x400, 0 },
	{ { "data/pointers.dat", "data/pointers.dat" }, &pointersData_AE448_AE438, nullptr, 0, 0 },
	{ { "data/pointers.tab", "data/pointers.tab" }, &pointersTab_AE450_AE440, &pointersTabEnd_AE43C_AE42C, 0, 0 },
	{ { "data/pal0-0.dat", "data/pal1-0.dat" }, &palette_AE428_AE418, nullptr, 0, 0 },
	{ { "", "" }, nullptr, nullptr, 0, 0 },
};

Type_Mc1FileEntry screenFilesMcga_997BC_997BC[5] = {
	{ { "*WScreen", "*WScreen" }, &workScreen_12EFF4_12EFE4, nullptr, 0x11580, 0 },
	{ { "*BScreen", "*BScreen" }, &backScreen_AE3FC_AE3EC, nullptr, 0x11170, 0 },
	{ { "data/mspr0-0.dat", "data/mspr1-0.dat" }, &spritesData_AE444_AE434, nullptr, 0, 0 },
	{ { "data/mspr0-0.tab", "data/mspr1-0.tab" }, &spritesTab_AE42C_AE41C, &spritesTabEnd_AE434_AE424, 0, 0 },
	{ { "", "" }, nullptr, nullptr, 0, 0 },
};

Type_Mc1FileEntry screenFilesHires_99898_99898[5] = {
	{ { "*WScreen", "*WScreen" }, &workScreen_12EFF4_12EFE4, nullptr, 0x4B000, 0 },
	{ { "*BScreen", "*BScreen" }, &backScreen_AE3FC_AE3EC, nullptr, 0x11170, 0 },
	{ { "data/hspr0-0.dat", "data/hspr1-0.dat" }, &spritesData_AE444_AE434, nullptr, 0, 0 },
	{ { "data/hspr0-0.tab", "data/hspr1-0.tab" }, &spritesTab_AE42C_AE41C, &spritesTabEnd_AE434_AE424, 0, 0 },
	{ { "", "" }, nullptr, nullptr, 0, 0 },
};

Type_Mc1FileEntry ftextFiles_995D8_995D8[2] = {
	{ { "data/ftext.dat", "data/ftext.dat" }, &textData_AE41C_AE40C, nullptr, 0, 0 },
	{ { "", "" }, nullptr, nullptr, 0, 0 },
};

Type_Mc1FileEntry itextFiles_99688_99688[2] = {
	{ { "data/itext.dat", "data/itext.dat" }, &textData_AE41C_AE40C, nullptr, 0, 0 },
	{ { "", "" }, nullptr, nullptr, 0, 0 },
};

Type_Mc1FileEntry blockFiles16_996E0_996E0[2] = {
	{ { "data/blk0-0.dat", "data/blk1-0.dat" }, &blockData_AE3F0_AE3E0, nullptr, 0, 0 },
	{ { "", "" }, nullptr, nullptr, 0, 0 },
};

Type_Mc1FileEntry blockFiles32_99738_99738[3] = {
	{ { "data/blk0-1.dat", "data/blk1-1.dat" }, &blockData_AE3F0_AE3E0, nullptr, 0, 0 },
	{ { "data/sky.dat", "data/sky1-0.dat" }, &skyData_AE3D8_AE3C8, nullptr, 0, 0 },
	{ { "", "" }, nullptr, nullptr, 0, 0 },
};

static const char* EntryPath(const Type_Mc1FileEntry* entry)
{
	return entry->path[mc1Variant == MC1_VARIANT_HW ? 1 : 0];
}

std::string Mc1DataFilePath(const char* path)
{
	// the original runs in the CD root, paths mix '/' and '\'
	std::string full = mc1DataPath + "/" + path;
	for (char& c : full)
		if (c == '\\')
			c = '/';
	return full;
}

int16_t LoadFileGroup_40440_40780(Type_Mc1FileEntry* group)
{
	int16_t errors = 0;
	for (Type_Mc1FileEntry* entry = group; entry->data_0x1C; entry++)
		FreeFileEntry_634A0_639B0(entry);
	for (Type_Mc1FileEntry* entry = group; entry->data_0x1C; entry++)
	{
		int32_t result = LoadFileEntry_634E0_639F0(entry);
		if (result > 0)
			continue;
		// the original installs the palette at AE428 for the text, prints the error and waits for return
		if (result < 0)
			printf("ERROR: Allocation %s.\n", EntryPath(entry));
		else
			printf("ERROR: File %s.\n", EntryPath(entry));
		Logger->error("MC1: loading {} failed", EntryPath(entry));
		errors++;
	}
	return errors;
}

void FreeFileEntry_634A0_639B0(Type_Mc1FileEntry* entry)
{
	if (*entry->data_0x1C)
	{
		Free_426E0_42A20(*entry->data_0x1C);
		*entry->data_0x1C = nullptr;
	}
}

int32_t LoadFileEntry_634E0_639F0(Type_Mc1FileEntry* entry)
{
	void* (*allocate)(uint32_t) = (entry->flags_0x28 & 1) ? LowMalloc_425C0_42900 : Malloc_42540_42880;
	FreeFileEntry_634A0_639B0(entry);
	const char* path = EntryPath(entry);
	if (path[0] == '*')
	{
		*entry->data_0x1C = (uint8_t*)allocate(entry->size_0x24);
		if (!*entry->data_0x1C)
			return 0;
	}
	else
	{
		entry->size_0x24 = GetFileUnpackSize_63910_63E20(entry);
		if ((int32_t)entry->size_0x24 <= 0)
			return 0;
		*entry->data_0x1C = (uint8_t*)allocate(entry->size_0x24);
		if (!*entry->data_0x1C)
			return -1;
		if (ReadPackedFile_3EEA0_3F1E0(path, *entry->data_0x1C) != (int32_t)entry->size_0x24)
		{
			//!!!!! the original keeps the block allocated and writes through dataEnd even when it is null
			Free_426E0_42A20(*entry->data_0x1C);
			*entry->data_0x1C = nullptr;
			if (entry->dataEnd_0x20)
				*entry->dataEnd_0x20 = nullptr;
			entry->size_0x24 = 0;
			return 0;
		}
	}
	if (entry->dataEnd_0x20)
		*entry->dataEnd_0x20 = *entry->data_0x1C + entry->size_0x24;
	return 1;
}

int32_t GetFileUnpackSize_63910_63E20(Type_Mc1FileEntry* entry)
{
	// same code as the MC2 one
	return DataFileIO::GetFileUnpackSize_AB9E1(Mc1DataFilePath(EntryPath(entry)).c_str());
}

int32_t ReadPackedFile_3EEA0_3F1E0(const char* path, uint8_t* buffer)
{
	FILE* file = DataFileIO::CreateOrOpenFile(Mc1DataFilePath(path).c_str(), 0x200);
	if (!file)
		return -1;
	int32_t length = (int32_t)DataFileIO::FileLengthBytes(file);
	DataFileIO::Read(file, buffer, length);
	DataFileIO::Close(file);
	int32_t result = DataFileRNC::Decompress(buffer, buffer);
	if (result < 0)
	{
		printf("ERROR decompressing %s\n", path);
		return -2;
	}
	return result ? result : length;
}

void RelocateTabs_59500_59A10(Type_Mc1TabRelocation* list)
{
	// the original adds the data address to every offset in the tab (sizes doubled in MCGA, 65D70 / 65DC0),
	// the port keeps the tab and builds a separate index with real pointers like remc2
	for (; list->tab_0; list++)
	{
		bitmap_pos_struct2_t* tab = (bitmap_pos_struct2_t*)*list->tab_0;
		bitmap_pos_struct2_t* tabEnd = (bitmap_pos_struct2_t*)*list->tabEnd_4;
		if (*list->index)
		{
			Free_426E0_42A20(*list->index);
			*list->index = nullptr;
		}
		if (!tab || !tabEnd)
			continue;
		*list->index = (bitmap_pos_struct_t*)Malloc_42540_42880((uint32_t)((tabEnd - tab) * sizeof(bitmap_pos_struct_t)));
		if (videoMode_12F02E_12F01E & 1)
			CreateIndexDatTabDoubled_98709(tab, tabEnd, *list->data_8, *list->index);
		else
			CreateIndexDatTab_9874D(tab, tabEnd, *list->data_8, *list->index);
	}
}
