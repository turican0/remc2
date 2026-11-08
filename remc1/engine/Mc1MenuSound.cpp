#include "Mc1MenuSound.h"

#include <cstdio>
#include <cstring>
#include <string>

#include "Mc1Game.h"
#include "Mc1Graphics.h"
#include "Mc1Install.h"
#include "Mc1Menu.h"
#include "Mc1MenuText.h"
#include "Mc1NotPorted.h"
#include "Mc1Screen.h"
#include "Mc1Shared.h"
#include "Mc1Sound.h"
#include "Mc1System.h"
#include "../../remc2/utilities/DataFileIO.h"

uint8_t* configSpritesTab_12CAF8_12CAE8 = nullptr;
uint8_t* configSpritesTabEnd_12CAFC_12CAEC = nullptr;
uint8_t* configSpritesData_12CB00_12CAF0 = nullptr;
uint8_t* fontTab_12CB28_12CB18 = nullptr;
uint8_t* fontTabEnd_12CB2C_12CB1C = nullptr;
uint8_t* fontData_12CB30_12CB20 = nullptr;
bitmap_pos_struct_t* configSpritesIndex = nullptr;
bitmap_pos_struct_t* fontIndex = nullptr;

Type_Mc1FileEntry configFiles_49C88_49FC8[5] = {
	{ { "data/screens/sfont0.dat", "data/screens/sfont0.dat" }, &fontData_12CB30_12CB20, nullptr, 0, 0 },
	{ { "data/screens/sfont0.tab", "data/screens/sfont0.tab" }, &fontTab_12CB28_12CB18, &fontTabEnd_12CB2C_12CB1C, 0, 0 },
	{ { "data/screens/gcspr.dat", "data/screens/gcspr.dat" }, &configSpritesData_12CB00_12CAF0, nullptr, 0, 0 },
	{ { "data/screens/gcspr.tab", "data/screens/gcspr.tab" }, &configSpritesTab_12CAF8_12CAE8, &configSpritesTabEnd_12CAFC_12CAEC, 0, 0 },
	{ { "", "" }, nullptr, nullptr, 0, 0 },
};

// 4A8BE / 4ABFE; devices 3 and 5 are skipped when clicking through them
const Type_Mc1ControlDevice controlDevices_4A8BE_4ABFE[7] = {
	{ 0xB1, 0x30, 9, 0x04 },
	{ 0xB4, 0x31, 7, 0x02 },
	{ 0xBA, 0x31, 2, 0x05 },
	{ 0xC0, 0x2D, 3, 0x20 },
	{ 0xB1, 0x31, 1, 0x0D },
	{ 0xB7, 0x2A, 4, 0x28 },
	{ 0xB4, 0x27, 8, 0x0A },
};

// 968B4: the help text of each page (textPointers_AE238_AE228 index)
static const uint16_t pageHelpTexts_968B4_968B4[7] = { 0x11, 0x12, 0x17, 0x18, 0x19, 0x1A, 0x1C };

