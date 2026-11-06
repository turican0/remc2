#include "Castle.h"
#include "EventsFunctions.h"
#include "Effects.h"
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

int speedIcrement_D4B84 = 16; // weak

int wizardSpeed1_D4B88 = 4294967216; // weak

int wizardSpeed2_D4B8C = 80; // weak

__int16 strafeAccel1_D4BA4 = 16; // weak

int wizardMinStrafe_D4BA8 = 4294967216; // weak

int wizardMaxStrafe_D4BAC = 80; // weak

__int16 strafeAccel2_D4BB0 = 65532; // weak

//0xc8+0x800+ x_DWORD_D9F50ar
//load *xadataspellsdat.colorPalette_var28 - lenght 2080

int8_t balloonChainTable_DB038[52] = {
0x00,0x00,0x01,0x00,0x04,0x01,0x04,0x01,0x04,0x05,0x04,0x05,0x08,0x09,0x08,0x09,0x00,0x00,0x04,0x04,0x03,0x03,
0x11,0x03,0x03,0x11,0x11,0x11,0x03,0x03,0x1F,0x03,0x03,0x1F,0x1F,0x1F,0x03,0x03,0x18,0x03,0x2D,0x03,0x03,0x18,
0x2D,0x18,0x03,0x2D,0x18,0x2D,0x2D,0x2D
};

//----- (00011A10) --------------------------------------------------------
char CanPlaceCastleArea_11A10(type_entity_0x6E8E* a1)//1f2a10
{
	type_entity_0x6E8E* ix; // ebx
	//unsigned __int16 v3; // bx
	uaxis_2d v3;
	__int16 v4; // di
	__int16 v5; // di
	uaxis_2d v6; // bx
	__int16 v7; // di
	uaxis_2d v8; // bx
	int v9; // eax
	__int16 v10; // di
	uaxis_2d v11; // bx
	__int16 v12; // [esp+0h] [ebp-24h]
	int v13; // [esp+4h] [ebp-20h]
	__int16 l; // [esp+8h] [ebp-1Ch]
	__int16 j; // [esp+Ch] [ebp-18h]
	__int16 k; // [esp+10h] [ebp-14h]
	int v17; // [esp+14h] [ebp-10h]
	__int16 v18; // [esp+14h] [ebp-10h]
	__int16 v19; // [esp+18h] [ebp-Ch]
	unsigned __int16 v20; // [esp+1Ch] [ebp-8h]
	__int16 v21; // [esp+20h] [ebp-4h]
	__int16 v22; // [esp+20h] [ebp-4h]

	v18 = a1->params_0x52_82.pitch >> 8;
	v21 = a1->params_0x52_82.roll >> 8;
	SetShiftByCastle_49EC0(a1, a1->param_0x10_16 + 1);
	for (ix = gameState_D41A4.class3List_38519; ix > Entities_EA3E4[0]; ix = ix->next_0)
	{
		if (ix->model_0x40_64 == 2 && ix != a1 && EntitiesOverlap_106C0(ix, a1))
		{
			SetShiftByCastle_49EC0(a1, a1->param_0x10_16);
			return 0;
		}
	}
	v19 = a1->params_0x52_82.pitch >> 8;
	v12 = a1->params_0x52_82.roll >> 8;
	LOBYTE(v20) = ((unsigned __int16)(a1->position_0x4C_76.x + 128) >> 8) - v19;
	HIBYTE(v20) = ((unsigned __int16)(a1->position_0x4C_76.y + 128) >> 8) - v12;
	LOWORD(v17) = v19 - v18;
	v3.word = v20;
	v22 = v12 - v21;
	v4 = v22;
	while (v4)
	{
		for (j = 2 * v19; j; j--)
		{
			if (!IsTilePassable_11C80(v3))
			{
				SetShiftByCastle_49EC0(a1, a1->param_0x10_16);
				return 0;
			}
			//LOBYTE(v3) = v3 + 1;
			v3._axis_2d.x++;
		}
		v4--;
		v3.word = __PAIR__(v3._axis_2d.y, (unsigned __int8)v20) + 256;
	}
	LOBYTE(v6) = v20;
	v5 = v22;
	HIBYTE(v6) = 2 * v12 + HIBYTE(v20) - v22;
	while (v5)
	{
		for (k = 2 * v19; k; k--)
		{
			if (!IsTilePassable_11C80(v6))
			{
				SetShiftByCastle_49EC0(a1, a1->param_0x10_16);
				return 0;
			}
			v6._axis_2d.x++;
		}
		v5--;
		v6.word = __PAIR__(v6._axis_2d.y, (unsigned __int8)v20) + 256;
	}
	LOBYTE(v8) = v20;
	v7 = v22;
	HIBYTE(v8) = v22 + HIBYTE(v20);
	while (v7)
	{
		v9 = v17;
		while (1)
		{
			v13 = v9;
			if (!(x_WORD)v9)
				break;
			if (!IsTilePassable_11C80(v8))
			{
				SetShiftByCastle_49EC0(a1, a1->param_0x10_16);
				return 0;
			}
			v9 = v13 - 1;
			v8._axis_2d.x++;
		}
		v7--;
		v8.word = __PAIR__(v8._axis_2d.y, (unsigned __int8)v20) + 256;
	}
	LOBYTE(v11) = v19 + v20 - v17;
	v10 = v22;
	HIBYTE(v11) = v22 + HIBYTE(v20);
	while (v10)
	{
		for (l = v17; l; l--)
		{
			if (!IsTilePassable_11C80(v11))
			{
				SetShiftByCastle_49EC0(a1, a1->param_0x10_16);
				return 0;
			}
			v11._axis_2d.x++;
		}
		v10--;
		v11.word = __PAIR__(v11._axis_2d.y, (unsigned __int8)v20) + 256;
	}
	SetShiftByCastle_49EC0(a1, a1->param_0x10_16);
	return 1;
}
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00038330) --------------------------------------------------------
int UpdateHouse_38330(type_entity_0x6E8E* event)//219330 //tex 96
{
	unsigned int v1; // eax
	signed int v2; // edx
	int v3; // eax
	type_entity_0x6E8E* v4x; // eax
	__int16 v5; // dx
	type_entity_0x6E8E* v6x; // edx
	__int16 v7; // cx
	__int16 v8; // ax
	__int16 v9; // cx
	unsigned int v10; // ecx
	int result; // eax

	v1 = CompareEvent08_38B00(event);
	if (v1 < 1)
	{
		if (v1)
		{
			result = GetTerrainAlt_10C40(&event->position_0x4C_76);
			event->position_0x4C_76.z = result;
			return result;
		}
	}
	else
	{
		if (v1 > 1)
		{
			if (v1 == 2)
				event->actionIndex_0x45_69 = 53;
			result = GetTerrainAlt_10C40(&event->position_0x4C_76);
			event->position_0x4C_76.z = result;
			return result;
		}
		v2 = event->param_0x10_16;
		if (v2 > 2)
		{
			v3 = event->levelPar3_0x46_70;
			event->param_0x10_16 = v2 - 1;
			if (v3 >= 0 && buildingParams_D93C0[v3].flags_2 & 1)//219378 movsx eax,byte ptr [ebx+46h]
			{
				predictedAxis_EB398ar = event->position_0x4C_76;
				predictedAxis_EB398ar.x += event->params_0x52_82.pitch;
				v4x = CreateEntity_4A190(&predictedAxis_EB398ar, 5, 4);
				if (v4x)
				{
					v4x->actionIndex_0x45_69 = 33;
					v5 = event->hitById_0x26_38;
					v4x->mail_0x5E_94.channel[0].amount.dword = 1;
					v4x->mail_0x5E_94.channel[0].source = v5;
				}
			}
			v6x = Entities_EA3E4[event->hitById_0x26_38];
			if (v6x > Entities_EA3E4[0] && (!v6x->model_0x40_64 || v6x->model_0x40_64 == 1))
				Entities_EA3E4[event->hitById_0x26_38]->playerData_0xA4_164x->attackedTimer_0x248_584 = 200;
		}
	}
	v7 = event->mail_0x5E_94.channel[1].source;
	if (v7)
	{
		if (v7 != event->playerEntityIndex_0x94_148)
		{
			if (event->mail_0x5E_94.channel[1].amount.dword)
			{
				event->playerEntityIndex_0x94_148 = v7;
				PrepareEventSound_6E450(v7, -1, 4);
				event->flags_0xc_12_15.dword &= 0xFFDFFFFE;
				event->flags_0xc_12_15.byte[2] |= 0x20u;
				SetEntityIndexAndRot_49CD0(event, 177);
				// this is fixed bug from original game!!!!!!
				// claimed building used to fly wizard 0's white banner in multiplayer.
				event->spriteIndex_0x5A_90 += TransformPlayerColorIndex_616D0(Entities_EA3E4[event->mail_0x5E_94.channel[1].source]->playerData_0xA4_164x->playerColorIndex_0x38_56);
			}
			else if (!(event->flags_0xc_12_15.byte[2] & 0x20))
			{
				v8 = event->mail_0x5E_94.channel[1].source;
				event->playerEntityIndex_0x94_148 = v8;
				PrepareEventSound_6E450(v8, -1, 4);
				event->flags_0xc_12_15.byte[0] &= 0xFEu;
				SetEntityIndexAndRot_49CD0(event, 177);
				// this is fixed bug from original game!!!!!!
				// claimed building used to fly wizard 0's white banner in multiplayer.
				event->spriteIndex_0x5A_90 += TransformPlayerColorIndex_616D0(Entities_EA3E4[event->mail_0x5E_94.channel[1].source]->playerData_0xA4_164x->playerColorIndex_0x38_56);
			}
		}
		event->mail_0x5E_94.channel[1].source = 0;
		event->mail_0x5E_94.channel[1].amount.dword = 0;
	}
	if (!(event->instanceIndex_0x3E_62 & 0x1F))
	{
		if (event->levelPar3_0x46_70 >= 0 && buildingParams_D93C0[event->levelPar3_0x46_70].flags_2 & 1)//2194FC movsx
		{
			v9 = event->minSpeed_0x84_132;
			if (v9 > 5 && v9 == event->param_0x10_16)
			{
				v10 = event->minSpeed_0x84_132;
				event->rand_0x14_20 = 9377 * event->rand_0x14_20 + 9439;
				if ((signed int)(event->rand_0x14_20 % v10) > event->minSpeed_0x84_132
					- (event->minSpeed_0x84_132 >> 4)
					- 2)
				{
					predictedAxis_EB398ar = event->position_0x4C_76;
					predictedAxis_EB398ar.x += event->params_0x52_82.pitch;
					GetRandManaSphere_38270(event);
				}
			}
		}
	}
	if (event->levelPar3_0x46_70 >= 0 && buildingParams_D93C0[event->levelPar3_0x46_70].flags_2 & 1)//219585 movsx
		SetMaxDistance_5C8D0(event);
	result = GetTerrainAlt_10C40(&event->position_0x4C_76);
	event->position_0x4C_76.z = result;
	return result;
}

//----- (000385C0) --------------------------------------------------------
void RemoveCastleStage_385C0(type_entity_0x6E8E* event)//2195c0 //remove castle stage //tex 96
{
	uaxis_2d locAxis1;
	uaxis_2d locAxis2;
	uaxis_2d locAxis3;
	int16_t zKoef;
	uint8_t* locData2;
	type_entity_0x6E8E* tempEvent;

	uint8_t* locData = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[event->levelPar3_0x46_70].data;
	unsigned int locHeight = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[event->levelPar3_0x46_70].height_5;
	unsigned int locWidth = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[event->levelPar3_0x46_70].width_4;
	if (videoMode_180660 == 1)
	{
		locHeight >>= 1;
		locWidth >>= 1;
	}
	locAxis1._axis_2d.x = ((unsigned __int16)(event->position_0x4C_76.x + 128) >> 8) - (locWidth >> 1);
	locAxis1._axis_2d.y = ((unsigned __int16)(event->position_0x4C_76.y + 128) >> 8) - (locHeight >> 1);
	if (!event->fontTypeIndex_0x3D_61)
	{
		if (event->model_0x40_64)
			zKoef = GetTerrainHeightFromSquare_48DF0(locAxis1._axis_2d.x, locAxis1._axis_2d.y, locHeight, locWidth);
		else
			zKoef = event->position_0x4C_76.z >> 5;
		locAxis2.word = locAxis1.word;
		locData2 = locData;
		int locIndex = 0;
		for (int y=0; y < locHeight; y++)
		{
			locAxis3.word = locAxis2.word;
			for(int x = 0; x < locWidth;x++)
			{
				if (locData2[1] != 0xff || locData2[0] != 0xff)
				{
					predictedAxis_EB398ar.x = locAxis3._axis_2d.x << 8;
					predictedAxis_EB398ar.y = locAxis3._axis_2d.y << 8;
					predictedAxis_EB398ar.z = 32 * zKoef;
					locIndex++;
					if (!(locIndex & 7))
						predictedAxis_EB398ar.z = 32 * (zKoef - 10);
					if (event->param_0x10_16 > 0)
					{
						event->param_0x10_16--;
						if (buildingParams_D93C0[event->levelPar3_0x46_70].flags_2 & 1)
						{
							if (event->param_0x10_16)
							{
								if (event->param_0x10_16 >= 4)
								{
									tempEvent = GetRandManaSphere_38270(event);
								}
								else
								{
									tempEvent = CreateEntity_4A190(&predictedAxis_EB398ar, 5, 4);
									if (tempEvent)
										tempEvent->actionIndex_0x45_69 = 33;
								}
							}
							else
							{
								tempEvent = CreateEntity_4A190(&predictedAxis_EB398ar, 5, 12);
								if (tempEvent)
									tempEvent->actionIndex_0x45_69 = 97;
							}
							if (tempEvent)
							{
								tempEvent->mail_0x5E_94.channel[0].amount.dword = 1;
								tempEvent->mail_0x5E_94.channel[0].source = event->hitById_0x26_38;
							}
						}
					}
					mapAngle_13B4E0[locAxis3.word] = mapAngle_13B4E0[locAxis3.word] & 0x70 | 1;
					AddBuildingToTerrain_46570(locAxis3, locAxis3);
					if (locData2[1] != 0xff)
					{
						if (locData2[1] >= mapHeightmap_11B4E0[locAxis3.word])
						{
							mapHeightmap_11B4E0[locAxis3.word] = 0;
						}
						else
						{
							event->rand_0x14_20 = 9377 * event->rand_0x14_20 + 9439;
							if ((signed int)(event->rand_0x14_20 % 0x32u) <= 20)
							{
								mapHeightmap_11B4E0[locAxis3.word] -= locData2[1];
							}
							else
							{
								event->rand_0x14_20 = 9377 * event->rand_0x14_20 + 9439;
								mapHeightmap_11B4E0[locAxis3.word] -= locData2[1] - event->rand_0x14_20 % 0x14u;
							}
						}
					}
				}
				locData2 += 2;
				locAxis3._axis_2d.x++;
			}
			locAxis2._axis_2d.y++;
		}
		SetHeightmapByBuildingArea_48B50(locAxis1._axis_2d.x, locAxis1._axis_2d.y, locHeight, locWidth);
		if (event->xtype_0x41_65)
			ResetLevelState_4A1E0(event->xtype_0x41_65, 1);
		if (event->levelPar3_0x46_70 == 68)
		{
			world_D41A0.buildingCheckEntity_0x3654A = 0;
			DisableEntityDrawing04_57F10(event);
			return;
		}
		DisableEntityDrawing04_57F10(event);
		return;
	}
	tempEvent = CreateEntity_4A190(&event->position_0x4C_76, 10, 45);
	if (!tempEvent)
	{
		event->fontTypeIndex_0x3D_61 = 0;
		DisableEntityDrawing04_57F10(event);
		return;
	}
	InitBuildingEntity_49A30(tempEvent, event->fontTypeIndex_0x3D_61);
	tempEvent->position_0x4C_76.z = event->axis_0x9A_154x.z;
	tempEvent->xtype_0x41_65 = event->xtype_0x41_65;
	if (event->playerEntityIndex_0x94_148)
	{
		tempEvent->playerEntityIndex_0x94_148 = event->playerEntityIndex_0x94_148;
		tempEvent->flags_0xc_12_15.byte[0] |= 1;
		if (tempEvent->flags_0xc_12_15.byte[2] & 0x20)
			tempEvent->flags_0xc_12_15.byte[2] |= 0x20u;
		else
			tempEvent->flags_0xc_12_15.byte[2] &= 0xDFu;
		SetEntityIndexAndRot_49CD0(tempEvent, 177);
		tempEvent->spriteIndex_0x5A_90 = event->spriteIndex_0x5A_90;
	}
	CheckObjectiveKill_59760(event, tempEvent);
	// The final sweep clears bit 0x80 over the WHOLE footprint: retail
	// (0x21999D) resets the cell to the row origin per row,
	// `inc ch` per cell and `inc dl` (the origin's y) per row. The cell
	// index used to stay at the origin, so only one corner was cleared
	// (mgcr replay, mc2l1-new t=11829/11868/14211: 109-152 cells left
	// with 0x80 that retail had cleared).
	locData2 = locData;
	uaxis_2d rowOrigin = locAxis1;
	for (int y = 0; y < locHeight; y++)
	{
		uaxis_2d cell = rowOrigin;
		for (int x = 0; x < locWidth; x++)
		{
			if (locData2[1] != 0xff || locData2[0] != 0xff)
				mapAngle_13B4E0[cell.word] &= 0x7Fu;
			locData2 += 2;
			cell._axis_2d.x++;
		}
		rowOrigin._axis_2d.y++;
	}
	DisableEntityDrawing04_57F10(event);
}

typedef union {
	type_entity_0x6E8E* un_0x6E8E;
	int32_t unint;
} type_un_v3;

