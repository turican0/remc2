#include "GameRenderHW.h"
#include <algorithm>
#include "globals.h"

#include "../utilities/RendererTests.h"

static constexpr float kBrightnessMax = 4161536.0f;   // 63.5 * 65536, the observed maximum
static constexpr float kBrightnessNeutral = 32.0f * 65536.0f;   // row 32 = fully lit, same as sprites

GameRenderHW::GameRenderHW(uint8_t* ptrScreenBuffer, uint8_t* pColorPalette, uint8_t viewDistanceScale, float fogScale) :
	m_ptrScreenBuffer_351628(ptrScreenBuffer), m_ptrColorPalette(pColorPalette), m_assignToSpecificCores(assignToSpecificCores)
{
	pdwScreenAlphaBuffer = new uint8_t[GAME_RES_MAX_WIDTH * GAME_RES_MAX_HEIGHT];

	m_viewDistanceScale = viewDistanceScale;

	{
		const int64_t scale = m_viewDistanceScale;
		const int64_t cutOff = 400 * scale * scale;
		int64_t fogEnd = cutOff - 39 * scale * scale;
		int64_t fogStart = cutOff - 175 * scale * scale;
		if (fogScale > 0 && fogScale != 1.0f)
		{
			const double fogScale2 = static_cast<double>(fogScale) * fogScale;
			fogEnd = std::llround(fogEnd * fogScale2);
			if (fogEnd > cutOff)
				fogEnd = cutOff;
			fogStart = std::llround(fogStart * fogScale2);
			if (fogStart > fogEnd - 1)
				fogStart = fogEnd - 1;
		}
		m_cutOffDistance = cutOff << 16;
		m_fogEnd = fogEnd << 16;
		m_fogStart = fogStart << 16;
	}

	m_tileRows = TILE_ROWS_COUNT * viewDistanceScale;
	m_tileColumns = TILE_COLUMNS_COUNT * viewDistanceScale;

	m_ptrStr_E9C38_smalltit = new type_E9C38_smalltit[m_tileRows * m_tileColumns];

	m_tileRenderStepTable_D4328x = new TileStepQuadrant[4]{
		// Quadrant 0 (270->0)
		{ 0xED, 0x01, 0x00, 0x00, 0x00, 0xFF, 0xD8, 0xFF, 0x01, 0x00 },
		// Quadrant 1 (0->90)
		{ 0x00, 0xED, 0xFF, 0x00, 0x01, 0x00, 0x01, 0xD8, 0x00, 0x01 },
		// Quadrant 2 (90->180)
		{ 0x13, 0x00, 0xFF, 0xFF, 0x00, 0x01, 0x28, 0x01, 0xFF, 0x00 },
		// Quadrant 3 (180->270)
		{ 0x01, 0x13, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x28, 0x00, 0xFF }
	};

	if (m_tileColumns > 40)
	{
		BuildTileRenderStepTable(m_tileRenderStepTable_D4328x, m_tileColumns);
	}
}

GameRenderHW::~GameRenderHW()
{
	SetBlurActive(false);
	EventDispatcher::I->DispatchEvent<ResourceType, RenderViews*>(EventType::E_RESOURCE_CHANGE, ResourceType::POLYGONS_DISPOSED, nullptr);
	EventDispatcher::I->DispatchEvent<ResourceType, uint32_t, uint8_t*, uint32_t, uint32_t>(EventType::E_RESOURCE_CHANGE, ResourceType::TEXTURE_DISPOSED, kTerrainAtlasId, NULL, 0, 0);
	EventDispatcher::I->DispatchEvent<ResourceType, uint32_t, uint8_t*, uint32_t, uint32_t>(EventType::E_RESOURCE_CHANGE, ResourceType::TEXTURE_DISPOSED, kSkyId, NULL, 0, 0);

	// Sprite textures are uploaded by the level loader (TextureMaps.cpp) under their sprite index
	for (uint32_t spriteId = 0; spriteId < m_spriteFrameCount.size(); spriteId++)
		EventDispatcher::I->DispatchEvent<ResourceType, uint32_t, uint8_t*, uint32_t, uint32_t>(EventType::E_RESOURCE_CHANGE, ResourceType::TEXTURE_DISPOSED, spriteId, NULL, 0, 0);

	delete[] m_ptrStr_E9C38_smalltit;
	delete[] pdwScreenAlphaBuffer;
	pdwScreenAlphaBuffer = nullptr;
}

void GameRenderHW::BuildTileRenderStepTable(TileStepQuadrant* table, int cols)
{
	uint8_t pos_cols = (uint8_t)cols;
	uint8_t neg_cols = (uint8_t)(256 - cols);

	uint8_t neg_half = (uint8_t)(0xED - (cols - 40) / 2);  // ~-cols/2
	uint8_t pos_half = (uint8_t)(0x13 + (cols - 40) / 2);  // ~+cols/2

	// Quadrant 0 (270->0)
	table[0].startX = neg_half + (m_viewDistanceScale - 1);
	table[0].startY = table[0].startY + (m_viewDistanceScale - 1);
	table[0].rowStepX = neg_cols;
	table[0].rowStepY = 0xFF;
	table[0].colStepX = 0x01;
	table[0].colStepY = 0x00;

	// Quadrant 1 (0->90)
	table[1].startY = neg_half + (m_viewDistanceScale - 1);
	table[1].rowStepX = 0x01;
	table[1].rowStepY = neg_cols;
	table[1].colStepX = 0x00;
	table[1].colStepY = 0x01;

	// Quadrant 2 (90->180)
	table[2].startX = pos_half - (m_viewDistanceScale - 1);
	table[2].startY = table[2].startY;
	table[2].rowStepX = pos_cols;
	table[2].rowStepY = 0x01;
	table[2].colStepX = 0xFF;
	table[2].colStepY = 0x00;

	// Quadrant 3 (180->270)
	table[3].startX = table[3].startX + (m_viewDistanceScale - 1);
	table[3].startY = pos_half - (m_viewDistanceScale - 1);
	table[3].rowStepX = 0xFF;
	table[3].rowStepY = pos_cols;
	table[3].colStepX = 0x00;
	table[3].colStepY = 0xFF;
}

void GameRenderHW::SetBlurActive(bool active)
{
	if (m_blurActive == active)
		return;

	m_blurActive = active;
	EventDispatcher::I->DispatchEvent<bool>(EventType::E_BLUR_EFFECT_CHANGE, active);
}

