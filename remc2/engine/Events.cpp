#include "Events.h"

#include "GameUI.h"
#include "Map.h"
#include "PlayerInput.h"
#include "Terrain.h"
#include "Type_D93C0_Bldgprmbuffer.h"
#include "LevelInit.h"
#include "DatTabIndexes.h"

#include "../sub_main.h"  // FIXME: only temporary until dependencies are untangled
#include "EventsFunctions.h"


// void CreateLinkedLevelFeatures_49090(Type_Level_2FECE* a1, Type_Level_2FECE* a2); // FIXME: investigate this function: the type of param 2 is wrong - this was the original declaration but the fucntion definition was as below with another type
int shortestLenght_48370(__int16 a1, __int16 a2, __int16 a3);
int SetPredictedPositionFromTiles_483A0(__int16 a1, int a2, char a3, char a4);

typedef std::function<void(uint16_t, uint16_t, uint16_t, uint16_t, uint8_t)> EventInitFunctionType;
void CreateSubtype27LineBetweenPoints_48400(uint16_t posX2, uint16_t posY2, uint16_t posX, uint16_t posY, uint8_t a5 = 0);
void CreateSubtype30LineBetweenPoints_48690(uint16_t posX2, uint16_t posY2, uint16_t posX, uint16_t posY, uint8_t a5 = 0);
void CreateSubtype32MovingToPoint_487D0(uint16_t posX2, uint16_t posY2, uint16_t posX, uint16_t posY, uint8_t a5);
void CreateSubtype51MovingToPoint_48880(uint16_t a1, uint16_t a2, uint16_t a3, uint16_t a4, uint8_t a5 = 0);
void CreateSubtype81WithTarget_48930(uint16_t posX2, uint16_t posY2, uint16_t posX, uint16_t posY, uint8_t a5);

void CreateLinkedLevelFeatures_49090(Type_Level_2FECE* terrain, type_entity_0x30311* entity);
void PrepareEvents_49540(Type_Level_2FECE* terrain, type_entity_0x30311* entity);
void ApplyEvents_498A0();


subx_BYTE_E7EE0x str_E7EE0x[700];
sub2x_BYTE_E7EE0x bitmaps_E9980x[69];

type_D4C52ar2 str_x_DWORD_D4C52ar_0x1D26[93] = {
{0x002A5C44,0x0000,0x0022F320,0x00000001},
{0x002A5C44,0x0001,0x0022F3B0,0x00000001},
{0x002A5C44,0x0002,0x0022F430,0x00000001},
{0x002A5C44,0x0003,0x0022F490,0x00000001},
{0x002A5C44,0x0004,0x0022F500,0x00000001},
{0x002A5C44,0x0005,0x0022F570,0x00000001},
{0x002A5C44,0x0006,0x0022F5F0,0x00000001},
{0x002A5C44,0x0007,0x0022F6A0,0x00000001},
{0x002A5C44,0x0008,0x0022F750,0x00000001},
{0x002A5C44,0x0009,0x0022F760,0x00000001},
{0x002A5C44,0x000A,0x0022F7D0,0x00000001},
{0x002A5C44,0x000B,0x0022F840,0x00000001},
{0x002A5C44,0x000C,0x0022F8C0,0x00000001},
{0x002A5C44,0x000D,0x0022F9E0,0x00000001},
{0x002A5C44,0x000E,0x0022FA20,0x00000001},
{0x002A5C44,0x000F,0x0022FCD0,0x00000001},
{0x002A5C44,0x0010,0x0022FDC0,0x00000001},
{0x002A5C44,0x0011,0x0022FD70,0x00000001},
{0x002A5C44,0x0012,0x0022FED0,0x00000001},
{0x002A5C44,0x0013,0x0022FF90,0x00000001},
{0x002A5C44,0x0014,0x00230020,0x00000001},
{0x002A5C44,0x0015,0x00230030,0x00000001},
{0x002A5C44,0x0016,0x00230040,0x00000001},
{0x002A5C44,0x0017,0x002305F0,0x00000001},
{0x002A5C44,0x0018,0x00230690,0x00000001},
{0x002A5C44,0x0019,0x002306A0,0x00000001},
{0x002A5C44,0x001A,0x00230720,0x00000001},
{0x002A5C44,0x001B,0x002307A0,0x00000001},
{0x002A5C44,0x001C,0x00230800,0x00000001},
{0x002A5C44,0x001D,0x00230A00,0x00000001},
{0x002A5C44,0x001E,0x002309A0,0x00000001},
{0x002A5C44,0x001F,0x00230AC0,0x00000001},
{0x002A5C44,0x0020,0x00230A60,0x00000001},
{0x002A5C44,0x0021,0x00231020,0x00000001},
{0x002A5C44,0x0022,0x00230E40,0x00000001},
{0x002A5C44,0x0023,0x00230F20,0x00000001},
{0x002A5C44,0x0024,0x00230F30,0x00000001},
{0x002A5C44,0x0000,0x00000000,0x00000000},
{0x002A5C44,0x0026,0x00230FB0,0x00000001},
{0x002A5C44,0x0027,0x00231080,0x00000001},
{0x002A5C44,0x0028,0x002311D0,0x00000001},
{0x002A5C44,0x0029,0x00231320,0x00000001},
{0x002A5C44,0x002A,0x00231370,0x00000001},
{0x002A5C44,0x002B,0x002312B0,0x00000001},
{0x002A5C44,0x002C,0x002313D0,0x00000001},
{0x002A5C44,0x002D,0x00231250,0x00000001},
{0x002A5C44,0x0000,0x00000000,0x00000000},
{0x002A5C44,0x0000,0x00000000,0x00000000},
{0x002A5C44,0x0000,0x00000000,0x00000000},
{0x002A5C44,0x0000,0x00000000,0x00000000},
{0x002A5C44,0x0032,0x00230DE0,0x00000001},
{0x002A5C44,0x0033,0x00230D70,0x00000001},
{0x002A5C44,0x0034,0x00231430,0x00000001},
{0x002A5C44,0x0035,0x002314B0,0x00000001},
{0x002A5C44,0x0036,0x00231500,0x00000001},
{0x002A5C44,0x0037,0x00231640,0x00000001},
{0x002A5C44,0x0038,0x002316E0,0x00000001},
{0x002A5C44,0x0039,0x00231130,0x00000001},
{0x002A5C44,0x003A,0x002310A0,0x00000001},
{0x002A5C44,0x003B,0x0022FB50,0x00000001},
{0x002A5C44,0x003C,0x0022FC10,0x00000001},
{0x002A5C44,0x003D,0x00230860,0x00000001},
{0x002A5C44,0x003E,0x002308B0,0x00000001},
{0x002A5C44,0x003F,0x00230900,0x00000001},
{0x002A5C44,0x0040,0x00230950,0x00000001},
{0x002A5C44,0x0041,0x00231780,0x00000001},
{0x002A5C44,0x0042,0x002317C0,0x00000001},
{0x002A5C44,0x0043,0x00232730,0x00000001},
{0x002A5C44,0x0044,0x002315A0,0x00000001},
{0x002A5C44,0x0045,0x00231500,0x00000001},
{0x002A5C44,0x0046,0x0022F950,0x00000001},
{0x002A5C44,0x0047,0x00232790,0x00000001},
{0x002A5C44,0x0048,0x00232800,0x00000001},
{0x002A5C44,0x0049,0x00232A00,0x00000001},
{0x002A5C44,0x004A,0x00231800,0x00000001},
{0x002A5C44,0x004B,0x00000000,0x00000000},
{0x002A5C44,0x004C,0x002302A0,0x00000001},
{0x002A5C44,0x004D,0x00000000,0x00000000},
{0x002A5C44,0x004E,0x00231840,0x00000001},
{0x002A5C44,0x004F,0x002318E0,0x00000001},
{0x002A5C44,0x0050,0x00230B80,0x00000001},
{0x002A5C44,0x0051,0x00230B20,0x00000001},
{0x002A5C44,0x0052,0x00230BE0,0x00000001},
{0x002A5C44,0x0053,0x00230C30,0x00000001},
{0x002A5C44,0x0054,0x00230CA0,0x00000001},
{0x002A5C44,0x0055,0x00230CD0,0x00000001},
{0x002A5C44,0x0056,0x00231960,0x00000001},
{0x002A5C44,0x0057,0x0022FA60,0x00000001},
{0x002A5C44,0x0058,0x00231A10,0x00000001},
{0x002A5C44,0x0059,0x00231A20,0x00000001},
{0x002A5C44,0x005A,0x00231A80,0x00000001},
{0x002A5C44,0x005B,0x0022FF30,0x00000001},
{0x00000000,0x0000,0x00000000,0x00000000}
};


//----- (00010080) --------------------------------------------------------
unsigned int AddE7EE0x_10080(int32_t a1, int32_t a2)//1f1080
{
	int i;
	for (i = 1; i < 0x64 && str_E7EE0x[i].dword_8 != -1; ++i);
	str_E7EE0x[i].dword_0 = a1;
	str_E7EE0x[i].dword_8 = a1;
	str_E7EE0x[i].dword_12 = 0;
	str_E7EE0x[i].dword_4 = a2;
	str_E7EE0x[i].bitmapsStr_16 = &bitmaps_E9980x[str_E7EE0x[i].dword_8];
	str_E7EE0x[i].bitmapsData_20xx = bitmaps_E9980x[str_E7EE0x[i].dword_8].bitmapData_0;
	return i;
}

//----- (00010100) --------------------------------------------------------
void ResetEvent08_10100(int32_t a1)//1f1100
{
	str_E7EE0x[a1].dword_8 = -1;
}

//----- (00049290) --------------------------------------------------------
void GenerateEvents_49290(Type_Level_2FECE* terrain, char a2, uint16_t width, uint16_t height)//22a290
{
	int ix;
	int jx;
	int kx;
	int lx;
	int mx;
	int nx;
	int ox;

	for (ix = 1; ix < 0x4b0; ix++)
	{
		if (terrain->entity_0x30311[ix].DisId == -1 && terrain->entity_0x30311[ix].type_0x30311 == 0x000a && terrain->entity_0x30311[ix].subtype_0x30311 == 0x52)
		{
			PrepareEvents_49540(terrain, &terrain->entity_0x30311[ix]);
			if (a2)
				terrain->entity_0x30311[ix].type_0x30311 = 0;
		}
	}
	ApplyEvents_498A0();//adress 22a2de
	if (CommandLineParams.DoDebugSequences()) {
		add_compare(0x22A2E3, CommandLineParams.DoDebugafterload());
	}

	for (jx = 1; jx < 0x4b0; jx++)
	{
		if (terrain->entity_0x30311[jx].DisId == -1 && terrain->entity_0x30311[jx].type_0x30311 == 0x000a)
		{
			if (terrain->entity_0x30311[jx].subtype_0x30311 == 0x09
				|| terrain->entity_0x30311[jx].subtype_0x30311 == 0x53
				|| terrain->entity_0x30311[jx].subtype_0x30311 == 0x54
				|| terrain->entity_0x30311[jx].subtype_0x30311 == 0x55
				|| terrain->entity_0x30311[jx].subtype_0x30311 == 0x0B
				|| terrain->entity_0x30311[jx].subtype_0x30311 == 0x0F
				|| terrain->entity_0x30311[jx].subtype_0x30311 == 0x1E
				|| terrain->entity_0x30311[jx].subtype_0x30311 == 0x1D
				|| terrain->entity_0x30311[jx].subtype_0x30311 == 0x20
				|| terrain->entity_0x30311[jx].subtype_0x30311 == 0x1F
				|| terrain->entity_0x30311[jx].subtype_0x30311 == 0x33
				|| terrain->entity_0x30311[jx].subtype_0x30311 == 0x32
				|| terrain->entity_0x30311[jx].subtype_0x30311 == 0x58)
			{
				PrepareEvents_49540(terrain, &terrain->entity_0x30311[jx]);//22a357
				if (a2)
					terrain->entity_0x30311[jx].type_0x30311 = 0;
			}
		}
	}
	if (CommandLineParams.DoDebugSequences()) {
		add_compare(0x22A383, CommandLineParams.DoDebugafterload());
	}

	ApplyEvents_498A0();//22a383

	if (CommandLineParams.DoDebugSequences()) {
		add_compare(0x22A388, CommandLineParams.DoDebugafterload());
	}
	for (kx = 1; kx < 0x4b0; kx++)
	{
		if (terrain->entity_0x30311[kx].DisId == -1 && terrain->entity_0x30311[kx].type_0x30311 == 0x000a)
		{
			if (terrain->entity_0x30311[kx].subtype_0x30311 == 0x51
				|| terrain->entity_0x30311[kx].subtype_0x30311 == 0x50)
			{
				PrepareEvents_49540(terrain, &terrain->entity_0x30311[kx]);
				if (a2)
					terrain->entity_0x30311[kx].type_0x30311 = 0;
			}
		}
	}
	if (CommandLineParams.DoDebugSequences()) {
		add_compare(0x22A3D7, CommandLineParams.DoDebugafterload());
	}
	ApplyEvents_498A0();//adress 22a3d7
	for (lx = 1; lx < 0x4b0; lx++)
	{
		if (terrain->entity_0x30311[lx].DisId == -1 && terrain->entity_0x30311[lx].type_0x30311 == 0x000e && terrain->entity_0x30311[lx].subtype_0x30311 == 0x0002)
		{
			PrepareEvents_49540(terrain, &terrain->entity_0x30311[lx]);
			if (a2)
				terrain->entity_0x30311[lx].type_0x30311 = 0;
		}
	}
	if (CommandLineParams.DoDebugSequences()) {
		add_compare(0x22A422, CommandLineParams.DoDebugafterload());
	}
	ApplyEvents_498A0();//22a422
	if (CommandLineParams.DoDebugSequences()) {
		add_compare(0x22A427, CommandLineParams.DoDebugafterload());
	}
	for (mx = 1; mx < 0x4b0; mx++)
	{
		if (terrain->entity_0x30311[mx].DisId == -1 && terrain->entity_0x30311[mx].type_0x30311 == 0x000a)
		{
			if (terrain->entity_0x30311[mx].subtype_0x30311 == 0x1B 
				|| terrain->entity_0x30311[mx].subtype_0x30311 == 0x1C)
			{
				PrepareEvents_49540(terrain, &terrain->entity_0x30311[mx]);
				if (a2)
					terrain->entity_0x30311[mx].type_0x30311 = 0;
			}
		}
	}
	ApplyEvents_498A0();//adress 22a476
	for (nx = 1; nx < 0x4b0; nx++)
	{
		if (terrain->entity_0x30311[nx].DisId == -1 && terrain->entity_0x30311[nx].type_0x30311 == 0x000a && terrain->entity_0x30311[nx].subtype_0x30311 == 0x002d && str_D93C0_bldgprmbuffer[terrain->entity_0x30311[nx].par1_14].byte_2 & 0x10)
		{
			PrepareEvents_49540(terrain, &terrain->entity_0x30311[nx]);
			if (a2)
				terrain->entity_0x30311[nx].type_0x30311 = 0;
		}
	}
	ApplyEvents_498A0();//adress 22a4d1
	if (CommandLineParams.DoDebugSequences()) {
		add_compare(0x22A4D6, CommandLineParams.DoDebugafterload());
	}
	for (ox = 1; ox < 0x4b0; ox++)
	{
		if (terrain->entity_0x30311[ox].DisId == -1 && terrain->entity_0x30311[ox].type_0x30311 == 0x000a && terrain->entity_0x30311[ox].subtype_0x30311 == 0x002d && !(str_D93C0_bldgprmbuffer[terrain->entity_0x30311[ox].par1_14].byte_2 & 0x10))
		{
			PrepareEvents_49540(terrain, &terrain->entity_0x30311[ox]);
			if (a2)
				terrain->entity_0x30311[ox].type_0x30311 = 0;
		}
	}
	if (CommandLineParams.DoDebugSequences()) {
		add_compare(0x22A52C, CommandLineParams.DoDebugafterload());
	}
	ApplyEvents_498A0();//adress 22a52c
}

int debugcounter_22a540 = 0;

//----- (00049540) --------------------------------------------------------
void PrepareEvents_49540(Type_Level_2FECE* terrain, type_entity_0x30311* entity)//22a540
{
	int16_t z_temp; // ax
	type_entity_0x6E8E* event; // eax
	uint32_t temp_adress;

	if (CommandLineParams.DoDebugSequences()) {
		debugcounter_22a540++;
		if (debugcounter_22a540 >= 0x14)
		{
			debugcounter_22a540++;
			debugcounter_22a540--;
		}
		add_compare(0x22a545, CommandLineParams.DoDebugafterload(),0x13);
	}
	switch (entity->type_0x30311)
	{
		case 0x02:
		case 0x0E:
		{			
			predictedAxis_EB398ar.x = entity->axis2d_4.x << 8;//adress 22a7b0
			predictedAxis_EB398ar.y = entity->axis2d_4.y << 8;
			z_temp = getTerrainAlt_10C40(&predictedAxis_EB398ar);
			predictedAxis_EB398ar.z = z_temp;
			temp_adress = str_x_DWORD_D4C52ar_0x2F22[entity->subtype_0x30311].address_6;
			if (temp_adress)
			{
				event = pre_sub_4A190_axis_3d(temp_adress, &predictedAxis_EB398ar);
				if (event)
				{
					event->word_0x2C_44 = entity->par1_14;
					event->word_0x96_150 = entity->par3_18;
				}
			}
			break;
		}
		case 0x0A:
		{
			switch (entity->subtype_0x30311)
			{
				case 0x1C:
				case 0x1D:
				case 0x1F:
				case 0x32:
				case 0x50:
				{
					if (entity->stageTag_12)//only 1c,1d,1f, 32 and 50
						CreateLinkedLevelFeatures_49090(terrain, entity);
					return;
				}
				case 0x2D:
				{
					predictedAxis_EB398ar.x = entity->axis2d_4.x << 8;//adress 22a5af
					predictedAxis_EB398ar.y = entity->axis2d_4.y << 8;
					z_temp = getTerrainAlt_10C40(&predictedAxis_EB398ar);
					predictedAxis_EB398ar.z = z_temp;
					temp_adress = str_x_DWORD_D4C52ar_0x1D26[entity->subtype_0x30311].address_6;
					if (temp_adress)
					{
						event = pre_sub_4A190_axis_3d(temp_adress, &predictedAxis_EB398ar);//(*(int(**)(int))((char *)&off_D697E + 14 * v4))((int)x_WORD_EB398ar);
						if (event)
							InitBuildingEntity_49A30(event, entity->par1_14);
					}
					return;
				}
			}
			predictedAxis_EB398ar.x = entity->axis2d_4.x << 8;
			predictedAxis_EB398ar.y = entity->axis2d_4.y << 8;
			predictedAxis_EB398ar.z = getTerrainAlt_10C40(&predictedAxis_EB398ar);
			temp_adress = str_x_DWORD_D4C52ar_0x1D26[entity->subtype_0x30311].address_6;
			if (temp_adress)
			{
				event = pre_sub_4A190_axis_3d(temp_adress, &predictedAxis_EB398ar);
				if (event)
				{
					switch (entity->subtype_0x30311)
					{
						case 0x09:
						case 0x0B:
						case 0x0F: {
							event->subSpellIndex_0x2A_42 = SPELLS_BEGIN_BUFFER_str[GetSpellIndex_6E020(entity->subtype_0x30311)].subspell[entity->par1_14].subSpellIndex_2;
							if (entity->subtype_0x30311 == 0x09)//9
								event->maxLife_0x4 = SPELLS_BEGIN_BUFFER_str[GetSpellIndex_6E020(entity->subtype_0x30311)].subspell[entity->par1_14].life_0x1A;
							else
								event->life_0x8 = SPELLS_BEGIN_BUFFER_str[GetSpellIndex_6E020(entity->subtype_0x30311)].subspell[entity->par1_14].life_0x1A;
							break;
						}
						case 0x52:
						{
							event->byte_0x46_70 = entity->par3_18;
							event->byte_0x43_67 = entity->par1_14;
							event->byte_0x44_68 = entity->par2_16;
							break;
						}
						case 0x53:
						{
							event->axis_0x9A_154x.x = entity->word_10;
							break;
						}
						case 0x54:
						case 0x55:
						{
							event->axis_0x9A_154x.x = entity->word_10;
							event->position_0x4C_76.z = entity->par3_18;
							break;
						}
						case 0x58:
						{
							event->array_0x52_82.pitch = entity->par1_14;
							event->array_0x52_82.roll = entity->par2_16;
							break;
						}
					}
				}
			}
			break;
		}
	}
}

