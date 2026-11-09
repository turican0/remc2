#pragma once

#include <cstdint>

// MC1 game state and settings, members only as far as they are used by the ported code

#pragma pack(push, 1)
typedef struct {
	int16_t x;
	int16_t y;
	int16_t z;
} Type_Mc1Position;

// a mail to an entity: the values of one kind are summed while the sender is set
typedef struct {
	int32_t value_0;
	uint16_t sender_4; // the id of the last sender, 0 none
} Type_Mc1Mail;

// an entity (DOS 0xA4 bytes at state + 0x7463 + n * 0xA4); members as far as they are checked in the MC1 asm
typedef struct Type_Mc1Entity {
	struct Type_Mc1Entity* next_0x00_0;
	uint32_t rand_0x04_4; // the entity index + the level seed
	int32_t maxLife_0x08_8; // MC2 maxLife_0x4
	int32_t life_0x0C_12; // MC2 life_0x8
	uint32_t flags_0x10_16; // 4 on the map, 8 new, 0x400 to free, 0x20400 reusable (MC2 flags_0xc_12_15)
	uint16_t mapNext_0x14_20; // the next entity in the map cell (MC2 oldMapEntity_0x16)
	uint16_t mapPrevious_0x16_22; // the previous entity in the map cell (MC2 nextEntity_0x18)
	uint16_t id_0x18_24; // MC2 id_0x1A
	int16_t word_0x1A_26;
	uint16_t mailMask_0x1C_28; // the kinds of mail the entity takes, bit by the kind
	int16_t word_0x1E_30;
	int16_t word_0x20_32;
	uint8_t unknown_0x22_34[0x2A - 0x22];
	uint16_t word_0x2A_42; // 27D30: the entity whose word_0x30 is set when the building is done
	int16_t word_0x2C_44;
	int16_t word_0x2E_46;
	int16_t word_0x30_48;
	int16_t word_0x32_50; // 127E0 sets 1Eh
	uint8_t unknown_0x34_52[0x3A - 0x34];
	uint8_t byte_0x3A_58;
	uint8_t byte_0x3B_59;
	uint8_t byte_0x3C_60;
	uint8_t unknown_0x3D_61[0x3F - 0x3D];
	uint8_t index_0x3F_63; // the low byte of the entity index
	uint8_t class_0x40_64; // MC2 class_0x3F
	uint8_t model_0x41_65; // MC2 model_0x40
	uint8_t byte_0x42_66;
	uint8_t byte_0x43_67;
	uint8_t byte_0x44_68;
	uint8_t unknown_0x45_69;
	uint8_t actionIndex_0x46_70; // the record of 14 bytes in the actions of the class (MC2 actionIndex_0x45)
	uint8_t byte_0x47_71;
	Type_Mc1Position position_0x48_72; // MC2 position_0x4C
	int16_t box_0x4E_78[4];
	int16_t sprite_0x56_86;
	uint8_t frame_0x58_88;
	uint8_t frameCount_0x59_89;
	Type_Mc1Mail mails_0x5A_90[6]; // by the kind, added by 127E0 / 120B0
	int16_t word_0x7E_126;
	int16_t word_0x80_128;
	uint8_t unknown_0x82_130[0x88 - 0x82];
	int32_t dword_0x88_136;
	int32_t mana_0x8C_140; // MC2 mana_0x90
	uint8_t unknown_0x90_144[0x96 - 0x90];
	Type_Mc1Position target_0x96_150; // moved by MoveEntity_41EC0 (MC2 axis_0x9A_154x)
	uint8_t* typeParams_0x9C_156; // dword_98F38 records (MC2 typeParams_0xA0)
	uint8_t* playerData_0xA0_160; // dword_B7330 (MC2 playerData_0xA4)
} Type_Mc1Entity;

