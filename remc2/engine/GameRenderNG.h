#pragma once

#ifndef GAME_RENDER_NG
#define GAME_RENDER_NG

#include "GameRenderInterface.h"
#include "RenderThread.h"

#include <array>
#include <atomic>
#include <thread>
#include <memory>
#include <vector>

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
#include "ProjectionVertex.h"

class GameRenderNG : public GameRenderInterface
{
public:
	// the texture columns of a sprite (x_DWORD_E9C38_smalltit + 36960): step from the previous column, offset in the row
	struct SpriteColumn
	{
		int32_t step;
		int32_t offset;
	};
	// a corner of a tile for DrawTriangleInProjectionSpace_B6253: screen position, texture position, light
	struct TerrainVertex
	{
		int x, y, u, v, light, unused;
	};
	// a line of a rotated sprite (x_DWORD_E9C38_smalltit + 45920): first step on the view line, steps, skipped columns
	struct SpriteSpan
	{
		int32_t start;
		int32_t length;
		int32_t skip;
	};


private:

	uint8_t m_tileSteps_D4328[40] = {
		0xED,0x01,0x00,0x00,0x00,0xFF,0xD8,0xFF,0x01,0x00,0x00,0xED,0xFF,0x00,0x01,0x00,
		0x01,0xD8,0x00,0x01,0x13,0x00,0xFF,0xFF,0x00,0x01,0x28,0x01,0xFF,0x00,0x01,0x13,
		0x00,0xFF,0xFF,0x00,0xFF,0x28,0x00,0xFF
	};//2a5328

	char m_spriteTables_D4750[60] = {
		0x00,0x00,0x02,0x03,0x04,0x05,0x01,0x00,0x06,0x07,0x04,0x05,0x00,0x01,0x01,0x02,
		0x02,0x03,0x03,0x04,0x04,0x03,0x03,0x02,0x02,0x01,0x01,0x00,0x00,0x00,0x00,0x01,
		0x01,0x01,0x02,0x02,0x02,0x02,0x02,0x01,0x01,0x01,0x00,0x00,0x03,0x03,0x03,0x03,
		0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0C,0x0C,0x0C
	};

	type_unk_F0E20x m_viewLinesMemory[1 + GAME_RES_MAX_WIDTH + 100];//[0]: step -1, the end of unk_F0A20x
	type_unk_F0E20x* const m_viewLines_F0E20 = &m_viewLinesMemory[1];//640 in the original

	int m_cameraSwayX_D4794 = 0;
	int m_cameraSwayY_D4798 = 0;
	char m_triangleMode_E126D = 0;
	char m_flatLight_E126C = 112;
	int m_eyeShift_D4324 = 0;
	char m_shadows_F2CC7 = 0;
	char m_notDay_D4320 = 0;
	char m_uprightSprite_F2CC6 = 0;
	int16_t m_cameraYaw_F2CC0 = 0;
	int16_t m_cameraY_F2CC2 = 0;
	int16_t m_cameraX_F2CC4 = 0;
	uint8_t* m_triangleTexture_DE55C = 0;

	uint8_t m_viewDistanceScale = 1;
	int m_tileColumns = 40;
	int m_tileRows = 21;
	int m_cameraRow = 1;//the tile row with the camera (ForEachTileFarToNear)
	type_E9C38_smalltit* m_tiles = Str_E9C38_smalltit;//the original grid, own storage for a bigger one
	std::vector<type_E9C38_smalltit> m_ownTiles;
	int64_t m_cutOffDistance = 0;//64 bits: a view distance from 10
	int64_t m_fogStart = 0;
	int64_t m_fogEnd = 0;
	int64_t m_standardFogStart = 0;//the fog of view distance 1 (FadesInStandardFog)
	int64_t m_standardFogEnd = 0;
	int64_t m_standardCutOff = 0;
	int m_standardFirstRow = 0;//the original grid (40 x 21) in a bigger one: its row 0 and column 0
	int m_standardFirstColumn = 0;
	bool m_standardView = true;//the entity drawn now is drawn also by the original grid and cut-off (the seen flag)
	int m_fadeVisibility = -1;//ChooseSprite: the mode of a sprite fading in the standard fog (FadesInStandardFog), -1 none

	bool m_largeScreen = false;//over 640x480
	uint8_t* m_scratch = nullptr;//the tables of SetBillboards_3B560 and of the sprites
	int m_columnsOffset = 36960;
	int m_spansOffset = 45920;
	int m_rowsOffset = 59360;
	int m_columnsCapacity = 0;//a big screen: the last index of the column table
	std::vector<uint8_t> m_largeScratch;
	uint8_t* m_blurBuffer = nullptr;
	std::vector<uint8_t> m_largeBlurBuffer;

	std::vector<std::unique_ptr<RenderThread>> m_renderThreads;
	bool m_assignToSpecificCores = false;

