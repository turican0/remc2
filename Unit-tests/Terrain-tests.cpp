#include "Terrain-tests.h"

//----- (00045AA0) --------------------------------------------------------
void sub_45AA0_setMax4Tiles_orig()//226aa0
{
	uaxis_2d indexx; // ax
	char v1; // dh
	//unsigned __int16 v2; // cx
	unsigned __int8 v3; // dl
	unsigned __int8 v4; // bl
	//unsigned __int16 v5; // cx
	//unsigned __int16 v6; // cx
	//unsigned __int16 v7; // cx
	//unsigned __int16 v8; // cx
	char v9; // [esp+0h] [ebp-4h]

	//[*][*]
	//[*][*]

	do
	{
		indexx.word = 0;
		v9 = 0;
		do
		{
			v1 = 0;
			if (!mapAngle_13B4E0[indexx.word])
				v1 = 1;
			//v2 = index;
			v3 = mapHeightmap_11B4E0[indexx.word];
			v4 = mapHeightmap_11B4E0[indexx.word];
			//LOBYTE(index)++;
			indexx._axis_2d.x++;
			if (!mapAngle_13B4E0[indexx.word])
				v1++;
			if (v3 > mapHeightmap_11B4E0[indexx.word])
				v3 = mapHeightmap_11B4E0[indexx.word];
			if (v4 < mapHeightmap_11B4E0[indexx.word])
				v4 = mapHeightmap_11B4E0[indexx.word];
			//HIBYTE(index)++;
			indexx._axis_2d.y++;
			if (!mapAngle_13B4E0[indexx.word])
				v1++;
			if (v3 > mapHeightmap_11B4E0[indexx.word])
				v3 = mapHeightmap_11B4E0[indexx.word];
			if (v4 < mapHeightmap_11B4E0[indexx.word])
				v4 = mapHeightmap_11B4E0[indexx.word];
			//LOBYTE(index)--;
			indexx._axis_2d.x--;
			if (!mapAngle_13B4E0[indexx.word])
				v1++;
			if (v3 > mapHeightmap_11B4E0[indexx.word])
				v3 = mapHeightmap_11B4E0[indexx.word];
			if (v4 < mapHeightmap_11B4E0[indexx.word])
				v4 = mapHeightmap_11B4E0[indexx.word];
			//HIBYTE(index)--;
			indexx._axis_2d.y--;
			if (v4 != v3 && v1 == 4)
			{
				v9 = 1;
				//v5 = index;
				mapHeightmap_11B4E0[indexx.word] = v3;
				//LOBYTE(index)++;
				indexx._axis_2d.x++;
				//v6 = index;
				mapHeightmap_11B4E0[indexx.word] = v3;
				//HIBYTE(index)++;
				indexx._axis_2d.y++;
				//v7 = index;
				mapHeightmap_11B4E0[indexx.word] = v3;
				//LOBYTE(index)--;
				indexx._axis_2d.x--;
				//v8 = index;
				mapHeightmap_11B4E0[indexx.word] = v3;
				//HIBYTE(index)--;
				indexx._axis_2d.y--;
			}
			indexx.word++;
		} while (indexx.word);
	} while (v9);
}

//----- (000440D0) --------------------------------------------------------
void sub_440D0_orig(unsigned __int16 a1)//2250d0
{
	unsigned __int16 i; // ax
	unsigned __int8 v2; // dh
	unsigned __int8 v3; // dl
	//unsigned __int16 v4; // ax
	//unsigned __int16 v5; // ax
	int v6; // ebx
	char v7; // bl
	char v8; // dl
	char v9; // dh

	// fix if begin
	//v4 = 0;
	//v5 = 0;
	// end

	i = 0;
	do
	{
		if (mapAngle_13B4E0[i] == 5)
		{
			v2 = 0;
			v3 = 0xFFu;
			if (mapHeightmap_11B4E0[i])
				v2 = mapHeightmap_11B4E0[i];
			if (mapHeightmap_11B4E0[i] < 0xFFu)
				v3 = mapHeightmap_11B4E0[i];
			HIBYTE(i)--;
			if (v2 < mapHeightmap_11B4E0[i])
				v2 = mapHeightmap_11B4E0[i];
			if (v3 > mapHeightmap_11B4E0[i])
				v3 = mapHeightmap_11B4E0[i];
			LOBYTE(i)++;
			HIBYTE(i)++;
			if (v2 < mapHeightmap_11B4E0[i])
				v2 = mapHeightmap_11B4E0[i];
			if (v3 > mapHeightmap_11B4E0[i])
				v3 = mapHeightmap_11B4E0[i];
			LOBYTE(i)--;
			HIBYTE(i)++;
			if (v2 < mapHeightmap_11B4E0[i])
				v2 = mapHeightmap_11B4E0[i];
			if (v3 > mapHeightmap_11B4E0[i])
				v3 = mapHeightmap_11B4E0[i];
			LOBYTE(i)--;
			HIBYTE(i)--;
			if (v2 < mapHeightmap_11B4E0[i])
				v2 = mapHeightmap_11B4E0[i];
			if (v3 > mapHeightmap_11B4E0[i])
				v3 = mapHeightmap_11B4E0[i];
			v6 = v2 - v3;
			LOBYTE(i)++;
			if (v6 <= a1)
			{
				if (v6 == a1)
					mapAngle_13B4E0[i] = 4;
				else
					mapAngle_13B4E0[i] = 3;
			}
		}
		i++;
	} while (i);
	do
	{
		v7 = 0;
		v8 = 0;
		v9 = 0;
		if (mapAngle_13B4E0[i] == 3)
			v7 = 1;
		if (mapAngle_13B4E0[i] == 2)
			v8 = 1;
		if (mapAngle_13B4E0[i] == 5)
			v9 = 1;
		LOBYTE(i)++;
		if (mapAngle_13B4E0[i] == 3)
			v7++;
		if (mapAngle_13B4E0[i] == 2)
			v8++;
		if (mapAngle_13B4E0[i] == 5)
			v9++;
		HIBYTE(i)++;
		if (mapAngle_13B4E0[i] == 3)
			v7++;
		if (mapAngle_13B4E0[i] == 2)
			v8++;
		if (mapAngle_13B4E0[i] == 5)
			v9++;
		LOBYTE(i)--;
		if (mapAngle_13B4E0[i] == 3)
			v7++;
		if (mapAngle_13B4E0[i] == 2)
			v8++;
		if (mapAngle_13B4E0[i] == 5)
			v9++;
		HIBYTE(i)--;
		if (!v8 && v7 && v9)
		{
			if (mapAngle_13B4E0[i] == 3)
				mapAngle_13B4E0[i] = 4;
			LOBYTE(i)++;
			if (mapAngle_13B4E0[i] == 3)
				mapAngle_13B4E0[i] = 4;
			HIBYTE(i)++;
			if (mapAngle_13B4E0[i] == 3)
				mapAngle_13B4E0[i] = 4;
			LOBYTE(i)--;
			if (mapAngle_13B4E0[i] == 3)
				mapAngle_13B4E0[i] = 4;
			HIBYTE(i)--;
		}
		i++;
	} while (i);
	//  return result;
}

