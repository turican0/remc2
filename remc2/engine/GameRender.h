#pragma once

#ifndef GAME_RENDER
#define GAME_RENDER

#include <array>
#include <thread>

#include "../utilities/Maths.h"
#include "../utilities/BitmapIO.h"
#include "../portability/mctypes.h"
#include "../utilities/SafeQueue.h"
#include "Terrain.h"
#include "Type_F2C20ar.h"
#include "Type_E9C38_Smalltit.h"
#include "Type_Unk_F0E20x.h"
#include "Type_WORD_D951C.h"
#include "XUnk_D4350.h"
#include "Type_D404C.h"
#include "Type_D94F0_Bldgprmbuffer.h"
#include "Type_D93C0_Bldgprmbuffer.h"
#include "TextureMaps.h"
#include "engine_support.h"
#include "ViewPort.h"
#include "defs.h"
#include "RenderThread.h"

class GameRender {

private:

	type_E9C38_smalltit m_Str_E9C38_smalltit[21 * 40];

	uint8_t unk_D4328x[40] = {
		0xED,0x01,0x00,0x00,0x00,0xFF,0xD8,0xFF,0x01,0x00,0x00,0xED,0xFF,0x00,0x01,0x00,
		0x01,0xD8,0x00,0x01,0x13,0x00,0xFF,0xFF,0x00,0x01,0x28,0x01,0xFF,0x00,0x01,0x13,
		0x00,0xFF,0xFF,0x00,0xFF,0x28,0x00,0xFF
	};

	uint8_t unk_DE56Cx[8][4194304]; //Number of possible render threads (8) //number of polygons (2048 * 2048)
	
	uint32_t x_DWORD_B8845[16] = {
		0x00000000,0xFFFFFFF1,0xFFFFFFF2,0xFFFFFFF3,
		0xFFFFFFF4,0xFFFFFFF5,0xFFFFFFF6,0xFFFFFFF7,
		0xFFFFFFF8,0xFFFFFFF9,0xFFFFFFFA,0xFFFFFFFB,
		0xFFFFFFFC,0xFFFFFFFD,0xFFFFFFFE,0xFFFFFFFF
	};

	char x_BYTE_D4750[60] = {
		0x00,0x00,0x02,0x03,0x04,0x05,0x01,0x00,0x06,0x07,0x04,0x05,0x00,0x01,0x01,0x02,
		0x02,0x03,0x03,0x04,0x04,0x03,0x03,0x02,0x02,0x01,0x01,0x00,0x00,0x00,0x00,0x01,
		0x01,0x01,0x02,0x02,0x02,0x02,0x02,0x01,0x01,0x01,0x00,0x00,0x03,0x03,0x03,0x03,
		0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0C,0x0C,0x0C
	};

	type_unk_F0E20x m_str_F0E20x[1920]; // Originally 640

	uint8_t* m_ptrScreenBuffer = nullptr;
	uint8_t* m_ptrX_BYTE_F6EE0_tablesx = nullptr;
	uint8_t m_uiScreenSize = 0;
	uint8_t* m_ptrViewPortRenderBufferStart = nullptr;
	uint8_t* m_ptrViewPortRenderBufferAltStart = nullptr;
	ViewPort m_viewPort;
	uint16_t m_uiScreenWidth = 0;
	uint16_t m_uiScreenHeight = 0;

	std::array<uint8_t*, 256> m_textureAddresses;
	uint8_t* m_ptrColorPalette = 0;
	type_F2C20ar m_str_F2C20ar;
	int16_t m_x_WORD_180660_VGA_type_resolution;

	std::vector<RenderThread*> m_renderThreads;
	bool m_multiThreadRender = false;
	bool m_assignToSpecificCores = false;

	int x_DWORD_D4794 = 0;
	int x_DWORD_D4798 = 0;
	char x_BYTE_E126D = 0;
	char x_BYTE_E126C = 112;
	int x_DWORD_D4790 = 20;
	int x_DWORD_D4324 = 0;
	char x_BYTE_F2CC7 = 0;
	char x_BYTE_D4320 = 0;
	char x_BYTE_F2CC6 = 0;
	int16_t x_WORD_F2CC0 = 0;
	int16_t x_WORD_F2CC2 = 0;
	int16_t x_WORD_F2CC4 = 0;

