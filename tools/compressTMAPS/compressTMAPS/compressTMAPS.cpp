#include <iostream>
#include <vector>

#include "png.h"

#pragma comment(lib, "zlib.lib") // must be before libpng!
#ifndef _WIN64
#pragma comment(lib, "libpng15.lib") // must be after zlib!
#else
#endif

#define level1 //TMAPS1 night
//#define level2 //TMAPS0 day
//#define level4 //TMAPS2 cave

//#define rewriteBadFiles

#define SIZEOF_UNSIGNED_INT 4

#define SIZEOF_UNSIGNED_CHAR 1

#define SIZEOF_UNSIGNED_SHORT 2

#if SIZEOF_UNSIGNED_SHORT != 2
#  error "sizeof (unsigned short) != 2"
#else
typedef unsigned short Bit16u;
typedef   signed short Bit16s;
#endif

#if SIZEOF_UNSIGNED_INT == 4
typedef unsigned int Bit32u;
typedef   signed int Bit32s;
#elif SIZEOF_UNSIGNED_LONG == 4
typedef unsigned long Bit32u;
typedef   signed long Bit32s;
#else
//#  error "can't find sizeof(type) of 4 bytes!"
#endif

#if SIZEOF_UNSIGNED_CHAR != 1
#  error "sizeof (unsigned char) != 1"
#else
typedef unsigned char Bit8u;
typedef   signed char Bit8s;
#endif

typedef          char   int8;
typedef   signed char   sint8;
typedef unsigned char   uint8;
typedef          short  int16;
typedef   signed short  sint16;
typedef unsigned short  uint16;
typedef          int    int32;
typedef   signed int    sint32;
typedef unsigned int    uint32;

int width, height;
int width_fr, height_fr;
png_byte color_type;
png_byte bit_depth;
png_bytep* row_pointers = NULL;


//png_bytep* row_alphapointers = NULL;


int file_exist(char* filename)
{
	struct stat   buffer;
	return (stat(filename, &buffer) == 0);
}

void read_png_file(char* filename) {
	FILE* fp;
	fopen_s(&fp,filename, "rb");

	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) abort();

	png_infop info = png_create_info_struct(png);
	if (!info) abort();

	if (setjmp(png_jmpbuf(png))) abort();

	png_init_io(png, fp);

	png_read_info(png, info);

	width_fr = png_get_image_width(png, info);
	height_fr = png_get_image_height(png, info);
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

	row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height_fr);
	for (int y = 0; y < height_fr; y++) {
		row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
	}

	png_read_image(png, row_pointers);

	fclose(fp);

	png_destroy_read_struct(&png, &info, NULL);
}
/*
void read_pngalpha_file(char* filename) {
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

	//if (row_alphapointers) abort();

	row_alphapointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
	for (int y = 0; y < height; y++) {
		row_alphapointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
	}

	png_read_image(png, row_alphapointers);

	fclose(fp);

	png_destroy_read_struct(&png, &info, NULL);
}
*/
int transparentColor = 0;
std::vector<uint8_t> usedColors;
int getIndexedColor(int colorR, int colorG, int colorB, unsigned char* palette, int paletteSize) {
	int index;
	int diference = 10000000000;
	for (int i = 0; i < paletteSize / 3; i++)
	{
#ifdef level1 //TMAPS1 night
		/*if ((i >= 0xAB) && (i <= 0xB7))
			continue;*/
		for(char locCol: usedColors)
			if(i== locCol)
				continue;
#endif
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

char buffer[4096*4096];
int score[2048 * 2048  * 8];
unsigned char maskBuffer2[2048 * 2048 * 4];

void findMask(unsigned char* maskBuffer, unsigned char* pngbufferAr[6], unsigned char* pngbufferFinal) {
	int koef = 10000;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++){
			score[y * width + x] = 0;
		}
	}

	for(int i = 0; i < 6; i++)
		for (int j = 0; j < 6; j++)
		{
			if (i != j)
			{
				for (int y = 0; y < height; y++)
				{
					for (int x = 0; x < width; x++)
					{
						score[y * width + x] += abs(pngbufferAr[i][(y * width + x) * 4] - pngbufferAr[j][(y * width + x) * 4]);
						score[y * width + x] += abs(pngbufferAr[i][(y * width + x) * 4 + 1] - pngbufferAr[j][(y * width + x) * 4 + 1]);
						score[y * width + x] += abs(pngbufferAr[i][(y * width + x) * 4 + 2] - pngbufferAr[j][(y * width + x) * 4 + 2]);
					}
				}
			}
		}

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (score[y * width + x] > koef)
			{
				maskBuffer2[(y * width + x) * 4 + 3] = 0;
			}
			else
			{
				maskBuffer2[(y * width + x) * 4 + 3] = 255;
			}
			maskBuffer2[(y * width + x) * 4] = pngbufferFinal[(y * width + x) * 4];
			maskBuffer2[(y * width + x) * 4 + 1] = pngbufferFinal[(y * width + x) * 4 + 1];
			maskBuffer2[(y * width + x) * 4 + 2] = pngbufferFinal[(y * width + x) * 4 + 2];
		}
	}
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			maskBuffer[(y * width + x) * 4] = maskBuffer2[(y * width + x) * 4];
			maskBuffer[(y * width + x) * 4 + 1] = maskBuffer2[(y * width + x) * 4 + 1];
			maskBuffer[(y * width + x) * 4 + 2] = maskBuffer2[(y * width + x) * 4 + 2];
			maskBuffer[(y * width + x) * 4 + 3] = maskBuffer2[(y * width + x) * 4 + 3];
			/*if (x > 0)
			{
				if(maskBuffer2[(y * width + (x - 1)) * 4 + 3] == 0)
					maskBuffer[(y * width + x) * 4 + 3] = 0;
			}
			if (x < width-1)
			{
				if (maskBuffer2[(y * width + (x + 1)) * 4 + 3] == 0)
					maskBuffer[(y * width + x) * 4 + 3] = 0;
			}
			if (y > 0)
			{
				if (maskBuffer2[((y-1) * width + x) * 4 + 3] == 0)
					maskBuffer[(y * width + x) * 4 + 3] = 0;
			}
			if (y < height - 1)
			{
				if (maskBuffer2[((y+1) * width + x) * 4 + 3] == 0)
					maskBuffer[(y * width + x) * 4 + 3] = 0;
			}*/
		}
	}
	//free(maskBuffer2);
	//free(score);
};

