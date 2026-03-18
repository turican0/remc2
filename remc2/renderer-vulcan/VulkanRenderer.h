#pragma once
// =============================================================================
// VulkanRenderer.h
// GPU backend replacing the software rasterizer in GameRenderOriginal.
//
// Architecture:
//   - CPU side (unchanged): projection, clipping, billboard maths
//   - GPU side (new):       triangle rasterisation, sky, sprite blitting
//   - After each frame:     VkImage readback → ViewPortRenderBufferStart_DE558
//
// Palette model:
//   The original engine is 8-bit paletted.  We keep that model on the GPU by
//   storing textures as VK_FORMAT_R8_UINT images and resolving palette entries
//   in the fragment shader using a 256-entry RGBA UBO / storage buffer.
//   The offscreen render target is VK_FORMAT_R8G8B8A8_UNORM; after readback
//   we convert back to 8-bit indices via a reverse-palette lookup so the rest
//   of the engine sees exactly the same bytes it always did.
//
//   Alternatively (simpler, chosen here) we keep the render target as
//   VK_FORMAT_R8_UINT (single-channel 8-bit) and use integer colour
//   attachments.  This avoids palette round-tripping entirely.
//
// Blend mode mapping (x_BYTE_E126D values → VkPipeline):
//   0/E/F  = solid fill (single colour)
//   2/3    = textured affine (UV, no alpha)
//   5/6    = textured + light-table shade (UV + shade index)
//   7/B    = textured + flat shade
//   8      = textured + transparent key colour (index 0 transparent)
//   9/A    = textured + transparent + light table
//   C/D    = textured + dark blend (x_BYTE_F6EE0_tablesx[16384+...])
//   1/4/10/11 = gradient (height-based colour ramp)
//   12/13  = textured + screen blend
//   14/15  = textured + dual-table blend
//   16/17  = textured + transparent screen blend
//   18/19  = textured + transparent dual-table blend
//   1A     = reflection blend
//
// =============================================================================

#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H

#include <vulkan/vulkan.h>
#include <vector>
#include <array>
#include <cstdint>
#include <cstddef>

// ---------------------------------------------------------------------------
// Forward declarations from the original engine
// ---------------------------------------------------------------------------
// These types/globals must be visible when compiling VulkanRenderer.cpp.
// Include your engine headers before including this file, or provide externs.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Constants
// ---------------------------------------------------------------------------
static constexpr uint32_t VR_MAX_FRAMES_IN_FLIGHT = 2;
static constexpr uint32_t VR_MAX_VERTICES_PER_FRAME   = 1 << 20;   // 1 M vertices
static constexpr uint32_t VR_MAX_INDICES_PER_FRAME    = 1 << 21;   // 2 M indices
static constexpr uint32_t VR_MAX_DRAW_CALLS_PER_FRAME = 1 << 14;   // 16 K draw calls
static constexpr uint32_t VR_PALETTE_ENTRIES          = 256;
static constexpr uint32_t VR_LIGHT_TABLE_SIZE         = 256 * 256; // shade × colour
static constexpr uint32_t VR_MAX_TEXTURES             = 4096;
static constexpr uint32_t VR_TEXTURE_ATLAS_DIM        = 4096;      // pixels per side

// ---------------------------------------------------------------------------
// Vertex layout sent to GPU
// ---------------------------------------------------------------------------
// We use a single interleaved vertex buffer for all geometry types.
// Unused fields are zeroed; the pipeline/shader ignores them when not needed.
// ---------------------------------------------------------------------------
struct VR_Vertex
{
    float    x, y;          // screen-space position (pixels)
    float    u, v;          // texture coordinates (texels, pre-divided by atlas size in shader)
    uint32_t shade;         // packed: lo16 = shade index, hi16 = blend-mode-specific data
    uint32_t colour;        // solid colour (palette index in bits 0-7)
};
static_assert(sizeof(VR_Vertex) == 24, "VR_Vertex size changed");

