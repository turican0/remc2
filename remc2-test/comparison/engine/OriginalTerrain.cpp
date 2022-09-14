#include "OriginalTerrain.h"

int sub_B5C60_getTerrainAlt2_orig(uint16_t a1, uint16_t a2, uint8_t* mapHeightmap_11B4E0)
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

void sub_44DB0_truncTerrainHeight_orig(int16_t mapEntityIndex_15B4E0[], uint8_t mapHeightmap_11B4E0[])//225db0 // map to heightmap
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
void sub_44EE0_smooth_tiles_orig(uaxis_2d a2x, uint8_t mapTerrainType_10B4E0[], uint8_t mapHeightmap_11B4E0[], uint8_t mapAngle_13B4E0[])//225ee0
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

void sub_44E40_orig(int a1, uint8_t a2, uint16_t x_WORD_17B4E0, uint8_t mapTerrainType_10B4E0[], uint8_t mapHeightmap_11B4E0[], uint8_t mapAngle_13B4E0[])//225e40
{
	int v2; // edi
	uint16_t v3; // si
	signed int v4; // ecx
	uint16_t v5; // ax
	uaxis_2d v6x; // dx
	uint16_t v7; // eax
	uint16_t v8; // ebx
	//unsigned __int16 i; // ax

	v2 = a1;
	v3 = 0;
	do
	{
		if (mapHeightmap_11B4E0[v3])
			mapAngle_13B4E0[v3] = 5;
		else
			mapAngle_13B4E0[v3] = 0;
		v3++;
	} while (v3);
LABEL_12:
	if (v2 > 0)
	{
		v4 = 1000;
		while (1)
		{
			//v8 = -1;
			v5 = 9377 * x_WORD_17B4E0 + 9439;
			x_WORD_17B4E0 = v5;
			v6x.word = v5 % 0xffffu;
			v7 = v5 % 0xffffu;
			v8 = mapHeightmap_11B4E0[v7] + 0xff00;
			if (!--v4)
				break;
			if ((v8 & 0xff) > a2 && mapAngle_13B4E0[v7])
			{
				v2--;
				//adress 225eb1
				//eax e568 ebx ff6f ecx 3e6 edx e568
				sub_44EE0_smooth_tiles_orig(/*v8,*/ v6x, mapTerrainType_10B4E0, mapHeightmap_11B4E0, mapAngle_13B4E0);
				goto LABEL_12;
			}
		}
	}
	do
	{
		//i = v3;
		mapTerrainType_10B4E0[v3++] = -1;
	} while (v3);
}

int sub_B5D68_orig(__int16 a1, __int16 a2, uint8_t* x_BYTE_14B4E0_second_heightmap)//296d68
{
	int v2; // ebx
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
	v3 = (unsigned __int8)a2;
	if ((HIBYTE(a1) + HIBYTE(a2)) & 1)
	{
		if (__CFADD__((x_BYTE)v3, (x_BYTE)a1))
		{
			HIBYTE(v2) = HIBYTE(a2) + 1;
			v4 = (unsigned __int8)x_BYTE_14B4E0_second_heightmap[v2];
			LOBYTE(v2) = v2 + 1;
			v8 = (unsigned __int8)x_BYTE_14B4E0_second_heightmap[v2];
			LOBYTE(v3) = ~(x_BYTE)a2;
			BYTE1(v2) = HIBYTE(a2);
			v6 = v3 * ((unsigned __int8)x_BYTE_14B4E0_second_heightmap[v2] - v8) + (unsigned __int8)a1 * (v8 - v4);
		}
		else
		{
			v4 = (unsigned __int8)x_BYTE_14B4E0_second_heightmap[v2];
			LOBYTE(v2) = v2 + 1;
			v5 = (unsigned __int8)a1 * ((unsigned __int8)x_BYTE_14B4E0_second_heightmap[v2] - v4);
			LOBYTE(v2) = v2 - 1;
			HIBYTE(v2) = HIBYTE(a2) + 1;
			v6 = (unsigned __int8)a2 * ((unsigned __int8)x_BYTE_14B4E0_second_heightmap[v2] - v4) + v5;
		}
		result = (v6 >> 3) + 32 * v4;
	}
	else
	{
		if ((unsigned __int8)a1 <= (unsigned __int8)v3)
		{
			v9 = (unsigned __int8)x_BYTE_14B4E0_second_heightmap[v2];
			HIBYTE(v2) = HIBYTE(a2) + 1;
			v12 = (unsigned __int8)x_BYTE_14B4E0_second_heightmap[v2];
			LOBYTE(v2)++;
			v11 = (unsigned __int8)a2 * (v12 - v9) + (unsigned __int8)a1 * ((unsigned __int8)x_BYTE_14B4E0_second_heightmap[v2] - v12);
		}
		else
		{
			v9 = (unsigned __int8)x_BYTE_14B4E0_second_heightmap[v2];
			LOBYTE(v2)++;
			v10 = (unsigned __int8)x_BYTE_14B4E0_second_heightmap[v2];
			HIBYTE(v2) = HIBYTE(a2) + 1;
			v11 = (unsigned __int8)a2 * ((unsigned __int8)x_BYTE_14B4E0_second_heightmap[v2] - v10) + (unsigned __int8)a1 * (v10 - v9);
		}
		result = (v11 >> 3) + 32 * v9;
	}
	return result;
}