//----- (00045060) --------------------------------------------------------
void sub_45060_orig(uint8_t a1, uint8_t a2)//226060
{
	uint16_t v2; // dx
	uint8_t index; // al
	uint8_t v4; // ah
	//unsigned __int16 v5; // dx

	qmemcpy(mapTerrainType_10B4E0, mapAngle_13B4E0, 0x10000);
	v2 = 0;
	do
	{
		index = 0;
		v4 = -1;
		if (mapHeightmap_11B4E0[v2])
			index = mapHeightmap_11B4E0[v2];
		if (mapHeightmap_11B4E0[v2] < 0xFFu)
			v4 = mapHeightmap_11B4E0[v2];
		HIBYTE(v2)--;
		if (index < mapHeightmap_11B4E0[v2])
			index = mapHeightmap_11B4E0[v2];
		if (v4 > mapHeightmap_11B4E0[v2])
			v4 = mapHeightmap_11B4E0[v2];
		LOBYTE(v2)++;
		if (index < mapHeightmap_11B4E0[v2])
			index = mapHeightmap_11B4E0[v2];
		if (v4 > mapHeightmap_11B4E0[v2])
			v4 = mapHeightmap_11B4E0[v2];
		HIBYTE(v2)++;
		if (index < mapHeightmap_11B4E0[v2])
			index = mapHeightmap_11B4E0[v2];
		if (v4 > mapHeightmap_11B4E0[v2])
			v4 = mapHeightmap_11B4E0[v2];
		HIBYTE(v2)++;
		if (index < mapHeightmap_11B4E0[v2])
			index = mapHeightmap_11B4E0[v2];
		if (v4 > mapHeightmap_11B4E0[v2])
			v4 = mapHeightmap_11B4E0[v2];
		LOBYTE(v2)--;
		if (index < mapHeightmap_11B4E0[v2])
			index = mapHeightmap_11B4E0[v2];
		if (v4 > mapHeightmap_11B4E0[v2])
			v4 = mapHeightmap_11B4E0[v2];
		LOBYTE(v2)--;
		if (index < mapHeightmap_11B4E0[v2])
			index = mapHeightmap_11B4E0[v2];
		if (v4 > mapHeightmap_11B4E0[v2])
			v4 = mapHeightmap_11B4E0[v2];
		HIBYTE(v2)--;
		if (index < mapHeightmap_11B4E0[v2])
			index = mapHeightmap_11B4E0[v2];
		if (v4 > mapHeightmap_11B4E0[v2])
			v4 = mapHeightmap_11B4E0[v2];
		HIBYTE(v2)--;
		if (index < mapHeightmap_11B4E0[v2])
			index = mapHeightmap_11B4E0[v2];
		if (v4 > mapHeightmap_11B4E0[v2])
			v4 = mapHeightmap_11B4E0[v2];
		LOBYTE(v2)++;
		HIBYTE(v2)++;
		if (index < a1 && index - v4 <= a2)
		{
			if (mapAngle_13B4E0[v2])
				mapAngle_13B4E0[v2] = 5;
		}
		v2++;
	} while (v2);
}

//----- (00045210) --------------------------------------------------------
void sub_45210_orig(uint8_t a1, uint8_t a2)//226210
{
	uint16_t v2; // dx
	uint16_t v3; // eax
	//char v4; // t1
	uint8_t v5; // bl
	uint8_t v6; // bh
	//unsigned __int16 v7; // dx
	int v8; // ecx

	//fix
	v3 = 0;
	//fix

	qmemcpy(mapTerrainType_10B4E0, mapAngle_13B4E0, 0x10000);
	v2 = 0;
	do
	{
		BYTE1(v3) = 0xffu;
		//v4 = x_BYTE_13B4E0_angle[v2];
		v5 = 0;
		v6 = 0;
		if (mapHeightmap_11B4E0[v2] > 0u)
			LOBYTE(v3) = mapHeightmap_11B4E0[v2];
		if (mapHeightmap_11B4E0[v2] < 0xFFu)
			BYTE1(v3) = mapHeightmap_11B4E0[v2];
		HIBYTE(v2)--;
		if (LOBYTE(v3) < mapHeightmap_11B4E0[v2])
			LOBYTE(v3) = mapHeightmap_11B4E0[v2];
		if (BYTE1(v3) > mapHeightmap_11B4E0[v2])
			BYTE1(v3) = mapHeightmap_11B4E0[v2];
		if (mapAngle_13B4E0[v2] == 5)
			v6 = 1;
		if (mapAngle_13B4E0[v2] == 2)
			v5 = 1;
		LOBYTE(v2)++;
		if (LOBYTE(v3) < mapHeightmap_11B4E0[v2])
			LOBYTE(v3) = mapHeightmap_11B4E0[v2];
		if (BYTE1(v3) > mapHeightmap_11B4E0[v2])
			BYTE1(v3) = mapHeightmap_11B4E0[v2];
		if (mapAngle_13B4E0[v2] == 5)
			v6++;
		if (mapAngle_13B4E0[v2] == 2)
			v5++;
		HIBYTE(v2)++;
		if (LOBYTE(v3) < mapHeightmap_11B4E0[v2])
			LOBYTE(v3) = mapHeightmap_11B4E0[v2];
		if (BYTE1(v3) > mapHeightmap_11B4E0[v2])
			BYTE1(v3) = mapHeightmap_11B4E0[v2];
		if (mapAngle_13B4E0[v2] == 5)
			v6++;
		if (mapAngle_13B4E0[v2] == 2)
			v5++;
		HIBYTE(v2)++;
		if (LOBYTE(v3) < mapHeightmap_11B4E0[v2])
			LOBYTE(v3) = mapHeightmap_11B4E0[v2];
		if (BYTE1(v3) > mapHeightmap_11B4E0[v2])
			BYTE1(v3) = mapHeightmap_11B4E0[v2];
		if (mapAngle_13B4E0[v2] == 5)
			v6++;
		if (mapAngle_13B4E0[v2] == 2)
			v5++;
		LOBYTE(v2)--;
		if (LOBYTE(v3) < mapHeightmap_11B4E0[v2])
			LOBYTE(v3) = mapHeightmap_11B4E0[v2];
		if (BYTE1(v3) > mapHeightmap_11B4E0[v2])
			BYTE1(v3) = mapHeightmap_11B4E0[v2];
		if (mapAngle_13B4E0[v2] == 5)
			v6++;
		if (mapAngle_13B4E0[v2] == 2)
			v5++;
		LOBYTE(v2)--;
		if (LOBYTE(v3) < mapHeightmap_11B4E0[v2])
			LOBYTE(v3) = mapHeightmap_11B4E0[v2];
		if (BYTE1(v3) > mapHeightmap_11B4E0[v2])
			BYTE1(v3) = mapHeightmap_11B4E0[v2];
		if (mapAngle_13B4E0[v2] == 5)
			v6++;
		if (mapAngle_13B4E0[v2] == 2)
			v5++;
		HIBYTE(v2)--;
		if (LOBYTE(v3) < mapHeightmap_11B4E0[v2])
			LOBYTE(v3) = mapHeightmap_11B4E0[v2];
		if (BYTE1(v3) > mapHeightmap_11B4E0[v2])
			BYTE1(v3) = mapHeightmap_11B4E0[v2];
		if (mapAngle_13B4E0[v2] == 5)
			v6++;
		if (mapAngle_13B4E0[v2] == 2)
			v5++;
		HIBYTE(v2)--;
		if (LOBYTE(v3) < mapHeightmap_11B4E0[v2])
			LOBYTE(v3) = mapHeightmap_11B4E0[v2];
		if (BYTE1(v3) > mapHeightmap_11B4E0[v2])
			BYTE1(v3) = mapHeightmap_11B4E0[v2];
		if (mapAngle_13B4E0[v2] == 5)
			v6++;
		if (mapAngle_13B4E0[v2] == 2)
			v5++;
		LOBYTE(v2)++;
		HIBYTE(v2)++;
		if (LOBYTE(v3) < a1)
		{
			v8 = LOBYTE(v3) - BYTE1(v3);
			LOBYTE(v3) = a2;
			if (v8 <= a2 && mapAngle_13B4E0[v2] == 5)
			{
				v3 = v6 + v5;
				if (v3 == 8)
					mapAngle_13B4E0[v2] = 2;
			}
		}
		v2++;
	} while (v2);
}
// 10000: using guessed type void /*__noreturn*/ sub_10000();

