#include "WizardAi.h"
#include "EventsFunctions.h"
#include "Castle.h"
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

__int16 spellCooldownTable_D3F4C[26] = { 2,10,40,32,300,1,1,1,1,4,1,1,0,0,0,0,0,0,400,600,600,400,400,0,0,0 }; // idb

uint8_t unk_D3F80x[9] = { 0x10,0x12,0x09,0x07,0x14,0x15,0x13,0x00,0xFF };

uint8_t unk_D3F89x[8] = { 0x10,0x12,0x07,0x09,0x11,0x14,0x00,0xFF };

uint8_t unk_D3F91x[5] = { 0x02,0x13,0x19,0x10,0xFF };

char avoidOffsetsX1_D3F96[14] = { 0x00,0xff,0x00,0xff,0x01,0xff,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00 }; // idb

char avoidOffsetsY1_D3FA4[14] = { 0x00,0x00,0xff,0x00,0x00,0x00,0xff,0xff,0x01,0x01,0x00,0x01,0x00,0x00 }; // weak

char avoidOffsetsX2_D3FB2[14] = { 0x00,0x01,0x00,0x00,0xff,0x01,0xff,0x00,0x00,0x01,0x01,0x00,0x00,0x00 }; // idb

char avoidOffsetsY2_D3FC0[14] = { 0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x01,0xff,0x00,0x00,0x01,0xff,0x00 }; // weak

__int16 steerYawTable1_D3FCE[13] = { 0, 1536, 0, 1536, 512, 1536, 0, 0, 1024, 1024, 1536, 1024, 512 }; // idb

__int16 steerYawTable2_D3FE8[14] = { 1024, 512, 1024, 1024, 1536, 512, 1536, 1024, 0, 512, 512, 1024, 0, 0 }; // idb

type_entity_0x6E8E* x_DWORD_E8840; // weak

//----- (00012910) --------------------------------------------------------
signed int UpdateWizardAi_12910(type_entity_0x6E8E* event)//1f3910
{
	signed int result; // eax

	if (CommandLineParams.DoDebugSequences()) {
		//add_compare(0x1f3912, CommandLineParams.DoDebugafterload(),0x19);
	}

	UpdateWizardAiState_12A70(event);
	switch (event->playerData_0xA4_164x->aiState_0x1C1_449)
	{
	case 0:
		WizardAiChooseState_12E70(event);
		result = WizardAiChooseState_12E70(event);
		break;
	case 1:
		WizardAiAttackTarget_12FF0(event);
		result = WizardAiChooseState_12E70(event);
		break;
	case 2:
		_nmemneed((uint8_t*)event);
		result = WizardAiChooseState_12E70(event);
		break;
	case 3:
		WizardAiMoveToPosition_13100(event);
		result = WizardAiChooseState_12E70(event);
		break;
	case 4:
		WizardAiFaceTarget_131F0(event);
		result = WizardAiChooseState_12E70(event);
		break;
	case 5:
		_nmemneed_0((uint8_t*)event);
		result = WizardAiChooseState_12E70(event);
		break;
	case 6:
		WizardAiPossessTarget_135C0(event);
		result = WizardAiChooseState_12E70(event);
		break;
	case 7:
		WizardAiAttackWithSpell_13710(event);
		result = WizardAiChooseState_12E70(event);
		break;
	case 8:
		WizardAiCombat2_13830(event);
		result = WizardAiChooseState_12E70(event);
		break;
	case 9:
		WizardAiCombat4_13870(event);
		result = WizardAiChooseState_12E70(event);
		break;
	case 0xB:
		WizardAiReturnToCastle_133B0(event);
		result = WizardAiChooseState_12E70(event);
		break;
	case 0xC:
		WizardAiCastFoolsMana_13270(event);
		result = WizardAiChooseState_12E70(event);
		break;
	case 0xD:
		WizardAiCombat3_13850(event);
		result = WizardAiChooseState_12E70(event);
		break;
	case 0xE:
		WizardAiAttackMetamorphed_161A0(event);
		result = WizardAiChooseState_12E70(event);
		break;
	default:
		result = WizardAiChooseState_12E70(event);
		break;
	}
	return result;
}
// 130F0: using guessed type int _nmemneed(x_DWORD);
// 135B0: using guessed type int _nmemneed_0(x_DWORD);

//----- (00012A70) --------------------------------------------------------
signed int UpdateWizardAiState_12A70(type_entity_0x6E8E* a1x)//1f3a70
{
	type_str_164* v1x; // edx
	signed int v2; // edi
	//type_str_0x6E8E* v2x; // edi
	//__int16 v3; // cx
	__int16 v4; // dx
	//int v5; // esi
	//__int16 v6; // ax
	__int16 i; // dx
	//int v8; // ecx
	type_str_164* v9x; // eax
	//int v10; // esi
	//unsigned __int16 v11; // cx
	//int v12; // eax
	//int v13; // eax
	//int v14; // esi
	//int v15; // eax
	unsigned __int16 v16; // dx
	//int v18; // edx
	//unsigned __int8 v19; // ah
	int v20; // ecx
	int v21; // edx
	//int v22; // edx
	//int v23; // ecx
	int v24; // ecx
	int v25; // edx
	type_entity_0x6E8E* v26x; // esi
	signed __int16 j; // si
	int v28; // edx
	//int v29; // eax
	//int v30; // eax
	//int v32; // [esp+0h] [ebp-4h]

	FindLocalWizard_15EE0();
	v1x = a1x->playerData_0xA4_164x;
	v2 = 0;
	//v3 = v1x->word_0x1A2_418;
	if (v1x->aiReactionDelay_0x1A2_418 < 0)
		v1x->aiReactionDelay_0x1A2_418++;
	v4 = 0;
	do
	{
		//v5 = 2 * v4 + a1x->dword_0xA4_164;
		//v6 = *(x_WORD *)(v5 + 871);
		if (a1x->playerData_0xA4_164x->str_611.spellCooldowns_0x367_871x.SpellEnabled[v4] > 0)
			a1x->playerData_0xA4_164x->str_611.spellCooldowns_0x367_871x.SpellEnabled[v4]--;
		v4++;
	} while (v4 < 26);
	for (i = 0; i < 8; i++)
	{
		//v8 = 8 * i;
		v9x = a1x->playerData_0xA4_164x;
		//v32 = v8;
		//v10 = v8 + v9;
		//v9x->word_0x204_516[4*i];
		//v11 = *(x_WORD *)(v8 + v9 + 516);
		if (v9x->aiTargets_0x1FC_508[4 * i + 4] < 0x601Fu)
		{
			v9x->aiTargets_0x1FC_508[4 * i + 4] = v9x->aiAggression_0x242_578 + 1 + v9x->aiTargets_0x1FC_508[4 * i + 4];//0x1F3AF9/0x1F3B11 same [esi+204h]
			//v12 = v32 + a1x->dword_0xA4_164;
			if (v9x->aiTargets_0x1FC_508[4 * i + 4] > 0x601F)
				v9x->aiTargets_0x1FC_508[4 * i + 4] = 0x601F;
		}
		//v13 = a1x->dword_0xA4_164;
		//v14 = v13 + 8 * i;
		if (v9x->aiTargets_0x1FC_508[4 * i + 4] > 0x601Fu)
		{
			if (!v9x->aiTargets_0x1FC_508[4 * i + 4 + 1])
				v9x->aiTargets_0x1FC_508[4 * i + 4] -= 256 - v9x->aiAggression_0x242_578;
			//v15 = a1x->dword_0xA4_164 + 8 * i;
			if (v9x->aiTargets_0x1FC_508[4 * i + 4] < 0x601F)
				v9x->aiTargets_0x1FC_508[4 * i + 4] = 0x601F;
		}
	}
	v16 = a1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58;
	if (v16 && EntitiesOverlap_106C0(a1x, Entities_EA3E4[v16]))
		v2 = 1;
	if (v2)
		a1x->playerData_0xA4_164x->invulnerableTimer_0x159_345 = 2;
	if (a1x->playerData_0xA4_164x->invulnerableTimer_0x159_345)
	{
		memset((void*)&a1x->mail_0x5E_94, 0, 36);
		//memset((void*)(a1 + 94), 0, 36);
		//fix
		/*a1x->mail_0x5E_94.channel[0].amount.dword = 0;
		a1x->mail_0x5E_94.channel[0].amount.dword = 0;//94
		a1x->mail_0x5E_94.channel[0].source = 0;//98 //4
		a1x->mail_0x5E_94.channel[1].amount.dword = 0;//100 //6
		a1x->mail_0x5E_94.channel[1].source = 0;//104 //10
		a1x->stub5b[24];
		memset(a1x->mail_0x5E_94.stub5b, 0, 24);*/
		//fix

		a1x->playerData_0xA4_164x->invulnerableTimer_0x159_345--;
	}
	else if (UpdateCastleTarget_5EFA0(a1x) == 2)
	{
		a1x->actionIndex_0x45_69 = 2;
		return 0;
	}
	UpdateWizardMovement_146F0(a1x);
	//v18 = a1x->dword_0xA4_164;
	//v19 = a1x->dword_0xA4_164->byte_0x154_340;
	if (a1x->playerData_0xA4_164x->chargeTime_0x154_340 < 200)
		a1x->playerData_0xA4_164x->chargeTime_0x154_340++;
	a1x->mana_0x90_144 += a1x->manaRegen_0x88_136;
	v20 = a1x->playerData_0xA4_164x->lifeRegen_0x163_355 + a1x->life_0x8;
	a1x->life_0x8 = v20;
	if (v20 < -1)
		a1x->life_0x8 = -1;
	v21 = a1x->maxLife_0x4;
	if (a1x->life_0x8 > v21)
		a1x->life_0x8 = v21;
	//v22 = a1x->dword_0xA4_164;
	//v23 = *(x_DWORD *)(v22 + 365);
	if (a1x->playerData_0xA4_164x->startTimer_0x16D_365)
		a1x->playerData_0xA4_164x->startTimer_0x16D_365--;
	if (v2 || a1x->flags_0xc_12_15.byte[1] & 0x10)
	{
		a1x->manaRegen_0x88_136 = a1x->maxMana_0x8C_140 / 200;
		a1x->playerData_0xA4_164x->lifeRegen_0x163_355 = a1x->maxLife_0x4 / 200;
		if (a1x->manaRegen_0x88_136 < 1000)
			a1x->manaRegen_0x88_136 = 1000;
		a1x->flags_0xc_12_15.byte[1] &= 0xEFu;
	}
	else
	{
		a1x->manaRegen_0x88_136 = a1x->maxMana_0x8C_140 / 2000;
		a1x->playerData_0xA4_164x->lifeRegen_0x163_355 = a1x->maxLife_0x4 / 500;
		if (a1x->manaRegen_0x88_136 < 100)
			a1x->manaRegen_0x88_136 = 100;
	}
	if (a1x->mana_0x90_144 < 0)
		a1x->mana_0x90_144 = 0;
	v24 = a1x->maxMana_0x8C_140;
	if (a1x->mana_0x90_144 > v24)
		a1x->mana_0x90_144 = v24;
	v25 = a1x->playerData_0xA4_164x->aiReflexes_0x246_582;
	//if (!(a1x->instanceIndex_0x3E_62 % (64 - ((v25 - (__CFSHL__(v25 >> 31, 2) + 4 * (v25 >> 31))) >> 2))))
	if (!(a1x->instanceIndex_0x3E_62 % (64 - (((v25 - (my_sign32(v25) << 2) + my_sign32(v25)) >> 2)))))
	{
		v26x = FindNearestEntityTargeting_15CB0(/*v2, */a1x);
		if (v26x)
		{
			SetWizardStrafeSpeed_15D20(a1x);
			RateWizardTarget_15D40(v2, a1x, v26x);
		}
		if (a1x->life_0x8 < a1x->maxLife_0x4)
		{
			for (j = a1x->playerData_0xA4_164x->str_611.SpellLevels_0x41D_1053z.SpellIndex[5]; j >= 0; j--)
			{
				if (WizardTrySelectSpell_15F20(a1x, j, 5) == 5)
				{
					WizardAiCastSpell_14E10(a1x, 5u);
					break;
				}
			}
		}
	}
	v28 = (signed __int16)GetTerrainAlt_10C40(&a1x->position_0x4C_76);
	//v29 = a1x->dword_0xA0_160;
	if (a1x->position_0x4C_76.z > v28 + a1x->typeParams_0xA0_160x->maxHeight_160_0xa_10)
		a1x->position_0x4C_76.z = a1x->typeParams_0xA0_160x->maxHeight_160_0xa_10 + v28;
	//v30 = a1x->dword_0xA0_160;
	if (a1x->position_0x4C_76.z < v28 + a1x->typeParams_0xA0_160x->minHeight_160_0xc_12)
		a1x->position_0x4C_76.z = a1x->typeParams_0xA0_160x->minHeight_160_0xc_12 + v28;
	return 1;
}
// 5E000: using guessed type int _wcpp_1_unwind_leave__99(x_DWORD);
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// EA3E4: using guessed type int Entities_EA3E4[];

int debug_counter_1f3e70 = 0;
//----- (00012E70) --------------------------------------------------------
signed int WizardAiChooseState_12E70(type_entity_0x6E8E* a1x)//1f3e70
{
	int v2; // edx

	/*
	uint8_t origbyte20 = 0;
	uint8_t remakebyte20 = 0;
	int comp20;
	if (debug_counter_1f3e70 >= 0xb5)
	{
	comp20 = compare_with_sequence((char*)"001F3E70-002DC4E0", (uint8_t*)x_BYTE_10B4E0_terraintype, 0x2dc4e0, debug_counter_1f3e70, 0x70000, 0x10000, &origbyte20, &remakebyte20);
	comp20 = compare_with_sequence((char*)"001F3E70-002DC4E0", (uint8_t*)x_BYTE_11B4E0_height, 0x2dc4e0, debug_counter_1f3e70, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x10000);
	comp20 = compare_with_sequence((char*)"001F3E70-002DC4E0", (uint8_t*)x_BYTE_12B4E0_shading, 0x2dc4e0, debug_counter_1f3e70, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x20000);
	comp20 = compare_with_sequence((char*)"001F3E70-002DC4E0", (uint8_t*)x_BYTE_13B4E0_angle, 0x2dc4e0, debug_counter_1f3e70, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x30000);
	//comp20 = compare_with_sequence((char*)"00228320", (uint8_t*)x_BYTE_14B4E0, 0x2dc4e0, debugcounter11, 0x70000,0x10000, &origbyte20, &remakebyte20, 0x40000);
	comp20 = compare_with_sequence((char*)"001F3E70-002DC4E0", (uint8_t*)x_WORD_15B4E0_source, 0x2dc4e0, debug_counter_1f3e70, 0x70000, 0x20000, &origbyte20, &remakebyte20, 0x50000);

	comp20 = compare_with_sequence_D41A0((char*)"001F3E70-00356038", (uint8_t*)& D41A0_BYTESTR_0, 0x356038, debug_counter_1f3e70, 224790, &origbyte20, &remakebyte20);
	}
	debug_counter_1f3e70++;
	*/
	if (WizardAiTryBuildCastle_13B00(a1x))
	{
		a1x->playerData_0xA4_164x->aiState_0x1C1_449 = 3;
	}
	else
	{
		if (WizardAiShouldRetreat_13DC0(a1x))
		{
			a1x->playerData_0xA4_164x->aiState_0x1C1_449 = 11;
			return 1;
		}
		v2 = a1x->playerData_0xA4_164x->aiReflexes_0x246_582;

		int cftemp = 0;
		if ((v2 >> 30) & 1)
			cftemp = 1;
		//uint8_t vara=a1x->instanceIndex_0x3E_62;
		//int varb=(64 - (((v2 - ((v2 >> 31) << 2)) - cftemp) >> 2));
		if (!(a1x->instanceIndex_0x3E_62 % (64 - (((v2 - ((v2 >> 31) << 2)) - cftemp) >> 2))))
			//if (!(a1x->instanceIndex_0x3E_62 % (64 - ((v2 - (__CFSHL__(v2 >> 31, 2) + 4 * (v2 >> 31))) >> 2))))
		{
			if (WizardAiTryGrowCastle_13C50(a1x))
			{
				a1x->playerData_0xA4_164x->aiState_0x1C1_449 = 1;
				return 1;
			}
			if (WizardAiChooseEnemyWizard_13E40(a1x))
			{
				a1x->playerData_0xA4_164x->aiState_0x1C1_449 = 7;
				return 1;
			}
			if (WizardAiChooseEnemyCreature_14030(a1x))
			{
				a1x->playerData_0xA4_164x->aiState_0x1C1_449 = 8;
				return 1;
			}
			if (WizardAiChooseEnemyCastle_14250(a1x))
			{
				a1x->playerData_0xA4_164x->aiState_0x1C1_449 = 9;
				return 1;
			}
			if ((unsigned __int16)WizardAiMetamorph_15FC0(a1x))
			{
				a1x->playerData_0xA4_164x->aiState_0x1C1_449 = 14;
				return 1;
			}
			if (WizardAiFindTarget_13CE0(a1x))
			{
				a1x->playerData_0xA4_164x->aiState_0x1C1_449 = 6;
				return 1;
			}
			if (WizardAiChooseAttackTarget_14530(a1x))
			{
				a1x->playerData_0xA4_164x->aiState_0x1C1_449 = 13;
				return 1;
			}
			WizardAiCheckHealth_14630(a1x);
		}
	}
	return 1;
}

