#include "Mc1Level.h"

#include <cstdio>
#include <cstring>
#include <string>

#include "Mc1Files.h"
#include "Mc1Game.h"
#include "Mc1Install.h"
#include "Mc1Menu.h"
#include "Mc1NotPorted.h"
#include "Mc1Shared.h"
#include "Mc1Sound.h"
#include "../../remc2/utilities/DataFileIO.h"

uint8_t word_AE454_AE444[6];

// 9075C: the heights (0-6) of the corners of a tile, x/y, x+1/y, x+1/y+1, x/y+1; -1 not used
const int8_t anglePatterns_9075C_9075C[148][4] = {
	{ 0, 0, 0, 0 }, { 1, 1, 1, 1 }, { 2, 2, 2, 2 }, { 3, 3, 3, 3 }, { 4, 4, 4, 4 }, { 5, 5, 5, 5 },
	{ 6, 6, 6, 6 }, { -1, -1, -1, -1 }, { -1, -1, -1, -1 }, { -1, -1, -1, -1 }, { -1, -1, -1, -1 }, { -1, -1, -1, -1 },
	{ -1, -1, -1, -1 }, { -1, -1, -1, -1 }, { -1, -1, -1, -1 }, { -1, -1, -1, -1 }, { -1, -1, -1, -1 }, { -1, -1, -1, -1 },
	{ -1, -1, -1, -1 }, { -1, -1, -1, -1 }, { -1, -1, -1, -1 }, { -1, -1, -1, -1 }, { -1, -1, -1, -1 }, { -1, -1, -1, -1 },
	{ -1, -1, -1, -1 }, { -1, -1, -1, -1 }, { -1, -1, -1, -1 }, { -1, -1, -1, -1 }, { -1, -1, -1, -1 }, { -1, -1, -1, -1 },
	{ -1, -1, -1, -1 }, { -1, -1, -1, -1 }, { -1, -1, -1, -1 }, { -1, -1, -1, -1 }, { -1, -1, -1, -1 }, { 6, 0, 1, 4 },
	{ 1, 1, 0, 0 }, { 1, 0, 0, 0 }, { 1, 0, 1, 0 }, { 0, 1, 1, 1 }, { 6, 6, 4, 4 }, { 6, 4, 6, 4 },
	{ 6, 4, 6, 6 }, { 4, 6, 4, 4 }, { 4, 4, 0, 0 }, { 4, 0, 0, 0 }, { 0, 4, 4, 4 }, { 0, 4, 0, 4 },
	{ 1, 3, 3, 3 }, { 1, 3, 1, 3 }, { 3, 1, 1, 1 }, { 1, 1, 3, 3 }, { 5, 1, 1, 1 }, { 1, 1, 5, 5 },
	{ 1, 5, 1, 5 }, { 1, 5, 5, 5 }, { 2, 5, 2, 5 }, { 5, 2, 2, 2 }, { 2, 5, 5, 5 }, { 5, 5, 2, 2 },
	{ 4, 4, 3, 3 }, { 4, 3, 3, 3 }, { 3, 4, 3, 4 }, { 3, 4, 4, 4 }, { 4, 5, 5, 5 }, { 5, 4, 4, 4 },
	{ 5, 4, 5, 4 }, { 4, 4, 5, 5 }, { 1, 2, 1, 2 }, { 2, 1, 1, 1 }, { 1, 2, 2, 2 }, { 1, 1, 2, 2 },
	{ 4, 1, 1, 1 }, { 1, 4, 1, 4 }, { 1, 4, 4, 4 }, { 1, 1, 4, 4 }, { 1, 6, 1, 1 }, { 6, 6, 1, 1 },
	{ 6, 1, 6, 1 }, { 6, 1, 6, 6 }, { 6, 6, 0, 0 }, { 6, 0, 6, 0 }, { 6, 0, 6, 6 }, { 0, 6, 0, 0 },
	{ 2, 1, 5, 1 }, { 1, 1, 5, 2 }, { 5, 1, 5, 2 }, { 2, 1, 2, 5 }, { 2, 2, 1, 5 }, { 5, 5, 1, 2 },
	{ 3, 3, 4, 1 }, { 4, 3, 4, 1 }, { 1, 1, 4, 3 }, { 1, 4, 4, 3 }, { 3, 4, 3, 1 }, { 1, 3, 1, 4 },
	{ 1, 6, 4, 6 }, { 1, 6, 1, 4 }, { 1, 6, 6, 4 }, { 1, 4, 6, 4 }, { 1, 6, 4, 1 }, { 1, 6, 4, 4 },
	{ 6, 4, 0, 4 }, { 0, 4, 6, 6 }, { 0, 4, 0, 6 }, { 0, 0, 4, 6 }, { 0, 6, 4, 4 }, { 6, 0, 6, 4 },
	{ 6, 0, 6, 1 }, { 1, 0, 6, 0 }, { 1, 6, 0, 0 }, { 1, 6, 6, 0 }, { 1, 6, 1, 0 }, { 1, 1, 0, 6 },
	{ 1, 0, 4, 0 }, { 1, 4, 0, 4 }, { 1, 4, 0, 0 }, { 1, 1, 4, 0 }, { 4, 1, 0, 4 }, { 1, 4, 1, 0 },
	{ 1, 5, 5, 4 }, { 4, 5, 4, 1 }, { 1, 1, 4, 5 }, { 1, 5, 4, 5 }, { 1, 4, 1, 5 }, { 1, 4, 4, 5 },
	{ 1, 6, 0, 4 }, { 6, 1, 0, 4 }, { 6, 6, 5, 5 }, { 6, 5, 6, 5 }, { 6, 5, 6, 6 }, { 5, 6, 5, 5 },
	{ 6, 6, 3, 3 }, { 6, 3, 6, 3 }, { 6, 3, 6, 6 }, { 3, 6, 3, 3 }, { 1, 5, 5, 6 }, { 6, 5, 6, 1 },
	{ 1, 1, 6, 5 }, { 1, 5, 6, 5 }, { 1, 6, 1, 5 }, { 1, 6, 6, 5 }, { 1, 3, 3, 6 }, { 6, 3, 6, 1 },
	{ 1, 1, 6, 3 }, { 1, 3, 6, 3 }, { 1, 6, 1, 3 }, { 1, 6, 6, 3 },
};

