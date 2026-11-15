#include "Mc1Render.h"

#include <cstring>
#include <vector>

#include "Mc1Blocks.h"
#include "Mc1Files.h"
#include "Mc1Game.h"
#include "Mc1Shared.h"
#include "../../remc2/engine/GameRenderOriginal.h"

// The MC1 world drawn by the renderer of remc2 (RenderGame::MC1 switches the differences of MC1 on).
// The renderer reads the tables, the textures, the sky and the settings of MC2: they are set from MC1 here.
// Not yet: the sprites (the entity index map holds MC1 entities, hidden while drawing), the motion blur by the speed.

static GameRenderOriginal* mc1Renderer = nullptr;

// the settings of MC2 the renderer reads, kept and put back after the drawing
struct SavedSettings
{
	type_D41A0_BYTESTR_0 world;
	uint8_t* skyTexture;
	uint8_t highSpeedSystem;
	bool isCave;
};

void DrawWorld_30D90_30DD0(uint8_t* screen, int posX, int posY, int16_t yaw, int16_t posZ, int16_t pitch, int16_t roll, int16_t fov)
{
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	if (!mc1Renderer)
		mc1Renderer = new GameRenderOriginal();
	renderGame = RenderGame::MC1;

	// the tables B7934 have the layout of ColourLookupTable_F6EE0 (fades, blend 4000h, colours 14000h), the tiles 9334C
	static_assert(sizeof(tables_B7934_B7924) == sizeof(ColourLookupTable_F6EE0), "the tables of MC1 and MC2");
	memcpy(ColourLookupTable_F6EE0, &tables_B7934_B7924, sizeof(ColourLookupTable_F6EE0));
	for (int i = 0; i < 256; i++)
		textureAddresses_DDF50[i] = blockPointers_9334C_9334C[i];

	static SavedSettings saved;
	saved.world = world_D41A0;
	saved.skyTexture = skyTexture_D41A8;
	saved.highSpeedSystem = gameState_D41A4.m_wHighSpeedSystem;
	saved.isCave = isCaveLevel_D41B6;

	auto& settings = world_D41A0.m_GameSettings;
	settings.m_Graphics.m_wReflections = state->reflections_0x2195_8597;
	settings.m_Graphics.m_wShadows = state->shadows_0x2196_8598;
	settings.m_Graphics.m_wSky = state->sky_0x2197_8599;
	settings.m_Graphics.m_wViewPortSize = state->setting_0x2198_8600;
	settings.m_Display.m_uiScreenSize = state->screenSize_0x219B_8603;
	settings.m_Display.xxxx_0x2191 = state->blur_0x219C_8604;
	settings.graphics_0x2192.xxxx_0x2193 = state->vfx_0x219E_8606;
	world_D41A0.terrain_2FECE.MapType = MapType_t::Day;
	// the waves: the turn of the player (+12h in both)
	world_D41A0.LevelIndex_0xc = 0;
	int32_t turn;
	memcpy(&turn, (uint8_t*)&state->players_0x340B_13323[state->currentPlayer_0x08_8] + 0x12, sizeof(turn));
	world_D41A0.players_0x2BDE[0].Turn_2BE0_11248 = turn;
	skyTexture_D41A8 = skyData_AE3D8_AE3C8;
	gameState_D41A4.m_wHighSpeedSystem = 0;
	isCaveLevel_D41B6 = 0;

	// no sprites yet: the entity index map is hidden while drawing
	static std::vector<uint8_t> entityIndex;
	entityIndex.assign((uint8_t*)mapEntityIndex_15B4E0, (uint8_t*)mapEntityIndex_15B4E0 + sizeof(mapEntityIndex_15B4E0));
	memset(mapEntityIndex_15B4E0, 0, sizeof(mapEntityIndex_15B4E0));

	(void)screen; // the work screen, as pdwScreenBuffer_351628 in remc2
	mc1Renderer->DrawWorld_411A0(posX, posY, yaw, posZ, pitch, roll, fov);

	memcpy(mapEntityIndex_15B4E0, entityIndex.data(), entityIndex.size());
	world_D41A0 = saved.world;
	skyTexture_D41A8 = saved.skyTexture;
	gameState_D41A4.m_wHighSpeedSystem = saved.highSpeedSystem;
	isCaveLevel_D41B6 = saved.isCave;
	renderGame = RenderGame::MC2;
}

void FreeMc1Renderer()
{
	delete mc1Renderer;
	mc1Renderer = nullptr;
}
