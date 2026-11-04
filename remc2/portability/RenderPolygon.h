#pragma once
#include <array>
#include <cstdint>
#include "../engine/ProjectionVertex.h"

struct HWVertex
{
	float X = 0.0f, Y = 0.0f, W = 1.0f;   // NDC + w
	float U = 0.0f, V = 0.0f;             // 0..1
	float Shade = 0.0f;                   // 0..1
	float Alpha = 1.0f;
	uint32_t Layer = 0;                   // texture array layer = textIndex_41
};

struct RenderPolygon
{
	std::array<HWVertex, 4> Vertices; // quad, fan order; held inline so a polygon needs no heap allocation
	uint32_t TextureId = 0;           // id returned by VulkanPolygonRenderer::UploadTexture
	uint8_t  SpriteMode = 0xFF;       // 0xFF = terrain; 0..8 = original visibilityIdx
	float Alpha = 1.0f;
	uint8_t  Tint = 0;                // dword0x07 low byte (modes 4/5)
	bool Reflection = false;          // drawn in the reflection pass, before the terrain: the water reads what it left
	uint16_t Frames = 1;              // sprites: animation frames stacked vertically in the texture (bilinear filtering keeps to one)
};