void sub_45DC0_orig(uint8_t a2, uaxis_2d a3x, unsigned __int8 a4, bool lowDiffHeightmap_D47DC, char isCaveLevel_D41B6, uint16_t x_WORD_17B4E0, char x_BYTE_F2CD0x[][2], uint8_t mapTerrainType_10B4E0[], uint8_t mapHeightmap_11B4E0[], uint8_t mapAngle_13B4E0[], uint8_t mapShading_12B4E0[], uint8_t* x_BYTE_14B4E0_second_heightmap)//226dc0
{
	uaxis_2d v4x; // bx
	uaxis_2d v5x; // eax
	char v6; // ch
	uint8_t* v7; // eax
	char v8; // al
	char v9; // ah
	unsigned __int8 v10; // al
	char v11; // cl
	unsigned __int8 v12; // dh
	unsigned __int8 v13; // dh
	char v14; // cl
	unsigned __int8 v15; // dh
	char v16; // cl
	unsigned __int8 v17; // dh
	char v18; // bl
	uaxis_2d v19x; // ax
	char v20; // ch
	uaxis_2d v21x; // ax
	char v22; // dl
	unsigned __int8 v24; // [esp+0h] [ebp-4h]

	v4x._axis_2d.y = a3x._axis_2d.y;
	v24 = 8 * ((a3x._axis_2d.y + a3x._axis_2d.x) & 1);
	if (a4 >= 8u)
	{
		switch (a4)
		{
		case 8u:
			mapTerrainType_10B4E0[a3x.word] = 8;
			break;
		case 9u:
			mapTerrainType_10B4E0[a3x.word] = 9;
			break;
		case 0xAu:
			v8 = sub_45BE0_orig(a2, a3x, lowDiffHeightmap_D47DC, mapHeightmap_11B4E0);
			v9 = v8;
			if (lowDiffHeightmap_D47DC)
				v9 = v8 + 8;
			v10 = v9;
			goto LABEL_28;
		case 0xBu:
			v10 = sub_45BE0_orig(a2, a3x, lowDiffHeightmap_D47DC, mapHeightmap_11B4E0) + 16;
			if (lowDiffHeightmap_D47DC)
				v10 += 8;
			goto LABEL_28;
		case 0xCu:
			v10 = sub_45BE0_orig(a2, a3x, lowDiffHeightmap_D47DC, mapHeightmap_11B4E0) + 32;
			if (lowDiffHeightmap_D47DC)
				v10 += 8;
			goto LABEL_28;
		case 0xDu:
			v10 = sub_45BE0_orig(a2, a3x, lowDiffHeightmap_D47DC, mapHeightmap_11B4E0) + 48;
			if (lowDiffHeightmap_D47DC)
				v10 += 8;
			goto LABEL_28;
		case 0xEu:
			v10 = sub_45BE0_orig(a2, a3x, lowDiffHeightmap_D47DC, mapHeightmap_11B4E0) + 64;
			if (lowDiffHeightmap_D47DC)
				v10 += 8;
		LABEL_28:
			v7 = &unk_D4A30[0x80] + 2 * v10;
			goto LABEL_29;
		case 0xFu:
			mapTerrainType_10B4E0[a3x.word] = 11;
			break;
		case 0x10u:
			v6 = mapTerrainType_10B4E0[a3x.word];
			if (v6 == 10 || v6 == 11 || v6 == 12)
				break;
			v7 = unk_D4A30 + 2 * sub_45BE0_orig(a2, a3x, lowDiffHeightmap_D47DC, mapHeightmap_11B4E0);
			goto LABEL_29;
		case 0x11u:
			v7 = &unk_D4A30[64] + 2 * sub_45BE0_orig(a2, a3x, lowDiffHeightmap_D47DC, mapHeightmap_11B4E0);
			goto LABEL_29;
		case 0x12u:
			v7 = &unk_D4A30[80] + 2 * (v24 + sub_45BE0_orig(a2, a3x, lowDiffHeightmap_D47DC, mapHeightmap_11B4E0));
			goto LABEL_29;
		case 0x13u:
			v7 = &unk_D4A30[80] + 2 * (sub_45BE0_orig(a2, a3x, lowDiffHeightmap_D47DC, mapHeightmap_11B4E0) + v24) + 16;
			goto LABEL_29;
		case 0x14u:
			v7 = &unk_D4A30[16] + 2 * sub_45BE0_orig(a2, a3x, lowDiffHeightmap_D47DC, mapHeightmap_11B4E0);
			goto LABEL_29;
		case 0x15u:
			v7 = &unk_D4A30[16] + 2 * sub_45BE0_orig(a2, a3x, lowDiffHeightmap_D47DC, mapHeightmap_11B4E0) + 16;
			goto LABEL_29;
		case 0x16u:
			v7 = &unk_D4A30[16] + 2 * sub_45BE0_orig(a2, a3x, lowDiffHeightmap_D47DC, mapHeightmap_11B4E0) + 32;
		LABEL_29:
			mapTerrainType_10B4E0[a3x.word] = v7[0];
			mapAngle_13B4E0[a3x.word] = mapAngle_13B4E0[a3x.word] & 0x8F | v7[1];
			break;
		default:
			break;
		}
		v11 = mapAngle_13B4E0[a3x.word] | 0x80;
		mapAngle_13B4E0[a3x.word] = v11;
		if (isCaveLevel_D41B6)
		{
			v12 = mapHeightmap_11B4E0[a3x.word];
			if (x_BYTE_14B4E0_second_heightmap[a3x.word] > v12)
			{
				mapAngle_13B4E0[a3x.word] = v11 & 0xF7;
			}
			else
			{
				mapAngle_13B4E0[a3x.word] = v11 | 8;
				x_BYTE_14B4E0_second_heightmap[a3x.word] = v12 - 1;
			}
			v4x._axis_2d.x = a3x._axis_2d.x + 1;
			v13 = mapHeightmap_11B4E0[v4x.word];
			if (x_BYTE_14B4E0_second_heightmap[v4x.word] > v13)
			{
				mapAngle_13B4E0[v4x.word] &= 0xF7u;
			}
			else
			{
				v14 = mapAngle_13B4E0[v4x.word] | 8;
				x_BYTE_14B4E0_second_heightmap[v4x.word] = v13 - 1;
				mapAngle_13B4E0[v4x.word] = v14;
			}
			v4x._axis_2d.y = a3x._axis_2d.y + 1;
			v15 = mapHeightmap_11B4E0[v4x.word];
			if (x_BYTE_14B4E0_second_heightmap[v4x.word] > v15)
			{
				mapAngle_13B4E0[v4x.word] &= 0xF7u;
			}
			else
			{
				v16 = mapAngle_13B4E0[v4x.word] | 8;
				x_BYTE_14B4E0_second_heightmap[v4x.word] = v15 - 1;
				mapAngle_13B4E0[v4x.word] = v16;
			}
			v4x._axis_2d.x = a3x._axis_2d.x;
			v5x.word = v4x.word;
			v17 = mapHeightmap_11B4E0[v4x.word];
			if (x_BYTE_14B4E0_second_heightmap[v4x.word] <= v17)
			{
				v18 = mapAngle_13B4E0[v4x.word] | 8;
				x_BYTE_14B4E0_second_heightmap[v5x.word] = v17 - 1;
				mapAngle_13B4E0[v5x.word] = v18;
				return;
			}
		}
		else
		{
			v4x._axis_2d.x = a3x._axis_2d.x + 1;
			mapAngle_13B4E0[a3x.word] = v11 & 0xF7;
			v19x.word = v4x.word;
			v20 = mapAngle_13B4E0[v4x.word] & 0xF7;
			v4x._axis_2d.y = a3x._axis_2d.y + 1;
			mapAngle_13B4E0[v19x.word] = v20;
			v21x.word = v4x.word;
			v22 = mapAngle_13B4E0[v4x.word] & 0xF7;
			v4x._axis_2d.x = a3x._axis_2d.x;
			mapAngle_13B4E0[v21x.word] = v22;
			v5x.word = v4x.word;
		}
		mapAngle_13B4E0[v5x.word] &= 0xF7u;
		return;
	}
	mapAngle_13B4E0[a3x.word] = a4 | mapAngle_13B4E0[a3x.word] & 0xF0;
	sub_462A0_orig(a3x, a3x, isCaveLevel_D41B6, MapType_t::Day, x_WORD_17B4E0, x_BYTE_F2CD0x, mapTerrainType_10B4E0, mapHeightmap_11B4E0, mapAngle_13B4E0, mapShading_12B4E0, x_BYTE_14B4E0_second_heightmap);
}

