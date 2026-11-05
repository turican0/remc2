#include "Spells.h"
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
type_SPELLS_BEGIN_BUFFER_str SPELLS_BEGIN_BUFFER_str[26] =
{ {0x03,0x00,{
{0x000000FA,0x00000064,0x00000000,0x00000000,0x00000000,0x00BA,0x0005,0x00,0x00},
{0x000000A0,0x000000FA,0x00000000,0x00000190,0x00000096,0x00BB,0x000B,0x01,0x01},
{0x00000050,0x000005DC,0x000009C4,0x00001388,0x000002EE,0x00BC,0x000F,0x02,0x00}}},
{0x03,0x00,{
{0x0000000A,0x00000064,0x00000000,0x00000000,0x00000000,0x00BD,0x0003,0x00,0x00},
{0x0000000F,0x000000FA,0x00000000,0x0000012C,0x00000032,0x00BE,0x0029,0x01,0x00},
{0x00000014,0x000003E8,0x000003E8,0x00000384,0x00000096,0x00BF,0x0033,0x02,0x00}}},
{0x03,0x00,{
{0x00000000,0x000003E8,0x00000000,0x00000000,0x00000000,0x00C0,0x0065,0x00,0x00},
{0x00000000,0x000004E2,0x00000000,0x00000019,0x00000004,0x00C1,0x0065,0x01,0x00},
{0x00000001,0x000007D0,0x00000000,0x0000007D,0x00000006,0x00C2,0x0065,0x02,0x00}}},
{0x03,0x04,{
{0x00000002,0x000003E8,0x00000000,0x00000000,0x00000000,0x00C3,0x012D,0x00,0x00},
{0x00000003,0x000009C4,0x00000000,0x0000008C,0x0000000F,0x00C4,0x01C3,0x00,0x00},
{0x00000004,0x00001388,0x00000000,0x00000118,0x0000002D,0x00C5,0x01F5,0x00,0x00}}},
{0x03,0x04,{
{0x00000064,0x00001388,0x00004E20,0x00000000,0x00000000,0x00C6,0x00C9,0x13,0x00},
{0x00000064,0x00001D4C,0x00004E20,0x00000050,0x00000014,0x00C8,0x012D,0x19,0x00},
{0x00000064,0x00003A98,0x000061A8,0x000000F0,0x0000003C,0x00C9,0x01C7,0x10,0x00}}},
{0x03,0x00,{
{0x00000001,0x000001F4,0x00000000,0x00000000,0x00000000,0x00CA,0x000B,0x00,0x00},
{0x00000001,0x000002EE,0x00000000,0x0000012C,0x0000004B,0x00CB,0x0015,0x00,0x00},
{0x0000000A,0x000005DC,0x00000000,0x00000384,0x0000012C,0x00CC,0x0029,0x00,0x00}}},
{0x03,0x04,{
{0x00000064,0x000003E8,0x00000000,0x00000000,0x00000000,0x00CD,0x0065,0x00,0x00},
{0x00000064,0x000005DC,0x00000000,0x00000078,0x0000001E,0x00CE,0x00C9,0x00,0x00},
{0x00000064,0x00002710,0x00000000,0x00000118,0x00000046,0x00CF,0x012D,0x01,0x00}}},
{0x03,0x00,{
{0x000000C8,0x000001F4,0x000002EE,0x00000000,0x00000000,0x00D0,0x0005,0x00,0x01},
{0x000000FA,0x000009C4,0x00001388,0x00000258,0x00000096,0x00D1,0x0023,0x01,0x00},
{0x00000320,0x00002710,0x00004E20,0x00001D4C,0x000001F4,0x00D2,0x002D,0x02,0x00}}},
{0x03,0x04,{
{0x00000000,0x000009C4,0x00001388,0x00000000,0x00000000,0x00D3,0x007D,0x00,0x00},
{0x00000000,0x00000EA6,0x00001D4C,0x00000078,0x0000001E,0x00D4,0x00FB,0x00,0x00},
{0x00000000,0x00001B58,0x00002710,0x000000F0,0x0000003C,0x00D5,0x007D,0x01,0x00}}},
{0x03,0x00,{
{0x000006D6,0x000003E8,0x00001388,0x00000000,0x00000000,0x00D6,0x0009,0x02,0x00},
{0x00000DAC,0x000005DC,0x00002328,0x00000352,0x00000032,0x00D7,0x0011,0x05,0x00},
{0x00001B58,0x000007D0,0x00004650,0x0000092E,0x00000096,0x00D8,0x0019,0x0A,0x00}}},
{0x03,0x00,{
{0x00000000,0x00001388,0x00001388,0x00000000,0x00000000,0x00D9,0x000B,0x00,0x00},
{0x00000000,0x00001770,0x00001388,0x000000C8,0x0000001E,0x00DA,0x000B,0x01,0x00},
{0x00000000,0x00001D4C,0x00001388,0x00000190,0x0000005A,0x00DB,0x0015,0x02,0x00}}},
{0x03,0x04,{
{0x00000000,0x000007D0,0x000007D0,0x00000000,0x00000000,0x00DC,0x00B5,0x01,0x00},
{0x00000000,0x000009C4,0x00000BB8,0x00000064,0x0000001E,0x00DD,0x00B7,0x02,0x00},
{0x00000000,0x00000BB8,0x00000FA0,0x0000012C,0x0000005A,0x00DE,0x00B7,0x03,0x00}}},
{0x03,0x04,{
{0x00000000,0x00000708,0x00000000,0x00000000,0x00000000,0x00DF,0x0097,0x00,0x00},
{0x00000000,0x00000960,0x00002710,0x00000078,0x0000001E,0x00E0,0x0105,0x00,0x00},
{0x00000000,0x00001388,0x00002710,0x00000168,0x0000003C,0x00E1,0x0169,0x00,0x00}}},
{0x03,0x00,{
{0x000007D0,0x000001F4,0x00002710,0x00000000,0x00000000,0x00E2,0x0005,0x00,0x00},
{0x00000FA0,0x000003E8,0x00003A98,0x00000190,0x00000064,0x00E3,0x0007,0x00,0x00},
{0x0000000A,0x000004B0,0x00002EE0,0x00000320,0x000000C8,0x00E4,0x0015,0x01,0x00}}},
{0x03,0x04,{
{0x00001432,0x000009C4,0x00002710,0x00000000,0x00000000,0x00E5,0x00C3,0x00,0x00},
{0x00001E28,0x00000DAC,0x00003A98,0x00000064,0x00000032,0x00E6,0x018B,0x01,0x00},
{0x00001E28,0x00001388,0x00004E20,0x000000C8,0x00000064,0x00E7,0x025B,0x02,0x00}}},
{0x03,0x00,{
{0x00000014,0x00001388,0x00002710,0x00000000,0x00000000,0x00E8,0x001F,0x3C,0x00},
{0x00000028,0x00001D4C,0x00004E20,0x000003E8,0x00000028,0x00E9,0x0029,0x50,0x00},
{0x00000032,0x00002710,0x00009C40,0x000007D0,0x00000064,0x00EA,0x003D,0x78,0x00}}},
{0x03,0x00,{
{0x00000032,0x00001770,0x00003A98,0x00000000,0x00000000,0x00EB,0x001F,0x06,0x00},
{0x00000064,0x00002328,0x00007530,0x000005DC,0x00000028,0x00EC,0x0029,0x0C,0x00},
{0x0000012C,0x00002EE0,0x0000EA60,0x00000BB8,0x00000050,0x00ED,0x0033,0x18,0x00}}},
{0x03,0x00,{
{0x00000050,0x00002710,0x00004650,0x00000000,0x00000000,0x00EE,0x0015,0x10,0x00},
{0x00000078,0x00002EE0,0x00008CA0,0x00000708,0x0000003C,0x00EF,0x001F,0x20,0x00},
{0x00000140,0x00003A98,0x00011940,0x00000E10,0x00000078,0x00F0,0x0029,0x40,0x00}}},
{0x03,0x00,{
{0x00000078,0x00002EE0,0x00004E20,0x00000000,0x00000000,0x00F1,0x003F,0x07,0x00},
{0x000000F0,0x00003A98,0x00009C40,0x000007D0,0x00000028,0x00F2,0x0049,0x09,0x00},
{0x000001E0,0x00004650,0x00013880,0x00000FA0,0x00000050,0x00F3,0x0053,0x0B,0x00}}},
{0x03,0x00,{
{0x00000064,0x00002710,0x00004E20,0x00000000,0x00000000,0x00F4,0x012B,0x13,0x00},
{0x00000064,0x00004E20,0x00009C40,0x00000050,0x00000028,0x00F6,0x018F,0x19,0x00},
{0x00000064,0x00009C40,0x00015F90,0x000000A0,0x00000050,0x00F7,0x01F3,0x10,0x00}}},
{0x03,0x00,{
{0x000002EE,0x00003A98,0x00007530,0x00000000,0x00000000,0x00F8,0x001F,0x10,0x00},
{0x000005DC,0x00004E20,0x0000EA60,0x000009C4,0x00000019,0x00F9,0x0033,0x1A,0x00},
{0x00000BB8,0x00007530,0x00015F90,0x00001388,0x00000032,0x00FA,0x003D,0x28,0x00}}},
{0x03,0x00,{
{0x00000064,0x000007D0,0x00000000,0x00000000,0x00000000,0x00FB,0x001F,0x05,0x00},
{0x00000064,0x000009C4,0x000009C4,0x000000C8,0x00000032,0x00FC,0x0033,0x0A,0x00},
{0x000000C8,0x00001388,0x00002710,0x00000190,0x00000064,0x00FD,0x003D,0x0A,0x00}}},
{0x03,0x00,{
{0x000000FA,0x000003E8,0x00001388,0x00000000,0x00000000,0x00FE,0x001F,0x00,0x00},
{0x000001F4,0x000005DC,0x00002710,0x00000050,0x0000001E,0x00FF,0x001F,0x01,0x00},
{0x000003E8,0x000007D0,0x00002710,0x000000A0,0x0000003C,0x0100,0x001F,0x02,0x00}}},
{0x03,0x00,{
{0x000003E8,0x00002710,0x00004E20,0x00000000,0x00000000,0x0101,0x002F,0x00,0x00},
{0x00001388,0x00002EE0,0x00004E20,0x00000078,0x0000001E,0x0102,0x0043,0x01,0x00},
{0x00002710,0x00005DC0,0x0000EA60,0x000000F0,0x0000003C,0x0103,0x0057,0x02,0x00}}},
{0x03,0x00,{
{0x00000262,0x00002710,0x00004E20,0x00000000,0x00000000,0x0104,0x002B,0x10,0x00},
{0x0000044C,0x00004650,0x00008CA0,0x0000005A,0x0000001E,0x0105,0x0021,0x1A,0x00},
{0x00000A96,0x00007530,0x00015F90,0x00000118,0x00000046,0x0106,0x0021,0x20,0x00}}},
{0x03,0x00,{
{0x00000064,0x00002AF8,0x00004A38,0x00000000,0x00000000,0x0107,0x001F,0x00,0x00},
{0x00000082,0x000032C8,0x00009470,0x000000C8,0x00000028,0x0108,0x0029,0x01,0x00},
{0x00000186,0x00006590,0x0000DAC0,0x00000190,0x00000078,0x0109,0x0033,0x02,0x00}}}
};

//----- (0005C0A0) --------------------------------------------------------
void SetDefaultSpells_5C0A0()//23d0a0
{
	for (int i = 0; i < 26; i++)
	{
		SPELLS_BEGIN_BUFFER_str[i].isEnabled_1 = 0;
		SPELLS_BEGIN_BUFFER_str[i].subspell[0].fontType_0x1B &= 0xFE;
		SPELLS_BEGIN_BUFFER_str[i].subspell[1].fontType_0x1B &= 0xFE;
		SPELLS_BEGIN_BUFFER_str[i].subspell[2].fontType_0x1B &= 0xFE;
		switch (i)
		{
		case 0:
			SPELLS_BEGIN_BUFFER_str[i].subspell[1].fontType_0x1B |= 1u;
			break;
		case 3:
		case 4:
		case 6:
		case 8:
		case 11:
		case 12:
		case 14:
			SPELLS_BEGIN_BUFFER_str[i].isEnabled_1 |= 4u;// 3 4 6 8 B C E
			break;
		case 7:
			SPELLS_BEGIN_BUFFER_str[i].subspell[0].fontType_0x1B |= 1u;
			break;
		case 23:
			SPELLS_BEGIN_BUFFER_str[i].subspell[0].maxManaLimit_A = 50000;
			SPELLS_BEGIN_BUFFER_str[i].subspell[1].maxManaLimit_A = 70000;
			SPELLS_BEGIN_BUFFER_str[i].subspell[2].maxManaLimit_A = 90000;
			break;
		}
		if (SPELLS_BEGIN_BUFFER_str[i].subspell[2].maxManaLimit_A <= 0 && 1000 / SPELLS_BEGIN_BUFFER_str[i].subspell[2].manaCost_6 > 0)
		{
			SPELLS_BEGIN_BUFFER_str[i].isEnabled_1 |= 0x20u;
		}
		else
		{
			if (SPELLS_BEGIN_BUFFER_str[i].subspell[1].maxManaLimit_A > 0 || 1000 / SPELLS_BEGIN_BUFFER_str[i].subspell[1].manaCost_6 <= 0)
				SPELLS_BEGIN_BUFFER_str[i].isEnabled_1 |= 0x8u;
			else
				SPELLS_BEGIN_BUFFER_str[i].isEnabled_1 |= 0x10u;
		}
	}
}


//----- (00039FA0) --------------------------------------------------------
char CanSpellAffectEntity_39FA0(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//21afa0
{
	char result; // al
	unsigned __int8 v3; // ah
	//char v4; // bl
	unsigned __int8 v5; // ah
	unsigned __int8 v6; // dl
	unsigned __int8 v7; // ah
	unsigned __int8 v8; // dl
	bool v9; // zf

	result = 1;
	v3 = a2x->class_0x3F_63 - 1;
	if (v3 <= 0xEu)
	{
		switch (v3)
		{
		case 0u:
		case 3u:
		case 5u:
		case 6u:
		case 7u:
		case 0xAu:
		case 0xBu:
		case 0xCu:
		case 0xEu:
			return 0;
		case 1u:
			return result;
		case 2u:
			v7 = a2x->model_0x40_64;
			if (v7 < 1u)
			{
				if (v7)
					return result;
			}
			else
			{
				if (v7 > 1u)
				{
					if (v7 != 2)
						return result;
					return 0;
				}
				if (a2x->flags_0xc_12_15.byte[0] & 0x21)
					return 0;
			}
			if (a1x->id_0x1A_26 == a2x->id_0x1A_26)
				result = 0;
			break;
		case 4u:
			if (a2x->flags_0xc_12_15.byte[0] & 0x21)
				return 0;
			//v4 = a2x->byte_0x45_69;
			if (a2x->actionIndex_0x45_69 == 232)
				return 0;
			v5 = a2x->model_0x40_64;
			if (v5 < 0x16u)
				return result;
			if (v5 <= 0x16u)
				return 0;
			if (v5 != 27)
				return result;
			if (a2x->actionIndex_0x45_69 == 233)
				return 0;
			if (a2x->actionIndex_0x45_69 == 234)
				result = 0;
			return result;
		case 8u:
			v6 = a2x->model_0x40_64;
			if (!v6)
				return result;
			if (v6 < 0xDu)
				return 0;
			if (v6 > 0xEu)
				result = 0;
			return result;
		case 9u:
			v8 = a2x->model_0x40_64;
			if (v8 < 0x27u)
			{
				v9 = v8 == 6;
			}
			else
			{
				if (v8 <= 0x28u)
					return result;
				v9 = v8 == 57;
			}
			if (!v9)
				result = 0;
			return result;
		case 0xDu:
			if (a2x->flags_0xc_12_15.byte[0] & 0x21 || a2x->model_0x40_64 == 1)
				return 0;
			return result;
		}
	}
	return result;
}

//----- (0003A200) --------------------------------------------------------
void ApplySpellToEntity_3A200(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//21b200
{
	bool v2; // ah
	unsigned __int8 v3; // al
	unsigned __int8 v4; // al
	//int v5; // edx
	unsigned __int16 v6; // ax
	char v7; // [esp+0h] [ebp-4h]

	v2 = 0;
	v7 = 0;
	v3 = a2x->class_0x3F_63;
	//a2x->byte_0xc_12 |= 0x01;//0x100001;
	//a2x->byte_0xe_14 |= 0x10;
	a2x->flags_0xc_12_15.dword |= 0x100001;
	if (v3 < 3u)
		goto LABEL_13;
	if (v3 > 3u)
	{
		if (v3 != 5)
			goto LABEL_13;
		v4 = a2x->model_0x40_64;
		if (v4 < 0x12u)
		{
			if (v4 != 12)
				goto LABEL_13;
		}
		else if (v4 > 0x12u)
		{
			if (v4 == 27)
				v7 = 1;
			goto LABEL_13;
		}
		v2 = 1;
		goto LABEL_13;
	}
	if (!a2x->model_0x40_64)
	{
		//v5 = a2x->dword_0xA4_164;
		a2x->pitch_0x1E_30 = 512;
		a2x->playerData_0xA4_164x->pitch_0x157_343 = 512;
	}
LABEL_13:
	if (!v2)
	{
		a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
		v2 = a1x->rand_0x14_20 % 7u == 0;
	}
	if (v2 && !v7)
	{
		if (a2x->mailMask_0x38_56 & 1)
		{
			a2x->mail_0x5E_94.channel[0].amount.dword += a2x->life_0x8 + 1;
			v6 = a1x->id_0x1A_26;
			a2x->mail_0x5E_94.channel[0].source = v6;
			SendSpellMessage_6D8B0(v6, 0x14u, 1);
		}
	}
}

//----- (0003A7F0) --------------------------------------------------------
char IsCreatureAffectable_3A7F0(type_entity_0x6E8E* a1x)//21b7f0
{
	char result; // al
	unsigned __int8 v2; // ah
	char v3; // bh

	result = 1;
	if (a1x->class_0x3F_63 != 5)
		return 0;
	v2 = a1x->model_0x40_64;
	if (v2 < 0x16u)
	{
		if (v2 >= 0xCu && v2 <= 0xFu)
			goto LABEL_12;
	}
	else
	{
		if (v2 <= 0x17u)
			goto LABEL_12;
		if (v2 >= 0x19u)
		{
			if (v2 > 0x19u)
			{
				if (v2 > 0x1Bu)
					goto LABEL_13;
				goto LABEL_12;
			}
			if (a1x->levelPar3_0x46_70)
			{
			LABEL_12:
				result = 0;
				goto LABEL_13;
			}
		}
	}
LABEL_13:
	if (result)
	{
		v3 = a1x->StageVar2_0x49_73;
		if (v3 == 14 || v3 == 13 || v3 == 16 || v3 == 17)
			result = 0;
	}
	if (result && a1x->actionIndex_0x45_69 == 232)
		return 0;
	return result;
}

//----- (0003A8B0) --------------------------------------------------------
void UpdatePossessedCreature_3A8B0(type_entity_0x6E8E* entity)//21b8b0
{
	if (CommandLineParams.DoDebugSequences()) {
		add_compare(0x21b8b4, CommandLineParams.DoDebugafterload());
	}

	type_entity_0x6E8E* entity2 = Entities_EA3E4[entity->leaderId_0x32_50];
	type_entity_0x6E8E* entity6 = 0;
	type_entity_0x6E8E* entity8 = Entities_EA3E4[entity->leaderId_0x32_50];
	if (entity2->life_0x8 < 0 || entity2->flags_0xc_12_15.byte[1] & 4)
	{
		DisableEntityDrawing04_57F10(entity);
		return;
	}
	int typeEntity;
	if (entity->levelPar2_0x44_68)
	{
		switch (abs(entity->levelPar2_0x44_68))
		{
		case 1:
			typeEntity = 0;
			break;
		case 2:
			typeEntity = 153;
			break;
		case 3:
			typeEntity = 307;
			break;
		case 4:
			typeEntity = 445;
			break;
		case 5:
			typeEntity = 491;
			break;
		case 6:
			typeEntity = 512;
			break;
		default:
			break;
		}
		predictedAxis_EB398ar = entity->axis_0x9A_154x;
		predictedAxis_EB398ar.z = entity->position_0x4C_76.z;
		MoveEntity_57FA0(&predictedAxis_EB398ar, entity->yaw_0x1C_28, 0, typeEntity);
		CopyEntityPosition_57CF0(entity, &predictedAxis_EB398ar);
		entity->levelPar2_0x44_68++;
		if (entity->levelPar2_0x44_68 > 0 && entity->levelPar2_0x44_68 > 6)
			entity->levelPar2_0x44_68 = -5;
	}
	int terrainAltitude = GetTerrainAlt_10C40(&entity->position_0x4C_76);
	if (entity->levelPar3_0x46_70 != 9 && entity->levelPar3_0x46_70 != 7)
	{
		entity->life_0x8--;
		if (entity->life_0x8 <= 0)
			entity->levelPar3_0x46_70 = 6;
		if (world_D41A0.LevelIndex_0xc == entity8->playerData_0xA4_164x->playerColorIndex_0x38_56)
		{
			entity->flags_0xc_12_15.byte[0] &= 0xFEu;
			if (entity->param3_0x36_54 != 0xFFFF)
				entity->flags_0xc_12_15.byte[2] |= 0x80u;
		}
		else if (!(entity->instanceIndex_0x3E_62 & 7))
		{
			type_entity_0x6E8E* entity3 = Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.str_611.SpellsEnabled_0x333_819x.SpellEnabled[12]];
			if (entity3 > Entities_EA3E4[0] && entity3->stage_0x2E_46 && entity3->levelPar3_0x46_70 >= 2)
				entity->flags_0xc_12_15.byte[0] &= 0xFEu;
			else
				entity->flags_0xc_12_15.byte[0] |= 1u;
		}
		if (entity->position_0x4C_76.z < terrainAltitude)
			entity->position_0x4C_76.z = terrainAltitude;
		if (!entity->levelPar2_0x44_68 && abs(entity->position_0x4C_76.z - (terrainAltitude + 1024)) > 96)
		{
			if (entity->position_0x4C_76.z - (terrainAltitude + 1024) <= 0)
				entity->position_0x4C_76.z += 48;
			else
				entity->position_0x4C_76.z += -48;
		}
		if (isCaveLevel_D41B6)
		{
			terrainAltitude = GetCeilingAlt_10C60(&entity->position_0x4C_76) - entity->params_0x52_82.fov;
			if (entity->position_0x4C_76.z > terrainAltitude)
				entity->position_0x4C_76.z = terrainAltitude;
		}
	}
	int spellLife;
	bool correctEntity;
	type_entity_0x6E8E* targetEntity;
	switch (entity->levelPar3_0x46_70)
	{
	case 0:
		spellLife = SPELLS_BEGIN_BUFFER_str[23].subspell[entity->subSpellIndex_0x2A_42].subSpellIndex_2;
		entity->maxLife_0x4 = spellLife;
		entity->life_0x8 = spellLife;
		entity->axis_0x9A_154x = entity->position_0x4C_76;
		switch (SPELLS_BEGIN_BUFFER_str[23].subspell[entity->subSpellIndex_0x2A_42].life_0x1A)
		{
		case 0:
			entity->levelPar1_0x43_67 = 1;
			break;
		case 1:
			entity->levelPar1_0x43_67 = 2;
			break;
		case 2:
			entity->levelPar1_0x43_67 = 4;
			break;
		case 3:
			entity->levelPar1_0x43_67 = 8;
			break;
		default:
			break;
		}
		entity->levelPar3_0x46_70 = 1;
		[[fallthrough]];
	case 1:
		if (entity->param3_0x36_54 != 0xffff)
			entity->levelPar3_0x46_70 = 2;
		return;
	case 2:
		entity->flags_0xc_12_15.byte[0] &= 0xF7u;
		entity->id_0x1A_26 = entity->leaderId_0x32_50;
		if (SPELLS_BEGIN_BUFFER_str[entity->param3_0x36_54].subspell[entity->nextLinkedId_0x34_52].fontType_0x1B & 1)
			entity->fontTypeIndex_0x3D_61 = 6;
		else
			entity->fontTypeIndex_0x3D_61 = 1;
		entity->rand_0x14_20 = 9377 * entity->rand_0x14_20 + 9439;
		entity->levelPar3_0x46_70 = 3;
		entity->param_0x10_16 = (entity->rand_0x14_20 % 0x32u)+16;
		return;
	case 3:
		entity->param_0x10_16--;
		if (!entity->param_0x10_16)
			entity->levelPar3_0x46_70 = 4;
		return;
	case 4:
		if (!(entity->instanceIndex_0x3E_62 & 0xF))
		{
			uint32_t nearestDistance = 0x10000;
			for (type_entity_0x6E8E* entity4 = gameState_D41A4.class3List_38519; entity4 > Entities_EA3E4[0]; entity4 = entity4->next_0)
			{
				if (entity4->model_0x40_64 <= 1u && entity4 != entity8)
				{
					uint32_t distance = Distance3D_583F0(&entity->position_0x4C_76, &entity4->position_0x4C_76);
					if (distance < 3584 && distance < nearestDistance)
					{
						entity6 = entity4;
						nearestDistance = distance;
					}
				}
			}
			if (entity6)
			{
				entity->levelPar3_0x46_70 = 5;
				entity->targetId_0x96_150 = entity6 - world_D41A0.entities_0x6E8E;
			}
		}
		return;
	case 5:
		entity->flags_0xc_12_15.dword &= 0xFF7FFFFE;
		correctEntity = false;
		targetEntity = entity->targetId_0x96_150 ? Entities_EA3E4[(uint16_t)entity->targetId_0x96_150] : nullptr;
		if (!targetEntity
			|| targetEntity->life_0x8 < 0
			|| targetEntity->flags_0xc_12_15.byte[1] & 4)
		{
			correctEntity = true;
		}
		else
		{
			SendSpellMessage_6D8B0(entity8->id_0x1A_26, 0x17u, 1);
			type_SPELLS_BEGIN_BUFFER_str_sub* subSpell = &SPELLS_BEGIN_BUFFER_str[entity->param3_0x36_54].subspell[entity->nextLinkedId_0x34_52];
			bool isType7 = entity->param3_0x36_54 == 7
				&& SPELLS_BEGIN_BUFFER_str[entity->param3_0x36_54].subspell[entity->nextLinkedId_0x34_52].life_0x1A == 2;
			int index = 0;
			for (; index < ((isType7 != 0) + 1); )
			{
				type_entity_0x6E8E* entity5 = CastSpell_6DCA0(entity8, &entity->position_0x4C_76, entity->param3_0x36_54, subSpell, 0, 1);
				if (entity5)
				{
					entity5->id_0x1A_26 = entity->leaderId_0x32_50;
					entity5->targetId_0x96_150 = entity->targetId_0x96_150;
					AimAtEntity_655C0(entity5, targetEntity);
					entity5->yaw_0x1C_28 = entity5->roll_0x20_32;
					entity5->pitch_0x1E_30 = entity5->fov_0x22_34;
					int16_t tempYaw = entity5->yaw_0x1C_28;
					entity5->position_0x4C_76.z += entity->params_0x52_82.yaw;
					HIBYTE(tempYaw) = (HIBYTE(tempYaw) + 4) & 7;
					entity->yaw_0x1C_28 = tempYaw;
					if (entity->levelPar2_0x44_68)
					{
						entity->levelPar2_0x44_68++;
						if (entity->levelPar2_0x44_68 > 5)
							entity->levelPar2_0x44_68 = 5;
					}
					else
					{
						entity->levelPar2_0x44_68 = 1;
					}
					if (isType7)
					{
						if (index)
							entity5->yaw_0x1C_28 = (entity5->yaw_0x1C_28 - 113) & 0x7FF;
						else
							entity5->yaw_0x1C_28 = (entity5->yaw_0x1C_28 + 113) & 0x7FF;
					}
					entity->fontTypeIndex_0x3D_61--;
					if (!entity->fontTypeIndex_0x3D_61)
						correctEntity = true;
				}
				index++;
			}
		}
		if (correctEntity)
		{
			entity->targetId_0x96_150 = 0;
			entity->levelPar1_0x43_67--;
			if (entity->levelPar1_0x43_67)
				entity->levelPar3_0x46_70 = 2;
			else
				entity->levelPar3_0x46_70 = 6;
		}
		return;
	case 6:
		entity->flags_0xc_12_15.dword &= 0xFF7FFFFE;
		if (!entity->levelPar2_0x44_68)
		{
			entity->levelPar3_0x46_70 = 7;
			entity->param_0x10_16 = 10;
		}
		return;
	case 7:
		entity->param_0x10_16--;
		if (!entity->param_0x10_16)
		{
			entity->levelPar3_0x46_70 = 9;
			entity->param_0x10_16 = 3;
		}
		return;
	case 8:
		entity->levelPar3_0x46_70 = 9;
		entity->param_0x10_16 = 3;
		entity->flags_0xc_12_15.dword &= 0xFF7FFFFE;
		return;
	case 9:
		entity->position_0x4C_76.z -= 32 * entity->param_0x10_16;
		entity->param_0x10_16++;
		if (entity->position_0x4C_76.z >= terrainAltitude)
			return;
		entity->position_0x4C_76.z = terrainAltitude;
		if (TerrainTileIsWater_104D0(&entity->position_0x4C_76) == 1)
			CreateEntity_4A190(&entity->position_0x4C_76, 10, 5);
		else
			CreateEntity_4A190(&entity->position_0x4C_76, 10, 0);
		DisableEntityDrawing04_57F10(entity);
		return;
	default:
		return;
	}
}

//----- (00051120) --------------------------------------------------------
type_entity_0x6E8E* AddSpellPickup_51120(axis_3d* position, char type, char a3)//232120 - add any //tex 59
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->class_0x3F_63 = 0xF;
		event->maxLife_0x4 = 0;
		event->model_0x40_64 = type;//zde
		event->actionIndex_0x45_69 = a3;
		event->life_0x8 = 0;
		event->flags_0xc_12_15.byte[0] &= 0xF7;
		AddEventToMap_57D70(event, position);
		SetEntityIndexAndRot_49CD0(event, 77);
		SetEntityShiftRot_49EA0(event, 768, 1280);
		CopyMaxLifeToLife_49A20(event);
		SetSpell_6D5E0(event, 0);
	}
	return event;
}

