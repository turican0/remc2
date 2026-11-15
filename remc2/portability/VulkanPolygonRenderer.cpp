#include "VulkanPolygonRenderer.h"

#define VMA_IMPLEMENTATION
#if __has_include(<vma/vk_mem_alloc.h>)
#include <vma/vk_mem_alloc.h>
#else
#include <vk_mem_alloc.h>
#endif

// ---------------------------------------------------------------------
// NOTE: shader bytecode is loaded from .spv files at runtime (see
// shaders/polygon.vert / polygon.frag + INTEGRATION.md for the CMake
// step that compiles them with glslc). Adjust ShaderPath() if you'd
// rather embed the bytes at build time instead.
// ---------------------------------------------------------------------

namespace
{
	VkShaderModule CreateShaderModule(VkDevice device, const std::vector<char>& code)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule module;
		if (vkCreateShaderModule(device, &createInfo, nullptr, &module) != VK_SUCCESS)
			throw std::runtime_error("Failed to create shader module");
		return module;
	}

	// Triangulates a convex polygon (fan from vertex 0) into a flat index list.
	void FanTriangulate(uint32_t baseVertex, uint32_t vertexCount, std::vector<uint32_t>& outIndices)
	{
		for (uint32_t i = 1; i + 1 < vertexCount; i++)
		{
			outIndices.push_back(baseVertex);
			outIndices.push_back(baseVertex + i);
			outIndices.push_back(baseVertex + i + 1);
		}
	}
}

VulkanPolygonRenderer::VulkanPolygonRenderer(std::string shaderFolder, bool enableValidation)
{
	m_ShaderFolder = shaderFolder;
	m_enableValidation = enableValidation;

	m_SetClearColourIdxCallBack =
		[this](ResourceType state, uint8_t index) {
		SetClearColourIndex(index);
		};

	RegisterEvent(new Event<ResourceType, uint8_t>(EventType::E_RESOURCE_CHANGE, m_SetClearColourIdxCallBack));

	m_SetPaletteLookupCallBack =
		[this](ResourceType state, uint8_t* data) {
		if (state == ResourceType::PALETTE_SET)
			SetPalette(data);
		if (state == ResourceType::LOOKUPTABLE_SET)
			SetColourLookupTable(data);
		};

	RegisterEvent(new Event<ResourceType, uint8_t*>(EventType::E_RESOURCE_CHANGE, m_SetPaletteLookupCallBack));

	m_TextureCallBack =
		[this](ResourceType state, uint32_t index, uint8_t* pixels, uint32_t width, uint32_t height) {
		SetTexture(state, index, pixels, width, height);
		};

	RegisterEvent(new Event<ResourceType, uint32_t, uint8_t*, uint32_t, uint32_t>(EventType::E_RESOURCE_CHANGE, m_TextureCallBack));

	m_PolyCallBack =
		[this](ResourceType state, RenderViews* views) {
		SetPolygons(state, views);
		};

	RegisterEvent(new Event<ResourceType, RenderViews*>(EventType::E_RESOURCE_CHANGE, m_PolyCallBack));

	m_ViewPortCallBack =
		[this](uint16_t posX, uint16_t posY, uint16_t width, uint16_t height) {
		m_gameViewPort = { posX, posY, width, height };
		};

	RegisterEvent(new Event<uint16_t, uint16_t, uint16_t, uint16_t>(EventType::E_VIEWPORT_CHANGE, m_ViewPortCallBack));

	m_BlurEffectCallBack =
		[this](bool active) {
		if (active && !m_blurActive)
			m_blurSeed = true;
		m_blurActive = active;
		};

	RegisterEvent(new Event<bool>(EventType::E_BLUR_EFFECT_CHANGE, m_BlurEffectCallBack));
}

VulkanPolygonRenderer::~VulkanPolygonRenderer()
{
	// UnregisterEvent also deletes the event
	for (IEvent* event : m_registeredEvents)
		EventDispatcher::I->UnregisterEvent(event);
	m_registeredEvents.clear();

	if (m_initialized)
		Shutdown();
}

void VulkanPolygonRenderer::RegisterEvent(IEvent* event)
{
	m_registeredEvents.push_back(event);
	EventDispatcher::I->RegisterEvent(event);
}

std::string VulkanPolygonRenderer::ShaderPath(const char* filename)
{
	return m_ShaderFolder + "/" + filename;
}

std::vector<char> VulkanPolygonRenderer::ReadFile(const std::string& path)
{
	std::ifstream file(path, std::ios::ate | std::ios::binary);
	if (!file.is_open())
		throw std::runtime_error("Failed to open shader file: " + path);

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	return buffer;
}

void VulkanPolygonRenderer::SetFrontFace(VkFrontFace frontFace)
{
	m_frontFace = frontFace;
	// Pipeline must be rebuilt for this to take effect if called after Init().
}

void VulkanPolygonRenderer::SetCullMode(VkCullModeFlags cullMode)
{
	m_cullMode = cullMode;
}

void VulkanPolygonRenderer::SetBilinearFiltering(bool enabled)
{
	m_bilinearFiltering = enabled;
}

void VulkanPolygonRenderer::SetAntiAliasing(int samples)
{
	m_requestedMsaaSamples = samples;
}

// The highest sample count up to the requested one that the device can render colour attachments with.
VkSampleCountFlagBits VulkanPolygonRenderer::PickMsaaSamples(int requested) const
{
	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(m_physicalDevice, &properties);
	const VkSampleCountFlags supported = properties.limits.framebufferColorSampleCounts;

	for (VkSampleCountFlagBits samples : { VK_SAMPLE_COUNT_8_BIT, VK_SAMPLE_COUNT_4_BIT, VK_SAMPLE_COUNT_2_BIT })
	{
		if ((int)samples <= requested && (supported & samples))
			return samples;
	}
	return VK_SAMPLE_COUNT_1_BIT;
}

bool VulkanPolygonRenderer::Init(SDL_Window* window, int windowWidth, int windowHeight)
{
	m_window = window;
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;

	vkb::InstanceBuilder instanceBuilder;
	auto instRet = instanceBuilder
		.set_app_name("remc2")
		.request_validation_layers(m_enableValidation)   // slow; --vulkan_validation turns them on
		.use_default_debug_messenger()
		.require_api_version(1, 2, 0)
		.build();

	if (!instRet)
		throw std::runtime_error("Failed to create Vulkan instance: " + instRet.error().message());

	m_vkbInstance = instRet.value();
	m_instance = m_vkbInstance.instance;
	m_debugMessenger = m_vkbInstance.debug_messenger;

	if (!SDL_Vulkan_CreateSurface(window, m_instance, &m_surface))
	{
		auto error = std::string(SDL_GetError());
		throw std::runtime_error("SDL_Vulkan_CreateSurface failed: " + error);
	}

	vkb::PhysicalDeviceSelector selector{ m_vkbInstance };
	auto physRet = selector
		.set_surface(m_surface)
		.set_minimum_version(1, 2)
		.select();

	if (!physRet)
		throw std::runtime_error("Failed to select Vulkan physical device: " + physRet.error().message());

	vkb::DeviceBuilder deviceBuilder{ physRet.value() };
	auto devRet = deviceBuilder.build();
	if (!devRet)
		throw std::runtime_error("Failed to create Vulkan device: " + devRet.error().message());

	m_vkbDevice = devRet.value();
	m_device = m_vkbDevice.device;
	m_physicalDevice = physRet.value().physical_device;
	m_msaaSamples = PickMsaaSamples(m_requestedMsaaSamples);

	auto gq = m_vkbDevice.get_queue(vkb::QueueType::graphics);
	if (!gq)
		throw std::runtime_error("Failed to get graphics queue: " + gq.error().message());
	m_graphicsQueue = gq.value();
	m_graphicsQueueFamily = m_vkbDevice.get_queue_index(vkb::QueueType::graphics).value();

	VmaAllocatorCreateInfo allocatorInfo{};
	allocatorInfo.physicalDevice = m_physicalDevice;
	allocatorInfo.device = m_device;
	allocatorInfo.instance = m_instance;
	allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_2;
	if (vmaCreateAllocator(&allocatorInfo, &m_allocator) != VK_SUCCESS)
		throw std::runtime_error("Failed to create VMA allocator");

	if (!CreateSwapchain(windowWidth, windowHeight))
		return false;
	if (!CreateRenderPass())
		return false;
	if (!CreateOffscreenRenderPass(VK_ATTACHMENT_LOAD_OP_CLEAR, VK_IMAGE_LAYOUT_UNDEFINED, m_offscreenRenderPass, m_msaaSamples))
		return false;
	// Blur history: the seed pass overwrites it, the blur pass blends onto what is already there.
	if (!CreateOffscreenRenderPass(VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_IMAGE_LAYOUT_UNDEFINED, m_blurSeedRenderPass))
		return false;
	if (!CreateOffscreenRenderPass(VK_ATTACHMENT_LOAD_OP_LOAD, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, m_blurRenderPass))
		return false;

	// Framebuffers depend on render pass + swapchain image views.
	m_framebuffers.resize(m_swapchainImageViews.size());
	for (size_t i = 0; i < m_swapchainImageViews.size(); i++)
	{
		VkImageView attachments[] = { m_swapchainImageViews[i] };
		VkFramebufferCreateInfo fbInfo{};
		fbInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		fbInfo.renderPass = m_renderPass;
		fbInfo.attachmentCount = 1;
		fbInfo.pAttachments = attachments;
		fbInfo.width = m_swapchainExtent.width;
		fbInfo.height = m_swapchainExtent.height;
		fbInfo.layers = 1;
		if (vkCreateFramebuffer(m_device, &fbInfo, nullptr, &m_framebuffers[i]) != VK_SUCCESS)
			throw std::runtime_error("Failed to create framebuffer");
	}

	if (!CreateDescriptorResources())
		return false;
	if (!CreatePaletteTexture())
		return false;
	if (!CreateBlendLutTexture())          // m_blendLutView, calls SetBlendLut -> UploadStagedImage
		return false;
	if (!CreateShadeLutTexture())          // m_shadeLutView, calls SetShadeLut -> UploadStagedImage
		return false;
	if (!CreateOverlayResources())
		return false;
	if (!CreatePipeline())
		return false;
	//if (!CreateWireframePipeline())
	//	return false;
	if (!CreateFrameData())
		return false;

	m_initialized = true;
	return true;
}

void VulkanPolygonRenderer::SetPolygons(ResourceType state, RenderViews* views)
{
	if (state == ResourceType::POLYGONS_UPDATED && views)
	{
		m_views.swap(views->lists);   // take them rather than copy them: the sender reuses the lists it gets back (the previous frame's)
		SetViewGrid(views->rows, views->columns);
		m_viewComposition = views->composition;
		m_darkLevel = views->darkLevel;
	}
	else if (state == ResourceType::POLYGONS_DISPOSED)
	{
		m_views.clear();
		SetViewGrid(1, 1);
		m_viewComposition = ViewComposition::Cells;
		m_darkLevel = false;
	}
}

// How many of the lists are drawn (each into its own image): one per cell, or the first one for every cell
size_t VulkanPolygonRenderer::GetFrameViewCount(size_t cellCount) const
{
	return m_views.size() == cellCount ? cellCount : (std::min)(m_views.size(), (size_t)1);
}

// The image of a view (its blur history while blurring)
VkDescriptorSet VulkanPolygonRenderer::GetViewImage(size_t view) const
{
	return m_blurActive ? m_blurTargets[view].descriptorSet : m_viewTargets[view].descriptorSet;
}

// Upscales the cell's view into it (the viewport and the scissor are set to the view port's area of the cell)
void VulkanPolygonRenderer::DrawCellViews(VkCommandBuffer commandBuffer, size_t cell, size_t view)
{
	const VkDescriptorSet viewPortImage = GetViewImage(view);
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_viewPortBlitPipeline);
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
		m_overlayPipelineLayout, 0, 1, &viewPortImage, 0, nullptr);
	vkCmdDraw(commandBuffer, 3, 1, 0, 0);
}

// The views' layout as the game sent it, kept to at most MaxViews() cells
void VulkanPolygonRenderer::SetViewGrid(int rows, int columns)
{
	const int maxViews = (std::max)(MaxViews(), 1);
	m_viewRows = (std::max)(rows, 1);
	m_viewColumns = (std::max)(columns, 1);
	while (m_viewRows * m_viewColumns > maxViews)
	{
		if (m_viewRows > m_viewColumns)
			m_viewRows--;
		else
			m_viewColumns--;
	}
}

