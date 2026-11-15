#include "Graphics.h"

#include "Basic.h"
#include "EventsFunctions.h"
#include "Vr.h"
#include "CommandLineParser.h"
#include "GameUI.h"
#include "MenusAndIntros.h"
#include "Network.h"
#include "Player.h"
#include "PlayerInput.h"
#include "Events.h"
#include "Level.h"
#include "../utilities/StateMonitor.h"
#include "../utilities/RendererTests.h"
#include "ConvertMapInfo.h"
#include "LevelInit.h"
#include "DatTabIndexes.h"
#include "MouseOnOff.h"
#include "PrintMessages.h"
#include "ReadAndDecompress.h"
#include "Type_DB080_CdTrack.h"


char screenModeFlag_D478C = 0; // weak

//__int16 drawFlags_E36D4 = 0; // weak

char paletteFadeActive_E390C = 0; // weak

__int16 vgaPaletteBank_E3BA4 = 0; // weak

char blendTable_F0220[256]; // idb

char blendTable_F0320[256]; // idb

char blendTable_F0420[256]; // idb

char blendTable_F0520[256]; // idb

char blendTable_F0620[256]; // idb

char blendTable_F0720[256]; // idb

char blendTable_F0820[256]; // idb

char blendTable_F0920[256]; // idb

TColor palette_F3FA0[256]; // fix it -  weak

int paletteComponent_F42A0; // weak

char gammaTable_181504[64]; // idb

TColor oldPalette_181544[256]; // idb

uint8_t* vesaInfoBuffer_181C40; // weak

GameRenderInterface* m_ptrGameRender;

uint8_t vgaBuffer_A0000[307200];
TColor tempPalette_EA3B8x[256]; // weak?langStrings_E9C4C[475]
int16_t videoModeNumber_180662; // weak
bitmap_pos_struct2_t* webSpritesData_D4188 = 0; // weak
bitmap_pos_struct2_t* webSpritesTab_D418C = 0; // weak
bitmap_pos_struct2_t* webSpritesTabEnd_D4190 = 0; // weak


//----- (000A0D50) --------------------------------------------------------
void SetViewport_A0D50(uint16_t posX, uint16_t posY, uint16_t width, uint16_t height)//281d50
{
	viewportX_18063C = posX;
	viewportY_180650 = posY;
	clipWidth_180648 = width;
	clipHeight_180644 = height;
	viewportWidth_180634 = width + posX;
	viewportHeight_180630 = height + posY;
}


/*
int8_t* x_DWORD_D9F50ar_ret(uint32_t adress) {
	if (adress >= 0x4400) return (int8_t*)&x_WORD_DE350[(adress - 0x4400)/2];
	if (adress >= 0x4000) return (int8_t*)& textureAddresses_DDF50[(adress - 0x4000)/4];
	//if (adress >= 0x2000) return (int8_t*)&off_DBF50[(adress - 0x2000)/4];
	if (adress >= 0x1800) return (int8_t*)&Maths::x_DWORD_DB750[(adress - 0x1800)/4];
	//if (adress >= 0x1400) return &x_DWORD_D9F50ar[0x800+0x800+(adress - 0x1400)];
	//if (adress >= 0x1000) return &x_DWORD_D9F50ar[0x800+0x800 + (adress - 0x1000)];
	//if (adress >= 0x800) return &x_DWORD_D9F50ar[0x800+(adress - 0x800)];
	return &x_DWORD_D9F50ar[adress];
}*/
/*
int8_t* x_DWORD_DA750ar_ret(uint32_t adress) {
	if (adress >= 0x3c00) return (int8_t*)&x_WORD_DE350[(adress - 0x3c00)/2];
	if (adress >= 0x3800) return (int8_t*)&textureAddresses_DDF50[(adress - 0x3800)/4];
	//if (adress >= 0x1800) return (int8_t*)&off_DBF50[(adress - 0x1800)/4];
	if (adress >= 0x1000) return (int8_t*)&Maths::x_DWORD_DB750[(adress - 0x1000)/4];
	//if (adress >= 0xc00) return &x_DWORD_D9F50ar[0x800+0x800 + (adress - 0xc00)];
	//if (adress >= 0x800) return &x_DWORD_D9F50ar[0x800+0x800 + (adress - 0x800)];
	return &x_DWORD_D9F50ar[0x800+adress];
}
*/
/*int8_t* x_DWORD_DAF50ar_ret(uint32_t adress) {
	if (adress >= 0x3400) return (int8_t*)&x_WORD_DE350[(adress - 0x3400) / 2];
	if (adress >= 0x3000) return (int8_t*)&textureAddresses_DDF50[(adress - 0x3000) / 4];
	//if (adress >= 0x1000) return (int8_t*)&off_DBF50[(adress - 0x1000) / 4];
	if (adress >= 0x800) return (int8_t*)&Maths::x_DWORD_DB750[(adress - 0x800) / 4];
	//if (adress >= 0x400) return &x_DWORD_D9F50ar[0x800+0x800+(adress - 0x400)];
	return &x_DWORD_D9F50ar[0x800+0x800+adress];
}*/
/*
int8_t* x_DWORD_DB750ar_ret(uint32_t adress) {
	if (adress >= 0x2c00) return (int8_t*)&x_WORD_DE350[(adress - 0x2c00) / 2];
	if (adress >= 0x2800) return (int8_t*)&textureAddresses_DDF50[(adress - 0x2800) / 4];
	//if (adress >= 0x800) return (int8_t*)&off_DBF50[(adress - 0x800) / 4];
	return (int8_t*)&Maths::x_DWORD_DB750[adress/4];
}*/
/*
int8_t* x_DWORD_DBF50ar_ret(uint32_t adress) {
	if (adress >= 0x2400) return (int8_t*)&x_WORD_DE350[(adress - 0x2400) / 2];
	if (adress >= 0x2000) return (int8_t*)&x_DWORD_DDF50[(adress - 0x2000) / 4];
	return (int8_t*)&off_DBF50[adress/4];
}*/

//----- (0003B560) --------------------------------------------------------
// 10000: using guessed type void /*__noreturn*/ sub_10000();
// D9F50: using guessed type int x_DWORD_D9F50[];
// DA750: using guessed type int x_DWORD_DA750[];
// DBF50: using guessed type void (/*__noreturn*/ *off_DBF50[2])();
// DE560: using guessed type int iScreenWidth;
// DE564: using guessed type int iViewPortWidth;
// DE568: using guessed type int iViewPortHeight;
// E9C38: using guessed type int smalltitBuffer_E9C38;
// F2C84: using guessed type int spriteRender_F2C20.dword0x19;
// F2C88: using guessed type int spriteRender_F2C20.dword0x1a;
// F2C8C: using guessed type int spriteRender_F2C20.dword0x1b;
// F2C90: using guessed type int spriteRender_F2C20.dword0x1c;
// F2C94: using guessed type int spriteRender_F2C20.dword0x1d;
// F2C98: using guessed type int spriteRender_F2C20.dword0x1e;
// F2C9C: using guessed type int spriteRender_F2C20.dword0x1f;
// F2CA4: using guessed type int spriteRender_F2C20.dword0x21;
// F2CAC: using guessed type int spriteRender_F2C20.dword0x23;
// F2CB4: using guessed type int spriteRender_F2C20.dword0x25;
// F2CB8: using guessed type int spriteRender_F2C20.dword0x26;
// F2CBC: using guessed type int spriteRender_F2C20.dword0x27;

/*
8 x_DWORD_D9F50ar[0x800] 800 * 0
8 x_DWORD_DA750ar[0x800] 800 * +800
8 x_DWORD_DAF50ar[0x800] 800   +1000

32 x_DWORD_DB350[103] +1400

32 Maths::x_DWORD_DB750[0x200] 800 +1800

32 off_DBF50[4096] 4000+ 2000

uint8_t* x_DWORD_DDF50[256] + 4000

uint16_t x_WORD_DE350[256] + 4400
*/

