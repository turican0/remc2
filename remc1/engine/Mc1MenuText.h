#pragma once

#include <cstdint>

#include "../../remc2/portability/bitmap_pos_struct.h"

// MC1 menu text: a font of RLE glyphs drawn with two colors into a text window

#pragma pack(push, 1)
typedef struct { // 8 bytes in DOS
	bitmap_pos_struct_t* glyphs_0x00_0; // from char 0x20 (the tab + 6); port: an index with real pointers
	uint8_t flags_0x04_4; // 1 color 1, 2 color 2, 0x10 glyphs drawn as sprites
	uint8_t mode_0x05_5;  // 0x40 / 0x80 / 0xC0 skip the pixels
	uint8_t color1_0x06_6;
	uint8_t color2_0x07_7;
} Type_Mc1Font_96884;

typedef struct {
	int32_t left_0;
	int32_t right_4;
	int32_t top_8;
	int32_t bottom_12;
	int32_t width_16;
	int32_t height_20;
} Type_Mc1TextWindow_12C970;
#pragma pack(pop)

extern Type_Mc1Font_96884 menuFont_96884_96884;
extern Type_Mc1TextWindow_12C970 textWindow_12C970_12C960;
extern Type_Mc1TextWindow_12C970 savedTextWindow_12C988_12C978;
extern int32_t dword_12CA78_12CA68;

void SetTextWindow_51360_516A0(int32_t x, int32_t y, int32_t width, int32_t height);
void GetTextWindow_513A0_516E0(Type_Mc1TextWindow_12C970* window);
void SaveTextWindow_513E0_51720();
void RestoreTextWindow_51400_51740();
int32_t TextWidth_51420_51760(Type_Mc1Font_96884* font, const char* text);
void InitFont_51480_517C0(Type_Mc1Font_96884* font, bitmap_pos_struct_t* glyphIndex, uint8_t* palette);
void DrawText_51560_518A0(Type_Mc1Font_96884* font, int32_t x, int32_t y, const char* text);
void DrawGlyph_51650_51990(Type_Mc1Font_96884* font, int32_t x, int32_t y, bitmap_pos_struct_t* glyph);
