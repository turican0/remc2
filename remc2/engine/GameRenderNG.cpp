#include "GameRenderNG.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <memory>
#include <vector>

// viewDistanceScale from 1: the terrain grid (40 x 21 tiles in the original) times the scale with the cut-off and the fog
// further away, over 6 the map (256 x 256 tiles) repeats; fogScale moves the fog (1.0 as the original). With 1 and 1.0 everything is as the original.
// renderThreads: over 640x480 the sky and the terrain drawn by bands of rows in them and in the game thread (same pixels); the
// sprites stay in the game thread, smaller screens draw everything in it. sizePercentToThreadRender of HD is not used.
GameRenderNG::GameRenderNG(uint8_t viewDistanceScale, float fogScale, uint8_t renderThreads, bool assignToSpecificCores, float /*sizePercentToThreadRender*/)
	: m_assignToSpecificCores(assignToSpecificCores)
{
	SetRenderThreads(renderThreads);
	m_viewDistanceScale = viewDistanceScale < 1 ? 1 : viewDistanceScale;
	m_tileColumns = 40 * m_viewDistanceScale;
	m_tileRows = 21 * m_viewDistanceScale;
	if (m_viewDistanceScale > 1)
	{
		m_ownTiles.resize(m_tileColumns * m_tileRows);
		m_tiles = m_ownTiles.data();
		BuildTileSteps();
	}

	// squared distances in 1/65536 of a tile (256 units) squared; the fog from 15 to 19 tiles (20 the cut-off) times the scale
	const auto fog = [fogScale](int64_t scale, int64_t& cutOffDistance, int64_t& fogStartDistance, int64_t& fogEndDistance)
	{
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
		cutOffDistance = cutOff << 16;
		fogEndDistance = fogEnd << 16;
		fogStartDistance = fogStart << 16;
	};
	fog(m_viewDistanceScale, m_cutOffDistance, m_fogStart, m_fogEnd);
	fog(1, m_standardCutOff, m_standardFogStart, m_standardFogEnd);
}

GameRenderNG::~GameRenderNG()
{
	SetRenderThreads(0);
}

// The steps of the map walk (m_tileSteps_D4328, 10 bytes per view quadrant) for a wider grid: the start shifted by half of
// the extra columns, the row step by the column count.
void GameRenderNG::BuildTileSteps()
{
	const int columns = m_tileColumns;
	const int shift = m_viewDistanceScale - 1;
	const uint8_t negativeColumns = 256 - columns;
	const uint8_t negativeHalf = 0xED - (columns - 40) / 2;
	const uint8_t positiveHalf = 0x13 + (columns - 40) / 2;
	uint8_t* quadrant0 = &m_tileSteps_D4328[0];
	uint8_t* quadrant1 = &m_tileSteps_D4328[10];
	uint8_t* quadrant2 = &m_tileSteps_D4328[20];
	uint8_t* quadrant3 = &m_tileSteps_D4328[30];
	quadrant0[0] = negativeHalf + shift;
	quadrant0[1] += shift;
	quadrant0[6] = negativeColumns;
	quadrant0[7] = 0xFF;
	quadrant0[8] = 1;
	quadrant0[9] = 0;
	quadrant1[1] = negativeHalf + shift;
	quadrant1[6] = 1;
	quadrant1[7] = negativeColumns;
	quadrant1[8] = 0;
	quadrant1[9] = 1;
	quadrant2[0] = positiveHalf - shift;
	quadrant2[6] = columns;
	quadrant2[7] = 1;
	quadrant2[8] = 0xFF;
	quadrant2[9] = 0;
	quadrant3[0] += shift;
	quadrant3[1] = positiveHalf - shift;
	quadrant3[6] = 0xFF;
	quadrant3[7] = columns;
	quadrant3[8] = 0;
	quadrant3[9] = 0xFF;
}

// The camera sways with the slope of the terrain under it; draws the world once, as two eyes (VR, stereo)
// or blurred with the previous frame when flying fast.
void GameRenderNG::DrawWorld_411A0(int posX, int posY, int16_t yaw, int16_t posZ, int16_t pitch, int16_t roll, int16_t fov)
{
	SetScreenBuffers();
	uint8_t tileX = (posX >> 8) & 0xFF;
	uint8_t tileY = (posY >> 8) & 0xFF;
	if ((posX & 0xFF) < 128)
		tileX--;
	if ((posY & 0xFF) < 128)
		tileY--;
	const auto height = [](uint8_t x, uint8_t y) -> int { return mapHeightmap_11B4E0[y << 8 | x]; };
	const int height00 = height(tileX, tileY);
	const int height20 = height(tileX + 2, tileY);
	const int height22 = height(tileX + 2, tileY + 2);
	const int height02 = height(tileX, tileY + 2);
	const int slopeX = std::clamp(2 * (height00 - height20 - height22 + height02), -100, 100);
	const int slopeY = std::clamp(2 * (height00 + height20 - height22 - height02), -100, 100);

	const int yawIndex = yaw & 0x7FF;
	m_cameraSwayX_D4794 += (slopeX - m_cameraSwayX_D4794) >> 3;
	m_cameraSwayY_D4798 += (slopeY - m_cameraSwayY_D4798) >> 3;
	const int cameraX = m_cameraSwayX_D4794 + posX;
	const int cameraY = m_cameraSwayY_D4798 + posY;
	const int eyeX = 20 * Maths::sin_DB750[512 + yawIndex] >> 16;
	const int eyeY = 20 * Maths::sin_DB750[yawIndex] >> 16;

	if (D41A0_0.m_GameSettings.str_0x2192.xxxx_0x2193 && D41A0_0.m_GameSettings.m_Display.m_uiScreenSize && screenWidth_18062C == 640)
	{
		//VR interlaced render
		viewPort.SetRenderViewPortSize_BCD45(pdwScreenBuffer_351628, 2 * screenWidth_18062C, screenWidth_18062C / 2 - 8, screenHeight_180624 / 2 - 40);
		m_eyeShift_D4324 = -5;
		DrawTerrainAndParticles_3C080(cameraX - eyeX, cameraY - eyeY, yawIndex, posZ, pitch, roll, fov);
		viewPort.SetRenderViewPortSize_BCD45(pdwScreenBuffer_351628 + (screenWidth_18062C / 2), 0, 0, 0);
		m_eyeShift_D4324 = 5;
		DrawTerrainAndParticles_3C080(cameraX + eyeX, cameraY + eyeY, yawIndex, posZ, pitch, roll, fov);
		m_eyeShift_D4324 = 0;
		viewPort.SetRenderViewPortSize_BCD45(pdwScreenBuffer_351628, screenWidth_18062C, screenWidth_18062C, screenHeight_180624);
	}
	else if (D41A0_0.m_GameSettings.m_Display.m_uiScreenSize != 1 || D41A0_0.m_GameSettings.str_0x2192.xxxx_0x2193)
	{
		const auto savedBlur = D41A0_0.m_GameSettings.m_Display.xxxx_0x2191;
		if (x_WORD_180660_VGA_type_resolution == 1 && !D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221
			&& x_D41A0_BYTEARRAY_4_struct.m_wHighSpeedSystem && m_blurBuffer && D41A0_0.m_GameSettings.m_Graphics.m_wViewPortSize == 40
			&& std::abs(Entities_EA3E4[D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240]->actSpeed_0x82_130) > 80)
			D41A0_0.m_GameSettings.m_Display.xxxx_0x2191 = 1;

		if (D41A0_0.str_0x21AE.xxxx_0x21B1 && D41A0_0.m_GameSettings.m_Display.xxxx_0x2191 && m_blurBuffer)
		{
			//Blur: the frame into the buffer, then blended with the previous one on the screen
			uint8_t* screen = ViewPortRenderBufferStart_DE558;
			viewPort.SetRenderViewPortSize_BCD45(m_blurBuffer, 0, 0, 0);
			DrawTerrainAndParticles_3C080(cameraX, cameraY, yawIndex, posZ, pitch, roll, fov);
			viewPort.SetRenderViewPortSize_BCD45(screen, 0, 0, 0);
			BlendBlurBuffer(screen, m_blurBuffer, D41A0_0.m_GameSettings.m_Display.xxxx_0x2191 == 1);
		}
		else
		{
			DrawTerrainAndParticles_3C080(cameraX, cameraY, yawIndex, posZ, pitch, roll, fov);
			if (D41A0_0.m_GameSettings.str_0x2192.xxxx_0x2192)
				SmoothViewPort();
		}
		D41A0_0.m_GameSettings.m_Display.xxxx_0x2191 = savedBlur;
	}
	else
	{
		//Stereo: the right eye on the screen, the left one into the buffer
		m_eyeShift_D4324 = iScreenWidth_DE560 / 40;
		DrawTerrainAndParticles_3C080(eyeX + cameraX, eyeY + cameraY, yawIndex, posZ, pitch, roll, fov);
		uint8_t* screen = ViewPortRenderBufferStart_DE558;
		viewPort.SetRenderViewPortSize_BCD45(m_blurBuffer, 0, 0, 0);
		m_eyeShift_D4324 = -(iScreenWidth_DE560 / 40);
		DrawTerrainAndParticles_3C080(cameraX - eyeX, cameraY - eyeY, yawIndex, posZ, pitch, roll, fov);
		viewPort.SetRenderViewPortSize_BCD45(screen, 0, 0, 0);
		m_eyeShift_D4324 = 0;
	}
}

// Up to 640x480 the scratch tables of the original (x_DWORD_E9C38_smalltit, the original offsets: the stale values read past
// the tables stay the same) and ptrMemoryBuffer_E9C3C; a bigger screen gets own buffers for its widths and heights.
void GameRenderNG::SetScreenBuffers()
{
	m_largeScreen = screenWidth_18062C > 640 || screenHeight_180624 > 480;
	if (!m_largeScreen)
	{
		m_scratch = x_DWORD_E9C38_smalltit;
		m_columnsOffset = 36960;
		m_spansOffset = 45920;
		m_rowsOffset = 59360;
		m_blurBuffer = ptrMemoryBuffer_E9C3C;
		return;
	}
	// columns (8 bytes) and spans (12 bytes) up to width + height, rows (4 bytes) up to the bigger side
	const int lines = GAME_RES_MAX_WIDTH + GAME_RES_MAX_HEIGHT + 2;
	m_columnsOffset = 0x4000;
	m_spansOffset = m_columnsOffset + 8 * lines + 0x100;
	m_rowsOffset = m_spansOffset + 12 * lines + 0x100;
	m_columnsCapacity = lines - 1;
	m_largeScratch.resize(m_rowsOffset + 4 * lines + 0x100);
	m_scratch = m_largeScratch.data();
	m_largeBlurBuffer.resize(screenWidth_18062C * screenHeight_180624);
	m_blurBuffer = m_largeBlurBuffer.data();
}

// Screen pixel = blend of the new frame (blur buffer) and the screen, groups of 4 pixels of each line.
// swapFirstGroup: the original builds the index of the first group of the first line as screen << 8 | buffer.
void GameRenderNG::BlendBlurBuffer(uint8_t* screen, const uint8_t* buffer, bool swapFirstGroup)
{
	const int drawnWidth = DrawnWidth();
	for (int row = 0; row < viewPort.Height_DE568; row++)
	{
		for (int column = 0; column < drawnWidth; column++)
		{
			const bool swap = swapFirstGroup && row == 0 && column < 4;
			const int index = swap ? screen[column] << 8 | buffer[column] : buffer[column] << 8 | screen[column];
			screen[column] = ColourLookupTable_F6EE0[COLOUR_BLEND_LOOKUP_OFFSET + index];
		}
		screen += iScreenWidth_DE560;
		buffer += iScreenWidth_DE560;
	}
}

// Every pixel (but the last line and column) = blend of the blends with the pixel below, of it and of its right neighbour.
void GameRenderNG::SmoothViewPort()
{
	uint8_t* line = ViewPortRenderBufferStart_DE558;
	for (int row = 1; row < viewPort.Height_DE568; row++)
	{
		for (int column = 0; column < viewPort.Width_DE564 - 1; column++)
		{
			const uint8_t* below = line + iScreenWidth_DE560;
			const int left = ColourLookupTable_F6EE0[COLOUR_BLEND_LOOKUP_OFFSET + (below[column] << 8 | line[column])];
			const int right = ColourLookupTable_F6EE0[COLOUR_BLEND_LOOKUP_OFFSET + (below[column + 1] << 8 | line[column + 1])];
			line[column] = ColourLookupTable_F6EE0[COLOUR_BLEND_LOOKUP_OFFSET + (right << 8 | left)];
		}
		line += iScreenWidth_DE560;
	}
}

