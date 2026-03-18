// Textured.vert
// Vertex shader for all textured pipelines (terrain tiles, sprites, sky).
// Screen-pixel -> NDC conversion, UV pass-through.
#version 450

layout(push_constant) uniform PC {
    float viewportW;
    float viewportH;
    uint  blendMode;
    uint  textureIndex;   // packed: bits[11:0]=atlasX, bits[23:12]=atlasY, bits[31:24]=srcW
    uint  lightTableBase;
    uint  pad[3];
} pc;

layout(location = 0) in vec2  inPos;
layout(location = 1) in vec2  inUV;      // source texel coordinates (absolute, not normalised)
layout(location = 2) in uint  inShade;   // shade index (bits 0-7) + roll for sky (bits 0-10)
layout(location = 3) in uint  inColour;  // unused for textured modes

layout(location = 0) out vec2  fragUV;
layout(location = 1) out flat uint  fragShade;
layout(location = 2) out flat uint  fragBlend;
layout(location = 3) out flat uint  fragTexIndex;

// Atlas dimensions match VR_TEXTURE_ATLAS_DIM
const float ATLAS_DIM = 4096.0;

void main()
{
    float ndcX = (inPos.x / pc.viewportW) * 2.0 - 1.0;
    float ndcY = (inPos.y / pc.viewportH) * 2.0 - 1.0;
    gl_Position = vec4(ndcX, ndcY, 0.0, 1.0);

    // Decode atlas slot from textureIndex:
    //   bits[11: 0] = atlas slot X in pixels
    //   bits[23:12] = atlas slot Y in pixels
    //   bits[31:24] = source texture width (for UV wrap)
    float atlasX = float(pc.textureIndex & 0xFFFu);
    float atlasY = float((pc.textureIndex >> 12u) & 0xFFFu);

    // Absolute texel -> normalised atlas UV
    fragUV = (vec2(atlasX, atlasY) + inUV) / ATLAS_DIM;

    fragShade    = inShade;
    fragBlend    = pc.blendMode;
    fragTexIndex = pc.textureIndex;
}
