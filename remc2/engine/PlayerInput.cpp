#include "PlayerInput.h"

#include "../utilities/DataFileIO.h"
#include "../shared/EventDispatcher.h"
#include "Animation.h"
#include "Basic.h"
#include "Events.h"
#include "GameUI.h"
#include "Graphics.h"
#include "Level.h"
#include "MenusAndIntros.h"
#include "Sound.h"
#include "engine_support.h"
#include "DatTabIndexes.h"
#include "EventsFunctions.h"
#include "Vr.h"
#include "CommandLineParser.h"
#include "Network.h"
#include "Player.h"
#include "../utilities/StateMonitor.h"
#include "../utilities/RendererTests.h"
#include "ConvertMapInfo.h"
#include "LevelInit.h"
#include "MouseOnOff.h"
#include "PrintMessages.h"
#include "ReadAndDecompress.h"
#include "Type_DB080_CdTrack.h"

__int16 joyFilteredX_E36CC = 0; // weak

__int16 joyFilteredY_E36CE = 0; // weak

char joystickInitialized_E36D0 = 0; // weak

int keyboardFlag_E4CA4 = 0; // weak

uint8_t keys_EB39E[10]; // weak 0 - setting keys

char* vrRecord4_17D6C4; // weak

uint8_t unk_180560x[44]; // weak

type_unk_18058Cstr inputState_18058C;

char keyShiftFlags_1806E5; // weak

int8_t cursorPosX_1806E8;

int8_t cursorPosY_1806E8;

int mouseInstalled2_180720; // weak


char cursorSpriteIndex_D419E = 0; // weak//2a519e
char IsPauseMenuOpen_D47D9 = 0; // weak
char keyToButton_E36DC[124] = { 
0x00,0x27,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x30,0x2D,0x3D,0x08,0x09,
0x51,0x57,0x45,0x52,0x54,0x59,0x55,0x49,0x4F,0x50,0x5B,0x5D,0x0D,0x00,0x41,0x53,
0x44,0x46,0x47,0x48,0x4A,0x4B,0x4C,0x3B,0x27,0x23,0x00,0x5C,0x5A,0x58,0x43,0x56,
0x42,0x4E,0x4D,0x2C,0x2E,0x2F,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2D,0x00,0x00,0x00,0x2B,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x2F,0x00,0x00,0x28,0x29,0x2F,0x2A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x2E,0x0D,0x00,0x40,0x01,0xC8,0x00,0x00,0x00,0x00,0x00 }; // weak//2b46dc
__int16 x_WORD_180463; // weak
int savedViewportY_18065C; // weak
int savedViewportHeight_180640; // weak
int savedViewportWidth_18064C; // weak
int savedClipWidth_180658; // weak
int savedViewportX_180638; // weak
int savedClipHeight_180654; // weak
char x_BYTE_1810A3[320]; // fix it -  weak
int x_DWORD_17FF14; // weak
int16_t x_WORD_17FF18; // weak
int x_DWORD_17FF20; // weak
int cdBuffer_17FF24; // weak
int x_DWORD_17FF28 = 0; // weak
int x_DWORD_17FF38; // weak
int x_DWORD_17FF40; // weak
int16_t scanlineOffsets_1811E4[400]; // fix it -  weak


char StopSoundAndMusic_5BF10();
void FindNearestEntity_70940();
void HandleOptionsMenuButtonClick_19A70();
void SwitchHelpMode_18AA0();
int SelectSpell_6D4F0(type_str_611* a1, int16_t mouseX);
void ChangeGameResolution_46B40();
void ClearScreenAndBlit_75C50();
int ClearVgaMemory_906B4();
signed int InitScanlineOffsets_90668(int a1);
void VgaSetVideoModeAltAndPalette_90E07(TColor* Palette);


int debugcounter_47560 = 0;
//----- (00017190) --------------------------------------------------------
void ProcessKeyboardPresses_17190()//1f8190
{
	type_entity_0x6E8E* event; // edx

	if (CommandLineParams.DoOffPause5()) {
		if (debugcounter_47560 == 5)
		{
			gameState_D41A4.OptionsSettingFlag_24 &= 0xfe;
			//SetPauseMenuClosed_41B60();
			/*gameState_D41A4.byteindex_205 = 0;
			gameState_D41A4.byteindex_206 = 0;
			SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]);
			SetMenuCursorPosition_52E90(
				&D41A0_BYTESTR_0.array_0x2BDE[D41A0_BYTESTR_0.word_0xc],
				D41A0_BYTESTR_0.array_0x2BDE[D41A0_BYTESTR_0.word_0xc].MenuState_0x3DF_2BE4_12221,
				1);
			if (x_BYTE_E37FD)
				sub_8E160_sound_proc15_startsequence(D41A0_BYTESTR_0.dword_0x235, 0x7Fu);
			QueryCdAudioStatus_86930(cdDrive_1803EC);
			if (gameState_D41A4.byteindex_225)
				gameState_D41A4.byteindex_225 = 1;
			gameState_D41A4.byteindex_208 = sub_55C00(gameState_D41A4.levelnumber_43w);*/
		}
	}

	if (!(gameState_D41A4.setting_byte1_22 & 4) || HasSkipInput_473E0() == 0)
	{
		if (!world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0)
		{
			if (!(world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] & 0x20))
			{
				event = Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
				if (pressedKeys_180664[56])
				{
					switch (LastPressedKey_1806E4)
					{
					case 0x3c: {//f2
						HandleButtonClick_191B0(30, 2);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x3d: {//f3
						HandleButtonClick_191B0(30, 3);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x3e: {//f4
						HandleButtonClick_191B0(30, 4);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x3f: {//f5
						HandleButtonClick_191B0(30, 5);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x40: {//f6
						HandleButtonClick_191B0(30, 6);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x41: {//f7
						HandleButtonClick_191B0(30, 7);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x42: {//f8
						HandleButtonClick_191B0(30, 8);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x43: {//f9
						HandleButtonClick_191B0(30, 9);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x44: {//f10
						HandleButtonClick_191B0(30, 10);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x2c: {//z
						if (gameState_D41A4.setting_byte2_23 < 0)
						{
							if (gameState_D41A4.setting_byte4_25 & 0x20)
								gameState_D41A4.setting_byte4_25 &= 0xDF;
							else
								gameState_D41A4.setting_byte4_25 |= 0x20;
						}
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x2f: {//v
						if (gameState_D41A4.setting_byte2_23 < 0)
							HandleButtonClick_191B0(4, 8);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x3b: {//f1
						HandleButtonClick_191B0(30, 1);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x14: {//t
						if (gameState_D41A4.setting_byte2_23 < 0)
						{
							if (pressedKeys_180664[54])
								FindNearestEntity_70940();
							else
								HandleButtonClick_191B0(4, 64);
						}
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x20: {//d
						if (gameState_D41A4.setting_byte2_23 < 0)
						{
							StopSoundAndMusic_5BF10();
							pressedKeys_180664[56] = 0;
							if (gameState_D41A4.OptionsSettingFlag_24 & 0x10)
								gameState_D41A4.OptionsSettingFlag_24 &= 0xEF;
							else
								gameState_D41A4.OptionsSettingFlag_24 |= 0x10;
						}
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x23: {//h
						world_D41A0.m_GameSettings.graphics_0x2192.xxxx_0x2193 ^= 1u;
						LastPressedKey_1806E4 = 0;
						break;
					}
					}
					if (pressedKeys_180664[24])
					{
						world_D41A0.altSpawnMode_0x36E03 = 2;
					}
					return;
				}
				if (pressedKeys_180664[42] || pressedKeys_180664[54])
				{
					switch (LastPressedKey_1806E4)
					{
					case 0x10: {//q
						HandleButtonClick_191B0(2, 0);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x12: {//e
						if (gameState_D41A4.setting_byte2_23 < 0)
							HandleButtonClick_191B0(26, 0);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x13: {//r
						if (gameState_D41A4.setting_byte2_23 < 0 && !(gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE))
						{
							world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] |= 0xCu;
							world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.CastleEntityIndex_0x3A_58 = 0;
						}
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x20: {//d
						if (gameState_D41A4.setting_byte2_23 < 0)
						{
							world_D41A0.stages_0x3654C[world_D41A0.playerObjectives_0x3659C[world_D41A0.LevelIndex_0xc].substr_3659C.ObjectiveText_1].stages_3654C_byte0 |= 2u;
						}
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x21: {//f
						if (gameState_D41A4.setting_byte2_23 < 0)
						{
							world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] |= 4u;
						}
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x25: {//k
						if (!(gameState_D41A4.setting_byte1_22 & 0x8010))
							event->life_0x8 = -1;
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x26: {//l remove castle stage
						if (event->playerData_0xA4_164x->CastleEntityIndex_0x3A_58)
							HandleButtonClick_191B0(42, 0);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x2e: {//c
						if (gameState_D41A4.setting_byte2_23 < 0)
						{
							world_D41A0.playerObjectives_0x3659C[world_D41A0.LevelIndex_0xc].substr_3659C.IsLevelEnd_0 = 1;
							HandleButtonClick_191B0(27, 0);
						}
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x3e: {//f4
						ChangeSettings_1A970(12, 0, 0);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x3f: {//f5
						ChangeSettings_1A970(13, 0, 0);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x40: {//f6
						ChangeSettings_1A970(14, 0, 0);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x41: {//f7
						ChangeSettings_1A970(15, 0, 0);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x44: {//f10
						gameState_D41A4.pauseToggle_205 = (gameState_D41A4.pauseToggle_205 == 0);
						gameState_D41A4.pauseState_206 = gameState_D41A4.pauseToggle_205;
						if (gameState_D41A4.pauseToggle_205 && gameState_D41A4.OptionsSettingFlag_24 & GAME_PAUSED)
							SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]); //Set cursor to Null (Don't Draw)
						if (!gameState_D41A4.pauseState_206 && gameState_D41A4.OptionsSettingFlag_24 & GAME_PAUSED)
							SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[cursorSpriteIndex_D419E]); //fix it LOBYTE(v0) = sub_8CD27(dword_EB394 + 6 * (unsigned __int8)byte_D419E);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x3c: {//f2
						ChangeSettings_1A970(10, 0, 0);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x3d: {//f3
						ChangeSettings_1A970(11, 0, 0);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x3b: {//f1
						ChangeSettings_1A970(9, 0, 0);
						LastPressedKey_1806E4 = 0;
						break;
					}
					}
				}
				else
				{
					switch (LastPressedKey_1806E4)
					{
					case 0x19: {//p
						if (!(gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE) && (!IsRecordingOrPlaying()))
							PauseUnpauseGame_18BB0();
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x13: {//r
						ChangeSettings_1A970(16, 0, 0);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x14: {//t
						ChangeSettings_1A970(19, 0, 0);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x39: {//space
						HandleButtonClick_191B0(15, 0);
						LastPressedKey_1806E4 = 0;
						break;
					}

					case 0x24: {//j // switch debug info
						if (CommandLineParams.DoShowInGameDebug())
						{
							FindNearestEntity_70940();
							ChangeSettings_1A970(20, 0, 0);
							LastPressedKey_1806E4 = 0;
						}
						break;
					}

					case 0x3e: {//f4
						ChangeSettings_1A970(4, 0, 0);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x3f: {//f5
						ChangeSettings_1A970(2, 0, 0);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x40: {//f6
						ChangeSettings_1A970(3, 0, 0);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x41: {//f7
						ChangeSettings_1A970(6, 0, 0);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x42: {//f8
						ChangeSettings_1A970(17, 0, 0);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x1f: {//s
						if (!(gameState_D41A4.OptionsSettingFlag_24 & GAME_PAUSED)
							&& videoMode_180660 & 1
							&& (inputState_18058C.joystickMode_1805C2 == 0x01 || inputState_18058C.joystickMode_1805C2 == 0x08 || inputState_18058C.joystickMode_1805C2 == 0x0c))
						{
							ChangeSettings_1A970(18, 0, 0);
						}
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x3b: {//f1
						ChangeSettings_1A970(1, 0, 0);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x3c: {//f2
						ChangeSettings_1A970(5, 0, 0);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x3d: {//f3
						ChangeSettings_1A970(7, 0, 0);
						LastPressedKey_1806E4 = 0;
						break;
					}
					}
				}
				if (pressedKeys_180664[24])
				{
					world_D41A0.objectiveBoxCounter_0x36E04 = 2;
				}
				return;
			}
		}
	}
}

//----- (00018BB0) --------------------------------------------------------
void PauseUnpauseGame_18BB0()//1f9bb0
{
	//int v0; // eax
	//char v1; // dl
	char v2; // cl
	//int v3; // eax
	//int v4; // edx
	//int result; // eax

	//v0 = gameState_D41A4.dwordindex_0;
	//v1 = gameState_D41A4.OptionsSettingFlag_24  ^ 1;
	gameState_D41A4.OptionsSettingFlag_24 ^= GAME_PAUSED;
	if (gameState_D41A4.OptionsSettingFlag_24 & GAME_PAUSED)
	{
		if (world_D41A0.gameFlags_0x36E0B & 2)//if ( *(0x36E09 + x_D41A0_BYTEARRAY_0 + 2) & 2 )
		{
			gameState_D41A4.OptionsSettingFlag_24 &= 0xFEu;
		}
		else
		{
			world_D41A0.objectiveBoxCounter_0x36E04 = 0;
			v2 = gameState_D41A4.pauseState_206;
			helpPopup_1804B0.pauseMenuItem_0xaa = -1;
			if (!v2 && (inputState_18058C.joystickMode_1805C2 == 7 || inputState_18058C.joystickMode_1805C2 == 1 || inputState_18058C.joystickMode_1805C2 == 2))
				SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[cursorSpriteIndex_D419E]);
			//SetCursor_8CD27(menuSprites_17DED4[39]);

			SetPausedMenuOpen_41AF0();
			if (!gameState_D41A4.pauseState_206)
				SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[cursorSpriteIndex_D419E]);
			SetMenuCursorPosition_52E90(
				&world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc],
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221, true, gameUiScale);
			if (soundActive_E3799)
				EndSample_8D8F0();
			if (musicActive_E37FD)
				StopMusic_8E020();
			if (gameState_D41A4.byteindex_225)
				gameState_D41A4.byteindex_225 = 2;
		}
	}
	else
	{
		SetPauseMenuClosed_41B60();
		//v3 = gameState_D41A4.dwordindex_0;
		//v4 = **filearray_2aa18c[0]; //fix it
		gameState_D41A4.pauseToggle_205 = 0;
		gameState_D41A4.pauseState_206 = 0;
		SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]); //Set cursor to Null (Don't Draw)
		SetMenuCursorPosition_52E90(
			&world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc],
			world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221, true, gameUiScale);
		if (musicActive_E37FD)
			StartMusic_8E160(world_D41A0.maptypeMusic_0x235, 0x7Fu);
		QueryCdAudioStatus_86930(cdDrive_1803EC);
		if (gameState_D41A4.byteindex_225)
			gameState_D41A4.byteindex_225 = 1;
	}
	//LOBYTE(result) = sub_55C00(gameState_D41A4.levelnumber_43);
	gameState_D41A4.saveFileExists_208 = DataFileIO::TestSaveFile2_55C00(gameState_D41A4.levelnumber_43w);
	//return result;
}

//----- (00017A00) --------------------------------------------------------
void MouseAndKeysEvents_17A00(int16_t turn)//1f8a00
{
	type_entity_0x6E8E* v8x; // ebx
	char v11; // dh
	type_entity_0x6E8E* v12x; // edi
	unsigned __int8 v16; // al
	char v17; // al
	int v18; // esi
	char v19; // al
	char v21; // dl
	char v23; // al
	type_entity_0x6E8E* v24x; // ebx
	unsigned char v27; // al
	signed int v28; // eax
	int v29; // eax
	int v30; // ecx
	char v32; // [esp-4h] [ebp-Ch]
	char v33; // [esp+0h] [ebp-8h]
	char v34; // [esp+4h] [ebp-4h]

	if (LastPressedKey_1806E4 >= 0x80u)
		LastPressedKey_1806E4 = 0;
	if (!(world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] & 0x20))
	{
		switch (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221)
		{
		case 0:
		case 4:
			if (world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0)
				goto LABEL_292;
			if (gameState_D41A4.OptionsSettingFlag_24 & GAME_PAUSED)
				ReadPauseMenuEvents_197F0();
			ProcessKeyboardPresses_17190();//test FnX
			v8x = Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
			world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].entityIndex_0x6E3E_byte5 = 0;
			if (inputState_18058C.MouseButtonState_18059C & 0x10)
			{
				if (v8x->life_0x8 >= 0)
					HandleButtonClick_191B0(20, 5);
			}
			else if (pressedKeys_180664[56] || pressedKeys_180664[keys_EB39E[9]])
			{
				switch (LastPressedKey_1806E4) {
					case 0x13://r
					case 0x19://p
					case 0x1f: {//s
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x21: {//f
						HandleButtonClick_191B0(4, 32);
						LastPressedKey_1806E4 = 0;
						break;
					}
				}

				if (inputState_18058C.MouseButtonState_18059C & 1)
				{
					CycleSelectedSpell_18DA0(v8x, 1, 1);
					inputState_18058C.MouseButtonState_18059C &= 0xFE;
				}
				else if (inputState_18058C.MouseButtonState_18059C & 2)
				{
					CycleSelectedSpell_18DA0(v8x, 2, 1);
					inputState_18058C.MouseButtonState_18059C &= 0xFD;
				}
			}
			else if (pressedKeys_180664[42] || pressedKeys_180664[keys_EB39E[8]]) //LShift or RShift
			{
				if (inputState_18058C.MouseButtonState_18059C & 1)
				{
					CycleSelectedSpell_18DA0(v8x, 1, 0);
					inputState_18058C.MouseButtonState_18059C &= 0xFE;
				}
				else if (inputState_18058C.MouseButtonState_18059C & 2)
				{
					CycleSelectedSpell_18DA0(v8x, 2, 0);
					inputState_18058C.MouseButtonState_18059C &= 0xFD;
				}
			}
			else
			{
				switch (LastPressedKey_1806E4)
				{
				case 0x1a: {//[
					if (!world_D41A0.m_GameSettings.m_Display.m_uiScreenSize)
					{
						if (world_D41A0.m_GameSettings.m_Graphics.m_wViewPortSize < 40)
						{
							world_D41A0.m_GameSettings.m_Graphics.m_wViewPortSize++;
							viewPort.ResizeViewPort_2CA90(world_D41A0.m_GameSettings.m_Graphics.m_wViewPortSize);
						}
					}
					LastPressedKey_1806E4 = 0;
					break;
				}
				case 0x1b: {//]
					if (!world_D41A0.m_GameSettings.m_Display.m_uiScreenSize)
					{
						if (world_D41A0.m_GameSettings.m_Graphics.m_wViewPortSize > 17)
						{
							world_D41A0.m_GameSettings.m_Graphics.m_wViewPortSize--;// = v10 - 1;
							viewPort.ResizeViewPort_2CA90(world_D41A0.m_GameSettings.m_Graphics.m_wViewPortSize);
						}
					}
					LastPressedKey_1806E4 = 0;
					break;
				}
				case 0x17://i
					if (!(gameState_D41A4.setting_byte4_25 & 2))
					{
						v11 = v8x->actionIndex_0x45_69;
						if (v11 != 2 && v11 != 3)
							HandleButtonClick_191B0(16, 0);
					}
					LastPressedKey_1806E4 = 0;
					goto LABEL_296;
				case 0x01://esc
					ToggleSpellMenu_18B30();
					LastPressedKey_1806E4 = 0;
					FlyAssistant_1A7A0();
					goto LABEL_306;
				case 0x39://space
					HandleButtonClick_191B0(15, 0);
					LastPressedKey_1806E4 = 0;
					break;
				}
				if (inputState_18058C.MouseButtonState_18059C & 1 && inputState_18058C.MouseButtonState_18059C & 2)
				{
					if (!disableLRButtonsMenuOpen && v8x->life_0x8 >= 0)
						HandleButtonClick_191B0(20, 6); //Mouse Open Map

					if (disableLRButtonsMenuOpen)
						inputState_18058C.MouseButtonState_18059C &= 0xFD;
					else
						inputState_18058C.MouseButtonState_18059C &= 0xFC;
				}
				if (LastPressedKey_1806E4 == keys_EB39E[4])
				{
					if (v8x->life_0x8 >= 0)
						HandleButtonClick_191B0(20, 6);  //Keyboard Open Map
					LastPressedKey_1806E4 = 0;
				}
				if (LastPressedKey_1806E4 >= 2u)//2-9
				{
					if (LastPressedKey_1806E4 <= 9u)
					{
						HandleButtonClick_191B0(43, LastPressedKey_1806E4 - 2);
						LastPressedKey_1806E4 = 0;
					}
					else if (LastPressedKey_1806E4 == 0x20)//d
					{
						HandleOptionsMenuButtonClick_19A70();
						LastPressedKey_1806E4 = 0;
					}
				}
				if (!pressedKeys_180664[47] || gameState_D41A4.volumeKeyHeld_38591)//volume
				{
					if (pressedKeys_180664[50] && !gameState_D41A4.volumeKeyHeld_38591)
						ChangeSoundLevel_19CA0(2u);
				}
				else
				{
					ChangeSoundLevel_19CA0(1u);
				}
				HandleMouseButtons_18F80(v8x);
			}
			if (LastPressedKey_1806E4 && keyToButton_E36DC[LastPressedKey_1806E4] == 8)
			{
				HandleButtonClick_191B0(39, 1);
				LastPressedKey_1806E4 = 0;
			}
			HandleArrowKeyPresses_1A8A0();
			ComputeMousePlayerMovement_17060(inputState_18058C.mouse_1805B0.x, inputState_18058C.mouse_1805B0.y);
			FlyAssistant_1A7A0();
			goto LABEL_306;
		case 1:
			ProcessKeyboardPresses_17190();
			if ((!(inputState_18058C.MouseButtonState_18059C & 1) || !(inputState_18058C.MouseButtonState_18059C & 2)) && LastPressedKey_1806E4 != 0x1c)
				goto LABEL_296;
			LastPressedKey_1806E4 = 0;
			inputState_18058C.MouseButtonState_18059C &= 0xFC;
			HandleButtonClick_191B0(20, 0);
			FlyAssistant_1A7A0();
			goto LABEL_306;
		case 3:
			if (!LastPressedKey_1806E4 || LastPressedKey_1806E4 >= 0x80u)
				goto LABEL_216;
			if (LastPressedKey_1806E4 < 0x1Cu)
			{
				if (LastPressedKey_1806E4 == 1)
				{
					HandleButtonClick_191B0(18, 0);
					goto LABEL_215;
				}
				goto LABEL_206;
			}
			if (LastPressedKey_1806E4 <= 0x1Cu)
			{
				HandleButtonClick_191B0(19, 0);
				goto LABEL_215;
			}
			if (LastPressedKey_1806E4 < 0x3Bu)
				goto LABEL_206;
			if (LastPressedKey_1806E4 <= 0x42u)
			{
				if (pressedKeys_180664[42] || pressedKeys_180664[54]) //LShift or RShift
				{
					HandleButtonClick_191B0(35, LastPressedKey_1806E4 - 59);
				}
				else if (LastPressedKey_1806E4 == 0x3b) //F1
				{
					SwitchHelpMode_18AA0();
				}
				LastPressedKey_1806E4 = 0;
			}
			else
			{
				if (LastPressedKey_1806E4 != 0x43)
				{
				LABEL_206:
					v27 = keyToButton_E36DC[LastPressedKey_1806E4];
					if (v27 >= 0x41u && v27 <= 0x5Au
						|| v27 >= 0x61u && v27 <= 0x7Au
						|| v27 >= 0x30u && v27 <= 0x39u
						|| v27 == 8
						|| v27 == 32)
					{
						HandleButtonClick_191B0(17, keyToButton_E36DC[LastPressedKey_1806E4]);
					}
					goto LABEL_215;
				}
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].chatState_0x3E1_2BE4_12223++;
				if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].chatState_0x3E1_2BE4_12223 >= 4)
					world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].chatState_0x3E1_2BE4_12223 = 0;
				HandleButtonClick_191B0(36, world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].chatState_0x3E1_2BE4_12223);
			}
		LABEL_215:
			LastPressedKey_1806E4 = 0;
		LABEL_216:
			if (videoMode_180660 & 1)
				v28 = 400;
			else
				v28 = screenHeight_180624;
			v29 = v28 - 72;
			if (inputState_18058C.mouse_1805B0.x < 4 || inputState_18058C.mouse_1805B0.x >= 66 || v29 + 28 > inputState_18058C.mouse_1805B0.y || v29 + 66 <= inputState_18058C.mouse_1805B0.y)
			{
				if (inputState_18058C.mouse_1805B0.x < 384 || inputState_18058C.mouse_1805B0.x >= 654 || v29 + 26 > inputState_18058C.mouse_1805B0.y || v29 + 48 <= inputState_18058C.mouse_1805B0.y)
				{
					if (inputState_18058C.mouse_1805B0.x < 82 || inputState_18058C.mouse_1805B0.x >= 378 || v29 + 32 > inputState_18058C.mouse_1805B0.y || v29 + 60 <= inputState_18058C.mouse_1805B0.y)
					{
						if (inputState_18058C.mouse_1805B0.x < 384 || inputState_18058C.mouse_1805B0.x >= 440 || v29 + 4 > inputState_18058C.mouse_1805B0.y || v29 + 22 <= inputState_18058C.mouse_1805B0.y)
						{
							if (inputState_18058C.mouse_1805B0.x < 474 || inputState_18058C.mouse_1805B0.x >= 530 || v29 + 4 > inputState_18058C.mouse_1805B0.y || v29 + 22 <= inputState_18058C.mouse_1805B0.y)
							{
								if (inputState_18058C.mouse_1805B0.x >= 564
									&& inputState_18058C.mouse_1805B0.x < 620
									&& v29 + 4 <= inputState_18058C.mouse_1805B0.y
									&& v29 + 22 > inputState_18058C.mouse_1805B0.y
									&& inputState_18058C.MouseButtonState_18059C & 1)
								{
									HandleButtonClick_191B0(18, 0);
								}
							}
							else if (inputState_18058C.MouseButtonState_18059C & 1)
							{
								HandleButtonClick_191B0(44, 0);
							}
						}
						else if (inputState_18058C.MouseButtonState_18059C & 1)
						{
							HandleButtonClick_191B0(19, 0);
						}
						goto LABEL_292;
					}
					if (inputState_18058C.mouse_1805B0.x < 82 || inputState_18058C.mouse_1805B0.x >= 150 || v29 + 32 > inputState_18058C.mouse_1805B0.y || v29 + 60 <= inputState_18058C.mouse_1805B0.y)
					{
						if (inputState_18058C.mouse_1805B0.x >= 152 && inputState_18058C.mouse_1805B0.x < 220 && v29 + 32 <= inputState_18058C.mouse_1805B0.y && v29 + 60 > inputState_18058C.mouse_1805B0.y)
						{
							if (gameState_D41A4.showHelp_10)
								helpPopup_1804B0.keyHelpItem_0xa9 = 99;
							if (inputState_18058C.MouseButtonState_18059C & 1)
								HandleButtonClick_191B0(36, 1);
							goto LABEL_292;
						}
						if (inputState_18058C.mouse_1805B0.x >= 222 && inputState_18058C.mouse_1805B0.x < 290 && v29 + 32 <= inputState_18058C.mouse_1805B0.y && v29 + 60 > inputState_18058C.mouse_1805B0.y)
						{
							if (gameState_D41A4.showHelp_10)
								helpPopup_1804B0.keyHelpItem_0xa9 = 100;
							if (inputState_18058C.MouseButtonState_18059C & 1)
								HandleButtonClick_191B0(36, 2);
							goto LABEL_292;
						}
						if (inputState_18058C.mouse_1805B0.x < 310 || inputState_18058C.mouse_1805B0.x >= 378 || v29 + 32 > inputState_18058C.mouse_1805B0.y || v29 + 60 <= inputState_18058C.mouse_1805B0.y)
							goto LABEL_292;
						if (gameState_D41A4.showHelp_10)
							helpPopup_1804B0.keyHelpItem_0xa9 = 101;
						if (!(inputState_18058C.MouseButtonState_18059C & 1))
							goto LABEL_292;
						v32 = 3;
					}
					else
					{
						if (gameState_D41A4.showHelp_10)
							helpPopup_1804B0.keyHelpItem_0xa9 = 98;
						if (!(inputState_18058C.MouseButtonState_18059C & 1))
							goto LABEL_292;
						v32 = 0;
					}
					HandleButtonClick_191B0(36, v32);
					goto LABEL_292;
				}
				if (gameState_D41A4.showHelp_10)
					helpPopup_1804B0.keyHelpItem_0xa9 = 102;
				if (inputState_18058C.MouseButtonState_18059C & 1)
				{
					v30 = (inputState_18058C.mouse_1805B0.x - 384) / 34;
					if (v30 >= world_D41A0.LevelIndex_0xc)
						v30++;
					HandleButtonClick_191B0(37, v30);
				}
			}
			else
			{
				if (gameState_D41A4.showHelp_10)
					helpPopup_1804B0.keyHelpItem_0xa9 = 97;
				if (inputState_18058C.MouseButtonState_18059C & 1)
					HandleButtonClick_191B0(
						35,
						4 * ((inputState_18058C.mouse_1805B0.y - (v29 + 28)) / 20)
						+ ((inputState_18058C.mouse_1805B0.x - 4 - (__CFSHL__((inputState_18058C.mouse_1805B0.x - 4) >> 31, 4) + 16 * ((inputState_18058C.mouse_1805B0.x - 4) >> 31))) >> 4));
			}
		LABEL_292:
			inputState_18058C.MouseButtonState_18059C &= 0xFC;
			FlyAssistant_1A7A0();
			goto LABEL_306;
		case 5:
		case 8:
			v33 = 0;
			v34 = 0;
			v12x = Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
			ProcessKeyboardPresses_17190();
			if (v12x->life_0x8 < 0)
			{
				v33 = 1;
			}
			else
			{
				v16 = v12x->playerData_0xA4_164x->str_611.selectSpellType_0x457_1111;
				if (v16)
				{
					if (v16 <= 2u)
					{
						if ((v16 != 1 || inputState_18058C.MouseButtonState_18059C & 4) && (v12x->playerData_0xA4_164x->str_611.selectSpellType_0x457_1111 != 2 || inputState_18058C.MouseButtonState_18059C & 8))
						{
							v23 = SelectSpell_6D4F0(&v12x->playerData_0xA4_164x->str_611, inputState_18058C.mouse_1805B0.x);
							v12x->playerData_0xA4_164x->str_611.subSpellIndex_0x459_1113 = v23;
							HandleButtonClick_191B0(41, v23);
						}
						else
						{
							if (v12x->playerData_0xA4_164x->str_611.selectSpellType_0x457_1111 == 1)
								world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0 = 31;
							else
								world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0 = 32;
							world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].param1_0x6E3E_byte1 = v12x->playerData_0xA4_164x->str_611.spellIndex_0x458_1112;
							world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].param2_0x6E3E_byte2 = v12x->playerData_0xA4_164x->str_611.subSpellIndex_0x459_1113;
							v12x->playerData_0xA4_164x->str_611.selectSpellType_0x457_1111 = 0;
							MoveCursorToSelectedSpell_6D200(&world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc]);
						}
					}
				}
				else
				{
					v34 = 1;
					v17 = SelectSpellCategory_6D420(inputState_18058C.mouse_1805B0.x, inputState_18058C.mouse_1805B0.y);
					v12x->playerData_0xA4_164x->str_611.spellIndex_0x458_1112 = v17;
					v18 = spellIndex_D94FF[v17];
					v19 = 1;
					if (!v12x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[v18] || !isCaveLevel_D41B6 && v18 == 25)
						v19 = 0;
					if (!v19)
						goto LABEL_122;
					UpdateSubSpellIndex_6D4C0(&v12x->playerData_0xA4_164x->str_611);
					if (!(inputState_18058C.MouseButtonState_18059C & 1) && !(inputState_18058C.MouseButtonState_18059C & 2))
						goto LABEL_122;
					if (pressedKeys_180664[42] || pressedKeys_180664[keys_EB39E[7]])
					{
						HandleButtonClick_191B0(38, v18);
						world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].param2_0x6E3E_byte2 = 0;
						v21 = v12x->playerData_0xA4_164x->str_611.spellSlotTypes_0x3B5_949x.SpellIndex[v18];
						if (v21)
						{
							if (inputState_18058C.MouseButtonState_18059C & 1)
							{
								if (v21 != 1)
									world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].param2_0x6E3E_byte2 = 1;
								goto LABEL_122;
							}
							if (v21 == 2)
								goto LABEL_122;
						}
						else if (inputState_18058C.MouseButtonState_18059C & 1)
						{
							world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].param2_0x6E3E_byte2 = 1;
							goto LABEL_122;
						}
						world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].param2_0x6E3E_byte2 = 2;
						goto LABEL_122;
					}
					if (inputState_18058C.MouseButtonState_18059C & 1 && inputState_18058C.MouseButtonState_18059C & 2)
					{
						HandleButtonClick_191B0(6, 64);
						world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].param1_0x6E3E_byte1 = v12x->playerData_0xA4_164x->str_611.spellIndex_0x458_1112;
					}
					else
					{
						v12x->playerData_0xA4_164x->str_611.selectSpellType_0x457_1111 = ((inputState_18058C.MouseButtonState_18059C & 1) == 0) + 1;
						MoveCursorToSelectedSpell_6D200(&world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc]);
						HandleButtonClick_191B0(40, v12x->playerData_0xA4_164x->str_611.spellIndex_0x458_1112);
						world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].param2_0x6E3E_byte2 = v12x->playerData_0xA4_164x->str_611.selectSpellType_0x457_1111;
					}
				}
			}
		LABEL_122:
			if (!(inputState_18058C.MouseButtonState_18059C & 0x10) && !(inputState_18058C.MouseButtonState_18059C & 4) && !(inputState_18058C.MouseButtonState_18059C & 8))
				v33 = 1;
			if (v33)
			{
				if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 == 5)
					HandleButtonClick_191B0(20, 0);
				else
					HandleButtonClick_191B0(20, 6);
			}
			else
			{
				if (v12x->life_0x8 >= 0)
				{
					if (inputState_18058C.arrowKeys_1805C0 & 1)
						HandleButtonClick_191B0(6, 1);
					if (inputState_18058C.arrowKeys_1805C0 & 2)
						HandleButtonClick_191B0(6, 2);
					if (inputState_18058C.arrowKeys_1805C0 & 4)
						HandleButtonClick_191B0(6, 4);
					if (inputState_18058C.arrowKeys_1805C0 & 8)
						HandleButtonClick_191B0(6, 8);
				}
				if (gameState_D41A4.setting_byte1_22 & 2
					&& v34
					&& !world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0)
				{
					HandleButtonClick_191B0(40, v12x->playerData_0xA4_164x->str_611.spellIndex_0x458_1112);
					world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].param2_0x6E3E_byte2 = 0;
				}
			}
			ComputeMousePlayerMovement_17060(v12x->playerData_0xA4_164x->position_backup_20.x, v12x->playerData_0xA4_164x->position_backup_20.y);
			LastPressedKey_1806E4 = 0;
			inputState_18058C.MouseButtonState_18059C &= 0xFC;
			FlyAssistant_1A7A0();
			goto LABEL_306;
		case 6:
		case 7:
			v24x = Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
			if (gameState_D41A4.OptionsSettingFlag_24 & GAME_PAUSED)
				ReadPauseMenuEvents_197F0();
			ProcessKeyboardPresses_17190();
			if (((inputState_18058C.MouseButtonState_18059C & 1 && inputState_18058C.MouseButtonState_18059C & 2) && !disableLRButtonsMenuOpen) || LastPressedKey_1806E4 == keys_EB39E[4] || v24x->life_0x8 < 0)
			{
				HandleButtonClick_191B0(20, 0); // Close Map
			}
			else if (LastPressedKey_1806E4 == 1)
			{
				ToggleSpellMenu_18B30();
				LastPressedKey_1806E4 = 0;
			}
			else
			{
				if (inputState_18058C.MouseButtonState_18059C & 0x10)
				{
					HandleButtonClick_191B0(20, 8);
				}
				else if (pressedKeys_180664[56] || pressedKeys_180664[keys_EB39E[9]] || pressedKeys_180664[keys_EB39E[6]])
				{
					if (inputState_18058C.MouseButtonState_18059C & 1)
					{
						CycleSelectedSpell_18DA0(v24x, 1, 1);
						inputState_18058C.MouseButtonState_18059C &= 0xFE;
					}
					else if (inputState_18058C.MouseButtonState_18059C & 2)
					{
						CycleSelectedSpell_18DA0(v24x, 2, 1);
						inputState_18058C.MouseButtonState_18059C &= 0xFD;
					}
					else if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 != 7)
					{
						HandleButtonClick_191B0(20, 7);
					}
				}
				else if (pressedKeys_180664[42] || pressedKeys_180664[keys_EB39E[8]])
				{
					if (inputState_18058C.MouseButtonState_18059C & 1)
					{
						CycleSelectedSpell_18DA0(v24x, 1, 0);
						inputState_18058C.MouseButtonState_18059C &= 0xFE;
					}
					else if (inputState_18058C.MouseButtonState_18059C & 2)
					{
						CycleSelectedSpell_18DA0(v24x, 2, 0);
						inputState_18058C.MouseButtonState_18059C &= 0xFD;
					}
				}
				else if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 != 6)
				{
					HandleButtonClick_191B0(20, 6);
				}
				HandleMouseButtons_18F80(v24x);
			}
			if (LastPressedKey_1806E4 == 0x20)
			{
				HandleOptionsMenuButtonClick_19A70();
				LastPressedKey_1806E4 = 0;
			}
			if (!pressedKeys_180664[47] || gameState_D41A4.volumeKeyHeld_38591)
			{
				if (pressedKeys_180664[50] && !gameState_D41A4.volumeKeyHeld_38591)
					ChangeSoundLevel_19CA0(2u);
			}
			else
			{
				ChangeSoundLevel_19CA0(1u);
			}
			if (LastPressedKey_1806E4 && keyToButton_E36DC[(unsigned __int8)LastPressedKey_1806E4] == 8)
			{
				HandleButtonClick_191B0(39, 1);
				LastPressedKey_1806E4 = 0;
			}
			HandleArrowKeyPresses_1A8A0();
			ComputeMousePlayerMovement_17060(inputState_18058C.mouse_1805B0.x, inputState_18058C.mouse_1805B0.y);
			LastPressedKey_1806E4 = 0;
			inputState_18058C.MouseButtonState_18059C &= 0xFC;
			FlyAssistant_1A7A0();
			goto LABEL_306;
		case 9:
		case 0xB:
			ReadOptionMenuEvents_19AB0();
			FlyAssistant_1A7A0();
			goto LABEL_306;
		case 0xA:
		case 0xC:
			AdjustVolume_1A070(turn);
			FlyAssistant_1A7A0();
			goto LABEL_306;
		case 0xD:
		case 0xE:
			ReadOkayCancelButtonEvents_19E00();
			goto LABEL_296;
		default:
		LABEL_296:
			FlyAssistant_1A7A0();
			goto LABEL_306;
		}
	}
	if (LastPressedKey_1806E4)
	{
		if (LastPressedKey_1806E4 >= 1u)
		{
			if (LastPressedKey_1806E4 <= 1u)
			{
				ToggleSpellMenu_18B30();
			}
			else if (LastPressedKey_1806E4 == 0x19)
			{
				if (!(gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE) && (!IsRecordingOrPlaying()))
					PauseUnpauseGame_18BB0();
			}
		}
		LastPressedKey_1806E4 = 0;
	}
LABEL_306:
	if (!(inputState_18058C.MouseButtonState_18059C & 1))
		mouseLeftButton_180746 = 0;
	if (!(inputState_18058C.MouseButtonState_18059C & 2))
		mouseRightButton_180744 = 0;
}

//----- (000191B0) --------------------------------------------------------
void HandleButtonClick_191B0(int16_t hiSetting, char loSetting)//1fa1b0 //set spell, destroy castle sub_191B0(0x2a, 0x00)
{
	switch (hiSetting)
	{
	case 0:
	case 1:
	case 2:
	case 26:
		world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0 = hiSetting;
		return;
	case 3:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
		return;
	case 4:
		if (world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0 != hiSetting && world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0)
			return;
		world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0 = hiSetting;
		world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].param1_0x6E3E_byte1 |= loSetting;
		return;
	case 5:
	case 6://set Player movement
		if (world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0 != hiSetting && world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0)
			return;
		world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0 = hiSetting;
		world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].entityIndex_0x6E3E_byte5 |= loSetting;
		return;
	case 12:
	case 13:
	case 14:
	case 16:
	case 18:
	case 19:
	case 29:
	case 42:
	case 44:
		if (world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0 != hiSetting && world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0)
			return;
		world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0 = hiSetting;
		return;
	case 15:
		if (world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0 != hiSetting && world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0)
			return;
		if (Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240]->life_0x8 >= 0 || Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240]->actionIndex_0x45_69 != 3)
			return;
		world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0 = hiSetting;
		return;
	case 27:
		if (world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0 != hiSetting && world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0)
			return;
		if (!(world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] & 2))
			return;
		world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0 = hiSetting;
		return;
	case 28:
		if (world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0 != hiSetting && world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0)
			return;
		if (!(world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] & 4))
			return;
		world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0 = hiSetting;
		return;
	case 30:
		if (gameState_D41A4.setting_byte2_23 < 0
			|| (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].WizardName_0x39f_2BFA_12157[0] == 'c')//cheat ?
			&& world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].WizardName_0x39f_2BFA_12157[1] == 'h'
			&& world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].WizardName_0x39f_2BFA_12157[2] == 'r'
			&& world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].WizardName_0x39f_2BFA_12157[3] == 'o'
			&& world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].WizardName_0x39f_2BFA_12157[4] == 'n'
			&& world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].WizardName_0x39f_2BFA_12157[5] == 'i'
			&& world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].WizardName_0x39f_2BFA_12157[6] == 'c'
			&& world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].WizardName_0x39f_2BFA_12157[7] == 'l'
			&& world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].WizardName_0x39f_2BFA_12157[8] == 'e')
		{
			if (world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0 == hiSetting || !world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0)
			{
				world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0 = hiSetting;
				world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].param1_0x6E3E_byte1 = loSetting;
				return;
			}
		}
		return;
	case 39:
		world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0 = hiSetting;
		world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].param1_0x6E3E_byte1 = loSetting;
		return;
	default:
		if (world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0 == hiSetting || !world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0)
		{
			world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0 = hiSetting;
			world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].param1_0x6E3E_byte1 = loSetting;
		}
		return;
	}
}

