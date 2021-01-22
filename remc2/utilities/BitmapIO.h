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
	static const int HIGHCOLOR_BYTES_PER_PIXEL;
	static const int HIGHCOLOR_FILE_HEADER_SIZE;
	static const int HIGHCOLOR_INFO_HEADER_SIZE;

	static void WriteImageBufferAsImageBMP(char* path, int width, int height, Bit8u* ptrBuffer);
};

#endif