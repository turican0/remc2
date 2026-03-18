// Textured.frag
// Uber fragment shader for all textured blend modes (modes 2-1A).
//
// Blend mode is passed via push constant and selects the output formula.
// The light/shade tables (x_BYTE_F6EE0_tablesx in the original engine) are
// uploaded as a storage buffer and indexed identically to the CPU rasteriser.
//
// Table layout (mirrors x_BYTE_F6EE0_tablesx[]):
//   offset      0 : shade table  — shadeTable[shadeIdx * 256 + colourIdx]
//   offset  16384 : blend table  — blendTable[dest * 256 + src]   (dark blend)
//   offset  32768 : reflect table — reflTable[src * 256 + dest]   (reflection)
//
// The render target is VK_FORMAT_R8_UINT; we output palette indices directly.
// The sampler reads the atlas as R8_UINT so texture() returns a uvec4 with
// the palette index in .r.
#version 450
#extension GL_EXT_samplerless_texture_functions : enable

layout(binding = 0) uniform PaletteBuf {
    uint palette[256];  // unused in 8-bit pipeline; reserved for potential RGBA output
} pal;

layout(std430, binding = 1) readonly buffer LightTable {
    uint data[];
} lt;

layout(binding = 2) uniform usampler2D texAtlas;   // R8_UINT atlas

layout(push_constant) uniform PC {
    float viewportW;
    float viewportH;
    uint  blendMode;
    uint  textureIndex;
    uint  lightTableBase;
    uint  pad[3];
} pc;

layout(location = 0) in      vec2  fragUV;
layout(location = 1) in flat uint  fragShade;
layout(location = 2) in flat uint  fragBlend;
layout(location = 3) in flat uint  fragTexIndex;

layout(location = 0) out uint outColour;

// Read one byte from the packed light-table buffer
uint ltByte(uint byteIdx) {
    uint word  = lt.data[byteIdx >> 2u];
    uint shift = (byteIdx & 3u) * 8u;
    return (word >> shift) & 0xFFu;
}

// Table base offsets (bytes)
const uint SHADE_BASE   =     0u;
const uint BLEND_BASE   = 16384u;
const uint REFLECT_BASE = 32768u;

// Sample the palette index from the atlas at the interpolated UV
uint sampleTex() {
    return texture(texAtlas, fragUV).r;
}

// Read the current destination pixel.
// In Vulkan we cannot read back from a colour attachment inside the same
// subpass.  For blend modes that require src+dst compositing (dark blend,
// reflection, etc.) we use a two-subpass setup *or* accept the approximation
// of using the shade value carried in the vertex as a proxy for the destination
// palette index.  The vertex shade field is set by the CPU geometry setup code
// to x_BYTE_E126C which holds the background luminance.
uint destApprox() {
    return fragShade & 0xFFu;
}

