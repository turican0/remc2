#pragma once

#include <cstdint>

// MC1 minimap (20FB0): the terrain turned by the yaw of the camera (49300), then the entities over it (48710)

// the rows of the minimap: [y][0] the end, [y][1] the start (a circle in the game, the whole width in the map mode)
extern int16_t minimapRows_12C1F0_12C1E0[0x1E0][2];
// two palette colours of each player (the dots and the names)
extern const uint8_t playerColors_99B58_99B58[8][2];

// mode 0 the circle blended with the screen, else the rectangle; scale 100h = a pixel a tile
void DrawMinimapTerrain_49300_49640(int32_t screenX, int32_t screenY, int32_t worldX, int32_t worldY, int32_t width, int32_t height,
	int32_t yaw, int32_t scale, int32_t unused, int32_t mode);
// the line to the castle, the entities as dots and sprites, the names of the players (the spell 5), the cross
void DrawMinimapEntities_48710_48A50(int32_t screenX, int32_t screenY, int32_t worldX, int32_t worldY, int32_t width, int32_t height,
	int32_t yaw, int32_t scale, int32_t unused, int32_t mode);