//----- (0001A970) --------------------------------------------------------
void ChangeSettings_1A970(char a1, int a2, int a3)//1fb970
{
	//int v3; // edx
	//char v4; // cl
	//char result; // al
	//char v6; // cl
	//bool v7; // zf
	//int v8; // eax
	//char v9; // cl
	//char v10; // bl
	//char v11; // ch
	//bool v12; // al
	type_entity_0x6E8E* v13x; // ebx
	unsigned __int8 v14; // al
	__int16 v15; // ST04_2
	//char v16; // dl
	MapType_t v17; // bl
	//char v18; // bl
	//char v19; // cl
	//char v20; // bl
	//int v21; // ebx
	//char v22; // cl
	//int v23; // ebx
	//char v24; // cl
	//bool v25; // dl
	//int v26; // ebx
	//char v27; // ch
	//char v28; // cl

	//v3 = a2;
	switch (a1)
	{
	case 1:
		SwitchHelpMode_18AA0();
		return;
	case 2:
		if (!soundAble_E3798)
			return;
		SetCurrentNotificationMessage_19760((char*)langStrings_E9C4C[(soundActive_E3799 != 0) + 390], 3u, 50);
		//Sound On / Sound Off

		EndSample_8D8F0();
		StopCdPlayback_86860(cdDrive_1803EC);
		soundActive_E3799 ^= 1u;
		return;
	case 3:
		if (!musicAble_E37FC)
			return;
		if (musicActive_E37FD)
		{
			StopMusic_8E020();
			SetCurrentNotificationMessage_19760((char*)langStrings_E9C4C[393], 3u, 50);//Music Off
			musicActive_E37FD = false;
		}
		else
		{
			v15 = world_D41A0.maptypeMusic_0x235;
			musicActive_E37FD = true;
			StartMusic_8E160(v15, 0x7Fu);
			SetCurrentNotificationMessage_19760(langStrings_E9C4C[392], 3u, 50);//Music On
		}
		return;
	case 4:
		if (!cdSpeechEnabled_E2A28)
			return;
		//v16 = gameState_D41A4.OptionsSettingFlag_24;
		if (gameState_D41A4.OptionsSettingFlag_24 & SPEECH_ENABLED)
			gameState_D41A4.OptionsSettingFlag_24 &= SPEECH_DISABLED;
		else
			gameState_D41A4.OptionsSettingFlag_24 |= SPEECH_ENABLED;
		SetCurrentNotificationMessage_19760(langStrings_E9C4C[((gameState_D41A4.OptionsSettingFlag_24 & SPEECH_ENABLED) == 0) + 469], 3u, 50);
		//	Speech On /	Speech Off
		return;
	case 5:
		if (gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
		{
			world_D41A0.flyAssistant_0x36DEA = 0;
		}
		else
		{
			//v12 = D41A0_BYTESTR_0.byte_0x36DEA_fly_asistant == 0;
			world_D41A0.flyAssistant_0x36DEA = !world_D41A0.flyAssistant_0x36DEA;
			SetCurrentNotificationMessage_19760(langStrings_E9C4C[!world_D41A0.flyAssistant_0x36DEA + 398], 3u, 50);
			// Flight Assistance On / Flight Assistance Off
		}
		return;
	case 6:
		if (gameState_D41A4.paletteMod_51 < 3u || gameState_D41A4.paletteSubMod_180)
			return;
		gameState_D41A4.setting_38402 = 1;
		v17 = world_D41A0.terrain_2FECE.MapType;
		if (v17 == MapType_t::Day)
		{
			if (a2)
			{
				// v3 = gameState_D41A4.dwordindex_0;
				// v19 = x_D41A0_BYTEARRAY_4[11] - 1;
				gameState_D41A4.brightness_11--; // = v19;
				if (gameState_D41A4.brightness_11 >= 0)
					goto LABEL_86;
				if (!a3)
				{
					gameState_D41A4.brightness_11 = 4;
					goto LABEL_86;
				}
			}
			else
			{
				// v3 = gameState_D41A4.dwordindex_0;
				// v18 = x_D41A0_BYTEARRAY_4[11] + 1;
				gameState_D41A4.brightness_11++; // = v18;
				if (gameState_D41A4.brightness_11 <= 4)
					goto LABEL_86;
				if (a3)
				{
					gameState_D41A4.brightness_11 = 4;
					goto LABEL_86;
				}
			}
			gameState_D41A4.brightness_11 = 0;
		LABEL_86:
			ApplyBrightnessToPalette_47650(a3 /*, a2*/);
			return;
		}
		if (v17 == MapType_t::Cave)
		{
			if (a2)//v17==1
			{
				//v23 = gameState_D41A4.dwordindex_0;
				//v24 = x_D41A0_BYTEARRAY_4[13] - 1;
				gameState_D41A4.brightness_13--;// = v24;
				if (gameState_D41A4.brightness_13 < 0)
				{
					if (!a3)
					{
						gameState_D41A4.brightness_13 = 4;
						ApplyBrightnessToPalette_47650(0/*, a2*/);
						return;
					}
					goto LABEL_85;
				}
			}
			else
			{
				//v23 = gameState_D41A4.dwordindex_0;
				//v3 = (unsigned __int8)(x_D41A0_BYTEARRAY_4[13] + 1);
				gameState_D41A4.brightness_13++;// = v3;
				if (gameState_D41A4.brightness_13 > 4)
				{
					if (a3)
					{
						gameState_D41A4.brightness_13 = 4;
						ApplyBrightnessToPalette_47650(a3/*, gameState_D41A4.brightness_13*/);
						return;
					}
				LABEL_85:
					gameState_D41A4.brightness_13 = 0;
					goto LABEL_86;
				}
			}
			goto LABEL_86;
		}
		if (a2)//v17==2
		{
			//v21 = gameState_D41A4.dwordindex_0;
			//v22 = D41A0_BYTESTR_0.word_0xc - 1;
			(gameState_D41A4.brightness_12)--;
			if (gameState_D41A4.brightness_12 >= 0)
				goto LABEL_86;
			if (a3)
			{
				gameState_D41A4.brightness_12 = 0;
				ApplyBrightnessToPalette_47650(a3/*, a2*/);
			}
			else
			{
				gameState_D41A4.brightness_12 = 4;
				ApplyBrightnessToPalette_47650(0/*, a2*/);
			}
			//result = 0;
		}
		else
		{
			//v3 = gameState_D41A4.dwordindex_0;
			//v20 = D41A0_BYTESTR_0.word_0xc + 1;
			(gameState_D41A4.brightness_12)++;// = v20;
			if (gameState_D41A4.brightness_12 <= 4)
				goto LABEL_86;
			if (a3)
			{
				gameState_D41A4.brightness_12 = 4;
				ApplyBrightnessToPalette_47650(a3/*, D41A0_BYTESTR_0.word_0xc*/);
			}
			else
			{
				gameState_D41A4.brightness_12 = 0;
				ApplyBrightnessToPalette_47650(0/*, D41A0_BYTESTR_0.word_0xc*/);
			}
			//result = 0;
		}
		return;// result;
	case 7:
		if (gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE || IsRecordingOrPlaying())
			return;
		v13x = Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
		if (v13x <= Entities_EA3E4[0] || v13x->life_0x8 < 0)
			return;
		if (CommandLineParams.DoAlternativeGamespeedControl()) {
			if (gameState_D41A4.speedIndex == 2)
				gameState_D41A4.speedIndex = 0;
			else
			{
				gameState_D41A4.speedIndex = gameState_D41A4.speedIndex + (a2 != 0) + 1;
				if (gameState_D41A4.speedIndex > 2)
				{
					gameState_D41A4.speedIndex = 2;
				}
			}
		}
		else {
			if (gameState_D41A4.speedIndex)
				gameState_D41A4.speedIndex = 0;
			else
				gameState_D41A4.speedIndex = (a2 != 0) + 1;
		}
		v14 = gameState_D41A4.speedIndex;
		if (v14 < 1u)
		{
			if (v14)
				return;
			SetCurrentNotificationMessage_19760((char*)langStrings_E9C4C[381], 3u, 50);//speed normal
			//result = 0;
		}
		else
		{
			if (v14 <= 1u)
			{
				SetCurrentNotificationMessage_19760((char*)langStrings_E9C4C[382], 3u, 50);//speed fast
			}
			else
			{
				if (v14 != 2)
					return;
				SetCurrentNotificationMessage_19760((char*)langStrings_E9C4C[383], 3u, 50);//speed super fast
			}
			v13x->playerData_0xA4_164x->roll_0x155_341 = 0;
			v13x->playerData_0xA4_164x->pitch_0x157_343 = 0;
			//result = 0;
		}
		return;// result;
	case 8:
		//v26 = (int)x_D41A0_BYTEARRAY_0;
		if (world_D41A0.m_GameSettings.m_Display.m_uiScreenSize)
			return;
		if (a2)
		{
			//v27 = *(x_BYTE *)(x_D41A0_BYTEARRAY_0 + 8589) + 1;
			world_D41A0.m_GameSettings.m_Graphics.m_wViewPortSize++;
			if (world_D41A0.m_GameSettings.m_Graphics.m_wViewPortSize <= 40)
				goto LABEL_97;
			if (!a3)
			{
				world_D41A0.m_GameSettings.m_Graphics.m_wViewPortSize = 16;
				goto LABEL_97;
			}
		}
		else
		{
			//v28 = *(x_BYTE *)(x_D41A0_BYTEARRAY_0 + 8589) - 1;
			world_D41A0.m_GameSettings.m_Graphics.m_wViewPortSize--;
			if (world_D41A0.m_GameSettings.m_Graphics.m_wViewPortSize >= 16)
				goto LABEL_97;
			if (a3)
			{
				world_D41A0.m_GameSettings.m_Graphics.m_wViewPortSize = 16;
				goto LABEL_97;
			}
		}
		world_D41A0.m_GameSettings.m_Graphics.m_wViewPortSize = 40;
	LABEL_97:
		viewPort.ResizeViewPort_2CA90(world_D41A0.m_GameSettings.m_Graphics.m_wViewPortSize);
		return;
	case 9:
		if (!world_D41A0.visualSettings_0x21AA.creflections_0x21AA)
			return;
		//v4 = *(x_BYTE *)(x_D41A0_BYTEARRAY_0 + 8586) ^ 1;
		world_D41A0.m_GameSettings.m_Graphics.m_wReflections ^= 1;
		SetCurrentNotificationMessage_19760(langStrings_E9C4C[-world_D41A0.m_GameSettings.m_Graphics.m_wReflections + 360], 3u, 50);
		//	Reflections On / Reflections Off
		return;
	case 10:
		if (!world_D41A0.visualSettings_0x21AA.csky_0x21AC || !skyTexture_D41A8)
			return;
		world_D41A0.m_GameSettings.m_Graphics.m_wSky ^= 1u;
		SetCurrentNotificationMessage_19760(langStrings_E9C4C[-world_D41A0.m_GameSettings.m_Graphics.m_wSky + 364], 3u, 50);
		// Sky On / Sky Off
		return;
	case 11:
		if (!world_D41A0.visualSettings_0x21AA.cshadows_0x21AB)
			return;
		//v6 = *(x_BYTE *)(x_D41A0_BYTEARRAY_0 + 8587) ^ 1;
		world_D41A0.m_GameSettings.m_Graphics.m_wShadows ^= 1;
		SetCurrentNotificationMessage_19760(langStrings_E9C4C[-world_D41A0.m_GameSettings.m_Graphics.m_wShadows + 358], 3u, 50);
		// Shadows On / Shadows Off
		return;
	case 12:
		if (!world_D41A0.detailSettings_0x21B6.clights_0x21B6)
			return;
		//v7 = *(x_BYTE *)(x_D41A0_BYTEARRAY_0 + 8598) == 1;
		world_D41A0.m_GameSettings.display_0x2196.m_wDynamicLighting ^= 1;
		SetCurrentNotificationMessage_19760(langStrings_E9C4C[-world_D41A0.m_GameSettings.display_0x2196.m_wDynamicLighting + 396], 3u, 50);
		//Light Sources On / Light Sources Off
		return;
	case 13:
		//v8 = (int)x_D41A0_BYTEARRAY_0;
		//v9 = *(x_BYTE *)(x_D41A0_BYTEARRAY_0 + 8591) ^ 1;
		world_D41A0.m_GameSettings.m_Display.m_wMiniMap ^= 1;
		world_D41A0.m_GameSettings.m_Display.m_wTopBar ^= 1;
		SetCurrentNotificationMessage_19760(langStrings_E9C4C[world_D41A0.m_GameSettings.m_Display.m_wMiniMap + 365], 3u, 50);
		// Icons and Map On / Icons and Map Off
		if (world_D41A0.m_GameSettings.m_Display.m_wMiniMap)
			return;
		CloseHelpPopup_87C10();
		return;
	case 14:
		if (!world_D41A0.detailSettings_0x21B6.ctransparency_0x21B8)
			return;
		//v10 = *(x_BYTE *)(x_D41A0_BYTEARRAY_0 + 8600) ^ 1;
		world_D41A0.m_GameSettings.display_0x2196.transparency_0x2198 ^= 1;
		SetCurrentNotificationMessage_19760(langStrings_E9C4C[world_D41A0.m_GameSettings.display_0x2196.transparency_0x2198 + 440], 3u, 50);
		// Panel Transparency On / Panel Transparency Off
		return;
	case 15:
		if (!world_D41A0.detailSettings_0x21B6.cflat_0x21B9)
			return;
		//v11 = *(x_BYTE *)(x_D41A0_BYTEARRAY_0 + 8601) ^ 1;
		world_D41A0.m_GameSettings.display_0x2196.flat_0x2199 ^= 1;
		SetCurrentNotificationMessage_19760(langStrings_E9C4C[-world_D41A0.m_GameSettings.display_0x2196.flat_0x2199 + 443], 3u, 50);
		// Flat Shading On / Flat Shading Off
		return;
	case 16://change resolution
		if (!world_D41A0.screenSettings_0x21B2.cresolution_0x21B4 || world_D41A0.m_GameSettings.m_Display.m_uiScreenSize)
			return;
		ClearPauseMenuState_41BC0();
		ChangeGameResolution_46B40();
		return;
	case 17:
		//v25 = gameState_D41A4.byteindex_207 == 0;
		gameState_D41A4.byteindex_207 ^= 1;
		SetCurrentNotificationMessage_19760(langStrings_E9C4C[gameState_D41A4.byteindex_207 + 402], 3u, 50);
		// Player Names On / Player Names Off
		return;
	case 18:
		//v7 = *(x_BYTE *)(x_D41A0_BYTEARRAY_0 + 8592) == 1;
		world_D41A0.m_GameSettings.m_Display.m_uiScreenSize ^= 1;
		if (!world_D41A0.m_GameSettings.m_Display.m_uiScreenSize)
		{
			InstallPaletteAndMouseMinMax_417A0();
			return;
		}
		InstallPaletteAndMouseMinMax2_417D0();
		return;
	case 19:
		if (m_ptrGameRender != nullptr && typeid(*m_ptrGameRender) == typeid(GameRenderNG))
		{
			GameRenderNG* render = (GameRenderNG*)m_ptrGameRender;
			const uint8_t numRenderThreads = render->GetRenderThreads() >= 7 ? 0 : render->GetRenderThreads() + 1;
			render->SetRenderThreads(numRenderThreads);
			const std::string message = numRenderThreads ? "Multi-thread render ON: Number of Threads: " + std::to_string(numRenderThreads) : "Multi-thread render OFF";
			SetCurrentNotificationMessage_19760(message.c_str(), 3u, 50);
		}
		if (m_ptrGameRender != nullptr && typeid(*m_ptrGameRender) == typeid(GameRenderHD))
		{
			uint8_t numRenderThreads = ((GameRenderHD*)m_ptrGameRender)->GetRenderThreads();
			if (numRenderThreads >= 7)
			{
				((GameRenderHD*)m_ptrGameRender)->SetRenderThreads(0);
				SetCurrentNotificationMessage_19760("Multi-thread render OFF", 3u, 50);
			}
			else
			{
				numRenderThreads++;
				((GameRenderHD*)m_ptrGameRender)->SetRenderThreads(numRenderThreads);
				std::string message = "Multi-thread render ON: Number of Threads: ";
				message += std::to_string(numRenderThreads);
				SetCurrentNotificationMessage_19760(message.c_str(), 3u, 50);
			}
		}
		return;
	case 20:
		switch(world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].ShowDebugTextFlag_0x005_2BE0_11235)
		{
			case 0x00:
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].ShowDebugTextFlag_0x005_2BE0_11235 = 0x08;
				break;
			case 0x08:
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].ShowDebugTextFlag_0x005_2BE0_11235 = 0x48;
				break;
			case 0x48:
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].ShowDebugTextFlag_0x005_2BE0_11235 = 0x40;
				break;
			case 0x40:
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].ShowDebugTextFlag_0x005_2BE0_11235 = 0x00;
				break;
		}
		return;
	default:
		return;
	}
}

