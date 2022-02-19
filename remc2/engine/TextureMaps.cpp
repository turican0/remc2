#include "TextureMaps.h"
#include "../utilities/BitmapIO.h"

type_BIG_SPRITES_BUFFER BIG_SPRITES_BUFFERx[max_sprites];

FILE* x_DWORD_DB73C_tmapsfile;
FILE* x_DWORD_DB740_tmaps00file;
FILE* x_DWORD_DB744_tmaps10file;
FILE* x_DWORD_DB748_tmaps20file;

type_particle_str** str_DWORD_F66F0x[504];
char x_BYTE_F5340[504];
int32_t x_DWORD_F5730[504];
subtype_x_DWORD_E9C28_str* str_F5F10[504];

type_x_DWORD_E9C28_str* x_DWORD_E9C28_str;

type_E9C08* x_DWORD_E9C08x; // weak
bool big_sprites_inited = false;
uint8_t* m_pColorPalette = NULL;

bool MainInitTmaps_71520(unsigned __int16 a1)
{
	int v1; // esi
	int v2; // ebx
	signed int i; // ebx
	unsigned __int16 v4; // ax

	v1 = 0;
	v2 = sub_70EF0(a1);
	for (i = v2 - sub_71E60(x_DWORD_E9C28_str) + 20; i > 0; i -= sub_71090(i))
	{
		v4 = v1++;
		if (v4 >= 4u)
			break;
	}
	if (i <= 0)
	{
		InitTmaps(a1);
		x_D41A0_BYTEARRAY_4_struct.byteindex_177 = 5;
	}
	return str_DWORD_F66F0x[a1] != 0;
}

int sub_70EF0(unsigned __int16 a1)
{
	unsigned __int16 v1; // dx
	int i; // ebx
	//int v3; // ecx
	v1 = str_TMAPS00TAB_BEGIN_BUFFER[a1].word_8;
	for (i = 0; str_TMAPS00TAB_BEGIN_BUFFER[a1].word_8 < 504; v1++)
	{
		//v3 = 10 * v1;
		if (str_TMAPS00TAB_BEGIN_BUFFER[a1].word_8 != str_TMAPS00TAB_BEGIN_BUFFER[v1].word_8)
			break;
		i += str_TMAPS00TAB_BEGIN_BUFFER[v1].word_8;
	}
	return i;
}

int sub_71E60(type_x_DWORD_E9C28_str* a1y)//252e60
{
	return a1y->dword_4;
}

signed int sub_71CD0(type_x_DWORD_E9C28_str* a1y)//252cd0
{
	int i; // edx

	for (i = 0; (signed __int16)i < (signed int)a1y->word_22; i++)
	{
		//if (!*(x_DWORD*)(14 * (signed __int16)i + a1y->dword_8_data + 4))
		if (!a1y->str_8_data[i].dword_4)
			return i;
	}
	return -1;
}