int debugcounter22a8a0 = 0;
int debugcounter22a8a0bb = 0;
//----- (000498A0) --------------------------------------------------------
void ApplyEvents_498A0()//22a8a0
{
	bool runagain; // esi
	int iy;
	uint8_t v3; // ah
	uint8_t v4; // al
	type_D4C52ar2* v5; // eax
	type_D4C52ar2* v6; // eax

	runagain = true;
	D41A0_0.rand_0x8 = 9377 * D41A0_0.rand_0x8 + 9439;
	while (runagain)
	{
		runagain = false;
		for (iy = 1;iy< 0x3e8; iy++)
		{
			v3 = D41A0_0.struct_0x6E8E[iy].class_0x3F_63;
			if (v3)
			{
				if (v3 != 0x0A)
				{
					if (v3 == 0x0E)//0xE
					{
						if (D41A0_0.struct_0x6E8E[iy].model_0x40_64 == 2 && !D41A0_0.struct_0x6E8E[iy].life_0x8)
						{
							v6 = &str_D4C48ar[D41A0_0.struct_0x6E8E[iy].class_0x3F_63].dword_10[D41A0_0.struct_0x6E8E[iy].actionIndex_0x45_69];
							runagain = true;
							if (v6->address_6 && v6->dword_10)
							{
								pre_sub_4A190_0x6E8E(v6->address_6, &D41A0_0.struct_0x6E8E[iy]);
							}
						}
					}
					else//all without 0xA and 0xE
					{
						DisableEntityDrawing04_57F10(&D41A0_0.struct_0x6E8E[iy]);
					}
					//all without 0xA
					if (D41A0_0.struct_0x6E8E[iy].struct_byte_0xc_12_15.byte[1] & 4)//all without 0xE
						ReleaseEntity_57F20(&D41A0_0.struct_0x6E8E[iy]);
					continue;
				}
				//0xA
				v4 = D41A0_0.struct_0x6E8E[iy].model_0x40_64;
				if (v4 < 0x1Bu)
				{
					if (v4 >= 0xB)
					{
						if (v4 > 0xBu && v4 != 0xFu)//T=0xA ST=0xC,0xD,0xE,0x10,0x11,0x12..0x1A
						{
							DisableEntityDrawing04_57F10(&D41A0_0.struct_0x6E8E[iy]);
							D41A0_0.struct_0x6E8E[iy].byte_0x3E_62++;
							if (D41A0_0.struct_0x6E8E[iy].struct_byte_0xc_12_15.byte[1] & 4)
								ReleaseEntity_57F20(&D41A0_0.struct_0x6E8E[iy]);
							continue;
						}
						v5 = &str_D4C48ar[D41A0_0.struct_0x6E8E[iy].class_0x3F_63].dword_10[D41A0_0.struct_0x6E8E[iy].actionIndex_0x45_69];//ok
						runagain = true;
						if (v5->address_6 && v5->dword_10)
						{
							pre_sub_4A190_0x6E8E(v5->address_6, &D41A0_0.struct_0x6E8E[iy]);
						}
						D41A0_0.struct_0x6E8E[iy].byte_0x3E_62++;
						if (D41A0_0.struct_0x6E8E[iy].struct_byte_0xc_12_15.byte[1] & 4)
							ReleaseEntity_57F20(&D41A0_0.struct_0x6E8E[iy]);
						continue;
					}
					if (v4 == 0x9)//0xA,0x9
					{
						v5 = &str_D4C48ar[D41A0_0.struct_0x6E8E[iy].class_0x3F_63].dword_10[D41A0_0.struct_0x6E8E[iy].actionIndex_0x45_69];//ok
						runagain = true;
						if (v5->address_6 && v5->dword_10)
						{
							pre_sub_4A190_0x6E8E(v5->address_6, &D41A0_0.struct_0x6E8E[iy]);
						}
						D41A0_0.struct_0x6E8E[iy].byte_0x3E_62++;
						if (D41A0_0.struct_0x6E8E[iy].struct_byte_0xc_12_15.byte[1] & 4)
							ReleaseEntity_57F20(&D41A0_0.struct_0x6E8E[iy]);
						continue;
					}
				}
				else
				{
					if (v4 <= 0x20)
					{
						v5 = &str_D4C48ar[D41A0_0.struct_0x6E8E[iy].class_0x3F_63].dword_10[D41A0_0.struct_0x6E8E[iy].actionIndex_0x45_69];//ok
						runagain = true;
						if (v5->address_6 && v5->dword_10)
						{
							pre_sub_4A190_0x6E8E(v5->address_6, &D41A0_0.struct_0x6E8E[iy]);
						}
						D41A0_0.struct_0x6E8E[iy].byte_0x3E_62++;
						if (D41A0_0.struct_0x6E8E[iy].struct_byte_0xc_12_15.byte[1] & 4)
							ReleaseEntity_57F20(&D41A0_0.struct_0x6E8E[iy]);
						continue;
					}
					if (v4 >= 0x32)
					{
						if (v4 > 0x33 && (v4 < 0x50 || v4 > 0x55 && v4 != 0x58))
						{
							DisableEntityDrawing04_57F10(&D41A0_0.struct_0x6E8E[iy]);
							D41A0_0.struct_0x6E8E[iy].byte_0x3E_62++;
							if (D41A0_0.struct_0x6E8E[iy].struct_byte_0xc_12_15.byte[1] & 4)
								ReleaseEntity_57F20(&D41A0_0.struct_0x6E8E[iy]);
							continue;
						}
						v5 = &str_D4C48ar[D41A0_0.struct_0x6E8E[iy].class_0x3F_63].dword_10[D41A0_0.struct_0x6E8E[iy].actionIndex_0x45_69];//ok
						runagain = true;
						if (v5->address_6 && v5->dword_10)
						{
							pre_sub_4A190_0x6E8E(v5->address_6, &D41A0_0.struct_0x6E8E[iy]);
						}
						D41A0_0.struct_0x6E8E[iy].byte_0x3E_62++;
						if (D41A0_0.struct_0x6E8E[iy].struct_byte_0xc_12_15.byte[1] & 4)
							ReleaseEntity_57F20(&D41A0_0.struct_0x6E8E[iy]);
						continue;
					}
					if (v4 == 0x2D)
					{
						if (D41A0_0.struct_0x6E8E[iy].actionIndex_0x45_69 != 0x33)
						{
							D41A0_0.struct_0x6E8E[iy].byte_0x3E_62++;
							if (D41A0_0.struct_0x6E8E[iy].struct_byte_0xc_12_15.byte[1] & 4)
								ReleaseEntity_57F20(&D41A0_0.struct_0x6E8E[iy]);
							continue;
						}
						v5 = &str_D4C48ar[D41A0_0.struct_0x6E8E[iy].class_0x3F_63].dword_10[D41A0_0.struct_0x6E8E[iy].actionIndex_0x45_69];//ok
						runagain = true;
						if (v5->address_6 && v5->dword_10)
						{
							pre_sub_4A190_0x6E8E(v5->address_6, &D41A0_0.struct_0x6E8E[iy]);
						}
						D41A0_0.struct_0x6E8E[iy].byte_0x3E_62++;
						if (D41A0_0.struct_0x6E8E[iy].struct_byte_0xc_12_15.byte[1] & 4)
							ReleaseEntity_57F20(&D41A0_0.struct_0x6E8E[iy]);
						continue;
					}
				}
				DisableEntityDrawing04_57F10(&D41A0_0.struct_0x6E8E[iy]);
				D41A0_0.struct_0x6E8E[iy].byte_0x3E_62++;
				if (D41A0_0.struct_0x6E8E[iy].struct_byte_0xc_12_15.byte[1] & 4)
					ReleaseEntity_57F20(&D41A0_0.struct_0x6E8E[iy]);
				continue;
			}
		}
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D4C52: using guessed type int x_DWORD_D4C52;

//----- (0004A050) --------------------------------------------------------
type_entity_0x6E8E* NewEvent_4A050()//22b050
{
	if (D41A0_0.dword_0x35 >= 0)
	{
		memset(D41A0_0.pointers_0x246[D41A0_0.dword_0x35], 0, sizeof(type_entity_0x6E8E));
		spellTableOverrun[D41A0_0.pointers_0x246[D41A0_0.dword_0x35] - D41A0_0.struct_0x6E8E] = false;
		D41A0_0.pointers_0x246[D41A0_0.dword_0x35]->maxLife_0x4 = 300;
		D41A0_0.pointers_0x246[D41A0_0.dword_0x35]->struct_byte_0xc_12_15.dword = 8;
		D41A0_0.pointers_0x246[D41A0_0.dword_0x35]->actSpeed_0x82_130 = 16;
		D41A0_0.pointers_0x246[D41A0_0.dword_0x35]->subSpellIndex_0x2A_42 = 100;
		D41A0_0.pointers_0x246[D41A0_0.dword_0x35]->id_0x1A_26 = D41A0_0.pointers_0x246[D41A0_0.dword_0x35] - D41A0_0.struct_0x6E8E;
		D41A0_0.pointers_0x246[D41A0_0.dword_0x35]->xtype_0x41_65 = -1;
		D41A0_0.pointers_0x246[D41A0_0.dword_0x35]->xsubtype_0x42_66 = -1;
		D41A0_0.pointers_0x246[D41A0_0.dword_0x35]->dword_0xA0_160x = &str_D7BD6[59]; //(type_str_160*)&unk_D7BD6[0x7d6];
		D41A0_0.pointers_0x246[D41A0_0.dword_0x35]->dword_0xA4_164x = unk_F42B0x;
		D41A0_0.pointers_0x246[D41A0_0.dword_0x35]->byte_0x43_67 = 10;
		D41A0_0.pointers_0x246[D41A0_0.dword_0x35]->byte_0x39_57 = -6;
		D41A0_0.pointers_0x246[D41A0_0.dword_0x35]->byte_0x3E_62 = D41A0_0.pointers_0x246[D41A0_0.dword_0x35] - D41A0_0.struct_0x6E8E;
		D41A0_0.pointers_0x246[D41A0_0.dword_0x35]->rand_0x14_20 = D41A0_0.pointers_0x246[D41A0_0.dword_0x35] - D41A0_0.struct_0x6E8E + D41A0_0.rand_0x8;//this is it line
		return D41A0_0.pointers_0x246[D41A0_0.dword_0x35--];
	}
	if (D41A0_0.dword_0x11e6 >= 0)
	{
		memset(x_D41A0_BYTEARRAY_4_struct.bytearray_38403x, 0, sizeof(type_entity_0x6E8E*) * 29);//type_entity_0x6E8E*
		x_D41A0_BYTEARRAY_4_struct.dword_38523 = 0;
		x_D41A0_BYTEARRAY_4_struct.dword_38527 = 0;
		x_D41A0_BYTEARRAY_4_struct.dword_38519 = 0;
		x_D41A0_BYTEARRAY_4_struct.dword_38531 = 0;
		x_D41A0_BYTEARRAY_4_struct.dword_38535 = 0;
		SetMapEntity_57E50(D41A0_0.dword_0x11EA[D41A0_0.dword_0x11e6]);
		D41A0_0.dword_0x11EA[D41A0_0.dword_0x11e6]->class_0x3F_63 = 0;
		memset(D41A0_0.dword_0x11EA[D41A0_0.dword_0x11e6], 0, sizeof(type_entity_0x6E8E));
		spellTableOverrun[D41A0_0.dword_0x11EA[D41A0_0.dword_0x11e6] - D41A0_0.struct_0x6E8E] = false;
		D41A0_0.dword_0x11EA[D41A0_0.dword_0x11e6]->maxLife_0x4 = 300;
		D41A0_0.dword_0x11EA[D41A0_0.dword_0x11e6]->struct_byte_0xc_12_15.dword = 8;
		D41A0_0.dword_0x11EA[D41A0_0.dword_0x11e6]->actSpeed_0x82_130 = 16;
		D41A0_0.dword_0x11EA[D41A0_0.dword_0x11e6]->subSpellIndex_0x2A_42 = 100;
		D41A0_0.dword_0x11EA[D41A0_0.dword_0x11e6]->id_0x1A_26 = D41A0_0.dword_0x11EA[D41A0_0.dword_0x11e6] - D41A0_0.struct_0x6E8E;
		D41A0_0.dword_0x11EA[D41A0_0.dword_0x11e6]->xtype_0x41_65 = -1;
		D41A0_0.dword_0x11EA[D41A0_0.dword_0x11e6]->xsubtype_0x42_66 = -1;
		D41A0_0.dword_0x11EA[D41A0_0.dword_0x11e6]->dword_0xA0_160x = &str_D7BD6[59]; //(type_str_160*)&unk_D7BD6[0x7d6];
		D41A0_0.dword_0x11EA[D41A0_0.dword_0x11e6]->dword_0xA4_164x = unk_F42B0x;
		D41A0_0.dword_0x11EA[D41A0_0.dword_0x11e6]->byte_0x43_67 = 10;
		D41A0_0.dword_0x11EA[D41A0_0.dword_0x11e6]->byte_0x39_57 = -6;
		D41A0_0.dword_0x11EA[D41A0_0.dword_0x11e6]->byte_0x3E_62 = D41A0_0.dword_0x11EA[D41A0_0.dword_0x11e6] - D41A0_0.struct_0x6E8E;
		D41A0_0.dword_0x11EA[D41A0_0.dword_0x11e6]->rand_0x14_20 = D41A0_0.dword_0x11EA[D41A0_0.dword_0x11e6] - D41A0_0.struct_0x6E8E + D41A0_0.rand_0x8;//this is it line
		return D41A0_0.dword_0x11EA[D41A0_0.dword_0x11e6--];
	}
	return 0;
}

void pre_sub_4A190_0x6E8E(uint32_t adress, type_entity_0x6E8E* a1_6E8E)//pre 22b190
{
	if (CommandLineParams.DoShowNewProcedures()) {
		test_pre_sub_4a190(adress);//for debug
	}
	if (spriteDefsTable_D951C[a1_6E8E->word_0x5A_90].textureIdx_0 == 58)
	{
		int xxx = 1;
		xxx++;
	}
	switch (adress)
	{
	case 0x1f3910: {
		UpdateWizardAi_12910(a1_6E8E);
		break;
	}
	case 0x1f3a70: {
		UpdateWizardAiState_12A70(a1_6E8E);
		break;
	}
	case 0x1f3e70: {
		WizardAiChooseState_12E70(a1_6E8E);
		break;
	}
	case 0x1f3ff0: {
		WizardAiAttackTarget_12FF0(a1_6E8E);
		break;
	}
	case 0x1f4100: {
		WizardAiMoveToPosition_13100(a1_6E8E);
		break;
	}

	case 0x1f41f0: {
		WizardAiFaceTarget_131F0(a1_6E8E);
		break;
	}
	case 0x1f4270: {
		WizardAiCastFoolsMana_13270(a1_6E8E);
		break;
	}
	case 0x1f43b0: {
		WizardAiReturnToCastle_133B0(a1_6E8E);
		break;
	}
	case 0x1f45c0: {
		WizardAiPossessTarget_135C0(a1_6E8E);
		break;
	}
	case 0x1f4710: {
		WizardAiAttackWithSpell_13710(a1_6E8E);
		break;
	}

	case 0x1f4830: {
		WizardAiCombat2_13830(a1_6E8E);
		break;
	}
	case 0x1f4850: {
		WizardAiCombat3_13850(a1_6E8E);
		break;
	}
	case 0x1f4870: {
		WizardAiCombat4_13870(a1_6E8E);
		break;
	}
	case 0x1f4890: {
		WizardAiCombat_13890(a1_6E8E);
		break;
	}
	case 0x1f4b00: {
		WizardAiTryBuildCastle_13B00(a1_6E8E);
		break;
	}

	case 0x1f4c50: {
		WizardAiTryGrowCastle_13C50(a1_6E8E);
		break;
	}
	case 0x1f4ce0: {
		WizardAiFindTarget_13CE0(a1_6E8E);
		break;
	}
	case 0x1f4dc0: {
		WizardAiShouldRetreat_13DC0(a1_6E8E);
		break;
	}
	case 0x1f4e40: {
		WizardAiChooseEnemyWizard_13E40(a1_6E8E);
		break;
	}
	case 0x1f5030: {
		WizardAiChooseEnemyCreature_14030(a1_6E8E);
		break;
	}

	case 0x1f5250: {
		WizardAiChooseEnemyCastle_14250(a1_6E8E);
		break;
	}
	case 0x1f5530: {
		WizardAiChooseAttackTarget_14530(a1_6E8E);
		break;
	}
	case 0x1f5630: {
		WizardAiCheckHealth_14630(a1_6E8E);
		break;
	}
				 /*case 0x1f56c0: {
					 GetPlayerSpellEntity_146C0(a1_6E8E,0);
					 break;
				 }*/
	case 0x1f56f0: {
		UpdateWizardMovement_146F0(a1_6E8E);
		break;
	}

	case 0x1f58e0: {
		FindNearestOwnBalloon_148E0(a1_6E8E);
		break;
	}
				 /*case 0x1f5b10: {
				   FindWizardTargetBySpell_14B10(a1_6E8E,0);
				   break;
				 }
				 case 0x1f5bd0: {
				   FindNearestOtherWizard_14BD0(a1_6E8E,0);
				   break;
				 }*/
	case 0x1f5c40: {
		GetEntityKey_14C40(a1_6E8E);
		break;
	}
				 /*case 0x1f5c60: {
				   IsTargetStillValid_14C60(a1_6E8E,0);
				   break;
				 }*/

	case 0x1f5c90: {
		WizardAiMoveTowards_14C90(a1_6E8E, 0, 0, 0);
		break;
	}
	case 0x1f5e10: {
		WizardAiCastSpell_14E10(a1_6E8E, 0);
		break;
	}
	case 0x1f6170: {
		WizardCanCastSpell_15170(a1_6E8E, 0);
		break;
	}
	case 0x1f65e0: {
		WizardAiCanGrowCastle_155E0(a1_6E8E);
		break;
	}
	case 0x1f66f0: {
		IsPlayerSpellActive_156F0(a1_6E8E, 0);
		break;
	}

	case 0x1f6730: {
		HasEnoughManaForSpell_15730(a1_6E8E, 0);
		break;
	}
	case 0x1f6760: {
		GetActivePlayerSpellEntity_15760(a1_6E8E, 0);
		break;
	}
	case 0x1f6790: {
		WizardAiChooseAttackSpell_15790(a1_6E8E);
		break;
	}
	case 0x1f6910: {
		WizardAiChooseSpell_15910(a1_6E8E);
		break;
	}
	case 0x1f69e0: {
		UpdateWizardAiTargets_159E0();
		break;
	}

	case 0x1f6cb0: {
		FindNearestEntityTargeting_15CB0(a1_6E8E);
		break;
	}
	case 0x1f6d20: {
		SetWizardStrafeSpeed_15D20(a1_6E8E);
		break;
	}
	case 0x1f6d40: {
#if defined(__linux__) || defined(__APPLE__) // FIXME: types
		std::cout << "FIXME: types @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
		RateWizardTarget_15D40((short)a1_6E8E, 0, 0);
		stub_fix_it();//bad retyping
#endif
		break;
	}

	case 0x1f6e60: {
		HasAttackSpell_15E60(a1_6E8E);
		break;
	}
	case 0x1f6ee0: {
		FindLocalWizard_15EE0();
		break;
	}

	case 0x1f6f20: {
		WizardTrySelectSpell_15F20(a1_6E8E, 0, 0);
		break;
	}
	case 0x1f6fc0: {
		WizardAiMetamorph_15FC0(a1_6E8E);
		break;
	}
	case 0x1f71a0: {
		WizardAiAttackMetamorphed_161A0(a1_6E8E);
		break;
	}
	case 0x1f74b0: {
		HasAreaSpell_164B0(a1_6E8E);
		break;
	}
	case 0x1f7580: {
		WizardAiSteer_16580(a1_6E8E);
		break;
	}

	case 0x1f7730: {
		FindAdjacentLavaDirection_16730(a1_6E8E, 0);
		break;
	}
	case 0x1f79c0: {
		WizardAiAvoidObstacles_169C0(a1_6E8E);
		break;
	}
				 /*case 0x1f7ca0: {
				   FindAdjacentLavaDirection2_16CA0(a1_6E8E,0,0);
				   break;
				 }*/
				 /*case 0x1f7e70: {
				   TraceTileLine_16E70((uint8_t*)a1_6E8E,0);
				   break;
				 }*/
	case 0x1f7fc0: {
		FindNearestOtherEntity_16FC0(a1_6E8E, 0);
		break;
	}

	case 0x1f8060: {
		FindAdjacentLavaDirection_16730(a1_6E8E, 0);
		break;
	}
	case 0x1f8190: {
		WizardAiAvoidObstacles_169C0(a1_6E8E);
		break;
	}
				 /*case 0x1f8a00: {
				   FindAdjacentLavaDirection2_16CA0(a1_6E8E,0,0);
				   break;
				 }*/
				 /*case 0x1f9aa0: {
				   TraceTileLine_16E70((uint8_t*)a1_6E8E,0);
				   break;
				 }*/
	case 0x1f9b30: {
		FindNearestOtherEntity_16FC0(a1_6E8E, 0);
		break;
	}

	case 0x1f9bb0: {
		PauseUnpauseGame_18BB0();
		break;
	}
	case 0x1f9da0: {
		CycleSelectedSpell_18DA0(a1_6E8E, 0, 0);
		break;
	}
	case 0x1f9f80: {
		HandleMouseButtons_18F80(a1_6E8E);
		break;
	}
	case 0x1fa1b0: {
#if defined(__linux__) || defined(__APPLE__) // FIXME: types
		std::cout << "FIXME: types @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
		HandleButtonClick_191B0((short)a1_6E8E, 0);
		stub_fix_it();//bad retyping
#endif
		break;
	}
	case 0x1fa760: {
		SetCurrentNotificationMessage_19760((const char*)a1_6E8E, 0, 0);
		break;
	}

	case 0x1fa7f0: {
		ReadPauseMenuEvents_197F0();
		break;
	}
	case 0x1faa50: {
		ClearMouseButtonState_19A50();
		break;
	}
	case 0x1faab0: {
		ReadOptionMenuEvents_19AB0();
		break;
	}
	case 0x1faca0: {
#if defined(__linux__) || defined(__APPLE__) // FIXME: types
		std::cout << "FIXME: types @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
		ChangeSoundLevel_19CA0((uint8_t)a1_6E8E);
		stub_fix_it();//bad retyping
#endif
		break;
	}
	case 0x1fad60: {
#ifdef TEST_x64
		allert_error();
#endif
#ifdef COMPILE_FOR_64BIT // FIXME: 64bit
  		std::cout << "FIXME: 64bit @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
		SetSoundEffectAndMusicLevelCoordinates_19D60((intptr_t)a1_6E8E);
		allert_error();
#endif
		break;
	}

	case 0x1fae00: {
		ReadOkayCancelButtonEvents_19E00();
		break;
	}
	case 0x1fb030: {
		SetOkayCancelButtonsCursorPosition_1A030();
		break;
	}
	case 0x1fb070: {
		AdjustVolume_1A070(0);
		break;
	}
	case 0x1fb280: {
		SetOptionsMenuFlags_1A280();
		break;
	}
	case 0x1fb4a0: {
		GetOptionsMenuWidth_1A4A0();
		break;
	}

	case 0x1fb5b0: {
#ifdef TEST_x64
	allert_error();
#endif
#ifdef COMPILE_FOR_64BIT // FIXME: 64bit
  std::cout << "FIXME: 64bit @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
		GetOptionsLangStrings_1A5B0((intptr_t)a1_6E8E, 0, 0);
		allert_error();
#endif
		break;
	}
	case 0x1fb7a0: {
		FlyAssistant_1A7A0();
		break;
	}
	case 0x1fb8a0: {
		HandleArrowKeyPresses_1A8A0();
		break;
	}
	case 0x1fb970: {
#if defined(__linux__) || defined(__APPLE__) // FIXME: types
		std::cout << "FIXME: types @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
		ChangeSettings_1A970((intptr_t)a1_6E8E, 0, 0);
		stub_fix_it();//bad retyping
#endif
		break;
	}
	case 0x1fc280: {
		PlayIntoSoundEvents_1B280((Type_SoundEvent_E17CC*)a1_6E8E);//FIX ME
		break;
	}
	case 0x1fc2e6:
	case 0x1fc2f7:
	case 0x1fc316:
	case 0x1fc334:
	case 0x1fc352:
	case 0x1fc36c:
	case 0x1fc37d:
	case 0x1fc398:
	case 0x1fc3c9:
	case 0x1fc413:
	case 0x1fc424:
	case 0x1fc450:
	case 0x1fc461:
	case 0x1fc4a0:
	case 0x1fc4df:
	case 0x1fc51e:
	case 0x1fc54a:
	case 0x1fc589:
	case 0x1fc5a7:
	case 0x1fc5bc:
	case 0x1fc5cb:
		allert_error();
		break;
	case 0x1fc5f0: {
		KillAllCreatures_1B5F0();
		break;
	}
	case 0x1fc6b0: {
		CreatureFollowParent_1B6B0(a1_6E8E);
		break;
	}
	case 0x1fc7a0: {
		GetTileSlope_1B7A0((axis_3d*)a1_6E8E);
		break;
	}
	case 0x1fc830: {
		GetTileSlopeWide_1B830((axis_3d*)a1_6E8E);
		break;
	}

	case 0x1fc8c0: {
		MoveCreature_1B8C0(a1_6E8E);
		break;
	}
	case 0x1fcd90: {
		CreatureIdle_1BD90(a1_6E8E, 0);
		break;
	}
	case 0x1fcf90: {
		CreatureWander_1BF90(a1_6E8E, 0);
		break;
	}
	case 0x1fd310: {
		CreatureAttack_1C310(a1_6E8E, 0, 0);
		break;
	}
	case 0x1fd560: {
		CreatureFollowLeader_1C560(a1_6E8E, 0);
		break;
	}

	case 0x1fd890: {
		PreKillEntity_1C890(a1_6E8E, 0);
		break;
	}
	case 0x1fd930: {
		KillEntity_1C930(a1_6E8E);
		break;
	}
	case 0x1fd980: {
		CreatureFlee_1C980(a1_6E8E, 0);
		break;
	}
	case 0x1fdc20: {
		CreatureShootFireball_1CC20(a1_6E8E, 0);
		break;
	}
	case 0x1fdce0: {
		CreatureShootArrow_1CCE0(a1_6E8E, 0);
		break;
	}

	case 0x1fdda0: {
		CreatureShootArrow2_1CDA0(a1_6E8E, 0);
		break;
	}
	case 0x1fde80: {
		CreatureMeleeAttack1024_1CE80(a1_6E8E, 0);
		break;
	}
	case 0x1fded0: {
		CreatureMeleeAttack768_1CED0(a1_6E8E, 0);
		break;
	}
	case 0x1fdf20: {
		CreatureMeleeAttack1536_1CF20(a1_6E8E, 0);
		break;
	}
	case 0x1fe0e0: {
		CreatureShootProjectile20_1D0E0(a1_6E8E, 0);
		break;
	}

	case 0x1fe1a0: {
		CreatureShootProjectile21_1D1A0(a1_6E8E, 0);
		break;
	}
	case 0x1fe260: {
		CreatureShootProjectile9_1D260(a1_6E8E, 0);
		break;
	}
	case 0x1fe460: {
		CreatureShootSpread_1D460(a1_6E8E, 0);
		break;
	}
	case 0x1fe5d0: {
		CreatureRunScript_1D5D0(a1_6E8E, 0);
		break;
	}
	case 0x1fe700: {
		CreatureScriptCmd4_1D700(a1_6E8E, 0);
		break;
	}

	case 0x1fe7c0: {
		CreatureScriptCmd3_1D7C0(a1_6E8E, 0);
		break;
	}
	case 0x1fe880: {
		CreatureScriptCmd8_1D880(a1_6E8E, 0);
		break;
	}
	case 0x1fe8a0: {
		CreatureScriptCmd9_1D8A0(a1_6E8E, 0);
		break;
	}
	case 0x1fe8c0: {
		CreatureScriptAttack_1D8C0(a1_6E8E, 0);
		break;
	}
	case 0x1febf0: {
		CreatureScriptGuardPosition_1DBF0(a1_6E8E, 0);
		break;
	}

	case 0x1feda0: {
		CreatureScriptGoToPosition_1DDA0(a1_6E8E, 0);
		break;
	}
	case 0x1ff000: {
		CreatureScriptCmd6_1E000(a1_6E8E, 0);
		break;
	}
	case 0x1ff020: {
		CreatureScriptCmd7_1E020(a1_6E8E, 0);
		break;
	}
	case 0x1ff040: {
		CreatureScriptResume_1E040(a1_6E8E, 0);
		break;
	}
	case 0x1ff1c0: {
		CreatureScriptIdle_1E1C0(a1_6E8E, 0);
		break;
	}

	case 0x1ff320: {
		CreatureScriptChaseTarget_1E320(a1_6E8E);
		break;
	}
	case 0x1ff3e0: {
		FindScriptGroupLeader_1E3E0(a1_6E8E);
		break;
	}
	case 0x1ff4d0: {
		CreatureScriptFollowParent_1E4D0(a1_6E8E);
		break;
	}
	case 0x1ff580: {
		CreatureScriptEscortParent_1E580(a1_6E8E, 0);
		break;
	}
	case 0x1ff700: {
		CreatureIdleNoHerd_1E700(a1_6E8E, 0);
		break;
	}

	case 0x1ff9c0: {
		CreatureScriptCmd14_1E9C0(a1_6E8E, 0);
		break;
	}
	case 0x1ffd30: {
		GetCreatureTarget_1ED30(a1_6E8E, 0);
		break;
	}
	case 0x1ffee0: {
		KeepCreatureAboveTerrain_1EEE0(a1_6E8E);
		break;
	}
	case 0x1fff20: {
		FlyingWormIdle_1EF20(a1_6E8E);
		break;
	}
	case 0x1fff40: {
		FlyingWormWander_1EF40(a1_6E8E);
		break;
	}

	case 0x1fff70: {
		FlyingWormAttack_1EF70(a1_6E8E);
		break;
	}
	case 0x1fffd0: {
		FlyingWormFollow_1EFD0(a1_6E8E);
		break;
	}

	case 0x200000: {
		FlyingWormPreKill_1F000(a1_6E8E);
		break;
	}
	case 0x200020: {
		FlyingWormKill_1F020(a1_6E8E);
		break;
	}
	case 0x200040: {
		ApplyFlyingWormGravity_1F040(a1_6E8E);
		break;
	}
	case 0x2000c0: {
		UpdateFlyingWormBody_1F0C0(a1_6E8E);
		break;
	}
	case 0x200300: {
		FlyingWormScripted_1F300(a1_6E8E);
		break;
	}

	case 0x200340: {
		GoatIdle_1F340(a1_6E8E);
		break;
	}
	case 0x2003c0: {
		GoatWander_1F3C0(a1_6E8E);
		break;
	}
	case 0x200440: {
		GoatAttack_1F440(a1_6E8E);
		break;
	}
	case 0x200470: {
		GoatFollow_1F470(a1_6E8E);
		break;
	}
	case 0x2004f0: {//begin of goat kill
		PreKillGoat_1F4F0(a1_6E8E);
		break;
	}

	case 0x200510: {//end of goat kill
		KillGoat_1F510(a1_6E8E);
		break;
	}
	case 0x200530: {
		HitGoat_1F530(a1_6E8E);
		break;
	}
	case 0x2005b0: {
		AddGoat05_01_1F5B0(a1_6E8E);
		break;
	}
	case 0x200630: {
		BeeIdle_1F630(a1_6E8E);
		break;
	}
	case 0x200660: {
		BeeWander_1F660(a1_6E8E);
		break;
	}

	case 0x2006d0: {
		BeeAttack_1F6D0(a1_6E8E);
		break;
	}
	case 0x200800: {
		BeeFollow_1F800(a1_6E8E);
		break;
	}
	case 0x200830: {//maybe bee dead
		BeePreKill_1F830(a1_6E8E);
		break;
	}
	case 0x200850: {
		BeeKill_1F850(a1_6E8E);
		break;
	}
	case 0x2008a0: {
		BeeScripted_1F8A0(a1_6E8E);
		break;
	}

	case 0x200950: {
		WormIdle_1F950(a1_6E8E);
		break;
	}
	case 0x200970: {
		WormWander_1F970(a1_6E8E);
		break;
	}
	case 0x200990: {
		WormAttack_1F990(a1_6E8E);
		break;
	}
	case 0x2009e0: {
		WormFollow_1F9E0(a1_6E8E);
		break;
	}
	case 0x200a00: {
		WormPreKill_1FA00(a1_6E8E);
		break;
	}

	case 0x200a20: {
		WormKill_1FA20(a1_6E8E);
		break;
	}
	case 0x200a50: {
		WormScripted_1FA50(a1_6E8E);
		break;
	}
	case 0x200a70: {
		ArcherIdle_1FA70(a1_6E8E);
		break;
	}
	case 0x200aa0: {
		ArcherWander_1FAA0(a1_6E8E);
		break;
	}
	case 0x200f40: {//get scroll5-help
		AddArcher0504_1FF40(a1_6E8E);
		break;
	}

	case 0x200fe0: {
		ArcherFollow_1FFE0(a1_6E8E);
		break;
	}
	case 0x201010: {
		HitArcher_20010(a1_6E8E);
		break;
	}
	case 0x201040: {
		KillArcher_20040(a1_6E8E);
		break;
	}
	case 0x201060: {
		ArcherStartShooting_20060(a1_6E8E);
		break;
	}
	case 0x2010f0: {
		ArcherResetAnimation_200F0(a1_6E8E);
		break;
	}

	case 0x201140: {//get scroll3
		AddScroll05_04_20140(a1_6E8E);
		break;
	}
	case 0x201370: {
		SkeletonArcherIdle_20370(a1_6E8E);
		break;
	}
	case 0x2013d0: {
		SkeletonArcherWander_203D0(a1_6E8E);
		break;
	}
	case 0x201940: {
		SkeletonArcherUpdate_20940(a1_6E8E);
		break;
	}
	case 0x201c50: {
		SkeletonArcherAttack_20C50(a1_6E8E);
		break;
	}

	case 0x201e50: {
		SkeletonArcherFollow_20E50(a1_6E8E);
		break;
	}
	case 0x201e80: {
		SkeletonArcherPreKill_20E80(a1_6E8E);
		break;
	}
	case 0x201ea0: {
		SkeletonArcherKill_20EA0(a1_6E8E);
		break;
	}
	case 0x201ec0: {
		SkeletonArcherCheckTarget_20EC0(a1_6E8E);
		break;
	}
	case 0x201f20: {
		SkeletonArcherReset_20F20(a1_6E8E);
		break;
	}

	case 0x201f60: {
		SkeletonArcherStartAnimation_20F60(a1_6E8E);
		break;
	}
	case 0x201f80: {
		SkeletonArcherSetCooldown_20F80(a1_6E8E);
		break;
	}
	case 0x201fc0: {
		SkeletonArcherScripted_20FC0(a1_6E8E);
		break;
	}

	case 0x202030: {
		EmuIdle_21030(a1_6E8E);
		break;
	}
	case 0x202490: {
		EmuUpdateState_21490(a1_6E8E);
		break;
	}
	case 0x202850: {
		EmuUpdateMovement_21850(a1_6E8E);

		break;
	}
	case 0x202ab0: {
		EmuFindTarget_21AB0(a1_6E8E);
		break;
	}
	case 0x202f60: {
		EmuCheckNearbyEntities_21F60(a1_6E8E);
		break;
	}
	case 0x203190: {
		EmuCheckDamage_22190(a1_6E8E);
		break;
	}
	case 0x2031f0: {
		SetEntitySpriteAndLoad_221F0(a1_6E8E, 0);
		break;
	}
	case 0x203270: {
		EmuSnapToTerrain_22270(a1_6E8E);
		break;
	}
	case 0x2032b0: {
		EmuFacePlayer_222B0(a1_6E8E);
		break;
	}
	case 0x2033e0: {
		CountWizards_223E0();
		break;
	}
	case 0x203490: {
		ScanTilesAround_22490(a1_6E8E);
		break;
	}
	case 0x203530: {
		EmuWander_22530(a1_6E8E);
		break;
	}
	case 0x203540: {
		EmuAttack_22540(a1_6E8E);
		break;
	}
	case 0x203550: {
		EmuFollow_22550(a1_6E8E);
		break;
	}
	case 0x203560: {
		EmuPreKill_22560(a1_6E8E);
		break;
	}
	case 0x203580: {
		EmuKill_22580(a1_6E8E);
		break;
	}
	case 0x2035a0: {
		EmuHit_225A0(a1_6E8E);
		break;
	}
	case 0x2035b0: {
		EmuScripted_225B0(a1_6E8E);
		break;
	}
	case 0x203640: {
		CanPlaceBuilding_22640((axis_3d*)a1_6E8E, 0, 0, 0);
		break;
	}
				 /*case 0x2036d0: {
					 GetBuildingBitmapSize_226D0((unsigned short)a1_6E8E,0,0);
					 break;
				 }*/
	case 0x203760: {
		BuilderIdle_22760(a1_6E8E);
		break;
	}
	case 0x203c80: {
		BuilderWander_22C80(a1_6E8E);
		break;
	}
	case 0x203e60: {
		BuilderAttack_22E60(a1_6E8E);
		break;
	}
	case 0x204020: {
		BuilderFollow_23020(a1_6E8E);
		break;
	}
	case 0x2041e0: {
		BuilderPreKill_231E0(a1_6E8E);
		break;
	}
	case 0x204200: {
		BuilderKill_23200(a1_6E8E);
		break;
	}
	case 0x204260: {
		BuilderHit_23260(a1_6E8E);
		break;
	}
	case 0x2042a0: {
		BuilderScripted_232A0(a1_6E8E);
		break;
	}
	case 0x2042c0: {
		ChooseRandomBuilding_232C0();
		break;
	}
	case 0x204320: {
		TownieIdle_23320(a1_6E8E);
		break;
	}
	case 0x204340: {
		TownieWander_23340(a1_6E8E);//kill goat?
		break;
	}
	case 0x204640: {
		TownieAttack_23640(a1_6E8E);
		break;
	}
	case 0x204660: {
		TownieFollow_23660(a1_6E8E);
		break;
	}
	case 0x204680: {//mana posses
		KillTownie_23680(a1_6E8E);
		break;
	}
	case 0x2046f0: {
		TownieKill_236F0(a1_6E8E);
		break;
	}
	case 0x204710: {
		HitTownie_23710(a1_6E8E);
		break;
	}
	case 0x204750: {
		AddTownie05_0D_23750(a1_6E8E);
		break;
	}
	case 0x204790: {
		TraderIdle_23790(a1_6E8E);
		break;
	}
	case 0x2047b0: {
		TraderWander_237B0(a1_6E8E);
		break;
	}
	case 0x204ac0: {
		TraderAttack_23AC0(a1_6E8E);
		break;
	}
	case 0x204ae0: {
		TraderFollow_23AE0(a1_6E8E);
		break;
	}
	case 0x204b00: {
		TraderPreKill_23B00(a1_6E8E);
		break;
	}
	case 0x204b30: {
		TraderKill_23B30(a1_6E8E);
		break;
	}
	case 0x204b90: {//kill human with bag
		TraderHit_23B90(a1_6E8E);
		break;
	}
	case 0x204bd0: {
		TraderScripted_23BD0(a1_6E8E);
		break;
	}
	case 0x204c20: {
		ExplodedHumanIdle_23C20(a1_6E8E);
		break;
	}
	case 0x204c40: {
		ExplodedHumanWander_23C40(a1_6E8E);
		break;
	}
	case 0x204e60: {
		ExplodedHumanAttack_23E60(a1_6E8E);
		break;
	}

	case 0x2050a0: {
		ExplodedHumanFollow_240A0(a1_6E8E);
		break;
	}
	case 0x2050c0: {
		ExplodedHumanPreKill_240C0(a1_6E8E);
		break;
	}
	case 0x2050e0: {
		ExplodedHumanKill_240E0(a1_6E8E);
		break;
	}
	case 0x205100: {
		ExplodedHumanStartAttack_24100(a1_6E8E);
		break;
	}
	case 0x205150: {
		ExplodedHumanResetAnimation_24150(a1_6E8E);
		break;
	}

	case 0x205190: {
		ExplodedHumanExplode_24190(a1_6E8E);
        break; // FIXME: this break was missing here - was this a mistake?
	}
	case 0x205400: {
		ExplodedHumanScripted_24400(a1_6E8E);
		break;
	}
	case 0x205420: {
		WyvernIdle_24420(a1_6E8E);
		break;
	}
	case 0x205440: {
		WyvernWander_24440(a1_6E8E);
		break;
	}
	case 0x205510: {
		WyvernAttack_24510(a1_6E8E);
		break;
	}

	case 0x2057d0: {
		WyvernFollow_247D0(a1_6E8E);
		break;
	}
	case 0x2057f0: {
		WyvernPreKill_247F0(a1_6E8E);
		break;
	}
	case 0x205810: {
		WyvernKill_24810(a1_6E8E);
		break;
	}
	case 0x205840: {
		WyvernScripted_24840(a1_6E8E);
		break;
	}
	case 0x205860: {
		LionIdle_24860(a1_6E8E);
		break;
	}

	case 0x2058c0: {
		LionWander_248C0(a1_6E8E);
		break;
	}
	case 0x205930: {
		LionAttack_24930(a1_6E8E);
		break;
	}
	case 0x205d40: {
		LionFollow_24D40(a1_6E8E);
		break;
	}
	case 0x205da0: {
		LionPreKill_24DA0(a1_6E8E);
		break;
	}
	case 0x205dc0: {
		LionKill_24DC0(a1_6E8E);
		break;
	}

	case 0x205df0: {
		LionScripted_24DF0(a1_6E8E);
		break;
	}
	case 0x205e20: {
		HeadIdle_24E20(a1_6E8E);
		break;
	}
	case 0x206050: {
		HeadWander_25050(a1_6E8E);
		break;
	}
	case 0x2060b0: {
		HeadAttack_250B0(a1_6E8E);
		break;
	}
	case 0x206280: {
		HeadFollow_25280(/*a1_6E8E,*/a1_6E8E);
		break;
	}

	case 0x2062a0: {
		HeadPreKill_252A0(a1_6E8E);
		break;
	}
	case 0x2062c0: {
		HeadKill_252C0(a1_6E8E);
		break;
	}
	case 0x2062e0: {
		HeadCheckDamage_252E0(a1_6E8E);
		break;
	}
	case 0x2063b0: {
		HeadSetState_253B0(a1_6E8E, 0, 0);
		break;
	}
	case 0x2064e0: {
		TurnTowardsEntity_254E0(a1_6E8E, 0, 0);
		break;
	}

	case 0x206550: {
		HeadScripted_25550(a1_6E8E);
		break;
	}
	case 0x206590: {
		FirebugIdle_25590(a1_6E8E);
		break;
	}
	case 0x2065c0: {
		FirebugWander_255C0(a1_6E8E);
		break;
	}
	case 0x206610: {
		HitFirebug_25610(a1_6E8E);
		break;
	}
	case 0x206cd0: {
		FirebugFollow_25CD0(a1_6E8E);
		break;
	}

	case 0x206d00: {
		FirebugPreKill_25D00(a1_6E8E);
		break;
	}
	case 0x206d20: {
		FirebugKill_25D20(a1_6E8E);
		break;
	}
	case 0x206d50: {
		AddFirebug05_13_25D50(a1_6E8E);
		break;
	}
	case 0x206d80: {
		SpiderIdle_25D80(a1_6E8E);
		break;
	}
	case 0x206de0: {
		SpiderWander_25DE0(a1_6E8E);
		break;
	}

	case 0x206e40: {
		SpiderAttack_25E40(a1_6E8E);
		break;
	}
	case 0x206f70: {
		SpiderFollow_25F70(a1_6E8E);
		break;
	}
	case 0x206fd0: {
		SpiderPreKill_25FD0(a1_6E8E);
		break;
	}
	case 0x206ff0: {
		SpiderKill_25FF0(a1_6E8E);
		break;
	}
	case 0x207020: {
		SpiderScripted_26020(a1_6E8E);
		break;
	}

	case 0x207050: {
		DiabloIdle_26050(a1_6E8E);
		break;
	}
	case 0x207070: {
		DiabloWander_26070(a1_6E8E);
		break;
	}
	case 0x207220: {
		DiabloAttack_26220(a1_6E8E);
		break;
	}
	case 0x2073c0: {
		DiabloFollow_263C0(a1_6E8E);
		break;
	}
	case 0x2073e0: {
		DiabloPreKill_263E0(a1_6E8E);
		break;
	}

	case 0x207400: {
		DiabloKill_26400(a1_6E8E);
		break;
	}
	case 0x207470: {
		DiabloScripted_26470(a1_6E8E);
		break;
	}
	case 0x207500: {
		DiabloSetSprite_26500(a1_6E8E);
		break;
	}
	case 0x2075a0: {
		DiabloUpdateState_265A0(a1_6E8E);
		break;
	}
	case 0x207830: {
		DiabloCheckDamage_26830(a1_6E8E);
		break;
	}

	case 0x2078f0: {
		DiabloSetMode_268F0(a1_6E8E, 0);
		break;
	}
	case 0x207930: {
		DiabloIsAttacking_26930(a1_6E8E);
		break;
	}
	case 0x207960: {
		FlyingManaIdle_26960(a1_6E8E);
		break;
	}
	case 0x207990: {
		FlyingManaWander_26990(a1_6E8E);
		break;
	}
	case 0x207aa0: {
		FlyingManaAttack_26AA0(a1_6E8E);
		break;
	}

	case 0x207bd0: {
		FlyingManaFollow_26BD0(a1_6E8E);
		break;
	}
	case 0x207ca0: {
		FlyingManaPreKill_26CA0(a1_6E8E);
		break;
	}
	case 0x207cc0: {
		FlyingManaKill_26CC0(a1_6E8E);
		break;
	}
	case 0x207d20: {
		FlyingManaFollowTarget_26D20(a1_6E8E);
		break;
	}
	case 0x207f10: {
		FlyingManaApplyDamage_26F10(a1_6E8E);
		break;
	}
	case 0x207ff0: {
		FlyingManaUpdateSpeed_26FF0(a1_6E8E);
		break;
	}

	case 0x208120: {
		FlyingManaAvoidOthers_27120(a1_6E8E);
		break;
	}
	case 0x2081d0: {
		FlyingManaSteerToTarget_271D0(a1_6E8E);
		break;
	}
	case 0x2082c0: {
		FlyingManaUpdateAnimation_272C0(a1_6E8E);
		break;
	}
	case 0x208470: {
		GetChainSegment_27470(a1_6E8E, 0);
		break;
	}
	case 0x208590: {
		SetFlyingManaSprite_27590(a1_6E8E);
		break;
	}

	case 0x208610: {
		SetFlyingManaChainSize_27610(a1_6E8E);
		break;
	}
	case 0x2086e0: {
		UpdateChainSegments_276E0(a1_6E8E);
		break;
	}
	case 0x208720: {
		SetFlyingManaLevel_27720(a1_6E8E, 0);
		break;
	}
	case 0x208880: {
		FlyingManaCountdown_27880(a1_6E8E);
		break;
	}
	case 0x208930: {
		FlyingManaScripted_27930(a1_6E8E);
		break;
	}

	case 0x208950: {
		BalloonIdle_27950(a1_6E8E);
		break;
	}
	case 0x208b20: {
		BalloonWander_27B20(a1_6E8E);
		break;
	}

	case 0x208c10: {
		BalloonFollow_27C10(a1_6E8E);
		break;
	}
	case 0x208e00: {
		BalloonAttack_27E00(a1_6E8E);
		break;
	}
	case 0x208fa0: {
		BalloonPreKill_27FA0(a1_6E8E);
		break;
	}
	case 0x208fc0: {
		BalloonKill_27FC0(a1_6E8E);
		break;
	}
	case 0x208fe0: {
		SetEntityActionState_27FE0(a1_6E8E, 0, 0, 0);
		break;
	}

	case 0x209000: {
		FindNearestCastle_28000(a1_6E8E);
		break;
	}
	case 0x209060: {
		IsTooCloseToSameModel_28060(a1_6E8E);
		break;
	}
	case 0x209110: {
		BalloonCheckDamage_28110(a1_6E8E);
		break;
	}
	case 0x2092d0: {
		FindBalloonTarget_282D0(a1_6E8E);
		break;
	}
	case 0x209390: {
		MoveTowardsEntity_28390(a1_6E8E, 0);
		break;
	}

	case 0x209420: {
		GetBalloonTarget_28420(a1_6E8E);
		break;
	}
	case 0x209470: {
		BalloonScripted_28470(a1_6E8E);
		break;
	}
	case 0x209490: {
		Creature24Idle_28490(a1_6E8E);
		break;
	}
	case 0x209500: {
		Creature24Wander_28500(a1_6E8E);
		break;
	}
	case 0x209570: {
		Creature24Attack_28570(a1_6E8E);
		break;
	}

	case 0x2095d0: {
		Creature24Follow_285D0(a1_6E8E);
		break;
	}
	case 0x2095f0: {
		Creature24PreKill_285F0(a1_6E8E);
		break;
	}
	case 0x209610: {
		Creature24Kill_28610(a1_6E8E);
		break;
	}
	case 0x209630: {
		Creature24Hit_28630(a1_6E8E);
		break;
	}
	case 0x209660: {
		Creature24Scripted_28660(a1_6E8E);
		break;
	}

	case 0x209690: {
		Creature24FindTarget_28690(a1_6E8E);
		break;
	}
	case 0x2097b0: {
		Creature24SetSprite_287B0(a1_6E8E);
		break;
	}
	case 0x209860: {
		FlyingCreatureIdle_28860(a1_6E8E);
		break;
	}
	case 0x209c30: {
		FlyingCreatureWander_28C30(a1_6E8E);
		break;
	}
	case 0x209c60: {
		FlyingCreatureAttack_28C60(a1_6E8E);
		break;
	}

	case 0x209cc0: {
		FlyingCreatureFollow_28CC0(a1_6E8E);
		break;
	}
	case 0x209ce0: {
		FlyingCreaturePreKill_28CE0(a1_6E8E);
		break;
	}
	case 0x209ec0: {
		FlyingCreatureKill_28EC0(a1_6E8E);
		break;
	}
	case 0x209f50: {
		FlyingCreatureScripted_28F50(a1_6E8E);
		break;
	}
	case 0x209f90: {
		MummyIdle_28F90(a1_6E8E);
		break;
	}
	case 0x209fc0: {
		MummyWander_28FC0(a1_6E8E);
		break;
	}
	case 0x209ff0: {
		MummyAttack_28FF0(a1_6E8E);
		break;
	}

	case 0x20a300: {
		MummyFollow_29300(a1_6E8E);
		break;
	}
	case 0x20a330: {
		MummyPreKill_29330(a1_6E8E);
		break;
	}
	case 0x20a350: {
		MummyKill_29350(a1_6E8E);
		break;
	}
	case 0x20a380: {
		MummyScripted_29380(a1_6E8E);
		break;
	}
	case 0x20a3b0: {
		MummyResetAttackFlag_293B0(a1_6E8E);
		break;
	}

	case 0x20a3d0: {
		MummySetAttackFlag_293D0(a1_6E8E);
		break;
	}
	case 0x20a400: {
		BigDragonIdle_29400(a1_6E8E);
		break;
	}
	case 0x20a670: {
		BigDragonWander_29670(a1_6E8E);
		break;
	}
	case 0x20a710: {
		BigDragonAttack_29710(a1_6E8E);
		break;
	}
	case 0x20a890: {
		BigDragonFollow_29890(a1_6E8E);
		break;
	}

	case 0x20a8b0: {
		BigDragonPreKill_298B0(a1_6E8E);
		break;
	}
	case 0x20a8d0: {
		BigDragonKill_298D0(a1_6E8E);
		break;
	}
	case 0x20a930: {
		BigDragonScripted_29930(a1_6E8E);
		break;
	}
	case 0x20aa90: {
		BigDragonUpdateBody_29A90(a1_6E8E);
		break;
	}
	case 0x20b340: {
		BigDragonMoveSegment_2A340(a1_6E8E);
		break;
	}

	case 0x20b5b0: {
		BigDragonPlaceSegment_2A5B0(a1_6E8E, 0, 0);
		break;
	}
	case 0x20b660: {
		BigDragonForwardDamage_2A660(a1_6E8E, 0);
		break;
	}
	case 0x20b6b0: {
		BigDragonCheckDamage_2A6B0(a1_6E8E);
		break;
	}
	case 0x20b6f0: {
		BigDragonFindTarget_2A6F0(a1_6E8E);
		break;
	}
	case 0x20b7b0: {
		BigDragonGetTarget_2A7B0(a1_6E8E);
		break;
	}

	case 0x20b7f0: {
		BigDragonChooseAttack_2A7F0(a1_6E8E, 0, 0);
		break;
	}
	case 0x20b940: {
		BigDragonSetSegmentPose_2A940(a1_6E8E, 0);
		break;
	}
	case 0x20b9f0: {
		BigDragonShootFromSegment_2A9F0(a1_6E8E, 0);
		break;
	}
	case 0x20ba90: {
		BigDragonAttachSegment_2AA90(a1_6E8E, 0);
		break;
	}
	case 0x20bc50: {
		BigDragonResetRotation_2AC50(a1_6E8E);
		break;
	}

	case 0x20bd40: {
		BigDragonCheckSegmentsDead_2AD40(a1_6E8E);
		break;
	}
	case 0x20be30: {
		BigDragonKillSegments_2AE30(a1_6E8E);
		break;
	}
	case 0x20be80: {
		BigDragonHideSegments_2AE80(a1_6E8E);
		break;
	}
	case 0x20bed0: {
		SetEntitySpriteIfChanged_2AED0(a1_6E8E, 0);
		break;
	}
	case 0x20bf10: {
		MoveDragonEntity_2AF10(a1_6E8E, 0);
		break;
	}

	case 0x20c1d0: {
		ManaIdle_2B1D0(a1_6E8E);
		break;
	}
	case 0x20c200: {
		ManaWander_2B200(a1_6E8E);
		break;
	}
	case 0x20c260: {
		ManaAttack_2B260(a1_6E8E);
		break;
	}
	case 0x20c750: {
		ManaFollow_2B750(a1_6E8E);
		break;
	}
	case 0x20c760: {
		ManaPreKill_2B760(a1_6E8E);
		break;
	}

	case 0x20c780: {
		ManaKill_2B780(a1_6E8E);
		break;
	}
	case 0x20c7b0: {
		ManaScripted_2B7B0(a1_6E8E);
		break;
	}
	case 0x20c7e0: {
		IsAnyCreature28Alive_2B7E0(a1_6E8E);
		break;
	}
	case 0x20c840: {
		ManaStartAttack_2B840(a1_6E8E);
		break;
	}
	case 0x20c860: {
		ManaSetMode_2B860(a1_6E8E, 0);
		break;
	}

	case 0x20c9a0: {
		ManaCheckDamage_2B9A0(a1_6E8E);
		break;
	}
	case 0x20ca50: {
		ManaSetState_2BA50(a1_6E8E, 0);
		break;
	}
				 /*case 0x20cb40: {
					 DrawBitmap_2BB40((int16_t)a1,0,(bitmap_pos_struct_t)0);

					 break;
				 }*/
				 /*case 0x20cbb0: {
					 DrawBitmapToScreenAndBuffer_2BBB0((short)a1,0, (bitmap_pos_struct_t)0);

					 break;
				 }*/
	case 0x20cc10: {
		DrawText_2BC10((char*)a1_6E8E, 0, 0, 0);
		break;
	}
	case 0x20cc80: {
#if defined(__linux__) || defined(__APPLE__) // FIXME: types
		std::cout << "FIXME: types @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
		DrawLine_2BC80((uint16_t)a1_6E8E, 0, 0, 0, 0);
		stub_fix_it();//bad retyping
#endif
		break;
	}

	case 0x211be0: {
		GetOkayCancelButtonPositions_30BE0((int16_t*)a1_6E8E, 0);
		break;
	}
	case 0x211d50: {
		UpdateEffect0A_00_30D50(a1_6E8E);//0xa0 0x00 pos 0x4a 0xdc
		break;
	}
	case 0x211f60: {
		AddQuickfair0A_01_30F60(a1_6E8E);
		break;
	}
	case 0x212100: {
		CastSpeedSpell_31100(a1_6E8E);
		break;
	}
	case 0x212120: {
		UpdateSpeedTrail_31120(a1_6E8E);
		break;
	}

	case 0x2121e0: {
		UpdateSpeedSpell_311E0(a1_6E8E);
		break;
	}
	case 0x212760: {
		UpdateEffect0A_06_31760(a1_6E8E);
		break;
	}
	case 0x212870: {
		UpdateEffect0A_07_31870(a1_6E8E);
		break;
	}
	case 0x212890: {
		UpdateEffect0A_03_31890(a1_6E8E);
		break;
	}
	case 0x2128b0: {//end of water splash
		AddAsh0A_05_318B0(a1_6E8E);
		break;
	}

	case 0x212920: {
		UpdateEffect0A_08_31920(a1_6E8E);
		break;
	}
	case 0x212940: {
		UpdateEffect0A_09_31940(a1_6E8E);
		break;
	}
	case 0x212e90: {
		UpdateEffect0A_0A_31E90(a1_6E8E);
		break;
	}
	case 0x212f00: {
		SpawnEffectAroundTile_31F00(a1_6E8E, 0, 0);
		break;
	}
	case 0x212fb0: {
		UpdateEffect0A_0B_31FB0(a1_6E8E);
		break;
	}

	case 0x2130e0: {
		PossesHitMana_320E0(a1_6E8E);//posses spell drop
		//debug
		//AddCastle03_02_4AA40(a1);//castle create
		//sub_5FA70(a1);//castle create2
		//debug
		break;
	}
	case 0x213120: {
		UpdateEffectLifetime_32120(a1_6E8E);
		break;
	}
	case 0x213160: {//in quest point3
		UpdateEffect0A_0D_32160(a1_6E8E);//0x0A 0x0D pos 0x72 0xd4
		break;
	}
	case 0x2132a0: {
		UpdateEffect0A_0E_322A0(a1_6E8E);//0x0a 0x0e pos 0x4e 0da
		break;
	}
	case 0x2133e0: {//in quest point2
		AddParticleSmoke0A_3B_323E0(a1_6E8E);
		break;
	}

	case 0x213400: {
		AddParticleSmoke0A_3C_32400(a1_6E8E);
		break;
	}
	case 0x213420: {
		AddParticleSmoke0A_3D_32420(a1_6E8E);
		break;
	}
	case 0x213520: {
		break;
	}
	case 0x213530: {
		UpdateEffect0A_0F_32530(a1_6E8E);
		break;
	}
	case 0x213600: {
		UpdateEffect0A_10_32600(a1_6E8E);
		break;
	}
	case 0x213880: {
		UpdateMeteor_32880(a1_6E8E);
		break;
	}

	case 0x213a70: {
		UpdateEffect0A_12_32A70(a1_6E8E);
		break;
	}
	case 0x213cf0: {
		SpawnManaSpheres_32CF0(a1_6E8E);
		break;
	}
	case 0x213f40: {
		UpdateEffect0A_13_32F40(a1_6E8E);
		break;
	}
	case 0x214110: {
		UpdateWind_33110(a1_6E8E);
		break;
	}
	case 0x2141a0: {
		UpdateWindMovement_331A0(a1_6E8E);
		break;
	}

	case 0x214340: {
		UpdateWindPull_33340(a1_6E8E);
		break;
	}
	case 0x214710: {
		WindKillOverlapping_33710(a1_6E8E);
		break;
	}
	case 0x214810: {
		CanWindAffectEntity_33810(a1_6E8E, 0);
		break;
	}
	case 0x2148d0: {
		WindPullNearbyEntities_338D0(a1_6E8E);
		break;
	}
	case 0x2149b0: {
		UpdateWhirlwindState_339B0(a1_6E8E);
		break;
	}

	case 0x214ad0: {
		UpdateWhirlwindSpeed_33AD0(a1_6E8E);
		break;
	}
	case 0x214b20: {
		SpinEntity_33B20(a1_6E8E);
		break;
	}
	case 0x214c00: {
		UpdateWhirlwindSegments_33C00(a1_6E8E);
		break;
	}
	case 0x214c70: {
		UpdateWhirlwindTarget_33C70(a1_6E8E);
		break;
	}
	case 0x214d40: {
		RemoveWhirlwindSegments_33D40(a1_6E8E);
		break;
	}

	case 0x214d80: {
		UpdateEffect0A_17_33D80(a1_6E8E);
		break;
	}
	case 0x214e20: {
		UpdateEffect0A_19_33E20(a1_6E8E);
		break;
	}
	case 0x214e80: {
		UpdateEffect0A_1A_33E80(a1_6E8E);
		break;
	}
	case 0x214f70: {
#ifdef TEST_x64
	allert_error();
#endif
#ifdef COMPILE_FOR_64BIT // FIXME: 64bit
  std::cout << "FIXME: 64bit @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
		CheckTileType8Neighbours_33F70((intptr_t)a1_6E8E);
		allert_error();
#endif
		break;
	}
	case 0x215000: {
		UpdateEffect0A_1C_34000(a1_6E8E);
		break;
	}

	case 0x215110: {
		UpdateEffect0A_1B_34110(a1_6E8E);
		break;
	}
	case 0x215210: {
		UpdateEffect0A_1D_34210(a1_6E8E);
		break;
	}
	case 0x215330: {
		UpdateEffect0A_1E_34330(a1_6E8E);
		break;
	}
	case 0x215350: {
		UpdateEffect0A_1F_34350(a1_6E8E);
		break;
	}
	case 0x215370: {
		break;
	}
	case 0x215380: {
		break;
	}
	case 0x215390: {
		SetLinkedEntityLife2AndHide_34390(a1_6E8E);
		break;
	}
	case 0x2153c0: {
		SetLinkedEntityLife1AndHide_343C0(a1_6E8E);
		break;
	}

	case 0x2153f0: {//2 instances in level 1
		ApplyPointToPath_343F0(a1_6E8E);
		break;
	}

	case 0x215480: {
		UpdateEffect0A_21_34480(a1_6E8E);
		break;
	}
	case 0x2154a0: {
		UpdateEffect0A_22_344A0(a1_6E8E);
		break;
	}
	case 0x215520: {
		HideEntity_34520(a1_6E8E);
		break;
	}
	case 0x215540: {
		UpdateArcProjectile_34540(a1_6E8E);
		break;
	}
	case 0x215910: {
		UpdateTerrainDeform_34910(a1_6E8E);
		break;
	}

	case 0x215b00: {
#if defined(__linux__) || defined(__APPLE__) // FIXME: types
		std::cout << "FIXME: types @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
		UpdateTileAngles_34B00((intptr_t)a1_6E8E, 0, 0, 0);
		stub_fix_it();//bad retyping
#endif
		break;
	}
	case 0x215c40: {
		UpdateCraterEffect_34C40(a1_6E8E);
		break;
	}
	case 0x215ee0: {
		UpdateVolcanoEffect_34EE0(a1_6E8E);
		break;
	}
	case 0x2162a0: {
		HideEntity2_352A0(a1_6E8E);
		break;
	}
	case 0x2162c0: {
		UpdateEffectUntilWater_352C0(a1_6E8E);
		break;
	}

	case 0x216390: {
		UpdateEffect0A_24_35390(a1_6E8E);
		break;
	}
	case 0x216530: {
		UpdateEffect0A_25_35530(a1_6E8E);

		break;
	}
	case 0x216600: {
		UpdateEffect0A_23_35600(a1_6E8E);
		break;
	}
	case 0x216640: {//lighting II
		UpdateEffect0A_28_35640(a1_6E8E);
		break;
	}
	case 0x2167c0: {
		UpdateEffect0A_26_357C0(a1_6E8E);
		break;
	}

	case 0x216940: {//end of mana sphere making(move sphere)
		TransformArcherToMana_35940(a1_6E8E);
		break;
	}
	case 0x216FB0: {
		UpdateManaSphere_35FB0(a1_6E8E);
		break;
	}
	case 0x217680: {
		ManaSphereCheckCollector_36680(a1_6E8E);
		break;
	}
	case 0x217770: {
		ManaSphereShootFireball_36770(a1_6E8E);
		break;
	}

	case 0x217850: {
		ManaSphereShootProjectile9_36850(a1_6E8E);
		break;
	}
	case 0x217920: {
		SetManaSphereColorAndRot_36920(a1_6E8E);
		break;
	}
	case 0x2179f0: {
#ifdef TEST_x64
	allert_error();
#endif
#ifdef COMPILE_FOR_64BIT // FIXME: 64bit
  std::cout << "FIXME: 64bit @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
		GetManaSphereColorIndexFromEntityId_369F0((intptr_t)a1_6E8E);
		allert_error();
#endif
		break;
	}
	case 0x217a50: {
#ifdef TEST_x64
	allert_error();
#endif
#ifdef COMPILE_FOR_64BIT // FIXME: 64bit
  std::cout << "FIXME: 64bit @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
		GetManaSphereIndexFromId_36A50((intptr_t)a1_6E8E);
		allert_error();
#endif
		break;
	}
	case 0x217ae0: {//enemy dead
		UpdateEffect0A_2A_36AE0(a1_6E8E);
		break;
	}

	case 0x217ba0: {
		TransformEntityToManaSphere_36BA0(a1_6E8E, false);
		break;
	}
	case 0x217d50: {
		MergeManaSpheres_36D50(a1_6E8E, 0);
		break;
	}
	case 0x217f30: {
		ManaSphereCollide_36F30(a1_6E8E, 0);
		break;
	}
	case 0x217fc0: {
		UpdateManaSphereMovement_36FC0(a1_6E8E);
		break;
	}
	case 0x218240: {//255 instances in level 1
		ApplyTerrainModification_37240(a1_6E8E);
		break;
	}

	case 0x218740: {
		IsNextEvent0A_2A_37740(a1_6E8E);
		break;
	}
	case 0x2187a0: {
		DamageOverlappingWizards_377A0(a1_6E8E);
		break;
	}
	case 0x2187f0: {
		UpdateEffect0A_2B_377F0(a1_6E8E);
		break;
	}
	case 0x218bc0: {
		AddTerrainMod0A_2A_37BC0(a1_6E8E);
		break;
	}
	case 0x219270: {
		GetRandManaSphere_38270(a1_6E8E);
		break;
	}

	case 0x219330: {
		AddHouse0A_2D_38330(a1_6E8E);
		break;
	}
	case 0x2195c0: {
		RemoveCastleStage_385C0(a1_6E8E);
		break;
	}
	case 0x2199f0: {//cast castleII 2
		UpdateProjectileLife_389F0(a1_6E8E);
		break;
	}
	case 0x219af0: {
		break;
	}
	case 0x219b00: {
		CompareEvent08_38B00(a1_6E8E);
		break;
	}
	case 0x219b70: {
		break;
	}
	case 0x219b80: {
		break;
	}

	case 0x219b90: {
		UpdateProjectileTrail_38B90(a1_6E8E);
		break;
	}
	case 0x219d80: {
		UpdateProjectileTimeout_38D80(a1_6E8E);
		break;
	}
	case 0x219e20: {
		HideProjectile_38E20(a1_6E8E);
		break;
	}
	case 0x219e40: {
		HideProjectile2_38E40(a1_6E8E);
		break;
	}
	case 0x219e60: {
		break;
	}
	case 0x219e70: {
		SlowDownWizard_38E70(a1_6E8E);
		break;
	}

	case 0x219f70: {
		MobilizeWizard_38F70(a1_6E8E);
		break;
	}
	case 0x21a040: {
		UpdateLightning_39040(a1_6E8E);
		break;
	}
	case 0x21a6a0: {
		UpdateEarthquake_396A0(a1_6E8E);
		break;
	}
	case 0x21a6d0: {
		UpdateTremorTiles_396D0(a1_6E8E);
		break;
	}
	case 0x21ab60: {
		ApplyEarthquakeTiles_39B60(a1_6E8E);
		break;
	}

	case 0x21ae40: {
		FindTremorTarget_39E40(a1_6E8E);
		break;
	}
	case 0x21afa0: {
		CanSpellAffectEntity_39FA0(a1_6E8E, 0);//fix it
		break;
	}
	case 0x21b909: {
		KillOverlappingEntities_3A090(a1_6E8E);
		break;
	}
	case 0x21b200: {
		ApplySpellToEntity_3A200(a1_6E8E, 0);
		break;
	}
	case 0x21b2d0: {
		UpdateShield_3A2D0(a1_6E8E);
		break;
	}

	case 0x21b5b0: {
		UpdateEffectFade_3A5B0(a1_6E8E);
		break;
	}
	case 0x21b630: {
		UpdateEffectCountdown_3A630(a1_6E8E);
		break;
	}
	case 0x21b650: {
		FollowTargetEntity_3A650(a1_6E8E);
		break;
	}
	case 0x21b7f0: {
		IsCreatureAffectable_3A7F0(a1_6E8E);
		break;
	}
	case 0x21b8b0: {
		UpdatePossessedCreature_3A8B0(a1_6E8E);
		break;
	}

	case 0x21bf00: {
		CastleDefendEvent_3AF00(a1_6E8E);
		break;
	}

	case 0x232bb0: {
		PlayerEvents_51BB0();
		break;
	}
	case 0x233d70: {
#if defined(__linux__) || defined(__APPLE__) // FIXME: types
		std::cout << "FIXME: types @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
		ShowMessage_52D70((unsigned short)a1_6E8E, 0);
		stub_fix_it();//bad retyping
#endif
		break;
	}
	case 0x233e90: {
		//SetMenuCursorPosition_52E90((type_str_0x2BDE*)a1_6E8E, 0, false);
		allert_error();
		break;
	}
	case 0x234120: {
		CheckCdPresent_53120();
		break;
	}
	case 0x234160: {
		UpdateGameFrame_53160();
		break;
	}
	case 0x2343b0: {
#if defined(__linux__) || defined(__APPLE__) // FIXME: types
		std::cout << "FIXME: types @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
		LevelDecompress_533B0((short)a1_6E8E, 0);
		stub_fix_it();//bad retyping
#endif
		break;
	}
	case 0x234590: {
		SetLevelId_53590((Type_Level_2FECE*)a1_6E8E);
		break;
	}

				 /*
				 char GetObjectiveStage_595C0(__int16 a1)//23a5c0
				 char GetObjectiveTargetEntity_59610(uint8_t** a1, __int16 a2)//23a610
				 char GetObjectiveTargetPosition_596C0(x_WORD *a1, __int16 a2)//23a6c0
				 void CheckObjectiveKill_59760(uint8_t* a1, uint8_t* a2)//23a760
				 void sub_59820()//23a820

				 int FadeDownSoundVolume_59A50()//23aa50
				 void StopCdPlayBackAndFadeUp_59AF0()//23aaf0
				 void sub_59B50_sound_proc10(HMDIDRIVER user)//23ab50
				 void sub_59BF0_sound_proc11_volume()//23abf0
				 */

	case 0x23ac40: {
		SnapEntityToTerrain_59C40(a1_6E8E);
		break;
	}
	case 0x23ac60: {
		SnapEntityToTerrain2_59C60(a1_6E8E);
		break;
	}
	case 0x23ac80: {
		UpdateScroll_59C80(a1_6E8E);
		break;
	}
	case 0x23ad90: {
		break;
	}
	case 0x23adc0: {
		CheckParentAlive_59DC0(a1_6E8E);
		break;
	}
	case 0x23af60: {
		UpdateCastle_59F60(a1_6E8E);
		break;
	}

	case 0x23c070: {
		FindEntityOnTile_5B070(a1_6E8E);
		break;
	}
	case 0x23c100: {
		UpdateCastleBuilding_5B100(a1_6E8E);
		break;
	}

	case 0x23f010: {
		AddPlayer03_00_5E010(a1_6E8E);
		break;
	}
	case 0x23f310: {
		MultiplayerTestDie_5E310(a1_6E8E);//enemy player die
		break;
	}
	case 0x23f660: {
		DisableEntitesDrawing_5E660(a1_6E8E);
		break;
	}
	case 0x23f6c0: {
		CheckWizardKiller_5E6C0(a1_6E8E);
		break;
	}

	case 0x23f7c0: {
		MultiplayerTestBanished_5E7C0(a1_6E8E);
		break;
	}
	case 0x23f8c0: {
		EndGameSequence_5E8C0(a1_6E8E);
		break;
	}
	case 0x23ff70: {
		SetWizardHitTimer_5EF70(a1_6E8E);
		break;
	}
	case 0x23ffa0: {
		UpdateCastleTarget_5EFA0(a1_6E8E);
		break;
	}

	case 0x240380: {
		UpdateWizardSpeed_5F380(a1_6E8E);
		break;
	}
	case 0x240660: {
		CastleAttackEntity_5F660(a1_6E8E, 0, 0);
		break;
	}
	case 0x2407b0: {
		SetCastleCooldown_5F7B0(a1_6E8E, 0, 0);
		break;
	}
	case 0x2407e0: {
		ClearEntityVisibleFlag_5F7E0(a1_6E8E, 0);
		break;
	}
	case 0x240810: {
		FindNearestEnemyBalloon_5F810(a1_6E8E, 0, 0);
		break;
	}

	case 0x240890: {
		UpdateCastleLevel_5F890(a1_6E8E, 0);
		break;
	}
	case 0x2408f0: {
		EndOfCastleProjectile_5F8F0(a1_6E8E);
		break;
	}
	case 0x240a70: {
		BeginOfCastleCreation_5FA70(a1_6E8E);//castle create2
		break;
	}
	case 0x240bd0: {
		SpawnCastleEffect42_5FBD0(a1_6E8E);
		break;
	}
	case 0x240c40: {
		SpawnCastleEffect41_5FC40(a1_6E8E);
		break;
	}

	case 0x240ca0: {
		DestroyCastleLevel_5FCA0(a1_6E8E);
		break;
	}
	case 0x240d00: {
		TransferCastleMana_5FD00(a1_6E8E);
		break;
	}
	case 0x240f50: {
		UpdateCastleStructure_5FF50(a1_6E8E);
		break;
	}
	case 0x241400: {
#if defined(__linux__) || defined(__APPLE__) // FIXME: types
		std::cout << "FIXME: types @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
        allert_error();
#else
		GetCastleLevelSize_60400((short)a1_6E8E, 0, 0);
		stub_fix_it();//bad retyping
#endif
		break;
	}
	case 0x241480: {
		SpawnCastleBuildEffect_60480(a1_6E8E);
		break;
	}

	case 0x2415e0: {
		ShrinkCastleMana_605E0(a1_6E8E);
		break;
	}
	case 0x241780: {
		SetCastleLifeAndMana_60780(a1_6E8E, 0, 0, 0);
		break;
	}
	case 0x241810: {
		GrowCastle_60810(a1_6E8E);
		break;
	}
	case 0x2419e0: {
		CheckCastleDestroyed_609E0(a1_6E8E);
		break;
	}
	case 0x241a90: {
		break;
	}
	case 0x241aa0: {
		break;
	}
	case 0x241ab0: {
		AddBallon_60AB0(a1_6E8E);
		break;
	}

	case 0x241d50: {
		SpawnCastleRubble_60D50((axis_3d*)a1_6E8E, 0);
		break;
	}
	case 0x241e90: {
		break;
	}
	case 0x241ea0: {
		ApplyCastleDamage_60EA0(a1_6E8E);
		break;
	}
	case 0x241f00: {
		UpdateCastles_60F00();
		break;
	}
	case 0x242000: {
		AddBalloonManaToCastle_61000(a1_6E8E);
		break;
	}
	case 0x241040: {
		break;
	}
	case 0x242050: {
		UpdateBalloonCollection_61050(a1_6E8E);
		break;
	}

	case 0x2423d0: {
		UpdateBalloonChain_613D0(a1_6E8E);
		break;
	}
	case 0x242620: {
		FindNearestManaForBalloon_61620(a1_6E8E, 0);
		break;
	}
	case 0x2426d0: {
#ifdef TEST_x64
	allert_error();
#endif
#ifdef COMPILE_FOR_64BIT // FIXME: 64bit
        std::cout << "FIXME: 64bit @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
		allert_error();
#else
		TransformPlayerColorIndex_616D0((intptr_t)a1_6E8E);
		allert_error();
#endif
		break;
	}
	case 0x242790: {
#ifdef TEST_x64
	allert_error();
#endif
#ifdef COMPILE_FOR_64BIT // FIXME: 64bit
        std::cout << "FIXME: 64bit @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
		allert_error();
#else
		GetTrueWizardNumber_61790((intptr_t)a1_6E8E);
		allert_error();
#endif
		break;
	}
	case 0x242810: {
		IsBalloonNearCastle_61810(a1_6E8E, 0);
		break;
	}

	case 0x242880: {
#ifdef TEST_x64
	allert_error();
#endif
#ifdef COMPILE_FOR_64BIT // FIXME: 64bit
        std::cout << "FIXME: 64bit @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
		allert_error();
#else
		DrawMinimapEntites_61880((intptr_t)a1_6E8E, 0, 0, 0, 0, 0, 0, 0, 0);
		allert_error();
#endif
		break;
	}
	case 0x242a00: {
#ifdef TEST_x64
	allert_error();
#endif
#ifdef COMPILE_FOR_64BIT // FIXME: 64bit
        std::cout << "FIXME: 64bit @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
		allert_error();
#else
		DrawMinimapEntities_B_61A00((intptr_t)a1_6E8E, 0, 0, 0, 0, 0, 0, 0, 0);
		allert_error();
#endif
		break;
	}
	case 0x2437f0: {
#ifdef TEST_x64
	allert_error();
#endif
#ifdef COMPILE_FOR_64BIT // FIXME: 64bit
        std::cout << "FIXME: 64bit @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
		allert_error();
#else
		DrawMinimapEntities_627F0((intptr_t)a1_6E8E, 0, 0, 0, 0, 0, 0, 0, 0);
		allert_error();
#endif
		break;
	}
	case 0x244570: {
		CanSeePlayerOnMinimap_63570(a1_6E8E, 0);
		break;
	}
	case 0x244600: {
#ifdef TEST_x64
	allert_error();
#endif
#ifdef COMPILE_FOR_64BIT // FIXME: 64bit
        std::cout << "FIXME: 64bit @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
		allert_error();
#else
		DrawMinimap_63600((intptr_t)a1_6E8E, 0, 0, 0, 0, 0, 0, 0, 0);
		allert_error();
#endif
		break;
	}

	case 0x244670: {
#ifdef TEST_x64
	allert_error();
#endif
#ifdef COMPILE_FOR_64BIT // FIXME: 64bit
        std::cout << "FIXME: 64bit @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
		allert_error();
#else
		DrawMapLowRes_63670((intptr_t)a1_6E8E, 0, 0, 0, 0, 0, 0, 0, 0);
		allert_error();
#endif
		break;
	}
	case 0x244c90: {
#ifdef TEST_x64
	allert_error();
#endif
#ifdef COMPILE_FOR_64BIT // FIXME: 64bit
        std::cout << "FIXME: 64bit @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
		allert_error();
#else
		DrawMapHighRes_63C90((intptr_t)a1_6E8E, 0, 0, 0, 0, 0, 0, 0, 0);
		allert_error();
#endif
		break;
	}
	case 0x2454f0: {
#ifdef TEST_x64
	allert_error();
#endif
#ifdef COMPILE_FOR_64BIT // FIXME: 64bit
        std::cout << "FIXME: 64bit @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
		allert_error();
#else
		DrawMinimapMarks_644F0((intptr_t)a1_6E8E, 0, 0, 0, 0, 0, 0, 0, 0);
		allert_error();
#endif
		break;
	}
	case 0x245ce0: {
		DrawObjectiveRectangle_64CE0((v51x_struct*)a1_6E8E);
		break;
	}
	case 0x245e20: {
		AddTree02_00_64E20(a1_6E8E);
		break;
	}

	case 0x245f60: {
		UpdateScenery02_01_64F60(a1_6E8E);
		break;
	}
	case 0x245ff0: {
		UpdateScenery02_02_64FF0(a1_6E8E);
		break;
	}
	case 0x246040: {
		AddStatue02_01_65040(a1_6E8E);
		break;
	}
	case 0x246080: {
		AddDolmen02_02_65080(a1_6E8E);
		break;
	}
	case 0x246110: {
		UpdateScenery02_09_65110(a1_6E8E);
		break;
	}
	case 0x246130:
	case 0x246140:
	case 0x246150:
	case 0x246160:
	case 0x246170:
	case 0x246180:
	case 0x246190:
	case 0x2461a0:
		break;
	case 0x2461b0: {
		UpdateScenery02_12_651B0(a1_6E8E);
		break;
	}
	case 0x246240: {
		UpdateScenery02_13_65240(a1_6E8E);
		break;
	}
	case 0x246280: {
		UpdateScenery02_15_65280(a1_6E8E);
		break;
	}
	case 0x2462a0: {
		UpdateScenery02_14_652A0(a1_6E8E);
		break;
	}
	case 0x2462c0: {
		MoveProjectile_652C0(a1_6E8E);
		break;
	}
	case 0x2464b0: {
		BounceProjectileOffTerrain_654B0(a1_6E8E);
		break;
	}
	case 0x246580: {
		RaiseToEntityCentre_65580(a1_6E8E);
		break;
	}
	case 0x2465a0: {
		LowerFromEntityCentre_655A0(a1_6E8E);
		break;
	}
	case 0x2465c0: {
		AimAtEntity_655C0(a1_6E8E, 0);
		break;
	}
	case 0x246610: {
		AimAtEntityLeading_65610(a1_6E8E, 0);
		break;
	}
	case 0x2466d0: {
		AimAtEntityDirect_656D0(a1_6E8E, 0);
		break;
	}
	case 0x246780: {
		SetProjectileDamage_65780(a1_6E8E, 0, 0);
		break;
	}
	case 0x246820: {
		UpdateProjectileTargeting_65820(a1_6E8E);
		break;
	}
	case 0x246b30: {
		CastPlayerFire_65B30(a1_6E8E);//fly fire projectile
		break;
	}
	case 0x246b50: {
		UpdateProjectile09_1D_65B50(a1_6E8E);
		break;
	}
	case 0x246c20: {
		UpdateProjectileTargeting2_65C20(a1_6E8E);
		break;
	}
	case 0x246f60: {
		CastPosses_65F60(a1_6E8E);//fly possess projectile
		break;
	}
	case 0x247160: {
		UpdateProjectile09_02_66160(a1_6E8E);
		break;
	}
	case 0x247180: {
		UpdateProjectile09_03_66180(a1_6E8E);
		break;
	}
	case 0x247250: {
		UpdateProjectile09_04_66250(a1_6E8E);
		break;
	}
	case 0x247280: {
		UpdateProjectile09_05_66280(a1_6E8E);
		break;
	}
	case 0x2472a0: {
		UpdateProjectile09_06_662A0(a1_6E8E);
		break;
	}
	case 0x2472c0: {
		UpdateProjectile09_07_662C0(a1_6E8E);
		break;
	}
	case 0x2472e0: {
		UpdateProjectile09_08_662E0(a1_6E8E);
		break;
	}
	case 0x247610: {
		UpdateProjectileExplosion_66610(a1_6E8E);
		break;
	}
	case 0x247750: {
		UpdateProjectile09_09_66750(a1_6E8E);
		break;
	}
	case 0x247b30: {
		CastCastleProjectile_66B30(a1_6E8E);//castle fly projectile
		break;
	}
	case 0x247d00: {
		UpdateHomingProjectile_66D00(a1_6E8E);
		break;
	}
	case 0x247fb0: {
		UpdateProjectile09_0B_66FB0(a1_6E8E);
		break;
	}
	case 0x247fd0: {//lighting II
		UpdateProjectile09_0C_66FD0(a1_6E8E);
		break;
	}
	case 0x2482e0: {//arrow2
		AddArcherArrow_672E0(a1_6E8E);
		break;
	}
	case 0x248410: {
		UpdateProjectile09_0E_67410(a1_6E8E);
		break;
	}
	case 0x248430: {
		UpdateProjectile09_0F_67430(a1_6E8E);
		break;
	}
	case 0x248450: {
		UpdateProjectile09_10_67450(a1_6E8E);
		break;
	}
	case 0x248470: {
		UpdateProjectile09_11_67470(a1_6E8E);
		break;
	}

	case 0x2484c0: {
		UpdateProjectile09_12_674C0(a1_6E8E);//possess mana ii
		break;
	}
	case 0x2486f0: {
		UpdateProjectile09_13_676F0(a1_6E8E);
		break;
	}
	case 0x248740: {
		UpdateProjectile09_14_67740(a1_6E8E);
		break;
	}
	case 0x248760: {
		UpdateProjectile09_15_67760(a1_6E8E);
		break;
	}
	case 0x248780: {
		UpdateProjectile09_16_67780(a1_6E8E);
		break;
	}

	case 0x2487a0: {
		UpdateProjectile09_17_677A0(a1_6E8E);
		break;
	}
	case 0x2487d0: {
		UpdateProjectile09_18_677D0(a1_6E8E);
		break;
	}
	case 0x248800: {
		UpdateProjectile09_19_67800(a1_6E8E);
		break;
	}
	case 0x248890: {
		UpdateProjectile09_1A_67890(a1_6E8E);
		break;
	}
	case 0x2488e0: {
		UpdateProjectile09_1B_678E0(a1_6E8E);
		break;
	}

	case 0x248910: {
		UpdateProjectile09_1F_67910(a1_6E8E);
		break;
	}
	case 0x248940: {
		UpdateProjectile09_1C_67940(a1_6E8E);
		break;
	}
	case 0x248960: {
		UpdateProjectile09_1E_67960(a1_6E8E/*,0*/);
		break;
	}
	case 0x248cb0: {
		UpdateProjectile_67CB0(a1_6E8E);
		break;
	}
	case 0x249490: {
		ProjectileHomeOnTarget_68490(a1_6E8E, 0, 0, 0);
		break;
	}

	case 0x2495d0: {
		ProjectileTurnTowardsTarget_685D0(a1_6E8E, 0, 0, 0);
		break;
	}
	case 0x2496d0: {
		CreditKillToWizard_686D0(a1_6E8E, 0);
		break;
	}
	case 0x249740: {
		ProjectileHitTarget_68740(a1_6E8E, 0, 0, 0);
		break;
	}
	case 0x249940: {
		FindProjectileTarget_68940(a1_6E8E);
		break;
	}
	case 0x249ac0: {
		CanProjectileHitEntity_68AC0(a1_6E8E, 0);
		break;
	}

	case 0x249bd0: {
		IsCreatureModel0_68BD0(a1_6E8E, 0);
		break;
	}
	case 0x249bf0: {
		UpdateAllCreatures_68BF0();
		break;
	}
	case 0x249c70: {
		UpdateChainedProjectile_68C70(a1_6E8E);
		break;
	}
	case 0x249d50: {
		CanSpellOwnerCast_68D50(a1_6E8E, 0);
		break;
	}
	case 0x249de0: {
		RechargeSpell_68DE0(a1_6E8E, 0);
		break;
	}
	case 0x249e50: {
		LaunchSpellProjectile_68E50(a1_6E8E, 0, 0);
		break;
	}

	case 0x249f00: {
		UpdateSpellPickupCommon_68FF0(a1_6E8E, 0, 0);
		break;
	}
	case 0x24a250: {
		UpdateSpellPickup_69250(a1_6E8E);
		break;
	}
	case 0x24a2a0: {
		UpdateSpellPickupFloating_692A0(a1_6E8E);
		break;
	}
	case 0x24a2c0: {
		ResetSpellToPickup_692C0(a1_6E8E);
		break;
	}
	case 0x24a300: {
		SpellHitByEntity_69300(a1_6E8E, 0);
		break;
	}

	case 0x24a3f0: {
		SpellFireballActive_693F0(a1_6E8E);//0xff 0x00 pos 0x4d 0xde
		break;
	}
	case 0x24a600: {
		SpellFireballPickup_69600(a1_6E8E);
		break;
	}
	case 0x24a620: {
		SpellFireballPickupUpgrade_69620(a1_6E8E);
		break;
	}
	case 0x24a640: {
		SpellPossessionActive_69640(a1_6E8E);//0xff 0x01 pos 0x4d 0xde
		break;
	}
	case 0x24a900: {
		CastPossessionProjectile_69900(a1_6E8E, 0);
		break;
	}

	case 0x24aa70: {
		SpellPossessionPickup_69A70(a1_6E8E);
		break;
	}
	case 0x24aa90: {
		SpellPossessionPickupUpgrade_69A90(a1_6E8E);
		break;
	}
	case 0x24aab0: {//get scroll6
		SpellCastleActive_69AB0(a1_6E8E);//0x0f 0x02 pos 0xc2 0xd5
		break;
	}
	case 0x24ad70: {
		SpellCastlePickup_69D70(a1_6E8E);//0x0f 0x02 pos 0x2c 0xd5
		break;
	}
	case 0x24ad90: {
		SpellCastlePickupUpgrade_69D90(a1_6E8E);
		break;
	}

	case 0x24adb0: {
		GetScroll_69DB0(a1_6E8E);
		break;
	}
	case 0x24aff0: {
		AllCreaturesKilled_69FF0(a1_6E8E);
		break;
	}

	case 0x24b010: {
		SpellSpeedUpPickupUpgrade_6A010(a1_6E8E);
		break;
	}
	case 0x24b030: {
		SpellMetamorphActive_6A030(a1_6E8E);
		break;
	}
	case 0x24b2c0: {
		SpellMetamorphPickup_6A2C0(a1_6E8E);
		break;
	}
	case 0x24b2e0: {
		SpellMetamorphPickupUpgrade_6A2E0(a1_6E8E);
		break;
	}
	case 0x24b300: {
		SpellHealActive_6A300(a1_6E8E);
		break;
	}

	case 0x24b440: {
		SpellHealPickup_6A440(a1_6E8E);
		break;
	}
	case 0x24b460: {
		SpellHealPickupUpgrade_6A460(a1_6E8E);
		break;
	}
	case 0x24b480: {
		SpellShieldActive_6A480(a1_6E8E);
		break;
	}
	case 0x24b580: {
		SpellShieldPickup_6A580(a1_6E8E);
		break;
	}
	case 0x24b5a0: {
		SpellShieldPickupUpgrade_6A5A0(a1_6E8E);
		break;
	}

	case 0x24b5c0: {
		SpellLightningActive_6A5C0(a1_6E8E);
		break;
	}
	case 0x24b9c0: {
		SpellLightningPickup_6A9C0(a1_6E8E);
		break;
	}
	case 0x24b9e0: {
		SpellLightningPickupUpgrade_6A9E0(a1_6E8E);
		break;
	}
	case 0x24ba00: {
		SpellReboundActive_6AA00(a1_6E8E);
		break;
	}
	case 0x24bac0: {
		SpellReboundPickup_6AAC0(a1_6E8E);
		break;
	}

	case 0x24bae0: {
		SpellReboundPickupUpgrade_6AAE0(a1_6E8E);
		break;
	}
	case 0x24bb00: {
		SpellMeteorActive_6AB00(a1_6E8E);
		break;
	}
	case 0x24bd00: {
		SpellMeteorPickup_6AD00(a1_6E8E);
		break;
	}
	case 0x24bd20: {
		SpellMeteorPickupUpgrade_6AD20(a1_6E8E);
		break;
	}
	case 0x24bd60: {
		SpellTeleportActive_6AD60(a1_6E8E);
		break;
	}

	case 0x24c180: {
		SpellTeleportPickup_6B180(a1_6E8E);
		break;
	}
	case 0x24c1a0: {
		SpellTeleportPickupUpgrade_6B1A0(a1_6E8E);
		break;
	}
	case 0x24c1c0: {
		SpellInvisibleActive_6B1C0(a1_6E8E);
		break;
	}
	case 0x24c2d0: {
		SpellInvisiblePickup_6B2D0(a1_6E8E);
		break;
	}
	case 0x24c2f0: {
		SpellInvisiblePickupUpgrade_6B2F0(a1_6E8E);
		break;
	}

	case 0x24c310: {
		SpellBeyondSightActive_6B310(a1_6E8E);
		break;
	}
	case 0x24c3a0: {
		SpellBeyondSightPickup_6B3A0(a1_6E8E);
		break;
	}
	case 0x24c3c0: {
		SpellBeyondSightPickupUpgrade_6B3C0(a1_6E8E);
		break;
	}
	case 0x24c3e0: {
		SpellStealManaActive_6B3E0(a1_6E8E);
		break;
	}
	case 0x24c5d0: {
		SpellStealManaPickup_6B5D0(a1_6E8E);
		break;
	}

	case 0x24c5f0: {
		SpellStealManaPickupUpgrade_6B5F0(a1_6E8E);
		break;
	}
	case 0x24c610: {
		SpellDuelActive_6B610(a1_6E8E);
		break;
	}
	case 0x24c830: {
		SpellDuelPickup_6B830(a1_6E8E);
		break;
	}
	case 0x24c850: {
		SpellDuelPickupUpgrade_6B850(a1_6E8E);
		break;
	}
	case 0x24c870: {
		SpellTremorActive_6B870(a1_6E8E);
		break;
	}

	case 0x24ca70: {
		SpellTremorPickup_6BA70(a1_6E8E);
		break;
	}
	case 0x24ca90: {
		SpellTremorPickupUpgrade_6BA90(a1_6E8E);
		break;
	}
	case 0x24cab0: {
		SpellCraterActive_6BAB0(a1_6E8E);
		break;
	}
	case 0x24ccb0: {
		SpellCraterPickup_6BCB0(a1_6E8E);
		break;
	}
	case 0x24ccd0: {
		SpellCraterPickupUpgrade_6BCD0(a1_6E8E);
		break;
	}

	case 0x24ccf0: {
		SpellEarthquakeActive_6BCF0(a1_6E8E);
		break;
	}
	case 0x24cef0: {
		SpellEarthquakePickup_6BEF0(a1_6E8E);
		break;
	}
	case 0x24cf10: {
		SpellEarthquakePickupUpgrade_6BF10(a1_6E8E);
		break;
	}
	case 0x24cf30: {
		SpellVolcanoActive_6BF30(a1_6E8E);
		break;
	}
	case 0x24d130: {
		SpellVolcanoPickup_6C130(a1_6E8E);
		break;
	}

	case 0x24d150: {
		SpellVolcanoPickupUpgrade_6C150(a1_6E8E);
		break;
	}
	case 0x24d170: {
		SpellSummonArmyActive_6C170(a1_6E8E);
		break;
	}
	case 0x24d3a0: {
		SpellSummonArmyPickup_6C3A0(a1_6E8E);
		break;
	}
	case 0x24d3c0: {
		SpellSummonArmyPickupUpgrade_6C3C0(a1_6E8E);
		break;
	}
	case 0x24d3e0: {
		SpellGravityWellActive_6C3E0(a1_6E8E);
		break;
	}

	case 0x24d5e0: {
		SpellGravityWellPickup_6C5E0(a1_6E8E);
		break;
	}
	case 0x24d600: {
		SpellGravityWellPickupUpgrade_6C600(a1_6E8E);
		break;
	}
	case 0x24d620: {
		SpellWhirlwindActive_6C620(a1_6E8E);
		break;
	}
	case 0x24d830: {
		SpellWhirlwindPickup_6C830(a1_6E8E);
		break;
	}
	case 0x24d850: {
		SpellWhirlwindPickupUpgrade_6C850(a1_6E8E);
		break;
	}

	case 0x24d870: {
		SpellFoolsManaActive_6C870(a1_6E8E);
		break;
	}
	case 0x24da80: {
		SpellFoolsManaPickup_6CA80(a1_6E8E);
		break;
	}
	case 0x24daa0: {
		SpellFoolsManaPickupUpgrade_6CAA0(a1_6E8E);
		break;
	}
	case 0x24dac0: {
		SpellMagicMineActive_6CAC0(a1_6E8E);
		break;
	}
	case 0x24dce0: {
		SpellMagicMinePickup_6CCE0(a1_6E8E);
		break;
	}

	case 0x24dd00: {
		SpellMagicMinePickupUpgrade_6CD00(a1_6E8E);
		break;
	}
	case 0x24dd20: {
		SpellAllianceActive_6CD20(a1_6E8E);

		break;
	}
	case 0x24df60: {
		SpellAlliancePickup_6CF60(a1_6E8E);
		break;
	}
	case 0x24df80: {
		SpellAlliancePickupUpgrade_6CF80(a1_6E8E);
		break;
	}
	case 0x24dfa0: {
		SpellCaveInActive_6CFA0(a1_6E8E);
		break;
	}

	case 0x24e1c0: {
		SpellCaveInPickup_6D1C0(a1_6E8E);
		break;
	}
	case 0x24e1e0: {
		SpellCaveInPickupUpgrade_6D1E0(a1_6E8E);
		break;
	}
	case 0x24e200: {
		MoveCursorToSelectedSpell_6D200((type_str_0x2BDE*)a1_6E8E);
		break;
	}
	case 0x24e420: {
#if defined(__linux__) || defined(__APPLE__) // FIXME: types
		std::cout << "FIXME: types @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
		SelectSpellCategory_6D420((short)a1_6E8E, 0);
		stub_fix_it();//bad retyping
#endif
		break;
	}
	case 0x24e4c0: {
		UpdateSubSpellIndex_6D4C0((type_str_611*)a1_6E8E);
		break;
	}

				 /*case 0x24e4f0: {
					  sub_6D4F0((uint8_t*)a1,0);
					 break;
				 }*/
	case 0x24e5e0: {
		SetSpell_6D5E0(a1_6E8E, 0);
		break;
	}
	case 0x24e710: {
		GetSpellManaCost_6D710(a1_6E8E, 0, 0);
		break;
	}
	case 0x24e830: {
		CopyAxisForSpellWithLife_6D830(a1_6E8E, 0);
		break;
	}
	case 0x24e880: {
		RestorePreviousSpell_6D880(a1_6E8E);
		break;
	}

	case 0x24e8b0: {
#if defined(__linux__) || defined(__APPLE__) // FIXME: types
		std::cout << "FIXME: types @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
        allert_error();
#else
		SendSpellMessage_6D8B0((unsigned short)a1_6E8E, 0, 0);
		stub_fix_it();//bad retyping
#endif
		break;
	}
	case 0x24e9c0: {
		UpdateSpellAvailability_6D9C0((type_str_611*)a1_6E8E, 0, 0, 0, 0);
		break;
	}
	case 0x24ead0: {
		UpdateSpellExperience_6DAD0((type_str_611*)a1_6E8E, 0, 0);
		break;
	}
	case 0x24eb50: {
#if defined(__linux__) || defined(__APPLE__) // FIXME: types
		std::cout << "FIXME: types @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
		GrantSpellToPlayer_6DB50((char)a1_6E8E, 0);
		stub_fix_it();//bad retyping
#endif
		break;
	}
	case 0x24ebd0: {
		GrantAllSpellsMultiplayer_6DBD0();
		break;
	}

	case 0x24ec40: {
#if defined(__linux__) || defined(__APPLE__) // FIXME: types
		std::cout << "FIXME: types @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
		ImproveAbility_6DC40((unsigned char)a1_6E8E);
		stub_fix_it();//bad retyping
#endif
		break;
	}
	case 0x24eca0: {
		CastSpell_6DCA0(a1_6E8E, 0, 0, 0, 0, 0);
		break;
	}
	case 0x24f020: {
#if defined(__linux__) || defined(__APPLE__) // FIXME: types
		std::cout << "FIXME: types @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
		GetSpellIndex_6E020((unsigned short)a1_6E8E);
		stub_fix_it();//bad retyping
#endif
		break;
	}
	case 0x24f090: {
		UpdateExperience_6E090((type_str_611*)a1_6E8E, 0);
		break;
	}
	case 0x24f0d0: {
		EnableAllSpells_6E0D0();
		break;
	}

	case 0x24f150: {
		PlayEntitySounds_6E150();
		break;
	}
	case 0x24f450: {
#if defined(__linux__) || defined(__APPLE__) // FIXME: types
		std::cout << "FIXME: types @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
		PrepareEventSound_6E450((short)a1_6E8E, 0, 0);
		stub_fix_it();//bad retyping
#endif
		break;
	}
	case 0x24fa90: {
#ifdef TEST_x64
	allert_error();
#endif
#ifdef COMPILE_FOR_64BIT // FIXME: 64bit
  std::cout << "FIXME: 64bit @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
		ShouldUpdateSound_6EA90((intptr_t)a1_6E8E, 0);
		allert_error();
#endif
		break;
	}
	case 0x24fab0: {
#ifdef TEST_x64
	allert_error();
#endif
#ifdef COMPILE_FOR_64BIT // FIXME: 64bit
  std::cout << "FIXME: 64bit @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
		EndLoop_6EAB0((intptr_t)a1_6E8E, 0, 0);
		allert_error();
#endif
		break;
	}
	case 0x24fb90: {
		CreateIndexes_6EB90((filearray_struct*)a1_6E8E);
		break;
	}

	case 0x24FBF0: {
		CreateFileArrayIndexes_6EBF0((filearray_struct*)a1_6E8E);
		break;
	}
	case 0x24FDB0: {
		SetCenterScreenForFlyAssistant_6EDB0();
		break;
	}
	case 0x24fde0: {
#if defined(__linux__) || defined(__APPLE__) // FIXME: types
		std::cout << "FIXME: types @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
		SetMousePosition_6EDE0((intptr_t)a1_6E8E, 0);
		stub_fix_it();//bad retyping
#endif
		break;
	}
	case 0x24ff10: {
#if defined(__linux__) || defined(__APPLE__) // FIXME: types
		std::cout << "FIXME: types @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
		SetMouseMinMax_6EF10((intptr_t)a1_6E8E, 0, 0, 0);
		stub_fix_it();//bad retyping
#endif
		break;
	}

	case 0x250030: {
		AddSwitch0B_00_6F030(a1_6E8E);
		break;
	}
	case 0x250070: {
		CheckpointArrived_6F070(a1_6E8E);
		break;
	}
	case 0x2500b0: {
		SwitchCheckPlayersNear2_6F0B0(a1_6E8E);
		break;
	}
	case 0x250100: {
		SwitchCheckPlayersNear_6F100(a1_6E8E);
		break;
	}
	case 0x250150: {
		AddSwitch0B_04_6F150(a1_6E8E);
		break;
	}

	case 0x2501c0: {
		AddSwitch0B_20_6F1C0(a1_6E8E);
		break;
	}
	case 0x2502b0: {
		ActivateSwitchChain_6F2B0(a1_6E8E);
		break;
	}
	case 0x250300: {
		CountCreaturesOfModel_6F300(a1_6E8E, 0);
		break;
	}
	case 0x250420: {
		CountCreatures00_6F420(a1_6E8E);
		break;
	}
	case 0x250440: {
		CountCreatures01_6F440(a1_6E8E);
		break;
	}

	case 0x250460: {
		CountCreatures02_6F460(a1_6E8E);
		break;
	}
	case 0x250480: {
		CountCreatures03_6F480(a1_6E8E);
		break;
	}
	case 0x2504a0: {//get scroll4
		AddSwitch0B_11_6F4A0(a1_6E8E);
		break;
	}
	case 0x2504c0: {
		CountCreatures05_6F4C0(a1_6E8E);
		break;
	}
	case 0x2504e0: {
		CountCreatures06_6F4E0(a1_6E8E);
		break;
	}

	case 0x250500: {
		CountCreatures07_6F500(a1_6E8E);
		break;
	}
	case 0x250520: {
		CountCreatures08_6F520(a1_6E8E);
		break;
	}
	case 0x250540: {
		CountCreatures09_6F540(a1_6E8E);
		break;
	}
	case 0x250560: {
		CountCreatures0A_6F560(a1_6E8E);
		break;
	}
	case 0x250580: {
		CountCreatures0B_6F580(a1_6E8E);
		break;
	}

	case 0x2505a0: {
		CountCreatures0C_6F5A0(a1_6E8E);
		break;
	}
	case 0x2505c0: {
		CountCreatures0D_6F5C0(a1_6E8E);
		break;
	}
	case 0x2505e0: {
		CountCreatures0E_6F5E0(a1_6E8E);
		break;
	}
	case 0x250600: {
		CountCreatures0F_6F600(a1_6E8E);
		break;
	}
	case 0x250620: {
		CountCreatures10_6F620(a1_6E8E);
		break;
	}

	case 0x250640: {
		CountCreatures11_6F640(a1_6E8E);
		break;
	}
	case 0x250660: {
		CountCreatures12_6F660(a1_6E8E);
		break;
	}
	case 0x250680: {
		CountCreatures13_6F680(a1_6E8E);
		break;
	}
	case 0x2506a0: {
		CountCreatures14_6F6A0(a1_6E8E);
		break;
	}
	case 0x2506c0: {
		CountCreatures15_6F6C0(a1_6E8E);
		break;
	}

	case 0x2506e0: {
		CountCreatures16_6F6E0(a1_6E8E);
		break;
	}
	case 0x250700: {
		CountCreatures17_6F700(a1_6E8E);
		break;
	}
	case 0x250720: {
		CountCreatures18_6F720(a1_6E8E);
		break;
	}
	case 0x250740: {
		CountCreatures19_6F740(a1_6E8E);
		break;
	}
	case 0x250760: {
		CountCreatures1A_6F760(a1_6E8E);
		break;
	}

	case 0x250780: {
		CountCreatures1B_6F780(a1_6E8E);
		break;
	}
	case 0x2507a0: {
		CountCreatures1C_6F7A0(a1_6E8E);
		break;
	}
	case 0x2507c0: {
		CountAllCreatures_6F7C0(a1_6E8E);
		break;
	}
	case 0x2507e0: {
		ActivateSwitchChainOnce_6F7E0(a1_6E8E);
		break;
	}
	case 0x250850: {
		InitSwitchChainZaxisAndSound_6F850(a1_6E8E, 0);
		break;
	}
	case 0x2508e0: {
		FindPlayerTouchingSwitch_6F8E0(a1_6E8E, 0);
		break;
	}
	default: {
		// bad end
		int yy = 0;
		int xx = 157 / yy;
		mydelay(10000);
		exit(1);
		//bad end
	}
	}
	//return 0;
}

type_entity_0x6E8E* pre_sub_4A190_axis_3d(uint32_t adress, axis_3d* a1_axis3d)//pre 22b190
{
	if (CommandLineParams.DoShowNewProcedures()) {
		test_pre_sub_4a190(adress);//for debug
	}
	switch (adress)
	{
		/*case 0x22b810: {
			return GetFreeEntityCount_4A810();
			break;
		}*/
	case 0x22b820: {// 1 instance in level 1
		return SetPlayerStart0_4A820(a1_axis3d);
		break;
	}
	case 0x22b840: {
		return SetPlayerStart1_4A840(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x22b860: {
		return SetPlayerStart2_4A860(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x22b880: {
		return SetPlayerStart3_4A880(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x22b8a0: {
		return SetPlayerStart4_4A8A0(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x22b8c0: {
		return SetPlayerStart5_4A8C0(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x22b8e0: {
		return SetPlayerStart6_4A8E0(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x22b900: {
		return SetPlayerStart7_4A900(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x22b920: {
		return AddPlayer_4A920(a1_axis3d);
		break;
	}
	case 0x22b9c0: {
		return AddWizard03_01_4A9C0(a1_axis3d);
		break;
	}
	case 0x22ba40: {
		return AddCastle03_02_4AA40(a1_axis3d);//castle create
		break;
	}
	case 0x22bba0: {
		return AddPlayer03_03_4ABA0(a1_axis3d);
		break;
	}
	case 0x22bc40: {
		return AddTree_4AC40(a1_axis3d);
		break;
	}
	case 0x22bd70: {
		return AddStone_4AD70(a1_axis3d);
		break;
	}
	case 0x22bdf0: {// 1 instance in level 3
		return AddDolmen_4ADF0(a1_axis3d);
		break;
	}
	case 0x22be80: {
		return AddScenery02_03_4AE80(a1_axis3d);
		break;
	}
	case 0x22bf00: {
		return AddScenery02_04_4AF00(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x22bf70: {
		return AddScenery02_05_4AF70(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x22bfe0: {
		return AddScenery02_06_4AFE0(a1_axis3d);
		break;
	}
	case 0x22c0f0: {
		return AddScenery02_07_4B0F0(a1_axis3d);
		break;
	}
	case 0x22c120: {
		return AddScenery02_08_4B120(a1_axis3d);
		break;
	}
				 /*case 0x22c150: {
					 return AddSceneryWithSprite_4B150(a1_axis3d, 0, 0, 0);//fix
					 break;
				 }*/
	case 0x22c240: {
		return AddFlyingWorm05_00_4B240(a1_axis3d);
		break;
	}
	case 0x22c490: {// 1 instance in level 33
		return AddCreature_4B490(a1_axis3d);
		break;
	}
	case 0x22c590: {
		return AddBee05_02_4B590(a1_axis3d);
		break;
	}
	case 0x22c6f0: {
		return AddWorm05_03_4B6F0(a1_axis3d);
		break;
	}
	case 0x22ca10: {//get scroll1 //create archers
		return AddArchers_4BA10(a1_axis3d);
		break;
	}
	case 0x22cbb0: {
		return AddSkeletonArcher05_09_4BBB0(a1_axis3d);
		break;
	}
	case 0x22cd00: {
		return AddEmu05_0A_4BD00(a1_axis3d);
		break;
	}
	case 0x22cdf0: {
		return AddBuilder05_0C_4BDF0(a1_axis3d);
		break;
	}
	case 0x22cf40: {// 1 instance in level 28
		return AddVilliger_4BF40(a1_axis3d);
		break;
	}
	case 0x22d0b0: {
		return AddTrader_4C0B0(a1_axis3d);
		break;
	}
	case 0x22d1e0: {
		return AddExplodedHuman05_0F_4C1E0(a1_axis3d);
		break;
	}
	case 0x22d310: {
		return AddWyvern05_10_4C310(a1_axis3d);
		break;
	}
	case 0x22d460: {
		return AddLion05_11_4C460(a1_axis3d);
		break;
	}
	case 0x22d590: {
		return AddHead05_12_4C590(a1_axis3d);
		break;
	}
	case 0x22d6b0: {
		return AddFirebug05_13_4C6B0(a1_axis3d);
		break;
	}
	case 0x22d7f0: {
		return AddSpider05_14_4C7F0(a1_axis3d);
		break;
	}
	case 0x22d8f0: {
		return AddDiablo05_15_4C8F0(a1_axis3d);
		break;
	}
	case 0x22da00: {
		return AddFlyingMana05_16_4CA00(a1_axis3d);
		break;
	}
				 /*case 0x22db60: {
					 AddFlyingManaSegments_4CB60((type_str_0x6E8E*)a1);
					 return fix_it_4A190();
					 break;
				 }*/
	case 0x22dbf0: {
		return AddBalloon05_17_4CBF0(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x22dcf0: {
		return AddCreature05_18_4CCF0(a1_axis3d);
		break;
	}
	case 0x22de00: {
		return AddFlyingCreature05_19_4CE00(a1_axis3d);
		break;
	}
	case 0x22df00: {
		return AddMummy05_1A_4CF00(a1_axis3d);
		break;
	}
	case 0x22e000: {
		return AddBigDragon05_1B_4D000(a1_axis3d);
		break;
	}
	case 0x22e1d0: {
		return AddMana05_1C_4D1D0(a1_axis3d);
		break;
	}
	case 0x22e2e0: {
		return SummonFireball_4D2E0(a1_axis3d);//summon spell fire
		break;
	}
	case 0x22e380: {
		return AddProjectile09_1C_4D380(a1_axis3d);
		break;
	}
	case 0x22e3b0: {
		return SummonManaPosession_4D3B0(a1_axis3d);//cast spell posses
		break;
	}
	case 0x22e470: {
		return AddProjectile09_02_4D470(a1_axis3d);
		break;
	}
	case 0x22e500: {
		return AddProjectile09_03_4D500(a1_axis3d);
		break;
	}
	case 0x22e590: {
		return AddProjectile09_04_4D590(a1_axis3d);
		break;
	}
	case 0x22e620: {
		return AddProjectile09_05_4D620(a1_axis3d);
		break;
	}
	case 0x22e6b0: {
		return AddProjectile09_06_4D6B0(a1_axis3d);
		break;
	}
	case 0x22e740: {
		return AddProjectile09_07_4D740(a1_axis3d);
		break;
	}
	case 0x22e7d0: {
		return AddProjectile09_08_4D7D0(a1_axis3d);
		break;
	}
	case 0x22e860: {
		return AddProjectile09_09_4D860(a1_axis3d);
		break;
	}
	case 0x22e900: {
		return AddProjectile09_0A_4D900(a1_axis3d);//castle cast
		break;
	}
	case 0x22e990: {
		return AddProjectile09_0B_4D990(a1_axis3d);
		break;
	}
	case 0x22ea20: {
		return AddProjectile09_0C_4DA20(a1_axis3d);
		break;
	}
	case 0x22eab0: {
		return AddEvent09_0D_4DAB0(a1_axis3d);
		break;
	}
	case 0x22ebc0: {
		return AddProjectile09_0E_4DBC0(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x22ec40: {
		return AddProjectile09_14_4DC40(a1_axis3d);
		break;
	}
	case 0x22ecc0: {
		return AddProjectile09_15_4DCC0(a1_axis3d);
		break;
	}
	case 0x22ed50: {
		return AddProjectile09_0F_4DD50(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x22edc0: {
		return NULL;
		break;
	}
	case 0x22edd0: {
		return AddProjectile09_11_4DDD0(a1_axis3d);
		break;
	}
	case 0x22ee80: {
		return NULL;
		break;
	}
	case 0x22ee90: {
		return NULL;
		break;
	}
	case 0x22eea0: {
		return AddProjectile09_16_4DEA0(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x22ef30: {
		return AddProjectile09_1B_4DF30(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x22efc0: {
		return AddProjectile09_17_4DFC0(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x22f050: {
		return AddProjectile09_18_4E050(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x22f0f0: {
		return AddProjectile09_19_4E0F0(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x22f180: {
		return AddProjectile09_1A_4E180(a1_axis3d);
		break;
	}
	case 0x22f210: {
		return AddProjectile09_1E_4E210(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x22f2a0: {
		return AddProjectile09_1D_4E2A0(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x22f320: {
		return NewAdd0A00_4E320(a1_axis3d);
		break;
	}
	case 0x22f3b0: {// 1 instance in level 8
		return NewAdd0A01_4E3B0(a1_axis3d);
		break;
	}
	case 0x22f430: {
		return NewAdd0A02_4E430(a1_axis3d);
		break;
	}
	case 0x22f490: {
		return NewAdd0A03_4E490(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x22f500: {
		return NewAdd0A04_4E500(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x22f570: {
		return NewAdd0A05_4E570(a1_axis3d);//begin of water splash
		break;
	}
	case 0x22f5f0: {
		return NewAdd0A06_4E5F0(a1_axis3d);
		break;
	}
	case 0x22f6a0: {
		return NewAdd0A07_4E6A0(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x22f750: {
		return AddEffect0A_08_4E750();
		break;
	}
	case 0x22f760: {
		return NewAdd0A09_4E760(a1_axis3d);
		break;
	}
	case 0x22f7d0: {
		return NewAdd0A0A_4E7D0(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x22f840: {
		return NewAdd0A0B_4E840(a1_axis3d);
		break;
	}
	case 0x22f8c0: {
		return NewAdd0A0C_4E8C0(a1_axis3d);//posses spell drop1
		break;
	}
	case 0x22f950: {
		return NewAdd0A46_4E950(a1_axis3d);
		break;
	}
	case 0x22f9e0: {
		return SetParticleSmoke3B_4E9E0(a1_axis3d);
		break;
	}
	case 0x22fa20: {
		return SetParticleSmoke3C_4EA20(a1_axis3d);
		break;
	}
	case 0x22fa60: {
		return AddRandomSmoke_4EA60(a1_axis3d);
		break;
	}
				 /*case 0x22faa0: {
					 sub_4EAA0(a1_axis3d, 0, 0, 0, 0);//fix it
					 return fix_it_4A190();
					 break;
				 }*/
	case 0x22fb50: {//in quest point
		return ArriveCheckpoint_4EB50(a1_axis3d);
		break;
	}
	case 0x22fc10: {// 1 instance in level 9
		return AddSmoke_4EC10(a1_axis3d);
		break;
	}
	case 0x22fcd0: {
		return AddEffect0A_0F_4ECD0(a1_axis3d);
		break;
	}
	case 0x22fd70: {
		return AddMeteor_4ED70(a1_axis3d);
		break;
	}
	case 0x22fdc0: {
		return AddEffect0A_10_4EDC0(a1_axis3d);
		break;
	}
	case 0x22fed0: {
		return AddEffect0A_12_4EED0(a1_axis3d);
		break;
	}
	case 0x22ff30: {
		return AddEffectAction98_4EF30(a1_axis3d);
		break;
	}
	case 0x22ff90: {
		return AddEffect0A_13_4EF90(a1_axis3d);
		break;
	}
	case 0x230020: {
		return 0;
		break;
	}
	case 0x230030: {
		return 0;
		break;
	}
	case 0x230040: {
		return AddWind_4F040(a1_axis3d);
		break;
	}
				 /*case 0x230c10: {
					 return SetChainToTerrainHeight_4F1C0((type_str_0x6E8E*)a1);
					 break;
				 }*/
	case 0x2302a0: {
		return AddFireSpheres_4F2A0(a1_axis3d);
		break;
	}
				 /*case 0x230440: {
					 return InitWindEntity_4F440((type_str_0x6E8E*)a1);
					 break;
				 }*/
	case 0x2305f0: {
		return AddEffect0A_17_4F5F0(a1_axis3d);
		break;
	}
	case 0x230690: {
		return 0;
		break;
	}
	case 0x2306a0: {
		return AddEffect0A_19_4F6A0(a1_axis3d);
		break;
	}
	case 0x230720: {
		return AddEffect0A_1A_4F720(a1_axis3d);
		break;
	}
	case 0x2307a0: {
		return AddEffect0A_1B_4F7A0(a1_axis3d);
		break;
	}
	case 0x230800: {
		return AddEffect0A_1C_4F800(a1_axis3d);
		break;
	}
	case 0x230860: {
		return AddEffectAction66_4F860(a1_axis3d);
		break;
	}
	case 0x2308b0: {
		return AddEffectAction67_4F8B0(a1_axis3d);
		break;
	}
	case 0x230900: {
		return AddEffectAction68_4F900(a1_axis3d);
		break;
	}
	case 0x230950: {
		return AddEffectAction69_4F950(a1_axis3d);
		break;
	}
	case 0x2309a0: {//2 instances in level 1
		return AddPointToPath_4F9A0(a1_axis3d);
		break;
	}
	case 0x230a00: {
		return AddEffect0A_1D_4FA00(a1_axis3d);
		break;
	}
	case 0x230a60: {
		return AddEffect0A_20_4FA60(a1_axis3d);
		break;
	}
	case 0x230ac0: {
		return AddEffect0A_1F_4FAC0(a1_axis3d);
		break;
	}
	case 0x230b20: {
		return AddCaveEffect1_4FB20(a1_axis3d);
		break;
	}
	case 0x230b80: {
		return AddCaveEffect2_4FB80(a1_axis3d);
		break;
	}
	case 0x230be0: {
		return AddCaveEffect3_4FBE0(a1_axis3d);
		break;
	}
	case 0x230c30: {
		return AddCaveEffect4_4FC30(a1_axis3d);
		break;
	}
	case 0x230ca0: {
		return AddCaveEffectAction91_4FCA0(a1_axis3d);
		break;
	}
	case 0x230cd0: {
		return AddCaveEffectAction92_4FCD0(a1_axis3d);
		break;
	}
	case 0x230d00: {
		return AddCaveEffect_4FD00(a1_axis3d);
		break;
	}
	case 0x230d70: {
		return AddEffectAction55_4FD70(a1_axis3d);
		break;
	}
	case 0x230de0: {
		return AddEffectAction54_4FDE0(a1_axis3d);
		break;
	}
	case 0x230e40: {
		return AddEffect0A_22_4FE40(a1_axis3d);
		break;
	}
	case 0x230f20: {
		return AddEffect0A_23_4FF20();
		break;
	}
	case 0x230f30: {
		return AddEffect0A_24_4FF30(a1_axis3d);
		break;
	}
	case 0x230fb0: {
		return AddEffect0A_26_4FFB0(a1_axis3d);
		break;
	}
	case 0x231020: {
		return AddEffect0A_21_50020(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x231080: {//creating mana sphere
		return CreateManaSphere512_50080(a1_axis3d);
		break;
	}
	case 0x2310a0: {
		return CreateManaSphere2560_500A0(a1_axis3d);
		break;
	}
	case 0x2310c0: {
		return CreateManaSphere_500C0(a1_axis3d, 0);//fix it
		break;
	}
	case 0x231130: {
		return AddEffectAction62_50130(a1_axis3d);
		break;
	}
	case 0x2311d0: {
		return AddEffect0A_28_501D0(a1_axis3d);
		break;
	}
	case 0x231250: {//47 instances in level 1
		return AddTerrainModification_50250(a1_axis3d);
		break;
	}
	case 0x2312b0: {//cast castleII
		return AddEffectAction45_502B0(a1_axis3d);
		break;
	}
	case 0x231320: {
		return AddEffect0A_29_50320(a1_axis3d);
		break;
	}
	case 0x231370: {//arrow1
		return AddEffectAction44_50370(a1_axis3d);
		break;
	}
	case 0x2313d0: {
		return AddEffectAction46_503D0(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x231430: {
		return AddEffectAction56_50430(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x2314b0: {
		return AddEffectAction58_504B0(a1_axis3d);
		break;
	}
	case 0x231500: {
		return AddAuxiliary_50500(a1_axis3d);
		break;
	}
	case 0x2315a0: {
		return AddEffectAction75_505A0(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x2315e0: {
		return AddMovingEffect_505E0(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x231640: {
		return AddEffectAction60_50640(a1_axis3d);
		break;
	}
	case 0x2316e0: {
		return AddEffectAction61_506E0(a1_axis3d);
		break;
	}
	case 0x231780: {
		return AddEffectAction70_50780(a1_axis3d);
		break;
	}
	case 0x2317c0: {
		return AddEffectAction71_507C0(a1_axis3d);
		break;
	}
	case 0x231800: {
		return AddEffectAction81_50800();
		break;
	}
	case 0x231840: {
		return AddEffectAction85_50840(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x2318e0: {
		return CastleDefendCreate_508E0(a1_axis3d);
		break;
	}
	case 0x231960: {
		return AddEffectAction93_50960(a1_axis3d);
		break;
	}
	case 0x231a10: {
		return 0;
		break;
	}
	case 0x231a20: {
		return AddCaveEffect5_50A20(a1_axis3d);
		break;
	}
	case 0x231a80: {
		return 0;
		break;
	}
	case 0x231a90: {
		return AddSwitchXX_50A90(a1_axis3d, 0, 0); //fix it
		break;
	}
	case 0x231ae0: {// 1 instance in level 21
		return AddSwitch00_50AE0(a1_axis3d);
		break;
	}
	case 0x231b00: {// 1 instance in level 1
		return AddSwitch01_50B00(a1_axis3d);//0B-01,0B-00
		break;
	}
	case 0x231b20: {
		return AddSwitch02_50B20(a1_axis3d);
		break;
	}
	case 0x231b40: {
		return AddSwitch03_50B40(a1_axis3d);
		break;
	}
	case 0x231b60: {// 1 instance in level 1
		return AddSwitch04_50B60(a1_axis3d);
		break;
	}
	case 0x231b80: {
		return AddSwitch32_50B80(a1_axis3d);
		break;
	}
	case 0x231c10: {
		return AddSwitch12_50C10(a1_axis3d);
		break;
	}
	case 0x231c30: {
		return AddSwitch13_50C30(a1_axis3d);
		break;
	}
	case 0x231c50: {
		return AddSwitch14_50C50(a1_axis3d);
		break;
	}
	case 0x231c70: {
		return AddSwitch15_50C70(a1_axis3d);
		break;
	}
	case 0x231c90: {
		return AddSwitch16_50C90(a1_axis3d);
		break;
	}
	case 0x231cb0: {//get scroll2
		return AddSwitch17_50CB0(a1_axis3d);
		break;
	}
	case 0x231cd0: {
		return AddSwitch18_50CD0(a1_axis3d);
		break;
	}
	case 0x231cf0: {
		return AddSwitch19_50CF0(a1_axis3d);
		break;
	}
	case 0x231d10: {
		return AddSwitch20_50D10(a1_axis3d);
		break;
	}
	case 0x231d30: {
		return AddSwitch21_50D30(a1_axis3d);
		break;
	}
	case 0x231d50: {
		return AddSwitch22_50D50(a1_axis3d);
		break;
	}
	case 0x231d70: {
		return AddSwitch23_50D70(a1_axis3d);
		break;
	}
	case 0x231d90: {
		return AddSwitch24_50D90(a1_axis3d);
		break;
	}
	case 0x231db0: {
		return AddSwitch25_50DB0(a1_axis3d);
		break;
	}
	case 0x231dd0: {
		return AddSwitch26_50DD0(a1_axis3d);
		break;
	}
	case 0x231df0: {
		return AddSwitch27_50DF0(a1_axis3d);
		break;
	}
	case 0x231e10: {
		return AddSwitch28_50E10(a1_axis3d);
		break;
	}
	case 0x231e30: {
		return AddSwitch29_50E30(a1_axis3d);
		break;
	}
	case 0x231e50: {
		return AddSwitch33_50E50(a1_axis3d);
		break;
	}
	case 0x231e70: {
		return AddSwitch34_50E70(a1_axis3d);
		break;
	}
	case 0x231e90: {
		return AddSwitch35_50E90(a1_axis3d);
		break;
	}
	case 0x231eb0: {
		return AddSwitch36_50EB0(a1_axis3d);
		break;
	}
	case 0x231ed0: {
		return AddSwitch37_50ED0(a1_axis3d);
		break;
	}
	case 0x231ef0: {
		return AddSwitch38_50EF0(a1_axis3d);
		break;
	}
	case 0x231f10: {
		return AddSwitch39_50F10(a1_axis3d);
		break;
	}
	case 0x231f30: {
		return AddSwitch40_50F30(a1_axis3d);
		break;
	}
	case 0x231f50: {
		return AddSwitch41_50F50(a1_axis3d);
		break;
	}
	case 0x231f70: {
		return AddSwitch42_50F70(a1_axis3d);
		break;
	}
	case 0x231f90: {
		return AddSwitch43_50F90(a1_axis3d);
		break;
	}
	case 0x231fb0: {
		return addCheckpoint44_50FB0(a1_axis3d);
		break;
	}
	case 0x231fd0: {
		return AddSwitch30_50FD0(a1_axis3d);
		break;
	}
	case 0x231ff0: {
		return AddSwitch31atyp_50FF0(a1_axis3d);
		break;
	}
	case 0x232120: {
		return AddSpellXX_XX_51120(a1_axis3d, 0, 0); //fix it
		break;
	}
	case 0x2321a0: {
		return AddSpell00_00_511A0(a1_axis3d);
		break;
	}
	case 0x2321c0: {
		return AddSpell01_03_511C0(a1_axis3d);
		break;
	}
	case 0x2321e0: {
		return AddSpell02_06_511E0(a1_axis3d);
		break;
	}
	case 0x232200: {
		return AddSpell03_09_51200(a1_axis3d);
		break;
	}
	case 0x232220: {
		return AddSpell04_12_51220(a1_axis3d);
		break;
	}
	case 0x232240: {
		return AddSpell05_15_51240(a1_axis3d);
		break;
	}
	case 0x232260: {
		return AddSpell06_18_51260(a1_axis3d);
		break;
	}
	case 0x232280: {
		return AddSpell07_21_51280(a1_axis3d);
		break;
	}
	case 0x2322a0: {
		return AddSpell08_24_512A0(a1_axis3d);
		break;
	}
	case 0x2322c0: {
		return AddSpell09_27_512C0(a1_axis3d);
		break;
	}
	case 0x2322e0: {
		return AddSpell10_30_512E0(a1_axis3d);
		break;
	}
	case 0x232300: {
		return AddSpell11_33_51300(a1_axis3d);
		break;
	}
	case 0x232320: {
		return AddSpell12_36_51320(a1_axis3d);
		break;
	}
	case 0x232340: {
		return AddSpell13_39_51340(a1_axis3d);
		break;
	}
	case 0x232360: {
		return AddSpell14_42_51360(a1_axis3d);
		break;
	}
	case 0x232380: {
		return AddSpell15_45_51380(a1_axis3d);
		break;
	}
	case 0x2323a0: {
		return AddSpell16_48_513A0(a1_axis3d);
		break;
	}
	case 0x2323c0: {
		return AddSpell17_51_513C0(a1_axis3d);
		break;
	}
	case 0x2323e0: {
		return AddSpell18_54_513E0(a1_axis3d);
		break;
	}
	case 0x232400: {
		return AddSpell19_57_51400(a1_axis3d);
		break;
	}
	case 0x232420: {
		return AddSpell20_60_51420(a1_axis3d);
		break;
	}
	case 0x232440: {
		return AddSpell21_63_51440(a1_axis3d);
		break;
	}
	case 0x232460: {
		return AddSpell22_66_51460(a1_axis3d);
		break;
	}
	case 0x232480: {
		return AddSpell23_69_51480(a1_axis3d);
		break;
	}
	case 0x2324a0: {
		return AddSpell24_72_514A0(a1_axis3d);
		break;
	}
	case 0x2324c0: {
		return AddSpell25_75_514C0(a1_axis3d);
		break;
	}
	case 0x2324e0: {
		return AddClass0EEntity_514E0(a1_axis3d, 0, 0, 0);//fix it
		break;
	}
	case 0x232530: {
		return AddClass0E_00_51530(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x232570: {
		return AddClass0E_03_51570(a1_axis3d);
		break;
	}
	case 0x2325c0: {
		return AddClass0E_04_515C0(a1_axis3d);
		break;
	}
	case 0x232610: {
		return AddClass0E_05_51610(a1_axis3d);
		break;
	}
	case 0x232660: {
		return AddClass0E_01_51660(a1_axis3d);
		break;
	}
	case 0x2326c0: {
		return AddClass0E_02_516C0(a1_axis3d);
		break;
	}
	case 0x232730: {
		return AddEffectAction72_51730(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	case 0x232790: {
		return AddEffectAction78_51790(a1_axis3d);
		break;
	}
	case 0x232800: {
		return AddEffectIfEntitiesFree_51800(a1_axis3d);
		break;
	}
	case 0x232a00: {
		return AddEffectAction80_51A00(a1_axis3d);
		//return fix_it_4A190();
		break;
	}
	}
	//bad end
	int yy = 0;
	int xx = 157 / yy;
	mydelay(10000);
	exit(1);
	//bad end
	return 0;
}

//----- (0004A190) --------------------------------------------------------
type_entity_0x6E8E* IfSubtypeCallCreatingManaSphere_4A190(axis_3d* position, int type, int subtype)//22b190
{
	if (str_D4C48ar[type].dword_14[subtype].dword_10 && str_D4C48ar[type].dword_14[subtype].word_4 == subtype)
		return pre_sub_4A190_axis_3d(str_D4C48ar[type].dword_14[subtype].address_6, position);
	return 0;
}

//----- (00057E50) --------------------------------------------------------
void SetMapEntity_57E50(type_entity_0x6E8E* entity)//238e50
{
	if (entity->struct_byte_0xc_12_15.byte[0] & 4)
	{
		if (entity->nextEntity_0x18_24)
			D41A0_0.struct_0x6E8E[entity->nextEntity_0x18_24].oldMapEntity_0x16_22 = entity->oldMapEntity_0x16_22;
		else
			mapEntityIndex_15B4E0[(entity->position_0x4C_76.x >> 8) + ((entity->position_0x4C_76.y >> 8) << 8)] = entity->oldMapEntity_0x16_22;
		if (entity->oldMapEntity_0x16_22)
			D41A0_0.struct_0x6E8E[entity->oldMapEntity_0x16_22].nextEntity_0x18_24 = entity->nextEntity_0x18_24;
		entity->struct_byte_0xc_12_15.byte[0] &= 0xFBu;
	}
}

//----- (00057F20) --------------------------------------------------------
void ReleaseEntity_57F20(type_entity_0x6E8E* entity)//238f20
{
	int32_t v1; // edx
	signed int v2; // ecx
	int v3x;
	SetMapEntity_57E50(entity);
	if (entity->struct_byte_0xc_12_15.byte[2] & 2)
	{
		v1 = 0;
		v2 = 0;
		v3x = 0;
		while (v1 <= D41A0_0.dword_0x11e6)
		{
			if (entity == D41A0_0.dword_0x11EA[v3x])
			{
				v2 = 1;
				break;
			}
			v3x++;
			v1++;
		}
		if (v2)
		{
			D41A0_0.dword_0x11EA[v3x] = D41A0_0.dword_0x11EA[D41A0_0.dword_0x11e6];
			D41A0_0.dword_0x11e6--;
		}
	}
	entity->class_0x3F_63 = 0;
	D41A0_0.dword_0x35++;
	D41A0_0.pointers_0x246[D41A0_0.dword_0x35] = entity;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (00069250) --------------------------------------------------------
signed int UpdateSpellPickup_69250(type_entity_0x6E8E* a1x)//24a250
{
	signed int result; // eax
	type_entity_0x6E8E* resultx;
	result = UpdateSpellPickupCommon_68FF0(a1x, a1x->model_0x40_64, a1x->actionIndex_0x45_69 - 2);
	if (result)
	{
		resultx = arsub_2a881e[a1x->model_0x40_64](&a1x->position_0x4C_76);
		//resultx = pre_sub_4A190(0x2a881e + 14 * a1x->byte_0x40_64, (int16_t*)&a1x->array_0x4C_76,2);//result = (*(int(**)(uint8_t*))((char *)&off_D781E + 14 * *(char *)(a1 + 64)))(a1 + 76);
		//v63 = (uint8_t*)pre_sub_4A190(0x232530 + 14 * v112, v113 + 76);//v63 = (uint8_t*)(*(int(**)(uint8_t*))((char *)&off_D781E + 14 * v112))(v113 + 76);
		if (resultx)
			resultx->actionIndex_0x45_69 += 2;
	}
	return result;
}
// D781E: using guessed type int (*off_D781E)(int);

//----- (00049090) --------------------------------------------------------
void CreateLinkedLevelFeatures_49090(Type_Level_2FECE* terrain, type_entity_0x30311* entity)//22a090
{
	type_entity_0x30311* tempEntity; // ebx
	int16_t tempSubtype; // si
	uint16_t tempType; // di
	unsigned __int16 tempY; // cx
	unsigned int v8; // eax
    EventInitFunctionType functionPointer; // [esp+4h] [ebp-10h]
	unsigned __int16 tempX; // [esp+8h] [ebp-Ch]

	tempEntity = entity;
	tempSubtype = entity->subtype_0x30311;
	tempType = entity->type_0x30311;
	functionPointer = nullptr;
	if (entity->type_0x30311 == 0x0A)
	{
		switch (entity->subtype_0x30311)
		{
			case 0x1C:
			{
				functionPointer = &CreateSubtype27LineBetweenPoints_48400;
				break;
			}
			case 0x1D:
			{
				functionPointer = &CreateSubtype30LineBetweenPoints_48690;
				break;
			}
			case 0x1F:
			{
				functionPointer = &CreateSubtype32MovingToPoint_487D0;
				break;
			}
			case 0x32:
			{
				functionPointer = &CreateSubtype51MovingToPoint_48880;
				break;
			}
			case 0x50:				
			{
				functionPointer = &CreateSubtype81WithTarget_48930;
				break;
			}
		}
	}
	if (functionPointer)
	{
		while (1)
		{
			if (!tempEntity->par1_14)
				break;
			tempEntity = &terrain->entity_0x30311[tempEntity->par1_14];
		}
		do
		{
			if (tempType != entity->type_0x30311)
				break;
			if (tempSubtype != entity->subtype_0x30311)
				break;
			tempEntity->stageTag_12 = 0;
			if (!tempEntity->par2_16)
				break;
			tempY = tempEntity->axis2d_4.y;
			tempX = tempEntity->axis2d_4.x;
			v8 = tempEntity->par3_18;
			tempEntity = &terrain->entity_0x30311[tempEntity->par2_16];
			switch (tempSubtype)
			{
				case 0x1F:
				{
					switch (v8)
					{
					case 0u:
						v8 = 2;
						break;
					case 1u:
						v8 = 6;
						break;
					case 2u:
						v8 = 16;
						break;
					case 3u:
						v8 = 32;
						break;
					}
					break;
				}
				case 0x50:
				{
					v8 = tempEntity->par3_18 & 0xF | 16 * (v8 & 0xF);
					break;
				}
			}
			functionPointer(
				tempX,
				tempY,
				tempEntity->axis2d_4.x,
				tempEntity->axis2d_4.y,
				v8);
		} while (tempEntity);
	}
}

//----- (00048400) --------------------------------------------------------
void CreateSubtype27LineBetweenPoints_48400(uint16_t posX2, uint16_t posY2, uint16_t posX, uint16_t posY, uint8_t a5)//229400
{
	int v4; // eax
	int v5; // esi
	int v6; // ebx
	int result; // eax
	int v8; // ecx
	int v9; // eax
	int v10; // eax
	int v11; // edi
	int v12; // esi
	type_entity_0x6E8E* v13x; // eax
	type_entity_0x6E8E* v14x; // eax
	int v15; // ebx
	int v16; // edx
	int v17; // eax
	int v18; // edi
	int v19; // eax
	int v20; // esi
	int v21; // ecx
	type_entity_0x6E8E* v22x; // eax
	int v23; // ebx
	type_entity_0x6E8E* v24x; // eax
	int v25; // [esp+8h] [ebp-Ch]
	int i; // [esp+8h] [ebp-Ch]
	int v27; // [esp+Ch] [ebp-8h]
	int v28; // [esp+Ch] [ebp-8h]
	int v29; // [esp+10h] [ebp-4h]
	int v30; // [esp+10h] [ebp-4h]

	v4 = shortestLenght_48370(posX2, posX, MAP_SIZE);
	v5 = v4;
	v6 = v4;
	result = shortestLenght_48370(posY2, posY, MAP_SIZE);
	v8 = result;
	if (v5 || result)
	{
		if (v6 < 0)
		{
			v9 = posX2;
			v6 = -v6;
			v8 = -v8;
			posX2 = posX;
			posX = v9;
			v10 = posY2;
			posY2 = posY;
			posY = v10;
		}
		if (v6 <= abs(v8))
		{
			v17 = abs(v8 / 10);
			v18 = v17 + 1;
			v19 = v8 / (v17 + 1);
			v20 = v19;
			v21 = v8 - v18 * v19;
			v30 = v6 / v18;
			result = v18 * (v6 / v18);
			v28 = v21;
			for (i = v6 - result; v18; v28 = 0)
			{
				SetPredictedPositionFromTiles_483A0(posX2, posY2, posX, posY);
				v22x = IfSubtypeCallCreatingManaSphere_4A190(&predictedAxis_EB398ar, 10, 27);
				if (v20 >= 0)
				{
					v22x->actionIndex_0x45_69 = 28;
					v23 = v20 + v28;
				}
				else
				{
					v22x->actionIndex_0x45_69 = 27;
					v23 = -v20 - v28;
				}
				v22x->dword_0x10_16 = v23;
				SetPredictedPositionFromTiles_483A0(posX2, (unsigned __int16)(v20 + v28 + posY2), posX, posY);
				v18--;
				posY2 += v20 + v28;
				v24x = IfSubtypeCallCreatingManaSphere_4A190(&predictedAxis_EB398ar, 10, 27);
				v24x->actionIndex_0x45_69 = 29;
				v24x->dword_0x10_16 = i + v30;
				result = 0;
				posX2 += i + v30;
				i = 0;
			}
		}
		else
		{
			v11 = v6 / 10 + 1;
			v29 = v6 / v11;
			v12 = v8 / v11;
			result = v11 * (v8 / v11);
			v25 = v6 - v11 * (v6 / v11);
			v27 = v8 - result;
			if (v6 / 10 != -1)
			{
				do
				{
					SetPredictedPositionFromTiles_483A0(posX2, posY2, posX, posY);
					v13x = IfSubtypeCallCreatingManaSphere_4A190(&predictedAxis_EB398ar, 10, 27);
					v13x->actionIndex_0x45_69 = 29;
					v13x->dword_0x10_16 = v25 + v29;
					posX2 += v25 + v29;
					SetPredictedPositionFromTiles_483A0(posX2, posY2, posX, posY);
					v14x = IfSubtypeCallCreatingManaSphere_4A190(&predictedAxis_EB398ar, 10, 27);
					if (v12 >= 0)
					{
						v14x->actionIndex_0x45_69 = 28;
						v15 = v12 + v27;
					}
					else
					{
						v14x->actionIndex_0x45_69 = 27;
						v15 = -v12 - v27;
					}
					v14x->dword_0x10_16 = v15;
					v11--;
					result = v12 + v27;
					v25 = 0;
					v16 = v12 + v27 + posY2;
					v27 = 0;
					posY2 = v16;
				} while (v11);
			}
		}
	}
}
// EB398: using guessed type __int16 x_WORD_EB398;

//----- (00048690) --------------------------------------------------------
void CreateSubtype30LineBetweenPoints_48690(uint16_t posX2, uint16_t posY2, uint16_t posX, uint16_t posY, uint8_t a5)//229690
{
	int Xdist; // eax
	signed __int16 Xdir; // si
	int Ydist; // eax
	signed __int16 Ydir; // bx
	int maxabsdist; // edi
	type_entity_0x6E8E* v10x; // eax
	type_entity_0x6E8E* resultx; // eax
	int distXYdiff; // [esp+0h] [ebp-14h]
	signed __int16 v14; // [esp+8h] [ebp-Ch]
	signed __int16 v15; // [esp+Ch] [ebp-8h]
	int absYdist; // [esp+10h] [ebp-4h]

	Xdist = shortestLenght_48370(posX2, posX, MAP_SIZE);
	Xdir = 0;
	Ydist = shortestLenght_48370(posY2, posY, MAP_SIZE);
	if (Xdist)
	{
		Xdir = -1;
		if (Xdist > 0)
			Xdir = 1;
	}
	Ydir = 0;
	if (Ydist)
	{
		Ydir = -1;
		if (Ydist > 0)
			Ydir = 1;
	}
	absYdist = abs(Ydist);
	maxabsdist = abs(Xdist);
	distXYdiff = abs(absYdist - maxabsdist);
	if (maxabsdist <= absYdist)
	{
		v15 = Ydir;
		v14 = 0;
	}
	else
	{
		maxabsdist = absYdist;
		v14 = Xdir;
		v15 = 0;
	}
	predictedAxis_EB398ar.x = posX2 << 8;
	predictedAxis_EB398ar.y = posY2 << 8;
	v10x = IfSubtypeCallCreatingManaSphere_4A190(&predictedAxis_EB398ar, 10, 30);
	if (v10x)
	{
		v10x->dword_0x10_16 = maxabsdist;
		v10x->yaw_0x1C_28 = Xdir;
		v10x->pitch_0x1E_30 = Ydir;
	}
	predictedAxis_EB398ar.x = (maxabsdist * Xdir + posX2) << 8;
	predictedAxis_EB398ar.y = (maxabsdist * Ydir + posY2) << 8;
	resultx = IfSubtypeCallCreatingManaSphere_4A190(&predictedAxis_EB398ar, 10, 30);
	if (resultx)
	{
		resultx->dword_0x10_16 = distXYdiff;
		resultx->yaw_0x1C_28 = v14;
		resultx->pitch_0x1E_30 = v15;
	}
}

//----- (000487D0) --------------------------------------------------------
void CreateSubtype32MovingToPoint_487D0(uint16_t posX2, uint16_t posY2, uint16_t posX, uint16_t posY, uint8_t a5)//2297d0
{
	__int16 v5; // si
	unsigned __int16 v6; // bx
	type_entity_0x6E8E* resultx; // eax
	axis_3d v8x; // [esp+0h] [ebp-10h]
	//__int16 v9; // [esp+2h] [ebp-Eh]
	//__int16 v10; // [esp+4h] [ebp-Ch]
	axis_3d v11x; // [esp+8h] [ebp-8h]
	//__int16 v12; // [esp+Ah] [ebp-6h]

	v8x.x = posX2 << 8;
	v8x.y = posY2 << 8;
	v8x.z = 32 * mapHeightmap_11B4E0[256 * posY2 + posX2];
	v11x.x = posX << 8;
	v11x.y = posY << 8;
	v5 = Maths::GetYawAngle_581E0(&v8x, &v11x);
	v6 = Maths::EuclideanDistXYZ_58490(&v8x, &v11x);
	resultx = IfSubtypeCallCreatingManaSphere_4A190(&v8x, 10, 32);
	if (resultx)
	{
		resultx->yaw_0x1C_28 = v5;
		resultx->life_0x8 = (signed int)v6 >> 8;
		resultx->byte_0x46_70 = a5;
	}
}

//----- (00048880) --------------------------------------------------------
void CreateSubtype51MovingToPoint_48880(uint16_t posX2, uint16_t posY2, uint16_t posX, uint16_t posY, uint8_t a5)//229880
{
	__int16 v4; // si
	unsigned __int16 v5; // di
	type_entity_0x6E8E* resultx; // eax
	//int v7; // ebx
	int v8; // ecx
	axis_3d v9x; // [esp+0h] [ebp-10h]
	//__int16 v10; // [esp+2h] [ebp-Eh]
	//__int16 v11; // [esp+4h] [ebp-Ch]
	axis_3d v12x; // [esp+8h] [ebp-8h]
	//__int16 v13; // [esp+Ah] [ebp-6h]

	v9x.x = posX2 << 8;
	v9x.y = posY2 << 8;
	v9x.z = 16 * mapHeightmap_11B4E0[256 * posY2 + posX2];
	v12x.x = posX << 8;
	v12x.y = posY << 8;
	v4 = Maths::GetYawAngle_581E0(&v9x, &v12x);
	v5 = Maths::EuclideanDistXYZ_58490(&v9x, &v12x);
	resultx = IfSubtypeCallCreatingManaSphere_4A190(&v9x, 10, 51);
	//v7x = resultx;
	if (resultx)
	{
		v8 = resultx->actSpeed_0x82_130;
		resultx->yaw_0x1C_28 = v4;
		//result = (uint8_t*)(v5 / v8);
		resultx->life_0x8 = (v5 / v8);
	}
	//return result;
}

int debugcounter_229930 = 0;
//uint16_t last_v8x = 0;
//uint16_t last_a1x = 0;
void CreateSubtype81WithTarget_48930(uint16_t posX2, uint16_t posY2, uint16_t posX, uint16_t posY, uint8_t a5)//229930
{
	type_entity_0x6E8E* result; // eax
	__int16 v6; // [esp+0h] [ebp-10h]
	__int16 v7; // [esp+2h] [ebp-Eh]
	axis_3d v8x; // [esp+8h] [ebp-8h]
	//__int16 v9; // [esp+Ah] [ebp-6h]
	//__int16 v10; // [esp+Ch] [ebp-4h]

	debugcounter_229930++;

	//fixed
	//v10=0;
	v8x.z = 0;// 0x52b0;
	//fixed

	v6 = posX2 << 8;
	v7 = posY2 << 8;
	v8x.x = posX << 8;
	v8x.y = posY << 8;

	//only for debugging
	/*switch (debugcounter_229930) {
	case 0x01:
	{
		last_v8x = 0x35;
		break;
	}
	case 0x04:
	{
		last_v8x = 0x00;
		break;
	}
	case 0x08:
	{
		last_v8x = 0x35;
		break;
	}
	case 0x1e:
	{
		last_v8x = 0x38;
		break;
	}
	case 0x24:
	{
		last_v8x = 0x35;
		break;
	}
	}
	v8x.z = last_v8x;*/
	/*v8x.z = 0x35;
	if ((v8x.x == 0xa500) && (v8x.y == 0x1600))
		v8x.z = 0x35;*/
		//only for debugging

		//result = sub_4A190(&v6, 10, 81);

	type_D4C52ar2* v3x; // eax
	//int resulty; // eax

	//v3 = *(int*)((char*)&dword_D4C56 + 18 * 10) + 14 * 81;
	//v3= (int)&x_DWORD_D4C52ar[*(uint32_t*)&x_DWORD_D4C52ar[4 + 18 * 10] + 81 * 14 - 0x2a5c52];
	//v3 = (int)& x_DWORD_D4C52ar[str_D4C48ar[10].dword_14 + 81 * 14 - 0x2a5c52];
	v3x = &str_D4C48ar[10].dword_14[81];
	//(str_D4C52ar[10]->dword_4)-0x2a5c52
	if (v3x->dword_10 && v3x->word_4/* *(signed __int16*)(v3 + 4)*/ == 81)
	{
		axis_3d a1x;
		a1x.x = v6;
		a1x.y = v7;
		a1x.z = 0;
		//only for debugging
		/*switch (debugcounter_229930) {
		case 0x01:
		{
			last_a1x = 0x2b;
			break;
		}
		case 0x04:
		{
			last_a1x = 0x51d4;
			break;
		}
		case 0x05:
		{
			last_a1x = 0x2b;
			break;
		}
		case 0x1b:
		{
			last_a1x = 0x38;
			break;
		}
		case 0x1e:
		{
			last_a1x = 0x35;
			break;
		}
		case 0x24:
		{
			last_a1x = 0x2b;
			break;
		}
		}
		a1x[2] = last_a1x;*/
		/*a1x[2] = 0x2b;
		if((v6==(int16_t)0x9400)&& (v7 == (int16_t)0x2100))a1x[2] = 0x2b;
		if ((v6 == (int16_t)0xaf00) && (v7 == (int16_t)0x0500))a1x[2] = 0x2b;
		if ((v6 == (int16_t)0xd100) && (v7 == (int16_t)0x2800))a1x[2] = 0x35;*/
		//only for debugging

		result = pre_sub_4A190_axis_3d(v3x->address_6 /** (_DWORD*)(v3 + 6)*/, &a1x); //(*(int(__cdecl * *)(int))(v3 + 6))(a1);
	}
	else
		result = 0;

	//result = pre_sub_4A190(v8x, (int16_t*)& x_WORD_EB398ar, 81);

	//v8x = *(uint32_t*) & (x_DWORD_D4C52ar[(0x2a8b7a + v13 * 14 - 0x2a5c52)]);
	//v14x = pre_sub_4A190(v8x, (int16_t*)& x_WORD_EB398ar, 1); //(*(int(**)(int))((char *)&off_D7B7A + 14 * v13))((int)x_WORD_EB398ar);

	if (result)
	{
		/* *(_DWORD*)(result + 154) = *(_DWORD*)&v8;
		*(_WORD*)(result + 158) = v10;
		*(_BYTE*)(result + 70) = a5;*/
		result->axis_0x9A_154x = v8x;
		result->byte_0x46_70 = a5;
	}
}

//----- (00048370) --------------------------------------------------------
int shortestLenght_48370(int16_t point1, int16_t point2, int16_t mapSize)//229370
{
	int result = point2 - point1;
	if (point2 - point1 > mapSize /2)
		return (result - mapSize);
	if (point2 - point1 < -(mapSize / 2))
		result = mapSize + result;
	return result;
}

//----- (000483A0) --------------------------------------------------------
int SetPredictedPositionFromTiles_483A0(__int16 a1, int a2, char a3, char a4)//2293a0
{
	unsigned __int16 v4; // ax
	__int16 v5; // bx
	unsigned __int16 v6; // ax
	signed int v7; // ecx
	int result; // eax

	LOBYTE(v4) = a1;
	HIBYTE(v4) = a2;
	LOBYTE(v5) = mapHeightmap_11B4E0[v4];
	LOBYTE(v6) = a3;
	v5 = (unsigned __int8)v5;
	HIBYTE(v6) = a4;
	v7 = mapHeightmap_11B4E0[v6];
	if (v7 > (unsigned __int8)v5)
		LOBYTE(v5) = mapHeightmap_11B4E0[v6];
	predictedAxis_EB398ar.x = a1 << 8;
	result = a2 << 8;
	predictedAxis_EB398ar.z = 32 * v5;
	predictedAxis_EB398ar.y = (x_WORD)a2 << 8;
	return result;
}
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39A: using guessed type __int16 x_WORD_EB398ar[1];
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];