//----- (00041AF0) --------------------------------------------------------
void SetPausedMenuOpen_41AF0()//222af0
{
	if (!IsPauseMenuOpen_D47D9)
	{
		if (world_D41A0.m_GameSettings.m_Display.m_uiScreenSize)
		{
			if (world_D41A0.m_GameSettings.graphics_0x2192.xxxx_0x2193 && iScreenWidth_DE560 == 640)
				ClearScreenAndBlit_75C50();
			savedScreenSize_D47D8 = world_D41A0.m_GameSettings.m_Display.m_uiScreenSize;
			InstallPaletteAndMouseMinMax_417A0();
			world_D41A0.m_GameSettings.m_Display.m_uiScreenSize = 0;
			VgaPaletteInstall_41A90((TColor*)*xadatapald0dat2.colorPalette_var28);
		}
	}
	IsPauseMenuOpen_D47D9++;
}

//----- (0005BF10) --------------------------------------------------------
char StopSoundAndMusic_5BF10()//23cf10
{
	char result; // al

	if (soundActive_E3799)
		EndSample_8D8F0();
	if (musicActive_E37FD)
		StopMusic_8E020();
	result = 0;
	musicActive_E37FD = false;
	soundActive_E3799 = false;
	return result;
}
// E3799: using guessed type char x_BYTE_E3799_sound_card;
// E37FD: using guessed type char x_BYTE_E37FD;

//----- (00070940) --------------------------------------------------------
void FindNearestEntity_70940()//251940
{
	type_entity_0x6E8E* v0x; // ebx
	int v1; // edi
	unsigned int result; // eax
	int v3; // esi
	int i; // eax
	char v5; // dl
	type_entity_0x6E8E* v6x; // eax
	int v7; // [esp+0h] [ebp-10h]
	int v8; // [esp+4h] [ebp-Ch]
	int v9; // [esp+8h] [ebp-8h]
	char v10; // [esp+Ch] [ebp-4h]

	v0x = Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
	v1 = (v0x->position_0x4C_76.x + 128) >> 8;
	v9 = (v0x->position_0x4C_76.y + 128) >> 8;
	result = AddE7EE0x_10080(0, 128);
	v3 = result;
	if (result)
	{
		v10 = 0;
		while (GetNextSearchOffset_10130(v3, &v8, &v7) == 1 && !v10)
		{
			for (i = mapEntityIndex_15B4E0[((unsigned __int8)(v9 + v7) << 8) + (unsigned __int8)(v1 + v8)];
				;
				i = v6x->oldMapEntity_0x16_22)
			{
				v6x = Entities_EA3E4[i];
				if (v6x == Entities_EA3E4[0] || v10)
					break;
				v5 = 1;
				if (v6x == v0x || v6x->class_0x3F_63 == 15 && v6x->id_0x1A_26 == v0x->id_0x1A_26 || v6x->flags_0xc_12_15.byte[0] & 1)
					v5 = 0;
				if (v5)
				{
					v10 = 1;
					gameState_D41A4.nearestEntity_38396 = v6x;
				}
			}
		}
		ResetEvent08_10100(v3);
	}
	//return result;
}

bitmap_pos_struct_t pathstr_to_posistr(Pathstruct in)
{
	bitmap_pos_struct_t out;
	out.data = *in.colorPalette_var28;
	out.width_4 = *in.colorPalette_var28[4];
	out.height_5 = *in.colorPalette_var28[5];
	return out;
};
//----- (0008CD27) --------------------------------------------------------
void SetCursor_8CD27(bitmap_pos_struct_t a2)//26dd27
{
	//a2
	// 446f1a00 80 52 35 00 e0 9e 2b
	// 1a6f44: 78651a00 00 00
	// 1a6578: 0b0005a1a1a1a1a10005

	//int result; // eax
	unsigned int i; // [esp+0h] [ebp-10h]
	int32_t v3; // [esp+4h] [ebp-Ch]
	//__int16 v4; // [esp+8h] [ebp-8h]
	//int v5; // [esp+Ch] [ebp-4h]
	cursorDrawn_E3758 = 1;//2b4758
	//v5 = (int)pdwScreenBuffer_351628;
	uint8_t* temp_screen_buffer = pdwScreenBuffer_351628;
	pdwScreenBuffer_351628 = cursorData_180730;//351730
	//cursorData_180730 = pdwScreenBuffer_351628;// hack
	if (a2.data)
	{
		cursorWidth_18072C = a2.width_4;
		cursorHeight_18072E = a2.height_5;
	}
	else
	{
		cursorWidth_18072C = 0;
		cursorHeight_18072E = 0;
	}
	if (videoMode_180660 & 1)
	{
		cursorWidth_18072C >>= 1;
		cursorHeight_18072E >>= 1;
	}
	savedViewportY_18065C = viewportY_180650;
	savedViewportHeight_180640 = viewportHeight_180630;
	savedViewportX_180638 = viewportX_18063C;
	savedViewportWidth_18064C = viewportWidth_180634;
	savedClipWidth_180658 = clipWidth_180648;
	savedClipHeight_180654 = clipHeight_180644;
	viewportY_180650 = 0;
	viewportHeight_180630 = 64;
	viewportX_18063C = 0;
	viewportWidth_180634 = 64;
	clipWidth_180648 = 64;
	clipHeight_180644 = 64;
	//v4 = drawFlags_E36D4;
	drawFlags_E36D4 = 0;
	v3 = screenWidth_18062C;
	screenWidth_18062C = 64;
	//cursorData_180730 = (uint8_t*)malloc(0x1000);//fix it
	for (i = 0; i < 0x1000; i++)
	{
		cursorData_180730[i] = 0xfe;
		//printf("%d",i);
	}
	if (a2.data)
		DrawBitmap320_8F8B0(0, 0, a2);//2708B0 super inicializace //a2 ma byt 86 nebo a1 nema nikam ukazovat
	pdwScreenBuffer_351628 = temp_screen_buffer;
	screenWidth_18062C = v3;
	//drawFlags_E36D4 = drawFlags_E36D4;//355230
	viewportY_180650 = savedViewportY_18065C;
	viewportHeight_180630 = savedViewportHeight_180640;
	viewportX_18063C = savedViewportX_180638;
	viewportWidth_180634 = savedViewportWidth_18064C;
	clipWidth_180648 = savedClipWidth_180658;
	clipHeight_180644 = savedClipHeight_180654;
	//result = v5;
	//pdwScreenBuffer_351628 = (uint8_t*)v5;
	cursorDrawn_E3758 = 0;

	//VgaBlit320_90478();//debug
	//return result;
}

//----- (000417D0) --------------------------------------------------------
void InstallPaletteAndMouseMinMax2_417D0()//2227d0
{
	ClearVgaMemory_906B4();
	VgaPaletteInstall_41A90((TColor*)*xadatapald0dat2.colorPalette_var28);
	SetMouseMinMax_6EF10(0, 640, 0, 800);
}

//----- (000197F0) --------------------------------------------------------
void ReadPauseMenuEvents_197F0()//1fa7f0
{
	char v1; // bl
	signed int v2; // ebx
	signed int v3; // edx
	unsigned __int8 v4; // al
	unsigned __int8 v5; // al
	int16_t width; // [esp+0h] [ebp-10h]
	int16_t height; // [esp+4h] [ebp-Ch]
	int16_t posX;
	int16_t posY; // [esp+Ch] [ebp-4h]

	uint8_t scale = 1;

	if (videoMode_180660 != 1)
		if (!DefaultResolutions())
		{
			scale = gameUiScale;
		}

	if (gameState_D41A4.OptionsSettingFlag_24 & GAME_PAUSED)
	{
		v1 = gameState_D41A4.byteindex_225;
		helpPopup_1804B0.pauseMenuItem_0xaa = -1;
		if (!v1 && !gameState_D41A4.volumeKeyHeld_38591)
		{
			if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 == 7)
			{
				SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]); //Set cursor to Null (Don't Draw)
			}
			else if (inputState_18058C.joystickMode_1805C2 == 7 || inputState_18058C.joystickMode_1805C2 == 1 || inputState_18058C.joystickMode_1805C2 == 2)
			{
				SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[cursorSpriteIndex_D419E]);
			}
			if (!gameState_D41A4.pauseState_206)
			{
				GetPauseMenuCoordinates_2FFE0(&posX, &posY, &width, &height, scale);
				if (inputState_18058C.mouse_1805B0.x >= posX && posX + width > inputState_18058C.mouse_1805B0.x)
				{
					v2 = -1;
					v3 = 0;
					while (v3 < 4 && v2 < 0)
					{
						if (posY <= inputState_18058C.mouse_1805B0.y && posY + height > inputState_18058C.mouse_1805B0.y)
							v2 = v3;
						v3++;
						posY += height;
					}
					if (v2 >= 0)
					{
						if (v2 != 2 || posX + width / 2 > inputState_18058C.mouse_1805B0.x)
						{
							if (v2 > 2)
								v2++;
						}
						else
						{
							v2 = 3;
						}
						helpPopup_1804B0.pauseMenuItem_0xaa = v2;
						if (inputState_18058C.MouseButtonState_18059C & 1 || inputState_18058C.MouseButtonState_18059C & 2)
						{
							switch (v2)
							{
							case 0:
								v4 = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221;
								if (v4 < 6u || v4 > 8u)
									HandleButtonClick_191B0(20, 13);
								else
									HandleButtonClick_191B0(20, 14);
								gameState_D41A4.SelectedMenuItem_38546 = 3;
								ClearMouseButtonState_19A50();
								return;
							case 1:
								if (!gameState_D41A4.saveFileExists_208)
									break;
								v5 = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221;
								if (v5 < 6u || v5 > 8u)
									HandleButtonClick_191B0(20, 13);
								else
									HandleButtonClick_191B0(20, 14);
								gameState_D41A4.SelectedMenuItem_38546 = 2;
								ClearMouseButtonState_19A50();
								return;
							case 2:
								ChangeSoundLevel_19CA0(1u);
								ClearMouseButtonState_19A50();
								return;
							case 3:
								ChangeSoundLevel_19CA0(2u);
								ClearMouseButtonState_19A50();
								return;
							case 4:
								HandleOptionsMenuButtonClick_19A70();
								break;
							default:
								break;
							}
						}
					}
				}
			}
			ClearMouseButtonState_19A50();
		}
	}
}

//----- (00018DA0) --------------------------------------------------------
char CycleSelectedSpell_18DA0(type_entity_0x6E8E* a1x, char a2, char a3)//1f9da0
{
	type_str_611* v3x; // edx
	signed int v4; // esi
	signed int v5; // eax
	int v6; // eax
	int v7; // ebx
	//int v8; // edi
	//int v9; // edx
	signed int v10; // ebx
	signed int v11; // esi
	signed int v12; // eax
	int v13; // ebx
	//int v14; // edi
	//int v15; // edx
	signed int v16; // ebx

	v3x = &a1x->playerData_0xA4_164x->str_611;
	if (a2 == 2)
	{
		v4 = 0;
		if (a3)
			v5 = -1;
		else
			v5 = 1;
		v6 = a1x->playerData_0xA4_164x->str_611.SpellIndexRight_0x453_1107 + v5;
		while (1)
		{
			if (a3 || v6 < 26)
			{
				if (a3 == 1 && v6 < 0)
					v6 = 25;
			}
			else
			{
				v6 = 0;
			}
			v7 = spellIndex_D94FF[v6];
			if (v3x->SpellsEnabled_0x333_819x.SpellEnabled[v7])
			{
				//v8 = v3 + v7;
				if (v3x->spellSlotTypes_0x3B5_949x.SpellIndex[v7] == 2)
					break;
			}
			if (a3)
				v10 = -1;
			else
				v10 = 1;
			v4++;
			v6 += v10;
			if (v4 >= 26)
				return v6;
		}
		a1x->playerData_0xA4_164x->str_611.byte_0x45B_1115 = v6;
		//v9 = (int)x_D41A0_BYTEARRAY_0;
		world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0 = 32;
		world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].param1_0x6E3E_byte1 = v7;
		//LOBYTE(v6) = v3x->array_0x437_1079[v7]; //*(x_BYTE *)(v8 + 468);
		world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].param2_0x6E3E_byte2 = v3x->selectedSubSpells_0x437_1079x.SpellIndex[v7]; //*(x_BYTE *)(v8 + 468);
	}
	else
	{
		v11 = 0;
		if (a3)
			v12 = -1;
		else
			v12 = 1;
		v6 = a1x->playerData_0xA4_164x->str_611.SpellIndexLeft_0x451_1105 + v12;
		while (1)
		{
			if (a3 || v6 < 26)
			{
				if (a3 == 1 && v6 < 0)
					v6 = 25;
			}
			else
			{
				v6 = 0;
			}
			v13 = spellIndex_D94FF[v6];
			if (v3x->SpellsEnabled_0x333_819x.SpellEnabled[v13])
			{
				//v14 = v3 + v13;
				if (v3x->spellSlotTypes_0x3B5_949x.SpellIndex[v13] == 1)
					break;
			}
			if (a3)
				v16 = -1;
			else
				v16 = 1;
			v11++;
			v6 += v16;
			if (v11 >= 26)
				return v6;
		}
		a1x->playerData_0xA4_164x->str_611.byte_0x45A_1114 = v6;
		//v15 = (int)x_D41A0_BYTEARRAY_0;
		world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0 = 31;
		world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].param1_0x6E3E_byte1 = v13;
		//LOBYTE(v6) = *(x_BYTE *)(v14 + 468);//v3x->str_611.array_0x437_1079[v13]
		world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].param2_0x6E3E_byte2 = v3x->selectedSubSpells_0x437_1079x.SpellIndex[v13];
	}
	return v6;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (00018B30) --------------------------------------------------------