// ---------------------------------------------------------------------------
// Per-draw-call push constants  (128 bytes max on most hardware, we use 32)
// ---------------------------------------------------------------------------
struct VR_PushConstants
{
    float    viewportW;      // viewport width  in pixels
    float    viewportH;      // viewport height in pixels
    uint32_t blendMode;      // x_BYTE_E126D value
    uint32_t textureIndex;   // index into atlas (or 0xFFFF for solid)
    uint32_t lightTableBase; // base row in light table (for shaded modes)
    uint32_t pad[3];
};
static_assert(sizeof(VR_PushConstants) == 32, "VR_PushConstants size changed");

// ---------------------------------------------------------------------------
// Draw call descriptor (CPU side batch record)
// ---------------------------------------------------------------------------
struct VR_DrawCall
{
    uint32_t  firstIndex;    // offset into the index buffer
    uint32_t  indexCount;    // number of indices
    uint32_t  blendMode;     // pipeline selector
    uint32_t  textureIndex;  // 0xFFFF = solid colour
    uint32_t  lightTableBase;
};

// ---------------------------------------------------------------------------
// Texture upload descriptor
// ---------------------------------------------------------------------------
struct VR_TextureDesc
{
    const uint8_t* pixels;   // 8-bit paletted pixel data
    uint32_t       width;
    uint32_t       height;
    uint32_t       atlasSlot;  // assigned by VulkanRenderer::uploadTexture()
};

// ---------------------------------------------------------------------------
// Vulkan object wrappers (RAII-lite, no exceptions)
// ---------------------------------------------------------------------------
struct VR_Buffer
{
    VkBuffer       handle = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    void*          mapped = nullptr;   // persistently mapped (HOST_VISIBLE)
    VkDeviceSize   size   = 0;
};

struct VR_Image
{
    VkImage        handle     = VK_NULL_HANDLE;
    VkDeviceMemory memory     = VK_NULL_HANDLE;
    VkImageView    view       = VK_NULL_HANDLE;
    VkFormat       format     = VK_FORMAT_UNDEFINED;
    uint32_t       width      = 0;
    uint32_t       height     = 0;
};

// ---------------------------------------------------------------------------
// Main renderer class
// ---------------------------------------------------------------------------
class VulkanRenderer
{
public:
    // -----------------------------------------------------------------------
    // Lifecycle
    // -----------------------------------------------------------------------
    VulkanRenderer() = default;
    ~VulkanRenderer();

    // Call once at startup.
    // window: platform window handle (HWND on Windows, GLFWwindow* etc.)
    // instance / physicalDevice / device: if you already have a Vulkan context
    //   pass them in; otherwise pass VK_NULL_HANDLE and the renderer creates
    //   its own headless context (suitable for offscreen rendering + readback).
    bool init(uint32_t screenWidth,
              uint32_t screenHeight,
              VkInstance        existingInstance    = VK_NULL_HANDLE,
              VkPhysicalDevice  existingPhysDevice  = VK_NULL_HANDLE,
              VkDevice          existingDevice      = VK_NULL_HANDLE);

    void shutdown();

    // -----------------------------------------------------------------------
    // Frame lifecycle — called by GameRenderVulkan replacements
    // -----------------------------------------------------------------------

    // Call at the start of DrawWorld / DrawTerrainAndParticles.
    void beginFrame(uint32_t viewportX, uint32_t viewportY,
                    uint32_t viewportW, uint32_t viewportH);

    // Submit all batched draw calls, execute, readback to cpuBuffer.
    // cpuBuffer must point to at least screenWidth*screenHeight bytes.
    void endFrameAndReadback(uint8_t* cpuBuffer);

    // -----------------------------------------------------------------------
    // Palette / light-table upload (call when engine palette changes)
    // -----------------------------------------------------------------------
    void uploadPalette(const uint32_t* rgba8Palette, uint32_t count = 256);

    // x_BYTE_F6EE0_tablesx  — 3 sub-tables of 256×256 bytes each
    // tableIndex: 0 = shading table (offset 0)
    //             1 = blend table   (offset 16384)
    //             2 = reflection    (offset 32768)
    void uploadLightTable(const uint8_t* table, uint32_t sizeBytes);

