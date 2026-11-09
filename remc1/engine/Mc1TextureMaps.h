#pragma once

#include <cstdint>
#include <cstdio>

// MC1 texture maps (tmaps0-0 / tmaps1-0) and the sprite definitions using them; MC2 has the same in TextureMaps

typedef struct { // DOS 14 bytes, the same in MC1 and Hidden Worlds
	uint16_t textureIndex_0x00_0;
	const char* name_0x02_2; // debug name, unused by the game
	uint16_t width_0x06_6; // 0: computed from height and the tmap header
	uint16_t height_0x08_8; // 0: computed from width and the tmap header
	uint8_t lightingTableIdx_0x0A_10;
	int8_t levelSpriteGroup_0x0B_11;
	uint8_t kind_0x0C_12; // byte 1 of the tmap header
} Type_Mc1SpriteDef_99BA0;

extern Type_Mc1SpriteDef_99BA0 spriteDefs_99BA0_99BA0[286]; // ends with width 0 and height 0
extern FILE* tmapsFile_968EC_968EC;

uint8_t OpenTmaps_587E0_58CF0();
int32_t ReadTmap_58860_58D70(uint16_t index, uint8_t* buffer);
void CloseTmaps_58F70_59480();
void ProcessTmaps_58F90_594A0();

extern uint8_t usedTmapGroups_12E9E1_12E9D1[0x211]; // the level sprite group of the tmap groups the level uses
void ClearUsedTmapGroups_59140_59650();
void MarkModelSprites_59160_59670(uint16_t entityClass, int32_t model, int32_t sprite);
void MarkSpriteTmapGroup_591E0_596F0(uint16_t sprite);