void GameRenderHW::DrawWorld_411A0(int posX, int posY, int16_t yaw, int16_t posZ, int16_t pitch, int16_t roll, int16_t fov)
{
	uint16_t v8; // ax
	int v9; // ecx
	int v10; // ebx
	int v11; // edx
	int v12; // ecx
	int v13; // ebx
	int v14; // edx
	int v15; // ecx
	int v16; // ebx
	int v17; // edx
	int v18; // ecx
	int v19; // ebx
	int v20; // edx
	int vYaw; // esi
	int v22; // edx
	int v23; // ebx
	uint32_t v24; // edx
	int v25; // ebx
	int v26; // edi
	int v28; // ebx
	uint32_t v29; // edx
	int v30; // ebx
	int v31; // edi
	int vPosX; // [esp+34h] [ebp+18h]
	int vPosY; // [esp+38h] [ebp+1Ch]
	LOBYTE(v8) = HIBYTE(posX);
	HIBYTE(v8) = HIBYTE(posY);
	if ((signed int)(uint8_t)posX < 128)
		LOBYTE(v8) = HIBYTE(posX) - 1;
	if ((signed int)(uint8_t)posY < 128)
		HIBYTE(v8) = HIBYTE(posY) - 1;
	v9 = mapHeightmap_11B4E0[v8];
	LOBYTE(v8) += 2;
	v10 = v9;
	v11 = v9;
	v12 = mapHeightmap_11B4E0[v8];
	HIBYTE(v8) += 2;
	v13 = v10 - v12;
	v14 = v12 + v11;
	v15 = mapHeightmap_11B4E0[v8];
	LOBYTE(v8) -= 2;
	v16 = v13 - v15;
	v17 = v14 - v15;
	v18 = mapHeightmap_11B4E0[v8];
	v19 = 2 * (v18 + v16);
	v20 = 2 * (v17 - v18);
	if (v19 <= 100)
	{
		if (v19 < -100)
			v19 = -100;
	}
	else
	{
		v19 = 100;
	}
	if (v20 <= 100)
	{
		if (v20 < -100)
			v20 = -100;
	}
	else
	{
		v20 = 100;
	}
	vYaw = yaw & 0x7FF;
	viewOffsetX_D4794 += (v19 - viewOffsetX_D4794) >> 3;
	viewOffsetY_D4798 += (v20 - viewOffsetY_D4798) >> 3;
	vPosX = viewOffsetX_D4794 + posX;
	vPosY = viewOffsetY_D4798 + posY;

	// Anaglyph (F11): the two eyes combined red / cyan. VR: the eyes side by side (1 x 2). The plain mode: the view grid
	// (1 x 1, or a Looking Glass quilt). The original's red/blue stereo mode still draws both eyes into one list (the GPU
	// shows the last)
	m_primaryView = true;
	m_frameViews.composition = ViewComposition::Cells;

	if (anaglyphMode)
	{
		// The original stereo mode's eyes (sideways along the camera's right axis, the projection centre shifted
		// 1/40 of the screen), each into its own list: the renderer combines them through the glasses' colours
		m_frameViews.rows = 1;
		m_frameViews.columns = 1;
		m_frameViews.composition = ViewComposition::Anaglyph;
		m_frameViews.darkLevel = world_D41A0.terrain_2FECE.MapType != MapType_t::Day;   // half-colour, so the dark scene reaches the left eye
		m_frameViews.lists.resize(2);
		SetBlurActive(false);
		v28 = 5 * Maths::sin_DB750[vYaw];
		v29 = Maths::sin_DB750[512 + vYaw];
		v30 = 4 * v28 >> 16;
		v31 = 20 * (signed int)v29 >> 16;
		viewShiftX_D4324 = 0 - (iScreenWidth_DE560 / 40);
		DrawTerrainAndParticles_3C080(vPosX - v31, vPosY - v30, vYaw, posZ, pitch, roll, fov, &m_frameViews.lists[0]);   // left
		m_primaryView = false;   // the HUD's names and health bars once, from the left eye
		viewShiftX_D4324 = iScreenWidth_DE560 / 40;
		DrawTerrainAndParticles_3C080(v31 + vPosX, v30 + vPosY, vYaw, posZ, pitch, roll, fov, &m_frameViews.lists[1]);   // right
		m_primaryView = true;
		viewShiftX_D4324 = 0;
	}
	else if (world_D41A0.m_GameSettings.graphics_0x2192.xxxx_0x2193 && world_D41A0.m_GameSettings.m_Display.m_uiScreenSize && screenWidth_18062C == 640)
	{
		//VR interlaced render: each eye is projected into half the screen and goes into its own cell, left then right
		m_frameViews.rows = 1;
		m_frameViews.columns = 2;
		m_frameViews.lists.resize(2);
		SetBlurActive(false);
		viewPort.SetRenderViewPortSize_BCD45(
			m_ptrScreenBuffer_351628,
			2 * screenWidth_18062C,
			screenWidth_18062C / 2 - 8,
			screenHeight_180624 / 2 - 40);
		v22 = Maths::sin_DB750[vYaw];
		v23 = 5 * v22;
		v24 = Maths::sin_DB750[512 + vYaw];
		viewShiftX_D4324 = -5;
		v25 = 4 * v23 >> 16;
		v26 = 20 * (signed int)v24 >> 16;
		DrawTerrainAndParticles_3C080(vPosX - v26, vPosY - v25, vYaw, posZ, pitch, roll, fov, &m_frameViews.lists[0]);
		viewPort.SetRenderViewPortSize_BCD45(m_ptrScreenBuffer_351628 + (screenWidth_18062C / 2), 0, 0, 0);
		viewShiftX_D4324 = 5;
		DrawTerrainAndParticles_3C080(vPosX + v26, vPosY + v25, vYaw, posZ, pitch, roll, fov, &m_frameViews.lists[1]);
		viewShiftX_D4324 = 0;
		viewPort.SetRenderViewPortSize_BCD45(m_ptrScreenBuffer_351628, screenWidth_18062C, screenWidth_18062C, screenHeight_180624);
	}
	else if (world_D41A0.m_GameSettings.m_Display.m_uiScreenSize != 1 || world_D41A0.m_GameSettings.graphics_0x2192.xxxx_0x2193)
	{
		bool blurActive = world_D41A0.m_GameSettings.m_Display.xxxx_0x2191;
		if (videoMode_180660 == 1
			&& !world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221
			&& gameState_D41A4.m_wHighSpeedSystem
			&& world_D41A0.m_GameSettings.m_Graphics.m_wViewPortSize == 40)
		{
			// Motion blur when the player is moving fast
			int32_t speed = Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240]->actSpeed_0x82_130;
			if (std::abs(speed) > 80)
				blurActive = true;
		}
		SetBlurActive(world_D41A0.miscSettings_0x21AE.xxxx_0x21B1 && blurActive);

		// The views step sideways along the camera's right axis (as the stereo eyes), centred on the camera, the leftmost
		// first: view i is (2i - (count - 1)) / 2 steps of viewSpacing from the centre
		const int viewCount = viewRows * viewColumns;
		const int primary = (viewCount - 1) / 2;
		const int rightX = Maths::sin_DB750[512 + vYaw];
		const int rightY = Maths::sin_DB750[vYaw];
		m_frameViews.rows = viewRows;
		m_frameViews.columns = viewColumns;
		m_frameViews.lists.resize(viewCount);
		for (int view = 0; view < viewCount; view++)
		{
			const int offset = (2 * view - (viewCount - 1)) * viewSpacing / 2;
			m_primaryView = view == primary;
			m_viewOffset = offset;
			DrawTerrainAndParticles_3C080(vPosX + (offset * rightX >> 16), vPosY + (offset * rightY >> 16),
				vYaw, posZ, pitch, roll, fov, &m_frameViews.lists[view]);
		}
		m_primaryView = true;
		m_viewOffset = 0;
	}
	else
	{
		m_frameViews.rows = 1;
		m_frameViews.columns = 1;
		m_frameViews.lists.resize(1);
		SetBlurActive(false);
		v28 = 5 * Maths::sin_DB750[vYaw];
		v29 = Maths::sin_DB750[512 + vYaw];
		viewShiftX_D4324 = iScreenWidth_DE560 / 40;
		v30 = 4 * v28 >> 16;
		v31 = 20 * (signed int)v29 >> 16;
		DrawTerrainAndParticles_3C080(v31 + vPosX, v30 + vPosY, vYaw, posZ, pitch, roll, fov, &m_frameViews.lists[0]);
		viewShiftX_D4324 = 0 - (iScreenWidth_DE560 / 40);
		DrawTerrainAndParticles_3C080(vPosX - v31, vPosY - v30, vYaw, posZ, pitch, roll, fov, &m_frameViews.lists[0]);
		viewShiftX_D4324 = 0;
	}

	EventDispatcher::I->DispatchEvent<ResourceType, RenderViews*>(EventType::E_RESOURCE_CHANGE, ResourceType::POLYGONS_UPDATED, &m_frameViews);
}