//----- (000511A0) --------------------------------------------------------
type_entity_0x6E8E* AddFireballSpell_511A0(axis_3d* position)//2321a0
{
	return AddSpellPickup_51120(position, 0, 0);
}

//----- (000511C0) --------------------------------------------------------
type_entity_0x6E8E* AddPossessSpell_511C0(axis_3d* position)//2321c0
{
	return AddSpellPickup_51120(position, 1, 3);
}

//----- (000511E0) --------------------------------------------------------
type_entity_0x6E8E* AddCastleSpell_511E0(axis_3d* position)//2321e0
{
	return AddSpellPickup_51120(position, 2, 6);//add scroll to map - castle1
}

//----- (00051200) --------------------------------------------------------
type_entity_0x6E8E* AddSpeedUpSpell_51200(axis_3d* position)//232200
{
	return AddSpellPickup_51120(position, 3, 9);
}

//----- (00051220) --------------------------------------------------------
type_entity_0x6E8E* AddMorphSpell_51220(axis_3d* position)//232220
{
	return AddSpellPickup_51120(position, 4, 12);
}

//----- (00051240) --------------------------------------------------------
type_entity_0x6E8E* AddHealSpell_51240(axis_3d* position)//232240
{
	return AddSpellPickup_51120(position, 5, 15);
}

//----- (00051260) --------------------------------------------------------
type_entity_0x6E8E* AddShieldSpell_51260(axis_3d* a1x)//232260
{
	return AddSpellPickup_51120(a1x, 6, 18);
}

//----- (00051280) --------------------------------------------------------
type_entity_0x6E8E* AddLightningSpell_51280(axis_3d* position)//232280
{
	return AddSpellPickup_51120(position, 7, 21);
}

//----- (000512A0) --------------------------------------------------------
type_entity_0x6E8E* AddReboundSpell_512A0(axis_3d* position)//2322a0
{
	return AddSpellPickup_51120(position, 8, 24);
}

//----- (000512C0) --------------------------------------------------------
type_entity_0x6E8E* AddMeteorSpell_512C0(axis_3d* position)//2322c0
{
	return AddSpellPickup_51120(position, 9, 27);
}

//----- (000512E0) --------------------------------------------------------
type_entity_0x6E8E* AddTeleportSpell_512E0(axis_3d* position)//2322e0
{
	return AddSpellPickup_51120(position, 10, 30);
}

//----- (00051300) --------------------------------------------------------
type_entity_0x6E8E* AddInvisibleSpell_51300(axis_3d* position)//232300
{
	return AddSpellPickup_51120(position, 11, 33);
}

//----- (00051320) --------------------------------------------------------
type_entity_0x6E8E* AddBeyondSightSpell_51320(axis_3d* position)//232320
{
	return AddSpellPickup_51120(position, 12, 36);
}

//----- (00051340) --------------------------------------------------------
type_entity_0x6E8E* AddStealManaSpell_51340(axis_3d* position)//232340
{
	return AddSpellPickup_51120(position, 13, 39);
}

//----- (00051360) --------------------------------------------------------
type_entity_0x6E8E* AddDuelSpell_51360(axis_3d* position)//232360
{
	return AddSpellPickup_51120(position, 14, 42);
}

//----- (00051380) --------------------------------------------------------
type_entity_0x6E8E* AddTremorSpell_51380(axis_3d* position)//232380
{
	return AddSpellPickup_51120(position, 15, 45);
}

//----- (000513A0) --------------------------------------------------------
type_entity_0x6E8E* AddCraterSpell_513A0(axis_3d* position)//2323a0
{
	return AddSpellPickup_51120(position, 16, 48);
}

//----- (000513C0) --------------------------------------------------------
type_entity_0x6E8E* AddEarthquakeSpell_513C0(axis_3d* position)//2323c0
{
	return AddSpellPickup_51120(position, 17, 51);
}

//----- (000513E0) --------------------------------------------------------
type_entity_0x6E8E* AddVolcanoSpell_513E0(axis_3d* position)//2323e0
{
	return AddSpellPickup_51120(position, 18, 54);
}

//----- (00051400) --------------------------------------------------------
type_entity_0x6E8E* AddSummonArmySpell_51400(axis_3d* position)//232400
{
	return AddSpellPickup_51120(position, 19, 57);
}

//----- (00051420) --------------------------------------------------------
type_entity_0x6E8E* AddGravityWellSpell_51420(axis_3d* position)//232420
{
	return AddSpellPickup_51120(position, 20, 60);
}

//----- (00051440) --------------------------------------------------------
type_entity_0x6E8E* AddWhirlwindSpell_51440(axis_3d* position)//232440
{
	return AddSpellPickup_51120(position, 21, 63);
}

//----- (00051460) --------------------------------------------------------
type_entity_0x6E8E* AddFoolsManaSpell_51460(axis_3d* position)//232460
{
	return AddSpellPickup_51120(position, 22, 66);
}

//----- (00051480) --------------------------------------------------------
type_entity_0x6E8E* AddMagicMineSpell_51480(axis_3d* position)//232480
{
	return AddSpellPickup_51120(position, 23, 69);
}

//----- (000514A0) --------------------------------------------------------
type_entity_0x6E8E* AddAllianceSpell_514A0(axis_3d* position)//2324a0
{
	return AddSpellPickup_51120(position, 24, 72);
}

//----- (000514C0) --------------------------------------------------------
type_entity_0x6E8E* AddCaveInSpell_514C0(axis_3d* position)//2324c0
{
	return AddSpellPickup_51120(position, 25, 75);
}

