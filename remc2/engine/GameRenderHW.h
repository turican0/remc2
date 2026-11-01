#pragma once

#ifndef GAME_RENDER_HW
#define GAME_RENDER_HW

#include "GameRenderInterface.h"

#include <array>
#include <thread>
#include <cmath>

#include "../utilities/Maths.h"
#include "../utilities/BitmapIO.h"
#include "../portability/bitmap_pos_struct.h"
#include "../utilities/SafeQueue.h"
#include "Terrain.h"
#include "Type_F2C20ar.h"
#include "Type_E9C38_Smalltit.h"
#include "Type_Unk_F0E20x.h"
#include "Type_SpriteDef_D951C.h"
#include "UVTable_D4350.h"
#include "Type_D404C.h"
#include "Type_D94F0_Bldgprmbuffer.h"
#include "Type_D93C0_Bldgprmbuffer.h"
#include "TextureMaps.h"
#include "defs.h"
#include "RenderThread.h"
#include "ProjectionVertex.h"
#include "../portability/RenderPolygon.h"

class GameRenderHW : public GameRenderInterface
{

private:

	const uint32_t kTilesWide = 8;
	const float kTileUVInset = 1.0f / 64.0f;   // texels kept clear of each tile edge in the atlas

	static constexpr uint32_t kMaxReservedViewDistance = 6;
	static constexpr uint32_t kMaxReservedSprites = 200;
	static constexpr uint32_t kReservedPolygons = 1
		+ 2 * TILE_ROWS_COUNT * TILE_COLUMNS_COUNT * kMaxReservedViewDistance * kMaxReservedViewDistance
		+ kMaxReservedSprites;

	struct TileStepQuadrant {
		uint8_t startX;      // [0] starting X map coordinate offset
		uint8_t startY;      // [1] starting Y map coordinate offset
		uint8_t texOffX;     // [2] texture neighbour X offset
		uint8_t texOffY;     // [3] texture neighbour Y offset
		uint8_t entOffX;     // [4] entity/billboard X offset
		uint8_t entOffY;     // [5] entity/billboard Y offset
		uint8_t rowStepX;    // [6] row step X
		uint8_t rowStepY;    // [7] row step Y
		uint8_t colStepX;    // [8] column step X
		uint8_t colStepY;    // [9] column step Y
	};

	//The sprite render uses hardcoded offsets of the buffer. This have been adjusted to the new buffer size of
	//m_ptrDWORD_E9C38_smalltit otherwise at higher resolutions sprite data they writes over each other
	const int m_bufferOffset_E9C38_1 = 995328; // 12%
	const int m_bufferOffset_E9C38_2 = 1161216; // 14%
	const int m_bufferOffset_E9C38_3 = 1658880; // 20%

	TileStepQuadrant* m_tileRenderStepTable_D4328x;

	uint8_t unk_DE56Cx[8][4194304]; //Number of possible render threads (8) //number of polygons (2048 * 2048)

	int offsets_B8845[16] = {
		  0, -15, -14, -13,
		-12, -11, -10,  -9,
		 -8,  -7,  -6,  -5,
		 -4,  -3,  -2,  -1
	};

	char x_BYTE_D4750[60] = {
		0x00,0x00,0x02,0x03,0x04,0x05,//0
		0x01,0x00,0x06,0x07,0x04,0x05,//6
		0x00,0x01,0x01,0x02,0x02,0x03,0x03,0x04,0x04,0x03,0x03,0x02,0x02,0x01,0x01,0x00,//12
		0x00,0x00,0x00,0x01,0x01,0x01,0x02,0x02,0x02,0x02,0x02,0x01,0x01,0x01,0x00,0x00,//28
		0x03,0x03,0x03,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0C,0x0C,0x0C//44
	};

	type_unk_F0E20x m_str_F0E20x[GAME_RES_MAX_WIDTH + 100]; // Originally 640
	type_E9C38_smalltit* m_ptrStr_E9C38_smalltit;
	uint16_t m_tileRows = TILE_ROWS_COUNT;
	uint16_t m_tileColumns = TILE_COLUMNS_COUNT;

	uint8_t m_viewDistanceScale = 1;

	// Squared distances in 1/65536 of a tile squared (64 bit: the 32-bit values overflow past view distance 6)
	int64_t m_cutOffDistance = 0;
	int64_t m_fogStart = 0;
	int64_t m_fogEnd = 0;

	uint8_t* m_ptrDWORD_E9C38_smalltit = nullptr;
	uint8_t* m_ptrScreenBuffer_351628 = nullptr;
	uint8_t* m_ptrColorPalette = nullptr;

	bool m_multiThreadRender = false;
	bool m_assignToSpecificCores = false;
	bool m_blurActive = false;

