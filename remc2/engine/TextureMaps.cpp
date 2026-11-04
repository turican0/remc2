#include "TextureMaps.h"

#include <filesystem>
#include <vector>
#include <map>
#include <tuple>
#include <algorithm>
#include "../utilities/DataFileRNC.h"
#include "../utilities/BitmapIO.h"

type_BIG_SPRITES_BUFFER BIG_SPRITES_BUFFERx[max_sprites];

FILE* tmapsFile_DB73C;
FILE* tmaps0File_DB740;
FILE* tmaps1File_DB744;
FILE* tmaps2File_DB748;

Type_Sprite** m_ptrLoadedSprites_F66F0x[504];
char m_LevelSpriteList_F5340[504];
int32_t tmapGroupLastUse_F5730[504];
subtype_x_DWORD_E9C28_str* str_F5F10[504];

type_x_DWORD_E9C28_str* tmapCache_E9C28;

type_E9C08* animations_E9C08x; // weak
bool big_sprites_inited = false;
uint8_t* m_pColorPalette = NULL;

// Sprites with an error in the original data: DecompressTmap_70C60 takes them from
// graphics/fixed/tmaps/TMAPS2-<map type>-<index>.data instead of the TMAPS file, the decompressed sprite
// (u16 word_0, u16 width, u16 height, pixels). Day and cave 452 is a 320x200 canvas with the figure in the middle,
// its neighbours 444-451 are cropped to the figure. Only drawing reads the size, the game state does not depend on it.
// Read with the TMAPS files (OpenTmaps_70A60).
MapType_t tmapsMapType = MapType_t::Day;
static std::vector<uint8_t> fixedSprites[3][504];//[map type][sprite]

static void LoadFixedTmaps()
{
	for (auto& type : fixedSprites)
		for (auto& sprite : type)
			sprite.clear();
	const std::string folder = GetSubDirectoryPath(fixedTmapsFolder.c_str());
	std::error_code error;
	if (folder.empty())
		return;
	for (const auto& entry : std::filesystem::directory_iterator(folder, error))
	{
		int type, index;
		if (sscanf(entry.path().filename().string().c_str(), "TMAPS2-%d-%d.data", &type, &index) != 2 || type < 0 || type > 2 || index < 0 || index >= 504)
			continue;
		FILE* file = fopen(entry.path().string().c_str(), "rb");
		if (!file)
			continue;
		std::vector<uint8_t>& data = fixedSprites[type][index];
		data.resize((size_t)std::filesystem::file_size(entry.path(), error));
		if (fread(data.data(), 1, data.size(), file) != data.size() || data.size() < 6)
			data.clear();
		fclose(file);
	}
}

bool MainInitTmaps_71520(unsigned __int16 a1)
{
	int v1; // esi
	int v2; // ebx
	signed int i; // ebx
	unsigned __int16 v4; // ax

	v1 = 0;
	v2 = GetTmapGroupSize_70EF0(a1);
	for (i = v2 - GetTmapCacheFreeSize_71E60(tmapCache_E9C28) + 20; i > 0; i -= EvictLeastUsedTmaps_71090(i))
	{
		v4 = v1++;
		if (v4 >= 4u)
			break;
	}
	if (i <= 0)
	{
		InitTmaps(a1);
		gameState_D41A4.byteindex_177 = 5;
	}
	return m_ptrLoadedSprites_F66F0x[a1] != 0;
}

int GetTmapGroupSize_70EF0(unsigned __int16 a1)//251ef0
{
	unsigned __int16 v1; // dx
	int i; // ebx
	//int v3; // ecx
	v1 = str_TMAPS00TAB_BEGIN_BUFFER[a1].group_8;
	for (i = 0; str_TMAPS00TAB_BEGIN_BUFFER[a1].group_8 < 504; v1++)
	{
		//v3 = 10 * v1;
		if (str_TMAPS00TAB_BEGIN_BUFFER[a1].group_8 != str_TMAPS00TAB_BEGIN_BUFFER[v1].group_8)
			break;
		i += str_TMAPS00TAB_BEGIN_BUFFER[v1].group_8;
	}
	return i;
}

int GetTmapCacheFreeSize_71E60(type_x_DWORD_E9C28_str* a1y)//252e60
{
	return a1y->freeSize_4;
}

signed int GetIndex_71CD0(type_x_DWORD_E9C28_str* a1y)//252cd0
{
	int i; // edx

	for (i = 0; (signed __int16)i < (signed int)a1y->maxEntries_22; i++)
	{
		//if (!*(x_DWORD*)(14 * (signed __int16)i + a1y->dword_8_data + 4))
		if (!a1y->entries_8[i].size_4)
			return i;
	}
	return -1;
}

