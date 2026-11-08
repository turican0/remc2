#include "Mc1Menu.h"

#include <cstdio>
#include <cstring>
#include <string>

#include "Mc1Game.h"
#include "Mc1Install.h"
#include "Mc1MenuSound.h"
#include "Mc1Movie.h"
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

uint8_t* languageSpritesTab_12CB58_12CB48 = nullptr;
uint8_t* languageSpritesTabEnd_12CB5C_12CB4C = nullptr;
uint8_t* languageSpritesData_12CB60_12CB50 = nullptr;
bitmap_pos_struct_t* languageSpritesIndex = nullptr;

Type_Mc1FileEntry languageFiles_4A050_4A390[3] = {
	{ { "data/screens/langspr.dat", "data/screens/langspr.dat" }, &languageSpritesData_12CB60_12CB50, nullptr, 0, 0 },
	{ { "data/screens/langspr.tab", "data/screens/langspr.tab" }, &languageSpritesTab_12CB58_12CB48, &languageSpritesTabEnd_12CB5C_12CB4C, 0, 0 },
	{ { "", "" }, nullptr, nullptr, 0, 0 },
};

// 4A8F4 / 4AC34: 10, then x, y of the flag of each language (in 640x400 mouse coordinates / 2)
static const int16_t languageFlags_4A8F4_4AC34[9] = { 10, 61, 45, 61, 95, 179, 45, 179, 95 };

Type_Mc1FileEntry menuFiles_49B80_49EC0[6] = {
	{ { "data/screens/sptrs.dat", "data/screens/sptrs.dat" }, &menuSpritesData_12CAF4_12CAE4, nullptr, 0, 0 },
	{ { "data/screens/sptrs.tab", "data/screens/sptrs.tab" }, &menuSpritesTab_12CAEC_12CADC, &menuSpritesTabEnd_12CAF0_12CAE0, 0, 0 },
	{ { "*PALETTE", "*PALETTE" }, &menuPalette_12CB9C_12CB8C, nullptr, 0x300, 0 },
	{ { "*SCREEN BACKUP", "*SCREEN BACKUP" }, &screenBackup_12CBA4_12CB94, nullptr, 0xFA00, 0 },
	{ { "*SCREEN BACKUP", "*SCREEN BACKUP" }, &screenBackup2_12CBA8_12CB98, nullptr, 0xFA00, 0 },
	{ { "", "" }, nullptr, nullptr, 0, 0 },
};

int16_t mouseX_12CADC_12CACC = 0;
int16_t mouseY_12CADE_12CACE = 0;
int16_t clickX_12CAE0_12CAD0 = 0;
int16_t clickY_12CAE2_12CAD2 = 0;
uint8_t inputFlags_12CAE4_12CAD4[5];

uint32_t dword_12CBB0_12CBA0 = 0;
int16_t word_12CBBC_12CBAC = 0;
int16_t word_12CBBE_12CBAE = 0;
int16_t word_12CBC0_12CBB0 = 0;
int16_t word_12CBC2_12CBB2 = 0;
uint8_t byte_12CBC6_12CBB6 = 0;
uint8_t menuPage_12CBC9_12CBB9 = 0;
uint8_t byte_12CBCA_12CBBA = 0;
uint8_t hoveredOption_12CBCB_12CBBB = 0;
uint8_t byte_12CBCC_12CBBC = 0;
uint8_t byte_12CBCD_12CBBD = 0;
uint8_t mainMenuItem_12CBCE_12CBBE = 0;
uint8_t inputMode_12CBCF_12CBBF = 0;
uint8_t byte_12CBD0_12CBC0 = 0;
uint8_t byte_12CBD1_12CBC1 = 0;
uint8_t byte_12CBD2_12CBC2 = 0;
uint8_t controlDevice_12CBD3_12CBC3 = 0;
uint8_t byte_12CBD4_12CBC4 = 0;
uint8_t menuFlags_12CBD5_12CBC5 = 0;
uint8_t byte_12CBD6_12CBC6 = 0;

