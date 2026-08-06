#pragma once
#ifndef BitmapIO_H
#define BitmapIO_H

#include "build_config.h"

#include <cstdint>
#include <vector>

#include "portability/port_filesystem.h"

struct RGBAImage
{
	std::vector<uint8_t> pixels;
	int width = 0;
	int height = 0;
	bool hasAlpha = false;
};

class BitmapIO
{
private:
	static unsigned char* CreateBitBitmapFileHeader(int fileHeaderSize, int infoHeaderSize, int height, int stride);
	static unsigned char* CreateBitBitmapInfoHeader(int infoHeaderSize, int width, int height, int bytesPerPixel);	
	static void WriteImagePNG(const char* filename, int width, int height, uint8_t* buffer);
#ifdef REMC2_CODE
	static void setRGBA(uint8_t* ptr, uint8_t* val);
#endif //REMC2_CODE	

public:
	static uint8_t MultiplyValue(uint8_t orig, double multiplier);
	static const int TRUECOLOR_BYTES_PER_PIXEL;
	static const int RGBA_BYTES_PER_PIXEL;
	static const int BITMAP_FILE_HEADER_SIZE;
	static const int BITMAP_INFO_HEADER_SIZE;
	static const int TRANSPARENT_COLOUR;

	static void WriteImageBufferAsImageBMP(const char* path, int width, int height, uint8_t* ptrPalette , uint8_t* ptrBuffer);
	static void WriteRGBAImageBufferAsImageBMP(const char* path, int width, int height, uint8_t* ptrPalette, uint8_t* ptrBuffer);
	static void WritePaletteAsImageBMP(const char* path, int numColors, uint8_t* ptrPalette);

	static bool ReadImagePNG(const char *filename, RGBAImage &out);
#ifdef _DEBUG
	static void WritePosistructToPng(uint8_t* ptrPalette, uint8_t* ptrBuffer, int width, int height, char* filename, char* title, int padding = 0, double multiplier = 4);
	static void WritePosistructToPng(uint8_t* ptrPalette, uint8_t* ptrBuffer, int width, int height, char* filename, char* title, int padding, double multiplier, uint8_t transColR, uint8_t transColG, uint8_t transColB);
#endif

};

#endif