uint8_t angleTypes_B5D40_B5D30[2401][2];

static uint16_t Tile(int x, int y)
{
	return (uint16_t)((x & 0xFF) | ((y & 0xFF) << 8));
}

void LoadingScreen_34460_34820()
{
	ReadPackedFile_3EEA0_3F1E0("data/smatitle.dat", *workScreen_12EFF4_12EFE4);
	BlitScreen();
	ReadPackedFile_3EEA0_3F1E0("data/smatitle.pal", palette_AE428_AE418);
	FadeInOut_61CC0_621D0((TColor*)palette_AE428_AE418, 0x20, false, 10);
	mc1State_AE400_AE3F0->dword_0x245_581 = 1;
	mc1State_AE400_AE3F0->dword_0x249_585 = 0;
	mc1State_AE400_AE3F0->dword_0x24D_589 = 0;
}

void LoadLevel_407A0_40AE0()
{
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	// no sound while the level is created
	uint8_t sound = byte_939E5_939E5;
	byte_939E5_939E5 = 0;
	ClearLevel_40550_40890();
	PrintLoadingText_5AD10_5B220("Load Level");
	LoadLevelData_3E100_3E440((int16_t)mc1Settings_AE408_AE3F8->levelIndex_0x11_17, &state->level_0x2F503_193795);
	if (!(mc1Settings_AE408_AE3F8->flags_0x00_0 & 0x10))
		state->playerCount_0x0A_10 = state->level_0x2F503_193795.playerCount_0x9792_38802;
	PrintLoadingText_5AD10_5B220("Generate map");
	GenerateLevelMap_31AA0_31AE0(&state->level_0x2F503_193795);
	sub_371C0_37580();
	PrintLoadingText_5AD10_5B220("Generate features");
	sub_36430_367F0(&state->level_0x2F503_193795);
	PrintLoadingText_5AD10_5B220("Initialise Models");
	memset(word_AE454_AE444, 0, sizeof(word_AE454_AE444));
	sub_37220_375E0();
	state->dword_0x11F1_4593 = -1;
	sub_37440_37800(0, 1);
	byte_939E5_939E5 = sound;
	sub_3DD50_3E090();
	Type_Mc1Player_340B* player = &state->players_0x340B_13323[state->currentPlayer_0x08_8];
	sub_48230_48570(&state->entities_0x7463_29795[player->entityIndex_0x0A_10]);
}