unsigned int sub_71090(unsigned int a1)//252090
{
	int v1; // eax
	unsigned __int16 v2; // dx
	int v3; // eax
	int v4; // ebx
	int v5; // ebx
	int v6; // ecx
	unsigned int v7; // eax
	int v8; // edx
	int v9; // esi
	int v10; // esi
	int v11; // edi
	int v12; // edi
	int v13; // esi
	int v14; // esi
	int v15; // edi
	unsigned __int16 v16; // di
	unsigned int v17; // ebx
	char v18; // al
	int v19; // esi
	int v21; // [esp+0h] [ebp-2Ch]
	int v22; // [esp+4h] [ebp-28h]
	int v23; // [esp+8h] [ebp-24h]
	int v24; // [esp+Ch] [ebp-20h]
	int v25; // [esp+10h] [ebp-1Ch]
	unsigned int v26; // [esp+14h] [ebp-18h]
	unsigned int v27; // [esp+18h] [ebp-14h]
	unsigned int v28; // [esp+1Ch] [ebp-10h]
	unsigned int v29; // [esp+20h] [ebp-Ch]
	unsigned int v30; // [esp+24h] [ebp-8h]
	char v31; // [esp+28h] [ebp-4h]

	//fix it
	v22 = 0;
	v23 = 0;
	v24 = 0;
	v27 = 0;
	v28 = 0;
	v29 = 0;
	v30 = 0;
	//fix it

	v31 = 1;
	v1 = 0;
	do
	{
		v2 = v1++;
		*(&v26 + v2) = -1;
		*(&v21 + v2) = -1;
	} while ((unsigned __int16)v1 < 5u);
	v3 = 0;
	do
	{
		v4 = str_TMAPS00TAB_BEGIN_BUFFER[v3 + 1].word_8;
		if (str_DWORD_F66F0x[v4] && !x_BYTE_F5340[v4])
			v31 = 0;
		while ((unsigned __int16)v3 < 0x1F8u
			&& str_TMAPS00TAB_BEGIN_BUFFER[v3 + 1].word_8 == v4)
			++v3;
		++v3;
	} while ((unsigned __int16)v3 < 0x1F8u);
	v5 = 0;
	do
	{
		v6 = str_TMAPS00TAB_BEGIN_BUFFER[v5].word_8;
		if ((!x_BYTE_F5340[v6] || v31) && str_DWORD_F66F0x[v6])
		{
			v7 = x_DWORD_F5730[v6];
			v8 = str_TMAPS00TAB_BEGIN_BUFFER[v5].word_8;
			if (v7 < v26)
			{
				v9 = v7 ^ v26;
				v7 ^= v26 ^ v7;
				v26 = v7 ^ v9;
				v8 = v6 ^ v6 ^ v21;
				v21 ^= v8 ^ v6;
			}
			if (v7 < v27)
			{
				v10 = v7 ^ v27;
				v11 = v8 ^ v22;
				v7 ^= v27 ^ v7;
				v8 ^= v22 ^ v8;
				v27 = v7 ^ v10;
				v22 = v8 ^ v11;
			}
			if (v7 < v28)
			{
				v12 = v7 ^ v28;
				v13 = v8 ^ v23;
				v7 ^= v28 ^ v7;
				v8 ^= v23 ^ v8;
				v28 = v7 ^ v12;
				v23 = v8 ^ v13;
			}
			if (v7 < v29)
			{
				v14 = v7 ^ v29;
				v15 = v8 ^ v24;
				v7 ^= v29 ^ v7;
				v8 ^= v24 ^ v8;
				v29 = v7 ^ v14;
				v24 = v8 ^ v15;
			}
			if (v7 < v30)
			{
				v30 ^= v7 ^ v7 ^ v7 ^ v30;
				v25 ^= v8 ^ v8 ^ v8 ^ v25;
			}
		}
		while ((unsigned __int16)v5 < 0x1F8u
			&& str_TMAPS00TAB_BEGIN_BUFFER[v5 + 1].word_8 == v6)
			++v5;
		++v5;
	} while ((unsigned __int16)v5 < 0x1F8u);
	v16 = 0;
	v17 = 0;
	while (v16 < 5u)
	{
		if (v17 >= a1)
			break;
		v19 = 4 * v16;
		if (*(int*)((char*)&v21 + v19) <= -1)
			break;
		if (v31)
			v18 = sub_70E10(*(x_WORD*)((char*)&v21 + v19));
		else
			v18 = sub_70D20(*(x_WORD*)((char*)&v21 + v19));
		if (v18)
			v17 += sub_70EF0(*(x_WORD*)((char*)&v21 + v19));
		++v16;
	}
	return v17;
}

