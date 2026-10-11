#include "PlayerInput.h"

#include "../utilities/DataFileIO.h"
#include "Animation.h"
#include "Basic.h"
#include "Events.h"
#include "GameUI.h"
#include "Graphics.h"
#include "Level.h"
#include "MenusAndIntros.h"
#include "Sound.h"
#include "engine_support.h"
#include "EventDispatcher.h"
#include "DatTabIndexes.h"


char CURSOR_SPRITE_INDEX_D419E = 0; // weak//2a519e
char IsPauseMenuOpen_D47D9 = 0; // weak
char x_BYTE_E36DC[124] = { 
0x00,0x27,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x30,0x2D,0x3D,0x08,0x09,
0x51,0x57,0x45,0x52,0x54,0x59,0x55,0x49,0x4F,0x50,0x5B,0x5D,0x0D,0x00,0x41,0x53,
0x44,0x46,0x47,0x48,0x4A,0x4B,0x4C,0x3B,0x27,0x23,0x00,0x5C,0x5A,0x58,0x43,0x56,
0x42,0x4E,0x4D,0x2C,0x2E,0x2F,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2D,0x00,0x00,0x00,0x2B,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x2F,0x00,0x00,0x28,0x29,0x2F,0x2A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x2E,0x0D,0x00,0x40,0x01,0xC8,0x00,0x00,0x00,0x00,0x00 }; // weak//2b46dc
__int16 x_WORD_180463; // weak
int x_DWORD_18065C; // weak
int x_DWORD_180640_help_screen_height; // weak
int x_DWORD_18064C_help_screen_width; // weak
int x_DWORD_180658; // weak
int x_DWORD_180638; // weak
int x_DWORD_180654; // weak
char x_BYTE_1810A3[320]; // fix it -  weak
int x_DWORD_17FF14; // weak
int16_t x_WORD_17FF18; // weak
int x_DWORD_17FF20; // weak
int x_DWORD_17FF24; // weak
int x_DWORD_17FF28 = 0; // weak
int x_DWORD_17FF38; // weak
int x_DWORD_17FF40; // weak
int16_t x_WORD_1811E4[400]; // fix it -  weak


char sub_5BF10();
void sub_70940();
void HandleOptionsMenuButtonClick_19A70();
void SwitchHelpMode_18AA0();
int SelectSpell_6D4F0(type_str_611* a1, int16_t mouseX);
void ChangeGameResolution_46B40();
void sub_75C50();
int sub_906B4();
signed int sub_90668(int a1);
void sub_90E07_VGA_set_video_mode_alt_and_Palette(TColor* Palette);