int BlitToVgaPlanar_40D10()//221d10//fix vga
{
	if (CommandLineParams.DoShowDebugPerifery())ShowPerifery();

	int v1; // ebx
	signed int v2; // ecx
	x_BYTE* v3; // esi
	int* v4; // edi
	signed int v5; // ecx
	x_BYTE* v6; // esi
	int* v7; // edi
	signed int v8; // ecx
	int* v9; // edi
	x_BYTE* v10; // esi
	signed int v11; // ecx
	int* v12; // edi
	x_BYTE* v13; // esi
	signed int v14; // ecx
	int* v15; // edi
	x_BYTE* v16; // esi
	signed int v17; // ecx
	int* v18; // edi
	x_BYTE* v19; // esi
	signed int v20; // ecx
	int* v21; // edi
	x_BYTE* v22; // esi
	signed int v23; // ecx
	int* v24; // edi
	x_BYTE* v25; // esi
	int result; // eax
	signed int v27; // [esp+4h] [ebp-10h]
	uint8_t* v28; // [esp+8h] [ebp-Ch]
	uint8_t* v29; // [esp+Ch] [ebp-8h]
	int* v30; // [esp+10h] [ebp-4h]

	int a1;

	//__outx_WORD(0x3CEu, 0xFF08u);
	HIWORD(v1) = 0;
	v30 = (int*)&vgaBuffer_A0000;
	v28 = pdwScreenBuffer_351628;
	v27 = 200;
	v29 = ptrMemoryBuffer_E9C3C;
	while (v27)
	{
		v2 = 10;
		v3 = (x_BYTE*)v28;
		v4 = v30;
		//__outx_WORD(0x3C4u, 0x102u);
		do
		{
			LOBYTE(a1) = v3[8];
			LOBYTE(v1) = v3[24];
			BYTE1(a1) = v3[12];
			BYTE1(v1) = v3[28];
			a1 = __SWAP_HILOWORD__(a1);
			v1 = __SWAP_HILOWORD__(v1);
			LOBYTE(a1) = *v3;
			LOBYTE(v1) = v3[16];
			BYTE1(a1) = v3[4];
			BYTE1(v1) = v3[20];
			*v4 = a1;
			v4[1] = v1;
			v3 += 32;
			v4 += 2;
			v2--;
		} while (v2);
		v5 = 10;
		v6 = (x_BYTE*)v29;
		v7 = v30 + 20;
		do
		{
			LOBYTE(a1) = v6[8];
			LOBYTE(v1) = v6[24];
			BYTE1(a1) = v6[12];
			BYTE1(v1) = v6[28];
			a1 = __SWAP_HILOWORD__(a1);
			v1 = __SWAP_HILOWORD__(v1);
			LOBYTE(a1) = *v6;
			LOBYTE(v1) = v6[16];
			BYTE1(a1) = v6[4];
			BYTE1(v1) = v6[20];
			*v7 = a1;
			v7[1] = v1;
			v6 += 32;
			v7 += 2;
			v5--;
		} while (v5);
		v8 = 10;
		v9 = v30;
		v10 = (x_BYTE*)(v28 + 1);
		//__outx_WORD(0x3C4u, 0x202u);
		do
		{
			LOBYTE(a1) = v10[8];
			LOBYTE(v1) = v10[24];
			BYTE1(a1) = v10[12];
			BYTE1(v1) = v10[28];
			a1 = __SWAP_HILOWORD__(a1);
			v1 = __SWAP_HILOWORD__(v1);
			LOBYTE(a1) = *v10;
			LOBYTE(v1) = v10[16];
			BYTE1(a1) = v10[4];
			BYTE1(v1) = v10[20];
			*v9 = a1;
			v9[1] = v1;
			v10 += 32;
			v9 += 2;
			v8--;
		} while (v8);
		v11 = 10;
		v12 = v30 + 20;
		v13 = (x_BYTE*)(v29 + 1);
		do
		{
			LOBYTE(a1) = v13[8];
			LOBYTE(v1) = v13[24];
			BYTE1(a1) = v13[12];
			BYTE1(v1) = v13[28];
			a1 = __SWAP_HILOWORD__(a1);
			v1 = __SWAP_HILOWORD__(v1);
			LOBYTE(a1) = *v13;
			LOBYTE(v1) = v13[16];
			BYTE1(a1) = v13[4];
			BYTE1(v1) = v13[20];
			*v12 = a1;
			v12[1] = v1;
			v13 += 32;
			v12 += 2;
			--v11;
		} while (v11);
		v14 = 10;
		v15 = v30;
		v16 = (x_BYTE*)(v28 + 2);
		//__outx_WORD(0x3C4u, 0x402u);
		do
		{
			LOBYTE(a1) = v16[8];
			LOBYTE(v1) = v16[24];
			BYTE1(a1) = v16[12];
			BYTE1(v1) = v16[28];
			a1 = __SWAP_HILOWORD__(a1);
			v1 = __SWAP_HILOWORD__(v1);
			LOBYTE(a1) = *v16;
			LOBYTE(v1) = v16[16];
			BYTE1(a1) = v16[4];
			BYTE1(v1) = v16[20];
			*v15 = a1;
			v15[1] = v1;
			v16 += 32;
			v15 += 2;
			v14--;
		} while (v14);
		v17 = 10;
		v18 = v30 + 20;
		v19 = (x_BYTE*)(v29 + 2);
		do
		{
			LOBYTE(a1) = v19[8];
			LOBYTE(v1) = v19[24];
			BYTE1(a1) = v19[12];
			BYTE1(v1) = v19[28];
			a1 = __SWAP_HILOWORD__(a1);
			v1 = __SWAP_HILOWORD__(v1);
			LOBYTE(a1) = *v19;
			LOBYTE(v1) = v19[16];
			BYTE1(a1) = v19[4];
			BYTE1(v1) = v19[20];
			*v18 = a1;
			v18[1] = v1;
			v19 += 32;
			v18 += 2;
			v17--;
		} while (v17);
		v20 = 10;
		v21 = v30;
		v22 = (x_BYTE*)(v28 + 3);
		//__outx_WORD(0x3C4u, 0x802u);
		do
		{
			LOBYTE(a1) = v22[8];
			LOBYTE(v1) = v22[24];
			BYTE1(a1) = v22[12];
			BYTE1(v1) = v22[28];
			a1 = __SWAP_HILOWORD__(a1);
			v1 = __SWAP_HILOWORD__(v1);
			LOBYTE(a1) = *v22;
			LOBYTE(v1) = v22[16];
			BYTE1(a1) = v22[4];
			BYTE1(v1) = v22[20];
			*v21 = a1;
			v21[1] = v1;
			v22 += 32;
			v21 += 2;
			v20--;
		} while (v20);
		v23 = 10;
		v24 = v30 + 20;
		v25 = (x_BYTE*)(v29 + 3);
		do
		{
			LOBYTE(a1) = v25[8];
			LOBYTE(v1) = v25[24];
			BYTE1(a1) = v25[12];
			BYTE1(v1) = v25[28];
			a1 = __SWAP_HILOWORD__(a1);
			v1 = __SWAP_HILOWORD__(v1);
			LOBYTE(a1) = *v25;
			LOBYTE(v1) = v25[16];
			BYTE1(a1) = v25[4];
			BYTE1(v1) = v25[20];
			*v24 = a1;
			v24[1] = v1;
			v25 += 32;
			v24 += 2;
			v23--;
		} while (v23);
		a1 = v27 - 1;
		v28 += 320;
		v29 += 320;
		v30 += 40;
		v27--;
	}
	result = 0;
	memset((void*)pdwScreenBuffer_351628, 0, 0xFA00u);
	return result;
}
// E9C3C: using guessed type int ptrMemoryBuffer_E9C3C;
// 180628: using guessed type int pdwScreenBuffer_351628;

//----- (00040F80) --------------------------------------------------------
void BlendAndBlit_40F80()//221f80
{
	// ── Half-size blending
	if (world_D41A0.m_GameSettings.m_Display.m_uiScreenSize == 2 && !screenModeFlag_D478C) {
		const int stride = (uint16_t)iScreenWidth_DE560;
		const int width_dwords = (uint16_t)viewPort.Width_DE564 >> 2;
		const int half_height = (uint16_t)viewPort.Height_DE568 / 2;
		uint8_t* scan = ptrMemoryBuffer_E9C3C;
		uint8_t* vp = ViewPortRenderBufferStart_DE558;
		for (int row = half_height; row; row--) {
			uint8_t* s = scan;
			uint8_t* d = vp;
			for (int col = width_dwords; col; col--, s += 4, d += 4) {
				*(uint32_t*)d =
					((uint32_t)(blendTable_F0520[d[2]] + blendTable_F0620[s[2]])) |
					((uint32_t)(blendTable_F0920[d[3]] + blendTable_F0220[s[3]]) << 8) |
					((uint32_t)(blendTable_F0520[d[0]] + blendTable_F0620[s[0]]) << 16) |
					((uint32_t)(blendTable_F0920[d[1]] + blendTable_F0220[s[1]]) << 24);
			}
			s = scan + stride;
			d = vp + stride;
			for (int col = width_dwords; col; col--, s += 4, d += 4) {
				*(uint32_t*)d =
					((uint32_t)(blendTable_F0820[d[2]] + blendTable_F0320[s[2]])) |
					((uint32_t)(blendTable_F0720[d[3]] + blendTable_F0420[s[3]]) << 8) |
					((uint32_t)(blendTable_F0820[d[0]] + blendTable_F0320[s[0]]) << 16) |
					((uint32_t)(blendTable_F0720[d[1]] + blendTable_F0420[s[1]]) << 24);
			}
			scan += 2 * stride;
			vp += 2 * stride;
		}
	}
	if (world_D41A0.m_GameSettings.m_Display.m_uiScreenSize == 1)
		BlitToVgaPlanar_40D10();
	else if (videoMode_180660 & 1)
	{
		if (screenModeFlag_D478C)
			BlitScreenToVgaBanks_BD2CB(unk_F0A20x);//maybe for virtual head set
		else if ((!DefaultResolutions()) && (videoMode_180660 != 1))
			VGA_BlitAny(maxGameFps);
		else if (videoMode_180660 & 1)
			VgaBlit320_90478(maxGameFps);
		else
			VgaBlit640_75200(480, maxGameFps);
	}
	else if (world_D41A0.m_GameSettings.graphics_0x2192.xxxx_0x2193 && world_D41A0.m_GameSettings.m_Display.m_uiScreenSize)
		BlitScreenToVga_BD3DD();
	else if (screenModeFlag_D478C)
		DrawVrFrame_BD1B6(unk_F0A20x);
	else if ((!DefaultResolutions()) && (videoMode_180660 != 1))
		VGA_BlitAny(maxGameFps);
	else if (videoMode_180660 & 1)
		VgaBlit320_90478(maxGameFps);
	else
		VgaBlit640_75200(480, maxGameFps);
}

