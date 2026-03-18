// SolidFill.frag
// Fragment shader for blendMode 0/E/F (solid fill) and 1/4/10/11 (gradient).
//
// The render target is VK_FORMAT_R8_UINT (single-channel 8-bit integer).
// We output a palette index; no colour conversion happens on the GPU.
// The CPU readback gives the engine the same 8-bit indexed buffer it always had.
#version 450

// Binding 0: palette (256 x RGBA8) — unused in solid fill but declared for
// layout compatibility so all pipelines share the same descriptor set.
layout(binding = 0) uniform PaletteBuf {
    uint palette[256];
} pal;

// Binding 1: light/shade table (3 x 256 x 256 bytes as uint array)
layout(std430, binding = 1) readonly buffer LightTable {
    uint data[];  // packed: 4 bytes per uint, index as data[byte >> 2] >> ((byte & 3)*8)
} lt;

layout(push_constant) uniform PC {
    float viewportW;
    float viewportH;
    uint  blendMode;
    uint  textureIndex;
    uint  lightTableBase;
    uint  pad[3];
} pc;

layout(location = 0) in flat uint  fragPalIdx;
layout(location = 1) in flat uint  fragBlend;
layout(location = 2) in flat uint  fragShade;

// Integer colour attachment output
layout(location = 0) out uint outColour;

// Helper: read one byte from the packed light table buffer
uint ltByte(uint byteIndex) {
    uint word  = lt.data[byteIndex >> 2u];
    uint shift = (byteIndex & 3u) * 8u;
    return (word >> shift) & 0xFFu;
}

void main()
{
    uint idx = fragPalIdx;

    if (fragBlend == 0u || fragBlend == 0xEu || fragBlend == 0xFu)
    {
        // blendMode 0: solid fill — output palette index directly
        outColour = idx;
    }
    else
    {
        // blendMode 1/4/10/11: gradient fill
        // Apply shade table: shadeTable[shadeRow * 256 + colourIdx]
        // Shade table is at offset 0 in the light table buffer.
        uint shadeRow = fragShade & 0xFFu;
        outColour = ltByte(shadeRow * 256u + idx);
    }
}