int debugcounter_47560 = 0;
//----- (00017190) --------------------------------------------------------
void ProcessKeyboardPresses_17190()//1f8190
{
	type_entity_0x6E8E* event; // edx

	if (CommandLineParams.DoOffPause5()) {
		if (debugcounter_47560 == 5)
		{
			x_D41A0_BYTEARRAY_4_struct.OptionsSettingFlag_24 &= 0xfe;
			//SetPauseMenuClosed_41B60();
			/*x_D41A0_BYTEARRAY_4_struct.byteindex_205 = 0;
			x_D41A0_BYTEARRAY_4_struct.byteindex_206 = 0;
			SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]);
			SetMenuCursorPosition_52E90(
				&D41A0_BYTESTR_0.array_0x2BDE[D41A0_BYTESTR_0.word_0xc],
				D41A0_BYTESTR_0.array_0x2BDE[D41A0_BYTESTR_0.word_0xc].MenuState_0x3DF_2BE4_12221,
				1);
			if (x_BYTE_E37FD)
				sub_8E160_sound_proc15_startsequence(D41A0_BYTESTR_0.dword_0x235, 0x7Fu);
			QueryCdAudioStatus_86930(x_WORD_1803EC);
			if (x_D41A0_BYTEARRAY_4_struct.byteindex_225)
				x_D41A0_BYTEARRAY_4_struct.byteindex_225 = 1;
			x_D41A0_BYTEARRAY_4_struct.byteindex_208 = sub_55C00(x_D41A0_BYTEARRAY_4_struct.levelnumber_43w);*/
		}
	}

	if (!(x_D41A0_BYTEARRAY_4_struct.setting_byte1_22 & 4) || sub_473E0() == 0)
	{
		if (!D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0)
		{
			if (!(D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dw_w_b_0_2BDE_11230.byte[2] & 0x20))
			{
				event = Entities_EA3E4[D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
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
						if (x_D41A0_BYTEARRAY_4_struct.setting_byte2_23 < 0)
						{
							if (x_D41A0_BYTEARRAY_4_struct.setting_byte4_25 & 0x20)
								x_D41A0_BYTEARRAY_4_struct.setting_byte4_25 &= 0xDF;
							else
								x_D41A0_BYTEARRAY_4_struct.setting_byte4_25 |= 0x20;
						}
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x2f: {//v
						if (x_D41A0_BYTEARRAY_4_struct.setting_byte2_23 < 0)
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
						if (x_D41A0_BYTEARRAY_4_struct.setting_byte2_23 < 0)
						{
							if (pressedKeys_180664[54])
								sub_70940();
							else
								HandleButtonClick_191B0(4, 64);
						}
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x20: {//d
						if (x_D41A0_BYTEARRAY_4_struct.setting_byte2_23 < 0)
						{
							sub_5BF10();
							pressedKeys_180664[56] = 0;
							if (x_D41A0_BYTEARRAY_4_struct.OptionsSettingFlag_24 & 0x10)
								x_D41A0_BYTEARRAY_4_struct.OptionsSettingFlag_24 &= 0xEF;
							else
								x_D41A0_BYTEARRAY_4_struct.OptionsSettingFlag_24 |= 0x10;
						}
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x23: {//h
						D41A0_0.m_GameSettings.str_0x2192.xxxx_0x2193 ^= 1u;
						LastPressedKey_1806E4 = 0;
						break;
					}
					}
					if (pressedKeys_180664[24])
					{
						D41A0_0.byte_0x36E03 = 2;
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
						if (x_D41A0_BYTEARRAY_4_struct.setting_byte2_23 < 0)
							HandleButtonClick_191B0(26, 0);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x13: {//r
						if (x_D41A0_BYTEARRAY_4_struct.setting_byte2_23 < 0 && !(x_D41A0_BYTEARRAY_4_struct.setting_byte1_22 & Setting::MULTIPLAYER_MODE))
						{
							D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dw_w_b_0_2BDE_11230.byte[2] |= 0xCu;
							D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dword_0x3E6_2BE4_12228.CastleEntityIndex_0x3A_58 = 0;
						}
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x20: {//d
						if (x_D41A0_BYTEARRAY_4_struct.setting_byte2_23 < 0)
						{
							D41A0_0.stages_0x3654C[D41A0_0.struct_0x3659C[D41A0_0.LevelIndex_0xc].substr_3659C.ObjectiveText_1].stages_3654C_byte0 |= 2u;
						}
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x21: {//f
						if (x_D41A0_BYTEARRAY_4_struct.setting_byte2_23 < 0)
						{
							D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dw_w_b_0_2BDE_11230.byte[2] |= 4u;
						}
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x25: {//k
						if (!(x_D41A0_BYTEARRAY_4_struct.setting_byte1_22 & 0x8010))
							event->life_0x8 = -1;
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x26: {//l remove castle stage
						if (event->dword_0xA4_164x->CastleEntityIndex_0x3A_58)
							HandleButtonClick_191B0(42, 0);
						LastPressedKey_1806E4 = 0;
						break;
					}
					case 0x2e: {//c
						if (x_D41A0_BYTEARRAY_4_struct.setting_byte2_23 < 0)
						{
							D41A0_0.struct_0x3659C[D41A0_0.LevelIndex_0xc].substr_3659C.IsLevelEnd_0 = 1;
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
						x_D41A0_BYTEARRAY_4_struct.byteindex_205 = (x_D41A0_BYTEARRAY_4_struct.byteindex_205 == 0);
						x_D41A0_BYTEARRAY_4_struct.byteindex_206 = x_D41A0_BYTEARRAY_4_struct.byteindex_205;
						if (x_D41A0_BYTEARRAY_4_struct.byteindex_205 && x_D41A0_BYTEARRAY_4_struct.OptionsSettingFlag_24 & GAME_PAUSED)
							SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]); //Set cursor to Null (Don't Draw)
						if (!x_D41A0_BYTEARRAY_4_struct.byteindex_206 && x_D41A0_BYTEARRAY_4_struct.OptionsSettingFlag_24 & GAME_PAUSED)
							SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[CURSOR_SPRITE_INDEX_D419E]); //fix it LOBYTE(v0) = sub_8CD27(dword_EB394 + 6 * (unsigned __int8)byte_D419E);
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
						if (!(x_D41A0_BYTEARRAY_4_struct.setting_byte1_22 & Setting::MULTIPLAYER_MODE) && (!IsRecordingOrPlaying()))
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
							sub_70940();
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
						if (!(x_D41A0_BYTEARRAY_4_struct.OptionsSettingFlag_24 & GAME_PAUSED)
							&& x_WORD_180660_VGA_type_resolution & 1
							&& (unk_18058Cstr.x_WORD_1805C2_joystick == 0x01 || unk_18058Cstr.x_WORD_1805C2_joystick == 0x08 || unk_18058Cstr.x_WORD_1805C2_joystick == 0x0c))
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
					D41A0_0.byte_counter_current_objective_box_0x36E04 = 2;
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

	//v0 = x_D41A0_BYTEARRAY_4_struct.dwordindex_0;
	//v1 = x_D41A0_BYTEARRAY_4_struct.OptionsSettingFlag_24  ^ 1;
	x_D41A0_BYTEARRAY_4_struct.OptionsSettingFlag_24 ^= GAME_PAUSED;
	if (x_D41A0_BYTEARRAY_4_struct.OptionsSettingFlag_24 & GAME_PAUSED)
	{
		if (D41A0_0.byte_0x36E0B & 2)//if ( *(0x36E09 + x_D41A0_BYTEARRAY_0 + 2) & 2 )
		{
			x_D41A0_BYTEARRAY_4_struct.OptionsSettingFlag_24 &= 0xFEu;
		}
		else
		{
			D41A0_0.byte_counter_current_objective_box_0x36E04 = 0;
			v2 = x_D41A0_BYTEARRAY_4_struct.byteindex_206;
			str_unk_1804B0ar.byte_0xaa = -1;
			if (!v2 && (unk_18058Cstr.x_WORD_1805C2_joystick == 7 || unk_18058Cstr.x_WORD_1805C2_joystick == 1 || unk_18058Cstr.x_WORD_1805C2_joystick == 2))
				SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[CURSOR_SPRITE_INDEX_D419E]);
			//SetCursor_8CD27(xy_DWORD_17DED4_spritestr[39]);

			SetPausedMenuOpen_41AF0();
			if (!x_D41A0_BYTEARRAY_4_struct.byteindex_206)
				SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[CURSOR_SPRITE_INDEX_D419E]);
			SetMenuCursorPosition_52E90(
				&D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc],
				D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221, true, gameUiScale);
			if (soundActive_E3799)
				EndSample_8D8F0();
			if (musicActive_E37FD)
				StopMusic_8E020();
			if (x_D41A0_BYTEARRAY_4_struct.byteindex_225)
				x_D41A0_BYTEARRAY_4_struct.byteindex_225 = 2;
		}
	}
	else
	{
		SetPauseMenuClosed_41B60();
		//v3 = x_D41A0_BYTEARRAY_4_struct.dwordindex_0;
		//v4 = **filearray_2aa18c[0]; //fix it
		x_D41A0_BYTEARRAY_4_struct.byteindex_205 = 0;
		x_D41A0_BYTEARRAY_4_struct.byteindex_206 = 0;
		SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]); //Set cursor to Null (Don't Draw)
		SetMenuCursorPosition_52E90(
			&D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc],
			D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221, true, gameUiScale);
		if (musicActive_E37FD)
			StartMusic_8E160(D41A0_0.maptypeMusic_0x235, 0x7Fu);
		QueryCdAudioStatus_86930(x_WORD_1803EC);
		if (x_D41A0_BYTEARRAY_4_struct.byteindex_225)
			x_D41A0_BYTEARRAY_4_struct.byteindex_225 = 1;
	}
	//LOBYTE(result) = sub_55C00(x_D41A0_BYTEARRAY_4_struct.levelnumber_43);
	x_D41A0_BYTEARRAY_4_struct.byteindex_208 = DataFileIO::sub_55C00_TestSaveFile2(x_D41A0_BYTEARRAY_4_struct.levelnumber_43w);
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
	if (!(D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dw_w_b_0_2BDE_11230.byte[2] & 0x20))
	{
		switch (D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221)
		{
		case 0:
		case 4:
			if (D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0)
				goto LABEL_292;
			if (x_D41A0_BYTEARRAY_4_struct.OptionsSettingFlag_24 & GAME_PAUSED)
				ReadPauseMenuEvents_197F0();
			ProcessKeyboardPresses_17190();//test FnX
			v8x = Entities_EA3E4[D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
			D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].entityIndex_0x6E3E_byte5 = 0;
			if (unk_18058Cstr.MouseButtonState_18059C & 0x10)
			{
				if (v8x->life_0x8 >= 0)
					HandleButtonClick_191B0(20, 5);
			}
			else if (pressedKeys_180664[56] || pressedKeys_180664[x_BYTE_EB39E_keys[9]])
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

				if (unk_18058Cstr.MouseButtonState_18059C & 1)
				{
					sub_18DA0(v8x, 1, 1);
					unk_18058Cstr.MouseButtonState_18059C &= 0xFE;
				}
				else if (unk_18058Cstr.MouseButtonState_18059C & 2)
				{
					sub_18DA0(v8x, 2, 1);
					unk_18058Cstr.MouseButtonState_18059C &= 0xFD;
				}
			}
			else if (pressedKeys_180664[42] || pressedKeys_180664[x_BYTE_EB39E_keys[8]]) //LShift or RShift
			{
				if (unk_18058Cstr.MouseButtonState_18059C & 1)
				{
					sub_18DA0(v8x, 1, 0);
					unk_18058Cstr.MouseButtonState_18059C &= 0xFE;
				}
				else if (unk_18058Cstr.MouseButtonState_18059C & 2)
				{
					sub_18DA0(v8x, 2, 0);
					unk_18058Cstr.MouseButtonState_18059C &= 0xFD;
				}
			}
			else
			{
				switch (LastPressedKey_1806E4)
				{
				case 0x1a: {//[
					if (!D41A0_0.m_GameSettings.m_Display.m_uiScreenSize)
					{
						if (D41A0_0.m_GameSettings.m_Graphics.m_wViewPortSize < 40)
						{
							D41A0_0.m_GameSettings.m_Graphics.m_wViewPortSize++;
							viewPort.ResizeViewPort_2CA90(D41A0_0.m_GameSettings.m_Graphics.m_wViewPortSize);
						}
					}
					LastPressedKey_1806E4 = 0;
					break;
				}
				case 0x1b: {//]
					if (!D41A0_0.m_GameSettings.m_Display.m_uiScreenSize)
					{
						if (D41A0_0.m_GameSettings.m_Graphics.m_wViewPortSize > 17)
						{
							D41A0_0.m_GameSettings.m_Graphics.m_wViewPortSize--;// = v10 - 1;
							viewPort.ResizeViewPort_2CA90(D41A0_0.m_GameSettings.m_Graphics.m_wViewPortSize);
						}
					}
					LastPressedKey_1806E4 = 0;
					break;
				}
				case 0x17://i
					if (!(x_D41A0_BYTEARRAY_4_struct.setting_byte4_25 & 2))
					{
						v11 = v8x->actionIndex_0x45_69;
						if (v11 != 2 && v11 != 3)
							HandleButtonClick_191B0(16, 0);
					}
					LastPressedKey_1806E4 = 0;
					goto LABEL_296;
				case 0x01://esc
					sub_18B30();
					LastPressedKey_1806E4 = 0;
					sub_1A7A0_fly_asistant();
					goto LABEL_306;
				case 0x39://space
					HandleButtonClick_191B0(15, 0);
					LastPressedKey_1806E4 = 0;
					break;
				}
				if (unk_18058Cstr.MouseButtonState_18059C & 1 && unk_18058Cstr.MouseButtonState_18059C & 2)
				{
					if (!disableLRButtonsMenuOpen && v8x->life_0x8 >= 0)
						HandleButtonClick_191B0(20, 6); //Mouse Open Map

					if (disableLRButtonsMenuOpen)
						unk_18058Cstr.MouseButtonState_18059C &= 0xFD;
					else
						unk_18058Cstr.MouseButtonState_18059C &= 0xFC;
				}
				if (LastPressedKey_1806E4 == x_BYTE_EB39E_keys[4])
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
				if (!pressedKeys_180664[47] || x_D41A0_BYTEARRAY_4_struct.byte_38591)//volume
				{
					if (pressedKeys_180664[50] && !x_D41A0_BYTEARRAY_4_struct.byte_38591)
						ChangeSoundLevel_19CA0(2u);
				}
				else
				{
					ChangeSoundLevel_19CA0(1u);
				}
				HandleMouseButtons_18F80(v8x);
			}
			if (LastPressedKey_1806E4 && x_BYTE_E36DC[LastPressedKey_1806E4] == 8)
			{
				HandleButtonClick_191B0(39, 1);
				LastPressedKey_1806E4 = 0;
			}
			HandleArrowKeyPresses_1A8A0();
			ComputeMousePlayerMovement_17060(unk_18058Cstr.x_DWORD_1805B0_mouse.x, unk_18058Cstr.x_DWORD_1805B0_mouse.y);
			sub_1A7A0_fly_asistant();
			goto LABEL_306;
		case 1:
			ProcessKeyboardPresses_17190();
			if ((!(unk_18058Cstr.MouseButtonState_18059C & 1) || !(unk_18058Cstr.MouseButtonState_18059C & 2)) && LastPressedKey_1806E4 != 0x1c)
				goto LABEL_296;
			LastPressedKey_1806E4 = 0;
			unk_18058Cstr.MouseButtonState_18059C &= 0xFC;
			HandleButtonClick_191B0(20, 0);
			sub_1A7A0_fly_asistant();
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
					v27 = x_BYTE_E36DC[LastPressedKey_1806E4];
					if (v27 >= 0x41u && v27 <= 0x5Au
						|| v27 >= 0x61u && v27 <= 0x7Au
						|| v27 >= 0x30u && v27 <= 0x39u
						|| v27 == 8
						|| v27 == 32)
					{
						HandleButtonClick_191B0(17, x_BYTE_E36DC[LastPressedKey_1806E4]);
					}
					goto LABEL_215;
				}
				D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].byte_0x3E1_2BE4_12223++;
				if (D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].byte_0x3E1_2BE4_12223 >= 4)
					D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].byte_0x3E1_2BE4_12223 = 0;
				HandleButtonClick_191B0(36, D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].byte_0x3E1_2BE4_12223);
			}
		LABEL_215:
			LastPressedKey_1806E4 = 0;
		LABEL_216:
			if (x_WORD_180660_VGA_type_resolution & 1)
				v28 = 400;
			else
				v28 = screenHeight_180624;
			v29 = v28 - 72;
			if (unk_18058Cstr.x_DWORD_1805B0_mouse.x < 4 || unk_18058Cstr.x_DWORD_1805B0_mouse.x >= 66 || v29 + 28 > unk_18058Cstr.x_DWORD_1805B0_mouse.y || v29 + 66 <= unk_18058Cstr.x_DWORD_1805B0_mouse.y)
			{
				if (unk_18058Cstr.x_DWORD_1805B0_mouse.x < 384 || unk_18058Cstr.x_DWORD_1805B0_mouse.x >= 654 || v29 + 26 > unk_18058Cstr.x_DWORD_1805B0_mouse.y || v29 + 48 <= unk_18058Cstr.x_DWORD_1805B0_mouse.y)
				{
					if (unk_18058Cstr.x_DWORD_1805B0_mouse.x < 82 || unk_18058Cstr.x_DWORD_1805B0_mouse.x >= 378 || v29 + 32 > unk_18058Cstr.x_DWORD_1805B0_mouse.y || v29 + 60 <= unk_18058Cstr.x_DWORD_1805B0_mouse.y)
					{
						if (unk_18058Cstr.x_DWORD_1805B0_mouse.x < 384 || unk_18058Cstr.x_DWORD_1805B0_mouse.x >= 440 || v29 + 4 > unk_18058Cstr.x_DWORD_1805B0_mouse.y || v29 + 22 <= unk_18058Cstr.x_DWORD_1805B0_mouse.y)
						{
							if (unk_18058Cstr.x_DWORD_1805B0_mouse.x < 474 || unk_18058Cstr.x_DWORD_1805B0_mouse.x >= 530 || v29 + 4 > unk_18058Cstr.x_DWORD_1805B0_mouse.y || v29 + 22 <= unk_18058Cstr.x_DWORD_1805B0_mouse.y)
							{
								if (unk_18058Cstr.x_DWORD_1805B0_mouse.x >= 564
									&& unk_18058Cstr.x_DWORD_1805B0_mouse.x < 620
									&& v29 + 4 <= unk_18058Cstr.x_DWORD_1805B0_mouse.y
									&& v29 + 22 > unk_18058Cstr.x_DWORD_1805B0_mouse.y
									&& unk_18058Cstr.MouseButtonState_18059C & 1)
								{
									HandleButtonClick_191B0(18, 0);
								}
							}
							else if (unk_18058Cstr.MouseButtonState_18059C & 1)
							{
								HandleButtonClick_191B0(44, 0);
							}
						}
						else if (unk_18058Cstr.MouseButtonState_18059C & 1)
						{
							HandleButtonClick_191B0(19, 0);
						}
						goto LABEL_292;
					}
					if (unk_18058Cstr.x_DWORD_1805B0_mouse.x < 82 || unk_18058Cstr.x_DWORD_1805B0_mouse.x >= 150 || v29 + 32 > unk_18058Cstr.x_DWORD_1805B0_mouse.y || v29 + 60 <= unk_18058Cstr.x_DWORD_1805B0_mouse.y)
					{
						if (unk_18058Cstr.x_DWORD_1805B0_mouse.x >= 152 && unk_18058Cstr.x_DWORD_1805B0_mouse.x < 220 && v29 + 32 <= unk_18058Cstr.x_DWORD_1805B0_mouse.y && v29 + 60 > unk_18058Cstr.x_DWORD_1805B0_mouse.y)
						{
							if (x_D41A0_BYTEARRAY_4_struct.showHelp_10)
								str_unk_1804B0ar.byte_0xa9 = 99;
							if (unk_18058Cstr.MouseButtonState_18059C & 1)
								HandleButtonClick_191B0(36, 1);
							goto LABEL_292;
						}
						if (unk_18058Cstr.x_DWORD_1805B0_mouse.x >= 222 && unk_18058Cstr.x_DWORD_1805B0_mouse.x < 290 && v29 + 32 <= unk_18058Cstr.x_DWORD_1805B0_mouse.y && v29 + 60 > unk_18058Cstr.x_DWORD_1805B0_mouse.y)
						{
							if (x_D41A0_BYTEARRAY_4_struct.showHelp_10)
								str_unk_1804B0ar.byte_0xa9 = 100;
							if (unk_18058Cstr.MouseButtonState_18059C & 1)
								HandleButtonClick_191B0(36, 2);
							goto LABEL_292;
						}
						if (unk_18058Cstr.x_DWORD_1805B0_mouse.x < 310 || unk_18058Cstr.x_DWORD_1805B0_mouse.x >= 378 || v29 + 32 > unk_18058Cstr.x_DWORD_1805B0_mouse.y || v29 + 60 <= unk_18058Cstr.x_DWORD_1805B0_mouse.y)
							goto LABEL_292;
						if (x_D41A0_BYTEARRAY_4_struct.showHelp_10)
							str_unk_1804B0ar.byte_0xa9 = 101;
						if (!(unk_18058Cstr.MouseButtonState_18059C & 1))
							goto LABEL_292;
						v32 = 3;
					}
					else
					{
						if (x_D41A0_BYTEARRAY_4_struct.showHelp_10)
							str_unk_1804B0ar.byte_0xa9 = 98;
						if (!(unk_18058Cstr.MouseButtonState_18059C & 1))
							goto LABEL_292;
						v32 = 0;
					}
					HandleButtonClick_191B0(36, v32);
					goto LABEL_292;
				}
				if (x_D41A0_BYTEARRAY_4_struct.showHelp_10)
					str_unk_1804B0ar.byte_0xa9 = 102;
				if (unk_18058Cstr.MouseButtonState_18059C & 1)
				{
					v30 = (unk_18058Cstr.x_DWORD_1805B0_mouse.x - 384) / 34;
					if (v30 >= D41A0_0.LevelIndex_0xc)
						v30++;
					HandleButtonClick_191B0(37, v30);
				}
			}
			else
			{
				if (x_D41A0_BYTEARRAY_4_struct.showHelp_10)
					str_unk_1804B0ar.byte_0xa9 = 97;
				if (unk_18058Cstr.MouseButtonState_18059C & 1)
					HandleButtonClick_191B0(
						35,
						4 * ((unk_18058Cstr.x_DWORD_1805B0_mouse.y - (v29 + 28)) / 20)
						+ ((unk_18058Cstr.x_DWORD_1805B0_mouse.x - 4 - (__CFSHL__((unk_18058Cstr.x_DWORD_1805B0_mouse.x - 4) >> 31, 4) + 16 * ((unk_18058Cstr.x_DWORD_1805B0_mouse.x - 4) >> 31))) >> 4));
			}
		LABEL_292:
			unk_18058Cstr.MouseButtonState_18059C &= 0xFC;
			sub_1A7A0_fly_asistant();
			goto LABEL_306;
		case 5:
		case 8:
			v33 = 0;
			v34 = 0;
			v12x = Entities_EA3E4[D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
			ProcessKeyboardPresses_17190();
			if (v12x->life_0x8 < 0)
			{
				v33 = 1;
			}
			else
			{
				v16 = v12x->dword_0xA4_164x->str_611.byte_0x457_1111;
				if (v16)
				{
					if (v16 <= 2u)
					{
						if ((v16 != 1 || unk_18058Cstr.MouseButtonState_18059C & 4) && (v12x->dword_0xA4_164x->str_611.byte_0x457_1111 != 2 || unk_18058Cstr.MouseButtonState_18059C & 8))
						{
							v23 = SelectSpell_6D4F0(&v12x->dword_0xA4_164x->str_611, unk_18058Cstr.x_DWORD_1805B0_mouse.x);
							v12x->dword_0xA4_164x->str_611.subSpellIndex_0x459_1113 = v23;
							HandleButtonClick_191B0(41, v23);
						}
						else
						{
							if (v12x->dword_0xA4_164x->str_611.byte_0x457_1111 == 1)
								D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0 = 31;
							else
								D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0 = 32;
							D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].str_0x6E3E_byte1 = v12x->dword_0xA4_164x->str_611.spellIndex_0x458_1112;
							D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].str_0x6E3E_byte2 = v12x->dword_0xA4_164x->str_611.subSpellIndex_0x459_1113;
							v12x->dword_0xA4_164x->str_611.byte_0x457_1111 = 0;
							MoveCursorToSelectedSpell_6D200(&D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc]);
						}
					}
				}
				else
				{
					v34 = 1;
					v17 = SelectSpellCategory_6D420(unk_18058Cstr.x_DWORD_1805B0_mouse.x, unk_18058Cstr.x_DWORD_1805B0_mouse.y);
					v12x->dword_0xA4_164x->str_611.spellIndex_0x458_1112 = v17;
					v18 = spellIndex_D94FF[v17];
					v19 = 1;
					if (!v12x->dword_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[v18] || !isCaveLevel_D41B6 && v18 == 25)
						v19 = 0;
					if (!v19)
						goto LABEL_122;
					sub_6D4C0(&v12x->dword_0xA4_164x->str_611);
					if (!(unk_18058Cstr.MouseButtonState_18059C & 1) && !(unk_18058Cstr.MouseButtonState_18059C & 2))
						goto LABEL_122;
					if (pressedKeys_180664[42] || pressedKeys_180664[x_BYTE_EB39E_keys[7]])
					{
						HandleButtonClick_191B0(38, v18);
						D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].str_0x6E3E_byte2 = 0;
						v21 = v12x->dword_0xA4_164x->str_611.array_0x3B5_949x.SpellIndex[v18];
						if (v21)
						{
							if (unk_18058Cstr.MouseButtonState_18059C & 1)
							{
								if (v21 != 1)
									D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].str_0x6E3E_byte2 = 1;
								goto LABEL_122;
							}
							if (v21 == 2)
								goto LABEL_122;
						}
						else if (unk_18058Cstr.MouseButtonState_18059C & 1)
						{
							D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].str_0x6E3E_byte2 = 1;
							goto LABEL_122;
						}
						D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].str_0x6E3E_byte2 = 2;
						goto LABEL_122;
					}
					if (unk_18058Cstr.MouseButtonState_18059C & 1 && unk_18058Cstr.MouseButtonState_18059C & 2)
					{
						HandleButtonClick_191B0(6, 64);
						D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].str_0x6E3E_byte1 = v12x->dword_0xA4_164x->str_611.spellIndex_0x458_1112;
					}
					else
					{
						v12x->dword_0xA4_164x->str_611.byte_0x457_1111 = ((unk_18058Cstr.MouseButtonState_18059C & 1) == 0) + 1;
						MoveCursorToSelectedSpell_6D200(&D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc]);
						HandleButtonClick_191B0(40, v12x->dword_0xA4_164x->str_611.spellIndex_0x458_1112);
						D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].str_0x6E3E_byte2 = v12x->dword_0xA4_164x->str_611.byte_0x457_1111;
					}
				}
			}
		LABEL_122:
			if (!(unk_18058Cstr.MouseButtonState_18059C & 0x10) && !(unk_18058Cstr.MouseButtonState_18059C & 4) && !(unk_18058Cstr.MouseButtonState_18059C & 8))
				v33 = 1;
			if (v33)
			{
				if (D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 == 5)
					HandleButtonClick_191B0(20, 0);
				else
					HandleButtonClick_191B0(20, 6);
			}
			else
			{
				if (v12x->life_0x8 >= 0)
				{
					if (unk_18058Cstr.x_WORD_1805C0_arrow_keys & 1)
						HandleButtonClick_191B0(6, 1);
					if (unk_18058Cstr.x_WORD_1805C0_arrow_keys & 2)
						HandleButtonClick_191B0(6, 2);
					if (unk_18058Cstr.x_WORD_1805C0_arrow_keys & 4)
						HandleButtonClick_191B0(6, 4);
					if (unk_18058Cstr.x_WORD_1805C0_arrow_keys & 8)
						HandleButtonClick_191B0(6, 8);
				}
				if (x_D41A0_BYTEARRAY_4_struct.setting_byte1_22 & 2
					&& v34
					&& !D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0)
				{
					HandleButtonClick_191B0(40, v12x->dword_0xA4_164x->str_611.spellIndex_0x458_1112);
					D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].str_0x6E3E_byte2 = 0;
				}
			}
			ComputeMousePlayerMovement_17060(v12x->dword_0xA4_164x->position_backup_20.x, v12x->dword_0xA4_164x->position_backup_20.y);
			LastPressedKey_1806E4 = 0;
			unk_18058Cstr.MouseButtonState_18059C &= 0xFC;
			sub_1A7A0_fly_asistant();
			goto LABEL_306;
		case 6:
		case 7:
			v24x = Entities_EA3E4[D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
			if (x_D41A0_BYTEARRAY_4_struct.OptionsSettingFlag_24 & GAME_PAUSED)
				ReadPauseMenuEvents_197F0();
			ProcessKeyboardPresses_17190();
			if (((unk_18058Cstr.MouseButtonState_18059C & 1 && unk_18058Cstr.MouseButtonState_18059C & 2) && !disableLRButtonsMenuOpen) || LastPressedKey_1806E4 == x_BYTE_EB39E_keys[4] || v24x->life_0x8 < 0)
			{
				HandleButtonClick_191B0(20, 0); // Close Map
			}
			else if (LastPressedKey_1806E4 == 1)
			{
				sub_18B30();
				LastPressedKey_1806E4 = 0;
			}
			else
			{
				if (unk_18058Cstr.MouseButtonState_18059C & 0x10)
				{
					HandleButtonClick_191B0(20, 8);
				}
				else if (pressedKeys_180664[56] || pressedKeys_180664[x_BYTE_EB39E_keys[9]] || pressedKeys_180664[x_BYTE_EB39E_keys[6]])
				{
					if (unk_18058Cstr.MouseButtonState_18059C & 1)
					{
						sub_18DA0(v24x, 1, 1);
						unk_18058Cstr.MouseButtonState_18059C &= 0xFE;
					}
					else if (unk_18058Cstr.MouseButtonState_18059C & 2)
					{
						sub_18DA0(v24x, 2, 1);
						unk_18058Cstr.MouseButtonState_18059C &= 0xFD;
					}
					else if (D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 != 7)
					{
						HandleButtonClick_191B0(20, 7);
					}
				}
				else if (pressedKeys_180664[42] || pressedKeys_180664[x_BYTE_EB39E_keys[8]])
				{
					if (unk_18058Cstr.MouseButtonState_18059C & 1)
					{
						sub_18DA0(v24x, 1, 0);
						unk_18058Cstr.MouseButtonState_18059C &= 0xFE;
					}
					else if (unk_18058Cstr.MouseButtonState_18059C & 2)
					{
						sub_18DA0(v24x, 2, 0);
						unk_18058Cstr.MouseButtonState_18059C &= 0xFD;
					}
				}
				else if (D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 != 6)
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
			if (!pressedKeys_180664[47] || x_D41A0_BYTEARRAY_4_struct.byte_38591)
			{
				if (pressedKeys_180664[50] && !x_D41A0_BYTEARRAY_4_struct.byte_38591)
					ChangeSoundLevel_19CA0(2u);
			}
			else
			{
				ChangeSoundLevel_19CA0(1u);
			}
			if (LastPressedKey_1806E4 && x_BYTE_E36DC[(unsigned __int8)LastPressedKey_1806E4] == 8)
			{
				HandleButtonClick_191B0(39, 1);
				LastPressedKey_1806E4 = 0;
			}
			HandleArrowKeyPresses_1A8A0();
			ComputeMousePlayerMovement_17060(unk_18058Cstr.x_DWORD_1805B0_mouse.x, unk_18058Cstr.x_DWORD_1805B0_mouse.y);
			LastPressedKey_1806E4 = 0;
			unk_18058Cstr.MouseButtonState_18059C &= 0xFC;
			sub_1A7A0_fly_asistant();
			goto LABEL_306;
		case 9:
		case 0xB:
			ReadOptionMenuEvents_19AB0();
			sub_1A7A0_fly_asistant();
			goto LABEL_306;
		case 0xA:
		case 0xC:
			AdjustVolume_1A070(turn);
			sub_1A7A0_fly_asistant();
			goto LABEL_306;
		case 0xD:
		case 0xE:
			ReadOkayCancelButtonEvents_19E00();
			goto LABEL_296;
		default:
		LABEL_296:
			sub_1A7A0_fly_asistant();
			goto LABEL_306;
		}
	}
	if (LastPressedKey_1806E4)
	{
		if (LastPressedKey_1806E4 >= 1u)
		{
			if (LastPressedKey_1806E4 <= 1u)
			{
				sub_18B30();
			}
			else if (LastPressedKey_1806E4 == 0x19)
			{
				if (!(x_D41A0_BYTEARRAY_4_struct.setting_byte1_22 & Setting::MULTIPLAYER_MODE) && (!IsRecordingOrPlaying()))
					PauseUnpauseGame_18BB0();
			}
		}
		LastPressedKey_1806E4 = 0;
	}
