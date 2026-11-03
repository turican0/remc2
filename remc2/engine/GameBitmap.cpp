#include "GameBitmap.h"

void GameBitmap::DrawColourizedBitmap(uint8_t* ptrBitmapData, uint8_t colour, uint8_t* ptrScreenBuffer, uint32_t stride, int16_t posX, int16_t posY, uint8_t posHeight, uint8_t scale, uint8_t* ptrScreenAlphaBuffer)
{
	ptrScreenBuffer = (stride * posY + posX + ptrScreenBuffer);
	if (ptrScreenAlphaBuffer != nullptr)
		ptrScreenAlphaBuffer = (stride * posY + posX + ptrScreenAlphaBuffer);

	int8_t width = 0;
	int8_t posWidth = 0;
	int8_t startOffsetX = -1;
	uint8_t pixel = 0;
	uint8_t* ptrScreenBufferLineStart = ptrScreenBuffer;
	uint8_t* ptrScreenAlphaBufferLineStart = ptrScreenAlphaBuffer;
	int lineStartBytes = 0;
	int countBytes = 0;
	int scaledLinesDrawn = 0;
	do
	{
		while (1)
		{
			while (1)
			{
				startOffsetX = *ptrBitmapData++;
				countBytes++;
				//Is width byte
				if (startOffsetX)
					break;
				//Move row
				if (scaledLinesDrawn < scale - 1)
				{
					int lineLengthBytes = countBytes - lineStartBytes;
					ptrBitmapData -= lineLengthBytes;
					countBytes -= lineLengthBytes;
					scaledLinesDrawn++;
				}
				else
				{
					posHeight--;
					scaledLinesDrawn = 0;
					lineStartBytes = countBytes;
				}
				ptrScreenBufferLineStart += stride;
				ptrScreenBuffer = ptrScreenBufferLineStart;
				if (ptrScreenAlphaBuffer != nullptr)
				{
					ptrScreenAlphaBufferLineStart += stride;
					ptrScreenAlphaBuffer = ptrScreenAlphaBufferLineStart;
				}
				if (!posHeight)
					return;
			}
			//Is width byte
			if ((startOffsetX & 0x80u) == 0)
			{
				//Start Drawing
				break;
			}
			//Is a change of start coordinate
			int offset = (char)startOffsetX;
			ptrScreenBuffer -= offset * scale;
			if (ptrScreenAlphaBuffer != nullptr)
				ptrScreenAlphaBuffer -= offset * scale;
			if (!posHeight)
				return;
		}
		posWidth = startOffsetX;
		width = startOffsetX;
		//Draw line
		if (scale > 1)
		{
			do
			{
				pixel = *ptrBitmapData++;
				countBytes++;
				for (int s = 0; s < scale; s++)
				{
					*ptrScreenBuffer++ = colour;
					if (ptrScreenAlphaBuffer != nullptr)
						*ptrScreenAlphaBuffer++ = 255;
				}
				posWidth--;
			} while (posWidth);
		}
		else
		{
			memset(ptrScreenBuffer, colour, width);
			if (ptrScreenAlphaBuffer != nullptr)
				memset(ptrScreenAlphaBuffer, 255, width);
			ptrBitmapData += width;
			ptrScreenBuffer += width;
			if (ptrScreenAlphaBuffer != nullptr)
				ptrScreenAlphaBuffer += width;
			countBytes += width;
		}
	} while (posHeight);
};