int debugcounter_235a50 = 0;
//----- (00054A50) --------------------------------------------------------
void InitialiseSpells_54A50(int playerIndex2, int playerIndex)//235a50
{
	int tempPlayerIndex2;
	int result;
	bool setSpell;

	//Unset all spells
	for (int i = 0; i < 26; i++)
	{
		world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.selectedSubSpells_0x437_1079x.SpellIndex[i] = 0;
		world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.SpellsEnabled_0x333_819x.SpellEnabled[i] = 0;
		world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.pickedUpSpells_0x403_1027x.SpellIndex[i] = 0;
	}

	world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.SpellIndexLeft_0x451_1105 = -1;
	world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.SpellIndexRight_0x453_1107 = -1;
	if (gameState_D41A4.setting_byte1_22 & 0x10)
		tempPlayerIndex2 = 0;
	else
		tempPlayerIndex2 = playerIndex2;
	int index = 0;

	if (m_InputRecorder != nullptr && m_InputRecorder->m_IsPlaying &&
		m_InputRecorder->GetCurrentPlayer(gameState_D41A4.levelnumber_43w, playerIndex) != nullptr)
	{
		auto currentPlayer = m_InputRecorder->GetCurrentPlayer(gameState_D41A4.levelnumber_43w, playerIndex);

		for (int i = 0; i < 26; i++)
		{
			world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.SpellsEnabled_0x333_819x.SpellEnabled[i] = currentPlayer->SpellsEnabled[i];
			world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.SpellIndexes_0x39B_923x.SpellIndex[i] = currentPlayer->SpellIndexes[i];
			world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.SpellLevels_0x41D_1053z.SpellIndex[i] = currentPlayer->SpellLevels[i];
			world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.SpellExperience_0x263_611x.SpellExperience[i] = currentPlayer->SpellsExperience[i];
		}
	}

	for (int i = 0; i < 26; i++)
	{
		result = spellIndex_D94FF[i];
		if (world_D41A0.terrain_2FECE.WizardMapSettings_0x360D2[tempPlayerIndex2].spellLevels_0x360FBx[result] > 2u)
			world_D41A0.terrain_2FECE.WizardMapSettings_0x360D2[tempPlayerIndex2].spellLevels_0x360FBx[result] = 2;
		if (world_D41A0.players_0x2BDE[playerIndex].IsAiPlayer_0x009_2BE4_11239 == 1)
		{
			world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.SpellLevels_0x41D_1053z.SpellIndex[result] = world_D41A0.terrain_2FECE.WizardMapSettings_0x360D2[tempPlayerIndex2].spellLevels_0x360FBx[result];
		}
		else if (gameState_D41A4.setting_byte1_22 & 8 || gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
		{
			int tempByte0x360FB = world_D41A0.terrain_2FECE.WizardMapSettings_0x360D2[tempPlayerIndex2].spellLevels_0x360FBx[result];
			if (world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.SpellLevels_0x41D_1053z.SpellIndex[result] < tempByte0x360FB)
			{
				tempByte0x360FB = world_D41A0.terrain_2FECE.WizardMapSettings_0x360D2[tempPlayerIndex2].spellLevels_0x360FBx[result];
				if (tempByte0x360FB < 0)
					tempByte0x360FB = 0;
				if (tempByte0x360FB > 2)
					tempByte0x360FB = 2;
				world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.SpellLevels_0x41D_1053z.SpellIndex[result] = tempByte0x360FB;
				world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.SpellExperience_0x263_611x.SpellExperience[result] = SPELLS_BEGIN_BUFFER_str[result].subspell[tempByte0x360FB].xpos2_0x12 + 1;
			}
		}
		world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.SpellIndexes_0x39B_923x.SpellIndex[i] = -1;
		setSpell = false;
		if (world_D41A0.players_0x2BDE[playerIndex].IsAiPlayer_0x009_2BE4_11239 == 1)
		{
			world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.blockedSpells_0x3CF_975x.SpellIndex[result] = world_D41A0.terrain_2FECE.WizardMapSettings_0x360D2[tempPlayerIndex2].BlockedSpells_0x36115x[result];
			if (world_D41A0.terrain_2FECE.WizardMapSettings_0x360D2[tempPlayerIndex2].StartingSpells_0x360E1x[result])
			{
				if (world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.blockedSpells_0x3CF_975x.SpellIndex[result] == 0)
					setSpell = true;
			}
		}
		//Else If player is a user and spell is not blocked...
		else if (!world_D41A0.terrain_2FECE.WizardMapSettings_0x360D2[tempPlayerIndex2].BlockedSpells_0x36115x[result])
		{
			if (gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
			{
				if (world_D41A0.terrain_2FECE.WizardMapSettings_0x360D2[tempPlayerIndex2].StartingSpells_0x360E1x[result])
				{
					if (world_D41A0.terrain_2FECE.WizardMapSettings_0x360D2[tempPlayerIndex2].BlockedSpells_0x36115x[result] == 0)
						setSpell = true;
				}
			}
			//If not loading from level prompt and level is > 0
			else if ((!(gameState_D41A4.OptionsSettingFlag_24 & Setting::LEVEL_LOADED_FROM_ARG)) && gameState_D41A4.levelnumber_43w)
			{
				if (world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.availableSpells_0x3E9_1001x.SpellIndex[result])
					setSpell = true;
			}
			//Load from players save
			else if (world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.availableSpells_0x3E9_1001x.SpellIndex[result])
			{
				setSpell = true;
			}
			//Load spells from Map file
			else if (world_D41A0.terrain_2FECE.WizardMapSettings_0x360D2[tempPlayerIndex2].StartingSpells_0x360E1x[result])
			{
				if (!world_D41A0.terrain_2FECE.WizardMapSettings_0x360D2[tempPlayerIndex2].BlockedSpells_0x36115x[result])
				{
					setSpell = true;
				}
			}
		}
		if (setSpell)//adress 235cc3
		{
			world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.SpellsEnabled_0x333_819x.SpellEnabled[result] = 1;
			world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.availableSpells_0x3E9_1001x.SpellIndex[result] = 1;
			if (world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.SpellIndexLeft_0x451_1105 == -1)
			{
				world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.SpellIndexLeft_0x451_1105 = result;
			}
			else if (world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.SpellIndexRight_0x453_1107 == -1)
			{
				world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.SpellIndexRight_0x453_1107 = result;
			}
			result = index + 1;
			world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.SpellIndexes_0x39B_923x.SpellIndex[index] = index;
			index++;
		}
	}

	if (m_InputRecorder != nullptr && m_InputRecorder->m_IsRecording)
		m_InputRecorder->RecordPlayerSpells(gameState_D41A4.levelnumber_43w, playerIndex,
			world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.SpellsEnabled_0x333_819x.SpellEnabled,
			world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.SpellIndexes_0x39B_923x.SpellIndex,
			world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.SpellLevels_0x41D_1053z.SpellIndex,
			world_D41A0.players_0x2BDE[playerIndex].playerData_0x3E6_2BE4_12228.str_611.SpellExperience_0x263_611x.SpellExperience);
}

//----- (00057450) --------------------------------------------------------
char IsSpellCastable_57450(uint8_t a1)//238450
{
	if (a1 < 0x53u)
	{
		if (a1 < 0x25u)
		{
			if (a1)
				return 0;
		}
		else if (a1 > 0x26u)
		{
			if (a1 >= 0x2Cu)
			{
				if (a1 > 0x2Fu && a1 != 81)
					return 0;
				return 1;
			}
			return 0;
		}
		return 1;
	}
	if (a1 <= 0x53u)
		return 1;
	if (a1 < 0x6Du)
	{
		if (a1 >= 0x68u)
		{
			if (a1 > 0x69u)
				return 0;
			return 1;
		}
		return 0;
	}
	if (a1 <= 0x6Du)
		return 1;
	if (a1 >= 0x72u)
	{
		if (a1 > 0x72u && a1 != 116)
			return 0;
		return 1;
	}
	return 0;
}

//----- (000574A0) --------------------------------------------------------
void ResetLevelSpells_574A0()//2384a0
{
	if (!(gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE))
	{
		memset(world_D41A0.creatureStats_0x364D2.spawnCounts_0x364D6, 0, 104);
		type_str_611* tempStr611 = &Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240]->playerData_0xA4_164x->str_611;
		for (int index = 1; index < 0x4b0; index++)
		{
			if (world_D41A0.terrain_2FECE.entity_0x30311[index].type_0x30311 == 0x0F)
			{
				if (!tempStr611->SpellsEnabled_0x333_819x.SpellEnabled[world_D41A0.terrain_2FECE.entity_0x30311[index].subtype_0x30311] && !tempStr611->availableSpells_0x3E9_1001x.SpellIndex[world_D41A0.terrain_2FECE.entity_0x30311[index].subtype_0x30311])
					world_D41A0.creatureStats_0x364D2.spawnCounts_0x364D6[world_D41A0.terrain_2FECE.entity_0x30311[index].subtype_0x30311]++;
			}
		}
	}
}

//----- (00059C80) --------------------------------------------------------
int UpdateScroll_59C80(type_entity_0x6E8E* entity)//23ac80
{
	type_entity_0x6E8E* entity2;
	if (gameState_D41A4.setting_38545 & 4)
	{
		entity->flags_0xc_12_15.byte[0] |= 1u;
		DisableEntityDrawing04_57F10(entity);
	}
	else
	{
		entity->position_0x4C_76.z = GetTerrainAlt_10C40(&entity->position_0x4C_76);
		for (entity2 = gameState_D41A4.class3List_38519; entity2 > Entities_EA3E4[0]; entity2 = entity2->next_0)
		{
			if (!entity2->model_0x40_64 && entity2->life_0x8 >= 0)
			{
				if (EntitiesOverlap_106C0(entity2, entity))
				{
					int playerIndex = entity2->playerData_0xA4_164x->playerColorIndex_0x38_56;
					if (playerIndex == world_D41A0.LevelIndex_0xc)
					{
						int countXP;
						if (gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
							countXP = 50;
						else
							countXP = 4;
						UpdateExperience_6E090(&entity2->playerData_0xA4_164x->str_611, countXP);
						PrepareEventSound_6E450(playerIndex, -1, 63);
						if (gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
							GrantAllSpellsMultiplayer_6DBD0();
						else
							GrantSpellToPlayer_6DB50(0, 1);
					}
					DisableEntityDrawing04_57F10(entity);
				}
			}
		}
	}
	return 1;
}

//----- (00068D50) --------------------------------------------------------
bool CanSpellOwnerCast_68D50(type_entity_0x6E8E* locEvent1, type_entity_0x6E8E* locEvent2)//249d50
{
	if (locEvent2->mana_0x90_144 < 0)
		return false;
	if (locEvent2->life_0x8 < 0)
		return false;
	if (locEvent1->manaRegen_0x88_136)
	{
		if (!locEvent2->playerData_0xA4_164x->CastleEntityIndex_0x3A_58 || locEvent1->manaRegen_0x88_136 > Entities_EA3E4[locEvent2->playerData_0xA4_164x->CastleEntityIndex_0x3A_58]->mana_0x90_144)
			return false;
	}
	if (locEvent2->mana_0x90_144 >= locEvent1->maxMana_0x8C_140 && locEvent1->stage_0x2E_46 == locEvent1->maxStage_0x30_48)
		return true;
	if (locEvent1->stage_0x2E_46 == locEvent1->maxStage_0x30_48)
		return false;
	else
		return true;
	return false;
}

//----- (00068DE0) --------------------------------------------------------
signed int RechargeSpell_68DE0(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//249de0
{
	__int16 v2; // bx
	int v3; // ebx
	signed int result; // eax

	v2 = a1x->stage_0x2E_46;
	if (v2 == a1x->maxStage_0x30_48)
	{
		v3 = a2x->manaRegen_0x88_136;
		if (v3 >= 0)
			a2x->manaRegen_0x88_136 = -a1x->maxMana_0x8C_140;
		else
			a2x->manaRegen_0x88_136 = v3 - a1x->maxMana_0x8C_140;
		result = 1;
	}
	else
	{
		if (v2 && a2x->manaRegen_0x88_136 > 0)
			a2x->manaRegen_0x88_136 = 0;
		result = 0;
	}
	return result;
}

//----- (00068E50) --------------------------------------------------------
signed int LaunchSpellProjectile_68E50(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x, type_entity_0x6E8E* a3x)//249e50
{
	uint8_t v3; // cl
	unsigned __int16 v4; // cx
	//unsigned __int16 v5; // ax
	signed __int16 v6; // ax
	int i; // eax
	type_entity_0x6E8E* v8x; // esi
	signed int result; // eax
	unsigned __int16 v10; // cx
	//unsigned __int16 v11; // ax
	signed __int16 v12; // ax
	type_entity_0x6E8E* jx; // esi
	axis_3d v14x; // [esp+0h] [ebp-8h]
	//int v15; // [esp+4h] [ebp-4h]

	//fix
	result = 0;
	//fix

	v14x = a2x->position_0x4C_76;
	v3 = a1x->flags_0xc_12_15.byte[1];
	if (v3 & 1)
	{
		if (a3x->model_0x40_64 == 4 && (v4 = a3x->targetId_0x96_150) != 0)
		{
			v14x.z = Entities_EA3E4[v4]->params_0x52_82.yaw + Entities_EA3E4[v4]->position_0x4C_76.z;
		}
		else
		{
			//v5 = a1x->word_0x1C_28 - 512;
			//HIBYTE(v5) &= 7u;
			MoveEntity_57FA0(&v14x, (a1x->yaw_0x1C_28 - 512) & 0x7ffu, 0, 256);
		}
		v6 = GetTerrainAlt_10C40(&v14x);
		if (v6 > v14x.z)
		{
			v14x = a2x->position_0x4C_76;
		}
		for (i = a2x->nextLinkedId_0x34_52; ; i = v8x->nextLinkedId_0x34_52)
		{
			v8x = Entities_EA3E4[i];
			if (v8x == Entities_EA3E4[0])
				break;
			CopyEntityPosition_57CF0(v8x, &v14x);
		}
		/*result = */CopyEntityPosition_57CF0(a2x, &v14x);
	}
	else
	{
		if (v3 & 2)
		{
			if (a3x->model_0x40_64 == 4 && (v10 = a3x->targetId_0x96_150) != 0)
			{
				v14x.z = Entities_EA3E4[v10]->params_0x52_82.yaw + Entities_EA3E4[v10]->position_0x4C_76.z;
			}
			else
			{
				//v11 = a1x->word_0x1C_28;
				//HIBYTE(v11) = (HIBYTE(v11) + 2) & 7;
				MoveEntity_57FA0(&v14x, (a1x->yaw_0x1C_28 + 0x200) & 0x7ffu, 0, 256);
			}
			v12 = GetTerrainAlt_10C40(&v14x);
			if (v12 > v14x.z)
			{
				v14x = a2x->position_0x4C_76;
			}
			for (jx = Entities_EA3E4[a2x->nextLinkedId_0x34_52];
				jx != Entities_EA3E4[0];
				jx = Entities_EA3E4[jx->nextLinkedId_0x34_52])
			{
				CopyEntityPosition_57CF0(jx, &v14x);
			}
		}
		/*result = */CopyEntityPosition_57CF0(a2x, &v14x);
	}
	return result;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00068FF0) --------------------------------------------------------
signed int UpdateSpellPickupCommon_68FF0(type_entity_0x6E8E* a1x, char a2, char a3)//249f00
{
	int v3; // edx
	__int16 v4; // ax
	type_entity_0x6E8E* ix; // esi
	//int v6; // eax
	char v7; // al
	//uint8_t* v8; // ecx
	__int16 v9; // ax
	//int v10; // eax
	char v12; // [esp+0h] [ebp-4h]

	v3 = a1x->life_0x8;
	v12 = 0;
	if (v3 && (a1x->life_0x8 = v3 - 1, v3 == 1))
	{
		DisableEntityDrawing04_57F10(a1x);
	}
	else
	{
		v4 = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
		ClampAboveTerrain_580E0(&a1x->position_0x4C_76, v4, 0, 0x2000, -128);
		if (!(a1x->instanceIndex_0x3E_62 & 3))
		{
			for (ix = gameState_D41A4.class3List_38519; ix > Entities_EA3E4[0]; ix = ix->next_0)
			{
				if (!ix->model_0x40_64 && ix->life_0x8 >= 0)
				{
					//v6 = ix->dword_0xA4_164;
					if (ix->playerData_0xA4_164x->playerColorIndex_0x38_56 == world_D41A0.LevelIndex_0xc
						&& !(a1x->flags_0xc_12_15.byte[0] & 1)
						&& ix->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[a1x->model_0x40_64])
					{
						a1x->flags_0xc_12_15.byte[0] |= 1u;
					}
					if (EntitiesOverlap_106C0(ix, a1x))
					{
						v7 = 0;
						if (ix->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[a2])
							v7 = 1;
						if (!v7)
						{
							PrepareEventSound_6E450(ix - world_D41A0.entities_0x6E8E, -1, 18);
							//v8 = x_D41A0_BYTEARRAY_0;
							a1x->flags_0xc_12_15.byte[0] |= 1u;
							//v8 += 28302;
							a1x->flags_0xc_12_15.byte[3] &= 0xFDu;
							a1x->parentId_0x28_40 = ix - world_D41A0.entities_0x6E8E;
							a1x->actionIndex_0x45_69 = a3;
							a1x->param3_0x36_54 = 64;
							ix->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[a2] = a1x - world_D41A0.entities_0x6E8E;
							ix->playerData_0xA4_164x->str_611.pickedUpSpells_0x403_1027x.SpellIndex[a2] = 1;
							v9 = a1x->nextStage_0x4A_74;
							if (v9)
							{
								if (v9 == 2)
									v12 = 1;
								a1x->nextStage_0x4A_74 = 0;
							}
							else
							{
								//v10 = ix->dword_0xA4_164;
								if (ix->playerData_0xA4_164x->str_611.SpellIndexLeft_0x451_1105 == -1 || ix->playerData_0xA4_164x->str_611.SpellIndexRight_0x453_1107 != -1)
									v12 = 1;
							}
							if (v12)
							{
								ix->playerData_0xA4_164x->str_611.SpellIndexLeft_0x451_1105 = a1x->model_0x40_64;
								ix->playerData_0xA4_164x->str_611.SubSpellIndexLeft_1109 = ix->playerData_0xA4_164x->str_611.selectedSubSpells_0x437_1079x.SpellIndex[a1x->model_0x40_64];
							}
							else
							{
								ix->playerData_0xA4_164x->str_611.SpellIndexRight_0x453_1107 = a1x->model_0x40_64;
								ix->playerData_0xA4_164x->str_611.SubSpellIndexRight_1110 = ix->playerData_0xA4_164x->str_611.selectedSubSpells_0x437_1079x.SpellIndex[a1x->model_0x40_64];
							}
							SetSpell_6D5E0(a1x, ix->playerData_0xA4_164x->str_611.selectedSubSpells_0x437_1079x.SpellIndex[a1x->model_0x40_64]);
							return 1;
						}
					}
				}
			}
		}
	}
	return 0;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//type_str_0x6E8E* sub_2a881e(axis_3d*)[20] = { sub_511A0 };
//void (*functptr[])() = { test1, test2, test3 };
//----- (000692A0) --------------------------------------------------------
signed int UpdateSpellPickupFloating_692A0(type_entity_0x6E8E* a1x)//24a2a0
{
	return UpdateSpellPickupCommon_68FF0(a1x, a1x->model_0x40_64, a1x->actionIndex_0x45_69 - 1);
}

//----- (000692C0) --------------------------------------------------------
char ResetSpellToPickup_692C0(type_entity_0x6E8E* a1x)//24a2c0
{
	char result; // al
	char v2; // ah

	result = CheckParentAlive_59DC0(a1x);
	if (result)
	{
		result = 3 * a1x->model_0x40_64 + 1;
		a1x->actionIndex_0x45_69 = result;
		v2 = a1x->flags_0xc_12_15.byte[3];
		a1x->hitById_0x26_38 = 0;
		a1x->flags_0xc_12_15.byte[3] = v2 | 2;
	}
	return result;
}

//----- (00069300) --------------------------------------------------------
void SpellHitByEntity_69300(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//24a300
{
	char v2; // ah
	int v3; // eax
	//int v4; // edx
	//int v5; // edx
	type_entity_0x6E8E* v6x; // [esp+0h] [ebp-4h]

	if (!a1x->param3_0x36_54)
	{
		a1x->hitById_0x26_38 = a2x - world_D41A0.entities_0x6E8E;
		v2 = a1x->flags_0xc_12_15.byte[0];
		a1x->actionIndex_0x45_69 = 78;
		a1x->flags_0xc_12_15.byte[0] = v2 & 0xFE;
		v3 = a1x->parentId_0x28_40;
		a1x->param_0x10_16 = 0;
		v6x = Entities_EA3E4[v3];
		predictedAxis_EB398ar = v6x->position_0x4C_76;
		CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
		v6x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[a1x->model_0x40_64] = 0;
		a1x->nextStage_0x4A_74 = 0;
		//v4 = v6x->dword_0xA4_164;
		if (a1x->model_0x40_64 == v6x->playerData_0xA4_164x->str_611.SpellIndexRight_0x453_1107)
		{
			v6x->playerData_0xA4_164x->str_611.SpellIndexRight_0x453_1107 = -1;
			a1x->nextStage_0x4A_74 = 1;
		}
		//v5 = v6x->dword_0xA4_164;
		if (a1x->model_0x40_64 == v6x->playerData_0xA4_164x->str_611.SpellIndexLeft_0x451_1105)
		{
			v6x->playerData_0xA4_164x->str_611.SpellIndexLeft_0x451_1105 = -1;
			a1x->nextStage_0x4A_74 = 2;
		}
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;

//----- (000693F0) --------------------------------------------------------
void SpellFireballActive_693F0(type_entity_0x6E8E* a1x)//24a3f0//spell fire //tex 55
{
	type_entity_0x6E8E* v1x; // ebx
	type_entity_0x6E8E* v2x; // eax
	//__int16 v3; // si
	//uint8_t* result; // eax
	//__int16 v5; // dx
	type_entity_0x6E8E* v6x; // [esp+0h] [ebp-4h]

	if (a1x->stage_0x2E_46 > 0)
	{
		v1x = Entities_EA3E4[a1x->parentId_0x28_40];
		if (v1x > Entities_EA3E4[0])
		{
			if (CanSpellOwnerCast_68D50(a1x, v1x))
			{
				if (a1x->stage_0x2E_46 == a1x->maxStage_0x30_48)
				{
					v2x = CastSpell_6DCA0(
						v1x,
						&v1x->position_0x4C_76,
						0,
						//(int16_t*)(&(*xadataspellsdat.colorPalette_var28)[80 * a1x->byte_0x40_64 + 2 + 26 * a1x->byte_0x46_70]),
						&SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70],
						v1x->actSpeed_0x82_130,
						1);
					v6x = v2x;
					if (v2x)
					{
						LaunchSpellProjectile_68E50(v1x, v2x, a1x);
						v6x->hitById_0x26_38 = a1x - world_D41A0.entities_0x6E8E;
						v6x->id_0x1A_26 = v1x->id_0x1A_26;
						v6x->subSpellIndex_0x2A_42 = a1x->subSpellIndex_0x2A_42;
						v6x->mana_0x90_144 = a1x->mana_0x90_144;
						v6x->position_0x4C_76.z += v1x->params_0x52_82.fov;
						v6x->yaw_0x1C_28 = v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v1x->yaw_0x1C_28;
						v6x->pitch_0x1E_30 = v1x->playerData_0xA4_164x->entityIndex2_0x1A_26 + v1x->pitch_0x1E_30;
						v6x->param_0x10_16 = v1x->playerData_0xA4_164x->chargeTime_0x154_340;
						v1x->playerData_0xA4_164x->chargeTime_0x154_340 = 0;
						v6x->axis_0x9A_154x = v1x->position_0x4C_76;
						MoveEntity_57FA0(
							&v6x->axis_0x9A_154x,
							v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v1x->yaw_0x1C_28,
							v1x->playerData_0xA4_164x->entityIndex2_0x1A_26 + v1x->pitch_0x1E_30,
							0x4000);
						if (v1x->playerData_0xA4_164x->playerColorIndex_0x38_56 == world_D41A0.LevelIndex_0xc)
							SetEntityIndex_49C90(v6x, 42);
					}
				}
				RechargeSpell_68DE0(a1x, v1x);
			}
			else
			{
				a1x->stage_0x2E_46 = 1;
			}
		}
		//v3 = *(x_WORD *)(a1 + 46) - 1;
		a1x->stage_0x2E_46--;// = v3;
		if (!a1x->stage_0x2E_46)
			RestorePreviousSpell_6D880(a1x);
	}
	//result = a1;
	//v5 = *(x_WORD *)(a1 + 54);
	if (a1x->param3_0x36_54)
		a1x->param3_0x36_54--;// = v5 - 1;
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00069600) --------------------------------------------------------
signed int SpellFireballPickup_69600(type_entity_0x6E8E* a1)//24a600
{
	return UpdateSpellPickupFloating_692A0(a1);
}

//----- (00069620) --------------------------------------------------------
signed int SpellFireballPickupUpgrade_69620(type_entity_0x6E8E* a1)//24a620
{
	return UpdateSpellPickup_69250(a1);
}

//----- (00069640) --------------------------------------------------------
void SpellPossessionActive_69640(type_entity_0x6E8E* a1x)//24a640//spell posses
{
	type_entity_0x6E8E* v1x; // eax
	//uint8_t* v2; // edi
	//char v3; // dl
	type_entity_0x6E8E* v4x; // esi
	//char v5; // dl
	char v6; // ah
	char v7; // cl
	__int16 v8; // di
	//__int16 v9; // dx
	//uint8_t* v10; // [esp+4h] [ebp-4h]

	if (CommandLineParams.DoDebugSequences()) {
		//add_compare(0x24a644, CommandLineParams.DoDebugafterload(),8);
	}

	if (a1x->stage_0x2E_46 > 0)
	{
		v1x = Entities_EA3E4[a1x->parentId_0x28_40];
		//v10 = (uint8_t*)v1;
		if (v1x > Entities_EA3E4[0])
		{
			if (CanSpellOwnerCast_68D50(a1x, v1x))
			{
				if (a1x->stage_0x2E_46 == a1x->maxStage_0x30_48)
				{
					//v2 = (uint8_t*)/**(x_DWORD *)*/(&(*xadataspellsdat.colorPalette_var28)[80 * a1x->byte_0x40_64 + 2 + 26 * a1x->byte_0x46_70]);
					//SPELLS_BEGIN_BUFFER_str[a1x->byte_0x40_64].subspell[a1x->byte_0x46_70].dword_2;
					//v2= (uint8_t*)*(x_DWORD *)&(*xadataspellsdat.colorPalette_var28)[80 * a1[64] + 2 + 26 * a1[70]];
					//v3 = v2[24];
					if (SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].life_0x1A)
					{
						if (SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].life_0x1A <= 3u)
						{
							v4x = CreateEntity_4A190(&v1x->position_0x4C_76, 9, 17);
							if (v4x)
							{
								v4x->actSpeed_0x82_130 += v1x->actSpeed_0x82_130;
								LaunchSpellProjectile_68E50(v1x, v4x, a1x);
								v4x->hitById_0x26_38 = a1x - world_D41A0.entities_0x6E8E;
								//v5 = v2[24];
								if (SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].life_0x1A >= 1u)
								{
									if (SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].life_0x1A <= 1u)
									{
										v4x->levelPar2_0x44_68 = 54;
									}
									else if (SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].life_0x1A == 2)
									{
										v4x->levelPar2_0x44_68 = 69;
									}
								}
								v4x->levelPar1_0x43_67 = 10;
								v4x->id_0x1A_26 = v1x->id_0x1A_26;
								v4x->mana_0x90_144 = a1x->mana_0x90_144;
								v4x->position_0x4C_76.z += v1x->params_0x52_82.fov;
								v4x->yaw_0x1C_28 = v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v1x->yaw_0x1C_28;
								v4x->pitch_0x1E_30 = v1x->playerData_0xA4_164x->entityIndex2_0x1A_26 + v1x->pitch_0x1E_30;
								v4x->param_0x10_16 = (a1x->subSpellIndex_0x2A_42 << 8) * (a1x->subSpellIndex_0x2A_42 << 8);
								v1x->playerData_0xA4_164x->chargeTime_0x154_340 = 0;
								v4x->axis_0x9A_154x = v1x->position_0x4C_76;
								MoveEntity_57FA0(
									&v4x->axis_0x9A_154x,
									v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v1x->yaw_0x1C_28,
									v1x->playerData_0xA4_164x->entityIndex2_0x1A_26 + v1x->pitch_0x1E_30,
									0x4000);
								PrepareEventSound_6E450(v4x - world_D41A0.entities_0x6E8E, -1, 40);
								RechargeSpell_68DE0(a1x, v1x);
								goto LABEL_26;
							}
						}
					}
					else
					{
						CastPossessionProjectile_69900(a1x, v1x);
					}
				}
				else
				{
					v6 = a1x->counter_0x3C_60;
					if (v6)
					{
						if (a1x->levelPar3_0x46_70)
						{
							if (v6 == 1)
								CastPossessionProjectile_69900(a1x, v1x);
							v7 = a1x->counter_0x3C_60 + 1;
							a1x->counter_0x3C_60 = v7;
							if (v7 > 3)
							{
								a1x->counter_0x3C_60 = 0;
								RechargeSpell_68DE0(a1x, v1x);
								goto LABEL_26;
							}
						}
						else
						{
							a1x->counter_0x3C_60 = 0;
						}
					}
				}
				RechargeSpell_68DE0(a1x, v1x);
			}
			else
			{
				a1x->stage_0x2E_46 = 1;
			}
		}
	LABEL_26:
		v8 = a1x->stage_0x2E_46 - 1;
		a1x->stage_0x2E_46 = v8;
		if (!v8)
			RestorePreviousSpell_6D880(a1x);
	}
	//LABEL_28:
		//v9 = *(x_WORD *)(a1 + 54);
	if (a1x->param3_0x36_54)
		a1x->param3_0x36_54--;// = v9 - 1;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00069900) --------------------------------------------------------
void CastPossessionProjectile_69900(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//24a900
{
	type_entity_0x6E8E* v2x; // ebx
	//int v3; // edx
	//uint8_t* v4; // eax

	v2x = CreateEntity_4A190(&a2x->position_0x4C_76, 9, 1);
	if (v2x)
	{
		v2x->actSpeed_0x82_130 += a2x->actSpeed_0x82_130;
		LaunchSpellProjectile_68E50(a2x, v2x, a1x);
		//v3 = a1x - D41A0_BYTESTR_0.struct_0x6E8E);
		v2x->levelPar1_0x43_67 = 10;
		v2x->hitById_0x26_38 = a1x - world_D41A0.entities_0x6E8E;
		v2x->levelPar2_0x44_68 = 12;
		v2x->id_0x1A_26 = a2x->id_0x1A_26;
		v2x->position_0x4C_76.z += a2x->params_0x52_82.fov;
		v2x->mana_0x90_144 = a1x->mana_0x90_144;
		v2x->param_0x10_16 = 200;
		a2x->playerData_0xA4_164x->chargeTime_0x154_340 = 0;
		v2x->axis_0x9A_154x = a2x->position_0x4C_76;
		MoveEntity_57FA0(
			&v2x->axis_0x9A_154x,
			a2x->playerData_0xA4_164x->nextEntity_0x18_24 + a2x->yaw_0x1C_28,
			a2x->playerData_0xA4_164x->entityIndex2_0x1A_26 + a2x->pitch_0x1E_30,
			10240);
		v2x->yaw_0x1C_28 = a2x->playerData_0xA4_164x->nextEntity_0x18_24 + a2x->yaw_0x1C_28;
		//v4 = x_D41A0_BYTEARRAY_0 + 28302;
		v2x->pitch_0x1E_30 = a2x->playerData_0xA4_164x->entityIndex2_0x1A_26 + a2x->pitch_0x1E_30;
		PrepareEventSound_6E450(v2x - world_D41A0.entities_0x6E8E, -1, 40);
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (00069A70) --------------------------------------------------------
signed int SpellPossessionPickup_69A70(type_entity_0x6E8E* a1x)//24aa70
{
	return UpdateSpellPickupFloating_692A0(a1x);
}

//----- (00069A90) --------------------------------------------------------
signed int SpellPossessionPickupUpgrade_69A90(type_entity_0x6E8E* a1)//24aa90
{
	return UpdateSpellPickup_69250(a1);
}

//----- (00069AB0) --------------------------------------------------------
type_entity_0x6E8E* SpellCastleActive_69AB0(type_entity_0x6E8E* a1x)//24aab0
{
	type_entity_0x6E8E* v1x; // ebx
	unsigned __int16 v2; // si
	//char *v3; // edx
	int v4; // eax
	type_entity_0x6E8E* v5x; // eax
	//int v6; // ecx
	type_entity_0x6E8E* v7x; // edx
	__int16 v8; // ax
	type_entity_0x6E8E* resultx; // eax
	__int16 v10; // di
	type_entity_0x6E8E* v11x; // [esp+0h] [ebp-8h]
	type_entity_0x6E8E* v12x; // [esp+4h] [ebp-4h]

	if (a1x->stage_0x2E_46 <= 0)
	{
		RestorePreviousSpell_6D880(a1x);
	}
	else
	{
		v1x = Entities_EA3E4[a1x->parentId_0x28_40];
		if (v1x > Entities_EA3E4[0])
		{
			if (CanSpellOwnerCast_68D50(a1x, v1x))
			{
				if (a1x->stage_0x2E_46 == a1x->maxStage_0x30_48)
				{
					v2 = v1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58;
					//v3 = (char*)&(*xadataspellsdat.colorPalette_var28)[80 * a1x->byte_0x40_64 + 2 + 26 * a1x->byte_0x46_70];
					//SPELLS_BEGIN_BUFFER_str[a1x->byte_0x40_64].subspell[a1x->byte_0x46_70].dword_2
					v4 = 1;
					if (v2)
						v4 = Entities_EA3E4[v2]->param_0x10_16 + 1;
					v1x->playerData_0xA4_164x->castleSpells_0x24E_590.at(v4) = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].subSpellIndex_2;
					v1x->playerData_0xA4_164x->castleSpells_0x24E_590.at(v4 + 9) = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].life_0x1A;
					RechargeSpell_68DE0(a1x, v1x);
					v5x = CreateEntity_4A190(&v1x->position_0x4C_76, 9, 10);
					v12x = v5x;
					v11x = v5x;
					if (v5x)
					{
						a1x->stage_0x2E_46 = a1x->maxStage_0x30_48 - 1;
						v5x->actSpeed_0x82_130 += v1x->actSpeed_0x82_130;
						LaunchSpellProjectile_68E50(v1x, v5x, a1x);
						//v6 = (int)x_D41A0_BYTEARRAY_0 + 28302;
						v12x->hitById_0x26_38 = a1x - world_D41A0.entities_0x6E8E;
						v12x->subSpellIndex_0x2A_42 = a1x->subSpellIndex_0x2A_42;
						v12x->id_0x1A_26 = v1x->id_0x1A_26;
						v12x->position_0x4C_76.z += v1x->params_0x52_82.fov;
						v12x->mana_0x90_144 = a1x->mana_0x90_144;
						v7x = Entities_EA3E4[v1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58];
						if (v7x <= Entities_EA3E4[0])
						{
							v12x->axis_0x9A_154x = v1x->position_0x4C_76;
							MoveEntity_57FA0(&v12x->axis_0x9A_154x, v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v1x->yaw_0x1C_28, 0, 4096);
							v8 = GetTerrainAlt_10C40(&v12x->axis_0x9A_154x);
							v12x->levelPar1_0x43_67 = 3;
							v12x->levelPar2_0x44_68 = 2;
							v12x->axis_0x9A_154x.z = v8;
						}
						else
						{
							v12x->levelPar1_0x43_67 = 10;
							v12x->levelPar2_0x44_68 = 43;
							v12x->targetId_0x96_150 = v7x - world_D41A0.entities_0x6E8E;
						}
						v11x->param_0x10_16 = v1x->playerData_0xA4_164x->chargeTime_0x154_340;
						v1x->playerData_0xA4_164x->chargeTime_0x154_340 = 0;
						v11x->yaw_0x1C_28 = v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v1x->yaw_0x1C_28;
						v11x->pitch_0x1E_30 = v1x->playerData_0xA4_164x->entityIndex2_0x1A_26 + v1x->pitch_0x1E_30;
						PrepareEventSound_6E450(v11x - world_D41A0.entities_0x6E8E, -1, 15);
					}
				}
			}
			else
			{
				a1x->stage_0x2E_46 = 0;
			}
		}
	}
	resultx = a1x;
	v10 = a1x->param3_0x36_54;
	if (v10)
		a1x->param3_0x36_54 = v10 - 1;
	return resultx;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00069D70) --------------------------------------------------------
signed int SpellCastlePickup_69D70(type_entity_0x6E8E* a1)//24ad70
{
	return UpdateSpellPickupFloating_692A0(a1);
}

//----- (00069D90) --------------------------------------------------------
signed int SpellCastlePickupUpgrade_69D90(type_entity_0x6E8E* a1)//24ad90
{
	return UpdateSpellPickup_69250(a1);
}

//----- (00069DB0) --------------------------------------------------------
void GetScroll_69DB0(type_entity_0x6E8E* a1x)//24adb0
{
	type_entity_0x6E8E* v1x; // esi
	signed __int16 v2; // di
	char v3; // dl
	//int v4; // eax
	char v5; // cl
	type_entity_0x6E8E* v6x; // eax
	int v7; // ecx
	__int16 v8; // cx
	__int16 v9; // cx
	signed int v10; // [esp+0h] [ebp-8h]
	//char *v11; // [esp+4h] [ebp-4h]

	if (a1x->stage_0x2E_46 > 0)
	{
		v1x = Entities_EA3E4[a1x->parentId_0x28_40];
		if (v1x > Entities_EA3E4[0])
		{
			if (v1x->model_0x40_64 == 1)
				v10 = 64;
			else
				v10 = 2;
			if (v1x->playerData_0xA4_164x->speed_0xc_12 >= 0)
				v2 = 1;
			else
				v2 = -1;
			if (!CanSpellOwnerCast_68D50(a1x, v1x) || v1x->playerData_0xA4_164x->castLock_0xe_14)
			{
				if (v1x->playerData_0xA4_164x->castLock_0xe_14)
					a1x->stage_0x2E_46 = 1;
			}
			else
			{
				if (a1x->stage_0x2E_46 == a1x->maxStage_0x30_48)
				{
					v3 = a1x->flags_0xc_12_15.byte[0];
					if (v3 >= 0)
					{
						//v4 = (int)x_D41A0_BYTEARRAY_0 + 28302;
						a1x->flags_0xc_12_15.byte[0] = v3 | 0x80;
						PrepareEventSound_6E450(v1x - world_D41A0.entities_0x6E8E, -1, 19);
					}
				}
				if (a1x->stage_0x2E_46 == a1x->maxStage_0x30_48 - v10)
				{
					v5 = a1x->flags_0xc_12_15.byte[0];
					if (v5 < 0)
						a1x->flags_0xc_12_15.byte[0] = v5 & 0x7F;
				}
				//v11 = (char*)&(*xadataspellsdat.colorPalette_var28)[80 * a1x->byte_0x40_64 + 2 + 26 * a1x->byte_0x46_70];
				//SPELLS_BEGIN_BUFFER_str[a1x->byte_0x40_64].subspell[a1x->byte_0x46_70].dword_2
				if (a1x->stage_0x2E_46 == a1x->maxStage_0x30_48)
				{
					SendSpellMessage_6D8B0(a1x->parentId_0x28_40, 3u, 1);
					v1x->playerData_0xA4_164x->speed_0xc_12 = v2 * v1x->minSpeed_0x84_132 * (SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].subSpellIndex_2 + 1);
				}
				else
				{
					v1x->playerData_0xA4_164x->speed_0xc_12 = v2 * v1x->minSpeed_0x84_132 * SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].subSpellIndex_2;
				}
				v1x->actSpeed_0x82_130 = v1x->playerData_0xA4_164x->speed_0xc_12;
				if (!(a1x->instanceIndex_0x3E_62 & 3))
				{
					v6x = CreateEntity_4A190(&v1x->position_0x4C_76, 10, 2);
					if (v6x)
					{
						v7 = 4 * v6x->life_0x8;
						v6x->id_0x1A_26 = v1x->id_0x1A_26;
						v6x->life_0x8 = v7;
					}
				}
				RechargeSpell_68DE0(a1x, v1x);
			}
			v8 = a1x->stage_0x2E_46 - 1;
			a1x->stage_0x2E_46 = v8;
			if (!v8)
			{
				v1x->playerData_0xA4_164x->speed_0xc_12 = v1x->minSpeed_0x84_132 * v2;
				v1x->actSpeed_0x82_130 = v1x->playerData_0xA4_164x->speed_0xc_12;
				a1x->flags_0xc_12_15.byte[0] &= 0x7Fu;
				RestorePreviousSpell_6D880(a1x);
			}
		}
	}
	v9 = a1x->param3_0x36_54;
	if (v9)
		a1x->param3_0x36_54 = v9 - 1;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0006A010) --------------------------------------------------------
signed int SpellSpeedUpPickupUpgrade_6A010(type_entity_0x6E8E* a1)//24b010
{
	return UpdateSpellPickup_69250(a1);
}

//----- (0006A030) --------------------------------------------------------
void SpellMetamorphActive_6A030(type_entity_0x6E8E* a1x)//24b030
{
	unsigned __int16 v1; // ax
	type_entity_0x6E8E* v2x; // esi
	int v3; // edi
	type_entity_0x6E8E* v4x; // eax
	type_entity_0x6E8E* v5x; // ecx
	char v6; // al
	//uint8_t* v7; // ST0C_4
	//int v8; // eax
	type_entity_0x6E8E* v9x; // ecx
	int v10; // edx
	__int16 v11; // dx
	unsigned __int16 v12; // di
	//int v13; // eax
	__int16 v14; // si
	int v16; // [esp+4h] [ebp-8h]

	v1 = a1x->parentId_0x28_40;
	v2x = Entities_EA3E4[v1];
	v3 = -1;
	if (v2x > Entities_EA3E4[0] && a1x->stage_0x2E_46 > 0)
	{
		if (CanSpellOwnerCast_68D50(a1x, v2x))
		{
			if (a1x->stage_0x2E_46 == a1x->maxStage_0x30_48)
			{
				SendSpellMessage_6D8B0(a1x->parentId_0x28_40, 4u, 1);
				//v4x = sub_4A190(&v2x->array_0x4C_76, 5, (unsigned __int8)(*xadataspellsdat.colorPalette_var28)[80 * a1x->byte_0x40_64 + 26 + 26 * a1x->byte_0x46_70]);
				v4x = CreateEntity_4A190(&v2x->position_0x4C_76, 5, SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].life_0x1A);
				v5x = v4x;
				if (v4x)
				{
					v6 = 8 * v4x->model_0x40_64;
					v5x->StageVar2_0x49_73 = 12;
					v5x->actionIndex_0x45_69 = v6 + 7;
					v5x->parentId_0x28_40 = a1x->parentId_0x28_40;
					v5x->id_0x1A_26 = v2x->id_0x1A_26;
					//v7 = x_D41A0_BYTEARRAY_0;
					a1x->targetId_0x96_150 = v5x - world_D41A0.entities_0x6E8E;
					v5x->flags_0xc_12_15.byte[0] |= 1u;
					//v8 = v2x->dword_0xA4_164;
					v2x->flags_0xc_12_15.byte[0] |= 0x21u;
					if (v2x->playerData_0xA4_164x->playerColorIndex_0x38_56 != world_D41A0.LevelIndex_0xc)
					{
						v5x->flags_0xc_12_15.byte[0] &= 0xFEu;
						v2x->flags_0xc_12_15.byte[0] |= 1u;
					}
					PrepareEventSound_6E450(v2x - world_D41A0.entities_0x6E8E, -1, 60);
				}
			}
			RechargeSpell_68DE0(a1x, v2x);
		}
		else
		{
			a1x->stage_0x2E_46 = 1;
		}
		v1 = v2x->playerData_0xA4_164x->playerColorIndex_0x38_56;
		if (v1 != world_D41A0.LevelIndex_0xc)
		{
			v1 = a1x->targetId_0x96_150;
			v9x = Entities_EA3E4[v1];
			if (v9x > Entities_EA3E4[0])
			{
				v10 = a1x->maxStage_0x30_48;
				v16 = a1x->stage_0x2E_46;
				LOBYTE(v1) = a1x->stage_0x2E_46;
				if (v16 <= v10 - 7)
				{
					v1 = a1x->stage_0x2E_46;
					if ((signed __int16)v1 < 7)
					{
						LOBYTE(v1) = v1 & 1;
						v3 = (unsigned __int8)v1;
					}
				}
				else
				{
					v3 = ((x_BYTE)v10 - (x_BYTE)v16) & 1;
				}
				if (v3 != -1)
				{
					if (v3)
					{
						if (v3 == 1)
						{
							v9x->flags_0xc_12_15.byte[0] |= 1u;
							v2x->flags_0xc_12_15.byte[0] &= 0xFEu;
						}
					}
					else
					{
						v9x->flags_0xc_12_15.byte[0] &= 0xFEu;
						v2x->flags_0xc_12_15.byte[0] |= 1u;
					}
				}
			}
		}
		v11 = a1x->stage_0x2E_46 - 1;
		a1x->stage_0x2E_46 = v11;
		if (!v11)
		{
			v12 = a1x->targetId_0x96_150;
			if (v12)
			{
				DisableEntityDrawing04_57F10(Entities_EA3E4[v12]);
				a1x->targetId_0x96_150 = 0;
			}
			//v13 = v2x->dword_0xA4_164;
			v2x->flags_0xc_12_15.byte[0] &= 0xDFu;
			if (v2x->playerData_0xA4_164x->playerColorIndex_0x38_56 == world_D41A0.LevelIndex_0xc)
				PrepareEventSound_6E450(v2x - world_D41A0.entities_0x6E8E, -1, 60);
			else
				v2x->flags_0xc_12_15.byte[0] &= 0xFEu;
			/*LOBYTE(v1) = */RestorePreviousSpell_6D880(a1x);
		}
	}
	v14 = a1x->param3_0x36_54;
	if (v14)
		a1x->param3_0x36_54 = v14 - 1;
	//return v1;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0006A2C0) --------------------------------------------------------
signed int SpellMetamorphPickup_6A2C0(type_entity_0x6E8E* a1)//24b2c0
{
	return UpdateSpellPickupFloating_692A0(a1);
}

//----- (0006A2E0) --------------------------------------------------------
signed int SpellMetamorphPickupUpgrade_6A2E0(type_entity_0x6E8E* a1)//24b2e0
{
	return UpdateSpellPickup_69250(a1);
}

//----- (0006A300) --------------------------------------------------------
void SpellHealActive_6A300(type_entity_0x6E8E* a1x)//24b300
{
	type_entity_0x6E8E* v1x; // esi
	int v2; // edx
	int v3; // ecx
	int v4; // edi
	__int16 v5; // ax
	__int16 v6; // cx

	if (a1x->stage_0x2E_46 > 0)
	{
		v1x = Entities_EA3E4[a1x->parentId_0x28_40];
		if (v1x > Entities_EA3E4[0])
		{
			if (CanSpellOwnerCast_68D50(a1x, v1x) && v1x->mana_0x90_144 >= a1x->maxMana_0x8C_140)
			{
				if (a1x->stage_0x2E_46 == a1x->maxStage_0x30_48)
					PrepareEventSound_6E450(v1x - world_D41A0.entities_0x6E8E, -1, 25);
				if (v1x->life_0x8 < v1x->maxLife_0x4)
				{
					if (a1x->stage_0x2E_46 == a1x->maxStage_0x30_48)
						SendSpellMessage_6D8B0(a1x->parentId_0x28_40, 5u, 1);
					v2 = v1x->maxLife_0x4 * a1x->subSpellIndex_0x2A_42 / 100 + v1x->life_0x8;
					v3 = v1x->maxLife_0x4;
					v1x->life_0x8 = v2;
					if (v2 > v3)
						v1x->life_0x8 = v3;
					v4 = v1x->manaRegen_0x88_136;
					if (v4 >= 0)
						v1x->manaRegen_0x88_136 = -a1x->maxMana_0x8C_140;
					else
						v1x->manaRegen_0x88_136 = v4 - a1x->maxMana_0x8C_140;
				}
			}
			else
			{
				a1x->stage_0x2E_46 = 1;
			}
		}
		v5 = a1x->stage_0x2E_46 - 1;
		a1x->stage_0x2E_46 = v5;
		if (!v5)
			RestorePreviousSpell_6D880(a1x);
	}
	v6 = a1x->param3_0x36_54;
	if (v6)
		a1x->param3_0x36_54 = v6 - 1;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0006A440) --------------------------------------------------------
signed int SpellHealPickup_6A440(type_entity_0x6E8E* a1)//24b440
{
	return UpdateSpellPickupFloating_692A0(a1);
}

//----- (0006A460) --------------------------------------------------------
signed int SpellHealPickupUpgrade_6A460(type_entity_0x6E8E* a1)//24b460
{
	return UpdateSpellPickup_69250(a1);
}

//----- (0006A480) --------------------------------------------------------
void SpellShieldActive_6A480(type_entity_0x6E8E* a1x)//24b480
{
	signed int v1; // eax
	type_entity_0x6E8E* v2x; // esi
	__int16 v3; // cx

	//LOWORD(v1) = a1x->word_0x28_40;
	v2x = Entities_EA3E4[a1x->parentId_0x28_40];
	if (v2x <= Entities_EA3E4[0] || a1x->stage_0x2E_46 <= 0)
		goto LABEL_15;
	//LOBYTE(v1) = (*xadataspellsdat.colorPalette_var28)[80 * a1x->byte_0x40_64 + 26 + 26 * a1x->byte_0x46_70];
	LOBYTE(v1) = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].life_0x1A;
	if (!(x_BYTE)v1)
	{
		v1 = CanSpellOwnerCast_68D50(a1x, v2x);
		if (v1)
		{
			v2x->flags_0xc_12_15.byte[1] |= 0x40u;
			LOBYTE(v1) = RechargeSpell_68DE0(a1x, v2x);
			a1x->stage_0x2E_46--;
			goto LABEL_13;
		}
		goto LABEL_12;
	}
	if ((x_BYTE)v1 == 1)
	{
		v1 = CanSpellOwnerCast_68D50(a1x, v2x);
		if (v1)
		{
			if (a1x->stage_0x2E_46 == a1x->maxStage_0x30_48)
				v2x->flags_0xc_12_15.byte[2] |= 0x40u;
			a1x->stage_0x2E_46--;
			LOBYTE(v1) = RechargeSpell_68DE0(a1x, v2x);
			goto LABEL_13;
		}
	LABEL_12:
		a1x->stage_0x2E_46 = 0;
	}
LABEL_13:
	if (!a1x->stage_0x2E_46)
	{
		v2x->flags_0xc_12_15.dword &= 0xFFBFBFFF;
		/*LOBYTE(v1) = */RestorePreviousSpell_6D880(a1x);
	}
LABEL_15:
	v3 = a1x->param3_0x36_54;
	if (v3)
		a1x->param3_0x36_54 = v3 - 1;
	//return v1;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0006A580) --------------------------------------------------------
signed int SpellShieldPickup_6A580(type_entity_0x6E8E* a1)//24b580
{
	return UpdateSpellPickupFloating_692A0(a1);
}

//----- (0006A5A0) --------------------------------------------------------
signed int SpellShieldPickupUpgrade_6A5A0(type_entity_0x6E8E* a1)//24b5a0
{
	return UpdateSpellPickup_69250(a1);
}

//----- (0006A5C0) --------------------------------------------------------
void SpellLightningActive_6A5C0(type_entity_0x6E8E* a1x)//24b5c0
{
	type_entity_0x6E8E* v1x; // ebx
	//char *v2; // edx
	char v3; // al
	type_entity_0x6E8E* v4x; // eax
	__int16 v5; // ax
	type_entity_0x6E8E* v6x; // edx
	__int16 v7; // ax
	//uint8_t* v8; // ecx
	__int16 v9; // bx
	//uint8_t* result; // eax
	__int16 v11; // si
	type_entity_0x6E8E* v12x; // [esp+0h] [ebp-20h]
	type_entity_0x6E8E* v13x; // [esp+4h] [ebp-1Ch]
	//char *v14; // [esp+Ch] [ebp-14h]
	type_entity_0x6E8E* v15x; // [esp+10h] [ebp-10h]
	type_entity_0x6E8E* v16x; // [esp+14h] [ebp-Ch]
	__int16 v17; // [esp+18h] [ebp-8h]
	__int16 v18; // [esp+1Ch] [ebp-4h]

	if (CommandLineParams.DoDebugSequences()) {
		//add_compare(0x24b5c4, CommandLineParams.DoDebugafterload(),0xa4);
	}

	v12x = 0;
	v13x = 0;
	if (a1x->stage_0x2E_46 <= 0)
		goto LABEL_29;
	v1x = Entities_EA3E4[a1x->parentId_0x28_40];
	if (v1x > Entities_EA3E4[0])
	{
		if (CanSpellOwnerCast_68D50(a1x, v1x))
		{
			if (a1x->stage_0x2E_46 == a1x->maxStage_0x30_48)
			{
				//v2 = (char*)&(*xadataspellsdat.colorPalette_var28)[80 * a1x->byte_0x40_64 + 2 + 26 * a1x->byte_0x46_70];
				//v3 = v2[24];
				v3 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].life_0x1A;
				//v14 = (char*)&(*xadataspellsdat.colorPalette_var28)[80 * a1x->byte_0x40_64 + 2 + 26 * a1x->byte_0x46_70];
				//SPELLS_BEGIN_BUFFER_str[a1x->byte_0x40_64].subspell[a1x->byte_0x46_70].byte_0x1A;
				if (v3)
				{
					if ((unsigned __int8)v3 <= 2u)
					{
						v18 = 0;
						v17 = (v3 != 1) + 1;
						while (v18 < v17)
						{
							v16x = CastSpell_6DCA0(v1x, &v1x->position_0x4C_76, 7u, &SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70], v1x->actSpeed_0x82_130, 1);
							if (v16x)
							{
								LaunchSpellProjectile_68E50(v1x, v16x, a1x);
								v16x->hitById_0x26_38 = a1x - world_D41A0.entities_0x6E8E;
								v16x->subSpellIndex_0x2A_42 = a1x->subSpellIndex_0x2A_42;
								v16x->id_0x1A_26 = v1x->id_0x1A_26;
								v16x->position_0x4C_76.z += v1x->params_0x52_82.fov;
								v16x->mana_0x90_144 = a1x->mana_0x90_144;
								v16x->subSpellIndex_0x2A_42 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].subSpellIndex_2;
								v16x->param_0x10_16 = v1x->playerData_0xA4_164x->chargeTime_0x154_340;
								v1x->playerData_0xA4_164x->chargeTime_0x154_340 = 0;
								v16x->axis_0x9A_154x = v1x->position_0x4C_76;
								MoveEntity_57FA0(
									&v16x->axis_0x9A_154x,
									v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v1x->yaw_0x1C_28,
									v1x->playerData_0xA4_164x->entityIndex2_0x1A_26 + v1x->pitch_0x1E_30,
									0x4000);
								if (SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].life_0x1A == 2)
								{
									if (v18)
										v5 = v1x->yaw_0x1C_28 - 113;
									else
										v5 = v1x->yaw_0x1C_28 + 113;
									v6x = v16x;
									v7 = (v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v5) & 0x7FF;
								}
								else
								{
									v6x = v16x;
									v7 = v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v1x->yaw_0x1C_28;
								}
								v6x->yaw_0x1C_28 = v7;
								v16x->pitch_0x1E_30 = v1x->playerData_0xA4_164x->entityIndex2_0x1A_26 + v1x->pitch_0x1E_30;
								if (v18)
									v13x = v16x;
								else
									v12x = v16x;
							}
							v18++;
						}
						if (v13x)
						{
							//v8 = x_D41A0_BYTEARRAY_0 + 28302;
							v12x->nextLinkedId_0x34_52 = v13x - world_D41A0.entities_0x6E8E;
							v13x->nextLinkedId_0x34_52 = v12x - world_D41A0.entities_0x6E8E;
						}
					}
				}
				else
				{
					v4x = CastSpell_6DCA0(v1x, &v1x->position_0x4C_76, 7u, &SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70], v1x->actSpeed_0x82_130, 1);
					v15x = v4x;
					if (v4x)
					{
						LaunchSpellProjectile_68E50(v1x, v4x, a1x);
						v15x->hitById_0x26_38 = a1x - world_D41A0.entities_0x6E8E;
						v15x->id_0x1A_26 = v1x->id_0x1A_26;
						v15x->mana_0x90_144 = a1x->mana_0x90_144;
						v15x->position_0x4C_76.z += v1x->params_0x52_82.fov;
						v15x->yaw_0x1C_28 = v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v1x->yaw_0x1C_28;
						v15x->pitch_0x1E_30 = v1x->playerData_0xA4_164x->entityIndex2_0x1A_26 + v1x->pitch_0x1E_30;
						v15x->subSpellIndex_0x2A_42 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].subSpellIndex_2;
						v15x->param_0x10_16 = v1x->playerData_0xA4_164x->chargeTime_0x154_340;
						v1x->playerData_0xA4_164x->chargeTime_0x154_340 = 0;
						v15x->axis_0x9A_154x = v1x->position_0x4C_76;
						MoveEntity_57FA0(
							&v15x->axis_0x9A_154x,
							v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v1x->yaw_0x1C_28,
							v1x->playerData_0xA4_164x->entityIndex2_0x1A_26 + v1x->pitch_0x1E_30,
							0x4000);
						RechargeSpell_68DE0(a1x, v1x);
						goto LABEL_27;
					}
				}
			}
			RechargeSpell_68DE0(a1x, v1x);
		}
		else
		{
			a1x->stage_0x2E_46 = 1;
		}
	}