//----- (00012FF0) --------------------------------------------------------
signed int WizardAiAttackTarget_12FF0(type_entity_0x6E8E* a1x)//1f3ff0
{
	type_entity_0x6E8E* v1x; // esi
	signed __int16 i; // di
	int v3; // eax

	v1x = Entities_EA3E4[a1x->targetId_0x96_150];
	if (!IsTargetStillValid_14C60(a1x, Entities_EA3E4[a1x->targetId_0x96_150]))
		goto LABEL_14;
	a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v1x->position_0x4C_76);
	if (WizardAiMoveTowards_14C90(a1x, v1x, 512, 2048))
	{
		for (i = a1x->playerData_0xA4_164x->str_611.SpellLevels_0x41D_1053z.SpellIndex[2]; i >= 0 && WizardTrySelectSpell_15F20(a1x, i, 2) != 2; i--)
			;
		if (!WizardAiCastSpell_14E10(a1x, 2u))
		{
			a1x->playerData_0xA4_164x->speed_0xc_12 = 0;
			v3 = a1x->position_0x4C_76.z - (v1x->position_0x4C_76.z + 512);
			if (a1x->position_0x4C_76.z != v1x->position_0x4C_76.z + 512)
			{
				if (v3 <= 0)
					v3 = -1;
				//LOWORD(v3) = -1;
				else
					v3 = 1;
				//LOWORD(v3) = 1;
			}
			a1x->position_0x4C_76.z += a1x->typeParams_0xA0_160x->climbSpeed_160_0xe_14 * v3;
			goto LABEL_13;
		}
	LABEL_14:
		WizardAiSteer_16580(a1x);
		return 0;
	}
LABEL_13:
	WizardAiSteer_16580(a1x);
	return 1;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00013100) --------------------------------------------------------
signed int WizardAiMoveToPosition_13100(type_entity_0x6E8E* a1x)//1f4100
{
	signed __int16 i; // si
	int v2; // eax
	signed int v3; // ebx

	a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &a1x->axis_0x9A_154x);
	if (!WizardAiMoveTowards_14C90(a1x, 0, 2048, 4096))
		goto LABEL_16;
	for (i = a1x->playerData_0xA4_164x->str_611.SpellLevels_0x41D_1053z.SpellIndex[2]; i >= 0 && (unsigned __int16)WizardTrySelectSpell_15F20(a1x, i, 2) != 2; i--)
		;
	if (WizardAiCastSpell_14E10(a1x, 2u))
	{
	LABEL_16:
		WizardAiSteer_16580(a1x);
		v3 = 0;
	}
	else
	{
		a1x->playerData_0xA4_164x->speed_0xc_12 = 0;
		v2 = a1x->position_0x4C_76.z - (a1x->axis_0x9A_154x.z + 512);
		if (a1x->position_0x4C_76.z != a1x->axis_0x9A_154x.z + 512)
		{
			if (a1x->position_0x4C_76.z - (a1x->axis_0x9A_154x.z + 512) <= 0)
				v2 = -1;
			//LOWORD(v2) = -1;
			else
				v2 = 1;
			//LOWORD(v2) = 1;
		}
		v3 = 1;
		a1x->position_0x4C_76.z += v2 * a1x->typeParams_0xA0_160x->climbSpeed_160_0xe_14;
		WizardAiSteer_16580(a1x);
	}
	return v3;
}

//----- (000131F0) --------------------------------------------------------
signed int WizardAiFaceTarget_131F0(type_entity_0x6E8E* a1x)//1f41f0
{
	type_entity_0x6E8E* v1x; // esi
	signed int result; // ebx

	v1x = Entities_EA3E4[a1x->targetId_0x96_150];
	if (!IsTargetStillValid_14C60(a1x, Entities_EA3E4[a1x->targetId_0x96_150])
		|| (a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &a1x->position_0x4C_76), WizardAiMoveTowards_14C90(a1x, v1x, 256, 2048)))
	{
		WizardAiSteer_16580(a1x);
		result = 0;
	}
	else
	{
		WizardAiSteer_16580(a1x);
		result = 1;
	}
	return result;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00013270) --------------------------------------------------------
signed int WizardAiCastFoolsMana_13270(type_entity_0x6E8E* a1x)//1f4270
{
	int v1; // edi
	signed __int16 v2; // si
	//int v3; // edx
	unsigned __int16 v4; // cx
	signed __int16 v5; // di
	signed __int16 i; // si
	signed int v7; // ebx
	signed int result; // eax
	type_entity_0x6E8E* v9x; // eax

	v1 = a1x->playerData_0xA4_164x->aiPerception_0x244_580;
	v2 = 0;
	if (watcomrand() % 255 < v1)
	{
		//v3 = a1x->dword_0xA4_164;
		v4 = a1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58;
		v5 = a1x->playerData_0xA4_164x->str_611.SpellLevels_0x41D_1053z.SpellIndex[2];
		if (v4 && EntitiesOverlap_106C0(a1x, Entities_EA3E4[v4]))
			v2 = 1;
		if (!v2)
		{
			for (i = v5; i >= 0; i--)
			{
				if ((unsigned __int16)WizardTrySelectSpell_15F20(a1x, v5, 22) == 22)
				{
					WizardAiCastSpell_14E10(a1x, 0x16u);
					WizardAiSteer_16580(a1x);
					v7 = 1;
					goto LABEL_19;
				}
			}
		}
	}
	if (WizardCanCastSpell_15170(a1x, 3u))
	{
		if (!IsPlayerSpellActive_156F0(a1x, 3u))
			WizardAiCastSpell_14E10(a1x, 3u);
		WizardAiSteer_16580(a1x);
		result = 1;
	}
	else
	{
		WizardAiSteer_16580(a1x);
		v9x = GetPlayerSpellEntity_146C0(a1x, 3u);
		if (v9x && v9x->stage_0x2E_46 > 0)
		{
			result = 1;
		}
		else
		{
			a1x->playerData_0xA4_164x->speed_0xc_12 = a1x->minSpeed_0x84_132;
			a1x->playerData_0xA4_164x->castLock_0xe_14 = 1;
			v7 = 0;
		LABEL_19:
			result = v7;
		}
	}
	return result;
}
// 8C278: using guessed type int rand(void);
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000133B0) --------------------------------------------------------
signed int WizardAiReturnToCastle_133B0(type_entity_0x6E8E* a1x)//1f43b0
{
	//int v1; // eax
	type_entity_0x6E8E* v2x; // esi
	signed __int16 k; // di
	signed __int16 l; // di
	signed __int16 i; // si
	signed __int16 j; // si
	type_entity_0x6E8E* v9x; // eax

	//v1 = a1x->dword_0xA4_164;
	v2x = Entities_EA3E4[a1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58];
	if (v2x <= Entities_EA3E4[0])
	{
		for (i = a1x->playerData_0xA4_164x->str_611.SpellLevels_0x41D_1053z.SpellIndex[11]; i >= 0; i--)
		{
			if ((unsigned __int16)WizardTrySelectSpell_15F20(a1x, i, 11) == 11)
			{
				WizardAiCastSpell_14E10(a1x, 0xBu);
				break;
			}
		}
		for (j = a1x->playerData_0xA4_164x->str_611.SpellLevels_0x41D_1053z.SpellIndex[3]; j >= 0; j--)
		{
			if ((unsigned __int16)WizardTrySelectSpell_15F20(a1x, j, 3) == 3)
			{
				WizardAiCastSpell_14E10(a1x, 3u);
				WizardAiSteer_16580(a1x);
				return 1;
			}
		}
		v9x = GetPlayerSpellEntity_146C0(a1x, 3u);
		if (v9x && v9x->stage_0x2E_46 > 0)
		{
			WizardAiSteer_16580(a1x);
			return 1;
		}
		a1x->playerData_0xA4_164x->speed_0xc_12 = a1x->minSpeed_0x84_132;
		a1x->playerData_0xA4_164x->castLock_0xe_14 = 1;
	}
	else
	{
		if ((unsigned int)Maths::EuclideanDistXY_584D0(&a1x->position_0x4C_76, &v2x->position_0x4C_76) > 0x6400000)
		{
			for (k = a1x->playerData_0xA4_164x->str_611.SpellLevels_0x41D_1053z.SpellIndex[1]; k >= 0; k--)
			{
				if ((unsigned __int16)WizardTrySelectSpell_15F20(a1x, k, 1) == 1 && WizardAiCastSpell_14E10(a1x, 1u))
					goto LABEL_6;
			}
		}
		for (l = a1x->playerData_0xA4_164x->str_611.SpellLevels_0x41D_1053z.SpellIndex[11]; l >= 0; l--)
		{
			if ((unsigned __int16)WizardTrySelectSpell_15F20(a1x, l, 11) == 11)
			{
				WizardAiCastSpell_14E10(a1x, 0xBu);
				break;
			}
		}
		if (IsTargetStillValid_14C60(a1x, v2x))
		{
			a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v2x->position_0x4C_76);
			if (WizardAiMoveTowards_14C90(a1x, v2x, 256, 2048))
			{
				WizardAiSteer_16580(a1x);
				return 0;
			}
		LABEL_6:
			WizardAiSteer_16580(a1x);
			return 1;
		}
	}
	WizardAiSteer_16580(a1x);
	return 0;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000135C0) --------------------------------------------------------
signed int WizardAiPossessTarget_135C0(type_entity_0x6E8E* a1x)//1f45c0
{
	type_entity_0x6E8E* v1x; // esi
	signed __int16 i; // di
	__int16 v3; // ax
	int v4; // esi
	__int16 v7; // [esp+0h] [ebp-4h]

	v1x = Entities_EA3E4[a1x->targetId_0x96_150];
	if (!IsTargetStillValid_14C60(a1x, Entities_EA3E4[a1x->targetId_0x96_150]))
	{
	LABEL_17:
		WizardAiSteer_16580(a1x);
		return 0;
	}
	a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v1x->position_0x4C_76);
	if (WizardAiMoveTowards_14C90(a1x, v1x, 1024, 3072))
	{
		for (i = a1x->playerData_0xA4_164x->str_611.SpellLevels_0x41D_1053z.SpellIndex[1]; i >= 0; i--)
		{
			v7 = WizardTrySelectSpell_15F20(a1x, i, 1);
			if (v7 == 1)
				break;
		}
		if (v7 == 1 && WizardAiCastSpell_14E10(a1x, 1u))
		{
			v3 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v1x->position_0x4C_76);
			if ((unsigned __int16)AngleDifference_582B0(a1x->yaw_0x1C_28, v3) < 0x1Cu)
				v1x->playerEntityIndex_0x94_148 = a1x->id_0x1A_26;
			goto LABEL_17;
		}
		a1x->playerData_0xA4_164x->speed_0xc_12 = 0;
		v4 = a1x->position_0x4C_76.z - (v1x->position_0x4C_76.z + 512);
		if (v4)
		{
			if (v4 <= 0)
				v4 = -1;
			//LOWORD(v4) = -1;
			else
				v4 = 1;
			//LOWORD(v4) = 1;
		}
		a1x->position_0x4C_76.z += v4 * a1x->typeParams_0xA0_160x->climbSpeed_160_0xe_14;
	}
	WizardAiSteer_16580(a1x);
	return 1;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00013710) --------------------------------------------------------
signed int WizardAiAttackWithSpell_13710(type_entity_0x6E8E* a1x)//1f4710
{
	type_entity_0x6E8E* v1x; // esi
	int v2; // edx
	signed __int8 v3; // al
	int v4; // edx

	v1x = Entities_EA3E4[a1x->targetId_0x96_150];
	if (!IsTargetStillValid_14C60(a1x, v1x))
		goto LABEL_12;
	a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v1x->position_0x4C_76);
	if (WizardAiMoveTowards_14C90(a1x, v1x, 2048, 3584))
	{
		v2 = a1x->playerData_0xA4_164x->aiReflexes_0x246_582;
		if (!(a1x->instanceIndex_0x3E_62 % (64 - ((v2 - (__CFSHL__(v2 >> 31, 2) + 4 * (v2 >> 31))) >> 2))))
		{
			v3 = WizardAiChooseSpell_15910(a1x);
			if (v3 == -1 || !WizardAiCastSpell_14E10(a1x, v3))
			{
				a1x->playerData_0xA4_164x->speed_0xc_12 = 0;
				v4 = a1x->position_0x4C_76.z - (v1x->position_0x4C_76.z + 512);
				if (a1x->position_0x4C_76.z != v1x->position_0x4C_76.z + 512)
				{
					if (v4 <= 0)
						v4 = -1;
					//LOWORD(v4) = -1;
					else
						v4 = 1;
					//LOWORD(v4) = 1;
				}
				a1x->position_0x4C_76.z += v4 * a1x->typeParams_0xA0_160x->climbSpeed_160_0xe_14;
				goto LABEL_11;
			}
		LABEL_12:
			WizardAiSteer_16580(a1x);
			return 0;
		}
	}
LABEL_11:
	WizardAiSteer_16580(a1x);
	return 1;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00013830) --------------------------------------------------------
signed int WizardAiCombat2_13830(type_entity_0x6E8E* event)//1f4830
{
	return WizardAiCombat_13890(event);
}

//----- (00013850) --------------------------------------------------------
signed int WizardAiCombat3_13850(type_entity_0x6E8E* event)//1f4850
{
	return WizardAiCombat_13890(event);
}

//----- (00013870) --------------------------------------------------------
int WizardAiCombat4_13870(type_entity_0x6E8E* event)//1f4870
{
	signed int v1 = WizardAiCombat_13890(event);
	return v1;// nullsub_1(v1);
}
// 13880: using guessed type int /*__fastcall*/ nullsub_1(x_DWORD);

//----- (00013890) --------------------------------------------------------
signed int WizardAiCombat_13890(type_entity_0x6E8E* a1x)//1f4890
{
	type_entity_0x6E8E* v1x; // esi
	signed __int8 v2; // al
	unsigned __int8 v3; // di
	//char v4; // ah
	//char v5; // dh
	//int v6; // eax
	//char v7; // ch
	//int v8; // eax
	//char v9; // cl
	//int v10; // eax
	//char v11; // dh
	int v12; // eax

	v1x = Entities_EA3E4[a1x->targetId_0x96_150];
	if (IsTargetStillValid_14C60(a1x, v1x))
	{
		a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v1x->position_0x4C_76);
		if (WizardAiMoveTowards_14C90(a1x, v1x, 3328, 4608) && a1x->playerData_0xA4_164x->aiReactionDelay_0x1A2_418 >= 0)
		{
			v2 = WizardAiChooseAttackSpell_15790(a1x);
			if (v2 != -1)
			{
				v3 = v2;
				if (WizardCanCastSpell_15170(a1x, v2))
				{
					if (WizardAiCastSpell_14E10(a1x, v3))
					{
						//v4 = v1x->byte_0x40_64;
						if (!v1x->model_0x40_64 || v1x->model_0x40_64 == 1)
							a1x->playerData_0xA4_164x->aiTargets_0x1FC_508[4 * v1x->playerData_0xA4_164x->playerColorIndex_0x38_56 + 5] = 0;
						WizardAiSteer_16580(a1x);
						return 0;
					}
				}
			}
			a1x->playerData_0xA4_164x->speed_0xc_12 = 0;
			//v5 = v1x->byte_0x40_64;
			if (!v1x->model_0x40_64 || v1x->model_0x40_64 == 1)
			{
				//v6 = a1x->dword_0xA4_164;
				//v7 = *(x_BYTE *)(v6 + 1117);
				if (a1x->playerData_0xA4_164x->aiCombatCounter_0x45D_1117 < 3 || a1x->playerData_0xA4_164x->aiCombatCounter_0x45D_1117 >= 5)
				{
					//v8 = a1x->dword_0xA4_164;
					//v9 = *(x_BYTE *)(v8 + 1117);
					if (a1x->playerData_0xA4_164x->aiCombatCounter_0x45D_1117 < 5 || a1x->playerData_0xA4_164x->aiCombatCounter_0x45D_1117 >= 20)
					{
						//v10 = a1x->dword_0xA4_164;
						//v11 = *(x_BYTE *)(v10 + 1117);
						if (a1x->playerData_0xA4_164x->aiCombatCounter_0x45D_1117 < 20)
						{
							if (a1x->playerData_0xA4_164x->aiCombatCounter_0x45D_1117)
							{
								if (a1x->playerData_0xA4_164x->aiCombatMode_0x45C_1116 == 1)
									a1x->roll_0x20_32 -= 512;
								else
									a1x->roll_0x20_32 += 512;
								a1x->roll_0x20_32 &= 0x7ff;
								a1x->actSpeed_0x82_130 = 3
									* a1x->minSpeed_0x84_132
									* a1x->playerData_0xA4_164x->aiReflexes_0x246_582
									/ 255;
								a1x->playerData_0xA4_164x->castLock_0xe_14 = 1;
							}
							else
							{
								a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
								if ((signed int)(a1x->rand_0x14_20 % 0xFFu) >= 127)
								{
									a1x->playerData_0xA4_164x->aiCombatMode_0x45C_1116 = 2;
									a1x->yaw_0x1C_28 += 512;
								}
								else
								{
									a1x->playerData_0xA4_164x->aiCombatMode_0x45C_1116 = 1;
									a1x->yaw_0x1C_28 -= 512;
								}
								a1x->yaw_0x1C_28 &= 0x7FF;
							}
							a1x->playerData_0xA4_164x->aiCombatCounter_0x45D_1117++;
						}
						else
						{
							a1x->playerData_0xA4_164x->aiCombatCounter_0x45D_1117 = 0;
						}
					}
					else
					{
						a1x->playerData_0xA4_164x->aiCombatCounter_0x45D_1117++;
					}
				}
				else
				{
					a1x->playerData_0xA4_164x->aiCombatCounter_0x45D_1117++;
				}
			}
			v12 = a1x->position_0x4C_76.z - (v1x->position_0x4C_76.z + 512);
			if (a1x->position_0x4C_76.z != v1x->position_0x4C_76.z + 512)
			{
				if (a1x->position_0x4C_76.z - (v1x->position_0x4C_76.z + 512) <= 0)
					v12 = -1;
				//LOWORD(v12) = -1;
				else
					v12 = 1;
				//LOWORD(v12) = 1;
			}
			a1x->position_0x4C_76.z += v12 * a1x->typeParams_0xA0_160x->climbSpeed_160_0xe_14;
		}
		WizardAiSteer_16580(a1x);
		return 1;
	}
	WizardAiSteer_16580(a1x);
	return 0;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00013B00) --------------------------------------------------------
