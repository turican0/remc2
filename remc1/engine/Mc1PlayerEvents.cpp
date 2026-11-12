#include "Mc1PlayerEvents.h"

#include <cstdio>
#include <cstring>

#include "Mc1Entity.h"
#include "Mc1EntityTables.h"
#include "Mc1Files.h"
#include "Mc1Game.h"
#include "Mc1Level.h"
#include "Mc1MainMenu.h"
#include "Mc1Movie.h"
#include "Mc1NotPorted.h"
#include "Mc1Screen.h"
#include "Mc1Sound.h"
#include "Mc1Terrain.h"
#include "Mc1World.h"
#include "../../remc2/engine/read_config.h"

uint8_t networkJoined_93959_93959 = 0;
int16_t networkIndex_9395A_9395A = 0;
int16_t networkCount_9395C_9395C = 0;
uint16_t networkMaster_9398A_9398A = 0;
uint32_t networkHandles_93990_93990[8];
uint8_t networkActive_939B0_939B0[8];

static Type_Mc1Entity* Entities()
{
	return mc1State_AE400_AE3F0->entities_0x7463_29795;
}

static uint16_t EntityIndex(const Type_Mc1Entity* entity)
{
	return (uint16_t)(entity - Entities());
}

static int16_t ReadS16(const uint8_t* data)
{
	int16_t value;
	memcpy(&value, data, sizeof(value));
	return value;
}

static void WriteS16(uint8_t* data, int16_t value)
{
	memcpy(data, &value, sizeof(value));
}

static int32_t ReadS32(const uint8_t* data)
{
	int32_t value;
	memcpy(&value, data, sizeof(value));
	return value;
}

static void WriteS32(uint8_t* data, int32_t value)
{
	memcpy(data, &value, sizeof(value));
}

static Type_Mc1Player_340B& CurrentPlayer()
{
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	return state->players_0x340B_13323[state->currentPlayer_0x08_8];
}

static void SetMessage(Type_Mc1Message& message, int16_t time, int16_t kind)
{
	message.time_0x40_64 = time;
	message.kind_0x42_66 = kind;
}

// 3D740...: the message of a cheat to the current player
static void CheatMessage(uint16_t player, const char* text)
{
	if (player != (uint16_t)mc1State_AE400_AE3F0->currentPlayer_0x08_8)
		return;
	Type_Mc1Message& message = CurrentPlayer().messages_0x1C_28[player];
	SetMessage(message, 100, 0);
	strcpy(message.text_0x00_0, text);
}

// 99B68: the names point into longer strings of DOS (the compiler shares the ends), 3C9D0 copies 12 bytes of them
static const char playerNameBytes_99B68_99B68[2][8][12] = {
	{ { 'Z', 'a', 'n', 'z', 'a', 'm', 'a', 'r', 0, 'r', 'a', 'h' }, { 'V', 'o', 'd', 'o', 'r', 0, 's', 'n', 'G', 'r', 'y', 's' },
		{ 'G', 'r', 'y', 's', 'h', 'n', 'a', 'k', 0, 'a', 'r', 't' }, { 'M', 'a', 'h', 'm', 'o', 'u', 'd', 0, 'S', 'y', 'e', 'd' },
		{ 'S', 'y', 'e', 'd', 0, 'y', 'a', 'h', 'R', 'a', 's', 'c' }, { 'R', 'a', 's', 'c', 'h', 'i', 'd', 0, 'A', 'l', 'h', 'a' },
		{ 'A', 'l', 'h', 'a', 'b', 'b', 'a', 'l', 0, 'e', 'n', 0 }, { 'S', 'c', 'h', 'e', 'h', 'e', 'r', 'a', 'z', 'a', 'd', 'e' } },
	{ { 'Z', 'a', 'n', 'z', 'a', 'm', 'a', 'r', 0, 'i', 'u', 's' }, { 'V', 'o', 'd', 'o', 'r', 0, 0, 0, 'G', 'r', 'y', 's' },
		{ 'G', 'r', 'y', 's', 'h', 'n', 'a', 'k', 0, 'i', 'r', 0 }, { 'M', 'a', 'h', 'm', 'o', 'u', 'd', 0, 'S', 'y', 'e', 'd' },
		{ 'S', 'y', 'e', 'd', 0, 'e', 'r', 'v', 'R', 'a', 's', 'c' }, { 'R', 'a', 's', 'c', 'h', 'i', 'd', 0, 'A', 'l', 'h', 'a' },
		{ 'A', 'l', 'h', 'a', 'b', 'b', 'a', 'l', 0, 'o', 'u', 'p' }, { 'S', 'c', 'h', 'e', 'h', 'e', 'r', 'a', 'z', 'a', 'd', 'e' } } };

static void CopyPlayerName(Type_Mc1Player_340B& player, uint16_t index)
{
	Type_Mc1Settings_AE408_AE3F8* settings = mc1Settings_AE408_AE3F8;
	const char* name = settings->playerName_0x1D_29[0] ? settings->nickname_0x3D_61 : playerNameBytes_99B68_99B68[mc1Variant == MC1_VARIANT_HW][index];
	memcpy(player.name_0x40A_1034, name, 12);
	player.name_0x40A_1034[12] = 0;
}

