#pragma once

#include <cstdint>
#include <cstdio>

#include "../../remc2/engine/engine_support.h"
#include "../../remc2/engine/Type_Animations.h"

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

// the cache of the tmaps (in the heap of InitMemory) and their animations, the same types as MC2 (TextureMaps)
extern type_x_DWORD_E9C28_str* tmapCache_AE3EC_AE3DC;
extern type_E9C08* animations_AE3E0_AE3D0;
extern int32_t tmapLastUse_12CF00_12CEF0[0x211]; // the frame of the last use (settings frames_0x04)
extern subtype_x_DWORD_E9C28_str* tmapEntries_12D748_12D738[0x211];
extern Type_Sprite** loadedTmaps_12DF8C_12DF7C[0x211]; // the entry of a loaded tmap (its first member is the sprite)
extern uint8_t lockedTmaps_12E7D0_12E7C0[0x211]; // 0FFh: always loaded (the sprites of level sprite group -1)
extern uint8_t byte_968E8_968E8;

type_x_DWORD_E9C28_str* InitTmapCache_36700_36AC0(uint32_t size, uint16_t count, uint8_t* buffer);
int32_t GetTmapCacheFreeSize_369D0_36D90(type_x_DWORD_E9C28_str* cache);
subtype_x_DWORD_E9C28_str* AddTmapCacheEntry_369E0_36DA0(type_x_DWORD_E9C28_str* cache, uint32_t size, int16_t tmap);
void FreeTmapCacheEntry_36A90_36E50(type_x_DWORD_E9C28_str* cache, subtype_x_DWORD_E9C28_str* entry);
void FreeTmapCache_36C10_36FD0(type_x_DWORD_E9C28_str** cache);
type_E9C08* AllocateAnimations_10CB0_10CB0(uint16_t count);
type_animations1* InitTmapAnimation_10D40_10D40(type_E9C08* animations, Type_Sprite** sprite, int16_t index);
void AnimateSprite_10EB0_10EB0(type_animations1* animation);
void FreeAnimations_11010_11010(type_E9C08** animations);
uint8_t ResetTmapGroup_58910_58E20(uint16_t tmap);
uint8_t UnloadTmapGroup_58A00_58F10(uint16_t tmap);
uint32_t GetTmapGroupSize_58AD0_58FE0(uint16_t tmap);
void LoadTmapGroup_58B30_59040(uint16_t tmap);
uint32_t EvictLeastUsedTmaps_58C30_59140(uint32_t needed);
void InitTmapsAndSprites_58E70_59380();
void FreeTmaps_58F00_59410();
int32_t MainInitTmaps_59050_59560(uint16_t tmap);
void UpdateSpriteAnimations_590D0_595E0();
void LoadLevelTmaps_59230_59740();
void LockLevelSprites_59320_59830();
void LoadLockedTmaps_59370_59880();
void MarkLoadedTmaps_593B0_598C0();
void ReloadTmaps_59420_59930();