void ToggleSpellMenu_18B30()//1f9b30
{
	if (!(world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] & 0x20))
	{
		if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 != 13 && world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 != 14)
		{
			if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 < 6u || world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 > 8u && (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 < 0xBu || world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 > 0xCu))
				HandleButtonClick_191B0(20, 13);
			else
				HandleButtonClick_191B0(20, 14);
			gameState_D41A4.SelectedMenuItem_38546 = 1;
		}
		else
		{
			HandleButtonClick_191B0(29, 0);
			HandleButtonClick_191B0(27, 0);
		}
	}
}

int debugcounter_1fb7a0 = 0;
//----- (0001A7A0) --------------------------------------------------------
void FlyAssistant_1A7A0()//1fb7a0 // fly asistant
{
	if (CommandLineParams.DoFixMouse()) {
		if (CommandLineParams.ModeRegressionsTestType()!=-1)
		{
			inputState_18058C.MouseButtonState_18059C = 0;
			mouseLeftButton2_18074C = 0;
			if (debugcounter_1fb7a0 < 1000) {
				inputState_18058C.mouseX_1805B8 = 0x128;
				inputState_18058C.mouseY_1805BC = 0x7e;
			}
			debugcounter_1fb7a0++;
		}
	}

	if (CommandLineParams.DoFixFlyasistant()) {
		if (gameState_D41A4.OptionsSettingFlag_24 & GAME_PAUSED)
			return;
	}

	if (world_D41A0.flyAssistant_0x36DEA)
	{
		if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 != 5 && world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 != 8 && world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 != 3 && !gameState_D41A4.byteindex_225)
		{
			if (world_D41A0.lastMouseX_0x36DEC != inputState_18058C.mouseX_1805B8
				|| world_D41A0.lastMouseY_0x36DEE != inputState_18058C.mouseY_1805BC
				|| world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].PlayerAction_byte0
				|| world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].entityIndex_0x6E3E_byte5)
			{
				;
			}
			else
			{
				world_D41A0.mouseIdleCounter_0x36DF0++;
				if (world_D41A0.mouseIdleCounter_0x36DF0 <= 0x30)
				{
					world_D41A0.lastMouseX_0x36DEC = inputState_18058C.mouseX_1805B8;
					world_D41A0.lastMouseY_0x36DEE = inputState_18058C.mouseY_1805BC;
					return;
				}
				HandleButtonClick_191B0(39, 0);
			}
			world_D41A0.mouseIdleCounter_0x36DF0 = 0;
			world_D41A0.lastMouseX_0x36DEC = inputState_18058C.mouseX_1805B8;
			world_D41A0.lastMouseY_0x36DEE = inputState_18058C.mouseY_1805BC;
		}
	}
}

//----- (00019A70) --------------------------------------------------------
void HandleOptionsMenuButtonClick_19A70()//1faa70
{
	if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 < 6u || world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 > 8u)
		HandleButtonClick_191B0(20, 9);
	else
		HandleButtonClick_191B0(20, 11);
}

//----- (00018F80) --------------------------------------------------------
void HandleMouseButtons_18F80(type_entity_0x6E8E* a1x)//1f9f80
{
	if (inputState_18058C.arrowKeys_1805C0 & 1)
		HandleButtonClick_191B0(6, 1);
	if (inputState_18058C.arrowKeys_1805C0 & 2)
		HandleButtonClick_191B0(6, 2);
	if (inputState_18058C.arrowKeys_1805C0 & 4)
		HandleButtonClick_191B0(6, 4);
	if (inputState_18058C.arrowKeys_1805C0 & 8)
		HandleButtonClick_191B0(6, 8);
	if (a1x->playerData_0xA4_164x->str_611.SpellIndexLeft_0x451_1105 == -1)
	{
		inputState_18058C.MouseButtonState_18059C &= 0xFE;
	}
	else
	{
		if (Entities_EA3E4[a1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[a1x->playerData_0xA4_164x->str_611.SpellIndexLeft_0x451_1105]]->poseIndex_0x3B_59 == 1)
		{
			if (inputState_18058C.MouseButtonState_18059C & 1) //fire left
			{
				HandleButtonClick_191B0(6, 16);
				inputState_18058C.MouseButtonState_18059C &= 0xFE;
			}
		}
		else if (inputState_18058C.MouseButtonState_18059C & 1 || inputState_18058C.MouseButtonState_18059C & 4 && Entities_EA3E4[a1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[a1x->playerData_0xA4_164x->str_611.SpellIndexLeft_0x451_1105]]->stage_0x2E_46 > 0)
		{
			HandleButtonClick_191B0(6, 16);
			inputState_18058C.MouseButtonState_18059C &= 0xFE;
		}
	}
	if (a1x->playerData_0xA4_164x->str_611.SpellIndexRight_0x453_1107 == -1)
	{
		inputState_18058C.MouseButtonState_18059C &= 0xFD;
	}
	else
	{
		if (Entities_EA3E4[a1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[a1x->playerData_0xA4_164x->str_611.SpellIndexRight_0x453_1107]]->poseIndex_0x3B_59 == 1)
		{
			if (inputState_18058C.MouseButtonState_18059C & 2) //fire right
			{
				HandleButtonClick_191B0(6, 32);
				inputState_18058C.MouseButtonState_18059C &= 0xFD;
			}
		}
		else if (inputState_18058C.MouseButtonState_18059C & 2 || inputState_18058C.MouseButtonState_18059C & 8 && Entities_EA3E4[a1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[a1x->playerData_0xA4_164x->str_611.SpellIndexRight_0x453_1107]]->stage_0x2E_46 > 0)
		{
			HandleButtonClick_191B0(6, 32);
			inputState_18058C.MouseButtonState_18059C &= 0xFD;
		}
	}
}

//----- (0001A8A0) --------------------------------------------------------
void HandleArrowKeyPresses_1A8A0()//1fb8a0
{
	if (inputState_18058C.arrowKeys_1805C0 & 4)
	{
		if (inputState_18058C.arrowKeys_1805C0 & 8)
		{
			if (!(gameState_D41A4.arrowKeyFlags_183 & 1) && !(gameState_D41A4.arrowKeyFlags_183 & 2))
				HandleButtonClick_191B0(6, 128);
		}
	}
	gameState_D41A4.arrowKeyFlags_183 &= 0xFC;
	if (inputState_18058C.arrowKeys_1805C0 & 4)
		gameState_D41A4.arrowKeyFlags_183 |= 1;
	if (inputState_18058C.arrowKeys_1805C0 & 8)
	{
		gameState_D41A4.arrowKeyFlags_183 |= 2;
	}
}

//----- (00017060) --------------------------------------------------------
void ComputeMousePlayerMovement_17060(int16_t x, int16_t y)//1f8060
{
	int roll; // ebx
	int pitch; // ecx

	if (CommandLineParams.DoMouseOff2()) {
		x = 0x140;
		y = 0xc8;
	}

	if (!gameState_D41A4.speedIndex)
	{
		if (videoMode_180660 == 1)
		{
			roll = ((x << 7) - 40960) / 320;
			pitch = ((y << 7) - 25600) / -200;
		}
		else
		{
			const int centerX = screenWidth_18062C / 2;//640x480: 320, 240 as the original
			const int centerY = screenHeight_180624 / 2;
			roll = ((x << 7) - (centerX << 7)) / centerX;//1f80c5: lea edx, [eax-0A000h]
			pitch = ((y << 7) - (centerY << 7)) / -centerY;//1f80df: lea edx, [eax-7800h]
		}
		if (roll < -127)
			roll = -127;
		if (roll > 127)
			roll = 127;
		if (pitch < -127)
			pitch = -127;
		if (pitch > 127)
			pitch = 127;

		if (!invertYAxis)
			pitch = pitch * -1;

		if (invertXAxis)
			roll = roll * -1;

		world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].roll = roll;
		world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].pitch = pitch;
		world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].nextEntity_0x6E3E_word6 = inputState_18058C.joyAxisX_180590;
		world_D41A0.playerInputs_0x6E3E[world_D41A0.LevelIndex_0xc].entityIndex2_0x6E3E_word8 = inputState_18058C.joyAxisY_180594;
	}
}

//----- (00018AA0) --------------------------------------------------------
void SwitchHelpMode_18AA0()//1f9aa0
{
	bool switchHelpMode = true;
	if (gameState_D41A4.showHelp_10)
	{
		gameState_D41A4.showHelp_10 = 0;
	}
	else
	{
		SetTextBoxMinMaxSizes_87090();
		if (helpPopup_1804B0.initialized_0xa2)
		{
			gameState_D41A4.showHelp_10 = 1;
		}
		else
		{
			switchHelpMode = false;
			SetCurrentNotificationMessage_19760("No Help Data", 3u, 50);
		}
	}
	if (switchHelpMode)
	{
		helpPopup_1804B0.pauseMenuItem_0xaa = -1;
		gameState_D41A4.setting_38402 = 1;
		SetCurrentNotificationMessage_19760((char*)langStrings_E9C4C[(gameState_D41A4.showHelp_10 == 0) + 400], 3u, 50);
		//Help Mode On / Help Mode Off
	}
}

//----- (0006D4F0) --------------------------------------------------------
int SelectSpell_6D4F0(type_str_611* a1x, int16_t mouseX)//24e4f0
{
	uint16_t subCategoryTotalWidth; // cx
	int spellMenuXPos; // esi
	int16_t spellMenuXPos16;
	int subCategoryIdx; // eax
	int16_t maxIdx;

	int16_t posXOffSet = 0;
	uint8_t scale = 1;

	if (videoMode_180660 != 1)
		if (!DefaultResolutions())
		{
			scale = gameUiScale;
			posXOffSet = ((screenWidth_18062C - (640 * scale)) / 2);
		}

	subCategoryTotalWidth = 3 * ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL2].width_4 * scale);
	spellMenuXPos = ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL].width_4 * scale) / 2
		+ ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[EDGE_PANEL].width_4 * scale)
		+ ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL].width_4 * scale) * (a1x->spellIndex_0x458_1112 % 13)
		- subCategoryTotalWidth / 2;

	spellMenuXPos16 = ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL].width_4 * scale) / 2
		+ ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[EDGE_PANEL].width_4 * scale)
		+ ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL].width_4 * scale) * (a1x->spellIndex_0x458_1112 % 13)
		- subCategoryTotalWidth / 2;

	if (spellMenuXPos <= (640 * scale) - subCategoryTotalWidth)
	{
		if ((spellMenuXPos & 0x8000u) != 0)
			spellMenuXPos16 = spellMenuXPos ^ spellMenuXPos16;
	}
	else
	{
		spellMenuXPos16 = (640 * scale) - subCategoryTotalWidth;
	}
	subCategoryIdx = ((mouseX - posXOffSet) - spellMenuXPos16) / ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[163].width_4 * scale);
	maxIdx = a1x->SpellLevels_0x41D_1053z.SpellIndex[spellIndex_D94FF[a1x->spellIndex_0x458_1112]];

	if ((signed __int16)subCategoryIdx > maxIdx)
		return maxIdx;

	if ((subCategoryIdx & 0x8000u) != 0)
		subCategoryIdx = 0;

	return subCategoryIdx;
}

//----- (0006D420) --------------------------------------------------------
int SelectSpellCategory_6D420(int16_t posX, int16_t posY)//24e420
{
	int16_t posXOffSet = 0;
	int spellPosY; // ebx
	int spellPosX; // eax
	uint8_t scale = 1;
	int16_t spellMenuPosY;

	if (videoMode_180660 & 1)
		spellMenuPosY = 400;
	else
		spellMenuPosY = 480;
	if (videoMode_180660 != 1)
		if (!DefaultResolutions())
		{
			spellMenuPosY = screenHeight_180624;
			if (screenWidth_18062C > 640)
			{
				scale = gameUiScale;
				posXOffSet = (screenWidth_18062C - (640 * scale)) / 2;
			}
		}

	spellPosY = (posY - (spellMenuPosY - 2 * (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL].height_5 * scale)) / ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL].height_5 * scale);
	spellPosX = ((posX - posXOffSet) - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[EDGE_PANEL].width_4 * scale) / ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL].width_4 * scale);
	if ((spellPosY & 0x8000u) == 0)
	{
		if (spellPosY > 1)
			spellPosY = 1;
	}
	else
	{
		spellPosY = 0;
	}
	if ((spellPosX & 0x8000u) == 0)
	{
		if (spellPosX > 12)
			spellPosX = 12;
	}
	else
	{
		spellPosX = 0;
	}
	return 13 * spellPosY + spellPosX;
}

//----- (0006D4C0) --------------------------------------------------------
char UpdateSubSpellIndex_6D4C0(type_str_611* a1x)//24e4c0
{
	a1x->subSpellIndex_0x459_1113 = a1x->selectedSubSpells_0x437_1079x.SpellIndex[spellIndex_D94FF[a1x->spellIndex_0x458_1112]];
	return a1x->subSpellIndex_0x459_1113;
}

//----- (00019AB0) --------------------------------------------------------
void ReadOptionMenuEvents_19AB0()//1faab0
{
	int okayBtnPosX; // eax
	int okayBtnPosY = 377;
	int menuBtnPosX; // edx
	int menuBtnPosY = 67; // eax
	int helpWidth = 640;
	uint8_t scale = 1;

	if (videoMode_180660 != 1)
		if (!DefaultResolutions())
		{
			helpWidth = screenWidth_18062C;
			scale = gameUiScale;
			menuBtnPosY = (screenHeight_180624 / 2) - ((17 * (OPTIONS_MENU_BTN_HEIGHT * scale)) / 2);
			okayBtnPosY = menuBtnPosY + ((OPTIONS_MENU_BTN_HEIGHT * scale) * 17) + 4;
		}

	if (inputState_18058C.joystickMode_1805C2 == 7 || inputState_18058C.joystickMode_1805C2 == 1 || inputState_18058C.joystickMode_1805C2 == 2)
		SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[cursorSpriteIndex_D419E]); //fix it
	if (LastPressedKey_1806E4 == 1)
	{
		ToggleSpellMenu_18B30();
		LastPressedKey_1806E4 = 0;
		ClearMouseButtonState_19A50();
		return;
	}
	if (!(inputState_18058C.MouseButtonState_18059C & 1) && !(inputState_18058C.MouseButtonState_18059C & 2) && LastPressedKey_1806E4 != 0x1c && LastPressedKey_1806E4 != 0x20)
	{
		ClearMouseButtonState_19A50();
		return;
	}

	//Ok button
	okayBtnPosX = (helpWidth - (gameState_D41A4.optionsMenuWidth_186 * scale)) / 2 + ((gameState_D41A4.optionsMenuWidth_186 * scale) - (OPTIONS_MENU_BTN_WIDTH * scale)) / 2;
	if (okayBtnPosX <= inputState_18058C.mouse_1805B0.x && okayBtnPosX + (OPTIONS_MENU_BTN_WIDTH * scale) > inputState_18058C.mouse_1805B0.x && inputState_18058C.mouse_1805B0.y >= okayBtnPosY && inputState_18058C.mouse_1805B0.y < (okayBtnPosY + (OPTIONS_MENU_BTN_HEIGHT * scale))
		|| LastPressedKey_1806E4 == 0x1c
		|| LastPressedKey_1806E4 == 0x20)
	{
		gameState_D41A4.byteindex_225 = 0;
		if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 == 9)
			HandleButtonClick_191B0(20, 0);
		else
			HandleButtonClick_191B0(20, 6);
		SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]); //Set cursor to Null (Don't Draw)
		LastPressedKey_1806E4 = 0;
		ClearMouseButtonState_19A50();
		return;
	}

	int btnIndex = 0;
	menuBtnPosX = (helpWidth - (gameState_D41A4.optionsMenuWidth_186 * scale)) / 2;

	//Menu Options
	do
	{
		if (menuBtnPosX <= inputState_18058C.mouse_1805B0.x
			&& menuBtnPosX + (gameState_D41A4.optionsMenuWidth_186 * scale) > inputState_18058C.mouse_1805B0.x
			&& menuBtnPosY <= inputState_18058C.mouse_1805B0.y
			&& menuBtnPosY + (OPTIONS_MENU_BTN_HEIGHT * scale) > inputState_18058C.mouse_1805B0.y)
		{
			ChangeSettings_1A970(btnIndex + 1, (inputState_18058C.MouseButtonState_18059C & 1) == 0, 1);
			SetOptionsMenuFlags_1A280();
			ClearMouseButtonState_19A50();
			return;
		}
		btnIndex++;
		menuBtnPosY += (OPTIONS_MENU_BTN_HEIGHT * scale);
	} while (btnIndex < OPTIONS_MENU_BTN_COUNT);

	ClearMouseButtonState_19A50();

	return;
}

//----- (00019E00) --------------------------------------------------------
void ReadOkayCancelButtonEvents_19E00()//1fae00
{
	int16_t okButtonY;
	int16_t okButtonX;
	bool pressed_enter = false;
	bool pressed_esc = false;
	bool selectSpell = false;

	uint8_t scale = 1;

	if (!DefaultResolutions())
	{
		scale = gameUiScale;
	}

	GetOkayCancelButtonPositions_30BE0(&okButtonX, &okButtonY, scale);
	if (LastPressedKey_1806E4)
	{
		switch (LastPressedKey_1806E4) {
			case 0x01:
			{
				pressed_esc = true;
				break;
			}
			case 0x1c:
			{
				pressed_enter = true;
				break;
			}
		}
		LastPressedKey_1806E4 = 0;
	}
	else if (inputState_18058C.MouseButtonState_18059C & 1 || inputState_18058C.MouseButtonState_18059C & 2)
	{
		if (okButtonX > inputState_18058C.mouse_1805B0.x || okButtonX + (50 * scale) <= inputState_18058C.mouse_1805B0.x || okButtonY > inputState_18058C.mouse_1805B0.y || okButtonY + (32 * scale) <= inputState_18058C.mouse_1805B0.y)
		{
			if (okButtonX + (50 * scale) <= inputState_18058C.mouse_1805B0.x && okButtonX + (100 * scale) > inputState_18058C.mouse_1805B0.x && okButtonY <= inputState_18058C.mouse_1805B0.y && okButtonY + (32 * scale) > inputState_18058C.mouse_1805B0.y)
				pressed_esc = true;
		}
		else
		{
			pressed_enter = true;
		}
		ClearMouseButtonState_19A50();
	}
	if (!pressed_enter)
		selectSpell = true;
	else
	{
		switch (gameState_D41A4.SelectedMenuItem_38546)
		{
		case 1:
			ToggleSpellMenu_18B30();
			break;
		case 2:
			if (gameState_D41A4.saveFileExists_208)
			{
				if (LoadLevel_555D0(0, gameState_D41A4.levelnumber_43w))
					sprintf(printbuffer, "%s:%s.", langStrings_E9C4C[423], "OK");//Load Level
				else
					sprintf(printbuffer, "%s:%s.", langStrings_E9C4C[423], langStrings_E9C4C[429]);//429 - Failed
				ShowMessage_52D70(0, printbuffer);
			}
			selectSpell = true;
			break;
		case 3:
			if (SaveLevel_55080(0, gameState_D41A4.levelnumber_43w, (char*)""))
				sprintf(printbuffer, "%s:%s.", langStrings_E9C4C[424], "OK");//Save Level
			else
				sprintf(printbuffer, "%s:%s.", langStrings_E9C4C[424], langStrings_E9C4C[429]);//429 - Failed
			ShowMessage_52D70(0, printbuffer);
			selectSpell = true;
			gameState_D41A4.saveFileExists_208 = DataFileIO::TestSaveFile2_55C00(gameState_D41A4.levelnumber_43w);
			break;
		}
	}
	if (pressed_enter || pressed_esc)
	{
		gameState_D41A4.SelectedMenuItem_38546 = 0;
		if (selectSpell)
			HandleButtonClick_191B0(20, gameState_D41A4.savedMenuState_38544);
	}
}

//----- (00019760) --------------------------------------------------------
void SetCurrentNotificationMessage_19760(const char* message, unsigned __int8 a2, __int16 duration)//1fa760
{
	int maxLength = sizeof(world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].CurrentNotificationText_0x01c_2BFA_11258);

	//char *v3; // esi
	//char *v4; // edi
	//char v5; // al
	//char v6; // al
	//int result; // eax

	//v3 = (char*)a1;
	//D41A0_BYTESTR_0.array_0x2BDE[D41A0_BYTESTR_0.word_0xc].CurrentNotificationText_0x01c_2BFA_11258
	//v4 = (char*)&x_D41A0_BYTEARRAY_0[2124 * D41A0_BYTESTR_0.word_0xc + 11230 + 28];

	if (strlen(message) < maxLength)
		strcpy(world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].CurrentNotificationText_0x01c_2BFA_11258, message);
	else
	{
		strncpy(world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].CurrentNotificationText_0x01c_2BFA_11258, message, maxLength - 1);
		world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].CurrentNotificationText_0x01c_2BFA_11258[48] = '\0';
	}

	/*do
	{
		v5 = *v3;
		*v4 = *v3;
		if (!v5)
			break;
		v6 = v3[1];
		v3 += 2;
		v4[1] = v6;
		v4 += 2;
	} while (v6);*/
	//result = (int)x_D41A0_BYTEARRAY_0;
	world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].CurrentNotificationDuration_0x04d_2C2B_11307 = duration;
	world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].endState_0x04f_2C2D_11309 = a2;
	//	return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (00047650) --------------------------------------------------------