// The sky texture (256x256, 1024x1024 with the big textures; row = v, column = u) rotated by the roll. Column p of a
// line reads the texel of column p - 1 of the original step table, so columns 0 and 1 share their texel.
void GameRenderNG::DrawSky_40950(int16_t roll)
{
	SkyPlan& sky = m_sky;
	sky.size = x_BYTE_D41B5_texture_size == 128 ? 1024 : 256;
	sky.mask = sky.size - 1;
	const int width = viewPort.Width_DE564;
	const int rollIndex = roll & 0x7FF;
	sky.cosRoll = Maths::sin_DB750[512 + rollIndex] * sky.size / width;
	sky.sinRoll = Maths::sin_DB750[rollIndex] * sky.size / width;

	sky.columnU.assign(width, 0);
	sky.columnV.assign(width, 0);
	for (int column = 1; column < width; column++)
	{
		sky.columnU[column] = ((column - 1) * sky.cosRoll >> 16) & sky.mask;
		sky.columnV[column] = ((column - 1) * sky.sinRoll >> 16) & sky.mask;
	}

	const int centerX = (-(str_F2C20ar.sin_0x0d * str_F2C20ar.dword0x22) >> 16) + str_F2C20ar.dword0x24;
	const int centerY = str_F2C20ar.dword0x10 - (str_F2C20ar.cos_0x11 * str_F2C20ar.dword0x22 >> 16);
	sky.lineU = (m_cameraYaw_F2CC0 << 15) * (sky.size / 256) - (centerX * sky.cosRoll - centerY * sky.sinRoll);
	sky.lineV = -(sky.cosRoll * centerY + sky.sinRoll * centerX);
	if (m_queueing)
		m_skyQueued = true;
	else
		DrawSkyRows(0, viewPort.Height_DE568);
}

// The rows firstRow..endRow - 1 of the sky of m_sky.
void GameRenderNG::DrawSkyRows(int firstRow, int endRow)
{
	const SkyPlan& sky = m_sky;
	const int skySize = sky.size;
	const int skyMask = sky.mask;
	const int sinRoll = sky.sinRoll;
	const int cosRoll = sky.cosRoll;
	const int* columnU = sky.columnU.data();
	const int* columnV = sky.columnV.data();
	int lineU = static_cast<int>(static_cast<uint32_t>(sky.lineU) - static_cast<uint32_t>(sinRoll) * firstRow);
	int lineV = static_cast<int>(static_cast<uint32_t>(sky.lineV) + static_cast<uint32_t>(cosRoll) * firstRow);

	const int drawnWidth = DrawnWidth();
	uint8_t* line = ViewPortRenderBufferStart_DE558 + iScreenWidth_DE560 * firstRow;
	const uint8_t* previousLine = nullptr;
	int previousV = 0;
	for (int row = firstRow; row < endRow; row++)
	{
		const int startU = lineU >> 16;
		const int startV = lineV >> 16;
		if (sinRoll)
			for (int column = 0; column < drawnWidth; column++)
				line[column] = off_D41A8_sky[((startV + columnV[column]) & skyMask) * skySize + ((startU + columnU[column]) & skyMask)];
		else if (previousLine && (startV & skyMask) == previousV)//no roll: startU is the same, a texture row gives the same line
			memcpy(line, previousLine, drawnWidth);
		else
		{
			previousLine = line;
			previousV = startV & skyMask;
			const uint8_t* skyLine = &off_D41A8_sky[previousV * skySize];
			for (int column = 0; column < drawnWidth; column++)
				line[column] = skyLine[(startU + columnU[column]) & skyMask];
		}
		line += iScreenWidth_DE560;
		lineU = static_cast<int>(static_cast<uint32_t>(lineU) - static_cast<uint32_t>(sinRoll));
		lineV = static_cast<int>(static_cast<uint32_t>(lineV) + static_cast<uint32_t>(cosRoll));
	}
}


// The terrain around the camera: a grid of m_tileColumns x m_tileRows tiles (m_tiles, index = row * columns + column) is laid out
// in the view direction, projected, rotated by the roll and drawn from the farthest row to the nearest with the sprites
// standing on the tiles. A cave has a ceiling (second heightmap), with reflections the water mirrors the terrain.
void GameRenderNG::DrawTerrainAndParticles_3C080(__int16 posX, __int16 posY, __int16 yaw, signed int posZ, int pitch, int16_t roll, int fov)
{
	// with render threads over 640x480: the queue of the terrain, drawn before the sprites and at the end
	struct QueueScope
	{
		GameRenderNG& render;
		explicit QueueScope(GameRenderNG& renderNG) : render(renderNG) { render.m_queueing = render.m_largeScreen && !render.m_renderThreads.empty(); }
		~QueueScope()
		{
			render.FlushQueue();
			render.m_queueing = false;
		}
	} queueScope(*this);
	m_shadows_F2CC7 = D41A0_0.m_GameSettings.m_Graphics.m_wShadows;//21d080
	m_notDay_D4320 = D41A0_0.terrain_2FECE.MapType != MapType_t::Day;
	str_F2C20ar.dword0x10 = viewPort.Height_DE568 >> 1;
	m_cameraX_F2CC4 = posX;
	m_cameraYaw_F2CC0 = yaw & 0x7FF;
	m_cameraY_F2CC2 = posY;
	const int yawShifted = (yaw & 0x7FF) + 256;
	str_F2C20ar.dword0x20 = posZ;
	str_F2C20ar.dword0x24 = m_eyeShift_D4324 + (viewPort.Width_DE564 >> 1);
	str_F2C20ar.cos2_0x0f = Maths::sin_DB750[256 + yawShifted];
	str_F2C20ar.sin2_0x17 = Maths::sin_DB750[yawShifted - 256];
	const int quadrant = (yawShifted >> 9) & 3;
	const int quadrantYaw = ((yawShifted & 0x1FF) - 256) & 0x7FF;
	const int quadrantCos = Maths::sin_DB750[512 + quadrantYaw];
	const int quadrantSin = Maths::sin_DB750[quadrantYaw];
	SetBillboards_3B560(-roll & 0x7FF);//21d1aa
	str_F2C20ar.dword0x18 = 7 * Maths::sub_7277A_radix_3d(viewPort.Width_DE564 * viewPort.Width_DE564 + viewPort.Height_DE568 * viewPort.Height_DE568) * fov >> 11;
	const uint8_t* steps = &m_tileSteps_D4328[10 * quadrant];

	// the grid: x across the view, y (depth) along it
	int across = 0;
	int along = 0;
	switch (quadrant)
	{
	case 0:
		along = (posY & 0xFF) - 256 * m_viewDistanceScale;
		across = -(posX & 0xFF) - 4864 * m_viewDistanceScale;
		break;
	case 1:
		across = -(posY & 0xFF) - 4864 * m_viewDistanceScale;
		along = -(posX & 0xFF);
		break;
	case 2:
		across = (posX & 0xFF) - 4864 * m_viewDistanceScale;
		along = -(posY & 0xFF);
		break;
	case 3:
		across = (posY & 0xFF) - 4864 * m_viewDistanceScale;
		along = (posX & 0xFF) - 256 * m_viewDistanceScale;
		break;
	}
	m_cameraRow = std::clamp((-along - 1) / 256 + 1, 1, m_tileRows - 1);
	m_standardFirstRow = quadrant == 0 || quadrant == 3 ? m_viewDistanceScale - 1 : 0;
	m_standardFirstColumn = 19 * (m_viewDistanceScale - 1);
	for (int column = 0; column < m_tileColumns; column++, across += 256)
	{
		for (int row = 0; row < m_tileRows; row++)
		{
			type_E9C38_smalltit& tile = m_tiles[row * m_tileColumns + column];
			tile.x_0 = static_cast<int>(static_cast<int64_t>(across) * quadrantCos >> 16);//64 bits: a view distance over 6
			tile.y_12 = static_cast<int>(static_cast<int64_t>(across) * quadrantSin >> 16);
			tile.triangleFeatures_38 = across < 0 ? 0 : 4;
		}
	}
	for (int row = 0; row < m_tileRows; row++, along += 256)
	{
		const int shiftX = static_cast<int>(static_cast<int64_t>(along) * quadrantSin >> 16);
		const int shiftY = static_cast<int>(static_cast<int64_t>(along) * quadrantCos >> 16);
		for (int column = 0; column < m_tileColumns; column++)
		{
			m_tiles[row * m_tileColumns + column].x_0 -= shiftX;
			m_tiles[row * m_tileColumns + column].y_12 += shiftY;
		}
	}

	str_F2C20ar.dword0x15_tileRenderCutOffDistance = static_cast<int>(m_cutOffDistance);//21d2df
	str_F2C20ar.dword0x12_FogThickness = static_cast<int>(m_fogEnd - m_fogStart);
	str_F2C20ar.dword0x22 = pitch * viewPort.Width_DE564 >> 8;
	const int rollIndex = roll & 0x7FF;
	str_F2C20ar.cos_0x11 = Maths::sin_DB750[512 + rollIndex];
	str_F2C20ar.dword0x16_FogEnd = static_cast<int>(m_fogEnd);
	str_F2C20ar.sin_0x0d = Maths::sin_DB750[rollIndex];
	str_F2C20ar.dword0x13_FogStart = static_cast<int>(m_fogStart);

	if (!D41A0_0.m_GameSettings.m_Graphics.m_wSky || isCaveLevel_D41B6)
	{
		uint8_t* line = ViewPortRenderBufferStart_DE558;
		for (int row = 0; row < viewPort.Height_DE568; row++, line += iScreenWidth_DE560)
			memset(line, keyColor1_D4B7C, DrawnWidth());
	}
	else
		DrawSky_40950(roll);

	const uint8_t mapX = steps[0] + ((posX >> 8) & 0xFF);
	const uint8_t mapY = steps[1] + ((posY >> 8) & 0xFF);
	TerrainVertex quad[4];
	if (isCaveLevel_D41B6)//21d3e3
	{
		ProjectTiles(TerrainProjection::Cave, mapX, mapY, steps, quadrant, posZ);
		RotateTiles(true);
		ForEachTileFarToNear([&](int tile) {
			DrawCaveTile(quad, tile);
		});
		return;
	}
	if (D41A0_0.m_GameSettings.m_Graphics.m_wReflections)//21de79
	{
		ProjectTiles(TerrainProjection::Reflections, mapX, mapY, steps, quadrant, posZ);
		RotateTiles(true);
		if (posZ < 4096)
		{
			ForEachTileFarToNear([&](int tile) {
				DrawReflectedTile(quad, tile);
			});
		}
		ForEachTileFarToNear([&](int tile) {
			DrawTerrainTile(quad, tile, true);
		});
		return;
	}
	ProjectTiles(TerrainProjection::Plain, mapX, mapY, steps, quadrant, posZ);//21eb44
	str_F2C20ar.sin_0x0d = Maths::sin_DB750[rollIndex];//21edb7
	str_F2C20ar.cos_0x11 = Maths::sin_DB750[512 + rollIndex];
	RotateTiles(false);
	ForEachTileFarToNear([&](int tile) {//21ee85
		DrawTerrainTile(quad, tile, false);
	});
}

// Screen positions (pnt1/pnt2; pnt4 the ceiling or the reflection), light (pnt5) and texture of the tiles. The map is walked
// from (mapX, mapY) by the steps of the view quadrant; the tiles farther than the cut-off or behind the camera get flag 2.
void GameRenderNG::ProjectTiles(TerrainProjection projection, uint8_t mapX, uint8_t mapY, const uint8_t* steps, int quadrant, int posZ)
{
	const int turn = D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].Turn_2BE0_11248;
	int index = 0;
	for (int row = 0; row < m_tileRows; row++)
	{
		for (int column = 0; column < m_tileColumns; column++, index++)
		{
			type_E9C38_smalltit& tile = m_tiles[index];
			const int map = mapY << 8 | mapX;
			const int shade = (mapShading_12B4E0[map] << 8) + 128;
			int depth = tile.y_12;
			const int64_t distance = static_cast<int64_t>(depth) * depth + static_cast<int64_t>(tile.x_0) * tile.x_0;
			tile.haveBillboard_36 = 0;
			if (depth <= -256 || distance >= m_cutOffDistance)
				tile.triangleFeatures_38 |= 2;
			else
			{
				if (depth < 128)
					depth = 128;
				tile.pnt1_16 = static_cast<int>(static_cast<int64_t>(str_F2C20ar.dword0x18) * tile.x_0 / depth);
				tile.alt_4 = 32 * mapHeightmap_11B4E0[map] - posZ;
				int wave;
				if (projection == TerrainProjection::Cave)
				{
					tile.inverse_alt_8 = (x_BYTE_14B4E0_second_heightmap[map] << 15 >> 10) - posZ;
					wave = 0;
					if (!mapTerrainType_10B4E0[map])
					{
						const int phase = 32 * turn;
						wave = (Maths::sin_DB750[(phase + (mapY << 7)) & 0x7FF] >> 8) * (Maths::sin_DB750[((mapX << 7) + phase) & 0x7FF] >> 8);
						tile.alt_4 -= wave >> 13;
						if (shade >= 14464)
							wave = 0;
					}
				}
				else
				{
					const int phase = turn << 6;
					wave = (Maths::sin_DB750[(phase + (mapY << 7)) & 0x7FF] >> 8) * (Maths::sin_DB750[((mapX << 7) + phase) & 0x7FF] >> 8);
					if (projection == TerrainProjection::Reflections)
						tile.inverse_alt_8 = -(mapHeightmap_11B4E0[map] * ((wave >> 4) + 0x8000) >> 10) - posZ;
					if (mapAngle_13B4E0[map] & 8)
						tile.alt_4 -= wave >> 10;
					if (!(mapAngle_13B4E0[map] & 8) || shade >= 14464)
						wave = 0;
				}
				const int light = (shade << 8) + 8 * wave;
				if (distance <= m_fogStart)
					tile.pnt5_32 = light;
				else if (distance < m_fogEnd)
					tile.pnt5_32 = static_cast<int>(light * (m_fogEnd - distance) / (m_fogEnd - m_fogStart));
				else
					tile.pnt5_32 = 0;
				if (projection == TerrainProjection::Cave && (mapAngle_13B4E0[map] & 8))
					tile.triangleFeatures_38 |= 0x80;
				tile.pnt2_20 = str_F2C20ar.dword0x22 + static_cast<int>(static_cast<int64_t>(str_F2C20ar.dword0x18) * tile.alt_4 / depth);
				if (projection != TerrainProjection::Plain)
					tile.pnt4_28 = str_F2C20ar.dword0x22 + static_cast<int>(static_cast<int64_t>(str_F2C20ar.dword0x18) * tile.inverse_alt_8 / depth);

				const uint8_t textureX = mapX + steps[2];
				const uint8_t textureY = mapY + steps[3];
				const int textureMap = textureY << 8 | textureX;
				tile.textIndex_41 = mapTerrainType_10B4E0[textureMap];
				if (projection == TerrainProjection::Reflections && Maths::x_BYTE_D41D8[164 + tile.textIndex_41])
					tile.triangleFeatures_38 |= 0x80;
				if (projection != TerrainProjection::Plain && D41A0_0.m_GameSettings.str_0x2196.flat_0x2199)
					tile.triangleFeatures_38 |= 0x1000;
				tile.textAtyp_43 = Maths::x_BYTE_D41D8[tile.textIndex_41];
				tile.textUV_42 = quadrant + ((mapAngle_13B4E0[textureMap] >> 2) & 0x1C);
				const uint8_t entityX = textureX + steps[4];
				const uint8_t entityY = textureY + steps[5];
				tile.haveBillboard_36 = mapEntityIndex_15B4E0[entityY << 8 | entityX];
			}
			tile.triangleFeatures_38 |= (mapX + mapY) & 1;
			mapX += steps[8];
			mapY += steps[9];
		}
		mapX += steps[6];
		mapY += steps[7];
	}
}

