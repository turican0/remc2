#include "GameUI.h"

#include <array>

#include "EventDispatcher.h"
#include "Basic.h"
#include "GameBitmapIndexes.h"
#include "LangTextIndexes.h"
#include "Player.h"
#include "PlayerInput.h"
#include "Terrain.h"
#include "MenusAndIntros.h"
#include "Type_D93C0_Bldgprmbuffer.h"
#include "MapColourIndexs.h"


char FontType_D419D = 1; // weak
int16_t x_WORD_D41D4 = 1; // weak
char* SpellLevelText_DB06C[5] = { (char*)"I",(char*)"II",(char*)"III",(char*)"IV",(char*)"V" };//x_DWORD_DAF50ar[0x11c]//2ac06c
std::array<uint8_t, 25> IndexLevelText_DB4EE = //x_DWORD_DAF50ar[0x59e] //index for levels endings?
{
	0x30, 0x36, 0x3C, 0x42, 0x45,
	0x48, 0x4D, 0x4F, 0x56, 0x5C,
	0x61, 0x66, 0x69, 0x6E, 0x73,
	0x76, 0x7C, 0x7E, 0x83, 0x85,
	0x88, 0x8C, 0x8F, 0x97, 0x9C
};
std::array<uint8_t, 25> LevelEndText_DB507 = ////index for levels endings?
{ 
	0x35, 0x3B, 0x41, 0x44, 0x47, 
	0x4C, 0x4E, 0x55, 0x5B, 0x60, 
	0x65, 0x68, 0x6D, 0x72, 0x75, 
	0x7B, 0x7D, 0x82, 0x84, 0x87, 
	0x8B, 0x8E, 0x96, 0x9C, 0x9E 
};
std::array<uint8_t, 24> x_BYTE_DB520 = 
{ //index for levels endings?
	0x30, 0x33, 0x3F, 0x3D, 0x92, 0x71,
	0x4E, 0x04, 0x01, 0x00, 0x02, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x20, 0x00,
	0x80, 0x00, 0x00, 0x01, 0x00, 0x04 
};
uint8_t xx_array_E36C4[64] = { //fix it in future, this is only for extern control
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
};


void(*ptrDrawBitmap_F01E8)(int16_t, int16_t, bitmap_pos_struct_t, uint8_t scale); // eax

char x_BYTE_D3F48 = 0; // weak
char x_BYTE_D47D8 = 0; // weak
int16_t x_WORD_F4960[1664 * 8]; // Was 1664. Seems to be used in rendering the map 2x indexes ar a time. fix it -  weak
char spellIndex_D94FF[29] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,0,3,0 }; // idb
int16_t x_WORD_E375C_mouse_position_x = 320; // weak
int16_t x_WORD_E375E_mouse_position_y = 200; // weak


void DrawSquareByColor_2E850(int posX, int posY, int width, int height, uint8_t color);
void sub_30870();
void sub_548B0(type_str_0x2BDE* a1);
signed __int16 sub_10010();
void sub_889F0(uint16_t a1);

std::vector<std::string> splitTextIntoWords(const std::string& text);
std::vector<std::string> splitTextIntoLines(const std::string& text, int width);


char* x_InterlockedExchange(char**  Target, char*  Value)
{
	char* temp = *Target;
	*Target = Value;
	return temp;
};

//----- (0002BD10) --------------------------------------------------------
void DrawLine_2BD10(int16_t posX1, int16_t posY1, int16_t posX2, int16_t posY2, uint8_t color, uint8_t thickness)//20cd10
{
	std::function<void(uint16_t, uint16_t, uint16_t, uint16_t, uint8_t)> func_draw;

	//void* v5; // ebx

	//int result; // eax
	uint8_t* temp_screen_buffer; // ST14_4

	if (x_WORD_180660_VGA_type_resolution & 1)
		func_draw = &DrawLine_BD524;
	else
		func_draw = &DrawLine_BD542;
	func_draw(posX1, posY1, posX2, posY2, color);

	//result = (int)x_D41A0_BYTEARRAY_0;
	if (D41A0_0.m_GameSettings.m_Display.m_uiScreenSize == 1)
	{
		temp_screen_buffer = pdwScreenBuffer_351628;
		pdwScreenBuffer_351628 = (uint8_t*)x_DWORD_E9C3C;
		func_draw(posX1, posY1, posX2, posY2, color);
		//result = v7;
		pdwScreenBuffer_351628 = temp_screen_buffer;
	}
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// E9C3C: using guessed type int x_DWORD_E9C3C;
// 180628: using guessed type int pdwScreenBuffer_351628;
// 180660: using guessed type int16_t x_WORD_180660_VGA_type_resolution;

//----- (0002D190) --------------------------------------------------------
void DrawBar_2D190(int16_t posStartX, int16_t posStartY, int16_t maxPosEndX, int16_t posEndY, int16_t posEndX, uint8 colorIdx)//20e190
{
	if (posEndX >= maxPosEndX)
		posEndX = maxPosEndX;
	if (posEndX >= 2)
		DrawLine_2BC80(posStartX, posStartY, posEndX, posEndY, colorIdx);
}

//----- (0002D710) --------------------------------------------------------
void DrawTopStatusBar_2D710(type_entity_0x6E8E* a1x, uint8_t scale)//20e710
{
    int16_t posX; // bx
	type_entity_0x6E8E* v2x; // ecx
	int v3; // ecx
	//int v4; // edx
	int v5; // eax
	//__int64 v6; // rtt
	type_entity_0x6E8E* v9x; // eax
	uint8_t v16; // [esp-4h] [ebp-4Ch]
	int v17; // [esp+0h] [ebp-48h]
	int v18; // [esp+8h] [ebp-40h]
	//type_str_0x6E8E* v19x; // [esp+Ch] [ebp-3Ch]
	//int v20; // [esp+10h] [ebp-38h]
	int16_t v21; // [esp+1Ch] [ebp-2Ch]
	int16_t barStartXPos; // [esp+24h] [ebp-24h]
	type_entity_0x6E8E* v23x; // [esp+28h] [ebp-20h]
	int16_t balloonIdx_v24; // [esp+2Ch] [ebp-1Ch]
	int16_t numBalloons_v25; // [esp+30h] [ebp-18h]
	uint8_t v26; // [esp+34h] [ebp-14h]
	uint8_t manaMaxColourIdx; // [esp+3Ch] [ebp-Ch]
	uint8_t manaColourIdx; // [esp+40h] [ebp-8h]

	manaColourIdx = playersColors_E88E0x[GetTrueWizardNumber_61790(a1x->dword_0xA4_164x->playerColorIndex_0x38_56)][0];
	manaMaxColourIdx = playersColors_E88E0x[GetTrueWizardNumber_61790(a1x->dword_0xA4_164x->playerColorIndex_0x38_56)][1];
	v26 = playersColors_E88E0x[GetTrueWizardNumber_61790(a1x->dword_0xA4_164x->playerColorIndex_0x38_56)][x_D41A0_BYTEARRAY_4_struct.colorIndex_121[2]];
	//ptrDrawBitmap_F01E8(2, 2, **filearray_2aa18c[6] + 240);

	//Panel around MiniMap
	ptrDrawBitmap_F01E8((2 * scale), (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[MINI_MAP_PANEL], scale);

	//v1 = *(uint8_t *)(*filearray_2aa18c[6] + 244) + 2;
	posX = ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[MINI_MAP_PANEL].width_4 * scale) + (2 * scale);
	//adress 20e7db
	v18 = x_D41A0_BYTEARRAY_4_struct.str_index_242ar.dword_4;
	//v20 = a1x->dword_0xA4_164;
	v2x = Entities_EA3E4[a1x->dword_0xA4_164x->CastleEntityIndex_0x3A_58];
	v23x = v2x;
	if (v2x > Entities_EA3E4[0] && v2x->dword_0x10_16)
	{
		if (a1x->dword_0xA4_164x->byte_0x195_405 > 0 && x_D41A0_BYTEARRAY_4_struct.colorIndex_121[1])
		{
			//ptrDrawBitmap_F01E8(v1, 2, **filearray_2aa18c[6] + 330);
			//Draw Castle Health Panel Hit
			ptrDrawBitmap_F01E8(posX, (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[HEALTH_PANEL_HIT], scale);
			a1x->dword_0xA4_164x->byte_0x195_405--;
		}
		else
		{
			//Draw Castle Health Panel
			ptrDrawBitmap_F01E8(posX, (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[HEALTH_PANEL], scale);
		}
		//DrawBitmap_2BB40(v1 + 2, 2, (uint8_t**)(6 * (v23[4] + 43) + **filearray_2aa18c[6]));
		
		//Castle Icon
		DrawBitmap_2BB40(posX + (2 * scale), (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v23x->dword_0x10_16 + 43], scale);
		//Castle Health And Mana Icon
		DrawBitmap_2BB40(posX + (38 * scale), (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[HEALTH_MANA_ICONS], scale);
		barStartXPos = posX + (58 * scale);
		DrawBar_2D190(posX + (58 * scale), 10 * scale, 64 * scale, 10 * scale, ((v23x->life_0x8 << 6) / v23x->maxLife_0x4) * scale, 0x7Bu);
		v17 = (v23x->mana_0x90_144 + a1x->dword_0xA4_164x->dword_0x13C_316) << 6;
		if (v23x->mana_0x90_144 + a1x->dword_0xA4_164x->dword_0x13C_316 == v23x->maxMana_0x8C_140)
		{
			if (x_D41A0_BYTEARRAY_4_struct.colorIndex_121[2] == 1)
			{
				v3 = v18;
				//v4 = v23x->dword_0x8C_140 << 6;
				v16 = manaMaxColourIdx;
				v5 = v23x->maxMana_0x8C_140 << 6;
			}
			else
			{
				v16 = manaColourIdx;
				v5 = (v23x->mana_0x90_144 + a1x->dword_0xA4_164x->dword_0x13C_316) << 6;
				//v4 = (v23x->dword_0x90_144 + a1x->dword_0xA4_164x->dword_0x13C_316) << 6;
				v3 = v18;
			}
		}
		else
		{
			DrawBar_2D190(barStartXPos, 28 * scale, 64 * scale, 10 * scale, ((v23x->maxMana_0x8C_140 << 6) / v18) * scale, manaMaxColourIdx);
			v16 = manaColourIdx;
			v5 = v17;
			//v4 = v17;
			v3 = v18;
		}
		//LODWORD(v6) = v5;
		//HIDWORD(v6) = v4 >> 31;
		DrawBar_2D190(barStartXPos, 28 * scale, 64 * scale, 10 * scale, /*v6*/(v5 / v3) * scale, v16);

		//Draw Mana Goal Lines
		if (!(x_D41A0_BYTEARRAY_4_struct.setting_byte1_22 & Setting::MULTIPLAYER_MODE) && D41A0_0.terrain_2FECE.word_0x2FED5)
		{
			DrawLine_2BC80(barStartXPos + ((D41A0_0.terrain_2FECE.word_0x2FED5 << 6) / 100) * scale, 26 * scale, 2 * scale, 2 * scale, v26);
			DrawLine_2BC80(barStartXPos + ((D41A0_0.terrain_2FECE.word_0x2FED5 << 6) / 100) * scale, 38 * scale, 2 * scale, 2 * scale, v26);
		}
	}
	else
	{
		//Panel Next to MiniMap
		ptrDrawBitmap_F01E8(posX, (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[PANEL], scale);
	}
	//v8 = *(uint8_t *)(**filearray_2aa18c[6] + 250) + v1;
	//adress 20eaa0
	posX = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[HEALTH_PANEL].width_4 * scale + posX;
	if (Entities_EA3E4[0] >= v23x)
	{
		//20eca6
		//VGA_Debug_Blit(640, 480, pdwScreenBuffer_351628);
	    //2nd Panel 
		ptrDrawBitmap_F01E8(posX, (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[PANEL], scale);
		//VGA_Debug_Blit(640, 480, pdwScreenBuffer_351628);
	}
	else
	{
		if (a1x->dword_0xA4_164x->byte_0x197_407 > 0 && x_D41A0_BYTEARRAY_4_struct.colorIndex_121[1])
		{
			//Draw Balloon Health Panel Hit
			ptrDrawBitmap_F01E8(posX, (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[HEALTH_PANEL_HIT], scale);
			a1x->dword_0xA4_164x->byte_0x197_407--;
		}
		else
		{
			//Draw Balloon Health Panel Hit
			ptrDrawBitmap_F01E8(posX, (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[HEALTH_PANEL], scale);
		}
		switch (v23x->dword_0x10_16)
		{
		case 1:
		case 2:
		case 3:
			numBalloons_v25 = 1;
			break;
		case 4:
		case 5:
			numBalloons_v25 = 2;
			break;
		case 6:
		case 7:
			numBalloons_v25 = 3;
			break;
		default:
			numBalloons_v25 = 0;
			break;
		}
		//Draw Balloon Icon
		DrawBitmap_2BB40(posX + (2 * scale), (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[numBalloons_v25 + BALLON1_ICON - 1], scale);
		balloonIdx_v24 = 0;
		//Draw Balloon Health Icon
		DrawBitmap_2BB40(posX + (38 * scale), (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[HEALTH_MANA_ICONS], scale);
		if (numBalloons_v25 > 0)
		{
			do
			{
				v21 = posX + (58 * scale);
				v9x = Entities_EA3E4[a1x->dword_0xA4_164x->array_0x3C_60[balloonIdx_v24]];
				//v19x = v9x;
				if (v9x > Entities_EA3E4[0])
				{
					//Life
					if (v9x->life_0x8 >= 0)
						DrawBar_2D190(v21, (2 * balloonIdx_v24 * scale) + (12 * scale), 64 * scale, 2 * scale, ((v9x->life_0x8 << 6) / v9x->maxLife_0x4) * scale, 0x7Bu);

					//Mana collected
					DrawBar_2D190(v21, (2 * balloonIdx_v24 * scale) + (30 * scale), 64 * scale, 2 * scale, ((v9x->mana_0x90_144 << 6) / v9x->maxMana_0x8C_140) * scale, manaColourIdx);
				}
				balloonIdx_v24++;
			} while (balloonIdx_v24 < numBalloons_v25);
		}
	}
	posX = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[HEALTH_PANEL].width_4 * scale + posX;

	if (a1x->dword_0xA4_164x->PlayerHitFrameTime_406 > 0 && x_D41A0_BYTEARRAY_4_struct.colorIndex_121[1])
	{
		//Player Health Panel on Hit
		ptrDrawBitmap_F01E8(posX, 2 * scale, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[HEALTH_PANEL_HIT], scale);
		a1x->dword_0xA4_164x->PlayerHitFrameTime_406--;
	}
	else
	{//adress 20ed35
		//Player Health Panel
		ptrDrawBitmap_F01E8(posX, 2 * scale, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[HEALTH_PANEL], scale);
	}

	if ((a1x->struct_byte_0xc_12_15.byte[0] & 0x20 || a1x->dword_0xA4_164x->word_0x159_345) && x_D41A0_BYTEARRAY_4_struct.colorIndex_121[2])
	{
		//Draw Player Health and Mana flickering
		GameBitmap::DrawTransparentBitmap_2DE80(posX + (2 * scale), (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[PLAYER_ICON], scale);
		GameBitmap::DrawTransparentBitmap_2DE80(posX + (38 * scale), (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[HEALTH_MANA_ICONS], scale);
	}
	else
	{
		//Draw Player Health and Mana
		DrawBitmap_2BB40(posX + (2 * scale), (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[PLAYER_ICON], scale);
		DrawBitmap_2BB40(posX + (38 * scale), (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[HEALTH_MANA_ICONS], scale);
	}
	//VGA_Debug_Blit(640, 480, pdwScreenBuffer_351628);
	posX = posX + (58 * scale);
	DrawBar_2D190(posX, 10 * scale, 64 * scale, 10 * scale, ((a1x->life_0x8 << 6) / a1x->maxLife_0x4) * scale, 0x7Bu);
	//adress 20ee1a
	//1b8 1c 40 0a 03 aa
	//0xfa00/4c79=3
	//v18=0x4c79
	DrawBar_2D190(posX, 28 * scale, 64 * scale, 10 * scale, ((a1x->maxMana_0x8C_140 << 6) / v18) * scale, manaMaxColourIdx);
	DrawBar_2D190(posX, 28 * scale, 64 * scale, 10 * scale, ((a1x->mana_0x90_144 << 6) / v18) * scale, manaColourIdx);
	//VGA_Debug_Blit(640, 480, pdwScreenBuffer_351628);
}

// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3DC: using guessed type int **filearray_2aa18c[6];
// EA3E4: using guessed type int Entities_EA3E4[];
// F01E8: using guessed type int (*ptrDrawBitmap_F01E8)(x_DWORD, x_DWORD, x_DWORD);
// 180628: using guessed type int pdwScreenBuffer_351628;
// 18062C: using guessed type int screenWidth_18062C;
// 180660: using guessed type int16_t x_WORD_180660_VGA_type_resolution;

//----- (0002E260) --------------------------------------------------------
void DrawSpellIcon_2E260(int16_t posX, int16_t posY, type_entity_0x6E8E* playerEvent, bool altDrawFunction, uint8_t scale)//20f260
{
	if (playerEvent > Entities_EA3E4[0])
	{
		GetFont_6FC50(FontType_D419D);
		type_entity_0x6E8E* parentEvent = Entities_EA3E4[playerEvent->parentId_0x28_40];
		if (parentEvent > Entities_EA3E4[0])
		{
			uint8_t color0 = playersColors_E88E0x[GetTrueWizardNumber_61790(parentEvent->dword_0xA4_164x->playerColorIndex_0x38_56)][0];
			uint8_t color1 = playersColors_E88E0x[GetTrueWizardNumber_61790(parentEvent->dword_0xA4_164x->playerColorIndex_0x38_56)][1];
			if (!(SPELLS_BEGIN_BUFFER_str[playerEvent->model_0x40_64].isEnabled_1 & 4)
				|| playerEvent->word_0x2E_46 <= 0
				|| playerEvent->word_0x2E_46 >= 32
				|| !x_D41A0_BYTEARRAY_4_struct.colorIndex_121[1])
			{
				if (altDrawFunction)//adress 20f334
				{
					bitmap_pos_struct_t bitmap;
					if (playerEvent->word_0x2E_46)
						bitmap = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TOPTILE_BAR_GLOW];
					else
						bitmap = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TOPTILE_BAR];
					DrawBitmap_2BB40(posX, posY, bitmap, scale);
				}
				else
				{
					bitmap_pos_struct_t bitmap;
					if (playerEvent->word_0x2E_46)
						bitmap = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TOPTILE_BAR_GLOW];
					else
						bitmap = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TOPTILE_BAR];
					ptrDrawBitmap_F01E8(posX, posY, bitmap, scale);
				}
				DrawBitmap_2BB40(posX, posY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[playerEvent->model_0x40_64 + SPELL_FIREBALL_BIG], scale);
				DrawText_2BC10((char*)SpellLevelText_DB06C[playerEvent->byte_0x46_70],
					(*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TOPTILE_BAR].width_4 * scale + posX - (8 * scale) * strlen((const char*)*(&SpellLevelText_DB06C[playerEvent->byte_0x46_70])) - (2 * scale),//test and fix
					posY,
					(*xadataclrd0dat.colorPalette_var28)[0],
					scale);
				if (playerEvent->maxMana_0x8C_140)
				{
					DrawLine_2BC80(
						posX + (4 * scale),
						posY + (36 * scale),
						((int64_t)(56 * scale) * (parentEvent->mana_0x90_144 % playerEvent->maxMana_0x8C_140)) / playerEvent->maxMana_0x8C_140,
						(4 * scale),
						color1);
					int manaScaled = parentEvent->mana_0x90_144 / playerEvent->maxMana_0x8C_140;
					for (int i = 0; i < 27 && manaScaled; i++)
					{
						for (int j = 0; j < 2 && manaScaled; j++)
						{
							DrawLine_2BC80(posX + (2 * (i + 2) * scale), posY + (2 * (j + 18) * scale), 2 * scale, 2 * scale, color0);
							manaScaled--;
						}
					}
				}
				if (playerEvent->manaRegen_0x88_136)
					if (!parentEvent->dword_0xA4_164x->CastleEntityIndex_0x3A_58 || playerEvent->manaRegen_0x88_136 > Entities_EA3E4[parentEvent->dword_0xA4_164x->CastleEntityIndex_0x3A_58]->mana_0x90_144)
						if (D41A0_0.terrain_2FECE.MapType != MapType_t::Day)
							DrawSquareByColor_2E850(posX, posY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TOPTILE_BAR].width_4 * scale, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TOPTILE_BAR].height_5 * scale, 16);
						else
							DrawSquareByColor_2E850(posX, posY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TOPTILE_BAR].width_4 * scale, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TOPTILE_BAR].height_5 * scale, 48);
			}
		}
	}
}

//----- (00089830) --------------------------------------------------------
void ComputeTextboxSizes_89830(Type_TextBox_1804B0* textbox, uint8_t scale)//26a830
{
	int16_t preMaxTextboxWidth = textbox->lineSrcX_0x1c + (25 * scale);
	if (textbox->maxPosX_0x16 < textbox->lineSrcX_0x1c + (135 * scale))
		preMaxTextboxWidth = textbox->lineSrcX_0x1c - (135 * scale);

	int16_t preMaxTextboxHeight = textbox->lineSrcY_0x1e - (94 * scale);
	if (textbox->lineSrcY_0x1e - (94 * scale) + (28 * scale) < textbox->minPosY_0x18)
		preMaxTextboxHeight = textbox->lineSrcY_0x1e + (66 * scale);

	textbox->maxTextboxWidth_0x0 = preMaxTextboxWidth;
	textbox->maxTextboxHeight_0x2 = preMaxTextboxHeight;
}

//----- (000898A0) --------------------------------------------------------
void ComputeTextboxLine_898A0(Type_TextBox_1804B0* textbox)//26a8a0
{
	int preLineX2;
	int preLineY2;

	int centerToFrameWidth = textbox->framePosX_0x24;

	if (int lineX1 = textbox->lineSrcX_0x1c; lineX1 >= centerToFrameWidth - 30)
	{
		if (lineX1 <= textbox->frameWidth_0x28 + centerToFrameWidth + 30)
			preLineX2 = textbox->maxTextboxWidth_0x0;
		else
			preLineX2 = textbox->frameWidth_0x28 + textbox->framePosX_0x24;
	}
	else
	{
		preLineX2 = textbox->framePosX_0x24 - 2;
	}
	textbox->lineDestX_0x20 = preLineX2;
	int centerToFrameHeight = textbox->framePosY_0x26;

	if (int lineY1 = textbox->lineSrcY_0x1e; lineY1 >= centerToFrameHeight - 30)
	{
		if (lineY1 > textbox->frameHeight_0x2a + centerToFrameHeight + 30)
		{
			textbox->lineDestY_0x22 = textbox->frameHeight_0x2a + textbox->framePosY_0x26;
			return;
		}
		preLineY2 = textbox->maxTextboxHeight_0x2;
	}
	else
	{
		preLineY2 = textbox->framePosY_0x26 - 2;
	}
	textbox->lineDestY_0x22 = preLineY2;
}

//----- (00089920) --------------------------------------------------------
void ComputeTextboxSizesFromTextLines_89920(Type_TextBox_1804B0* textbox, int16_t countLines, int16_t* textLines, uint8_t scale)//26a920
{
	int maxTextLenght = 0;
	int textLenght;
	for (int indexLine = 0; indexLine < countLines; indexLine++)
	{
		textLenght = strlen((const char*)x_DWORD_E9C4C_langindexbuffer[textLines[indexLine]]) + 1;
		// FireBall | Rapid Fire | Fire Storm		
		if ((textLenght - 1) > maxTextLenght)
			maxTextLenght = textLenght - 1;
	}
	textbox->textBoxWidth_0x4 = (textbox->charWidth_0x10 * scale) * maxTextLenght;
	textbox->textboxHeight_0x6 = (textbox->charHeight_0x12 * scale) * countLines;
}

//----- (00089A30) --------------------------------------------------------
void DrawTextboxLine_89A30(const Type_TextBox_1804B0* textbox)//26aa30
{
	int widthFix = ((x_WORD_180660_VGA_type_resolution & 1) != 0) + 1;
	if (textbox->lineDestX_0x20 != textbox->maxTextboxWidth_0x0 || textbox->lineDestY_0x22 != textbox->maxTextboxHeight_0x2)
	{
		DrawLine_2BD10(textbox->lineSrcX_0x1c, textbox->lineSrcY_0x1e, textbox->lineDestX_0x20, textbox->lineDestY_0x22, *xadataclrd0dat.colorPalette_var28[0]);
		DrawLine_2BD10(textbox->lineSrcX_0x1c + widthFix, textbox->lineSrcY_0x1e, widthFix + textbox->lineDestX_0x20, textbox->lineDestY_0x22, (*xadataclrd0dat.colorPalette_var28)[3840]);
	}
}

//----- (00089AC0) --------------------------------------------------------
void GetHintText_89AC0(char* buffer, int helpIndex)//26aac0
{
	switch (helpIndex) 
	{
	case 92:
		sprintf(buffer, "[F1] %s", x_DWORD_E9C4C_langindexbuffer[14]);//Toggle help on / off
		break;
	case 96:
		sprintf(buffer, (const char*)x_DWORD_E9C4C_langindexbuffer[NUMBER_OF_TIMES_YOU_HAVE_KILLED], std::string(WizardsNames_D93A0[GetTrueWizardNumber_61790(str_unk_1804B0ar.byte_0xa8)]).c_str() );//Number of times you have killed %s
		break;
	default:
		buffer[0] = 0;
		break;
	}
}

//----- (0002E850) --------------------------------------------------------
void DrawSquareByColor_2E850(int posX, int posY, int width, int height, uint8_t color)//20f850
{
	int locPosX = posX;
	int locPosY = posY;
	int locWidth = width;
	int locHeight = height;

	if (x_WORD_180660_VGA_type_resolution == 1)
	{
		locPosX = posX / 2;
		locPosY = posY / 2;
		locWidth = width / 2;
		locHeight = height / 2;
	}
	for (int indexY = locPosY; indexY < locPosY + locHeight; indexY++)
	{
		for (int indexX = locPosX; indexX < locWidth + locPosX; indexX++)
		{
			pdwScreenBuffer_351628[screenWidth_18062C * indexY + indexX] = x_BYTE_F6EE0_tablesx[256 * color
				+ pdwScreenBuffer_351628[screenWidth_18062C * indexY + indexX]];
		}
	}
}

//----- (00030630) --------------------------------------------------------
void DrawCurrentObjectiveTextbox_30630(uint8_t scale)//211630
{
	int result; // eax
	uint16_t v2; // cx
	char* text; // ebx
	Type_TextBox_1804B0 textbox;

	result = 0;
	if (!(x_D41A0_BYTEARRAY_4_struct.setting_byte1_22 & Setting::MULTIPLAYER_MODE) && !(x_D41A0_BYTEARRAY_4_struct.setting_byte1_22 & 0x20))
	{
		if (x_D41A0_BYTEARRAY_4_struct.setting_byte2_23 & 2)
		{
			if (D41A0_0.struct_0x3659C[D41A0_0.LevelIndex_0xc].substr_3659C.IsLevelEnd_0)
				result = x_BYTE_DB520[6];//Toggle help on / off
			else
				result = x_BYTE_DB520[D41A0_0.struct_0x3659C[D41A0_0.LevelIndex_0xc].substr_3659C.ObjectiveText_1];
		}
		else if (D41A0_0.byte_0x36E0B & 1)
		{
			result = (D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].byte_0x3E4_2BE4_12226 != 0) + 284;//Invisible
		}
		else
		{
			v2 = x_D41A0_BYTEARRAY_4_struct.levelnumber_43w;
			if (v2 >= 0x1Eu && v2 <= 0x22u)
			{
				if (D41A0_0.struct_0x3659C[D41A0_0.LevelIndex_0xc].substr_3659C.IsLevelEnd_0)
					result = LevelEndText_DB507[10];
				else
					result = IndexLevelText_DB4EE[0] + 3;
			}
			else if (x_D41A0_BYTEARRAY_4_struct.setting_38545 & 0x20
				&& !D41A0_0.struct_0x3659C[D41A0_0.LevelIndex_0xc].substr_3659C.ObjectiveText_1)
			{
				D41A0_0.byte_counter_current_objective_box_0x36E04 = 0;
			}
			else
			{
				if (D41A0_0.struct_0x3659C[D41A0_0.LevelIndex_0xc].substr_3659C.IsLevelEnd_0)
					result = LevelEndText_DB507[x_D41A0_BYTEARRAY_4_struct.levelnumber_43w];
				else
					result = D41A0_0.struct_0x3659C[D41A0_0.LevelIndex_0xc].substr_3659C.ObjectiveText_1
					+ IndexLevelText_DB4EE[x_D41A0_BYTEARRAY_4_struct.levelnumber_43w];
			}
		}
		if (result)
		{
			text = (char*)x_DWORD_E9C4C_langindexbuffer[result];
			/*"Fly towards my beacon."*/

			textbox.charWidth_0x10 = 8;
			textbox.maxTextboxHeight2_0xe = 400;
			textbox.minPosX_0x14 = 40;
			textbox.maxPosX_0x16 = 600;
			textbox.charHeight_0x12 = 14;
			textbox.maxPosY_0x1a = 260;

			if (!DefaultResolutions())
			{
				textbox.maxPosX_0x16 = screenWidth_18062C - 40;
				textbox.maxPosY_0x1a = screenHeight_180624 / 2;
			}

			textbox.color1_0x30 = (*xadataclrd0dat.colorPalette_var28)[3840];
			textbox.color2_0x31 = ((*xadataclrd0dat.colorPalette_var28)[0]);
			textbox.minPosY_0x18 = 80;
			textbox.maxTextboxWidth_0x0 = 320;
			textbox.maxTextboxHeight_0x2 = 300;
			textbox.maxTextboxWidth2_0xc = 320;

			if (!DefaultResolutions())
			{
				textbox.maxTextboxWidth_0x0 = screenWidth_18062C / 2;
				textbox.maxTextboxHeight_0x2 = screenHeight_180624 / 2 - 100;
				textbox.maxTextboxWidth2_0xc = screenWidth_18062C / 2;
				textbox.maxTextboxHeight2_0xe = screenHeight_180624 / 2;
			}

			ComputeTextboxSizesFromTextWords_89420(&textbox, text, scale);
			ConstrainTextboxSizes_89520(&textbox);
			ComputeFrameSizes_89980(&textbox, scale);
			ColorizeScreen_2E790(textbox.framePosX_0x24, textbox.framePosY_0x26, textbox.frameWidth_0x28, textbox.frameHeight_0x2a, textbox.color2_0x31);
			DrawTextboxText_895D0(&textbox, text, scale);
			DrawTextboxFrame_89690(&textbox, scale);
		}
	}
}

//----- (00041B60) --------------------------------------------------------
void SetPauseMenuClosed_41B60()//222b60
{
	if (IsPauseMenuOpen_D47D9 == 1)
	{
		D41A0_0.m_GameSettings.m_Display.m_uiScreenSize = x_BYTE_D47D8;
		if ((unsigned __int8)x_BYTE_D47D8 >= 1u && (unsigned __int8)x_BYTE_D47D8 <= 1u)
			sub_417D0_install_pal_and_mouse_minmax2();
		x_BYTE_D47D8 = 0;
	}
	if (IsPauseMenuOpen_D47D9)
		IsPauseMenuOpen_D47D9--;
}

//----- (00052E90) --------------------------------------------------------
void SetMenuCursorPosition_52E90(type_str_0x2BDE* playStr, uint16_t newMenuState, bool useSound, uint8_t scale)//233e90
{
	// type == 0 -> hide in-game dialog
	// type == 9 -> show in-game settings dialog
	// type == 13 -> show in-game abandon game yes/no dialog
	uint8_t currentMenuState  = playStr->MenuState_0x3DF_2BE4_12221;
	playStr->MenuState_0x3DF_2BE4_12221 = newMenuState;
	if (playStr->word_0x007_2BE4_11237 != D41A0_0.LevelIndex_0xc)
	{
		sub_53120();
		return;
	}
	x_D41A0_BYTEARRAY_4_struct.byte_38544 = currentMenuState ;
	sub_87C10();
	if (newMenuState)
	{
		SetPausedMenuOpen_41AF0();
	}
	else if (currentMenuState )
	{
		SetPauseMenuClosed_41B60();
	}
	if (newMenuState && (newMenuState < 6u || newMenuState > 7u))
	{
		if (unk_18058Cstr.x_WORD_1805C2_joystick == 7 || unk_18058Cstr.x_WORD_1805C2_joystick == 1 || unk_18058Cstr.x_WORD_1805C2_joystick == 2)
			SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[CURSOR_SPRITE_INDEX_D419E]); // fix it
	}
	else if (x_D41A0_BYTEARRAY_4_struct.OptionsSettingFlag_24 & GAME_PAUSED)
	{
		if (unk_18058Cstr.x_WORD_1805C2_joystick != 7 && unk_18058Cstr.x_WORD_1805C2_joystick != 1 && unk_18058Cstr.x_WORD_1805C2_joystick != 2)
			SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]);
		else
			SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[CURSOR_SPRITE_INDEX_D419E]); // fix it
	}
	else
	{
		SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]);
	}
	switch (newMenuState)
	{
	case (int)MenuState::SHOW_CHAT_MENU:
	case (int)MenuState::SHOW_MAP_SORCERER_SCORES:
	case (int)MenuState::SHOW_IN_GAME_OPTIONS:
	case (int)MenuState::SHOW_VOLUME_OPTIONS:
	case (int)MenuState::SHOW_MAP_GAME_OPTIONS:
	case (int)MenuState::SHOW_MAP_VOLUME_OPTIONS:
	case (int)MenuState::SHOW_OK_CANCEL_OPTIONS:
	case (int)MenuState::SHOW_MAP_OK_CANCEL_OPTIONS:
		D41A0_0.byte_counter_current_objective_box_0x36E04 = 0;  // hide objective message box
		break;
	default:
		break;
	}
	//Close of menu
	switch (currentMenuState )
	{
	case (int)MenuState::NONE:
	case 6u:
		if (newMenuState == currentMenuState && !(x_D41A0_BYTEARRAY_4_struct.OptionsSettingFlag_24 & GAME_PAUSED))
		{
			sub_548B0(playStr);
			EventDispatcher::I->DispatchEvent(EventType::E_SCENE_CHANGE, Scene::FLIGHT);
		}
		break;
	case (int)MenuState::SHOW_CHAT_MENU:
	case (int)MenuState::SHOW_BOTTOM_MENU:
	case (int)MenuState::SHOW_MAP_BOTTOM_MENU:
	case (int)MenuState::SHOW_IN_GAME_OPTIONS:
	case (int)MenuState::SHOW_VOLUME_OPTIONS:
	case (int)MenuState::SHOW_MAP_GAME_OPTIONS:
	case (int)MenuState::SHOW_MAP_VOLUME_OPTIONS:
	case (int)MenuState::SHOW_OK_CANCEL_OPTIONS:
	case (int)MenuState::SHOW_MAP_OK_CANCEL_OPTIONS:
		sub_548B0(playStr);
		if (!(x_D41A0_BYTEARRAY_4_struct.OptionsSettingFlag_24 & GAME_PAUSED))
			EventDispatcher::I->DispatchEvent(EventType::E_SCENE_CHANGE, Scene::FLIGHT);
		FlvInitSet_473B0();
		break;
	default:
		break;
	}
	//Open of Menu
	switch (newMenuState)
	{
	case (int)MenuState::NONE:
	case 6:
		if (newMenuState == currentMenuState  && x_D41A0_BYTEARRAY_4_struct.OptionsSettingFlag_24 & GAME_PAUSED)
		{
			SetCenterScreenForFlyAssistant_6EDB0();
			sub_548F0(playStr);
			break;
		}
		break;
	case (int)MenuState::SHOW_CHAT_MENU:
	case (int)MenuState::SHOW_BOTTOM_MENU:
	case (int)MenuState::SHOW_MAP_BOTTOM_MENU:
		sub_548F0(playStr);
		break;
	case (int)MenuState::SHOW_IN_GAME_OPTIONS:
	case (int)MenuState::SHOW_MAP_GAME_OPTIONS:
		sub_1A280();
		x_D41A0_BYTEARRAY_4_struct.byteindex_225 = 1;
		x_D41A0_BYTEARRAY_4_struct.byteindex_186 = sub_1A4A0();
		sub_548F0(playStr);
		break;
	case (int)MenuState::SHOW_VOLUME_OPTIONS:
	case (int)MenuState::SHOW_MAP_VOLUME_OPTIONS:
		sub_548F0(playStr);
		SetSoundEffectAndMusicLevelCoordinates_19D60(newMenuState);
		break;
	case (int)MenuState::SHOW_OK_CANCEL_OPTIONS:
	case (int)MenuState::SHOW_MAP_OK_CANCEL_OPTIONS:
		sub_548F0(playStr);
		SetOkayCancelButtonsCursorPosition_1A030();
		break;
	default:
		break;
	}
	if (newMenuState >= 5u)
	{
		if (newMenuState > 5u)
		{
			if (newMenuState == (int)MenuState::SHOW_MAP_BOTTOM_MENU)
				useSound = false;
		}

		if (newMenuState == (int)MenuState::SHOW_BOTTOM_MENU || newMenuState == (int)MenuState::SHOW_MAP_BOTTOM_MENU)
		{
			playStr->dword_0x3E6_2BE4_12228.str_611.byte_0x457_1111 = 0;
			MoveCursorToSelectedSpell_6D200(playStr);
			FlvInitSet_473B0();
		}
	}
	if (currentMenuState  >= 5u && (currentMenuState  <= 5u || currentMenuState  == 8))
		useSound = 0;
	switch (newMenuState)
	{
	case 6:
	case 7:
	case (int)MenuState::SHOW_MAP_BOTTOM_MENU:
	case (int)MenuState::SHOW_MAP_GAME_OPTIONS:
	case (int)MenuState::SHOW_MAP_VOLUME_OPTIONS:
	case (int)MenuState::SHOW_MAP_OK_CANCEL_OPTIONS:
		if ((!DefaultResolutions())&&(x_WORD_180660_VGA_type_resolution != 1))
			viewPort.SetViewPortScreenCoordinates_2CA60(384 * scale, 0, screenWidth_18062C - 384, screenHeight_180624 - 80);
		else
			viewPort.SetViewPortScreenCoordinates_2CA60(384, 0, 256, 400);
		break;
	default:
		viewPort.ResizeViewPort_2CA90(D41A0_0.m_GameSettings.m_Graphics.m_wViewPortSize);
		break;
	}
	if (useSound)
		PrepareEventSound_6E450(0, playStr->word_0x007_2BE4_11237, 14);
	sub_53120();
}