void sub_462A0_orig(uaxis_2d a1x, uaxis_2d a2x, char isCaveLevel_D41B6, MapType_t mapType, uint16_t x_WORD_17B4E0, char x_BYTE_F2CD0x[][2], uint8_t mapTerrainType_10B4E0[], uint8_t mapHeightmap_11B4E0[], uint8_t mapAngle_13B4E0[], uint8_t mapShading_12B4E0[], uint8_t* x_BYTE_14B4E0_second_heightmap)//2272a0
{
	uaxis_2d v2x; // cx
	bool v3; // zf
	uaxis_2d v4x; // eax
	uint8_t v4y;
	uaxis_2d v5x; // cx
	int v6; // edx
	int v8; // edx
	int v9; // ebx
	int v10; // eax
	int v11index; // edx
	char v12; // ST10_1
	uaxis_2d v13x; // cx
	char v14; // dh
	char j; // dl
	uaxis_2d v16x; // ax
	uaxis_2d v17x; // bx
	char v18; // al
	char v19; // bl
	char i; // [esp+0h] [ebp-14h]
	char v23; // [esp+4h] [ebp-10h]
	char v24; // [esp+4h] [ebp-10h]
	char v25; // [esp+8h] [ebp-Ch]
	char v26; // [esp+8h] [ebp-Ch]
	char v27; // [esp+8h] [ebp-Ch]
	char v28; // [esp+Ch] [ebp-8h]

	v4x._axis_2d.x = a2x._axis_2d.x - a1x._axis_2d.x + 1;
	v2x.word = a1x.word;
	v25 = a2x._axis_2d.x - a1x._axis_2d.x + 1;
	v4x._axis_2d.y = a2x._axis_2d.y - a1x._axis_2d.y + 1;
	v3 = v4x._axis_2d.y == 0;
	v23 = a2x._axis_2d.y - a1x._axis_2d.y + 1;
	while (v4x._axis_2d.y != 0)
	{
		v4x._axis_2d.x = a2x._axis_2d.x - a1x._axis_2d.x + 1;
		while (v4x._axis_2d.x)
		{
			if ((int8_t)mapAngle_13B4E0[v2x.word] >= 0)
				mapTerrainType_10B4E0[v2x.word] = 1;
			v2x._axis_2d.x--;
			if ((int8_t)mapAngle_13B4E0[v2x.word] >= 0)
				mapTerrainType_10B4E0[v2x.word] = 1;
			v2x._axis_2d.y--;
			if ((int8_t)mapAngle_13B4E0[v2x.word] >= 0)
				mapTerrainType_10B4E0[v2x.word] = 1;
			v2x._axis_2d.x++;
			if ((int8_t)mapAngle_13B4E0[v2x.word] >= 0)
				mapTerrainType_10B4E0[v2x.word] = 1;
			v2x._axis_2d.y++;
			v4x._axis_2d.x--;
			v2x._axis_2d.x++;
		}
		v2x._axis_2d.x -= v25;
		v2x._axis_2d.y++;
		v3 = --v4x._axis_2d.y == 0;
	}
	a1x._axis_2d.x--;
	a1x._axis_2d.y--;
	v24 = v23 + 1;
	v28 = v24;
	v5x.word = a1x.word;
	v26 = v25 + 1;
	while (v28)
	{
		v4x._axis_2d.x = v26;
		for (i = v26; i; i--)
		{
			v4x.word = v5x.word;
			if (mapTerrainType_10B4E0[v5x.word] == 1)
			{
				v6 = 343 * (mapAngle_13B4E0[v4x.word] & 7);
				v5x._axis_2d.x++;
				v8 = 49 * (mapAngle_13B4E0[v5x.word] & 7) + v6;
				v5x._axis_2d.y++;
				v9 = mapAngle_13B4E0[v5x.word] & 7;
				v5x._axis_2d.x--;
				v10 = mapAngle_13B4E0[v5x.word] & 7;
				v5x._axis_2d.y--;
				v11index = v10 + 7 * v9 + v8;
				mapTerrainType_10B4E0[v5x.word] = x_BYTE_F2CD0x[v11index][0];
				if (x_BYTE_F2CD0x[v11index][0] >= 8u)
				{
					v4y = x_BYTE_F2CD0x[v11index][1] + (mapAngle_13B4E0[v5x.word] & 0x87);
				}
				else
				{
					v12 = mapAngle_13B4E0[v5x.word] & 0x87;
					x_WORD_17B4E0 = 9377 * x_WORD_17B4E0 + 9439;
					v4y = v12 + 16 * (x_WORD_17B4E0 % 7u);
				}
				mapAngle_13B4E0[v5x.word] = v4y;
			}
			v5x._axis_2d.x++;
		}
		v5x._axis_2d.x -= v26;
		v5x._axis_2d.y++;
		v28--;
	}
	v27 = v26 + 1;
	v13x.word = a1x.word;//fixed
	v14 = v24 + 1;
	if (v24 != -1)
	{
		do
		{
			for (j = v27; j; j--)
			{
				v13x._axis_2d.x++;
				v13x._axis_2d.y++;
				v16x.word = v13x.word;
				v13x._axis_2d.x -= 2;
				v13x._axis_2d.y -= 2;
				v17x.word = v13x.word;
				v13x._axis_2d.x++;
				v18 = mapHeightmap_11B4E0[v17x.word] - mapHeightmap_11B4E0[v16x.word] + 32;
				v13x._axis_2d.y++;
				if (v18 >= 28)
				{
					if (v18 > 40)
						v18 = (v18 & 7) + 40;
				}
				else
				{
					v18 = (v18 & 3) + 28;
				}
				if (mapType != MapType_t::Day)
					v19 = 32 - v18 + 32;
				else
					v19 = v18;
				v4x.word = v13x.word;
				mapShading_12B4E0[v13x.word] = v19;
				if (isCaveLevel_D41B6 && (x_BYTE_14B4E0_second_heightmap[v13x.word] <= mapHeightmap_11B4E0[v13x.word]))
				{
					x_BYTE_14B4E0_second_heightmap[v13x.word] = mapHeightmap_11B4E0[v13x.word] - 1;
					mapAngle_13B4E0[v13x.word] |= 8u;
				}
				else
				{
					mapAngle_13B4E0[v13x.word] &= 0xF7u;
				}
				v13x._axis_2d.x++;
			}
			v13x._axis_2d.x -= v27;
			v14--;
			v13x._axis_2d.y++;
		} while (v14);
	}
}