// the entity with the 32-bit pointers of DOS: saved games and the checks against DOSBox
typedef struct {
	uint32_t next_0x00_0;
	uint32_t rand_0x04_4; // the entity index + the level seed
	int32_t maxLife_0x08_8; // MC2 maxLife_0x4
	int32_t life_0x0C_12; // MC2 life_0x8
	uint32_t flags_0x10_16; // 4 on the map, 8 new, 0x400 to free, 0x20400 reusable (MC2 flags_0xc_12_15)
	uint16_t mapNext_0x14_20; // the next entity in the map cell (MC2 oldMapEntity_0x16)
	uint16_t mapPrevious_0x16_22; // the previous entity in the map cell (MC2 nextEntity_0x18)
	uint16_t id_0x18_24; // MC2 id_0x1A
	int16_t word_0x1A_26;
	uint16_t mailMask_0x1C_28; // the kinds of mail the entity takes, bit by the kind
	int16_t word_0x1E_30;
	int16_t word_0x20_32;
	uint8_t unknown_0x22_34[0x2A - 0x22];
	uint16_t word_0x2A_42; // 27D30: the entity whose word_0x30 is set when the building is done
	int16_t word_0x2C_44;
	int16_t word_0x2E_46;
	int16_t word_0x30_48;
	int16_t word_0x32_50; // 127E0 sets 1Eh
	uint8_t unknown_0x34_52[0x3A - 0x34];
	uint8_t byte_0x3A_58;
	uint8_t byte_0x3B_59;
	uint8_t byte_0x3C_60;
	uint8_t unknown_0x3D_61[0x3F - 0x3D];
	uint8_t index_0x3F_63; // the low byte of the entity index
	uint8_t class_0x40_64; // MC2 class_0x3F
	uint8_t model_0x41_65; // MC2 model_0x40
	uint8_t byte_0x42_66;
	uint8_t byte_0x43_67;
	uint8_t byte_0x44_68;
	uint8_t unknown_0x45_69;
	uint8_t actionIndex_0x46_70; // the record of 14 bytes in the actions of the class (MC2 actionIndex_0x45)
	uint8_t byte_0x47_71;
	Type_Mc1Position position_0x48_72; // MC2 position_0x4C
	int16_t box_0x4E_78[4];
	int16_t sprite_0x56_86;
	uint8_t frame_0x58_88;
	uint8_t frameCount_0x59_89;
	Type_Mc1Mail mails_0x5A_90[6]; // by the kind, added by 127E0 / 120B0
	int16_t word_0x7E_126;
	int16_t word_0x80_128;
	uint8_t unknown_0x82_130[0x88 - 0x82];
	int32_t dword_0x88_136;
	int32_t mana_0x8C_140; // MC2 mana_0x90
	uint8_t unknown_0x90_144[0x96 - 0x90];
	Type_Mc1Position target_0x96_150; // moved by MoveEntity_41EC0 (MC2 axis_0x9A_154x)
	uint32_t typeParams_0x9C_156; // dword_98F38 records (MC2 typeParams_0xA0)
	uint32_t playerData_0xA0_160; // dword_B7330 (MC2 playerData_0xA4)
} Type_Mc1EntityShadow;

typedef struct { // 18 bytes, a feature of the level (2000 at level + 0x430, 0 not used)
	uint16_t type_0x00_0; // 10 created by CreateFeatures_36430, then 0
	uint16_t model_0x02_2;
	uint16_t x_0x04_4;
	uint16_t y_0x06_6;
	uint16_t word_0x08_8; // FFFFh created
	uint16_t word_0x0A_10;
	uint16_t word_0x0C_12; // a line to the linked feature
	uint16_t next_0x0E_14; // the previous feature of a line (the building sprite - 10h for model 2Dh)
	uint16_t link_0x10_16; // the next feature of a line
} Type_Mc1LevelFeature;

typedef struct { // size 0x979C, levels.dat (ddlevels.dat in HW) unpacked, at state + 0x2F503, no pointers
	uint8_t unknown_0x00_0[4];
	uint32_t seed_0x04_4;
	uint16_t offset_0x08_8;
	uint8_t unknown_0x0A_10[2];
	uint16_t raise_0x0C_12;
	uint8_t unknown_0x0E_14[2];
	uint16_t gnarl_0x10_16;
	uint8_t unknown_0x12_18[2];
	uint32_t river_0x14_20;
	uint16_t lriver_0x18_24;
	uint8_t unknown_0x1A_26[2];
	uint16_t source_0x1C_28;
	uint8_t unknown_0x1E_30[2];
	uint16_t snLin_0x20_32;
	uint8_t unknown_0x22_34[2];
	uint16_t snFlt_0x24_36;
	uint8_t unknown_0x26_38[2];
	uint16_t bhLin_0x28_40;
	uint8_t unknown_0x2A_42[2];
	uint16_t bhFlt_0x2C_44;
	uint8_t unknown_0x2E_46[0x430 - 0x2E];
	Type_Mc1LevelFeature features_0x430_1072[2000];
	uint8_t unknown_0x90D0_37072[0x9792 - 0x90D0];
	uint16_t playerCount_0x9792_38802;
	uint8_t unknown_0x9794_38804[0x979C - 0x9794];
} Type_Mc1Level_2F503;

typedef struct { // size 0x801
	uint8_t unknown_0x00_0[2];
	uint16_t levelResult_0x02_2; // 2 next level, 4 play again, 8 end
	uint8_t exitGame_0x04_4;
	uint8_t unknown_0x05_5[0x0A - 0x05];
	uint16_t entityIndex_0x0A_10; // the entity of the player (wizard)
	uint8_t unknown_0x0C_12[0x18 - 0x0C];
	int32_t cheat_0x18_24;
	uint8_t unknown_0x1C_28[0x7CB - 0x1C];
	uint8_t progress_0x7CB_1995[0x18]; // cleared by a new game, kept by the demo, in the saved games
	uint8_t unknown_0x7E3_2019[0x801 - 0x7E3];
} Type_Mc1Player_340B;