// the cheat 1: all the spells of class 12 the wizard has not got yet, into the free slots
static void GiveAllSpells(Type_Mc1Entity* wizard)
{
	uint8_t* playerData = wizard->playerData_0xA0_160;
	for (int spell = 0; spell < 24; spell++)
	{
		if (ReadS16(playerData + 0x2A4 + spell * 2))
			continue;
		for (int slot = 0; slot < 24; slot++)
		{
			if (ReadS32(playerData + 0x214 + slot * 4))
				continue;
			Type_Mc1Entity* entity = classes_96902_96902[12].models_4[spell].create_6(&wizard->position_0x48_72);
			if (!entity)
				continue;
			entity->flags_0x10_16 |= 0x40001;
			entity->word_0x2A_42 = EntityIndex(wizard);
			entity->dword_0x84_132 = 0;
			WriteS32(playerData + 0x214 + slot * 4, EntityIndex(entity));
			for (int i = 0; i < 10; i++)
				if ((int8_t)playerData[0x304 + i] == -1)
				{
					playerData[0x304 + i] = (uint8_t)slot;
					break;
				}
			break;
		}
	}
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	for (int i = 1; i < 1000; i++)
		if (state->entities_0x7463_29795[i].class_0x40_64 == 0x0B)
			state->entities_0x7463_29795[i].flags_0x10_16 &= ~1u;
}

// the cheats 3-5: the wizards of the others by the model (0 / 1 players, 2 castles, 3 balloons) die
static void DestroyWizardsByModel(const Type_Mc1Entity* wizard, uint8_t model, uint8_t model2)
{
	for (Type_Mc1Entity* entity = mc1Settings_AE408_AE3F8->entityLists_0x8E1A_36378[21]; entity > Entities(); entity = entity->next_0x00_0)
		if (entity->id_0x18_24 != wizard->id_0x18_24 && (entity->model_0x41_65 == model || entity->model_0x41_65 == model2))
			entity->life_0x0C_12 = -1;
}