uint8_t sub_45BE0_orig(uint8_t a2, uaxis_2d a3x, bool lowDiffHeightmap_D47DC, uint8_t mapHeightmap_11B4E0[])//226be0
{
	uaxis_2d v3x; // ax
	uint8_t v4; // bl
	uint8_t v5; // dh
	uint8_t v6; // bh
	uint8_t v7; // al
	uint32_t v8; // edi
	uint8_t result; // al

	uint8_t a1 = 0;

	v3x.word = a3x.word;
	v4 = 0xFFu;
	v5 = 0;
	if (mapHeightmap_11B4E0[a3x.word])
	{
		v5 = mapHeightmap_11B4E0[a3x.word];
		a1 = 0;
	}
	if (mapHeightmap_11B4E0[a3x.word] < 0xFFu)
		v4 = mapHeightmap_11B4E0[a3x.word];
	v3x._axis_2d.x++;
	if (mapHeightmap_11B4E0[v3x.word] > v5)
	{
		v5 = mapHeightmap_11B4E0[v3x.word];
		a1 = 1;
	}
	if (mapHeightmap_11B4E0[v3x.word] < v4)
		v4 = mapHeightmap_11B4E0[v3x.word];
	v3x._axis_2d.y++;
	if (mapHeightmap_11B4E0[v3x.word] > v5)
	{
		v5 = mapHeightmap_11B4E0[v3x.word];
		a1 = 2;
	}
	if (mapHeightmap_11B4E0[v3x.word] < v4)
		v4 = mapHeightmap_11B4E0[v3x.word];
	v3x._axis_2d.x--;
	if (mapHeightmap_11B4E0[v3x.word] > v5)
	{
		v5 = mapHeightmap_11B4E0[v3x.word];
		a1 = 3;
	}
	if (mapHeightmap_11B4E0[v3x.word] < v4)
		v4 = mapHeightmap_11B4E0[v3x.word];
	v3x._axis_2d.y--;
	v6 = 0;
	if (a1 && mapHeightmap_11B4E0[v3x.word])
	{
		v6 = mapHeightmap_11B4E0[v3x.word];
		a2 = 0;
	}
	v3x._axis_2d.x++;
	if (a1 != 1 && mapHeightmap_11B4E0[v3x.word] > v6)
	{
		v6 = mapHeightmap_11B4E0[v3x.word];
		a2 = 1;
	}
	v3x._axis_2d.y++;
	if (a1 != 2 && mapHeightmap_11B4E0[v3x.word] > v6)
	{
		v6 = mapHeightmap_11B4E0[v3x.word];
		a2 = 2;
	}
	v3x._axis_2d.x--;
	if (a1 != 3)
	{
		v7 = mapHeightmap_11B4E0[v3x.word];
		if (v7 > v6)
		{
			a2 = 3;
			v6 = v7;
		}
	}
	v8 = 0;
	if (v5 - v4 <= 8)
		v8 = 1;
	if (v5 - v6 >= 8)
	{
		result = a1;
	LABEL_44:
		lowDiffHeightmap_D47DC = v8;
		return result;
	}
	if (a1 > 3u)
	{
		result = 0;
		goto LABEL_44;
	}
	switch (a1)
	{
	case 0:
		if (a2 != 1)
			goto LABEL_42;
		goto LABEL_36;
	case 1:
		if (a2 == 2)
			goto LABEL_38;
	LABEL_36:
		result = 4;
		lowDiffHeightmap_D47DC = v8;
		return result;
	case 2:
		if (a2 == 3)
			goto LABEL_40;
	LABEL_38:
		result = 5;
		lowDiffHeightmap_D47DC = v8;
		break;
	case 3:
		if (a2)
		{
		LABEL_40:
			result = 6;
			lowDiffHeightmap_D47DC = v8;
		}
		else
		{
		LABEL_42:
			result = 7;
			lowDiffHeightmap_D47DC = v8;
		}
		break;
	}
	return result;
}