	void DrawSky_40950(int16_t roll, uint8_t* ptrViewPortRenderBufferStart, uint16_t viewPortWidth, uint16_t viewPortHeight, uint16_t pitch, uint8_t startLine, uint8_t drawEveryNthLine);
	void DrawSky_40950_TH(int16_t roll, uint8_t* ptrViewPortRenderBufferStart, uint16_t viewPortWidth, uint16_t viewPortHeight, uint16_t pitch);
	void DrawTerrainAndParticles_3C080(__int16 posX, __int16 posY, __int16 yaw, signed int posZ, int pitch, int16_t roll, int fov, type_particle_str** str_DWORD_F66F0x[], uint8_t x_BYTE_E88E0x[], int32_t x_DWORD_F5730[], uint8_t unk_F0A20x[], type_event_0x6E8E* x_DWORD_EA3E4[], type_str_unk_1804B0ar str_unk_1804B0ar, int16_t x_WORD_D4B7C, char isCaveLevel, ViewPort viewPort, uint16_t screenWidth);
	void DrawSprite_41BD3(uint32 a1, uint8_t x_BYTE_E88E0x[], type_event_0x6E8E* x_DWORD_EA3E4[], type_str_unk_1804B0ar str_unk_1804B0ar, ViewPort viewPort, uint16_t pitch);
	void DrawSquareInProjectionSpace(int* vertexs, int index, uint16_t viewPortWidth, uint16_t viewPortHeight, uint16_t pitch);
	void DrawInverseSquareInProjectionSpace(int* vertexs, int index, uint16_t viewPortWidth, uint16_t viewPortHeight, uint16_t pitch);
	void DrawInverseSquareInProjectionSpace(int* vertexs, int index, uint8_t* pTexture, uint16_t viewPortWidth, uint16_t viewPortHeight, uint16_t pitch);
	void DrawParticles_3E360(int a2x, type_particle_str** str_DWORD_F66F0x[], uint8_t x_BYTE_E88E0x[], int32_t x_DWORD_F5730[], type_event_0x6E8E* x_DWORD_EA3E4[], type_str_unk_1804B0ar str_unk_1804B0ar, ViewPort viewPort, uint16_t screenWidth);
	void DrawTriangleInProjectionSpace_B6253(x_DWORD* vertex1, x_DWORD* vertex2, x_DWORD* vertex3, uint8_t* pTexture, uint8_t unk_DE56Cx[], uint16_t viewPortWidth, uint16_t viewPortHeight, uint16_t pitch, uint8_t startLine, uint8_t drawEveryNthLine);
	x_DWORD* LoadPolygon(x_DWORD* ptrPolys, int* v0, int* v1, int s0, int s1, int* line);
	x_DWORD* LoadPolygon(x_DWORD* ptrPolys, int* v0, int* v1, int* v4, int s0, int s1, int s4, int* line);
	x_DWORD* LoadPolygon(x_DWORD* ptrPolys, int* v0, int* v1, int* v2, int* v3, int s0, int s1, int s2, int s3, int* line);
	x_DWORD* LoadPolygon(x_DWORD* ptrPolys, int* v0, int* v1, int* v2, int* v3, int* v4, int s0, int s1, int s2, int s3, int s4, int* line);
	int32_t* x_DWORD_DB350_ret(uint32_t adress);
	uint16_t sub_3FD60(int a2x, uint8_t x_BYTE_E88E0x[], type_event_0x6E8E* x_DWORD_EA3E4[], type_str_unk_1804B0ar str_unk_1804B0ar, type_particle_str** str_DWORD_F66F0x[], int32_t x_DWORD_F5730[], ViewPort viewPort, uint16_t screenWidth);
	void sub_88740(type_event_0x6E8E* a1, type_event_0x6E8E* x_DWORD_EA3E4[], type_str_unk_1804B0ar str_unk_1804B0ar, int a2, int a3);
	void SetBillboards_3B560(int16_t roll, uint8_t unk_F0A20x[], uint16_t viewPortWidth, uint16_t viewPortHeight, uint16_t screenWidth);
	void DrawSorcererNameAndHealthBar_2CB30(type_event_0x6E8E* a1, uint8_t x_BYTE_E88E0x[], __int16 a2, int a3, __int16 a4, int16_t viewPortX, int16_t viewPortY, uint16_t viewPortWidth, uint16_t viewPortHeight, uint16_t pitch);
	void StartWorkerThreads(uint8_t numOfThreads, bool assignToSpecificCores);
	void StartWorkerThread();
	void StartWorkerThread(int core);
	void StopWorkerThreads();
	void WaitForRenderFinish();

public:
	GameRender(uint8_t* pScreenBuffer, uint8_t* pColorPalette, uint16_t screenWidth, uint16_t screenHeight, uint16_t viewPortPosX, uint16_t viewPortPosY, uint16_t viewPortWidth, uint16_t viewPortHeight, std::array<uint8_t*, 256> &textureAdresses, uint8_t x_BYTE_F6EE0_tablesx[], uint8_t renderThreads, bool assignToSpecificCores);
	~GameRender();

	void SetTextures(std::array<uint8_t*, 256> &textureAdresses);
	void SetX_BYTE_F6EE0_tablesx(uint8_t pX_BYTE_F6EE0_tablesx[]);
	void SetRenderViewPortSize_BCD45(ViewPort viewPort, uint16_t screenWidth, uint16_t screenHeight);
	void SetRenderViewPortSize_BCD45(uint8_t viewPortSizeSetting, uint16_t screenWidth, uint16_t screenHeight);
	void SetRenderViewPortSize_BCD45(uint8_t* ptrScreenBufferStart, uint16_t viewPortWidth, uint16_t viewPortHeight, uint16_t screenWidth);
	void SetRenderThreads(uint8_t renderThreads);
	uint8_t GetRenderThreads();

	void DrawWorld(int posX, int posY, int16_t yaw, int16_t posZ, int16_t pitch, int16_t roll, int16_t fov, uint8_t heightmap[], type_particle_str** str_DWORD_F66F0x[], uint8_t x_BYTE_E88E0x[], int32_t x_DWORD_F5730[], uint8_t unk_F0A20x[], type_event_0x6E8E* x_DWORD_EA3E4[], type_str_unk_1804B0ar str_unk_1804B0ar, int16_t x_WORD_180660_VGA_type_resolution, int16_t x_WORD_D4B7C, char isCaveLevel);
	void WriteWorldToBMP();
	void ClearGraphicsBuffer(uint8_t colorIdx);
};

#endif //GAME_RENDER