LABEL_306:
	if (!(unk_18058Cstr.MouseButtonState_18059C & 1))
		x_WORD_180746_mouse_left_button = 0;
	if (!(unk_18058Cstr.MouseButtonState_18059C & 2))
		x_WORD_180744_mouse_right_button = 0;
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
		D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0 = hiSetting;
		return;
	case 3:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
		return;
	case 4:
		if (D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0 != hiSetting && D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0)
			return;
		D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0 = hiSetting;
		D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].str_0x6E3E_byte1 |= loSetting;
		return;
	case 5:
	case 6://set Player movement
		if (D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0 != hiSetting && D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0)
			return;
		D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0 = hiSetting;
		D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].entityIndex_0x6E3E_byte5 |= loSetting;
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
		if (D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0 != hiSetting && D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0)
			return;
		D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0 = hiSetting;
		return;
	case 15:
		if (D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0 != hiSetting && D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0)
			return;
		if (Entities_EA3E4[D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240]->life_0x8 >= 0 || Entities_EA3E4[D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240]->actionIndex_0x45_69 != 3)
			return;
		D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0 = hiSetting;
		return;
	case 27:
		if (D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0 != hiSetting && D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0)
			return;
		if (!(D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dw_w_b_0_2BDE_11230.byte[2] & 2))
			return;
		D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0 = hiSetting;
		return;
	case 28:
		if (D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0 != hiSetting && D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0)
			return;
		if (!(D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dw_w_b_0_2BDE_11230.byte[2] & 4))
			return;
		D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0 = hiSetting;
		return;
	case 30:
		if (x_D41A0_BYTEARRAY_4_struct.setting_byte2_23 < 0
			|| (D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].WizardName_0x39f_2BFA_12157[0] == 'c')//cheat ?
			&& D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].WizardName_0x39f_2BFA_12157[1] == 'h'
			&& D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].WizardName_0x39f_2BFA_12157[2] == 'r'
			&& D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].WizardName_0x39f_2BFA_12157[3] == 'o'
			&& D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].WizardName_0x39f_2BFA_12157[4] == 'n'
			&& D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].WizardName_0x39f_2BFA_12157[5] == 'i'
			&& D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].WizardName_0x39f_2BFA_12157[6] == 'c'
			&& D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].WizardName_0x39f_2BFA_12157[7] == 'l'
			&& D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].WizardName_0x39f_2BFA_12157[8] == 'e')
		{
			if (D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0 == hiSetting || !D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0)
			{
				D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0 = hiSetting;
				D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].str_0x6E3E_byte1 = loSetting;
				return;
			}
		}
		return;
	case 39:
		D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0 = hiSetting;
		D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].str_0x6E3E_byte1 = loSetting;
		return;
	default:
		if (D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0 == hiSetting || !D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0)
		{
			D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0 = hiSetting;
			D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].str_0x6E3E_byte1 = loSetting;
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
		SetCurrentNotificationMessage_19760((char*)x_DWORD_E9C4C_langindexbuffer[(soundActive_E3799 != 0) + 390], 3u, 50);
		//Sound On / Sound Off

		EndSample_8D8F0();
		StopCdPlayback_86860(x_WORD_1803EC);
		soundActive_E3799 ^= 1u;
		return;
	case 3:
		if (!musicAble_E37FC)
			return;
		if (musicActive_E37FD)
		{
			StopMusic_8E020();
			SetCurrentNotificationMessage_19760((char*)x_DWORD_E9C4C_langindexbuffer[393], 3u, 50);//Music Off
			musicActive_E37FD = false;
		}
		else
		{
			v15 = D41A0_0.maptypeMusic_0x235;
			musicActive_E37FD = true;
			StartMusic_8E160(v15, 0x7Fu);
			SetCurrentNotificationMessage_19760(x_DWORD_E9C4C_langindexbuffer[392], 3u, 50);//Music On
		}
		return;
	case 4:
		if (!cdSpeechEnabled_E2A28)
			return;
		//v16 = x_D41A0_BYTEARRAY_4_struct.OptionsSettingFlag_24;
		if (x_D41A0_BYTEARRAY_4_struct.OptionsSettingFlag_24 & SPEECH_ENABLED)
			x_D41A0_BYTEARRAY_4_struct.OptionsSettingFlag_24 &= SPEECH_DISABLED;
		else
			x_D41A0_BYTEARRAY_4_struct.OptionsSettingFlag_24 |= SPEECH_ENABLED;
		SetCurrentNotificationMessage_19760(x_DWORD_E9C4C_langindexbuffer[((x_D41A0_BYTEARRAY_4_struct.OptionsSettingFlag_24 & SPEECH_ENABLED) == 0) + 469], 3u, 50);
		//	Speech On /	Speech Off
		return;
	case 5:
		if (x_D41A0_BYTEARRAY_4_struct.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
		{
			D41A0_0.byte_0x36DEA_fly_asistant = 0;
		}
		else
		{
			//v12 = D41A0_BYTESTR_0.byte_0x36DEA_fly_asistant == 0;
			D41A0_0.byte_0x36DEA_fly_asistant = !D41A0_0.byte_0x36DEA_fly_asistant;
			SetCurrentNotificationMessage_19760(x_DWORD_E9C4C_langindexbuffer[!D41A0_0.byte_0x36DEA_fly_asistant + 398], 3u, 50);
			// Flight Assistance On / Flight Assistance Off
		}
		return;
	case 6:
		if (x_D41A0_BYTEARRAY_4_struct.paletteMod_51 < 3u || x_D41A0_BYTEARRAY_4_struct.paletteSubMod_180)
			return;
		x_D41A0_BYTEARRAY_4_struct.setting_38402 = 1;
		v17 = D41A0_0.terrain_2FECE.MapType;
		if (v17 == MapType_t::Day)
		{
			if (a2)
			{
				// v3 = x_D41A0_BYTEARRAY_4_struct.dwordindex_0;
				// v19 = x_D41A0_BYTEARRAY_4[11] - 1;
				x_D41A0_BYTEARRAY_4_struct.brightness_11--; // = v19;
				if (x_D41A0_BYTEARRAY_4_struct.brightness_11 >= 0)
					goto LABEL_86;
				if (!a3)
				{
					x_D41A0_BYTEARRAY_4_struct.brightness_11 = 4;
					goto LABEL_86;
				}
			}
			else
			{
				// v3 = x_D41A0_BYTEARRAY_4_struct.dwordindex_0;
				// v18 = x_D41A0_BYTEARRAY_4[11] + 1;
				x_D41A0_BYTEARRAY_4_struct.brightness_11++; // = v18;
				if (x_D41A0_BYTEARRAY_4_struct.brightness_11 <= 4)
					goto LABEL_86;
				if (a3)
				{
					x_D41A0_BYTEARRAY_4_struct.brightness_11 = 4;
					goto LABEL_86;
				}
			}
			x_D41A0_BYTEARRAY_4_struct.brightness_11 = 0;
		LABEL_86:
			sub_47650(a3 /*, a2*/);
			return;
		}
		if (v17 == MapType_t::Cave)
		{
			if (a2)//v17==1
			{
				//v23 = x_D41A0_BYTEARRAY_4_struct.dwordindex_0;
				//v24 = x_D41A0_BYTEARRAY_4[13] - 1;
				x_D41A0_BYTEARRAY_4_struct.brightness_13--;// = v24;
				if (x_D41A0_BYTEARRAY_4_struct.brightness_13 < 0)
				{
					if (!a3)
					{
						x_D41A0_BYTEARRAY_4_struct.brightness_13 = 4;
						sub_47650(0/*, a2*/);
						return;
					}
					goto LABEL_85;
				}
			}
			else
			{
				//v23 = x_D41A0_BYTEARRAY_4_struct.dwordindex_0;
				//v3 = (unsigned __int8)(x_D41A0_BYTEARRAY_4[13] + 1);
				x_D41A0_BYTEARRAY_4_struct.brightness_13++;// = v3;
				if (x_D41A0_BYTEARRAY_4_struct.brightness_13 > 4)
				{
					if (a3)
					{
						x_D41A0_BYTEARRAY_4_struct.brightness_13 = 4;
						sub_47650(a3/*, x_D41A0_BYTEARRAY_4_struct.brightness_13*/);
						return;
					}
				LABEL_85:
					x_D41A0_BYTEARRAY_4_struct.brightness_13 = 0;
					goto LABEL_86;
				}
			}
			goto LABEL_86;
		}
		if (a2)//v17==2
		{
			//v21 = x_D41A0_BYTEARRAY_4_struct.dwordindex_0;
			//v22 = D41A0_BYTESTR_0.word_0xc - 1;
			(x_D41A0_BYTEARRAY_4_struct.brightness_12)--;
			if (x_D41A0_BYTEARRAY_4_struct.brightness_12 >= 0)
				goto LABEL_86;
			if (a3)
			{
				x_D41A0_BYTEARRAY_4_struct.brightness_12 = 0;
				sub_47650(a3/*, a2*/);
			}
			else
			{
				x_D41A0_BYTEARRAY_4_struct.brightness_12 = 4;
				sub_47650(0/*, a2*/);
			}
			//result = 0;
		}
		else
		{
			//v3 = x_D41A0_BYTEARRAY_4_struct.dwordindex_0;
			//v20 = D41A0_BYTESTR_0.word_0xc + 1;
			(x_D41A0_BYTEARRAY_4_struct.brightness_12)++;// = v20;
			if (x_D41A0_BYTEARRAY_4_struct.brightness_12 <= 4)
				goto LABEL_86;
			if (a3)
			{
				x_D41A0_BYTEARRAY_4_struct.brightness_12 = 4;
				sub_47650(a3/*, D41A0_BYTESTR_0.word_0xc*/);
			}
			else
			{
				x_D41A0_BYTEARRAY_4_struct.brightness_12 = 0;
				sub_47650(0/*, D41A0_BYTESTR_0.word_0xc*/);
			}
			//result = 0;
		}
		return;// result;
	case 7:
		if (x_D41A0_BYTEARRAY_4_struct.setting_byte1_22 & Setting::MULTIPLAYER_MODE || IsRecordingOrPlaying())
			return;
		v13x = Entities_EA3E4[D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
		if (v13x <= Entities_EA3E4[0] || v13x->life_0x8 < 0)
			return;
		if (CommandLineParams.DoAlternativeGamespeedControl()) {
			if (x_D41A0_BYTEARRAY_4_struct.speedIndex == 2)
				x_D41A0_BYTEARRAY_4_struct.speedIndex = 0;
			else
			{
				x_D41A0_BYTEARRAY_4_struct.speedIndex = x_D41A0_BYTEARRAY_4_struct.speedIndex + (a2 != 0) + 1;
				if (x_D41A0_BYTEARRAY_4_struct.speedIndex > 2)
				{
					x_D41A0_BYTEARRAY_4_struct.speedIndex = 2;
				}
			}
		}
		else {
			if (x_D41A0_BYTEARRAY_4_struct.speedIndex)
				x_D41A0_BYTEARRAY_4_struct.speedIndex = 0;
			else
				x_D41A0_BYTEARRAY_4_struct.speedIndex = (a2 != 0) + 1;
		}
		v14 = x_D41A0_BYTEARRAY_4_struct.speedIndex;
		if (v14 < 1u)
		{
			if (v14)
				return;
			SetCurrentNotificationMessage_19760((char*)x_DWORD_E9C4C_langindexbuffer[381], 3u, 50);//speed normal
			//result = 0;
		}
		else
		{
			if (v14 <= 1u)
			{
				SetCurrentNotificationMessage_19760((char*)x_DWORD_E9C4C_langindexbuffer[382], 3u, 50);//speed fast
			}
			else
			{
				if (v14 != 2)
					return;
				SetCurrentNotificationMessage_19760((char*)x_DWORD_E9C4C_langindexbuffer[383], 3u, 50);//speed super fast
			}
			v13x->dword_0xA4_164x->roll_0x155_341 = 0;
			v13x->dword_0xA4_164x->pitch_0x157_343 = 0;
			//result = 0;
		}
		return;// result;
	case 8:
		//v26 = (int)x_D41A0_BYTEARRAY_0;
		if (D41A0_0.m_GameSettings.m_Display.m_uiScreenSize)
			return;
		if (a2)
		{
			//v27 = *(x_BYTE *)(x_D41A0_BYTEARRAY_0 + 8589) + 1;
			D41A0_0.m_GameSettings.m_Graphics.m_wViewPortSize++;
			if (D41A0_0.m_GameSettings.m_Graphics.m_wViewPortSize <= 40)
				goto LABEL_97;
			if (!a3)
			{
				D41A0_0.m_GameSettings.m_Graphics.m_wViewPortSize = 16;
				goto LABEL_97;
			}
		}
		else
		{
			//v28 = *(x_BYTE *)(x_D41A0_BYTEARRAY_0 + 8589) - 1;
			D41A0_0.m_GameSettings.m_Graphics.m_wViewPortSize--;
			if (D41A0_0.m_GameSettings.m_Graphics.m_wViewPortSize >= 16)
				goto LABEL_97;
			if (a3)
			{
				D41A0_0.m_GameSettings.m_Graphics.m_wViewPortSize = 16;
				goto LABEL_97;
			}
		}
		D41A0_0.m_GameSettings.m_Graphics.m_wViewPortSize = 40;
	LABEL_97:
		viewPort.ResizeViewPort_2CA90(D41A0_0.m_GameSettings.m_Graphics.m_wViewPortSize);
		return;
	case 9:
		if (!D41A0_0.str_0x21AA.creflections_0x21AA)
			return;
		//v4 = *(x_BYTE *)(x_D41A0_BYTEARRAY_0 + 8586) ^ 1;
		D41A0_0.m_GameSettings.m_Graphics.m_wReflections ^= 1;
		SetCurrentNotificationMessage_19760(x_DWORD_E9C4C_langindexbuffer[-D41A0_0.m_GameSettings.m_Graphics.m_wReflections + 360], 3u, 50);
		//	Reflections On / Reflections Off
		return;
	case 10:
		if (!D41A0_0.str_0x21AA.csky_0x21AC || !off_D41A8_sky)
			return;
		D41A0_0.m_GameSettings.m_Graphics.m_wSky ^= 1u;
		SetCurrentNotificationMessage_19760(x_DWORD_E9C4C_langindexbuffer[-D41A0_0.m_GameSettings.m_Graphics.m_wSky + 364], 3u, 50);
		// Sky On / Sky Off
		return;
	case 11:
		if (!D41A0_0.str_0x21AA.cshadows_0x21AB)
			return;
		//v6 = *(x_BYTE *)(x_D41A0_BYTEARRAY_0 + 8587) ^ 1;
		D41A0_0.m_GameSettings.m_Graphics.m_wShadows ^= 1;
		SetCurrentNotificationMessage_19760(x_DWORD_E9C4C_langindexbuffer[-D41A0_0.m_GameSettings.m_Graphics.m_wShadows + 358], 3u, 50);
		// Shadows On / Shadows Off
		return;
	case 12:
		if (!D41A0_0.str_0x21B6.clights_0x21B6)
			return;
		//v7 = *(x_BYTE *)(x_D41A0_BYTEARRAY_0 + 8598) == 1;
		D41A0_0.m_GameSettings.str_0x2196.m_wDynamicLighting ^= 1;
		SetCurrentNotificationMessage_19760(x_DWORD_E9C4C_langindexbuffer[-D41A0_0.m_GameSettings.str_0x2196.m_wDynamicLighting + 396], 3u, 50);
		//Light Sources On / Light Sources Off
		return;
	case 13:
		//v8 = (int)x_D41A0_BYTEARRAY_0;
		//v9 = *(x_BYTE *)(x_D41A0_BYTEARRAY_0 + 8591) ^ 1;
		D41A0_0.m_GameSettings.m_Display.m_wMiniMap ^= 1;
		D41A0_0.m_GameSettings.m_Display.m_wTopBar ^= 1;
		SetCurrentNotificationMessage_19760(x_DWORD_E9C4C_langindexbuffer[D41A0_0.m_GameSettings.m_Display.m_wMiniMap + 365], 3u, 50);
		// Icons and Map On / Icons and Map Off
		if (D41A0_0.m_GameSettings.m_Display.m_wMiniMap)
			return;
		sub_87C10();
		return;
	case 14:
		if (!D41A0_0.str_0x21B6.ctransparency_0x21B8)
			return;
		//v10 = *(x_BYTE *)(x_D41A0_BYTEARRAY_0 + 8600) ^ 1;
		D41A0_0.m_GameSettings.str_0x2196.transparency_0x2198 ^= 1;
		SetCurrentNotificationMessage_19760(x_DWORD_E9C4C_langindexbuffer[D41A0_0.m_GameSettings.str_0x2196.transparency_0x2198 + 440], 3u, 50);
		// Panel Transparency On / Panel Transparency Off
		return;
	case 15:
		if (!D41A0_0.str_0x21B6.cflat_0x21B9)
			return;
		//v11 = *(x_BYTE *)(x_D41A0_BYTEARRAY_0 + 8601) ^ 1;
		D41A0_0.m_GameSettings.str_0x2196.flat_0x2199 ^= 1;
		SetCurrentNotificationMessage_19760(x_DWORD_E9C4C_langindexbuffer[-D41A0_0.m_GameSettings.str_0x2196.flat_0x2199 + 443], 3u, 50);
		// Flat Shading On / Flat Shading Off
		return;
	case 16://change resolution
		if (!D41A0_0.str_0x21B2.cresolution_0x21B4 || D41A0_0.m_GameSettings.m_Display.m_uiScreenSize)
			return;
		ClearPauseMenuState_41BC0();
		ChangeGameResolution_46B40();
		return;
	case 17:
		//v25 = x_D41A0_BYTEARRAY_4_struct.byteindex_207 == 0;
		x_D41A0_BYTEARRAY_4_struct.byteindex_207 ^= 1;
		SetCurrentNotificationMessage_19760(x_DWORD_E9C4C_langindexbuffer[x_D41A0_BYTEARRAY_4_struct.byteindex_207 + 402], 3u, 50);
		// Player Names On / Player Names Off
		return;
	case 18:
		//v7 = *(x_BYTE *)(x_D41A0_BYTEARRAY_0 + 8592) == 1;
		D41A0_0.m_GameSettings.m_Display.m_uiScreenSize ^= 1;
		if (!D41A0_0.m_GameSettings.m_Display.m_uiScreenSize)
		{
			sub_417A0_install_pal_and_mouse_minmax();
			return;
		}
		sub_417D0_install_pal_and_mouse_minmax2();
		return;
	case 19:
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
		switch(D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].ShowDebugTextFlag_0x005_2BE0_11235)
		{
			case 0x00:
				D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].ShowDebugTextFlag_0x005_2BE0_11235 = 0x08;
				break;
			case 0x08:
				D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].ShowDebugTextFlag_0x005_2BE0_11235 = 0x48;
				break;
			case 0x48:
				D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].ShowDebugTextFlag_0x005_2BE0_11235 = 0x40;
				break;
			case 0x40:
				D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].ShowDebugTextFlag_0x005_2BE0_11235 = 0x00;
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
		if (D41A0_0.m_GameSettings.m_Display.m_uiScreenSize)
		{
			if (D41A0_0.m_GameSettings.str_0x2192.xxxx_0x2193 && iScreenWidth_DE560 == 640)
				sub_75C50();
			x_BYTE_D47D8 = D41A0_0.m_GameSettings.m_Display.m_uiScreenSize;
			sub_417A0_install_pal_and_mouse_minmax();
			D41A0_0.m_GameSettings.m_Display.m_uiScreenSize = 0;
			sub_41A90_VGA_Palette_install((TColor*)*xadatapald0dat2.colorPalette_var28);
		}
	}
	IsPauseMenuOpen_D47D9++;
}