// option lists of the pages, ended by "!"
static const Type_Mc1SoundOption soundCards_4A66C_4A9AC[] = {
	{ "No sound", "NONE" }, { "Soundblaster", "SBLAST" }, { "Soundblaster pro", "SBPRO" },
	{ "Soundblaster 16", "SB16" }, { "Adlib gold", "ADLIBG" }, { "Microsoft 8", "MICROSOFT8" },
	{ "Microsoft 16", "MICROSOFT16" }, { "Pro audio spectrum 8", "PAS8" }, { "Pro audio spectrum 16", "PAS16" },
	{ "Sound master II", "SOUNDMASTERII" }, { "Soundblaster compatible", "COMPATIBLE" }, { "!", "!" },
};
static const Type_Mc1SoundOption musicCards_4A6CC_4AA0C[] = {
	{ "No music", "NONE" }, { "Adlib fm", "ADLIB" }, { "Adlib gold fm", "ADLIBG" },
	{ "Soundblaster fm", "SBLAST" }, { "Soundblaster pro fm", "SBPRO" }, { "Soundblaster 16 fm", "SB16FM" },
	{ "Soundblaster AWE 32", "SBAWE32" }, { "Wave blaster", "WBLAST" }, { "Roland mt32", "ROLAND" },
	{ "General midi", "GENERAL" }, { "Pro audio spectrum fm", "PASFM" }, { "Soundblaster compatible", "COMPATIBLE" },
	{ "!", "!" },
};
static const Type_Mc1SoundOption soundPorts_4A734_4AA74[] = { { "220", "220" }, { "240", "240" }, { "!", "!" } };
static const Type_Mc1SoundOption soundIrqs_4A74C_4AA8C[] = {
	{ "2", "2" }, { "3", "3" }, { "5", "5" }, { "7", "7" }, { "10", "10" }, { "!", "!" },
};
static const Type_Mc1SoundOption soundDmas_4A77C_4AABC[] = { { "0", "0" }, { "1", "1" }, { "3", "3" }, { "!", "!" } };
static const Type_Mc1SoundOption musicPorts_4A79C_4AADC[] = { { "300", "300" }, { "330", "330" }, { "!", "!" } };
static const Type_Mc1SoundOption saveSetup_4A7B4_4AAF4[] = { { "YES", "YES" }, { "NO", "NO" }, { "!", "!" } };

static bool MouseIn(int16_t left, int16_t right, int16_t top, int16_t bottom)
{
	// sar of the 640x400 mouse coordinates
	int16_t x = mouseX_12CADC_12CACC >> 1;
	int16_t y = mouseY_12CADE_12CACE >> 1;
	return x >= left && x <= right && y >= top && y <= bottom;
}

static void SelectOption(char* text, char* value, const Type_Mc1SoundOption* options)
{
	sprintf(text, "%s", options[hoveredOption_12CBCB_12CBBB].text_0);
	sprintf(value, "%s", options[hoveredOption_12CBCB_12CBBB].value_4);
}

static void SaveSoundSetup()
{
	// C:\carpet.cd\sndsetup.inf, written with fopen "wt" / fprintf
	std::string path = Mc1InstallPath() + "/sndsetup.inf";
	FILE* file = DataFileIO::CreateOrOpenFile(path.c_str(), 0x222);
	if (file)
	{
		char line[160];
		snprintf(line, sizeof(line), "SOUNDFX = %s %s %s %s\n", soundDriver_12C9A0_12C990, soundPort_12CA9E_12CA8E,
			soundIrq_12CAB2_12CAA2, soundDma_12CAC6_12CAB6);
		DataFileIO::x_write2(file, (uint8_t*)line, (uint32_t)strlen(line));
		snprintf(line, sizeof(line), "MUSIC = %s %s 0 0 \n", musicDriver_12CA00_12C9F0, musicPort_12CAA8_12CA98);
		DataFileIO::x_write2(file, (uint8_t*)line, (uint32_t)strlen(line));
		DataFileIO::Close(file);
	}
	// C:\carpet.cd\sndsetup.dat, read back by LoadSoundSetup_505A0_508E0
	path = Mc1InstallPath() + "/sndsetup.dat";
	file = DataFileIO::CreateOrOpenFile(path.c_str(), 0x222);
	if (file)
	{
		WriteFile_62ED0_633E0(file, (uint8_t*)soundDriver_12C9A0_12C990, 32);
		WriteFile_62ED0_633E0(file, (uint8_t*)soundCardText_12C9C0_12C9B0, 32);
		WriteFile_62ED0_633E0(file, (uint8_t*)musicDriver_12CA00_12C9F0, 32);
		WriteFile_62ED0_633E0(file, (uint8_t*)musicCardText_12C9E0_12C9D0, 32);
		WriteFile_62ED0_633E0(file, (uint8_t*)soundPort_12CA9E_12CA8E, 10);
		WriteFile_62ED0_633E0(file, (uint8_t*)soundPortText_12CA80_12CA70, 10);
		WriteFile_62ED0_633E0(file, (uint8_t*)soundIrq_12CAB2_12CAA2, 10);
		WriteFile_62ED0_633E0(file, (uint8_t*)soundIrqText_12CA8A_12CA7A, 10);
		WriteFile_62ED0_633E0(file, (uint8_t*)soundDma_12CAC6_12CAB6, 10);
		WriteFile_62ED0_633E0(file, (uint8_t*)soundDmaText_12CAD0_12CAC0, 10);
		WriteFile_62ED0_633E0(file, (uint8_t*)musicPort_12CAA8_12CA98, 10);
		WriteFile_62ED0_633E0(file, (uint8_t*)musicPortText_12CA94_12CA84, 10);
		DataFileIO::Close(file);
	}
}