void VulkanPolygonRenderer::SetTexture(ResourceType state, uint32_t index, uint8_t* pixels, uint32_t width, uint32_t height)
{
	if (state == ResourceType::TEXTURE_LOADED)
		UploadTexture(index, pixels, width, height);
	if (state == ResourceType::TEXTURE_DISPOSED)
		FreeTexture(index);
}

bool VulkanPolygonRenderer::CreateSwapchain(int width, int height)
{
	vkb::SwapchainBuilder swapchainBuilder{ m_physicalDevice, m_device, m_surface };
	auto swapRet = swapchainBuilder
		.set_desired_extent(width, height)
		.set_desired_format({ VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR })
		.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR) // vsync; use MAILBOX_KHR if you want uncapped
		.build();

	if (!swapRet)
		throw std::runtime_error("Failed to create swapchain: " + swapRet.error().message());

	m_vkbSwapchain = swapRet.value();
	m_swapchain = m_vkbSwapchain.swapchain;
	m_swapchainFormat = m_vkbSwapchain.image_format;
	m_swapchainExtent = m_vkbSwapchain.extent;
	m_swapchainImages = m_vkbSwapchain.get_images().value();
	m_swapchainImageViews = m_vkbSwapchain.get_image_views().value();
	return true;
}

void VulkanPolygonRenderer::DestroySwapchain()
{
	for (auto fb : m_framebuffers)
		vkDestroyFramebuffer(m_device, fb, nullptr);
	m_framebuffers.clear();

	m_vkbSwapchain.destroy_image_views(m_swapchainImageViews);
	m_swapchainImageViews.clear();
	m_swapchainImages.clear();

	vkb::destroy_swapchain(m_vkbSwapchain);
	m_swapchain = VK_NULL_HANDLE;
}

void VulkanPolygonRenderer::Resize(int windowWidth, int windowHeight)
{
	vkDeviceWaitIdle(m_device);

	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;

	DestroySwapchain();
	CreateSwapchain(windowWidth, windowHeight);

	m_framebuffers.resize(m_swapchainImageViews.size());
	for (size_t i = 0; i < m_swapchainImageViews.size(); i++)
	{
		VkImageView attachments[] = { m_swapchainImageViews[i] };
		VkFramebufferCreateInfo fbInfo{};
		fbInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		fbInfo.renderPass = m_renderPass;
		fbInfo.attachmentCount = 1;
		fbInfo.pAttachments = attachments;
		fbInfo.width = m_swapchainExtent.width;
		fbInfo.height = m_swapchainExtent.height;
		fbInfo.layers = 1;
		vkCreateFramebuffer(m_device, &fbInfo, nullptr, &m_framebuffers[i]);
	}
}

bool VulkanPolygonRenderer::CreateRenderPass()
{
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = m_swapchainFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorRef{};
	colorRef.attachment = 0;
	colorRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorRef;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo rpInfo{};
	rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	rpInfo.attachmentCount = 1;
	rpInfo.pAttachments = &colorAttachment;
	rpInfo.subpassCount = 1;
	rpInfo.pSubpasses = &subpass;
	rpInfo.dependencyCount = 1;
	rpInfo.pDependencies = &dependency;

	return vkCreateRenderPass(m_device, &rpInfo, nullptr, &m_renderPass) == VK_SUCCESS;
}

// The same attachment format as m_renderPass, but it ends in SHADER_READ_ONLY_OPTIMAL so the result can be
// sampled for the upscale. loadOp/initialLayout don't affect render pass compatibility, so the same pipelines
// work with every single-sampled variant. With samples > 1 the subpass draws into a multisampled attachment
// (0) and resolves it into the sampled one (1); m_pipeline is built for that pass, with the same sample count.
bool VulkanPolygonRenderer::CreateOffscreenRenderPass(VkAttachmentLoadOp loadOp, VkImageLayout initialLayout, VkRenderPass& renderPass,
	VkSampleCountFlagBits samples)
{
	const bool msaa = samples != VK_SAMPLE_COUNT_1_BIT;

	VkAttachmentDescription attachments[2]{};
	VkAttachmentDescription& colorAttachment = attachments[0];
	colorAttachment.format = m_swapchainFormat;
	colorAttachment.samples = samples;
	colorAttachment.loadOp = loadOp;
	colorAttachment.storeOp = msaa ? VK_ATTACHMENT_STORE_OP_DONT_CARE : VK_ATTACHMENT_STORE_OP_STORE;   // only the resolve is kept
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = msaa ? VK_IMAGE_LAYOUT_UNDEFINED : initialLayout;
	colorAttachment.finalLayout = msaa ? VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkAttachmentDescription& resolveAttachment = attachments[1];
	resolveAttachment.format = m_swapchainFormat;
	resolveAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	resolveAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;   // every pixel is overwritten by the resolve
	resolveAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	resolveAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	resolveAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	resolveAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	resolveAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkAttachmentReference colorRef{};
	colorRef.attachment = 0;
	colorRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference resolveRef{};
	resolveRef.attachment = 1;
	resolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorRef;
	subpass.pResolveAttachments = msaa ? &resolveRef : nullptr;

	VkSubpassDependency dependencies[2]{};
	// The previous frame's upscale must finish reading the image before we draw into it again.
	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[0].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	// The upscale samples the image after the polygons are written.
	dependencies[1].srcSubpass = 0;
	dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	VkRenderPassCreateInfo rpInfo{};
	rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	rpInfo.attachmentCount = msaa ? 2 : 1;
	rpInfo.pAttachments = attachments;
	rpInfo.subpassCount = 1;
	rpInfo.pSubpasses = &subpass;
	rpInfo.dependencyCount = 2;
	rpInfo.pDependencies = dependencies;

	return vkCreateRenderPass(m_device, &rpInfo, nullptr, &renderPass) == VK_SUCCESS;
}

bool VulkanPolygonRenderer::CreateOffscreenTarget(uint32_t width, uint32_t height, size_t viewCount)
{
	// The blur history is only ever drawn with the blur render passes, which are compatible with each other.
	if (m_msaaSamples != VK_SAMPLE_COUNT_1_BIT && !CreateMsaaImage(width, height))
		return false;

	if (!CreateColourTarget(width, height, m_offscreenRenderPass, m_reflectionTarget, m_msaaView))
		return false;

	m_viewTargets.resize(viewCount);
	m_blurTargets.resize(viewCount);
	for (size_t view = 0; view < viewCount; view++)
	{
		if (!CreateColourTarget(width, height, m_offscreenRenderPass, m_viewTargets[view], m_msaaView) ||
			!CreateColourTarget(width, height, m_blurRenderPass, m_blurTargets[view]))
			return false;
	}

	m_offscreenWidth = width;
	m_offscreenHeight = height;
	m_blurSeed = true;
	return true;
}

// The multisampled image is only drawn into and resolved within the render pass, so it never needs real memory
// on GPUs with lazily allocated (tile) memory.
bool VulkanPolygonRenderer::CreateMsaaImage(uint32_t width, uint32_t height)
{
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.format = m_swapchainFormat;
	imageInfo.extent = { width, height, 1 };
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.samples = m_msaaSamples;
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	VmaAllocationCreateInfo allocInfo{};
	allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
	allocInfo.preferredFlags = VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;

	if (vmaCreateImage(m_allocator, &imageInfo, &allocInfo, &m_msaaImage, &m_msaaAllocation, nullptr) != VK_SUCCESS)
		return false;

	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = m_msaaImage;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = imageInfo.format;
	viewInfo.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
	return vkCreateImageView(m_device, &viewInfo, nullptr, &m_msaaView) == VK_SUCCESS;
}

void VulkanPolygonRenderer::DestroyMsaaImage()
{
	if (m_msaaView)
		vkDestroyImageView(m_device, m_msaaView, nullptr);
	if (m_msaaImage)
		vmaDestroyImage(m_allocator, m_msaaImage, m_msaaAllocation);
	m_msaaView = VK_NULL_HANDLE;
	m_msaaImage = VK_NULL_HANDLE;
	m_msaaAllocation = VK_NULL_HANDLE;
}

bool VulkanPolygonRenderer::CreateColourTarget(uint32_t width, uint32_t height, VkRenderPass renderPass, ColourTarget& target,
	VkImageView msaaView)
{
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.format = m_swapchainFormat;
	imageInfo.extent = { width, height, 1 };
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	VmaAllocationCreateInfo allocInfo{};
	allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

	if (vmaCreateImage(m_allocator, &imageInfo, &allocInfo, &target.image, &target.allocation, nullptr) != VK_SUCCESS)
		return false;

	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = target.image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = imageInfo.format;
	viewInfo.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
	if (vkCreateImageView(m_device, &viewInfo, nullptr, &target.view) != VK_SUCCESS)
		return false;

	// With MSAA the render pass draws into msaaView (attachment 0) and resolves into this target (attachment 1).
	const VkImageView attachments[2] = { msaaView ? msaaView : target.view, target.view };

	VkFramebufferCreateInfo fbInfo{};
	fbInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	fbInfo.renderPass = renderPass;
	fbInfo.attachmentCount = msaaView ? 2 : 1;
	fbInfo.pAttachments = attachments;
	fbInfo.width = width;
	fbInfo.height = height;
	fbInfo.layers = 1;
	if (vkCreateFramebuffer(m_device, &fbInfo, nullptr, &target.framebuffer) != VK_SUCCESS)
		return false;

	// Sampled by the overlay shaders, so it uses the overlay's descriptor layout and nearest sampler.
	VkDescriptorSetAllocateInfo dsAllocInfo{};
	dsAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	dsAllocInfo.descriptorPool = m_descriptorPool;
	dsAllocInfo.descriptorSetCount = 1;
	dsAllocInfo.pSetLayouts = &m_overlayDescriptorSetLayout;
	if (vkAllocateDescriptorSets(m_device, &dsAllocInfo, &target.descriptorSet) != VK_SUCCESS)
		return false;

	VkDescriptorImageInfo imgInfo{};
	imgInfo.sampler = m_overlaySampler;
	imgInfo.imageView = target.view;
	imgInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkWriteDescriptorSet write{};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.dstSet = target.descriptorSet;
	write.dstBinding = 0;
	write.descriptorCount = 1;
	write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	write.pImageInfo = &imgInfo;
	vkUpdateDescriptorSets(m_device, 1, &write, 0, nullptr);

	return true;
}

// Caller must make sure the GPU is no longer using the target (vkDeviceWaitIdle).
void VulkanPolygonRenderer::DestroyOffscreenTarget()
{
	for (ColourTarget& target : m_viewTargets)
		DestroyColourTarget(target);
	for (ColourTarget& target : m_blurTargets)
		DestroyColourTarget(target);
	m_viewTargets.clear();
	m_blurTargets.clear();
	DestroyColourTarget(m_reflectionTarget);
	DestroyMsaaImage();
	m_offscreenWidth = 0;
	m_offscreenHeight = 0;
}

void VulkanPolygonRenderer::DestroyColourTarget(ColourTarget& target)
{
	if (target.descriptorSet)
		vkFreeDescriptorSets(m_device, m_descriptorPool, 1, &target.descriptorSet);
	if (target.framebuffer)
		vkDestroyFramebuffer(m_device, target.framebuffer, nullptr);
	if (target.view)
		vkDestroyImageView(m_device, target.view, nullptr);
	if (target.image)
		vmaDestroyImage(m_allocator, target.image, target.allocation);

	target = ColourTarget{};
}