//----- (000454F0) --------------------------------------------------------
void sub_454F0_orig(uint8_t a1, uint8_t a2)//2264f0
{
	uint16_t index; // ax
	uint8_t v3; // dh
	uint8_t v4; // dl
	//unsigned __int16 v5; // ax
	//uint16_t v6; // ax

	// fix if begin
	//v5 = 0;
	//v6 = 0;
	// end

	index = 0;
	do
	{
		if (mapHeightmap_11B4E0[index] > a1)
		{
			v3 = 0;
			v4 = -1;
			if (mapHeightmap_11B4E0[index])
				v3 = mapHeightmap_11B4E0[index];
			if (mapHeightmap_11B4E0[index] < 0xFFu)
				v4 = mapHeightmap_11B4E0[index];
			HIBYTE(index)--;
			if (v3 < mapHeightmap_11B4E0[index])
				v3 = mapHeightmap_11B4E0[index];
			if (v4 > mapHeightmap_11B4E0[index])
				v4 = mapHeightmap_11B4E0[index];
			LOBYTE(index)++;
			HIBYTE(index)++;
			if (v3 < mapHeightmap_11B4E0[index])
				v3 = mapHeightmap_11B4E0[index];
			if (v4 > mapHeightmap_11B4E0[index])
				v4 = mapHeightmap_11B4E0[index];
			HIBYTE(index)++;
			LOBYTE(index)--;
			if (v3 < mapHeightmap_11B4E0[index])
				v3 = mapHeightmap_11B4E0[index];
			if (v4 > mapHeightmap_11B4E0[index])
				v4 = mapHeightmap_11B4E0[index];
			LOBYTE(index)--;
			HIBYTE(index)--;
			if (v3 < mapHeightmap_11B4E0[index])
				v3 = mapHeightmap_11B4E0[index];
			if (v4 > mapHeightmap_11B4E0[index])
				v4 = mapHeightmap_11B4E0[index];
			LOBYTE(index)++;
			if (mapAngle_13B4E0[index])
			{
				if (v3 - v4 < a2)
					mapAngle_13B4E0[index] = 6;
			}
		}
		index++;
	} while (index);
}

void test_454F0() {
	uint8_t* tempHeight1 = (uint8_t*)malloc(65536);
	uint8_t* tempHeight2 = (uint8_t*)malloc(65536);

	for (int y = 0; y < 256; y++)
	{
		if ((y > 3) && (y < 256 - 3))//skip steps for quick compare
			continue;
		for (int x = 0; x < 256; x++)
		{
			for (int i = 0; i < 256 * 256; i++)
			{
				mapHeightmap_11B4E0[i] = pseudoRand() % 256;
				tempHeight1[i] = mapHeightmap_11B4E0[i];
			}
			sub_454F0_orig(x, y);
			for (int i = 0; i < 256 * 256; i++)
			{
				tempHeight2[i] = mapHeightmap_11B4E0[i];
				mapHeightmap_11B4E0[i] = tempHeight1[i];
			}
			sub_454F0(x, y);
			for (int i = 0; i < 256 * 256; i++)
			{
				if (tempHeight2[i] != mapHeightmap_11B4E0[i])
					TestError();
			}
		}
	}
	free(tempHeight1);
	free(tempHeight2);
}

void test_45210() {
	uint8_t* tempAng1 = (uint8_t*)malloc(65536);
	uint8_t* tempHeight1 = (uint8_t*)malloc(65536);

	uint8_t* tempAng2 = (uint8_t*)malloc(65536);
	uint8_t* tempHeight2 = (uint8_t*)malloc(65536);

	for (int y = 0; y < 256; y++)
	{
		if ((y > 3) && (y < 256 - 3))//skip steps for quick compare
			continue;
		for (int x = 0; x < 256; x++)
		{
			for (int i = 0; i < 256 * 256; i++)
			{
				mapAngle_13B4E0[i] = pseudoRand() % 256;
				mapHeightmap_11B4E0[i] = pseudoRand() % 256;
				tempAng1[i] = mapAngle_13B4E0[i];
				tempHeight1[i] = mapHeightmap_11B4E0[i];
			}
			sub_45210_orig(x, y);
			for (int i = 0; i < 256 * 256; i++)
			{
				tempAng2[i] = mapAngle_13B4E0[i];
				mapAngle_13B4E0[i] = tempAng1[i];
				tempHeight2[i] = mapHeightmap_11B4E0[i];
				mapHeightmap_11B4E0[i] = tempHeight1[i];
			}
			sub_45210(x, y);
			for (int i = 0; i < 256 * 256; i++)
			{
				if ((tempAng2[i] != mapAngle_13B4E0[i]) ||
					(tempHeight2[i] != mapHeightmap_11B4E0[i]))
					TestError();
			}
		}
	}
	free(tempAng1);
	free(tempHeight1);
	free(tempAng2);
	free(tempHeight2);
}

void test_45060() {
	uint8_t* tempAng1 = (uint8_t*)malloc(65536);
	uint8_t* tempHeight1 = (uint8_t*)malloc(65536);

	uint8_t* tempAng2 = (uint8_t*)malloc(65536);
	uint8_t* tempHeight2 = (uint8_t*)malloc(65536);

	for (int y = 0; y < 256; y++)
	{
		if ((y > 3) && (y < 256 - 3))//skip steps for quick compare
			continue;
		for (int x = 0; x < 256; x++)
		{
			for (int i = 0; i < 256 * 256; i++)
			{
				mapAngle_13B4E0[i] = pseudoRand() % 256;
				mapHeightmap_11B4E0[i] = pseudoRand() % 256;
				tempAng1[i] = mapAngle_13B4E0[i];
				tempHeight1[i] = mapHeightmap_11B4E0[i];
			}
			sub_45060_orig(x, y);
			for (int i = 0; i < 256 * 256; i++)
			{
				tempAng2[i] = mapAngle_13B4E0[i];
				mapAngle_13B4E0[i] = tempAng1[i];
				tempHeight2[i] = mapHeightmap_11B4E0[i];
				mapHeightmap_11B4E0[i] = tempHeight1[i];
			}
			sub_45060(x, y);
			for (int i = 0; i < 256 * 256; i++)
			{
				if ((tempAng2[i] != mapAngle_13B4E0[i]) ||
					(tempHeight2[i] != mapHeightmap_11B4E0[i]))
					TestError();
			}
		}
	}
	free(tempAng1);
	free(tempHeight1);
	free(tempAng2);
	free(tempHeight2);
}

//----- (00044320) --------------------------------------------------------
void sub_44320_orig()//225320
{
	uint16_t index; // ax
	char v1; // bl
	char v2; // dh
	char v3; // dl

	index = 0;
	do
	{
		v1 = 0;
		v2 = 0;
		v3 = 0;
		if (!mapAngle_13B4E0[index])
			v1 = 1;
		if (mapAngle_13B4E0[index] == 5)
			v3 = 1;
		if (mapAngle_13B4E0[index] == 3)
			v2 = 1;
		LOBYTE(index)++;
		if (!mapAngle_13B4E0[index])
			v1++;
		if (mapAngle_13B4E0[index] == 5)
			v3++;
		if (mapAngle_13B4E0[index] == 3)
			v2++;
		HIBYTE(index)++;
		if (!mapAngle_13B4E0[index])
			v1++;
		if (mapAngle_13B4E0[index] == 5)
			v3++;
		if (mapAngle_13B4E0[index] == 3)
			v2++;
		LOBYTE(index)--;
		if (!mapAngle_13B4E0[index])
			v1++;
		if (mapAngle_13B4E0[index] == 5)
			v3++;
		if (mapAngle_13B4E0[index] == 3)
			v2++;
		HIBYTE(index)--;
		if (v2 && v3)
		{
			if (mapAngle_13B4E0[index] == 5)
				mapAngle_13B4E0[index] = 4;
			LOBYTE(index)++;
			if (mapAngle_13B4E0[index] == 5)
				mapAngle_13B4E0[index] = 4;
			HIBYTE(index)++;
			if (mapAngle_13B4E0[index] == 5)
				mapAngle_13B4E0[index] = 4;
			LOBYTE(index)--;
			if (mapAngle_13B4E0[index] == 5)
				mapAngle_13B4E0[index] = 4;
			HIBYTE(index)--;
		}
		if (v2 && v1)
		{
			if (mapAngle_13B4E0[index] == 3)
				mapAngle_13B4E0[index] = 4;
			LOBYTE(index)++;
			if (mapAngle_13B4E0[index] == 3)
				mapAngle_13B4E0[index] = 4;
			HIBYTE(index)++;
			if (mapAngle_13B4E0[index] == 3)
				mapAngle_13B4E0[index] = 4;
			LOBYTE(index)--;
			if (mapAngle_13B4E0[index] == 3)
				mapAngle_13B4E0[index] = 4;
			HIBYTE(index)--;
		}
		if (v1 && v3)
		{
			if (mapAngle_13B4E0[index])
				mapAngle_13B4E0[index] = 4;
			LOBYTE(index)++;
			if (mapAngle_13B4E0[index])
				mapAngle_13B4E0[index] = 4;
			HIBYTE(index)++;
			if (mapAngle_13B4E0[index])
				mapAngle_13B4E0[index] = 4;
			LOBYTE(index)--;
			if (mapAngle_13B4E0[index])
				mapAngle_13B4E0[index] = 4;
			HIBYTE(index)--;
		}
		index++;
	} while (index);
}

