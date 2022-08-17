// PalToPng.cpp : Tento soubor obsahuje funkci main. Provádění programu se tam zahajuje a ukončuje.
//

#include <iostream>
#include "math.h"

#define BMPTOBIG

//#define sky1
//#define sky2
#define sky3

#ifdef sky1
const char* standartpal_filename = "c:\\prenos\\remc2\\tools\\palletelight\\Debug\\out-n.pal";
const char* outdata_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\skyn0-0.png";
const char* orig32_filename = "c:\\prenos\\Magic2\\mc2-orig\\data\\skyn0-0.dat";
const char* bigbmp_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\skyn128.bmp";
const char* outdata2_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\skyn1024.data";
#endif sky1
#ifdef sky2
const char* standartpal_filename = "c:\\prenos\\remc2\\tools\\palletelight\\Debug\\out-c.pal";
const char* outdata_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\skyc0-0.png";
const char* orig32_filename = "c:\\prenos\\Magic2\\mc2-orig\\data\\skyc0-0.dat";
const char* bigbmp_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\skyc128.bmp";
const char* outdata2_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\skyc1024.data";
#endif sky2
#ifdef sky3
const char* standartpal_filename = "c:\\prenos\\remc2\\tools\\palletelight\\Debug\\out-block.pal";
const char* outdata_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\skyd0-0.png";
const char* orig32_filename = "c:\\prenos\\Magic2\\mc2-orig\\data\\skyd0-0.dat";
const char* bigbmp_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\skyd128.bmp";
const char* outdata2_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\skyd1024.data";
#endif sky3

#include "png.h"
#pragma comment(lib, "zlib.lib") // must be before libpng!
#ifndef _WIN64
#pragma comment(lib, "libpng15.lib") // must be after zlib!
#else
#endif

typedef unsigned char Bit8u;

inline void setRGBA(png_byte* ptr, Bit8u* val)
{
	ptr[0] = val[0];
	ptr[1] = val[1];
	ptr[2] = val[2];
	ptr[3] = val[3];
}

void writeImagePNG(char* filename, int width, int height, Bit8u* buffer, char* title)
{
	int code = 0;
	FILE* fp = NULL;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	png_bytep row = NULL;

	// Open file for writing (binary mode)
	fopen_s(&fp, filename, "wb");
	if (fp == NULL) {
		fprintf(stderr, "Could not open file %s for writing\n", filename);
		code = 1;
		goto finalise;
	}

	// Initialize write structure
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fprintf(stderr, "Could not allocate write struct\n");
		code = 1;
		goto finalise;
	}

	// Initialize info structure
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fprintf(stderr, "Could not allocate info struct\n");
		code = 1;
		goto finalise;
	}

	// Setup Exception handling
	if (setjmp(png_jmpbuf(png_ptr))) {
		fprintf(stderr, "Error during png creation\n");
		code = 1;
		goto finalise;
	}

	png_init_io(png_ptr, fp);

	// Write header (8 bit colour depth)
	png_set_IHDR(png_ptr, info_ptr, width, height,
		8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	// Set title
	if (title != NULL) {
		png_text title_text;
		title_text.compression = PNG_TEXT_COMPRESSION_NONE;
		title_text.key = (png_charp)"Title";
		title_text.text = title;
		png_set_text(png_ptr, info_ptr, &title_text, 1);
	}

	png_write_info(png_ptr, info_ptr);

	// Allocate memory for one row (3 bytes per pixel - RGB)
	row = (png_bytep)malloc(4 * width * sizeof(png_byte));

	// Write image data
	int x, y;
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			setRGBA(&(row[x * 4]), buffer + (y * width + x) * 4);
		}
		png_write_row(png_ptr, row);
	}

	// End write
	png_write_end(png_ptr, NULL);

finalise:
	if (fp != NULL) fclose(fp);
	if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
	if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	if (row != NULL) free(row);

	//return code;
}