bool VulkanPolygonRenderer::CreateDescriptorResources()
{
	// binding 0: palette (256x1 RGBA8)
	// binding 1: current indexed texture (R8_UINT)
	// binding 2: blend LUT (256x256 R8_UINT), tint modes 4/5
	// binding 3: shade LUT (256x64 R8_UINT), lighting rows
	constexpr uint32_t kBindingCount = 4;

	VkDescriptorSetLayoutBinding bindings[kBindingCount]{};
	for (uint32_t i = 0; i < kBindingCount; i++)
	{
		bindings[i].binding = i;
		bindings[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		bindings[i].descriptorCount = 1;
		bindings[i].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	}

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = kBindingCount;
	layoutInfo.pBindings = bindings;
	if (vkCreateDescriptorSetLayout(m_device, &layoutInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS)
		return false;

	// Each texture set uses kBindingCount combined image samplers. The HUD, colour target (offscreen,
	// reflection, blur) and no-reflection sets allocated later from this pool use a few more: leave headroom.
	// Live textures: up to 504 sprite IDs (m_spriteFrameCount) + terrain atlas + sky.
	// Plus the offscreen and blur targets of every view.
	const uint32_t kMaxTextures = 512;
	const uint32_t kViewTargetSets = 2 * (uint32_t)(std::max)(MaxViews(), 1);
	VkDescriptorPoolSize poolSize{};
	poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSize.descriptorCount = kMaxTextures * kBindingCount + kViewTargetSets + 16;

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	poolInfo.maxSets = kMaxTextures + kViewTargetSets + 16;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;
	if (vkCreateDescriptorPool(m_device, &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS)
		return false;

	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_NEAREST;
	samplerInfo.minFilter = VK_FILTER_NEAREST;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.unnormalizedCoordinates = VK_TRUE;
	if (vkCreateSampler(m_device, &samplerInfo, nullptr, &m_nearestSampler) != VK_SUCCESS)
		return false;

	return true;
}

bool VulkanPolygonRenderer::CreatePaletteTexture()
{
	VkImageCreateInfo imgInfo{};
	imgInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imgInfo.imageType = VK_IMAGE_TYPE_2D;
	imgInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
	imgInfo.extent = { 256, 1, 1 };
	imgInfo.mipLevels = 1;
	imgInfo.arrayLayers = 1;
	imgInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imgInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imgInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	imgInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	VmaAllocationCreateInfo allocInfo{};
	allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
	if (vmaCreateImage(m_allocator, &imgInfo, &allocInfo, &m_paletteImage, &m_paletteAllocation, nullptr) != VK_SUCCESS)
		return false;

	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = m_paletteImage;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
	viewInfo.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
	if (vkCreateImageView(m_device, &viewInfo, nullptr, &m_paletteView) != VK_SUCCESS)
		return false;

	// Persistently-mapped staging buffer we re-upload from on every SetPalette().
	VkBufferCreateInfo bufInfo{};
	bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufInfo.size = 256 * 4;
	bufInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

	VmaAllocationCreateInfo stagingAllocInfo{};
	stagingAllocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;
	stagingAllocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

	VmaAllocationInfo outInfo{};
	if (vmaCreateBuffer(m_allocator, &bufInfo, &stagingAllocInfo, &m_paletteStagingBuffer, &m_paletteStagingAllocation, &outInfo) != VK_SUCCESS)
		return false;
	m_paletteStagingMapped = outInfo.pMappedData;

	return true;
}

void VulkanPolygonRenderer::SetPalette(const uint8_t* rgb768)
{
	uint8_t* dst = static_cast<uint8_t*>(m_paletteStagingMapped);
	for (int i = 0; i < 256; i++)
	{
		for (int c = 0; c < 3; c++)
		{
			uint8_t v = rgb768[i * 3 + c] & 0x3F;          // 6-bit VGA value
			dst[i * 4 + c] = (uint8_t)((v << 2) | (v >> 4)); // 63 -> 255, not 252
		}
		dst[i * 4 + 3] = 255;
	}

	if (m_clearPaletteIndex >= 0 && m_clearPaletteIndex < 256)
	{
		const uint8_t* p = dst + m_clearPaletteIndex * 4;     // already expanded to 8-bit RGBA
		m_clearColor = { { p[0] / 255.0f, p[1] / 255.0f, p[2] / 255.0f, 1.0f } };
		m_clearColorSet = true;                               // once; remove this line to follow palette changes
	}

	// One-shot copy + layout transition. Simple and correct; if you call
	// SetPalette() every frame, consider batching this into your normal
	// frame command buffer instead of a dedicated submit.
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = m_graphicsQueueFamily;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
	VkCommandPool pool;
	vkCreateCommandPool(m_device, &poolInfo, nullptr, &pool);

	VkCommandBufferAllocateInfo cbInfo{};
	cbInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cbInfo.commandPool = pool;
	cbInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cbInfo.commandBufferCount = 1;
	VkCommandBuffer cmd;
	vkAllocateCommandBuffers(m_device, &cbInfo, &cmd);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	vkBeginCommandBuffer(cmd, &beginInfo);

	VkImageMemoryBarrier toTransfer{};
	toTransfer.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	toTransfer.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	toTransfer.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	toTransfer.image = m_paletteImage;
	toTransfer.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
	toTransfer.srcAccessMask = 0;
	toTransfer.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	vkCmdPipelineBarrier(cmd,
		VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,     // wait for earlier frames' palette reads
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		0, 0, nullptr, 0, nullptr, 1, &toTransfer);

	VkBufferImageCopy copy{};
	copy.imageSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
	copy.imageExtent = { 256, 1, 1 };
	vkCmdCopyBufferToImage(cmd, m_paletteStagingBuffer, m_paletteImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);

	VkImageMemoryBarrier toShaderRead = toTransfer;
	toShaderRead.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	toShaderRead.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	toShaderRead.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	toShaderRead.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
		0, 0, nullptr, 0, nullptr, 1, &toShaderRead);

	vkEndCommandBuffer(cmd);

	VkSubmitInfo submit{};
	submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit.commandBufferCount = 1;
	submit.pCommandBuffers = &cmd;
	vkQueueSubmit(m_graphicsQueue, 1, &submit, VK_NULL_HANDLE);
	vkQueueWaitIdle(m_graphicsQueue);

	vkDestroyCommandPool(m_device, pool, nullptr);
}

void VulkanPolygonRenderer::SetClearColourIndex(uint8_t paletteIndex) 
{
	m_clearPaletteIndex = paletteIndex;
	m_clearColorSet = false; 
}

bool VulkanPolygonRenderer::CreateShadeLutTexture()
{
	VkImageCreateInfo img{};
	img.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	img.imageType = VK_IMAGE_TYPE_2D;
	img.format = VK_FORMAT_R8_UINT;
	img.extent = { 256, kShadeRows, 1 };
	img.mipLevels = 1; img.arrayLayers = 1;
	img.samples = VK_SAMPLE_COUNT_1_BIT;
	img.tiling = VK_IMAGE_TILING_OPTIMAL;
	img.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	img.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	VmaAllocationCreateInfo ai{};
	ai.usage = VMA_MEMORY_USAGE_GPU_ONLY;
	if (vmaCreateImage(m_allocator, &img, &ai, &m_shadeLutImage, &m_shadeLutAllocation, nullptr) != VK_SUCCESS)
		return false;

	VkImageViewCreateInfo view{};
	view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	view.image = m_shadeLutImage;
	view.viewType = VK_IMAGE_VIEW_TYPE_2D;
	view.format = VK_FORMAT_R8_UINT;
	view.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
	if (vkCreateImageView(m_device, &view, nullptr, &m_shadeLutView) != VK_SUCCESS)
		return false;

	VkBufferCreateInfo buf{};
	buf.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buf.size = 256 * kShadeRows;
	buf.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	VmaAllocationCreateInfo sai{};
	sai.usage = VMA_MEMORY_USAGE_CPU_ONLY;
	sai.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;
	VmaAllocationInfo out{};
	if (vmaCreateBuffer(m_allocator, &buf, &sai, &m_shadeLutStaging, &m_shadeLutStagingAllocation, &out) != VK_SUCCESS)
		return false;
	m_shadeLutStagingMapped = out.pMappedData;

	// Safe default until the game's table arrives: every row is the identity (no shading)
	std::vector<uint8_t> identity(256 * kShadeRows);
	for (uint32_t r = 0; r < kShadeRows; r++)
		for (uint32_t x = 0; x < 256; x++)
			identity[r * 256 + x] = (uint8_t)x;
	SetShadeLut(identity.data());
	return true;
}

bool VulkanPolygonRenderer::CreateBlendLutTexture()
{
	VkImageCreateInfo img{};
	img.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	img.imageType = VK_IMAGE_TYPE_2D;
	img.format = VK_FORMAT_R8_UINT;
	img.extent = { 256, 256, 1 };
	img.mipLevels = 1; img.arrayLayers = 1;
	img.samples = VK_SAMPLE_COUNT_1_BIT;
	img.tiling = VK_IMAGE_TILING_OPTIMAL;
	img.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	img.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	VmaAllocationCreateInfo ai{};
	ai.usage = VMA_MEMORY_USAGE_GPU_ONLY;
	if (vmaCreateImage(m_allocator, &img, &ai, &m_blendLutImage, &m_blendLutAllocation, nullptr) != VK_SUCCESS)
		return false;

	VkImageViewCreateInfo view{};
	view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	view.image = m_blendLutImage;
	view.viewType = VK_IMAGE_VIEW_TYPE_2D;
	view.format = VK_FORMAT_R8_UINT;
	view.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
	if (vkCreateImageView(m_device, &view, nullptr, &m_blendLutView) != VK_SUCCESS)
		return false;

	VkBufferCreateInfo buf{};
	buf.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buf.size = 256 * 256;
	buf.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	VmaAllocationCreateInfo sai{};
	sai.usage = VMA_MEMORY_USAGE_CPU_ONLY;
	sai.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;
	VmaAllocationInfo out{};
	if (vmaCreateBuffer(m_allocator, &buf, &sai, &m_blendLutStaging, &m_blendLutStagingAllocation, &out) != VK_SUCCESS)
		return false;
	m_blendLutStagingMapped = out.pMappedData;

	// Puts the image in SHADER_READ_ONLY so descriptors that reference it are valid
	std::vector<uint8_t> zeros(256 * 256, 0);
	SetBlendLut(zeros.data());
	return true;
}

void VulkanPolygonRenderer::SetBlendLut(const uint8_t* lut65536)
{
	std::memcpy(m_blendLutStagingMapped, lut65536, 256 * 256);
	UploadStagedImage(m_blendLutImage, m_blendLutStaging, 256, 256);
}

void VulkanPolygonRenderer::SetShadeLut(const uint8_t* table)
{
	std::memcpy(m_shadeLutStagingMapped, table, 256 * kShadeRows);
	UploadStagedImage(m_shadeLutImage, m_shadeLutStaging, 256, kShadeRows);

	// Optional: row 0 is where the fog ends. Once LITROW shows it is the haze colour, use it for the clear colour.
	// m_clearPaletteIndex = table[1];
	// RefreshClearColour();
}

// The same one-shot copy as SetBlendLut, with the image and size as parameters (SetBlendLut can call this too)
void VulkanPolygonRenderer::UploadStagedImage(VkImage image, VkBuffer staging, uint32_t width, uint32_t height)
{
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = m_graphicsQueueFamily;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
	VkCommandPool pool;
	vkCreateCommandPool(m_device, &poolInfo, nullptr, &pool);

	VkCommandBufferAllocateInfo cbInfo{};
	cbInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cbInfo.commandPool = pool;
	cbInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cbInfo.commandBufferCount = 1;
	VkCommandBuffer cmd;
	vkAllocateCommandBuffers(m_device, &cbInfo, &cmd);

	VkCommandBufferBeginInfo begin{};
	begin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	vkBeginCommandBuffer(cmd, &begin);

	VkImageMemoryBarrier toTransfer{};
	toTransfer.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	toTransfer.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	toTransfer.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	toTransfer.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	toTransfer.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	toTransfer.image = image;
	toTransfer.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
	toTransfer.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
		0, 0, nullptr, 0, nullptr, 1, &toTransfer);

	VkBufferImageCopy copy{};
	copy.imageSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
	copy.imageExtent = { width, height, 1 };
	vkCmdCopyBufferToImage(cmd, staging, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);

	VkImageMemoryBarrier toRead = toTransfer;
	toRead.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	toRead.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	toRead.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	toRead.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
		0, 0, nullptr, 0, nullptr, 1, &toRead);

	vkEndCommandBuffer(cmd);

	VkSubmitInfo submit{};
	submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit.commandBufferCount = 1;
	submit.pCommandBuffers = &cmd;
	vkQueueSubmit(m_graphicsQueue, 1, &submit, VK_NULL_HANDLE);
	vkQueueWaitIdle(m_graphicsQueue);
	vkDestroyCommandPool(m_device, pool, nullptr);
}

void VulkanPolygonRenderer::RefreshClearColour()
{
	if (m_clearPaletteIndex < 0 || !m_paletteStagingMapped)
		return;
	const uint8_t* p = static_cast<const uint8_t*>(m_paletteStagingMapped) + m_clearPaletteIndex * 4;
	m_clearColor = { { p[0] / 255.0f, p[1] / 255.0f, p[2] / 255.0f, 1.0f } };
}

