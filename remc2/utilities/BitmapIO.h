#pragma once
#ifndef BitmapIO_H
#define BitmapIO_H

#include "../portability/port_filesystem.h"

class BitmapIO
{
private:
	static unsigned char* CreateBitBitmapFileHeader(int fileHeaderSize, int infoHeaderSize, int height, int stride);
	static unsigned char* CreateBitBitmapInfoHeader(int infoHeaderSize, int width, int height, int bytesPerPixel);

public:
	static const int TRUECOLOR_BYTES_PER_PIXEL;
	static const int BITMAP_FILE_HEADER_SIZE;
	static const int BITMAP_INFO_HEADER_SIZE;

	static void WriteImageBufferAsImageBMP(char* path, int width, int height, Bit8u* ptrPalette , Bit8u* ptrBuffer);
	static void WritePaletteAsImageBMP(char* path, int numColors, Bit8u* ptrPalette);
};

#endif