char sub_70E10(unsigned __int16 a1)//251e10
{
	//int v1; // edx
	//__int16 v2; // di
	unsigned __int16 i; // bx
	type_particle_str** v4x; // ecx
	type_animations1* v5x; // eax

	//v1 = 10 * a1;
	//v2 = str_TMAPS00TAB_BEGIN_BUFFER[a1].word_8;
	if (!str_TMAPS00TAB_BEGIN_BUFFER[a1].word_8)
		return 0;
	for (i = str_TMAPS00TAB_BEGIN_BUFFER[a1].word_8; i < 0x1F8u && str_TMAPS00TAB_BEGIN_BUFFER[a1].word_8 == str_TMAPS00TAB_BEGIN_BUFFER[i].word_8; i++)
	{
		v4x = str_DWORD_F66F0x[i];
		if (v4x)
		{
			if ((*v4x)->word_0 & 1)
			{
				v5x = sub_724F0(x_DWORD_E9C08x, i);
				sub_72410(v5x);
			}
			sub_71F20(x_DWORD_E9C28_str, str_F5F10[i]);
			str_DWORD_F66F0x[i] = 0;
			str_F5F10[i] = 0;
			x_DWORD_F5730[i] = 0;
		}
	}
	return 1;
}

char sub_70D20(unsigned __int16 a1)//251d20
{
	//unsigned __int16 v1; // ax
	//__int16 v2; // di
	unsigned __int16 i; // bx
	type_particle_str** v4x; // ecx
	type_animations1* v5; // eax

	//v1 = str_TMAPS00TAB_BEGIN_BUFFER[a1].word_8;
	if (x_BYTE_F5340[str_TMAPS00TAB_BEGIN_BUFFER[a1].word_8])
		return 0;
	//v2 = str_TMAPS00TAB_BEGIN_BUFFER[a1].word_8;
	if (!str_DWORD_F66F0x[str_TMAPS00TAB_BEGIN_BUFFER[a1].word_8])
		return 0;
	for (i = str_TMAPS00TAB_BEGIN_BUFFER[a1].word_8; i < 504 && str_TMAPS00TAB_BEGIN_BUFFER[a1].word_8 == str_TMAPS00TAB_BEGIN_BUFFER[i].word_8; i++)
	{
		v4x = str_DWORD_F66F0x[i];
		if (v4x)
		{
			if ((*v4x)->word_0 & 1)
			{
				v5 = sub_724F0(x_DWORD_E9C08x, i);
				sub_72410(v5);
			}
			sub_71F20(x_DWORD_E9C28_str, str_F5F10[i]);
			str_DWORD_F66F0x[i] = 0;
			str_F5F10[i] = 0;
			x_DWORD_F5730[i] = 0;
		}
	}
	return 1;
}

type_animations1* sub_724F0(type_E9C08* a1x, __int16 a2)
{
	//type_animations1* resultx; // eax
	int resulty;
	//int v3; // edx
	int16_t v3x;
	//HIWORD(v3) = HIWORD(a1);
	//resultx = a1x->dword_2;
	resulty = 0;
	//LOWORD(v3) = *(x_WORD *)a1;
	v3x = a1x->word_0;
	if (!a1x->word_0)
		return 0;
	while (!a1x->dword_2[resulty].Particles_4 || a2 != a1x->dword_2[resulty].word_26)
	{
		--v3x;
		//result += 28;
		//resultx++;
		resulty++;
		if (!v3x)
			return 0;
	}
	return &(a1x->dword_2[resulty]);
}