void main()
{
    uint src = sampleTex();
    uint mode = fragBlend;

    // Key colour: palette index 0 is always transparent in modes that test it.
    // Early discard for transparency modes.
    bool keyTransparent = (src == 0u);

    switch (mode)
    {
    // -----------------------------------------------------------------------
    // Mode 2/3: Textured opaque (no transparency, no shading)
    //   Output the raw palette index from the texture.
    // -----------------------------------------------------------------------
    case 0x02u:
    case 0x03u:
        outColour = src;
        break;

    // -----------------------------------------------------------------------
    // Mode 5/6: Textured + light-table shade (shadeTable[shade][src])
    //   Equivalent to x_BYTE_F6EE0_tablesx[shadeIdx * 256 + paletteIdx]
    //   in the original code (table offset 0).
    // -----------------------------------------------------------------------
    case 0x05u:
    case 0x06u: {
        uint shadeRow = (fragShade >> 8u) & 0xFFu;  // hi-byte of shade field
        outColour = ltByte(SHADE_BASE + shadeRow * 256u + src);
        break;
    }

    // -----------------------------------------------------------------------
    // Mode 7/B: Textured + flat shade (same as 5/6 but shade from x_BYTE_E126C)
    // -----------------------------------------------------------------------
    case 0x07u:
    case 0x0Bu: {
        uint shadeRow = fragShade & 0xFFu;
        outColour = ltByte(SHADE_BASE + shadeRow * 256u + src);
        break;
    }

    // -----------------------------------------------------------------------
    // Mode 8: Textured + key-colour transparency (index 0 = transparent)
    //   Equivalent to: if (src != 0) *dst = src
    // -----------------------------------------------------------------------
    case 0x08u:
        if (keyTransparent) discard;
        outColour = src;
        break;

    // -----------------------------------------------------------------------
    // Mode 9/A: Textured + transparent + light-table shade
    //   if (src != 0) *dst = shadeTable[shade][src]
    // -----------------------------------------------------------------------
    case 0x09u:
    case 0x0Au: {
        if (keyTransparent) discard;
        uint shadeRow = fragShade & 0xFFu;
        outColour = ltByte(SHADE_BASE + shadeRow * 256u + src);
        break;
    }

    // -----------------------------------------------------------------------
    // Mode C/D: Dark blend — blendTable[dest][src]
    //   Equivalent to: *dst = x_BYTE_F6EE0_tablesx[16384 + (dst << 8) + src]
    //   We approximate dst with destApprox() (fragShade carries background idx).
    // -----------------------------------------------------------------------
    case 0x0Cu:
    case 0x0Du: {
        uint dst = destApprox();
        outColour = ltByte(BLEND_BASE + dst * 256u + src);
        break;
    }

    // -----------------------------------------------------------------------
    // Mode 12/13: Screen blend (textured)
    //   *dst = blendTable[blendTable[dst][src] * 256 + src]
    //   (double lookup for screen-like compositing)
    // -----------------------------------------------------------------------
    case 0x12u:
    case 0x13u: {
        uint dst  = destApprox();
        uint mid  = ltByte(BLEND_BASE + dst * 256u + src);
        outColour = ltByte(BLEND_BASE + mid * 256u + src);
        break;
    }

    // -----------------------------------------------------------------------
    // Mode 14/15: Dual-table blend (textured + transparent dual)
    //   First pass through shade table, then blend table.
    //   if (src != 0) *dst = blendTable[dst][ shadeTable[shade][src] ]
    // -----------------------------------------------------------------------
    case 0x14u:
    case 0x15u: {
        if (keyTransparent) discard;
        uint shadeRow = fragShade & 0xFFu;
        uint shaded   = ltByte(SHADE_BASE + shadeRow * 256u + src);
        uint dst      = destApprox();
        outColour     = ltByte(BLEND_BASE + dst * 256u + shaded);
        break;
    }

    // -----------------------------------------------------------------------
    // Mode 16/17: Transparent screen blend
    //   if (src != 0) *dst = blendTable[dst][ blendTable[dst][src] ]
    // -----------------------------------------------------------------------
    case 0x16u:
    case 0x17u: {
        if (keyTransparent) discard;
        uint dst = destApprox();
        uint mid = ltByte(BLEND_BASE + dst * 256u + src);
        outColour = ltByte(BLEND_BASE + mid * 256u + src);
        break;
    }

    // -----------------------------------------------------------------------
    // Mode 18/19: Transparent dual-table blend
    //   if (src != 0) {
    //     shaded = shadeTable[shade][src]
    //     *dst   = blendTable[blendTable[dst][shaded]][shaded]
    //   }
    // -----------------------------------------------------------------------
    case 0x18u:
    case 0x19u: {
        if (keyTransparent) discard;
        uint shadeRow = fragShade & 0xFFu;
        uint shaded   = ltByte(SHADE_BASE + shadeRow * 256u + src);
        uint dst      = destApprox();
        uint mid      = ltByte(BLEND_BASE + dst    * 256u + shaded);
        outColour     = ltByte(BLEND_BASE + mid    * 256u + shaded);
        break;
    }

    // -----------------------------------------------------------------------
    // Mode 1A: Reflection blend
    //   if (src < 0x0C) *dst = blendTable[src_refl][dst]
    //   else            *dst = shadeTable[shade][src]
    //   (matches the CPU rasteriser branch in case 0x1A of DrawTriangle)
    // -----------------------------------------------------------------------
    case 0x1Au: {
        if (keyTransparent) discard;
        uint dst = destApprox();
        if (src < 0x0Cu) {
            outColour = ltByte(REFLECT_BASE + src * 256u + dst);
        } else {
            uint shadeRow = fragShade & 0xFFu;
            outColour = ltByte(SHADE_BASE + shadeRow * 256u + src);
        }
        break;
    }

    // -----------------------------------------------------------------------
    // Fallback: pass through raw palette index
    // -----------------------------------------------------------------------
    default:
        outColour = src;
        break;
    }
}