/*
* Sky texture is currently 256x256
*/
void GameRenderHW::DrawSky_40950(
	int16_t roll,
	std::vector<RenderPolygon>* polygons)
{
	if (!polygons ||
		viewPort.Width_DE564 <= 0 ||
		viewPort.Height_DE568 <= 0)
		return;

	constexpr float kSkySize = 256.0f;
	constexpr float kFixedOne = 65536.0f;

	const float width = (float)viewPort.Width_DE564;
	const float height = (float)viewPort.Height_DE568;

	const int roundRoll = roll & 0x7FF;
	const int sinRoll =
		(Maths::sin_DB750[roundRoll] * 256) /
		viewPort.Width_DE564;
	const int cosRoll =
		(Maths::sin_DB750[512 + roundRoll] * 256) /
		viewPort.Width_DE564;

	const int addX =
		(-(spriteRender_F2C20.sin_0x0d * spriteRender_F2C20.horizonOffset_0x22) >> 16)
		+ spriteRender_F2C20.centerX_0x24;

	const int addY =
		spriteRender_F2C20.centerY_0x10 -
		(spriteRender_F2C20.cos_0x11 * spriteRender_F2C20.horizonOffset_0x22 >> 16);

	// Preserve the original software renderer's starting offsets.
	const uint32_t beginX =
		(yaw_F2CC0 << 15) -
		(addX * cosRoll - addY * sinRoll);

	const uint32_t beginY =
		-(cosRoll * addY + sinRoll * addX);

	// BYTE2() in the original renderer: the high byte is the
	// initial texel coordinate for the 256x256 texture.
	const float baseU =
		(float)((beginX >> 16) & 0xFF) / kSkySize;
	const float baseV =
		(float)((beginY >> 16) & 0xFF) / kSkySize;

	// Texture-space movement per screen pixel.
	const float duDx = (float)cosRoll / kFixedOne / kSkySize;
	const float dvDx = (float)sinRoll / kFixedOne / kSkySize;
	const float duDy = (float)-sinRoll / kFixedOne / kSkySize;
	const float dvDy = (float)cosRoll / kFixedOne / kSkySize;

	auto makeVertex = [&](float x, float y) {
		HWVertex v{};
		v.X = x / width * 2.0f - 1.0f;
		v.Y = y / height * 2.0f - 1.0f;
		v.W = 1.0f;

		v.U = baseU + x * duDx + y * duDy;
		v.V = baseV + x * dvDx + y * dvDy;
		v.Shade = 1.0f;

		return v;
		};

	RenderPolygon sky{};
	sky.TextureId = kSkyId;
	sky.SpriteMode = 0xFE;
	sky.Vertices = {
		makeVertex(0.0f,  0.0f),
		makeVertex(width, 0.0f),
		makeVertex(width, height),
		makeVertex(0.0f,  height)
	};

	polygons->push_back(std::move(sky));
}
/*
* Draws Terrain, Sprites and Particals using a Painter's algorithm.
*/
void GameRenderHW::DrawTerrainAndParticles_3C080(__int16 posX, __int16 posY, __int16 yaw, signed int posZ, int pitch, int16_t roll, int fov,
	std::vector<RenderPolygon>* polygons)
{
	std::vector<int> projectedVertexBuffer(24);   // the corners of the tile quad being drawn (LoadTileQuad), 6 values each

	for (int i = 0; i < (m_tileRows * m_tileColumns); i++)
		memset(&m_ptrStr_E9C38_smalltit[i], 0, sizeof(type_E9C38_smalltit));

	shadows_F2CC7 = world_D41A0.m_GameSettings.m_Graphics.m_wShadows;//21d080
	notDay_D4320 = world_D41A0.terrain_2FECE.MapType != MapType_t::Day;
	spriteRender_F2C20.centerY_0x10 = (signed int)(uint16_t)viewPort.Height_DE568 >> 1;
	cameraX_F2CC4 = posX;
	yaw_F2CC0 = yaw & 0x7FF;
	cameraY_F2CC2 = posY;
	const int yaw90 = (yaw & 0x7FF) + 256;
	spriteRender_F2C20.cameraZ_0x20 = posZ;
	spriteRender_F2C20.centerX_0x24 = viewShiftX_D4324 + ((signed int)(uint16_t)viewPort.Width_DE564 >> 1);
	spriteRender_F2C20.cos2_0x0f = Maths::sin_DB750[256 + yaw90];
	spriteRender_F2C20.sin2_0x17 = Maths::sin_DB750[yaw90 - 256];

	// The map is walked in the quadrant the camera looks into; the yaw within it lays out the tile grid
	const uint8_t quadrant = (yaw90 >> 9) & 3;
	const int quadrantYaw = ((yaw90 & 0x1FF) - 256) & 0x7FF;
	const int quadrantCos = Maths::sin_DB750[512 + quadrantYaw];
	const int quadrantSin = Maths::sin_DB750[quadrantYaw];
	spriteRender_F2C20.projectionScale_0x18 = 7
		* Maths::IntegerSqrt_7277A(
			(uint16_t)viewPort.Width_DE564 * (uint16_t)viewPort.Width_DE564
			+ (uint16_t)viewPort.Height_DE568 * (uint16_t)viewPort.Height_DE568)
		* fov >> 11;
	const TileStepQuadrant& steps = m_tileRenderStepTable_D4328x[quadrant];

	int a1 = 0;
	int a2 = 0;
	//This is based on rotation direction there is always a direction
	switch (quadrant)
	{
	case 0u: // 270 -> 0
		a2 = (uint8_t)posY - 256 * m_viewDistanceScale;
		a1 = -(uint8_t)posX - 4864 * m_viewDistanceScale;
		break;
	case 1u: // 0 -> 90
		a1 = -(uint8_t)posY - 4864 * m_viewDistanceScale;
		a2 = -(uint8_t)posX;
		break;
	case 2u: // 90 -> 180
		a1 = (uint8_t)posX - 4864 * m_viewDistanceScale;
		a2 = -(uint8_t)posY;
		break;
	case 3u: // 180 -> 270
		a1 = (uint8_t)posY - 4864 * m_viewDistanceScale;
		a2 = (uint8_t)posX - 256 * m_viewDistanceScale;
		break;
	}

	Logger->trace("------DrawTerrainAndParticles_3C080: {}-------", viewPort.Width_DE564);
	// The tiles' positions in the view space: across the columns, then along the rows
	for (int column = 0; column < m_tileColumns; column++, a1 += 256)
	{
		const int x = a1 * quadrantCos >> 16;
		const int y = a1 * quadrantSin >> 16;
		for (int row = 0; row < m_tileRows; row++)
		{
			type_E9C38_smalltit& tile = m_ptrStr_E9C38_smalltit[row * m_tileColumns + column];
			tile.x_0 = x;
			tile.y_12 = y;
			tile.triangleFeatures_38 = a1 < 0 ? 0 : 4;
		}
	}
	for (int row = 0; row < m_tileRows; row++, a2 += 256)
	{
		const int x = a2 * quadrantSin >> 16;
		const int y = a2 * quadrantCos >> 16;
		for (int column = 0; column < m_tileColumns; column++)
		{
			type_E9C38_smalltit& tile = m_ptrStr_E9C38_smalltit[row * m_tileColumns + column];
			tile.x_0 -= x;
			tile.y_12 += y;
		}
	}

	// Cut-off and fog distances come from the members (64 bit, see the constructor)
	spriteRender_F2C20.tileCutOffDistance_0x15 = static_cast<int>(m_cutOffDistance); //Distance cut-off for tile render
	spriteRender_F2C20.fogThickness_0x12 = static_cast<int>(m_fogEnd - m_fogStart);
	spriteRender_F2C20.horizonOffset_0x22 = pitch * (uint16_t)viewPort.Width_DE564 >> 8;
	const int rollIndex = roll & 0x7FF;
	spriteRender_F2C20.cos_0x11 = Maths::sin_DB750[512 + rollIndex];
	spriteRender_F2C20.sin_0x0d = Maths::sin_DB750[rollIndex];

	// Convergence (Looking Glass views): an off-axis shear rather than toeing the cameras in. A point at depth d moves
	// projectionScale * offset / d across the screen between this view and the centre one, so shifting the projection
	// centre by projectionScale * offset / viewConvergence cancels it at the convergence distance: that plane sits at the
	// display surface, nearer pops out and further sinks in. The shift is across the camera's (rolled) screen.
	if (m_viewOffset != 0 && viewConvergence > 0)
	{
		const int64_t shift = (int64_t)spriteRender_F2C20.projectionScale_0x18 * m_viewOffset / viewConvergence;
		spriteRender_F2C20.centerX_0x24 += (int)(shift * spriteRender_F2C20.cos_0x11 >> 16);
		spriteRender_F2C20.centerY_0x10 -= (int)(shift * spriteRender_F2C20.sin_0x0d >> 16);
	}
	spriteRender_F2C20.fogEnd_0x16 = static_cast<int>(m_fogEnd);
	spriteRender_F2C20.fogStart_0x13 = static_cast<int>(m_fogStart);

	polygons->clear();
	polygons->reserve(1 + 2 * (size_t)m_tileRows * m_tileColumns + kReservedSprites);

	if (!world_D41A0.m_GameSettings.m_Graphics.m_wSky || isCaveLevel_D41B6)
	{
		// The background colour over the view port, 4 bytes at a time (as the original, the width rounded down to 4)
		const uint8_t colour = m_BackgroundColour_D4B7C;
		const int fill = colour | colour << 8 | colour << 16 | colour << 24;
		const int filled = ((uint16_t)viewPort.Width_DE564 / 4) * 4;
		uint8_t* line = ViewPortRenderBufferStart_DE558;
		for (int row = (uint16_t)viewPort.Height_DE568; row; row--, line += filled + iScreenWidth_DE560 - viewPort.Width_DE564)
			memset32(line, fill, filled);
	}
	else
	{
		DrawSky_40950(roll, polygons);
	}

	const uint8_t mapX = steps.startX + HIBYTE(posX);
	const uint8_t mapY = steps.startY + HIBYTE(posY);
	if (isCaveLevel_D41B6)//21d3e3
	{
		ProjectTiles(TerrainProjection::Cave, mapX, mapY, steps, quadrant, posZ);
		RotateTiles(true);
		SubDrawCaveTerrainAndParticles(projectedVertexBuffer, polygons);
	}
	else if (world_D41A0.m_GameSettings.m_Graphics.m_wReflections)
	{
		Logger->trace("Start Drawing Terrain Frame with Reflection");
		ProjectTiles(TerrainProjection::Reflections, mapX, mapY, steps, quadrant, posZ);
		RotateTiles(true);

		// As the original (and NG): no reflection from high up, where the sea is blended over the sky instead
		if (posZ < 4096)
		{
			m_reflectionPass = true;
			SubDrawInverseTerrainAndParticles(projectedVertexBuffer, polygons);
			m_reflectionPass = false;
		}
		SubDrawTerrainAndParticles(projectedVertexBuffer, polygons);
		Logger->trace("Finished Drawing Terrain Frame with Reflection");
	}
	else
	{
		ProjectTiles(TerrainProjection::Plain, mapX, mapY, steps, quadrant, posZ);//21eb44
		RotateTiles(false);
		SubDrawTerrainAndParticles(projectedVertexBuffer, polygons);
	}
}

