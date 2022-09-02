#include "Terrain-tests.h"

unsigned int pseudoRand()
{
	static unsigned int nSeed = 5323;
	nSeed = (8253729 * nSeed + 2396403);
	return nSeed % 32767;
}

int sub_B5C60_getTerrainAlt2_orig(uint16_t a1, uint16_t a2)//296c60
{
	uint16_t v2; // ebx
	int v3; // edx
	int v4; // eax
	int v5; // esi
	int v6; // esi
	int result; // eax
	int v8; // esi
	int v9; // eax
	int v10; // esi
	int v11; // esi
	int v12; // edi

	v2 = HIBYTE(a1);
	HIBYTE(v2) = HIBYTE(a2);
	v3 = LOBYTE(a2);
	if ((HIBYTE(a1) + HIBYTE(a2)) & 1)
	{
		//if (__CFADD__((x_BYTE)v3, (x_BYTE)a1))
		if (uint8(v3) > uint8(v3 + a1))
		{
			HIBYTE(v2) = HIBYTE(a2) + 1;
			v4 = mapHeightmap_11B4E0[v2];
			LOBYTE(v2)++;
			v8 = mapHeightmap_11B4E0[v2];
			LOBYTE(v3) = ~(x_BYTE)a2;
			HIBYTE(v2) = HIBYTE(a2);
			v6 = v3 * (mapHeightmap_11B4E0[v2] - v8) + (unsigned __int8)a1 * (v8 - v4);
		}
		else
		{
			v4 = mapHeightmap_11B4E0[v2];
			LOBYTE(v2)++;
			v5 = (unsigned __int8)a1 * (mapHeightmap_11B4E0[v2] - v4);
			LOBYTE(v2)--;
			HIBYTE(v2) = HIBYTE(a2) + 1;
			v6 = (unsigned __int8)a2 * (mapHeightmap_11B4E0[v2] - v4) + v5;
		}
		result = (v6 >> 3) + 32 * v4;
	}
	else
	{
		if ((unsigned __int8)a1 <= (unsigned __int8)v3)
		{
			v9 = mapHeightmap_11B4E0[v2];
			HIBYTE(v2) = HIBYTE(a2) + 1;
			v12 = mapHeightmap_11B4E0[v2];
			LOBYTE(v2)++;
			v11 = (unsigned __int8)a2 * (v12 - v9) + (unsigned __int8)a1 * (mapHeightmap_11B4E0[v2] - v12);
		}
		else
		{
			v9 = mapHeightmap_11B4E0[v2];
			LOBYTE(v2)++;
			v10 = mapHeightmap_11B4E0[v2];
			HIBYTE(v2) = HIBYTE(a2) + 1;
			v11 = (unsigned __int8)a2 * (mapHeightmap_11B4E0[v2] - v10) + (unsigned __int8)a1 * (v10 - v9);
		}
		result = (v11 >> 3) + 32 * v9;
	}
	return result;
}

//----- (00044DB0) --------------------------------------------------------
void sub_44DB0_truncTerrainHeight_orig()//225db0 // map to heightmap
{
	__int16 v0; // dx
	__int16 v1; // cx
	uint16_t v2; // bx
	__int16 v3; // ax
	int v4; // edx
	uint32_t x; // eax

	v0 = -32000;
	v1 = 32000;
	v2 = 0;
	do
	{
		v3 = mapEntityIndex_15B4E0[v2];
		if (v3 > v0)
			v0 = mapEntityIndex_15B4E0[v2];
		if (v3 < v1)
			v1 = mapEntityIndex_15B4E0[v2];
		v2++;
	} while (v2);//find min and max height
	if (v0)
		v4 = 0xC40000 / v0;
	else
		v4 = 0;
	do
	{
		x = v4 * mapEntityIndex_15B4E0[v2] >> 16;
		mapEntityIndex_15B4E0[v2] = 0;
		if ((x & 0x8000u) != 0)//water level trunc
			x = 0;
		if (x > 196)//trunc max height
			x = 196;
		mapHeightmap_11B4E0[v2] = x;
		v2++;
	} while (v2);
	//return result;
}