LABEL_27:
	v9 = a1x->stage_0x2E_46 - 1;
	a1x->stage_0x2E_46 = v9;
	if (!v9)
		RestorePreviousSpell_6D880(a1x);
LABEL_29:
	//result = a1;
	v11 = a1x->param3_0x36_54;
	if (v11)
		a1x->param3_0x36_54 = v11 - 1;
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0006A9C0) --------------------------------------------------------
signed int SpellLightningPickup_6A9C0(type_entity_0x6E8E* a1)//24b9c0
{
	return UpdateSpellPickupFloating_692A0(a1);
}

//----- (0006A9E0) --------------------------------------------------------
signed int SpellLightningPickupUpgrade_6A9E0(type_entity_0x6E8E* a1)//24b9e0
{
	return UpdateSpellPickup_69250(a1);
}

//----- (0006AA00) --------------------------------------------------------
__int16 SpellReboundActive_6AA00(type_entity_0x6E8E* a1x)//24ba00
{
	type_entity_0x6E8E* v1x; // esi
	//char *v2; // edi
	char v3; // al
	__int16 v4; // cx
	__int16 result; // ax

	v1x = Entities_EA3E4[a1x->parentId_0x28_40];
	if (v1x > Entities_EA3E4[0])
	{
		if (a1x->stage_0x2E_46 <= 0)
		{
			v1x->flags_0xc_12_15.word[0] &= 0x7FEFu;
		}
		else
		{
			//v2 = (char*)&(*xadataspellsdat.colorPalette_var28)[80 * a1x->byte_0x40_64 + 2 + 26 * a1x->byte_0x46_70];
			//SPELLS_BEGIN_BUFFER_str[a1x->byte_0x40_64].subspell[a1x->byte_0x46_70].byte_0x1A
			if (CanSpellOwnerCast_68D50(a1x, v1x))
			{
				v3 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].life_0x1A;
				if (v3)
				{
					if (v3 == 1)
						v1x->flags_0xc_12_15.byte[0] |= 0x10u;
				}
				else
				{
					v1x->flags_0xc_12_15.byte[1] |= 0x80u;
				}
				RechargeSpell_68DE0(a1x, v1x);
			}
			else
			{
				a1x->stage_0x2E_46 = 1;
			}
			v4 = a1x->stage_0x2E_46 - 1;
			a1x->stage_0x2E_46 = v4;
			if (!v4)
				RestorePreviousSpell_6D880(a1x);
		}
	}
	result = a1x->param3_0x36_54;
	if (result)
		a1x->param3_0x36_54 = result - 1;
	return result;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0006AAC0) --------------------------------------------------------
