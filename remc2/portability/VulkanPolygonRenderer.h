#pragma once
#ifndef VULKANPOLYGONRENDER_H
#define VULKANPOLYGONRENDER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <VkBootstrap.h>
#if __has_include(<vma/vk_mem_alloc.h>)
#include <vma/vk_mem_alloc.h>
#else
#include <vk_mem_alloc.h>
#endif
#include <vector>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <functional>

#include "../shared/ResourceType.h"
#include "../shared/EventDispatcher.h"
#include "../engine/Basic.h"
#include "RenderPolygon.h"

struct SDL_Window;

// Minimal Vulkan renderer that takes a per-frame list of RenderPolygon
// (screen-space, palette-textured, Gouraud-shaded) and rasterizes them.
//
// Usage per frame:
//     renderer.BeginFrame();
//     renderer.DrawPolygons(polygons);
//     renderer.EndFrame();
//
// Textures are 8-bit palette-indexed (one byte per texel = palette index,
// same convention as m_gamePalletisedSurface in the SDL path). Upload once
// with UploadTexture() and reuse the returned TextureId across frames.
// Call SetPalette() whenever the 256-color palette changes.
class VulkanPolygonRenderer
{
public:
	// enableValidation loads the Khronos validation layers: very slow, so only when chasing a Vulkan bug.
	VulkanPolygonRenderer(std::string shaderFolder, bool enableValidation = false);
	~VulkanPolygonRenderer();

	VulkanPolygonRenderer(const VulkanPolygonRenderer&) = delete;
	VulkanPolygonRenderer& operator=(const VulkanPolygonRenderer&) = delete;

	// Creates instance/device/swapchain/pipeline. windowWidth/Height should
	// match the SDL window's drawable size in pixels.
	bool Init(SDL_Window* window, int windowWidth, int windowHeight);

	void Shutdown();

	// Call when the window is resized (recreates swapchain + framebuffers).
	void Resize(int windowWidth, int windowHeight);

	// rgb triplets, 256 entries (768 bytes), 0-255 per channel.
	// (If you're feeding it straight from VGA_Get_Palette()/tempPalettebuffer,
	// multiply by 4 first - those are 6-bit VGA DAC values, see VGA_Set_Palette.)
	void SetPalette(const uint8_t* rgb768);
	void SetColourLookupTable(uint8_t* colourLookupTable);
	void SetClearColourIndex(uint8_t paletteIndex);

	// Uploads an 8-bit indexed texture (width*height bytes, one palette
	// index per texel) and returns a TextureId to use in RenderPolygon.
	uint32_t UploadTexture(uint32_t index, const uint8_t* indexedPixels, uint32_t width, uint32_t height);

	// Frees a previously uploaded texture's GPU resources.
	void FreeTexture(uint32_t textureId);

	std::string ShaderPath(const char* filename);
	std::vector<char> ReadFile(const std::string& path);

	// If your projection code winds vertices clockwise in screen space,
	// leave this at default; flip if you get back-face culling artifacts.
	void SetFrontFace(VkFrontFace frontFace);
	void SetCullMode(VkCullModeFlags cullMode); // default: VK_CULL_MODE_NONE

	// Hardware Settings; both take effect when Init() builds the pipelines, so call them before it.
	void SetBilinearFiltering(bool enabled);
	void SetAntiAliasing(int samples);   // MSAA samples: 0/1 = off, 2, 4 or 8; capped to what the GPU supports

	// hudSurface is the 8-bit palettised screen; hudAlpha is one alpha byte per pixel of it, or nullptr for opaque.
	bool BeginFrame(SDL_Surface* hudSurface, const uint8_t* hudAlpha, SDL_Rect srcRect, SDL_Rect destRect);
	bool UploadHud(SDL_Surface* hudSurface, const uint8_t* hudAlpha, VkCommandBuffer commandBuffer);   // false = nothing to draw
	void DrawOverlay(SDL_Rect dscrect, VkCommandBuffer commandBuffer);
	static constexpr size_t kNoReflectionPass = (size_t)-1;
	void DrawPolygons(const std::vector<RenderPolygon>& polygons, size_t reflectionEnd = kNoReflectionPass);
	void DrawPolygonsWireframe(const std::vector<RenderPolygon>& polygons);
	void EndFrame();

private:

	static constexpr uint32_t kShadeRows = 64;

