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
std::vector<uint8_t> SaveLevelToBuffer();
bool LoadLevelFromBuffer(const std::vector<uint8_t>& save);
void GenerateLevelFeatures_49270(Type_Level_2FECE* terrain);
static std::string MapTypeFolder(MapType_t mt);
static std::string DatBaseName(MapType_t mt);
static uint8_t NearestPaletteIndex(uint8_t r, uint8_t g, uint8_t b, const TColor* palette, int paletteSize);
static void EncodeRLE(const uint8_t* raw, const uint8_t* hasAlpha, int w, int h, std::vector<uint8_t>& out);
static bool PatchSprite(bitmap_pos_struct2_t* tabBase, uint8_t* datBase, size_t& datUsed, size_t datCapacity, int spriteIndex, const char* pngPath, 
	const TColor* palette, int paletteSize);
void LoadFixedMenuGraphics();
void LoadSpr_47160();
void LoadTextureData(__int16 vgaTypeResolution, MapType_t MapType, uint8_t* textureBuffer);
void ConvertObjectivePointers_55100(char a1);
void FixPointersAfterLoad_57680();
void CopyPlayerSpellData_549A0(type_str_611* a1, type_str_611* a2);
void RebuildEntityLists_49F90();
void CreatePlayerSpellEntities_55AB0(type_str_0x2BDE* playStr);
void ReloadLevelTmaps_71990();
void MarkLoadedTmaps_71930();
void SetStagetagForTermod_49830(Type_Level_2FECE* terrain);
void SetSpell_6D5E0(type_entity_0x6E8E* event, int spellId);
void InitTmapsAndSprites_712F0();
void SetTmapsFile_71A70(MapType_t a1);
int GetSpellManaCost_6D710(type_entity_0x6E8E* event, uint8 spellIndex, uint8 subSpellIndex);
void SetAllocationTag_83CC0(char a1);
type_x_DWORD_E9C28_str* InitTmapCache_71B40(int a1, unsigned __int16 a2, type_x_DWORD_E9C28_str* a3);
type_E9C08* AllocateAnimations_72120(unsigned __int16 a1);