//----- (0003AF00) --------------------------------------------------------
void CastleDefendEvent_3AF00(type_entity_0x6E8E* a1x)//21bf00 //tex 55
{
	int v1; // edx
	__int16 v2; // dx
	type_un_v3 v3;
	//type_str_0x6E8E* v3x; // esi
	//int v3z = 0;
	//int16_t* v3y; // esi
	unsigned int v4; // edx
	int v5; // eax
	type_entity_0x6E8E* ix; // ecx
	unsigned __int8 v7; // al
	unsigned __int8 v8; // al
	bool v9; // zf
	int v10; // eax
	int v11; // edx
	__int16 v12; // di
	signed int v13; // edx
	__int16 v14; // ax
	unsigned __int16 v15; // ax
	type_entity_0x6E8E* v16x; // edi
	type_entity_0x6E8E* v17x; // eax
	__int16 v18; // ax
	__int16 v19; // ax
	char v20; // dh
	char v21; // al
	type_entity_0x6E8E* v22x; // eax
	char v23; // dl
	__int16 v24; // ax
	char v25; // dh
	__int16 v26; // ax
	int result; // eax
	int v28; // ecx
	int v29; // ecx
	//int16_t* v30; // esi
	//signed int v31; // edx
	int v32; // [esp+0h] [ebp-30h]
	int v33; // [esp+4h] [ebp-2Ch]
	int v34; // [esp+8h] [ebp-28h]
	int v35; // [esp+Ch] [ebp-24h]
	int v36; // [esp+10h] [ebp-20h]
	int v37; // [esp+14h] [ebp-1Ch]
	type_entity_0x6E8E* v38x; // [esp+18h] [ebp-18h]
	char v39; // [esp+1Ch] [ebp-14h]
	unsigned __int8 v40; // [esp+20h] [ebp-10h]
	char v41; // [esp+24h] [ebp-Ch]
	unsigned __int8 v42; // [esp+28h] [ebp-8h]
	char v43; // [esp+2Ch] [ebp-4h]

	v1 = a1x->life_0x8;
	v41 = 0;
	if (v1 < 0)
	{
		DisableEntityDrawing04_57F10(a1x);
		return;
	}
	v2 = a1x->id_0x1A_26;
	if (!v2)
	{
		DisableEntityDrawing04_57F10(a1x);
		return;
	}
	v38x = Entities_EA3E4[v2];
	v3.un_0x6E8E = Entities_EA3E4[v38x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58];
	if (a1x->nextStage_0x4A_74 <= 1)
		v37 = 384;
	else
		v37 = 224;
	switch (a1x->levelPar3_0x46_70)
	{
	case 0:
		a1x->axis_0x9A_154x = a1x->position_0x4C_76;
		//v3y = a1x->array_0x52_82;
		a1x->levelPar3_0x46_70 = 1;
		goto LABEL_74;
	case 1:
		a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
		v4 = a1x->rand_0x14_20 % 0x30u;
		a1x->levelPar3_0x46_70 = 2;
		a1x->param_0x10_16 = v4 + 16;
		goto LABEL_9;
	case 2:
	LABEL_9:
		v5 = a1x->param_0x10_16 - 1;
		a1x->param_0x10_16 = v5;
		if (!v5)
			a1x->levelPar3_0x46_70 = 3;
		goto LABEL_74;
	case 3:
		if (a1x->instanceIndex_0x3E_62 & 0x3F)
			goto LABEL_74;
		v36 = (a1x->position_0x4C_76.x + 128) >> 8;
		v35 = (a1x->position_0x4C_76.y + 128) >> 8;
		v3.unint = AddE7EE0x_10080(3, 12);
		if (!v3.un_0x6E8E)
			goto LABEL_74;
		v39 = 0;
		break;
	case 4:
		a1x->levelPar3_0x46_70 = 5;
		a1x->param_0x10_16 = 4;
		goto LABEL_37;
	case 5:
	LABEL_37:
		v11 = a1x->param_0x10_16 - 1;
		a1x->param_0x10_16 = v11;
		if (v11)
		{
			a1x->param3_0x36_54 += 160;
		}
		else
		{
			a1x->levelPar3_0x46_70 = 6;
			a1x->param3_0x36_54 = 0;
		}
		goto LABEL_74;
	case 6:
		v12 = a1x->rand_0x14_20;
		a1x->axis_0x9A_154x.z = a1x->position_0x4C_76.z;
		a1x->rand_0x14_20 = 9377 * v12 + 9439;
		v13 = a1x->rand_0x14_20 % 0x64u;
		a1x->levelPar3_0x46_70 = 7;
		if (v13)
		{
			if (v13 <= 5)
			{
				v14 = (a1x->levelPar1_0x43_67 == 1) + 2;
			}
			else
			{
				v14 = a1x->levelPar1_0x43_67 != 1;
			}
			a1x->param2_0x2C_44 = v14;
		}
		else
		{
			a1x->param2_0x2C_44 = 4;
		}
		v15 = a1x->param2_0x2C_44;
		a1x->fontTypeIndex_0x3D_61 = 1;
		if (v15 <= 1u)
			a1x->fontTypeIndex_0x3D_61 = 6;
		goto LABEL_48;
	case 7:
	case 8:
	LABEL_48:
		v43 = 0;
		if (!a1x->targetId_0x96_150)
			goto LABEL_104;
		v16x = Entities_EA3E4[a1x->targetId_0x96_150];
		if (v16x->life_0x8 < 0 || v16x->flags_0xc_12_15.byte[1] & 4)
			goto LABEL_104;
		switch (a1x->param2_0x2C_44)
		{
		case 0:
			v40 = 0;
			v42 = 1;
			break;
		case 1:
			v40 = 7;
			goto LABEL_57;
		case 2:
			v40 = 7;
			v42 = 1;
			break;
		case 3:
			v40 = 0;
			v42 = 2;
			break;
		case 4:
			v40 = 9;
		LABEL_57:
			v42 = 0;
			break;
		default:
			break;
		}
		//v17x = CastSpell_6DCA0(v3.un_0x6E8E, &a1x->array_0x4C_76, v40, (__int16*) & (*xadataspellsdat.colorPalette_var28)[80 * v40 + 2 + 26 * v42], 0, a1x->byte_0x46_70 == 7);
		v17x = CastSpell_6DCA0(v3.un_0x6E8E, &a1x->position_0x4C_76, v40, &SPELLS_BEGIN_BUFFER_str[v40].subspell[v42], 0, a1x->levelPar3_0x46_70 == 7);
		v3.un_0x6E8E = v17x;
		if (v17x)
		{
			v17x->id_0x1A_26 = a1x->id_0x1A_26;
			v17x->targetId_0x96_150 = a1x->targetId_0x96_150;
			if (v38x->playerData_0xA4_164x->playerColorIndex_0x38_56 == world_D41A0.LevelIndex_0xc && !v40)
				SetEntityIndex_49C90(v17x, 42);
			AimAtEntity_655C0(v3.un_0x6E8E, v16x);
			v3.un_0x6E8E->yaw_0x1C_28 = v3.un_0x6E8E->roll_0x20_32;
			v3.un_0x6E8E->pitch_0x1E_30 = v3.un_0x6E8E->fov_0x22_34;
			v18 = v3.un_0x6E8E->yaw_0x1C_28;
			v3.un_0x6E8E->position_0x4C_76.z += a1x->params_0x52_82.yaw;
			v18 &= 0x7ffu;
			a1x->yaw_0x1C_28 = v18;
			v19 = v3.un_0x6E8E->pitch_0x1E_30;
			v19 &= 0x7ffu;
			v20 = a1x->levelPar2_0x44_68;
			a1x->pitch_0x1E_30 = v19;
			if (v20)
			{
				a1x->levelPar2_0x44_68 = v20 + 1;
				if ((char)(v20 + 1) > 5)
					a1x->levelPar2_0x44_68 = 5;
			}
			else
			{
				a1x->levelPar2_0x44_68 = 1;
			}
			v21 = a1x->fontTypeIndex_0x3D_61 - 1;
			a1x->fontTypeIndex_0x3D_61 = v21;
			if (!v21)
				LABEL_104:
			v43 = 1;
		}
		if (v43)
		{
			a1x->targetId_0x96_150 = 0;
			a1x->levelPar3_0x46_70 = 1;
		}
		else
		{
			a1x->levelPar3_0x46_70 = 8;
		}
		goto LABEL_74;
	case 9:
		goto LABEL_73;
	case 0xA:
		v22x = CreateEntity_4A190(&a1x->position_0x4C_76, 10, 1);
		if (v22x)
			v22x->id_0x1A_26 = a1x->id_0x1A_26;
	LABEL_73:
		v41 = 1;
		goto LABEL_74;
	default:
		goto LABEL_74;
	}
LABEL_33:
	if (GetNextSearchOffset_10130(v3.unint, &v33, &v32) == 1 && !v39)
	{
		/*//fix
		int previndex = -1;
		//fix*/

		for (ix = Entities_EA3E4[mapEntityIndex_15B4E0[(unsigned __int8)(v33 + v36) + ((unsigned __int8)(v32 + v35) << 8)]];
			;
			ix = Entities_EA3E4[ix->oldMapEntity_0x16_22])
		{
			//fix
			/*if (ix->word_0x16_22 == Entities_EA3E4[ix->word_0x16_22]->word_0x16_22)
				return;*/
				//fix

			if (ix == Entities_EA3E4[0] || v39)
				goto LABEL_33;
			v7 = ix->class_0x3F_63;
			if (v7 >= 3u)
			{
				if (v7 <= 3u)
				{
					v8 = ix->model_0x40_64;
					if (v8 <= 1u || v8 == 3)
					{
						v9 = ix->id_0x1A_26 == a1x->id_0x1A_26;
					LABEL_26:
						if (v9)
							goto LABEL_28;
					LABEL_27:
						v39 = 1;
						goto LABEL_28;
					}
				}
				else if (v7 == 5 && ix->model_0x40_64 != 22 && ix->id_0x1A_26 != a1x->id_0x1A_26)
				{
					if (ix->StageVar2_0x49_73 == 14)
					{
						v9 = ix->parentId_0x28_40 == a1x->id_0x1A_26;
						goto LABEL_26;
					}
					goto LABEL_27;
				}
			}
		LABEL_28:
			if (v39)
			{
				v10 = ix - world_D41A0.entities_0x6E8E;
				a1x->levelPar3_0x46_70 = 4;
				a1x->targetId_0x96_150 = v10;
			}
		}
	}
	ResetEvent08_10100(v3.unint);
LABEL_74:
	if (v41)
	{
		ReleaseEntity_57F20(a1x);
		return;
	}
	v23 = a1x->levelPar2_0x44_68;
	if (v23)
	{
		switch (abs(v23))
		{
		case 1:
			v34 = 0;
			break;
		case 2:
			v34 = 115;
			break;
		case 3:
			v34 = 230;
			break;
		case 4:
			v34 = 334;
			break;
		case 5:
			v34 = 368;
			break;
		case 6:
			v34 = 384;
			break;
		default:
			break;
		}
		v24 = -(signed __int16)v34;
		predictedAxis_EB398ar = a1x->axis_0x9A_154x;
		//v3z = a1x->dword_0xA0_160;
		MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, a1x->pitch_0x1E_30, v24);
		CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
		v25 = a1x->levelPar2_0x44_68 + 1;
		a1x->levelPar2_0x44_68 = v25;
		if (v25 > 0 && v25 > 6)
			a1x->levelPar2_0x44_68 = -5;
	}
	v26 = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
	result = v37 + v26;
	v28 = a1x->position_0x4C_76.z;
	if (v28 < result)
	{
	LABEL_98:
		a1x->position_0x4C_76.z = result;
		return;
	}
	//LOWORD(v3) = *(x_WORD *)(a1 + 54);
	if (a1x->param3_0x36_54)
	{
		result += a1x->param3_0x36_54;
		goto LABEL_98;
	}
	if (!a1x->levelPar2_0x44_68)
	{
		v29 = v28 - result;
		result = abs(v29);
		if (result > 32)
		{
			//v30 = &a1x->array_0x4C_76.z;
			if (v29 <= 0)
				a1x->position_0x4C_76.z += (int16_t)16;
			else
				a1x->position_0x4C_76.z += (int16_t)-16;
			//result = v31 + (signed __int16)*v30;
			//*v30 = result;
			//a1x->array_0x4C_76.z += v31;
		}
		if (a1x->instanceIndex_0x3E_62 & 8)
			a1x->position_0x4C_76.z += (int16_t)6;
		else
			a1x->position_0x4C_76.z += (int16_t)-6;
	}
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;

//----- (00049A30) --------------------------------------------------------
void InitBuildingEntity_49A30(type_entity_0x6E8E* a1x, unsigned __int16 a2)//22aa30
{
	bitmap_pos_struct_t v2; // eax
	__int16 v3; // dx
	//int v4; // eax
	char v5; // dl
	int v6; // edx
	unsigned __int16 v8; // [esp+0h] [ebp-10h]
	unsigned __int16 v9; // [esp+4h] [ebp-Ch]
	unsigned __int8 v10; // [esp+8h] [ebp-8h]
	unsigned __int8 v11; // [esp+Ch] [ebp-4h]

	v2 = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[a2];
	v8 = v2.height_5;
	v9 = v2.width_4;
	a1x->param_0x10_16 = 2;
	v3 = videoMode_180660;
	a1x->minSpeed_0x84_132 = v9 * v8 >> 4;
	if (v3 == 1)
	{
		v8 >>= 1;
		v9 >>= 1;
	}
	predictedAxis_EB398ar = a1x->position_0x4C_76;
	predictedAxis_EB398ar.x = a1x->position_0x4C_76.x >> 8 << 8;
	predictedAxis_EB398ar.y = a1x->position_0x4C_76.y >> 8 << 8;
	CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
	v10 = (a1x->position_0x4C_76.x >> 8) - ((signed int)v9 >> 1);
	v11 = (a1x->position_0x4C_76.y >> 8) - ((signed int)v8 >> 1);
	if ((v10 + v11) % 2)
	{
		predictedAxis_EB398ar = a1x->position_0x4C_76;
		predictedAxis_EB398ar.x += 256;
		CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
		v10++;
	}
	SetShiftByCastle_49EC0(a1x, a2);
	a1x->position_0x4C_76.z = 32 * GetTerrainHeightFromSquare_48DF0(v10, v11, v8, v9);
	a1x->levelPar3_0x46_70 = a2;
	//LOWORD(v4) = a2;
	a1x->life_0x8 = 30;
	a1x->subSpellIndex_0x2A_42 = buildingParams_D93C0[a2].subSpellIndex_0;
	v5 = buildingParams_D93C0[a2].nextIndex_3;
	a1x->xtype_0x41_65 = 0;
	a1x->mana_0x90_144 = 0;
	a1x->fontTypeIndex_0x3D_61 = v5;
	if (!(buildingParams_D93C0[a2].flags_2 & 8))
	{
		v6 = a1x->mana_0x90_144;
		a1x->mailMask_0x38_56 |= 2u;
		if (v6 < 0)
			a1x->mana_0x90_144 = 0;
		if (a1x->mana_0x90_144 > 1000)
			a1x->mana_0x90_144 = 1000;
		//v4 = 1000 * a1x->word_0x2A_42 >> 7;
		a1x->mana_0x90_144 = 1000 * a1x->subSpellIndex_0x2A_42 >> 7;
	}
	if (a2 == 68)
	{
		world_D41A0.buildingCheckEntity_0x3654A = a1x - world_D41A0.entities_0x6E8E;
	}
	//return v4;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D93C0: using guessed type __int16 x_WORD_D93C0_bldgprmbuffer[];
// EA3BC: using guessed type int **filearray_2aa18c[24];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39A: using guessed type __int16 x_WORD_EB398ar[1];
// 180660: using guessed type __int16 videoMode_180660;

//----- (0004AA40) --------------------------------------------------------
type_entity_0x6E8E* AddCastle_4AA40(axis_3d* position)//22ba40 //Spawn Scenery Bad Stone //tex 96
{
	type_entity_0x6E8E* event; // eax
	type_entity_0x6E8E* v2x; // ebx
	unsigned __int16 v4; // ax
	unsigned __int16 v5; // cx
	axis_3d v6ar; // [esp+0h] [ebp-Ch]
	int v8; // [esp+8h] [ebp-4h]

	predictedAxis_EB398ar = *position;
	event = NewEvent_4A050();
	v2x = event;
	if (event)
	{
		event->actionIndex_0x45_69 = 5;
		event->class_0x3F_63 = 3;
		event->model_0x40_64 = 2;
		event->maxLife_0x4 = 40000;
		event->param_0x10_16 = 0;
		event->mailMask_0x38_56 = 33;
		v6ar = predictedAxis_EB398ar;
		v6ar.x >>= 8;
		v6ar.y >>= 8;
		v6ar.z = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
		if ((v6ar.x + v6ar.y) % 2)
			v6ar.x++;
		v6ar.x <<= 8;
		v6ar.y <<= 8;
		v2x->axis_0x9A_154x = v6ar;
		v4 = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[1].width_4;
		v5 = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[1].height_5;
		if (videoMode_180660 == 1)
		{
			v5 >>= 1;
			v4 >>= 1;
		}
		v8 = (v6ar.x >> 8) - v4 / 2;
		v2x->axis_0x9A_154x.z = 32 * GetMinHeightInArea_48E60((v6ar.x >> 8) - v4 / 2, (v6ar.y >> 8) - v5 / 2, v4, v5);
		AddEventToMap_57D70(v2x, &v6ar);
		CopyMaxLifeToLife_49A20(v2x);
		SetEntityIndexAndRot_49CD0(v2x, 177);
		event = v2x;
	}
	return event;
}

//----- (0004ABA0) --------------------------------------------------------
type_entity_0x6E8E* AddCastleBalloon_4ABA0(axis_3d* position)//22bba0 //Spawn Creature 3? //tex 88
{
	type_entity_0x6E8E* entity = NewEvent_4A050();
	if (entity)
	{
		entity->actionIndex_0x45_69 = 7;
		entity->class_0x3F_63 = 3;
		entity->model_0x40_64 = 3;
		entity->maxLife_0x4 = 10000;
		entity->actSpeed_0x82_130 = 48;
		entity->maxMana_0x8C_140 = 10000;
		entity->mana_0x90_144 = 0;
		entity->mailMask_0x38_56 = 1;
		entity->typeParams_0xA0_160x = &str_D7BD6[68]; //(type_str_160*)&unk_D7BD6[0x908];
		AddEventToMap_57D70(entity, position);
		CopyMaxLifeToLife_49A20(entity);
		SetEntityIndexAndRot_49CD0(entity, 169);
		if (isCaveLevel_D41B6)
			SetEntityShiftRot_49EA0(entity, 256, 768);
	}
	return entity;
}

//----- (000508E0) --------------------------------------------------------
type_entity_0x6E8E* CastleDefendCreate_508E0(axis_3d* position)//2318e0 //tex 80
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->maxLife_0x4 = 100000;
		event->actionIndex_0x45_69 = 0x56;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x4F;
		event->position_0x4C_76 = *position;
		event->levelPar3_0x46_70 = 0;
		event->levelPar1_0x43_67 = 0;
		event->param2_0x2C_44 = 0;
		event->fontTypeIndex_0x3D_61 = 1;
		event->param3_0x36_54 = 0;
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 66);
	}
	return event;
}