// Rotation by the roll around the view center; flags 8/0x10/0x20/0x40: pnt1/pnt2 left of/right of/above/below the
// view port, 0x100/0x200/0x400/0x800 the same for pnt3/pnt4 (the second point set, ceiling or reflection).
void GameRenderNG::RotateTiles(bool secondPoints)
{
	const int width = viewPort.Width_DE564;
	const int height = viewPort.Height_DE568;
	const int64_t cosRoll = str_F2C20ar.cos_0x11;//64 bits: the tiles beside the camera project far out of a big screen
	const int64_t sinRoll = str_F2C20ar.sin_0x0d;
	for (int index = 0; index < m_tileColumns * m_tileRows; index++)
	{
		type_E9C38_smalltit& tile = m_tiles[index];
		const int x = tile.pnt1_16;
		tile.pnt1_16 = static_cast<int>((x * cosRoll - sinRoll * tile.pnt2_20) >> 16) + str_F2C20ar.dword0x24;
		tile.pnt2_20 = str_F2C20ar.dword0x10 - static_cast<int>((x * sinRoll + cosRoll * tile.pnt2_20) >> 16);
		if (tile.pnt1_16 < 0)
			tile.triangleFeatures_38 |= 8;
		else if (tile.pnt1_16 >= width)
			tile.triangleFeatures_38 |= 0x10;
		if (tile.pnt2_20 < 0)
			tile.triangleFeatures_38 |= 0x20;
		else if (tile.pnt2_20 >= height)
			tile.triangleFeatures_38 |= 0x40;
		if (!secondPoints)
			continue;
		tile.pnt3_24 = static_cast<int>((x * cosRoll - sinRoll * tile.pnt4_28) >> 16) + str_F2C20ar.dword0x24;
		tile.pnt4_28 = str_F2C20ar.dword0x10 - static_cast<int>((x * sinRoll + cosRoll * tile.pnt4_28) >> 16);
		if (tile.pnt3_24 < 0)
			tile.triangleFeatures_38 |= 0x100;
		else if (tile.pnt3_24 >= width)
			tile.triangleFeatures_38 |= 0x200;
		if (tile.pnt4_28 < 0)
			tile.triangleFeatures_38 |= 0x400;
		else if (tile.pnt4_28 >= height)
			tile.triangleFeatures_38 |= 0x800;
	}
}

// Rows m_tileRows - 1 down to the camera row, then the rows behind it from row 1 up (a longer view has more of them), the
// camera row last; row 0 is only the far edge of row 1. The rows on the two sides of the camera do not cover each other.
// In a row from the left up to the tile whose right neighbour lies behind the camera (flag 4), then from the right back to it.
void GameRenderNG::ForEachTileFarToNear(const std::function<void(int)>& drawTile)
{
	const auto drawRow = [&](int row)
	{
		const int rowStart = row * m_tileColumns;
		int tile = rowStart;
		int left = m_tileColumns - 1;
		for (; left; left--, tile++)
		{
			if (m_tiles[tile + 1].triangleFeatures_38 & 4)
				break;
			drawTile(tile);
		}
		if (left)
			for (int back = rowStart + m_tileColumns - 2; back >= tile; back--)
				drawTile(back);
	};
	for (int row = m_tileRows - 1; row > m_cameraRow; row--)
		drawRow(row);
	for (int row = 1; row < m_cameraRow; row++)
		drawRow(row);
	drawRow(m_cameraRow);
}

// The corners of the tile: [0] the tile of the previous row, [1] its right neighbour, [2] the right neighbour, [3] the tile.
void GameRenderNG::LoadTileQuad(TerrainVertex quad[4], int tile, bool secondPoints, uint8_t& anyFlags, uint8_t& allFlags)
{
	const int corners[4] = { tile - m_tileColumns, tile - m_tileColumns + 1, tile + 1, tile };
	anyFlags = 0;
	allFlags = 0xFF;
	for (int corner = 0; corner < 4; corner++)
	{
		const type_E9C38_smalltit& point = m_tiles[corners[corner]];
		quad[corner].x = secondPoints ? point.pnt3_24 : point.pnt1_16;
		quad[corner].y = secondPoints ? point.pnt4_28 : point.pnt2_20;
		quad[corner].light = point.pnt5_32;
		anyFlags |= point.triangleFeatures_38 & 0xFF;
		allFlags &= point.triangleFeatures_38 & 0xFF;
	}
}

// Flat shading (flag 0x1000): the average light of the corners, else Gouraud.
void GameRenderNG::SetTileShading(const type_E9C38_smalltit& tile, const TerrainVertex quad[4])
{
	if (tile.triangleFeatures_38 & 0x1000)
	{
		m_triangleMode_E126D = 7;
		m_flatLight_E126C = (quad[1].light + quad[2].light + quad[3].light + quad[0].light) >> 18;
	}
	else
		m_triangleMode_E126D = 5;
}

// The texture coordinates of the corners and the texture of the tile.
void GameRenderNG::SetTileTexture(TerrainVertex quad[4], const type_E9C38_smalltit& tile, int textureIndex)
{
	const uint32_t* uv = UVTable_D4350[tile.textUV_42];
	quad[3].u = uv[0];
	quad[3].v = uv[1];
	quad[2].u = uv[2];
	quad[2].v = uv[3];
	quad[1].u = uv[4];
	quad[1].v = uv[5];
	quad[0].u = uv[6];
	quad[0].v = uv[7];
	m_triangleTexture_DE55C = x_DWORD_DDF50_texture_adresses[textureIndex];
}

// The two triangles of the tile, split by its diagonal (flag 1); upside down: the other winding (ceiling, reflection).
void GameRenderNG::DrawTileTriangles(TerrainVertex quad[4], const type_E9C38_smalltit& tile, bool upsideDown)
{
	if (tile.triangleFeatures_38 & 1)
	{
		if (upsideDown)
		{
			DrawTriangleInProjectionSpace_B6253(quad[3], quad[0], quad[2]);
			DrawTriangleInProjectionSpace_B6253(quad[0], quad[1], quad[2]);
		}
		else
		{
			DrawTriangleInProjectionSpace_B6253(quad[3], quad[2], quad[0]);
			DrawTriangleInProjectionSpace_B6253(quad[0], quad[2], quad[1]);
		}
	}
	else
	{
		if (upsideDown)
		{
			DrawTriangleInProjectionSpace_B6253(quad[3], quad[1], quad[2]);
			DrawTriangleInProjectionSpace_B6253(quad[3], quad[0], quad[1]);
		}
		else
		{
			DrawTriangleInProjectionSpace_B6253(quad[3], quad[2], quad[1]);
			DrawTriangleInProjectionSpace_B6253(quad[3], quad[1], quad[0]);
		}
	}
}

// Cave: the ceiling (texture 1, pnt3/pnt4) and the floor with its sprites; nothing when all corners have flag 0x80.
void GameRenderNG::DrawCaveTile(TerrainVertex quad[4], int index)
{
	const type_E9C38_smalltit& tile = m_tiles[index];
	uint8_t anyFlags;
	uint8_t allFlags;
	LoadTileQuad(quad, index, true, anyFlags, allFlags);
	if (!(allFlags & 0x80))
	{
		SetTileShading(tile, quad);
		if (!(anyFlags & 2))
		{
			SetTileTexture(quad, tile, 1);
			m_triangleMode_E126D = 5;
			DrawTileTriangles(quad, tile, true);
		}
	}
	LoadTileQuad(quad, index, false, anyFlags, allFlags);
	if (allFlags & 0x80)
		return;
	SetTileShading(tile, quad);
	if (!(anyFlags & 2) && !(allFlags & 0x78))
	{
		SetTileTexture(quad, tile, tile.textIndex_41);
		DrawTileTriangles(quad, tile, false);
	}
	if (tile.haveBillboard_36)
		DrawSprites_3E360(index);
}

// The reflection in the water: the mirrored tile (pnt3/pnt4) and the mirrored sprites.
void GameRenderNG::DrawReflectedTile(TerrainVertex quad[4], int index)
{
	const type_E9C38_smalltit& tile = m_tiles[index];
	uint8_t anyFlags;
	uint8_t allFlags;
	LoadTileQuad(quad, index, true, anyFlags, allFlags);
	if (tile.textIndex_41)
	{
		SetTileShading(tile, quad);
		if (!(anyFlags & 2))
		{
			SetTileTexture(quad, tile, tile.textIndex_41);
			m_triangleMode_E126D = 5;
			DrawTileTriangles(quad, tile, true);
		}
	}
	if (tile.haveBillboard_36)
		sub_3FD60(index);
}

// The terrain tile and its sprites; with water the tiles with flag 0x80 are drawn transparent (mode 26) over the reflection.
void GameRenderNG::DrawTerrainTile(TerrainVertex quad[4], int index, bool water)
{
	const type_E9C38_smalltit& tile = m_tiles[index];
	uint8_t anyFlags;
	uint8_t allFlags;
	LoadTileQuad(quad, index, false, anyFlags, allFlags);
	if (water && (tile.triangleFeatures_38 & 0x80))
		m_triangleMode_E126D = 26;
	else
		SetTileShading(tile, quad);
	if (!(anyFlags & 2) && !(allFlags & 0x78))
	{
		SetTileTexture(quad, tile, tile.textIndex_41);
		DrawTileTriangles(quad, tile, false);
	}
	if (tile.haveBillboard_36)
		DrawSprites_3E360(index);
}