unsigned int EvictLeastUsedTmaps_71090(unsigned int a1)//252090
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
		v4 = str_TMAPS00TAB_BEGIN_BUFFER[v3 + 1].group_8;
		if (m_ptrLoadedSprites_F66F0x[v4] && !m_LevelSpriteList_F5340[v4])
			v31 = 0;
		while ((unsigned __int16)v3 < 0x1F8u
			&& str_TMAPS00TAB_BEGIN_BUFFER[v3 + 1].group_8 == v4)
			++v3;
		++v3;
	} while ((unsigned __int16)v3 < 0x1F8u);
	v5 = 0;
	do
	{
		v6 = str_TMAPS00TAB_BEGIN_BUFFER[v5].group_8;
		if ((!m_LevelSpriteList_F5340[v6] || v31) && m_ptrLoadedSprites_F66F0x[v6])
		{
			v7 = tmapGroupLastUse_F5730[v6];
			v8 = str_TMAPS00TAB_BEGIN_BUFFER[v5].group_8;
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
			&& str_TMAPS00TAB_BEGIN_BUFFER[v5 + 1].group_8 == v6)
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
			v18 = UnloadTmapGroup_70E10(*(x_WORD*)((char*)&v21 + v19));
		else
			v18 = ResetTmap_70D20(*(x_WORD*)((char*)&v21 + v19));
		if (v18)
			v17 += GetTmapGroupSize_70EF0(*(x_WORD*)((char*)&v21 + v19));
		++v16;
	}
	return v17;
}

char UnloadTmapGroup_70E10(unsigned __int16 a1)//251e10
{
	//int v1; // edx
	//__int16 v2; // di
	unsigned __int16 i; // bx
	Type_Sprite** v4x; // ecx
	type_animations1* v5x; // eax

	//v1 = 10 * a1;
	//v2 = str_TMAPS00TAB_BEGIN_BUFFER[a1].word_8;
	if (!str_TMAPS00TAB_BEGIN_BUFFER[a1].group_8)
		return 0;
	for (i = str_TMAPS00TAB_BEGIN_BUFFER[a1].group_8; i < 0x1F8u && str_TMAPS00TAB_BEGIN_BUFFER[a1].group_8 == str_TMAPS00TAB_BEGIN_BUFFER[i].group_8; i++)
	{
		v4x = m_ptrLoadedSprites_F66F0x[i];
		if (v4x)
		{
			if ((*v4x)->flags_0 & 1)
			{
				v5x = GetAnimationByIndex_724F0(animations_E9C08x, i);
				ResetAnimation_72410(v5x);
			}
			FreeTmapCacheEntry_71F20(tmapCache_E9C28, str_F5F10[i]);
			m_ptrLoadedSprites_F66F0x[i] = 0;
			str_F5F10[i] = 0;
			tmapGroupLastUse_F5730[i] = 0;
		}
	}
	return 1;
}

char ResetTmap_70D20(uint16 tmapsIndex)//251d20
{
	if (m_LevelSpriteList_F5340[str_TMAPS00TAB_BEGIN_BUFFER[tmapsIndex].group_8])
		return 0;
	if (!m_ptrLoadedSprites_F66F0x[str_TMAPS00TAB_BEGIN_BUFFER[tmapsIndex].group_8])
		return 0;
	for (uint16 i = str_TMAPS00TAB_BEGIN_BUFFER[tmapsIndex].group_8; i < 504 && str_TMAPS00TAB_BEGIN_BUFFER[tmapsIndex].group_8 == str_TMAPS00TAB_BEGIN_BUFFER[i].group_8; i++)
	{
		Type_Sprite** particle = m_ptrLoadedSprites_F66F0x[i];
		if (particle)
		{
			if ((*particle)->flags_0 & 1)
			{
				ResetAnimation_72410(GetAnimationByIndex_724F0(animations_E9C08x, i));
			}
			FreeTmapCacheEntry_71F20(tmapCache_E9C28, str_F5F10[i]);
			m_ptrLoadedSprites_F66F0x[i] = 0;
			str_F5F10[i] = 0;
			tmapGroupLastUse_F5730[i] = 0;
		}
	}
	return 1;
}

type_animations1* GetAnimationByIndex_724F0(type_E9C08* animations, __int16 index)
{
	int resulty = 0;
	int16_t v3x = animations->count_0;
	if (!animations->count_0)
		return 0;
	while (!animations->animations_2[resulty].Sprite_4 || index != animations->animations_2[resulty].spriteIndex_26)
	{
		--v3x;
		resulty++;
		if (!v3x)
			return 0;
	}
	return &(animations->animations_2[resulty]);
}