//----- (00059F60) --------------------------------------------------------
void UpdateCastle_59F60(type_entity_0x6E8E* a1x)//23af60
{
	unsigned int v1; // eax
	//uaxis_2d v2x; // eax
	char v3; // al
	uaxis_2d v4x; // bx
	uaxis_2d v5x; // cx
	signed int v6; // eax
	uaxis_2d v7x; // bx
	uaxis_2d v8x; // cx
	signed int v9; // edx
	uaxis_2d v10x; // di
	int v11; // eax
	uaxis_2d v12x; // bx
	signed int v13; // edx
	uaxis_2d v14x; // cx
	uaxis_2d v15x; // di
	int v16; // edi
	uaxis_2d v17x; // bx
	uaxis_2d v18x; // cx
	unsigned __int8 v19; // dh
	signed int v20; // edx
	uaxis_2d v21x; // bx
	uaxis_2d v22x; // ax
	uaxis_2d v23x; // cx
	char v24; // al
	//uaxis_2d v25x; // bx
	char v26; // cl
	uaxis_2d v27x; // ax
	int v28; // eax
	uaxis_2d v29x; // bx
	signed int v30; // edx
	uaxis_2d v31x; // cx
	uaxis_2d v32x; // di
	uaxis_2d v33x; // bx
	uaxis_2d v34x; // cx
	uaxis_2d v35x; // ax
	uaxis_2d v36x; // bx
	signed int v37; // eax
	signed int v38; // edx
	uaxis_2d v39x; // cx
	uaxis_2d v40x; // di
	uaxis_2d v41x; // bx
	signed int v42; // eax
	int v43; // edx
	uaxis_2d v44x; // cx
	uaxis_2d v45x; // di
	uaxis_2d v46x; // bx
	signed int v47; // edi
	uaxis_2d v48x; // cx
	unsigned __int8 v49; // dh
	signed int v50; // edx
	uaxis_2d v51x; // bx
	uaxis_2d v52x; // ax
	uaxis_2d v53x; // cx
	char v54; // al
	char v55; // cl
	uaxis_2d v56x; // ax
	signed int v57; // eax
	uaxis_2d v58x; // bx
	int v59; // edx
	uaxis_2d v60x; // cx
	uaxis_2d v61x; // di
	char v62; // al
	int v63; // eax
	uaxis_2d v64x; // bx
	uaxis_2d v65x; // cx
	signed int v66; // edx
	uaxis_2d v67x; // di
	int v68; // edx
	uaxis_2d v69x; // bx
	signed int v70; // eax
	uaxis_2d v71x; // cx
	uaxis_2d v72x; // di
	int v73; // edx
	uaxis_2d jx; // bx
	signed int v75; // eax
	uaxis_2d v76x; // cx
	uaxis_2d v77x; // di
	uaxis_2d v78x; // bx
	signed int v79; // edx
	int v80; // eax
	uaxis_2d v81x; // cx
	uaxis_2d v82x; // di
	signed int v83; // edx
	uaxis_2d v84x; // bx
	int v85; // eax
	uaxis_2d v86x; // cx
	uaxis_2d v87x; // di
	signed int v88; // edx
	uaxis_2d v89x; // bx
	int v90; // eax
	uaxis_2d v91x; // cx
	uaxis_2d v92x; // di
	char v93; // al
	int v94; // eax
	uaxis_2d v95x; // ebx
	uaxis_2d v96x; // dx
	char v97; // cl
	uaxis_2d v98x; // dx
	int v99; // edx
	uaxis_2d kx; // bx
	unsigned __int8 v101; // ch
	unsigned __int8 v102; // ch
	int v103; // eax
	uaxis_2d v104x; // ebx
	uaxis_2d v105x; // dx
	char v106; // cl
	uaxis_2d v107x; // ebx
	uaxis_2d v108x; // dx
	int v109; // edx
	uaxis_2d v110x; // ebx
	unsigned __int8 v111; // ch
	uaxis_2d v112x; // ebx
	unsigned __int8 v113; // ch
	char v114; // al
	signed int v115; // edx
	uaxis_2d lx; // bx
	uaxis_2d v117x; // ax
	uaxis_2d v118x; // cx
	char v119; // al
	uaxis_2d v120x; // bx
	char v121; // cl
	uaxis_2d v122x; // ax
	uaxis_2d v123x; // bx
	signed int v124; // edx
	uaxis_2d v125x; // ax
	uaxis_2d v126x; // cx
	char v127; // al
	uaxis_2d v128x; // bx
	char v129; // cl
	uaxis_2d v130x; // ax
	//uint8_t* v131; // edx
	char v132; // al
	int v133; // edx
	uaxis_2d mx; // bx
	uaxis_2d v135x; // ax
	int v136; // ecx
	int v137; // edi
	int v138; // ecx
	int v139; // edx
	uaxis_2d v140x; // ebx
	int v141; // ecx
	int v142; // edi
	uaxis_2d v143x; // ebx
	int v144; // ecx
	__int16 v145; // di
	char v146; // al
	int v147; // edx
	uaxis_2d v148x; // bx
	uaxis_2d v149x; // ax
	uaxis_2d v150x; // di
	uaxis_2d v151x; // di
	uaxis_2d v152x; // di
	char v153; // ch
	char v154; // cl
	uaxis_2d v155x; // di
	char v156; // ah
	int v157; // edx
	uaxis_2d v158x; // bx
	unsigned __int8 v159; // ch
	unsigned __int8 v160; // ch
	unsigned __int8 v161; // ch
	unsigned __int8 v162; // ch
	int v163; // edx
	uaxis_2d v164x; // bx
	uaxis_2d v165x; // bx
	uaxis_2d v166x; // ax
	char v167; // dh
	uaxis_2d v168x; // ax
	char v169; // cl
	signed int v170; // eax
	uaxis_2d v171x; // cx
	uaxis_2d v172x; // di
	int v173; // edx
	uaxis_2d v174x; // ebx
	uaxis_2d v175x; // ax
	uaxis_2d v176x; // ebx
	uaxis_2d v177x; // di
	uaxis_2d v178x; // di
	uaxis_2d v179x; // di
	char v180; // ch
	char v181; // cl
	uaxis_2d v182x; // di
	char v183; // ah
	int v184; // edx
	uaxis_2d v185x; // ebx
	uaxis_2d v186x; // ebx
	unsigned __int8 v187; // ch
	uaxis_2d v188x; // ebx
	unsigned __int8 v189; // ch
	uaxis_2d v190x; // ebx
	unsigned __int8 v191; // ch
	uaxis_2d v192x; // ebx
	unsigned __int8 v193; // ch
	signed int v194; // eax
	uaxis_2d v195x; // bx
	uaxis_2d v196x; // bx
	uaxis_2d v197x; // ax
	signed int v198; // edx //0x23BFF9 mov edx, -3
	uaxis_2d v199x; // cx
	uaxis_2d v200x; // di
	signed int v201; // [esp+4h] [ebp-1Ch]
	int v202; // [esp+8h] [ebp-18h]
	signed int v203; // [esp+Ch] [ebp-14h]
	int i; // [esp+10h] [ebp-10h]
	signed int v205; // [esp+14h] [ebp-Ch]
	int v206; // [esp+18h] [ebp-8h]
	uaxis_2d v207x; // [esp+1Ch] [ebp-4h]
	uaxis_2d v208x; // [esp+1Ch] [ebp-4h]

	if (CommandLineParams.DoDebugSequences()) {
		//add_compare(0x23af64, CommandLineParams.DoDebugafterload(),9);
	}

	v1 = a1x->life_0x8;
	if (v1 < 1)
	{
		if (!v1)
		{
			/*LOBYTE(v208) = (unsigned __int16)(a1x->array_0x4C_76.x + 128) >> 8;
			v2 = (a1x->array_0x4C_76.y + 128) >> 8;
			HIBYTE(v208) = (unsigned __int16)(a1x->array_0x4C_76.y + 128) >> 8;
			BYTE1(v2) = a1x->byte_0x46_70;*/
			v208x._axis_2d.x = (unsigned __int16)(a1x->position_0x4C_76.x + 128) >> 8;
			v208x._axis_2d.y = (unsigned __int16)(a1x->position_0x4C_76.y + 128) >> 8;
			a1x->param_0x10_16++;
			if (a1x->levelPar3_0x46_70)
				//HIBYTE(v208) = v2 - 1;
				v208x._axis_2d.y--;
			else
				v208x._axis_2d.x--;
			v3 = a1x->levelPar3_0x46_70;
			if (v3)
			{
				if (v3 == 1)
				{
					v33x.word = v208x.word;
					v201 = 0;
					do
					{
						v34x.word = v33x.word;
						for (i = 0; i < a1x->param_0x10_16; i++)
						{
							if (mapTerrainType_10B4E0[v34x.word] != 8
								/*|| (LOBYTE(v35) = v33,
									HIBYTE(v35) = HIBYTE(v34) + 1,*/
								|| (v35x._axis_2d.x = v33x._axis_2d.x,
									v35x._axis_2d.y = v34x._axis_2d.y + 1,
									abs(mapHeightmap_11B4E0[v34x.word] - mapHeightmap_11B4E0[v35x.word]) > 30))
							{
								mapHeightmap_11B4E0[v34x.word] += 48;
							}
							//HIBYTE(v34)++;
							v34x._axis_2d.y++;
						}
						v33x.word--;
						v201++;
					} while (v201 < 2);
					v36x.word = v208x.word;//23b26c
					v37 = 0;
					while (v37 < 3)
					{
						v38 = -1;
						v39x.word = v36x.word - 256;
						while (v38 < a1x->param_0x10_16)
						{
							v40x.word = v39x.word;
							v38++;
							//HIBYTE(v39)++;
							v39x._axis_2d.y++;
							mapTerrainType_10B4E0[v40x.word] = 8;
							mapAngle_13B4E0[v40x.word] = 1;
						}
						v37++;
						v36x.word--;
					}
					if (CommandLineParams.DoDebugSequences()) {
						//add_compare(0x23b2a6, CommandLineParams.DoDebugafterload());
					}
					if (isCaveLevel_D41B6)//23b2a9
					{
						v46x.word = v208x.word + 1;
						v47 = 0;
						while (v47 < 4)
						{
							v202 = -1;
							v48x.word = v46x.word - 256;
							while (a1x->param_0x10_16 + 1 > v202)
							{
								v49 = mapHeightmap_11B4E0[v48x.word];
								if ((unsigned __int8)secondHeightmap_14B4E0[v48x.word] > v49)
								{
									mapAngle_13B4E0[v48x.word] &= 0xF7u;
								}
								else
								{
									secondHeightmap_14B4E0[v48x.word] = v49 - 1;
									mapAngle_13B4E0[v48x.word] |= 8u;
								}
								//HIBYTE(v48)++;
								v48x._axis_2d.y++;
								v202++;
							}
							v47++;
							//v46--;
							v46x._axis_2d.x--;
						}
					}
					else
					{
						v41x.word = v208x.word + 1;
						v42 = 0;
						while (v42 < 4)
						{
							v43 = -1;
							v44x.word = v41x.word - 256;
							while (v43 < a1x->param_0x10_16 + 1)
							{
								v45x.word = v44x.word;
								v43++;
								//HIBYTE(v44)++;
								v44x._axis_2d.y++;
								mapAngle_13B4E0[v45x.word] &= 0xF7u;
							}
							v42++;
							v41x.word--;
						}
					}
					v50 = -1;//23b33d
					//v51x.word = v208x.word - 256;
					v51x.word = v208x.word - 256;
					while (v50 < a1x->param_0x10_16)
					{
						//LOBYTE(v51) = v51 + 1;
						//HIBYTE(v51)++;
						v51x._axis_2d.x++;
						v51x._axis_2d.y++;
						v52x.word = v51x.word;
						//LOBYTE(v51) = v51 - 2;
						//HIBYTE(v51) -= 2;
						v51x._axis_2d.x -= 2;
						v51x._axis_2d.y -= 2;
						v53x.word = v51x.word;
						//LOBYTE(v51) = v51 + 1;
						v51x._axis_2d.x++;
						v54 = mapHeightmap_11B4E0[v53x.word] - mapHeightmap_11B4E0[v52x.word] + 32;
						//HIBYTE(v51)++;
						v51x._axis_2d.y++;
						if (v54 >= 28)
						{
							if (v54 > 40)
								v54 = (v54 & 7) + 40;
						}
						else
						{
							v54 = (v54 & 3) + 28;
						}
						if (world_D41A0.terrain_2FECE.MapType != MapType_t::Day)
							v55 = 32 - v54 + 32;
						else
							v55 = v54;
						v56x.word = v51x.word;
						v50++;
						//HIBYTE(v51)++;
						v51x._axis_2d.y++;
						mapShading_12B4E0[v56x.word] = v55;
					}
					v57 = -3;
					v58x.word = v208x.word - 3;
					while (v57 < 6)
					{
						v59 = -3;
						v60x.word = v58x.word - 768;
						while (v59 < a1x->param_0x10_16 + 3)
						{
							v61x.word = v60x.word;
							v59++;
							//HIBYTE(v60)++;
							v60x._axis_2d.y++;
							mapAngle_13B4E0[v61x.word] |= 0x80u;
						}
						v57++;
						v58x.word++;
					}
				}
				a1x->subSpellIndex_0x2A_42 = 48;
				a1x->life_0x8 = 3;
			}
			else
			{
				v206 = 0;
				v4x.word = v208x.word;
				while (v206 < a1x->param_0x10_16)
				{
					v205 = 0;
					v5x.word = v4x.word;
					while (v205 < 2)
					{
						if (mapTerrainType_10B4E0[v5x.word] != 8
							|| abs(mapHeightmap_11B4E0[v5x.word] - mapHeightmap_11B4E0[v5x.word + 1]) > 30)
						{
							mapHeightmap_11B4E0[v5x.word] += 48;
						}
						//HIBYTE(v5)--;
						v5x._axis_2d.y--;
						v205++;
					}
					//v4++;
					v4x.word++;
					v206++;
				}
				v6 = -1;
				v7x.word = v208x.word - 1;
				while (v6 < a1x->param_0x10_16)
				{
					v8x.word = v7x.word;
					v9 = 0;
					while (v9 < 3)
					{
						v10x.word = v8x.word;
						v9++;
						//HIBYTE(v8)--;
						v8x._axis_2d.y--;
						mapTerrainType_10B4E0[v10x.word] = 8;
						mapAngle_13B4E0[v10x.word] = 1;
					}
					v6++;
					//v7++;
					v7x.word++;
				}
				if (isCaveLevel_D41B6)//092
				{
					v16 = -1;
					v17x.word = v208x.word - 1;
					while (v16 < a1x->param_0x10_16 + 1)
					{
						v203 = 0;
						v18x.word = v17x.word + 256;
						while (v203 < 4)
						{
							v19 = mapHeightmap_11B4E0[v18x.word];
							if ((unsigned __int8)secondHeightmap_14B4E0[v18x.word] > v19)
							{
								mapAngle_13B4E0[v18x.word] &= 0xF7u;
							}
							else
							{
								secondHeightmap_14B4E0[v18x.word] = v19 - 1;
								mapAngle_13B4E0[v18x.word] |= 8u;
							}
							//HIBYTE(v18)--;
							v18x._axis_2d.y--;
							v203++;
						}
						v16++;
						v17x.word++;
					}
				}
				else
				{
					v11 = -1;
					v12x.word = v208x.word - 1;
					while (v11 < a1x->param_0x10_16 + 1)
					{
						v13 = 0;
						v14x.word = v12x.word + 256;
						while (v13 < 4)
						{
							v15x.word = v14x.word;
							v13++;
							//HIBYTE(v14)--;
							v14x._axis_2d.y--;
							mapAngle_13B4E0[v15x.word] &= 0xF7u;
						}
						v11++;
						v12x.word++;
					}
				}
				v20 = -1;
				v21x.word = v208x.word - 1;//12c
				if (CommandLineParams.DoDebugSequences()) {
					//add_compare(0x23b12c, CommandLineParams.DoDebugafterload());
				}
				while (v20 < a1x->param_0x10_16)//23b194
				{
					//LOBYTE(v21)++;
					//HIBYTE(v21)++;
					v21x._axis_2d.x++;
					v21x._axis_2d.y++;
					v22x.word = v21x.word;
					//LOBYTE(v21) -= 2;
					//HIBYTE(v21) -= 2;
					v21x._axis_2d.x -= 2;
					v21x._axis_2d.y -= 2;
					v23x.word = v21x.word;
					//LOBYTE(v21)++;
					v21x._axis_2d.x++;
					v24 = mapHeightmap_11B4E0[v23x.word] - mapHeightmap_11B4E0[v22x.word] + 32;
					//HIBYTE(v25) = HIBYTE(v21) + 1;
					//v25x._axis_2d.y= v21x._axis_2d.y+1;
					v21x._axis_2d.y++;
					if (v24 >= 28)
					{
						if (v24 > 40)
							v24 = (v24 & 7) + 40;
					}
					else
					{
						v24 = (v24 & 3) + 28;
					}
					if (world_D41A0.terrain_2FECE.MapType != MapType_t::Day)
						v26 = 32 - v24 + 32;
					else
						v26 = v24;
					v27x.word = v21x.word;
					v20++;
					v21x.word = v21x.word + 1;
					mapShading_12B4E0[v27x.word] = v26;
				}
				v28 = -3;
				v29x.word = v208x.word - 3;
				while (v28 < a1x->param_0x10_16 + 3)
				{
					v30 = -3;
					v31x.word = v29x.word + 768;
					while (v30 < 6)
					{
						v32x.word = v31x.word;
						v30++;
						//HIBYTE(v31)--;
						v31x._axis_2d.y--;
						mapAngle_13B4E0[v32x.word] |= 0x80u;
					}
					v28++;
					v29x.word++;
				}
				a1x->subSpellIndex_0x2A_42 = 48;
				a1x->life_0x8 = 3;
			}
		}
		return;
	}
	if (v1 <= 1)
	{
		if (a1x->subSpellIndex_0x2A_42 < 0x30u)
		{
			PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 47);
			v207x._axis_2d.x = (unsigned __int16)(a1x->position_0x4C_76.x + 128) >> 8;
			if (a1x->levelPar3_0x46_70)
			{
				v207x._axis_2d.y = ((unsigned __int16)(a1x->position_0x4C_76.y + 128) >> 8) - 1;
			}
			else
			{
				v207x._axis_2d.y = (unsigned __int16)(a1x->position_0x4C_76.y + 128) >> 8;
				v207x.word--;
			}
			if (!a1x->subSpellIndex_0x2A_42)
			{
				v62 = a1x->levelPar3_0x46_70;
				if (v62)
				{
					if (v62 == 1)
					{
						v78x.word = v207x.word;
						v79 = 0;
						while (v79 < 3)
						{
							v80 = 2;
							v81x.word = v78x.word + 512;
							while (v80 < a1x->param_0x10_16 - 3)
							{
								v82x.word = v81x.word;
								v80++;
								//HIBYTE(v81)++;
								v81x._axis_2d.y++;
								mapTerrainType_10B4E0[v82x.word] = 8;
								mapAngle_13B4E0[v82x.word] = 1;
							}
							v79++;
							v78x.word--;
						}
						if (!isCaveLevel_D41B6)
						{
							v83 = 0;
							v84x.word = v207x.word + 1;
							while (v83 < 4)
							{
								v85 = 2;
								v86x.word = v84x.word + 512;
								while (v85 < a1x->param_0x10_16 - 2)
								{
									v87x.word = v86x.word;
									v85++;
									//HIBYTE(v86)++;
									v86x._axis_2d.y++;
									mapAngle_13B4E0[v87x.word] &= 0xF7u;
								}
								v83++;
								v84x.word--;
							}
						}
						v88 = -3;
						v89x.word = v207x.word - 3;
						while (v88 < 6)
						{
							v90 = -3;
							v91x.word = v89x.word - 768;
							while (v90 < a1x->param_0x10_16 + 3)
							{
								v92x.word = v91x.word;
								v90++;
								//HIBYTE(v91)++;
								v91x._axis_2d.y++;
								mapAngle_13B4E0[v92x.word] |= 0x80u;
							}
							v88++;
							v89x.word++;
						}
					}
				}
				else
				{
					v63 = 2;
					v64x.word = v207x.word + 2;
					while (v63 < a1x->param_0x10_16 - 3)
					{
						v65x.word = v64x.word;
						v66 = 0;
						while (v66 < 3)
						{
							v67x.word = v65x.word;
							v66++;
							//HIBYTE(v65)--;
							v65x._axis_2d.y--;
							mapTerrainType_10B4E0[v67x.word] = 8;
							mapAngle_13B4E0[v67x.word] = 1;
						}
						v63++;
						v64x.word++;
					}
					if (!isCaveLevel_D41B6)
					{
						v68 = 2;
						v69x.word = v207x.word + 2;
						while (v68 < a1x->param_0x10_16 - 2)
						{
							v70 = 0;
							v71x.word = v69x.word + 256;
							while (v70 < 4)
							{
								v72x.word = v71x.word;
								v70++;
								//HIBYTE(v71)--;
								v71x._axis_2d.y--;
								mapAngle_13B4E0[v72x.word] &= 0xF7u;
							}
							v68++;
							v69x.word++;
						}
					}
					v73 = -3;
					for (jx.word = v207x.word - 3; v73 < a1x->param_0x10_16 + 3; jx.word++)
					{
						v75 = -3;
						v76x.word = jx.word + 768;
						while (v75 < 6)
						{
							v77x.word = v76x.word;
							v75++;
							//HIBYTE(v76)--;
							v76x._axis_2d.y--;
							mapAngle_13B4E0[v77x.word] |= 0x80u;
						}
						v73++;
					}
				}
			}
			a1x->subSpellIndex_0x2A_42++;
			v93 = a1x->levelPar3_0x46_70;
			if (v93)
			{
				if (v93 == 1)
				{
					v104x.word = v207x.word;
					v103 = 3;
					//BYTE1(v104) += 3;
					v104x._axis_2d.y += 3;
					while (v103 < a1x->param_0x10_16 - 3)
					{
						v105x.word = v104x.word;
						v106 = mapHeightmap_11B4E0[v104x.word] + 1;
						v107x.word = v104x.word - 1;
						mapHeightmap_11B4E0[v105x.word] = v106;
						v108x.word = v107x.word;
						++v103;
						v104x.word = v107x.word + 1;
						//++BYTE1(v104);
						v104x._axis_2d.y++;
						++mapHeightmap_11B4E0[v108x.word];
					}
					if (isCaveLevel_D41B6)
					{
						v110x.word = v207x.word;
						v109 = 3;
						//BYTE1(v110) += 3;
						v110x._axis_2d.y += 3;
						while (v109 < a1x->param_0x10_16 - 3)
						{
							v111 = mapHeightmap_11B4E0[v110x.word];
							if ((unsigned __int8)secondHeightmap_14B4E0[v110x.word] > v111)
							{
								mapAngle_13B4E0[v110x.word] &= 0xF7u;
							}
							else
							{
								secondHeightmap_14B4E0[v110x.word] = v111 - 1;
								mapAngle_13B4E0[v110x.word] |= 8u;
							}
							v112x.word = v110x.word - 1;
							v113 = mapHeightmap_11B4E0[v112x.word];
							if ((unsigned __int8)secondHeightmap_14B4E0[v112x.word] > v113)
							{
								mapAngle_13B4E0[v112x.word] &= 0xF7u;
							}
							else
							{
								secondHeightmap_14B4E0[v112x.word] = v113 - 1;
								mapAngle_13B4E0[v112x.word] |= 8u;
							}
							v110x.word = v112x.word + 1;
							v109++;
							//BYTE1(v110)++;
							v110x._axis_2d.y++;
						}
					}
				}
			}
			else
			{
				v94 = 3;
				v95x.word = v207x.word + 3;
				while (v94 < a1x->param_0x10_16 - 3)
				{
					v96x.word = v95x.word;
					v97 = mapHeightmap_11B4E0[v95x.word] + 1;
					//HIBYTE(v95)--;
					v95x._axis_2d.y--;
					mapHeightmap_11B4E0[v96x.word] = v97;
					v98x.word = v95x.word;
					v94++;
					//HIBYTE(v95)++;
					v95x._axis_2d.y++;
					v95x.word++;
					mapHeightmap_11B4E0[v98x.word]++;
				}
				if (isCaveLevel_D41B6)
				{
					v99 = 3;
					for (kx.word = v207x.word + 3; v99 < a1x->param_0x10_16 - 3; kx.word++)
					{
						v101 = mapHeightmap_11B4E0[kx.word];
						if ((unsigned __int8)secondHeightmap_14B4E0[kx.word] > v101)
						{
							mapAngle_13B4E0[kx.word] &= 0xF7u;
						}
						else
						{
							secondHeightmap_14B4E0[kx.word] = v101 - 1;
							mapAngle_13B4E0[kx.word] |= 8u;
						}
						//HIBYTE(k)--;
						kx._axis_2d.y--;
						v102 = mapHeightmap_11B4E0[kx.word];
						if ((unsigned __int8)secondHeightmap_14B4E0[kx.word] > v102)
						{
							mapAngle_13B4E0[kx.word] &= 0xF7u;
						}
						else
						{
							secondHeightmap_14B4E0[kx.word] = v102 - 1;
							mapAngle_13B4E0[kx.word] |= 8u;
						}
						//HIBYTE(k)++;
						kx._axis_2d.y++;
						v99++;
					}
				}
			}
			if (a1x->subSpellIndex_0x2A_42 >= 0x30u)
			{
				v114 = a1x->levelPar3_0x46_70;
				if (v114)
				{
					if (v114 == 1)
					{
						v123x.word = v207x.word;
						v124 = 0;
						do
						{
							//LOBYTE(v123) = v123 + 1;
							//++HIBYTE(v123);
							v123x._axis_2d.x++;
							v123x._axis_2d.y++;
							v125x.word = v123x.word;
							//LOBYTE(v123) = v123 - 2;
							//HIBYTE(v123) -= 2;
							v123x._axis_2d.x -= 2;
							v123x._axis_2d.y -= 2;
							v126x.word = v123x.word;
							//LOBYTE(v123) = v123 + 1;
							v123x._axis_2d.x++;
							v127 = mapHeightmap_11B4E0[v126x.word] - mapHeightmap_11B4E0[v125x.word] + 32;
							//HIBYTE(v128) = HIBYTE(v123) + 1;
							v128x._axis_2d.y = v123x._axis_2d.y + 1;
							if (v127 >= 28)
							{
								if (v127 > 40)
									v127 = (v127 & 7) + 40;
							}
							else
							{
								v127 = (v127 & 3) + 28;
							}
							if (world_D41A0.terrain_2FECE.MapType != MapType_t::Day)
								v129 = 32 - v127 + 32;
							else
								v129 = v127;
							v130x.word = v128x.word;
							v124++;
							//v123 = v128 + 1;
							v123x.word = v128x.word + 1;
							mapShading_12B4E0[v130x.word] = v129;
						} while (v124 < 3);
					}
				}
				else
				{
					v115 = -1;
					for (lx.word = v207x.word - 1; v115 < a1x->param_0x10_16; mapShading_12B4E0[v122x.word] = v121)
					{
						//LOBYTE(l) = l + 1;
						//++HIBYTE(l);
						lx._axis_2d.x++;
						lx._axis_2d.y++;
						v117x.word = lx.word;
						//LOBYTE(l) = l - 2;
						//HIBYTE(l) -= 2;
						lx._axis_2d.x -= 2;
						lx._axis_2d.y -= 2;
						v118x.word = lx.word;
						//LOBYTE(l) = l + 1;
						lx._axis_2d.x++;
						v119 = mapHeightmap_11B4E0[v118x.word] - mapHeightmap_11B4E0[v117x.word] + 32;
						//HIBYTE(v120) = HIBYTE(l) + 1;
						v120x._axis_2d.y = lx._axis_2d.y + 1;
						if (v119 >= 28)
						{
							if (v119 > 40)
								v119 = (v119 & 7) + 40;
						}
						else
						{
							v119 = (v119 & 3) + 28;
						}
						if (world_D41A0.terrain_2FECE.MapType != MapType_t::Day)
							v121 = 32 - v119 + 32;
						else
							v121 = v119;
						v122x.word = v120x.word;
						v115++;
						lx.word = v120x.word + 1;
					}
				}
			}
			return;
		}
		a1x->life_0x8 = 3;
	LABEL_292:
		EndLoop_6EAB0(a1x - world_D41A0.entities_0x6E8E, -1, 47);
		return;
	}
	if (v1 != 2)
		return;
	if (!a1x->subSpellIndex_0x2A_42)
	{
		a1x->life_0x8 = 4;
		goto LABEL_292;
	}
	PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 47);
	v207x._axis_2d.x = (unsigned __int16)(a1x->position_0x4C_76.x + 128) >> 8;
	if (a1x->levelPar3_0x46_70)
	{
		v207x._axis_2d.y = ((unsigned __int16)(a1x->position_0x4C_76.y + 128) >> 8) - 1;
	}
	else
	{
		v207x._axis_2d.y = (unsigned __int16)(a1x->position_0x4C_76.y + 128) >> 8;
		v207x.word--;
	}
	v132 = a1x->levelPar3_0x46_70;
	if (v132)
	{
		if (v132 == 1)
		{
			v140x.word = v207x.word;
			v139 = 3;
			//BYTE1(v140) += 3;
			v140x._axis_2d.y += 3;
			while (v139 < a1x->param_0x10_16 - 3)
			{
				v141 = mapHeightmap_11B4E0[v140x.word];
				v142 = (mapHeightmap_11B4E0[v140x.word - 2]
					+ mapHeightmap_11B4E0[v140x.word + 1]) >> 1;
				if (v142 < v141)
					mapHeightmap_11B4E0[v140x.word] = v141 - 1;
				v143x.word = v140x.word - 1;
				v144 = mapHeightmap_11B4E0[v143x.word];
				if (v142 < v144)
					mapHeightmap_11B4E0[v143x.word] = v144 - 1;
				v140x.word = v143x.word + 1;
				++v139;
				//++BYTE1(v140);
				v140x._axis_2d.y++;
			}
		}
	}
	else
	{
		v133 = 3;
		for (mx.word = v207x.word + 3; v133 < a1x->param_0x10_16 - 3; ++mx.word)
		{
			v135x.word = mx.word + 256;
			//HIBYTE(v135) = HIBYTE(m) - 2;
			v135x._axis_2d.y = mx._axis_2d.y - 2;
			v136 = mapHeightmap_11B4E0[mx.word];
			v137 = (mapHeightmap_11B4E0[v135x.word] + mapHeightmap_11B4E0[mx.word + 256]) >> 1;
			if (v137 < v136)
				mapHeightmap_11B4E0[mx.word] = v136 - 1;
			//--HIBYTE(m);
			mx._axis_2d.y--;
			v138 = mapHeightmap_11B4E0[mx.word];
			if (v137 < v138)
				mapHeightmap_11B4E0[mx.word] = v138 - 1;
			//++HIBYTE(m);
			mx._axis_2d.y++;
			++v133;
		}
	}
	v145 = a1x->subSpellIndex_0x2A_42 - 1;
	a1x->subSpellIndex_0x2A_42 = v145;
	if (!v145)
	{
		v146 = a1x->levelPar3_0x46_70;
		if (v146)
		{
			if (v146 != 1)
				return;
			v174x.word = v207x.word;
			v173 = 3;
			//BYTE1(v174) += 3;
			v174x._axis_2d.y += 3;
			while (v173 < a1x->param_0x10_16 - 4)
			{
				v175x.word = v174x.word + 2;
				if (mapTerrainType_10B4E0[v174x.word + 2] == 8)
					v175x._axis_2d.y += a1x->param_0x10_16 >> 1;
				v176x.word = v174x.word + 1;
				v177x.word = v176x.word;
				mapTerrainType_10B4E0[v176x.word] = mapTerrainType_10B4E0[v175x.word];
				mapAngle_13B4E0[v176x.word] = mapAngle_13B4E0[v175x.word];
				mapShading_12B4E0[v176x.word--] = 32;
				mapAngle_13B4E0[v177x.word] |= 0x80u;
				v178x.word = v176x.word;
				mapTerrainType_10B4E0[v176x.word] = mapTerrainType_10B4E0[v175x.word];
				mapAngle_13B4E0[v176x.word] = mapAngle_13B4E0[v175x.word];
				mapShading_12B4E0[v176x.word--] = 32;
				mapAngle_13B4E0[v178x.word] |= 0x80u;
				v179x.word = v176x.word;
				mapTerrainType_10B4E0[v176x.word] = mapTerrainType_10B4E0[v175x.word];
				mapAngle_13B4E0[v176x.word] = mapAngle_13B4E0[v175x.word];
				mapShading_12B4E0[v176x.word] = 32;
				v180 = mapAngle_13B4E0[v176x.word--] | 0x80;
				mapAngle_13B4E0[v179x.word] = v180;
				v181 = mapTerrainType_10B4E0[v175x.word];
				v182x.word = v176x.word;
				++v173;
				mapAngle_13B4E0[v176x.word] = mapAngle_13B4E0[v175x.word];
				//v174 = v176 + 2;
				v174x.word = v176x.word + 2;
				mapShading_12B4E0[v182x.word] = 32;
				v183 = mapAngle_13B4E0[v182x.word];
				mapTerrainType_10B4E0[v182x.word] = v181;
				//++BYTE1(v174);
				v174x._axis_2d.y++;
				mapAngle_13B4E0[v182x.word] = v183 | 0x80;
			}
			if (isCaveLevel_D41B6)
			{
				v185x.word = v207x.word;
				v184 = 3;
				//BYTE1(v185) += 3;
				v185x._axis_2d.y += 3;
				if (a1x->param_0x10_16 - 4 > 3)
				{
					do
					{
						v186x.word = v185x.word + 1;
						v187 = mapHeightmap_11B4E0[v186x.word];
						if ((unsigned __int8)secondHeightmap_14B4E0[v186x.word] > v187)
						{
							mapAngle_13B4E0[v186x.word] &= 0xF7u;
						}
						else
						{
							secondHeightmap_14B4E0[v186x.word] = v187 - 1;
							mapAngle_13B4E0[v186x.word] |= 8u;
						}
						v188x.word = v186x.word - 1;
						v189 = mapHeightmap_11B4E0[v188x.word];
						if ((unsigned __int8)secondHeightmap_14B4E0[v188x.word] > v189)
						{
							mapAngle_13B4E0[v188x.word] &= 0xF7u;
						}
						else
						{
							secondHeightmap_14B4E0[v188x.word] = v189 - 1;
							mapAngle_13B4E0[v188x.word] |= 8u;
						}
						v190x.word = v188x.word - 1;
						v191 = mapHeightmap_11B4E0[v190x.word];
						if ((unsigned __int8)secondHeightmap_14B4E0[v190x.word] > v191)
						{
							mapAngle_13B4E0[v190x.word] &= 0xF7u;
						}
						else
						{
							secondHeightmap_14B4E0[v190x.word] = v191 - 1;
							mapAngle_13B4E0[v190x.word] |= 8u;
						}
						v192x.word = v190x.word - 1;
						v193 = mapHeightmap_11B4E0[v192x.word];
						if ((unsigned __int8)secondHeightmap_14B4E0[v192x.word] > v193)
						{
							mapAngle_13B4E0[v192x.word] &= 0xF7u;
						}
						else
						{
							secondHeightmap_14B4E0[v192x.word] = v193 - 1;
							mapAngle_13B4E0[v192x.word] |= 8u;
						}
						v185x.word = v192x.word + 2;
						++v184;
						//++BYTE1(v185);
						v185x._axis_2d.y++;
					} while (v184 < a1x->param_0x10_16 - 4);
					v194 = -3;
					v195x.word = v207x.word - 3;
					goto LABEL_286;
				}
			}
			else
			{
				v196x.word = v207x.word + 768;
				mapAngle_13B4E0[v196x.word + 1] &= 0xF7u;
				mapAngle_13B4E0[v196x.word] &= 0xF7u;
				v197x.word = v207x.word + 767;
				mapAngle_13B4E0[v197x.word] = mapAngle_13B4E0[v207x.word + 767] & 0xF7;
				mapAngle_13B4E0[v197x.word - 1] &= 0xF7u;
			}
			v194 = -3;
			v195x.word = v207x.word - 3;
			do
			{
			LABEL_286:
				v198 = -3;
				v199x.word = v195x.word - 768;
				while (v198 < a1x->param_0x10_16 + 3)
				{
					v200x.word = v199x.word;
					++v198;
					//++HIBYTE(v199);
					v199x._axis_2d.y++;
					mapAngle_13B4E0[v200x.word] |= 0x80u;
				}
				++v194;
				v195x.word++;//0x23C01D inc ebx
			} while (v194 < 6);
			return;
		}
		v147 = 3;
		v148x.word = v207x.word + 3;
		while (v147 < a1x->param_0x10_16 - 4)
		{
			v149x.word = v148x.word + 512;
			if (mapTerrainType_10B4E0[v148x.word + 512] == 8)
				v149x.word += a1x->param_0x10_16 >> 1;
			//++HIBYTE(v148);
			v148x._axis_2d.y++;
			v150x.word = v148x.word;
			mapTerrainType_10B4E0[v148x.word] = mapTerrainType_10B4E0[v149x.word];
			mapAngle_13B4E0[v148x.word] = mapAngle_13B4E0[v149x.word];
			mapShading_12B4E0[v148x.word] = 32;
			//--HIBYTE(v148);
			v148x._axis_2d.y--;
			mapAngle_13B4E0[v150x.word] |= 0x80u;
			v151x.word = v148x.word;
			mapTerrainType_10B4E0[v148x.word] = mapTerrainType_10B4E0[v149x.word];
			mapAngle_13B4E0[v148x.word] = mapAngle_13B4E0[v149x.word];
			mapShading_12B4E0[v148x.word] = 32;
			//--HIBYTE(v148);
			v148x._axis_2d.y--;
			mapAngle_13B4E0[v151x.word] |= 0x80u;
			v152x.word = v148x.word;
			mapTerrainType_10B4E0[v148x.word] = mapTerrainType_10B4E0[v149x.word];
			mapAngle_13B4E0[v148x.word] = mapAngle_13B4E0[v149x.word];
			mapShading_12B4E0[v148x.word] = 32;
			v153 = mapAngle_13B4E0[v148x.word] | 0x80;
			//--HIBYTE(v148);
			v148x._axis_2d.y--;
			mapAngle_13B4E0[v152x.word] = v153;
			v154 = mapTerrainType_10B4E0[v149x.word];
			v155x.word = v148x.word;
			++v147;
			mapAngle_13B4E0[v148x.word] = mapAngle_13B4E0[v149x.word];
			//HIBYTE(v148) += 2;
			v148x._axis_2d.y += 2;
			mapShading_12B4E0[v155x.word] = 32;
			v156 = mapAngle_13B4E0[v155x.word];
			mapTerrainType_10B4E0[v155x.word] = v154;
			++v148x.word;
			mapAngle_13B4E0[v155x.word] = v156 | 0x80;
		}
		if (isCaveLevel_D41B6)
		{
			v157 = 3;
			v158x.word = v207x.word + 3;
			if (a1x->param_0x10_16 - 4 > 3)
			{
				do
				{
					//++HIBYTE(v158);
					v158x._axis_2d.y++;
					v159 = mapHeightmap_11B4E0[v158x.word];
					if ((unsigned __int8)secondHeightmap_14B4E0[v158x.word] > v159)
					{
						mapAngle_13B4E0[v158x.word] &= 0xF7u;
					}
					else
					{
						secondHeightmap_14B4E0[v158x.word] = v159 - 1;
						mapAngle_13B4E0[v158x.word] |= 8u;
					}
					//--HIBYTE(v158);
					v158x._axis_2d.y--;
					v160 = mapHeightmap_11B4E0[v158x.word];
					if ((unsigned __int8)secondHeightmap_14B4E0[v158x.word] > v160)
					{
						mapAngle_13B4E0[v158x.word] &= 0xF7u;
					}
					else
					{
						secondHeightmap_14B4E0[v158x.word] = v160 - 1;
						mapAngle_13B4E0[v158x.word] |= 8u;
					}
					//--HIBYTE(v158);
					v158x._axis_2d.y--;
					v161 = mapHeightmap_11B4E0[v158x.word];
					if ((unsigned __int8)secondHeightmap_14B4E0[v158x.word] > v161)
					{
						mapAngle_13B4E0[v158x.word] &= 0xF7u;
					}
					else
					{
						secondHeightmap_14B4E0[v158x.word] = v161 - 1;
						mapAngle_13B4E0[v158x.word] |= 8u;
					}
					//--HIBYTE(v158);
					v158x._axis_2d.y--;
					v162 = mapHeightmap_11B4E0[v158x.word];
					if ((unsigned __int8)secondHeightmap_14B4E0[v158x.word] > v162)
					{
						mapAngle_13B4E0[v158x.word] &= 0xF7u;
					}
					else
					{
						secondHeightmap_14B4E0[v158x.word] = v162 - 1;
						mapAngle_13B4E0[v158x.word] |= 8u;
					}
					//HIBYTE(v158) += 2;
					v158x._axis_2d.y += 2;
					++v157;
					++v158x.word;
				} while (v157 < a1x->param_0x10_16 - 4);
				v163 = -3;
				v164x.word = v207x.word - 3;
				goto LABEL_261;
			}
		}
		else
		{
			v165x.word = v207x.word + 259;
			v165x._axis_2d.y = (unsigned __int16)(v207x.word + 3) >> 8;
			mapAngle_13B4E0[v207x.word + 259] &= 0xF7u;
			v166x.word = v165x.word;
			v167 = mapAngle_13B4E0[v165x.word] & 0xF7;
			//--HIBYTE(v165);
			v165x._axis_2d.y--;
			mapAngle_13B4E0[v166x.word] = v167;
			v168x.word = v165x.word;
			v169 = mapAngle_13B4E0[v165x.word] & 0xF7;
			//--HIBYTE(v165);
			v165x._axis_2d.y--;
			mapAngle_13B4E0[v168x.word] = v169;
			mapAngle_13B4E0[v165x.word] &= 0xF7u;
		}
		v163 = -3;
		v164x.word = v207x.word - 3;
	LABEL_261:
		while (v163 < a1x->param_0x10_16 + 3)
		{
			v170 = -3;
			v171x.word = v164x.word + 768;
			while (v170 < 6)
			{
				v172x.word = v171x.word;
				++v170;
				//--HIBYTE(v171);
				v171x._axis_2d.y--;
				mapAngle_13B4E0[v172x.word] |= 0x80u;
			}
			++v163;
			++v164x.word;
		}
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41B6: using guessed type char x_BYTE_D41B6;

//----- (0005B100) --------------------------------------------------------
void UpdateCastleBuilding_5B100(type_entity_0x6E8E* locEvent)//23c100
{
	uaxis_2d eventPoint;
	uaxis_2d koefPoint;
	uaxis_2d indexPoint1;
	uaxis_2d indexPoint2;
	uaxis_2d indexPoint3;
	uaxis_2d indexPoint4;
	uaxis_2d indexPoint5;
	int koefZ1;
	int koefZ2;
	int koefZ3;
	int koefZ4;
	int koefZ5;
	int koefX;
	int koefY;

	if (locEvent->life_0x8 <= 2u)
	{
		int locEventZ = locEvent->position_0x4C_76.z;
		int locKoef2 = (2 * ((locEvent->targetId_0x96_150 << 8) + 512) + 128) >> 8;
		if (locEvent->param2_0x2C_44)
		{
			koefX = locKoef2;
			koefY = 2;
		}
		else
		{
			koefY = locKoef2;
			koefX = 2;
		}
		eventPoint._axis_2d.x = locEvent->position_0x4C_76.x >> 8;
		eventPoint._axis_2d.y = locEvent->position_0x4C_76.y >> 8;
		koefPoint._axis_2d.x = eventPoint._axis_2d.x - (koefX >> 1);
		koefPoint._axis_2d.y = eventPoint._axis_2d.y - (koefY >> 1);
		int signLocKoef2 = (8 * locKoef2 - (my_sign32(8 * locKoef2) * 31)) >> 5;
		switch (locEvent->life_0x8)
		{
		case 0:
		{
			locEvent->life_0x8 = 4;
			int mapHeight1 = -1;
			int mapHeight2 = -1;
			if (locEvent->param2_0x2C_44)
			{
				bool finded1 = false;
				indexPoint1 = eventPoint;
				while (!finded1)
				{
					if ((mapAngle_13B4E0[indexPoint1.word] & 8) != 0)
					{
						finded1 = true;
						mapHeight1 = mapHeightmap_11B4E0[indexPoint1.word];
					}
					indexPoint1._axis_2d.x--;
				}
				indexPoint1 = eventPoint;
				finded1 = false;
				while (!finded1)
				{
					if ((mapAngle_13B4E0[indexPoint1.word] & 8) != 0)
					{
						mapHeight2 = mapHeightmap_11B4E0[indexPoint1.word];
						finded1 = true;
					}
					indexPoint1._axis_2d.x++;
				}
			}
			else
			{
				indexPoint1 = eventPoint;
				bool finded1 = false;
				while (!finded1)
				{
					if ((mapAngle_13B4E0[indexPoint1.word] & 8) != 0)
					{
						finded1 = true;
						mapHeight1 = mapHeightmap_11B4E0[indexPoint1.word];
					}
					indexPoint1._axis_2d.y--;
				}
				finded1 = false;
				indexPoint1 = eventPoint;
				while (!finded1)
				{
					if ((mapAngle_13B4E0[indexPoint1.word] & 8) != 0)
					{
						mapHeight2 = mapHeightmap_11B4E0[indexPoint1.word];
						finded1 = true;
					}
					indexPoint1._axis_2d.y++;
				}
			}
			if (mapHeight1 == -1 || mapHeight2 == -1)
			{
				ReleaseEntity_57F20(locEvent);
			}
			else
			{
				locEvent->position_0x4C_76.z = (mapHeight2 + mapHeight1) >> 1;
				indexPoint1 = koefPoint;
				for (int i = 0; i < koefY; i++)
				{
					indexPoint2 = indexPoint1;
					for (int j = 0; j < koefX; j++)
					{
						mapAngle_13B4E0[indexPoint2.word] |= 0x80u;
						indexPoint2._axis_2d.x++;
					}
					indexPoint1._axis_2d.y++;
				}
			}
			break;
		}
		case 1:
		{
			PrepareEventSound_6E450(locEvent - world_D41A0.entities_0x6E8E, -1, 47);
			koefZ1 = signLocKoef2 + mapHeightmap_11B4E0[eventPoint.word] + 1;
			koefZ2 = secondHeightmap_14B4E0[eventPoint.word] - signLocKoef2 - 1;
			if (koefZ1 < 0)
				koefZ1 = 0;
			if (koefZ1 > locEventZ)
				koefZ1 = locEventZ;
			if (locEventZ > koefZ2)
				koefZ2 = locEventZ;
			if (koefZ2 > 254)
				koefZ2 = 254;
			bool finded1 = true;
			indexPoint1 = koefPoint;
			for (int i = 0; i < koefY; i++)
			{
				indexPoint2 = indexPoint1;
				for (int j = 0; j < koefX; j++)
				{
					if ((mapAngle_13B4E0[indexPoint2.word] & 8) == 0)
					{
						koefZ5 = signLocKoef2 + mapHeightmap_11B4E0[indexPoint2.word];
						if (koefZ5 > koefZ1)
							koefZ5 = koefZ1;
						if (koefZ5 > mapHeightmap_11B4E0[indexPoint2.word])
						{
							mapHeightmap_11B4E0[indexPoint2.word] = koefZ5;
							finded1 = false;
						}
						koefZ3 = secondHeightmap_14B4E0[indexPoint2.word] - signLocKoef2;
						if (koefZ3 < koefZ2)
							koefZ3 = koefZ2;
						if (koefZ3 < secondHeightmap_14B4E0[indexPoint2.word])
						{
							secondHeightmap_14B4E0[indexPoint2.word] = koefZ3;
							finded1 = false;
						}
					}
					if (secondHeightmap_14B4E0[indexPoint2.word] > mapHeightmap_11B4E0[indexPoint2.word])
						mapAngle_13B4E0[indexPoint2.word] &= 0xF7u;
					else
						mapAngle_13B4E0[indexPoint2.word] |= 8u;
					indexPoint2._axis_2d.x++;
				}
				indexPoint1._axis_2d.y++;
			}
			if (finded1)
			{
				locEvent->life_0x8 = 3;
				EndLoop_6EAB0(locEvent - world_D41A0.entities_0x6E8E, -1, 47);
			}
			break;
		}
		case 2:
		{
			PrepareEventSound_6E450(locEvent - world_D41A0.entities_0x6E8E, -1, 47);
			bool finded1 = true;
			indexPoint1 = koefPoint;
			for (int i = 0; i < koefY; i++)
			{
				indexPoint2 = indexPoint1;
				for (int j = 0; j < koefX; j++)
				{
					if (locEvent->param2_0x2C_44)
					{
						indexPoint3._axis_2d.x = indexPoint2._axis_2d.x;
						indexPoint4._axis_2d.x = indexPoint2._axis_2d.x;
						indexPoint5._axis_2d.x = indexPoint2._axis_2d.x;
						indexPoint3._axis_2d.y = koefPoint._axis_2d.y - 1;
						indexPoint4._axis_2d.y = koefPoint._axis_2d.y + 2;
						indexPoint5._axis_2d.y = koefPoint._axis_2d.y + 3;
					}
					else
					{
						indexPoint3._axis_2d.y = indexPoint1._axis_2d.y;
						indexPoint4._axis_2d.y = indexPoint1._axis_2d.y;
						indexPoint5._axis_2d.y = indexPoint1._axis_2d.y;
						indexPoint3._axis_2d.x = koefPoint._axis_2d.x - 1;
						indexPoint4._axis_2d.x = koefPoint._axis_2d.x + 2;
						indexPoint5._axis_2d.x = koefPoint._axis_2d.x + 3;
					}
					if (abs(mapHeightmap_11B4E0[indexPoint3.word] - mapHeightmap_11B4E0[indexPoint4.word]) <= 4)
					{
						koefZ1 = (mapHeightmap_11B4E0[indexPoint3.word] + mapHeightmap_11B4E0[indexPoint4.word]) / 2;
					}
					else
					{
						if (abs(mapHeightmap_11B4E0[indexPoint3.word] - mapHeightmap_11B4E0[indexPoint5.word]) > 4)
							koefZ1 = mapHeightmap_11B4E0[indexPoint4.word];
						else
							koefZ1 = mapHeightmap_11B4E0[indexPoint3.word];
					}
					koefZ2 = mapHeightmap_11B4E0[indexPoint2.word] - signLocKoef2;
					if (koefZ2 < koefZ1)
						koefZ2 = koefZ1;
					if (koefZ2 < mapHeightmap_11B4E0[indexPoint2.word])
					{
						mapHeightmap_11B4E0[indexPoint2.word] = koefZ2;
						finded1 = false;
					}
					if (abs(secondHeightmap_14B4E0[indexPoint3.word] - secondHeightmap_14B4E0[indexPoint4.word]) <= 4)
					{
						koefZ3 = (secondHeightmap_14B4E0[indexPoint3.word] + secondHeightmap_14B4E0[indexPoint4.word]) / 2;
					}
					else
					{
						if (abs(secondHeightmap_14B4E0[indexPoint3.word] - secondHeightmap_14B4E0[indexPoint5.word]) > 4)
							koefZ3 = secondHeightmap_14B4E0[indexPoint4.word];
						else
							koefZ3 = secondHeightmap_14B4E0[indexPoint3.word];
					}
					koefZ4 = signLocKoef2 + secondHeightmap_14B4E0[indexPoint2.word];
					if (koefZ4 > koefZ3)
						koefZ4 = koefZ3;
					if (secondHeightmap_14B4E0[indexPoint2.word] < koefZ4)
					{
						secondHeightmap_14B4E0[indexPoint2.word] = koefZ4;
						finded1 = false;
					}
					if (secondHeightmap_14B4E0[indexPoint2.word] > mapHeightmap_11B4E0[indexPoint2.word])
						mapAngle_13B4E0[indexPoint2.word] &= 0xF7u;
					else
						mapAngle_13B4E0[indexPoint2.word] |= 8u;
					indexPoint2._axis_2d.x++;
				}
				indexPoint1._axis_2d.y++;
			}
			if (finded1)
			{
				locEvent->life_0x8 = 4;
				EndLoop_6EAB0(locEvent - world_D41A0.entities_0x6E8E, -1, 47);
			}
			break;
		}
		}
	}
}

//----- (0005EFA0) --------------------------------------------------------
signed int UpdateCastleTarget_5EFA0(type_entity_0x6E8E* a1x)//23ffa0
{
	signed int v1; // esi
	type_entity_0x6E8E* v2x; // eax
	type_entity_0x6E8E* v3x; // edi
	//int v4; // eax
	//int v5; // eax
	//int v6; // eax
	//int v7; // eax
	__int16 v8; // ax
	char v9; // ch
	int v10; // eax
	char v11; // dl
	int v12; // edi
	char v13; // dh
	unsigned __int16 v14; // di
	type_entity_0x6E8E* v15x; // edi
	//int v16; // eax
	//int v17; // eax

	a1x->hitById_0x26_38 = 0;
	v1 = 0;
	if (a1x->targetId_0x96_150)
	{
		v2x = Entities_EA3E4[a1x->targetId_0x96_150];
		if (v2x > Entities_EA3E4[0] && (v2x->life_0x8 <= 0 || v2x->flags_0xc_12_15.byte[1] & 4))
			a1x->targetId_0x96_150 = 0;
	}
	if (a1x->life_0x8 >= 0)
	{
		if (a1x->mail_0x5E_94.channel[4].source)
		{
			v3x = Entities_EA3E4[a1x->mail_0x5E_94.channel[4].source];
			if (v3x)
			{
				v3x->playerData_0xA4_164x->mountId_0x146_326 = a1x - world_D41A0.entities_0x6E8E;
				v3x->playerData_0xA4_164x->targetDistance_0x142_322 = Maths::Distance3D_583F0(&a1x->position_0x4C_76, &v3x->position_0x4C_76);
				v3x->playerData_0xA4_164x->duelLevel_0x14A_330 = a1x->mail_0x5E_94.channel[4].amount.word[0];
				//v4 = v3x->dword_0xA4_164;
				if (v3x->playerData_0xA4_164x->targetDistance_0x142_322 < 1024)
					v3x->playerData_0xA4_164x->targetDistance_0x142_322 = 1024;
				//v5 = v3x->dword_0xA4_164;
				if (v3x->playerData_0xA4_164x->targetDistance_0x142_322 > 3072)
					v3x->playerData_0xA4_164x->targetDistance_0x142_322 = 3072;
				SendSpellMessage_6D8B0(a1x->mail_0x5E_94.channel[4].source, 0xEu, 1);
			}
			//v6 = a1x->dword_0xA4_164;
			a1x->mail_0x5E_94.channel[4].source = 0;
			a1x->playerData_0xA4_164x->PlayerHitFrameTime_406 = 4;
			a1x->playerData_0xA4_164x->dword_0x18D_397 = 16;
			a1x->playerData_0xA4_164x->word_0x24C_588 = 64;
			SetWizardHitTimer_5EF70(a1x);
		}
		if (a1x->mail_0x5E_94.channel[3].source)
			UpdateBalloonCollection_61050(a1x);
		//v7 = a1x->dword_0xA4_164;
		if (a1x->playerData_0xA4_164x->moveSpeed_0x14C_332 && gameState_D41A4.moveSpeedFlag_181)
			SetPaletteModification_5C830(a1x, 3, 171 * a1x->playerData_0xA4_164x->moveSpeed_0x14C_332 / 3 + 85);
		v8 = a1x->mail_0x5E_94.channel[0].source;
		if (v8)
		{
			v9 = a1x->flags_0xc_12_15.byte[1];
			a1x->hitById_0x26_38 = v8;
			if (v9 & 0x40 || a1x->flags_0xc_12_15.byte[2] & 0x40)
			{
				SendSpellMessage_6D8B0(a1x - world_D41A0.entities_0x6E8E, 6u, 1);
				if (a1x->flags_0xc_12_15.byte[1] & 0x40)
				{
					v10 = (a1x->mail_0x5E_94.channel[0].amount.dword - (__CFSHL__(a1x->mail_0x5E_94.channel[0].amount.dword >> 31, 2) + 4 * (a1x->mail_0x5E_94.channel[0].amount.dword >> 31))) >> 2;
					v11 = a1x->flags_0xc_12_15.byte[1];
					v12 = a1x->mana_0x90_144 - v10;
					a1x->mail_0x5E_94.channel[0].amount.dword = v10;
					a1x->mana_0x90_144 = v12;
					a1x->flags_0xc_12_15.byte[1] = v11 & 0xBF;
				}
				else
				{
					a1x->flags_0xc_12_15.dword &= 0xFFBFBFFF;
					v13 = a1x->flags_0xc_12_15.byte[1];
					a1x->mail_0x5E_94.channel[0].amount.dword = 0;
					a1x->flags_0xc_12_15.byte[1] = v13 | 0x40;
				}
			}
			v14 = a1x->mail_0x5E_94.channel[0].source;
			a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
			v15x = Entities_EA3E4[v14];
			a1x->playerData_0xA4_164x->yaw_0x1E_30 = Maths::GetYawAngle_581E0(&v15x->position_0x4C_76, &a1x->position_0x4C_76);
			a1x->playerData_0xA4_164x->fov_0x22_34 = Maths::GetPitchAngle_58210(&v15x->position_0x4C_76, &a1x->position_0x4C_76);
			a1x->playerData_0xA4_164x->moveBoost_0x1E_30 = a1x->mail_0x5E_94.channel[0].amount.dword / 10;
			//v16 = a1x->dword_0xA4_164;
			if (a1x->playerData_0xA4_164x->moveBoost_0x1E_30 < 0)
				a1x->playerData_0xA4_164x->moveBoost_0x1E_30 = 0;
			//v17 = a1x->dword_0xA4_164;
			if (a1x->playerData_0xA4_164x->moveBoost_0x1E_30 > 80)
				a1x->playerData_0xA4_164x->moveBoost_0x1E_30 = 80;
			SetLocalPaletteSubMode_5C800(a1x, 2);
			a1x->playerData_0xA4_164x->PlayerHitFrameTime_406 = 4;
			a1x->playerData_0xA4_164x->dword_0x18D_397 = 16;
			a1x->playerData_0xA4_164x->word_0x24C_588 = 64;
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, (a1x->rand_0x14_20 & 3) + 54);
			if (a1x->life_0x8 < 0)
			{
				a1x->ownerPlayerId_0x24_36 = a1x->mail_0x5E_94.channel[0].source;
				if (v15x->class_0x3F_63 == 10 && v15x->model_0x40_64 == 67)
					a1x->ownerPlayerId_0x24_36 = 0;
				v1 = 2;
			}
			if (v1 != 2)
			{
				v1 = 1;
				SetWizardHitTimer_5EF70(a1x);
				a1x->mail_0x5E_94.channel[0].source = 0;
			}
		}
	}
	else
	{
		v1 = 2;
	}
	if (gameState_D41A4.setting_byte4_25 & 1 && !a1x->model_0x40_64)
	{
		a1x->hitById_0x26_38 = 0;
		a1x->ownerPlayerId_0x24_36 = 0;
		a1x->mail_0x5E_94.channel[0].source = 0;
		a1x->life_0x8 = 10000;
		v1 = 0;
	}
	return v1;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0005F380) --------------------------------------------------------