void PlayerEvents_3C9D0_3CD10()
{
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	Type_Mc1Settings_AE408_AE3F8* settings = mc1Settings_AE408_AE3F8;
	uint8_t nextMovie = 0;
	if (settings->flags_0x00_0 & 0x10)
	{
		// the network: the commands of all, then the players who joined
		settings->networkTicks_0x9D_157 = timerTicks_AC5D4_AC5C4;
		ExchangeAll_43930_43C70((uint8_t*)state->playerInputs_0x7413_29715, sizeof(Type_Mc1PlayerInput));
		settings->networkTicks_0x9D_157 = timerTicks_AC5D4_AC5C4 - settings->networkTicks_0x9D_157;
		bool joined = false;
		for (uint16_t i = 0; i < state->playerCount_0x0A_10; i++)
		{
			if (state->playerInputs_0x7413_29715[i].command_0x00_0 != 1)
				continue;
			Type_Mc1Player_340B& player = state->players_0x340B_13323[i];
			player.active_0x06_6 = 1;
			if (i == (uint16_t)state->currentPlayer_0x08_8)
				CopyPlayerName(player, i);
			joined = true;
		}
		if (joined)
			ExchangeAll_43930_43C70((uint8_t*)state->players_0x340B_13323, sizeof(Type_Mc1Player_340B));
	}
	CurrentPlayer().turn_0x12_18++;
	settings->frames_0x04_4++;
	for (int16_t i = 1; i < 16; i++)
		settings->colorIndex_0x5D_93[i] = (uint8_t)((uint32_t)CurrentPlayer().turn_0x12_18 / (uint32_t)i & 1);

	for (uint16_t i = 0; i < state->playerCount_0x0A_10; i++)
	{
		Type_Mc1Player_340B* player = &state->players_0x340B_13323[i];
		Type_Mc1PlayerInput* input = &state->playerInputs_0x7413_29715[i];
		Type_Mc1Entity* wizard = &Entities()[player->entityIndex_0x0A_10];
		uint8_t* playerData = wizard->playerData_0xA0_160;
		const bool current = i == (uint16_t)state->currentPlayer_0x08_8;
		RecordOrPlayMovieInput_3E9D0_3ED10(input);
		player->byte_0x44B_1099 = 0;
		switch (input->command_0x00_0)
		{
		case 1: // join
			player->active_0x06_6 = 1;
			if (current)
				CopyPlayerName(*player, i);
			CreatePlayerWizard_44D30_45070(player, wizard);
			SetPlayerMode_3DC90_3DFD0(player, 0);
			break;
		case 2: // leave
			CheckCd_3DD10_3E050();
			if (current)
				player->exitGame_0x04_4 = 1;
			wizard->life_0x0C_12 = -1;
			RemoveNetworkPlayer_43760_43AA0((int16_t)i);
			player->active_0x06_6 = 0;
			break;
		case 3: // again
			CreatePlayerWizard_44D30_45070(player, wizard);
			SetPlayerMode_3DC90_3DFD0(player, 0);
			break;
		case 4:
			player->pause_0x05_5 ^= (uint8_t)input->param_0x01_1;
			break;
		case 7: // the camera record
		{
			const int record = player->word_0x0E_14 + input->param_0x01_1;
			if (record >= 0 && record < player->recordCount_0x10_16 - 1)
				player->word_0x0E_14 += input->param_0x01_1;
			break;
		}
		case 8: // the field of view
			player->recordTemplate_0x23C_572.word_0x0C_12 += input->param_0x01_1;
			break;
		case 10:
			if (!(settings->flags_0x00_0 & 0x10))
				sub_3E410_3E750(10000);
			break;
		case 11:
			if (!(settings->flags_0x00_0 & 0x10))
				sub_3E350_3E690(10000);
			break;
		case 12: // start recording the movie
			if (!(settings->flags_0x00_0 & 0x20) && current)
			{
				settings->entityLists_0x8E1A_36378[0] = nullptr;
				settings->flags_0x00_0 |= 2;
			}
			break;
		case 14: // the next movie
			nextMovie = 1;
			break;
		case 15:
			if (!ReadS16(playerData + 0x32))
			{
				if (settings->flags_0x00_0 & 0x10)
					goto case29;
				player->levelResult_0x02_2 |= 0x0C;
			}
			CreatePlayerWizard_44D30_45070(player, wizard);
			SetPlayerMode_3DC90_3DFD0(player, 0);
			break;
		case 16: // a message: typing starts
		{
			memset(&CurrentPlayer().messages_0x1C_28[i], 0, sizeof(Type_Mc1Message));
			SetPlayerMode_3DC90_3DFD0(player, 3);
			SetMessage(player->messages_0x1C_28[i], 0x7D00, 1);
			break;
		}
		case 17: // a message: a character
		{
			char* text = CurrentPlayer().messages_0x1C_28[i].text_0x00_0;
			const size_t length = strlen(text);
			if (input->param_0x01_1 == 8)
			{
				if (length)
					text[length - 1] = 0;
			}
			else if (input->param_0x01_1 && length < 0x3F)
			{
				char character[0x100];
				sprintf(character, "%c", input->param_0x01_1);
				strcat(text, character);
			}
			SetMessage(player->messages_0x1C_28[i], 0x7D00, 1);
			break;
		}
		case 18: // a message: cancelled
			memset(&CurrentPlayer().messages_0x1C_28[i], 0, sizeof(Type_Mc1Message));
			SetMessage(CurrentPlayer().messages_0x1C_28[i], 0, 0);
			SetPlayerMode_3DC90_3DFD0(player, 0);
			break;
		case 19: // a message: sent ("QUICK" is the cheat 8000h)
		{
			Type_Mc1Message& message = CurrentPlayer().messages_0x1C_28[i];
			const char* text = message.text_0x00_0;
			if ((text[0] == 'Q' || text[0] == 'q') && (text[1] == 'U' || text[1] == 'u') && (text[2] == 'I' || text[2] == 'i')
				&& (text[3] == 'C' || text[3] == 'c') && (text[4] == 'K' || text[4] == 'k'))
			{
				SetMessage(message, 0, 0);
				if (current)
					settings->flags_0x00_0 |= 0x8000;
			}
			else
				SetMessage(message, 200, 3);
			SetPlayerMode_3DC90_3DFD0(player, 0);
			break;
		}
		case 20:
			SetPlayerMode_3DC90_3DFD0(player, (uint8_t)input->param_0x01_1);
			break;
		case 21: // the first spell
		case 22: // the second spell
			if (input->param_0x01_1 == -1)
				break;
			WriteS16(playerData + (input->command_0x00_0 == 21 ? 0x3AC : 0x3B0), input->param_0x01_1);
			sub_55370_558A0(0, player->index_0x07_7, 0x0E);
			SetPlayerMode_3DC90_3DFD0(player, 0);
			break;
		case 23: // a spell into a slot
			if (input->param2_0x02_2 == -1)
				break;
			for (int slot = 0; slot < 24; slot++)
				if (playerData[0x304 + slot] == (uint8_t)input->param2_0x02_2)
					playerData[0x304 + slot] = 0xFF;
			playerData[0x304 + input->param_0x01_1] = (uint8_t)input->param2_0x02_2;
			sub_55370_558A0(0, player->index_0x07_7, 0x0E);
			break;
		case 24: // the first spell from a slot
		case 25: // the second spell from a slot
		{
			CheckCd_3DD10_3E050();
			const int8_t slot = (int8_t)playerData[0x304 + input->param_0x01_1];
			if (slot != -1)
			{
				WriteS16(playerData + (input->command_0x00_0 == 24 ? 0x3AC : 0x3B0), slot);
				const Type_Mc1Entity& spell = Entities()[ReadS32(playerData + 0x214 + slot * 4)];
				playerData[0x34C + (int8_t)spell.model_0x41_65] = 0x20;
			}
			sub_55370_558A0(0, player->index_0x07_7, 0x0E);
			break;
		}
		case 26: // all leave
			for (uint16_t j = 0; j < state->playerCount_0x0A_10; j++)
				if (state->players_0x340B_13323[j].active_0x06_6)
				{
					state->players_0x340B_13323[j].exitGame_0x04_4 = 1;
					RemoveNetworkPlayer_43760_43AA0((int16_t)j);
				}
			break;
		case 27:
			CheckCd_3DD10_3E050();
			player->levelResult_0x02_2 = 0x0A;
			RemoveNetworkPlayer_43760_43AA0((int16_t)i);
			break;
		case 28:
			player->levelResult_0x02_2 = 0x0C;
			RemoveNetworkPlayer_43760_43AA0((int16_t)i);
			break;
		case 29: // the player is dead in the network game
		case29:
		{
			Type_Mc1Message& message = CurrentPlayer().messages_0x1C_28[ReadS16(playerData + 0x30)];
			strcpy(message.text_0x00_0, (const char*)textPointers_AE238_AE228[62]);
			SetMessage(message, 100, 0);
			player->levelResult_0x02_2 = 8;
			RemoveNetworkPlayer_43760_43AA0((int16_t)i);
			player->active_0x06_6 = 0;
			break;
		}
		case 30: // the cheats
			CheckCd_3DD10_3E050();
			if (wizard->actionIndex_0x46_70 || wizard->life_0x0C_12 <= 0)
				break;
			switch (input->param_0x01_1)
			{
			case 1:
				GiveAllSpells(wizard);
				CheatMessage(i, ".. CHEAT: access all spells");
				break;
			case 2:
			{
				Type_Mc1Entity* mana = CreateEntity_373F0_377B0(&wizard->position_0x48_72, 10, 0x27);
				if (mana)
				{
					mana->mana_0x8C_140 = 100000;
					mana->owner_0x90_144 = EntityIndex(wizard);
				}
				wizard->mana_0x8C_140 = wizard->dword_0x88_136;
				CheatMessage(i, ".. CHEAT: more mana");
				break;
			}
			case 3:
				DestroyWizardsByModel(wizard, 0, 1);
				CheatMessage(i, ".. CHEAT: destroy all players");
				break;
			case 4:
				DestroyWizardsByModel(wizard, 2, 2);
				CheatMessage(i, ".. CHEAT: destroy all castles");
				break;
			case 5:
				DestroyWizardsByModel(wizard, 3, 3);
				CheatMessage(i, ".. CHEAT: destroy all balloons");
				break;
			case 6:
				wizard->life_0x0C_12 = wizard->maxLife_0x08_8;
				CheatMessage(i, ".. CHEAT: heal");
				break;
			case 7:
				KillAllCreatures_194F0_194F0();
				CheatMessage(i, ".. CHEAT: Kill all creatures");
				break;
			}
			break;
		}

		// the movement and the buttons to the wizard of the player
		wizard = &Entities()[player->entityIndex_0x0A_10];
		playerData = wizard->playerData_0xA0_160;
		WriteS16(playerData + 4, (int16_t)((input->moveX_0x03_3 * 2 - ReadS16(playerData + 0x147)) / 4));
		WriteS16(playerData + 6, (int16_t)((input->moveY_0x04_4 * 2 - ReadS16(playerData + 0x149)) / 4));
		WriteS32(playerData, input->buttons_0x05_5);
		if (CurrentPlayer().exitGame_0x04_4 || nextMovie)
		{
			CloseMovie_3EC50_3EF90();
			if (nextMovie)
			{
				settings->flags_0x00_0 &= ~2u;
				settings->movie_0x0D_13++;
			}
		}
		SetPlayerCamera_41B20_41E60(player, wizard);
		memset(input, 0, sizeof(Type_Mc1PlayerInput));
	}
}