VkDescriptorSet VulkanPolygonRenderer::AllocateTextureDescriptorSet(VkImageView view)
{
	constexpr uint32_t kBindingCount = 4;

	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = m_descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &m_descriptorSetLayout;

	VkDescriptorSet set;
	if (vkAllocateDescriptorSets(m_device, &allocInfo, &set) != VK_SUCCESS)
		throw std::runtime_error("Failed to allocate descriptor set (out of pool space? raise kMaxTextures)");

	VkDescriptorImageInfo paletteInfo{};
	paletteInfo.sampler = m_nearestSampler;
	paletteInfo.imageView = m_paletteView;
	paletteInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkDescriptorImageInfo texInfo{};
	texInfo.sampler = m_nearestSampler;
	texInfo.imageView = view;
	texInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkDescriptorImageInfo lutInfo{};
	lutInfo.sampler = m_nearestSampler;
	lutInfo.imageView = m_blendLutView;
	lutInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkDescriptorImageInfo shadeInfo{};
	shadeInfo.sampler = m_nearestSampler;
	shadeInfo.imageView = m_shadeLutView;
	shadeInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkWriteDescriptorSet writes[kBindingCount]{};
	const VkDescriptorImageInfo* infos[kBindingCount] = { &paletteInfo, &texInfo, &lutInfo, &shadeInfo };
	for (uint32_t i = 0; i < kBindingCount; i++)
	{
		writes[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writes[i].dstSet = set;
		writes[i].dstBinding = i;
		writes[i].descriptorCount = 1;
		writes[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		writes[i].pImageInfo = infos[i];
	}

	vkUpdateDescriptorSets(m_device, kBindingCount, writes, 0, nullptr);
	return set;
}

void VulkanPolygonRenderer::SetColourLookupTable(uint8_t* colourLookupTable)
{
	std::vector<uint8_t> lut(65536);
	for (int i = 0; i < 65536; i++)
		lut[i] = colourLookupTable[COLOUR_BLEND_LOOKUP_OFFSET + i];
	SetBlendLut(lut.data());

	SetShadeLut(colourLookupTable);   // rows 0..63 sit at the start of the table, as in DrawTriangleRow
}

uint32_t VulkanPolygonRenderer::UploadTexture(uint32_t index, const uint8_t* indexedPixels, uint32_t width, uint32_t height)
{
	FreeTexture(index);

	VkDeviceSize size = (VkDeviceSize)width * height;

	VkBufferCreateInfo stagingInfo{};
	stagingInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	stagingInfo.size = size;
	stagingInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

	VmaAllocationCreateInfo stagingAllocInfo{};
	stagingAllocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;
	stagingAllocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

	VkBuffer stagingBuffer;
	VmaAllocation stagingAllocation;
	VmaAllocationInfo stagingOut{};
	vmaCreateBuffer(m_allocator, &stagingInfo, &stagingAllocInfo, &stagingBuffer, &stagingAllocation, &stagingOut);
	std::memcpy(stagingOut.pMappedData, indexedPixels, (size_t)size);

	Texture tex{};
	tex.width = width;
	tex.height = height;

	VkImageCreateInfo imgInfo{};
	imgInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imgInfo.imageType = VK_IMAGE_TYPE_2D;
	imgInfo.format = VK_FORMAT_R8_UINT; // raw palette index, resolved in the fragment shader
	imgInfo.extent = { width, height, 1 };
	imgInfo.mipLevels = 1;
	imgInfo.arrayLayers = 1;
	imgInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imgInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imgInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	imgInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	VmaAllocationCreateInfo allocInfo{};
	allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
	vmaCreateImage(m_allocator, &imgInfo, &allocInfo, &tex.image, &tex.allocation, nullptr);

	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = tex.image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = VK_FORMAT_R8_UINT;
	viewInfo.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
	vkCreateImageView(m_device, &viewInfo, nullptr, &tex.view);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = m_graphicsQueueFamily;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
	VkCommandPool pool;
	vkCreateCommandPool(m_device, &poolInfo, nullptr, &pool);

	VkCommandBufferAllocateInfo cbInfo{};
	cbInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cbInfo.commandPool = pool;
	cbInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cbInfo.commandBufferCount = 1;
	VkCommandBuffer cmd;
	vkAllocateCommandBuffers(m_device, &cbInfo, &cmd);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	vkBeginCommandBuffer(cmd, &beginInfo);

	VkImageMemoryBarrier toTransfer{};
	toTransfer.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	toTransfer.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	toTransfer.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	toTransfer.image = tex.image;
	toTransfer.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
	toTransfer.srcAccessMask = 0;
	toTransfer.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
		0, 0, nullptr, 0, nullptr, 1, &toTransfer);

	VkBufferImageCopy copy{};
	copy.imageSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
	copy.imageExtent = { width, height, 1 };
	vkCmdCopyBufferToImage(cmd, stagingBuffer, tex.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);

	VkImageMemoryBarrier toShaderRead = toTransfer;
	toShaderRead.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	toShaderRead.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	toShaderRead.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	toShaderRead.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
		0, 0, nullptr, 0, nullptr, 1, &toShaderRead);

	vkEndCommandBuffer(cmd);

	VkSubmitInfo submit{};
	submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit.commandBufferCount = 1;
	submit.pCommandBuffers = &cmd;
	vkQueueSubmit(m_graphicsQueue, 1, &submit, VK_NULL_HANDLE);
	vkQueueWaitIdle(m_graphicsQueue);

	vkDestroyCommandPool(m_device, pool, nullptr);
	vmaDestroyBuffer(m_allocator, stagingBuffer, stagingAllocation);

	tex.descriptorSet = AllocateTextureDescriptorSet(tex.view);

	m_textures[index] = tex;
	return index;
}

void VulkanPolygonRenderer::FreeTexture(uint32_t textureId)
{
	auto it = m_textures.find(textureId);
	if (it == m_textures.end())
		return;

	vkDeviceWaitIdle(m_device); // simplest-correct; batch frees if this shows up in profiling
	vkFreeDescriptorSets(m_device, m_descriptorPool, 1, &it->second.descriptorSet);
	vkDestroyImageView(m_device, it->second.view, nullptr);
	vmaDestroyImage(m_allocator, it->second.image, it->second.allocation);
	m_textures.erase(it);
}

bool VulkanPolygonRenderer::CreateOverlayResources()
{
	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_NEAREST; // UI/HUD pixels - nearest avoids blurring text
	samplerInfo.minFilter = VK_FILTER_NEAREST;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	if (vkCreateSampler(m_device, &samplerInfo, nullptr, &m_overlaySampler) != VK_SUCCESS)
		return false;

	// --- overlay descriptor set layout (single combined image sampler), for the viewport blit and the blur ---
	VkDescriptorSetLayoutBinding binding{};
	binding.binding = 0;
	binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	binding.descriptorCount = 1;
	binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutCreateInfo setLayoutInfo{};
	setLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	setLayoutInfo.bindingCount = 1;
	setLayoutInfo.pBindings = &binding;
	if (vkCreateDescriptorSetLayout(m_device, &setLayoutInfo, nullptr, &m_overlayDescriptorSetLayout) != VK_SUCCESS)
		return false;

	// --- HUD palette, re-uploaded with the HUD every frame ---
	VkImageCreateInfo paletteInfo{};
	paletteInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	paletteInfo.imageType = VK_IMAGE_TYPE_2D;
	paletteInfo.format = VK_FORMAT_R8G8B8A8_UNORM; // same byte order as SDL_Color
	paletteInfo.extent = { 256, 1, 1 };
	paletteInfo.mipLevels = 1;
	paletteInfo.arrayLayers = 1;
	paletteInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	paletteInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	paletteInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	paletteInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	VmaAllocationCreateInfo allocInfo{};
	allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

	if (vmaCreateImage(m_allocator, &paletteInfo, &allocInfo, &m_hudPaletteImage, &m_hudPaletteAlloc, nullptr) != VK_SUCCESS)
		return false;

	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = m_hudPaletteImage;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = paletteInfo.format;
	viewInfo.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
	if (vkCreateImageView(m_device, &viewInfo, nullptr, &m_hudPaletteView) != VK_SUCCESS)
		return false;

	// --- HUD descriptor set: binding 0 = index/alpha image (written by EnsureHudImage), binding 1 = palette ---
	VkDescriptorSetLayoutBinding hudBindings[2]{};
	for (uint32_t i = 0; i < 2; i++)
	{
		hudBindings[i].binding = i;
		hudBindings[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		hudBindings[i].descriptorCount = 1;
		hudBindings[i].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	}

	VkDescriptorSetLayoutCreateInfo hudLayoutInfo{};
	hudLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	hudLayoutInfo.bindingCount = 2;
	hudLayoutInfo.pBindings = hudBindings;
	if (vkCreateDescriptorSetLayout(m_device, &hudLayoutInfo, nullptr, &m_hudDescriptorSetLayout) != VK_SUCCESS)
		return false;

	VkDescriptorSetAllocateInfo dsAllocInfo{};
	dsAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	dsAllocInfo.descriptorPool = m_descriptorPool;
	dsAllocInfo.descriptorSetCount = 1;
	dsAllocInfo.pSetLayouts = &m_hudDescriptorSetLayout;
	if (vkAllocateDescriptorSets(m_device, &dsAllocInfo, &m_hudDescriptorSet) != VK_SUCCESS)
		return false;

	VkDescriptorImageInfo paletteImgInfo{};
	paletteImgInfo.sampler = m_overlaySampler;
	paletteImgInfo.imageView = m_hudPaletteView;
	paletteImgInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkWriteDescriptorSet write{};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.dstSet = m_hudDescriptorSet;
	write.dstBinding = 1;
	write.descriptorCount = 1;
	write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	write.pImageInfo = &paletteImgInfo;
	vkUpdateDescriptorSets(m_device, 1, &write, 0, nullptr);

	// --- polygon.frag set 1 while there is no reflection to read: any image in SHADER_READ_ONLY will do ---
	dsAllocInfo.pSetLayouts = &m_overlayDescriptorSetLayout;
	if (vkAllocateDescriptorSets(m_device, &dsAllocInfo, &m_noReflectionSet) != VK_SUCCESS)
		return false;

	VkDescriptorImageInfo noReflectionInfo{};
	noReflectionInfo.sampler = m_overlaySampler;
	noReflectionInfo.imageView = m_paletteView;
	noReflectionInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	write.dstSet = m_noReflectionSet;
	write.dstBinding = 0;
	write.pImageInfo = &noReflectionInfo;
	vkUpdateDescriptorSets(m_device, 1, &write, 0, nullptr);

	// hud.frag push constants: ivec2 size, int hasAlpha
	VkPushConstantRange pushRange{};
	pushRange.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	pushRange.offset = 0;
	pushRange.size = sizeof(int32_t) * 3;

	VkPipelineLayoutCreateInfo hudPipelineLayoutInfo{};
	hudPipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	hudPipelineLayoutInfo.setLayoutCount = 1;
	hudPipelineLayoutInfo.pSetLayouts = &m_hudDescriptorSetLayout;
	hudPipelineLayoutInfo.pushConstantRangeCount = 1;
	hudPipelineLayoutInfo.pPushConstantRanges = &pushRange;
	if (vkCreatePipelineLayout(m_device, &hudPipelineLayoutInfo, nullptr, &m_hudPipelineLayout) != VK_SUCCESS)
		return false;

	// Alpha-blended for the HUD overlay; opaque for upscaling the offscreen polygon image,
	// since the polygon pipeline leaves alpha < 1 wherever transparent polygons were drawn.
	return CreateOverlayPipeline(OverlayBlend::Alpha, m_hudPipeline, "hud.frag.spv", m_hudPipelineLayout) &&
		CreateOverlayPipeline(OverlayBlend::Opaque, m_viewPortBlitPipeline) &&
		CreateOverlayPipeline(OverlayBlend::Opaque, m_reflectionCopyPipeline, "overlay.frag.spv", VK_NULL_HANDLE,
			m_offscreenRenderPass, m_msaaSamples) &&
		CreateOverlayPipeline(OverlayBlend::Constant, m_blurPipeline) &&
		CreateExtraResources();
}

bool VulkanPolygonRenderer::CreateExtraResources()
{
	return true;
}

// ── Cell hooks: the game's grid of cells, in the window ──────────────────────────────────────────────────────
VulkanPolygonRenderer::FrameCells VulkanPolygonRenderer::GetFrameCells(const SDL_Rect& srcRect, const SDL_Rect& destRect)
{
	FrameCells cells;
	cells.count = (size_t)m_viewRows * m_viewColumns;
	cells.rect = destRect;
	cells.extent = m_swapchainExtent;
	return cells;
}

// rows x columns cells over cells.rect: cell 0 bottom left, then left to right and bottom to top
SDL_Rect VulkanPolygonRenderer::GetCellRect(size_t cell, const FrameCells& cells, const SDL_Rect& srcRect) const
{
	const int columns = m_viewColumns;
	const int rows = m_viewRows;
	const SDL_Rect& area = cells.rect;
	const int column = (int)(cell % columns);
	const int row = rows - 1 - (int)(cell / columns);   // from the top of the window
	const int x0 = area.x + area.w * column / columns;
	const int y0 = area.y + area.h * row / rows;
	const int x1 = area.x + area.w * (column + 1) / columns;
	const int y1 = area.y + area.h * (row + 1) / rows;
	return SDL_Rect{ x0, y0, x1 - x0, y1 - y0 };
}

void VulkanPolygonRenderer::BeginCellsRenderPass(VkCommandBuffer commandBuffer, const VkClearValue& clearColor)
{
	BeginWindowRenderPass(commandBuffer, clearColor);
}

void VulkanPolygonRenderer::EndCellsRenderPass(VkCommandBuffer commandBuffer, const VkClearValue& clearColor)
{
}

// The window's (swapchain image's) render pass: EndFrame ends it
void VulkanPolygonRenderer::BeginWindowRenderPass(VkCommandBuffer commandBuffer, const VkClearValue& clearColor)
{
	VkRenderPassBeginInfo rpBegin{};
	rpBegin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	rpBegin.renderPass = m_renderPass;
	rpBegin.framebuffer = m_framebuffers[m_currentImageIndex];
	rpBegin.renderArea.extent = m_swapchainExtent;
	rpBegin.clearValueCount = 1;
	rpBegin.pClearValues = &clearColor;
	vkCmdBeginRenderPass(commandBuffer, &rpBegin, VK_SUBPASS_CONTENTS_INLINE);
}

// (Re)creates the HUD index/alpha image and its staging buffer when the game resolution changes.
bool VulkanPolygonRenderer::EnsureHudImage(uint32_t width, uint32_t height)
{
	if (m_hudImage != VK_NULL_HANDLE && width == m_hudWidth && height == m_hudHeight)
		return true;

	// The other frame in flight may still be reading the old image or staging slot.
	vkDeviceWaitIdle(m_device);
	DestroyHudImage();

	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.format = VK_FORMAT_R8_UINT;
	imageInfo.extent = { width, height, 1 };
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 2;   // 0 = palette index, 1 = alpha
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	VmaAllocationCreateInfo allocInfo{};
	allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

	if (vmaCreateImage(m_allocator, &imageInfo, &allocInfo, &m_hudImage, &m_hudImageAlloc, nullptr) != VK_SUCCESS)
		return false;

	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = m_hudImage;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
	viewInfo.format = imageInfo.format;
	viewInfo.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 2 };
	if (vkCreateImageView(m_device, &viewInfo, nullptr, &m_hudImageView) != VK_SUCCESS)
		return false;

	// --- persistently-mapped staging buffer; each slot keeps the palette copy 4-byte aligned ---
	m_hudStagingSlotSize = (kHudPaletteBytes + 2 * (VkDeviceSize)width * height + 3) & ~(VkDeviceSize)3;
	VkBufferCreateInfo stagingInfo{};
	stagingInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	stagingInfo.size = m_hudStagingSlotSize * kFramesInFlight;
	stagingInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

	VmaAllocationCreateInfo stagingAllocInfo{};
	stagingAllocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
	stagingAllocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

	VmaAllocationInfo resultInfo{};
	if (vmaCreateBuffer(m_allocator, &stagingInfo, &stagingAllocInfo, &m_hudStagingBuffer, &m_hudStagingAlloc, &resultInfo) != VK_SUCCESS)
		return false;
	m_hudStagingMapped = resultInfo.pMappedData;

	VkDescriptorImageInfo imgInfo{};
	imgInfo.sampler = m_overlaySampler;
	imgInfo.imageView = m_hudImageView;
	imgInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkWriteDescriptorSet write{};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.dstSet = m_hudDescriptorSet;
	write.dstBinding = 0;
	write.descriptorCount = 1;
	write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	write.pImageInfo = &imgInfo;
	vkUpdateDescriptorSets(m_device, 1, &write, 0, nullptr);

	m_hudWidth = width;
	m_hudHeight = height;
	return true;
}

