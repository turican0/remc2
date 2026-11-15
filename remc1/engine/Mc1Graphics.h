#pragma once

#include <cstdint>

#include "../../remc2/portability/bitmap_pos_struct.h"

// MC1 palette and font helpers

extern uint8_t colorLookup_AD168_AD158[16 * 16 * 16]; // nearest palette color for 4 bit r, g, b
extern bitmap_pos_struct_t* currentFont_AE44C_AE43C;

uint8_t FindNearestColor_5CC70_5D180(const uint8_t* palette, uint8_t red, uint8_t green, uint8_t blue);
void SetFont_5A3C0_5A8D0(int16_t index);
uint16_t GetSpaceWidth_5A3A0_5A8B0(); // the width of the space of the current font
uint16_t GetLineHeight_5A3B0_5A8C0();
// the text with 0Ah new lines up to x 280h, returns x after it
int32_t DrawText_5A180_5A690(const char* text, int32_t posX, int32_t posY, uint8_t color);