static const char zeroText_A9990_A99A8[2] = { '0', 0 }; // copied as a word
const char yesText_A99A0_A99B8[4] = "YES"; // copied as a dword

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
	inputMode_12CBCF_12CBBF = 2;
	byte_12CBD1_12CBC1 = 2;
	byte_12CBCC_12CBBC = 1;
	dword_12CBB0_12CBA0 = 0;
	menuFlags_12CBD5_12CBC5 = (menuFlags_12CBD5_12CBC5 | 2) & 0x42;
	byte_12CBD2_12CBC2 = 6;
	byte_12CBD6_12CBC6 &= 0xF8;
	mainMenuItem_12CBCE_12CBBE = 6;
	controlDevice_12CBD3_12CBC3 = 0;
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

// port: a separate index with real pointers instead of relocating the tab in place (65D70 / 65DC0)
void BuildSpritesIndex(bitmap_pos_struct_t** index, uint8_t* tabStart, uint8_t* tabEnd, uint8_t* data, bool doubleInMcga)
{
	bitmap_pos_struct2_t* tab = (bitmap_pos_struct2_t*)tabStart;
	bitmap_pos_struct2_t* end = (bitmap_pos_struct2_t*)tabEnd;
	Free_426E0_42A20(*index);
	*index = (bitmap_pos_struct_t*)Malloc_42540_42880((uint32_t)((end - tab) * sizeof(bitmap_pos_struct_t)));
	if (doubleInMcga && (*videoMode_12F02E_12F01E & 1))
		CreateIndexDatTabDoubled_65D70_66280(tab, end, data, *index);
	else
		CreateIndexDatTab_65DC0_662D0(tab, end, data, *index);
}

void FreeSpritesIndex(bitmap_pos_struct_t** index)
{
	Free_426E0_42A20(*index);
	*index = nullptr;
}

void RunMenu_4AB20_4AE60()
{
	if (loadMenuFiles_12CA7C_12CA6C)
	{
		LoadFileGroup_40440_40780(menuFiles_49B80_49EC0);
		BuildSpritesIndex(&menuSpritesIndex, menuSpritesTab_12CAEC_12CADC, menuSpritesTabEnd_12CAF0_12CAE0, menuSpritesData_12CAF4_12CAE4, true);
		loadMenuFiles_12CA7C_12CA6C = 0;
	}
	UpdateMenuInput_50770_50AB0();
	switch (mainMenuItem_12CBCE_12CBBE)
	{
	case 0:
		sub_4D3B0_4D6F0();
		break;
	case 1:
		SoundSetup_4AC70_4AFB0();
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
		SelectLanguage_4F3F0_4F730();
		break;
	case 7:
		PlayIntelIntro_4EFC0_4F300();
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
		FreeSpritesIndex(&menuSpritesIndex);
		if (*videoMode_12F02E_12F01E & 1)
			ClearScreen320_411D7_41517(backScreen_AE3FC_AE3EC, 320, 200, 0);
		else
			//!!!!! 480 rows of 640 do not fit the 0x11170 byte *BScreen; not reached, 34C80 / 35040 switch the menu to MCGA
			ClearScreen640_411FD_4153D(backScreen_AE3FC_AE3EC, 640, 480, 0);
		loadMenuFiles_12CA7C_12CA6C = 1;
		sub_20E60_20E60();
	}
}

// bits of inputFlags_12CAE4_12CAD4 (12CAE4 bit 0 = 0): first bit of each 3-bit group
enum Mc1MenuInputBit {
	INPUT_MOUSE_LEFT = 0, INPUT_MOUSE_MIDDLE = 3, INPUT_MOUSE_RIGHT = 6,
	INPUT_KEY_HOME = 12, INPUT_KEY_PAGEUP = 15, INPUT_KEY_END = 18, INPUT_KEY_PAGEDOWN = 21,
	INPUT_KEY_LEFT = 24, INPUT_KEY_RIGHT = 27, INPUT_KEY_UP = 30, INPUT_KEY_DOWN = 33,
};

static bool InputBit(int bit)
{
	return (inputFlags_12CAE4_12CAD4[bit >> 3] >> (bit & 7)) & 1;
}

static void SetInputBit(int bit, bool value)
{
	if (value)
		inputFlags_12CAE4_12CAD4[bit >> 3] |= (uint8_t)(1 << (bit & 7));
	else
		inputFlags_12CAE4_12CAD4[bit >> 3] &= (uint8_t)~(1 << (bit & 7));
}