signed int SpellReboundPickup_6AAC0(type_entity_0x6E8E* a1)//24bac0
{
	return UpdateSpellPickupFloating_692A0(a1);
}

//----- (0006AAE0) --------------------------------------------------------
signed int SpellReboundPickupUpgrade_6AAE0(type_entity_0x6E8E* a1)//24bae0
{
	return UpdateSpellPickup_69250(a1);
}

//----- (0006AB00) --------------------------------------------------------
void SpellMeteorActive_6AB00(type_entity_0x6E8E* a1x)//24bb00
{
	type_entity_0x6E8E* v1x; // ebx
	//char *v2; // esi
	__int16 v3; // si
	//int result; // eax
	__int16 v5; // dx
	type_entity_0x6E8E* v6x; // [esp+0h] [ebp-4h]

	if (a1x->stage_0x2E_46 > 0)
	{
		v1x = Entities_EA3E4[a1x->parentId_0x28_40];
		if (v1x > Entities_EA3E4[0])
		{
			if (CanSpellOwnerCast_68D50(a1x, v1x))
			{
				if (a1x->stage_0x2E_46 == a1x->maxStage_0x30_48)
				{
					//v2 = (char*)&(*xadataspellsdat.colorPalette_var28)[80 * a1x->byte_0x40_64 + 2 + 26 * a1x->byte_0x46_70];
					//SPELLS_BEGIN_BUFFER_str[a1x->byte_0x40_64].subspell[a1x->byte_0x46_70].byte_0x1B
					v6x = CastSpell_6DCA0(v1x, &v1x->position_0x4C_76, 9u, &SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70], v1x->actSpeed_0x82_130, 1);
					if (v6x)
					{
						LaunchSpellProjectile_68E50(v1x, v6x, a1x);
						v6x->hitById_0x26_38 = a1x - world_D41A0.entities_0x6E8E;
						v6x->id_0x1A_26 = v1x->id_0x1A_26;
						v6x->position_0x4C_76.z += v1x->params_0x52_82.fov;
						v6x->mana_0x90_144 = a1x->mana_0x90_144;
						v6x->subSpellIndex_0x2A_42 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].subSpellIndex_2;
						v6x->levelPar3_0x46_70 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].life_0x1A;
						v6x->param_0x10_16 = v1x->playerData_0xA4_164x->chargeTime_0x154_340;
						v1x->playerData_0xA4_164x->chargeTime_0x154_340 = 0;
						v6x->axis_0x9A_154x = v1x->position_0x4C_76;
						MoveEntity_57FA0(
							&v6x->axis_0x9A_154x,
							v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v1x->yaw_0x1C_28,
							v1x->playerData_0xA4_164x->entityIndex2_0x1A_26 + v1x->pitch_0x1E_30,
							10240);
						v6x->yaw_0x1C_28 = v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v1x->yaw_0x1C_28;
						v6x->pitch_0x1E_30 = v1x->playerData_0xA4_164x->entityIndex2_0x1A_26 + v1x->pitch_0x1E_30;
					}
				}
				RechargeSpell_68DE0(a1x, v1x);
			}
			else
			{
				a1x->stage_0x2E_46 = 1;
			}
		}
		v3 = a1x->stage_0x2E_46 - 1;
		a1x->stage_0x2E_46 = v3;
		if (!v3)
			RestorePreviousSpell_6D880(a1x);
	}
	//result = a1;
	v5 = a1x->param3_0x36_54;
	if (v5)
		a1x->param3_0x36_54 = v5 - 1;
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0006AD00) --------------------------------------------------------
signed int SpellMeteorPickup_6AD00(type_entity_0x6E8E* a1)//24bd00
{
	return UpdateSpellPickupFloating_692A0(a1);
}

//----- (0006AD20) --------------------------------------------------------
signed int SpellMeteorPickupUpgrade_6AD20(type_entity_0x6E8E* a1)//24bd20
{
	return UpdateSpellPickup_69250(a1);
}

//----- (0006AD60) --------------------------------------------------------
void SpellTeleportActive_6AD60(type_entity_0x6E8E* a1x)//24bd60
{
	type_entity_0x6E8E* v1x; // eax
	//char *v2; // esi
	char v3; // al
	type_entity_0x6E8E* v4x; // eax
	type_entity_0x6E8E* v5x; // esi
	axis_3d* v6x; // esi
	unsigned __int16 v7; // ax
	type_entity_0x6E8E* v8x; // edx
	__int16 v9; // ax
	//axis_3d* v10x; // esi
	unsigned __int16 v11; // ax
	type_entity_0x6E8E* v12x; // esi
	bool v13; // cf
	bool v14; // zf
	int v15; // eax
	type_entity_0x6E8E* v16x; // eax
	axis_3d* v17x; // esi
	unsigned __int16 v18; // ax
	unsigned __int16 v19; // si
	__int16 v20; // ax
	//int v21; // esi
	int v22; // ecx
	__int16 v23; // cx
	__int16 v24; // di
	axis_3d v25x; // [esp+0h] [ebp-14h]
	//__int16 v26; // [esp+4h] [ebp-10h]
	int v27; // [esp+8h] [ebp-Ch]
	int v28; // [esp+Ch] [ebp-8h]
	type_entity_0x6E8E* v29x; // [esp+10h] [ebp-4h]

	//fix
	v16x = 0;
	//fix

	v28 = 1;
	if (a1x->stage_0x2E_46 > 0)
	{
		v1x = Entities_EA3E4[a1x->parentId_0x28_40];
		v29x = v1x;
		if (v1x > Entities_EA3E4[0])
		{
			if (CanSpellOwnerCast_68D50(a1x, v1x))
			{
				if (a1x->stage_0x2E_46 == a1x->maxStage_0x30_48)
				{
					//v2 = (char*)&(*xadataspellsdat.colorPalette_var28)[80 * a1x->byte_0x40_64 + 2 + 26 * a1x->byte_0x46_70];
					//SPELLS_BEGIN_BUFFER_str[a1x->byte_0x40_64].subspell[a1x->byte_0x46_70].byte_0x1A
					SendSpellMessage_6D8B0(a1x->parentId_0x28_40, 0xAu, 1);
					v3 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].life_0x1A;
					if ((unsigned __int8)v3 < 1u)
					{
						if (!v3)
						{
							v4x = v29x;
							a1x->targetId_0x96_150 = 0;
							v5x = Entities_EA3E4[v4x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58];
							if (v5x <= Entities_EA3E4[0])
							{
								v28 = 0;
							}
							else
							{
								v6x = &v5x->position_0x4C_76;
								v25x = *v6x;
								v7 = v29x->yaw_0x1C_28 - 204;
								HIBYTE(v7) &= 7u;
								MoveEntity_57FA0(&v25x, v7, 0, -448);
								CopyEntityPosition_57CF0(v29x, &v25x);
							}
						}
					}
					else if ((unsigned __int8)v3 <= 1u)
					{
						v8x = Entities_EA3E4[v29x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58];
						if (v8x <= Entities_EA3E4[0])
						{
							v28 = 0;
						}
						else
						{
							v9 = a1x->targetId_0x96_150;
							if (v9 && v9 == 1)
							{
								CopyEntityPosition_57CF0(v29x, &a1x->axis_0x9A_154x);
								a1x->targetId_0x96_150 = 0;
							}
							else
							{
								//v10x = v29x->array_0x4C_76;
								a1x->axis_0x9A_154x = v29x->position_0x4C_76;
								//*(x_WORD *)(a1 + 158) = *(x_WORD *)(v10 + 4);
								v25x = v8x->position_0x4C_76;
								//v25x[2] = *(x_WORD *)(v8 + 80);
								v11 = v29x->yaw_0x1C_28 - 204;
								HIBYTE(v11) &= 7u;
								MoveEntity_57FA0(&v25x, v11, 0, -448);
								CopyEntityPosition_57CF0(v29x, &v25x);
								a1x->targetId_0x96_150 = 1;
							}
						}
					}
					else if (v3 == 2)
					{
						v27 = 0;
						v28 = 0;
						while (v27 < 9 && !v28)
						{
							switch (a1x->targetId_0x96_150)
							{
							case 1:
								break;
							case 2:
							case 3:
							case 4:
							case 5:
							case 6:
							case 7:
							case 8:
							case 9:
								v15 = a1x->targetId_0x96_150 - 2;
								if (v15 != v29x->playerData_0xA4_164x->playerColorIndex_0x38_56 && v15 < world_D41A0.NumberOfPlayers_0xe)
								{
									v16x = Entities_EA3E4[world_D41A0.players_0x2BDE[v15].playerIndex_0x00a_2BE4_11240];
									if (v16x > Entities_EA3E4[0])
									{
										v12x = Entities_EA3E4[v16x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58];
										v13 = v12x < Entities_EA3E4[0];
										v14 = v12x == Entities_EA3E4[0];
										goto LABEL_26;
									}
								}
								break;
							default:
								v12x = Entities_EA3E4[v29x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58];
								v13 = v12x < Entities_EA3E4[0];
								v14 = v12x == Entities_EA3E4[0];
							LABEL_26:
								if (!((unsigned __int8)v13 | (unsigned __int8)v14))
								{
									v17x = &v12x->position_0x4C_76;
									v25x = *v17x;
									v18 = (v29x->yaw_0x1C_28 - 204) & 0x7ffu;
									//HIBYTE(v18) &= 7u;
									MoveEntity_57FA0(&v25x, v18, 0, -448);
									CopyEntityPosition_57CF0(v29x, &v25x);
									v28 = 1;
								}
								break;
							}
							v19 = a1x->targetId_0x96_150 + 1;
							a1x->targetId_0x96_150 = v19;
							if (v19 >= 9u)
								a1x->targetId_0x96_150 = 0;
							v27++;
						}
					}
					v20 = v28;
					if (!v28)
					{
						//v21 = (int)(v29 + 19);
						a1x->axis_0x9A_154x = v29x->position_0x4C_76;
						a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
						MoveEntity_57FA0(&a1x->axis_0x9A_154x, a1x->rand_0x14_20 & 0x7FF, v20, 0x4000);
						CopyEntityPosition_57CF0(v29x, &a1x->axis_0x9A_154x);
						a1x->targetId_0x96_150 = 0;
					}
					v22 = v28;
					v29x->playerData_0xA4_164x->speed_0xc_12 = 0;
					if (v22)
						PrepareEventSound_6E450(v29x - world_D41A0.entities_0x6E8E, -1, 22);
				}
				RechargeSpell_68DE0(a1x, v29x);
			}
			else
			{
				a1x->stage_0x2E_46 = 1;
			}
			v23 = a1x->stage_0x2E_46 - 1;
			a1x->stage_0x2E_46 = v23;
			if (!v23)
			{
				v29x->playerData_0xA4_164x->speed_0xc_12 = 0;
				RestorePreviousSpell_6D880(a1x);
			}
		}
	}
	v24 = a1x->param3_0x36_54;
	if (v24)
		a1x->param3_0x36_54 = v24 - 1;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0006B180) --------------------------------------------------------
signed int SpellTeleportPickup_6B180(type_entity_0x6E8E* a1)//24c180
{
	return UpdateSpellPickupFloating_692A0(a1);
}

//----- (0006B1A0) --------------------------------------------------------
signed int SpellTeleportPickupUpgrade_6B1A0(type_entity_0x6E8E* a1)//24c1a0
{
	return UpdateSpellPickup_69250(a1);
}

//----- (0006B1C0) --------------------------------------------------------
void SpellInvisibleActive_6B1C0(type_entity_0x6E8E* a1x)//24c1c0
{
	// The invisibility flag (byte[0] & 0x20), the flight words word_0x159 /
	// byte_0x1BF and the end-of-spell clear all belong to the CASTER v1x
	// (0x24C1C0: every one of them is addressed off esi, the
	// Entities[parentId] register; [esi+0Ch] / [esi+0A4h]). They used to be
	// written on the spell entity a1x, whose flag is never set, so the
	// `!(flag & 0x20)` upkeep test failed on the first tick and the spell
	// ended after one frame (mgcr replay, mc2l0-spells-galore t=10945).
	type_entity_0x6E8E* v1x; // esi
	//char *v2; // edx
	__int16 v3; // di
	//int v4; // eax
	__int16 v5; // dx

	if (a1x->stage_0x2E_46 > 0)
	{
		v1x = Entities_EA3E4[a1x->parentId_0x28_40];
		if (v1x > Entities_EA3E4[0])
		{
			if (CanSpellOwnerCast_68D50(a1x, v1x))
			{
				if (a1x->stage_0x2E_46 == a1x->maxStage_0x30_48)
				{
					SendSpellMessage_6D8B0(a1x->parentId_0x28_40, 0xBu, 1);
					//v2 = (char*)&(*xadataspellsdat.colorPalette_var28)[80 * a1x->byte_0x40_64 + 2 + 26 * a1x->byte_0x46_70];
					//SPELLS_BEGIN_BUFFER_str[a1x->byte_0x40_64].subspell[a1x->byte_0x46_70].byte_0x1A
					v1x->playerData_0xA4_164x->invulnerableTimer_0x159_345 = 0;
					v1x->playerData_0xA4_164x->invisibleTime_0x1BF_447 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].life_0x1A;
					v1x->flags_0xc_12_15.byte[0] |= 0x20u;
				}
				else if (!(v1x->flags_0xc_12_15.byte[0] & 0x20))
				{
					a1x->stage_0x2E_46 = 1;
				}
				RechargeSpell_68DE0(a1x, v1x);
			}
			else
			{
				a1x->stage_0x2E_46 = 1;
			}
		}
		v3 = a1x->stage_0x2E_46 - 1;
		a1x->stage_0x2E_46 = v3;
		if (!v3)
		{
			//v4 = a1x->dword_0xA4_164;
			v1x->flags_0xc_12_15.byte[0] &= 0xDFu;
			v1x->playerData_0xA4_164x->invisibleTime_0x1BF_447 = 0;
			RestorePreviousSpell_6D880(a1x);
		}
	}
	v5 = a1x->param3_0x36_54;
	if (v5)
		a1x->param3_0x36_54 = v5 - 1;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0006B2D0) --------------------------------------------------------
signed int SpellInvisiblePickup_6B2D0(type_entity_0x6E8E* a1)//24c2d0
{
	return UpdateSpellPickupFloating_692A0(a1);
}

