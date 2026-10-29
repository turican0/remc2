#include <cmath>
#include <cstdint>
#include <gtest/gtest.h>
#include "../../remc2/engine/GameUI.h"
#include "../../remc2/engine/read_config.h"
#include "../../remc2/utilities/Maths.h"

namespace
{
	struct MapCase
	{
		int width;
		int height;
		int scaling;
	};

	// minimap with gameUiScale 1-3, full screen map 1920x1080 (scale 2) and 640x480, low res
	const MapCase cases[] = { { 128, 128, 256 }, { 256, 256, 128 }, { 384, 384, 85 }, { 766, 800, 102 }, { 382, 400, 204 }, { 191, 200, 408 } };

	// World distance of a point from the position the terrain shows in the pixel its mark lands in
	double MarkError(const MapWalk& walk, int fracBits, const MapProjection& projection, const MapCase& c, int posX, int posY, int dx, int dy)
	{
		const int i = c.width / 2 + projection.X(dx, dy);
		const int j = c.height / 2 + projection.Y(dx, dy);
		const int64_t u = (int64_t)walk.startU + (int64_t)i * walk.du + (int64_t)j * walk.rowStepU;
		const int64_t v = (int64_t)walk.startV + (int64_t)i * walk.dv + (int64_t)j * walk.rowStepV;
		// the world wraps every 0x10000
		const int64_t mask = ((int64_t)0x10000 << fracBits) - 1;
		const int64_t eu = (((u - ((int64_t)(posX + dx) << fracBits)) & mask) ^ ((mask + 1) >> 1)) - ((mask + 1) >> 1);
		const int64_t ev = (((v - ((int64_t)(posY + dy) << fracBits)) & mask) ^ ((mask + 1) >> 1)) - ((mask + 1) >> 1);
		return std::hypot((double)eu, (double)ev) / (double)(1 << fracBits);
	}

	double MaxMarkError(bool fixed, const MapCase& c)
	{
		const int posX = 0x1234;
		const int posY = 0x6BCD;
		double maxError = 0;
		for (int yaw = 0; yaw < 2048; yaw += 13)
		{
			const int coefScale = 0x10000 / c.scaling;
			const MapProjection projection(
				(coefScale * Maths::sin_DB750[0x200 + yaw]) >> 16, -(coefScale * Maths::sin_DB750[yaw]) >> 16, yaw, c.scaling);
			const MapWalk walk = fixed ? GetFixedMapWalk(posX, posY, c.width, c.height, yaw, c.scaling) : GetMapWalk(posX, posY, c.width, c.height, yaw, c.scaling);
			const int fracBits = fixed ? MAP_WALK_FRAC_BITS : 0;
			const int reach = (std::min)(c.height / 2 * c.scaling * 9 / 10, 30000);
			for (int dy = -reach; dy <= reach; dy += reach / 7 + 1)
			{
				for (int dx = -reach; dx <= reach; dx += reach / 7 + 1)
				{
					const int i = c.width / 2 + projection.X(dx, dy);
					const int j = c.height / 2 + projection.Y(dx, dy);
					if (i < 0 || i >= c.width || j < 0 || j >= c.height)
						continue;
					maxError = (std::max)(maxError, MarkError(walk, fracBits, projection, c, posX, posY, dx, dy));
				}
			}
		}
		return maxError;
	}
}

// fixedMapDrawing: a mark lands in the pixel that shows its place, so it is off by half a pixel at most
TEST(MapProjection, FixedMarksMatchTerrain)
{
	for (const MapCase& c : cases)
	{
		const double halfPixelDiagonal = c.scaling * std::sqrt(0.5) + 1;
		EXPECT_LE(MaxMarkError(true, c), halfPixelDiagonal) << c.width << "x" << c.height << " scaling " << c.scaling;
	}
}

// The original drawing is kept for comparisons: its marks drift by several pixels on the full screen map
TEST(MapProjection, OriginalMarksDrift)
{
	const MapCase fullMap = { 766, 800, 102 };
	EXPECT_GT(MaxMarkError(false, fullMap), 10.0 * fullMap.scaling);
}