void VgaPaletteInstall_41A90(TColor* bufferx)//222a90
{
	if (CommandLineParams.DoShowDebugPerifery())
		ShowPerifery();

	//354f24 - 000000 00002a 002a00 002a2a 2a0000
	//debug
	//loadfromsnapshot((char*)"0160-00222A90-x", a1, 0x1a7358, 0x300);//4c
	//loadfromsnapshot((char*)"0160-00222A90-x", a1, 0x3aa0a4, 0x300);//4c
	//debug

	EventDispatcher::I->DispatchEvent<ResourceType, uint8_t*>(EventType::E_RESOURCE_CHANGE, ResourceType::PALETTE_SET, (uint8_t*)bufferx);
	EventDispatcher::I->DispatchEvent<ResourceType, uint8_t*>(EventType::E_RESOURCE_CHANGE, ResourceType::LOOKUPTABLE_SET, ColourLookupTable_F6EE0);

	VGA_Set_Palette((uint8_t*)bufferx);
}

//----- (00047760) --------------------------------------------------------
void PaletteChanges_47760()//228760
{
	char dataPath[MAX_PATH];
	TColor** DefaultPal = (TColor**)xadatapald0dat2.colorPalette_var28;
	gameState_D41A4.moveSpeedFlag_181 = 0;
	switch (gameState_D41A4.paletteMod_51)
	{
	case 0:
	case 1: //Fade out loading screen
	{
		PaletteFadeIn_480A0();
		gameState_D41A4.paletteMod_51++;
		break;
	}
	case 2:
	{
		gameState_D41A4.paletteMod_51++;
		gameState_D41A4.paletteSubMod_180 = 1;
		memset((void*)*DefaultPal, 0, 768);
		VgaPaletteInstall_41A90(*DefaultPal);
		switch (world_D41A0.terrain_2FECE.MapType)
		{
		case MapType_t::Day:
		{
			sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/PALD-0.DAT");
			DataFileIO::ReadFileAndDecompress(dataPath, xadatapald0dat2.colorPalette_var28);
			sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/CLRD-0.DAT");
			DataFileIO::ReadFileAndDecompress(dataPath, xadataclrd0dat.colorPalette_var28);
		}
		break;
		case MapType_t::Night:
		{
			if (world_D41A0.terrain_2FECE.levelType_0x2FED2 & 2)
			{
				sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/PALF-0.DAT");
				DataFileIO::ReadFileAndDecompress(dataPath, xadatapald0dat2.colorPalette_var28);
			}
			else
			{
				sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/PALN-0.DAT");
				DataFileIO::ReadFileAndDecompress(dataPath, xadatapald0dat2.colorPalette_var28);
			}
			sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/CLRN-0.DAT");
			DataFileIO::ReadFileAndDecompress(dataPath, xadataclrd0dat.colorPalette_var28);
			break;
		}
		case MapType_t::Cave:
		{
			sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/PALC-0.DAT");
			DataFileIO::ReadFileAndDecompress(dataPath, xadatapald0dat2.colorPalette_var28);
			sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/CLRC-0.DAT");
			DataFileIO::ReadFileAndDecompress(dataPath, xadataclrd0dat.colorPalette_var28);
			break;
		}
		}
		qmemcpy((void*)tempPalette_EA3B8x, (void*)*DefaultPal, 0x300u);
		ApplyBrightnessToPalette_47650(0x300);
		ResetPaletteFade_90D27();
		uiBackGroundColorIdx_EB3A8 = (*DefaultPal)[0].red;
		AutoSaveLevel_57640();
		break;
	}
	case 3:
	{
		switch (gameState_D41A4.paletteSubMod_180)
		{
		case 1:
			//Fade back
			gameState_D41A4.moveSpeedFlag_181 = 1;
			if (VgaPalFadeInFadeOut_90B27(*DefaultPal, 4u, 1, 0) == 4)
			{
				gameState_D41A4.paletteSubMod_180 = 0;
			}
			break;
		case 2:
			//Hit (red flash)
			for (int i = 1; i < 256; i++)
			{
				paletteComponent_F42A0 = (*DefaultPal)[i].red + 40;
				if (paletteComponent_F42A0 < 0)
					paletteComponent_F42A0 = 0;
				if (paletteComponent_F42A0 > 63)
					paletteComponent_F42A0 = 63;
				palette_F3FA0[i].red = paletteComponent_F42A0;
				paletteComponent_F42A0 = (*DefaultPal)[i].green;
				if (paletteComponent_F42A0 < 0)
					paletteComponent_F42A0 = 0;
				if (paletteComponent_F42A0 > 63)
					paletteComponent_F42A0 = 63;
				palette_F3FA0[i].green = paletteComponent_F42A0;
				paletteComponent_F42A0 = (*DefaultPal)[i].blue;
				if (paletteComponent_F42A0 < 0)
					paletteComponent_F42A0 = 0;
				if (paletteComponent_F42A0 > 63)
					paletteComponent_F42A0 = 63;
				palette_F3FA0[i].blue = paletteComponent_F42A0;
			}
			ResetPaletteFade_90D27();
			VgaPaletteInstall_41A90(palette_F3FA0);
			gameState_D41A4.paletteSubMod_180 = 1;
			gameState_D41A4.moveSpeedFlag_181 = 1;
			break;
		case 3:
			for (int i = 1; i < 256; i++)
			{
				paletteComponent_F42A0 = (*DefaultPal)[i].red;
				paletteComponent_F42A0 += -56 * gameState_D41A4.paletteCount_184w >> 8;
				if (paletteComponent_F42A0 < 0)
					paletteComponent_F42A0 = 0;
				if (paletteComponent_F42A0 > 63)
					paletteComponent_F42A0 = 63;
				palette_F3FA0[i].red = paletteComponent_F42A0;
				paletteComponent_F42A0 = (*DefaultPal)[i].green;
				if (paletteComponent_F42A0 < 0)
					paletteComponent_F42A0 = 0;
				if (paletteComponent_F42A0 > 63)
					paletteComponent_F42A0 = 63;
				palette_F3FA0[i].green = paletteComponent_F42A0;
				paletteComponent_F42A0 = (*DefaultPal)[i].blue;
				paletteComponent_F42A0 += -56 * gameState_D41A4.paletteCount_184w >> 8;
				if (paletteComponent_F42A0 < 0)
					paletteComponent_F42A0 = 0;
				if (paletteComponent_F42A0 > 63)
					paletteComponent_F42A0 = 63;
				palette_F3FA0[i].blue = paletteComponent_F42A0;
			}
			ResetPaletteFade_90D27();
			VgaPaletteInstall_41A90(palette_F3FA0);
			gameState_D41A4.paletteSubMod_180 = 1;
			gameState_D41A4.moveSpeedFlag_181 = 1;
			break;
		case 4:
			for (int i = 1; i < 256; i++)
			{
				paletteComponent_F42A0 = 255;
				palette_F3FA0[i].red = (*DefaultPal)[i].red;
				palette_F3FA0[i].green = (*DefaultPal)[i].green;
				palette_F3FA0[i].blue = 63;
			}
			ResetPaletteFade_90D27();
			VgaPaletteInstall_41A90(palette_F3FA0);
			gameState_D41A4.paletteSubMod_180 = 1;
			gameState_D41A4.moveSpeedFlag_181 = 1;
			break;
		case 5:
			memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 768);
			gameState_D41A4.paletteSubMod_180 = 10;
			gameState_D41A4.moveSpeedFlag_181 = 1;
			break;
		case 6:
			for (int i = 1; i < 256; i++)
			{
				paletteComponent_F42A0 = (*DefaultPal)[i].blue + 48;
				if (paletteComponent_F42A0 < 0)
					paletteComponent_F42A0 = 0;
				if (paletteComponent_F42A0 > 63)
					paletteComponent_F42A0 = 63;
				palette_F3FA0[i].red = paletteComponent_F42A0;
				paletteComponent_F42A0 = (*DefaultPal)[i].green + 32;
				if (paletteComponent_F42A0 < 0)
					paletteComponent_F42A0 = 0;
				if (paletteComponent_F42A0 > 63)
					paletteComponent_F42A0 = 63;
				palette_F3FA0[i].green = paletteComponent_F42A0;
				paletteComponent_F42A0 = (*DefaultPal)[i].blue + 32;
				if (paletteComponent_F42A0 < 0)
					paletteComponent_F42A0 = 0;
				if (paletteComponent_F42A0 > 63)
					paletteComponent_F42A0 = 63;
				palette_F3FA0[i].blue = paletteComponent_F42A0;
			}
			ResetPaletteFade_90D27();
			VgaPaletteInstall_41A90(palette_F3FA0);
			gameState_D41A4.paletteSubMod_180 = 1;
			gameState_D41A4.moveSpeedFlag_181 = 1;
			break;
		case 7:
			//Black and White
			for (int i = 1; i < 256; i++)
			{
				paletteComponent_F42A0 = ((*DefaultPal)[i].red + (*DefaultPal)[i].green + (*DefaultPal)[i].blue) / 3;
				palette_F3FA0[i].red = paletteComponent_F42A0;
				palette_F3FA0[i].green = paletteComponent_F42A0;
				palette_F3FA0[i].blue = paletteComponent_F42A0;
			}
			ResetPaletteFade_90D27();
			VgaPaletteInstall_41A90(palette_F3FA0);
			gameState_D41A4.paletteSubMod_180 = 1;
			gameState_D41A4.moveSpeedFlag_181 = 1;
			break;
		case 8:
			for (int i = 1; i < 256; i++)
			{
				paletteComponent_F42A0 = (*DefaultPal)[i].red + 48;
				if (paletteComponent_F42A0 < 0)
					paletteComponent_F42A0 = 0;
				if (paletteComponent_F42A0 > 63)
					paletteComponent_F42A0 = 63;
				palette_F3FA0[i].red = paletteComponent_F42A0;
				paletteComponent_F42A0 = (*DefaultPal)[i].green + 48;
				if (paletteComponent_F42A0 < 0)
					paletteComponent_F42A0 = 0;
				if (paletteComponent_F42A0 > 63)
					paletteComponent_F42A0 = 63;
				palette_F3FA0[i].green = paletteComponent_F42A0;
				paletteComponent_F42A0 = (*DefaultPal)[i].blue + 48;
				if (paletteComponent_F42A0 < 0)
					paletteComponent_F42A0 = 0;
				if (paletteComponent_F42A0 > 63)
					paletteComponent_F42A0 = 63;
				palette_F3FA0[i].blue = paletteComponent_F42A0;
			}
			ResetPaletteFade_90D27();
			VgaPaletteInstall_41A90(palette_F3FA0);
			gameState_D41A4.paletteSubMod_180 = 9;
			gameState_D41A4.moveSpeedFlag_181 = 1;
			break;
		case 9:
			gameState_D41A4.moveSpeedFlag_181 = 1;
			if (VgaPalFadeInFadeOut_90B27((TColor*)*xadatapald0dat2.colorPalette_var28, 0x10u, 1) == 16)
			{
				gameState_D41A4.paletteSubMod_180 = 0;
			}
			break;
		case 0xA:
			gameState_D41A4.moveSpeedFlag_181 = 1;
			if (VgaPalFadeInFadeOut_90B27((TColor*)*xadatapald0dat2.colorPalette_var28, 0x1Cu, 1) == 28)
			{
				gameState_D41A4.paletteSubMod_180 = 0;
			}
			break;
		default:
			return;
		}
		break;
	}
	}
}