void VulkanPolygonRenderer::DestroyHudImage()
{
	if (m_hudStagingBuffer != VK_NULL_HANDLE)
		vmaDestroyBuffer(m_allocator, m_hudStagingBuffer, m_hudStagingAlloc);
	if (m_hudImageView != VK_NULL_HANDLE)
		vkDestroyImageView(m_device, m_hudImageView, nullptr);
	if (m_hudImage != VK_NULL_HANDLE)
		vmaDestroyImage(m_allocator, m_hudImage, m_hudImageAlloc);

	m_hudStagingBuffer = VK_NULL_HANDLE;
	m_hudStagingAlloc = VK_NULL_HANDLE;
	m_hudStagingMapped = nullptr;
	m_hudImageView = VK_NULL_HANDLE;
	m_hudImage = VK_NULL_HANDLE;
	m_hudImageAlloc = VK_NULL_HANDLE;
	m_hudWidth = 0;
	m_hudHeight = 0;
}

bool VulkanPolygonRenderer::CreateWireframePipeline()
{
	auto vertCode = ReadFile(ShaderPath("polygon.vert.spv"));
	auto fragCode = ReadFile(ShaderPath("wireframe.frag.spv")); // NEW shader, no sampler
	VkShaderModule vertModule = CreateShaderModule(m_device, vertCode);
	VkShaderModule fragModule = CreateShaderModule(m_device, fragCode);

	if (vertModule == VK_NULL_HANDLE || fragModule == VK_NULL_HANDLE)
		return false;

	VkPipelineShaderStageCreateInfo vertStage{};
	vertStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertStage.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertStage.module = vertModule;
	vertStage.pName = "main";

	VkPipelineShaderStageCreateInfo fragStage{};
	fragStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragStage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragStage.module = fragModule;
	fragStage.pName = "main";

	VkPipelineShaderStageCreateInfo stages[] = { vertStage, fragStage };

	VkVertexInputBindingDescription binding{};
	binding.binding = 0;
	binding.stride = sizeof(HWVertex);
	binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	// Keep all 3 attributes bound even though the wireframe fragment shader
	// only consumes brightness - the vertex shader still writes uv out, and
	// vertex input layout must match the shared Vertex struct/vertex shader.
	VkVertexInputAttributeDescription attrs[3]{};
	attrs[0] = { 0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(HWVertex, X) };
	attrs[1] = { 1, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(HWVertex, U) };
	attrs[2] = { 2, 0, VK_FORMAT_R32_SFLOAT, offsetof(HWVertex, Shade) };

	VkPipelineVertexInputStateCreateInfo vertexInput{};
	vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInput.vertexBindingDescriptionCount = 1;
	vertexInput.pVertexBindingDescriptions = &binding;
	vertexInput.vertexAttributeDescriptionCount = 3;
	vertexInput.pVertexAttributeDescriptions = attrs;

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;

	VkPipelineRasterizationStateCreateInfo raster{};
	raster.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	raster.polygonMode = VK_POLYGON_MODE_LINE;
	raster.cullMode = VK_CULL_MODE_NONE;
	raster.frontFace = m_frontFace;
	raster.lineWidth = 1.0f;

	VkPipelineMultisampleStateCreateInfo multisample{};
	multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisample.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineColorBlendAttachmentState blendAttachment{};
	blendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	blendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo blend{};
	blend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	blend.attachmentCount = 1;
	blend.pAttachments = &blendAttachment;

	VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = 2;
	dynamicState.pDynamicStates = dynamicStates;

	// No descriptor set layout - this pipeline never samples a texture.
	VkPushConstantRange pushConstant{};
	pushConstant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	pushConstant.offset = 0;
	pushConstant.size = sizeof(float) * 2;

	VkPipelineLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	layoutInfo.setLayoutCount = 0;
	layoutInfo.pSetLayouts = nullptr;
	layoutInfo.pushConstantRangeCount = 1;
	layoutInfo.pPushConstantRanges = &pushConstant;
	if (vkCreatePipelineLayout(m_device, &layoutInfo, nullptr, &m_wireframePipelineLayout) != VK_SUCCESS)
		return false;

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = stages;
	pipelineInfo.pVertexInputState = &vertexInput;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &raster;
	pipelineInfo.pMultisampleState = &multisample;
	pipelineInfo.pColorBlendState = &blend;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.layout = m_wireframePipelineLayout;
	pipelineInfo.renderPass = m_renderPass;
	pipelineInfo.subpass = 0;

	VkResult result = vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_wireframePipeline);

	vkDestroyShaderModule(m_device, vertModule, nullptr);
	vkDestroyShaderModule(m_device, fragModule, nullptr);

	return result == VK_SUCCESS;
}

bool VulkanPolygonRenderer::CreatePipeline()
{
	auto vertCode = ReadFile(ShaderPath("polygon.vert.spv"));
	auto fragCode = ReadFile(ShaderPath("polygon.frag.spv"));
	VkShaderModule vertModule = CreateShaderModule(m_device, vertCode);
	VkShaderModule fragModule = CreateShaderModule(m_device, fragCode);

	if (vertModule == VK_NULL_HANDLE || fragModule == VK_NULL_HANDLE)
		return false;

	VkPipelineShaderStageCreateInfo vertStage{};
	vertStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertStage.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertStage.module = vertModule;
	vertStage.pName = "main";

	// polygon.frag constant_id 0 = BILINEAR, fixed per pipeline so the nearest path pays nothing for it
	const VkBool32 bilinear = m_bilinearFiltering ? VK_TRUE : VK_FALSE;
	VkSpecializationMapEntry specEntry{ 0, 0, sizeof(VkBool32) };
	VkSpecializationInfo specInfo{};
	specInfo.mapEntryCount = 1;
	specInfo.pMapEntries = &specEntry;
	specInfo.dataSize = sizeof(bilinear);
	specInfo.pData = &bilinear;

	VkPipelineShaderStageCreateInfo fragStage{};
	fragStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragStage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragStage.module = fragModule;
	fragStage.pName = "main";
	fragStage.pSpecializationInfo = &specInfo;

	VkPipelineShaderStageCreateInfo stages[] = { vertStage, fragStage };

	VkVertexInputBindingDescription binding{};
	binding.binding = 0;
	binding.stride = sizeof(HWVertex);
	binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	VkVertexInputAttributeDescription attrs[4]{};
	attrs[0] = { 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(HWVertex, X) };      // x, y, w
	attrs[1] = { 1, 0, VK_FORMAT_R32G32_SFLOAT,    offsetof(HWVertex, U) };      // u, v
	attrs[2] = { 2, 0, VK_FORMAT_R32_SFLOAT,       offsetof(HWVertex, Shade) };  // shade
	attrs[3] = { 3, 0, VK_FORMAT_R32_UINT,         offsetof(HWVertex, Layer) };  // layer

	VkPipelineVertexInputStateCreateInfo vertexInput{};
	vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInput.vertexBindingDescriptionCount = 1;
	vertexInput.pVertexBindingDescriptions = &binding;
	vertexInput.vertexAttributeDescriptionCount = 3;
	vertexInput.pVertexAttributeDescriptions = attrs;

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;

	VkPipelineRasterizationStateCreateInfo raster{};
	raster.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	raster.polygonMode = VK_POLYGON_MODE_FILL;
	raster.cullMode = m_cullMode;
	raster.frontFace = m_frontFace;
	raster.lineWidth = 1.0f;

	VkPipelineMultisampleStateCreateInfo multisample{};
	multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisample.rasterizationSamples = m_msaaSamples;   // must match m_offscreenRenderPass

	VkPipelineColorBlendAttachmentState blendAttachment{};
	blendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	blendAttachment.blendEnable = VK_TRUE;
	blendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	blendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	blendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	blendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	blendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	blendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo blend{};
	blend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	blend.attachmentCount = 1;
	blend.pAttachments = &blendAttachment;

	VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = 2;
	dynamicState.pDynamicStates = dynamicStates;

	// Fragment-stage push constants (matches `PC` in polygon.frag)
	VkPushConstantRange pushConstant{};
	pushConstant.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstant.offset = 0;
	pushConstant.size = sizeof(uint32_t) * 3 + sizeof(float);   // 16 bytes: mode, tint, alpha, flags

	VkPipelineLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	// set 0 = the texture's set, set 1 = the reflection the water reads (m_reflectionTarget or m_noReflectionSet)
	const VkDescriptorSetLayout setLayouts[] = { m_descriptorSetLayout, m_overlayDescriptorSetLayout };
	layoutInfo.setLayoutCount = 2;
	layoutInfo.pSetLayouts = setLayouts;
	layoutInfo.pushConstantRangeCount = 1;
	layoutInfo.pPushConstantRanges = &pushConstant;
	if (vkCreatePipelineLayout(m_device, &layoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
		return false;

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = stages;
	pipelineInfo.pVertexInputState = &vertexInput;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &raster;
	pipelineInfo.pMultisampleState = &multisample;
	pipelineInfo.pColorBlendState = &blend;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.layout = m_pipelineLayout;
	pipelineInfo.renderPass = m_offscreenRenderPass;   // the only pass the polygons are drawn in (it carries the MSAA sample count)
	pipelineInfo.subpass = 0;

	VkResult result = vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline);

	vkDestroyShaderModule(m_device, vertModule, nullptr);
	vkDestroyShaderModule(m_device, fragModule, nullptr);

	return result == VK_SUCCESS;
}

bool VulkanPolygonRenderer::CreateOverlayPipeline(OverlayBlend blendMode, VkPipeline& pipeline, const char* fragShader, VkPipelineLayout layout,
	VkRenderPass renderPass, VkSampleCountFlagBits samples)
{
	auto vertCode = ReadFile(ShaderPath("overlay.vert.spv"));
	auto fragCode = ReadFile(ShaderPath(fragShader));
	VkShaderModule vertModule = CreateShaderModule(m_device, vertCode);
	VkShaderModule fragModule = CreateShaderModule(m_device, fragCode);
	if (vertModule == VK_NULL_HANDLE || fragModule == VK_NULL_HANDLE)
		return false;

	VkPipelineShaderStageCreateInfo vertStage{};
	vertStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertStage.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertStage.module = vertModule;
	vertStage.pName = "main";

	VkPipelineShaderStageCreateInfo fragStage{};
	fragStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragStage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragStage.module = fragModule;
	fragStage.pName = "main";

	VkPipelineShaderStageCreateInfo stages[] = { vertStage, fragStage };

	// No vertex input at all - the vertex shader generates positions from
	// gl_VertexIndex, so vertexBindingDescriptionCount/attributeCount stay 0.
	VkPipelineVertexInputStateCreateInfo vertexInput{};
	vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;

	VkPipelineRasterizationStateCreateInfo raster{};
	raster.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	raster.polygonMode = VK_POLYGON_MODE_FILL;
	raster.cullMode = VK_CULL_MODE_NONE;
	raster.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	raster.lineWidth = 1.0f;

	VkPipelineMultisampleStateCreateInfo multisample{};
	multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisample.rasterizationSamples = samples;

	VkPipelineColorBlendAttachmentState blendAttachment{};
	blendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	blendAttachment.blendEnable = blendMode != OverlayBlend::Opaque ? VK_TRUE : VK_FALSE;
	blendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	blendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
	if (blendMode == OverlayBlend::Constant)
	{
		// The weight is set per draw with vkCmdSetBlendConstants (alpha component).
		blendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_CONSTANT_ALPHA;
		blendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
		blendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_CONSTANT_ALPHA;
		blendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
	}
	else
	{
		blendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		blendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		blendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		blendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	}

	VkPipelineColorBlendStateCreateInfo blend{};
	blend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	blend.attachmentCount = 1;
	blend.pAttachments = &blendAttachment;

	VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_BLEND_CONSTANTS };
	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = blendMode == OverlayBlend::Constant ? 3 : 2;
	dynamicState.pDynamicStates = dynamicStates;

	// Layout is shared by both overlay pipelines.
	if (m_overlayPipelineLayout == VK_NULL_HANDLE)
	{
		VkPipelineLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		layoutInfo.setLayoutCount = 1;
		layoutInfo.pSetLayouts = &m_overlayDescriptorSetLayout;
		if (vkCreatePipelineLayout(m_device, &layoutInfo, nullptr, &m_overlayPipelineLayout) != VK_SUCCESS)
			return false;
	}

	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_FALSE;
	depthStencil.depthWriteEnable = VK_FALSE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_ALWAYS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.stencilTestEnable = VK_FALSE;

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = stages;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pVertexInputState = &vertexInput;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &raster;
	pipelineInfo.pMultisampleState = &multisample;
	pipelineInfo.pColorBlendState = &blend;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.layout = layout != VK_NULL_HANDLE ? layout : m_overlayPipelineLayout;
	pipelineInfo.renderPass = renderPass != VK_NULL_HANDLE ? renderPass : m_renderPass;
	pipelineInfo.subpass = 0;

	VkResult result = vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline);

	vkDestroyShaderModule(m_device, vertModule, nullptr);
	vkDestroyShaderModule(m_device, fragModule, nullptr);

	return result == VK_SUCCESS;
}