//----- (0005BF10) --------------------------------------------------------
char sub_5BF10()//23cf10
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
void sub_70940()//251940
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

	v0x = Entities_EA3E4[D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
	v1 = (v0x->position_0x4C_76.x + 128) >> 8;
	v9 = (v0x->position_0x4C_76.y + 128) >> 8;
	result = AddE7EE0x_10080(0, 128);
	v3 = result;
	if (result)
	{
		v10 = 0;
		while (sub_10130(v3, &v8, &v7) == 1 && !v10)
		{
			for (i = mapEntityIndex_15B4E0[((unsigned __int8)(v9 + v7) << 8) + (unsigned __int8)(v1 + v8)];
				;
				i = v6x->oldMapEntity_0x16_22)
			{
				v6x = Entities_EA3E4[i];
				if (v6x == Entities_EA3E4[0] || v10)
					break;
				v5 = 1;
				if (v6x == v0x || v6x->class_0x3F_63 == 15 && v6x->id_0x1A_26 == v0x->id_0x1A_26 || v6x->struct_byte_0xc_12_15.byte[0] & 1)
					v5 = 0;
				if (v5)
				{
					v10 = 1;
					x_D41A0_BYTEARRAY_4_struct.dwordindex_38396 = v6x;
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
	x_DWORD_E3758 = 1;//2b4758
	//v5 = (int)pdwScreenBuffer_351628;
	uint8_t* temp_screen_buffer = pdwScreenBuffer_351628;
	pdwScreenBuffer_351628 = x_DWORD_180730_cursor_data;//351730
	//x_DWORD_180730_cursor_data = pdwScreenBuffer_351628;// hack
	if (a2.data)
	{
		x_WORD_18072C_cursor_sizex = a2.width_4;
		x_WORD_18072E_cursor_sizey = a2.height_5;
	}
	else
	{
		x_WORD_18072C_cursor_sizex = 0;
		x_WORD_18072E_cursor_sizey = 0;
	}
	if (x_WORD_180660_VGA_type_resolution & 1)
	{
		x_WORD_18072C_cursor_sizex >>= 1;
		x_WORD_18072E_cursor_sizey >>= 1;
	}
	x_DWORD_18065C = x_DWORD_180650_positiony;
	x_DWORD_180640_help_screen_height = x_DWORD_180630_screen_height;
	x_DWORD_180638 = x_DWORD_18063C_sprite_sizex;
	x_DWORD_18064C_help_screen_width = x_DWORD_180634_screen_width;
	x_DWORD_180658 = x_DWORD_180648_map_resolution2_x;
	x_DWORD_180654 = x_DWORD_180644_map_resolution2_y;
	x_DWORD_180650_positiony = 0;
	x_DWORD_180630_screen_height = 64;
	x_DWORD_18063C_sprite_sizex = 0;
	x_DWORD_180634_screen_width = 64;
	x_DWORD_180648_map_resolution2_x = 64;
	x_DWORD_180644_map_resolution2_y = 64;
	//v4 = x_WORD_E36D4;
	x_WORD_E36D4 = 0;
	v3 = screenWidth_18062C;
	screenWidth_18062C = 64;
	//x_DWORD_180730_cursor_data = (uint8_t*)malloc(0x1000);//fix it
	for (i = 0; i < 0x1000; i++)
	{
		x_DWORD_180730_cursor_data[i] = 0xfe;
		//printf("%d",i);
	}
	if (a2.data)
		drawBitmap320_8F8B0(0, 0, a2);//2708B0 super inicializace //a2 ma byt 86 nebo a1 nema nikam ukazovat
	pdwScreenBuffer_351628 = temp_screen_buffer;
	screenWidth_18062C = v3;
	//x_WORD_E36D4 = x_WORD_E36D4;//355230
	x_DWORD_180650_positiony = x_DWORD_18065C;
	x_DWORD_180630_screen_height = x_DWORD_180640_help_screen_height;
	x_DWORD_18063C_sprite_sizex = x_DWORD_180638;
	x_DWORD_180634_screen_width = x_DWORD_18064C_help_screen_width;
	x_DWORD_180648_map_resolution2_x = x_DWORD_180658;
	x_DWORD_180644_map_resolution2_y = x_DWORD_180654;
	//result = v5;
	//pdwScreenBuffer_351628 = (uint8_t*)v5;
	x_DWORD_E3758 = 0;

	//sub_90478_VGA_Blit320();//debug
	//return result;
}

//----- (000417D0) --------------------------------------------------------
void sub_417D0_install_pal_and_mouse_minmax2()//2227d0
{
	sub_906B4();
	sub_41A90_VGA_Palette_install((TColor*)*xadatapald0dat2.colorPalette_var28);
	sub_6EF10_set_mouse_minmax(0, 640, 0, 800);
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

	if (x_WORD_180660_VGA_type_resolution != 1)
		if (!DefaultResolutions())
		{
			scale = gameUiScale;
		}

	if (x_D41A0_BYTEARRAY_4_struct.OptionsSettingFlag_24 & GAME_PAUSED)
	{
		v1 = x_D41A0_BYTEARRAY_4_struct.byteindex_225;
		str_unk_1804B0ar.byte_0xaa = -1;
		if (!v1 && !x_D41A0_BYTEARRAY_4_struct.byte_38591)
		{
			if (D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 == 7)
			{
				SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]); //Set cursor to Null (Don't Draw)
			}
			else if (unk_18058Cstr.x_WORD_1805C2_joystick == 7 || unk_18058Cstr.x_WORD_1805C2_joystick == 1 || unk_18058Cstr.x_WORD_1805C2_joystick == 2)
			{
				SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[CURSOR_SPRITE_INDEX_D419E]);
			}
			if (!x_D41A0_BYTEARRAY_4_struct.byteindex_206)
			{
				GetPauseMenuCoordinates_2FFE0(&posX, &posY, &width, &height, scale);
				if (unk_18058Cstr.x_DWORD_1805B0_mouse.x >= posX && posX + width > unk_18058Cstr.x_DWORD_1805B0_mouse.x)
				{
					v2 = -1;
					v3 = 0;
					while (v3 < 4 && v2 < 0)
					{
						if (posY <= unk_18058Cstr.x_DWORD_1805B0_mouse.y && posY + height > unk_18058Cstr.x_DWORD_1805B0_mouse.y)
							v2 = v3;
						v3++;
						posY += height;
					}
					if (v2 >= 0)
					{
						if (v2 != 2 || posX + width / 2 > unk_18058Cstr.x_DWORD_1805B0_mouse.x)
						{
							if (v2 > 2)
								v2++;
						}
						else
						{
							v2 = 3;
						}
						str_unk_1804B0ar.byte_0xaa = v2;
						if (unk_18058Cstr.MouseButtonState_18059C & 1 || unk_18058Cstr.MouseButtonState_18059C & 2)
						{
							switch (v2)
							{
							case 0:
								v4 = D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221;
								if (v4 < 6u || v4 > 8u)
									HandleButtonClick_191B0(20, 13);
								else
									HandleButtonClick_191B0(20, 14);
								x_D41A0_BYTEARRAY_4_struct.SelectedMenuItem_38546 = 3;
								sub_19A50();
								return;
							case 1:
								if (!x_D41A0_BYTEARRAY_4_struct.byteindex_208)
									break;
								v5 = D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221;
								if (v5 < 6u || v5 > 8u)
									HandleButtonClick_191B0(20, 13);
								else
									HandleButtonClick_191B0(20, 14);
								x_D41A0_BYTEARRAY_4_struct.SelectedMenuItem_38546 = 2;
								sub_19A50();
								return;
							case 2:
								ChangeSoundLevel_19CA0(1u);
								sub_19A50();
								return;
							case 3:
								ChangeSoundLevel_19CA0(2u);
								sub_19A50();
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
			sub_19A50();
		}
	}
}

//----- (00018DA0) --------------------------------------------------------
char sub_18DA0(type_entity_0x6E8E* a1x, char a2, char a3)//1f9da0
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

	v3x = &a1x->dword_0xA4_164x->str_611;
	if (a2 == 2)
	{
		v4 = 0;
		if (a3)
			v5 = -1;
		else
			v5 = 1;
		v6 = a1x->dword_0xA4_164x->str_611.SpellIndexRight_0x453_1107 + v5;
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
				if (v3x->array_0x3B5_949x.SpellIndex[v7] == 2)
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
		a1x->dword_0xA4_164x->str_611.byte_0x45B_1115 = v6;
		//v9 = (int)x_D41A0_BYTEARRAY_0;
		D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0 = 32;
		D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].str_0x6E3E_byte1 = v7;
		//LOBYTE(v6) = v3x->array_0x437_1079[v7]; //*(x_BYTE *)(v8 + 468);
		D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].str_0x6E3E_byte2 = v3x->array_0x437_1079x.SpellIndex[v7]; //*(x_BYTE *)(v8 + 468);
	}
	else
	{
		v11 = 0;
		if (a3)
			v12 = -1;
		else
			v12 = 1;
		v6 = a1x->dword_0xA4_164x->str_611.SpellIndexLeft_0x451_1105 + v12;
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
				if (v3x->array_0x3B5_949x.SpellIndex[v13] == 1)
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
		a1x->dword_0xA4_164x->str_611.byte_0x45A_1114 = v6;
		//v15 = (int)x_D41A0_BYTEARRAY_0;
		D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0 = 31;
		D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].str_0x6E3E_byte1 = v13;
		//LOBYTE(v6) = *(x_BYTE *)(v14 + 468);//v3x->str_611.array_0x437_1079[v13]
		D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].str_0x6E3E_byte2 = v3x->array_0x437_1079x.SpellIndex[v13];
	}
	return v6;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (00018B30) --------------------------------------------------------
void sub_18B30()//1f9b30
{
	if (!(D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dw_w_b_0_2BDE_11230.byte[2] & 0x20))
	{
		if (D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 != 13 && D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 != 14)
		{
			if (D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 < 6u || D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 > 8u && (D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 < 0xBu || D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 > 0xCu))
				HandleButtonClick_191B0(20, 13);
			else
				HandleButtonClick_191B0(20, 14);
			x_D41A0_BYTEARRAY_4_struct.SelectedMenuItem_38546 = 1;
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
void sub_1A7A0_fly_asistant()//1fb7a0 // fly asistant
{
	if (CommandLineParams.DoFixMouse()) {
		if (CommandLineParams.ModeRegressionsTestType()!=-1)
		{
			unk_18058Cstr.MouseButtonState_18059C = 0;
			x_WORD_18074C_mouse_left2_button = 0;
			if (debugcounter_1fb7a0 < 1000) {
				unk_18058Cstr.x_DWORD_1805B8_mouse_position_x = 0x128;
				unk_18058Cstr.x_DWORD_1805BC_mouse_position_y = 0x7e;
			}
			debugcounter_1fb7a0++;
		}
	}

	if (CommandLineParams.DoFixFlyasistant()) {
		if (x_D41A0_BYTEARRAY_4_struct.OptionsSettingFlag_24 & GAME_PAUSED)
			return;
	}

	if (D41A0_0.byte_0x36DEA_fly_asistant)
	{
		if (D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 != 5 && D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 != 8 && D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 != 3 && !x_D41A0_BYTEARRAY_4_struct.byteindex_225)
		{
			if (D41A0_0.word_0x36DEC_mousex != unk_18058Cstr.x_DWORD_1805B8_mouse_position_x
				|| D41A0_0.word_0x36DEE_mousey != unk_18058Cstr.x_DWORD_1805BC_mouse_position_y
				|| D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].PlayerAction_byte0
				|| D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].entityIndex_0x6E3E_byte5)
			{
				;
			}
			else
			{
				D41A0_0.word_0x36DF0_mousexx++;
				if (D41A0_0.word_0x36DF0_mousexx <= 0x30)
				{
					D41A0_0.word_0x36DEC_mousex = unk_18058Cstr.x_DWORD_1805B8_mouse_position_x;
					D41A0_0.word_0x36DEE_mousey = unk_18058Cstr.x_DWORD_1805BC_mouse_position_y;
					return;
				}
				HandleButtonClick_191B0(39, 0);
			}
			D41A0_0.word_0x36DF0_mousexx = 0;
			D41A0_0.word_0x36DEC_mousex = unk_18058Cstr.x_DWORD_1805B8_mouse_position_x;
			D41A0_0.word_0x36DEE_mousey = unk_18058Cstr.x_DWORD_1805BC_mouse_position_y;
		}
	}
}

//----- (00019A70) --------------------------------------------------------
void HandleOptionsMenuButtonClick_19A70()//1faa70
{
	if (D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 < 6u || D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 > 8u)
		HandleButtonClick_191B0(20, 9);
	else
		HandleButtonClick_191B0(20, 11);
}

//----- (00018F80) --------------------------------------------------------
void HandleMouseButtons_18F80(type_entity_0x6E8E* a1x)//1f9f80
{
	if (unk_18058Cstr.x_WORD_1805C0_arrow_keys & 1)
		HandleButtonClick_191B0(6, 1);
	if (unk_18058Cstr.x_WORD_1805C0_arrow_keys & 2)
		HandleButtonClick_191B0(6, 2);
	if (unk_18058Cstr.x_WORD_1805C0_arrow_keys & 4)
		HandleButtonClick_191B0(6, 4);
	if (unk_18058Cstr.x_WORD_1805C0_arrow_keys & 8)
		HandleButtonClick_191B0(6, 8);
	if (a1x->dword_0xA4_164x->str_611.SpellIndexLeft_0x451_1105 == -1)
	{
		unk_18058Cstr.MouseButtonState_18059C &= 0xFE;
	}
	else
	{
		if (Entities_EA3E4[a1x->dword_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[a1x->dword_0xA4_164x->str_611.SpellIndexLeft_0x451_1105]]->byte_0x3B_59 == 1)
		{
			if (unk_18058Cstr.MouseButtonState_18059C & 1) //fire left
			{
				HandleButtonClick_191B0(6, 16);
				unk_18058Cstr.MouseButtonState_18059C &= 0xFE;
			}
		}
		else if (unk_18058Cstr.MouseButtonState_18059C & 1 || unk_18058Cstr.MouseButtonState_18059C & 4 && Entities_EA3E4[a1x->dword_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[a1x->dword_0xA4_164x->str_611.SpellIndexLeft_0x451_1105]]->word_0x2E_46 > 0)
		{
			HandleButtonClick_191B0(6, 16);
			unk_18058Cstr.MouseButtonState_18059C &= 0xFE;
		}
	}
	if (a1x->dword_0xA4_164x->str_611.SpellIndexRight_0x453_1107 == -1)
	{
		unk_18058Cstr.MouseButtonState_18059C &= 0xFD;
	}
	else
	{
		if (Entities_EA3E4[a1x->dword_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[a1x->dword_0xA4_164x->str_611.SpellIndexRight_0x453_1107]]->byte_0x3B_59 == 1)
		{
			if (unk_18058Cstr.MouseButtonState_18059C & 2) //fire right
			{
				HandleButtonClick_191B0(6, 32);
				unk_18058Cstr.MouseButtonState_18059C &= 0xFD;
			}
		}
		else if (unk_18058Cstr.MouseButtonState_18059C & 2 || unk_18058Cstr.MouseButtonState_18059C & 8 && Entities_EA3E4[a1x->dword_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[a1x->dword_0xA4_164x->str_611.SpellIndexRight_0x453_1107]]->word_0x2E_46 > 0)
		{
			HandleButtonClick_191B0(6, 32);
			unk_18058Cstr.MouseButtonState_18059C &= 0xFD;
		}
	}
}

//----- (0001A8A0) --------------------------------------------------------
void HandleArrowKeyPresses_1A8A0()//1fb8a0
{
	if (unk_18058Cstr.x_WORD_1805C0_arrow_keys & 4)
	{
		if (unk_18058Cstr.x_WORD_1805C0_arrow_keys & 8)
		{
			if (!(x_D41A0_BYTEARRAY_4_struct.byteindex_183 & 1) && !(x_D41A0_BYTEARRAY_4_struct.byteindex_183 & 2))
				HandleButtonClick_191B0(6, 128);
		}
	}
	x_D41A0_BYTEARRAY_4_struct.byteindex_183 &= 0xFC;
	if (unk_18058Cstr.x_WORD_1805C0_arrow_keys & 4)
		x_D41A0_BYTEARRAY_4_struct.byteindex_183 |= 1;
	if (unk_18058Cstr.x_WORD_1805C0_arrow_keys & 8)
	{
		x_D41A0_BYTEARRAY_4_struct.byteindex_183 |= 2;
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

	if (!x_D41A0_BYTEARRAY_4_struct.speedIndex)
	{
		if (x_WORD_180660_VGA_type_resolution == 1)
		{
			roll = ((x << 7) - 40960) / 320;
			pitch = ((y << 7) - 25600) / -200;
		}
		else
		{
			roll = ((x << 7) - ((gameResWidth / 2) << 7)) / (gameResWidth / 2);
			pitch = ((y << 7) - ((gameResHeight / 2) << 7)) / -(gameResHeight / 2);
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

		D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].roll = roll;
		D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].pitch = pitch;
		D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].nextEntity_0x6E3E_word6 = unk_18058Cstr.x_DWORD_180590;
		D41A0_0.playerInputs_0x6E3E[D41A0_0.LevelIndex_0xc].entityIndex2_0x6E3E_word8 = unk_18058Cstr.x_DWORD_180594;
	}
}

//----- (00018AA0) --------------------------------------------------------
void SwitchHelpMode_18AA0()//1f9aa0
{
	bool switchHelpMode = true;
	if (x_D41A0_BYTEARRAY_4_struct.showHelp_10)
	{
		x_D41A0_BYTEARRAY_4_struct.showHelp_10 = 0;
	}
	else
	{
		SetTextBoxMinMaxSizes_87090();
		if (str_unk_1804B0ar.byte_0xa2)
		{
			x_D41A0_BYTEARRAY_4_struct.showHelp_10 = 1;
		}
		else
		{
			switchHelpMode = false;
			SetCurrentNotificationMessage_19760("No Help Data", 3u, 50);
		}
	}
	if (switchHelpMode)
	{
		str_unk_1804B0ar.byte_0xaa = -1;
		x_D41A0_BYTEARRAY_4_struct.setting_38402 = 1;
		SetCurrentNotificationMessage_19760((char*)x_DWORD_E9C4C_langindexbuffer[(x_D41A0_BYTEARRAY_4_struct.showHelp_10 == 0) + 400], 3u, 50);
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

	if (x_WORD_180660_VGA_type_resolution != 1)
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

	if (x_WORD_180660_VGA_type_resolution & 1)
		spellMenuPosY = 400;
	else
		spellMenuPosY = 480;
	if (x_WORD_180660_VGA_type_resolution != 1)
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
char sub_6D4C0(type_str_611* a1x)//24e4c0
{
	a1x->subSpellIndex_0x459_1113 = a1x->array_0x437_1079x.SpellIndex[spellIndex_D94FF[a1x->spellIndex_0x458_1112]];
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

	if (x_WORD_180660_VGA_type_resolution != 1)
		if (!DefaultResolutions())
		{
			helpWidth = screenWidth_18062C;
			scale = gameUiScale;
			menuBtnPosY = (screenHeight_180624 / 2) - ((17 * (OPTIONS_MENU_BTN_HEIGHT * scale)) / 2);
			okayBtnPosY = menuBtnPosY + ((OPTIONS_MENU_BTN_HEIGHT * scale) * 17) + 4;
		}

	if (unk_18058Cstr.x_WORD_1805C2_joystick == 7 || unk_18058Cstr.x_WORD_1805C2_joystick == 1 || unk_18058Cstr.x_WORD_1805C2_joystick == 2)
		SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[CURSOR_SPRITE_INDEX_D419E]); //fix it
	if (LastPressedKey_1806E4 == 1)
	{
		sub_18B30();
		LastPressedKey_1806E4 = 0;
		sub_19A50();
		return;
	}
	if (!(unk_18058Cstr.MouseButtonState_18059C & 1) && !(unk_18058Cstr.MouseButtonState_18059C & 2) && LastPressedKey_1806E4 != 0x1c && LastPressedKey_1806E4 != 0x20)
	{
		sub_19A50();
		return;
	}

	//Ok button
	okayBtnPosX = (helpWidth - (x_D41A0_BYTEARRAY_4_struct.byteindex_186 * scale)) / 2 + ((x_D41A0_BYTEARRAY_4_struct.byteindex_186 * scale) - (OPTIONS_MENU_BTN_WIDTH * scale)) / 2;
	if (okayBtnPosX <= unk_18058Cstr.x_DWORD_1805B0_mouse.x && okayBtnPosX + (OPTIONS_MENU_BTN_WIDTH * scale) > unk_18058Cstr.x_DWORD_1805B0_mouse.x && unk_18058Cstr.x_DWORD_1805B0_mouse.y >= okayBtnPosY && unk_18058Cstr.x_DWORD_1805B0_mouse.y < (okayBtnPosY + (OPTIONS_MENU_BTN_HEIGHT * scale))
		|| LastPressedKey_1806E4 == 0x1c
		|| LastPressedKey_1806E4 == 0x20)
	{
		x_D41A0_BYTEARRAY_4_struct.byteindex_225 = 0;
		if (D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 == 9)
			HandleButtonClick_191B0(20, 0);
		else
			HandleButtonClick_191B0(20, 6);
		SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]); //Set cursor to Null (Don't Draw)
		LastPressedKey_1806E4 = 0;
		sub_19A50();
		return;
	}

	int btnIndex = 0;
	menuBtnPosX = (helpWidth - (x_D41A0_BYTEARRAY_4_struct.byteindex_186 * scale)) / 2;

	//Menu Options
	do
	{
		if (menuBtnPosX <= unk_18058Cstr.x_DWORD_1805B0_mouse.x
			&& menuBtnPosX + (x_D41A0_BYTEARRAY_4_struct.byteindex_186 * scale) > unk_18058Cstr.x_DWORD_1805B0_mouse.x
			&& menuBtnPosY <= unk_18058Cstr.x_DWORD_1805B0_mouse.y
			&& menuBtnPosY + (OPTIONS_MENU_BTN_HEIGHT * scale) > unk_18058Cstr.x_DWORD_1805B0_mouse.y)
		{
			ChangeSettings_1A970(btnIndex + 1, (unk_18058Cstr.MouseButtonState_18059C & 1) == 0, 1);
			sub_1A280();
			sub_19A50();
			return;
		}
		btnIndex++;
		menuBtnPosY += (OPTIONS_MENU_BTN_HEIGHT * scale);
	} while (btnIndex < OPTIONS_MENU_BTN_COUNT);

	sub_19A50();

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
	else if (unk_18058Cstr.MouseButtonState_18059C & 1 || unk_18058Cstr.MouseButtonState_18059C & 2)
	{
		if (okButtonX > unk_18058Cstr.x_DWORD_1805B0_mouse.x || okButtonX + (50 * scale) <= unk_18058Cstr.x_DWORD_1805B0_mouse.x || okButtonY > unk_18058Cstr.x_DWORD_1805B0_mouse.y || okButtonY + (32 * scale) <= unk_18058Cstr.x_DWORD_1805B0_mouse.y)
		{
			if (okButtonX + (50 * scale) <= unk_18058Cstr.x_DWORD_1805B0_mouse.x && okButtonX + (100 * scale) > unk_18058Cstr.x_DWORD_1805B0_mouse.x && okButtonY <= unk_18058Cstr.x_DWORD_1805B0_mouse.y && okButtonY + (32 * scale) > unk_18058Cstr.x_DWORD_1805B0_mouse.y)
				pressed_esc = true;
		}
		else
		{
			pressed_enter = true;
		}
		sub_19A50();
	}
	if (!pressed_enter)
		selectSpell = true;
	else
	{
		switch (x_D41A0_BYTEARRAY_4_struct.SelectedMenuItem_38546)
		{
		case 1:
			sub_18B30();
			break;
		case 2:
			if (x_D41A0_BYTEARRAY_4_struct.byteindex_208)
			{
				if (LoadLevel_555D0(0, x_D41A0_BYTEARRAY_4_struct.levelnumber_43w))
					sprintf(printbuffer, "%s:%s.", x_DWORD_E9C4C_langindexbuffer[423], "OK");//Load Level
				else
					sprintf(printbuffer, "%s:%s.", x_DWORD_E9C4C_langindexbuffer[423], x_DWORD_E9C4C_langindexbuffer[429]);//429 - Failed
				ShowMessage_52D70(0, printbuffer);
			}
			selectSpell = true;
			break;
		case 3:
			if (SaveLevel_55080(0, x_D41A0_BYTEARRAY_4_struct.levelnumber_43w, (char*)""))
				sprintf(printbuffer, "%s:%s.", x_DWORD_E9C4C_langindexbuffer[424], "OK");//Save Level
			else
				sprintf(printbuffer, "%s:%s.", x_DWORD_E9C4C_langindexbuffer[424], x_DWORD_E9C4C_langindexbuffer[429]);//429 - Failed
			ShowMessage_52D70(0, printbuffer);
			selectSpell = true;
			x_D41A0_BYTEARRAY_4_struct.byteindex_208 = DataFileIO::sub_55C00_TestSaveFile2(x_D41A0_BYTEARRAY_4_struct.levelnumber_43w);
			break;
		}
	}
	if (pressed_enter || pressed_esc)
	{
		x_D41A0_BYTEARRAY_4_struct.SelectedMenuItem_38546 = 0;
		if (selectSpell)
			HandleButtonClick_191B0(20, x_D41A0_BYTEARRAY_4_struct.byte_38544);
	}
}

//----- (00019760) --------------------------------------------------------
void SetCurrentNotificationMessage_19760(const char* message, unsigned __int8 a2, __int16 duration)//1fa760
{
	int maxLength = sizeof(D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].CurrentNotificationText_0x01c_2BFA_11258);

	//char *v3; // esi
	//char *v4; // edi
	//char v5; // al
	//char v6; // al
	//int result; // eax

	//v3 = (char*)a1;
	//D41A0_BYTESTR_0.array_0x2BDE[D41A0_BYTESTR_0.word_0xc].CurrentNotificationText_0x01c_2BFA_11258
	//v4 = (char*)&x_D41A0_BYTEARRAY_0[2124 * D41A0_BYTESTR_0.word_0xc + 11230 + 28];

	if (strlen(message) < maxLength)
		strcpy(D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].CurrentNotificationText_0x01c_2BFA_11258, message);
	else
	{
		strncpy(D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].CurrentNotificationText_0x01c_2BFA_11258, message, maxLength - 1);
		D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].CurrentNotificationText_0x01c_2BFA_11258[48] = '\0';
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
	D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].CurrentNotificationDuration_0x04d_2C2B_11307 = duration;
	D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].word_0x04f_2C2D_11309 = a2;
	//	return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (00047650) --------------------------------------------------------
