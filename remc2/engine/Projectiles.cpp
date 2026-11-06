#include "Projectiles.h"
#include "EventsFunctions.h"
#include "Castle.h"
#include "Creatures.h"
#include "Effects.h"

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

char arcPoints_F01FE[34]; // fix it -  weak

int debugafter_215540 = 0;
int debugcounter_215540 = 0;
//----- (00034540) --------------------------------------------------------
void UpdateArcProjectile_34540(type_entity_0x6E8E* a1x)//215540
{
	//int v1; // ebx
	int v1x;
	int v2; // eax
	int v3; // edx
	int v4; // esi
	int v5; // edi
	signed int v6; // eax
	signed int v7; // ebx
	signed int v8; // ebx
	signed int v9; // eax
	int v10; // ebx
	int v11; // eax
	unsigned int v12; // ST14_4
	int v13; // ebx
	unsigned int v14; // eax
	int v15; // ebx
	int v16; // ebx
	uaxis_2d v17x; // ebx
	unsigned __int8 v18; // ah
	char v19; // dl
	signed int i; // eax
	int v21; // ebx
	axis_3d v23x; // [esp+0h] [ebp-4Ch]
	//__int16 v24; // [esp+4h] [ebp-48h]
	axis_3d v25x; // [esp+8h] [ebp-44h]
	//__int16 v26; // [esp+Ch] [ebp-40h]
	int v27; // [esp+10h] [ebp-3Ch]
	int v28; // [esp+14h] [ebp-38h]
	int v29; // [esp+18h] [ebp-34h]
	int v30; // [esp+1Ch] [ebp-30h]
	unsigned int v31; // [esp+20h] [ebp-2Ch]
	int v32; // [esp+24h] [ebp-28h]
	int v33; // [esp+28h] [ebp-24h]
	int v34; // [esp+2Ch] [ebp-20h]
	int v35; // [esp+30h] [ebp-1Ch]
	int v36; // [esp+34h] [ebp-18h]
	uaxis_2d v37x; // [esp+38h] [ebp-14h]
	uaxis_2d v38x; // [esp+3Ch] [ebp-10h]
	uaxis_2d v39x; // [esp+40h] [ebp-Ch]
	unsigned __int8 v40; // [esp+44h] [ebp-8h]
	unsigned __int8 v41; // [esp+48h] [ebp-4h]

	debugafter_215540 = 1;

	v28 = (a1x->levelPar3_0x46_70 >> 4 << 8) + 512;
	v27 = ((a1x->levelPar3_0x46_70 & 0xF) << 8) + 512;
	v29 = Maths::DistanceXY_58490(&a1x->position_0x4C_76, &a1x->axis_0x9A_154x) / 0x55;
	v34 = (unsigned __int16)Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &a1x->axis_0x9A_154x);
	v33 = 0;
	v25x = a1x->position_0x4C_76;
	while (v33 < 32)
	{
		//LOBYTE(v39) = v25x.x>>8;
		//HIBYTE(v39) = v25x.y>>8;
		v39x._axis_2d.x = v25x.x >> 8;
		v39x._axis_2d.y = v25x.y >> 8;
		v1x = (mapHeightmap_11B4E0[v39x.word] + secondHeightmap_14B4E0[v39x.word]) / 2;
		if (v1x < 0)
			v1x = 0;
		if (v1x > 254)
			v1x = 254;
		arcPoints_F01FE[2 + v33++] = v1x;
		MoveEntity_57FA0(&v25x, v34, 0, 85);
	}
	v30 = 0;//adress 215637
	v23x = a1x->position_0x4C_76;
	while (v30 < v29)
	{
		v2 = v28 + v30 * ((v27 - v28) / v29);
		v36 = v2 * v2;
		v3 = 2 * v2 + 128;
		v40 = (unsigned __int16)(v23x.x + 128) >> 8;
		v35 = (unsigned __int8)arcPoints_F01FE[2 + 0];
		v4 = v3 >> 8;
		v3 >>= 9;
		v41 = (unsigned __int16)(v23x.y + 128) >> 8;
		//LOBYTE(v37) = v40 - v3;
		//HIBYTE(v37) = v41 - v3;
		v37x._axis_2d.x = v40 - v3;
		v37x._axis_2d.y = v41 - v3;
		v5 = 0;
		//LOWORD(v38) = v37;
		v38x.word = v37x.word;
		while (v5 < v4)
		{
			v32 = 0;
			//v39 = v38;
			v39x.word = v38x.word;
			while (v4 > v32)
			{
				//adress 2156d8
				//if (debugcounter_215540 >= 0x8cc)
				if (debugcounter_215540 >= 0xbf)
				{
					debugcounter_215540++;
					debugcounter_215540--;
				}
				/*uint8_t origbyte20 = 0;
				uint8_t remakebyte20 = 0;
				int comp20;

				comp20 = compare_with_sequence((char*)"002156D8-002C1200", (uint8_t*)& arcPoints_F01FE[2 + 0], 0x2c1200, debugcounter_215540, 0x20, 0x20, &origbyte20, &remakebyte20);
				*/

				debugcounter_215540++;

				v6 = abs(v39x._axis_2d.x - v40);
				v7 = v6;
				if (v6 >= 0x80)
					v7 = abs(v6 - 256);
				v8 = v7 << 8;
				v9 = abs(v39x._axis_2d.y - v41);
				if (v9 >= 0x80)
					v9 = abs(v9 - 256);
				v10 = v8 * v8;
				v11 = (v9 << 8) * (v9 << 8);
				if (v10 + v11 <= v36)
				{
					v12 = v36 - v10 - v11;
					v13 = v35;
					v14 = Maths::IntegerSqrt_7277A(v12) >> 5;
					v15 = v13 - v14;
					v31 = v14;
					if (v15 < 0)
						v15 = 0;
					if (v15 > 254)
						v15 = 254;
					if (mapHeightmap_11B4E0[v39x.word] > v15)
						FindFreeTileInArea_570F0(v39x._axis_2d.x, v39x._axis_2d.y, v15, 0, 0, 1);
					v16 = v31 + v35;
					if ((signed int)(v31 + v35) < 0)
						v16 = 0;
					if (v16 > 254)
						v16 = 254;
					if ((unsigned __int8)secondHeightmap_14B4E0[v39x.word] < v16)
						secondHeightmap_14B4E0[v39x.word] = v16;
				}
				//v17 = v39;
				v17x.word = v39x.word;
				v18 = mapHeightmap_11B4E0[v39x.word];
				if (secondHeightmap_14B4E0[v39x.word] > v18)
				{
					mapAngle_13B4E0[v39x.word] &= 0xF7u;
				}
				else
				{
					v19 = mapAngle_13B4E0[v39x.word] | 8;
					secondHeightmap_14B4E0[v39x.word] = v18 - 1;
					mapAngle_13B4E0[v17x.word] = v19;
				}
				v32++;
				//LOBYTE(v39)++;
				v39x._axis_2d.x++;
			}
			v5++;
			//BYTE1(v38)++;
			v38x._axis_2d.y++;
		}
		UpdateTileAngles_34B00(v37x._axis_2d.x - 1, v37x._axis_2d.y - 1, v4 + 1, v4 + 1);
		MoveEntity_57FA0(&v23x, v34, 0, 85);
		for (i = 1; i < 32; i++)
			arcPoints_F01FE[i + 1] = arcPoints_F01FE[2 + i];
		//LOBYTE(v39) = v25x.x >> 8;
		//HIBYTE(v39) = v25x.y >> 8;
		v39x._axis_2d.x = v25x.x >> 8;
		v39x._axis_2d.y = v25x.y >> 8;
		v21 = (mapHeightmap_11B4E0[v39x.word] + secondHeightmap_14B4E0[v39x.word]) / 2;
		if (v21 < 0)
			v21 = 0;
		if (v21 > 254)
			v21 = 254;
		arcPoints_F01FE[2 + 31] = v21;
		v30++;
		MoveEntity_57FA0(&v25x, v34, 0, 85);
	}
	DisableEntityDrawing04_57F10(a1x);
}
// F021F: using guessed type char x_BYTE_F021F;

//----- (0004D2E0) --------------------------------------------------------
type_entity_0x6E8E* AddFireballProjectile_4D2E0(axis_3d* position)//22e2e0 //tex 463
{
	type_entity_0x6E8E* v1x = NewEvent_4A050();
	if (v1x)
	{
		v1x->actionIndex_0x45_69 = 0;
		v1x->class_0x3F_63 = 9;
		v1x->model_0x40_64 = 0;
		v1x->actSpeed_0x82_130 = 384;
		v1x->minSpeed_0x84_132 = 384;
		v1x->mana_0x90_144 = 50;
		v1x->maxLife_0x4 = 0x2000 / v1x->actSpeed_0x82_130;
		v1x->typeParams_0xA0_160x = &str_D7BD6[64]; //(type_str_160*)&unk_D7BD6[0x880];
		v1x->flags_0xc_12_15.byte[0] &= 0xF7;
		AddEventToMap_57D70(v1x, position);
		CopyMaxLifeToLife_49A20(v1x);
		SetEntityIndexAndRot_49CD0(v1x, 340);
		AddDynamicLight_847D0(v1x, 128, 1, 0);
	}
	return v1x;
}

//----- (0004D380) --------------------------------------------------------
type_entity_0x6E8E* AddFireStormProjectile_4D380(axis_3d* position)//22e380
{
	type_entity_0x6E8E* event = AddFireballProjectile_4D2E0(position);
	if (event)
	{
		event->actionIndex_0x45_69 = 29;
		event->model_0x40_64 = 28;
	}
	return event;
}

//----- (0004D3B0) --------------------------------------------------------
type_entity_0x6E8E* AddPossessionProjectile_4D3B0(axis_3d* position)//22e3b0 //tex 144
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 1;
		event->class_0x3F_63 = 9;
		event->model_0x40_64 = 1;
		event->actSpeed_0x82_130 = 384;
		event->minSpeed_0x84_132 = 384;
		event->mana_0x90_144 = 50;
		event->typeParams_0xA0_160x = &str_D7BD6[61]; //(type_str_160*)&unk_D7BD6[0x81a];
		event->maxLife_0x4 = 4096 / event->actSpeed_0x82_130;
		event->xtype_0x41_65 = 10;
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 209);
		SetEntityShiftRot_49EA0(event, 2 * event->params_0x52_82.pitch, 5 * event->params_0x52_82.fov / 2);
	}
	return event;
}

//----- (0004D470) --------------------------------------------------------
type_entity_0x6E8E* AddEarthquakeProjectile_4D470(axis_3d* position)//22e470 //tex 146
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 2;
		event->class_0x3F_63 = 9;
		event->model_0x40_64 = 2;
		event->actSpeed_0x82_130 = 384;
		event->minSpeed_0x84_132 = 384;
		event->mana_0x90_144 = 50;
		event->maxLife_0x4 = 0x2000 / event->actSpeed_0x82_130;
		event->typeParams_0xA0_160x = &str_D7BD6[60]; //(type_str_160*)&unk_D7BD6[0x7f8];
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 211);
	}
	return event;
}

//----- (0004D500) --------------------------------------------------------
type_entity_0x6E8E* AddMeteorProjectile_4D500(axis_3d* position)//22e500 //tex 64
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 3;
		event->class_0x3F_63 = 9;
		event->model_0x40_64 = 3;
		event->actSpeed_0x82_130 = 384;
		event->minSpeed_0x84_132 = 384;
		event->mana_0x90_144 = 50;
		event->maxLife_0x4 = 0x2000 / event->actSpeed_0x82_130;
		event->typeParams_0xA0_160x = &str_D7BD6[60]; //(type_str_160*)&unk_D7BD6[0x7F8];
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 76);
	}
	return event;
}

//----- (0004D590) --------------------------------------------------------
type_entity_0x6E8E* AddVolcanoProjectile_4D590(axis_3d* position)//22e590 //tex 145
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 4;
		event->class_0x3F_63 = 9;
		event->model_0x40_64 = 4;
		event->actSpeed_0x82_130 = 384;
		event->minSpeed_0x84_132 = 384;
		event->mana_0x90_144 = 50;
		event->maxLife_0x4 = 0x2000 / event->actSpeed_0x82_130;
		event->typeParams_0xA0_160x = &str_D7BD6[60]; //(type_str_160*)&unk_D7BD6[0x7F8];
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 210);
	}
	return event;
}

//----- (0004D620) --------------------------------------------------------
type_entity_0x6E8E* AddCraterProjectile_4D620(axis_3d* position)//22e620 //tex 146
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 5;
		event->class_0x3F_63 = 9;
		event->model_0x40_64 = 5;
		event->actSpeed_0x82_130 = 384;
		event->minSpeed_0x84_132 = 384;
		event->mana_0x90_144 = 50;
		event->maxLife_0x4 = 0x2000 / event->actSpeed_0x82_130;
		event->typeParams_0xA0_160x = &str_D7BD6[60]; //(type_str_160*)&unk_D7BD6[0x7f8];
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 211);
	}
	return event;
}

//----- (0004D6B0) --------------------------------------------------------
type_entity_0x6E8E* AddBlueOrbProjectile_4D6B0(axis_3d* position)//22e6b0 //tex 147
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 6;
		event->class_0x3F_63 = 9;
		event->model_0x40_64 = 6;
		event->actSpeed_0x82_130 = 384;
		event->minSpeed_0x84_132 = 384;
		event->mana_0x90_144 = 50;
		event->maxLife_0x4 = 0x2000 / event->actSpeed_0x82_130;
		event->typeParams_0xA0_160x = &str_D7BD6[60]; //(type_str_160*)&unk_D7BD6[0x7f8];
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 212);
	}
	return event;
}

//----- (0004D740) --------------------------------------------------------
type_entity_0x6E8E* AddDuelProjectile_4D740(axis_3d* position)//22e740 //tex 148
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 7;
		event->class_0x3F_63 = 9;
		event->model_0x40_64 = 7;
		event->actSpeed_0x82_130 = 384;
		event->minSpeed_0x84_132 = 384;
		event->mana_0x90_144 = 50;
		event->maxLife_0x4 = 0x2000 / event->actSpeed_0x82_130;
		event->typeParams_0xA0_160x = &str_D7BD6[60]; //(type_str_160*)&unk_D7BD6[0x7f8];
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 213);
	}
	return event;
}

//----- (0004D7D0) --------------------------------------------------------
type_entity_0x6E8E* AddStealManaProjectile_4D7D0(axis_3d* position)//22e7d0 //tex 149
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 8;
		event->class_0x3F_63 = 9;
		event->model_0x40_64 = 8;
		event->actSpeed_0x82_130 = 384;
		event->minSpeed_0x84_132 = 384;
		event->mana_0x90_144 = 50;
		event->maxLife_0x4 = 0x2000 / event->actSpeed_0x82_130;
		event->typeParams_0xA0_160x = &str_D7BD6[63]; //(type_str_160*)&unk_D7BD6[0x85e];
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 214);
	}
	return event;
}

//----- (0004D860) --------------------------------------------------------
type_entity_0x6E8E* AddLightningProjectile_4D860(axis_3d* position)//22e860 //tex 151
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 9;
		event->class_0x3F_63 = 9;
		event->model_0x40_64 = 9;
		event->actSpeed_0x82_130 = 384;
		event->minSpeed_0x84_132 = 384;
		event->mana_0x90_144 = 50;
		event->maxLife_0x4 = 3584 / event->actSpeed_0x82_130;
		event->typeParams_0xA0_160x = &str_D7BD6[63]; //(type_str_160*)&unk_D7BD6[0x85e];
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 216);
		AddDynamicLight_847D0(event, 128, 9, 0);
	}
	return event;
}

//----- (0004D900) --------------------------------------------------------
type_entity_0x6E8E* AddCastleProjectile_4D900(axis_3d* position)//22e900 //tex 61
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 10;
		event->class_0x3F_63 = 9;
		event->model_0x40_64 = 10;
		event->actSpeed_0x82_130 = 384;
		event->minSpeed_0x84_132 = 384;
		event->mana_0x90_144 = 50;
		event->maxLife_0x4 = 0x2000 / event->actSpeed_0x82_130;
		event->typeParams_0xA0_160x = &str_D7BD6[60]; //(type_str_160*)&unk_D7BD6[0x7f8];
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 18);
	}
	return event;
}

//----- (0004D990) --------------------------------------------------------
type_entity_0x6E8E* AddBoneProjectile_4D990(axis_3d* position)//22e990 //tex 260
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 11;
		event->class_0x3F_63 = 9;
		event->model_0x40_64 = 11;
		event->actSpeed_0x82_130 = 384;
		event->minSpeed_0x84_132 = 384;
		event->mana_0x90_144 = 50;
		event->maxLife_0x4 = 0x2000 / event->actSpeed_0x82_130;
		event->typeParams_0xA0_160x = &str_D7BD6[60]; //(type_str_160*)&unk_D7BD6[0x7f8];
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 281);
	}
	return event;
}

//----- (0004DA20) --------------------------------------------------------
type_entity_0x6E8E* AddThunderboltProjectile_4DA20(axis_3d* position)//22ea20 //tex 151
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 12;
		event->class_0x3F_63 = 9;
		event->model_0x40_64 = 12;
		event->actSpeed_0x82_130 = 384;
		event->minSpeed_0x84_132 = 384;
		event->mana_0x90_144 = 50;
		event->maxLife_0x4 = 2048 / event->actSpeed_0x82_130;
		event->typeParams_0xA0_160x = &str_D7BD6[60]; //(type_str_160*)&unk_D7BD6[0x7f8];
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 216);
	}
	return event;
}

