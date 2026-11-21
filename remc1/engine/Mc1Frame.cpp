#include "Mc1Frame.h"

#include <cstdio>
#include <cstring>

#include "Mc1Files.h"
#include "Mc1Game.h"
#include "Mc1Graphics.h"
#include "Mc1Level.h"
#include "Mc1Minimap.h"
#include "Mc1NotPorted.h"
#include "Mc1Panel.h"
#include "Mc1Render.h"
#include "Mc1Screen.h"
#include "Mc1Shared.h"

uint8_t screenCleared_90094_90094 = 0x63; // 0 cleared for the game view, 2 for the map
uint8_t clearedSetting_90096_90096 = 0x28; // the view port setting the screen was cleared for
int32_t dword_90098_90098 = 0;

// 9AB44: the credits over the demo, '!' ends a page, '#' the list
static const char* credits_9AB44_9AB44[112] = {
	"             Designed by", "BULLFROG PRODUCTIONS LTD", "!", "Engine Programmed by", "        Glenn Corpes", "!",
	"Executive Producer", "    Peter Molyneux", "!", "Management", " Les Edgar", "!", "   Producer", "Sean Cooper", "!",
	"Lead Programmers", "     Sean Cooper", "    Mark Huntley", "!", " Programmers", "Simon Carter", "  Phil Jones",
	"!", "Graphics and Art Concepts by", "            Findlay McGechie", "             Paul McLaughlin",
	"                 Michael Man", "                 Mark Healey", "                  Eoin Rogan",
	"                 Barry Meade", "                 Tony Dawson", "!", "Introductory Sequence by",
	"              Chris Hill", "!", "Introductory Support", "         Michael Man", "     Paul McLaughlin",
	"          Eoin Rogan", "      Sean Masterson", "!", "Level Concepts and Architecture",
	"                 Sean Masterson", "!", "  Level Design", "   Barry Meade", "  Alex Trowers", "  Jonty Barnes",
	"Daniel Russell", "!", "Sound and Music", "   Russell Shaw", "!", "        Audio Technology",
	"Human Machine Interfaces", "!", "Technical Assistants", "        Kevin Donkin", "!", "PR Management",
	" Cathy Campos", "!", "       Game Testers", "     Andy Cakebread", "       Paul Boulden", "       Mike Diskett",
	"       Kevin Donkin", "C A Garner-Hamilton", "    Stuart Hastings", "       Mark Lamport", "       Paul Lockley",
	"   Roderic Mathison", "    Tristan Paramor", "      Randine Perry", "    Scott Rawlinson", "      Andrew Robins",
	"        Mat Solomom", "        Mark Webley", "!", "Special Thanks to", "      Sue Mumford", "       Jo Goodwin",
	"           Dennis", "    Kathy McEntee", "!", " EA Producer", "Matt Webster", "!", "EA Assistant Producer",
	"          Mike Cooper", "!", "      EA Testers", "     Darren King", "     Kevin Locke", "  Graham Harbour",
	"  Jamie Bradshaw", "   Darren Tuckey", "     Mark Bergan", "   Julian Glover", "    Erin Roberts",
	"     Danny Isaac", "Nick Goldsworthy", "!", "   Manual", "Neil Cook", "!", "Product Manager", " Sean Ratcliffe",
	"!", "COPYRIGHT BULLFROG PRODUCTIONS LIMITED 1994", "!", "#"
};

// 218EC: the help, a line lower each, nullptr an empty line
static const char* helpLines[] = {
	"Cursor up       Move forwards.",
	"Cursor down     Move backwards.",
	"Cursor left     Move left.",
	"Cursor right    Move right.",
	"Left button     Use spell to the left.",
	"Right button    Use spell to the right.",
	"1 - 0           Select assigned spells.",
	"Space           Rebirth.",
	"Shift Q         Exit to dos.",
	"F5              Reflections toggle.",
	"F6              Sky toggle.",
	"F7              Shadows toggle.",
	"F8              Icons toggle.",
	"F9              Speed blur toggle.",
	"F10             Stereo modes toggle.",
	"Return          ",
	"Left and Right  Spell selection.",
	nullptr,
	nullptr,
	"If you are experiencing slowness, try Pressing F5,F6,F7.",
	nullptr,
	nullptr,
	"Magic Carpet comes to you from Bullfrog Productions Ltd.",
	"And will be released on the 15th November 1994.",
	"Copyright 1994 Bullfrog Productions Ltd.",
	nullptr,
	"Good Luck!",
};

