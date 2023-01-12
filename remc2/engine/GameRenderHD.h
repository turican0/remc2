#pragma once

#ifndef GAME_RENDER_HD
#define GAME_RENDER_HD

#include "GameRenderInterface.h"

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
#include "defs.h"
#include "RenderThread.h"
#include "ProjectionPolygon.h"

class GameRenderHD : public GameRenderInterface
{

private:

	uint8_t unk_D4328x[40] = {
		0xED,0x01,0x00,0x00,0x00,0xFF,0xD8,0xFF,0x01,0x00,0x00,0xED,0xFF,0x00,0x01,0x00,
		0x01,0xD8,0x00,0x01,0x13,0x00,0xFF,0xFF,0x00,0x01,0x28,0x01,0xFF,0x00,0x01,0x13,
		0x00,0xFF,0xFF,0x00,0xFF,0x28,0x00,0xFF
	};

	uint8_t unk_DE56Cx[8][4194304]; //Number of possible render threads (8) //number of polygons (2048 * 2048)
	
	int offsets_B8845[16] = {
		  0, -15, -14, -13,
		-12, -11, -10,  -9,
		 -8,  -7,  -6,  -5,
		 -4,  -3,  -2,  -1
	};

	char x_BYTE_D4750[60] = {
		0x00,0x00,0x02,0x03,0x04,0x05,0x01,0x00,0x06,0x07,0x04,0x05,0x00,0x01,0x01,0x02,
		0x02,0x03,0x03,0x04,0x04,0x03,0x03,0x02,0x02,0x01,0x01,0x00,0x00,0x00,0x00,0x01,
		0x01,0x01,0x02,0x02,0x02,0x02,0x02,0x01,0x01,0x01,0x00,0x00,0x03,0x03,0x03,0x03,
		0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0C,0x0C,0x0C
	};

	type_unk_F0E20x m_str_F0E20x[1920]; // Originally 640

	uint8_t* m_ptrScreenBuffer_351628 = nullptr;
	uint8_t* m_ptrColorPalette = nullptr;
	uint16_t m_uiScreenWidth_18062C = 0;
	uint16_t m_uiScreenHeight_180624 = 0;

	std::vector<RenderThread*> m_renderThreads;
	bool m_multiThreadRender = false;
	bool m_assignToSpecificCores = false;

	int x_DWORD_D4794 = 0;
	int x_DWORD_D4798 = 0;
	char x_BYTE_E126D = 0;
	char x_BYTE_E126C = 112;
	int x_DWORD_D4790 = 20;
	int x_DWORD_D4324 = 0;
	char shadows_F2CC7 = 0;
	char notDay_D4320 = 0;
	char x_BYTE_F2CC6 = 0;
	int16_t yaw_F2CC0 = 0;
	int16_t x_WORD_F2CC2 = 0;
	int16_t x_WORD_F2CC4 = 0;

	uint8_t* x_DWORD_DE55C_ActTexture = 0;

	void DrawSky_40950(int16_t roll, uint8_t startLine, uint8_t drawEveryNthLine);
	void DrawSky_40950_TH(int16_t roll);
	void DrawTerrainAndParticles_3C080(__int16 posX, __int16 posY, __int16 yaw, signed int posZ, int pitch, int16_t roll, int fov);
	void SubDrawTerrainAndParticles(std::vector<int>& projectedVertexBuffer, int pitch);
	void SubDrawInverseTerrainAndParticles(std::vector<int>& projectedVertexBuffer, int pitch);
	void SubDrawCaveTerrainAndParticles(std::vector<int>& projectedVertexBuffer, int pitch);
	void DrawSprite_41BD3(uint32 a1);
	void DrawSquareInProjectionSpace(std::vector<int>& vertexs, int index);
	void DrawInverseSquareInProjectionSpace(int* vertexs, int index);
	void DrawInverseSquareInProjectionSpace(int* vertexs, int index, uint8_t* pTexture);
	void DrawParticles_3E360(int a2x, type_particle_str** str_DWORD_F66F0x[], uint8_t x_BYTE_E88E0x[], int32_t x_DWORD_F5730[], type_event_0x6E8E* x_DWORD_EA3E4[], type_str_unk_1804B0ar str_unk_1804B0ar, ViewPort viewPort, uint16_t screenWidth);
	void DrawTriangleInProjectionSpace_B6253(const ProjectionPolygon* vertex1, const ProjectionPolygon* vertex2, const ProjectionPolygon* vertex3, uint8_t startLine, uint8_t drawEveryNthLine);
	x_DWORD* LoadPolygon(x_DWORD* ptrPolys, int* v0, int* v1, int s0, int s1, int* line);
	x_DWORD* LoadPolygon(x_DWORD* ptrPolys, int* v0, int* v1, int* v4, int s0, int s1, int s4, int* line);
	x_DWORD* LoadPolygon(x_DWORD* ptrPolys, int* v0, int* v1, int* v2, int* v3, int s0, int s1, int s2, int s3, int* line);
	x_DWORD* LoadPolygon(x_DWORD* ptrPolys, int* v0, int* v1, int* v2, int* v3, int* v4, int s0, int s1, int s2, int s3, int s4, int* line);
	uint16_t sub_3FD60(int a2x, uint8_t x_BYTE_E88E0x[], type_event_0x6E8E* x_DWORD_EA3E4[], type_str_unk_1804B0ar str_unk_1804B0ar, type_particle_str** str_DWORD_F66F0x[], int32_t x_DWORD_F5730[], ViewPort viewPort, uint16_t screenWidth);
	void sub_88740(type_event_0x6E8E* a1, int a2, int a3);
	void SetBillboards_3B560(int16_t roll);
	void DrawSorcererNameAndHealthBar_2CB30(type_event_0x6E8E* a1, __int16 a2, int a3, __int16 a4);
	void StartWorkerThreads(uint8_t numOfThreads, bool assignToSpecificCores);
	void StartWorkerThread();
	void StartWorkerThread(int core);
	void StopWorkerThreads();
	void WaitForRenderFinish();

public:
	GameRenderHD(uint8_t* ptrScreenBuffer, uint8_t* pColorPalette, uint16_t screenWidth, uint16_t screenHeight, uint8_t renderThreads, bool assignToSpecificCores);
	~GameRenderHD();
	
	void SetRenderThreads(uint8_t renderThreads);
	uint8_t GetRenderThreads();

	void DrawWorld_411A0(int posX, int posY, int16_t yaw, int16_t posZ, int16_t pitch, int16_t roll, int16_t fov);
	void WriteWorldToBMP();
	void ClearGraphicsBuffer(uint8_t colorIdx);
};

#endif //GAME_RENDER_HD