void UpdateWizardSpeed_5F380(type_entity_0x6E8E* a1x)//240380
{
	signed __int16 v1; // ax
	//int v2; // edx
	//int v3; // edx
	//int v4; // eax
	//int v5; // eax
	signed __int16 v6; // ax
	__int16 v7; // di
	signed __int16 v8; // ax
	signed __int16 v9; // dx
	__int16 v10; // di
	signed int v11; // eax
	//int v12; // eax
	//int v13; // eax
	int v14; // edx
	bool v15; // zf
	bool v16; // sf
	unsigned __int8 v17; // of
	//int v18; // eax
	int v19; // edx
	//int v20; // eax
	//int v21; // eax
	//x_BYTE *v22; // eax

	v1 = 0;
	a1x->playerData_0xA4_164x->castLock_0xe_14 = 0;
	//v2 = a1x->dword_0xA4_164;
	if (a1x->playerData_0xA4_164x->entityIndex_0x0 & 1 && a1x->playerData_0xA4_164x->speed_0xc_12 < wizardSpeed2_D4B8C)
		v1 = 1;
	//v3 = a1x->dword_0xA4_164;
	if (a1x->playerData_0xA4_164x->entityIndex_0x0 & 2 && a1x->playerData_0xA4_164x->speed_0xc_12 > wizardSpeed1_D4B88)
		v1 = -1;
	if (v1)
	{
		a1x->playerData_0xA4_164x->speed_0xc_12 += speedIcrement_D4B84 * v1;
		//v4 = a1x->dword_0xA4_164;
		if (a1x->playerData_0xA4_164x->speed_0xc_12 < wizardSpeed1_D4B88)
			a1x->playerData_0xA4_164x->speed_0xc_12 = wizardSpeed1_D4B88;
		//v5 = a1x->dword_0xA4_164;
		if (a1x->playerData_0xA4_164x->speed_0xc_12 > wizardSpeed2_D4B8C)
			a1x->playerData_0xA4_164x->speed_0xc_12 = wizardSpeed2_D4B8C;
		a1x->playerData_0xA4_164x->castLock_0xe_14 = 1;
	}
	v6 = 0;
	if (a1x->playerData_0xA4_164x->entityIndex_0x0 & 4)
		v6 = -1;
	if (a1x->playerData_0xA4_164x->entityIndex_0x0 & 8)
		v6 = 1;
	if (v6)
	{
		a1x->playerData_0xA4_164x->strafeSpeed_0x10_16 += strafeAccel1_D4BA4 * v6;
		//v18 = a1x->dword_0xA4_164;
		if (a1x->playerData_0xA4_164x->strafeSpeed_0x10_16 < wizardMinStrafe_D4BA8)
			a1x->playerData_0xA4_164x->strafeSpeed_0x10_16 = wizardMinStrafe_D4BA8;
		//v13 = a1x->dword_0xA4_164;
		v19 = a1x->playerData_0xA4_164x->strafeSpeed_0x10_16;
		v17 = __OFSUB__(v19, wizardMaxStrafe_D4BAC);
		v15 = v19 == wizardMaxStrafe_D4BAC;
		v16 = v19 - wizardMaxStrafe_D4BAC < 0;
	}
	else
	{
		v7 = a1x->playerData_0xA4_164x->strafeSpeed_0x10_16;
		if (v7)
		{
			if (v7 <= 0)
				v8 = -1;
			else
				v8 = 1;
		}
		else
		{
			v8 = 0;
		}
		v9 = v8;
		a1x->playerData_0xA4_164x->strafeSpeed_0x10_16 += v8 * strafeAccel2_D4BB0;
		v10 = a1x->playerData_0xA4_164x->strafeSpeed_0x10_16;
		if (v10)
		{
			if (v10 <= 0)
				v11 = -1;
			else
				v11 = 1;
		}
		else
		{
			v11 = 0;
		}
		if (v9 != v11)
			a1x->playerData_0xA4_164x->strafeSpeed_0x10_16 = 0;
		//v12 = a1x->dword_0xA4_164;
		if (a1x->playerData_0xA4_164x->strafeSpeed_0x10_16 < wizardMinStrafe_D4BA8)
			a1x->playerData_0xA4_164x->strafeSpeed_0x10_16 = wizardMinStrafe_D4BA8;
		//v13 = a1x->dword_0xA4_164;
		v14 = a1x->playerData_0xA4_164x->strafeSpeed_0x10_16;
		v17 = __OFSUB__(v14, wizardMaxStrafe_D4BAC);
		v15 = v14 == wizardMaxStrafe_D4BAC;
		v16 = v14 - wizardMaxStrafe_D4BAC < 0;
	}
	if (!((unsigned __int8)(v16 ^ v17) | (unsigned __int8)v15))
		a1x->playerData_0xA4_164x->strafeSpeed_0x10_16 = wizardMaxStrafe_D4BAC;
	//v20 = a1x->dword_0xA4_164;
	if (a1x->playerData_0xA4_164x->entityIndex_0x0 & 0x10)
		CastleAttackEntity_5F660(a1x, Entities_EA3E4[a1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[a1x->playerData_0xA4_164x->str_611.SpellIndexLeft_0x451_1105]], 256);
	//v21 = a1x->dword_0xA4_164;
	if (a1x->playerData_0xA4_164x->entityIndex_0x0 & 0x20)
		CastleAttackEntity_5F660(a1x, Entities_EA3E4[a1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[a1x->playerData_0xA4_164x->str_611.SpellIndexRight_0x453_1107]], 512);
	//v22 = *(x_BYTE**)&a1x->dword_0xA4_164;
	if (a1x->playerData_0xA4_164x->entityIndex_0x0 & 0x40)
		/*LOBYTE(v22) = */
		CastleAttackEntity_5F660(
			a1x,
			Entities_EA3E4[a1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[spellIndex_D94FF[a1x->playerData_0xA4_164x->str_611.spellIndex_0x458_1112]]],
			256);
	//return (char)v22;
}
// D4B84: using guessed type int x_DWORD_D4B84;
// D4B88: using guessed type int wizardSpeed1_D4B88;
// D4B8C: using guessed type int wizardSpeed2_D4B8C;
// D4BA4: using guessed type __int16 strafeAccel1_D4BA4;
// D4BA8: using guessed type int wizardMinStrafe_D4BA8;
// D4BAC: using guessed type int wizardMaxStrafe_D4BAC;
// D4BB0: using guessed type __int16 strafeAccel2_D4BB0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0005F660) --------------------------------------------------------
char CastleAttackEntity_5F660(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x, int a3)//240660
{
	int v3; // ecx
	char v5; // [esp+0h] [ebp-Ch]
	char v6; // [esp+4h] [ebp-8h]
	char v7; // [esp+8h] [ebp-4h]

	v3 = a3;
	v5 = 0;
	v7 = 0;
	v6 = 0;
	if (a2x > Entities_EA3E4[0])
	{
		if (a1x->model_0x40_64 == 1)
		{
			v5 = 1;
			v3 = 0;
		}
		switch (a2x->model_0x40_64)
		{
		case 0:
			if (a2x->levelPar3_0x46_70 < 2)
				break;
			goto LABEL_16;
		case 1:
			if (a2x->stage_0x2E_46 <= 0)
				break;
			a2x->counter_0x3C_60 = 1;
			a1x->flags_0xc_12_15.byte[1] &= 0xFCu;
			a1x->flags_0xc_12_15.dword |= v3;
			ClearEntityVisibleFlag_5F7E0(a2x, a1x);
			v7 = 1;
			goto LABEL_23;
		case 2:
			if (a2x->stage_0x2E_46 <= 0)
				break;
			if (!v5)
				PrepareEventSound_6E450(0, a1x->playerData_0xA4_164x->playerColorIndex_0x38_56, 29);
			goto LABEL_23;
		case 4:
		case 6:
		case 8:
		case 0xB:
		case 0xC:
		case 0xE:
			if (a1x->model_0x40_64)
				goto LABEL_16;
			if (a2x->stage_0x2E_46 <= 0)
				break;
			if (a2x->model_0x40_64 == 4)
				a2x->stage_0x2E_46 = 7;
			else
				a2x->stage_0x2E_46 = 1;
			goto LABEL_23;
		case 7:
			if (a2x->levelPar3_0x46_70 < 1 || !a2x->stage_0x2E_46)
				break;
			goto LABEL_23;
		case 9:
		case 0xA:
		case 0xD:
		case 0xF:
		case 0x10:
		case 0x11:
		case 0x12:
		case 0x13:
		case 0x14:
		case 0x15:
		case 0x16:
		case 0x17:
		case 0x18:
		LABEL_16:
			if (a2x->stage_0x2E_46)
				goto LABEL_23;
			break;
		default:
			break;
		}
		if (a1x->mana_0x90_144 < a2x->maxMana_0x8C_140)
		{
			v6 = 1;
		}
		else
		{
			SetCastleCooldown_5F7B0(a2x, a1x, v3);
			v7 = 1;
		}
	}
LABEL_23:
	if (v6 && !v5)
	{
		SetHelpItem41Active_88B60();
		PrepareEventSound_6E450(0, a1x->playerData_0xA4_164x->playerColorIndex_0x38_56, 29);
	}
	return v7;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0005F7B0) --------------------------------------------------------