void sub_45600_orig(uint8_t a1)//226600
{
	uint16_t index; // ax
	unsigned __int8 v2; // dh
	unsigned __int8 v3; // dl
	//unsigned __int16 v4; // ax
	//unsigned __int16 v5; // ax
	//unsigned __int16 v6; // ax
	__int16 v7; // bx
	char v8; // dh
	char v9; // dl
	//char v10; // t0
	//char v11; // t0
	//char v12; // t0
	//char v13; // t0
	//char v14; // t0
	//char v15; // t0
	//char v16; // t0
	//char v17; // t0
	//char v18; // t0
	//char v19; // t0
	//char v20; // t0
	//char v21; // t0
	//char v22; // t0
	//char v23; // t0
	//char v24; // t0
	//char v25; // t0
	//char v26; // t0
	//char v27; // t0
	//char v28; // t0
	//char v29; // t0
	//char v30; // t0
	//char v31; // t0
	//char v32; // t0
	//char v33; // t0

	// fix if begin
	//v4 = 0;
	//v5 = 0;
	//v6 = 0;
	// end

	qmemcpy(mapTerrainType_10B4E0, mapAngle_13B4E0, 0x10000);
	index = 0;
	do
	{
		v2 = 0;
		v3 = 0xffu;
		if (mapHeightmap_11B4E0[index])
			v2 = mapHeightmap_11B4E0[index];
		if (mapHeightmap_11B4E0[index] < 0xFFu)
			v3 = mapHeightmap_11B4E0[index];
		HIBYTE(index)--;
		if (v2 < mapHeightmap_11B4E0[index])
			v2 = mapHeightmap_11B4E0[index];
		if (v3 > mapHeightmap_11B4E0[index])
			v3 = mapHeightmap_11B4E0[index];
		LOBYTE(index)++;
		HIBYTE(index)++;
		if (v2 < mapHeightmap_11B4E0[index])
			v2 = mapHeightmap_11B4E0[index];
		if (v3 > mapHeightmap_11B4E0[index])
			v3 = mapHeightmap_11B4E0[index];
		HIBYTE(index)++;
		LOBYTE(index)--;
		if (v2 < mapHeightmap_11B4E0[index])
			v2 = mapHeightmap_11B4E0[index];
		if (v3 > mapHeightmap_11B4E0[index])
			v3 = mapHeightmap_11B4E0[index];
		LOBYTE(index)--;
		HIBYTE(index)--;
		if (v2 < mapHeightmap_11B4E0[index])
			v2 = mapHeightmap_11B4E0[index];
		if (v3 > mapHeightmap_11B4E0[index])
			v3 = mapHeightmap_11B4E0[index];
		LOBYTE(index)++;
		if (mapAngle_13B4E0[index] && v2 - v3 >= a1)
			mapAngle_13B4E0[index] = 1;
		index++;
	} while (index);
	do
	{
		if (mapAngle_13B4E0[index] != 6)
			goto LABEL_100;
		v7 = 0;
		HIBYTE(index)--;
		v8 = 0;
		v9 = 0;
		if (mapAngle_13B4E0[index] == 3)
			HIBYTE(v7) = 1;
		if (mapAngle_13B4E0[index] == 2)
			v9 = 1;
		if (mapAngle_13B4E0[index] == 5)
			LOBYTE(v7) = 1;
		//v10 = x_BYTE_13B4E0_angle[index];
		//v11 = x_BYTE_13B4E0_angle[index];
		if (mapAngle_13B4E0[index] == 4)
			v8 = 1;
		//v12 = x_BYTE_13B4E0_angle[index];
		LOBYTE(index)++;
		if (mapAngle_13B4E0[index] == 3)
			HIBYTE(v7)++;
		if (mapAngle_13B4E0[index] == 2)
			v9++;
		if (mapAngle_13B4E0[index] == 5)
			LOBYTE(v7)++;
		//v13 = x_BYTE_13B4E0_angle[index];
		//v14 = x_BYTE_13B4E0_angle[index];
		if (mapAngle_13B4E0[index] == 4)
			v8++;
		//v15 = x_BYTE_13B4E0_angle[index];
		HIBYTE(index)++;
		if (mapAngle_13B4E0[index] == 3)
			HIBYTE(v7)++;
		if (mapAngle_13B4E0[index] == 2)
			v9++;
		if (mapAngle_13B4E0[index] == 5)
			LOBYTE(v7)++;
		//v16 = x_BYTE_13B4E0_angle[index];
		//v17 = x_BYTE_13B4E0_angle[index];
		if (mapAngle_13B4E0[index] == 4)
			v8++;
		//v18 = x_BYTE_13B4E0_angle[index];
		HIBYTE(index)++;
		if (mapAngle_13B4E0[index] == 3)
			HIBYTE(v7)++;
		if (mapAngle_13B4E0[index] == 2)
			v9++;
		if (mapAngle_13B4E0[index] == 5)
			LOBYTE(v7)++;
		//v19 = x_BYTE_13B4E0_angle[index];
		//v20 = x_BYTE_13B4E0_angle[index];
		if (mapAngle_13B4E0[index] == 4)
			v8++;
		//v21 = x_BYTE_13B4E0_angle[index];
		LOBYTE(index) = index - 1;
		if (mapAngle_13B4E0[index] == 3)
			HIBYTE(v7)++;
		if (mapAngle_13B4E0[index] == 2)
			v9++;
		if (mapAngle_13B4E0[index] == 5)
			LOBYTE(v7)++;
		//v22 = x_BYTE_13B4E0_angle[index];
		//v23 = x_BYTE_13B4E0_angle[index];
		if (mapAngle_13B4E0[index] == 4)
			v8++;
		//v24 = x_BYTE_13B4E0_angle[index];
		LOBYTE(index)--;
		if (mapAngle_13B4E0[index] == 3)
			HIBYTE(v7)++;
		if (mapAngle_13B4E0[index] == 2)
			v9++;
		if (mapAngle_13B4E0[index] == 5)
			LOBYTE(v7)++;
		//v25 = x_BYTE_13B4E0_angle[index];
		//v26 = x_BYTE_13B4E0_angle[index];
		if (mapAngle_13B4E0[index] == 4)
			v8++;
		//v27 = x_BYTE_13B4E0_angle[index];
		HIBYTE(index)--;
		if (mapAngle_13B4E0[index] == 3)
			HIBYTE(v7)++;
		if (mapAngle_13B4E0[index] == 2)
			v9++;
		if (mapAngle_13B4E0[index] == 5)
			LOBYTE(v7)++;
		//v28 = x_BYTE_13B4E0_angle[index];
		//v29 = x_BYTE_13B4E0_angle[index];
		if (mapAngle_13B4E0[index] == 4)
			v8++;
		//v30 = x_BYTE_13B4E0_angle[index];
		HIBYTE(index)--;
		if (mapAngle_13B4E0[index] == 3)
			HIBYTE(v7)++;
		if (mapAngle_13B4E0[index] == 2)
			v9++;
		if (mapAngle_13B4E0[index] == 5)
			LOBYTE(v7)++;
		//v31 = x_BYTE_13B4E0_angle[index];
		//v32 = x_BYTE_13B4E0_angle[index];
		if (mapAngle_13B4E0[index] == 4)
			v8++;
		//v33 = x_BYTE_13B4E0_angle[index];
		LOBYTE(index)++;
		HIBYTE(index)++;
		if (HIBYTE(v7))
		{
			if (v9 || LOBYTE(v7) || v8)
				goto LABEL_99;
		}
		else if (v9 || (LOBYTE(v7) && v8))
		{
		LABEL_99:
			mapAngle_13B4E0[index] = 1;
			goto LABEL_100;
		}
	LABEL_100:
		index++;
	} while (index);
}