bool VulkanPolygonRenderer::CreateFrameData()
{
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = m_graphicsQueueFamily;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	VkSemaphoreCreateInfo semInfo{};
	semInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	for (auto& frame : m_frames)
	{
		if (vkCreateCommandPool(m_device, &poolInfo, nullptr, &frame.commandPool) != VK_SUCCESS)
			return false;

		VkCommandBufferAllocateInfo cbInfo{};
		cbInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		cbInfo.commandPool = frame.commandPool;
		cbInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		cbInfo.commandBufferCount = 1;
		if (vkAllocateCommandBuffers(m_device, &cbInfo, &frame.commandBuffer) != VK_SUCCESS)
			return false;

		vkCreateSemaphore(m_device, &semInfo, nullptr, &frame.imageAvailable);
		vkCreateSemaphore(m_device, &semInfo, nullptr, &frame.renderFinished);
		vkCreateFence(m_device, &fenceInfo, nullptr, &frame.inFlight);

		// Reasonable starting size; EnsureFrameBufferCapacity grows these
		// on demand if a frame submits more geometry.
		EnsureFrameBufferCapacity(frame, sizeof(HWVertex) * 4096, sizeof(uint32_t) * 8192);
	}

	return true;
}

// Copies the 8-bit screen, its alpha and the palette into this frame's staging slot and records the uploads.
// The palette lookup and the scaling to the window happen in hud.frag.
bool VulkanPolygonRenderer::UploadHud(SDL_Surface* hudSurface, const uint8_t* hudAlpha, VkCommandBuffer commandBuffer)
{
	const uint32_t width = (uint32_t)hudSurface->w;
	const uint32_t height = (uint32_t)hudSurface->h;
	const SDL_Palette* palette = hudSurface->format->palette;
	if (width == 0 || height == 0 || palette == nullptr || !EnsureHudImage(width, height))
		return false;

	const VkDeviceSize pixelCount = (VkDeviceSize)width * height;
	const VkDeviceSize paletteOffset = m_hudStagingSlotSize * m_currentFrame;
	const VkDeviceSize indexOffset = paletteOffset + kHudPaletteBytes;
	const VkDeviceSize alphaOffset = indexOffset + pixelCount;
	uint8_t* staging = (uint8_t*)m_hudStagingMapped;

	// SDL_Color is r, g, b, a - the same bytes as RGBA8.
	std::memset(staging + paletteOffset, 0, kHudPaletteBytes);
	std::memcpy(staging + paletteOffset, palette->colors, sizeof(SDL_Color) * (std::min)(palette->ncolors, 256));

	if (SDL_MUSTLOCK(hudSurface))
		SDL_LockSurface(hudSurface);
	if ((uint32_t)hudSurface->pitch == width)
	{
		std::memcpy(staging + indexOffset, hudSurface->pixels, (size_t)pixelCount);
	}
	else
	{
		for (uint32_t y = 0; y < height; ++y)
			std::memcpy(staging + indexOffset + (VkDeviceSize)y * width, (const uint8_t*)hudSurface->pixels + y * hudSurface->pitch, width);
	}
	if (SDL_MUSTLOCK(hudSurface))
		SDL_UnlockSurface(hudSurface);

	m_hudHasAlpha = hudAlpha != nullptr;
	if (m_hudHasAlpha)
		std::memcpy(staging + alphaOffset, hudAlpha, (size_t)pixelCount);

	// Both images are rewritten completely (an unwritten alpha layer is never read), so their old contents can go.
	VkImageMemoryBarrier toTransferDst[2]{};
	for (VkImageMemoryBarrier& barrier : toTransferDst)
	{
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	}
	toTransferDst[0].image = m_hudImage;
	toTransferDst[0].subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 2 };
	toTransferDst[1].image = m_hudPaletteImage;
	toTransferDst[1].subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
		0, 0, nullptr, 0, nullptr, 2, toTransferDst);

	VkBufferImageCopy hudRegions[2]{};
	hudRegions[0].bufferOffset = indexOffset;
	hudRegions[0].imageSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
	hudRegions[0].imageExtent = { width, height, 1 };
	hudRegions[1].bufferOffset = alphaOffset;
	hudRegions[1].imageSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 1 };
	hudRegions[1].imageExtent = { width, height, 1 };
	vkCmdCopyBufferToImage(commandBuffer, m_hudStagingBuffer, m_hudImage,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_hudHasAlpha ? 2 : 1, hudRegions);

	VkBufferImageCopy paletteRegion{};
	paletteRegion.bufferOffset = paletteOffset;
	paletteRegion.imageSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
	paletteRegion.imageExtent = { 256, 1, 1 };
	vkCmdCopyBufferToImage(commandBuffer, m_hudStagingBuffer, m_hudPaletteImage,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &paletteRegion);

	VkImageMemoryBarrier toShaderRead[2] = { toTransferDst[0], toTransferDst[1] };
	for (VkImageMemoryBarrier& barrier : toShaderRead)
	{
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	}
	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
		0, 0, nullptr, 0, nullptr, 2, toShaderRead);

	return true;
}

void VulkanPolygonRenderer::EnsureFrameBufferCapacity(FrameData& frame, size_t vertexBytes, size_t indexBytes)
{
	if (frame.vertexBufferSize < vertexBytes)
	{
		if (frame.vertexBuffer)
			vmaDestroyBuffer(m_allocator, frame.vertexBuffer, frame.vertexAllocation);

		VkBufferCreateInfo bufInfo{};
		bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufInfo.size = vertexBytes;
		bufInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

		VmaAllocationCreateInfo allocInfo{};
		allocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
		allocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

		VmaAllocationInfo outInfo{};
		vmaCreateBuffer(m_allocator, &bufInfo, &allocInfo, &frame.vertexBuffer, &frame.vertexAllocation, &outInfo);
		frame.vertexMapped = outInfo.pMappedData;
		frame.vertexBufferSize = vertexBytes;
	}

	if (frame.indexBufferSize < indexBytes)
	{
		if (frame.indexBuffer)
			vmaDestroyBuffer(m_allocator, frame.indexBuffer, frame.indexAllocation);

		VkBufferCreateInfo bufInfo{};
		bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufInfo.size = indexBytes;
		bufInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

		VmaAllocationCreateInfo allocInfo{};
		allocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
		allocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

		VmaAllocationInfo outInfo{};
		vmaCreateBuffer(m_allocator, &bufInfo, &allocInfo, &frame.indexBuffer, &frame.indexAllocation, &outInfo);
		frame.indexMapped = outInfo.pMappedData;
		frame.indexBufferSize = indexBytes;
	}
}

void VulkanPolygonRenderer::DestroyFrameData()
{
	for (auto& frame : m_frames)
	{
		if (frame.vertexBuffer) vmaDestroyBuffer(m_allocator, frame.vertexBuffer, frame.vertexAllocation);
		if (frame.indexBuffer) vmaDestroyBuffer(m_allocator, frame.indexBuffer, frame.indexAllocation);
		vkDestroySemaphore(m_device, frame.imageAvailable, nullptr);
		vkDestroySemaphore(m_device, frame.renderFinished, nullptr);
		vkDestroyFence(m_device, frame.inFlight, nullptr);
		vkDestroyCommandPool(m_device, frame.commandPool, nullptr);
	}
}

