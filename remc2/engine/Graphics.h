#pragma once

#include <cstdint>
#include "engine_support.h"
#include "ViewPort.h"
#include "GameRenderInterface.h"

extern GameRenderInterface* m_ptrGameRender;

extern uint8_t vgaBuffer_A0000[307200];
extern TColor tempPalette_EA3B8x[256];
extern int16_t videoModeNumber_180662;
extern bitmap_pos_struct2_t* webSpritesData_D4188;
extern bitmap_pos_struct2_t* webSpritesTab_D418C;
extern bitmap_pos_struct2_t* webSpritesTabEnd_D4190;

void SetViewport_A0D50(uint16_t posX, uint16_t posY, uint16_t width, uint16_t height);

extern char screenModeFlag_D478C;
extern char paletteFadeActive_E390C;
extern __int16 vgaPaletteBank_E3BA4;
extern char blendTable_F0220[];
extern char blendTable_F0320[];
extern char blendTable_F0420[];
extern char blendTable_F0520[];
extern char blendTable_F0620[];
extern char blendTable_F0720[];
extern char blendTable_F0820[];
extern char blendTable_F0920[];
extern TColor palette_F3FA0[];
extern int paletteComponent_F42A0;
extern TColor oldPalette_181544[];
extern TColor blackPalette_181844[];
extern uint8_t* vesaInfoBuffer_181C40;
int BlitToVgaPlanar_40D10();
void BlendAndBlit_40F80();
void VgaPaletteInstall_41A90(TColor* a1x);
void PaletteChanges_47760();
void LoadScreen_47FC0(bool isSecretLevel);
void PaletteFadeIn_480A0(/*int a1, int a2, int a3*/);
void SetLocalPaletteSubMode_5C800(type_entity_0x6E8E* a1, char a2);
void SetPaletteModification_5C830(type_entity_0x6E8E* event, char paletteSubMod, __int16 PaletteCount);
void ShakeScreenNearPlayer_5C870(type_entity_0x6E8E* a1);
void SetVgaPaletteEntry_727F0(unsigned __int8 a1, unsigned __int8 a2, unsigned __int8 a3, unsigned __int8 a4);
void ClearGraphicsBuffer_72883(void* ptrBuffer, uint16_t width, uint16_t height, uint8_t value);
void ClearScreenAlphaBuffer(uint16_t width, uint16_t height, uint8_t value);
void FadeClearBlit_7B5D0();
void CopyScreen_85B20(uint8_t* src, uint8_t* dest, uint16_t rows_count);
uint32_t FadePalettes_86EA0(/*int a1, int a2, int a3*/uint32_t interval);
int SetViewportBounds_90374(uint16_t viewPortX, uint16_t viewPortY, uint16_t viewPortWidth, uint16_t viewPortHeight, unsigned __int16 a5, __int16 a6);
char SetGammaCorrection_904C0(float a1);
int16_t VgaPalFadeInFadeOut_90B27(TColor* a1x, uint8_t shadow_levels, bool a3, int32_t frameDelay = 10);
void ResetPaletteFade_90D27();
void VgaSetVideoMode320x200AndPalette_90D6E(TColor* Palette);
void VgaSetVideoMode640x480AndPalette_90E07(TColor* Palette);
int SetVideoMode_9937E(__int16 a1);
signed int SetVgaPaletteBank_9951B(__int16 a1);
int TestVgaDriver_995B0();
void CopyScreen(void* source, void* desc, unsigned __int16 width, unsigned __int16 height);
int SetVgaRegister_A0B24(int a1);
int ReadVgaCrtcRegisters_A0BB0(int* a1, int a2);
void VgaGetPalette_A0D2C(TColor* a1);
void BlitScreenToVgaBanks_BD2CB(uint8_t* a1);
int BlitToVgaPlanar2_BD320(int result, int8_t* a2, int8_t* a3, int a4, int a5, uint8_t* a6);
void BlitScreenToVga_BD3DD();