//----- (00043FC0) --------------------------------------------------------
void sub_43FC0_orig()//224fc0
{
	uint16_t index; // ax
	uint16_t v1; // dx
	char v2; // dh
	unsigned __int16 v3; // bx
	unsigned __int16 v4; // bx
	unsigned __int16 v5; // bx
	unsigned __int16 v6; // bx
	unsigned __int16 v7; // bx
	char v8; // bl
	//unsigned __int16 v9; // ax

	index = 0;
	do
	{
		HIBYTE(index)--;
		v1 = index;
		LOBYTE(index)++;
		v2 = mapAngle_13B4E0[v1];
		LOBYTE(v1) = v2 == mapAngle_13B4E0[index];
		HIBYTE(index)++;
		v3 = index;
		HIBYTE(index)++;
		LOBYTE(v1) = (v2 == mapAngle_13B4E0[v3]) + v1;
		v4 = index;
		LOBYTE(index)--;
		LOBYTE(v1) = (v2 == mapAngle_13B4E0[v4]) + v1;
		v5 = index;
		LOBYTE(index)--;
		LOBYTE(v1) = (v2 == mapAngle_13B4E0[v5]) + v1;
		v6 = index;
		HIBYTE(index)--;
		LOBYTE(v1) = (v2 == mapAngle_13B4E0[v6]) + v1;
		v7 = index;
		HIBYTE(index)--;
		v8 = (v2 == mapAngle_13B4E0[index]) + (v2 == mapAngle_13B4E0[v7]) + v1;
		LOBYTE(index)++;
		HIBYTE(index)++;
		if (v2)
		{
			if (v8 == 7)
				mapAngle_13B4E0[index] = v2;
		}
		index++;
	} while (index);
}

void sub_43EE0_orig()//224ee0
{
	unsigned __int16 index; // ax
	unsigned __int16 v1; // cx
	char v2; // dh
	unsigned __int8 v3; // dl
	int v4; // ecx
	char v5; // bl
	char v6; // bh
	unsigned __int8 v7; // cl
	char v8; // bh
	char v9; // bh
	unsigned __int16 v10; // cx
	unsigned __int16 v11; // cx
	unsigned __int16 v12; // cx
	unsigned __int16 v13; // cx

	index = 0;
	do
	{
		v1 = index;
		v2 = 0;
		LOBYTE(index)++;
		v3 = mapHeightmap_11B4E0[v1];
		v4 = v1;
		LOWORD(v4) = index;
		v5 = 0;
		v6 = mapAngle_13B4E0[index];
		if (v6)
		{
			if (v6 == 4)
				v2 = 1;
		}
		else
		{
			v7 = mapHeightmap_11B4E0[v4];
			v5 = 1;
			if (v7 < v3)
				v3 = v7;
		}
		HIBYTE(index)++;
		v8 = mapAngle_13B4E0[index];
		if (v8)
		{
			if (v8 == 4)
				v2++;
		}
		else
		{
			v5++;
			if (mapHeightmap_11B4E0[index] < v3)
				v3 = mapHeightmap_11B4E0[index];
		}
		LOBYTE(index)--;
		v9 = mapAngle_13B4E0[index];
		if (v9)
		{
			if (v9 == 4)
				v2++;
		}
		else
		{
			v5++;
			if (mapHeightmap_11B4E0[index] < v3)
				v3 = mapHeightmap_11B4E0[index];
		}
		HIBYTE(index)--;
		if (v2 && v5 && !v3)
		{
			v10 = index;
			LOBYTE(index)++;
			mapHeightmap_11B4E0[v10] = 0;
			v11 = index;
			HIBYTE(index)++;
			mapHeightmap_11B4E0[v11] = 0;
			v12 = index;
			LOBYTE(index)--;
			mapHeightmap_11B4E0[v12] = 0;
			v13 = index;
			HIBYTE(index)--;
			mapHeightmap_11B4E0[v13] = 0;
		}
		index++;
	} while (index);
}