void sub_71F20(type_x_DWORD_E9C28_str* a1y, subtype_x_DWORD_E9C28_str* a2x)//252f20
{
	int v2x; // eax
	type_particle_str* v2y;
	subtype_x_DWORD_E9C28_str* v3x; // ecx
	unsigned __int16 v4; // bx
	int v5; // esi
	subtype_x_DWORD_E9C28_str* v6x; // ecx
	unsigned __int16 v7; // cx
	//int v8; // edi
	//int v9; // eax
	const void* v10; // esi
	type_particle_str* i; // [esp+4h] [ebp-4h]

	//allert_error();//fix this code
	//a2x->word_10
	//v2x = *(x_WORD*)((int8_t*)a2x + 10);
	if (a2x->word_10 < a1y->word_22)
	{
		//v2 = 14 * (unsigned __int16)v2;
		v3x = &a1y->str_8_data[a2x->word_10];
		if (v3x->dword_4)
		{
			v4 = v3x->word_8;
			v5 = v3x->dword_4 + a1y->dword_4;
			v6x = a1y->str_8_data;
			a1y->dword_4 = v5;
			//*(x_DWORD*)(v6 + v2 + 4) = 0;
			v6x[a2x->word_10].dword_4 = 0;
			//v2 = 14 * (unsigned __int16)v2;
			//v2 = *(x_DWORD*)(a1y->dword_8_data + v2);
			//v2y = a1y->dword_0;
			//v2y = *(_DWORD*)(*(_DWORD*)(a1 + 8) + v2);//a1y->str_8_data
			v2y = a1y->str_8_data[a2x->word_10].partstr_0;
			for (i = v2y; ; i += a1y->dword_12x[v4]->dword_4)
			{
				++v4;
				//v7 = a1y->word_20;
				if (v4 >= a1y->word_20)
					break;
				//v8 = 4 * v4;
				a1y->dword_12x[v4 - 1] = a1y->dword_12x[v4];
				//v9 = a1y->dword_12x[v4];
				a1y->dword_12x[v4]->word_8 = v4 - 1;
				//v10 = *(const void**)a1y->dword_12x[v4];
				a1y->dword_12x[v4]->partstr_0 = i;
				qmemcpy(i, a1y->dword_12x[v4], a1y->dword_12x[v4]->dword_4);

				//v2 = *(_DWORD*)(*(_DWORD*)(4 * v4 + a1y->dword_12x) + 4);

				/*
				v4++;
				v7 = a1y->word_20;
				if (v4 >= v7)
					break;
				//v8 = 4 * v4;
				//*(x_DWORD *)(v8 + a1y->dword_12 - 4) = *(x_DWORD *)(v8 + a1y->dword_12);
				*(x_DWORD*)(v4 + a1y->dword_12x - 1) = *(x_DWORD*)(v4 + a1y->dword_12x);
				//v9 = *(x_DWORD *)(v8 + a1y->dword_12);
				*(x_WORD*)(*(x_DWORD*)(v4 + a1y->dword_12x) + 8) = v4 - 1;
				v10 = *(const void**)*(x_DWORD*)(v4 + a1y->dword_12x);
#ifdef TEST_x64
	allert_error();
#endif
#ifdef COMPILE_FOR_64BIT // FIXME: 64bit
  std::cout << "FIXME: 64bit @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
				**(x_DWORD**)(v4 + a1y->dword_12x) = (x_DWORD)i;
#endif
				qmemcpy(i, (void*)v10, *(x_DWORD*)(*(x_DWORD*)(v4 + a1y->dword_12x) + 4));
				//v2 = *(x_DWORD*)(*(x_DWORD*)(v4 + a1y->dword_12x) + 4);*/
			}
			//a1y->word_20 = v7 - 1;
			a1y->word_20--;
		}
	}
	//return v2;
}