    // -----------------------------------------------------------------------
    // Texture management
    // -----------------------------------------------------------------------
    // Register an 8-bit paletted texture; returns atlas slot index.
    // Call this when the engine loads / decompresses a sprite/terrain texture.
    uint32_t uploadTexture(const uint8_t* pixels, uint32_t w, uint32_t h);
    void     invalidateTexture(uint32_t atlasSlot);

    // -----------------------------------------------------------------------
    // Draw call submission — called from GameRenderVulkan methods
    // -----------------------------------------------------------------------

    // Submit a screen-space triangle (all three vertices pre-projected).
    // Mirrors DrawTriangleInProjectionSpace_B6253.
    void submitTriangle(const VR_Vertex& v0,
                        const VR_Vertex& v1,
                        const VR_Vertex& v2,
                        uint32_t blendMode,
                        uint32_t textureIndex,
                        uint32_t lightTableBase);

    // Submit a filled rectangle (sky clear / solid fill).
    void submitRect(int x, int y, int w, int h,
                    uint8_t colourIndex);

    // Submit a sprite quad.
    void submitSprite(int screenX,    int screenY,
                      int screenW,    int screenH,
                      int srcU,       int srcV,
                      int srcW,       int srcH,
                      uint32_t textureIndex,
                      uint32_t blendMode,
                      uint32_t shadeIndex);

    // -----------------------------------------------------------------------
    // Accessors
    // -----------------------------------------------------------------------
    VkDevice         device()         const { return m_device; }
    VkPhysicalDevice physicalDevice() const { return m_physDevice; }
    uint32_t         screenWidth()    const { return m_screenW; }
    uint32_t         screenHeight()   const { return m_screenH; }

private:
    // -----------------------------------------------------------------------
    // Internal helpers
    // -----------------------------------------------------------------------
    bool createInstance();
    bool selectPhysicalDevice();
    bool createDevice();
    bool createCommandPool();
    bool createOffscreenTarget();
    bool createReadbackBuffer();
    bool createVertexIndexBuffers();
    bool createDescriptorPool();
    bool createDescriptorSetLayout();
    bool createDescriptorSets();
    bool createPipelines();
    bool createRenderPass();
    bool createFramebuffer();
    bool createSyncObjects();

