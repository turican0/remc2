#include "LookingGlassRenderer.h"

#include <cmath>

LookingGlassRenderer::LookingGlassRenderer(std::string shaderFolder, bool enableValidation)
	: VulkanPolygonRenderer(shaderFolder, enableValidation)
{
}

LookingGlassRenderer::~LookingGlassRenderer()
{
	// The base destructor can't reach this class's resources (it would call its own Shutdown), so free them here
	Shutdown();
}

void LookingGlassRenderer::SetLookingGlassDevice(const LookingGlassDevice& device)
{
	m_lenticular = device.found;
	m_lenticularWidth = (uint32_t)device.width;
	m_lenticularHeight = (uint32_t)device.height;
	m_lenticularConstants.pitch = device.pitch;
	m_lenticularConstants.tilt = device.tilt;
	m_lenticularConstants.center = device.center;
	m_lenticularConstants.subp = device.subp;
	m_lenticularConstants.invView = device.invView;
	m_lenticularConstants.ri = device.ri;
	m_lenticularConstants.bi = device.bi;
	m_lenticularConstants.flipX = device.flipX ? 1 : 0;
}

void LookingGlassRenderer::Shutdown()
{
	if (!m_initialized)
		return;

	vkDeviceWaitIdle(m_device);
	DestroyLookingGlassResources();
	VulkanPolygonRenderer::Shutdown();
}

void LookingGlassRenderer::DestroyLookingGlassResources()
{
	DestroyColourTarget(m_quiltTarget);
	m_quiltExtent = { 0, 0 };
	vkDestroyPipeline(m_device, m_lenticularPipeline, nullptr);
	m_lenticularPipeline = VK_NULL_HANDLE;
	vkDestroyPipelineLayout(m_device, m_lenticularPipelineLayout, nullptr);
	m_lenticularPipelineLayout = VK_NULL_HANDLE;
	vkDestroyRenderPass(m_device, m_quiltRenderPass, nullptr);
	m_quiltRenderPass = VK_NULL_HANDLE;
}

bool LookingGlassRenderer::CreateExtraResources()
{
	return !m_lenticular || CreateLenticularResources();
}

// ── Cells: the quilt ─────────────────────────────────────────────────────────────────────────────────────────
// Without a display the base lays the game's grid out in the window. With one the cells go into the quilt image
// instead (in the same order: the leftmost view bottom left, then left to right and bottom to top). Each view fills the
// whole display, so a cell has the display's shape: the frame (the game resolution, 1:1) fits it in one direction and
// is centred with black bars in the other, for screens of another shape than the cells in the config (the menus,
// loading screens)
VulkanPolygonRenderer::FrameCells LookingGlassRenderer::GetFrameCells(const SDL_Rect& srcRect, const SDL_Rect& destRect)
{
	m_frameLenticular = m_lenticular && srcRect.w > 0 && srcRect.h > 0;
	if (!m_frameLenticular)
		return VulkanPolygonRenderer::GetFrameCells(srcRect, destRect);

	int cellWidth = srcRect.w;
	int cellHeight = srcRect.h;
	if (m_lenticularWidth > 0 && m_lenticularHeight > 0)
	{
		const double displayAspect = (double)m_lenticularWidth / (double)m_lenticularHeight;
		if ((double)srcRect.w / (double)srcRect.h > displayAspect)
			cellHeight = (int)std::lround((double)srcRect.w / displayAspect);   // wider: bars above and below
		else
			cellWidth = (int)std::lround((double)srcRect.h * displayAspect);    // taller: bars at the sides
	}
	const VkExtent2D quiltExtent{ (uint32_t)(cellWidth * m_viewColumns), (uint32_t)(cellHeight * m_viewRows) };
	if (!EnsureQuiltTarget(quiltExtent.width, quiltExtent.height))
	{
		Logger->error("Looking Glass: could not create the {}x{} quilt image, showing the quilt instead", quiltExtent.width, quiltExtent.height);
		m_lenticular = m_frameLenticular = false;
		return VulkanPolygonRenderer::GetFrameCells(srcRect, destRect);
	}

	FrameCells cells;
	cells.count = (size_t)m_viewRows * m_viewColumns;
	cells.rect = SDL_Rect{ 0, 0, (int)quiltExtent.width, (int)quiltExtent.height };
	cells.extent = quiltExtent;
	return cells;
}

SDL_Rect LookingGlassRenderer::GetCellRect(size_t cell, const FrameCells& cells, const SDL_Rect& srcRect) const
{
	const SDL_Rect gridCell = VulkanPolygonRenderer::GetCellRect(cell, cells, srcRect);
	if (!m_frameLenticular)
		return gridCell;

	// The frame at 1:1, centred in the quilt's cell (the quilt's clear colour makes the bars)
	return SDL_Rect{ gridCell.x + (gridCell.w - srcRect.w) / 2, gridCell.y + (gridCell.h - srcRect.h) / 2, srcRect.w, srcRect.h };
}

