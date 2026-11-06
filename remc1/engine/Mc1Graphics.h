#pragma once

#include <cstdint>

#include "../../remc2/portability/bitmap_pos_struct.h"

// MC1 palette and font helpers

extern uint8_t colorLookup_AD168_AD158[16 * 16 * 16]; // nearest palette color for 4 bit r, g, b
extern bitmap_pos_struct_t* currentFont_AE44C_AE43C;

uint8_t FindNearestColor_5CC70_5D180(const uint8_t* palette, uint8_t red, uint8_t green, uint8_t blue);
void SetFont_5A3C0_5A8D0(int16_t index);
