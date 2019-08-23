//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>

//#define RGBA
#define level1
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

int main(int argc, char* argv[]) {
#ifdef level1
	const char* standartpal_filename = "c:\\prenos\\remc2\\tools\\palletelight\\Debug\\\out-n.pal";
	const char* data_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\bl32n0-0-src.data";
	const char* outdata_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\bl128n0-0.data";
	const char* orig32_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\bl32n0-0.data";
#endif level1
#ifdef level2
	const char* standartpal_filename = "c:\\prenos\\remc2\\tools\\palletelight\\Debug\\out-block.pal";
	const char* data_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\block32-src.data";
	const char* outdata_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\block128.data";
	const char* orig32_filename = "c:\\prenos\\remc2\\Debug\\biggraphics\\block32.data";
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

	FILE* fptr_stdorig32;
	fopen_s(&fptr_stdorig32, orig32_filename, "rb");
	fseek(fptr_stdorig32, 0L, SEEK_END);
	long szstdorig32 = ftell(fptr_stdorig32);
	fseek(fptr_stdorig32, 0L, SEEK_SET);
	unsigned char* content_data32 = (unsigned char*)malloc(szstdorig32 * sizeof(char*));
	fread(content_data32, szstdorig32, 1, fptr_stdorig32);
	fclose(fptr_stdorig32);
	/*
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
		*/
	FILE* fptr_data;
	fopen_s(&fptr_data, data_filename, "rb");
	fseek(fptr_data, 0L, SEEK_END);
	long szdata = ftell(fptr_data);
	fseek(fptr_data, 0L, SEEK_SET);
	unsigned char* content_data = (unsigned char*)malloc(szdata * sizeof(char*));
	fread(content_data, szdata, 1, fptr_data);
	fclose(fptr_data);

	//find water colors
	int watercolorindex = 0;
	unsigned char watercolor[256];
	for (int i = 0; i < 32 * 256; i++)
		//for (int j = 0; j < 32; j++)
	{
		int xindex = (i / 32) % 8;
		int yindex = i / (32 * 32 * 8);
		int tileindex = yindex * 8 + xindex;
		int xpos = (i - yindex * 32 * 32 * 8 - xindex * 32) % 32;
		int ypos = (i - (yindex * 32 * 32 * 8)) / (32 * 8);
		if (tileindex == 0)
		{
			bool notinindex = true;
			for (int ll = 0; ll < watercolorindex; ll++)
				if (watercolor[ll] == content_data32[i])
					notinindex = false;
			if (notinindex)
				watercolor[watercolorindex++] = content_data32[i];
		}
	}

	//find water colors

	//find water colors2
	int watercolorindex2 = 0;
	unsigned char watercolor2[256];
	for (int i = 0; i < 128 * 256; i++)
		//for (int j = 0; j < 32; j++)
	{
		int xindex = (i / 128) % 2;
		int yindex = i / (128 * 128 * 2);
		int tileindex = yindex * 2 + xindex;
		int xpos = (i - yindex * 128 * 128 * 2 - xindex * 128) % 128;
		int ypos = (i - (yindex * 128 * 128 * 2)) / (128 * 2);
		if (tileindex == 0)
		{
			bool notinindex = true;

			int best = 1000;
			unsigned char x = 0;
			for (int j = 0; j < szstd / 3; j++)
			{
				int score = 0;
				#ifdef RGBA
								score += abs(content_data[i * 4 + 0] - content_stdpal[j * 3 + 0]);
								score += abs(content_data[i * 4 + 1] - content_stdpal[j * 3 + 1]);
								score += abs(content_data[i * 4 + 2] - content_stdpal[j * 3 + 2]);
				#else
								score += abs(content_data[i * 3 + 0] - content_stdpal[j * 3 + 0]);
								score += abs(content_data[i * 3 + 1] - content_stdpal[j * 3 + 1]);
								score += abs(content_data[i * 3 + 2] - content_stdpal[j * 3 + 2]);
				#endif
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

			for (int ll = 0; ll < watercolorindex2; ll++)
				if (watercolor2[ll] == x)
					notinindex = false;
			if (notinindex)
				watercolor2[watercolorindex2++] = x;
		}
	}
	
	//find water colors2
	int outwatercolorindex = 0;
	unsigned char outwatercolor[256];
	/*
	for (int i = 0; i < watercolorindex2; i++)
	{
		bool add = true;
		for (int j = 0; j < watercolorindex; j++)
		{
			if (watercolor2[i] == watercolor[j])
				add = false;
		}
		if (add)
			outwatercolor[outwatercolorindex++] = watercolor2[i];
	}
	*/
#ifdef level1
	//watercolor2[watercolorindex2++] = 0x9e;
	//watercolor2[watercolorindex2++] = 0xa2;
#endif level1
#ifdef level2
	//outwatercolor[outwatercolorindex++] = 0x05;
	//outwatercolor[outwatercolorindex++] = 0x06;
	//outwatercolor[outwatercolorindex++] = 0x07;
	//outwatercolor[outwatercolorindex++] = 0x8;
	//outwatercolor[outwatercolorindex++] = 0x9;
	//outwatercolor[outwatercolorindex++] = 0xa;
	//outwatercolor[outwatercolorindex++] = 0xb;
	//outwatercolor[outwatercolorindex++] = 0x5b;
	//outwatercolor[outwatercolorindex++] = 0xaf;
#endif level2

	FILE* fptw_outdata;
	fopen_s(&fptw_outdata, outdata_filename, "wb");

#ifdef RGBA
		for (int i = 0; i < szdata / 4; i++)
#else
		for (int i = 0; i < szdata / 3; i++)
#endif
	{

		if (i % 1000)
			printf("%f percent compledted\n",((float)i/(szdata / 3))*100);

		int xindex = (i/128) % 2;
		int yindex = i / (128 * 128 * 2);
		int tileindex = yindex * 2 + xindex;
		int xpos = (i - yindex * 128 * 128 * 2 - xindex * 128)%128;
		int ypos = (i - (yindex * 128 * 128 * 2)) / (128 * 2);
		
		/*
		bool inindex = false;
		int score2 = 0;
		score2 += abs(content_data[i * 3 + 0] - 0x30);
		score2 += abs(content_data[i * 3 + 1] - 0x30);
		score2 += abs(content_data[i * 3 + 2] - 0x30);
		if(score2<5)
			inindex = true;
		*/
		bool inindex = false;
		//if(get32colorIndex(content_data32, xindex, yindex, xpos, ypos)==1)inindex = true;
		//if (get32colorIndex(content_data32, xindex, yindex, xpos, ypos) == 2)inindex = true;
		//0 1 2 3 4 5
		//int counterremoved = 0;
		//unsigned int removed[256];
		//bool again = true;
		unsigned char x;
		//while (again)
		{
			int best = 1000;
			x = 0;
			for (int j = 0; j < szstd / 3; j++)

#ifdef level1
				if ((j != 0x24) && (j != 0x3D) && (j != 0x41) && (j != 0xC7) && (j != 0x22) && (j != 0x23)&&(j != 0xa1) && (j != 0x3e))
#endif level1
#ifdef level2
				if ((j != 0x67))
#endif level2
#ifdef level4
					if (true)
#endif level4
			{
				int score = 0;
#ifdef RGBA
				score += abs(content_data[i * 4 + 0] - content_stdpal[j * 3 + 0]);
				score += abs(content_data[i * 4 + 1] - content_stdpal[j * 3 + 1]);
				score += abs(content_data[i * 4 + 2] - content_stdpal[j * 3 + 2]);
#else
				score += abs(content_data[i * 3 + 0] - content_stdpal[j * 3 + 0]);
				score += abs(content_data[i * 3 + 1] - content_stdpal[j * 3 + 1]);
				score += abs(content_data[i * 3 + 2] - content_stdpal[j * 3 + 2]);
#endif
				/*bool skip = false;
				if (tileindex == 0)
				{
					bool inindex2 = false;
					for (int mm = 0; mm < watercolorindex; mm++)
						if (j == watercolor[mm])
							inindex2 = true;
					if (!inindex2)
					{			
						bool inindex = false;
						for (int mm = 0; mm < outwatercolorindex; mm++)
							if (j == outwatercolor[mm])
								inindex = true;
						if(!inindex)outwatercolor[outwatercolorindex++] = j;	
						skip = true;
					}
				}*/
				//if (!skip)
				if (score < best)
				{ 					
					/*for (int mm = 0; mm < outwatercolorindex; mm++)
						if (j == outwatercolor[mm])*/
					/*if(tileindex ==0)
							inindex = true;
					if ((tileindex == 21)|| (tileindex == 31))
					if(xpos>ypos)
					{
						best = 0;
						x = 0;
						break;
					}*/
					if (inindex)
					{
						if (get32color(content_data32, j, xindex, yindex, xpos, ypos))
						{
							best = score;
							x = j;
						}
					}
					else
					{
						best = score;
						x = j;
					}
				}
			}
			/*again = false;
			//if ((tileindex == 0) && (j != 1) && (j != 2))
			bool inindex = false;
			for (int mm = 0; mm < outwatercolorindex; mm++)
				if (x==outwatercolor[mm])
					inindex = true;
			//if ((x == 5) || (x == 7) || (x == 8) || (x == 9) || (x == 0xa))
			//if(inindex)
			{
				//x = 0;
				if (!get32color(content_data32, x, xindex, yindex, xpos, ypos))
				{
					again = true;
					removed[counterremoved++] = x;
				}		
			}*/
		}

		/*
		unsigned char x = index2[content_data[i]];
		*/
		fwrite(&x, 1, 1, fptw_outdata);
	}
#ifdef RGBA
	for (int i = szdata / 4; i < 160 * 128 * 128; i++)
#else
	for (int i = szdata / 3; i < 160 * 128 * 128; i++)
#endif
	{
		unsigned char x = 0xff;
		fwrite(&x, 1, 1, fptw_outdata);
	}// sss
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