//----- (00044EE0) --------------------------------------------------------
void sub_44EE0_smooth_tiles_orig(/*int a1,*/ uaxis_2d a2x)//225ee0
{
	uaxis_2d v2x; // eax
	uint16_t v3; // dx
	unsigned __int8 v4; // dh
	//uaxis_2d v5x; // si
	//uaxis_2d v6x; // esi
	unsigned __int8 v7; // dl
	uint16_t index; // ax

	//[*][*][*]
	//[*][X][*]
	//[*][*][*]

	uaxis_2d a1x;

	v2x.word = a2x.word;
	v3 = 0;
	do
		mapTerrainType_10B4E0[v3++] = 3;
	while (v3);//set all tiles to 3
	v4 = mapHeightmap_11B4E0[a2x.word];
	do
	{
		//v5x.word = v2x.word;
		mapTerrainType_10B4E0[v2x.word] = 0;
		//BYTE1(v2)--;
		v2x._axis_2d.y--;

		//v6 = v5;
		//v6 = v2;
		v7 = 0xFFu;
		if (mapTerrainType_10B4E0[v2x.word] && mapHeightmap_11B4E0[v2x.word] < 0xFFu)
		{
			v7 = mapHeightmap_11B4E0[v2x.word];
			a1x.word = v2x.word;
		}
		//LOBYTE(v2)++;
		v2x._axis_2d.x++;
		if (mapTerrainType_10B4E0[v2x.word] && v7 > mapHeightmap_11B4E0[v2x.word])
		{
			v7 = mapHeightmap_11B4E0[v2x.word];
			a1x.word = v2x.word;
		}
		//BYTE1(v2)++;
		v2x._axis_2d.y++;
		if (mapTerrainType_10B4E0[v2x.word] && v7 > mapHeightmap_11B4E0[v2x.word])
		{
			v7 = mapHeightmap_11B4E0[v2x.word];
			a1x.word = v2x.word;
		}
		//BYTE1(v2)++;
		v2x._axis_2d.y++;
		if (mapTerrainType_10B4E0[v2x.word] && v7 > mapHeightmap_11B4E0[v2x.word])
		{
			v7 = mapHeightmap_11B4E0[v2x.word];
			a1x.word = v2x.word;
		}
		//LOBYTE(v2)--;
		v2x._axis_2d.x--;
		if (mapTerrainType_10B4E0[v2x.word] && v7 > mapHeightmap_11B4E0[v2x.word])
		{
			v7 = mapHeightmap_11B4E0[v2x.word];
			a1x.word = v2x.word;
		}
		//LOBYTE(v2)--;
		v2x._axis_2d.x--;
		if (mapTerrainType_10B4E0[v2x.word] && v7 > mapHeightmap_11B4E0[v2x.word])
		{
			v7 = mapHeightmap_11B4E0[v2x.word];
			a1x.word = v2x.word;
		}
		//BYTE1(v2)--;
		v2x._axis_2d.y--;
		if (mapTerrainType_10B4E0[v2x.word] && v7 > mapHeightmap_11B4E0[v2x.word])
		{
			v7 = mapHeightmap_11B4E0[v2x.word];
			a1x.word = v2x.word;
		}
		//BYTE1(v2)--;
		v2x._axis_2d.y--;
		if (mapTerrainType_10B4E0[v2x.word] && v7 > mapHeightmap_11B4E0[v2x.word])
		{
			v7 = mapHeightmap_11B4E0[v2x.word];
			a1x.word = v2x.word;
		}
		if (!mapAngle_13B4E0[a1x.word] || v7 == 0xFFu)
			break;
		if (v7 > v4)//if near tile is higger then central tile set central as near tile
			mapHeightmap_11B4E0[a1x.word] = v4;
		v4 = mapHeightmap_11B4E0[a1x.word];
		v2x.word = a1x.word;
	} while (v4);
	index = 0;
	do
	{
		if (!mapTerrainType_10B4E0[index])
			mapAngle_13B4E0[index] = 0;
		index++;
	} while (index);//delete type
}

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

void test_45210() {
	uint8_t* tempAng1 = (uint8_t*)malloc(65536);
	uint8_t* tempHeight1 = (uint8_t*)malloc(65536);

	uint8_t* tempAng2 = (uint8_t*)malloc(65536);
	uint8_t* tempHeight2 = (uint8_t*)malloc(65536);

	for (int y = 0; y < 256; y++)
	{
		if ((y > 5) && (y < 256 - 5))//skip steps for quick compare
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
					allert_error();
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
		if ((y > 5) && (y < 256 - 5))//skip steps for quick compare
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
					allert_error();
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
			allert_error();
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
				allert_error();
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
			allert_error();
	}
	free(tempAng1);
	free(tempHeight1);
	free(tempAng2);
	free(tempHeight2);
}