char getIndexedColor(int colorR, int colorG, int colorB, unsigned char* palette, int paletteSize) {
	char index;
	int diference = 10000000000;
	for (int i = 0; i < paletteSize; i++)
	{
		int testDiference = (abs(colorR- palette[i*3+0])+ abs(colorG - palette[i * 3 + 1])+ abs(colorB - palette[i * 3 + 2]));
		if (testDiference < diference)
		{
			index = i;
			diference = testDiference;
		}
	}
	return index;
};

int main()
{
#ifdef BMPTOBIG
	FILE* fptr_stdpal;
	fopen_s(&fptr_stdpal, standartpal_filename, "rb");
	fseek(fptr_stdpal, 0L, SEEK_END);
	long szstd = ftell(fptr_stdpal);
	fseek(fptr_stdpal, 0L, SEEK_SET);
	unsigned char* content_stdpal = (unsigned char*)malloc(szstd * sizeof(char*));
	fread(content_stdpal, szstd, 1, fptr_stdpal);
	fclose(fptr_stdpal);

	FILE* fptr_stdbigbmp;
	fopen_s(&fptr_stdbigbmp, bigbmp_filename, "rb");
	fseek(fptr_stdbigbmp, 0L, SEEK_END);
	long szstdbigbmp = ftell(fptr_stdbigbmp);
	fseek(fptr_stdbigbmp, 0L, SEEK_SET);
	unsigned char* content_bigbmp = (unsigned char*)malloc(szstdbigbmp * sizeof(char*));
	fread(content_bigbmp, szstdbigbmp, 1, fptr_stdbigbmp);
	fclose(fptr_stdbigbmp);

	char x;
	int sizeBMPheader = 138;
	FILE* fptw_outdata2;
	fopen_s(&fptw_outdata2, outdata2_filename, "wb");
	for (int i = 0; i < 1024; i++)
		for (int j = 0; j < 1024; j++)
		{
			int colorR = content_bigbmp[sizeBMPheader + szstdbigbmp - 1 - (3 * (i * 1024 + j) + 0)];
			int colorG = content_bigbmp[sizeBMPheader + szstdbigbmp - 1 - (3 * (i * 1024 + j) + 1)];
			int colorB = content_bigbmp[sizeBMPheader + szstdbigbmp - 1 - (3 * (i * 1024 + j) + 2)];
			x = getIndexedColor(colorR,colorG,colorB, content_stdpal, szstd);
			fwrite(&x, 1, 1, fptw_outdata2);
		}
	fclose(fptw_outdata2);
#else
	FILE* fptr_stdpal;
	fopen_s(&fptr_stdpal, standartpal_filename, "rb");
	fseek(fptr_stdpal, 0L, SEEK_END);
	long szstd = ftell(fptr_stdpal);
	fseek(fptr_stdpal, 0L, SEEK_SET);
	unsigned char* content_stdpal = (unsigned char*)malloc(szstd * sizeof(char*));
	fread(content_stdpal, szstd, 1, fptr_stdpal);
	fclose(fptr_stdpal);

	FILE* fptr_stdorig32;
	fopen_s(&fptr_stdorig32, orig32_filename, "rb");
	fseek(fptr_stdorig32, 0L, SEEK_END);
	long szstdorig32 = ftell(fptr_stdorig32);
	fseek(fptr_stdorig32, 0L, SEEK_SET);
	unsigned char* content_data32 = (unsigned char*)malloc(szstdorig32 * sizeof(char*));
	fread(content_data32, szstdorig32, 1, fptr_stdorig32);
	fclose(fptr_stdorig32);

	int width = 256;
	int height = 256;

	Bit8u* buffer2 = (Bit8u*)malloc(width * height * 4);

	for(int i=0;i< height;i++)
		for (int j = 0; j < width; j++)
		{
			buffer2[(i * width + j) * 4] = content_stdpal[content_data32[i * width + j] * 3 + 0];
			buffer2[(i * width + j) * 4+1] = content_stdpal[content_data32[i * width + j] * 3 + 1];
			buffer2[(i * width + j) * 4+2] = content_stdpal[content_data32[i * width + j] * 3 + 2];
		}

	writeImagePNG((char*)outdata_filename, width, height, buffer2, (char*)"aa");
#endif
}