signed int WizardAiTryBuildCastle_13B00(type_entity_0x6E8E* a1x)//1f4b00
{
	type_entity_0x6E8E* v1x; // esi
	__int16 v2; // bx
	type_entity_0x6E8E* v3x; // eax
	//uint8_t* v4; // edi
	//int v5; // esi
	type_entity_0x6E8E* v7x; // eax
	//uint8_t* v8; // edi
	//int v9; // esi
	int v11; // [esp+4h] [ebp-Ch]
	__int16 i; // [esp+8h] [ebp-8h]
	int v13; // [esp+Ch] [ebp-4h]

	v1x = Entities_EA3E4[0];
	if (!a1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58 && GetPlayerSpellEntity_146C0(a1x, 2u) && HasEnoughManaForSpell_15730(a1x, 2u))
	{
		v1x->id_0x1A_26 = a1x->id_0x1A_26;
		//v13 = (a1x->array_0x4C_76.x	- (__CFSHL__(a1x->array_0x4C_76.x >> 31, 14) + (a1x->array_0x4C_76.x >> 31 << 14))) >> 14;
		//v13 = (a1x->array_0x4C_76.x-(my_sign16(a1x->array_0x4C_76.x) * -16383)) >> 14;
		v13 = (int16_t)(((int16_t)a1x->position_0x4C_76.x) - (my_sign16(a1x->position_0x4C_76.x) << 14) - my_sign16(a1x->position_0x4C_76.x)) >> 14;
		v2 = 0;
		//v11 = (a1x->array_0x4C_76.y - (__CFSHL__(a1x->array_0x4C_76.y >> 31, 14) + (a1x->array_0x4C_76.y >> 31 << 14))) >> 14;
		v11 = (int16_t)(((int16_t)a1x->position_0x4C_76.y) - (my_sign16(a1x->position_0x4C_76.y) << 14) - my_sign16(a1x->position_0x4C_76.y)) >> 14;
		while (v2 < 4)
		{
			for (i = 0; i < 4; i++)
			{
				v1x->position_0x4C_76.x = (((x_BYTE)v13 + (x_BYTE)i) & 3) << 14;
				v1x->position_0x4C_76.y = (((x_BYTE)v2 + (x_BYTE)v11) & 3) << 14;
				v3x = FindWizardTargetBySpell_14B10(v1x, 2u);
				if (!v3x || (unsigned int)GetChebyshevDistXY_583B0(&v3x->position_0x4C_76, &v1x->position_0x4C_76) > 0x3000)
				{
					a1x->axis_0x9A_154x = v1x->position_0x4C_76;
					return 1;
				}
				v7x = FindWizardTargetBySpell_14B10(v1x, 2u);
				if (!v7x || (unsigned int)GetChebyshevDistXY_583B0(&v7x->position_0x4C_76, &v1x->position_0x4C_76) > 0x3000)
				{
					a1x->axis_0x9A_154x = v1x->position_0x4C_76;
					return 1;
				}
			}
			v2++;
		}
	}
	return 0;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00013C50) --------------------------------------------------------
signed int WizardAiTryGrowCastle_13C50(type_entity_0x6E8E* a1x)//1f4c50
{
	type_entity_0x6E8E* v1x; // ebx

	v1x = Entities_EA3E4[a1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58];
	if (v1x <= Entities_EA3E4[0] || v1x->actionIndex_0x45_69 != 4 || v1x->maxStage_0x30_48 || !WizardAiCanGrowCastle_155E0(a1x))
		return 0;
	a1x->targetId_0x96_150 = v1x - world_D41A0.entities_0x6E8E;
	a1x->targetKey_0x98_152 = GetEntityKey_14C40(v1x);
	return 1;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00013CE0) --------------------------------------------------------
type_entity_0x6E8E* WizardAiFindTarget_13CE0(type_entity_0x6E8E* a1x)//1f4ce0
{
	type_entity_0x6E8E* resultx; // eax
	type_entity_0x6E8E* v2x; // eax
	type_entity_0x6E8E* v3x; // eax
	type_entity_0x6E8E* v4x; // eax

	resultx = GetPlayerSpellEntity_146C0(a1x, 1u);
	if (resultx)
	{
		v2x = GetPlayerSpellEntity_146C0(a1x, 2u);
		if (v2x)
		{
			if (a1x->maxMana_0x8C_140 <= v2x->maxMana_0x8C_140)
			{
				v3x = FindNearestOwnBalloon_148E0(a1x);
				if (v3x)
				{
					a1x->targetId_0x96_150 = v3x - world_D41A0.entities_0x6E8E;
					a1x->targetKey_0x98_152 = GetEntityKey_14C40(v3x);
					return (type_entity_0x6E8E*)1;
				}
			}
		}
		else
		{
			v4x = FindNearestOwnBalloon_148E0(a1x);
			if (v4x)
			{
				a1x->targetId_0x96_150 = v4x - world_D41A0.entities_0x6E8E;
				a1x->targetKey_0x98_152 = GetEntityKey_14C40(v4x);
				return (type_entity_0x6E8E*)1;
			}
		}
		resultx = 0;
	}
	return resultx;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (00013DC0) --------------------------------------------------------
signed int WizardAiShouldRetreat_13DC0(type_entity_0x6E8E* a1x)//1f4dc0
{
	type_entity_0x6E8E* v1x; // ecx
	type_entity_0x6E8E* v2x; // ST00_4

	if (a1x->maxLife_0x4 / 2 <= a1x->life_0x8)
		return 0;
	v1x = Entities_EA3E4[a1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58];
	if (v1x <= Entities_EA3E4[0])
		return 0;
	v2x = Entities_EA3E4[a1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58];
	a1x->targetId_0x96_150 = v1x - world_D41A0.entities_0x6E8E;
	a1x->targetKey_0x98_152 = GetEntityKey_14C40(v2x);
	return 1;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00013E40) --------------------------------------------------------
signed int WizardAiChooseEnemyWizard_13E40(type_entity_0x6E8E* a1x)//1f4e40
{
	type_entity_0x6E8E* v1x; // edi
	type_entity_0x6E8E* ix; // ebx
	type_entity_0x6E8E* v3x; // ecx
	unsigned int v4; // eax
	int v5; // ebx
	signed int result; // ebx
	unsigned int v8; // [esp+8h] [ebp-8h]

	v8 = -1;
	if (!HasAreaSpell_164B0(a1x) || !a1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58 && GetPlayerSpellEntity_146C0(a1x, 2u))
		return 0;
	v1x = 0;
	for (ix = gameState_D41A4.class3List_38519; ix > Entities_EA3E4[0]; ix = ix->next_0)
	{
		if (ix->id_0x1A_26 != a1x->id_0x1A_26 && ix->model_0x40_64 == 2)
		{
			if ((v3x = Entities_EA3E4[ix->id_0x1A_26],
				50000 - a1x->playerData_0xA4_164x->aiAggression_0x242_578 * (v3x->maxMana_0x8C_140 / 10) / 255 < a1x->playerData_0xA4_164x->aiTargets_0x1FC_508[4 * v3x->playerData_0xA4_164x->playerColorIndex_0x38_56 + 4])
				&& (unsigned int)Maths::EuclideanDistXY_584D0(&v3x->position_0x4C_76, &ix->position_0x4C_76) > 0x3840000
				&& !EntitiesOverlap_106C0(Entities_EA3E4[ix->id_0x1A_26], ix)
				|| Entities_EA3E4[a1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58]->mana_0x90_144 > 640
				* (255
					- a1x->playerData_0xA4_164x->aiAggression_0x242_578)
				+ ix->mana_0x90_144)
			{
				v4 = Maths::EuclideanDistXY_584D0(&a1x->position_0x4C_76, &ix->position_0x4C_76);
				if (v4 < v8)
				{
					v1x = ix;
					v8 = v4;
				}
			}
		}
	}
	if (!v1x)
		return 0;
	v5 = a1x->typeParams_0xA0_160x->range_160_0x1c_28;
	if (Maths::EuclideanDistXY_584D0(&v1x->position_0x4C_76, &a1x->position_0x4C_76) >= (unsigned int)(v5 * v5))
		return 0;
	result = 1;
	a1x->targetId_0x96_150 = v1x - world_D41A0.entities_0x6E8E;
	a1x->targetKey_0x98_152 = GetEntityKey_14C40(v1x);
	return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00014030) --------------------------------------------------------
signed int WizardAiChooseEnemyCreature_14030(type_entity_0x6E8E* a1x)//1f5030
{
	type_entity_0x6E8E* v1x; // edi
	type_entity_0x6E8E* ix; // ebx
	char v3; // ah
	//int v4; // ecx
	//int v5; // eax
	unsigned int v7; // eax
	int v8; // ebx
	unsigned int v9; // [esp+0h] [ebp-14h]

	v9 = -1;
	if (!HasAttackSpell_15E60(a1x) || !a1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58 && GetPlayerSpellEntity_146C0(a1x, 2u))
		return 0;
	v1x = 0;
	for (ix = gameState_D41A4.class3List_38519; ix > Entities_EA3E4[0]; ix = ix->next_0)
	{
		if (ix->id_0x1A_26 != a1x->id_0x1A_26)
		{
			v3 = ix->model_0x40_64;
			if ((!v3 || v3 == 1) && !GetActivePlayerSpellEntity_15760(ix, 0xBu))
			{
				//v4 = ix->dword_0xA4_164;
				//v5 = a1x->dword_0xA4_164 + 8 * ix->playerData_0xA4_164x->word_0x38_56;
				if (a1x->playerData_0xA4_164x->aiTargets_0x1FC_508[1 + 4 + 4 * ix->playerData_0xA4_164x->playerColorIndex_0x38_56] == 1)
				{
					a1x->targetId_0x96_150 = ix - world_D41A0.entities_0x6E8E;
					a1x->targetKey_0x98_152 = GetEntityKey_14C40(ix);
					return 1;
				}
				if (50000 - ix->maxMana_0x8C_140 / 10 * a1x->playerData_0xA4_164x->aiAggression_0x242_578 / 255 <= a1x->playerData_0xA4_164x->aiTargets_0x1FC_508[4 * ix->playerData_0xA4_164x->playerColorIndex_0x38_56 + 4]
					|| !ix->playerData_0xA4_164x->CastleEntityIndex_0x3A_58
					&& GetPlayerSpellEntity_146C0(ix, 2u)
					&& ix->mana_0x90_144 + 32 * (255 - a1x->playerData_0xA4_164x->aiAggression_0x242_578) < a1x->mana_0x90_144)
				{
					v7 = Maths::EuclideanDistXY_584D0(&a1x->position_0x4C_76, &ix->position_0x4C_76);
					if (v7 < v9)
					{
						v1x = ix;
						v9 = v7;
					}
				}
			}
		}
	}
	if (!v1x)
		return 0;
	v8 = a1x->typeParams_0xA0_160x->range_160_0x1c_28 + 10;
	if (Maths::EuclideanDistXY_584D0(&v1x->position_0x4C_76, &a1x->position_0x4C_76) >= (unsigned int)(v8 * v8))
		return 0;
	a1x->targetId_0x96_150 = v1x - world_D41A0.entities_0x6E8E;
	a1x->targetKey_0x98_152 = GetEntityKey_14C40(v1x);
	return 1;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00014250) --------------------------------------------------------
signed int WizardAiChooseEnemyCastle_14250(type_entity_0x6E8E* a1x)//1f5250
{
	type_entity_0x6E8E* v1x; // esi
	type_entity_0x6E8E* ix; // ebx
	type_entity_0x6E8E* v3x; // edx
	unsigned int v4; // eax
	int v5; // ebx
	int v7; // [esp+Ch] [ebp-Ch]
	//int v8; // [esp+10h] [ebp-8h]
	unsigned int v9; // [esp+14h] [ebp-4h]

	v9 = -1;
	if (!HasAttackSpell_15E60(a1x))
		return 0;
	v1x = 0;
	for (ix = gameState_D41A4.class3List_38519; ix > Entities_EA3E4[0]; ix = ix->next_0)
	{
		if (ix->id_0x1A_26 != a1x->id_0x1A_26 && ix->model_0x40_64 == 3)
		{
			v3x = Entities_EA3E4[ix->id_0x1A_26];
			//v8 = v3x->dword_0xA4_164;
			v7 = a1x->playerData_0xA4_164x->aiAggression_0x242_578;
			if (50000 - v3x->maxMana_0x8C_140 / 10 * v7 / 255 < a1x->playerData_0xA4_164x->aiTargets_0x1FC_508[4 * v3x->playerData_0xA4_164x->playerColorIndex_0x38_56 + 4]
				&& 10 * (275 - v7) < ix->mana_0x90_144
				&& !EntitiesOverlap_106C0(ix, Entities_EA3E4[v3x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58]))
			{
				v4 = Maths::EuclideanDistXY_584D0(&a1x->position_0x4C_76, &ix->position_0x4C_76);
				if (v4 < v9)
				{
					v1x = ix;
					v9 = v4;
				}
			}
		}
	}
	if (!v1x)
		return 0;
	v5 = a1x->typeParams_0xA0_160x->range_160_0x1c_28;
	if (Maths::EuclideanDistXY_584D0(&v1x->position_0x4C_76, &a1x->position_0x4C_76) >= (unsigned int)(v5 * v5))
		return 0;
	a1x->targetId_0x96_150 = v1x - world_D41A0.entities_0x6E8E;
	a1x->targetKey_0x98_152 = GetEntityKey_14C40(v1x);
	return 1;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00014530) --------------------------------------------------------
signed int WizardAiChooseAttackTarget_14530(type_entity_0x6E8E* a1x)//1f5530
{
	type_entity_0x6E8E* v1x; // esi
	signed int i; // edi
	type_entity_0x6E8E* jx; // ebx
	unsigned int v4; // eax
	unsigned int v6; // [esp+0h] [ebp-8h]
	type_entity_0x6E8E* v7x; // [esp+4h] [ebp-4h]

	v1x = 0;
	v6 = -1;
	if (!HasAttackSpell_15E60(a1x))
		return 0;
	v7x = Entities_EA3E4[a1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58];
	if (v7x == Entities_EA3E4[0])
		v7x = a1x;
	for (i = 0; i < 29; i++)
	{
		for (jx = gameState_D41A4.creatureLists_38403x[i]; jx > Entities_EA3E4[0]; jx = jx->next_0)
		{
			if (jx->id_0x1A_26 != a1x->id_0x1A_26 && jx->mana_0x90_144 > 0)
			{
				v4 = Maths::EuclideanDistXY_584D0(&v7x->position_0x4C_76, &jx->position_0x4C_76);
				if (v4 < v6)
				{
					v1x = jx;
					v6 = v4;
				}
			}
		}
	}
	if (!v1x)
		return 0;
	a1x->targetId_0x96_150 = v1x - world_D41A0.entities_0x6E8E;
	a1x->targetKey_0x98_152 = GetEntityKey_14C40(v1x);
	return 1;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00014630) --------------------------------------------------------
signed int WizardAiCheckHealth_14630(type_entity_0x6E8E* a1x)//1f5630
{
	type_entity_0x6E8E* v1x; // ecx

	if (a1x->life_0x8 >= a1x->maxLife_0x4
		|| (v1x = Entities_EA3E4[a1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58], v1x <= Entities_EA3E4[0]))
	{
		a1x->playerData_0xA4_164x->aiState_0x1C1_449 = 12;
	}
	else
	{
		a1x->targetId_0x96_150 = v1x - world_D41A0.entities_0x6E8E;
		a1x->targetKey_0x98_152 = GetEntityKey_14C40(v1x);
		a1x->playerData_0xA4_164x->aiState_0x1C1_449 = 11;
	}
	return 1;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000146C0) --------------------------------------------------------
type_entity_0x6E8E* GetPlayerSpellEntity_146C0(type_entity_0x6E8E* a1x, unsigned __int8 a2)//1f56c0
{
	type_entity_0x6E8E* resultx; // eax

	resultx = Entities_EA3E4[a1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[a2]];
	if (resultx <= Entities_EA3E4[0])
		resultx = 0;
	return resultx;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000146F0) --------------------------------------------------------
void UpdateWizardMovement_146F0(type_entity_0x6E8E* a1x)//1f56f0
{
	//char v1; // ah
	//signed __int16 *v2; // edx
	__int16 v3; // ST10_2
	int v4; // ST0C_4
	int v5; // ST08_4
	__int16 v6; // ax
	unsigned __int16 v7; // ax
	//int v8; // edx
	//__int16 v9; // cx
	//x_WORD *v10; // eax
	signed __int16 v11; // dx
	signed __int16 v12; // ax
	//__int16 v13; // ax
	unsigned __int16 v14; // si
	int32_t v15; // rax
	int v16; // eax
	//int v17; // edx
	__int16 v18; // si
	__int16 v19; // si
	unsigned int v20; // eax
	unsigned __int16 v21; // si
	unsigned __int16 v22; // dx

	//v1 = a1x->flags_0xc_12_15.byte[1];
	if (a1x->flags_0xc_12_15.byte[1] & 8)
	{
		a1x->flags_0xc_12_15.byte[1] &= 0xF7;
	}
	else
	{
		predictedAxis_EB398ar = a1x->position_0x4C_76;
		//v2 = *(signed __int16 **)a1x->dword_0xA0_160;
		v3 = a1x->typeParams_0xA0_160x->climbSpeed_160_0xe_14;
		v4 = a1x->typeParams_0xA0_160x->maxHeight_160_0xa_10;
		v5 = a1x->typeParams_0xA0_160x->minHeight_160_0xc_12;
		v6 = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
		ClampAboveTerrain_580E0(&predictedAxis_EB398ar, v6, v5, v4, v3);
		MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, 0, a1x->actSpeed_0x82_130);
		v7 = a1x->yaw_0x1C_28;
		//HIBYTE(v7) += 2;
		v7 += 512;
		MoveEntity_57FA0(&predictedAxis_EB398ar, v7, 0, a1x->playerData_0xA4_164x->strafeSpeed_0x10_16);
		//v8 = a1x->dword_0xA4_164;
		//v9 = *(x_WORD *)(v8 + 16);
		//v10 = (x_WORD *)(v8 + 16);
		if (a1x->playerData_0xA4_164x->strafeSpeed_0x10_16)
		{
			if (a1x->playerData_0xA4_164x->strafeSpeed_0x10_16 <= 0)
				v11 = -1;
			else
				v11 = 1;
		}
		else
		{
			v11 = 0;
		}
		a1x->playerData_0xA4_164x->strafeSpeed_0x10_16 -= 4 * v11;
		CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
		v12 = a1x->playerData_0xA4_164x->speed_0xc_12 - a1x->actSpeed_0x82_130;
		if (a1x->playerData_0xA4_164x->speed_0xc_12 != a1x->actSpeed_0x82_130)
		{
			if (a1x->playerData_0xA4_164x->speed_0xc_12 - a1x->actSpeed_0x82_130 <= 0)
				v12 = -1;
			else
				v12 = 1;
		}
		a1x->actSpeed_0x82_130 += 16 * v12;
		//v13 = a1x->roll_0x20_32;
		//HIBYTE(v13) &= 7ffu;
		v14 = AngleDifference_582B0(a1x->yaw_0x1C_28, a1x->roll_0x20_32 & 0x7ffu);
		v15 = 255 - a1x->playerData_0xA4_164x->aiReflexes_0x246_582;
		//v16 = v14	/ (signed int)(unsigned __int16)(((signed int)(v15 - (__CFSHL__(HIDWORD(v15), 4) + 16 * HIDWORD(v15))) >> 4) + 8);
		v16 = v14 / (((v15 - (my_sign32(v15) << 4) + my_sign32(v15)) >> 4) + 8);
		//v17 = a1x->dword_0xA0_160;
		v18 = v16;
		if ((signed __int16)v16 <= a1x->typeParams_0xA0_160x->subtype_160_0x2_2)
		{
			if ((signed __int16)v16 < a1x->typeParams_0xA0_160x->yawTurnLimit_160_0x4_4)
				v18 = a1x->typeParams_0xA0_160x->yawTurnLimit_160_0x4_4;
		}
		else
		{
			v18 = a1x->typeParams_0xA0_160x->subtype_160_0x2_2;
		}
		v19 = AngleTurnDirection_582F0(a1x->yaw_0x1C_28, a1x->roll_0x20_32) * v18;
		v20 = a1x->yaw_0x1C_28;
		v21 = (v20 + v19) & 0x7FF;
		v22 = a1x->roll_0x20_32;
		a1x->yaw_0x1C_28 = v21;
		if ((unsigned __int16)v20 < (unsigned int)v22 && v21 > v22
			|| (v22 = a1x->roll_0x20_32, v20 > v22) && v22 > a1x->yaw_0x1C_28)
		{
			a1x->yaw_0x1C_28 = v22;
		}
	}
	//return 1;
}
// EB398: using guessed type __int16 x_WORD_EB398;

