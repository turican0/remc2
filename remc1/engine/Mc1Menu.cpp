#include "Mc1Menu.h"

#include <cstdio>
#include <cstring>
#include <string>

#include "Mc1Game.h"
#include "Mc1Install.h"
#include "Mc1NotPorted.h"
#include "Mc1Shared.h"
#include "Mc1System.h"
#include "../../remc2/utilities/DataFileIO.h"

char soundDriver_12C9A0_12C990[32];
char soundCardText_12C9C0_12C9B0[32];
char musicCardText_12C9E0_12C9D0[32];
char musicDriver_12CA00_12C9F0[32];
char saveSetupText_12CA20_12CA10[64];
char soundPortText_12CA80_12CA70[10];
char soundIrqText_12CA8A_12CA7A[10];
char musicPortText_12CA94_12CA84[10];
char soundPort_12CA9E_12CA8E[10];
char musicPort_12CAA8_12CA98[10];
char soundIrq_12CAB2_12CAA2[10];
char saveSetup_12CABC_12CAAC[10];
char soundDma_12CAC6_12CAB6[10];
char soundDmaText_12CAD0_12CAC0[12];

uint8_t* menuSpritesTab_12CAEC_12CADC = nullptr;
uint8_t* menuSpritesTabEnd_12CAF0_12CAE0 = nullptr;
uint8_t* menuSpritesData_12CAF4_12CAE4 = nullptr;
uint8_t* menuPalette_12CB9C_12CB8C = nullptr;
uint8_t* screenBackup_12CBA4_12CB94 = nullptr;
uint8_t* screenBackup2_12CBA8_12CB98 = nullptr;
bitmap_pos_struct_t* menuSpritesIndex = nullptr;
uint32_t loadMenuFiles_12CA7C_12CA6C = 0;

Type_Mc1FileEntry menuFiles_49B80_49EC0[6] = {
	{ { "data/screens/sptrs.dat", "data/screens/sptrs.dat" }, &menuSpritesData_12CAF4_12CAE4, nullptr, 0, 0 },
	{ { "data/screens/sptrs.tab", "data/screens/sptrs.tab" }, &menuSpritesTab_12CAEC_12CADC, &menuSpritesTabEnd_12CAF0_12CAE0, 0, 0 },
	{ { "*PALETTE", "*PALETTE" }, &menuPalette_12CB9C_12CB8C, nullptr, 0x300, 0 },
	{ { "*SCREEN BACKUP", "*SCREEN BACKUP" }, &screenBackup_12CBA4_12CB94, nullptr, 0xFA00, 0 },
	{ { "*SCREEN BACKUP", "*SCREEN BACKUP" }, &screenBackup2_12CBA8_12CB98, nullptr, 0xFA00, 0 },
	{ { "", "" }, nullptr, nullptr, 0, 0 },
};

uint32_t dword_12CBB0_12CBA0 = 0;
int16_t word_12CBBC_12CBAC = 0;
int16_t word_12CBBE_12CBAE = 0;
int16_t word_12CBC0_12CBB0 = 0;
int16_t word_12CBC2_12CBB2 = 0;
uint8_t byte_12CBC6_12CBB6 = 0;
uint8_t menuPage_12CBC9_12CBB9 = 0;
uint8_t byte_12CBCA_12CBBA = 0;
uint8_t byte_12CBCC_12CBBC = 0;
uint8_t byte_12CBCD_12CBBD = 0;
uint8_t mainMenuItem_12CBCE_12CBBE = 0;
uint8_t byte_12CBCF_12CBBF = 0;
uint8_t byte_12CBD0_12CBC0 = 0;
uint8_t byte_12CBD1_12CBC1 = 0;
uint8_t byte_12CBD2_12CBC2 = 0;
uint8_t byte_12CBD3_12CBC3 = 0;
uint8_t byte_12CBD4_12CBC4 = 0;
uint8_t menuFlags_12CBD5_12CBC5 = 0;
uint8_t byte_12CBD6_12CBC6 = 0;

static const char zeroText_A9990_A99A8[2] = { '0', 0 }; // copied as a word
static const char yesText_A99A0_A99B8[4] = "YES"; // copied as a dword