void FreeTmapCacheEntry_71F20(type_x_DWORD_E9C28_str* a1y, subtype_x_DWORD_E9C28_str* a2x)//252f20
{
	Type_Sprite* v2y;
	subtype_x_DWORD_E9C28_str* v3x; // ecx
	unsigned __int16 v4; // bx
	int v5; // esi
	subtype_x_DWORD_E9C28_str* v6x; // ecx
	Type_Sprite* i; // [esp+4h] [ebp-4h]

	if (a2x->Index < a1y->maxEntries_22)
	{
		v3x = &a1y->entries_8[a2x->Index];
		if (v3x->size_4)
		{
			v4 = v3x->slotIndex_8;
			v5 = v3x->size_4 + a1y->freeSize_4;
			v6x = a1y->entries_8;
			a1y->freeSize_4 = v5;
			v6x[a2x->Index].size_4 = 0;
			v2y = a1y->entries_8[a2x->Index].partstr_0;
			for (i = v2y; ; i += a1y->entryPtrs_12x[v4]->size_4)
			{
				++v4;
				if (v4 >= a1y->usedCount_20)
					break;
				a1y->entryPtrs_12x[v4 - 1] = a1y->entryPtrs_12x[v4];
				a1y->entryPtrs_12x[v4]->slotIndex_8 = v4 - 1;
				a1y->entryPtrs_12x[v4]->partstr_0 = i;
				//qmemcpy(i, a1y->dword_12x[v4], a1y->dword_12x[v4]->dword_4);

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
			a1y->usedCount_20--;
		}
	}
	//return v2;
}

void InitTmaps(unsigned __int16 a1)//251f50
{
	type_animations1* index; // eax
	//uint8_t* index2; // eax
	subtype_x_DWORD_E9C28_str* index3x; // eax
	Type_Sprite* index5x; // eax
	Type_Sprite** index6x; // eax
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
			sprintf(spritePath, "%s", highResGraphicsPath.c_str());
		}
		else
		{
			sprintf(spritePath, "%s", gameDataPath.c_str());
		}

		switch (world_D41A0.terrain_2FECE.MapType) {
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

	v5 = gameState_D41A4.FrameTimingIndex_26;
	//index = (int)TMAPS00TAB_BEGIN_BUFFER;
	//str_TMAPS00TAB_BEGIN_BUFFER[a1].word_8
	v2 = str_TMAPS00TAB_BEGIN_BUFFER[a1].group_8;
	for (i = str_TMAPS00TAB_BEGIN_BUFFER[a1].group_8; i < 504; i++)
	{
		//index2 = 10 * i + TMAPS00TAB_BEGIN_BUFFER;
		if (v2 != str_TMAPS00TAB_BEGIN_BUFFER[i].group_8)
			break;
		v6 = i;
		if (!m_ptrLoadedSprites_F66F0x[i])
		{
			index3x = LoadTMapMetadata_71E70(tmapCache_E9C28, (unsigned __int16)(4 * ((unsigned int)(str_TMAPS00TAB_BEGIN_BUFFER[i].size_0 + 13) >> 2)), i);
			//v4 = index3;
			if (index3x)
			{
				int index4 = DecompressTmap_70C60(i, (uint8_t*)index3x->partstr_0);
				if (index4 != -1)
				{
					if (bigSprites)
					{
						Type_Sprite* oldtmapx = index3x->partstr_0;
						int oldwidth = oldtmapx->width;
						int oldheight = oldtmapx->height;
						if ((oldwidth > 0) && (oldwidth < 512) && (oldheight > 0) && (oldheight < 512))
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

								//Saves BigSprites to png
								/*if (m_pColorPalette == NULL)
								{
									m_pColorPalette = LoadTMapColorPalette(world_D41A0.terrain_2FECE.MapType);
								}
								BitmapIO::WritePosistructToPng(m_pColorPalette, BIG_SPRITES_BUFFERx[i].frames[mm], oldwidth * 4, oldheight * 4, filebuffer, filebuffer);*/
							}

							BIG_SPRITES_BUFFERx[i].actdatax = (Type_Sprite*)malloc(oldwidth * 4 * oldheight * 4 + 6 + 2);
							memcpy(BIG_SPRITES_BUFFERx[i].actdatax->textureBuffer, BIG_SPRITES_BUFFERx[i].frames[0], oldwidth * 4 * oldheight * 4);

							BIG_SPRITES_BUFFERx[i].actdatax->flags_0 = oldtmapx->flags_0;
							BIG_SPRITES_BUFFERx[i].actdatax->width = oldwidth * 4;
							BIG_SPRITES_BUFFERx[i].actdatax->height = oldheight * 4;
							*(uint16_t*)&BIG_SPRITES_BUFFERx[i].actdatax->textureBuffer[oldwidth * 4 * oldheight * 4] = mm;

							Logger->trace("BIG_SPRITES_BUFFERx[i].actdatax->word_0: {}", BIG_SPRITES_BUFFERx[i].actdatax->flags_0);
							Logger->trace("BIG_SPRITES_BUFFERx[i].actdatax->width: {}", BIG_SPRITES_BUFFERx[i].actdatax->width);
							Logger->trace("BIG_SPRITES_BUFFERx[i].actdatax->height: {}", BIG_SPRITES_BUFFERx[i].actdatax->height);
							Logger->trace("BIG_SPRITES_BUFFERx[i].actdatax frames: {}", BIG_SPRITES_BUFFERx[i].actdatax->textureBuffer[oldwidth * 4 * oldheight * 4]);
							int8_t* buffer = BIG_SPRITES_BUFFERx[i].actdatax->textureBuffer;
							Logger->trace("BIG_SPRITES_BUFFERx[i].actdatax->texture buffer start: {}", fmt::ptr(buffer));
							Logger->trace("BIG_SPRITES_BUFFERx[i].actdatax->texture buffer end: {}", fmt::ptr(buffer + (oldwidth * 4 * oldheight * 4 + 6 + 2)));

							/*for (int xx = 0; xx < oldwidth*4; xx++)
								for (int yy = 0; yy < oldheight*4; yy++)
									* (uint16_t*)(BIG_SPRITES_BUFFER[i] + 6+ yy * oldwidth + xx) = 128;*/
							index3x->partstr_0 = BIG_SPRITES_BUFFERx[i].actdatax;
						}
					}
					str_F5F10[v6] = index3x;
					m_ptrLoadedSprites_F66F0x[v6] = &index3x->partstr_0;
					tmapGroupLastUse_F5730[v6] = v5;
					index6x = m_ptrLoadedSprites_F66F0x[v6];

					//if (**(uint8_t**)index6 & 1)
					if ((*index6x)->flags_0 & 1)
						index = InitTmapAnimation_721C0(animations_E9C08x, index6x, i);
					else if ((*index6x)->height > 512 || (*index6x)->width == 0 || (*index6x)->height == 0)
					{
						//Error
						Logger->error("Bad Texture Data!");
					}
					else
					{
						m_spriteFrameCount[i] = std::max<uint16_t>(1, 1);
						EventDispatcher::I->DispatchEvent<ResourceType, uint32_t, uint8_t*, uint32_t, uint32_t>(
							EventType::E_RESOURCE_CHANGE, ResourceType::TEXTURE_LOADED, i, (uint8_t*)(*index6x)->textureBuffer, (*index6x)->width, (*index6x)->height);
					}

					if (v2 < 480)
					{
						if (v2 != 311)
							continue;
						index5x = *m_ptrLoadedSprites_F66F0x[i];
						index5x->flags_0 |= 0x20u;
						continue;
					}
					if (v2 <= 480 || v2 >= 488 && (v2 <= 488 || v2 == 496))
					{
						index5x = *m_ptrLoadedSprites_F66F0x[i];
						index5x->flags_0 |= 0x20u;
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
	signed __int16 idx; // ax
	subtype_x_DWORD_E9C28_str* result; // eax

	v3 = -1;
	if (a2 < a1y->freeSize_4)
	{
		idx = GetIndex_71CD0(a1y);
		v3 = idx;
		if (idx > -1)
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
			a1y->entries_8[idx].Index = idx;
			a1y->entries_8[idx].size_4 = a2;
			a1y->entries_8[idx].partstr_0 = (Type_Sprite*)(a1y->buffer_16x + (a1y->capacity_0 - a1y->freeSize_4));
			a1y->entries_8[idx].tmapIndex_12 = a3;
			a1y->freeSize_4 -= a2;
			a1y->entries_8[idx].slotIndex_8 = a1y->usedCount_20;
			//*(x_DWORD*)(a1y->dword_12x + (unsigned __int16)(a1y->word_20)++) = (uint32_t)&a1y->str_8_data[v4];
			a1y->entryPtrs_12x[a1y->usedCount_20++] = &a1y->entries_8[idx];
			//allert_error();//for 64x fix
			//it is must rewrite
		}
	}
	if (v3 <= -1)
		result = 0;
	else
		result = &a1y->entries_8[v3];
	return result;
}

void OpenTmaps_70A60()//251a60
{

	std::string tMapPath0 = GetSubDirectoryFile(gameFolder.c_str(), "CDATA", "TMAPS0-0.DAT");
	tmaps0File_DB740 = DataFileIO::CreateOrOpenFile(tMapPath0.c_str(), 512);
	if (tmaps0File_DB740 == NULL)
	{
		tMapPath0 = GetSubDirectoryFile(gameFolder.c_str(), "DATA", "TMAPS0-0.DAT");
		tmaps0File_DB740 = DataFileIO::CreateOrOpenFile(tMapPath0.c_str(), 512);
	}
	std::string tMapPath1 = GetSubDirectoryFile(gameFolder.c_str(), "CDATA", "TMAPS1-0.DAT");
	tmaps1File_DB744 = DataFileIO::CreateOrOpenFile(tMapPath1.c_str(), 512);
	if (tmaps1File_DB744 == NULL)
	{
		tMapPath1 = GetSubDirectoryFile(gameFolder.c_str(), "DATA", "TMAPS1-0.DAT");
		tmaps1File_DB744 = DataFileIO::CreateOrOpenFile(tMapPath1.c_str(), 512);
	}
	std::string tMapPath2 = GetSubDirectoryFile(gameFolder.c_str(), "CDATA", "TMAPS2-0.DAT");
	tmaps2File_DB748 = DataFileIO::CreateOrOpenFile(tMapPath2.c_str(), 512);
	if (tmaps2File_DB748 == NULL)
	{
		tMapPath2 = GetSubDirectoryFile(gameFolder.c_str(), "DATA", "TMAPS2-0.DAT");
		tmaps2File_DB748 = DataFileIO::CreateOrOpenFile(tMapPath2.c_str(), 512);
	}
	tmapsFile_DB73C = tmaps0File_DB740;
	tmapsMapType = MapType_t::Day;
	
	if (fixedTmaps)
		LoadFixedTmaps();

	//return 1;
}

void CloseTmaps_70BF0()//251bf0
{
	//int result; // eax

	if (tmaps0File_DB740 != NULL)
	{
		DataFileIO::Close(tmaps0File_DB740);
		tmaps0File_DB740 = NULL;
	}
	if (tmaps1File_DB744 != NULL)
	{
		DataFileIO::Close(tmaps1File_DB744);
		tmaps1File_DB744 = NULL;
	}
	if (tmaps2File_DB748 != NULL)
	{
		DataFileIO::Close(tmaps2File_DB748);
		tmaps2File_DB748 = NULL;
	}
	tmapsFile_DB73C = NULL;
	//return result;
}

// Regression tests without the game (GitHub Actions): data.binz has no TMAPS?-0.DAT, but the simulation reads the
// sprites - whether one is loaded and its frame count (CountOfFrames_16 of sub_721C0, e.g. SetEntitySpriteAndLoad_221F0 dword_0x10_16).
// sub_70C60 reads the DAT of tmapsMapType at the offset and length of the TAB loaded now, which can be the TAB of
// another map type (a day level loads night sprites with TMAPS0-0.TAB), so TMAPSMETA.DAT ("MC2TMAP2") keeps every
// (DAT, offset, length) of the three TABs: u32 records; per record u8 DAT 0-2, u32 offset, i32 length, i32 result of
// sub_70C60, u16 word_0, u16 width, u16 height, u16 frame count (the byte after the pixels); the pixels stay zero.
static const int kTmapsMetaSprites = 504;

static bool ReadWholeFile(const std::string& path, std::vector<uint8_t>& data)
{
	FILE* file = fopen(path.c_str(), "rb");
	if (!file)
		return false;
	fseek(file, 0, SEEK_END);
	data.resize(ftell(file));
	fseek(file, 0, SEEK_SET);
	const bool read = fread(data.data(), 1, data.size(), file) == data.size();
	fclose(file);
	return read;
}

bool WriteTmapsMeta(const std::string& dataDir, const std::string& outPath)
{
	std::vector<uint8_t> tabs[3], dats[3];
	for (int type = 0; type < 3; type++)
	{
		const std::string name = dataDir + "/TMAPS" + std::to_string(type) + "-0.";
		if (!ReadWholeFile(name + "TAB", tabs[type]) || !ReadWholeFile(name + "DAT", dats[type]))
			return false;
	}
	std::vector<uint8_t> out = { 'M', 'C', '2', 'T', 'M', 'A', 'P', '2', 0, 0, 0, 0 };
	auto put = [&out](uint32_t v, int bytes) { for (int b = 0; b < bytes; b++) out.push_back((uint8_t)(v >> (8 * b))); };
	std::vector<uint8_t> buffer(0x100000);
	std::vector<std::tuple<int, uint32_t, int32_t>> done;
	uint32_t count = 0;
	for (int dat = 0; dat < 3; dat++)
		for (int tab = 0; tab < 3; tab++)
		{
			const type_TMAPS00TAB_BEGIN_BUFFER* entries = (const type_TMAPS00TAB_BEGIN_BUFFER*)tabs[tab].data();
			for (int i = 0; i < kTmapsMetaSprites; i++)
			{
				const uint32_t offset = entries[i].offset_4;
				const int32_t length = entries[i + 1].offset_4 - entries[i].offset_4;
				if (std::find(done.begin(), done.end(), std::make_tuple(dat, offset, length)) != done.end())
					continue;
				done.emplace_back(dat, offset, length);
				// as DecompressTmap_70C60: a short read is -1, a decompression error -2, a plain block its length
				std::fill(buffer.begin(), buffer.end(), 0);
				int32_t result = -1;
				if (length >= 0 && (size_t)offset + length <= dats[dat].size() && (size_t)length <= buffer.size())
				{
					memcpy(buffer.data(), &dats[dat][offset], length);
					result = DataFileRNC::Decompress(buffer.data(), buffer.data());
					if (result >= 0)
					{
						if (!result)
							result = length;
					}
					else
						result = -2;
				}
				const Type_Sprite* sprite = (const Type_Sprite*)buffer.data();
				const uint32_t pixels = (uint32_t)sprite->width * sprite->height;
				put(dat, 1);
				put(offset, 4);
				put(length, 4);
				put(result, 4);
				put(sprite->flags_0, 2);
				put(sprite->width, 2);
				put(sprite->height, 2);
				put(6 + pixels < buffer.size() ? (uint8_t)sprite->textureBuffer[pixels] : 0, 2);
				count++;
			}
		}
	for (int b = 0; b < 4; b++)
		out[8 + b] = (uint8_t)(count >> (8 * b));
	FILE* file = fopen(outPath.c_str(), "wb");
	if (!file)
		return false;
	const bool written = fwrite(out.data(), 1, out.size(), file) == out.size();
	fclose(file);
	return written;
}

static int PackedTmap(uint16_t texture_index, uint8_t* texture_buffer)
{
	struct Record { int32_t result; uint16_t flags_0, width, height, frames; };
	static std::map<std::tuple<int, uint32_t, int32_t>, Record> records;
	if (records.empty())
	{
		std::vector<uint8_t> meta;
		ReadWholeFile(GetSubDirectoryFile(cdFolder.c_str(), "DATA", "TMAPSMETA.DAT"), meta);
		auto get = [&meta](size_t p, int bytes) { uint32_t v = 0; for (int b = 0; b < bytes; b++) v |= (uint32_t)meta[p + b] << (8 * b); return v; };
		const uint32_t count = meta.size() >= 12 && memcmp(meta.data(), "MC2TMAP2", 8) == 0 ? get(8, 4) : 0;
		for (uint32_t i = 0; i < count && 12 + (i + 1) * 21 <= meta.size(); i++)
		{
			const size_t p = 12 + i * 21;
			records[{ (int)get(p, 1), get(p + 1, 4), (int32_t)get(p + 5, 4) }] =
				{ (int32_t)get(p + 9, 4), (uint16_t)get(p + 13, 2), (uint16_t)get(p + 15, 2), (uint16_t)get(p + 17, 2), (uint16_t)get(p + 19, 2) };
		}
	}
	const uint32_t offset = str_TMAPS00TAB_BEGIN_BUFFER[texture_index].offset_4;
	const int32_t length = str_TMAPS00TAB_BEGIN_BUFFER[texture_index + 1].offset_4 - str_TMAPS00TAB_BEGIN_BUFFER[texture_index].offset_4;
	const auto found = records.find({ (int)tmapsMapType, offset, length });
	if (found == records.end())
		return -1;
	const Record& r = found->second;
	if (r.result == -1)
		return -1;
	const int size = r.result > length ? r.result : length;
	memset(texture_buffer, 0, size);
	Type_Sprite* sprite = (Type_Sprite*)texture_buffer;
	if (size >= 6)
	{
		sprite->flags_0 = r.flags_0;
		sprite->width = r.width;
		sprite->height = r.height;
	}
	const uint32_t pixels = (uint32_t)r.width * r.height;
	if (6 + pixels < (uint32_t)size)
		sprite->textureBuffer[pixels] = (int8_t)r.frames;
	return r.result;
}

int DecompressTmap_70C60(uint16_t texture_index, uint8_t* texture_buffer)//251c60
{
	int result; // eax

	const std::vector<uint8_t>& fixed = fixedSprites[(int)tmapsMapType][texture_index];
	if (!fixed.empty() && fixed.size() <= 4 * ((unsigned int)(str_TMAPS00TAB_BEGIN_BUFFER[texture_index].size_0 + 13) >> 2))//graphics/fixed/tmaps, the buffer of LoadTMapMetadata_71E70
	{
		memcpy(texture_buffer, fixed.data(), fixed.size());
		return (int)fixed.size();
	}
	if (PackedGameData() && tmapsFile_DB73C == NULL)//regression tests without the game: the header and the frame count from TMAPSMETA.DAT
		return PackedTmap(texture_index, texture_buffer);
	if (tmapsFile_DB73C == NULL) {
		return 0; //(int)tmapsFile_DB73C;
	}

	DataFileIO::Seek(tmapsFile_DB73C, str_TMAPS00TAB_BEGIN_BUFFER[texture_index].offset_4, 0);//lseek
	int v3 = str_TMAPS00TAB_BEGIN_BUFFER[texture_index + 1].offset_4 - str_TMAPS00TAB_BEGIN_BUFFER[texture_index].offset_4;
	if (DataFileIO::Read(tmapsFile_DB73C, texture_buffer, v3) != v3)
		return -1;
	result = DataFileRNC::Decompress(texture_buffer, texture_buffer);
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

void WriteTextureMapToBmp(uint16_t textureIndex, uint8_t* ptextureMap, uint16_t width, uint16_t height, MapType_t mapType)
{
	char name[MAX_PATH];
	std::string path = GetSubDirectoryPath("BufferOut");
	if (myaccess(path.c_str(), 0) < 0)
	{
		std::string exepath = get_exe_path();
		mymkdir((exepath + "/" + "BufferOut").c_str());
	}

	if (m_pColorPalette == NULL)
	{
		m_pColorPalette = LoadTMapColorPalette(mapType);
		path = GetSubDirectoryFilePath("BufferOut", "PalletOut.bmp");
		BitmapIO::WritePaletteAsImageBMP(path.c_str(), 256, m_pColorPalette);
	}

	sprintf(name, "TmapOut%03d%s", textureIndex, ".bmp");
	path = GetSubDirectoryFilePath("BufferOut", name);
	BitmapIO::WriteRGBAImageBufferAsImageBMP(path.c_str(), width, height, m_pColorPalette, ptextureMap);
}

void WriteTextureMapToBmp(uint16_t textureIndex, uint16_t frameIndex, uint8_t* ptextureMap, uint16_t width, uint16_t height, MapType_t mapType)
{
	char name[MAX_PATH];
	std::string path = GetSubDirectoryPath("BufferOut");
	if (myaccess(path.c_str(), 0) < 0)
	{
		std::string exepath = get_exe_path();
		mymkdir((exepath + "/" + "BufferOut").c_str());
	}

	if (m_pColorPalette == NULL)
	{
		m_pColorPalette = LoadTMapColorPalette(mapType);
		path = GetSubDirectoryFilePath("BufferOut","PalletOut.bmp");
		BitmapIO::WritePaletteAsImageBMP(path.c_str(), 256, m_pColorPalette);
	}

	sprintf(name, "TmapOut%03d-F%03d%s", textureIndex, frameIndex, ".bmp");
	path = GetSubDirectoryFilePath("BufferOut", name);
	BitmapIO::WriteRGBAImageBufferAsImageBMP(path.c_str(), width, height, m_pColorPalette, ptextureMap);
}

std::vector<uint8_t*> GetRawFrames(uint16_t frameCount, Type_Sprite* ptextureMap, size_t startOffset)
{
	std::vector<uint8_t*> frames;

	if (!ptextureMap || !ptextureMap->textureBuffer)
		return frames;

	FlcState st;
	st.width = ptextureMap->width;
	st.height = ptextureMap->height;

	size_t frameBytes = static_cast<size_t>(st.width) * st.height;
	uint8_t* buf = reinterpret_cast<uint8_t*>(ptextureMap->textureBuffer);

	// Working buffer containing the currently decoded frame.
	std::vector<uint8_t> work(buf, buf + frameBytes);

	// Allocate/copy frame 0.
	uint8_t* frame0 = new uint8_t[frameBytes];
	std::memcpy(frame0, work.data(), frameBytes);
	frames.push_back(frame0);

	size_t off = startOffset ? startOffset : frameBytes + 6;

	for (uint16_t f = 1; f < frameCount; ++f)
	{
		const uint8_t* next = ApplyFlcFrame(buf + off, work.data(), st);

		if (!next)
			break;

		off = static_cast<size_t>(next - buf);

		// Allocate an independent buffer for this completed frame.
		uint8_t* frame = new uint8_t[frameBytes];
		std::memcpy(frame, work.data(), frameBytes);

		frames.push_back(frame);
	}
	return frames;
}

static uint16_t FlcRd16(uint8_t* p) 
{
	return (uint16_t)(p[0] | (p[1] << 8)); 
}

uint8_t* ApplyDeltaFlc(uint8_t* p, uint8_t* img, const FlcState& st)
{
	int w = st.width, h = st.height;
	int lines = FlcRd16(p); p += 2;          // line records (0x2c = 44 in your dump)
	int y = 0;

	while (lines > 0 && y < h) {
		uint16_t op = FlcRd16(p); p += 2;

		switch (op & 0xC000) {
		case 0xC000: y += -(int16_t)op; continue;              // skip lines
		case 0x8000: {                                         // last pixel of the line
			const uint8_t v = op & 0xFF;
			if (y < h && (!st.skipZero || v)) img[(size_t)y * w + (w - 1)] = v;
			continue;                                          // then the packet count follows
		}
		case 0x4000: continue;                                 // undefined
		}

		int packets = op;                                      // 0x0000: packets on this line
		int x = 0;
		while (packets-- > 0) {
			x += *p++;                                         // skip unchanged pixels
			int8_t cnt = (int8_t)*p++;
			if (cnt >= 0) {                                    // cnt words of literal pixels
				for (int i = 0; i < cnt * 2; ++i, ++x)
					if (x < w) img[(size_t)y * w + x] = p[i];
				p += cnt * 2;
			}
			else {                                           // one word repeated -cnt times
				uint8_t a = p[0], b = p[1]; p += 2;
				for (int i = 0; i < -cnt; ++i) {
					if (x < w) { img[(size_t)y * w + x] = a; } ++x;
					if (x < w) { img[(size_t)y * w + x] = b; } ++x;
				}
			}
		}
		++y; --lines;
	}
	return p;
}

void ApplyByteRun(uint8_t* p, uint8_t* end, uint8_t* img, const FlcState& st)
{
	int w = st.width, h = st.height;
	auto put = [&](int y, int& x, uint8_t v) {
		if (x < w && !(st.skipZero && v == 0)) img[(size_t)y * w + x] = v;
		++x;
		};
	for (int y = 0; y < h && p < end; ++y) {
		++p;                                                   // obsolete packet count
		int x = 0;
		while (x < w && p + 1 < end) {
			int8_t cnt = (int8_t)*p++;
			if (cnt > 0) {
				uint8_t v = *p++;
				for (int i = 0; i < cnt; ++i) put(y, x, v);
			}
			else if (cnt < 0) {
				for (int i = 0; i < -cnt; ++i, ++p) put(y, x, *p);
			}
			else {
				++p;   // the game treats 0 as a 256-byte literal run; unused for widths < 256
			}
		}
	}
}

uint8_t* ApplyFlcFrame(uint8_t* src, uint8_t* img, FlcState& st)
{
	uint8_t* p = src;
	uint16_t magic;
	for (;;) {
		p += 4;
		magic = FlcRd16(p); p += 2;
		if (magic != 0xAF12) break;
		st.width = FlcRd16(p + 2);
		st.height = FlcRd16(p + 4);
		p += 6;
	}
	if (magic != 0xF1FA) return nullptr;

	int chunks = FlcRd16(p); p += 2; 
	p += 8;
	while (chunks-- > 0) {
		uint16_t size16 = FlcRd16(p);
		uint16_t type = FlcRd16(p + 4);
		p += 6;
		switch (type) {
		case 7:  p = ApplyDeltaFlc(p, img, st);                         
			break;
		case 15: ApplyByteRun(p, p + size16 - 6, img, st); p += size16 - 6; 
			break;
		default: p += size16 - 6;                                        
			break;
		}
	}
	return p;
}

uint8_t* LoadTMapColorPalette(MapType_t mapType)
{
	uint8_t* Palettebuffer = new uint8_t[768];
	FILE* palfile;
	char palleteName[50];

	switch (mapType)
	{
		case MapType_t::Cave:
			sprintf(palleteName, "CD_Files/DATA/PALC-0.DAT");
		break;
		case MapType_t::Day:
			sprintf(palleteName, "CD_Files/DATA/PALD-0.DAT");
		break;
		case MapType_t::Night:
			sprintf(palleteName, "CD_Files/DATA/PALN-0.DAT");
		break;
	}

	std::string path = GetSubDirectoryPath(palleteName);
	if (std::filesystem::exists(path))
	{
		palfile = fopen(path.c_str(), "rb");
		fread(Palettebuffer, 768, 1, palfile);
		fclose(palfile);
	}

	return Palettebuffer;
}

type_animations1* InitTmapAnimation_721C0(type_E9C08* a1x, Type_Sprite** a2x, int16_t index)//2531c0
{
	signed __int16 v3; // cx
	signed __int16 v4; // si
	signed __int16 i; // bx
	//x_DWORD *v6; // edx
	type_animations1* v6x;
	Type_Sprite* v7x; // ebx
	int length_v8; // ecx
	int16_t frameCount_v9; // ST08_2
	//int v10; // edx
	signed __int16 v12; // [esp+Ch] [ebp-4h]

	v3 = -1;
	v4 = -1;
	if (!(a1x->count_0))
		return 0;
	for (i = 0; i < a1x->count_0; i++)
	{
		v6x = &a1x->animations_2[i];
		if (v6x->Sprite_4)
		{
			if (!v6x->active_0)
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
	length_v8 = (*a2x)->height * (*a2x)->width;
	//v9 = *(x_WORD*)(v8 + (*a2x)->un_0.byte[0] + 6);//? is ok
	frameCount_v9 = ((*a2x)->textureBuffer)[length_v8];//? is ok
	//v10 = 28 * v12;
	a1x->animations_2[v12].Sprite_4 = *a2x;
	a1x->animations_2[v12].word_12 = 6;
	a1x->animations_2[v12].NextFrameOffset_14 = length_v8 + 6;
	a1x->animations_2[v12].CountOfFrames_16 = frameCount_v9;
	a1x->animations_2[v12].Width_18 = v7x->width;
	a1x->animations_2[v12].Height_20 = v7x->height;
	a1x->animations_2[v12].frameOffset_8 = length_v8 + 6;
	a1x->animations_2[v12].FrameIndex_22 = 1;
	a1x->animations_2[v12].active_0 = 1;
	a1x->animations_2[v12].word_24 = v12;
	a1x->animations_2[v12].spriteIndex_26 = index;


	auto rawData = new uint8_t[a1x->animations_2[v12].CountOfFrames_16 * a1x->animations_2[v12].Width_18 * a1x->animations_2[v12].Height_20]();
	size_t frameBytes = a1x->animations_2[v12].Width_18 * a1x->animations_2[v12].Height_20;

	if (bigSprites && index >= 0 && index < max_sprites && v7x == BIG_SPRITES_BUFFERx[index].actdatax)
	{
		// High-res sprite: the buffer only holds frame 0 and the frame count, the frames are
		// already decoded in BIG_SPRITES_BUFFERx (AnimateSprite_72350 copies them in the same way).
		for (int f = 0; f < a1x->animations_2[v12].CountOfFrames_16 && f < max_sprites_frames; f++)
		{
			if (BIG_SPRITES_BUFFERx[index].frames[f])
				memcpy(rawData + (frameBytes * f), BIG_SPRITES_BUFFERx[index].frames[f], frameBytes);
		}
	}
	else
	{
		auto frames = GetRawFrames(frameCount_v9, a1x->animations_2[v12].Sprite_4, a1x->animations_2[v12].NextFrameOffset_14);

		for (int f = 0; f < frames.size(); f++)
		{
			memcpy(rawData + (frameBytes * f), frames[f], frameBytes);
			//WriteTextureMapToBmp(v12, f, frames[f], a1x->dword_2[v12].Width_18, a1x->dword_2[v12].Height_20, world_D41A0.terrain_2FECE.MapType);
		}

		for (uint8_t* frame : frames)
			delete[] frame;

		frames.clear();
	}

	//WriteTextureMapToBmp(v12, rawData, a1x->dword_2[v12].Width_18, a1x->dword_2[v12].Height_20 * a1x->dword_2[v12].CountOfFrames_16, world_D41A0.terrain_2FECE.MapType);

	m_spriteFrameCount[index] = std::max<uint16_t>(1, a1x->animations_2[v12].CountOfFrames_16);
	EventDispatcher::I->DispatchEvent<ResourceType, uint32_t, uint8_t*, uint32_t, uint32_t>(
		EventType::E_RESOURCE_CHANGE, ResourceType::TEXTURE_LOADED, index, rawData, a1x->animations_2[v12].Width_18, a1x->animations_2[v12].Height_20 * a1x->animations_2[v12].CountOfFrames_16);
	
	delete[] rawData;

	return &a1x->animations_2[v12];
}

void ResetAnimation_72410(type_animations1* animation)
{
	if (animation)
	{
		animation->active_0 = 0;
		animation->Sprite_4 = 0;
	}
}
