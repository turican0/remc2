#include "Mc1Game.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Mc1Blocks.h"
#include "Mc1Files.h"
#include "Mc1Graphics.h"
#include "Mc1Install.h"
#include "Mc1EntityTables.h"
#include "Mc1Level.h"
#include "Mc1Menu.h"
#include "Mc1Sound.h"
#include "Mc1World.h"
#include "Mc1NotPorted.h"
#include "Mc1Screen.h"
#include "Mc1Search.h"
#include "Mc1TextureMaps.h"
#include "Mc1System.h"
#include "../../remc2/engine/Graphics.h"
#include "../../remc2/engine/PlayerInput.h"
#include "../../remc2/portability/port_filesystem.h"

#if defined(__linux__) || defined(__APPLE__)
#include <strings.h>
#define _stricmp strcasecmp
#endif

Mc1Variant mc1Variant = MC1_VARIANT_MC1;
Type_Mc1State_AE400_AE3F0* mc1State_AE400_AE3F0 = nullptr;
Type_Mc1Settings_AE408_AE3F8* mc1Settings_AE408_AE3F8 = nullptr;
uint8_t* blurBuffer_AE404_AE3F4 = nullptr;
uint8_t* textPointers_AE238_AE228[80];
uint8_t byte_939CC_939CC = 1;
uint8_t byte_939CD_939CD = 1;
uint8_t byte_939E4_939E4 = 1;
uint8_t byte_939E5_939E5 = 1;
uint8_t menuDone_96880_96880 = 0;
uint8_t* vesaInfo_12F080_12F070 = nullptr;
uint8_t debugOption_AE45A_AE44A = 0;
uint8_t inputDevices_90B23_90B23 = 0;
uint8_t joystickFound_B7700_B76F0 = 0;
uint8_t byte_A9058_A9058 = 0;

static Type_Mc1Player_340B* CurrentPlayer()
{
	return &mc1State_AE400_AE3F0->players_0x340B_13323[mc1State_AE400_AE3F0->currentPlayer_0x08_8];
}

int Main_404F0_40830(int argc, char** argv)
{
	CreateInstallDirectory_3EC90_3EFD0('C', "\\carpet.cd", "save");
	CreateInstallDirectory_3EC90_3EFD0('C', "\\carpet.cd", "data");
	CreateInstallDirectory_3EC90_3EFD0('C', "\\carpet.cd", "levels");
	// the same main calls the game's own top procedure
	if (mc1Variant == MC1_VARIANT_HW)
		TopProcedure_34470((uint16_t)argc, argv);
	else
		TopProcedure_340B0((uint16_t)argc, argv);
	return 0;
}