//----- (0004DAB0) --------------------------------------------------------
type_entity_0x6E8E* AddArrow_4DAB0(axis_3d* position)//22eab0
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 0xD;
		event->class_0x3F_63 = 9;
		event->model_0x40_64 = 0xD;
		event->actSpeed_0x82_130 = 384;
		event->minSpeed_0x84_132 = 384;
		event->maxLife_0x4 = 5120 / event->actSpeed_0x82_130;
		event->flags_0xc_12_15.byte[0] &= 0xF7u;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntitySpriteDoubleSize_49E10(event, 195);
	}
	return event;
}

//----- (0004DBC0) --------------------------------------------------------
type_entity_0x6E8E* AddGreenBlobProjectile_4DBC0(axis_3d* position)//22ebc0 //tex 60
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 15;
		event->class_0x3F_63 = 9;
		event->model_0x40_64 = 14;
		event->actSpeed_0x82_130 = 128;
		event->minSpeed_0x84_132 = 128;
		event->maxLife_0x4 = 4096 / event->actSpeed_0x82_130;
		event->flags_0xc_12_15.byte[0] &= 0xF7u;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 196);
	}
	return event;
}

//----- (0004DC40) --------------------------------------------------------
type_entity_0x6E8E* AddManticoreStunProjectile_4DC40(axis_3d* position)//22ec40 //tex 60
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 21;
		event->class_0x3F_63 = 9;
		event->model_0x40_64 = 20;
		event->actSpeed_0x82_130 = 394;
		event->minSpeed_0x84_132 = event->actSpeed_0x82_130;
		event->maxLife_0x4 = 7680 / event->actSpeed_0x82_130;
		event->flags_0xc_12_15.byte[0] &= 0xF7u;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 196);
	}
	return event;
}

//----- (0004DCC0) --------------------------------------------------------
type_entity_0x6E8E* AddSpiderWebProjectile_4DCC0(axis_3d* position)//22ecc0 //tex 419
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 22;
		event->class_0x3F_63 = 9;
		event->model_0x40_64 = 21;
		event->actSpeed_0x82_130 = 394;
		event->minSpeed_0x84_132 = event->actSpeed_0x82_130;
		event->maxLife_0x4 = 7680 / event->actSpeed_0x82_130;
		event->flags_0xc_12_15.byte[0] &= 0xF7u;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 319);
		SetEntityShiftRot_49EA0(event, 256, 512);
	}
	return event;
}

//----- (0004DD50) --------------------------------------------------------
type_entity_0x6E8E* AddPurpleOrbProjectile_4DD50(axis_3d* position)//22ed50 //tex 150
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->maxLife_0x4 = 80;
		event->actionIndex_0x45_69 = 16;
		event->class_0x3F_63 = 9;
		event->model_0x40_64 = 15;
		event->actSpeed_0x82_130 = 128;
		event->minSpeed_0x84_132 = 128;
		event->flags_0xc_12_15.byte[0] &= 0xF7u;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 215);
	}
	return event;
}

//----- (0004DDD0) --------------------------------------------------------
type_entity_0x6E8E* AddPossessionBeam_4DDD0(axis_3d* position)//22edd0 //tex 144
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 18;
		event->class_0x3F_63 = 9;
		event->model_0x40_64 = 17;
		event->actSpeed_0x82_130 = 384;
		event->minSpeed_0x84_132 = 384;
		event->mana_0x90_144 = 50;
		event->maxLife_0x4 = 4096 / event->actSpeed_0x82_130;
		event->typeParams_0xA0_160x = &str_D7BD6[61]; //(type_str_160*)&unk_D7BD6[0x81a];
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 209);
		SetEntityShiftRot_49EA0(event, 2 * event->params_0x52_82.pitch, 2 * event->params_0x52_82.fov);
	}
	return event;
}

//----- (0004DEA0) --------------------------------------------------------
type_entity_0x6E8E* AddGravityWellProjectile_4DEA0(axis_3d* position)//22eea0 //tex 146
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 23;
		event->class_0x3F_63 = 9;
		event->model_0x40_64 = 22;
		event->actSpeed_0x82_130 = 384;
		event->minSpeed_0x84_132 = 384;
		event->mana_0x90_144 = 50;
		event->maxLife_0x4 = 0x2000 / event->actSpeed_0x82_130;
		event->typeParams_0xA0_160x = &str_D7BD6[60]; //(type_str_160*)&unk_D7BD6[0x7f8];
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 211);
	}
	return event;
}

//----- (0004DF30) --------------------------------------------------------
type_entity_0x6E8E* AddPurpleOrb2Projectile_4DF30(axis_3d* position)//22ef30 //tex 150
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 28;
		event->class_0x3F_63 = 9;
		event->model_0x40_64 = 27;
		event->actSpeed_0x82_130 = 384;
		event->minSpeed_0x84_132 = 384;
		event->mana_0x90_144 = 50;
		event->maxLife_0x4 = 0x2000 / event->actSpeed_0x82_130;
		event->typeParams_0xA0_160x = &str_D7BD6[60]; //(type_str_160*)&unk_D7BD6[0x7f8];
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 215);
	}
	return event;
}

//----- (0004DFC0) --------------------------------------------------------
type_entity_0x6E8E* AddTremorProjectile_4DFC0(axis_3d* position)//22efc0 //tex 146
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 24;
		event->class_0x3F_63 = 9;
		event->model_0x40_64 = 23;
		event->actSpeed_0x82_130 = 384;
		event->minSpeed_0x84_132 = 384;
		event->mana_0x90_144 = 50;
		event->maxLife_0x4 = 0x2000 / event->actSpeed_0x82_130;
		event->typeParams_0xA0_160x = &str_D7BD6[60]; //(type_str_160*)&unk_D7BD6[0x7f8];
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 211);
	}
	return event;
}

//----- (0004E050) --------------------------------------------------------
type_entity_0x6E8E* AddSummonArmyProjectile_4E050(axis_3d* position)//22f050 //tex 260
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 25;
		event->class_0x3F_63 = 9;
		event->model_0x40_64 = 24;
		event->actSpeed_0x82_130 = 384;
		event->minSpeed_0x84_132 = 384;
		event->mana_0x90_144 = 50;
		event->maxLife_0x4 = 0x2000 / event->actSpeed_0x82_130;
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		event->typeParams_0xA0_160x = &str_D7BD6[60]; //(type_str_160*)&unk_D7BD6[0x7f8];
		event->maxLife_0x4 = event->maxLife_0x4 & 0xFC;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 281);
	}
	return event;
}

//----- (0004E0F0) --------------------------------------------------------
type_entity_0x6E8E* AddAllianceProjectile_4E0F0(axis_3d* position)//22f0f0 //tex 421
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 26;
		event->class_0x3F_63 = 9;
		event->model_0x40_64 = 25;
		event->actSpeed_0x82_130 = 384;
		event->minSpeed_0x84_132 = 384;
		event->mana_0x90_144 = 50;
		event->maxLife_0x4 = 4096 / event->actSpeed_0x82_130;
		event->typeParams_0xA0_160x = &str_D7BD6[61]; //(type_str_160*)&unk_D7BD6[0x81a];
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 321);
	}
	return event;
}

//----- (0004E180) --------------------------------------------------------
type_entity_0x6E8E* AddWhirlwindProjectile_4E180(axis_3d* position)//22f180 //tex 420
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 27;
		event->class_0x3F_63 = 9;
		event->model_0x40_64 = 26;
		event->actSpeed_0x82_130 = 384;
		event->minSpeed_0x84_132 = 384;
		event->mana_0x90_144 = 50;
		event->maxLife_0x4 = 0x2000 / event->actSpeed_0x82_130;
		event->typeParams_0xA0_160x = &str_D7BD6[60]; //(type_str_160*)&unk_D7BD6[0x7f8];
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 320);
	}
	return event;
}

//----- (0004E210) --------------------------------------------------------
type_entity_0x6E8E* AddCaveInProjectile_4E210(axis_3d* position)//22f210 //tex 146
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 31;
		event->class_0x3F_63 = 9;
		event->model_0x40_64 = 30;
		event->actSpeed_0x82_130 = 384;
		event->minSpeed_0x84_132 = 384;
		event->mana_0x90_144 = 50;
		event->maxLife_0x4 = 0x2000 / event->actSpeed_0x82_130;
		event->typeParams_0xA0_160x = &str_D7BD6[60]; //(type_str_160*)&unk_D7BD6[0x7f8];
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 211);
	}
	return event;
}

//----- (0004E2A0) --------------------------------------------------------
type_entity_0x6E8E* AddMagicMineProjectile_4E2A0(axis_3d* position)//22f2a0 //tex 80
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 30;
		event->class_0x3F_63 = 9;
		event->model_0x40_64 = 29;
		event->actSpeed_0x82_130 = 384;
		event->minSpeed_0x84_132 = 384;
		event->maxLife_0x4 = 10;
		event->mana_0x90_144 = 50;
		event->typeParams_0xA0_160x = &str_D7BD6[60]; //(type_str_160*)&unk_D7BD6[0x7f8];
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 66);
	}
	return event;
}

//----- (0004FB20) --------------------------------------------------------
type_entity_0x6E8E* AddArcProjectile_4FB20(axis_3d* position)//230b20
{
	type_entity_0x6E8E* event; // eax
	if (!isCaveLevel_D41B6)
		return 0;
	event = NewEvent_4A050();
	if (event)
	{
		event->maxLife_0x4 = 0;
		event->actionIndex_0x45_69 = 0x58;
		event->class_0x3F_63 = 0xA;
		event->model_0x40_64 = 0x51;
		event->position_0x4C_76 = *position;
		event->actSpeed_0x82_130 = 256;
		event->levelPar3_0x46_70 = 2;
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		CopyMaxLifeToLife_49A20(event);
		return event;
	}
	return 0;
}

//----- (000652C0) --------------------------------------------------------
void MoveProjectile_652C0(type_entity_0x6E8E* a1x)//2462c0
{
	char v1; // ah
	__int16 v2; // dx
	__int16 v3; // cx
	__int16 v4; // di
	__int16 v5; // ax
	signed int v6; // esi
	signed int v7; // ecx
	signed int v8; // ecx
	__int16 v9; // ax
	unsigned int v10; // edx
	__int16 v11; // di
	__int16 v12; // ax
	int v13; // eax
	int v14; // edx

	if (CommandLineParams.DoDebugSequences()) {
		//add_compare(0x2462c4,CommandLineParams.DoDebugafterload(),0x8f);
	}

	v1 = a1x->flags_0xc_12_15.byte[1];
	if (v1 & 8)
	{
		a1x->flags_0xc_12_15.byte[1] = v1 & 0xF7;
	}
	else
	{
		predictedAxis_EB398ar = a1x->position_0x4C_76;
		if ((int16_t)a1x->position_0x4C_76.z > (signed __int16)GetTerrainAlt_10C40(&a1x->position_0x4C_76))
		{
			v2 = a1x->actSpeed_0x82_130;
			if (v2)
				MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, 0, v2);
		}
		else
		{
			a1x->actSpeed_0x82_130 = 0;
			BounceProjectileOffTerrain_654B0(a1x);
		}
		CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
		v3 = a1x->actSpeed_0x82_130;
		if (v3)
			a1x->actSpeed_0x82_130 = v3 - 1;
		v4 = a1x->param2_0x2C_44 + a1x->position_0x4C_76.z;
		v5 = a1x->param2_0x2C_44 - 24;
		a1x->param2_0x2C_44 = v5;
		a1x->position_0x4C_76.z = v4;
		if (v5 < -192)
			a1x->param2_0x2C_44 = -192;
		if (a1x->param2_0x2C_44 > 192)
			a1x->param2_0x2C_44 = 192;
		v6 = (signed __int16)GetTerrainAlt_10C40(&a1x->position_0x4C_76);
		if ((int16_t)a1x->position_0x4C_76.z <= v6)
			a1x->position_0x4C_76.z = v6;
		if (a1x->mail_0x5E_94.channel[0].source)
		{
			if ((int16_t)a1x->position_0x4C_76.z <= v6)
			{
				v7 = a1x->mail_0x5E_94.channel[0].amount.dword >> 2;
				if (v7 < 2)
					v7 = 2;
				if (v7 > 192)
					v7 = 192;
				a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
				v8 = a1x->rand_0x14_20 % (unsigned int)v7 + v7;
				v9 = 9377 * a1x->rand_0x14_20;
				a1x->param2_0x2C_44 = v8;
				a1x->rand_0x14_20 = v9 + 9439;
				v10 = a1x->rand_0x14_20 % (unsigned int)(v8 >> 1);
				a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
				v11 = a1x->position_0x4C_76.z;
				a1x->yaw_0x1C_28 = a1x->rand_0x14_20 & 0x7FF;
				v12 = a1x->param2_0x2C_44;
				a1x->actSpeed_0x82_130 = v10 + 1;
				a1x->position_0x4C_76.z = v12 + v11;
			}
			v13 = a1x->mail_0x5E_94.channel[0].amount.dword;
			v14 = a1x->life_0x8;
			a1x->mail_0x5E_94.channel[0].source = 0;
			a1x->life_0x8 = v14 - v13;
		}
		if (a1x->life_0x8 < 0)
		{
			CreateEntity_4A190(&a1x->position_0x4C_76, 10, 13);
			goto LABEL_27;
		}
		if ((int16_t)a1x->position_0x4C_76.z <= v6 && TerrainTileIsWater_104D0(&a1x->position_0x4C_76) == 1)
		{
			CreateEntity_4A190(&a1x->position_0x4C_76, 10, 5);
		LABEL_27:
			DisableEntityDrawing04_57F10(a1x);
			return;
		}
	}
}
// EB398: using guessed type __int16 x_WORD_EB398;

//----- (000654B0) --------------------------------------------------------
void BounceProjectileOffTerrain_654B0(/*axis_3d* a1x,*/ type_entity_0x6E8E* a2x)//2464b0
{
	int result; // eax
	uint32_t v3; // esi
	uint32_t v4; // eax
	axis_3d v5x; // [esp+0h] [ebp-1Ch]
	//__int16 v6; // [esp+4h] [ebp-18h]
	axis_3d v7x; // [esp+8h] [ebp-14h]
	//__int16 v8; // [esp+Ch] [ebp-10h]
	int v9; // [esp+10h] [ebp-Ch]
	uint32_t v10; // [esp+14h] [ebp-8h]
	int v11; // [esp+18h] [ebp-4h]

	result = GetTileSlope_1B7A0(&predictedAxis_EB398ar);
	if (result > 20)
	{
		v9 = 0;
		v5x = predictedAxis_EB398ar;
		result = a2x->yaw_0x1C_28;
		v10 = 0x10000;
		while (1)
		{
			v11 = result;
			if (v9 >= 8)
				break;
			v7x = predictedAxis_EB398ar;
			MoveEntity_57FA0(&v7x, v11, 0, 64);
			v3 = v10;
			v4 = GetTerrainAlt_10C40(&v7x);
			if (v4 < v3)
			{
				v10 = v4;
				v5x = v7x;
			}
			result = ((x_WORD)v11 + 256) & 0x7FF;
			v9++;
		}
		//a1x = &v5x;
		predictedAxis_EB398ar = v5x;
	}
	//return result;
}
// 10000: using guessed type void /*__noreturn*/ sub_10000();

//----- (000655C0) --------------------------------------------------------
void AimAtEntity_655C0(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//2465c0
{
	RaiseToEntityCentre_65580(a2x);
	a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &a2x->position_0x4C_76);
	a1x->fov_0x22_34 = Maths::GetPitchAngle_58210(&a1x->position_0x4C_76, &a2x->position_0x4C_76);
	LowerFromEntityCentre_655A0(a2x);
}

//----- (00065610) --------------------------------------------------------
void AimAtEntityLeading_65610(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//246610
{
	type_entity_0x6E8E* v2x; // ebx
	__int16 v3; // ax
	//int v4; // esi
	__int16 v5; // ax
	//int v6; // esi
	__int16 v7; // ax

	v2x = a1x;
	RaiseToEntityCentre_65580(a2x);
	a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &a2x->position_0x4C_76);
	v3 = Maths::GetPitchAngle_58210(&a1x->position_0x4C_76, &a2x->position_0x4C_76);
	//v4 = a1x->dword_0xA0_160;
	a1x->fov_0x22_34 = v3;
	v5 = v2x->yaw_0x1C_28
		+ Maths::TurnTowardsAngle_58350(v2x->yaw_0x1C_28, v2x->roll_0x20_32, a1x->typeParams_0xA0_160x->yawTurnLimit_160_0x4_4, a1x->typeParams_0xA0_160x->subtype_160_0x2_2);
	HIBYTE(v5) &= 7u;
	//v6 = a1x->dword_0xA0_160;
	a1x->yaw_0x1C_28 = v5;
	v7 = v2x->pitch_0x1E_30
		+ Maths::TurnTowardsAngle_58350(v2x->pitch_0x1E_30, v2x->fov_0x22_34, a1x->typeParams_0xA0_160x->pitchTurnLimit_160_0x8_8, a1x->typeParams_0xA0_160x->pitchTurnSpeed_160_0x6_6);
	HIBYTE(v7) &= 7u;
	a1x->pitch_0x1E_30 = v7;
	LowerFromEntityCentre_655A0(a2x);
}