	VkImage       m_shadeLutImage = VK_NULL_HANDLE;
	VmaAllocation m_shadeLutAllocation = nullptr;
	VkImageView   m_shadeLutView = VK_NULL_HANDLE;
	VkBuffer      m_shadeLutStaging = VK_NULL_HANDLE;
	VmaAllocation m_shadeLutStagingAllocation = nullptr;
	void* m_shadeLutStagingMapped = nullptr;

	VkImage       m_blendLutImage = VK_NULL_HANDLE;
	VmaAllocation m_blendLutAllocation = nullptr;
	VkImageView   m_blendLutView = VK_NULL_HANDLE;
	VkBuffer      m_blendLutStaging = VK_NULL_HANDLE;
	VmaAllocation m_blendLutStagingAllocation = nullptr;
	void* m_blendLutStagingMapped = nullptr;

	bool CreateShadeLutTexture();
	bool CreateBlendLutTexture();
	void UploadStagedImage(VkImage image, VkBuffer staging, uint32_t width, uint32_t height);
	void RefreshClearColour();

	struct Texture
	{
		VkImage image = VK_NULL_HANDLE;
		VkImageView view = VK_NULL_HANDLE;
		VmaAllocation allocation = VK_NULL_HANDLE;
		VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
		uint32_t width = 0;
		uint32_t height = 0;
	};

	struct FrameData
	{
		VkCommandPool commandPool = VK_NULL_HANDLE;
		VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
		VkSemaphore imageAvailable = VK_NULL_HANDLE;
		VkSemaphore renderFinished = VK_NULL_HANDLE;
		VkFence inFlight = VK_NULL_HANDLE;

		// Host-visible, persistently-mapped per-frame vertex/index buffers.
		VkBuffer vertexBuffer = VK_NULL_HANDLE;
		VmaAllocation vertexAllocation = VK_NULL_HANDLE;
		void* vertexMapped = nullptr;
		VkDeviceSize vertexBufferSize = 0;

		VkBuffer indexBuffer = VK_NULL_HANDLE;
		VmaAllocation indexAllocation = VK_NULL_HANDLE;
		void* indexMapped = nullptr;
		VkDeviceSize indexBufferSize = 0;
	};

	std::vector<RenderPolygon> m_polygons;

	void SetPolygons(ResourceType state, std::vector<RenderPolygon>* polygons);
	void SetTexture(ResourceType state, uint32_t index, uint8_t* pixels, uint32_t width, uint32_t height);
	void SetShadeLut(const uint8_t* table);   // first kShadeRows * 256 bytes of ColourLookupTable_F6EE0
	void SetBlendLut(const uint8_t* lut65536);
	bool CreateSwapchain(int width, int height);
	void DestroySwapchain();
	bool CreateRenderPass();
	bool CreatePipeline();
	bool CreateWireframePipeline();
	bool CreateDescriptorResources();
	bool CreatePaletteTexture();
	bool CreateFrameData();
	bool CreateOverlayResources();
	bool EnsureHudImage(uint32_t width, uint32_t height);
	void DestroyHudImage();
	enum class OverlayBlend
	{
		Opaque,     // straight copy
		Alpha,      // alpha-over, for the HUD
		Constant,   // src * blend constant + dst * (1 - blend constant), for the blur history
	};

	struct ColourTarget
	{
		VkImage image = VK_NULL_HANDLE;
		VmaAllocation allocation = VK_NULL_HANDLE;
		VkImageView view = VK_NULL_HANDLE;
		VkFramebuffer framebuffer = VK_NULL_HANDLE;
		VkDescriptorSet descriptorSet = VK_NULL_HANDLE;   // overlay layout, for sampling the target
	};