void GameBitmap::DrawBitmap(uint8_t* ptrBitmapData, uint8_t* ptrScreenBuffer, uint32_t stride, int16_t posX, int16_t posY, uint8_t posHeight, uint8_t scale, uint8_t* ptrScreenAlphaBuffer)
{
	ptrScreenBuffer = (stride * posY + posX + ptrScreenBuffer);
	if (ptrScreenAlphaBuffer != nullptr)
		ptrScreenAlphaBuffer = (stride * posY + posX + ptrScreenAlphaBuffer);

	int8_t width = 0;
	int8_t posWidth = 0;
	int8_t startOffsetX = -1;
	uint8_t pixel = 0;
	uint8_t* ptrScreenBufferLineStart = ptrScreenBuffer;
	uint8_t* ptrScreenAlphaBufferLineStart = ptrScreenAlphaBuffer;
	int lineStartBytes = 0;
	int countBytes = 0;
	int scaledLinesDrawn = 0;
	do
	{
		while (1)
		{
			while (1)
			{
				startOffsetX = *ptrBitmapData++;
				countBytes++;
				//Is width byte
				if (startOffsetX)
					break;
				//Move row
				if (scaledLinesDrawn < scale - 1)
				{
					int lineLengthBytes = countBytes - lineStartBytes;
					ptrBitmapData -= lineLengthBytes;
					countBytes -= lineLengthBytes;
					scaledLinesDrawn++;
				}
				else
				{
					posHeight--;
					scaledLinesDrawn = 0;
					lineStartBytes = countBytes;
				}
				ptrScreenBufferLineStart += stride;
				ptrScreenBuffer = ptrScreenBufferLineStart;
				if (ptrScreenAlphaBuffer != nullptr)
				{
					ptrScreenAlphaBufferLineStart += stride;
					ptrScreenAlphaBuffer = ptrScreenAlphaBufferLineStart;
				}
				if (!posHeight)
					return;
			}
			//Is width byte
			if ((startOffsetX & 0x80u) == 0)
			{
				//Start Drawing
				break;
			}
			//Is a change of start coordinate
			int offset = (char)startOffsetX;
			ptrScreenBuffer -= offset * scale;
			if (ptrScreenAlphaBuffer != nullptr)
				ptrScreenAlphaBuffer -= offset * scale;
			if (!posHeight)
				return;
		}
		posWidth = startOffsetX;
		width = startOffsetX;
		//Draw line
		if (scale > 1)
		{
			do
			{
				pixel = *ptrBitmapData++;
				countBytes++;
				for (int s = 0; s < scale; s++)
				{
					*ptrScreenBuffer++ = pixel;
					if (ptrScreenAlphaBuffer != nullptr)
						*ptrScreenAlphaBuffer++ = 255;
				}
				posWidth--;
			} while (posWidth);
		}
		else
		{
			qmemcpy(ptrScreenBuffer, ptrBitmapData, width);
			if (ptrScreenAlphaBuffer != nullptr)
				memset(ptrScreenAlphaBuffer, 255, width);
			ptrBitmapData += width;
			ptrScreenBuffer += width;
			if (ptrScreenAlphaBuffer != nullptr)
				ptrScreenAlphaBuffer += width;
			countBytes += width;
		}
	} while (posHeight);
};

void GameBitmap::DrawBitmap(uint8_t* ptrBitmapData, uint8_t* ptrScreenBuffer, uint32_t stride, int16_t posX, int16_t posY, uint16_t width, uint16_t height, uint8_t v134, uint8_t* ptrScreenAlphaBuffer)
{
	ptrScreenBuffer = (stride * posY + posX + ptrScreenBuffer);
	if (ptrScreenAlphaBuffer != nullptr)
		ptrScreenAlphaBuffer = (stride * posY + posX + ptrScreenAlphaBuffer);

	uint8_t pixel;
	int32_t startOffsetX;
	int32_t posWidth;
	uint8_t* ptrScreenBufferLineStart = ptrScreenBuffer;
	uint8_t* ptrScreenAlphaBufferLineStart = ptrScreenAlphaBuffer;

	uint8_t widthByte = v134; // byte-precision sentinel - must match original 8-bit register wrap behavior

	do
	{
		while (1)
		{
			while (1)
			{
				startOffsetX = *ptrBitmapData++;
				//Is width byte
				if ((startOffsetX & 0x80u) == 0)
				{
					//Start Drawing
					break;
				}
				ptrScreenBuffer += startOffsetX;
				if (ptrScreenAlphaBuffer != nullptr)
					ptrScreenAlphaBuffer += startOffsetX;
				widthByte = widthByte - startOffsetX;
			}
			if (!startOffsetX)
				break;
			posWidth = startOffsetX;
			//Draw Line
			do
			{
				pixel = *ptrBitmapData++;
				widthByte = widthByte + 1;
				if ((widthByte & 0x80u) == 0)
				{
					*ptrScreenBuffer = pixel;
					if (ptrScreenAlphaBuffer != nullptr)
						*ptrScreenAlphaBuffer = 255;
				}
				--ptrScreenBuffer;
				if (ptrScreenAlphaBuffer != nullptr)
					--ptrScreenAlphaBuffer;
				--posWidth;
			} while (posWidth);
		}
		ptrScreenBufferLineStart += stride;
		ptrScreenBuffer = ptrScreenBufferLineStart;
		if (ptrScreenAlphaBuffer != nullptr)
		{
			ptrScreenAlphaBufferLineStart += stride;
			ptrScreenAlphaBuffer = ptrScreenAlphaBufferLineStart;
		}
		widthByte = v134;
		--height;
	} while (height);
};