    void destroyBuffer(VR_Buffer& buf);
    void destroyImage(VR_Image& img);

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags props);
    bool     createBuffer(VkDeviceSize size,
                          VkBufferUsageFlags usage,
                          VkMemoryPropertyFlags props,
                          VR_Buffer& out);
    bool     createImage(uint32_t w, uint32_t h,
                         VkFormat fmt,
                         VkImageUsageFlags usage,
                         VkMemoryPropertyFlags props,
                         VR_Image& out);
    VkImageView createImageView(VkImage image, VkFormat fmt,
                                VkImageAspectFlags aspect);
    VkShaderModule createShaderModule(const uint32_t* spv, size_t sizeBytes);

    void flushBatch();
    void transitionImageLayout(VkCommandBuffer cmd,
                               VkImage image,
                               VkImageLayout oldLayout,
                               VkImageLayout newLayout);

    // -----------------------------------------------------------------------
    // Vulkan handles
    // -----------------------------------------------------------------------
    bool             m_ownsInstance   = false;
    bool             m_ownsDevice     = false;

    VkInstance       m_instance       = VK_NULL_HANDLE;
    VkPhysicalDevice m_physDevice     = VK_NULL_HANDLE;
    VkDevice         m_device         = VK_NULL_HANDLE;

    uint32_t         m_graphicsFamily = UINT32_MAX;
    uint32_t         m_transferFamily = UINT32_MAX;
    VkQueue          m_graphicsQueue  = VK_NULL_HANDLE;

    VkCommandPool    m_cmdPool        = VK_NULL_HANDLE;
    VkCommandBuffer  m_cmdBuf         = VK_NULL_HANDLE;   // current frame

    // Offscreen render target (8-bit R8_UINT)
    VR_Image         m_renderTarget;
    VkRenderPass     m_renderPass     = VK_NULL_HANDLE;
    VkFramebuffer    m_framebuffer    = VK_NULL_HANDLE;

    // Readback buffer (host-visible)
    VR_Buffer        m_readbackBuf;

    // Vertex / index streams (host-visible, persistently mapped)
    VR_Buffer        m_vertexBuf;
    VR_Buffer        m_indexBuf;
    VR_Vertex*       m_vtxPtr         = nullptr;   // write cursor
    uint32_t*        m_idxPtr         = nullptr;   // write cursor
    uint32_t         m_vtxCount       = 0;
    uint32_t         m_idxCount       = 0;

    // Texture atlas (device-local, 4096×4096 R8_UINT)
    VR_Image         m_textureAtlas;
    VkSampler        m_sampler        = VK_NULL_HANDLE;
    uint32_t         m_atlasNextSlotX = 0;
    uint32_t         m_atlasNextSlotY = 0;
    uint32_t         m_atlasRowHeight = 0;

    // Palette buffer (256 × RGBA8 = 1 KB)
    VR_Buffer        m_paletteBuf;

    // Light/shade table buffer (3 × 256 × 256 bytes)
    VR_Buffer        m_lightTableBuf;

    // Descriptor pool / sets / layout
    VkDescriptorPool      m_descPool        = VK_NULL_HANDLE;
    VkDescriptorSetLayout m_descSetLayout   = VK_NULL_HANDLE;
    VkDescriptorSet       m_descSet         = VK_NULL_HANDLE;

    // Pipeline layout shared by all pipelines
    VkPipelineLayout      m_pipelineLayout  = VK_NULL_HANDLE;

    // One pipeline per "blend mode group"
    enum class PipelineType : uint32_t
    {
        SolidFill       = 0,   // modes 0,E,F
        TexturedOpaque  = 1,   // modes 2,3
        TexturedShaded  = 2,   // modes 5,6,7,B
        TexturedKeyTrans= 3,   // modes 8,9,A
        TexturedDarkBlend=4,   // modes C,D
        GradientFill    = 5,   // modes 1,4,10,11
        TexturedScreenBlend=6, // modes 12,13
        TexturedDualTable=7,   // modes 14,15
        TexturedTransScreen=8, // modes 16,17
        TexturedTransDual =9,  // modes 18,19
        ReflectionBlend =10,   // mode 1A
        COUNT
    };
    static constexpr uint32_t PIPELINE_COUNT = static_cast<uint32_t>(PipelineType::COUNT);
    std::array<VkPipeline, PIPELINE_COUNT> m_pipelines{};

    // Batch records
    std::vector<VR_DrawCall> m_drawCalls;
    VR_DrawCall              m_currentBatch{};
    bool                     m_batchOpen = false;

    VkFence   m_fence     = VK_NULL_HANDLE;

    // Screen / viewport dimensions
    uint32_t  m_screenW   = 0;
    uint32_t  m_screenH   = 0;
    uint32_t  m_vpX       = 0;
    uint32_t  m_vpY       = 0;
    uint32_t  m_vpW       = 0;
    uint32_t  m_vpH       = 0;
};

// ---------------------------------------------------------------------------
// Blend-mode → PipelineType lookup (used by GameRenderVulkan)
// ---------------------------------------------------------------------------
inline VulkanRenderer::PipelineType blendModeToPipeline(uint32_t mode)
{
    using PT = VulkanRenderer::PipelineType;
    switch (mode)
    {
    case 0x00: case 0x0E: case 0x0F: return PT::SolidFill;
    case 0x02: case 0x03:            return PT::TexturedOpaque;
    case 0x05: case 0x06:
    case 0x07: case 0x0B:            return PT::TexturedShaded;
    case 0x08: case 0x09: case 0x0A: return PT::TexturedKeyTrans;
    case 0x0C: case 0x0D:            return PT::TexturedDarkBlend;
    case 0x01: case 0x04:
    case 0x10: case 0x11:            return PT::GradientFill;
    case 0x12: case 0x13:            return PT::TexturedScreenBlend;
    case 0x14: case 0x15:            return PT::TexturedDualTable;
    case 0x16: case 0x17:            return PT::TexturedTransScreen;
    case 0x18: case 0x19:            return PT::TexturedTransDual;
    case 0x1A:                       return PT::ReflectionBlend;
    default:                         return PT::TexturedOpaque;
    }
}

#endif // VULKAN_RENDERER_H
