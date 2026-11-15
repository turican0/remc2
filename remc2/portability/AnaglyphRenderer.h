#pragma once
#ifndef ANAGLYPHRENDERER_H
#define ANAGLYPHRENDERER_H

#include "VulkanPolygonRenderer.h"

// The Vulkan renderer with anaglyph stereo (coloured glasses, red / cyan by default).
//
// It draws like the base renderer until the game sends an anaglyph frame (F11: GameRenderHW draws the left and the right
// eye with the original stereo mode's camera, as two lists with ViewComposition::Anaglyph). Then both eyes are rendered,
// each into its own image, and combined into the window by anaglyph.frag: on day levels each eye in its own colours, on
// night and cave levels the left as its brightness (half-colour, so the dark, blue scene still reaches the left eye).
class AnaglyphRenderer : public VulkanPolygonRenderer
{
public:
	AnaglyphRenderer(std::string shaderFolder, bool enableValidation = false);
	~AnaglyphRenderer() override;

	// The lenses' colours, 0 - 255 per channel (before Init or any time after)
	void SetColours(const uint8_t leftRgb[3], const uint8_t rightRgb[3]);
	// How much of the other eye's image each lens lets through, 0 - 0.5: that much of it is cancelled out
	// (0 = no ghost cancellation; higher takes more contrast)
	void SetGhostReduction(float amount);

	void Shutdown() override;

protected:
	bool CreateExtraResources() override;
	size_t GetFrameViewCount(size_t cellCount) const override;
	void DrawCellViews(VkCommandBuffer commandBuffer, size_t cell, size_t view) override;

private:
	struct FilterPushConstants
	{
		float leftColour[4];
		float rightColour[4];
		float ghost[4];   // [0]: the ghost reduction, [1]: 1 = half-colour (set per frame: a dark level)
	};

	bool IsAnaglyphFrame() const;

	// red left, cyan dimmed to 66% (#00A8A8, like the original stereo mode) right, 10% ghost reduction
	FilterPushConstants m_filters{ { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 168.0f / 255.0f, 168.0f / 255.0f, 1.0f }, { 0.1f, 0.0f, 0.0f, 0.0f } };
	VkPipelineLayout m_anaglyphPipelineLayout = VK_NULL_HANDLE;
	VkPipeline m_anaglyphPipeline = VK_NULL_HANDLE;
};

#endif //ANAGLYPHRENDERER_H