	// layout VK_NULL_HANDLE = the overlay layout (one sampled image); renderPass VK_NULL_HANDLE = m_renderPass
	bool CreateOverlayPipeline(OverlayBlend blendMode, VkPipeline& pipeline,
		const char* fragShader = "overlay.frag.spv", VkPipelineLayout layout = VK_NULL_HANDLE,
		VkRenderPass renderPass = VK_NULL_HANDLE, VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT);
	bool CreateOffscreenRenderPass(VkAttachmentLoadOp loadOp, VkImageLayout initialLayout, VkRenderPass& renderPass,
		VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT);
	bool CreateOffscreenTarget(uint32_t width, uint32_t height);
	void DestroyOffscreenTarget();
	// msaaView: the multisampled image the render pass resolves into target, or VK_NULL_HANDLE without MSAA
	bool CreateColourTarget(uint32_t width, uint32_t height, VkRenderPass renderPass, ColourTarget& target,
		VkImageView msaaView = VK_NULL_HANDLE);
	bool CreateMsaaImage(uint32_t width, uint32_t height);
	void DestroyMsaaImage();
	VkSampleCountFlagBits PickMsaaSamples(int requested) const;
	void DestroyColourTarget(ColourTarget& target);
	void DrawBlur(VkCommandBuffer commandBuffer);
	size_t ReflectionEnd(const std::vector<RenderPolygon>& polygons) const;
	void BeginTerrainPass(VkCommandBuffer commandBuffer);
	void DestroyFrameData();
	void EnsureFrameBufferCapacity(FrameData& frame, size_t vertexBytes, size_t indexBytes);
	VkDescriptorSet AllocateTextureDescriptorSet(VkImageView view);

	VkSampler m_overlaySampler = VK_NULL_HANDLE;
	VkDescriptorSetLayout m_overlayDescriptorSetLayout = VK_NULL_HANDLE;
	VkPipelineLayout m_overlayPipelineLayout = VK_NULL_HANDLE;

	// The HUD is uploaded as the game's 8-bit screen and resolved through the palette on the GPU.
	// m_hudImage is R8_UINT with two layers at game resolution: layer 0 = palette index, layer 1 = alpha.
	static constexpr VkDeviceSize kHudPaletteBytes = 256 * 4;
	VkImage m_hudImage = VK_NULL_HANDLE;
	VmaAllocation m_hudImageAlloc = VK_NULL_HANDLE;
	VkImageView m_hudImageView = VK_NULL_HANDLE;
	VkImage m_hudPaletteImage = VK_NULL_HANDLE;        // 256x1 RGBA8
	VmaAllocation m_hudPaletteAlloc = VK_NULL_HANDLE;
	VkImageView m_hudPaletteView = VK_NULL_HANDLE;
	VkDescriptorSetLayout m_hudDescriptorSetLayout = VK_NULL_HANDLE;
	VkDescriptorSet m_hudDescriptorSet = VK_NULL_HANDLE;
	VkPipelineLayout m_hudPipelineLayout = VK_NULL_HANDLE;
	VkPipeline m_hudPipeline = VK_NULL_HANDLE;

	// One slot per frame in flight, each [palette][indices][alpha], so a frame never overwrites one still being copied.
	VkBuffer m_hudStagingBuffer = VK_NULL_HANDLE;
	VmaAllocation m_hudStagingAlloc = VK_NULL_HANDLE;
	void* m_hudStagingMapped = nullptr;
	VkDeviceSize m_hudStagingSlotSize = 0;
	uint32_t m_hudWidth = 0;
	uint32_t m_hudHeight = 0;
	bool m_hudHasAlpha = false;

	// Polygons are rendered into this image at the game viewport's resolution,
	// then upscaled into the window with m_viewPortBlitPipeline.
	VkRenderPass m_offscreenRenderPass = VK_NULL_HANDLE;
	ColourTarget m_offscreenTarget;
	ColourTarget m_reflectionTarget;
	VkDescriptorSet m_noReflectionSet = VK_NULL_HANDLE;
	uint32_t m_offscreenWidth = 0;
	uint32_t m_offscreenHeight = 0;
	VkPipeline m_viewPortBlitPipeline = VK_NULL_HANDLE;

	// MSAA: the offscreen render pass draws into this multisampled image and resolves it into the target it is
	// drawn for (the reflection, then the offscreen target: one after the other, so they share it).
	// Unused (null) when m_msaaSamples is 1.
	int m_requestedMsaaSamples = 0;
	VkSampleCountFlagBits m_msaaSamples = VK_SAMPLE_COUNT_1_BIT;
	VkImage m_msaaImage = VK_NULL_HANDLE;
	VmaAllocation m_msaaAllocation = VK_NULL_HANDLE;
	VkImageView m_msaaView = VK_NULL_HANDLE;
	VkPipeline m_reflectionCopyPipeline = VK_NULL_HANDLE;   // m_viewPortBlitPipeline for the offscreen render pass (its sample count)

	bool m_bilinearFiltering = false;   // polygon.frag's BILINEAR specialization constant

