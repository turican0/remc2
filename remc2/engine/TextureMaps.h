#pragma once

#ifndef TEXTURE_MAPS_H
#define TEXTURE_MAPS_H

#include "Type_Animations.h"
#include "Type_BigSpritesBuffer.h"
#include "Basic.h"

const int max_sprites = 600;

extern type_BIG_SPRITES_BUFFER BIG_SPRITES_BUFFERx[max_sprites];

extern FILE* tmapsFile_DB73C;
extern FILE* tmaps0File_DB740;
extern FILE* tmaps1File_DB744;
extern FILE* tmaps2File_DB748;

extern Type_Sprite** m_ptrLoadedSprites_F66F0x[504];
extern char m_LevelSpriteList_F5340[504];
extern int32_t tmapGroupLastUse_F5730[504];
extern subtype_x_DWORD_E9C28_str* str_F5F10[504];

extern type_x_DWORD_E9C28_str* tmapCache_E9C28;

extern type_E9C08* animations_E9C08x; // weak
extern bool big_sprites_inited;

extern uint8_t* m_pColorPalette;

struct FlcState {
	int  width = 0;
	int  height = 0;
	bool skipZero = false;
};

static uint16_t FlcRd16(uint8_t* p);

bool MainInitTmaps_71520(unsigned __int16 a1);
int GetTmapGroupSize_70EF0(unsigned __int16 a1);
signed int GetIndex_71CD0(type_x_DWORD_E9C28_str* a1);
int GetTmapCacheFreeSize_71E60(type_x_DWORD_E9C28_str* a1y);
unsigned int EvictLeastUsedTmaps_71090(unsigned int a1);
char UnloadTmapGroup_70E10(unsigned __int16 a1);
char ResetTmap_70D20(uint16 tmapsIndex);
type_animations1* GetAnimationByIndex_724F0(type_E9C08* a1x, __int16 a2);
void FreeTmapCacheEntry_71F20(type_x_DWORD_E9C28_str* a1y, subtype_x_DWORD_E9C28_str* a2x);
void InitTmaps(unsigned __int16 a1);
subtype_x_DWORD_E9C28_str* LoadTMapMetadata_71E70(type_x_DWORD_E9C28_str* a1y, unsigned int a2, __int16 a3);
void OpenTmaps_70A60();
void CloseTmaps_70BF0();
int DecompressTmap_70C60(uint16_t texture_index, uint8_t* texture_buffer);
bool WriteTmapsMeta(const std::string& dataDir, const std::string& outPath);//TMAPSMETA.DAT for the tests without the game
extern MapType_t tmapsMapType;//the TMAPS file tmapsFile_DB73C stands for (sub_70A60 Day, sub_71A70)
void WriteTextureMapToBmp(uint16_t textureIndex, uint8_t* ptextureMap, uint16_t width, uint16_t height, MapType_t mapType);
void WriteTextureMapToBmp(uint16_t textureIndex, uint16_t frameIndex, uint8_t* ptextureMap, uint16_t width, uint16_t height, MapType_t mapType);
uint8_t* LoadTMapColorPalette(MapType_t mapType);
type_animations1* InitTmapAnimation_721C0(type_E9C08* a1, Type_Sprite** a2x, int16_t index);
void ResetAnimation_72410(type_animations1* animation);

std::vector<uint8_t*> GetRawFrames(uint16_t frameCount, Type_Sprite* ptextureMap, size_t startOffset = 0);
uint8_t* ApplyDeltaFlc(uint8_t* p, uint8_t* img, const FlcState& st);
void ApplyByteRun(uint8_t* p, uint8_t* end, uint8_t* img, const FlcState& st);
uint8_t* ApplyFlcFrame(uint8_t* src, uint8_t* img, FlcState& st);

void FreeTmapsAndAnimations_713A0();
void ProcessTmaps_71410();
void UpdateSpriteAnimations_715B0();
void ClearUsedTmapFlags_716A0();
void LoadEntitySprites_716C0(unsigned __int16 a1, unsigned __int16 a2, unsigned __int16 a3);
void MarkSpriteTmapUsed_71730(unsigned __int16 a1);
void ReloadSpritesForLevel_71780();
void MarkTmapGroupUsed_71AB0(__int16 a1, char a2);
void FreeTmapCache_720C0(type_x_DWORD_E9C28_str** a1);
void AnimateSprite_72350(type_animations1* a1);
void AnimateSpriteFrames_723B0(type_animations1* a1, char a2);
void FreeAnimations_72550(type_E9C08** a1);

#endif //TEXTURE_MAPS_H