// The tiles in view space projected to the screen (pnt1/pnt2; for the cave ceiling and the reflection also pnt3/pnt4 from
// inverse_alt_8), lit with fog (pnt5) and textured. The map is walked from (mapX, mapY) along the quadrant's steps, each
// byte of the position wrapping on its own as in the original.
void GameRenderHW::ProjectTiles(TerrainProjection projection, uint8_t mapX, uint8_t mapY, const TileStepQuadrant& steps, uint8_t quadrant, int posZ)
{
	int tileIndex = 0;
	for (int row = 0; row < m_tileRows; row++, mapX += steps.rowStepX, mapY += steps.rowStepY)
	{
		for (int column = 0; column < m_tileColumns; column++, tileIndex++, mapX += steps.colStepX, mapY += steps.colStepY)
		{
			type_E9C38_smalltit& tile = m_ptrStr_E9C38_smalltit[tileIndex];
			const uint16_t map = mapY << 8 | mapX;
			const int shading = ((uint8_t)mapShading_12B4E0[map] << 8) + 128;
			int depth = tile.y_12;
			const int64_t distance = (int64_t)depth * depth + (int64_t)tile.x_0 * tile.x_0;
			tile.haveBillboard_36 = 0;
			if (depth <= -256 || distance >= m_cutOffDistance)
			{
				tile.triangleFeatures_38 |= 2u;
			}
			else
			{
				if (depth < 128)
					depth = 128;
				tile.pnt1_16 = spriteRender_F2C20.projectionScale_0x18 * tile.x_0 / depth;
				tile.alt_4 = 32 * mapHeightmap_11B4E0[map] - posZ;

				// The waves of the water, which also bend its reflection; the cave has the ceiling instead
				int wave = 0;
				if (projection == TerrainProjection::Cave)
				{
					tile.inverse_alt_8 = ((uint8_t)secondHeightmap_14B4E0[map] << 15 >> 10) - posZ;
					if (!mapTerrainType_10B4E0[map])
					{
						const int turn = 32 * world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].Turn_2BE0_11248;
						wave = (Maths::sin_DB750[(turn + (mapY << 7)) & 0x7FF] >> 8) * (Maths::sin_DB750[((mapX << 7) + turn) & 0x7FF] >> 8);
						tile.alt_4 -= wave >> 13;
						if (shading >= 14464)
							wave = 0;
					}
					if (mapAngle_13B4E0[map] & 8)
						tile.triangleFeatures_38 |= 0x80u;
				}
				else
				{
					const int tick = (uint16_t)world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].Turn_2BE0_11248 << 6;
					wave = (Maths::sin_DB750[(tick + (mapY << 7)) & 0x7FF] >> 8) * (Maths::sin_DB750[(tick + (mapX << 7)) & 0x7FF] >> 8);
					if (projection == TerrainProjection::Reflections)
						tile.inverse_alt_8 = -(mapHeightmap_11B4E0[map] * ((wave >> 4) + 0x8000) >> 10) - posZ;
					if (!(mapAngle_13B4E0[map] & 8) || (tile.alt_4 -= wave >> 10, shading >= 14464))
						wave = 0;
				}
				tile.pnt5_32 = FogLight((shading << 8) + 8 * wave, distance);
				tile.pnt2_20 = spriteRender_F2C20.horizonOffset_0x22 + spriteRender_F2C20.projectionScale_0x18 * tile.alt_4 / depth;
				if (projection != TerrainProjection::Plain)
					tile.pnt4_28 = spriteRender_F2C20.horizonOffset_0x22 + spriteRender_F2C20.projectionScale_0x18 * tile.inverse_alt_8 / depth;

				const uint8_t textureX = mapX + steps.texOffX;
				const uint8_t textureY = mapY + steps.texOffY;
				const uint16_t textureMap = textureY << 8 | textureX;
				tile.textIndex_41 = mapTerrainType_10B4E0[textureMap];
				if (projection == TerrainProjection::Reflections && Maths::x_BYTE_D41D8[164 + tile.textIndex_41])
					tile.triangleFeatures_38 |= 0x80u;   // water
				tile.textAtyp_43 = Maths::x_BYTE_D41D8[tile.textIndex_41];
				tile.textUV_42 = quadrant + (((signed int)(uint8_t)mapAngle_13B4E0[textureMap] >> 2) & 0x1C);
				const uint8_t entityX = textureX + steps.entOffX;
				const uint8_t entityY = textureY + steps.entOffY;
				tile.haveBillboard_36 = mapEntityIndex_15B4E0[entityY << 8 | entityX];
			}
			tile.triangleFeatures_38 |= (mapX + mapY) & 1;   // the diagonal
		}
	}
}

// The tiles rotated by the roll, with the flags of the points off the view port (left 8, right 0x10, above 0x20,
// below 0x40; for pnt3/pnt4 the same << 5)
void GameRenderHW::RotateTiles(bool secondPoints)
{
	const int width = (uint16_t)viewPort.Width_DE564;
	const int height = (uint16_t)viewPort.Height_DE568;
	const auto outside = [](int value, int size, uint16_t below, uint16_t beyond) -> uint16_t {
		return value < 0 ? below : (size <= value ? beyond : 0);
		};
	for (int i = 0; i < m_tileRows * m_tileColumns; i++)
	{
		type_E9C38_smalltit& tile = m_ptrStr_E9C38_smalltit[i];
		const int x = tile.pnt1_16;
		tile.pnt1_16 = CalculateRotationTranslationX(spriteRender_F2C20.cos_0x11, x, spriteRender_F2C20.sin_0x0d, tile.pnt2_20);
		tile.pnt2_20 = CalculateRotationTranslationY(x, spriteRender_F2C20.sin_0x0d, spriteRender_F2C20.cos_0x11, tile.pnt2_20);
		tile.triangleFeatures_38 |= outside(tile.pnt1_16, width, 8u, 0x10u) | outside(tile.pnt2_20, height, 0x20u, 0x40u);
		if (secondPoints)
		{
			tile.pnt3_24 = CalculateRotationTranslationX(spriteRender_F2C20.cos_0x11, x, spriteRender_F2C20.sin_0x0d, tile.pnt4_28);
			tile.pnt4_28 = CalculateRotationTranslationY(x, spriteRender_F2C20.sin_0x0d, spriteRender_F2C20.cos_0x11, tile.pnt4_28);
			tile.triangleFeatures_38 |= outside(tile.pnt3_24, width, 0x100u, 0x200u) | outside(tile.pnt4_28, height, 0x400u, 0x800u);
		}
	}
}

int32_t GameRenderHW::CalculateRotationTranslationX(int64_t cos_0x11, int64_t pnt1, int64_t sin_0x0d, int64_t pnt2)
{
	int64_t rotation = ((cos_0x11 * pnt1 - sin_0x0d * pnt2) >> 16);
	return rotation + spriteRender_F2C20.centerX_0x24;
}

int32_t GameRenderHW::CalculateRotationTranslationY(int64_t pnt1, int64_t sin_0x0d, int64_t cos_0x11, int64_t pnt2)
{
	int64_t rotation = ((pnt1 * sin_0x0d + cos_0x11 * pnt2) >> 16);
	return spriteRender_F2C20.centerY_0x10 - rotation;
}

// The tiles far to near, as the original draws them: the rows from the back, each from the left up to the first tile whose
// right neighbour is behind the camera (flag 4), then from the right back to that tile.
template<typename DrawTile>
void GameRenderHW::ForEachTileFarToNear(DrawTile drawTile)
{
	for (int rowStart = (m_tileRows - 1) * m_tileColumns; rowStart > 0; rowStart -= m_tileColumns)
	{
		int index = rowStart;
		int remaining = m_tileColumns - 1;
		for (; remaining; remaining--, index++)
		{
			if (m_ptrStr_E9C38_smalltit[index + 1].triangleFeatures_38 & 4)
				break;
			drawTile(index);
		}
		if (remaining)
		{
			for (int i = rowStart + m_tileColumns - 2; i >= index; i--)
				drawTile(i);
		}
	}
}

// The quad of the tile into vertexs (6 values a corner): 18 the tile, 12 its right neighbour, 6 the tile behind that, 0 the
// tile behind it; pnt1/pnt2 (the terrain) or pnt3/pnt4 (the cave ceiling, the reflection), with the light pnt5.
// Returns the corners' flags (low byte) OR-ed and AND-ed.
GameRenderHW::TileQuadFlags GameRenderHW::LoadTileQuad(std::vector<int>& vertexs, int tileIndex, bool secondPoints)
{
	const int corners[4][2] = {
		{ 18, tileIndex }, { 12, tileIndex + 1 }, { 6, tileIndex + 1 - m_tileColumns }, { 0, tileIndex - m_tileColumns } };
	TileQuadFlags flags{ 0, 0xFF };
	for (const auto& corner : corners)
	{
		const type_E9C38_smalltit& tile = m_ptrStr_E9C38_smalltit[corner[1]];
		vertexs[corner[0]] = secondPoints ? tile.pnt3_24 : tile.pnt1_16;
		vertexs[corner[0] + 1] = secondPoints ? tile.pnt4_28 : tile.pnt2_20;
		vertexs[corner[0] + 4] = tile.pnt5_32;
		flags.any |= (uint8_t)tile.triangleFeatures_38;
		flags.all &= (uint8_t)tile.triangleFeatures_38;
	}
	return flags;
}

// Cave: the ceiling (texture 1, pnt3/pnt4) and the floor with its sprites; nothing where all corners have flag 0x80
void GameRenderHW::SubDrawCaveTerrainAndParticles(std::vector<int>& vertexs, std::vector<RenderPolygon>* polygons)
{
	ForEachTileFarToNear([&](int index) {
		TileQuadFlags flags = LoadTileQuad(vertexs, index, true);
		if (!(flags.all & 0x80) && !(flags.any & 2))
			DrawInverseSquareInProjectionSpace(vertexs.data(), index, polygons, 1);

		flags = LoadTileQuad(vertexs, index, false);
		if (!(flags.all & 0x80))
		{
			if (!(flags.any & 2) && !(flags.all & 0x78))
				DrawSquareInProjectionSpace(vertexs, index, polygons);
			if (m_ptrStr_E9C38_smalltit[index].haveBillboard_36)
				DrawSprites_3E360(index, polygons);
		}
		});
}

// The reflection: the mirrored tiles (pnt3/pnt4) that have a texture, and the mirrored sprites
void GameRenderHW::SubDrawInverseTerrainAndParticles(std::vector<int>& vertexs, std::vector<RenderPolygon>* polygons)
{
	Logger->trace("Start Drawing Reflection");
	ForEachTileFarToNear([&](int index) {
		const TileQuadFlags flags = LoadTileQuad(vertexs, index, true);
		if (m_ptrStr_E9C38_smalltit[index].textIndex_41 && !(flags.any & 2))
			DrawInverseSquareInProjectionSpace(vertexs.data(), index, polygons);
		if (m_ptrStr_E9C38_smalltit[index].haveBillboard_36)
			DrawTileBillboards_3FD60(index, polygons);
		});
}