	// Motion blur: while active, each frame is blended into m_blurTarget (same size as the
	// offscreen target) and that is upscaled into the window instead of the offscreen image.
	static constexpr float kBlurNewFrameWeight = 1.0f - COLOUR_BLEND_ALPHA;
	VkRenderPass m_blurSeedRenderPass = VK_NULL_HANDLE;
	VkRenderPass m_blurRenderPass = VK_NULL_HANDLE;
	ColourTarget m_blurTarget;
	VkPipeline m_blurPipeline = VK_NULL_HANDLE;
	bool m_blurActive = false;
	bool m_blurSeed = true;   // the history holds nothing useful, so the next blur frame copies instead of blends

	vkb::Instance m_vkbInstance{};
	vkb::Device m_vkbDevice{};
	vkb::Swapchain m_vkbSwapchain{};

	VkInstance m_instance = VK_NULL_HANDLE;
	VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
	VkSurfaceKHR m_surface = VK_NULL_HANDLE;
	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
	VkDevice m_device = VK_NULL_HANDLE;
	VkQueue m_graphicsQueue = VK_NULL_HANDLE;
	uint32_t m_graphicsQueueFamily = 0;

	VmaAllocator m_allocator = VK_NULL_HANDLE;

	VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;
	VkFormat m_swapchainFormat = VK_FORMAT_UNDEFINED;
	VkExtent2D m_swapchainExtent{};
	std::vector<VkImage> m_swapchainImages;
	std::vector<VkImageView> m_swapchainImageViews;
	std::vector<VkFramebuffer> m_framebuffers;

	VkRenderPass m_renderPass = VK_NULL_HANDLE;
	VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
	VkPipelineLayout m_wireframePipelineLayout = VK_NULL_HANDLE;
	VkPipeline m_pipeline = VK_NULL_HANDLE;
	VkPipeline m_wireframePipeline = VK_NULL_HANDLE;

	VkDescriptorSetLayout m_descriptorSetLayout = VK_NULL_HANDLE;
	VkDescriptorPool m_descriptorPool = VK_NULL_HANDLE;
	VkSampler m_nearestSampler = VK_NULL_HANDLE;

	// Palette: 256x1 RGBA8 texture, binding 0 in the descriptor set.
	VkImage m_paletteImage = VK_NULL_HANDLE;
	VkImageView m_paletteView = VK_NULL_HANDLE;
	VmaAllocation m_paletteAllocation = VK_NULL_HANDLE;
	VkBuffer m_paletteStagingBuffer = VK_NULL_HANDLE;
	VmaAllocation m_paletteStagingAllocation = VK_NULL_HANDLE;
	void* m_paletteStagingMapped = nullptr;

	std::unordered_map<uint32_t, Texture> m_textures;

	static constexpr int kFramesInFlight = 2;
	FrameData m_frames[kFramesInFlight];
	size_t m_currentFrame = 0;
	uint32_t m_currentImageIndex = 0;

	VkFrontFace m_frontFace = VK_FRONT_FACE_CLOCKWISE;
	VkCullModeFlags m_cullMode = VK_CULL_MODE_BACK_BIT;

	VkClearColorValue m_clearColor{ { 0.0f, 0.0f, 0.0f, 1.0f } };
	int  m_clearPaletteIndex = -1;      // -1 = keep black
	bool m_clearColorSet = false;

	int m_windowWidth = 0;
	int m_windowHeight = 0;
	SDL_Window* m_window = nullptr;

	// Game world viewport in game-resolution pixels; w/h of 0 = not yet set, use full destRect
	SDL_Rect m_gameViewPort{ 0, 0, 0, 0 };

	bool m_initialized = false;
	bool m_enableValidation = false;
	std::string m_ShaderFolder;

	std::function<void(ResourceType, uint8_t)> m_SetClearColourIdxCallBack;
	std::function<void(ResourceType, uint8_t*)> m_SetPaletteLookupCallBack;
	std::function<void(ResourceType, uint32_t, uint8_t*, uint32_t, uint32_t)> m_TextureCallBack;
	std::function<void(ResourceType, std::vector<RenderPolygon>*)> m_PolyCallBack;
	std::function<void(uint16_t, uint16_t, uint16_t, uint16_t)> m_ViewPortCallBack;
	std::function<void(bool)> m_BlurEffectCallBack;
};

#endif //VULKANPOLYGONRENDER_H 