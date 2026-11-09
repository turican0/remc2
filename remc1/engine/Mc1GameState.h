#pragma once

#include <cstdint>

// MC1 game state and settings, members only as far as they are used by the ported code

#pragma pack(push, 1)
typedef struct { // size 0x801
	uint8_t unknown_0x00_0[2];
	uint16_t levelResult_0x02_2; // 2 next level, 4 play again, 8 end
	uint8_t exitGame_0x04_4;
	uint8_t unknown_0x05_5[0x18 - 0x05];
	int32_t cheat_0x18_24;
	uint8_t unknown_0x1C_28[0x7CB - 0x1C];
	uint8_t progress_0x7CB_1995[0x18]; // cleared by a new game, kept by the demo, in the saved games
	uint8_t unknown_0x7E3_2019[0x801 - 0x7E3];
} Type_Mc1Player_340B;

typedef struct { // size 232713, DOSBox 26C400 (pointer)
	uint8_t unknown_0x00_0[0x08];
	int16_t currentPlayer_0x08_8;
	uint16_t playerCount_0x0A_10;
	uint8_t unknown_0x0C_12[0x240 - 0x0C];
	int32_t levelVariant_0x240_576;
	uint8_t unknown_0x244_580[0x2195 - 0x244];
	uint8_t reflections_0x2195_8597;
	uint8_t shadows_0x2196_8598;
	uint8_t sky_0x2197_8599;
	uint8_t setting_0x2198_8600; // 40 by default
	uint8_t map_0x2199_8601;
	uint8_t icons_0x219A_8602;
	uint8_t screenSize_0x219B_8603; // 0 full screen (MC2 0x2190)
	uint8_t unknown_0x219C_8604[0x219E - 0x219C];
	uint8_t vfx_0x219E_8606;
	uint8_t unknown_0x219F_8607;
	int8_t tileTextureSize_0x21A0_8608; // 0x20 (blk?-1) or 0x10 (blk?-0), by the memory
	uint8_t unknown_0x21A1_8609[0x21AD - 0x21A1];
	uint8_t options_0x21AD_8621[11]; // all 1 by default, [10] cleared with less memory
	uint8_t unknown_0x21B8_8632[0x2409 - 0x21B8];
	Type_Mc1Player_340B networkPlayer_0x2409_9225; // player 0 kept by the network game (4FEB0)
	uint8_t unknown_0x2C0A_11274[0x340B - 0x2C0A];
	Type_Mc1Player_340B players_0x340B_13323[8];
	uint8_t unknown_0x7413_29715[232713 - 0x7413];
} Type_Mc1State_AE400_AE3F0;

typedef struct { // size 36478
	uint32_t flags_0x00_0; // 4 no exit screen, 8 no game, 0x10 network, 0x40 time, 0x80 debug, 0x100 no menu (demo, custom, roll), 0x200-0x2000 demo number, 0x20 roll
	uint8_t unknown_0x04_4[0x08 - 0x04];
	uint8_t fastCpu_0x08_8;
	uint8_t unknown_0x09_9[0x0D - 0x09];
	uint16_t movie_0x0D_13;
	uint16_t roll_0x0F_15;
	uint16_t levelIndex_0x11_17;
	uint16_t networkLevel_0x13_19; // the level when the network game was chosen
	uint8_t unknown_0x15_21[0x17 - 0x15];
	uint8_t flag_0x17_23;
	uint8_t unknown_0x18_24;
	int32_t password_0x19_25;
	char playerName_0x1D_29[32]; // the first byte from byte_A9058
	char password_0x3D_61[32];
	uint8_t unknown_0x5D_93[0x75 - 0x5D];
	char sessionName_0x75_117[32];
	uint8_t unknown_0x95_149[0x97 - 0x95];
	uint8_t language_0x97_151; // 0-3, chosen on the language screen
	uint8_t unknown_0x98_152[0xA1 - 0x98];
	uint8_t unknown_0xA1_161; // 3 in the demo
	uint32_t unknown_0xA2_162; // 200 in the demo
	uint8_t unknown_0xA6_166[0xA8 - 0xA6];
	uint8_t* heap_0xA8_168; // allocated by InitMemory
	uint32_t heapSize_0xAC_172;
	uint8_t unknown_0xB0_176[0xCE - 0xB0];
	uint32_t timerDivisor_0xCE_206; // 26AFh, the PIT divisor of the own 120 Hz timer
	uint32_t unknown_0xD2_210;
	uint8_t unknown_0xD6_214[36478 - 0xD6];
} Type_Mc1Settings_AE408_AE3F8;
#pragma pack(pop)

static_assert(sizeof(Type_Mc1Player_340B) == 0x801, "Type_Mc1Player_340B");
static_assert(sizeof(Type_Mc1State_AE400_AE3F0) == 232713, "Type_Mc1State_AE400_AE3F0");
static_assert(sizeof(Type_Mc1Settings_AE408_AE3F8) == 36478 + sizeof(uint8_t*) - 4, "Type_Mc1Settings_AE408_AE3F8"); // pointers are 4 bytes in DOS