void SetPlayerMode_3DC90_3DFD0(Type_Mc1Player_340B* player, uint8_t mode)
{
	player->mode_0x44A_1098 = mode;
	if (player->index_0x07_7 == (uint16_t)mc1State_AE400_AE3F0->currentPlayer_0x08_8)
	{
		if (player->mode_0x44A_1098 == 2)
		{
			SetCursor_5C05C_5C56C(pointersIndex[1]);
			SetFullScreen_31A00_31A40();
		}
		else
		{
			SetCursor_5C05C_5C56C(pointersIndex[0]);
			SetCenterMouse_59720_59C30();
			RestoreScreenSize_31A60_31AA0();
		}
		sub_55370_558A0(0, player->index_0x07_7, 0x0E);
	}
	CheckCd_3DD10_3E050();
}

void CheckCd_3DD10_3E050()
{
	Type_Mc1Settings_AE408_AE3F8* settings = mc1Settings_AE408_AE3F8;
	if (!(settings->frames_0x04_4 & 7) && (settings->cd_0x18_24 & 3) == 1 && !IsCdPresent_11050_11050())
		CurrentPlayer().exitGame_0x04_4 = 2;
}

void RecordOrPlayMovieInput_3E9D0_3ED10(Type_Mc1PlayerInput* input)
{
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	Type_Mc1Settings_AE408_AE3F8* settings = mc1Settings_AE408_AE3F8;
	char path[0x40];
	if (settings->flags_0x00_0 & 4)
	{
		// playing: the movie opens at the command of player 0
		if (!settings->movieFile_0x09_9 && input == &state->playerInputs_0x7413_29715[0])
		{
			sprintf(path, "%s/mvi%05d.dat", "movie", settings->movie_0x0D_13);
			settings->movieFile_0x09_9 = DataFileIO::CreateOrOpenFile(path, 0x200);
			if (!settings->movieFile_0x09_9)
			{
				settings->flags_0x00_0 &= ~4u;
				return;
			}
			sub_3E350_3E690(settings->movie_0x0D_13);
			sub_3E4B0_3E7F0(settings->movie_0x0D_13);
			sub_59420_59930();
			InitInputChange_356E0_35AA0();
		}
		FILE* file = settings->movieFile_0x09_9;
		if (!file)
			return;
		if (input->command_0x00_0 != 2)
		{
			if (DataFileIO::Read(file, (uint8_t*)input, sizeof(Type_Mc1PlayerInput)) != sizeof(Type_Mc1PlayerInput))
			{
				CloseMovie_3EC50_3EF90();
				CurrentPlayer().levelResult_0x02_2 = 8;
				input->command_0x00_0 = 0;
			}
			// any input ends the movie
			if (input->command_0x00_0 != 2 && !InputChanged_35710_35AD0())
				return;
		}
		CloseMovie_3EC50_3EF90();
		CurrentPlayer().levelResult_0x02_2 = 8;
		input->command_0x00_0 = 0;
		return;
	}
	if (!(settings->flags_0x00_0 & 2))
		return;
	// recording
	if (!settings->movieFile_0x09_9 && input == &state->playerInputs_0x7413_29715[0])
	{
		sprintf(path, "%s/mvi%05d.dat", "movie", settings->movie_0x0D_13);
		settings->movieFile_0x09_9 = DataFileIO::CreateOrOpenFile(path, 0x222);
		if (!settings->movieFile_0x09_9)
		{
			settings->flags_0x00_0 &= ~2u;
			return;
		}
		sub_593B0_598C0();
		sub_3E410_3E750(settings->movie_0x0D_13);
		sub_3E580_3E8C0(settings->movie_0x0D_13);
		RebuildFreeLists_37220_375E0();
		state->reuseTop_0x11F1_4593 = -1;
		settings->unknown_0xA1_161 = 3;
		settings->unknown_0xA2_162 = 200;
	}
	FILE* file = settings->movieFile_0x09_9;
	if (!file)
		return;
	if (WriteFile_62ED0_633E0(file, (uint8_t*)input, sizeof(Type_Mc1PlayerInput)) != sizeof(Type_Mc1PlayerInput))
		CloseMovie_3EC50_3EF90();
	if (input->command_0x00_0 == 12)
		input->command_0x00_0 = 0;
}