// Help (showHelp_10): the help text (str_E2A74) of the entity on the screen; the nearest entity of a text wins,
// a text being shown follows its entity on the screen.
void GameRenderNG::sub_88740(type_entity_0x6E8E* entity, int16_t posX, int16_t posY)
{
	if (str_unk_1804B0ar.PopupStatusByte_0x9e & 1)
		return;
	type_entity_0x6E8E* player = Entities_EA3E4[D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
	const bool own = entity->id_0x1A_26 == player->id_0x1A_26;
	const uint8_t model = entity->model_0x40_64;
	int text = 0;
	switch (entity->class_0x3F_63)
	{
	case 2:
		if (model == 1)
			text = 27;
		else if (model == 2)
			text = 22;
		break;
	case 3:
		if (model == 1)
			text = 28;
		else if (model == 2)
			text = own ? 24 : 25;
		else if (model == 3)
			text = own ? 23 : 26;
		break;
	case 5:
		if (own || (model >= 12 && model <= 14))
			break;
		if (model == 22)
		{
			if (entity->playerEntityIndex_0x94_148 != player->id_0x1A_26)
				text = 18;
		}
		else if ((entity->actionIndex_0x45_69 < 0xE8 || entity->actionIndex_0x45_69 > 0xEA)
			&& !((entity->StageVar2_0x49_73 == 14 || entity->StageVar2_0x49_73 == 13) && entity->parentId_0x28_40 == player->id_0x1A_26))
			text = 19;
		break;
	case 10:
		if (model == 39 && entity->playerEntityIndex_0x94_148 != player->id_0x1A_26)
			text = 18;
		break;
	case 15:
		if (!(entity->struct_byte_0xc_12_15.byte[0] & 1))
			text = 20;
		break;
	default:
		break;
	}
	if (!text)
		return;
	if (x_WORD_180660_VGA_type_resolution & 1)
	{
		posX *= 2;
		posY *= 2;
	}
	if (str_E2A74[text].axis_2[0] & 2)
	{
		if (entity == str_E2A74[text].dword_12 && !(str_unk_1804B0ar.byte_0x9f & 1))
		{
			str_E2A74[text].axis_2[3] = posX;
			str_E2A74[text].axis_2[0] |= 8;
			str_E2A74[text].axis_2[4] = posY;
			str_unk_1804B0ar.byte_0x9f |= 2;
		}
	}
	else
	{
		const int distance = Maths::EuclideanDistXYZ_58490(&player->position_0x4C_76, &entity->position_0x4C_76);
		if (!str_E2A74[text].dword_12 || (distance < str_E2A74[text].dword_20 && distance > 1024))
		{
			str_E2A74[text].dword_20 = distance;
			str_E2A74[text].dword_12 = entity;
			str_E2A74[text].axis_2[0] |= 8;
		}
	}
}

// The lines of the rolled view (8 octants of the roll): for each step along a line its screen offset (dword_1),
// its row (dword_2) and the offset against the previous step (dword_0). The steps where the line moves to the
// next row go into the row table (scratch, x_DWORD_E9C38_smalltit + 59360 in the original).
void GameRenderNG::SetBillboards_3B560(int16_t roll)
{
	struct Octant
	{
		bool alongWidth;
		int primaryX, primaryY;
		int secondaryX, secondaryY;
		int strideX, strideY;
	};
	static const Octant octants[8] = {
		{ true, 1, 0, 0, 1, 0, 1 },
		{ false, 0, 1, 1, 0, -1, 0 },
		{ false, 0, 1, -1, 0, -1, 0 },
		{ true, -1, 0, 0, 1, 0, -1 },
		{ true, -1, 0, 0, -1, 0, -1 },
		{ false, 0, -1, -1, 0, 1, 0 },
		{ false, 0, -1, 1, 0, 1, 0 },
		{ true, 1, 0, 0, -1, 0, 1 }
	};
	const int rollIndex = roll & 0x7FF;
	const int octantIndex = rollIndex >> 8;
	const Octant& octant = octants[octantIndex];
	const int angle = rollIndex - 512 * (octantIndex / 2);
	const bool diagonal = angle == 256;
	str_F2C20ar.dword0x1e = octantIndex;
	str_F2C20ar.dword0x27 = Maths::sin_DB750[angle];
	str_F2C20ar.dword0x1b = Maths::sin_DB750[angle + 512];
	if (diagonal)
		str_F2C20ar.dword0x1f = 0x10000;
	else if (octantIndex & 1)
		str_F2C20ar.dword0x1f = (str_F2C20ar.dword0x1b << 8) / (str_F2C20ar.dword0x27 >> 8);
	else
		str_F2C20ar.dword0x1f = (str_F2C20ar.dword0x27 << 8) / (str_F2C20ar.dword0x1b >> 8);

	const int width = viewPort.Width_DE564;
	const int height = viewPort.Height_DE568;
	const int count = octant.alongWidth ? width : height;
	const int primaryStep = octant.primaryX + octant.primaryY * iScreenWidth_DE560;
	const int secondaryStep = octant.secondaryX + octant.secondaryY * iScreenWidth_DE560;
	const int fraction = str_F2C20ar.dword0x1f & 0xFFFF;
	int32_t* rowSteps = reinterpret_cast<int32_t*>(&m_scratch[m_rowsOffset]);
	int error = 0;
	int offset = 0;
	int rows = 0;
	for (int step = 1; step <= count; step++)
	{
		m_viewLines_F0E20[step - 1].dword_1 = offset;
		m_viewLines_F0E20[step - 1].dword_2 = rows;
		error += fraction;
		if (diagonal || error > 0xFFFF)
		{
			offset += secondaryStep;
			rowSteps[rows++] = step;
		}
		error &= 0xFFFF;
		offset += primaryStep;
	}

	str_F2C20ar.dword0x1d = rows;
	str_F2C20ar.dword0x21 = -rows;
	str_F2C20ar.width0x25 = octant.alongWidth ? height : width;
	str_F2C20ar.height0x26 = count;
	str_F2C20ar.Height_0x19 = str_F2C20ar.width0x25 - rows;
	str_F2C20ar.dword0x1c = height + width;
	str_F2C20ar.pbyte0x1a = &m_scratch[m_rowsOffset + 4 * (rows - 1)];
	str_F2C20ar.dword0x23_stride = octant.strideX + octant.strideY * iScreenWidth_DE560;

	// step -1: in the original memory unk_F0A20x lies right before unk_F0E20x, a sprite span can start there
	memcpy(&m_viewLines_F0E20[-1], &unk_F0A20x[1012], sizeof(type_unk_F0E20x));
	int previousOffset = m_viewLines_F0E20[-1].dword_1;
	for (int step = 0; step < count; step++)
	{
		m_viewLines_F0E20[step].dword_0 = m_viewLines_F0E20[step].dword_1 - previousOffset;
		previousOffset = m_viewLines_F0E20[step].dword_1;
	}
}

// The name tag of a wizard above his sprite (screenX, screenY its top left, spriteWidth): the framed name with
// the health bar under it, shortened at the right edge of the view port.
void GameRenderNG::DrawSorcererNameAndHealthBar_2CB30(type_entity_0x6E8E* entity, int16_t screenX, int screenY, int16_t spriteWidth)//20db30
{
	const int right = viewPort.PreWidth_EA3C4 + viewPort.PosX_EA3D0 - 4;
	const int bottom = viewPort.PreHeight_EA3C0 + viewPort.PosY_EA3CC - 22;
	const int wizard = entity->dword_0xA4_164x->playerColorIndex_0x38_56;
	char name[32];
	strcpy(name, D41A0_0.array_0x2BDE[wizard].WizardName_0x39f_2BFA_12157);
	const uint8_t wizardColor = playersColors_E88E0x[GetTrueWizardNumber_61790(wizard)][0];
	const uint8_t barColor = (*xadataclrd0dat.colorPalette_var28)[0];
	const uint8_t* frameColors = str_D94F0_bldgprmbuffer[static_cast<std::underlying_type<MapType_t>::type>(D41A0_0.terrain_2FECE.MapType)];
	const uint8_t lightColor = frameColors[2];
	const uint8_t darkColor = frameColors[3];
	const uint8_t backgroundColor = frameColors[0];
	int16_t center = (spriteWidth >> 1) + screenX;
	if (x_WORD_180660_VGA_type_resolution & 1)
	{
		center *= 2;
		screenY *= 2;
	}
	const int16_t left = viewPort.PosX_EA3D0 + center;
	const int top = viewPort.PosY_EA3CC + screenY - 20;
	if (left < viewPort.PosX_EA3D0 || static_cast<int16_t>(top) < viewPort.PosY_EA3CC || left >= right || static_cast<int16_t>(top) >= bottom)
		return;
	int length = strlen(name);
	int width = 8 * length + 4;
	if (left + width > right)
	{
		width = right - left;
		length = (right - left - 4) / 8;
	}
	if (length <= 0)
		return;
	name[length] = 0;
	const int16_t frameWidth = width + 2;
	DrawLine_2BC80(left, top, width + 2, 18, backgroundColor);
	DrawLine_2BC80(left, top, frameWidth, 2, lightColor);
	DrawLine_2BC80(left, top + 16, frameWidth, 2, darkColor);
	DrawLine_2BC80(left, top, 2, 16, lightColor);
	DrawLine_2BC80(left + width, top, 2, 18, darkColor);
	DrawText_2BC10(name, left + 4, top, wizardColor);
	DrawLine_2BC80(left + 2, top + 14, width - 2, 2, barColor);
	if (entity->maxLife_0x4)
	{
		int health = entity->life_0x8 * (width - 2) / entity->maxLife_0x4;
		if (left + 2 + health > right - 2)
			health = right - 2 - (left + 2);
		if (health > 0)
			DrawLine_2BC80(left + 2, top + 14, health, 2, wizardColor);
	}
}

// The entities standing on the tile (a list by oldMapEntity_0x16_22): the shadow on the terrain, then the entity.
void GameRenderNG::DrawSprites_3E360(int tileIndex)//21f360
{
	FlushQueue();
	Type_Sprite* sprite = nullptr;
	uint16_t entityIndex = m_tiles[tileIndex].haveBillboard_36;
	do
	{
		type_entity_0x6E8E* entity = Entities_EA3E4[entityIndex];
		str_F2C20ar.dword0x14x = entity;
		if (!(entity->struct_byte_0xc_12_15.byte[0] & 0x21))
			DrawEntity(entity, m_tiles[tileIndex], sprite);
		entityIndex = str_F2C20ar.dword0x14x->oldMapEntity_0x16_22;
	} while (entityIndex);
}

// The entity on the tile in the view space (across, depth). dx and dy wrap in 16 bits (128 tiles); a view over 6 holds more
// copies of the map (256 tiles), there the entity goes to the copy of the tile.
void GameRenderNG::EntityViewPosition(const type_entity_0x6E8E* entity, const type_E9C38_smalltit& tile, int& across, int& depth) const
{
	const int64_t cos = str_F2C20ar.cos2_0x0f;
	const int64_t sin = str_F2C20ar.sin2_0x17;
	int64_t dx = static_cast<int16_t>(entity->position_0x4C_76.x - m_cameraX_F2CC4);
	int64_t dy = static_cast<int16_t>(m_cameraY_F2CC2 - entity->position_0x4C_76.y);
	if (m_viewDistanceScale > 6)
	{
		const int64_t tileDx = (cos * tile.x_0 + sin * tile.y_12) >> 16;
		const int64_t tileDy = (cos * tile.y_12 - sin * tile.x_0) >> 16;
		dx += (tileDx - dx + 0x8000) >> 16 << 16;
		dy += (tileDy - dy + 0x8000) >> 16 << 16;
	}
	across = static_cast<int>((cos * dx - sin * dy) >> 16);
	depth = static_cast<int>((sin * dx + cos * dy) >> 16);
}

// The shadow and the entity; nothing more when a sprite cannot be loaded (MainInitTmaps_71520).
void GameRenderNG::DrawEntity(type_entity_0x6E8E* entity, const type_E9C38_smalltit& tile, Type_Sprite*& sprite)
{
	int across;
	int depth;
	EntityViewPosition(entity, tile, across, depth);
	if (m_shadows_F2CC7 && !tile.textAtyp_43 && !(entity->struct_byte_0xc_12_15.word[1] & 0x808))
	{
		//21f40c
		const int height = sub_B5C60_getTerrainAlt2(entity->position_0x4C_76.x, entity->position_0x4C_76.y) - str_F2C20ar.dword0x20;
		const int64_t distance = static_cast<int64_t>(depth) * depth + static_cast<int64_t>(across) * across;
		if (depth > 64 && distance < m_cutOffDistance)
		{
			str_F2C20ar.dword0x00 = SpriteLight(distance);
			const Type_SpriteDef_D951C& def = spriteDefsTable_D951C[str_F2C20ar.dword0x14x->word_0x5A_90];
			if (!def.lightingTableIdx_10)
			{
				ProjectSprite(across, height, depth);
				if (ChooseSprite(str_F2C20ar.dword0x14x, def, depth, distance, false, false, sprite) == SpriteChoice::Skip)
					return;
				str_F2C20ar.dword0x02_data = sprite->textureBuffer;
				sprite->word_0 |= 8;
				str_F2C20ar.dword0x01_visibilityIdx = m_spriteTables_D4750[(str_F2C20ar.dword0x00 == 0x2000 ? 0 : 6) + def.lightingTableIdx_10];
				str_F2C20ar.dword0x0c_realHeight >>= 2;
				if (str_F2C20ar.dword0x09_realWidth > 0 && str_F2C20ar.dword0x0c_realHeight > 0)
				{
					const int light = str_F2C20ar.dword0x00 >> 2;
					str_F2C20ar.dword0x00 = m_notDay_D4320 ? 0x2000 - light : light + 0x2000;
					str_F2C20ar.dword0x01_visibilityIdx = 8;
					DrawSprite_41BD3((uint32_t)SpritePass::Shadow);
				}
			}
		}
	}

	type_entity_0x6E8E* owner = str_F2C20ar.dword0x14x;
	const int z = owner->struct_byte_0xc_12_15.byte[3] >= 0 ? owner->position_0x4C_76.z : owner->position_0x4C_76.z - 160;
	if (owner->struct_byte_0xc_12_15.byte[3] & 0x20)
	{
		// a part of a bigger entity: shifted by the direction of the parent
		const type_D404C& offsets = str_D404C[owner->byte_0x3B_59];
		switch (((Entities_EA3E4[owner->word_0x32_50]->yaw_0x1C_28 - m_cameraYaw_F2CC0) >> 7) & 0xF)
		{
		case 0:
		case 15:
			depth -= offsets.word_16;
			break;
		case 1:
		case 14:
			depth -= offsets.word_18;
			break;
		case 2:
		case 13:
			depth -= offsets.word_20;
			break;
		case 5:
		case 10:
			depth += offsets.word_20;
			break;
		case 6:
		case 9:
			depth += offsets.word_18;
			break;
		case 7:
		case 8:
			depth += offsets.word_16;
			break;
		default:
			break;
		}
	}
	const int64_t distance = static_cast<int64_t>(depth) * depth + static_cast<int64_t>(across) * across;
	if (depth <= 64 || distance >= m_cutOffDistance)
		return;
	// a longer view: the game logic (sub_21490) reacts to the seen flag, only the entities of the original grid and
	// cut-off set it (the rows behind the camera and the far tiles are drawn only here)
	const int tileIndex = static_cast<int>(&tile - m_tiles);
	const int row = tileIndex / m_tileColumns - m_standardFirstRow;
	const int column = tileIndex % m_tileColumns - m_standardFirstColumn;
	m_standardView = m_viewDistanceScale == 1 || (row >= 1 && row <= 20 && column >= 0 && column <= 38 && distance < m_standardCutOff);
	str_F2C20ar.dword0x00 = SpriteLight(distance);
	const Type_SpriteDef_D951C& def = spriteDefsTable_D951C[owner->word_0x5A_90];
	ProjectSprite(across, z - str_F2C20ar.dword0x20, depth);
	if (ChooseSprite(owner, def, depth, distance, true, true, sprite) == SpriteChoice::Skip)
		return;
	sprite->word_0 |= 8;
	str_F2C20ar.dword0x02_data = sprite->textureBuffer;
	if (owner->struct_byte_0xc_12_15.word[1] & 0x380)//220ba6: mov cx, [ebx+0Eh]; test cx, 380h
	{
		const int8_t flags = owner->struct_byte_0xc_12_15.byte[3];
		if (flags & 2)
		{
			str_F2C20ar.dword0x01_visibilityIdx = 4;
			str_F2C20ar.dword0x07 = playersColors_E88E0x[Entities_EA3E4[owner->parentId_0x28_40]->dword_0xA4_164x->playerColorIndex_0x38_56][2];
		}
		else if (flags & 4)
		{
			str_F2C20ar.dword0x01_visibilityIdx = 5;
			str_F2C20ar.dword0x07 = playersColors_E88E0x[Entities_EA3E4[owner->parentId_0x28_40]->dword_0xA4_164x->playerColorIndex_0x38_56][2];
		}
		else if (owner->struct_byte_0xc_12_15.byte[2] < 0)
			str_F2C20ar.dword0x01_visibilityIdx = Visibility::Transparent;
		else if (flags & 1)
			str_F2C20ar.dword0x01_visibilityIdx = Visibility::InFog;
	}
	else
		str_F2C20ar.dword0x01_visibilityIdx = m_spriteTables_D4750[(str_F2C20ar.dword0x00 == 0x2000 ? 0 : 6) + def.lightingTableIdx_10];
	if (m_fadeVisibility >= 0)
		str_F2C20ar.dword0x01_visibilityIdx = m_fadeVisibility;
	str_F2C20ar.dword0x09_realWidth++;
	str_F2C20ar.dword0x0c_realHeight++;
	DrawSprite_41BD3((uint32_t)SpritePass::Main);
}

// The entities of the tile mirrored in the water (below the terrain, upside down).
void GameRenderNG::sub_3FD60(int tileIndex)
{
	FlushQueue();
	Type_Sprite* sprite = nullptr;
	type_entity_0x6E8E* entity = nullptr;
	uint16_t entityIndex = m_tiles[tileIndex].haveBillboard_36;
	do
	{
		if (entityIndex < 1000)
		{
			entity = Entities_EA3E4[entityIndex];
			if (!(entity->struct_byte_0xc_12_15.byte[0] & 0x21))
			{
				const int height = -entity->position_0x4C_76.z - str_F2C20ar.dword0x20;
				int across;
				int depth;
				EntityViewPosition(entity, m_tiles[tileIndex], across, depth);
				const int64_t distance = static_cast<int64_t>(depth) * depth + static_cast<int64_t>(across) * across;
				if (depth > 64 && distance < m_cutOffDistance)
				{
					str_F2C20ar.dword0x00 = SpriteLight(distance);
					const Type_SpriteDef_D951C& def = spriteDefsTable_D951C[entity->word_0x5A_90];
					ProjectSprite(across, height, depth);
					if (ChooseSprite(entity, def, depth, distance, true, false, sprite) != SpriteChoice::Skip)
					{
						str_F2C20ar.dword0x02_data = sprite->textureBuffer;
						sprite->word_0 |= 8;
						str_F2C20ar.dword0x01_visibilityIdx = m_fadeVisibility >= 0 ? m_fadeVisibility
							: m_spriteTables_D4750[(str_F2C20ar.dword0x00 == 0x2000 ? 0 : 6) + def.lightingTableIdx_10];
						str_F2C20ar.dword0x09_realWidth++;
						str_F2C20ar.dword0x0c_realHeight++;
						DrawSprite_41BD3((uint32_t)SpritePass::Reflection);
					}
				}
			}
		}
		entityIndex = entity->oldMapEntity_0x16_22;
	} while (entityIndex);
}

// The light of a sprite by its distance (fog): 0x2000 full, 0 in the fog end.
int GameRenderNG::SpriteLight(int64_t distance)
{
	if (distance <= m_fogStart)
		return 0x2000;
	if (distance < m_fogEnd)
		return static_cast<int>(32 * (m_fogEnd - distance) / (m_fogEnd - m_fogStart)) << 8;
	return 0;
}

// The flying creature (sprites 295-302) is meant to come out of the fog near the player: with a longer view it fades in the fog
// of view distance 1 in the steps of the vanishing of sub_21490 (InFog, Transparent) and is gone past it.
static bool FadesInStandardFog(int spriteIndex)
{
	return spriteIndex >= 295 && spriteIndex <= 302;
}

// The screen position of a sprite: across and height in the view space, rotated by the roll.
void GameRenderNG::ProjectSprite(int across, int height, int depth)
{
	const int64_t x = static_cast<int64_t>(across) * str_F2C20ar.dword0x18 / depth;//64 bits: near sprites project far out of a big screen
	const int64_t y = static_cast<int64_t>(str_F2C20ar.dword0x18) * height / depth + str_F2C20ar.dword0x22;
	str_F2C20ar.dword0x04_screenY = static_cast<int>((x * str_F2C20ar.cos_0x11 - str_F2C20ar.sin_0x0d * y) >> 16) + str_F2C20ar.dword0x24;
	str_F2C20ar.dword0x03_screenX = str_F2C20ar.dword0x10 - static_cast<int>((str_F2C20ar.sin_0x0d * x + y * str_F2C20ar.cos_0x11) >> 16);
}

// The sprite loaded (MainInitTmaps_71520 when needed) and marked as used in this frame; nullptr when it cannot be loaded.
Type_Sprite* GameRenderNG::UseSprite(int index)
{
	if (!m_ptrLoadedSprites_F66F0x[index] && !MainInitTmaps_71520(index))
		return nullptr;
	x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[index].word_8] = x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26;
	return *m_ptrLoadedSprites_F66F0x[index];
}