//----- (000656D0) --------------------------------------------------------
__int16 AimAtEntityDirect_656D0(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//2466d0
{
	type_entity_0x6E8E* v2x; // ebx
	__int16 v3; // ax
	//int v4; // esi
	__int16 v5; // ax
	//int v6; // esi
	__int16 result; // ax

	v2x = a1x;
	a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &a2x->position_0x4C_76);
	v3 = Maths::GetPitchAngle_58210(&a1x->position_0x4C_76, &a2x->position_0x4C_76);
	//v4 = a1x->dword_0xA0_160;
	a1x->fov_0x22_34 = v3;
	v5 = v2x->yaw_0x1C_28
		+ Maths::TurnTowardsAngle_58350(v2x->yaw_0x1C_28, v2x->roll_0x20_32, a1x->typeParams_0xA0_160x->yawTurnLimit_160_0x4_4, a1x->typeParams_0xA0_160x->subtype_160_0x2_2);
	HIBYTE(v5) &= 7u;
	//v6 = a1x->dword_0xA0_160;
	a1x->yaw_0x1C_28 = v5;
	result = v2x->pitch_0x1E_30
		+ Maths::TurnTowardsAngle_58350(v2x->pitch_0x1E_30, v2x->fov_0x22_34, a1x->typeParams_0xA0_160x->pitchTurnLimit_160_0x8_8, a1x->typeParams_0xA0_160x->pitchTurnSpeed_160_0x6_6);
	HIBYTE(result) &= 7u;
	a1x->pitch_0x1E_30 = result;
	return result;
}

//----- (00065780) --------------------------------------------------------
void SetProjectileDamage_65780(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x, type_entity_0x6E8E* a3x)//246780
{
	type_entity_0x6E8E* v3x; // eax
	//int v4; // ebx

	//LOBYTE(v3) = a1x->byte_0x40_64;
	if (a1x->model_0x40_64 < 0xCu)
	{
		if (a1x->model_0x40_64 < 3u)
		{
			if (a1x->model_0x40_64 > 1u)
				return;
		}
		else if (a1x->model_0x40_64 > 3u && (a1x->model_0x40_64 < 7u || a1x->model_0x40_64 > 9u))
		{
			return;
		}
	}
	else if (a1x->model_0x40_64 > 0xCu)
	{
		if (a1x->model_0x40_64 < 0x19u)
		{
			if (a1x->model_0x40_64 != 17)
				return;
		}
		else if (a1x->model_0x40_64 > 0x19u && a1x->model_0x40_64 != 28)
		{
			return;
		}
	}
	v3x = Entities_EA3E4[a1x->id_0x1A_26];
	if (v3x->class_0x3F_63 == 3 && !v3x->model_0x40_64)
	{
		v3x->playerData_0xA4_164x->shotsFired_0x165_357++;
		if (a2x > Entities_EA3E4[0] && a3x > Entities_EA3E4[0] && a2x->id_0x1A_26 == a3x->id_0x1A_26)
		{
			//v4 = v3x->dword_0xA4_164;
			if (v3x->playerData_0xA4_164x)
				v3x->playerData_0xA4_164x->shotsHit_0x169_361++;
		}
	}
	//return v3;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00065820) --------------------------------------------------------
type_entity_0x6E8E* UpdateProjectileTargeting_65820(type_entity_0x6E8E* a1x)//246820
{
	type_entity_0x6E8E* v1x; // eax
	char v2; // dl
	signed __int16 v3; // ax
	type_entity_0x6E8E* v4x; // eax
	type_entity_0x6E8E* v5x; // esi
	__int16 v6; // ax
	__int16 v7; // cx
	char v8; // al
	type_entity_0x6E8E* v9x; // eax
	int v10; // edi
	type_entity_0x6E8E* v11x; // edi
	type_entity_0x6E8E* v13x; // [esp+0h] [ebp-8h]
	char v14; // [esp+4h] [ebp-4h]

	v14 = 0;
	v1x = Entities_EA3E4[a1x->targetId_0x96_150];
	v13x = v1x;
	if (v1x <= Entities_EA3E4[0])
	{
		v2 = a1x->flags_0xc_12_15.byte[0];
		if (!(v2 & 2))
		{
			a1x->flags_0xc_12_15.byte[0] = v2 | 2;
			if (FindProjectileTarget_68940(a1x) || UpdateProjectile_67CB0(a1x))
			{
				a1x->yaw_0x1C_28 = a1x->roll_0x20_32;
				a1x->pitch_0x1E_30 = a1x->fov_0x22_34;
			}
			else
			{
				a1x->roll_0x20_32 = a1x->yaw_0x1C_28;
				a1x->fov_0x22_34 = a1x->pitch_0x1E_30;
			}
		}
	}
	else
	{
		AimAtEntityLeading_65610(a1x, v1x);
	}
	v3 = a1x->minSpeed_0x84_132 - a1x->actSpeed_0x82_130;
	if (a1x->minSpeed_0x84_132 != a1x->actSpeed_0x82_130)
	{
		if (a1x->minSpeed_0x84_132 - a1x->actSpeed_0x82_130 <= 0)
			v3 = -1;
		else
			v3 = 1;
	}
	a1x->actSpeed_0x82_130 += 2 * v3;
	predictedAxis_EB398ar = a1x->position_0x4C_76;
	MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, a1x->pitch_0x1E_30, a1x->actSpeed_0x82_130);
	CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
	v4x = FindOverlappingSelectableEntity_10780(a1x);
	v5x = v4x;
	if (v4x)
	{
		if (v4x->flags_0xc_12_15.word[0] & 0x8010 && ProjectileHitTarget_68740(a1x, v4x, 0x2Du, 22))
			return 0;
		RaiseToEntityCentre_65580(v5x);
		CopyEntityPosition_57CF0(a1x, &v5x->position_0x4C_76);
		LowerFromEntityCentre_655A0(v5x);
		v14 = 1;
		goto LABEL_29;
	}
	v6 = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
	v7 = a1x->position_0x4C_76.z;
	predictedAxis_EB398ar.z = v6;
	if (v6 > v7
		|| isCaveLevel_D41B6
		&& (predictedAxis_EB398ar.z = GetCeilingAlt_10C60(&a1x->position_0x4C_76) - a1x->params_0x52_82.fov, a1x->position_0x4C_76.z > predictedAxis_EB398ar.z))
	{
		a1x->position_0x4C_76.z = predictedAxis_EB398ar.z;
		v8 = a1x->model_0x40_64;
		if (v8 != 4 && v8 != 22 && v8 != 24 && v8 != 26 && TerrainTileIsWater_104D0(&a1x->position_0x4C_76) == 1)
		{
			v9x = CreateEntity_4A190(&a1x->position_0x4C_76, 10, 5);
			if (v9x)
				v9x->id_0x1A_26 = a1x->id_0x1A_26;
			DisableEntityDrawing04_57F10(a1x);
			goto LABEL_29;
		}
		goto LABEL_28;
	}
	v10 = a1x->life_0x8 - 1;
	a1x->life_0x8 = v10;
	if (v10 < 0)
		LABEL_28:
	v14 = 1;
LABEL_29:
	if (!v14)
		return 0;
	if (CanProjectileHitEntity_68AC0(a1x, v5x))
	{
		DisableEntityDrawing04_57F10(a1x);
		return 0;
	}
	v11x = CreateEntity_4A190(&a1x->position_0x4C_76, a1x->levelPar1_0x43_67, a1x->levelPar2_0x44_68);
	if (!v11x)
		return 0;
	SetProjectileDamage_65780(a1x, v5x, v13x);
	CreditKillToWizard_686D0(a1x, v5x);
	if (v5x > Entities_EA3E4[0] && a1x->hitById_0x26_38)
		SendSpellMessage_6D8B0(a1x->id_0x1A_26, Entities_EA3E4[a1x->hitById_0x26_38]->model_0x40_64, 1);
	if (a1x->levelPar2_0x44_68 == 34)
		v11x->life_0x8 = a1x->subSpellIndex_0x2A_42;
	v11x->id_0x1A_26 = a1x->id_0x1A_26;
	v11x->yaw_0x1C_28 = a1x->yaw_0x1C_28;
	v11x->pitch_0x1E_30 = a1x->pitch_0x1E_30;
	if (v5x)
		v11x->targetId_0x96_150 = v5x - world_D41A0.entities_0x6E8E;
	v11x->subSpellIndex_0x2A_42 = a1x->subSpellIndex_0x2A_42;
	v11x->levelPar3_0x46_70 = a1x->levelPar3_0x46_70;
	DisableEntityDrawing04_57F10(a1x);
	return v11x;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41B6: using guessed type char x_BYTE_D41B6;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (00065B30) --------------------------------------------------------
void UpdateFireballProjectile_65B30(type_entity_0x6E8E* entity)//246b30
{
	if (UpdateProjectileTargeting2_65C20(entity))
		DisableEntityDrawing04_57F10(entity);//fire drop
}

//----- (00065B50) --------------------------------------------------------
void UpdateFireStormProjectile_65B50(type_entity_0x6E8E* a1x)//246b50 //tex 55
{
	//int result; // eax
	type_entity_0x6E8E* v2x; // esi
	__int16 v3; // bx
	__int16 v4; // dx
	type_entity_0x6E8E* v5x; // eax
	type_entity_0x6E8E* ix; // ebx
	char v7; // [esp+0h] [ebp-4h]

	v7 = 0;
	v2x = UpdateProjectileTargeting2_65C20(a1x);
	//v2 = result;
	if (v2x)
	{
		v3 = a1x->targetId_0x96_150;
		v2x->maxLife_0x4 = 30;
		v2x->targetId_0x96_150 = v3;
		v4 = v2x->id_0x1A_26;
		v2x->life_0x8 = v2x->maxLife_0x4;
		if (v4)
		{
			v5x = Entities_EA3E4[v4];
			if (v5x->class_0x3F_63 == 3
				&& !v5x->model_0x40_64
				&& v5x->playerData_0xA4_164x->playerColorIndex_0x38_56 == world_D41A0.LevelIndex_0xc)
			{
				v7 = 1;
			}
		}
		for (ix = Entities_EA3E4[v2x->nextLinkedId_0x34_52];
			ix > Entities_EA3E4[0];
			ix = Entities_EA3E4[ix->nextLinkedId_0x34_52])
		{
			ix->id_0x1A_26 = v2x->id_0x1A_26;
			if (v7)
				SetEntityIndex_49C90(ix, 42);
		}
		DisableEntityDrawing04_57F10(a1x);
	}
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00065C20) --------------------------------------------------------
type_entity_0x6E8E* UpdateProjectileTargeting2_65C20(type_entity_0x6E8E* a1x)//246c20
{
	type_entity_0x6E8E* v1x; // eax
	char v2; // dl
	signed __int16 v3; // si
	__int16 v4; // dx
	__int16 v5; // ax
	signed __int16 v6; // si
	__int16 v7; // cx
	type_entity_0x6E8E* v8x; // eax
	type_entity_0x6E8E* v9x; // esi
	__int16 v11; // ax
	__int16 v12; // dx
	type_entity_0x6E8E* v13x; // eax
	int v14; // edi
	type_entity_0x6E8E* v15x; // edi
	axis_3d v16x; // [esp+0h] [ebp-14h]
	//__int16 v17; // [esp+4h] [ebp-10h]
	type_entity_0x6E8E* v18x; // [esp+8h] [ebp-Ch]
	type_entity_0x6E8E* v19x; // [esp+Ch] [ebp-8h]
	char v20; // [esp+10h] [ebp-4h]

	v20 = 0;
	v1x = Entities_EA3E4[a1x->targetId_0x96_150];
	v18x = 0;
	v19x = v1x;
	if (v1x > Entities_EA3E4[0])
	{
		AimAtEntityLeading_65610(a1x, v1x);
		goto LABEL_18;
	}
	v2 = a1x->flags_0xc_12_15.byte[0];
	if (!(v2 & 2))
	{
		a1x->flags_0xc_12_15.byte[0] = v2 | 2;
		if (FindProjectileTarget_68940(a1x))
		{
			v6 = Maths::AngleDifference_582B0(a1x->yaw_0x1C_28, a1x->roll_0x20_32);
			if (v6 < 0)
				v6 = 0;
			if (v6 > 34)
				v6 = 34;
			v7 = v6 * Maths::AngleTurnDirection_582F0(a1x->yaw_0x1C_28, a1x->roll_0x20_32) + a1x->yaw_0x1C_28;
			v5 = a1x->fov_0x22_34;
			a1x->yaw_0x1C_28 = v7;
		}
		else
		{
			if (!UpdateProjectile_67CB0(a1x))
			{
				a1x->roll_0x20_32 = a1x->yaw_0x1C_28;
				a1x->fov_0x22_34 = a1x->pitch_0x1E_30;
				goto LABEL_18;
			}
			v3 = Maths::AngleDifference_582B0(a1x->yaw_0x1C_28, a1x->roll_0x20_32);
			if (v3 < 0)
				v3 = 0;
			if (v3 > 34)
				v3 = 34;
			v4 = v3 * Maths::AngleTurnDirection_582F0(a1x->yaw_0x1C_28, a1x->roll_0x20_32) + a1x->yaw_0x1C_28;
			v5 = a1x->fov_0x22_34;
			a1x->yaw_0x1C_28 = v4;
		}
		a1x->pitch_0x1E_30 = v5;
	}
LABEL_18:
	v16x = a1x->position_0x4C_76;
	predictedAxis_EB398ar = v16x;
	MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, a1x->pitch_0x1E_30, a1x->actSpeed_0x82_130);
	CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);//move projectile?
	v8x = FindOverlappingSelectableEntity_10780(a1x);
	v9x = v8x;
	if (!v8x)
	{
		v11 = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
		v12 = a1x->position_0x4C_76.z;
		predictedAxis_EB398ar.z = v11;
		if (v11 > v12
			|| isCaveLevel_D41B6
			&& (predictedAxis_EB398ar.z = GetCeilingAlt_10C60(&a1x->position_0x4C_76) - a1x->params_0x52_82.fov, a1x->position_0x4C_76.z > predictedAxis_EB398ar.z))
		{
			v16x.z = predictedAxis_EB398ar.z;
			CopyEntityPosition_57CF0(a1x, &v16x);
			if (a1x->model_0x40_64 != 4 && TerrainTileIsWater_104D0(&a1x->position_0x4C_76) == 1)
			{
				v13x = CreateEntity_4A190(&a1x->position_0x4C_76, 10, 5);
				if (v13x)
					v13x->id_0x1A_26 = a1x->id_0x1A_26;
				DisableEntityDrawing04_57F10(a1x);
				goto LABEL_35;
			}
		}
		else
		{
			v14 = a1x->life_0x8 - 1;
			a1x->life_0x8 = v14;
			if (v14 >= 0)
				goto LABEL_35;
		}
		v20 = 1;
		goto LABEL_35;
	}
	if (v8x->flags_0xc_12_15.word[0] & 0x8010)
	{
		if (ProjectileHitTarget_68740(a1x, v8x, 0x5Bu, 45))
			return 0;
	}
	else
	{
		if (v8x->typeParams_0xA0_160x->flags_160_0x20_32 & 0x10)
			a1x->subSpellIndex_0x2A_42 = 1;
		RaiseToEntityCentre_65580(v8x);
		CopyEntityPosition_57CF0(a1x, &v9x->position_0x4C_76);
		LowerFromEntityCentre_655A0(v9x);
		v20 = 1;
	}
LABEL_35:
	if (v20)
	{
		if (CanProjectileHitEntity_68AC0(a1x, v9x))
		{
			DisableEntityDrawing04_57F10(a1x);
		}
		else
		{
			v18x = CreateEntity_4A190(&a1x->position_0x4C_76, a1x->levelPar1_0x43_67, a1x->levelPar2_0x44_68);
			if (v18x)
			{
				SetProjectileDamage_65780(a1x, v9x, v19x);
				CreditKillToWizard_686D0(a1x, v9x);
				if (v9x > Entities_EA3E4[0])
					SendSpellMessage_6D8B0(a1x->id_0x1A_26, 0, 1);
				v15x = v18x;
				v18x->subSpellIndex_0x2A_42 = a1x->subSpellIndex_0x2A_42;
				v15x->id_0x1A_26 = a1x->id_0x1A_26;
				v15x->yaw_0x1C_28 = a1x->yaw_0x1C_28;
				v15x->pitch_0x1E_30 = a1x->pitch_0x1E_30;
				if (!v9x)
					a1x->targetId_0x96_150 = 0;
			}
		}
	}
	return v18x;
}
// D41B6: using guessed type char x_BYTE_D41B6;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

int debugcounter_sub_65F60 = 0;