void ApplyBrightnessToPalette_47650(int  /*a1*//*, int a2*/)//228650
{
	MapType_t v2; // al
	signed int v3; // esi
	int v4; // edx
	signed int v5; // ebx
	int v6; // ecx
	int v7; // eax
	//int result; // eax

	int v8 = 0;

	qmemcpy((void*)*xadatapald0dat2.colorPalette_var28, (void*)tempPalette_EA3B8x, 0x300u);
	v2 = world_D41A0.terrain_2FECE.MapType;
	if (v2 == MapType_t::Day)
		v8 = gameState_D41A4.brightness_11;
	else if (v2 == MapType_t::Night)
	{
		v8 = gameState_D41A4.brightness_12;
	}
	else if (v2 == MapType_t::Cave)
	{
		v8 = gameState_D41A4.brightness_13;
	}
	v3 = 0;
	v4 = 6 * v8;
	while (v3 < 256)
	{
		v5 = 0;
		while (v5 < 3)
		{
			v6 = v4 + *(*xadatapald0dat2.colorPalette_var28 + v5 + 3 * v3);
			if (v6 < 0)
				v6 = 0;
			if (v6 > 63)
				LOBYTE(v6) = 63;
			v7 = v5++ + 3 * v3;
			*(x_BYTE*)(*xadatapald0dat2.colorPalette_var28 + v7) = v6;
		}
		v3++;
	}
	//result = (int)x_D41A0_BYTEARRAY_4;
	gameState_D41A4.paletteSubMod_180 = 1;
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3B8: using guessed type int x_DWORD_EA3B8;
// EA3D8: using guessed type int *xadatapald0dat2.colorPalette_var28;

//----- (00046B40) --------------------------------------------------------
void ChangeGameResolution_46B40()//227b40
{
	char v1; // al
	unsigned __int8 v2; // al
	unsigned __int8 v3; // al

	int16_t v0_tempmousex = mouse_E3760.x;//227b46: mov si, word_E3760
	int16_t v0_tempmousey = mouse_E3760.y;//227b53: mov bx, word_E3762

	VgaPalFadeInFadeOut_90B27(0, 0x10u, 0);
	InstallPaletteAndMouseMinMax_417A0();
	memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 768);
	VgaPaletteInstall_41A90((TColor*)*xadatapald0dat2.colorPalette_var28);

	if (memoryBufferAlloc_E9C3C)
	{
		FreeMem_83E80(memoryBufferAlloc_E9C3C);
		memoryBufferAlloc_E9C3C = 0;
		ptrMemoryBuffer_E9C3C = 0;
	}

	MouseReset_54600();

	CreateFileArrayIndexes_6EBF0(&filearray_2aa18c[filearrayindex_POINTERSDATTAB]);
	CreateFileArrayIndexes_6EBF0(&filearray_2aa18c[filearrayindex_MSPRD00DATTAB]);
	CreateFileArrayIndexes_6EBF0(&filearray_2aa18c[filearrayindex_BUILD00DATTAB]);

	FreeWebSprites_47130();

	//Change resolution
	if (videoMode_180660 == 1)
	{
		CreateFileArrayIndexes_6EBF0(&filearray_2aa18c[filearrayindex_FONTS0DATTAB]);
		DataFileIO::UnloadFileArray_90D3F(psxadatamsprd00dat);
		videoMode_180660 = 8;
	}
	else
	{
		CreateFileArrayIndexes_6EBF0(&filearray_2aa18c[filearrayindex_HFONT3DATTAB]);
		DataFileIO::UnloadFileArray_90D3F(psxadatahsprd00dat);
		videoMode_180660 = 1;
	}
	LoadSpr_47160();

	CreateIndexes_6EB90(&filearray_2aa18c[filearrayindex_POINTERSDATTAB]);
	CreateIndexes_6EB90(&filearray_2aa18c[filearrayindex_BUILD00DATTAB]);

	if (extendedFonts)
		LoadFixedFonts(3, (char*)"FONT1");

	memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 768);
	if (videoMode_180660 & 1)
	{
		v1 = GetPaletteIndex_5BE80((TColor*)*xadatapald0dat2.colorPalette_var28, 0, 0, 0);
		ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, v1);
		ClearScreenAlphaBuffer(320, 200, 255);
	}
	else
	{
		v2 = GetPaletteIndex_5BE80((TColor*)*xadatapald0dat2.colorPalette_var28, 0, 0, 0);
		ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, screenWidth_18062C, screenHeight_180624, v2);
		ClearScreenAlphaBuffer(screenWidth_18062C, screenHeight_180624, 255);
	}
	memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 768);
	VgaPaletteInstall_41A90((TColor*)*xadatapald0dat2.colorPalette_var28);
	if (videoMode_180660 & 1)
		VgaSetVideoMode320x200AndPalette_90D6E((TColor*)*xadatapald0dat2.colorPalette_var28);
	else
	{
		if (((gameResWidth > 640) || (gameResHeight > 480)) && (videoMode_180660 != 1))
		{
			screenWidth_18062C = gameResWidth;
			screenHeight_180624 = gameResHeight;
			VgaSetVideoModeAltAndPalette_90E07((TColor*)*xadatapald0dat2.colorPalette_var28);
			v0_tempmousex = gameResWidth / 2;
			v0_tempmousey = gameResHeight / 2;
		}
		else
			VgaSetVideoMode640x480AndPalette_90E07((TColor*)*xadatapald0dat2.colorPalette_var28);
	}

	VgaPaletteInstall_41A90((TColor*)*xadatapald0dat2.colorPalette_var28);
	v3 = GetPaletteIndex_5BE80((TColor*)*xadatapald0dat2.colorPalette_var28, 0, 0, 0);
	uiBackGroundColorIdx_EB3A8 = v3;
	if (videoMode_180660 & 1)
	{
		ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, v3);
		ClearScreenAlphaBuffer(320, 200, 255);
	}
	else
	{
		ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, screenWidth_18062C, screenHeight_180624, v3);
		ClearScreenAlphaBuffer(screenWidth_18062C, screenHeight_180624, 255);
	}

	InstallMouse_8CEDF();
	SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]); //Set cursor to Null (Don't Draw)
	gameState_D41A4.paletteMod_51 = 2;
	if (videoMode_180660 == 1)
		FontType_D419D = 1;
	else
		FontType_D419D = 3;

	SetMousePositionInMemory_5BDC0(v0_tempmousex, v0_tempmousey);

	if (EventDispatcher::I != nullptr)
		EventDispatcher::I->DispatchEvent(EventType::E_RESOLUTION_CHANGE, screenWidth_18062C, screenHeight_180624);

	// Recalculate the viewport for the new resolution (dispatches E_VIEWPORT_CHANGE)
	viewPort.ResizeViewPort_2CA90(world_D41A0.m_GameSettings.m_Graphics.m_wViewPortSize);
}

//----- (00075C50) --------------------------------------------------------
void ClearScreenAndBlit_75C50()//fix
{
	if (CommandLineParams.DoShowDebugPerifery())ShowPerifery();

	/* __outx_BYTE(0x302u, 2u);
	 __outx_BYTE(0x303u, 1u);*/
	memset((void*)pdwScreenBuffer_351628, 0, screenHeight_180624 * screenWidth_18062C);
	if (videoMode_180660 & 1)
		VgaBlit320_90478();
	else
		VgaBlit640_75200(480);
}

//----- (000906B4) --------------------------------------------------------
int ClearVgaMemory_906B4()//fix bios graphics//2716b4
{
	if (CommandLineParams.DoShowDebugPerifery())ShowPerifery();

	char* v0; // edx
	signed int v1; // ebx
	char v2; // cl
	signed int v3; // ebx
	int v4; // ecx
	//int v5; // eax
	//unsigned __int8 v6; // al
	int result; // eax
	//char v8; // [esp+0h] [ebp-30h]
	//char v9; // [esp+1h] [ebp-2Fh]
	//int v10; // [esp+1Ch] [ebp-14h]

	//v8 = 0x13;
	//v9 = 0;
	//x_WORD_E3908 = MEMORY[0x449];
//removed int386(0x10, (REGS*)&v8, (REGS*)&v8);//Write string (BIOS after 1/10/86) (graphics)
	//ReadVgaCrtcRegisters_A0BB0((int *)&unk_E3894, 25);
	/*__outx_WORD(0x3C4u, 0x604u);
	__outx_WORD(0x3D4u, 0x14u);
	__outx_WORD(0x3D4u, 0xE317u);
	__outx_WORD(0x3CEu, 0xFF08u);
	__outx_WORD(0x3C4u, 0xF02u);*/
	v0 = (char*)&vgaBuffer_A0000;
	do
	{
		*(x_DWORD*)v0 = 0;
		v0 += 4;
	} while (v0 != (char*)&vgaBuffer_A0000+0x10000);
	v1 = 0;
	do
	{
		v2 = v1++ & 3;
		x_BYTE_1810A3[v1] = 1 << v2; // FIXME: only written to, can this whole function be removed?
	} while (v1 < 320);
	InitScanlineOffsets_90668(0);
	v3 = 0;
	v4 = 0;
	do
	{
		//v10 = 3;
		++v4;
		//v5 = 63 * (v3 & 7) / 7 | (63 * (v3 >> 6) / 3 << 16) | (63 * ((v3 >> 3) & 7) / 7 << 8);
		++v3;
		//x_DWORD_180CA0[v4] = v5;
	} while (v3 < 256);
	//sub_90530((int)&unk_180CA4, 256, 1.2);
	//v6 = __inx_BYTE(0x3DAu);
	//__outx_BYTE(0x3C0u, 0x11u);
	//__outx_BYTE(0x3C0u, 0);
	result = 0;
	//LOBYTE(result) = __inx_BYTE(0x3DAu);
	LOBYTE(result) = 32;
	//__outx_BYTE(0x3C0u, 0x20u);
	return result;
}
// E3908: using guessed type __int16 x_WORD_E3908;
// 180CA0: using guessed type int x_DWORD_180CA0[];

//----- (0002FFE0) --------------------------------------------------------
void GetPauseMenuCoordinates_2FFE0(int16_t* posX, int16_t* posY, int16_t* width, int16_t* height, uint8_t scale)//210fe0
{
	*width = ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SHOW_ICON].width_4 * scale) + (2 * scale);
	*height = ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SHOW_ICON].height_5 * scale);
	int32_t locHeight = (400 - 4 * *height) / 2 - 60;
	int32_t locWidth = (640 - *width) / 2;
	if (videoMode_180660 != 1)
		if (!DefaultResolutions())
		{
			locHeight = ((screenHeight_180624 - (4 * *height)) / 2) - (60 * scale);
			locWidth = (screenWidth_18062C - *width) / 2;
		}

	locWidth &= 0xFFFFFFFE;
	*posX = locWidth;
	locHeight &= 0xFFFFFFFE;
	*posY = locHeight;
}