void test_44EE0() {
	uint8_t* tempTerr1 = (uint8_t*)malloc(65536);
	uint8_t* tempAng1 = (uint8_t*)malloc(65536);
	uint8_t* tempHeight1 = (uint8_t*)malloc(65536);

	uint8_t* tempTerr2 = (uint8_t*)malloc(65536);
	uint8_t* tempAng2 = (uint8_t*)malloc(65536);
	uint8_t* tempHeight2 = (uint8_t*)malloc(65536);

	for (int j = 0; j < 256 * 256; j++)
	{
		if ((j > 260) && (j < 256 * 256 - 260))//skip steps for quick compare
			continue;
		for (int i = 0; i < 256 * 256; i++)
		{
			mapTerrainType_10B4E0[i] = pseudoRand() % 256;
			mapAngle_13B4E0[i] = pseudoRand() % 256;
			mapHeightmap_11B4E0[i] = pseudoRand() % 256;
			tempTerr1[i] = mapTerrainType_10B4E0[i];
			tempAng1[i] = mapAngle_13B4E0[i];
			tempHeight1[i] = mapHeightmap_11B4E0[i];
		}
		uaxis_2d axis;
		axis.word = j;
		sub_44EE0_smooth_tiles_orig(axis);
		for (int i = 0; i < 256 * 256; i++)
		{
			tempTerr2[i] = mapTerrainType_10B4E0[i];
			mapTerrainType_10B4E0[i] = tempTerr1[i];
			tempAng2[i] = mapAngle_13B4E0[i];
			mapAngle_13B4E0[i] = tempAng1[i];
			tempHeight2[i] = mapHeightmap_11B4E0[i];
			mapHeightmap_11B4E0[i] = tempHeight1[i];
		}
		sub_44EE0_smooth_tiles(axis);
		for (int i = 0; i < 256 * 256; i++)
		{
			if ((tempTerr2[i] != mapTerrainType_10B4E0[i]) ||
				(tempAng2[i] != mapAngle_13B4E0[i]) ||
				(tempHeight2[i] != mapHeightmap_11B4E0[i]))
				allert_error();
		}
	}
	free(tempTerr1);
	free(tempAng1);
	free(tempHeight1);
	free(tempTerr2);
	free(tempAng2);
	free(tempHeight2);
}

void test_B5C60() {
	for (int i = 0; i < 256 * 256; i++)
		mapHeightmap_11B4E0[i] = pseudoRand() % 256;
	for (int y = 0; y < 256 * 256; y++)
	{
		if ((y > 260) && (y < 256 * 256 - 260))//skip steps for quick compare
			continue;
		for (int x = 0; x < 256 * 256; x++)
		{
			int a = sub_B5C60_getTerrainAlt2(x, y);
			int b = sub_B5C60_getTerrainAlt2_orig(x, y);
			if (a != b)
				allert_error();
		}
	}
};

void test_44DB0() {
	int16_t* tempTerrain1 = (int16_t*)malloc(65536 * sizeof(int16_t));
	int16_t* tempTerrain2 = (int16_t*)malloc(65536 * sizeof(int16_t));
	for (int i = 0; i < 256 * 256; i++)
	{
		mapEntityIndex_15B4E0[i] = pseudoRand() % (256 * 256);
		tempTerrain1[i] = mapEntityIndex_15B4E0[i];
	}
	sub_44DB0_truncTerrainHeight_orig();
	for (int i = 0; i < 256 * 256; i++)
	{
		tempTerrain2[i] = mapEntityIndex_15B4E0[i];
		mapEntityIndex_15B4E0[i] = tempTerrain1[i];
	}
	sub_44DB0_truncTerrainHeight();
	for (int i = 0; i < 256 * 256; i++)
	{
		if (mapEntityIndex_15B4E0[i] != tempTerrain2[i])
			allert_error();
	}
	free(tempTerrain2);
	free(tempTerrain1);
};

void Terrain_test() {
	test_44320();
	printf("test_44320 OK\n");
	test_45060();
	printf("test_45060 OK\n");
	test_440D0();
	printf("test_440D0 OK\n");
	test_45AA0();
	printf("test_45AA0 OK\n");
	test_44EE0();
	printf("test_44EE0 OK\n");
	test_44DB0();
	printf("test_44DB0 OK\n");
	test_B5C60();
	printf("test_B5C60 OK\n");
}