void GameBitmap::DrawTransparentBitmap_2DE80(int16_t posX, int16_t posY, bitmap_pos_struct_t a3, uint8_t scale)//20ee80
{
	int32_t startOffsetX; // eax
	int16_t posHeight; // bx
	uint8_t* ptrScreenBuffer;
	uint8_t* ptrScreenAlphaBuffer = nullptr;
	uint8_t* ptrBitmapData = nullptr; // edx
	uint8_t* ptrBitmapPixel = nullptr; // esi
	int32_t posWidth; // ecx
	int v15; // [esp+0h] [ebp-Ch]
	int32_t width; // [esp+0h] [ebp-Ch]
	uint8_t* ptrScreenBufferLineStart = nullptr;
	uint8_t* ptrScreenAlphaBufferLineStart = nullptr;

	if (x_WORD_180660_VGA_type_resolution == 1)
	{
		posHeight = a3.height_5 / 2;
		startOffsetX = posY / 2 * screenWidth_18062C + posX / 2;
		ptrScreenBuffer = (startOffsetX + pdwScreenBuffer_351628);
		if (pdwScreenAlphaBuffer != nullptr)
			ptrScreenAlphaBuffer = (startOffsetX + pdwScreenAlphaBuffer);

		ptrBitmapData = a3.data;

		if (pdwScreenAlphaBuffer != nullptr)
			ptrScreenAlphaBufferLineStart = startOffsetX + pdwScreenAlphaBuffer;

		for (ptrScreenBufferLineStart = startOffsetX + pdwScreenBuffer_351628; posHeight; ptrBitmapData += v15)
		{
			while (1)
			{
				while (1)
				{
					LOBYTE(startOffsetX) = *ptrBitmapData++;
					if ((x_BYTE)startOffsetX)
						break;
					posHeight--;
					ptrScreenBufferLineStart += screenWidth_18062C;
					if (pdwScreenAlphaBuffer != nullptr)
						ptrScreenAlphaBufferLineStart += screenWidth_18062C;
					ptrScreenBuffer = ptrScreenBufferLineStart;
					ptrScreenAlphaBuffer = ptrScreenAlphaBufferLineStart;
					if (!posHeight)
						return;
				}
				if ((startOffsetX & 0x80u) == 0)
					break;
				ptrScreenBuffer -= (char)startOffsetX;
				if (pdwScreenAlphaBuffer != nullptr)
					ptrScreenAlphaBuffer -= (char)startOffsetX;
				if (!posHeight)
					return;
			}
			startOffsetX = (char)startOffsetX;//20ef1f
			ptrBitmapPixel = ptrBitmapData;
			posWidth = startOffsetX;
			v15 = (char)startOffsetX;
			HIWORD(startOffsetX) = 0;
			do
			{
				LOBYTE(startOffsetX) = *ptrBitmapPixel++;
				uint8_t srcIndex = LOBYTE(startOffsetX);

				if (pdwScreenAlphaBuffer != nullptr && *ptrScreenAlphaBuffer != 255)
				{
					*ptrScreenBuffer++ = srcIndex;
					*ptrScreenAlphaBuffer++ = kTransparentBitmapAlpha;
				}
				else
				{
					HIBYTE(startOffsetX) = *ptrScreenBuffer;
					LOBYTE(startOffsetX) = ColourLookupTable_F6EE0[COLOUR_BLEND_LOOKUP_OFFSET + startOffsetX];
					*ptrScreenBuffer++ = startOffsetX;
					if (pdwScreenAlphaBuffer != nullptr)
						ptrScreenAlphaBuffer++;
				}
				posWidth--;
			} while (posWidth);
		}
	}
	else
	{
		if (a3.height_5)
		{
			startOffsetX = posX + screenWidth_18062C * posY;
			posHeight = a3.height_5;
			ptrScreenBuffer = (startOffsetX + pdwScreenBuffer_351628);
			if (pdwScreenAlphaBuffer != nullptr)
				ptrScreenAlphaBuffer = (startOffsetX + pdwScreenAlphaBuffer);

			ptrScreenBufferLineStart = (startOffsetX + pdwScreenBuffer_351628);
			if (pdwScreenAlphaBuffer != nullptr)
				ptrScreenAlphaBufferLineStart = (startOffsetX + pdwScreenAlphaBuffer);

			ptrBitmapData = a3.data;
			int lineStartBytes = 0;
			int countBytes = 0;
			int scaledLinesDrawn = 0;
			do
			{
				while (1)
				{
					while (1)
					{
						LOBYTE(startOffsetX) = *ptrBitmapData++;
						countBytes++;
						//If it has value
						if ((x_BYTE)startOffsetX)
							break;
						//Move row
						if (scaledLinesDrawn < scale - 1)
						{
							int lineLengthBytes = countBytes - lineStartBytes;
							ptrBitmapData -= lineLengthBytes;
							countBytes -= lineLengthBytes;
							scaledLinesDrawn++;
						}
						else
						{
							posHeight--;
							scaledLinesDrawn = 0;
							lineStartBytes = countBytes;
						}
						ptrScreenBufferLineStart += screenWidth_18062C;
						if (pdwScreenAlphaBuffer != nullptr)
							ptrScreenAlphaBufferLineStart += screenWidth_18062C;
						ptrScreenBuffer = ptrScreenBufferLineStart;
						ptrScreenAlphaBuffer = ptrScreenAlphaBufferLineStart;
						if (!posHeight)
							return;
					}
					//Is width byte
					if ((startOffsetX & 0x80u) == 0)
					{
						//Start Drawing
						break;
					}
					//Is a change of start coordinate
					int offset = (char)startOffsetX;
					ptrScreenBuffer -= offset * scale;
					if (pdwScreenAlphaBuffer != nullptr)
						ptrScreenAlphaBuffer -= offset * scale;
					if (!posHeight)
						return;
				}
				posWidth = LOBYTE(startOffsetX);
				width = LOBYTE(startOffsetX);
				ptrBitmapPixel = ptrBitmapData;
				HIWORD(startOffsetX) = 0;
				//Draw line
				do
				{
					for (int s = 0; s < scale; s++)
					{
						LOBYTE(startOffsetX) = *ptrBitmapPixel;
						uint8_t srcIndex = LOBYTE(startOffsetX);

						if (pdwScreenAlphaBuffer != nullptr && *ptrScreenAlphaBuffer != 255)
						{
							*ptrScreenBuffer++ = srcIndex;
							*ptrScreenAlphaBuffer++ = kTransparentBitmapAlpha;
						}
						else
						{
							HIBYTE(startOffsetX) = *ptrScreenBuffer;
							LOBYTE(startOffsetX) = ColourLookupTable_F6EE0[COLOUR_BLEND_LOOKUP_OFFSET + startOffsetX];
							*ptrScreenBuffer++ = startOffsetX;
							if (pdwScreenAlphaBuffer != nullptr)
								ptrScreenAlphaBuffer++;
						}
					}
					ptrBitmapPixel++;
					countBytes++;
					posWidth--;
				} while (posWidth);
				ptrBitmapData += width;
			} while (posHeight);
		}
	}
};