void ClearLevel_40550_40890()
{
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	Type_Mc1Settings_AE408_AE3F8* settings = mc1Settings_AE408_AE3F8;
	memset(state->modelCounters_0x0C_12, 0, sizeof(state->modelCounters_0x0C_12));
	state->dword_0x20_32 = 0;
	state->word_0x24_36 = 0;
	state->word_0x26_38 = 0;
	state->dword_0x28_40 = 0;
	state->byte_0x244_580 = 0;
	memset(state->creatureStats_0x38C9F_232607, 0, sizeof(state->creatureStats_0x38C9F_232607));
	memset(state->playerStarts_0x23D9_9177, 0, sizeof(state->playerStarts_0x23D9_9177));
	memset(state->entities_0x7463_29795, 0, sizeof(state->entities_0x7463_29795));
	settings->flag_0x17_23 = 0;
	memset(settings->colorIndex_0x5D_93, 0, sizeof(settings->colorIndex_0x5D_93));
	settings->speedIndex_0x96_150 = 0;
	settings->paletteSubMod_0x98_152 = 0;
	memset(settings->manaStats_0xB8_184, 0, sizeof(settings->manaStats_0xB8_184));
	memset(settings->entityLists_0x8E1A_36378, 0, sizeof(settings->entityLists_0x8E1A_36378));
	memset(mapEntityIndex_10C1E0_10C1D0, 0, 0x20000);
	memset(mapTerrainType_CC1E0_CC1D0, 0, 0x10000);
	memset(mapHeightmap_DC1E0_DC1D0, 0, 0x10000);
	memset(mapShading_EC1E0_EC1D0, 0, 0x10000);
	memset(mapAngle_FC1E0_FC1D0, 0, 0x10000);
	memset(backScreen_AE3FC_AE3EC, 0, 0xFA00);
	settings->flags_0x00_0 &= 0xFFFE3FFF;
}

uint8_t LoadLevelData_3E100_3E440(int16_t levelIndex, Type_Mc1Level_2F503* level)
{
	if (levelIndex >= 1000)
		return 1;
	// C:\carpet.cd\levels\levels.dat, else from the CD; ddlevels in Hidden Worlds
	const char* name = mc1Variant == MC1_VARIANT_HW ? "ddlevels" : "levels";
	std::string path = Mc1InstallPath() + "/levels/" + name + ".dat";
	FILE* data = DataFileIO::CreateOrOpenFile(path.c_str(), 0x200);
	if (!data)
		data = DataFileIO::CreateOrOpenFile(Mc1DataFilePath((std::string("levels/") + name + ".dat").c_str()).c_str(), 0x200);
	if (!data)
		return 0;
	path = Mc1InstallPath() + "/levels/" + name + ".tab";
	FILE* tab = DataFileIO::CreateOrOpenFile(path.c_str(), 0x200);
	if (!tab)
		tab = DataFileIO::CreateOrOpenFile(Mc1DataFilePath((std::string("levels/") + name + ".tab").c_str()).c_str(), 0x200);
	if (!tab)
	{
		DataFileIO::Close(data);
		return 0;
	}
	// the tab and the packed level go through *BScreen
	uint8_t* buffer = backScreen_AE3FC_AE3EC;
	DataFileIO::Read(tab, buffer, 0xFA0);
	uint32_t start = ((uint32_t*)buffer)[levelIndex];
	uint32_t size = ((uint32_t*)buffer)[levelIndex + 1] - start;
	DataFileIO::Close(tab);
	// 3E1F8: the address of filelength is tested, never 0
	DataFileIO::Seek(data, start, 0);
	DataFileIO::Read(data, buffer, size);
	if (DecompressRnc_62B60_63070(buffer, buffer) < 0)
	{
		printf("ERROR decompressing levels.dat\n");
		DataFileIO::Close(data); // port: the original leaves the file open
		return 0;
	}
	memcpy(level, buffer, sizeof(Type_Mc1Level_2F503));
	memset(buffer, 0, sizeof(Type_Mc1Level_2F503));
	DataFileIO::Close(data);
	return 1;
}