//----- (00053120) --------------------------------------------------------
void sub_53120()//234120
{
	if (!(x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26 & 7))
	{
		if ((x_D41A0_BYTEARRAY_4_struct.byteindex_52 & 3) == 1)
		{
			if (!sub_10010())
			{
				D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].byte_0x004_2BE0_11234 = 2;
			}
		}
	}
}

//----- (000595C0) --------------------------------------------------------
char sub_595C0(int16_t a1)//23a5c0
{
	char result = -1;
	if (D41A0_0.byte_0x36E0B & 1)
		return -3;
	if (!D41A0_0.struct_0x3659C[a1].substr_3659C.IsLevelEnd_0)
		result = D41A0_0.stages_0x3654C[D41A0_0.struct_0x3659C[a1].substr_3659C.ObjectiveText_1].stages_3654C_byte0;
	return result;
}

//----- (00059610) --------------------------------------------------------
char sub_59610(type_entity_0x6E8E** a1y, int16_t a2)//23a610
{
	char v5; // dl
	*a1y = 0;
	v5 = 0;
	if (D41A0_0.struct_0x3659C[a2].substr_3659C.IsLevelEnd_0)
		return v5;
	if (D41A0_0.stages_0x3654C[D41A0_0.struct_0x3659C[a2].substr_3659C.ObjectiveText_1].stages_3654C_byte0 < 4u)
	{
		if (D41A0_0.stages_0x3654C[D41A0_0.struct_0x3659C[a2].substr_3659C.ObjectiveText_1].stages_3654C_byte0 < 1u || D41A0_0.stages_0x3654C[D41A0_0.struct_0x3659C[a2].substr_3659C.ObjectiveText_1].stages_3654C_byte0 > 2u)
			return v5;
	}
	else if (D41A0_0.stages_0x3654C[D41A0_0.struct_0x3659C[a2].substr_3659C.ObjectiveText_1].stages_3654C_byte0 > 4u)
	{
		if (D41A0_0.stages_0x3654C[D41A0_0.struct_0x3659C[a2].substr_3659C.ObjectiveText_1].stages_3654C_byte0 == 6 && D41A0_0.stages_0x3654C[D41A0_0.struct_0x3659C[a2].substr_3659C.ObjectiveText_1].str_3654D_byte1 & 1)
		{
			v5 = 1;
			*a1y = Entities_EA3E4[D41A0_0.stages_0x3654C[D41A0_0.struct_0x3659C[a2].substr_3659C.ObjectiveText_1].str_36552_un.dword];
			return v5;
		}
		return v5;
	}
	if (D41A0_0.stages_0x3654C[D41A0_0.struct_0x3659C[a2].substr_3659C.ObjectiveText_1].str_3654D_byte1 & 1)
	{
		v5 = 1;
		*a1y = D41A0_0.stages_0x3654C[D41A0_0.struct_0x3659C[a2].substr_3659C.ObjectiveText_1].str_36552_un.ptr0x6E8E;
	}
	return v5;
}

//----- (000596C0) --------------------------------------------------------
bool sub_596C0(axis_3d* position, int16_t a2)//23a6c0
{
	bool result = false;
	position->x = -1;
	position->y = -1;
	position->z = -1;
	if (!D41A0_0.struct_0x3659C[a2].substr_3659C.IsLevelEnd_0)
	{
		if (D41A0_0.stages_0x3654C[D41A0_0.struct_0x3659C[a2].substr_3659C.ObjectiveText_1].stages_3654C_byte0 >= 4u && D41A0_0.stages_0x3654C[D41A0_0.struct_0x3659C[a2].substr_3659C.ObjectiveText_1].stages_3654C_byte0 <= 5u)
		{
			position->x = D41A0_0.stages_0x3654C[D41A0_0.struct_0x3659C[a2].substr_3659C.ObjectiveText_1].str_3654E_axis.x;
			position->y = D41A0_0.stages_0x3654C[D41A0_0.struct_0x3659C[a2].substr_3659C.ObjectiveText_1].str_3654E_axis.y;
			result = true;
		}
	}
	return result;
}