// The sprite of the entity by its kind: one picture, an animation frame or one of the directions the camera sees it from
// (the picture mirrored for the other side); sets its size on the screen. rotatedKind21: kind 21 is drawn rotated
// (m_uprightSprite_F2CC6) as the kinds 22-36, turnTable: the directions of kind 17 through m_spriteTables_D4750 (entity flag 0x40).
// An unknown kind keeps the previous sprite and size. With a longer view the sprites of FadesInStandardFog are skipped past
// the end of the standard fog and set m_fadeVisibility in it.
GameRenderNG::SpriteChoice GameRenderNG::ChooseSprite(type_entity_0x6E8E* entity, const Type_SpriteDef_D951C& def, int depth, int64_t distance, bool rotatedKind21, bool turnTable, Type_Sprite*& sprite)
{
	m_uprightSprite_F2CC6 = 0;
	m_fadeVisibility = -1;
	int direction = ((entity->yaw_0x1C_28 - m_cameraYaw_F2CC0) >> 7) & 0xF;
	int index;
	bool mirrored = false;
	switch (def.kind_12)
	{
	case 0:
	case 1:
	case 21:
		index = def.textureIdx_0;
		break;
	case 17:
		if (turnTable && (entity->struct_byte_0xc_12_15.byte[3] & 0x40))
			direction = m_spriteTables_D4750[44 + direction];
		mirrored = direction >= 8;
		index = def.textureIdx_0 + (mirrored ? 15 - direction : direction);
		break;
	case 18:
		index = def.textureIdx_0 + direction;
		break;
	case 19:
		mirrored = direction >= 8;
		index = def.textureIdx_0 + m_spriteTables_D4750[12 + direction];
		break;
	case 20:
		mirrored = direction >= 8;
		index = def.textureIdx_0 + m_spriteTables_D4750[28 + direction];
		break;
	default:
		if (def.kind_12 > 36)
			return SpriteChoice::KeepPrevious;
		if (def.kind_12 >= 22)
			m_uprightSprite_F2CC6 = 1;
		index = def.textureIdx_0 + entity->animationFrame_0x5C_92;
		break;
	}
	if (m_viewDistanceScale > 1 && FadesInStandardFog(index))
	{
		if (distance >= m_standardFogEnd)
			return SpriteChoice::Skip;
		if (distance > m_standardFogStart)
			m_fadeVisibility = distance - m_standardFogStart < (m_standardFogEnd - m_standardFogStart) / 2 ? Visibility::InFog : Visibility::Transparent;
	}
	Type_Sprite* chosen = UseSprite(index);
	if (!chosen)
		return SpriteChoice::Skip;
	sprite = chosen;
	if (def.kind_12 == 21 && rotatedKind21)
		m_uprightSprite_F2CC6 = 1;
	str_F2C20ar.dword0x08_width = sprite->width;
	str_F2C20ar.dword0x06_height = sprite->height;
	str_F2C20ar.dword0x0c_realHeight = str_F2C20ar.dword0x18 * def.worldSize_8 / depth;
	str_F2C20ar.dword0x09_realWidth = str_F2C20ar.dword0x0c_realHeight * str_F2C20ar.dword0x08_width / str_F2C20ar.dword0x06_height;
	str_F2C20ar.dword0x05 = mirrored ? -str_F2C20ar.dword0x08_width : str_F2C20ar.dword0x08_width;
	return SpriteChoice::Draw;
}

// The sprite set up in str_F2C20ar (data, size, screen position, mode in dword0x01_visibilityIdx). Rotated by the roll
// along the lines of SetBillboards_3B560, or upright and scaled (m_uprightSprite_F2CC6). The entity is anchored at its bottom
// center, the shadow and the reflection at their centers; for the entity also its name tag and help text.
void GameRenderNG::DrawSprite_41BD3(uint32 pass)
{
	// a big screen or a longer view: the sprites beside the camera project far out of the view port, their 16.16 positions
	// would overflow and wrap into it; not drawn when off the view port or past 16 bits (the flags below stay as they are)
	bool draw = true;
	if (m_largeScreen || m_viewDistanceScale > 1)
	{
		const int64_t size = static_cast<int64_t>(str_F2C20ar.dword0x09_realWidth) + str_F2C20ar.dword0x0c_realHeight;
		const int64_t x = str_F2C20ar.dword0x04_screenY;
		const int64_t y = str_F2C20ar.dword0x03_screenX;
		draw = x + size >= 0 && x - size < viewPort.Width_DE564 && y + size >= 0 && y - size < viewPort.Height_DE568
			&& x > -32768 && x < 32768 && y > -32768 && y < 32768 && size < 32768;
	}
	if (m_uprightSprite_F2CC6)
	{
		if (draw)
			DrawUprightSprite(pass);
		return;
	}
	if (pass == (uint32_t)SpritePass::Main)
	{
		str_F2C20ar.dword0x04_screenY -= ((str_F2C20ar.cos_0x11 * str_F2C20ar.dword0x09_realWidth >> 1) + str_F2C20ar.sin_0x0d * str_F2C20ar.dword0x0c_realHeight) >> 16;
		str_F2C20ar.dword0x03_screenX -= (str_F2C20ar.cos_0x11 * str_F2C20ar.dword0x0c_realHeight - (str_F2C20ar.sin_0x0d * str_F2C20ar.dword0x09_realWidth >> 1)) >> 16;
	}
	else if (pass == (uint32_t)SpritePass::Shadow || pass == (uint32_t)SpritePass::Reflection)
	{
		str_F2C20ar.dword0x04_screenY -= str_F2C20ar.cos_0x11 * str_F2C20ar.dword0x09_realWidth >> 17;
		str_F2C20ar.dword0x03_screenX -= -(str_F2C20ar.sin_0x0d * str_F2C20ar.dword0x09_realWidth) >> 17;
	}
	if (draw)
		DrawRotatedSprite(pass);
	if (pass != (uint32_t)SpritePass::Main)
		return;
	type_entity_0x6E8E* entity = str_F2C20ar.dword0x14x;
	if (draw && !x_D41A0_BYTEARRAY_4_struct.byteindex_207 && entity->class_0x3F_63 == 3 && (!entity->model_0x40_64 || entity->model_0x40_64 == 1))
		DrawSorcererNameAndHealthBar_2CB30(entity, str_F2C20ar.dword0x04_screenY, static_cast<int16_t>(str_F2C20ar.dword0x03_screenX), str_F2C20ar.dword0x09_realWidth);
	if (x_D41A0_BYTEARRAY_4_struct.showHelp_10)
		sub_88740(entity, str_F2C20ar.dword0x04_screenY + (str_F2C20ar.dword0x09_realWidth >> 1), str_F2C20ar.dword0x03_screenX + (str_F2C20ar.dword0x0c_realHeight >> 1));
	if ((entity->struct_byte_0xc_12_15.byte[3] & 0x40) && m_standardView)
		entity->subSpellIndex_0x2A_42 |= 0x40;
}