bool VulkanPolygonRenderer::BeginFrame(SDL_Surface* hudSurface, const uint8_t* hudAlpha, SDL_Rect srcRect, SDL_Rect destRect)
{
	FrameData& frame = m_frames[m_currentFrame];
	vkWaitForFences(m_device, 1, &frame.inFlight, VK_TRUE, UINT64_MAX);

	VkResult acquireResult = vkAcquireNextImageKHR(m_device, m_swapchain, UINT64_MAX,
		frame.imageAvailable, VK_NULL_HANDLE, &m_currentImageIndex);

	if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR)
	{
		Resize(m_windowWidth, m_windowHeight);
		return false; // skip this frame; caller should just try again next tick
	}

	// Where the cells go (one per view, each showing the whole frame): the window, or a derived renderer's layout
	const FrameCells cells = GetFrameCells(srcRect, destRect);
	const size_t cellCount = (std::max)(cells.count, (size_t)1);
	const VkExtent2D cellsExtent = cells.extent;
	const auto cellRect = [&](size_t cell) -> SDL_Rect { return GetCellRect(cell, cells, srcRect); };

	// Polygon vertices are NDC relative to the game viewport, so map that
	// viewport (game-resolution pixels) into the letterboxed cell.
	const auto polyRectIn = [&](const SDL_Rect& cell) -> SDL_Rect {
		SDL_Rect polyRect = cell;
		if (m_gameViewPort.w > 0 && m_gameViewPort.h > 0 && srcRect.w > 0 && srcRect.h > 0)
		{
			const float scaleX = (float)cell.w / (float)srcRect.w;
			const float scaleY = (float)cell.h / (float)srcRect.h;
			polyRect.x = cell.x + (int)(m_gameViewPort.x * scaleX);
			polyRect.y = cell.y + (int)(m_gameViewPort.y * scaleY);
			polyRect.w = (int)(m_gameViewPort.w * scaleX);
			polyRect.h = (int)(m_gameViewPort.h * scaleY);
		}
		return polyRect;
		};

	// Scissor must stay within the framebuffer.
	const auto scissorOf = [&](const SDL_Rect& rect) -> VkRect2D {
		const int x0 = (std::max)(rect.x, 0);
		const int y0 = (std::max)(rect.y, 0);
		const int x1 = (std::min)(rect.x + rect.w, (int)cellsExtent.width);
		const int y1 = (std::min)(rect.y + rect.h, (int)cellsExtent.height);
		return VkRect2D{ { x0, y0 }, { (uint32_t)(std::max)(x1 - x0, 0), (uint32_t)(std::max)(y1 - y0, 0) } };
		};

	// A list per cell gives each cell its own view; anything else shows the first list everywhere (or what a derived
	// renderer makes of them: the anaglyph's two eyes)
	const size_t viewCount = GetFrameViewCount(cellCount);
	const auto viewOfCell = [&](size_t cell) { return viewCount == cellCount ? cell : 0; };

	// Each view is rendered at the game viewport's resolution (with Looking Glass the game resolution is the cell
	// size), or the full game resolution until the first E_VIEWPORT_CHANGE arrives.
	const uint32_t targetWidth = (uint32_t)(m_gameViewPort.w > 0 ? m_gameViewPort.w : srcRect.w);
	const uint32_t targetHeight = (uint32_t)(m_gameViewPort.h > 0 ? m_gameViewPort.h : srcRect.h);

	// Vulkan requires a non-zero viewport, so skip the polygons when there is nowhere to draw them.
	const SDL_Rect firstPolyRect = polyRectIn(cellRect(0));
	const VkRect2D firstScissor = scissorOf(firstPolyRect);
	bool drawPolygons = viewCount > 0 && targetWidth > 0 && targetHeight > 0 &&
		firstPolyRect.w > 0 && firstPolyRect.h > 0 && firstScissor.extent.width > 0 && firstScissor.extent.height > 0 &&
		std::any_of(m_views.begin(), m_views.begin() + viewCount, [](const std::vector<RenderPolygon>& view) { return !view.empty(); });

	if (drawPolygons && (targetWidth != m_offscreenWidth || targetHeight != m_offscreenHeight || m_viewTargets.size() != viewCount))
	{
		// The other frame in flight may still be sampling the old targets.
		vkDeviceWaitIdle(m_device);
		DestroyOffscreenTarget();
		if (!CreateOffscreenTarget(targetWidth, targetHeight, viewCount))
		{
			DestroyOffscreenTarget();
			drawPolygons = false;
		}
	}

	vkResetFences(m_device, 1, &frame.inFlight);
	vkResetCommandBuffer(frame.commandBuffer, 0);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	vkBeginCommandBuffer(frame.commandBuffer, &beginInfo);

	// The viewport clears to the map's clear colour; the window it is blitted into clears to black.
	VkClearValue clearColor{};
	clearColor.color = m_clearColor;

	VkClearValue windowClearColor{};
	windowClearColor.color = { { 0.0f, 0.0f, 0.0f, 1.0f } };

	const bool drawHud = UploadHud(hudSurface, hudAlpha, frame.commandBuffer);

	if (drawPolygons)
	{
		// The geometry of every view goes into the frame's buffers at once: the draws only read them when the GPU runs
		std::vector<HWVertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<Batch> batches;
		std::vector<size_t> firstBatch(viewCount + 1);
		std::vector<size_t> reflectionEnds(viewCount);
		size_t polygonCount = 0;
		for (size_t view = 0; view < viewCount; view++)
			polygonCount += m_views[view].size();
		vertices.reserve(polygonCount * 4);
		indices.reserve(polygonCount * 6);
		for (size_t view = 0; view < viewCount; view++)
		{
			// With water the sky and the reflection go into m_reflectionTarget first (DrawBatches then starts the terrain pass)
			reflectionEnds[view] = ReflectionEnd(m_views[view]);
			firstBatch[view] = batches.size();
			AppendViewGeometry(m_views[view], reflectionEnds[view], vertices, indices, batches);
		}
		firstBatch[viewCount] = batches.size();

		if (!vertices.empty() && !indices.empty())
		{
			EnsureFrameBufferCapacity(frame, vertices.size() * sizeof(HWVertex), indices.size() * sizeof(uint32_t));
			std::memcpy(frame.vertexMapped, vertices.data(), vertices.size() * sizeof(HWVertex));
			std::memcpy(frame.indexMapped, indices.data(), indices.size() * sizeof(uint32_t));
		}

		for (size_t view = 0; view < viewCount; view++)
		{
			const ColourTarget& target = m_viewTargets[view];

			VkRenderPassBeginInfo offscreenBegin{};
			offscreenBegin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			offscreenBegin.renderPass = m_offscreenRenderPass;
			offscreenBegin.framebuffer = reflectionEnds[view] != kNoReflectionPass ? m_reflectionTarget.framebuffer : target.framebuffer;
			offscreenBegin.renderArea.extent = { m_offscreenWidth, m_offscreenHeight };
			offscreenBegin.clearValueCount = 1;
			offscreenBegin.pClearValues = &clearColor;

			vkCmdBeginRenderPass(frame.commandBuffer, &offscreenBegin, VK_SUBPASS_CONTENTS_INLINE);
			vkCmdBindPipeline(frame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
			vkCmdBindDescriptorSets(frame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
				m_pipelineLayout, 1, 1, &m_noReflectionSet, 0, nullptr);

			VkViewport offscreenViewport{ 0.0f, 0.0f, (float)m_offscreenWidth, (float)m_offscreenHeight, 0.0f, 1.0f };
			VkRect2D offscreenScissor{ { 0, 0 }, { m_offscreenWidth, m_offscreenHeight } };
			vkCmdSetViewport(frame.commandBuffer, 0, 1, &offscreenViewport);
			vkCmdSetScissor(frame.commandBuffer, 0, 1, &offscreenScissor);

			DrawBatches(frame.commandBuffer, batches.data() + firstBatch[view], firstBatch[view + 1] - firstBatch[view],
				reflectionEnds[view], target);

			vkCmdEndRenderPass(frame.commandBuffer);

			if (m_blurActive)
				DrawBlur(frame.commandBuffer, target, m_blurTargets[view]);
		}
		if (m_blurActive)
			m_blurSeed = false;
	}
	else
	{
		// Nothing new goes into the history, so don't blend the next frame with a stale one.
		m_blurSeed = true;
	}

	// The window's render pass, or a derived renderer's target for the cells
	BeginCellsRenderPass(frame.commandBuffer, windowClearColor);

	for (size_t cell = 0; cell < cellCount; cell++)
	{
		const SDL_Rect cellArea = cellRect(cell);
		if (drawPolygons)
		{
			// Upscale the view's image (or its blur history) into the viewport's area of the cell.
			const SDL_Rect polyRect = polyRectIn(cellArea);
			const VkRect2D scissor = scissorOf(polyRect);
			if (polyRect.w > 0 && polyRect.h > 0 && scissor.extent.width > 0 && scissor.extent.height > 0)
			{
				VkViewport viewport{ (float)polyRect.x, (float)polyRect.y, (float)polyRect.w, (float)polyRect.h, 0.0f, 1.0f };
				vkCmdSetViewport(frame.commandBuffer, 0, 1, &viewport);
				vkCmdSetScissor(frame.commandBuffer, 0, 1, &scissor);

				DrawCellViews(frame.commandBuffer, cell, viewOfCell(cell));
			}
		}

		if (drawHud && cellArea.w > 0 && cellArea.h > 0)
			DrawOverlay(cellArea, frame.commandBuffer, cellsExtent);
	}

	// Whatever goes into the window after the cells (EndFrame ends the window's render pass)
	EndCellsRenderPass(frame.commandBuffer, windowClearColor);

	return true;
}

// Where the terrain pass starts, when the frame has water: after the sky and the reflection at the front of the
// list (possibly 0: from high up there is no reflection, and the water is blended with what was cleared).
size_t VulkanPolygonRenderer::ReflectionEnd(const std::vector<RenderPolygon>& polygons) const
{
	const bool hasWater = std::any_of(polygons.begin(), polygons.end(), [](const RenderPolygon& p) {
		return p.TextureId == kTerrainAtlasId && p.SpriteMode == 26;
		});
	if (!hasWater)
		return kNoReflectionPass;

	size_t end = 0;
	while (end < polygons.size() && (polygons[end].Reflection || polygons[end].TextureId == kSkyId))
		end++;
	return end;
}

// Ends the reflection pass and starts the terrain pass on the offscreen target, beginning with a copy of the
// reflection (the water blends with it, everything else draws over it as before) and with set 1 = the reflection.
void VulkanPolygonRenderer::BeginTerrainPass(VkCommandBuffer commandBuffer, const ColourTarget& target)
{
	vkCmdEndRenderPass(commandBuffer);

	VkClearValue clearColor{};
	clearColor.color = m_clearColor;

	VkRenderPassBeginInfo begin{};
	begin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	begin.renderPass = m_offscreenRenderPass;
	begin.framebuffer = target.framebuffer;
	begin.renderArea.extent = { m_offscreenWidth, m_offscreenHeight };
	begin.clearValueCount = 1;
	begin.pClearValues = &clearColor;
	vkCmdBeginRenderPass(commandBuffer, &begin, VK_SUBPASS_CONTENTS_INLINE);

	// Same size, nearest sampling: an exact copy
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_reflectionCopyPipeline);
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
		m_overlayPipelineLayout, 0, 1, &m_reflectionTarget.descriptorSet, 0, nullptr);
	vkCmdDraw(commandBuffer, 3, 1, 0, 0);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
		m_pipelineLayout, 1, 1, &m_reflectionTarget.descriptorSet, 0, nullptr);
}

// Blends the frame just drawn into the blur history: history = new * weight + history * (1 - weight).
// The first frame after the history becomes invalid is copied instead (weight 1, history not loaded).
// The caller clears m_blurSeed once every view's history has been drawn.
void VulkanPolygonRenderer::DrawBlur(VkCommandBuffer commandBuffer, const ColourTarget& frame, const ColourTarget& history)
{
	VkRenderPassBeginInfo blurBegin{};
	blurBegin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	blurBegin.renderPass = m_blurSeed ? m_blurSeedRenderPass : m_blurRenderPass;
	blurBegin.framebuffer = history.framebuffer;
	blurBegin.renderArea.extent = { m_offscreenWidth, m_offscreenHeight };

	vkCmdBeginRenderPass(commandBuffer, &blurBegin, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport blurViewport{ 0.0f, 0.0f, (float)m_offscreenWidth, (float)m_offscreenHeight, 0.0f, 1.0f };
	VkRect2D blurScissor{ { 0, 0 }, { m_offscreenWidth, m_offscreenHeight } };
	vkCmdSetViewport(commandBuffer, 0, 1, &blurViewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &blurScissor);

	const float weight = m_blurSeed ? 1.0f : kBlurNewFrameWeight;
	const float blendConstants[4] = { weight, weight, weight, weight };
	vkCmdSetBlendConstants(commandBuffer, blendConstants);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_blurPipeline);
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
		m_overlayPipelineLayout, 0, 1, &frame.descriptorSet, 0, nullptr);
	vkCmdDraw(commandBuffer, 3, 1, 0, 0);

	vkCmdEndRenderPass(commandBuffer);
}