//----- (00047FC0) --------------------------------------------------------
void LoadScreen_47FC0(bool isSecretLevel)//228fc0
{
	char dataPath[MAX_PATH];

	//int result; // eax

	VgaPalFadeInFadeOut_90B27(0, 0x10u, 0);

	//debug
	VgaPalFadeInFadeOut_90B27((TColor*)*xadatapald0dat2.colorPalette_var28, 0x20u, 0);
	//debug

	if (isSecretLevel)
	{
		sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/SMATITL2.DAT");
		DataFileIO::ReadFileAndDecompress(dataPath, &smalltitBuffer_E9C38);
	}
	else
	{
		sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/SMATITLE.DAT");
		DataFileIO::ReadFileAndDecompress(dataPath, &smalltitBuffer_E9C38);
	}
	CopyScreen_85B20(smalltitBuffer_E9C38, pdwScreenBuffer_351628, 400);
	if (videoMode_180660 & 1)
		VgaBlit320_90478();
	else
		VgaBlit640_75200(480);
	if (isSecretLevel)
	{
		sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/SMATITL2.PAL");
		DataFileIO::ReadFileAndDecompress(dataPath, xadatapald0dat2.colorPalette_var28);
	}
	else
	{
		sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/SMATITLE.PAL");
		DataFileIO::ReadFileAndDecompress(dataPath, xadatapald0dat2.colorPalette_var28);
	}
	VgaPalFadeInFadeOut_90B27((TColor*)*xadatapald0dat2.colorPalette_var28, 0x20u, 0);
	//result = (int)x_D41A0_BYTEARRAY_0;
	/*D41A0_BYTESTR_0.dword_0x23a = 1;//0x23a
	*(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 574) = 0;//0x23e
	*(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 578) = 0;//0x242*/
	world_D41A0.dword_0x23a = 1;
	world_D41A0.dword_0x23e = 0;
	world_D41A0.dword_0x242 = 0;
}

//----- (000480A0) --------------------------------------------------------
void PaletteFadeIn_480A0()//2290a0
{
	char dataPath[MAX_PATH];
	unsigned int timeDiff = 0;
	SetMusicVolume_98790(500, 0);
	long time = j___clock();
	do
		timeDiff = j___clock() - time;
	while (timeDiff < 50); //delay 50 mms
	VgaPalFadeInFadeOut_90B27(0, 0x10u, 0);
	world_D41A0.dword_0x23a = 0;
	sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/PALD-0.DAT");
	DataFileIO::ReadFileAndDecompress(dataPath, xadatapald0dat2.colorPalette_var28);
	sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/CLRD-0.DAT");
	DataFileIO::ReadFileAndDecompress(dataPath, xadataclrd0dat.colorPalette_var28);
	SetPlayerColoursForMapType_48120();
}

//----- (0005C800) --------------------------------------------------------
void SetLocalPaletteSubMode_5C800(type_entity_0x6E8E* a1x, char a2)//23d800
{
	//__int16 result; // ax

	//result = D41A0_BYTESTR_0.word_0xc;
	if (world_D41A0.LevelIndex_0xc == a1x->playerData_0xA4_164x->playerColorIndex_0x38_56)
	{
		//result = (short)x_D41A0_BYTEARRAY_4;
		gameState_D41A4.paletteSubMod_180 = a2;
	}
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;

//----- (0005C830) --------------------------------------------------------
void SetPaletteModification_5C830(type_entity_0x6E8E* event, char paletteSubMod, __int16 PaletteCount)//23d830
{
	if (world_D41A0.LevelIndex_0xc == event->playerData_0xA4_164x->playerColorIndex_0x38_56)
	{
		gameState_D41A4.paletteSubMod_180 = paletteSubMod;
		gameState_D41A4.paletteCount_184w = PaletteCount;
	}
}

//----- (0005C870) --------------------------------------------------------
void ShakeScreenNearPlayer_5C870(type_entity_0x6E8E* a1x)//23d870
{
	type_entity_0x6E8E* v1x; // ebx
	//type_str_164* v2x; // esi
	uint32_t index; // eax

	v1x = Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
	//v2x = v1x->playerData_0xA4_164x;
	index = Maths::Distance3D_583F0(&a1x->position_0x4C_76, &v1x->position_0x4C_76);
	if (index <= v1x->playerData_0xA4_164x->screenShake_0x19A_410)
		v1x->playerData_0xA4_164x->screenShake_0x19A_410 = index;
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000727F0) --------------------------------------------------------
void SetVgaPaletteEntry_727F0(unsigned __int8 a1, unsigned __int8 a2, unsigned __int8 a3, unsigned __int8 a4)//fix
{
	if (CommandLineParams.DoShowDebugPerifery())ShowPerifery();

	/*__outx_BYTE(0x3C8u, a1);
	__outx_BYTE(0x3C9u, a2);
	__outx_BYTE(0x3C9u, a3);
	__outx_BYTE(0x3C9u, a4);*/
}

void ClearGraphicsBuffer_72883(void* ptrScreenBuffer, uint16_t width, uint16_t height, uint8_t value)//2538a9
{
	memset32(ptrScreenBuffer, value, width * height);
}

void ClearScreenAlphaBuffer(uint16_t width, uint16_t height, uint8_t value)
{
	if (pdwScreenAlphaBuffer != nullptr)
		ClearGraphicsBuffer_72883((void*)pdwScreenAlphaBuffer, width, height, value);
}

//----- (0007B5D0) --------------------------------------------------------
void FadeClearBlit_7B5D0()
{
	VgaPalFadeInFadeOut_90B27(0, 0x10u, 0);
	memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 768);
	if (lowResScaledUi)
	{
		// the low res UI at another resolution
		ClearGraphicsBuffer_72883(pdwScreenBuffer_351628, screenWidth_18062C, screenHeight_180624, 0);
		ClearScreenAlphaBuffer(screenWidth_18062C, screenHeight_180624, 255);
	}
	else if (videoMode_180660 & 1)
	{
		ClearGraphicsBuffer_72883(pdwScreenBuffer_351628, 320, 200, 0);
		ClearScreenAlphaBuffer(320, 200, 255);
	}
	else
	{
		ClearGraphicsBuffer_72883(pdwScreenBuffer_351628, 640, 480, 0);
		ClearScreenAlphaBuffer(640, 480, 255);
	}
	if (videoMode_180660 & 1)
		VgaBlit320_90478(menuFps);
	else
		VgaBlit640_75200(480, menuFps);
}