unsigned char content_stdbuff[2048 * 2048 * 4];

void copyFile(const char* out_filename, int from_etap, int to_etap, int begin_type , int end_type, int from_frame, int begin_frame, int end_frame) {
	char infile[512];
	char outfile[512];
	char out_filename2[512];

	strcpy_s(out_filename2,out_filename);
	out_filename2[strlen(out_filename2)-2] = 0;
	for (int i= begin_type;i<= end_type;i++)
		for (int j = begin_frame; j <= end_frame; j++)
		{
			if (from_etap!= to_etap)from_frame = j;
			sprintf_s(infile, "%s%01d-%03d-%02i.data", out_filename2, from_etap, i, from_frame);
			sprintf_s(outfile, "%s%01d-%03d-%02i.data", out_filename2, to_etap, i, j);

			if (!file_exist(infile))break;

			FILE* fptr_std;
			fopen_s(&fptr_std, infile, "rb");
			fseek(fptr_std, 0L, SEEK_END);
			long szstd = ftell(fptr_std);
			fseek(fptr_std, 0L, SEEK_SET);
			fread(content_stdbuff, szstd, 1, fptr_std);
			fclose(fptr_std);

			FILE* fptr_outdata;
			fopen_s(&fptr_outdata, outfile, "wb");
			for (int bb = 0; bb < szstd; bb++)
				fwrite(&content_stdbuff[bb], 1, 1, fptr_outdata);
			fclose(fptr_outdata);

			sprintf_s(infile, "%s%01d-%03d-%02i.png", out_filename2, from_etap, i, from_frame);
			sprintf_s(outfile, "%s%01d-%03d-%02i.png", out_filename2, to_etap, i, j);

			if (!file_exist(infile))break;

			fopen_s(&fptr_std, infile, "rb");
			fseek(fptr_std, 0L, SEEK_END);
			szstd = ftell(fptr_std);
			fseek(fptr_std, 0L, SEEK_SET);
			fread(content_stdbuff, szstd, 1, fptr_std);
			fclose(fptr_std);

			fopen_s(&fptr_outdata, outfile, "wb");
			for (int bb = 0; bb < szstd; bb++)
				fwrite(&content_stdbuff[bb], 1, 1, fptr_outdata);
			fclose(fptr_outdata);

		}
}