static Type_Mc1Entity* Entities()
{
	return mc1State_AE400_AE3F0->entities_0x7463_29795;
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

static void ClearScreen()
{
	if (*videoMode_12F02E_12F01E & 1)
		ClearScreen320_411D7_41517(*workScreen_12EFF4_12EFE4, 320, 0xC8, 0);
	else
		ClearScreen640_411FD_4153D(*workScreen_12EFF4_12EFE4, 640, 0x1E0, 0);
}

// 30D90 from the last record of the player (the camera)
static void DrawPlayerWorld(const Type_Mc1Player_340B& player)
{
	const Type_Mc1PlayerRecord& camera = player.records_0x24A_586[player.word_0x0E_14];
	DrawWorld_30D90_30DD0(*workScreen_12EFF4_12EFE4, camera.x_0x00_0, camera.y_0x02_2, camera.yaw_0x06_6,
		(int16_t)(camera.z_0x04_4 + 0x80), camera.pitch_0x08_8, camera.roll_0x0A_10, (int16_t)camera.word_0x0C_12);
}

// the colours of the texts: AE068 red, AE167 white, AD168 black, the blinking white / black or red / black
static uint8_t Red()
{
	return colorLookup_AD168_AD158[0xF00];
}

static uint8_t White()
{
	return colorLookup_AD168_AD158[0xFFF];
}

static uint8_t BlinkGrey()
{
	const int32_t level = mc1Settings_AE408_AE3F8->colorIndex_0x5D_93[2] * 0xFF0 / 256;
	return colorLookup_AD168_AD158[level * 0x111];
}

static uint8_t BlinkRed()
{
	const int32_t level = mc1Settings_AE408_AE3F8->colorIndex_0x5D_93[2] * 0xFF0 / 256;
	return colorLookup_AD168_AD158[level << 8];
}

static bool MouseInCell(int32_t x, int32_t y)
{
	return mousePos_9AD90_9AD90->x >= x && mousePos_9AD90_9AD90->x < x + 0x40
		&& mousePos_9AD90_9AD90->y >= y && mousePos_9AD90_9AD90->y < y + spritesIndex[3].height_5;
}

// 20FF4: the world, the minimap, the icons and the texts
static void DrawGame(Type_Mc1State_AE400_AE3F0* state, Type_Mc1Player_340B& player)
{
	Type_Mc1Settings_AE408_AE3F8* settings = mc1Settings_AE408_AE3F8;
	if (screenCleared_90094_90094 || (int8_t)state->setting_0x2198_8600 != clearedSetting_90096_90096)
	{
		screenCleared_90094_90094 = 0;
		clearedSetting_90096_90096 = state->setting_0x2198_8600;
		if ((int8_t)clearedSetting_90096_90096 < 0x28)
			ClearScreen();
	}
	if ((int8_t)state->setting_0x2198_8600 < 0x28)
		ClearScreen();
	viewPort.SetRenderViewPortSize_40C50(state->setting_0x2198_8600);
	DrawPlayerWorld(player);

	Type_Mc1Entity* wizard = &Entities()[player.entityIndex_0x0A_10];
	if (wizard->life_0x0C_12 < 0 || (settings->flags_0x00_0 & 4))
		return;
	const Type_Mc1PlayerRecord& camera = player.records_0x24A_586[player.word_0x0E_14];
	const int32_t mapWidth = *viewportWidth_12F000_12EFF0 - 2 * (int32_t)((uint32_t)*viewportWidth_12F000_12EFF0 / 5);
	if (state->map_0x2199_8601)
	{
		DrawMinimapTerrain_49300_49640(0, 0, camera.x_0x00_0, camera.y_0x02_2, 0x80, 0x80, camera.yaw_0x06_6, 0x100, mapWidth, 0);
		DrawMinimapEntities_48710_48A50(0, 0, camera.x_0x00_0, camera.y_0x02_2, 0x80, 0x80, camera.yaw_0x06_6, 0x100, mapWidth, 0);
	}
	SetFont_5A3C0_5A8D0(1);
	if (state->icons_0x219A_8602)
	{
		// the spells of the two buttons: the slot (playerData + 3ACh / 3B0h) of the spell entities (+ 214h)
		const uint8_t* playerData = wizard->playerData_0xA0_160;
		DrawSpellIcon_23D40_23D40(0x1FE, 2, &Entities()[ReadS32(playerData + 0x214 + ReadS16(playerData + 0x3AC) * 4)], 0);
		DrawSpellIcon_23D40_23D40(0x23E, 2, &Entities()[ReadS32(playerData + 0x214 + ReadS16(playerData + 0x3B0) * 4)], 0);
		DrawPlayerPanel_22E50_22E50(wizard);
	}
	SetFont_5A3C0_5A8D0(1);
	if (settings->flags_0x00_0 & 0x200)
	{
		SetFont_5A3C0_5A8D0(1);
		return;
	}
	SetFont_5A3C0_5A8D0(1);
	const int32_t x = 0x84;
	int32_t y = 0x32;
	if (player.levelResult_0x02_2 & 2)
	{
		// the texts 60 and 61 (the level is completed)
		DrawText_5A180_5A690((const char*)textPointers_AE238_AE228[60], x, (int16_t)y,
			state->setting_0x2198_8600 == 0x28 ? colorLookup_AD168_AD158[0] : BlinkGrey());
		y += GetLineHeight_5A3B0_5A8C0();
		DrawText_5A180_5A690((const char*)textPointers_AE238_AE228[61], x, (int16_t)y,
			state->setting_0x2198_8600 == 0x28 ? colorLookup_AD168_AD158[0] : BlinkGrey());
		y += GetLineHeight_5A3B0_5A8C0();
	}
	if (settings->flags_0x00_0 & 0x10000)
	{
		DrawText_5A180_5A690("PAUSED!", x, (int16_t)y, state->setting_0x2198_8600 == 0x28 ? colorLookup_AD168_AD158[0] : BlinkGrey());
		y += GetLineHeight_5A3B0_5A8C0();
	}
	// the messages from the players: 0 a message, 1 being typed, 2 a text, 3 a message to all (blinking first)
	char buffer[0x100];
	for (uint16_t i = 0; i < state->playerCount_0x0A_10; i++)
	{
		Type_Mc1Message& message = state->players_0x340B_13323[state->currentPlayer_0x08_8].messages_0x1C_28[i];
		const char* name = state->players_0x340B_13323[i].name_0x40A_1034;
		switch ((uint16_t)message.kind_0x42_66)
		{
		case 0:
			if (message.time_0x40_64 <= 0)
				break;
			snprintf(buffer, sizeof(buffer), "%s %s", name, message.text_0x00_0);
			DrawText_5A180_5A690(buffer, x, (int16_t)y, Red());
			y += GetLineHeight_5A3B0_5A8C0();
			message.time_0x40_64--;
			break;
		case 1:
			snprintf(buffer, sizeof(buffer), ">%s?", message.text_0x00_0);
			DrawText_5A180_5A690(buffer, x, (int16_t)y, Red());
			y += GetLineHeight_5A3B0_5A8C0();
			break;
		case 2:
			if (message.time_0x40_64 <= 0)
				break;
			snprintf(buffer, sizeof(buffer), "%s", message.text_0x00_0);
			DrawText_5A180_5A690(buffer, x, (int16_t)y, Red());
			y += GetLineHeight_5A3B0_5A8C0();
			message.time_0x40_64--;
			break;
		case 3:
			if (message.time_0x40_64 <= 0)
				break;
			snprintf(buffer, sizeof(buffer), "[%s] %s", name, message.text_0x00_0);
			DrawText_5A180_5A690(buffer, x, (int16_t)y, message.time_0x40_64 > 100 ? BlinkRed() : Red());
			y += GetLineHeight_5A3B0_5A8C0();
			message.time_0x40_64--;
			break;
		}
	}
	if (settings->flags_0x00_0 & 0x200)
		SetFont_5A3C0_5A8D0(1);
}

// 218EC: the world and the help over it
static void DrawHelp(Type_Mc1State_AE400_AE3F0* state, Type_Mc1Player_340B& player)
{
	if (screenCleared_90094_90094 || (int8_t)state->setting_0x2198_8600 != clearedSetting_90096_90096)
	{
		memset(*workScreen_12EFF4_12EFE4, 0, *screenWidth_12EFF0_12EFE0 * *screenHeight_12EFF8_12EFE8);
		screenCleared_90094_90094 = 0;
		clearedSetting_90096_90096 = state->setting_0x2198_8600;
	}
	viewPort.SetRenderViewPortSize_40C50(state->setting_0x2198_8600);
	DrawPlayerWorld(player);
	SetFont_5A3C0_5A8D0(1);
	const int16_t x = (int16_t)GetSpaceWidth_5A3A0_5A8B0();
	int32_t y = -(int16_t)GetLineHeight_5A3B0_5A8C0() / 2;
	for (const char* line : helpLines)
	{
		y += (int16_t)GetLineHeight_5A3B0_5A8C0();
		if (line)
			DrawText_5A180_5A690(line, x, (int16_t)y, White());
	}
}

// 2231E: the map in the top right corner and the minimap at 17Eh
static void DrawMap(Type_Mc1State_AE400_AE3F0* state, Type_Mc1Player_340B& player)
{
	if (screenCleared_90094_90094 != 2)
	{
		memset(*workScreen_12EFF4_12EFE4, 0, *screenWidth_12EFF0_12EFE0 * *screenHeight_12EFF8_12EFE8);
		screenCleared_90094_90094 = 2;
	}
	SetMapViewPort_309D0_30A10(0x10);
	DrawPlayerWorld(player);
	const Type_Mc1PlayerRecord& camera = player.records_0x24A_586[player.word_0x0E_14];
	const int32_t width = *viewportWidth_12F000_12EFF0 - 0xC0;
	// 320: 48710 has 17Eh where 49300 has 17Ah
	if (*videoMode_12F02E_12F01E == 1)
	{
		DrawMinimapTerrain_49300_49640(0, 0, camera.x_0x00_0, camera.y_0x02_2, 0x17E, 0x17A, camera.yaw_0x06_6, 0xAA, width, 1);
		DrawMinimapEntities_48710_48A50(0, 0, camera.x_0x00_0, camera.y_0x02_2, 0x17E, 0x17E, camera.yaw_0x06_6, 0xAA, width, 1);
	}
	else
	{
		DrawMinimapTerrain_49300_49640(0, 0, camera.x_0x00_0, camera.y_0x02_2, 0x17E, 0x19E, camera.yaw_0x06_6, 0xAA, width, 1);
		DrawMinimapEntities_48710_48A50(0, 0, camera.x_0x00_0, camera.y_0x02_2, 0x17E, 0x19E, camera.yaw_0x06_6, 0xAA, width, 1);
	}
	if (mousePos_9AD90_9AD90->y >= 0x17E)
		sub_22880_22880();
}

// 21FDC: the spells of the wizard in a grid of 4 x 6 (the one under the mouse larger), then the map
static void DrawSpells(Type_Mc1State_AE400_AE3F0* state, Type_Mc1Player_340B& player)
{
	Type_Mc1Settings_AE408_AE3F8* settings = mc1Settings_AE408_AE3F8;
	ClearScreen();
	Type_Mc1Entity* wizard = &Entities()[player.entityIndex_0x0A_10];
	const uint8_t* playerData = wizard->playerData_0xA0_160;
	const int32_t cellHeight = spritesIndex[3].height_5;
	int32_t x = 0x180;
	int32_t y = *videoMode_12F02E_12F01E == 1 ? 0xA2 : 0xC2;
	int32_t selectedX = 0;
	int32_t selectedY = 0;
	settings->selectedSpell_0x16_22 = -1;
	for (int16_t i = 0; i < 24; i++)
	{
		const uint8_t spell = spellOrder_99B88_99B88[i];
		// playerData + 2A4h: the entity of the spell, 0 not got
		const int16_t index = ReadS16(playerData + 0x2A4 + spell * 2);
		bool highlight = false;
		if (index > 0)
		{
			Type_Mc1Entity* entity = &Entities()[index];
			// the mana of the spell over the mana of the castle (playerData + 32h): not selectable
			const uint16_t castle = (uint16_t)ReadS16(playerData + 0x32);
			const bool usable = !entity->dword_0x84_132 || (castle && entity->dword_0x84_132 <= Entities()[castle].mana_0x8C_140);
			if (usable && MouseInCell(x, y))
			{
				settings->selectedSpell_0x16_22 = (int8_t)i;
				selectedX = x;
				selectedY = y;
			}
			else
			{
				sub_24230_24230(x, y, entity, 0);
				highlight = true;
			}
		}
		else
		{
			sub_23CF0_23CF0(x, y, spell);
			highlight = true;
		}
		if (highlight && joystickFound_B7700_B76F0 && MouseInCell(x, y))
		{
			if (*videoMode_12F02E_12F01E & 1)
				sub_24D20_24D20(x, y, spritesIndex[3].width_4, cellHeight, White());
			else
				sub_24DA0_24DA0(x, y, spritesIndex[3].width_4, cellHeight, White());
		}
		x += 0x40;
		if (x >= 0x280)
		{
			x = 0x180;
			y += cellHeight;
		}
	}
	if (settings->selectedSpell_0x16_22 != -1)
	{
		const int16_t index = ReadS16(playerData + 0x2A4 + spellOrder_99B88_99B88[settings->selectedSpell_0x16_22] * 2);
		if (index > 0)
			DrawSpellIcon_23D40_23D40((int16_t)selectedX, (int16_t)selectedY, &Entities()[index], 1);
	}
	DrawMap(state, player);
}

// 225DA: the credits over the demo, a page 50 frames, 200 frames between the runs
static void DrawCredits()
{
	Type_Mc1Settings_AE408_AE3F8* settings = mc1Settings_AE408_AE3F8;
	if (settings->credits_0xA1_161 == 3)
	{
		if (settings->creditsTime_0xA2_162 > 0)
			settings->creditsTime_0xA2_162--;
		else
			settings->credits_0xA1_161 = 2;
	}
	if (settings->credits_0xA1_161 == 2)
	{
		settings->credits_0xA1_161 = 1;
		settings->creditsLine_0xA6_166 = 0;
		settings->creditsTime_0xA2_162 = 0x32;
	}
	if (settings->credits_0xA1_161 != 1)
		return;
	if (settings->creditsTime_0xA2_162 > 0)
	{
		int32_t count = 0;
		while (credits_9AB44_9AB44[settings->creditsLine_0xA6_166 + count][0] != '!')
			count++;
		const int16_t x = 8;
		int32_t y = 8;
		for (int16_t i = 0; count > 0; count--, i++)
		{
			DrawText_5A180_5A690(credits_9AB44_9AB44[settings->creditsLine_0xA6_166 + i], x, (int16_t)y,
				i ? colorLookup_AD168_AD158[0] : White());
			y += (int16_t)GetLineHeight_5A3B0_5A8C0();
		}
		settings->creditsTime_0xA2_162--;
		return;
	}
	// the next page, the end of the list waits 200 frames
	while (credits_9AB44_9AB44[settings->creditsLine_0xA6_166][0] != '!')
		settings->creditsLine_0xA6_166++;
	settings->creditsLine_0xA6_166++;
	if (credits_9AB44_9AB44[settings->creditsLine_0xA6_166][0] == '#')
	{
		settings->credits_0xA1_161 = 3;
		settings->creditsTime_0xA2_162 = 0xC8;
		return;
	}
	settings->creditsTime_0xA2_162 = 0x32;
}

void DrawFrame_20FB0_20FB0()
{
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	Type_Mc1Settings_AE408_AE3F8* settings = mc1Settings_AE408_AE3F8;
	Type_Mc1Player_340B& player = state->players_0x340B_13323[state->currentPlayer_0x08_8];
	dword_90098_90098 = 0;
	switch (player.mode_0x44A_1098)
	{
	case 0:
	case 3:
		DrawGame(state, player);
		break;
	case 1:
		DrawHelp(state, player);
		break;
	case 2:
		DrawSpells(state, player);
		break;
	case 4:
		DrawMap(state, player);
		break;
	}
	SetFont_5A3C0_5A8D0(1);
	// the number of the movie being recorded, blinking
	if (!(settings->flags_0x00_0 & 0x20) && (player.turn_0x12_18 & 2) && (settings->flags_0x00_0 & 2))
	{
		char buffer[0x100];
		snprintf(buffer, sizeof(buffer), "MOVIE: %d", (int16_t)settings->movie_0x0D_13);
		DrawText_5A180_5A690(buffer, 0, 0, Red());
	}
	if (settings->flags_0x00_0 & 4)
		DrawCredits();
}