//----- (0006B2F0) --------------------------------------------------------
signed int SpellInvisiblePickupUpgrade_6B2F0(type_entity_0x6E8E* a1)//24c2f0
{
	return UpdateSpellPickup_69250(a1);
}

//----- (0006B310) --------------------------------------------------------
__int16 SpellBeyondSightActive_6B310(type_entity_0x6E8E* a1x)//24c310
{
	unsigned __int16 v1; // ax
	type_entity_0x6E8E* v2x; // esi
	__int16 v3; // si
	//__int16 result; // ax

	if (a1x->stage_0x2E_46 > 0)
	{
		v1 = a1x->parentId_0x28_40;
		v2x = Entities_EA3E4[v1];
		if (v2x > Entities_EA3E4[0])
		{
			if (a1x->stage_0x2E_46 == a1x->maxStage_0x30_48)
				SendSpellMessage_6D8B0(v1, 0xCu, 1);
			if (CanSpellOwnerCast_68D50(a1x, v2x))
				RechargeSpell_68DE0(a1x, v2x);
			else
				a1x->stage_0x2E_46 = 1;
		}
		v3 = a1x->stage_0x2E_46 - 1;
		a1x->stage_0x2E_46 = v3;
		if (!v3)
			RestorePreviousSpell_6D880(a1x);
	}
	//result = a1x->word_0x36_54;
	if (a1x->param3_0x36_54)
		a1x->param3_0x36_54--;
	return a1x->param3_0x36_54;//fix -remove it ?
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0006B3A0) --------------------------------------------------------
signed int SpellBeyondSightPickup_6B3A0(type_entity_0x6E8E* a1)//24c3a0
{
	return UpdateSpellPickupFloating_692A0(a1);
}

//----- (0006B3C0) --------------------------------------------------------
signed int SpellBeyondSightPickupUpgrade_6B3C0(type_entity_0x6E8E* a1)//24c3c0
{
	return UpdateSpellPickup_69250(a1);
}

//----- (0006B3E0) --------------------------------------------------------
void SpellStealManaActive_6B3E0(type_entity_0x6E8E* a1x)//24c3e0
{
	type_entity_0x6E8E* v1x; // ebx
	type_entity_0x6E8E* v2x; // eax
	__int16 v3; // dx
	//int result; // eax
	__int16 v5; // cx
	type_entity_0x6E8E* v6x; // [esp+0h] [ebp-4h]

	if (a1x->stage_0x2E_46 > 0)
	{
		v1x = Entities_EA3E4[a1x->parentId_0x28_40];
		if (v1x > Entities_EA3E4[0])
		{
			if (CanSpellOwnerCast_68D50(a1x, v1x))
			{
				if (a1x->stage_0x2E_46 == a1x->maxStage_0x30_48)
				{
					v2x = CastSpell_6DCA0(
						v1x,
						&v1x->position_0x4C_76,
						0xDu,
						//(__int16 *)&(*xadataspellsdat.colorPalette_var28)[80 * a1x->byte_0x40_64 + 2 + 26 * a1x->byte_0x46_70],
						&SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70],
						v1x->actSpeed_0x82_130,
						1);
					v6x = v2x;
					if (v2x)
					{
						LaunchSpellProjectile_68E50(v1x, v2x, a1x);
						v6x->hitById_0x26_38 = a1x - world_D41A0.entities_0x6E8E;
						v6x->id_0x1A_26 = v1x->id_0x1A_26;
						v6x->position_0x4C_76.z += v1x->params_0x52_82.fov;
						v6x->mana_0x90_144 = a1x->mana_0x90_144;
						v6x->param_0x10_16 = v1x->playerData_0xA4_164x->chargeTime_0x154_340;
						v1x->playerData_0xA4_164x->chargeTime_0x154_340 = 0;
						v6x->levelPar3_0x46_70 = a1x->levelPar3_0x46_70;
						v6x->axis_0x9A_154x = v1x->position_0x4C_76;
						MoveEntity_57FA0(
							&v6x->axis_0x9A_154x,
							v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v1x->yaw_0x1C_28,
							v1x->playerData_0xA4_164x->entityIndex2_0x1A_26 + v1x->pitch_0x1E_30,
							0x4000);
						v6x->yaw_0x1C_28 = v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v1x->yaw_0x1C_28;
						v6x->pitch_0x1E_30 = v1x->playerData_0xA4_164x->entityIndex2_0x1A_26 + v1x->pitch_0x1E_30;
					}
				}
				RechargeSpell_68DE0(a1x, v1x);
			}
			else
			{
				a1x->stage_0x2E_46 = 1;
			}
		}
		v3 = a1x->stage_0x2E_46 - 1;
		a1x->stage_0x2E_46 = v3;
		if (!v3)
			RestorePreviousSpell_6D880(a1x);
	}
	//result = a1;
	v5 = a1x->param3_0x36_54;
	if (v5)
		a1x->param3_0x36_54 = v5 - 1;
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0006B5D0) --------------------------------------------------------
signed int SpellStealManaPickup_6B5D0(type_entity_0x6E8E* a1)//24c5d0
{
	return UpdateSpellPickupFloating_692A0(a1);
}

//----- (0006B5F0) --------------------------------------------------------
signed int SpellStealManaPickupUpgrade_6B5F0(type_entity_0x6E8E* a1)//24c5f0
{
	return UpdateSpellPickup_69250(a1);
}

//----- (0006B610) --------------------------------------------------------
__int16 SpellDuelActive_6B610(type_entity_0x6E8E* a1x)//24c610
{
	type_entity_0x6E8E* v1x; // eax
	type_entity_0x6E8E* v2x; // eax
	type_entity_0x6E8E* v3x; // edi
	//axis_3d* v3y; // edi
	__int16 v4; // ax
	__int16 v5; // si
	__int16 result; // ax
	type_entity_0x6E8E* v7x; // [esp+0h] [ebp-Ch]
	type_entity_0x6E8E* v8x; // [esp+4h] [ebp-8h]
	char v9; // [esp+8h] [ebp-4h]

	v9 = 0;
	if (a1x->stage_0x2E_46 > 0)
	{
		v1x = Entities_EA3E4[a1x->parentId_0x28_40];
		v8x = v1x;
		if (v1x > Entities_EA3E4[0])
		{
			if (!CanSpellOwnerCast_68D50(a1x, v1x))
				goto LABEL_19;
			if (a1x->stage_0x2E_46 <= a1x->maxStage_0x30_48 - 28 && !v8x->playerData_0xA4_164x->mountId_0x146_326)
				v9 = 1;
			if (v9)
			{
			LABEL_19:
				a1x->stage_0x2E_46 = 1;
			}
			else
			{
				if (a1x->stage_0x2E_46 == a1x->maxStage_0x30_48)
				{
					v2x = CreateEntity_4A190(&v8x->position_0x4C_76, 9, 7);
					v7x = v2x;
					if (v2x)
					{
						v3x = v2x;
						LaunchSpellProjectile_68E50(v8x, v2x, a1x);
						v3x->hitById_0x26_38 = a1x - world_D41A0.entities_0x6E8E;
						v4 = a1x->subSpellIndex_0x2A_42;
						v3x->levelPar1_0x43_67 = 10;
						v3x->subSpellIndex_0x2A_42 = v4;
						v3x->levelPar2_0x44_68 = 26;
						v3x->id_0x1A_26 = v8x->id_0x1A_26;
						v3x->position_0x4C_76.z += v8x->params_0x52_82.fov;
						v3x->mana_0x90_144 = a1x->mana_0x90_144;
						v3x->levelPar3_0x46_70 = a1x->levelPar3_0x46_70;
						v3x->axis_0x9A_154x = v8x->position_0x4C_76;
						MoveEntity_57FA0(
							&v7x->axis_0x9A_154x,
							v8x->playerData_0xA4_164x->nextEntity_0x18_24 + v8x->yaw_0x1C_28,
							v8x->playerData_0xA4_164x->entityIndex2_0x1A_26 + v8x->pitch_0x1E_30,
							10240);
						v7x->yaw_0x1C_28 = v8x->playerData_0xA4_164x->nextEntity_0x18_24 + v8x->yaw_0x1C_28;
						v7x->pitch_0x1E_30 = v8x->playerData_0xA4_164x->entityIndex2_0x1A_26 + v8x->pitch_0x1E_30;
						PrepareEventSound_6E450(v7x - world_D41A0.entities_0x6E8E, -1, 9);
					}
				}
				RechargeSpell_68DE0(a1x, v8x);
			}
		}
		v5 = a1x->stage_0x2E_46 - 1;
		a1x->stage_0x2E_46 = v5;
		if (!v5)
			RestorePreviousSpell_6D880(a1x);
	}
	result = a1x->param3_0x36_54;
	if (result)
		a1x->param3_0x36_54 = result - 1;
	return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0006B830) --------------------------------------------------------
signed int SpellDuelPickup_6B830(type_entity_0x6E8E* a1)//24c830
{
	return UpdateSpellPickupFloating_692A0(a1);
}

//----- (0006B850) --------------------------------------------------------
signed int SpellDuelPickupUpgrade_6B850(type_entity_0x6E8E* a1)//24c850
{
	return UpdateSpellPickup_69250(a1);
}

//----- (0006B870) --------------------------------------------------------
void SpellTremorActive_6B870(type_entity_0x6E8E* a1x)//24c870
{
	type_entity_0x6E8E* v1x; // ebx
	//char *v2; // esi
	__int16 v3; // si
	//int result; // eax
	__int16 v5; // dx
	type_entity_0x6E8E* v6x; // [esp+0h] [ebp-4h]

	if (a1x->stage_0x2E_46 > 0)
	{
		v1x = Entities_EA3E4[a1x->parentId_0x28_40];
		if (v1x > Entities_EA3E4[0])
		{
			if (CanSpellOwnerCast_68D50(a1x, v1x))
			{
				if (a1x->stage_0x2E_46 == a1x->maxStage_0x30_48)
				{
					//v2 = (char*)&(*xadataspellsdat.colorPalette_var28)[80 * a1x->byte_0x40_64 + 2 + 26 * a1x->byte_0x46_70];
					//SPELLS_BEGIN_BUFFER_str[a1x->byte_0x40_64].subspell[a1x->byte_0x46_70].byte_0x1A
					v6x = CastSpell_6DCA0(v1x, &v1x->position_0x4C_76, 0xFu, &SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70], v1x->actSpeed_0x82_130, 1);
					if (v6x)
					{
						LaunchSpellProjectile_68E50(v1x, v6x, a1x);
						v6x->hitById_0x26_38 = a1x - world_D41A0.entities_0x6E8E;
						v6x->id_0x1A_26 = v1x->id_0x1A_26;
						v6x->position_0x4C_76.z += v1x->params_0x52_82.fov;
						v6x->yaw_0x1C_28 = v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v1x->yaw_0x1C_28;
						v6x->pitch_0x1E_30 = v1x->playerData_0xA4_164x->entityIndex2_0x1A_26 + v1x->pitch_0x1E_30;
						v6x->mana_0x90_144 = a1x->mana_0x90_144;
						v6x->subSpellIndex_0x2A_42 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].subSpellIndex_2;
						v6x->levelPar3_0x46_70 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].life_0x1A;
						v6x->param_0x10_16 = v1x->playerData_0xA4_164x->chargeTime_0x154_340;
						v1x->playerData_0xA4_164x->chargeTime_0x154_340 = 0;
						v6x->axis_0x9A_154x = v1x->position_0x4C_76;
						MoveEntity_57FA0(&v6x->axis_0x9A_154x, v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v1x->yaw_0x1C_28, 0, 4096);
						v6x->axis_0x9A_154x.z = GetTerrainAlt_10C40(&v6x->axis_0x9A_154x);
					}
				}
				RechargeSpell_68DE0(a1x, v1x);
			}
			else
			{
				a1x->stage_0x2E_46 = 1;
			}
		}
		v3 = a1x->stage_0x2E_46 - 1;
		a1x->stage_0x2E_46 = v3;
		if (!v3)
			RestorePreviousSpell_6D880(a1x);
	}
	//result = a1;
	v5 = a1x->param3_0x36_54;
	if (v5)
		a1x->param3_0x36_54 = v5 - 1;
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0006BA70) --------------------------------------------------------
signed int SpellTremorPickup_6BA70(type_entity_0x6E8E* a1)//24ca70
{
	return UpdateSpellPickupFloating_692A0(a1);
}

//----- (0006BA90) --------------------------------------------------------
signed int SpellTremorPickupUpgrade_6BA90(type_entity_0x6E8E* a1)//24ca90
{
	return UpdateSpellPickup_69250(a1);
}

//----- (0006BAB0) --------------------------------------------------------
void SpellCraterActive_6BAB0(type_entity_0x6E8E* a1x)//24cab0
{
	type_entity_0x6E8E* v1x; // ebx
	//char *v2; // esi
	__int16 v3; // si
	//int result; // eax
	__int16 v5; // dx
	type_entity_0x6E8E* v6x; // [esp+0h] [ebp-4h]

	if (a1x->stage_0x2E_46 > 0)
	{
		v1x = Entities_EA3E4[a1x->parentId_0x28_40];
		if (v1x > Entities_EA3E4[0])
		{
			if (CanSpellOwnerCast_68D50(a1x, v1x))
			{
				if (a1x->stage_0x2E_46 == a1x->maxStage_0x30_48)
				{
					//v2 = (char*)&(*xadataspellsdat.colorPalette_var28)[80 * a1x->byte_0x40_64 + 2 + 26 * a1x->byte_0x46_70];
					//SPELLS_BEGIN_BUFFER_str[a1x->byte_0x40_64].subspell[a1x->byte_0x46_70].byte_0x1A
					v6x = CastSpell_6DCA0(v1x, &v1x->position_0x4C_76, 0x10u, &SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70], v1x->actSpeed_0x82_130, 1);
					if (v6x)
					{
						LaunchSpellProjectile_68E50(v1x, v6x, a1x);
						v6x->hitById_0x26_38 = a1x - world_D41A0.entities_0x6E8E;
						v6x->id_0x1A_26 = v1x->id_0x1A_26;
						v6x->position_0x4C_76.z += v1x->params_0x52_82.fov;
						v6x->yaw_0x1C_28 = v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v1x->yaw_0x1C_28;
						v6x->pitch_0x1E_30 = v1x->playerData_0xA4_164x->entityIndex2_0x1A_26 + v1x->pitch_0x1E_30;
						v6x->mana_0x90_144 = a1x->mana_0x90_144;
						v6x->subSpellIndex_0x2A_42 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].subSpellIndex_2;
						v6x->levelPar3_0x46_70 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].life_0x1A;
						v6x->param_0x10_16 = v1x->playerData_0xA4_164x->chargeTime_0x154_340;
						v1x->playerData_0xA4_164x->chargeTime_0x154_340 = 0;
						v6x->axis_0x9A_154x = v1x->position_0x4C_76;
						MoveEntity_57FA0(&v6x->axis_0x9A_154x, v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v1x->yaw_0x1C_28, 0, 4096);
						v6x->axis_0x9A_154x.z = GetTerrainAlt_10C40(&v6x->axis_0x9A_154x);
					}
				}
				RechargeSpell_68DE0(a1x, v1x);
			}
			else
			{
				a1x->stage_0x2E_46 = 1;
			}
		}
		v3 = a1x->stage_0x2E_46 - 1;
		a1x->stage_0x2E_46 = v3;
		if (!v3)
			RestorePreviousSpell_6D880(a1x);
	}
	//result = a1;
	v5 = a1x->param3_0x36_54;
	if (v5)
		a1x->param3_0x36_54 = v5 - 1;
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0006BCB0) --------------------------------------------------------
signed int SpellCraterPickup_6BCB0(type_entity_0x6E8E* a1)//24ccb0
{
	return UpdateSpellPickupFloating_692A0(a1);
}

//----- (0006BCD0) --------------------------------------------------------
signed int SpellCraterPickupUpgrade_6BCD0(type_entity_0x6E8E* a1)//24ccd0
{
	return UpdateSpellPickup_69250(a1);
}

//----- (0006BCF0) --------------------------------------------------------
void SpellEarthquakeActive_6BCF0(type_entity_0x6E8E* a1x)//24ccf0
{
	type_entity_0x6E8E* v1x; // ebx
	//char *v2; // esi
	__int16 v3; // si
	//int result; // eax
	__int16 v5; // dx
	type_entity_0x6E8E* v6x; // [esp+0h] [ebp-4h]

	if (a1x->stage_0x2E_46 > 0)
	{
		v1x = Entities_EA3E4[a1x->parentId_0x28_40];
		if (v1x > Entities_EA3E4[0])
		{
			if (CanSpellOwnerCast_68D50(a1x, v1x))
			{
				if (a1x->stage_0x2E_46 == a1x->maxStage_0x30_48)
				{
					//v2 = (char*)&(*xadataspellsdat.colorPalette_var28)[80 * a1x->byte_0x40_64 + 2 + 26 * a1x->byte_0x46_70];
					v6x = CastSpell_6DCA0(v1x, &v1x->position_0x4C_76, 0x11u, &SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70], v1x->actSpeed_0x82_130, 1);
					if (v6x)
					{
						LaunchSpellProjectile_68E50(v1x, v6x, a1x);
						v6x->hitById_0x26_38 = a1x - world_D41A0.entities_0x6E8E;
						v6x->id_0x1A_26 = v1x->id_0x1A_26;
						v6x->position_0x4C_76.z += v1x->params_0x52_82.fov;
						v6x->yaw_0x1C_28 = v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v1x->yaw_0x1C_28;
						v6x->pitch_0x1E_30 = v1x->playerData_0xA4_164x->entityIndex2_0x1A_26 + v1x->pitch_0x1E_30;
						v6x->mana_0x90_144 = a1x->mana_0x90_144;
						v6x->subSpellIndex_0x2A_42 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].subSpellIndex_2;
						v6x->levelPar3_0x46_70 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].life_0x1A;
						v6x->param_0x10_16 = v1x->playerData_0xA4_164x->chargeTime_0x154_340;
						v1x->playerData_0xA4_164x->chargeTime_0x154_340 = 0;
						v6x->axis_0x9A_154x = v1x->position_0x4C_76;
						MoveEntity_57FA0(&v6x->axis_0x9A_154x, v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v1x->yaw_0x1C_28, 0, 4096);
						v6x->axis_0x9A_154x.z = GetTerrainAlt_10C40(&v6x->axis_0x9A_154x);
					}
					RechargeSpell_68DE0(a1x, v1x);
				}
			}
			else
			{
				a1x->stage_0x2E_46 = 1;
			}
		}
		v3 = a1x->stage_0x2E_46 - 1;
		a1x->stage_0x2E_46 = v3;
		if (!v3)
			RestorePreviousSpell_6D880(a1x);
	}
	//result = a1;
	v5 = a1x->param3_0x36_54;
	if (v5)
		a1x->param3_0x36_54 = v5 - 1;
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0006BEF0) --------------------------------------------------------
signed int SpellEarthquakePickup_6BEF0(type_entity_0x6E8E* a1)//24cef0
{
	return UpdateSpellPickupFloating_692A0(a1);
}

//----- (0006BF10) --------------------------------------------------------
signed int SpellEarthquakePickupUpgrade_6BF10(type_entity_0x6E8E* a1)//24cf10
{
	return UpdateSpellPickup_69250(a1);
}

//----- (0006BF30) --------------------------------------------------------
void SpellVolcanoActive_6BF30(type_entity_0x6E8E* a1x)//24cf30
{
	type_entity_0x6E8E* v1x; // ebx
	//char *v2; // esi
	__int16 v3; // dx
	//int result; // eax
	__int16 v5; // cx
	type_entity_0x6E8E* v6x; // [esp+0h] [ebp-4h]

	if (a1x->stage_0x2E_46 > 0)
	{
		v1x = Entities_EA3E4[a1x->parentId_0x28_40];
		if (v1x > Entities_EA3E4[0])
		{
			if (CanSpellOwnerCast_68D50(a1x, v1x))
			{
				if (a1x->stage_0x2E_46 == a1x->maxStage_0x30_48)
				{
					//v2 = (char*)&(*xadataspellsdat.colorPalette_var28)[80 * a1x->byte_0x40_64 + 2 + 26 * a1x->byte_0x46_70];
					v6x = CastSpell_6DCA0(v1x, &v1x->position_0x4C_76, 0x12u, &SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70], v1x->actSpeed_0x82_130, 1);
					if (v6x)
					{
						LaunchSpellProjectile_68E50(v1x, v6x, a1x);
						v6x->hitById_0x26_38 = a1x - world_D41A0.entities_0x6E8E;
						v6x->id_0x1A_26 = v1x->id_0x1A_26;
						v6x->position_0x4C_76.z += v1x->params_0x52_82.fov;
						v6x->mana_0x90_144 = a1x->mana_0x90_144;
						v6x->subSpellIndex_0x2A_42 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].subSpellIndex_2;
						v6x->levelPar3_0x46_70 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].life_0x1A;
						v6x->param_0x10_16 = v1x->playerData_0xA4_164x->chargeTime_0x154_340;
						v1x->playerData_0xA4_164x->chargeTime_0x154_340 = 0;
						v6x->axis_0x9A_154x = v1x->position_0x4C_76;
						MoveEntity_57FA0(&v6x->axis_0x9A_154x, v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v1x->yaw_0x1C_28, 0, 4096);
						v6x->axis_0x9A_154x.z = GetTerrainAlt_10C40(&v6x->axis_0x9A_154x);
						v6x->yaw_0x1C_28 = v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v1x->yaw_0x1C_28;
						v6x->pitch_0x1E_30 = v1x->playerData_0xA4_164x->entityIndex2_0x1A_26 + v1x->pitch_0x1E_30;
					}
					RechargeSpell_68DE0(a1x, v1x);
				}
			}
			else
			{
				a1x->stage_0x2E_46 = 1;
			}
		}
		v3 = a1x->stage_0x2E_46 - 1;
		a1x->stage_0x2E_46 = v3;
		if (!v3)
			RestorePreviousSpell_6D880(a1x);
	}
	//result = a1;
	v5 = a1x->param3_0x36_54;
	if (v5)
		a1x->param3_0x36_54 = v5 - 1;
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0006C130) --------------------------------------------------------
signed int SpellVolcanoPickup_6C130(type_entity_0x6E8E* a1)//24d130
{
	return UpdateSpellPickupFloating_692A0(a1);
}