void sub_44580_orig()//225580
{
	uint8_t* v0; // eax
	unsigned __int16 v1; // bx
	unsigned __int16 v2; // bx
	char v3; // al
	char v4; // dl
	char v5; // cl
	char v6; // ah
	int v7; // edi
	uint8_t* v8; // esi
	uint8_t v9; // dh
	uint8_t* v10; // esi
	char v11; // dh
	char v12; // al
	char v13; // dl
	char v14; // dh
	int v15; // ST00_4
	char v16; // ah
	uint8_t* v17; // esi
	char v18; // cl
	unsigned __int8 v19; // dh
	uint8_t* v20; // esi
	char v21; // dh
	char v22; // al
	char v23; // ah
	char v24; // dh
	char v25; // dl
	int v26; // edi
	uint8_t* v27; // esi
	char v28; // cl
	unsigned __int8 v29; // dh
	uint8_t* v30; // esi
	char v31; // dh
	char v32; // al
	char v33; // dl
	char v34; // dh
	int v35; // ST00_4
	char v36; // ah
	uint8_t* v37; // esi
	char v38; // cl
	unsigned __int8 v39; // dh
	uint8_t* v40; // esi
	char v41; // dh
	int v42; // edi
	int v43; // esi
	char v44; // al
	uint8_t* v45; // esi
	char v46; // cl
	unsigned __int8 v47; // dh
	uint8_t* v48; // esi
	char v49; // dh
	char v50; // al
	char v51; // dl
	char v52; // dh
	int v53; // edi
	int v54; // esi
	char v55; // ah
	uint8_t* v56; // esi
	char v57; // cl
	unsigned __int8 v58; // dh
	uint8_t* v59; // esi
	char v60; // dh
	char v61; // al
	char v62; // ah
	char v63; // dh
	char v64; // dl
	int v65; // edi
	uint8_t* v66; // esiv
	char v67; // cl
	unsigned __int8 v68; // dh
	uint8_t* v69; // esi
	uint8_t* v70; // eax
	uint8_t v71; // edx
	//char *v72; // esi
	int v72x;
	char i; // bl
	char j; // ch
	char k; // cl
	char l; // bh
	uint8_t* v77; // eax
	uint16_t v78; // cx
	//uint16_t v79; // eax
	char v80; // ST14_1
	//unsigned __int16 v81; // ax
	char v82; // bh
	unsigned __int16 v83; // ax
	int v84; // esi
	uint8_t v85; // al
	uint8_t v86; // bl
	uint8_t* v87; // edx
	int8_t* v89; // [esp+8h] [ebp-10h]
	uint8_t* v90; // [esp+Ch] [ebp-Ch]

	v0 = pdwScreenBuffer_351628;
	v1 = 0;
	while (v1 < 0x961u)
	{
		v1++;
		v0[0] = 0;
		v0 += 25;
	}
	v2 = 0;
	v90 = pdwScreenBuffer_351628;
	v89 = unk_D47E0;
	while (v2 < 0x94u)
	{
		v3 = v89[0];
		if (v89[0] >= 0)
		{
			v4 = v89[1];
			if (v4 >= 0)
			{
				v5 = v89[2];
				if (v5 >= 0)
				{
					v6 = v89[3];
					if (v6 >= 0)
					{
						v7 = 49 * v4 + 7 * v5 + v6 + 343 * v3;
						v8 = v90 + 25 * v7;
						v9 = v8[0];
						if (v8[0] < 0xCu)
						{
							v8[0] = v9 + 1;
							v10 = &v8[v9];
							v10[13] = 0;
							v10[1] = v2;
						}
						v11 = v3;
						v12 = v4;
						v13 = v11;
						v14 = v6;
						v15 = 7 * v6;
						v16 = v5;
						v17 = v90 + 25 * (49 * v13 + v5 + v15 + 343 * v12);
						v18 = v14;
						v19 = v17[0];
						if (v17[0] < 0xCu)
						{
							v17[0] = v19 + 1;
							v20 = &v17[v19];
							v20[13] = 16;
							v20[1] = v2;
						}
						v21 = v12;
						v22 = v16;
						v23 = v21;
						v24 = v13;
						v25 = v18;
						v26 = 49 * v18 + v23 + 7 * v24 + 343 * v22;
						v27 = v90 + 25 * v26;
						v28 = v24;
						v29 = v27[0];
						if (v27[0] < 0xCu)
						{
							v27[0] = v29 + 1;
							v30 = &v27[v29];
							v30[13] = 48;
							v30[1] = v2;
						}
						v31 = v22;
						v32 = v25;
						v33 = v31;
						v34 = v23;
						v35 = 7 * v23;
						v36 = v28;
						v37 = v90 + 25 * (49 * v33 + v28 + v35 + 343 * v32);
						v38 = v34;
						v39 = *v37;
						if (*v37 < 0xCu)
						{
							*v37 = v39 + 1;
							v40 = &v37[v39];
							v40[13] = 32;
							v40[1] = v2;
						}
						v41 = v32;
						v42 = v32;
						v43 = 8 * v32;
						v44 = v38;
						v45 = v90 + 25 * (49 * v33 + v43 - v42 + v36 + 343 * v38);
						v46 = v41;
						v47 = v45[0];
						if (v45[0] < 0xCu)
						{
							v45[0] = v47 + 1;
							v48 = &v45[v47];
							v48[13] = 96;
							v48[1] = v2;
						}
						v49 = v44;
						v50 = v33;
						v51 = v49;
						v52 = v36;
						v53 = v36;
						v54 = 8 * v36;
						v55 = v46;
						v56 = v90 + 25 * (49 * v51 + v54 - v53 + v46 + 343 * v50);
						v57 = v52;
						v58 = v56[0];
						if (v56[0] < 0xCu)
						{
							v56[0] = v58 + 1;
							v59 = &v56[v58];
							v59[13] = 112;
							v59[1] = v2;
						}
						v60 = v50;
						v61 = v55;
						v62 = v60;
						v63 = v51;
						v64 = v57;
						v65 = 49 * v57 + 7 * v63 + v62 + 343 * v61;
						v66 = v90 + 25 * v65;
						v67 = v63;
						v68 = v66[0];
						if (v66[0] < 0xCu)
						{
							v66[0] = v68 + 1;
							v69 = &v66[v68];
							v69[13] = 80;
							v69[1] = v2;
						}
						v70 = &v90[25 * (343 * v64 + 7 * v62 + v67 + 49 * v61)];
						v71 = v70[0];
						if (v70[0] < 0xCu)
						{
							v70[0] = v71 + 1;
							//v71 = v71&0xff;
							v70[v71 + 13] = 64;
							v70[v71 + 1] = v2;
						}
					}
				}
			}
		}
		v2++;
		v89 += 4;
	}
	//v72 = x_BYTE_F2CD0x;//adress 225afd
	v72x = 0;
	for (i = 0; i < 7; i++)
	{
		for (j = 0; j < 7; j++)
		{
			for (k = 0; k < 7; k++)
			{
				for (l = 0; l < 7; l++)
				{
					v77 = &v90[25 * (49 * j + 7 * k + l + 343 * i)];
					if (v77[0])
					{
						x_BYTE_F2CD0x[v72x][0] = v77[1];
						x_BYTE_F2CD0x[v72x][1] = v77[13];
					}
					else
					{
						x_BYTE_F2CD0x[v72x][0] = 1;
						x_BYTE_F2CD0x[v72x][1] = 0;
					}
					//v72 += 2;
					v72x++;
				}
			}
		}
	}
	v78 = 0;//adress 225bbd
	do
	{
		//v79 = v78;
		if (!mapTerrainType_10B4E0[v78])
		{
			v80 = mapAngle_13B4E0[v78] & 7;
			LOBYTE(v78)++;
			//v81 = v78;
			v82 = mapAngle_13B4E0[v78];
			HIBYTE(v78)++;
			v83 = v78;
			LOBYTE(v78)--;
			v84 = 343 * v80 + 49 * (v82 & 7) + (mapAngle_13B4E0[v78] & 7) + 7 * (mapAngle_13B4E0[v83] & 7);
			v85 = v90[25 * v84];
			HIBYTE(v78)--;
			if (v85)
			{
				x_WORD_17B4E0 = 9377 * x_WORD_17B4E0 + 9439;
				v86 = x_WORD_17B4E0 % (v85 + 1);
				if (v86 >= v85)
					v86 = 0;
				v87 = (v86 + 25 * v84 + v90);
				//LOWORD(v79) = v78;
				mapTerrainType_10B4E0[v78] = v87[1];//problem is here!
				mapAngle_13B4E0[v78] = (mapAngle_13B4E0[v78] & 7) + v87[13];
			}
			else
			{
				//LOWORD(v79) = v78;
				mapTerrainType_10B4E0[v78] = 1;
			}
		}
		v78++;
	} while (v78);
}

//----- (00043BB0) --------------------------------------------------------
void sub_43BB0_orig()//224bb0
{
	unsigned int v0; // esi
	unsigned __int16 v1; // cx
	signed int v2; // ebx
	unsigned __int16 v3; // bx
	//int v4; // eax
	unsigned __int8 v5; // dh
	char v6; // cl

	v0 = 37487429;
	v1 = 0;
	do
	{
		if (!(mapAngle_13B4E0[v1] & 8))
		{
			v0 = 9377 * v0 + 9439;
			v2 = v0 % 7 - 3 + (unsigned __int8)x_BYTE_14B4E0_second_heightmap[v1];
			if (v2 < 0)
				v2 = 0;
			if (v2 > 254)
				LOBYTE(v2) = -2;
			x_BYTE_14B4E0_second_heightmap[v1] = v2;
		}
		v1++;
	} while (v1);
	v3 = 0;
	do
	{
		//v4 = v3;
		v5 = mapHeightmap_11B4E0[v3];
		if ((unsigned __int8)x_BYTE_14B4E0_second_heightmap[v3] > v5)
		{
			mapAngle_13B4E0[v3] &= 0xF7u;
		}
		else
		{
			v6 = mapAngle_13B4E0[v3] | 8;
			x_BYTE_14B4E0_second_heightmap[v3] = v5 - 1;
			mapAngle_13B4E0[v3] = v6;
		}
		v3++;
	} while (v3);
}

//----- (00043B40) --------------------------------------------------------
void sub_43B40_orig()//224b40
{
	uint16_t v0; // dx
	uint8_t v1; // eax
	uint8_t v2; // bl
	uint8_t v3; // bh
	uint8_t v4; // cl

	v0 = 0;
	do
	{
		v1 = mapHeightmap_11B4E0[v0];
		if (v1 > x_BYTE_D41B7)
			v1 = x_BYTE_D41B7;
		v2 = x_BYTE_D41B7 - v1;
		x_BYTE_14B4E0_second_heightmap[v0] = x_BYTE_D41B7 - v1;
		v3 = mapHeightmap_11B4E0[v0];
		if (v2 > v3)
		{
			mapAngle_13B4E0[v0] &= 0xF7u;
		}
		else
		{
			v4 = mapAngle_13B4E0[v0] | 8;
			x_BYTE_14B4E0_second_heightmap[v0] = v3 - 1;
			mapAngle_13B4E0[v0] = v4;
		}
		v0++;
	} while (v0);
	sub_43BB0_orig();
}
// D41B7: using guessed type char x_BYTE_D41B7;