int TopProcedure_340B0(int argc, char** argv)
{
	if (ProcessCommandLine_34DD0_35190((uint16_t)argc, argv) == -1)
		return -1;
	InitGameData_34B40_34F00();
	if (!(mc1Settings_AE408_AE3F8->flags_0x00_0 & 8))
	{
		mc1Settings_AE408_AE3F8->flag_0x17_23 = 0;
		SetupInstallFiles_3F1D0_3F510();
		ProcessTmaps_58F90_594A0();
		InitMemory_41230_41570();
		InitGraphics_44730_44A70();
		if (mc1Settings_AE408_AE3F8->flags_0x00_0 & 0x100)
		{
			byte_939CD_939CD = 0;
			byte_939E5_939E5 = 0;
			byte_939CC_939CC = 0;
			byte_939E4_939E4 = 0;
			LoadFileGroup_40440_40780(ftextFiles_995D8_995D8);
			SetTextPointers_44700_44A40((char*)textData_AE41C_AE40C, (char**)textPointers_AE238_AE228, 80);
		}
		else
			InitMenu_4A980_4ACC0();

		CurrentPlayer()->exitGame_0x04_4 = 0;
		int16_t screenMode = 1;
		uint32_t seed = 0;
		while (!CurrentPlayer()->exitGame_0x04_4)
		{
			FadeInOut_61CC0_621D0(nullptr, 0x10, false, 10);
			if (!(mc1Settings_AE408_AE3F8->flags_0x00_0 & 0x100))
			{
				SetFullScreen_31A00_31A40();
				screenMode = 1;
				if (*videoMode_12F02E_12F01E == 8)
				{
					sub_34C80();
					screenMode = 8;
				}
				ShrinkHeapForMenu_41490_417D0();
				SetAllocationTag_42530_42870(2);
				byte_939E5_939E5 = byte_939E4_939E4;
				loadMenuFiles_12CA7C_12CA6C = 1;
				byte_939CD_939CD = byte_939CC_939CC;
				menuDone_96880_96880 = 0;
				do
					RunMenu_4AB20_4AE60();
				while (!menuDone_96880_96880);
				SkipMissingLevel_34070();
				FadeInOut_61CC0_621D0(nullptr, 0x10, false, 10);
				SetCursor_5C05C_5C56C(pointersIndex[0]);
				if (!CurrentPlayer()->exitGame_0x04_4)
					LoadingScreen_34460_34820();
				LoadGameData_34B00_34EC0();
				FreeFileGroup_61610_61B20(soundFiles_9AE84_9AE84);
				FreeFileGroup_61610_61B20(musicFiles_9AE00_9AE00);
				GrowHeapAfterMenu_41540_41880();
				if (!CurrentPlayer()->exitGame_0x04_4)
				{
					LoadSoundBank_5D070_5D580(0);
					LoadMusicBank_5CEF0_5D400(0);
				}
				if (!(mc1Settings_AE408_AE3F8->flags_0x00_0 & 4))
				{
					SetFullScreen_31A00_31A40();
					if (screenMode == 8)
						sub_34C80();
					RestoreScreenSize_31A60_31AA0();
				}
				SetAllocationTag_42530_42870(3);
			}
			if (!CurrentPlayer()->exitGame_0x04_4)
			{
				seed = mc1Settings_AE408_AE3F8->levelIndex_0x11_17;
				LoadLevel_407A0_40AE0();
				if (*videoMode_12F02E_12F01E == 1 && !blurBuffer_AE404_AE3F4)
					blurBuffer_AE404_AE3F4 = (uint8_t*)Malloc_42540_42880(64000);
			}
			while (!CurrentPlayer()->exitGame_0x04_4)
			{
				if (byte_939CC_939CC && byte_939CD_939CD && musicCount_939D6_939D6)
				{
					seed = seed * 0x24A1 + 0x24DF;
					// movsx + unsigned div: the 16-bit seed is sign extended before the division
					mc1State_AE400_AE3F0->levelVariant_0x240_576 = (int32_t)((uint32_t)(int32_t)(int16_t)seed % 3 + 1);
					PlayMusic_5D290_5D7A0((int16_t)mc1State_AE400_AE3F0->levelVariant_0x240_576);
				}
				sub_59720_59C30();
				GameLoop_34610_349D0();
				StopAllSamples_5D010_5D520();
				StopMusic_20E60_20E60();
				FadeInOut_61CC0_621D0(nullptr, 0x10, false, 10);
				Type_Mc1Player_340B* player = CurrentPlayer();
				if ((player->levelResult_0x02_2 & 6) != 4)
				{
					if (player->levelResult_0x02_2 & 2)
					{
						player->levelResult_0x02_2 = 2;
						sub_448E0_44C20();
						mc1Settings_AE408_AE3F8->levelIndex_0x11_17++;
					}
					else
						player->levelResult_0x02_2 = 8;
					break;
				}
				sub_408D0_40C10();
				CurrentPlayer()->levelResult_0x02_2 = 4;
			}
			FadeInOut_61CC0_621D0(nullptr, 0x10, false, 10);
		}
		ShutdownSound_358A0_35C60();
	}
	// the original calls an empty function (nullsub_2) when flag 8 is set
	FadeInOut_61CC0_621D0(nullptr, 0x10, false, 10);
	Cleanup_34C60_35020();
	sub_433C0_43700();
	return 0;
}