static bool MusicNeedsPort()
{
	return !strcmp(musicDriver_12CA00_12C9F0, "WBLAST") || !strcmp(musicDriver_12CA00_12C9F0, "ROLAND")
		|| !strcmp(musicDriver_12CA00_12C9F0, "GENERAL") || !strcmp(musicDriver_12CA00_12C9F0, "SBAWE32");
}

static void ClickOption()
{
	switch (menuPage_12CBC9_12CBB9)
	{
	case 0:
		if (!IsOptionValid_4B520_4B860())
			return;
		SelectOption(soundCardText_12C9C0_12C9B0, soundDriver_12C9A0_12C990, soundCards_4A66C_4A9AC);
		menuPage_12CBC9_12CBB9 = 1;
		return;
	case 1:
		if (!IsOptionValid_4B520_4B860())
			return;
		SelectOption(musicCardText_12C9E0_12C9D0, musicDriver_12CA00_12C9F0, musicCards_4A6CC_4AA0C);
		if (strcmp(soundDriver_12C9A0_12C990, "NONE"))
			menuPage_12CBC9_12CBB9 = 2;
		else if (strcmp(musicDriver_12CA00_12C9F0, "NONE"))
			menuPage_12CBC9_12CBB9 = 5;
		else
			menuPage_12CBC9_12CBB9 = 6;
		return;
	case 2:
		if (!IsOptionValid_4B520_4B860())
			return;
		SelectOption(soundPortText_12CA80_12CA70, soundPort_12CA9E_12CA8E, soundPorts_4A734_4AA74);
		menuPage_12CBC9_12CBB9 = 3;
		return;
	case 3:
		if (!IsOptionValid_4B520_4B860())
			return;
		SelectOption(soundIrqText_12CA8A_12CA7A, soundIrq_12CAB2_12CAA2, soundIrqs_4A74C_4AA8C);
		menuPage_12CBC9_12CBB9 = 4;
		return;
	case 4:
		if (!IsOptionValid_4B520_4B860())
			return;
		SelectOption(soundDmaText_12CAD0_12CAC0, soundDma_12CAC6_12CAB6, soundDmas_4A77C_4AABC);
		menuPage_12CBC9_12CBB9 = 5;
		if (!MusicNeedsPort())
		{
			menuPage_12CBC9_12CBB9 = 6;
			sprintf(musicPortText_12CA94_12CA84, "%s", "388");
			sprintf(musicPort_12CAA8_12CA98, "%s", "388");
		}
		return;
	case 5:
		if (!IsOptionValid_4B520_4B860())
			return;
		SelectOption(musicPortText_12CA94_12CA84, musicPort_12CAA8_12CA98, musicPorts_4A79C_4AADC);
		menuPage_12CBC9_12CBB9 = 6;
		return;
	case 6:
		if (!IsOptionValid_4B520_4B860())
			return;
		SelectOption(saveSetupText_12CA20_12CA10, saveSetup_12CABC_12CAAC, saveSetup_4A7B4_4AAF4);
		if (!strcmp(saveSetup_12CABC_12CAAC, yesText_A99A0_A99B8))
		{
			SaveSoundSetup();
			menuPage_12CBC9_12CBB9 = 7;
		}
		else
			menuPage_12CBC9_12CBB9 = 0;
		return;
	}
}