void sub_47650(int  /*a1*//*, int a2*/)//228650
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
	v2 = D41A0_0.terrain_2FECE.MapType;
	if (v2 == MapType_t::Day)
		v8 = x_D41A0_BYTEARRAY_4_struct.brightness_11;
	else if (v2 == MapType_t::Night)
	{
		v8 = x_D41A0_BYTEARRAY_4_struct.brightness_12;
	}
	else if (v2 == MapType_t::Cave)
	{
		v8 = x_D41A0_BYTEARRAY_4_struct.brightness_13;
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
	x_D41A0_BYTEARRAY_4_struct.paletteSubMod_180 = 1;
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

	int16_t v0_tempmousex = 320;
	int16_t v0_tempmousey = 200;

	sub_90B27_VGA_pal_fadein_fadeout(0, 0x10u, 0);
	sub_417A0_install_pal_and_mouse_minmax();
	memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 768);
	sub_41A90_VGA_Palette_install((TColor*)*xadatapald0dat2.colorPalette_var28);

	if (pre_ptrMemoryBuffer_E9C3C)
	{
		FreeMem_83E80(pre_ptrMemoryBuffer_E9C3C);
		pre_ptrMemoryBuffer_E9C3C = 0;
		ptrMemoryBuffer_E9C3C = 0;
	}

	sub_54600_mouse_reset();

	sub_6EBF0(&filearray_2aa18c[filearrayindex_POINTERSDATTAB]);
	sub_6EBF0(&filearray_2aa18c[filearrayindex_MSPRD00DATTAB]);
	sub_6EBF0(&filearray_2aa18c[filearrayindex_BUILD00DATTAB]);

	sub_47130();

	//Change resolution
	if (x_WORD_180660_VGA_type_resolution == 1)
	{
		sub_6EBF0(&filearray_2aa18c[filearrayindex_FONTS0DATTAB]);
		DataFileIO::sub_90D3F_unload_file_array(psxadatamsprd00dat);
		x_WORD_180660_VGA_type_resolution = 8;
	}
	else
	{
		sub_6EBF0(&filearray_2aa18c[filearrayindex_HFONT3DATTAB]);
		DataFileIO::sub_90D3F_unload_file_array(psxadatahsprd00dat);
		x_WORD_180660_VGA_type_resolution = 1;
	}
	LoadSpr_47160();

	CreateIndexes_6EB90(&filearray_2aa18c[filearrayindex_POINTERSDATTAB]);
	CreateIndexes_6EB90(&filearray_2aa18c[filearrayindex_BUILD00DATTAB]);

	if (extendedFonts)
		LoadFixedFonts(3, (char*)"FONT1");

	memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 768);
	if (x_WORD_180660_VGA_type_resolution & 1)
	{
		v1 = getPaletteIndex_5BE80((TColor*)*xadatapald0dat2.colorPalette_var28, 0, 0, 0);
		ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, v1);
	}
	else
	{
		v2 = getPaletteIndex_5BE80((TColor*)*xadatapald0dat2.colorPalette_var28, 0, 0, 0);
		ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, screenWidth_18062C, screenHeight_180624, v2);
	}
	memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 768);
	sub_41A90_VGA_Palette_install((TColor*)*xadatapald0dat2.colorPalette_var28);
	if (x_WORD_180660_VGA_type_resolution & 1)
		sub_90D6E_VGA_set_video_mode_320x200_and_Palette((TColor*)*xadatapald0dat2.colorPalette_var28);
	else
	{
		if (((gameResWidth > 640) || (gameResHeight > 480)) && (x_WORD_180660_VGA_type_resolution != 1))
		{
			screenWidth_18062C = gameResWidth;
			screenHeight_180624 = gameResHeight;
			sub_90E07_VGA_set_video_mode_alt_and_Palette((TColor*)*xadatapald0dat2.colorPalette_var28);
			v0_tempmousex = gameResWidth / 2;
			v0_tempmousey = gameResHeight / 2;
		}
		else
			sub_90E07_VGA_set_video_mode_640x480_and_Palette((TColor*)*xadatapald0dat2.colorPalette_var28);
	}

	sub_41A90_VGA_Palette_install((TColor*)*xadatapald0dat2.colorPalette_var28);
	v3 = getPaletteIndex_5BE80((TColor*)*xadatapald0dat2.colorPalette_var28, 0, 0, 0);
	uiBackGroundColorIdx_EB3A8 = v3;
	if (x_WORD_180660_VGA_type_resolution & 1)
		ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, v3);
	else
		ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, screenWidth_18062C, screenHeight_180624, v3);

	sub_8CEDF_install_mouse();
	SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]); //Set cursor to Null (Don't Draw)
	x_D41A0_BYTEARRAY_4_struct.paletteMod_51 = 2;
	if (x_WORD_180660_VGA_type_resolution == 1)
		FontType_D419D = 1;
	else
		FontType_D419D = 3;

	SetMousePositionInMemory_5BDC0(v0_tempmousex, v0_tempmousey);

	if (EventDispatcher::I != nullptr)
		EventDispatcher::I->DispatchEvent(EventType::E_RESOLUTION_CHANGE, screenWidth_18062C, screenHeight_180624);
}

