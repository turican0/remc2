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

// D41B7: using guessed type char x_BYTE_D41B7;

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

