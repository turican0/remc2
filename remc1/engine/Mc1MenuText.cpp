#include "Mc1MenuText.h"

#include "Mc1Graphics.h"
#include "Mc1Shared.h"

Type_Mc1Font_96884 menuFont_96884_96884 = { nullptr, 1, 0, 0, 0 };
Type_Mc1TextWindow_12C970 textWindow_12C970_12C960 = {};
Type_Mc1TextWindow_12C970 savedTextWindow_12C988_12C978 = {};
int32_t dword_12CA78_12CA68 = 0;

void SetTextWindow_51360_516A0(int32_t x, int32_t y, int32_t width, int32_t height)
{
	textWindow_12C970_12C960.left_0 = x;
	textWindow_12C970_12C960.right_4 = x + width;
	textWindow_12C970_12C960.top_8 = y;
	textWindow_12C970_12C960.bottom_12 = y + height;
	textWindow_12C970_12C960.width_16 = width;
	textWindow_12C970_12C960.height_20 = height;
}

void GetTextWindow_513A0_516E0(Type_Mc1TextWindow_12C970* window)
{
	*window = textWindow_12C970_12C960;
}

void SaveTextWindow_513E0_51720()
{
	dword_12CA78_12CA68 = 0;
	savedTextWindow_12C988_12C978 = textWindow_12C970_12C960;
}

void RestoreTextWindow_51400_51740()
{
	dword_12CA78_12CA68 = 0;
	textWindow_12C970_12C960 = savedTextWindow_12C988_12C978;
}

int32_t TextWidth_51420_51760(Type_Mc1Font_96884* font, const char* text)
{
	int16_t widest = 0;
	int16_t width = 0;
	for (const char* p = text; *p; p++)
	{
		int8_t c = (int8_t)*p; // 51439: cmp cl, 1Fh; jle (signed)
		if (c > 0x1F)
			width += font->glyphs_0x00_0[c - 0x20].width_4 - 1;
		else if (c == '\n')
		{
			if (width > widest)
				widest = width;
			width = 0;
		}
	}
	if (width > widest)
		widest = width;
	return widest;
}

void InitFont_51480_517C0(Type_Mc1Font_96884* font, bitmap_pos_struct_t* glyphIndex, uint8_t* palette)
{
	// 519E0 / 51D20 relocates the tab in place, the port gets the index built from it
	font->glyphs_0x00_0 = glyphIndex + 1;
	font->color1_0x06_6 = FindNearestColor_5CC70_5D180(palette, 0xFF, 0xFF, 0xFF);
	font->color2_0x07_7 = FindNearestColor_5CC70_5D180(palette, 0, 0, 0);
	font->flags_0x04_4 = 3; // 514D4: mov word ptr [ebx+4], 3
	font->mode_0x05_5 = 0;
}

void DrawText_51560_518A0(Type_Mc1Font_96884* font, int32_t x, int32_t y, const char* text)
{
	int32_t startX = x;
	if (!text)
		return;
	for (const uint8_t* p = (const uint8_t*)text; *p; )
	{
		uint8_t c = *p;
		if (c < 0x20)
		{
			switch (c)
			{
			case 1:
				font->color1_0x06_6 = *++p;
				break;
			case 2:
				font->color2_0x07_7 = *++p;
				break;
			case 3:
				font->flags_0x04_4 |= 3;
				break;
			case 4:
				font->flags_0x04_4 &= 0xFC;
				break;
			case 5:
				font->flags_0x04_4 |= 5;
				break;
			case 6:
				font->flags_0x04_4 &= 0xFA;
				break;
			case '\n':
				y += font->glyphs_0x00_0[0].height_5;
				x = startX;
				break;
			}
			p++;
			continue;
		}
		bitmap_pos_struct_t* glyph = &font->glyphs_0x00_0[c - 0x20];
		p++;
		if (font->flags_0x04_4 & 0x10)
		{
			if (c != ' ')
				DrawBitmap640_60D18_61228((int16_t)x, (int16_t)y, *glyph, 1);
			x += glyph->width_4 - 2;
		}
		else
		{
			DrawGlyph_51650_51990(font, (int16_t)x, (int16_t)y, glyph);
			x += glyph->width_4 - 1;
		}
	}
}

// the RLE rows of a glyph: n > 0 n pixels follow, n < 0 skip -n pixels, 0 ends the row
static const int8_t* SkipGlyphRow(const int8_t* src)
{
	for (;;)
	{
		int8_t c = *src;
		if (c > 0)
			src += c + 1;
		else
		{
			src++;
			if (c == 0)
				return src;
		}
	}
}