// The terrain tiles and their sprites
void GameRenderHW::SubDrawTerrainAndParticles(std::vector<int>& vertexs, std::vector<RenderPolygon>* polygons)
{
	ForEachTileFarToNear([&](int index) {
		const TileQuadFlags flags = LoadTileQuad(vertexs, index, false);
		if (!(flags.any & 2) && !(flags.all & 0x78))
			DrawSquareInProjectionSpace(vertexs, index, polygons);
		if (m_ptrStr_E9C38_smalltit[index].haveBillboard_36)
			DrawSprites_3E360(index, polygons);
		});
}

// The sprite loaded (MainInitTmaps_71520 when needed) and marked as used in this frame; nullptr when it cannot be loaded.
Type_Sprite* GameRenderHW::UseSprite(int index)
{
	if (!m_ptrLoadedSprites_F66F0x[index] && !MainInitTmaps_71520(index))
		return nullptr;
	tmapGroupLastUse_F5730[str_TMAPS00TAB_BEGIN_BUFFER[index].group_8] = gameState_D41A4.FrameTimingIndex_26;
	return *m_ptrLoadedSprites_F66F0x[index];
}

// The sprite of the entity by its kind: one picture, an animation frame or one of the directions the camera sees it from
// (the picture mirrored for the other side); sets its size on the screen and x_BYTE_F2CC6 (drawn upright: the kinds 22-36,
// and 21 when uprightKind21). spriteBase is the texture (the sprite's frames stacked), spriteFrame the frame in it.
// turnTable: the directions of kind 17 through x_BYTE_D4750 (entity flag 0x40).
GameRenderHW::SpriteChoice GameRenderHW::ChooseSprite(type_entity_0x6E8E* entity, const Type_SpriteDef_D951C& def, int depth,
	bool uprightKind21, bool turnTable, Type_Sprite*& sprite, int& spriteBase, int& spriteFrame)
{
	x_BYTE_F2CC6 = 0;
	int direction = (((entity->yaw_0x1C_28 - (uint16_t)yaw_F2CC0) >> 3) & 0xF0) >> 4;
	int frame = 0;
	bool mirrored = false;
	const int kind = def.kind_12;
	if (kind == 0 || kind == 1 || kind == 21)
	{
		frame = 0;
	}
	else if (kind == 17)
	{
		if (turnTable && (entity->flags_0xc_12_15.byte[3] & 0x40))
			direction = (uint8_t)directionTable_D4750[44 + direction];
		mirrored = direction >= 8;
		frame = mirrored ? 15 - direction : direction;
	}
	else if (kind == 18)
	{
		frame = direction;
	}
	else if (kind == 19 || kind == 20)
	{
		mirrored = direction >= 8;
		frame = (uint8_t)directionTable_D4750[(kind == 19 ? 12 : 28) + direction];
	}
	else if ((kind >= 2 && kind <= 16) || (kind >= 22 && kind <= 36))
	{
		if (kind >= 22)
			x_BYTE_F2CC6 = 1;
		frame = entity->animationFrame_0x5C_92;
	}
	else
	{
		return SpriteChoice::Unknown;
	}

	sprite = UseSprite(def.textureIdx_0 + frame);
	if (!sprite)
		return SpriteChoice::LoadFailed;
	if (kind == 21 && uprightKind21)
		x_BYTE_F2CC6 = 1;
	spriteBase = def.textureIdx_0;
	spriteFrame = frame;
	spriteRender_F2C20.width_0x08 = sprite->width;
	spriteRender_F2C20.height_0x06 = sprite->height;
	const int realHeight = (signed __int64)(spriteRender_F2C20.projectionScale_0x18 * def.worldSize_8) / depth;
	spriteRender_F2C20.realHeight_0x0c = realHeight;
	spriteRender_F2C20.realWidth_0x09 = realHeight * spriteRender_F2C20.width_0x08 / spriteRender_F2C20.height_0x06;
	spriteRender_F2C20.signedWidth_0x05 = mirrored ? -spriteRender_F2C20.width_0x08 : spriteRender_F2C20.width_0x08;
	return SpriteChoice::Draw;
}

// The screen position of a sprite: across and height in the view space at the depth, rotated by the roll
void GameRenderHW::ProjectSprite(int across, int height, int depth)
{
	const int x = across * spriteRender_F2C20.projectionScale_0x18 / depth;
	const int y = spriteRender_F2C20.projectionScale_0x18 * height / depth + spriteRender_F2C20.horizonOffset_0x22;
	spriteRender_F2C20.screenY_0x04 = ((x * spriteRender_F2C20.cos_0x11 - spriteRender_F2C20.sin_0x0d * y) >> 16) + spriteRender_F2C20.centerX_0x24;
	spriteRender_F2C20.screenX_0x03 = spriteRender_F2C20.centerY_0x10 - ((spriteRender_F2C20.sin_0x0d * x + y * spriteRender_F2C20.cos_0x11) >> 16);
}

// The sprite's drawing mode by its lighting table, lit (dword0x00 = 0x2000) or in the fog
uint8_t GameRenderHW::SpriteVisibility(const Type_SpriteDef_D951C& def) const
{
	return (uint8_t)directionTable_D4750[(spriteRender_F2C20.fogShade_0x00 == 0x2000 ? 0 : 6) + def.lightingTableIdx_10];
}

// The entities of the tile mirrored in the water (below the terrain, upside down)
void GameRenderHW::DrawTileBillboards_3FD60(int tileIndex, std::vector<RenderPolygon>* polygons)
{
	type_entity_0x6E8E* entity = nullptr;
	uint16_t entityIndex = m_ptrStr_E9C38_smalltit[tileIndex].haveBillboard_36;
	do
	{
		if (entityIndex < 0x3E8u)
		{
			entity = Entities_EA3E4[entityIndex];
			if (!(entity->flags_0xc_12_15.byte[0] & 0x21))
			{
				const int dx = (int16_t)(entity->position_0x4C_76.x - cameraX_F2CC4);
				const int dy = (int16_t)(cameraY_F2CC2 - entity->position_0x4C_76.y);
				const int height = -entity->position_0x4C_76.z - spriteRender_F2C20.cameraZ_0x20;
				const int across = (dx * spriteRender_F2C20.cos2_0x0f - dy * spriteRender_F2C20.sin2_0x17) >> 16;
				const int depth = (spriteRender_F2C20.sin2_0x17 * dx + spriteRender_F2C20.cos2_0x0f * dy) >> 16;
				const int64_t distance = (int64_t)depth * depth + (int64_t)across * across;
				if (depth > 64 && distance < m_cutOffDistance)
				{
					spriteRender_F2C20.fogShade_0x00 = SpriteLight(distance);
					const Type_SpriteDef_D951C& def = spriteDefsTable_D951C[entity->spriteIndex_0x5A_90];
					ProjectSprite(across, height, depth);
					Type_Sprite* sprite = nullptr;
					int spriteBase = 0, spriteFrame = 0;
					if (ChooseSprite(entity, def, depth, true, false, sprite, spriteBase, spriteFrame) == SpriteChoice::Draw)
					{
						spriteRender_F2C20.texture_0x02 = sprite->textureBuffer;
						sprite->flags_0 |= 8;
						spriteRender_F2C20.visibility_0x01 = SpriteVisibility(def);
						spriteRender_F2C20.realWidth_0x09++;
						spriteRender_F2C20.realHeight_0x0c++;
						DrawSprite_41BD3((uint32_t)SpritePass::Reflection, polygons, spriteBase + spriteFrame, spriteFrame);
					}
				}
			}
		}
		entityIndex = entity->oldMapEntity_0x16_22;
	} while (entityIndex);
}