//----- (000148E0) --------------------------------------------------------
type_entity_0x6E8E* FindNearestOwnBalloon_148E0(type_entity_0x6E8E* a1x)//1f58e0
{
	unsigned int v1; // edi
	type_entity_0x6E8E* v2x; // ebx
	int v3; // ST08_4
	int v4; // eax
	type_entity_0x6E8E* v5x; // edx
	char v6; // cl
	unsigned int v7; // eax
	type_entity_0x6E8E* v8x; // eax
	type_entity_0x6E8E* ix; // ebx
	unsigned int v10; // eax
	type_entity_0x6E8E* v12x; // [esp+8h] [ebp-14h]
	axis_3d* v13; // [esp+Ch] [ebp-10h]
	type_entity_0x6E8E* v14x; // [esp+10h] [ebp-Ch]
	type_entity_0x6E8E* v15x; // [esp+14h] [ebp-8h]
	type_entity_0x6E8E* resultx; // [esp+18h] [ebp-4h]

	v1 = -1;
	resultx = 0;
	v2x = gameState_D41A4.manaSphereList_38523;
	v15x = Entities_EA3E4[a1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58];
	while (v2x > Entities_EA3E4[0])
	{
		if (a1x == Entities_EA3E4[v2x->playerEntityIndex_0x94_148])
			goto LABEL_22;
		if (v2x->model_0x40_64 == 57)
		{
			v3 = a1x->playerData_0xA4_164x->aiPerception_0x244_580;
			if (watcomrand() % 255 < v3)
				break;
		}
		v4 = v2x->playerEntityIndex_0x94_148;
		v5x = Entities_EA3E4[(unsigned __int16)v4];
		v13 = &v2x->position_0x4C_76;
		v6 = v5x->class_0x3F_63;
		if (v6 == 3)
		{
			if (v4 == a1x->id_0x1A_26)
				goto LABEL_22;
			if (50000 - v5x->maxMana_0x8C_140 / 10 * a1x->playerData_0xA4_164x->aiAggression_0x242_578 / 255 >= a1x->playerData_0xA4_164x->aiTargets_0x1FC_508[4 * v5x->playerData_0xA4_164x->playerColorIndex_0x38_56 + 4])
			{
				v12x = FindNearestOtherEntity_16FC0(v2x, a1x);
				if (!v12x)
					goto LABEL_22;
				v8x = (v15x <= Entities_EA3E4[0] ? FindWizardTargetBySpell_14B10(v2x, 2u) : FindNearestOtherWizard_14BD0(v2x, v15x));//fix uint8_t*
				v14x = v8x;
				if (a1x->id_0x1A_26 != v12x->id_0x1A_26
					&& (unsigned int)Maths::EuclideanDistXY_584D0(&v2x->position_0x4C_76, &v12x->position_0x4C_76) <= 0x1900000)
				{
					goto LABEL_22;
				}
				if (v14x && EntitiesOverlap_106C0(v2x, v14x))
					goto LABEL_22;
				v7 = Maths::EuclideanDistXY_584D0(&a1x->position_0x4C_76, &v2x->position_0x4C_76);
			}
			else
			{
				v7 = Maths::EuclideanDistXY_584D0(&v15x->position_0x4C_76, v13);
			}
		}
		else
		{
			if (v6 == 3)
				goto LABEL_22;
			v7 = Maths::EuclideanDistXY_584D0(&a1x->position_0x4C_76, v13);
		}
		if (v7 < v1)
		{
			resultx = v2x;
			v1 = v7;
		}
	LABEL_22:
		v2x = v2x->next_0;
	}
	if (!resultx)
	{
		for (ix = gameState_D41A4.creatureLists_38403x[88 / 4]; ix > Entities_EA3E4[0]; ix = ix->next_0)
		{
			if (ix->model_0x40_64 != 57 && ix->playerEntityIndex_0x94_148 != a1x->id_0x1A_26)
			{
				v10 = Maths::EuclideanDistXY_584D0(&a1x->position_0x4C_76, &ix->position_0x4C_76);
				if (v10 < v1)
				{
					resultx = ix;
					v1 = v10;
				}
			}
		}
	}
	return resultx;
}
// 8C278: using guessed type int rand(void);
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00014B10) --------------------------------------------------------
type_entity_0x6E8E* FindWizardTargetBySpell_14B10(type_entity_0x6E8E* a1x, unsigned __int8 a2)//1f5b10
{
	unsigned int v2; // edi
	type_entity_0x6E8E* resultx; // esi
	type_entity_0x6E8E* ix; // ebx
	unsigned int v5; // eax
	type_entity_0x6E8E* jx; // ebx
	unsigned int v7; // eax

	v2 = -1;
	resultx = 0;
	if (a2 < 2u)
	{
		if (a2)
			return resultx;
	}
	else if (a2 > 3u)
	{
		if (a2 == 0xff)
		{
			for (ix = gameState_D41A4.class3List_38519; ix > Entities_EA3E4[0]; ix = ix->next_0)
			{
				if (ix->id_0x1A_26 != a1x->id_0x1A_26)
				{
					v5 = Maths::EuclideanDistXY_584D0(&a1x->position_0x4C_76, &ix->position_0x4C_76);
					if (v5 < v2)
					{
						resultx = ix;
						v2 = v5;
					}
				}
			}
		}
		return resultx;
	}
	for (jx = gameState_D41A4.class3List_38519; jx > Entities_EA3E4[0]; jx = jx->next_0)
	{
		if (jx->id_0x1A_26 != a1x->id_0x1A_26 && jx->model_0x40_64 == a2)
		{
			v7 = Maths::EuclideanDistXY_584D0(&a1x->position_0x4C_76, &jx->position_0x4C_76);
			if (v7 < v2)
			{
				resultx = jx;
				v2 = v7;
			}
		}
	}
	return resultx;
}
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00014BD0) --------------------------------------------------------
type_entity_0x6E8E* FindNearestOtherWizard_14BD0(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//1f5bd0
{
	unsigned int v2; // edi
	type_entity_0x6E8E* resultx; // esi
	type_entity_0x6E8E* v4x; // ebx
	__int16 v6; // ax
	unsigned int v7; // eax

	v2 = -1;
	resultx = 0;
	v4x = gameState_D41A4.class3List_38519;
	if (v4x <= Entities_EA3E4[0])
		return 0;
	do
	{
		v6 = v4x->id_0x1A_26;
		if (v6 != a1x->id_0x1A_26 && v6 != a2x->id_0x1A_26 && v4x->model_0x40_64 == 2)
		{
			v7 = Maths::EuclideanDistXY_584D0(&a1x->position_0x4C_76, &v4x->position_0x4C_76);
			if (v7 < v2)
			{
				resultx = v4x;
				v2 = v7;
			}
		}
		v4x = v4x->next_0;
	} while (v4x > Entities_EA3E4[0]);
	return resultx;
}
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00014C40) --------------------------------------------------------
__int16 GetEntityKey_14C40(type_entity_0x6E8E* a1)//1f5c40
{
	return a1->id_0x1A_26 + a1->model_0x40_64 + (a1->class_0x3F_63 << 7);
}