bool sub_33F70_orig(unsigned __int16 a1, uint8_t mapTerrainType_10B4E0[], uint8_t mapHeightmap_11B4E0[])//214f70
{
	unsigned __int16 v1; // ax
	signed int v2; // edx
	unsigned __int16 v3; // ax
	bool result; // al

	HIBYTE(v1) = HIBYTE(a1);
	LOBYTE(v1) = a1 - 1;
	result = 1;
	if (mapTerrainType_10B4E0[v1] == 8)
	{
		v2 = (unsigned __int16)(mapHeightmap_11B4E0[a1] + 30);
		if (mapHeightmap_11B4E0[v1] <= (signed int)(unsigned __int16)v2)
		{
			LOBYTE(v1) = v1 + 2;
			if (mapHeightmap_11B4E0[v1] <= v2)
			{
				LOBYTE(v3) = v1 - 1;
				HIBYTE(v3) = HIBYTE(a1) + 1;
				if (mapHeightmap_11B4E0[v3] <= v2)
				{
					HIBYTE(v3) = HIBYTE(a1) - 1;
					if (mapHeightmap_11B4E0[v3] <= v2)
						result = 0;
				}
			}
		}
	}
	return result;
}

int sub_1B830_orig(axis_3d* a1, uint8_t mapHeightmap_11B4E0[])//1fc830
{
	unsigned __int16 v1; // ax
	unsigned __int16 v2; // dx
	unsigned __int16 v3; // bx
	unsigned __int8 v4; // cl
	unsigned __int16 v5; // bx
	int v6; // esi
	int v7; // ebx
	int v8; // ecx
	int v9; // ebx

	LOBYTE(v1) = (a1->x >> 8) - 1;
	HIBYTE(v1) = (a1->y >> 8) - 1;
	v2 = v1;
	LOBYTE(v1) = v1 + 2;
	v3 = v1;
	HIBYTE(v1) += 2;
	v4 = mapHeightmap_11B4E0[v3];
	v5 = v1;
	LOBYTE(v2) = mapHeightmap_11B4E0[v2];
	LOBYTE(v1) = v1 - 2;
	HIBYTE(v2) = mapHeightmap_11B4E0[v5];
	v6 = mapHeightmap_11B4E0[v1];
	v7 = (unsigned __int8)v2 + v4 - v6;
	v8 = abs((unsigned __int8)v2 + v6 - v4 - HIBYTE(v2));
	v9 = abs(v7 - HIBYTE(v2));
	if (v8 > v9)
		v9 = v8;
	return v9;
}

int sub_1B7A0_tile_compare_orig(axis_3d* a1, uint8_t mapHeightmap_11B4E0[])//1fc7a0
{
	uint8_t v1x[2]; // eax
	//uint8_t v2x[2]; // dx
	//uint8_t v3x[2]; // bx
	unsigned __int8 v4_00; // cl
	unsigned __int8 v4_01; // cl
	unsigned __int8 v4_10; // cl
	unsigned __int8 v4_11; // cl

	//uint8_t v5x[2]; // bx
	//int v6; // esi
	//int v7; // ebx
	int v8; // ecx
	int v9; // ebx

	v1x[0] = a1->x >> 8;
	v1x[1] = a1->y >> 8;

	v4_00 = mapHeightmap_11B4E0[*(uint16_t*)v1x];//0000
	v1x[0]++;
	v4_10 = mapHeightmap_11B4E0[*(uint16_t*)v1x];//+100
	v1x[1]++;
	v4_11 = mapHeightmap_11B4E0[*(uint16_t*)v1x];//+1+1
	v1x[0]--;
	v4_01 = mapHeightmap_11B4E0[*(uint16_t*)v1x];//00+1

	//v7 = v4_00 + v4_10 - v4_01;
	v8 = abs(v4_00 + v4_01 - v4_10 - v4_11);
	v9 = abs(v4_00 + v4_10 - v4_01 - v4_11);
	if (v8 > v9)
		v9 = v8;
	return v9;
}

