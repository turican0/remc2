//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <vector>
#include <algorithm>    // std::sort

#include "png.h"

#pragma comment(lib, "zlib.lib") // must be before libpng!
#ifndef _WIN64
#pragma comment(lib, "libpng15.lib") // must be after zlib!
#else
#endif

//#define PalToPNG

#define level1
//#define level25
//#define level4
//#define level2
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

unsigned char get32colorIndex(unsigned char* content_data32, int xindex2, int yindex2, int xpos, int ypos)
{
	int newindex = yindex2 * 2 + xindex2;
	int xindex = newindex % 8;
	int yindex = newindex / 8;
	int xpos32 = ((xpos + (0 + 32) % 32) / 4) % 32;
	int ypos32 = ((ypos + (0 + 32) % 32) / 4) % 32;
	int index32 = xindex * 32 + yindex * 32 * 32 * 8 + xpos32 + ypos32 * 32 * 8;
	unsigned char data = content_data32[index32];
	return data;			
};

bool get32color(unsigned char* content_data32, unsigned char x, int xindex2, int yindex2, int xpos, int ypos)
{
	int newindex = yindex2 * 2 + xindex2;
	int xindex = newindex % 8;
	int yindex = newindex / 8;

	for(int i=-3;i<=3;i++)
		for (int j = -3; j <=3; j++)
		{

			int xpos32 = ((xpos + (i+32)%32) / 4) % 32;
			int ypos32 = ((ypos + (j + 32) % 32) / 4) % 32;
			int index32 = xindex * 32 + yindex * 32 * 32 * 8 + xpos32 + ypos32 * 32 * 8;
			unsigned char data = content_data32[index32];
			if (data == x)return true;
		}
	/*
	int xpos32 = (xpos) / 4;
	int ypos32 = (ypos) / 4;	
	int index32 = xindex * 32 + yindex * 32 * 32 * 8 + xpos32 + ypos32*32*8;
	unsigned char data=content_data32[index32];
	if (data == x)return true;

	xpos32 = ((xpos+1) / 4)%32;
	ypos32 = (ypos) / 4;
	index32 = xindex * 32 + yindex * 32 * 32 * 8 + xpos32 + ypos32 * 32 * 8;
	data = content_data32[index32];
	if (data == x)return true;
	xpos32 = ((xpos) / 4);
	ypos32 = ((ypos+1) / 4)%32;
	index32 = xindex * 32 + yindex * 32 * 32 * 8 + xpos32 + ypos32 * 32 * 8;
	data = content_data32[index32];
	if (data == x)return true;
	xpos32 = ((xpos + 127) / 4) % 32;
	ypos32 = (ypos) / 4;
	index32 = xindex * 32 + yindex * 32 * 32 * 8 + xpos32 + ypos32 * 32 * 8;
	data = content_data32[index32];
	if (data == x)return true;
	xpos32 = (xpos) / 4;
	ypos32 = ((ypos + 127) / 4)%32;
	index32 = xindex * 32 + yindex * 32 * 32 * 8 + xpos32 + ypos32 * 32 * 8;
	data = content_data32[index32];
	if (data == x)return true;

	xpos32 = ((xpos + 1) / 4) % 32;
	ypos32 = ((ypos+1) / 4)%32;
	index32 = xindex * 32 + yindex * 32 * 32 * 8 + xpos32 + ypos32 * 32 * 8;
	data = content_data32[index32];
	if (data == x)return true;
	xpos32 = ((xpos + 127) / 4) % 32;
	ypos32 = ((ypos + 1) / 4) % 32;
	index32 = xindex * 32 + yindex * 32 * 32 * 8 + xpos32 + ypos32 * 32 * 8;
	data = content_data32[index32];
	if (data == x)return true;
	xpos32 = ((xpos + 127) / 4) % 32;
	ypos32 = ((ypos + 127) / 4) % 32;
	index32 = xindex * 32 + yindex * 32 * 32 * 8 + xpos32 + ypos32 * 32 * 8;
	data = content_data32[index32];
	if (data == x)return true;
	xpos32 = ((xpos + 1) / 4) % 32;
	ypos32 = ((ypos + 127) / 4) % 32;
	index32 = xindex * 32 + yindex * 32 * 32 * 8 + xpos32 + ypos32 * 32 * 8;
	data = content_data32[index32];
	if (data == x)return true;

	xpos32 = ((xpos + 2) / 4) % 32;
	ypos32 = (ypos) / 4;
	index32 = xindex * 32 + yindex * 32 * 32 * 8 + xpos32 + ypos32 * 32 * 8;
	data = content_data32[index32];
	if (data == x)return true;
	xpos32 = ((xpos) / 4);
	ypos32 = ((ypos + 2) / 4) % 32;
	index32 = xindex * 32 + yindex * 32 * 32 * 8 + xpos32 + ypos32 * 32 * 8;
	data = content_data32[index32];
	if (data == x)return true;
	xpos32 = ((xpos + 126) / 4) % 32;
	ypos32 = (ypos) / 4;
	index32 = xindex * 32 + yindex * 32 * 32 * 8 + xpos32 + ypos32 * 32 * 8;
	data = content_data32[index32];
	if (data == x)return true;
	xpos32 = (xpos) / 4;
	ypos32 = ((ypos + 126) / 4) % 32;
	index32 = xindex * 32 + yindex * 32 * 32 * 8 + xpos32 + ypos32 * 32 * 8;
	data = content_data32[index32];
	if (data == x)return true;
	*/




	return false;

	/*int xindex32 = (i / 32) % 2;
	int yindex32 = i / (32 * 32 * 2);
	int tileindex32 = yindex32 * 2 + xindex32;
	int xpos32 = (i - yindex32 * 32 * 32 * 2 - xindex32 * 32) % 32;
	int ypos32 = (i - (yindex32 * 32 * 32 * 2)) / (32 * 2);*/
};
typedef unsigned char Bit8u;
inline void setRGBA(png_byte* ptr, Bit8u* val)
{
	ptr[0] = val[0];
	ptr[1] = val[1];
	ptr[2] = val[2];
	ptr[3] = val[3];
}