int TopProcedure_34470(int argc, char** argv)
{
	PatchClassTablesHw();
	if (ProcessCommandLine_34DD0_35190((uint16_t)argc, argv) == -1)
		return -1;
	InitGameData_34B40_34F00();
	if (!(mc1Settings_AE408_AE3F8->flags_0x00_0 & 8))
	{
		mc1Settings_AE408_AE3F8->flag_0x17_23 = 0;
		SetupInstallFiles_3F1D0_3F510();
		ProcessTmaps_58F90_594A0();
		InitMemory_41230_41570();
		InitGraphics_44730_44A70();
		if (mc1Settings_AE408_AE3F8->flags_0x00_0 & 0x100)
		{
			byte_939CD_939CD = 0;
			byte_939E5_939E5 = 0;
			byte_939CC_939CC = 0;
			byte_939E4_939E4 = 0;
			LoadFileGroup_40440_40780(ftextFiles_995D8_995D8);
			SetTextPointers_44700_44A40((char*)textData_AE41C_AE40C, (char**)textPointers_AE238_AE228, 80);
		}
		else
			InitMenu_4A980_4ACC0();

		CurrentPlayer()->exitGame_0x04_4 = 0;
		int16_t screenMode = 1;
		uint32_t seed = 0;
		while (!CurrentPlayer()->exitGame_0x04_4)
		{
			FadeInOut_61CC0_621D0(nullptr, 0x10, false, 10);
			if (!(mc1Settings_AE408_AE3F8->flags_0x00_0 & 0x100))
			{
				SetFullScreen_31A00_31A40();
				screenMode = 1;
				if (*videoMode_12F02E_12F01E == 8)
				{
					sub_35040();
					screenMode = 8;
				}
				ShrinkHeapForMenu_41490_417D0();
				SetAllocationTag_42530_42870(2);
				byte_939E5_939E5 = byte_939E4_939E4;
				loadMenuFiles_12CA7C_12CA6C = 1;
				byte_939CD_939CD = byte_939CC_939CC;
				menuDone_96880_96880 = 0;
				do
					RunMenu_4AB20_4AE60();
				while (!menuDone_96880_96880);
				// Hidden Worlds has all levels, no SkipMissingLevel here
				FadeInOut_61CC0_621D0(nullptr, 0x10, false, 10);
				SetCursor_5C05C_5C56C(pointersIndex[0]);
				if (!CurrentPlayer()->exitGame_0x04_4)
					LoadingScreen_34460_34820();
				LoadGameData_34B00_34EC0();
				FreeFileGroup_61610_61B20(soundFiles_9AE84_9AE84);
				FreeFileGroup_61610_61B20(musicFiles_9AE00_9AE00);
				GrowHeapAfterMenu_41540_41880();
				if (!CurrentPlayer()->exitGame_0x04_4)
				{
					LoadSoundBank_5D070_5D580(0);
					LoadMusicBank_5CEF0_5D400(0);
				}
				if (!(mc1Settings_AE408_AE3F8->flags_0x00_0 & 4))
				{
					SetFullScreen_31A00_31A40();
					if (screenMode == 8)
						sub_35040();
					RestoreScreenSize_31A60_31AA0();
				}
				SetAllocationTag_42530_42870(3);
			}
			if (!CurrentPlayer()->exitGame_0x04_4)
			{
				seed = mc1Settings_AE408_AE3F8->levelIndex_0x11_17;
				LoadLevel_407A0_40AE0();
				if (*videoMode_12F02E_12F01E == 1 && !blurBuffer_AE404_AE3F4)
					blurBuffer_AE404_AE3F4 = (uint8_t*)Malloc_42540_42880(64000);
			}
			while (!CurrentPlayer()->exitGame_0x04_4)
			{
				if (byte_939CC_939CC && byte_939CD_939CD && musicCount_939D6_939D6)
				{
					seed = seed * 0x24A1 + 0x24DF;
					// movsx + unsigned div: the 16-bit seed is sign extended before the division
					mc1State_AE400_AE3F0->levelVariant_0x240_576 = (int32_t)((uint32_t)(int32_t)(int16_t)seed % 3 + 1);
					PlayMusic_5D290_5D7A0((int16_t)mc1State_AE400_AE3F0->levelVariant_0x240_576);
				}
				sub_59720_59C30();
				GameLoop_34610_349D0();
				StopAllSamples_5D010_5D520();
				StopMusic_20E60_20E60();
				FadeInOut_61CC0_621D0(nullptr, 0x10, false, 10);
				Type_Mc1Player_340B* player = CurrentPlayer();
				if ((player->levelResult_0x02_2 & 6) != 4)
				{
					if (player->levelResult_0x02_2 & 2)
					{
						player->levelResult_0x02_2 = 2;
						sub_448E0_44C20();
						mc1Settings_AE408_AE3F8->levelIndex_0x11_17++;
					}
					else
						player->levelResult_0x02_2 = 8;
					break;
				}
				sub_408D0_40C10();
				CurrentPlayer()->levelResult_0x02_2 = 4;
			}
			FadeInOut_61CC0_621D0(nullptr, 0x10, false, 10);
		}
		ShutdownSound_358A0_35C60();
	}
	FadeInOut_61CC0_621D0(nullptr, 0x10, false, 10);
	Cleanup_34C60_35020();
	sub_433C0_43700();
	return 0;
}