//----- (00075C50) --------------------------------------------------------
void sub_75C50()//fix
{
	if (CommandLineParams.DoShowDebugPerifery())ShowPerifery();

	/* __outx_BYTE(0x302u, 2u);
	 __outx_BYTE(0x303u, 1u);*/
	memset((void*)pdwScreenBuffer_351628, 0, screenHeight_180624 * screenWidth_18062C);
	if (x_WORD_180660_VGA_type_resolution & 1)
		sub_90478_VGA_Blit320();
	else
		sub_75200_VGA_Blit640(480);
}

//----- (000906B4) --------------------------------------------------------
int sub_906B4()//fix bios graphics//2716b4
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
	//sub_A0BB0((int *)&unk_E3894, 25);
	/*__outx_WORD(0x3C4u, 0x604u);
	__outx_WORD(0x3D4u, 0x14u);
	__outx_WORD(0x3D4u, 0xE317u);
	__outx_WORD(0x3CEu, 0xFF08u);
	__outx_WORD(0x3C4u, 0xF02u);*/
	v0 = (char*)&loc_A0000_vga_buffer;
	do
	{
		*(x_DWORD*)v0 = 0;
		v0 += 4;
	} while (v0 != (char*)&loc_A0000_vga_buffer+0x10000);
	v1 = 0;
	do
	{
		v2 = v1++ & 3;
		x_BYTE_1810A3[v1] = 1 << v2; // FIXME: only written to, can this whole function be removed?
	} while (v1 < 320);
	sub_90668(0);
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
	if (x_WORD_180660_VGA_type_resolution != 1)
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
signed int sub_10130(int a1, x_DWORD* a2, x_DWORD* a3)//1f1130
{
	*a2 = str_E7EE0x[a1].bitmapsData_20xx[0];
	*a3 = str_E7EE0x[a1].bitmapsData_20xx[1];
	str_E7EE0x[a1].bitmapsData_20xx += sizeof(int32_t);
	str_E7EE0x[a1].dword_12++;
	if (str_E7EE0x[a1].bitmapsStr_16->bitmapIndex_4 > str_E7EE0x[a1].dword_12)
	{
		return 1;
	}
	else
	{
		str_E7EE0x[a1].dword_12 = 0;
		str_E7EE0x[a1].bitmapsStr_16++;
		str_E7EE0x[a1].dword_8++;
		if (str_E7EE0x[a1].dword_8 <= str_E7EE0x[a1].dword_4)
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
void sub_19A50()//1faa50
{
	if (unk_18058Cstr.MouseButtonState_18059C & 1 || unk_18058Cstr.MouseButtonState_18059C & 2)
		unk_18058Cstr.MouseButtonState_18059C &= 0xFC;
}

//----- (00090668) --------------------------------------------------------
signed int sub_90668(int a1)//271668
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
			x_WORD_1811E4[v3] = v4;
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
			*(__int16*)((char*)x_WORD_1811E4 + v1) = v5;
			v1 += 2;
			v5 += 80;
		} while (result < 400);
	}
	return result;
}
// 1811E4: using guessed type __int16 x_WORD_1811E4[];