void moveSome(const char* out_filename)
{
	copyFile(out_filename, 2, 2, 453, 453, 0, 1, 1);
	copyFile(out_filename, 2, 2, 454, 460, 0, 1, 24);
	copyFile(out_filename, 0, 2, 464, 504, 0, 0, 24);
	/*
	TMAPS2 - 2 - 453 - 01.pngB.png
		TMAPS2 - 2 - 454 - 01.pngB.png
		TMAPS2 - 2 - 455 - 01.pngB.png
		-
		TMAPS2 - 2 - 455 - 24.pngB.png
		TMAPS2 - 2 - 456 - 01.pngB.png
		-
		TMAPS2 - 2 - 456 - 24.pngB.png
		TMAPS2 - 2 - 457 - 01.pngB.png
		-
		TMAPS2 - 2 - 457 - 24.pngB.png
		TMAPS2 - 2 - 458 - 01.pngB.png
		-
		TMAPS2 - 2 - 458 - 24.pngB.png
		TMAPS2 - 2 - 459 - 01.pngB.png
		-
		TMAPS2 - 2 - 459 - 24.pngB.png
		TMAPS2 - 2 - 460 - 01.pngB.png
		-
		TMAPS2 - 2 - 460 - 24.pngB.png*/
}

void frBufferToBuffer(unsigned char* bigbufferAr, unsigned char* pngbufferAr, int width, int height, int frame) {
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
		{
			pngbufferAr[(y * width + x) * 4 + 0] = bigbufferAr[((y + frame) * (width + 2 * frame) + (x + frame)) * 4 + 0];
			pngbufferAr[(y * width + x) * 4 + 1] = bigbufferAr[((y + frame) * (width + 2 * frame) + (x + frame)) * 4 + 1];
			pngbufferAr[(y * width + x) * 4 + 2] = bigbufferAr[((y + frame) * (width + 2 * frame) + (x + frame)) * 4 + 2];
			pngbufferAr[(y * width + x) * 4 + 3] = bigbufferAr[((y + frame) * (width + 2 * frame) + (x + frame)) * 4 + 3];
		}
};

Bit8u buffer2[2048 * 2048 * 4];

unsigned char content_stdpal[2048 * 2048 * 4];

//unsigned char content_outdata[2048 * 2048 * 4];

int hex2int(char locchar) {
	if ((locchar >= '0') && (locchar <= '9'))
		return locchar - '0';
	if ((locchar >= 'a') && (locchar <= 'f'))
		return locchar - 'a';
	if ((locchar >= 'A') && (locchar <= 'F'))
		return locchar - 'a';
};

void loadColors(std::vector<uint8_t>* usedColor, char* path) {
	unsigned char content_col[10000];

	FILE* colfile;
	fopen_s(&colfile, path, "rt");
	fseek(colfile, 0L, SEEK_END);
	long szstd = ftell(colfile);
	fseek(colfile, 0L, SEEK_SET);
	fread(content_col, szstd, 1, colfile);
	fclose(colfile);

	usedColor->clear();
	for (int i = 0; i < szstd; i++)
	{
		if (content_col[i] == 'x')
		{
			int col = 16 * hex2int(content_col[i + 1]) + hex2int(content_col[i + 2]);
			usedColor->push_back(col);
			i += 3;
		}
	}
}

