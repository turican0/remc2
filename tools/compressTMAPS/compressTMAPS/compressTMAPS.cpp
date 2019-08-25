// compressTMAPS.cpp : Tento soubor obsahuje funkci main. Provádění programu se tam zahajuje a ukončuje.
//

#include <iostream>

#include "png.h"

#pragma comment(lib, "zlib.lib") // must be before libpng!
#ifndef _WIN64
#pragma comment(lib, "libpng15.lib") // must be after zlib!
#else
#endif

#define level2 // TMAPS0

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
png_byte color_type;
png_byte bit_depth;
png_bytep* row_pointers = NULL;


png_bytep* row_alphapointers = NULL;


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


int main()
{
#ifdef level1
	const char* standartpal_filename = "c:\\prenos\\remc2\\tools\\palletelight\\Debug\\\out-n.pal";
	const char* data_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\bl32n0-0-src.data";
	const char* outdata_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\bl128n0-0.data";
	const char* orig32_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\bl32n0-0.data";
#endif level1
#ifdef level2
	const char* standartpal_filename = "c:\\prenos\\remc2\\tools\\palletelight\\Debug\\out-block.pal";
	const char* data_filename = "c:\\prenos\\remc2\\tools\\decompressTMAPS\\out\\big\\";
	const char* alpha_filename = "c:\\prenos\\remc2\\tools\\decompressTMAPS\\out\\big\\";
	//const char* alpha_filename = "c:\\prenos\remc2\tools\\decompressTMAPS\\out\\alpha\\";
	const char* out_filename = "c:\\prenos\\remc2\\tools\\compressTMAPS\\compressTMAPS\\out\\";
#endif level2
#ifdef level4
	const char* standartpal_filename = "c:\\prenos\\remc2\\tools\\palletelight\\Debug\\out-c.pal";
	const char* data_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\bl32c0-0-src.data";
	const char* outdata_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\bl128c0-0.data";
	const char* orig32_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\bl32c0-0.data";
#endif level4

	FILE* fptr_stdpal;
	fopen_s(&fptr_stdpal, standartpal_filename, "rb");
	fseek(fptr_stdpal, 0L, SEEK_END);
	long szstd = ftell(fptr_stdpal);
	fseek(fptr_stdpal, 0L, SEEK_SET);
	unsigned char* content_stdpal = (unsigned char*)malloc(szstd * sizeof(char*));
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


	/*char buffer[512];
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

	for(int fileindex=0; fileindex<1000; fileindex++)
	{
		
		char buffer[512];
		sprintf_s(buffer, "%sTMAPS2-0-%03d_cartoonpainted_400000.png", data_filename, fileindex);
		if (!file_exist(buffer))break;
		read_png_file(buffer);


		sprintf_s(buffer, "%sTMAPS2-0-%03d-alpha_cartoonpainted_400000_gaus.png", alpha_filename, fileindex);//gaus 3x3
		//sprintf_s(buffer, "%sTMAPS2-0-%03d-alpha_cartoonpainted_400000.png", alpha_filename, fileindex);
		read_pngalpha_file(buffer);

		/*
		void process_png_file() {
	  for(int y = 0; y < height; y++) {
		png_bytep row = row_pointers[y];
		for(int x = 0; x < width; x++) {
		  png_bytep px = &(row[x * 4]);
		  // Do something awesome for each pixel here...
		  //printf("%4d, %4d = RGBA(%3d, %3d, %3d, %3d)\n", x, y, px[0], px[1], px[2], px[3]);
		}
	  }
	}
		*/

		sprintf_s(buffer, "%sTMAPS2-0-%03d.data", out_filename, fileindex);
		FILE* fptr_outdata;
		fopen_s(&fptr_outdata, buffer, "wb");


		Bit8u alphaweight = 32;
		/*int datapos = 54;
		int wpos = 18;
		int hpos = 22;


		int widthimg = *(Bit32u*)&content_data[wpos]; //18
		int heightimg = *(Bit32u*)&content_data[hpos];//22
		Bit8u* dataimg = content_data + datapos;
			//79x65 4f x 41

		Bit8u* alphaimg = content_alphadata + datapos;*/

		unsigned char* content_outdata = (unsigned char*)malloc(width * height * sizeof(char*));

		for (int yy = 0; yy < height; yy++)
		{
			png_bytep row = row_pointers[yy];
			png_bytep rowalpha = row_alphapointers[yy];
			for (int xx = 0; xx < width; xx++) {
				//int bmpx = xx;// widthimg - xx - 1;
				//int bmpy = heightimg- yy - 1;
				//Bit8u alphablue = alphaimg[3 * (bmpy * widthimg + bmpx)];
				png_bytep px = &(row[xx * 4]);
				png_bytep alpx = &(rowalpha[xx * 4]);
				Bit8u alphablue = alpx[2];
				Bit8u wrbyte = 0;
				if (alphablue > alphaweight) {
					Bit8u datared = px[0];
					Bit8u datagreen = px[1];
					Bit8u datablue = px[2];

					int best = 1000;
					unsigned char x = 0;
					for (int j = 0; j < szstd / 3; j++)
					{
						int score = 0;

						score += abs(px[0] - content_stdpal[j * 3 + 0]);
						score += abs(px[1] - content_stdpal[j * 3 + 1]);
						score += abs(px[2] - content_stdpal[j * 3 + 2]);

						bool notinremoved = true;
						/*for (int kk = 0; kk < counterremoved; kk++)
							if (removed[kk] == j)notinremoved = false;*/
						if (notinremoved)
							if (score < best)
							{
								best = score;
								x = j;
							}
					}


					wrbyte = x;
				}

				fwrite(&wrbyte, 1, 1, fptr_outdata);

			}
		}

		for (int y = 0; y < height; y++) {
			free(row_pointers[y]);
		}
		free(row_pointers);

		for (int y = 0; y < height; y++) {
			free(row_alphapointers[y]);
		}
		free(row_alphapointers);

		fclose(fptr_outdata);
	}

}

// Spuštění programu: Ctrl+F5 nebo nabídka Ladit > Spustit bez ladění
// Ladění programu: F5 nebo nabídka Ladit > Spustit ladění

// Tipy pro zahájení práce:
//   1. K přidání nebo správě souborů použijte okno Průzkumník řešení.
//   2. Pro připojení ke správě zdrojového kódu použijte okno Team Explorer.
//   3. K zobrazení výstupu sestavení a dalších zpráv použijte okno Výstup.
//   4. K zobrazení chyb použijte okno Seznam chyb.
//   5. Pokud chcete vytvořit nové soubory kódu, přejděte na Projekt > Přidat novou položku. Pokud chcete přidat do projektu existující soubory kódu, přejděte na Projekt > Přidat existující položku.
//   6. Pokud budete chtít v budoucnu znovu otevřít tento projekt, přejděte na Soubor > Otevřít > Projekt a vyberte příslušný soubor .sln.