//----- (00065F60) --------------------------------------------------------
void UpdatePossessionProjectile_65F60(type_entity_0x6E8E* a1x)//246f60
{
	type_entity_0x6E8E* v1x; // eax
	//char v2; // dl
	__int16 v3; // ax
	__int16 v4; // ax
	type_entity_0x6E8E* v5x; // eax
	type_entity_0x6E8E* v6x; // esi
	type_entity_0x6E8E* v7x; // edi
	//type_str_0x6E8E* v8x; // eax
	signed __int16 v9; // dx
	//signed __int16 v10; // cx
	int v11; // esi
	type_entity_0x6E8E* v12x; // esi
	type_entity_0x6E8E* v14x; // [esp+0h] [ebp-8h]
	char v15; // [esp+4h] [ebp-4h]

	/*uint8_t origbyte20 = 0;
	uint8_t remakebyte20 = 0;
	int comp20;

	comp20 = compare_with_sequence_D41A0((char*)"00246F60-00356038", (uint8_t*)& D41A0_BYTESTR_0, 0x356038, debugcounter_sub_65F60, 224790, &origbyte20, &remakebyte20);
	debugcounter_sub_65F60++;*/

	if (debugcounter_sub_65F60 >= 0x5)
	{
		debugcounter_sub_65F60 = debugcounter_sub_65F60;
	}
	debugcounter_sub_65F60++;

	v15 = 0;
	v1x = Entities_EA3E4[a1x->targetId_0x96_150];
	v14x = v1x;
	if (v1x <= Entities_EA3E4[0])
	{
		//v2 = *(x_BYTE *)(a1 + 12);
		if (!(a1x->flags_0xc_12_15.byte[0] & 2))
		{
			a1x->flags_0xc_12_15.byte[0] |= 2;
			if (UpdateProjectile_67CB0(a1x))//problem je zde
			{
				a1x->yaw_0x1C_28 = a1x->roll_0x20_32;
				a1x->pitch_0x1E_30 = a1x->fov_0x22_34;
			}
		}
	}
	else
	{
		AimAtEntityLeading_65610(a1x, v1x);
	}
	predictedAxis_EB398ar = a1x->position_0x4C_76;
	MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, a1x->pitch_0x1E_30, a1x->actSpeed_0x82_130);
	v3 = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
	if (v3 > predictedAxis_EB398ar.z)
		predictedAxis_EB398ar.z = v3;
	if (isCaveLevel_D41B6)
	{
		v4 = GetCeilingAlt_10C60(&predictedAxis_EB398ar) - a1x->params_0x52_82.fov;
		if (v4 < predictedAxis_EB398ar.z)
			predictedAxis_EB398ar.z = v4;
	}
	CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
	v5x = FindOverlappingHostileEntity_108B0(a1x);
	v6x = v5x;
	v7x = v5x;
	if (!v5x)
	{
		/*LOWORD(v8) = *///sub_10C40_getTerrainAlt(a1x->array_0x4C_76);
		v9 = a1x->position_0x4C_76.z;
		predictedAxis_EB398ar.z = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
		if (predictedAxis_EB398ar.z > v9
			|| isCaveLevel_D41B6
			&& (predictedAxis_EB398ar.z = GetCeilingAlt_10C60(&a1x->position_0x4C_76) - a1x->params_0x52_82.fov,
				/*x_WORD_EB398ar[2] = (short)v8,*/
				//v10 = x_WORD_EB398ar[2],
				//LOWORD(v8) = a1x->array_0x4C_76.z,
				a1x->position_0x4C_76.z > predictedAxis_EB398ar.z))
		{
			//LOWORD(v8) = x_WORD_EB398ar[2];
			a1x->position_0x4C_76.z = predictedAxis_EB398ar.z;
		}
		else
		{
			v11 = a1x->life_0x8 - 1;
			a1x->life_0x8 = v11;
			if (v11 >= 0)
				goto LABEL_19;
		}
		v15 = 1;
		goto LABEL_19;
	}
	RaiseToEntityCentre_65580(v5x);
	CopyEntityPosition_57CF0(a1x, &v6x->position_0x4C_76);
	/*LOWORD(v8) = */LowerFromEntityCentre_655A0(v6x);
	v15 = 1;
LABEL_19:
	if (v15)
	{
		//v8x = sub_4A190(a1x->array_0x4C_76, *(char *)(a1 + 67), *(char *)(a1 + 68));
		v12x = CreateEntity_4A190(&a1x->position_0x4C_76, a1x->levelPar1_0x43_67, a1x->levelPar2_0x44_68);
		if (v12x)
		{
			SetProjectileDamage_65780(a1x, v7x, v14x);
			if (v7x > Entities_EA3E4[0])
				SendSpellMessage_6D8B0(a1x->id_0x1A_26, 1u, 1);
			v12x->id_0x1A_26 = a1x->id_0x1A_26;
			v12x->yaw_0x1C_28 = a1x->yaw_0x1C_28;
			v12x->pitch_0x1E_30 = a1x->pitch_0x1E_30;
			DisableEntityDrawing04_57F10(a1x);
		}
	}
	return;
}
// D41B6: using guessed type char x_BYTE_D41B6;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (00066160) --------------------------------------------------------
type_entity_0x6E8E* UpdateEarthquakeProjectile_66160(type_entity_0x6E8E* a1x)//247160
{
	type_entity_0x6E8E* resultx; // eax

	resultx = UpdateProjectileTargeting_65820(a1x);
	if (resultx)
		resultx->life_0x8 = a1x->levelPar3_0x46_70;
	return resultx;
}

//----- (00066180) --------------------------------------------------------
void UpdateMeteorProjectile_66180(type_entity_0x6E8E* a1x)//247180
{
	type_entity_0x6E8E* v1x; // esi
	type_entity_0x6E8E* resultx; // eax
	__int16 v3; // dx
	axis_3d v4x; // [esp+0h] [ebp-8h]
	//__int16 v5; // [esp+2h] [ebp-6h]
	//__int16 v6; // [esp+4h] [ebp-4h]

	//resultx = UpdateProjectileTargeting_65820(a1x);
	v1x = UpdateProjectileTargeting_65820(a1x);
	//BYTE1(result) = *(x_BYTE *)(a1 + 63);
	if (a1x->class_0x3F_63)
	{
		a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
		v4x.x = a1x->rand_0x14_20 % 0x81u + a1x->position_0x4C_76.x - 96 - 64;
		a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
		v4x.y = a1x->rand_0x14_20 % 0x81u + a1x->position_0x4C_76.y - 96 - 64;
		v4x.z = a1x->position_0x4C_76.z;
		resultx = CreateEntity_4A190(&v4x, 10, 0);
		if (resultx)
		{
			resultx->flags_0xc_12_15.dword |= 0x10080;
			resultx->id_0x1A_26 = a1x->id_0x1A_26;
			v3 = a1x->yaw_0x1C_28;
			resultx->life_0x8 = 4;
			resultx->animationFrame_0x5C_92 = 3;
			resultx->yaw_0x1C_28 = v3;
		}
		if (v1x)
		{
			//result = *(char *)(a1 + 70);
			v1x->maxLife_0x4 = a1x->levelPar3_0x46_70;
			v1x->life_0x8 = a1x->levelPar3_0x46_70;
		}
	}
	//return result;
}

//----- (00066250) --------------------------------------------------------
type_entity_0x6E8E* UpdateVolcanoProjectile_66250(type_entity_0x6E8E* a1x)//247250
{
	type_entity_0x6E8E* resultx; // eax
	int v2; // edx

	resultx = UpdateProjectileTargeting_65820(a1x);
	if (resultx)
	{
		v2 = a1x->levelPar3_0x46_70;
		resultx->levelPar3_0x46_70 = 0;
		resultx->maxLife_0x4 = v2;
	}
	return resultx;
}

//----- (00066280) --------------------------------------------------------
type_entity_0x6E8E* UpdateCraterProjectile_66280(type_entity_0x6E8E* a1x)//247280
{
	type_entity_0x6E8E* resultx; // eax

	resultx = UpdateProjectileTargeting_65820(a1x);
	if (resultx)
		resultx->life_0x8 = a1x->levelPar3_0x46_70;
	return resultx;
}

//----- (000662A0) --------------------------------------------------------
type_entity_0x6E8E* UpdateBlueOrbProjectile_662A0(type_entity_0x6E8E* a1x)//2472a0
{
	return UpdateProjectileTargeting_65820(a1x);
}

//----- (000662C0) --------------------------------------------------------
void UpdateDuelProjectile_662C0(type_entity_0x6E8E* a1x)//2472c0
{
	UpdateStealManaProjectile_662E0(a1x);
}

//----- (000662E0) --------------------------------------------------------
void UpdateStealManaProjectile_662E0(type_entity_0x6E8E* a1x)//2472e0
{
	type_entity_0x6E8E* v1x; // eax
	char v2; // dl
	signed __int16 v3; // ax
	type_entity_0x6E8E* v4x; // eax
	type_entity_0x6E8E* v5x; // esi
	type_entity_0x6E8E* v6x; // eax
	uint8_t v6y;
	signed __int16 v7; // cx
	//signed __int16 v8; // di
	type_entity_0x6E8E* v9x; // eax
	int v10; // edi
	char v11; // al
	type_entity_0x6E8E* v12x; // edi
	type_entity_0x6E8E* v14x; // [esp+0h] [ebp-8h]
	char v15; // [esp+4h] [ebp-4h]

	//fix
	v6x = 0;
	//fix

	v15 = 0;
	v1x = Entities_EA3E4[a1x->targetId_0x96_150];
	v14x = v1x;
	if (v1x <= Entities_EA3E4[0])
	{
		v2 = a1x->flags_0xc_12_15.byte[0];
		if (!(v2 & 2))
		{
			a1x->flags_0xc_12_15.byte[0] = v2 | 2;
			if (FindProjectileTarget_68940(a1x) || UpdateProjectile_67CB0(a1x))
			{
				a1x->yaw_0x1C_28 = a1x->roll_0x20_32;
				a1x->pitch_0x1E_30 = a1x->fov_0x22_34;
			}
			else
			{
				a1x->roll_0x20_32 = a1x->yaw_0x1C_28;
				a1x->fov_0x22_34 = a1x->pitch_0x1E_30;
			}
		}
	}
	else
	{
		AimAtEntityLeading_65610(a1x, v1x);
	}
	v3 = a1x->minSpeed_0x84_132 - a1x->actSpeed_0x82_130;
	if (a1x->minSpeed_0x84_132 != a1x->actSpeed_0x82_130)
	{
		if (a1x->minSpeed_0x84_132 - a1x->actSpeed_0x82_130 <= 0)
			v3 = -1;
		else
			v3 = 1;
	}
	a1x->actSpeed_0x82_130 += 2 * v3;
	predictedAxis_EB398ar = a1x->position_0x4C_76;
	MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, a1x->pitch_0x1E_30, a1x->actSpeed_0x82_130);
	CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
	v4x = FindOverlappingSelectableEntity_10780(a1x);
	v5x = v4x;
	if (v4x)
	{
		if (v4x->flags_0xc_12_15.word[0] & 0x8010)
		{
			v6y = ProjectileHitTarget_68740(a1x, v4x, 0x2Du, 22);
			if (v6y)
				return;// v6y;
		}
		RaiseToEntityCentre_65580(v5x);
		CopyEntityPosition_57CF0(a1x, &v5x->position_0x4C_76);
		/*LOBYTE(v6) = */LowerFromEntityCentre_655A0(v5x);
		v15 = 1;
	}
	else
	{
		//LOWORD(v6) = sub_10C40_getTerrainAlt(a1x->array_0x4C_76);
		v7 = a1x->position_0x4C_76.z;
		predictedAxis_EB398ar.z = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
		if (predictedAxis_EB398ar.z > v7
			|| isCaveLevel_D41B6
			&& (predictedAxis_EB398ar.z = GetCeilingAlt_10C60(&a1x->position_0x4C_76) - a1x->params_0x52_82.fov,
				//x_WORD_EB398ar[2] = (signed __int16)v6,
				//v8 = (signed __int16)v6,
				//LOWORD(v6) = a1x->array_0x4C_76.z,
				a1x->position_0x4C_76.z > predictedAxis_EB398ar.z))
		{
			//LOBYTE(v6) = x_WORD_EB398ar[2];
			a1x->position_0x4C_76.z = predictedAxis_EB398ar.z;
			if (a1x->model_0x40_64 != 4)
			{
				//v6 = TerrainTileIsWater_104D0((signed __int16 *)(a1 + 76));
				if (TerrainTileIsWater_104D0(&a1x->position_0x4C_76) == 1)
				{
					v9x = CreateEntity_4A190(&a1x->position_0x4C_76, 10, 5);
					if (v9x)
						v9x->id_0x1A_26 = a1x->id_0x1A_26;
					DisableEntityDrawing04_57F10(a1x);
					goto LABEL_26;
				}
			}
		}
		else
		{
			v10 = a1x->life_0x8 - 1;
			a1x->life_0x8 = v10;
			if (v10 >= 0)
				goto LABEL_26;
		}
		v15 = 1;
	}
LABEL_26:
	if (v15)
	{
		if (CanProjectileHitEntity_68AC0(a1x, v5x))
		{
			DisableEntityDrawing04_57F10(a1x);
		}
		else if (!v5x || v5x->class_0x3F_63 != 3 || (v11 = v5x->model_0x40_64) != 0 && v11 != 1)
		{
			SetProjectileDamage_65780(a1x, 0, v14x);
			DisableEntityDrawing04_57F10(a1x);
		}
		else
		{
			v6x = CreateEntity_4A190(&a1x->position_0x4C_76, a1x->levelPar1_0x43_67, a1x->levelPar2_0x44_68);
			v12x = v6x;
			if (v6x)
			{
				SetProjectileDamage_65780(a1x, v5x, v14x);
				CreditKillToWizard_686D0(a1x, v5x);
				if (v5x > Entities_EA3E4[0] && a1x->hitById_0x26_38)
					SendSpellMessage_6D8B0(a1x->id_0x1A_26, Entities_EA3E4[a1x->hitById_0x26_38]->model_0x40_64, 1);
				if (a1x->levelPar2_0x44_68 == 34)
					v12x->life_0x8 = a1x->subSpellIndex_0x2A_42;
				v12x->id_0x1A_26 = a1x->id_0x1A_26;
				v12x->yaw_0x1C_28 = a1x->yaw_0x1C_28;
				v12x->pitch_0x1E_30 = a1x->pitch_0x1E_30;
				v12x->targetId_0x96_150 = v5x - world_D41A0.entities_0x6E8E;
				v12x->subSpellIndex_0x2A_42 = a1x->subSpellIndex_0x2A_42;
				v12x->levelPar3_0x46_70 = a1x->levelPar3_0x46_70;
				DisableEntityDrawing04_57F10(a1x);
			}
		}
	}
	//return v6x;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41B6: using guessed type char x_BYTE_D41B6;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (00066610) --------------------------------------------------------