void sub_43D50_orig(uint8_t mapTerrainType_10B4E0[], uint8_t mapHeightmap_11B4E0[], uint8_t mapAngle_13B4E0[])//224d50
{
	uint16_t index; // ax
	uint16_t v1; // dx
	//uint16_t v2; // bx
	//uint16_t v3; // bx
	//uint16_t v4; // bx
	//uint16_t v5; // bx
	//uint16_t v6; // bx
	uint16_t v7; // bx
	uint16_t v8; // dx
	uint16_t v9; // bx
	uint16_t v10; // bx

	index = 0;
	do
	{
		mapAngle_13B4E0[index] &= 0xF7u;
		if (!mapHeightmap_11B4E0[index])
		{
			HIBYTE(index)--;
			v1 = index;
			LOBYTE(index)++;
			//v2 = index;
			LOBYTE(v1) = (mapHeightmap_11B4E0[index] != 0) + (mapHeightmap_11B4E0[v1] != 0);
			HIBYTE(index)++;
			//v3 = index;
			LOBYTE(v1) = (mapHeightmap_11B4E0[index] != 0) + v1;
			HIBYTE(index)++;
			//v4 = index;
			LOBYTE(v1) = (mapHeightmap_11B4E0[index] != 0) + v1;
			LOBYTE(index)--;
			//v5 = index;
			LOBYTE(v1) = (mapHeightmap_11B4E0[index] != 0) + v1;
			LOBYTE(index)--;
			//v6 = index;
			LOBYTE(v1) = (mapHeightmap_11B4E0[index] != 0) + v1;
			HIBYTE(index)--;
			v7 = index;
			HIBYTE(index)--;
			LOBYTE(v1) = (mapHeightmap_11B4E0[v7] != 0) + v1;
			LOBYTE(v7) = mapHeightmap_11B4E0[index] != 0;
			LOBYTE(index)++;
			HIBYTE(index)++;
			if (!(LOBYTE(v7) + LOBYTE(v1)))
			{
				v8 = index;
				LOBYTE(index)--;
				v9 = index;
				HIBYTE(index)--;
				LOBYTE(v8) = (mapTerrainType_10B4E0[v9] != 0) + (mapTerrainType_10B4E0[v8] != 0);
				v10 = index;
				LOBYTE(index)++;
				LOBYTE(v8) = (mapTerrainType_10B4E0[index] != 0) + (mapTerrainType_10B4E0[v10] != 0) + v8;
				HIBYTE(index)++;
				if (!(x_BYTE)v8)
					mapAngle_13B4E0[index] |= 8u;
			}
		}
		index++;
	} while (index);
}

void sub_44D00_orig(MapType_t mapType, uint8_t mapHeightmap_11B4E0[], uint8_t mapShading_12B4E0[])//225d00
{
	uaxis_2d v0x; // cx
	//uaxis_2d v1x; // dx
	uaxis_2d indexx; // eax
	//unsigned __int16 v3; // cx
	//uaxis_2d v4x; // et2
	char v5; // dl

	v0x.word = 0;
	uint16_t x_WORD_17B4E0 = 0;
	do
	{
		//adress 225d0f
		v0x._axis_2d.x++;
		//indexx._axis_2d.y = 0;
		v0x._axis_2d.y++;
		indexx.word = v0x.word;
		v0x._axis_2d.x -= 2;
		v0x._axis_2d.y -= 2;
		//v1x.word = v0x.word;

		indexx._axis_2d.x = mapHeightmap_11B4E0[v0x.word] - mapHeightmap_11B4E0[indexx.word] + 32;
		v0x._axis_2d.x++;
		v0x._axis_2d.y++;
		if (indexx._axis_2d.x == 32)
		{
			indexx.word = 9377 * x_WORD_17B4E0 + 9439;
			//LOWORD(index) += 9439;
			x_WORD_17B4E0 = indexx.word;
			//v4x.word = indexx.word;
			indexx._axis_2d.y = (x_WORD_17B4E0 / 9u) >> 8;
			indexx._axis_2d.x = x_WORD_17B4E0 % 9 + 28;
		}
		else if ((int8_t)indexx._axis_2d.x >= 28)
		{
			if ((int8_t)indexx._axis_2d.x > 40)
				indexx._axis_2d.x = (indexx._axis_2d.x & 7) + 40;
		}
		else
		{
			indexx._axis_2d.x = (indexx._axis_2d.x & 3) + 28;
		}
		if (mapType != MapType_t::Day)
		{
			//index = 32 - (index & 0xff);
			//v5 = (32 - (indexx._axis_2d.x)) + 32;
			v5 = (64 - indexx._axis_2d.x);
		}
		else
		{
			v5 = indexx._axis_2d.x;
		}
		//LOBYTE(index) = v5;
		mapShading_12B4E0[v0x.word] = v5;
		v0x.word++;
	} while (v0x.word);
}