// The rotated sprite: its rows lie on the lines of the octant (dword0x1e) of SetBillboards_3B560, every line of the
// sprite is a span on one line of the view. The spans are clipped by the view port and the rows table, the texture
// columns go through the column table, the rows through dword0x0a_actIdx (upside down for the shadow and the reflection).
void GameRenderNG::DrawRotatedSprite(uint32 pass)
{
	const int octant = str_F2C20ar.dword0x1e;
	if ((unsigned)octant > 7)
		return;
	const bool odd = octant & 1;
	const int spanFactor = odd ? str_F2C20ar.dword0x27 : str_F2C20ar.dword0x1b;
	const int shiftFactor = odd ? str_F2C20ar.dword0x1b : str_F2C20ar.dword0x27;
	const int spanLength = spanFactor * str_F2C20ar.dword0x09_realWidth >> 16;
	if (spanLength <= 0)
		return;
	int lines = (str_F2C20ar.dword0x0c_realHeight << 16) / spanFactor;
	if (lines <= 0)
		return;
	const int screenX = str_F2C20ar.dword0x04_screenY;
	const int screenY = str_F2C20ar.dword0x03_screenX;
	const int width = str_F2C20ar.width0x25;
	const int height = str_F2C20ar.height0x26;
	if (octant == 7 && screenX >= height)
		return;
	int rowStep = (str_F2C20ar.dword0x06_height << 16) / lines;
	if (pass == (uint32_t)SpritePass::Main)
		str_F2C20ar.dword0x0a_actIdx = 0;
	else
	{
		str_F2C20ar.dword0x0a_actIdx = (lines - 1) * rowStep;
		rowStep = -rowStep;
	}
	const int spanShift = shiftFactor * str_F2C20ar.dword0x0c_realHeight / lines;

	// a: the line of the sprite position, b: its position along the line
	int a = 0;
	int b = 0;
	switch (octant)
	{
	case 0: a = screenY; b = screenX; break;
	case 1: a = screenX; b = screenY; break;
	case 2: a = width - screenX; b = screenY; break;
	case 3: a = screenY; b = height - screenX; break;
	case 4: a = width - screenY; b = height - screenX; break;
	case 5: a = width - screenX; b = height - screenY; break;
	case 6: a = screenX; b = height - screenY; break;
	case 7: a = width - screenY; b = screenX; break;
	}
	int spanStart = b << 16;
	int line = a - (str_F2C20ar.dword0x1f * b >> 16);
	if (!odd)
	{
		if (line < str_F2C20ar.dword0x21)
		{
			const int cut = str_F2C20ar.dword0x21 - line;
			lines -= cut;
			if (lines <= 0)
				return;
			str_F2C20ar.dword0x0a_actIdx += cut * rowStep;
			spanStart -= cut * spanShift;
			line = str_F2C20ar.dword0x21;
		}
	}
	else if (line < width)
	{
		if (line < str_F2C20ar.dword0x21)
			return;
	}
	else
	{
		const int cut = line - width;
		lines -= cut;
		if (lines <= 0)
			return;
		str_F2C20ar.dword0x0a_actIdx += cut * rowStep;
		spanStart += cut * spanShift;
		line = width;
	}

	const int screenWidth = iScreenWidth_DE560;
	int base = 0;
	switch (octant)
	{
	case 0: base = screenWidth * line; break;
	case 1: base = line; break;
	case 2: base = width - 1 - line; break;
	case 3: base = height + screenWidth * line - 1; break;
	case 4: base = height + screenWidth * (width - line - 1) - 1; break;
	case 5: base = (height - 1) * screenWidth + width - 1 - line; break;
	case 6: base = screenWidth * (height - 1) + line; break;
	case 7: base = screenWidth * (width - 1 - line); break;
	}
	uint8_t* lineStart = ViewPortRenderBufferStart_DE558 + base;

	SpriteSpan* spans = reinterpret_cast<SpriteSpan*>(&m_scratch[m_spansOffset]);
	const int32_t* rowLimits = reinterpret_cast<const int32_t*>(str_F2C20ar.pbyte0x1a);
	const int32_t* rowStarts = reinterpret_cast<const int32_t*>(&m_scratch[m_rowsOffset]);
	const int lastLine = width - str_F2C20ar.dword0x21;
	int minSkip = 9999999;
	if (!odd)
	{
		if (line <= 0)
		{
			if (lines > lastLine)
				lines = lastLine;
		}
		else if (line + lines > lastLine)
		{
			lines = lastLine - line;
			if (lines <= 0)
				return;
		}
		// a big screen: the lines from width on lie below the view port, the row limits past their table are stale in
		// the original (its lines land in the spare memory after a 320x200 screen)
		if (m_largeScreen && line + lines > width)
		{
			lines = width - line;
			if (lines <= 0)
				return;
		}
		const int count = lines;
		for (int i = 0; i < count; i++, spanStart -= spanShift)
		{
			SpriteSpan& span = spans[i];
			const int start = spanStart >> 16;
			if (start >= 0)
			{
				span.start = start;
				span.length = spanLength;
				span.skip = 0;
				minSkip = 0;
			}
			else
			{
				span.start = 0;
				span.length = spanLength + start;
				if (span.length <= 0)
				{
					lines -= count - i;
					break;
				}
				span.skip = -start;
				if (-start < minSkip)
					minSkip = -start;
			}
			if (span.length + span.start > height)
				span.length = height - span.start;
		}
		if (line + lines > str_F2C20ar.Height_0x19)
		{
			int spanIndex = 0;
			int limitIndex = 0;
			int count2 = 0;
			if (str_F2C20ar.Height_0x19 - line <= 0)
			{
				limitIndex = str_F2C20ar.Height_0x19 - line;
				count2 = lines;
			}
			else
			{
				spanIndex = str_F2C20ar.Height_0x19 - line;
				count2 = line + lines - str_F2C20ar.Height_0x19;
			}
			for (; count2; count2--, spanIndex++, limitIndex--)
			{
				SpriteSpan& span = spans[spanIndex];
				if (span.start + span.length > rowLimits[limitIndex])
				{
					if (span.start >= rowLimits[limitIndex])
					{
						lines -= count2;
						break;
					}
					span.length = rowLimits[limitIndex] - span.start;
				}
			}
		}
		for (int spanIndex = 0, rowIndex = -1 - line; line < 0; line++, spanIndex++, rowIndex--)
		{
			SpriteSpan& span = spans[spanIndex];
			if (rowStarts[rowIndex] > span.start)
			{
				if (span.length + span.start >= rowStarts[rowIndex])
				{
					const int cut = rowStarts[rowIndex] - span.start;
					span.start = rowStarts[rowIndex];
					span.skip += cut;
					span.length -= cut;
				}
				else
					span.length = 0;
			}
		}
	}
	else
	{
		if (line < width)
		{
			if (lines > lastLine)
				lines = lastLine;
		}
		else if (line + lines - width > lastLine)
		{
			lines = lastLine - line + width;
			if (lines <= 0)
				return;
		}
		for (int i = 0; i < lines; i++, spanStart += spanShift)
		{
			SpriteSpan& span = spans[i];
			const int start = spanStart >> 16;
			if (start < 0)
			{
				span.start = 0;
				span.length = spanLength + start;
				span.skip = -start;
				if (-start < minSkip)
					minSkip = -start;
			}
			else
			{
				span.start = start;
				span.length = spanLength;
				span.skip = 0;
				minSkip = 0;
			}
			if (span.length + span.start > height)
				span.length = height - span.start;
		}
		const int over = line + str_F2C20ar.dword0x1d - width + 1;
		for (int i = 0; i < over; i++)
		{
			SpriteSpan& span = spans[over - 1 - i];
			const int excess = span.length + span.start - rowLimits[-i];
			if (excess > 0)
			{
				span.length -= excess;
				if (span.length < 0)
					span.length = 0;
			}
		}
		if (line - lines < 0)
		{
			int remaining = lines - line;
			const int cut = line - lines - str_F2C20ar.dword0x21;
			if (cut < 0)
			{
				lines = line - str_F2C20ar.dword0x21;
				if (lines <= 0)
					return;
				remaining += cut;
			}
			for (int spanIndex = line, rowIndex = 0, n = remaining - 1; n > 0; n--, rowIndex++)
			{
				spanIndex++;
				if (spanIndex < 0)
					continue;
				SpriteSpan& span = spans[spanIndex];
				const int shift = rowStarts[rowIndex] - span.start;
				if (shift > 0)
				{
					span.start = rowStarts[rowIndex];
					span.length -= shift;
					if (span.length < 0)
						span.length = 0;
					span.skip += shift;
				}
			}
		}
	}

	// the texture columns from the first one any span shows
	const int columnStep = (str_F2C20ar.dword0x05 << 16) / spanLength;
	str_F2C20ar.dword0x0b = 0;
	if (str_F2C20ar.dword0x05 < 0)
		str_F2C20ar.dword0x0b -= columnStep * (spanLength - 1);
	int columnCount = spanLength - minSkip;
	if (columnCount <= 0)
		return;
	if (m_largeScreen)
	{
		// a big screen: the columns the spans show, up to the table (past dword0x1c the original reads stale columns)
		int shown = 0;
		for (int i = 0; i < lines; i++)
			if (spans[i].length > 0)
				shown = (std::max)(shown, spans[i].skip - minSkip + spans[i].length);
		columnCount = (std::min)({ columnCount, shown, m_columnsCapacity });
	}
	else if (columnCount > str_F2C20ar.dword0x1c)
		columnCount = str_F2C20ar.dword0x1c;
	str_F2C20ar.dword0x0b += columnStep * minSkip;
	SpriteColumn* columns = reinterpret_cast<SpriteColumn*>(&m_scratch[m_columnsOffset]);
	for (int column = 0; column <= columnCount; column++)
	{
		columns[column].offset = str_F2C20ar.dword0x0b >> 16;
		columns[column].step = columns[column].offset - columns[column - 1].offset;
		str_F2C20ar.dword0x0b += columnStep;
	}
	for (int i = 0; i < lines; i++)
	{
		const SpriteSpan& span = spans[i];
		const int length = m_largeScreen ? (std::min)(span.length, columnCount - (span.skip - minSkip)) : span.length;
		if (length > 0)
		{
			const SpriteColumn* first = &columns[span.skip - minSkip];
			const int8_t* texels = &str_F2C20ar.dword0x02_data[str_F2C20ar.dword0x08_width * (str_F2C20ar.dword0x0a_actIdx >> 16) + first->offset];
			DrawSpriteSpan(texels, first, lineStart + m_viewLines_F0E20[span.start].dword_1, &m_viewLines_F0E20[span.start], length, 8);
		}
		str_F2C20ar.dword0x0a_actIdx += rowStep;
		lineStart += str_F2C20ar.dword0x23_stride;
	}
}

// The sprite upright and scaled into the rectangle at the screen position, clipped by the view port.
void GameRenderNG::DrawUprightSprite(uint32 pass)
{
	const int shift = (str_F2C20ar.dword0x0c_realHeight + str_F2C20ar.dword0x09_realWidth) >> 2;
	if (pass == (uint32_t)SpritePass::Main)
	{
		str_F2C20ar.dword0x04_screenY += -(str_F2C20ar.sin_0x0d * shift >> 16) - shift;
		str_F2C20ar.dword0x03_screenX += -(str_F2C20ar.cos_0x11 * shift >> 16) - shift;
	}
	else if (pass == (uint32_t)SpritePass::Reflection)
	{
		str_F2C20ar.dword0x04_screenY += (str_F2C20ar.sin_0x0d * shift >> 16) - shift;
		str_F2C20ar.dword0x03_screenX += (str_F2C20ar.cos_0x11 * shift >> 16) - shift;
	}
	const int width = viewPort.Width_DE564;
	const int height = viewPort.Height_DE568;
	if (str_F2C20ar.dword0x04_screenY >= width)
		return;
	const int columnStep = (str_F2C20ar.dword0x05 << 16) / str_F2C20ar.dword0x09_realWidth;
	if (str_F2C20ar.dword0x04_screenY >= 0)
	{
		str_F2C20ar.dword0x0b = 0;
		if (str_F2C20ar.dword0x09_realWidth + str_F2C20ar.dword0x04_screenY - width > 0)
			str_F2C20ar.dword0x09_realWidth = width - str_F2C20ar.dword0x04_screenY;
	}
	else
	{
		str_F2C20ar.dword0x09_realWidth += str_F2C20ar.dword0x04_screenY;
		if (str_F2C20ar.dword0x09_realWidth <= 0)
			return;
		str_F2C20ar.dword0x0b = columnStep * -str_F2C20ar.dword0x04_screenY;
		str_F2C20ar.dword0x04_screenY = 0;
		if (width <= str_F2C20ar.dword0x09_realWidth)
			str_F2C20ar.dword0x09_realWidth = width;
	}
	if (str_F2C20ar.dword0x03_screenX >= height)
		return;
	const int rowStep = (str_F2C20ar.dword0x06_height << 16) / str_F2C20ar.dword0x0c_realHeight;
	if (str_F2C20ar.dword0x03_screenX >= 0)
	{
		str_F2C20ar.dword0x0a_actIdx = 0;
		if (str_F2C20ar.dword0x0c_realHeight + str_F2C20ar.dword0x03_screenX - height > 0)
			str_F2C20ar.dword0x0c_realHeight = height - str_F2C20ar.dword0x03_screenX;
	}
	else
	{
		str_F2C20ar.dword0x0c_realHeight += str_F2C20ar.dword0x03_screenX;
		if (str_F2C20ar.dword0x0c_realHeight <= 0)
			return;
		str_F2C20ar.dword0x0a_actIdx = rowStep * -str_F2C20ar.dword0x03_screenX;
		str_F2C20ar.dword0x03_screenX = 0;
		if (height <= str_F2C20ar.dword0x0c_realHeight)
			str_F2C20ar.dword0x0c_realHeight = height;
	}
	SpriteColumn* columns = reinterpret_cast<SpriteColumn*>(&m_scratch[m_columnsOffset]);
	for (int column = 0; column < str_F2C20ar.dword0x09_realWidth; column++)
	{
		columns[column].offset = str_F2C20ar.dword0x0b >> 16;
		columns[column].step = column ? columns[column].offset - columns[column - 1].offset : 22;
		str_F2C20ar.dword0x0b += columnStep;
	}
	if (pass == (uint32_t)SpritePass::Main && x_D41A0_BYTEARRAY_4_struct.showHelp_10)
		sub_88740(str_F2C20ar.dword0x14x, str_F2C20ar.dword0x04_screenY + (str_F2C20ar.dword0x09_realWidth >> 1), str_F2C20ar.dword0x03_screenX + (str_F2C20ar.dword0x0c_realHeight >> 1));
	uint8_t* line = &ViewPortRenderBufferStart_DE558[str_F2C20ar.dword0x03_screenX * iScreenWidth_DE560 + str_F2C20ar.dword0x04_screenY];
	for (; str_F2C20ar.dword0x0c_realHeight; str_F2C20ar.dword0x0c_realHeight--)
	{
		const int8_t* texels = &str_F2C20ar.dword0x02_data[str_F2C20ar.dword0x08_width * (str_F2C20ar.dword0x0a_actIdx >> 16) + columns[0].offset];
		DrawSpriteSpan(texels, columns, line, nullptr, str_F2C20ar.dword0x09_realWidth, 7);
		str_F2C20ar.dword0x0a_actIdx += rowStep;
		line += iScreenWidth_DE560;
	}
}

