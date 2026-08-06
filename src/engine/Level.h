#pragma once

#include "global_types.h"
#include "engine_support.h"
#include "Basic.h"

#include "Animation.h"
#include "Basic.h"
#include "Events.h"
#include "GameUI.h"
#include "Graphics.h"
#include "MenusAndIntros.h"
#include "PlayerInput.h"
#include "Terrain.h"
#include "DatTabIndexes.h"

extern type_str_164 unk_F42B0x[1136];
extern type_str_160 str_D7BD6[157];
extern bool IsAfterLoad;
extern void LoadFixedFonts(int fontStructIndex, char* type);

bool SaveLevelSLEV_55250(uint8_t savefileindex, char* filenameindex);//SLEV1
bool SaveLevelSMAP_55320(uint8_t savefileindex, char* filenameindex);
bool SaveLevelSVER_55450(uint8_t savefileindex, int32_t a2, char* filenameindex);
bool LoadLevel_555D0(uint8_t fileindex, int levelindex, bool loadRegressionTest =false);
bool LoadLevelSMAP_558E0(uint8_t savefileindex, bool loadRegressionTest = false);
bool LoadLevelSLEV_55A10(uint8_t savefileindex, bool loadRegressionTest = false);
bool SaveLevel_55080(uint8_t savefileindex, int32_t LevelNumber, char* filenameindex);
void sub_49270_generate_level_features(Type_Level_2FECE* terrain);
static std::string MapTypeFolder(MapType_t mt);
static std::string DatBaseName(MapType_t mt);
static uint8_t NearestPaletteIndex(uint8_t r, uint8_t g, uint8_t b, const TColor* palette, int paletteSize);
static void EncodeRLE(const uint8_t* raw, const uint8_t* hasAlpha, int w, int h, std::vector<uint8_t>& out);
static bool PatchSprite(bitmap_pos_struct2_t* tabBase, uint8_t* datBase, size_t& datUsed, size_t datCapacity, int spriteIndex, const char* pngPath, 
	const TColor* palette, int paletteSize);
void LoadFixedMenuGraphics();
void LoadSpr_47160();
void LoadTextureData(__int16 vgaTypeResolution, MapType_t MapType, uint8_t* textureBuffer);
void sub_55100(char a1);
void sub_57680_FixPointersAfterLoad();
void sub_549A0(type_str_611* a1, type_str_611* a2);
void sub_49F90();
void sub_55AB0(type_str_0x2BDE* playStr);
void sub_71990();
void sub_71930();
void SetStagetagForTermod_49830(Type_Level_2FECE* terrain);
void SetSpell_6D5E0(type_entity_0x6E8E* event, int spellId);
void sub_712F0();
void sub_71A70_setTmaps(MapType_t a1);
int GetSpellManaCost_6D710(type_entity_0x6E8E* event, uint8 spellIndex, uint8 subSpellIndex);
void sub_83CC0(char a1);
type_x_DWORD_E9C28_str* sub_71B40(int a1, unsigned __int16 a2, type_x_DWORD_E9C28_str* a3);
type_E9C08* sub_72120(unsigned __int16 a1);
