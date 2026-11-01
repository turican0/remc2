#version 450

// centroid (see polygon.vert): with MSAA the UV is taken inside the polygon, not at a pixel centre outside it
layout(location = 0) centroid in vec2  fragUV;
layout(location = 1) centroid in float fragShade;  // lighting row / 32 (terrain: 0..1.97, sprites: 0..1.25); fog is already in the row

layout(set = 0, binding = 0) uniform sampler2D  uPalette;    // 256x1 RGBA8
layout(set = 0, binding = 1) uniform usampler2D uIndexTex;   // R8_UINT, terrain atlas or stacked sprite frames
layout(set = 0, binding = 2) uniform usampler2D uBlendLut;   // 256x256, tint modes 4/5
layout(set = 0, binding = 3) uniform usampler2D uShadeLut;   // 256x64, x = texel, y = lighting row -> palette index

// The sky and the reflection, drawn in a pass of their own before the terrain: what the water lies over.
// Only the water reads it (anything bound while there is no reflection pass is never sampled).
layout(set = 1, binding = 0) uniform sampler2D uReflection;

layout(push_constant) uniform PC
{
	uint  mode;      // 255 = terrain, 26 = water terrain, 0..8 = original visibilityIdx
	uint  tint;      // dword0x07 low byte, used by modes 4/5; water: MapType_t (0 day, 1 night)
	float alpha;     // per-polygon alpha
	uint  flags;     // bits 8-15 = the sky (clear) colour index; bits 16-31 = sprite frames stacked in the texture
} pc;

// Hardware Settings "Bilinear filtering", set when the pipeline is built. The textures hold palette indices,
// which can't be filtered, so the four nearest texels are each lit and coloured and the colours are mixed.
layout(constant_id = 0) const bool BILINEAR = false;

layout(location = 0) out vec4 outColor;

const uint  MODE_TERRAIN = 255u;
const uint  MODE_SKY = 254u;
const uint  MODE_WATER   = 26u;
const uint  WATER_TEXELS = 12u;
const int   ATLAS_TILES_WIDE = 8;   // GameRenderHW::kTilesWide

const vec3  LUMA = vec3(0.299, 0.587, 0.114);

const vec3  DAY_WATER = vec3(0.810, 0.294, -4.4 / 255.0);
const int   DAY_RAMP_STEPS = 27;
const uint  DAY_RAMP[DAY_RAMP_STEPS - 1] = uint[](   // palette indices by brightness; the sky colour follows
	196u, 197u, 198u, 199u, 200u, 201u, 202u, 203u, 101u, 102u, 103u,
	1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u, 9u, 10u, 11u, 104u, 105u, 106u, 107u);

// Night: an ordinary blend, out = x * water + y * reflection + z (RGB rms error 6 of 255). The table maps a colour
// blended with itself to itself, so as the fog thickens (rows 32 -> 0) it eases to weights summing to 1.
const vec3  NIGHT_WATER = vec3(0.694, 0.314, 0.0);
const float NIGHT_WATER_FOG_ALPHA = 0.70;

vec3 palette(uint i)
{
	return texelFetch(uPalette, ivec2(int(i), 0), 0).rgb;
}

// The software renderer's lit[row << 8 | texel], with the two nearest rows mixed so the steps do not band
vec3 litColour(uint idx, float row)
{
	row = clamp(row, 0.0, 63.0);
	int  r0 = int(floor(row));
	int  r1 = min(r0 + 1, 63);
	vec3 a = palette(texelFetch(uShadeLut, ivec2(int(idx), r0), 0).r);
	vec3 b = palette(texelFetch(uShadeLut, ivec2(int(idx), r1), 0).r);
	return mix(a, b, fract(row));
}

// The day water ramp's colour at a brightness, mixed between its two nearest steps
vec3 dayRamp(float brightness)
{
	const uint sky = (pc.flags >> 8) & 0xFFu;
	vec3  prev = palette(DAY_RAMP[0]);
	float prevL = dot(prev, LUMA);
	if (brightness <= prevL)
		return prev;
	for (int i = 1; i < DAY_RAMP_STEPS; i++)
	{
		vec3  next = palette(i < DAY_RAMP_STEPS - 1 ? DAY_RAMP[i] : sky);
		float nextL = dot(next, LUMA);
		if (brightness <= nextL)
			return mix(prev, next, (brightness - prevL) / max(nextL - prevL, 1.0 / 255.0));
		prev = next;
		prevL = nextL;
	}
	return prev;   // brighter than the sky: the sky colour
}