void GameRenderHW::SetEntityHelpText_88740(type_entity_0x6E8E* a1x, int16_t posX, int16_t posY)
{
	int v3; // esi
	type_entity_0x6E8E* v4x; // edx
	uint8_t v5; // al
	uint8_t v6; // al
	uint8_t v7; // al
	uint8_t v8; // al
	//char v9; // cl
	signed int v10; // eax
	uint8_t v11; // al
	//unsigned int v12; // edi
	signed int v13; // eax
	//int v14; // esi
	//char v15; // dl
	//char v16; // dh
	//char v17; // bl
	char v18; // [esp+0h] [ebp-4h]

	v3 = 0;
	if (helpPopup_1804B0.PopupStatusByte_0x9e & 1)
		return;
	v4x = Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
	v5 = a1x->class_0x3F_63;
	if (v5 < 5u)
	{
		if (v5 >= 2u)
		{
			if (v5 <= 2u)
			{
				v6 = a1x->model_0x40_64;
				if (v6 >= 1u)
				{
					if (v6 <= 1u)
					{
						v3 = 27;
					}
					else if (v6 == 2)
					{
						v3 = 22;
					}
				}
			}
			else if (v5 == 3)
			{
				v11 = a1x->model_0x40_64;
				if (v11 < 2u)
				{
					if (v11 == 1)
						v3 = 28;
				}
				else if (v11 <= 2u)
				{
					v3 = (a1x->id_0x1A_26 != v4x->id_0x1A_26) + 24;
				}
				else if (v11 == 3)
				{
					if (a1x->id_0x1A_26 == v4x->id_0x1A_26)
						v3 = 23;
					else
						v3 = 26;
				}
			}
		}
		goto LABEL_48;
	}
	if (v5 > 5u)
	{
		if (v5 >= 0xAu)
		{
			if (v5 <= 0xAu)
			{
				if (a1x->model_0x40_64 == 39 && a1x->playerEntityIndex_0x94_148 != v4x->id_0x1A_26)
					v3 = 18;
			}
			else if (v5 == 15 && !(a1x->flags_0xc_12_15.byte[0] & 1))
			{
				v3 = 20;
			}
		}
		goto LABEL_48;
	}
	if (a1x->id_0x1A_26 != v4x->id_0x1A_26)
	{
		v7 = a1x->model_0x40_64;
		if (v7 < 0xCu)
			goto LABEL_30;
		if (v7 > 0xEu)
		{
			if (v7 == 22)
			{
				if (((int8_t)a1x->actionIndex_0x45_69 != -76) && a1x->playerEntityIndex_0x94_148 != v4x->id_0x1A_26)
					v3 = 18;
				goto LABEL_48;
			}
		LABEL_30:
			v8 = a1x->actionIndex_0x45_69;
			if (v8 < 0xE8u || v8 > 0xEAu)
			{
				v10 = 1;
				if ((a1x->StageVar2_0x49_73 == 14 || a1x->StageVar2_0x49_73 == 13) && a1x->parentId_0x28_40 == v4x->id_0x1A_26)
					v10 = 0;
				if (v10)
					v3 = 19;
			}
			goto LABEL_48;
		}
	}
LABEL_48:
	if (v3)
	{
		if (videoMode_180660 & 1)
		{
			posX *= 2;
			posY *= 2;
		}
		if (str_E2A74[v3].axis_2[0] & 2)
		{
			if (a1x == str_E2A74[v3].entity_12)
			{
				if (!(helpPopup_1804B0.popupFlags_0x9f & 0x1))
				{
					str_E2A74[v3].axis_2[3] = posX;
					str_E2A74[v3].axis_2[0] |= 8;
					str_E2A74[v3].axis_2[4] = posY;
					helpPopup_1804B0.popupFlags_0x9f |= 2;
				}
			}
		}
		else
		{
			v18 = 0;
			v13 = Maths::DistanceXY_58490(&v4x->position_0x4C_76, &a1x->position_0x4C_76);
			if (!str_E2A74[v3].entity_12 || v13 < str_E2A74[v3].countdown_20 && v13 > 1024)
				v18 = 1;
			if (v18)
			{
				str_E2A74[v3].countdown_20 = v13;
				str_E2A74[v3].entity_12 = a1x;
				str_E2A74[v3].axis_2[0] |= 8;
			}
		}
	}
}

void GameRenderHW::DrawSorcererNameAndHealthBar_2CB30(type_entity_0x6E8E* entity, int16_t screenX, int screenY, int16_t spriteWidth)//20db30
{
	uint8_t scale = 1;
	if (!DefaultResolutions())
		scale = gameUiScale;
	// The low res UI (320 mode) draws in doubled coordinates, so the view port's pixels are doubled to match
	const int uiFactor = (videoMode_180660 & 1) ? 2 : 1;
	const int viewLeft = uiFactor * viewPort.PosX_EA3D0;
	const int viewTop = uiFactor * viewPort.PosY_EA3CC;
	const int right = uiFactor * (viewPort.PreWidth_EA3C4 + viewPort.PosX_EA3D0) - (4 * scale);
	const int bottom = uiFactor * (viewPort.PreHeight_EA3C0 + viewPort.PosY_EA3CC) - (22 * scale);
	const int wizard = entity->playerData_0xA4_164x->playerColorIndex_0x38_56;
	char name[32];
	strcpy(name, world_D41A0.players_0x2BDE[wizard].WizardName_0x39f_2BFA_12157);
	const uint8_t wizardColor = playersColors_E88E0x[GetTrueWizardNumber_61790(wizard)][0];
	const uint8_t barColor = m_ptrColorPalette[0];
	const uint8_t* frameColors = frameColors_D94F0[static_cast<std::underlying_type<MapType_t>::type>(world_D41A0.terrain_2FECE.MapType)];
	const uint8_t lightColor = frameColors[2];
	const uint8_t darkColor = frameColors[3];
	const uint8_t backgroundColor = frameColors[0];
	const int16_t center = (spriteWidth >> 1) + screenX;
	const int16_t left = uiFactor * (viewPort.PosX_EA3D0 + center);
	const int top = uiFactor * (viewPort.PosY_EA3CC + screenY) - (20 * scale);
	if (left < viewLeft || static_cast<int16_t>(top) < viewTop || left >= right || static_cast<int16_t>(top) >= bottom)
		return;
	const int border = 2 * scale;
	const int letterWidth = 8 * scale;
	int length = strlen(name);
	int width = letterWidth * length + (4 * scale);
	if (left + width > right)
	{
		width = right - left;
		length = (right - left - (4 * scale)) / letterWidth;
	}
	if (length <= 0)
		return;
	name[length] = 0;
	const int16_t frameWidth = width + border;
	DrawLine_2BC80(left, top, frameWidth, 18 * scale, backgroundColor);
	DrawLine_2BC80(left, top, frameWidth, border, lightColor);
	DrawLine_2BC80(left, top + (16 * scale), frameWidth, border, darkColor);
	DrawLine_2BC80(left, top, border, 16 * scale, lightColor);
	DrawLine_2BC80(left + width, top, border, 18 * scale, darkColor);
	DrawText_2BC10(name, left + (4 * scale), top, wizardColor, scale);
	DrawLine_2BC80(left + border, top + (14 * scale), width - border, border, barColor);
	if (entity->maxLife_0x4)
	{
		int health = entity->life_0x8 * (width - border) / entity->maxLife_0x4;
		if (left + border + health > right - border)
			health = right - border - (left + border);
		if (health > 0)
			DrawLine_2BC80(left + border, top + (14 * scale), health, border, wizardColor);
	}
}

HWVertex GameRenderHW::MakeHWVertex(const ProjectionVertex& v, uint32_t textureIndex)
{
	HWVertex h;
	h.X = (float)v.X / (float)viewPort.Width_DE564 * 2.0f - 1.0f;
	h.Y = (float)v.Y / (float)viewPort.Height_DE568 * 2.0f - 1.0f;
	h.W = 1.0f;

	float tileX = (float)(textureIndex % kTilesWide) * TileTextureSize_D41B5;
	float tileY = (float)(textureIndex / kTilesWide) * TileTextureSize_D41B5;

	// The far edge is TileTextureSize_D41B5 - 1/65536, which rounds to the next tile's edge once the
	// tile origin is added (a float can't hold that step at atlas coordinates in the hundreds). Pull
	// both edges in a hair, so interpolation can't floor a texel into a neighbouring tile.
	const float maxUV = (float)TileTextureSize_D41B5 - kTileUVInset;
	h.U = tileX + std::clamp((float)v.U / 65536.0f, kTileUVInset, maxUV);    // inside the tile, plus the tile's origin
	h.V = tileY + std::clamp((float)v.V / 65536.0f, kTileUVInset, maxUV);

	h.Shade = std::clamp((float)v.Brightness / kBrightnessNeutral, 0.0f, 2.0f);  // >1 brightens
	return h;
}

// The tile's texture coordinates into vertexs, then its four corners (vertexs at 0, 6, 12 and 18, in "Screen Space": x & y,
// top left 0,0) as the GPU's vertices, textured with textureIndex; false when the tile is culled.
bool GameRenderHW::ProjectTileCorners(int* vertexs, int tileIndex, uint32_t textureIndex, HWVertex corners[4])
{
	const auto& uv = UVTable_D4350[m_ptrStr_E9C38_smalltit[tileIndex].textUV_42];
	vertexs[20] = uv[0];
	vertexs[21] = uv[1];
	vertexs[14] = uv[2];
	vertexs[15] = uv[3];
	vertexs[8] = uv[4];
	vertexs[9] = uv[5];
	vertexs[2] = uv[6];
	vertexs[3] = uv[7];

	ProjectionVertex vertex0(&vertexs[0]);
	ProjectionVertex vertex6(&vertexs[6]);
	ProjectionVertex vertex12(&vertexs[12]);
	ProjectionVertex vertex18(&vertexs[18]);

	if (CheckViewPortCull(vertex18, vertex12, vertex0) || CheckViewPortCull(vertex0, vertex12, vertex6))
		return false;

	corners[0] = MakeHWVertex(vertex0, textureIndex);
	corners[1] = MakeHWVertex(vertex6, textureIndex);
	corners[2] = MakeHWVertex(vertex12, textureIndex);
	corners[3] = MakeHWVertex(vertex18, textureIndex);
	return true;
}

void GameRenderHW::DrawSquareInProjectionSpace(std::vector<int>& vertexs, int tileIndex, std::vector<RenderPolygon>* polygons)
{
	const auto& tile = m_ptrStr_E9C38_smalltit[tileIndex];
	HWVertex h[4];   // corners 0, 6, 12, 18
	if (!ProjectTileCorners(vertexs.data(), tileIndex, tile.textIndex_41, h))
		return;

	RenderPolygon poly;
	poly.TextureId = kTerrainAtlasId;
	if (tile.triangleFeatures_38 & 0x80)   // water tile (only set when reflections are on)
	{
		poly.SpriteMode = 26;
		poly.Tint = static_cast<uint8_t>(world_D41A0.terrain_2FECE.MapType);   // the shader picks the blend fitted for the map type
	}

	// Fan (a,b,c,d) splits into (a,b,c) and (a,c,d), so the first vertex picks the diagonal.
	if ((uint8_t)tile.triangleFeatures_38 & 1)
		poly.Vertices = { h[2], h[1], h[0], h[3] };    // diagonal 12-0
	else
		poly.Vertices = { h[3], h[2], h[1], h[0] };    // diagonal 18-6

	polygons->push_back(std::move(poly));
}

