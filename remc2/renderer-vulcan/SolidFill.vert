// SolidFill.vert
// Vertex shader shared by SolidFill and GradientFill pipelines.
// Input positions are in screen pixels; we convert to NDC here.
#version 450

// Push constants — same layout as VR_PushConstants in VulkanRenderer.h
layout(push_constant) uniform PC {
    float    viewportW;
    float    viewportH;
    uint     blendMode;
    uint     textureIndex;
    uint     lightTableBase;
    uint     pad[3];
} pc;

// Per-vertex inputs (VR_Vertex layout)
layout(location = 0) in vec2  inPos;     // screen-space pixels (x, y)
layout(location = 1) in vec2  inUV;      // unused for solid fill
layout(location = 2) in uint  inShade;   // packed shade data
layout(location = 3) in uint  inColour;  // palette index (bits 0-7)

// Interpolated outputs to fragment shader
layout(location = 0) out flat uint  fragPalIdx;   // solid palette index
layout(location = 1) out flat uint  fragBlend;    // blend mode
layout(location = 2) out flat uint  fragShade;    // shade level

void main()
{
    // Convert screen-pixel coordinates to Vulkan NDC [-1, 1]
    // Vulkan Y axis points down, matching the original engine's screen space.
    float ndcX = (inPos.x / pc.viewportW) * 2.0 - 1.0;
    float ndcY = (inPos.y / pc.viewportH) * 2.0 - 1.0;

    gl_Position = vec4(ndcX, ndcY, 0.0, 1.0);

    fragPalIdx = inColour & 0xFFu;
    fragBlend  = pc.blendMode;
    fragShade  = inShade & 0xFFu;
}