void InitTmaps(unsigned __int16 a1)//251f50
{
	type_animations1* index; // eax
	//uint8_t* index2; // eax
	subtype_x_DWORD_E9C28_str* index3x; // eax
	type_particle_str* index5x; // eax
	type_particle_str** index6x; // eax
	unsigned __int16 v2; // bx
	unsigned __int16 i; // si
	//uint8_t* v4; // edi
	int v5; // [esp+0h] [ebp-Ch]
	int v6; // [esp+8h] [ebp-4h]
	char tmapsdirpost[512];

	if (bigSprites)
	{
		if (!big_sprites_inited)
		{
			for (int i = 0; i < max_sprites; i++)
			{
				BIG_SPRITES_BUFFERx[i].actdatax = NULL;
				for (int j = 0; j < max_sprites_frames; j++)
				{
					BIG_SPRITES_BUFFERx[i].frames[j] = NULL;
				}
			}
			big_sprites_inited = true;
		}

		char spritePath[512];
		if (big_sprites_inited)
		{
			sprintf(spritePath, "%s", bigGraphicsPath);
		}
		else
		{
			sprintf(spritePath, "%s", gameDataPath);
		}

		switch (D41A0_0.terrain_2FECE.MapType) {
		case MapType_t::Day: {
			sprintf(tmapsdirpost, "%s/%s", spritePath, "TMAPS/TMAPS2-0-");
			break;
		}
		case MapType_t::Night: {
			sprintf(tmapsdirpost, "%s/%s", spritePath, "TMAPS/TMAPS2-1-");
			break;
		}
		case MapType_t::Cave: {
			sprintf(tmapsdirpost, "%s/%s", spritePath, "TMAPS/TMAPS2-2-");
			break;
		}
		}
	}

	v5 = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
	//index = (int)TMAPS00TAB_BEGIN_BUFFER;
	//str_TMAPS00TAB_BEGIN_BUFFER[a1].word_8
	v2 = str_TMAPS00TAB_BEGIN_BUFFER[a1].word_8;
	for (i = str_TMAPS00TAB_BEGIN_BUFFER[a1].word_8; i < 504; i++)
	{
		//index2 = 10 * i + TMAPS00TAB_BEGIN_BUFFER;
		if (v2 != str_TMAPS00TAB_BEGIN_BUFFER[i].word_8)
			break;
		v6 = i;
		if (!str_DWORD_F66F0x[i])
		{
			index3x = LoadTMapMetadata_71E70(x_DWORD_E9C28_str, (unsigned __int16)(4 * ((unsigned int)(str_TMAPS00TAB_BEGIN_BUFFER[i].dword_0 + 13) >> 2)), i);
			//v4 = index3;
			if (index3x)
			{
				int index4 = sub_70C60_decompress_tmap(i, *(uint8_t**)index3x);
				if (index4 != -1)
				{
					if (bigSprites)
					{
						uint8_t* oldtmap = *(uint8_t**)index3x;
						int oldwidth = *(uint16_t*)(oldtmap + 2);
						int oldheight = *(uint16_t*)(oldtmap + 4);
						if ((oldwidth > 0) && (oldwidth < 0x200) && (oldheight > 0) && (oldheight < 0x200))
						{
							int actnumber = 0;
							if (BIG_SPRITES_BUFFERx[i].actdatax != NULL)
							{
								free(BIG_SPRITES_BUFFERx[i].actdatax);
								BIG_SPRITES_BUFFERx[i].actdatax = NULL;
							}
							for (int mm = 0; mm < max_sprites_frames; mm++)
								if (BIG_SPRITES_BUFFERx[i].frames[mm] != NULL)
								{
									free(BIG_SPRITES_BUFFERx[i].frames[mm]);
									BIG_SPRITES_BUFFERx[i].frames[mm] = NULL;
								};
							int mm;
							for (mm = 0; mm < max_sprites_frames; mm++)
							{
								char filebuffer[512];
								//FILE* fptr_outdata;
								sprintf(filebuffer, "%s%03d-%02d.data", tmapsdirpost, i, mm);
								//if (!fix_file_exists(filebuffer))
								//	break;
								if (!ExistGraphicsfile(filebuffer))
									break;
								//fptr_outdata= myopent(filebuffer, (char*)"rb");
								BIG_SPRITES_BUFFERx[i].frames[mm] = (uint8_t*)malloc(oldwidth * 4 * oldheight * 4);
								//fread(BIG_SPRITES_BUFFERx[i].frames[mm], oldwidth * 4 * oldheight * 4, 1, fptr_outdata);
								//myclose(fptr_outdata);
								ReadGraphicsfile(filebuffer, BIG_SPRITES_BUFFERx[i].frames[mm], oldwidth * 4 * oldheight * 4);
							}

							BIG_SPRITES_BUFFERx[i].actdatax = (type_particle_str*)malloc(oldwidth * 4 * oldheight * 4 + 6 + 2);
							memcpy(BIG_SPRITES_BUFFERx[i].actdatax->textureBuffer, BIG_SPRITES_BUFFERx[i].frames[0], oldwidth * 4 * oldheight * 4);

							BIG_SPRITES_BUFFERx[i].actdatax->word_0 = *(uint16_t*)oldtmap;
							BIG_SPRITES_BUFFERx[i].actdatax->width = oldwidth * 4;
							BIG_SPRITES_BUFFERx[i].actdatax->height = oldheight * 4;
							*(uint16_t*)&BIG_SPRITES_BUFFERx[i].actdatax->textureBuffer[oldwidth * 4 * oldheight * 4] = mm;

							/*for (int xx = 0; xx < oldwidth*4; xx++)
								for (int yy = 0; yy < oldheight*4; yy++)
									* (uint16_t*)(BIG_SPRITES_BUFFER[i] + 6+ yy * oldwidth + xx) = 128;*/
							index3x->partstr_0 = BIG_SPRITES_BUFFERx[i].actdatax;
						}
					}
					str_F5F10[v6] = index3x;
					str_DWORD_F66F0x[v6] = &index3x->partstr_0;
					x_DWORD_F5730[v6] = v5;
					index6x = str_DWORD_F66F0x[v6];
					//if (**(uint8_t**)index6 & 1)
					if ((*index6x)->word_0 & 1)
						index = sub_721C0_initTmap(x_DWORD_E9C08x, index6x, i);
					if (v2 < 480)
					{
						if (v2 != 311)
							continue;
						index5x = *str_DWORD_F66F0x[i];
						index5x->word_0 |= 0x20u;
						continue;
					}
					if (v2 <= 480 || v2 >= 488 && (v2 <= 488 || v2 == 496))
					{
						index5x = *str_DWORD_F66F0x[i];
						index5x->word_0 |= 0x20u;
						continue;
					}
				}
			}
		}
	}
}

