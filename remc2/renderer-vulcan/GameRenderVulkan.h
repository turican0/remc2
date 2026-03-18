#pragma once
// =============================================================================
// GameRenderVulkan.h
//
// Drop-in replacement for GameRenderOriginal.
// All public method signatures are identical.
// Internally, instead of writing directly to ViewPortRenderBufferStart_DE558,
// we batch geometry into the VulkanRenderer which rasterises on GPU and
// reads back into ViewPortRenderBufferStart_DE558 at endFrame.
//
// Usage:
//   Replace   GameRenderOriginal  renderer;
//   with      GameRenderVulkan    renderer;
//   Anywhere that queries the type directly needs updating; everything else
//   is source-compatible because both classes expose the same public API.
// =============================================================================

#ifndef GAME_RENDER_VULKAN_H
#define GAME_RENDER_VULKAN_H

#include "VulkanRenderer.h"

// Engine includes (adjust paths to match your build)
// #include "GameRenderOriginal.h"  // for type definitions reused here
// Alternatively, if the types/globals are in separate headers, include those.

class GameRenderVulkan
{
public:
    GameRenderVulkan();
    ~GameRenderVulkan();

    // -----------------------------------------------------------------------
    // Initialisation — call once after the engine has set up its window and
    // Vulkan context (or let the renderer create its own headless context).
    // -----------------------------------------------------------------------
    bool InitVulkan(uint32_t screenWidth,
                    uint32_t screenHeight,
                    VkInstance       existingInstance   = VK_NULL_HANDLE,
                    VkPhysicalDevice existingPhysDevice = VK_NULL_HANDLE,
                    VkDevice         existingDevice     = VK_NULL_HANDLE);

    // -----------------------------------------------------------------------
    // Original engine API — identical signatures to GameRenderOriginal
    // -----------------------------------------------------------------------
    void DrawWorld_411A0(int posX, int posY,
                         int16_t yaw, int16_t posZ,
                         int16_t pitch, int16_t roll, int16_t fov);

    void DrawSky_40950(int16_t roll);

    void DrawTerrainAndParticles_3C080(int16_t posX, int16_t posY,
                                       int16_t yaw, int32_t posZ,
                                       int pitch, int16_t roll, int fov);

    void DrawSprites_3E360(int a2x);
    uint16_t sub_3FD60(int a2x);

    void SetBillboards_3B560(int16_t roll);

    void DrawTriangleInProjectionSpace_B6253(uint32_t* vertex1,
                                             uint32_t* vertex2,
                                             uint32_t* vertex3);

    void DrawSprite_41BD3(uint32_t a1);

    void DrawSorcererNameAndHealthBar_2CB30(void* a1x,
                                            int16_t a2, int a3, int16_t a4);

    void sub_88740(void* a1x, int16_t posX, int16_t posY);

    // -----------------------------------------------------------------------
    // Palette sync — call whenever the engine's colour palette changes.
    // paletteRGBA8: pointer to 256 × uint32_t ARGB values.
    // -----------------------------------------------------------------------
    void SyncPalette(const uint32_t* paletteRGBA8);

    // -----------------------------------------------------------------------
    // Texture registration — call when the engine decompresses a new sprite
    // or terrain texture.  Returns an atlas slot index used internally.
    // -----------------------------------------------------------------------
    uint32_t RegisterTexture(const uint8_t* pixels, uint32_t w, uint32_t h);

    // -----------------------------------------------------------------------
    // Access to the underlying Vulkan renderer (for integration code)
    // -----------------------------------------------------------------------
    VulkanRenderer& vulkan() { return m_vk; }

private:
    // -----------------------------------------------------------------------
    // Frame boundary helpers — DrawWorld_411A0 calls beginFrame / endFrame
    // -----------------------------------------------------------------------
    void beginGpuFrame();
    void endGpuFrameAndReadback();

    // -----------------------------------------------------------------------
    // Triangle submission helper called from DrawTriangleInProjectionSpace_B6253
    //
    // The original rasteriser packs vertex data into global scratch arrays
    // (unk_DE56Cx, Str_E9C38_smalltit, etc.).  We intercept the final stage
    // where vertex[0/1] = screen-X, vertex[1/1] = screen-Y, vertex[2/5] = UV/shade.
    // -----------------------------------------------------------------------
    void submitEngineTriangle(const uint32_t* v0,
                              const uint32_t* v1,
                              const uint32_t* v2,
                              uint32_t blendMode,
                              uint32_t textureIndex,
                              uint32_t lightTableBase);

    // -----------------------------------------------------------------------
    // Original engine sub-functions that run unchanged on CPU
    // (projection, culling, billboard computation)
    // These are called from the Vulkan versions of Draw* methods.
    // -----------------------------------------------------------------------
    void CPU_DrawSky_40950(int16_t roll);
    void CPU_SetBillboards_3B560(int16_t roll);
    void CPU_DrawSprite_41BD3_submit(uint32_t a1);

    // -----------------------------------------------------------------------
    // State
    // -----------------------------------------------------------------------
    VulkanRenderer m_vk;

    bool     m_vulkanReady  = false;
    bool     m_frameOpen    = false;
    uint32_t m_screenW      = 0;
    uint32_t m_screenH      = 0;

    // Current blend mode / texture (updated by DrawTerrainAndParticles_3C080
    // before calling DrawTriangleInProjectionSpace_B6253)
    uint32_t m_activeBlendMode     = 5;   // x_BYTE_E126D
    uint32_t m_activeTextureIndex  = 0;   // atlas slot
    uint32_t m_activeLightTableBase= 0;

    // Fallback: original CPU renderer (used for HUD, text, UI elements that
    // we have not ported to GPU yet)
    // GameRenderOriginal m_cpuFallback;
};

#endif // GAME_RENDER_VULKAN_H