//----- (00010130) --------------------------------------------------------
signed int GetNextSearchOffset_10130(int a1, x_DWORD* a2, x_DWORD* a3)//1f1130
{
	*a2 = str_E7EE0x[a1].bitmapsData_20xx[0];
	*a3 = str_E7EE0x[a1].bitmapsData_20xx[1];
	str_E7EE0x[a1].bitmapsData_20xx += sizeof(int32_t);
	str_E7EE0x[a1].frameCounter_12++;
	if (str_E7EE0x[a1].bitmapsStr_16->bitmapIndex_4 > str_E7EE0x[a1].frameCounter_12)
	{
		return 1;
	}
	else
	{
		str_E7EE0x[a1].frameCounter_12 = 0;
		str_E7EE0x[a1].bitmapsStr_16++;
		str_E7EE0x[a1].bitmapIndex_8++;
		if (str_E7EE0x[a1].bitmapIndex_8 <= str_E7EE0x[a1].lastBitmapIndex_4)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	return 0;
}

//----- (00019A50) --------------------------------------------------------
void ClearMouseButtonState_19A50()//1faa50
{
	if (inputState_18058C.MouseButtonState_18059C & 1 || inputState_18058C.MouseButtonState_18059C & 2)
		inputState_18058C.MouseButtonState_18059C &= 0xFC;
}

//----- (00090668) --------------------------------------------------------
signed int InitScanlineOffsets_90668(int a1)//271668
{
	int v1; // edx
	signed int result; // eax
	int v3; // edx
	int v4; // ebx
	int v5; // ebx

	v1 = a1;
	if (a1)
	{
		result = 0;
		v3 = 0;
		v4 = 0;
		do
		{
			++result;
			scanlineOffsets_1811E4[v3] = v4;
			++v3;
			v4 += 160;
		} while (result < 400);
	}
	else
	{
		result = 0;
		v5 = 80 * a1;
		do
		{
			++result;
			*(__int16*)((char*)scanlineOffsets_1811E4 + v1) = v5;
			v1 += 2;
			v5 += 80;
		} while (result < 400);
	}
	return result;
}
// 1811E4: using guessed type __int16 scanlineOffsets_1811E4[];

void VgaSetVideoModeAltAndPalette_90E07(TColor* Palette)//271e07
{
	//unsigned __int8 v2; // [esp+0h] [ebp-38h]
	//int v3; // [esp+1Ch] [ebp-1Ch]

	//v3 = 0xf00;//
//removed int386(0x10, (REGS*)&v3, (REGS*)&v2);//Set video mode
	VGA_Resize(screenWidth_18062C, screenHeight_180624);
	if (!videoModeNumber_180662)
		videoModeNumber_180662 = 0x13;
	screenWidth_18062C = screenWidth_18062C;
	screenHeight_180624 = screenHeight_180624;
//removed sub_994BA_cursor_move(0x101); //set position of cursor?
	VgaPaletteInstall_41A90(Palette);
//removed sub_8D12F_set_mouse_viewport(); //set min/max of viewport
	SetViewport_A0D50(0, 0, screenWidth_18062C, screenHeight_180624);
}

//----- (00047130) --------------------------------------------------------
void FreeWebSprites_47130()//228130
{
	if (webSpritesData_D4188)
	{
		FreeMem_83E80((uint8_t*)webSpritesData_D4188);
		webSpritesData_D4188 = 0;
		webSpritesTab_D418C = 0;
		webSpritesTabEnd_D4190 = 0;
	}
}
// D4188: using guessed type int webSpritesData_D4188;
// D418C: using guessed type int webSpritesTab_D418C;
// D4190: using guessed type int webSpritesTabEnd_D4190;


//----- (00054600) --------------------------------------------------------
void MouseReset_54600()////235600 mouse reset see:https://www.equestionanswers.com/c/c-int33-mouse-service.php
{
	//int result; // eax
	//char v1; // [esp+0h] [ebp-38h]
	//__int16 v2; // [esp+1Ch] [ebp-1Ch]

	//v2 = 0;
//removed result = int386(0x33, (REGS*)&v2, (REGS*)&v1);//Mouse Reset/Get Mouse Installed Flag //fix
	//result=1;
	mouseInstalled_E3768 = 0;
	//return result;
}
// E3768: using guessed type int mouseInstalled_E3768;

//----- (0005BCC0) --------------------------------------------------------
void SetDefaultKeys_5BCC0()//23ccc0
{
	//int result; // eax //push ebx=0 ebp=355250

	keys_EB39E[0] = 0x48;//2bc39e//UP
	keys_EB39E[1] = 0x50;//2bc39f//DOWN
	keys_EB39E[3] = 0x4D;//2bc3a1//RIGHT
	keys_EB39E[7] = 0x36;//2bc3a5//RIGHT SHIFT
	keys_EB39E[8] = 0x36;//2bc3a6//RIGHT SHIFT
	//result = x_D41A0_BYTEARRAY_4;//2a51a4
	keys_EB39E[5] = 0x1D;//2bc3A3//CTRL
	gameState_D41A4.langIndex_4 = 0;//dword 38cf54
	gameState_D41A4.soundVolume_6 = 127;//dword 38cf56
	gameState_D41A4.musicVolume_8 = 127;//dword 38cf58
	keys_EB39E[2] = 0x4B;//2BC3A0//LEFT
	gameState_D41A4.showHelp_10 = 1;//byte 38cf5a
	keys_EB39E[4] = 0x1C;//2BC3A2//ENTER
	gameState_D41A4.brightness_11 = 0;//byte 38cf5b
	keys_EB39E[6] = 0x38;//2BC3A4//ALT
	world_D41A0.LevelIndex_0xc = 0;//byte 38cf5c
	keys_EB39E[9] = 0x38;//2BC3A7//ALT
	gameState_D41A4.brightness_13 = 0;//byte 38cf5d
	//return result;
}

//----- (0006EDE0) --------------------------------------------------------
void SetMousePosition_6EDE0(int16_t posX, int16_t posY)//24fde0
{
#if 0
	int locScreenWidth;
	if (videoMode_180660 == 1)
	{
		locScreenWidth = 640;
	}
	else
	{
		locScreenWidth = screenWidth_18062C;
	}

	if (mouseInstalled_E3768 > 0)
	{
		if (posX != -1 || posY != -1)
		{
			if (posX != -1)
			{
				if (posX <= 0)
				{
					if (!(videoMode_180660 & 1))
						posX = 8 * posX;
				}
				else
				{
					if (posX > locScreenWidth - 2)
						posX = locScreenWidth - 2;
					mouse_E3760.x = posX;
					if (!(videoMode_180660 & 1))
						posX *= 8;
				}
				if (posY <= 0)
				{
					if (videoMode_180660 & 1)
						posY = mouse_E3760.y;
					else
						posY = 8 * mouse_E3760.y;
				}
				else
				{
					if (videoMode_180660 & 1)
					{
						if (posY > 398)
							posY = 398;
						mouse_E3760.y = posY;
						if (!(videoMode_180660 & 1))
							posY *= 8;
					}
					else
					{
						if (posY > screenHeight_180624 - 2)
							posY = screenHeight_180624 - 2;
						mouse_E3760.y = posY;
						if (!(videoMode_180660 & 1))
							posY *= 8;
					}
				}

				//320x200 fix
				if (videoMode_180660 & 1)
				{
					posX *= 8;
					posY *= 8;
				}
				//320x200 fix
				//Logger->info("set wonky center {} {} for {},{} {},{}", posX / 8, posY / 8, posX, posY, screenHeight_180624, screenWidth_18062C);
				VGA_Set_mouse(posX / 8, posY / 8);
			}
		}
	}
#else

	uint32_t display_w = screenWidth_18062C, display_h = screenHeight_180624;
	uint32_t new_x, new_y;


	// we get in posX, posY the location of the pointer for a 640x480 display
	// so recalculate the position based on what we actually have

	if (videoMode_180660 & 1) {
		// 320x240 virtual screen
		display_w = 320;
		display_h = 240;
	}

	new_x = display_w * posX / 640;
	new_y = display_h * posY / 480;

	VGA_Set_mouse(new_x, new_y);
#endif
}

//----- (000753D0) --------------------------------------------------------
void ClearPressedKeys_753D0()
{
	__int16 v0; // ax
	int v1; // edx
	//void (*v2)(); // eax
	//__int16 v3; // dx

	//fix it
	//v3 = 0;
	//v2 = 0;
	//fix it

	v0 = 0;
	while (v0 < 128)
	{
		v1 = v0++;
		pressedKeys_180664[v1] = 0;
	}
	//v2 = dos_getvect(9);
	//x_WORD_17D434 = v3;
	//x_DWORD_17D430 = v2;
	//return 0;//fix it
}
// 99BA7: using guessed type x_DWORD dos_getvect(x_DWORD);
// 99BDB: using guessed type x_DWORD dos_setvect(x_DWORD, x_DWORD, x_DWORD);
// 17D430: using guessed type int x_DWORD_17D430;
// 17D434: using guessed type __int16 x_WORD_17D434;

//----- (000759B0) --------------------------------------------------------
void SetMouseMinMaxVert_759B0()
{
	//int result; // eax
	__int16 v1; // [esp+0h] [ebp-1Ch]
	unsigned __int16 v2; // [esp+4h] [ebp-18h]

	if (vrDeviceActive_E12EC)
	{
		memset(&v1, 0, 28);
		v1 = 0x6008;
		v2 = bswap_16(*(x_WORD*)(vrRecord4_17D6C4 + 2));
	}
	//return result;
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// E12EC: using guessed type char vrDeviceActive_E12EC;
// 17D6C4: using guessed type int vrRecord4_17D6C4;


/*void Debug_Blit()
{
	VgaBlit640_75200(480);
}*/

//----- (0007A060) --------------------------------------------------------
void GetMouseAndKeyboardEvents_7A060()//25B060
{
	menuState_17DE38.mouseButtons_17DEEE = 0;
	if (mouseLeftButton_180746)
		menuState_17DE38.mouseButtons_17DEEE |= 1;
	if (mouseRightButton_180744)
		menuState_17DE38.mouseButtons_17DEEE |= 2;
	menuState_17DE38.mouseX_17DEE4 = mouse_E3760.x;//2b4760
	menuState_17DE38.mouseY_17DEE6 = mouse_E3760.y;//2b4762
	ReadKeyboardKeysInMenu_7C050();
	mouseRightButton_180744 = 0;
	mouseLeftButton_180746 = 0;
	mouseRightButton2_18074A = 0;
	mouseLeftButton2_18074C = 0;
}


//----- (0007B5A0) --------------------------------------------------------
void ResetMouse_7B5A0()
{
	if (CommandLineParams.DoShowDebugPerifery())ShowPerifery();

	//_disable();
	menuState_17DE38.mouseButtons_17DEEE = 0;
	mouseRightButton_180744 = 0;
	mouseRightButton2_18074A = 0;
	mouseLeftButton_180746 = 0;
	mouseLeftButton2_18074C = 0;
	//_enable();
}
// 17DEEE: using guessed type __int16 x_WORD_17DEEE_mouse_buttons;
// 180744: using guessed type __int16 mouseRightButton_180744;
// 180746: using guessed type __int16 mouseLeftButton_180746;
// 18074A: using guessed type __int16 mouseRightButton2_18074A;
// 18074C: using guessed type __int16 mouseLeftButton2_18074C;

//----- (00089D10) --------------------------------------------------------
int ReadGameUserInputs_89D10()//26ad10
{
	unsigned __int8 v3; // al
	type_str_611* v5; // ecx
	unsigned __int8 v6; // al
	int v7; // ebx
	signed int v8; // edx
	int v9; // edx
	signed int v10; // eax
	signed int v11; // eax
	signed int v12; // ecx
	int v13; // eax
	signed int v14; // edx
	signed int v15; // eax
	int v16; // ecx
	signed int v17; // eax
	signed int v18; // eax
	unsigned __int8 v19; // al
	int v20; // esi
	int v21; // ecx
	int v22; // ebx
	int v23; // eax
	int v24; // ecx
	int v25; // eax
	__int64 v26; // rax
	__int64 v27; // rax
	__int16 v28; // si
	unsigned __int8 v31; // al
	unsigned __int8 v33; // al
	type_str_611* v34; // edx
	signed int v35; // edx
	signed int v36; // eax
	char v37; // ch
	int v38; // edx
	int v39; // edx
	signed int v40; // edx
	int v41; // ebx
	signed int v42; // eax
	signed int v43; // ecx
	signed int v44; // eax
	int v45; // ebx
	int v46; // ecx
	signed int v47; // eax
	int v48; // esi
	signed int v49; // eax
	unsigned __int8 v50; // al
	int v51; // esi
	int v52; // ecx
	int v53; // ebx
	int v54; // eax
	int v55; // ecx
	char v56; // [esp+0h] [ebp-48h]
	int16_t width; // [esp+34h] [ebp-14h]
	int16_t height; // [esp+38h] [ebp-10h]
	int16_t posX; // [esp+3Ch] [ebp-Ch]
	int16_t posY; // [esp+40h] [ebp-8h]
	int v61; // [esp+44h] [ebp-4h]

	v61 = -1;
	if (!joystickInitialized_E36D0)
	{
		inputState_18058C.x_DWORD_1805A0 = 0;
		inputState_18058C.MouseButtonState_18059C = 0;
		joystickInitialized_E36D0 = 1;
	}
	if (!joystickEnabled_E36D1)
		return v61;
	switch (inputState_18058C.joystickMode_1805C2)
	{
	case 1:
	case 8:
	case 12:
		inputState_18058C.x_WORD_1805C8 = 1;
		v61 = ReadSerialTracker_8B790(inputState_18058C);
		if (inputState_18058C.joystickMode_1805C2 != 8 && inputState_18058C.joystickMode_1805C2 != 12)
		{
			inputState_18058C.mouse_1805B0.x = mouse_E3760.x;
			inputState_18058C.mouse_1805B0.y = mouse_E3760.y;
			inputState_18058C.mouseX_1805B8 = mousePosX_E375C;
			inputState_18058C.mouseY_1805BC = mousePosY_E375E;
			inputState_18058C.arrowKeys_1805C0 = 0;
			if (pressedKeys_180664[keys_EB39E[0]])//UP
				inputState_18058C.arrowKeys_1805C0 |= 1u;
			if (pressedKeys_180664[keys_EB39E[1]])//DOWN
				inputState_18058C.arrowKeys_1805C0 |= 2u;
			if (pressedKeys_180664[keys_EB39E[2]])//LEFT
				inputState_18058C.arrowKeys_1805C0 |= 4u;
			if (pressedKeys_180664[keys_EB39E[3]])//RIGHT
				inputState_18058C.arrowKeys_1805C0 |= 8u;
			inputState_18058C.MouseButtonState_18059C = 0;
			if (mouseLeftButton_180746)
				inputState_18058C.MouseButtonState_18059C = 1;
			if (mouseRightButton_180744)
				inputState_18058C.MouseButtonState_18059C |= 2;
			if (mouseLeftButton2_18074C)
				inputState_18058C.MouseButtonState_18059C |= 4;
			if (mouseRightButton2_18074A)
				inputState_18058C.MouseButtonState_18059C |= 8;
			if (pressedKeys_180664[keys_EB39E[5]])//CTRL
				inputState_18058C.MouseButtonState_18059C |= 0x10;
		}
		inputState_18058C.joyAxisY_180594 *= 4;
		inputState_18058C.joyAxisX_180590 = (inputState_18058C.joyAxisX_180590 << 11) / 360;
		if (inputState_18058C.joystickMode_1805C2 == 8)
			goto LABEL_53;
		if (inputState_18058C.joystickMode_1805C2 == 12)
			goto LABEL_217;
		return v61;
	case 2:
	case 9:
	case 13:
		inputState_18058C.x_WORD_1805C8 = 1;
		ShutdownVrDevice_75910();
		if (inputState_18058C.joystickMode_1805C2 != 9 && inputState_18058C.joystickMode_1805C2 != 13)
		{
			inputState_18058C.mouse_1805B0.x = mouse_E3760.x;
			inputState_18058C.mouse_1805B0.y = mouse_E3760.y;
			inputState_18058C.mouseX_1805B8 = mousePosX_E375C;
			inputState_18058C.mouseY_1805BC = mousePosY_E375E;
			inputState_18058C.arrowKeys_1805C0 = 0;
			if (pressedKeys_180664[keys_EB39E[0]])//UP
				inputState_18058C.arrowKeys_1805C0 |= 1u;
			if (pressedKeys_180664[keys_EB39E[1]])//DOWN
				inputState_18058C.arrowKeys_1805C0 |= 2u;
			if (pressedKeys_180664[keys_EB39E[2]])//LEFT
				inputState_18058C.arrowKeys_1805C0 |= 4u;
			if (pressedKeys_180664[keys_EB39E[3]])//RIGHT
				inputState_18058C.arrowKeys_1805C0 |= 8u;
			inputState_18058C.MouseButtonState_18059C = 0;
			if (mouseLeftButton_180746)
				inputState_18058C.MouseButtonState_18059C = 1;
			if (mouseRightButton_180744)
				inputState_18058C.MouseButtonState_18059C |= 2;
			if (mouseLeftButton2_18074C)
				inputState_18058C.MouseButtonState_18059C |= 4;
			if (mouseRightButton2_18074A)
				inputState_18058C.MouseButtonState_18059C |= 8;
			if (pressedKeys_180664[keys_EB39E[5]])//CTRL
				inputState_18058C.MouseButtonState_18059C |= 0x10;
		}
		inputState_18058C.joyAxisX_180590 = -2048 * vrJoystick_17D6CC[0] / 0xFFFFu & 0x7FF;
		if (LastPressedKey_1806E4 == 46)
			SetMouseMinMaxVert_759B0();
		if (inputState_18058C.joystickMode_1805C2 == 9)
			goto LABEL_53;
		if (inputState_18058C.joystickMode_1805C2 == 13)
			goto LABEL_217;
		inputState_18058C.joyAxisY_180594 = 0;
		return v61;
	case 4:
	LABEL_53:
		inputState_18058C.arrowKeys_1805C0 = 0;
		ReadJoystick_8BBE0(unk_180560x);
		goto LABEL_54;
	case 6:
		memset(&v56, 0, 50);
		GetJoystickButtons_8C140(*(x_WORD*)&externControl_E36C4[0], (uint8_t*)&v56);
		inputState_18058C.joyAxisX_180590 = *(uint32_t*)&externControl_E36C4[4];
		v28 = *(int16_t*)&externControl_E36C4[62];
		inputState_18058C.joyAxisY_180594 = *(uint32_t*)&externControl_E36C4[8];
		if (!v28)
		{
			inputState_18058C.mouse_1805B0.x = mouse_E3760.x;
			inputState_18058C.mouse_1805B0.y = mouse_E3760.y;
			inputState_18058C.mouseX_1805B8 = mousePosX_E375C;
			inputState_18058C.mouseY_1805BC = mousePosY_E375E;
			inputState_18058C.arrowKeys_1805C0 = 0;
			if (pressedKeys_180664[keys_EB39E[0]])//UP
				inputState_18058C.arrowKeys_1805C0 |= 1u;
			if (pressedKeys_180664[keys_EB39E[1]])//DOWN
				inputState_18058C.arrowKeys_1805C0 |= 2u;
			if (pressedKeys_180664[keys_EB39E[2]])//LEFT
				inputState_18058C.arrowKeys_1805C0 |= 4u;
			if (pressedKeys_180664[keys_EB39E[3]])//RIGHT
				inputState_18058C.arrowKeys_1805C0 |= 8u;
			inputState_18058C.MouseButtonState_18059C = 0;
			if (mouseLeftButton_180746)
				inputState_18058C.MouseButtonState_18059C = 1;
			if (mouseRightButton_180744)
				inputState_18058C.MouseButtonState_18059C |= 2;
			if (mouseLeftButton2_18074C)
				inputState_18058C.MouseButtonState_18059C |= 4;
			if (mouseRightButton2_18074A)
				inputState_18058C.MouseButtonState_18059C |= 8;
			if (pressedKeys_180664[keys_EB39E[5]])//CTRL
			{
				inputState_18058C.MouseButtonState_18059C |= 0x10;
				return v61;
			}
			return v61;
		}
		if (v28 != 4)
		{
			if (v28 != 11)
				return v61;
			unk_180560x[26] = externControl_E36C4[16];
			unk_180560x[28] = inputState_18058C.mouse_1805B0.x;
			unk_180560x[29] = inputState_18058C.mouse_1805B0.y;
		LABEL_218:
			if (!(unk_180560x[26] & 1) || unk_180560x[27] & 1)
			{
				if (!(unk_180560x[26] & 1) && unk_180560x[27] & 1)
				{
					LOBYTE(inputState_18058C.MouseButtonState_18059C) = inputState_18058C.MouseButtonState_18059C & 0xFA;
					unk_180560x[27] &= 0xFEu;
				}
			}
			else
			{
				inputState_18058C.mouseX_1805B8 = inputState_18058C.mouse_1805B0.x;
				inputState_18058C.MouseButtonState_18059C |= 0xffffff05u;
				inputState_18058C.mouseY_1805BC = inputState_18058C.mouse_1805B0.y;
				unk_180560x[27] |= 1u;
			}
			if (!(unk_180560x[26] & 2) || unk_180560x[27] & 2)
			{
				if (!(unk_180560x[26] & 2) && unk_180560x[27] & 2)
				{
					inputState_18058C.MouseButtonState_18059C |= 0xfffffff5u;
					unk_180560x[27] &= 0xFDu;
				}
			}
			else
			{
				inputState_18058C.mouseX_1805B8 = inputState_18058C.mouse_1805B0.x;
				inputState_18058C.MouseButtonState_18059C |= 0xffffff0au;
				inputState_18058C.mouseY_1805BC = inputState_18058C.mouse_1805B0.y;
				unk_180560x[27] |= 2u;
			}
			if (pressedKeys_180664[keys_EB39E[5]])//CTRL
			{
				unk_180560x[27] &= 0xF7u;
				unk_180560x[26] &= 0xF7u;
				inputState_18058C.MouseButtonState_18059C |= 0xffffff10u;
			}
			else if (!(unk_180560x[26] & 8) || unk_180560x[27] & 8 || inputState_18058C.MouseButtonState_18059C & 0x10)
			{
				if (unk_180560x[26] & 8 || !(unk_180560x[27] & 8) || inputState_18058C.MouseButtonState_18059C & 0x10)
				{
					if (unk_180560x[26] & 8 && unk_180560x[27] & 8 && inputState_18058C.MouseButtonState_18059C & 0x10)
					{
						unk_180560x[27] &= 0xF7u;
						unk_180560x[26] &= 0xF7u;
					}
					else if (!(unk_180560x[26] & 8) && !(unk_180560x[27] & 8) && inputState_18058C.MouseButtonState_18059C & 0x10)
					{
						inputState_18058C.MouseButtonState_18059C |= 0xffffffEF;
					}
				}
				else
				{
					inputState_18058C.MouseButtonState_18059C |= 0xffffff10;
				}
			}
			else
			{
				unk_180560x[27] |= 8u;
			}
			v31 = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221;
			if (v31 < 5u)
			{
				if (v31 == 3)
				{
					if (82 - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL_WITH_FRAME].width_4 <= inputState_18058C.mouse_1805B0.x)
					{
						if (inputState_18058C.mouse_1805B0.x >= 384)
						{
							if (videoMode_180660 & 1)
								v49 = 400;
							else
								v49 = 480;
							posY = v49 - 72;
							if (v49 - 46 >= inputState_18058C.mouse_1805B0.y)
								v50 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BUTTON_CANCEL2].width_4;
							else
								v50 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_MINISQUARE1].width_4;
							v42 = (signed int)v50 >> 2;
							v9 = ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_MINISQUARE1].height_5 >> 1) + posY + 26;
							v51 = ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BUTTON_CANCEL2].height_5 >> 1) + 4 + posY;
							posX = 0;
							posY = v51;
							v41 = screenWidth_18062C;
							v43 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BUTTON_CANCEL2].height_5;
						}
						else
						{
							if (videoMode_180660 & 1)
								v47 = 400;
							else
								v47 = 480;
							posY = v47 - 72;
							v48 = ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_MINISQUARE1_WITH_FRAME].height_5 >> 1) + 32 + v47 - 72;
							posX = 0;
							posY = v48;
							v41 = screenWidth_18062C;
							v9 = v48;
							v42 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_MINISQUARE1_WITH_FRAME].width_4 >> 2;
							v43 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_MINISQUARE1_WITH_FRAME].height_5 >> 1;
						}
					}
					else
					{
						if (videoMode_180660 & 1)
							v44 = 400;
						else
							v44 = 480;
						posY = v44 - 72;
						v45 = ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL_WITH_FRAME].height_5 >> 1) + 28 + v44 - 72;
						posY = v45;
						posX = ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL_WITH_FRAME].width_4 >> 1) + 4;
						v46 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL_WITH_FRAME].height_5;
						v9 = v45 + v46;
						v43 = v46 >> 1;
						v42 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL_WITH_FRAME].width_4 >> 2;
						v41 = posX + screenWidth_18062C - ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL_WITH_FRAME].width_4 >> 1);
					}
					goto LABEL_301;
				}
			}
			else if (v31 <= 5u || v31 == 8)
			{
				v33 = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.str_611.selectSpellType_0x457_1111;
				v34 = &world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.str_611;
				if (v33)
				{
					if (v33 <= 2u)
					{
						height = 3 * (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL2].width_4;
						v37 = v34->spellIndex_0x458_1112;
						if (v37 < 13)
							v38 = v37;
						else
							v38 = v37 - 13;
						v39 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL].width_4
							+ (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_FRAME].width_4 * v38
							+ ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_FRAME].width_4 >> 1)
							- (height >> 1);
						posX = v39;
						if (screenWidth_18062C - height >= v39)
						{
							if (v39 < 0)
								posX = 0;
						}
						else
						{
							posX = screenWidth_18062C - height;
						}
						v41 = height + posX;
						if (videoMode_180660 & 1)
							v40 = 400;
						else
							v40 = 480;
						v9 = v40
							- 2 * (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_FRAME].height_5
							- ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_FRAME].height_5 >> 1);
						posY = v9;
						v42 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_GOLD_FRAME].width_4 >> 2;
						v43 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_GOLD_FRAME].height_5 >> 1;
					}
					else
					{
						v41 = screenWidth_18062C;
						v9 = 480;
						v42 = 12;
						v43 = 32;
						posX = 0;
						posY = 0;
					}
				}
				else
				{
					v41 = screenWidth_18062C;
					posX = 0;
					if (videoMode_180660 & 1)
						v35 = 400;
					else
						v35 = 480;
					v9 = v35 - ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_FRAME].height_5 >> 1);
					if (videoMode_180660 & 1)
						v36 = 400;
					else
						v36 = 480;
					posY = v36 - 2 * (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_FRAME].height_5;
					posY += (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_FRAME].height_5 >> 1;
					v42 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_FRAME].width_4 >> 2;
					v43 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_FRAME].height_5 >> 1;
				}
				goto LABEL_301;
			}
			if (gameState_D41A4.SelectedMenuItem_38546)
			{
				GetOkayCancelButtonPositions_30BE0(&posX, &posY);
				posX += (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BUTTON_CANCEL1].width_4 >> 1;
				v52 = ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BUTTON_CANCEL1].height_5 >> 1) + posY;
				posY = v52;
				v42 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BUTTON_CANCEL1].width_4;
				v9 = v52;
				v41 = v42 + posX;
				v43 = 0;
			}
			else if (gameState_D41A4.byteindex_225)
			{
				v53 = gameState_D41A4.optionsMenuWidth_186;
				v43 = 4;
				posY = 67;
				v54 = (v53 >> 1) + (screenWidth_18062C - v53) / 2;
				v9 = 393;
				inputState_18058C.mouse_1805B0.x = v54;
				v41 = (v53 >> 1) + (screenWidth_18062C - v53) / 2;
				posX = v54;
				v42 = 0;
			}
			else if (gameState_D41A4.volumeKeyHeld_38591)
			{
				GetPauseMenuCoordinates_2FFE0(&posX, &posY, &height, &width);
				height -= 2;
				v41 = height + posX;
				v9 = 4 * width + 12 + posY;
				v43 = 0;
				v42 = 4;
				posY += 4 * width + 12;
			}
			else if (gameState_D41A4.OptionsSettingFlag_24 & GAME_PAUSED)
			{
				height = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[MENU_ICON_LOAD].width_4;
				width = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_SUB_FIREBALL1_SMALL].height_5
					+ (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[MUSIC_ICON].height_5
					+ (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SOUND_ICON].height_5
					+ (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[MENU_ICON_LOAD].height_5;
				posY = ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[MENU_ICON_LOAD].height_5 >> 1) + (400 - width) / 2 - 60;
				posX = ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[MENU_ICON_LOAD].width_4 >> 2) + (screenWidth_18062C - height) / 2;
				v55 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[MENU_ICON_LOAD].height_5;
				v9 = width + posY - v55;
				v43 = v55 >> 2;
				v42 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[MENU_ICON_LOAD].width_4 >> 2;
				v41 = height + posX - ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[MENU_ICON_LOAD].width_4 >> 1);
			}
			else
			{
				v41 = screenWidth_18062C;
				v9 = 480;
				v43 = 32;
				posX = 0;
				posY = 0;
				v42 = 12;
			}
		LABEL_301:
			if (!(inputState_18058C.MouseButtonState_18059C & 0x10)
				&& !(gameState_D41A4.OptionsSettingFlag_24 & GAME_PAUSED)
				&& !gameState_D41A4.SelectedMenuItem_38546
				&& !gameState_D41A4.byteindex_225
				&& world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 != 3)
			{
				if (unk_180560x[26] & 4)
				{
					if (unk_180560x[29] < 0)
						inputState_18058C.arrowKeys_1805C0 |= 1u;//UP
					if (unk_180560x[29] > 0)
						inputState_18058C.arrowKeys_1805C0 |= 2u;//DOWN
					if (unk_180560x[28] < 0)
						inputState_18058C.arrowKeys_1805C0 |= 4u;//LEFT
					if (unk_180560x[28] > 0)
						inputState_18058C.arrowKeys_1805C0 |= 8u;//RIGHT
				}
				else
				{
					joyFilteredX_E36CC = unk_180560x[28];
					joyFilteredY_E36CE = unk_180560x[29];
					inputState_18058C.mouse_1805B0.x = 2 * unk_180560x[28] + 320;
					inputState_18058C.mouse_1805B0.y = 2 * unk_180560x[29] + 200;
					if (pressedKeys_180664[keys_EB39E[0]])//UP
						inputState_18058C.arrowKeys_1805C0 |= 1u;
					if (pressedKeys_180664[keys_EB39E[1]])//DOWN
						inputState_18058C.arrowKeys_1805C0 |= 2u;
					if (pressedKeys_180664[keys_EB39E[2]])//LEFT
						inputState_18058C.arrowKeys_1805C0 |= 4u;
					if (pressedKeys_180664[keys_EB39E[3]])//RIGHT
						inputState_18058C.arrowKeys_1805C0 |= 8u;
				}
				return v61;
			}
			if (unk_180560x[29] >= 0)
			{
				if (unk_180560x[29] > 0)
					inputState_18058C.mouse_1805B0.y += v43;
			}
			else
			{
				inputState_18058C.mouse_1805B0.y -= v43;
			}
			if (unk_180560x[28] >= 0)
			{
				if (unk_180560x[28] > 0)
					inputState_18058C.mouse_1805B0.x += v42;
			}
			else
			{
				inputState_18058C.mouse_1805B0.x -= v42;
			}
			if (inputState_18058C.mouse_1805B0.x >= posX)
			{
				if (v41 < inputState_18058C.mouse_1805B0.x)
					inputState_18058C.mouse_1805B0.x = v41;
			}
			else
			{
				inputState_18058C.mouse_1805B0.x = posX;
			}
			v25 = inputState_18058C.mouse_1805B0.y;
			if (inputState_18058C.mouse_1805B0.y < posY)
			{
				inputState_18058C.mouse_1805B0.y = posY;
				return v61;
			}
		LABEL_157:
			if (v9 >= v25)
				return v61;
			inputState_18058C.mouse_1805B0.y = v9;
			return v61;
		}
		unk_180560x[26] = externControl_E36C4[16];
		unk_180560x[28] = inputState_18058C.mouse_1805B0.x;
		unk_180560x[29] = inputState_18058C.mouse_1805B0.y;
	LABEL_54:
		if (!(unk_180560x[26] & 1) || unk_180560x[27] & 1)
		{
			if (!(unk_180560x[26] & 1) && unk_180560x[27] & 1)
			{
				inputState_18058C.MouseButtonState_18059C |= 0xffffffFA;
				unk_180560x[27] &= 0xFEu;
			}
		}
		else
		{
			inputState_18058C.MouseButtonState_18059C |= 0xffffff05;
			inputState_18058C.mouseX_1805B8 = inputState_18058C.mouse_1805B0.x;
			unk_180560x[27] |= 1u;
			inputState_18058C.mouseY_1805BC = inputState_18058C.mouse_1805B0.y;
		}
		if (!(unk_180560x[26] & 2) || unk_180560x[27] & 2)
		{
			if (!(unk_180560x[26] & 2) && unk_180560x[27] & 2)
			{
				inputState_18058C.MouseButtonState_18059C |= 0xffffffF5;
				unk_180560x[27] &= 0xFDu;
			}
		}
		else
		{
			inputState_18058C.MouseButtonState_18059C |= 0xffffff0A;
			inputState_18058C.mouseX_1805B8 = inputState_18058C.mouse_1805B0.x;
			unk_180560x[27] |= 2u;
			inputState_18058C.mouseY_1805BC = inputState_18058C.mouse_1805B0.y;
		}
		if (pressedKeys_180664[keys_EB39E[5]])//CTRL
		{
			unk_180560x[26] &= 0xF7u;
			inputState_18058C.MouseButtonState_18059C |= 0xffffff10;
			unk_180560x[27] &= 0xF7u;
		}
		else if (!(gameState_D41A4.OptionsSettingFlag_24 & GAME_PAUSED))
		{
			if (!(unk_180560x[26] & 8) || unk_180560x[27] & 8 || inputState_18058C.MouseButtonState_18059C & 0x10)
			{
				if (unk_180560x[26] & 8 || !(unk_180560x[27] & 8) || inputState_18058C.MouseButtonState_18059C & 0x10)
				{
					if (unk_180560x[26] & 8 && unk_180560x[27] & 8 && inputState_18058C.MouseButtonState_18059C & 0x10)
					{
						unk_180560x[27] &= 0xF7u;
						unk_180560x[26] &= 0xF7u;
					}
					else if (!(unk_180560x[26] & 8) && !(unk_180560x[27] & 8) && inputState_18058C.MouseButtonState_18059C & 0x10)
					{
						inputState_18058C.MouseButtonState_18059C |= 0xffffffEF;
					}
				}
				else
				{
					inputState_18058C.MouseButtonState_18059C |= 0xffffff10;
				}
			}
			else
			{
				unk_180560x[27] |= 8u;
			}
		}
		v3 = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221;
		if (v3 < 5u)
		{
			if (v3 == 3)
			{
				if (82 - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL_WITH_FRAME].width_4 <= inputState_18058C.mouse_1805B0.x) // fix it
				{
					if (inputState_18058C.mouse_1805B0.x >= 384)
					{
						if (videoMode_180660 & 1)
							v18 = 400;
						else
							v18 = 480;
						posY = v18 - 72;
						if (v18 - 46 >= inputState_18058C.mouse_1805B0.y)
							v19 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BUTTON_CANCEL2].width_4;
						else
							v19 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BUTTON_CANCEL1].width_4;
						v11 = (signed int)v19 >> 2;
						v9 = posY + 26 + ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BUTTON_CANCEL1].height_5 >> 1);
						v20 = ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BUTTON_CANCEL2].height_5 >> 1) + 4 + posY;
						posX = 0;
						posY = v20;
						v7 = screenWidth_18062C;
						v12 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BUTTON_CANCEL2].height_5;
					}
					else
					{
						if (videoMode_180660 & 1)
							v17 = 400;
						else
							v17 = 480;
						posY = v17 - 72;
						v9 = ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_MINISQUARE1_WITH_FRAME].height_5 >> 1) + 32 + v17 - 72;
						posX = 0;
						posY = v9;
						v7 = screenWidth_18062C;
						v11 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_MINISQUARE1_WITH_FRAME].width_4 >> 2;
						v12 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_MINISQUARE1_WITH_FRAME].height_5 >> 1;
					}
				}
				else
				{
					if (videoMode_180660 & 1)
						v15 = 400;
					else
						v15 = 480;
					posY = v15 - 72;
					posY = ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL_WITH_FRAME].height_5 >> 1) + 28 + v15 - 72;
					posX = ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL_WITH_FRAME].width_4 >> 1) + 4;
					v16 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL_WITH_FRAME].height_5;
					v9 = posY + v16;
					v12 = v16 >> 1;
					v11 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL_WITH_FRAME].width_4 >> 2;
					v7 = posX + screenWidth_18062C - ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL_WITH_FRAME].width_4 >> 1);
				}
				goto LABEL_138;
			}
		}
		else if (v3 <= 5u || v3 == 8)
		{
			v5 = &world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.str_611;
			v6 = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.str_611.selectSpellType_0x457_1111;
			if (!v6)
			{
				v7 = screenWidth_18062C;
				posX = 0;
				if (videoMode_180660 & 1)
					v8 = 400;
				else
					v8 = 480;
				v9 = v8 - ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_FRAME].height_5 >> 1);
				if (videoMode_180660 & 1)
					v10 = 400;
				else
					v10 = 480;
				posY = v10 - 2 * (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_FRAME].height_5;
				posY += (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_FRAME].height_5 >> 1;
				v11 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_FRAME].width_4 >> 2;
				v12 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_FRAME].height_5 >> 1;
				goto LABEL_138;
			}
			if (v6 <= 2u)
			{
				height = 3 * (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_GOLD_FRAME].width_4;
				LOBYTE(v13) = v5->spellIndex_0x458_1112;
				if ((char)v13 < 13)
					v13 = (char)v13;
				else
					v13 = (char)v13 - 13;
				posX = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_FRAME].width_4 * v13
					+ (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL].width_4
					+ ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_FRAME].width_4 >> 1)
					- (height >> 1);
				if (screenWidth_18062C - height >= posX)
				{
					if (posX < 0)
						posX = 0;
				}
				else
				{
					posX = screenWidth_18062C - height;
				}
				v7 = height + posX;
				if (videoMode_180660 & 1)
					v14 = 400;
				else
					v14 = 480;
				v9 = v14
					- 2 * (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_FRAME].height_5
					- ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_FRAME].height_5 >> 1);
				posY = v9;
				v11 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_GOLD_FRAME].width_4 >> 2;
				v12 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_GOLD_FRAME].height_5 >> 1;
				goto LABEL_138;
			}
			goto LABEL_137;
		}
		if (gameState_D41A4.SelectedMenuItem_38546)
		{
			GetOkayCancelButtonPositions_30BE0(&posX, &posY);
			posX += (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BUTTON_CANCEL1].width_4 >> 1;
			v21 = ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BUTTON_CANCEL1].height_5 >> 1) + posY;
			posY = v21;
			v11 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BUTTON_CANCEL1].width_4;
			v9 = v21;
			v7 = v11 + posX;
			v12 = 0;
			goto LABEL_138;
		}
		if (gameState_D41A4.byteindex_225)
		{
			v22 = gameState_D41A4.optionsMenuWidth_186;
			v12 = 4;
			posY = 67;
			v23 = (v22 >> 1) + (screenWidth_18062C - v22) / 2;
			v9 = 393;
			inputState_18058C.mouse_1805B0.x = v23;
			v7 = (v22 >> 1) + (screenWidth_18062C - v22) / 2;
			posX = v23;
			v11 = 0;
			goto LABEL_138;
		}
		if (gameState_D41A4.volumeKeyHeld_38591)
		{
			GetPauseMenuCoordinates_2FFE0(&posX, &posY, &height, &width);
			height -= 2;
			v7 = height + posX;
			v9 = 4 * width + 12 + posY;
			v12 = 0;
			v11 = 4;
			posY += 4 * width + 12;
			goto LABEL_138;
		}
		if (gameState_D41A4.OptionsSettingFlag_24 & GAME_PAUSED)
		{
			height = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[MENU_ICON_LOAD].width_4;
			width = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_SUB_FIREBALL1_SMALL].height_5
				+ (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[MUSIC_ICON].height_5
				+ (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SOUND_ICON].height_5
				+ (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_SUB_FIREBALL1_SMALL].height_5;
			posY = ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_SUB_FIREBALL1_SMALL].height_5 >> 1) + (400 - width) / 2 - 60;
			posX = ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_SUB_FIREBALL1_SMALL].width_4 >> 2) + (screenWidth_18062C - height) / 2;
			v24 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_SUB_FIREBALL1_SMALL].height_5;
			v9 = width + posY - v24;
			v12 = v24 >> 2;
			v11 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_SUB_FIREBALL1_SMALL].width_4 >> 2;
			v7 = height + posX - ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_SUB_FIREBALL1_SMALL].width_4 >> 1);
			goto LABEL_138;
		}
	LABEL_137:
		v7 = screenWidth_18062C;
		v9 = 480;
		v12 = 32;
		posX = 0;
		posY = 0;
		v11 = 12;
	LABEL_138:
		if (inputState_18058C.MouseButtonState_18059C & 0x10
			|| gameState_D41A4.OptionsSettingFlag_24 & GAME_PAUSED
			|| gameState_D41A4.SelectedMenuItem_38546
			|| gameState_D41A4.byteindex_225
			|| world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 == 3)
		{
			if (unk_180560x[29] >= 0)
			{
				if (unk_180560x[29] > 0)
					inputState_18058C.mouse_1805B0.y += v12;
			}
			else
			{
				inputState_18058C.mouse_1805B0.y -= v12;
			}
			if (unk_180560x[28] >= 0)
			{
				if (unk_180560x[28] > 0)
					inputState_18058C.mouse_1805B0.x += v11;
			}
			else
			{
				inputState_18058C.mouse_1805B0.x -= v11;
			}
			if (inputState_18058C.mouse_1805B0.x >= posX)
			{
				if (v7 < inputState_18058C.mouse_1805B0.x)
					inputState_18058C.mouse_1805B0.x = v7;
			}
			else
			{
				inputState_18058C.mouse_1805B0.x = posX;
			}
			v25 = inputState_18058C.mouse_1805B0.y;
			if (inputState_18058C.mouse_1805B0.y < posY)
			{
				inputState_18058C.mouse_1805B0.y = posY;
				return v61;
			}
			goto LABEL_157;
		}
		if (unk_180560x[26] & 4)
		{
			if (unk_180560x[29] < 0)
				inputState_18058C.arrowKeys_1805C0 |= 1u;//UP
			if (unk_180560x[29] > 0)
				inputState_18058C.arrowKeys_1805C0 |= 2u;//DOWN
			if (unk_180560x[28] < 0)
				inputState_18058C.arrowKeys_1805C0 |= 4u;//LEFT
			if (unk_180560x[28] > 0)
				inputState_18058C.arrowKeys_1805C0 |= 8u;//RIGHT
		}
		else
		{
			v26 = 28 * (signed __int16)(32 * unk_180560x[28] + joyFilteredX_E36CC);
			joyFilteredX_E36CC = (signed int)(v26 - (__CFSHL__(HIDWORD(v26), 5) + 32 * HIDWORD(v26))) >> 5;
			v27 = 31 * (signed __int16)(16 * unk_180560x[29] + joyFilteredY_E36CE);
			joyFilteredY_E36CE = (signed int)(v27 - (__CFSHL__(HIDWORD(v27), 5) + 32 * HIDWORD(v27))) >> 5;
			inputState_18058C.mouse_1805B0.x = 2 * joyFilteredX_E36CC + 320;
			inputState_18058C.mouse_1805B0.y = 2 * joyFilteredY_E36CE + 200;
			if (pressedKeys_180664[keys_EB39E[0]])//UP
				inputState_18058C.arrowKeys_1805C0 |= 1u;
			if (pressedKeys_180664[keys_EB39E[1]])//DOWN
				inputState_18058C.arrowKeys_1805C0 |= 2u;
			if (pressedKeys_180664[keys_EB39E[2]])//LEFT
				inputState_18058C.arrowKeys_1805C0 |= 4u;
			if (pressedKeys_180664[keys_EB39E[3]])//RIGHT
				inputState_18058C.arrowKeys_1805C0 |= 8u;
		}
		return v61;
	case 7:
		inputState_18058C.mouse_1805B0.x = mouse_E3760.x;
		inputState_18058C.mouse_1805B0.y = mouse_E3760.y;
		inputState_18058C.mouseX_1805B8 = mousePosX_E375C;
		inputState_18058C.mouseY_1805BC = mousePosY_E375E;
		inputState_18058C.arrowKeys_1805C0 = 0;
		if (pressedKeys_180664[keys_EB39E[0]])//UP
			inputState_18058C.arrowKeys_1805C0 |= 1u;
		if (pressedKeys_180664[keys_EB39E[1]])//DOWN
			inputState_18058C.arrowKeys_1805C0 |= 2u;
		if (pressedKeys_180664[keys_EB39E[2]])//RIGHT
			inputState_18058C.arrowKeys_1805C0 |= 4u;
		if (pressedKeys_180664[keys_EB39E[3]])//LEFT
			inputState_18058C.arrowKeys_1805C0 |= 8u;
		inputState_18058C.MouseButtonState_18059C = 0;
		if (mouseLeftButton_180746)
			inputState_18058C.MouseButtonState_18059C = 1;
		if (mouseRightButton_180744)
			inputState_18058C.MouseButtonState_18059C |= 2;
		if (mouseLeftButton2_18074C)
			inputState_18058C.MouseButtonState_18059C |= 4;
		if (mouseRightButton2_18074A)
			inputState_18058C.MouseButtonState_18059C |= 8;
		if (!pressedKeys_180664[keys_EB39E[5]])//CTRL
			return v61;
		inputState_18058C.MouseButtonState_18059C |= 0x10;
		return v61;
	case 11:
	LABEL_217:
		inputState_18058C.arrowKeys_1805C0 = 0;
		ReadJoystick_8BBE0(unk_180560x);
		goto LABEL_218;
	default:
		return v61;
	}
}

