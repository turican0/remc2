#include "Mc1Render.h"

#include <cstring>
#include <vector>

#include "Mc1Blocks.h"
#include "Mc1Files.h"
#include "Mc1Game.h"
#include "Mc1Shared.h"
#include "Mc1TextureMaps.h"
#include "../../remc2/engine/GameRenderHD.h"
#include "../../remc2/engine/GameRenderHW.h"
#include "../../remc2/engine/GameRenderNG.h"
#include "../../remc2/engine/GameRenderOriginal.h"
#include "../../remc2/engine/ReadAndDecompress.h"
#include "../../remc2/engine/read_config.h"
#include "../../remc2/portability/port_filesystem.h"

// The MC1 world drawn by the renderer of remc2 (RenderGame::MC1 switches the differences of MC1 on).
// The renderer reads the tables, the textures, the sky and the settings of MC2: they are set from MC1 here, the sprites
// come from the source below. Not yet: the motion blur by the speed.

static GameRenderInterface* mc1Renderer = nullptr;

// the renderer of the config, as MC2 chooses it (EventsFunctions)
static GameRenderInterface* CreateMc1Renderer()
{
	if (m_render == "HW")
		return new GameRenderHW(*workScreen_12EFF4_12EFE4, palette_AE428_AE418, viewDistanceScale);
	if (m_render == "Original")
		return new GameRenderOriginal();
	if (m_render == "HD")
		return new GameRenderHD(*workScreen_12EFF4_12EFE4, palette_AE428_AE418, multiThreadedRender ? numberOfRenderThreads : 0,
			assignToSpecificCores, sizePercentToThreadRender, sizePercentToThreadRenderSprites, viewDistanceScale);
	if (gameResWidth <= 640 && gameResHeight <= 480)
		return new GameRenderOriginal();
	return new GameRenderNG(viewDistanceScale, fogScale, multiThreadedRender ? numberOfRenderThreads : 0, assignToSpecificCores, sizePercentToThreadRender);
}

// the sprites (2DCB0, 2FC50): the MC1 entities as the entities of MC2, only what the sprites read (the flags 21h, the
// next in the map cell, the yaw, the position, the sprite and its frame), the tmaps of MC1 and 99BA0 as D951C
static type_entity_0x6E8E renderEntities[1000];
static Type_SpriteDef_D951C renderSpriteDefs[286];

static type_entity_0x6E8E* Mc1RenderEntity(uint16_t index)
{
	return &renderEntities[index];
}

static Type_Sprite** Mc1LoadedTmap(uint16_t tmap)
{
	return loadedTmaps_12DF8C_12DF7C[tmap];
}

static bool Mc1LoadTmap(uint16_t tmap)
{
	return MainInitTmaps_59050_59560(tmap) != 0;
}

static void Mc1TmapUsed(uint16_t tmap)
{
	// the group of the tmap (+ 8 in the tab) gets the frame of the game
	uint16_t group;
	memcpy(&group, tmapsTab_12D744_12D734 + tmap * 10 + 8, sizeof(group));
	tmapLastUse_12CF00_12CEF0[group] = mc1Settings_AE408_AE3F8->frames_0x04_4;
}

static Type_SpriteDef_D951C* Mc1SpriteDef(int16_t index)
{
	return &renderSpriteDefs[index];
}

static type_animations1* Mc1TmapAnimation(uint16_t tmap)
{
	return GetAnimationByIndex_10FD0_10FD0(animations_AE3E0_AE3D0, (int16_t)tmap);
}

static const RenderSpriteSource mc1Sprites = { Mc1RenderEntity, Mc1LoadedTmap, Mc1LoadTmap, Mc1TmapUsed, Mc1SpriteDef, Mc1TmapAnimation };

static void SetRenderSprites()
{
	const Type_Mc1Entity* entities = mc1State_AE400_AE3F0->entities_0x7463_29795;
	for (int i = 0; i < 1000; i++)
	{
		const Type_Mc1Entity& entity = entities[i];
		type_entity_0x6E8E& render = renderEntities[i];
		render.flags_0xc_12_15.dword = entity.flags_0x10_16 & 0x21;
		render.oldMapEntity_0x16_22 = entity.mapNext_0x14_20;
		render.yaw_0x1C_28 = entity.word_0x1E_30;
		render.position_0x4C_76.x = entity.position_0x48_72.x;
		render.position_0x4C_76.y = entity.position_0x48_72.y;
		render.position_0x4C_76.z = entity.position_0x48_72.z;
		render.spriteIndex_0x5A_90 = entity.sprite_0x56_86;
		render.animationFrame_0x5C_92 = (int8_t)entity.frame_0x58_88;
	}
	for (int i = 0; i < 286; i++)
	{
		const Type_Mc1SpriteDef_99BA0& def = spriteDefs_99BA0_99BA0[i];
		Type_SpriteDef_D951C& render = renderSpriteDefs[i];
		render.textureIdx_0 = def.textureIndex_0x00_0;
		render.speed_6 = def.width_0x06_6;
		render.worldSize_8 = def.height_0x08_8;
		render.lightingTableIdx_10 = def.lightingTableIdx_0x0A_10;
		render.levelSpriteGroup_11 = def.levelSpriteGroup_0x0B_11;
		render.kind_12 = def.kind_0x0C_12;
	}
	renderSprites = &mc1Sprites;
}

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
	{
		mc1Renderer = CreateMc1Renderer();
		// port: the textures to the HW renderer (MC2 sends them when it loads them): the 256 blocks as the atlas, the
		// sky and the tmaps loaded so far (the next ones by LoadTmapGroup_58B30)
		UpdateTileTextures(state->tileTextureSize_0x21A0_8608, blockData_AE3F0_AE3E0, MapType_t::Day, 256);
		if (skyData_AE3D8_AE3C8)
			UpdateSkyTexture(256, skyData_AE3D8_AE3C8);
		UploadLoadedTmapTextures();
	}
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
	const uint8_t savedTileSize = TileTextureSize_D41B5;
	TileTextureSize_D41B5 = (uint8_t)state->tileTextureSize_0x21A0_8608; // the atlas of the HW renderer

	SetRenderSprites();
	// the scratch of the billboards and the sprites (E9C38 *BScreen) is AE3FC (*BScreen) in MC1
	uint8_t* savedScratch = smalltitBuffer_E9C38;
	smalltitBuffer_E9C38 = backScreen_AE3FC_AE3EC;

	(void)screen; // the work screen, as pdwScreenBuffer_351628 in remc2
	mc1Renderer->DrawWorld_411A0(posX, posY, yaw, posZ, pitch, roll, fov);

	smalltitBuffer_E9C38 = savedScratch;
	renderSprites = nullptr;
	world_D41A0 = saved.world;
	skyTexture_D41A8 = saved.skyTexture;
	gameState_D41A4.m_wHighSpeedSystem = saved.highSpeedSystem;
	isCaveLevel_D41B6 = saved.isCave;
	TileTextureSize_D41B5 = savedTileSize;
	renderGame = RenderGame::MC2;
}

void FreeMc1Renderer()
{
	delete mc1Renderer;
	mc1Renderer = nullptr;
}