void CloseMovie_3EC50_3EF90()
{
	Type_Mc1Settings_AE408_AE3F8* settings = mc1Settings_AE408_AE3F8;
	if (!settings->movieFile_0x09_9)
		return;
	DataFileIO::Close(settings->movieFile_0x09_9);
	settings->movieFile_0x09_9 = nullptr;
	settings->flags_0x00_0 &= ~6u;
}

void KillAllCreatures_194F0_194F0()
{
	// the lists 1-20: the creatures by the model
	for (int list = 1; list <= 20; list++)
		for (Type_Mc1Entity* entity = mc1Settings_AE408_AE3F8->entityLists_0x8E1A_36378[list]; entity > Entities(); entity = entity->next_0x00_0)
			entity->life_0x0C_12 = -1;
}

void RemoveNetworkPlayer_43760_43AA0(int16_t player)
{
	if (!networkFound_93958_93958 || !networkJoined_93959_93959)
		return;
	if (player == networkIndex_9395A_9395A)
	{
		// this computer leaves: the links to the others closed, the name of the network made free
		for (int16_t i = 0; i < networkCount_9395C_9395C; i++)
		{
			if (i == networkIndex_9395A_9395A)
				continue;
			sub_42C70_42FB0(i);
			sub_42DE0_43120(networkHandles_93990_93990[i]);
		}
		char name[0x14];
		sprintf(name, "%s%d", "TESTER", networkIndex_9395A_9395A);
		sub_42D40_43080(networkHandles_93990_93990[player], name);
		networkJoined_93959_93959 = 0;
	}
	else
	{
		sub_42C70_42FB0(player);
		sub_42DE0_43120(networkHandles_93990_93990[player]);
	}
	sub_43150_43490();
	// the first one still in the game sends the commands
	for (int16_t i = 0; i < networkCount_9395C_9395C; i++)
		if (networkActive_939B0_939B0[i])
		{
			networkMaster_9398A_9398A = (uint16_t)i;
			break;
		}
}

void ExchangeAll_43930_43C70(uint8_t* buffer, uint32_t size)
{
	if (!networkFound_93958_93958)
		return;
	if (networkIndex_9395A_9395A == (int16_t)networkMaster_9398A_9398A)
	{
		// the master: the part of each one in, then all to all
		for (int16_t i = 0; i < networkCount_9395C_9395C; i++)
			if (i != networkIndex_9395A_9395A)
				sub_438D0_43C10((uint16_t)i, buffer + i * size, size);
		for (int16_t i = 0; i < networkCount_9395C_9395C; i++)
			if (i != networkIndex_9395A_9395A)
				sub_43870_43BB0((uint16_t)i, buffer, networkCount_9395C_9395C * size);
	}
	else
	{
		// the own part to the master, all from it
		sub_43870_43BB0(networkMaster_9398A_9398A, buffer + networkIndex_9395A_9395A * size, size);
		sub_438D0_43C10(networkMaster_9398A_9398A, buffer, networkCount_9395C_9395C * size);
	}
}