typedef struct { // size 232713, DOSBox 26C400 (pointer)
	uint8_t unknown_0x00_0[0x04];
	uint32_t rand_0x04_4; // the seed of the level
	int16_t currentPlayer_0x08_8;
	uint16_t playerCount_0x0A_10;
	uint8_t modelCounters_0x0C_12[0x14]; // cleared by ClearLevel_40550 (MC2 modelCounters_0x10)
	uint32_t dword_0x20_32;
	uint16_t word_0x24_36;
	uint16_t word_0x26_38;
	int32_t freeTop_0x28_40; // the top of freeEntities_0x251, -1 empty
	uint8_t unknown_0x2C_44[0x240 - 0x2C];
	int32_t levelVariant_0x240_576;
	uint8_t byte_0x244_580;
	uint32_t dword_0x245_581; // 1 after the loading screen
	uint32_t dword_0x249_585;
	uint32_t dword_0x24D_589;
	Type_Mc1Entity* freeEntities_0x251_593[1000];
	int32_t reuseTop_0x11F1_4593; // the top of reuseEntities_0x11F5, -1 empty
	Type_Mc1Entity* reuseEntities_0x11F5_4597[1000]; // entities with the flags 0x20400, taken when none is free
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
	uint8_t unknown_0x21B8_8632[0x23D9 - 0x21B8];
	uint8_t playerStarts_0x23D9_9177[0x30]; // MC2 playerStarts_0x2362
	Type_Mc1Player_340B networkPlayer_0x2409_9225; // player 0 kept by the network game (4FEB0)
	uint8_t unknown_0x2C0A_11274[0x340B - 0x2C0A];
	Type_Mc1Player_340B players_0x340B_13323[8];
	uint8_t unknown_0x7413_29715[0x7463 - 0x7413];
	Type_Mc1Entity entities_0x7463_29795[1000];
	Type_Mc1Level_2F503 level_0x2F503_193795;
	uint8_t creatureStats_0x38C9F_232607[0x6A]; // MC2 creatureStats_0x364D2
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
	char nickname_0x3D_61[32];
	uint8_t colorIndex_0x5D_93[16];
	uint8_t unknown_0x6D_109[0x75 - 0x6D];
	char sessionName_0x75_117[32];
	uint8_t unknown_0x95_149;
	uint8_t speedIndex_0x96_150;
	uint8_t language_0x97_151; // 0-3, chosen on the language screen
	uint8_t paletteSubMod_0x98_152;
	uint8_t unknown_0x99_153[0xA1 - 0x99];
	uint8_t unknown_0xA1_161; // 3 in the demo
	uint32_t unknown_0xA2_162; // 200 in the demo
	uint8_t unknown_0xA6_166[0xA8 - 0xA6];
	uint8_t* heap_0xA8_168; // allocated by InitMemory
	uint32_t heapSize_0xAC_172;
	uint8_t unknown_0xB0_176[0xB8 - 0xB0];
	uint8_t manaStats_0xB8_184[14];
	uint8_t unknown_0xC6_198[0xCE - 0xC6];
	uint32_t timerDivisor_0xCE_206; // 26AFh, the PIT divisor of the own 120 Hz timer
	uint32_t unknown_0xD2_210;
	uint8_t unknown_0xD6_214[0x8E1A - 0xD6];
	Type_Mc1Entity* entityLists_0x8E1A_36378[25]; // the entities by kind (MC2 creatureLists_38403x ...)
} Type_Mc1Settings_AE408_AE3F8;
#pragma pack(pop)

static_assert(sizeof(Type_Mc1Player_340B) == 0x801, "Type_Mc1Player_340B");
static_assert(sizeof(Type_Mc1EntityShadow) == 0xA4, "Type_Mc1EntityShadow");
static_assert(sizeof(Type_Mc1Entity) == 0xA4 + 3 * (sizeof(uint8_t*) - 4), "Type_Mc1Entity");
static_assert(sizeof(Type_Mc1State_AE400_AE3F0) == 232713 + 1000 * (sizeof(Type_Mc1Entity) - 0xA4) + 2000 * (sizeof(uint8_t*) - 4),
	"Type_Mc1State_AE400_AE3F0");
static_assert(sizeof(Type_Mc1Settings_AE408_AE3F8) == 36478 + 26 * (sizeof(uint8_t*) - 4), "Type_Mc1Settings_AE408_AE3F8"); // pointers are 4 bytes in DOS
static_assert(sizeof(Type_Mc1Level_2F503) == 0x979C, "Type_Mc1Level_2F503");
