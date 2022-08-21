#pragma once
#ifndef BitmapIO_H
#define BitmapIO_H

#include <cstdint>
#include <png.h>
#include "port_filesystem.h"

class BitmapIO
{
private:
	static unsigned char* CreateBitBitmapFileHeader(int fileHeaderSize, int infoHeaderSize, int height, int stride);
	static unsigned char* CreateBitBitmapInfoHeader(int infoHeaderSize, int width, int height, int bytesPerPixel);

public:
	static const int TRUECOLOR_BYTES_PER_PIXEL;
	static const int RGBA_BYTES_PER_PIXEL;
	static const int BITMAP_FILE_HEADER_SIZE;
	static const int BITMAP_INFO_HEADER_SIZE;

	static void WriteImageBufferAsImageBMP(const char* path, int width, int height, uint8_t* ptrPalette , uint8_t* ptrBuffer);
	static void WriteRGBAImageBufferAsImageBMP(const char* path, int width, int height, uint8_t* ptrPalette, uint8_t* ptrBuffer);
	static void WritePaletteAsImageBMP(const char* path, int numColors, uint8_t* ptrPalette);
};

#endif