void /*__spoils<ecx>*/ sub_B5E70_decompress_terrain_map_level_orig(__int16 a1, unsigned __int16 a2, __int16 a3, int32_t a4, int16_t mapEntityIndex_15B4E0[])//296e70
{
	/*
	uint16_t v3; // ebx
	__int16 v4; // cx
	//char v5; // sf
	//char v6; // of
	int8_t v7; // [esp+1h] [ebp-3h]
	int8_t v8; // [esp+1h] [ebp-3h]
	int8_t v9; // [esp+2h] [ebp-2h]
	int8_t v10; // [esp+2h] [ebp-2h]
	int8_t v11; // [esp+3h] [ebp-1h]
	//uint8_t savedregs[20]; // [esp+4h] [ebp+0h]

	mapEntityIndex_15B4E0[a2] = a3;//32c4e0 //first seed
	v11 = 7;
	do
	{
		v3 = a2;
		v4 = (1 << v11);
		v7 = 1 << (7 - v11);
		do
		{
			v9 = 1 << (7 - v11);
			do
			{
				sub_B5EFA_orig(v4, &v3, a4, &a1);//355220
				v9--;
			} while (v9);
			v3 += (v4 + v4) << 8;
			v7--;
		} while (v7);
		v8 = 1 << (7 - v11);
		do
		{
			v10 = 1 << (7 - v11);
			do
			{
				sub_B5F8F_orig(v4, &v3, a4, &a1);
				v10--;
			} while (v10);
			v3 += (v4 + v4) << 8;
			v8--;
		} while (v8);
		v11--;
		//v6 = __OFSUB__(v11, 1);
		//v5 = (v11-- - 1) < 0;
	} while (v11 >= 1);
	*/
	uint16_t v3; // ebx
	__int16 v4; // cx
	char v6; // [esp+1h] [ebp-3h]
	char v7; // [esp+1h] [ebp-3h]
	char v8; // [esp+2h] [ebp-2h]
	char v9; // [esp+2h] [ebp-2h]
	char v10; // [esp+3h] [ebp-1h]
	int savedregs; // [esp+4h] [ebp+0h] BYREF

	mapEntityIndex_15B4E0[a2] = a3;
	v10 = 7;
	do
	{
		v3 = a2;
		v4 = (unsigned __int8)(1 << v10);
		v6 = 1 << (7 - v10);
		do
		{
			v8 = 1 << (7 - v10);
			do
			{
				sub_B5EFA_orig(v4, &v3, a4, &a1, mapEntityIndex_15B4E0);
				--v8;
			} while (v8);
			BYTE1(v3) += v4 + v4;
			--v6;
		} while (v6);
		v7 = 1 << (7 - v10);
		do
		{
			v9 = 1 << (7 - v10);
			do
			{
				sub_B5F8F_orig(v4, &v3, a4, &a1, mapEntityIndex_15B4E0);
				--v9;
			} while (v9);
			BYTE1(v3) += v4 + v4;
			--v7;
		} while (v7);
	} while (v10-- >= 1);
}

void sub_B5EFA_orig(__int16 a1, uint16_t* a2, int32_t a3, __int16* a4, int16_t mapEntityIndex_15B4E0[])//296EFA
{
	int16_t v4; // di
	//__int16 v5; // di
	//__int16 v6; // di
	uint16_t v7; // si
	uint32_t v8; // di
	//__int16 result; // ax

	v4 = mapEntityIndex_15B4E0[*a2];
	LOBYTE(*a2) += (a1 + a1);
	v4 += mapEntityIndex_15B4E0[*a2];
	*a2 += (a1 + a1) << 8;
	v4 += mapEntityIndex_15B4E0[*a2];
	LOBYTE(*a2) -= (a1 + a1);
	v4 += mapEntityIndex_15B4E0[*a2];
	LOBYTE(*a2) += a1;
	*a2 -= a1 << 8;
	v7 = 9377 * *a4 + 9439;
	*a4 = v7;
	v8 = v7 % (uint16_t)(2 * a3 + 1)
		+ v7 % (uint16_t)((a1 << 6) + 1)
		+ (v4 >> 2)
		- 32 * a1
		- a3;
	//result = v8;
	if (!mapEntityIndex_15B4E0[*a2])
		mapEntityIndex_15B4E0[*a2] = v8;
	LOBYTE(*a2) += a1;
	*a2 -= a1 << 8;
	//return result;
}

void sub_B5F8F_orig(__int16 a1, uint16_t* a2, int32_t a3, __int16* a4, int16_t mapEntityIndex_15B4E0[])//296f8f
{
	__int16 v4; // di
	//__int16 v5; // di
	//__int16 v6; // di
	uint16_t v7; // si
	uint16_t v7b; // si
	uint16_t v8; // ax
	int16_t v9; // di
	//__int16 v10; // di
	//__int16 v11; // di
	uint32_t v12; // di
	//__int16 result; // ax
	uint16_t v14; // [esp-2h] [ebp-2h]

	v4 = mapEntityIndex_15B4E0[*a2];
	v14 = v4;
	LOBYTE(*a2) += a1;
	*a2 -= a1 << 8;
	v4 += mapEntityIndex_15B4E0[*a2];
	LOBYTE(*a2) += a1;
	*a2 += a1 << 8;
	v4 += mapEntityIndex_15B4E0[*a2];
	LOBYTE(*a2) -= a1;
	*a2 += a1 << 8;
	v4 += mapEntityIndex_15B4E0[*a2];
	v7 = 9377 * *a4 + 9439;
	//*a4 = v7;
	//2ae9*24a1
	v8 = v7 % (uint16_t)(2 * a3 + 1)
		+ v7 % (uint16_t)((a1 << 6) + 1)
		+ (uint16_t)(v4 >> 2)
		- 32 * a1
		- a3;
	//(uint16_t)(v4 >> 2)-(a1*32)+v7 % (uint16_t)((a1 << 6) + 1)-a3+v7 % (uint16_t)(2 * a3 + 1)
	v9 = mapEntityIndex_15B4E0[*a2];
	*a2 -= a1 << 8;
	if (!mapEntityIndex_15B4E0[*a2])
		mapEntityIndex_15B4E0[*a2] = v8;
	v9 += v14;
	LOBYTE(*a2) -= (a1 + a1);
	*a2 += a1 << 8;
	v9 += mapEntityIndex_15B4E0[*a2];
	LOBYTE(*a2) += a1;
	*a2 += a1 << 8;
	v9 += mapEntityIndex_15B4E0[*a2];
	*a2 -= a1 << 8;
	v7b = 9377 * v7 + 9439;
	*a4 = v7b;
	v12 = v7b % (unsigned __int16)(2 * a3 + 1)
		+ v7b % (unsigned __int16)((a1 << 6) + 1)
		+ (uint16_t)(v9 >> 2)
		- 32 * a1
		- a3;
	//result = v12;
	if (!mapEntityIndex_15B4E0[*a2])
		mapEntityIndex_15B4E0[*a2] = v12;
	*a2 -= a1 << 8;
	LOBYTE(*a2) += (a1 + a1);
	//return result;
}