bool GameRenderHW::CheckViewPortCull(ProjectionVertex v1, ProjectionVertex v2, ProjectionVertex v3, int maxCoordinate, int minCoordinate)
{
	if ((((int64_t)v1.X << 16) > maxCoordinate) || (((int64_t)v1.Y << 16) > maxCoordinate) || (((int64_t)v2.X << 16) > maxCoordinate) ||
		(((int64_t)v2.Y << 16) > maxCoordinate) || (((int64_t)v3.X << 16) > maxCoordinate) || (((int64_t)v3.Y << 16) > maxCoordinate))
	{
		return true;
	}
	if ((((int64_t)v1.X << 16) < minCoordinate) || (((int64_t)v1.Y << 16) < minCoordinate) || (((int64_t)v2.X << 16) < minCoordinate) ||
		(((int64_t)v2.Y << 16) < minCoordinate) || (((int64_t)v3.X << 16) < minCoordinate) || (((int64_t)v3.Y << 16) < minCoordinate))
	{
		return true;
	}
	return false;
}

// The light of a sprite by its distance (fog): 0x2000 full, 0 in the fog end.
int GameRenderHW::SpriteLight(int64_t distance) const
{
	if (distance <= m_fogStart)
		return 0x2000;
	if (distance < m_fogEnd)
		return static_cast<int>(32 * (m_fogEnd - distance) / (m_fogEnd - m_fogStart)) << 8;
	return 0;
}

// The light of a tile corner, faded by its distance.
int GameRenderHW::FogLight(int light, int64_t distance) const
{
	if (distance <= m_fogStart)
		return light;
	if (distance >= m_fogEnd)
		return 0;
	return static_cast<int>(light * (m_fogEnd - distance) / (m_fogEnd - m_fogStart));
}

// The tile upside down (the cave ceiling, the reflection): the other winding; overrideTextIndex > -1 replaces its texture
void GameRenderHW::DrawInverseSquareInProjectionSpace(int* vertexs, int tileIndex, std::vector<RenderPolygon>* polygons, int overrideTextIndex)
{
	const auto& tile = m_ptrStr_E9C38_smalltit[tileIndex];
	HWVertex h[4];   // corners 0, 6, 12, 18
	if (!ProjectTileCorners(vertexs, tileIndex, overrideTextIndex > -1 ? overrideTextIndex : tile.textIndex_41, h))
		return;

	RenderPolygon poly;
	poly.TextureId = kTerrainAtlasId;
	poly.Reflection = m_reflectionPass;

	// Fan (a,b,c,d) splits into (a,b,c) and (a,c,d), so the first vertex picks the diagonal.
	if ((uint8_t)tile.triangleFeatures_38 & 1)
		poly.Vertices = { h[2], h[3], h[0], h[1] };    // diagonal 12-0
	else
		poly.Vertices = { h[3], h[0], h[1], h[2] };    // diagonal 18-6

	polygons->push_back(std::move(poly));
}

// The entities of the tile: each one's shadow on the ground (shadows on, not over water), then the entity itself.
// spriteBase/spriteFrame: the texture and the frame in it (ChooseSprite); an entity whose shadow sprite cannot be
// loaded is skipped altogether, as in the original.
void GameRenderHW::DrawSprites_3E360(int tileIndex, std::vector<RenderPolygon>* polygons)
{
	uint16_t entityIndex = m_ptrStr_E9C38_smalltit[tileIndex].haveBillboard_36;
	do
	{
		//adress 21f370
		type_entity_0x6E8E* entity = Entities_EA3E4[entityIndex];
		spriteRender_F2C20.entity_0x14x = entity;
		if (!(entity->flags_0xc_12_15.byte[0] & 0x21))
		{
			const int dx = (int16_t)(entity->position_0x4C_76.x - cameraX_F2CC4);
			const int dy = (int16_t)(cameraY_F2CC2 - entity->position_0x4C_76.y);
			Type_Sprite* sprite = nullptr;
			int spriteBase = 0, spriteFrame = 0;

			// ---- the shadow ----
			if (shadows_F2CC7 && !m_ptrStr_E9C38_smalltit[tileIndex].textAtyp_43 && !(entity->flags_0xc_12_15.word[1] & 0x808))
			{
				//adress 21f40c
				const int height = GetTerrainAlt2_B5C60(entity->position_0x4C_76.x, entity->position_0x4C_76.y) - spriteRender_F2C20.cameraZ_0x20;
				const int across = (spriteRender_F2C20.cos2_0x0f * dx - spriteRender_F2C20.sin2_0x17 * dy) >> 16;
				const int depth = (spriteRender_F2C20.sin2_0x17 * dx + spriteRender_F2C20.cos2_0x0f * dy) >> 16;
				const int64_t distance = (int64_t)depth * depth + (int64_t)across * across;
				if (depth > 64 && distance < m_cutOffDistance)
				{
					spriteRender_F2C20.fogShade_0x00 = SpriteLight(distance);
					const Type_SpriteDef_D951C& def = spriteDefsTable_D951C[entity->spriteIndex_0x5A_90];
					if (!def.lightingTableIdx_10)
					{
						ProjectSprite(across, height, depth);
						const SpriteChoice choice = ChooseSprite(entity, def, depth, false, false, sprite, spriteBase, spriteFrame);
						if (choice == SpriteChoice::LoadFailed)
						{
							entityIndex = spriteRender_F2C20.entity_0x14x->oldMapEntity_0x16_22;
							continue;
						}
						if (choice == SpriteChoice::Draw)
						{
							spriteRender_F2C20.texture_0x02 = sprite->textureBuffer;
							sprite->flags_0 |= 8;
							spriteRender_F2C20.visibility_0x01 = SpriteVisibility(def);
							spriteRender_F2C20.realHeight_0x0c >>= 2;
							if (spriteRender_F2C20.realWidth_0x09 > 0 && spriteRender_F2C20.realHeight_0x0c > 0)
							{
								const int light = spriteRender_F2C20.fogShade_0x00 >> 2;
								spriteRender_F2C20.fogShade_0x00 = notDay_D4320 ? 0x2000 - light : light + 0x2000;
								spriteRender_F2C20.visibility_0x01 = 8;
								DrawSprite_41BD3((uint32_t)SpritePass::Shadow, polygons, spriteBase + spriteFrame, spriteFrame);
							}
						}
					}
				}
			}

			// ---- the entity ----
			const int z = spriteRender_F2C20.entity_0x14x->flags_0xc_12_15.byte[3] >= 0
				? spriteRender_F2C20.entity_0x14x->position_0x4C_76.z : spriteRender_F2C20.entity_0x14x->position_0x4C_76.z - 160;
			int depth = (spriteRender_F2C20.sin2_0x17 * dx + spriteRender_F2C20.cos2_0x0f * dy) >> 16;
			const int across = (spriteRender_F2C20.cos2_0x0f * dx - spriteRender_F2C20.sin2_0x17 * dy) >> 16;
			if (spriteRender_F2C20.entity_0x14x->flags_0xc_12_15.byte[3] & 0x20)
			{
				// carried: moved along the carrier's direction
				const type_D404C* offsets = &str_D404C[spriteRender_F2C20.entity_0x14x->poseIndex_0x3B_59];
				switch ((((Entities_EA3E4[spriteRender_F2C20.entity_0x14x->leaderId_0x32_50]->yaw_0x1C_28 - (uint16_t)yaw_F2CC0) >> 3) & 0xF0) >> 4)
				{
				case 0: case 15: depth -= offsets->depthOffset1_16; break;
				case 1: case 14: depth -= offsets->depthOffset2_18; break;
				case 2: case 13: depth -= offsets->depthOffset3_20; break;
				case 5: case 10: depth += offsets->depthOffset3_20; break;
				case 6: case 9:  depth += offsets->depthOffset2_18; break;
				case 7: case 8:  depth += offsets->depthOffset1_16; break;
				default: break;
				}
			}
			const int64_t distance = (int64_t)depth * depth + (int64_t)across * across;
			if (depth > 64 && distance < m_cutOffDistance)
			{
				spriteRender_F2C20.fogShade_0x00 = SpriteLight(distance);
				const Type_SpriteDef_D951C& def = spriteDefsTable_D951C[spriteRender_F2C20.entity_0x14x->spriteIndex_0x5A_90];
				ProjectSprite(across, z - spriteRender_F2C20.cameraZ_0x20, depth);
				if (ChooseSprite(spriteRender_F2C20.entity_0x14x, def, depth, true, true, sprite, spriteBase, spriteFrame) == SpriteChoice::Draw)
				{
					type_entity_0x6E8E* drawn = spriteRender_F2C20.entity_0x14x;
					sprite->flags_0 |= 8u;
					const __int16 flags2 = drawn->flags_0xc_12_15.byte[2];
					spriteRender_F2C20.texture_0x02 = sprite->textureBuffer;
					if (flags2 & 0x380)
					{
						// a player's: tinted with the player's colour (4/5), or blended (2/3)
						const char flags3 = drawn->flags_0xc_12_15.byte[3];
						if (flags3 & 2)
						{
							spriteRender_F2C20.visibility_0x01 = 4;
							spriteRender_F2C20.tint_0x07 = playersColors_E88E0x[Entities_EA3E4[drawn->parentId_0x28_40]->playerData_0xA4_164x->playerColorIndex_0x38_56][2];
						}
						else if (flags3 & 4)
						{
							spriteRender_F2C20.visibility_0x01 = 5;
							spriteRender_F2C20.tint_0x07 = playersColors_E88E0x[Entities_EA3E4[drawn->parentId_0x28_40]->playerData_0xA4_164x->playerColorIndex_0x38_56][2];
						}
						else if (drawn->flags_0xc_12_15.byte[2] >= 0)
						{
							if (flags3 & 1)
								spriteRender_F2C20.visibility_0x01 = 3;
						}
						else
						{
							spriteRender_F2C20.visibility_0x01 = 2;
						}
					}
					else
					{
						spriteRender_F2C20.visibility_0x01 = SpriteVisibility(def);
					}
					spriteRender_F2C20.realWidth_0x09++;
					spriteRender_F2C20.realHeight_0x0c++;

					DrawSprite_41BD3((uint32_t)SpritePass::Main, polygons, spriteBase + spriteFrame, spriteFrame);
				}
			}
		}
		entityIndex = spriteRender_F2C20.entity_0x14x->oldMapEntity_0x16_22;
	} while (entityIndex);
}