void UpdateProjectileExplosion_66610(type_entity_0x6E8E* a1x)//247610
{
	char v1; // dl
	type_entity_0x6E8E* v2x; // eax
	signed __int16 v3; // dx
	//signed __int16 v4; // cx
	int v5; // ecx
	char v7; // [esp+0h] [ebp-4h]

	v7 = 0;
	if (Entities_EA3E4[0] >= Entities_EA3E4[a1x->targetId_0x96_150])
	{
		v1 = a1x->flags_0xc_12_15.byte[0];
		if (!(v1 & 2))
		{
			a1x->flags_0xc_12_15.byte[0] = v1 | 2;
			if (FindProjectileTarget_68940(a1x) || UpdateProjectile_67CB0(a1x))
			{
				a1x->yaw_0x1C_28 = a1x->roll_0x20_32;
				a1x->pitch_0x1E_30 = a1x->fov_0x22_34;
			}
			else
			{
				a1x->roll_0x20_32 = a1x->yaw_0x1C_28;
				a1x->fov_0x22_34 = a1x->pitch_0x1E_30;
			}
		}
	}
	predictedAxis_EB398ar = a1x->position_0x4C_76;
	MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, a1x->pitch_0x1E_30, a1x->actSpeed_0x82_130);
	a1x->position_0x4C_76 = predictedAxis_EB398ar;
	v2x = FindOverlappingSelectableEntity_10780(a1x);
	if (v2x)
	{
		v7 = 1;
		a1x->position_0x4C_76 = v2x->position_0x4C_76;
	}
	else
	{
		//LOWORD(v2) = sub_10C40_getTerrainAlt(a1x->array_0x4C_76);
		v3 = a1x->position_0x4C_76.z;
		predictedAxis_EB398ar.z = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
		if (predictedAxis_EB398ar.z > v3
			|| isCaveLevel_D41B6
			&& (predictedAxis_EB398ar.z = GetCeilingAlt_10C60(&a1x->position_0x4C_76) - a1x->params_0x52_82.fov,
				//x_WORD_EB398ar[2] = (short)v2,
				//v4 = x_WORD_EB398ar[2],
				//LOWORD(v2) = *(x_WORD *)(a1 + 80),
				a1x->position_0x4C_76.z > predictedAxis_EB398ar.z)
			|| (v5 = a1x->life_0x8 - 1, a1x->life_0x8 = v5, v5 < 0))
		{
			v7 = 1;
		}
	}
	if (v7)
		DisableEntityDrawing04_57F10(a1x);
	//return v2;
}
// D41B6: using guessed type char x_BYTE_D41B6;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (00066750) --------------------------------------------------------
void UpdateLightningProjectile_66750(type_entity_0x6E8E* a1x)//247750 //tex 151
{
	__int16 v1; // di
	__int16 v2; // si
	int v3; // edx
	__int16 v4; // ST08_2
	unsigned __int16 v5; // ST04_2
	type_entity_0x6E8E* v6x; // eax
	type_entity_0x6E8E* v7x; // esi
	signed __int16 v8; // cx
	int v9; // esi
	int v10; // esi
	int v11; // esi
	unsigned __int16 v12; // ax
	type_entity_0x6E8E* v13x; // esi
	type_entity_0x6E8E* v14x; // eax
	int v14b; // eax
	type_entity_0x6E8E* v15x; // edi
	char v16; // cl
	axis_3d v18x; // [esp+0h] [ebp-2Ch]
	//__int16 v19; // [esp+4h] [ebp-28h]
	axis_3d v20x; // [esp+8h] [ebp-24h]
	//__int16 v21; // [esp+Ch] [ebp-20h]
	axis_3d v22x; // [esp+10h] [ebp-1Ch]
	//int v23; // [esp+14h] [ebp-18h]
	type_entity_0x6E8E* v24x; // [esp+18h] [ebp-14h]
	int v25; // [esp+1Ch] [ebp-10h]
	int v26; // [esp+20h] [ebp-Ch]
	int v27; // [esp+24h] [ebp-8h]
	int v28; // [esp+28h] [ebp-4h]

	if (CommandLineParams.DoDebugSequences()) {
		//add_compare(0x247754, CommandLineParams.DoDebugafterload(),0x75);
	}

	a1x->actSpeed_0x82_130 = a1x->minSpeed_0x84_132;
	v18x = a1x->position_0x4C_76;
	SetMapEntity_57E50(a1x);
	UpdateProjectileExplosion_66610(a1x);
	v1 = a1x->yaw_0x1C_28;
	v2 = a1x->pitch_0x1E_30;
	v27 = 1;
	while (!(a1x->flags_0xc_12_15.byte[1] & 4))
	{
		UpdateProjectileExplosion_66610(a1x);
		v27++;
	}
	a1x->yaw_0x1C_28 = v1;
	a1x->pitch_0x1E_30 = v2;
	v22x.x = 0;
	v22x.y = 0;
	v22x.z = 0;
	v3 = a1x->actSpeed_0x82_130;
	v27 *= 8;
	//v26 = (v3 - (__CFSHL__(v3 >> 31, 3) + 8 * (v3 >> 31))) >> 3;
	v26 = (v3 - my_sign32(v3) * 8 + my_sign32(v3)) >> 3;
	v4 = a1x->pitch_0x1E_30;
	v5 = a1x->yaw_0x1C_28;
	v25 = 0;
	v28 = 0;
	MoveEntity_57FA0(&v22x, v5, v4, v26);
	predictedAxis_EB398ar = v18x;
	v20x = v18x;
	while ((v27 & 0x8000u) == 0)
	{
		v6x = NewEvent_4A050();
		v7x = v6x;
		if (v6x)
		{
			v6x->actionIndex_0x45_69 = 14;
			v6x->class_0x3F_63 = 9;
			v6x->model_0x40_64 = 9;
			v6x->id_0x1A_26 = a1x->id_0x1A_26;
			v6x->maxLife_0x4 = (v6x >= a1x) - 1;
			SetEntityIndexAndRot_49CD0(v6x, 216);
			AddEventToMap_57D70(v7x, &v20x);
			CopyMaxLifeToLife_49A20(v7x);
		}
		v8 = (signed __int16)v27 / 2;
		if ((signed __int16)v27 / 2 < 0)
			v8 = 0;
		if (v8 > 8)
			v8 = 8;
		if (v8 >= (signed __int16)v28)
		{
			v10 = (signed __int16)v28;
			if ((signed __int16)v28 < -v8)
			{
				++v28;
				goto LABEL_17;
			}
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			v9 = 2 * ((signed int)(a1x->rand_0x14_20 % 0x9Du) / 79) - 1 + v10;
		}
		else
		{
			v9 = v28 - 1;
		}
		v28 = v9;
	LABEL_17:
		if (v8 >= (signed __int16)v25)
		{
			v11 = (signed __int16)v25;
			if ((signed __int16)v25 >= -v8)
			{
				a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
				v25 = 2 * ((signed int)(a1x->rand_0x14_20 % 0x9Du) / 79) - 1 + v11;
			}
			else
			{
				v25++;
			}
		}
		else
		{
			v25--;
		}
		predictedAxis_EB398ar.x += v22x.x;
		predictedAxis_EB398ar.y += v22x.y;
		predictedAxis_EB398ar.z += v22x.z;
		v20x = predictedAxis_EB398ar;
		v20x.z = v28
			* (((signed __int16)v26 - (__CFSHL__((signed __int16)v26 >> 31, 2) + 4 * ((signed __int16)v26 >> 31))) >> 2)
			+ predictedAxis_EB398ar.z;
		v12 = a1x->yaw_0x1C_28;
		HIBYTE(v12) = (HIBYTE(v12) + 2) & 7;
		MoveEntity_57FA0(
			&v20x,
			v12,
			0,
			v28 * (((signed __int16)v26 - (__CFSHL__((signed __int16)v26 >> 31, 2) + 4 * ((signed __int16)v26 >> 31))) >> 2));
		v27--;
	}
	v13x = FindOverlappingSelectableEntity_10780(a1x);
	v24x = Entities_EA3E4[a1x->targetId_0x96_150];
	v14x = CreateEntity_4A190(&v20x, a1x->levelPar1_0x43_67, a1x->levelPar2_0x44_68);
	v15x = v14x;
	if (!v14x)
		return;// (short)v14x;
	SetProjectileDamage_65780(a1x, v13x, v24x);
	CreditKillToWizard_686D0(a1x, v13x);
	CanProjectileHitEntity_68AC0(a1x, v13x);
	if (v13x > Entities_EA3E4[0])
		SendSpellMessage_6D8B0(a1x->id_0x1A_26, 7u, 1);
	v15x->id_0x1A_26 = a1x->id_0x1A_26;
	v15x->yaw_0x1C_28 = a1x->yaw_0x1C_28;
	v15x->pitch_0x1E_30 = a1x->pitch_0x1E_30;
	v16 = 0;
	//fix
	if (v13x == NULL)
		v15x->targetId_0x96_150 = 0xae02;
	else
		//fix
		v15x->targetId_0x96_150 = v13x - world_D41A0.entities_0x6E8E;
	if (!v13x || v13x->class_0x3F_63 != 3 || !(v13x->flags_0xc_12_15.word[0] & 0x8010))
	{
	LABEL_35:
		v16 = 1;
		goto LABEL_36;
	}
	if (v13x->flags_0xc_12_15.byte[1] >= 0)
	{
		//v14b = (a1x->dword_0x90_144 - (__CFSHL__(a1x->dword_0x90_144 >> 31, 3) + 8 * (a1x->dword_0x90_144 >> 31))) >> 3;
		v14b = (a1x->mana_0x90_144 - (my_sign32(a1x->mana_0x90_144) * 8) + my_sign32(a1x->mana_0x90_144)) >> 3;
		if (v14b <= v13x->mana_0x90_144)//0x247AF9 cmp eax, [esi+90h] (esi = v13x)
		{
			v14b = a1x->subSpellIndex_0x2A_42 / 2;
			v15x->subSpellIndex_0x2A_42 = v14b;
			goto LABEL_36;
		}
		goto LABEL_35;
	}
	//v14b = (a1x->dword_0x90_144 - (__CFSHL__(a1x->dword_0x90_144 >> 31, 2) + 4 * (a1x->dword_0x90_144 >> 31))) >> 2;
	v14b = (a1x->mana_0x90_144 - (my_sign32(a1x->mana_0x90_144) * 4) + my_sign32(a1x->mana_0x90_144)) >> 2;
	if (v14b > v13x->mana_0x90_144)
		goto LABEL_35;
	//v14b = a1x->word_0x2A_42 >> 2;
	v15x->subSpellIndex_0x2A_42 = a1x->subSpellIndex_0x2A_42 >> 2;
LABEL_36:
	if (v16)
	{
		//LOWORD(v14b) = a1x->word_0x2A_42;
		v15x->subSpellIndex_0x2A_42 = a1x->subSpellIndex_0x2A_42;
	}
	//return v14b;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39A: using guessed type __int16 x_WORD_EB398ar[1];
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (00066B30) --------------------------------------------------------
void UpdateCastleProjectile_66B30(type_entity_0x6E8E* a1x)//247b30
{
	type_entity_0x6E8E* v1x; // eax
	__int16 v2; // ax
	int v3; // eax
	__int16 v4; // dx
	__int16 v5; // ax
	__int16 v6; // cx
	int v7; // edi
	type_entity_0x6E8E* v8x; // eax
	type_entity_0x6E8E* v10x; // [esp+0h] [ebp-8h]
	char v11; // [esp+4h] [ebp-4h]

	//fix
	v2 = 0;
	//fix

	v11 = 0;
	v1x = Entities_EA3E4[a1x->targetId_0x96_150];
	v10x = v1x;
	if (v1x > Entities_EA3E4[0])
	{
		AimAtEntityDirect_656D0(a1x, v1x);
		v3 = a1x->minSpeed_0x84_132 - a1x->actSpeed_0x82_130;
		if (a1x->minSpeed_0x84_132 != a1x->actSpeed_0x82_130)
		{
			if (v3 <= 0)
				LOWORD(v3) = -1;
			else
				LOWORD(v3) = 1;
		}
		a1x->actSpeed_0x82_130 += 2 * v3;
		predictedAxis_EB398ar = a1x->position_0x4C_76;
		MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, a1x->pitch_0x1E_30, a1x->actSpeed_0x82_130);
		CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
		if (EntitiesOverlap_106C0(a1x, v10x))
		{
			/*LOBYTE(v2) = */CopyEntityPosition_57CF0(a1x, &v10x->position_0x4C_76);
			v11 = 1;
		}
		else
		{
			v2 = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
			v4 = a1x->position_0x4C_76.z;
			predictedAxis_EB398ar.z = v2;
			if (v2 > v4
				|| isCaveLevel_D41B6
				&& (v5 = GetCeilingAlt_10C60(&a1x->position_0x4C_76) - a1x->params_0x52_82.fov,
					predictedAxis_EB398ar.z = v5,
					v6 = v5,
					v2 = a1x->position_0x4C_76.z,
					v2 > v6))
			{
				//LOBYTE(v2) = x_WORD_EB398ar.z;
				v11 = 1;
				a1x->position_0x4C_76.z = predictedAxis_EB398ar.z;
			}
			else
			{
				v7 = a1x->life_0x8 - 1;
				a1x->life_0x8 = v7;
				if (v7 < 0)
					v11 = 1;
			}
		}
		if (v11)
		{
			if (a1x->levelPar1_0x43_67 == 3 && Entities_EA3E4[a1x->id_0x1A_26]->playerData_0xA4_164x->CastleEntityIndex_0x3A_58)
			{
				DisableEntityDrawing04_57F10(a1x);
			}
			else
			{
				v8x = CreateEntity_4A190(&a1x->position_0x4C_76, a1x->levelPar1_0x43_67, a1x->levelPar2_0x44_68);
				if (v8x)
				{
					v8x->id_0x1A_26 = a1x->id_0x1A_26;
					DisableEntityDrawing04_57F10(a1x);
				}
				else
				{
					/*LOBYTE(v2) = */UpdateCastleLevel_5F890(Entities_EA3E4[a1x->id_0x1A_26], 0);
				}
			}
		}
	}
	else
	{
		/*LOBYTE(v2) = */UpdateHomingProjectile_66D00(a1x);
	}
	//return v2;
}
// D41B6: using guessed type char x_BYTE_D41B6;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (00066D00) --------------------------------------------------------
void UpdateHomingProjectile_66D00(type_entity_0x6E8E* a1x)//247d00
{
	char v1; // dh
	type_entity_0x6E8E* v2x; // eax
	__int16 v3; // ax
	__int16 v4; // ax
	signed __int16 v5; // ax
	unsigned __int16 v6; // ax
	type_entity_0x6E8E* v7x; // esi
	signed __int16 v8; // di
	//signed __int16 v9; // dx
	int v10; // ecx
	unsigned __int16 v11; // ax
	char v13; // [esp+0h] [ebp-8h]
	char v14; // [esp+4h] [ebp-4h]

	//fix
	v2x = 0;
	//fix

	v1 = a1x->flags_0xc_12_15.byte[0];
	v13 = 0;
	v14 = 0;
	if (!(v1 & 2))
	{
		a1x->flags_0xc_12_15.byte[0] = v1 | 2;
		//LOBYTE(v2) = IsNearPlayer_11CB0(&a1x->array_0x4C_76);
		if (!IsNearPlayer_11CB0(&a1x->position_0x4C_76))
		{
			UpdateCastleLevel_5F890(a1x, 0);
			DisableEntityDrawing04_57F10(a1x);
			/*LOBYTE(v2) = */SetHelpPopupIndex93_88D00();
		}
		return;
	}
	a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &a1x->axis_0x9A_154x);
	a1x->fov_0x22_34 = Maths::GetPitchAngle_58210(&a1x->position_0x4C_76, &a1x->axis_0x9A_154x);
	v3 = a1x->yaw_0x1C_28
		+ Maths::TurnTowardsAngle_58350(
			a1x->yaw_0x1C_28,
			a1x->roll_0x20_32,
			a1x->typeParams_0xA0_160x->yawTurnLimit_160_0x4_4,
			a1x->typeParams_0xA0_160x->subtype_160_0x2_2);
	HIBYTE(v3) &= 7u;
	a1x->yaw_0x1C_28 = v3;
	v4 = a1x->pitch_0x1E_30
		+ Maths::TurnTowardsAngle_58350(
			a1x->pitch_0x1E_30,
			a1x->fov_0x22_34,
			a1x->typeParams_0xA0_160x->pitchTurnLimit_160_0x8_8,
			a1x->typeParams_0xA0_160x->pitchTurnSpeed_160_0x6_6);
	HIBYTE(v4) &= 7u;
	a1x->pitch_0x1E_30 = v4;
	v5 = a1x->minSpeed_0x84_132 - a1x->actSpeed_0x82_130;
	if (a1x->minSpeed_0x84_132 != a1x->actSpeed_0x82_130)
	{
		if (a1x->minSpeed_0x84_132 - a1x->actSpeed_0x82_130 <= 0)
			v5 = -1;
		else
			v5 = 1;
	}
	a1x->actSpeed_0x82_130 += 2 * v5;
	predictedAxis_EB398ar = a1x->position_0x4C_76;
	MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, a1x->pitch_0x1E_30, a1x->actSpeed_0x82_130);
	CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
	v6 = a1x->targetId_0x96_150;
	v7x = Entities_EA3E4[v6];
	if (v7x > Entities_EA3E4[0] && EntitiesOverlap_106C0(a1x, Entities_EA3E4[v6]))
	{
		RaiseToEntityCentre_65580(v7x);
		/*LOBYTE(v2) = */CopyEntityPosition_57CF0(a1x, &v7x->position_0x4C_76);
		v13 = 1;
		goto LABEL_18;
	}
	//LOWORD(v2x) = sub_10C40_getTerrainAlt(&a1x->array_0x4C_76);
	v8 = a1x->position_0x4C_76.z;
	predictedAxis_EB398ar.z = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
	if (predictedAxis_EB398ar.z > v8
		|| isCaveLevel_D41B6
		&& (predictedAxis_EB398ar.z = GetCeilingAlt_10C60(&a1x->position_0x4C_76) - a1x->params_0x52_82.fov,
			//x_WORD_EB398ar.z = (short)v2,
			//v9 = (short)v2,
			//LOWORD(v2) = a1x->array_0x4C_76.z,
			a1x->position_0x4C_76.z > predictedAxis_EB398ar.z))
	{
		//LOBYTE(v2) = x_WORD_EB398ar[2];
		a1x->position_0x4C_76.z = predictedAxis_EB398ar.z;
	LABEL_17:
		v13 = 1;
		goto LABEL_18;
	}
	v10 = a1x->life_0x8 - 1;
	a1x->life_0x8 = v10;
	if (v10 < 0)
		goto LABEL_17;
	//LOBYTE(v2) = IsNearPlayer_11CB0(&a1x->array_0x4C_76) == 0;
	v14 = IsNearPlayer_11CB0(&a1x->position_0x4C_76) == 0;
	if (v14)
		goto LABEL_17;
LABEL_18:
	if (v13)
	{
		if (v14)
		{
			predictedAxis_EB398ar = a1x->position_0x4C_76;
			//*(x_WORD_EB398ar + 2) = *(x_WORD *)(a1 + 80);
			v11 = a1x->yaw_0x1C_28;
			HIBYTE(v11) = (HIBYTE(v11) + 4) & 7;
			MoveEntity_57FA0(&predictedAxis_EB398ar, v11, a1x->pitch_0x1E_30, a1x->actSpeed_0x82_130);
			CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
		}
		v2x = CreateEntity_4A190(&a1x->position_0x4C_76, a1x->levelPar1_0x43_67, a1x->levelPar2_0x44_68);
		if (v2x)
		{
			v2x->id_0x1A_26 = a1x->id_0x1A_26;
			DisableEntityDrawing04_57F10(a1x);
		}
	}
	//	return (char)v2x;
}
// D41B6: using guessed type char x_BYTE_D41B6;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (00066FB0) --------------------------------------------------------
type_entity_0x6E8E* UpdateBoneProjectile_66FB0(type_entity_0x6E8E* a1)//247fb0
{
	return UpdateProjectileTargeting_65820(a1);
}