//----- (000616D0) --------------------------------------------------------
int TransformPlayerColorIndex_616D0(int index)//2426d0
{
	int index2 = 0;
	if (index >= 0 && index <= 7)
	{
		if (x_D41A0_BYTEARRAY_4_struct.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
		{
			switch (D41A0_0.array_0x2BDE[index].dword_0x3E6_2BE4_12228.byte_0x1C0_448)
			{
			case 0:
				index2 = 0;
				break;
			case 1:
				index2 = 4;
				break;
			case 2:
				index2 = 1;
				break;
			case 3:
				index2 = 2;
				break;
			case 4:
				index2 = 5;
				break;
			case 5:
				index2 = 7;
				break;
			case 7:
				index2 = 6;//fix wizard index, in original is 7
				break;
			case 6:
				index2 = 3;
				break;
			default:
				return index2;
			}
		}
		else
		{
			switch (index)
			{
			case 0:
				index2 = 0;
				break;
			case 1:
				index2 = 1;
				break;
			case 2:
				index2 = 4;
				break;
			case 3:
				index2 = 3;
				break;
			case 4:
				index2 = 2;
				break;
			case 5:
				index2 = 5;
				break;
			case 6:
				index2 = 7;
				break;
			case 7:
				index2 = 6;
				break;
			default:
				return index2;
			}
		}
	}
	return index2;
}

uint8_t CalculateScaleOffset(uint8_t scale)
{
	if (scale > 2)
		return (uint8_t)std::floor(((double)scale - 1) / 2);
	return 0;
}

int debugcounter2 = 0;
//----- (00061A00) --------------------------------------------------------
void DrawMinimapEntities_B_61A00(int16_t x, int16_t y, int16_t posX, int16_t posY, uint16_t width, uint16_t height, int16_t yaw, int16_t scaling, uint8_t scale)//242a00
{
	int v8; // edx
	int v9; // esi
	MapType_t v11; // al
	__int64 v12; // rtt
	int v13; // ebx
	int v14; // edx
	int v15; // ebx
	int16_t v16; // dx
	//int v17; // ebx
	int v18; // eax
	int v19; // edx
	int v20; // esi
	int v21; // edi
	type_entity_0x6E8E* v22x; // edx
	int v23; // eax
	int v24; // edx
	int v25; // ebx
	int v26; // ST48_4
	int v27; // edx
	int i; // eax
	int v29; // ebx
	type_entity_0x6E8E* jy; // ebx
	uint8_t v31; // al
	uint8_t v32; // ah
	uint8_t v33; // ah
	uint8_t v34; // ah
	uint16_t v35; // si
	type_entity_0x6E8E* v36x; // eax
	char v37; // al
	char v38; // al
	type_entity_0x6E8E* v39x; // eax
	uint16_t v40; // si
	type_entity_0x6E8E* v41x; // eax
	uint8_t v42; // ah
	char v43; // alv39x
	uint8_t v44; // ah
	int v45; // eax
	uint8_t v46; // al
	char v47; // al
	char v48; // dl
	char v49; // al
	uint8_t v50; // ah
	int16_t v51; // ax
	int v52; // edx
	int v53; // ecx
	int posx; // esi
	int posy; // edi
	char* ptrBlipBufferIdx_v56; // edx
	int v57; // ecx
	char* v58; // edx
	int v59; // eax
	type_entity_0x6E8E* v60x; // ebx
	int v61; // edx
	int v62; // eax
	int v63; // esi
	int v64; // edi
	uint8_t* v65; // ST14_4
	uint8_t v66; // al
	int v72; // [esp+0h] [ebp-60h]
	int v73; // [esp+8h] [ebp-58h]
	type_entity_0x6E8E* v74x; // [esp+Ch] [ebp-54h]
	signed int v75; // [esp+10h] [ebp-50h]
	int v76; // [esp+14h] [ebp-4Ch]
	signed int v78; // [esp+1Ch] [ebp-44h]
	//int v79; // [esp+20h] [ebp-40h]
	type_entity_0x6E8E* v80x; // [esp+24h] [ebp-3Ch]
	int v82; // [esp+2Ch] [ebp-34h]
	int v83; // [esp+30h] [ebp-30h]
	uint8_t* ptrMapBufferStart_v84; // [esp+34h] [ebp-2Ch]
	int v85; // [esp+38h] [ebp-28h]
	int v86; // [esp+3Ch] [ebp-24h]
	uint16_t v87; // [esp+40h] [ebp-20h]
	uint16_t v88; // [esp+44h] [ebp-1Ch]
	char v89; // [esp+48h] [ebp-18h]
	uint8_t v90; // [esp+4Ch] [ebp-14h]
	uint8_t v91; // [esp+50h] [ebp-10h]
	uint8_t v92; // [esp+54h] [ebp-Ch]
	uint8_t CentreCrossColour_v93; // [esp+58h] [ebp-8h]
	char BlipColourIdx_v94; // [esp+5Ch] [ebp-4h]

	if (debugcounter2 >= 8)
	{
		debugcounter2 = debugcounter2;
	}
	debugcounter2++;

	v8 = x;
	v9 = y;
	v11 = D41A0_0.terrain_2FECE.MapType;
	v83 = 0;
	if (v11 == MapType_t::Day)
	{
		v92 = (*xadataclrd0dat.colorPalette_var28)[0];
		CentreCrossColour_v93 = (*xadataclrd0dat.colorPalette_var28)[0];
		v91 = 0xe8;
		v90 = 0x1c;
	}
	else if (v11 == MapType_t::Night)
	{
		v92 = (*xadataclrd0dat.colorPalette_var28)[4095];
		CentreCrossColour_v93 = (*xadataclrd0dat.colorPalette_var28)[4095];
		v91 = 0xe8;//0xe8
		v90 = 0x84;//0x84
	}
	else if (v11 == MapType_t::Cave)
	{
		v92 = (*xadataclrd0dat.colorPalette_var28)[4095];
		CentreCrossColour_v93 = (*xadataclrd0dat.colorPalette_var28)[4095];
		v91 = 0x1c;
		v90 = (*xadataclrd0dat.colorPalette_var28)[240];
	}
	if (x_WORD_180660_VGA_type_resolution & 1)
	{
		v8 = x >> 1;
		v9 = y >> 1;
		scaling *= 2;
		width >>= 1;
		height >>= 1;
	}

	ptrMapBufferStart_v84 = v9 * screenWidth_18062C + pdwScreenBuffer_351628 + v8;
	LODWORD(v12) = 0x10000;
	HIDWORD(v12) = 0x10000 >> 31;
	v13 = v12 / scaling;
	v82 = width / 2;
	v14 = v13 * Maths::sin_DB750[yaw & 0x7FF];
	v76 = height / 2;
	v15 = (x_DWORD)Maths::sin_DB750[0x200 + yaw & 0x7FF] * v13;
	v86 = -v14 >> 16;
	GetFont_6FC50(FontType_D419D);
	v73 = v15 >> 16;
	v80x = Entities_EA3E4[D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
	v16 = v80x->dword_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[12];
	if (v16)
		v83 = Entities_EA3E4[v16]->word_0x2E_46;
	//v17 = v80x->dword_0xA4_164;
	if (v80x->dword_0xA4_164x->CastleEntityIndex_0x3A_58)
	{
		v18 = (int16_t)(v80x->position_0x4C_76.x - posX);
		v19 = (int16_t)(v80x->position_0x4C_76.y - posY);
		v20 = v82 + ((v18 * v73 - v19 * v86) >> 16);
		v21 = ((v73 * v19 + v86 * v18) >> 16) + v76;
		v22x = Entities_EA3E4[v80x->dword_0xA4_164x->CastleEntityIndex_0x3A_58];
		v23 = (int16_t)(v22x->position_0x4C_76.x - posX);
		v24 = (int16_t)(v22x->position_0x4C_76.y - posY);
		v25 = v82 + ((v23 * v73 - v24 * v86) >> 16);
		v26 = ((v73 * v24 + v86 * v23) >> 16) + v76;
		v27 = v82 + ((v23 * v73 - v24 * v86) >> 16) - v20;
		v75 = Maths::sub_7277A_radix_3d(v27 * v27 + (v26 - v21) * (v26 - v21));
		v72 = (uint16_t)Maths::sub_72633_maybe_tan(v25 - v20, v26 - v21);
		for (i = D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].Turn_2BE0_11248 & 3;
			;
			*(x_BYTE*)(ptrMapBufferStart_v84 + v20 + (i * Maths::sin_DB750[v72] >> 16) + screenWidth_18062C * v85) = x_BYTE_F6EE0_tablesx[0x4000 + 256
			* *(uint8_t*)(ptrMapBufferStart_v84 + v20 + (i * Maths::sin_DB750[v72] >> 16) + screenWidth_18062C * v85)
			+ (uint8_t)(*xadataclrd0dat.colorPalette_var28)[4095]])//castle rope
		{
			i += 4;
			if (i > v75)
				break;

			if (v72 < 0 || v72 > 2560)
				break;

			v29 = v20 + (i * Maths::sin_DB750[v72] >> 16);
			//v29 = v20 + (i * Maths::x_DWORD_DB750ar_ret(4*v72) >> 16);

			if (v72 < 0 || 0x200 + v72 >= 2560)
				break;

			v85 = v21 + (-i * Maths::sin_DB750[0x200 + v72] >> 16);

			if ((1 + 2 * v85) < 0 || (1 + 2 * v85) > 1664 * 8)
				break;

			if (v29 < 0 || v29 >= width || v85 < 0 || v85 >= height || v29 < x_WORD_F4960[1 + 2 * v85] || v29 >= x_WORD_F4960[2 * v85])
				break;
		}
	}
	//for all entites
	//VGA_Debug_Blit(640, 480, pdwScreenBuffer_351628);
	//for (jy = &D41A0_BYTESTR_0.struct_0x6E8E[1]; jy <= &D41A0_BYTESTR_0.struct_0x6E8E[0x3e8-1]; jy += sizeof(type_str_0x6E8E))
	for (jy = &D41A0_0.struct_0x6E8E[1]; jy <= &D41A0_0.struct_0x6E8E[0x3e8 - 1]; jy++)
	{
		//a3 //adress 242cf9 20-ok 80-ok e0-ok e1-ok e2-ok

		v31 = 0;
		v89 = 0;
		v78 = 1;
		v32 = jy->class_0x3F_63 - 2;//entity type
		v88 = 0;
		if (v32 <= 0xDu)
		{
			switch (v32)
			{
			case 0u:
				v42 = jy->model_0x40_64;//entity subtype
				if (v42 > 3u)
				{
				LABEL_78:
					v38 = v90;
					goto LABEL_121;
				}
				switch (v42)
				{
				case 0u:
					if (jy->actionIndex_0x45_69 != 2)
						goto LABEL_78;
					BlipColourIdx_v94 = v90;
					v31 = 0;
					break;
				case 1u:
					v31 = 1;
					if (x_D41A0_BYTEARRAY_4_struct.colorIndex_121[3])
						BlipColourIdx_v94 = (*xadataclrd0dat.colorPalette_var28)[MapColourIndexs::MARKER_STONE];
					else
						v89 = 1;
					v88 = 27;
					break;
				case 2u:
					if (x_D41A0_BYTEARRAY_4_struct.colorIndex_121[2])
						v43 = (*xadataclrd0dat.colorPalette_var28)[MapColourIndexs::UNPOSSESSED_BUILDING];
					else
						v43 = v90;
					BlipColourIdx_v94 = v43;
					v31 = 1;
					v88 = 22;
					break;
				case 3u:
					goto LABEL_123;
				}
				break;
			case 1u:
				v50 = jy->model_0x40_64;
				if (v50 >= 2u)
				{
					if (v50 <= 2u)
					{
						LOBYTE(v51) = jy->id_0x1A_26 != v80x->id_0x1A_26;
						HIBYTE(v51) = 0;
						v88 = v51 + 24;
						v31 = TransformPlayerColorIndex_616D0(Entities_EA3E4[jy->id_0x1A_26]->dword_0xA4_164x->playerColorIndex_0x38_56)//castle rope
							+ 58;
					}
					else if (v50 == 3)
					{
						if (jy->id_0x1A_26 == v80x->id_0x1A_26 || v83)
							v31 = TransformPlayerColorIndex_616D0(Entities_EA3E4[jy->id_0x1A_26]->dword_0xA4_164x->playerColorIndex_0x38_56)
							+ 66;
						if (jy->id_0x1A_26 == v80x->id_0x1A_26)
						{
							v88 = 23;
						}
						else if (v83)
						{
							v88 = 26;
						}
					}
				}
				break;
			case 2u:
			case 4u:
			case 5u:
			case 6u:
			case 0xBu:
				break;
			case 3u:
				v44 = jy->actionIndex_0x45_69;
				if (v44 >= 0xB4u && (v44 <= 0xB4u || v44 >= 0xE8u && v44 <= 0xEAu) || jy->struct_byte_0xc_12_15.byte[0] & 1)
					break;
				v45 = jy->id_0x1A_26;
				if (v45 != jy - D41A0_0.struct_0x6E8E)
				{
					v47 = playersColors_E88E0x[GetTrueWizardNumber_61790(Entities_EA3E4[v45]->dword_0xA4_164x->playerColorIndex_0x38_56)][1];
					goto LABEL_102;
				}
				v46 = jy->model_0x40_64;
				if (v46 >= 0xCu)
				{
					if (v46 <= 0xEu)
					{
						v47 = (*xadataclrd0dat.colorPalette_var28)[MapColourIndexs::CIVILIANS];
					LABEL_102:
						BlipColourIdx_v94 = v47;
						goto LABEL_122;
					}
					if (v46 == 22)
					{
						if (jy->playerEntityIndex_0x94_148 != v80x->id_0x1A_26)
							v88 = 18;
						v47 = v92;
						goto LABEL_102;
					}
				}
				v48 = jy->StageVar2_0x49_73;
				BlipColourIdx_v94 = v92;
				v49 = 1;
				if ((v48 == 14 || v48 == 13) && jy->parentId_0x28_40 == v80x->id_0x1A_26)
					v49 = 0;
				if (v49)
					v88 = 19;
				goto LABEL_122;
			case 7u:
				goto LABEL_56;
			case 8u:
				v34 = jy->model_0x40_64;
				if (v34 < 0x27u)
				{
					if (v34 >= 0x12u)
					{
						if (v34 <= 0x12u)
							break;
						if (v34 == 34)
							v78 = 2;
					}
					goto LABEL_56;
				}
				if (v34 <= 0x27u)
					goto LABEL_173;
				if (v34 < 0x39u)
				{
					if (v34 != 45)
						goto LABEL_56;
					v40 = jy->playerEntityIndex_0x94_148;
					if (v40)
					{
						v41x = Entities_EA3E4[v40];
						if (v41x->class_0x3F_63 == 3)
						{
							if (x_D41A0_BYTEARRAY_4_struct.colorIndex_121[3])
								v38 = playersColors_E88E0x[GetTrueWizardNumber_61790(v41x->dword_0xA4_164x->playerColorIndex_0x38_56)][0];
							else
								v38 = playersColors_E88E0x[GetTrueWizardNumber_61790(v41x->dword_0xA4_164x->playerColorIndex_0x38_56)][1];
							goto LABEL_121;
						}
					}
					if (!(str_D93C0_bldgprmbuffer[jy->byte_0x46_70].byte_2 & 8))
						v88 = 21;
				LABEL_56:
					v39x = Entities_EA3E4[jy->id_0x1A_26];
					if (v39x->class_0x3F_63 == 3)
						v38 = playersColors_E88E0x[GetTrueWizardNumber_61790(v39x->dword_0xA4_164x->playerColorIndex_0x38_56)][0];
					else
						v38 = (*xadataclrd0dat.colorPalette_var28)[MapColourIndexs::UNPOSSESSED_BUILDING2];
				LABEL_121:
					BlipColourIdx_v94 = v38;
					goto LABEL_122;
				}
				if (v34 <= 0x39u)
				{
				LABEL_173:
					v35 = jy->playerEntityIndex_0x94_148;
					if (v35 && (v36x = Entities_EA3E4[v35], v36x->class_0x3F_63 == 3))
					{
						if (x_D41A0_BYTEARRAY_4_struct.colorIndex_121[3])
							v37 = playersColors_E88E0x[GetTrueWizardNumber_61790(v36x->dword_0xA4_164x->playerColorIndex_0x38_56)][0];
						else
							v37 = playersColors_E88E0x[GetTrueWizardNumber_61790(v36x->dword_0xA4_164x->playerColorIndex_0x38_56)][1];
						BlipColourIdx_v94 = v37;
					}
					else
					{
						BlipColourIdx_v94 = v91;
						v88 = 18;
					}
				LABEL_122:
					v31 = 1;
				}
				else
				{
					if (v34 < 0x4Eu)
						goto LABEL_56;
					if (v34 <= 0x4Eu)
					{
						if (jy->word_0x32_50 != D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240)
							break;
						v38 = playersColors_E88E0x[GetTrueWizardNumber_61790(v80x->dword_0xA4_164x->playerColorIndex_0x38_56)][x_D41A0_BYTEARRAY_4_struct.colorIndex_121[3]];
						goto LABEL_121;
					}
					if (v34 < 0x56u || v34 > 0x57u)
						goto LABEL_56;
				}
				break;
			case 9u:
				v33 = jy->model_0x40_64;
				if (v33 <= 0x2Cu)
				{
					switch (v33)
					{
					case 0u:
					case 1u:
					case 2u:
					case 3u:
					case 4u:
					case 5u:
					case 6u:
					case 7u:
					case 8u:
					case 9u:
					case 0xAu:
					case 0xBu:
					case 0xDu:
					case 0xEu:
					case 0xFu:
					case 0x10u:
					case 0x11u:
					case 0x12u:
					case 0x13u:
					case 0x14u:
					case 0x15u:
					case 0x16u:
					case 0x17u:
					case 0x18u:
					case 0x19u:
					case 0x1Au:
					case 0x1Bu:
					case 0x1Cu:
					case 0x1Du:
					case 0x1Eu:
					case 0x20u:
					case 0x21u:
					case 0x22u:
					case 0x23u:
					case 0x24u:
					case 0x25u:
					case 0x26u:
					case 0x27u:
					case 0x28u:
					case 0x29u:
					case 0x2Au:
					case 0x2Bu:
					case 0x2Cu:
						goto LABEL_123;
					case 0xCu:
						v31 = 83;
						v88 = 83;
						break;
					case 0x1Fu:
						v31 = 84;
						v88 = 83;
						break;
					}
				}
				break;
			case 0xAu:
			case 0xDu:
				if (jy->struct_byte_0xc_12_15.byte[0] & 1)
					break;
				v38 = (*xadataclrd0dat.colorPalette_var28)[MapColourIndexs::SPELLS];
				v88 = 20;
				goto LABEL_121;
			case 0xCu:
				if (jy->model_0x40_64 != 5)
					break;
				if (x_D41A0_BYTEARRAY_4_struct.colorIndex_121[3])
					v38 = (*xadataclrd0dat.colorPalette_var28)[MapColourIndexs::SPELLS];
				else
					v38 = (*xadataclrd0dat.colorPalette_var28)[MapColourIndexs::CREATURE];
				goto LABEL_121;
			}
		}
	LABEL_123:
		if (v31)//adress 243207
		{
			v52 = (int16_t)(jy->position_0x4C_76.x - posX);
			v53 = (int16_t)(jy->position_0x4C_76.y - posY);
			posx = v82 + ((v52 * v73 - v53 * v86) >> 16);
			posx -= CalculateScaleOffset(scale);
			if (posx >= 0 && posx < width)
			{//24324e
				posy = ((v73 * v53 + v86 * v52) >> 16) + v76;
				posy -= CalculateScaleOffset(scale);
				if (posy >= 0 && posy < height && posx >= x_WORD_F4960[1 + 2 * posy] && posx < x_WORD_F4960[2 * posy])
				{//adress 24329e 0x1e xx 0x17
					ptrBlipBufferIdx_v56 = (char*)(posx + ptrMapBufferStart_v84 + posy * screenWidth_18062C);
					if (!v89)
					{
						if (v31 <= 1u)
						{
							int blipSize = 1;
							if (v78 > 1)
								blipSize = 2;

							for (int y = 0; y < (blipSize * scale); y++)
							{
								for (int x = 0; x < (blipSize * scale); x++)
								{
									ptrBlipBufferIdx_v56[x] = BlipColourIdx_v94;
								}
								ptrBlipBufferIdx_v56 += screenWidth_18062C;
							}
						}
						else if (x_WORD_180660_VGA_type_resolution == 1)
						{
							if (v31 < 0x42u || v31 >= 0x4Au)
							{
								if (v31 < 0x3Au || v31 >= 0x42u)
								{
									if (v31 == 83 || v31 == 84)
										DrawBitmap_2BB40(
											2 * posx - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v31].width_4 / 2,
											2 * posy - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v31].height_5 / 2,
											(*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v31]);
								}
								else
								{
									DrawBitmap_2BB40(2 * posx, 2 * posy - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v31].height_5, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v31]);
								}
							}
							else
							{
								DrawBitmap_2BB40(
									2 * posx - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v31].width_4 / 2,
									2 * posy - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v31].height_5,
									(*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v31]);
							}
						}
						else if (v31 >= 0x3Cu)
						{
							//Draw other stuff (ballon icons)
							DrawBitmap_2BB40(
								posx - ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v31].width_4 / 2) * scale,
								posy - ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v31].height_5 * scale),
								(*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v31], scale);
						}
						else
						{
							//Draw Flag
							
							//DrawBitmap_2BB40(posx, posy - *(uint8_t *)(6 * v31 + **filearray_2aa18c[6] + 5), (uint8_t**)(6 * v31 + **filearray_2aa18c[6]));
							DrawBitmap_2BB40(posx, posy - ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v31].height_5 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v31], scale);
						}
					}
					if (v88)
						sub_885E0(jy, (int16_t)posx, (int16_t)posy, v88);
				}
			}
		}
	}

	if (v83)//player cross or other
	{
		v87 = 0;
		v74x = Entities_EA3E4[D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
		while (v87 < D41A0_0.NumberOfPlayers_0xe)
		{
			if (v87 != D41A0_0.LevelIndex_0xc)
			{
				//v79 = 2124 * v87;
				v59 = D41A0_0.array_0x2BDE[v87].playerIndex_0x00a_2BE4_11240;
				v60x = Entities_EA3E4[v59];
				if (v60x->life_0x8 >= 0)
				{
					if (sub_63570(v74x, Entities_EA3E4[v59]))
					{
						v61 = (int16_t)(v60x->position_0x4C_76.x - posX);
						v62 = (int16_t)(v60x->position_0x4C_76.y - posY);
						v63 = v82 + ((v61 * v73 - v62 * v86) >> 16);
						if (v63 >= 0 && v63 < width)
						{
							v64 = v76 + ((v86 * v61 + v73 * v62) >> 16);
							if (v64 >= 0 && v64 < height && v63 >= x_WORD_F4960[1 + 2 * v64] && v63 < x_WORD_F4960[2 * v64])
							{
								v65 = (uint8_t*)(v64 * screenWidth_18062C + v63 + ptrMapBufferStart_v84);
								v66 = playersColors_E88E0x[GetTrueWizardNumber_61790(v60x->dword_0xA4_164x->playerColorIndex_0x38_56)][0];
								*v65 = v66;
								if (x_WORD_180660_VGA_type_resolution == 1)
									DrawText_2BC10(D41A0_0.array_0x2BDE[v87].WizardName_0x39f_2BFA_12157, 2 * v63 + 4, 2 * v64, v66);
								else
									DrawText_2BC10(D41A0_0.array_0x2BDE[v87].WizardName_0x39f_2BFA_12157, v63 + 4, v64, v66);
							}
						}
					}
				}
			}
			v87++;
		}
	}

	//Draw center Cross
	if (scale == 1)
	{
		int v71 = 0;
		int crossWidth_v67 = width / 12;
		uint8_t* ptrCrossWest_v68 = (uint8_t*)(screenWidth_18062C * (int)(height / 2) + width / 2 + ptrMapBufferStart_v84 - 1);
		char* index = (char*)&x_BYTE_F6EE0_tablesx[0x4000 + 256 * CentreCrossColour_v93];
		uint8_t* ptrCrossSouth_v70 = (uint8_t*)(screenWidth_18062C * (int)(height / 2) + width / 2 + ptrMapBufferStart_v84 - 1);
		uint8_t* ptrCrossEast_v77 = (uint8_t*)(screenWidth_18062C * (int)(height / 2) + width / 2 + ptrMapBufferStart_v84 - 1);
		uint8_t* ptrCrossNorth_v81 = (uint8_t*)(screenWidth_18062C * (int)(height / 2) + width / 2 + ptrMapBufferStart_v84 - 1);

		for (*ptrCrossWest_v68 = index[(uint8_t)*ptrCrossWest_v68]; crossWidth_v67; *ptrCrossWest_v68 = x_BYTE_F6EE0_tablesx[0x4000 + 256 * CentreCrossColour_v93 + v71])
		{
			ptrCrossNorth_v81 -= screenWidth_18062C;
			ptrCrossSouth_v70 += screenWidth_18062C;
			ptrCrossEast_v77++;
			*ptrCrossNorth_v81 = x_BYTE_F6EE0_tablesx[0x4000 + 256 * CentreCrossColour_v93 + (uint8_t)*ptrCrossNorth_v81];
			*ptrCrossEast_v77 = x_BYTE_F6EE0_tablesx[0x4000 + 256 * CentreCrossColour_v93 + (uint8_t)*ptrCrossEast_v77];
			*ptrCrossSouth_v70 = x_BYTE_F6EE0_tablesx[0x4000 + 256 * CentreCrossColour_v93 + (uint8_t)*ptrCrossSouth_v70];
			v71 = (uint8_t) * (ptrCrossWest_v68-- - 1);
			crossWidth_v67--;
		}
		return;
	}

	//Draw center Cross at other scales (2x or greater)
	int crossWidth_v67 = width / 12;

	uint8_t* crossCenter = (uint8_t*)(screenWidth_18062C * ((int)height / 2) + width / 2 + ptrMapBufferStart_v84 - 1);

	for (int s = 0; s < scale; s++)
	{
		uint8_t* ptrCrossWest_v68 = (crossCenter - CalculateScaleOffset(scale)) + (screenWidth_18062C * s);
		uint8_t* ptrCrossEast_v77 = (crossCenter + (scale - CalculateScaleOffset(scale)) + 1) + (screenWidth_18062C * s);

		for (int i = 0; i < crossWidth_v67; i++)
		{
			*ptrCrossWest_v68 = x_BYTE_F6EE0_tablesx[0x4000 + 256 * CentreCrossColour_v93 + (uint8_t)*ptrCrossWest_v68];
			*ptrCrossEast_v77 = x_BYTE_F6EE0_tablesx[0x4000 + 256 * CentreCrossColour_v93 + (uint8_t)*ptrCrossEast_v77];
			ptrCrossWest_v68--;
			ptrCrossEast_v77++;
		}
	}

	for (int s = 0; s < scale; s++)
	{
		uint8_t* ptrCrossNorth_v81 = (crossCenter - CalculateScaleOffset(scale)) + 1 + s;
		uint8_t* ptrCrossSouth_v70 = ((crossCenter - CalculateScaleOffset(scale)) + screenWidth_18062C) + 1 + s;

		for (int i = 0; i < crossWidth_v67; i++)
		{
			*ptrCrossNorth_v81 = x_BYTE_F6EE0_tablesx[0x4000 + 256 * CentreCrossColour_v93 + (uint8_t)*ptrCrossNorth_v81];
			*ptrCrossSouth_v70 = x_BYTE_F6EE0_tablesx[0x4000 + 256 * CentreCrossColour_v93 + (uint8_t)*ptrCrossSouth_v70];
			ptrCrossNorth_v81 -= screenWidth_18062C;
			ptrCrossSouth_v70 += screenWidth_18062C;
		}
	}
}