	int x_DWORD_D4794 = 0;
	int x_DWORD_D4798 = 0;
	char x_BYTE_E126D = 0;
	char x_BYTE_E126C = 112;
	int x_DWORD_D4790 = 20;
	int x_DWORD_D4324 = 0;
	char shadows_F2CC7 = 0;
	char notDay_D4320 = 0;
	bool m_reflectionPass = false;   // set while the reflection pass emits its polygons (RenderPolygon::Reflection)
	char x_BYTE_F2CC6 = 0;
	int16_t yaw_F2CC0 = 0;
	int16_t cameraY_F2CC2 = 0;
	int16_t cameraX_F2CC4 = 0;

	uint8_t* x_DWORD_DE55C_ActTexture = 0;

	void DrawSky_40950(int16_t roll, std::vector<RenderPolygon>* polygons);
	void DrawTerrainAndParticles_3C080(__int16 posX, __int16 posY, __int16 yaw, signed int posZ, int pitch, int16_t roll, int fov);
	void SubDrawTerrainAndParticles(std::vector<int>& projectedVertexBuffer, int pitch, std::vector<RenderPolygon>* polygons);
	void SubDrawInverseTerrainAndParticles(std::vector<int>& projectedVertexBuffer, int pitch, std::vector<RenderPolygon>* polygons);
	void SubDrawCaveTerrainAndParticles(std::vector<int>& projectedVertexBuffer, int pitch, std::vector<RenderPolygon>* polygons);
	uint32_t SpriteFrameRow(int spriteIndex, int spriteFrame);
	void DrawSprite_41BD3(uint32_t pass, std::vector<RenderPolygon>* polygons, int spriteIndex, int spriteFrame);
	void EmitSpriteQuad(std::vector<RenderPolygon>* polygons, int spriteIndex, int spriteFrame,
		float x, float y, float rx, float ry, float dx, float dy, bool flipV);
	void DrawSquareInProjectionSpace(std::vector<int>& vertexs, int tileIndex, std::vector<RenderPolygon>* polygons);
	HWVertex MakeHWVertex(const ProjectionVertex& v, uint32_t textureIndex);
	void DrawInverseSquareInProjectionSpace(int* vertexs, int tileIndex, std::vector<RenderPolygon>* polygons, int overrideTextIndex = -1);
	void DrawSprites_3E360(int a2x, Type_Sprite** m_ptrLoadedSprites_F66F0x[], uint8_t playersColors_E88E0x[][3], int32_t x_DWORD_F5730[], type_entity_0x6E8E* Entities_EA3E4[], type_str_unk_1804B0ar str_unk_1804B0ar, ViewPort viewPort, uint16_t screenWidth, std::vector<RenderPolygon>* polygons);
	x_DWORD* LoadPolygon(x_DWORD* ptrPolys, int* v0, int* v1, int s0, int s1, int* line);
	x_DWORD* LoadPolygon(x_DWORD* ptrPolys, int* v0, int* v1, int* v4, int s0, int s1, int s4, int* line);
	x_DWORD* LoadPolygon(x_DWORD* ptrPolys, int* v0, int* v1, int* v2, int* v3, int s0, int s1, int s2, int s3, int* line);
	x_DWORD* LoadPolygon(x_DWORD* ptrPolys, int* v0, int* v1, int* v2, int* v3, int* v4, int s0, int s1, int s2, int s3, int s4, int* line);
	uint16_t DrawTileBillboards_3FD60(int a2x, uint8_t playersColors_E88E0x[][3], type_entity_0x6E8E* x_DWORD_EA3E4[], type_str_unk_1804B0ar str_unk_1804B0ar, Type_Sprite** m_ptrLoadedSprites_F66F0x[], int32_t x_DWORD_F5730[], ViewPort viewPort, uint16_t screenWidth, std::vector<RenderPolygon>* polygons);
	void sub_88740(type_entity_0x6E8E* a1, int16_t posX, int16_t posY);
	void SetBillboards_3B560(int16_t roll);
	void DrawSorcererNameAndHealthBar_2CB30(type_entity_0x6E8E* a1, int16_t a2, int a3, int16_t a4);
	void BuildTileRenderStepTable(TileStepQuadrant* table, int cols);
	bool CheckViewPortCull(ProjectionVertex v1, ProjectionVertex v2, ProjectionVertex v3, int maxCoordinate = 2147483647, int minCoordinate = -2147483647);
	int SpriteLight(int64_t distance) const;
	int FogLight(int light, int64_t distance) const;

public:
	GameRenderHW(uint8_t* ptrScreenBuffer, uint8_t* pColorPalette, uint8_t viewDistanceScale = 1, float fogScale = 1.0f);
	~GameRenderHW();

	void SetBlurActive(bool active);
	void DrawWorld_411A0(int posX, int posY, int16_t yaw, int16_t posZ, int16_t pitch, int16_t roll, int16_t fov);
	void ClearGraphicsBuffer(uint8_t colorIdx);

	static int32_t CalculateRotationTranslationX(int64_t cos_0x11, int64_t pnt1, int64_t sin_0x0d, int64_t pnt2);
	static int32_t CalculateRotationTranslationY(int64_t pnt1, int64_t sin_0x0d, int64_t cos_0x11, int64_t pnt2);
};

#endif //GAME_RENDER_HW