//----- (000439A0) --------------------------------------------------------
unsigned int sub_439A0_orig(unsigned __int16 a2, uint8_t mapHeightmap_11B4E0[], uint8_t mapAngle_13B4E0[])//2249a0
{
	unsigned __int16 v2; // ax
	unsigned __int8 v3; // dl
	unsigned __int8 v4; // dh
	unsigned __int8 v5; // ch
	unsigned __int8 v6; // cl
	int v7; // ebx
	unsigned __int8 v8; // cl
	int v9; // ebx
	unsigned __int8 v10; // cl
	int v11; // ebx
	unsigned __int8 v12; // cl
	int v13; // ebx
	unsigned __int8 v14; // cl
	int v15; // ebx
	unsigned __int8 v16; // cl
	int v17; // ebx
	unsigned __int8 v18; // cl
	int v19; // ebx
	unsigned __int8 v20; // al
	unsigned int v21; // ebx
	unsigned int v22; // ebx

	unsigned int a1;
	LOBYTE(v2) = a2;
	LOWORD(a1) = mapHeightmap_11B4E0[a2];
	if (mapAngle_13B4E0[a2] & 7)
	{
		HIBYTE(v2) = HIBYTE(a2) - 1;
		v3 = mapHeightmap_11B4E0[a2];
		v4 = v3;
		v5 = mapHeightmap_11B4E0[a2];
		v6 = mapHeightmap_11B4E0[v2];
		v7 = v6;
		if (v6 > v3)
			v3 = mapHeightmap_11B4E0[v2];
		if (v6 < v4)
			v4 = mapHeightmap_11B4E0[v2];
		LOBYTE(v2) = a2 + 1;
		v8 = mapHeightmap_11B4E0[v2];
		v9 = v8 + v7;
		if (v8 > v3)
			v3 = mapHeightmap_11B4E0[v2];
		if (v8 < v4)
			v4 = mapHeightmap_11B4E0[v2];
		HIBYTE(v2) = HIBYTE(a2);
		v10 = mapHeightmap_11B4E0[v2];
		v11 = v10 + v9;
		if (v10 > v3)
			v3 = mapHeightmap_11B4E0[v2];
		if (v10 < v4)
			v4 = mapHeightmap_11B4E0[v2];
		HIBYTE(v2) = HIBYTE(a2) + 1;
		v12 = mapHeightmap_11B4E0[v2];
		v13 = v12 + v11;
		if (v12 > v3)
			v3 = mapHeightmap_11B4E0[v2];
		if (v12 < v4)
			v4 = mapHeightmap_11B4E0[v2];
		LOBYTE(v2) = a2;
		v14 = mapHeightmap_11B4E0[v2];
		v15 = v14 + v13;
		if (v14 > v3)
			v3 = mapHeightmap_11B4E0[v2];
		if (v14 < v4)
			v4 = mapHeightmap_11B4E0[v2];
		LOBYTE(v2) = a2 - 1;
		v16 = mapHeightmap_11B4E0[v2];
		v17 = v16 + v15;
		if (v16 > v3)
			v3 = mapHeightmap_11B4E0[v2];
		if (v16 < v4)
			v4 = mapHeightmap_11B4E0[v2];
		HIBYTE(v2) = HIBYTE(a2);
		v18 = mapHeightmap_11B4E0[v2];
		v19 = v18 + v17;
		if (v18 > v3)
			v3 = mapHeightmap_11B4E0[v2];
		if (v18 < v4)
			v4 = mapHeightmap_11B4E0[v2];
		HIBYTE(v2) = HIBYTE(a2) - 1;
		v20 = mapHeightmap_11B4E0[v2];
		v21 = v20 + v19;
		if (v20 > v3)
			v3 = v20;
		if (v20 < v4)
			v4 = v20;
		v22 = v21 >> 3;
		if ((unsigned __int8)(v5 - v4) <= 4u)
		{
			if ((unsigned __int8)(v3 - v5) <= 4u)
				return a1;
			if ((unsigned __int8)(v3 - v5) <= 0xAu)
				v22 = (v5 + v22) >> 1;
		}
		else if ((unsigned __int8)(v5 - v4) <= 0xAu)
		{
			return (v22 + v5) >> 1;
		}
		a1 = v22;
	}
	return a1;
}

//----- (00043B40) --------------------------------------------------------
void sub_43B40_orig(uint8_t x_BYTE_D41B7, uint8_t mapHeightmap_11B4E0[], uint8_t mapAngle_13B4E0[], uint8_t* x_BYTE_14B4E0_second_heightmap)//224b40
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
	sub_43BB0_orig(mapHeightmap_11B4E0, mapAngle_13B4E0, x_BYTE_14B4E0_second_heightmap);
}

//----- (00043BB0) --------------------------------------------------------
void sub_43BB0_orig(uint8_t mapHeightmap_11B4E0[], uint8_t mapAngle_13B4E0[], uint8_t* x_BYTE_14B4E0_second_heightmap)//224bb0
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