//----- (0008BB40) --------------------------------------------------------
void InitJoystickState_8BB40(uint8_t* a1, char a2)//26cb40
{
	a1[37] = a2;
	a1[36] = 2;
	*(x_DWORD*)&a1[8] = 10000000;
	*(x_DWORD*)&a1[20] = 10000000;
	*(x_DWORD*)&a1[12] = 0;
	*(x_DWORD*)&a1[24] = 0;
	if (a2 == 8)
	{
		a1[37] = 7;
		a1[36] = 3;
	}
}

//----- (0008BBE0) --------------------------------------------------------
signed int ReadJoystick_8BBE0(uint8_t* a1)//fix//26cbe0
{
	if (CommandLineParams.DoShowDebugPerifery())ShowPerifery();

	char v1 = 0; // dl
	unsigned __int8 v2; // al
	char v3; // al
	signed int result; // eax
	signed int v5; // ecx
	unsigned __int8 v6; // al
	signed int v7; // ecx
	unsigned __int8 v8; // al
	signed int v9; // ecx
	unsigned __int8 v10; // al
	unsigned __int8 v11; // al
	int v12; // esi
	int v13; // esi
	int v14; // edi
	int v15; // ecx
	int v16; // eax
	int v17; // ecx
	int v18; // esi
	int v19; // eax
	int v20; // eax
	int v21; // ecx
	int v22; // esi
	int v23; // eax
	int v24; // ecx
	int v25; // edi
	int v26; // eax
	int v27; // ecx
	int v28; // esi
	int v29; // eax
	int v30; // eax
	int v31; // ecx
	int v32; // esi
	int v33; // eax
	char v34; // al
	int v35; // edx
	int v36; // esi
	unsigned __int8 v37; // al
	int v38; // eax
	char v39; // dh
	int v40; // [esp+0h] [ebp-14h]
	int v41; // [esp+4h] [ebp-10h]
	int v42; // [esp+8h] [ebp-Ch]
	int v43; // [esp+Ch] [ebp-8h]
	unsigned __int8 v44; // [esp+10h] [ebp-4h]
	char v45; // [esp+10h] [ebp-4h]

	//fix it
	v6 = 0;
	v8 = 0;
	v10 = 0;
	//fix it

	v1 = 0;
	v43 = 0;
	v42 = 0;
	v41 = 0;
	v40 = 0;
	v44 = 0;
	if (*(x_BYTE*)(a1 + 36))
	{
		//_disable();
		v2 = *(x_BYTE*)(a1 + 37);
		switch (v2)
		{
		case 0u:
		LABEL_3:
			*(x_BYTE*)(a1 + 38) = 0;
			*(x_BYTE*)(a1 + 43) = 0;
			v3 = *(x_BYTE*)(a1 + 43);
			*(x_BYTE*)(a1 + 42) = v3;
			*(x_BYTE*)(a1 + 41) = v3;
			*(x_BYTE*)(a1 + 40) = v3;
			return 0;
		case 1u:
		case 2u:
		case 3u:
		case 4u:
			v5 = 10000;
			//_disable();
			v1 = 1;
			//__outx_BYTE(0x201u, v2);
			do
			{
				// v6 = __inx_BYTE(0x201u);
				if (!(v6 & 3))
					break;
				if (v6 & 1)
					++v43;
				if (v6 & 2)
					++v42;
				--v5;
			} while (v5);
			//_enable();
			v44 = v6;
			if (!v5)
				*(x_BYTE*)(a1 + 36) = 0;
			break;
		case 5u:
		case 6u:
			v7 = 10000;
			//_disable();
			v1 = 1;
			//__outx_BYTE(0x201u, v2);
			do
			{
				//v8 = __inx_BYTE(0x201u);

				if (!(v8 & 0xB))
					break;
				if (v8 & 1)
					v43++;
				if (v8 & 2)
					v42++;
				if (v8 & 8)
					v40++;
				v7--;
			} while (v7);
			//_enable();
			v44 = v8;
			if (!v7)
				*(x_BYTE*)(a1 + 36) = 0;
			break;
		case 7u:
			v9 = 10000;
			//_disable();
			v1 = 1;
			//__outx_BYTE(0x201u, v2);
			do
			{
				//v10 = __inx_BYTE(0x201u);

				if (!(v10 & 0xF))
					break;
				if (v10 & 1)
					++v43;
				if (v10 & 2)
					++v42;
				if (v10 & 4)
					++v41;
				if (v10 & 8)
					++v40;
				--v9;
			} while (v9);
			//_enable();
			v44 = v10;
			if (!v9)
			{
				if (*(x_BYTE*)(a1 + 36) == 3)
				{
					v1 = 123;
					*(x_BYTE*)(a1 + 37) = 2;
				}
				else
				{
					*(x_BYTE*)(a1 + 36) = 1;
				}
			}
			break;
		default:
			break;
		}
		//_enable();
	}
	v45 = (~v44 >> 4) & 0xF;
	v11 = *(x_BYTE*)(a1 + 37);
	if (v11 < 3u)
	{
		if (v11 != 1)
			goto LABEL_46;
		goto LABEL_45;
	}
	if (v11 <= 3u || v11 == 5)
		LABEL_45:
	v45 &= 3u;
LABEL_46:
	switch (*(x_BYTE*)(a1 + 36))
	{
	case 0:
		goto LABEL_3;
	case 1:
		*(x_BYTE*)(a1 + 38) = v45;
		switch (*(x_BYTE*)(a1 + 37))
		{
		case 1:
		case 2:
			goto LABEL_52;
		case 3:
		case 4:
			goto LABEL_66;
		case 5:
		case 6:
			switch (8 * v40 / *(x_DWORD*)(a1 + 32))
			{
			case 0:
				*(x_BYTE*)(a1 + 42) = 0;
				*(x_BYTE*)(a1 + 43) = -1;
				goto LABEL_66;
			case 1:
			case 2:
				*(x_BYTE*)(a1 + 42) = 1;
				break;
			case 3:
			case 4:
				*(x_BYTE*)(a1 + 42) = 0;
				*(x_BYTE*)(a1 + 43) = 1;
				goto LABEL_66;
			case 5:
			case 6:
				*(x_BYTE*)(a1 + 42) = -1;
				break;
			default:
				*(x_BYTE*)(a1 + 42) = 0;
				break;
			}
			*(x_BYTE*)(a1 + 43) = 0;
		LABEL_66:
			v14 = *(x_DWORD*)(a1 + 16);
			v15 = v43 - v14;
			if (v43 - v14 < 0)
			{
				v20 = (*(x_DWORD*)(a1 + 8) - v14) >> 3;
				v21 = v15 - v20;
				v22 = *(x_DWORD*)(a1 + 8) - v14 - 2 * v20;
				if (v21 <= 0)
				{
					if (*(x_DWORD*)(a1 + 8) - v14 == 2 * v20)
						v22 = 1;
					v23 = -127 * v21 / v22;
					if (v23 <= -127)
						*(x_BYTE*)(a1 + 40) = -127;
					else
						*(x_BYTE*)(a1 + 40) = v23;
				}
				else
				{
					*(x_BYTE*)(a1 + 40) = 0;
				}
			}
			else
			{
				v16 = (*(x_DWORD*)(a1 + 12) - v14) >> 3;
				v17 = v15 - v16;
				v18 = *(x_DWORD*)(a1 + 12) - v14 - 2 * v16;
				if (v17 >= 0)
				{
					if (*(x_DWORD*)(a1 + 12) - v14 == 2 * v16)
						v18 = 1;
					v19 = 127 * v17 / v18;
					if (v19 >= 127)
						*(x_BYTE*)(a1 + 40) = 127;
					else
						*(x_BYTE*)(a1 + 40) = v19;
				}
				else
				{
					*(x_BYTE*)(a1 + 40) = 0;
				}
			}
			v24 = v42 - *(x_DWORD*)(a1 + 28);
			if (v24 < 0)
			{
				v30 = (*(x_DWORD*)(a1 + 20) - *(x_DWORD*)(a1 + 28)) >> 3;
				v31 = v24 - v30;
				v32 = *(x_DWORD*)(a1 + 20) - *(x_DWORD*)(a1 + 28) - 2 * v30;
				if (v31 <= 0)
				{
					if (*(x_DWORD*)(a1 + 20) - *(x_DWORD*)(a1 + 28) == 2 * v30)
						v32 = 1;
					v33 = -127 * v31 / v32;
					if (v33 <= -127)
						*(x_BYTE*)(a1 + 41) = -127;
					else
						*(x_BYTE*)(a1 + 41) = v33;
					result = 0;
				}
				else
				{
					*(x_BYTE*)(a1 + 41) = 0;
					result = 0;
				}
			}
			else
			{
				v25 = *(x_DWORD*)(a1 + 28);
				v26 = (*(x_DWORD*)(a1 + 24) - v25) >> 3;
				v27 = v24 - v26;
				v28 = *(x_DWORD*)(a1 + 24) - v25 - 2 * v26;
				if (v27 >= 0)
				{
					if (*(x_DWORD*)(a1 + 24) - v25 == 2 * v26)
						v28 = 1;
					v29 = 127 * v27 / v28;
					if (v29 >= 127)
						*(x_BYTE*)(a1 + 41) = 127;
					else
						*(x_BYTE*)(a1 + 41) = v29;
					result = 0;
				}
				else
				{
					*(x_BYTE*)(a1 + 41) = 0;
					result = 0;
				}
			}
			break;
		case 7:
			if (v41 < *(x_DWORD*)(a1 + 32))
				*(x_BYTE*)(a1 + 38) |= 0x10u;
			if (v40 < *(x_DWORD*)(a1 + 32))
				*(x_BYTE*)(a1 + 38) |= 0x20u;
		LABEL_52:
			v12 = *(x_DWORD*)(a1 + 16);
			if ((v12 + *(x_DWORD*)(a1 + 8)) >> 1 <= v43)
				*(x_BYTE*)(a1 + 40) = (v12 + *(x_DWORD*)(a1 + 12)) >> 1 < v43;
			else
				*(x_BYTE*)(a1 + 40) = -1;
			v13 = *(x_DWORD*)(a1 + 28);
			if ((v13 + *(x_DWORD*)(a1 + 20)) >> 1 <= v42)
				*(x_BYTE*)(a1 + 41) = (v13 + *(x_DWORD*)(a1 + 24)) >> 1 < v42;
			else
				*(x_BYTE*)(a1 + 41) = -1;
			*(x_DWORD*)a1 = v43;
			*(x_DWORD*)(a1 + 4) = v42;
			result = 0;
			break;
		default:
			goto LABEL_123;
		}
		return result;
	case 2:
		*(x_BYTE*)(a1 + 38) = 0;
		*(x_BYTE*)(a1 + 43) = 0;
		v34 = *(x_BYTE*)(a1 + 43);
		*(x_BYTE*)(a1 + 42) = v34;
		*(x_BYTE*)(a1 + 41) = v34;
		*(x_BYTE*)(a1 + 40) = v34;
		if (!(v45 & 1)
			|| (*(x_DWORD*)(a1 + 16) = v43,
				*(x_DWORD*)(a1 + 28) = v42,
				v35 = *(x_DWORD*)(a1 + 16),
				v35 <= *(x_DWORD*)(a1 + 8))
			|| v35 >= *(x_DWORD*)(a1 + 12)
			|| (v36 = *(x_DWORD*)(a1 + 28), *(x_DWORD*)(a1 + 20) >= v36)
			|| v36 >= *(x_DWORD*)(a1 + 24))
		{
			if (v43 < *(x_DWORD*)(a1 + 8))
				*(x_DWORD*)(a1 + 8) = v43;
			if (v43 > *(x_DWORD*)(a1 + 12))
				*(x_DWORD*)(a1 + 12) = v43;
			if (v42 < *(x_DWORD*)(a1 + 20))
				*(x_DWORD*)(a1 + 20) = v42;
			if (v42 > *(x_DWORD*)(a1 + 24))
				*(x_DWORD*)(a1 + 24) = v42;
			return 1;
		}
		v37 = *(x_BYTE*)(a1 + 37);
		if (v37 < 5u)
			goto LABEL_122;
		if (v37 <= 6u)
		{
			v38 = v40;
		}
		else
		{
			if (v37 != 7)
				goto LABEL_122;
			v38 = (v40 + v41) >> 2;
		}
		*(x_DWORD*)(a1 + 32) = v38;
	LABEL_122:
		*(x_BYTE*)(a1 + 36) = 1;
	LABEL_123:
		result = 0;
		break;
	case 3:
		if (v1 == 123)
			goto LABEL_123;
		*(x_DWORD*)(a1 + 16) = v43;
		*(x_DWORD*)(a1 + 28) = v42;
		*(x_DWORD*)(a1 + 12) = v43 / 2 + v43;
		*(x_DWORD*)(a1 + 24) = v42 / 2 + v42;
		*(x_DWORD*)(a1 + 8) = v43 / 6;
		v39 = *(x_BYTE*)(a1 + 37);
		*(x_DWORD*)(a1 + 20) = v42 / 6;
		if (v39 == 7)
			*(x_DWORD*)(a1 + 32) = (v40 + v41) >> 2;
		goto LABEL_122;
	default:
		goto LABEL_123;
	}
	return result;
}

