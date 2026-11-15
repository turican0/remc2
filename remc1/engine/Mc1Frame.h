#pragma once

#include <cstdint>

// MC1 drawing of the frame (20FB0): the world, the minimap, the icons, the texts, the help, the spells, the map, the credits

extern uint8_t screenCleared_90094_90094;
extern uint8_t clearedSetting_90096_90096;
extern int32_t dword_90098_90098;

// by the mode of the player: 0 and 3 the game, 1 the help, 2 the spells (and the map), 4 the map
void DrawFrame_20FB0_20FB0();