//----- (00066FD0) --------------------------------------------------------
void UpdateThunderboltProjectile_66FD0(type_entity_0x6E8E* a1x)//247fd0
{
	type_entity_0x6E8E* v1x; // eax
	char v2; // dl
	int v3; // esi
	unsigned __int16 v4; // dx
	signed __int16 v5; // ax
	type_entity_0x6E8E* v6x; // eax
	type_entity_0x6E8E* v7x; // edi
	//uint8_t* v8; // eax
	__int16 v9; // ax
	__int16 v10; // dx
	type_entity_0x6E8E* v11x; // eax
	type_entity_0x6E8E* v12x; // esi
	type_entity_0x6E8E* v14x; // [esp+0h] [ebp-8h]
	char v15; // [esp+4h] [ebp-4h]

	//fix
	//v8 = 0;
	//fix

	v15 = 0;
	v1x = Entities_EA3E4[a1x->targetId_0x96_150];
	v14x = v1x;
	if (v1x <= Entities_EA3E4[0])
	{
		v2 = a1x->flags_0xc_12_15.byte[0];
		if (!(v2 & 2))
		{
			a1x->flags_0xc_12_15.byte[0] = v2 | 2;
			if (FindProjectileTarget_68940(a1x))
			{
				v3 = a1x->life_0x8 + 32;
				a1x->yaw_0x1C_28 = a1x->roll_0x20_32;
				a1x->life_0x8 = v3;
				a1x->maxLife_0x4 = v3;
				v4 = a1x->nextLinkedId_0x34_52;
				a1x->pitch_0x1E_30 = a1x->fov_0x22_34;
				if (v4)
				{
					ReleaseEntity_57F20(Entities_EA3E4[v4]);
					a1x->nextLinkedId_0x34_52 = 0;
				}
			}
			else if (UpdateProjectile_67CB0(a1x))
			{
				a1x->yaw_0x1C_28 = a1x->roll_0x20_32;
				a1x->pitch_0x1E_30 = a1x->fov_0x22_34;
			}
			else
			{
				a1x->roll_0x20_32 = a1x->yaw_0x1C_28;
				a1x->fov_0x22_34 = a1x->pitch_0x1E_30;
			}
		}
	}
	else
	{
		AimAtEntityLeading_65610(a1x, v1x);
	}
	v5 = a1x->minSpeed_0x84_132 - a1x->actSpeed_0x82_130;
	if (a1x->minSpeed_0x84_132 != a1x->actSpeed_0x82_130)
	{
		if (a1x->minSpeed_0x84_132 - a1x->actSpeed_0x82_130 <= 0)
			v5 = -1;
		else
			v5 = 1;
	}
	a1x->actSpeed_0x82_130 += 2 * v5;
	predictedAxis_EB398ar = a1x->position_0x4C_76;
	MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, a1x->pitch_0x1E_30, a1x->actSpeed_0x82_130);
	CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
	v6x = FindOverlappingSelectableEntity_10780(a1x);
	v7x = v6x;
	if (v6x)
	{
		if (v6x->flags_0xc_12_15.word[0] & 0x8010)
		{
			//LOBYTE(v8) = ProjectileHitTarget_68740(a1, v6, 0x2Du, 22);
			if (ProjectileHitTarget_68740(a1x, v6x, 0x2Du, 22))
				return;// (char)v8;
		}
		RaiseToEntityCentre_65580(v7x);
		CopyEntityPosition_57CF0(a1x, &v7x->position_0x4C_76);
		/*LOBYTE(v8) = */LowerFromEntityCentre_655A0(v7x);
		v15 = 1;
	}
	else
	{
		v9 = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
		v10 = a1x->position_0x4C_76.z;
		predictedAxis_EB398ar.z = v9;
		if (v9 > v10
			|| isCaveLevel_D41B6
			&& (predictedAxis_EB398ar.z = GetCeilingAlt_10C60(&a1x->position_0x4C_76) - a1x->params_0x52_82.fov, a1x->position_0x4C_76.z > predictedAxis_EB398ar.z))
		{
			//LOBYTE(v8) = x_WORD_EB398ar[2];
			a1x->position_0x4C_76.z = predictedAxis_EB398ar.z;
			if (a1x->model_0x40_64 != 4)
			{
				//v8 = (uint8_t*)TerrainTileIsWater_104D0((signed __int16 *)(a1 + 76));
				if (TerrainTileIsWater_104D0(&a1x->position_0x4C_76) == 1)
				{
					v11x = CreateEntity_4A190(&a1x->position_0x4C_76, 10, 5);
					if (v11x)
						v11x->id_0x1A_26 = a1x->id_0x1A_26;
					DisableEntityDrawing04_57F10(a1x);
					goto LABEL_28;
				}
			}
		}
		else
		{
			//v8 = (Bit8u*)(*(x_DWORD *)(a1 + 8) - 1);
			a1x->life_0x8--;
			if (a1x->life_0x8 >= 0)
				goto LABEL_28;
		}
		v15 = 1;
	}
LABEL_28:
	if (v15)
	{
		if (CanProjectileHitEntity_68AC0(a1x, v7x))
		{
			DisableEntityDrawing04_57F10(a1x);
		}
		else
		{
			//v8 = sub_4A190((uint8_t*)(a1 + 76), 10, 38);
			v12x = CreateEntity_4A190(&a1x->position_0x4C_76, 10, 38);
			if (v12x)
			{
				SetProjectileDamage_65780(a1x, v7x, v14x);
				if (v7x > Entities_EA3E4[0])
					SendSpellMessage_6D8B0(a1x->id_0x1A_26, 7u, 1);
				v12x->id_0x1A_26 = a1x->id_0x1A_26;
				v12x->yaw_0x1C_28 = a1x->yaw_0x1C_28;
				v12x->pitch_0x1E_30 = a1x->pitch_0x1E_30;
				if (v7x == 0)
				{
					v12x->targetId_0x96_150 = 0xae02;//fix
				}
				else
					v12x->targetId_0x96_150 = v7x - world_D41A0.entities_0x6E8E;
				v12x->subSpellIndex_0x2A_42 = a1x->subSpellIndex_0x2A_42;
				v12x->levelPar1_0x43_67 = a1x->levelPar1_0x43_67;
				v12x->levelPar2_0x44_68 = a1x->levelPar2_0x44_68;
				DisableEntityDrawing04_57F10(a1x);
			}
		}
	}
	//return (char)v8;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41B6: using guessed type char x_BYTE_D41B6;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (000672E0) --------------------------------------------------------
void UpdateArrow_672E0(type_entity_0x6E8E* a1x)//2482e0
{
	//uint8_t* v1; // esi
	//int v2; // eax
	type_entity_0x6E8E* v3x; // edi
	//int v4; // eax
	//char result; // al

	if (!(a1x->flags_0xc_12_15.byte[0] & 2))
	{
		//v1 = x_D41A0_BYTEARRAY_0;
		//v2 = 9377 * *(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 8) + 9439;
		//*(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 8) = v2;
		world_D41A0.rand_0x8 = 9377 * world_D41A0.rand_0x8 + 9439;
		PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, (world_D41A0.rand_0x8 & 1) + 33);
		a1x->flags_0xc_12_15.byte[0] |= 2u;
	}
	predictedAxis_EB398ar = a1x->position_0x4C_76;
	MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, a1x->pitch_0x1E_30, a1x->actSpeed_0x82_130);
	v3x = FindOverlappingSelectableEntity_10780(a1x);
	if (GetTerrainAlt_10C40(&predictedAxis_EB398ar) <= predictedAxis_EB398ar.z)
	{
		//v4 = *(x_DWORD *)(a1 + 8);
		//(a1x->dword_0x8)--;// = v4 - 1;
		if (a1x->life_0x8--)
		{
			if (!v3x)
			{
				CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
				return;
			}
		}
	}
	if (!v3x)
	{
	LABEL_10:
		SendMailToOverlappingEntities_10C80(a1x, 0, a1x->subSpellIndex_0x2A_42);
		DisableEntityDrawing04_57F10(a1x);
		return;//fix
	}
	if (!(v3x->flags_0xc_12_15.word[0] & 0x8010) || (/*result = */ProjectileHitTarget_68740(a1x, v3x, 0x2Du, 22)) == 0)
	{
		RaiseToEntityCentre_65580(v3x);
		CopyEntityPosition_57CF0(a1x, &v3x->position_0x4C_76);
		LowerFromEntityCentre_655A0(v3x);
		goto LABEL_10;
	}
	return;// result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (00067410) --------------------------------------------------------
void UpdateProjectileAction14_67410(type_entity_0x6E8E* a1x)//248410
{
	int result; // eax

	result = a1x->life_0x8;
	a1x->life_0x8 = result - 1;
	if (result < 0)
		DisableEntityDrawing04_57F10(a1x);
	//return result;
}

//----- (00067430) --------------------------------------------------------
void UpdateGreenBlobProjectile_67430(type_entity_0x6E8E* a1)//248430
{
	UpdateProjectileTargeting_65820(a1);
}

//----- (00067450) --------------------------------------------------------
void UpdatePurpleOrbProjectile_67450(type_entity_0x6E8E* a1)//248450
{
	UpdateProjectileTargeting_65820(a1);
}

//----- (00067470) --------------------------------------------------------
void UpdateProjectileAction17_67470(type_entity_0x6E8E* a1x)//248470
{
	type_entity_0x6E8E* resultx; // eax
	char v2; // dh

	/*result = */UpdateProjectileTargeting_65820(a1x);
	//BYTE1(result) = *(x_BYTE *)(a1 + 63);
	if (a1x->class_0x3F_63)
	{
		resultx = CreateEntity_4A190(&a1x->position_0x4C_76, 10, 0);
		if (resultx)
		{
			resultx->flags_0xc_12_15.byte[0] |= 0x80u;
			v2 = resultx->flags_0xc_12_15.byte[2] | 1;
			resultx->id_0x1A_26 = a1x->id_0x1A_26;
			resultx->flags_0xc_12_15.byte[2] = v2;
		}
	}
	//return result;
}

//----- (000674C0) --------------------------------------------------------
void UpdatePossessionBeam_674C0(type_entity_0x6E8E* a1x)//2484c0
{
	type_entity_0x6E8E* v1x; // eax
	char v2; // dl
	__int16 v3; // ax
	type_entity_0x6E8E* v4x; // eax
	type_entity_0x6E8E* v5x; // esi
	type_entity_0x6E8E* v6x; // edi
	__int16 result; // ax
	__int16 v8; // cx
	__int16 v9; // ax
	__int16 v10; // si
	int v11; // esi
	type_entity_0x6E8E* v12x; // eax
	type_entity_0x6E8E* v13x; // esi
	type_entity_0x6E8E* v14x; // esi
	type_entity_0x6E8E* v15x; // [esp+0h] [ebp-8h]
	char v16; // [esp+4h] [ebp-4h]

	v16 = 0;
	v1x = Entities_EA3E4[a1x->targetId_0x96_150];
	v15x = v1x;
	if (v1x <= Entities_EA3E4[0])
	{
		v2 = a1x->flags_0xc_12_15.byte[0];
		if (!(v2 & 2))
		{
			a1x->flags_0xc_12_15.byte[0] = v2 | 2;
			if (UpdateProjectile_67CB0(a1x))
			{
				a1x->yaw_0x1C_28 = a1x->roll_0x20_32;
				a1x->pitch_0x1E_30 = a1x->fov_0x22_34;
			}
		}
	}
	else
	{
		AimAtEntityLeading_65610(a1x, v1x);
	}
	predictedAxis_EB398ar = a1x->position_0x4C_76;

	MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, a1x->pitch_0x1E_30, a1x->actSpeed_0x82_130);
	v3 = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
	if (v3 > predictedAxis_EB398ar.z)
		predictedAxis_EB398ar.z = v3;
	CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
	v4x = FindOverlappingHostileEntity_108B0(a1x);
	v5x = v4x;
	v6x = v4x;
	if (v4x)
	{
		RaiseToEntityCentre_65580(v4x);
		CopyEntityPosition_57CF0(a1x, &v5x->position_0x4C_76);
		/*result = */LowerFromEntityCentre_655A0(v5x);
		v16 = 1;
		goto LABEL_16;
	}
	result = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
	v8 = a1x->position_0x4C_76.z;
	predictedAxis_EB398ar.z = result;
	if (result > v8
		|| isCaveLevel_D41B6
		&& (v9 = GetCeilingAlt_10C60(&a1x->position_0x4C_76) - a1x->params_0x52_82.fov,
			predictedAxis_EB398ar.z = v9,
			v10 = v9,
			result = a1x->position_0x4C_76.z,
			result > v10))
	{
		result = predictedAxis_EB398ar.z;
		a1x->position_0x4C_76.z = predictedAxis_EB398ar.z;
	}
	else
	{
		v11 = a1x->life_0x8 - 1;
		a1x->life_0x8 = v11;
		if (v11 >= 0)
			goto LABEL_16;
	}
	v16 = 1;
LABEL_16:
	if (v16)
	{
		if (v6x)
		{
			if (a1x->levelPar1_0x43_67 != 10 || a1x->levelPar2_0x44_68 != 69)
				v12x = CreateEntity_4A190(&a1x->position_0x4C_76, 10, 12);
			else
				v12x = CreateEntity_4A190(&a1x->position_0x4C_76, 10, 70);
			v13x = v12x;
			if (v12x)
			{
				SetProjectileDamage_65780(a1x, v6x, v15x);
				v13x->id_0x1A_26 = a1x->id_0x1A_26;
				v13x->yaw_0x1C_28 = a1x->yaw_0x1C_28;
				v13x->pitch_0x1E_30 = a1x->pitch_0x1E_30;
			}
			v14x = CreateEntity_4A190(&a1x->position_0x4C_76, a1x->levelPar1_0x43_67, a1x->levelPar2_0x44_68);
			if (v14x)
			{
				SetProjectileDamage_65780(a1x, v6x, v15x);
				SendSpellMessage_6D8B0(a1x->id_0x1A_26, 1u, 1);
				v14x->id_0x1A_26 = a1x->id_0x1A_26;
				v14x->yaw_0x1C_28 = a1x->yaw_0x1C_28;
				v14x->pitch_0x1E_30 = a1x->pitch_0x1E_30;
				v14x->param_0x10_16 = a1x->param_0x10_16;
			}
		}
		DisableEntityDrawing04_57F10(a1x);
	}
	//return result;
}
// D41B6: using guessed type char x_BYTE_D41B6;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (000676F0) --------------------------------------------------------
type_entity_0x6E8E* SpawnStoredEntity_676F0(type_entity_0x6E8E* a1x)//2486f0
{
	type_entity_0x6E8E* resultx; // eax

	resultx = CreateEntity_4A190(&a1x->position_0x4C_76, a1x->levelPar1_0x43_67, a1x->levelPar2_0x44_68);
	if (resultx)
	{
		resultx->id_0x1A_26 = a1x->id_0x1A_26;
		resultx->yaw_0x1C_28 = a1x->yaw_0x1C_28;
		resultx->pitch_0x1E_30 = a1x->pitch_0x1E_30;
		resultx->subSpellIndex_0x2A_42 = a1x->subSpellIndex_0x2A_42;
		DisableEntityDrawing04_57F10(a1x);
	}
	return resultx;
}

//----- (00067740) --------------------------------------------------------
type_entity_0x6E8E* UpdateProjectileAction20_67740(type_entity_0x6E8E* a1)//248740
{
	return UpdateProjectileTargeting_65820(a1);
}

//----- (00067760) --------------------------------------------------------
type_entity_0x6E8E* UpdateManticoreStunProjectile_67760(type_entity_0x6E8E* a1)//248760
{
	return UpdateProjectileTargeting_65820(a1);
}

//----- (00067780) --------------------------------------------------------
type_entity_0x6E8E* UpdateSpiderWebProjectile_67780(type_entity_0x6E8E* a1)//248780
{
	return UpdateProjectileTargeting_65820(a1);
}

//----- (000677A0) --------------------------------------------------------
type_entity_0x6E8E* UpdateGravityWellProjectile_677A0(type_entity_0x6E8E* a1x)//2487a0
{
	type_entity_0x6E8E* resultx; // eax
	int v2; // edx

	resultx = UpdateProjectileTargeting_65820(a1x);
	if (resultx)
	{
		v2 = a1x->levelPar3_0x46_70;
		resultx->levelPar3_0x46_70 = 0;
		resultx->life_0x8 = v2;
	}
	return resultx;
}

//----- (000677D0) --------------------------------------------------------
type_entity_0x6E8E* UpdateTremorProjectile_677D0(type_entity_0x6E8E* a1x)//2487d0
{
	type_entity_0x6E8E* resultx; // eax
	int v2; // edx

	resultx = UpdateProjectileTargeting_65820(a1x);
	if (resultx)
	{
		v2 = a1x->levelPar3_0x46_70;
		LOBYTE(v2) = v2 & 0xF0;
		resultx->levelPar3_0x46_70 = 0;
		resultx->maxLife_0x4 = v2;
		resultx->life_0x8 = v2;
	}
	return resultx;
}

//----- (00067800) --------------------------------------------------------
type_entity_0x6E8E* UpdateSummonArmyProjectile_67800(type_entity_0x6E8E* a1x)//248800
{
	unsigned __int8 v1; // al
	type_entity_0x6E8E* resultx; // eax

	v1 = a1x->levelPar3_0x46_70;
	if (v1 < 0x10u)
	{
		if (v1 != 2)
			goto LABEL_10;
	LABEL_8:
		gameState_D41A4.summonCount_224 = 8;
		goto LABEL_11;
	}
	if (v1 <= 0x10u || v1 < 0x13u)
		goto LABEL_10;
	if (v1 <= 0x13u)
		goto LABEL_8;
	if (v1 == 25)
	{
		gameState_D41A4.summonCount_224 = 4;
		goto LABEL_11;
	}
LABEL_10:
	gameState_D41A4.summonCount_224 = 2;
LABEL_11:
	resultx = UpdateProjectileTargeting_65820(a1x);
	if (resultx)
	{
		while (resultx != Entities_EA3E4[0])
		{
			resultx->levelPar3_0x46_70 = a1x->levelPar3_0x46_70;
			resultx->parentId_0x28_40 = a1x->parentId_0x28_40;
			resultx->id_0x1A_26 = a1x->id_0x1A_26;
			resultx = Entities_EA3E4[resultx->nextLinkedId_0x34_52];
		}
	}
	return resultx;
}
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00067890) --------------------------------------------------------
type_entity_0x6E8E* UpdateAllianceProjectile_67890(type_entity_0x6E8E* a1x)//248890
{
	type_entity_0x6E8E* resultx; // eax
	char v2; // dh

	resultx = UpdateProjectileTargeting_65820(a1x);
	if (resultx)
	{
		resultx = Entities_EA3E4[a1x->id_0x1A_26];
		if (resultx > Entities_EA3E4[0] && resultx->class_0x3F_63 == 3)
		{
			v2 = resultx->model_0x40_64;
			if (!v2 || v2 == 1)
				resultx->targetId_0x96_150 = 0;
		}
	}
	return resultx;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000678E0) --------------------------------------------------------