//----- (000627F0) --------------------------------------------------------
void sub_627F0_draw_minimap_entites_a(int16_t x, int16_t y, int16_t posX, int16_t posY, uint16_t width, uint16_t height, int16_t yaw, int16_t scaling, uint8_t scale)//2437f0
{//
	int v8; // edx
	int v9; // esi
	int v10; // ebx
	MapType_t v11; // al
	char v12; // al
	__int64 v13; // rtt
	int v14; // ebx
	int v15; // edx
	int v16; // ebx
	int16_t v17; // dx
	//int v18; // ebx
	int v19; // edx
	int v20; // eax
	int v21; // esi
	int v22; // edx
	type_entity_0x6E8E* v23x; // eax
	int v24; // edi
	int v25; // edx
	int v26; // eax
	int v27; // ebx
	int v28; // ST48_4
	int v29; // edx
	int v30; // eax
	int v31; // ebx
	uint8_t* v32x; // edx
	uint8_t* v33x; // ST44_4
	char v34; // bl
	type_entity_0x6E8E* iy; // ebx
	uint8_t v36; // al
	uint8_t v37; // ah
	uint8_t v38; // ah
	uint8_t v39; // ah
	uint16_t v40; // si
	type_entity_0x6E8E* v41x; // eax
	char v42; // al
	char v43; // al
	type_entity_0x6E8E* v44x; // eax
	uint16_t v45; // si
	type_entity_0x6E8E* v46x; // eax
	//bool v47; // zf
	uint8_t v48; // ah
	uint8_t v49; // ah
	int v50; // eax
	uint8_t v51; // al
	char v52; // al
	char v53; // al
	char v54; // ah
	uint8_t v55; // ah
	int16_t v56; // ax
	int v57; // edx
	int v58; // ecx
	int v59; // esi
	int v60; // edi
	uint8_t* v61x; // ecx
	uint8_t* v62x; // edx
	uint8_t* v63; // edx
	uint8_t* v64; // ecx
	int v65; // eax
	type_entity_0x6E8E* v66x; // ebx
	int v67; // edx
	int v68; // eax
	int v69; // esi
	int v70; // edi
	uint8_t* v71; // ST2C_4
	uint8_t* v72; // ST28_4
	uint8_t v73; // al
	int v74; // eax
	uint8_t* v75; // esi
	uint8_t* v76; // ebx
	char* index; // eax
	uint8_t* v78; // edx
	char v79; // cl
	char v80; // cl
	char v81; // cl
	char v82; // cl
	uint8_t* v83; // [esp+0h] [ebp-1Eh]
	uint8_t* v84; // [esp+4h] [ebp-1Ah]
	int v85; // [esp+8h] [ebp-16h]
	int v86; // [esp+Ch] [ebp-12h]
	int v87; // [esp+10h] [ebp-Eh]
	uint8_t* v88; // [esp+14h] [ebp-Ah]
	int v89; // [esp+20h] [ebp+2h]
	type_entity_0x6E8E* v90x; // [esp+24h] [ebp+6h]
	signed int v91; // [esp+28h] [ebp+Ah]
	signed int v92; // [esp+2Ch] [ebp+Eh]
	int v93; // [esp+30h] [ebp+12h]
	int v94; // [esp+38h] [ebp+1Ah]
	uint8_t* v95; // [esp+3Ch] [ebp+1Eh]
	//int v96; // [esp+40h] [ebp+22h]
	int v97; // [esp+44h] [ebp+26h]
	int v98; // [esp+48h] [ebp+2Ah]
	uint8_t* v99x=0; // [esp+4Ch] [ebp+2Eh]
	uint8_t* v100x=0; // [esp+50h] [ebp+32h]
	uint8_t* v101; // [esp+54h] [ebp+36h]
	type_entity_0x6E8E* v102x; // [esp+58h] [ebp+3Ah]
	uint16_t j; // [esp+5Ch] [ebp+3Eh]
	uint16_t v104; // [esp+60h] [ebp+42h]
	char v105; // [esp+68h] [ebp+4Ah]
	uint8_t v106; // [esp+6Ch] [ebp+4Eh]
	char v107; // [esp+70h] [ebp+52h]
	char v108; // [esp+74h] [ebp+56h]
	char v109; // [esp+78h] [ebp+5Ah]
	char v110; // [esp+7Ch] [ebp+5Eh]

	v8 = x;
	v9 = y;
	v10 = scaling;
	v11 = D41A0_0.terrain_2FECE.MapType;
	v93 = 0;
	if (v11 == MapType_t::Day)
	{
    v109 = (*xadataclrd0dat.colorPalette_var28)[0x000];
    v106 = (*xadataclrd0dat.colorPalette_var28)[0x000];
    v108 = 28;
    v107 = -24;
	}
	else if (v11 == MapType_t::Night)
	{
		v109 = (*xadataclrd0dat.colorPalette_var28)[4095];
		v106 = (*xadataclrd0dat.colorPalette_var28)[4095];
		v12 = -124;
		v107 = -24;
	  v108 = v12;
	}
	else // Cave
	{
		v109 = (*xadataclrd0dat.colorPalette_var28)[4095];
		v106 = (*xadataclrd0dat.colorPalette_var28)[4095];
		v107 = 28;
		v12 = (*xadataclrd0dat.colorPalette_var28)[240];
	  v108 = v12;
	}

	if (x_WORD_180660_VGA_type_resolution & 1)
	{
		v8 = x >> 1;
		v9 = y >> 1;
		v10 = 2 * scaling;
		width >>= 1;
		height >>= 1;
	}
	//v99 = v9 * x_DWORD_18062C_resolution_x + (int)pdwScreenBuffer + v8;
	v99x = &pdwScreenBuffer_351628[v9 * screenWidth_18062C + v8];
	v100x = &x_DWORD_E9C3C[v9 * screenWidth_18062C + v8];
	LODWORD(v13) = 0x10000;
	HIDWORD(v13) = 0x10000 >> 31;
	v14 = v13 / v10;
	v98 = width / 2;
	v15 = v14 * Maths::sin_DB750[yaw & 0x7FF];
	v16 = (x_DWORD)Maths::sin_DB750[0x200 + yaw & 0x7FF] * v14;
	v85 = height / 2;
	v86 = -v15 >> 16;
	GetFont_6FC50(FontType_D419D);
	v87 = v16 >> 16;
	v102x = Entities_EA3E4[D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
	v17 = v102x->dword_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[12];
	if (v17)
		v93 = Entities_EA3E4[v17]->word_0x2E_46;
	//v18 = v102x->dword_0xA4_164;
	if (v102x->dword_0xA4_164x->CastleEntityIndex_0x3A_58)
	{
		v19 = (int16_t)(v102x->position_0x4C_76.x - posX);
		v20 = (int16_t)(v102x->position_0x4C_76.y - posY);
		v21 = v98 + ((v19 * v87 - v20 * v86) >> 16);
		v22 = v87 * v20 + v86 * v19;
		v23x = Entities_EA3E4[v102x->dword_0xA4_164x->CastleEntityIndex_0x3A_58];
		v24 = (v22 >> 16) + v85;
		v25 = (int16_t)(v23x->position_0x4C_76.x - posX);
		v26 = (int16_t)(v23x->position_0x4C_76.y - posY);
		v27 = v98 + ((v25 * v87 - v26 * v86) >> 16);
		v28 = ((v86 * v25 + v87 * v26) >> 16) + v85;
		v29 = v98 + ((v25 * v87 - v26 * v86) >> 16) - v21;
		v91 = Maths::sub_7277A_radix_3d(v29 * v29 + (v28 - v24) * (v28 - v24));
		v89 = (uint16_t)Maths::sub_72633_maybe_tan(v27 - v21, v28 - v24);
		v30 = D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].Turn_2BE0_11248 & 3;
		while (1)
		{
			v30 += 4;
			if (v30 > v91)
				break;
			v31 = v21 + (v30 * Maths::sin_DB750[v89] >> 16);
			v94 = v24 + (-v30 * (signed int)Maths::sin_DB750[0x200 + v89] >> 16);
			if (v31 < 0 || v31 >= width || v94 < 0 || v94 >= height || v31 < x_WORD_F4960[1 + 2 * v94] || v31 >= x_WORD_F4960[2 * v94])
				break;
			v32x = &v99x[screenWidth_18062C * v94 + v31];
			v33x = &v100x[screenWidth_18062C * v94 + v31]; //(char*)(v100 + v31 + x_DWORD_18062C_resolution_x * v94);
			v34 = x_BYTE_F6EE0_tablesx[0x4000 + 256 * *v32x + (uint8_t)(*xadataclrd0dat.colorPalette_var28)[4095]];
			*v33x = v34;
			*v32x = v34;
		}
	}
	for (iy = &D41A0_0.struct_0x6E8E[1]; iy <= &D41A0_0.struct_0x6E8E[0x3e8 - 1]; iy++)
	{
		v36 = 0;
		v105 = 0;
		v92 = 1;
		v37 = iy->class_0x3F_63 - 2;
		v104 = 0;
		if (v37 <= 0xDu)
		{
			switch (v37)
			{
			case 0u:
				v48 = iy->model_0x40_64;
				if (v48 > 3u)
				{
				LABEL_74:
					v110 = v108;
					goto LABEL_118;
				}
				switch (v48)
				{
				case 0u:
					if (iy->actionIndex_0x45_69 != 2)
						goto LABEL_74;
					v110 = v108;
					v36 = 0;
					break;
				case 1u:
					v36 = 1;
					if (x_D41A0_BYTEARRAY_4_struct.colorIndex_121[2])
						v110 = (*xadataclrd0dat.colorPalette_var28)[0x88];
					else
						v105 = 1;
					v104 = 27;
					break;
				case 2u:
					v110 = v108;
					v104 = 22;
					goto LABEL_118;
				case 3u:
					goto LABEL_119;
				}
				break;
			case 1u:
				v55 = iy->model_0x40_64;
				if (v55 >= 2u)
				{
					if (v55 <= 2u)
					{
						LOBYTE(v56) = iy->id_0x1A_26 != v102x->id_0x1A_26;
						HIBYTE(v56) = 0;
						v104 = v56 + 24;
						v36 = Entities_EA3E4[iy->id_0x1A_26]->dword_0xA4_164x->playerColorIndex_0x38_56 + 58;
					}
					else if (v55 == 3)
					{
						if (iy->id_0x1A_26 == v102x->id_0x1A_26 || v93)
							v36 = Entities_EA3E4[iy->id_0x1A_26]->dword_0xA4_164x->playerColorIndex_0x38_56 + 66;
						if (iy->id_0x1A_26 == v102x->id_0x1A_26)
						{
							v104 = 23;
						}
						else if (v93)
						{
							v104 = 26;
						}
					}
				}
				break;
			case 2u:
			case 4u:
			case 5u:
			case 6u:
			case 0xBu:
				break;
			case 3u:
				v49 = iy->actionIndex_0x45_69;
				if (v49 >= 0xB4u && (v49 <= 0xB4u || v49 >= 0xE8u && v49 <= 0xEAu) || iy->struct_byte_0xc_12_15.byte[0] & 1)
					break;
				v50 = iy->id_0x1A_26;
				if (v50 != iy - D41A0_0.struct_0x6E8E)
				{
					v52 = playersColors_E88E0x[GetTrueWizardNumber_61790(Entities_EA3E4[v50]->dword_0xA4_164x->playerColorIndex_0x38_56)][1];
					goto LABEL_98;
				}
				v51 = iy->model_0x40_64;
				if (v51 >= 0xCu)
				{
					if (v51 <= 0xEu)
					{
						v52 = (*xadataclrd0dat.colorPalette_var28)[15];
					LABEL_98:
						v110 = v52;
						goto LABEL_118;
					}
					if (v51 == 22)
					{
						if (iy->playerEntityIndex_0x94_148 != v102x->id_0x1A_26)
							v104 = 18;
						v52 = v109;
						goto LABEL_98;
					}
				}
				v110 = v109;
				v53 = 1;
				v54 = iy->StageVar2_0x49_73;
				if ((v54 == 14 || v54 == 13) && iy->parentId_0x28_40 == v102x->id_0x1A_26)
					v53 = 0;
				if (v53)
					v104 = 19;
				goto LABEL_118;
			case 7u:
				goto LABEL_57;
			case 8u:
				v39 = iy->model_0x40_64;
				if (v39 < 0x27u)
				{
					if (v39 >= 0x12u)
					{
						if (v39 <= 0x12u)
							break;
						if (v39 == 34)
							v92 = 2;
					}
					goto LABEL_57;
				}
				if (v39 <= 0x27u)
					goto LABEL_169;
				if (v39 < 0x39u)
				{
					if (v39 == 45)
					{
						v45 = iy->playerEntityIndex_0x94_148;
						if (v45 && (v46x = Entities_EA3E4[v45], v46x->class_0x3F_63 == 3))
						{
							//v47 = x_D41A0_BYTEARRAY_4_struct.byteindex_121[3] == 0;
							GetTrueWizardNumber_61790(v46x->dword_0xA4_164x->playerColorIndex_0x38_56);
						}
						else
						{
							if (!(str_D93C0_bldgprmbuffer[iy->byte_0x46_70].byte_2 & 8))
								v104 = 21;
							v105 = 1;
						}
					}
				LABEL_57:
					v44x = Entities_EA3E4[iy->id_0x1A_26];
					if (v44x->class_0x3F_63 == 3)
						v43 = playersColors_E88E0x[GetTrueWizardNumber_61790(v44x->dword_0xA4_164x->playerColorIndex_0x38_56)][0];
					else
						v43 = (*xadataclrd0dat.colorPalette_var28)[0xf0f];
				LABEL_117:
					v110 = v43;
					goto LABEL_118;
				}
				if (v39 <= 0x39u)
				{
				LABEL_169:
					v40 = iy->playerEntityIndex_0x94_148;
					if (v40 && (v41x = Entities_EA3E4[v40], v41x->class_0x3F_63 == 3))
					{
						if (x_D41A0_BYTEARRAY_4_struct.colorIndex_121[3])
							v42 = playersColors_E88E0x[GetTrueWizardNumber_61790(v41x->dword_0xA4_164x->playerColorIndex_0x38_56)][0];
						else
							v42 = playersColors_E88E0x[GetTrueWizardNumber_61790(v41x->dword_0xA4_164x->playerColorIndex_0x38_56)][1];
						v110 = v42;
					}
					else
					{
						v110 = v107;
						v104 = 18;
					}
				LABEL_118:
					v36 = 1;
				}
				else
				{
					if (v39 < 0x4Eu)
						goto LABEL_57;
					if (v39 <= 0x4Eu)
					{
						if (iy->word_0x32_50 != D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240)
							break;
						v43 = playersColors_E88E0x[GetTrueWizardNumber_61790(v102x->dword_0xA4_164x->playerColorIndex_0x38_56)][x_D41A0_BYTEARRAY_4_struct.colorIndex_121[3]];
						goto LABEL_117;
					}
					if (v39 < 0x56u || v39 > 0x57u)
						goto LABEL_57;
				}
				break;
			case 9u:
				v38 = iy->model_0x40_64;
				if (v38 <= 0x2Cu)
				{
					switch (v38)
					{
					case 0u:
					case 1u:
					case 2u:
					case 3u:
					case 4u:
					case 5u:
					case 6u:
					case 7u:
					case 8u:
					case 9u:
					case 0xAu:
					case 0xBu:
					case 0xDu:
					case 0xEu:
					case 0xFu:
					case 0x10u:
					case 0x11u:
					case 0x12u:
					case 0x13u:
					case 0x14u:
					case 0x15u:
					case 0x16u:
					case 0x17u:
					case 0x18u:
					case 0x19u:
					case 0x1Au:
					case 0x1Bu:
					case 0x1Cu:
					case 0x1Du:
					case 0x1Eu:
					case 0x20u:
					case 0x21u:
					case 0x22u:
					case 0x23u:
					case 0x24u:
					case 0x25u:
					case 0x26u:
					case 0x27u:
					case 0x28u:
					case 0x29u:
					case 0x2Au:
					case 0x2Bu:
					case 0x2Cu:
						goto LABEL_119;
					case 0xCu:
						v36 = 83;
						v104 = 83;
						break;
					case 0x1Fu:
						v36 = 84;
						v104 = 83;
						break;
					}
				}
				break;
			case 0xAu:
			case 0xDu:
				if (iy->struct_byte_0xc_12_15.byte[0] & 1)
					break;
				v43 = (*xadataclrd0dat.colorPalette_var28)[3840];
				v104 = 20;
				goto LABEL_117;
			case 0xCu:
				if (iy->model_0x40_64 != 5)
					break;
				if (x_D41A0_BYTEARRAY_4_struct.colorIndex_121[3])
					v43 = (*xadataclrd0dat.colorPalette_var28)[3840];
				else
					v43 = (*xadataclrd0dat.colorPalette_var28)[4095];
				goto LABEL_117;
			}
		}
	LABEL_119:
		if (v36)
		{
			v57 = (int16_t)(iy->position_0x4C_76.x - posX);
			v58 = (int16_t)(iy->position_0x4C_76.y - posY);
			v59 = v98 + ((v57 * v87 - v58 * v86) >> 16);
			if (v59 >= 0 && v59 < width)
			{
				v60 = ((v87 * v58 + v86 * v57) >> 16) + v85;
				if (v60 >= 0 && v60 < height && v59 >= x_WORD_F4960[1 + 2 * v60] && v59 < x_WORD_F4960[2 * v60])
				{
					/*v61 = (char*)(v60 * x_DWORD_18062C_resolution_x + v59 + v99);
					v62 = (char*)(v60 * x_DWORD_18062C_resolution_x + v59 + v100);*/
					v61x = &v99x[v60 * screenWidth_18062C + v59];
					v62x = &v100x[v60 * screenWidth_18062C + v59];
					if (!v105)
					{
						if (v36 <= 1u)
						{
							*v62x = v110;
							*v61x = v110;
							if (v92 > 1)
							{
								v62x[1] = v110;
								v61x[1] = v110;
								v63 = &v62x[screenWidth_18062C];
								*v63 = v110;
								v64 = &v61x[screenWidth_18062C];
								*v64 = v110;
								v63[1] = v110;
								v64[1] = v110;
							}
						}
						else if (x_WORD_180660_VGA_type_resolution == 1)
						{
							if (v36 < 0x42u || v36 >= 0x4Au)
							{
								if (v36 < 0x3Au || v36 >= 0x42u)
								{
									/*if ( v36 == 83 || v36 == 84 )
									  DrawBitmap_2BB40(
										2 * v59 - *(uint8_t *)(6 * v36 + **filearray_2aa18c[6] + 4) / 2,
										2 * v60 - *(uint8_t *)(6 * v36 + **filearray_2aa18c[6] + 5) / 2,
										  (uint8_t**)(6 * v36 + **filearray_2aa18c[6]));*/
									if (v36 == 83 || v36 == 84)
										DrawBitmap_2BB40(
											2 * v59 - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v36].width_4 / 2,
											2 * v60 - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v36].height_5 / 2,
											(*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v36]);
								}
								else
								{
									//DrawBitmap_2BB40(2 * v59, 2 * v60 - *(uint8_t *)(**filearray_2aa18c[6] + 6 * v36 + 5), (uint8_t**)(**filearray_2aa18c[6] + 6 * v36));
									DrawBitmap_2BB40(2 * v59, 2 * v60 - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v36].height_5, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v36]);
								}
							}
							else
							{
								/*DrawBitmap_2BB40(
								  2 * v59 - *(uint8_t *)(6 * v36 + **filearray_2aa18c[6] + 4) / 2,
								  2 * v60 - *(uint8_t *)(6 * v36 + **filearray_2aa18c[6] + 5),
									(uint8_t**)(6 * v36 + **filearray_2aa18c[6]));*/
								DrawBitmap_2BB40(
									2 * v59 - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v36].width_4 / 2,
									2 * v60 - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v36].height_5,
									(*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v36]);
							}
						}
						else if (v36 >= 0x3Cu)
						{
							/*DrawBitmap_2BB40(
							  v59 - *(uint8_t *)(6 * v36 + **filearray_2aa18c[6] + 4) / 2,
							  v60 - *(uint8_t *)(6 * v36 + **filearray_2aa18c[6] + 5),
								(uint8_t**)(6 * v36 + **filearray_2aa18c[6]));*/
							DrawBitmap_2BB40(
								v59 - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v36].width_4 / 2,
								v60 - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v36].height_5,
								(*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v36]);
						}
						else
						{
							//DrawBitmap_2BB40(v59, v60 - *(uint8_t *)(6 * v36 + **filearray_2aa18c[6] + 5), (uint8_t**)(6 * v36 + **filearray_2aa18c[6]));
							DrawBitmap_2BB40(v59, v60 - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v36].height_5, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v36]);
						}
					}
					if (v104)
						sub_885E0(iy, (int16_t)v59, (int16_t)v60, v104);
				}
			}
		}
	}
	if (v93)
	{
		v90x = Entities_EA3E4[D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
		for (j = 0; j < D41A0_0.NumberOfPlayers_0xe; j++)
		{
			if (j != D41A0_0.LevelIndex_0xc)
			{
				//v96 = 2124 * j;
				v65 = D41A0_0.array_0x2BDE[j].playerIndex_0x00a_2BE4_11240;
				v66x = Entities_EA3E4[v65];
				if (v66x->life_0x8 >= 0)
				{
					if (sub_63570(v90x, Entities_EA3E4[v65]))
					{
						v67 = (int16_t)(v66x->position_0x4C_76.x - posX);
						v68 = (int16_t)(v66x->position_0x4C_76.y - posY);
						v69 = v98 + ((v67 * v87 - v68 * v86) >> 16);
						if (v69 >= 0 && v69 < width)
						{
							v70 = ((v86 * v67 + v87 * v68) >> 16) + v85;
							if (v70 >= 0 && v70 < height && v69 >= x_WORD_F4960[1 + 2 * v70] && v69 < x_WORD_F4960[2 * v70])
							{
								v71 = &v99x[v70 * screenWidth_18062C + v69];
								v72 = &v100x[v70 * screenWidth_18062C + v69];
								v73 = playersColors_E88E0x[GetTrueWizardNumber_61790(v66x->dword_0xA4_164x->playerColorIndex_0x38_56)][1];
								*v72 = v73;
								*v71 = v73;
								if (x_WORD_180660_VGA_type_resolution == 1)
									DrawText_2BC10(D41A0_0.array_0x2BDE[j].WizardName_0x39f_2BFA_12157, 2 * v69 + 4, 2 * v70, v73);
								else
									DrawText_2BC10(D41A0_0.array_0x2BDE[j].WizardName_0x39f_2BFA_12157, v69 + 4, v70, v73);
							}
						}
					}
				}
			}
		}
	}
	v97 = width / 12;
	v74 = screenWidth_18062C * (height / 2);
	v75 = &v100x[v74 + width / 2 - 1];
	v76 = &v99x[v74 + width / 2 - 1];
	index = (char*)&x_BYTE_F6EE0_tablesx[0x4000 + 256 * v106];
	v88 = v76;
	v101 = v76;
	*v76 = index[(uint8_t)*v76];
	v78 = v76;
	v83 = v75;
	v84 = v75;
	v95 = v75;
	for (*v75 = index[(uint8_t)*v76]; v97; --v97)
	{
		v101++;
		v88 += screenWidth_18062C;
		v78 -= screenWidth_18062C;
		v95 -= screenWidth_18062C;
		v84++;
		v83 += screenWidth_18062C;
		v79 = x_BYTE_F6EE0_tablesx[0x4000 + 256 * v106 + (uint8_t)*v78];
		*v78 = v79;
		*v95 = v79;
		v80 = x_BYTE_F6EE0_tablesx[0x4000 + 256 * v106 + (uint8_t)*v101];
		*v101 = v80;
		*v84 = v80;
		v81 = x_BYTE_F6EE0_tablesx[0x4000 + 256 * v106 + (uint8_t)*v88];
		*v88 = v81;
		*v83 = v81;
		v82 = x_BYTE_F6EE0_tablesx[0x4000 + 256 * v106 + (uint8_t)*(v76 - 1)];
		*(v76 - 1) = v82;
		*(v75 - 1) = v82;
		v76--;
		v75--;
	}
	//return result;
}