//----- (00085B20) --------------------------------------------------------
void CopyScreen_85B20(uint8_t* src, uint8_t* dest, uint16_t rows_count) //266b20
{
	if (videoMode_180660 & 1) {
		CopyScreen(src, dest, 320, 200);//copy 320x200 to 320x200
	}
	else {
		uint16_t rows = rows_count / 2;
		for (int y = 0; y < rows; y++) {//copy 320x200 to 640x480
			for (int doubleRowIndex = 0; doubleRowIndex < 2; doubleRowIndex++)
				for (int x = 0; x < 320; x++) {
					uint8_t scr_byte = src[y * 320 + x];
					dest[y * 2 * 320 + doubleRowIndex * 320 + x] = (scr_byte << 8) | scr_byte;//set two bytes
				}
		}
		if (dest != vgaBuffer_A0000 && rows_count >= 400)//clear last 80 rows
			memset(&dest[rows * 320 * 2], 0, 320 * 2 * 80);
	}
}

//----- (00086EA0) --------------------------------------------------------
uint32_t FadePalettes_86EA0(uint32_t interval)
{
	PaletteChanges_47760();
	return interval;
}

//----- (00090374) --------------------------------------------------------
int SetViewportBounds_90374(uint16_t viewPortX, uint16_t viewPortY, uint16_t viewPortWidth, uint16_t viewPortHeight, unsigned __int16 a5, __int16 a6)//271374
{
	x_BYTE* v7; // edi
	int v8; // eax
	unsigned int v9; // ecx
	int v10; // ecx
	int v11; // esi
	int v12; // esi
	int v13; // [esp+4h] [ebp-4h]

	//fix it
	v13 = 0;
	//fix it

	if (viewPortX < (signed __int16)viewportX_18063C)
		viewPortX = viewportX_18063C;
	if (viewPortWidth < (signed __int16)viewportX_18063C)
		return v13;
	if (viewPortX >= (signed __int16)viewportWidth_180634)
		return v13;
	if (viewPortWidth >= (signed __int16)viewportWidth_180634)
		viewPortWidth = viewportWidth_180634;
	if (viewPortY < (signed __int16)viewportY_180650)
		viewPortY = viewportY_180650;
	if (viewPortHeight < (signed __int16)viewportY_180650)
		return v13;
	if (viewPortY >= (signed __int16)viewportHeight_180630)
		return v13;
	if (viewPortHeight >= (signed __int16)viewportHeight_180630)
		viewPortHeight = viewportHeight_180630;
	v7 = (x_BYTE*)(viewportWidth_180634 * (unsigned __int16)viewPortY + pdwScreenBuffer_351628 + (unsigned __int16)viewPortX);
	v8 = a5;
	v9 = (unsigned __int16)viewPortWidth;
	LOWORD(v9) = viewPortWidth - viewPortX;
	if (viewPortWidth == viewPortX)
	{
		v10 = (unsigned __int16)viewPortHeight;
		LOWORD(v10) = viewPortHeight - viewPortY;
		if (a6 & 4)
		{
			v11 = x_DWORD_E3890;
			do
			{
				BYTE1(v8) = *v7;
				*v7 = *(x_BYTE*)(v8 + v11);
				v7 += viewportWidth_180634;
				--v10;
			} while (v10);
		}
		else
		{
			do
			{
				*v7 = a5;
				v7 += viewportWidth_180634;
				--v10;
			} while (v10);
		}
	}
	else if (a6 & 4)
	{
		v12 = x_DWORD_E3890;
		do
		{
			BYTE1(v8) = *v7;
			*v7++ = *(x_BYTE*)(v8 + v12);
			--v9;
		} while (v9);
	}
	else
	{
		memset(v7, a5, v9);
	}
	return v13;
}

//----- (000904C0) --------------------------------------------------------
char SetGammaCorrection_904C0(float a1)//2714c0
{
	double v1; // st7
	int v2; // eax
	char result; // al
	int v4; // edx
	signed int v5; // ecx

	//fix it
	v2 = 0;
	v4 = 0;
	v5 = 0;
	//fix it

	if (a1 < 0.01)
		a1 = 0.0099999998;
	v1 = a1;
	do
	{
		IF_DPOW(v1, 1.0 / v1);
		v1 = v1 * 63.0 + 0.5;
		result = (signed int)_CHP(v2);
		gammaTable_181504[v4] = result;
	} while (v5 < 64);
	return result;
}
// 9B4F6: using guessed type double /*__fastcall*/ _CHP(x_DWORD);
// A097C: using guessed type double IF_DPOW(double, double);
// D220C: using guessed type double dbl_D220C;
// D2218: using guessed type double dbl_D2218;
// D2220: using guessed type double dbl_D2220;

//----- (00090530) --------------------------------------------------------
//unsigned __int8 sub_90530(int a1, int a2, float a3)//fix//271530 NOTE: removed (unused)

//----- (00090810) --------------------------------------------------------
/*int sub_90810()//271810
{
	int result=0; // eax

	if (x_WORD_E3908)
	{
		//result = sub_905EC_any_graphics_command2(x_WORD_E3908);
		x_WORD_E3908 = 0;
	}
	return result;
}*/
// E3908: using guessed type __int16 x_WORD_E3908;
int16_t VgaPalFadeInFadeOut_90B27(TColor* newpalbufferx, uint8_t shadow_levels, bool singlestep, int32_t frameDelay)//271B27 init and nightfall
{
	//uint8_t *v3; // eax
	//uint16_t v4; // edx
	//uint8_t* v5; // eax
	//uint16_t v6; // edx
	TColor outbufferx[256]; // [esp+0h] [ebp-30Ch]
	uint16_t i; // [esp+300h] [ebp-Ch]
	//uint8_t j = 0;
	//uint8_t* v10; // [esp+308h] [ebp-4h]

	TColor zero_bufferx[256];

	VGA_Init(windowResWidth, windowResHeight, gameResWidth, gameResHeight, maintainAspectRatio, displayIndex);

	if (singlestep)
	{
		if (paletteFadeActive_E390C)
		{
			CurrentPaletteFade_181B44++;
			if (shadow_levels == CurrentPaletteFade_181B44)
				paletteFadeActive_E390C = 0;
		}
		else
		{
			CurrentPaletteFade_181B44 = 0;
			paletteFadeActive_E390C = 1;
			VgaGetPalette_A0D2C(oldPalette_181544);
			if (!newpalbufferx)
				memset(zero_bufferx, 0, 0x300);
		}
		if (!newpalbufferx)
			newpalbufferx = zero_bufferx;
		for (i = 0; i < 0x100; i++)
		{
			//v5 = &buffer[i];
			//LOWORD(v5) = *v5;
			//v6 = i;
			//LOWORD(v6) = x_BYTE_181544_oldpalbuffer[i];
			//v10 = &v5[-v6];
			//outbuffer[i] = x_BYTE_181544_oldpalbuffer[i] + ((unk_181B42 >> 16)* (newpalbuffer[i] - x_BYTE_181544_oldpalbuffer[i])/ shadow_levels);
			outbufferx[i].red = oldPalette_181544[i].red + ((CurrentPaletteFade_181B44) * (newpalbufferx[i].red - oldPalette_181544[i].red) / shadow_levels);
			outbufferx[i].green = oldPalette_181544[i].green + ((CurrentPaletteFade_181B44) * (newpalbufferx[i].green - oldPalette_181544[i].green) / shadow_levels);
			outbufferx[i].blue = oldPalette_181544[i].blue + ((CurrentPaletteFade_181B44) * (newpalbufferx[i].blue - oldPalette_181544[i].blue) / shadow_levels);
		}
		//WaitToScreenBeam_9A0FC();
		VgaPaletteInstall_41A90(outbufferx);
		fix_sub_9A0FC_wait_to_screen_beam(frameDelay);
		//return j;
	}
	else
	{
		VgaGetPalette_A0D2C(oldPalette_181544);
		if (!newpalbufferx)//ebp+14 - 355204
		{
			newpalbufferx = zero_bufferx;
			memset(zero_bufferx, 0, 768);
		}
		for (CurrentPaletteFade_181B44 = 0; CurrentPaletteFade_181B44 < shadow_levels; CurrentPaletteFade_181B44++)
		{
			for (i = 0; i < 0x100; i++)
			{
				/*al=352544[i];
				dl = 352544[i];
				edx=al-dl
				eax=[352b42]>>16
				edx=edx*eax
				bl= shadow_levels
				sar edx,1f//!
				idiv ebx
				edx=352544[i]&0xff
				eax+=edx
				v8[i]=al*/

				//v3 = buffer[i];
				//LOWORD(v3) = *v3;
				//v4 = i;
				//v4 = x_BYTE_181544_oldpalbuffer[i];
				//v10 = &v3[-v4];
				//outbuffer[i] = x_BYTE_181544_oldpalbuffer[i] + ((unk_181B42 >> 16) * (newpalbuffer[i] - x_BYTE_181544_oldpalbuffer[i]) / shadow_levels);//352b42 352544
				outbufferx[i].red = oldPalette_181544[i].red + ((CurrentPaletteFade_181B44) * (newpalbufferx[i].red - oldPalette_181544[i].red) / shadow_levels);//352b42 352544
				outbufferx[i].green = oldPalette_181544[i].green + ((CurrentPaletteFade_181B44) * (newpalbufferx[i].green - oldPalette_181544[i].green) / shadow_levels);//352b42 352544
				outbufferx[i].blue = oldPalette_181544[i].blue + ((CurrentPaletteFade_181B44) * (newpalbufferx[i].blue - oldPalette_181544[i].blue) / shadow_levels);//352b42 352544
			}
			//WaitToScreenBeam_9A0FC();
			VgaPaletteInstall_41A90(outbufferx);
			fix_sub_9A0FC_wait_to_screen_beam(frameDelay);
			//mydelay(10);
		}
		paletteFadeActive_E390C = 0;
	}

	/*if ((a1 == NULL) && (a2 == 0x10) && (a3 == 0)) {//fix it - a1 not changed
		VGA_Init();
	}*/
	//return 0;
	return CurrentPaletteFade_181B44;
}