void test_43B40_43BB0() {

	uint8_t* tempMapHeightmap1 = (uint8_t*)malloc(65536);
	uint8_t* tempSecondHeightmap1 = (uint8_t*)malloc(65536);
	uint8_t* tempAng1 = (uint8_t*)malloc(65536);

	uint8_t* tempMapHeightmap2 = (uint8_t*)malloc(65536);
	uint8_t* tempSecondHeightmap2 = (uint8_t*)malloc(65536);
	uint8_t* tempAng2 = (uint8_t*)malloc(65536);

	x_BYTE_14B4E0_second_heightmap = (uint8_t*)malloc(65536);

	uint16_t temp_x_BYTE_D41B7_1;
	uint16_t temp_x_BYTE_D41B7_2;

	for (int i = 0; i < 256 * 256; i++)
	{
		mapAngle_13B4E0[i] = pseudoRand() % 256;
		mapHeightmap_11B4E0[i] = pseudoRand() % 256;
		x_BYTE_14B4E0_second_heightmap[i] = pseudoRand() % 256;
		tempAng1[i] = mapAngle_13B4E0[i];
		tempMapHeightmap1[i] = mapHeightmap_11B4E0[i];
		tempSecondHeightmap1[i] = x_BYTE_14B4E0_second_heightmap[i];
	}
	x_BYTE_D41B7 = pseudoRand() % 256;
	temp_x_BYTE_D41B7_1 = x_BYTE_D41B7;
	sub_43B40_orig();
	for (int i = 0; i < 256 * 256; i++)
	{
		tempAng2[i] = mapAngle_13B4E0[i];
		mapAngle_13B4E0[i] = tempAng1[i];
		tempSecondHeightmap2[i] = x_BYTE_14B4E0_second_heightmap[i];
		x_BYTE_14B4E0_second_heightmap[i] = tempSecondHeightmap1[i];
		tempMapHeightmap2[i] = mapHeightmap_11B4E0[i];
		mapHeightmap_11B4E0[i] = tempMapHeightmap1[i];
	}
	temp_x_BYTE_D41B7_2 = x_BYTE_D41B7;
	x_BYTE_D41B7 = temp_x_BYTE_D41B7_1;
	sub_43B40();
	if (temp_x_BYTE_D41B7_2 != x_BYTE_D41B7)
		TestError();
	for (int i = 0; i < 256 * 256; i++)
	{
		if ((tempAng2[i] != mapAngle_13B4E0[i]) ||
			(tempMapHeightmap2[i] != mapHeightmap_11B4E0[i]) ||
			(tempSecondHeightmap2[i] != x_BYTE_14B4E0_second_heightmap[i]))
				TestError();
	}
	free(tempMapHeightmap1);
	free(tempSecondHeightmap1);
	free(tempAng1);

	free(tempMapHeightmap2);
	free(tempSecondHeightmap2);
	free(tempAng2);

	free(x_BYTE_14B4E0_second_heightmap);
}

void test_44580() {

	pdwScreenBuffer_351628 = (uint8_t*)malloc(0x961u * 25);

	uint8_t* tempScreenBuffer1 = (uint8_t*)malloc(0x961u * 25);
	uint8_t* tempAng1 = (uint8_t*)malloc(65536);
	uint8_t* tempTerrainType1 = (uint8_t*)malloc(65536);

	uint8_t* tempScreenBuffer2 = (uint8_t*)malloc(0x961u * 25);
	uint8_t* tempAng2 = (uint8_t*)malloc(65536);
	uint8_t* tempTerrainType2 = (uint8_t*)malloc(65536);

	uint16_t temp_x_WORD_17B4E0_1;
	uint16_t temp_x_WORD_17B4E0_2;

	char tempx_BYTE_F2CD0x1[7 * 7 * 7 * 7][2];
	char tempx_BYTE_F2CD0x2[7 * 7 * 7 * 7][2];

	for (int i = 0; i < 256 * 256; i++)
	{
		mapAngle_13B4E0[i] = pseudoRand() % 256;
		mapTerrainType_10B4E0[i] = pseudoRand() % 256;
		tempAng1[i] = mapAngle_13B4E0[i];
		tempTerrainType1[i] = mapTerrainType_10B4E0[i];
	}
	for (int i = 0; i < 7*7*7*7; i++)
		for (int m = 0; m < 2; m++)
		{
			x_BYTE_F2CD0x[i][m] = pseudoRand() % 256;
			tempx_BYTE_F2CD0x1[i][m] = x_BYTE_F2CD0x[i][m];
		}
	for (int i = 0; i < 0x961u * 25; i++)
	{
		pdwScreenBuffer_351628[i] = pseudoRand() % 256;
		tempScreenBuffer1[i] = pdwScreenBuffer_351628[i];
	}
	x_WORD_17B4E0 = pseudoRand() % (256 * 256);
	temp_x_WORD_17B4E0_1 = x_WORD_17B4E0;
	sub_44580_orig();
	for (int i = 0; i < 256 * 256; i++)
	{
		tempAng2[i] = mapAngle_13B4E0[i];
		mapAngle_13B4E0[i] = tempAng1[i];
		tempTerrainType2[i] = mapTerrainType_10B4E0[i];
		mapTerrainType_10B4E0[i] = tempTerrainType1[i];
	}
	for (int i = 0; i < 0x961u * 25; i++)
	{
		tempScreenBuffer2[i] = pdwScreenBuffer_351628[i];
		pdwScreenBuffer_351628[i] = tempScreenBuffer1[i];
	}
	for (int i = 0; i < 7*7*7*7; i++)
		for (int m = 0; m < 2; m++)
		{
			tempx_BYTE_F2CD0x2[i][m] = x_BYTE_F2CD0x[i][m];
			x_BYTE_F2CD0x[i][m] = tempx_BYTE_F2CD0x1[i][m];
		}
	temp_x_WORD_17B4E0_2 = x_WORD_17B4E0;
	x_WORD_17B4E0 = temp_x_WORD_17B4E0_1;
	sub_44580();
	for (int i = 0; i < 7*7*7*7; i++)
		for (int m = 0; m < 2; m++)
		{
			if (tempx_BYTE_F2CD0x2[i][m] != x_BYTE_F2CD0x[i][m])
				TestError();
		}
	if (temp_x_WORD_17B4E0_2 != x_WORD_17B4E0)
		TestError();
	for (int i = 0; i < 0x961u * 25; i++)
	{
		if (tempScreenBuffer2[i] != pdwScreenBuffer_351628[i])
			TestError();
	}
	for (int i = 0; i < 256 * 256; i++)
	{
		if ((tempAng2[i] != mapAngle_13B4E0[i]) ||
			(tempTerrainType2[i] != mapTerrainType_10B4E0[i]))
			TestError();
	}
	free(tempAng1);
	free(tempTerrainType1);
	free(tempAng2);
	free(tempTerrainType2);

	free(tempScreenBuffer1);
	free(tempScreenBuffer2);

	free(pdwScreenBuffer_351628);
}