//----- (00014C60) --------------------------------------------------------
bool IsTargetStillValid_14C60(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//1f5c60
{
	return GetEntityKey_14C40(a2x) == a1x->targetKey_0x98_152;
}

//----- (00014C90) --------------------------------------------------------
signed int WizardAiMoveTowards_14C90(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x, signed int a3, signed int a4)//1f5c90
{
	signed int v4; // eax
	signed int v6; // eax

	a1x->playerData_0xA4_164x->castLock_0xe_14 = 0;
	if (!a2x)
	{
		v4 = Maths::EuclideanDistXYZ_58490(&a1x->position_0x4C_76, &a1x->axis_0x9A_154x);
		if (v4 > a3)
		{
			if (v4 > a4 && WizardCanCastSpell_15170(a1x, 3u))
			{
				if (!IsPlayerSpellActive_156F0(a1x, 3u))
					WizardAiCastSpell_14E10(a1x, 3u);
			}
			else
			{
				a1x->playerData_0xA4_164x->speed_0xc_12 = a1x->minSpeed_0x84_132;
				a1x->playerData_0xA4_164x->castLock_0xe_14 = 1;
			}
			return 0;
		}
	LABEL_16:
		a1x->playerData_0xA4_164x->speed_0xc_12 = 0;
		a1x->playerData_0xA4_164x->castLock_0xe_14 = 1;
		return 1;
	}
	v6 = Maths::EuclideanDistXYZ_58490(&a1x->position_0x4C_76, &a2x->position_0x4C_76);
	if (v6 <= a3)
		goto LABEL_16;
	if (v6 > a4 && WizardCanCastSpell_15170(a1x, 3u))
	{
		if (!IsPlayerSpellActive_156F0(a1x, 3u))
			WizardAiCastSpell_14E10(a1x, 3u);
	}
	else
	{
		a1x->playerData_0xA4_164x->speed_0xc_12 = a1x->minSpeed_0x84_132;
		a1x->playerData_0xA4_164x->castLock_0xe_14 = 1;
	}
	return 0;
}

//int debugcounter_1f5e10 = 0;
//----- (00014E10) --------------------------------------------------------
signed int WizardAiCastSpell_14E10(type_entity_0x6E8E* a1x, unsigned __int8 a2)//1f5e10
{
	type_entity_0x6E8E* v2x; // eax
	signed int result; // eax
	type_entity_0x6E8E* v4x; // eax
	type_entity_0x6E8E* v5x; // eax
	type_entity_0x6E8E* v6x; // edi
	//int v7; // esi
	type_entity_0x6E8E* v8x; // esi
	type_entity_0x6E8E* v9x; // eax

	if (CommandLineParams.DoDebugSequences()) {
		/*if (CommandLineParams.DoDebugafterload())
		{
			if (debugcounter_1f5e10 >= 0x34)
			{
				int comp20;
				uint8_t origbyte20 = 0;
				uint8_t remakebyte20 = 0;

				comp20 = compare_with_sequence((char*)"001f5e14-002DC4E0", (uint8_t*)x_BYTE_10B4E0_terraintype, 0x2dc4e0, debugcounter_1f5e10, 0x70000, 0x10000, &origbyte20, &remakebyte20);
				comp20 = compare_with_sequence((char*)"001f5e14-002DC4E0", (uint8_t*)x_BYTE_11B4E0_height, 0x2dc4e0, debugcounter_1f5e10, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x10000);
				comp20 = compare_with_sequence((char*)"001f5e14-002DC4E0", (uint8_t*)x_BYTE_12B4E0_shading, 0x2dc4e0, debugcounter_1f5e10, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x20000);
				comp20 = compare_with_sequence((char*)"001f5e14-002DC4E0", (uint8_t*)x_BYTE_13B4E0_angle, 0x2dc4e0, debugcounter_1f5e10, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x30000);
				comp20 = compare_with_sequence((char*)"001f5e14-002DC4E0", (uint8_t*)x_WORD_15B4E0_source, 0x2dc4e0, debugcounter_1f5e10, 0x70000, 0x20000, &origbyte20, &remakebyte20, 0x50000);
				comp20 = compare_with_sequence_D41A0((char*)"001f5e14-00356038", (uint8_t*)& D41A0_BYTESTR_0, 0x356038, debugcounter_1f5e10, 224790, &origbyte20, &remakebyte20);

				comp20 = compare_with_sequence_array_E2A74((char*)"001f5e14-002B3A74", (uint8_t*)& str_E2A74, 0x2b3a74, debugcounter_1f5e10, 0xc4e, 0xc4e, &origbyte20, &remakebyte20);
			}
			debugcounter_1f5e10++;
		}*/
	}

	if (!WizardCanCastSpell_15170(a1x, a2))
		return 0;
	a1x->flags_0xc_12_15.byte[1] &= 0xFEu;
	if (a2 > 0x16u)
		return 0;
	switch (a2)
	{
	case 0u:
	case 1u:
	case 7u:
	case 0x16u:
		v6x = GetPlayerSpellEntity_146C0(a1x, a2);
		if (!v6x
			|| a1x->playerData_0xA4_164x->aiReactionDelay_0x1A2_418 < 0
			|| (unsigned __int16)AngleDifference_582B0(a1x->yaw_0x1C_28, a1x->roll_0x20_32) >= 0xAAu)
		{
			return 0;
		}
		a1x->pitch_0x1E_30 = Maths::GetPitchAngle_58210(&a1x->position_0x4C_76, &Entities_EA3E4[a1x->targetId_0x96_150]->position_0x4C_76);
		a1x->playerData_0xA4_164x->aiReactionDelay_0x1A2_418++;
		//v7 = a1x->dword_0xA4_164;
		if (a1x->playerData_0xA4_164x->aiReactionDelay_0x1A2_418 >= 8)
			//a1x->playerData_0xA4_164x->word_0x1A2_418 = ((a1x->playerData_0xA4_164x->word_0x246_582- 255- (__CFSHL__((a1x->playerData_0xA4_164x->word_0x246_582 - 255) >> 31, 3)+ 8 * ((a1x->playerData_0xA4_164x->word_0x246_582 - 255) >> 31))) >> 3)- 1;
			a1x->playerData_0xA4_164x->aiReactionDelay_0x1A2_418 = ((a1x->playerData_0xA4_164x->aiReflexes_0x246_582 - 255 - (my_sign32(a1x->playerData_0xA4_164x->aiReflexes_0x246_582 - 255) * 8) + (my_sign32(a1x->playerData_0xA4_164x->aiReflexes_0x246_582 - 255))) >> 3) - 1;
		if (CastleAttackEntity_5F660(a1x, v6x, 0) != 1)
			return 0;
		a1x->playerData_0xA4_164x->str_611.spellCooldowns_0x367_871x.SpellEnabled[a2] = spellCooldownTable_D3F4C[a2];
		return 1;
	case 2u:
		v2x = GetPlayerSpellEntity_146C0(a1x, a2);
		if (!v2x)
			return 0;
		if (a1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58)
		{
			if (CastleAttackEntity_5F660(a1x, v2x, 0) != 1)
				return 0;
			a1x->playerData_0xA4_164x->str_611.spellCooldowns_0x367_871x.SpellEnabled[a2] = spellCooldownTable_D3F4C[a2];
			result = 1;
		}
		else
		{
			v4x = CreateEntity_4A190(&a1x->axis_0x9A_154x, 3, 2);
			if (v4x)
			{
				v4x->id_0x1A_26 = a1x->id_0x1A_26;
				a1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58 = v4x - world_D41A0.entities_0x6E8E;
			}
			result = 1;
		}
		break;
	case 3u:
		v5x = GetPlayerSpellEntity_146C0(a1x, a2);
		if (!v5x || CastleAttackEntity_5F660(a1x, v5x, 0) != 1)
			return 0;
		a1x->playerData_0xA4_164x->str_611.spellCooldowns_0x367_871x.SpellEnabled[a2] = spellCooldownTable_D3F4C[a2];
		return 1;
	case 4u:
	case 9u:
	case 0xDu:
	case 0xEu:
	case 0x12u:
	case 0x13u:
	case 0x15u:
		v8x = GetPlayerSpellEntity_146C0(a1x, a2);
		if (!v8x)
			return 0;
		if ((unsigned __int16)AngleDifference_582B0(a1x->yaw_0x1C_28, a1x->roll_0x20_32) >= 0xE3u)
			return 0;
		a1x->pitch_0x1E_30 = Maths::GetPitchAngle_58210(&a1x->position_0x4C_76, &Entities_EA3E4[a1x->targetId_0x96_150]->position_0x4C_76);
		if (CastleAttackEntity_5F660(a1x, v8x, 0) != 1)
			return 0;
		a1x->playerData_0xA4_164x->str_611.spellCooldowns_0x367_871x.SpellEnabled[a2] = spellCooldownTable_D3F4C[a2];
		return 1;
	case 5u:
	case 6u:
	case 8u:
	case 0xAu:
	case 0xBu:
	case 0xCu:
	case 0x10u:
	case 0x11u:
	case 0x14u:
		v9x = GetPlayerSpellEntity_146C0(a1x, a2);
		if (!v9x || CastleAttackEntity_5F660(a1x, v9x, 0) != 1)
			return 0;
		a1x->playerData_0xA4_164x->str_611.spellCooldowns_0x367_871x.SpellEnabled[a2] = spellCooldownTable_D3F4C[a2];
		return 1;
	case 0xFu:
		return 0;
	}
	return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00015170) --------------------------------------------------------
type_entity_0x6E8E* WizardCanCastSpell_15170(type_entity_0x6E8E* a1x, unsigned __int8 a2)//1f6170
{
	type_entity_0x6E8E* v2x; // eax
	type_entity_0x6E8E* v3x; // esi
	//int v4; // eax
	//int v5; // edx
	type_entity_0x6E8E* v7x; // eax
	type_entity_0x6E8E* v8x; // esi
	//int v9; // eax
	type_entity_0x6E8E* v10x; // eax
	type_entity_0x6E8E* v11x; // esi
	//int v12; // eax
	//int v13; // eax
	int v14; // edi
	type_entity_0x6E8E* v15x; // eax
	type_entity_0x6E8E* v16x; // esi
	//int v17; // eax
	//int v18; // eax
	//int v19; // edx
	__int64 v20; // rax
	type_entity_0x6E8E* v21x; // eax
	type_entity_0x6E8E* v22x; // esi
	//int v23; // eax
	//int v24; // eax
	int v25; // edi
	type_entity_0x6E8E* v26x; // eax
	type_entity_0x6E8E* v27x; // esi
	//int v28; // eax
	//int v29; // edx

	if (a2 > 0x16u)
	{
	LABEL_43:
		if (a2 < 0x1Au)
		{
			v26x = GetPlayerSpellEntity_146C0(a1x, a2);
			v27x = v26x;
			if (v26x)
			{
				//v28 = 80 * a2 + 26 * v26x->byte_0x46_70;
				//if (a1x->dword_0x8C_140 >= *(int *)((char *)&*(int32_t*)& SPELLS_BEGIN_BUFFER_DA818[10] + v28))
				if (a1x->maxMana_0x8C_140 >= SPELLS_BEGIN_BUFFER_str[a2].subspell[v26x->levelPar3_0x46_70].maxManaLimit_A)
				{
					//v29 = a1x->dword_0x90_144;
					//if (v29 >= *(int *)((char *)&*(int32_t*)& SPELLS_BEGIN_BUFFER_DA818[6] + v28)
					if (a1x->mana_0x90_144 >= SPELLS_BEGIN_BUFFER_str[a2].subspell[v26x->levelPar3_0x46_70].manaCost_6
						&& !a1x->playerData_0xA4_164x->str_611.spellCooldowns_0x367_871x.SpellEnabled[a2]
						&& a1x->mana_0x90_144 >= v27x->maxMana_0x8C_140)
					{
						return v27x;
					}
				}
			}
		}
	}
	else
	{
		switch (a2)
		{
		case 0u:
		case 7u:
		case 0xDu:
		case 0xEu:
		case 0x16u:
			v21x = GetPlayerSpellEntity_146C0(a1x, a2);
			v22x = v21x;
			if (!v21x)
				return 0;
			//v23 = 80 * a2 + 26 * v21x->byte_0x46_70;
			//if (a1x->dword_0x8C_140 < *(int *)((char *)&*(int32_t*)& SPELLS_BEGIN_BUFFER_DA818[10] + v23))
			if (a1x->maxMana_0x8C_140 < SPELLS_BEGIN_BUFFER_str[a2].subspell[v21x->levelPar3_0x46_70].maxManaLimit_A)
				return 0;
			//if (a1x->dword_0x90_144 < *(int *)((char *)&*(int32_t*)& SPELLS_BEGIN_BUFFER_DA818[6] + v23))
			if (a1x->mana_0x90_144 < SPELLS_BEGIN_BUFFER_str[a2].subspell[v21x->levelPar3_0x46_70].manaCost_6)
				return 0;
			//v24 = a1x->dword_0xA4_164;
			if (a1x->playerData_0xA4_164x->str_611.spellCooldowns_0x367_871x.SpellEnabled[a2])
				return 0;
			if (a1x->mana_0x90_144 < v22x->maxMana_0x8C_140)
				return 0;
			/*v25 = ((((255
				- a1x->playerData_0xA4_164x->word_0x244_580
				- (__CFSHL__((255 - a1x->playerData_0xA4_164x->word_0x244_580) >> 31, 2)
					+ 4 * ((255 - a1x->playerData_0xA4_164x->word_0x244_580) >> 31))) >> 2)
				+ 20) << 11)
				/ 360;*/
			v25 = (((((255 - a1x->playerData_0xA4_164x->aiPerception_0x244_580) - (my_sign32(255 - a1x->playerData_0xA4_164x->aiPerception_0x244_580) << 2) + my_sign32(255 - a1x->playerData_0xA4_164x->aiPerception_0x244_580)) >> 2) + 20) << 11) / 360;

			if ((unsigned __int16)AngleDifference_582B0(a1x->yaw_0x1C_28, a1x->roll_0x20_32) >= v25)
				return 0;
			return v22x;
		case 1u:
		case 9u:
		case 0x10u:
		case 0x12u:
		case 0x13u:
		case 0x15u:
			v10x = GetPlayerSpellEntity_146C0(a1x, a2);
			v11x = v10x;
			if (!v10x)
				return 0;
			//v12 = 26 * v10x->byte_0x46_70 + 80 * a2;
			//if (a1x->dword_0x8C_140 < *(int *)((char *)&*(int32_t*)& SPELLS_BEGIN_BUFFER_DA818[10] + v12))
			if (a1x->maxMana_0x8C_140 < SPELLS_BEGIN_BUFFER_str[a2].subspell[v10x->levelPar3_0x46_70].maxManaLimit_A)
				return 0;
			//if (a1x->dword_0x90_144 < *(int *)((char *)&*(int32_t*)& SPELLS_BEGIN_BUFFER_DA818[6] + v12))
			if (a1x->mana_0x90_144 < SPELLS_BEGIN_BUFFER_str[a2].subspell[v10x->levelPar3_0x46_70].manaCost_6)
				return 0;
			if (v11x->stage_0x2E_46)
				return 0;
			//v13 = a1x->dword_0xA4_164;
			if (a1x->playerData_0xA4_164x->str_611.spellCooldowns_0x367_871x.SpellEnabled[a2])
				return 0;
			if (a1x->mana_0x90_144 < v11x->maxMana_0x8C_140)
				return 0;
			/*v14 = ((((255
				- a1x->playerData_0xA4_164x->word_0x244_580
				- (__CFSHL__((255 - a1x->playerData_0xA4_164x->word_0x244_580) >> 31, 2)
					+ 4 * ((255 - a1x->playerData_0xA4_164x->word_0x244_580) >> 31))) >> 2)
				+ 20) << 11)
				/ 360;*/
			v14 = (((((255 - a1x->playerData_0xA4_164x->aiPerception_0x244_580) - (my_sign32(255 - a1x->playerData_0xA4_164x->aiPerception_0x244_580) << 2) + my_sign32(255 - a1x->playerData_0xA4_164x->aiPerception_0x244_580)) >> 2) + 20) << 11) / 360;

			if ((unsigned __int16)AngleDifference_582B0(a1x->yaw_0x1C_28, a1x->roll_0x20_32) >= v14)
				return 0;
			return v11x;
		case 2u:
			v15x = GetPlayerSpellEntity_146C0(a1x, a2);
			v16x = v15x;
			if (!v15x)
				return 0;
			//v17 = 80 * a2 + 26 * v15x->byte_0x46_70;
			//if (a1x->dword_0x8C_140 < *(int *)((char *)&*(int32_t*)& SPELLS_BEGIN_BUFFER_DA818[10] + v17)
			//	|| a1x->dword_0x90_144 < *(int *)((char *)&*(int32_t*)& SPELLS_BEGIN_BUFFER_DA818[6] + v17))
			if (a1x->maxMana_0x8C_140 < SPELLS_BEGIN_BUFFER_str[a2].subspell[v15x->levelPar3_0x46_70].maxManaLimit_A
				|| a1x->mana_0x90_144 < SPELLS_BEGIN_BUFFER_str[a2].subspell[v15x->levelPar3_0x46_70].manaCost_6)
			{
				return 0;
			}
			//v18 = a1x->dword_0xA4_164;
			//v19 = v18 + 2 * a2;
			if (a1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58)
			{
				if (!v16x->stage_0x2E_46 && !a1x->playerData_0xA4_164x->str_611.spellCooldowns_0x367_871x.SpellEnabled[a2])
				{
					if (CanPlaceCastleArea_11A10(Entities_EA3E4[a1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58]))
					{
						if (a1x->mana_0x90_144 >= v16x->maxMana_0x8C_140)
						{
							v20 = 255 - a1x->playerData_0xA4_164x->aiPerception_0x244_580;
							if ((unsigned __int16)AngleDifference_582B0(a1x->yaw_0x1C_28, a1x->roll_0x20_32) < ((((signed int)(v20 - (__CFSHL__(HIDWORD(v20), 2) + 4 * HIDWORD(v20))) >> 2)
								+ 20) << 11)
								/ 360)
								return v16x;
						}
					}
				}
			}
			else if (!a1x->playerData_0xA4_164x->str_611.spellCooldowns_0x367_871x.SpellEnabled[a2] && a1x->mana_0x90_144 >= v16x->maxMana_0x8C_140)
			{
				return v16x;
			}
			break;
		case 3u:
			v2x = GetPlayerSpellEntity_146C0(a1x, a2);
			v3x = v2x;
			if (!v2x)
				return 0;
			//v4 = 80 * a2 + 26 * v2x->byte_0x46_70;
			//if (a1x->dword_0x8C_140 < *(int *)((char *)&*(int32_t*)& SPELLS_BEGIN_BUFFER_DA818[10] + v4))
			if (a1x->maxMana_0x8C_140 < SPELLS_BEGIN_BUFFER_str[a2].subspell[v2x->levelPar3_0x46_70].maxManaLimit_A)
				return 0;
			//v5 = a1x->dword_0x90_144;
			//if (v5 < *(int *)((char *)&*(int32_t*)& SPELLS_BEGIN_BUFFER_DA818[6] + v4) || v5 < v3x->dword_0x8C_140)
			if (a1x->mana_0x90_144 < SPELLS_BEGIN_BUFFER_str[a2].subspell[v2x->levelPar3_0x46_70].manaCost_6 || a1x->mana_0x90_144 < v3x->maxMana_0x8C_140)
				return 0;
			return v3x;
		case 4u:
		case 6u:
		case 8u:
		case 0xBu:
			v7x = GetPlayerSpellEntity_146C0(a1x, a2);
			v8x = v7x;
			if (!v7x)
				return 0;
			//		v9 = 26 * v7x->byte_0x46_70 + 80 * a2;
					//if (a1x->dword_0x8C_140 < *(int *)((char *)&*(int32_t*)& SPELLS_BEGIN_BUFFER_DA818[10] + v9)
		//				|| a1x->dword_0x90_144 < *(int *)((char *)&*(int32_t*)& SPELLS_BEGIN_BUFFER_DA818[6] + v9)
			if (a1x->maxMana_0x8C_140 < SPELLS_BEGIN_BUFFER_str[a2].subspell[v7x->levelPar3_0x46_70].maxManaLimit_A
				|| a1x->mana_0x90_144 < SPELLS_BEGIN_BUFFER_str[a2].subspell[v7x->levelPar3_0x46_70].manaCost_6
				|| v8x->stage_0x2E_46
				|| a1x->playerData_0xA4_164x->str_611.spellCooldowns_0x367_871x.SpellEnabled[a2]
				|| a1x->mana_0x90_144 < v8x->maxMana_0x8C_140)
			{
				return 0;
			}
			return v8x;
		case 5u:
		case 0xAu:
		case 0xCu:
		case 0xFu:
		case 0x11u:
		case 0x14u:
			goto LABEL_43;
		}
	}
	return 0;
}
// DA81E: using guessed type int x_DWORD_DA81E;
// DA822: using guessed type int x_DWORD_DA822;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000155E0) --------------------------------------------------------
signed int WizardAiCanGrowCastle_155E0(type_entity_0x6E8E* a1x)//1f65e0
{
	type_entity_0x6E8E* v1x; // ebx
	//uint8_t* v2; // eax
	__int64 v4; // rax

	v1x = GetPlayerSpellEntity_146C0(a1x, 2u);
	if (v1x)
	{
		//v2 = (uint8_t*)a1x->dword_0xA4_164;
		if (a1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58)
		{
			if (!v1x->stage_0x2E_46 && !a1x->playerData_0xA4_164x->str_611.spellCooldowns_0x367_871x.SpellEnabled[2])//0x1F6641 cmp word ptr [ebx+2Eh], 0 (ebx = v1x)
			{
				if (CanPlaceCastleArea_11A10(Entities_EA3E4[a1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58]))
				{
					if (a1x->maxMana_0x8C_140 >= v1x->maxMana_0x8C_140)
					{
						v4 = 255 - a1x->playerData_0xA4_164x->aiPerception_0x244_580;
						if ((unsigned __int16)AngleDifference_582B0(a1x->yaw_0x1C_28, a1x->roll_0x20_32) < ((((signed int)(v4 - (__CFSHL__(HIDWORD(v4), 2) + 4 * HIDWORD(v4))) >> 2)
							+ 20) << 11)
							/ 360)
							return 1;
					}
				}
			}
		}
		else if (!a1x->playerData_0xA4_164x->str_611.spellCooldowns_0x367_871x.SpellEnabled[2] && a1x->maxMana_0x8C_140 >= v1x->maxMana_0x8C_140)
		{
			return 1;
		}
	}
	return 0;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000156F0) --------------------------------------------------------
bool IsPlayerSpellActive_156F0(type_entity_0x6E8E* a1x, unsigned __int8 a2)//1f66f0
{
	type_entity_0x6E8E* v2x; // eax
	bool result; // eax

	result = 0;
	if (a2 < 0x1Au)
	{
		v2x = GetPlayerSpellEntity_146C0(a1x, a2);
		if (v2x)
		{
			if (v2x->stage_0x2E_46 > 0)
				result = 1;
		}
	}
	return result;
}

//----- (00015730) --------------------------------------------------------
bool HasEnoughManaForSpell_15730(type_entity_0x6E8E* a1x, unsigned __int8 a2)//1f6730
{
	return a1x->maxMana_0x8C_140 >= GetPlayerSpellEntity_146C0(a1x, a2)->maxMana_0x8C_140;
}

//----- (00015760) --------------------------------------------------------
type_entity_0x6E8E* GetActivePlayerSpellEntity_15760(type_entity_0x6E8E* a1x, unsigned __int8 a2)//1f6760
{
	type_entity_0x6E8E* resultx; // eax

	resultx = GetPlayerSpellEntity_146C0(a1x, a2);
	if (!resultx || resultx->stage_0x2E_46 <= 0)
		resultx = 0;
	return resultx;
}

//int debugcounter_1f6790 = 0;
//----- (00015790) --------------------------------------------------------
char WizardAiChooseAttackSpell_15790(type_entity_0x6E8E* a1x)//1f6790
{
	int v1; // eax
	//int v2; // esi
	int v3; // eax
	int v4; // ecx
	int v5; // edi
	int i; // edi
	char* j; // esi
	int k; // edi
	char v10; // dl
	int v11; // edi
	type_entity_0x6E8E* v12x; // [esp+0h] [ebp-4h]

	v12x = Entities_EA3E4[a1x->targetId_0x96_150];
	v1 = (a1x->maxMana_0x8C_140 - (my_sign32(a1x->maxMana_0x8C_140) << 2) + my_sign32(a1x->maxMana_0x8C_140)) >> 2;
	if (v1 <= a1x->mana_0x90_144)
	{
		if (a1x->playerData_0xA4_164x->word_0x1A4_420)
		{
			v3 = v1 + 6000;
			v4 = a1x->maxMana_0x8C_140;
			if (v3 >= v4)
				v3 = v4 / 2;
			if (v3 <= a1x->mana_0x90_144)
				a1x->playerData_0xA4_164x->word_0x1A4_420 = 0;
		}
	}
	else
	{
		a1x->playerData_0xA4_164x->word_0x1A4_420 = 1;
	}
	if (!a1x->playerData_0xA4_164x->word_0x1A4_420)
	{
		if (GetActivePlayerSpellEntity_15760(Entities_EA3E4[a1x->targetId_0x96_150], 8u))
		{
			v5 = a1x->playerData_0xA4_164x->aiPerception_0x244_580;
			if (watcomrand() % 255 < v5)
			{
				for (i = a1x->playerData_0xA4_164x->str_611.SpellLevels_0x41D_1053z.SpellIndex[7]; i >= 0; i--)
				{
					if ((signed __int16)WizardTrySelectSpell_15F20(a1x, i, 7) == 7)
						return 7;
				}
			}
		}
		for (j = (char*)unk_D3F80x; ; j++)//xxx89b
		{
			v11 = *j;
			if (v11 == -1)
				break;
			for (k = a1x->playerData_0xA4_164x->str_611.SpellLevels_0x41D_1053z.SpellIndex[v11]; k >= 0; k--)
			{
				if ((signed __int16)WizardTrySelectSpell_15F20(a1x, k, *j) == *j)
				{
					if (*j == 19)
					{
						v10 = v12x->model_0x40_64;
						if (!v10 || v10 == 1)
							return *j;
					}
					if (*j != 19)
						return *j;
				}
			}
		}
	}
	return -1;
}

//----- (00015910) --------------------------------------------------------
char WizardAiChooseSpell_15910(type_entity_0x6E8E* a1x)//1f6910
{
	int v1; // eax
	//int v2; // esi
	int v3; // eax
	int v4; // ecx
	unsigned __int8* i; // esi
	signed __int16 j; // di
	__int16 v7; // ax
	int v8; // eax

	v1 = (a1x->maxMana_0x8C_140 - (__CFSHL__(a1x->maxMana_0x8C_140 >> 31, 2) + 4 * (a1x->maxMana_0x8C_140 >> 31))) >> 2;
	if (v1 <= a1x->mana_0x90_144)
	{
		//v2 = a1x->dword_0xA4_164;
		if (a1x->playerData_0xA4_164x->word_0x1A4_420)
		{
			v3 = v1 + 6000;
			v4 = a1x->maxMana_0x8C_140;
			if (v3 >= v4)
				v3 = v4 / 2;
			if (v3 <= a1x->mana_0x90_144)
				a1x->playerData_0xA4_164x->word_0x1A4_420 = 0;
		}
	}
	else
	{
		a1x->playerData_0xA4_164x->word_0x1A4_420 = 1;
	}
	if (!a1x->playerData_0xA4_164x->word_0x1A4_420)
	{
		for (i = (unsigned __int8*)unk_D3F89x; ; i++)
		{
			v8 = *i;
			if (v8 == -1)
				break;
			for (j = a1x->playerData_0xA4_164x->str_611.SpellLevels_0x41D_1053z.SpellIndex[v8]; j >= 0; j--)
			{
				v7 = WizardTrySelectSpell_15F20(a1x, j, *i);
				if (v7 == *i)
					return v7;
			}
		}
	}
	LOBYTE(v7) = -1;
	return v7;
}

/*
bool memory_readable(void* ptr, size_t byteCount)
{
	MEMORY_BASIC_INFORMATION mbi;
	if (VirtualQuery(ptr, &mbi, sizeof(MEMORY_BASIC_INFORMATION)) == 0)
		return false;

	if (mbi.State != MEM_COMMIT)
		return false;

	if (mbi.Protect == PAGE_NOACCESS || mbi.Protect == PAGE_EXECUTE)
		return false;

	// This checks that the start of memory block is in the same "region" as the
	// end. If it isn't you "simplify" the problem into checking that the rest of
	// the memory is readable.
	size_t blockOffset = (size_t)((char*)ptr - (char*)mbi.AllocationBase);
	size_t blockBytesPostPtr = mbi.RegionSize - blockOffset;

	if (blockBytesPostPtr < byteCount)
		return memory_readable((char*)ptr + blockBytesPostPtr,
			byteCount - blockBytesPostPtr);

	return true;
}*/

//----- (000159E0) --------------------------------------------------------
void UpdateWizardAiTargets_159E0()//1f69e0
{
	type_entity_0x6E8E* v0x; // edi
	type_entity_0x6E8E* v1x; // esi
	type_entity_0x6E8E* v2x; // ecx
	unsigned __int8 v3; // al
	uint32_t v4; // ebx
	uint32_t v6; // ebx
	unsigned __int8 v7; // al
	type_entity_0x6E8E* v10x = NULL; // eax
	uint16_t* v89;

	v0x = gameState_D41A4.projectileList_38531;
	if (v0x > Entities_EA3E4[0])
	{
		while (1)
		{
			if (v0x->flags_0xc_12_15.byte[1] & 0x20)
				goto LABEL_48;
			v1x = Entities_EA3E4[v0x->id_0x1A_26];
			if (v1x <= Entities_EA3E4[0])
				goto LABEL_48;
			if (v1x->class_0x3F_63 != 3)
				goto LABEL_48;
			v2x = Entities_EA3E4[v0x->targetId_0x96_150];
			if (v2x <= Entities_EA3E4[0])
				goto LABEL_48;
			v0x->flags_0xc_12_15.byte[1] |= 0x20u;
			v3 = v2x->class_0x3F_63;
			if (v3 < 3u)
				goto LABEL_48;
			if (v3 <= 3u)
				break;
			if (v3 != 10)
				goto LABEL_48;
			if (v0x->model_0x40_64 != 1)
				goto LABEL_48;
			if (v2x->model_0x40_64 != 39)
				goto LABEL_48;
			v10x = Entities_EA3E4[v2x->playerEntityIndex_0x94_148];
			if (v10x <= Entities_EA3E4[0] || v10x->class_0x3F_63 != 3)
				goto LABEL_48;
			v6 = ((v2x->mana_0x90_144 - (__CFSHL__(v2x->mana_0x90_144 >> 31, 2) + 4 * (v2x->mana_0x90_144 >> 31))) >> 2)
				+ v10x->playerData_0xA4_164x->aiTargets_0x1FC_508[4 * v1x->playerData_0xA4_164x->playerColorIndex_0x38_56 + 4];
			if (v6 < 0)
				v6 = 0;
			if (v6 > 0xffff)
				v6 = 0xffff;
			v89 = &v10x->playerData_0xA4_164x->aiTargets_0x1FC_508[4 * v1x->playerData_0xA4_164x->playerColorIndex_0x38_56 + 4];
		LABEL_47:
			*v89 = v6;
		LABEL_48:
			v0x = v0x->next_0;
			if (v0x <= Entities_EA3E4[0])
				return;
		}
		if (v2x->model_0x40_64 == 2)
		{
			v4 = Entities_EA3E4[v2x->id_0x1A_26]->playerData_0xA4_164x->aiTargets_0x1FC_508[4 * v1x->playerData_0xA4_164x->playerColorIndex_0x38_56 + 4];
			if (v0x->model_0x40_64 < 0xAu)
			{
				if (v0x->model_0x40_64 >= 3u && v0x->model_0x40_64 <= 4u)
					goto LABEL_17;
			LABEL_18:
				v4 += 1000;
			}
			else if (v0x->model_0x40_64 > 0xAu)
			{
				if (v0x->model_0x40_64 != 11)
					goto LABEL_18;
			LABEL_17:
				v4 += 5000;
			}
			if (v4 < 0)
				v4 = 0;
			if (v4 > 0xFFFF)
				v4 = 0xFFFF;
			Entities_EA3E4[v2x->id_0x1A_26]->playerData_0xA4_164x->aiTargets_0x1FC_508[4 * v1x->playerData_0xA4_164x->playerColorIndex_0x38_56 + 4] = v4;

			if (v4 > 50000 - v1x->maxMana_0x8C_140 / 10 * v2x->playerData_0xA4_164x->aiAggression_0x242_578 / 255)
				Entities_EA3E4[v2x->id_0x1A_26]->playerData_0xA4_164x->aiTargets_0x1FC_508[4 * v1x->playerData_0xA4_164x->playerColorIndex_0x38_56 + 5] = 1;
			goto LABEL_48;
		}
		v6 = Entities_EA3E4[v2x->id_0x1A_26]->playerData_0xA4_164x->aiTargets_0x1FC_508[4 * v1x->playerData_0xA4_164x->playerColorIndex_0x38_56 + 4];
		v7 = v0x->model_0x40_64;
		if (v7 < 0xAu)
		{
			if (v7 < 3u || v7 > 4u)
				goto LABEL_32;
		}
		else
		{
			if (v7 <= 0xAu)
			{
			LABEL_33:
				if (v6 < 0)
					v6 = 0;
				if (v6 > 0xFFFF)
					v6 = 0xFFFF;
				v89 = &Entities_EA3E4[v2x->id_0x1A_26]->playerData_0xA4_164x->aiTargets_0x1FC_508[4 * v1x->playerData_0xA4_164x->playerColorIndex_0x38_56 + 4];
				goto LABEL_47;
			}
			if (v7 != 11)
			{
			LABEL_32:
				v6 += 500;
				goto LABEL_33;
			}
		}
		v6 += 3000;
		goto LABEL_33;
	}
}

//----- (00015CB0) --------------------------------------------------------
type_entity_0x6E8E* FindNearestEntityTargeting_15CB0(/*type_str_0x6E8E* a1,*/ type_entity_0x6E8E* a2x)//1f6cb0
{
	unsigned int v2; // esi
	type_entity_0x6E8E* ix; // ebx
	unsigned int v4; // eax
	type_entity_0x6E8E* result; // eax

	type_entity_0x6E8E* a1;
	//fix
	a1 = 0;
	//fix
	v2 = -1;
	for (ix = gameState_D41A4.projectileList_38531; ix > Entities_EA3E4[0]; ix = ix->next_0)
	{
		if (ix->targetId_0x96_150 == a2x->id_0x1A_26)
		{
			v4 = Maths::EuclideanDistXY_584D0(&a2x->position_0x4C_76, &ix->position_0x4C_76);
			if (v4 < v2)
			{
				v2 = v4;
				a1 = ix;
			}
		}
	}
	if (v2 >= 0x1900000)
		result = 0;
	else
		result = a1;
	return result;
}
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00015D20) --------------------------------------------------------
void SetWizardStrafeSpeed_15D20(type_entity_0x6E8E* a1x)//1f6d20
{
	//int index; // eax

	//index = a1x->dword_0xA4_164;
	if (!a1x->playerData_0xA4_164x->aiSteerCounter_0x45E_1118)
		a1x->playerData_0xA4_164x->strafeSpeed_0x10_16 = 80;
	//	return result;
}