//----- (00063570) --------------------------------------------------------
char sub_63570(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//244570
{
	type_entity_0x6E8E* v2x; // ecx
	char v3; // ah
	type_entity_0x6E8E* v4x; // ebx
	char v5; // dl
	uint8_t v6; // al

	v2x = Entities_EA3E4[a1x->dword_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[12]];
	v3 = 1;
	v4x = Entities_EA3E4[a2x->dword_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[4]];
	v5 = 0;
	if (v4x > Entities_EA3E4[0] && v4x->word_0x2E_46)
		v5 = 1;
	if (v2x > Entities_EA3E4[0])
	{
		v6 = v2x->byte_0x46_70;
		if (v6 < 1u)
		{
			if (v6)
				return v3;
			if (a2x->dword_0xA4_164x->byte_0x1BF_447)
				return 0;
		}
		else if (v6 > 1u)
		{
			return v3;
		}
		if (v5)
			return 0;
	}
	return v3;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00063600) --------------------------------------------------------
void DrawMinimap_63600(int16_t x, int16_t y, int16_t posX, int16_t posY, uint16_t width, uint16_t height, int16_t yaw, int16_t scaling, int a10)//244600
{
	if (D41A0_0.m_GameSettings.m_Display.m_uiScreenSize == 1)
		sub_63670_draw_minimap_a(x, y, posX, posY, width, height, yaw, scaling, a10);
	else
		sub_63C90_draw_minimap_b(x, y, posX, posY, width, height, yaw, scaling, a10);
}

//----- (00063670) --------------------------------------------------------
void sub_63670_draw_minimap_a(int16_t x, int16_t y, int16_t posX, int16_t posY, uint16_t width, uint16_t height, int16_t yaw, int16_t scaling, int a10)//244670
{
	int v10; // eax
	int v11; // esi
	int v12; // edi
	int v13; // ecx
	bool v14; // dl
	int16_t* v15; // ebx
	int v16; // edx
	int16_t* v17; // esi
	int v18; // ebx
	int16_t v19; // ax
	int16_t* v20; // edx
	int16_t v21; // ax
	int8_t* v22x; // edx
	int v23; // ebx
	int v24; // esi
	int v25; // eax
	int v26; // eax
	//int indexx; // eax
	int v28; // ebx
	int v29; // ecx
	char* v30; // edi
	int v31; // esi
	int v32; // eax
	int v33; // edx
	char v34; // al
	int v35; // ecx
	int v36; // ebx
	x_BYTE* v37; // edi
	int v38; // ecx
	int v39; // esi
	int v40; // eax
	int v41; // edx
	int v42; // ebx
	char* v43; // edi
	int v44; // ecx
	int v45; // esi
	int v46; // eax
	int v47; // edx
	char v48; // al
	int v49; // ecx
	int v50; // ebx
	x_BYTE* v51; // edi
	int v52; // ecx
	int v53; // esi
	int v54; // eax
	int v55; // edx
	int v56; // ebx
	char* v57; // edi
	int v58; // ecx
	int v59; // esi
	int v60; // eax
	int v61; // edx
	char v62; // al
	char* v63; // edi
	uint8_t* v64x; // [esp+0h] [ebp-60h]
	int v65; // [esp+4h] [ebp-5Ch]
	int v66; // [esp+8h] [ebp-58h]
	int v67; // [esp+Ch] [ebp-54h]
	//int v68; // [esp+10h] [ebp-50h]
	int v69; // [esp+14h] [ebp-4Ch]
	uint8_t* v70x; // [esp+18h] [ebp-48h]
	uint8_t* v71x; // [esp+1Ch] [ebp-44h]
	int l; // [esp+20h] [ebp-40h]
	int v73; // [esp+24h] [ebp-3Ch]
	int i; // [esp+28h] [ebp-38h]
	int k; // [esp+2Ch] [ebp-34h]
	int m; // [esp+30h] [ebp-30h]
	int j; // [esp+34h] [ebp-2Ch]
	int v78; // [esp+38h] [ebp-28h]
	int16_t* v79; // [esp+3Ch] [ebp-24h]
	int v80; // [esp+40h] [ebp-20h]
	//uint8_t* v81z; // [esp+44h] [ebp-1Ch]
	int v81y; // [esp+44h] [ebp-1Ch]
	uint8_t* v82x; // [esp+48h] [ebp-18h]
	int v83; // [esp+4Ch] [ebp-14h]
	uint8_t* v84x; // [esp+50h] [ebp-10h]
	int v85; // [esp+54h] [ebp-Ch]
	int16_t* v86; // [esp+58h] [ebp-8h]
	bool v87; // [esp+5Ch] [ebp-4h]

	v10 = x;
	v11 = y;
	v12 = width;
	v13 = height;
	v14 = !D41A0_0.m_GameSettings.str_0x2196.transparency_0x2198 && D41A0_0.terrain_2FECE.MapType != MapType_t::Cave;
	v87 = v14;
	if (x_WORD_180660_VGA_type_resolution & 1)
	{
		v10 = x >> 1;
		v11 = y >> 1;
		v12 = width >> 1;
		v13 = height >> 1;
		scaling *= 2;
	}
	v70x = &pdwScreenBuffer_351628[v11 * screenWidth_18062C + v10];
	v71x = &x_DWORD_E9C3C[v11 * screenWidth_18062C + v10];
	if (a10)
	{
		v15 = x_WORD_F4960;
		v16 = v13;
		while (v16)
		{
			v15 += 2;
			*(v15 - 1) = 0;
			v16--;
			*(v15 - 2) = v12;
		}
	}
	else
	{
		v78 = v13 / 2;
		v17 = &x_WORD_F4960[2 * (v13 / 2)];
		v79 = v17 - 2;
		v73 = 0;
		//v68 = v13 >> 1;
		v18 = v12 >> 1;
		v69 = 0x1000000 / (v13 >> 1);
		while (v78)
		{
			v65 = v18 * (uint8_t)x_BYTE_F6EE0_tablesx[(0x14300 + v73) >> 16] >> 8;
			v19 = v18 + v65;
			v20 = v79;
			*v17 = v18 + v65;
			*v20 = v19;
			v21 = v18 - v65;
			v22x = (int8_t*)v79;
			v17[1] = v18 - v65;
			*(x_WORD*)(v22x + 2) = v21;
			v79 = (int16_t*)(v22x - 4);
			v17 += 2;
			v73 += v69;
			v78--;
		}
	}
	v23 = yaw & 0x7FF;
	v24 = v13 * (scaling * Maths::sin_DB750[v23] >> 16);
	v25 = scaling * (signed int)Maths::sin_DB750[0x200 + v23] >> 16;
	v80 = scaling * Maths::sin_DB750[v23] >> 16;
	v81y = v25;
	v67 = v24 / v12;
	v26 = v13 * v25;
	v66 = v26 / v12;
	v85 = posX - (v12 * (v26 / v12) - v24) / 2;
	v86 = x_WORD_F4960;
	v83 = posY - (v26 + v12 * (v24 / v12)) / 2;
	v84x = v70x;
	v82x = v71x;
	if (isCaveLevel_D41B6)
	{
		if (a10)
		{
			for (i = v13; i; i--)
			{
				v33 = v86[1];
				v28 = v85 + v33 * v66;
				v29 = v83 + v33 * v67;
				v30 = (char*)(v33 + v84x);
				v64x = v33 + v82x;
				v31 = *v86 - v86[1];
				v32 = 0;
				HIWORD(v33) = 0;
				do
				{
					LOBYTE(v33) = BYTE1(v28);
					BYTE1(v33) = BYTE1(v29);
					v34 = 0;
					if (!(mapAngle_13B4E0[v33] & 8))
					{
						LOBYTE(v32) = mapTerrainType_10B4E0[v33];
						BYTE1(v33) = mapShading_12B4E0[v33];
						LOBYTE(v33) = x_BYTE_F6EE0_tablesx[0x14000 + v32];
						v34 = x_BYTE_F6EE0_tablesx[v33];
					}
					*v30++ = v34;
					LOWORD(v28) = v66 + v28;
					LOWORD(v29) = v67 + v29;
					v31--;
				} while (v31);
				v84x += screenWidth_18062C;
				v82x += screenWidth_18062C;
				v86 += 2;
				v85 -= v80;
				//indexx = i - 1;
				v83 += v81y;
			}
		}
		else if (v87)
		{
			for (j = v13; j; j--)
			{
				v35 = v86[1];
				v36 = v85 + v35 * v66;
				v37 = (x_BYTE*)(v35 + v84x);
				v64x = v35 + v82x;
				v38 = v83 + v35 * v67;
				v39 = *v86 - v86[1];
				v40 = 0;
				HIWORD(v41) = 0;
				do
				{
					LOBYTE(v41) = BYTE1(v36);
					BYTE1(v41) = BYTE1(v38);
					if (!(mapAngle_13B4E0[v41] & 8))
					{
						LOBYTE(v40) = mapTerrainType_10B4E0[v41];
						BYTE1(v41) = mapShading_12B4E0[v41];
						LOBYTE(v41) = x_BYTE_F6EE0_tablesx[0x14000 + v40];
						LOBYTE(v41) = x_BYTE_F6EE0_tablesx[v41];
						BYTE1(v41) = *v37;
						*v37 = x_BYTE_F6EE0_tablesx[0x4000 + v41];
					}
					v37++;
					LOWORD(v36) = v66 + v36;
					LOWORD(v38) = v67 + v38;
					v39--;
				} while (v39);
				v84x += screenWidth_18062C;
				v82x += screenWidth_18062C;
				v86 += 2;
				v85 -= v80;
				//indexx = j - 1;
				v83 += v81y;
			}
		}
		else
		{
			for (k = v13; k; v83 += v81y)
			{
				v47 = v86[1];
				v42 = v85 + v47 * v66;
				v43 = (char*)(v47 + v84x);
				v44 = v83 + v47 * v67;
				v64x = v47 + v82x;
				v45 = *v86 - v86[1];
				v46 = 0;
				HIWORD(v47) = 0;
				do
				{
					LOBYTE(v47) = BYTE1(v42);
					BYTE1(v47) = BYTE1(v44);
					if (mapAngle_13B4E0[v47] & 8)
					{
						v48 = 0;
					}
					else
					{
						LOBYTE(v46) = mapTerrainType_10B4E0[v47];
						BYTE1(v47) = mapShading_12B4E0[v47];
						LOBYTE(v47) = x_BYTE_F6EE0_tablesx[0x14000 + v46];
						v48 = x_BYTE_F6EE0_tablesx[v47];
					}
					*v43++ = v48;
					LOWORD(v42) = v66 + v42;
					LOWORD(v44) = v67 + v44;
					v45--;
				} while (v45);
				v84x += screenWidth_18062C;
				v82x += screenWidth_18062C;
				v86 += 2;
				//indexx = v81y;
				v85 -= v80;
				k--;
			}
		}
	}
	else if (a10)
	{
		for (l = v13; l; l--)
		{
			v49 = v86[1];
			v50 = v85 + v49 * v66;
			v51 = (x_BYTE*)(v49 + v84x);
			v64x = v49 + v82x;
			v52 = v83 + v49 * v67;
			v53 = *v86 - v86[1];
			v54 = 0;
			HIWORD(v55) = 0;
			do
			{
				LOBYTE(v55) = BYTE1(v50);
				BYTE1(v55) = BYTE1(v52);
				LOBYTE(v54) = mapTerrainType_10B4E0[v55];
				BYTE1(v55) = mapShading_12B4E0[v55];
				LOBYTE(v55) = x_BYTE_F6EE0_tablesx[0x14000 + v54];
				*v51++ = x_BYTE_F6EE0_tablesx[v55];
				v50 += v66;
				v52 += v67;
				--v53;
			} while (v53);
			v84x += screenWidth_18062C;
			v82x += screenWidth_18062C;
			v86 += 2;
			v85 -= v80;
			//indexx = l - 1;
			v83 += v81y;
		}
	}
	else
	{
		for (m = v13; m; v83 += v81y)
		{
			v61 = v86[1];
			v56 = v85 + v61 * v66;
			v57 = (char*)(v61 + v84x);
			v58 = v83 + v61 * v67;
			v64x = v61 + v82x;
			v59 = *v86 - v86[1];
			v60 = 0;
			HIWORD(v61) = 0;
			do
			{
				LOBYTE(v61) = BYTE1(v56);
				BYTE1(v61) = BYTE1(v58);
				LOBYTE(v60) = mapTerrainType_10B4E0[v61];
				BYTE1(v61) = mapShading_12B4E0[v61];
				LOBYTE(v61) = x_BYTE_F6EE0_tablesx[0x14000 + v60];
				LOBYTE(v61) = x_BYTE_F6EE0_tablesx[v61];
				BYTE1(v61) = *v57;
				v62 = x_BYTE_F6EE0_tablesx[0x4000 + v61];
				*v57 = v62;
				v63 = x_InterlockedExchange((char**)&v64x, (v57 + 1));
				*v63 = v62;
				v57 = x_InterlockedExchange((char**)&v64x, (v63 + 1));
				v56 += v66;
				v58 += v67;
				v59--;
			} while (v59);
			v84x += screenWidth_18062C;
			v82x += screenWidth_18062C;
			v86 += 2;
			//indexx = v81y;
			v85 -= v80;
			m--;
		}
	}
}

//----- (00063C90) --------------------------------------------------------
void sub_63C90_draw_minimap_b(int16_t x, int16_t y, int16_t posX, int16_t posY, uint16_t width, uint16_t height, int16_t yaw, int16_t scaling, int a10)//244c90
{
	//bool v14; // al
	int16_t* v15; // ebx
	int v16; // edx
	int16_t* v17; // esi
	int v18; // ebx
	int v19; // ST00_4
	int v20; // ebx
	int v21; // esi
	int v22; // eax
	int v23; // eax
	int v24; // edx
	//int index; // eax
	int v26; // ebx
	int v27; // ecx
	char* v28; // edi
	int v29; // esi
	int v30; // eax
	int v31; // edx
	char v32; // al
	int v33; // ebx
	int v34; // ecx
	x_BYTE* v35; // edi
	int v36; // esi
	int v37; // eax
	int v38; // edx
	int v39; // ebx
	int v40; // ecx
	char* v41; // edi
	int v42; // esi
	int v43; // eax
	int v44; // edx
	char v45; // al
	int v46; // ebx
	int v47; // ecx
	x_BYTE* v48; // edi
	int v49; // esi
	int v50; // eax
	int v51; // edx
	int v52; // ebx
	int v53; // ecx
	uint8_t* v54; // edi
	int v55; // esi
	int v56; // eax
	int v57; // edx
	int v58; // [esp+4h] [ebp-50h]
	int v59; // [esp+8h] [ebp-4Ch]
	uint8_t* ptrScreenBuffer; // [esp+14h] [ebp-40h]
	int m; // [esp+18h] [ebp-3Ch]
	int l; // [esp+1Ch] [ebp-38h]
	int k; // [esp+20h] [ebp-34h]
	int v64; // [esp+24h] [ebp-30h]
	int i; // [esp+28h] [ebp-2Ch]
	int j; // [esp+2Ch] [ebp-28h]
	int v67; // [esp+30h] [ebp-24h]
	int16_t* v68; // [esp+34h] [ebp-20h]
	int v69; // [esp+38h] [ebp-1Ch]
	int v70; // [esp+3Ch] [ebp-18h]
	int v72; // [esp+44h] [ebp-10h]
	int v73; // [esp+48h] [ebp-Ch]
	int16_t* v74; // [esp+4Ch] [ebp-8h]
	//bool v75; // [esp+50h] [ebp-4h]

	//v14 = !x_D41A0_BYTEARRAY_0[8600] && x_D41A0_BYTEARRAY_0[196308] != 2;
	//v75 = !x_D41A0_BYTEARRAY_0[8600] && x_D41A0_BYTEARRAY_0[196308] != 2;
	if (x_WORD_180660_VGA_type_resolution & 1)
	{
		x = x >> 1;
		y = y >> 1;
		width = width >> 1;
		height = height >> 1;
		scaling *= 2;
	}

	ptrScreenBuffer = &pdwScreenBuffer_351628[screenWidth_18062C * y + x];
	if (a10)
	{
		v15 = x_WORD_F4960;
		v16 = height;
		while (v16)
		{
			v15 += 2;
			*(v15 - 1) = 0;
			v16--;
			*(v15 - 2) = width;
		}
	}
	else
	{
		v67 = height / 2;//adress 244d1b
		v17 = &x_WORD_F4960[2 * (height / 2)];
		v68 = v17 - 2;
		v64 = 0;
		v18 = width >> 1;
		while (v67)
		{
			v19 = v18 * x_BYTE_F6EE0_tablesx[0x14300 + (v64 >> 16)] >> 8;
			v17[0] = v18 + v19;
			v68[0] = v18 + v19;
			v17[1] = v18 - v19;
			v68[1] = v18 - v19;
			v68 -= 2;
			v17 += 2;
			v64 += 0x1000000 / (height >> 1);
			v67--;
		}
	}
	v20 = yaw & 0x7FF;
	v21 = height * (scaling * Maths::sin_DB750[v20] >> 16);
	v22 = scaling * (signed int)Maths::sin_DB750[0x200 + v20] >> 16;
	v70 = scaling * Maths::sin_DB750[v20] >> 16;
	v69 = v22;
	v59 = v21 / width;
	v23 = height * v22;
	v58 = v23 / width;
	v72 = posX - (width * (v23 / width) - v21) / 2;
	v74 = x_WORD_F4960;
	v24 = posY - (v23 + width * (v21 / width)) / 2;
	v73 = v24;
	if (isCaveLevel_D41B6)//adress 244e53
	{
		if (a10)
		{
			for (i = height; i; v73 += v69)
			{
				v31 = v74[1];
				v26 = v72 + v31 * v58;
				v27 = v73 + v31 * v59;
				v28 = (char*)(v31 + ptrScreenBuffer);
				v29 = *v74 - v31;
				v30 = 0;
				HIWORD(v31) = 0;
				do
				{
					LOBYTE(v31) = HIBYTE(v26);
					HIBYTE(v31) = HIBYTE(v27);
					v32 = 0;
					if (!(mapAngle_13B4E0[v31] & 8))
					{
						LOBYTE(v30) = mapTerrainType_10B4E0[v31];
						HIBYTE(v31) = mapShading_12B4E0[v31];
						LOBYTE(v31) = x_BYTE_F6EE0_tablesx[0x14000 + v30];
						v32 = x_BYTE_F6EE0_tablesx[v31];
					}
					*v28++ = v32;
					LOWORD(v26) = v58 + v26;
					LOWORD(v27) = v59 + v27;
					v29--;
				} while (v29);
				ptrScreenBuffer += screenWidth_18062C;
				v74 += 2;
				//index = v69;
				i--;
				v72 -= v70;
			}
		}
		else if (!D41A0_0.m_GameSettings.str_0x2196.transparency_0x2198 && D41A0_0.terrain_2FECE.MapType != MapType_t::Cave)
		{
			for (j = height; j; v73 += v69)
			{
				v38 = v74[1];
				v33 = v72 + v38 * v58;
				v34 = v73 + v38 * v59;
				v35 = (x_BYTE*)(v38 + ptrScreenBuffer);
				v36 = *v74 - v38;
				v37 = 0;
				HIWORD(v38) = 0;
				do
				{
					LOBYTE(v38) = BYTE1(v33);
					BYTE1(v38) = BYTE1(v34);
					if (!(mapAngle_13B4E0[v38] & 8))
					{
						LOBYTE(v37) = mapTerrainType_10B4E0[v38];
						BYTE1(v38) = mapShading_12B4E0[v38];
						LOBYTE(v38) = x_BYTE_F6EE0_tablesx[0x14000 + v37];
						LOBYTE(v38) = x_BYTE_F6EE0_tablesx[v38];
						BYTE1(v38) = *v35;
						*v35 = x_BYTE_F6EE0_tablesx[0x4000 + v38];
					}
					v35++;
					LOWORD(v33) = v58 + v33;
					LOWORD(v34) = v59 + v34;
					v36--;
				} while (v36);
				ptrScreenBuffer += screenWidth_18062C;
				v74 += 2;
				//index = v69;
				j--;
				v72 -= v70;
			}
		}
		else
		{
			for (k = height; k; v73 += v69)
			{
				v44 = v74[1];
				v39 = v72 + v44 * v58;
				v40 = v73 + v44 * v59;
				v41 = (char*)(v44 + ptrScreenBuffer);
				v42 = *v74 - v44;
				v43 = 0;
				HIWORD(v44) = 0;
				do
				{
					LOBYTE(v44) = HIBYTE(v39);
					HIBYTE(v44) = HIBYTE(v40);
					if (mapAngle_13B4E0[v44] & 8)
					{
						v45 = 0;
					}
					else
					{
						LOBYTE(v43) = mapTerrainType_10B4E0[v44];
						HIBYTE(v44) = mapShading_12B4E0[v44];
						LOBYTE(v44) = x_BYTE_F6EE0_tablesx[0x14000 + v43];
						v45 = x_BYTE_F6EE0_tablesx[v44];
					}
					*v41++ = v45;
					LOWORD(v39) = v58 + v39;
					LOWORD(v40) = v59 + v40;
					v42--;
				} while (v42);
				ptrScreenBuffer += screenWidth_18062C;
				v74 += 2;
				//index = v69;
				k--;
				v72 -= v70;
			}
		}
	}
	else if (a10)
	{
		for (l = height; l; v73 += v69)
		{
			v51 = v74[1];
			v46 = v72 + v51 * v58;
			v47 = v73 + v51 * v59;
			v48 = (x_BYTE*)(v51 + ptrScreenBuffer);
			v49 = *v74 - v51;
			v50 = 0;
			HIWORD(v51) = 0;
			do
			{
				LOBYTE(v51) = HIBYTE(v46);
				HIBYTE(v51) = HIBYTE(v47);
				LOBYTE(v50) = mapTerrainType_10B4E0[v51];
				HIBYTE(v51) = mapShading_12B4E0[v51];
				LOBYTE(v51) = x_BYTE_F6EE0_tablesx[0x14000 + v50];
				*v48++ = x_BYTE_F6EE0_tablesx[v51];
				v46 += v58;
				v47 += v59;
				v49--;
			} while (v49);
			ptrScreenBuffer += screenWidth_18062C;
			v74 += 2;
			//index = v69;
			l--;
			v72 -= v70;
		}
	}
	else
	{//adress 245156
		for (m = height; m; v73 += v69)
		{//0x2580=v71-pdwScreenBuffer_351628
			v57 = v74[1];//adress 245185 mem edx 3ac764
			v52 = v72 + v57 * v58;
			v53 = v73 + v57 * v59;
			v54 = &ptrScreenBuffer[v57];
			v55 = v74[0] - v57;
			v56 = 0;
			HIWORD(v57) = 0;
			do
			{
				LOBYTE(v57) = HIBYTE(v52);
				HIBYTE(v57) = HIBYTE(v53);
				LOBYTE(v56) = mapTerrainType_10B4E0[v57];
				HIBYTE(v57) = mapShading_12B4E0[v57];
				LOBYTE(v57) = x_BYTE_F6EE0_tablesx[0x14000 + v56];
				LOBYTE(v57) = x_BYTE_F6EE0_tablesx[v57];
				HIBYTE(v57) = v54[0];
				*v54++ = x_BYTE_F6EE0_tablesx[0x4000 + v57];//here adress 2451b8
				v52 += v58;
				v53 += v59;
				v55--;
			} while (v55);
			ptrScreenBuffer += screenWidth_18062C;
			v74 += 2;
			//index = v69;
			m--;
			v72 -= v70;
		}
	}
}

int debugcounter_644F0 = 0;


//----- (000644F0) --------------------------------------------------------
void DrawMinimapMarks_644F0(int16_t x, int16_t y, int16_t posX, int16_t posY, uint16_t width, uint16_t height, int16_t yaw, int16_t scaling, uint8_t scale)//2454f0
{
	signed int v8; // ebx
	//__int64 v9; // rtt
	int v9x;
	int v10; // edi
	int v11; // edx
	int v12; // eax
	int16_t v13; // ST10_2
	char v14; // dl
	type_entity_0x6E8E* ix; // eax
	axis_3d* v16x; // eax
	axis_3d* v17x; // esi
	int v18; // edx
	signed int v19; // eax
	signed int v20; // eax
	int v21; // ebx
	int v22; // ecx
	int v23; // ebx
	int v24; // ecx
	int v25; // esi
	signed int v26; // edx
	int v27; // ecx
	uint32_t v28; // ebx
	int v29; // eax
	uint8_t colour_v30; // cl
	signed int v31; // ebx
	int v32; // eax
	int v33; // edi
	//uint8_t v34; // bl

	/*
	uint8_t origbyte20 = 0;
	uint8_t remakebyte20 = 0;
	int debugcounter11 = 0;
	int comp20;

	//if (debugcounter_644F0 >= 0x1c0)
	{
		comp20 = compare_with_sequence((char*)"002454F0-002DC4E0", (uint8_t*)x_BYTE_10B4E0_terraintype, 0x2dc4e0, debugcounter_644F0, 0x70000, 0x10000, &origbyte20, &remakebyte20);
		comp20 = compare_with_sequence((char*)"002454F0-002DC4E0", (uint8_t*)x_BYTE_11B4E0_height, 0x2dc4e0, debugcounter_644F0, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x10000);
		comp20 = compare_with_sequence((char*)"002454F0-002DC4E0", (uint8_t*)x_BYTE_12B4E0_shading, 0x2dc4e0, debugcounter_644F0, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x20000);
		comp20 = compare_with_sequence((char*)"002454F0-002DC4E0", (uint8_t*)x_BYTE_13B4E0_angle, 0x2dc4e0, debugcounter_644F0, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x30000);
		//comp20 = compare_with_sequence((char*)"00228320", (uint8_t*)x_BYTE_14B4E0, 0x2dc4e0, debugcounter11, 0x70000,0x10000, &origbyte20, &remakebyte20, 0x40000);
		comp20 = compare_with_sequence((char*)"002454F0-002DC4E0", (uint8_t*)x_WORD_15B4E0_source, 0x2dc4e0, debugcounter_644F0, 0x70000, 0x20000, &origbyte20, &remakebyte20, 0x50000);

		//uint8_t origbyte20 = 0;
		//uint8_t remakebyte20 = 0;
		comp20 = compare_with_sequence_D41A0((char*)"002454F0-00356038", (uint8_t*)& D41A0_BYTESTR_0, 0x356038, debugcounter_644F0, 224790, &origbyte20, &remakebyte20);

		comp20 = compare_with_sequence_array_E2A74((char*)"002454F0-002B3A74", (uint8_t*)& array_E2A74, 0x2b3a74, debugcounter_644F0, 0xc4e, 0xc4e, &origbyte20, &remakebyte20);
		comp20 = compare_with_sequence((char*)"002454F0-003514B0", (uint8_t*)& str_unk_1804B0ar, 0x3514b0, debugcounter_644F0, 0xab, 0xab, &origbyte20, &remakebyte20);
	}

	//if (debugcounter_644F0 >= 0x110)
	{
		debugcounter_644F0++;
		debugcounter_644F0--;
	}
	comp20 = compare_with_sequence((char*)"002454F0-003AA0A4", pdwScreenBuffer_351628, 0x3aa0a4, debugcounter_47560, 320 * 200, 320 * 200, &origbyte20, &remakebyte20);
	debugcounter_644F0++;
	*/

	axis_3d* v36x; // [esp-4h] [ebp-BEh]

	int v37x[0xa8]; // [esp+0h] [ebp-BAh]
	//int v38[5]; // [esp+4h] [ebp-B6h]//v37x[1]

	//int v39; // [esp+18h] [ebp-A2h]//v37x[6]
	//int v40; // [esp+1Ch] [ebp-9Eh]//v37x[7]
	//int v41; // [esp+30h] [ebp-8Ah]//v37x[12]
	//int v42; // [esp+34h] [ebp-86h]//v37x[13]
	//int v43; // [esp+48h] [ebp-72h]//v37x[18]
	//int v44; // [esp+4Ch] [ebp-6Eh]//v37x[19]
	//int v45; // [esp+60h] [ebp-5Ah]//v37x[24]
	//int v46; // [esp+64h] [ebp-56h]//v37x[25]
	//int v47; // [esp+78h] [ebp-42h]//v37x[30]
	//int v48; // [esp+7Ch] [ebp-3Eh]//v37x[31]
	//int v49; // [esp+90h] [ebp-2Ah]//v37x[36]
	//int v50; // [esp+94h] [ebp-26h]//v37x[37]

	v51x_struct v51x; // [esp+A8h] [ebp-12h]//0
	//int16_t v52; // [esp+ACh] [ebp-Eh]//+4
	//uint8_t* v53; // [esp+AEh] [ebp-Ch]//+6
	//int v54; // [esp+B2h] [ebp-8h]//+10
	//int v55; // [esp+B6h] [ebp-4h]//+14
	//int v56; // [esp+BAh] [ebp+0h]//+18
	//int v57; // [esp+BEh] [ebp+4h]//+22
	//int v58; // [esp+C2h] [ebp+8h]//+26
	//int v59; // [esp+C6h] [ebp+Ch]//+30
	//int v60; // [esp+CAh] [ebp+10h]//+34
	//int v61; // [esp+CEh] [ebp+14h]//+38
	//int16_t v62; // [esp+D2h] [ebp+18h]//+42
	//int16_t v63; // [esp+D4h] [ebp+1Ah]//+44
	//char v64; // [esp+D6h] [ebp+1Ch]//+46
	axis_3d v65y; // [esp+D8h] [ebp+1Eh]
	//int16_t v66; // [esp+DCh] [ebp+22h]
	axis_3d v67x; // [esp+E0h] [ebp+26h]//+6
	//int16_t v68; // [esp+E4h] [ebp+2Ah]//+10
	int v69; // [esp+E8h] [ebp+2Eh]//+14
	int v70; // [esp+ECh] [ebp+32h]//+18
	int v71; // [esp+F0h] [ebp+36h]//+22
	type_entity_0x6E8E* v72x; // [esp+F4h] [ebp+3Ah]//+26
	//int v73; // [esp+F8h] [ebp+3Eh]//+30
	//int v74; // [esp+FCh] [ebp+42h]//34
	type_entity_0x6E8E* v75x; // [esp+100h] [ebp+46h]//38
	int v76; // [esp+104h] [ebp+4Ah]//+40
	int v77; // [esp+108h] [ebp+4Eh]
	int v78; // [esp+10Ch] [ebp+52h]
	int v79; // [esp+110h] [ebp+56h]
	uint8_t j; // [esp+114h] [ebp+5Ah]
	char v81; // [esp+118h] [ebp+5Eh]
	int v82; // [esp+148h] [ebp+8Eh]
	int v83; // [esp+14Ch] [ebp+92h]

	//fix it
	v51x.v62xw_42 = 0;
	v51x.v63xw_44 = 0;
	v72x = 0;
	//v66 = 0;
	//fix it

	v8 = 0x1000000;
	v81 = 0;
	if (x_WORD_180660_VGA_type_resolution & 1)
	{
		x >>= 1;
		y >>= 1;
		width >>= 1;
		height >>= 1;
		scaling *= 2;
	}

	v51x.v53xdw_6 = screenWidth_18062C * y + pdwScreenBuffer_351628 + x;
	//LODWORD(v9) = 0x10000;
	//HIDWORD(v9) = 0x10000 >> 31;
	v9x = 0x10000;
	v83 = v9x / scaling;
	v51x.v56xdw_18 = posX;
	v51x.v57xdw_22 = posY;
	v51x.v58xdw_26 = width;
	v51x.v59xdw_30 = height;
	v51x.v60xdw_34 = width / 2;
	v10 = yaw & 0x7FF;
	v11 = v83 * Maths::sin_DB750[v10];
	v51x.v61xdw_38 = height / 2;
	v12 = v83 * Maths::sin_DB750[0x200 + v10] >> 16;
	v51x.v55xdw_14 = -v11 >> 16;
	v51x.v54xdw_10 = v12;
	v82 = yaw & 0x7FF;
	v13 = D41A0_0.LevelIndex_0xc;
	v75x = Entities_EA3E4[D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
	v14 = sub_595C0(v13);
	switch (v14 + 3)
	{
	case 0:
	case 1:
	case 2:
		if (v14 == -3)
		{
			for (ix = x_D41A0_BYTEARRAY_4_struct.dword_38535; ; ix = ix->next_0)
			{
				v72x = ix;
				if (ix <= Entities_EA3E4[0])
					break;
				//LOBYTE(i) = (uint8_t)v72;//can remove it
				if (ix->model_0x40_64 == 31)
				{
					v81 = 1;
					break;
				}
			}
		}
		else
		{
			for (ix = x_D41A0_BYTEARRAY_4_struct.dword_38535; ; ix = ix->next_0)
			{
				v72x = ix;
				if (ix <= Entities_EA3E4[0])
					break;
				//LOBYTE(i) = (uint8_t)v72;//can remove it
				if (ix->model_0x40_64 == 12)
				{
					v81 = 1;
					break;
				}
			}
		}
		if (v81)
		{
			v36x = &v67x;
			v16x = &v75x->position_0x4C_76;
			v17x = &v72x->position_0x4C_76;
			goto LABEL_47;
		}
		break;
	case 4:
	case 5:
		//LOBYTE(i) = (uint8_t)sub_59610(v72x, D41A0_BYTESTR_0.word_0xc);
		if (sub_59610(&v72x, D41A0_0.LevelIndex_0xc))
		{
			v51x.v64xb_46 = (*xadataclrd0dat.colorPalette_var28)[3840];
			v51x.v51y = v72x->position_0x4C_76;
			if (DrawObjectiveRectangle_64CE0(&v51x, scale))
				sub_885E0(v72x, v51x.v62xw_42, v51x.v63xw_44, 0x52u);
			v36x = &v72x->position_0x4C_76;
			v17x = &v72x->position_0x4C_76;
			v16x = &v75x->position_0x4C_76;
			v81 = 1;
			goto LABEL_47;
		}
		break;
	case 8: //Draw Objective Rectangle
		//LOBYTE(i) = sub_596C0(v65x, D41A0_BYTESTR_0.word_0xc);
		if (sub_596C0(&v65y, D41A0_0.LevelIndex_0xc))
		{
			v51x.v64xb_46 = (*xadataclrd0dat.colorPalette_var28)[0xff0];
			v51x.v51y = v65y;
			if (DrawObjectiveRectangle_64CE0(&v51x, scale))
				sub_885E0(0, v51x.v62xw_42, v51x.v63xw_44, 0x51u);
			v36x = &v65y;
			v17x = &v65y;
			v16x = &v75x->position_0x4C_76;
			v81 = 1;
		LABEL_47:
			v67x = *v17x;
			//i = (uint8_t*)Maths::EuclideanDistXYZ_58490(v16x, v36x);
			v8 = Maths::EuclideanDistXYZ_58490(v16x, v36x);
		}
		break;
	case 0xA: //Markers around Enemies
		v18 = D41A0_0.stages_0x3654C[D41A0_0.struct_0x3659C[D41A0_0.LevelIndex_0xc].substr_3659C.ObjectiveText_1].str_36552_un.dword;
		v51x.v64xb_46 = (*xadataclrd0dat.colorPalette_var28)[3840];
		for (ix = x_D41A0_BYTEARRAY_4_struct.bytearray_38403x[v18]; ; ix = ix->next_0)
		{
			v72x = ix;
			if (ix <= Entities_EA3E4[0])
				break;
			v51x.v51y = v72x->position_0x4C_76;
			if (DrawObjectiveRectangle_64CE0(&v51x, scale))
				sub_885E0(v72x, v51x.v62xw_42, v51x.v63xw_44, 0x52u);
			v19 = Maths::EuclideanDistXYZ_58490(&v75x->position_0x4C_76, &v72x->position_0x4C_76);
			if (v19 < v8)
			{
				v81 = 1;
				v8 = v19;
				v67x = v72x->position_0x4C_76;
			}
		}
		break;
	case 0xC:
		v51x.v64xb_46 = (*xadataclrd0dat.colorPalette_var28)[3840];
		v76 = D41A0_0.stages_0x3654C[D41A0_0.struct_0x3659C[D41A0_0.LevelIndex_0xc].substr_3659C.ObjectiveText_1].str_36552_un.dword;
		for (ix = x_D41A0_BYTEARRAY_4_struct.dword_38527; ; ix = ix->next_0)
		{
			v72x = ix;
			if (ix <= Entities_EA3E4[0])
				break;
			v79 = v76;
			for (j = 0; v79 && j < 8u; j++)
			{
				if (v72x->byte_0x46_70 == v79)
				{
					v51x.v51y = v72x->position_0x4C_76;
					if (DrawObjectiveRectangle_64CE0(&v51x, scale))
						sub_885E0(v72x, v51x.v62xw_42, v51x.v63xw_44, 0x52u);
					v20 = Maths::EuclideanDistXYZ_58490(&v75x->position_0x4C_76, &v72x->position_0x4C_76);
					if (v20 < v8)
					{
						v8 = v20;
						v81 = 1;
						v67x = v72x->position_0x4C_76;
					}
				}
				v79 = str_D93C0_bldgprmbuffer[v79].byte_3;
			}
		}
		break;
	default:
		break;
	}
	if (v81)
	{
		//Draw Arrow
		
		//LOBYTE(i) = (uint8)x_D41A0_BYTEARRAY_4;
		if (x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26 & 0x40)
		{
			if (x_D41A0_BYTEARRAY_4_struct.colorIndex_121[6])
			{
				v21 = v8 - 512;
				LOWORD(v78) = Maths::sub_581E0_maybe_tan2(&v75x->position_0x4C_76, &v67x);
				v78 = (uint16_t)v78;
				if (v21 > 15872)
					LOWORD(v21) = 15872;
				v67x = v75x->position_0x4C_76;
				MoveEntity_57FA0(&v67x, v78, 0, v21);
				v22 = v83 * Maths::sin_DB750[v82];
				v23 = v83 * (x_DWORD)Maths::sin_DB750[0x200 + v82];
				v71 = (int16_t)(*(int16_t*)&v67x - posX);
				v23 >>= 16;
				v24 = -v22 >> 16;
				v25 = width / 2 + ((v23 * v71 - v24 * (int16_t)((int16_t)v67x.y - posY)) >> 16);
				v37x[7] = 13 * scale;
				v37x[13] = 13 * scale;
				v37x[19] = 13 * scale;
				v37x[37] = 13 * scale;
				v37x[6] = 9 * scale;
				v77 = height / 2 + ((v71 * v24 + (int16_t)((int16_t)v67x.y - posY) * v23) >> 16);
				v37x[0] = 0;
				v37x[1] = 0;
				v37x[12] = -9 * scale;
				v37x[30] = -4 * scale;
				v37x[36] = -4 * scale;
				v37x[25] = 23 * scale;
				v37x[31] = 23 * scale;
				v37x[18] = 4 * scale;
				v37x[24] = 4 * scale;
				v26 = 1;
				v78 = ((x_WORD)v78 - (x_WORD)v82) & 0x7FF;
				v27 = Maths::sin_DB750[v78];
				v28 = Maths::sin_DB750[0x200 + v78];
				while (v26 < 7)
				{
					v29 = 3 * v26;
					v70 = 16 * v37x[6 * v26];
					v69 = 16 * v37x[1 + 6 * v26];
					v37x[2 * v29] = ((signed int)v28 * v70 - v27 * v69) >> 20;
					//v73 = v27 * v70;
					//v74 = (x_DWORD)v28 * v69;
					v26++;
					v37x[1 + 2 * v29] = ((signed int)v28 * v69 + v27 * v70) >> 20;
				}
				if (x_WORD_180660_VGA_type_resolution & 1)
				{
					v25 *= 2;
					v77 *= 2;
				}
				colour_v30 = (*xadataclrd0dat.colorPalette_var28)[3840];
				v31 = 0;
				while (v31 < 7)
				{
					v32 = 3 * v31;
					v37x[2 * v32] = v25 + x + v37x[6 * v31];
					v33 = v37x[1 + (6 * v31++)];
					v37x[1 + 2 * v32] = v77 + y + v33;
				}
				//adress 245be1
				//v34 = v30;
				DrawLine_2BD10(v37x[0], v37x[1], v37x[6], v37x[7], colour_v30);
				DrawLine_2BD10(v37x[6], v37x[7], v37x[18], v37x[19], colour_v30);
				DrawLine_2BD10(v37x[18], v37x[19], v37x[24], v37x[25], colour_v30);
				DrawLine_2BD10(v37x[24], v37x[25], v37x[30], v37x[31], colour_v30);
				DrawLine_2BD10(v37x[30], v37x[31], v37x[36], v37x[37], colour_v30);
				DrawLine_2BD10(v37x[36], v37x[37], v37x[12], v37x[13], colour_v30);
				DrawLine_2BD10(v37x[12], v37x[13], v37x[0], v37x[1], colour_v30);
			}
		}
	}
}

//----- (00064CE0) --------------------------------------------------------
char DrawObjectiveRectangle_64CE0(v51x_struct* a1, uint8_t scale)//245ce0
{
	int16_t v1; // ecx
	int16_t v2; // esi
	char v3; // ah
	int posX_v4; // ebx
	char colour_v5; // al
	int posY_v6; // ecx
	int screenWidth_v7; // esi
	int startY_v8; // ecx
	int startX_v9; // ebx
	x_BYTE* v10; // edx
	x_BYTE* v11; // edx
	x_BYTE* v12; // edx
	x_BYTE* v13; // edx
	x_BYTE* v14; // edx
	x_BYTE* v15; // edx

	v1 = a1->v51y.x - a1->v56xdw_18;
	v2 = a1->v51y.y - a1->v57xdw_22;
	v3 = 0;
	posX_v4 = a1->v60xdw_34 + ((int32_t)(v1 * a1->v54xdw_10 - v2 * a1->v55xdw_14) >> 16);
	colour_v5 = a1->v64xb_46;
	if (posX_v4 >= 0 && posX_v4 < a1->v58xdw_26)
	{
		posY_v6 = a1->v61xdw_38 + (int16_t)((a1->v54xdw_10 * v2 + a1->v55xdw_14 * v1) >> 16);
		if (posY_v6 >= 3 && posY_v6 < a1->v59xdw_30 - 3 && posX_v4 >= x_WORD_F4960[1 + 2 * posY_v6] + 3 && posX_v4 < x_WORD_F4960[2 * posY_v6] - 3)
		{
			a1->v62xw_42 = posX_v4;
			a1->v63xw_44 = posY_v6;
			v3 = 1;
			if (!(x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26 & 3))
			{
				DrawRectangle(pdwScreenBuffer_351628, posX_v4 - scale - CalculateScaleOffset(scale), posY_v6 - scale - CalculateScaleOffset(scale), 3 * scale, 3 * scale, scale, screenWidth_18062C, colour_v5);

				if (1 == D41A0_0.m_GameSettings.m_Display.m_uiScreenSize)
				{
					screenWidth_v7 = screenWidth_18062C;
					startY_v8 = screenWidth_18062C * (posY_v6 - scale);
					startX_v9 = posX_v4 - scale;

					v13 = (x_BYTE*)(startY_v8 + startX_v9 + x_DWORD_E9C3C);
					v13[0] = colour_v5;
					v13[1] = colour_v5;
					v13[2] = colour_v5;
					v14 = &v13[screenWidth_v7];
					v14[0] = colour_v5;
					v14[2] = colour_v5;
					v15 = &v14[screenWidth_v7];
					v15[0] = colour_v5;
					v15[1] = colour_v5;
					v15[2] = colour_v5;
				}
			}
		}
	}
	return v3;
}

void DrawRectangle(uint8_t* ptrImageBuffer, int posX, int posY, int width, int height, uint8_t lineThickness, uint32_t pitch, uint8_t colour)
{
	if (posX < 0 || posY < 0)
		return;

	uint8_t * ptrImageStart =  (pitch * posY) + posX + ptrImageBuffer;

	//Top
	for (int y = 0; y < lineThickness; y++)
	{
		for (int x = 0; x < width; x++)
		{
			*ptrImageStart = colour;
			ptrImageStart++;
		}
		ptrImageStart += (pitch - width);
	}

	//Middle
	ptrImageStart = (pitch * (posY + lineThickness)) + posX + ptrImageBuffer;
	for (int y = 0; y < (height - (2 * lineThickness)); y++)
	{
		for (int x = 0; x < lineThickness; x++)
		{
			*ptrImageStart = colour;
			ptrImageStart++;
		}
		ptrImageStart += (pitch - lineThickness);
	}
	ptrImageStart = (pitch * (posY + lineThickness)) + (posX + (width - lineThickness)) + ptrImageBuffer;
	for (int y = 0; y < (height - (2 * lineThickness)); y++)
	{
		for (int x = 0; x < lineThickness; x++)
		{
			*ptrImageStart = colour;
			ptrImageStart++;
		}
		ptrImageStart += (pitch - lineThickness);
	}

	//Bottom
	ptrImageStart = (pitch * (posY + height - lineThickness)) + posX + ptrImageBuffer;
	for (int y = 0; y < lineThickness; y++)
	{
		for (int x = 0; x < width; x++)
		{
			*ptrImageStart = colour;
			ptrImageStart++;
		}
		ptrImageStart += (pitch - width);
	}
}

//----- (00087C10) --------------------------------------------------------
void sub_87C10()//268c10
{
	if (x_D41A0_BYTEARRAY_4_struct.showHelp_10)
	{
		str_unk_1804B0ar.dword_0x6a = 0;
		str_unk_1804B0ar.PopupStatusByte_0x9e &= 0xDFu;
		str_unk_1804B0ar.byte_0x9f &= 0xFEu;
		str_unk_1804B0ar.dword_0x72 = str_unk_1804B0ar.dword_0x6e;
		str_E2A74[str_unk_1804B0ar.Index_0x86].axis_2[0] &= 0xffFDu;
		str_E2A74[str_unk_1804B0ar.Index_0x86].dword_16 = 0;
		str_E2A74[str_unk_1804B0ar.Index_0x86].dword_12 = 0;
		str_E2A74[str_unk_1804B0ar.Index_0x86].byte_29 = str_E2A74[str_unk_1804B0ar.Index_0x86].byte_28;
		sub_889F0(str_unk_1804B0ar.Index_0x86);
		str_unk_1804B0ar.Index_0x86 = 0;
	}
}

int debugcounter_2695e0 = 0;

//----- (000885E0) --------------------------------------------------------
void sub_885E0(type_entity_0x6E8E* a1x, int16_t posX, int16_t posY, uint16_t a4)//2695e0
{
	signed int v4; // eax
	int v5; // edx
	//unsigned int v6; // ebx
	//int v7; // ebx
	//char v8; // al
	//char v9; // dl
	//char v10; // dh
	char v12; // [esp+0h] [ebp-4h]

	if (debugcounter_2695e0 == 0xa)
	{
		debugcounter_2695e0++;
		debugcounter_2695e0--;
	}

	//LOBYTE(v4) = (uint8)x_D41A0_BYTEARRAY_4;
	v5 = 0;
	if (x_D41A0_BYTEARRAY_4_struct.showHelp_10)
	{
		//LOBYTE(v4) = a4;
		if (!(str_E2A74[a4].axis_2[0] & 8))
		{
			if (x_WORD_180660_VGA_type_resolution & 1)
			{
				posX *= 2;
				posY *= 2;
			}
			//LOBYTE(v4) = a4;
			//v6 = 30 * (int16_t)a4;
			if (str_E2A74[a4].axis_2[0] & 2)
			{
				if (a1x == str_E2A74[a4].dword_12 && !(*(int16_t*)&str_unk_1804B0ar.PopupStatusByte_0x9e & 0x20))
				{
					//v9 = array_E2A74[0x2 + 30 * a4] | 0x10;
					str_E2A74[a4].axis_2[3] = posX;
					str_E2A74[a4].axis_2[0] |= 0x10;
					//LOBYTE(v4) = a3;
					//v10 = HIBYTE(*(int16_t*)&str_unk_1804B0ar.PopupStatusByte_0x9e) | 2;
					str_E2A74[a4].axis_2[4] = posY;
					str_unk_1804B0ar.byte_0x9f |= 2;
				}
			}
			else
			{
				v12 = 0;
				if (!a1x
					|| (v4 = Maths::EuclideanDistXYZ_58490(
						&Entities_EA3E4[D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240]->position_0x4C_76,
						&a1x->position_0x4C_76),
						v5 = v4,
						!str_E2A74[a4].dword_12)
					|| v4 < str_E2A74[a4].dword_20)
				{
					v12 = 1;
				}
				if (v12)
				{
					//v7 = 15 * (int16_t)a4;
					str_E2A74[a4].dword_12 = a1x;//xx
					//v8 = array_E2A74[0x2 + 30 * a4];
					str_E2A74[a4].dword_20 = v5;
					//LOBYTE(v4) = v8 | 0x10;
					str_E2A74[a4].axis_2[0] |= 0x10;
				}
			}
		}
		if (a4 >= 0x51u && a4 <= 0x53u)
		{
			//LOBYTE(v4) = a4;
			str_E2A74[a4].axis_2[0] |= 0x40u;
		}
	}

	debugcounter_2695e0++;
}

//----- (00089420) --------------------------------------------------------
void ComputeTextboxSizesFromTextWords_89420(Type_TextBox_1804B0* textbox, const char* text, uint8_t scale)//26a420
{
	int lineCharWidth; // eax
	unsigned int textBufferLenght; // kr04_4
	signed int textWidth; // ecx
	signed int textboxWidth; // ebx
	int lastLineIndex;
	int textIndex;
	int16_t countOfLines; // [esp+4h] [ebp-4h]

	textBufferLenght = strlen(text) + 1;
	countOfLines = 1;
	textbox->textBoxWidth_0x4 = 0;
	textbox->textboxHeight_0x6 = 0;
	if (textBufferLenght != 1)
	{
		textWidth = (textbox->charWidth_0x10 * scale) * strlen(text);
		textboxWidth = textbox->maxTextboxWidth2_0xc;
		if (textWidth <= textboxWidth)
		{
			textbox->textBoxWidth_0x4 = textWidth;
		}
		else
		{
			if (textWidth % textboxWidth < textboxWidth / 2)//last line is lower than halfscreen
			{
				while (1)
				{
					if (textWidth % textboxWidth >= textboxWidth / 2 || textboxWidth <= 0)
						break;
					textboxWidth -= textbox->charWidth_0x10 * scale;
				}
			}
			if (textboxWidth > 0)
			{
				lineCharWidth = (textboxWidth + (textbox->charWidth_0x10 * scale) - 1) / (textbox->charWidth_0x10 * scale);
				countOfLines = splitTextIntoLines(text, lineCharWidth).size();
				textbox->textBoxWidth_0x4 = textboxWidth;
			}
		}
		textbox->textboxHeight_0x6 = (textbox->charHeight_0x12 * scale) * countOfLines;
	}
}

//----- (00089520) --------------------------------------------------------
void ConstrainTextboxSizes_89520(Type_TextBox_1804B0* textbox, uint8_t scale)//26a520
{
	int textboxHeight; // edi
	int centerToMaxWidth; // ecx
	int centerToMaxHeight; // esi
	int textBoxWidth; // [esp+0h] [ebp-Ch]

	int frameCornerWidth = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[FRAME_CORNER].width_4 * scale;
	int frameCornerHeight = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[FRAME_CORNER].height_5 * scale;

	textBoxWidth = textbox->textBoxWidth_0x4;
	textboxHeight = textbox->textboxHeight_0x6;
	centerToMaxWidth = textbox->maxTextboxWidth_0x0 - (textBoxWidth / 2);
	centerToMaxHeight = textbox->maxTextboxHeight_0x2 - (textboxHeight / 2);

	if (centerToMaxWidth < textbox->minPosX_0x14 + frameCornerWidth)
		centerToMaxWidth = textbox->minPosX_0x14 + frameCornerWidth;

	if (centerToMaxHeight < textbox->minPosY_0x18 + frameCornerHeight)
		centerToMaxHeight = textbox->minPosY_0x18 + frameCornerHeight;

	if (centerToMaxWidth + textBoxWidth >= textbox->maxPosX_0x16)
		centerToMaxWidth = textbox->maxPosX_0x16 - textBoxWidth;

	if (centerToMaxHeight + textboxHeight >= textbox->maxPosY_0x1a)
		centerToMaxHeight = textbox->maxPosY_0x1a - textboxHeight;

	textbox->textboxPosX_0x8 = centerToMaxWidth;
	textbox->textboxPosY_0xa = centerToMaxHeight;
	textbox->maxTextboxWidth_0x0 = centerToMaxWidth + (textBoxWidth / 2);
	textbox->maxTextboxHeight_0x2 = centerToMaxHeight + (textboxHeight / 2);
}

//----- (000895D0) --------------------------------------------------------
void DrawTextboxText_895D0(Type_TextBox_1804B0* textbox, const char* text, uint8_t scale)//26a5d0
{
	int width = textbox->textBoxWidth_0x4 / (textbox->charWidth_0x10 * scale);
	std::vector<std::string> lines = splitTextIntoLines(text, width);
	int posY = textbox->textboxPosY_0xa;
	for (auto& line : lines) {
		DrawText_2BC10(line.c_str(), textbox->textboxPosX_0x8, posY, textbox->color1_0x30, scale);
		posY += textbox->charHeight_0x12 * scale;
	}
}


//----- (00089690) --------------------------------------------------------
void DrawTextboxFrame_89690(Type_TextBox_1804B0* textbox, uint8_t scale)//26a690
{
	int width1 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[FRAME_TOP_BOTTOM_SIDE].width_4 * scale;
	int width2 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[FRAME_LEFT_RIGHT_SIDE].width_4 * scale;
	int height1 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[FRAME_CORNER].height_5 * scale;
	int height2 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[FRAME_LEFT_RIGHT_SIDE].height_5 * scale;
	int beginX = textbox->framePosX_0x24 - width2;
	int endX = textbox->frameWidth_0x28 + textbox->framePosX_0x24;
	int beginYPlus = textbox->framePosY_0x26 - ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[FRAME_TOP_BOTTOM_SIDE].height_5 * scale);
	int endY = textbox->frameHeight_0x2a + textbox->framePosY_0x26;
	int countYchars = textbox->textboxHeight_0x6 / (textbox->charHeight_0x12 * scale);
	//Top Left/Right corners
	DrawBitmap_2BB40(textbox->framePosX_0x24 - width2, beginYPlus, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[FRAME_CORNER], scale);
	DrawBitmap_2BB40(endX, beginYPlus, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[FRAME_CORNER], scale);
	int indexY = height1 + beginYPlus;

	//Sides Left/Right 
	for (int i = 0; i < countYchars; i++)
	{
		DrawBitmap_2BB40(beginX, indexY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[FRAME_LEFT_RIGHT_SIDE], scale);
		DrawBitmap_2BB40(endX, indexY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[FRAME_LEFT_RIGHT_SIDE], scale);
		indexY += height2;
	}

	//Bottom Left/Right corners
	DrawBitmap_2BB40(beginX, indexY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[FRAME_CORNER], scale);
	DrawBitmap_2BB40(endX, indexY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[FRAME_CORNER], scale);

	//Sides Top/Bottom 
	for (int i = beginX + width2; i < endX; i += width1)
	{
		DrawBitmap_2BB40(i, beginYPlus, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[FRAME_TOP_BOTTOM_SIDE], scale);
		DrawBitmap_2BB40(i, endY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[FRAME_TOP_BOTTOM_SIDE], scale);
	}
}


//----- (00089980) --------------------------------------------------------
void ComputeFrameSizes_89980(Type_TextBox_1804B0* textbox, uint8_t scale)//26a980
{
	int width = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[FRAME_TOP_BOTTOM_SIDE].width_4 * scale;

	int height1 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[FRAME_TOP_BOTTOM_SIDE].height_5 * scale;
	int height2 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[FRAME_LEFT_RIGHT_SIDE].height_5 * scale;
	int height3 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[FRAME_CORNER].height_5 * scale;
	textbox->frameWidth_0x28 = width * (((width) - (1 * scale) + textbox->textBoxWidth_0x4 + (8 * scale)) / (width));
	textbox->framePosX_0x24 = textbox->textboxPosX_0x8 - (textbox->frameWidth_0x28 - textbox->textBoxWidth_0x4) / 2;
	textbox->frameHeight_0x2a = (height2 - (1 * scale) + textbox->textboxHeight_0x6) / height2 * height2 + 2 * (height3 - height1);
	textbox->framePosY_0x26 = textbox->textboxPosY_0xa - (textbox->frameHeight_0x2a - textbox->textboxHeight_0x6) / 2;
}

//----- (000BD524) --------------------------------------------------------
void DrawLine_BD524(int16_t posX1, int16_t posY1, int16_t posX2, int16_t posY2, uint8_t colour)//29e524
{
	DrawLine_BD542(posX1 / 2, posY1 / 2, posX2 / 2, posY2 / 2, colour);
}

//----- (000BD542) --------------------------------------------------------
void DrawLine_BD542(uint16_t posX1, uint16_t posY1, uint16_t posX2, uint16_t posY2, uint8_t colour)//29e542
{
	int16_t v5; // bx
	int16_t v6; // cx
	uint16_t v7; // cx
	uint16_t v8; // ax
	uint16_t v9; // ax
	_BYTE* v10; // edi
	int v11; // esi
	uint16_t v12; // ax
	uint16_t v13; // cx
	uint16_t v14; // si
	int16_t v15; // dx
	int16_t v16; // bx
	int16_t v17; // cx
	int v18; // edi
	unsigned int v19; // ecx
	int16_t v20; // ax
	int v21; // eax
	int v22; // ecx
	int16_t v23; // cx
	_BYTE* v24; // edi
	int v25; // [esp+0h] [ebp-Ch]
	int v26; // [esp+4h] [ebp-8h]
	uint8_t* v27; // [esp+8h] [ebp-4h]

	v27 = &pdwScreenBuffer_351628[x_DWORD_18063C_sprite_sizex + screenWidth_18062C * x_DWORD_180650_positiony];
	v5 = posY1;
	if ((posY1 & 0x8000u) == 0)
	{
		v7 = posY2;
		if ((posY2 & 0x8000u) == 0)
		{
			if (posY1 == posY2)
			{
				if ((int16_t)posY1 >= (int16_t)x_DWORD_180644_map_resolution2_y)
					return;
				v18 = posX1;
				v19 = posX2;
				if (posX2 < posX1)
				{
					LOWORD(v19) = posX1;
					LOWORD(v18) = posX2;
				}
				v20 = x_DWORD_180648_map_resolution2_x - 1;
				if ((v18 & 0x8000u) == 0)
				{
					if ((int16_t)v18 > v20)
						return;
					if ((uint16_t)v19 > (uint16_t)v20)
						LOWORD(v19) = x_DWORD_180648_map_resolution2_x - 1;
				}
				else
				{
					if ((int16_t)v19 <= 0)
						return;
					LOWORD(v18) = 0;
					if ((uint16_t)v19 > (uint16_t)v20)
						LOWORD(v19) = x_DWORD_180648_map_resolution2_x - 1;
				}
				LOWORD(v19) = v19 - v18;
				goto LABEL_54;
			}
			if ((int16_t)(posY1 - posY2) < 0)
			{
				if ((int16_t)posY1 >= (int16_t)x_DWORD_180644_map_resolution2_y)
					return;
				if ((int16_t)posY2 >= (int16_t)x_DWORD_180644_map_resolution2_y)
					goto LABEL_5;
			}
			else
			{
				if ((int16_t)posY2 >= (int16_t)x_DWORD_180644_map_resolution2_y)
					return;
				posY2 = posY1;
				posY1 = v7;
				v9 = posX1;
				posX1 = posX2;
				posX2 = v9;
				if (v5 >= (int16_t)x_DWORD_180644_map_resolution2_y)
					goto LABEL_5;
			}
			goto LABEL_15;
		}
		posY2 = posY1;
		v5 = v7;
		v6 = posY1;
		v8 = posX1;
		posX1 = posX2;
		posX2 = v8;
	}
	else
	{
		v6 = posY2;
	}
	if (v6 < 0)
		return;
	posX1 += (int16_t)-v5 * (int16_t)(posX2 - posX1) / (int16_t)(v6 - v5);
	posY1 = 0;
	if ((int16_t)(posY2 - x_DWORD_180644_map_resolution2_y) >= 0)
	{
	LABEL_5:
		posX2 = posX1 + (int16_t)(x_DWORD_180644_map_resolution2_y - posY1) * (int16_t)(posX2 - posX1) / (int16_t)(posY2 - posY1);
		posY2 = x_DWORD_180644_map_resolution2_y - 1;
	}
LABEL_15:
	if ((posX1 & 0x8000u) == 0)
	{
		if ((int16_t)(posX1 - x_DWORD_180648_map_resolution2_x) < 0)
		{
			if ((posX2 & 0x8000u) == 0)
			{
				if ((int16_t)(posX2 - x_DWORD_180648_map_resolution2_x) < 0)
				{
					v26 = 1;
					if (posX1 == posX2)
					{
						v21 = posY1;
						v22 = posY2;
						if (posY2 >= posY1)
						{
							v23 = posY2 - posY1;
						}
						else
						{
							LOWORD(v21) = posY2;
							v23 = posY1 - posY2;
						}
						v24 = (_BYTE*)(v27 + screenWidth_18062C * v21 + posX1);
						LOWORD(v22) = v23 + 1;
						do
						{
							*v24 = colour;
							v24 += screenWidth_18062C;
							--v22;
						} while (v22);
						return;
					}
					if ((int16_t)(posX1 - posX2) >= 0)
						v26 = -1;
				}
				else
				{
					v26 = 1;
					posY2 = posY1 + (int16_t)(x_DWORD_180648_map_resolution2_x - posX1) * (int16_t)(posY2 - posY1) / (int16_t)(posX2 - posX1);
					posX2 = x_DWORD_180648_map_resolution2_x - 1;
				}
			}
			else
			{
				v26 = -1;
				posY2 = posY1 + (int16_t)posX1 * (int16_t)(posY2 - posY1) / (int16_t)(posX1 - posX2);
				posX2 = 0;
			}
		}
		else
		{
			if ((int16_t)posX2 >= (int16_t)x_DWORD_180648_map_resolution2_x)
				return;
			v26 = -1;
			posY1 += (int16_t)(posX1 - x_DWORD_180648_map_resolution2_x) * (int16_t)(posY2 - posY1) / (int16_t)(posX1 - posX2);
			posX1 = x_DWORD_180648_map_resolution2_x - 1;
			if ((posX2 & 0x8000u) != 0)
			{
				posY2 -= (int16_t)-posX2 * (int16_t)(posY2 - posY1) / (int16_t)(posX1 - posX2);
				posX2 = 0;
			}
		}
	}
	else
	{
		if ((posX2 & 0x8000u) != 0)
			return;
		v26 = 1;
		posY1 += (int16_t)-posX1 * (int16_t)(posY2 - posY1) / (int16_t)(posX2 - posX1);
		posX1 = 0;
		if ((int16_t)(posX2 - x_DWORD_180648_map_resolution2_x) >= 0)
		{
			posY2 = posY1 + (int16_t)x_DWORD_180648_map_resolution2_x * (int16_t)(posY2 - posY1) / (int16_t)posX2;
			posX2 = x_DWORD_180648_map_resolution2_x - 1;
		}
	}
	v10 = (_BYTE*)(v27 + screenWidth_18062C * posY1 + posX1);
	v11 = v26;
	v12 = v26 * (posX2 - posX1);
	v13 = posY2 - posY1;
	if (posY2 == posY1)
	{
		v18 = posX1;
		v19 = posX2;
		if (posX2 >= posX1)
		{
			LOWORD(v19) = posX2 - posX1;
		}
		else
		{
			LOWORD(v18) = posX2;
			LOWORD(v19) = posX1 - posX2;
		}
	LABEL_54:
		LOWORD(v19) = v19 + 1;
		memset((void*)(v27 + screenWidth_18062C * posY1 + v18), colour, v19);
		return;
	}
	if (v13 <= v12)
	{
		v25 = screenWidth_18062C;
	}
	else
	{
		v12 = posY2 - posY1;
		v13 = v26 * (posX2 - posX1);
		v26 = screenWidth_18062C;
		v25 = v11;
	}
	v14 = v12;
	v15 = 2 * v13;
	v16 = 2 * v13 - v12;
	v17 = 2 * (v13 - v12);
	*v10 = colour;
	do
	{
		while (1)
		{
			v10 += v26;
			if (v16 >= 0)
				break;
			v16 += v15;
			*v10 = colour;
			if (!--v14)
				return;
		}
		v10 += v25;
		v16 += v17;
		*v10 = colour;
		--v14;
	} while (v14);
}

std::vector<std::string> splitTextIntoWords(const std::string& text) {
	std::vector<std::string> words;
	std::string word;
	std::istringstream tokenStream(text);
	while (std::getline(tokenStream, word, ' ')) {
		words.push_back(word);
	}
	return words;
}

std::vector<std::string> splitTextIntoLines(const std::string& text, int width) {
	std::vector<std::string> lines;
	std::vector<std::string> words = splitTextIntoWords(text);
	std::string line;
	for (const std::string& word : words) {
		if (line.empty()) {
			line = word;
		}
		else {
			if (line.size() + word.size() + 1 <= width) {
				line += " " + word;
			}
			else {
				lines.push_back(line);
				line = word;
			}
		}
	}
	if (!line.empty()) {
		lines.push_back(line);
	}
	return lines;
}	

//----- (000548B0) --------------------------------------------------------
// returns viewport from menu to flight mode
void sub_548B0(type_str_0x2BDE* a1x)//2358b0
{
	if (a1x->word_0x007_2BE4_11237 == D41A0_0.LevelIndex_0xc)
	{
		SetMousePositionInMemory_5BDC0(a1x->dword_0x3E6_2BE4_12228.position_backup_20.x, a1x->dword_0x3E6_2BE4_12228.position_backup_20.y);
		// if a joystick is used, do not set that random resting point from above
		//SetMousePositionInMemory_5BDC0(320, 240);
	}
}

//----- (000548F0) --------------------------------------------------------
void sub_548F0(type_str_0x2BDE* a1x)//2358f0
{
	if (a1x->word_0x007_2BE4_11237 == D41A0_0.LevelIndex_0xc)
	{
		if (unk_18058Cstr.x_WORD_1805C2_joystick != 7 && unk_18058Cstr.x_WORD_1805C2_joystick != 1 && unk_18058Cstr.x_WORD_1805C2_joystick != 2)
		{
			a1x->dword_0x3E6_2BE4_12228.position_backup_20.x = unk_18058Cstr.x_DWORD_1805B0_mouse.x;
			a1x->dword_0x3E6_2BE4_12228.position_backup_20.y = unk_18058Cstr.x_DWORD_1805B0_mouse.y;
		}
		else
		{
			a1x->dword_0x3E6_2BE4_12228.position_backup_20 = x_WORD_E3760_mouse;
		}
	}
}

//----- (0001A280) --------------------------------------------------------
void sub_1A280()//1fb280
{
	x_D41A0_BYTEARRAY_4_struct.dwordindex_192 = 0;
	x_D41A0_BYTEARRAY_4_struct.dwordindex_192 |= 0x01;//help
	if (soundAble_E3798)
		x_D41A0_BYTEARRAY_4_struct.dwordindex_192 |= 0x02;//sound
	if (musicAble_E37FC)
		x_D41A0_BYTEARRAY_4_struct.dwordindex_192 |= 0x04;//music
	if (cdSpeechEnabled_E2A28)
		x_D41A0_BYTEARRAY_4_struct.dwordindex_192 |= 0x08;//speek
	x_D41A0_BYTEARRAY_4_struct.dwordindex_192 |= 0xF0u;//fly,bright,speed,screen size
	if (D41A0_0.str_0x21AA.creflections_0x21AA)
		x_D41A0_BYTEARRAY_4_struct.dwordindex_192 |= 0x100;//reflections
	if (D41A0_0.str_0x21AA.cshadows_0x21AB)
		x_D41A0_BYTEARRAY_4_struct.dwordindex_192 |= 0x400;//shadows
	if (D41A0_0.str_0x21AA.csky_0x21AC)
		x_D41A0_BYTEARRAY_4_struct.dwordindex_192 |= 0x200;//sky
	if (D41A0_0.str_0x21B6.clights_0x21B6)
		x_D41A0_BYTEARRAY_4_struct.dwordindex_192 |= 0x800;//lights
	x_D41A0_BYTEARRAY_4_struct.dwordindex_192 |= 0x1000;//icons
	if (D41A0_0.str_0x21B6.ctransparency_0x21B8)
		x_D41A0_BYTEARRAY_4_struct.dwordindex_192 |= 0x2000;//transparency
	if (D41A0_0.str_0x21B6.cflat_0x21B9)
		x_D41A0_BYTEARRAY_4_struct.dwordindex_192 |= 0x4000;//flat
	if (D41A0_0.str_0x21B2.cresolution_0x21B4)
		x_D41A0_BYTEARRAY_4_struct.dwordindex_192 |= 0x8000;//resolution
	x_D41A0_BYTEARRAY_4_struct.dwordindex_192 |= 0x10000;//names
	x_D41A0_BYTEARRAY_4_struct.dwordindex_188 = 0;
	if (x_D41A0_BYTEARRAY_4_struct.showHelp_10)
		x_D41A0_BYTEARRAY_4_struct.dwordindex_188 |= 0x01;//help
	if (soundActive_E3799)
		x_D41A0_BYTEARRAY_4_struct.dwordindex_188 |= 0x02;//sound
	if (musicActive_E37FD)
		x_D41A0_BYTEARRAY_4_struct.dwordindex_188 |= 0x04;//music
	if (x_D41A0_BYTEARRAY_4_struct.OptionsSettingFlag_24 & SPEECH_ENABLED)
		x_D41A0_BYTEARRAY_4_struct.dwordindex_188 |= 0x08;//speech
	if (D41A0_0.byte_0x36DEA_fly_asistant)
		x_D41A0_BYTEARRAY_4_struct.dwordindex_188 |= 0x10u;//fly
	x_D41A0_BYTEARRAY_4_struct.dwordindex_188 |= 0x20;//bright
	if (x_D41A0_BYTEARRAY_4_struct.speedIndex)
		x_D41A0_BYTEARRAY_4_struct.dwordindex_188 |= 0x40;//speed
	x_D41A0_BYTEARRAY_4_struct.dwordindex_188 |= 0x80u;//screen size
	if (D41A0_0.m_GameSettings.m_Graphics.m_wReflections)
		x_D41A0_BYTEARRAY_4_struct.dwordindex_188 |= 0x100;//reflections
	if (D41A0_0.m_GameSettings.m_Graphics.m_wShadows)
		x_D41A0_BYTEARRAY_4_struct.dwordindex_188 |= 0x400;//shadows
	if (D41A0_0.m_GameSettings.m_Graphics.m_wSky)
		x_D41A0_BYTEARRAY_4_struct.dwordindex_188 |= 0x200;//sky
	if (D41A0_0.m_GameSettings.str_0x2196.m_wDynamicLighting)
		x_D41A0_BYTEARRAY_4_struct.dwordindex_188 |= 0x800;//lights
	if (D41A0_0.m_GameSettings.m_Display.m_wMiniMap)
		x_D41A0_BYTEARRAY_4_struct.dwordindex_188 |= 0x1000;//icons
	if (D41A0_0.m_GameSettings.str_0x2196.transparency_0x2198)
		x_D41A0_BYTEARRAY_4_struct.dwordindex_188 |= 0x2000;//transparency
	if (D41A0_0.m_GameSettings.str_0x2196.flat_0x2199)
		x_D41A0_BYTEARRAY_4_struct.dwordindex_188 |= 0x4000;//flat
	if (D41A0_0.m_GameSettings.str_0x2192.m_wResolution)
		x_D41A0_BYTEARRAY_4_struct.dwordindex_188 |= 0x8000;//resolution
	if (!x_D41A0_BYTEARRAY_4_struct.byteindex_207)
		x_D41A0_BYTEARRAY_4_struct.dwordindex_188 |= 0x10000;//names
}

//----- (0001A4A0) --------------------------------------------------------
int sub_1A4A0()//1fb4a0
{
	int v0; // ebx
	signed int i; // esi
	unsigned int v2; // kr04_4
	unsigned int v3; // kr08_4
	int v5; // [esp+0h] [ebp-8h]
	int v6; // [esp+4h] [ebp-4h]

	v0 = 0;
	for (i = 0; i < 17; i++)
	{
		sub_1A5B0_getLangStrings(i + 1, &v6, &v5);
		v2 = strlen((const char*)x_DWORD_E9C4C_langindexbuffer[v6]) + 1;
		//Help Mode On and etc.
		if ((signed int)(8 * (v2 - 1)) > v0)
			v0 = 8 * (v2 - 1);
		v3 = strlen((const char*)x_DWORD_E9C4C_langindexbuffer[v5]) + 1;
		//Help Mode Off and etc.
		if ((signed int)(8 * (v3 - 1)) > v0)
			v0 = 8 * (v3 - 1);
	}
	return 6 * ((v0 - 11) / 6 + 1) + 16;
}

//----- (0001A030) --------------------------------------------------------
void SetOkayCancelButtonsCursorPosition_1A030()//1fb030
{
	int16_t posX;
	int16_t posY;

	uint8_t scale = 1;

	if (!DefaultResolutions())
	{
		scale = gameUiScale;
}

	GetOkayCancelButtonPositions_30BE0(&posX, &posY, scale);
	SetMousePositionInMemory_5BDC0(posX + (87 * scale), posY + (24 * scale));
}

//----- (0006D200) --------------------------------------------------------
void MoveCursorToSelectedSpell_6D200(type_str_0x2BDE* a1x)//24e200
{
	int8_t selectedSpellIndex; // bl
	unsigned __int8 v5; // bl
	int16_t posX = 0;
	int16_t posY = 0;
	int16_t offsetPosX = 0;
	__int16 subCategoryWidth; // bx
	__int16 subCategoryTotalWidth; // di
	int v10; // ebx
	int subCategoryPosX; // eax
	uint8_t subCategoryIndex;
	int16_t spellIconHeight; // [esp+8h] [ebp-14h]
	int16_t spellIdxX; // [esp+14h] [ebp-8h]
	uint8_t scale = 1;

	int16_t helpHeight = 480;
	if (x_WORD_180660_VGA_type_resolution & 1)
		helpHeight = 400;
	if (x_WORD_180660_VGA_type_resolution != 1)
		if(!DefaultResolutions())
		{
			scale = gameUiScale;
			helpHeight = screenHeight_180624;
			offsetPosX = (screenWidth_18062C - (640 * scale)) / 2;
		}

	if (a1x->word_0x007_2BE4_11237 == D41A0_0.LevelIndex_0xc)
	{
		if (a1x->MenuState_0x3DF_2BE4_12221 == 5 || a1x->MenuState_0x3DF_2BE4_12221 == 8)
		{
			selectedSpellIndex = a1x->dword_0x3E6_2BE4_12228.str_611.spellIndex_0x458_1112;
			if (selectedSpellIndex < 13)
				spellIdxX = selectedSpellIndex;
			else
				spellIdxX = selectedSpellIndex - 13;

			spellIconHeight = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL].height_5 * scale;
			v5 = a1x->dword_0x3E6_2BE4_12228.str_611.byte_0x457_1111;
			if (v5)
			{
				if (v5 <= 2u)
				{
					subCategoryWidth = ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL2].width_4 * scale);
					subCategoryTotalWidth = 3 * subCategoryWidth;
					v10 = helpHeight - 2 * spellIconHeight - ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL2].height_5 * scale);

					subCategoryPosX = offsetPosX + (((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL].width_4 * scale) >> 1)
						+ ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL].width_4 * scale) * spellIdxX
						+ ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[EDGE_PANEL].width_4 * scale)
						- (subCategoryTotalWidth >> 1);

					posY = v10 + (18 * scale);

					if (((640 * scale) + offsetPosX) - subCategoryTotalWidth >= subCategoryPosX)
					{
						//TEST IT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
						if ((subCategoryPosX - offsetPosX) < 0)
							subCategoryPosX = offsetPosX;
					}
					else
					{
						subCategoryPosX = ((640 * scale) + offsetPosX) - subCategoryTotalWidth;
					}

					//Calucation Spell Sub Category position
					subCategoryIndex = a1x->dword_0x3E6_2BE4_12228.str_611.array_0x437_1079x.SpellIndex[spellIndex_D94FF[a1x->dword_0x3E6_2BE4_12228.str_611.spellIndex_0x458_1112]];

					posX = posX + (subCategoryWidth * subCategoryIndex
						+ (subCategoryWidth / 2)
						+ (8 * scale)
						+ (subCategoryPosX));

					if (unk_18058Cstr.x_WORD_1805C2_joystick != 7 && unk_18058Cstr.x_WORD_1805C2_joystick != 1 && unk_18058Cstr.x_WORD_1805C2_joystick != 2)
					{
						unk_18058Cstr.x_DWORD_1805B0_mouse.x = (signed __int16)posX;
						unk_18058Cstr.x_DWORD_1805B0_mouse.y = (signed __int16)posY;
					}
					else
					{
						SetMousePositionInMemory_5BDC0(posX, posY);
					}
				}
			}
			else
			{
				posX = offsetPosX + ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL].width_4 * scale) * spellIdxX
					+ ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[EDGE_PANEL].width_4 * scale)
					+ (((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL].width_4 * scale) / 2);

				posY = (spellIconHeight / 2)
					+ helpHeight
					- 2 * spellIconHeight
					+ spellIconHeight * (a1x->dword_0x3E6_2BE4_12228.str_611.spellIndex_0x458_1112 >= 13);

				if (unk_18058Cstr.x_WORD_1805C2_joystick != 7 && unk_18058Cstr.x_WORD_1805C2_joystick != 1 && unk_18058Cstr.x_WORD_1805C2_joystick != 2)
				{
					unk_18058Cstr.x_DWORD_1805B0_mouse.x = (signed __int16)posX;
					unk_18058Cstr.x_DWORD_1805B0_mouse.y = (signed __int16)posY;
				}
				else
				{
					SetMousePositionInMemory_5BDC0(posX, posY);
				}
			}
		}
	}
}