static void UpdateInputBits(int first, bool down)
{
	// first + 0 pressed this frame, + 1 released this frame, + 2 held
	bool held = InputBit(first + 2);
	if (down)
	{
		SetInputBit(first + 1, false);
		SetInputBit(first, !held);
		SetInputBit(first + 2, true);
	}
	else
	{
		SetInputBit(first, false);
		SetInputBit(first + 1, held);
		SetInputBit(first + 2, false);
	}
}

void UpdateMenuInput_50770_50AB0()
{
	int8_t* keys = *pressedKeys_12EEF0_12EEE0;
	switch (inputMode_12CBCF_12CBBF)
	{
	case 0:
		// keeps only the held bits of the arrows
		inputFlags_12CAE4_12CAD4[3] &= 0x24;
		inputFlags_12CAE4_12CAD4[4] &= 0xF9;
		break;
	case 1:
		UpdateInputBits(INPUT_KEY_UP, keys[0x48] != 0);
		UpdateInputBits(INPUT_KEY_DOWN, keys[0x50] != 0);
		UpdateInputBits(INPUT_KEY_LEFT, keys[0x4B] != 0);
		UpdateInputBits(INPUT_KEY_RIGHT, keys[0x4D] != 0);
		UpdateInputBits(INPUT_KEY_HOME, keys[0x47] != 0);
		UpdateInputBits(INPUT_KEY_PAGEUP, keys[0x49] != 0);
		UpdateInputBits(INPUT_KEY_END, keys[0x4F] != 0);
		UpdateInputBits(INPUT_KEY_PAGEDOWN, keys[0x51] != 0);
		break;
	case 2:
		if (inputDevices_90B23_90B23 & 0x23)
			sub_3FCA0_3FFE0(2, 0, 0, 640, 400, 4, 4); // the joystick moves the mouse
		mouseX_12CADC_12CACC = mousePos_9AD90_9AD90->x;
		mouseY_12CADE_12CACE = mousePos_9AD90_9AD90->y;
		clickX_12CAE0_12CAD0 = *mouseClickX_9AD8C_9AD8C;
		clickY_12CAE2_12CAD2 = *mouseClickY_9AD8E_9AD8E;
		UpdateInputBits(INPUT_MOUSE_LEFT, *mouseLeftDown_12EFE4_12EFD4 != 0);
		UpdateInputBits(INPUT_MOUSE_RIGHT, *mouseRightDown_12EFE2_12EFD2 != 0);
		UpdateInputBits(INPUT_MOUSE_MIDDLE, *mouseMiddleDown_12EFE0_12EFD0 != 0);
		break;
	}
	*mouseRightPressed_12EFDC_12EFCC = 0;
	*mouseLeftPressed_12EFDE_12EFCE = 0;
}

void CopyBackScreen()
{
	if (*videoMode_12F02E_12F01E & 1)
		CopyScreen320_62FA8_634B8(backScreen_AE3FC_AE3EC, *workScreen_12EFF4_12EFE4, 320, 200);
	else
		CopyScreen640_62FC4_634D4(backScreen_AE3FC_AE3EC, *workScreen_12EFF4_12EFE4, 640, 480);
}

void BlitScreen()
{
	if (*videoMode_12F02E_12F01E & 1)
		Blit320_5CDA0_5D2B0(0);
	else
		Blit640_599B0_59EC0(480, 0);
}

static bool ClickIn(int16_t left, int16_t right, int16_t top, int16_t bottom)
{
	// sar of the 640x400 mouse coordinates
	int16_t x = clickX_12CAE0_12CAD0 >> 1;
	int16_t y = clickY_12CAE2_12CAD2 >> 1;
	return x >= left && x <= right && y >= top && y <= bottom;
}