//----- (00015D40) --------------------------------------------------------
char RateWizardTarget_15D40(__int16 a1, type_entity_0x6E8E* a2x, type_entity_0x6E8E* a3)//1f6d40
{
	unsigned int result; // eax
	signed __int16 i; // si
	signed __int16 v5; // si
	signed __int16 v6; // si

	result = Maths::EuclideanDistXY_584D0(&a2x->position_0x4C_76, &a3->position_0x4C_76);
	if (result >= 0x100000)
		return result;
	result = a3->model_0x40_64;
	if (result < 3u)
	{
		if (result)
			return result;
	}
	else if (result > 3u)
	{
		if (result == 4)
		{
			//result = a2x->dword_0xA4_164;
			v6 = a2x->playerData_0xA4_164x->str_611.SpellLevels_0x41D_1053z.SpellIndex[6];
			if (v6 >= 0)
			{
				while (1)
				{
					result = WizardTrySelectSpell_15F20(a2x, v6, 6);
					if (result == 6)
						break;
					if (--v6 < 0)
						return result;
				}
				result = WizardAiCastSpell_14E10(a2x, 6u);
			}
		}
		return result;
	}
	//result = a2x->dword_0xA4_164;
	for (i = a2x->playerData_0xA4_164x->str_611.SpellLevels_0x41D_1053z.SpellIndex[8]; i >= 0; i--)
	{
		result = WizardTrySelectSpell_15F20(a2x, i, 8);
		a1 = result;
		if (result == 8)
		{
			result = WizardAiCastSpell_14E10(a2x, 8u);
			break;
		}
	}
	if (a1 != 8)
	{
		//result = a2x->dword_0xA4_164;
		v5 = a2x->playerData_0xA4_164x->str_611.SpellLevels_0x41D_1053z.SpellIndex[6];
		if (v5 >= 0)
		{
			while (1)
			{
				result = WizardTrySelectSpell_15F20(a2x, v5, 6);
				if (result == 6)
					break;
				if (--v5 < 0)
					return result;
			}
			result = WizardAiCastSpell_14E10(a2x, 6u);
		}
	}
	return result;
}

//----- (00015E60) --------------------------------------------------------
bool HasAttackSpell_15E60(type_entity_0x6E8E* a1x)//1f6e60
{
	return GetPlayerSpellEntity_146C0(a1x, 0)
		|| GetPlayerSpellEntity_146C0(a1x, 7u)
		|| GetPlayerSpellEntity_146C0(a1x, 0x12u)
		|| GetPlayerSpellEntity_146C0(a1x, 0x10u)
		|| GetPlayerSpellEntity_146C0(a1x, 0x14u)
		|| GetPlayerSpellEntity_146C0(a1x, 0x15u)
		|| GetPlayerSpellEntity_146C0(a1x, 9u);
}

//----- (00015EE0) --------------------------------------------------------
char FindLocalWizard_15EE0()//1f6ee0
{
	type_entity_0x6E8E* v0x; // eax

	v0x = gameState_D41A4.class3List_38519;
	if (v0x <= Entities_EA3E4[0])
		return 1;
	do
	{
		if (!v0x->model_0x40_64)
			x_DWORD_E8840 = v0x;
		v0x = v0x->next_0;
	} while (v0x > Entities_EA3E4[0]);
	return 1;
}
// D41A4: using guessed type int x_DWORD_D41A4;
// E8840: using guessed type int x_DWORD_E8840;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00015F20) --------------------------------------------------------
signed int WizardTrySelectSpell_15F20(type_entity_0x6E8E* a1x, signed __int16 a2, signed int a3)//1f6f20
{
	//int v3; // ebx
	type_entity_0x6E8E* v4x; // eax
	//int v5; // ebx
	signed int result; // eax

	//v3 = 80 * (signed __int16)a3;
	//if ((unsigned __int8)(*xadataspellsdat.colorPalette_var28)[v3] <= a2)
	if (SPELLS_BEGIN_BUFFER_str[a3].levelCount_0 <= a2)
		return 0;
	v4x = GetPlayerSpellEntity_146C0(a1x, a3);
	if (!v4x)
		return 0;
	SetSpell_6D5E0(v4x, a2);
	if (!WizardCanCastSpell_15170(a1x, a3))
		return 0;
	//v5 = 26 * a2 + v3;
	//if (a1x->dword_0x8C_140 < *(int *)((char *)&*(int32_t*)& SPELLS_BEGIN_BUFFER_DA818[10] + v5)
//		|| a1x->dword_0x90_144 < *(int *)((char *)&*(int32_t*)& SPELLS_BEGIN_BUFFER_DA818[6] + v5))
	if (a1x->maxMana_0x8C_140 < SPELLS_BEGIN_BUFFER_str[a3].subspell[a2].maxManaLimit_A
		|| a1x->mana_0x90_144 < SPELLS_BEGIN_BUFFER_str[a3].subspell[a2].manaCost_6)
	{
		result = -1;
	}
	else
	{
		result = a3;
	}
	return result;
}
// DA81E: using guessed type int x_DWORD_DA81E;
// DA822: using guessed type int x_DWORD_DA822;