subtype_x_DWORD_E9C28_str* LoadTMapMetadata_71E70(type_x_DWORD_E9C28_str* a1y, unsigned int a2, __int16 a3)//252e70
{
	signed __int16 v3; // si
	signed __int16 v4; // ax
	signed __int16 v5; // dx
	//int v6; // ecx
	//int v7; // eax
	subtype_x_DWORD_E9C28_str* result; // eax

	v3 = -1;
	if (a2 < a1y->dword_4)
	{
		v4 = sub_71CD0(a1y);
		v5 = v4;
		//v6 = v4;
		v3 = v4;
		if (v4 > -1)
		{
			//v7 = 14 * v4;
			/*
			*(x_WORD*)(a1y->dword_8_data + v7 + 10) = v5;
			*(x_DWORD*)(a1y->dword_8_data + v7 + 4) = a2;
#ifdef TEST_x64
	allert_error();
#endif
#ifdef COMPILE_FOR_64BIT // FIXME: 64bit
  std::cout << "FIXME: 64bit @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
			*(x_DWORD*)(a1y->dword_8_data + v7) = a1y->dword_0 + (int)a1y->dword_16x - a1y->dword_4;
#endif
			*(x_WORD*)(a1y->dword_8_data + v7 + 12) = a3;
			a1y->dword_4 -= a2;
			*(x_WORD*)(a1y->dword_8_data + v7 + 8) = a1y->word_20;
#ifdef TEST_x64
	allert_error();
#endif
#ifdef COMPILE_FOR_64BIT // FIXME: 64bit
  std::cout << "FIXME: 64bit @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
			*(x_DWORD*)(a1y->dword_12x + (unsigned __int16)(a1y->word_20)++) = (uint32_t)a1y->dword_8_data + 14 * v6;
#endif
*/
			a1y->str_8_data[v4].word_10 = v5;
			a1y->str_8_data[v4].dword_4 = a2;
			a1y->str_8_data[v4].partstr_0 = (type_particle_str*)(a1y->dword_16x + (a1y->dword_0 - a1y->dword_4));
			a1y->str_8_data[v4].word_12 = a3;
			a1y->dword_4 -= a2;
			a1y->str_8_data[v4].word_8 = a1y->word_20;
			//*(x_DWORD*)(a1y->dword_12x + (unsigned __int16)(a1y->word_20)++) = (uint32_t)&a1y->str_8_data[v4];
			a1y->dword_12x[a1y->word_20++] = &a1y->str_8_data[v4];
			//allert_error();//for 64x fix
			//it is must rewrite
		}
	}
	if (v3 <= -1)
		result = 0;
	else
		//result = 14 * v3 + a1y->dword_8_data;
		result = &a1y->str_8_data[v4];
	return result;
}