type_entity_0x6E8E* UpdateWhirlwindProjectile_678E0(type_entity_0x6E8E* a1x)//2488e0
{
	type_entity_0x6E8E* resultx; // eax
	int v2; // edx

	resultx = UpdateProjectileTargeting_65820(a1x);
	if (resultx)
	{
		v2 = 8 * a1x->levelPar3_0x46_70;
		resultx->maxLife_0x4 = v2;
		resultx->life_0x8 = v2;
	}
	return resultx;
}

//----- (00067910) --------------------------------------------------------
type_entity_0x6E8E* UpdateCaveInProjectile_67910(type_entity_0x6E8E* a1x)//248910
{
	type_entity_0x6E8E* resultx; // eax
	int v2; // edx

	resultx = UpdateProjectileTargeting_65820(a1x);
	if (resultx)
	{
		v2 = a1x->levelPar3_0x46_70;
		resultx->levelPar3_0x46_70 = 0;
		resultx->maxLife_0x4 = v2;
	}
	return resultx;
}

//----- (00067940) --------------------------------------------------------
type_entity_0x6E8E* UpdatePurpleOrb2Projectile_67940(type_entity_0x6E8E* a1)//248940
{
	return UpdateProjectileTargeting_65820(a1);
}

//----- (00067960) --------------------------------------------------------
void UpdateMagicMineProjectile_67960(/*type_str_0x6E8E* a1x,*/ type_entity_0x6E8E* a2x)//248960
{
	int v2; // eax
	type_entity_0x6E8E* v3x; // eax
	type_entity_0x6E8E* ix; // edi
	signed int v5; // ST10_4
	signed int v6; // eax
	int v7; // eax
	//unsigned __int16 v8; // ax
	__int16 v9; // ax
	__int16 v10; // dx
	type_entity_0x6E8E* resultx; // eax
	__int16 v12; // dx
	axis_3d* v13x; // [esp+4h] [ebp-18h]
	uint32_t v14; // [esp+8h] [ebp-14h]
	type_entity_0x6E8E* v15x; // [esp+Ch] [ebp-10h]
	type_entity_0x6E8E* v16x; // [esp+10h] [ebp-Ch]
	char v17; // [esp+14h] [ebp-8h]
	char v18; // [esp+18h] [ebp-4h]

	//fix
	resultx = 0;
	//fix

	v15x = 0;
	v17 = 0;
	v18 = 0;
	v14 = 0x10000;
	if (a2x->life_0x8 == a2x->maxLife_0x4 && a2x->subSpellIndex_0x2A_42 >= 2u)
	{
		v2 = a2x->id_0x1A_26;
		a2x->targetId_0x96_150 = 0;
		v3x = Entities_EA3E4[v2];
		v16x = v3x;
		if (v3x > Entities_EA3E4[0] && v3x->class_0x3F_63 == 3)
		{
			for (ix = gameState_D41A4.miscList_38535; ix > Entities_EA3E4[0]; ix = ix->next_0)
			{
				if (ix->model_0x40_64 == 78
					&& ix->leaderId_0x32_50 != a2x->id_0x1A_26
					&& ix->subSpellIndex_0x2A_42 < 2u)
				{
					v5 = v16x->typeParams_0xA0_160x->range_160_0x1c_28;
					v6 = Maths::Distance3D_583F0(&ix->position_0x4C_76, &a2x->position_0x4C_76);
					if (v6 < v5 && v6 < (signed int)v14)
					{
						v15x = ix;
						v14 = v6;
					}
				}
			}
			if (v15x)
			{
				a2x->maxLife_0x4 = 32;
				a2x->life_0x8 = a2x->maxLife_0x4 - 1;
				a2x->targetId_0x96_150 = v15x - world_D41A0.entities_0x6E8E;
				AimAtEntity_655C0(a2x, v15x);
			}
		}
	}
	v7 = a2x->minSpeed_0x84_132 - a2x->actSpeed_0x82_130;
	if (a2x->minSpeed_0x84_132 != a2x->actSpeed_0x82_130)
	{
		if (v7 <= 0)
			LOWORD(v7) = -1;
		else
			LOWORD(v7) = 1;
	}
	a2x->actSpeed_0x82_130 += 2 * v7;
	if (a2x->targetId_0x96_150)
	{
		//v8 = a2x->targetId_0x96_150;
		//a1x = Entities_EA3E4[a2x->targetId_0x96_150];
		if (Entities_EA3E4[a2x->targetId_0x96_150] <= Entities_EA3E4[0] || Entities_EA3E4[a2x->targetId_0x96_150]->life_0x8 < 0 || Entities_EA3E4[a2x->targetId_0x96_150]->flags_0xc_12_15.byte[1] & 4)
		{
			a2x->targetId_0x96_150 = 0;
		}
		else
		{
			AimAtEntityLeading_65610(a2x, Entities_EA3E4[a2x->targetId_0x96_150]);
			if (EntitiesOverlap_106C0(a2x, Entities_EA3E4[a2x->targetId_0x96_150]))
				v17 = 1;
		}
	}
	v13x = &a2x->position_0x4C_76;
	if (v17)
	{
		RaiseToEntityCentre_65580(Entities_EA3E4[a2x->targetId_0x96_150]);
		CopyEntityPosition_57CF0(a2x, &Entities_EA3E4[a2x->targetId_0x96_150]->position_0x4C_76);
		LowerFromEntityCentre_655A0(Entities_EA3E4[a2x->targetId_0x96_150]);
		Entities_EA3E4[a2x->targetId_0x96_150]->levelPar3_0x46_70 = 8;
		CreateEntity_4A190(v13x, 10, 0);
		DisableEntityDrawing04_57F10(a2x);
	}
	else
	{
		predictedAxis_EB398ar = a2x->position_0x4C_76;
		MoveEntity_57FA0(&predictedAxis_EB398ar, a2x->yaw_0x1C_28, a2x->pitch_0x1E_30, a2x->actSpeed_0x82_130);
		CopyEntityPosition_57CF0(a2x, &predictedAxis_EB398ar);
		v9 = GetTerrainAlt_10C40(v13x);
		v10 = a2x->position_0x4C_76.z;
		predictedAxis_EB398ar.z = v9;
		if (v9 > v10 || isCaveLevel_D41B6 && (predictedAxis_EB398ar.z = GetCeilingAlt_10C60(v13x) - a2x->params_0x52_82.fov, a2x->position_0x4C_76.z > predictedAxis_EB398ar.z))
		{
			v18 = 1;
			a2x->position_0x4C_76.z = predictedAxis_EB398ar.z;
		}
		//result = *(x_DWORD *)(a2 + 8) - 1;
		a2x->life_0x8 = a2x->life_0x8 - 1;
		if (a2x->life_0x8 < 0)
			v18 = 1;
		if (v18)
		{
			resultx = CreateEntity_4A190(&a2x->position_0x4C_76, a2x->levelPar1_0x43_67, a2x->levelPar2_0x44_68);
			if (resultx)
			{
				resultx->leaderId_0x32_50 = a2x->id_0x1A_26;
				v12 = a2x->subSpellIndex_0x2A_42;
				resultx->levelPar3_0x46_70 = 0;
				resultx->subSpellIndex_0x2A_42 = v12;
				DisableEntityDrawing04_57F10(a2x);
			}
		}
	}
}
// 10000: using guessed type void /*__noreturn*/ sub_10000();
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// D41B6: using guessed type char x_BYTE_D41B6;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

int debugcounter_249226 = 0;

//----- (00067CB0) --------------------------------------------------------
signed int UpdateProjectile_67CB0(type_entity_0x6E8E* a1x)//248cb0
{
	unsigned int v1; // edi
	type_entity_0x6E8E* iix; // esi
	unsigned int v3; // ST10_4
	unsigned int v4; // eax
	type_entity_0x6E8E* kkx; // esi
	unsigned int v6; // eax
	type_entity_0x6E8E* v8x; // esi
	type_entity_0x6E8E* ix; // edi
	unsigned int v10; // ST10_4
	unsigned int v11; // eax
	type_entity_0x6E8E* kx; // edi
	__int16 v13; // cx
	unsigned int v14; // eax
	type_entity_0x6E8E* lx; // edi
	int m; // eax
	unsigned int v17; // eax
	type_entity_0x6E8E* v18x; // edi
	type_entity_0x6E8E* llx; // esi
	unsigned int v20; // ST10_4
	unsigned int v21; // eax
	type_entity_0x6E8E* nnx; // esi
	__int16 v23; // cx
	unsigned int v24; // eax
	unsigned int v25; // edi
	type_entity_0x6E8E* v26x; // esi
	unsigned __int8 v27; // al
	int v28; // eax
	int v29; // edx
	unsigned int v30; // eax
	type_entity_0x6E8E* i3x; // esi
	unsigned int v32; // eax
	type_entity_0x6E8E* i4x; // esi
	int i5; // eax
	unsigned int v35; // eax
	type_entity_0x6E8E* v36x; // edi
	type_entity_0x6E8E* nx; // esi
	unsigned int v38; // ST10_4
	unsigned int v39; // eax
	type_entity_0x6E8E* v40x; // edi
	type_entity_0x6E8E* i2x; // esi
	unsigned int v42; // eax
	type_entity_0x6E8E* v43x; // [esp+4h] [ebp-40h]
	type_entity_0x6E8E* v44x; // [esp+8h] [ebp-3Ch]
	type_entity_0x6E8E* v45x; // [esp+Ch] [ebp-38h]
	signed int j; // [esp+10h] [ebp-34h]
	unsigned int v47; // [esp+14h] [ebp-30h]
	unsigned int v48; // [esp+18h] [ebp-2Ch]
	signed int jj; // [esp+1Ch] [ebp-28h]
	signed int mm; // [esp+20h] [ebp-24h]
	signed int i1; // [esp+24h] [ebp-20h]
	type_entity_0x6E8E* v52x; // [esp+28h] [ebp-1Ch]
	type_entity_0x6E8E* v53x; // [esp+2Ch] [ebp-18h]
	type_entity_0x6E8E* v54x; // [esp+30h] [ebp-14h]
	unsigned int v55; // [esp+34h] [ebp-10h]
	type_entity_0x6E8E* v56x; // [esp+38h] [ebp-Ch]
	unsigned int v57; // [esp+40h] [ebp-4h]

	switch (a1x->model_0x40_64)
	{
	case 0:
	case 3:
	case 4:
	case 0x12:
	case 0x13:
	case 0x16:
	case 0x1A:
	case 0x1C:
	case 0x1E:
		v57 = -1;
		v43x = Entities_EA3E4[a1x->id_0x1A_26];
		v8x = 0;
		for (ix = gameState_D41A4.class3List_38519; ix > Entities_EA3E4[0]; ix = ix->next_0)
		{
			if (ix->id_0x1A_26 != a1x->id_0x1A_26 && !(ix->flags_0xc_12_15.byte[0] & 0x20))
			{
				v10 = v43x->typeParams_0xA0_160x->range_160_0x1c_28;
				if (Maths::Distance3D_583F0(&ix->position_0x4C_76, &ix->position_0x4C_76) <= v10)
				{
					v11 = ix->model_0x40_64 == 2 ? ProjectileTurnTowardsTarget_685D0(a1x, ix, 0x71u, 0x71u) : ProjectileHomeOnTarget_68490(
						a1x,
						ix,
						0x71u,
						0x71u);
					if (v11 < v57)
					{
						v8x = ix;
						v57 = v11;
					}
				}
			}
		}
		for (j = 0; j < 29; j++)
		{
			if (j != 22)
			{
				for (kx = gameState_D41A4.creatureLists_38403x[j]; kx > Entities_EA3E4[0]; kx = kx->next_0)
				{
					v13 = a1x->id_0x1A_26;
					if (kx->id_0x1A_26 != v13
						&& kx->timer_0x39_57
						&& (kx->StageVar2_0x49_73 != 14 || kx->parentId_0x28_40 != v13))
					{
						v14 = ProjectileHomeOnTarget_68490(a1x, kx, 0x71u, 0x71u);
						if (v14 < v57)
						{
							v8x = kx;
							v57 = v14;
						}
					}
				}
			}
		}
		if (!v8x)
		{
			for (lx = gameState_D41A4.creatureLists_38403x[88 / 4]; lx > Entities_EA3E4[0]; lx = lx->next_0)
			{
				if (lx->timer_0x39_57)
				{
					for (m = lx->nextLinkedId_0x34_52; ; m = v53x->nextLinkedId_0x34_52)
					{
						v53x = Entities_EA3E4[m];
						if (v53x == Entities_EA3E4[0])
							break;
						v17 = ProjectileHomeOnTarget_68490(a1x, v53x, 0x71u, 0x71u);
						if (v17 < v57)
						{
							v8x = v53x;
							v57 = v17;
						}
					}
				}
			}
		}
		if (!v8x)
			return 0;
		a1x->targetId_0x96_150 = v8x - world_D41A0.entities_0x6E8E;
		IsCreatureModel0_68BD0(a1x, v8x);
		AimAtEntity_655C0(a1x, v8x);
		if (v8x->class_0x3F_63 == 3 && !v8x->model_0x40_64)
			SetWizardHitTimer_5EF70(v8x);
		return 1;
	case 1:
	case 0x11:
		v25 = -1;
		v56x = 0;
		v26x = gameState_D41A4.manaSphereList_38523;
		break;
	case 7:
	case 8:
	case 0xB:
	case 0xC:
		v48 = -1;
		v36x = 0;
		v44x = Entities_EA3E4[a1x->id_0x1A_26];
		for (nx = gameState_D41A4.class3List_38519; nx > Entities_EA3E4[0]; nx = nx->next_0)
		{
			if (nx->id_0x1A_26 != a1x->id_0x1A_26 && !(nx->flags_0xc_12_15.byte[0] & 0x20))
			{
				v38 = v44x->typeParams_0xA0_160x->range_160_0x1c_28;
				if (Maths::Distance3D_583F0(&nx->position_0x4C_76, &a1x->position_0x4C_76) <= v38)
				{
					v39 = ProjectileHomeOnTarget_68490(a1x, nx, 0x71u, 0x71u);
					if (v39 < v48)
					{
						v36x = nx;
						v48 = v39;
					}
				}
			}
		}
		if (!v36x)
			return 0;
		a1x->targetId_0x96_150 = v36x - world_D41A0.entities_0x6E8E;
		AimAtEntity_655C0(a1x, v36x);
		if (v36x->class_0x3F_63 == 3 && !v36x->model_0x40_64)
			SetWizardHitTimer_5EF70(v36x);
		return 1;
	case 9:
		v1 = -1;
		v54x = 0;
		for (iix = gameState_D41A4.class3List_38519; iix > Entities_EA3E4[0]; iix = iix->next_0)
		{
			if (iix->id_0x1A_26 != a1x->id_0x1A_26 && !(iix->flags_0xc_12_15.byte[0] & 0x20))
			{
				v3 = a1x->minSpeed_0x84_132 * a1x->maxLife_0x4;
				if (Maths::Distance3D_583F0(&iix->position_0x4C_76, &a1x->position_0x4C_76) <= v3)
				{
					v4 = iix->model_0x40_64 == 2 ? ProjectileTurnTowardsTarget_685D0(a1x, iix, 0x71u, 0x71u) : ProjectileHomeOnTarget_68490(
						a1x,
						iix,
						0x71u,
						0x71u);
					if (v4 < v1)
					{
						v54x = iix;
						v1 = v4;
					}
				}
			}
		}
		for (jj = 0; jj < 29; ++jj)
		{
			for (kkx = gameState_D41A4.creatureLists_38403x[jj]; kkx > Entities_EA3E4[0]; kkx = kkx->next_0)
			{
				if (kkx->id_0x1A_26 != a1x->id_0x1A_26
					&& kkx->timer_0x39_57
					&& (kkx->StageVar2_0x49_73 != 14 || kkx->parentId_0x28_40 != a1x->id_0x1A_26))//248D8C mov dx,[esi+28h]
				{
					v6 = ProjectileHomeOnTarget_68490(a1x, kkx, 0x71u, 0x200u);
					if (v6 < v1)
					{
						v54x = kkx;
						v1 = v6;
					}
				}
			}
		}
		if (!v54x)
			return 0;
		a1x->targetId_0x96_150 = v54x - world_D41A0.entities_0x6E8E;
		AimAtEntity_655C0(a1x, v54x);
		return 1;
	case 0x10:
		v55 = -1;
		v45x = Entities_EA3E4[a1x->id_0x1A_26];
		v18x = 0;
		for (llx = gameState_D41A4.class3List_38519; llx > Entities_EA3E4[0]; llx = llx->next_0)
		{
			if (llx->id_0x1A_26 != a1x->id_0x1A_26 && !(llx->flags_0xc_12_15.byte[0] & 0x20))
			{
				v20 = v45x->typeParams_0xA0_160x->range_160_0x1c_28;
				if (Maths::Distance3D_583F0(&llx->position_0x4C_76, &a1x->position_0x4C_76) <= v20)
				{
					v21 = llx->model_0x40_64 == 2 ? ProjectileTurnTowardsTarget_685D0(a1x, llx, 0x100u, 0x71u) : ProjectileHomeOnTarget_68490(
						a1x,
						llx,
						0x100u,
						0x71u);
					if (v21 < v55)
					{
						v18x = llx;
						v55 = v21;
					}
				}
			}
		}
		for (mm = 0; mm < 29; ++mm)
		{
			for (nnx = gameState_D41A4.creatureLists_38403x[mm]; nnx > Entities_EA3E4[0]; nnx = nnx->next_0)
			{
				v23 = a1x->id_0x1A_26;
				if (nnx->id_0x1A_26 != v23
					&& nnx->timer_0x39_57
					&& (nnx->StageVar2_0x49_73 != 14 || nnx->parentId_0x28_40 != v23))
				{
					v24 = ProjectileHomeOnTarget_68490(a1x, nnx, 0x100u, 0x71u);
					if (v24 < v55)
					{
						v18x = nnx;
						v55 = v24;
					}
				}
			}
		}
		if (!v18x)
			return 0;
		a1x->targetId_0x96_150 = v18x - world_D41A0.entities_0x6E8E;
		AimAtEntity_655C0(a1x, v18x);
		if (v18x->class_0x3F_63 == 3 && !v18x->model_0x40_64)
			SetWizardHitTimer_5EF70(v18x);
		return 1;
	case 0x19:
		v40x = 0;
		v47 = -1;
		for (i1 = 0; i1 < 29; ++i1)
		{
			for (i2x = gameState_D41A4.creatureLists_38403x[i1]; i2x > Entities_EA3E4[0]; i2x = i2x->next_0)
			{
				if (i2x->id_0x1A_26 != a1x->id_0x1A_26)
				{
					if (i2x->timer_0x39_57)
					{
						if (IsCreatureAffectable_3A7F0(i2x))
						{
							v42 = ProjectileHomeOnTarget_68490(a1x, i2x, 0x71u, 0x71u);
							if (v42 < v47)
							{
								v40x = i2x;
								v47 = v42;
							}
						}
					}
				}
			}
		}
		if (!v40x)
			return 0;
		a1x->targetId_0x96_150 = v40x - world_D41A0.entities_0x6E8E;
		AimAtEntity_655C0(a1x, v40x);
		return 1;
	default:
		return 0;
	}
	while (v26x > Entities_EA3E4[0])//adress 2481e6
	{
		v27 = v26x->model_0x40_64;
		if (v27 < 0x27u)
			goto LABEL_105;
		if (v27 <= 0x27u)
		{
			v28 = a1x->id_0x1A_26;
			v29 = v26x->playerEntityIndex_0x94_148;
		}
		else
		{
			if (v27 != 57)
				goto LABEL_105;
			v28 = a1x->id_0x1A_26;
			v29 = v26x->parentId_0x28_40;
		}
		if (v29 != v28)
		{
			if (v26x->timer_0x39_57)
			{
				v30 = ProjectileHomeOnTarget_68490(a1x, v26x, 0x71u, 0x71u);
				if (v30 < v25)
				{
					v56x = v26x;
					v25 = v30;
				}
			}
		}
	LABEL_105:
		v26x = v26x->next_0;
	}//adress 2481ee
	for (i3x = gameState_D41A4.buildingList_38527; i3x > Entities_EA3E4[0]; i3x = i3x->next_0)
	{
		if (i3x->playerEntityIndex_0x94_148 != a1x->id_0x1A_26)
		{
			if (i3x->timer_0x39_57)
			{
				if (!(buildingParams_D93C0[i3x->levelPar3_0x46_70].flags_2 & 8))
				{
					if (debugcounter_249226 >= 0x9)
					{
						debugcounter_249226 = 1 * debugcounter_249226;
					}
					debugcounter_249226++;
					//adress 249226
					v32 = ProjectileHomeOnTarget_68490(a1x, i3x, 0x71u, 0x71u);
					//adress 24922c
					if (v32 < v25)
					{
						v56x = i3x;
						v25 = v32;
					}
				}
			}
		}
	}
	for (i4x = gameState_D41A4.creatureLists_38403x[88 / 4]; i4x > Entities_EA3E4[0]; i4x = i4x->next_0)
	{
		if (i4x->timer_0x39_57)
		{
			for (i5 = i4x->nextLinkedId_0x34_52; ; i5 = v52x->nextLinkedId_0x34_52)
			{
				v52x = Entities_EA3E4[i5];
				if (v52x == Entities_EA3E4[0])
					break;
				v35 = ProjectileHomeOnTarget_68490(a1x, v52x, 0x71u, 0x71u);
				if (v35 < v25)
				{
					v25 = v35;
					v56x = v52x;
				}
			}
		}
	}
	if (!v56x)
		return 0;
	a1x->targetId_0x96_150 = v56x - world_D41A0.entities_0x6E8E;
	AimAtEntity_655C0(a1x, v56x);
	return 1;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00068490) --------------------------------------------------------