void GameBitmap::DrawMenuGraphic(uint16_t width, uint16_t height, uint8_t scale, uint8_t* ptrSrc, uint8_t* ptrDest)
{
	int lineCount = 0;
	int index = 0;
	int lineStartIndex = 0;
	int byteCount = 0;
	int32_t pixel = 0;

	while (lineCount < height)
	{
		while (lineCount < height)
		{
			LOBYTE(pixel) = ptrSrc[index];
			index++;
			if ((char)pixel)
				break;

			//line ended, move row
			lineStartIndex += width;
			byteCount = lineStartIndex;
			lineCount++;
		}

		if (lineCount < height)
		{
			if ((pixel & 0x80u) == 0)
			{
				uint16_t lnWidth = (char)pixel * scale;

				//Draw line
				for (int x = 0; x < lnWidth; x++)
				{
					ptrDest[byteCount] = ptrSrc[index];
					byteCount++;
					index++;
				}
			}
			else
			{
				byteCount -= (char)pixel * scale;
			}
		}
	}
};

void GameBitmap::DrawColourizedBitmap(int16_t posX, int16_t posY, bitmap_pos_struct_t a3, uint8_t colour, uint8_t scale)
{
	if (x_WORD_180660_VGA_type_resolution == 1)
	{
		DrawColourizedBitmap(a3.data, colour, pdwScreenBuffer_351628, screenWidth_18062C, posX / 2, posY / 2, a3.height_5 / 2, 1, pdwScreenAlphaBuffer);
	}
	else
	{
		DrawColourizedBitmap(a3.data, colour, pdwScreenBuffer_351628, screenWidth_18062C, posX, posY, a3.height_5, scale, pdwScreenAlphaBuffer);
	}
}