int main()
{
#ifdef level1
	const char* standartpal_filename = "c:\\prenos\\remc2\\tools\\palletelight\\Debug\\\out-n.pal";
	const char* data_filename = "c:\\prenos\\remc2\\tools\\decompressTMAPS\\out\\big\\TMAPS2-1-";
	const char* out_filename = "c:\\prenos\\remc2\\tools\\compressTMAPS\\compressTMAPS\\out\\TMAPS2-1-";
	//const char* orig_filename = "c:\\prenos\\remc2\\tools\\decompressTMAPS\\out\\TMAPS2-1-";
#endif level1
#ifdef level2
	const char* standartpal_filename = "c:\\prenos\\remc2\\tools\\palletelight\\Debug\\out-block.pal";
	const char* data_filename = "c:\\prenos\\remc2\\tools\\decompressTMAPS\\out\\big\\TMAPS2-0-";
	const char* out_filename = "c:\\prenos\\remc2\\tools\\compressTMAPS\\compressTMAPS\\out\\TMAPS2-0-";
	//const char* orig_filename = "c:\\prenos\\remc2\\tools\\decompressTMAPS\\out\\TMAPS2-0-";
#endif level2
#ifdef level4
	const char* standartpal_filename = "c:\\prenos\\remc2\\tools\\palletelight\\Debug\\out-c.pal";
	const char* data_filename = "c:\\prenos\\remc2\\tools\\decompressTMAPS\\out\\big\\TMAPS2-2-";
	const char* out_filename = "c:\\prenos\\remc2\\tools\\compressTMAPS\\compressTMAPS\\out\\TMAPS2-2-";
	//const char* orig_filename = "c:\\prenos\\remc2\\tools\\decompressTMAPS\\out\\TMAPS2-2-";
#endif level4
#ifdef rewriteBadFiles
	moveSome(out_filename);
#else
	FILE* fptr_stdpal;
	fopen_s(&fptr_stdpal, standartpal_filename, "rb");
	fseek(fptr_stdpal, 0L, SEEK_END);
	long szstd = ftell(fptr_stdpal);
	fseek(fptr_stdpal, 0L, SEEK_SET);
	fread(content_stdpal, szstd, 1, fptr_stdpal);
	fclose(fptr_stdpal);

	/*FILE* fptr_stdorig32;
	fopen_s(&fptr_stdorig32, orig32_filename, "rb");
	fseek(fptr_stdorig32, 0L, SEEK_END);
	long szstdorig32 = ftell(fptr_stdorig32);
	fseek(fptr_stdorig32, 0L, SEEK_SET);
	unsigned char* content_data32 = (unsigned char*)malloc(szstdorig32 * sizeof(char*));
	fread(content_data32, szstdorig32, 1, fptr_stdorig32);
	fclose(fptr_stdorig32);*/

	/*
	sprintf_s(buffer,"%s", data_filename);
	FILE* fptr_data;
	fopen_s(&fptr_data, buffer, "rb");
	fseek(fptr_data, 0L, SEEK_END);
	long szdata = ftell(fptr_data);
	fseek(fptr_data, 0L, SEEK_SET);
	unsigned char* content_data = (unsigned char*)malloc(szdata * sizeof(char*));
	fread(content_data, szdata, 1, fptr_data);
	fclose(fptr_data);

	sprintf_s(buffer, "%s", alpha_filename);
	FILE* fptr_alphadata;
	fopen_s(&fptr_alphadata, buffer, "rb");
	fseek(fptr_alphadata, 0L, SEEK_END);
	long szalpha = ftell(fptr_alphadata);
	fseek(fptr_alphadata, 0L, SEEK_SET);
	unsigned char* content_alphadata = (unsigned char*)malloc(szalpha * sizeof(char*));
	fread(content_alphadata, szalpha, 1, fptr_alphadata);
	fclose(fptr_alphadata);*/

	unsigned char* pngbufferAr[6];
	unsigned char* pngbufferfinal;

	pngbufferAr[0] = (unsigned char*)malloc(2048 * 2048 * 4);
	pngbufferAr[1] = (unsigned char*)malloc(2048 * 2048 * 4);
	pngbufferAr[2] = (unsigned char*)malloc(2048 * 2048 * 4);
	pngbufferAr[3] = (unsigned char*)malloc(2048 * 2048 * 4);
	pngbufferAr[4] = (unsigned char*)malloc(2048 * 2048 * 4);
	pngbufferAr[5] = (unsigned char*)malloc(2048 * 2048 * 4);
	pngbufferfinal = (unsigned char*)malloc(2048 * 2048 * 4);
	unsigned char* bigbufferAr = (unsigned char*)malloc(2048 * 2048 * 4);
	unsigned char* pngbuffer = (unsigned char*)malloc(2048 * 2048 * 4);

	for (int fileindex = 0; fileindex < 1000; fileindex++)
	//for(int fileindex=255; fileindex<1000; fileindex++)
	//for (int fileindex = 461; fileindex < 1000; fileindex++)
	//for (int fileindex = 450; fileindex < 1000; fileindex++)
	//	for (int fileindex = 198; fileindex < 199; fileindex++)
		for (int mainindex = 0; mainindex < 24; mainindex++)
	{		
		char textbuffer[512];
		char prefix[512];

		sprintf_s(prefix, "%s%03d-%02i%s-col.txt", data_filename);
		loadColors(&usedColors, prefix);


		sprintf_s(prefix, "%s", "");
		sprintf_s(textbuffer, "%s%03d-%02i%s.pngR_rlt.png", data_filename, fileindex,mainindex, prefix);
		if (!file_exist(textbuffer))
		{
			sprintf_s(prefix, "%s", "-other");
			sprintf_s(textbuffer, "%s%03d-%02i%s.pngR_rlt.png", data_filename, fileindex, mainindex, prefix);
			if (!file_exist(textbuffer))
				break;
		}
		int frame = 30 * 4;
		read_png_file(textbuffer);		
		width = width_fr;
		height = height_fr;
		pngToBuffer(bigbufferAr);
		width = width_fr - 2 * frame;
		height = height_fr - 2 * frame;
		frBufferToBuffer(bigbufferAr, pngbufferAr[0], width, height, frame);
		for (int y = 0; y < height_fr; y++) {
			free(row_pointers[y]);
		}
		free(row_pointers);

		sprintf_s(textbuffer, "%s%03d-%02i%s.pngG_rlt.png", data_filename, fileindex, mainindex, prefix);
		if (!file_exist(textbuffer))break;
		read_png_file(textbuffer);
		width = width_fr;
		height = height_fr;
		pngToBuffer(bigbufferAr);
		width = width_fr - 2 * frame;
		height = height_fr - 2 * frame;
		frBufferToBuffer(bigbufferAr, pngbufferAr[1], width, height, frame);
		for (int y = 0; y < height_fr; y++) {
			free(row_pointers[y]);
		}
		free(row_pointers);

		sprintf_s(textbuffer, "%s%03d-%02i%s.pngB_rlt.png", data_filename, fileindex, mainindex, prefix);
		if (!file_exist(textbuffer))break;
		read_png_file(textbuffer);
		width = width_fr;
		height = height_fr;
		pngToBuffer(bigbufferAr);
		width = width_fr - 2 * frame;
		height = height_fr - 2 * frame;
		frBufferToBuffer(bigbufferAr, pngbufferAr[2], width, height, frame);
		for (int y = 0; y < height_fr; y++) {
			free(row_pointers[y]);
		}
		free(row_pointers);

		sprintf_s(textbuffer, "%s%03d-%02i%s.pngBl_rlt.png", data_filename, fileindex, mainindex, prefix);
		if (!file_exist(textbuffer))break;
		read_png_file(textbuffer);
		width = width_fr;
		height = height_fr;
		pngToBuffer(bigbufferAr);
		width = width_fr - 2 * frame;
		height = height_fr - 2 * frame;
		frBufferToBuffer(bigbufferAr, pngbufferAr[3], width, height, frame);
		for (int y = 0; y < height_fr; y++) {
			free(row_pointers[y]);
		}
		free(row_pointers);

		sprintf_s(textbuffer, "%s%03d-%02i%s.pngGr_rlt.png", data_filename, fileindex, mainindex, prefix);
		if (!file_exist(textbuffer))break;
		read_png_file(textbuffer);
		width = width_fr;
		height = height_fr;
		pngToBuffer(bigbufferAr);
		width = width_fr - 2 * frame;
		height = height_fr - 2 * frame;
		frBufferToBuffer(bigbufferAr, pngbufferAr[4], width, height, frame);
		for (int y = 0; y < height_fr; y++) {
			free(row_pointers[y]);
		}
		free(row_pointers);

		sprintf_s(textbuffer, "%s%03d-%02i%s.pngWh_rlt.png", data_filename, fileindex, mainindex, prefix);
		if (!file_exist(textbuffer))break;
		read_png_file(textbuffer);
		width = width_fr;
		height = height_fr;
		pngToBuffer(bigbufferAr);
		width = width_fr - 2 * frame;
		height = height_fr - 2 * frame;
		frBufferToBuffer(bigbufferAr, pngbufferAr[5], width, height, frame);
		for (int y = 0; y < height_fr; y++) {
			free(row_pointers[y]);
		}
		free(row_pointers);

		sprintf_s(textbuffer, "%s%03d-%02i%s.pngFinal_rlt.png", data_filename, fileindex, mainindex, prefix);
		if (!file_exist(textbuffer))break;
		read_png_file(textbuffer);
		width = width_fr;
		height = height_fr;
		pngToBuffer(bigbufferAr);
		width = width_fr - 2 * frame;
		height = height_fr - 2 * frame;
		frBufferToBuffer(bigbufferAr, pngbufferfinal, width, height, frame);
		for (int y = 0; y < height_fr; y++) {
			free(row_pointers[y]);
		}
		free(row_pointers);

		//unsigned char* maskBuffer = (unsigned char*)malloc(width * height * 4);

		findMask(pngbuffer, pngbufferAr, pngbufferfinal);

		//sprintf_s(textbuffer, "%s%03d-%02i-mask.png", out_filename, fileindex, mainindex);
		//writeImagePNG((char*)textbuffer, width, height, pngbuffer, (char*)"aa");

		//free(maskBuffer);

		sprintf_s(textbuffer, "%s%03d-%02i.data", out_filename, fileindex, mainindex);
		FILE* fptr_outdata;
		fopen_s(&fptr_outdata, textbuffer, "wb");


		Bit8u alphaweight = 32;

		//unsigned char* pngbuffer = (unsigned char*)malloc(width * height * 4);

		/*for (int yy = 0; yy < height; yy++)
		{
			png_bytep row = row_pointers[yy];
			for (int xx = 0; xx < width; xx++) {
				png_bytep px = &(row[xx * 4]);
				Bit8u wrbyte = 0;		
				Bit8u datared = px[0];
				Bit8u datagreen = px[1];
				Bit8u datablue = px[2];
				Bit8u dataalpha = px[3];
				pngbuffer[4*(xx+width * yy) + 0] = datared;
				pngbuffer[4 * (xx + width * yy) + 1] = datagreen;
				pngbuffer[4 * (xx + width * yy) + 2] = datablue;
				pngbuffer[4 * (xx + width * yy) + 3] = dataalpha;
			}
		}*/

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

		/*sprintf_s(textbuffer, "%s%03d-%02i-test.png", out_filename, fileindex, mainindex);
		writeImagePNG((char*)textbuffer, width, height, pngbuffer, (char*)"aa");*/
		

		int j;

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
				
				x = getIndexedColor(colorR - colorRerr, colorG - colorGerr, colorB - colorBerr, content_stdpal, szstd);
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
			/*colorRIerr[i] = 0;
			colorGIerr[i] = 0;
			colorBIerr[i] = 0;*/
		}

		/*sprintf_s(textbuffer, "%s%03d-%02i-test2.png", out_filename, fileindex, mainindex);
		writeImagePNG((char*)textbuffer, width, height, buffer2, (char*)"aa");*/

		int minalpha = 128;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				bool isAlpha = false;
				if (pngbuffer[4 * (j + width * i) + 3] < minalpha)
					isAlpha = true;
				if(i>0)
					if (pngbuffer[4 * (j + width * (i - 1)) + 3] < minalpha)
						isAlpha = true;
				if (i < height-1)
					if (pngbuffer[4 * (j + width * (i + 1)) + 3] < minalpha)
						isAlpha = true;
				if (j > 0)
					if (pngbuffer[4 * ((j-1) + width * i) + 3] < minalpha)
						isAlpha = true;
				if (j < width - 1)
					if (pngbuffer[4 * ((j+1) + width * i) + 3] < minalpha)
						isAlpha = true;
				if ((i > 0) && ((j > 0)))
					if (pngbuffer[4 * ((j-1) + width * (i - 1)) + 3] < minalpha)
						isAlpha = true;
				if ((i < height - 1) && (j < width - 1))
					if (pngbuffer[4 * ((j+1) + width * (i + 1)) + 3] < minalpha)
						isAlpha = true;
				if ((i < height - 1) && (j > 0))
					if (pngbuffer[4 * ((j - 1) + width * (i+1)) + 3] < minalpha)
						isAlpha = true;
				if ((i > 0)&&(j < width - 1))
					if (pngbuffer[4 * ((j + 1) + width * (i-1)) + 3] < minalpha)
						isAlpha = true;

				if (isAlpha)
				{
					buffer[i * width + j] = transparentColor;
				}
			}
		}

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (buffer[i * width + j] == transparentColor)
					buffer2[(i * width + j) * 4 + 3] = 0;
				else
					buffer2[(i * width + j) * 4 + 3] = 255;
			}
		}



		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				fwrite(&buffer[i * width + j], 1, 1, fptr_outdata);

		sprintf_s(textbuffer, "%s%03d-%02i.png", out_filename, fileindex, mainindex);
		writeImagePNG((char*)textbuffer, width, height, buffer2, (char*)"aa");

		/*for (int y = 0; y < height; y++) {
			free(row_alphapointers[y]);
		}
		free(row_alphapointers);*/

		fclose(fptr_outdata);
	}

	for (int i = 0; i < 6; i++)
		free(pngbufferAr[i]);

	free(pngbufferfinal);

	free(pngbuffer);
#endif	
}