int ProjectileHomeOnTarget_68490(type_entity_0x6E8E* a1y, type_entity_0x6E8E* a2x, unsigned __int16 a3, unsigned __int16 a4)//249490
{
	//x_WORD *v4; // edi
	__int16 v5; // ax
	int result; // eax
	__int16 v7; // ax
	signed int v8; // ebx
	int v9; // esi
	int v10; // edi
	int v11; // eax
	int v12; // edx
	unsigned __int16 v13; // [esp+0h] [ebp-8h]
	unsigned __int16 v14; // [esp+4h] [ebp-4h]
	//a1+0x4c==804e 80de 0414 4b00 b400
	RaiseToEntityCentre_65580(a2x);
	//v4 = a1 + 38;
	v5 = Maths::GetYawAngle_581E0(&a1y->position_0x4C_76, &a2x->position_0x4C_76);//problem je zde
	v13 = Maths::AngleDifference_582B0(a1y->yaw_0x1C_28, v5);
	if (v13 <= a3)
	{
		v7 = Maths::GetPitchAngle_58210(&a1y->position_0x4C_76, &a2x->position_0x4C_76);
		v14 = Maths::AngleDifference_582B0(a1y->pitch_0x1E_30, v7);
		if (v14 <= a4)
		{
			v8 = Maths::DistanceXY_58490(&a1y->position_0x4C_76, &a2x->position_0x4C_76);
			if (v8 <= 5120)
			{
				LowerFromEntityCentre_655A0(a2x);
				v9 = v8 * Maths::sin_DB750[0x200 + v13];
				v10 = v8 * Maths::sin_DB750[v13];
				v11 = v8 * Maths::sin_DB750[0x200 + v14];
				v12 = 4 * Maths::sin_DB750[v14] * v8 >> 16;
				result = (v11 >> 16) * (v11 >> 16) + (v9 >> 16) * (v9 >> 16) + (4 * v10 >> 16) * (4 * v10 >> 16) + v12 * v12;
			}
			else
			{
				LowerFromEntityCentre_655A0(a2x);
				result = -1;
			}
		}
		else
		{
			LowerFromEntityCentre_655A0(a2x);
			result = -1;
		}
	}
	else
	{
		LowerFromEntityCentre_655A0(a2x);
		result = -1;
	}
	return result;
}
// DBF50: using guessed type void (/*__noreturn*/ *off_DBF50[2])();

//----- (000685D0) --------------------------------------------------------
int ProjectileTurnTowardsTarget_685D0(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x, unsigned __int16 a3, unsigned __int16 a4)//2495d0
{
	axis_3d* v4x; // ebx
	axis_3d* v5x; // esi
	__int16 v6; // ax
	__int16 v8; // ax
	unsigned __int16 v9; // di
	signed int v10; // eax
	int v11; // ebx
	int v12; // ecx
	int v13; // esi
	int v14; // eax
	unsigned __int16 v15; // [esp+0h] [ebp-4h]

	v4x = &a2x->position_0x4C_76;
	v5x = &a1x->position_0x4C_76;
	v6 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &a2x->position_0x4C_76);
	v15 = Maths::AngleDifference_582B0(a1x->yaw_0x1C_28, v6);
	if (v15 > a3)
		return -1;
	v8 = Maths::GetPitchAngle_58210(v5x, v4x);
	v9 = Maths::AngleDifference_582B0(a1x->pitch_0x1E_30, v8);
	if (v9 > a4)
		return -1;
	v10 = Maths::DistanceXY_58490(v5x, v4x);
	if (v10 > 5120)
		return -1;
	v11 = v10 * (x_DWORD)Maths::sin_DB750[0x200 + v15];
	v12 = v10 * Maths::sin_DB750[v15];
	v13 = v10 * (x_DWORD)Maths::sin_DB750[0x200 + v9];
	v14 = 4 * Maths::sin_DB750[v9] * v10 >> 16;
	return (4 * v12 >> 16) * (4 * v12 >> 16) + (v11 >> 16) * (v11 >> 16) + (v13 >> 16) * (v13 >> 16) + v14 * v14;
}
// DBF50: using guessed type void (/*__noreturn*/ *off_DBF50[2])();

//----- (00068740) --------------------------------------------------------
char ProjectileHitTarget_68740(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x, unsigned __int16 a3, __int16 a4)//249740
{
	char v4; // cl
	char v5; // ch
	unsigned __int8 v6; // al
	__int16 v7; // ax
	int v8; // esi
	int v9; // eax
	__int16 v10; // dx
	type_entity_0x6E8E* v11x; // eax

	v4 = 0;
	v5 = 0;
	if ((a1x->mana_0x90_144 - (__CFSHL__(a1x->mana_0x90_144 >> 31, 2) + 4 * (a1x->mana_0x90_144 >> 31))) >> 2 > a2x->mana_0x90_144)
		return v4;
	if (a1x->levelPar1_0x43_67 != 10)
	{
		if (a1x->model_0x40_64 != 13)
			goto LABEL_23;
		goto LABEL_22;
	}
	v6 = a1x->levelPar2_0x44_68;
	if (v6 >= 0x11u)
	{
		if (v6 > 0x11u)
		{
			if (v6 < 0x43u)
			{
				if (v6 != 22)
					goto LABEL_23;
			}
			else if (v6 > 0x43u && (v6 < 0x47u || v6 > 0x47u && v6 != 89))
			{
				goto LABEL_23;
			}
		}
	LABEL_22:
		v5 = 1;
		goto LABEL_23;
	}
	if (v6 < 9u)
	{
		if (v6 > 1u)
			goto LABEL_23;
		goto LABEL_22;
	}
	if (v6 <= 9u || v6 >= 0xBu && (v6 <= 0xBu || v6 == 15))
		goto LABEL_22;
LABEL_23:
	if (v5)
	{
		PrepareEventSound_6E450(a2x - world_D41A0.entities_0x6E8E, -1, 28);
		SendSpellMessage_6D8B0(a2x - world_D41A0.entities_0x6E8E, 8u, 1);
		a2x->mana_0x90_144 -= (a1x->mana_0x90_144
			- (__CFSHL__(a1x->mana_0x90_144 >> 31, 2)
				+ 4 * (a1x->mana_0x90_144 >> 31))) >> 2;
		v7 = a1x->yaw_0x1C_28;
		HIBYTE(v7) = (HIBYTE(v7) + 4) & 7;
		a1x->roll_0x20_32 = v7;
		v8 = Maths::AngleTurnDirection_582F0(0, a1x->pitch_0x1E_30);
		v9 = -(v8 * Maths::AngleDifference_582B0(0, a1x->pitch_0x1E_30));
		BYTE1(v9) &= 7u;
		a1x->fov_0x22_34 = v9;
		a1x->pitch_0x1E_30 = v9;
		if (a2x->flags_0xc_12_15.byte[0] & 0x10)
		{
			v10 = a1x->subSpellIndex_0x2A_42;
			a1x->yaw_0x1C_28 = a1x->roll_0x20_32;
			a1x->subSpellIndex_0x2A_42 = 2 * v10;
		}
		else
		{
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			a1x->yaw_0x1C_28 = a1x->roll_0x20_32 + a1x->rand_0x14_20 % a3 - a4;
		}
		a1x->targetId_0x96_150 = a1x->id_0x1A_26;
		a1x->id_0x1A_26 = a2x->id_0x1A_26;
		a1x->life_0x8 = a1x->maxLife_0x4;
		v11x = Entities_EA3E4[a1x->targetId_0x96_150];
		a1x->xtype_0x41_65 = v11x->class_0x3F_63;
		a1x->xsubtype_0x42_66 = v11x->model_0x40_64;
		predictedAxis_EB398ar = a2x->position_0x4C_76;
		predictedAxis_EB398ar.z += a2x->params_0x52_82.fov;
		CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
		v4 = 1;
	}
	return v4;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (00068940) --------------------------------------------------------
char FindProjectileTarget_68940(type_entity_0x6E8E* a1x)//249940
{
	unsigned __int8 v1; // al
	__int16 v2; // dx
	type_entity_0x6E8E* ix; // ebx
	signed int v4; // ST08_4
	signed int v5; // edi
	__int16 v6; // ax
	uint32_t v8; // [esp+8h] [ebp-10h]
	type_entity_0x6E8E* v9x; // [esp+Ch] [ebp-Ch]
	type_entity_0x6E8E* v10x; // [esp+10h] [ebp-8h]
	char v11; // [esp+14h] [ebp-4h]

	v11 = 0;
	v10x = 0;
	v1 = a1x->model_0x40_64;
	v8 = 0x10000;
	if (v1 < 0xCu)
	{
		if (v1 < 2u)
		{
			if (v1)
				return v11;
		}
		else if (v1 > 5u && (v1 < 8u || v1 > 9u))
		{
			return v11;
		}
	}
	else if (v1 > 0xCu)
	{
		if (v1 < 0x1Au)
		{
			if (v1 < 0x16u || v1 > 0x17u)
				return v11;
		}
		else if (v1 > 0x1Au && (v1 < 0x1Cu || v1 > 0x1Cu && v1 != 30))
		{
			return v11;
		}
	}
	v2 = a1x->id_0x1A_26;
	if (v2)
	{
		v9x = Entities_EA3E4[v2];
		if (v9x->class_0x3F_63 == 3)
		{
			for (ix = gameState_D41A4.miscList_38535; ix > Entities_EA3E4[0]; ix = ix->next_0)
			{
				if (ix->model_0x40_64 == 78
					&& ix->leaderId_0x32_50 == a1x->id_0x1A_26
					&& ix->param3_0x36_54 == 0xFFFF)
				{
					v4 = v9x->typeParams_0xA0_160x->range_160_0x1c_28;
					v5 = Maths::Distance3D_583F0(&ix->position_0x4C_76, &v9x->position_0x4C_76);
					if (v5 < v4 && v5 < (signed int)v8)
					{
						v6 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &ix->position_0x4C_76);
						if ((unsigned __int16)Maths::AngleDifference_582B0(a1x->yaw_0x1C_28, v6) < 0xAAu)
						{
							v10x = ix;
							v8 = v5;
						}
					}
				}
			}
			if (v10x)
			{
				a1x->targetId_0x96_150 = v10x - world_D41A0.entities_0x6E8E;
				v11 = 1;
				AimAtEntity_655C0(a1x, v10x);
			}
		}
	}
	return v11;
}
// 10000: using guessed type void /*__noreturn*/ sub_10000();
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00068AC0) --------------------------------------------------------
char CanProjectileHitEntity_68AC0(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//249ac0
{
	unsigned __int8 v2; // al
	char v3; // dl
	unsigned __int16 v4; // di
	type_entity_0x6E8E* v5x; // edx

	v2 = a1x->model_0x40_64;
	v3 = 0;
	if (v2 < 0xCu)
	{
		if (v2 < 2u)
		{
			if (v2)
				return v3;
		}
		else if (v2 > 5u && (v2 < 8u || v2 > 9u))
		{
			return v3;
		}
	}
	else if (v2 > 0xCu)
	{
		if (v2 < 0x1Au)
		{
			if (v2 < 0x16u || v2 > 0x17u)
				return v3;
		}
		else if (v2 > 0x1Au && (v2 < 0x1Cu || v2 > 0x1Cu && v2 != 30))
		{
			return v3;
		}
	}
	if (a2x
		&& a2x->class_0x3F_63 == 10
		&& a2x->model_0x40_64 == 78
		&& a2x->leaderId_0x32_50 == a1x->id_0x1A_26
		&& a2x->param3_0x36_54 == 0xFFFF)
	{
		CreateEntity_4A190(&a1x->position_0x4C_76, 10, 0);
		PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 26);
		v4 = a1x->hitById_0x26_38;
		if (v4)
		{
			v5x = Entities_EA3E4[v4];
			a2x->param3_0x36_54 = v5x->model_0x40_64;
			a2x->nextLinkedId_0x34_52 = v5x->levelPar3_0x46_70;
			v5x->stage_0x2E_46 = 1;
		}
		v3 = 1;
	}
	return v3;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00068C70) --------------------------------------------------------
int UpdateChainedProjectile_68C70(type_entity_0x6E8E* a1x)//249c70
{
	char v1; // dh
	int v2; // eax
	type_entity_0x6E8E* v3x; // eax
	int result; // eax
	int i; // eax
	type_entity_0x6E8E* v6x; // eax

	if (a1x->timer_0x39_57)
	{
		for (i = a1x->nextLinkedId_0x34_52; ; i = v6x->nextLinkedId_0x34_52)
		{
			v6x = Entities_EA3E4[i];
			if (v6x <= Entities_EA3E4[0])
				break;
			v6x->timer_0x39_57 = a1x->timer_0x39_57;
		}
		a1x->timer_0x39_57--;
		return 0;
	}
	if (a1x->flags_0xc_12_15.byte[0] & 1)
		return 0;
	v1 = a1x->chainCount_0x3A_58;
	if (v1)
	{
		a1x->chainCount_0x3A_58 = v1 - 1;
		result = 0;
	}
	else
	{
		if (Maths::DistanceSquaredXY_584D0(
			&a1x->position_0x4C_76, &Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240]->position_0x4C_76) < 0x2400000)
		{
			v2 = a1x->nextLinkedId_0x34_52;
			a1x->timer_0x39_57 = 16;
			while (1)
			{
				v3x = Entities_EA3E4[v2];
				if (v3x <= Entities_EA3E4[0])
					break;
				v3x->timer_0x39_57 = a1x->timer_0x39_57 + 2;
				v2 = v3x->nextLinkedId_0x34_52;
			}
		}
		a1x->chainCount_0x3A_58 = 0;
		result = 0;
	}
	return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];