void CreatePlayerWizard_44D30_45070(Type_Mc1Player_340B* player, Type_Mc1Entity* wizard)
{
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	// the computer players of the level: 90D4h + 0D8h each (words 0, 4, 8), 9794h the levels of their castles
	uint8_t* level = (uint8_t*)&state->level_0x2F503_193795;
	bool created = false;
	RebuildFreeLists_37220_375E0();
	const int16_t index = (int16_t)(player - state->players_0x340B_13323);
	Type_Mc1Position position = state->playerStarts_0x23D9_9177[index];
	position.z = (int16_t)(GetTerrainAlt_11F50_11F50((axis_3d*)&position) + 0x100);
	if (wizard == Entities())
	{
		wizard = CreateEntity_373F0_377B0(&position, 3, player->computer_0x09_9 == 1 ? 1 : 0);
		created = true;
	}
	else
	{
		wizard->actionIndex_0x46_70 = player->computer_0x09_9 == 1;
		wizard->flags_0x10_16 &= ~0x20u;
		const uint16_t castle = (uint16_t)ReadS16(wizard->playerData_0xA0_160 + 0x32);
		if (castle)
			position = Entities()[castle].position_0x48_72;
		CopyEntityPosition_41C70_41FB0(wizard, &position);
	}
	wizard->playerData_0xA0_160 = (uint8_t*)player + 0x44F;
	uint8_t* playerData = wizard->playerData_0xA0_160;
	WriteS16(playerData + 0x30, index);
	WriteS16(playerData + 0x14B, 100);
	WriteS32(playerData + 0x15F, 2000);
	WriteS16(playerData + 0x0C, 0);
	WriteS16(playerData + 0x18, 0);
	WriteS16(playerData + 0x1A, 0);
	WriteS16(playerData + 0x16, 0);
	WriteS16(playerData + 0x10, 0);
	// the spells of the player (by the model, -1 none) become entities of class 12
	for (int slot = 0; slot < 24; slot++)
	{
		const int32_t model = ReadS32(playerData + 0x214 + slot * 4);
		if (model < 0)
		{
			WriteS32(playerData + 0x214 + slot * 4, 0);
			continue;
		}
		Type_Mc1Entity* spell = CreateEntity_373F0_377B0(&wizard->position_0x48_72, 0x0C, model);
		if (!spell)
		{
			WriteS32(playerData + 0x214 + slot * 4, 0);
			continue;
		}
		WriteS32(playerData + 0x214 + slot * 4, EntityIndex(spell));
		spell->word_0x2A_42 = EntityIndex(wizard);
		spell->flags_0x10_16 |= 1;
		if (playerData[0x394 + (int8_t)spell->model_0x41_65])
		{
			spell->sprite_0x56_86 = 0x118;
			spell->dword_0x84_132 = 0;
			spell->flags_0x10_16 |= 0x40000;
		}
	}
	if (created)
	{
		WriteS32(playerData + 0x17B, (int32_t)timerTicks_AC5D4_AC5C4);
		const uint16_t number = (uint16_t)ReadS16(playerData + 0x30);
		if (number == 0)
			SetSprite_36FA0_37360(wizard, 0x2C);
		else if (number <= 7)
			SetSprite_36FA0_37360(wizard, (int16_t)(0x110 + number));
		UpdateSpellSlots_45C10_45F50(wizard);
		if (player->computer_0x09_9 == 1)
		{
			// the computer player: its settings and the castle from the level
			const int16_t computer = ReadS16(playerData + 0x30);
			WriteS16(playerData + 0x20A, ReadS16(level + 0x90D4 + computer * 0xD8));
			WriteS16(playerData + 0x20C, ReadS16(level + 0x90DC + computer * 0xD8));
			WriteS16(playerData + 0x20E, ReadS16(level + 0x90D8 + computer * 0xD8));
			if (ReadS16(playerData + 0x2C4) && level[0x9794 + computer])
			{
				Type_Mc1Entity* castle = CreateEntity_373F0_377B0(&wizard->position_0x48_72, 3, 2);
				if (castle)
				{
					castle->id_0x18_24 = wizard->id_0x18_24;
					WriteS16(playerData + 0x32, (int16_t)EntityIndex(castle));
					sub_55370_558A0((int16_t)EntityIndex(wizard), -1, 0x1E);
					// each level of the castle stamped through entity 0
					Type_Mc1Entity& stamp = Entities()[0];
					for (int floor = 0; floor < level[0x9794 + computer]; floor++)
					{
						stamp.position_0x48_72 = castle->target_0x96_150;
						stamp.model_0x41_65 = 0;
						stamp.word_0x1A_26 = 0;
						stamp.id_0x18_24 = castle->id_0x18_24;
						stamp.byte_0x47_71 = (uint8_t)floor;
						StampCastle_279D0_279D0(&stamp);
					}
					castle->word_0x1A_26 = (int16_t)(level[0x9794 + computer] - 1);
					SetBuildingBox_37150_37510(castle, castle->word_0x1A_26);
					GrowCastle_47C60_47FA0(castle);
					castle->mana_0x8C_140 = castle->dword_0x88_136;
					if (castle->mana_0x8C_140 < 0)
						castle->mana_0x8C_140 = 0;
					if (castle->mana_0x8C_140 > 0x4E200)
						castle->mana_0x8C_140 = 0x4E200;
				}
			}
		}
		WriteS32(playerData + 0x167, 0);
	}
	player->entityIndex_0x0A_10 = EntityIndex(wizard);
	if (state->currentPlayer_0x08_8 == ReadS16(playerData + 0x30))
		wizard->flags_0x10_16 |= 1;
	if (player->cheat_0x18_24 == 0xAE89E)
	{
		wizard->dword_0x88_136 = 1000000;
		wizard->maxLife_0x08_8 = 1000000;
	}
	else
	{
		wizard->dword_0x88_136 = 1000;
		wizard->maxLife_0x08_8 = 10000;
	}
	wizard->life_0x0C_12 = wizard->maxLife_0x08_8;
	wizard->mana_0x8C_140 = wizard->dword_0x88_136;
	WriteS32(playerData + 0x142, wizard->dword_0x88_136);
	UpdateSpellSlots_45C10_45F50(wizard);
	const uint16_t castle = (uint16_t)ReadS16(playerData + 0x32);
	if (castle)
		SetCastleMana_47DD0_48110(&Entities()[castle]);
	// the other wizards forget the attacks of this one
	for (Type_Mc1Entity* other = mc1Settings_AE408_AE3F8->entityLists_0x8E1A_36378[21]; other > Entities(); other = other->next_0x00_0)
		if (other->id_0x18_24 != wizard->id_0x18_24 && (other->model_0x41_65 == 0 || other->model_0x41_65 == 1))
			WriteS16(other->playerData_0xA0_160 + 0x1CC + ReadS16(playerData + 0x30) * 8, (int16_t)0x9FDF);
	WriteS16(playerData + 0x13A, 0);
	WriteS16(playerData + 0x13C, 0);
	if (wizard->model_0x41_65 == 1)
	{
		playerData[0x19F] = 0;
		for (int offset = 8; offset <= 0x40; offset += 8)
			WriteS16(playerData + 0x1C4 + offset, 0x601F);
		WriteS16(playerData + 0x2F4, (int16_t)(ReadS16(playerData + 0x30) << 2));
	}
	WriteS32(playerData + 0x18C, 0x800);
	WriteS32(playerData + 0x190, 0x800);
	WriteS16(playerData + 0x2E, 0);
	memset(playerData + 0x14D, 0x10, 8);
	state->reuseTop_0x11F1_4593 = -1;
	if (ReadS16(playerData + 0x30) == state->currentPlayer_0x08_8)
		mc1Settings_AE408_AE3F8->speedIndex_0x96_150 = 0;
}

