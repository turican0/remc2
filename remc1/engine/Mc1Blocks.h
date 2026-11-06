#pragma once

#include <cstdint>

// MC1 landscape blocks (blk?-0 16x16 / blk?-1 32x32 tiles), the level heap and the color tables (tables.dat)

#pragma pack(push, 1)
typedef struct { // DOS B7934 / B7924, the content of data/tables.dat (MC1) and data/dtables.dat (HW)
	uint8_t fades_0x0000[64][256]; // 0-31 to the last palette color, 32-63 to the first one
	uint8_t blend_0x4000[256][256]; // a third of the way from color [y] to color [x]
	uint8_t blockColors_0x14000[0x300]; // average color of each block, twice (+0x80)
	uint8_t distances_0x14300[0x300]; // sqrt(0x10000 - i * i), [0] = 0xFF
} Type_Mc1Tables_B7934;
#pragma pack(pop)

static_assert(sizeof(Type_Mc1Tables_B7934) == 0x14600, "Type_Mc1Tables_B7934");

extern uint8_t* blockPointers_9334C_9334C[256];
extern Type_Mc1Tables_B7934 tables_B7934_B7924;
extern uint8_t tablesMissing_90B48_90B48;
extern uint32_t largestFreeBlock_1314AC_13149C;
extern uint8_t byte_968F0_968F0;
extern uint8_t byte_939EC_939EC;

void InitMemory_41230_41570();
void InitBlockTables_44470_447B0();
void MakeFadeTable_40EDC_4121C(const uint8_t* palette, uint8_t* table, uint8_t targetRed, uint8_t targetGreen,
	uint8_t targetBlue, int16_t factorRed, int16_t factorGreen, int16_t factorBlue);
void MakeBlockColors_72807_72D17(const uint8_t* palette, uint8_t* colors);