//----- (00015FC0) --------------------------------------------------------
signed int WizardAiMetamorph_15FC0(type_entity_0x6E8E* a1x)//1f6fc0
{
	unsigned __int8 v1; // bl
	type_entity_0x6E8E* v2x; // esi
	type_entity_0x6E8E* ix; // esi
	char v5; // ah
	unsigned int v6; // eax
	type_entity_0x6E8E* kx; // esi
	unsigned int v8; // eax
	int v9; // eax
	type_entity_0x6E8E* v11x; // [esp+0h] [ebp-18h]
	type_entity_0x6E8E* v12x; // [esp+4h] [ebp-14h]
	unsigned int v13; // [esp+8h] [ebp-10h]
	unsigned int v14; // [esp+Ch] [ebp-Ch]
	char* j; // [esp+10h] [ebp-8h]
	type_entity_0x6E8E* v16x; // [esp+14h] [ebp-4h]

	v11x = 0;
	v16x = 0;
	v13 = -1;
	v14 = -1;
	v1 = 0;
	v2x = Entities_EA3E4[a1x->targetId_0x96_150];
	if (!IsPlayerSpellActive_156F0(a1x, 4u) || !IsTargetStillValid_14C60(a1x, v2x))
	{
		v12x = GetPlayerSpellEntity_146C0(a1x, 4u);
		if (!v12x)
			return 0;
		for (ix = gameState_D41A4.class3List_38519; ix > Entities_EA3E4[0]; ix = ix->next_0)
		{
			if (ix->id_0x1A_26 != a1x->id_0x1A_26)
			{
				v5 = ix->model_0x40_64;
				if (!v5 || v5 == 1)
				{
					v6 = Distance3D_583F0(&a1x->position_0x4C_76, &ix->position_0x4C_76);
					if (v6 < v14)
					{
						v11x = ix;
						v14 = v6;
					}
				}
			}
		}
		if (v14 > 0x1400)
			v11x = 0;
		if (!v11x)
			return 0;
		for (j = (char*)unk_D3F91x; ; j++)
		{
			v9 = *j;
			if (v9 == -1)
				break;
			for (kx = gameState_D41A4.creatureLists_38403x[v9]; kx > Entities_EA3E4[0]; kx = kx->next_0)
			{
				if (kx->id_0x1A_26 != a1x->id_0x1A_26)
				{
					v8 = Distance3D_583F0(&v11x->position_0x4C_76, &kx->position_0x4C_76);
					if (v8 < v13)
					{
						v16x = kx;
						v13 = v8;
						v1 = *j;
					}
				}
			}
		}
		if (!v16x || v13 >= 0x1400 || !v1)
			return 0;
		if (v1 < 0x10u)
		{
			if (v1 == 2)
			{
			LABEL_35:
				SetSpell_6D5E0(v12x, 0);
				goto LABEL_38;
			}
		}
		else
		{
			if (v1 <= 0x10u)
			{
				SetSpell_6D5E0(v12x, 2);
				goto LABEL_38;
			}
			if (v1 >= 0x13u)
			{
				if (v1 > 0x13u)
				{
					if (v1 == 25)
						SetSpell_6D5E0(v12x, 1);
					goto LABEL_38;
				}
				goto LABEL_35;
			}
		}
	LABEL_38:
		a1x->targetId_0x96_150 = v16x - world_D41A0.entities_0x6E8E;
		a1x->targetKey_0x98_152 = GetEntityKey_14C40(v16x);
		return 1;
	}
	return 1;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000161A0) --------------------------------------------------------
signed int WizardAiAttackMetamorphed_161A0(type_entity_0x6E8E* a1x)//1f71a0
{
	type_entity_0x6E8E* v1x; // esi
	int v2; // eax
	int v3; // eax
	type_entity_0x6E8E* v4x; // eax
	int v5; // edi
	type_entity_0x6E8E* v6x; // edi
	type_entity_0x6E8E* ix; // esi
	char v8; // cl
	unsigned int v9; // eax
	signed __int8 v10; // al
	signed int result; // eax
	int v12; // eax
	__int16 v13; // ax
	type_entity_0x6E8E* v14x; // [esp+4h] [ebp-8h]
	unsigned int v15; // [esp+8h] [ebp-4h]

	v1x = Entities_EA3E4[a1x->targetId_0x96_150];
	v15 = -1;
	if (IsTargetStillValid_14C60(a1x, Entities_EA3E4[a1x->targetId_0x96_150]))
	{
		WizardAiCastSpell_14E10(a1x, 4u);
		Maths::EuclideanDistXY_584D0(&a1x->position_0x4C_76, &v1x->position_0x4C_76);
		a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v1x->position_0x4C_76);
		v2 = a1x->position_0x4C_76.z - (int16_t)(v1x->position_0x4C_76.z + 512);
		if (a1x->position_0x4C_76.z != v1x->position_0x4C_76.z + 512)
		{
			if (v2 <= 0)
				v2 = -1;
			//LOWORD(v2) = -1;
			else
				//LOWORD(v2) = 1;
				v2 = 1;
		}
		a1x->position_0x4C_76.z += v2 * a1x->typeParams_0xA0_160x->climbSpeed_160_0xe_14;
		a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v1x->position_0x4C_76);
		v3 = a1x->position_0x4C_76.z - (int16_t)(v1x->position_0x4C_76.z + 512);
		if (a1x->position_0x4C_76.z != v1x->position_0x4C_76.z + 512)
		{
			if (a1x->position_0x4C_76.z - (v1x->position_0x4C_76.z + 512) <= 0)
				v3 = -1;
			//LOWORD(v3) = -1;
			else
				v3 = 1;
			//LOWORD(v3) = 1;
		}
		a1x->position_0x4C_76.z += v3 * a1x->typeParams_0xA0_160x->climbSpeed_160_0xe_14;
		v4x = GetPlayerSpellEntity_146C0(a1x, 4u);
		v14x = v4x;
		if (v4x && !v4x->levelPar3_0x46_70)
		{
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			v5 = 2 * ((signed int)(a1x->rand_0x14_20 % 0x9Du) / 79);
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			a1x->roll_0x20_32 += (v5 - 1) * (a1x->rand_0x14_20 % 0x55u);
			a1x->roll_0x20_32 &= 0x7ff;
		}
		v6x = 0;
		for (ix = gameState_D41A4.class3List_38519; ix > Entities_EA3E4[0]; ix = ix->next_0)
		{
			if (ix->id_0x1A_26 != a1x->id_0x1A_26)
			{
				v8 = ix->model_0x40_64;
				if (!v8 || v8 == 1)
				{
					v9 = Distance3D_583F0(&a1x->position_0x4C_76, &ix->position_0x4C_76);
					if (v9 < v15)
					{
						v6x = ix;
						v15 = v9;
					}
				}
			}
		}
		if (v6x != 0 && v15 < 0x1400 && v15 > 0xA00)//1f735d
		{
			a1x->targetId_0x96_150 = v6x - world_D41A0.entities_0x6E8E;
			a1x->targetKey_0x98_152 = GetEntityKey_14C40(v6x);
			v10 = WizardAiChooseAttackSpell_15790(a1x);
			a1x->playerData_0xA4_164x->speed_0xc_12 = 0;
			a1x->playerData_0xA4_164x->castLock_0xe_14 = 1;
			if (v10 != -1 && WizardAiCastSpell_14E10(a1x, v10))
			{
				WizardAiSteer_16580(a1x);
				return 1;
			}
		}
		else
		{
			//fix
			type_entity_0x6E8E v6y;
			if (v6x == 0)
			{
				v6y.position_0x4C_76.x = 0;
				v6y.position_0x4C_76.y = 0;
				v6y.position_0x4C_76.z = 0;
			}
			else
			{
				v6y.position_0x4C_76.x = v6x->position_0x4C_76.x;
				v6y.position_0x4C_76.y = v6x->position_0x4C_76.y;
				v6y.position_0x4C_76.z = v6x->position_0x4C_76.z;
			}
			//fix
			a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v6y.position_0x4C_76);
			v12 = a1x->position_0x4C_76.z - (v6y.position_0x4C_76.z + 512);
			if (a1x->position_0x4C_76.z != v6y.position_0x4C_76.z + 512)
			{
				if (a1x->position_0x4C_76.z - (v6y.position_0x4C_76.z + 512) <= 0)
					v12 = -1;
				//LOWORD(v12) = -1;
				else
					v12 = 1;
				//LOWORD(v12) = 1;
			}
			a1x->position_0x4C_76.z += v12 * a1x->typeParams_0xA0_160x->climbSpeed_160_0xe_14;
		}
		if (v14x)
		{
			if (v14x->levelPar3_0x46_70)
				v13 = a1x->minSpeed_0x84_132;
			else
				v13 = 3 * a1x->minSpeed_0x84_132;
			a1x->actSpeed_0x82_130 = v13;
		}
		WizardAiSteer_16580(a1x);
		result = 1;
	}
	else
	{
		WizardAiSteer_16580(a1x);
		result = 0;
	}
	return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000164B0) --------------------------------------------------------
bool HasAreaSpell_164B0(type_entity_0x6E8E* a1x)//1f74b0
{
	return GetPlayerSpellEntity_146C0(a1x, 0x11u)
		|| GetPlayerSpellEntity_146C0(a1x, 0x10u)
		|| GetPlayerSpellEntity_146C0(a1x, 0x12u)
		|| GetPlayerSpellEntity_146C0(a1x, 7u)
		|| GetPlayerSpellEntity_146C0(a1x, 9u)
		|| GetPlayerSpellEntity_146C0(a1x, 0x14u)
		|| GetPlayerSpellEntity_146C0(a1x, 0x13u)
		|| GetPlayerSpellEntity_146C0(a1x, 0x15u)
		|| GetPlayerSpellEntity_146C0(a1x, 0);
}

//----- (00016580) --------------------------------------------------------
signed int WizardAiSteer_16580(type_entity_0x6E8E* a1x)//1f7580
{
	__int16 v1; // di
	//__int16 v2; // dx
	signed int v3; // esi
	signed __int16 v4; // ax
	signed int result; // eax
	//int v6; // eax
	//int v7; // eax
	unsigned __int8 v8; // [esp+0h] [ebp-4h]
	//uaxis_2d v8x;

	//LOBYTE(v2) = 0;
	v8 = 0;
	v1 = a1x->yaw_0x1C_28;
	//HIBYTE(v2) = a1x->playerData_0xA4_164x->str_611_byte_0x45E_1118;
	v3 = 0;
	if (a1x->playerData_0xA4_164x->aiSteerCounter_0x45E_1118 <= 2 || a1x->playerData_0xA4_164x->aiSteerCounter_0x45E_1118 >= 8)
	{
		if (a1x->playerData_0xA4_164x->aiSteerCounter_0x45E_1118 <= 7)
			v4 = (unsigned __int8)WizardAiAvoidObstacles_169C0(a1x);
		else
			v4 = 0;
	}
	else
	{
		v4 = 3;
	}
	switch (v4)
	{
	case 0:
		a1x->playerData_0xA4_164x->aiSteerCounter_0x45E_1118 = 0;
		return 0;
	case 1:
		v8 = FindAdjacentLavaDirection_16730(/*v2,*/ a1x, 0);
		if (v8)
			a1x->yaw_0x1C_28 = steerYawTable1_D3FCE[v8];
		goto LABEL_11;
	case 2:
		v8 = FindAdjacentLavaDirection_16730(/*v2,*/ a1x, 1);
		if (v8)
			a1x->yaw_0x1C_28 = steerYawTable2_D3FE8[v8];
		goto LABEL_11;
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
		result = 0;
		a1x->playerData_0xA4_164x->aiSteerCounter_0x45E_1118++;
		break;
	default:
	LABEL_11:
		if (v8)
		{
			//v6 = a1x->dword_0xA4_164;
			v3 = 1;
			if (a1x->playerData_0xA4_164x->aiLavaDirection_0x45E_1119 != v8)
				a1x->playerData_0xA4_164x->aiLavaDirection_0x45E_1119 = v8;
			if (v1 != a1x->yaw_0x1C_28)
			{
				//v7 = a1x->dword_0xA4_164;
				a1x->actSpeed_0x82_130 = 0;
				a1x->playerData_0xA4_164x->speed_0xc_12 = 0;
				a1x->playerData_0xA4_164x->castLock_0xe_14 = 1;
			}
			a1x->roll_0x20_32 = a1x->yaw_0x1C_28;
		}
		result = v3;
		break;
	}
	return result;
}

//----- (00016730) --------------------------------------------------------
int16_t FindAdjacentLavaDirection_16730(/*int a1, */type_entity_0x6E8E* a2x, char a3)//1f7730
{
	uaxis_2d v3x; // eax
	uaxis_2d v4x; // eax
	//int result; // eax
	uaxis_2d v6x; // eax
	uaxis_2d v7x; // eax
	uaxis_2d v8x; // eax
	uaxis_2d v9x; // eax
	uaxis_2d v10x; // eax
	uaxis_2d v11x; // eax
	uaxis_2d v12x; // eax
	int v13; // [esp+0h] [ebp-8h]
	int v14; // [esp+4h] [ebp-4h]

	uaxis_2d a1y;

	v14 = a2x->position_0x4C_76.y >> 8;
	v3x._axis_2d.x = a2x->position_0x4C_76.x >> 8;
	v3x._axis_2d.y = v14 - 1;
	a1y.word = 0;
	if (mapTerrainType_10B4E0[v3x.word] == 8)
	{
		a1y._axis_2d.x = 1;
	}
	else
	{
		v3x._axis_2d.y = v14 + 1;
		if (mapTerrainType_10B4E0[v3x.word] == 8)
			a1y._axis_2d.x = 4;
	}
	v3x._axis_2d.y = a2x->position_0x4C_76.y >> 8;
	if (a3)
		v3x._axis_2d.x++;
	else
		v3x._axis_2d.x--;
	if (mapTerrainType_10B4E0[v3x.word] == 8)
	{
		if (a3)
		{
			a1y._axis_2d.x |= 2;
			goto LABEL_18;
		}
	LABEL_17:
		a1y._axis_2d.x |= 8;
		goto LABEL_18;
	}
	v13 = a2x->position_0x4C_76.x >> 8;
	if (a3)
		v3x._axis_2d.x = v13 - 1;
	else
		v3x._axis_2d.x = v13 + 1;
	if (mapTerrainType_10B4E0[v3x.word] == 8)
	{
		if (!a3)
		{
			a1y._axis_2d.x |= 2;
			goto LABEL_18;
		}
		goto LABEL_17;
	}
LABEL_18:
	if (a1y._axis_2d.x)
		return a1y.word;
	if (a3)
	{
		switch (a2x->playerData_0xA4_164x->aiLavaDirection_0x45E_1119)
		{
		case 1:
		case 9:
			v9x._axis_2d.x = a2x->position_0x4C_76.x >> 8;
			v9x._axis_2d.y = (a2x->position_0x4C_76.y >> 8) - 1;
			v9x._axis_2d.x--;
			if (mapTerrainType_10B4E0[v9x.word] != 8)
				return a1y.word;
			a1y.word = 1544;
			return a1y.word;
		case 2:
		case 3:
			v10x._axis_2d.x = a2x->position_0x4C_76.x >> 8;
			v10x._axis_2d.y = (a2x->position_0x4C_76.y >> 8) - 1;
			v10x._axis_2d.x++;
			if (mapTerrainType_10B4E0[v10x.word] != 8)
				return a1y.word;
			a1y.word = 3073;
			return a1y.word;
		case 4:
		case 6:
			v11x._axis_2d.x = a2x->position_0x4C_76.x >> 8;
			v11x._axis_2d.y = (a2x->position_0x4C_76.y >> 8) + 1;
			v11x._axis_2d.x++;
			if (mapTerrainType_10B4E0[v11x.word] != 8)
				return a1y.word;
			a1y.word = 2306;
			return a1y.word;
		case 8:
		case 0xC:
			v12x._axis_2d.x = a2x->position_0x4C_76.x >> 8;
			v12x._axis_2d.y = (a2x->position_0x4C_76.y >> 8) + 1;
			v12x._axis_2d.x--;
			if (mapTerrainType_10B4E0[v12x.word] == 8)
				a1y.word = 772;
			return a1y.word;
		default:
			return a1y.word;
		}
		return a1y.word;
	}
	switch (a2x->playerData_0xA4_164x->aiLavaDirection_0x45E_1119)
	{
	case 1:
	case 3:
		v4x._axis_2d.x = a2x->position_0x4C_76.x >> 8;
		v4x._axis_2d.y = (a2x->position_0x4C_76.y >> 8) - 1;
		v4x._axis_2d.x++;
		if (mapTerrainType_10B4E0[v4x.word] != 8)
			return a1y.word;
		a1y.word = 770;
		//result = a1x;
		break;
	case 2:
	case 6:
		v6x._axis_2d.x = a2x->position_0x4C_76.x >> 8;
		v6x._axis_2d.y = (a2x->position_0x4C_76.y >> 8) + 1;
		v6x._axis_2d.x++;
		if (mapTerrainType_10B4E0[v6x.word] != 8)
			return a1y.word;
		a1y.word = 1540;
		//result = a1x;
		break;
	case 4:
		v7x._axis_2d.x = a2x->position_0x4C_76.x >> 8;
		v7x._axis_2d.y = (a2x->position_0x4C_76.y >> 8) + 1;
		v7x._axis_2d.x--;
		if (mapTerrainType_10B4E0[v7x.word] != 8)
			return a1y.word;
		a1y.word = 3080;
		//result = a1x;
		break;
	case 8:
	case 9:
		v8x._axis_2d.x = a2x->position_0x4C_76.x >> 8;
		v8x._axis_2d.y = (a2x->position_0x4C_76.y >> 8) - 1;
		v8x._axis_2d.x--;
		if (mapTerrainType_10B4E0[v8x.word] != 8)
			return a1y.word;
		a1y.word = 2305;
		//result = a1x;
		break;
	default:
		return a1y.word;
	}
	return a1y.word;
}

