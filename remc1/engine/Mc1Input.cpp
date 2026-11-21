#include "Mc1Input.h"

#include <cstring>

#include "Mc1Entity.h"
#include "Mc1Files.h"
#include "Mc1Game.h"
#include "Mc1Level.h"
#include "Mc1Movie.h"
#include "Mc1NotPorted.h"
#include "Mc1Save.h"
#include "Mc1Screen.h"
#include "Mc1Shared.h"
#include "Mc1Sound.h"
#include "Mc1TextureMaps.h"

// 9AD0C: the character of a scan code (UK keyboard); 70h-73h are a pointer (its value in DOSBox), 7Ch-7Fh the
// variable 9AD88 (0 or 1), no letter or digit there
const uint8_t scanCodeChars_9AD0C_9AD0C[128] = {
	0x00, 0x27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0x08, 0x09,
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', 0x0D, 0x00, 'A', 'S',
	'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', 0x27, '#', 0x00, '\\', 'Z', 'X', 'C', 'V',
	'B', 'N', 'M', ',', '.', '/', 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2D, 0x00, 0x00, 0x00, 0x2B, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x2F, 0x00, 0x00, 0x28, 0x29, 0x2F, 0x2A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x2E, 0x0D, 0x00, 0x40, 0x01, 0xC8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

// 3FCA0: the joystick in the menus, the VFX1 head and the joystick in the game
uint8_t joyMenuMouse_90B20_90B20 = 0; // the mouse set to 0, 0 at the first menu frame
uint8_t joyLeftDown_90B21_90B21 = 0;
uint8_t joyRightDown_90B22_90B22 = 0;
int16_t joyMouseX_90B1C_90B1C = 0; // the mouse of the joystick from the centre
int16_t joyMouseY_90B1E_90B1E = 0;
int16_t vfxSamplesX_90B04_90B04[2];
int16_t vfxSamplesY_90B0C_90B0C[2];
int32_t vfxIndexX_90B18_90B18 = 0;
int32_t vfxIndexY_90B14_90B14 = 0;
uint8_t vfxTracking_9AD08_9AD08 = 0; // the VFX1 head tracker moves the mouse
int16_t vfxTrackerY_12EE9E_12EE8E = 0; // read by 5B210
int16_t vfxTrackerX_12EEA0_12EE90 = 0;
// B7700-B772A (joystickFound_B7700_B76F0 in Mc1Game): set by the calibration 3F820 / 3FB60
uint8_t secondStick_B7701_B76F1 = 0;
int16_t joyX_B7702_B76F2 = 0; // -1..1 digital, -80h..80h analog
int16_t joyY_B7704_B76F4 = 0;
int16_t joyMaxX_B7706_B76F6 = 0; // the dead zone of the counts
int16_t joyMaxY_B7708_B76F8 = 0;
int16_t joyMinX_B770A_B76FA = 0;
int16_t joyMinY_B770C_B76FC = 0;
uint16_t joyScaleMaxX_B770E_B76FE = 0; // 800h / the range of the counts
uint16_t joyScaleMaxY_B7710_B7700 = 0;
int16_t joyScaleMinX_B7712_B7702 = 0;
int16_t joyScaleMinY_B7714_B7704 = 0;
int16_t joy2X_B7716_B7706 = 0; // the second stick
int16_t joy2Y_B7718_B7708 = 0;
int16_t joy2MaxX_B771A_B770A = 0;
int16_t joy2MaxY_B771C_B770C = 0;
int16_t joy2MinX_B771E_B770E = 0;
int16_t joy2MinY_B7720_B7710 = 0;
uint16_t joy2ScaleMaxX_B7722_B7712 = 0;
uint16_t joy2ScaleMaxY_B7724_B7714 = 0;
int16_t joy2ScaleMinX_B7726_B7716 = 0;
int16_t joy2ScaleMinY_B7728_B7718 = 0;
uint8_t joyButtons_B772A_B771A = 0; // the bits 10h-80h of 201h, 0 pressed

static Type_Mc1Entity* Entities()
{
	return mc1Entities;
}

static Type_Mc1PlayerInput& CurrentInput()
{
	return mc1State_AE400_AE3F0->playerInputs_0x7413_29715[mc1State_AE400_AE3F0->currentPlayer_0x08_8];
}

static int16_t ReadS16(const uint8_t* data)
{
	int16_t value;
	memcpy(&value, data, sizeof(value));
	return value;
}

static int32_t ReadS32(const uint8_t* data)
{
	int32_t value;
	memcpy(&value, data, sizeof(value));
	return value;
}

static bool KeyDown(uint8_t scanCode)
{
	return (*pressedKeys_12EEF0_12EEE0)[scanCode] != 0;
}

// the command is set only into an empty slot or over the same command
static bool CommandFree(uint16_t command)
{
	const Type_Mc1PlayerInput& input = CurrentInput();
	return (int16_t)(int8_t)input.command_0x00_0 == (int16_t)command || !input.command_0x00_0;
}

void SetPlayerCommand_188A0_188A0(uint16_t command, uint8_t param)
{
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	Type_Mc1PlayerInput& input = CurrentInput();
	Type_Mc1Player_340B& player = state->players_0x340B_13323[state->currentPlayer_0x08_8];
	switch (command)
	{
	case 0:
	case 1:
	case 2:
	case 26:
		input.command_0x00_0 = (uint8_t)command;
		break;
	case 4:
		if (CommandFree(command))
		{
			input.command_0x00_0 = (uint8_t)command;
			input.param_0x01_1 |= param;
		}
		break;
	case 6:
		if (CommandFree(command))
		{
			input.command_0x00_0 = (uint8_t)command;
			input.buttons_0x05_5 |= param;
		}
		break;
	case 10:
	case 11:
	case 12:
	case 14:
	case 16:
	case 18:
	case 19:
	case 29:
		if (CommandFree(command))
			input.command_0x00_0 = (uint8_t)command;
		break;
	case 15:
	{
		// the rebirth: only a dead wizard in the action 3
		if (!CommandFree(command))
			break;
		const Type_Mc1Entity& wizard = Entities()[player.entityIndex_0x0A_10];
		if (wizard.life_0x0C_12 < 0 && wizard.actionIndex_0x46_70 == 3)
			input.command_0x00_0 = (uint8_t)command;
		break;
	}
	case 27:
		if (CommandFree(command) && (player.levelResult_0x02_2 & 2))
			input.command_0x00_0 = (uint8_t)command;
		break;
	case 28:
		if (CommandFree(command) && (player.levelResult_0x02_2 & 4))
			input.command_0x00_0 = (uint8_t)command;
		break;
	case 17:
		if (CommandFree(command))
		{
			input.param_0x01_1 = param;
			input.command_0x00_0 = (uint8_t)command;
		}
		break;
	case 30:
		// the cheats: with the flag 8000h, or for the player named "chronicle"
		if (!(mc1Settings_AE408_AE3F8->flags_0x00_0 & 0x8000) && memcmp(player.name_0x40A_1034, "chronicle", 9))
			break;
		// fall through
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
		if (CommandFree(command))
		{
			input.command_0x00_0 = (uint8_t)command;
			input.param_0x01_1 = param;
		}
		break;
	}
}

void MouseToMove_169E0_169E0()
{
	if (mc1Settings_AE408_AE3F8->speedIndex_0x96_150)
		return;
	int32_t moveX, moveY;
	if (*videoMode_12F02E_12F01E == 1)
	{
		moveX = (mousePos_9AD90_9AD90->x * 0x80 - 0xA000) / 0x140;
		moveY = -(mousePos_9AD90_9AD90->y * 0x80 - 0x6400) / 0xC8;
	}
	else
	{
		moveX = (mousePos_9AD90_9AD90->x * 0x80 - 0xA000) / 0x140;
		moveY = -((mousePos_9AD90_9AD90->y * 0x80 - 0x7800) / 0xF0);
	}
	if (moveX < -0x7F)
		moveX = -0x7F;
	if (moveX > 0x7F)
		moveX = 0x7F;
	if (moveY < -0x7F)
		moveY = -0x7F;
	if (moveY > 0x7F)
		moveY = 0x7F;
	CurrentInput().moveX_0x03_3 = (int8_t)moveX;
	CurrentInput().moveY_0x04_4 = (int8_t)moveY;
}

// the message of the current player to itself (shown by 20FB0 for the time)
static void ShowMessage(const char* text, int16_t time, int16_t kind)
{
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	Type_Mc1Message& message = state->players_0x340B_13323[state->currentPlayer_0x08_8].messages_0x1C_28[state->currentPlayer_0x08_8];
	message.time_0x40_64 = time;
	message.kind_0x42_66 = kind;
	strcpy(message.text_0x00_0, text);
}

static void ShowText(int index)
{
	ShowMessage((const char*)textPointers_AE238_AE228[index], 0x32, 2);
}

void FunctionKeys_16B00_16B00()
{
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	Type_Mc1Settings_AE408_AE3F8* settings = mc1Settings_AE408_AE3F8;
	Type_Mc1Player_340B& player = state->players_0x340B_13323[state->currentPlayer_0x08_8];
	uint8_t& key = *lastKey_12EF70_12EF60;
	if (CurrentInput().command_0x00_0)
		return;
	if (KeyDown(0x38))
	{
		// Alt
		switch (key)
		{
		case 0x3B: // F1-F7
		case 0x3C:
		case 0x3D:
		case 0x3E:
		case 0x3F:
		case 0x40:
		case 0x41:
			SetPlayerCommand_188A0_188A0(0x1E, key - 0x3A);
			key = 0;
			break;
		case 0x2F: // V
			if (settings->flags_0x00_0 & 0x8000)
				SetPlayerCommand_188A0_188A0(4, 8);
			key = 0;
			break;
		case 0x1F: // S: save the game
			if (!(settings->flags_0x00_0 & 0x10))
			{
				MarkLoadedTmaps_593B0_598C0();
				state->creatureStats_0x38C9F_232607.savedLevel_0x64 = settings->levelIndex_0x11_17;
				state->creatureStats_0x38C9F_232607.savedTimer_0x66 = timerTicks_AC5D4_AC5C4;
				SaveGame_3E750_3EA90(0xC7);
				SaveMaps_3E8C0_3EC00(0xC7);
				RebuildFreeLists_37220_375E0();
				state->reuseTop_0x11F1_4593 = -1;
				ShowMessage("Game saved.", 100, 0);
			}
			key = 0;
			break;
		case 0x26: // L: load the game
			if (!(settings->flags_0x00_0 & 0x10))
			{
				LoadGame_3E690_3E9D0(0xC7);
				LoadMaps_3E7F0_3EB30(0xC7);
				settings->levelIndex_0x11_17 = state->creatureStats_0x38C9F_232607.savedLevel_0x64;
				timerTicks_AC5D4_AC5C4 = state->creatureStats_0x38C9F_232607.savedTimer_0x66;
				ReloadTmaps_59420_59930();
				InitInputChange_356E0_35AA0();
				ShowMessage("Game loaded.", 100, 0);
			}
			key = 0;
			break;
		}
		return;
	}
	if (KeyDown(0x2A) || KeyDown(0x36))
	{
		// Shift
		switch (key)
		{
		case 0x10: // Q: quit
			SetPlayerCommand_188A0_188A0(2, 0);
			key = 0;
			break;
		case 0x12: // E
			if (settings->flags_0x00_0 & 0x8000)
				SetPlayerCommand_188A0_188A0(0x1A, 0);
			key = 0;
			break;
		case 0x13: // R: restart the level, no castle
			if (!(settings->flags_0x00_0 & 0x10))
			{
				player.levelResult_0x02_2 |= 0x0C;
				memset(player.playerData_0x44F_1103 + 0x32, 0, sizeof(int16_t));
			}
			key = 0;
			break;
		case 0x21: // F: the level failed
			if (settings->flags_0x00_0 & 0x8000)
				player.levelResult_0x02_2 |= 4;
			key = 0;
			break;
		case 0x2E: // C: the level completed
			if (settings->flags_0x00_0 & 0x8000)
				player.levelResult_0x02_2 |= 2;
			key = 0;
			break;
		}
		return;
	}
	if (!(settings->flags_0x00_0 & 0x10) && key == 0x19)
	{
		// P: the pause, the sound and the music stop
		settings->flags_0x00_0 ^= 0x10000;
		if (settings->flags_0x00_0 & 0x10000)
		{
			if (byte_939E5_939E5)
				StopAllSamples_5D010_5D520();
			if (byte_939CD_939CD)
				StopMusic_20E60_20E60();
		}
		else if (byte_939CD_939CD)
			PlayMusic_5D290_5D7A0((int16_t)state->levelVariant_0x240_576);
		key = 0;
	}
	if (key == 0x13)
	{
		// R
		if (state->options_0x21AD_8621[10] && !state->screenSize_0x219B_8603)
			ToggleResolution_34C80_35040();
		key = 0;
	}
	if (key == 0x39)
	{
		// Space: the rebirth
		SetPlayerCommand_188A0_188A0(0x0F, 0);
		SetPlayerCommand_188A0_188A0(0x1B, 0);
		key = 0;
	}
	switch (key)
	{
	case 0x3B: // F1: the sound
		if (byte_939E4_939E4)
		{
			byte_939E5_939E5 ^= 1;
			ShowText(byte_939E5_939E5 ? 75 : 76);
			StopAllSamples_5D010_5D520();
		}
		break;
	case 0x3C: // F2: the music
		if (!byte_939CC_939CC)
			break;
		if (!byte_939CD_939CD)
		{
			byte_939CD_939CD = 1;
			PlayMusic_5D290_5D7A0((int16_t)state->levelVariant_0x240_576);
			ShowText(77);
		}
		else
		{
			StopMusic_20E60_20E60();
			ShowText(78);
			byte_939CD_939CD = 0;
		}
		break;
	case 0x3D: // F3: the speed (1, 4 or 16 ticks in a frame)
		if (settings->flags_0x00_0 & 0x10)
			break;
		settings->speedIndex_0x96_150 = (uint8_t)((settings->speedIndex_0x96_150 + 1) % 3);
		switch (settings->speedIndex_0x96_150)
		{
		case 0:
			ShowText(63);
			break;
		case 1:
			ShowText(64);
			break;
		case 2:
			ShowText(65);
			break;
		}
		break;
	case 0x3E: // F4: soften
		if (state->options_0x21AD_8621[8])
		{
			state->soften_0x219D_8605 ^= 1;
			ShowText(state->soften_0x219D_8605 ? 41 : 42);
		}
		break;
	case 0x3F: // F5: the reflections
		if (state->options_0x21AD_8621[0])
		{
			state->reflections_0x2195_8597 ^= 1;
			ShowText(state->reflections_0x2195_8597 ? 39 : 40);
		}
		break;
	case 0x40: // F6: the sky
		if (state->options_0x21AD_8621[2] && skyData_AE3D8_AE3C8)
		{
			state->sky_0x2197_8599 ^= 1;
			ShowText(state->sky_0x2197_8599 ? 43 : 44);
		}
		break;
	case 0x41: // F7: the shadows
		if (state->options_0x21AD_8621[1])
		{
			state->shadows_0x2196_8598 ^= 1;
			ShowText(state->shadows_0x2196_8598 ? 37 : 38);
		}
		break;
	case 0x42: // F8: the icons and the map
		state->map_0x2199_8601 ^= 1;
		state->icons_0x219A_8602 ^= 1;
		ShowText(state->map_0x2199_8601 ? 45 : 46);
		break;
	case 0x43: // F9: the speed blur, light, heavy, off
		if (!state->options_0x21AD_8621[7])
			break;
		switch (state->blur_0x219C_8604)
		{
		case 0:
			if (*blurBuffer_AE404_AE3F4)
			{
				state->blur_0x219C_8604++;
				ShowText(49);
			}
			break;
		case 1:
			state->blur_0x219C_8604++;
			ShowText(50);
			break;
		case 2:
			ShowText(51);
			state->blur_0x219C_8604 = 0;
			break;
		}
		break;
	case 0x44: // F10: the screen sizes: red-blue stereo, VFX1, normal
		if (player.mode_0x44A_1098 || !state->options_0x21AD_8621[6])
			break;
		switch (state->screenSize_0x219B_8603)
		{
		case 0:
			if (*blurBuffer_AE404_AE3F4)
			{
				SetStereoScreen_31600_31640();
				state->screenSize_0x219B_8603 = 1;
				state->setting_0x2198_8600 = 0x28;
			}
			break;
		case 1:
			SetScreenModeAndPalette_315C0_31600();
			state->screenSize_0x219B_8603 = 2;
			break;
		case 2:
			state->screenSize_0x219B_8603 = 0;
			break;
		}
		break;
	default:
		return;
	}
	key = 0;
}

// the game port (201h): the port has none (remc2 leaves out the joystick I/O as well), it reads as with nothing
// connected; a gamepad goes through the SDL layer of remc2 (gamepad_init) as the mouse and the keys
static uint8_t ReadGamePort()
{
	return 0xFF;
}

struct GamePortCounts
{
	uint8_t bits;
	int16_t x, y, x2, y2;
};

// 3F76D: the time of the one-shots of the axes (the second stick with B7701), joystickFound cleared on the timeout
static GamePortCounts ReadGamePortCounts_3F76D_3FAAD()
{
	GamePortCounts counts = {};
	const uint8_t mask = secondStick_B7701_B76F1 ? 0x0F : 3;
	int32_t count = 0x2710;
	while (true)
	{
		counts.bits = ReadGamePort();
		if (!(counts.bits & mask))
			return counts;
		if (counts.bits & 1)
			counts.x++;
		if (counts.bits & 2)
			counts.y++;
		if (counts.bits & 4)
			counts.x2++;
		if (counts.bits & 8)
			counts.y2++;
		if (--count < 0)
			break;
	}
	joystickFound_B7700_B76F0 = 0;
	return counts;
}

// the axis of the analog joystick: -80h..80h out of the calibrated dead zone
static int16_t AnalogAxis(int16_t count, int16_t min, int16_t max, int16_t scaleMin, uint16_t scaleMax)
{
	int32_t value = 0;
	if (count < min)
	{
		value = (int16_t)(count - min) * scaleMin >> 11;
		if (value < -0x80)
			value = -0x80;
	}
	else if (count > max)
	{
		value = (int32_t)((uint32_t)(uint16_t)(count - max) * scaleMax) >> 11;
		if ((uint32_t)value > 0x80)
			value = 0x80;
	}
	return (int16_t)value;
}

void ReadAnalogJoystick_3F587_3F8C7()
{
	if (!joystickFound_B7700_B76F0)
	{
		joyX_B7702_B76F2 = 0;
		joyY_B7704_B76F4 = 0;
		joyButtons_B772A_B771A = 0xFF;
		return;
	}
	const GamePortCounts counts = ReadGamePortCounts_3F76D_3FAAD();
	joyButtons_B772A_B771A = counts.bits & 0xF0;
	if (joystickFound_B7700_B76F0)
	{
		joyX_B7702_B76F2 = AnalogAxis(counts.x, joyMinX_B770A_B76FA, joyMaxX_B7706_B76F6, joyScaleMinX_B7712_B7702, joyScaleMaxX_B770E_B76FE);
		joyY_B7704_B76F4 = AnalogAxis(counts.y, joyMinY_B770C_B76FC, joyMaxY_B7708_B76F8, joyScaleMinY_B7714_B7704, joyScaleMaxY_B7710_B7700);
	}
	if (secondStick_B7701_B76F1)
	{
		joy2X_B7716_B7706 = AnalogAxis(counts.x2, joy2MinX_B771E_B770E, joy2MaxX_B771A_B770A, joy2ScaleMinX_B7726_B7716, joy2ScaleMaxX_B7722_B7712);
		joy2Y_B7718_B7708 = AnalogAxis(counts.y2, joy2MinY_B7720_B7710, joy2MaxY_B771C_B770C, joy2ScaleMinY_B7728_B7718, joy2ScaleMaxY_B7724_B7714);
	}
}

void ReadDigitalJoystick_3F413_3F753()
{
	if (joystickFound_B7700_B76F0)
	{
		int16_t x = 0;
		int16_t y = 0;
		for (int32_t count = 0x2710; count; count--)
		{
			const uint8_t bits = ReadGamePort();
			if (!(bits & 3))
			{
				joyButtons_B772A_B771A = bits;
				joyX_B7702_B76F2 = x < joyMinX_B770A_B76FA ? -1 : x > joyMaxX_B7706_B76F6 ? 1 : 0;
				joyY_B7704_B76F4 = y < joyMinY_B770C_B76FC ? -1 : y > joyMaxY_B7708_B76F8 ? 1 : 0;
				return;
			}
			if (bits & 1)
				x++;
			if (bits & 2)
				y++;
		}
		joystickFound_B7700_B76F0 = 0;
	}
	joyX_B7702_B76F2 = 0;
	joyY_B7704_B76F4 = 0;
	joyButtons_B772A_B771A = 0xFF;
}

// the joystick buttons 1 and 2 (the bits 10h and 20h, 0 pressed) as the mouse buttons
static void JoystickButtons()
{
	if (!(joyButtons_B772A_B771A & 0x10) && !joyLeftDown_90B21_90B21)
	{
		joyLeftDown_90B21_90B21 = 1;
		*mouseLeftDown_12EFE4_12EFD4 = 1;
		*mouseLeftPressed_12EFDE_12EFCE = 1;
	}
	else if (joyLeftDown_90B21_90B21 && (joyButtons_B772A_B771A & 0x10))
	{
		joyLeftDown_90B21_90B21 = 0;
		*mouseLeftDown_12EFE4_12EFD4 = 0;
		*mouseLeftPressed_12EFDE_12EFCE = 0;
	}
	if (!(joyButtons_B772A_B771A & 0x20) && !joyRightDown_90B22_90B22)
	{
		joyRightDown_90B22_90B22 = 1;
		*mouseRightDown_12EFE2_12EFD2 = 1;
		*mouseRightPressed_12EFDC_12EFCC = 1;
	}
	else if (joyRightDown_90B22_90B22 && (joyButtons_B772A_B771A & 0x20))
	{
		joyRightDown_90B22_90B22 = 0;
		*mouseRightDown_12EFE2_12EFD2 = 0;
		*mouseRightPressed_12EFDC_12EFCC = 0;
	}
}

// the axes as the arrow keys (the joystick without the button 3, or with the VFX1 and the button 3), else 5B2B0
static void JoystickArrows()
{
	const bool button3Up = joyButtons_B772A_B771A & 0x40;
	if (button3Up == (vfxTracking_9AD08_9AD08 != 0))
	{
		if (joyY_B7704_B76F4 < 0)
			(*pressedKeys_12EEF0_12EEE0)[0x48] = 1;
		else if (joyY_B7704_B76F4 > 0)
			(*pressedKeys_12EEF0_12EEE0)[0x50] = 1;
		if (joyX_B7702_B76F2 > 0)
			(*pressedKeys_12EEF0_12EEE0)[0x4D] = 1;
		else if (joyX_B7702_B76F2 < 0)
			(*pressedKeys_12EEF0_12EEE0)[0x4B] = 1;
	}
	else if (vfxTracking_9AD08_9AD08 && !button3Up)
		sub_5B2B0_5B7C0();
}

static void ClearArrows()
{
	(*pressedKeys_12EEF0_12EEE0)[0x48] = 0;
	(*pressedKeys_12EEF0_12EEE0)[0x50] = 0;
	(*pressedKeys_12EEF0_12EEE0)[0x4B] = 0;
	(*pressedKeys_12EEF0_12EEE0)[0x4D] = 0;
}

// the button 4 (bit 80h) is Return every 4 frames
static void JoystickReturn()
{
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	if (!(joyButtons_B772A_B771A & 0x80) && !(state->players_0x340B_13323[state->currentPlayer_0x08_8].turn_0x12_18 & 3))
		*lastKey_12EF70_12EF60 = 0x1C;
}

void JoystickInput_3FCA0_3FFE0(int32_t mode, int32_t minX, int32_t minY, int32_t maxX, int32_t maxY, int32_t stepX, int32_t stepY)
{
	if ((uint8_t)mode)
	{
		// a menu: the joystick moves the mouse by the steps in the area, the buttons press
		if (!(inputDevices_90B23_90B23 & 2) && !(inputDevices_90B23_90B23 & 1))
			return;
		JoystickReturn();
		if (*mouseInstalled_9AD98_9AD98)
			ResetMouse_3F370_3F6B0();
		*mouseRightPressed_12EFDC_12EFCC = 0;
		*mouseRightDown_12EFE2_12EFD2 = 0;
		*mouseLeftPressed_12EFDE_12EFCE = 0;
		*mouseLeftDown_12EFE4_12EFD4 = 0;
		if (!joyMenuMouse_90B20_90B20)
		{
			mousePos_9AD90_9AD90->y = 0;
			mousePos_9AD90_9AD90->x = 0;
			joyMenuMouse_90B20_90B20 = 1;
		}
		if (inputDevices_90B23_90B23 & 2)
			ReadDigitalJoystick_3F413_3F753();
		else
			ReadAnalogJoystick_3F587_3F8C7();
		if (joyY_B7704_B76F4 < 0)
			mousePos_9AD90_9AD90->y -= (int16_t)stepY;
		else if (joyY_B7704_B76F4 > 0)
			mousePos_9AD90_9AD90->y += (int16_t)stepY;
		else if (joyX_B7702_B76F2 > 0)
			mousePos_9AD90_9AD90->x += (int16_t)stepX;
		else if (joyX_B7702_B76F2 < 0)
			mousePos_9AD90_9AD90->x -= (int16_t)stepX;
		if (mousePos_9AD90_9AD90->x < (int16_t)minX)
			mousePos_9AD90_9AD90->x = (int16_t)minX;
		else if (mousePos_9AD90_9AD90->x > (int16_t)maxX)
			mousePos_9AD90_9AD90->x = (int16_t)maxX;
		if (mousePos_9AD90_9AD90->y < (int16_t)minY)
			mousePos_9AD90_9AD90->y = (int16_t)minY;
		else if (mousePos_9AD90_9AD90->y > (int16_t)maxY)
			mousePos_9AD90_9AD90->y = (int16_t)maxY;
		if (!(joyButtons_B772A_B771A & 0x10))
		{
			*mouseLeftDown_12EFE4_12EFD4 = 1;
			*mouseLeftPressed_12EFDE_12EFCE = 1;
		}
		if (!(joyButtons_B772A_B771A & 0x20))
		{
			*mouseRightDown_12EFE2_12EFD2 = 1;
			*mouseRightPressed_12EFDC_12EFCC = 1;
		}
		return;
	}

	// the game
	joyMenuMouse_90B20_90B20 = 0;
	if (vfxTracking_9AD08_9AD08)
	{
		// the VFX1 head moves the mouse, the average of the last 2 readings
		if (*mouseInstalled_9AD98_9AD98)
			ResetMouse_3F370_3F6B0();
		sub_5B210_5B720();
		vfxSamplesY_90B0C_90B0C[vfxIndexY_90B14_90B14] = vfxTrackerY_12EE9E_12EE8E;
		vfxIndexY_90B14_90B14 = (vfxIndexY_90B14_90B14 + 1) % 2;
		const int32_t y = (int16_t)(vfxSamplesY_90B0C_90B0C[0] + vfxSamplesY_90B0C_90B0C[1]) / 2;
		if (*videoMode_12F02E_12F01E & 1)
			mousePos_9AD90_9AD90->y = (int16_t)(y * 400 / 0x31C7 + 0xC8);
		else
			mousePos_9AD90_9AD90->y = (int16_t)(y * 480 / 0x31C7 + 0xF0);
		vfxSamplesX_90B04_90B04[vfxIndexX_90B18_90B18] = vfxTrackerX_12EEA0_12EE90;
		vfxIndexX_90B18_90B18 = (vfxIndexX_90B18_90B18 + 1) % 2;
		const int32_t x = (int16_t)(vfxSamplesX_90B04_90B04[0] + vfxSamplesX_90B04_90B04[1]) / 2;
		mousePos_9AD90_9AD90->x = (int16_t)(x * 640 / 0x31C7 + 0x140);
	}
	if (!(inputDevices_90B23_90B23 & 2) && !(inputDevices_90B23_90B23 & 1) && !(inputDevices_90B23_90B23 & 0x20))
		return;
	if (*mouseInstalled_9AD98_9AD98)
		ResetMouse_3F370_3F6B0();
	if (joystickFound_B7700_B76F0 == 2)
	{
		// digital: the button 3 held moves the mouse smoothly, else the arrows
		ReadDigitalJoystick_3F413_3F753();
		ClearArrows();
		JoystickReturn();
		if ((joyButtons_B772A_B771A & 0x40) && !vfxTracking_9AD08_9AD08)
		{
			joyMouseX_90B1C_90B1C = (int16_t)((int16_t)(joyMouseX_90B1C_90B1C + joyX_B7702_B76F2 * 32) * 28 / 32);
			joyMouseY_90B1E_90B1E = (int16_t)((int16_t)(joyMouseY_90B1E_90B1E + joyY_B7704_B76F4 * 16) * 31 / 32);
			mousePos_9AD90_9AD90->x = (int16_t)(joyMouseX_90B1C_90B1C * 2 + 0x140);
			mousePos_9AD90_9AD90->y = (int16_t)(joyMouseY_90B1E_90B1E * 2 + ((*videoMode_12F02E_12F01E & 1) ? 0xC8 : 0xF0));
		}
		else
			JoystickArrows();
		JoystickButtons();
	}
	else if (joystickFound_B7700_B76F0 == 1)
	{
		// analog: the stick is the mouse, or the arrows
		ReadAnalogJoystick_3F587_3F8C7();
		if ((inputDevices_90B23_90B23 & 0x20) || vfxTracking_9AD08_9AD08)
			ClearArrows();
		JoystickReturn();
		if ((joyButtons_B772A_B771A & 0x40) && !vfxTracking_9AD08_9AD08)
		{
			joyMouseX_90B1C_90B1C = (int16_t)(joyX_B7702_B76F2 * 2);
			joyMouseY_90B1E_90B1E = (int16_t)(joyY_B7704_B76F4 * 2);
			mousePos_9AD90_9AD90->x = (int16_t)(joyMouseX_90B1C_90B1C + 0x140);
			mousePos_9AD90_9AD90->y = (int16_t)(joyMouseY_90B1E_90B1E + ((*videoMode_12F02E_12F01E & 1) ? 0xC8 : 0xF0));
		}
		else
			JoystickArrows();
		JoystickButtons();
	}
}

// a mouse button casts the spell of its side: a click, or held for a spell that repeats (word_0x30) or charges
static void CastSpell(Type_Mc1Entity* wizard, uint16_t slotOffset, int16_t* pressed, const int16_t* down, uint8_t button)
{
	const uint8_t* playerData = wizard->playerData_0xA0_160;
	const int16_t slot = ReadS16(playerData + slotOffset);
	if (slot == -1)
	{
		*pressed = 0;
		return;
	}
	const Type_Mc1Entity& spell = Entities()[ReadS32(playerData + 0x214 + slot * 4)];
	if (spell.byte_0x3C_60 == 1)
	{
		if (*pressed)
		{
			SetPlayerCommand_188A0_188A0(6, button);
			*pressed = 0;
		}
		return;
	}
	if (*pressed || (*down && spell.word_0x30_48 > 0) || (*down && spell.byte_0x3E_62 && (int8_t)spell.byte_0x3D_61 > 0))
	{
		SetPlayerCommand_188A0_188A0(6, button);
		*pressed = 0;
	}
}

// 17C79: the keys with Ctrl (1-0), Alt or Shift, then the plain keys, the arrows and the mouse
static void ProcessGameInput(Type_Mc1State_AE400_AE3F0* state, Type_Mc1Player_340B& player)
{
	Type_Mc1Settings_AE408_AE3F8* settings = mc1Settings_AE408_AE3F8;
	if (CurrentInput().command_0x00_0)
	{
		*mouseLeftPressed_12EFDE_12EFCE = 0;
		*mouseRightPressed_12EFDC_12EFCC = 0;
		return;
	}
	FunctionKeys_16B00_16B00();
	JoystickInput_3FCA0_3FFE0(0, 0x100, 0xA0, 0x280, 0x190, 0x10, 8);
	Type_Mc1Entity* wizard = &Entities()[player.entityIndex_0x0A_10];
	CurrentInput().buttons_0x05_5 = 0;
	uint8_t& key = *lastKey_12EF70_12EF60;
	if (KeyDown(0x1D))
	{
		// Ctrl 1-0
		if (key >= 2 && key <= 0x0B)
		{
			SetPlayerCommand_188A0_188A0(0x19, key - 2);
			key = 0;
		}
		MouseToMove_169E0_169E0();
		return;
	}
	if (KeyDown(0x38))
	{
		// Alt
		switch (key)
		{
		case 0x23: // H: VFX1
			state->vfx_0x219E_8606 ^= 1;
			key = 0;
			break;
		case 0x13: // R
			SetPlayerCommand_188A0_188A0(0x0C, 0);
			key = 0;
			break;
		case 0x1F: // S
			SetPlayerCommand_188A0_188A0(0x0A, 0);
			key = 0;
			break;
		case 0x2F: // V
			SetPlayerCommand_188A0_188A0(4, 8);
			key = 0;
			break;
		case 0x32: // M
			SetPlayerCommand_188A0_188A0(4, 0x10);
			key = 0;
			break;
		case 0x21: // F
			SetPlayerCommand_188A0_188A0(4, 0x20);
			key = 0;
			break;
		case 0x3B: // F1-F7
		case 0x3C:
		case 0x3D:
		case 0x3E:
		case 0x3F:
		case 0x40:
		case 0x41:
			SetPlayerCommand_188A0_188A0(0x1E, key - 0x3A);
			key = 0;
			break;
		}
		MouseToMove_169E0_169E0();
		return;
	}
	if (KeyDown(0x2A) || KeyDown(0x36))
	{
		// Shift
		switch (key)
		{
		case 0x25: // K: the wizard dies
			if (!(settings->flags_0x00_0 & 0x10))
				wizard->life_0x0C_12 = -1;
			key = 0;
			break;
		case 0x26: // L
			SetPlayerCommand_188A0_188A0(6, 0x30);
			key = 0;
			break;
		case 0x2E: // C
			if (settings->flags_0x00_0 & 0x8000)
				SetPlayerCommand_188A0_188A0(0x1B, 0);
			key = 0;
			break;
		case 0x10: // Q: quit
			SetPlayerCommand_188A0_188A0(2, 0);
			key = 0;
			break;
		case 0x12: // E
			if (settings->flags_0x00_0 & 0x8000)
				SetPlayerCommand_188A0_188A0(0x1A, 0);
			key = 0;
			break;
		}
		MouseToMove_169E0_169E0();
		return;
	}
	if (key == 0x1A)
	{
		// [ ]: the size of the view port
		if (!state->screenSize_0x219B_8603 && (int8_t)state->setting_0x2198_8600 < 0x28)
			state->setting_0x2198_8600++;
		key = 0;
	}
	if (key == 0x1B)
	{
		if (!state->screenSize_0x219B_8603 && (int8_t)state->setting_0x2198_8600 > 0x11)
			state->setting_0x2198_8600--;
		key = 0;
	}
	if (key == 0x17)
	{
		// I
		SetPlayerCommand_188A0_188A0(0x10, 0);
		key = 0;
		return;
	}
	if (key == 1)
	{
		// Esc
		SetPlayerCommand_188A0_188A0(0x1B, 0);
		SetPlayerCommand_188A0_188A0(0x1D, 0);
		key = 0;
		return;
	}
	if (key == 0x13)
	{
		// R
		if (state->options_0x21AD_8621[10])
			ToggleResolution_34C80_35040();
		key = 0;
	}
	if (key == 0x39)
	{
		// Space: the rebirth
		SetPlayerCommand_188A0_188A0(0x0F, 0);
		SetPlayerCommand_188A0_188A0(0x1B, 0);
		SetPlayerCommand_188A0_188A0(0x1C, 0);
		key = 0;
	}
	if (*mouseLeftPressed_12EFDE_12EFCE && *mouseRightPressed_12EFDC_12EFCC)
	{
		// both buttons: the spells mode
		if (!(settings->flags_0x00_0 & 0x200) && wizard->life_0x0C_12 >= 0)
			SetPlayerCommand_188A0_188A0(0x14, 2);
		*mouseRightPressed_12EFDC_12EFCC = 0;
		*mouseLeftPressed_12EFDE_12EFCE = 0;
	}
	if (key >= 2 && key <= 0x0B)
	{
		// 1-0
		SetPlayerCommand_188A0_188A0(0x18, key - 2);
		key = 0;
	}
	else if (key == 0x1C)
	{
		// Return: the spells mode
		if (wizard->life_0x0C_12 >= 0)
			SetPlayerCommand_188A0_188A0(0x14, 2);
		key = 0;
	}
	// the arrows
	if (KeyDown(0x48))
		SetPlayerCommand_188A0_188A0(6, 1);
	if (KeyDown(0x50))
		SetPlayerCommand_188A0_188A0(6, 2);
	if (KeyDown(0x4B))
		SetPlayerCommand_188A0_188A0(6, 4);
	if (KeyDown(0x4D))
		SetPlayerCommand_188A0_188A0(6, 8);
	CastSpell(wizard, 0x3AC, mouseLeftPressed_12EFDE_12EFCE, mouseLeftDown_12EFE4_12EFD4, 0x10);
	CastSpell(wizard, 0x3B0, mouseRightPressed_12EFDC_12EFCC, mouseRightDown_12EFE2_12EFD2, 0x20);
	MouseToMove_169E0_169E0();
}

// 18457: the spell under the mouse to a key 1-0 (17h), to the left (15h) or the right (16h) button
static void ProcessSpellsInput(Type_Mc1State_AE400_AE3F0* state, Type_Mc1Player_340B& player)
{
	Type_Mc1Settings_AE408_AE3F8* settings = mc1Settings_AE408_AE3F8;
	const uint8_t width = spritesIndex[3].width_4;
	const uint8_t height = spritesIndex[3].height_5;
	JoystickInput_3FCA0_3FFE0(1, (int16_t)(0x280 - width * 4), 0xA2, (int16_t)(0x280 - width), (int16_t)(0xA2 + height * 5), width, height);
	FunctionKeys_16B00_16B00();
	Type_Mc1Entity* wizard = &Entities()[player.entityIndex_0x0A_10];
	Type_Mc1PlayerInput& input = CurrentInput();
	uint8_t& key = *lastKey_12EF70_12EF60;
	if (wizard->life_0x0C_12 < 0)
		SetPlayerCommand_188A0_188A0(0x14, 0);
	// the slot of the selected spell: its entity (playerData + 2A4h) among the spells of the wizard (+ 214h)
	auto spellSlot = [&](int8_t* slot) {
		const uint8_t* playerData = wizard->playerData_0xA0_160;
		for (int32_t i = 0; i < 24; i++)
		{
			if (ReadS16(playerData + 0x2A4 + spellOrder_99B88_99B88[settings->selectedSpell_0x16_22] * 2) == ReadS32(playerData + 0x214 + i * 4))
			{
				*slot = (int8_t)i;
				return true;
			}
		}
		return false;
	};
	if (key >= 2 && key <= 0x0B)
	{
		if (settings->selectedSpell_0x16_22 != -1)
		{
			input.command_0x00_0 = 0x17;
			input.param_0x01_1 = (int8_t)(key - 2);
			input.param2_0x02_2 = -1;
			spellSlot(&input.param2_0x02_2);
		}
		key = 0;
	}
	if ((*mouseLeftPressed_12EFDE_12EFCE && *mouseRightPressed_12EFDC_12EFCC) || key == 0x1C)
	{
		key = 0;
		*mouseRightPressed_12EFDC_12EFCC = 0;
		*mouseLeftPressed_12EFDE_12EFCE = 0;
		SetPlayerCommand_188A0_188A0(0x14, 0);
	}
	else if (*mouseLeftPressed_12EFDE_12EFCE)
	{
		*mouseLeftPressed_12EFDE_12EFCE = 0;
		if (settings->selectedSpell_0x16_22 != -1)
		{
			input.command_0x00_0 = 0x15;
			input.param_0x01_1 = -1;
			spellSlot(&input.param_0x01_1);
		}
	}
	else if (*mouseRightPressed_12EFDC_12EFCC)
	{
		*mouseRightPressed_12EFDC_12EFCC = 0;
		if (settings->selectedSpell_0x16_22 != -1)
		{
			input.command_0x00_0 = 0x16;
			input.param_0x01_1 = -1;
			spellSlot(&input.param_0x01_1);
		}
	}
	key = 0;
}

void ProcessInput_17C20_17C20()
{
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	Type_Mc1Player_340B& player = state->players_0x340B_13323[state->currentPlayer_0x08_8];
	uint8_t& key = *lastKey_12EF70_12EF60;
	if (player.computer_0x09_9 == 1)
	{
		// the computer plays (the demo): F1 leaves
		if (KeyDown(0x3B))
			SetPlayerCommand_188A0_188A0(2, 0);
		return;
	}
	switch (player.mode_0x44A_1098)
	{
	case 0:
	case 4:
		ProcessGameInput(state, player);
		break;
	case 1:
		// the help: both buttons or Return back to the game
		FunctionKeys_16B00_16B00();
		if ((*mouseLeftPressed_12EFDE_12EFCE && *mouseRightPressed_12EFDC_12EFCC) || key == 0x1C)
		{
			key = 0;
			*mouseRightPressed_12EFDC_12EFCC = 0;
			*mouseLeftPressed_12EFDE_12EFCE = 0;
			SetPlayerCommand_188A0_188A0(0x14, 0);
		}
		break;
	case 2:
		ProcessSpellsInput(state, player);
		break;
	case 3:
		// a message being typed: Return sends it, letters, digits, backspace and space are added
		if (key && key <= 0x7F)
		{
			const uint8_t c = scanCodeChars_9AD0C_9AD0C[key];
			if (key == 0x1C)
				SetPlayerCommand_188A0_188A0(0x13, 0);
			else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == 8 || c == ' ')
				SetPlayerCommand_188A0_188A0(0x11, c);
		}
		key = 0;
		break;
	}
}