void InitMenu_4A980_4ACC0()
{
	if (LoadSoundSetup_505A0_508E0())
		menuPage_12CBC9_12CBB9 = 7;
	else
		menuPage_12CBC9_12CBB9 = 0;
	byte_12CBD4_12CBC4 = 0;
	word_12CBC0_12CBB0 = 0;
	word_12CBC2_12CBB2 = 0;
	word_12CBBC_12CBAC = 0;
	word_12CBBE_12CBAE = 0;
	byte_12CBCA_12CBBA = 0;
	byte_12CBC6_12CBB6 = 0;
	byte_12CBCD_12CBBD = 0;
	byte_12CBD0_12CBC0 = 0;
	byte_12CBCF_12CBBF = 2;
	byte_12CBD1_12CBC1 = 2;
	byte_12CBCC_12CBBC = 1;
	dword_12CBB0_12CBA0 = 0;
	menuFlags_12CBD5_12CBC5 = (menuFlags_12CBD5_12CBC5 | 2) & 0x42;
	byte_12CBD2_12CBC2 = 6;
	byte_12CBD6_12CBC6 &= 0xF8;
	mainMenuItem_12CBCE_12CBBE = 6;
	byte_12CBD3_12CBC3 = 0;
	sprintf(mc1Settings_AE408_AE3F8->sessionName_0x75_117, "CARPET%d", 0);

	// C:\carpet.cd\intro.pld
	std::string path = Mc1InstallPath() + "/intro.pld";
	FILE* file = DataFileIO::CreateOrOpenFile(path.c_str(), 0x200); // port: 0x202, only tested for existence
	if (file)
		DataFileIO::Close(file);
	else
	{
		menuFlags_12CBD5_12CBC5 &= ~2;
		file = DataFileIO::CreateOrOpenFile(path.c_str(), 0x222);
		if (file)
		{
			uint32_t handle = 0; // 4AAC1 / 4AE01: call sub_62ED0 with its DOS file handle, never read
			WriteFile_62ED0_633E0(file, (uint8_t*)&handle, 4);
			DataFileIO::Close(file);
		}
	}
	mc1Settings_AE408_AE3F8->language_0x97_151 = 0;
}

int32_t LoadSoundSetup_505A0_508E0()
{
	int32_t result = 0;
	// C:\carpet.cd\sndsetup.inf
	std::string path = Mc1InstallPath() + "/sndsetup.inf";
	FILE* file = DataFileIO::CreateOrOpenFile(path.c_str(), 0x200); // port: 0x202, only read
	if (file)
	{
		DataFileIO::Close(file);
		path = Mc1InstallPath() + "/sndsetup.dat";
		file = DataFileIO::CreateOrOpenFile(path.c_str(), 0x200);
		if (file)
		{
			DataFileIO::Read(file, (uint8_t*)soundDriver_12C9A0_12C990, 32);
			DataFileIO::Read(file, (uint8_t*)soundCardText_12C9C0_12C9B0, 32);
			DataFileIO::Read(file, (uint8_t*)musicDriver_12CA00_12C9F0, 32);
			DataFileIO::Read(file, (uint8_t*)musicCardText_12C9E0_12C9D0, 32);
			DataFileIO::Read(file, (uint8_t*)soundPort_12CA9E_12CA8E, 10);
			DataFileIO::Read(file, (uint8_t*)soundPortText_12CA80_12CA70, 10);
			DataFileIO::Read(file, (uint8_t*)soundIrq_12CAB2_12CAA2, 10);
			DataFileIO::Read(file, (uint8_t*)soundIrqText_12CA8A_12CA7A, 10);
			DataFileIO::Read(file, (uint8_t*)soundDma_12CAC6_12CAB6, 10);
			DataFileIO::Read(file, (uint8_t*)soundDmaText_12CAD0_12CAC0, 10);
			DataFileIO::Read(file, (uint8_t*)musicPort_12CAA8_12CA98, 10);
			DataFileIO::Read(file, (uint8_t*)musicPortText_12CA94_12CA84, 10);
			result = 1;
			DataFileIO::Close(file);
		}
	}
	else
	{
		memcpy(soundCardText_12C9C0_12C9B0, zeroText_A9990_A99A8, 2);
		memcpy(musicCardText_12C9E0_12C9D0, zeroText_A9990_A99A8, 2);
		memcpy(soundPort_12CA9E_12CA8E, zeroText_A9990_A99A8, 2);
		memcpy(soundPortText_12CA80_12CA70, zeroText_A9990_A99A8, 2);
		memcpy(soundIrq_12CAB2_12CAA2, zeroText_A9990_A99A8, 2);
		memcpy(soundIrqText_12CA8A_12CA7A, zeroText_A9990_A99A8, 2);
		memcpy(soundDma_12CAC6_12CAB6, zeroText_A9990_A99A8, 2);
		strcpy(soundDriver_12C9A0_12C990, "NONE");
		memcpy(soundDmaText_12CAD0_12CAC0, zeroText_A9990_A99A8, 2);
		memcpy(musicPort_12CAA8_12CA98, zeroText_A9990_A99A8, 2);
		memcpy(musicPortText_12CA94_12CA84, zeroText_A9990_A99A8, 2);
		strcpy(musicDriver_12CA00_12C9F0, "NONE");
	}
	memcpy(saveSetup_12CABC_12CAAC, yesText_A99A0_A99B8, 4);
	memcpy(saveSetupText_12CA20_12CA10, zeroText_A9990_A99A8, 2);
	return result;
}