void UpdateSpellSlots_45C10_45F50(Type_Mc1Entity* wizard)
{
	// by the model of the spell its entity (2A4h), from the slots (214h)
	uint8_t* playerData = wizard->playerData_0xA0_160;
	memset(playerData + 0x2A4, 0, 0x30);
	for (int slot = 0; slot < 24; slot++)
	{
		const int32_t entity = ReadS32(playerData + 0x214 + slot * 4);
		if (!entity)
			continue;
		const int8_t model = (int8_t)Entities()[entity].model_0x41_65;
		WriteS16(playerData + 0x2A4 + model * 2, (int16_t)entity);
	}
}

void SetCastleLifeAndMana_47BD0_47F10(Type_Mc1Entity* castle, Type_Mc1Entity* manaEntity, int32_t life, int32_t mana)
{
	if (life)
	{
		// the damage stays, at most a half of the new life
		int32_t damage = 0;
		const int32_t old = castle->life_0x0C_12;
		castle->maxLife_0x08_8 = life;
		if (old < 0)
		{
			damage = -old;
			if (damage > life / 2)
				damage = life / 2;
		}
		castle->life_0x0C_12 = castle->maxLife_0x08_8 - damage;
	}
	if (manaEntity)
	{
		manaEntity->dword_0x88_136 = mana;
		manaEntity->mana_0x8C_140 = mana / manaEntity->word_0x32_50;
	}
	castle->dword_0x88_136 = mana;
}

static Type_Mc1Entity* CastleManaEntity(const Type_Mc1Entity* castle)
{
	// 47C80: the entity at 2C4h of the owner (a wizard of the actions 0 / 1)
	const Type_Mc1Entity& owner = Entities()[castle->id_0x18_24];
	if (owner.actionIndex_0x46_70 != 0 && owner.actionIndex_0x46_70 != 1)
		return nullptr;
	const int16_t entity = ReadS16(owner.playerData_0xA0_160 + 0x2C4);
	return entity ? &Entities()[entity] : nullptr;
}

void GrowCastle_47C60_47FA0(Type_Mc1Entity* castle)
{
	// the life and the mana by the level of the castle
	static const int32_t lives[8] = { 0, 20000, 40000, 40000, 60000, 60000, 80000, 80000 };
	static const int32_t manas[8] = { 5000, 10000, 20000, 40000, 80000, 160000, 320000, 30000000 };
	Type_Mc1Entity* manaEntity = CastleManaEntity(castle);
	const uint16_t level = (uint16_t)castle->word_0x1A_26;
	if (level <= 7)
		SetCastleLifeAndMana_47BD0_47F10(castle, manaEntity, lives[level], manas[level]);
}

void SetCastleMana_47DD0_48110(Type_Mc1Entity* castle)
{
	static const int32_t manas[8] = { 5000, 10000, 20000, 40000, 80000, 160000, 320000, 30000000 };
	Type_Mc1Entity* manaEntity = CastleManaEntity(castle);
	if (!manaEntity)
		return;
	const uint16_t level = (uint16_t)castle->word_0x1A_26;
	const int32_t mana = level <= 7 ? manas[level] : 0;
	manaEntity->dword_0x88_136 = mana;
	manaEntity->mana_0x8C_140 = mana / manaEntity->word_0x32_50;
}

