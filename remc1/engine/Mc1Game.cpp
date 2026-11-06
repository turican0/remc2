#include "Mc1Game.h"

#include "Mc1NotPorted.h"

Mc1Variant mc1Variant = MC1_VARIANT_MC1;
Type_Mc1State_AE400_AE3F0* mc1State_AE400_AE3F0 = nullptr;
Type_Mc1Settings_AE408_AE3F8* mc1Settings_AE408_AE3F8 = nullptr;
uint8_t* blurBuffer_AE404_AE3F4 = nullptr;
uint8_t* textData_AE41C_AE40C = nullptr;
uint8_t* textPointers_AE238_AE228[80];
uint8_t* pointersTable_AE450_AE440 = nullptr;
uint8_t byte_939CC_939CC = 0;
uint8_t byte_939CD_939CD = 0;
uint8_t byte_939E4_939E4 = 0;
uint8_t byte_939E5_939E5 = 0;
uint16_t word_939D6_939D6 = 0;
uint8_t menuDone_96880_96880 = 0;
uint8_t byte_12CA7C_12CA6C = 0;
int16_t videoMode_12F02E_12F01E = 0;

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
	sub_34B40_34F00();
	if (!(mc1Settings_AE408_AE3F8->flags_0x00_0 & 8))
	{
		mc1Settings_AE408_AE3F8->flag_0x17_23 = 0;
		sub_3F1D0();
		sub_58F90_594A0();
		sub_41230();
		sub_44730_44A70();
		if (mc1Settings_AE408_AE3F8->flags_0x00_0 & 0x100)
		{
			byte_939CD_939CD = 0;
			byte_939E5_939E5 = 0;
			byte_939CC_939CC = 0;
			byte_939E4_939E4 = 0;
			sub_40440_40780("data/ftext.dat");
			sub_44700_44A40(textData_AE41C_AE40C, textPointers_AE238_AE228, 80);
		}
		else
			sub_4A980_4ACC0();

		CurrentPlayer()->exitGame_0x04_4 = 0;
		int16_t screenMode = 1;
		uint32_t seed = 0;
		while (!CurrentPlayer()->exitGame_0x04_4)
		{
			FadeInOut_61CC0_621D0(nullptr, 0x10, 0);
			if (!(mc1Settings_AE408_AE3F8->flags_0x00_0 & 0x100))
			{
				sub_31A00_31A40();
				screenMode = 1;
				if (videoMode_12F02E_12F01E == 8)
				{
					sub_34C80();
					screenMode = 8;
				}
				sub_41490_417D0();
				sub_42530_42870(2);
				byte_939E5_939E5 = byte_939E4_939E4;
				byte_12CA7C_12CA6C = 1;
				byte_939CD_939CD = byte_939CC_939CC;
				menuDone_96880_96880 = 0;
				do
					sub_4AB20();
				while (!menuDone_96880_96880);
				SkipMissingLevel_34070();
				FadeInOut_61CC0_621D0(nullptr, 0x10, 0);
				sub_5C05C_5C56C(pointersTable_AE450_AE440);
				if (!CurrentPlayer()->exitGame_0x04_4)
					sub_34460_34820();
				sub_34B00_34EC0();
				sub_61610_61B20("data/snds0-0.dat");
				sub_61610_61B20("data/music0-0.dat");
				sub_41540_41880();
				if (!CurrentPlayer()->exitGame_0x04_4)
				{
					sub_5D070_5D580(0);
					sub_5CEF0_5D400(0);
				}
				if (!(mc1Settings_AE408_AE3F8->flags_0x00_0 & 4))
				{
					sub_31A00_31A40();
					if (screenMode == 8)
						sub_34C80();
					sub_31A60_31AA0();
				}
				sub_42530_42870(3);
			}
			if (!CurrentPlayer()->exitGame_0x04_4)
			{
				seed = mc1Settings_AE408_AE3F8->levelIndex_0x11_17;
				sub_407A0_40AE0();
				if (videoMode_12F02E_12F01E == 1 && !blurBuffer_AE404_AE3F4)
					blurBuffer_AE404_AE3F4 = (uint8_t*)Malloc_42540_42880(64000);
			}
			while (!CurrentPlayer()->exitGame_0x04_4)
			{
				if (byte_939CC_939CC && byte_939CD_939CD && word_939D6_939D6)
				{
					seed = seed * 0x24A1 + 0x24DF;
					// movsx + unsigned div: the 16-bit seed is sign extended before the division
					mc1State_AE400_AE3F0->levelVariant_0x240_576 = (int32_t)((uint32_t)(int32_t)(int16_t)seed % 3 + 1);
					sub_5D290_5D7A0((int16_t)mc1State_AE400_AE3F0->levelVariant_0x240_576);
				}
				sub_59720_59C30();
				GameLoop_34610_349D0();
				sub_5D010_5D520();
				sub_20E60_20E60();
				FadeInOut_61CC0_621D0(nullptr, 0x10, 0);
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
			FadeInOut_61CC0_621D0(nullptr, 0x10, 0);
		}
		sub_358A0_35C60();
	}
	// the original calls an empty function (nullsub_2) when flag 8 is set
	FadeInOut_61CC0_621D0(nullptr, 0x10, 0);
	sub_34C60_35020();
	sub_433C0_43700();
	return 0;
}