/// <summary>
/// Utility method, not currently used
/// </summary>
/// <param name="height"></param>
/// <param name="scale"></param>
/// <param name="ptrSrc"></param>
/// <param name="ptrDest"></param>
void GameBitmap::ScaleMenuGraphic(uint16_t height, uint8_t scale, uint8_t* ptrSrc, uint8_t* ptrDest)
{
	int lineCount = 0;
	int index = 0;
	int32_t pixel = 0;
	int countBytes = 0;
	int lineStartXIndex = 0;

	while (lineCount < height)
	{
		while (lineCount < height)
		{
			LOBYTE(pixel) = ptrSrc[index];
			index++;
			if ((char)pixel)
				break;

			//line ended, move row
			ptrDest[countBytes] = (char)pixel;
			countBytes++;
			lineCount++;

			int lineLength = countBytes - lineStartXIndex;

			for (int s = 0; s < scale - 1; s++)
			{
				std::memcpy(&ptrDest[countBytes], &ptrDest[lineStartXIndex], lineLength);
				countBytes += lineLength;
			}
			lineStartXIndex = countBytes;
		}

		if (lineCount < height)
		{
			if ((pixel & 0x80u) == 0)
			{
				int32_t lnWidth = (char)pixel;
				ptrDest[countBytes] = (char)pixel;
				countBytes++;

				//Draw line
				for (int x = 0; x < lnWidth; x++)
				{
					for (int s = 0; s < scale; s++)
					{
						ptrDest[countBytes] = ptrSrc[index];
						countBytes++;
					}
					index++;
				}
			}
			else
			{
				ptrDest[countBytes] = (char)pixel;
				countBytes++;
			}
		}
	}
};

void GameBitmap::PaletteToRgb(uint8_t* ptrPalette, uint8_t colorIdx, uint8_t truColorOut[3])
{
	truColorOut[0] = ptrPalette[colorIdx * 3 + 2];
	truColorOut[1] = ptrPalette[colorIdx * 3 + 1];
	truColorOut[2] = ptrPalette[colorIdx * 3];
}

void GameBitmap::PaletteToRgba(uint8_t* ptrPalette, uint8_t colorIdx, uint8_t truColorOut[4])
{
	truColorOut[0] = ptrPalette[colorIdx * 3 + 2];
	truColorOut[1] = ptrPalette[colorIdx * 3 + 1];
	truColorOut[2] = ptrPalette[colorIdx * 3];

	if (colorIdx != 255)
		truColorOut[3] = 255;
}