void GenerateLevelMap_31AA0_31AE0(Type_Mc1Level_2F503* level)
{
	*randSeed_12C1E0_12C1D0 = (uint16_t)level->seed_0x04_4;
	mc1State_AE400_AE3F0->rand_0x04_4 = level->seed_0x04_4;
	// a map scanned by the editor replaces the generated heights
	if (ReadPackedFile_3EEA0_3F1E0("c:/carpet.cd/save/scanned.rmd", mapHeightmap_DC1E0_DC1D0) != 0x10000)
	{
		DecompressTerrainMapLevel_725C8_72AD8((int16_t)*randSeed_12C1E0_12C1D0, level->offset_0x08_8,
			(int16_t)level->raise_0x0C_12, level->gnarl_0x10_16);
		TruncTerrainHeight_32A50_32E40(mapEntityIndex_10C1E0_10C1D0, mapHeightmap_DC1E0_DC1D0);
	}
	memset(mapEntityIndex_10C1E0_10C1D0, 0, 0x20000);
	MarkLandAndRivers_32AE0_32ED0(level->river_0x14_20, (uint8_t)level->lriver_0x18_24);
	SetMax4Tiles_33500_338F0();
	ApplySnowLine_320A0_32490(level->snLin_0x20_32);
	MarkFlatLowland_32D00_330F0((uint8_t)level->snFlt_0x24_36, (uint8_t)level->bhLin_0x28_40);
	SpreadTerrainClass4_32300_326F0();
	MarkFlatLowlandClass2_32EB0_332A0((uint8_t)level->snFlt_0x24_36, (uint8_t)level->bhLin_0x28_40);
	MarkSteepTerrain_33180_33570((uint8_t)level->bhFlt_0x2C_44);
	if (mc1Variant == MC1_VARIANT_HW)
		MarkFlatHighland_31C10((uint8_t)level->source_0x1C_28, (uint8_t)level->snLin_0x20_32);
	RemoveIsolatedTerrainClasses_31FA0_32390();
	memset(mapTerrainType_CC1E0_CC1D0, 0, 0x10000);
	SmoothTerrain_31BB0_31FA0();
	AddRivers_31EC0_322B0();
	SetTerrainAngles_32560_32950();
	MarkWaterEdges_31D40_32130();
	ComputeTerrainShading_329C0_32DB0();
}

void SmoothTerrain_31BB0_31FA0()
{
	static const int8_t around[8][2] = { { 0, -1 }, { 1, -1 }, { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 } };
	uint8_t* height = mapHeightmap_DC1E0_DC1D0;
	for (int i = 0; i < 0x10000; i++)
	{
		if (!(mapAngle_FC1E0_FC1D0[i] & 7))
			continue;
		int x = i & 0xFF;
		int y = i >> 8;
		uint8_t center = height[i];
		uint8_t highest = center;
		uint8_t lowest = center;
		uint32_t sum = 0;
		for (const int8_t* d : around)
		{
			uint8_t h = height[Tile(x + d[0], y + d[1])];
			sum += h;
			if (h > highest)
				highest = h;
			if (h < lowest)
				lowest = h;
		}
		// the center is not in the average of the 8 neighbours
		uint8_t average = (uint8_t)(sum >> 3);
		uint8_t below = center - lowest;
		uint8_t above = highest - center;
		uint8_t step = below > 4 ? below : above;
		if (step > 4)
			height[i] = step > 10 ? average : (uint8_t)((center + average) >> 1);
	}
}

