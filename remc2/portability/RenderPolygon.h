#pragma once
#include <array>
#include <cstdint>
#include <vector>
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

// A frame's polygons as the game sends them to the renderer (E_RESOURCE_CHANGE / POLYGONS_UPDATED): one list per view,
// and how the views are laid out in the window. The window is split into rows x columns cells, each showing the whole
// frame (its view and the HUD): list 0 goes in the bottom left cell, then left to right and bottom to top. With any
// other number of lists than cells, the first list is shown in every cell.
//   1 x 1: the normal frame. 1 x 2: the VR (side by side) eyes, left then right. rows x columns: a Looking Glass quilt.
// With ViewComposition::Anaglyph the grid is 1 x 1 and the two lists are the left and the right eye, combined into the one
// cell through coloured filters (red / cyan glasses); a renderer that can't combine them shows the left eye.
enum class ViewComposition
{
	Cells,      // one list per cell
	Anaglyph,   // two lists (left, right eye) combined into every cell
};

struct RenderViews
{
	int rows = 1;
	int columns = 1;
	ViewComposition composition = ViewComposition::Cells;
	bool darkLevel = false;   // a night or cave level: the anaglyph shows the left eye as brightness (half-colour)
	std::vector<std::vector<RenderPolygon>> lists;
};