//----- (00010010) --------------------------------------------------------
signed __int16 sub_10010()
{
	__int16 v0; // bx
	__int16 v1; // cx
	__int16 v2; // [esp+0h] [ebp-Ch]
	__int16 v3; // [esp+2h] [ebp-Ah]
	int v4; // [esp+4h] [ebp-8h]
	int v5; // [esp+8h] [ebp-4h]

	// fix if begin
	v1 = 0;
	v3 = 0;
	v4 = 0;
	// end

	if (x_BYTE_D3F48)
		return 1;
#ifdef _MSC_VER
	dos_getdrive(&v5);
	if (!dos_getdiskfree(v1, 0, v5, &v2) && v2 == -1 && !v3 && v4 == 134217729)
	{
		v0 = v4;
		x_BYTE_D3F48 = 1;
	}
	return v0;
#else
	x_BYTE_D3F48 = 1;
	return 0;
#endif
}

//----- (000889F0) --------------------------------------------------------
void sub_889F0(uint16_t a1)//2699f0
{
	if (a1 < 0x15u)
	{
		if (a1 < 0x12u)
		{
			if (a1 == 15)
			{
				str_E2A74[16].byte_29 = 6;
				return;
			}
			goto LABEL_22;
		}
		if (a1 > 0x12u)
		{
			if (a1 != 19)
				goto LABEL_22;
			str_unk_1804B0ar.PopupStatusByte_0x9e |= 0x80;
			return;
		}
		str_unk_1804B0ar.PopupStatusByte_0x9e |= 0x40;
		return;
	}
	if (a1 <= 0x15u)
	{
		str_unk_1804B0ar.PopupStatusByte_0x9e |= 0x40;
		return;
	}
	if (a1 < 0x1Cu)
	{
		if (a1 < 0x19u || a1 > 0x1Au)
			goto LABEL_22;
		{
			str_unk_1804B0ar.PopupStatusByte_0x9e |= 0x80;
			return;
		}
	}
	if (a1 <= 0x1Cu)
	{
		str_unk_1804B0ar.PopupStatusByte_0x9e |= 0x80;
		return;
	}
	if (a1 < 0x52u)
	{
	LABEL_22:
		if (str_E2A74[a1].word_0 == 7)
		{
			switch (D41A0_0.stages_0x3654C[D41A0_0.struct_0x3659C[D41A0_0.LevelIndex_0xc].substr_3659C.ObjectiveText_1].stages_3654C_byte0)
			{
			case 0:
				str_unk_1804B0ar.PopupStatusByte_0x9e |= 0x40;
				break;
			case 1:
			case 2:
			case 3:
			case 7:
			case 8:
			case 9:
				str_unk_1804B0ar.PopupStatusByte_0x9e |= 0x80;
				break;
			default:
				break;
			}
		}
		if (a1 == 79)
		{
			str_E2A74[2].byte_29 = 6;
			str_E2A74[81].byte_29 = 6;
		}
		return;
	}
	if (a1 <= 0x52u)
	{
		str_unk_1804B0ar.PopupStatusByte_0x9e |= 0x80;
		return;
	}
	if (a1 != 93)
		goto LABEL_22;
	if (Entities_EA3E4[D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240]->dword_0xA4_164x->CastleEntityIndex_0x3A_58)
		str_unk_1804B0ar.word_0x88 = 94;
}

