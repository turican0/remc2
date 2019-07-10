//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char* argv[]) {
	const char* standartpal_filename = "c:\\prenos\\remc2\\tools\\palletelight\\Debug\\\out.pal";
	const char* atyppal_filename = "c:\\prenos\\remc2\\biggraphics\\out_rlt-ff.data.pal";
	//const char* data_filename = "c:\\prenos\\remc2\\biggraphics\\out_rlt-ff-rgb.data";
	const char* data_filename = "c:\\prenos\\remc2\\biggraphics\\out_rlt-n.data";
	//const char* data_filename = "c:\\prenos\\remc2\\biggraphics\\test.data";
	const char* outdata_filename = "c:\\prenos\\remc2\\biggraphics\\out_rlt-n-out.data";

	FILE* fptr_stdpal;
	fopen_s(&fptr_stdpal, standartpal_filename, "rb");
	fseek(fptr_stdpal, 0L, SEEK_END);
	long szstd = ftell(fptr_stdpal);
	fseek(fptr_stdpal, 0L, SEEK_SET);
	unsigned char* content_stdpal = (unsigned char*)malloc(szstd * sizeof(char*));
	fread(content_stdpal, szstd, 1, fptr_stdpal);
	fclose(fptr_stdpal);

	FILE* fptr_atyppal;
	fopen_s(&fptr_atyppal, atyppal_filename, "rb");
	fseek(fptr_atyppal, 0L, SEEK_END);
	long szatyp = ftell(fptr_atyppal);
	fseek(fptr_atyppal, 0L, SEEK_SET);
	unsigned char* content_atyppal = (unsigned char*)malloc(szatyp * sizeof(char*));
	fread(content_atyppal, szatyp, 1, fptr_atyppal);
	fclose(fptr_atyppal);

	unsigned char index1[256];
	unsigned char index2[256];

	for (int i = 0; i < szstd / 3; i++)
		for (int j = 0; j < szatyp / 3; j++)
		{
			if (
				(content_stdpal[i * 3 + 0] == content_atyppal[j * 3 + 0]) &&
				(content_stdpal[i * 3 + 1] == content_atyppal[j * 3 + 1]) &&
				(content_stdpal[i * 3 + 2] == content_atyppal[j * 3 + 2])
				)
			{
				index1[i] = j;
				index2[j] = i;
			}
		}

	FILE* fptr_data;
	fopen_s(&fptr_data, data_filename, "rb");
	fseek(fptr_data, 0L, SEEK_END);
	long szdata = ftell(fptr_data);
	fseek(fptr_data, 0L, SEEK_SET);
	unsigned char* content_data = (unsigned char*)malloc(szdata * sizeof(char*));
	fread(content_data, szdata, 1, fptr_data);
	fclose(fptr_data);


	FILE* fptw_outdata;
	fopen_s(&fptw_outdata, outdata_filename, "wb");

	for (int i = 0; i < szdata / 3; i++)
	{
		int best = 1000;
		unsigned char x = 0;
		for (int j = 0; j < szstd / 3; j++)
		{
			int score = 0;
			score += abs(content_data[i * 3 + 0] - content_stdpal[j * 3 + 0]);
			score += abs(content_data[i * 3 + 1] - content_stdpal[j * 3 + 1]);
			score += abs(content_data[i * 3 + 2] - content_stdpal[j * 3 + 2]);
			if (score < best)
			{
				best = score;
				x = j;
			}
		}
		/*
		unsigned char x = index2[content_data[i]];
		*/
		fwrite(&x, 1, 1, fptw_outdata);
	}
	for (int i = szdata / 3; i < 160 * 128 * 128; i++)
	{
		unsigned char x = 0xff;
		fwrite(&x, 1, 1, fptw_outdata);
	}
	/*
	for (int i = 119; i < 256; i++)
		for (int j = 0; j < 128 * 10; j++)
		{
			unsigned char x = i;
			fwrite(&x, 1, 1, fptw_outdata);
		}
	*/
	fclose(fptw_outdata);
	
	return 0;
}