// 33180 / 31C10: marks the tiles by the height range of the 4 neighbours, then sets 1 to the marked tiles next to
// the classes 3, 2, 5 and 4
static void MarkTerrainByRange(bool highland, uint8_t minHeight, uint8_t range, uint8_t mark)
{
	static const int8_t cross[4][2] = { { 0, -1 }, { 1, 0 }, { 0, 1 }, { -1, 0 } };
	static const int8_t around[8][2] = { { 0, -1 }, { 1, -1 }, { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 } };
	uint8_t* height = mapHeightmap_DC1E0_DC1D0;
	uint8_t* angle = mapAngle_FC1E0_FC1D0;
	memcpy(mapTerrainType_CC1E0_CC1D0, angle, 0x10000);
	for (int i = 0; i < 0x10000; i++)
	{
		if (highland && height[i] <= minHeight)
			continue;
		int x = i & 0xFF;
		int y = i >> 8;
		uint8_t highest = height[i];
		uint8_t lowest = height[i];
		for (const int8_t* d : cross)
		{
			uint8_t h = height[Tile(x + d[0], y + d[1])];
			if (h > highest)
				highest = h;
			if (h < lowest)
				lowest = h;
		}
		if (!angle[i])
			continue;
		int32_t difference = highest - lowest;
		if (highland ? difference < range : difference >= range)
			angle[i] = mark;
	}
	for (int i = 0; i < 0x10000; i++)
	{
		if (angle[i] != mark)
			continue;
		int x = i & 0xFF;
		int y = i >> 8;
		uint8_t count3 = 0, count2 = 0, count5 = 0, count4 = 0;
		for (const int8_t* d : around)
		{
			uint8_t a = angle[Tile(x + d[0], y + d[1])];
			count3 += a == 3;
			count2 += a == 2;
			count5 += a == 5;
			count4 += a == 4;
		}
		if (count3 ? (count2 || count5 || count4) : (count2 || (count5 && count4)))
			angle[i] = 1;
	}
}

void MarkSteepTerrain_33180_33570(uint8_t minRange)
{
	// HW marks 1 instead of 6 (its second pass then changes nothing)
	MarkTerrainByRange(false, 0, minRange, mc1Variant == MC1_VARIANT_HW ? 1 : 6);
}

void MarkFlatHighland_31C10(uint8_t minHeight, uint8_t maxRange)
{
	MarkTerrainByRange(true, minHeight, maxRange, 6);
}

void MarkWaterEdges_31D40_32130()
{
	static const int8_t around[8][2] = { { 0, -1 }, { 1, -1 }, { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 } };
	uint8_t* height = mapHeightmap_DC1E0_DC1D0;
	uint8_t* type = mapTerrainType_CC1E0_CC1D0;
	for (int i = 0; i < 0x10000; i++)
	{
		mapAngle_FC1E0_FC1D0[i] &= 0xF7;
		if (height[i])
			continue;
		int x = i & 0xFF;
		int y = i >> 8;
		bool land = false;
		for (const int8_t* d : around)
			land |= height[Tile(x + d[0], y + d[1])] != 0;
		if (land)
			continue;
		if (type[i] || type[Tile(x - 1, y)] || type[Tile(x - 1, y - 1)] || type[Tile(x, y - 1)])
			continue;
		mapAngle_FC1E0_FC1D0[i] |= 8;
	}
}

static int AngleKey(int a, int b, int c, int d)
{
	return a * 343 + b * 49 + c * 7 + d;
}