//----- (000169C0) --------------------------------------------------------
char WizardAiAvoidObstacles_169C0(type_entity_0x6E8E* a1x)//1f79c0
{
	type_entity_0x6E8E* v1x; // eax
	__int16 v2; // dx
	//__int16 v3; // dx
	char result; // al
	unsigned __int16 v5; // si
	unsigned __int16 v6; // si
	//int v7; // eax
	char v8; // dl
	baxis_2d v9;
	//unsigned __int8 v9x[18]; // [esp+0h] [ebp-1Ch]
	//unsigned __int8 v10; // [esp+1h] [ebp-1Bh]//1//v9x[1]
	baxis_2d v11; // [esp+4h] [ebp-18h]//2//*(int32_t*)&v9x[2]
	baxis_2d v12; // [esp+8h] [ebp-14h]//6//*(int32_t*)&v9x[6]
	baxis_2d v13; // [esp+Ch] [ebp-10h]//10//*(int16_t*)&v9x[10]
	uaxis_2d v14b; // [esp+10h] [ebp-Ch]//12//*(int16_t*)&v9x[12]
	uaxis_2d v15b; // [esp+14h] [ebp-8h]//14//*(int16_t*)&v9x[14]
	baxis_2d v16; // [esp+18h] [ebp-4h]//16//*(int16_t*)&v9x[16] //18

	uint8_t v14a;
	uint8_t v15a;

	// fix if begin
	//v3 = 0;
	// end

	if (CommandLineParams.DoDebugSequences()) {
		//add_compare(0x1f79c3, CommandLineParams.DoDebugafterload(),0x77);
	}

	//LOBYTE(*(int32_t*)&v9x[6]) = a1x->array_0x4C_76.x >> 8;
	//BYTE1(*(int32_t*)&v9x[6]) = a1x->array_0x4C_76.y >> 8;
	v12.x = a1x->position_0x4C_76.x >> 8;
	v12.y = a1x->position_0x4C_76.y >> 8;
	v11 = v12;
	v1x = Entities_EA3E4[a1x->targetId_0x96_150];
	v13.x = 0;
	v13.y = 0;
	v16.x = 0;
	v16.y = 0;
	v9.x = v1x->position_0x4C_76.x >> 8;
	v9.y = v1x->position_0x4C_76.y >> 8;
	LOBYTE(v2) = 0;
	HIBYTE(v2) = a1x->playerData_0xA4_164x->aiSteerCounter_0x45E_1118;
	switch (HIBYTE(v2))
	{
	case 0:
		v15a = FindAdjacentLavaDirection_16730(/*v2,*/ a1x, 0);
		v14a = FindAdjacentLavaDirection_16730(/*v3,*/ a1x, 1);
		if (!v14a && !v15a)
			return 0;
		if (v15a)
		{
			v5 = 0;
			while (v5 < 0x28u)
			{
				v12.x += avoidOffsetsX1_D3F96[v15a];
				v12.y += avoidOffsetsY1_D3FA4[v15a];
				v16 = v12;
				v5++;
				v15a = FindAdjacentLavaDirection2_16CA0(&v12, v15a, 0);
			}
		}
		if (v14a)
		{
			v6 = 0;
			while (v6 < 0x28u)
			{
				v11.x += avoidOffsetsX2_D3FB2[v14a];
				v11.y += avoidOffsetsY2_D3FC0[v14a];
				v13 = v11;
				v6++;
				v14a = FindAdjacentLavaDirection2_16CA0(&v11, v14a, 1);
			}
		}
		if (v15a && v14a)
		{
			if (abs(v9.y - v16.y) * abs(v9.x - v16.x) >
				abs(v9.x - v13.x) * abs(v9.y - v13.y))
			{
				//v7 = a1x->dword_0xA4_164;
				v8 = 2;
			}
			else
			{
				//v7 = a1x->dword_0xA4_164;
				v8 = 1;
			}
			goto LABEL_21;
		}
		if (!v15a)
		{
			//v7 = a1x->dword_0xA4_164;
			v8 = 2;
		LABEL_21:
			a1x->playerData_0xA4_164x->aiSteerCounter_0x45E_1118 = v8;
			return v8;
		}
		LOBYTE(v2) = 1;
		a1x->playerData_0xA4_164x->aiSteerCounter_0x45E_1118 = 1;
		return v2;
	case 1:
		v15b.word = FindAdjacentLavaDirection_16730(/*v2,*/ a1x, 0);
		if (!v15b._axis_2d.y || (unsigned __int16)TraceTileLine_16E70(&v12, &v9))
		{
			result = 1;
		}
		else
		{
			a1x->playerData_0xA4_164x->aiSteerCounter_0x45E_1118 = 3;
			result = 3;
		}
		break;
	case 2:
		v14b.word = FindAdjacentLavaDirection_16730(/*v2,*/ a1x, 1);
		if (!v14b._axis_2d.y || (unsigned __int16)TraceTileLine_16E70(&v12, &v9))
		{
			result = 2;
		}
		else
		{
			a1x->playerData_0xA4_164x->aiSteerCounter_0x45E_1118 = 3;
			result = 3;
		}
		break;
	default:
		return v2;
	}
	return result;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00016CA0) --------------------------------------------------------
int16_t FindAdjacentLavaDirection2_16CA0(baxis_2d* a2x, __int16 a3, char a4)//1f7ca0
{
	uaxis_2d v4x; // ax
	char v5; // al
	uaxis_2d v6x; // eax
	char v7; // al
	uaxis_2d v8x; // eax
	char v9; // al
	char v10; // ah
	uaxis_2d v11x; // ax
	int result; // eax
	uaxis_2d v13x; // ax
	uaxis_2d v14x; // ax
	uaxis_2d v15x; // ax
	char v16; // al
	char v17; // ah
	uaxis_2d v18x; // ax
	uaxis_2d v19x; // ax
	uaxis_2d v20x; // ax
	uaxis_2d v21x; // ax

	int16_t a1x;

	v4x._axis_2d.x = a2x->x;
	v4x._axis_2d.y = a2x->y - 1;
	a1x = 0;
	if (mapTerrainType_10B4E0[v4x.word] == 8)
	{
		a1x = 1;
	}
	else
	{
		v4x._axis_2d.y = a2x->y + 1;
		if (mapTerrainType_10B4E0[v4x.word] == 8)
			a1x = 4;
	}
	v5 = a2x->x;
	if (a4)
		v6x._axis_2d.x = v5 + 1;
	else
		v6x._axis_2d.x = v5 - 1;
	v6x._axis_2d.y = a2x->y;
	if (mapTerrainType_10B4E0[v6x.word] == 8)
	{
		if (!a4)
		{
			a1x |= 8;
			goto LABEL_18;
		}
		goto LABEL_16;
	}
	v7 = a2x->x;
	if (a4)
		v8x._axis_2d.x = v7 - 1;
	else
		v8x._axis_2d.x = v7 + 1;
	v8x._axis_2d.y = a2x->y;
	if (mapTerrainType_10B4E0[v8x.word] == 8)
	{
		if (!a4)
		{
		LABEL_16:
			a1x |= 2;
			goto LABEL_18;
		}
		a1x |= 8;
	}
LABEL_18:
	if (a1x)
		return a1x;
	if (a4)
	{
		v16 = a2x->x;
		v17 = a2x->y;
		switch (a3)
		{
		case 1:
		case 9:
			v18x._axis_2d.y = v17 - 1;
			v18x._axis_2d.x = v16 - 1;
			if (mapTerrainType_10B4E0[v18x.word] != 8)
				return a1x;
			a1x = 1544;
			return a1x;
		case 2:
		case 3:
			v19x._axis_2d.y = v17 - 1;
			v19x._axis_2d.x = v16 + 1;
			if (mapTerrainType_10B4E0[v19x.word] != 8)
				return a1x;
			a1x = 3073;
			return a1x;
		case 4:
		case 6:
			v20x._axis_2d.y = v17 + 1;
			v20x._axis_2d.x = v16 + 1;
			if (mapTerrainType_10B4E0[v20x.word] != 8)
				return a1x;
			a1x = 2306;
			return a1x;
		case 8:
		case 12:
			v21x._axis_2d.y = v17 + 1;
			v21x._axis_2d.x = v16 - 1;
			if (mapTerrainType_10B4E0[v21x.word] == 8)
				a1x = 772;
			return a1x;
		default:
			return a1x;
		}
		return a1x;
	}
	v9 = a2x->x;
	v10 = a2x->y;
	switch (a3)
	{
	case 1:
	case 3:
		v11x._axis_2d.y = v10 - 1;
		v11x._axis_2d.x = v9 + 1;
		if (mapTerrainType_10B4E0[v11x.word] != 8)
			return a1x;
		a1x = 770;
		result = a1x;
		break;
	case 2:
	case 6:
		v13x._axis_2d.y = v10 + 1;
		v13x._axis_2d.x = v9 + 1;
		if (mapTerrainType_10B4E0[v13x.word] != 8)
			return a1x;
		a1x = 1540;
		result = a1x;
		break;
	case 4:
		v14x._axis_2d.y = v10 + 1;
		v14x._axis_2d.x = v9 - 1;
		if (mapTerrainType_10B4E0[v14x.word] != 8)
			return a1x;
		a1x = 3080;
		result = a1x;
		break;
	case 8:
	case 9:
		v15x._axis_2d.y = v10 - 1;
		v15x._axis_2d.x = v9 - 1;
		if (mapTerrainType_10B4E0[v15x.word] != 8)
			return a1x;
		a1x = 2305;
		result = a1x;
		break;
	default:
		return a1x;
	}
	return result;
}

//----- (00016E70) --------------------------------------------------------
int TraceTileLine_16E70(baxis_2d* a1x, baxis_2d* a2x)//1f7e70
{
	__int16 v2; // cx
	__int16 v3; // si
	int v4; // ebx
	signed __int16 v5; // ax
	signed __int16 v6; // ax
	__int16 v7; // ax
	__int16 v8; // dx
	int j; // ecx
	uaxis_2d v10x; // ST00_2
	int i; // ecx
	//unsigned __int16 v12; // ST00_2
	uaxis_2d v12x;
	int result; // eax
	int v14; // ecx
	signed __int16 v15; // [esp+4h] [ebp-10h]
	int v16; // [esp+8h] [ebp-Ch]
	int v17; // [esp+Ch] [ebp-8h]
	signed __int16 v18; // [esp+10h] [ebp-4h]

	v2 = a2x->x - a1x->x;
	v3 = a2x->y - a1x->y;
	v17 = 2 * abs(v2);
	v4 = 0;
	v16 = 2 * abs(v3);
	if (v2 >= 0)
	{
		LOBYTE(v5) = a2x->x != a1x->x;
		HIBYTE(v5) = 0;
		v15 = v5;
	}
	else
	{
		v15 = -1;
	}
	if (v3 >= 0)
	{
		LOBYTE(v6) = a2x->y != a1x->y;
		HIBYTE(v6) = 0;
		v18 = v6;
	}
	else
	{
		v18 = -1;
	}
	v7 = a1x->x;
	v8 = a1x->y;
	if ((signed __int16)v17 <= (signed __int16)v16)
	{
		for (i = (signed __int16)v17 - ((signed __int16)v16 >> 1); ; i += v17)
		{
			v12x._axis_2d.x = v7;
			v12x._axis_2d.y = v8;
			if (mapTerrainType_10B4E0[v12x.word] == 8)
				return v4 + 1;
			if (v8 == a2x->y)
				break;
			if ((i & 0x8000u) == 0)
			{
				LOBYTE(v7) = v15 + v7;
				i -= v16;
			}
			v4++;
			v8 += v18;
		}
		result = 0;
	}
	else
	{
		for (j = (signed __int16)v16 - ((signed __int16)v17 >> 1); ; j += v16)
		{
			v10x._axis_2d.x = v7;
			v10x._axis_2d.y = v8;
			if (mapTerrainType_10B4E0[v10x.word] == 8)
			{
				v14 = v4 + 1;
				goto LABEL_24;
			}
			if (v7 == a2x->x)
				break;
			if ((j & 0x8000u) == 0)
			{
				LOBYTE(v8) = v18 + v8;
				j -= v17;
			}
			v4++;
			v7 += v15;
		}
		v14 = 0;
	LABEL_24:
		result = v14;
	}
	return result;
}

//----- (00016FC0) --------------------------------------------------------
type_entity_0x6E8E* FindNearestOtherEntity_16FC0(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//1f7fc0
{
	unsigned int v2; // edi
	type_entity_0x6E8E* v3x; // esi
	type_entity_0x6E8E* v4x; // ebx
	__int16 v6; // ax
	char v7; // ah
	unsigned int v8; // eax

	v2 = -1;
	v3x = 0;
	v4x = gameState_D41A4.class3List_38519;
	if (v4x <= Entities_EA3E4[0])
		return 0;
	do
	{
		v6 = v4x->id_0x1A_26;
		if (v6 != a1x->id_0x1A_26 && v6 != a2x->id_0x1A_26)
		{
			v7 = v4x->model_0x40_64;
			if (!v7 || v7 == 1)
			{
				v8 = Maths::EuclideanDistXY_584D0(&a1x->position_0x4C_76, &v4x->position_0x4C_76);
				if (v8 < v2)
				{
					v3x = v4x;
					v2 = v8;
				}
			}
		}
		v4x = v4x->next_0;
	} while (v4x > Entities_EA3E4[0]);
	return v3x;
}
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000223E0) --------------------------------------------------------
void CountWizards_223E0()//2033e0
{
	//uint8_t* v0; // edx
	//uint8_t* v1; // ecx
	type_entity_0x6E8E* ix; // eax
	type_entity_0x6E8E* jx; // eax
	type_entity_0x6E8E* kx; // eax
	type_entity_0x6E8E* indexx; // eax

	//v0 = x_D41A0_BYTEARRAY_0;
	//v1 = x_D41A0_BYTEARRAY_4;
	//!!!!! 2033e0: the original also counts list 0 (dragons) three times, [ecx+9603h] - maybe a bug of the original game, investigate later
	world_D41A0.dragonCount1_0x3653E = 0;
	for (ix = gameState_D41A4.creatureLists_38403x[0]; ix > Entities_EA3E4[0]; ix = ix->next_0)
		world_D41A0.dragonCount1_0x3653E++;
	world_D41A0.dragonCount2_0x36540 = 0;//!!!!! same list 0, investigate later
	for (jx = gameState_D41A4.creatureLists_38403x[0]; jx > Entities_EA3E4[0]; jx = jx->next_0)
		world_D41A0.dragonCount2_0x36540++;
	world_D41A0.dragonCount3_0x36544 = 0;//!!!!! same list 0, investigate later
	for (kx = gameState_D41A4.creatureLists_38403x[0]; kx > Entities_EA3E4[0]; kx = kx->next_0)
		world_D41A0.dragonCount3_0x36544++;
	world_D41A0.cymmerianCount_0x36542 = 0;
	for (indexx = gameState_D41A4.creatureLists_38403x[100 / 4]; indexx > Entities_EA3E4[0]; indexx = indexx->next_0)
	{
		if (indexx->actionIndex_0x45_69 != 200)
			world_D41A0.cymmerianCount_0x36542++;
	}
	//x_D41A0_BYTEARRAY_4 = (uint8_t*)v1;
	//x_D41A0_BYTEARRAY_0 = v0;
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000585D0) --------------------------------------------------------
void UpdateWizardHeights_585D0()//2395d0
{
	int v0; // esi
	type_entity_0x6E8E* ix; // ebx
	__int16 v2; // ax

	//v0 = *(uint16_t*)&(unk_D7BD6[0x8d0]);  8C4 +0xc
	v0 = str_D7BD6[66].minHeight_160_0xc_12;
	for (ix = gameState_D41A4.class3List_38519; ix > Entities_EA3E4[0]; ix = ix->next_0)
	{
		if (!ix->model_0x40_64)
		{
			v2 = GetTerrainAlt_10C40(&ix->position_0x4C_76);
			if (ix->position_0x4C_76.z < v2 + v0)
				ix->position_0x4C_76.z = v0 + v2;
		}
	}
}
// D41A4: using guessed type int x_DWORD_D41A4;
// D84A6: using guessed type __int16 x_WORD_D84A6;
// EA3E4: using guessed type int Entities_EA3E4[];
