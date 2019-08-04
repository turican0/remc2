//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char* argv[]) {
	FILE* fptr;
	fopen_s(&fptr, "c:\\prenos\\remc2\\tools\\palletelight\\Debug\\level4.pal", "rb");

	fseek(fptr, 0L, SEEK_END);
	long sz = ftell(fptr);
	fseek(fptr, 0L, SEEK_SET);

	unsigned char* content = (unsigned char*)malloc(sz * sizeof(char*));

	fread(content, sz, 1, fptr);
	fclose(fptr);

	FILE* fptw;
	fopen_s(&fptw, "c:\\prenos\\remc2\\tools\\palletelight\\Debug\\out-d.pal", "wb");

	for (int i = 0; i < 256*3; i++)
	{
		unsigned char x = content[i] * 4;
		fwrite(&x, 1, 1, fptw);
	}
	fclose(fptw);
	return 0;
}