//----- (00090B27) --------------------------------------------------------
__int16 VgaPalFadeInFadeOutOrig_90B27(char*  /*a1*/, unsigned __int8  /*a2*/, char  /*a3*/)
{
	/*
  char *v3; // eax
  int v4; // edx
  char *v5; // eax
  int v6; // edx
  char v8[768]; // [esp+0h] [ebp-30Ch]
  int i; // [esp+300h] [ebp-Ch]
  unsigned __int8 *v10; // [esp+308h] [ebp-4h]

  if ( a3 )
  {
	if ( paletteFadeActive_E390C )
	{
	  if ( a2 == ++CurrentPaletteFade_181B44 )
		paletteFadeActive_E390C = 0;
	}
	else
	{
	  CurrentPaletteFade_181B44 = 0;
	  paletteFadeActive_E390C = 1;
	  VgaGetPalette_A0D2C(x_BYTE_181544_oldpalbuffer);
	  if ( !a1 )
		memset(&unk_181844, 0, 768);
	}
	if ( !a1 )
	  a1 = (char *)&unk_181844;
	for ( i = 0; (signed __int16)i < 768; i++ )
	{
	  v5 = &a1[(signed __int16)i];
	  LOWORD(v5) = (unsigned __int8)*v5;
	  v6 = (signed __int16)i;
	  LOWORD(v6) = x_BYTE_181544_oldpalbuffer[(signed __int16)i];
	  v10 = (unsigned __int8 *)&v5[-v6];
	  v8[(signed __int16)i] = x_BYTE_181544_oldpalbuffer[(signed __int16)i]
							+ (unsigned int)((unk_181B42 >> 16)
										   * (signed int)(signed __int16)((unsigned __int8)v5 - (unsigned __int8)v6)
										   / a2);
	}
	WaitToScreenBeam_9A0FC();
	VgaPaletteInstall_41A90((unsigned __int8 *)v8);
  }
  else
  {
	VgaGetPalette_A0D2C(x_BYTE_181544_oldpalbuffer);
	if ( !a1 )
	{
	  a1 = (char *)&unk_181844;
	  memset(&unk_181844, 0, 768);
	}
	for ( CurrentPaletteFade_181B44 = 0; a2 >= CurrentPaletteFade_181B44; ++CurrentPaletteFade_181B44 )
	{
	  for ( i = 0; (signed __int16)i < 768; i++ )
	  {
		v3 = &a1[(signed __int16)i];
		LOWORD(v3) = (unsigned __int8)*v3;
		v4 = (signed __int16)i;
		LOWORD(v4) = x_BYTE_181544_oldpalbuffer[(signed __int16)i];
		v10 = (unsigned __int8 *)&v3[-v4];
		v8[(signed __int16)i] = x_BYTE_181544_oldpalbuffer[(signed __int16)i]
							  + (unsigned int)((unk_181B42 >> 16)
											 * (signed int)(signed __int16)((unsigned __int8)v3 - (unsigned __int8)v4)
											 / a2);
	  }
	  WaitToScreenBeam_9A0FC();
	  VgaPaletteInstall_41A90((unsigned __int8 *)v8);
	}
	paletteFadeActive_E390C = 0;
  }
  return CurrentPaletteFade_181B44;*/
	return 0;
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// E390C: using guessed type char paletteFadeActive_E390C;
// 181B44: using guessed type __int16 CurrentPaletteFade_181B44;
// 90B27: using guessed type char var_30C[768];

//----- (00090D27) --------------------------------------------------------
void ResetPaletteFade_90D27()
{
	paletteFadeActive_E390C = 0;
}
// E390C: using guessed type char paletteFadeActive_E390C;

void SetLowResScaledUiScreen()
{
	VGA_Resize(gameResWidth, gameResHeight);
	screenWidth_18062C = gameResWidth;
	screenHeight_180624 = gameResHeight;
	videoMode_180660 = 1;
	SetViewport_A0D50(0, 0, gameResWidth, gameResHeight);
}

void VgaSetVideoMode320x200AndPalette_90D6E(TColor* Palettex)//271d6e
{
	if (lowResScaledUi)
	{
		if (!videoModeNumber_180662)
			videoModeNumber_180662 = 0x13;
		SetLowResScaledUiScreen();
		VgaPaletteInstall_41A90(Palettex);
		return;
	}

	//find 351662 - 3;//fix it
	//unsigned __int8 v2=3; // [esp+0h] [ebp-38h]
	//int v3; // [esp+1Ch] [ebp-1Ch]
			//input1 355200(180200) - 3
			//input2 35521c 00 0f 00 00 00
	//v3 = 0xf00;
//removed int386(0x10, (REGS*)&v3, (REGS*)&v2);//Set video mode
	VGA_Resize(320, 200);
	if (!videoModeNumber_180662)
		videoModeNumber_180662 = 0x13;
	//v3 = 0x13;
	screenWidth_18062C = 320;
	screenHeight_180624 = 200;
	//removed int386(0x10, (REGS*)&v3, (REGS*)&v2);//Write string (BIOS after 1/10/86)

		//a1 - 3aa0a4
	VgaPaletteInstall_41A90(Palettex);
	//removed sub_8D12F_set_mouse_viewport();//set min/max viewport
	SetViewport_A0D50(0, 0, 320, 200);
}

//----- (00090D6E) --------------------------------------------------------
void VgaSetVideoMode320x200AndPaletteOrig_90D6E(TColor* a1x)
{
	unsigned __int8 v2; // [esp+0h] [ebp-38h]
	//int v3; // [esp+1Ch] [ebp-1Ch]

	//fix
	v2 = 0;
	//fix

	//v3 = 0xf00;
//removed int386(0x10, (REGS*)&v3, (REGS*)&v2);//Set video mode
	if (!videoModeNumber_180662)
		videoModeNumber_180662 = v2;
	//v3 = 0x13;
	screenWidth_18062C = 320;
	screenHeight_180624 = 200;
	//removed int386(0x10, (REGS*)&v3, (REGS*)&v2);//Write string (BIOS after 1/10/86)
	VgaPaletteInstall_41A90(a1x);
	//removed sub_8D12F_set_mouse_viewport(); /set min/max viewport
	SetViewport_A0D50(0, 0, 320, 200);
}

//----- (00090E07) --------------------------------------------------------
void VgaSetVideoMode640x480AndPalette_90E07(TColor* Palettex)//271e07
{
	//unsigned __int8 v2; // [esp+0h] [ebp-38h]
	//int v3; // [esp+1Ch] [ebp-1Ch]

	//v3 = 0xf00;//
//removed int386(0x10, (REGS*)&v3, (REGS*)&v2);//Set video mode
	VGA_Resize(640, 480);
	if (!videoModeNumber_180662)
		videoModeNumber_180662 = 0x13;
	screenWidth_18062C = 640;
	screenHeight_180624 = 480;
	//removed sub_994BA_cursor_move(0x101); //set position of cursor?
	VgaPaletteInstall_41A90(Palettex);
	//removed sub_8D12F_set_mouse_viewport(); //set min/max of viewport
	SetViewport_A0D50(0, 0, 640, 480);

	EventDispatcher::I->DispatchEvent<ResourceType, uint8_t>(EventType::E_RESOURCE_CHANGE, ResourceType::CLEAR_COLOURIDX_SET, 0);
}

//----- (00098FF5) --------------------------------------------------------
/*int sub_98FF5_atoi(char *a1)
{
  char *i; // eax
  char v2; // cl
  int v3; // ebx
  int v4; // ebx

  for ( i = a1; IsTable[(unsigned __int8)(*i + 1)] & 2; i++ )
	;
  v2 = i[0];
  if ( i[0] == 43 || v2 == 45 )
	++i;
  v3 = 0;
  while ( IsTable[(unsigned __int8)(*i + 1)] & 0x20 )
  {
	v4 = (unsigned __int8)*i++ + 10 * v3;
	v3 = v4 - 48;
  }
  if ( v2 == 45 )
	v3 = -v3;
  return v3;
}*/

//----- (00099080) --------------------------------------------------------
/*void sub_99080(char a1)
{
  autoScanForSoundHardware_E379B = a1 != 0;
}*/
// E379B: using guessed type char autoScanForSoundHardware_E379B;

//----- (0009937E) --------------------------------------------------------
int SetVideoMode_9937E(__int16  /*a1*/)//27a37e
{
	/*int result; // eax
	int v2; // [esp+0h] [ebp-78h]
	int v3; // [esp+18h] [ebp-60h]
	int v4; // [esp+1Ch] [ebp-5Ch]
	__int16 v5; // [esp+22h] [ebp-56h]
	__int16 v6; // [esp+24h] [ebp-54h]
	char v7; // [esp+34h] [ebp-44h]
	int v8; // [esp+50h] [ebp-28h]
	int v9; // [esp+54h] [ebp-24h]
	int *v10; // [esp+64h] [ebp-14h]
	char v11; // [esp+6Ch] [ebp-Ch]

	memset(&v2, 0, 50);
	v5 = (unsigned int)vesaInfoBuffer_181C40 >> 4;
	v6 = (unsigned int)vesaInfoBuffer_181C40 >> 4;
	v2 = 0;
	v4 = 20225;
	v3 = a1;
	memset(&v8, 0, 28);
	memset(&v7, 0, 28);
	memset(&v11, 0, 12);
	segread((SREGS*)&v11);
	v8 = 0x300;
	v9 = 0x10;
	v10 = &v2;
	int386x(0x31, (REGS*)&v8, (REGS*)&v7, (SREGS*)&v11);//Set video mode
	x_WORD_181C44 = *(unsigned __int8 *)(vesaInfoBuffer_181C40 + 4) + (*(unsigned __int8 *)(vesaInfoBuffer_181C40 + 5) << 8);
	x_WORD_E3BA6 = *(unsigned __int8 *)(vesaInfoBuffer_181C40 + 16) + (*(unsigned __int8 *)(vesaInfoBuffer_181C40 + 17) << 8);
	x_WORD_E3BA8 = *(unsigned __int8 *)(vesaInfoBuffer_181C40 + 18) + (*(unsigned __int8 *)(vesaInfoBuffer_181C40 + 19) << 8);
	HIWORD(result) = HIWORD(vesaInfoBuffer_181C40);
	LOWORD(result) = *(unsigned __int8 *)(vesaInfoBuffer_181C40 + 20);
	x_WORD_E3BAA = (unsigned __int8)result + (*(unsigned __int8 *)(vesaInfoBuffer_181C40 + 21) << 8);
	return result;*/
	return 0;//fix it
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// 99DE6: using guessed type x_DWORD segread(x_DWORD);
// 99E10: using guessed type x_DWORD int386x(x_DWORD, x_DWORD, x_DWORD, x_DWORD);
// E3BA6: using guessed type __int16 x_WORD_E3BA6;
// E3BA8: using guessed type __int16 x_WORD_E3BA8;
// E3BAA: using guessed type __int16 x_WORD_E3BAA;
// 181C40: using guessed type int vesaInfoBuffer_181C40;
// 181C44: using guessed type __int16 x_WORD_181C44;

//----- (000994BA) --------------------------------------------------------
// int sub_994BA_cursor_move(__int16  /*a1*/)//27a4ba NOTE: removed (DOS memory handling)

//----- (0009951B) --------------------------------------------------------
signed int SetVgaPaletteBank_9951B(__int16 a1)//27a51b //fix graphics
{
	if (CommandLineParams.DoShowDebugPerifery())ShowPerifery();

	signed int result; // eax

	vgaPaletteBank_E3BA4 = a1;
	if (a1 > 5 || a1 < 0)
	{
		/*__outx_BYTE(0x3C8u, 0);
		__outx_BYTE(0x3C9u, 0x3Fu);
		__outx_BYTE(0x3C9u, 0);
		__outx_BYTE(0x3C9u, 0);*/
	}
	//fix it:__asm { int     10h; - VIDEO - VESA SuperVGA BIOS -  VESA SuperVGA BIOS - CPU VIDEO MEMORY CONTROL }
	result = 20229;
	//fix it:__asm { int     10h; - VIDEO - VESA SuperVGA BIOS -  VESA SuperVGA BIOS - CPU VIDEO MEMORY CONTROL }
	return result;
}
// E3BA4: using guessed type __int16 vgaPaletteBank_E3BA4;
// 181C40: using guessed type int vesaInfoBuffer_181C40;

//----- (000995B0) --------------------------------------------------------
int TestVgaDriver_995B0() //27A5B0
{
	//int v1; // [esp+0h] [ebp-7Ch]
	//int v2; // [esp+10h] [ebp-6Ch]
	//int v3; // [esp+1Ch] [ebp-60h]
	/*__int16 v4; // [esp+22h] [ebp-5Ah]
	__int16 v5; // [esp+24h] [ebp-58h]
	int v6; // [esp+34h] [ebp-48h]
	int v7; // [esp+38h] [ebp-44h]
	int *v8; // [esp+48h] [ebp-34h]
	char v9; // [esp+50h] [ebp-2Ch]
	char v10; // [esp+6Ch] [ebp-10h]

	memset(&v1, 0, 50);//3551A8
	v4 = (unsigned int)vesaInfoBuffer_181C40 >> 4;//2C03
	v5 = (unsigned int)vesaInfoBuffer_181C40 >> 4;//2C03

	//ebp-7c=0 //dword
	//ebp-60=4f//dword
	//ebp-6c=1//dword
	//v1 = 0;
	//v3 = 20224;
	//v2 = 257;
	memset(&v6, 0, 28);//3551dc
	memset(&v9, 0, 28);//3551f8
	memset(&v10, 0, 12);//355214
	segread((SREGS*)&v10);
	v6 = 0x300;
	v7 = 0x10;
	v8 = &v1;
	int386x(0x31, (REGS*)&v6, (REGS*)&v9, (SREGS*)&v10);//?video*/
	//fix! initialize video

	for (int i = 0; i < 20; i++)vesaInfoBuffer_181C40[i] = 0x0;//fixed
	memcpy((char*)vesaInfoBuffer_181C40, "VESA\0", 5);//fixed
	vesaInfoBuffer_181C40[5] = 0x02;//fixed
	vesaInfoBuffer_181C40[6] = 0x67;//fixed
	vesaInfoBuffer_181C40[7] = 0x2f;//fixed
	vesaInfoBuffer_181C40[9] = 0xc0;//fixed
	vesaInfoBuffer_181C40[14] = 0xab;//fixed
	vesaInfoBuffer_181C40[15] = 0x2e;//fixed
	vesaInfoBuffer_181C40[17] = 0xc0;//fixed
	vesaInfoBuffer_181C40[18] = 0x20;//fixed

	return strncmp((char*)vesaInfoBuffer_181C40, "VESA", 4);
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// 99682: using guessed type x_DWORD strncmp(x_DWORD, x_DWORD, x_DWORD);
// 99DE6: using guessed type x_DWORD segread(x_DWORD);
// 99E10: using guessed type x_DWORD int386x(x_DWORD, x_DWORD, x_DWORD, x_DWORD);
// 181C40: using guessed type int vesaInfoBuffer_181C40;

void CopyScreen(void* source, void* desc, unsigned __int16 width, unsigned __int16 height)
{
	qmemcpy(desc, source, width * height);
}

//----- (000A0B24) --------------------------------------------------------
int SetVgaRegister_A0B24(int a1)//281b24
{
	if ((unsigned __int16)a1 < 0x3C4u && (unsigned __int16)a1 >= 0x3C0u)
	{
		if ((unsigned __int16)a1 <= 0x3C0u)
		{
			x_inp(986);
			x_outp(960, BYTE2(a1) | 0x20);
			return x_outp(960, SHIBYTE(a1));
		}
		if ((unsigned __int16)a1 >= 0x3C2u)
			return x_outp((unsigned __int16)a1, SHIBYTE(a1));
	}
	x_outp((unsigned __int16)a1, SBYTE2(a1));
	return x_outp((unsigned __int16)a1 + 1, SHIBYTE(a1));
}
// A0ED7: using guessed type x_DWORD outp(x_DWORD, char);
// A0EE1: using guessed type x_DWORD inp(x_DWORD);

//----- (000A0BB0) --------------------------------------------------------
int ReadVgaCrtcRegisters_A0BB0(int* a1, int a2)//281bb0
{
	int* v2; // ebx
	int v3; // esi
	char v4; // di
	int result; // eax
	int v6; // ST04_4

	v2 = a1;
	v3 = a2;
	x_outp(980, 17);
	v4 = x_inp(981);
	x_outp(980, 17);
	for (result = x_outp(981, v4 & 0x7F); --v3 != -1; result = SetVgaRegister_A0B24(v6))
	{
		v6 = *v2;
		++v2;
	}
	return result;
}
// A0ED7: using guessed type x_DWORD outp(x_DWORD, char);
// A0EE1: using guessed type x_DWORD inp(x_DWORD);

//----- (000A0D2C) --------------------------------------------------------
void VgaGetPalette_A0D2C(TColor* bufferx)//281d2c
{
	if (CommandLineParams.DoShowDebugPerifery())ShowPerifery();

	uint8_t* tempbuffer = VGA_Get_Palette();
	memcpy(bufferx, tempbuffer, 768);
	/*int v2; // [esp+4h] [ebp-4h]

	//fix it
	v2 = 0;
	//fix it

	__outx_BYTE(0x3C7u, 0);
	__inx_BYTEstring(0x3C9u, a1, 0x300u);
	return v2;*/
}

//----- (000BD2CB) --------------------------------------------------------
void /*__spoils<ecx>*/ BlitScreenToVgaBanks_BD2CB(uint8_t* a1)//29e2cb
{
	//int v1; // ecx
	int v2; // eax
	x_BYTE* v3; // edx
	x_BYTE* v4; // ecx
	int v5; // ST04_4
	x_BYTE* v6; // ST08_4
	x_BYTE* v7; // ST0C_4
	//int v8; // [esp-Ch] [ebp-18h]
	//int v9; // [esp+8h] [ebp-4h]
	int savedregs; // [esp+Ch] [ebp+0h]

	//fix it
	//v1 = 0;
	v3 = 0;
	v4 = 0;
	//fix it

	//v8 = v1;
	SetVgaPaletteBank_9951B(0);
	//v9 = 4096;
	savedregs = 4096;
	v2 = BlitToVgaPlanar2_BD320(0, (int8_t*)pdwScreenBuffer_351628, (int8_t*)ptrMemoryBuffer_E9C3C, 0, savedregs, a1);
	v7 = v4;
	v6 = v3;
	v5 = v2;
	SetVgaPaletteBank_9951B(1);
	//v9 = 3904;
	savedregs = 3904;
	BlitToVgaPlanar2_BD320(v5, (int8_t*)v6, (int8_t*)v7, 0, savedregs, a1);
}

//----- (000BD320) --------------------------------------------------------
int BlitToVgaPlanar2_BD320(int result, int8_t* a2, int8_t* a3, int a4, int savedregs, uint8_t* a6)//29e320
{
	// FIXME: Is this dead code? Seems to be only used for a strange graphics setting

	int* v6; // edi
	int v7; // eax

	v6 = (int*)&vgaBuffer_A0000;
	do
	{
		LOBYTE(a4) = a3[1];
		LOWORD(result) = *(x_WORD*)(a6 + 2 * a4);
		LOBYTE(a4) = a2[1];
		LOWORD(result) = *(x_WORD*)(a6 + 2 * a4 + 512) + result;
		v7 = result << 16;
		LOBYTE(a4) = *a3;
		LOWORD(v7) = *(x_WORD*)(a6 + 2 * a4);
		LOBYTE(a4) = *a2;
		LOWORD(v7) = *(x_WORD*)(a6 + 2 * a4 + 512) + v7;
		*v6 = v7;
		LOBYTE(a4) = a3[3];
		LOWORD(v7) = *(x_WORD*)(a6 + 2 * a4);
		LOBYTE(a4) = a2[3];
		LOWORD(v7) = *(x_WORD*)(a6 + 2 * a4 + 512) + v7;
		v7 <<= 16;
		LOBYTE(a4) = a3[2];
		LOWORD(v7) = *(x_WORD*)(a6 + 2 * a4);
		LOBYTE(a4) = a2[2];
		LOWORD(v7) = *(x_WORD*)(a6 + 2 * a4 + 512) + v7;
		v6[1] = v7;
		LOBYTE(a4) = a3[5];
		LOWORD(v7) = *(x_WORD*)(a6 + 2 * a4);
		LOBYTE(a4) = a2[5];
		LOWORD(v7) = *(x_WORD*)(a6 + 2 * a4 + 512) + v7;
		v7 <<= 16;
		LOBYTE(a4) = a3[4];
		LOWORD(v7) = *(x_WORD*)(a6 + 2 * a4);
		LOBYTE(a4) = a2[4];
		LOWORD(v7) = *(x_WORD*)(a6 + 2 * a4 + 512) + v7;
		v6[2] = v7;
		LOBYTE(a4) = a3[7];
		LOWORD(v7) = *(x_WORD*)(a6 + 2 * a4);
		LOBYTE(a4) = a2[7];
		LOWORD(v7) = *(x_WORD*)(a6 + 2 * a4 + 512) + v7;
		result = v7 << 16;
		LOBYTE(a4) = a3[6];
		LOWORD(result) = *(x_WORD*)(a6 + 2 * a4);
		LOBYTE(a4) = a2[6];
		LOWORD(result) = *(x_WORD*)(a6 + 2 * a4 + 512) + result;
		v6[3] = result;
		a3 += 8;
		a2 += 8;
		v6 += 4;
	} while (savedregs-- != 1);
	return result;
}

//----- (000BD3DD) --------------------------------------------------------
void BlitScreenToVga_BD3DD()//29e3dd
{
	char* v0; // edi
	char* v1; // esi
	signed int v2; // edx
	char* v3; // esi
	char* v4; // esi
	char* v5; // edi
	signed int v6; // edx
	char* v7; // esi
	char* v8; // esi
	char* v9; // edi
	signed int v10; // edx
	char* v11; // edi
	signed int v12; // edx
	char* v13; // edi
	signed int v14; // edx

	if (!blitLock_E3766)
		DrawCursorIfEnabled_8CACD();
	SetVgaPaletteBank_9951B(0);
	v0 = (char*)&vgaBuffer_A0000 + 51200;
	v1 = (char*)pdwScreenBuffer_351628;
	v2 = 11;
	do
	{
		qmemcpy(v0, v1, 640);
		v1 += 1280;
		v0 += 1280;
		--v2;
	} while (v2);
	qmemcpy(v0, v1, 0x100u);
	v3 = v1 + 256;
	SetVgaPaletteBank_9951B(1);
	qmemcpy(&vgaBuffer_A0000, v3, 0x180u);
	v4 = v3 + 1024;
	v5 = (char*)&vgaBuffer_A0000 + 1024;
	v6 = 50;
	do
	{
		qmemcpy(v5, v4, 640);
		v4 += 1280;
		v5 += 1280;
		--v6;
	} while (v6);
	qmemcpy(v5, v4, 0x200u);
	v7 = v4 + 512;
	SetVgaPaletteBank_9951B(2);
	qmemcpy(&vgaBuffer_A0000, v7, 0x80u);
	v8 = v7 + 768;
	v9 = (char*)&vgaBuffer_A0000 + 768;
	v10 = 51;
	do
	{
		qmemcpy(v9, v8, 640);
		v8 += 1280;
		v9 += 1280;
		--v10;
	} while (v10);
	SetVgaPaletteBank_9951B(3);
	v11 = (char*)&vgaBuffer_A0000 + 512;
	v12 = 51;
	do
	{
		qmemcpy(v11, v8, 640);
		v8 += 1280;
		v11 += 1280;
		--v12;
	} while (v12);
	SetVgaPaletteBank_9951B(4);
	v13 = (char*)&vgaBuffer_A0000 + 256;
	v14 = 34;
	do
	{
		qmemcpy(v13, v8, 640);
		v8 += 1280;
		v13 += 1280;
		--v14;
	} while (v14);
	if (!blitLock_E3766)
		ClearCursorDrawnFlag_8CB1F();
}