//----- (0008C140) --------------------------------------------------------
char GetJoystickButtons_8C140(unsigned __int16 a1, uint8_t* a2)//26d140
{
	int v3; // [esp+0h] [ebp-28h]
	//int v4; // [esp+4h] [ebp-24h]
	//int v5; // [esp+14h] [ebp-14h]
	//char v6; // [esp+1Ch] [ebp-Ch]

	memset(&v3, 0, 28);
	//v4 = a1;
	v3 = 0x300;
	//segread((SREGS*)&v6);
	//v5 = a2;
	//int386x(0x31, (REGS*)&v3, (REGS*)&v3, (SREGS*)&v6);
	return *(x_BYTE*)(a2 + 28);
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// 99DE6: using guessed type x_DWORD segread(x_DWORD);
// 99E10: using guessed type x_DWORD int386x(x_DWORD, x_DWORD, x_DWORD, x_DWORD);

//----- (0008C21F) --------------------------------------------------------
// int sub_8C21F_any_graphics_command()//26d21f NOTE: removed as it did DOS memory handling

//----- (0008C2DE) --------------------------------------------------------
/*void sub_8C2DE()//26d2de
{
	;
}*/

//----- (0008C329) --------------------------------------------------------
int UpdateMouseCursorPosition_8C329()//26D329
{
	int result; // eax
	x_BYTE* v1; // eax
	x_BYTE* v2; // eax

	cursorPosX_1806E8 = mouse_E3760.x;
	cursorPosY_1806E8 = mouse_E3760.y;//fix it

	if (videoMode_180660 & 1)
	{
		cursorPosX_1806E8 >>= 1;
		cursorPosY_1806E8 >>= 1;
	}
	cursorScreenIndex2_1806F4 = (cursorPosX_1806E8 >> 16) + (cursorPosY_1806E8 >> 16) * screenWidth_18062C;
	if (videoMode_180660 & 8)
	{
		HIWORD(cursorColumn_18073A) = HIWORD(cursorScreenIndex2_1806F4);
		SetVgaPaletteBank_9951B(SHIWORD(cursorScreenIndex2_1806F4));//GET VIDEO STATE
	}
	unk_1806EC = screenWidth_18062C - cursorPosX_1806E8;
	if ((signed __int16)(screenWidth_18062C - cursorPosX_1806E8) > cursorWidth_18072C)
		unk_1806EC = cursorWidth_18072C;
	cursorRows_1806EE = screenHeight_180624 - cursorPosY_1806E8;
	if ((signed __int16)(screenHeight_180624 - cursorPosY_1806E8) > cursorHeight_18072E)
		cursorRows_1806EE = cursorHeight_18072E;
	cursorBackgroundPtr_180714 = cursorBackground2_1806F0;
	ptrScreenPos_180708 = (uint8_t*)&vgaBuffer_A0000 + (unsigned __int16)cursorScreenIndex2_1806F4;
	ptrCursorBitmapPos_18070C = cursorData_180730;
	if (videoMode_180660 & 1)
	{
		for (cursorRow_180738 = 0; ; ++cursorRow_180738)
		{
			result = unk_1806EC >> 16;
			if ((signed int)(unsigned __int16)cursorRow_180738 >= (unk_1806EC >> 16))
				break;
			LOWORD(cursorColumn_18073A) = 0;
			while ((unsigned __int16)cursorColumn_18073A < (cursorPosY_1806E8 >> 16))
			{
				v1 = (x_BYTE*)cursorBackgroundPtr_180714++;
				*v1 = *(x_BYTE*)ptrScreenPos_180708;
				if (*(x_BYTE*)ptrCursorBitmapPos_18070C != -2)
					*(x_BYTE*)ptrScreenPos_180708 = *(x_BYTE*)ptrCursorBitmapPos_18070C;
				++ptrScreenPos_180708;
				++ptrCursorBitmapPos_18070C;
				LOWORD(cursorColumn_18073A) = cursorColumn_18073A + 1;
			}
			ptrScreenPos_180708 += screenWidth_18062C - (cursorPosY_1806E8 >> 16);
			ptrCursorBitmapPos_18070C += (*(int*)((char*)&cursorData_180730 + 2) >> 16) - (cursorPosY_1806E8 >> 16);
		}
	}
	else
	{
		for (cursorRow_180738 = 0; ; ++cursorRow_180738)
		{
			result = unk_1806EC >> 16;
			if ((signed int)(unsigned __int16)cursorRow_180738 >= unk_1806EC >> 16)
				break;
			LOWORD(cursorColumn_18073A) = 0;
			while ((unsigned __int16)cursorColumn_18073A < cursorPosY_1806E8 >> 16)
			{
				v2 = (x_BYTE*)cursorBackgroundPtr_180714++;
				*v2 = *(x_BYTE*)ptrScreenPos_180708;
				if (*(x_BYTE*)ptrCursorBitmapPos_18070C != -2)
					*(x_BYTE*)ptrScreenPos_180708 = *(x_BYTE*)ptrCursorBitmapPos_18070C;
				if (++ptrScreenPos_180708 >= (uint8_t*)0xAFFFE + 2)
				{
					++HIWORD(cursorColumn_18073A);
					SetVgaPaletteBank_9951B(SHIWORD(cursorColumn_18073A));
					ptrScreenPos_180708 -= 0x10000;
				}
				++ptrCursorBitmapPos_18070C;
				LOWORD(cursorColumn_18073A) = cursorColumn_18073A + 1;
			}
			ptrScreenPos_180708 += screenWidth_18062C - (cursorPosY_1806E8 >> 16);
			if (ptrScreenPos_180708 >= (uint8_t*)0xAFFFE + 2)
			{
				++HIWORD(cursorColumn_18073A);
				SetVgaPaletteBank_9951B(SHIWORD(cursorColumn_18073A));
				ptrScreenPos_180708 -= 0x10000;
			}
			ptrCursorBitmapPos_18070C += (*(int*)((char*)&cursorData_180730 + 2) >> 16) - (cursorPosY_1806E8 >> 16);
		}
	}
	return result;
}

//----- (0008C839) --------------------------------------------------------
int SaveCursorBackground_8C839()//26d839
{
	int result; // eax
	x_BYTE* v1; // eax
	x_BYTE* v2; // edx
	x_BYTE* v3; // eax
	x_BYTE* v4; // edx

	if (!(videoMode_180660 & 1))
	{
		HIWORD(cursorColumn_18073A) = HIWORD(cursorScreenIndex2_1806F4);
		SetVgaPaletteBank_9951B(SHIWORD(cursorScreenIndex2_1806F4));
	}
	cursorBackgroundPtr_180714 = cursorBackground2_1806F0;
	ptrScreenPos_180708 = (uint8_t*)&vgaBuffer_A0000 + (unsigned __int16)cursorScreenIndex2_1806F4;//graphics memory maybe
	if (videoMode_180660 & 1)
	{
		for (cursorRow_180738 = 0; ; ++cursorRow_180738)
		{
			result = unk_1806EC >> 16;
			if ((signed int)(unsigned __int16)cursorRow_180738 >= unk_1806EC >> 16)
				break;
			LOWORD(cursorColumn_18073A) = 0;
			while ((unsigned __int16)cursorColumn_18073A < (cursorPosY_1806E8 >> 16))
			{
				v1 = (x_BYTE*)cursorBackgroundPtr_180714++;
				v2 = (x_BYTE*)ptrScreenPos_180708++;
				*v2 = *v1;
				LOWORD(cursorColumn_18073A) = cursorColumn_18073A + 1;
			}
			ptrScreenPos_180708 += screenWidth_18062C - (cursorPosY_1806E8 >> 16);
		}
	}
	else
	{
		for (cursorRow_180738 = 0; ; ++cursorRow_180738)
		{
			result = unk_1806EC >> 16;
			if ((signed int)(unsigned __int16)cursorRow_180738 >= unk_1806EC >> 16)
				break;
			LOWORD(cursorColumn_18073A) = 0;
			while ((unsigned __int16)cursorColumn_18073A < (cursorPosY_1806E8 >> 16))
			{
				v3 = (x_BYTE*)cursorBackgroundPtr_180714++;
				v4 = (x_BYTE*)ptrScreenPos_180708++;
				*v4 = *v3;
				if (ptrScreenPos_180708 >= (uint8_t*)0xAFFFE + 2)
				{
					++HIWORD(cursorColumn_18073A);
					SetVgaPaletteBank_9951B(SHIWORD(cursorColumn_18073A));
					ptrScreenPos_180708 -= 0x10000;
				}
				LOWORD(cursorColumn_18073A) = cursorColumn_18073A + 1;
			}
			ptrScreenPos_180708 += screenWidth_18062C - (cursorPosY_1806E8 >> 16);
			if (ptrScreenPos_180708 >= (uint8_t*)0xAFFFE + 2)
			{
				++HIWORD(cursorColumn_18073A);
				SetVgaPaletteBank_9951B(SHIWORD(cursorColumn_18073A));
				ptrScreenPos_180708 -= 0x10000;
			}
		}
	}
	return result;
}

//----- (0008CA16) --------------------------------------------------------
int RestoreCursorBackground_8CA16()//26da16
{
	int result; // eax
	x_BYTE* v1; // eax
	x_BYTE* v2; // edx

	cursorBackgroundPtr_180714 = cursorBackground_180700;
	ptrScreenPos_180708 = cursorScreenIndex_180704 + pdwScreenBuffer_351628;
	for (cursorRow_180738 = 0; ; ++cursorRow_180738)
	{
		result = cursorVisibleWidth_1806FC >> 16;
		if (cursorRow_180738 >= cursorVisibleWidth_1806FC >> 16)
			break;
		LOWORD(cursorColumn_18073A) = 0;
		while (cursorColumn_18073A < cursorY_1806F8 >> 16)
		{
			v1 = (x_BYTE*)cursorBackgroundPtr_180714++;
			v2 = (x_BYTE*)ptrScreenPos_180708++;
			*v2 = *v1;
			LOWORD(cursorColumn_18073A) = cursorColumn_18073A + 1;
		}
		ptrScreenPos_180708 += screenWidth_18062C - (cursorX_1806F8 >> 16);
	}
	return result;
}

//----- (0008CB1F) --------------------------------------------------------
void ClearCursorDrawnFlag_8CB1F()//26db1f
{
	cursorDrawn_E3758 = 0;
}

void MouseEvents(uint32_t buttons, int x, int y)
{
	// playback: the recording drives the mouse, but the menus outside the level and the pause
	// menu keep the host mouse, otherwise there is no way to reach a playback at all
	if (m_InputRecorder != nullptr && m_InputRecorder->m_IsPlaying && g_inGameLoop
		&& !(gameState_D41A4.OptionsSettingFlag_24 & GAME_PAUSED))
		return;
	UpdateMouseEventData_8CB3A(buttons, x, y);
};

int x_BYTE_E3767;
int mouseButtonStates_180710;
int mouseRepeatTimer_18073E;
int word_E3764;
int x_WORD_180742;
int mouseEventFlagB_180748;
int mouseEventFlagA_180740;

//----- (0008CB3A) --------------------------------------------------------
void UpdateMouseEventData_8CB3A(uint32_t mouse_states, int32_t mouse_posx, int32_t mouse_posy)//26db3a
{
	int helpWidth = 640;
	int helpHeight = 480;
	if (videoMode_180660 != 1)
		if (!DefaultResolutions())
		{
			helpWidth = screenWidth_18062C;
			helpHeight = screenHeight_180624;
		}

	//!!!!!!!! debug
	if (mouseturnoff)
	{
		mouse_posx = 0x140;
		mouse_posy = 0xc8;
	}

	if (mouseInstalled_E3768)//2b4768 - 00000001
	{
		x_BYTE_E3767 = 1;
		mouseButtonStates_180710 = mouse_states;//[ebp-c] - 00000001 // state of buttons 1 -change position
		/*if (videoMode_180660 & 8)
		{
			temp_mouse_x = mouse_posx >> 3;//[ss:ebp-8] 0 - x
			temp_mouse_y = mouse_posy >> 3;//[ss:ebp-4] 0 - y
			//only for text mode?
		}*/

		bool warpMouse = false;
		if (mouse_posx > (helpWidth - 2))
		{
			mouse_posx = (helpWidth - 2);
			warpMouse = true;
		}
		if (mouse_posy > (helpHeight - 2))
		{
			mouse_posy = (helpHeight - 2);
			warpMouse = true;
		}

		mouse_E3760.x = mouse_posx; //set x
		mouse_E3760.y = mouse_posy; //set y

		if (warpMouse)
			VGA_Set_mouse(mouse_posx, mouse_posy);

		if (mouseButtonStates_180710 & 2) // left button pressed
		{
			if (!mouseLeftButton2_18074C)//first cycle after press
			{
				if (mouseRepeatTimer_18073E <= 0)
				{
					//mouse_state = word_E3764;
					mouseRepeatTimer_18073E = word_E3764;
				}
				else
				{
					x_WORD_180742 = 1;
				}
			}
			if (!mouseLeftButton2_18074C && !mouseLeftButton_180746)//first cycle after press and ...
			{
				mouseLeftButton_180746 = 1;
				mousePosX_E375C = mouse_posx;
				//mouse_state = temp_mouse_y;
				mousePosY_E375E = mouse_posy;
			}
			mouseLeftButton2_18074C = 1;
		}
		if (mouseButtonStates_180710 & 4)// left button released
			mouseLeftButton2_18074C = 0;
		if (mouseButtonStates_180710 & 8)// right button pressed
		{
			if (!mouseRightButton2_18074A && !mouseRightButton_180744)//first cycle after press and ...
			{
				mouseRightButton_180744 = 1;
				mousePosX_E375C = mouse_posx;
				//mouse_state = temp_mouse_y;
				mousePosY_E375E = mouse_posy;
			}
			mouseRightButton2_18074A = 1;
		}
		if (mouseButtonStates_180710 & 0x10)// right button released
			mouseRightButton2_18074A = 0;
		if (mouseButtonStates_180710 & 0x20)// middle button pressed?
		{
			if (!mouseEventFlagB_180748 && !mouseEventFlagA_180740)
			{
				mouseEventFlagA_180740 = 1;
				mousePosX_E375C = mouse_posx;
				//mouse_state = temp_mouse_y;
				mousePosY_E375E = mouse_posy;
			}
			mouseEventFlagB_180748 = 1;
		}
		if (mouseButtonStates_180710 & 0x40)// middle button released?
			mouseEventFlagB_180748 = 0;
		if (!cursorDrawn_E3758)
		{
			HIWORD(cursorParam_180734) = vgaPaletteBank_E3BA4;
			SaveCursorBackground_8C839();//draw cursor maybe
			/*mouse_state = */UpdateMouseCursorPosition_8C329();
			if (videoMode_180660 & 8)
				/*mouse_state = */SetVgaPaletteBank_9951B(SHIWORD(cursorParam_180734));
		}
	}
}

// 8CB3A: could not find valid save-restore pair for ebx
// 8CB3A: could not find valid save-restore pair for ebp
// 8CB3A: could not find valid save-restore pair for edi
// 8CB3A: could not find valid save-restore pair for esi
// E3758: using guessed type int cursorDrawn_E3758;
// E375C: using guessed type __int16 mousePosX_E375C;
// E375E: using guessed type __int16 mousePosY_E375E;
// E3760: using guessed type int x_DWORD_E3760;
// E3767: using guessed type char x_BYTE_E3767;
// E3768: using guessed type int mouseInstalled_E3768;
// E3BA4: using guessed type __int16 vgaPaletteBank_E3BA4;
// 180660: using guessed type __int16 videoMode_180660;
// 180710: using guessed type int mouseButtonStates_180710;
// 180734: using guessed type int cursorParam_180734;
// 18073E: using guessed type __int16 mouseRepeatTimer_18073E;
// 180740: using guessed type __int16 mouseEventFlagA_180740;
// 180742: using guessed type __int16 x_WORD_180742;
// 180744: using guessed type __int16 mouseRightButton_180744;
// 180746: using guessed type __int16 mouseLeftButton_180746;
// 180748: using guessed type __int16 mouseEventFlagB_180748;
// 18074A: using guessed type __int16 mouseRightButton2_18074A;
// 18074C: using guessed type __int16 mouseLeftButton2_18074C;

//----- (0008CEDF) --------------------------------------------------------
signed int InstallMouse_8CEDF()//26dedf
{
	//__int16 v1; // [esp+0h] [ebp-54h]
	//__int16 v2; // [esp+1Ch] [ebp-38h]
	//__int16 v3; // [esp+24h] [ebp-30h]
	void(*v4)(uint32_t a1, int32_t a2, int32_t a4); // [esp+28h] [ebp-2Ch]
	//__int16 v5; // [esp+38h] [ebp-1Ch]
	void(*v6)(uint32_t a1, int32_t a2, int32_t a4); // [esp+44h] [ebp-10h]
	//int v7; // [esp+48h] [ebp-Ch]
	int i; // [esp+4Ch] [ebp-8h]

	mouseInstalled_E3768 = 0;
	//segread((SREGS*)&v5);
	//v2 = 0;
//removed int386(0x33, (REGS*)&v2, (REGS*)&v1);//mouse reset
	//if ( v1 != -1 )
	//  return 0;
	//v2 = 0xc;
	//v3 = 127;
	//fix it LOWORD(v7) = __CS__;
	v6 = UpdateMouseEventData_8CB3A;
	v4 = UpdateMouseEventData_8CB3A;
	//fix it v5 = __CS__;
	/*
	adress: 160:26db3a - sub_8CB3A
	mask:7f
	http://stanislavs.org/helppc/int_33-c.html
	*/
	//int386x(0x33, (REGS*)&v2, (REGS*)&v1, (SREGS*)&v5);//set mouse subroutine
//removed sub_8D12F_set_mouse_viewport(); //set min/max of viewport
	if (!cursorData_180730)
		cursorData_180730 = (uint8_t*)Malloc_83CD0(4096);//image buffers?-blit?
	if (!cursorBackground_180700)
		cursorBackground_180700 = (uint8_t*)Malloc_83CD0(4096);
	if (!cursorBackground2_1806F0)
		cursorBackground2_1806F0 = (uint8_t*)Malloc_83CD0(4096);
	if (!cursorData_180730 || !cursorBackground_180700 || !cursorBackground2_1806F0)
		return 0;
	cursorWidth_18072C = 0;
	cursorHeight_18072E = 0;
	LOWORD(cursorParam_180734) = 64;
	for (i = 0; i < 4096; i++)
		*(x_BYTE*)(i + cursorData_180730) = -2;
	if (mouseInstalled2_180720)
		;// fix it! SetCursor_8CD27((uint8_t**)mouseInstalled2_180720);
	//v2 = 2;
//removed int386(0x33, (REGS*)&v2, (REGS*)&v1);//hide mouse
	if (videoMode_180660 & 8)
	{
		//v2 = 0xF;
		//v3 = 1;
		LOWORD(v4) = 1;
		//removed int386(0x33, (REGS*)&v2, (REGS*)&v1);//set pixel ratio
	}
	mouseInstalled_E3768 = 1;//fix it
	return 1;
}
// 99DE6: using guessed type x_DWORD segread(x_DWORD);
// 99E10: using guessed type x_DWORD int386x(x_DWORD, x_DWORD, x_DWORD, x_DWORD);
// E3768: using guessed type int mouseInstalled_E3768;
// 180660: using guessed type __int16 videoMode_180660;
// 1806F0: using guessed type int cursorBackground2_1806F0;
// 180700: using guessed type int cursorBackground_180700;
// 180720: using guessed type int mouseInstalled2_180720;
// 18072C: using guessed type __int16 cursorWidth_18072C;
// 18072E: using guessed type __int16 cursorHeight_18072E;
// 180730: using guessed type int cursorData_180730;
// 180734: using guessed type int cursorParam_180734;

//----- (0008D12F) --------------------------------------------------------
// int sub_8D12F_set_mouse_viewport()//26e12f NOTE: unused -> removed 

/*
bool Sprite::loadSprite(uint8 * tabData, uint8 * spriteData, uint32 offset,
	bool rle)
{
	assert(tabData);
	assert(spriteData);

	uint8 *tabEntry = tabData + offset * TABENTRY_SIZE;

	uint32 spriteOffset = READ_LE_UINT32(tabEntry);

	tabEntry += 4;
	width_ = *tabEntry;
	tabEntry += 1;
	height_ = *tabEntry;

	if (width_ == 0 || height_ == 0)
		return true;

	stride_ = ceil8(width_);
	uint8 *spriteBlocks = spriteData + spriteOffset;

	sprite_data_ = new uint8[stride_ * height_];
	memset(sprite_data_, 255, stride_ * height_);

	uint8 *currentPixel;

	if (rle) {
		for (int i = 0; i < height_; ++i) {
			int spriteWidth = width_;
			currentPixel = sprite_data_ + i * stride_;

			uint8 b = *spriteBlocks++;
			int runLength = b < 128 ? b : -(256 - b);
			while (runLength != 0) {
				spriteWidth -= runLength;

				if (runLength > 0) {
					if (currentPixel < sprite_data_)
						currentPixel = sprite_data_;
					if (currentPixel + runLength >
						sprite_data_ + height_ * stride_)
						runLength =
						sprite_data_ + height_ * stride_ -
						currentPixel;
					// pixel run
					for (int j = 0; j < runLength; ++j)
						*currentPixel++ = *spriteBlocks++;
				}
				else if (runLength < 0) {
					// transparent run
					runLength *= -1;
					if (currentPixel < sprite_data_)
						currentPixel = sprite_data_;
					if (currentPixel + runLength >
						sprite_data_ + height_ * stride_)
						runLength =
						sprite_data_ + height_ * stride_ -
						currentPixel;
					memset(currentPixel, 255, runLength);
					currentPixel += runLength;
				}
				else if (runLength == 0) {
					// end of the row
					spriteWidth = 0;
				}

				b = *spriteBlocks++;
				runLength = b < 128 ? b : -(256 - b);
			}
		}
	}
	else {
		for (int j = 0; j < height_; ++j) {
			currentPixel = sprite_data_ + j * stride_;

			for (int i = 0; i < width_; i += PIXELS_PER_BLOCK) {
				unpackBlocks1(spriteBlocks, currentPixel);

				spriteBlocks += BLOCK_LENGTH;
				currentPixel += PIXELS_PER_BLOCK;
			}
		}
	}

	return true;
}
*/

//----- (0009025C) --------------------------------------------------------
int SetMouseBoundsScaled_9025C(__int16 a1, __int16 a2, __int16 a3, __int16 a4, unsigned __int16 a5, __int16 a6)//27125c
{
	__int16 v6; // bx
	__int16 v7; // cx
	__int16 v8; // dx
	uint8_t* v10; // edi
	int v11; // eax
	unsigned int v12; // ecx
	int v13; // ecx
	int v14; // esi
	int v15; // esi
	int v16; // [esp+4h] [ebp-4h]

	//fix it
	v16 = 0;
	//fix it

	v6 = 2 * viewportWidth_180634;
	v7 = 2 * viewportY_180650;
	v8 = 2 * viewportHeight_180630;
	if (a1 < (signed __int16)(2 * viewportX_18063C))
		a1 = 2 * viewportX_18063C;
	if (a3 < (signed __int16)(2 * viewportX_18063C))
		return v16;
	if (a1 >= v6)
		return v16;
	if (a3 >= v6)
		a3 = 2 * viewportWidth_180634;
	if (a2 < v7)
		a2 = 2 * viewportY_180650;
	if (a4 < v7)
		return v16;
	if (a2 >= v8)
		return v16;
	if (a4 >= v8)
		a4 = 2 * viewportHeight_180630;
	/*v10 = (x_BYTE*)(320 * ((unsigned int)(unsigned __int16)a2 >> 1)
		+ (int)pdwScreenBuffer_351628
		+ ((unsigned int)(unsigned __int16)a1 >> 1));*/
	v10 = &pdwScreenBuffer_351628[320 * (a2 >> 1) + (a1 >> 1)];
	v11 = a5;
	v12 = (unsigned __int16)a3;
	if (a3 == a1)
	{
		v13 = (unsigned __int16)a4;
		LOWORD(v13) = (unsigned __int16)(a4 - a2) >> 1;
		if (a6 & 4)
		{
			v14 = x_DWORD_E3890;
			do
			{
				BYTE1(v11) = *v10;
				*v10 = *(x_BYTE*)(v11 + v14);
				v10 += 320;
				--v13;
			} while (v13);
		}
		else
		{
			do
			{
				*v10 = a5;
				v10 += 320;
				--v13;
			} while (v13);
		}
	}
	else
	{
		LOWORD(v12) = (unsigned __int16)(a3 - a1) >> 1;
		if (a6 & 4)
		{
			v15 = x_DWORD_E3890;
			do
			{
				BYTE1(v11) = *v10;
				*v10++ = *(x_BYTE*)(v11 + v15);
				--v12;
			} while (v12);
		}
		else
		{
			memset(v10, a5, v12);
		}
	}
	return v16;
}

//----- (0009A10A) --------------------------------------------------------
signed int CheckKeyboard_9A10A(/*signed int result*/)//27B10a
{
	if (CommandLineParams.DoShowDebugPerifery())ShowPerifery();

	if (keyboardFlag_E4CA4)
		return 1;
	return VGA_check_standart_input_status();
	//fix it:__asm { int     21h; DOS - CHECK STANDARD INPUT STATUS }
	/*
	on return:
	  AL = 00 if no character available
		 = FF if character available

	  - checks STDIN for available characters
	  - character is not returned
	  - if Ctrl-Break is detected INT 23 is executed
	*/
	return 0;//fix it
}

//----- (0009D31C) --------------------------------------------------------
__int16 /*__fastcall*/ ReadKeyboardShiftFlags_9D31C(__int16 result)//27e31c
{
	if (CommandLineParams.DoShowDebugPerifery())ShowPerifery();

	bool v1; // zf

	//fix it:__asm { int     16h; KEYBOARD - GET ENHANCED SHIFT FLAGS (AT model 339,XT2,XT286,PS) }
	keyShiftFlags_1806E5 = 0;
	if (result & 3)
		keyShiftFlags_1806E5 = 1;
	if (result & 4)
		keyShiftFlags_1806E5 |= 2u;
	v1 = (result & 8) == 0;
	if (result & 8)
	{
		v1 = ((unsigned __int8)keyShiftFlags_1806E5 | 4) == 0;
		keyShiftFlags_1806E5 |= 4u;
	}
	LastPressedKey_1806E4 = 0;
	//fix it:__asm { int     16h; KEYBOARD - CHECK ENHANCED KEYSTROKE (AT model 339,XT2,XT286,PS) }
	if (!v1)
	{
		//fix it:__asm { int     16h; KEYBOARD - GET ENHANCED KEYSTROKE (AT model 339,XT2,XT286,PS) }
		LastPressedKey_1806E4 = 16;
	}
	return result;
}
// 1806E4: using guessed type char x_BYTE_1806E4;
// 1806E5: using guessed type char keyShiftFlags_1806E5;