// The pixels of a span: the texel steps go by columns[1..], the screen by steps[1..] (nullptr: the next pixel).
template<typename Pixel>
static void DrawSpritePixels(const int8_t* texels, const GameRenderNG::SpriteColumn* columns, uint8_t* screen, const type_unk_F0E20x* steps, int length, Pixel pixel)
{
	for (int i = 1; i <= length; i++)
	{
		const uint8_t texel = *texels;
		texels += columns[i].step;
		if (texel)
			pixel(*screen, texel);
		screen += steps ? steps[i].dword_0 : 1;
	}
}

// The sprite modes (dword0x01_visibilityIdx) up to lastMode: 0 the texel, 1 lit (dword0x00), 2/3 blended with the
// screen, 4/5 blended with the player color (dword0x07), 6/7 blended and lit, 8 the screen darkened (shadow).
void GameRenderNG::DrawSpriteSpan(const int8_t* texels, const SpriteColumn* columns, uint8_t* screen, const type_unk_F0E20x* steps, int length, int lastMode)
{
	const int mode = str_F2C20ar.dword0x01_visibilityIdx;
	if ((unsigned)mode > (unsigned)lastMode)
		return;
	const int light = str_F2C20ar.dword0x00 & ~0xFF;
	const int color = str_F2C20ar.dword0x07;
	const uint8_t* lit = ColourLookupTable_F6EE0;
	const uint8_t* blend = &ColourLookupTable_F6EE0[COLOUR_BLEND_LOOKUP_OFFSET];
	switch (mode)
	{
	case 0:
		DrawSpritePixels(texels, columns, screen, steps, length, [](uint8_t& pixel, uint8_t texel) { pixel = texel; });
		break;
	case 1:
		DrawSpritePixels(texels, columns, screen, steps, length, [=](uint8_t& pixel, uint8_t texel) { pixel = lit[light | texel]; });
		break;
	case 2:
		DrawSpritePixels(texels, columns, screen, steps, length, [=](uint8_t& pixel, uint8_t texel) { pixel = blend[texel << 8 | pixel]; });
		break;
	case 3:
		DrawSpritePixels(texels, columns, screen, steps, length, [=](uint8_t& pixel, uint8_t texel) { pixel = blend[pixel << 8 | texel]; });
		break;
	case 4:
		DrawSpritePixels(texels, columns, screen, steps, length, [=](uint8_t& pixel, uint8_t texel) { pixel = blend[(color & 0xFFFF0000) | (color & 0xFF) << 8 | texel]; });
		break;
	case 5:
		DrawSpritePixels(texels, columns, screen, steps, length, [=](uint8_t& pixel, uint8_t texel) { pixel = blend[(color & 0xFFFF00FF) | texel << 8]; });
		break;
	case 6:
		DrawSpritePixels(texels, columns, screen, steps, length, [=](uint8_t& pixel, uint8_t texel) { pixel = lit[light | blend[texel << 8 | pixel]]; });
		break;
	case 7:
		DrawSpritePixels(texels, columns, screen, steps, length, [=](uint8_t& pixel, uint8_t texel) { pixel = lit[light | blend[pixel << 8 | texel]]; });
		break;
	case 8:
		DrawSpritePixels(texels, columns, screen, steps, length, [=](uint8_t& pixel, uint8_t) { pixel = lit[light | pixel]; });
		break;
	}
}

// Wrapping 32-bit arithmetic of the original: start + step * count, a coordinate in 16.16.
static int StepValue(int start, int step, int count)
{
	return static_cast<int>(static_cast<uint32_t>(start) + static_cast<uint32_t>(step) * static_cast<uint32_t>(count));
}

static int Fixed16(int value)
{
	return static_cast<int>(static_cast<uint32_t>(value) << 16);
}

// A value along the rows of a triangle: start at the row startRow, + slope per row (32-bit wrapping as the original).
struct RowValue
{
	int start;
	int slope;
	int startRow;
	uint32_t At(int row) const
	{
		return static_cast<uint32_t>(StepValue(start, slope, row - startRow));
	}
};

// The edges and the values along the left edge of one part of a triangle (above or below the middle corner).
struct TrianglePart
{
	RowValue left;
	RowValue right;
	RowValue u;
	RowValue v;
	RowValue light;
};

// The step of a value across the triangle: from the corner `from` to the value `to` + `edgeOffset` (the long edge at the
// middle row), wrapped to 32 bits as the original, divided by the width at the middle row + 1.
static int CrossStep(int to, int64_t edgeOffset, int from, int divisor)
{
	return static_cast<int>(static_cast<uint32_t>(to) + static_cast<uint32_t>(edgeOffset) - static_cast<uint32_t>(from)) / divisor;
}

// The triangle in screen space (x, y, texture u, v and light in 16.16) with the mode m_triangleMode_E126D: 5 textured with Gouraud
// light, 7 textured with the flat light m_flatLight_E126C, 26 water (texels under 12 blended with the screen). Only the
// clockwise triangles are drawn, the rows outside the view port are skipped. The texture coordinates and the light go
// along the left edge, across the rows by the steps taken from the widest (middle) row. With render threads queued.
void GameRenderNG::DrawTriangleInProjectionSpace_B6253(const TerrainVertex& vertex1, const TerrainVertex& vertex2, const TerrainVertex& vertex3)//sub_B6253 - 297253
{
	const int mode = m_triangleMode_E126D;
	if (mode != 5 && mode != 7 && mode != 26)
		return;
	if (m_largeScreen || m_viewDistanceScale > 1)
	{
		const int minX = (std::min)({ vertex1.x, vertex2.x, vertex3.x });
		const int maxX = (std::max)({ vertex1.x, vertex2.x, vertex3.x });
		const int minY = (std::min)({ vertex1.y, vertex2.y, vertex3.y });
		const int maxY = (std::max)({ vertex1.y, vertex2.y, vertex3.y });
		if (minX < -32767 || maxX > 32767 || minY < -32767 || maxY > 32767 || maxX - minX > 32767 || maxY - minY > 32767)
		{
			DrawClippedTriangle(vertex1, vertex2, vertex3);
			return;
		}
	}
	if (!m_queueing)
	{
		DrawTriangle(vertex1, vertex2, vertex3, mode, m_triangleTexture_DE55C, m_flatLight_E126C, 0, viewPort.Height_DE568);
		return;
	}
	QueuedTriangle& queued = m_queue.emplace_back();
	queued.corners[0] = vertex1;
	queued.corners[1] = vertex2;
	queued.corners[2] = vertex3;
	queued.texture = m_triangleTexture_DE55C;
	queued.mode = mode;
	queued.flatLight = m_flatLight_E126C;
	queued.minY = (std::min)({ vertex1.y, vertex2.y, vertex3.y });
	queued.maxY = (std::max)({ vertex1.y, vertex2.y, vertex3.y });
	const int64_t width = std::clamp((std::max)({ vertex1.x, vertex2.x, vertex3.x }), 0, static_cast<int>(viewPort.Width_DE564))
		- std::clamp((std::min)({ vertex1.x, vertex2.x, vertex3.x }), 0, static_cast<int>(viewPort.Width_DE564));
	const int64_t height = std::clamp(queued.maxY, 0, static_cast<int>(viewPort.Height_DE568)) - std::clamp(queued.minY, 0, static_cast<int>(viewPort.Height_DE568));
	m_queuedArea += width * height;
}

// A triangle of a big screen or a longer view whose corners or spans do not fit the 16.16 rows (the tiles beside the camera,
// depth clamped to 128, project far out): clipped to the view port with a margin of 8192 pixels, the texture and the light
// linear along the edges, and drawn as a fan of triangles that fit. The new edges lie outside the view port.
void GameRenderNG::DrawClippedTriangle(const TerrainVertex& vertex1, const TerrainVertex& vertex2, const TerrainVertex& vertex3)
{
	struct ClipVertex
	{
		double x, y, u, v, light;
	};
	const auto fromVertex = [](const TerrainVertex& vertex) { return ClipVertex{ double(vertex.x), double(vertex.y), double(vertex.u), double(vertex.v), double(vertex.light) }; };
	const double margin = 8192;
	const double limits[4] = { -margin, viewPort.Width_DE564 + margin, -margin, viewPort.Height_DE568 + margin };
	std::array<ClipVertex, 8> polygon = { fromVertex(vertex1), fromVertex(vertex2), fromVertex(vertex3) };
	int count = 3;
	for (int side = 0; side < 4 && count >= 3; side++)
	{
		const auto coordinate = [side](const ClipVertex& point) { return side < 2 ? point.x : point.y; };
		const auto inside = [&](const ClipVertex& point) { return side % 2 == 0 ? coordinate(point) >= limits[side] : coordinate(point) <= limits[side]; };
		std::array<ClipVertex, 8> clipped{};
		int clippedCount = 0;
		for (int i = 0; i < count; i++)
		{
			const ClipVertex& from = polygon[i];
			const ClipVertex& to = polygon[(i + 1) % count];
			if (inside(from))
				clipped[clippedCount++] = from;
			if (inside(from) != inside(to))
			{
				const double t = (limits[side] - coordinate(from)) / (coordinate(to) - coordinate(from));
				clipped[clippedCount++] = { from.x + (to.x - from.x) * t, from.y + (to.y - from.y) * t, from.u + (to.u - from.u) * t,
					from.v + (to.v - from.v) * t, from.light + (to.light - from.light) * t };
			}
		}
		polygon = clipped;
		count = clippedCount;
	}
	const auto toVertex = [](const ClipVertex& point)
	{
		return TerrainVertex{ static_cast<int>(std::lround(point.x)), static_cast<int>(std::lround(point.y)), static_cast<int>(std::llround(point.u)),
			static_cast<int>(std::llround(point.v)), static_cast<int>(std::llround(point.light)), 0 };
	};
	for (int i = 1; i + 1 < count; i++)
		DrawTriangleInProjectionSpace_B6253(toVertex(polygon[0]), toVertex(polygon[i]), toVertex(polygon[i + 1]));
}