extern POSITION textPosition_F4720;
void InitStageVars_11EE0();
void SetStageVarForEntity_12100(type_entity_0x30311* a1, type_entity_0x6E8E* a2, char a3);
void AdvanceEntityStage_122A0(type_entity_0x6E8E* event);
void MarkStageVarsForIndex_122C0(__int16 a1);
void AdvanceStage_12330(type_entity_0x6E8E* a1, __int16 a2);
void AdvanceStageOrSetAction_12410(type_entity_0x6E8E* event, char actionIndex);
void ClearEntityStage_12470(type_entity_0x6E8E* a1, char a2);
void UpdateEntityStage_12500(type_entity_0x6E8E* entity);
void UpdateStageVars_12780();
void CheckStageVarsComplete_12870();
void ResetLevelState_4A1E0(int a1, char a2);
void SpawnLevelEntity_4A310(type_entity_0x30311* a1);
type_entity_0x6E8E* ArriveCheckpoint_4EB50(axis_3d* a1);
type_entity_0x6E8E* AddSwitchXX_50A90(axis_3d* a1, char a2, char a3);
type_entity_0x6E8E* AddSwitch00_50AE0(axis_3d* a1);
type_entity_0x6E8E* AddSwitch01_50B00(axis_3d* a1);
type_entity_0x6E8E* AddSwitch02_50B20(axis_3d* a1);
type_entity_0x6E8E* AddSwitch03_50B40(axis_3d* a1);
type_entity_0x6E8E* AddSwitch04_50B60(axis_3d* a1);
type_entity_0x6E8E* AddSwitch32_50B80(axis_3d* a1);
type_entity_0x6E8E* AddSwitch12_50C10(axis_3d* a1);
type_entity_0x6E8E* AddSwitch13_50C30(axis_3d* a1);
type_entity_0x6E8E* AddSwitch14_50C50(axis_3d* a1);
type_entity_0x6E8E* AddSwitch15_50C70(axis_3d* a1);
type_entity_0x6E8E* AddSwitch16_50C90(axis_3d* a1);
type_entity_0x6E8E* AddSwitch17_50CB0(axis_3d* a1);
type_entity_0x6E8E* AddSwitch18_50CD0(axis_3d* a1);
type_entity_0x6E8E* AddSwitch19_50CF0(axis_3d* a1);
type_entity_0x6E8E* AddSwitch20_50D10(axis_3d* a1);
type_entity_0x6E8E* AddSwitch21_50D30(axis_3d* a1);
type_entity_0x6E8E* AddSwitch22_50D50(axis_3d* a1);
type_entity_0x6E8E* AddSwitch23_50D70(axis_3d* a1);
type_entity_0x6E8E* AddSwitch24_50D90(axis_3d* a1);
type_entity_0x6E8E* AddSwitch25_50DB0(axis_3d* a1);
type_entity_0x6E8E* AddSwitch26_50DD0(axis_3d* a1);
type_entity_0x6E8E* AddSwitch27_50DF0(axis_3d* a1);
type_entity_0x6E8E* AddSwitch28_50E10(axis_3d* a1);
type_entity_0x6E8E* AddSwitch29_50E30(axis_3d* a1);
type_entity_0x6E8E* AddSwitch33_50E50(axis_3d* a1);
type_entity_0x6E8E* AddSwitch34_50E70(axis_3d* a1);
type_entity_0x6E8E* AddSwitch35_50E90(axis_3d* a1);
type_entity_0x6E8E* AddSwitch36_50EB0(axis_3d* a1);
type_entity_0x6E8E* AddSwitch37_50ED0(axis_3d* a1);
type_entity_0x6E8E* AddSwitch38_50EF0(axis_3d* a1);
type_entity_0x6E8E* AddSwitch39_50F10(axis_3d* a1);
type_entity_0x6E8E* AddSwitch40_50F30(axis_3d* a1);
type_entity_0x6E8E* AddSwitch41_50F50(axis_3d* a1);
type_entity_0x6E8E* AddSwitch42_50F70(axis_3d* a1);
type_entity_0x6E8E* AddSwitch43_50F90(axis_3d* a1);
type_entity_0x6E8E* AddCheckpoint44_50FB0(axis_3d* a1);
type_entity_0x6E8E* AddSwitch30_50FD0(axis_3d* a1);
type_entity_0x6E8E* AddSwitch31atyp_50FF0(axis_3d* a1);
char LevelDecompress_533B0(int16 levelIndex, Type_Level_2FECE* levelData, std::string customLevelPath = "");
char TestOpenMovieGam_53770(uint16_t a1);
char TestOpenMovieMap_53950(uint16_t a1);
void LoadBldgPrm_539A0();
void RecordOrPlayMovieInput_53A40(Type_PlayerInput_0x6E3E* a1);
void StartMovieRecording_53C70();
void StopMovieRecording_53CA0();
void CloseMovie_53CC0();
uint8_t CreateNetherSubdir_53D10(const std::string& gameDir, const std::string& subDir);
char CreateUserDirectories_54200();
bool SaveSMAPSLEVmovie_54D30(__int16 a1);
bool SaveSMAPSLEVmovie2_54F00(__int16 a1);
void LoadLevel_56D60(unsigned int a1, char a2);
void CheckLevelEndReached_57570();
void CheckLevelEndFlags_575C0();
void AutoSaveLevel_57640();
void InitStages_58940(/*uint8_t* a1*/);
void LinkObjectiveToEntity_58DA0(type_entity_0x30311* a1, type_entity_0x6E8E* a2);
void GameObjectives_58F00();
void CheckObjectiveKill_59760(type_entity_0x6E8E* a1, type_entity_0x6E8E* a2);
void PresentObjective_59820();
void CollectLevelStats_5C530();
type_entity_0x6E8E* SwitchResetLevel_6F030(type_entity_0x6E8E* a1);
type_entity_0x6E8E* CheckpointArrived_6F070(type_entity_0x6E8E* a1);
type_entity_0x6E8E* InitSwitchChainZaxisAndSound_6F850(type_entity_0x6E8E* a1, int a2);
type_entity_0x6E8E* SwitchCheckPlayersNear_6F100(type_entity_0x6E8E* a1);
extern type_entity_0x6E8E* (*spellPickupCreators_2A881E[])(axis_3d*);
type_entity_0x6E8E* SwitchCheckLevelEnd_6F150(type_entity_0x6E8E* a1);
void SwitchCheckStage_6F1C0(type_entity_0x6E8E* a1);
type_entity_0x6E8E* FindPlayerTouchingSwitch_6F8E0(type_entity_0x6E8E* a1, int a2);
void SwitchCheckPlayersNear2_6F0B0(type_entity_0x6E8E* a1);
void ActivateSwitchChain_6F2B0(type_entity_0x6E8E* a1);
unsigned int CountCreaturesOfModel_6F300(type_entity_0x6E8E* a1, signed int a2);
unsigned int CountCreatures00_6F420(type_entity_0x6E8E* a1);
unsigned int CountCreatures01_6F440(type_entity_0x6E8E* a1);
unsigned int CountCreatures02_6F460(type_entity_0x6E8E* a1);
unsigned int CountCreatures03_6F480(type_entity_0x6E8E* a1);
unsigned int CountCreatures04_6F4A0(type_entity_0x6E8E* a1);
unsigned int CountCreatures05_6F4C0(type_entity_0x6E8E* a1);
unsigned int CountCreatures06_6F4E0(type_entity_0x6E8E* a1);
unsigned int CountCreatures07_6F500(type_entity_0x6E8E* a1);
unsigned int CountCreatures08_6F520(type_entity_0x6E8E* a1);
unsigned int CountCreatures09_6F540(type_entity_0x6E8E* a1);
unsigned int CountCreatures0A_6F560(type_entity_0x6E8E* a1);
unsigned int CountCreatures0B_6F580(type_entity_0x6E8E* a1);
unsigned int CountCreatures0C_6F5A0(type_entity_0x6E8E* a1);
unsigned int CountCreatures0D_6F5C0(type_entity_0x6E8E* a1);
unsigned int CountCreatures0E_6F5E0(type_entity_0x6E8E* a1);
unsigned int CountCreatures0F_6F600(type_entity_0x6E8E* a1);
unsigned int CountCreatures10_6F620(type_entity_0x6E8E* a1);
unsigned int CountCreatures11_6F640(type_entity_0x6E8E* a1);
unsigned int CountCreatures12_6F660(type_entity_0x6E8E* a1);
unsigned int CountCreatures13_6F680(type_entity_0x6E8E* a1);
unsigned int CountCreatures14_6F6A0(type_entity_0x6E8E* a1);
unsigned int CountCreatures15_6F6C0(type_entity_0x6E8E* a1);
unsigned int CountCreatures16_6F6E0(type_entity_0x6E8E* a1);
unsigned int CountCreatures17_6F700(type_entity_0x6E8E* a1);
unsigned int CountCreatures18_6F720(type_entity_0x6E8E* a1);
unsigned int CountCreatures19_6F740(type_entity_0x6E8E* a1);
unsigned int CountCreatures1A_6F760(type_entity_0x6E8E* a1);
unsigned int CountCreatures1B_6F780(type_entity_0x6E8E* a1);
unsigned int CountCreatures1C_6F7A0(type_entity_0x6E8E* a1);
unsigned int CountAllCreatures_6F7C0(type_entity_0x6E8E* a1);
void ActivateSwitchChainOnce_6F7E0(type_entity_0x6E8E* entity);
signed int AllCreaturesKilled_69FF0(type_entity_0x6E8E* a1);
