#pragma once

#include <cstdint>

// 30D90 / 30DD0: the MC1 world drawn by the renderer of remc2 (the scene of MC1 set into what the renderer reads)
void DrawWorld_30D90_30DD0(uint8_t* screen, int posX, int posY, int16_t yaw, int16_t posZ, int16_t pitch, int16_t roll, int16_t fov);
// the world from the camera of the current player, as 20FB0 calls 30D90 (the view port by the setting 2198h)
void DrawPlayerView();
void FreeMc1Renderer();