void SkipMissingLevel_34070()
{
	uint16_t level = mc1Settings_AE408_AE3F8->levelIndex_0x11_17;
	if (level == 8 || level == 0x11 || level == 0x1C || level == 0x21 || level == 0x27)
		mc1Settings_AE408_AE3F8->levelIndex_0x11_17++;
}

static void CopyArgument(char* target, const char* source)
{
	// the original copies into 32 byte buffers without a length check
	snprintf(target, 32, "%s", source);
}

int16_t ProcessCommandLine_34DD0_35190(uint16_t argc, char** argv)
{
	char option[32];
	char value[32];
	char playerName[32]; // read but never used
	char sessionName[32];
	int8_t result = 0;
	uint8_t digitalJoystick = 0;
	uint8_t analogJoystick = 0;
	uint8_t analogJoystick2 = 0; // never set
	int32_t help = 0;
	int32_t custom = 0;
	int32_t network = 0;
	int32_t setup = 0; // never set
	int32_t videoMode = 1;
	int32_t setSound = 0; // read but never used
	int32_t detail = 1;
	int32_t cheat = 0;
	int32_t level = 0;
	int32_t movie = 0;
	int32_t roll = 0;
	int32_t time = 0;
	int32_t password = 0;
	int32_t vfx = 0;
	int32_t demo = 0;
	int32_t players = 2;

	debugOption_AE45A_AE44A = 0;
	CopyArgument(playerName, "PLAYER");
	CopyArgument(sessionName, "CARPET");
	for (int32_t i = 1; i < argc; i++)
	{
		if (argv[i][0] != '-' && argv[i][0] != '/')
		{
			printf("ERROR : Incorrect command : %d\n", i);
			result = -1;
			break;
		}
		CopyArgument(option, argv[i] + 1);
		value[0] = 0;
		if (i + 1 < argc)
			CopyArgument(value, argv[i + 1]);

		if (!_stricmp("digijoy", option))
			digitalJoystick = 1;
		else if (!_stricmp("anojoy", option) || !_stricmp("anojoy4", option))
			analogJoystick = 1;
		else if (!_stricmp("vfx", option))
			vfx = 1;
		else if (!_stricmp("help", option))
			help = 1;
		else if (!_stricmp("debug", option))
			debugOption_AE45A_AE44A = 1;
		else if (!_stricmp("network", option))
			network = 1;
		else if (!_stricmp("custom", option))
			custom = 1;
		else if (!_stricmp("setsound", option))
			setSound = 1;
		else if (!_stricmp("demo", option))
		{
			demo = atoi(value);
			i++;
		}
		else if (!_stricmp("detail", option))
		{
			detail = atoi(value);
			i++;
		}
		else if (!_stricmp("cheat", option))
		{
			cheat = atoi(value);
			i++;
		}
		else if (!_stricmp("name", option))
		{
			CopyArgument(playerName, value);
			i++;
		}
		else if (!_stricmp("level", option))
		{
			level = atoi(value);
			i++;
		}
		else if (!_stricmp("movie", option))
		{
			movie = atoi(value);
			i++;
		}
		else if (!_stricmp("roll", option))
		{
			roll = atoi(value);
			i++;
		}
		else if (!_stricmp("time", option))
			time = 1;
		else if (!_stricmp("password", option))
		{
			password = atoi(value);
			i++;
		}
		else if (!_stricmp("players", option))
		{
			players = atoi(value);
			i++;
		}
		else if (!_stricmp("session", option))
		{
			CopyArgument(sessionName, value);
			i++;
		}
		else
		{
			printf("ERROR : Incorrect command : %d\n", i);
			result = -1;
			break;
		}
	}
	if (result == -1)
		return result;

	if (network)
	{
		InitNetwork_43270_435B0();
		network = 0; // cleared here, so the session code below never runs
	}
	if (vfx && InitVfx_5AF50_5B460())
	{
		//!!!!! the original writes into the game state before it is allocated (null pointer), investigate later
		mc1State_AE400_AE3F0->vfx_0x219E_8606 = 1;
		inputDevices_90B23_90B23 |= 8;
		printf("VFX\n");
	}
	if (digitalJoystick)
	{
		InitDigitalJoystick_3F7D0_3FB10(nullptr); //!!!!! the original passes stack garbage as the sprite
		if (joystickFound_B7700_B76F0)
			inputDevices_90B23_90B23 |= 2;
	}
	if (analogJoystick)
	{
		// the shared command line calls the game's own joystick setup
		if (mc1Variant == MC1_VARIANT_HW)
			InitAnalogJoystick_3FB60(nullptr, nullptr, nullptr, 0); //!!!!! stack garbage in the original
		else
			InitAnalogJoystick_3F820(nullptr, nullptr, nullptr, 0); //!!!!! stack garbage in the original
		if (joystickFound_B7700_B76F0)
		{
			joystickFound_B7700_B76F0 = 1;
			inputDevices_90B23_90B23 |= 1;
		}
	}
	if (analogJoystick2)
	{
		if (mc1Variant == MC1_VARIANT_HW)
			InitAnalogJoystick_3FB60(nullptr, nullptr, nullptr, 0); //!!!!! stack garbage in the original
		else
			InitAnalogJoystick_3F820(nullptr, nullptr, nullptr, 0); //!!!!! stack garbage in the original
		if (joystickFound_B7700_B76F0)
		{
			joystickFound_B7700_B76F0 = 1;
			inputDevices_90B23_90B23 |= 0x20;
		}
	}

	vesaInfo_12F080_12F070 = (uint8_t*)LowMalloc_425C0_42900(0x100);
	mc1State_AE400_AE3F0 = (Type_Mc1State_AE400_AE3F0*)Malloc_42540_42880(sizeof(Type_Mc1State_AE400_AE3F0));
	mc1Settings_AE408_AE3F8 = (Type_Mc1Settings_AE408_AE3F8*)Malloc_42540_42880(sizeof(Type_Mc1Settings_AE408_AE3F8));
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	Type_Mc1Settings_AE408_AE3F8* settings = mc1Settings_AE408_AE3F8;
	for (uint8_t& option_ : state->options_0x21AD_8621)
		option_ = 1;
	DetectCpu_19470_19470();
	state->reflections_0x2195_8597 = settings->fastCpu_0x08_8 ? 1 : 0;
	state->sky_0x2197_8599 = settings->fastCpu_0x08_8 ? 1 : 0;
	state->shadows_0x2196_8598 = 1;
	state->icons_0x219A_8602 = 1;
	state->map_0x2199_8601 = 1;
	state->setting_0x2198_8600 = 40;
	state->playerCount_0x0A_10 = (uint16_t)players;
	strcpy(settings->sessionName_0x75_117, sessionName);
	settings->playerName_0x1D_29[0] = byte_A9058_A9058;
	if (!detail)
	{
		state->reflections_0x2195_8597 = 0;
		state->sky_0x2197_8599 = 0;
		state->shadows_0x2196_8598 = 0;
		state->setting_0x2198_8600 = 40;
	}
	if (demo)
	{
		if (demo >= 1 && demo <= 5)
			settings->flags_0x00_0 |= 0x200 << (demo - 1);
		settings->flags_0x00_0 |= 0x100;
	}
	if (time)
		settings->flags_0x00_0 |= 0x40;
	if (custom)
		settings->flags_0x00_0 |= 0x100;
	if (help)
		result = -1;
	if (network)
	{
		if (state->playerCount_0x0A_10 > 1)
		{
			state->currentPlayer_0x08_8 = JoinNetworkSession_43430_43770(settings->sessionName_0x75_117, state->playerCount_0x0A_10);
			if (state->currentPlayer_0x08_8 >= 0)
				settings->flags_0x00_0 |= 0x10;
			else
				state->currentPlayer_0x08_8 = 0;
		}
		settings->flags_0x00_0 |= 0x100;
	}
	if (setup)
	{
		videoMode = 8;
		setSound = 0;
		settings->flags_0x00_0 |= 8;
	}
	state->players_0x340B_13323[state->currentPlayer_0x08_8].cheat_0x18_24 = cheat;
	settings->levelIndex_0x11_17 = (uint16_t)level;
	if (movie)
		settings->movie_0x0D_13 = (uint16_t)movie;
	if (roll)
	{
		settings->movie_0x0D_13 = 0;
		settings->roll_0x0F_15 = (uint16_t)roll;
		settings->flags_0x00_0 |= 0x120;
	}
	if (debugOption_AE45A_AE44A)
	{
		debugOption_AE45A_AE44A = 0;
		settings->flags_0x00_0 |= 0x80;
	}
	settings->password_0x19_25 = password;
	if (result != -1)
		*videoMode_12F02E_12F01E = (int16_t)videoMode;
	(void)setSound;
	return result;
}