void SoundSetup_4AC70_4AFB0()
{
	int8_t* keys = *pressedKeys_12EEF0_12EEE0;
	bool leave = false;
	bool escape = false;
	if (!(menuFlags_12CBD5_12CBC5 & 1))
		OpenSoundSetup_4BB20_4BE60();
	if (keys[1])
	{
		keys[1] = 0;
		escape = true;
		leave = true;
	}
	if ((inputFlags_12CAE4_12CAD4[0] & 1) || (inputFlags_12CAE4_12CAD4[0] & 0x40))
	{
		if (MouseIn(0x118, 0x12E, 0x79, 0x93) && menuPage_12CBC9_12CBB9 == 7)
		{
			keys[1] = 0;
			leave = true; // OK
		}
	}
	hoveredOption_12CBCB_12CBBB = 0xFF;
	if (MouseIn(0x38, 0x92, 0x40, 0x8F))
	{
		if (menuPage_12CBC9_12CBB9 == 7)
		{
			if (inputFlags_12CAE4_12CAD4[0] & 1)
			{
				inputFlags_12CAE4_12CAD4[0] &= 0xFE;
				menuPage_12CBC9_12CBB9 = 0; // a click into the summary starts the setup again
			}
		}
		else
			hoveredOption_12CBCB_12CBBB = (uint8_t)(((mouseY_12CADE_12CACE >> 1) - 0x40) / (menuFont_96884_96884.glyphs_0x00_0[0].height_5 - 2));
	}
	if (inputFlags_12CAE4_12CAD4[0] & 1)
	{
		*mouseLeftPressed_12EFDE_12EFCE = 0;
		if (MouseIn(0xAB, 0x108, 0x2A, 0x93))
		{
			// the control device icon: 0, 1, 2, 4, 6
			controlDevice_12CBD3_12CBC3++;
			if (controlDevice_12CBD3_12CBC3 == 7)
				controlDevice_12CBD3_12CBC3 = 0;
			if (controlDevice_12CBD3_12CBC3 == 3)
				controlDevice_12CBD3_12CBC3 = 4;
			if (controlDevice_12CBD3_12CBC3 == 5)
				controlDevice_12CBD3_12CBC3 = 6;
		}
		else if (MouseIn(0x38, 0x92, 0x40, 0x8F))
			ClickOption();
	}
	CopyBackScreen();
	DrawSoundSetup_4B560_4B8A0();
	BlitScreen();
	if (!leave)
		return;
	SetControlDevice_51DF0_52130(controlDevices_4A8BE_4ABFE[controlDevice_12CBD3_12CBC3].flags_6);
	CloseSoundSetup_4BC90_4BFD0();
	if (!(menuFlags_12CBD5_12CBC5 & 0x80))
	{
		SoundInit_357C0_35B80();
		menuFlags_12CBD5_12CBC5 |= 0x80;
	}
	if ((menuFlags_12CBD5_12CBC5 & 2) && (menuFlags_12CBD5_12CBC5 & 4) && !(inputFlags_12CAE4_12CAD4[0] & 0x40))
		mainMenuItem_12CBCE_12CBBE = 2;
	else if (escape)
		mainMenuItem_12CBCE_12CBBE = 2;
	else
		mainMenuItem_12CBCE_12CBBE = 7;
	inputFlags_12CAE4_12CAD4[0] &= 0xBE;
}