static void DrawGlyphPixels(Type_Mc1Font_96884* font, const int8_t* src, uint8_t* dest, uint8_t count)
{
	// pixel 1 takes color 1, pixel 2 color 2, the other values are transparent; modes 4000h-C000h draw nothing
	if (font->mode_0x05_5 & 0xC0)
		return;
	for (; count; count--, src++, dest++)
	{
		if ((uint8_t)*src == 1 && (font->flags_0x04_4 & 1))
			*dest = font->color1_0x06_6;
		else if ((uint8_t)*src == 2 && (font->flags_0x04_4 & 2))
			*dest = font->color2_0x07_7;
	}
}

void DrawGlyph_51650_51990(Type_Mc1Font_96884* font, int32_t x, int32_t y, bitmap_pos_struct_t* glyph)
{
	// 16-bit coordinates as in the original
	int16_t px = (int16_t)(x + textWindow_12C970_12C960.left_0);
	int16_t py = (int16_t)(y + textWindow_12C970_12C960.top_8);
	uint8_t width = glyph->width_4;
	uint8_t height = glyph->height_5;

	uint8_t leftCut = 0;
	int16_t over = (int16_t)(textWindow_12C970_12C960.left_0 - px);
	if (over > 0)
	{
		if (over >= width)
			return;
		leftCut = (uint8_t)over;
	}
	uint8_t visibleWidth = width;
	over = (int16_t)(px + width - textWindow_12C970_12C960.right_4);
	if (over > 0)
	{
		if (over >= width)
			return;
		visibleWidth = (uint8_t)(width - over);
	}
	uint8_t topCut = 0;
	over = (int16_t)(textWindow_12C970_12C960.top_8 - py);
	if (over > 0)
	{
		if (over >= height)
			return;
		topCut = (uint8_t)over;
	}
	uint8_t visibleHeight = height;
	over = (int16_t)(py + height - textWindow_12C970_12C960.bottom_12);
	if (over > 0)
	{
		if (over >= height)
			return;
		visibleHeight = (uint8_t)(height - over);
	}
	uint8_t columns = (uint8_t)(visibleWidth - leftCut);
	uint16_t rowsLeft = (uint8_t)(visibleHeight - topCut);
	int32_t pitch = (int16_t)*screenWidth_12EFF0_12EFE0;
	uint8_t* dest = *workScreen_12EFF4_12EFE4 + (py + topCut) * (int32_t)*screenWidth_12EFF0_12EFE0 + px + leftCut;
	const int8_t* src = (const int8_t*)glyph->data;
	for (uint8_t row = topCut; row; row--)
		src = SkipGlyphRow(src);
	uint8_t* nextRow = dest + pitch;
	for (;;)
	{
		int16_t remaining = columns;
		if (leftCut)
		{
			// 518AB: the first leftCut pixels of the row are clipped
			uint8_t skip = leftCut;
			for (;;)
			{
				int8_t c = *src;
				if (c > 0)
				{
					if (c > skip)
					{
						uint8_t count = (uint8_t)(c - skip);
						if (count > columns)
							count = columns;
						const int8_t* nextSrc = src + 1 + c;
						DrawGlyphPixels(font, src + 1 + skip, dest, count);
						remaining -= count;
						dest += count;
						src = nextSrc;
						break;
					}
					src += c + 1;
					skip -= c;
					if (!skip)
						break;
				}
				else if (c == 0)
				{
					remaining = 0;
					break;
				}
				else
				{
					if (c < -skip)
					{
						uint8_t count = (uint8_t)(-c - skip);
						if (count > columns)
							count = columns;
						src++;
						dest += count;
						remaining -= count;
						break;
					}
					src++;
					skip += c;
					if (!skip)
						break;
				}
			}
		}
		for (;;)
		{
			if (remaining <= 0)
			{
				if (--rowsLeft == 0)
					return;
				src = SkipGlyphRow(src);
				break;
			}
			int8_t c = *src;
			if (c > 0)
			{
				uint8_t count = c < remaining ? (uint8_t)c : (uint8_t)remaining;
				remaining -= c;
				DrawGlyphPixels(font, src + 1, dest, count);
				dest += count;
				src += 1 + c;
			}
			else if (c < 0)
			{
				remaining += c;
				dest -= c;
				src++;
			}
			else
			{
				if (--rowsLeft == 0)
					return;
				src++;
				break;
			}
		}
		dest = nextRow;
		nextRow += pitch;
	}
}