void Cleanup_34C60_35020()
{
	sub_58F00_59410();
	ShutdownGraphics_44840_44B80();
	FreeFileGroup_61610_61B20(searchDFiles_99318_99318);
	PrintStartTexts_5ACA0_5B1B0();
}

void PrintStartTexts_5ACA0_5B1B0()
{
	printf("Copyright (c) 1995 Bullfrog Productions Ltd.\n");
	printf("All rights reserved.\n");
	printf("%s \n", "Magic Carpet");
}

void PrintLoadingText_5AD10_5B220(const char* text)
{
	// the format is the address of off_ABCE0 read as a string: its bytes are "%s\n"
	if (mc1Settings_AE408_AE3F8->flags_0x00_0 & 0x100)
		printf("%s\n", text);
}

void LoadGameData_34B00_34EC0()
{
	LoadFileGroup_40440_40780(searchDFiles_99318_99318);
	if (*videoMode_12F02E_12F01E == 1)
		LoadFileGroup_40440_40780(screenFilesMcga_997BC_997BC);
	else
		LoadFileGroup_40440_40780(screenFilesHires_99898_99898);
	RelocateTabs_59500_59A10(tabRelocations_99974_99974);
	InitSearchRings_11540_11540();
}

void InitGameData_34B40_34F00()
{
	PrintStartTexts_5ACA0_5B1B0();
	PrintLoadingText_5AD10_5B220("Load all data files");
	LoadGameData_34B00_34EC0();
	PrintLoadingText_5AD10_5B220("Initialise Colour Lookup");
	// 4 bit components, the middle of each step (r * 4 + 3), indexed r * 256 + g * 16 + b
	for (int32_t red = 0; red < 16; red++)
		for (int32_t green = 0; green < 16; green++)
			for (int32_t blue = 0; blue < 16; blue++)
				colorLookup_AD168_AD158[red * 256 + green * 16 + blue] = FindNearestColor_5CC70_5D180(
					palette_AE428_AE418, (uint8_t)(red * 4 + 3), (uint8_t)(green * 4 + 3), (uint8_t)(blue * 4 + 3));
	mc1Settings_AE408_AE3F8->flag_0x17_23 = 1;
	SetFont_5A3C0_5A8D0(0);
}
