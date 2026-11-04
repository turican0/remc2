#pragma once

#ifndef TEXTURE_MAPS_H
#define TEXTURE_MAPS_H

#include "Type_Animations.h"
#include "Type_BigSpritesBuffer.h"
#include "Basic.h"

const int max_sprites = 600;

extern type_BIG_SPRITES_BUFFER BIG_SPRITES_BUFFERx[max_sprites];

extern FILE* x_DWORD_DB73C_tmapsfile;
extern FILE* x_DWORD_DB740_tmaps00file;
extern FILE* x_DWORD_DB744_tmaps10file;
extern FILE* x_DWORD_DB748_tmaps20file;

extern Type_Sprite** m_ptrLoadedSprites_F66F0x[504];
extern char m_LevelSpriteList_F5340[504];
extern int32_t x_DWORD_F5730[504];
extern subtype_x_DWORD_E9C28_str* str_F5F10[504];

extern type_x_DWORD_E9C28_str* x_DWORD_E9C28_str;

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
int sub_70EF0(unsigned __int16 a1);
signed int GetIndex_71CD0(type_x_DWORD_E9C28_str* a1);
int sub_71E60(type_x_DWORD_E9C28_str* a1y);
unsigned int sub_71090(unsigned int a1);
char sub_70E10(unsigned __int16 a1);
char ResetTmap_70D20(uint16 tmapsIndex);
type_animations1* GetAnimationByIndex_724F0(type_E9C08* a1x, __int16 a2);
void sub_71F20(type_x_DWORD_E9C28_str* a1y, subtype_x_DWORD_E9C28_str* a2x);
void InitTmaps(unsigned __int16 a1);
subtype_x_DWORD_E9C28_str* LoadTMapMetadata_71E70(type_x_DWORD_E9C28_str* a1y, unsigned int a2, __int16 a3);
void sub_70A60_open_tmaps();
void sub_70BF0_close_tmaps();
int sub_70C60_decompress_tmap(uint16_t texture_index, uint8_t* texture_buffer);
bool WriteTmapsMeta(const std::string& dataDir, const std::string& outPath);//TMAPSMETA.DAT for the tests without the game
extern MapType_t tmapsMapType;//the TMAPS file x_DWORD_DB73C_tmapsfile stands for (sub_70A60 Day, sub_71A70)
void WriteTextureMapToBmp(uint16_t textureIndex, uint8_t* ptextureMap, uint16_t width, uint16_t height, MapType_t mapType);
void WriteTextureMapToBmp(uint16_t textureIndex, uint16_t frameIndex, uint8_t* ptextureMap, uint16_t width, uint16_t height, MapType_t mapType);
uint8_t* LoadTMapColorPalette(MapType_t mapType);
type_animations1* sub_721C0_initTmap(type_E9C08* a1, Type_Sprite** a2x, int16_t index);
void ResetAnimation_72410(type_animations1* animation);

std::vector<uint8_t*> GetRawFrames(uint16_t frameCount, Type_Sprite* ptextureMap, size_t startOffset = 0);
uint8_t* ApplyDeltaFlc(uint8_t* p, uint8_t* img, const FlcState& st);
void ApplyByteRun(uint8_t* p, uint8_t* end, uint8_t* img, const FlcState& st);
uint8_t* ApplyFlcFrame(uint8_t* src, uint8_t* img, FlcState& st);

#endif //TEXTURE_MAPS_H