void sub_90E07_VGA_set_video_mode_alt_and_Palette(TColor* Palette)//271e07
{
	//unsigned __int8 v2; // [esp+0h] [ebp-38h]
	//int v3; // [esp+1Ch] [ebp-1Ch]

	//v3 = 0xf00;//
//removed int386(0x10, (REGS*)&v3, (REGS*)&v2);//Set video mode
	VGA_Resize(screenWidth_18062C, screenHeight_180624);
	if (!x_WORD_180662_graphics_handle)
		x_WORD_180662_graphics_handle = 0x13;
	screenWidth_18062C = screenWidth_18062C;
	screenHeight_180624 = screenHeight_180624;
//removed sub_994BA_cursor_move(0x101); //set position of cursor?
	sub_41A90_VGA_Palette_install(Palette);
//removed sub_8D12F_set_mouse_viewport(); //set min/max of viewport
	sub_A0D50_set_viewport(0, 0, screenWidth_18062C, screenHeight_180624);
}

//----- (00047130) --------------------------------------------------------
void sub_47130()//228130
{
	if (x_DWORD_D4188)
	{
		FreeMem_83E80((uint8_t*)x_DWORD_D4188);
		x_DWORD_D4188 = 0;
		x_DWORD_D418C = 0;
		x_DWORD_D4190 = 0;
	}
}
// D4188: using guessed type int x_DWORD_D4188;
// D418C: using guessed type int x_DWORD_D418C;
// D4190: using guessed type int x_DWORD_D4190;
