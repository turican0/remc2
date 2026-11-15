#pragma once
#ifndef LOOKINGGLASSRENDERER_H
#define LOOKINGGLASSRENDERER_H

#include "VulkanPolygonRenderer.h"
#include "LookingGlassBridge.h"

// The Vulkan renderer for Looking Glass (lenticular) displays: the frame is a quilt of the game's rows x columns cells
// (RenderViews), each showing the whole frame (the 3D view and the HUD at the game resolution) from its own camera.
//
// Without a Looking Glass device the quilt's cells are shown side by side in the window, as the base renderer does.
// With one (SetLookingGlassDevice) the cells are drawn into a quilt image, each the display's shape with the frame
// centred in it at 1:1, and the window (the display) shows it interleaved by lenticular.frag with the device's calibration.
class LookingGlassRenderer : public VulkanPolygonRenderer
{
public:
	LookingGlassRenderer(std::string shaderFolder, bool enableValidation = false);
	~LookingGlassRenderer() override;

	static constexpr int kMaxViews = 256;

	// Before Init
	void SetLookingGlassDevice(const LookingGlassDevice& device);

	void Shutdown() override;

protected:
	FrameCells GetFrameCells(const SDL_Rect& srcRect, const SDL_Rect& destRect) override;
	SDL_Rect GetCellRect(size_t cell, const FrameCells& cells, const SDL_Rect& srcRect) const override;
	void BeginCellsRenderPass(VkCommandBuffer commandBuffer, const VkClearValue& clearColor) override;
	void EndCellsRenderPass(VkCommandBuffer commandBuffer, const VkClearValue& clearColor) override;
	bool CreateExtraResources() override;
	int MaxViews() const override { return kMaxViews; }

private:
	struct LenticularPushConstants
	{
		float pitch;
		float tilt;
		float center;
		float subp;
		int32_t invView;
		int32_t ri;
		int32_t bi;
		int32_t columns;
		int32_t rows;
		int32_t flipX;
	};

	bool CreateLenticularResources();
	bool EnsureQuiltTarget(uint32_t width, uint32_t height);
	void DrawLenticular(VkCommandBuffer commandBuffer);
	void DestroyLookingGlassResources();

	// Looking Glass display: the quilt (cells of the game resolution, view 0 bottom left) is drawn into m_quiltTarget,
	// the swapchain's format so the window's pipelines draw into it, then interleaved into the window
	bool m_lenticular = false;
	LenticularPushConstants m_lenticularConstants{};
	uint32_t m_lenticularWidth = 0;    // the display's size, to check the window against
	uint32_t m_lenticularHeight = 0;
	VkRenderPass m_quiltRenderPass = VK_NULL_HANDLE;
	ColourTarget m_quiltTarget;
	VkExtent2D m_quiltExtent{ 0, 0 };
	VkPipelineLayout m_lenticularPipelineLayout = VK_NULL_HANDLE;
	VkPipeline m_lenticularPipeline = VK_NULL_HANDLE;

	// This frame (set by GetFrameCells): the cells go into the quilt image to be interleaved
	bool m_frameLenticular = false;
};

#endif //LOOKINGGLASSRENDERER_H