void test_43EE0() {
	uint8_t* tempAng1 = (uint8_t*)malloc(65536);
	uint8_t* tempHeight1 = (uint8_t*)malloc(65536);

	uint8_t* tempAng2 = (uint8_t*)malloc(65536);
	uint8_t* tempHeight2 = (uint8_t*)malloc(65536);

	for (int i = 0; i < 256 * 256; i++)
	{
		mapAngle_13B4E0[i] = pseudoRand() % 256;
		mapHeightmap_11B4E0[i] = pseudoRand() % 256;
		tempAng1[i] = mapAngle_13B4E0[i];
		tempHeight1[i] = mapHeightmap_11B4E0[i];
	}
	sub_43EE0_orig();
	for (int i = 0; i < 256 * 256; i++)
	{
		tempAng2[i] = mapAngle_13B4E0[i];
		mapAngle_13B4E0[i] = tempAng1[i];
		tempHeight2[i] = mapHeightmap_11B4E0[i];
		mapHeightmap_11B4E0[i] = tempHeight1[i];
	}
	sub_43EE0();
	for (int i = 0; i < 256 * 256; i++)
	{
		if ((tempAng2[i] != mapAngle_13B4E0[i]) ||
			(tempHeight2[i] != mapHeightmap_11B4E0[i]))
			TestError();
	}
	free(tempAng1);
	free(tempHeight1);
	free(tempAng2);
	free(tempHeight2);
}

void test_43FC0() {
	uint8_t* tempAng1 = (uint8_t*)malloc(65536);
	uint8_t* tempAng2 = (uint8_t*)malloc(65536);

	for (int i = 0; i < 256 * 256; i++)
	{
		mapAngle_13B4E0[i] = pseudoRand() % 256;
		tempAng1[i] = mapAngle_13B4E0[i];
	}
	sub_43FC0_orig();
	for (int i = 0; i < 256 * 256; i++)
	{
		tempAng2[i] = mapAngle_13B4E0[i];
		mapAngle_13B4E0[i] = tempAng1[i];
	}
	sub_43FC0();
	for (int i = 0; i < 256 * 256; i++)
	{
		if (tempAng2[i] != mapAngle_13B4E0[i])
			TestError();
	}
	free(tempAng1);
	free(tempAng2);
}

void test_45600() {
	uint8_t* tempAng1 = (uint8_t*)malloc(65536);
	uint8_t* tempHeight1 = (uint8_t*)malloc(65536);

	uint8_t* tempAng2 = (uint8_t*)malloc(65536);
	uint8_t* tempHeight2 = (uint8_t*)malloc(65536);

	for (int j = 0; j < 256; j++)
	{
		for (int i = 0; i < 256 * 256; i++)
		{
			mapAngle_13B4E0[i] = pseudoRand() % 256;
			mapHeightmap_11B4E0[i] = pseudoRand() % 256;
			tempAng1[i] = mapAngle_13B4E0[i];
			tempHeight1[i] = mapHeightmap_11B4E0[i];
		}
		sub_45600_orig(j);
		for (int i = 0; i < 256 * 256; i++)
		{
			tempAng2[i] = mapAngle_13B4E0[i];
			mapAngle_13B4E0[i] = tempAng1[i];
			tempHeight2[i] = mapHeightmap_11B4E0[i];
			mapHeightmap_11B4E0[i] = tempHeight1[i];
		}
		sub_45600(j);
		for (int i = 0; i < 256 * 256; i++)
		{
			if ((tempAng2[i] != mapAngle_13B4E0[i]) ||
				(tempHeight2[i] != mapHeightmap_11B4E0[i]))
				TestError();
		}
	}
	free(tempAng1);
	free(tempHeight1);
	free(tempAng2);
	free(tempHeight2);
}

void test_44320() {
	uint8_t* tempAng1 = (uint8_t*)malloc(65536);
	uint8_t* tempAng2 = (uint8_t*)malloc(65536);

	for (int i = 0; i < 256 * 256; i++)
	{
		mapAngle_13B4E0[i] = pseudoRand() % 256;
		tempAng1[i] = mapAngle_13B4E0[i];
	}
	sub_44320_orig();
	for (int i = 0; i < 256 * 256; i++)
	{
		tempAng2[i] = mapAngle_13B4E0[i];
		mapAngle_13B4E0[i] = tempAng1[i];
	}
	sub_44320();
	for (int i = 0; i < 256 * 256; i++)
	{
		if (tempAng2[i] != mapAngle_13B4E0[i])
			TestError();
	}
	free(tempAng1);
	free(tempAng2);
}

void test_440D0() {
	uint8_t* tempAng1 = (uint8_t*)malloc(65536);
	uint8_t* tempHeight1 = (uint8_t*)malloc(65536);

	uint8_t* tempAng2 = (uint8_t*)malloc(65536);
	uint8_t* tempHeight2 = (uint8_t*)malloc(65536);

	for (int j = 0; j < 256 * 256; j++)
	{
		if ((j > 260) && (j < 256 * 256 - 260))//skip steps for quick compare
			continue;
		for (int i = 0; i < 256 * 256; i++)
		{
			mapAngle_13B4E0[i] = pseudoRand() % 256;
			mapHeightmap_11B4E0[i] = pseudoRand() % 256;
			tempAng1[i] = mapAngle_13B4E0[i];
			tempHeight1[i] = mapHeightmap_11B4E0[i];
		}
		sub_440D0_orig(j);
		for (int i = 0; i < 256 * 256; i++)
		{
			tempAng2[i] = mapAngle_13B4E0[i];
			mapAngle_13B4E0[i] = tempAng1[i];
			tempHeight2[i] = mapHeightmap_11B4E0[i];
			mapHeightmap_11B4E0[i] = tempHeight1[i];
		}
		sub_440D0(j);
		for (int i = 0; i < 256 * 256; i++)
		{
			if ((tempAng2[i] != mapAngle_13B4E0[i]) ||
				(tempHeight2[i] != mapHeightmap_11B4E0[i]))
				TestError();
		}
	}
	free(tempAng1);
	free(tempHeight1);
	free(tempAng2);
	free(tempHeight2);
}

void test_45AA0() {
	uint8_t* tempAng1 = (uint8_t*)malloc(65536);
	uint8_t* tempHeight1 = (uint8_t*)malloc(65536);

	uint8_t* tempAng2 = (uint8_t*)malloc(65536);
	uint8_t* tempHeight2 = (uint8_t*)malloc(65536);

	for (int i = 0; i < 256 * 256; i++)
	{
		mapAngle_13B4E0[i] = pseudoRand() % 256;
		mapHeightmap_11B4E0[i] = pseudoRand() % 256;
		tempAng1[i] = mapAngle_13B4E0[i];
		tempHeight1[i] = mapHeightmap_11B4E0[i];
	}
	sub_45AA0_setMax4Tiles_orig();
	for (int i = 0; i < 256 * 256; i++)
	{
		tempAng2[i] = mapAngle_13B4E0[i];
		mapAngle_13B4E0[i] = tempAng1[i];
		tempHeight2[i] = mapHeightmap_11B4E0[i];
		mapHeightmap_11B4E0[i] = tempHeight1[i];
	}
	sub_45AA0_setMax4Tiles();
	for (int i = 0; i < 256 * 256; i++)
	{
		if ((tempAng2[i] != mapAngle_13B4E0[i]) ||
			(tempHeight2[i] != mapHeightmap_11B4E0[i]))
			TestError();
	}
	free(tempAng1);
	free(tempHeight1);
	free(tempAng2);
	free(tempHeight2);
}

void Terrain_test() {

	printf("test_43B40_43BB0 - ");
	test_43B40_43BB0();
	printf("OK\n");

	printf("test_44580 - ");
	test_44580();
	printf("OK\n");

	printf("test_43EE0 - ");
	test_43EE0();
	printf("OK\n");

	printf("test_43FC0 - ");
	test_43FC0();
	printf("OK\n");

	printf("test_45600 - ");
	test_45600();
	printf("OK\n");

	printf("test_454F0 - ");
	test_454F0();
	printf("OK\n");

	printf("test_45210 - ");
	test_45210();
	printf("OK\n");

	printf("test_44320 - ");
	test_44320();
	printf("OK\n");

	printf("test_45060 - ");
	test_45060();
	printf("OK\n");

	printf("test_440D0 - ");
	test_440D0();
	printf("OK\n");

	printf("test_45AA0 - ");
	test_45AA0();
	printf("OK\n");
}