int32_t IsOptionValid_4B520_4B860()
{
	const Type_Mc1SoundOption* option = GetSoundOptions_4B6A0_4B9E0();
	for (uint8_t index = 0; option->text_0[0] != '!' && index != hoveredOption_12CBCB_12CBBB; index++)
		option++;
	return option->text_0[0] != '!';
}

void DrawSoundSetup_4B560_4B8A0()
{
	SaveTextWindow_513E0_51720();
	SetTextWindow_51360_516A0(0x38, 0x40, 0x5A, 0x4E);
	const Type_Mc1SoundOption* options = GetSoundOptions_4B6A0_4B9E0();
	if (!options)
		DrawSoundSummary_4B8D0_4BC10(0);
	else
		DrawSoundOptions_4B6F0_4BA30(options);
	if (menuPage_12CBC9_12CBB9 == 7)
	{
		// the OK button
		if (*videoMode_12F02E_12F01E & 1)
			DrawBitmap320_60CE0_611F0(0x22A, 0xE0, configSpritesIndex[11], 1);
		else
			DrawBitmap640_60D18_61228(0x22A, 0xE0, configSpritesIndex[11], 1);
	}
	RestoreTextWindow_51400_51740();
	const Type_Mc1ControlDevice& device = controlDevices_4A8BE_4ABFE[controlDevice_12CBD3_12CBC3];
	if (*videoMode_12F02E_12F01E & 1)
		DrawBitmap320_60CE0_611F0(device.x_0 * 2, device.y_2 * 2, configSpritesIndex[device.sprite_4], 1);
	else
		DrawBitmap640_60D18_61228(device.x_0 * 2, device.y_2 * 2, configSpritesIndex[device.sprite_4], 1);
}

const Type_Mc1SoundOption* GetSoundOptions_4B6A0_4B9E0()
{
	switch (menuPage_12CBC9_12CBB9)
	{
	case 0:
		return soundCards_4A66C_4A9AC;
	case 1:
		return musicCards_4A6CC_4AA0C;
	case 2:
		return soundPorts_4A734_4AA74;
	case 3:
		return soundIrqs_4A74C_4AA8C;
	case 4:
		return soundDmas_4A77C_4AABC;
	case 5:
		return musicPorts_4A79C_4AADC;
	case 6:
		return saveSetup_4A7B4_4AAF4;
	default:
		return nullptr; // 7 the summary; the original returns eax unchanged above 7
	}
}

void DrawSoundOptions_4B6F0_4BA30(const Type_Mc1SoundOption* options)
{
	if (!options)
		return;
	int32_t lineHeight = menuFont_96884_96884.glyphs_0x00_0[0].height_5 - 2;
	menuFont_96884_96884.flags_0x04_4 &= 0xFD;
	uint8_t normalColor = FindNearestColor_5CC70_5D180(menuPalette_12CB9C_12CB8C, 0x26, 0x22, 0x19);
	uint8_t highlightColor = FindNearestColor_5CC70_5D180(menuPalette_12CB9C_12CB8C, 0x1E, 0x18, 0x11);
	uint8_t hoverColor = FindNearestColor_5CC70_5D180(menuPalette_12CB9C_12CB8C, 0x20, 0x1E, 0x17);
	int32_t y = 0;
	uint8_t index = 0;
	if (menuPage_12CBC9_12CBB9 == 6)
	{
		// the translated YES / NO
		for (const char* text : { (const char*)textPointers_AE238_AE228[73], (const char*)textPointers_AE238_AE228[74] })
		{
			menuFont_96884_96884.color1_0x06_6 = normalColor;
			if (index == hoveredOption_12CBCB_12CBBB)
				menuFont_96884_96884.color1_0x06_6 = hoverColor;
			if (index == byte_12CBCA_12CBBA)
				menuFont_96884_96884.color1_0x06_6 = highlightColor;
			DrawText_51560_518A0(&menuFont_96884_96884, 0, (int16_t)y, text);
			index++;
			y += lineHeight;
		}
	}
	else
	{
		for (; options->text_0[0] != '!'; options++)
		{
			menuFont_96884_96884.color1_0x06_6 = normalColor;
			if (index == hoveredOption_12CBCB_12CBBB)
				menuFont_96884_96884.color1_0x06_6 = hoverColor;
			if (index == byte_12CBCA_12CBBA)
				menuFont_96884_96884.color1_0x06_6 = highlightColor;
			DrawText_51560_518A0(&menuFont_96884_96884, 0, (int16_t)y, options->text_0);
			index++;
			y += lineHeight;
		}
	}
	if (menuPage_12CBC9_12CBB9 == 6)
		DrawSoundSummary_4B8D0_4BC10(0x10);
	if (menuPage_12CBC9_12CBB9 < 7)
	{
		SaveTextWindow_513E0_51720();
		SetTextWindow_51360_516A0(0x42, 0x2B, 0x54, 0x0E);
		menuFont_96884_96884.color1_0x06_6 = highlightColor;
		DrawText_51560_518A0(&menuFont_96884_96884, 1, 1,
			(const char*)textPointers_AE238_AE228[pageHelpTexts_968B4_968B4[menuPage_12CBC9_12CBB9]]);
		RestoreTextWindow_51400_51740();
	}
}