void SelectLanguage_4F3F0_4F730()
{
	int8_t* keys = *pressedKeys_12EEF0_12EEE0;
	uint8_t* language = &mc1Settings_AE408_AE3F8->language_0x97_151;
	// C:\carpet.cd\language.inf
	std::string path = Mc1InstallPath() + "/language.inf";
	FILE* file = DataFileIO::CreateOrOpenFile(path.c_str(), 0x200); // port: 0x202, only read
	if (!file)
	{
		bool done = false;
		dword_12CBB0_12CBA0 = 0;
		ReadPackedFile_3EEA0_3F1E0("data/screens/language.pal", menuPalette_12CB9C_12CB8C);
		ReadPackedFile_3EEA0_3F1E0("data/screens/language.dat", backScreen_AE3FC_AE3EC);
		LoadFileGroup_40440_40780(languageFiles_4A050_4A390);
		BuildSpritesIndex(&languageSpritesIndex, languageSpritesTab_12CB58_12CB48, languageSpritesTabEnd_12CB5C_12CB4C,
			languageSpritesData_12CB60_12CB50, true);
		CopyBackScreen();
		BlitScreen();
		if (!inputDevices_90B23_90B23)
			SetCursor_5C05C_5C56C(menuSpritesIndex[mainMenuItem_12CBCE_12CBBE]);
		FadeInOut_61CC0_621D0((TColor*)menuPalette_12CB9C_12CB8C, 0x20, false, 10);
		inputFlags_12CAE4_12CAD4[0] &= 0xFE;
		keys[1] = 0;
		menuFlags_12CBD5_12CBC5 |= 1;
		while (!done)
		{
			UpdateMenuInput_50770_50AB0();
			if (inputFlags_12CAE4_12CAD4[0] & 1)
			{
				if (ClickIn(0x40, 0x8B, 0x2B, 0x52))
					*language = 0;
				else if (ClickIn(0x40, 0x8B, 0x5F, 0x86))
					*language = 1;
				else if (ClickIn(0xB3, 0xFE, 0x2B, 0x52))
					*language = 2;
				else if (ClickIn(0xB3, 0xFE, 0x5F, 0x86))
					*language = 3;
				else if (ClickIn(0x119, 0x130, 0x73, 0x8E))
					done = true; // OK
				inputFlags_12CAE4_12CAD4[0] &= 0xFE;
			}
			CopyBackScreen();
			int16_t x = languageFlags_4A8F4_4AC34[1 + *language * 2] * 2;
			int16_t y = languageFlags_4A8F4_4AC34[2 + *language * 2] * 2;
			if (*videoMode_12F02E_12F01E & 1)
				DrawBitmap320_60CE0_611F0(x, y, languageSpritesIndex[*language + 1], 1);
			else
				DrawBitmap640_60D18_61228(x, y, languageSpritesIndex[*language + 1], 1);
			BlitScreen();
			dword_12CBB0_12CBA0++;
		}
		FadeInOut_61CC0_621D0(nullptr, 0x10, false, 10);
		if (!inputDevices_90B23_90B23)
			SetCursor_5C05C_5C56C(bitmap_pos_struct_t{});
		keys[1] = 0;
		inputFlags_12CAE4_12CAD4[0] &= 0xFE;
		file = DataFileIO::CreateOrOpenFile(path.c_str(), 0x222);
		if (file)
		{
			WriteFile_62ED0_633E0(file, language, 1);
			DataFileIO::Close(file);
		}
	}
	else
	{
		DataFileIO::Close(file);
		file = DataFileIO::CreateOrOpenFile(path.c_str(), 0x200);
		if (file)
		{
			DataFileIO::Read(file, language, 1);
			DataFileIO::Close(file);
		}
	}
	switch (*language)
	{
	case 0:
		LoadFileGroup_40440_40780(etextFiles_99580_99580);
		break;
	case 1:
		LoadFileGroup_40440_40780(ftextFiles_995D8_995D8);
		break;
	case 2:
		LoadFileGroup_40440_40780(gtextFiles_99630_99630);
		break;
	case 3:
		LoadFileGroup_40440_40780(itextFiles_99688_99688);
		break;
	}
	FreeFileGroup_61610_61B20(languageFiles_4A050_4A390);
	FreeSpritesIndex(&languageSpritesIndex);
	SetTextPointers_44700_44A40((char*)textData_AE41C_AE40C, (char**)textPointers_AE238_AE228, 80);
	mainMenuItem_12CBCE_12CBBE = 1;
	menuFlags_12CBD5_12CBC5 &= 0xFE;
}