//----- (0005BDC0) --------------------------------------------------------
void SetMousePositionInMemory_5BDC0(int16_t posX, int16_t posY)//23cdc0
{
	if (CommandLineParams.DoShowDebugPerifery())ShowPerifery();

	if (posX < 0)
		posX = 0;
	if (posY < 0)
		posY = 0;

	x_WORD_E375C_mouse_position_x = posX;
	x_WORD_E3760_mouse.x = posX;
	x_WORD_E375E_mouse_position_y = posY;
	x_WORD_E3760_mouse.y = posY;
	unk_18058Cstr.x_DWORD_1805B8_mouse_position_x = posX;
	unk_18058Cstr.x_DWORD_1805B0_mouse.x = posX;
	unk_18058Cstr.x_DWORD_1805BC_mouse_position_y = posY;
	unk_18058Cstr.x_DWORD_1805B0_mouse.y = posY;
	if (*(uint32_t*)&xx_array_E36C4[0])
	{
		*(uint32_t*)&xx_array_E36C4[40] = posY;
		*(uint32_t*)&xx_array_E36C4[36] = unk_18058Cstr.x_DWORD_1805B0_mouse.x;
	}
	if (x_WORD_180660_VGA_type_resolution & 8)
	{
		//v2 *= 8;
		//v3 *= 8;
	}
	else
	{
		//v2 = (float)v2 / 320 * 640;
		//v3 = (float)v3 / 200 * 480;
	}

	VGA_Set_mouse(posX, posY);
}

//----- (0001A5B0) --------------------------------------------------------
void sub_1A5B0_getLangStrings(int a1, int* a2, int* a3)//1fb5b0
{
	int result; // eax

	switch (a1)
	{
	case 1:
		*a2 = 400;//Help Mode On
		switch (a1)
		{
		case 6:
		case 16:
			goto LABEL_20;
		case 7:
			goto LABEL_21;
		case 14:
			goto LABEL_22;
		default:
			goto LABEL_23;
		}
		goto LABEL_23;
	case 2:
		*a2 = 390;//Sound On
		switch (a1)
		{
		case 6:
		case 16:
			goto LABEL_20;
		case 7:
			goto LABEL_21;
		case 14:
			goto LABEL_22;
		default:
			goto LABEL_23;
		}
		goto LABEL_23;
	case 3:
		*a2 = 392;//Music On
		switch (a1)
		{
		case 6:
		case 16:
			goto LABEL_20;
		case 7:
			goto LABEL_21;
		case 14:
			goto LABEL_22;
		default:
			goto LABEL_23;
		}
		goto LABEL_23;
	case 4:
		*a2 = 469;//Speech On
		switch (a1)
		{
		case 6:
		case 16:
			goto LABEL_20;
		case 7:
			goto LABEL_21;
		case 14:
			goto LABEL_22;
		default:
			goto LABEL_23;
		}
		goto LABEL_23;
	case 5:
		*a2 = 398;//Flight Assistance On
		switch (a1)
		{
		case 6:
		case 16:
			goto LABEL_20;
		case 7:
			goto LABEL_21;
		case 14:
			goto LABEL_22;
		default:
			goto LABEL_23;
		}
		goto LABEL_23;
	case 6:
		*a2 = 7;//Brightness correction
		switch (a1)
		{
		case 6:
		case 16:
			goto LABEL_20;
		case 7:
			goto LABEL_21;
		case 14:
			goto LABEL_22;
		default:
			goto LABEL_23;
		}
		goto LABEL_23;
	case 7:
		*a2 = 382;//speed fast
		switch (a1)
		{
		case 6:
		case 16:
			goto LABEL_20;
		case 7:
			goto LABEL_21;
		case 14:
			goto LABEL_22;
		default:
			goto LABEL_23;
		}
		goto LABEL_23;
	case 8:
		*a2 = 13;//Alter screen size
		switch (a1)
		{
		case 6:
		case 16:
			goto LABEL_20;
		case 7:
			goto LABEL_21;
		case 14:
			goto LABEL_22;
		default:
			goto LABEL_23;
		}
		goto LABEL_23;
	case 9:
		*a2 = 359;//Reflections On
		switch (a1)
		{
		case 6:
		case 16:
			goto LABEL_20;
		case 7:
			goto LABEL_21;
		case 14:
			goto LABEL_22;
		default:
			goto LABEL_23;
		}
		goto LABEL_23;
	case 10:
		*a2 = 363;//Sky On
		switch (a1)
		{
		case 6:
		case 16:
			goto LABEL_20;
		case 7:
			goto LABEL_21;
		case 14:
			goto LABEL_22;
		default:
			goto LABEL_23;
		}
		goto LABEL_23;
	case 11:
		*a2 = 357;//Stereo Mode On
		switch (a1)
		{
		case 6:
		case 16:
			goto LABEL_20;
		case 7:
			goto LABEL_21;
		case 14:
			goto LABEL_22;
		default:
			goto LABEL_23;
		}
		goto LABEL_23;
	case 12:
		*a2 = 395;//Light Sources On
		switch (a1)
		{
		case 6:
		case 16:
			goto LABEL_20;
		case 7:
			goto LABEL_21;
		case 14:
			goto LABEL_22;
		default:
			goto LABEL_23;
		}
		goto LABEL_23;
	case 13:
		*a2 = 365;//Icons and Map On
		switch (a1)
		{
		case 6:
		case 16:
			goto LABEL_20;
		case 7:
			goto LABEL_21;
		case 14:
			goto LABEL_22;
		default:
			goto LABEL_23;
		}
		goto LABEL_23;
	case 14:
		*a2 = 441;//Panel Transparency Off
		switch (a1)
		{
		case 6:
		case 16:
			goto LABEL_20;
		case 7:
			goto LABEL_21;
		case 14:
			goto LABEL_22;
		default:
			goto LABEL_23;
		}
		goto LABEL_23;
	case 15:
		*a2 = 442;//Flat Shading On
		switch (a1)
		{
		case 6:
		case 16:
			goto LABEL_20;
		case 7:
			goto LABEL_21;
		case 14:
			goto LABEL_22;
		default:
			goto LABEL_23;
		}
		goto LABEL_23;
	case 16:
		*a2 = 468;//Change screen resolution
		switch (a1)
		{
		case 6:
		case 16:
			goto LABEL_20;
		case 7:
			goto LABEL_21;
		case 14:
			goto LABEL_22;
		default:
			break;
		}
	LABEL_23:
		result = *a2 + 1;
		*a3 = result;
		return;
	case 17:
		*a2 = 402;//Player Names On
		break;
	default:
		break;
	}
	switch (a1)
	{
	case 6:
	case 16:
	LABEL_20:
		result = *a2;
		*a3 = *a2;
		break;
	case 7:
	LABEL_21:
		*a3 = 381;//speed normal
		break;
	case 14:
	LABEL_22:
		result = *a2 - 1;
		*a3 = result;
		break;
	default:
		goto LABEL_23;
	}
}

//----- (00030BE0) --------------------------------------------------------
void GetOkayCancelButtonPositions_30BE0(int16_t* ptrX, int16_t* ptrY, uint8_t scale)//211be0
{
	*ptrX = 270;
	*ptrY = 104;
	if (x_WORD_180660_VGA_type_resolution != 1)
		if (!DefaultResolutions())
		{
			*ptrX = (screenWidth_18062C / 2) - (50 * scale);
			*ptrY = (screenHeight_180624 / 2) - (32 * scale);
	}
}

//----- (00030A60) --------------------------------------------------------
void DrawOkCancelMenu_30A60(int16_t posTextX, int16_t posTextY, uint8_t scale)//211a60
{
	__int64 v4; // rax
	int16_t posX;
	int16_t posY;

	if (x_D41A0_BYTEARRAY_4_struct.SelectedMenuItem_38546)
	{
		switch (x_D41A0_BYTEARRAY_4_struct.SelectedMenuItem_38546)
		{
			case 1:
			{
				sprintf(printbuffer, "%s?", x_DWORD_E9C4C_langindexbuffer[2]);//Abandon level.
				break;
			}
			case 2:
			{
				sprintf(printbuffer, "%s?", x_DWORD_E9C4C_langindexbuffer[423]);//Load Level
				break;
			}
			case 3:
			{
				sprintf(printbuffer, "%s?", x_DWORD_E9C4C_langindexbuffer[424]);//Save Level
				break;
			}
		}

		v4 = 4080 * x_D41A0_BYTEARRAY_4_struct.colorIndex_121[4];
		DrawText_2BC10(printbuffer, posTextX * scale, posTextY * scale, (*xadataclrd0dat.colorPalette_var28)[256 * ((signed int)(v4 - (__CFSHL__(HIDWORD(v4), 8) + (HIDWORD(v4) << 8))) >> 8)], scale);
		GetOkayCancelButtonPositions_30BE0(&posX, &posY, scale);
		DrawBitmap_2BB40(posX, posY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BUTTON_OK1], scale);
		DrawBitmap_2BB40(posX + (50 * scale), posY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BUTTON_CANCEL1], scale);
		if (unk_18058Cstr.x_WORD_1805C2_joystick == 8
			|| unk_18058Cstr.x_WORD_1805C2_joystick == 12
			|| unk_18058Cstr.x_WORD_1805C2_joystick == 13
			|| unk_18058Cstr.x_WORD_1805C2_joystick == 4
			|| unk_18058Cstr.x_WORD_1805C2_joystick == 6
			|| unk_18058Cstr.x_WORD_1805C2_joystick == 11
			|| unk_18058Cstr.x_WORD_1805C2_joystick == 5)
		{
			DrawBitmap_2BB40(unk_18058Cstr.x_DWORD_1805B0_mouse.x, unk_18058Cstr.x_DWORD_1805B0_mouse.y, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[CURSOR_SPRITE_INDEX_D419E], scale);
		}
	}
	EventDispatcher::I->DispatchEvent(EventType::E_SCENE_CHANGE, Scene::FLIGHT_MENU);
}

//----- (00052D70) --------------------------------------------------------
void ShowMessage_52D70(uint16_t playerIndex, char* cheatMessage)//233d70
{
	strcpy(D41A0_0.array_0x2BDE[playerIndex].CurrentNotificationText_0x01c_2BFA_11258, cheatMessage);
	D41A0_0.array_0x2BDE[playerIndex].word_0x04d_2C2B_11307 = 100;
	D41A0_0.array_0x2BDE[playerIndex].word_0x04f_2C2D_11309 = 1;
}