// The water blended with the reflection under it, opaque
vec3 waterOverReflection(vec3 water, float row)
{
	vec3 reflection = texelFetch(uReflection, ivec2(gl_FragCoord.xy), 0).rgb;

	if (pc.tint == 0u)   // day
		return dayRamp(DAY_WATER.x * dot(water, LUMA) + DAY_WATER.y * dot(reflection, LUMA) + DAY_WATER.z);

	// night
	float fog = clamp(1.0 - row / 32.0, 0.0, 1.0);
	vec3  w = mix(NIGHT_WATER, vec3(NIGHT_WATER_FOG_ALPHA, 1.0 - NIGHT_WATER_FOG_ALPHA, 0.0), fog);
	return water * w.x + reflection * w.y + w.z;
}

// One texel as the polygon's mode draws it: colour and alpha (the pipeline blends colour * alpha + screen *
// (1 - alpha)); alpha 0 = transparent (sprite index 0)
vec4 shadeTexel(uint idx, float row, bool isTerrain, bool isSky)
{
	// ---- sky: tiles, opaque, no transparency ----
	if (isSky)
		return vec4(palette(idx), 1.0);

	// ---- terrain and water ----
	if (isTerrain)
	{
		vec3 c = litColour(idx, row);
		if (pc.mode == MODE_WATER && idx < WATER_TEXELS)
			c = waterOverReflection(c, row);
		return vec4(c, 1.0);
	}

	// ---- sprites: index 0 is transparent in every mode ----
	if (idx == 0u)
		return vec4(0.0);

	switch (pc.mode)
	{
	case 0u:   // texel as is
	case 2u:   // blended with the screen
	case 3u:
		return vec4(palette(idx), pc.alpha);

	case 1u:   // lit
	case 6u:   // blended, then lit
	case 7u:
		return vec4(litColour(idx, row), pc.alpha);

	case 4u:   // tint blend: LUT[tint << 8 | src]
		return vec4(palette(texelFetch(uBlendLut, ivec2(int(idx), int(pc.tint & 0xFFu)), 0).r), pc.alpha);

	case 5u:   // tint blend: LUT[src << 8 | tint]
		return vec4(palette(texelFetch(uBlendLut, ivec2(int(pc.tint & 0xFFu), int(idx)), 0).r), pc.alpha);

	case 8u:   // shadow: black, blended with the destination at pc.alpha
		return vec4(0.0, 0.0, 0.0, pc.alpha);

	default:
		return vec4(litColour(idx, row), pc.alpha);
	}
}

void main()
{
	const bool isTerrain = (pc.mode == MODE_TERRAIN) || (pc.mode == MODE_WATER);
	const bool isSky     = (pc.mode == MODE_SKY);

	ivec2 size = textureSize(uIndexTex, 0);
	vec2  uv   = isTerrain ? fragUV : fragUV * vec2(size);

	const float row = fragShade * 32.0;

	if (!BILINEAR)
	{
		ivec2 texel = ivec2(floor(uv));

		if (isSky)
			texel = ((texel % size) + size) % size;   // wrap, negatives included
		else
			texel = clamp(texel, ivec2(0), size - 1);

		uint idx = texelFetch(uIndexTex, texel, 0).r;

		if (!isTerrain && !isSky && idx == 0u)
			discard;

		outColor = shadeTexel(idx, row, isTerrain, isSky);
		return;
	}

	// The four taps stay inside the texel's own atlas tile or sprite frame, so neighbours don't bleed in; the sky wraps.
	ivec2 centre = clamp(ivec2(floor(uv)), ivec2(0), size - 1);
	ivec2 lo = ivec2(0);
	ivec2 hi = size - 1;
	if (isTerrain)
	{
		int tile = size.x / ATLAS_TILES_WIDE;
		lo = (centre / tile) * tile;
		hi = lo + tile - 1;
	}
	else if (!isSky)
	{
		int frameHeight = size.y / int(max(pc.flags >> 16, 1u));
		lo.y = (centre.y / frameHeight) * frameHeight;
		hi.y = lo.y + frameHeight - 1;
	}

	vec2  p = uv - 0.5;
	ivec2 base = ivec2(floor(p));
	vec2  f = p - floor(p);

	// Mixed weighted by alpha, so transparent sprite texels fade the edge instead of darkening it
	vec4 sum = vec4(0.0);
	for (int i = 0; i < 4; i++)
	{
		ivec2 offset = ivec2(i & 1, i >> 1);
		ivec2 texel = base + offset;
		if (isSky)
			texel = ((texel % size) + size) % size;
		else
			texel = clamp(texel, lo, hi);

		vec2  w2 = mix(1.0 - f, f, vec2(offset));
		float weight = w2.x * w2.y;

		vec4 s = shadeTexel(texelFetch(uIndexTex, texel, 0).r, row, isTerrain, isSky);
		sum += vec4(s.rgb * s.a, s.a) * weight;
	}

	if (sum.a <= 0.0)
		discard;

	outColor = vec4(sum.rgb / sum.a, sum.a);
}