static bool SpriteFitsScreen(uint8_t viewDistanceScale)
{
	if (screenWidth_18062C <= 640 && screenHeight_180624 <= 480 && viewDistanceScale <= 1)
		return true;
	const int64_t size = static_cast<int64_t>(spriteRender_F2C20.realWidth_0x09) + spriteRender_F2C20.realHeight_0x0c;
	const int64_t x = spriteRender_F2C20.screenY_0x04;
	const int64_t y = spriteRender_F2C20.screenX_0x03;
	return x + size >= 0 && x - size < viewPort.Width_DE564 && y + size >= 0 && y - size < viewPort.Height_DE568
		&& x > -32768 && x < 32768 && y > -32768 && y < 32768 && size < 32768;
}

void GameRenderHW::DrawSprite_41BD3(uint32_t pass, std::vector<RenderPolygon>* polygons, int spriteIndex, int spriteFrame)
{
	auto& s = spriteRender_F2C20;
	constexpr float kFix = 1.0f / 65536.0f;
	const bool draw = SpriteFitsScreen(m_viewDistanceScale);   // before the pass moves the position, as NG

	if (!x_BYTE_F2CC6)
	{
		// ---- rotated path: same integer prologue as the original ----
		// (kept exactly, the overlay calls below read the adjusted values)
		if (pass == 1)
		{
			s.screenY_0x04 -= ((s.cos_0x11 * s.realWidth_0x09 >> 1) + s.sin_0x0d * s.realHeight_0x0c) >> 16;
			s.screenX_0x03 -= (s.cos_0x11 * s.realHeight_0x0c - (s.sin_0x0d * s.realWidth_0x09 >> 1)) >> 16;
		}
		else if (pass == 0 || pass == 2)
		{
			s.screenY_0x04 -= s.cos_0x11 * s.realWidth_0x09 >> 17;
			s.screenX_0x03 -= -(s.sin_0x0d * s.realWidth_0x09) >> 17;
		}

		if (draw && s.realWidth_0x09 > 0 && s.realHeight_0x0c > 0
			&& (unsigned)s.visibility_0x01 <= 8)
		{
			const float c = s.cos_0x11 * kFix;
			const float sn = s.sin_0x0d * kFix;
			// right = (cos, -sin), down = (sin, cos); a1 != 1 is the flipped reflection
			EmitSpriteQuad(polygons, spriteIndex, spriteFrame,
				(float)s.screenY_0x04, (float)s.screenX_0x03,
				c, -sn, sn, c, pass != 1);
		}

		if (pass == 1 && m_primaryView)
		{
			if (!gameState_D41A4.byteindex_207
				&& s.entity_0x14x->class_0x3F_63 == 3
				&& (!s.entity_0x14x->model_0x40_64 || s.entity_0x14x->model_0x40_64 == 1))
			{
				DrawSorcererNameAndHealthBar_2CB30(s.entity_0x14x, s.screenY_0x04,
					(int16_t)s.screenX_0x03, s.realWidth_0x09);
			}
			if (gameState_D41A4.showHelp_10)
				SetEntityHelpText_88740(s.entity_0x14x,
					(int16_t)(s.screenY_0x04 + (s.realWidth_0x09 >> 1)),
					(int16_t)(s.screenX_0x03 + (s.realHeight_0x0c >> 1)));
			if (s.entity_0x14x->flags_0xc_12_15.byte[3] & 0x40)
				s.entity_0x14x->subSpellIndex_0x2A_42 |= 0x40u;
		}
		return;
	}

	// ---- axis-aligned path ----
	if (!draw)
		return;
	const int q = (s.realHeight_0x0c + s.realWidth_0x09) >> 2;
	if (pass == 1)
	{
		s.screenY_0x04 += -(s.sin_0x0d * q >> 16) - q;
		s.screenX_0x03 += -(s.cos_0x11 * q >> 16) - q;
	}
	else if (pass == 2)
	{
		s.screenY_0x04 += (s.sin_0x0d * q >> 16) - q;
		s.screenX_0x03 += (s.cos_0x11 * q >> 16) - q;
	}

	// Cheap cull; the scissor rect clips the rest.
	if (s.screenY_0x04 >= (uint16_t)viewPort.Width_DE564 || s.screenY_0x04 + s.realWidth_0x09 <= 0 ||
		s.screenX_0x03 >= (uint16_t)viewPort.Height_DE568 || s.screenX_0x03 + s.realHeight_0x0c <= 0)
		return;

	if (pass == 1 && m_primaryView && gameState_D41A4.showHelp_10)
		SetEntityHelpText_88740(s.entity_0x14x,
			(int16_t)(s.screenY_0x04 + (s.realWidth_0x09 >> 1)),
			(int16_t)(s.screenX_0x03 + (s.realHeight_0x0c >> 1)));

	// the original has no mode 8 in this path
	if ((unsigned)s.visibility_0x01 <= 7)
		EmitSpriteQuad(polygons, spriteIndex, spriteFrame,
			(float)s.screenY_0x04, (float)s.screenX_0x03, 1, 0, 0, 1, false);
}

uint32_t GameRenderHW::SpriteFrameRow(int spriteIndex, int spriteFrame)
{
	const uint32_t frames = m_spriteFrameCount[spriteIndex];
	if (frames <= 1)
		return 0;

	if (m_ptrLoadedSprites_F66F0x[spriteIndex] && ((*m_ptrLoadedSprites_F66F0x[spriteIndex])->flags_0 & 1))
	{
		if (const type_animations1* anim = GetAnimationByIndex_724F0(animations_E9C08x, spriteIndex))
			return (uint32_t)std::clamp(anim->FrameIndex_22 - 2, 0, (int)frames - 1);
	}

	return std::min<uint32_t>((uint32_t)spriteFrame, frames - 1);
}

void GameRenderHW::EmitSpriteQuad(std::vector<RenderPolygon>* polygons, int spriteIndex, int spriteFrame,
	float x, float y, float rx, float ry, float dx, float dy, bool flipV)
{
	auto& s = spriteRender_F2C20;

	// Not uploaded yet (TMAPS loads lazily, the upload event may land a frame later): skip.
	const uint32_t frames = m_spriteFrameCount[spriteIndex];
	if (frames == 0)
		return;

	const uint32_t frame = SpriteFrameRow(spriteIndex, spriteFrame);

	const float w = (float)s.realWidth_0x09;
	const float h = (float)s.realHeight_0x0c;

	// dword0x05 = +/- frame width; negative = mirrored view
	float u0 = 0.0f, u1 = 1.0f;
	if (s.signedWidth_0x05 < 0) std::swap(u0, u1);

	// Frame inside the stacked texture (first frame at the top, V = 0)
	const float fv = 1.0f / (float)frames;
	float v0 = frame * fv, v1 = v0 + fv;
	if (flipV) std::swap(v0, v1);

	// Viewport pixels -> NDC. In this struct "screenY" is the column (X), "screenX" is the row (Y).
	const float invW = 2.0f / (float)(uint16_t)viewPort.Width_DE564;
	const float invH = 2.0f / (float)(uint16_t)viewPort.Height_DE568;

	// dword0x00 high byte = colour LUT row, 32 = fully lit
	const float shade = (float)((s.fogShade_0x00 >> 8) & 0xFF) / 32.0f;

	const float px[4] = { x, x + rx * w, x + rx * w + dx * h, x + dx * h };
	const float py[4] = { y, y + ry * w, y + ry * w + dy * h, y + dy * h };
	const float pu[4] = { u0, u1, u1, u0 };
	const float pv[4] = { v0, v0, v1, v1 };

	RenderPolygon poly;
	poly.TextureId = (uint32_t)spriteIndex;
	poly.SpriteMode = (uint8_t)s.visibility_0x01;
	poly.Reflection = m_reflectionPass;
	poly.Frames = (uint16_t)frames;

	switch (poly.SpriteMode)
	{
	case 2: case 6:                 poly.Alpha = 1.0f - COLOUR_BLEND_ALPHA; break;
	case 3: case 7:                 poly.Alpha = COLOUR_BLEND_ALPHA;        break;
	case 8:                         poly.Alpha = 0.25f; break;   // shadow: darkens through the lighting table, not the blend table
	default:                        poly.Alpha = 1.0f;  break;
	}

	poly.Tint = (uint8_t)(s.tint_0x07 & 0xFF);
	for (int i = 0; i < 4; i++)
	{
		HWVertex& v = poly.Vertices[i];
		v.X = px[i] * invW - 1.0f;
		v.Y = py[i] * invH - 1.0f;
		v.W = 1.0f;
		v.U = pu[i];
		v.V = pv[i];
		v.Shade = shade;
	}
	polygons->push_back(std::move(poly));
}