int TopProcedure_34470(int argc, char** argv)
{
	if (ProcessCommandLine_34DD0_35190((uint16_t)argc, argv) == -1)
		return -1;
	sub_34B40_34F00();
	if (!(mc1Settings_AE408_AE3F8->flags_0x00_0 & 8))
	{
		mc1Settings_AE408_AE3F8->flag_0x17_23 = 0;
		sub_3F510();
		sub_58F90_594A0();
		sub_41570();
		sub_44730_44A70();
		if (mc1Settings_AE408_AE3F8->flags_0x00_0 & 0x100)
		{
			byte_939CD_939CD = 0;
			byte_939E5_939E5 = 0;
			byte_939CC_939CC = 0;
			byte_939E4_939E4 = 0;
			sub_40440_40780("data/ftext.dat");
			sub_44700_44A40(textData_AE41C_AE40C, textPointers_AE238_AE228, 80);
		}
		else
			sub_4A980_4ACC0();

		CurrentPlayer()->exitGame_0x04_4 = 0;
		int16_t screenMode = 1;
		uint32_t seed = 0;
		while (!CurrentPlayer()->exitGame_0x04_4)
		{
			FadeInOut_61CC0_621D0(nullptr, 0x10, 0);
			if (!(mc1Settings_AE408_AE3F8->flags_0x00_0 & 0x100))
			{
				sub_31A00_31A40();
				screenMode = 1;
				if (videoMode_12F02E_12F01E == 8)
				{
					sub_35040();
					screenMode = 8;
				}
				sub_41490_417D0();
				sub_42530_42870(2);
				byte_939E5_939E5 = byte_939E4_939E4;
				byte_12CA7C_12CA6C = 1;
				byte_939CD_939CD = byte_939CC_939CC;
				menuDone_96880_96880 = 0;
				do
					sub_4AE60();
				while (!menuDone_96880_96880);
				// Hidden Worlds has all levels, no SkipMissingLevel here
				FadeInOut_61CC0_621D0(nullptr, 0x10, 0);
				sub_5C05C_5C56C(pointersTable_AE450_AE440);
				if (!CurrentPlayer()->exitGame_0x04_4)
					sub_34460_34820();
				sub_34B00_34EC0();
				sub_61610_61B20("data/snds0-0.dat");
				sub_61610_61B20("data/music0-0.dat");
				sub_41540_41880();
				if (!CurrentPlayer()->exitGame_0x04_4)
				{
					sub_5D070_5D580(0);
					sub_5CEF0_5D400(0);
				}
				if (!(mc1Settings_AE408_AE3F8->flags_0x00_0 & 4))
				{
					sub_31A00_31A40();
					if (screenMode == 8)
						sub_35040();
					sub_31A60_31AA0();
				}
				sub_42530_42870(3);
			}
			if (!CurrentPlayer()->exitGame_0x04_4)
			{
				seed = mc1Settings_AE408_AE3F8->levelIndex_0x11_17;
				sub_407A0_40AE0();
				if (videoMode_12F02E_12F01E == 1 && !blurBuffer_AE404_AE3F4)
					blurBuffer_AE404_AE3F4 = (uint8_t*)Malloc_42540_42880(64000);
			}
			while (!CurrentPlayer()->exitGame_0x04_4)
			{
				if (byte_939CC_939CC && byte_939CD_939CD && word_939D6_939D6)
				{
					seed = seed * 0x24A1 + 0x24DF;
					// movsx + unsigned div: the 16-bit seed is sign extended before the division
					mc1State_AE400_AE3F0->levelVariant_0x240_576 = (int32_t)((uint32_t)(int32_t)(int16_t)seed % 3 + 1);
					sub_5D290_5D7A0((int16_t)mc1State_AE400_AE3F0->levelVariant_0x240_576);
				}
				sub_59720_59C30();
				GameLoop_34610_349D0();
				sub_5D010_5D520();
				sub_20E60_20E60();
				FadeInOut_61CC0_621D0(nullptr, 0x10, 0);
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
			FadeInOut_61CC0_621D0(nullptr, 0x10, 0);
		}
		sub_358A0_35C60();
	}
	FadeInOut_61CC0_621D0(nullptr, 0x10, 0);
	sub_34C60_35020();
	sub_433C0_43700();
	return 0;
}

void SkipMissingLevel_34070()
{
	uint16_t level = mc1Settings_AE408_AE3F8->levelIndex_0x11_17;
	if (level == 8 || level == 0x11 || level == 0x1C || level == 0x21 || level == 0x27)
		mc1Settings_AE408_AE3F8->levelIndex_0x11_17++;
}
