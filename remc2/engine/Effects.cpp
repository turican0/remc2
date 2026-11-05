#include "Effects.h"
#include "EventsFunctions.h"
#include "Castle.h"
#include "Projectiles.h"

#include "CommandLineParser.h"
#include "GameUI.h"
#include "Graphics.h"
#include "MenusAndIntros.h"
#include "Network.h"
#include "Player.h"
#include "PlayerInput.h"
//#include "engine_support.h"
//#include "CompareMaps.h"
#include "Events.h"
#include "Level.h"
#include "../utilities/StateMonitor.h"
#include "../utilities/RendererTests.h"
#include "ConvertMapInfo.h"
#include "LevelInit.h"
#include "DatTabIndexes.h"
#include "MouseOnOff.h"
#include "PrintMessages.h"
#include "ReadAndDecompress.h"
#include "Type_DB080_CdTrack.h"


#if defined(__linux__) || defined(__APPLE__)
#include <strings.h>
#include <cstdlib>
#include <cstring>
#define _stricmp strcasecmp
#define _strnicmp strncasecmp
#define strnicmp strncasecmp
#define __cdecl
#include <ctype.h>
#include <iostream>
#include <functional>
#include <type_traits>
#else
#endif //__linux__


#include <filesystem>


int manaSphereSizeTable_DB538[8] = { 256, 512, 1024, 2048, 4096, 9192, 18384, 36768 }; // idb//x_DWORD_DAF50ar[0x5e8]

int hitCounter_E9B90; // weak

//uint8_t* x_DWORD_E9C28x; // weak

axis_3d predictedAxis_EB398ar; // weak

//----- (00030D50) --------------------------------------------------------
void UpdateImpactExplosion_30D50(type_entity_0x6E8E* a1x)//211d50
{
	signed int v3; // esi
	uint8_t v4; // cl
	uint8_t altitude; // ch
	int v9; // esi
	uaxis_2d v10x; // [esp+0h] [ebp-4h]

	v4 = 0;

	if (a1x->param_0x10_16 & 3)
	{
		a1x->param_0x10_16--;
	}
	else
	{
		a1x->life_0x8--;
		if (a1x->life_0x8 >= -1)
		{
			a1x->flags_0xc_12_15.byte[0] &= 0xFEu;
			v3 = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
			if (!(a1x->flags_0xc_12_15.byte[0] & 2))
			{
				v10x._axis_2d.x = (a1x->position_0x4C_76.x + 128) >> 8;
				v4 = a1x->flags_0xc_12_15.byte[2];//211DFC call SetTileTerrainType_45DC0: the original leaves dl (type) from sub_10C40, a DOS address; type 0 here and in DOSBox
				v10x._axis_2d.y = (a1x->position_0x4C_76.y + 128) >> 8;
				if (!(a1x->flags_0xc_12_15.byte[2] & 1))
					SendMailToOverlappingEntities_10C80(a1x, 0, a1x->subSpellIndex_0x2A_42);
				altitude = mapTerrainType_10B4E0[v10x.word];
				if (altitude)
				{
					switch (altitude)
					{
					case 26:
						SetTileTerrainType_45DC0(v4, v10x, 0x14u);
						break;
					case 10:
						SetTileTerrainType_45DC0(v4, v10x, 0x15u);
						break;
					case 11:
						SetTileTerrainType_45DC0(v4, v10x, 0x16u);
						break;
					default:
						if ((altitude < 6u || altitude > 0x22u)
							&& (mapAngle_13B4E0[v10x.word] & 7) != 1
							&& a1x->position_0x4C_76.z - v3 <= 128
							&& !(GetTileAngleBit_104A0(&a1x->position_0x4C_76) & 1))
						{
							a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
							SpawnAroundEntity_572C0(a1x, 0, 0, 0 - (a1x->rand_0x14_20 % 7u), 1);
						}
						break;
					}
				}
				a1x->flags_0xc_12_15.byte[0] |= 2u;
				a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
				a1x->param2_0x2C_44 = a1x->rand_0x14_20 % 0x41u - 32;
				PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 3);
			}
			ClampAboveTerrain_580E0(&a1x->position_0x4C_76, v3, 0, 0, a1x->param2_0x2C_44);
			if (isCaveLevel_D41B6)
			{
				v9 = GetCeilingAlt_10C60(&a1x->position_0x4C_76) - a1x->params_0x52_82.fov;
				if (a1x->position_0x4C_76.z > v9)
					a1x->position_0x4C_76.z = v9;
			}
			AdvanceAnimationFrame_585A0(a1x);
		}
		else
		{
			DisableEntityDrawing04_57F10(a1x);
		}
	}
}

//----- (00030F60) --------------------------------------------------------
void UpdateDeathExplosion_30F60(type_entity_0x6E8E* event)//211f60
{
	int strIndex; // esi
	type_entity_0x6E8E* event2; // eax
	axis_3d position; // [esp+0h] [ebp-10h]
	int tempY; // [esp+8h] [ebp-8h]
	int tempX; // [esp+Ch] [ebp-4h]

	if (event->life_0x8-- < 0)
	{
		DisableEntityDrawing04_57F10(event);
		return;
	}
	if (!(event->flags_0xc_12_15.byte[0] & 2))
	{
		event->flags_0xc_12_15.byte[0] |= 2u;
		PrepareEventSound_6E450(event - world_D41A0.entities_0x6E8E, -1, 3);
	}
	strIndex = AddE7EE0x_10080(event->param_0x10_16, event->param_0x10_16);
	if (strIndex)
	{
		while (GetNextSearchOffset_10130(strIndex, &tempX, &tempY) == 1)
		{
			//adress 211fd8
			event->rand_0x14_20 = 9377 * event->rand_0x14_20 + 9439;
			if (2 * ((signed int)(event->rand_0x14_20 % 0x9Du) / 79) - 1 > 0)
			{
				event->rand_0x14_20 = 9377 * event->rand_0x14_20 + 9439;
				position.x = event->position_0x4C_76.x - 96 + 192 * tempX + event->rand_0x14_20 % 0x81u - 64;
				event->rand_0x14_20 = 9377 * event->rand_0x14_20 + 9439;
				position.y = event->position_0x4C_76.y - 96 + 192 * tempY + event->rand_0x14_20 % 0x81u - 64;
				position.z = event->position_0x4C_76.z;
				event2 = CreateEntity_4A190(&position, 10, 0);
				if (event2)
				{
					event2->id_0x1A_26 = event->id_0x1A_26;//here
					event2->yaw_0x1C_28 = event->yaw_0x1C_28;
					event2->flags_0xc_12_15.byte[0] |= 0x80;//here
				}
			}
		}
		ResetEvent08_10100(strIndex);
	}
}

//----- (00031100) --------------------------------------------------------
void CastSpeedSpell_31100(type_entity_0x6E8E* a1x)//212100 // zrychleni
{
	//int v1; // eax

	//v1 = a1x->dword_0x8;
	a1x->life_0x8--;// = v1 - 1;
	if (a1x->life_0x8 < -1)
		DisableEntityDrawing04_57F10(a1x);
	//return result;
}

//----- (00031120) --------------------------------------------------------
void UpdateSpeedTrail_31120(type_entity_0x6E8E* a1x)//212120
{
	//int result; // eax
	//int v2; // edx
	//int v3; // eax
	bool v4; // [esp+0h] [ebp-4h]

	v4 = false;
	//result = sub_10C40_getTerrainAlt(a1x->array_0x4C_76);
	if (GetTerrainAlt_10C40(&a1x->position_0x4C_76) != a1x->position_0x4C_76.z)
		goto LABEL_12;
	if (a1x->maxLife_0x4 - 5 == a1x->life_0x8)
	{
		CreateEntity_4A190(&a1x->position_0x4C_76, 10, 87);
		//v2 = (int)x_D41A0_BYTEARRAY_0;
		//v3 = 9377 * *(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 8) + 9439;
		//*(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 8) = v3;
		world_D41A0.rand_0x8 = 9377 * world_D41A0.rand_0x8 + 9439;
		if (!(world_D41A0.rand_0x8 & 1))
			PrepareEventSound_6E450((a1x - world_D41A0.entities_0x6E8E), -1, a1x->spriteIndex_0x5A_90 - 282);
	}
	//result = *(x_DWORD *)(a1 + 8);
	if (a1x->life_0x8-- < 0)
		LABEL_12:
	v4 = true;
	else
		AdvanceAnimationFrame_585A0(a1x);
	if (v4)
		DisableEntityDrawing04_57F10(a1x);
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (000311E0) --------------------------------------------------------
void UpdateSpeedSpell_311E0(type_entity_0x6E8E* a1x)//2121e0
{
	unsigned int v1; // eax
	type_entity_0x6E8E* v2x; // eax
	unsigned __int8 v3; // dl
	__int16 v4; // di
	__int16 v5; // si
	__int16 v6; // ax
	signed int v7; // edi
	//char v8; // al
	char v9; // al
	signed int v10; // eax
	int v11; // eax
	int v12; // esi
	int v13; // eax
	type_entity_0x6E8E* jx; // esi
	signed int v15; // eax
	signed int v16; // ecx
	signed int v17; // eax
	signed int v18; // edx
	int v19; // ecx
	int v20; // edx
	int v21; // eax
	unsigned __int8 v22; // dh
	char v23; // cl
	int v24; // eax
	unsigned __int16 v25; // dx
	__int16 v26; // si
	axis_3d v28x; // [esp+0h] [ebp-50h]
	//__int16 v29; // [esp+2h] [ebp-4Eh]
	int v30; // [esp+8h] [ebp-48h]
	unsigned int v31; // [esp+Ch] [ebp-44h]
	int v32; // [esp+10h] [ebp-40h]
	int v33; // [esp+14h] [ebp-3Ch]
	int v34; // [esp+18h] [ebp-38h]
	int v35; // [esp+1Ch] [ebp-34h]
	int v36; // [esp+20h] [ebp-30h]
	int k; // [esp+24h] [ebp-2Ch]
	int v38; // [esp+28h] [ebp-28h]
	int v39; // [esp+2Ch] [ebp-24h]
	int v40; // [esp+30h] [ebp-20h]
	int v41; // [esp+34h] [ebp-1Ch]
	int v42; // [esp+38h] [ebp-18h]
	int v43; // [esp+3Ch] [ebp-14h]
	int v44; // [esp+40h] [ebp-10h]
	int v45; // [esp+44h] [ebp-Ch]
	int v46; // [esp+48h] [ebp-8h]
	unsigned __int16 ix; // [esp+4Ch] [ebp-4h]
	//unsigned __int16 iy; // [esp+4Ch] [ebp-4h]

	v1 = a1x->maxLife_0x4;
	if (v1 < 1)
		goto LABEL_78;
	if (v1 <= 1)
	{
		v39 = 5;
		goto LABEL_8;
	}
	if (v1 == 2)
		v39 = 7;
	else
		LABEL_78:
	v39 = 3;
LABEL_8:
	v36 = v39 + 12;
	//LOBYTE(v2) = a1x->byte_0x46_70;
	if (a1x->levelPar3_0x46_70 == 1)
		return;
	if (a1x->levelPar3_0x46_70 == 0)
	{
		v3 = ((unsigned __int16)(a1x->position_0x4C_76.x + 128) >> 8) - v36;
		BYTE1(v46) = ((unsigned __int16)(a1x->position_0x4C_76.y + 128) >> 8) - v36;
		v4 = BYTE1(v46);
		v5 = v3;
		LOBYTE(v46) = v3;
		a1x->position_0x4C_76.z = GetMaxCeilingInArea_48EF0(v3, BYTE1(v46), 2 * v36, 2 * v36);
		v6 = GetMinHeightInArea_48E60(v5, v4, 2 * v36, 2 * v36);
		a1x->life_0x8 = 40;
		a1x->param2_0x2C_44 = 227;
		a1x->param3_0x36_54 = 0;
		a1x->levelPar3_0x46_70 = 2;
		a1x->axis_0x9A_154x.z = v6;
	}
	else if (a1x->levelPar3_0x46_70 > 2)
	{
		if (a1x->levelPar3_0x46_70 == 3)
			DisableEntityDrawing04_57F10(a1x);
		return;
	}
	v32 = a1x->position_0x4C_76.z;
	v42 = a1x->axis_0x9A_154x.z;
	v33 = 0;
	v44 = v32 - v42;
	v35 = 0;
	v7 = a1x->param2_0x2C_44;
	//LOBYTE(v2) = v39;
	v44 = ((v32 - v42) >> 1) + v32 - v42;
	v38 = v39;
	while (v33 < 6)
	{
		v43 = v35 - 1024;
		if (v35 - 1024 < 0)
			v43 = 0;
		if (v36 << 8 < v43)
			v43 = v36 << 8;
		v35 = v38 << 8;
		//LOBYTE(v2) = 2 * v38;
		v41 = 2 * v38;
		if (v7 > 0 && v7 <= 512)
		{
			//v8 = ((unsigned __int16)(a1x->array_0x4C_76.y + 128) >> 8) - v38;
			LOBYTE(v46) = ((unsigned __int16)(a1x->position_0x4C_76.x + 128) >> 8) - v38;
			HIBYTE(v46) = ((unsigned __int16)(a1x->position_0x4C_76.y + 128) >> 8) - v38;
			v34 = 0;
			for (ix = v46; ; ++HIBYTE(ix))
			{
				//LOBYTE(v2) = v34;
				if (v34 >= v41)
					break;
				v40 = 0;
				v9 = v46;
				while (1)
				{
					LOBYTE(ix) = v9;
					if (v40 >= v41)
						break;
					v28x.x = LOBYTE(ix) << 8;
					v28x.y = HIBYTE(ix) << 8;
					v10 = Maths::EuclideanDistXYZ_58490(&a1x->position_0x4C_76, &v28x);
					if (v10 < v35 && v10 >= v43)
					{
						v11 = (v44 * ((x_DWORD)(0x10000 + (signed int)Maths::sin_DB750[0x200 + (v10 << 10) / v35]) >> 1) >> 16)
							* (0x10000 - Maths::sin_DB750[0x200 + v7]);
						v12 = v11 >> 16;
						v45 = (v11 >> 18) + v42;
						if (mapHeightmap_11B4E0[ix] < v45)
							FindFreeTileInArea_570F0(ix, HIBYTE(ix), v45, 0, 1, 1);
						v13 = secondHeightmap_14B4E0[ix];
						v45 = v32 - v12;
						if (v32 - v12 < 0)
							v45 = 0;
						if (v13 > v45)
							secondHeightmap_14B4E0[ix] = v45;
						for (jx = gameState_D41A4.class3List_38519; jx > Entities_EA3E4[0]; jx = jx->next_0)
						{
							if (!jx->model_0x40_64)
							{
								v15 = abs(v28x.x - jx->position_0x4C_76.x);
								v16 = v15;
								if (v15 >= 0x8000)
									v16 = abs(v15 - 0x10000);
								v17 = abs(v28x.y - jx->position_0x4C_76.y);
								v18 = v17;
								if (v17 >= 0x8000)
									v18 = abs(v17 - 0x10000);
								v19 = v16 * v16;
								v20 = v18 * v18;
								if (v19 + v20 <= 0x64000)
								{
									v31 = Maths::IntegerSqrt_7277A((unsigned int)(0x64000 - v19 - v20)) >> 5;
									v30 = jx->position_0x4C_76.z >> 5;
									v45 = v30 - v31;
									if ((signed int)(v30 - v31) < 0)
										v45 = 0;
									if (v45 > 254)
										v45 = 254;
									if (mapHeightmap_11B4E0[ix] > v45)
										FindFreeTileInArea_570F0(LOBYTE(ix), HIBYTE(ix), v45, 0, 0, 1);
									v45 = v31 + v30;
									if ((signed int)(v31 + v30) < 0)
										v45 = 0;
									if (v45 > 254)
										v45 = 254;
									if (secondHeightmap_14B4E0[ix] < v45)
										secondHeightmap_14B4E0[ix] = v45;
								}
							}
						}
					}
					v21 = ix;
					v22 = mapHeightmap_11B4E0[ix];
					if (secondHeightmap_14B4E0[ix] > v22)
					{
						mapAngle_13B4E0[ix] &= 0xF7u;
					}
					else
					{
						v23 = mapAngle_13B4E0[ix] | 8;
						secondHeightmap_14B4E0[ix] = v22;
						mapAngle_13B4E0[v21] = v23;
					}
					v9 = ix + 1;
					v40++;
				}
				v34++;
			}
		}
		v7 -= 68;
		v33++;
		v38 += 2;
	}
	if (!a1x->param3_0x36_54 && a1x->param2_0x2C_44 > 455)
	{
		a1x->param3_0x36_54 = 1;
		for (k = 0; k < 2048; k += 28)
		{
			v24 = (v39 << 8) - 768;
			predictedAxis_EB398ar = a1x->position_0x4C_76;
			if (v24 < 256)
				v24 = 256;
			if (v24 > 0x2000)
				LOWORD(v24) = 0x2000;
			MoveEntity_57FA0(&predictedAxis_EB398ar, k, 0, v24);
			v2x = CreateEntity_4A190(&predictedAxis_EB398ar, 10, 13);
			if (v2x)
			{
				v2x->yaw_0x1C_28 = k;
				v25 = ix;
				v2x->maxSpeed_0x86_134 = 512;
				predictedAxis_EB398ar.z = 32 * mapHeightmap_11B4E0[v25];
				/*LOBYTE(v2) = */CopyEntityPosition_57CF0(v2x, &predictedAxis_EB398ar);
			}
		}
	}
	v26 = a1x->param2_0x2C_44 + 22;
	a1x->life_0x8 += 4;
	a1x->param2_0x2C_44 = v26;
	if (v26 > 1024)
		a1x->levelPar3_0x46_70 = 3;
	//return v2;
}
// 10000: using guessed type void /*__noreturn*/ sub_10000();
// D41A4: using guessed type int x_DWORD_D41A4;
// DBF50: using guessed type void (/*__noreturn*/ *off_DBF50[2])();
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (00031760) --------------------------------------------------------
void UpdateFire_31760(type_entity_0x6E8E* a1x)//212760
{
	int v2; // esi
	char v4; // ah
	type_entity_0x6E8E* v5x; // eax
	__int16 v6; // dx
	__int16 v7; // di
	uint32_t v8; // eax
	if (a1x->life_0x8-- < 0)
	{
		DisableEntityDrawing04_57F10(a1x);
		if (!(a1x->flags_0xc_12_15.byte[2] & 1))
			SendMailToOverlappingEntities2_11400(a1x, 0, a1x->subSpellIndex_0x2A_42);
		return;
	}
	ShakeScreenNearPlayer_5C870(a1x);
	if (a1x->life_0x8 < 12)
	{
		if (a1x->param_0x10_16 > 0)
		{
			a1x->param_0x10_16--;
			v4 = a1x->flags_0xc_12_15.byte[0];
			a1x->spriteIndex_0x5A_90--;// = v3 - 1;
			if (v4 >= 0)
			{
				a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
				if (!(a1x->rand_0x14_20 % 7u))
				{
					v5x = CreateEntity_4A190(&a1x->position_0x4C_76, 10, 14);
					if (v5x)
					{
						v6 = a1x->id_0x1A_26;
						v5x->param_0x10_16 = 100;
						v7 = v5x->spriteIndex_0x5A_90;
						v5x->life_0x8 = 15;
						v5x->id_0x1A_26 = v6;
						v5x->spriteIndex_0x5A_90 = v7 + 2;
					}
				}
			}
		}
	}
	else if (a1x->param_0x10_16 <= 6)
	{
		v2 = a1x->param_0x10_16 + 1;
		a1x->spriteIndex_0x5A_90++;
		a1x->param_0x10_16 = v2;
	}
	a1x->position_0x4C_76.z = a1x->param2_0x2C_44 + GetTerrainAlt_10C40(&a1x->position_0x4C_76);
	v8 = TerrainTileIsWater_104D0(&a1x->position_0x4C_76);
	if (v8 == 1)
		DisableEntityDrawing04_57F10(a1x);
	if (!(a1x->flags_0xc_12_15.byte[2] & 1))
		SendMailToOverlappingEntities2_11400(a1x, 0, a1x->subSpellIndex_0x2A_42);
}

//----- (00031870) --------------------------------------------------------
void UpdateEffect0A07_31870(type_entity_0x6E8E* event)//212870
{
	DisableEntityDrawing04_57F10(event);
}

//----- (00031890) --------------------------------------------------------
void UpdateWizardGhost_31890(type_entity_0x6E8E* event)//212890
{
	if (event->life_0x8-- < 0)
		DisableEntityDrawing04_57F10(event);
}

//----- (000318B0) --------------------------------------------------------
void UpdateAsh_318B0(type_entity_0x6E8E* event)//2128b0
{
	if (event->life_0x8-- >= 0)
	{
		AdvanceAnimationFrame_585A0(event);
		if (!(event->flags_0xc_12_15.byte[0] & 2))
		{
			event->flags_0xc_12_15.byte[0] |= 2u;
			PrepareEventSound_6E450(event - world_D41A0.entities_0x6E8E, -1, 27);
		}
	}
	else
	{
		DisableEntityDrawing04_57F10(event);
	}
}

//----- (00031920) --------------------------------------------------------
void UpdateEffectAction8_31920(type_entity_0x6E8E* a1x)//212920
{
	DisableEntityDrawing04_57F10(a1x);
}

//----- (00031940) --------------------------------------------------------
void UpdateManaSphereSpawner_31940(type_entity_0x6E8E* a1x)//212940
{
	//int v1;
	type_entity_0x6E8E* v1x; // eax
	unsigned int v2; // ebx
	__int16 v3; // ST0C_2
	__int16 v4; // ST08_2
	int v5; // esi
	signed int v6; // edi
	int v7; // esi
	int v8; // eax
	uaxis_2d v9x; // bx
	int v10; // edx
	signed int v11; // eax
	int v12; // edx
	int v13; // eax
	int v14; // edx
	int v15; // eax
	uaxis_2d v16x; // bx
	signed int v17; // edx
	signed int v18; // eax
	uaxis_2d v19x; // si
	int v20; // eax
	int v21; // esi
	int v22; // esi
	uaxis_2d v23x; // bx
	int v24; // eax
	signed int v25; // eax
	uaxis_2d v26x; // bx
	int v27; // edx
	signed int v28; // edx
	uaxis_2d v29x; // si
	axis_3d v31x; // [esp+0h] [ebp-3Ch]
	//__int16 v32; // [esp+2h] [ebp-3Ah]
	int v33; // [esp+8h] [ebp-34h]
	int v34; // [esp+Ch] [ebp-30h]
	int v35; // [esp+10h] [ebp-2Ch]
	int v36; // [esp+14h] [ebp-28h]
	uaxis_2d v37x; // [esp+18h] [ebp-24h]
	int v38; // [esp+1Ch] [ebp-20h]
	int v39; // [esp+20h] [ebp-1Ch]
	int v40; // [esp+24h] [ebp-18h]
	int v41; // [esp+28h] [ebp-14h]
	int v42; // [esp+2Ch] [ebp-10h]
	int v43; // [esp+30h] [ebp-Ch]
	uaxis_2d v44x; // [esp+34h] [ebp-8h]
	uaxis_2d v45x; // [esp+38h] [ebp-4h]

	v45x._axis_2d.x = (unsigned __int16)(a1x->position_0x4C_76.x + 128) >> 8;
	v45x._axis_2d.y = (unsigned __int16)(a1x->position_0x4C_76.y + 128) >> 8;
	//LOBYTE(v1) = a1x->byte_0x46_70;
	v39 = 0;
	if (a1x->levelPar3_0x46_70 < 1u)
	{
		if (a1x->levelPar3_0x46_70)
			return;
		v2 = a1x->maxLife_0x4;
		LOBYTE(v2) = v2 | 1;
		SetEntityShiftRot_49EA0(a1x, (x_WORD)v2 << 8, 0x4000);
		v44x._axis_2d.x = v45x._axis_2d.x - v2;
		v44x._axis_2d.y = v45x._axis_2d.y - v2;
		v3 = (unsigned __int8)(v45x._axis_2d.y - v2);
		v4 = (unsigned __int8)(v45x._axis_2d.x - v2);
		LOWORD(v2) = 2 * v2;
		a1x->position_0x4C_76.z = GetMinHeightInArea_48E60(v4, v3, v2, v2);
		a1x->param2_0x2C_44 = v2 + 100;
		if (a1x->position_0x4C_76.z + a1x->param2_0x2C_44 > 255)
			a1x->param2_0x2C_44 = 255 - a1x->position_0x4C_76.z;
		a1x->levelPar3_0x46_70 = 1;
	}
	else if (a1x->levelPar3_0x46_70 > 1u)
	{
		if (a1x->levelPar3_0x46_70 == 2)
		{
			v43 = a1x->position_0x4C_76.z + a1x->param2_0x2C_44 - 24;
			v20 = a1x->params_0x52_82.pitch;
			v21 = v20;
			v20 >>= 8;
			v44x._axis_2d.x = v45x._axis_2d.x - v20;
			v44x._axis_2d.y = v45x._axis_2d.y - v20;
			v22 = v21 >> 7;
			v23x._axis_2d.y = v45x._axis_2d.y - v20;
			v24 = 0;
			if (v22 > 0)
			{
				do
				{
					v23x._axis_2d.x = v44x._axis_2d.x;
					v27 = 0;
					while (v27 < v22)
					{
						if (mapHeightmap_11B4E0[v23x.word] > v43)
							mapHeightmap_11B4E0[v23x.word] = v43;
						v27++;
						v23x._axis_2d.x++;
					}
					v24++;
					v23x._axis_2d.y++;
				} while (v24 < v22);
				v25 = 0;
				v26x._axis_2d.y = v45x._axis_2d.y - 1;
			}
			else
			{
				v25 = 0;
				v26x._axis_2d.y = v45x._axis_2d.y - 1;
			}
			while (v25 < 2)
			{
				v28 = 0;
				v26x._axis_2d.x = v45x._axis_2d.x - 1;
				while (v28 < 2)
				{
					mapHeightmap_11B4E0[v26x.word] = v43 - 16;
					if (world_D41A0.terrain_2FECE.MapType != MapType_t::Day)
						v36 = 1;
					else
						v36 = 63;
					v29x.word = v26x.word;
					v28++;
					v26x._axis_2d.x++;
					mapShading_12B4E0[v29x.word] = v36;
				}
				v25++;
				v26x._axis_2d.y++;
			}
			DisableEntityDrawing04_57F10(a1x);
		}
		return;
	}
	//LOBYTE(v1) = a1;
	v5 = a1x->life_0x8 - 1;
	a1x->life_0x8 = v5;
	if (v5 <= 0)
	{
		a1x->levelPar3_0x46_70 = 2;
	}
	else
	{
		v6 = a1x->params_0x52_82.pitch;
		v7 = v6 >> 7;
		v44x._axis_2d.x = v45x._axis_2d.x - BYTE1(v6);
		v34 = v6 - ((((v6 >> 8) - 7) >> 1 << 8) + 512);
		v44x._axis_2d.y = v45x._axis_2d.y - BYTE1(v6);
		v8 = 0;
		for (v9x._axis_2d.y = v45x._axis_2d.y - BYTE1(v6); ; ++v9x._axis_2d.y)
		{
			v41 = v8;
			if (v7 <= v8)
				break;
			v10 = 0;
			for (v9x._axis_2d.x = v44x._axis_2d.x; ; v9x._axis_2d.x++)
			{
				v40 = v10;
				if (v7 <= v10)
					break;
				v31x.x = v9x._axis_2d.x << 8;
				v31x.y = v9x._axis_2d.y << 8;
				v11 = Maths::EuclideanDistXYZ_58490(&a1x->position_0x4C_76, &v31x);
				v33 = v11;
				if (v11 < v6)
				{
					v12 = (a1x->param2_0x2C_44
						* ((x_DWORD)(0x10000 + (signed int)Maths::sin_DB750[0x200 + (v11 << 10) / v6]) >> 1) >> 16)
						+ a1x->position_0x4C_76.z;
					v43 = v12;
					v42 = mapHeightmap_11B4E0[v9x.word];
					v13 = v12;
					v14 = v42;
					if (v13 > v42)
						v14 = (v13 - v42) / a1x->life_0x8 + v42;
					v43 = v14;
					FindFreeTileInArea_570F0(v9x._axis_2d.x, v9x._axis_2d.y, v14, 0, v33 <= v34, 1);
					if (isCaveLevel_D41B6)
					{
						v43 += 64;
						if (v43 > 254)
							v43 = 254;
						v37x.word = v9x.word;
						v38 = (unsigned __int8)secondHeightmap_14B4E0[v9x.word];
						if (v43 > v38)
						{
							v15 = (v38 - v43) / a1x->life_0x8;
							v43 = v38 - v15;
							secondHeightmap_14B4E0[v37x.word] = v38 - v15;
						}
					}
				}
				if (isCaveLevel_D41B6)
				{
					if (secondHeightmap_14B4E0[v9x.word] > mapHeightmap_11B4E0[v9x.word])
						mapAngle_13B4E0[v9x.word] &= 0xF7u;
					else
						mapAngle_13B4E0[v9x.word] |= 8u;
				}
				v10 = v40 + 1;
			}
			v8 = v41 + 1;
		}
		if (!world_D41A0.altSpawnMode_0x36E03)
			v39 = (signed __int16)SendMailToOverlappingEntities3_116A0(a1x, 0, a1x->subSpellIndex_0x2A_42);
		if (v39)
			SendSpellMessage_6D8B0(a1x->id_0x1A_26, 0x12u, v39);
		PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 10);
		if (world_D41A0.altSpawnMode_0x36E03 && !(a1x->instanceIndex_0x3E_62 & 3))
			PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 63);
		//LOBYTE(v1) = a1;
		if (a1x->life_0x8 == 3)
		{
			v16x._axis_2d.y = v45x._axis_2d.y - 1;
			v17 = 0;
			v43 = a1x->position_0x4C_76.z + a1x->param2_0x2C_44 - 24;
			while (v17 < 2)
			{
				v18 = 0;
				v16x._axis_2d.x = v45x._axis_2d.x - 1;
				while (v18 < 2)
				{
					mapHeightmap_11B4E0[v16x.word] = v43 - 16;
					if (world_D41A0.terrain_2FECE.MapType != MapType_t::Day)
						v35 = 1;
					else
						v35 = 63;
					v19x.word = v16x.word;
					v18++;
					v16x._axis_2d.x++;
					mapShading_12B4E0[v19x.word] = v35;
				}
				v17++;
				v16x._axis_2d.y++;
			}
			predictedAxis_EB398ar = a1x->position_0x4C_76;
			predictedAxis_EB398ar.z = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
			v1x = world_D41A0.altSpawnMode_0x36E03 ? CreateEntity_4A190(&predictedAxis_EB398ar, 10, 91) : CreateEntity_4A190(
				&predictedAxis_EB398ar, 10, 18);
			if (v1x)
				v1x->id_0x1A_26 = a1x->id_0x1A_26;
		}
	}
	return;
}
// 10000: using guessed type void /*__noreturn*/ sub_10000();
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41B6: using guessed type char x_BYTE_D41B6;
// DBF50: using guessed type void (/*__noreturn*/ *off_DBF50[2])();
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (00031E90) --------------------------------------------------------
void UpdateSpawnAround_31E90(type_entity_0x6E8E* a1x)//212e90
{
	if (!(GetTileAngleBit_104A0(&a1x->position_0x4C_76) & 1))
	{
		a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
		SpawnAroundEntity_572C0(
			a1x,
			0,
			(a1x->params_0x52_82.pitch - (__CFSHL__(a1x->params_0x52_82.pitch >> 31, 8) + (a1x->params_0x52_82.pitch >> 31 << 8))) >> 8,
			0 - (a1x->rand_0x14_20 % 7u),
			1);
	}
	DisableEntityDrawing04_57F10(a1x);
	//return 1;
}

int debugcounter_212f00 = 0;
//----- (00031F00) --------------------------------------------------------
void SpawnEffectAroundTile_31F00(type_entity_0x6E8E* a1x, __int16 a2, __int16 a3)//212f00
{
	__int16 v3; // ebx
	int v4; // esi
	int v5; // edi
	int v7; // ebx
	int v8; // [esp+0h] [ebp-Ch]
	int v9; // [esp+4h] [ebp-8h]
	int v10; // [esp+8h] [ebp-4h]

	if (CommandLineParams.DoDebugSequences()) {
		//add_compare(0x212f04, CommandLineParams.DoDebugafterload(),0x218);
	}

	v3 = a3;
	v4 = (a1x->position_0x4C_76.x + 128) >> 8;
	v5 = (a1x->position_0x4C_76.y + 128) >> 8;
	if (a3 > ((a1x->params_0x52_82.pitch - (my_sign32(a1x->params_0x52_82.pitch) << 8) + my_sign32(a1x->params_0x52_82.pitch))) >> 8)
		v3 = ((a1x->params_0x52_82.pitch - (my_sign32(a1x->params_0x52_82.pitch) << 8) + my_sign32(a1x->params_0x52_82.pitch))) >> 8;
	v10 = -3;
	v7 = AddE7EE0x_10080(a2, v3);
	if (v7)
	{
		while (GetNextSearchOffset_10130(v7, &v9, &v8) == 1)
			FindFreeTileNear_56F10(v4 + v9, v5 + v8, v10, 0);
		ResetEvent08_10100(v7);
	}
}

//----- (00031FB0) --------------------------------------------------------
void UpdateSpellBlast_31FB0(type_entity_0x6E8E* a1x)//212fb0
{
	int v1; // edx
	unsigned __int16 v2; // ax
	__int16 v3; // ax
	unsigned __int16 v4; // dx
	int v5; // eax
	__int16 v6; // si

	if (!(a1x->instanceIndex_0x3E_62 % 3))
		a1x->param_0x10_16++;
	v1 = a1x->life_0x8;
	a1x->life_0x8 = v1 - 1;
	if (v1 < 0 || GetTileAngleBit_104A0(&a1x->position_0x4C_76) & 1)
	{
		DisableEntityDrawing04_57F10(a1x);
	}
	else
	{
		if (a1x->flags_0xc_12_15.byte[0] & 2)
			v2 = a1x->subSpellIndex_0x2A_42 / 25;
		else
			v2 = a1x->subSpellIndex_0x2A_42;
		v3 = SendMailToOverlappingEntities3_116A0(a1x, 0, v2);
		if (v3)
		{
			v4 = a1x->hitById_0x26_38;
			if (v4 >= 0xBu)
			{
				if (v4 <= 0xBu)
				{
					SendSpellMessage_6D8B0(a1x->id_0x1A_26, 0x10u, v3);
				}
				else if (v4 == 15)
				{
					SendSpellMessage_6D8B0(a1x->id_0x1A_26, 0x11u, v3);
				}
			}
		}
		//v5 = (a1x->array_0x52_82.pitch-(__CFSHL__(a1x->array_0x52_82.pitch >> 31, 8)+ (a1x->array_0x52_82.pitch >> 31 << 8))) >> 8;
		v5 = (a1x->params_0x52_82.pitch - (my_sign32(a1x->params_0x52_82.pitch) << 8) + my_sign32(a1x->params_0x52_82.pitch)) >> 8;
		v6 = a1x->param_0x10_16;
		if (v6 > v5 - 1)
		{
			v6 = v5 - 1;
			if (!(a1x->flags_0xc_12_15.byte[0] & 2))
				SpawnEffectAroundTile_31F00(a1x, v5, v5);
		}
		a1x->flags_0xc_12_15.byte[0] |= 2u;
		SpawnEffectAroundTile_31F00(a1x, 0, v6);
		PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 10);
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (000320E0) --------------------------------------------------------
void PossesHitMana_320E0(type_entity_0x6E8E* entity)//2130e0
{
	entity->param_0x10_16++;
	if (entity->life_0x8-- < 0)
		DisableEntityDrawing04_57F10(entity);
	else
	{
		AdvanceAnimationFrame_585A0(entity);
		SendMailToNearbyEntities_112D0(entity, 0);
	}
}

//----- (00032120) --------------------------------------------------------
void UpdateEffectLifetime_32120(type_entity_0x6E8E* entity)//213120
{
	entity->param_0x10_16++;
	if (entity->life_0x8-- < 0)
	{
		DisableEntityDrawing04_57F10(entity);
		return;
	}
	AdvanceAnimationFrame_585A0(entity);
	SendMailToNearbyEntities_112D0(entity, 1u);
}

//----- (00032160) --------------------------------------------------------
void UpdateRisingSmoke_32160(type_entity_0x6E8E* entity)//213160
{
	if (entity->life_0x8-- < 0)
	{
		DisableEntityDrawing04_57F10(entity);
		return;
	}
	predictedAxis_EB398ar = entity->position_0x4C_76;
	entity->actSpeed_0x82_130 -= 4;
	if (entity->actSpeed_0x82_130 < 64)
		entity->actSpeed_0x82_130 = 64;
	if (entity->actSpeed_0x82_130 > 128)
		entity->actSpeed_0x82_130 = 128;
	predictedAxis_EB398ar.z += entity->actSpeed_0x82_130;
	int tempAlt = GetTerrainAlt_10C40(&entity->position_0x4C_76);
	if (predictedAxis_EB398ar.z < tempAlt)
		predictedAxis_EB398ar.z = tempAlt;
	entity->param_0x10_16++;
	if (entity->param_0x10_16 < 16)
	{
		MoveEntity_57FA0(&predictedAxis_EB398ar, entity->yaw_0x1C_28, 0, entity->maxSpeed_0x86_134);
		entity->maxSpeed_0x86_134 -= 52;
		if (entity->maxSpeed_0x86_134 < 30)
			entity->maxSpeed_0x86_134 = 30;
		if (entity->maxSpeed_0x86_134 > 1024)
			entity->maxSpeed_0x86_134 = 1024;
		if (!(entity->param_0x10_16 & 1))
		{
			if (entity->spriteIndex_0x5A_90 < 74)
				entity->spriteIndex_0x5A_90++;
		}
	}
	if (entity->life_0x8 < 6)
	{
		if (entity->spriteIndex_0x5A_90 > 67)
			entity->spriteIndex_0x5A_90--;
	}
	CopyEntityPosition_57CF0(entity, &predictedAxis_EB398ar);
}

//----- (000322A0) --------------------------------------------------------
void UpdateRisingSmoke2_322A0(type_entity_0x6E8E* entity)//2132a0
{
	if (entity->life_0x8-- < 0)
	{
		DisableEntityDrawing04_57F10(entity);
		return;
	}
	predictedAxis_EB398ar = entity->position_0x4C_76;
	entity->actSpeed_0x82_130 -= 4;
	if (entity->actSpeed_0x82_130 < 64)
		entity->actSpeed_0x82_130 = 64;
	if (entity->actSpeed_0x82_130 > 128)
		entity->actSpeed_0x82_130 = 128;
	predictedAxis_EB398ar.z += entity->actSpeed_0x82_130;
	int tempAlt = GetTerrainAlt_10C40(&entity->position_0x4C_76);
	if (predictedAxis_EB398ar.z < tempAlt)
		predictedAxis_EB398ar.z = tempAlt;
	entity->param_0x10_16++;
	if (entity->param_0x10_16 < 16)
	{
		MoveEntity_57FA0(&predictedAxis_EB398ar, entity->yaw_0x1C_28, 0, entity->maxSpeed_0x86_134);
		entity->maxSpeed_0x86_134 -= 52;
		if (entity->maxSpeed_0x86_134 < 30)
			entity->maxSpeed_0x86_134 = 30;
		if (entity->maxSpeed_0x86_134 > 1024)
			entity->maxSpeed_0x86_134 = 1024;
		if (!(entity->param_0x10_16 & 1))
		{
			if (entity->spriteIndex_0x5A_90 < 16)
				entity->spriteIndex_0x5A_90++;
		}
	}
	if (entity->life_0x8 < 6)
	{
		if (entity->spriteIndex_0x5A_90 > 9)
			entity->spriteIndex_0x5A_90--;
	}
	CopyEntityPosition_57CF0(entity, &predictedAxis_EB398ar);
}

//----- (000323E0) --------------------------------------------------------
void UpdateParticleSmoke3B_323E0(type_entity_0x6E8E* event)//2133e0
{
	UpdateParticleSmoke_32420(event);
}

//----- (00032400) --------------------------------------------------------
void UpdateParticleSmoke3C_32400(type_entity_0x6E8E* event)//213400
{
	UpdateParticleSmoke_32420(event);
}

//----- (00032420) --------------------------------------------------------
void UpdateParticleSmoke_32420(type_entity_0x6E8E* event)//213420
{
	type_entity_0x6E8E* tempentity = 0; // ecx

	if (event->life_0x8-- < 0)
	{
		DisableEntityDrawing04_57F10(event);
		return;
	}
	axis_3d position = event->position_0x4C_76;
	event->rand_0x14_20 = 9377 * event->rand_0x14_20 + 9439;
	position.x += event->rand_0x14_20 % 0xA0u;
	event->rand_0x14_20 = 9377 * event->rand_0x14_20 + 9439;
	position.z += event->rand_0x14_20 % 0xA0u;
	if (event->model_0x40_64 == 0x3Bu)
		tempentity = SetParticleSmoke3B_4E9E0(&position);
	else if (event->model_0x40_64 == 0x3Cu)
		tempentity = SetParticleSmoke3C_4EA20(&position);
	if (tempentity)
	{
		event->rand_0x14_20 = 9377 * event->rand_0x14_20 + 9439;
		tempentity->life_0x8 = 32;
		tempentity->maxLife_0x4 = 32;
		tempentity->actSpeed_0x82_130 += event->actSpeed_0x82_130 + (event->rand_0x14_20 % 0x4Du);
	}
}

//----- (00032530) --------------------------------------------------------
void UpdateEmberSpawner_32530(type_entity_0x6E8E* entity)//213530
{
	type_entity_0x6E8E* resultx; // eax

	if (GetTileAngleBit_104A0(&entity->position_0x4C_76) & 1)
	{
		entity->param_0x10_16++;
	}
	else
	{
		if (entity->param_0x10_16 > 0)
			entity->param_0x10_16--;// = v1 - 1;
	}
	entity->life_0x8--;// = v2 - 1;
	if (entity->life_0x8 < -1 || entity->param_0x10_16 > 8)
	{
		DisableEntityDrawing04_57F10(entity);
		return;
	}
	entity->rand_0x14_20 = 9377 * entity->rand_0x14_20 + 9439;
	entity->yaw_0x1C_28 = ((entity->rand_0x14_20 % 0x5Bu) + entity->yaw_0x1C_28 - 45) & 0x7FF;
	MoveEntity_57FA0(&entity->position_0x4C_76, entity->yaw_0x1C_28, 0, 256);
	resultx = CreateEntity_4A190(&entity->position_0x4C_76, 10, 11);
	if (resultx)
	{
		resultx->params_0x52_82.pitch = entity->params_0x52_82.pitch;
		resultx->params_0x52_82.roll = entity->params_0x52_82.roll;
		resultx->params_0x52_82.fov = entity->params_0x52_82.fov;
		resultx->life_0x8 = 10;
		resultx->hitById_0x26_38 = 15;
		resultx->id_0x1A_26 = entity->id_0x1A_26;
	}
}

//----- (00032600) --------------------------------------------------------
void UpdateVolcanoRock_32600(type_entity_0x6E8E* a1x)//213600
{
	int v1; // eax
	int v2; // eax
	char v3; // ah
	//__int16 v4; // di
	//__int16 v5; // ax
	//__int16 v6; // ax
	int16_t v7; // si
	int v8; // edx
	type_entity_0x6E8E* v9x; // eax
	type_entity_0x6E8E* v10x; // eax
	__int16 v11; // cx
	int v12; // eax
	//int16_t v13; // edx

	v1 = a1x->life_0x8;
	a1x->life_0x8 = v1 - 1;
	if (v1 >= 0)
	{
		v3 = a1x->flags_0xc_12_15.byte[0];
		if (!(v3 & 2))
			a1x->flags_0xc_12_15.byte[0] = v3 | 2;
		if ((int16_t)a1x->axis_0x9A_154x.x < -80)
			a1x->axis_0x9A_154x.x = -80;
		if ((int16_t)a1x->axis_0x9A_154x.x > 80)
			a1x->axis_0x9A_154x.x = 80;
		if ((int16_t)a1x->axis_0x9A_154x.y < -80)
			a1x->axis_0x9A_154x.y = -80;
		if ((int16_t)a1x->axis_0x9A_154x.y > 80)
			a1x->axis_0x9A_154x.y = 80;
		predictedAxis_EB398ar = a1x->position_0x4C_76;
		//v4 = a1x->word_0x9A_154[1];
		predictedAxis_EB398ar.x = a1x->axis_0x9A_154x.x + a1x->position_0x4C_76.x;
		predictedAxis_EB398ar.y = a1x->axis_0x9A_154x.y + a1x->position_0x4C_76.y;
		//v5 = *(x_WORD *)(a1 + 44);
		predictedAxis_EB398ar.z += a1x->param2_0x2C_44;
		a1x->param2_0x2C_44 -= 28;
		if (a1x->param2_0x2C_44 < -384)
			a1x->param2_0x2C_44 = -384;
		if (a1x->param2_0x2C_44 > 256)
			a1x->param2_0x2C_44 = 256;
		//v6 = sub_10C40_getTerrainAlt(x_WORD_EB398ar);
		v7 = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
		if (v7 > predictedAxis_EB398ar.z)
		{
			v8 = a1x->param2_0x2C_44;
			predictedAxis_EB398ar.z = v7;
			//a1x->word_0x2C_44 = -((v8 - (__CFSHL__(v8 >> 31, 2) + 4 * (v8 >> 31))) >> 2);
			a1x->param2_0x2C_44 = -((v8 - (my_sign32(v8) << 2) + my_sign32(v8)) >> 2);
			if (TerrainTileIsWater_104D0(&a1x->position_0x4C_76) == 1)
			{
				v9x = CreateEntity_4A190(&predictedAxis_EB398ar, 10, 5);
				if (v9x)
				{
					v9x->id_0x1A_26 = a1x->id_0x1A_26;
					DisableEntityDrawing04_57F10(a1x);
				}
			}
			else
			{
				if (!IsEntityOfModelNearby_10B70(&predictedAxis_EB398ar, 10, 6))
				{
					v10x = CreateEntity_4A190(&predictedAxis_EB398ar, 10, 6);
					if (v10x)
					{
						v11 = v10x->subSpellIndex_0x2A_42;
						v10x->id_0x1A_26 = a1x->id_0x1A_26;
						v10x->life_0x8 = 30;
						v10x->subSpellIndex_0x2A_42 = 3 * v11;
						a1x->param_0x10_16 = 0;
					}
				}
				if (a1x->param2_0x2C_44 <= 28)
					a1x->param2_0x2C_44 = 0;
			}
		}
		a1x->param_0x10_16++;
		CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
		//LOWORD(v2) = x_WORD_EB398ar.z;
		if (v7 == predictedAxis_EB398ar.z)
		{
			GetTerrainHeightAt_58030(&a1x->position_0x4C_76, &predictedAxis_EB398ar);
			a1x->axis_0x9A_154x.x += predictedAxis_EB398ar.x;
			//v12 = (250 * a1x->axis_0x9A_154x.x - (__CFSHL__(250 * a1x->axis_0x9A_154x.x >> 31, 8) + (250 * a1x->axis_0x9A_154x.x >> 31 << 8))) >> 8;
			v12 = ((250 * (int16_t)a1x->axis_0x9A_154x.x) - (my_sign32(250 * (int16_t)a1x->axis_0x9A_154x.x) << 8) + my_sign32(250 * (int16_t)a1x->axis_0x9A_154x.x)) >> 8;
			a1x->axis_0x9A_154x.y += predictedAxis_EB398ar.y;
			//v13 = a1x->axis_0x9A_154x.y;
			a1x->axis_0x9A_154x.x = v12;
			//v2 = (250 * v13 - (__CFSHL__(250 * v13 >> 31, 8) + (250 * v13 >> 31 << 8))) >> 8;
			v2 = ((250 * (int16_t)a1x->axis_0x9A_154x.y) - (my_sign32(250 * (int16_t)a1x->axis_0x9A_154x.y) << 8) + my_sign32(250 * (int16_t)a1x->axis_0x9A_154x.y)) >> 8;
			a1x->axis_0x9A_154x.y = v2;
		}
	}
	else
	{
		DisableEntityDrawing04_57F10(a1x);
	}
	//return v2;
}
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39A: using guessed type __int16 x_WORD_EB398ar[1];
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (00032880) --------------------------------------------------------
void UpdateMeteor_32880(type_entity_0x6E8E* a1x)//213880
{
	int v1; // edx
	//int result; // eax
	//int v3; // edx
	__int16 v4; // ax
	int v5; // edi
	type_entity_0x6E8E* v6x; // eax
	//uint8_t* v7; // esi
	//int v8; // ecx
	axis_3d v9x; // [esp+0h] [ebp-10h]
	//__int16 v10; // [esp+2h] [ebp-Eh]
	//__int16 v11; // [esp+4h] [ebp-Ch]
	int v12; // [esp+8h] [ebp-8h]
	int v13; // [esp+Ch] [ebp-4h]

	v1 = a1x->life_0x8 - 1;
	a1x->life_0x8 = v1;
	if (v1 < 0)
	{
		DisableEntityDrawing04_57F10(a1x);
		return;
	}
	if (!(a1x->flags_0xc_12_15.byte[0] & 2))
	{
		a1x->flags_0xc_12_15.dword |= 0x10002;
		//a1x->byte_0xc_12 |= 0x02;
		//a1x->byte_0xe_14 |= 0x1;
		PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 30);
	}
	//sub_49EA0(a1x,(768 * a1x->dword_0x10_16- (__CFSHL__(768 * a1x->dword_0x10_16 >> 31, 2)+ 4 * (768 * a1x->dword_0x10_16 >> 31))) >> 2,512);
	SetEntityShiftRot_49EA0(a1x, (768 * a1x->param_0x10_16 - my_sign32(768 * a1x->param_0x10_16) * 3) >> 2, 512);//0x2138F3 shl edx, 2 / sbb eax, edx

	//my_sign(768 * a1x->dword_0x10_16)

	v4 = SendMailToOverlappingEntities_10C80(a1x, 0, a1x->subSpellIndex_0x2A_42 / a1x->maxLife_0x4);
	if (v4)
		SendSpellMessage_6D8B0(a1x->id_0x1A_26, 9u, v4);
	v5 = AddE7EE0x_10080(a1x->param_0x10_16, a1x->param_0x10_16);
	if (v5)
	{
		while (GetNextSearchOffset_10130(v5, &v12, &v13) == 1)
		{
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			v9x.x = a1x->position_0x4C_76.x - 96 + 160 * v12 + a1x->rand_0x14_20 % 0x81u - 64;
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			v9x.y = a1x->rand_0x14_20 % 0x81u + 160 * v13 + a1x->position_0x4C_76.y - 96 - 64;
			v9x.z = a1x->position_0x4C_76.z;
			v6x = CreateEntity_4A190(&v9x, 10, 0);
			//v7 = v6;
			if (v6x)
			{
				//v8 = v6x->byte_0xc_12;
				v6x->id_0x1A_26 = a1x->id_0x1A_26;
				v6x->yaw_0x1C_28 = a1x->yaw_0x1C_28;
				//v6x->byte_0xc_12 = 0x10080 | v8;//(unsigned int)sub_10080 | v8;
				v6x->flags_0xc_12_15.dword |= 0x10080;
				//v6x->byte_0xc_12 |= 0x80;
				//v6x->byte_0xe_14 |= 0x1;
				SetEntityShiftRot_49EA0(v6x, 512, 512);
				v6x->param_0x10_16 = 0;
			}
		}
		ResetEvent08_10100(v5);
	}
	//result = (a1x->dword_0x10_16 + 2) / 11;
	a1x->param_0x10_16 = (a1x->param_0x10_16 + 2) % 11;
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (00032A70) --------------------------------------------------------
void UpdateVolcanoEruption_32A70(type_entity_0x6E8E* a1x)//213a70
{
	__int16 v1; // si
	__int16 v2; // ax
	__int16 v3; // si
	__int16 v4; // ax
	type_entity_0x6E8E* v5x; // eax
	type_entity_0x6E8E* v6x; // eax
	//uint8_t* v7; // esi
	type_entity_0x6E8E* v8x; // eax
	//__int16 v9; // dx
	int v10; // esi
	__int16 v11; // ax
	type_entity_0x6E8E* v12x; // [esp+0h] [ebp-4h]

	if (a1x->param_0x10_16 > 2500)
	{
		a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
		if (!(a1x->rand_0x14_20 % 0x64u) && !world_D41A0.volcanoEntity_0x31)
		{
			v1 = a1x->position_0x4C_76.z;
			v2 = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
			a1x->position_0x4C_76.z = v2;
			if (v1 != v2)
			{
				DisableEntityDrawing04_57F10(a1x);
				return;
			}
			a1x->param_0x10_16 = 0;
		}
	}
	if (a1x->param_0x10_16 < 128
		&& a1x->param_0x10_16 & 0xF
		&& (a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439, !(a1x->rand_0x14_20 % 5u))
		|| !a1x->param_0x10_16)
	{
		v3 = a1x->position_0x4C_76.z;
		v4 = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
		a1x->position_0x4C_76.z = v4;
		if (v3 != v4)
		{
			DisableEntityDrawing04_57F10(a1x);
			world_D41A0.volcanoEntity_0x31 = 0;
			return;
		}
		if (!a1x->param_0x10_16)
		{
			v5x = Entities_EA3E4[world_D41A0.volcanoEntity_0x31];
			if (v5x > Entities_EA3E4[0])
				v5x->param_0x10_16 = 250;
			world_D41A0.volcanoEntity_0x31 = a1x - world_D41A0.entities_0x6E8E;
			v6x = CreateEntity_4A190(&a1x->position_0x4C_76, 10, 19);
			//v7 = v6;
			if (v6x)
			{
				v6x->id_0x1A_26 = a1x->id_0x1A_26;
				if (Entities_EA3E4[world_D41A0.volcanoFireEntity_0x33] > Entities_EA3E4[0])
					DisableEntityDrawing04_57F10(Entities_EA3E4[world_D41A0.volcanoFireEntity_0x33]);
				world_D41A0.volcanoFireEntity_0x33 = v6x - world_D41A0.entities_0x6E8E;
			}
		}
		v8x = CreateEntity_4A190(&a1x->position_0x4C_76, 10, 16);
		if (v8x)
		{
			v8x->id_0x1A_26 = a1x->id_0x1A_26;
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			//a1x->word_0x14_20 = v9;
			v8x->rand_0x14_20 = a1x->rand_0x14_20;
		}
		v10 = a1x->param_0x10_16;
		a1x->yaw_0x1C_28 += 1280;
		if (!v10)
		{
			v12x = CreateEntity_4A190(&a1x->position_0x4C_76, 9, 0);
			if (v12x)
			{
				v12x->id_0x1A_26 = a1x->id_0x1A_26;
				v11 = a1x->yaw_0x1C_28;
				v12x->pitch_0x1E_30 = -386;
				v12x->levelPar1_0x43_67 = 10;
				v12x->levelPar2_0x44_68 = 17;
				HIBYTE(v11) &= 7u;
				v12x->life_0x8 = 1;
				v12x->yaw_0x1C_28 = v11;
				v12x->axis_0x9A_154x = a1x->position_0x4C_76;
				MoveEntity_57FA0(&v12x->axis_0x9A_154x, v12x->yaw_0x1C_28, 0, 1536);
				v12x->axis_0x9A_154x.z = GetTerrainAlt_10C40(&v12x->axis_0x9A_154x);
			}
		}
		if (a1x->param_0x10_16 >= 127)
		{
			DisableEntityDrawing04_57F10(a1x);
			world_D41A0.volcanoEntity_0x31 = 0;
		}
	}
	a1x->param_0x10_16++;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00032CF0) --------------------------------------------------------
void SpawnManaSpheres_32CF0(type_entity_0x6E8E* a1x)//213cf0
{
	type_entity_0x6E8E* v1x; // eax
	//uint8_t* v2; // ecx
	//uint8_t* v3; // ebx
	char v4; // ah
	__int16 v5; // dx
	__int16 v6; // ax
	unsigned int v7; // ecx
	signed int v8; // esi
	__int16 v9; // ax
	__int16 v10; // ax
	//__int16 v11; // cx
	//char *v12; // esi
	int v12x;
	signed int v13; // ebx
	int v14; // ST0C_4
	unsigned __int16 v15; // ST08_2
	signed int i; // [esp+0h] [ebp-4h]

	//fix
	//v3 = 0;
	//fix

	for (i = 0; i < 3; i++)
	{
		v1x = CreateEntity_4A190(&a1x->position_0x4C_76, 10, 39);
		//v2 = v1;
		//v3 = v1;
		if (v1x)
		{
			v4 = v1x->flags_0xc_12_15.byte[1] | 0x20;
			v1x->maxLife_0x4 = 140;
			v1x->flags_0xc_12_15.byte[1] = v4;
			v1x->life_0x8 = v1x->maxLife_0x4;
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			v5 = a1x->rand_0x14_20 % 0x300u;
			v1x->actSpeed_0x82_130 = v5;
			if (v5 < 64)
				v1x->actSpeed_0x82_130 = 64;
			if (v1x->actSpeed_0x82_130 > 768)
				v1x->actSpeed_0x82_130 = 768;
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			v6 = a1x->rand_0x14_20 & 0x7F;
			v1x->playerEntityIndex_0x94_148 = 0;
			v1x->param2_0x2C_44 = v6 + 128;
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			v7 = a1x->rand_0x14_20 % 9u - 1;
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			v8 = 0;
			v1x->mana_0x90_144 = a1x->rand_0x14_20 % 0xA00u + 1;
			while (v8 < 7 && v1x->mana_0x90_144 > manaSphereSizeTable_DB538[v8])
				v8++;
			v9 = GetManaSphereIndexFromId_36A50(/*(int)v3, */v7);
			SetEntityIndexAndRot_49CD0(v1x, v8 + v9);
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			v10 = a1x->rand_0x14_20 & 0x7FF;
			v1x->pitch_0x1E_30 = 0;
			v1x->yaw_0x1C_28 = v10;
			v1x->roll_0x20_32 = v1x->yaw_0x1C_28;
			v1x->axis_0x9A_154x = a1x->position_0x4C_76;
			MoveEntity_57FA0(&v1x->axis_0x9A_154x, v1x->yaw_0x1C_28, 0, v1x->actSpeed_0x82_130);
			//v11 = v1x->axis_0x9A_154x[1] - v1x->array_0x4C_76.y;
			v1x->axis_0x9A_154x.x -= v1x->position_0x4C_76.x;
			v1x->axis_0x9A_154x.y -= v1x->position_0x4C_76.y;
			v1x->position_0x4C_76.z = GetTerrainAlt_10C40(&v1x->position_0x4C_76) + 96;
		}
	}
	if (!(a1x->instanceIndex_0x3E_62 & 1))
	{
		//v12 = (char*)&(*xadataspellsdat.colorPalette_var28)[0];
		v12x = 0;
		v13 = 0;
		while (v13 < 26)
		{
			//v14 = (*(x_DWORD *)(v12 + 66) - (__CFSHL__(*(x_DWORD *)(v12 + 66) >> 31, 9) + (*(x_DWORD *)(v12 + 66) >> 31 << 9))) >> 9;
			v14 = (SPELLS_BEGIN_BUFFER_str[v12x].subspell[2].xpos1_E - (my_sign32(SPELLS_BEGIN_BUFFER_str[v12x].subspell[2].xpos1_E) << 9) + my_sign32(SPELLS_BEGIN_BUFFER_str[v12x].subspell[2].xpos1_E)) >> 9;

			v15 = v13++;
			//v12 += 80;
			v12x++;
			SendSpellMessage_6D8B0(world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240, v15, v14);
		}
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (00032F40) --------------------------------------------------------
void UpdateVolcanoFire_32F40(type_entity_0x6E8E* a1x)//213f40
{
	int v1; // eax
	int v2; // esi
	type_entity_0x6E8E* v3x; // eax
	axis_3d v5x; // [esp+0h] [ebp-14h]
	//__int16 v6; // [esp+2h] [ebp-12h]
	//__int16 v7; // [esp+4h] [ebp-10h]
	int v8; // [esp+8h] [ebp-Ch]
	int v9; // [esp+Ch] [ebp-8h]
	int v10; // [esp+10h] [ebp-4h]

	v1 = a1x->life_0x8;
	a1x->life_0x8 = v1 - 1;
	if (v1 >= 0)
	{
		a1x->param_0x10_16 = 0;
		v2 = AddE7EE0x_10080(0, a1x->param_0x10_16);
		if (v2)
		{
			while (GetNextSearchOffset_10130(v2, &v9, &v8) == 1)
			{
				a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
				if (2 * ((signed int)(a1x->rand_0x14_20 % 0x9Du) / 79) - 1 > 0)
				{
					a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
					v5x.x = a1x->rand_0x14_20 % 0x81u + a1x->position_0x4C_76.x - 96 + 192 * v9 - 64;
					a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
					v5x.y = a1x->position_0x4C_76.y - 96 + 192 * v8 + a1x->rand_0x14_20 % 0x81u - 64;
					v5x.z = a1x->position_0x4C_76.z;
					if (a1x->life_0x8 & 1)
					{
						LOWORD(v10) = (a1x->life_0x8 / 2 & 1) << 8;
						while ((unsigned __int16)v10 < 0x800u)
						{
							v3x = CreateEntity_4A190(&v5x, 10, 14);
							if (v3x)
							{
								v3x->id_0x1A_26 = a1x->id_0x1A_26;
								v3x->yaw_0x1C_28 = v10;
							}
							BYTE1(v10) += 2;
						}
					}
				}
			}
			ResetEvent08_10100(v2);
		}
		a1x->position_0x4C_76.z = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
	}
	else
	{
		DisableEntityDrawing04_57F10(a1x);
		world_D41A0.volcanoFireEntity_0x33 = 0;
	}
	/*return */SendMailToOverlappingEntities_10C80(a1x, 0, a1x->subSpellIndex_0x2A_42);
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (00033110) --------------------------------------------------------
void UpdateWind_33110(type_entity_0x6E8E* a1x)//214110
{
	int v1; // edx

	v1 = a1x->life_0x8 - 1;
	a1x->life_0x8 = v1;
	if (v1 < 0)
	{
		EndLoop_6EAB0(a1x - world_D41A0.entities_0x6E8E, -1, 49);
		WindPullNearbyEntities_338D0(a1x);
	}
	else
	{
		UpdateWindMovement_331A0(a1x);
		UpdateWindPull_33340(a1x);
		WindKillOverlapping_33710(a1x);
		PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 49);
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (000331A0) --------------------------------------------------------
void UpdateWindMovement_331A0(type_entity_0x6E8E* a1x)//2141a0
{
	__int16 v1; // ax
	__int16 v2; // ax
	type_entity_0x6E8E* resultx; // eax
	axis_3d* v4; // edi
	signed int v5; // eax
	int v6; // edi
	type_entity_0x6E8E* v7x; // [esp+0h] [ebp-4h]

	predictedAxis_EB398ar = a1x->position_0x4C_76;
	a1x->maxStage_0x30_48 = predictedAxis_EB398ar.z;
	if (!(a1x->instanceIndex_0x3E_62 & 0xF))
	{
		a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
		if (!(a1x->rand_0x14_20 & 1))
			a1x->stage_0x2E_46 = -a1x->stage_0x2E_46;
	}
	v1 = a1x->roll_0x20_32 + 11 * a1x->stage_0x2E_46;
	HIBYTE(v1) &= 7u;
	a1x->roll_0x20_32 = v1;
	MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->roll_0x20_32, 0, 32);
	a1x->axis_0x9A_154x = predictedAxis_EB398ar;
	v2 = a1x->yaw_0x1C_28 + 341;
	HIBYTE(v2) &= 7u;
	a1x->yaw_0x1C_28 = v2;
	MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, 0, 120);
	predictedAxis_EB398ar.z = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
	CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
	v7x = Entities_EA3E4[a1x->nextLinkedId_0x34_52];
	resultx = a1x;
	while (v7x > Entities_EA3E4[0])
	{
		predictedAxis_EB398ar = v7x->position_0x4C_76;
		v4 = &resultx->position_0x4C_76;
		v7x->yaw_0x1C_28 = Maths::GetYawAngle_581E0(&v7x->position_0x4C_76, &resultx->position_0x4C_76);
		v5 = Maths::EuclideanDistXYZ_58490(v4, &v7x->position_0x4C_76);
		v6 = 72 - 4 * (12 - v7x->param2_0x2C_44);
		if (v5 > v6)
			MoveEntity_57FA0(&predictedAxis_EB398ar, v7x->yaw_0x1C_28, 0, v5 - v6);
		predictedAxis_EB398ar.z = v7x->param3_0x36_54 + a1x->position_0x4C_76.z;
		CopyEntityPosition_57CF0(v7x, &predictedAxis_EB398ar);
		resultx = v7x;
		v7x = Entities_EA3E4[v7x->nextLinkedId_0x34_52];
	}
	//return resultx;
}
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (00033340) --------------------------------------------------------
void UpdateWindPull_33340(type_entity_0x6E8E* a1x)//214340
{
	type_entity_0x6E8E* ix; // ebx
	char v2; // dl
	signed int v3; // eax
	signed int v4; // eax
	int v5; // eax
	__int16 v6; // dx
	//int v7; // eax
	__int16 v8; // cx
	int v9; // edi
	__int16 v10; // si
	__int16 v11; // ax
	__int16 v12; // cx
	unsigned int v13; // esi
	__int16 v14; // dx
	__int16 v15; // ax
	__int16 v16; // ax
	char v17; // ch
	//int v18; // edx
	int v19; // edi
	int v20; // esi
	//int v21; // eax
	__int16 v22; // ST10_2
	int v23; // ST0C_4
	int v24; // ST08_4
	__int16 v25; // ax
	unsigned __int16 v26; // ST10_2
	int v27; // esi
	int v28; // [esp+0h] [ebp-34h]
	int v29; // [esp+4h] [ebp-30h]
	int v30; // [esp+8h] [ebp-2Ch]
	int v31; // [esp+Ch] [ebp-28h]
	int v32; // [esp+10h] [ebp-24h]
	int v33; // [esp+14h] [ebp-20h]
	int v34; // [esp+18h] [ebp-1Ch]
	int v35; // [esp+1Ch] [ebp-18h]
	int v36; // [esp+20h] [ebp-14h]
	int v37; // [esp+24h] [ebp-10h]
	int v38; // [esp+28h] [ebp-Ch]
	char v39; // [esp+2Ch] [ebp-8h]
	bool v40; // [esp+30h] [ebp-4h]

	v31 = 0;
	v32 = (a1x->position_0x4C_76.x + 128) >> 8;
	v36 = (a1x->position_0x4C_76.y + 128) >> 8;
	v35 = AddE7EE0x_10080(0, 12);
	if (v35)
	{
		while (1)
		{
			v27 = v35;
			if (GetNextSearchOffset_10130(v35, &v29, &v28) != 1)
				break;
			for (ix = Entities_EA3E4[mapEntityIndex_15B4E0[(unsigned __int8)(v29 + v32) + ((unsigned __int8)(v28 + v36) << 8)]];
				ix != Entities_EA3E4[0];
				ix = Entities_EA3E4[ix->oldMapEntity_0x16_22])
			{
				if (CanWindAffectEntity_33810(a1x, ix))
				{
					v2 = ix->class_0x3F_63;
					v39 = 0;
					v37 = 0;
					v30 = 0;
					v40 = v2 == 3 && !ix->model_0x40_64;
					if (v40)
						v3 = 56;
					else
						v3 = 204;
					v38 = v3;
					if (v40)
						v4 = 384;
					else
						v4 = 768;
					v34 = v4;
					v5 = Maths::EuclideanDistXY_584D0(&a1x->axis_0x9A_154x, &ix->position_0x4C_76);
					v33 = v5;
					if (v5 >= 3211264)
					{
						predictedAxis_EB398ar = ix->position_0x4C_76;
						if (ix->flags_0xc_12_15.byte[3] & 0x10)
						{
							v17 = ix->flags_0xc_12_15.byte[1] | 8;
							v39 = 1;
							//v18 = ix->dword_0xA0_160;
							ix->flags_0xc_12_15.byte[1] = v17;
							v37 = ix->typeParams_0xA0_160x->climbSpeed_160_0xe_14;
							//LOWORD(v18) = v38 + ix->word_0x1C_28;
							//BYTE1(v18) &= 7u;
							v30 = 64;
							ix->yaw_0x1C_28 = (v38 + ix->yaw_0x1C_28) & 0x7ffu;
							if (v5 >= 5308416)
								ix->flags_0xc_12_15.byte[3] &= 0xEFu;
						}
					}
					else
					{
						v6 = Maths::GetYawAngle_581E0(&a1x->axis_0x9A_154x, &ix->position_0x4C_76);
						if (ix->flags_0xc_12_15.byte[3] & 0x10)
						{
							ix->flags_0xc_12_15.byte[1] |= 8u;
							v30 = 128;
							predictedAxis_EB398ar = ix->position_0x4C_76;
							v15 = ix->yaw_0x1C_28;
							v39 = 1;
							v16 = v38 + v15;
							v16 &= 0x7ffu;
							predictedAxis_EB398ar.z += 114;
							ix->yaw_0x1C_28 = v16;
						}
						else
						{
							if (v40)
							{
								//v7 = ix->dword_0xA4_164;
								v8 = ix->playerData_0xA4_164x->roll_0x155_341;
								if (v8 < 256)
									ix->playerData_0xA4_164x->roll_0x155_341 = v8 + 28;
								ix->actSpeed_0x82_130 = 80;
							}
							if (v33 >= 0x40000)
							{
								v14 = v6 + 591;
								v14 &= 0x7ffu;
								predictedAxis_EB398ar = ix->position_0x4C_76;
								ix->maxStage_0x30_48 = v14;
								ix->yaw_0x1C_28 = v14;
								v30 = 96;
							}
							else
							{
								ix->flags_0xc_12_15.byte[1] |= 8u;
								predictedAxis_EB398ar = a1x->axis_0x9A_154x;
								v9 = ix->position_0x4C_76.z - a1x->maxStage_0x30_48 + 57;
								v10 = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
								predictedAxis_EB398ar.z = v9 + v10;
								if (predictedAxis_EB398ar.z < v10)
									predictedAxis_EB398ar.z = v10;
								v11 = v38 + ix->yaw_0x1C_28;
								v11 &= 0x7ffu;
								v12 = ix->rand_0x14_20;
								ix->yaw_0x1C_28 = v11;
								v13 = v34;
								ix->rand_0x14_20 = 9377 * v12 + 9439;
								if (v9 >= (signed int)(v13 + ix->rand_0x14_20 % v13))
								{
									ix->flags_0xc_12_15.byte[3] |= 0x10u;
									ix->maxStage_0x30_48 = ix->yaw_0x1C_28;
								}
							}
						}
					}
					MoveEntity_57FA0(&predictedAxis_EB398ar, ix->maxStage_0x30_48, 0, v30);
					if (isCaveLevel_D41B6)
					{
						v19 = ix->params_0x52_82.fov;
						v20 = (signed __int16)GetCeilingAlt_10C60(&predictedAxis_EB398ar) - v19;
						if (predictedAxis_EB398ar.z > v20)
							predictedAxis_EB398ar.z = v20;
					}
					//v21 = ix->dword_0xA0_160;
					v22 = v37;
					v23 = ix->typeParams_0xA0_160x->maxHeight_160_0xa_10;
					v24 = ix->typeParams_0xA0_160x->minHeight_160_0xc_12;
					v25 = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
					ClampAboveTerrain_580E0(&predictedAxis_EB398ar, v25, v24, v23, v22);
					CopyEntityPosition_57CF0(ix, &predictedAxis_EB398ar);
					if (v39)
					{
						v26 = a1x->subSpellIndex_0x2A_42;
						v31++;
						SendMailToEntity_11900(a1x, ix, 0, v26);
					}
				}
			}
		}
		ResetEvent08_10100(v27);
		if (v31)
			SendSpellMessage_6D8B0(a1x->id_0x1A_26, 0x15u, v31);
	}
}
// D41B6: using guessed type char x_BYTE_D41B6;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (00033710) --------------------------------------------------------
void WindKillOverlapping_33710(type_entity_0x6E8E* a1x)//214710
{
	int v1; // edi
	type_entity_0x6E8E* ix; // ebx
	type_entity_0x6E8E* jx; // ebx
	__int16 v4; // ax

	v1 = 0;
	if (!(a1x->instanceIndex_0x3E_62 & 7))
	{
		for (ix = gameState_D41A4.buildingList_38527; ix > Entities_EA3E4[0]; ix = ix->next_0)
		{
			if (CompareAxisWithShift_10750(a1x, ix))
				SendMailToEntity_11900(a1x, ix, 0, a1x->subSpellIndex_0x2A_42);
		}
		for (jx = gameState_D41A4.class3List_38519; jx > Entities_EA3E4[0]; jx = jx->next_0)
		{
			if (jx->model_0x40_64 == 2 && CompareAxisWithShift_10750(a1x, jx))
			{
				v4 = a1x - world_D41A0.entities_0x6E8E;
				jx->maxStage_0x30_48 = 30;
				jx->hitById_0x26_38 = v4;
				jx->mail_0x5E_94.channel[0].amount.dword += a1x->subSpellIndex_0x2A_42;
				v1 += 2;
				jx->mail_0x5E_94.channel[0].source = a1x->id_0x1A_26;
			}
		}
		if (v1)
			SendSpellMessage_6D8B0(a1x->id_0x1A_26, 0x15u, v1);
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00033810) --------------------------------------------------------
char CanWindAffectEntity_33810(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//214810
{
	char result; // al
	unsigned __int8 v3; // ah
	//char v4; // bl
	unsigned __int8 v5; // ah
	unsigned __int8 v6; // ah
	unsigned __int8 v7; // ah

	result = 1;
	v3 = a2x->class_0x3F_63 - 2;
	if (v3 > 8u)
		return 0;
	switch (v3)
	{
	case 0u:
		v7 = a2x->model_0x40_64;
		if (v7 < 7u || v7 > 8u)
			return 0;
		return result;
	case 1u:
		if (a2x->id_0x1A_26 == a1x->id_0x1A_26)
			return 0;
		if (a2x->model_0x40_64 == 2)
			result = 0;
		return result;
	case 2u:
	case 4u:
	case 5u:
	case 6u:
	case 7u:
		return 0;
	case 3u:
		//v4 = a2x->byte_0x45_69;
		if (a2x->actionIndex_0x45_69 == 232 || a2x->actionIndex_0x45_69 == 180)
			return 0;
		v5 = a2x->model_0x40_64;
		if (v5 < 0xFu)
		{
			if (v5 == 10)
				result = 0;
		}
		else if (v5 <= 0xFu || v5 >= 0x12u && (v5 <= 0x12u || v5 >= 0x1Bu && v5 <= 0x1Cu))
		{
			return 0;
		}
		return result;
	case 8u:
		v6 = a2x->model_0x40_64;
		if (v6 < 0x27u)
		{
			if (v6 < 0xDu)
				return 0;
			if (v6 > 0xEu)
				result = 0;
		}
		else if (v6 > 0x27u && v6 != 57)
		{
			result = 0;
		}
		break;
	}
	return result;
}

//----- (000338D0) --------------------------------------------------------
void WindPullNearbyEntities_338D0(type_entity_0x6E8E* a1x)//2148d0
{
	int v1; // edi
	int v2; // esi
	//unsigned int result; // eax
	int v4; // edi
	int v5; // ebx
	int16_t ix; // eax
	type_entity_0x6E8E* v7x; // eax
	type_entity_0x6E8E* jx; // ebx
	int v9; // [esp+0h] [ebp-8h]
	int v10; // [esp+4h] [ebp-4h]

	v1 = a1x->position_0x4C_76.y + 128;
	v2 = (a1x->position_0x4C_76.x + 128) >> 8;
	v5 = AddE7EE0x_10080(0, 12);
	v4 = v1 >> 8;
	//v5 = result;
	if (v5)
	{
		while (GetNextSearchOffset_10130(v5, &v10, &v9) == 1)
		{
			for (ix = mapEntityIndex_15B4E0[((unsigned __int8)(v4 + v9) << 8) + (unsigned __int8)(v2 + v10)];
				;
				ix = v7x->oldMapEntity_0x16_22)
			{
				v7x = Entities_EA3E4[ix];
				if (v7x == Entities_EA3E4[0])
					break;
				//v7x->byte_0xc_12 &= 0xEFFFF7FF;
				v7x->flags_0xc_12_15.dword &= 0xEFFFF7FF;
				//v7x->byte_0xd_13 &= 0xF7;
				//v7x->byte_0xf_15 &= 0xEF;
			}
		}
		ResetEvent08_10100(v5);
	}
	for (jx = a1x; jx > Entities_EA3E4[0]; jx = Entities_EA3E4[jx->nextLinkedId_0x34_52])
		DisableEntityDrawing04_57F10(jx);
	//return result;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000339B0) --------------------------------------------------------
void UpdateWhirlwindState_339B0(type_entity_0x6E8E* a1x)//2149b0
{
	__int16 v1; // ax
	unsigned __int16 v2; // dx
	type_entity_0x6E8E* v3x; // eax
	__int16 v4; // cx
	int v5; // edx
	char v6; // ah
	__int16 v7; // di

	LOBYTE(v1) = a1x->levelPar3_0x46_70;
	if ((unsigned __int8)v1 < 1u)
	{
		if ((x_BYTE)v1)
			return;// v1;
		v2 = a1x->targetId_0x96_150;
		if (v2)
		{
			v3x = Entities_EA3E4[v2];
			a1x->maxSpeed_0x86_134 = v3x->params_0x52_82.pitch >> 1;
			v4 = a1x->maxSpeed_0x86_134;
			a1x->minSpeed_0x84_132 = 6 * v3x->params_0x52_82.pitch >> 2;
			if (v4 < 128)
				a1x->maxSpeed_0x86_134 = 128;
			if (a1x->minSpeed_0x84_132 > 640)
				a1x->minSpeed_0x84_132 = 640;
		}
		a1x->levelPar3_0x46_70 = 1;
	}
	else if ((unsigned __int8)v1 > 1u)
	{
		if ((x_BYTE)v1 == 2)
		{
			v6 = a1x->fontTypeIndex_0x3D_61;
			if (v6 < 0)
				a1x->fontTypeIndex_0x3D_61 = -v6;
			SpinEntity_33B20(a1x);
			v1 = a1x->fontTypeIndex_0x3D_61;
			v7 = a1x->param2_0x2C_44 - v1;
			a1x->param2_0x2C_44 = v7;
			if (v7 < 0)
			{
				CreateEntity_4A190(&a1x->position_0x4C_76, 10, 0);
				/*LOBYTE(v1) = */RemoveWhirlwindSegments_33D40(a1x);
			}
		}
		return;// v1;
	}
	UpdateWhirlwindTarget_33C70(a1x);
	UpdateWhirlwindSpeed_33AD0(a1x);
	SpinEntity_33B20(a1x);
	/*LOBYTE(v1) = */UpdateWhirlwindSegments_33C00(a1x);
	v5 = a1x->life_0x8 - 1;
	a1x->life_0x8 = v5;
	if (v5 < 1)
		a1x->levelPar3_0x46_70 = 2;
	return;// v1;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00033AD0) --------------------------------------------------------
void UpdateWhirlwindSpeed_33AD0(type_entity_0x6E8E* a1x)//214ad0
{
	//int result; // eax
	__int16 v2; // bx
	__int16 v3; // cx
	char v4; // dl
	__int16 v5; // si
	char v6; // dh

	//result = a1;
	v2 = a1x->fontTypeIndex_0x3D_61 + a1x->param2_0x2C_44;
	v3 = a1x->minSpeed_0x84_132;
	a1x->param2_0x2C_44 = v2;
	if (v2 <= v3)
	{
		v5 = a1x->maxSpeed_0x86_134;
		if (v2 < v5)
		{
			v6 = a1x->fontTypeIndex_0x3D_61;
			a1x->param2_0x2C_44 = v5;
			a1x->fontTypeIndex_0x3D_61 = -v6;
		}
	}
	else
	{
		v4 = a1x->fontTypeIndex_0x3D_61;
		a1x->param2_0x2C_44 = v3;
		a1x->fontTypeIndex_0x3D_61 = -v4;
	}
	//return result;
}

//----- (00033B20) --------------------------------------------------------
void SpinEntity_33B20(type_entity_0x6E8E* a1x)//214b20
{
	__int16 v1; // ax
	__int16 v2; // ax
	int i; // eax
	__int16 v4; // ax
	__int16 v5; // ax
	__int16 v6; // ax
	__int16 v7; // ST08_2
	unsigned __int16 v8; // ax
	type_entity_0x6E8E* v9x; // ebx

	v1 = a1x->yaw_0x1C_28 + 22;
	HIBYTE(v1) &= 7u;
	a1x->yaw_0x1C_28 = v1;
	v2 = a1x->pitch_0x1E_30 + 16;
	HIBYTE(v2) &= 7u;
	a1x->pitch_0x1E_30 = v2;
	for (i = a1x->nextLinkedId_0x34_52; ; i = v9x->nextLinkedId_0x34_52)
	{
		v9x = Entities_EA3E4[i];
		if (v9x <= Entities_EA3E4[0])
			break;
		v4 = v9x->roll_0x20_32 + v9x->yaw_0x1C_28;
		HIBYTE(v4) &= 7u;
		v9x->yaw_0x1C_28 = v4;
		v5 = v9x->fov_0x22_34 + v9x->pitch_0x1E_30;
		HIBYTE(v5) &= 7u;
		v9x->pitch_0x1E_30 = v5;
		predictedAxis_EB398ar = a1x->position_0x4C_76;
		v6 = v9x->pitch_0x1E_30 + a1x->pitch_0x1E_30;
		HIBYTE(v6) &= 7u;
		v7 = v6;
		v8 = v9x->yaw_0x1C_28 + a1x->yaw_0x1C_28;
		HIBYTE(v8) &= 7u;
		MoveEntity_57FA0(&predictedAxis_EB398ar, v8, v7, a1x->param2_0x2C_44);
		CopyEntityPosition_57CF0(v9x, &predictedAxis_EB398ar);
	}
	//return i;
}
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;

//----- (00033C00) --------------------------------------------------------
void UpdateWhirlwindSegments_33C00(type_entity_0x6E8E* a1x)//214c00
{
	int result; // eax
	type_entity_0x6E8E* v2x; // ebx

	for (result = a1x->nextLinkedId_0x34_52; ; result = v2x->nextLinkedId_0x34_52)
	{
		v2x = Entities_EA3E4[result];
		if (v2x <= Entities_EA3E4[0])
			break;
		if (!v2x->levelPar2_0x44_68)
		{
			if (SendMailToOverlappingEntities_10C80(v2x, 0, a1x->subSpellIndex_0x2A_42))
				PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 3);
		}
	}
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00033C70) --------------------------------------------------------
void UpdateWhirlwindTarget_33C70(type_entity_0x6E8E* a1x)//214c70
{
	unsigned __int16 v1; // dx
	type_entity_0x6E8E* v2x; // eax
	//uint8_t* v3; // esi
	int v4; // edi
	int result; // eax
	int v6; // esi
	int v7; // edi
	int v8; // esi
	char v9; // [esp+4h] [ebp-4h]

	v1 = a1x->targetId_0x96_150;
	v9 = 0;
	if (v1)
	{
		v2x = Entities_EA3E4[v1];
		predictedAxis_EB398ar = v2x->position_0x4C_76;
		//v3 = v2;
		predictedAxis_EB398ar.z += v2x->params_0x52_82.yaw;
		CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
		if (v2x->life_0x8 < 0 || v2x->flags_0xc_12_15.byte[1] & 4)
			v9 = 1;
	}
	v4 = a1x->param2_0x2C_44;
	result = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
	v6 = v4 + (signed __int16)result;
	if (a1x->position_0x4C_76.z < v6)
		a1x->position_0x4C_76.z = v6;
	if (isCaveLevel_D41B6)
	{
		v7 = a1x->param2_0x2C_44;
		result = GetCeilingAlt_10C60(&a1x->position_0x4C_76);
		v8 = (signed __int16)result - v7;
		if (a1x->position_0x4C_76.z > v8)
			a1x->position_0x4C_76.z = v8;
	}
	if (v9)
		a1x->levelPar3_0x46_70 = 2;
	//return result;
}
// D41B6: using guessed type char x_BYTE_D41B6;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (00033D40) --------------------------------------------------------
void RemoveWhirlwindSegments_33D40(type_entity_0x6E8E* a1x)//214d40
{
	int i; // ebx
	type_entity_0x6E8E* v2x; // ebx

	for (i = a1x->nextLinkedId_0x34_52; ; i = v2x->nextLinkedId_0x34_52)
	{
		v2x = Entities_EA3E4[i];
		if (v2x <= Entities_EA3E4[0])
			break;
		DisableEntityDrawing04_57F10(v2x);
	}
	DisableEntityDrawing04_57F10(a1x);
	//return 1;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00033D80) --------------------------------------------------------
void UpdateSmallExplosion_33D80(type_entity_0x6E8E* a1x)//214d80
{
	int v1; // eax
	__int16 v2; // ax
	char v3; // dl

	v1 = a1x->life_0x8;
	a1x->param_0x10_16++;
	a1x->life_0x8 = v1 - 1;
	if (v1 >= 0)
	{
		if (!(a1x->flags_0xc_12_15.byte[0] & 2))
		{
			v2 = SendMailToOverlappingEntities_10C80(a1x, 0, a1x->subSpellIndex_0x2A_42);
			if (v2)
				SendSpellMessage_6D8B0(a1x->id_0x1A_26, 7u, v2);
			PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 24);
			v3 = a1x->flags_0xc_12_15.byte[0];
			a1x->life_0x8 = 1;
			a1x->flags_0xc_12_15.byte[0] = v3 | 2;
		}
	}
	else
	{
		DisableEntityDrawing04_57F10(a1x);
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (00033E20) --------------------------------------------------------
void UpdateEffect0A19_33E20(type_entity_0x6E8E* a1x)//214e20
{
	int v1; // ecx
	unsigned __int16 v2; // ST08_2

	v1 = a1x->life_0x8 - 1;
	a1x->param_0x10_16++;
	a1x->life_0x8 = v1;
	if (v1 >= 0)
	{
		if (!(a1x->flags_0xc_12_15.byte[0] & 2))
		{
			v2 = a1x->levelPar3_0x46_70;
			a1x->flags_0xc_12_15.byte[0] |= 2u;
			if (SendMailToOverlappingEntities_10C80(a1x, 3u, v2))
				a1x->life_0x8 = 0;
		}
	}
	else
	{
		DisableEntityDrawing04_57F10(a1x);
	}
}

//----- (00033E80) --------------------------------------------------------
void UpdateDuelHand_33E80(type_entity_0x6E8E* a1x)//214e80
{
	int v1; // eax
	int result; // eax

	v1 = a1x->life_0x8;
	a1x->param_0x10_16++;
	a1x->life_0x8 = v1 - 1;
	if (v1 < 0)
	{
		DisableEntityDrawing04_57F10(a1x);
		return;
	}
	AdvanceAnimationFrame_585A0(a1x);
	result = SendMailToOverlappingEntities_10C80(a1x, 4u, a1x->levelPar3_0x46_70);
	if (result)
		a1x->life_0x8 = 0;
	//return result;
}

//----- (00034000) --------------------------------------------------------
void UpdateEffectAction28_34000(type_entity_0x6E8E* a1x)//215000
{
	unsigned __int16 v1; // bx
	unsigned __int16 v3; // [esp+0h] [ebp-Ch]
	__int16 i; // [esp+4h] [ebp-8h]
	__int16 v5; // [esp+8h] [ebp-4h]

	LOBYTE(v3) = (a1x->position_0x4C_76.x + 128) >> 8;
	HIBYTE(v3) = (a1x->position_0x4C_76.y + 128) >> 8;
	if (v3 % 2)
		LOBYTE(v3) = v3 + 1;
	LOBYTE(v3) = v3 - a1x->life_0x8 + 1;
	v5 = a1x->life_0x8 + a1x->param_0x10_16;
	while (v5)
	{
		HIBYTE(v1) = HIBYTE(v3);
		LOBYTE(v1) = v3 - 1;
		mapAngle_13B4E0[v1] |= 0x80u;
		LOBYTE(v1) = v3;
		for (i = a1x->life_0x8; i; i--)
		{
			if (mapTerrainType_10B4E0[v1] != 8 || CheckTileType8Neighbours_33F70(v1))
				mapHeightmap_11B4E0[v1] += 48;
			SetTerrainTypeAndNeighbours_46180(v1++, 8);
		}
		mapAngle_13B4E0[v1] |= 0x80u;
		v5--;
		HIBYTE(v3)++;
	}
	DisableEntityDrawing04_57F10(a1x);
	//return 1;
}

//----- (00034110) --------------------------------------------------------
void UpdateTerrainPaint_34110(type_entity_0x6E8E* a1x)//215110
{
	unsigned __int16 v1; // bx
	char v3; // [esp+0h] [ebp-Ch]
	char v4; // [esp+1h] [ebp-Bh]
	__int16 i; // [esp+4h] [ebp-8h]
	__int16 v6; // [esp+8h] [ebp-4h]

	v3 = (a1x->position_0x4C_76.x + 128) >> 8;
	v4 = ((a1x->position_0x4C_76.y + 128) >> 8) + 2;
	v6 = a1x->life_0x8 + a1x->param_0x10_16;
	while (v6)
	{
		HIBYTE(v1) = v4;
		LOBYTE(v1) = v3 - 1;
		mapAngle_13B4E0[v1] |= 0x80u;
		LOBYTE(v1) = v3;
		for (i = a1x->life_0x8; i; i--)
		{
			if (mapTerrainType_10B4E0[v1] != 8 || CheckTileType8Neighbours_33F70(v1))
				mapHeightmap_11B4E0[v1] += 48;
			SetTerrainTypeAndNeighbours_46180(v1++, 8);
		}
		mapAngle_13B4E0[v1] |= 0x80u;
		v6--;
		v4--;
	}
	DisableEntityDrawing04_57F10(a1x);
	//return 1;
}

//----- (00034210) --------------------------------------------------------
void UpdateEffectAction29_34210(type_entity_0x6E8E* a1x)//215210
{
	int v1; // edx
	int v2; // ebx
	unsigned __int16 v3; // di
	char v4; // cl
	unsigned __int16 v5; // bx
	int v6; // ebx
	__int16 v7; // dx
	unsigned __int16 v8; // di
	char v9; // cl
	int v11; // [esp+0h] [ebp-Ch]
	int v12; // [esp+4h] [ebp-8h]
	__int16 i; // [esp+8h] [ebp-4h]

	LOBYTE(v12) = (unsigned __int16)(a1x->position_0x4C_76.x + 128) >> 8;
	HIBYTE(v12) = (unsigned __int16)(a1x->position_0x4C_76.y + 128) >> 8;
	v1 = ((unsigned __int8)v12 + BYTE1(v12)) % 2;
	if (v1)
		LOBYTE(v12) = v12 + 1;
	v2 = v12;
	LOWORD(v1) = a1x->param_0x10_16;
	--BYTE1(v2);
	while ((x_WORD)v1)
	{
		v3 = v2;
		--v1;
		v4 = mapAngle_13B4E0[(unsigned __int16)v2++] | 0x80;
		mapAngle_13B4E0[v3] = v4;
	}
	for (i = a1x->life_0x8; i; i--)
	{
		LOWORD(v1) = a1x->param_0x10_16;
		v5 = v12;
		while (1)
		{
			v11 = v1;
			if (!(x_WORD)v1)
				break;
			if (mapTerrainType_10B4E0[v5] != 8 || CheckTileType8Neighbours_33F70(v5))
				mapHeightmap_11B4E0[v5] += 48;
			SetTerrainTypeAndNeighbours_46180(v5++, 8);
			v1 = v11 - 1;
		}
		++BYTE1(v12);
	}
	v6 = v12;
	v7 = a1x->param_0x10_16;
	while (v7)
	{
		v8 = v6;
		v7--;
		v9 = mapAngle_13B4E0[(unsigned __int16)v6++] | 0x80;
		mapAngle_13B4E0[v8] = v9;
	}
	DisableEntityDrawing04_57F10(a1x);
	//return 1;
}

//----- (00034330) --------------------------------------------------------
void UpdateEffect0A1C_34330(type_entity_0x6E8E* a1x)//215330
{
	DisableEntityDrawing04_57F10(a1x);
	//return 1;
}

//----- (00034350) --------------------------------------------------------
void UpdateEffect0A1D_34350(type_entity_0x6E8E* a1x)//215350
{
	DisableEntityDrawing04_57F10(a1x);
	//return 1;
}

//----- (00034390) --------------------------------------------------------
void SetLinkedEntityLife2AndHide_34390(type_entity_0x6E8E* a1x)//215390
{
	type_entity_0x6E8E* v1x; // eax

	v1x = FindEntityOnTile_5B070(a1x);
	if (v1x)
		v1x->life_0x8 = 2;
	DisableEntityDrawing04_57F10(a1x);
	//return 1;
}

//----- (000343C0) --------------------------------------------------------
void SetLinkedEntityLife1AndHide_343C0(type_entity_0x6E8E* a1x)//2153c0
{
	type_entity_0x6E8E* v1x; // eax

	v1x = FindEntityOnTile_5B070(a1x);
	if (v1x)
		v1x->life_0x8 = 1;
	DisableEntityDrawing04_57F10(a1x);
	//return 1;
}

//----- (000343F0) --------------------------------------------------------
void ApplyPointToPath_343F0(type_entity_0x6E8E* event)//2153f0
{
	uaxis_2d v1; // bx
	uint16_t v2; // di
	v1._axis_2d.x = (event->position_0x4C_76.x + 128) >> 8;
	v1._axis_2d.y = (event->position_0x4C_76.y + 128) >> 8;
	v2 = event->param_0x10_16;
	while (v2)
	{
		mapAngle_13B4E0[v1.word] = mapAngle_13B4E0[v1.word] & 0xF0 | 1;
		v2--;
		UpdateTerrainTiles_462A0(v1, v1);
		v1._axis_2d.x += event->yaw_0x1C_28;
		v1._axis_2d.y += event->pitch_0x1E_30;
	}
	DisableEntityDrawing04_57F10(event);
}

//----- (00034480) --------------------------------------------------------
void UpdateEffect0A1F_34480(type_entity_0x6E8E* a1x)//215480
{
	DisableEntityDrawing04_57F10(a1x);
}

//----- (000344A0) --------------------------------------------------------
void UpdateEffect0A20_344A0(type_entity_0x6E8E* a1x)//2154a0
{
	int v1; // eax
	type_entity_0x6E8E* v3x; // eax

	v1 = a1x->life_0x8;
	a1x->life_0x8 = v1 - 1;
	if (v1 < 0 || GetTileAngleBit_104A0(&a1x->position_0x4C_76) & 1)
	{
		DisableEntityDrawing04_57F10(a1x);
		return;// NULL;
	}
	v3x = CreateEntity_4A190(&a1x->position_0x4C_76, 10, 11);
	if (v3x)
	{
		v3x->params_0x52_82.fov = a1x->params_0x52_82.fov;
		v3x->id_0x1A_26 = a1x->id_0x1A_26;
		v3x->life_0x8 = a1x->levelPar3_0x46_70;
	}
	MoveEntity_57FA0(&a1x->position_0x4C_76, a1x->yaw_0x1C_28, 0, a1x->actSpeed_0x82_130);
}

//----- (00034520) --------------------------------------------------------
void HideEntity_34520(type_entity_0x6E8E* a1x)//215520
{
	DisableEntityDrawing04_57F10(a1x);
}

//----- (00034910) --------------------------------------------------------
void UpdateTerrainDeform_34910(type_entity_0x6E8E* a1x)//215910
{
	int v1; // ebx
	int v2; // esi
	int v3; // ecx
	int v4; // edi
	uaxis_2d v5x; // ebx
	int v6; // eax
	int v7; // eax
	unsigned __int8 v8; // dh
	char v9; // cl
	signed int v11; // [esp+8h] [ebp-1Ch]
	signed int v12; // [esp+Ch] [ebp-18h]
	int v13; // [esp+10h] [ebp-14h]
	int v14; // [esp+14h] [ebp-10h]
	int v15; // [esp+18h] [ebp-Ch]
	uaxis_2d v16x; // [esp+1Ch] [ebp-8h]
	uaxis_2d v17x; // [esp+20h] [ebp-4h]

	v13 = 3 * a1x->levelPar3_0x46_70;
	v1 = a1x->levelPar1_0x43_67;
	v2 = 2 * v1;
	v5x._axis_2d.x = (a1x->position_0x4C_76.x >> 8) - v1;
	v3 = a1x->levelPar2_0x44_68;
	v4 = 2 * v3;
	v5x._axis_2d.y = (a1x->position_0x4C_76.y >> 8) - v3;
	v11 = GetMinHeightInArea_48E60(v5x._axis_2d.x, v5x._axis_2d.y, v2, 2 * v3);
	v16x.word = v5x.word;
	v12 = GetMaxHeightInArea_48E90(v5x._axis_2d.x, v5x._axis_2d.y, v2, v4);
	v14 = 0;
	while (v4 > v14)
	{
		v15 = 0;
		v17x.word = v16x.word;
		while (v2 > v15)
		{
			v6 = v11 - v13;
			if (v11 - v13 < 0)
				v6 = 0;
			if (v6 > 254)
				v6 = 254;
			if (mapHeightmap_11B4E0[v17x.word] > v6)
				FindFreeTileInArea_570F0(v17x._axis_2d.x, v16x._axis_2d.y, v6, 0, 0, 0);
			v7 = v13 + v12;
			if (v13 + v12 < 0)
				v7 = 0;
			if (v7 > 254)
				v7 = 254;
			if (v7 > (unsigned __int8)secondHeightmap_14B4E0[v17x.word])
				secondHeightmap_14B4E0[v17x.word] = v7;
			v8 = mapHeightmap_11B4E0[v17x.word];
			if ((unsigned __int8)secondHeightmap_14B4E0[v17x.word] > v8)
			{
				mapAngle_13B4E0[v17x.word] &= 0xF7u;
			}
			else
			{
				v9 = mapAngle_13B4E0[v17x.word] | 8;
				secondHeightmap_14B4E0[v17x.word] = v8 - 1;
				mapAngle_13B4E0[v17x.word] = v9;
			}
			v15++;
			v17x._axis_2d.x++;
		}
		v14++;
		v16x._axis_2d.y++;
	}
	UpdateTileAngles_34B00(v5x._axis_2d.x - 1, v5x._axis_2d.y - 1, v2 + 1, v4 + 1);
	RandomiseTerrainArea_43C60(v5x._axis_2d.x, v5x._axis_2d.y, (unsigned __int16)v2, (unsigned __int16)v4);
	DisableEntityDrawing04_57F10(a1x);
	//return 1;
}

//----- (00034B00) --------------------------------------------------------
void UpdateTileAngles_34B00(char a1, char a2, __int16 a3, __int16 a4)//215b00
{
	uaxis_2d v4x; // bx
	char v5; // dl
	uaxis_2d v6x; // eax
	char v7; // ch
	int result; // eax
	uaxis_2d v9x; // bx
	char v10; // dl
	uaxis_2d v11x; // eax
	char v12; // ch
	signed int v13; // [esp+0h] [ebp-Ch]
	signed int i; // [esp+4h] [ebp-8h]

	for (i = 0; a3 > i; i++)
	{
		//HIBYTE(v4) = a2;
		//LOBYTE(v4) = i + a1;
		v4x._axis_2d.y = a2;
		v4x._axis_2d.x = i + a1;
		v5 = mapAngle_13B4E0[v4x.word];
		if (v5 & 8)
		{
			mapAngle_13B4E0[v4x.word] = v5 & 0xF8 | 1;
			mapTerrainType_10B4E0[v4x.word] = 1;
			//uaxis_2d temp_v4;
			//temp_v4.word = v4;
			UpdateTerrainTiles_462A0(v4x, v4x);
		}
		//HIWORD(v6) = 0;
		//v6x._axis_2d.y = 0;
		//HIBYTE(v4) = a4 + a2;
		v4x._axis_2d.y = a4 + a2;
		//LOWORD(v6) = v4;
		v6x.word = v4x.word;
		v7 = mapAngle_13B4E0[v4x.word];
		if (v7 & 8)
		{
			mapAngle_13B4E0[v6x.word] = v7 & 0xF8 | 1;
			//uaxis_2d temp_v4;
			//temp_v4.word = v4;
			UpdateTerrainTiles_462A0(v4x, v4x);
		}
	}
	v13 = 0;
	for (result = a4; a4 > v13; result = a4)
	{
		//LOBYTE(v9) = a1;
		//HIBYTE(v9) = v13 + a2;
		v9x._axis_2d.x = a1;
		v9x._axis_2d.y = v13 + a2;
		v10 = mapAngle_13B4E0[v9x.word];
		if (v10 & 8)
		{
			mapAngle_13B4E0[v9x.word] = v10 & 0xF8 | 1;
			mapTerrainType_10B4E0[v9x.word] = 1;
			//uaxis_2d temp_v9;
			//temp_v9.word = v9;
			UpdateTerrainTiles_462A0(v9x, v9x);
		}
		//HIWORD(v11) = 0;
		//LOBYTE(v9) = a3 + v9;
		//LOWORD(v11) = v9;
		//v11x._axis_2d.y == 0;
		v9x._axis_2d.x += a3;
		v11x.word = v9x.word;
		v12 = mapAngle_13B4E0[v9x.word];
		if (v12 & 8)
		{
			mapAngle_13B4E0[v11x.word] = v12 & 0xF8 | 1;
			//uaxis_2d temp_v9;
			//temp_v9.word = v9;
			UpdateTerrainTiles_462A0(v9x, v9x);
		}
		v13++;
	}
	//return result;
}

//----- (00034C40) --------------------------------------------------------
void UpdateCraterEffect_34C40(type_entity_0x6E8E* a1x)//215c40
{
	int v1; // edx
	int v2; // eax
	int v3; // esi
	unsigned __int8 v4; // cl
	unsigned __int8 v5; // dl
	__int16 v6; // di
	signed int v7; // edi
	signed int v8; // eax
	int v9; // ecx
	int v10; // ecx
	axis_3d v12x; // [esp+0h] [ebp-40h]
	//__int16 v13; // [esp+2h] [ebp-3Eh]
	int v14; // [esp+8h] [ebp-38h]
	int v15; // [esp+Ch] [ebp-34h]
	int v16; // [esp+10h] [ebp-30h]
	int v17; // [esp+14h] [ebp-2Ch]
	int v18; // [esp+18h] [ebp-28h]
	int v19; // [esp+1Ch] [ebp-24h]
	int v20; // [esp+20h] [ebp-20h]
	int v21; // [esp+24h] [ebp-1Ch]
	int v22; // [esp+28h] [ebp-18h]
	int v23; // [esp+2Ch] [ebp-14h]
	int v24; // [esp+30h] [ebp-10h]
	int v25; // [esp+34h] [ebp-Ch]
	unsigned __int16 i; // [esp+38h] [ebp-8h]
	int v27; // [esp+3Ch] [ebp-4h]

	//fixed
	v2 = 0;
	//fixed

	v1 = a1x->life_0x8 - 1;
	a1x->life_0x8 = v1;
	if (v1 <= 0)
	{
		DisableEntityDrawing04_57F10(a1x);
	}
	else
	{
		v2 = a1x->axis_0x9A_154x.x;
		v3 = 2 * v2;
		v4 = ((unsigned __int16)(a1x->position_0x4C_76.x + 128) >> 8) - v2;
		BYTE1(v27) = ((unsigned __int16)(a1x->position_0x4C_76.y + 128) >> 8) - v2;
		v5 = a1x->levelPar3_0x46_70;
		LOBYTE(v27) = v4;
		if (v5 < 1u)
		{
			if (!v5)
			{
				v6 = v4;
				v21 = BYTE1(v27);
				a1x->position_0x4C_76.z = GetMinHeightInArea_48E60(v4, BYTE1(v27), v3, v3);
				LOWORD(v2) = GetMaxCeilingInArea_48EF0(v6, v21, v3, v3);
				a1x->axis_0x9A_154x.z = v2;
				LOBYTE(v2) = ((signed __int16)v2 - a1x->position_0x4C_76.z <= 0) + 1;
				a1x->levelPar3_0x46_70 = v2;
			}
		}
		else if (v5 <= 1u)
		{
			v7 = v2 << 8;
			v23 = a1x->position_0x4C_76.z;
			v2 = a1x->axis_0x9A_154x.z;
			v24 = v2;
			v17 = 192 * v7 >> 8;
			v14 = v2 - v23;
			LOBYTE(v2) = v27;
			v20 = 0;
			for (i = v27; v3 > v20; ++HIBYTE(i))
			{
				v19 = 0;
				LOBYTE(v2) = v27;
				LOBYTE(i) = v27;
				while (v3 > v19)
				{
					v12x.x = (unsigned __int8)i << 8;
					v12x.y = HIBYTE(i) << 8;
					v16 = 0;
					v8 = Maths::EuclideanDistXYZ_58490(&a1x->position_0x4C_76, &v12x);
					v15 = v8;
					if (v8 < v7)
					{
						v22 = mapHeightmap_11B4E0[i];
						v25 = secondHeightmap_14B4E0[i];
						v18 = v14 * ((x_DWORD)(0x10000 + (signed int)Maths::sin_DB750[0x200 + (v8 << 10) / v7]) >> 1) >> 16;
						v9 = v18 + v23;
						if (v18 + v23 > 254)
							v9 = 254;
						if (v9 > v22)
							FindFreeTileInArea_570F0(
								(unsigned __int8)i,
								HIBYTE(i),
								(unsigned int)((v9 - v22) / a1x->life_0x8) + v22,
								0,
								v15 <= v17,
								1);
						v10 = v16 + v24 - v18;
						if (v10 < 0)
							v10 = 0;
						if (v10 < v25)
							secondHeightmap_14B4E0[i] = v25 - (unsigned int)((v25 - v10) / a1x->life_0x8);
						if ((unsigned __int8)secondHeightmap_14B4E0[i] > mapHeightmap_11B4E0[i])
							mapAngle_13B4E0[i] &= 0xF7u;
						else
							mapAngle_13B4E0[i] |= 8u;
					}
					LOBYTE(v2) = v19++ + 1;
					LOBYTE(i) = i + 1;
				}
				++v20;
			}
		}
		else if (v5 == 2)
		{
			a1x->life_0x8 = 0;
		}
	}
	//return v2;
}
// 10000: using guessed type void /*__noreturn*/ sub_10000();
// DBF50: using guessed type void (/*__noreturn*/ *off_DBF50[2])();

//----- (00034EE0) --------------------------------------------------------
void UpdateVolcanoEffect_34EE0(/*signed int a1, int a2, */type_entity_0x6E8E* a3x)//215ee0
{
	int v3; // edx
	int v4; // eax
	int v5; // ecx
	int v6; // edx
	unsigned __int8 v7; // dl
	unsigned __int8 v8; // al
	__int16 v9; // dx
	unsigned int v10; // esi
	char v11; // ah
	__int16 v12; // di
	int v13; // edi
	int v14; // esi
	signed int v15; // eax
	int v16; // ecx
	unsigned __int8 v17; // al
	int v18; // ecx
	int v19; // ecx
	int v20; // eax
	int v21; // eax
	int v22; // ecx
	int v23; // ecx
	int v24; // eax
	int v25; // ecx
	unsigned __int8 v26; // dl
	char v27; // dh
	axis_3d v28x; // [esp+0h] [ebp-34h]
	//__int16 v29; // [esp+2h] [ebp-32h]
	int v30; // [esp+8h] [ebp-2Ch]
	int v31; // [esp+Ch] [ebp-28h]
	int v32; // [esp+10h] [ebp-24h]
	int v33; // [esp+14h] [ebp-20h]
	int v34; // [esp+18h] [ebp-1Ch]
	int v35; // [esp+1Ch] [ebp-18h]
	int v36; // [esp+20h] [ebp-14h]
	int v37; // [esp+24h] [ebp-10h]
	int v38; // [esp+28h] [ebp-Ch]
	unsigned int v39; // [esp+2Ch] [ebp-8h]
	unsigned __int16 i; // [esp+30h] [ebp-4h]

	signed int a1;
	int a2;

	v3 = a3x->life_0x8 - 1;
	a3x->life_0x8 = v3;
	if (v3 <= 0)
	{
		DisableEntityDrawing04_57F10(a3x);
	}
	else
	{
		v4 = a3x->axis_0x9A_154x.x;
		v5 = a3x->position_0x4C_76.y;
		v38 = 2 * v4;
		v6 = (a3x->position_0x4C_76.x + 128) >> 8;
		v36 = (v5 + 128) >> 8;
		LOBYTE(v39) = v6 - v4;
		BYTE1(v39) = v36 - v4;
		v7 = a3x->levelPar3_0x46_70;
		if (v7 < 1u)
		{
			if (!v7)
			{
				v8 = a3x->model_0x40_64;
				if (v8 >= 0x54u)
				{
					if (v8 <= 0x54u)
					{
						v37 = BYTE1(v39);
						v36 = (unsigned __int8)v39;
						a1 = GetMaxCeilingInArea_48EF0((unsigned __int8)v39, BYTE1(v39), v38, v38);
						a2 = a1 - GetMaxHeightInArea_48E90((unsigned __int8)v39, BYTE1(v39), v38, v38) - 1;
					}
					else if (v8 == 85)
					{
						v36 = BYTE1(v39);
						v37 = (unsigned __int8)v39;
						a1 = GetMinHeightInArea_48E60((unsigned __int8)v39, BYTE1(v39), v38, v38);
						a2 = GetMinCeilingInArea_48EC0((unsigned __int8)v39, BYTE1(v39), v38, v38) - a1 - 1;
					}
				}
				if (a2 <= 0)
				{
					a3x->levelPar3_0x46_70 = 2;
				}
				else
				{
					v9 = a3x->position_0x4C_76.z;
					if (v9)
					{
						v10 = (51 * v9 * a2 - (__CFSHL__(51 * v9 * a2 >> 31, 8) + (51 * v9 * a2 >> 31 << 8))) >> 8;
					}
					else
					{
						a3x->rand_0x14_20 = 9377 * a3x->rand_0x14_20 + 9439;
						v10 = a3x->rand_0x14_20 % (unsigned int)a2;
					}
					v11 = a3x->model_0x40_64;
					a3x->axis_0x9A_154x.z = a1;
					if (v11 == 84)
						v12 = a1 - v10;
					else
						v12 = v10 + a1;
					a3x->position_0x4C_76.z = v12;
					a3x->levelPar3_0x46_70 = 1;
				}
			}
		}
		else if (v7 <= 1u)
		{
			v35 = v4 << 8;
			v13 = a3x->axis_0x9A_154x.z;
			v14 = abs(a3x->position_0x4C_76.z - v13);
			v34 = 0;
			v30 = 49152 * v4 >> 8;
			for (i = v39; v34 < v38; ++HIBYTE(i))
			{
				v33 = 0;
				LOBYTE(i) = v39;
				while (v33 < v38)
				{
					v28x.x = (unsigned __int8)i << 8;
					v28x.y = HIBYTE(i) << 8;
					v15 = Maths::EuclideanDistXYZ_58490(&a3x->position_0x4C_76, &v28x);
					v31 = v15;
					if (v15 < v35)
					{
						v16 = v14 * ((x_DWORD)(0x10000 + (signed int)Maths::sin_DB750[0x200 + (v15 << 10) / v35]) >> 1);
						v17 = a3x->model_0x40_64;
						v18 = v16 >> 16;
						if (v17 >= 0x54u)
						{
							if (v17 <= 0x54u)
							{
								v19 = v13 - v18;
								if (v19 < 0)
									v19 = 0;
								v32 = i;
								v20 = (unsigned __int8)secondHeightmap_14B4E0[i];
								v37 = v20;
								v21 = (v20 - v19) / a3x->life_0x8;
								if (v37 - v21 < v37)
									secondHeightmap_14B4E0[v32] = v37 - v21;
							}
							else if (v17 == 85)
							{
								v22 = v13 + v18;
								if (v22 > 254)
									v22 = 254;
								v23 = v22 - mapHeightmap_11B4E0[i];
								v37 = mapHeightmap_11B4E0[i];
								v24 = v23 / a3x->life_0x8;
								if (v24 + v37 > v37)
									FindFreeTileInArea_570F0((unsigned __int8)i, HIBYTE(i), v24 + v37, 0, v31 <= v30, 1);
							}
						}
						v25 = i;
						v26 = mapHeightmap_11B4E0[i];
						if ((unsigned __int8)secondHeightmap_14B4E0[i] > v26)
						{
							mapAngle_13B4E0[i] &= 0xF7u;
						}
						else
						{
							v27 = mapAngle_13B4E0[i] | 8;
							secondHeightmap_14B4E0[i] = v26 - 1;
							mapAngle_13B4E0[v25] = v27;
						}
					}
					v33++;
					LOBYTE(i) = i + 1;
				}
				v34++;
			}
		}
		else if (v7 == 2)
		{
			if (a3x->model_0x40_64 == 84)
				RandomiseTerrainArea_43C60(v39, SBYTE1(v39), (unsigned __int16)v38, (unsigned __int16)v38);
			a3x->life_0x8 = 0;
		}
	}
}
// 10000: using guessed type void /*__noreturn*/ sub_10000();
// DBF50: using guessed type void (/*__noreturn*/ *off_DBF50[2])();

//----- (000352A0) --------------------------------------------------------
void HideEntity2_352A0(type_entity_0x6E8E* a1x)//2162a0
{
	DisableEntityDrawing04_57F10(a1x);
	//return 1;
}

//----- (000352C0) --------------------------------------------------------
void UpdateEffectUntilWater_352C0(type_entity_0x6E8E* a1x)//2162c0
{
	int v1; // edx

	v1 = a1x->life_0x8;
	a1x->life_0x8 = v1 - 1;
	if (v1 < 0 || GetTileAngleBit_104A0(&a1x->position_0x4C_76) & 1)
	{
		DisableEntityDrawing04_57F10(a1x);
		return;// &a1x->array_0x4C_76;
	}
	a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
	if (!SpawnAroundEntity_572C0(a1x, 0, 1024, a1x->rand_0x14_20 % 0xFu + 10, 0))
	{
		SendMailToOverlappingEntities_10C80(a1x, 0, a1x->subSpellIndex_0x2A_42);
		PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 10);
	}
	MoveEntity_57FA0(&a1x->position_0x4C_76, a1x->yaw_0x1C_28, 0, a1x->actSpeed_0x82_130);
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (00035390) --------------------------------------------------------
void UpdatePortal_35390(type_entity_0x6E8E* a1x)//216390
{
	//uint8_t* v1; // eax
	int v2; // edx
	//uint8_t* v3; // edi
	int v3x;
	type_entity_0x6E8E* v4x; // esi
	__int16 v5; // ax
	//int v6; // ST0C_4
	__int16 v7; // ax
	//uint8_t* v8; // edx
	axis_3d* v9; // [esp+8h] [ebp-8h]
	unsigned __int16 i; // [esp+Ch] [ebp-4h]

	v3x = 0;

	if (!(a1x->flags_0xc_12_15.byte[0] & 2))
	{
		a1x->flags_0xc_12_15.byte[0] |= 2u;
		PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 21);
	}
	v2 = a1x->life_0x8;
	if (v2 <= 0 || (a1x->life_0x8 = v2 - 1, v2 != 1))
	{
		//v3 = x_D41A0_BYTEARRAY_0 + 11230;
		for (i = 0; ; i++)
		{
			v9 = &a1x->position_0x4C_76;
			if (world_D41A0.NumberOfPlayers_0xe <= i)
				break;
			v4x = Entities_EA3E4[world_D41A0.players_0x2BDE[v3x].playerIndex_0x00a_2BE4_11240];
			if (EntitiesOverlap_106C0(a1x, Entities_EA3E4[world_D41A0.players_0x2BDE[v3x].playerIndex_0x00a_2BE4_11240]))
			{
				v5 = Maths::GetYawAngle_581E0(&v4x->position_0x4C_76, v9);
				if ((unsigned __int16)AngleDifference_582B0(v4x->yaw_0x1C_28, v5) < 0xAAu)
				{
					//v6 = a1x->dword_0xA0_160;
					v7 = GetTerrainAlt_10C40(&a1x->axis_0x9A_154x);
					//v8 = x_D41A0_BYTEARRAY_0;
					a1x->axis_0x9A_154x.z = a1x->typeParams_0xA0_160x->minHeight_160_0xc_12 + v7;
					PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 22);
					CopyEntityPosition_57CF0(v4x, &a1x->axis_0x9A_154x);
					SetLocalPaletteSubMode_5C800(v4x, 6);
				}
			}
			//v3 += 2124;
			v3x++;
		}
		a1x->position_0x4C_76.z = GetTerrainAlt_10C40(v9);
	}
	else
	{
		DisableEntityDrawing04_57F10(a1x);
		PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 20);
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00035530) --------------------------------------------------------
void UpdateEffectAction37_35530(type_entity_0x6E8E* a1x)//216530
{
	type_entity_0x6E8E* resultx; // eax
	__int16 v2; // ax
	__int16 v3; // ax

	resultx = Entities_EA3E4[a1x->parentId_0x28_40];
	if (resultx > Entities_EA3E4[0])
	{
		if (resultx->life_0x8 == 1)
		{
			DisableEntityDrawing04_57F10(a1x);
		}
		else
		{
			predictedAxis_EB398ar = resultx->position_0x4C_76;
			MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, a1x->pitch_0x1E_30, a1x->actSpeed_0x82_130);
			v2 = a1x->yaw_0x1C_28 + 45;
			HIBYTE(v2) &= 7u;
			a1x->yaw_0x1C_28 = v2;
			v3 = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
			HIBYTE(v3)++;
			predictedAxis_EB398ar.z = v3;
			/*result = */CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
		}
	}
	//return result;
}
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (00035600) --------------------------------------------------------
void UpdateEffect0A21_35600(type_entity_0x6E8E* a1x)//216600
{
	type_entity_0x6E8E* resultx; // eax
	int v2; // edx
	char v3; // dl

	resultx = a1x;
	resultx->param_0x10_16++;
	v2 = a1x->life_0x8;
	a1x->life_0x8 = v2 - 1;
	if (v2 < 0)
	{
		DisableEntityDrawing04_57F10(a1x);
		return;// (type_str_0x6E8E*)1;
	}
	v3 = a1x->flags_0xc_12_15.byte[0];
	if (!(v3 & 2))
		a1x->flags_0xc_12_15.byte[0] = v3 | 2;
	//return resultx;
}

//----- (00035640) --------------------------------------------------------
void UpdateThunderball_35640(type_entity_0x6E8E* a1x)//216640
{
	signed __int16 v1; // si
	__int16 v2; // dx
	int v3; // eax
	__int16 v4; // ax
	__int16 v5; // ax
	int v6; // ST08_4
	int v7; // ST04_4
	type_entity_0x6E8E* v8x; // eax
	type_entity_0x6E8E* v9x; // ecx
	__int16 v10; // ax
	signed int v11; // [esp+0h] [ebp-4h]

	v1 = 0;
	v2 = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
	if (a1x->position_0x4C_76.z < v2 + 1024)
	{
		v1 = 1;
		a1x->position_0x4C_76.z += 64;
	}
	if (a1x->position_0x4C_76.z > v2 + 1024)
	{
		HIBYTE(v2) += 4;
		v1 = 1;
		a1x->position_0x4C_76.z = v2;
	}
	if (!v1)
	{
		v3 = a1x->life_0x8;
		a1x->life_0x8 = v3 - 1;
		if (v3 >= 0)
		{
			v4 = 9377 * a1x->rand_0x14_20;
			a1x->pitch_0x1E_30 = 56;
			a1x->rand_0x14_20 = v4 + 9439;
			v11 = 0;
			a1x->yaw_0x1C_28 = a1x->rand_0x14_20 & 0x7FF;
			do
			{
				v5 = a1x->yaw_0x1C_28;
				HIBYTE(v5) = (HIBYTE(v5) + 4) & 7;
				a1x->yaw_0x1C_28 = v5;
				predictedAxis_EB398ar = a1x->position_0x4C_76;
				v6 = a1x->levelPar2_0x44_68;
				v7 = a1x->levelPar1_0x43_67;
				predictedAxis_EB398ar.z += a1x->params_0x52_82.yaw;
				v8x = CreateEntity_4A190(&a1x->position_0x4C_76, v7, v6);
				v9x = v8x;
				if (v8x)
				{
					v8x->id_0x1A_26 = a1x->id_0x1A_26;
					v8x->life_0x8 /= 3;
					v8x->yaw_0x1C_28 = a1x->yaw_0x1C_28;
					v8x->pitch_0x1E_30 = a1x->pitch_0x1E_30;
					v10 = a1x->subSpellIndex_0x2A_42;
					v9x->levelPar1_0x43_67 = 10;
					v9x->levelPar2_0x44_68 = 23;
					v9x->subSpellIndex_0x2A_42 = v10;
					if (!v11)
						PrepareEventSound_6E450(v9x - world_D41A0.entities_0x6E8E, -1, 23);
				}
				v11++;
			} while (v11 < 2);
		}
		else
		{
			DisableEntityDrawing04_57F10(a1x);
		}
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (000357C0) --------------------------------------------------------
void UpdateSkeletonSpawn_357C0(type_entity_0x6E8E* a1x)//2167c0
{
	signed __int16 v1; // bx
	unsigned __int16 v2; // cx
	type_entity_0x6E8E* ix; // eax
	int v4; // eax
	type_entity_0x6E8E* v5x; // ecx
	int v7; // [esp+0h] [ebp-10h]
	unsigned __int16 v8; // [esp+4h] [ebp-Ch]
	__int16 v9; // [esp+8h] [ebp-8h]
	int v10; // [esp+Ch] [ebp-4h]

	a1x->subSpellIndex_0x2A_42 = 10000;
	if (a1x->subSpellIndex_0x2A_42)
	{
		v1 = GetFreeEntityCount_4A810();
		if (v1 < 0)
			v1 = 0;
		if (v1 > 8)
			v1 = 8;
		v2 = 0;
		for (ix = gameState_D41A4.creatureLists_38403x[36 / 4]; ix > Entities_EA3E4[0]; ix = ix->next_0)
		{
			if (ix->playerEntityIndex_0x94_148 == a1x->id_0x1A_26)
				v2++;
		}
		if (v1 < 0)
			v1 = 0;
		if (v1 > 64 - v2)
			v1 = 64 - v2;
		if (v1 > 0)
		{
			HIWORD(v4) = HIWORD(a1x);
			LOWORD(v4) = a1x->subSpellIndex_0x2A_42;
			v7 = v4;
			v10 = (signed __int16)v4 / v1;
			v9 = 2048 / v1;
			v8 = 0;
			while (v1 > 0)
			{
				predictedAxis_EB398ar = a1x->position_0x4C_76;
				MoveEntity_57FA0(&predictedAxis_EB398ar, v8, 0, 512);
				v5x = CreateEntity_4A190(&predictedAxis_EB398ar, 5, 9);
				if (v5x)
				{
					v5x->mana_0x90_144 = (signed __int16)v7 % (signed __int16)v10;
					v5x->playerEntityIndex_0x94_148 = a1x->id_0x1A_26;
					v5x->id_0x1A_26 = a1x->id_0x1A_26;
					v5x->roll_0x20_32 = __PAIR__(HIBYTE(v8) + 4, (unsigned __int8)v8) & 0x7FF;
					v5x->yaw_0x1C_28 = __PAIR__(HIBYTE(v8) + 4, (unsigned __int8)v8) & 0x7FF;
					v7 -= v10;
				}
				v1--;
				v8 += v9;
			}
		}
	}
	DisableEntityDrawing04_57F10(a1x);
}

int debugcounter_216940 = 0;

//----- (00035940) --------------------------------------------------------
void TransformArcherToMana_35940(type_entity_0x6E8E* a1x)//216940 //move mana sphere
{
	//char v1; // dh
	//__int16 v2; // dx
	//__int16 v3; // ax
	//__int16 v4; // ax
	//unsigned __int16 v5; // si
	//__int16 v6; // ST0C_2
	//unsigned __int16 v7; // ST04_2
	type_entity_0x6E8E* v8x; // eax
	char v9; // dl
	unsigned __int16 v10; // ax
	signed int v11; // eax
	type_entity_0x6E8E* v12x; // eax
	//__int16 v13; // si
	__int16 v14; // ax
	//__int16 v15; // cx
	//__int16 v16; // ax
	//__int16 v17; // t0
	signed int v18; // eax
	//__int16 v19; // ax
	//__int16 v20; // dx
	//__int16 v21; // cx
	__int16 v22; // si
	int v23; // eax
	__int16 v24; // cx
	type_entity_0x6E8E* v25x; // eax
	int v26; // eax
	//int v27; // edx
	unsigned int v28; // eax
	char v29; // cl
	int v30; // [esp+0h] [ebp-1Ch]
	unsigned __int16 v31; // [esp+4h] [ebp-18h]
	signed int v32; // [esp+8h] [ebp-14h]
	signed __int16 v33; // [esp+Ch] [ebp-10h]
	char v34; // [esp+10h] [ebp-Ch]
	char v35; // [esp+14h] [ebp-8h]
	char v36; // [esp+18h] [ebp-4h]

	if (debugcounter_216940 >= 0x12) {
		debugcounter_216940++;
		debugcounter_216940--;
	}

	//v1 = a1x->flags_0xc_12_15.byte[1];
	v35 = 0;
	v36 = 0;
	if (a1x->flags_0xc_12_15.byte[1] & 8)
	{
		a1x->flags_0xc_12_15.byte[1] &= 0xF7;
	}
	else
	{
		//v2 = a1x->mail_0x5E_94.word_0x68_104;
		if (a1x->mail_0x5E_94.channel[1].source)
		{
			if (a1x->mail_0x5E_94.channel[1].source != a1x->playerEntityIndex_0x94_148)
			{
				v36 = 1;
				if (a1x->mail_0x5E_94.channel[1].amount.dword)
				{
					//v4 = a1x->mail_0x5E_94.word_0x68_104;
					a1x->playerEntityIndex_0x94_148 = a1x->mail_0x5E_94.channel[1].source;
					PrepareEventSound_6E450(a1x->mail_0x5E_94.channel[1].source, -1, 4);
					//a1x->byte_0xc_12 &= 0xFFDFFFBF;
					a1x->flags_0xc_12_15.dword &= 0xFFDFFFBF;
					/*a1x->byte_0xc_12 &= 0xBF;
					a1x->byte_0xe_14 &= 0xDF;*/
					a1x->flags_0xc_12_15.byte[2] |= 0x20u;
				}
				else if (!(a1x->flags_0xc_12_15.byte[2] & 0x20))
				{
					//v3 = a1x->mail_0x5E_94.word_0x68_104;
					a1x->playerEntityIndex_0x94_148 = a1x->mail_0x5E_94.channel[1].source;
					PrepareEventSound_6E450(a1x->mail_0x5E_94.channel[1].source, -1, 4);
					a1x->flags_0xc_12_15.byte[0] &= 0xBFu;
				}
			}
			a1x->mail_0x5E_94.channel[1].source = 0;
			a1x->mail_0x5E_94.channel[1].amount.dword = 0;
		}
		//v5 = a1x->mail_0x5E_94.word_0x7A_122;
		if (a1x->mail_0x5E_94.channel[4].source)
		{
			v35 = 1;
			a1x->yaw_0x1C_28 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &Entities_EA3E4[a1x->mail_0x5E_94.channel[4].source]->position_0x4C_76);
			//v6 = a1x->mail_0x5E_94.word_0x76_118;
			//v7 = a1x->word_0x1C_28;
			predictedAxis_EB398ar.x = 0;
			predictedAxis_EB398ar.y = 0;
			predictedAxis_EB398ar.z = 0;
			MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, 0, a1x->mail_0x5E_94.channel[4].amount.word[0]);
			a1x->axis_0x9A_154x.x = predictedAxis_EB398ar.x;
			a1x->axis_0x9A_154x.y = predictedAxis_EB398ar.y;
			a1x->mail_0x5E_94.channel[4].source = 0;
		}
		if (a1x->flags_0xc_12_15.byte[0] & 0x40)
		{
			v8x = Entities_EA3E4[a1x->targetId_0x96_150];
			v9 = 1;
			if (v8x->class_0x3F_63 != 3 || 3 != v8x->model_0x40_64)
			{
				if (v8x->class_0x3F_63 == 5 && v8x->model_0x40_64 == 23)
				{
					v9 = 0;
					v33 = v8x->param2_0x2C_44;
				}
			}
			else
			{
				v9 = 0;
				v33 = 32;
			}
			if (v9)
			{
				a1x->flags_0xc_12_15.byte[0] &= 0xBFu;
			}
			else
			{
				v10 = a1x->targetId_0x96_150;
				a1x->param2_0x2C_44 = 128;
				a1x->yaw_0x1C_28 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &Entities_EA3E4[v10]->position_0x4C_76);
				v11 = Maths::EuclideanDistXYZ_58490(&a1x->position_0x4C_76, &Entities_EA3E4[a1x->targetId_0x96_150]->position_0x4C_76);
				if (v11 <= 1024)
				{
					if (v11 >= 16)
					{
						predictedAxis_EB398ar = a1x->position_0x4C_76;
						MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, 0, 16);
					}
					else
					{
						predictedAxis_EB398ar = a1x->position_0x4C_76;
						v12x = Entities_EA3E4[a1x->targetId_0x96_150];
						predictedAxis_EB398ar.x = v12x->position_0x4C_76.x;
						//v13 = v12x->array_0x4C_76.z;
						predictedAxis_EB398ar.y = v12x->position_0x4C_76.y;
						if (predictedAxis_EB398ar.z >= v12x->position_0x4C_76.z)
						{
							if (predictedAxis_EB398ar.z > v12x->position_0x4C_76.z + 512)
								predictedAxis_EB398ar.z -= v33;
						}
						else
						{
							predictedAxis_EB398ar.z += v33;
						}
					}
					v14 = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
					if (v14 > predictedAxis_EB398ar.z)
						predictedAxis_EB398ar.z = v14;
					CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
				}
				else
				{
					a1x->flags_0xc_12_15.byte[0] &= 0xBFu;
				}
			}
		}
		else if (a1x->timer_0x39_57 || v35)
		{
			if ((int16_t)a1x->axis_0x9A_154x.x < -64)
				a1x->axis_0x9A_154x.x = -64;
			if ((int16_t)a1x->axis_0x9A_154x.x > 64)
				a1x->axis_0x9A_154x.x = 64;
			if ((int16_t)a1x->axis_0x9A_154x.y < -64)
				a1x->axis_0x9A_154x.y = -64;
			if ((int16_t)a1x->axis_0x9A_154x.y > 64)
				a1x->axis_0x9A_154x.y = 64;
			predictedAxis_EB398ar = a1x->position_0x4C_76;
			//v15 = a1x->axis_0x9A_154x.y;
			predictedAxis_EB398ar.x = (int16_t)a1x->axis_0x9A_154x.x + a1x->position_0x4C_76.x;
			predictedAxis_EB398ar.y = (int16_t)a1x->axis_0x9A_154x.y + a1x->position_0x4C_76.y;
			//v16 = a1x->word_0x2C_44;
			predictedAxis_EB398ar.z += a1x->param2_0x2C_44;
			a1x->param2_0x2C_44 -= 16;
			if (a1x->param2_0x2C_44 < -128)
				a1x->param2_0x2C_44 = -128;
			if (isCaveLevel_D41B6 && IsTouchingCeiling_11E70(a1x, &predictedAxis_EB398ar))
			{
				if (v35)
				{
					predictedAxis_EB398ar = a1x->position_0x4C_76;
				}
				else
				{
					//v17 = a1x->word_0x82_130;
					a1x->actSpeed_0x82_130 = 256;
					v32 = 170;
					v34 = 0;
					v30 = 1;
					while (v32 <= 1024)
					{
						predictedAxis_EB398ar = a1x->position_0x4C_76;
						v31 = (v30 * v32 + a1x->yaw_0x1C_28 + 512) & 0x7FF;
						MoveEntity_57FA0(&predictedAxis_EB398ar, v31, 0, a1x->actSpeed_0x82_130);
						predictedAxis_EB398ar.z = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
						if (!IsTouchingCeiling_11E70(a1x, &predictedAxis_EB398ar))
						{
							v34 = 1;
							break;
						}
						v30 = -v30;
						if (v30 == 1)
							v18 = 170;
						else
							v18 = 0;
						v32 += v18;
					}
					if (v34)
					{
						a1x->roll_0x20_32 = v31;
						a1x->yaw_0x1C_28 = v31;
					}
					else
					{
						a1x->axis_0x9A_154x.x = 64;
						//v19 = a1x->array_0x4C_76.x;
						//v20 = a1x->axis_0x9A_154x.x;
						a1x->axis_0x9A_154x.y = 64;
						//v21 = a1x->axis_0x9A_154x.y;
						predictedAxis_EB398ar.x = (int16_t)a1x->axis_0x9A_154x.x + a1x->position_0x4C_76.x;
						predictedAxis_EB398ar.y = (int16_t)a1x->axis_0x9A_154x.y + a1x->position_0x4C_76.y;
					}
				}
				a1x->param2_0x2C_44 = -128;
			}
			v22 = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
			if (v22 > (int16_t)predictedAxis_EB398ar.z)
			{
				int cftemp = 0;
				if ((a1x->param2_0x2C_44 >> 30) & 1)cftemp = 1;
				v23 = -(((a1x->param2_0x2C_44 - ((a1x->param2_0x2C_44 >> 31) << 2)) - cftemp) >> 2);
				/*v23 = -((a1x->word_0x2C_44
					- (__CFSHL__(a1x->word_0x2C_44 >> 31, 2)
						+ 4 * (a1x->word_0x2C_44 >> 31))) >> 2);*/
				a1x->param2_0x2C_44 = v23;
				if ((signed __int16)v23 <= 16)
					a1x->param2_0x2C_44 = 0;
				predictedAxis_EB398ar.z = v22;
			}
			if (isCaveLevel_D41B6)
			{
				v24 = GetCeilingAlt_10C60(&predictedAxis_EB398ar) - a1x->params_0x52_82.fov;
				if (v24 < (int16_t)predictedAxis_EB398ar.z)
				{
					a1x->param2_0x2C_44 = -abs(a1x->param2_0x2C_44);
					predictedAxis_EB398ar.z = v24;
				}
			}
			CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
			if (v22 == predictedAxis_EB398ar.z)
			{
				v25x = FindOverlappingEntityOfType_10A50(a1x);
				if (v25x && !(a1x->flags_0xc_12_15.byte[1] & 0x20))
					MergeManaSpheres_36D50(a1x, v25x);
				GetTerrainHeightAt_58030(&a1x->position_0x4C_76, &predictedAxis_EB398ar);
				a1x->axis_0x9A_154x.x += predictedAxis_EB398ar.x;
				int cftemp = 0;
				if ((250 * (int16_t)a1x->axis_0x9A_154x.x >> 30) & 1)cftemp = 1;
				v26 = (((250 * (int16_t)a1x->axis_0x9A_154x.x - ((250 * (int16_t)a1x->axis_0x9A_154x.x >> 31) << 8)) - cftemp) >> 8);
				/*v26 = (250 * (int16_t)a1x->axis_0x9A_154x.x
					- (__CFSHL__(250 * (int16_t)a1x->axis_0x9A_154x.x >> 31, 8)
						+ (250 * (int16_t)a1x->axis_0x9A_154x.x >> 31 << 8))) >> 8;*/
				a1x->axis_0x9A_154x.y += predictedAxis_EB398ar.y;
				//v27 = (int16_t)a1x->axis_0x9A_154x.y;
				a1x->axis_0x9A_154x.x = v26;
				cftemp = 0;
				if ((250 * (int16_t)a1x->axis_0x9A_154x.y >> 30) & 1)cftemp = 1;
				//a1x->axis_0x9A_154x.y = (250 * v27 - (__CFSHL__(250 * v27 >> 31, 8) + (250 * v27 >> 31 << 8))) >> 8;
				a1x->axis_0x9A_154x.y = (((250 * (int16_t)a1x->axis_0x9A_154x.y - ((250 * (int16_t)a1x->axis_0x9A_154x.y >> 31) << 8)) - cftemp) >> 8);
			}
			if (!(a1x->flags_0xc_12_15.byte[1] & 0x20) || v36)
				SetManaSphereColorAndRot_36920(/*a1x,*/ a1x);
		}
		if (a1x->flags_0xc_12_15.byte[1] & 0x20)
		{
			v28 = a1x->life_0x8 - 1;
			a1x->life_0x8 = v28;
			if (v28 < 6)
			{
				if (!v28)
					DisableEntityDrawing04_57F10(a1x);
			}
			else if (v28 <= 6)
			{
				v29 = a1x->flags_0xc_12_15.byte[3] & 0xFE;
				a1x->flags_0xc_12_15.byte[2] |= 0x80u;
				a1x->flags_0xc_12_15.byte[3] = v29;
			}
			else if (v28 == 12)
			{
				a1x->flags_0xc_12_15.byte[3] |= 1u;
			}
		}
	}
	debugcounter_216940++;
}
// D41B6: using guessed type char x_BYTE_D41B6;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39A: using guessed type __int16 x_WORD_EB398ar[1];
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

void UpdateManaSphere_35FB0(type_entity_0x6E8E* a1x)//216FB0
{
	char v1; // dl
	unsigned __int16 v2; // ax
	//int v3; // ST0C_4
	//char v4; // ST04_1
	unsigned __int16 v5; // ax
	type_entity_0x6E8E* v6x; // eax
	char v7; // dl
	unsigned __int16 v8; // ax
	signed int v9; // eax
	type_entity_0x6E8E* v10x; // eax
	__int16 v11; // si
	__int16 v12; // ax
	__int16 v13; // si
	__int16 v14; // si
	__int16 v15; // ax
	type_entity_0x6E8E* v16x; // eax
	__int16 v17; // si
	int v18; // eax
	//__int16 v19; // cx
	__int16 v20; // ax
	__int16 v21; // ax
	__int16 v22; // dx
	__int16 v23; // cx
	__int16 v24; // si
	__int16 v24b; // cx
	int v25; // eax
	//int v26; // eax
	type_entity_0x6E8E* v27x; // eax
	int v28; // eax
	int v29; // edx
	signed __int16 v30; // [esp+0h] [ebp-8h]
	char v31; // [esp+4h] [ebp-4h]

	if (CommandLineParams.DoDebugSequences()) {
		//add_compare(0x216FB4, CommandLineParams.DoDebugafterload());
	}

	v1 = a1x->flags_0xc_12_15.byte[1];
	v31 = 0;
	if (v1 & 8)
	{
		a1x->flags_0xc_12_15.byte[1] = v1 & 0xF7;
	}
	else if (a1x->mail_0x5E_94.channel[1].source && ManaSphereCheckCollector_36680(a1x))
	{
		CreateEntity_4A190(&a1x->position_0x4C_76, 10, 0);
		DisableEntityDrawing04_57F10(a1x);
	}
	else
	{
		if (a1x->mail_0x5E_94.channel[4].source)
		{
			v2 = a1x->mail_0x5E_94.channel[4].source;
			a1x->actSpeed_0x82_130 = 0;
			v31 = 1;
			a1x->yaw_0x1C_28 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &Entities_EA3E4[v2]->position_0x4C_76);
			//v3 = a1x->mail_0x5E_94.word_0x76_118;
			//v4 = a1x->word_0x1C_28;
			predictedAxis_EB398ar.x = 0;
			predictedAxis_EB398ar.y = 0;
			predictedAxis_EB398ar.z = 0;
			MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, 0, a1x->mail_0x5E_94.channel[4].amount.word[0]);
			a1x->axis_0x9A_154x.x = predictedAxis_EB398ar.x;
			a1x->axis_0x9A_154x.y = predictedAxis_EB398ar.y;
			a1x->mail_0x5E_94.channel[4].source = 0;
		}
		if (a1x->flags_0xc_12_15.byte[0] & 0x40)
		{
			v5 = a1x->targetId_0x96_150;
			a1x->actSpeed_0x82_130 = 0;
			v6x = Entities_EA3E4[v5];
			v7 = 1;
			if (v6x->class_0x3F_63 != 3 || 3 != v6x->model_0x40_64)
			{
				if (v6x->class_0x3F_63 == 5 && v6x->model_0x40_64 == 23)
				{
					v7 = 0;
					v30 = v6x->param2_0x2C_44;
				}
			}
			else
			{
				v7 = 0;
				v30 = 32;
			}
			if (v7)
			{
				a1x->flags_0xc_12_15.byte[0] &= 0xBFu;
			}
			else
			{
				v8 = a1x->targetId_0x96_150;
				a1x->param2_0x2C_44 = 128;
				a1x->yaw_0x1C_28 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &Entities_EA3E4[v8]->position_0x4C_76);
				v9 = Maths::EuclideanDistXYZ_58490(&a1x->position_0x4C_76, &Entities_EA3E4[a1x->targetId_0x96_150]->position_0x4C_76);
				if (v9 <= 1024)
				{
					if (v9 >= 16)
					{
						predictedAxis_EB398ar = a1x->position_0x4C_76;
						//*(_DWORD*)& word_EB398 = *(_DWORD*)(a1 + 76);
						//*(&word_EB398 + 2) = *(_WORD*)(a1 + 80);
						MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, 0, 16);
					}
					else
					{
						predictedAxis_EB398ar = a1x->position_0x4C_76;
						//*(_DWORD*)& word_EB398 = *(_DWORD*)(a1 + 76);
						//*(&word_EB398 + 2) = *(_WORD*)(a1 + 80);
						v10x = Entities_EA3E4[a1x->targetId_0x96_150];
						//word_EB398 = v10[38];
						//v11 = v10[40];
						//word_EB39A = v10[39];
						predictedAxis_EB398ar.x = v10x->position_0x4C_76.x;
						predictedAxis_EB398ar.y = v10x->position_0x4C_76.y;
						v11 = v10x->position_0x4C_76.z;
						if (predictedAxis_EB398ar.z >= v11)
						{
							if (predictedAxis_EB398ar.z > v11 + 512)
								predictedAxis_EB398ar.z -= v30;
						}
						else
						{
							predictedAxis_EB398ar.z += v30;
						}
					}
					v12 = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
					if (v12 > predictedAxis_EB398ar.z)
						predictedAxis_EB398ar.z = v12;
					CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
				}
				else
				{
					a1x->flags_0xc_12_15.byte[0] &= 0xBFu;
				}
			}
		}
		else
		{
			v13 = a1x->actSpeed_0x82_130;
			if (v13)
			{
				if (v13 <= 0)
				{
					if (v13 < -4)
						a1x->actSpeed_0x82_130 = v13 + 4;
				}
				else if (v13 > 4)
				{
					a1x->actSpeed_0x82_130 = v13 - 4;
				}
				//*(_DWORD*)& word_EB398 = *(_DWORD*)(a1 + 76);
				//*(&word_EB398 + 2) = *(_WORD*)(a1 + 80);
				predictedAxis_EB398ar = a1x->position_0x4C_76;
				MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, a1x->pitch_0x1E_30, a1x->actSpeed_0x82_130);
				v14 = a1x->param2_0x2C_44 - 16;
				a1x->param2_0x2C_44 = v14;
				if (v14 < -128)
					a1x->param2_0x2C_44 = -128;
				predictedAxis_EB398ar.z += a1x->param2_0x2C_44;
				if (isCaveLevel_D41B6 && (unsigned __int8)IsTouchingCeiling_11E70(a1x, &predictedAxis_EB398ar))
				{
					//*(_DWORD*)& word_EB398 = *(_DWORD*)(a1 + 76);
					//*(&word_EB398 + 2) = *(_WORD*)(a1 + 80);
					predictedAxis_EB398ar = a1x->position_0x4C_76;
					a1x->actSpeed_0x82_130 = 0;
					a1x->param2_0x2C_44 = -128;
				}
				else
				{
					CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
				}
				v15 = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
				if (v15 <= (int16_t)predictedAxis_EB398ar.z)
				{
					if (isCaveLevel_D41B6)
					{
						v17 = GetCeilingAlt_10C60(&predictedAxis_EB398ar) - a1x->params_0x52_82.fov;
						if (v17 < predictedAxis_EB398ar.z)
						{
							a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
							v18 = (a1x->rand_0x14_20 & 3) - 2;
							a1x->actSpeed_0x82_130 = v18;
							if (!(_WORD)v18)
								a1x->actSpeed_0x82_130 = 1;
							a1x->param2_0x2C_44 = -abs(a1x->param2_0x2C_44);
							predictedAxis_EB398ar.z = v17;
						}
					}
				}
				else
				{
					a1x->actSpeed_0x82_130 = 0;
					a1x->position_0x4C_76.z = v15;
					if (TerrainTileIsWater_104D0(&a1x->position_0x4C_76) == 1)
					{
						a1x->param2_0x2C_44 = 0;
						v16x = CreateEntity_4A190(&a1x->position_0x4C_76, 10, 5);
						if (v16x)
							PrepareEventSound_6E450(v16x - world_D41A0.entities_0x6E8E, -1, 27);
					}
					else
					{
						a1x->param2_0x2C_44 = 128;
					}
				}
			}
			else if (a1x->timer_0x39_57 || v31)
			{
				if ((int16_t)a1x->axis_0x9A_154x.x < -64)
					a1x->axis_0x9A_154x.x = -64;
				if ((int16_t)a1x->axis_0x9A_154x.x > 64)
					a1x->axis_0x9A_154x.x = 64;
				if ((int16_t)a1x->axis_0x9A_154x.y < -64)
					a1x->axis_0x9A_154x.y = -64;
				if ((int16_t)a1x->axis_0x9A_154x.y > 64)
					a1x->axis_0x9A_154x.y = 64;
				//*(_DWORD*)& word_EB398 = *(_DWORD*)(a1 + 76);
				//*(&word_EB398 + 2) = *(_WORD*)(a1 + 80);
				predictedAxis_EB398ar = a1x->position_0x4C_76;
				//v19 = a1x->axis_0x9A_154x.y;
				predictedAxis_EB398ar.x = a1x->axis_0x9A_154x.x + a1x->position_0x4C_76.x;
				predictedAxis_EB398ar.y = a1x->axis_0x9A_154x.y + a1x->position_0x4C_76.y;
				v20 = a1x->param2_0x2C_44;
				predictedAxis_EB398ar.z += v20;
				a1x->param2_0x2C_44 = v20 - 16;
				if ((int16_t)(v20 - 16) < -128)
					a1x->param2_0x2C_44 = -128;
				if (isCaveLevel_D41B6 && (unsigned __int8)IsTouchingCeiling_11E70(a1x, &predictedAxis_EB398ar))
				{
					if (v31)
					{
						//*(_DWORD*)& word_EB398 = *(_DWORD*)(a1 + 76);
						//*(&word_EB398 + 2) = *(_WORD*)(a1 + 80);
						predictedAxis_EB398ar = a1x->position_0x4C_76;
					}
					else
					{
						a1x->axis_0x9A_154x.x = 64;
						v21 = a1x->position_0x4C_76.x;
						v22 = a1x->axis_0x9A_154x.x;
						a1x->axis_0x9A_154x.y = 64;
						v23 = a1x->axis_0x9A_154x.y;
						predictedAxis_EB398ar.x = v22 + v21;
						predictedAxis_EB398ar.y = v23 + a1x->position_0x4C_76.y;
					}
					a1x->param2_0x2C_44 = -128;
				}
				v24 = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
				if (v24 > (int16_t)predictedAxis_EB398ar.z)
				{
					/*v25 = -((signed int)(a1x->word_0x2C_44
						- (__CFSHL__(a1x->word_0x2C_44 >> 32, 2)
							+ 4 * (a1x->word_0x2C_44 >> 32))) >> 2);*/
					v25 = -((a1x->param2_0x2C_44 - (my_sign32(a1x->param2_0x2C_44) * 4) + my_sign32(a1x->param2_0x2C_44)) >> 2);
					a1x->param2_0x2C_44 = v25;
					if ((signed __int16)v25 <= 16)
						a1x->param2_0x2C_44 = 0;
					predictedAxis_EB398ar.z = v24;
				}
				if (isCaveLevel_D41B6)
				{
					v24b = GetCeilingAlt_10C60(&predictedAxis_EB398ar) - a1x->params_0x52_82.fov;//0x21757A mov ecx, eax
					if (v24b < predictedAxis_EB398ar.z)//0x21758C cmp cx, di
					{
						a1x->param2_0x2C_44 = -abs(a1x->param2_0x2C_44);
						predictedAxis_EB398ar.z = v24b;//0x2175A0 mov word_EB39C, cx
					}
				}
				CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
				if (v24 == predictedAxis_EB398ar.z)//0x2175BB cmp si, ax (esi = terrain alt)
				{
					v27x = FindOverlappingEntityOfType_10A50(a1x);
					if (v27x)
					{
						ManaSphereCollide_36F30(a1x, v27x);
						SetManaSphereColorAndRot_36920(a1x);
					}
					GetTerrainHeightAt_58030(&a1x->position_0x4C_76, &predictedAxis_EB398ar);
					a1x->axis_0x9A_154x.x += predictedAxis_EB398ar.x;
					/*v28 = (250 * a1x->axis_0x9A_154x.x
						- (__CFSHL__(250 * a1x->axis_0x9A_154x.x >> 31, 8)
							+ (250 * a1x->axis_0x9A_154x.x >> 31 << 8))) >> 8;*/
					v28 = ((250 * (int16_t)a1x->axis_0x9A_154x.x) - (my_sign32(250 * (int16_t)a1x->axis_0x9A_154x.x) * 256) + my_sign32(250 * (int16_t)a1x->axis_0x9A_154x.x)) >> 8;
					a1x->axis_0x9A_154x.y += predictedAxis_EB398ar.y;
					v29 = a1x->axis_0x9A_154x.y;
					a1x->axis_0x9A_154x.x = v28;
					//a1x->axis_0x9A_154x.y = (250 * v29 - (__CFSHL__(250 * v29 >> 31, 8) + (250 * v29 >> 31 << 8))) >> 8;
					a1x->axis_0x9A_154x.y = ((250 * (int16_t)v29) - (my_sign32(250 * (int16_t)v29) * 256) + my_sign32(250 * (int16_t)v29)) >> 8;
				}
			}
		}
	}
}

//----- (00036680) --------------------------------------------------------
char ManaSphereCheckCollector_36680(type_entity_0x6E8E* a1x)//217680
{
	unsigned __int8 v1; // al
	signed int v2; // eax
	int v3; // eax
	char v5; // [esp+0h] [ebp-4h]

	v5 = 0;
	if (a1x->parentId_0x28_40 == a1x->mail_0x5E_94.channel[1].source)
	{
		a1x->mail_0x5E_94.channel[1].source = 0;
		a1x->mail_0x5E_94.channel[1].amount.dword = 0;
	}
	else
	{
		v1 = a1x->levelPar3_0x46_70;
		if (v1 < 1u)
		{
			if (!v1)
			{
				ManaSphereShootFireball_36770(a1x);
				SendSpellMessage_6D8B0(a1x->parentId_0x28_40, 0x16u, 1);
				v5 = 1;
			}
		}
		else if (v1 <= 1u)
		{
			v2 = a1x->param_0x10_16;
			a1x->param_0x10_16 = v2 + 1;
			if (v2 >= 8)
			{
				SendSpellMessage_6D8B0(a1x->parentId_0x28_40, 0x16u, 1);
				return 1;
			}
			if (!(a1x->param_0x10_16 & 1))
				ManaSphereShootFireball_36770(a1x);
		}
		else if (v1 <= 3u)
		{
			v3 = a1x->param_0x10_16;
			a1x->param_0x10_16 = v3 + 1;
			if (!v3)
			{
				ManaSphereShootProjectile9_36850(a1x);
				return 0;
			}
			if (v3 + 1 > 2)
			{
				SendSpellMessage_6D8B0(a1x->parentId_0x28_40, 0x16u, 1);
				return 1;
			}
		}
	}
	return v5;
}

//----- (00036770) --------------------------------------------------------
void ManaSphereShootFireball_36770(type_entity_0x6E8E* a1x)//217770
{
	type_entity_0x6E8E* v1x; // ebx
	type_entity_0x6E8E* v2x; // ST08_4
	type_entity_0x6E8E* v3x; // eax

	v1x = CreateEntity_4A190(&a1x->position_0x4C_76, 9, 0);
	if (v1x)
	{
		v2x = Entities_EA3E4[a1x->mail_0x5E_94.channel[1].source];
		v1x->targetId_0x96_150 = a1x->mail_0x5E_94.channel[1].source;
		AimAtEntity_655C0(v1x, v2x);
		v1x->yaw_0x1C_28 = v1x->roll_0x20_32;
		v1x->pitch_0x1E_30 = v1x->fov_0x22_34;
		v1x->position_0x4C_76.z += a1x->params_0x52_82.fov;
		v1x->subSpellIndex_0x2A_42 = a1x->subSpellIndex_0x2A_42;
		PrepareEventSound_6E450(v1x - world_D41A0.entities_0x6E8E, -1, 9);
		if (TerrainTileIsWater_104D0(&v1x->position_0x4C_76) == 1)
		{
			v3x = CreateEntity_4A190(&a1x->position_0x4C_76, 10, 5);
			if (v3x)
				PrepareEventSound_6E450(v3x - world_D41A0.entities_0x6E8E, -1, 27);
		}
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00036850) --------------------------------------------------------
void ManaSphereShootProjectile9_36850(type_entity_0x6E8E* a1x)//217850
{
	type_entity_0x6E8E* v1x; // eax
	type_entity_0x6E8E* v2x; // ebx
	type_entity_0x6E8E* v3x; // esi

	predictedAxis_EB398ar = a1x->position_0x4C_76;
	v1x = CreateEntity_4A190(&predictedAxis_EB398ar, 9, 9);
	v2x = v1x;
	if (v1x)
	{
		v3x = Entities_EA3E4[a1x->mail_0x5E_94.channel[1].source];
		v1x->levelPar1_0x43_67 = 10;
		v1x->levelPar2_0x44_68 = 23;
		v1x->id_0x1A_26 = a1x->id_0x1A_26;
		v1x->targetId_0x96_150 = a1x->mail_0x5E_94.channel[1].source;
		AimAtEntity_655C0(v1x, v3x);
		//LOWORD(v1) = *(x_WORD *)(v2 + 32);
		v2x->typeParams_0xA0_160x = &str_D7BD6[64]; //(type_str_160*)&unk_D7BD6[0x880];
		v2x->yaw_0x1C_28 = v2x->roll_0x20_32;
		v2x->pitch_0x1E_30 = v2x->fov_0x22_34;
		v2x->xsubtype_0x42_66 = v3x->model_0x40_64;
		//LOBYTE(v1) = *(x_BYTE *)(v3 + 63);
		v2x->xtype_0x41_65 = v3x->class_0x3F_63;
		v2x->position_0x4C_76.z += a1x->params_0x52_82.fov;
		v2x->subSpellIndex_0x2A_42 = a1x->subSpellIndex_0x2A_42;
	}
	//return v1;
}
// D8456: using guessed type void *off_D8456;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;

//----- (00036920) --------------------------------------------------------
void SetManaSphereColorAndRot_36920(type_entity_0x6E8E* entity)//217920
{
	int sphereSize;
	for (sphereSize = 0; sphereSize < 7 && entity->mana_0x90_144 > manaSphereSizeTable_DB538[sphereSize]; sphereSize++)
		;
	int colorIndex = GetManaSphereColorIndexFromEntityId_369F0(entity->playerEntityIndex_0x94_148);
	if (colorIndex + sphereSize != entity->spriteIndex_0x5A_90)
	{
		SetEntityIndex_49C90(entity, sphereSize + colorIndex);
		int16_t rotation;
		switch (sphereSize)
		{
		case 0:
			rotation = 13;
			break;
		case 1:
			rotation = 28;
			break;
		case 2:
			rotation = 42;
			break;
		case 3:
			rotation = 56;
			break;
		case 4:
			rotation = 70;
			break;
		case 5:
			rotation = 84;
			break;
		case 6:
			rotation = 98;
			break;
		case 7:
			rotation = 112;
			break;
		default:
			break;
		}
		entity->params_0x52_82.pitch = rotation;
		entity->params_0x52_82.roll = rotation;
		entity->params_0x52_82.fov = rotation;
		entity->params_0x52_82.yaw = rotation;
	}
}

//----- (000369F0) --------------------------------------------------------
int GetManaSphereColorIndexFromEntityId_369F0(int playerEntityIndex)//2179f0
{
	int result;
	type_entity_0x6E8E* playerEntity = Entities_EA3E4[playerEntityIndex];
	if (playerEntity <= Entities_EA3E4[0] || playerEntity->class_0x3F_63 != 3)
		result = 52;
	else
		result = GetManaSphereIndexFromId_36A50(playerEntity->playerData_0xA4_164x->playerColorIndex_0x38_56);
	return result;
}

//----- (00036A50) --------------------------------------------------------
int GetManaSphereIndexFromId_36A50(char index)//217a50
{
	int result=0;

	char index2 = index;
	if (index >= 0)
		index2 = TransformPlayerColorIndex_616D0(index);
	switch (index2 + 1)
	{
	case 0:
		result = 52;
		break;
	case 1:
		result = 105;
		break;
	case 2:
		result = 113;
		break;
	case 3:
		result = 121;
		break;
	case 4:
		result = 129;
		break;
	case 5:
		result = 137;
		break;
	case 6:
		result = 145;
		break;
	case 7:
		result = 153;
		break;
	case 8:
		result = 161;
		break;
	}
	return result;
}

//----- (00036AE0) --------------------------------------------------------
void UpdatePlayerDeathEffect_36AE0(type_entity_0x6E8E* a1x)//217ae0
{
	int result; // eax
	unsigned __int16 v2; // dx
	type_entity_0x6E8E* v3x; // edi
	type_entity_0x6E8E* ix; // ecx

	result = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
	v2 = a1x->mail_0x5E_94.channel[1].source;
	a1x->position_0x4C_76.z = result;
	if (v2)
	{
		v3x = Entities_EA3E4[v2];
		if (v3x->class_0x3F_63 == 3 && !a1x->playerEntityIndex_0x94_148)
		{
			for (ix = Entities_EA3E4[1]; ix < Entities_EA3E4[1000]; ix++)
			{
				if (a1x - world_D41A0.entities_0x6E8E == ix->playerEntityIndex_0x94_148)
					ix->playerEntityIndex_0x94_148 = v3x - world_D41A0.entities_0x6E8E;
			}
		}
		a1x->mail_0x5E_94.channel[1].source = 0;
		DisableEntityDrawing04_57F10(a1x);
	}
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];
// EA3E8: using guessed type int x_DWORD_EA3E8;
// EB384: using guessed type int x_DWORD_EB384;

//----- (00036BA0) --------------------------------------------------------
void TransformEntityToManaSphere_36BA0(type_entity_0x6E8E* entity, bool useManaFraction)//217ba0
{
	int manaFraction = 1;
	int locMana;
	int manaIndex;
	if (entity->mana_0x90_144 > 0)
	{
		if (useManaFraction)
		{
			manaFraction = entity->mana_0x90_144 / 1000;//217BCC mov edi,eax: the count of the spheres
			if (entity->mana_0x90_144 / 1000 < 1)
				manaFraction = 1;
			if (manaFraction > 16)
				manaFraction = 16;
			locMana = entity->mana_0x90_144 / manaFraction;
		}
		else
		{
			manaFraction = 1;
			locMana = entity->mana_0x90_144;
		}
		manaIndex = 0;
		entity->rand_0x14_20 = 9377 * entity->rand_0x14_20 + 9439;
		while (manaFraction > manaIndex)
		{
			//create mana sphere
			type_entity_0x6E8E* manaSphereEntity = CreateEntity_4A190(&entity->position_0x4C_76, 10, 39);
			//create mana sphere
			if (manaSphereEntity)
			{
				if (manaFraction - 1 == manaIndex)
					manaSphereEntity->mana_0x90_144 = entity->mana_0x90_144 - (manaFraction - 1) * locMana;
				else
					manaSphereEntity->mana_0x90_144 = locMana;
				manaSphereEntity->playerEntityIndex_0x94_148 = entity->playerEntityIndex_0x94_148;
				manaSphereEntity->rand_0x14_20 = 9377 * manaSphereEntity->rand_0x14_20 + 9439;
				manaSphereEntity->yaw_0x1C_28 = (manaSphereEntity->rand_0x14_20 % 0x71u + entity->yaw_0x1C_28 - 56) & 0x7FF;
				manaSphereEntity->rand_0x14_20 = 9377 * manaSphereEntity->rand_0x14_20 + 9439;
				manaSphereEntity->axis_0x9A_154x.x = 0;
				manaSphereEntity->axis_0x9A_154x.y = 0;
				manaSphereEntity->actSpeed_0x82_130 = manaSphereEntity->rand_0x14_20 % 0x30u + 16;
				int zDiff = entity->position_0x4C_76.z - GetTerrainAlt_10C40(&entity->position_0x4C_76);
				manaSphereEntity->param2_0x2C_44 = (1024 - zDiff - (my_sign32(1024 - zDiff) * 7)) >> 3;//0x217CF5 shl edx, 3 / sbb eax, edx
				MoveEntity_57FA0(&manaSphereEntity->axis_0x9A_154x, manaSphereEntity->yaw_0x1C_28, 0, manaSphereEntity->actSpeed_0x82_130);
			}
			manaIndex++;
		}
		entity->playerEntityIndex_0x94_148 = 0;
	}
}

//----- (00036D50) --------------------------------------------------------
void MergeManaSpheres_36D50(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//217d50
{
	type_entity_0x6E8E* v3x; // edx
	type_entity_0x6E8E* v4x; // eax
	//int v5; // eax
	int v6; // eax
	//int v7; // edi
	//int v8; // edx
	//int v9; // eax
	//uint8_t* v10; // eax
	//__int64 v11; // rtt

	if (a1x->playerEntityIndex_0x94_148 || a2x->playerEntityIndex_0x94_148)
	{
		if (!a1x->playerEntityIndex_0x94_148)
		{
			a1x->mana_0x90_144 += a2x->mana_0x90_144;
			a1x->playerEntityIndex_0x94_148 = a2x->playerEntityIndex_0x94_148;
			if (a2x->flags_0xc_12_15.byte[2] & 0x20)
			{
				a1x->flags_0xc_12_15.byte[2] |= 0x20u;
				return ReleaseEntity_57F20(a2x);
			}
			return ReleaseEntity_57F20(a2x);
		}
		if (!a2x->playerEntityIndex_0x94_148)
		{
			a1x->mana_0x90_144 += a2x->mana_0x90_144;
			return ReleaseEntity_57F20(a2x);
		}
		v3x = Entities_EA3E4[a1x->playerEntityIndex_0x94_148];
		v4x = Entities_EA3E4[a2x->playerEntityIndex_0x94_148];
		if (v3x->class_0x3F_63 == 10 && v4x->class_0x3F_63 == 10)
		{
			a1x->mana_0x90_144 += a2x->mana_0x90_144;
			return ReleaseEntity_57F20(a2x);
		}
		if (v3x->class_0x3F_63 == 10)
		{
			a1x->mana_0x90_144 += a2x->mana_0x90_144;
			v6 = v4x - world_D41A0.entities_0x6E8E;
		LABEL_22:
			a1x->playerEntityIndex_0x94_148 = v6;
			return ReleaseEntity_57F20(a2x);
		}
		if (v4x->class_0x3F_63 == 10)
		{
			//v7 = a2x->dword_0x90_144 + a1x->dword_0x90_144;
			//v8 = v3x-D41A0_BYTESTR_0.struct_0x6E8E;
			//v9 = v8;
			v6 = v3x - world_D41A0.entities_0x6E8E;
		}
		else
		{
			if (v3x == v4x)
			{
				a1x->mana_0x90_144 += a2x->mana_0x90_144;
				return ReleaseEntity_57F20(a2x);
			}
			if (v3x->maxMana_0x8C_140 > v4x->maxMana_0x8C_140)
			{
				a1x->mana_0x90_144 += a2x->mana_0x90_144;
				v6 = v3x - world_D41A0.entities_0x6E8E;
				goto LABEL_22;
			}
			//v7 = a2x->dword_0x90_144 + a1x->dword_0x90_144;
			//v9 = (uint8_t*)v4x - (&x_D41A0_BYTEARRAY_0[2288330022]);
			//v8 = v9;
			v6 = v4x - world_D41A0.entities_0x6E8E;
		}
		//LODWORD(v11) = v9;
		//HIDWORD(v11) = v8 >> 31;
		//v6 = v11 / 168;
		a1x->mana_0x90_144 += a2x->mana_0x90_144;
		goto LABEL_22;
	}
	a1x->mana_0x90_144 += a2x->mana_0x90_144;
	return ReleaseEntity_57F20(a2x);
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00036F30) --------------------------------------------------------
void ManaSphereCollide_36F30(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//217f30
{
	__int16 v2; // bx

	LOBYTE(v2) = 0;
	if (a1x->levelPar3_0x46_70 >= a2x->levelPar3_0x46_70)
	{
		if (a2x->parentId_0x28_40 != a1x->parentId_0x28_40)
		{
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			v2 = a1x->rand_0x14_20 & 1;
		}
	}
	else
	{
		LOBYTE(v2) = 1;
	}
	if ((x_BYTE)v2)
	{
		a1x->parentId_0x28_40 = a2x->parentId_0x28_40;
		a1x->playerEntityIndex_0x94_148 = a2x->playerEntityIndex_0x94_148;
		a1x->levelPar3_0x46_70 = a2x->levelPar3_0x46_70;
		a1x->subSpellIndex_0x2A_42 = a2x->subSpellIndex_0x2A_42;
	}
	a1x->mana_0x90_144 += a2x->mana_0x90_144;
	return ReleaseEntity_57F20(a2x);
}

//----- (00036FC0) --------------------------------------------------------
void UpdateManaSphereMovement_36FC0(type_entity_0x6E8E* a1x)//217fc0
{
	unsigned __int8 v1; // bl
	//int v2; // eax
	unsigned int v3; // esi
	unsigned int v4; // edi
	char result; // al
	uaxis_2d v6x; // bx
	unsigned __int8 v7; // cl
	char v8; // dl
	__int16 v9; // dx
	int v10; // eax
	int v11; // eax
	unsigned __int8 v12; // cl
	char v13; // ch
	char v14; // cl
	uaxis_2d v15x; // bx
	unsigned __int8* v16; // [esp+0h] [ebp-2Ch]
	unsigned __int8* v17; // [esp+4h] [ebp-28h]
	unsigned int v18; // [esp+8h] [ebp-24h]
	unsigned int v19; // [esp+Ch] [ebp-20h]
	unsigned int v20; // [esp+10h] [ebp-1Ch]
	unsigned int v21; // [esp+14h] [ebp-18h]
	unsigned __int8* v22; // [esp+18h] [ebp-14h]
	uaxis_2d v23x; // [esp+1Ch] [ebp-10h]
	uaxis_2d v24x; // [esp+1Ch] [ebp-10h]
	__int16 v25; // [esp+20h] [ebp-Ch]
	char v26; // [esp+24h] [ebp-8h]
	uaxis_2d v27x; // [esp+24h] [ebp-8h]
	char v28; // [esp+25h] [ebp-7h]
	char v29; // [esp+28h] [ebp-4h]

	if (CommandLineParams.DoDebugSequences()) {
		//add_compare(0x217fc4, CommandLineParams.DoDebugafterload(),1);
	}

	v29 = 0;
	v26 = (unsigned __int16)(a1x->position_0x4C_76.x + 128) >> 8;
	v28 = (unsigned __int16)(a1x->position_0x4C_76.y + 128) >> 8;
	v1 = a1x->levelPar3_0x46_70;
	v25 = a1x->position_0x4C_76.z >> 5;
	//v2 = 6 * v1;
	/*  v3 = *(unsigned __int8 *)(v2 + **filearray_2aa18c[24] + 5);
	  v17 = *(unsigned __int8 **)(v2 + **filearray_2aa18c[24]);
	  v4 = *(unsigned __int8 *)(v2 + **filearray_2aa18c[24] + 4);
	  result = sub_37740(a1);*/
	v3 = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[v1].height_5;
	v17 = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[v1].data;
	v4 = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[v1].width_4;
	result = IsCastleEffect42At_37740(a1x);
	//result = 0;v3 = 0;v4 = 0;v17 = 0;  //fix it
	if (!result)
	{
		if (videoMode_180660 == 1)
		{
			v3 >>= 1;
			v4 >>= 1;
		}
		if (isCaveLevel_D41B6 && !(buildingParams_D93C0[v1].flags_2 & 4))
			v29 = 1;
		v27x._axis_2d.x = v26 - (v4 >> 1);
		v27x._axis_2d.y = v28 - (v3 >> 1);
		v23x.word = v27x.word;
		v20 = 0;
		v22 = v17;
		while (v3 > v20)
		{
			v6x.word = v23x.word;
			v18 = 0;
			while (v4 > v18)
			{
				DamageSceneryOnTile_57390(v6x, a1x->id_0x1A_26);
				v7 = v22[1];
				if (v7 != 0xff)
				{
					mapHeightmap_11B4E0[v6x.word] = v7 + v25;
					v8 = mapAngle_13B4E0[v6x.word];
					if (!(v8 & 7))
					{
						mapAngle_13B4E0[v6x.word] = v8 & 0xF8 | 1;
						UpdateTerrainTiles_462A0(v6x, v6x);
					}
				}
				if (v29)
				{
					v9 = mapHeightmap_11B4E0[v6x.word];
					if (v9 >= v25)
						v10 = (unsigned __int8)v9;
					else
						v10 = v25;
					v11 = v10 + 80;
					if (v11 > 255)
						v11 = 255;
					if (v11 > (unsigned __int8)secondHeightmap_14B4E0[v6x.word])
						secondHeightmap_14B4E0[v6x.word] = v11;
					v12 = mapHeightmap_11B4E0[v6x.word];
					if ((unsigned __int8)secondHeightmap_14B4E0[v6x.word] > v12)
					{
						mapAngle_13B4E0[v6x.word] &= 0xF7u;
					}
					else
					{
						v13 = mapAngle_13B4E0[v6x.word] | 8;
						secondHeightmap_14B4E0[v6x.word] = v12 - 1;
						mapAngle_13B4E0[v6x.word] = v13;
					}
				}
				//LOBYTE(v6) = v6 + 1;
				v6x._axis_2d.x++;
				++v18;
				v22 += 2;
			}
			++v20;
			//++HIBYTE(v23);
			v23x._axis_2d.y++;
		}
		v24x.word = v27x.word;
		//result = (char)v17;
		v19 = 0;
		v16 = v17;
		if (v3)
		{
			do
			{
				v14 = 0;
				v15x.word = v24x.word;
				v21 = 0;
				while (v4 > v21)
				{
					if (*v16 != 0xff)
						SetTileTerrainType_45DC0(/*(uint8_t)v16,*/ v14, v15x, *v16);
					v15x._axis_2d.x++;
					v21++;
					v16 += 2;
				}
				//result = v19++ + 1;
				v19++;
				v24x._axis_2d.y++;
			} while (v3 > v19);
		}
	}
	//return result;
}
// D41B6: using guessed type char x_BYTE_D41B6;
// EA3BC: using guessed type int **filearray_2aa18c[24];
// 180660: using guessed type __int16 videoMode_180660;

int debugcounter_218240 = 0;

//----- (00037240) --------------------------------------------------------
void ApplyTerrainModification_37240(type_entity_0x6E8E* event)//218240
{
	bitmap_pos_struct_t v2; // esi
	unsigned int v3; // edi
	unsigned int v4; // esi
	uint16_t v7; // dx
	int v8; // eax
	int v9; // eax
	int v10; // edx
	uint8_t v11; // ch
	uint8_t* v13; // ebx
	unsigned int v14; // edx
	uint8_t* v15; // ebx
	uint8_t v17; // al
	uint16_t v18; // ax
	unsigned int v19; // esi
	int v20; // edi
	unsigned int v21; // [esp+0h] [ebp-58h]
	unsigned int v22; // [esp+4h] [ebp-54h]
	unsigned int v23; // [esp+8h] [ebp-50h]
	unsigned int v24; // [esp+Ch] [ebp-4Ch]
	unsigned int v25; // [esp+18h] [ebp-40h]
	uint8_t* v26; // [esp+1Ch] [ebp-3Ch]
	uint8_t* v27; // [esp+20h] [ebp-38h]
	unsigned int v28; // [esp+24h] [ebp-34h]
	unsigned int v29; // [esp+28h] [ebp-30h]
	unsigned int v30; // [esp+2Ch] [ebp-2Ch]
	unsigned int v31; // [esp+30h] [ebp-28h]
	unsigned int v32; // [esp+34h] [ebp-24h]
	unsigned int v33; // [esp+38h] [ebp-20h]
	uint8_t* v34; // [esp+40h] [ebp-18h]
	int v35; // [esp+44h] [ebp-14h]
	uint8_t v36; // [esp+48h] [ebp-10h]
	uaxis_2d v37x; // [esp+48h] [ebp-10h]
	uaxis_2d v38x; // [esp+48h] [ebp-10h]
	uint8_t v39; // [esp+49h] [ebp-Fh]
	uaxis_2d v40x; // [esp+4Ch] [ebp-Ch]
	uaxis_2d v41x; // [esp+4Ch] [ebp-Ch]
	uaxis_2d v42x; // [esp+4Ch] [ebp-Ch]
	uaxis_2d v43x; // [esp+4Ch] [ebp-Ch]
	uaxis_2d v44x; // [esp+4Ch] [ebp-Ch]
	uaxis_2d v45x; // [esp+50h] [ebp-8h]
	uaxis_2d v46x; // [esp+50h] [ebp-8h]
	uaxis_2d v47x; // [esp+50h] [ebp-8h]
	uaxis_2d v48x; // [esp+50h] [ebp-8h]
	uaxis_2d v49x; // [esp+50h] [ebp-8h]
	uint8_t v50; // [esp+54h] [ebp-4h]
	//uint8_t v17x;

	if (CommandLineParams.DoDebugSequences()) {
		//add_compare(0x218244, CommandLineParams.DoDebugafterload(),0xb8);
	}

	v50 = 0;
	if (event->maxLife_0x4 == event->life_0x8)
		SetShiftByCastle_49EC0(event, event->levelPar3_0x46_70);
	if (!IsCastleEffect42At_37740(event))
	{
		v36 = (event->position_0x4C_76.x + 128) >> 8;
		v39 = (event->position_0x4C_76.y + 128) >> 8;
		v35 = event->position_0x4C_76.z >> 5;
		v2 = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[event->levelPar3_0x46_70];
		v3 = v2.height_5;
		v26 = v2.data;
		v4 = v2.width_4;
		if (videoMode_180660 == 1)
		{
			v3 >>= 1;
			v4 >>= 1;
		}
		if (isCaveLevel_D41B6 && !(buildingParams_D93C0[event->levelPar3_0x46_70].flags_2 & 4))
			v50 = 1;
		v22 = v3 >> 1;
		event->life_0x8--;
		v25 = v4 >> 1;
		if (event->life_0x8 <= 0)//adress 21830a
		{
			v38x._axis_2d.x = v36 - v25;
			v38x._axis_2d.y = v39 - v22;
			v23 = 0;
			v44x.word = v38x.word;
			v27 = v26;
			while (v3 > v23)
			{
				v21 = 0;
				v49x.word = v44x.word;
				while (v4 > v21)
				{
					if (v27[0] != 0xffu)
					{
						UpdateTerrainTiles_462A0(v49x, v49x);//218609
					}
					v21++;
					v27 += 2;
					v49x._axis_2d.x++;
				}
				v23++;
				v44x._axis_2d.y++;
			}
			v17 = event->actionIndex_0x45_69;//adress 218650
			if (v17 >= 0x30u)
			{
				if (v17 <= 0x30u)
				{
					Entities_EA3E4[event->parentId_0x28_40]->stage_0x2E_46 = 2;
					DamageOverlappingWizards_377A0(event);
					DisableEntityDrawing04_57F10(event);
				}
				else if (v17 == 51)
				{
					event->life_0x8 = 1000 * event->subSpellIndex_0x2A_42;
					v18 = event->playerEntityIndex_0x94_148;
					event->actionIndex_0x45_69 = 52;
					if (v18)
						event->flags_0xc_12_15.byte[0] &= 0xFEu;
					else
						event->flags_0xc_12_15.byte[0] |= 1u;
					event->axis_0x9A_154x.z = event->position_0x4C_76.z;
					v19 = v4 >> 1;
					event->position_0x4C_76.z = GetTerrainAlt_10C40(&event->position_0x4C_76);
					v20 = (v3 >> 1) & 0xFFFF;
					DrawTerrainLine_48A20(v20, v38x._axis_2d.x, v38x._axis_2d.y, v20, (uint16_t)v19, 2u);
					DrawTerrainLine_48A20(v20, v38x._axis_2d.x, v38x._axis_2d.y, v20, (uint16_t)v19, 5u);//problem
					DamageOverlappingWizards_377A0(event);
				}
			}
		}
		else
		{
			v37x._axis_2d.x = v36 - v25;
			v37x._axis_2d.y = v39 - v22;
			if (event->maxLife_0x4 - 1 == event->life_0x8)
			{
				v29 = 0;
				v40x.word = v37x.word;
				while (v3 > v29)
				{
					v30 = 0;
					v45x.word = v40x.word;
					while (v4 > v30)
					{
						DamageSceneryOnTile_57390(v45x, event->id_0x1A_26);
						v30++;
						v45x._axis_2d.x++;
					}
					v29++;
					v40x._axis_2d.y++;
				}
			}
			v41x.word = v37x.word;//adress 218389
			v28 = 0;
			v34 = v26;
			while (v3 > v28)
			{
				v33 = 0;
				v46x.word = v41x.word;
				while (v4 > v33)
				{
					if (v34[1] != 0xffu)
					{
						mapHeightmap_11B4E0[v46x.word] += ((int32_t)((uint8_t)v34[1] + (uint16_t)v35 - mapHeightmap_11B4E0[v46x.word])) / event->life_0x8;
						if (!(mapAngle_13B4E0[v46x.word] & 7))
						{
							mapAngle_13B4E0[v46x.word] = mapAngle_13B4E0[v46x.word] & 0xF0 | 1;

							UpdateTerrainTiles_462A0(v46x, v46x);
						}
					}
					if (v50)
					{
						v7 = mapHeightmap_11B4E0[v46x.word];
						if (v7 >= (uint16_t)v35)
							v8 = (uint8_t)v7;
						else
							v8 = (uint16_t)v35;
						v9 = v8 + 80;
						if (v9 > 255)
							v9 = 255;
						v10 = secondHeightmap_14B4E0[v46x.word];
						if (v9 > v10)
							secondHeightmap_14B4E0[v46x.word] = v10 + (uint32_t)((v9 - v10) / event->life_0x8);
						v11 = mapHeightmap_11B4E0[v46x.word];
						if (secondHeightmap_14B4E0[v46x.word] > v11)
						{
							mapAngle_13B4E0[v46x.word] &= 0xF7u;
						}
						else
						{
							secondHeightmap_14B4E0[v46x.word] = v11 - 1;
							mapAngle_13B4E0[v46x.word] |= 8u;
						}
					}
					v33++;
					v34 += 2;
					v46x._axis_2d.x++;
				}
				v28++;
				v41x._axis_2d.y++;
			}
			//adress 2184e0
			if (!(event->life_0x8 % 5) || event->life_0x8 == 1)
			{
				v13 = v26;
				v14 = 0;
				v42x.word = v37x.word;
				while (v14 < v3)
				{
					v32 = 0;
					v47x = v42x;
					while (v4 > v32)
					{
						if (v13[0] != 0xffu)
						{//adress 218519
							mapAngle_13B4E0[v47x.word] = mapAngle_13B4E0[v47x.word] & 0xF0 | 1;
							mapTerrainType_10B4E0[v47x.word] = 1;
						}
						v13 += 2;
						v32++;
						v47x._axis_2d.x++;
					}
					v14++;
					v42x._axis_2d.y++;
				}
				v31 = 0;//21855e
				v15 = v26;
				v43x.word = v37x.word;
				if (v3)
				{
					do
					{
						v24 = 0;
						v48x = v43x;
						//v17x = 0;
						while (v4 > v24)
						{
							if (v15[0] != 0xffu)//adress 218593
								SetTileTerrainType_45DC0(v24, v48x, v15[0], v24 ? v48x._axis_2d.x : 0);//21859F mov dl,[ebp+var_8]; 218575 xor edx,edx
							v15 += 2;
							//v17x = v48x._axis_2d.x + 1;
							v24++;
							v48x._axis_2d.x++;
						}
						v31++;
						v43x._axis_2d.y++;
					} while (v3 > v31);
				}
			}
		}
	}
}
// D41B6: using guessed type char x_BYTE_D41B6;
// EA3BC: using guessed type int **filearray_2aa18c[24];
// EA3E4: using guessed type int Entities_EA3E4[];
// 180660: using guessed type __int16 videoMode_180660;

//----- (00037740) --------------------------------------------------------
bool IsCastleEffect42At_37740(type_entity_0x6E8E* event)//218740
{
	type_entity_0x6E8E* event2 = gameState_D41A4.miscList_38535;
	if (event2 <= Entities_EA3E4[0])
		return false;
	while (event2->class_0x3F_63 != 0xA || event2->model_0x40_64 != 0x2A || !CompareAxisWithShift_10750(event, event2))
	{
		event2 = event2->next_0;//fix it
		if (event2 <= Entities_EA3E4[0])
			return false;
	}
	return true;
}

//----- (000377A0) --------------------------------------------------------
void DamageOverlappingWizards_377A0(type_entity_0x6E8E* a1x)//2187a0
{
	type_entity_0x6E8E* ix; // ebx

	for (ix = gameState_D41A4.class3List_38519; ix > Entities_EA3E4[0]; ix = ix->next_0)
	{
		if (CompareAxisWithShift_10750(a1x, ix))
			SpawnCastleEffect42_5FBD0(ix);
	}
}
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000377F0) --------------------------------------------------------
void UpdateCastleEffect41_377F0(type_entity_0x6E8E* a1x)//2187f0
{
	__int16 v1; // dx
	//signed int v2; // eax
	unsigned int v3; // esi
	//unsigned int v4; // eax
	unsigned int v5; // edi
	char v6; // cl
	__int16 v7; // ax
	int v8; // eax
	int v9; // eax
	int v10; // ecx
	int v11; // edx
	int v12; // edx
	unsigned int v13; // edx
	unsigned __int16 v14; // ax
	char v15; // cl
	unsigned __int8 v16; // ch
	unsigned int k; // ecx
	unsigned int v18; // edx
	unsigned __int8 v19; // ch
	unsigned int l; // [esp+0h] [ebp-28h]
	unsigned int i; // [esp+4h] [ebp-24h]
	int v23; // [esp+8h] [ebp-20h]
	unsigned int j; // [esp+10h] [ebp-18h]
	unsigned __int16 v25; // [esp+18h] [ebp-10h]

	LOBYTE(v1) = (unsigned __int16)(a1x->position_0x4C_76.x + 128) >> 8;
	HIBYTE(v1) = (unsigned __int16)(a1x->position_0x4C_76.y + 128) >> 8;
	/* if ( videoMode_180660 == 1 )
	   v2 = (signed int)*(unsigned __int8 *)(**filearray_2aa18c[24] + 6 * *(char *)(a1 + 70) + 4) >> 1;
	 else
	   v2 = *(unsigned __int8 *)(**filearray_2aa18c[24] + 6 * *(char *)(a1 + 70) + 4);
	 v3 = v2;
	 if ( videoMode_180660 == 1 )
	   v4 = (signed int)*(unsigned __int8 *)(**filearray_2aa18c[24] + 6 * *(char *)(a1 + 70) + 5) >> 1;
	 else
	   v4 = *(unsigned __int8 *)(**filearray_2aa18c[24] + 6 * *(char *)(a1 + 70) + 5);
	 v5 = v4;*/
	// BUILD00 table (dword_EA3BC): 0x21883A
	if (videoMode_180660 == 1)
		v3 = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[a1x->levelPar3_0x46_70].width_4 >> 1;
	else
		v3 = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[a1x->levelPar3_0x46_70].width_4;
	//v3 = v2;
	if (videoMode_180660 == 1)
		v5 = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[a1x->levelPar3_0x46_70].height_5 >> 1;
	else
		v5 = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[a1x->levelPar3_0x46_70].height_5;
	//v5 = v4;
	if (videoMode_180660 == 1)
	{
		v5 >>= 1;
		v3 >>= 1;
	}
	LOBYTE(v25) = v1 - (v3 >> 1);
	v6 = a1x->flags_0xc_12_15.byte[0];
	HIBYTE(v25) = HIBYTE(v1) - (v5 >> 1);
	if (v6 & 2)
	{
		if (!Entities_EA3E4[a1x->parentId_0x28_40]->maxStage_0x30_48 && a1x->param_0x10_16)
		{
			v9 = a1x->subSpellIndex_0x2A_42 - a1x->stage_0x2E_46;
			v10 = a1x->param_0x10_16;
			HIWORD(v11) = (unsigned int)(v9 % v10) >> 16;
			v23 = v9 / v10;
			LOWORD(v11) = a1x->stage_0x2E_46;
			v12 = v9 / v10 + v11;
			v8 = a1x->param_0x10_16;
			a1x->stage_0x2E_46 = v12;
			if (v8 == 1)
			{
				v13 = 0;
				v14 = v25;
				while (v5 > v13)
				{
					for (i = 0; v3 > i; i++)
					{
						v15 = mapAngle_13B4E0[v14];
						if (v15 < 0)
						{
							mapAngle_13B4E0[v14] = v15 & 0x7F;
							if (!isCaveLevel_D41B6)
								mapAngle_13B4E0[v14] = v15 & 0x7F | 8;
						}
						v16 = v23 + mapHeightmap_11B4E0[v14];
						mapHeightmap_11B4E0[v14] = v16;
						if (isCaveLevel_D41B6)
						{
							if (v16 < (unsigned __int8)secondHeightmap_14B4E0[v14])
							{
								mapAngle_13B4E0[v14] &= 0xF7u;
							}
							else
							{
								secondHeightmap_14B4E0[v14] = v16 - 1;
								mapAngle_13B4E0[v14] |= 8u;
							}
						}
						LOBYTE(v14) = v14 + 1;
					}
					v13++;
					v14 = __PAIR__(HIBYTE(v14), (unsigned __int8)v25) + 256;
				}
				LOWORD(v8) = -10;
				a1x->param_0x10_16 = -10;
			}
			else if (v8 == -1)
			{
				LOWORD(v8) = v25;
				for (j = 0; v5 > j; j++)
				{
					for (k = 0; k < v3; k++)
					{
						if (!isCaveLevel_D41B6 && mapAngle_13B4E0[(unsigned __int16)v8] & 8)
						{
							mapAngle_13B4E0[(unsigned __int16)v8] |= 0x80u;
							mapAngle_13B4E0[(unsigned __int16)v8] &= 0xF7u;
						}
						LOBYTE(v8) = v8 + 1;
					}
					LOWORD(v8) = __PAIR__(BYTE1(v8), (unsigned __int8)v25) + 256;
				}
				a1x->param_0x10_16++;
			}
			else if (v8 <= 0)
			{
				a1x->param_0x10_16 = v8 + 1;
			}
			else
			{
				v18 = 0;
				LOWORD(v8) = v25;
				while (v5 > v18)
				{
					for (l = 0; v3 > l; l++)
					{
						v19 = v23 + mapHeightmap_11B4E0[(unsigned __int16)v8];
						mapHeightmap_11B4E0[(unsigned __int16)v8] = v19;
						if (isCaveLevel_D41B6)
						{
							if (v19 < (unsigned __int8)secondHeightmap_14B4E0[(unsigned __int16)v8])
							{
								mapAngle_13B4E0[(unsigned __int16)v8] &= 0xF7u;
							}
							else
							{
								secondHeightmap_14B4E0[(unsigned __int16)v8] = v19 - 1;
								mapAngle_13B4E0[(unsigned __int16)v8] |= 8u;
							}
						}
						LOBYTE(v8) = v8 + 1;
					}
					v18++;
					LOWORD(v8) = __PAIR__(BYTE1(v8), (unsigned __int8)v25) + 256;
				}
				a1x->param_0x10_16--;
			}
		}
		else
		{
			Entities_EA3E4[a1x->parentId_0x28_40]->stage_0x2E_46 = 2;
			Entities_EA3E4[a1x->parentId_0x28_40]->axis_0x9A_154x.z = 32 * a1x->stage_0x2E_46;
			DrawTerrainLine_48A20(v5 >> 1, v1, SHIBYTE(v1), (unsigned __int16)(v5 >> 1), (v3 >> 1) & 0xFFFF, 3u);
			DisableEntityDrawing04_57F10(a1x);
		}
	}
	else
	{
		a1x->stage_0x2E_46 = a1x->position_0x4C_76.z >> 5;
		a1x->param_0x10_16 = 10;
		a1x->flags_0xc_12_15.byte[0] = v6 | 2;
		v7 = GetTerrainHeightFromSquare_48DF0(v25 - 1, HIBYTE(v25) - 1, v5 + 2, v3 + 2);
		a1x->subSpellIndex_0x2A_42 = v7;
		if ((unsigned __int16)v7 > 0xDCu)
			a1x->subSpellIndex_0x2A_42 = 220;
		v8 = a1x->stage_0x2E_46;
		if (v8 == a1x->subSpellIndex_0x2A_42)
			a1x->param_0x10_16 = 0;
	}
}

//----- (00037BC0) --------------------------------------------------------
void UpdateCastleEffect42_37BC0(type_entity_0x6E8E* a1x)//218bc0 // groove castle
{
	signed int v3; // eax
	unsigned int v4; // eax
	signed int v6; // eax
	unsigned int v7; // edx
	unsigned int v8; // edx
	uaxis_2d v9x; // ax
	unsigned __int8* v10; // esi
	int v11; // edi
	unsigned __int8 v12; // ch
	//int v13; // edx
	unsigned int v14; // edi
	int v15; // esi
	char v16; // al
	char v17; // dl
	signed __int16 v18; // dx
	int v19; // eax
	int v20; // eax
	int v21; // edx
	unsigned __int8 v22; // ch
	signed int v23; // eax
	unsigned int v24; // esi
	unsigned int v25; // eax
	uaxis_2d v26x; // ST44_2
	unsigned int v27; // edx
	unsigned int v28; // ecx
	uint8_t* v30; // [esp+4h] [ebp-54h]
	unsigned int v31; // [esp+10h] [ebp-48h]
	unsigned int v32; // [esp+14h] [ebp-44h]
	int v33; // [esp+18h] [ebp-40h]
	unsigned int k; // [esp+1Ch] [ebp-3Ch]
	int v35; // [esp+20h] [ebp-38h]
	unsigned int v36; // [esp+24h] [ebp-34h]
	unsigned int v37; // [esp+28h] [ebp-30h]
	unsigned int v38; // [esp+2Ch] [ebp-2Ch]
	unsigned int v39; // [esp+30h] [ebp-28h]
	int v40; // [esp+34h] [ebp-24h]
	uaxis_2d v41x; // [esp+38h] [ebp-20h]
	uaxis_2d v42x; // [esp+3Ch] [ebp-1Ch]
	uaxis_2d jx; // [esp+3Ch] [ebp-1Ch]
	uaxis_2d v44x; // [esp+3Ch] [ebp-1Ch]
	uaxis_2d v45x; // [esp+40h] [ebp-18h]
	unsigned __int8 i; // [esp+44h] [ebp-14h]
	char v47; // [esp+48h] [ebp-10h]
	unsigned __int8 v48; // [esp+50h] [ebp-8h]
	char v49; // [esp+54h] [ebp-4h]
	char v50; // [esp+54h] [ebp-4h]

	v30 = smalltitBuffer_E9C38;
	if (!(a1x->flags_0xc_12_15.byte[0] & 0x2))
	{
		a1x->param_0x10_16 = 0x13;
		a1x->flags_0xc_12_15.byte[0] |= 2;
	}
	if (a1x->param_0x10_16 <= 0)
	{
		a1x->param_0x10_16++;
		if (a1x->param_0x10_16 == 0)
		{
			v50 = (unsigned __int16)(a1x->position_0x4C_76.x + 128) >> 8;
			if (a1x->levelPar3_0x46_70 < 0)//fix
			{//fix
				v23 = 0;//fix
				v25 = 0;//fix
			}//fix
			else//fix
			{
				if (videoMode_180660 == 1)
					v23 = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[a1x->levelPar3_0x46_70].width_4 >> 1;
				else
					v23 = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[a1x->levelPar3_0x46_70].width_4;

				if (videoMode_180660 == 1)
					v25 = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[a1x->levelPar3_0x46_70].height_5 >> 1;
				else
					v25 = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[a1x->levelPar3_0x46_70].height_5;
			}

			v24 = v23;

			if (!isCaveLevel_D41B6)
			{
				v27 = 0;
				v26x._axis_2d.y = ((unsigned __int16)(a1x->position_0x4C_76.y + 128) >> 8) - (v25 >> 1);
				v26x._axis_2d.x = v50 - (v24 >> 1);
				v44x.word = v26x.word;
				while (v27 < v25)
				{
					v28 = 0;
					while (v28 < v24)
					{
						if (mapAngle_13B4E0[v44x.word] & 8)
						{
							mapAngle_13B4E0[v44x.word] |= 0x80u;
							mapAngle_13B4E0[v44x.word] &= 0xF7u;
						}
						v28++;
						v44x._axis_2d.x++;
					}
					v44x._axis_2d.x = v50 - (v24 >> 1);
					v27++;
					v44x._axis_2d.y++;
				}
			}
			Entities_EA3E4[a1x->parentId_0x28_40]->stage_0x2E_46 = 2;
			DisableEntityDrawing04_57F10(a1x);
		}
	}
	else
	{
		a1x->param_0x10_16--;// = v2 - 1;
		if (a1x->param_0x10_16 == 0)
		{
			if (a1x->poseIndex_0x3B_59)
				a1x->param_0x10_16 = -25;
			else
				a1x->param_0x10_16 = -1;
		}
		else
		{
			if (!Entities_EA3E4[a1x->parentId_0x28_40]->maxStage_0x30_48)
			{
				v49 = (unsigned __int16)(a1x->position_0x4C_76.x + 128) >> 8;
				v47 = (unsigned __int16)(a1x->position_0x4C_76.y + 128) >> 8;
				v40 = a1x->position_0x4C_76.z >> 5;
				if (a1x->levelPar3_0x46_70 < 0)//fix
				{//fix
					v3 = 0;//fix
					v4 = 0;//fix
				}//fix
				else//fix
				{
					if (videoMode_180660 == 1)
						v3 = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[a1x->levelPar3_0x46_70].width_4 >> 1;
					else
						v3 = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[a1x->levelPar3_0x46_70].width_4;

					if (videoMode_180660 == 1)
						v4 = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[a1x->levelPar3_0x46_70].height_5 >> 1;
					else
						v4 = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[a1x->levelPar3_0x46_70].height_5;
				}

				v36 = v3;

				v37 = v4;
				v48 = v4 >> 1;
				v41x._axis_2d.x = v49 - (v36 >> 1);
				v41x._axis_2d.y = v47 - v48;
				memset((void*)v30, 0, 2 * v4 * v36);
				for (i = 1; i <= a1x->levelPar3_0x46_70; i++)
				{
					if (videoMode_180660 == 1)
						v6 = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[i].width_4 >> 1;
					else
						v6 = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[i].width_4;
					v31 = v6;
					if (videoMode_180660 == 1)
						v7 = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[i].height_5 >> 1;
					else
						v7 = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[i].height_5;
					v39 = v7;
					v8 = v7 >> 1;
					v9x._axis_2d.x = v49 - v8;
					v9x._axis_2d.y = v47 - (v31 >> 1);
					v10 = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[i].data;
					v45x.word = v9x.word;
					v33 = (unsigned __int8)(v36 >> 1) - v8;
					v38 = 0;
					v11 = v36 * (v48 - (v31 >> 1));
					while (v38 < v31)
					{
						v35 = v11 + v33;
						v32 = 0;
						v42x.word = v45x.word;
						while (v32 < v39)
						{
							if (a1x->flags_0xc_12_15.byte[2] & 1)
								DamageSceneryOnTile_57390(v42x, a1x->id_0x1A_26);
							v12 = v10[1];
							if (v12 != 0xff)
								*(x_WORD*)(v30 + 2 * v35) = v12 + (x_WORD)v40 - mapHeightmap_11B4E0[v42x.word];
							//v13 = a1x->dword_0x10_16 % 7;
							if ((!(a1x->param_0x10_16 % 7) || a1x->param_0x10_16 == 1) && v10[0] != 0xff)
								SetTileTerrainType_45DC0(7, v42x, v10[0], a1x->param_0x10_16 % 7);//218E74 idiv ecx
							v10 += 2;
							v32++;
							v42x._axis_2d.x++;
							v35++;
						}
						v11 += v36;
						v38++;
						v45x._axis_2d.y++;
					}
				}
				v14 = 0;
				v15 = 0;
				for (jx.word = v41x.word; v14 < v37; ++jx._axis_2d.y)
				{
					for (k = 0; k < v36; k++)
					{
						if (*(x_WORD*)(v30 + 2 * v15))
						{
							if (!mapHeightmap_11B4E0[jx.word] || IsSpellCastable_57450(mapTerrainType_10B4E0[jx.word]))
							{
								//adress 218f4e
								mapAngle_13B4E0[jx.word] = mapAngle_13B4E0[jx.word] & 0xF8 | 1;
								AddBuildingToTerrain_46570(jx, jx);
							}
							mapHeightmap_11B4E0[jx.word] += (signed int)*(int16_t*)(v30 + 2 * v15) / a1x->param_0x10_16;
							if (a1x->param_0x10_16 == 1)
							{
								v16 = mapAngle_13B4E0[jx.word];
								if (v16 < 0)
								{
									v17 = isCaveLevel_D41B6;
									mapAngle_13B4E0[jx.word] = v16 & 0x7F;
									if (!v17)
										mapAngle_13B4E0[jx.word] = v16 & 0x7F | 8;
								}
							}
						}
						if (isCaveLevel_D41B6)
						{
							v18 = mapHeightmap_11B4E0[jx.word];
							if (v18 >= (signed __int16)v40)
								v19 = (unsigned __int8)v18;
							else
								v19 = (signed __int16)v40;
							v20 = v19 + 100;
							if (v20 > 255)
								v20 = 255;
							v21 = (unsigned __int8)secondHeightmap_14B4E0[jx.word];
							if (v20 > v21)
								secondHeightmap_14B4E0[jx.word] += (v20 - v21) / a1x->param_0x10_16;
							v22 = mapHeightmap_11B4E0[jx.word];
							if ((unsigned __int8)secondHeightmap_14B4E0[jx.word] > v22)
							{
								mapAngle_13B4E0[jx.word] &= 0xF7u;
							}
							else
							{
								secondHeightmap_14B4E0[jx.word] = v22 - 1;
								mapAngle_13B4E0[jx.word] |= 8u;
							}
						}
						if (!isCaveLevel_D41B6 && a1x->param_0x10_16 == 2)
							mapAngle_13B4E0[jx.word] &= 0xF7u;
						jx._axis_2d.x++;
						v15++;
					}
					jx._axis_2d.x = v49 - (v36 >> 1);
					v14++;
				}

				{
					int sizecpy = 2 * v4 * v36;//fix for debug
					if (sizeof(type_E9C38_smalltit) * 40 * 21 < sizecpy)//fix for debug
						sizecpy = sizeof(type_E9C38_smalltit) * 40 * 21;//fix for debug
					memcpy(terrainTiles_E9C38, smalltitBuffer_E9C38, sizecpy);//fix for debug
				}

			}
		}
	}
}

//----- (00038270) --------------------------------------------------------
type_entity_0x6E8E* GetRandManaSphere_38270(type_entity_0x6E8E* event)//219270
{
	type_entity_0x6E8E* resultx;
	event->rand_0x14_20 = 9377 * event->rand_0x14_20 + 9439;
	switch (event->rand_0x14_20 % 0xCu)
	{
	case 0u:
	case 1u:
		resultx = CreateEntity_4A190(&predictedAxis_EB398ar, 5, 4);
		if (resultx)
			resultx->actionIndex_0x45_69 = 33;
		break;
	case 2u:
	case 3u:
		resultx = CreateEntity_4A190(&predictedAxis_EB398ar, 5, 14);
		if (resultx)
			resultx->actionIndex_0x45_69 = 113;
		break;
	case 4u:
	case 5u:
	case 6u:
	case 7u:
	case 8u:
		resultx = CreateEntity_4A190(&predictedAxis_EB398ar, 5, 13);
		if (resultx)
			resultx->actionIndex_0x45_69 = 105;
		break;
	case 9u:
	case 0xAu:
	case 0xBu:
		resultx = CreateEntity_4A190(&predictedAxis_EB398ar, 5, 12);
		if (resultx)
			resultx->actionIndex_0x45_69 = 97;
		break;
	default:
		resultx = NULL;
		break;
	}
	return resultx;
}

//----- (000389F0) --------------------------------------------------------
int UpdateProjectileLife_389F0(type_entity_0x6E8E* a1x)//2199f0
{
	int v1; // eax
	//__int16 v2; // dx

	v1 = a1x->life_0x8;
	a1x->param_0x10_16++;
	a1x->life_0x8 = v1 - 1;
	if (v1 >= 0)
	{
		AdvanceAnimationFrame_585A0(a1x);
		if (!(a1x->flags_0xc_12_15.byte[0] & 2))
		{
			a1x->flags_0xc_12_15.byte[0] |= 2u;
			if ((signed __int16)GetTerrainAlt_10C40(&a1x->position_0x4C_76) > 58880)
				UpdateCastleLevel_5F890(a1x, 0);
			if (EntitiesOverlap_106C0(
				a1x,
				Entities_EA3E4[Entities_EA3E4[a1x->id_0x1A_26]->playerData_0xA4_164x->CastleEntityIndex_0x3A_58]))
			{
				//v2 =a1x->word_0x1A_26;
				Entities_EA3E4[Entities_EA3E4[a1x->id_0x1A_26]->playerData_0xA4_164x->CastleEntityIndex_0x3A_58]->mail_0x5E_94.channel[5].source = a1x->id_0x1A_26;
				Entities_EA3E4[Entities_EA3E4[a1x->id_0x1A_26]->playerData_0xA4_164x->CastleEntityIndex_0x3A_58]->mail_0x5E_94.channel[5].amount.word[0] = 10;
			}
			else
			{
				UpdateCastleLevel_5F890(a1x, 0);
			}
		}
	}
	DisableEntityDrawing04_57F10(a1x);
	return 1;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00038B00) --------------------------------------------------------
signed int CompareEvent08_38B00(type_entity_0x6E8E* event)//219b00
{
	event->hitById_0x26_38 = 0;
	if (event->life_0x8 < 0)
		return 2;
	if (event->mail_0x5E_94.channel[0].source)
	{
		event->life_0x8 -= event->mail_0x5E_94.channel[0].amount.dword;
		if (event->life_0x8 < 0)
		{
			event->ownerPlayerId_0x24_36 = event->mail_0x5E_94.channel[0].source;
			event->hitById_0x26_38 = event->mail_0x5E_94.channel[0].source;
			return 2;
		}
		event->hitById_0x26_38 = event->mail_0x5E_94.channel[0].source;
		event->mail_0x5E_94.channel[0].amount.dword = 0;
		event->mail_0x5E_94.channel[0].source = 0;
		return 1;
	}
	return 0;
}

//----- (00038B90) --------------------------------------------------------
int UpdateProjectileTrail_38B90(type_entity_0x6E8E* a1x)//219b90
{
	int v1; // eax
	int result; // eax
	//uint8_t* v3; // eax
	int v4; // edi
	type_entity_0x6E8E* v5x; // eax
	type_entity_0x6E8E* v6x; // esi
	int v7; // edx
	axis_3d v8x; // [esp+0h] [ebp-10h]
	//__int16 v9; // [esp+2h] [ebp-Eh]
	//__int16 v10; // [esp+4h] [ebp-Ch]
	int v11; // [esp+8h] [ebp-8h]
	int v12; // [esp+Ch] [ebp-4h]

	v1 = a1x->life_0x8;
	a1x->life_0x8 = v1 - 1;
	if (v1 < 0)
	{
		return 1;
	}
	if (!(a1x->flags_0xc_12_15.byte[0] & 2))
	{
		//a1x->byte_0xc_12 |= 0x10002;
		a1x->flags_0xc_12_15.dword |= 0x10002;
		//a1x->byte_0xc_12 |= 0x02;
		//a1x->byte_0xe_14 |= 0x1;
		PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 30);
	}
	SetEntityShiftRot_49EA0(
		a1x,
		(768 * a1x->param_0x10_16
			- (__CFSHL__(768 * a1x->param_0x10_16 >> 31, 2)
				+ 4 * (768 * a1x->param_0x10_16 >> 31))) >> 2,
		512);
	SendMailToOverlappingEntities_10C80(a1x, 0, (signed int)a1x->subSpellIndex_0x2A_42 / a1x->maxLife_0x4);
	v4 = AddE7EE0x_10080(a1x->param_0x10_16, a1x->param_0x10_16);
	if (v4)
	{
		a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
		while (GetNextSearchOffset_10130(v4, &v12, &v11) == 1)
		{
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			v8x.x = a1x->position_0x4C_76.x - 96 + 160 * v12 + a1x->rand_0x14_20 % 0x81u - 64;
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			v8x.y = a1x->rand_0x14_20 % 0x81u + a1x->position_0x4C_76.y - 96 + 160 * v11 - 64;
			v8x.z = a1x->position_0x4C_76.z;
			v5x = CreateEntity_4A190(&v8x, 10, 0);
			v6x = v5x;
			if (v5x)
			{
				v5x->id_0x1A_26 = a1x->id_0x1A_26;
				v5x->yaw_0x1C_28 = a1x->yaw_0x1C_28;
				//v5x->byte_0xc_12 |= 0x80;//(unsigned int)sub_10080;
				//v5x->byte_0xd_13 |= 0x00;
				//v5x->byte_0xe_14 |= 0x01;
				v5x->flags_0xc_12_15.dword |= 0x10080;
				SetEntityShiftRot_49EA0(v5x, 512, 512);
				v6x->param_0x10_16 = 0;
			}
		}
		ResetEvent08_10100(v4);
	}
	v7 = a1x->param_0x10_16 + 2;
	result = v7 / 7;
	a1x->param_0x10_16 = v7 % 7;
	return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (00038D80) --------------------------------------------------------
unsigned int UpdateProjectileTimeout_38D80(type_entity_0x6E8E* a1x)//219d80
{
	unsigned int result; // eax
	type_entity_0x6E8E* ix; // ebx
	unsigned int v3; // ecx

	result = a1x->life_0x8;
	a1x->life_0x8 = result - 1;
	if ((result & 0x80000000) != 0)
	{
		DisableEntityDrawing04_57F10(a1x);
		return 0;
	}
	for (ix = gameState_D41A4.manaSphereList_38523; ix > Entities_EA3E4[0]; ix = ix->next_0)
	{
		if (!ix->mail_0x5E_94.channel[4].source)
		{
			result = Maths::EuclideanDistXY_584D0(&a1x->position_0x4C_76, &ix->position_0x4C_76);
			if (result < a1x->param_0x10_16)
			{
				v3 = Maths::IntegerSqrt_7277A(result);
				if (v3 > 0x2A)
					v3 = 42;
				result = a1x - world_D41A0.entities_0x6E8E;
				ix->mail_0x5E_94.channel[4].amount.word[0] = LOWORD(v3);
				ix->mail_0x5E_94.channel[4].amount.word[1] = HIWORD(v3);
				ix->mail_0x5E_94.channel[4].source = result;
			}
		}
	}
	return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00038E20) --------------------------------------------------------
int HideProjectile_38E20(type_entity_0x6E8E* a1x)//219e20
{
	DisableEntityDrawing04_57F10(a1x);
	return 1;
}

//----- (00038E40) --------------------------------------------------------
int HideProjectile2_38E40(type_entity_0x6E8E* a1x)//219e40
{
	DisableEntityDrawing04_57F10(a1x);
	return 1;
}

//----- (00038E70) --------------------------------------------------------
void SlowDownWizard_38E70(type_entity_0x6E8E* event)//219e70
{
	if (event->targetId_0x96_150)
	{
		type_entity_0x6E8E* event2 = Entities_EA3E4[event->targetId_0x96_150];
		if (event2->class_0x3F_63 == 3 && !event2->model_0x40_64)
		{
			if (event2->playerData_0xA4_164x->moveSpeed_0x14C_332 < 3u)
			{
				if (!event2->playerData_0xA4_164x->moveSpeed_0x14C_332)
				{
					event2->playerData_0xA4_164x->moveBoost_0x1E_30 = -80;
					event->rand_0x14_20 = 9377 * event->rand_0x14_20 + 9439;
					PrepareEventSound_6E450(event->targetId_0x96_150, -1, (event->rand_0x14_20 & 3) + 54);
				}
				SetPaletteModification_5C830(event2, 3, 171 * ++event2->playerData_0xA4_164x->moveSpeed_0x14C_332 / 3 + 85);
			}
			event2->playerData_0xA4_164x->moveSpeedCounter_0x14D_333 = 8;
		}
	}
	DisableEntityDrawing04_57F10(event);
}

//----- (00038F70) --------------------------------------------------------
int MobilizeWizard_38F70(type_entity_0x6E8E* a1x)//219f70
{
	type_entity_0x6E8E* v1x; // esi
	//int v2; // eax

	if (a1x->targetId_0x96_150)
	{
		v1x = Entities_EA3E4[a1x->targetId_0x96_150];
		if (v1x->class_0x3F_63 == 3 && !v1x->model_0x40_64)
		{
			//v2 = v1x->dword_0xA4_164;
			if (!v1x->playerData_0xA4_164x->mobilizeCounter_0x14E_334)
			{
				v1x->playerData_0xA4_164x->moveBoost_0x1E_30 = -80;
				a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
				PrepareEventSound_6E450(a1x->targetId_0x96_150, -1, (a1x->rand_0x14_20 & 3) + 54);
				SendMailToEntity_11900(a1x, v1x, 0, a1x->subSpellIndex_0x2A_42);
			}
			v1x->playerData_0xA4_164x->mobilizeCounter_0x14E_334 = 1;
			v1x->playerData_0xA4_164x->mobilizeCounter2_0x150_336 = 10;
		}
	}
	DisableEntityDrawing04_57F10(a1x);
	return 1;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00039040) --------------------------------------------------------
void UpdateLightning_39040(type_entity_0x6E8E* a1x)//21a040
{
	int v1; // eax
	int v2; // ebx
	int v3; // edx
	unsigned __int8 v4; // bl
	unsigned __int16 v5; // ax
	int v6; // edx
	int v7; // ecx
	uaxis_2d v8x; // bx
	signed int v9; // eax
	__int16 v10; // ax
	int v11; // ecx
	int v12; // ecx
	char v13; // dl
	int v14; // esi
	int v15; // edx
	int v16; // esi
	char v17; // al
	uaxis_2d v18x; // cx
	int v19; // eax
	int v20; // ecx
	int v21; // eax
	char v22; // dh
	uaxis_2d v23x; // dx
	uaxis_2d v24x; // ax
	signed int v25; // esi
	uaxis_2d v26x; // bx
	int v27; // ecx
	int v28; // edi
	int v29; // eax
	uaxis_2d v30x; // bx
	signed int v31; // esi
	signed int v32; // edi
	uaxis_2d v33x; // ax
	uaxis_2d v34x; // dx
	uaxis_2d v35x; // bx
	signed int v36; // eax
	signed int v37; // edx
	uaxis_2d v38x; // si
	axis_3d v40x; // [esp+0h] [ebp-3Ch]
	//__int16 v41; // [esp+2h] [ebp-3Ah]
	axis_3d v42x; // [esp+8h] [ebp-34h]
	//__int16 v43; // [esp+Ch] [ebp-30h]
	int i; // [esp+10h] [ebp-2Ch]
	int v45; // [esp+14h] [ebp-28h]
	int v46; // [esp+18h] [ebp-24h]
	uaxis_2d v47x; // [esp+1Ch] [ebp-20h]
	int v48; // [esp+20h] [ebp-1Ch]
	uaxis_2d v49x; // [esp+24h] [ebp-18h]
	int v50; // [esp+28h] [ebp-14h]
	int v51; // [esp+2Ch] [ebp-10h]
	uaxis_2d v52x; // [esp+30h] [ebp-Ch]
	uaxis_2d v53x; // [esp+34h] [ebp-8h]
	char v54; // [esp+38h] [ebp-4h]

	v54 = 0;
	//LOBYTE(v1) = a1;
	v2 = a1x->life_0x8 - 1;
	hitCounter_E9B90 = 0;
	a1x->life_0x8 = v2;
	if (v2 <= 0)
	{
		a1x->actionIndex_0x45_69 = 74;
		a1x->levelPar3_0x46_70 = 0;
	}
	else
	{
		v3 = (a1x->position_0x4C_76.x + 128) >> 8;
		v1 = (a1x->position_0x4C_76.y + 128) >> 8;
		v53x._axis_2d.x = v3 - 15;
		v53x._axis_2d.y = v1 - 15;
		v52x._axis_2d.x = v3;
		v4 = a1x->levelPar3_0x46_70;
		v52x._axis_2d.y = v1;
		if (v4 <= 3u)
		{
			switch (v4)
			{
			case 0u:
				LOBYTE(v1) = FindTremorTarget_39E40(a1x);
				if ((x_BYTE)v1)
					a1x->levelPar3_0x46_70 = 1;
				else
					DisableEntityDrawing04_57F10(a1x);
				break;
			case 1u:
				v5 = GetTerrainHeightFromSquare_48DF0(v3 - 9, v1 - 9, 18, 18);
				a1x->position_0x4C_76.z = 0;
				a1x->param2_0x2C_44 = 0;
				if ((signed int)v5 > 64)
				{
					a1x->position_0x4C_76.z = v5 - 64;
					if ((signed __int16)(v5 - 64) > 16)
						a1x->param2_0x2C_44 = 32 * (v5 - 80);
				}
				a1x->mana_0x90_144 = 0;
				a1x->levelPar3_0x46_70 = 2;
				a1x->param_0x10_16 = 12;
				PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 64);
				goto LABEL_11;
			case 2u:
			LABEL_11:
				v6 = a1x->param_0x10_16 - 1;
				a1x->param_0x10_16 = v6;
				if (v6 <= 0)
				{
					a1x->levelPar3_0x46_70 = 3;
				}
				else
				{
					v7 = 0;
					for (v8x._axis_2d.y = v53x._axis_2d.y; ; ++v8x._axis_2d.y)
					{
						v51 = v7;
						if (v7 >= 30)
							break;
						v8x._axis_2d.x = v53x._axis_2d.x;
						for (i = 0; i < 30; i++)
						{
							v40x.x = v8x._axis_2d.x << 8;
							v40x.y = v8x._axis_2d.y << 8;
							v9 = Maths::EuclideanDistXYZ_58490(&a1x->position_0x4C_76, &v40x);
							v48 = v9;
							if (v9 < 3840)
							{
								if (v9 >= 2304)
								{
									v10 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v40x);
									v42x = a1x->position_0x4C_76;
									MoveEntity_57FA0(&v42x, v10, 0, 3840);
									v11 = (signed __int16)GetTerrainAlt_10C40(&v42x) >> 5;
									if (a1x->param2_0x2C_44 < v11)
										a1x->param2_0x2C_44 = v11;
									v50 = v11
										- (((x_DWORD)(0x10000 + (signed int)Maths::sin_DB750[0x200 + ((v48 - 2304) << 10) / 1536]) >> 1)
											* (v11 - (a1x->position_0x4C_76.z + 64)) >> 16);
								}
								else
								{
									v50 = a1x->position_0x4C_76.z
										+ 64
										- ((0x10000 - Maths::sin_DB750[0x200 + ((2304 - v9) << 9) / 2304]) << 6 >> 16);
								}
								v12 = (v50 - mapHeightmap_11B4E0[v8x.word]) / a1x->param_0x10_16
									+ mapHeightmap_11B4E0[v8x.word];
								if (v12 < 1)
									v12 = 1;
								if (v12 > 255)
									v12 = 255;
								v13 = isCaveLevel_D41B6;
								mapHeightmap_11B4E0[v8x.word] = v12;
								if (v13)
								{
									v50 = v12 + 64;
									if (v12 + 64 > 254)
										v50 = 254;
									v14 = (unsigned __int8)secondHeightmap_14B4E0[v8x.word];
									v15 = (unsigned __int8)secondHeightmap_14B4E0[v8x.word];
									v47x.word = v8x.word;
									v50 = v14 - (v15 - v50) / a1x->param_0x10_16;
									secondHeightmap_14B4E0[v47x.word] = v50;
								}
								v16 = a1x->position_0x4C_76.z;
								if (v12 <= v16 + 64 && v12 >= v16 + 6 * a1x->param_0x10_16 && IsSpellCastable_57450(mapTerrainType_10B4E0[v8x.word]))
								{
									v17 = mapAngle_13B4E0[v8x.word] & 0xF8;
									v54 = 1;
									mapTerrainType_10B4E0[v8x.word] = 1;
									mapAngle_13B4E0[v8x.word] = v17 | 1;
								}
							}
							v8x._axis_2d.x++;
							v8x._axis_2d.y++;
							v18x.word = v8x.word;
							v8x._axis_2d.x -= 2;
							v8x._axis_2d.y -= 2;
							v19 = mapHeightmap_11B4E0[v8x.word];
							v8x._axis_2d.x++;
							v20 = v19 - mapHeightmap_11B4E0[v18x.word] + 32;
							v8x._axis_2d.y++;
							if (v20 >= 28)
							{
								if (v20 > 40)
									v20 = (v20 & 7) + 40;
							}
							else
							{
								v20 = (v20 & 3) + 28;
							}
							if (world_D41A0.terrain_2FECE.MapType != MapType_t::Day)
								v21 = 32 - v20 + 32;
							else
								LOBYTE(v21) = v20;
							v22 = isCaveLevel_D41B6;
							mapShading_12B4E0[v8x.word] = v21;
							if (v22)
							{
								if (secondHeightmap_14B4E0[v8x.word] > mapHeightmap_11B4E0[v8x.word])
									mapAngle_13B4E0[v8x.word] &= 0xF7u;
								else
									mapAngle_13B4E0[v8x.word] |= 8u;
							}
							v8x._axis_2d.x++;
						}
						v7 = v51 + 1;
					}
					if (a1x->param_0x10_16 == 5)
					{
						KillOverlappingEntities_3A090(a1x);
						v54 = 1;
					}
					if (v54)
					{
						v23x._axis_2d.x = v52x._axis_2d.x - 15;
						v24x._axis_2d.x = v52x._axis_2d.x + 15;
						v23x._axis_2d.y = v52x._axis_2d.y - 15;
						v24x._axis_2d.y = v52x._axis_2d.y + 15;
						UpdateTerrainTiles_462A0(v23x, v24x);
					}
					v25 = 0;
					v26x._axis_2d.y = v52x._axis_2d.y - 1;
					do
					{
						v46 = 0;
						v26x._axis_2d.x = v52x._axis_2d.x - 1;
						while (v46 < 2)
						{
							v27 = mapHeightmap_11B4E0[v26x.word] - (signed int)mapHeightmap_11B4E0[v26x.word] / a1x->param_0x10_16;
							if (v27 < 0)
								v27 = 0;
							if (v27 > 255)
								LOBYTE(v27) = -1;
							mapHeightmap_11B4E0[v26x.word] = v27;
							v28 = a1x->param_0x10_16;
							v29 = 31 / v28 + 32;
							if (world_D41A0.terrain_2FECE.MapType != MapType_t::Day)
								v29 = -31 / v28 + 32;
							mapShading_12B4E0[v26x.word] = v29;
							v26x._axis_2d.x++;
							v46++;
						}
						v25++;
						v26x._axis_2d.y++;
					} while (v25 < 2);
				}
				//LOBYTE(v1) = a1;
				if (a1x->param_0x10_16 < 6)
					/*LOBYTE(v1) = */ApplyEarthquakeTiles_39B60(a1x);
				break;
			case 3u:
				v30x._axis_2d.y = v53x._axis_2d.y;
				v31 = 0;
				while (v31 < 30)
				{
					v30x._axis_2d.x = v53x._axis_2d.x;
					v32 = 0;
					while (v32 < 30)
					{
						v49x.word = v30x.word;
						if (IsSpellCastable_57450(mapTerrainType_10B4E0[v30x.word]) || mapTerrainType_10B4E0[v49x.word] == 8)
						{
							mapTerrainType_10B4E0[v30x.word] = 1;
							mapAngle_13B4E0[v30x.word] = mapAngle_13B4E0[v30x.word] & 0xF8 | 1;
						}
						v32++;
						v30x._axis_2d.x++;
					}
					v31++;
					v30x._axis_2d.y++;
				}
				v33x._axis_2d.x = v52x._axis_2d.x - 15;
				v34x._axis_2d.x = v52x._axis_2d.x + 15;
				v33x._axis_2d.y = v52x._axis_2d.y - 15;
				v34x._axis_2d.y = v52x._axis_2d.y + 15;
				AddBuildingToTerrain_46570(v33x, v34x);
				v35x._axis_2d.y = v52x._axis_2d.y - 1;
				v36 = 0;
				while (v36 < 2)
				{
					v37 = 0;
					v35x._axis_2d.x = v52x._axis_2d.x - 1;
					while (v37 < 2)
					{
						if (world_D41A0.terrain_2FECE.MapType != MapType_t::Day)
							v45 = 1;
						else
							v45 = 63;
						v38x.word = v35x.word;
						v37++;
						v35x._axis_2d.x++;
						mapShading_12B4E0[v38x.word] = v45;
					}
					v36++;
					v35x._axis_2d.y++;
				}
				ApplyEarthquakeTiles_39B60(a1x);
				//LOBYTE(v1) = a1;
				a1x->actionIndex_0x45_69 = 73;
				break;
			}
		}
	}
	//return v1;
}
// 10000: using guessed type void /*__noreturn*/ sub_10000();
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41B6: using guessed type char x_BYTE_D41B6;
// DBF50: using guessed type void (/*__noreturn*/ *off_DBF50[2])();
// E9B90: using guessed type int hitCounter_E9B90;

//----- (000396A0) --------------------------------------------------------
void UpdateEarthquake_396A0(type_entity_0x6E8E* a1x)//21a6a0
{
	//int result; // eax
	int v2; // edx

	//result = a1;
	v2 = a1x->life_0x8 - 1;
	a1x->life_0x8 = v2;
	if (v2 > 0)
	{
		/*return */ApplyEarthquakeTiles_39B60(a1x);
		return;
	}
	a1x->actionIndex_0x45_69 = 74;
	a1x->levelPar3_0x46_70 = 0;
	//return result;
}

//----- (000396D0) --------------------------------------------------------
unsigned __int8 UpdateTremorTiles_396D0(type_entity_0x6E8E* a1x)//21a6d0
{
	int v1; // eax
	__int16 v2; // dx
	unsigned __int8 resulty; // al
	//type_str_0x6E8E* resultx; // al
	signed int v4; // edi
	__int16 v5; // dx
	unsigned __int16 v6; // bx
	uaxis_2d v7x; // dx
	uaxis_2d v8x; // ax
	int v9; // edx
	unsigned __int16 v10; // bx
	__int16 v11; // ax
	__int16 v12; // ax
	int v13; // ecx
	int v14; // edx
	int v15; // ecx
	unsigned __int16 v16; // ax
	int v17; // edx
	int v18; // eax
	int v19; // edx
	char v20; // ch
	unsigned __int16 v21; // bx
	signed int v22; // edi
	signed int v23; // esi
	char v24; // al
	unsigned __int16 v25; // dx
	type_entity_0x6E8E* jx; // ecx
	char v27; // dl
	axis_3d v28x; // [esp+0h] [ebp-30h]
	//__int16 v29; // [esp+4h] [ebp-2Ch]
	axis_3d v30x; // [esp+8h] [ebp-28h]
	//__int16 v31; // [esp+Ah] [ebp-26h]
	int i; // [esp+10h] [ebp-20h]
	int k; // [esp+14h] [ebp-1Ch]
	int v34; // [esp+18h] [ebp-18h]
	int v35; // [esp+1Ch] [ebp-14h]
	int v36; // [esp+20h] [ebp-10h]
	int v37; // [esp+24h] [ebp-Ch]
	char v38; // [esp+28h] [ebp-8h]
	char v39; // [esp+2Ch] [ebp-4h]

	v1 = (a1x->position_0x4C_76.y + 128) >> 8;
	v2 = a1x->position_0x4C_76.x + 128;
	v38 = v1;
	BYTE1(v37) = v1 - 15;
	v39 = HIBYTE(v2);
	resulty = a1x->levelPar3_0x46_70;
	LOBYTE(v37) = HIBYTE(v2) - 15;
	if (resulty < 1u)
	{
		if (resulty)
			return resulty;
		ApplyEarthquakeTiles_39B60(a1x);
		KillOverlappingEntities_3A090(a1x);
		a1x->levelPar3_0x46_70 = 1;
		v4 = 0;
		v5 = a1x->position_0x4C_76.z;
		a1x->param_0x10_16 = 16;
		a1x->position_0x4C_76.z = v5 + 64;
		HIBYTE(v6) = BYTE1(v37);
		while (v4 < 30)
		{
			LOBYTE(v6) = v37;
			for (i = 0; i < 30; i++)
			{
				if (IsSpellCastable_57450(mapTerrainType_10B4E0[v6]))
				{
					mapTerrainType_10B4E0[v6] = 1;
					mapAngle_13B4E0[v6] = mapAngle_13B4E0[v6] & 0xF8 | 1;
				}
				LOBYTE(v6) = v6 + 1;
			}
			v4++;
			HIBYTE(v6)++;
		}
		/*LOBYTE(v7) = v39 - 15;
		LOBYTE(v8) = v39 + 15;
		HIBYTE(v7) = v38 - 15;
		HIBYTE(v8) = v38 + 15;*/

		v7x._axis_2d.x = v39 - 15;
		v8x._axis_2d.x = v39 + 15;
		v7x._axis_2d.y = v38 - 15;
		v8x._axis_2d.y = v38 + 15;

		/*uaxis_2d temp_v7;
		temp_v7.word = v7;
		uaxis_2d temp_v8;
		temp_v8.word = v8;*/
		UpdateTerrainTiles_462A0(v7x, v8x);
		PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 64);
	}
	else if (resulty > 1u)
	{
		if (resulty == 2)
		{
			HIBYTE(v21) = BYTE1(v37);
			v22 = 0;
			while (v22 < 30)
			{
				LOBYTE(v21) = v37;
				v23 = 0;
				while (v23 < 30)
				{
					v24 = GetSmoothedHeight_439A0(v21);
					v23++;
					v25 = v21;
					LOBYTE(v21) = v21 + 1;
					mapHeightmap_11B4E0[v25] = v24;
				}
				v22++;
				HIBYTE(v21)++;
			}
			for (jx = gameState_D41A4.class3List_38519; jx > Entities_EA3E4[0]; jx = jx->next_0)
			{
				if (jx->model_0x40_64 == 2
					&& jx->flags_0xc_12_15.byte[2] & 0x10
					&& jx->hitById_0x26_38 == a1x - world_D41A0.entities_0x6E8E)
				{
					v27 = jx->flags_0xc_12_15.byte[2];
					jx->hitById_0x26_38 = 0;
					jx->flags_0xc_12_15.byte[2] = v27 & 0xEF;
				}
			}
			DisableEntityDrawing04_57F10(a1x);
		}
		return resulty;
	}
	//resultx = a1x;
	if (!(a1x->life_0x8 & 3))
	{
		v9 = a1x->param_0x10_16 - 1;
		a1x->param_0x10_16 = v9;
		if (v9 <= 0)
		{
			a1x->levelPar3_0x46_70 = 2;
		}
		else
		{
			HIBYTE(v10) = BYTE1(v37);
			v35 = 0;
			do
			{
				LOBYTE(v10) = v37;
				for (k = 0; k < 30; k++)
				{
					v30x.x = (unsigned __int8)v10 << 8;
					v30x.y = HIBYTE(v10) << 8;
					v36 = Maths::EuclideanDistXYZ_58490(&a1x->position_0x4C_76, &v30x);
					if (v36 < 3840)
					{
						v11 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v30x);
						v28x = a1x->position_0x4C_76;
						MoveEntity_57FA0(&v28x, v11, 0, 3840);
						v12 = GetTerrainAlt_10C40(&v28x);
						v13 = (v12 >> 5)
							- (((x_DWORD)(0x10000 + (signed int)Maths::sin_DB750[0x200 + (v36 << 10) / 3840]) >> 1)
								* ((v12 >> 5) - a1x->position_0x4C_76.z) >> 16);
						a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
						v14 = (a1x->rand_0x14_20 & 3) + v13 - 2;
						v34 = mapHeightmap_11B4E0[v10];
						v34 += (v14 - v34) / a1x->param_0x10_16;
						if (v34 < 1)
							v34 = 1;
						if (v34 > 255)
							v34 = 255;
						mapHeightmap_11B4E0[v10] = v34;
						if (a1x->param_0x10_16 < 3)
							mapHeightmap_11B4E0[v10] = GetSmoothedHeight_439A0(v10);
						if (isCaveLevel_D41B6)
						{
							v15 = mapHeightmap_11B4E0[v10] + 64;
							if (v15 > 254)
								v15 = 254;
							secondHeightmap_14B4E0[v10] -= ((unsigned __int8)secondHeightmap_14B4E0[v10] - v15) / a1x->param_0x10_16;
						}
					}
					LOBYTE(v10)++;
					HIBYTE(v10)++;
					v16 = v10;
					LOBYTE(v10) -= 2;
					HIBYTE(v10) -= 2;
					v17 = mapHeightmap_11B4E0[v10];
					LOBYTE(v10)++;
					v18 = v17 - mapHeightmap_11B4E0[v16] + 32;
					HIBYTE(v10)++;
					if (v18 >= 28)
					{
						if (v18 > 40)
							v18 = (v18 & 7) + 40;
					}
					else
					{
						v18 = (v18 & 3) + 28;
					}
					if (world_D41A0.terrain_2FECE.MapType != MapType_t::Day)
						v19 = 32 - v18 + 32;
					else
						LOBYTE(v19) = v18;
					v20 = isCaveLevel_D41B6;
					mapShading_12B4E0[v10] = v19;
					if (v20)
					{
						if (secondHeightmap_14B4E0[v10] > mapHeightmap_11B4E0[v10])
							mapAngle_13B4E0[v10] &= 0xF7u;
						else
							mapAngle_13B4E0[v10] |= 8u;
					}
					//resulty = k + 1;
					LOBYTE(v10) = v10 + 1;
				}
				HIBYTE(v10)++;
				v35++;
			} while (v35 < 30);
		}
	}
	return resulty;
}
// 10000: using guessed type void /*__noreturn*/ sub_10000();
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// D41B6: using guessed type char x_BYTE_D41B6;
// DBF50: using guessed type void (/*__noreturn*/ *off_DBF50[2])();
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00039B60) --------------------------------------------------------
void ApplyEarthquakeTiles_39B60(type_entity_0x6E8E* a1x)//21ab60
{
	char v1; // al
	int i; // eax
	signed int v3; // eax
	int v4; // edx
	int v5; // esi
	int v6; // esi
	__int16 v7; // ax
	int v8; // ecx
	int v9; // eax
	type_entity_0x6E8E* v10x; // ebx
	int result; // eax
	axis_3d v12x; // [esp+0h] [ebp-1Ch]
	//__int16 v13; // [esp+2h] [ebp-1Ah]
	int v14; // [esp+8h] [ebp-14h]
	int v15; // [esp+Ch] [ebp-10h]
	unsigned __int16 v16; // [esp+10h] [ebp-Ch]
	int v17; // [esp+14h] [ebp-8h]
	int v18; // [esp+18h] [ebp-4h]

	v1 = ((unsigned __int16)(a1x->position_0x4C_76.y + 128) >> 8) - 13;
	LOBYTE(v15) = ((unsigned __int16)(a1x->position_0x4C_76.x + 128) >> 8) - 13;
	BYTE1(v15) = v1;
	v17 = 0;
	v16 = v15;
	do
	{
		v18 = 0;
		LOBYTE(v16) = v15;
		while ((signed __int16)v18 < 26)
		{
			v12x.x = (unsigned __int8)v16 << 8;
			v12x.y = HIBYTE(v16) << 8;
			if ((unsigned int)Maths::EuclideanDistXY_584D0(&a1x->position_0x4C_76, &v12x) < 0xA90000)
			{
				for (i = mapEntityIndex_15B4E0[v16]; ; i = v10x->oldMapEntity_0x16_22)
				{
					v10x = Entities_EA3E4[i];
					if (v10x == Entities_EA3E4[0])
						goto LABEL_35;
					if (CanSpellAffectEntity_39FA0(a1x, Entities_EA3E4[i]))
					{
						v3 = Maths::EuclideanDistXYZ_58490(&v10x->position_0x4C_76, &a1x->position_0x4C_76);
						v4 = a1x->param2_0x2C_44;
						v5 = v10x->position_0x4C_76.z - v4;
						v14 = v10x->position_0x4C_76.z - v4;
						if (v3 < 3328 && v5 < 4096)
							break;
					}
				LABEL_25:
					if (a1x->actionIndex_0x45_69 == 74 && v10x->flags_0xc_12_15.byte[2] & 0x10)
					{
						if (v10x->class_0x3F_63 != 3
							|| v10x->model_0x40_64
							|| v10x->playerData_0xA4_164x->playerColorIndex_0x38_56 != world_D41A0.LevelIndex_0xc)
						{
							v10x->flags_0xc_12_15.byte[0] &= 0xFEu;
						}
						else
						{
							v10x->flags_0xc_12_15.byte[0] |= 1u;
						}
						v10x->flags_0xc_12_15.byte[2] &= 0xEFu;
					}
				}
				if (v3 <= 32 || v5 <= 96)
				{
					ApplySpellToEntity_3A200(a1x, v10x);
					goto LABEL_25;
				}
				predictedAxis_EB398ar = v10x->position_0x4C_76;
				v6 = ((3328 - v3) << 8) / 3328 << 7 >> 8;
				if (v6 < 4)
					v6 = 4;
				if (v6 > 128)
					v6 = 128;
				if (v6 > v3)
					LOWORD(v6) = v3;
				v7 = Maths::GetYawAngle_581E0(&predictedAxis_EB398ar, &a1x->position_0x4C_76);
				MoveEntity_57FA0(&predictedAxis_EB398ar, v7, 0, v6);
				v8 = (signed __int16)GetTerrainAlt_10C40(&predictedAxis_EB398ar);
				if (v10x->class_0x3F_63 == 3 && !v10x->model_0x40_64)
				{
					v9 = (signed __int16)(predictedAxis_EB398ar.z
						- (48
							* ((((4096 - v14) << 8)
								- (__CFSHL__((4096 - v14) << 8 >> 31, 12)
									+ ((4096 - v14) << 8 >> 31 << 12))) >> 12) >> 8));
					predictedAxis_EB398ar.z -= 48
						* ((((4096 - v14) << 8)
							- (__CFSHL__((4096 - v14) << 8 >> 31, 12)
								+ ((4096 - v14) << 8 >> 31 << 12))) >> 12) >> 8;
					goto LABEL_21;
				}
				if (v10x->typeParams_0xA0_160x->climbSpeed_160_0xe_14 < -64)
					goto LABEL_40;
				predictedAxis_EB398ar.z -= 48
					* ((((4096 - v14) << 8) - (__CFSHL__((4096 - v14) << 8 >> 31, 12) + ((4096 - v14) << 8 >> 31 << 12))) >> 12) >> 8;
				v9 = predictedAxis_EB398ar.z;
			LABEL_21:
				if (v9 < v8)
					LABEL_40 :
					predictedAxis_EB398ar.z = v8;
				CopyEntityPosition_57CF0(v10x, &predictedAxis_EB398ar);
				goto LABEL_25;
			}
		LABEL_35:
			v18++;
			LOBYTE(v16) = v16 + 1;
		}
		result = v17++ + 1;
		HIBYTE(v16)++;
	} while ((signed __int16)v17 < 26);
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (00039E40) --------------------------------------------------------
char FindTremorTarget_39E40(type_entity_0x6E8E* a1x)//21ae40
{
	char v1; // ch
	signed int v2; // edx
	signed int v3; // esi
	unsigned __int16 v4; // ax
	signed int v5; // edi
	char v6; // bl
	signed int v7; // esi
	unsigned __int16 v8; // ax
	int v9; // edx
	int i; // edx
	unsigned __int8 v11; // ch
	char v12; // ch
	type_entity_0x6E8E* v13x; // edx
	int v15; // [esp+4h] [ebp-8h]

	v1 = (unsigned __int16)(a1x->position_0x4C_76.x + 128) >> 8;
	v2 = 0;
	v3 = 0;
	HIBYTE(v4) = ((unsigned __int16)(a1x->position_0x4C_76.y + 128) >> 8) - 15;
	while (v3 < 30)
	{
		v5 = 0;
		LOBYTE(v4) = v1 - 15;
		while (v5 < 30)
		{
			if (!mapTerrainType_10B4E0[v4])
				v2++;
			v5++;
			LOBYTE(v4) = v4 + 1;
		}
		++v3;
		++HIBYTE(v4);
	}
	if (v2 >= 225)
		return 0;
	v6 = v1 - 27;
	v7 = 0;
	HIBYTE(v8) = ((unsigned __int16)(a1x->position_0x4C_76.y + 128) >> 8) - 27;
	do
	{
		v9 = 0;
		for (LOBYTE(v8) = v6; ; LOBYTE(v8) = v8 + 1)
		{
			v15 = v9;
			if (v9 >= 54)
				break;
			for (i = mapEntityIndex_15B4E0[v8]; ; i = v13x->oldMapEntity_0x16_22)
			{
				v13x = Entities_EA3E4[i];
				if (v13x == Entities_EA3E4[0])
					break;
				if (v13x != a1x && v13x->class_0x3F_63 == 10)
				{
					v11 = v13x->model_0x40_64;
					if (v11 >= 0x2Du)
					{
						if (v11 <= 0x2Du)
						{
							v12 = v13x->actionIndex_0x45_69;
							if (v12 == 48 || v12 == 51)
								return 0;
						}
						else if (v11 == 67)
						{
							return 0;
						}
					}
				}
			}
			v9 = v15 + 1;
		}
		v7++;
		HIBYTE(v8)++;
	} while (v7 < 54);
	return 1;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0003A090) --------------------------------------------------------
void KillOverlappingEntities_3A090(type_entity_0x6E8E* a1x)//21b909
{
	type_entity_0x6E8E* ix; // ebx
	type_entity_0x6E8E* jx; // ebx
	int v3; // edi
	//uint8_t* v4; // eax
	int v5; // eax
	unsigned __int16 v6; // bx
	char v7; // al
	int v8; // [esp+0h] [ebp-10h]
	signed int k; // [esp+4h] [ebp-Ch]
	int v10; // [esp+8h] [ebp-8h]
	char v11; // [esp+Ch] [ebp-4h]

	v8 = 0;
	for (ix = gameState_D41A4.buildingList_38527; ix > Entities_EA3E4[0]; ix = ix->next_0)
	{
		if (CompareAxisWithShift_10750(a1x, ix))
		{
			ix->life_0x8 = -1;
			ix->fontTypeIndex_0x3D_61 = 0;
		}
	}
	for (jx = gameState_D41A4.class3List_38519; jx > Entities_EA3E4[0]; jx = jx->next_0)
	{
		if (jx->model_0x40_64 == 2 && CompareAxisWithShift_10750(a1x, jx))
		{
			v3 = hitCounter_E9B90 + 1;
			jx->flags_0xc_12_15.byte[2] |= 0x10u;
			hitCounter_E9B90 = v3;
			jx->maxStage_0x30_48 = 30;
			jx->hitById_0x26_38 = a1x - world_D41A0.entities_0x6E8E;
			jx->mail_0x5E_94.channel[0].amount.dword += a1x->subSpellIndex_0x2A_42;
			v8 += 2;
			jx->mail_0x5E_94.channel[0].source = a1x->id_0x1A_26;
		}
	}
	v11 = ((unsigned __int16)(a1x->position_0x4C_76.x + 128) >> 8) - 15;
	v5 = 0;
	for (HIBYTE(v6) = ((unsigned __int16)(a1x->position_0x4C_76.y + 128) >> 8) - 15; ; ++HIBYTE(v6))
	{
		v10 = v5;
		if (v5 >= 30)
			break;
		LOBYTE(v6) = v11;
		for (k = 0; k < 30; k++)
		{
			if ((unsigned int)TerrainTileType_10590(mapTerrainType_10B4E0[v6]) & 0x7F0000)
			{
				v7 = mapAngle_13B4E0[v6] & 0xF8 | 1;
				mapTerrainType_10B4E0[v6] = 1;
				mapAngle_13B4E0[v6] = v7;
			}
			LOBYTE(v6) = v6 + 1;
		}
		v5 = v10 + 1;
	}
	if (v8)
		SendSpellMessage_6D8B0(a1x->id_0x1A_26, 0x14u, v8);
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// E9B90: using guessed type int hitCounter_E9B90;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0003A2D0) --------------------------------------------------------
void UpdateShield_3A2D0(type_entity_0x6E8E* a1x)//21b2d0
{
	unsigned __int8 v1; // al
	int v2; // ecx
	int v3; // edx
	int v4; // esi
	int v5; // edi
	int v6; // esi
	int v7; // edx
	char v8; // dl
	unsigned __int16 v9; // dx
	signed int v10; // ecx
	int v11; // eax
	int v12; // edi
	unsigned __int16 v13; // dx
	int v14; // eax
	__int16 v15; // ax
	int32_t v16; // [esp+0h] [ebp-1Ch]
	int32_t v17; // [esp+4h] [ebp-18h]
	int v18; // [esp+8h] [ebp-14h]
	int v19; // [esp+Ch] [ebp-10h]
	int v20; // [esp+10h] [ebp-Ch]
	char v21; // [esp+14h] [ebp-8h]
	char v22; // [esp+18h] [ebp-4h]

	if (a1x->life_0x8 >= 0)
	{
		v1 = a1x->levelPar3_0x46_70;
		if (v1)
		{
			if (v1 > 3u)
			{
			LABEL_51:
				a1x->life_0x8--;
				return;
			}
		}
		else
		{
			v2 = a1x->maxLife_0x4;
			v3 = a1x->subSpellIndex_0x2A_42;
			a1x->param2_0x2C_44 = a1x->maxLife_0x4 >> 3;
			a1x->param_0x10_16 = 0;
			a1x->levelPar3_0x46_70 = 1;
			a1x->subSpellIndex_0x2A_42 = 4 * (unsigned int)(v3 / v2);
		}
		v4 = a1x->param2_0x2C_44;
		v5 = a1x->life_0x8;
		if (a1x->maxLife_0x4 - 3 * v4 >= v5)
		{
			if (a1x->maxLife_0x4 - 5 * v4 > v5)
			{
				v6 = a1x->param_0x10_16 - 1;
				a1x->param_0x10_16 = v6;
			}
			else
			{
				v6 = 3 * v4;
				a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
				if (!(a1x->rand_0x14_20 % 5u))
					a1x->levelPar3_0x46_70 += 2;
			}
		}
		else
		{
			v6 = a1x->param_0x10_16 + 1;
			a1x->param_0x10_16 = v6;
		}
		if (v6 < 0)
			v6 = 0;
		v7 = a1x->param2_0x2C_44;
		if (v6 > 3 * v7)
			v6 = 3 * v7;
		if (v6 < 0)
			v6 = 0;
		if (v6 > 15)
			v6 = 15;
		v8 = a1x->levelPar3_0x46_70;
		v20 = 0;
		if (v8 > 1)
		{
			v20 = 1;
			a1x->levelPar3_0x46_70 = v8 - 1;
		}
		if (v6 > 0)
		{
			v21 = (unsigned __int16)(a1x->position_0x4C_76.x + 128) >> 8;
			v22 = (unsigned __int16)(a1x->position_0x4C_76.y + 128) >> 8;
			v18 = AddE7EE0x_10080(0, v6);
			if (v18)
			{
				while (GetNextSearchOffset_10130(v18, (x_DWORD*)&v17, (x_DWORD*)&v16) == 1)
				{
					LOBYTE(v9) = v17 + v21;
					HIBYTE(v9) = v16 + v22;
					if (a1x->life_0x8 & 1)
						v10 = 1;
					else
						v10 = -1;
					v11 = v10 + mapHeightmap_11B4E0[v9];
					if (v11 < 0)
						v11 = 0;
					if (v11 > 255)
						LOBYTE(v11) = -1;
					mapHeightmap_11B4E0[v9] = v11;
				}
				ResetEvent08_10100(v18);
			}
			if (v20)
			{
				v12 = AddE7EE0x_10080(0, v6 >> 1);
				if (v12)
				{
					while (GetNextSearchOffset_10130(v12, (x_DWORD*)&v17, (x_DWORD*)&v16) == 1)
					{
						LOBYTE(v13) = v17 + v21;
						HIBYTE(v13) = v16 + v22;
						if (a1x->life_0x8 & 1)
							v19 = 1;
						else
							v19 = -1;
						v14 = v19 + mapHeightmap_11B4E0[v13];
						if (v14 < 0)
							v14 = 0;
						if (v14 > 255)
							LOBYTE(v14) = -1;
						mapHeightmap_11B4E0[v13] = v14;
					}
					ResetEvent08_10100(v12);
				}
			}
			if (!(a1x->life_0x8 & 3))
			{
				SetEntityShiftRot_49EA0(a1x, (x_WORD)v6 << 8, 2048);
				PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 10);
				v15 = SendMailToOverlappingEntities_10C80(a1x, 0, a1x->subSpellIndex_0x2A_42);
				if (v15)
					SendSpellMessage_6D8B0(a1x->id_0x1A_26, 0xFu, v15);
			}
		}
		goto LABEL_51;
	}
	DisableEntityDrawing04_57F10(a1x);
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (0003A5B0) --------------------------------------------------------
void UpdateEffectFade_3A5B0(type_entity_0x6E8E* a1x)//21b5b0
{
	int v1; // edx
	type_entity_0x6E8E* v2x; // eax
	char v3; // dl
	__int16 v4; // dx

	v1 = a1x->life_0x8 - 1;
	a1x->life_0x8 = v1;
	if (v1 < 0)
	{
		DisableEntityDrawing04_57F10(a1x);
	}
	else if (!v1)
	{
		v2x = CreateEntity_4A190(&a1x->position_0x4C_76, 5, a1x->levelPar3_0x46_70);
		if (v2x)
		{
			v3 = 8 * v2x->model_0x40_64;
			v2x->StageVar2_0x49_73 = 13;
			v2x->actionIndex_0x45_69 = v3 + 7;
			v2x->parentId_0x28_40 = a1x->parentId_0x28_40;
			v4 = a1x->id_0x1A_26;
			v2x->targetId_0x96_150 = 0;
			v2x->mana_0x90_144 = 0;
			v2x->maxMana_0x8C_140 = 0;
			v2x->stage_0x2E_46 = 250;
			v2x->id_0x1A_26 = v4;
		}
	}
}

//----- (0003A630) --------------------------------------------------------
void UpdateEffectCountdown_3A630(type_entity_0x6E8E* a1x)//21b630
{
	//int result; // eax
	int v2; // edx

	//result = a1;
	v2 = a1x->life_0x8 - 1;
	a1x->life_0x8 = v2;
	if (v2 <= 0)
		DisableEntityDrawing04_57F10(a1x);
	//return result;
}

//----- (0003A650) --------------------------------------------------------
int FollowTargetEntity_3A650(type_entity_0x6E8E* a1x)//21b650
{
	type_entity_0x6E8E* v1x; // edi
	char v2; // al
	int k; // eax
	__int16 v4; // ax
	__int16 v5; // ax
	type_entity_0x6E8E* v6x; // ebx
	signed __int16 v8; // [esp+0h] [ebp-14h]
	char v9; // [esp+4h] [ebp-10h]
	char v10; // [esp+8h] [ebp-Ch]
	unsigned __int8 j; // [esp+Ch] [ebp-8h]
	unsigned __int8 i; // [esp+10h] [ebp-4h]

	if (a1x->targetId_0x96_150)
	{
		v1x = Entities_EA3E4[a1x->targetId_0x96_150];
		if (v1x > Entities_EA3E4[0])
		{
			v2 = a1x->levelPar3_0x46_70;
			v10 = ((unsigned __int16)(v1x->position_0x4C_76.x + 128) >> 8) - v2;
			v8 = 2 * a1x->levelPar3_0x46_70;
			v9 = ((unsigned __int16)(v1x->position_0x4C_76.y + 128) >> 8) - v2;
			for (i = 0; i < v8; i++)
			{
				for (j = 0; j < v8; j++)
				{
					for (k = mapEntityIndex_15B4E0[(unsigned __int8)(j + v10) + ((unsigned __int8)(v9 + i) << 8)];
						;
						k = v6x->oldMapEntity_0x16_22)
					{
						v6x = Entities_EA3E4[k];
						if (v6x <= Entities_EA3E4[0])
							break;
						if (v6x->class_0x3F_63 == v1x->class_0x3F_63
							&& v6x->model_0x40_64 == v1x->model_0x40_64
							&& IsCreatureAffectable_3A7F0(v6x))
						{
							PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 6);
							v6x->StageVar2_0x49_73 = 14;
							v4 = v6x->StageVar1_0x48_72;
							v6x->StageVar1_0x48_72 = 0;
							v6x->nextStage_0x4A_74 = v4;
							v6x->parentId_0x28_40 = a1x->id_0x1A_26;
							v5 = a1x->subSpellIndex_0x2A_42;
							v6x->stage_0x2E_46 = v5;
							v6x->maxStage_0x30_48 = v5;
							if ((v6x->actionIndex_0x45_69 & 7) == 2)
								v6x->targetId_0x96_150 = 0;
							else
								v6x->actionIndex_0x45_69 = 8 * v6x->model_0x40_64 + 7;
						}
					}
				}
			}
		}
	}
	DisableEntityDrawing04_57F10(a1x);
	return 1;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0004AC40) --------------------------------------------------------
type_entity_0x6E8E* AddTree_4AC40(axis_3d* position)//22bc40 Spawn ??
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 0;
		event->class_0x3F_63 = 2;
		event->model_0x40_64 = 0;
		event->param_0x10_16 = (event - world_D41A0.entities_0x6E8E) % 11;
		event->rand_0x14_20 = 9377 * event->rand_0x14_20 + 9439;
		event->mailMask_0x38_56 = 1;
		event->life_0x8 = event->rand_0x14_20 % 0x1388u + 2500;
		event->rand_0x14_20 = 9377 * event->rand_0x14_20 + 9439;
		position->x += (event->rand_0x14_20 & 0x3F) - 32;
		event->rand_0x14_20 = 9377 * event->rand_0x14_20 + 9439;
		position->y += (event->rand_0x14_20 & 0x3F) - 32;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		event->rand_0x14_20 = 9377 * event->rand_0x14_20 + 9439;
		if (event->rand_0x14_20 & 1)
		{
			if ((event->rand_0x14_20 & 1) == 1)
				SetHalfSpeedEntity_49DA0(event, 84);
		}
		else
		{
			SetHalfSpeedEntity_49DA0(event, 83);
		}
	}
	return event;
}

//----- (0004AD70) --------------------------------------------------------
type_entity_0x6E8E* AddStone_4AD70(axis_3d* position)//22bd70 //Spawn ??
{
	type_entity_0x6E8E* entity = NewEvent_4A050();
	if (entity)
	{
		entity->flags_0xc_12_15.byte[0] &= 0xF7u;
		entity->param_0x10_16 = (entity - world_D41A0.entities_0x6E8E) % 11;
		entity->actionIndex_0x45_69 = 3;
		entity->class_0x3F_63 = 2;
		entity->model_0x40_64 = 1;
		AddEventToMap_57D70(entity, position);
		CopyMaxLifeToLife_49A20(entity);
		SetHalfSpeedEntity_49DA0(entity, 79);
	}
	return entity;
}

//----- (0004ADF0) --------------------------------------------------------
type_entity_0x6E8E* AddDolmen_4ADF0(axis_3d* position)//22bdf0 - add scenery Dolmen(Similar as Obelisk)
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->flags_0xc_12_15.byte[0] &= 0xF7u;
		event->param_0x10_16 = (event - world_D41A0.entities_0x6E8E) % 11;
		event->actionIndex_0x45_69 = 6;
		event->class_0x3F_63 = 2;
		event->model_0x40_64 = 2;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetHalfSpeedEntity_49DA0(event, 39);
		SetEntityShiftRot_49EA0(event, 1024, 1024);
	}
	return event;
}

//----- (0004AE80) --------------------------------------------------------
type_entity_0x6E8E* AddSceneryType3_4AE80(axis_3d* position)//22be80 // Spawn Creture 3
{
	type_entity_0x6E8E* v1x = NewEvent_4A050();
	if (v1x)
	{
		v1x->flags_0xc_12_15.byte[0] &= 0xF7u;
		v1x->param_0x10_16 = (v1x - world_D41A0.entities_0x6E8E) % 11;
		v1x->actionIndex_0x45_69 = 9;
		v1x->class_0x3F_63 = 2;
		v1x->model_0x40_64 = 3;
		AddEventToMap_57D70(v1x, position);
		CopyMaxLifeToLife_49A20(v1x);
		SetHalfSpeedEntity_49DA0(v1x, 270);
	}
	return v1x;
}

//----- (0004AF00) --------------------------------------------------------
type_entity_0x6E8E* AddSceneryType4_4AF00(axis_3d* position)//22bf00 // Spawn 4??
{
	type_entity_0x6E8E* v1x = NewEvent_4A050();
	if (v1x)
	{
		v1x->actionIndex_0x45_69 = 12;
		v1x->class_0x3F_63 = 2;
		v1x->model_0x40_64 = 4;
		v1x->param_0x10_16 = (v1x - world_D41A0.entities_0x6E8E) % 11;
		AddEventToMap_57D70(v1x, position);
		CopyMaxLifeToLife_49A20(v1x);
		SetHalfSpeedEntity_49DA0(v1x, 48);
	}
	return v1x;
}

//----- (0004AF70) --------------------------------------------------------
type_entity_0x6E8E* AddSceneryType5_4AF70(axis_3d* position)//22bf70 //Spawn 5??
{
	type_entity_0x6E8E* v1x = NewEvent_4A050();
	if (v1x)
	{
		v1x->actionIndex_0x45_69 = 15;
		v1x->class_0x3F_63 = 2;
		v1x->model_0x40_64 = 5;
		v1x->param_0x10_16 = (v1x - world_D41A0.entities_0x6E8E) % 11;
		AddEventToMap_57D70(v1x, position);
		CopyMaxLifeToLife_49A20(v1x);
		SetHalfSpeedEntity_49DA0(v1x, 48);
	}
	return v1x;
}

//----- (0004AFE0) --------------------------------------------------------
type_entity_0x6E8E* AddSceneryType6_4AFE0(axis_3d* position)//22bfe0 //Spawn Creture Bee
{
	type_entity_0x6E8E* v1x; // eax
	type_entity_0x6E8E* v2x; // ebx
	unsigned __int16 v4; // ax
	axis_3d v6x; // [esp+0h] [ebp-8h]
	if (world_D41A0.terrain_2FECE.MapType != MapType_t::Cave)
		return 0;
	v1x = NewEvent_4A050();
	v2x = v1x;
	if (!v1x)
		return 0;
	v1x->actionIndex_0x45_69 = 18;
	v1x->class_0x3F_63 = 2;
	v1x->model_0x40_64 = 6;
	v1x->rand_0x14_20 = 9377 * v1x->rand_0x14_20 + 9439;
	v4 = v1x->rand_0x14_20;
	v2x->mailMask_0x38_56 = 1;
	v2x->maxLife_0x4 = v4 % 0x50u + 100;
	v6x = *position;
	v2x->rand_0x14_20 = 9377 * v2x->rand_0x14_20 + 9439;
	v6x.x += (v2x->rand_0x14_20 & 0x3F) - 32;
	v2x->rand_0x14_20 = 9377 * v2x->rand_0x14_20 + 9439;
	v6x.y += (v2x->rand_0x14_20 & 0x3F) - 32;
	AddEventToMap_57D70(v2x, &v6x);
	CopyMaxLifeToLife_49A20(v2x);
	v2x->rand_0x14_20 = 9377 * v2x->rand_0x14_20 + 9439;
	SetHalfSpeedEntity_49DA0(v2x, (v2x->rand_0x14_20 & 3) + 324);
	return v2x;
}

//----- (0004B0F0) --------------------------------------------------------
type_entity_0x6E8E* AddSceneryType7_4B0F0(axis_3d* position)//22c0f0
{
	type_entity_0x6E8E* event; // eax
	if (world_D41A0.terrain_2FECE.MapType == MapType_t::Cave)
		event = 0;
	else
		event = AddSceneryWithSprite_4B150(position, 7, 20, 322);
	return event;
}

//----- (0004B120) --------------------------------------------------------
type_entity_0x6E8E* AddSceneryType8_4B120(axis_3d* position)//22c120
{
	type_entity_0x6E8E* event; // eax
	if (world_D41A0.terrain_2FECE.MapType == MapType_t::Cave)
		event = 0;
	else
		event = AddSceneryWithSprite_4B150(position, 8, 21, 323);
	return event;
}

//----- (0004B150) --------------------------------------------------------
type_entity_0x6E8E* AddSceneryWithSprite_4B150(axis_3d* position, char a2, char a3, __int16 a4)//22c150
{
	type_entity_0x6E8E* v4x; // eax
	type_entity_0x6E8E* v5x; // ebx
	unsigned __int16 v7; // ax
	axis_3d v9x; // [esp+0h] [ebp-8h]
	v4x = NewEvent_4A050();
	v5x = v4x;
	if (v4x)
	{
		v4x->actionIndex_0x45_69 = a3;
		v4x->model_0x40_64 = a2;
		v4x->rand_0x14_20 = 9377 * v4x->rand_0x14_20 + 9439;
		v7 = v4x->rand_0x14_20;
		v5x->class_0x3F_63 = 2;
		v5x->mailMask_0x38_56 = 1;
		v5x->levelPar3_0x46_70 = 0;
		v5x->param2_0x2C_44 = -128;
		v5x->actSpeed_0x82_130 = 0;
		v5x->maxLife_0x4 = v7 % 0x7D0u + 400;
		v9x = *position;
		v5x->rand_0x14_20 = 9377 * v5x->rand_0x14_20 + 9439;
		v9x.x += (v5x->rand_0x14_20 & 0x3F) - 32;
		v5x->rand_0x14_20 = 9377 * v5x->rand_0x14_20 + 9439;
		v9x.y += (v5x->rand_0x14_20 & 0x3F) - 32;
		AddEventToMap_57D70(v5x, &v9x);
		CopyMaxLifeToLife_49A20(v5x);
		SetHalfSpeedEntity_49DA0(v5x, a4);
	}
	return v5x;
}

//----- (0004E320) --------------------------------------------------------
type_entity_0x6E8E* AddImpactExplosion_4E320(axis_3d* position)//22f320 //tex 38
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->maxLife_0x4 = 8;
		event->actionIndex_0x45_69 = 0;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0;
		event->subSpellIndex_0x2A_42 = 400;
		event->mailMask_0x38_56 = 0;
		event->flags_0xc_12_15.dword &= 0xFFFDFFF7;
		event->flags_0xc_12_15.byte[2] |= 2u;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 7);//problem
		SetEntityShiftRot_49EA0(event, 128, 128);
		AddDynamicLight_847D0(event, 128, 7, 1);
	}
	return event;
}

//----- (0004E3B0) --------------------------------------------------------
type_entity_0x6E8E* AddDeathExplosion_4E3B0(axis_3d* position)//22f3b0 - add effect Big explosion //tex 54
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 1;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 1;
		event->maxLife_0x4 = 1;
		event->subSpellIndex_0x2A_42 = 400;
		event->flags_0xc_12_15.dword &= 0xFFFDFFF7;
		event->flags_0xc_12_15.byte[2] |= 2u;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 41);
		AddDynamicLight_847D0(event, 128, 7, 1);
	}
	return event;
}

//----- (0004E430) --------------------------------------------------------
type_entity_0x6E8E* AddSpeedSpellEffect_4E430(axis_3d* position)//22f430
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->maxLife_0x4 = 8;
		event->actionIndex_0x45_69 = 2;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 2;
		event->position_0x4C_76 = *position;
		event->param_0x10_16 = 0;
		event->flags_0xc_12_15.dword = event->flags_0xc_12_15.dword & 0xFFFDFFF6 | 0x20001;
		CopyMaxLifeToLife_49A20(event);
	}
	return event;
}

//----- (0004E490) --------------------------------------------------------
type_entity_0x6E8E* AddWizardGhost_4E490(axis_3d* position)//22f490 //tex 0
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->maxLife_0x4 = 7;
		event->actionIndex_0x45_69 = 3;
		event->class_0x3F_63 = 10;
		event->model_0x40_64 = 3;
		event->subSpellIndex_0x2A_42 = 0;
		event->flags_0xc_12_15.dword &= 0xFFFDFFF7;
		event->param_0x10_16 = 0;
		event->flags_0xc_12_15.byte[2] |= 2;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 36);
	}
	return event;
}

//----- (0004E500) --------------------------------------------------------
type_entity_0x6E8E* NewAdd0A04_4E500(axis_3d* position)//22f500
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->maxLife_0x4 = 100;
		event->actionIndex_0x45_69 = 4;
		event->class_0x3F_63 = 10;
		event->model_0x40_64 = 4;
		event->subSpellIndex_0x2A_42 = 0;
		event->flags_0xc_12_15.dword &= 0xFFFDFFF7;
		event->flags_0xc_12_15.byte[2] |= 2u;
		event->param_0x10_16 = 0;
		event->position_0x4C_76 = *position;
		event->position_0x4C_76.z = GetTerrainAlt_10C40(&event->position_0x4C_76);
		CopyMaxLifeToLife_49A20(event);
	}
	return event;
}

//----- (0004E570) --------------------------------------------------------
type_entity_0x6E8E* AddAsh_4E570(axis_3d* position)//22f570 //tex 186
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->maxLife_0x4 = 8;
		event->actionIndex_0x45_69 = 5;
		event->class_0x3F_63 = 10;
		event->model_0x40_64 = 5;
		event->subSpellIndex_0x2A_42 = 0;
		event->flags_0xc_12_15.dword &= 0xFFFDFFF7;
		event->param_0x10_16 = 0;
		event->flags_0xc_12_15.byte[2] |= 2;
		AddEventToMap_57D70(event, position);
		event->position_0x4C_76.z = GetTerrainAlt_10C40(&event->position_0x4C_76);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 244);
	}
	return event;
}

//----- (0004E5F0) --------------------------------------------------------
type_entity_0x6E8E* AddFire_4E5F0(axis_3d* position)//22f5f0 //tex 77
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 6;
		event->class_0x3F_63 = 10;
		event->model_0x40_64 = 6;
		event->subSpellIndex_0x2A_42 = 50;
		event->maxLife_0x4 = 240;
		event->param2_0x2C_44 = 0;
		event->flags_0xc_12_15.dword &= 0xFFFDFFF7;
		event->flags_0xc_12_15.byte[2] |= 2u;
		AddEventToMap_57D70(event, position);
		event->position_0x4C_76.z = GetTerrainAlt_10C40(position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 228);
		SetEntityShiftRot_49EA0(event, 272, 1536);
		event->param_0x10_16 = 0;
		AddDynamicLight_847D0(event, 80, 11, 1);
	}
	return event;
}

//----- (0004E6A0) --------------------------------------------------------
type_entity_0x6E8E* AddEffect0A07_4E6A0(axis_3d* position)//22f6a0
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 7;
		event->class_0x3F_63 = 10;
		event->model_0x40_64 = 7;
		event->maxLife_0x4 = 12;
		event->flags_0xc_12_15.dword &= 0xFFFDFFF7;
		event->flags_0xc_12_15.byte[2] |= 2u;
		event->rand_0x14_20 = 9377 * event->rand_0x14_20 + 9439;
		event->xtype_0x41_65 = 10;
		event->xsubtype_0x42_66 = 7;
		event->actSpeed_0x82_130 = (event->rand_0x14_20 % 0x14u) + 20;
		AddEventToMap_57D70(event, position);
		event->position_0x4C_76.z = GetTerrainAlt_10C40(position);
		SetHalfSpeedEntity_49DA0(event, 78);
		CopyMaxLifeToLife_49A20(event);
		event->flags_0xc_12_15.byte[0] |= 1u;
	}
	return event;
}

type_entity_0x6E8E* AddEffect0A08_4E750()//22f750
{
	return 0;
}

//----- (0004E760) --------------------------------------------------------
type_entity_0x6E8E* AddManaSphereSpawner_4E760(axis_3d* position)//22f760
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 9;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 9;
		event->maxLife_0x4 = 11;
		event->life_0x8 = 17;
		event->position_0x4C_76 = *position;
		event->subSpellIndex_0x2A_42 = 2000;
		event->flags_0xc_12_15.byte[0] &= 0xF7u;
		SetEntityShiftRot_49EA0(event, 7, 0x4000);
		world_D41A0.altSpawnMode_0x36E03 = 0;
	}
	return event;
}

//----- (0004E7D0) --------------------------------------------------------
type_entity_0x6E8E* AddSpawnAround_4E7D0(axis_3d* position)//22f7d0
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 10;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0xA;
		event->maxLife_0x4 = 1;
		event->position_0x4C_76 = *position;
		event->subSpellIndex_0x2A_42 = 100;
		event->flags_0xc_12_15.dword &= 0xFFFDFFF7;
		event->flags_0xc_12_15.byte[2] |= 2u;
		CopyMaxLifeToLife_49A20(event);
		SetEntityShiftRot_49EA0(event, 128, 128);
	}
	return event;
}

//----- (0004E840) --------------------------------------------------------
type_entity_0x6E8E* AddSpellBlast_4E840(axis_3d* position)//22f840
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 11;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0xB;
		event->maxLife_0x4 = 40;
		event->position_0x4C_76 = *position;
		event->subSpellIndex_0x2A_42 = 200;
		event->flags_0xc_12_15.dword &= 0xFFFDFFF7;
		event->hitById_0x26_38 = 11;
		event->flags_0xc_12_15.byte[2] |= 2;
		CopyMaxLifeToLife_49A20(event);
		SetEntityShiftRot_49EA0(event, 2304, 0x2000);
	}
	return event;
}

//----- (0004E8C0) --------------------------------------------------------
type_entity_0x6E8E* AddPossessHitEffect_4E8C0(axis_3d* position)//22f8c0 //tex 54
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 12;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0xC;
		event->maxLife_0x4 = 8;
		event->position_0x4C_76 = *position;
		event->subSpellIndex_0x2A_42 = 64000;
		event->flags_0xc_12_15.byte[0] = event->flags_0xc_12_15.byte[0] & 0xF6 | 1;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 41);
		event->flags_0xc_12_15.byte[0] |= 1u;
		SetEntityShiftRot_49EA0(event, 512, 512);
	}
	return event;
}

//----- (0004E950) --------------------------------------------------------
type_entity_0x6E8E* AddEffectLifetime_4E950(axis_3d* position)//22f950 //tex 54
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 0x4D;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x46;
		event->maxLife_0x4 = 8;
		event->position_0x4C_76 = *position;
		event->subSpellIndex_0x2A_42 = 64000;
		event->flags_0xc_12_15.byte[0] = event->flags_0xc_12_15.byte[0] & 0xF6 | 1;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 41);
		event->flags_0xc_12_15.byte[0] |= 1u;
		SetEntityShiftRot_49EA0(event, 512, 512);
	}
	return event;
}

//----- (0004E9E0) --------------------------------------------------------
type_entity_0x6E8E* SetParticleSmoke3B_4E9E0(axis_3d* position)//22f9e0
{
	world_D41A0.rand_0x8 = 9377 * world_D41A0.rand_0x8 + 9439;
	return SetSmoke4_4EAA0(position, 0xD, 0xD, 67, world_D41A0.rand_0x8 % 0x17u + 17);
}

//----- (0004EA20) --------------------------------------------------------
type_entity_0x6E8E* SetParticleSmoke3C_4EA20(axis_3d* position)//22fa20
{
	world_D41A0.rand_0x8 = 9377 * world_D41A0.rand_0x8 + 9439;
	return SetSmoke4_4EAA0(position, 0xE, 0xE, 9, world_D41A0.rand_0x8 % 0x21u + 28);
}

//----- (0004EA60) --------------------------------------------------------
type_entity_0x6E8E* AddRandomSmoke_4EA60(axis_3d* position)//22fa60
{
	world_D41A0.rand_0x8 = 9377 * world_D41A0.rand_0x8 + 9439;
	return SetSmoke4_4EAA0(position, 0x57, 0x5E, 67, world_D41A0.rand_0x8 % 0x17u + 17);
}

//----- (0004EAA0) --------------------------------------------------------
type_entity_0x6E8E* SetSmoke4_4EAA0(axis_3d* position, char a2, char a3, __int16 entityIndex, int a5)//22faa0
{
	type_entity_0x6E8E* tempevent = NewEvent_4A050();
	if (tempevent)
	{
		tempevent->actionIndex_0x45_69 = a3;
		tempevent->flags_0xc_12_15.dword &= 0xFFFDFFF7;
		tempevent->model_0x40_64 = a2;
		tempevent->maxLife_0x4 = a5;
		tempevent->rand_0x14_20 = 9377 * tempevent->rand_0x14_20 + 9439;//mybe must fix
		tempevent->class_0x3F_63 = 0xA;
		tempevent->maxSpeed_0x86_134 = 30;
		tempevent->xtype_0x41_65 = 10;
		tempevent->xsubtype_0x42_66 = a2;
		tempevent->actSpeed_0x82_130 = tempevent->rand_0x14_20 % 0x35 + 51;
		tempevent->flags_0xc_12_15.byte[2] |= 2;
		AddEventToMap_57D70(tempevent, position);
		SetHalfSpeedEntity_49DA0(tempevent, entityIndex);
		CopyMaxLifeToLife_49A20(tempevent);
	}
	return tempevent;
}

//----- (0004EC10) --------------------------------------------------------
type_entity_0x6E8E* AddSmoke_4EC10(axis_3d* position)//22fc10- add effect ??
{
	type_entity_0x6E8E* tempevent; // eax
	if (GetFreeEntityCount_4A810() < 32)
		return 0;
	tempevent = NewEvent_4A050();
	if (!tempevent)
		return 0;
	tempevent->actionIndex_0x45_69 = 0x41;
	tempevent->class_0x3F_63 = 0xA;
	tempevent->model_0x40_64 = 0x3C;
	tempevent->rand_0x14_20 = 9377 * tempevent->rand_0x14_20 + 9439;
	tempevent->maxLife_0x4 = tempevent->rand_0x14_20 % 0x64u + 800;
	tempevent->flags_0xc_12_15.byte[0] = (tempevent->flags_0xc_12_15.byte[0] & 0xF6) | 1;
	tempevent->rand_0x14_20 = 9377 * tempevent->rand_0x14_20 + 9439;
	tempevent->actSpeed_0x82_130 = tempevent->rand_0x14_20 % 0x11u;
	tempevent->position_0x4C_76 = *position;
	CopyMaxLifeToLife_49A20(tempevent);
	return tempevent;
}

//----- (0004ECD0) --------------------------------------------------------
type_entity_0x6E8E* AddEmberSpawner_4ECD0(axis_3d* position)//22fcd0
{
	type_entity_0x6E8E* v1x; // eax
	v1x = NewEvent_4A050();
	if (v1x)
	{
		v1x->actionIndex_0x45_69 = 15;
		v1x->class_0x3F_63 = 10;
		v1x->model_0x40_64 = 15;
		v1x->maxLife_0x4 = 128;
		v1x->actSpeed_0x82_130 = 256;
		v1x->flags_0xc_12_15.byte[0] &= 0xF7u;
		v1x->subSpellIndex_0x2A_42 = 100;
		v1x->rand_0x14_20 = 9377 * v1x->rand_0x14_20 + 9439;
		v1x->param_0x10_16 = 0;
		v1x->yaw_0x1C_28 = v1x->rand_0x14_20 & 0x7FF;
		v1x->position_0x4C_76 = *position;
		CopyMaxLifeToLife_49A20(v1x);
		SetEntityShiftRot_49EA0(v1x, 1024, 0x4000);
	}
	return v1x;
}

//----- (0004ED70) --------------------------------------------------------
type_entity_0x6E8E* AddMeteor_4ED70(axis_3d* position)//22fd70
{
	type_entity_0x6E8E* entity= NewEvent_4A050();
	if (entity)
	{
		entity->actionIndex_0x45_69 = 17;
		entity->class_0x3F_63 = 10;
		entity->model_0x40_64 = 17;
		entity->position_0x4C_76 = *position;
		entity->maxLife_0x4 = 10;
		entity->subSpellIndex_0x2A_42 = 3000;
		entity->flags_0xc_12_15.byte[0] &= 0xF7u;
		CopyMaxLifeToLife_49A20(entity);
	}
	return entity;
}

//----- (0004EDC0) --------------------------------------------------------
type_entity_0x6E8E* AddVolcanoRock_4EDC0(axis_3d* position)//22fdc0 //tex 145
{
	type_entity_0x6E8E* v1x = NewEvent_4A050();
	if (v1x)
	{
		v1x->actionIndex_0x45_69 = 16;
		v1x->class_0x3F_63 = 10;
		v1x->model_0x40_64 = 16;
		v1x->flags_0xc_12_15.dword &= 0xFFFDFFF7;
		v1x->subSpellIndex_0x2A_42 = 200;
		v1x->rand_0x14_20 = 9377 * v1x->rand_0x14_20 + 9439;
		v1x->maxLife_0x4 = v1x->rand_0x14_20 % 0x64u + 100;
		v1x->rand_0x14_20 = 9377 * v1x->rand_0x14_20 + 9439;
		v1x->actSpeed_0x82_130 = (v1x->rand_0x14_20 % 0x32u) + 52;
		v1x->rand_0x14_20 = 9377 * v1x->rand_0x14_20 + 9439;
		v1x->param2_0x2C_44 = 256;
		v1x->yaw_0x1C_28 = v1x->rand_0x14_20 & 0x7FF;
		v1x->flags_0xc_12_15.byte[2] |= 2;
		AddEventToMap_57D70(v1x, position);
		v1x->position_0x4C_76.z = GetTerrainAlt_10C40(position) + 64;
		MoveEntity_57FA0(&v1x->axis_0x9A_154x, v1x->yaw_0x1C_28, 0, v1x->actSpeed_0x82_130);
		CopyMaxLifeToLife_49A20(v1x);
		SetEntityIndexAndRot_49CD0(v1x, 210);
	}
	return v1x;
}

//----- (0004EED0) --------------------------------------------------------
type_entity_0x6E8E* AddVolcanoEruption_4EED0(axis_3d* position)//22fed0
{
	type_entity_0x6E8E* v1x; // eax
	v1x = NewEvent_4A050();
	if (v1x)
	{
		v1x->actionIndex_0x45_69 = 18;
		v1x->class_0x3F_63 = 10;
		v1x->model_0x40_64 = 18;
		v1x->subSpellIndex_0x2A_42 = 200;
		v1x->param_0x10_16 = 0;
		v1x->maxLife_0x4 = 10000;
		v1x->flags_0xc_12_15.byte[0] &= 0xF7;
		v1x->position_0x4C_76 = *position;
		CopyMaxLifeToLife_49A20(v1x);
	}
	return v1x;
}

//----- (0004EF30) --------------------------------------------------------
type_entity_0x6E8E* AddManaSphereSpawn_4EF30(axis_3d* a1x)//22ff30
{
	type_entity_0x6E8E* v1x; // eax
	//uint8_t* v2; // ebx
	//char v3; // ah

	v1x = NewEvent_4A050();
	//v2 = v1;
	if (v1x)
	{
		v1x->actionIndex_0x45_69 = 98;
		v1x->class_0x3F_63 = 10;
		v1x->model_0x40_64 = 91;
		v1x->subSpellIndex_0x2A_42 = 200;
		//v3 = *(x_BYTE *)(v1 + 12);
		v1x->param_0x10_16 = 0;
		//v3 &= 0xF6u;
		v1x->maxLife_0x4 = 10000;
		v1x->flags_0xc_12_15.byte[0] &= 0xF6u;
		v1x->flags_0xc_12_15.byte[0] |= 1;
		v1x->position_0x4C_76 = *a1x;
		CopyMaxLifeToLife_49A20(v1x);
	}
	return v1x;
}

//----- (0004EF90) --------------------------------------------------------
type_entity_0x6E8E* AddVolcanoFire_4EF90(axis_3d* a1x)//22ff90 //tex 77
{
	type_entity_0x6E8E* v1x; // eax
	//uint8_t* v2; // ebx
	//int v3; // edx

	v1x = NewEvent_4A050();
	//v2 = v1;
	if (v1x)
	{
		v1x->actionIndex_0x45_69 = 19;
		v1x->class_0x3F_63 = 10;
		v1x->model_0x40_64 = 19;
		v1x->subSpellIndex_0x2A_42 = 200;
		//v3 = *(x_DWORD *)(v1 + 12);
		v1x->maxLife_0x4 = 240;
		v1x->flags_0xc_12_15.dword &= 0xFFFDFFF7;
		v1x->flags_0xc_12_15.byte[2] |= 2u;
		AddEventToMap_57D70(v1x, a1x);
		v1x->flags_0xc_12_15.byte[0] |= 1u;
		CopyMaxLifeToLife_49A20(v1x);
		SetEntityIndexAndRot_49CD0(v1x, 228);
		SetEntityShiftRot_49EA0(v1x, 512, 512);
	}
	return v1x;
}

//----- (0004F040) --------------------------------------------------------
type_entity_0x6E8E* AddWind_4F040(axis_3d* axis)//230040
{
	if (GetFreeEntityCount_4A810() < 12)
		return 0;
	type_entity_0x6E8E* entity = NewEvent_4A050();
	if (!entity)
		return 0;
	entity->actionIndex_0x45_69 = 22;
	entity->class_0x3F_63 = 10;
	entity->model_0x40_64 = 22;
	entity->param2_0x2C_44 = 0;
	entity->stage_0x2E_46 = 1;
	entity->counter_0x3C_60 = 0;
	entity->minSpeed_0x84_132 = 20;
	entity->maxSpeed_0x86_134 = 10;
	entity->actSpeed_0x82_130 = 50;
	entity->maxLife_0x4 = 500;
	entity->subSpellIndex_0x2A_42 = 1000;
	entity->flags_0xc_12_15.byte[0] &= 0xF7u;
	entity->rand_0x14_20 = 9377 * entity->rand_0x14_20 + 9439;
	entity->fov_0x22_34 = 0;
	entity->mailMask_0x38_56 = 1;
	entity->roll_0x20_32 = (entity->rand_0x14_20 & 0x7FF) - 1;
	entity->yaw_0x1C_28 = (entity->rand_0x14_20 & 0x7FF) - 1;
	type_entity_0x6E8E* entity3 = entity;
	entity->pitch_0x1E_30 = entity->roll_0x20_32;
	CopyMaxLifeToLife_49A20(entity);
	for (int i = 0; i < 11; i++)
	{
		type_entity_0x6E8E* entity2 = NewEvent_4A050();
		type_entity_0x6E8E* entity4 = entity2;
		if (entity2)
		{
			qmemcpy(entity2, entity, 0xA8u);
			entity2->model_0x40_64 = 75;
			entity2->actionIndex_0x45_69 = 82;
			entity2->param2_0x2C_44 = i + 1;
			entity2->leaderId_0x32_50 = entity3 - world_D41A0.entities_0x6E8E;
			entity3->nextLinkedId_0x34_52 = entity2 - world_D41A0.entities_0x6E8E;
			entity2->nextLinkedId_0x34_52 = 0;
			entity2->instanceIndex_0x3E_62 = i;
			AddEventToMap_57D70(entity2, axis);
		}
		entity3 = entity4;
	}
	AddEventToMap_57D70(entity, axis);
	SetChainToTerrainHeight_4F1C0(entity);
	return entity;
}

//----- (0004F1C0) --------------------------------------------------------
unsigned __int16 SetChainToTerrainHeight_4F1C0(type_entity_0x6E8E* a1x)//230c10
{
	type_entity_0x6E8E* v1x; // ebx
	int v2; // esi
	unsigned __int16 result; // ax
	int v4; // ecx
	unsigned __int16 v5; // di
	int v6; // ST0C_4
	int v7; // edi
	unsigned __int16 i; // [esp+4h] [ebp-8h]

	v1x = a1x;
	v2 = 0;
	result = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
	for (i = result; v1x > Entities_EA3E4[0]; v1x = Entities_EA3E4[result])
	{
		v4 = v1x->param2_0x2C_44 + 293;
		v5 = spriteDefsTable_D951C[v4].worldSize_8;
		v6 = 550 * v5 / 1000;
		v7 = 450 * v5 / 1000;
		SetEntityIndexAndRot_49CD0(v1x, v4);
		SetEntityShiftRot_49EA0(v1x, v6, v7);
		v1x->position_0x4C_76.z = v2 + i;
		result = v1x->nextLinkedId_0x34_52;
		v1x->param3_0x36_54 = v2;
		v2 += 2 * v7;
	}
	return result;
}
// D9524: using guessed type __int16 x_WORD_D9524[];
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0004F2A0) --------------------------------------------------------
type_entity_0x6E8E* AddFireSpheres_4F2A0(axis_3d* position)//2302a0
{	
	type_entity_0x6E8E* entity = NULL;
	if (GetFreeEntityCount_4A810() >= 26)
	{
		entity = NewEvent_4A050();
		if (entity)
		{
			entity->actionIndex_0x45_69 = 83;
			entity->class_0x3F_63 = 10;
			entity->model_0x40_64 = 76;
			entity->maxLife_0x4 = 80;
			entity->subSpellIndex_0x2A_42 = 70;
			entity->actSpeed_0x82_130 = 40;
			entity->maxSpeed_0x86_134 = 192;
			entity->minSpeed_0x84_132 = 480;
			entity->actSpeed_0x82_130 = 40;
			entity->mailMask_0x38_56 = 1;
			entity->levelPar1_0x43_67 = 0;
			entity->levelPar2_0x44_68 = 0;
			entity->param2_0x2C_44 = 0;
			entity->fontTypeIndex_0x3D_61 = 0;
			type_entity_0x6E8E* entity3 = entity;
			entity->flags_0xc_12_15.byte[0] &= 0xf6;
			entity->flags_0xc_12_15.byte[0] |= 1;
			CopyMaxLifeToLife_49A20(entity);
			for (int i = 0; i < 25; i++)
			{
				type_entity_0x6E8E* entity2 = NewEvent_4A050();
				if (entity2)
				{
					qmemcpy(entity2, entity, sizeof(type_entity_0x6E8E));
					entity2->model_0x40_64 = 77;
					entity2->actionIndex_0x45_69 = 84;
					entity2->leaderId_0x32_50 = entity3 - world_D41A0.entities_0x6E8E;
					entity3->nextLinkedId_0x34_52 = entity2 - world_D41A0.entities_0x6E8E;
					entity2->instanceIndex_0x3E_62 = i;
					entity2->levelPar1_0x43_67 = i / 5;
					entity2->nextLinkedId_0x34_52 = 0;
					entity2->levelPar2_0x44_68 = i % 5;
					AddEventToMap_57D70(entity2, position);
				}
				entity3 = entity2;
			}
			AddEventToMap_57D70(entity, position);
			SetEntityShiftRot_49EA0(entity, 640, 640);
			InitWindEntity_4F440(entity);
		}
	}
	return entity;
}

//----- (0004F440) --------------------------------------------------------
__int16 InitWindEntity_4F440(type_entity_0x6E8E* a1x)//230440 //tex 463
{
	unsigned __int16 v1; // ax
	int v2; // eax
	//char v3; // ah
	char v4; // dl
	//char v5; // cl
	__int16 v6; // ax
	int v7; // edx
	int v8; // edx
	__int16 v9; // dx
	signed __int16 v10; // cx
	__int16 v11; // dx
	__int16 v12; // dx
	type_entity_0x6E8E* v13x; // ebx

	v1 = a1x->maxSpeed_0x86_134;
	a1x->fontTypeIndex_0x3D_61 = 18;
	a1x->param2_0x2C_44 = v1;
	a1x->yaw_0x1C_28 = 0;
	v2 = v1;
	LOWORD(v2) = a1x->nextLinkedId_0x34_52;
	a1x->pitch_0x1E_30 = 0;
	while (1)
	{
		v13x = Entities_EA3E4[v2];
		if (v13x <= Entities_EA3E4[0])
			return v2;
		//v3 = v13x->flags_0xc_12_15.byte[0] & 0xFE;
		v4 = v13x->levelPar2_0x44_68;
		v13x->flags_0xc_12_15.byte[0] &= 0xFE;
		if (v4)
		{
			//v5 = v13x->flags_0xc_12_15.dbyte1_2.byte1 & 0xF7;
			v13x->flags_0xc_12_15.byte[2] |= 0x80u;
			v13x->flags_0xc_12_15.byte[0] &= 0xF7;
		}
		else
		{
			v13x->flags_0xc_12_15.byte[0] |= 8;
		}
		v13x->rand_0x14_20 = 9377 * v13x->rand_0x14_20 + 9439;
		v6 = (v13x->rand_0x14_20 & 0x3F) + 84;
		v7 = v13x->levelPar1_0x43_67;
		switch ((x_BYTE)v7)
		{
		case 0:
			LOWORD(v7) = v13x->levelPar2_0x44_68;
			v13x->pitch_0x1E_30 = 0;
			v13x->fov_0x22_34 = 0;
			v8 = 512 - 96 * v7;
			v13x->roll_0x20_32 = v6;
			BYTE1(v8) &= 7u;
			v13x->yaw_0x1C_28 = v8;
			break;
		case 1:
			v9 = 96 * v13x->levelPar2_0x44_68;
			v10 = 512;
			v13x->yaw_0x1C_28 = 512;
			goto LABEL_11;
		case 2:
			v11 = v13x->levelPar2_0x44_68;
			v13x->yaw_0x1C_28 = 0;
			v13x->roll_0x20_32 = 0;
			v12 = -96 * v11;
			goto LABEL_12;
		case 3:
			v9 = 96 * v13x->levelPar2_0x44_68;
			v10 = 256;
			v13x->yaw_0x1C_28 = 256;
			goto LABEL_11;
		case 4:
			v9 = 96 * v13x->levelPar2_0x44_68;
			v10 = 768;
			v13x->yaw_0x1C_28 = 768;
		LABEL_11:
			v13x->roll_0x20_32 = 0;
			v12 = v10 - v9;
		LABEL_12:
			v13x->fov_0x22_34 = v6;
			HIBYTE(v12) &= 7u;
			v13x->pitch_0x1E_30 = v12;
			break;
		default:
			break;
		}
		predictedAxis_EB398ar = a1x->position_0x4C_76;
		MoveEntity_57FA0(&predictedAxis_EB398ar, v13x->yaw_0x1C_28, v13x->pitch_0x1E_30, a1x->param2_0x2C_44);
		CopyEntityPosition_57CF0(v13x, &predictedAxis_EB398ar);
		SetEntityIndexAndRot_49CD0(v13x, 340);
		AddDynamicLight_847D0(v13x, 128, 1, 0);
		v2 = v13x->nextLinkedId_0x34_52;
	}
}
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;

//----- (0004F5F0) --------------------------------------------------------
type_entity_0x6E8E* AddSmallExplosion_4F5F0(axis_3d* position)//2305f0 //tex 38
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->maxLife_0x4 = 8;
		event->actionIndex_0x45_69 = 0x17;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x17;
		event->subSpellIndex_0x2A_42 = 25;
		event->flags_0xc_12_15.dword &= 0xFFFDFFF7;
		event->flags_0xc_12_15.byte[2] |= 2u;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 7);
		SetEntityShiftRot_49EA0(event, 200, 200);
		event->flags_0xc_12_15.byte[0] |= 1u;
		AddDynamicLight_847D0(event, 128, 9, 0);
	}
	return event;
}

//----- (0004F6A0) --------------------------------------------------------
type_entity_0x6E8E* AddEffect0A19_4F6A0(axis_3d* position)//2306a0
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 0x19;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x19;
		event->maxLife_0x4 = 8;
		event->position_0x4C_76 = *position;
		event->subSpellIndex_0x2A_42 = 2000;
		event->flags_0xc_12_15.byte[0] = event->flags_0xc_12_15.byte[0] & 0xF6 | 1;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityShiftRot_49EA0(event, 512, 512);
	}
	return event;
}

//----- (0004F720) --------------------------------------------------------
type_entity_0x6E8E* AddDuelHand_4F720(axis_3d* position)//230720 //tex 148
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 0x1A;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x1A;
		event->maxLife_0x4 = 8;
		event->position_0x4C_76 = *position;
		event->subSpellIndex_0x2A_42 = 200;
		event->flags_0xc_12_15.byte[0] &= 0xF7u;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 213);
		SetEntityShiftRot_49EA0(event, 512, 512);
	}
	return event;
}

//----- (0004F7A0) --------------------------------------------------------
type_entity_0x6E8E* AddTerrainPaint_4F7A0(axis_3d* position)//2307a0
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->maxLife_0x4 = 2;
		event->actionIndex_0x45_69 = 0x1B;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x1B;
		event->subSpellIndex_0x2A_42 = (position->z >> 5) + 48;
		event->param_0x10_16 = 10;
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
	}
	return event;
}

//----- (0004F800) --------------------------------------------------------
type_entity_0x6E8E* AddEffect0A1C_4F800(axis_3d* position)//230800
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->maxLife_0x4 = 0;
		event->actionIndex_0x45_69 = 0x1E;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x1C;
		event->position_0x4C_76 = *position;
		event->flags_0xc_12_15.byte[0] &= 0xF7u;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
	}
	return event;
}

//----- (0004F860) --------------------------------------------------------
type_entity_0x6E8E* AddEffectAction66_4F860(axis_3d* position)//230860
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->maxLife_0x4 = 52;
		event->actionIndex_0x45_69 = 0x42;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x3D;
		event->flags_0xc_12_15.byte[0] = event->flags_0xc_12_15.byte[0] & 0xF6 | 1;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
	}
	return event;
}

//----- (0004F8B0) --------------------------------------------------------
type_entity_0x6E8E* AddEffectAction67_4F8B0(axis_3d* position)//2308b0
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->maxLife_0x4 = 52;
		event->actionIndex_0x45_69 = 0x43;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x3E;
		event->flags_0xc_12_15.byte[0] = event->flags_0xc_12_15.byte[0] & 0xF6 | 1;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
	}
	return event;
}

//----- (0004F900) --------------------------------------------------------
type_entity_0x6E8E* AddEffectAction68_4F900(axis_3d* position)//230900
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->maxLife_0x4 = 1;
		event->actionIndex_0x45_69 = 0x44;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x3F;
		event->flags_0xc_12_15.byte[0] = event->flags_0xc_12_15.byte[0] & 0xF6 | 1;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
	}
	return event;
}

//----- (0004F950) --------------------------------------------------------
type_entity_0x6E8E* AddEffectAction69_4F950(axis_3d* position)//230950
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->maxLife_0x4 = 1;
		event->actionIndex_0x45_69 = 0x45;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x40;
		event->flags_0xc_12_15.byte[0] = event->flags_0xc_12_15.byte[0] & 0xF6 | 1;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
	}
	return event;
}

//----- (0004F9A0) --------------------------------------------------------
type_entity_0x6E8E* AddPointToPath_4F9A0(axis_3d* position)//2309a0 - summon effect black smoke
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->maxLife_0x4 = 0;
		event->actionIndex_0x45_69 = 0x20;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x1E;
		event->position_0x4C_76 = *position;
		event->flags_0xc_12_15.byte[0] &= 0xF7u;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
	}
	return event;
}

//----- (0004FA00) --------------------------------------------------------
type_entity_0x6E8E* AddEffect0A1D_4FA00(axis_3d* position)//230a00
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->maxLife_0x4 = 0;
		event->actionIndex_0x45_69 = 0x1F;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x1D;
		event->position_0x4C_76 = *position;
		event->flags_0xc_12_15.byte[0] &= 0xF7u;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
	}
	return event;
}

//----- (0004FA60) --------------------------------------------------------
type_entity_0x6E8E* AddEffect0A20_4FA60(axis_3d* position)//230a60
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->maxLife_0x4 = 0;
		event->actionIndex_0x45_69 = 0x22;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x20;
		event->position_0x4C_76 = *position;
		event->actSpeed_0x82_130 = 256;
		event->levelPar3_0x46_70 = 2;
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		CopyMaxLifeToLife_49A20(event);
	}
	return event;
}

//----- (0004FAC0) --------------------------------------------------------
type_entity_0x6E8E* AddEffect0A1F_4FAC0(axis_3d* position)//230ac0
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->maxLife_0x4 = 0;
		event->actionIndex_0x45_69 = 0x21;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x1F;
		event->position_0x4C_76 = *position;
		event->flags_0xc_12_15.byte[0] &= 0xF7u;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
	}
	return event;
}

//----- (0004FB80) --------------------------------------------------------
type_entity_0x6E8E* AddCaveEffect2_4FB80(axis_3d* position)//230b80
{
	type_entity_0x6E8E* event; // eax
	if (!isCaveLevel_D41B6)
		return 0;
	event = NewEvent_4A050();
	if (event)
	{
		event->maxLife_0x4 = 0;
		event->actionIndex_0x45_69 = 0x57;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x50;
		event->position_0x4C_76 = *position;
		event->flags_0xc_12_15.byte[0] &= 0xF7u;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		return event;
	}
	return 0;
}

//----- (0004FBE0) --------------------------------------------------------
type_entity_0x6E8E* AddTerrainDeform_4FBE0(axis_3d* position)//230be0
{
	type_entity_0x6E8E* event; // eax

	if (!isCaveLevel_D41B6)
		return 0;
	event = NewEvent_4A050();
	if (event)
	{
		event->maxLife_0x4 = 0;
		event->actionIndex_0x45_69 = 0x59;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x52;
		event->position_0x4C_76 = *position;
		event->levelPar3_0x46_70 = 2;
		event->levelPar1_0x43_67 = 3;
		event->levelPar2_0x44_68 = 3;
		return event;
	}
	return 0;
}

//----- (0004FC30) --------------------------------------------------------
type_entity_0x6E8E* AddCraterEffect_4FC30(axis_3d* position)//230c30
{
	if (!isCaveLevel_D41B6)
		return 0;
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->life_0x8 = 16;
		event->actionIndex_0x45_69 = 90;
		event->class_0x3F_63 = 10;
		event->model_0x40_64 = 83;
		event->position_0x4C_76 = *position;
		event->levelPar3_0x46_70 = 0;
		event->axis_0x9A_154x.x = 2;
		event->axis_0x9A_154x.z = 0;
		event->flags_0xc_12_15.byte[0] |= 1u;
		event->position_0x4C_76.z = 0;
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		return event;
	}
	return 0;
}

//----- (0004FCA0) --------------------------------------------------------
type_entity_0x6E8E* AddVolcanoEffect91_4FCA0(axis_3d* position)//230ca0
{
	type_entity_0x6E8E* event = AddCaveEffect_4FD00(position);
	if (event)
	{
		event->actionIndex_0x45_69 = 0x5B;
		event->model_0x40_64 = 0x54;
	}
	return event;
}

//----- (0004FCD0) --------------------------------------------------------
type_entity_0x6E8E* AddVolcanoEffect92_4FCD0(axis_3d* position)//230cd0
{
	type_entity_0x6E8E* event = AddCaveEffect_4FD00(position);
	if (event)
	{
		event->actionIndex_0x45_69 = 0x5C;
		event->model_0x40_64 = 0x55;
	}
	return event;
}

//----- (0004FD00) --------------------------------------------------------
type_entity_0x6E8E* AddCaveEffect_4FD00(axis_3d* position)//230d00
{
	type_entity_0x6E8E* event = NULL;
	if (isCaveLevel_D41B6)
	{
		event = NewEvent_4A050();
		if (event)
		{
			event->life_0x8 = 16;
			event->class_0x3F_63 = 0xA;
			event->position_0x4C_76 = *position;
			event->levelPar3_0x46_70 = 0;
			event->axis_0x9A_154x.x = 2;
			event->axis_0x9A_154x.z = 0;
			event->flags_0xc_12_15.byte[0] |= 1u;
			event->position_0x4C_76.z = 0;
			event->flags_0xc_12_15.byte[0] &= 0xF7;
		}
	}
	return event;
}

//----- (0004FD70) --------------------------------------------------------
type_entity_0x6E8E* AddEffectUntilWater_4FD70(axis_3d* position)//230d70
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->maxLife_0x4 = 0;
		event->actionIndex_0x45_69 = 0x37;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x33;
		event->position_0x4C_76 = *position;
		event->param_0x10_16 = 256;
		event->actSpeed_0x82_130 = 1024;
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		SetEntityShiftRot_49EA0(event, 768, 768);
		CopyMaxLifeToLife_49A20(event);
	}
	return event;
}

//----- (0004FDE0) --------------------------------------------------------
type_entity_0x6E8E* AddEffectAction54_4FDE0(axis_3d* position)//230de0
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->maxLife_0x4 = 0;
		event->actionIndex_0x45_69 = 0x36;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x32;
		event->position_0x4C_76 = *position;
		event->flags_0xc_12_15.byte[0] &= 0xF7u;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
	}
	return event;
}

//----- (0004FE40) --------------------------------------------------------
type_entity_0x6E8E* AddPortal_4FE40(axis_3d* position)//230e40 //tex 168
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 0x24;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x22;
		event->maxLife_0x4 = 0;
		event->xtype_0x41_65 = 3;
		event->xsubtype_0x42_66 = -1;
		event->flags_0xc_12_15.byte[0] &= 0xF7u;
		event->position_0x4C_76 = *position;
		SetEntityIndexAndRot_49CD0(event, 223);
		SetEntityShiftRot_49EA0(event, 256, 256);
		CopyMaxLifeToLife_49A20(event);
		AddEventToMap_57D70(event, position);
		event->position_0x4C_76.z = GetTerrainAlt_10C40(&event->position_0x4C_76) + 640;
		event->axis_0x9A_154x = event->position_0x4C_76;
		event->rand_0x14_20 = 9377 * event->rand_0x14_20 + 9439;
		MoveEntity_57FA0(&event->axis_0x9A_154x, event->rand_0x14_20 & 0x7FF, 0, -32768);
	}
	return event;
}

//----- (0004FF20) --------------------------------------------------------
type_entity_0x6E8E* AddEffect0A23_4FF20()//230f20
{
	return NewEvent_4A050();
}

//----- (0004FF30) --------------------------------------------------------
type_entity_0x6E8E* AddSkeletonSpawn_4FF30(axis_3d* position)//230f30 //tex 54
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 0x26;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x24;
		event->maxLife_0x4 = 8;
		event->position_0x4C_76 = *position;
		event->subSpellIndex_0x2A_42 = 64000;
		event->flags_0xc_12_15.byte[0] &= 0xF7u;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 41);
		SetEntityShiftRot_49EA0(event, 512, 512);
	}
	return event;
}

//----- (0004FFB0) --------------------------------------------------------
type_entity_0x6E8E* AddThunderball_4FFB0(axis_3d* position)//230fb0 //tex 202
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 0x28;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x26;
		event->maxLife_0x4 = 32;
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 272);
		SetEntityShiftRot_49EA0(event, 512, 512);
	}
	return event;
}

//----- (00050020) --------------------------------------------------------
type_entity_0x6E8E* AddEffect0A21_50020(axis_3d* position)//231020
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 0x23;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x21;
		event->flags_0xc_12_15.byte[0] &= 0xF7u;
		event->position_0x4C_76 = *position;
		CopyMaxLifeToLife_49A20(event);
		SetEntityShiftRot_49EA0(event, 512, 512);
	}
	return event;
}

//----- (00050080) --------------------------------------------------------
type_entity_0x6E8E* CreateManaSphere512_50080(axis_3d* position)//231080
{
	return CreateManaSphere_500C0(position, 512);
}

//----- (000500A0) --------------------------------------------------------
type_entity_0x6E8E* CreateManaSphere2560_500A0(axis_3d* position)//2310a0
{
	return CreateManaSphere_500C0(position, 2560);
}

//----- (000500C0) --------------------------------------------------------
type_entity_0x6E8E* CreateManaSphere_500C0(axis_3d* position, __int16 mana)//2310c0
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 0x29;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x27;
		event->xtype_0x41_65 = 10;
		event->xsubtype_0x42_66 = 39;
		event->param2_0x2C_44 = 128;
		event->actSpeed_0x82_130 = 32;
		event->mailMask_0x38_56 = 3;
		event->timer_0x39_57 = 128;
		event->chainCount_0x3A_58 = 0;
		event->mana_0x90_144 = mana;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetManaSphereColorAndRot_36920(event);
	}
	return event;
}

//----- (00050130) --------------------------------------------------------
type_entity_0x6E8E* AddFoolsManaSphere_50130(axis_3d* position)//231130
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 0x3E;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x39;
		event->xtype_0x41_65 = 10;
		event->xsubtype_0x42_66 = 57;
		event->param2_0x2C_44 = 128;
		event->rand_0x14_20 = 9377 * event->rand_0x14_20 + 9439;
		event->actSpeed_0x82_130 = 0;
		event->mailMask_0x38_56 = 3;
		event->timer_0x39_57 = -128;
		event->chainCount_0x3A_58 = 0;
		event->levelPar1_0x43_67 = 10;
		event->mana_0x90_144 = event->rand_0x14_20 % 0x7D0;
		event->levelPar2_0x44_68 = 1;
		event->flags_0xc_12_15.byte[2] |= 2;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetManaSphereColorAndRot_36920(event);
	}
	return event;
}

//----- (000501D0) --------------------------------------------------------
type_entity_0x6E8E* AddPlayerDeathEffect_501D0(axis_3d* position)//2311d0
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 0x2A;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x28;
		event->param_0x10_16 = (event - world_D41A0.entities_0x6E8E) % 11;
		event->mailMask_0x38_56 = 2;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetHalfSpeedEntity_49DA0(event, 65);
	}
	return event;
}

//----- (00050250) --------------------------------------------------------
type_entity_0x6E8E* AddTerrainModification_50250(axis_3d* position)//231250 - add effect ?? no in INF //tex 96
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->maxLife_0x4 = 30;
		event->actionIndex_0x45_69 = 0x33;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x2D;
		event->subSpellIndex_0x2A_42 = 100;
		event->param_0x10_16 = 4;
		event->flags_0xc_12_15.byte[0] = 9;
		event->mailMask_0x38_56 = 33;
		AddEventToMap_57D70(event, position);
		SetEntityIndexAndRot_49CD0(event, 177);
	}
	return event;
}

//----- (000502B0) --------------------------------------------------------
type_entity_0x6E8E* AddProjectileLife_502B0(axis_3d* position)//2312b0 //tex 54
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 0x2D;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x2B;
		event->maxLife_0x4 = 8;
		event->subSpellIndex_0x2A_42 = 64000;
		event->flags_0xc_12_15.byte[0] &= 0xF7u;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 41);
		SetEntityShiftRot_49EA0(event, 512, 512);
	}
	return event;
}

//----- (00050320) --------------------------------------------------------
type_entity_0x6E8E* AddCastleEffect41_50320(axis_3d* position)//231320
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 0x2B;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x29;
		event->maxLife_0x4 = 0;
		event->flags_0xc_12_15.byte[0] &= 0xF7u;
		event->position_0x4C_76 = *position;
		CopyMaxLifeToLife_49A20(event);
	}
	return event;
}

//----- (00050370) --------------------------------------------------------
type_entity_0x6E8E* AddCastleEffect42_50370(axis_3d* position)//231370
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 0x2C;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x2A;
		event->maxLife_0x4 = 0;
		event->flags_0xc_12_15.byte[0] &= 0xF6;
		event->flags_0xc_12_15.byte[0] |= 1;
		event->position_0x4C_76 = *position;
		event->poseIndex_0x3B_59 = 1;
		CopyMaxLifeToLife_49A20(event);
	}
	return event;
}

//----- (000503D0) --------------------------------------------------------
type_entity_0x6E8E* AddEffectAction46_503D0(axis_3d* position)//2313d0 //tex 60
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 0x2E;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x2C;
		event->maxLife_0x4 = 500;
		event->subSpellIndex_0x2A_42 = 500;
		event->flags_0xc_12_15.byte[0] &= 0xF7u;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 8);
	}
	return event;
}

//----- (00050430) --------------------------------------------------------
type_entity_0x6E8E* AddEffectAction56_50430(axis_3d* position)//231430 //tex 0
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 0x38;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x34;
		event->maxLife_0x4 = 100000;
		event->subSpellIndex_0x2A_42 = 500;
		event->param_0x10_16 = 600;
		event->mana_0x90_144 = 500;
		event->maxMana_0x8C_140 = 2000;
		event->flags_0xc_12_15.byte[0] &= 0xF7u;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 205);
	}
	return event;
}

//----- (000504B0) --------------------------------------------------------
type_entity_0x6E8E* AddProjectileTrail_504B0(axis_3d* position)//2314b0
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 0x3A;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x35;
		event->position_0x4C_76 = *position;
		event->maxLife_0x4 = 6;
		event->subSpellIndex_0x2A_42 = 3000;
		event->flags_0xc_12_15.byte[0] &= 0xF7u;
		CopyMaxLifeToLife_49A20(event);
	}
	return event;
}

//----- (00050500) --------------------------------------------------------
type_entity_0x6E8E* AddAuxiliary_50500(axis_3d* position)//231500
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 0x3B;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x36;
		event->maxLife_0x4 = 128;
		event->actSpeed_0x82_130 = 256;
		event->flags_0xc_12_15.byte[0] &= 0xF7u;
		event->subSpellIndex_0x2A_42 = 100;
		event->rand_0x14_20 = 9377 * event->rand_0x14_20 + 9439;
		event->param_0x10_16 = 12845056;
		event->yaw_0x1C_28 = event->rand_0x14_20 & 0x7FF;
		event->position_0x4C_76 = *position;
		event->flags_0xc_12_15.byte[0] |= 1u;
		CopyMaxLifeToLife_49A20(event);
		SetEntityShiftRot_49EA0(event, 1024, 0x4000);
	}
	return event;
}

//----- (000505A0) --------------------------------------------------------
type_entity_0x6E8E* AddEffectAction75_505A0(axis_3d* position)//2315a0
{
	type_entity_0x6E8E* event = AddMovingEffect_505E0(position);
	if (event)
	{
		event->model_0x40_64 = 0x44;
		event->actionIndex_0x45_69 = 0x4B;
	}
	return event;
}

//----- (000505E0) --------------------------------------------------------
type_entity_0x6E8E* AddMovingEffect_505E0(axis_3d* position)//2315e0
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->class_0x3F_63 = 0xA;
		event->actSpeed_0x82_130 = 0x40;
		event->param_0x10_16 = 12845056;
		event->position_0x4C_76 = *position;
		event->flags_0xc_12_15.byte[0] = event->flags_0xc_12_15.byte[0] & 0xF6 | 1;
		SetEntityShiftRot_49EA0(event, 256, 256);
	}
	return event;
}

//----- (00050640) --------------------------------------------------------
type_entity_0x6E8E* AddEffectAction60_50640(axis_3d* position)//231640
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 0x3C;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x37;
		event->maxLife_0x4 = 19;
		event->actSpeed_0x82_130 = 256;
		event->flags_0xc_12_15.byte[0] &= 0xF7u;
		event->subSpellIndex_0x2A_42 = 100;
		event->rand_0x14_20 = 9377 * event->rand_0x14_20 + 9439;
		event->param_0x10_16 = 32;
		event->yaw_0x1C_28 = event->rand_0x14_20 & 0x7FF;
		event->position_0x4C_76 = *position;
		event->flags_0xc_12_15.byte[0] |= 1u;
		CopyMaxLifeToLife_49A20(event);
		SetEntityShiftRot_49EA0(event, 1024, 0x4000);
	}
	return event;
}

//----- (000506E0) --------------------------------------------------------
type_entity_0x6E8E* AddEffectAction61_506E0(axis_3d* position)//2316e0
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 0x3D;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x38;
		event->maxLife_0x4 = 128;
		event->actSpeed_0x82_130 = 256;
		event->flags_0xc_12_15.byte[0] &= 0xF7u;
		event->subSpellIndex_0x2A_42 = 100;
		event->rand_0x14_20 = 9377 * event->rand_0x14_20 + 9439;
		event->param_0x10_16 = 0;
		event->yaw_0x1C_28 = event->rand_0x14_20 & 0x7FF;
		event->position_0x4C_76 = *position;
		event->flags_0xc_12_15.byte[0] |= 1u;
		CopyMaxLifeToLife_49A20(event);
		SetEntityShiftRot_49EA0(event, 1024, 0x4000);
	}
	return event;
}

//----- (00050780) --------------------------------------------------------
type_entity_0x6E8E* AddSlowDownWizard_50780(axis_3d* position)//231780
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 0x46;
		event->class_0x3F_63 = 0xA;
		event->flags_0xc_12_15.byte[0] &= 0xF6u;
		event->model_0x40_64 = 0x41;
		event->flags_0xc_12_15.byte[0] |= 1;
		event->position_0x4C_76 = *position;
	}
	return event;
}

//----- (000507C0) --------------------------------------------------------
type_entity_0x6E8E* AddMobilizeWizard_507C0(axis_3d* position)//2317c0
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 0x47;
		event->class_0x3F_63 = 0xA;
		event->flags_0xc_12_15.byte[0] &= 0xF6u;
		event->model_0x40_64 = 0x42;
		event->flags_0xc_12_15.byte[0] |= 1;
		event->position_0x4C_76 = *position;
		event->subSpellIndex_0x2A_42 = 200;
	}
	return event;
}

//----- (00050800) --------------------------------------------------------
type_entity_0x6E8E* AddFollowTarget_50800()//231800
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 0x51;
		event->class_0x3F_63 = 0xA;
		event->flags_0xc_12_15.byte[0] &= 0xF6u;
		event->model_0x40_64 = 0x4A;
		event->flags_0xc_12_15.byte[0] |= 1;
	}
	return event;
}

//----- (00050840) --------------------------------------------------------
type_entity_0x6E8E* AddPossessedCreatureMarker_50840(axis_3d* position)//231840 //tex 80
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->maxLife_0x4 = 1000;
		event->actionIndex_0x45_69 = 0x55;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x4E;
		event->position_0x4C_76 = *position;
		event->flags_0xc_12_15.byte[0] |= 8u;
		event->subSpellIndex_0x2A_42 = 0;
		event->levelPar3_0x46_70 = 0;
		event->param3_0x36_54 = -1;
		event->fontTypeIndex_0x3D_61 = 1;
		event->levelPar1_0x43_67 = 1;
		event->levelPar2_0x44_68 = 0;
		event->leaderId_0x32_50 = event - world_D41A0.entities_0x6E8E;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 66);
	}
	return event;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (00050960) --------------------------------------------------------
type_entity_0x6E8E* AddSpeedTrail_50960(axis_3d* position)//231960
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->maxLife_0x4 = 9;
		event->actionIndex_0x45_69 = 0x5D;
		event->class_0x3F_63 = 0xA;
		event->life_0x8 = event->maxLife_0x4;
		event->flags_0xc_12_15.dword &= 0xFFFDFFF7;
		event->model_0x40_64 = 0x56;
		event->flags_0xc_12_15.byte[2] |= 2;
		AddEventToMap_57D70(event, position);
		event->position_0x4C_76.z = GetTerrainAlt_10C40(&event->position_0x4C_76);
		event->rand_0x14_20 = 9377 * event->rand_0x14_20 + 9439;
		SetEntityIndexAndRot_49CD0(event, event->rand_0x14_20 % 3u + 332);
		if (!(GetTileAngleBit_104A0(&event->position_0x4C_76) & 1))
		{
			ReleaseEntity_57F20(event);
			event = 0;
		}
	}
	return event;
}

//----- (00050A20) --------------------------------------------------------
type_entity_0x6E8E* AddSpeedSpell_50A20(axis_3d* position)//231a20
{
	type_entity_0x6E8E* event; // eax
	if (world_D41A0.terrain_2FECE.MapType != MapType_t::Cave)
		return 0;
	event = NewEvent_4A050();
	if (event)
	{
		event->flags_0xc_12_15.byte[0] &= 0xF6;
		event->actionIndex_0x45_69 = 0x60;
		event->flags_0xc_12_15.byte[0] |= 1;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x59;
		event->life_0x8 = 40;
		AddEventToMap_57D70(event, position);
		return event;
	}
	return 0;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (000514E0) --------------------------------------------------------
type_entity_0x6E8E* AddClass0EEntity_514E0(axis_3d* position, char a2, char a3, __int16 a4)//2324e0
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->class_0x3F_63 = 0xE;
		event->levelPar3_0x46_70 = 0;
		event->actionIndex_0x45_69 = a3;
		event->model_0x40_64 = a2;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetHalfSpeedEntity_49DA0(event, a4);
	}
	return event;
}

//----- (00051530) --------------------------------------------------------
type_entity_0x6E8E* AddClass0E00_51530(axis_3d* position)//232530
{
	type_entity_0x6E8E* event = AddClass0EEntity_514E0(position, 0, 0, 77);
	if (event)
		SetEntityShiftRot_49EA0(event, 384, 384);
	return event;
}

//----- (00051570) --------------------------------------------------------
type_entity_0x6E8E* AddClass0E03_51570(axis_3d* position)//232570
{
	type_entity_0x6E8E* event = AddClass0EEntity_514E0(position, 3, 8, 338);
	if (event)
	{
		world_D41A0.switchChainEntity_0x36DFE = event - world_D41A0.entities_0x6E8E;
		event->flags_0xc_12_15.byte[0] |= 1u;
	}
	return event;
}

//----- (000515C0) --------------------------------------------------------
type_entity_0x6E8E* AddClass0E04_515C0(axis_3d* position)//2325c0
{
	type_entity_0x6E8E* event = AddClass0EEntity_514E0(position, 4, 9, 339);
	if (event)
	{
		world_D41A0.switchOnceEntity_0x36DFC = event - world_D41A0.entities_0x6E8E;
		event->flags_0xc_12_15.byte[0] |= 1u;
	}
	return event;
}

//----- (00051610) --------------------------------------------------------
type_entity_0x6E8E* AddClass0E05_51610(axis_3d* position)//232610
{
	type_entity_0x6E8E* event = AddClass0EEntity_514E0(position, 5, 10, 280);
	if (event)
	{
		SetEntityShiftRot_49EA0(event, 768, 1280);
		if (gameState_D41A4.setting_38545 & 4)
			event->flags_0xc_12_15.byte[0] |= 1u;
	}
	return event;
}

//----- (00051660) --------------------------------------------------------
type_entity_0x6E8E* AddClass0E01_51660(axis_3d* position)//232660
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 6;
		event->class_0x3F_63 = 0xE;
		event->flags_0xc_12_15.byte[0] &= 0xF6u;
		event->model_0x40_64 = 1;
		event->flags_0xc_12_15.byte[0] |= 1;
		event->maxLife_0x4 = 0;
		event->life_0x8 = 0;
		event->subSpellIndex_0x2A_42 = 0;
		AddEventToMap_57D70(event, position);
	}
	return event;
}

//----- (000516C0) --------------------------------------------------------
type_entity_0x6E8E* AddClass0E02_516C0(axis_3d* position)//2326c0
{
	type_entity_0x6E8E* event = 0;
	if (isCaveLevel_D41B6)
	{
		event = NewEvent_4A050();
		if (event)
		{
			event->actionIndex_0x45_69 = 7;
			event->class_0x3F_63 = 0xE;
			event->model_0x40_64 = 2;
			event->flags_0xc_12_15.byte[0] &= 0xF6u;
			event->life_0x8 = 0;
			event->flags_0xc_12_15.byte[0] |= 1;
			event->subSpellIndex_0x2A_42 = 0;
			event->param2_0x2C_44 = 0;
			event->targetId_0x96_150 = 0;
			AddEventToMap_57D70(event, position);
		}
	}
	return event;
}

//----- (00051730) --------------------------------------------------------
type_entity_0x6E8E* AddLightningEffect_51730(axis_3d* position)//232730
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 0x48;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x43;
		event->flags_0xc_12_15.byte[0] = event->flags_0xc_12_15.byte[0] & 0xF6 | 1;
		event->life_0x8 = 120;
		event->subSpellIndex_0x2A_42 = 20000;
		AddEventToMap_57D70(event, position);
		SetEntityShiftRot_49EA0(event, 4352, 4352);
	}
	return event;
}

//----- (00051790) --------------------------------------------------------
type_entity_0x6E8E* AddShield_51790(axis_3d* position)//232790
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 0x4E;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x47;
		event->life_0x8 = 120;
		event->subSpellIndex_0x2A_42 = 20000;
		event->flags_0xc_12_15.byte[0] = event->flags_0xc_12_15.byte[0] & 0xF6 | 1;
		event->levelPar3_0x46_70 = 0;
		event->maxLife_0x4 = event->life_0x8;
		AddEventToMap_57D70(event, position);
		SetEntityShiftRot_49EA0(event, 1280, 2048);
	}
	return event;
}

//----- (00051800) --------------------------------------------------------
type_entity_0x6E8E* AddEffectIfEntitiesFree_51800(axis_3d* position)//232800 //tex 158
{
	type_entity_0x6E8E* event = nullptr;
	if (GetFreeEntityCount_4A810() >= gameState_D41A4.summonCount_224)
	{
		event = NewEvent_4A050();
		if (event)
		{
			event->actionIndex_0x45_69 = 0x4F;
			event->class_0x3F_63 = 0xA;
			event->model_0x40_64 = 0x48;
			event->maxLife_0x4 = 16;
			event->flags_0xc_12_15.byte[0] &= 0xF7;
			event->life_0x8 = event->maxLife_0x4;
			event->instanceIndex_0x3E_62 = 0;
			type_entity_0x6E8E* prevEvent = event;
			for (int index = 1; gameState_D41A4.summonCount_224 > index; index++)
			{
				type_entity_0x6E8E* newEvent = NewEvent_4A050();
				if (newEvent)
				{
					qmemcpy(newEvent, event, sizeof(type_entity_0x6E8E));
					newEvent->leaderId_0x32_50 = prevEvent - world_D41A0.entities_0x6E8E;
					prevEvent->nextLinkedId_0x34_52 = newEvent - world_D41A0.entities_0x6E8E;
					newEvent->nextLinkedId_0x34_52 = 0;
					newEvent->instanceIndex_0x3E_62 = index;
					predictedAxis_EB398ar = *position;
					MoveEntity_57FA0(&predictedAxis_EB398ar, index * (2048 / gameState_D41A4.summonCount_224), 0, 512);
					predictedAxis_EB398ar.z = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
					AddEventToMap_57D70(newEvent, &predictedAxis_EB398ar);
					SetEntityShiftRot_49EA0(newEvent, 128, 256);
					SetEntityIndexAndRot_49CD0(newEvent, 220);
				}
				prevEvent = newEvent;
			}
			predictedAxis_EB398ar = *position;
			MoveEntity_57FA0(&predictedAxis_EB398ar, 0, 0, 512);
			predictedAxis_EB398ar.z = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
			AddEventToMap_57D70(event, &predictedAxis_EB398ar);
			SetEntityShiftRot_49EA0(event, 128, 256);
			SetEntityIndexAndRot_49CD0(event, 220);
		}
	}
	return event;
}

//----- (00051A00) --------------------------------------------------------
type_entity_0x6E8E* AddEffectCountdown_51A00(axis_3d* position)//232a00 //tex 158
{
	type_entity_0x6E8E* entity = NewEvent_4A050();
	if (entity)
	{
		entity->actionIndex_0x45_69 = 0x50;
		entity->class_0x3F_63 = 0xA;
		entity->model_0x40_64 = 0x49;
		entity->life_0x8 = 16;
		entity->flags_0xc_12_15.byte[0] &= 0xF7;
		entity->maxLife_0x4 = entity->life_0x8;
		predictedAxis_EB398ar = *position;
		predictedAxis_EB398ar.z = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
		AddEventToMap_57D70(entity, &predictedAxis_EB398ar);
		SetEntityShiftRot_49EA0(entity, 128, 256);
		SetEntityIndexAndRot_49CD0(entity, 220);
	}
	return entity;
}

//----- (00064E20) --------------------------------------------------------
uint32_t UpdateTree_64E20(type_entity_0x6E8E* a1x)//245e20
{
	__int16 v1; // dx
	int v2; // edx
	type_entity_0x6E8E* v3x; // eax
	type_entity_0x6E8E* v4x; // ecx
	type_entity_0x6E8E* v5x; // esi
	__int64 v6; // rax
	__int16 v7; // di
	unsigned int v8; // edx
	//int v9; // esi
	uint32_t result; // eax
	type_entity_0x6E8E* v11x; // eax

	v1 = a1x->mail_0x5E_94.channel[0].source;
	a1x->flags_0xc_12_15.byte[2] |= 2u;
	if (v1)
	{
		v2 = a1x->life_0x8 - a1x->mail_0x5E_94.channel[0].amount.dword;
		a1x->life_0x8 = v2;
		if (v2 < 0)
		{
			v3x = CreateEntity_4A190(&a1x->position_0x4C_76, 10, 6);
			v4x = v3x;
			v5x = v3x;
			if (v3x)
			{
				v3x->id_0x1A_26 = Entities_EA3E4[a1x->mail_0x5E_94.channel[0].source]->id_0x1A_26;
				v6 = 3 * a1x->params_0x52_82.fov;
				v7 = v4x->position_0x4C_76.z;
				v4x->param2_0x2C_44 = (signed int)(v6 - (__CFSHL__(HIDWORD(v6), 2) + 4 * HIDWORD(v6))) >> 2;
				if (v7 <= 128)
					v4x->position_0x4C_76.z = 0;
				else
					v4x->position_0x4C_76.z = v7 - 128;
				a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
				v8 = a1x->rand_0x14_20 % 0x3Cu + 130;
				v5x->life_0x8 = v8;
				//v9 = *(x_DWORD *)(a1 + 12);
				a1x->life_0x8 = v8;
				a1x->flags_0xc_12_15.dword &= 0xFFFDFFF7;
				LOBYTE(v8) = a1x->flags_0xc_12_15.byte[2];
				a1x->actionIndex_0x45_69 = 1;
				a1x->flags_0xc_12_15.byte[2] = v8 | 2;
				MoveEntityOnMap_57D40(a1x, &a1x->position_0x4C_76);
			}
		}
		a1x->mail_0x5E_94.channel[0].source = 0;
	}
	a1x->position_0x4C_76.z = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
	result = TerrainTileIsWater_104D0(&a1x->position_0x4C_76);
	if (result == 1)
	{
		v11x = CreateEntity_4A190(&a1x->position_0x4C_76, 10, 5);
		if (v11x)
			v11x->id_0x1A_26 = a1x->id_0x1A_26;
		DisableEntityDrawing04_57F10(a1x);
	}
	return result;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00064F60) --------------------------------------------------------
uint32_t UpdateBurningTree_64F60(type_entity_0x6E8E* a1x)//245f60
{
	int v1; // edx
	unsigned __int16 v2; // si
	uint32_t result; // eax
	type_entity_0x6E8E* v4x; // eax

	v1 = a1x->life_0x8 - 1;
	a1x->life_0x8 = v1;
	if (v1 < 60)
	{
		v2 = a1x->spriteIndex_0x5A_90;
		a1x->actionIndex_0x45_69 = 2;
		if (v2 >= 0x53u)
		{
			if (v2 <= 0x53u)
			{
				SetHalfSpeedEntity_49DA0(a1x, 226);
			}
			else if (v2 == 84)
			{
				SetHalfSpeedEntity_49DA0(a1x, 227);
			}
		}
	}
	a1x->position_0x4C_76.z = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
	result = TerrainTileIsWater_104D0(&a1x->position_0x4C_76);
	if (result == 1)
	{
		v4x = CreateEntity_4A190(&a1x->position_0x4C_76, 10, 5);
		if (v4x)
			v4x->id_0x1A_26 = a1x->id_0x1A_26;
		DisableEntityDrawing04_57F10(a1x);
	}
	return result;
}

//----- (00064FF0) --------------------------------------------------------
uint32_t UpdateBurntTree_64FF0(type_entity_0x6E8E* a1x)//245ff0
{
	uint32_t result; // eax
	type_entity_0x6E8E* v2x; // eax

	a1x->position_0x4C_76.z = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
	result = TerrainTileIsWater_104D0(&a1x->position_0x4C_76);
	if (result == 1)
	{
		v2x = CreateEntity_4A190(&a1x->position_0x4C_76, 10, 5);
		if (v2x)
			v2x->id_0x1A_26 = a1x->id_0x1A_26;
		DisableEntityDrawing04_57F10(a1x);
	}
	return result;
}

//----- (00065040) --------------------------------------------------------
void UpdateStone_65040(type_entity_0x6E8E* event)//246040
{
	event->flags_0xc_12_15.byte[2] |= 2u;
	event->position_0x4C_76.z = GetTerrainAlt_10C40(&event->position_0x4C_76);
}

//----- (00065080) --------------------------------------------------------
void UpdateDolmen_65080(type_entity_0x6E8E* event)//246080
{
	for (int v2x = 0; v2x < world_D41A0.NumberOfPlayers_0xe; v2x++)
	{
		type_entity_0x6E8E* event2 = Entities_EA3E4[world_D41A0.players_0x2BDE[v2x].playerIndex_0x00a_2BE4_11240];
		if (event2->life_0x8 >= 0 && EntitiesOverlap_106C0(event2, event))
			event2->flags_0xc_12_15.byte[1] |= 0x10u;
	}
	event->position_0x4C_76.z = GetTerrainAlt_10C40(&event->position_0x4C_76);
}

//----- (00065110) --------------------------------------------------------
int UpdateSceneryType3_65110(type_entity_0x6E8E* a1x)//246110
{
	int result; // eax

	a1x->flags_0xc_12_15.byte[2] |= 2u;
	result = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
	a1x->position_0x4C_76.z = result;
	return result;
}

//----- (000651B0) --------------------------------------------------------
uint32_t UpdateSceneryType6_651B0(type_entity_0x6E8E* a1x)//2461b0
{
	__int16 v1; // dx
	int v2; // edx
	__int16 v3; // ax
	char v4; // dl
	uint32_t result; // eax

	v1 = a1x->mail_0x5E_94.channel[0].source;
	a1x->flags_0xc_12_15.byte[2] |= 2u;
	if (v1)
	{
		v2 = a1x->life_0x8 - a1x->mail_0x5E_94.channel[0].amount.dword;
		a1x->life_0x8 = v2;
		if (v2 < 0)
		{
			v3 = a1x->spriteIndex_0x5A_90;
			a1x->mail_0x5E_94.channel[0].source = 0;
			v4 = a1x->flags_0xc_12_15.byte[0];
			a1x->actionIndex_0x45_69 = 19;
			a1x->flags_0xc_12_15.byte[0] = v4 & 0xF7;
			SetHalfSpeedEntity_49DA0(a1x, v3 + 4);
			CreateEntity_4A190(&a1x->position_0x4C_76, 10, 13);
		}
		a1x->mail_0x5E_94.channel[0].source = 0;
	}
	a1x->position_0x4C_76.z = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
	result = TerrainTileIsWater_104D0(&a1x->position_0x4C_76);
	if (result == 1)
		DisableEntityDrawing04_57F10(a1x);
	return result;
}

//----- (00065240) --------------------------------------------------------
uint32_t UpdateSceneryType6Burnt_65240(type_entity_0x6E8E* a1x)//246240
{
	uint32_t result; // eax

	a1x->position_0x4C_76.z = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
	result = TerrainTileIsWater_104D0(&a1x->position_0x4C_76);
	if (result == 1)
		DisableEntityDrawing04_57F10(a1x);
	return result;
}

//----- (00065280) --------------------------------------------------------
void UpdateSceneryType6Falling2_65280(type_entity_0x6E8E* a1x)//246280
{
	MoveProjectile_652C0(a1x);
}

//----- (000652A0) --------------------------------------------------------
void UpdateSceneryType6Falling_652A0(type_entity_0x6E8E* a1x)//2462a0
{
	MoveProjectile_652C0(a1x);
}
