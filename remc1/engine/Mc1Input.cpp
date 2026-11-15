#include "Mc1Input.h"

#include <cstring>

#include "Mc1Files.h"
#include "Mc1Game.h"
#include "Mc1Level.h"
#include "Mc1NotPorted.h"
#include "Mc1Shared.h"

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

static Type_Mc1Entity* Entities()
{
	return mc1State_AE400_AE3F0->entities_0x7463_29795;
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
	sub_16B00_16B00();
	sub_3FCA0_3FFE0(0, 0x100, 0xA0, 0x280, 0x190, 0x10, 8);
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
			sub_34C80();
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
	sub_3FCA0_3FFE0(1, (int16_t)(0x280 - width * 4), 0xA2, (int16_t)(0x280 - width), (int16_t)(0xA2 + height * 5), width, height);
	sub_16B00_16B00();
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
		sub_16B00_16B00();
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
