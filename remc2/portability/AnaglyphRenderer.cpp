#include "AnaglyphRenderer.h"

#include <algorithm>

AnaglyphRenderer::AnaglyphRenderer(std::string shaderFolder, bool enableValidation)
	: VulkanPolygonRenderer(shaderFolder, enableValidation)
{
}

AnaglyphRenderer::~AnaglyphRenderer()
{
	// The base destructor can't reach this class's resources (it would call its own Shutdown), so free them here
	Shutdown();
}

void AnaglyphRenderer::SetColours(const uint8_t leftRgb[3], const uint8_t rightRgb[3])
{
	for (int channel = 0; channel < 3; channel++)
	{
		m_filters.leftColour[channel] = leftRgb[channel] / 255.0f;
		m_filters.rightColour[channel] = rightRgb[channel] / 255.0f;
	}
	m_filters.leftColour[3] = 1.0f;
	m_filters.rightColour[3] = 1.0f;
}

void AnaglyphRenderer::SetGhostReduction(float amount)
{
	m_filters.ghost[0] = (std::clamp)(amount, 0.0f, 0.5f);
}

void AnaglyphRenderer::Shutdown()
{
	if (!m_initialized)
		return;

	vkDeviceWaitIdle(m_device);
	vkDestroyPipeline(m_device, m_anaglyphPipeline, nullptr);
	m_anaglyphPipeline = VK_NULL_HANDLE;
	vkDestroyPipelineLayout(m_device, m_anaglyphPipelineLayout, nullptr);
	m_anaglyphPipelineLayout = VK_NULL_HANDLE;
	VulkanPolygonRenderer::Shutdown();
}

// The combining pipeline: the two eyes' images through the overlay layout (one set each), the lenses' colours as push
// constants, into the window's render pass
bool AnaglyphRenderer::CreateExtraResources()
{
	VkPushConstantRange pushRange{};
	pushRange.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	pushRange.offset = 0;
	pushRange.size = sizeof(FilterPushConstants);

	const VkDescriptorSetLayout setLayouts[2] = { m_overlayDescriptorSetLayout, m_overlayDescriptorSetLayout };
	VkPipelineLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	layoutInfo.setLayoutCount = 2;
	layoutInfo.pSetLayouts = setLayouts;
	layoutInfo.pushConstantRangeCount = 1;
	layoutInfo.pPushConstantRanges = &pushRange;
	if (vkCreatePipelineLayout(m_device, &layoutInfo, nullptr, &m_anaglyphPipelineLayout) != VK_SUCCESS)
		return false;

	return CreateOverlayPipeline(OverlayBlend::Opaque, m_anaglyphPipeline, "anaglyph.frag.spv", m_anaglyphPipelineLayout);
}

// An anaglyph frame: the game's two eyes for one cell
bool AnaglyphRenderer::IsAnaglyphFrame() const
{
	return m_viewComposition == ViewComposition::Anaglyph && m_views.size() >= 2 && m_viewRows * m_viewColumns == 1;
}

// Both eyes are rendered, each into its own image
size_t AnaglyphRenderer::GetFrameViewCount(size_t cellCount) const
{
	return IsAnaglyphFrame() ? 2 : VulkanPolygonRenderer::GetFrameViewCount(cellCount);
}

// The eyes combined through the lenses' colours (full colour, or half-colour on a dark level)
void AnaglyphRenderer::DrawCellViews(VkCommandBuffer commandBuffer, size_t cell, size_t view)
{
	if (!IsAnaglyphFrame() || m_viewTargets.size() < 2 || m_anaglyphPipeline == VK_NULL_HANDLE)
	{
		VulkanPolygonRenderer::DrawCellViews(commandBuffer, cell, view);
		return;
	}

	const VkDescriptorSet eyes[2] = { GetViewImage(0), GetViewImage(1) };
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_anaglyphPipeline);
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_anaglyphPipelineLayout, 0, 2, eyes, 0, nullptr);
	FilterPushConstants filters = m_filters;
	filters.ghost[1] = m_darkLevel ? 1.0f : 0.0f;   // night and cave levels: half-colour
	vkCmdPushConstants(commandBuffer, m_anaglyphPipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(filters), &filters);
	vkCmdDraw(commandBuffer, 3, 1, 0, 0);
}