int width, height;
png_byte color_type;
png_byte bit_depth;
png_bytep* row_pointers = NULL;

void read_png_file(char* filename) {
	FILE* fp;
	fopen_s(&fp, filename, "rb");

	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) abort();

	png_infop info = png_create_info_struct(png);
	if (!info) abort();

	if (setjmp(png_jmpbuf(png))) abort();

	png_init_io(png, fp);

	png_read_info(png, info);

	width = png_get_image_width(png, info);
	height = png_get_image_height(png, info);
	color_type = png_get_color_type(png, info);
	bit_depth = png_get_bit_depth(png, info);

	// Read any color_type into 8bit depth, RGBA format.
	// See http://www.libpng.org/pub/png/libpng-manual.txt

	if (bit_depth == 16)
		png_set_strip_16(png);

	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png);

	// PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(png);

	if (png_get_valid(png, info, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png);

	// These color_type don't have an alpha channel then fill it with 0xff.
	if (color_type == PNG_COLOR_TYPE_RGB ||
		color_type == PNG_COLOR_TYPE_GRAY ||
		color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

	if (color_type == PNG_COLOR_TYPE_GRAY ||
		color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png);

	png_read_update_info(png, info);

	//if (row_pointers) abort();

	row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
	for (int y = 0; y < height; y++) {
		row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
	}

	png_read_image(png, row_pointers);

	fclose(fp);

	png_destroy_read_struct(&png, &info, NULL);
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

Bit8u buffer[1024 * 1024];
Bit8u buffer2[1024 * 1024];
char textbuffer[512];

unsigned char* pngBuffer;

void addToBuffer2(unsigned char* buffer, unsigned char* buffer2, int width, int height, int posX, int posY, int orX, int orY) {
	for (int y = 0; y < 32; y++)
		for (int x = 0; x < 32; x++)
		{
			int x2 = x;
			int y2 = y;
			if (orY==-1) y2 = 32 - y - 1;
			if (orX==-1) x2 = 32 - x - 1;
			buffer2[(posY * (4 * 32 * 32) + posX * 32 + y2 * 4 * 32 + x2) * 4] = buffer[(y * 32 + x) * 4];
			buffer2[(posY * (4 * 32 * 32) + posX * 32 + y2 * 4 * 32 + x2) * 4 + 1] = buffer[(y * 32 + x) * 4 + 1];
			buffer2[(posY * (4 * 32 * 32) + posX * 32 + y2 * 4 * 32 + x2) * 4 + 2] = buffer[(y * 32 + x) * 4 + 2];
			buffer2[(posY * (4 * 32 * 32) + posX * 32 + y2 * 4 * 32 + x2) * 4 + 3] = 255;
		}
}

int file_exist(char* filename)
{
	struct stat   buffer;
	return (stat(filename, &buffer) == 0);
}

void pngToBuffer(unsigned char* pngbuffer) {
	for (int yy = 0; yy < height; yy++)
	{
		png_bytep row = row_pointers[yy];
		for (int xx = 0; xx < width; xx++) {
			png_bytep px = &(row[xx * 4]);
			Bit8u wrbyte = 0;
			Bit8u datared = px[0];
			Bit8u datagreen = px[1];
			Bit8u datablue = px[2];
			Bit8u dataalpha = px[3];
			pngbuffer[4 * (xx + width * yy) + 0] = datared;
			pngbuffer[4 * (xx + width * yy) + 1] = datagreen;
			pngbuffer[4 * (xx + width * yy) + 2] = datablue;
			pngbuffer[4 * (xx + width * yy) + 3] = dataalpha;
}
	}
}

int transparentColor = 0;
std::vector<int> usedColors;
int getIndexedColorL1(int colorR, int colorG, int colorB, unsigned char* palette) {
	int index;
	int diference = 10000000000;
	for (int i : usedColors)
	{
		if (transparentColor != i)
		{
			int testDiference = (abs(colorR - palette[i * 3 + 0]) + abs(colorG - palette[i * 3 + 1]) + abs(colorB - palette[i * 3 + 2]));
			if (testDiference < diference)
			{
				index = i;
				diference = testDiference;
			}
		}
	}
	return index;
};
int getIndexedColor(int minColor, int maxColor, int colorR, int colorG, int colorB, unsigned char* palette) {
	int index;
	int diference = 10000000000;
	for (int i = minColor; i < maxColor; i++)
	{
		if (transparentColor != i)
		{
			int testDiference = (abs(colorR - palette[i * 3 + 0]) + abs(colorG - palette[i * 3 + 1]) + abs(colorB - palette[i * 3 + 2]));
			if (testDiference < diference)
			{
				index = i;
				diference = testDiference;
			}
		}
	}
	return index;
};

void writeBufferToAllTextures(unsigned char* buffer, unsigned char* allTexturesBuffer, int posX, int posY) {
	int posX2 = posX + 8 * posY;
	int posY2 = posX2 / 2;
	posX2 = posX2 % 2;

	for (int y = 0; y < 128; y++)
		for (int x = 0; x < 128; x++)
		{
			allTexturesBuffer[posY2 * (2 * 128 * 128) + posX2 * (128) + y * 2 * 128 + x] = buffer[y * 128 + x];
		}
};

int main(int argc, char* argv[]) {
#ifdef level1
	const char* standartpal_filename = "c:\\prenos\\remc2\\tools\\palletelight\\Debug\\out-n.pal";
	const char* outdata_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\bl128n0-0.data";
	const char* orig32_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\bl32n0-0.data";
#endif level1
#ifdef level2
	const char* standartpal_filename = "c:\\prenos\\remc2\\tools\\palletelight\\Debug\\out-block.pal";
	const char* outdata_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\block128.data";
	const char* orig32_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\block32.data";
#endif level2
#ifdef level4
	const char* standartpal_filename = "c:\\prenos\\remc2\\tools\\palletelight\\Debug\\out-c.pal";
	const char* outdata_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\bl128c0-0.data";
	const char* orig32_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\bl32c0-0.data";
#endif level4
#ifdef level25
	const char* standartpal_filename = "c:\\prenos\\remc2\\tools\\palletelight\\Debug\\out-f.pal";
	const char* outdata_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\bl128f0-0.data";
	const char* orig32_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\bl32f0-0.data";
#endif level25

#ifdef PalToPNG
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
	
	int tileX = 8;
	int tileY = 19;
	for (int i = 0; i < tileY; i++)
		for (int j = 0; j < tileX ; j++)
		{
			for (int y = 0; y < 32; y++)
				for (int x = 0; x < 32; x++)
				{
					buffer[(y * 32 + x) * 4] = content_stdpal[content_data32[i * (tileX * 32 * 32) + j * (32) + y * tileX * 32 + x] * 3 + 0];
					buffer[(y * 32 + x) * 4 + 1] = content_stdpal[content_data32[i * (tileX * 32 * 32) + j * (32) + y * tileX * 32 + x] * 3 + 1];
					buffer[(y * 32 + x) * 4 + 2] = content_stdpal[content_data32[i * (tileX * 32 * 32) + j * (32) + y * tileX * 32 + x] * 3 + 2];
					buffer[(y * 32 + x) * 4 + 3] = 255;
				}
			addToBuffer2(buffer, buffer2, 4, 4, 0, 0, 1, 1);
			addToBuffer2(buffer, buffer2, 4, 4, 1, 0, -1, 1);
			addToBuffer2(buffer, buffer2, 4, 4, 2, 0, 1, 1);
			addToBuffer2(buffer, buffer2, 4, 4, 3, 0, -1, 1);
			addToBuffer2(buffer, buffer2, 4, 4, 0, 1, 1, -1);
			addToBuffer2(buffer, buffer2, 4, 4, 1, 1, -1, -1);
			addToBuffer2(buffer, buffer2, 4, 4, 2, 1, 1, -1);
			addToBuffer2(buffer, buffer2, 4, 4, 3, 1, -1, -1);
			addToBuffer2(buffer, buffer2, 4, 4, 0, 2, 1, 1);
			addToBuffer2(buffer, buffer2, 4, 4, 1, 2, -1, 1);
			addToBuffer2(buffer, buffer2, 4, 4, 2, 2, 1, 1);
			addToBuffer2(buffer, buffer2, 4, 4, 3, 2, -1, 1);
			addToBuffer2(buffer, buffer2, 4, 4, 0, 3, 1, -1);
			addToBuffer2(buffer, buffer2, 4, 4, 1, 3, -1, -1);
			addToBuffer2(buffer, buffer2, 4, 4, 2, 3, 1, -1);
			addToBuffer2(buffer, buffer2, 4, 4, 3, 3, -1, -1);
			sprintf_s(textbuffer,"%sx%01d-%01d.png", outdata_filename,i,j);
			//writeImagePNG((char*)textbuffer, 32*4, 32*4, (Bit8u*)buffer2, (char*)"aa");
			writeImagePNG((char*)textbuffer, 32, 32, (Bit8u*)buffer, (char*)"aa");
		}

#else
	unsigned char* pngbuffer;
	int tileX = 8;
	int tileY = 19;

	unsigned char* allTexturesBuffer=(unsigned char*)malloc(128*128* tileX* tileY);

	float remLineKoef = 1;

	for(int posY = 0; posY < tileY; posY++)
	{
		for (int posX = 0; posX < tileX; posX++)
		{
			sprintf_s(textbuffer, "%sx%d-%01d_rlt.png", outdata_filename, posY,posX);
			if (!file_exist(textbuffer))break;
			read_png_file(textbuffer);
			pngbuffer = (unsigned char*)malloc(width * height * 4);
			pngToBuffer(pngbuffer);

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

			
			//find water colors
			int watercolorindex = 0;
			unsigned char watercolor[256];
			/*for (int i = 0; i < 32 * 32 * 8*19; i++)
				//for (int j = 0; j < 32; j++)
			{
				int xindex = (i / 32) % 8;
				int yindex = i / (32 * 32 * 8);
				int tileindex = yindex * 8 + xindex;
				int xpos = (i - yindex * 32 * 32 * 8 - xindex * 32) % 32;
				int ypos = (i - (yindex * 32 * 32 * 8)) / (32 * 8);
				if (tileindex == 5+5*8)
				{
					bool notinindex = true;
					for (int ll = 0; ll < watercolorindex; ll++)
						if (watercolor[ll] == content_data32[i])
							notinindex = false;
					if (notinindex)
						watercolor[watercolorindex++] = content_data32[i];
				}
			}*/
			watercolor[watercolorindex++] = 1;
			watercolor[watercolorindex++] = 2;
			watercolor[watercolorindex++] = 3;
			watercolor[watercolorindex++] = 4;
			watercolor[watercolorindex++] = 5;
			watercolor[watercolorindex++] = 6;
			watercolor[watercolorindex++] = 7;
			watercolor[watercolorindex++] = 8;
			watercolor[watercolorindex++] = 9;
			watercolor[watercolorindex++] = 10;
			watercolor[watercolorindex++] = 11;

			unsigned char* buffer32 = (unsigned char*)malloc(32*32);
			
			for (int y = 0; y < 32; y++)
				for (int x = 0; x < 32; x++)
				{
					buffer32[y * 32 + x] = content_data32[posY * (tileX * 32 * 32) + posX * (32) + y * tileX * 32 + x];
				}

			int x;
			float colorRIerr[4096];
			float colorGIerr[4096];
			float colorBIerr[4096];
			float colorRJerr[4096];
			float colorGJerr[4096];
			float colorBJerr[4096];
			int colorRerr = 0;
			int colorGerr = 0;
			int colorBerr = 0;
			for (int i = 0; i < width; i++)
			{
				colorRIerr[i] = 0;
				colorGIerr[i] = 0;
				colorBIerr[i] = 0;
			}

			for (int j = 0; j < width; j++)
			{
				colorRJerr[j] = 0;
				colorGJerr[j] = 0;
				colorBJerr[j] = 0;
			}

			int j;

			usedColors.clear();
			for (int y = 0; y < 32; y++)
				for (int x = 0; x < 32; x++)
				{
					unsigned char originalColor = content_data32[posY * (tileX * 32 * 32) + posX * (32) + y * tileX * 32 + x];
					bool isIncluded = false;
					for (int k : usedColors)
						if (originalColor == k)
						{
							isIncluded = true;
							break;
						}
					if (!isIncluded)
						usedColors.push_back(originalColor);
				}
			sort(usedColors.begin(), usedColors.end());

			for (int i = 0; i < height; i++)
			{
				for (int jx = 0; jx < width; jx++)
				{
					if (i % 2 == 0)j = jx;
					else j = width - 1 - jx;

					int colorR = pngbuffer[4 * (j + width * i) + 0];
					int colorG = pngbuffer[4 * (j + width * i) + 1];
					int colorB = pngbuffer[4 * (j + width * i) + 2];

					float borderKoef = 40;
					if (colorRIerr[i] < -borderKoef) colorRIerr[i] = -borderKoef;
					if (colorGIerr[i] < -borderKoef) colorGIerr[i] = -borderKoef;
					if (colorBIerr[i] < -borderKoef) colorBIerr[i] = -borderKoef;

					if (colorRIerr[i] > borderKoef) colorRIerr[i] = borderKoef;
					if (colorGIerr[i] > borderKoef) colorGIerr[i] = borderKoef;
					if (colorBIerr[i] > borderKoef) colorBIerr[i] = borderKoef;

					borderKoef = 80;
					if (colorRIerr[j] < -borderKoef) colorRIerr[j] = -borderKoef;
					if (colorGIerr[j] < -borderKoef) colorGIerr[j] = -borderKoef;
					if (colorBIerr[j] < -borderKoef) colorBIerr[j] = -borderKoef;

					if (colorRIerr[j] > borderKoef) colorRIerr[j] = borderKoef;
					if (colorGIerr[j] > borderKoef) colorGIerr[j] = borderKoef;
					if (colorBIerr[j] > borderKoef) colorBIerr[j] = borderKoef;

					if (pngbuffer[4 * (j + width * i) + 3] < 128)
					{
						colorRIerr[i] = 0;
						colorGIerr[i] = 0;
						colorBIerr[i] = 0;

						colorRJerr[j] = 0;
						colorGJerr[j] = 0;
						colorBJerr[j] = 0;
					}

					float koef = 0.5;

					colorRerr = koef * (colorRIerr[i] + colorRJerr[j]);
					colorGerr = koef * (colorGIerr[i] + colorGJerr[j]);
					colorBerr = koef * (colorBIerr[i] + colorBJerr[j]);

					colorRIerr[i] -= 0.5 * colorRerr;
					colorGIerr[i] -= 0.5 * colorGerr;
					colorBIerr[i] -= 0.5 * colorBerr;

					colorRJerr[j] -= 0.5 * colorRerr;
					colorGJerr[j] -= 0.5 * colorGerr;
					colorBJerr[j] -= 0.5 * colorBerr;
#ifdef level1
					x = getIndexedColorL1(colorR - colorRerr, colorG - colorGerr, colorB - colorBerr, content_stdpal);
#else
#ifndef level4
					unsigned char originalColor = content_data32[(posY * 32 * 32 * 8) + (posX * 32) + ((int)(j/4) + 8 * 32 * ((int)(i/4)))];
					if(originalColor <=11)
						x = getIndexedColor(0, 12, colorR - colorRerr, colorG - colorGerr, colorB - colorBerr, content_stdpal);
					else
#endif // !level4
						x = getIndexedColor(12, szstd / 3, colorR - colorRerr, colorG - colorGerr, colorB - colorBerr, content_stdpal);
#endif // level1
						
					//fwrite((char*)&x, 1, 1, fptw_outdata2);
					buffer[i * width + j] = (char)x;

					colorRIerr[i] += (content_stdpal[x * 3 + 0] - colorR) * 0.5;
					colorGIerr[i] += (content_stdpal[x * 3 + 1] - colorG) * 0.5;
					colorBIerr[i] += (content_stdpal[x * 3 + 2] - colorB) * 0.5;

					colorRJerr[j] += (content_stdpal[x * 3 + 0] - colorR) * 0.5;
					colorGJerr[j] += (content_stdpal[x * 3 + 1] - colorG) * 0.5;
					colorBJerr[j] += (content_stdpal[x * 3 + 2] - colorB) * 0.5;

					buffer2[(i * width + j) * 4] = content_stdpal[x * 3 + 0];
					buffer2[(i * width + j) * 4 + 1] = content_stdpal[x * 3 + 1];
					buffer2[(i * width + j) * 4 + 2] = content_stdpal[x * 3 + 2];
					buffer2[(i * width + j) * 4 + 3] = 255;

				}
			}

			writeBufferToAllTextures(buffer, allTexturesBuffer, posX, posY);

			sprintf_s(textbuffer, "%sx%d-%01d_test1.png", outdata_filename, posY, posX);
			writeImagePNG((char*)textbuffer, width, height, buffer2, (char*)"aa");

		free(content_stdpal);
		free(buffer32);
		free(pngBuffer);

		}
	}
	FILE* fptw_outdata;
	fopen_s(&fptw_outdata, outdata_filename, "wb");
	for (int i=0 ; i<128*128*2*76; i++)
		fwrite(&allTexturesBuffer[i], 1, 1, fptw_outdata);
	fclose(fptw_outdata);

	free(allTexturesBuffer);
#endif
	return 0;
}