//----- (0006C150) --------------------------------------------------------
signed int SpellVolcanoPickupUpgrade_6C150(type_entity_0x6E8E* a1)//24d150
{
	return UpdateSpellPickup_69250(a1);
}

//----- (0006C170) --------------------------------------------------------
void SpellSummonArmyActive_6C170(type_entity_0x6E8E* a1x)//24d170
{
	type_entity_0x6E8E* v1x; // eax
	//char *v2; // esi
	__int16 v3; // ax
	__int16 v4; // si
	//__int16 result; // ax
	type_entity_0x6E8E* v6x; // [esp+0h] [ebp-8h]
	type_entity_0x6E8E* v7x; // [esp+4h] [ebp-4h]

	if (a1x->stage_0x2E_46 > 0)
	{
		v1x = Entities_EA3E4[a1x->parentId_0x28_40];
		v7x = v1x;
		if (v1x > Entities_EA3E4[0])
		{
			if (CanSpellOwnerCast_68D50(a1x, v1x))
			{
				if (a1x->stage_0x2E_46 == a1x->maxStage_0x30_48)
				{
					//v2 = (char*)&(*xadataspellsdat.colorPalette_var28)[80 * a1x->byte_0x40_64 + 2 + 26 * a1x->byte_0x46_70];
					v6x = CreateEntity_4A190(&v7x->position_0x4C_76, 9, 24);
					if (v6x)
					{
						v6x->actSpeed_0x82_130 += v7x->actSpeed_0x82_130;
						LaunchSpellProjectile_68E50(v7x, v6x, a1x);
						v3 = a1x - world_D41A0.entities_0x6E8E;
						v6x->levelPar1_0x43_67 = 10;
						v6x->hitById_0x26_38 = v3;
						v6x->levelPar2_0x44_68 = 72;
						v6x->id_0x1A_26 = v7x->id_0x1A_26;
						v6x->parentId_0x28_40 = a1x->parentId_0x28_40;
						v6x->levelPar3_0x46_70 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].life_0x1A;
						v6x->param_0x10_16 = v7x->playerData_0xA4_164x->chargeTime_0x154_340;
						v7x->playerData_0xA4_164x->chargeTime_0x154_340 = 0;
						v6x->axis_0x9A_154x = v7x->position_0x4C_76;
						MoveEntity_57FA0(
							&v6x->axis_0x9A_154x,
							v7x->playerData_0xA4_164x->nextEntity_0x18_24 + v7x->yaw_0x1C_28,
							v7x->playerData_0xA4_164x->entityIndex2_0x1A_26 + v7x->pitch_0x1E_30,
							0x4000);
						v6x->yaw_0x1C_28 = v7x->playerData_0xA4_164x->nextEntity_0x18_24 + v7x->yaw_0x1C_28;
						v6x->pitch_0x1E_30 = v7x->playerData_0xA4_164x->entityIndex2_0x1A_26 + v7x->pitch_0x1E_30;
						PrepareEventSound_6E450(v6x - world_D41A0.entities_0x6E8E, -1, 9);
					}
				}
				RechargeSpell_68DE0(a1x, v7x);
			}
			else
			{
				a1x->stage_0x2E_46 = 1;
			}
		}
		v4 = a1x->stage_0x2E_46 - 1;
		a1x->stage_0x2E_46 = v4;
		if (!v4)
			RestorePreviousSpell_6D880(a1x);
	}
	//result = *(x_WORD *)(a1 + 54);
	if (a1x->param3_0x36_54)
		a1x->param3_0x36_54 = a1x->param3_0x36_54 - 1;
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0006C3A0) --------------------------------------------------------
signed int SpellSummonArmyPickup_6C3A0(type_entity_0x6E8E* a1)//24d3a0
{
	return UpdateSpellPickupFloating_692A0(a1);
}

//----- (0006C3C0) --------------------------------------------------------
signed int SpellSummonArmyPickupUpgrade_6C3C0(type_entity_0x6E8E* a1)//24d3c0
{
	return UpdateSpellPickup_69250(a1);
}

//----- (0006C3E0) --------------------------------------------------------
void SpellGravityWellActive_6C3E0(type_entity_0x6E8E* a1x)//24d3e0
{
	type_entity_0x6E8E* v1x; // ebx
	//char *v2; // esi
	__int16 v3; // si
	//int result; // eax
	__int16 v5; // dx
	type_entity_0x6E8E* v6x; // [esp+0h] [ebp-4h]

	if (a1x->stage_0x2E_46 > 0)
	{
		v1x = Entities_EA3E4[a1x->parentId_0x28_40];
		if (v1x > Entities_EA3E4[0])
		{
			if (CanSpellOwnerCast_68D50(a1x, v1x))
			{
				if (a1x->stage_0x2E_46 == a1x->maxStage_0x30_48)
				{
					//v2 = (char*)&(*xadataspellsdat.colorPalette_var28)[80 * a1x->byte_0x40_64 + 2 + 26 * a1x->byte_0x46_70];
					v6x = CastSpell_6DCA0(v1x, &v1x->position_0x4C_76, 0x14u, &SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70], v1x->actSpeed_0x82_130, 1);
					if (v6x)
					{
						LaunchSpellProjectile_68E50(v1x, v6x, a1x);
						v6x->hitById_0x26_38 = a1x - world_D41A0.entities_0x6E8E;
						v6x->id_0x1A_26 = v1x->id_0x1A_26;
						v6x->position_0x4C_76.z += v1x->params_0x52_82.fov;
						v6x->yaw_0x1C_28 = v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v1x->yaw_0x1C_28;
						v6x->pitch_0x1E_30 = v1x->playerData_0xA4_164x->entityIndex2_0x1A_26 + v1x->pitch_0x1E_30;
						v6x->mana_0x90_144 = a1x->mana_0x90_144;
						v6x->subSpellIndex_0x2A_42 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].subSpellIndex_2;
						v6x->levelPar3_0x46_70 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].life_0x1A;
						v6x->param_0x10_16 = v1x->playerData_0xA4_164x->chargeTime_0x154_340;
						v1x->playerData_0xA4_164x->chargeTime_0x154_340 = 0;
						v6x->axis_0x9A_154x = v1x->position_0x4C_76;
						MoveEntity_57FA0(&v6x->axis_0x9A_154x, v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v1x->yaw_0x1C_28, 0, 4096);
						v6x->axis_0x9A_154x.z = GetTerrainAlt_10C40(&v6x->axis_0x9A_154x);
					}
				}
				RechargeSpell_68DE0(a1x, v1x);
			}
			else
			{
				a1x->stage_0x2E_46 = 1;
			}
		}
		v3 = a1x->stage_0x2E_46 - 1;
		a1x->stage_0x2E_46 = v3;
		if (!v3)
			RestorePreviousSpell_6D880(a1x);
	}
	//result = a1;
	v5 = a1x->param3_0x36_54;
	if (v5)
		a1x->param3_0x36_54 = v5 - 1;
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0006C5E0) --------------------------------------------------------
signed int SpellGravityWellPickup_6C5E0(type_entity_0x6E8E* a1)//24d5e0
{
	return UpdateSpellPickupFloating_692A0(a1);
}

//----- (0006C600) --------------------------------------------------------
signed int SpellGravityWellPickupUpgrade_6C600(type_entity_0x6E8E* a1)//24d600
{
	return UpdateSpellPickup_69250(a1);
}

//----- (0006C620) --------------------------------------------------------
void SpellWhirlwindActive_6C620(type_entity_0x6E8E* a1x)//24d620
{
	type_entity_0x6E8E* v1x; // ebx
	//char *v2; // edi
	type_entity_0x6E8E* v3x; // esi
	unsigned __int8 v4; // ah
	int v5; // eax
	__int16 v6; // dx
	//int result; // eax
	__int16 v8; // cx

	if (a1x->stage_0x2E_46 > 0)
	{
		v1x = Entities_EA3E4[a1x->parentId_0x28_40];
		if (v1x > Entities_EA3E4[0])
		{
			if (CanSpellOwnerCast_68D50(a1x, v1x))
			{
				if (a1x->stage_0x2E_46 == a1x->maxStage_0x30_48)
				{
					//v2 = (char*)&(*xadataspellsdat.colorPalette_var28)[80 * a1x->byte_0x40_64 + 2 + 26 * a1x->byte_0x46_70];
					v3x = CastSpell_6DCA0(v1x, &v1x->position_0x4C_76, 0x15u, &SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70], v1x->actSpeed_0x82_130, 1);
					if (v3x)
					{
						LaunchSpellProjectile_68E50(v1x, v3x, a1x);
						v3x->hitById_0x26_38 = a1x - world_D41A0.entities_0x6E8E;
						v3x->id_0x1A_26 = v1x->id_0x1A_26;
						v3x->position_0x4C_76.z += v1x->params_0x52_82.fov;
						v3x->mana_0x90_144 = a1x->mana_0x90_144;
						v4 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].life_0x1A;
						if (v4)
							v5 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].subSpellIndex_2 / (signed int)v4;
						else
							LOWORD(v5) = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].subSpellIndex_2;
						v3x->subSpellIndex_0x2A_42 = v5;
						v3x->levelPar3_0x46_70 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].life_0x1A;
						v3x->param_0x10_16 = v1x->playerData_0xA4_164x->chargeTime_0x154_340;
						v1x->playerData_0xA4_164x->chargeTime_0x154_340 = 0;
						v3x->axis_0x9A_154x = v1x->position_0x4C_76;
						MoveEntity_57FA0(&v3x->axis_0x9A_154x, v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v1x->yaw_0x1C_28, 0, 4096);
						v3x->axis_0x9A_154x.z = GetTerrainAlt_10C40(&v3x->axis_0x9A_154x);
						v3x->yaw_0x1C_28 = v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v1x->yaw_0x1C_28;
						v3x->pitch_0x1E_30 = v1x->playerData_0xA4_164x->entityIndex2_0x1A_26 + v1x->pitch_0x1E_30;
					}
					RechargeSpell_68DE0(a1x, v1x);
				}
			}
			else
			{
				a1x->stage_0x2E_46 = 1;
			}
		}
		v6 = a1x->stage_0x2E_46 - 1;
		a1x->stage_0x2E_46 = v6;
		if (!v6)
			RestorePreviousSpell_6D880(a1x);
	}
	//result = a1;
	v8 = a1x->param3_0x36_54;
	if (v8)
		a1x->param3_0x36_54 = v8 - 1;
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0006C830) --------------------------------------------------------
signed int SpellWhirlwindPickup_6C830(type_entity_0x6E8E* a1)//24d830
{
	return UpdateSpellPickupFloating_692A0(a1);
}

//----- (0006C850) --------------------------------------------------------
signed int SpellWhirlwindPickupUpgrade_6C850(type_entity_0x6E8E* a1)//24d850
{
	return UpdateSpellPickup_69250(a1);
}

//----- (0006C870) --------------------------------------------------------
void SpellFoolsManaActive_6C870(type_entity_0x6E8E* a1x)//24d870
{
	type_entity_0x6E8E* v1x; // esi
	type_entity_0x6E8E* v2x; // edi
	int v3; // eax
	__int16 v4; // si
	//__int16 result; // ax
	signed int v6; // [esp+0h] [ebp-8h]
	//char *v7; // [esp+4h] [ebp-4h]

	v1x = 0;
	if (a1x->stage_0x2E_46 > 0)
	{
		v2x = Entities_EA3E4[a1x->parentId_0x28_40];
		if (v2x > Entities_EA3E4[0])
		{
			if (CanSpellOwnerCast_68D50(a1x, v2x))
			{
				if (a1x->stage_0x2E_46 == a1x->maxStage_0x30_48)
				{
					if (GetFreeEntityCount_4A810() > 6)
					{
						v6 = 0;
						//v7 = (char*)&(*xadataspellsdat.colorPalette_var28)[80 * a1x->byte_0x40_64 + 2 + 26 * a1x->byte_0x46_70];
						while (v6 < 6)
						{
							v1x = CreateEntity_4A190(&v2x->position_0x4C_76, 10, 57);
							if (v1x)
							{
								v3 = 4 * v2x->actSpeed_0x82_130;
								if (v3 < 140)
									v3 = 140;
								if (v3 > 280)
									LOWORD(v3) = 280;
								a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
								v1x->actSpeed_0x82_130 = (a1x->rand_0x14_20 & 0x7F) + v3;
								LaunchSpellProjectile_68E50(v2x, v1x, a1x);
								v1x->parentId_0x28_40 = v2x->id_0x1A_26;
								v1x->subSpellIndex_0x2A_42 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].subSpellIndex_2;
								v1x->levelPar3_0x46_70 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].life_0x1A;
								v1x->playerEntityIndex_0x94_148 = 0;
								if ((unsigned __int8)SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].life_0x1A >= 3u)
								{
									v1x->playerEntityIndex_0x94_148 = v2x->id_0x1A_26;
									SetManaSphereColorAndRot_36920(/*a1x, */v1x);
								}
								a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
								v1x->yaw_0x1C_28 = (v2x->yaw_0x1C_28
									- 85
									+ a1x->rand_0x14_20 % 0xAAu
									+ v2x->playerData_0xA4_164x->nextEntity_0x18_24) & 0x7FF;
								v1x->pitch_0x1E_30 = v2x->playerData_0xA4_164x->entityIndex2_0x1A_26 + v2x->pitch_0x1E_30;
							}
							v6++;
						}
						if (v1x)
							PrepareEventSound_6E450(v1x - world_D41A0.entities_0x6E8E, -1, 11);
					}
					RechargeSpell_68DE0(a1x, v2x);
				}
			}
			else
			{
				a1x->stage_0x2E_46 = 1;
			}
		}
		v4 = a1x->stage_0x2E_46 - 1;
		a1x->stage_0x2E_46 = v4;
		if (!v4)
			RestorePreviousSpell_6D880(a1x);
	}
	//result = *(x_WORD *)(a1 + 54);
	if (a1x->param3_0x36_54)
		a1x->param3_0x36_54 = a1x->param3_0x36_54 - 1;
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0006CA80) --------------------------------------------------------
signed int SpellFoolsManaPickup_6CA80(type_entity_0x6E8E* a1)//24da80
{
	return UpdateSpellPickupFloating_692A0(a1);
}

//----- (0006CAA0) --------------------------------------------------------
signed int SpellFoolsManaPickupUpgrade_6CAA0(type_entity_0x6E8E* a1)//24daa0
{
	return UpdateSpellPickup_69250(a1);
}

//----- (0006CAC0) --------------------------------------------------------
void SpellMagicMineActive_6CAC0(type_entity_0x6E8E* a1x)//24dac0
{
	type_entity_0x6E8E* v1x; // eax
	type_entity_0x6E8E* v2x; // ebx
	//int v3; // edx
	//uint8_t* v4; // eax
	__int16 v5; // di
	//int result; // eax
	__int16 v7; // bx
	type_entity_0x6E8E* v8x; // [esp+0h] [ebp-4h]

	if (a1x->stage_0x2E_46 > 0)
	{
		v1x = Entities_EA3E4[a1x->parentId_0x28_40];
		v8x = v1x;
		if (v1x > Entities_EA3E4[0])
		{
			if (CanSpellOwnerCast_68D50(a1x, v1x))
			{
				if (a1x->stage_0x2E_46 == a1x->maxStage_0x30_48)
				{
					v2x = CreateEntity_4A190(&v8x->position_0x4C_76, 9, 29);
					if (v2x)
					{
						v2x->actSpeed_0x82_130 += v8x->actSpeed_0x82_130;
						LaunchSpellProjectile_68E50(v8x, v2x, a1x);
						//v3 = (uint8_t*)a1x - (&x_D41A0_BYTEARRAY_0[28302]);
						v2x->levelPar1_0x43_67 = 10;
						v2x->levelPar2_0x44_68 = 78;
						v2x->hitById_0x26_38 = a1x - world_D41A0.entities_0x6E8E;
						v2x->id_0x1A_26 = v8x->id_0x1A_26;
						v2x->position_0x4C_76.z += v8x->params_0x52_82.fov;
						v2x->mana_0x90_144 = a1x->mana_0x90_144;
						v2x->subSpellIndex_0x2A_42 = a1x->levelPar3_0x46_70;
						v2x->param_0x10_16 = v8x->playerData_0xA4_164x->chargeTime_0x154_340;
						v8x->playerData_0xA4_164x->chargeTime_0x154_340 = 0;
						v2x->axis_0x9A_154x = v8x->position_0x4C_76;
						MoveEntity_57FA0(&v2x->axis_0x9A_154x, v8x->playerData_0xA4_164x->nextEntity_0x18_24 + v8x->yaw_0x1C_28, 0, 4096);
						v2x->axis_0x9A_154x.z = GetTerrainAlt_10C40(&v2x->axis_0x9A_154x);
						v2x->yaw_0x1C_28 = v8x->playerData_0xA4_164x->nextEntity_0x18_24 + v8x->yaw_0x1C_28;
						//v4 = x_D41A0_BYTEARRAY_0 + 28302;
						v2x->pitch_0x1E_30 = v8x->playerData_0xA4_164x->entityIndex2_0x1A_26 + v8x->pitch_0x1E_30;
						PrepareEventSound_6E450(v2x - world_D41A0.entities_0x6E8E, -1, 15);
					}
					RechargeSpell_68DE0(a1x, v8x);
				}
			}
			else
			{
				a1x->stage_0x2E_46 = 1;
			}
		}
		v5 = a1x->stage_0x2E_46 - 1;
		a1x->stage_0x2E_46 = v5;
		if (!v5)
			RestorePreviousSpell_6D880(a1x);
	}
	//result = a1;
	v7 = a1x->param3_0x36_54;
	if (v7)
		a1x->param3_0x36_54 = v7 - 1;
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0006CCE0) --------------------------------------------------------
signed int SpellMagicMinePickup_6CCE0(type_entity_0x6E8E* a1)//24dce0
{
	return UpdateSpellPickupFloating_692A0(a1);
}

//----- (0006CD00) --------------------------------------------------------
signed int SpellMagicMinePickupUpgrade_6CD00(type_entity_0x6E8E* a1)//24dd00
{
	return UpdateSpellPickup_69250(a1);
}

//----- (0006CD20) --------------------------------------------------------
void SpellAllianceActive_6CD20(type_entity_0x6E8E* a1x)//24dd20
{
	type_entity_0x6E8E* v1x; // eax
	//char *v2; // esi
	type_entity_0x6E8E* v3x; // ebx
	__int16 v4; // ax
	//uint8_t* v5; // eax
	__int16 v6; // di
	//int result; // eax
	__int16 v8; // bx
	type_entity_0x6E8E* v9x; // [esp+0h] [ebp-4h]

	if (a1x->stage_0x2E_46 > 0)
	{
		v1x = Entities_EA3E4[a1x->parentId_0x28_40];
		v9x = v1x;
		if (v1x > Entities_EA3E4[0])
		{
			if (CanSpellOwnerCast_68D50(a1x, v1x))
			{
				if (a1x->stage_0x2E_46 == a1x->maxStage_0x30_48)
				{
					//v2 = (char*)&(*xadataspellsdat.colorPalette_var28)[80 * a1x->byte_0x40_64 + 2 + 26 * a1x->byte_0x46_70];
					v3x = CreateEntity_4A190(&v9x->position_0x4C_76, 9, 25);
					if (v3x)
					{
						v3x->actSpeed_0x82_130 += v9x->actSpeed_0x82_130;
						LaunchSpellProjectile_68E50(v9x, v3x, a1x);
						v4 = a1x - world_D41A0.entities_0x6E8E;
						v3x->levelPar1_0x43_67 = 10;
						v3x->hitById_0x26_38 = v4;
						v3x->levelPar2_0x44_68 = 74;
						v3x->id_0x1A_26 = v9x->id_0x1A_26;
						v3x->parentId_0x28_40 = a1x->parentId_0x28_40;
						v3x->subSpellIndex_0x2A_42 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].subSpellIndex_2;
						v3x->levelPar3_0x46_70 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].life_0x1A;
						v3x->param_0x10_16 = v9x->playerData_0xA4_164x->chargeTime_0x154_340;
						v9x->playerData_0xA4_164x->chargeTime_0x154_340 = 0;
						v3x->axis_0x9A_154x = v9x->position_0x4C_76;
						MoveEntity_57FA0(
							&v3x->axis_0x9A_154x,
							v9x->playerData_0xA4_164x->nextEntity_0x18_24 + v9x->yaw_0x1C_28,
							v9x->playerData_0xA4_164x->entityIndex2_0x1A_26 + v9x->pitch_0x1E_30,
							0x4000);
						v3x->yaw_0x1C_28 = v9x->playerData_0xA4_164x->nextEntity_0x18_24 + v9x->yaw_0x1C_28;
						//v5 = x_D41A0_BYTEARRAY_0 + 28302;
						v3x->pitch_0x1E_30 = v9x->playerData_0xA4_164x->entityIndex2_0x1A_26 + v9x->pitch_0x1E_30;
						PrepareEventSound_6E450(v3x - world_D41A0.entities_0x6E8E, -1, 9);
					}
				}
				RechargeSpell_68DE0(a1x, v9x);
			}
			else
			{
				a1x->stage_0x2E_46 = 1;
			}
		}
		v6 = a1x->stage_0x2E_46 - 1;
		a1x->stage_0x2E_46 = v6;
		if (!v6)
			RestorePreviousSpell_6D880(a1x);
	}
	//result = a1;
	v8 = a1x->param3_0x36_54;
	if (v8)
		a1x->param3_0x36_54 = v8 - 1;
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0006CF60) --------------------------------------------------------
signed int SpellAlliancePickup_6CF60(type_entity_0x6E8E* a1)//24df60
{
	return UpdateSpellPickupFloating_692A0(a1);
}

//----- (0006CF80) --------------------------------------------------------
signed int SpellAlliancePickupUpgrade_6CF80(type_entity_0x6E8E* a1)//24df80
{
	return UpdateSpellPickup_69250(a1);
}