void SetCastleCooldown_5F7B0(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x, int a3)//2407b0
{
	a1x->stage_0x2E_46 = a1x->maxStage_0x30_48;
	a2x->flags_0xc_12_15.byte[1] &= 0xFCu;
	a2x->flags_0xc_12_15.dword |= a3;
	ClearEntityVisibleFlag_5F7E0(a1x, a2x);
}

//----- (0005F810) --------------------------------------------------------
type_entity_0x6E8E* FindNearestEnemyBalloon_5F810(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x, type_entity_0x6E8E* a3x)//240810
{
	unsigned int v3; // edi
	type_entity_0x6E8E* v4x; // esi
	type_entity_0x6E8E* v5x; // ebx
	unsigned int v7; // eax

	v3 = -1;
	v4x = 0;
	v5x = gameState_D41A4.manaSphereList_38523;
	if (v5x <= Entities_EA3E4[0])
		return 0;
	do
	{
		if (v5x->model_0x40_64 == 39
			&& !(v5x->flags_0xc_12_15.byte[1] & 0x20)
			&& v5x->playerEntityIndex_0x94_148 == a1x->id_0x1A_26
			&& v5x != a2x
			&& v5x != a3x)
		{
			v7 = Maths::DistanceSquared3D_58440(&a1x->position_0x4C_76, &v5x->position_0x4C_76);
			if (v7 < v3)
			{
				v4x = v5x;
				v3 = v7;
			}
		}
		v5x = v5x->next_0;
	} while (v5x > Entities_EA3E4[0]);
	return v4x;
}
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0005F890) --------------------------------------------------------
void UpdateCastleLevel_5F890(type_entity_0x6E8E* a1x, __int16 a2)//240890
{
	//x_WORD *v2; // eax
	type_entity_0x6E8E* v2x;
	__int16 v3; // bx

	//v2 = *(x_WORD **)&Entities_EA3E4[a1x->word_0x1A_26]->dword_0xA4_164;
	v3 = Entities_EA3E4[a1x->id_0x1A_26]->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[2];
	if (v3)
	{
		if (a2)
		{
			v2x = Entities_EA3E4[v3];
			v2x->stage_0x2E_46 = v2x->maxStage_0x30_48 - 1;
		}
		else
		{
			Entities_EA3E4[v3]->stage_0x2E_46 = 0;
			/*LOBYTE(v2) = */RestorePreviousSpell_6D880(a1x);
		}
	}
	//return (char)v2;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0005F8F0) --------------------------------------------------------
void EndOfCastleProjectile_5F8F0(type_entity_0x6E8E* a1x)//2408f0
{
	//__int16 v1; // si
	//char v2; // dl
	type_entity_0x6E8E* v3x; // esi

	//v1 = a1x->word_0x30_48;
	if (a1x->maxStage_0x30_48)
	{
		if (a1x->maxStage_0x30_48 == 1)
		{
			if (!(a1x->flags_0xc_12_15.byte[2] & 0x10))
			{
				a1x->actionIndex_0x45_69 = 5;
				a1x->stage_0x2E_46 = 3;
				a1x->maxStage_0x30_48 = 0;
			}
		}
		else
		{
			a1x->maxStage_0x30_48--;
			UpdateCastleLevel_5F890(a1x, 1);
			a1x->position_0x4C_76.z = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
		}
	}
	else
	{
		if (CheckCastleDestroyed_609E0(a1x) == 2)
		{
			a1x->actionIndex_0x45_69 = 6;
		}
		else if (a1x->flags_0xc_12_15.byte[0] & 0x40)
		{
			a1x->stage_0x2E_46 = 0;
			a1x->actionIndex_0x45_69 = 5;
		}
		a1x->position_0x4C_76.z = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
		//v2 = a1x->instanceIndex_0x3E_62;
		a1x->playerEntityIndex_0x94_148 = a1x->id_0x1A_26;
		if (!(a1x->instanceIndex_0x3E_62 & 1))
		{
			TransferCastleMana_5FD00(a1x);
			SetShiftByCastle_49EC0(a1x, a1x->param_0x10_16);
			a1x->params_0x52_82.yaw = -8192;
			a1x->params_0x52_82.fov = 0x4000;
			UpdateCastleStructure_5FF50(a1x);
			if (a1x->mana_0x90_144 < a1x->maxMana_0x8C_140)
			{
				v3x = gameState_D41A4.manaSphereList_38523;
				if (v3x > Entities_EA3E4[0])
				{
					while (v3x->model_0x40_64 != 39
						|| v3x->playerEntityIndex_0x94_148 != a1x->id_0x1A_26
						|| !CompareAxisWithShift_10750(a1x, v3x))
					{
						v3x = v3x->next_0;
						if (v3x <= Entities_EA3E4[0])
							return;
					}
					a1x->mana_0x90_144 += v3x->mana_0x90_144;
					DisableEntityDrawing04_57F10(v3x);
				}
			}
		}
	}
}

//----- (0005FA70) --------------------------------------------------------
void BeginOfCastleCreation_5FA70(type_entity_0x6E8E* locEvent)//240a70
{
	switch (locEvent->stage_0x2E_46)
	{
	case 0:
		KillEntitiesInArea_11960(locEvent);
		if (!locEvent->param_0x10_16 || CanPlaceCastleArea_11A10(locEvent))
		{
			if ((locEvent->flags_0xc_12_15.byte[0] & 2) == 0)
			{
				locEvent->spriteIndex_0x5A_90 += TransformPlayerColorIndex_616D0(Entities_EA3E4[locEvent->id_0x1A_26]->playerData_0xA4_164x->playerColorIndex_0x38_56);
				locEvent->flags_0xc_12_15.byte[0] |= 2u;
			}
			SpawnCastleBuildEffect_60480(locEvent);
		}
		else
		{
			locEvent->stage_0x2E_46 = 2;
			locEvent->flags_0xc_12_15.byte[0] &= 0xBF;
			SetHelpPopupIndex93_88D00();
		}
		break;
	case 1:
	case 6:
		locEvent->position_0x4C_76.z = GetTerrainAlt_10C40(&locEvent->position_0x4C_76);
		break;
	case 2:
		locEvent->actionIndex_0x45_69 = 4;
		UpdateCastleLevel_5F890(locEvent, 0);
		locEvent->stage_0x2E_46 = 0;
		break;
	case 3:
		UpdateCastleLevel_5F890(locEvent, 1);
		SpawnCastleEffect42_5FBD0(locEvent);
		break;
	case 4:
		locEvent->position_0x4C_76.z = GetTerrainAlt_10C40(&locEvent->position_0x4C_76);
		if ((locEvent->instanceIndex_0x3E_62 & 0x1F) == 0)
		{
			bool is10_42Type = false;
			for (type_entity_0x6E8E* i = gameState_D41A4.miscList_38535; i > Entities_EA3E4[0] && !is10_42Type; i = i->next_0)
			{
				if (i->class_0x3F_63 == 10 && i->model_0x40_64 == 42)
					is10_42Type = true;
			}
			if (!is10_42Type)
				locEvent->stage_0x2E_46 = 3;
		}
		break;
	case 5:
		UpdateCastleLevel_5F890(locEvent, 1);
		locEvent->position_0x4C_76.z = GetTerrainAlt_10C40(&locEvent->position_0x4C_76);
		SpawnCastleEffect41_5FC40(locEvent);
		break;
	default:
		return;
	}
}

//----- (0005FBD0) --------------------------------------------------------
void SpawnCastleEffect42_5FBD0(type_entity_0x6E8E* a1x)//240bd0
{
	type_entity_0x6E8E* indexx; // eax
	__int16 v2; // ST08_2

	indexx = CreateEntity_4A190(&a1x->axis_0x9A_154x, 10, 42);
	if (indexx)
	{
		indexx->levelPar3_0x46_70 = a1x->param_0x10_16;
		indexx->id_0x1A_26 = a1x->id_0x1A_26;
		indexx->parentId_0x28_40 = a1x - world_D41A0.entities_0x6E8E;
		v2 = a1x->param_0x10_16;
		a1x->stage_0x2E_46 = 4;
		/*result = */SetShiftByCastle_49EC0(indexx, v2);
	}
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (0005FC40) --------------------------------------------------------
type_entity_0x6E8E* SpawnCastleEffect41_5FC40(type_entity_0x6E8E* a1x)//240c40
{
	type_entity_0x6E8E* resultx; // eax
	type_entity_0x6E8E* v2x; // ebx

	resultx = CreateEntity_4A190(&a1x->axis_0x9A_154x, 10, 41);
	v2x = resultx;
	if (resultx)
	{
		resultx->levelPar3_0x46_70 = a1x->param_0x10_16;
		resultx->id_0x1A_26 = a1x->id_0x1A_26;
		v2x->parentId_0x28_40 = a1x - world_D41A0.entities_0x6E8E;
		a1x->stage_0x2E_46 = 6;
	}
	return resultx;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (0005FCA0) --------------------------------------------------------
void DestroyCastleLevel_5FCA0(type_entity_0x6E8E* a1x)//240ca0
{
	if (GetFreeEntityCount_4A810())
	{
		ShrinkCastleMana_605E0(a1x);
		a1x->actionIndex_0x45_69 = 4;
		TransferCastleMana_5FD00(a1x);
		UpdateCastleStructure_5FF50(a1x);
		a1x->stage_0x2E_46 = 0;
		a1x->maxStage_0x30_48 = 5;
	}
	else
	{
		a1x->actionIndex_0x45_69 = 4;
	}
	//return result;
}

//----- (0005FD00) --------------------------------------------------------
void TransferCastleMana_5FD00(type_entity_0x6E8E* a1x)//240d00
{
	int result; // eax
	//int v2; // esi
	int v3; // esi
	type_entity_0x6E8E* v4x; // eax
	//type_str_0x6E8E* v5x; // esi
	//__int16 v6; // di
	//unsigned __int16 v7; // ax
	int v8; // edi
	__int16 v9; // ax
	__int16 v10; // ST0C_2
	int v11; // eax
	int v12; // edi
	signed int v13; // [esp+0h] [ebp-10h]
	int v14; // [esp+4h] [ebp-Ch]
	int v15; // [esp+8h] [ebp-8h]
	__int16 v16; // [esp+Ch] [ebp-4h]

	//result = Entities_EA3E4[a1x->word_0x1A_26]->dword_0xA4_164;
	v14 = 0;
	//v2 = a1x->dword_0x8C_140;
	if (Entities_EA3E4[a1x->id_0x1A_26]->playerData_0xA4_164x->extraMana_0x13C_316 + a1x->mana_0x90_144 > a1x->maxMana_0x8C_140)
		v14 = a1x->mana_0x90_144 - a1x->maxMana_0x8C_140;
	if (!a1x->param_0x10_16)
	{
		//result = a1x->dword_0x90_144;
		v14 = a1x->mana_0x90_144;
	}
	if (v14 > 0)
	{
		if (v14)
		{
			v3 = v14 / 1000;
			v16 = GetFreeEntityCount_4A810();
			if (!v16)
			{
				RebuildEntityLists_49F90();
				result = GetFreeEntityCount_4A810();
				v3 = 8;//0x240D90 unconditional
				v16 = result;
				world_D41A0.freeEntityCount2_0x11e6 = -1;
			}
			if (v16)
			{
				if (v3 < 1)
					v3 = 1;
				if (v3 > 32)
					v3 = 32;
				if (v16 < 0)
					v16 = 0;
				if (v16 > v3)
					v16 = v3;
				v13 = 0;
				v15 = v14 / v16;
				for (result = v16; v16 > v13; result = v16)
				{
					predictedAxis_EB398ar = a1x->position_0x4C_76;
					v4x = CreateEntity_4A190(&predictedAxis_EB398ar, 10, 39);
					//v5x = v4x;
					if (v4x)
					{
						v4x->mana_0x90_144 = v15;
						//v6 = v4x->word_0x14_20;
						v4x->playerEntityIndex_0x94_148 = a1x->id_0x1A_26;
						v4x->rand_0x14_20 = 9377 * v4x->rand_0x14_20 + 9439;
						//v7 = v4x->word_0x14_20;
						v4x->axis_0x9A_154x.x = 0;
						v4x->axis_0x9A_154x.y = 0;
						v4x->actSpeed_0x82_130 = v4x->rand_0x14_20 % 0x30u + 16;
						v8 = a1x->position_0x4C_76.z;
						v9 = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
						//v5x->word_0x2C_44 = (1024-(v8-v9)-(__CFSHL__((1024-(v8-v9)) >> 31, 3)+8*((1024-(v8-v9))>>31)))>>3;
						v4x->param2_0x2C_44 = (1024 - (v8 - v9) - (my_sign32(1024 - (v8 - v9)) * 7)) >> 3;
						a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
						v10 = a1x->rand_0x14_20 % 0x1400u + 3840;
						a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
						MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->rand_0x14_20 & 0x7FF, 0, v10);
						CopyEntityPosition_57CF0(v4x, &predictedAxis_EB398ar);
						v11 = v4x->mana_0x90_144;
						v12 = v14 - v11;
						v14 = v12;
						a1x->mana_0x90_144 -= v11;
						if (v12 < v15)
							v15 = v12;
					}
					v13++;
				}
			}
		}
	}
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;

int debugcounter_240f50 = 0;

//----- (0005FF50) --------------------------------------------------------
void UpdateCastleStructure_5FF50(type_entity_0x6E8E* a1x)//240f50
{
	int v1; // edi
	//int v2; // eax
	type_entity_0x6E8E* v3x; // esi
	type_entity_0x6E8E* v4x; // eax
	//int v5; // eax
	//int v6; // edx
	//__int64 v7; // rtt
	type_entity_0x6E8E* v8x; // eax
	type_entity_0x6E8E* v9x; // esi
	type_entity_0x6E8E* v10x; // esi
	__int16 v11; // si
	//int result; // eax
	//int v13; // edi
	type_entity_0x6E8E* v14x; // esi
	__int16 v15; // ax
	//int v16; // [esp+4h] [ebp-1Ch]
	type_entity_0x6E8E* v17x; // [esp+8h] [ebp-18h]
	type_entity_0x6E8E* v18x; // [esp+Ch] [ebp-14h]
	__int16 v19; // [esp+14h] [ebp-Ch]
	int v20; // [esp+18h] [ebp-8h]
	__int16 v21; // [esp+1Ch] [ebp-4h]

	v18x = Entities_EA3E4[a1x->id_0x1A_26];
	v1 = 0;
	GetCastleLevelSize_60400(a1x->param_0x10_16, &v21, &v19);
	if (v21 > 0)
	{
		do
		{
			while (1)
			{
				v18x->playerData_0xA4_164x->castleManaSum_0x12E_302 = 0;
				//v2 = v18x->dword_0xA4_164;
				//v16 = 2 * (signed __int16)v1;
				v3x = Entities_EA3E4[v18x->playerData_0xA4_164x->balloons_0x3C_60[v1]];
				if (v3x <= Entities_EA3E4[0])
				{
					v8x = CreateEntity_4A190(&a1x->position_0x4C_76, 3, 3);
					v9x = v8x;
					if (v8x)
					{
						v8x->id_0x1A_26 = a1x->id_0x1A_26;
						v8x->spriteIndex_0x5A_90 += TransformPlayerColorIndex_616D0(v18x->playerData_0xA4_164x->playerColorIndex_0x38_56);
						v9x->playerEntityIndex_0x94_148 = a1x->id_0x1A_26;
						v18x->playerData_0xA4_164x->balloons_0x3C_60[v1] = v9x - world_D41A0.entities_0x6E8E;
						v9x->actionIndex_0x45_69 = 9;
					}
					goto LABEL_17;
				}
				if (v3x->life_0x8 >= 0)
					break;
				TransformEntityToManaSphere_36BA0(v3x, false);
				DisableEntityDrawing04_57F10(v3x);
				v18x->playerData_0xA4_164x->balloons_0x3C_60[v1] = 0;
			LABEL_17:
				if ((signed __int16)++v1 >= v21)
					goto LABEL_22;
			}
			if (v18x->playerData_0xA4_164x->extraMana_0x13C_316 + a1x->mana_0x90_144 >= a1x->maxMana_0x8C_140)
			{
				v3x->targetId_0x96_150 = a1x - world_D41A0.entities_0x6E8E;
			}
			else
			{
				if (a1x->instanceIndex_0x3E_62 % v21)
					goto LABEL_13;
				if (v3x->actionIndex_0x45_69 != 9)
					goto LABEL_13;
				v3x->targetId_0x96_150 = a1x - world_D41A0.entities_0x6E8E;
				if (v3x->maxMana_0x8C_140 <= v3x->mana_0x90_144)
					goto LABEL_13;
				Entities_EA3E4[0]->targetId_0x96_150 = 0;
				v4x = FindNearestEnemyBalloon_5F810(
					v3x,
					Entities_EA3E4[Entities_EA3E4[v18x->playerData_0xA4_164x->balloons_0x3C_60[((signed __int16)v1 + 1) % 3]]->targetId_0x96_150],
					Entities_EA3E4[Entities_EA3E4[v18x->playerData_0xA4_164x->balloons_0x3C_60[((signed __int16)v1 + 2) % 3]]->targetId_0x96_150]);
				if (!v4x)
					goto LABEL_13;
				v3x->targetId_0x96_150 = v4x - world_D41A0.entities_0x6E8E;
			}
			//LODWORD(v7) = v5;
			//HIDWORD(v7) = v6 >> 31;
			//v3x->targetId_0x96_150 = v7 / 168;
		LABEL_13:
			v18x->playerData_0xA4_164x->castleManaSum_0x12E_302 += v3x->mana_0x90_144;
			v18x->playerData_0xA4_164x->castleMaxManaSum_0x12A_298 += v3x->maxMana_0x8C_140;
			v1++;
		} while ((signed __int16)v1 < v21);
	}
LABEL_22:
	while ((signed __int16)v1 < 3)
	{
		v10x = Entities_EA3E4[v18x->playerData_0xA4_164x->balloons_0x3C_60[v1]];
		if (v10x > Entities_EA3E4[0])
		{
			TransformEntityToManaSphere_36BA0(v10x, false);
			DisableEntityDrawing04_57F10(v10x);
			v18x->playerData_0xA4_164x->balloons_0x3C_60[v1] = 0;
		}
		LOWORD(v1) = v1 + 1;
	}
	v11 = a1x->param2_0x2C_44;
	if (v11 > 0)
		a1x->param2_0x2C_44 = v11 - 1;
	v20 = 0;
	//result = 0;
	if (v19 > 0)
	{
		do
		{
			if (CommandLineParams.DoDebugSequences()) {
				//adress 24128D
				/*if (CommandLineParams.DoDebugafterload())
				{
					uint8_t origbyte20 = 0;
					uint8_t remakebyte20 = 0;
					int comp20;
					comp20 = compare_with_sequence((char*)"0024128D-002DC4E0", (uint8_t*)x_BYTE_10B4E0_terraintype, 0x2dc4e0, debugcounter_240f50, 0x70000, 0x10000, &origbyte20, &remakebyte20);
					comp20 = compare_with_sequence((char*)"0024128D-002DC4E0", (uint8_t*)x_BYTE_11B4E0_height, 0x2dc4e0, debugcounter_240f50, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x10000);
					comp20 = compare_with_sequence((char*)"0024128D-002DC4E0", (uint8_t*)x_BYTE_12B4E0_shading, 0x2dc4e0, debugcounter_240f50, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x20000);
					comp20 = compare_with_sequence((char*)"0024128D-002DC4E0", (uint8_t*)x_BYTE_13B4E0_angle, 0x2dc4e0, debugcounter_240f50, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x30000);
					//comp20 = compare_with_sequence((char*)"00228320", (uint8_t*)x_BYTE_14B4E0, 0x2dc4e0, debugcounter11, 0x70000,0x10000, &origbyte20, &remakebyte20, 0x40000);
					comp20 = compare_with_sequence((char*)"0024128D-002DC4E0", (uint8_t*)x_WORD_15B4E0_source, 0x2dc4e0, debugcounter_240f50, 0x70000, 0x20000, &origbyte20, &remakebyte20, 0x50000);

					//uint8_t origbyte20 = 0;
					//uint8_t remakebyte20 = 0;
					comp20 = compare_with_sequence_D41A0((char*)"0024128D-00356038", (uint8_t*)& D41A0_BYTESTR_0, 0x356038, debugcounter_240f50, 224790, &origbyte20, &remakebyte20);

					comp20 = compare_with_sequence_array_E2A74((char*)"0024128D-002B3A74", (uint8_t*)& array_E2A74, 0x2b3a74, debugcounter_240f50, 0xc4e, 0xc4e, &origbyte20, &remakebyte20);
					//comp20 = compare_with_sequence((char*)"0022860F-003514B0", (uint8_t*)& helpPopup_1804B0, 0x3514b0, debugcounter_47560, 0xab, 0xab, &origbyte20, &remakebyte20);

					//comp20 = compare_with_sequence((char*)"0022860F-003AA0A4", pdwScreenBuffer_351628, 0x3aa0a4, debugcounter_47560, 320 * 200, 320 * 200, &origbyte20, &remakebyte20);

					debugcounter_240f50++;
				}*/
			}

			//v13 = 2 * (signed __int16)v20;
			v14x = Entities_EA3E4[v18x->playerData_0xA4_164x->castleParts_0x5C_92[v20]];
			if (v14x <= Entities_EA3E4[0])
			{
				if (!a1x->param2_0x2C_44)
				{
					v17x = CreateEntity_4A190(&a1x->position_0x4C_76, 5, 15);
					if (v17x)
					{
						a1x->param2_0x2C_44 = 16;
						v15 = a1x->id_0x1A_26;
						v17x->id_0x1A_26 = v15;
						v17x->playerEntityIndex_0x94_148 = v15;
						v18x->playerData_0xA4_164x->castleParts_0x5C_92.at(v20) = v17x - world_D41A0.entities_0x6E8E;
						v17x->yaw_0x1C_28 = 512;
						v17x->roll_0x20_32 = v17x->yaw_0x1C_28;
						predictedAxis_EB398ar = v17x->position_0x4C_76;
						predictedAxis_EB398ar.x += 128;
						predictedAxis_EB398ar.y += 640;
						predictedAxis_EB398ar.z = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
						CopyEntityPosition_57CF0(v17x, &predictedAxis_EB398ar);
					}
				}
			}
			else if (v14x->class_0x3F_63 != 5 || v14x->model_0x40_64 != 15 || v14x->actionIndex_0x45_69 == 125)
			{
				v18x->playerData_0xA4_164x->castleParts_0x5C_92[v20] = 0;
				a1x->param2_0x2C_44 = 16;
			}
			/*result = */v20++;
		} while ((signed __int16)v20 < v19);
	}
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39A: using guessed type __int16 x_WORD_EB398ar[1];
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (00060400) --------------------------------------------------------
x_WORD* GetCastleLevelSize_60400(__int16 a1, x_WORD* a2, x_WORD* a3)//241400
{
	x_WORD* result; // eax

	result = a3;
	switch (a1)
	{
	case 1:
	case 2:
		*a2 = 1;
		*a3 = 0;
		break;
	case 3:
		*a2 = 1;
		*a3 = 4;
		break;
	case 4:
		*a2 = 2;
		*a3 = 6;
		break;
	case 5:
		*a2 = 2;
		*a3 = 14;
		break;
	case 6:
		*a2 = 3;
		*a3 = 18;
		break;
	case 7:
		*a2 = 3;
		*a3 = 34;
		break;
	default:
		*a2 = 0;
		*a3 = 0;
		break;
	}
	return result;
}

//----- (00060480) --------------------------------------------------------
void SpawnCastleBuildEffect_60480(type_entity_0x6E8E* a1x)//241480
{
	type_entity_0x6E8E* v1x; // eax
	type_entity_0x6E8E* v2x; // esi
	//int v3; // edx
	//__int16 v4; // ST08_2
	//__int16 v5; // ST08_2
	type_entity_0x6E8E* v6x; // edi
	//char v7; // dl

	v1x = AddCastleEffect42_50370(&a1x->axis_0x9A_154x);
	v2x = v1x;
	if (v1x)
	{
		PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 10);
		//v3 = a1x->dword_0x10_16 + 1;
		a1x->flags_0xc_12_15.byte[0] &= 0xBFu;
		a1x->param_0x10_16++;
		//v4 = a1x->dword_0x10_16;
		a1x->actionIndex_0x45_69 = 5;
		a1x->stage_0x2E_46 = 4;
		SetShiftByCastle_49EC0(a1x, a1x->param_0x10_16);
		//v5 = a1x->dword_0x10_16;
		a1x->params_0x52_82.yaw = 0xe000;
		a1x->params_0x52_82.fov = 0x4000;
		SetShiftByCastle_49EC0(v2x, a1x->param_0x10_16);
		v6x = Entities_EA3E4[a1x->id_0x1A_26];
		v6x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58 = a1x - world_D41A0.entities_0x6E8E;
		v6x->playerData_0xA4_164x->word_0x1C2_450 = a1x->param_0x10_16;
		v6x->playerData_0xA4_164x->castleBuilding_0x1BE_446 = 0;
		GrowCastle_60810(a1x);
		UpdateBalloonChain_613D0(a1x);
		SendSpellMessage_6D8B0(v6x - world_D41A0.entities_0x6E8E, 2u, 1);
		v2x->parentId_0x28_40 = a1x - world_D41A0.entities_0x6E8E;
		v2x->id_0x1A_26 = a1x->id_0x1A_26;
		//v7 = v2x->flags_0xc_12_15.byte[2];
		//LOBYTE(v1) = a1x->dword_0x10_16;
		v2x->poseIndex_0x3B_59 = 0;
		v2x->levelPar3_0x46_70 = a1x->param_0x10_16;
		v2x->flags_0xc_12_15.byte[2] |= 1;
	}
	//	return v1;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000605E0) --------------------------------------------------------
void ShrinkCastleMana_605E0(type_entity_0x6E8E* a1x)//2415e0
{
	int v1; // esi
	__int16 v2; // ST00_2
	int v5; // eax
	type_entity_0x6E8E* v6x; // edx
	type_entity_0x6E8E* v8x; // esi
	__int16 v9; // dx

	if (a1x->param_0x10_16 > 0)
	{
		v1 = 10 * a1x->maxMana_0x8C_140 / 100;
		a1x->maxMana_0x8C_140 -= v1;
		TransferCastleMana_5FD00(a1x);
		v2 = a1x - world_D41A0.entities_0x6E8E;
		a1x->maxMana_0x8C_140 += v1;
		PrepareEventSound_6E450(v2, -1, 30);
		Entities_EA3E4[0]->position_0x4C_76 = a1x->axis_0x9A_154x;
		Entities_EA3E4[0]->levelPar3_0x46_70 = a1x->param_0x10_16;
		Entities_EA3E4[0]->id_0x1A_26 = a1x->id_0x1A_26;
		v5 = a1x - world_D41A0.entities_0x6E8E;
		v6x = Entities_EA3E4[0];
		Entities_EA3E4[0]->model_0x40_64 = 0;
		v6x->param_0x10_16 = 0;
		v6x->parentId_0x28_40 = v5;
		RemoveCastleStage_385C0(v6x);
		a1x->param_0x10_16--;
		SetShiftByCastle_49EC0(a1x, a1x->param_0x10_16);
		a1x->params_0x52_82.yaw = 0xE000;
		a1x->params_0x52_82.fov = 0x4000;
		GrowCastle_60810(a1x);
		UpdateBalloonChain_613D0(a1x);
		UpdateCastleLevel_5F890(a1x, 1);
	}
	if (!a1x->param_0x10_16)
	{
		v8x = Entities_EA3E4[a1x->id_0x1A_26];
		if (v8x->model_0x40_64 == 1)
		{
			if (world_D41A0.terrain_2FECE.levelType_0x2FED2 & 4)
			{
				v9 = v8x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[2];
				if (v9)
				{
					DisableEntityDrawing04_57F10(Entities_EA3E4[v9]);
					v8x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[2] = 0;
				}
			}
		}
		else
		{
			UpdateCastleLevel_5F890(a1x, a1x->param_0x10_16);
		}
		v8x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58 = 0;
		DisableEntityDrawing04_57F10(a1x);
	}
}

//----- (00060780) --------------------------------------------------------
void SetCastleLifeAndMana_60780(type_entity_0x6E8E* locEvent, type_entity_0x6E8E* locEvent2, int number1, int number2)//241780
{
	if (number1)
	{
		int number3 = 0;
		locEvent->maxLife_0x4 = number1;
		if (locEvent->life_0x8 < 0)
		{
			number3 = -locEvent->life_0x8;
			if (-locEvent->life_0x8 > number1 / 2)
				number3 = number1 / 2;
		}
		locEvent->life_0x8 = locEvent->maxLife_0x4 - number3;
	}
	if (locEvent2)
	{
		short originalWord46 = locEvent2->stage_0x2E_46;
		locEvent2->stage_0x2E_46 = 0;
		SetSpell_6D5E0(locEvent2, locEvent2->levelPar3_0x46_70);
		locEvent2->stage_0x2E_46 = originalWord46;
	}
	locEvent->maxMana_0x8C_140 = number2;
}

//----- (00060810) --------------------------------------------------------
void GrowCastle_60810(type_entity_0x6E8E* locEvent)//241810
{
	type_entity_0x6E8E* locEvent2 = nullptr;

	if ((Entities_EA3E4[locEvent->id_0x1A_26]->actionIndex_0x45_69 <= 1u) &&
		(Entities_EA3E4[locEvent->id_0x1A_26]->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[2]))
	{
		locEvent2 = Entities_EA3E4[Entities_EA3E4[locEvent->id_0x1A_26]->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[2]];
	}
	int number1 = (Entities_EA3E4[locEvent->id_0x1A_26]->playerData_0xA4_164x->lifeScale_0x24A_586 * ((Entities_EA3E4[locEvent->id_0x1A_26]->playerData_0xA4_164x->castleSpells_0x24E_590[locEvent->param_0x10_16] << 8) + 256)) >> 8;
	switch (locEvent->param_0x10_16) {
	case 0:
		SetCastleLifeAndMana_60780(locEvent, locEvent2, 0, 5000);
		break;
	case 1:
		SetCastleLifeAndMana_60780(locEvent, locEvent2, 20000 * number1 >> 8, 8500);
		break;
	case 2:
		SetCastleLifeAndMana_60780(locEvent, locEvent2, 40000 * number1 >> 8, 18000);
		break;
	case 3:
		SetCastleLifeAndMana_60780(locEvent, locEvent2, 40000 * number1 >> 8, 38800);
		break;
	case 4:
		SetCastleLifeAndMana_60780(locEvent, locEvent2, 60000 * number1 >> 8, 78600);
		break;
	case 5:
		SetCastleLifeAndMana_60780(locEvent, locEvent2, 60000 * number1 >> 8, 158200);
		break;
	case 6:
		SetCastleLifeAndMana_60780(locEvent, locEvent2, 80000 * number1 >> 8, 317400);
		break;
	case 7:
		SetCastleLifeAndMana_60780(locEvent, locEvent2, 80000 * number1 >> 8, 300000000);
		break;
	}
}

//----- (000609E0) --------------------------------------------------------
int CheckCastleDestroyed_609E0(type_entity_0x6E8E* locEvent)//2419e0
{
	int result = 0;
	if (locEvent->life_0x8 < 0)
		return 2;
	if (locEvent->mail_0x5E_94.channel[0].source)
	{
		locEvent->life_0x8 -= locEvent->mail_0x5E_94.channel[0].amount.dword;
		if (locEvent->life_0x8 < 0)
		{
			locEvent->ownerPlayerId_0x24_36 = locEvent->mail_0x5E_94.channel[0].source;
			locEvent->mail_0x5E_94.channel[0].source = 0;
			return 2;
		}
		locEvent->mail_0x5E_94.channel[0].source = 0;
		locEvent->mail_0x5E_94.channel[0].amount.dword = 0;
		result = 1;
		Entities_EA3E4[locEvent->id_0x1A_26]->playerData_0xA4_164x->castleDestroyedBlink_0x195_405 = 4;
	}
	if (locEvent->mail_0x5E_94.channel[5].source == locEvent->id_0x1A_26)
	{
		if (locEvent->param_0x10_16 < 7)
			locEvent->flags_0xc_12_15.byte[0] |= 0x40u;
		locEvent->mail_0x5E_94.channel[5].source = 0;
	}
	return result;
}

//----- (00060AB0) --------------------------------------------------------
void UpdateCastleBalloon_60AB0(type_entity_0x6E8E* a1x)//241ab0
{
	type_entity_0x6E8E* v1x; // eax
	axis_3d* v2; // esi
	char v3; // dl
	signed int v4; // esi
	signed int v5; // edi
	signed int v6; // eax
	unsigned int v7; // edi
	int v8; // esi
	int v9; // ecx
	//signed __int16 *v10; // eax
	__int16 v11; // ST10_2
	int v12; // ST0C_4
	int v13; // ST08_4
	__int16 v14; // ax
	type_entity_0x6E8E* v16x; // [esp+0h] [ebp-4h]

	v1x = Entities_EA3E4[a1x->targetId_0x96_150];
	v16x = v1x;
	if (v1x > Entities_EA3E4[0])
	{
		predictedAxis_EB398ar = a1x->position_0x4C_76;
		v2 = &v1x->position_0x4C_76;
		a1x->yaw_0x1C_28 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v1x->position_0x4C_76);
		v3 = v16x->class_0x3F_63;
		if (v3 == 10)
		{
			if (v16x->playerEntityIndex_0x94_148 != a1x->id_0x1A_26)
				goto LABEL_20;
			if (isCaveLevel_D41B6)
				v4 = 2048;
			else
				v4 = 1024;
			v5 = Maths::DistanceXY_58490(&predictedAxis_EB398ar, &v16x->position_0x4C_76);
			if (v5 > v4)
			{
				v16x->flags_0xc_12_15.byte[0] &= 0xBFu;
			}
			else
			{
				v16x->flags_0xc_12_15.byte[0] |= 0x40u;
				v16x->targetId_0x96_150 = a1x - world_D41A0.entities_0x6E8E;
				if (EntitiesOverlap_106C0(v16x, a1x))
				{
					a1x->mana_0x90_144 += v16x->mana_0x90_144;
					a1x->playerEntityIndex_0x94_148 = v16x->playerEntityIndex_0x94_148;
					a1x->life_0x8 = a1x->maxLife_0x4;
					a1x->targetId_0x96_150 = 0;
					DisableEntityDrawing04_57F10(v16x);
				}
			}
			v6 = a1x->actSpeed_0x82_130;
			if (v5 <= v6)
			{
				predictedAxis_EB398ar.x = v16x->position_0x4C_76.x;
				predictedAxis_EB398ar.y = v16x->position_0x4C_76.y;
				goto LABEL_20;
			}
		}
		else
		{
			if (v3 == 3)
			{
				v7 = a1x->actSpeed_0x82_130 * v16x->param_0x10_16;
				if (Maths::DistanceXY_58490(&predictedAxis_EB398ar, v2) <= v7)
				{
					v8 = a1x->typeParams_0xA0_160x->minHeight_160_0xc_12;
					if (predictedAxis_EB398ar.z <= v8 + (signed __int16)GetTerrainAlt_10C40(&predictedAxis_EB398ar) && v16x->param_0x10_16 > 0)
					{
						predictedAxis_EB398ar.x = v16x->position_0x4C_76.x;
						v9 = v16x->mana_0x90_144;
						predictedAxis_EB398ar.y = v16x->position_0x4C_76.y;
						v16x->mana_0x90_144 = a1x->mana_0x90_144 + v9;
						a1x->mana_0x90_144 = 0;
						a1x->playerEntityIndex_0x94_148 = a1x->id_0x1A_26;
						a1x->life_0x8 = a1x->maxLife_0x4;
					}
					goto LABEL_20;
				}
			}
			LOWORD(v6) = a1x->actSpeed_0x82_130;
		}
		MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, a1x->pitch_0x1E_30, v6);
	LABEL_20:
		if (isCaveLevel_D41B6)
		{
			SpawnCastleRubble_60D50(&predictedAxis_EB398ar, a1x);
		}
		else
		{
			//v10 = *(signed __int16 **)&a1x->dword_0xA0_160;
			v11 = a1x->typeParams_0xA0_160x->climbSpeed_160_0xe_14;
			v12 = a1x->typeParams_0xA0_160x->maxHeight_160_0xa_10;
			v13 = a1x->typeParams_0xA0_160x->minHeight_160_0xc_12;
			v14 = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
			ClampAboveTerrain_580E0(&predictedAxis_EB398ar, v14, v13, v12, v11);
			CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
		}
	}
	ApplyCastleDamage_60EA0(a1x);
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41B6: using guessed type char x_BYTE_D41B6;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39A: using guessed type __int16 x_WORD_EB398ar[1];
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (00060D50) --------------------------------------------------------
void SpawnCastleRubble_60D50(axis_3d* a1x, type_entity_0x6E8E* a2x)//241d50
{
	int v2; // edx
	char v3; // ch
	//signed __int16 *v4; // edx
	__int16 v5; // ST10_2
	int v6; // ST0C_4
	int v7; // ST08_4
	__int16 v8; // ax
	int v9; // edi
	int v10; // edx
	char v12; // [esp+0h] [ebp-4h]

	v12 = 0;
	v2 = a1x->x >> 8;
	BYTE1(v2) = a1x->y >> 8;
	if (a2x->flags_0xc_12_15.byte[0] & 1)
	{
		if (!(mapAngle_13B4E0[(unsigned __int16)v2] & 8) && !IsTouchingCeiling_11E70(a2x, a1x))
		{
			a2x->flags_0xc_12_15.byte[0] &= 0xFEu;
			v12 = 1;
		}
		a2x->actSpeed_0x82_130 = 96;
	}
	else
	{
		if (mapAngle_13B4E0[(unsigned __int16)v2] & 8 || IsTouchingCeiling_11E70(a2x, a1x))
		{
			v12 = 1;
			a2x->flags_0xc_12_15.byte[0] |= 1u;
		}
		a2x->actSpeed_0x82_130 = 48;
	}
	v3 = a2x->levelPar3_0x46_70;
	if (v3)
		a2x->levelPar3_0x46_70 = v3 - 1;
	if (v12 && !a2x->levelPar3_0x46_70)
	{
		PrepareEventSound_6E450(a2x - world_D41A0.entities_0x6E8E, -1, 22);
		a2x->levelPar3_0x46_70 = 32;
	}
	//v4 = *(signed __int16 **)&a2x->dword_0xA0_160;
	v5 = a2x->typeParams_0xA0_160x->climbSpeed_160_0xe_14;
	v6 = a2x->typeParams_0xA0_160x->maxHeight_160_0xa_10;
	v7 = a2x->typeParams_0xA0_160x->minHeight_160_0xc_12;
	v8 = GetTerrainAlt_10C40(a1x);
	ClampAboveTerrain_580E0(a1x, v8, v7, v6, v5);
	if (!(a2x->flags_0xc_12_15.byte[0] & 1))
	{
		v9 = a2x->params_0x52_82.fov;
		v10 = (signed __int16)GetCeilingAlt_10C60(a1x) - v9;
		if (a1x->z > v10)
			a1x->z = v10;
	}
	CopyEntityPosition_57CF0(a2x, a1x);
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (00060EA0) --------------------------------------------------------
void ApplyCastleDamage_60EA0(type_entity_0x6E8E* a1x)//241ea0
{
	//signed int v1; // edx
	//int v3; // edx

	//v1 = 0;
	if (a1x->life_0x8 < 0)
		return/* 2*/;
	if (a1x->mail_0x5E_94.channel[0].source)
	{
		//v3 = a1x->dword_0xA4_164;
		a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
		a1x->playerData_0xA4_164x->castleDamagedBlink_0x197_407 = 4;
		if (a1x->life_0x8 < 0)
		{
			a1x->ownerPlayerId_0x24_36 = a1x->mail_0x5E_94.channel[0].source;
			return/* 2*/;
		}
		//v1 = 1;
		a1x->mail_0x5E_94.channel[0].source = 0;
	}
	return/* v1*/;
}

int debugcounter_sub_241F7A = 0;

//----- (00060F00) --------------------------------------------------------
void UpdateCastles_60F00()//241f00
{
	unsigned __int16 v0; // bx
	//uint8_t* v1; // eax
	type_entity_0x6E8E* v2x; // edx
	//x_DWORD *v3; // eax
	type_entity_0x6E8E* v4x; // ebx
	uint8_t v3a;
	type_entity_0x6E8E* v3c;

	/*uint8_t origbyte20 = 0;
	uint8_t remakebyte20 = 0;
	int comp20;

	comp20 = compare_with_sequence_D41A0((char*)"00241F00-00356038", (uint8_t*)& D41A0_BYTESTR_0, 0x356038, debugcounter_sub_241F7A, 224790, &origbyte20, &remakebyte20);
	if(debugcounter_sub_241F7A>=0xb)
	{
		debugcounter_sub_241F7A++;
		debugcounter_sub_241F7A--;
	}
	debugcounter_sub_241F7A++;*/

	v0 = 0;
	//v1 = &x_D41A0_BYTEARRAY_0[11230];
	int v1index = 0;
	while (v0 < world_D41A0.NumberOfPlayers_0xe)
	{
		v2x = Entities_EA3E4[world_D41A0.players_0x2BDE[v1index].playerIndex_0x00a_2BE4_11240];
		v2x->maxMana_0x8C_140 = v2x->playerData_0xA4_164x->maxMana_0x150_336;
		//v1 += 2124;
		v1index++;
		v0++;
		v2x->playerData_0xA4_164x->extraMana_0x13C_316 = 0;
	}
	//v3 = (int32_t*)x_D41A0_BYTEARRAY_4;
	//adress 241f53
	v4x = Entities_EA3E4[1];
	gameState_D41A4.manaStats_242ar.totalMana_4 = 1;
	gameState_D41A4.manaStats_242ar.dword_8 = 0;

	/**(_DWORD *)(dword_D41A4 + 246) = 1;
	*(_DWORD *)(dword_D41A4 + 250) = 0;*/

	if (v4x < Entities_EA3E4[1000])
	{
		while (1)
		{
			v3a = v4x->class_0x3F_63;
			if (v3a < 5u)
				break;
			if (v3a <= 5u)
				goto LABEL_23;
			if (v3a != 10)
				goto LABEL_24;
			v3a = v4x->model_0x40_64;
			if (v3a < 0x2Du)
			{
				if (v3a == 39)
					goto LABEL_23;
			}
			else
			{
				if (v3a <= 0x2Du)
				{
					v3c = AddBalloonManaToCastle_61000(v4x);
					if (v3c)
						if (v3c && v3c->playerData_0xA4_164x)
						{
							v3c->playerData_0xA4_164x->extraMana_0x13C_316 += v4x->mana_0x90_144;
							//v3b = (uint8_t*)(x_DWORD *)v3b[41];
							//v3b[79] += v4x->dword_0x90_144;
						}
					goto LABEL_24;
				}
				if (v3a == 58)
					goto LABEL_23;
			}
		LABEL_24:
			v4x++;
			if (v4x >= Entities_EA3E4[1000])
				return/* (char)v3*/;
		}
		if (v3a != 3)
			goto LABEL_24;
		v3a = v4x->model_0x40_64;
		if (v3a < 2u || v3a > 2u && v3a != 3)
			goto LABEL_24;
	LABEL_23:
		AddBalloonManaToCastle_61000(v4x);
		goto LABEL_24;
	}

	//return (char)v3;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];
// EA3E8: using guessed type int x_DWORD_EA3E8;
// EB384: using guessed type int x_DWORD_EB384;

//----- (00061000) --------------------------------------------------------
type_entity_0x6E8E* AddBalloonManaToCastle_61000(type_entity_0x6E8E* a1x)//242000
{
	if (a1x->playerEntityIndex_0x94_148)
	{
		//LOWORD(v1) = *(x_WORD *)(a1 + 148);
		//v1x = Entities_EA3E4[a1x->playerEntityIndex_0x94_148];
		Entities_EA3E4[a1x->playerEntityIndex_0x94_148]->maxMana_0x8C_140 += a1x->mana_0x90_144;
	}
	gameState_D41A4.manaStats_242ar.totalMana_4 += a1x->mana_0x90_144;
	return Entities_EA3E4[a1x->playerEntityIndex_0x94_148];
}
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00061050) --------------------------------------------------------
type_entity_0x6E8E* UpdateBalloonCollection_61050(type_entity_0x6E8E* a1x)//242050
{
	unsigned __int16 v1; // ax
	type_entity_0x6E8E* v2x; // edx
	//int v3; // eax
	unsigned __int8 v4; // dl
	int v5; // eax
	type_entity_0x6E8E* v6x; // ebx
	type_entity_0x6E8E* v7x; // eax
	int v8; // ecx
	__int64 v9; // rtt
	type_entity_0x6E8E* v10x; // edx
	int v11; // edx
	int v12; // ecx
	__int16 v13; // ax
	__int16 v14; // si
	__int16 v15; // ax
	type_entity_0x6E8E* v16x; // eax
	type_entity_0x6E8E* v17x; // ebx
	__int16 v18; // ax
	__int16 v19; // si
	__int16 v20; // ST0C_2
	unsigned __int16 v21; // ax
	int v22; // esi
	int v23; // esi
	int v24; // ebx
	type_entity_0x6E8E* resultx; // eax
	axis_3d v26x; // [esp+0h] [ebp-28h]
	//__int16 v27; // [esp+4h] [ebp-24h]
	//int *v28; // [esp+8h] [ebp-20h]
	unsigned int v29; // [esp+Ch] [ebp-1Ch]
	int v30; // [esp+10h] [ebp-18h]
	type_entity_0x6E8E* v31x; // [esp+14h] [ebp-14h]
	//uint8_t* v32; // [esp+18h] [ebp-10h]
	int v33; // [esp+1Ch] [ebp-Ch]
	type_entity_0x6E8E* v34x; // [esp+20h] [ebp-8h]
	char v35; // [esp+24h] [ebp-4h]

	v33 = 0;
	v35 = 0;
	v1 = a1x->mail_0x5E_94.channel[3].source;
	v2x = Entities_EA3E4[v1];
	v34x = v2x;
	if (v2x < Entities_EA3E4[0])
		goto LABEL_35;
	if (v2x->class_0x3F_63 == 3)
	{
		SendSpellMessage_6D8B0(v1, 0xDu, 1);
		//v3 = 26 * a1x->mail_0x5E_94.dword_0x70_112;
		//v28 = (int *)& SPELLS_BEGIN_BUFFER_DA818[0x15a +0x2d2];
		//SPELLS_BEGIN_BUFFER_str[13].subspell[2].dword_2
		//v32 = (uint8_t*)& SPELLS_BEGIN_BUFFER_DA818[0x15a +0x2b8+v3];
		//SPELLS_BEGIN_BUFFER_str[13].subspell[a1x->mail_0x5E_94.dword_0x70_112].dword_2
		//v4 = SPELLS_BEGIN_BUFFER_DA818[0x15a +0x2b8+v3 + 24];
		v4 = SPELLS_BEGIN_BUFFER_str[13].subspell[a1x->mail_0x5E_94.channel[3].amount.dword].life_0x1A;
		if (v4)
		{
			if (v4 > 2u)
				goto LABEL_23;
			v6x = Entities_EA3E4[a1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58];
			v7x = Entities_EA3E4[v34x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58];
			v31x = Entities_EA3E4[v34x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58];
			if (v6x <= Entities_EA3E4[0] || v7x <= Entities_EA3E4[0] || (v8 = v6x->mana_0x90_144, v8 <= 0))
			{
				v35 = 1;
			}
			else
			{
				v9 = v8 * SPELLS_BEGIN_BUFFER_str[13].subspell[a1x->mail_0x5E_94.channel[3].amount.dword].subSpellIndex_2;
				v10x = v31x;
				v6x->mana_0x90_144 = v8 - v8 * SPELLS_BEGIN_BUFFER_str[13].subspell[a1x->mail_0x5E_94.channel[3].amount.dword].subSpellIndex_2 / 100;
				v11 = v10x->params_0x52_82.pitch * v10x->params_0x52_82.pitch;
				v33 = v9 / 100;
				v29 = Maths::IntegerSqrt_7277A(v11 + v31x->params_0x52_82.roll * v31x->params_0x52_82.roll);
				while (v33 > 0)
				{
					if (v33 <= 500)
						v30 = v33;
					else
						v30 = 500;
					v12 = v33 - v30;
					v26x = v31x->position_0x4C_76;
					v13 = 9377 * a1x->rand_0x14_20 + 9439;
					a1x->rand_0x14_20 = v13;
					v14 = v13 & 0x7FF;
					v33 = v12;
					MoveEntity_57FA0(&v26x, v13 & 0x7FF, 0, v29);
					v15 = GetTerrainAlt_10C40(&v26x);
					HIBYTE(v15) += 4;
					v26x.z = v15;
					v16x = CreateEntity_4A190(&v26x, 10, 39);
					v17x = v16x;
					if (v16x)
					{
						v16x->rand_0x14_20 = 9377 * v16x->rand_0x14_20 + 9439;
						v18 = (v14 + v16x->rand_0x14_20 % 0x71u - 56) & 0x7FF;
						v19 = v17x->rand_0x14_20;
						v17x->yaw_0x1C_28 = v18;
						v17x->rand_0x14_20 = 9377 * v19 + 9439;
						v17x->actSpeed_0x82_130 = v17x->rand_0x14_20 % 0x30u + 16;
						v20 = v17x->actSpeed_0x82_130;
						v21 = v17x->yaw_0x1C_28;
						v17x->axis_0x9A_154x.x = 0;
						v17x->axis_0x9A_154x.y = 0;
						v17x->param2_0x2C_44 = 128;
						MoveEntity_57FA0(&v17x->axis_0x9A_154x, v21, 0, v20);
						v17x->mana_0x90_144 = v30;
						if (SPELLS_BEGIN_BUFFER_str[13].subspell[a1x->mail_0x5E_94.channel[3].amount.dword].life_0x1A == 2)
							v17x->playerEntityIndex_0x94_148 = v34x - world_D41A0.entities_0x6E8E;
						else
							v17x->playerEntityIndex_0x94_148 = 0;
					}
				}
			}
			if (!v35)
				goto LABEL_23;
			v5 = SPELLS_BEGIN_BUFFER_str[13].subspell[a1x->mail_0x5E_94.channel[3].amount.dword].subSpellIndex_2;
		}
		else
		{
			v5 = SPELLS_BEGIN_BUFFER_str[13].subspell[a1x->mail_0x5E_94.channel[3].amount.dword].subSpellIndex_2;
			v35 = 1;
		}
		v33 = v5;
	}
LABEL_23:
	if (v35)
	{
		v22 = v33;
		if (v33)
		{
			v34x->mana_0x90_144 += v33;
			a1x->mana_0x90_144 -= v22;
		}
	}
	if (v34x->mana_0x90_144 < 0)
		v34x->mana_0x90_144 = 0;
	v23 = v34x->maxMana_0x8C_140;
	if (v34x->mana_0x90_144 > v23)
		v34x->mana_0x90_144 = v23;
	if (a1x->mana_0x90_144 < 0)
		a1x->mana_0x90_144 = 0;
	v24 = a1x->maxMana_0x8C_140;
	if (a1x->mana_0x90_144 > v24)
		a1x->mana_0x90_144 = v24;
	a1x->playerData_0xA4_164x->PlayerHitFrameTime_406 = 4;
	a1x->playerData_0xA4_164x->dword_0x18D_397 = 16;
	a1x->playerData_0xA4_164x->word_0x24C_588 = 64;
LABEL_35:
	SetWizardHitTimer_5EF70(a1x);
	resultx = a1x;
	a1x->mail_0x5E_94.channel[3].source = 0;
	return resultx;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000613D0) --------------------------------------------------------
void UpdateBalloonChain_613D0(type_entity_0x6E8E* a1x)//2423d0
{
	int i1; // eax
	type_entity_0x6E8E* i2x; // eax
	type_entity_0x6E8E* v2x; // ebx
	__int16 v3; // dx
	signed int v4; // esi
	type_entity_0x6E8E* v5x; // edx
	//int v6; // ebx
	//int v7; // ebx
	signed int v8; // edx
	type_entity_0x6E8E* v9x; // ebx
	//uint8_t* v10; // ST14_4
	__int16 v11; // ax
	char* v13; // [esp+0h] [ebp-24h]
	int v14; // [esp+4h] [ebp-20h]
	int v15; // [esp+Ch] [ebp-18h]
	char v16; // [esp+10h] [ebp-14h]
	type_entity_0x6E8E* v17x; // [esp+14h] [ebp-10h]
	type_entity_0x6E8E* v18x; // [esp+18h] [ebp-Ch]
	unsigned __int8 v19; // [esp+1Ch] [ebp-8h]
	unsigned __int8 v20; // [esp+20h] [ebp-4h]

	for (i1 = a1x->nextLinkedId_0x34_52; ; i1 = v2x->nextLinkedId_0x34_52)
	{
		v2x = Entities_EA3E4[i1];
		if (v2x == Entities_EA3E4[0])
			break;
		ReleaseEntity_57F20(Entities_EA3E4[i1]);
	}
	v3 = a1x->id_0x1A_26;
	a1x->nextLinkedId_0x34_52 = 0;
	if (v3 && a1x->param_0x10_16)
	{
		//LOWORD(i) = v3;
		v4 = a1x->param_0x10_16;
		v5x = Entities_EA3E4[v3];
		while (v4 > 0)
		{
			//LOWORD(i) = *(char *)(v4 + *(x_DWORD *)(v5 + 164) + 599);
			v16 = v5x->playerData_0xA4_164x->castleSpells_0x24E_590.at(9 + v4);
			if (v5x->playerData_0xA4_164x->castleSpells_0x24E_590.at(9 + v4))
				break;
			v4--;
		}
		if (v4)
		{
			/*v6 = 6 * v4;
			i = videoMode_180660 == 1 ? (signed int)*(unsigned __int8 *)(v6 + **filearray_2aa18c[24] + 4) >> 1 : *(unsigned __int8 *)(v6 + **filearray_2aa18c[24] + 4);
			v7 = 6 * v4;
			v8 = videoMode_180660 == 1 ? (signed int)*(unsigned __int8 *)(v7 + **filearray_2aa18c[24] + 5) >> 1 : *(unsigned __int8 *)(v7 + **filearray_2aa18c[24] + 5);
			*/
			//filearrayindex_MSPRD00DATTAB
			i1 = videoMode_180660 == 1 ? (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[v4].width_4 >> 1 : (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[v4].width_4;
			v8 = videoMode_180660 == 1 ? (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[v4].height_5 >> 1 : (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[v4].height_5;

			v20 = ((unsigned __int16)(a1x->position_0x4C_76.x + 128) >> 8) - (i1 >> 1);
			v19 = ((unsigned __int16)(a1x->position_0x4C_76.y + 128) >> 8) - (v8 >> 1);
			v15 = 0;
			v14 = (unsigned __int8)balloonChainTable_DB038[2 * v4];
			v18x = a1x;
			v13 = (char*)&balloonChainTable_DB038[18] + 2 * (unsigned __int8)balloonChainTable_DB038[1 + 2 * v4];
			//LOWORD(i) = 0;
			if (v14 > 0)
			{
				do
				{
					predictedAxis_EB398ar.x = (v20 + v13[0]) << 8;
					predictedAxis_EB398ar.y = (v19 + v13[1]) << 8;
					i2x = CreateEntity_4A190(&predictedAxis_EB398ar, 10, 79);
					v9x = i2x;
					v17x = i2x;
					if (!i2x)
						break;
					i2x->leaderId_0x32_50 = v18x - world_D41A0.entities_0x6E8E;
					v18x->nextLinkedId_0x34_52 = i2x - world_D41A0.entities_0x6E8E;
					i2x->nextLinkedId_0x34_52 = 0;
					i2x->id_0x1A_26 = a1x->id_0x1A_26;
					i2x->levelPar1_0x43_67 = v16;
					i2x->nextStage_0x4A_74 = v4;
					v11 = GetTerrainAlt_10C40(&i2x->position_0x4C_76);
					v9x->position_0x4C_76.z = v11;
					v9x->position_0x4C_76.z = v4 <= 1 ? v11 + 384 : v11 + 224;
					v18x = v17x;
					v15++;
					v13 += 2;
					//LOWORD(i2) = v15;
				} while (v15 < v14);
			}
		}
	}
	//return i;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3BC: using guessed type int **filearray_2aa18c[24];
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39A: using guessed type __int16 x_WORD_EB398ar[1];
// 180660: using guessed type __int16 videoMode_180660;

//----- (00061620) --------------------------------------------------------
char FindNearestManaForBalloon_61620(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//242620
{
	int32_t v2x; // edi
	type_entity_0x6E8E* v3x; // esi
	type_entity_0x6E8E* ix; // ebx
	int32_t v5x; // eax
	char v7; // [esp+0h] [ebp-4h]

	v2x = 0x10000;
	v3x = 0;
	v7 = 0;
	for (ix = gameState_D41A4.class3List_38519; ix > Entities_EA3E4[0]; ix = ix->next_0)
	{
		if (!ix->model_0x40_64 && ix != a1x)
		{
			v5x = IsBalloonNearCastle_61810(a1x, ix);
			if (v5x)
			{
				if (v5x < v2x)
				{
					v2x = v5x;
					v3x = ix;
				}
			}
		}
	}
	if (v3x == a2x)
		v7 = 1;
	return v7;
}
// 10000: using guessed type void /*__noreturn*/ sub_10000();
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00061810) --------------------------------------------------------
signed int IsBalloonNearCastle_61810(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//242810
{
	signed int v2; // ebx
	signed int v3; // esi
	__int16 v4; // ax

	v2 = 0;
	v3 = Maths::Distance3D_583F0(&a1x->position_0x4C_76, &a2x->position_0x4C_76);
	if (v3 < 5376)
	{
		v4 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &a2x->position_0x4C_76);
		if (Maths::AngleDifference_582B0(a1x->yaw_0x1C_28, v4) < 0x100u)
			v2 = v3;
	}
	return v2;
}
