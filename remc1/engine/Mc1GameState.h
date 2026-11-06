#pragma once

#include <cstdint>

// MC1 game state and settings, members only as far as they are used by the ported code

#pragma pack(push, 1)
typedef struct { // size 0x801
	uint8_t unknown_0x00_0[2];
	uint16_t levelResult_0x02_2; // 2 next level, 4 play again, 8 end
	uint8_t exitGame_0x04_4;
	uint8_t unknown_0x05_5[0x801 - 0x05];
} Type_Mc1Player_340B;

typedef struct { // size 232713, DOSBox 26C400 (pointer)
	uint8_t unknown_0x00_0[0x08];
	int16_t currentPlayer_0x08_8;
	uint8_t unknown_0x0A_10[0x240 - 0x0A];
	int32_t levelVariant_0x240_576;
	uint8_t unknown_0x244_580[0x340B - 0x244];
	Type_Mc1Player_340B players_0x340B_13323[8];
	uint8_t unknown_0x7413_29715[232713 - 0x7413];
} Type_Mc1State_AE400_AE3F0;

typedef struct { // size 36478
	uint32_t flags_0x00_0; // 4 no exit screen, 8 no game, 0x100 roll (no menu)
	uint8_t unknown_0x04_4[0x11 - 0x04];
	uint16_t levelIndex_0x11_17;
	uint8_t unknown_0x13_19[0x17 - 0x13];
	uint8_t flag_0x17_23;
	uint8_t unknown_0x18_24[36478 - 0x18];
} Type_Mc1Settings_AE408_AE3F8;
#pragma pack(pop)

static_assert(sizeof(Type_Mc1Player_340B) == 0x801, "Type_Mc1Player_340B");
static_assert(sizeof(Type_Mc1State_AE400_AE3F0) == 232713, "Type_Mc1State_AE400_AE3F0");
static_assert(sizeof(Type_Mc1Settings_AE408_AE3F8) == 36478, "Type_Mc1Settings_AE408_AE3F8");