void StampCastle_279D0_279D0(Type_Mc1Entity* entity)
{
	// the level byte_0x47 of the castle from *build tab (RLE rows) into the heights, the corners and the terrain types
	uint8_t* heights = mapHeightmap_DC1E0_DC1D0;
	uint8_t* angles = mapAngle_FC1E0_FC1D0;
	uint8_t x = (uint8_t)((entity->position_0x48_72.x + 0x80) >> 8);
	uint8_t y = (uint8_t)((entity->position_0x48_72.y + 0x80) >> 8);
	const uint8_t height = (uint8_t)(entity->position_0x48_72.z >> 5);
	const bitmap_pos_struct_t& record = buildIndex[entity->byte_0x47_71];
	const uint8_t* data = record.data;
	uint32_t rows = record.height_5;
	uint32_t columns = record.width_4;
	if (*videoMode_12F02E_12F01E == 1)
	{
		rows >>= 1;
		columns >>= 1;
	}
	x = (uint8_t)(x - (columns >> 1));
	y = (uint8_t)(y - (rows >> 1));
	const uint8_t startX = x;
	const uint8_t startY = y;
	const uint32_t allRows = rows;
	uint32_t at = 0;
	const auto setCorner = [&](uint16_t tile, uint8_t value) {
		heights[tile] = value;
		const uint8_t corner = angles[tile];
		if (corner & 7)
			return;
		angles[tile] = (uint8_t)((corner & 0xF8) | 1);
		UpdateTerrainTiles_33B90_33F80(tile, tile);
	};
	// the heights: 0 the next row, negative skips, positive a run of codes
	uint8_t tileX = x;
	uint8_t tileY = y;
	while (rows)
	{
		const int8_t count = (int8_t)data[at++];
		if (!count)
		{
			tileY++;
			rows--;
			tileX = startX;
			continue;
		}
		if (count < 0)
		{
			tileX = (uint8_t)(tileX - count);
			continue;
		}
		for (int run = count; run; run--, tileX++)
		{
			const uint8_t code = data[at++];
			const uint16_t tile = (uint16_t)(tileX | tileY << 8);
			if (code < 0x0F)
			{
				if (code > 6)
					setCorner(tile, height);
				continue;
			}
			const int kind = code / 16;
			const int part = code % 16;
			if (kind == 3)
			{
				const int third = part % 3;
				if (third == 1)
					setCorner(tile, (uint8_t)(height + 0x0C));
				else if (third == 2)
					setCorner(tile, (uint8_t)(height + 0x10));
			}
			else if (part)
				setCorner(tile, (uint8_t)(height + ((part - 1) << 2)));
		}
	}
	// the terrain types
	rows = allRows;
	at = 0;
	tileX = startX;
	tileY = startY;
	while (rows)
	{
		const int8_t count = (int8_t)data[at++];
		if (!count)
		{
			tileY++;
			rows--;
			tileX = startX;
			continue;
		}
		if (count < 0)
		{
			tileX = (uint8_t)(tileX - count);
			continue;
		}
		for (int run = count; run; run--, tileX++)
		{
			const uint8_t code = data[at++];
			const uint16_t tile = (uint16_t)(tileX | tileY << 8);
			const int kind = code >> 4;
			// cl and dl of the caller as 33800 finds them
			if (kind == 0)
			{
				const uint8_t seventh = (uint8_t)(code % 7);
				if (seventh)
					SetTileTerrainType_33800_33BF0(7, tile, (uint8_t)(seventh - 1), seventh);
			}
			else if (kind <= 2)
				SetTileTerrainType_33800_33BF0(code, tile, (uint8_t)(kind + 7), 0);
			else if (kind == 3)
			{
				const uint8_t type = (uint8_t)((code % 16) / 3 + 0x0A);
				SetTileTerrainType_33800_33BF0(type, tile, type, (uint8_t)((code % 16) % 3));
			}
			else
				SetTileTerrainType_33800_33BF0(code, tile, (uint8_t)(kind + 0x0B), 0);
		}
	}
}

void SetCenterMouse_59720_59C30()
{
	if (*videoMode_12F02E_12F01E == 1)
		SetMousePosition_59750_59C60(0x140, 0xC8);
	else
		SetMousePosition_59750_59C60(0x140, 0xF0);
}

void SetMousePosition_59750_59C60(int16_t x, int16_t y)
{
	// the position in the coordinates of the mouse: 640x400 in 320x200, 640x480 in 640x480 (int 33h function 4;
	// the port moves the cursor of the window, as MC2 6EDE0)
	if (*mouseInstalled_9AD98_9AD98 <= 0)
		return;
	if (x == -1)
		return;
	if (x > 0)
	{
		if (x > 0x27E)
			x = 0x27E;
		mousePos_9AD90_9AD90->x = x;
	}
	if (y > 0)
	{
		const int16_t maxY = (*videoMode_12F02E_12F01E & 1) ? 0x18E : 0x1DE;
		if (y > maxY)
			y = maxY;
		mousePos_9AD90_9AD90->y = y;
	}
	else
		y = mousePos_9AD90_9AD90->y;
	if (*videoMode_12F02E_12F01E & 1)
		VGA_Set_mouse(x / 2, y / 2);
	else
		VGA_Set_mouse(x, y);
}