void sub_70A60_open_tmaps()//251a60
{
	//char printbuffer[512];//char v1; // [esp+0h] [ebp-40h]

	GetSubDirectoryFile(printbuffer, gameFolder, "CDATA", "TMAPS0-0.DAT");
	x_DWORD_DB740_tmaps00file = DataFileIO::CreateOrOpenFile(printbuffer, 512);
	if (x_DWORD_DB740_tmaps00file == NULL)
	{
		GetSubDirectoryFile(printbuffer, gameFolder, "DATA", "TMAPS0-0.DAT");
		x_DWORD_DB740_tmaps00file = DataFileIO::CreateOrOpenFile(printbuffer, 512);
	}
	GetSubDirectoryFile(printbuffer, gameFolder, "CDATA", "TMAPS1-0.DAT");
	x_DWORD_DB744_tmaps10file = DataFileIO::CreateOrOpenFile(printbuffer, 512);
	if (x_DWORD_DB744_tmaps10file == NULL)
	{
		GetSubDirectoryFile(printbuffer, gameFolder, "DATA", "TMAPS1-0.DAT");
		x_DWORD_DB744_tmaps10file = DataFileIO::CreateOrOpenFile(printbuffer, 512);
	}
	GetSubDirectoryFile(printbuffer, gameFolder, "CDATA", "TMAPS2-0.DAT");
	x_DWORD_DB748_tmaps20file = DataFileIO::CreateOrOpenFile(printbuffer, 512);
	if (x_DWORD_DB748_tmaps20file == NULL)
	{
		GetSubDirectoryFile(printbuffer, gameFolder, "DATA", "TMAPS2-0.DAT");
		x_DWORD_DB748_tmaps20file = DataFileIO::CreateOrOpenFile(printbuffer, 512);
	}
	x_DWORD_DB73C_tmapsfile = x_DWORD_DB740_tmaps00file;
	//return 1;
}

void sub_70BF0_close_tmaps()//251bf0
{
	//int result; // eax

	if (x_DWORD_DB740_tmaps00file != NULL)
	{
		DataFileIO::Close(x_DWORD_DB740_tmaps00file);
		x_DWORD_DB740_tmaps00file = NULL;
	}
	if (x_DWORD_DB744_tmaps10file != NULL)
	{
		DataFileIO::Close(x_DWORD_DB744_tmaps10file);
		x_DWORD_DB744_tmaps10file = NULL;
	}
	if (x_DWORD_DB748_tmaps20file != NULL)
	{
		DataFileIO::Close(x_DWORD_DB748_tmaps20file);
		x_DWORD_DB748_tmaps20file = NULL;
	}
	x_DWORD_DB73C_tmapsfile = NULL;
	//return result;
}

int sub_70C60_decompress_tmap(uint16_t texture_index, uint8_t* texture_buffer)//251c60
{
	int result; // eax

	if (x_DWORD_DB73C_tmapsfile == NULL) {
		return 0; //(int)x_DWORD_DB73C_tmapsfile;
	}

	DataFileIO::Seek(x_DWORD_DB73C_tmapsfile, str_TMAPS00TAB_BEGIN_BUFFER[texture_index].dword_4, 0);//lseek
	int v3 = str_TMAPS00TAB_BEGIN_BUFFER[texture_index + 1].dword_4 - str_TMAPS00TAB_BEGIN_BUFFER[texture_index].dword_4;
	if (DataFileIO::Read(x_DWORD_DB73C_tmapsfile, texture_buffer, v3) != v3)
		return -1;
	result = DataFileIO::Decompress(texture_buffer, texture_buffer);
	if (result >= 0)
	{
		if (!result)
			result = v3;
	}
	else
	{
		myprintf("ERROR decompressing tmap%03d\n");
		result = -2;
	}
	return result;
}