//----- (0002F6B0) --------------------------------------------------------
void DrawChatMenu_2F6B0()//2106b0
{
	signed int v0; // eax
	//int v1; // eax
	unsigned __int8 v2; // dl
	char v3; // dh
	__int16 v4; // bx
	unsigned __int8 v5; // di
	unsigned __int8 v6; // al
	__int16 v7; // si
	int v8; // ebx
	__int16 v9; // di
	unsigned int v10; // ebx
	__int16 v11; // si
	unsigned __int8 v12; // al
	__int16 v13; // ST04_2
	__int16 v14; // ST00_2
	__int16 v15; // si
	__int16 v16; // bx
	__int16 v17; // si
	__int16 v18; // bx
	bitmap_pos_struct_t v19; // eax
	int v20; // ebx
	__int16 v21; // si
	bitmap_pos_struct_t v22; // eax
	signed int v23; // eax
	int v24; // ebx
	int v25; // esi
	char v26; // al
	bitmap_pos_struct_t v27; // eax
	__int16 v28; // si
	__int16 v29; // bx
	__int16 v30; // si
	unsigned __int8 v31; // di
	//int result; // eax
	//char v33; // [esp+0h] [ebp-Eh]
	int v34; // [esp+48h] [ebp+3Ah]
	int v35; // [esp+4Ch] [ebp+3Eh]
	int v36; // [esp+50h] [ebp+42h]
	int v37; // [esp+54h] [ebp+46h]
	int v38; // [esp+58h] [ebp+4Ah]
	int v39; // [esp+5Ch] [ebp+4Eh]
	int v40; // [esp+60h] [ebp+52h]
	//x_BYTE *v41; // [esp+64h] [ebp+56h]
	unsigned __int8 v42; // [esp+68h] [ebp+5Ah]
	unsigned __int8 v43; // [esp+6Ch] [ebp+5Eh]
	unsigned __int8 v44; // [esp+70h] [ebp+62h]
	unsigned __int8 v45; // [esp+74h] [ebp+66h]
	unsigned __int8 v46; // [esp+78h] [ebp+6Ah]
	unsigned __int8 v47; // [esp+7Ch] [ebp+6Eh]
	unsigned __int8 v48; // [esp+80h] [ebp+72h]
	unsigned __int8 v49; // [esp+84h] [ebp+76h]
	bool v50; // [esp+88h] [ebp+7Ah]
	char v51; // [esp+8Ch] [ebp+7Eh]

	//v41 = (x_BYTE *)(2124 * D41A0_BYTESTR_0.word_0xc + x_D41A0_BYTEARRAY_0 + 11230);
	if (x_WORD_180660_VGA_type_resolution & 1)
		v0 = 400;
	else
		v0 = screenHeight_180624;
	v40 = v0 - 72;
	v36 = 0;
	//v1 = 5 * static_cast<std::underlying_type<MapType_t>::type>(D41A0_BYTESTR_0.terrain_2FECE.MapType);
	v2 = str_D94F0_bldgprmbuffer[static_cast<std::underlying_type<MapType_t>::type>(D41A0_0.terrain_2FECE.MapType)][0];
	v47 = str_D94F0_bldgprmbuffer[static_cast<std::underlying_type<MapType_t>::type>(D41A0_0.terrain_2FECE.MapType)][1];
	v3 = str_D94F0_bldgprmbuffer[static_cast<std::underlying_type<MapType_t>::type>(D41A0_0.terrain_2FECE.MapType)][2];
	v48 = str_D94F0_bldgprmbuffer[static_cast<std::underlying_type<MapType_t>::type>(D41A0_0.terrain_2FECE.MapType)][3];
	v42 = (*xadataclrd0dat.colorPalette_var28)[4095];
	v44 = (*xadataclrd0dat.colorPalette_var28)[0];
	v49 = v3;
	v45 = (*xadataclrd0dat.colorPalette_var28)[240];
	v43 = (*xadataclrd0dat.colorPalette_var28)[3840];
	v4 = v40;
	v51 = -1;
	DrawLine_2BC80(0, v40, 640, 72, v2);
	v34 = v49;
	DrawLine_2BC80(0, v4, 640, 2, v49);
	v5 = v48;
	DrawLine_2BC80(0, v40 + 70, 640, 2, v48);
	DrawLine_2BC80(0, v4, 2, 70, v34);
	DrawLine_2BC80(v36 + 638, v4, 2, 72, v5);
	if (x_WORD_E3760_mouse.x < 4
		|| x_WORD_E3760_mouse.x >= 66
		|| x_WORD_E3760_mouse.y < v40 + 28
		|| x_WORD_E3760_mouse.y >= v40 + 66)
	{
		v6 = D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].byte_0x3E0_2BE4_12222;
	}
	else
	{
		v6 = 4 * ((x_WORD_E3760_mouse.y - (v40 + 28)) / 20)
			+ ((x_WORD_E3760_mouse.x
				- 4
				- (__CFSHL__((x_WORD_E3760_mouse.x - 4) >> 31, 4)
					+ 16 * ((x_WORD_E3760_mouse.x - 4) >> 31))) >> 4);
	}
	v46 = v6;
	v7 = v40 + 4;
	v8 = v36 + 4;
	v9 = v36 + 4;
	v34 = (signed __int16)(v40 + 4);
	DrawLine_2BC80(v36 + 4, v40 + 4, 372, 20, v47);
	v35 = v48;
	DrawLine_2BC80(v9, v34, 372, 2, v48);
	v38 = v49;
	DrawLine_2BC80(v9, v7 + 18, 372, 2, v49);
	DrawLine_2BC80(v9, v34, 2, 18, v35);
	DrawLine_2BC80(v8 + 370, v34, 2, 20, v38);
	sprintf(printbuffer, "%s", D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].names_81[v46]);
	v7 += 2;
	v8 += 6;
	DrawText_2BC10(printbuffer, v8, v7, v42);
	v10 = 8 * strlen(printbuffer) + v8;
	v11 = v7 + 2;
	if (x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26 & 8)
		v12 = v43;
	else
		v12 = v47;
	v13 = v11;
	v39 = 0;
	v14 = v10;
	v15 = v40;
	v16 = v36;
	DrawLine_2BC80(v14, v13, 6, 12, v12);
	v17 = v15 + 28;
	v18 = v16 + 4;
	while (v39 < 8)
	{
		if (D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].byte_0x3E0_2BE4_12222 == v39)
			v19 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_MINISQUARE1_WITH_FRAME];//930
		else
			v19 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_MINISQUARE1];//948
		DrawBitmap_2BB40(v18, v17, v19);
		sprintf(printbuffer, "%d", v39 + 1);
		DrawText_2BC10(printbuffer, v18 + 4, v17 + 2, v44);
		if (++v39 == 4)
		{
			v18 = 4;
			v17 += 20;
		}
		else
		{
			v18 += 16;
		}
	}
	v39 = 0;
	v20 = v36 + 82;
	v21 = v40 + 32;
	while (v39 < 4)
	{
		if (D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].byte_0x3E1_2BE4_12223 == v39)
			v22 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL_WITH_FRAME];
		else
			v22 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL];
		DrawBitmap_2BB40(v20, v21, v22);
		switch (v39)
		{
		case 0:
			v37 = 154;
			break;
		case 1:
			v37 = 153;
			break;
		case 2:
			v37 = 152;
			break;
		case 3:
			v37 = 151;
			break;
		default:
			break;
		}
		//DrawBitmap_2BB40(v20 + 2, v21 + 2, (uint8_t**)(6 * v37 + **filearray_2aa18c[6]));
		DrawBitmap_2BB40(v20 + 2, v21 + 2, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v37]);
		if (++v39 == 3)
			v23 = 20;
		else
			v23 = 2;
		v20 += v23 + 68;
	}
	v50 = D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].byte_0x3E1_2BE4_12223 != 3;
	v39 = 0;
	v24 = v36 + 384;
	v25 = v40 + 26;
	while (v39 < 8)
	{
		if (D41A0_0.LevelIndex_0xc == v39)
			++v39;
		v26 = 1;
		if (!v50 && (1 << v39) & D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].byte_0x3E3_2BE4_12225)
			v26 = 0;
		/*if ( v26 )
		  v27 = **filearray_2aa18c[6] + 960;
		else
		  v27 = **filearray_2aa18c[6] + 942;
		DrawBitmap_2BB40(v24, v25, (uint8_t**)v27);
		DrawBitmap_2BB40(v24 + 2, v25 + 2, (uint8_t**)(6 * (v39 + 74) + **filearray_2aa18c[6]));*/
		if (v26)
			v27 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL_SMALL];
		else
			v27 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL_SMALL_WITH_FRAME];
		DrawBitmap_2BB40(v24, v25, v27);
		DrawBitmap_2BB40(v24 + 2, v25 + 2, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v39 + INCHAT_PLAYER1]);
		if (x_WORD_E3760_mouse.x >= v24
			&& x_WORD_E3760_mouse.x < v24 + 32
			&& x_WORD_E3760_mouse.y >= v25
			&& x_WORD_E3760_mouse.y < v25 + 22)
		{
			v51 = v39;
		}
		v24 += 34;
		v39++;
	}
	v28 = v40 + 4;
	v29 = v36 + 384;
	/*DrawBitmap_2BB40(v36 + 384, v40 + 4, (uint8_t**)(**filearray_2aa18c[6] + 1554));
	DrawBitmap_2BB40(v36 + 474, v28, (uint8_t**)(**filearray_2aa18c[6] + 1566));
	DrawBitmap_2BB40(v36 + 564, v28, (uint8_t**)(**filearray_2aa18c[6] + 1560));*/
	DrawBitmap_2BB40(v36 + 384, v40 + 4, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BUTTON_OK2]);
	DrawBitmap_2BB40(v36 + 474, v28, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BUTTON_BACK]);
	DrawBitmap_2BB40(v36 + 564, v28, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BUTTON_CANCEL2]);
	v30 = v40 + 50;
	DrawLine_2BC80(v29, v40 + 50, 236, 18, v47);
	v31 = v48;
	DrawLine_2BC80(v29, v30, 236, 2, v48);
	v38 = v49;
	DrawLine_2BC80(v29, v40 + 66, 236, 2, v49);
	DrawLine_2BC80(v29, v30, 2, 16, v31);
	DrawLine_2BC80(v36 + 618, v30, 2, 18, v38);
	//result = v51;
	if (v51 != -1)
		//v51
		DrawText_2BC10(D41A0_0.array_0x2BDE[v51].WizardName_0x39f_2BFA_12157, v36 + 390, v40 + 52, v45);
	if (unk_18058Cstr.x_WORD_1805C2_joystick == 8
		|| unk_18058Cstr.x_WORD_1805C2_joystick == 12
		|| unk_18058Cstr.x_WORD_1805C2_joystick == 13
		|| unk_18058Cstr.x_WORD_1805C2_joystick == 4
		|| unk_18058Cstr.x_WORD_1805C2_joystick == 6
		|| unk_18058Cstr.x_WORD_1805C2_joystick == 11
		|| unk_18058Cstr.x_WORD_1805C2_joystick == 5)
	{
		//result = DrawBitmap_2BB40(x_DWORD_1805B0_mouse.x, x_DWORD_1805B0_mouse.y, (uint8_t**)(**filearray_2aa18c[0] + 6 * (unsigned __int8)CURSOR_SPRITE_INDEX_D419E));
		DrawBitmap_2BB40(unk_18058Cstr.x_DWORD_1805B0_mouse.x, unk_18058Cstr.x_DWORD_1805B0_mouse.y, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[CURSOR_SPRITE_INDEX_D419E]);
	}
	//return result;
	EventDispatcher::I->DispatchEvent(EventType::E_SCENE_CHANGE, Scene::CHAT_MENU);
}
// 8E3D5: using guessed type x_DWORD sprintf(x_DWORD, const char *, ...);
// D419E: using guessed type char CURSOR_SPRITE_INDEX_D419E;
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// E3760: using guessed type int x_DWORD_E3760;
// E89F0: using guessed type char x_BYTE_E89F0;
// E9800: using guessed type char x_BYTE_E9800;
// E98FF: using guessed type char x_BYTE_E98FF;
// EA3DC: using guessed type int **filearray_2aa18c[6];
// EB394: using guessed type int **filearray_2aa18c[0];
// 1805B0: using guessed type int x_DWORD_1805B0_mouse.x;
// 1805B4: using guessed type int x_DWORD_1805B0_mouse.y;
// 1805C2: using guessed type __int16 x_WORD_1805C2_joystick;
// 180660: using guessed type __int16 x_WORD_180660_VGA_type_resolution;

//----- (0002FD90) --------------------------------------------------------
void DrawPauseMenu_2FD90(uint8_t scale)//210d90
{
	signed int v1; // ebx
	signed int v2; // ebx
	signed int v3; // ebx
	int16_t heigth; // [esp+0h] [ebp-14h]
	int16_t width; // [esp+4h] [ebp-10h]
	int16_t posX; // [esp+8h] [ebp-Ch]
	int16_t posY; // [esp+Ch] [ebp-8h]
	uint8_t colour; // [esp+10h] [ebp-4h]

	colour = (*xadataclrd0dat.colorPalette_var28)[0];
	if (x_D41A0_BYTEARRAY_4_struct.OptionsSettingFlag_24 & GAME_PAUSED)
	{
		if (!x_D41A0_BYTEARRAY_4_struct.byteindex_206)
		{
			GetPauseMenuCoordinates_2FFE0(&posX, &posY, &width, &heigth, scale);
			DrawBitmap_2BB40(posX, posY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[MENU_ICON_SAVE], scale);//Save Dialog
			posY += heigth;
			DrawBitmap_2BB40(posX, posY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[MENU_ICON_LOAD], scale);//Load Dialog
			if (!x_D41A0_BYTEARRAY_4_struct.byteindex_208)
			{
				v1 = 0;
				while (v1 < 2)
				{
					v1++;
					ColorizeScreen_2E790(posX, posY, width - 2, heigth, colour);//Shade Load Dialog
				}
			}
			posY += heigth;
			DrawBitmap_2BB40(posX, posY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SOUND_ICON], scale);//Sound button
			if (!soundAble_E3798)
			{
				v2 = 0;
				while (v2 < 2)
				{
					v2++;
					ColorizeScreen_2E790(
						posX,
						posY,
						(*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SOUND_ICON].width_4 * scale,
						(*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SOUND_ICON].height_5 * scale,
						colour);
				}
			}
			DrawBitmap_2BB40(posX + ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[MUSIC_ICON].width_4 * scale), posY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[MUSIC_ICON], scale);//Music button
			if (!musicAble_E37FC)
			{
				v3 = 0;
				while (v3 < 2)
				{
					v3++;
					ColorizeScreen_2E790(
						posX + ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[MUSIC_ICON].width_4 * scale),
						posY,
						(*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[MUSIC_ICON].width_4 * scale,
						(*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[MUSIC_ICON].height_5 * scale,
						colour);
				}
			}
			posY += heigth;
			DrawBitmap_2BB40(posX, posY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SHOW_ICON], scale);//Settings button
			EventDispatcher::I->DispatchEvent(EventType::E_SCENE_CHANGE, Scene::FLIGHT_MENU);
		}
		if (unk_18058Cstr.x_WORD_1805C2_joystick == 8
			|| unk_18058Cstr.x_WORD_1805C2_joystick == 12
			|| unk_18058Cstr.x_WORD_1805C2_joystick == 13
			|| unk_18058Cstr.x_WORD_1805C2_joystick == 4
			|| unk_18058Cstr.x_WORD_1805C2_joystick == 6
			|| unk_18058Cstr.x_WORD_1805C2_joystick == 11
			|| unk_18058Cstr.x_WORD_1805C2_joystick == 5)
		{
			DrawBitmap_2BB40(unk_18058Cstr.x_DWORD_1805B0_mouse.x, unk_18058Cstr.x_DWORD_1805B0_mouse.y, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[CURSOR_SPRITE_INDEX_D419E], scale);
		}
		if (x_D41A0_BYTEARRAY_4_struct.setting_byte4_25 & 0x10)
			sub_30870();
	}

}

//----- (00030050) --------------------------------------------------------
void DrawInGameOptionsMenu_30050(uint8_t scale)//211050
{
	int v1; // edx
	int v3; // eax
	int v4; // eax
	int v5; // ecx
	int v6; // edi
	uint8_t textColour; // dl
	int v9; // esi
	int v10; // edi
	unsigned __int8 v12; // dl
	//int result; // eax
	int v14; // [esp+0h] [ebp-20h]
	int v15; // [esp+4h] [ebp-1Ch]
	int v16; // [esp+8h] [ebp-18h]
	int v17; // [esp+Ch] [ebp-14h]
	char* v18; // [esp+10h] [ebp-10h]
	int v19; // [esp+14h] [ebp-Ch]
	int v20; // [esp+18h] [ebp-8h]
	int v21; // [esp+1Ch] [ebp-4h]
	int okayBtnPosY = 377;
	int menuBtnPosY = 67;

	//fix
	v9 = 0;
	v10 = 0;
	//fix
	v1 = 0;
	
	int optionMenuXPos = (640 - x_D41A0_BYTEARRAY_4_struct.byteindex_186) / 2;
	if (x_WORD_180660_VGA_type_resolution != 1)
		if (!DefaultResolutions())
		{
			optionMenuXPos = (screenWidth_18062C - (x_D41A0_BYTEARRAY_4_struct.byteindex_186 * scale)) / 2;
		    menuBtnPosY = (screenHeight_180624 / 2) - ((OPTIONS_MENU_BTN_COUNT * (OPTIONS_MENU_BTN_HEIGHT * scale)) / 2);
			okayBtnPosY = menuBtnPosY + ((OPTIONS_MENU_BTN_HEIGHT * scale) * OPTIONS_MENU_BTN_COUNT) + (4 * scale);
		}

	while (1)
	{
		v21 = v1;
		if (v1 >= 17)
			break;
		sub_1A5B0_getLangStrings(v21 + 1, &v15, &v14);
		v19 = 1 << v21;
		if ((1 << v21) & x_D41A0_BYTEARRAY_4_struct.dwordindex_192 && (1 << v21) & x_D41A0_BYTEARRAY_4_struct.dwordindex_188)
			v3 = v15;
		else
			v3 = v14;
		v18 = (char*)x_DWORD_E9C4C_langindexbuffer[v3];
		//help mode On and etc
		v4 = x_D41A0_BYTEARRAY_4_struct.byteindex_186 * scale;
		/*v16 = v2 + *(unsigned __int8 *)(**filearray_2aa18c[6] + 1012);
			v5 = v2 + v4 - *(unsigned __int8 *)(**filearray_2aa18c[6] + 1024);
			v17 = *(unsigned __int8 *)(**filearray_2aa18c[6] + 1018);
			v20 = v5;
			DrawBitmap_2BB40(v2, v0, (uint8_t**)(**filearray_2aa18c[6] + 1008));
			v6 = v16;
			DrawBitmap_2BB40(v20, v0, (uint8_t**)(**filearray_2aa18c[6] + 1020));
			while ( v6 < v20 )
			{
			  DrawBitmap_2BB40(v6, v0, (uint8_t**)(**filearray_2aa18c[6] + 1014));
			  v6 += v17;
			}*/
		v16 = optionMenuXPos + ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BACKGROUND_NUMBER4].width_4 * scale);
		v5 = optionMenuXPos + v4 - ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BACKGROUND_NUMBER6].width_4 * scale);
		v17 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BACKGROUND_NUMBER5].width_4 * scale;
		v20 = v5;
		DrawBitmap_2BB40(optionMenuXPos, menuBtnPosY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BACKGROUND_NUMBER4], scale);
		v6 = v16;
		DrawBitmap_2BB40(v20, menuBtnPosY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BACKGROUND_NUMBER6], scale);
		while (v6 < v20)
		{
			DrawBitmap_2BB40(v6, menuBtnPosY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BACKGROUND_NUMBER5], scale);
			v6 += v17;
		}
		if (v19 & x_D41A0_BYTEARRAY_4_struct.dwordindex_192)
		{
			if (optionMenuXPos > unk_18058Cstr.x_DWORD_1805B0_mouse.x
				|| optionMenuXPos + (x_D41A0_BYTEARRAY_4_struct.byteindex_186 * scale) <= unk_18058Cstr.x_DWORD_1805B0_mouse.x
				|| menuBtnPosY > unk_18058Cstr.x_DWORD_1805B0_mouse.y
				|| menuBtnPosY + (OPTIONS_MENU_BTN_HEIGHT * scale) <= unk_18058Cstr.x_DWORD_1805B0_mouse.y)
			{
				textColour = (*xadataclrd0dat.colorPalette_var28)[4095];
			}
			else
			{
				textColour = (*xadataclrd0dat.colorPalette_var28)[3840];
			}
		}
		else
		{
			textColour = (*xadataclrd0dat.colorPalette_var28)[0x888];
		}
		DrawText_2BC10((char*)v18, optionMenuXPos + (((x_D41A0_BYTEARRAY_4_struct.byteindex_186 * scale) - (8 * scale) * strlen(v18)) >> 1), menuBtnPosY + (2 * scale), textColour, scale);
		menuBtnPosY += (OPTIONS_MENU_BTN_HEIGHT * scale);
		v1 = v21 + 1;
	}

	int okayBtnXPos = (x_D41A0_BYTEARRAY_4_struct.byteindex_186 - OPTIONS_MENU_BTN_WIDTH) / 2 + (640 - x_D41A0_BYTEARRAY_4_struct.byteindex_186) / 2;
	if (x_WORD_180660_VGA_type_resolution != 1)
		if (!DefaultResolutions())
		{
			okayBtnXPos = ((x_D41A0_BYTEARRAY_4_struct.byteindex_186 * scale) - (OPTIONS_MENU_BTN_WIDTH * scale)) / 2 + (screenWidth_18062C - (x_D41A0_BYTEARRAY_4_struct.byteindex_186 * scale)) / 2;
		}

	/*v16 = v8 + *(unsigned __int8 *)(**filearray_2aa18c[6] + 1012);
	v9 = *(unsigned __int8 *)(**filearray_2aa18c[6] + 1018);
	v20 = v8 + OPTIONS_MENU_BTN_WIDTH - *(unsigned __int8 *)(**filearray_2aa18c[6] + 1024);
	DrawBitmap_2BB40(v8, 377, (uint8_t**)(**filearray_2aa18c[6] + 1008));
	v10 = v16;
	DrawBitmap_2BB40(v20, 377, (uint8_t**)(**filearray_2aa18c[6] + 1020));
	while ( v10 < v20 )
	{
	  DrawBitmap_2BB40(v10, 377, (uint8_t**)(**filearray_2aa18c[6] + 1014));
	  v10 += v9;
	}*/
	v16 = okayBtnXPos + (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BACKGROUND_NUMBER4].width_4 * scale;
	v9 = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BACKGROUND_NUMBER5].width_4 * scale;
	v20 = okayBtnXPos + (OPTIONS_MENU_BTN_WIDTH * scale) - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BACKGROUND_NUMBER6].width_4 * scale;
	DrawBitmap_2BB40(okayBtnXPos, okayBtnPosY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BACKGROUND_NUMBER4], scale);
	v10 = v16;
	DrawBitmap_2BB40(v20, okayBtnPosY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BACKGROUND_NUMBER6], scale);
	while (v10 < v20)
	{
		DrawBitmap_2BB40(v10, okayBtnPosY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BACKGROUND_NUMBER5], scale);
		v10 += v9;
	}

	int okayBtnTextXPos = (640 - x_D41A0_BYTEARRAY_4_struct.byteindex_186) / 2 + (x_D41A0_BYTEARRAY_4_struct.byteindex_186 - OPTIONS_MENU_BTN_WIDTH) / 2;
	if (x_WORD_180660_VGA_type_resolution != 1)
		if (!DefaultResolutions())
		{
			okayBtnTextXPos = (screenWidth_18062C - (x_D41A0_BYTEARRAY_4_struct.byteindex_186 * scale)) / 2 + ((x_D41A0_BYTEARRAY_4_struct.byteindex_186 * scale) - (OPTIONS_MENU_BTN_WIDTH * scale)) / 2;
		}

	if (okayBtnTextXPos > unk_18058Cstr.x_DWORD_1805B0_mouse.x || okayBtnTextXPos + (OPTIONS_MENU_BTN_WIDTH * scale) <= unk_18058Cstr.x_DWORD_1805B0_mouse.x || unk_18058Cstr.x_DWORD_1805B0_mouse.y < okayBtnPosY || unk_18058Cstr.x_DWORD_1805B0_mouse.y >= (okayBtnPosY + (OPTIONS_MENU_BTN_HEIGHT * scale)))
		v12 = (*xadataclrd0dat.colorPalette_var28)[4095];
	else
		v12 = (*xadataclrd0dat.colorPalette_var28)[3840];
		
	if ((!DefaultResolutions())&& (x_WORD_180660_VGA_type_resolution != 1))
		DrawText_2BC10((char*)"OK", okayBtnTextXPos + (33 * scale), okayBtnPosY + (2 * scale), v12, scale);
	else
		DrawText_2BC10((char*)"OK", (640 - x_D41A0_BYTEARRAY_4_struct.byteindex_186) / 2 + (x_D41A0_BYTEARRAY_4_struct.byteindex_186 - OPTIONS_MENU_BTN_WIDTH) / 2 + 33, 379, v12);	

	EventDispatcher::I->DispatchEvent(EventType::E_SCENE_CHANGE, Scene::FLIGHT_MENU);

	if (unk_18058Cstr.x_WORD_1805C2_joystick == 8
		|| unk_18058Cstr.x_WORD_1805C2_joystick == 12
		|| unk_18058Cstr.x_WORD_1805C2_joystick == 13
		|| unk_18058Cstr.x_WORD_1805C2_joystick == 4
		|| unk_18058Cstr.x_WORD_1805C2_joystick == 6
		|| unk_18058Cstr.x_WORD_1805C2_joystick == 11
		|| unk_18058Cstr.x_WORD_1805C2_joystick == 5)
	{
		DrawBitmap_2BB40(unk_18058Cstr.x_DWORD_1805B0_mouse.x, unk_18058Cstr.x_DWORD_1805B0_mouse.y, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[CURSOR_SPRITE_INDEX_D419E], scale);
	}

}

//----- (000303D0) --------------------------------------------------------
void DrawVolumeSettings_303D0(uint8_t scale)//2113d0
{
	int16_t height;
	int16_t width;
	int16_t posY;
	int16_t posX;
	int index = 0;
	signed int volume = 0;

	unsigned __int8 color1 = str_D94F0_bldgprmbuffer[static_cast<std::underlying_type<MapType_t>::type>(D41A0_0.terrain_2FECE.MapType)][0];
	unsigned __int8 color2 = str_D94F0_bldgprmbuffer[static_cast<std::underlying_type<MapType_t>::type>(D41A0_0.terrain_2FECE.MapType)][2];
	unsigned __int8 color3 = str_D94F0_bldgprmbuffer[static_cast<std::underlying_type<MapType_t>::type>(D41A0_0.terrain_2FECE.MapType)][3];

	GetPauseMenuCoordinates_2FFE0(&posX, &posY, &width, &height, scale);
	switch (x_D41A0_BYTEARRAY_4_struct.byte_38591)
	{
	case 1:
		volume = 127;
		index = x_D41A0_BYTEARRAY_4_struct.soundVolume_6;
		break;
	case 2:
		volume = 127;
		index = x_D41A0_BYTEARRAY_4_struct.musicVolume_8;
		break;
	}
	width -= (2 * scale);
	posY += 4 * height;
	DrawLine_2BC80(posX, posY, width, (24 * scale), color1);
	DrawLine_2BC80(posX, posY, width, (2 * scale), color2);
	DrawLine_2BC80(posX, posY + (22 * scale), width, (2 * scale), color3);
	DrawLine_2BC80(posX, posY, (2 * scale), (22 * scale), color2);
	DrawLine_2BC80(width + posX - (2 * scale), posY, (2 * scale), (24 * scale), color3);
	DrawLine_2BC80(posX + (4 * scale), posY + (4 * scale), width - (8 * scale), (16 * scale), color1);
	DrawLine_2BC80(posX + (4 * scale), posY + (4 * scale), width - (8 * scale), (2 * scale), color3);
	DrawLine_2BC80(posX + (4 * scale), posY + (18 * scale), width - (8 * scale), (2 * scale), color2);
	DrawLine_2BC80(posX + (4 * scale), posY + (4 * scale), (2 * scale), (14 * scale), color3);
	DrawLine_2BC80(posX + (4 * scale) + width - (8 * scale) - (2 * scale), posY + (4 * scale), (2 * scale), (16 * scale), color2);
	posY += (6 * scale);
	posX += (6 * scale);
	DrawLine_2BC80(posX, posY, width - (12 * scale), (12 * scale), (*xadataclrd0dat.colorPalette_var28)[0]);
	if (volume > 0) {
		DrawLine_2BC80(posX, posY, (index * (width - (12 * scale))) / volume, (12 * scale), (*xadataclrd0dat.colorPalette_var28)[240]);
	}
}

//----- (00030870) --------------------------------------------------------
void sub_30870()//211870
{
	char* v0; // ebx
	__int16 v1; // ax
	//char v3; // [esp+0h] [ebp-32h]
	Type_TextBox_1804B0 textbox; // [esp+80h] [ebp+4Eh]
	//__int16 v5; // [esp+82h] [ebp+50h]//v4x[1]
	//__int16 v6; // [esp+8Ch] [ebp+5Ah]//v4x[6]
	//__int16 v7; // [esp+8Eh] [ebp+5Ch]//v4x[7]
	//__int16 v8; // [esp+90h] [ebp+5Eh]//v4x[8]
	//__int16 v9; // [esp+92h] [ebp+60h]//v4x[9]
	//__int16 v10; // [esp+94h] [ebp+62h]//v4x[10]
	//__int16 v11; // [esp+96h] [ebp+64h]//v4x[11]
	//__int16 v12; // [esp+98h] [ebp+66h]//v4x[12]
	//__int16 v13; // [esp+9Ah] [ebp+68h]//v4x[13]
	//char v14; // [esp+B0h] [ebp+7Eh]
	//char v15; // [esp+B1h] [ebp+7Fh]

	DrawLine_2BC80(4, 286, 632, 190, 0);
	sprintf(printbuffer, "%d", x_WORD_D41D4);
	DrawText_2BC10(printbuffer, 10, 300, (*xadataclrd0dat.colorPalette_var28)[4095]);
	textbox.maxTextboxHeight2_0xe = 400;
	textbox.minPosX_0x14 = 4;
	textbox.charWidth_0x10 = 8;
	textbox.charHeight_0x12 = 14;
	textbox.maxPosX_0x16 = 636;

	if (!DefaultResolutions())
	{
		textbox.maxPosX_0x16 = screenWidth_18062C - 4;
	}

	v0 = x_DWORD_E9C4C_langindexbuffer[x_WORD_D41D4];
	textbox.maxTextboxWidth2_0xc = 600;
	textbox.maxTextboxWidth_0x0 = 320;
	textbox.color1_0x30 = (*xadataclrd0dat.colorPalette_var28)[3840];
	textbox.color2_0x31 = (*xadataclrd0dat.colorPalette_var28)[0];
	textbox.minPosY_0x18 = 310;
	textbox.maxPosY_0x1a = 630;
	textbox.maxTextboxHeight_0x2 = 310;
	if (!DefaultResolutions())
	{
		textbox.maxPosX_0x16 = screenWidth_18062C - 40;
		textbox.maxTextboxWidth_0x0 = screenWidth_18062C / 2;
		textbox.maxTextboxHeight_0x2 = screenHeight_180624 / 2 - 90;
	}
	ComputeTextboxSizesFromTextWords_89420(&textbox, v0);
	ConstrainTextboxSizes_89520(&textbox);
	ComputeFrameSizes_89980(&textbox);
	DrawTextboxText_895D0(&textbox, v0);
	switch (LastPressedKey_1806E4)
	{
	case 0x48:
		if (++x_WORD_D41D4 < 1)
			x_WORD_D41D4 = 1;
		if (x_WORD_D41D4 > 470)
			x_WORD_D41D4 = 470;
		LastPressedKey_1806E4 = 0;
		break;
	case 0x50:
		v1 = x_WORD_D41D4 - 1;
		x_WORD_D41D4 = v1;
		if (v1 < 1)
			x_WORD_D41D4 = 1;
		if (x_WORD_D41D4 > 470)
			x_WORD_D41D4 = 470;
		LastPressedKey_1806E4 = 0;
		break;
	case 0x4b:
		x_WORD_D41D4 = 1;
		LastPressedKey_1806E4 = 0;
		break;
	case 0x4d:
		x_WORD_D41D4 = 470;
		//		LOBYTE(v1) = 0;
		LastPressedKey_1806E4 = 0;
		break;
	}
	//	return v1;
}