// a record of 25 bytes in *WScreen: count, 12 terrain types, 12 angles
static void AddAngleType(uint8_t* records, int key, uint8_t type, uint8_t angle)
{
	uint8_t* record = records + key * 25;
	uint8_t count = record[0];
	if (count >= 12)
		return;
	record[0] = count + 1;
	record[1 + count] = type;
	record[0x0D + count] = angle;
}

void SetTerrainAngles_32560_32950()
{
	uint8_t* records = *workScreen_12EFF4_12EFE4;
	for (int i = 0; i < 0x961; i++)
		records[i * 25] = 0;
	// every pattern in its 8 rotations / mirrors
	for (uint8_t i = 0; i < 148; i++)
	{
		const int8_t* p = anglePatterns_9075C_9075C[i];
		if (p[0] < 0 || p[1] < 0 || p[2] < 0 || p[3] < 0)
			continue;
		int a = p[0], b = p[1], c = p[2], d = p[3];
		AddAngleType(records, AngleKey(a, b, c, d), i, 0x00);
		AddAngleType(records, AngleKey(b, a, d, c), i, 0x10);
		AddAngleType(records, AngleKey(c, d, a, b), i, 0x30);
		AddAngleType(records, AngleKey(d, c, b, a), i, 0x20);
		AddAngleType(records, AngleKey(b, c, d, a), i, 0x60);
		AddAngleType(records, AngleKey(c, b, a, d), i, 0x70);
		AddAngleType(records, AngleKey(d, a, b, c), i, 0x50);
		AddAngleType(records, AngleKey(a, d, c, b), i, 0x40);
	}
	for (int key = 0; key < 2401; key++)
	{
		const uint8_t* record = records + key * 25;
		angleTypes_B5D40_B5D30[key][0] = record[0] ? record[1] : 1;
		angleTypes_B5D40_B5D30[key][1] = record[0] ? record[0x0D] : 0;
	}
	uint8_t* type = mapTerrainType_CC1E0_CC1D0;
	uint8_t* angle = mapAngle_FC1E0_FC1D0;
	uint16_t* seed = randSeed_12C1E0_12C1D0;
	for (int i = 0; i < 0x10000; i++)
	{
		if (type[i])
			continue;
		int x = i & 0xFF;
		int y = i >> 8;
		int key = AngleKey(angle[i] & 7, angle[Tile(x + 1, y)] & 7, angle[Tile(x + 1, y + 1)] & 7, angle[Tile(x, y + 1)] & 7);
		const uint8_t* record = records + key * 25;
		uint8_t count = record[0];
		if (!count)
		{
			type[i] = 1;
			continue;
		}
		*seed = (uint16_t)(*seed * 0x24A1 + 0x24DF);
		uint8_t pick = (uint8_t)(*seed % (count + 1));
		if (pick >= count)
			pick = 0;
		type[i] = record[1 + pick];
		angle[i] = (angle[i] & 7) + record[0x0D + pick];
	}
}

void ComputeTerrainShading_329C0_32DB0()
{
	uint8_t* height = mapHeightmap_DC1E0_DC1D0;
	uint16_t* seed = randSeed_12C1E0_12C1D0;
	*seed = 0;
	for (int i = 0; i < 0x10000; i++)
	{
		int x = i & 0xFF;
		int y = i >> 8;
		uint8_t shade = (uint8_t)(height[Tile(x - 1, y - 1)] - height[Tile(x + 1, y + 1)] + 0x20);
		if (shade == 0x20)
		{
			*seed = (uint16_t)(*seed * 0x24A1 + 0x24DF);
			shade = (uint8_t)(*seed % 9 + 0x1C);
		}
		else if ((int8_t)shade < 0x1C)
			shade = (shade & 3) + 0x1C;
		else if ((int8_t)shade > 0x28)
			shade = (shade & 7) + 0x28;
		mapShading_EC1E0_EC1D0[i] = shade;
	}
}
