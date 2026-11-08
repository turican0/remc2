#pragma once

#include <cstdint>
#include <string>

#include "../../remc2/portability/bitmap_pos_struct.h"
#include "Mc1Shared.h"

// MC1 data file tables and loading; DOS record: path[28], data, dataEnd, size, flags (44 bytes)

typedef struct {
	const char* path[2]; // MC1, HW
	uint8_t** data_0x1C;
	uint8_t** dataEnd_0x20;
	uint32_t size_0x24; // allocation size for '*' entries, file size after loading
	uint32_t flags_0x28; // 1 low memory
} Type_Mc1FileEntry;

extern std::string mc1DataPath;

extern uint8_t* searchData_AE3F4_AE3E4;
extern uint8_t* searchDataEnd_AE414_AE404;
extern uint8_t* buildData_AE430_AE420;
extern uint8_t* buildDataEnd_AE438_AE428;
extern uint8_t* buildTab_AE440_AE430;
extern uint8_t* buildTabEnd_AE40C_AE3FC;
extern uint8_t* font0Data_AE378_AE368;
extern uint8_t* font1Data_AE37C_AE36C;
extern uint8_t* fontTabs_AE3B8_AE3A8[8]; // font0.tab, font1.tab
extern uint8_t* font0TabEnd_AE398_AE388;
extern uint8_t* font1TabEnd_AE39C_AE38C;
extern uint8_t* tmapsTab_12D744_12D734;
extern uint8_t* palData_AE424_AE414;
extern uint8_t* palMem_AE418_AE408;
extern uint8_t* pointersData_AE448_AE438;
extern uint8_t* pointersTab_AE450_AE440;
extern uint8_t* pointersTabEnd_AE43C_AE42C;
extern uint8_t* palette_AE428_AE418;
// *WScreen is workScreen_12EFF4_12EFE4 (Mc1Shared.h)
extern uint8_t* backScreen_AE3FC_AE3EC;
extern uint8_t* spritesData_AE444_AE434;
extern uint8_t* spritesTab_AE42C_AE41C;
extern uint8_t* spritesTabEnd_AE434_AE424;
extern uint8_t* textData_AE41C_AE40C;
extern uint8_t* blockData_AE3F0_AE3E0; // 256 tiles in a 256 pixel wide image
extern uint8_t* skyData_AE3D8_AE3C8;

// tab, tabEnd and data of a sprite table; the port builds the index instead of relocating the tab in place
typedef struct {
	uint8_t** tab_0;
	uint8_t** tabEnd_4;
	uint8_t** data_8;
	bitmap_pos_struct_t** index;
} Type_Mc1TabRelocation;

extern bitmap_pos_struct_t* pointersIndex;
extern bitmap_pos_struct_t* fontIndexes[8];
extern bitmap_pos_struct_t* spritesIndex;
extern bitmap_pos_struct_t* buildIndex;
extern Type_Mc1TabRelocation tabRelocations_99974_99974[6];

extern Type_Mc1FileEntry searchDFiles_99318_99318[14];
extern Type_Mc1FileEntry screenFilesMcga_997BC_997BC[5];
extern Type_Mc1FileEntry screenFilesHires_99898_99898[5];
extern Type_Mc1FileEntry etextFiles_99580_99580[2];
extern Type_Mc1FileEntry ftextFiles_995D8_995D8[2];
extern Type_Mc1FileEntry gtextFiles_99630_99630[2];
extern Type_Mc1FileEntry itextFiles_99688_99688[2];
extern Type_Mc1FileEntry blockFiles16_996E0_996E0[2];
extern Type_Mc1FileEntry blockFiles32_99738_99738[3];

std::string Mc1DataFilePath(const char* path);
int16_t LoadFileGroup_40440_40780(Type_Mc1FileEntry* group);
void FreeFileGroup_61610_61B20(Type_Mc1FileEntry* group);
void FreeFileEntry_634A0_639B0(Type_Mc1FileEntry* entry);
int32_t LoadFileEntry_634E0_639F0(Type_Mc1FileEntry* entry);
int32_t GetFileUnpackSize_63910_63E20(Type_Mc1FileEntry* entry);
int32_t ReadPackedFile_3EEA0_3F1E0(const char* path, uint8_t* buffer);
void RelocateTabs_59500_59A10(Type_Mc1TabRelocation* list);
