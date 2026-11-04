#pragma once

#ifndef GAME_BITMAP
#define GAME_BITMAP

#include "Basic.h"

class GameBitmap
{
public:
	static constexpr uint8_t kTransparentBitmapAlpha = (uint8_t)(COLOUR_BLEND_ALPHA * 255.0f + 0.5f);

    static void DrawColourizedBitmap(uint8_t* ptrBitmapData, uint8_t colour, uint8_t* ptrScreenBuffer, uint32_t stride, int16_t posX, int16_t posY, uint8_t height, uint8_t scale = 1, uint8_t* ptrScreenAlphaBuffer = nullptr);
	static void DrawColourizedBitmap(int16_t posX, int16_t posY, bitmap_pos_struct_t a3, uint8_t colour, uint8_t scale = 1);
	static void DrawBitmap(uint8_t* ptrBitmapData, uint8_t* ptrScreenBuffer, uint32_t stride, int16_t posX, int16_t posY, uint8_t height, uint8_t scale = 1, uint8_t* ptrScreenAlphaBuffer = nullptr);
	static void DrawBitmap(uint8_t* ptrBitmapData, uint8_t* ptrScreenBuffer, uint32_t stride, int16_t posX, int16_t posY, uint16_t width, uint16_t height, uint8_t v134, uint8_t* ptrScreenAlphaBuffer = nullptr);
	static void DrawTransparentBitmap_2DE80(int16_t posX, int16_t posY, bitmap_pos_struct_t a3, uint8_t scale = 1);
	static void DrawMenuGraphic(uint16_t width, uint16_t height, uint8_t scale, uint8_t* ptrSrc, uint8_t* ptrDest);
	static void ScaleMenuGraphic(uint16_t height, uint8_t scale, uint8_t* ptrSrc, uint8_t* ptrDest);
	static void PaletteToRgb(uint8_t* ptrPalette, uint8_t colorAddressIdx, uint8_t truColorOut[3]);
	static void PaletteToRgba(uint8_t* ptrPalette, uint8_t colorIdx, uint8_t truColorOut[4]);
};

#endif //GAME_BITMAP