static void DrawRightAligned(const Type_Mc1TextWindow_12C970& window, int32_t y, const char* text)
{
	int16_t x = (int16_t)(window.right_4 - window.left_0 - TextWidth_51420_51760(&menuFont_96884_96884, text));
	DrawText_51560_518A0(&menuFont_96884_96884, x, (int16_t)y, text);
}

void DrawSoundSummary_4B8D0_4BC10(int32_t y)
{
	char line[40];
	int32_t lineHeight = menuFont_96884_96884.glyphs_0x00_0[0].height_5;
	Type_Mc1TextWindow_12C970 window;
	GetTextWindow_513A0_516E0(&window);
	menuFont_96884_96884.color1_0x06_6 = FindNearestColor_5CC70_5D180(menuPalette_12CB9C_12CB8C, 0x1E, 0x18, 0x11);
	sprintf(line, "%s :", (const char*)textPointers_AE238_AE228[17]);
	DrawText_51560_518A0(&menuFont_96884_96884, 0, (int16_t)y, line);
	lineHeight -= 2;
	sprintf(line, "%s", soundCardText_12C9C0_12C9B0);
	y += lineHeight;
	DrawRightAligned(window, y, line);
	sprintf(line, "%s :", (const char*)textPointers_AE238_AE228[18]);
	y += lineHeight;
	DrawText_51560_518A0(&menuFont_96884_96884, 0, (int16_t)y, line);
	sprintf(line, "%s", musicCardText_12C9E0_12C9D0);
	y += lineHeight;
	DrawRightAligned(window, y, line);
	y += lineHeight;
	if (strncmp(soundDriver_12C9A0_12C990, "NONE", 4))
	{
		sprintf(line, "%s : %s\n", (const char*)textPointers_AE238_AE228[23], soundPortText_12CA80_12CA70);
		DrawText_51560_518A0(&menuFont_96884_96884, 0, (int16_t)y, line);
		sprintf(line, "%s : %s\n", (const char*)textPointers_AE238_AE228[24], soundIrqText_12CA8A_12CA7A);
		y += lineHeight;
		DrawText_51560_518A0(&menuFont_96884_96884, 0, (int16_t)y, line);
		sprintf(line, "%s : %s\n", (const char*)textPointers_AE238_AE228[25], soundDmaText_12CAD0_12CAC0);
		y += lineHeight;
		DrawText_51560_518A0(&menuFont_96884_96884, 0, (int16_t)y, line);
		y += lineHeight;
	}
	if (strncmp(musicDriver_12CA00_12C9F0, "NONE", 4))
	{
		sprintf(line, "%s : %s\n", (const char*)textPointers_AE238_AE228[26], musicPortText_12CA94_12CA84);
		DrawText_51560_518A0(&menuFont_96884_96884, 0, (int16_t)y, line);
	}
}