void VulkanPolygonRenderer::DrawPolygonsWireframe(const std::vector<RenderPolygon>& polygons)
{
	FrameData& frame = m_frames[m_currentFrame];

	std::vector<HWVertex> vertices;
	std::vector<uint32_t> indices;
	vertices.reserve(polygons.size() * 4);
	indices.reserve(polygons.size() * 6);

	for (const auto& poly : polygons)
	{
		if (poly.Vertices.size() < 3)
			continue;

		uint32_t baseVertex = (uint32_t)vertices.size();
		for (const auto& v : poly.Vertices)
		{
			vertices.push_back(v);
		}
		FanTriangulate(baseVertex, (uint32_t)poly.Vertices.size(), indices);
	}

	if (vertices.empty() || indices.empty())
		return;

	EnsureFrameBufferCapacity(frame, vertices.size() * sizeof(HWVertex), indices.size() * sizeof(uint32_t));
	std::memcpy(frame.vertexMapped, vertices.data(), vertices.size() * sizeof(HWVertex));
	std::memcpy(frame.indexMapped, indices.data(), indices.size() * sizeof(uint32_t));

	VkDeviceSize offset = 0;
	vkCmdBindVertexBuffers(frame.commandBuffer, 0, 1, &frame.vertexBuffer, &offset);
	vkCmdBindIndexBuffer(frame.commandBuffer, frame.indexBuffer, 0, VK_INDEX_TYPE_UINT32);
	vkCmdBindPipeline(frame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_wireframePipeline);

	// No texture sorting/batching and no descriptor set bind - this pipeline
	// has no sampler, so all polygons draw in one call regardless of TextureId.
	vkCmdDrawIndexed(frame.commandBuffer, (uint32_t)indices.size(), 1, 0, 0, 0);
}

void VulkanPolygonRenderer::AppendViewGeometry(const std::vector<RenderPolygon>& polygons, size_t reflectionEnd,
	std::vector<HWVertex>& vertices, std::vector<uint32_t>& indices, std::vector<Batch>& batches)
{
	auto modeOf = [](const RenderPolygon& p) -> uint32_t {
		if (p.TextureId == kTerrainAtlasId)
			return p.SpriteMode == 26 ? 26u : 255u;
		return (uint32_t)p.SpriteMode;
		};
	auto tintOf = [](const RenderPolygon& p) -> uint32_t {
		if (p.TextureId == kTerrainAtlasId)
			return p.SpriteMode == 26 ? (uint32_t)p.Tint : 0u;   // water: the map type
		return (uint32_t)p.Tint;
		};
	auto alphaOf = [](const RenderPolygon& p) -> float {
		return p.Alpha;   // terrain defaults to 1.0
		};

	bool haveBatch = false;
	uint32_t curTexture = 0, curMode = 0, curTint = 0;
	bool     curTerrainPass = false;
	uint32_t curFrames = 1;
	float    curAlpha = 1.0f;
	uint32_t batchIndexStart = 0;

	for (size_t polyIndex = 0; polyIndex < polygons.size(); polyIndex++)
	{
		const RenderPolygon& poly = polygons[polyIndex];
		if (poly.Vertices.size() < 3)
			continue;

		const uint32_t m = modeOf(poly);
		const uint32_t t = tintOf(poly);
		const float    a = alphaOf(poly);
		const bool     tp = reflectionEnd != kNoReflectionPass && polyIndex >= reflectionEnd;

		if (!haveBatch)
		{
			curTexture = poly.TextureId; curMode = m; curTint = t; curAlpha = a; curTerrainPass = tp; curFrames = poly.Frames;
			batchIndexStart = (uint32_t)indices.size();
			haveBatch = true;
		}
		else if (poly.TextureId != curTexture || m != curMode || t != curTint || a != curAlpha || tp != curTerrainPass)
		{
			batches.push_back({ curTexture, curMode, curTint, curAlpha, curTerrainPass, curFrames,
				batchIndexStart, (uint32_t)indices.size() - batchIndexStart });
			batchIndexStart = (uint32_t)indices.size();
			curTexture = poly.TextureId; curMode = m; curTint = t; curAlpha = a; curTerrainPass = tp; curFrames = poly.Frames;
		}

		uint32_t baseVertex = (uint32_t)vertices.size();
		for (const auto& v : poly.Vertices)
			vertices.push_back(v);
		FanTriangulate(baseVertex, (uint32_t)poly.Vertices.size(), indices);
	}

	if (haveBatch)
		batches.push_back({ curTexture, curMode, curTint, curAlpha, curTerrainPass, curFrames,
			batchIndexStart, (uint32_t)indices.size() - batchIndexStart });
}

void VulkanPolygonRenderer::DrawBatches(VkCommandBuffer commandBuffer, const Batch* batches, size_t batchCount,
	size_t reflectionEnd, const ColourTarget& target)
{
	FrameData& frame = m_frames[m_currentFrame];

	if (batchCount > 0)
	{
		VkDeviceSize offset = 0;
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &frame.vertexBuffer, &offset);
		vkCmdBindIndexBuffer(commandBuffer, frame.indexBuffer, 0, VK_INDEX_TYPE_UINT32);
	}

	struct PC { uint32_t mode; uint32_t tint; float alpha; uint32_t flags; };

	// polygon.frag flags bits 8-15: the sky (clear) colour index, the top of the day water's ramp; bits 16-31: sprite frames
	const uint32_t skyIndex = (uint32_t)(m_clearPaletteIndex >= 0 ? m_clearPaletteIndex : 0xFE) << 8;

	bool inTerrainPass = reflectionEnd == kNoReflectionPass;
	for (size_t batchIndex = 0; batchIndex < batchCount; batchIndex++)
	{
		const Batch& batch = batches[batchIndex];
		if (batch.terrainPass && !inTerrainPass)
		{
			BeginTerrainPass(commandBuffer, target);
			inTerrainPass = true;
		}

		auto it = m_textures.find(batch.textureId);
		if (it == m_textures.end())
			continue;

		PC pc{ batch.mode, batch.tint, batch.alpha, skyIndex | (std::min)(batch.frames, 0xFFFFu) << 16 };
		vkCmdPushConstants(commandBuffer, m_pipelineLayout,
			VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(pc), &pc);

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
			m_pipelineLayout, 0, 1, &it->second.descriptorSet, 0, nullptr);
		vkCmdDrawIndexed(commandBuffer, batch.indexCount, 1, batch.indexOffset, 0, 0);
	}

	// The offscreen target must get the reflection even if nothing came after it
	if (!inTerrainPass)
		BeginTerrainPass(commandBuffer, target);
}

void VulkanPolygonRenderer::EndFrame()
{
	FrameData& frame = m_frames[m_currentFrame];

	vkCmdEndRenderPass(frame.commandBuffer);
	vkEndCommandBuffer(frame.commandBuffer);

	VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	VkSubmitInfo submit{};
	submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit.waitSemaphoreCount = 1;
	submit.pWaitSemaphores = &frame.imageAvailable;
	submit.pWaitDstStageMask = &waitStage;
	submit.commandBufferCount = 1;
	submit.pCommandBuffers = &frame.commandBuffer;
	submit.signalSemaphoreCount = 1;
	submit.pSignalSemaphores = &frame.renderFinished;
	vkQueueSubmit(m_graphicsQueue, 1, &submit, frame.inFlight);

	VkPresentInfoKHR present{};
	present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	present.waitSemaphoreCount = 1;
	present.pWaitSemaphores = &frame.renderFinished;
	present.swapchainCount = 1;
	present.pSwapchains = &m_swapchain;
	present.pImageIndices = &m_currentImageIndex;
	VkResult presentResult = vkQueuePresentKHR(m_graphicsQueue, &present);

	if (presentResult == VK_ERROR_OUT_OF_DATE_KHR || presentResult == VK_SUBOPTIMAL_KHR)
		Resize(m_windowWidth, m_windowHeight);

	m_currentFrame = (m_currentFrame + 1) % kFramesInFlight;
}

void VulkanPolygonRenderer::DrawOverlay(SDL_Rect dscrect, VkCommandBuffer commandBuffer, VkExtent2D extent)
{
	VkViewport overlayViewport{};
	overlayViewport.x = (float)dscrect.x;
	overlayViewport.y = (float)dscrect.y;
	overlayViewport.width = (float)dscrect.w;
	overlayViewport.height = (float)dscrect.h;
	overlayViewport.minDepth = 0.0f;
	overlayViewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &overlayViewport);

	// Scissor to the same rect so nothing spills into the letterbox bars,
	// even though the fullscreen triangle's clip-space coords already cover
	// the full -1..1 range - the viewport alone maps that range onto
	// dscrect, but without a matching scissor, rasterization can still touch
	// pixels outside it depending on the driver/triangle setup.
	VkRect2D overlayScissor{};
	overlayScissor.offset = { dscrect.x, dscrect.y };
	overlayScissor.extent = { (uint32_t)dscrect.w, (uint32_t)dscrect.h };
	vkCmdSetScissor(commandBuffer, 0, 1, &overlayScissor);

	const int32_t hudPushConstants[3] = { (int32_t)m_hudWidth, (int32_t)m_hudHeight, m_hudHasAlpha ? 1 : 0 };
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_hudPipeline);
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
		m_hudPipelineLayout, 0, 1, &m_hudDescriptorSet, 0, nullptr);
	vkCmdPushConstants(commandBuffer, m_hudPipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(hudPushConstants), hudPushConstants);
	vkCmdDraw(commandBuffer, 3, 1, 0, 0);

	// Restore full-extent viewport/scissor so anything drawn after this in
	// the same render pass isn't affected.
	VkViewport fullViewport{ 0, 0, (float)extent.width, (float)extent.height, 0.0f, 1.0f };
	VkRect2D fullScissor{ {0, 0}, extent };
	vkCmdSetViewport(commandBuffer, 0, 1, &fullViewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &fullScissor);
}

void VulkanPolygonRenderer::Shutdown()
{
	if (!m_initialized)
		return;

	vkDeviceWaitIdle(m_device);

	for (auto& [id, tex] : m_textures)
	{
		vkFreeDescriptorSets(m_device, m_descriptorPool, 1, &tex.descriptorSet);
		vkDestroyImageView(m_device, tex.view, nullptr);
		vmaDestroyImage(m_allocator, tex.image, tex.allocation);
	}
	m_textures.clear();

	DestroyFrameData();
	DestroyOffscreenTarget();
	vkDestroyPipeline(m_device, m_viewPortBlitPipeline, nullptr);
	vkDestroyPipeline(m_device, m_reflectionCopyPipeline, nullptr);
	vkDestroyPipeline(m_device, m_blurPipeline, nullptr);
	vkDestroyRenderPass(m_device, m_offscreenRenderPass, nullptr);
	vkDestroyRenderPass(m_device, m_blurSeedRenderPass, nullptr);
	vkDestroyRenderPass(m_device, m_blurRenderPass, nullptr);

	vkDestroyPipeline(m_device, m_hudPipeline, nullptr);
	vkDestroyPipelineLayout(m_device, m_hudPipelineLayout, nullptr);
	vkDestroyDescriptorSetLayout(m_device, m_hudDescriptorSetLayout, nullptr);
	DestroyHudImage();
	vkDestroyImageView(m_device, m_hudPaletteView, nullptr);
	vmaDestroyImage(m_allocator, m_hudPaletteImage, m_hudPaletteAlloc);

	vkDestroyPipelineLayout(m_device, m_overlayPipelineLayout, nullptr);
	vkDestroyDescriptorSetLayout(m_device, m_overlayDescriptorSetLayout, nullptr);
	vkDestroySampler(m_device, m_overlaySampler, nullptr);

	vmaDestroyBuffer(m_allocator, m_shadeLutStaging, m_shadeLutStagingAllocation);
	vkDestroyImageView(m_device, m_shadeLutView, nullptr);
	vmaDestroyImage(m_allocator, m_shadeLutImage, m_shadeLutAllocation);
	vmaDestroyBuffer(m_allocator, m_blendLutStaging, m_blendLutStagingAllocation);
	vkDestroyImageView(m_device, m_blendLutView, nullptr);
	vmaDestroyImage(m_allocator, m_blendLutImage, m_blendLutAllocation);

	vmaDestroyBuffer(m_allocator, m_paletteStagingBuffer, m_paletteStagingAllocation);
	vkDestroyImageView(m_device, m_paletteView, nullptr);
	vmaDestroyImage(m_allocator, m_paletteImage, m_paletteAllocation);

	vkDestroyPipeline(m_device, m_pipeline, nullptr);
	vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
	vkDestroyPipeline(m_device, m_wireframePipeline, nullptr);
	vkDestroyPipelineLayout(m_device, m_wireframePipelineLayout, nullptr);
	vkDestroySampler(m_device, m_nearestSampler, nullptr);
	vkDestroyDescriptorPool(m_device, m_descriptorPool, nullptr);
	vkDestroyDescriptorSetLayout(m_device, m_descriptorSetLayout, nullptr);
	vkDestroyRenderPass(m_device, m_renderPass, nullptr);

	DestroySwapchain();

	vmaDestroyAllocator(m_allocator);

	vkb::destroy_surface(m_vkbInstance, m_surface);
	vkb::destroy_device(m_vkbDevice);
	vkb::destroy_instance(m_vkbInstance);

	m_initialized = false;
}
