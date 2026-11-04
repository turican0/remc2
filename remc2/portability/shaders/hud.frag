// hud.frag
#version 450

layout(location = 0) in vec2 inUV;
layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform usampler2DArray uHud;    // R8_UINT, layer 0 = palette index, layer 1 = alpha
layout(set = 0, binding = 1) uniform sampler2D uHudPalette;   // 256x1 RGBA8, the screen palette

layout(push_constant) uniform PushConstants
{
	ivec2 size;      // HUD size in texels
	int hasAlpha;    // 0 = no alpha buffer, draw opaque
} pc;

void main()
{
	// Nearest-neighbour scale of the game-resolution HUD to the window.
	ivec2 texel = min(ivec2(inUV * vec2(pc.size)), pc.size - 1);
	uint idx = texelFetch(uHud, ivec3(texel, 0), 0).r;
	float alpha = pc.hasAlpha != 0 ? float(texelFetch(uHud, ivec3(texel, 1), 0).r) / 255.0 : 1.0;
	outColor = vec4(texelFetch(uHudPalette, ivec2(int(idx), 0), 0).rgb, alpha);
}