void OpenSoundSetup_4BB20_4BE60()
{
	FadeInOut_61CC0_621D0(nullptr, 0x10, false, 10);
	LoadFileGroup_40440_40780(configFiles_49C88_49FC8);
	ReadPackedFile_3EEA0_3F1E0("data/screens/gconfig.dat", backScreen_AE3FC_AE3EC);
	ReadPackedFile_3EEA0_3F1E0("data/screens/gconfig.pal", menuPalette_12CB9C_12CB8C);
	BuildSpritesIndex(&configSpritesIndex, configSpritesTab_12CAF8_12CAE8, configSpritesTabEnd_12CAFC_12CAEC,
		configSpritesData_12CB00_12CAF0, true);
	// 51480 / 517C0 relocates the font tab without doubling (519E0 / 51D20)
	BuildSpritesIndex(&fontIndex, fontTab_12CB28_12CB18, fontTabEnd_12CB2C_12CB1C, fontData_12CB30_12CB20, false);
	InitFont_51480_517C0(&menuFont_96884_96884, fontIndex, menuPalette_12CB9C_12CB8C);
	menuFont_96884_96884.flags_0x04_4 &= 0xFD;
	if (!inputDevices_90B23_90B23)
		SetCursor_5C05C_5C56C(menuSpritesIndex[mainMenuItem_12CBCE_12CBBE]);
	CopyBackScreen();
	BlitScreen();
	FadeInOut_61CC0_621D0((TColor*)menuPalette_12CB9C_12CB8C, 0x20, false, 10);
	menuFlags_12CBD5_12CBC5 |= 1;
	inputFlags_12CAE4_12CAD4[0] &= 0xBE;
}

void CloseSoundSetup_4BC90_4BFD0()
{
	FadeInOut_61CC0_621D0(nullptr, 0x10, false, 10);
	if (!inputDevices_90B23_90B23)
		SetCursor_5C05C_5C56C(bitmap_pos_struct_t{});
	if (*videoMode_12F02E_12F01E & 1)
		ClearScreen320_411D7_41517(*workScreen_12EFF4_12EFE4, 320, 200, 0);
	else
		ClearScreen640_411FD_4153D(*workScreen_12EFF4_12EFE4, 640, 480, 0);
	BlitScreen();
	FreeFileGroup_61610_61B20(configFiles_49C88_49FC8);
	FreeSpritesIndex(&configSpritesIndex);
	FreeSpritesIndex(&fontIndex);
	menuFlags_12CBD5_12CBC5 &= 0xFE;
}

void SetControlDevice_51DF0_52130(uint16_t flags)
{
	inputDevices_90B23_90B23 = 0;
	if (flags & 2)
	{
		InitDigitalJoystick_3F7D0_3FB10(&configSpritesIndex[6]);
		if (*mouseInstalled_9AD98_9AD98)
			ResetMouse_3F370_3F6B0();
	}
	else if ((flags & 1) || (flags & 0x20))
	{
		// the joystick calibration screens use gcspr records 10, 5 and 6
		if (mc1Variant == MC1_VARIANT_HW)
			InitAnalogJoystick_3FB60(&configSpritesIndex[10], &configSpritesIndex[5], &configSpritesIndex[6], (uint8_t)flags);
		else
			InitAnalogJoystick_3F820(&configSpritesIndex[10], &configSpritesIndex[5], &configSpritesIndex[6], (uint8_t)flags);
		if (*mouseInstalled_9AD98_9AD98)
			ResetMouse_3F370_3F6B0();
	}
	if ((flags & 8) && InitVfx_5AF50_5B460())
	{
		mc1State_AE400_AE3F0->vfx_0x219E_8606 = 1;
		inputDevices_90B23_90B23 |= 8;
	}
}