void WriteTextureMapToBmp(uint16_t texture_index, type_particle_str* ptextureMap, MapType_t mapType)
{
	char path[MAX_PATH];
	char name[50];
	GetSubDirectoryPath(path, "BufferOut");
	if (myaccess(path, 0) < 0)
	{
		mymkdir(path);
	}

	if (m_pColorPalette == NULL)
	{
		m_pColorPalette = LoadTMapColorPalette(mapType);
		GetSubDirectoryPath(path, "BufferOut/PalletOut.bmp");
		BitmapIO::WritePaletteAsImageBMP(path, 256, m_pColorPalette);
	}

	sprintf(name, "BufferOut/TmapOut%03d%s", texture_index, ".bmp");
	GetSubDirectoryPath(path, name);
	BitmapIO::WriteRGBAImageBufferAsImageBMP(path, ptextureMap->width, ptextureMap->height, m_pColorPalette, (uint8_t*)ptextureMap->textureBuffer);
}

uint8_t* LoadTMapColorPalette(MapType_t mapType)
{
	uint8_t* pallettebuffer = new uint8_t[768];
	FILE* palfile;
	char path[MAX_PATH];
	char palleteName[50];

	switch (mapType)
	{
		case MapType_t::Cave:
			sprintf(palleteName, "../tools/palletelight/Debug/out-%s.pal", "c");
		break;
		case MapType_t::Day:
			sprintf(palleteName, "../tools/palletelight/Debug/out-%s.pal", "block");
		break;
		case MapType_t::Night:
			sprintf(palleteName, "../tools/palletelight/Debug/out-%s.pal", "n");
		break;
	}

	GetSubDirectoryPath(path, palleteName);
	palfile = fopen(path, "rb");
	fread(pallettebuffer, 768, 1, palfile);
	fclose(palfile);

	return pallettebuffer;
}

type_animations1* sub_721C0_initTmap(type_E9C08* a1x, type_particle_str** a2x, __int16 a3)//2531c0
{
	signed __int16 v3; // cx
	signed __int16 v4; // si
	signed __int16 i; // bx
	//x_DWORD *v6; // edx
	type_animations1* v6x;
	type_particle_str* v7x; // ebx
	int v8; // ecx
	__int16 v9; // ST08_2
	//int v10; // edx
	signed __int16 v12; // [esp+Ch] [ebp-4h]

	v3 = -1;
	v4 = -1;
	if (!(a1x->word_0))
		return 0;
	for (i = 0; i < a1x->word_0; i++)
	{
		v6x = &a1x->dword_2[i];
		if (v6x->Particles_4)
		{
			if (!v6x->dword_0)
				v4 = i;
		}
		else
		{
			v3 = i;
		}
	}
	v12 = v3 <= 0 ? v4 : v3;
	if (v12 <= -1)
		return 0;
	v7x = *a2x;
	v8 = (*a2x)->height * (*a2x)->width;
	//v9 = *(x_WORD*)(v8 + (*a2x)->un_0.byte[0] + 6);//? is ok
	v9 = ((*a2x)->textureBuffer)[v8];//? is ok
	//v10 = 28 * v12;
	a1x->dword_2[v12].Particles_4 = *a2x;
	a1x->dword_2[v12].word_12 = 6;
	a1x->dword_2[v12].word_14 = v8 + 6;
	a1x->dword_2[v12].CountOfFrames_16 = v9;
	a1x->dword_2[v12].Width_18 = v7x->width;
	a1x->dword_2[v12].Height_20 = v7x->height;
	a1x->dword_2[v12].dword_8 = v8 + 6;
	a1x->dword_2[v12].FrameIndex_22 = 1;
	a1x->dword_2[v12].dword_0 = 1;
	a1x->dword_2[v12].word_24 = v12;
	a1x->dword_2[v12].word_26 = a3;
	//return v10 + a1x->dword_2;
	return &a1x->dword_2[v12];
}

void sub_72410(type_animations1* a1)
{
	//x_DWORD* result; // eax

	//result = a1;
	if (a1)
	{
		a1->dword_0 = 0;
		a1->Particles_4 = 0;
		/*a1[0] = 0;
		a1[1] = 0;*/
	}
	//return result;
}