//----- (0006CFA0) --------------------------------------------------------
void SpellCaveInActive_6CFA0(type_entity_0x6E8E* a1x)//24dfa0
{
	type_entity_0x6E8E* v1x; // ebx
	//char *v2; // edi
	type_entity_0x6E8E* v3x; // esi
	unsigned __int8 v4; // dl
	int v5; // eax
	__int16 v6; // dx
	//int result; // eax
	__int16 v8; // cx

	if (isCaveLevel_D41B6 && a1x->stage_0x2E_46 > 0)
	{
		v1x = Entities_EA3E4[a1x->parentId_0x28_40];
		if (v1x > Entities_EA3E4[0])
		{
			if (CanSpellOwnerCast_68D50(a1x, v1x))
			{
				if (a1x->stage_0x2E_46 == a1x->maxStage_0x30_48)
				{
					//v2 = (char*)&(*xadataspellsdat.colorPalette_var28)[80 * a1x->byte_0x40_64 + 2 + 26 * a1x->byte_0x46_70];
					v3x = CastSpell_6DCA0(v1x, &v1x->position_0x4C_76, 0x19u, &SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70], v1x->actSpeed_0x82_130, 1);
					if (v3x)
					{
						LaunchSpellProjectile_68E50(v1x, v3x, a1x);
						v3x->hitById_0x26_38 = a1x - world_D41A0.entities_0x6E8E;
						v3x->id_0x1A_26 = v1x->id_0x1A_26;
						v3x->position_0x4C_76.z += v1x->params_0x52_82.fov;
						v3x->mana_0x90_144 = a1x->mana_0x90_144;
						v4 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].life_0x1A;
						if (v4)
							v5 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].subSpellIndex_2 / (signed int)v4;
						else
							LOWORD(v5) = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].subSpellIndex_2;
						v3x->subSpellIndex_0x2A_42 = v5;
						v3x->levelPar3_0x46_70 = SPELLS_BEGIN_BUFFER_str[a1x->model_0x40_64].subspell[a1x->levelPar3_0x46_70].life_0x1A;
						v3x->param_0x10_16 = v1x->playerData_0xA4_164x->chargeTime_0x154_340;
						v1x->playerData_0xA4_164x->chargeTime_0x154_340 = 0;
						v3x->axis_0x9A_154x = v1x->position_0x4C_76;
						MoveEntity_57FA0(&v3x->axis_0x9A_154x, v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v1x->yaw_0x1C_28, 0, 4096);
						v3x->axis_0x9A_154x.z = GetTerrainAlt_10C40(&v3x->axis_0x9A_154x);
						v3x->yaw_0x1C_28 = v1x->playerData_0xA4_164x->nextEntity_0x18_24 + v1x->yaw_0x1C_28;
						v3x->pitch_0x1E_30 = v1x->playerData_0xA4_164x->entityIndex2_0x1A_26 + v1x->pitch_0x1E_30;
					}
					RechargeSpell_68DE0(a1x, v1x);
				}
			}
			else
			{
				a1x->stage_0x2E_46 = 1;
			}
		}
		v6 = a1x->stage_0x2E_46 - 1;
		a1x->stage_0x2E_46 = v6;
		if (!v6)
			RestorePreviousSpell_6D880(a1x);
	}
	//result = a1;
	v8 = a1x->param3_0x36_54;
	if (v8)
		a1x->param3_0x36_54 = v8 - 1;
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41B6: using guessed type char x_BYTE_D41B6;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0006D1C0) --------------------------------------------------------
signed int SpellCaveInPickup_6D1C0(type_entity_0x6E8E* a1)//24e1c0
{
	return UpdateSpellPickupFloating_692A0(a1);
}

//----- (0006D1E0) --------------------------------------------------------
signed int SpellCaveInPickupUpgrade_6D1E0(type_entity_0x6E8E* a1)//24e1e0
{
	return UpdateSpellPickup_69250(a1);
}

//----- (0006D830) --------------------------------------------------------
void CopyAxisForSpellWithLife_6D830(type_entity_0x6E8E* entity, int spellId)//24e830
{
	if (entity->model_0x40_64 == 10)
	{
		if (Entities_EA3E4[entity->parentId_0x28_40] > Entities_EA3E4[0] && SPELLS_BEGIN_BUFFER_str[10].subspell[spellId].life_0x1A == 1)
		{
			entity->axis_0x9A_154x = Entities_EA3E4[entity->parentId_0x28_40]->position_0x4C_76;
		}
	}
}

//----- (0006D880) --------------------------------------------------------
void RestorePreviousSpell_6D880(type_entity_0x6E8E* a1x)//24e880
{
	//char result=0; // al

	if (a1x->param2_0x2C_44)
	{
		/*result = */SetSpell_6D5E0(a1x, a1x->param2_0x2C_44 - 1);
		a1x->param2_0x2C_44 = 0;
	}
	//return result;
}

//----- (0006D8B0) --------------------------------------------------------
void SendSpellMessage_6D8B0(unsigned __int16 a1, unsigned __int16 a2, __int16 a3)//24e8b0
{
	type_entity_0x6E8E* v3x; // eax
	//int v4; // ebx
	int v5; // ecx
	//int v6; // ebx

	if (!(gameState_D41A4.setting_38545 & 4))
	{
		if (a1)
		{
			v3x = Entities_EA3E4[a1];
			if (v3x->class_0x3F_63 == 3 && !v3x->model_0x40_64)
			{
				//v4 = v3x->dword_0xA4_164;
				v5 = v3x->playerData_0xA4_164x->str_611.spellsExperience_0x2CB_715x.at(a2);
				//v6 = v4 + 611;
				v3x->playerData_0xA4_164x->str_611.spellsExperience_0x2CB_715x.at(a2) = a3 + v5;
				if (a2 == 2)
					SetSpell_6D5E0(Entities_EA3E4[v3x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[2]], v3x->playerData_0xA4_164x->str_611.selectedSubSpells_0x437_1079x.SpellIndex[2]);
				if (gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
				{
					if (a1 == world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240)
						//UpdateSpellExperience_6DAD0(&v3x->playerData_0xA4_164x->str_611, (unsigned __int8 *)&(*xadataspellsdat.colorPalette_var28)[80 * a2], a2);
						UpdateSpellExperience_6DAD0(&v3x->playerData_0xA4_164x->str_611, &SPELLS_BEGIN_BUFFER_str[a2], a2);
				}
				else
				{
					//UpdateSpellAvailability_6D9C0(&v3x->playerData_0xA4_164x->str_611, (unsigned __int8 *)&(*xadataspellsdat.colorPalette_var28)[80 * a2], a2, 0, 1);
					UpdateSpellAvailability_6D9C0(&v3x->playerData_0xA4_164x->str_611, &SPELLS_BEGIN_BUFFER_str[a2], a2, 0, 1);
				}
			}
		}
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0006D9C0) --------------------------------------------------------
void UpdateSpellAvailability_6D9C0(type_str_611* a1x, type_SPELLS_BEGIN_BUFFER_str* a2x, __int16 a3, char a4, char a5)//24e9c0
{
	signed int v5; // esi
	int v6; // eax
	int v7; // edi
	//int v8; // edx
	//int result; // eax
	char v10; // dh

	v5 = 0;
	if ((a1x->availableSpells_0x3E9_1001x.SpellIndex[a3] || a1x->SpellsEnabled_0x333_819x.SpellEnabled[a3]) && (isCaveLevel_D41B6 || a3 != 25))
		v5 = 1;
	if (gameState_D41A4.setting_byte2_23 >= 0 && a1x->spellsExperience_0x2CB_715x.at((int)spell_t::castle) > 7) // FIXME: replace magic numbers 2 and 7
		a1x->spellsExperience_0x2CB_715x.at((int)spell_t::castle) = 7;
	//v6 = *a2;
	v6 = a2x->levelCount_0;
	v7 = a1x->spellsExperience_0x2CB_715x.at(a3) + a1x->SpellExperience_0x263_611x.SpellExperience[a3];
	do
		v6--;
	//while (v6 >= 0 && v7 < *(x_DWORD *)&a2[26 * v6 + 14]);
	while (v6 >= 0 && v7 < a2x->subspell[v6].xpos1_E);
	if (v6 < 0)
		v6 = 0;
	//v8 = a1 + a3;
	if (v6 != a1x->SpellLevels_0x41D_1053z.SpellIndex[a3])
	{
		a1x->SpellLevels_0x41D_1053z.SpellIndex[a3] = v6;
		if (v5)
		{
			if (a5)
				ImproveAbility_6DC40(a3);
		}
	}
	//result = a1 + a3;
	v10 = a1x->SpellLevels_0x41D_1053z.SpellIndex[a3];
	if (a1x->selectedSubSpells_0x437_1079x.SpellIndex[a3] > v10)
		a1x->selectedSubSpells_0x437_1079x.SpellIndex[a3] = v10;
	if (v5 && a4)
	{
		//if (v7 >= *(x_DWORD *)(a2 + 66))
		if (v7 >= a2x->subspell[2].xpos1_E)
		{
			//result = *(x_DWORD *)(a2 + 66);
			//a1x->SpellExperience_0x263_611x.dword[a3] = *(x_DWORD*)(a2 + 66);
			a1x->SpellExperience_0x263_611x.SpellExperience[a3] = a2x->subspell[2].xpos1_E;
		}
		else
		{
			//result = a3;
			a1x->SpellExperience_0x263_611x.SpellExperience[a3] = v7;
		}
	}
	//return result;
}
// D41A4: using guessed type int x_DWORD_D41A4;
// D41B6: using guessed type char x_BYTE_D41B6;

//----- (0006DAD0) --------------------------------------------------------
void UpdateSpellExperience_6DAD0(type_str_611* a1x, type_SPELLS_BEGIN_BUFFER_str* a2x, __int16 a3)//24ead0
{
	int v3; // eax
	//int v4; // edx

	//if (a1x->SpellsEnabled_0x333_819x.word[a3] && a1x->SpellLevels_0x41D_1053z.byte[a3] < *a2 - 1)
	if (a1x->SpellsEnabled_0x333_819x.SpellEnabled[a3] && a1x->SpellLevels_0x41D_1053z.SpellIndex[a3] < a2x->levelCount_0 - 1)
	{
		v3 = a1x->spellsExperience_0x2CB_715x.at(a3) + a1x->SpellExperience_0x263_611x.SpellExperience[a3];
		if (v3 < 0)
			v3 = 0;
		/*if (v3 > *(x_DWORD *)(a2 + 70) + 2)
			v3 = *(x_DWORD *)(a2 + 70) + 2;*/
		if (v3 > a2x->subspell[2].xpos2_0x12 + 2)
			v3 = a2x->subspell[2].xpos2_0x12 + 2;
		//v4 = a1 + a3;
		//if (v3 >= *(x_DWORD *)&a2[26 * (a1x->SpellLevels_0x41D_1053z.byte[a3] + 1) + 18])
		if (v3 >= a2x->subspell[a1x->SpellLevels_0x41D_1053z.SpellIndex[a3] + 1].xpos2_0x12)
		{
			a1x->SpellLevels_0x41D_1053z.SpellIndex[a3]++;
			ImproveAbility_6DC40(a3);
		}
	}
}

//----- (0006DB50) --------------------------------------------------------
void GrantSpellToPlayer_6DB50(char a1, char a2)//24eb50
{
	int v3x;
	signed int v4; // ebx
	type_str_611* v5; // edi
	__int16 v6; // ST08_2

	if (!(gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE))
	{
		v3x = 0;
		v4 = 0;
		v5 = &Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240]->playerData_0xA4_164x->str_611;
		while (v4 < 26)
		{
			v6 = v4++;
			UpdateSpellAvailability_6D9C0(v5, &SPELLS_BEGIN_BUFFER_str[v3x], v6, a1, a2);
			v3x++;
		}
	}
}

//----- (0006DBD0) --------------------------------------------------------
void GrantAllSpellsMultiplayer_6DBD0()//24ebd0
{
	//unsigned __int8 *v0; // esi
	int v0x;
	signed int v1; // ebx
	type_str_611* v2; // edi
	__int16 v3; // ST08_2

	if (gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
	{
		//v0 = (unsigned __int8 *)*xadataspellsdat.colorPalette_var28;
		v0x = 0;
		v1 = 0;
		v2 = &Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240]->playerData_0xA4_164x->str_611;
		while (v1 < 26)
		{
			v3 = v1++;
			UpdateSpellExperience_6DAD0(v2, &SPELLS_BEGIN_BUFFER_str[v0x], v3);
			//v0 += 80;
			v0x++;
		}
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0006DC40) --------------------------------------------------------
void ImproveAbility_6DC40(uint8_t ability)//24ec40
{
	//char v1; // [esp+0h] [ebp-2h]

	sprintf(printbuffer, (const char*)langStrings_E9C4C[159], langStrings_E9C4C[160 + ability]);//Your ability to cast %s has improved.
	SetCurrentNotificationMessage_19760(printbuffer, 5u, 200);
	PrepareEventSound_6E450(world_D41A0.LevelIndex_0xc, -1, 61);
}
// 8E3D5: using guessed type x_DWORD sprintf(x_DWORD, const char *, ...);
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// E9EC8: using guessed type int x_DWORD_E9EC8;

//----- (0006DCA0) --------------------------------------------------------
type_entity_0x6E8E* CastSpell_6DCA0(type_entity_0x6E8E* a1x, axis_3d* a2x, unsigned __int16 a3, type_SPELLS_BEGIN_BUFFER_str_sub* a4x, __int16 a5, char a6)//24eca0
{
	__int16 v6; // di
	type_entity_0x6E8E* v7x; // ebx
	type_entity_0x6E8E* v8x; // eax
	//unsigned __int8 v9; // dl
	type_entity_0x6E8E* v10x; // eax
	__int16 v11; // ax
	type_entity_0x6E8E* v12x; // eax
	type_entity_0x6E8E* v13x; // eax
	type_entity_0x6E8E* v14x; // eax
	type_entity_0x6E8E* v15x; // eax
	type_entity_0x6E8E* v16x; // eax
	type_entity_0x6E8E* v17x; // eax
	type_entity_0x6E8E* v18x; // eax
	type_entity_0x6E8E* v19x; // eax
	type_entity_0x6E8E* v20x; // eax
	type_entity_0x6E8E* v21x; // ecx
	int v22; // eax
	type_entity_0x6E8E* v23x; // eax
	__int16 v24; // dx

	v6 = 15;
	v7x = 0;
	if (a3 < 0x10u)
	{
		if (a3 < 9u)
		{
			if (a3)
			{
				if (a3 != 7)
					goto LABEL_60;
				//v9 = *((x_BYTE *)a4 + 24);
				if (a4x->life_0x1A)
				{
					if (a4x->life_0x1A > 2u)
						goto LABEL_60;
					v12x = CreateEntity_4A190(a2x, 9, 12);
					v7x = v12x;
					if (!v12x)
						goto LABEL_60;
					v12x->levelPar1_0x43_67 = 9;
					v12x->levelPar2_0x44_68 = 9;
					v11 = a4x->subSpellIndex_2;
					v6 = 9;
				}
				else
				{
					v10x = CreateEntity_4A190(a2x, 9, 9);
					v7x = v10x;
					if (!v10x)
						goto LABEL_60;
					v10x->levelPar1_0x43_67 = 10;
					v10x->levelPar2_0x44_68 = 23;
					v11 = a4x->subSpellIndex_2;
					v6 = 23;
				}
				v7x->subSpellIndex_0x2A_42 = v11;
				goto LABEL_60;
			}
			if (a4x->life_0x1A >= 2u)
				v8x = CreateEntity_4A190(a2x, 9, 28);
			else
				v8x = CreateEntity_4A190(a2x, 9, 0);
			v7x = v8x;
			if (v8x)
			{
				v8x->levelPar1_0x43_67 = 10;
				if (a4x->life_0x1A >= 2u)
					v8x->levelPar2_0x44_68 = 76;
				else
					v8x->levelPar2_0x44_68 = 0;
				v6 = 9;
			}
		}
		else
		{
			if (a3 <= 9u)
			{
				v13x = CreateEntity_4A190(a2x, 9, 3);
				v7x = v13x;
				if (!v13x)
					goto LABEL_60;
				v13x->levelPar1_0x43_67 = 10;
				v13x->levelPar2_0x44_68 = 17;
				v13x->subSpellIndex_0x2A_42 = a4x->subSpellIndex_2;
				goto LABEL_59;
			}
			if (a3 < 0xDu)
				goto LABEL_60;
			if (a3 <= 0xDu)
			{
				v14x = CreateEntity_4A190(a2x, 9, 8);
				v7x = v14x;
				if (v14x)
				{
					v14x->levelPar1_0x43_67 = 10;
					v14x->levelPar2_0x44_68 = 25;
				}
			}
			else if (a3 == 15)
			{
				v15x = CreateEntity_4A190(a2x, 9, 23);
				v7x = v15x;
				if (v15x)
				{
					v15x->levelPar1_0x43_67 = 10;
					v15x->levelPar2_0x44_68 = 71;
					v15x->subSpellIndex_0x2A_42 = a4x->subSpellIndex_2;
				LABEL_59:
					v7x->levelPar3_0x46_70 = a4x->life_0x1A;
					goto LABEL_60;
				}
			}
		}
	}
	else if (a3 <= 0x10u)
	{
		v16x = CreateEntity_4A190(a2x, 9, 5);
		v7x = v16x;
		if (v16x)
		{
			v16x->levelPar1_0x43_67 = 10;
			v16x->levelPar2_0x44_68 = 11;
			v16x->subSpellIndex_0x2A_42 = a4x->subSpellIndex_2;
			v16x->levelPar3_0x46_70 = a4x->life_0x1A;
		}
	}
	else if (a3 < 0x14u)
	{
		if (a3 <= 0x11u)
		{
			v17x = CreateEntity_4A190(a2x, 9, 2);
			v7x = v17x;
			if (v17x)
			{
				v17x->levelPar1_0x43_67 = 10;
				v17x->levelPar2_0x44_68 = 15;
				v17x->subSpellIndex_0x2A_42 = a4x->subSpellIndex_2;
				v17x->levelPar3_0x46_70 = a4x->life_0x1A;
			}
		}
		else if (a3 == 18)
		{
			v18x = CreateEntity_4A190(a2x, 9, 4);
			v7x = v18x;
			if (v18x)
			{
				v18x->levelPar1_0x43_67 = 10;
				v18x->levelPar2_0x44_68 = 9;
				v18x->subSpellIndex_0x2A_42 = a4x->subSpellIndex_2;
				v18x->levelPar3_0x46_70 = a4x->life_0x1A;
			}
		}
	}
	else if (a3 <= 0x14u)
	{
		v19x = CreateEntity_4A190(a2x, 9, 22);
		v7x = v19x;
		if (v19x)
		{
			v19x->levelPar1_0x43_67 = 10;
			v19x->levelPar2_0x44_68 = 67;
			v19x->subSpellIndex_0x2A_42 = a4x->subSpellIndex_2;
			v19x->levelPar3_0x46_70 = a4x->life_0x1A;
		}
	}
	else
	{
		if (a3 <= 0x15u)
		{
			v20x = CreateEntity_4A190(a2x, 9, 26);
			v21x = v20x;
			v7x = v20x;
			if (!v20x)
				goto LABEL_60;
			v20x->levelPar1_0x43_67 = 10;
			v20x->levelPar2_0x44_68 = 22;
			if (a4x->life_0x1A)
				v22 = a4x->subSpellIndex_2 / a4x->life_0x1A;
			else
				LOWORD(v22) = a4x->subSpellIndex_2;
			goto LABEL_58;
		}
		if (a3 == 25)
		{
			v23x = CreateEntity_4A190(a2x, 9, 30);
			v21x = v23x;
			v7x = v23x;
			if (v23x)
			{
				v23x->levelPar1_0x43_67 = 10;
				v23x->levelPar2_0x44_68 = 89;
				if (a4x->life_0x1A)
					v22 = a4x->subSpellIndex_2 / a4x->life_0x1A;
				else
					LOWORD(v22) = a4x->subSpellIndex_2;
			LABEL_58:
				v21x->subSpellIndex_0x2A_42 = v22;
				goto LABEL_59;
			}
		}
	}
LABEL_60:
	if (v7x)
	{
		v24 = a5 + v7x->actSpeed_0x82_130;
		v7x->actSpeed_0x82_130 = v24;
		if (v24 < 384)
			v7x->actSpeed_0x82_130 = 384;
		if (v7x->actSpeed_0x82_130 > 0x2000)
			v7x->actSpeed_0x82_130 = 0x2000;
		if (a6)
			PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, v6);
	}
	return v7x;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (0006E020) --------------------------------------------------------
int GetSpellIndex_6E020(int entitySubtype)//24f020
{
	switch (entitySubtype) {
		case 9:
			return 18;
		case 11:
			return 16;
		case 15:
			return 17;
		case 17:
			return 9;
		case 22:
			return 21;
		case 67:
			return 20;
		case 71:
			return 15;
	}
	return 0;
}

//----- (0006E090) --------------------------------------------------------
void UpdateExperience_6E090(type_str_611* spells, int countXP)//24f090
{
	for (int i = 0; i < 26; i++)
	{
		if (spells->SpellsEnabled_0x333_819x.SpellEnabled[i])
			spells->spellsExperience_0x2CB_715x.at(i) += countXP;
	}
	if (gameState_D41A4.setting_byte2_23 >= 0 && spells->spellsExperience_0x2CB_715x.at((int)spell_t::castle) > 7)
		spells->spellsExperience_0x2CB_715x.at((int)spell_t::castle) = 7;
}

//----- (0006E0D0) --------------------------------------------------------
void EnableAllSpells_6E0D0()//24f0d0
{
	signed int v0; // edx
	type_str_611* v1x; // ecx
	int v2; // ebx

	v0 = 0;
	v1x = &Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240]->playerData_0xA4_164x->str_611;
	while (v0 < 26)
	{
		v1x->availableSpells_0x3E9_1001x.SpellIndex[v0] = 1;
		//v2 = *(int32_t*)& SPELLS_BEGIN_BUFFER_DA818[0x42+4*20 * v0];
		v2 = SPELLS_BEGIN_BUFFER_str[v0].subspell[2].xpos1_E;
		v1x->SpellExperience_0x263_611x.SpellExperience[v0] = v2 + 1;
		v0++;
	}
	GrantSpellToPlayer_6DB50(1, 0);
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];