// The quilt first, into its own image, when the window shows it interleaved; otherwise the window
void LookingGlassRenderer::BeginCellsRenderPass(VkCommandBuffer commandBuffer, const VkClearValue& clearColor)
{
	if (!m_frameLenticular)
	{
		VulkanPolygonRenderer::BeginCellsRenderPass(commandBuffer, clearColor);
		return;
	}

	VkRenderPassBeginInfo quiltBegin{};
	quiltBegin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	quiltBegin.renderPass = m_quiltRenderPass;
	quiltBegin.framebuffer = m_quiltTarget.framebuffer;
	quiltBegin.renderArea.extent = m_quiltExtent;
	quiltBegin.clearValueCount = 1;
	quiltBegin.pClearValues = &clearColor;
	vkCmdBeginRenderPass(commandBuffer, &quiltBegin, VK_SUBPASS_CONTENTS_INLINE);
}

// Then the window: the quilt interleaved for the display (EndFrame ends this pass)
void LookingGlassRenderer::EndCellsRenderPass(VkCommandBuffer commandBuffer, const VkClearValue& clearColor)
{
	if (!m_frameLenticular)
		return;

	vkCmdEndRenderPass(commandBuffer);
	BeginWindowRenderPass(commandBuffer, clearColor);
	DrawLenticular(commandBuffer);
}

// The quilt's render pass (the swapchain's format and one sample: the window's pipelines are compatible with it) and
// the interleave's pipeline (lenticular.frag: the quilt sampled through the overlay layout, the calibration as push
// constants)
bool LookingGlassRenderer::CreateLenticularResources()
{
	// The interleave works in screen fractions, so it only lines up with the lenticules at the display's own size
	if (m_swapchainExtent.width != m_lenticularWidth || m_swapchainExtent.height != m_lenticularHeight)
		Logger->warn("Looking Glass: the window is {}x{}, not the display's {}x{}: the hologram will not line up "
			"(is Windows scaling the display?)", m_swapchainExtent.width, m_swapchainExtent.height, m_lenticularWidth, m_lenticularHeight);

	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = m_swapchainFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkAttachmentReference colorRef{ 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorRef;

	// In: the previous frame's interleave has finished reading the quilt (and the views' passes are done, as for the
	// window's pass). Out: the interleave reads what was drawn.
	VkSubpassDependency dependencies[2]{};
	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	dependencies[0].srcAccessMask = 0;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[1].srcSubpass = 0;
	dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	VkRenderPassCreateInfo rpInfo{};
	rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	rpInfo.attachmentCount = 1;
	rpInfo.pAttachments = &colorAttachment;
	rpInfo.subpassCount = 1;
	rpInfo.pSubpasses = &subpass;
	rpInfo.dependencyCount = 2;
	rpInfo.pDependencies = dependencies;
	if (vkCreateRenderPass(m_device, &rpInfo, nullptr, &m_quiltRenderPass) != VK_SUCCESS)
		return false;

	VkPushConstantRange pushRange{};
	pushRange.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	pushRange.offset = 0;
	pushRange.size = sizeof(LenticularPushConstants);

	VkPipelineLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	layoutInfo.setLayoutCount = 1;
	layoutInfo.pSetLayouts = &m_overlayDescriptorSetLayout;
	layoutInfo.pushConstantRangeCount = 1;
	layoutInfo.pPushConstantRanges = &pushRange;
	if (vkCreatePipelineLayout(m_device, &layoutInfo, nullptr, &m_lenticularPipelineLayout) != VK_SUCCESS)
		return false;

	return CreateOverlayPipeline(OverlayBlend::Opaque, m_lenticularPipeline, "lenticular.frag.spv", m_lenticularPipelineLayout);
}

bool LookingGlassRenderer::EnsureQuiltTarget(uint32_t width, uint32_t height)
{
	if (m_quiltTarget.framebuffer && m_quiltExtent.width == width && m_quiltExtent.height == height)
		return true;
	// The other frame in flight may still be reading the old quilt
	vkDeviceWaitIdle(m_device);
	DestroyColourTarget(m_quiltTarget);
	m_quiltExtent = { 0, 0 };
	if (!CreateColourTarget(width, height, m_quiltRenderPass, m_quiltTarget))
	{
		DestroyColourTarget(m_quiltTarget);
		return false;
	}
	m_quiltExtent = { width, height };
	return true;
}

// The quilt interleaved over the whole window (the display): every pixel picks its subpixels' views
void LookingGlassRenderer::DrawLenticular(VkCommandBuffer commandBuffer)
{
	VkViewport viewport{ 0.0f, 0.0f, (float)m_swapchainExtent.width, (float)m_swapchainExtent.height, 0.0f, 1.0f };
	VkRect2D scissor{ { 0, 0 }, m_swapchainExtent };
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	LenticularPushConstants constants = m_lenticularConstants;
	constants.columns = m_viewColumns;
	constants.rows = m_viewRows;
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_lenticularPipeline);
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_lenticularPipelineLayout, 0, 1,
		&m_quiltTarget.descriptorSet, 0, nullptr);
	vkCmdPushConstants(commandBuffer, m_lenticularPipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(constants), &constants);
	vkCmdDraw(commandBuffer, 3, 1, 0, 0);
}
