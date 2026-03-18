// =============================================================================
// VulkanRenderer.cpp
// =============================================================================

#include "VulkanRenderer.h"

#include <cstdio>
#include <cstring>
#include <cassert>
#include <algorithm>

// ---------------------------------------------------------------------------
// Inline SPIR-V shaders (compiled from GLSL, embedded as uint32_t arrays).
// We embed them directly to avoid external shader files at runtime.
// ---------------------------------------------------------------------------
#include "shaders/SolidFill.vert.spv.h"
#include "shaders/SolidFill.frag.spv.h"
#include "shaders/Textured.vert.spv.h"
#include "shaders/Textured.frag.spv.h"

// ---------------------------------------------------------------------------
// Helper macros
// ---------------------------------------------------------------------------
#define VR_CHECK(expr)                                              \
    do {                                                            \
        VkResult _r = (expr);                                       \
        if (_r != VK_SUCCESS) {                                     \
            fprintf(stderr, "[VR] %s failed: %d (%s:%d)\n",        \
                    #expr, (int)_r, __FILE__, __LINE__);            \
            return false;                                           \
        }                                                           \
    } while (0)

#define VR_CHECK_V(expr)                                            \
    do {                                                            \
        VkResult _r = (expr);                                       \
        if (_r != VK_SUCCESS) {                                     \
            fprintf(stderr, "[VR] %s failed: %d (%s:%d)\n",        \
                    #expr, (int)_r, __FILE__, __LINE__);            \
        }                                                           \
    } while (0)

// ===========================================================================
// Lifecycle
// ===========================================================================

VulkanRenderer::~VulkanRenderer()
{
    shutdown();
}

bool VulkanRenderer::init(uint32_t screenWidth,
                          uint32_t screenHeight,
                          VkInstance        existingInstance,
                          VkPhysicalDevice  existingPhysDevice,
                          VkDevice          existingDevice)
{
    m_screenW = screenWidth;
    m_screenH = screenHeight;

    if (existingInstance == VK_NULL_HANDLE)
    {
        if (!createInstance()) return false;
        m_ownsInstance = true;
    }
    else
    {
        m_instance = existingInstance;
    }

    if (existingPhysDevice == VK_NULL_HANDLE)
    {
        if (!selectPhysicalDevice()) return false;
    }
    else
    {
        m_physDevice = existingPhysDevice;
        // Still need to find queue family indices even for existing physical device
        uint32_t qfCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(m_physDevice, &qfCount, nullptr);
        std::vector<VkQueueFamilyProperties> qf(qfCount);
        vkGetPhysicalDeviceQueueFamilyProperties(m_physDevice, &qfCount, qf.data());
        for (uint32_t i = 0; i < qfCount; ++i)
        {
            if (qf[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            { m_graphicsFamily = i; break; }
        }
    }

    if (existingDevice == VK_NULL_HANDLE)
    {
        if (!createDevice()) return false;
        m_ownsDevice = true;
    }
    else
    {
        m_device = existingDevice;
        vkGetDeviceQueue(m_device, m_graphicsFamily, 0, &m_graphicsQueue);
    }

    if (!createCommandPool())      return false;
    if (!createOffscreenTarget())  return false;
    if (!createReadbackBuffer())   return false;
    if (!createVertexIndexBuffers()) return false;
    if (!createDescriptorPool())   return false;
    if (!createDescriptorSetLayout()) return false;

    // Palette buffer (256 RGBA8 entries)
    if (!createBuffer(VR_PALETTE_ENTRIES * sizeof(uint32_t),
                      VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                      VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                      m_paletteBuf)) return false;

    // Light table buffer
    if (!createBuffer(VR_LIGHT_TABLE_SIZE * 3,
                      VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                      VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                      m_lightTableBuf)) return false;

    // Texture atlas (device-local, transferred via staging)
    if (!createImage(VR_TEXTURE_ATLAS_DIM, VR_TEXTURE_ATLAS_DIM,
                     VK_FORMAT_R8_UINT,
                     VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                     VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                     m_textureAtlas)) return false;

    // Nearest-neighbour sampler (paletted textures must not be filtered)
    VkSamplerCreateInfo samplerCI{};
    samplerCI.sType        = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerCI.magFilter    = VK_FILTER_NEAREST;
    samplerCI.minFilter    = VK_FILTER_NEAREST;
    samplerCI.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCI.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCI.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    VR_CHECK(vkCreateSampler(m_device, &samplerCI, nullptr, &m_sampler));

    if (!createDescriptorSets())  return false;
    if (!createRenderPass())      return false;
    if (!createFramebuffer())     return false;
    if (!createPipelines())       return false;
    if (!createSyncObjects())     return false;

    // Allocate command buffer
    VkCommandBufferAllocateInfo cbAI{};
    cbAI.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cbAI.commandPool        = m_cmdPool;
    cbAI.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cbAI.commandBufferCount = 1;
    VR_CHECK(vkAllocateCommandBuffers(m_device, &cbAI, &m_cmdBuf));

    m_drawCalls.reserve(VR_MAX_DRAW_CALLS_PER_FRAME);

    fprintf(stderr, "[VR] VulkanRenderer initialised (%ux%u)\n",
            m_screenW, m_screenH);
    return true;
}

void VulkanRenderer::shutdown()
{
    if (m_device == VK_NULL_HANDLE) return;

    vkDeviceWaitIdle(m_device);

    if (m_fence != VK_NULL_HANDLE)
        vkDestroyFence(m_device, m_fence, nullptr);

    for (auto& p : m_pipelines)
        if (p != VK_NULL_HANDLE) vkDestroyPipeline(m_device, p, nullptr);

    if (m_pipelineLayout != VK_NULL_HANDLE)
        vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);

    if (m_descPool != VK_NULL_HANDLE)
        vkDestroyDescriptorPool(m_device, m_descPool, nullptr);
    if (m_descSetLayout != VK_NULL_HANDLE)
        vkDestroyDescriptorSetLayout(m_device, m_descSetLayout, nullptr);

    if (m_sampler != VK_NULL_HANDLE)
        vkDestroySampler(m_device, m_sampler, nullptr);

    if (m_framebuffer != VK_NULL_HANDLE)
        vkDestroyFramebuffer(m_device, m_framebuffer, nullptr);
    if (m_renderPass != VK_NULL_HANDLE)
        vkDestroyRenderPass(m_device, m_renderPass, nullptr);

    destroyImage(m_renderTarget);
    destroyImage(m_textureAtlas);
    destroyBuffer(m_readbackBuf);
    destroyBuffer(m_vertexBuf);
    destroyBuffer(m_indexBuf);
    destroyBuffer(m_paletteBuf);
    destroyBuffer(m_lightTableBuf);

    if (m_cmdPool != VK_NULL_HANDLE)
        vkDestroyCommandPool(m_device, m_cmdPool, nullptr);

    if (m_ownsDevice)
        vkDestroyDevice(m_device, nullptr);
    if (m_ownsInstance && m_instance != VK_NULL_HANDLE)
        vkDestroyInstance(m_instance, nullptr);

    m_device   = VK_NULL_HANDLE;
    m_instance = VK_NULL_HANDLE;
}

// ===========================================================================
// Instance / device creation
// ===========================================================================

bool VulkanRenderer::createInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "GameRenderVulkan";
    appInfo.apiVersion       = VK_API_VERSION_1_1;

    VkInstanceCreateInfo ci{};
    ci.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    ci.pApplicationInfo = &appInfo;

    VR_CHECK(vkCreateInstance(&ci, nullptr, &m_instance));
    return true;
}

bool VulkanRenderer::selectPhysicalDevice()
{
    uint32_t count = 0;
    vkEnumeratePhysicalDevices(m_instance, &count, nullptr);
    if (count == 0) { fputs("[VR] No Vulkan physical device found\n", stderr); return false; }

    std::vector<VkPhysicalDevice> devices(count);
    vkEnumeratePhysicalDevices(m_instance, &count, devices.data());

    // Prefer discrete GPU
    for (auto pd : devices)
    {
        VkPhysicalDeviceProperties p;
        vkGetPhysicalDeviceProperties(pd, &p);
        if (p.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        { m_physDevice = pd; break; }
    }
    if (m_physDevice == VK_NULL_HANDLE) m_physDevice = devices[0];

    // Find graphics queue family
    uint32_t qfCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_physDevice, &qfCount, nullptr);
    std::vector<VkQueueFamilyProperties> qf(qfCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_physDevice, &qfCount, qf.data());
    for (uint32_t i = 0; i < qfCount; ++i)
    {
        if (qf[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        { m_graphicsFamily = i; break; }
    }
    if (m_graphicsFamily == UINT32_MAX)
    { fputs("[VR] No graphics queue family\n", stderr); return false; }

    return true;
}

bool VulkanRenderer::createDevice()
{
    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueCI{};
    queueCI.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCI.queueFamilyIndex = m_graphicsFamily;
    queueCI.queueCount       = 1;
    queueCI.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures features{};  // we need none beyond base

    VkDeviceCreateInfo ci{};
    ci.sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    ci.queueCreateInfoCount = 1;
    ci.pQueueCreateInfos    = &queueCI;
    ci.pEnabledFeatures     = &features;

    VR_CHECK(vkCreateDevice(m_physDevice, &ci, nullptr, &m_device));
    vkGetDeviceQueue(m_device, m_graphicsFamily, 0, &m_graphicsQueue);
    return true;
}

bool VulkanRenderer::createCommandPool()
{
    VkCommandPoolCreateInfo ci{};
    ci.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    ci.queueFamilyIndex = m_graphicsFamily;
    ci.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    VR_CHECK(vkCreateCommandPool(m_device, &ci, nullptr, &m_cmdPool));
    return true;
}

// ===========================================================================
// Resource creation
// ===========================================================================

bool VulkanRenderer::createOffscreenTarget()
{
    if (!createImage(m_screenW, m_screenH,
                     VK_FORMAT_R8_UINT,
                     VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                     VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
                     VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                     m_renderTarget))
        return false;

    return true;
}

bool VulkanRenderer::createReadbackBuffer()
{
    return createBuffer(
        (VkDeviceSize)m_screenW * m_screenH,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        m_readbackBuf);
}

bool VulkanRenderer::createVertexIndexBuffers()
{
    const VkDeviceSize vtxSize = VR_MAX_VERTICES_PER_FRAME * sizeof(VR_Vertex);
    const VkDeviceSize idxSize = VR_MAX_INDICES_PER_FRAME  * sizeof(uint32_t);

    if (!createBuffer(vtxSize,
                      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                      VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                      m_vertexBuf))
        return false;

    if (!createBuffer(idxSize,
                      VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                      VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                      m_indexBuf))
        return false;

    m_vtxPtr = static_cast<VR_Vertex*>(m_vertexBuf.mapped);
    m_idxPtr = static_cast<uint32_t*>(m_indexBuf.mapped);
    return true;
}

bool VulkanRenderer::createDescriptorPool()
{
    VkDescriptorPoolSize sizes[] = {
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         1 },  // palette
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         1 },  // light table
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 },  // texture atlas
    };
    VkDescriptorPoolCreateInfo ci{};
    ci.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    ci.maxSets       = 1;
    ci.poolSizeCount = 3;
    ci.pPoolSizes    = sizes;
    VR_CHECK(vkCreateDescriptorPool(m_device, &ci, nullptr, &m_descPool));
    return true;
}

bool VulkanRenderer::createDescriptorSetLayout()
{
    VkDescriptorSetLayoutBinding bindings[] = {
        // binding 0: palette UBO
        { 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
          1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
        // binding 1: light table SSBO
        { 1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
          1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
        // binding 2: texture atlas + sampler
        { 2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
          1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
    };
    VkDescriptorSetLayoutCreateInfo ci{};
    ci.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    ci.bindingCount = 3;
    ci.pBindings    = bindings;
    VR_CHECK(vkCreateDescriptorSetLayout(m_device, &ci, nullptr, &m_descSetLayout));
    return true;
}

bool VulkanRenderer::createDescriptorSets()
{
    VkDescriptorSetAllocateInfo ai{};
    ai.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    ai.descriptorPool     = m_descPool;
    ai.descriptorSetCount = 1;
    ai.pSetLayouts        = &m_descSetLayout;
    VR_CHECK(vkAllocateDescriptorSets(m_device, &ai, &m_descSet));

    // Palette buffer
    VkDescriptorBufferInfo palBI{ m_paletteBuf.handle, 0, VK_WHOLE_SIZE };
    // Light table buffer
    VkDescriptorBufferInfo ltBI { m_lightTableBuf.handle, 0, VK_WHOLE_SIZE };
    // Atlas
    VkDescriptorImageInfo atlasII{
        m_sampler,
        m_textureAtlas.view,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    };

    VkWriteDescriptorSet writes[3] = {};
    writes[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[0].dstSet          = m_descSet;
    writes[0].dstBinding      = 0;
    writes[0].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writes[0].descriptorCount = 1;
    writes[0].pBufferInfo     = &palBI;

    writes[1].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[1].dstSet          = m_descSet;
    writes[1].dstBinding      = 1;
    writes[1].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    writes[1].descriptorCount = 1;
    writes[1].pBufferInfo     = &ltBI;

    writes[2].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[2].dstSet          = m_descSet;
    writes[2].dstBinding      = 2;
    writes[2].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writes[2].descriptorCount = 1;
    writes[2].pImageInfo      = &atlasII;

    vkUpdateDescriptorSets(m_device, 3, writes, 0, nullptr);
    return true;
}

bool VulkanRenderer::createRenderPass()
{
    VkAttachmentDescription att{};
    att.format         = VK_FORMAT_R8_UINT;
    att.samples        = VK_SAMPLE_COUNT_1_BIT;
    att.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
    att.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
    att.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
    att.finalLayout    = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;

    VkAttachmentReference ref{ 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

    VkSubpassDescription sub{};
    sub.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
    sub.colorAttachmentCount = 1;
    sub.pColorAttachments    = &ref;

    VkSubpassDependency dep{};
    dep.srcSubpass    = VK_SUBPASS_EXTERNAL;
    dep.dstSubpass    = 0;
    dep.srcStageMask  = VK_PIPELINE_STAGE_TRANSFER_BIT;
    dep.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dep.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    dep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo ci{};
    ci.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    ci.attachmentCount = 1;
    ci.pAttachments    = &att;
    ci.subpassCount    = 1;
    ci.pSubpasses      = &sub;
    ci.dependencyCount = 1;
    ci.pDependencies   = &dep;
    VR_CHECK(vkCreateRenderPass(m_device, &ci, nullptr, &m_renderPass));
    return true;
}

bool VulkanRenderer::createFramebuffer()
{
    VkFramebufferCreateInfo ci{};
    ci.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    ci.renderPass      = m_renderPass;
    ci.attachmentCount = 1;
    ci.pAttachments    = &m_renderTarget.view;
    ci.width           = m_screenW;
    ci.height          = m_screenH;
    ci.layers          = 1;
    VR_CHECK(vkCreateFramebuffer(m_device, &ci, nullptr, &m_framebuffer));
    return true;
}

bool VulkanRenderer::createPipelines()
{
    // Pipeline layout with push constants
    VkPushConstantRange pcRange{};
    pcRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pcRange.offset     = 0;
    pcRange.size       = sizeof(VR_PushConstants);

    VkPipelineLayoutCreateInfo layoutCI{};
    layoutCI.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutCI.setLayoutCount         = 1;
    layoutCI.pSetLayouts            = &m_descSetLayout;
    layoutCI.pushConstantRangeCount = 1;
    layoutCI.pPushConstantRanges    = &pcRange;
    VR_CHECK(vkCreatePipelineLayout(m_device, &layoutCI, nullptr, &m_pipelineLayout));

    // Vertex input: VR_Vertex
    VkVertexInputBindingDescription binding{};
    binding.binding   = 0;
    binding.stride    = sizeof(VR_Vertex);
    binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    VkVertexInputAttributeDescription attrs[4] = {};
    attrs[0] = { 0, 0, VK_FORMAT_R32G32_SFLOAT,       offsetof(VR_Vertex, x)      };  // position
    attrs[1] = { 1, 0, VK_FORMAT_R32G32_SFLOAT,       offsetof(VR_Vertex, u)      };  // uv
    attrs[2] = { 2, 0, VK_FORMAT_R32_UINT,             offsetof(VR_Vertex, shade)  };  // shade
    attrs[3] = { 3, 0, VK_FORMAT_R32_UINT,             offsetof(VR_Vertex, colour) };  // colour

    VkPipelineVertexInputStateCreateInfo vis{};
    vis.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vis.vertexBindingDescriptionCount   = 1;
    vis.pVertexBindingDescriptions      = &binding;
    vis.vertexAttributeDescriptionCount = 4;
    vis.pVertexAttributeDescriptions    = attrs;

    VkPipelineInputAssemblyStateCreateInfo ias{};
    ias.sType    = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    ias.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkViewport vp{ 0, 0, (float)m_screenW, (float)m_screenH, 0.f, 1.f };
    VkRect2D   sc{ {0,0}, {m_screenW, m_screenH} };
    VkPipelineViewportStateCreateInfo vs{};
    vs.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    vs.viewportCount = 1; vs.pViewports = &vp;
    vs.scissorCount  = 1; vs.pScissors  = &sc;

    VkPipelineRasterizationStateCreateInfo rs{};
    rs.sType       = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rs.polygonMode = VK_POLYGON_MODE_FILL;
    rs.cullMode    = VK_CULL_MODE_NONE;           // engine does no back-face culling
    rs.frontFace   = VK_FRONT_FACE_CLOCKWISE;     // match original screen-space winding
    rs.lineWidth   = 1.f;

    VkPipelineMultisampleStateCreateInfo ms{};
    ms.sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    ms.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    // All pipelines write to R8_UINT; blending on an integer format must be
    // disabled at the VkPipeline level — the "blending" logic lives entirely
    // in the fragment shader via the light-table / shade-table lookups.
    VkPipelineColorBlendAttachmentState cba{};
    cba.colorWriteMask = VK_COLOR_COMPONENT_R_BIT;
    // blendEnable = VK_FALSE for all pipelines (shader does it)

    VkPipelineColorBlendStateCreateInfo cbs{};
    cbs.sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    cbs.attachmentCount = 1;
    cbs.pAttachments    = &cba;

    // Create two shader modules: "solid" and "textured"
    // (In a production build these would be multiple specialised fragment shaders,
    //  but a single fragment shader with a specialisation constant / push-constant
    //  blend-mode selector is cleaner and equally fast on modern GPU drivers.)
    VkShaderModule vertSolid = createShaderModule(
        SolidFill_vert_spv, sizeof(SolidFill_vert_spv));
    VkShaderModule fragSolid = createShaderModule(
        SolidFill_frag_spv, sizeof(SolidFill_frag_spv));
    VkShaderModule vertTex   = createShaderModule(
        Textured_vert_spv,  sizeof(Textured_vert_spv));
    VkShaderModule fragTex   = createShaderModule(
        Textured_frag_spv,  sizeof(Textured_frag_spv));

    auto makeStage = [](VkShaderStageFlagBits stage, VkShaderModule mod)
    {
        VkPipelineShaderStageCreateInfo s{};
        s.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        s.stage  = stage;
        s.module = mod;
        s.pName  = "main";
        return s;
    };

    VkPipelineShaderStageCreateInfo solidStages[] = {
        makeStage(VK_SHADER_STAGE_VERTEX_BIT,   vertSolid),
        makeStage(VK_SHADER_STAGE_FRAGMENT_BIT, fragSolid),
    };
    VkPipelineShaderStageCreateInfo texStages[] = {
        makeStage(VK_SHADER_STAGE_VERTEX_BIT,   vertTex),
        makeStage(VK_SHADER_STAGE_FRAGMENT_BIT, fragTex),
    };

    VkGraphicsPipelineCreateInfo pipeCI{};
    pipeCI.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeCI.layout              = m_pipelineLayout;
    pipeCI.renderPass          = m_renderPass;
    pipeCI.subpass             = 0;
    pipeCI.pVertexInputState   = &vis;
    pipeCI.pInputAssemblyState = &ias;
    pipeCI.pViewportState      = &vs;
    pipeCI.pRasterizationState = &rs;
    pipeCI.pMultisampleState   = &ms;
    pipeCI.pColorBlendState    = &cbs;

    // SolidFill pipeline
    pipeCI.stageCount = 2;
    pipeCI.pStages    = solidStages;
    VR_CHECK(vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipeCI,
                                       nullptr,
                                       &m_pipelines[(int)PipelineType::SolidFill]));
    // GradientFill (same shader, different push constant blendMode interpreted in frag)
    VR_CHECK(vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipeCI,
                                       nullptr,
                                       &m_pipelines[(int)PipelineType::GradientFill]));

    // All textured pipelines share the same vertex/fragment shader;
    // the fragment shader reads blendMode from the push constants.
    pipeCI.stageCount = 2;
    pipeCI.pStages    = texStages;
    static const PipelineType texPipes[] = {
        PipelineType::TexturedOpaque,
        PipelineType::TexturedShaded,
        PipelineType::TexturedKeyTrans,
        PipelineType::TexturedDarkBlend,
        PipelineType::TexturedScreenBlend,
        PipelineType::TexturedDualTable,
        PipelineType::TexturedTransScreen,
        PipelineType::TexturedTransDual,
        PipelineType::ReflectionBlend,
    };
    for (auto pt : texPipes)
    {
        VR_CHECK(vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipeCI,
                                           nullptr, &m_pipelines[(int)pt]));
    }

    vkDestroyShaderModule(m_device, vertSolid, nullptr);
    vkDestroyShaderModule(m_device, fragSolid, nullptr);
    vkDestroyShaderModule(m_device, vertTex,   nullptr);
    vkDestroyShaderModule(m_device, fragTex,   nullptr);

    return true;
}

bool VulkanRenderer::createSyncObjects()
{
    VkFenceCreateInfo fi{ VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
    fi.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    VR_CHECK(vkCreateFence(m_device, &fi, nullptr, &m_fence));
    return true;
}

// ===========================================================================
// Palette / light-table upload
// ===========================================================================

void VulkanRenderer::uploadPalette(const uint32_t* rgba8Palette, uint32_t count)
{
    assert(count <= VR_PALETTE_ENTRIES);
    memcpy(m_paletteBuf.mapped, rgba8Palette, count * sizeof(uint32_t));
}

void VulkanRenderer::uploadLightTable(const uint8_t* table, uint32_t sizeBytes)
{
    assert(sizeBytes <= m_lightTableBuf.size);
    memcpy(m_lightTableBuf.mapped, table, sizeBytes);
}

// ===========================================================================
// Texture upload
// ===========================================================================

uint32_t VulkanRenderer::uploadTexture(const uint8_t* pixels, uint32_t w, uint32_t h)
{
    // Simple shelf packing into the atlas
    if (m_atlasNextSlotX + w > VR_TEXTURE_ATLAS_DIM)
    {
        m_atlasNextSlotX = 0;
        m_atlasNextSlotY += m_atlasRowHeight;
        m_atlasRowHeight = 0;
    }
    if (m_atlasNextSlotY + h > VR_TEXTURE_ATLAS_DIM)
    {
        fprintf(stderr, "[VR] Texture atlas full!\n");
        return 0;
    }

    const uint32_t slotX = m_atlasNextSlotX;
    const uint32_t slotY = m_atlasNextSlotY;

    // Upload via staging buffer
    VR_Buffer staging;
    createBuffer((VkDeviceSize)w * h,
                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 staging);
    memcpy(staging.mapped, pixels, (size_t)w * h);

    // One-shot command buffer
    VkCommandBufferAllocateInfo ai{};
    ai.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    ai.commandPool        = m_cmdPool;
    ai.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    ai.commandBufferCount = 1;
    VkCommandBuffer cb;
    vkAllocateCommandBuffers(m_device, &ai, &cb);

    VkCommandBufferBeginInfo bi{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
    bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(cb, &bi);

    transitionImageLayout(cb, m_textureAtlas.handle,
                          VK_IMAGE_LAYOUT_UNDEFINED,
                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    VkBufferImageCopy region{};
    region.imageSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
    region.imageOffset      = { (int32_t)slotX, (int32_t)slotY, 0 };
    region.imageExtent      = { w, h, 1 };
    vkCmdCopyBufferToImage(cb, staging.handle, m_textureAtlas.handle,
                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    transitionImageLayout(cb, m_textureAtlas.handle,
                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                          VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkEndCommandBuffer(cb);

    VkSubmitInfo si{ VK_STRUCTURE_TYPE_SUBMIT_INFO };
    si.commandBufferCount = 1;
    si.pCommandBuffers    = &cb;
    vkQueueSubmit(m_graphicsQueue, 1, &si, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_graphicsQueue);
    vkFreeCommandBuffers(m_device, m_cmdPool, 1, &cb);
    destroyBuffer(staging);

    // Encode atlas slot as packed (slotX | slotY<<12 | w<<24) — limited to
    // 4096 atlas, 256-wide textures.  Caller stores this as textureIndex.
    const uint32_t atlasSlot = slotX | (slotY << 12) | (w << 24);

    m_atlasNextSlotX += w;
    m_atlasRowHeight  = std::max(m_atlasRowHeight, h);

    return atlasSlot;
}

// ===========================================================================
// Frame management
// ===========================================================================

void VulkanRenderer::beginFrame(uint32_t vpX, uint32_t vpY,
                                uint32_t vpW, uint32_t vpH)
{
    // Wait for previous frame to finish
    vkWaitForFences(m_device, 1, &m_fence, VK_TRUE, UINT64_MAX);
    vkResetFences(m_device, 1, &m_fence);

    m_vpX = vpX; m_vpY = vpY; m_vpW = vpW; m_vpH = vpH;

    m_vtxCount = 0;
    m_idxCount = 0;
    m_drawCalls.clear();
    m_batchOpen = false;

    // Reset command buffer
    vkResetCommandBuffer(m_cmdBuf, 0);

    VkCommandBufferBeginInfo bi{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
    bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(m_cmdBuf, &bi);
}

void VulkanRenderer::endFrameAndReadback(uint8_t* cpuBuffer)
{
    // Close open batch
    if (m_batchOpen) flushBatch();

    // Begin render pass
    VkClearValue clearVal{};
    clearVal.color.uint32[0] = 0;  // palette index 0 = background colour

    VkRenderPassBeginInfo rpBI{};
    rpBI.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rpBI.renderPass        = m_renderPass;
    rpBI.framebuffer       = m_framebuffer;
    rpBI.renderArea.extent = { m_screenW, m_screenH };
    rpBI.clearValueCount   = 1;
    rpBI.pClearValues      = &clearVal;

    vkCmdBeginRenderPass(m_cmdBuf, &rpBI, VK_SUBPASS_CONTENTS_INLINE);

    // Set viewport/scissor to the engine's current viewport
    VkViewport vp{ (float)m_vpX, (float)m_vpY,
                   (float)m_vpW, (float)m_vpH, 0.f, 1.f };
    VkRect2D   sc{ { (int32_t)m_vpX, (int32_t)m_vpY }, { m_vpW, m_vpH } };
    vkCmdSetViewport(m_cmdBuf, 0, 1, &vp);
    vkCmdSetScissor (m_cmdBuf, 0, 1, &sc);

    // Bind vertex / index buffers
    VkDeviceSize offset = 0;
    vkCmdBindVertexBuffers(m_cmdBuf, 0, 1, &m_vertexBuf.handle, &offset);
    vkCmdBindIndexBuffer  (m_cmdBuf, m_indexBuf.handle, 0, VK_INDEX_TYPE_UINT32);
    vkCmdBindDescriptorSets(m_cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS,
                            m_pipelineLayout, 0, 1, &m_descSet, 0, nullptr);

    VkPipeline activePipeline = VK_NULL_HANDLE;

    for (const auto& dc : m_drawCalls)
    {
        const auto pt = blendModeToPipeline(dc.blendMode);
        VkPipeline pipe = m_pipelines[(int)pt];
        if (pipe != activePipeline)
        {
            vkCmdBindPipeline(m_cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, pipe);
            activePipeline = pipe;
        }

        VR_PushConstants pc{};
        pc.viewportW      = (float)m_vpW;
        pc.viewportH      = (float)m_vpH;
        pc.blendMode      = dc.blendMode;
        pc.textureIndex   = dc.textureIndex;
        pc.lightTableBase = dc.lightTableBase;
        vkCmdPushConstants(m_cmdBuf, m_pipelineLayout,
                           VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                           0, sizeof(pc), &pc);

        vkCmdDrawIndexed(m_cmdBuf, dc.indexCount, 1, dc.firstIndex, 0, 0);
    }

    vkCmdEndRenderPass(m_cmdBuf);

    // Copy render target → readback buffer
    // (render pass already transitioned image to TRANSFER_SRC_OPTIMAL)
    VkBufferImageCopy copyRegion{};
    copyRegion.imageSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
    copyRegion.imageExtent      = { m_screenW, m_screenH, 1 };
    vkCmdCopyImageToBuffer(m_cmdBuf,
                           m_renderTarget.handle,
                           VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                           m_readbackBuf.handle,
                           1, &copyRegion);

    vkEndCommandBuffer(m_cmdBuf);

    VkSubmitInfo si{ VK_STRUCTURE_TYPE_SUBMIT_INFO };
    si.commandBufferCount = 1;
    si.pCommandBuffers    = &m_cmdBuf;
    VR_CHECK_V(vkQueueSubmit(m_graphicsQueue, 1, &si, m_fence));

    // Wait and copy to CPU buffer
    vkWaitForFences(m_device, 1, &m_fence, VK_TRUE, UINT64_MAX);
    memcpy(cpuBuffer, m_readbackBuf.mapped, (size_t)m_screenW * m_screenH);
}

// ===========================================================================
// Draw call submission
// ===========================================================================

void VulkanRenderer::submitTriangle(const VR_Vertex& v0,
                                    const VR_Vertex& v1,
                                    const VR_Vertex& v2,
                                    uint32_t blendMode,
                                    uint32_t textureIndex,
                                    uint32_t lightTableBase)
{
    if (m_vtxCount + 3 > VR_MAX_VERTICES_PER_FRAME ||
        m_idxCount + 3 > VR_MAX_INDICES_PER_FRAME)
    {
        fprintf(stderr, "[VR] Vertex/index buffer overflow — frame dropped\n");
        return;
    }

    // Check if we can append to the current batch
    const bool sameBatch = m_batchOpen
        && m_currentBatch.blendMode      == blendMode
        && m_currentBatch.textureIndex   == textureIndex
        && m_currentBatch.lightTableBase == lightTableBase;

    if (!sameBatch)
    {
        if (m_batchOpen) flushBatch();
        m_currentBatch.firstIndex    = m_idxCount;
        m_currentBatch.indexCount    = 0;
        m_currentBatch.blendMode     = blendMode;
        m_currentBatch.textureIndex  = textureIndex;
        m_currentBatch.lightTableBase= lightTableBase;
        m_batchOpen = true;
    }

    const uint32_t base = m_vtxCount;
    m_vtxPtr[m_vtxCount++] = v0;
    m_vtxPtr[m_vtxCount++] = v1;
    m_vtxPtr[m_vtxCount++] = v2;
    m_idxPtr[m_idxCount++] = base;
    m_idxPtr[m_idxCount++] = base + 1;
    m_idxPtr[m_idxCount++] = base + 2;
    m_currentBatch.indexCount += 3;
}

void VulkanRenderer::submitRect(int x, int y, int w, int h, uint8_t colIdx)
{
    const float fx = (float)x, fy = (float)y;
    const float fw = (float)w, fh = (float)h;

    VR_Vertex v{};
    v.colour = colIdx;

    VR_Vertex tl = v; tl.x = fx;      tl.y = fy;
    VR_Vertex tr = v; tr.x = fx + fw; tr.y = fy;
    VR_Vertex br = v; br.x = fx + fw; br.y = fy + fh;
    VR_Vertex bl = v; bl.x = fx;      bl.y = fy + fh;

    submitTriangle(tl, tr, br, 0, 0xFFFF, 0);
    submitTriangle(tl, br, bl, 0, 0xFFFF, 0);
}

void VulkanRenderer::submitSprite(int screenX,  int screenY,
                                  int screenW,  int screenH,
                                  int srcU,     int srcV,
                                  int srcW,     int srcH,
                                  uint32_t textureIndex,
                                  uint32_t blendMode,
                                  uint32_t shadeIndex)
{
    const float fx  = (float)screenX;
    const float fy  = (float)screenY;
    const float fw  = (float)screenW;
    const float fh  = (float)screenH;
    const float fu  = (float)srcU;
    const float fv  = (float)srcV;
    const float fuw = (float)srcW;
    const float fvh = (float)srcH;

    VR_Vertex tl, tr, br, bl;
    tl.x = fx;      tl.y = fy;      tl.u = fu;       tl.v = fv;
    tr.x = fx + fw; tr.y = fy;      tr.u = fu + fuw;  tr.v = fv;
    br.x = fx + fw; br.y = fy + fh; br.u = fu + fuw;  br.v = fv + fvh;
    bl.x = fx;      bl.y = fy + fh; bl.u = fu;        bl.v = fv + fvh;
    tl.shade = tr.shade = br.shade = bl.shade = shadeIndex;
    tl.colour = tr.colour = br.colour = bl.colour = 0;

    submitTriangle(tl, tr, br, blendMode, textureIndex, 0);
    submitTriangle(tl, br, bl, blendMode, textureIndex, 0);
}

// ===========================================================================
// Internal helpers
// ===========================================================================

void VulkanRenderer::flushBatch()
{
    if (!m_batchOpen) return;
    m_drawCalls.push_back(m_currentBatch);
    m_batchOpen = false;
}

void VulkanRenderer::destroyBuffer(VR_Buffer& buf)
{
    if (buf.handle != VK_NULL_HANDLE)
    {
        if (buf.mapped) vkUnmapMemory(m_device, buf.memory);
        vkDestroyBuffer(m_device, buf.handle, nullptr);
        vkFreeMemory   (m_device, buf.memory, nullptr);
        buf = {};
    }
}

void VulkanRenderer::destroyImage(VR_Image& img)
{
    if (img.view   != VK_NULL_HANDLE) vkDestroyImageView(m_device, img.view, nullptr);
    if (img.handle != VK_NULL_HANDLE) vkDestroyImage    (m_device, img.handle, nullptr);
    if (img.memory != VK_NULL_HANDLE) vkFreeMemory      (m_device, img.memory, nullptr);
    img = {};
}

uint32_t VulkanRenderer::findMemoryType(uint32_t typeFilter,
                                        VkMemoryPropertyFlags props)
{
    VkPhysicalDeviceMemoryProperties mp;
    vkGetPhysicalDeviceMemoryProperties(m_physDevice, &mp);
    for (uint32_t i = 0; i < mp.memoryTypeCount; ++i)
    {
        if ((typeFilter & (1u << i)) &&
            (mp.memoryTypes[i].propertyFlags & props) == props)
            return i;
    }
    assert(false && "No suitable memory type");
    return UINT32_MAX;
}

bool VulkanRenderer::createBuffer(VkDeviceSize size,
                                  VkBufferUsageFlags usage,
                                  VkMemoryPropertyFlags props,
                                  VR_Buffer& out)
{
    VkBufferCreateInfo ci{};
    ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    ci.size  = size;
    ci.usage = usage;
    ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    VR_CHECK(vkCreateBuffer(m_device, &ci, nullptr, &out.handle));

    VkMemoryRequirements req;
    vkGetBufferMemoryRequirements(m_device, out.handle, &req);

    VkMemoryAllocateInfo ai{};
    ai.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    ai.allocationSize  = req.size;
    ai.memoryTypeIndex = findMemoryType(req.memoryTypeBits, props);
    VR_CHECK(vkAllocateMemory(m_device, &ai, nullptr, &out.memory));
    VR_CHECK(vkBindBufferMemory(m_device, out.handle, out.memory, 0));

    if (props & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
        VR_CHECK(vkMapMemory(m_device, out.memory, 0, size, 0, &out.mapped));

    out.size = size;
    return true;
}

bool VulkanRenderer::createImage(uint32_t w, uint32_t h,
                                 VkFormat fmt,
                                 VkImageUsageFlags usage,
                                 VkMemoryPropertyFlags props,
                                 VR_Image& out)
{
    VkImageCreateInfo ci{};
    ci.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    ci.imageType     = VK_IMAGE_TYPE_2D;
    ci.format        = fmt;
    ci.extent        = { w, h, 1 };
    ci.mipLevels     = 1;
    ci.arrayLayers   = 1;
    ci.samples       = VK_SAMPLE_COUNT_1_BIT;
    ci.tiling        = VK_IMAGE_TILING_OPTIMAL;
    ci.usage         = usage;
    ci.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;
    ci.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    VR_CHECK(vkCreateImage(m_device, &ci, nullptr, &out.handle));

    VkMemoryRequirements req;
    vkGetImageMemoryRequirements(m_device, out.handle, &req);

    VkMemoryAllocateInfo ai{};
    ai.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    ai.allocationSize  = req.size;
    ai.memoryTypeIndex = findMemoryType(req.memoryTypeBits, props);
    VR_CHECK(vkAllocateMemory(m_device, &ai, nullptr, &out.memory));
    VR_CHECK(vkBindImageMemory(m_device, out.handle, out.memory, 0));

    out.view   = createImageView(out.handle, fmt, VK_IMAGE_ASPECT_COLOR_BIT);
    out.format = fmt;
    out.width  = w;
    out.height = h;
    return true;
}

VkImageView VulkanRenderer::createImageView(VkImage image, VkFormat fmt,
                                            VkImageAspectFlags aspect)
{
    VkImageViewCreateInfo ci{};
    ci.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    ci.image                           = image;
    ci.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
    ci.format                          = fmt;
    ci.subresourceRange.aspectMask     = aspect;
    ci.subresourceRange.levelCount     = 1;
    ci.subresourceRange.layerCount     = 1;
    VkImageView view = VK_NULL_HANDLE;
    vkCreateImageView(m_device, &ci, nullptr, &view);
    return view;
}

VkShaderModule VulkanRenderer::createShaderModule(const uint32_t* spv,
                                                   size_t sizeBytes)
{
    VkShaderModuleCreateInfo ci{};
    ci.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    ci.codeSize = sizeBytes;
    ci.pCode    = spv;
    VkShaderModule mod = VK_NULL_HANDLE;
    vkCreateShaderModule(m_device, &ci, nullptr, &mod);
    return mod;
}

void VulkanRenderer::transitionImageLayout(VkCommandBuffer cmd,
                                           VkImage image,
                                           VkImageLayout oldLayout,
                                           VkImageLayout newLayout)
{
    VkImageMemoryBarrier barrier{};
    barrier.sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout           = oldLayout;
    barrier.newLayout           = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image               = image;
    barrier.subresourceRange    = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

    VkPipelineStageFlags srcStage, dstStage;
    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
        newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
             newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else
    {
        barrier.srcAccessMask = VK_ACCESS_MEMORY_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        srcStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
        dstStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
    }

    vkCmdPipelineBarrier(cmd, srcStage, dstStage, 0,
                         0, nullptr, 0, nullptr, 1, &barrier);
}
