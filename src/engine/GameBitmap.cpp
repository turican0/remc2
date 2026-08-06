#include "GameBitmap.h"

void GameBitmap::DrawColourizedBitmap(uint8_t* ptrBitmapData, uint8_t colour, uint8_t* ptrScreenBuffer, uint32_t stride, int16_t posX, int16_t posY, uint8_t posHeight, uint8_t scale)
{
	ptrScreenBuffer = (stride * posY + posX + ptrScreenBuffer);
	int8_t width = 0;
	int8_t posWidth = 0;
	int8_t startOffsetX = -1;
	uint8_t pixel = 0;
	uint8_t* ptrScreenBufferLineStart = ptrScreenBuffer;
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
				}
				posWidth--;
			} while (posWidth);
		}
		else
		{
			memset(ptrScreenBuffer, colour, width);
			ptrBitmapData += width;
			ptrScreenBuffer += width;
			countBytes += width;
		}
	} while (posHeight);
};

void GameBitmap::DrawBitmap(uint8_t* ptrBitmapData, uint8_t* ptrScreenBuffer, uint32_t stride, int16_t posX, int16_t posY, uint8_t posHeight, uint8_t scale)
{
	ptrScreenBuffer = (stride * posY + posX + ptrScreenBuffer);
	int8_t width = 0;
	int8_t posWidth = 0;
	int8_t startOffsetX = -1;
	uint8_t pixel = 0;
	uint8_t* ptrScreenBufferLineStart = ptrScreenBuffer;
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
				}
				posWidth--;
			} while (posWidth);
		}
		else
		{
			qmemcpy(ptrScreenBuffer, ptrBitmapData, width);
			ptrBitmapData += width;
			ptrScreenBuffer += width;
			countBytes += width;
		}
	} while (posHeight);
};

void GameBitmap::DrawBitmap(uint8_t* ptrBitmapData, uint8_t* ptrScreenBuffer, uint32_t stride, int16_t posX, int16_t posY, uint16_t width, uint16_t height, uint8_t v134)
{
	ptrScreenBuffer = (stride * posY + posX + ptrScreenBuffer);
	uint8_t pixel;
	int32_t startOffsetX;
	int32_t posWidth;
	uint8_t* ptrScreenBufferLineStart = ptrScreenBuffer;

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
				width = width - startOffsetX;
			}
			if (!startOffsetX)
				break;
			posWidth = startOffsetX;

			//Draw Line
			do
			{
				pixel = *ptrBitmapData++;
				width = width + 1;
				if ((width & 0x80u) == 0)
					*ptrScreenBuffer = pixel;
				--ptrScreenBuffer;
				--posWidth;
			} while (posWidth);
		}
		ptrScreenBufferLineStart += stride;
		ptrScreenBuffer = ptrScreenBufferLineStart;
		width = __PAIR__(height, v134) - 256;
	} while (height);
};

void GameBitmap::DrawTransparentBitmap_2DE80(int16_t posX, int16_t posY, bitmap_pos_struct_t a3, uint8_t scale)//20ee80
{
	int32_t startOffsetX; // eax
	int16_t posHeight; // bx
	uint8_t* ptrScreenBuffer;
	uint8_t* ptrBitmapData = nullptr; // edx
	uint8_t* ptrBitmapPixel = nullptr; // esi
	int32_t posWidth; // ecx
	int v15; // [esp+0h] [ebp-Ch]
	int32_t width; // [esp+0h] [ebp-Ch]
	uint8_t* ptrScreenBufferLineStart; // [esp+4h] [ebp-8h]

	if (x_WORD_180660_VGA_type_resolution == 1)
	{
		posHeight = a3.height_5 / 2;
		startOffsetX = posY / 2 * screenWidth_18062C + posX / 2;
		ptrScreenBuffer = (startOffsetX + pdwScreenBuffer_351628);
		ptrBitmapData = a3.data;
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
					ptrScreenBuffer = ptrScreenBufferLineStart;
					if (!posHeight)
						return;
				}
				if ((startOffsetX & 0x80u) == 0)
					break;
				ptrScreenBuffer -= (char)startOffsetX;
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
				HIBYTE(startOffsetX) = *ptrScreenBuffer;
				LOBYTE(startOffsetX) = x_BYTE_F6EE0_tablesx[0x4000 + startOffsetX];
				*ptrScreenBuffer++ = startOffsetX;
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
			ptrScreenBufferLineStart = (startOffsetX + pdwScreenBuffer_351628);
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
						ptrScreenBuffer = ptrScreenBufferLineStart;
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
						HIBYTE(startOffsetX) = *ptrScreenBuffer;
						LOBYTE(startOffsetX) = x_BYTE_F6EE0_tablesx[0x4000 + startOffsetX];
						*ptrScreenBuffer++ = startOffsetX;
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
		DrawColourizedBitmap(a3.data, colour, pdwScreenBuffer_351628, screenWidth_18062C, posX / 2, posY / 2, a3.height_5 / 2, 1);
	}
	else
	{
		DrawColourizedBitmap(a3.data, colour, pdwScreenBuffer_351628, screenWidth_18062C, posX, posY, a3.height_5, scale);
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