// The rows bandTop..bandBottom - 1 of the triangle (the whole view port without threads).
void GameRenderNG::DrawTriangle(const TerrainVertex& vertex1, const TerrainVertex& vertex2, const TerrainVertex& vertex3, int mode, const uint8_t* texture,
	int flatLight, int bandTop, int bandBottom)
{
	const bool light = mode != 7;
	const TerrainVertex* corners[3] = { &vertex1, &vertex2, &vertex3 };
	int minY = vertex1.y < vertex2.y ? vertex1.y : vertex2.y;
	if (vertex3.y < minY)
		minY = vertex3.y;
	const int maxY = vertex1.y > vertex2.y ? (vertex1.y > vertex3.y ? vertex1.y : vertex3.y) : (vertex2.y > vertex3.y ? vertex2.y : vertex3.y);
	const int minX = vertex1.x < vertex2.x ? (vertex1.x < vertex3.x ? vertex1.x : vertex3.x) : (vertex2.x < vertex3.x ? vertex2.x : vertex3.x);
	const int maxX = vertex1.x > vertex2.x ? (vertex1.x > vertex3.x ? vertex1.x : vertex3.x) : (vertex2.x > vertex3.x ? vertex2.x : vertex3.x);
	// a big screen or a longer view: the tiles beside the camera (depth clamped to 128) project far out; the corners and
	// the spans must stay in 16 bits, else the 16.16 slopes overflow into streaks across the screen
	if (m_largeScreen || m_viewDistanceScale > 1)
		if (minX < -32767 || maxX > 32767 || minY < -32767 || maxY > 32767 || maxX - minX > 32767 || maxY - minY > 32767)
			return;
	// outside the view port, before the divisions: the rows are clipped to it and an edge stays between its corners
	// (in 16 bits, as the rows take x)
	if (maxY <= 0 || minY >= viewPort.Height_DE568)
		return;
	if ((maxX < 0 && minX >= -32768) || (minX >= viewPort.Width_DE564 && maxX <= 32767))
		return;
	int topIndex = 0;
	int atTop = 0;
	for (int i = 0; i < 3; i++)
		if (corners[i]->y == minY)
		{
			atTop++;
			topIndex = i;
		}
	if (atTop == 3)
		return;
	if (atTop == 2)//the first of the two top corners in the winding
		topIndex = corners[(topIndex + 1) % 3]->y == minY ? topIndex : (topIndex + 2) % 3;
	const TerrainVertex& a = *corners[topIndex];
	const TerrainVertex& b = *corners[(topIndex + 1) % 3];
	const TerrainVertex& c = *corners[(topIndex + 2) % 3];

	enum class Shape { FlatTop, FlatBottom, MiddleNext, MiddleLast } shape;
	if (atTop == 2)
		shape = Shape::FlatTop;
	else if (b.y == c.y)
		shape = Shape::FlatBottom;
	else
		shape = b.y < c.y ? Shape::MiddleNext : Shape::MiddleLast;

	int du = 0;
	int dv = 0;
	int dl = 0;
	int bottomY = 0;
	int splitY = 0;
	int leftSlope = 0;
	int leftSlope2 = 0;
	int rightSlope = 0;
	int rightSlope2 = 0;
	int leftU = 0, leftV = 0, leftL = 0;
	int leftU2 = 0, leftV2 = 0, leftL2 = 0;
	switch (shape)
	{
	case Shape::FlatTop://LABEL_277: a and b at the top, the left edge a-c, the right edge b-c
	{
		if (b.x <= a.x)
			return;
		const int total = c.y - a.y;
		bottomY = c.y;
		leftSlope = Fixed16(c.x - a.x) / total;
		rightSlope = Fixed16(c.x - b.x) / total;
		const int width = b.x - a.x;
		du = (b.u - a.u) / width;
		dv = (b.v - a.v) / width;
		if (light)
			dl = (b.light - a.light) / width;
		leftU = (c.u - a.u) / total;
		leftV = (c.v - a.v) / total;
		leftL = (c.light - a.light) / total;
		break;
	}
	case Shape::FlatBottom://LABEL_234: b and c at the bottom, the left edge a-c, the right edge a-b
	{
		if (b.x <= c.x)
			return;
		const int total = c.y - a.y;
		bottomY = c.y;
		leftSlope = Fixed16(c.x - a.x) / total;
		rightSlope = Fixed16(b.x - a.x) / total;
		const int width = b.x - c.x;
		du = (b.u - c.u) / width;
		dv = (b.v - c.v) / width;
		if (light)
			dl = (b.light - c.light) / width;
		leftU = (c.u - a.u) / total;
		leftV = (c.v - a.v) / total;
		leftL = (c.light - a.light) / total;
		break;
	}
	case Shape::MiddleNext://LABEL_24: b in the middle on the right, the long edge a-c on the left
	{
		const int total = c.y - a.y;
		const int firstPart = b.y - a.y;
		bottomY = c.y;
		splitY = b.y;
		leftSlope = Fixed16(c.x - a.x) / total;
		rightSlope = Fixed16(b.x - a.x) / firstPart;
		if (rightSlope <= leftSlope)
			return;
		rightSlope2 = Fixed16(c.x - b.x) / (c.y - b.y);
		const int edgeOffset = static_cast<int>(firstPart * static_cast<int64_t>(a.x - c.x) / total);
		const int64_t width = static_cast<int64_t>(edgeOffset) + (b.x - a.x);
		if (width < 0)
			return;
		if (static_cast<int>(width))
		{
			const int divisor = static_cast<int>(width) + 1;
			du = CrossStep(b.u, firstPart * static_cast<int64_t>(a.u - c.u) / total, a.u, divisor);
			dv = CrossStep(b.v, firstPart * static_cast<int64_t>(a.v - c.v) / total, a.v, divisor);
			if (light)
				dl = CrossStep(b.light, firstPart * static_cast<int64_t>(a.light - c.light) / total, a.light, divisor);
		}
		leftU = (c.u - a.u) / total;
		leftV = (c.v - a.v) / total;
		leftL = (c.light - a.light) / total;
		break;
	}
	case Shape::MiddleLast://LABEL_129: c in the middle on the left, the long edge a-b on the right
	{
		const int total = b.y - a.y;
		const int firstPart = c.y - a.y;
		const int secondPart = b.y - c.y;
		bottomY = b.y;
		splitY = c.y;
		leftSlope = Fixed16(c.x - a.x) / firstPart;
		rightSlope = Fixed16(b.x - a.x) / total;
		if (rightSlope <= leftSlope)
			return;
		leftSlope2 = Fixed16(b.x - c.x) / secondPart;
		const int edgeOffset = static_cast<int>(firstPart * static_cast<int64_t>(b.x - a.x) / total);
		const int64_t width = static_cast<int64_t>(edgeOffset) + (a.x - c.x);
		if (width < 0)
			return;
		if (static_cast<int>(width))
		{
			const int divisor = static_cast<int>(width) + 1;
			du = CrossStep(a.u, firstPart * static_cast<int64_t>(b.u - a.u) / total, c.u, divisor);
			dv = CrossStep(a.v, firstPart * static_cast<int64_t>(b.v - a.v) / total, c.v, divisor);
			if (light)
				dl = CrossStep(a.light, firstPart * static_cast<int64_t>(b.light - a.light) / total, c.light, divisor);
		}
		else if (light)
			dl = edgeOffset;
		leftU = (c.u - a.u) / firstPart;
		leftV = (c.v - a.v) / firstPart;
		leftL = (c.light - a.light) / firstPart;
		leftU2 = (b.u - c.u) / secondPart;
		leftV2 = (b.v - c.v) / secondPart;
		leftL2 = (b.light - c.light) / secondPart;
		break;
	}
	}

	const int firstRow = a.y > bandTop ? a.y : bandTop;
	const int lastRow = bottomY < bandBottom ? bottomY : bandBottom;
	if (firstRow >= lastRow)
		return;
	if (shape == Shape::FlatTop || shape == Shape::FlatBottom)
		splitY = bottomY;
	// above the middle corner (all the triangle when flat) and below it
	TrianglePart top;
	top.left = { Fixed16(a.x), leftSlope, a.y };
	top.right = { Fixed16(shape == Shape::FlatTop ? b.x : a.x), rightSlope, a.y };
	top.u = { a.u, leftU, a.y };
	top.v = { a.v, leftV, a.y };
	top.light = { a.light, leftL, a.y };
	TrianglePart bottom = top;
	if (shape == Shape::MiddleNext)
		bottom.right = { Fixed16(b.x), rightSlope2, splitY };
	else if (shape == Shape::MiddleLast)
	{
		const int firstPart = splitY - a.y;
		bottom.left = { Fixed16(c.x), leftSlope2, splitY };
		bottom.u = { StepValue(a.u, leftU, firstPart), leftU2, splitY };
		bottom.v = { StepValue(a.v, leftV, firstPart), leftV2, splitY };
		bottom.light = { StepValue(a.light, leftL, firstPart), leftL2, splitY };
	}
	const auto drawRows = [&](auto modeTag)
	{
		const auto drawPart = [&](const TrianglePart& part, int fromRow, int toRow)
		{
			if (fromRow >= toRow)
				return;
			uint32_t left = part.left.At(fromRow);
			uint32_t right = part.right.At(fromRow);
			uint32_t u = part.u.At(fromRow);
			uint32_t v = part.v.At(fromRow);
			uint32_t light = part.light.At(fromRow);
			uint8_t* line = ViewPortRenderBufferAltStart_DE554 + iScreenWidth_DE560 * (fromRow + 1);
			for (int row = fromRow; row < toRow; row++, line += iScreenWidth_DE560)
			{
				DrawTriangleRow<decltype(modeTag)::value>(line, static_cast<int>(left), static_cast<int>(right), static_cast<int>(u), static_cast<int>(v),
					static_cast<int>(light), du, dv, dl, texture, flatLight);
				left += part.left.slope;
				right += part.right.slope;
				u += part.u.slope;
				v += part.v.slope;
				light += part.light.slope;
			}
		};
		drawPart(top, firstRow, splitY < lastRow ? splitY : lastRow);
		drawPart(bottom, splitY > firstRow ? splitY : firstRow, lastRow);
	};
	if (mode == 5)
		drawRows(std::integral_constant<int, 5>());
	else if (mode == 7)
		drawRows(std::integral_constant<int, 7>());
	else
		drawRows(std::integral_constant<int, 26>());
}

// The queue (sky and triangles) by bands of rows: the render threads and the game thread take the bands one after another,
// a band gets the queue in its order. A small queue is drawn by the game thread alone.
void GameRenderNG::FlushQueue()
{
	if (m_queue.empty() && !m_skyQueued)
		return;
	const int height = viewPort.Height_DE568;
	if (!m_skyQueued && m_queuedArea < 32768)
	{
		for (const QueuedTriangle& queued : m_queue)
			DrawTriangle(queued.corners[0], queued.corners[1], queued.corners[2], queued.mode, queued.texture, queued.flatLight, 0, height);
	}
	else
	{
		const int bands = 4 * (static_cast<int>(m_renderThreads.size()) + 1);
		m_nextBand = 0;
		const auto drawBands = [this, bands, height]
		{
			for (int band = m_nextBand++; band < bands; band = m_nextBand++)
			{
				const int bandTop = height * band / bands;
				const int bandBottom = height * (band + 1) / bands;
				if (m_skyQueued)
					DrawSkyRows(bandTop, bandBottom);
				for (const QueuedTriangle& queued : m_queue)
					if (queued.maxY > bandTop && queued.minY < bandBottom)
						DrawTriangle(queued.corners[0], queued.corners[1], queued.corners[2], queued.mode, queued.texture, queued.flatLight, bandTop, bandBottom);
			}
		};
		for (const auto& thread : m_renderThreads)
			thread->Run(drawBands);
		drawBands();
		WaitForRenderThreads();
	}
	m_queue.clear();
	m_queuedArea = 0;
	m_skyQueued = false;
}

// 0 threads: everything in the game thread at once; the 'T' key goes through 1-7.
void GameRenderNG::SetRenderThreads(uint8_t renderThreads)
{
	WaitForRenderThreads();
	m_renderThreads.clear();
	for (int thread = 0; thread < renderThreads; thread++)
		m_renderThreads.push_back(m_assignToSpecificCores ? std::make_unique<RenderThread>(thread + 1) : std::make_unique<RenderThread>());
}

uint8_t GameRenderNG::GetRenderThreads() const
{
	return static_cast<uint8_t>(m_renderThreads.size());
}

void GameRenderNG::WaitForRenderThreads()
{
	for (const auto& thread : m_renderThreads)
		while (thread->GetIsTaskRunning())
			std::this_thread::yield();
}

// One row of the triangle from the pixel left >> 16 to right >> 16 (16-bit screen coordinates as the original), clipped by
// the view port; at the left edge the texture and the light move by the skipped pixels. Mode as m_triangleMode_E126D.
template <int Mode>
void GameRenderNG::DrawTriangleRow(uint8_t* line, int left, int right, int u, int v, int light, int du, int dv, int dl, const uint8_t* texture, int flatLight)
{
	const int width = viewPort.Width_DE564;
	const int16_t startX = static_cast<int16_t>(left >> 16);
	const int endX = (right >> 16) & 0xFFFF;
	int count;
	uint8_t* pixel;
	if (startX >= 0)
	{
		count = static_cast<int16_t>((endX > width ? width : endX) - startX);
		if (count <= 0)
			return;
		pixel = line + startX;
	}
	else
	{
		if (static_cast<int16_t>(endX) <= 0)
			return;
		const int skipped = static_cast<uint16_t>(-startX);
		u = StepValue(u, du, skipped);
		v = StepValue(v, dv, skipped);
		light = StepValue(light, dl, skipped);
		count = static_cast<int16_t>(endX) > width ? width : static_cast<int16_t>(endX);
		pixel = line;
	}
	const uint8_t* const lit = ColourLookupTable_F6EE0;
	uint32_t textureU = u;
	uint32_t textureV = v;
	uint32_t shade = light;
	uint8_t* const end = pixel + count;
	if constexpr (Mode == 5)
		for (; pixel < end; pixel++)
		{
			*pixel = lit[((shade >> 8) & 0xFF00) | texture[((textureV >> 8) & 0xFF00) | ((textureU >> 16) & 0xFF)]];
			textureU += du;
			textureV += dv;
			shade += dl;
		}
	else if constexpr (Mode == 7)
	{
		const uint8_t* const flatLit = lit + (static_cast<uint8_t>(flatLight) << 8);
		for (; pixel < end; pixel++)
		{
			*pixel = flatLit[texture[((textureV >> 8) & 0xFF00) | ((textureU >> 16) & 0xFF)]];
			textureU += du;
			textureV += dv;
		}
	}
	else
	{
		const uint8_t* const blend = lit + COLOUR_BLEND_LOOKUP_OFFSET;
		for (; pixel < end; pixel++)
		{
			const uint8_t texel = ClampReflectionTexel(((textureV >> 8) & 0xFF00) | ((textureU >> 16) & 0xFF), texture);
			const uint8_t shaded = lit[((shade >> 8) & 0xFF00) | texel];
			*pixel = texel >= 12 ? shaded : blend[*pixel << 8 | shaded];
			textureU += du;
			textureV += dv;
			shade += dl;
		}
	}
}