	// With render threads over 640x480 the terrain is queued and drawn by bands of rows: every thread takes bands and draws
	// the queue in its order into them (every pixel gets the same writes in the same order as in one thread).
	struct QueuedTriangle
	{
		TerrainVertex corners[3];
		const uint8_t* texture;
		int mode;
		int flatLight;
		int minY;
		int maxY;
	};
	std::vector<QueuedTriangle> m_queue;
	int64_t m_queuedArea = 0;//pixels of the bounding boxes in the view port: a small queue is drawn by the game thread
	bool m_queueing = false;
	bool m_skyQueued = false;
	std::atomic<int> m_nextBand{ 0 };
	// the sky of the frame (DrawSky_40950), drawn by rows
	struct SkyPlan
	{
		int size = 256;
		int mask = 255;
		int cosRoll = 0;
		int sinRoll = 0;
		int lineU = 0;
		int lineV = 0;
		std::vector<int> columnU;
		std::vector<int> columnV;
	} m_sky;

	void BuildTileSteps();
	void WaitForRenderThreads();
	void FlushQueue();
	void SetScreenBuffers();
	// the original draws groups of 4 pixels only
	int DrawnWidth() const { return m_largeScreen ? viewPort.Width_DE564 : viewPort.Width_DE564 & ~3; }
	void DrawSky_40950(int16_t roll);
	void DrawSkyRows(int firstRow, int endRow);
	void BlendBlurBuffer(uint8_t* screen, const uint8_t* buffer, bool swapFirstGroup);
	void SmoothViewPort();
	enum class TerrainProjection { Plain, Reflections, Cave };

	void DrawTerrainAndParticles_3C080(__int16 posX, __int16 posY, __int16 yaw, signed int posZ, int pitch, int16_t roll, int fov);
	void ProjectTiles(TerrainProjection projection, uint8_t mapX, uint8_t mapY, const uint8_t* steps, int quadrant, int posZ);
	void RotateTiles(bool secondPoints);
	void ForEachTileFarToNear(const std::function<void(int)>& drawTile);
	void LoadTileQuad(TerrainVertex quad[4], int tile, bool secondPoints, uint8_t& anyFlags, uint8_t& allFlags);
	void SetTileShading(const type_E9C38_smalltit& tile, const TerrainVertex quad[4]);
	void SetTileTexture(TerrainVertex quad[4], const type_E9C38_smalltit& tile, int textureIndex);
	void DrawTileTriangles(TerrainVertex quad[4], const type_E9C38_smalltit& tile, bool upsideDown);
	void DrawCaveTile(TerrainVertex quad[4], int index);
	void DrawReflectedTile(TerrainVertex quad[4], int index);
	void DrawTerrainTile(TerrainVertex quad[4], int index, bool water);
	void DrawSprite_41BD3(uint32 pass);
	void DrawRotatedSprite(uint32 pass);
	void DrawUprightSprite(uint32 pass);
	void DrawSpriteSpan(const int8_t* texels, const SpriteColumn* columns, uint8_t* screen, const type_unk_F0E20x* steps, int length, int lastMode);
	enum class SpriteChoice { Draw, Skip, KeepPrevious };
	void DrawSprites_3E360(int tileIndex);
	void DrawEntity(type_entity_0x6E8E* entity, const type_E9C38_smalltit& tile, Type_Sprite*& sprite);
	void EntityViewPosition(const type_entity_0x6E8E* entity, const type_E9C38_smalltit& tile, int& across, int& depth) const;
	int SpriteLight(int64_t distance);
	void ProjectSprite(int across, int height, int depth);
	Type_Sprite* UseSprite(int index);
	SpriteChoice ChooseSprite(type_entity_0x6E8E* entity, const Type_SpriteDef_D951C& def, int depth, int64_t distance, bool rotatedKind21, bool turnTable, Type_Sprite*& sprite);
	void DrawTriangleInProjectionSpace_B6253(const TerrainVertex& vertex1, const TerrainVertex& vertex2, const TerrainVertex& vertex3);
	void DrawClippedTriangle(const TerrainVertex& vertex1, const TerrainVertex& vertex2, const TerrainVertex& vertex3);
	void DrawTriangle(const TerrainVertex& vertex1, const TerrainVertex& vertex2, const TerrainVertex& vertex3, int mode, const uint8_t* texture,
		int flatLight, int bandTop, int bandBottom);
	template <int Mode>
	void DrawTriangleRow(uint8_t* line, int left, int right, int u, int v, int light, int du, int dv, int dl, const uint8_t* texture, int flatLight);
	void sub_3FD60(int tileIndex);
	void sub_88740(type_entity_0x6E8E* entity, int16_t posX, int16_t posY);
	void SetBillboards_3B560(int16_t roll);
	void DrawSorcererNameAndHealthBar_2CB30(type_entity_0x6E8E* entity, int16_t screenX, int screenY, int16_t spriteWidth);

public:
	GameRenderNG(uint8_t viewDistanceScale = 1, float fogScale = 1.0f, uint8_t renderThreads = 0, bool assignToSpecificCores = false, float sizePercentToThreadRender = 10.0f);
	void SetRenderThreads(uint8_t renderThreads);
	uint8_t GetRenderThreads() const;
	~GameRenderNG();
	void DrawWorld_411A0(int posX, int posY, int16_t yaw, int16_t posZ, int16_t pitch, int16_t roll, int16_t fov);
};

#endif //GAME_RENDER_NG