static void FreeMenuSpritesIndex()
{
	Free_426E0_42A20(menuSpritesIndex);
	menuSpritesIndex = nullptr;
}

void RunMenu_4AB20_4AE60()
{
	if (loadMenuFiles_12CA7C_12CA6C)
	{
		LoadFileGroup_40440_40780(menuFiles_49B80_49EC0);
		// port: a separate index instead of relocating the tab in place (see RelocateTabs_59500_59A10)
		bitmap_pos_struct2_t* tab = (bitmap_pos_struct2_t*)menuSpritesTab_12CAEC_12CADC;
		bitmap_pos_struct2_t* tabEnd = (bitmap_pos_struct2_t*)menuSpritesTabEnd_12CAF0_12CAE0;
		FreeMenuSpritesIndex();
		menuSpritesIndex = (bitmap_pos_struct_t*)Malloc_42540_42880((uint32_t)((tabEnd - tab) * sizeof(bitmap_pos_struct_t)));
		if (*videoMode_12F02E_12F01E & 1)
			CreateIndexDatTabDoubled_65D70_66280(tab, tabEnd, menuSpritesData_12CAF4_12CAE4, menuSpritesIndex);
		else
			CreateIndexDatTab_65DC0_662D0(tab, tabEnd, menuSpritesData_12CAF4_12CAE4, menuSpritesIndex);
		loadMenuFiles_12CA7C_12CA6C = 0;
	}
	sub_50770_50AB0();
	switch (mainMenuItem_12CBCE_12CBBE)
	{
	case 0:
		sub_4D3B0_4D6F0();
		break;
	case 1:
		sub_4AC70_4AFB0();
		break;
	case 2:
		sub_4BD60_4C0A0();
		break;
	case 4:
		sub_4D680_4D9C0();
		break;
	case 5:
		sub_4E5B0_4E8F0();
		break;
	case 6:
		sub_4F3F0_4F730();
		break;
	case 7:
		sub_4EFC0_4F300();
		break;
	case 8:
		sub_4F1E0_4F520();
		break;
	case 9:
		sub_4EE70_4F1B0();
		break;
	case 10:
		sub_4D560_4D8A0();
		break;
	}
	dword_12CBB0_12CBA0++;
	sub_65E00_66310();
	if (menuDone_96880_96880)
	{
		FreeFileGroup_61610_61B20(menuFiles_49B80_49EC0);
		FreeMenuSpritesIndex();
		if (*videoMode_12F02E_12F01E & 1)
			ClearScreen320_411D7_41517(backScreen_AE3FC_AE3EC, 320, 200, 0);
		else
			//!!!!! 480 rows of 640 do not fit the 0x11170 byte *BScreen; not reached, 34C80 / 35040 switch the menu to MCGA
			ClearScreen640_411FD_4153D(backScreen_AE3FC_AE3EC, 640, 480, 0);
		loadMenuFiles_12CA7C_12CA6C = 1;
		sub_20E60_20E60();
	}
}
