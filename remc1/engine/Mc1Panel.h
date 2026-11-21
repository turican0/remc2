#pragma once

#include <cstdint>

#include "Mc1GameState.h"

// MC1 frame icons (20FB0): the castle, balloons and wizard panel (22E50) and the spell icons (23D40)

// the castle with its life, mana and size, the balloons, the wizard; the flashes of pd+187h..189h counted down
void DrawPlayerPanel_22E50_22E50(Type_Mc1Entity* wizard);
// the spell of the slot: the mana of the owner per cast, the charge, the key number (pd+34Ch flash), darkened
// without castle mana
void DrawSpellIcon_23D40_23D40(int32_t x, int32_t y, Type_Mc1Entity* spell, int32_t selected);
// the spells mode: a spell not got (the shape of its icon over the cell)
void DrawSpellAbsent_23CF0_23CF0(int32_t x, int32_t y, int32_t model);
// the spells mode: the cell of a got spell with its key, darkened without castle mana
void DrawSpellCell_24230_24230(int32_t x, int32_t y, Type_Mc1Entity* spell, int32_t selected);
// the frame of the cell under the joystick in the spells mode (320x200 / 640x480)
void FrameRect320_24D20_24D20(int32_t x, int32_t y, int32_t width, int32_t height, int32_t color);
void FrameRect640_24DA0_24DA0(int32_t x, int32_t y, int32_t width, int32_t height, int32_t color);
// the map: the table of the players with their kills of each other (the mouse at the bottom)
void DrawPlayersTable_22880_22880();
