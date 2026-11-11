#include "GameUI.h"

#include <array>

#include "../shared/EventDispatcher.h"
#include "Basic.h"
#include "GameBitmapIndexes.h"
#include "LangTextIndexes.h"
#include "Player.h"
#include "PlayerInput.h"
#include "Terrain.h"
#include "MenusAndIntros.h"
#include "Type_D93C0_Bldgprmbuffer.h"
#include "MapColourIndexs.h"
#include "read_config.h"
#include "EventsFunctions.h"
#include "CommandLineParser.h"
#include "Graphics.h"
#include "Network.h"
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


char redrawFrame_D41C4 = 99; // weak

char oldViewPortSize_D41C6 = 40; // weak

int x_DWORD_D41C8 = 0; // weak

char* creditsDesignedBy_2A2144 = (char*)"Designed By";

char* creditsBullfrogProductionsLtd_2A2150 = (char*)"Bullfrog Productions Ltd";

char* creditsSeparator_2A216C = (char*)"!";

char* creditsProjectLeader_2A2170 = (char*)"Project Leader";

char* creditsSpace_2A2180 = (char*)" ";

char* creditsAlanWright_2A2184 = (char*)"Alan Wright";

char* creditsEngineProgramming_2A2190 = (char*)"Engine Programming";

char* creditsGlennCorpes_2A21A4 = (char*)"Glenn Corpes";

char* creditsLeadProgramming_2A21B4 = (char*)"Lead Programming";

char* creditsProgramming_2A21C8 = (char*)"Programming";

char* creditsMarkLamport_2A21D4 = (char*)"Mark Lamport";

char* creditsMarkAdami_2A21E4 = (char*)"Mark Adami";

char* creditsLeadArtist_2A21F0 = (char*)"Lead Artist";

char* creditsEoinRogan_2A21FC = (char*)"Eoin Rogan";

char* creditsGraphicsAndArt_2A2208 = (char*)"Graphics and Art";

char* creditsDarrenThomas_2A221C = (char*)"Darren Thomas";

char* creditsFinMcGechie_2A222C = (char*)"Fin McGechie";

char* creditsIntroductorySequence_2A223C = (char*)"Introductory Sequence";

char* creditsChrisHill_2A2254 = (char*)"Chris Hill";

char* creditsScripting_2A2260 = (char*)"Scripting";

char* creditsSeanMasterson_2A226C = (char*)"Sean Masterson";

char* creditsLevelDesign_2A227C = (char*)"Level Design";

char* creditsAdrianMoore_2A228C = (char*)"Adrian Moore";

char* creditsVinceFarquharson_2A229C = (char*)"Vince Farquharson";

char* creditsBarryMeade_2A22B0 = (char*)"Barry Meade";

char* creditsSoundAndMusic_2A22BC = (char*)"Sound and Music";

char* creditsRussellShaw_2A22CC = (char*)"Russell Shaw";

char* creditsVoiceOvers_2A22DC = (char*)"Voice Overs";

char* creditsHugoMyatt_2A22E8 = (char*)"Hugo Myatt";

char* creditsTechnicalSupport_2A22F4 = (char*)"Technical Support";

char* creditsKevinDonkin_2A2308 = (char*)"Kevin Donkin";

char* creditsMikeBurnham_2A2318 = (char*)"Mike Burnham";

char* creditsPRAndMarketing_2A2328 = (char*)"PR and Marketing";

char* creditsCathyCampos_2A233C = (char*)"Cathy Campos";

char* creditsSeanRatcliffe_2A234C = (char*)"Sean Ratcliffe";

char* creditsPlayguide_2A235C = (char*)"Playguide";

char* creditsNeilCook_2A2368 = (char*)"Neil Cook";

char* creditsPlayguideDesign_2A2374 = (char*)"Playguide Design";

char* creditsRachelClose_2A2388 = (char*)"Rachel Close";

char* creditsTesting_2A2398 = (char*)"Testing";

char* creditsAndyRobsonJeffBrutus_2A23A0 = (char*)"Andy Robson, Jeff Brutus";

char* creditsAndyNuttallSteveFitton_2A23BC = (char*)"Andy Nuttall, Steve Fitton";

char* creditsRobertByrneRichardElliot_2A23D8 = (char*)"Robert Byrne, Richard Elliot";

char* creditsDavidRoganChristopherMitchell_2A23F8 = (char*)"David Rogan, Christopher Mitchell";

char* creditsNickHannahKenMalcolm_2A241C = (char*)"Nick Hannah, Ken Malcolm";

char* creditsPhilNortonAndrewTrowers_2A2438 = (char*)"Phil Norton, Andrew Trowers";

char* creditsDarrenJonesLukeFerris_2A2454 = (char*)"Darren Jones, Luke Ferris";

char* creditsCharlieRightMikeMan_2A2470 = (char*)"Charlie Right, Mike Man";

char* creditsRichardVelatiMarkStacey_2A2488 = (char*)"Richard Velati, Mark Stacey";

char* creditsKennethChanMikeDiskett_2A24A4 = (char*)"Kenneth Chan, Mike Diskett";

char* creditsJamesSmithEoinRogan_2A24C0 = (char*)"James Smith, Eoin Rogan";

char* creditsMRowbottomGerardO_2A24D8 = (char*)"M Rowbottom, Gerard O'Donaghue";

char* creditsPeterBlowPeterHughes_2A24F8 = (char*)"Peter Blow, Peter Hughes";

char* creditsBarrieParker_2A2514 = (char*)"Barrie Parker";

char* creditsAndreaDaSilvaGoncalves_2A2524 = (char*)"Andrea Da Silva Goncalves";

char* creditsMattSullivanAndyCakebread_2A2540 = (char*)"Matt Sullivan, Andy Cakebread";

char* creditsBarryClarkeJustinAmoore_2A2560 = (char*)"Barry Clarke, Justin Amoore";

char* creditsGrahamHarbourSimonRomans_2A257C = (char*)"Graham Harbour, Simon Romans";

char* creditsSymeonCosburn_2A259C = (char*)"Symeon Cosburn";

char* creditsThanksTo_2A25AC = (char*)"Thanks to";

char* creditsJoGoodwin_2A25B8 = (char*)"Jo Goodwin";

char* creditsKathyMcEntee_2A25C4 = (char*)"Kathy McEntee";

char* creditsDenis_2A25D4 = (char*)"Denis";

char* creditsQA_2A25DC = (char*)"QA";

char* creditsIainHancock_2A25E0 = (char*)"Iain Hancock";

char* creditsProducer_2A25F0 = (char*)"Producer";

char* creditsSeanCooper_2A25FC = (char*)"Sean Cooper";

//2AC558:
char* off_DB558[] = {
creditsDesignedBy_2A2144, creditsBullfrogProductionsLtd_2A2150, creditsSeparator_2A216C, creditsProjectLeader_2A2170,
creditsSpace_2A2180, creditsAlanWright_2A2184, creditsSpace_2A2180, creditsEngineProgramming_2A2190,
creditsSpace_2A2180, creditsGlennCorpes_2A21A4, creditsSeparator_2A216C, creditsLeadProgramming_2A21B4,
creditsSpace_2A2180, creditsAlanWright_2A2184, creditsSpace_2A2180, creditsProgramming_2A21C8,
creditsSpace_2A2180, creditsMarkLamport_2A21D4, creditsMarkAdami_2A21E4, creditsSeparator_2A216C,
creditsLeadArtist_2A21F0, creditsSpace_2A2180, creditsEoinRogan_2A21FC, creditsSpace_2A2180,
creditsGraphicsAndArt_2A2208, creditsSpace_2A2180, creditsDarrenThomas_2A221C, creditsFinMcGechie_2A222C,
creditsSeparator_2A216C, creditsIntroductorySequence_2A223C, creditsSpace_2A2180, creditsChrisHill_2A2254,
creditsSpace_2A2180, creditsScripting_2A2260, creditsSpace_2A2180, creditsSeanMasterson_2A226C,
creditsSeparator_2A216C, creditsLevelDesign_2A227C, creditsSpace_2A2180, creditsSeanMasterson_2A226C,
creditsAdrianMoore_2A228C, creditsVinceFarquharson_2A229C, creditsBarryMeade_2A22B0, creditsSeparator_2A216C,
creditsSoundAndMusic_2A22BC, creditsSpace_2A2180, creditsRussellShaw_2A22CC, creditsSeparator_2A216C,
creditsVoiceOvers_2A22DC, creditsSpace_2A2180, creditsHugoMyatt_2A22E8, creditsSeparator_2A216C,
creditsTechnicalSupport_2A22F4, creditsSpace_2A2180, creditsKevinDonkin_2A2308, creditsMikeBurnham_2A2318,
creditsSeparator_2A216C, creditsPRAndMarketing_2A2328, creditsSpace_2A2180, creditsCathyCampos_2A233C,
creditsSeanRatcliffe_2A234C, creditsSeparator_2A216C, creditsPlayguide_2A235C, creditsSpace_2A2180,
creditsNeilCook_2A2368, creditsSeanMasterson_2A226C, creditsSeparator_2A216C, creditsPlayguideDesign_2A2374,
creditsSpace_2A2180, creditsRachelClose_2A2388, creditsSeparator_2A216C, creditsTesting_2A2398,
creditsSpace_2A2180, creditsAndyRobsonJeffBrutus_2A23A0, creditsAndyNuttallSteveFitton_2A23BC, creditsRobertByrneRichardElliot_2A23D8,
creditsSeparator_2A216C, creditsSpace_2A2180, creditsDavidRoganChristopherMitchell_2A23F8, creditsNickHannahKenMalcolm_2A241C,
creditsPhilNortonAndrewTrowers_2A2438, creditsSeparator_2A216C, creditsSpace_2A2180, creditsDarrenJonesLukeFerris_2A2454,
creditsCharlieRightMikeMan_2A2470, creditsRichardVelatiMarkStacey_2A2488, creditsSeparator_2A216C, creditsSpace_2A2180,
creditsKennethChanMikeDiskett_2A24A4, creditsJamesSmithEoinRogan_2A24C0, creditsMRowbottomGerardO_2A24D8, creditsSeparator_2A216C,
creditsSpace_2A2180, creditsPeterBlowPeterHughes_2A24F8, creditsBarrieParker_2A2514, creditsAndreaDaSilvaGoncalves_2A2524,
creditsSeparator_2A216C, creditsSpace_2A2180, creditsMattSullivanAndyCakebread_2A2540, creditsBarryClarkeJustinAmoore_2A2560,
creditsGrahamHarbourSimonRomans_2A257C, creditsSymeonCosburn_2A259C, creditsSeparator_2A216C, creditsSpace_2A2180,
creditsThanksTo_2A25AC, creditsSpace_2A2180, creditsJoGoodwin_2A25B8, creditsKathyMcEntee_2A25C4,
creditsDenis_2A25D4, creditsSeparator_2A216C, creditsQA_2A25DC, creditsSpace_2A2180,
creditsIainHancock_2A25E0, creditsSeparator_2A216C, creditsProducer_2A25F0, creditsSpace_2A2180,
creditsSeanCooper_2A25FC, NULL, NULL, NULL };

int debugTurn_E9C14; // weak

int debugLastTurn_E9C18; // weak

int x_DWORD_E9C1C; // weak

int debugLastValue_E9C20; // weak

int debugFps_E9C24; // weak

uint8_t uiBackGroundColorIdx_EB3A8;

void(*ptrDrawBitmap_F01EC)(int16_t, int16_t, bitmap_pos_struct_t, uint8_t scale); // eax

int memFree_17FEA4; // weak

int memUsed_17FEA8; // weak

char FontType_D419D = 1; // weak
int16_t debugLangIndex_D41D4 = 1; // weak
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
std::array<uint8_t, 24> objectiveTextIndexes_DB520 = 
{ //index for levels endings?
	0x30, 0x33, 0x3F, 0x3D, 0x92, 0x71,
	0x4E, 0x04, 0x01, 0x00, 0x02, 0x00,
	0x08, 0x00, 0x10, 0x00, 0x20, 0x00,
	0x80, 0x00, 0x00, 0x01, 0x00, 0x04 
};
uint8_t externControl_E36C4[64] = { //fix it in future, this is only for extern control
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
};


void(*ptrDrawBitmap_F01E8)(int16_t, int16_t, bitmap_pos_struct_t, uint8_t scale); // eax

char cdChecked_D3F48 = 0; // weak
char savedScreenSize_D47D8 = 0; // weak
int16_t mapSpans_F4960[1664 * 8]; // Was 1664. Seems to be used in rendering the map 2x indexes ar a time. fix it -  weak
char spellIndex_D94FF[29] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,0,3,0 }; // idb
int16_t mousePosX_E375C = 320; // weak
int16_t mousePosY_E375E = 200; // weak


void DrawLangStringDebugViewer_30870();
void RestoreMousePosition_548B0(type_str_0x2BDE* a1);
signed __int16 IsCdPresent_10010();
void UpdateHelpItemState_889F0(uint16_t a1);

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

	if (videoMode_180660 & 1)
		func_draw = &DrawLine_BD524;
	else
		func_draw = &DrawLine_BD542;
	func_draw(posX1, posY1, posX2, posY2, color);

	//result = (int)x_D41A0_BYTEARRAY_0;
	if (world_D41A0.m_GameSettings.m_Display.m_uiScreenSize == 1)
	{
		temp_screen_buffer = pdwScreenBuffer_351628;
		pdwScreenBuffer_351628 = (uint8_t*)ptrMemoryBuffer_E9C3C;
		func_draw(posX1, posY1, posX2, posY2, color);
		//result = v7;
		pdwScreenBuffer_351628 = temp_screen_buffer;
	}
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// E9C3C: using guessed type int ptrMemoryBuffer_E9C3C;
// 180628: using guessed type int pdwScreenBuffer_351628;
// 180660: using guessed type int16_t videoMode_180660;

//----- (0002D190) --------------------------------------------------------
void DrawBar_2D190(int16_t posStartX, int16_t posStartY, int16_t maxPosEndX, int16_t posEndY, int16_t posEndX, uint8 colorIdx)//20e190
{
	if (posEndX >= maxPosEndX)
		posEndX = maxPosEndX;
	if (posEndX >= 2)
		DrawLine_2BC80(posStartX, posStartY, posEndX, posEndY, colorIdx);
}

bool UseInGameUiLayout()
{
	return videoMode_180660 != 1 && !DefaultResolutions();
}

int GetUiElementPosX(const UiElementLayout& layout, int elementWidth)
{
	const int screenWidth = screenWidth_18062C;
	int posX;
	switch (layout.alignment)
	{
	case UiAlignment::Right:
		posX = screenWidth - layout.marginX - elementWidth;   // in from the right edge
		break;
	case UiAlignment::Center:
		posX = (screenWidth - elementWidth) / 2 + layout.marginX;   // moved right (+) or left (-) of the middle
		break;
	case UiAlignment::Left:
	default:
		posX = layout.marginX;   // in from the left edge
		break;
	}

	// Keep the element on the screen
	const int maxPosX = (std::max)(screenWidth - elementWidth, 0);
	return (std::clamp)(posX, 0, maxPosX);
}

int GetTopBarWidth(uint8_t scale)
{
	// 2 pixel border either side, the panel around the mini-map and the 3 health/mana panels
	return (4 + (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[MINI_MAP_PANEL].width_4
		+ 3 * (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[HEALTH_PANEL].width_4) * scale;
}

void GetTopBarPosition(int16_t* posX, int16_t* posY)
{
	*posX = 0;
	*posY = 0;
	if (UseInGameUiLayout())
	{
		*posX = GetUiElementPosX(topBarLayout, GetTopBarWidth(gameUiScale));
		*posY = (std::clamp)(topBarLayout.marginY, 0, (int)screenHeight_180624);   // down from the top
	}
}

void GetSpellIconsPosition(int16_t* leftIconPosX, int16_t* rightIconPosX, int16_t* posY)
{
	*leftIconPosX = 510;
	*rightIconPosX = 574;
	*posY = 2;
	if (UseInGameUiLayout())
	{
		const int scale = gameUiScale;
		// 2 pixel border either side of the 2 icons
		const int posX = GetUiElementPosX(spellIconsLayout, 132 * scale);
		*leftIconPosX = posX + (2 * scale);
		*rightIconPosX = posX + (66 * scale);
		*posY = (2 * scale) + (std::clamp)(spellIconsLayout.marginY, 0, (int)screenHeight_180624);   // down from the top
	}
}

void GetSpellMenuPosition(int16_t* posX, int16_t* bottomPosY)
{
	*posX = 0;
	*bottomPosY = (videoMode_180660 & 1) ? 400 : 480;
	if (UseInGameUiLayout())
	{
		*posX = GetUiElementPosX(spellMenuLayout, 640 * gameUiScale);
		*bottomPosY = (int)screenHeight_180624 - (std::clamp)(spellMenuLayout.marginY, 0, (int)screenHeight_180624);   // up from the bottom
	}
}

//----- (0002D710) --------------------------------------------------------
void DrawTopStatusBar_2D710(type_entity_0x6E8E* a1x, int16_t barPosX, int16_t barPosY, uint8_t scale)//20e710
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

	manaColourIdx = playersColors_E88E0x[GetTrueWizardNumber_61790(a1x->playerData_0xA4_164x->playerColorIndex_0x38_56)][0];
	manaMaxColourIdx = playersColors_E88E0x[GetTrueWizardNumber_61790(a1x->playerData_0xA4_164x->playerColorIndex_0x38_56)][1];
	v26 = playersColors_E88E0x[GetTrueWizardNumber_61790(a1x->playerData_0xA4_164x->playerColorIndex_0x38_56)][gameState_D41A4.colorIndex_121[2]];
	//ptrDrawBitmap_F01E8(2, 2, **filearray_2aa18c[6] + 240);

	//Panel around MiniMap
	ptrDrawBitmap_F01E8(barPosX + (2 * scale), barPosY + (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[MINI_MAP_PANEL], scale);

	//v1 = *(uint8_t *)(*filearray_2aa18c[6] + 244) + 2;
	posX = ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[MINI_MAP_PANEL].width_4 * scale) + (2 * scale) + barPosX;
	//adress 20e7db
	v18 = gameState_D41A4.manaStats_242ar.totalMana_4;
	//v20 = a1x->dword_0xA4_164;
	v2x = Entities_EA3E4[a1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58];
	v23x = v2x;
	if (v2x > Entities_EA3E4[0] && v2x->param_0x10_16)
	{
		if (a1x->playerData_0xA4_164x->castleDestroyedBlink_0x195_405 > 0 && gameState_D41A4.colorIndex_121[1])
		{
			//ptrDrawBitmap_F01E8(v1, 2, **filearray_2aa18c[6] + 330);
			//Draw Castle Health Panel Hit
			ptrDrawBitmap_F01E8(posX, barPosY + (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[HEALTH_PANEL_HIT], scale);
			a1x->playerData_0xA4_164x->castleDestroyedBlink_0x195_405--;
		}
		else
		{
			//Draw Castle Health Panel
			ptrDrawBitmap_F01E8(posX, barPosY + (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[HEALTH_PANEL], scale);
		}
		//DrawBitmap_2BB40(v1 + 2, 2, (uint8_t**)(6 * (v23[4] + 43) + **filearray_2aa18c[6]));
		
		//Castle Icon
		DrawBitmap_2BB40(posX + (2 * scale), barPosY + (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v23x->param_0x10_16 + 43], scale);
		//Castle Health And Mana Icon
		DrawBitmap_2BB40(posX + (38 * scale), barPosY + (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[HEALTH_MANA_ICONS], scale);
		barStartXPos = posX + (58 * scale);
		DrawBar_2D190(posX + (58 * scale), barPosY + 10 * scale, 64 * scale, 10 * scale, ((v23x->life_0x8 << 6) / v23x->maxLife_0x4) * scale, 0x7Bu);
		v17 = (v23x->mana_0x90_144 + a1x->playerData_0xA4_164x->extraMana_0x13C_316) << 6;
		if (v23x->mana_0x90_144 + a1x->playerData_0xA4_164x->extraMana_0x13C_316 == v23x->maxMana_0x8C_140)
		{
			if (gameState_D41A4.colorIndex_121[2] == 1)
			{
				v3 = v18;
				//v4 = v23x->dword_0x8C_140 << 6;
				v16 = manaMaxColourIdx;
				v5 = v23x->maxMana_0x8C_140 << 6;
			}
			else
			{
				v16 = manaColourIdx;
				v5 = (v23x->mana_0x90_144 + a1x->playerData_0xA4_164x->extraMana_0x13C_316) << 6;
				//v4 = (v23x->dword_0x90_144 + a1x->playerData_0xA4_164x->dword_0x13C_316) << 6;
				v3 = v18;
			}
		}
		else
		{
			DrawBar_2D190(barStartXPos, barPosY + 28 * scale, 64 * scale, 10 * scale, ((v23x->maxMana_0x8C_140 << 6) / v18) * scale, manaMaxColourIdx);
			v16 = manaColourIdx;
			v5 = v17;
			//v4 = v17;
			v3 = v18;
		}
		//LODWORD(v6) = v5;
		//HIDWORD(v6) = v4 >> 31;
		DrawBar_2D190(barStartXPos, barPosY + 28 * scale, 64 * scale, 10 * scale, /*v6*/(v5 / v3) * scale, v16);

		//Draw Mana Goal Lines
		if (!(gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE) && world_D41A0.terrain_2FECE.manaGoalPercent_0x2FED5)
		{
			DrawLine_2BC80(barStartXPos + ((world_D41A0.terrain_2FECE.manaGoalPercent_0x2FED5 << 6) / 100) * scale, barPosY + 26 * scale, 2 * scale, 2 * scale, v26);
			DrawLine_2BC80(barStartXPos + ((world_D41A0.terrain_2FECE.manaGoalPercent_0x2FED5 << 6) / 100) * scale, barPosY + 38 * scale, 2 * scale, 2 * scale, v26);
		}
	}
	else
	{
		//Panel Next to MiniMap
		ptrDrawBitmap_F01E8(posX, barPosY + (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[PANEL], scale);
	}
	//v8 = *(uint8_t *)(**filearray_2aa18c[6] + 250) + v1;
	//adress 20eaa0
	posX = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[HEALTH_PANEL].width_4 * scale + posX;
	if (Entities_EA3E4[0] >= v23x)
	{
		//20eca6
		//VGA_Debug_Blit(640, 480, pdwScreenBuffer_351628);
	    //2nd Panel 
		ptrDrawBitmap_F01E8(posX, barPosY + (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[PANEL], scale);
		//VGA_Debug_Blit(640, 480, pdwScreenBuffer_351628);
	}
	else
	{
		if (a1x->playerData_0xA4_164x->castleDamagedBlink_0x197_407 > 0 && gameState_D41A4.colorIndex_121[1])
		{
			//Draw Balloon Health Panel Hit
			ptrDrawBitmap_F01E8(posX, barPosY + (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[HEALTH_PANEL_HIT], scale);
			a1x->playerData_0xA4_164x->castleDamagedBlink_0x197_407--;
		}
		else
		{
			//Draw Balloon Health Panel Hit
			ptrDrawBitmap_F01E8(posX, barPosY + (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[HEALTH_PANEL], scale);
		}
		switch (v23x->param_0x10_16)
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
		DrawBitmap_2BB40(posX + (2 * scale), barPosY + (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[numBalloons_v25 + BALLON1_ICON - 1], scale);
		balloonIdx_v24 = 0;
		//Draw Balloon Health Icon
		DrawBitmap_2BB40(posX + (38 * scale), barPosY + (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[HEALTH_MANA_ICONS], scale);
		if (numBalloons_v25 > 0)
		{
			do
			{
				v21 = posX + (58 * scale);
				v9x = Entities_EA3E4[a1x->playerData_0xA4_164x->balloons_0x3C_60[balloonIdx_v24]];
				//v19x = v9x;
				if (v9x > Entities_EA3E4[0])
				{
					//Life
					if (v9x->life_0x8 >= 0)
						DrawBar_2D190(v21, barPosY + (2 * balloonIdx_v24 * scale) + (12 * scale), 64 * scale, 2 * scale, ((v9x->life_0x8 << 6) / v9x->maxLife_0x4) * scale, 0x7Bu);

					//Mana collected
					DrawBar_2D190(v21, barPosY + (2 * balloonIdx_v24 * scale) + (30 * scale), 64 * scale, 2 * scale, ((v9x->mana_0x90_144 << 6) / v9x->maxMana_0x8C_140) * scale, manaColourIdx);
				}
				balloonIdx_v24++;
			} while (balloonIdx_v24 < numBalloons_v25);
		}
	}
	posX = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[HEALTH_PANEL].width_4 * scale + posX;

	if (a1x->playerData_0xA4_164x->PlayerHitFrameTime_406 > 0 && gameState_D41A4.colorIndex_121[1])
	{
		//Player Health Panel on Hit
		ptrDrawBitmap_F01E8(posX, barPosY + 2 * scale, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[HEALTH_PANEL_HIT], scale);
		a1x->playerData_0xA4_164x->PlayerHitFrameTime_406--;
	}
	else
	{//adress 20ed35
		//Player Health Panel
		ptrDrawBitmap_F01E8(posX, barPosY + 2 * scale, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[HEALTH_PANEL], scale);
	}

	if ((a1x->flags_0xc_12_15.byte[0] & 0x20 || a1x->playerData_0xA4_164x->invulnerableTimer_0x159_345) && gameState_D41A4.colorIndex_121[2])
	{
		//Draw Player Health and Mana flickering
		GameBitmap::DrawTransparentBitmap_2DE80(posX + (2 * scale), barPosY + (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[PLAYER_ICON], scale);
		GameBitmap::DrawTransparentBitmap_2DE80(posX + (38 * scale), barPosY + (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[HEALTH_MANA_ICONS], scale);
	}
	else
	{
		//Draw Player Health and Mana
		DrawBitmap_2BB40(posX + (2 * scale), barPosY + (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[PLAYER_ICON], scale);
		DrawBitmap_2BB40(posX + (38 * scale), barPosY + (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[HEALTH_MANA_ICONS], scale);
	}
	//VGA_Debug_Blit(640, 480, pdwScreenBuffer_351628);
	posX = posX + (58 * scale);
	DrawBar_2D190(posX, barPosY + 10 * scale, 64 * scale, 10 * scale, ((a1x->life_0x8 << 6) / a1x->maxLife_0x4) * scale, 0x7Bu);
	//adress 20ee1a
	//1b8 1c 40 0a 03 aa
	//0xfa00/4c79=3
	//v18=0x4c79
	DrawBar_2D190(posX, barPosY + 28 * scale, 64 * scale, 10 * scale, ((a1x->maxMana_0x8C_140 << 6) / v18) * scale, manaMaxColourIdx);
	DrawBar_2D190(posX, barPosY + 28 * scale, 64 * scale, 10 * scale, ((a1x->mana_0x90_144 << 6) / v18) * scale, manaColourIdx);
	//VGA_Debug_Blit(640, 480, pdwScreenBuffer_351628);
}

// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3DC: using guessed type int **filearray_2aa18c[6];
// EA3E4: using guessed type int Entities_EA3E4[];
// F01E8: using guessed type int (*ptrDrawBitmap_F01E8)(x_DWORD, x_DWORD, x_DWORD);
// 180628: using guessed type int pdwScreenBuffer_351628;
// 18062C: using guessed type int screenWidth_18062C;
// 180660: using guessed type int16_t videoMode_180660;

//----- (0002E260) --------------------------------------------------------
void DrawSpellIcon_2E260(int16_t posX, int16_t posY, type_entity_0x6E8E* playerEvent, bool altDrawFunction, uint8_t scale)//20f260
{
	if (playerEvent > Entities_EA3E4[0])
	{
		GetFont_6FC50(FontType_D419D);
		type_entity_0x6E8E* parentEvent = Entities_EA3E4[playerEvent->parentId_0x28_40];
		if (parentEvent > Entities_EA3E4[0])
		{
			uint8_t color0 = playersColors_E88E0x[GetTrueWizardNumber_61790(parentEvent->playerData_0xA4_164x->playerColorIndex_0x38_56)][0];
			uint8_t color1 = playersColors_E88E0x[GetTrueWizardNumber_61790(parentEvent->playerData_0xA4_164x->playerColorIndex_0x38_56)][1];
			if (!(SPELLS_BEGIN_BUFFER_str[playerEvent->model_0x40_64].isEnabled_1 & 4)
				|| playerEvent->stage_0x2E_46 <= 0
				|| playerEvent->stage_0x2E_46 >= 32
				|| !gameState_D41A4.colorIndex_121[1])
			{
				if (altDrawFunction)//adress 20f334
				{
					bitmap_pos_struct_t bitmap;
					if (playerEvent->stage_0x2E_46)
						bitmap = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TOPTILE_BAR_GLOW];
					else
						bitmap = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TOPTILE_BAR];
					DrawBitmap_2BB40(posX, posY, bitmap, scale);
				}
				else
				{
					bitmap_pos_struct_t bitmap;
					if (playerEvent->stage_0x2E_46)
						bitmap = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TOPTILE_BAR_GLOW];
					else
						bitmap = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TOPTILE_BAR];
					ptrDrawBitmap_F01E8(posX, posY, bitmap, scale);
				}
				DrawBitmap_2BB40(posX, posY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[playerEvent->model_0x40_64 + SPELL_FIREBALL_BIG], scale);
				DrawText_2BC10((char*)SpellLevelText_DB06C[playerEvent->levelPar3_0x46_70],
					(*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TOPTILE_BAR].width_4 * scale + posX - (8 * scale) * strlen((const char*)*(&SpellLevelText_DB06C[playerEvent->levelPar3_0x46_70])) - (2 * scale),//test and fix
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
					if (!parentEvent->playerData_0xA4_164x->CastleEntityIndex_0x3A_58 || playerEvent->manaRegen_0x88_136 > Entities_EA3E4[parentEvent->playerData_0xA4_164x->CastleEntityIndex_0x3A_58]->mana_0x90_144)
						if (world_D41A0.terrain_2FECE.MapType != MapType_t::Day)
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
		textLenght = strlen((const char*)langStrings_E9C4C[textLines[indexLine]]) + 1;
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
	int widthFix = ((videoMode_180660 & 1) != 0) + 1;
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
		sprintf(buffer, "[F1] %s", langStrings_E9C4C[14]);//Toggle help on / off
		break;
	case 96:
		sprintf(buffer, (const char*)langStrings_E9C4C[NUMBER_OF_TIMES_YOU_HAVE_KILLED], std::string(WizardsNames_D93A0[GetTrueWizardNumber_61790(helpPopup_1804B0.wizardNumber_0xa8)]).c_str() );//Number of times you have killed %s
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

	if (videoMode_180660 == 1)
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
			pdwScreenBuffer_351628[screenWidth_18062C * indexY + indexX] = ColourLookupTable_F6EE0[256 * color
				+ pdwScreenBuffer_351628[screenWidth_18062C * indexY + indexX]];
			SetAlphaXY(indexX, indexY, ALPHA_BLENDED);
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
	if (!(gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE) && !(gameState_D41A4.setting_byte1_22 & 0x20))
	{
		if (gameState_D41A4.setting_byte2_23 & 2)
		{
			if (world_D41A0.playerObjectives_0x3659C[world_D41A0.LevelIndex_0xc].substr_3659C.IsLevelEnd_0)
				result = objectiveTextIndexes_DB520[6];//Toggle help on / off
			else
				result = objectiveTextIndexes_DB520[world_D41A0.playerObjectives_0x3659C[world_D41A0.LevelIndex_0xc].substr_3659C.ObjectiveText_1];
		}
		else if (world_D41A0.gameFlags_0x36E0B & 1)
		{
			result = (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].secretCounter_0x3E4_2BE4_12226 != 0) + 284;//Invisible
		}
		else
		{
			v2 = gameState_D41A4.levelnumber_43w;
			if (v2 >= 0x1Eu && v2 <= 0x22u)
			{
				if (world_D41A0.playerObjectives_0x3659C[world_D41A0.LevelIndex_0xc].substr_3659C.IsLevelEnd_0)
					result = LevelEndText_DB507[10];
				else
					result = IndexLevelText_DB4EE[0] + 3;
			}
			else if (gameState_D41A4.setting_38545 & 0x20
				&& !world_D41A0.playerObjectives_0x3659C[world_D41A0.LevelIndex_0xc].substr_3659C.ObjectiveText_1)
			{
				world_D41A0.objectiveBoxCounter_0x36E04 = 0;
			}
			else
			{
				if (world_D41A0.playerObjectives_0x3659C[world_D41A0.LevelIndex_0xc].substr_3659C.IsLevelEnd_0)
					result = LevelEndText_DB507[gameState_D41A4.levelnumber_43w];
				else
					result = world_D41A0.playerObjectives_0x3659C[world_D41A0.LevelIndex_0xc].substr_3659C.ObjectiveText_1
					+ IndexLevelText_DB4EE[gameState_D41A4.levelnumber_43w];
			}
		}
		if (result)
		{
			text = (char*)langStrings_E9C4C[result];
			/*"Fly towards my beacon."*/

			textbox.charWidth_0x10 = 8;
			textbox.maxTextboxHeight2_0xe = 400;
			textbox.minPosX_0x14 = 40;
			textbox.maxPosX_0x16 = 600;
			textbox.charHeight_0x12 = 14;
			textbox.maxPosY_0x1a = 260;

			if (!DefaultResolutions())
			{
				textbox.minPosX_0x14 = 40 * scale;
				textbox.maxPosX_0x16 = screenWidth_18062C - (40 * scale);
				textbox.maxPosY_0x1a = screenHeight_180624 / 2;
			}

			textbox.color1_0x30 = (*xadataclrd0dat.colorPalette_var28)[3840];
			textbox.color2_0x31 = ((*xadataclrd0dat.colorPalette_var28)[0]);
			textbox.minPosY_0x18 = 80 * scale;
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
			ConstrainTextboxSizes_89520(&textbox, scale);
			ComputeFrameSizes_89980(&textbox, scale);
			ColorizeScreen_2E790(textbox.framePosX_0x24, textbox.framePosY_0x26, textbox.frameWidth_0x28, textbox.frameHeight_0x2a, textbox.color2_0x31);
			AlphaRectangle(textbox.framePosX_0x24, textbox.framePosY_0x26, textbox.frameWidth_0x28, textbox.frameHeight_0x2a, 100);
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
		world_D41A0.m_GameSettings.m_Display.m_uiScreenSize = savedScreenSize_D47D8;
		if ((unsigned __int8)savedScreenSize_D47D8 >= 1u && (unsigned __int8)savedScreenSize_D47D8 <= 1u)
			InstallPaletteAndMouseMinMax2_417D0();
		savedScreenSize_D47D8 = 0;
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
	if (playStr->playerSlot_0x007_2BE4_11237 != world_D41A0.LevelIndex_0xc)
	{
		CheckCdPresent_53120();
		return;
	}
	gameState_D41A4.savedMenuState_38544 = currentMenuState ;
	CloseHelpPopup_87C10();
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
		if (inputState_18058C.joystickMode_1805C2 == 7 || inputState_18058C.joystickMode_1805C2 == 1 || inputState_18058C.joystickMode_1805C2 == 2)
			SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[cursorSpriteIndex_D419E]); // fix it
	}
	else if (gameState_D41A4.OptionsSettingFlag_24 & GAME_PAUSED)
	{
		if (inputState_18058C.joystickMode_1805C2 != 7 && inputState_18058C.joystickMode_1805C2 != 1 && inputState_18058C.joystickMode_1805C2 != 2)
			SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]);
		else
			SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[cursorSpriteIndex_D419E]); // fix it
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
		world_D41A0.objectiveBoxCounter_0x36E04 = 0;  // hide objective message box
		break;
	default:
		break;
	}
	//Close of menu
	switch (currentMenuState )
	{
	case (int)MenuState::NONE:
	case 6u:
		if (newMenuState == currentMenuState && !(gameState_D41A4.OptionsSettingFlag_24 & GAME_PAUSED))
		{
			RestoreMousePosition_548B0(playStr);
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
		RestoreMousePosition_548B0(playStr);
		if (!(gameState_D41A4.OptionsSettingFlag_24 & GAME_PAUSED))
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
		if (newMenuState == currentMenuState  && gameState_D41A4.OptionsSettingFlag_24 & GAME_PAUSED)
		{
			SetCenterScreenForFlyAssistant_6EDB0();
			BackupMousePosition_548F0(playStr);
			break;
		}
		break;
	case (int)MenuState::SHOW_CHAT_MENU:
	case (int)MenuState::SHOW_BOTTOM_MENU:
	case (int)MenuState::SHOW_MAP_BOTTOM_MENU:
		BackupMousePosition_548F0(playStr);
		break;
	case (int)MenuState::SHOW_IN_GAME_OPTIONS:
	case (int)MenuState::SHOW_MAP_GAME_OPTIONS:
		SetOptionsMenuFlags_1A280();
		gameState_D41A4.byteindex_225 = 1;
		gameState_D41A4.optionsMenuWidth_186 = GetOptionsMenuWidth_1A4A0();
		BackupMousePosition_548F0(playStr);
		break;
	case (int)MenuState::SHOW_VOLUME_OPTIONS:
	case (int)MenuState::SHOW_MAP_VOLUME_OPTIONS:
		BackupMousePosition_548F0(playStr);
		SetSoundEffectAndMusicLevelCoordinates_19D60(newMenuState);
		break;
	case (int)MenuState::SHOW_OK_CANCEL_OPTIONS:
	case (int)MenuState::SHOW_MAP_OK_CANCEL_OPTIONS:
		BackupMousePosition_548F0(playStr);
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
			playStr->playerData_0x3E6_2BE4_12228.str_611.selectSpellType_0x457_1111 = 0;
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
		if ((!DefaultResolutions())&&(videoMode_180660 != 1))
			viewPort.SetViewPortScreenCoordinates_2CA60(384 * scale, 0, screenWidth_18062C - 384, screenHeight_180624 - 80);
		else
			viewPort.SetViewPortScreenCoordinates_2CA60(384, 0, 256, 400);
		break;
	default:
		viewPort.ResizeViewPort_2CA90(world_D41A0.m_GameSettings.m_Graphics.m_wViewPortSize);
		break;
	}
	if (useSound)
		PrepareEventSound_6E450(0, playStr->playerSlot_0x007_2BE4_11237, 14);
	CheckCdPresent_53120();
}

//----- (00053120) --------------------------------------------------------
void CheckCdPresent_53120()//234120
{
	if (!(gameState_D41A4.FrameTimingIndex_26 & 7))
	{
		if ((gameState_D41A4.cdFlags_52 & 3) == 1)
		{
			if (!IsCdPresent_10010())
			{
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].syncReady_0x004_2BE0_11234 = 2;
			}
		}
	}
}

//----- (000595C0) --------------------------------------------------------
char GetObjectiveStage_595C0(int16_t a1)//23a5c0
{
	char result = -1;
	if (world_D41A0.gameFlags_0x36E0B & 1)
		return -3;
	if (!world_D41A0.playerObjectives_0x3659C[a1].substr_3659C.IsLevelEnd_0)
		result = world_D41A0.stages_0x3654C[world_D41A0.playerObjectives_0x3659C[a1].substr_3659C.ObjectiveText_1].stages_3654C_byte0;
	return result;
}

//----- (00059610) --------------------------------------------------------
char GetObjectiveTargetEntity_59610(type_entity_0x6E8E** a1y, int16_t a2)//23a610
{
	char v5; // dl
	*a1y = 0;
	v5 = 0;
	if (world_D41A0.playerObjectives_0x3659C[a2].substr_3659C.IsLevelEnd_0)
		return v5;
	if (world_D41A0.stages_0x3654C[world_D41A0.playerObjectives_0x3659C[a2].substr_3659C.ObjectiveText_1].stages_3654C_byte0 < 4u)
	{
		if (world_D41A0.stages_0x3654C[world_D41A0.playerObjectives_0x3659C[a2].substr_3659C.ObjectiveText_1].stages_3654C_byte0 < 1u || world_D41A0.stages_0x3654C[world_D41A0.playerObjectives_0x3659C[a2].substr_3659C.ObjectiveText_1].stages_3654C_byte0 > 2u)
			return v5;
	}
	else if (world_D41A0.stages_0x3654C[world_D41A0.playerObjectives_0x3659C[a2].substr_3659C.ObjectiveText_1].stages_3654C_byte0 > 4u)
	{
		if (world_D41A0.stages_0x3654C[world_D41A0.playerObjectives_0x3659C[a2].substr_3659C.ObjectiveText_1].stages_3654C_byte0 == 6 && world_D41A0.stages_0x3654C[world_D41A0.playerObjectives_0x3659C[a2].substr_3659C.ObjectiveText_1].flags_3654D & 1)
		{
			v5 = 1;
			*a1y = Entities_EA3E4[world_D41A0.stages_0x3654C[world_D41A0.playerObjectives_0x3659C[a2].substr_3659C.ObjectiveText_1].target_36552.dword];
			return v5;
		}
		return v5;
	}
	if (world_D41A0.stages_0x3654C[world_D41A0.playerObjectives_0x3659C[a2].substr_3659C.ObjectiveText_1].flags_3654D & 1)
	{
		v5 = 1;
		*a1y = world_D41A0.stages_0x3654C[world_D41A0.playerObjectives_0x3659C[a2].substr_3659C.ObjectiveText_1].target_36552.ptr0x6E8E;
	}
	return v5;
}

//----- (000596C0) --------------------------------------------------------
bool GetObjectiveTargetPosition_596C0(axis_3d* position, int16_t a2)//23a6c0
{
	bool result = false;
	position->x = -1;
	position->y = -1;
	position->z = -1;
	if (!world_D41A0.playerObjectives_0x3659C[a2].substr_3659C.IsLevelEnd_0)
	{
		if (world_D41A0.stages_0x3654C[world_D41A0.playerObjectives_0x3659C[a2].substr_3659C.ObjectiveText_1].stages_3654C_byte0 >= 4u && world_D41A0.stages_0x3654C[world_D41A0.playerObjectives_0x3659C[a2].substr_3659C.ObjectiveText_1].stages_3654C_byte0 <= 5u)
		{
			position->x = world_D41A0.stages_0x3654C[world_D41A0.playerObjectives_0x3659C[a2].substr_3659C.ObjectiveText_1].position_3654E.x;
			position->y = world_D41A0.stages_0x3654C[world_D41A0.playerObjectives_0x3659C[a2].substr_3659C.ObjectiveText_1].position_3654E.y;
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
		if (gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
		{
			switch (world_D41A0.players_0x2BDE[index].playerData_0x3E6_2BE4_12228.wizardType_0x1C0_448)
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

//Draws an enemy player's position on the minimap as a scale x scale block centred on (posX, posY), clipped to the minimap shape
void DrawMinimapPlayerDot(uint8_t* mapBuffer, uint8_t* mapBuffer2, int posX, int posY, uint16_t width, uint16_t height, uint8_t colour, uint8_t scale, bool setAlpha)
{
	const int offset = CalculateScaleOffset(scale);
	for (int y = posY - offset; y < posY - offset + scale; y++)
	{
		if (y < 0 || y >= height)
			continue;
		for (int x = posX - offset; x < posX - offset + scale; x++)
		{
			if (x < 0 || x >= width || x < mapSpans_F4960[1 + 2 * y] || x >= mapSpans_F4960[2 * y])
				continue;
			uint8_t* pixel = &mapBuffer[y * screenWidth_18062C + x];
			*pixel = colour;
			if (setAlpha)
				SetAlphaAt(pixel, ALPHA_VISIBLE);
			if (mapBuffer2 != nullptr)
				mapBuffer2[y * screenWidth_18062C + x] = colour;
		}
	}
}

void AlphaRectangle(int posX, int posY, int width, int height, uint8_t alphaAmount)
{
	if (pdwScreenAlphaBuffer == nullptr)
		return;

	int locPosX = posX;
	int locPosY = posY;
	int locWidth = width;
	int locHeight = height;

	if (videoMode_180660 == 1)
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
			pdwScreenAlphaBuffer[screenWidth_18062C * indexY + indexX] = alphaAmount;
		}
	}
}

int debugcounter2 = 0;
//----- (00061A00) --------------------------------------------------------
void DrawMinimapEntitiesB_61A00(int16_t x, int16_t y, int16_t posX, int16_t posY, uint16_t width, uint16_t height, int16_t yaw, int16_t scaling, uint8_t scale)//242a00
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
	v11 = world_D41A0.terrain_2FECE.MapType;
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
	if (videoMode_180660 & 1)
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
	const MapProjection projection(v73, v86, yaw, scaling);
	v80x = Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
	v16 = v80x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[12];
	if (v16)
		v83 = Entities_EA3E4[v16]->stage_0x2E_46;
	//v17 = v80x->dword_0xA4_164;
	if (v80x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58)
	{
		v18 = (int16_t)(v80x->position_0x4C_76.x - posX);
		v19 = (int16_t)(v80x->position_0x4C_76.y - posY);
		v20 = v82 + projection.X(v18, v19);
		v21 = projection.Y(v18, v19) + v76;
		v22x = Entities_EA3E4[v80x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58];
		v23 = (int16_t)(v22x->position_0x4C_76.x - posX);
		v24 = (int16_t)(v22x->position_0x4C_76.y - posY);
		v25 = v82 + projection.X(v23, v24);
		v26 = projection.Y(v23, v24) + v76;
		v27 = v82 + projection.X(v23, v24) - v20;
		v75 = Maths::IntegerSqrt_7277A(v27 * v27 + (v26 - v21) * (v26 - v21));
		v72 = (uint16_t)Maths::Atan2_72633(v25 - v20, v26 - v21);
		for (i = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].Turn_2BE0_11248 & 3;
			;
			*(x_BYTE*)(ptrMapBufferStart_v84 + v20 + (i * Maths::sin_DB750[v72] >> 16) + screenWidth_18062C * v85) = ColourLookupTable_F6EE0[COLOUR_BLEND_LOOKUP_OFFSET + 256
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

			if (v29 < 0 || v29 >= width || v85 < 0 || v85 >= height || v29 < mapSpans_F4960[1 + 2 * v85] || v29 >= mapSpans_F4960[2 * v85])
				break;
		}
	}
	//for all entites
	//VGA_Debug_Blit(640, 480, pdwScreenBuffer_351628);
	//for (jy = &D41A0_BYTESTR_0.struct_0x6E8E[1]; jy <= &D41A0_BYTESTR_0.struct_0x6E8E[0x3e8-1]; jy += sizeof(type_str_0x6E8E))
	for (jy = &world_D41A0.entities_0x6E8E[1]; jy <= &world_D41A0.entities_0x6E8E[0x3e8 - 1]; jy++)
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
					if (gameState_D41A4.colorIndex_121[3])
						BlipColourIdx_v94 = (*xadataclrd0dat.colorPalette_var28)[MapColourIndexs::MARKER_STONE];
					else
						v89 = 1;
					v88 = 27;
					break;
				case 2u:
					if (gameState_D41A4.colorIndex_121[2])
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
						v31 = TransformPlayerColorIndex_616D0(Entities_EA3E4[jy->id_0x1A_26]->playerData_0xA4_164x->playerColorIndex_0x38_56)//castle rope
							+ 58;
					}
					else if (v50 == 3)
					{
						if (jy->id_0x1A_26 == v80x->id_0x1A_26 || v83)
							v31 = TransformPlayerColorIndex_616D0(Entities_EA3E4[jy->id_0x1A_26]->playerData_0xA4_164x->playerColorIndex_0x38_56)
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
				if (v44 >= 0xB4u && (v44 <= 0xB4u || v44 >= 0xE8u && v44 <= 0xEAu) || jy->flags_0xc_12_15.byte[0] & 1)
					break;
				v45 = jy->id_0x1A_26;
				if (v45 != jy - world_D41A0.entities_0x6E8E)
				{
					v47 = playersColors_E88E0x[GetTrueWizardNumber_61790(Entities_EA3E4[v45]->playerData_0xA4_164x->playerColorIndex_0x38_56)][1];
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
							if (gameState_D41A4.colorIndex_121[3])
								v38 = playersColors_E88E0x[GetTrueWizardNumber_61790(v41x->playerData_0xA4_164x->playerColorIndex_0x38_56)][0];
							else
								v38 = playersColors_E88E0x[GetTrueWizardNumber_61790(v41x->playerData_0xA4_164x->playerColorIndex_0x38_56)][1];
							goto LABEL_121;
						}
					}
					if (!(buildingParams_D93C0[jy->levelPar3_0x46_70].flags_2 & 8))
						v88 = 21;
				LABEL_56:
					v39x = Entities_EA3E4[jy->id_0x1A_26];
					if (v39x->class_0x3F_63 == 3)
						v38 = playersColors_E88E0x[GetTrueWizardNumber_61790(v39x->playerData_0xA4_164x->playerColorIndex_0x38_56)][0];
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
						if (gameState_D41A4.colorIndex_121[3])
							v37 = playersColors_E88E0x[GetTrueWizardNumber_61790(v36x->playerData_0xA4_164x->playerColorIndex_0x38_56)][0];
						else
							v37 = playersColors_E88E0x[GetTrueWizardNumber_61790(v36x->playerData_0xA4_164x->playerColorIndex_0x38_56)][1];
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
						if (jy->leaderId_0x32_50 != world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240)
							break;
						v38 = playersColors_E88E0x[GetTrueWizardNumber_61790(v80x->playerData_0xA4_164x->playerColorIndex_0x38_56)][gameState_D41A4.colorIndex_121[3]];
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
				if (jy->flags_0xc_12_15.byte[0] & 1)
					break;
				v38 = (*xadataclrd0dat.colorPalette_var28)[MapColourIndexs::SPELLS];
				v88 = 20;
				goto LABEL_121;
			case 0xCu:
				if (jy->model_0x40_64 != 5)
					break;
				if (gameState_D41A4.colorIndex_121[3])
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
			posx = v82 + projection.X(v52, v53);
			posx -= CalculateScaleOffset(scale);
			if (posx >= 0 && posx < width)
			{//24324e
				posy = projection.Y(v52, v53) + v76;
				posy -= CalculateScaleOffset(scale);
				if (posy >= 0 && posy < height && posx >= mapSpans_F4960[1 + 2 * posy] && posx < mapSpans_F4960[2 * posy])
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
									SetAlphaAt((uint8_t*)&ptrBlipBufferIdx_v56[x], ALPHA_VISIBLE);
								}
								ptrBlipBufferIdx_v56 += screenWidth_18062C;
							}
						}
						else if (videoMode_180660 == 1)
						{
							if (v31 < 0x42u || v31 >= 0x4Au)
							{
								if (v31 < 0x3Au || v31 >= 0x42u)
								{
									if (v31 == 83 || v31 == 84)
										DrawBitmap_2BB40(
											x + 2 * posx - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v31].width_4 / 2,
											y + 2 * posy - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v31].height_5 / 2,
											(*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v31]);
								}
								else
								{
									DrawBitmap_2BB40(x + 2 * posx, y + 2 * posy - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v31].height_5, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v31]);
								}
							}
							else
							{
								DrawBitmap_2BB40(
									x + 2 * posx - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v31].width_4 / 2,
									y + 2 * posy - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v31].height_5,
									(*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v31]);
							}
						}
						else if (v31 >= 0x3Cu)
						{
							//Draw other stuff (ballon icons)
							DrawBitmap_2BB40(
								x + posx - ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v31].width_4 / 2) * scale,
								y + posy - ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v31].height_5 * scale),
								(*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v31], scale);
						}
						else
						{
							//Draw Flag
							
							//DrawBitmap_2BB40(posx, posy - *(uint8_t *)(6 * v31 + **filearray_2aa18c[6] + 5), (uint8_t**)(6 * v31 + **filearray_2aa18c[6]));
							DrawBitmap_2BB40(x + posx, y + posy - ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v31].height_5 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v31], scale);
						}
					}
					if (v88)
						SetHelpItemTarget_885E0(jy, (int16_t)(v8 + posx), (int16_t)(v9 + posy), v88);   // v8, v9: the map's position
				}
			}
		}
	}

	if (v83)//player cross or other
	{
		v87 = 0;
		v74x = Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
		while (v87 < world_D41A0.NumberOfPlayers_0xe)
		{
			if (v87 != world_D41A0.LevelIndex_0xc)
			{
				//v79 = 2124 * v87;
				v59 = world_D41A0.players_0x2BDE[v87].playerIndex_0x00a_2BE4_11240;
				v60x = Entities_EA3E4[v59];
				if (v60x->life_0x8 >= 0)
				{
					if (CanSeePlayerOnMinimap_63570(v74x, Entities_EA3E4[v59]))
					{
						v61 = (int16_t)(v60x->position_0x4C_76.x - posX);
						v62 = (int16_t)(v60x->position_0x4C_76.y - posY);
						v63 = v82 + projection.X(v61, v62);
						if (v63 >= 0 && v63 < width)
						{
							v64 = v76 + projection.Y(v61, v62);
							if (v64 >= 0 && v64 < height && v63 >= mapSpans_F4960[1 + 2 * v64] && v63 < mapSpans_F4960[2 * v64])
							{
								v66 = playersColors_E88E0x[GetTrueWizardNumber_61790(v60x->playerData_0xA4_164x->playerColorIndex_0x38_56)][0];
								DrawMinimapPlayerDot(ptrMapBufferStart_v84, nullptr, v63, v64, width, height, v66, scale, true);
								if (videoMode_180660 == 1)
									DrawText_2BC10(world_D41A0.players_0x2BDE[v87].WizardName_0x39f_2BFA_12157, x + 2 * v63 + 4, y + 2 * v64, v66);
								else
									DrawText_2BC10(world_D41A0.players_0x2BDE[v87].WizardName_0x39f_2BFA_12157, x + v63 - CalculateScaleOffset(scale) + (4 * scale), y + v64 - CalculateScaleOffset(scale), v66, scale);
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
		char* index = (char*)&ColourLookupTable_F6EE0[COLOUR_BLEND_LOOKUP_OFFSET + 256 * CentreCrossColour_v93];
		uint8_t* ptrCrossSouth_v70 = (uint8_t*)(screenWidth_18062C * (int)(height / 2) + width / 2 + ptrMapBufferStart_v84 - 1);
		uint8_t* ptrCrossEast_v77 = (uint8_t*)(screenWidth_18062C * (int)(height / 2) + width / 2 + ptrMapBufferStart_v84 - 1);
		uint8_t* ptrCrossNorth_v81 = (uint8_t*)(screenWidth_18062C * (int)(height / 2) + width / 2 + ptrMapBufferStart_v84 - 1);

		for (*ptrCrossWest_v68 = index[(uint8_t)*ptrCrossWest_v68]; crossWidth_v67; *ptrCrossWest_v68 = ColourLookupTable_F6EE0[COLOUR_BLEND_LOOKUP_OFFSET + 256 * CentreCrossColour_v93 + v71])
		{
			ptrCrossNorth_v81 -= screenWidth_18062C;
			ptrCrossSouth_v70 += screenWidth_18062C;
			ptrCrossEast_v77++;
			*ptrCrossNorth_v81 = ColourLookupTable_F6EE0[COLOUR_BLEND_LOOKUP_OFFSET + 256 * CentreCrossColour_v93 + (uint8_t)*ptrCrossNorth_v81];
			*ptrCrossEast_v77 = ColourLookupTable_F6EE0[COLOUR_BLEND_LOOKUP_OFFSET + 256 * CentreCrossColour_v93 + (uint8_t)*ptrCrossEast_v77];
			*ptrCrossSouth_v70 = ColourLookupTable_F6EE0[COLOUR_BLEND_LOOKUP_OFFSET + 256 * CentreCrossColour_v93 + (uint8_t)*ptrCrossSouth_v70];
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
			*ptrCrossWest_v68 = ColourLookupTable_F6EE0[COLOUR_BLEND_LOOKUP_OFFSET + 256 * CentreCrossColour_v93 + (uint8_t)*ptrCrossWest_v68];
			*ptrCrossEast_v77 = ColourLookupTable_F6EE0[COLOUR_BLEND_LOOKUP_OFFSET + 256 * CentreCrossColour_v93 + (uint8_t)*ptrCrossEast_v77];
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
			*ptrCrossNorth_v81 = ColourLookupTable_F6EE0[COLOUR_BLEND_LOOKUP_OFFSET + 256 * CentreCrossColour_v93 + (uint8_t)*ptrCrossNorth_v81];
			*ptrCrossSouth_v70 = ColourLookupTable_F6EE0[COLOUR_BLEND_LOOKUP_OFFSET + 256 * CentreCrossColour_v93 + (uint8_t)*ptrCrossSouth_v70];
			ptrCrossNorth_v81 -= screenWidth_18062C;
			ptrCrossSouth_v70 += screenWidth_18062C;
		}
	}
}

//----- (000627F0) --------------------------------------------------------
void DrawMinimapEntities_627F0(int16_t x, int16_t y, int16_t posX, int16_t posY, uint16_t width, uint16_t height, int16_t yaw, int16_t scaling, uint8_t scale)//2437f0
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
	v11 = world_D41A0.terrain_2FECE.MapType;
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

	if (videoMode_180660 & 1)
	{
		v8 = x >> 1;
		v9 = y >> 1;
		v10 = 2 * scaling;
		width >>= 1;
		height >>= 1;
	}
	//v99 = v9 * x_DWORD_18062C_resolution_x + (int)pdwScreenBuffer + v8;
	v99x = &pdwScreenBuffer_351628[v9 * screenWidth_18062C + v8];
	v100x = &ptrMemoryBuffer_E9C3C[v9 * screenWidth_18062C + v8];
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
	const MapProjection projection(v87, v86, yaw, v10);
	v102x = Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
	v17 = v102x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[12];
	if (v17)
		v93 = Entities_EA3E4[v17]->stage_0x2E_46;
	//v18 = v102x->dword_0xA4_164;
	if (v102x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58)
	{
		v19 = (int16_t)(v102x->position_0x4C_76.x - posX);
		v20 = (int16_t)(v102x->position_0x4C_76.y - posY);
		v21 = v98 + projection.X(v19, v20);
		v22 = v87 * v20 + v86 * v19;
		v23x = Entities_EA3E4[v102x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58];
		v24 = projection.Y(v19, v20) + v85;
		v25 = (int16_t)(v23x->position_0x4C_76.x - posX);
		v26 = (int16_t)(v23x->position_0x4C_76.y - posY);
		v27 = v98 + projection.X(v25, v26);
		v28 = projection.Y(v25, v26) + v85;
		v29 = v98 + projection.X(v25, v26) - v21;
		v91 = Maths::IntegerSqrt_7277A(v29 * v29 + (v28 - v24) * (v28 - v24));
		v89 = (uint16_t)Maths::Atan2_72633(v27 - v21, v28 - v24);
		v30 = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].Turn_2BE0_11248 & 3;
		while (1)
		{
			v30 += 4;
			if (v30 > v91)
				break;
			v31 = v21 + (v30 * Maths::sin_DB750[v89] >> 16);
			v94 = v24 + (-v30 * (signed int)Maths::sin_DB750[0x200 + v89] >> 16);
			if (v31 < 0 || v31 >= width || v94 < 0 || v94 >= height || v31 < mapSpans_F4960[1 + 2 * v94] || v31 >= mapSpans_F4960[2 * v94])
				break;
			v32x = &v99x[screenWidth_18062C * v94 + v31];
			v33x = &v100x[screenWidth_18062C * v94 + v31]; //(char*)(v100 + v31 + x_DWORD_18062C_resolution_x * v94);
			v34 = ColourLookupTable_F6EE0[COLOUR_BLEND_LOOKUP_OFFSET + 256 * *v32x + (uint8_t)(*xadataclrd0dat.colorPalette_var28)[4095]];
			*v33x = v34;
			*v32x = v34;
		}
	}
	for (iy = &world_D41A0.entities_0x6E8E[1]; iy <= &world_D41A0.entities_0x6E8E[0x3e8 - 1]; iy++)
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
					if (gameState_D41A4.colorIndex_121[2])
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
						v36 = Entities_EA3E4[iy->id_0x1A_26]->playerData_0xA4_164x->playerColorIndex_0x38_56 + 58;
					}
					else if (v55 == 3)
					{
						if (iy->id_0x1A_26 == v102x->id_0x1A_26 || v93)
							v36 = Entities_EA3E4[iy->id_0x1A_26]->playerData_0xA4_164x->playerColorIndex_0x38_56 + 66;
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
				if (v49 >= 0xB4u && (v49 <= 0xB4u || v49 >= 0xE8u && v49 <= 0xEAu) || iy->flags_0xc_12_15.byte[0] & 1)
					break;
				v50 = iy->id_0x1A_26;
				if (v50 != iy - world_D41A0.entities_0x6E8E)
				{
					v52 = playersColors_E88E0x[GetTrueWizardNumber_61790(Entities_EA3E4[v50]->playerData_0xA4_164x->playerColorIndex_0x38_56)][1];
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
							//v47 = gameState_D41A4.byteindex_121[3] == 0;
							GetTrueWizardNumber_61790(v46x->playerData_0xA4_164x->playerColorIndex_0x38_56);
						}
						else
						{
							if (!(buildingParams_D93C0[iy->levelPar3_0x46_70].flags_2 & 8))
								v104 = 21;
							v105 = 1;
						}
					}
				LABEL_57:
					v44x = Entities_EA3E4[iy->id_0x1A_26];
					if (v44x->class_0x3F_63 == 3)
						v43 = playersColors_E88E0x[GetTrueWizardNumber_61790(v44x->playerData_0xA4_164x->playerColorIndex_0x38_56)][0];
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
						if (gameState_D41A4.colorIndex_121[3])
							v42 = playersColors_E88E0x[GetTrueWizardNumber_61790(v41x->playerData_0xA4_164x->playerColorIndex_0x38_56)][0];
						else
							v42 = playersColors_E88E0x[GetTrueWizardNumber_61790(v41x->playerData_0xA4_164x->playerColorIndex_0x38_56)][1];
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
						if (iy->leaderId_0x32_50 != world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240)
							break;
						v43 = playersColors_E88E0x[GetTrueWizardNumber_61790(v102x->playerData_0xA4_164x->playerColorIndex_0x38_56)][gameState_D41A4.colorIndex_121[3]];
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
				if (iy->flags_0xc_12_15.byte[0] & 1)
					break;
				v43 = (*xadataclrd0dat.colorPalette_var28)[3840];
				v104 = 20;
				goto LABEL_117;
			case 0xCu:
				if (iy->model_0x40_64 != 5)
					break;
				if (gameState_D41A4.colorIndex_121[3])
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
			v59 = v98 + projection.X(v57, v58);
			if (v59 >= 0 && v59 < width)
			{
				v60 = projection.Y(v57, v58) + v85;
				if (v60 >= 0 && v60 < height && v59 >= mapSpans_F4960[1 + 2 * v60] && v59 < mapSpans_F4960[2 * v60])
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
						else if (videoMode_180660 == 1)
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
											x + 2 * v59 - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v36].width_4 / 2,
											y + 2 * v60 - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v36].height_5 / 2,
											(*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v36]);
								}
								else
								{
									//DrawBitmap_2BB40(2 * v59, 2 * v60 - *(uint8_t *)(**filearray_2aa18c[6] + 6 * v36 + 5), (uint8_t**)(**filearray_2aa18c[6] + 6 * v36));
									DrawBitmap_2BB40(x + 2 * v59, y + 2 * v60 - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v36].height_5, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v36]);
								}
							}
							else
							{
								/*DrawBitmap_2BB40(
								  2 * v59 - *(uint8_t *)(6 * v36 + **filearray_2aa18c[6] + 4) / 2,
								  2 * v60 - *(uint8_t *)(6 * v36 + **filearray_2aa18c[6] + 5),
									(uint8_t**)(6 * v36 + **filearray_2aa18c[6]));*/
								DrawBitmap_2BB40(
									x + 2 * v59 - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v36].width_4 / 2,
									y + 2 * v60 - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v36].height_5,
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
								x + v59 - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v36].width_4 / 2,
								y + v60 - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v36].height_5,
								(*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v36]);
						}
						else
						{
							//DrawBitmap_2BB40(v59, v60 - *(uint8_t *)(6 * v36 + **filearray_2aa18c[6] + 5), (uint8_t**)(6 * v36 + **filearray_2aa18c[6]));
							DrawBitmap_2BB40(x + v59, y + v60 - (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v36].height_5, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[v36]);
						}
					}
					if (v104)
						SetHelpItemTarget_885E0(iy, (int16_t)(v8 + v59), (int16_t)(v9 + v60), v104);   // v8, v9: the map's position
				}
			}
		}
	}
	if (v93)
	{
		v90x = Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
		for (j = 0; j < world_D41A0.NumberOfPlayers_0xe; j++)
		{
			if (j != world_D41A0.LevelIndex_0xc)
			{
				//v96 = 2124 * j;
				v65 = world_D41A0.players_0x2BDE[j].playerIndex_0x00a_2BE4_11240;
				v66x = Entities_EA3E4[v65];
				if (v66x->life_0x8 >= 0)
				{
					if (CanSeePlayerOnMinimap_63570(v90x, Entities_EA3E4[v65]))
					{
						v67 = (int16_t)(v66x->position_0x4C_76.x - posX);
						v68 = (int16_t)(v66x->position_0x4C_76.y - posY);
						v69 = v98 + projection.X(v67, v68);
						if (v69 >= 0 && v69 < width)
						{
							v70 = projection.Y(v67, v68) + v85;
							if (v70 >= 0 && v70 < height && v69 >= mapSpans_F4960[1 + 2 * v70] && v69 < mapSpans_F4960[2 * v70])
							{
								v73 = playersColors_E88E0x[GetTrueWizardNumber_61790(v66x->playerData_0xA4_164x->playerColorIndex_0x38_56)][1];
								DrawMinimapPlayerDot(v99x, v100x, v69, v70, width, height, v73, scale, false);
								if (videoMode_180660 == 1)
									DrawText_2BC10(world_D41A0.players_0x2BDE[j].WizardName_0x39f_2BFA_12157, x + 2 * v69 + 4, y + 2 * v70, v73);
								else
									DrawText_2BC10(world_D41A0.players_0x2BDE[j].WizardName_0x39f_2BFA_12157, x + v69 - CalculateScaleOffset(scale) + (4 * scale), y + v70 - CalculateScaleOffset(scale), v73, scale);
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
	index = (char*)&ColourLookupTable_F6EE0[COLOUR_BLEND_LOOKUP_OFFSET + 256 * v106];
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
		v79 = ColourLookupTable_F6EE0[COLOUR_BLEND_LOOKUP_OFFSET + 256 * v106 + (uint8_t)*v78];
		*v78 = v79;
		*v95 = v79;
		v80 = ColourLookupTable_F6EE0[COLOUR_BLEND_LOOKUP_OFFSET + 256 * v106 + (uint8_t)*v101];
		*v101 = v80;
		*v84 = v80;
		v81 = ColourLookupTable_F6EE0[COLOUR_BLEND_LOOKUP_OFFSET + 256 * v106 + (uint8_t)*v88];
		*v88 = v81;
		*v83 = v81;
		v82 = ColourLookupTable_F6EE0[COLOUR_BLEND_LOOKUP_OFFSET + 256 * v106 + (uint8_t)*(v76 - 1)];
		*(v76 - 1) = v82;
		*(v75 - 1) = v82;
		v76--;
		v75--;
	}
	//return result;
}

//----- (00063570) --------------------------------------------------------
char CanSeePlayerOnMinimap_63570(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//244570
{
	type_entity_0x6E8E* v2x; // ecx
	char v3; // ah
	type_entity_0x6E8E* v4x; // ebx
	char v5; // dl
	uint8_t v6; // al

	v2x = Entities_EA3E4[a1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[12]];
	v3 = 1;
	v4x = Entities_EA3E4[a2x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[4]];
	v5 = 0;
	if (v4x > Entities_EA3E4[0] && v4x->stage_0x2E_46)
		v5 = 1;
	if (v2x > Entities_EA3E4[0])
	{
		v6 = v2x->levelPar3_0x46_70;
		if (v6 < 1u)
		{
			if (v6)
				return v3;
			if (a2x->playerData_0xA4_164x->invisibleTime_0x1BF_447)
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
	if (PackedGameData())//regression tests without the game: needs TABLES?.DAT (ColourLookupTable_F6EE0), only reads the state
		return;
	if (world_D41A0.m_GameSettings.m_Display.m_uiScreenSize == 1)
		DrawMapLowRes_63670(x, y, posX, posY, width, height, yaw, scaling, a10);
	else
		DrawMapHighRes_63C90(x, y, posX, posY, width, height, yaw, scaling, a10);
}

//----- (00063670) --------------------------------------------------------
void DrawMapLowRes_63670(int16_t x, int16_t y, int16_t posX, int16_t posY, uint16_t width, uint16_t height, int16_t yaw, int16_t scaling, int a10)//244670
{
	if (videoMode_180660 & 1)
	{
		x = x >> 1;
		y = y >> 1;
		width = width >> 1;
		height = height >> 1;
		scaling *= 2;
	}

	if (width == 0 || height == 0)
		return;

	uint8_t* ptrScreenBuffer = &pdwScreenBuffer_351628[y * screenWidth_18062C + x];
	uint8_t* ptrMemoryBuffer = &ptrMemoryBuffer_E9C3C[y * screenWidth_18062C + x];

	BuildMapRowSpansLowRes(a10 != 0, width, height);

	if (!CommandLineParams.DoUseOldMapDrawing())
		DrawMapLowResWalk<MAP_WALK_FRAC_BITS>(ptrScreenBuffer, ptrMemoryBuffer, height, GetFixedMapWalk(posX, posY, width, height, yaw, scaling), a10);
	else
		DrawMapLowResWalk<0>(ptrScreenBuffer, ptrMemoryBuffer, height, GetMapWalk(posX, posY, width, height, yaw, scaling), a10);
}

MapProjection::MapProjection(int cosCoef, int sinCoef, int16_t yaw, int scaling)
	: m_cosCoef(cosCoef), m_sinCoef(sinCoef),
	m_cos(Maths::sin_DB750[0x200 + (yaw & 0x7FF)]), m_sin(Maths::sin_DB750[yaw & 0x7FF]),
	m_divisor((int64_t)scaling << 16)
{
}

// Rounds towards minus infinity, the divisor is positive
static int FloorDiv(int64_t value, int64_t divisor)
{
	int64_t result = value / divisor;
	if (value % divisor < 0)
		result--;
	return (int)result;
}

int MapProjection::X(int dx, int dy) const
{
	if (CommandLineParams.DoUseOldMapDrawing())
		return (dx * m_cosCoef - dy * m_sinCoef) >> 16;
	return FloorDiv(dx * m_cos + dy * m_sin, m_divisor);
}

int MapProjection::Y(int dx, int dy) const
{
	if (CommandLineParams.DoUseOldMapDrawing())
		return (m_cosCoef * dy + m_sinCoef * dx) >> 16;
	return FloorDiv(dy * m_cos - dx * m_sin, m_divisor);
}

// The original walk: steps truncated to whole world units, the column step stretched by height / width
// (the full screen map is not square, its marks are), pixels sampled in their top left corner
MapWalk GetMapWalk(int16_t posX, int16_t posY, int width, int height, int16_t yaw, int scaling)
{
	const int angle = yaw & 0x7FF;
	const int sinScaled = (scaling * Maths::sin_DB750[angle]) >> 16;                      // v80 / v70
	const int cosScaled = (scaling * (signed int)Maths::sin_DB750[0x200 + angle]) >> 16;  // v81y / v69

	const int hSin = height * sinScaled;
	const int hCos = height * cosScaled;

	MapWalk walk;
	walk.du = hCos / width;                                     // v66 / v58
	walk.dv = hSin / width;                                     // v67 / v59
	walk.startU = posX - (width * walk.du - hSin) / 2;          // v85 / v72
	walk.startV = posY - (hCos + width * walk.dv) / 2;          // v83 / v73
	// Per row: u moves by -sin, v moves by +cos
	walk.rowStepU = -sinScaled;
	walk.rowStepV = cosScaled;
	return walk;
}

// fixedMapDrawing: the same scale in both directions, the coordinates carry MAP_WALK_FRAC_BITS bits of
// a world unit, and pixel (i, j) shows the world at its centre (i + 0.5 - width / 2, j + 0.5 - height / 2),
// i.e. exactly the point MapProjection places into that pixel
MapWalk GetFixedMapWalk(int16_t posX, int16_t posY, int width, int height, int16_t yaw, int scaling)
{
	const int angle = yaw & 0x7FF;
	// world units << 16 per pixel
	const int64_t sinStep = (int64_t)scaling * Maths::sin_DB750[angle];
	const int64_t cosStep = (int64_t)scaling * Maths::sin_DB750[0x200 + angle];
	const int shift = 16 - MAP_WALK_FRAC_BITS;
	const int64_t half = (int64_t)1 << (shift - 1);

	// twice the offset of the centre of pixel (0, 0) from the map centre
	const int64_t col2 = 1 - 2 * (width / 2);
	const int64_t row2 = 1 - 2 * (height / 2);

	MapWalk walk;
	walk.du = (int)((cosStep + half) >> shift);
	walk.dv = (int)((sinStep + half) >> shift);
	walk.rowStepU = -walk.dv;
	walk.rowStepV = walk.du;
	walk.startU = ((int)(uint16_t)posX << MAP_WALK_FRAC_BITS) + (int)((col2 * cosStep - row2 * sinStep + 2 * half) >> (shift + 1));
	walk.startV = ((int)(uint16_t)posY << MAP_WALK_FRAC_BITS) + (int)((col2 * sinStep + row2 * cosStep + 2 * half) >> (shift + 1));
	return walk;
}

template <int FracBits>
void DrawMapLowResWalk(uint8_t* ptrScreenBuffer, uint8_t* ptrMemoryBuffer, int height, const MapWalk& walk, int a10)
{
	// Cave-level blending is only allowed when transparency is off and it isn't a Cave map
	const bool caveBlend = !world_D41A0.m_GameSettings.display_0x2196.transparency_0x2198
		&& world_D41A0.terrain_2FECE.MapType != MapType_t::Cave;

	if (isCaveLevel_D41B6)
	{
		if (!a10 && caveBlend)
			DrawMapRowsLowRes<true, true, false, FracBits>(ptrScreenBuffer, ptrMemoryBuffer, height, walk.startU, walk.startV, walk.du, walk.dv, walk.rowStepU, walk.rowStepV);
		else
			DrawMapRowsLowRes<true, false, false, FracBits>(ptrScreenBuffer, ptrMemoryBuffer, height, walk.startU, walk.startV, walk.du, walk.dv, walk.rowStepU, walk.rowStepV);
	}
	else
	{
		if (a10)
			DrawMapRowsLowRes<false, false, false, FracBits>(ptrScreenBuffer, ptrMemoryBuffer, height, walk.startU, walk.startV, walk.du, walk.dv, walk.rowStepU, walk.rowStepV);
		else
			DrawMapRowsLowRes<false, true, true, FracBits>(ptrScreenBuffer, ptrMemoryBuffer, height, walk.startU, walk.startV, walk.du, walk.dv, walk.rowStepU, walk.rowStepV);
	}
}

void BuildMapRowSpansLowRes(bool fullRows, int width, int height)
{
	int16_t* spans = mapSpans_F4960;

	if (fullRows)
	{
		for (int r = 0; r < height; r++)
		{
			spans[2 * r] = (int16_t)width;
			spans[2 * r + 1] = 0;
		}
		return;
	}

	const int half = height / 2;
	if (half <= 0)
		return;

	const int halfWidth = width >> 1;
	const int step = 0x1000000 / half;
	int16_t* upper = &spans[2 * half];
	int16_t* lower = upper - 2;
	int fixed = 0;

	for (int k = 0; k < half; k++)
	{
		// NOTE: index is (0x14300 + fixed) >> 16, exactly as in the original listing.
		// DrawMapHighRes uses 0x14300 + (fixed >> 16). See the note below.
		const int d = (halfWidth * (uint8_t)ColourLookupTable_F6EE0[(0x14300 + fixed) >> 16]) >> 8;
		upper[0] = lower[0] = (int16_t)(halfWidth + d);
		upper[1] = lower[1] = (int16_t)(halfWidth - d);
		upper += 2;
		lower -= 2;
		fixed += step;
	}
}

template <bool Masked, bool Blend, bool WriteMemory, int FracBits>
void DrawMapRowsLowRes(uint8_t* ptrScreenBuffer, uint8_t* ptrMemoryBuffer, int height,
	int rowU, int rowV, int du, int dv, int rowStepU, int rowStepV)
{
	const int16_t* span = mapSpans_F4960;

	for (int row = 0; row < height; row++, span += 2)
	{
		const int start = span[1];
		int count = span[0] - start;

		uint8_t* dst = ptrScreenBuffer + start;
		uint8_t* mem = ptrMemoryBuffer + start;
		int u = rowU + start * du;
		int v = rowV + start * dv;

		for (; count > 0; count--, dst++, mem++, u += du, v += dv)
		{
			// Map cell: x = bits 8..15 of u, y = bits 8..15 of v (above the FracBits fraction bits)
			const unsigned idx = (((unsigned)u >> (8 + FracBits)) & 0xFFu) | (((unsigned)v >> FracBits) & 0xFF00u);

			if (Masked && (mapAngle_13B4E0[idx] & 8))
			{
				if (!Blend) { *dst = 0; SetAlphaAt(dst, ALPHA_HIDDEN); }
				continue;
			}

			const unsigned terrainColour = ColourLookupTable_F6EE0[MAP_COLOUR_BLEND_LOOKUP_OFFSET + mapTerrainType_10B4E0[idx]];
			const uint8_t colour = ColourLookupTable_F6EE0[((unsigned)mapShading_12B4E0[idx] << 8) | terrainColour];

			if (Blend)
			{
				const uint8_t result = ColourLookupTable_F6EE0[COLOUR_BLEND_LOOKUP_OFFSET + (((unsigned)*dst << 8) | colour)];
				*dst = result;
				SetAlphaAt(dst, ALPHA_BLENDED);
				if (WriteMemory) *mem = result;
			}
			else
			{
				*dst = colour;
				SetAlphaAt(dst, ALPHA_VISIBLE);
			}
		}

		ptrScreenBuffer += screenWidth_18062C;
		ptrMemoryBuffer += screenWidth_18062C;
		rowU += rowStepU;
		rowV += rowStepV;
	}
}

void BuildMapRowSpans(bool fullRows, int width, int height)
{
	int16_t* spans = mapSpans_F4960;

	if (fullRows)
	{
		for (int r = 0; r < height; r++)
		{
			spans[2 * r] = (int16_t)width;
			spans[2 * r + 1] = 0;
		}
		return;
	}

	// Elliptical mask, filled outwards from the middle
	const int half = height / 2;
	const int halfWidth = width >> 1;
	int16_t* upper = &spans[2 * half];
	int16_t* lower = upper - 2;
	int fixed = 0;

	for (int k = 0; k < half; k++)
	{
		const int d = (halfWidth * ColourLookupTable_F6EE0[0x14300 + (fixed >> 16)]) >> 8;
		upper[0] = lower[0] = (int16_t)(halfWidth + d);
		upper[1] = lower[1] = (int16_t)(halfWidth - d);
		upper += 2;
		lower -= 2;
		fixed += 0x1000000 / half;
	}
}

template <bool Masked, bool Blend, int FracBits>
void DrawMapRows(uint8_t* ptrScreenBuffer, int height, int rowU, int rowV,
	int du, int dv, int rowStepU, int rowStepV)
{
	const int16_t* span = mapSpans_F4960;

	for (int row = 0; row < height; row++, span += 2)
	{
		const int start = span[1];
		int count = span[0] - start;

		uint8_t* dst = ptrScreenBuffer + start;
		int u = rowU + start * du;
		int v = rowV + start * dv;

		for (; count > 0; count--, dst++, u += du, v += dv)
		{
			// Map cell: x = bits 8..15 of u, y = bits 8..15 of v (above the FracBits fraction bits)
			const unsigned idx = (((unsigned)u >> (8 + FracBits)) & 0xFFu) | (((unsigned)v >> FracBits) & 0xFF00u);

			if (Masked && (mapAngle_13B4E0[idx] & 8))
			{
				if (!Blend)
				{
					*dst = 0;
					SetAlphaAt(dst, ALPHA_HIDDEN);   // masked cave pixel
				}
				continue;                            // Blend: screen untouched, alpha untouched
			}

			const unsigned terrainColour = ColourLookupTable_F6EE0[MAP_COLOUR_BLEND_LOOKUP_OFFSET + mapTerrainType_10B4E0[idx]];
			const uint8_t colour = ColourLookupTable_F6EE0[((unsigned)mapShading_12B4E0[idx] << 8) | terrainColour];

			if (Blend)
			{
				*dst = ColourLookupTable_F6EE0[COLOUR_BLEND_LOOKUP_OFFSET + (((unsigned)*dst << 8) | colour)];
				SetAlphaAt(dst, ALPHA_BLENDED);
			}
			else
			{
				*dst = colour;
				SetAlphaAt(dst, ALPHA_VISIBLE);
			}
		}

		ptrScreenBuffer += screenWidth_18062C;
		rowU += rowStepU;
		rowV += rowStepV;
	}
}

void DrawMapHighRes_63C90(int16_t x, int16_t y, int16_t posX, int16_t posY, uint16_t width, uint16_t height, int16_t yaw, int16_t scaling, int a10)//244c90
{
	if (videoMode_180660 & 1)
	{
		x = x >> 1;
		y = y >> 1;
		width = width >> 1;
		height = height >> 1;
		scaling *= 2;
	}

	if (width == 0 || height == 0)
		return;

	uint8_t* ptrScreenBuffer = &pdwScreenBuffer_351628[screenWidth_18062C * y + x];

	BuildMapRowSpans(a10 != 0, width, height);

	if (!CommandLineParams.DoUseOldMapDrawing())
		DrawMapHighResWalk<MAP_WALK_FRAC_BITS>(ptrScreenBuffer, height, GetFixedMapWalk(posX, posY, width, height, yaw, scaling), a10);
	else
		DrawMapHighResWalk<0>(ptrScreenBuffer, height, GetMapWalk(posX, posY, width, height, yaw, scaling), a10);
}

template <int FracBits>
void DrawMapHighResWalk(uint8_t* ptrScreenBuffer, int height, const MapWalk& walk, int a10)
{
	// Pick the variant once
	const bool masked = isCaveLevel_D41B6;

	bool blend = !a10;
	if (masked && blend)
	{
		blend = !world_D41A0.m_GameSettings.display_0x2196.transparency_0x2198
			&& world_D41A0.terrain_2FECE.MapType != MapType_t::Cave;
	}

	if (masked)
	{
		if (blend) DrawMapRows<true, true, FracBits>(ptrScreenBuffer, height, walk.startU, walk.startV, walk.du, walk.dv, walk.rowStepU, walk.rowStepV);
		else       DrawMapRows<true, false, FracBits>(ptrScreenBuffer, height, walk.startU, walk.startV, walk.du, walk.dv, walk.rowStepU, walk.rowStepV);
	}
	else
	{
		if (blend) DrawMapRows<false, true, FracBits>(ptrScreenBuffer, height, walk.startU, walk.startV, walk.du, walk.dv, walk.rowStepU, walk.rowStepV);
		else       DrawMapRows<false, false, FracBits>(ptrScreenBuffer, height, walk.startU, walk.startV, walk.du, walk.dv, walk.rowStepU, walk.rowStepV);
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
		comp20 = compare_with_sequence((char*)"002454F0-003514B0", (uint8_t*)& helpPopup_1804B0, 0x3514b0, debugcounter_644F0, 0xab, 0xab, &origbyte20, &remakebyte20);
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
	const int uiX = x;
	const int uiY = y;
	if (videoMode_180660 & 1)
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
	v51x.yaw_47 = yaw;
	v51x.scaling_49 = scaling;
	v82 = yaw & 0x7FF;
	v13 = world_D41A0.LevelIndex_0xc;
	v75x = Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
	v14 = GetObjectiveStage_595C0(v13);
	switch (v14 + 3)
	{
	case 0:
	case 1:
	case 2:
		if (v14 == -3)
		{
			for (ix = gameState_D41A4.miscList_38535; ; ix = ix->next_0)
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
			for (ix = gameState_D41A4.miscList_38535; ; ix = ix->next_0)
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
		//LOBYTE(i) = (uint8_t)GetObjectiveTargetEntity_59610(v72x, D41A0_BYTESTR_0.word_0xc);
		if (GetObjectiveTargetEntity_59610(&v72x, world_D41A0.LevelIndex_0xc))
		{
			v51x.v64xb_46 = (*xadataclrd0dat.colorPalette_var28)[3840];
			v51x.v51y = v72x->position_0x4C_76;
			if (DrawObjectiveRectangle_64CE0(&v51x, scale))
				SetHelpItemTarget_885E0(v72x, x + v51x.v62xw_42, y + v51x.v63xw_44, 0x52u);
			v36x = &v72x->position_0x4C_76;
			v17x = &v72x->position_0x4C_76;
			v16x = &v75x->position_0x4C_76;
			v81 = 1;
			goto LABEL_47;
		}
		break;
	case 8: //Draw Objective Rectangle
		//LOBYTE(i) = GetObjectiveTargetPosition_596C0(v65x, D41A0_BYTESTR_0.word_0xc);
		if (GetObjectiveTargetPosition_596C0(&v65y, world_D41A0.LevelIndex_0xc))
		{
			v51x.v64xb_46 = (*xadataclrd0dat.colorPalette_var28)[0xff0];
			v51x.v51y = v65y;
			if (DrawObjectiveRectangle_64CE0(&v51x, scale))
				SetHelpItemTarget_885E0(0, x + v51x.v62xw_42, y + v51x.v63xw_44, 0x51u);
			v36x = &v65y;
			v17x = &v65y;
			v16x = &v75x->position_0x4C_76;
			v81 = 1;
		LABEL_47:
			v67x = *v17x;
			//i = (uint8_t*)Maths::DistanceXY_58490(v16x, v36x);
			v8 = Maths::DistanceXY_58490(v16x, v36x);
		}
		break;
	case 0xA: //Markers around Enemies
		v18 = world_D41A0.stages_0x3654C[world_D41A0.playerObjectives_0x3659C[world_D41A0.LevelIndex_0xc].substr_3659C.ObjectiveText_1].target_36552.dword;
		v51x.v64xb_46 = (*xadataclrd0dat.colorPalette_var28)[3840];
		for (ix = gameState_D41A4.creatureLists_38403x[v18]; ; ix = ix->next_0)
		{
			v72x = ix;
			if (ix <= Entities_EA3E4[0])
				break;
			v51x.v51y = v72x->position_0x4C_76;
			if (DrawObjectiveRectangle_64CE0(&v51x, scale))
				SetHelpItemTarget_885E0(v72x, x + v51x.v62xw_42, y + v51x.v63xw_44, 0x52u);
			v19 = Maths::DistanceXY_58490(&v75x->position_0x4C_76, &v72x->position_0x4C_76);
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
		v76 = world_D41A0.stages_0x3654C[world_D41A0.playerObjectives_0x3659C[world_D41A0.LevelIndex_0xc].substr_3659C.ObjectiveText_1].target_36552.dword;
		for (ix = gameState_D41A4.buildingList_38527; ; ix = ix->next_0)
		{
			v72x = ix;
			if (ix <= Entities_EA3E4[0])
				break;
			v79 = v76;
			for (j = 0; v79 && j < 8u; j++)
			{
				if (v72x->levelPar3_0x46_70 == v79)
				{
					v51x.v51y = v72x->position_0x4C_76;
					if (DrawObjectiveRectangle_64CE0(&v51x, scale))
						SetHelpItemTarget_885E0(v72x, x + v51x.v62xw_42, y + v51x.v63xw_44, 0x52u);
					v20 = Maths::DistanceXY_58490(&v75x->position_0x4C_76, &v72x->position_0x4C_76);
					if (v20 < v8)
					{
						v8 = v20;
						v81 = 1;
						v67x = v72x->position_0x4C_76;
					}
				}
				v79 = buildingParams_D93C0[v79].nextIndex_3;
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
		if (gameState_D41A4.FrameTimingIndex_26 & 0x40)
		{
			if (gameState_D41A4.colorIndex_121[6])
			{
				v21 = v8 - 512;
				LOWORD(v78) = Maths::GetYawAngle_581E0(&v75x->position_0x4C_76, &v67x);
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
				const MapProjection arrowProjection(v23, v24, yaw, scaling);
				v25 = width / 2 + arrowProjection.X(v71, (int16_t)((int16_t)v67x.y - posY));
				v37x[7] = 13 * scale;
				v37x[13] = 13 * scale;
				v37x[19] = 13 * scale;
				v37x[37] = 13 * scale;
				v37x[6] = 9 * scale;
				v77 = height / 2 + arrowProjection.Y(v71, (int16_t)((int16_t)v67x.y - posY));
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
				if (videoMode_180660 & 1)
				{
					v25 *= 2;
					v77 *= 2;
				}
				colour_v30 = (*xadataclrd0dat.colorPalette_var28)[3840];
				v31 = 0;
				while (v31 < 7)
				{
					v32 = 3 * v31;
					v37x[2 * v32] = v25 + uiX + v37x[6 * v31];
					v33 = v37x[1 + (6 * v31++)];
					v37x[1 + 2 * v32] = v77 + uiY + v33;
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
	const MapProjection projection(a1->v54xdw_10, a1->v55xdw_14, a1->yaw_47, a1->scaling_49);
	if (!CommandLineParams.DoUseOldMapDrawing())
		posX_v4 = a1->v60xdw_34 + projection.X(v1, v2);
	else
		posX_v4 = a1->v60xdw_34 + ((int32_t)(v1 * a1->v54xdw_10 - v2 * a1->v55xdw_14) >> 16);
	colour_v5 = a1->v64xb_46;
	if (posX_v4 >= 0 && posX_v4 < a1->v58xdw_26)
	{
		if (!CommandLineParams.DoUseOldMapDrawing())
			posY_v6 = a1->v61xdw_38 + projection.Y(v1, v2);
		else
			posY_v6 = a1->v61xdw_38 + (int16_t)((a1->v54xdw_10 * v2 + a1->v55xdw_14 * v1) >> 16);
		if (posY_v6 >= 3 && posY_v6 < a1->v59xdw_30 - 3 && posX_v4 >= mapSpans_F4960[1 + 2 * posY_v6] + 3 && posX_v4 < mapSpans_F4960[2 * posY_v6] - 3)
		{
			a1->v62xw_42 = posX_v4;
			a1->v63xw_44 = posY_v6;
			v3 = 1;
			if (!(gameState_D41A4.FrameTimingIndex_26 & 3))
			{
				DrawRectangle(pdwScreenBuffer_351628, posX_v4 - scale - CalculateScaleOffset(scale), posY_v6 - scale - CalculateScaleOffset(scale), 3 * scale, 3 * scale, scale, screenWidth_18062C, colour_v5);

				if (1 == world_D41A0.m_GameSettings.m_Display.m_uiScreenSize)
				{
					screenWidth_v7 = screenWidth_18062C;
					startY_v8 = screenWidth_18062C * (posY_v6 - scale);
					startX_v9 = posX_v4 - scale;

					v13 = (x_BYTE*)(startY_v8 + startX_v9 + ptrMemoryBuffer_E9C3C);
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
void CloseHelpPopup_87C10()//268c10
{
	if (gameState_D41A4.showHelp_10)
	{
		helpPopup_1804B0.dword_0x6a = 0;
		helpPopup_1804B0.PopupStatusByte_0x9e &= 0xDFu;
		helpPopup_1804B0.popupFlags_0x9f &= 0xFEu;
		helpPopup_1804B0.blinkCounter_0x72 = helpPopup_1804B0.blinkPeriod_0x6e;
		str_E2A74[helpPopup_1804B0.Index_0x86].axis_2[0] &= 0xffFDu;
		str_E2A74[helpPopup_1804B0.Index_0x86].counter_16 = 0;
		str_E2A74[helpPopup_1804B0.Index_0x86].entity_12 = 0;
		str_E2A74[helpPopup_1804B0.Index_0x86].stage_29 = str_E2A74[helpPopup_1804B0.Index_0x86].resetStage_28;
		UpdateHelpItemState_889F0(helpPopup_1804B0.Index_0x86);
		helpPopup_1804B0.Index_0x86 = 0;
	}
}

int debugcounter_2695e0 = 0;

//----- (000885E0) --------------------------------------------------------
void SetHelpItemTarget_885E0(type_entity_0x6E8E* a1x, int16_t posX, int16_t posY, uint16_t a4)//2695e0
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
	if (gameState_D41A4.showHelp_10)
	{
		//LOBYTE(v4) = a4;
		if (!(str_E2A74[a4].axis_2[0] & 8))
		{
			if (videoMode_180660 & 1)
			{
				posX *= 2;
				posY *= 2;
			}
			//LOBYTE(v4) = a4;
			//v6 = 30 * (int16_t)a4;
			if (str_E2A74[a4].axis_2[0] & 2)
			{
				if (a1x == str_E2A74[a4].entity_12 && !(*(int16_t*)&helpPopup_1804B0.PopupStatusByte_0x9e & 0x20))
				{
					//v9 = array_E2A74[0x2 + 30 * a4] | 0x10;
					str_E2A74[a4].axis_2[3] = posX;
					str_E2A74[a4].axis_2[0] |= 0x10;
					//LOBYTE(v4) = a3;
					//v10 = HIBYTE(*(int16_t*)&helpPopup_1804B0.PopupStatusByte_0x9e) | 2;
					str_E2A74[a4].axis_2[4] = posY;
					helpPopup_1804B0.popupFlags_0x9f |= 2;
				}
			}
			else
			{
				v12 = 0;
				if (!a1x
					|| (v4 = Maths::DistanceXY_58490(
						&Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240]->position_0x4C_76,
						&a1x->position_0x4C_76),
						v5 = v4,
						!str_E2A74[a4].entity_12)
					|| v4 < str_E2A74[a4].countdown_20)
				{
					v12 = 1;
				}
				if (v12)
				{
					//v7 = 15 * (int16_t)a4;
					str_E2A74[a4].entity_12 = a1x;//xx
					//v8 = array_E2A74[0x2 + 30 * a4];
					str_E2A74[a4].countdown_20 = v5;
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
	// The original treats the coordinates as signed 16-bit
	int x1 = (int16_t)posX1, y1 = (int16_t)posY1;
	int x2 = (int16_t)posX2, y2 = (int16_t)posY2;

	const int maxX = clipWidth_180648 - 1;
	const int maxY = clipHeight_180644 - 1;

	// Trivial reject: both endpoints outside on the same side
	if ((x1 < 0 && x2 < 0) || (x1 > maxX && x2 > maxX) ||
		(y1 < 0 && y2 < 0) || (y1 > maxY && y2 > maxY))
		return;

	uint8_t* origin = &pdwScreenBuffer_351628[viewportX_18063C + screenWidth_18062C * viewportY_180650];

	// Horizontal fast path (y is in range thanks to the reject above)
	if (y1 == y2)
	{
		const int xa = (std::max)((std::min)(x1, x2), 0);
		const int xb = (std::min)((std::max)(x1, x2), maxX);
		uint8_t* p = origin + screenWidth_18062C * y1 + xa;
		memset(p, colour, xb - xa + 1);
		if (pdwScreenAlphaBuffer)
			memset(pdwScreenAlphaBuffer + (p - pdwScreenBuffer_351628), ALPHA_VISIBLE, xb - xa + 1);
		return;
	}

	// General case: Bresenham with per-pixel clipping
	const int dx = std::abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
	const int dy = -std::abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
	int err = dx + dy;

	for (;;)
	{
		if ((unsigned)x1 <= (unsigned)maxX && (unsigned)y1 <= (unsigned)maxY)
		{
			uint8_t* p = origin + screenWidth_18062C * y1 + x1;
			*p = colour;
			SetAlphaAt(p, ALPHA_VISIBLE);
		}
		if (x1 == x2 && y1 == y2)
			break;
		const int e2 = 2 * err;
		if (e2 >= dy) { err += dy; x1 += sx; }
		if (e2 <= dx) { err += dx; y1 += sy; }
	}
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
void RestoreMousePosition_548B0(type_str_0x2BDE* a1x)//2358b0
{
	if (a1x->playerSlot_0x007_2BE4_11237 == world_D41A0.LevelIndex_0xc)
	{
		SetMousePositionInMemory_5BDC0(a1x->playerData_0x3E6_2BE4_12228.position_backup_20.x, a1x->playerData_0x3E6_2BE4_12228.position_backup_20.y);
		// if a joystick is used, do not set that random resting point from above
		//SetMousePositionInMemory_5BDC0(320, 240);
	}
}

//----- (000548F0) --------------------------------------------------------
void BackupMousePosition_548F0(type_str_0x2BDE* a1x)//2358f0
{
	if (a1x->playerSlot_0x007_2BE4_11237 == world_D41A0.LevelIndex_0xc)
	{
		if (inputState_18058C.joystickMode_1805C2 != 7 && inputState_18058C.joystickMode_1805C2 != 1 && inputState_18058C.joystickMode_1805C2 != 2)
		{
			a1x->playerData_0x3E6_2BE4_12228.position_backup_20.x = inputState_18058C.mouse_1805B0.x;
			a1x->playerData_0x3E6_2BE4_12228.position_backup_20.y = inputState_18058C.mouse_1805B0.y;
		}
		else
		{
			a1x->playerData_0x3E6_2BE4_12228.position_backup_20 = mouse_E3760;
		}
	}
}

//----- (0001A280) --------------------------------------------------------
void SetOptionsMenuFlags_1A280()//1fb280
{
	gameState_D41A4.optionsMenuFlags2_192 = 0;
	gameState_D41A4.optionsMenuFlags2_192 |= 0x01;//help
	if (soundAble_E3798)
		gameState_D41A4.optionsMenuFlags2_192 |= 0x02;//sound
	if (musicAble_E37FC)
		gameState_D41A4.optionsMenuFlags2_192 |= 0x04;//music
	if (cdSpeechEnabled_E2A28)
		gameState_D41A4.optionsMenuFlags2_192 |= 0x08;//speek
	gameState_D41A4.optionsMenuFlags2_192 |= 0xF0u;//fly,bright,speed,screen size
	if (world_D41A0.visualSettings_0x21AA.creflections_0x21AA)
		gameState_D41A4.optionsMenuFlags2_192 |= 0x100;//reflections
	if (world_D41A0.visualSettings_0x21AA.cshadows_0x21AB)
		gameState_D41A4.optionsMenuFlags2_192 |= 0x400;//shadows
	if (world_D41A0.visualSettings_0x21AA.csky_0x21AC)
		gameState_D41A4.optionsMenuFlags2_192 |= 0x200;//sky
	if (world_D41A0.detailSettings_0x21B6.clights_0x21B6)
		gameState_D41A4.optionsMenuFlags2_192 |= 0x800;//lights
	gameState_D41A4.optionsMenuFlags2_192 |= 0x1000;//icons
	if (world_D41A0.detailSettings_0x21B6.ctransparency_0x21B8)
		gameState_D41A4.optionsMenuFlags2_192 |= 0x2000;//transparency
	if (world_D41A0.detailSettings_0x21B6.cflat_0x21B9)
		gameState_D41A4.optionsMenuFlags2_192 |= 0x4000;//flat
	if (world_D41A0.screenSettings_0x21B2.cresolution_0x21B4)
		gameState_D41A4.optionsMenuFlags2_192 |= 0x8000;//resolution
	gameState_D41A4.optionsMenuFlags2_192 |= 0x10000;//names
	gameState_D41A4.optionsMenuFlags_188 = 0;
	if (gameState_D41A4.showHelp_10)
		gameState_D41A4.optionsMenuFlags_188 |= 0x01;//help
	if (soundActive_E3799)
		gameState_D41A4.optionsMenuFlags_188 |= 0x02;//sound
	if (musicActive_E37FD)
		gameState_D41A4.optionsMenuFlags_188 |= 0x04;//music
	if (gameState_D41A4.OptionsSettingFlag_24 & SPEECH_ENABLED)
		gameState_D41A4.optionsMenuFlags_188 |= 0x08;//speech
	if (world_D41A0.flyAssistant_0x36DEA)
		gameState_D41A4.optionsMenuFlags_188 |= 0x10u;//fly
	gameState_D41A4.optionsMenuFlags_188 |= 0x20;//bright
	if (gameState_D41A4.speedIndex)
		gameState_D41A4.optionsMenuFlags_188 |= 0x40;//speed
	gameState_D41A4.optionsMenuFlags_188 |= 0x80u;//screen size
	if (world_D41A0.m_GameSettings.m_Graphics.m_wReflections)
		gameState_D41A4.optionsMenuFlags_188 |= 0x100;//reflections
	if (world_D41A0.m_GameSettings.m_Graphics.m_wShadows)
		gameState_D41A4.optionsMenuFlags_188 |= 0x400;//shadows
	if (world_D41A0.m_GameSettings.m_Graphics.m_wSky)
		gameState_D41A4.optionsMenuFlags_188 |= 0x200;//sky
	if (world_D41A0.m_GameSettings.display_0x2196.m_wDynamicLighting)
		gameState_D41A4.optionsMenuFlags_188 |= 0x800;//lights
	if (world_D41A0.m_GameSettings.m_Display.m_wMiniMap)
		gameState_D41A4.optionsMenuFlags_188 |= 0x1000;//icons
	if (world_D41A0.m_GameSettings.display_0x2196.transparency_0x2198)
		gameState_D41A4.optionsMenuFlags_188 |= 0x2000;//transparency
	if (world_D41A0.m_GameSettings.display_0x2196.flat_0x2199)
		gameState_D41A4.optionsMenuFlags_188 |= 0x4000;//flat
	if (world_D41A0.m_GameSettings.graphics_0x2192.m_wResolution)
		gameState_D41A4.optionsMenuFlags_188 |= 0x8000;//resolution
	if (!gameState_D41A4.byteindex_207)
		gameState_D41A4.optionsMenuFlags_188 |= 0x10000;//names
}

//----- (0001A4A0) --------------------------------------------------------
int GetOptionsMenuWidth_1A4A0()//1fb4a0
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
		GetOptionsLangStrings_1A5B0(i + 1, &v6, &v5);
		v2 = strlen((const char*)langStrings_E9C4C[v6]) + 1;
		//Help Mode On and etc.
		if ((signed int)(8 * (v2 - 1)) > v0)
			v0 = 8 * (v2 - 1);
		v3 = strlen((const char*)langStrings_E9C4C[v5]) + 1;
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

	int16_t helpHeight;   // the bottom of the spell menu
	if (UseInGameUiLayout())
		scale = gameUiScale;
	GetSpellMenuPosition(&offsetPosX, &helpHeight);

	if (a1x->playerSlot_0x007_2BE4_11237 == world_D41A0.LevelIndex_0xc)
	{
		if (a1x->MenuState_0x3DF_2BE4_12221 == 5 || a1x->MenuState_0x3DF_2BE4_12221 == 8)
		{
			selectedSpellIndex = a1x->playerData_0x3E6_2BE4_12228.str_611.spellIndex_0x458_1112;
			if (selectedSpellIndex < 13)
				spellIdxX = selectedSpellIndex;
			else
				spellIdxX = selectedSpellIndex - 13;

			spellIconHeight = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL].height_5 * scale;
			v5 = a1x->playerData_0x3E6_2BE4_12228.str_611.selectSpellType_0x457_1111;
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
					subCategoryIndex = a1x->playerData_0x3E6_2BE4_12228.str_611.selectedSubSpells_0x437_1079x.SpellIndex[spellIndex_D94FF[a1x->playerData_0x3E6_2BE4_12228.str_611.spellIndex_0x458_1112]];

					posX = posX + (subCategoryWidth * subCategoryIndex
						+ (subCategoryWidth / 2)
						+ (8 * scale)
						+ (subCategoryPosX));

					if (inputState_18058C.joystickMode_1805C2 != 7 && inputState_18058C.joystickMode_1805C2 != 1 && inputState_18058C.joystickMode_1805C2 != 2)
					{
						inputState_18058C.mouse_1805B0.x = (signed __int16)posX;
						inputState_18058C.mouse_1805B0.y = (signed __int16)posY;
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
					+ spellIconHeight * (a1x->playerData_0x3E6_2BE4_12228.str_611.spellIndex_0x458_1112 >= 13);

				if (inputState_18058C.joystickMode_1805C2 != 7 && inputState_18058C.joystickMode_1805C2 != 1 && inputState_18058C.joystickMode_1805C2 != 2)
				{
					inputState_18058C.mouse_1805B0.x = (signed __int16)posX;
					inputState_18058C.mouse_1805B0.y = (signed __int16)posY;
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
signed __int16 IsCdPresent_10010()
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

	if (cdChecked_D3F48)
		return 1;
#ifdef _MSC_VER
	dos_getdrive(&v5);
	if (!dos_getdiskfree(v1, 0, v5, &v2) && v2 == -1 && !v3 && v4 == 134217729)
	{
		v0 = v4;
		cdChecked_D3F48 = 1;
	}
	return v0;
#else
	cdChecked_D3F48 = 1;
	return 0;
#endif
}

//----- (000889F0) --------------------------------------------------------
void UpdateHelpItemState_889F0(uint16_t a1)//2699f0
{
	if (a1 < 0x15u)
	{
		if (a1 < 0x12u)
		{
			if (a1 == 15)
			{
				str_E2A74[16].stage_29 = 6;
				return;
			}
			goto LABEL_22;
		}
		if (a1 > 0x12u)
		{
			if (a1 != 19)
				goto LABEL_22;
			helpPopup_1804B0.PopupStatusByte_0x9e |= 0x80;
			return;
		}
		helpPopup_1804B0.PopupStatusByte_0x9e |= 0x40;
		return;
	}
	if (a1 <= 0x15u)
	{
		helpPopup_1804B0.PopupStatusByte_0x9e |= 0x40;
		return;
	}
	if (a1 < 0x1Cu)
	{
		if (a1 < 0x19u || a1 > 0x1Au)
			goto LABEL_22;
		{
			helpPopup_1804B0.PopupStatusByte_0x9e |= 0x80;
			return;
		}
	}
	if (a1 <= 0x1Cu)
	{
		helpPopup_1804B0.PopupStatusByte_0x9e |= 0x80;
		return;
	}
	if (a1 < 0x52u)
	{
	LABEL_22:
		if (str_E2A74[a1].type_0 == 7)
		{
			switch (world_D41A0.stages_0x3654C[world_D41A0.playerObjectives_0x3659C[world_D41A0.LevelIndex_0xc].substr_3659C.ObjectiveText_1].stages_3654C_byte0)
			{
			case 0:
				helpPopup_1804B0.PopupStatusByte_0x9e |= 0x40;
				break;
			case 1:
			case 2:
			case 3:
			case 7:
			case 8:
			case 9:
				helpPopup_1804B0.PopupStatusByte_0x9e |= 0x80;
				break;
			default:
				break;
			}
		}
		if (a1 == 79)
		{
			str_E2A74[2].stage_29 = 6;
			str_E2A74[81].stage_29 = 6;
		}
		return;
	}
	if (a1 <= 0x52u)
	{
		helpPopup_1804B0.PopupStatusByte_0x9e |= 0x80;
		return;
	}
	if (a1 != 93)
		goto LABEL_22;
	if (Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240]->playerData_0xA4_164x->CastleEntityIndex_0x3A_58)
		helpPopup_1804B0.pendingHelpIndex_0x88 = 94;
}

//----- (0005BDC0) --------------------------------------------------------
void SetMousePositionInMemory_5BDC0(int16_t posX, int16_t posY)//23cdc0
{
	if (CommandLineParams.DoShowDebugPerifery())ShowPerifery();

	if (posX < 0)
		posX = 0;
	if (posY < 0)
		posY = 0;

	mousePosX_E375C = posX;
	mouse_E3760.x = posX;
	mousePosY_E375E = posY;
	mouse_E3760.y = posY;
	inputState_18058C.mouseX_1805B8 = posX;
	inputState_18058C.mouse_1805B0.x = posX;
	inputState_18058C.mouseY_1805BC = posY;
	inputState_18058C.mouse_1805B0.y = posY;
	if (*(uint32_t*)&externControl_E36C4[0])
	{
		*(uint32_t*)&externControl_E36C4[40] = posY;
		*(uint32_t*)&externControl_E36C4[36] = inputState_18058C.mouse_1805B0.x;
	}
	if (videoMode_180660 & 8)
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
void GetOptionsLangStrings_1A5B0(int a1, int* a2, int* a3)//1fb5b0
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
	if (videoMode_180660 != 1)
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

	if (gameState_D41A4.SelectedMenuItem_38546)
	{
		switch (gameState_D41A4.SelectedMenuItem_38546)
		{
			case 1:
			{
				sprintf(printbuffer, "%s?", langStrings_E9C4C[2]);//Abandon level.
				break;
			}
			case 2:
			{
				sprintf(printbuffer, "%s?", langStrings_E9C4C[423]);//Load Level
				break;
			}
			case 3:
			{
				sprintf(printbuffer, "%s?", langStrings_E9C4C[424]);//Save Level
				break;
			}
		}

		v4 = 4080 * gameState_D41A4.colorIndex_121[4];
		DrawText_2BC10(printbuffer, posTextX * scale, posTextY * scale, (*xadataclrd0dat.colorPalette_var28)[256 * ((signed int)(v4 - (__CFSHL__(HIDWORD(v4), 8) + (HIDWORD(v4) << 8))) >> 8)], scale);
		GetOkayCancelButtonPositions_30BE0(&posX, &posY, scale);
		DrawBitmap_2BB40(posX, posY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BUTTON_OK1], scale);
		DrawBitmap_2BB40(posX + (50 * scale), posY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BUTTON_CANCEL1], scale);
		if (inputState_18058C.joystickMode_1805C2 == 8
			|| inputState_18058C.joystickMode_1805C2 == 12
			|| inputState_18058C.joystickMode_1805C2 == 13
			|| inputState_18058C.joystickMode_1805C2 == 4
			|| inputState_18058C.joystickMode_1805C2 == 6
			|| inputState_18058C.joystickMode_1805C2 == 11
			|| inputState_18058C.joystickMode_1805C2 == 5)
		{
			DrawBitmap_2BB40(inputState_18058C.mouse_1805B0.x, inputState_18058C.mouse_1805B0.y, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[cursorSpriteIndex_D419E], scale);
		}
	}
	EventDispatcher::I->DispatchEvent(EventType::E_SCENE_CHANGE, Scene::FLIGHT_MENU);
}

//----- (00052D70) --------------------------------------------------------
void ShowMessage_52D70(uint16_t playerIndex, char* cheatMessage)//233d70
{
	strcpy(world_D41A0.players_0x2BDE[playerIndex].CurrentNotificationText_0x01c_2BFA_11258, cheatMessage);
	world_D41A0.players_0x2BDE[playerIndex].CurrentNotificationDuration_0x04d_2C2B_11307 = 100;
	world_D41A0.players_0x2BDE[playerIndex].endState_0x04f_2C2D_11309 = 1;
}

//----- (0002F6B0) --------------------------------------------------------
void DrawChatMenu_2F6B0(uint8_t scale)//2106b0
{
	int16_t chatPosX;
	int16_t chatPosY;
	GetChatMenuCoordinates(&chatPosX, &chatPosY, scale);

	uint8_t mapType = static_cast<std::underlying_type<MapType_t>::type>(world_D41A0.terrain_2FECE.MapType);
	uint8_t backColour = frameColors_D94F0[mapType][0];
	uint8_t boxColour = frameColors_D94F0[mapType][1];
	uint8_t lightColour = frameColors_D94F0[mapType][2];
	uint8_t darkColour = frameColors_D94F0[mapType][3];
	uint8_t textColour = (*xadataclrd0dat.colorPalette_var28)[4095];
	uint8_t blinkColour = (*xadataclrd0dat.colorPalette_var28)[3840];
	uint8_t numberColour = (*xadataclrd0dat.colorPalette_var28)[0];
	uint8_t nameColour = (*xadataclrd0dat.colorPalette_var28)[240];
	int hoveredPlayer = -1;
	int16_t mouseX = mouse_E3760.x;
	int16_t mouseY = mouse_E3760.y;

	//Panel and frame
	DrawLine_2BC80(chatPosX, chatPosY, CHAT_MENU_WIDTH * scale, CHAT_MENU_HEIGHT * scale, backColour);
	DrawLine_2BC80(chatPosX, chatPosY, CHAT_MENU_WIDTH * scale, 2 * scale, lightColour);
	DrawLine_2BC80(chatPosX, chatPosY + (70 * scale), CHAT_MENU_WIDTH * scale, 2 * scale, darkColour);
	DrawLine_2BC80(chatPosX, chatPosY, 2 * scale, 70 * scale, lightColour);
	DrawLine_2BC80(chatPosX + (638 * scale), chatPosY, 2 * scale, CHAT_MENU_HEIGHT * scale, darkColour);

	//Line shown in the text box: the one under the mouse in the 4x2 line grid, else the selected one
	uint8_t shownLine = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].chatLineIndex_0x3E0_2BE4_12222;
	if (mouseX >= chatPosX + (4 * scale)
		&& mouseX < chatPosX + (66 * scale)
		&& mouseY >= chatPosY + (28 * scale)
		&& mouseY < chatPosY + (66 * scale))
	{
		shownLine = 4 * ((mouseY - (chatPosY + (28 * scale))) / (20 * scale)) + ((mouseX - (chatPosX + (4 * scale))) / (16 * scale));
	}

	//Text box
	int16_t boxPosX = chatPosX + (4 * scale);
	int16_t boxPosY = chatPosY + (4 * scale);
	DrawLine_2BC80(boxPosX, boxPosY, 372 * scale, 20 * scale, boxColour);
	DrawLine_2BC80(boxPosX, boxPosY, 372 * scale, 2 * scale, darkColour);
	DrawLine_2BC80(boxPosX, boxPosY + (18 * scale), 372 * scale, 2 * scale, lightColour);
	DrawLine_2BC80(boxPosX, boxPosY, 2 * scale, 18 * scale, darkColour);
	DrawLine_2BC80(boxPosX + (370 * scale), boxPosY, 2 * scale, 20 * scale, lightColour);
	sprintf(printbuffer, "%s", world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].names_81[shownLine]);
	DrawText_2BC10(printbuffer, boxPosX + (6 * scale), boxPosY + (2 * scale), textColour, scale);

	//Blinking text cursor
	DrawLine_2BC80(
		boxPosX + (6 * scale) + ((8 * scale) * strlen(printbuffer)),
		boxPosY + (4 * scale),
		6 * scale,
		12 * scale,
		(gameState_D41A4.FrameTimingIndex_26 & 8) ? blinkColour : boxColour);

	//Chat line buttons 1-8, two rows of four
	int16_t btnPosX = chatPosX + (4 * scale);
	int16_t btnPosY = chatPosY + (28 * scale);
	for (int i = 0; i < 8; i++)
	{
		if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].chatLineIndex_0x3E0_2BE4_12222 == i)
			DrawBitmap_2BB40(btnPosX, btnPosY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_MINISQUARE1_WITH_FRAME], scale);//930
		else
			DrawBitmap_2BB40(btnPosX, btnPosY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_MINISQUARE1], scale);//948
		sprintf(printbuffer, "%d", i + 1);
		DrawText_2BC10(printbuffer, btnPosX + (4 * scale), btnPosY + (2 * scale), numberColour, scale);
		if (i == 3)
		{
			btnPosX = chatPosX + (4 * scale);
			btnPosY += (20 * scale);
		}
		else
		{
			btnPosX += (16 * scale);
		}
	}

	//Chat state buttons (recipients mode)
	btnPosX = chatPosX + (82 * scale);
	btnPosY = chatPosY + (32 * scale);
	for (int i = 0; i < 4; i++)
	{
		if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].chatState_0x3E1_2BE4_12223 == i)
			DrawBitmap_2BB40(btnPosX, btnPosY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL_WITH_FRAME], scale);
		else
			DrawBitmap_2BB40(btnPosX, btnPosY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL], scale);
		DrawBitmap_2BB40(btnPosX + (2 * scale), btnPosY + (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[154 - i], scale);
		//82, 152, 222, then a wider gap to 310
		btnPosX += ((i == 2) ? 88 : 70) * scale;
	}

	//Player recipient buttons, the local player is skipped
	bool chatToSelected = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].chatState_0x3E1_2BE4_12223 == 3;
	btnPosX = chatPosX + (384 * scale);
	btnPosY = chatPosY + (26 * scale);
	for (int i = 0; i < 8; i++)
	{
		if (world_D41A0.LevelIndex_0xc == i)
			++i;
		if (chatToSelected && (1 << i) & world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].chatRecipients_0x3E3_2BE4_12225)
			DrawBitmap_2BB40(btnPosX, btnPosY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL_SMALL_WITH_FRAME], scale);
		else
			DrawBitmap_2BB40(btnPosX, btnPosY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL_SMALL], scale);
		DrawBitmap_2BB40(btnPosX + (2 * scale), btnPosY + (2 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[i + INCHAT_PLAYER1], scale);
		if (mouseX >= btnPosX
			&& mouseX < btnPosX + (32 * scale)
			&& mouseY >= btnPosY
			&& mouseY < btnPosY + (22 * scale))
		{
			hoveredPlayer = i;
		}
		btnPosX += (34 * scale);
	}

	//OK, Back and Cancel
	DrawBitmap_2BB40(chatPosX + (384 * scale), chatPosY + (4 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BUTTON_OK2], scale);
	DrawBitmap_2BB40(chatPosX + (474 * scale), chatPosY + (4 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BUTTON_BACK], scale);
	DrawBitmap_2BB40(chatPosX + (564 * scale), chatPosY + (4 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_BUTTON_CANCEL2], scale);

	//Name box for the hovered player
	boxPosX = chatPosX + (384 * scale);
	boxPosY = chatPosY + (50 * scale);
	DrawLine_2BC80(boxPosX, boxPosY, 236 * scale, 18 * scale, boxColour);
	DrawLine_2BC80(boxPosX, boxPosY, 236 * scale, 2 * scale, darkColour);
	DrawLine_2BC80(boxPosX, boxPosY + (16 * scale), 236 * scale, 2 * scale, lightColour);
	DrawLine_2BC80(boxPosX, boxPosY, 2 * scale, 16 * scale, darkColour);
	DrawLine_2BC80(boxPosX + (234 * scale), boxPosY, 2 * scale, 18 * scale, lightColour);
	if (hoveredPlayer != -1)
		DrawText_2BC10(world_D41A0.players_0x2BDE[hoveredPlayer].WizardName_0x39f_2BFA_12157, boxPosX + (6 * scale), boxPosY + (2 * scale), nameColour, scale);

	if (inputState_18058C.joystickMode_1805C2 == 8
		|| inputState_18058C.joystickMode_1805C2 == 12
		|| inputState_18058C.joystickMode_1805C2 == 13
		|| inputState_18058C.joystickMode_1805C2 == 4
		|| inputState_18058C.joystickMode_1805C2 == 6
		|| inputState_18058C.joystickMode_1805C2 == 11
		|| inputState_18058C.joystickMode_1805C2 == 5)
	{
		DrawBitmap_2BB40(inputState_18058C.mouse_1805B0.x, inputState_18058C.mouse_1805B0.y, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[cursorSpriteIndex_D419E], scale);
	}
	EventDispatcher::I->DispatchEvent(EventType::E_SCENE_CHANGE, Scene::CHAT_MENU);
}
// 8E3D5: using guessed type x_DWORD sprintf(x_DWORD, const char *, ...);
// D419E: using guessed type char cursorSpriteIndex_D419E;
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
// 180660: using guessed type __int16 videoMode_180660;

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
	if (gameState_D41A4.OptionsSettingFlag_24 & GAME_PAUSED)
	{
		if (!gameState_D41A4.pauseState_206)
		{
			GetPauseMenuCoordinates_2FFE0(&posX, &posY, &width, &heigth, scale);
			DrawBitmap_2BB40(posX, posY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[MENU_ICON_SAVE], scale);//Save Dialog
			posY += heigth;
			DrawBitmap_2BB40(posX, posY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[MENU_ICON_LOAD], scale);//Load Dialog
			if (!gameState_D41A4.saveFileExists_208)
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
		if (inputState_18058C.joystickMode_1805C2 == 8
			|| inputState_18058C.joystickMode_1805C2 == 12
			|| inputState_18058C.joystickMode_1805C2 == 13
			|| inputState_18058C.joystickMode_1805C2 == 4
			|| inputState_18058C.joystickMode_1805C2 == 6
			|| inputState_18058C.joystickMode_1805C2 == 11
			|| inputState_18058C.joystickMode_1805C2 == 5)
		{
			DrawBitmap_2BB40(inputState_18058C.mouse_1805B0.x, inputState_18058C.mouse_1805B0.y, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[cursorSpriteIndex_D419E], scale);
		}
		if (gameState_D41A4.setting_byte4_25 & 0x10)
			DrawLangStringDebugViewer_30870();
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
	
	int optionMenuXPos = (640 - gameState_D41A4.optionsMenuWidth_186) / 2;
	if (videoMode_180660 != 1)
		if (!DefaultResolutions())
		{
			optionMenuXPos = (screenWidth_18062C - (gameState_D41A4.optionsMenuWidth_186 * scale)) / 2;
		    menuBtnPosY = (screenHeight_180624 / 2) - ((OPTIONS_MENU_BTN_COUNT * (OPTIONS_MENU_BTN_HEIGHT * scale)) / 2);
			okayBtnPosY = menuBtnPosY + ((OPTIONS_MENU_BTN_HEIGHT * scale) * OPTIONS_MENU_BTN_COUNT) + (4 * scale);
		}

	while (1)
	{
		v21 = v1;
		if (v1 >= 17)
			break;
		GetOptionsLangStrings_1A5B0(v21 + 1, &v15, &v14);
		v19 = 1 << v21;
		if ((1 << v21) & gameState_D41A4.optionsMenuFlags2_192 && (1 << v21) & gameState_D41A4.optionsMenuFlags_188)
			v3 = v15;
		else
			v3 = v14;
		v18 = (char*)langStrings_E9C4C[v3];
		//help mode On and etc
		v4 = gameState_D41A4.optionsMenuWidth_186 * scale;
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
		if (v19 & gameState_D41A4.optionsMenuFlags2_192)
		{
			if (optionMenuXPos > inputState_18058C.mouse_1805B0.x
				|| optionMenuXPos + (gameState_D41A4.optionsMenuWidth_186 * scale) <= inputState_18058C.mouse_1805B0.x
				|| menuBtnPosY > inputState_18058C.mouse_1805B0.y
				|| menuBtnPosY + (OPTIONS_MENU_BTN_HEIGHT * scale) <= inputState_18058C.mouse_1805B0.y)
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
		DrawText_2BC10((char*)v18, optionMenuXPos + (((gameState_D41A4.optionsMenuWidth_186 * scale) - (8 * scale) * strlen(v18)) >> 1), menuBtnPosY + (2 * scale), textColour, scale);
		menuBtnPosY += (OPTIONS_MENU_BTN_HEIGHT * scale);
		v1 = v21 + 1;
	}

	int okayBtnXPos = (gameState_D41A4.optionsMenuWidth_186 - OPTIONS_MENU_BTN_WIDTH) / 2 + (640 - gameState_D41A4.optionsMenuWidth_186) / 2;
	if (videoMode_180660 != 1)
		if (!DefaultResolutions())
		{
			okayBtnXPos = ((gameState_D41A4.optionsMenuWidth_186 * scale) - (OPTIONS_MENU_BTN_WIDTH * scale)) / 2 + (screenWidth_18062C - (gameState_D41A4.optionsMenuWidth_186 * scale)) / 2;
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

	int okayBtnTextXPos = (640 - gameState_D41A4.optionsMenuWidth_186) / 2 + (gameState_D41A4.optionsMenuWidth_186 - OPTIONS_MENU_BTN_WIDTH) / 2;
	if (videoMode_180660 != 1)
		if (!DefaultResolutions())
		{
			okayBtnTextXPos = (screenWidth_18062C - (gameState_D41A4.optionsMenuWidth_186 * scale)) / 2 + ((gameState_D41A4.optionsMenuWidth_186 * scale) - (OPTIONS_MENU_BTN_WIDTH * scale)) / 2;
		}

	if (okayBtnTextXPos > inputState_18058C.mouse_1805B0.x || okayBtnTextXPos + (OPTIONS_MENU_BTN_WIDTH * scale) <= inputState_18058C.mouse_1805B0.x || inputState_18058C.mouse_1805B0.y < okayBtnPosY || inputState_18058C.mouse_1805B0.y >= (okayBtnPosY + (OPTIONS_MENU_BTN_HEIGHT * scale)))
		v12 = (*xadataclrd0dat.colorPalette_var28)[4095];
	else
		v12 = (*xadataclrd0dat.colorPalette_var28)[3840];
		
	if ((!DefaultResolutions())&& (videoMode_180660 != 1))
		DrawText_2BC10((char*)"OK", okayBtnTextXPos + (33 * scale), okayBtnPosY + (2 * scale), v12, scale);
	else
		DrawText_2BC10((char*)"OK", (640 - gameState_D41A4.optionsMenuWidth_186) / 2 + (gameState_D41A4.optionsMenuWidth_186 - OPTIONS_MENU_BTN_WIDTH) / 2 + 33, 379, v12);	

	EventDispatcher::I->DispatchEvent(EventType::E_SCENE_CHANGE, Scene::FLIGHT_MENU);

	if (inputState_18058C.joystickMode_1805C2 == 8
		|| inputState_18058C.joystickMode_1805C2 == 12
		|| inputState_18058C.joystickMode_1805C2 == 13
		|| inputState_18058C.joystickMode_1805C2 == 4
		|| inputState_18058C.joystickMode_1805C2 == 6
		|| inputState_18058C.joystickMode_1805C2 == 11
		|| inputState_18058C.joystickMode_1805C2 == 5)
	{
		DrawBitmap_2BB40(inputState_18058C.mouse_1805B0.x, inputState_18058C.mouse_1805B0.y, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[cursorSpriteIndex_D419E], scale);
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

	unsigned __int8 color1 = frameColors_D94F0[static_cast<std::underlying_type<MapType_t>::type>(world_D41A0.terrain_2FECE.MapType)][0];
	unsigned __int8 color2 = frameColors_D94F0[static_cast<std::underlying_type<MapType_t>::type>(world_D41A0.terrain_2FECE.MapType)][2];
	unsigned __int8 color3 = frameColors_D94F0[static_cast<std::underlying_type<MapType_t>::type>(world_D41A0.terrain_2FECE.MapType)][3];

	GetPauseMenuCoordinates_2FFE0(&posX, &posY, &width, &height, scale);
	switch (gameState_D41A4.volumeKeyHeld_38591)
	{
	case 1:
		volume = 127;
		index = gameState_D41A4.soundVolume_6;
		break;
	case 2:
		volume = 127;
		index = gameState_D41A4.musicVolume_8;
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
void DrawLangStringDebugViewer_30870()//211870
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
	sprintf(printbuffer, "%d", debugLangIndex_D41D4);
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

	v0 = langStrings_E9C4C[debugLangIndex_D41D4];
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
		if (++debugLangIndex_D41D4 < 1)
			debugLangIndex_D41D4 = 1;
		if (debugLangIndex_D41D4 > 470)
			debugLangIndex_D41D4 = 470;
		LastPressedKey_1806E4 = 0;
		break;
	case 0x50:
		v1 = debugLangIndex_D41D4 - 1;
		debugLangIndex_D41D4 = v1;
		if (v1 < 1)
			debugLangIndex_D41D4 = 1;
		if (debugLangIndex_D41D4 > 470)
			debugLangIndex_D41D4 = 470;
		LastPressedKey_1806E4 = 0;
		break;
	case 0x4b:
		debugLangIndex_D41D4 = 1;
		LastPressedKey_1806E4 = 0;
		break;
	case 0x4d:
		debugLangIndex_D41D4 = 470;
		//		LOBYTE(v1) = 0;
		LastPressedKey_1806E4 = 0;
		break;
	}
	//	return v1;
}

// For code that already has a pointer into pdwScreenBuffer_351628
static inline void SetAlphaAt(const uint8_t* screenPixel, uint8_t alpha)
{
	if (pdwScreenAlphaBuffer)
		pdwScreenAlphaBuffer[screenPixel - pdwScreenBuffer_351628] = alpha;
}

// For code that has x/y coordinates
static inline void SetAlphaXY(int x, int y, uint8_t alpha)
{
	if (pdwScreenAlphaBuffer)
		pdwScreenAlphaBuffer[screenWidth_18062C * y + x] = alpha;
}

static inline void FillAlphaRect(int x, int y, int w, int h, uint8_t alpha)
{
	if (!pdwScreenAlphaBuffer) return;
	for (int j = y; j < y + h; j++)
		memset(&pdwScreenAlphaBuffer[screenWidth_18062C * j + x], alpha, w);
}


//----- (0002BBB0) --------------------------------------------------------
void DrawBitmapToScreenAndBuffer_2BBB0(int16_t posX, int16_t posY, bitmap_pos_struct_t a3, uint8_t scale)//20cbb0
{
	//int result; // eax
	uint8_t* temp_screen_buffer; // edi

	ptrDrawBitmap_F01EC(posX, posY, a3, scale);
	//result = (int)x_D41A0_BYTEARRAY_0;
	if (world_D41A0.m_GameSettings.m_Display.m_uiScreenSize == 1)
	{
		temp_screen_buffer = pdwScreenBuffer_351628;
		pdwScreenBuffer_351628 = (uint8_t*)ptrMemoryBuffer_E9C3C;
		/*result = */ptrDrawBitmap_F01EC(posX, posY, a3, scale);
		pdwScreenBuffer_351628 = temp_screen_buffer;
	}
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// E9C3C: using guessed type int ptrMemoryBuffer_E9C3C;
// F01EC: using guessed type int (*ptrDrawBitmap_F01EC)(x_DWORD, x_DWORD, x_DWORD);
// 180628: using guessed type int pdwScreenBuffer_351628;

/*
void debugtestscreen() {
	//save
#ifdef OLD_PARTICLES
	bool save = true;
#else
	bool save = false;
#endif

	if (save)
	{
		FILE* file = fopen("c:/prenos/remc2/testscreen.dat", "wb");
		fwrite(pdwScreenBuffer_351628, 320 * 200, 1, file);
		fclose(file);
	}
	else
	{
		uint8_t buffer[320 * 200];
		FILE* file = fopen("c:/prenos/remc2/testscreen.dat", "rb");
		fread(buffer, 320 * 200, 1, file);
		for (int i = 0; i < 320 * 200; i++)
		{
			if (pdwScreenBuffer_351628[i] != buffer[i])
			{
				myprintf("screen compare error:\n%X\n", i);
				break;
			}
		}
		fclose(file);
	}

	//compare
}
*/

//----- (0002BE30) --------------------------------------------------------
void DrawGameFrame_2BE30()//20CE30
{
	int16_t spellLeftPosX;
	int16_t spellRightPosX;
	int16_t spellIconsPosY;
	int16_t topBarPosX;
	int16_t topBarPosY;
	uint8_t scale = 1;

	type_entity_0x6E8E* playerEntity;
	void (*drawBitmapFunction)(int16_t, int16_t, bitmap_pos_struct_t, uint8_t scale); // eax

	SetTextBoxMinMaxForSetResolution();

	if (videoMode_180660 & 1)
		ClearScreenAlphaBuffer(320, 200, 0);
	else
		ClearScreenAlphaBuffer(screenWidth_18062C, screenHeight_180624, 0);

	if (UseInGameUiLayout())
		scale = gameUiScale;
	GetSpellIconsPosition(&spellLeftPosX, &spellRightPosX, &spellIconsPosY);
	GetTopBarPosition(&topBarPosX, &topBarPosY);

	x_DWORD_D41C8 = 0;
	if (world_D41A0.m_GameSettings.display_0x2196.transparency_0x2198)
	{
		if (videoMode_180660 & 1)
			drawBitmapFunction = DrawBitmap320_8F8B0;
		else
			drawBitmapFunction = DrawBitmap640_8F8E8;
		ptrDrawBitmap_F01E8 = drawBitmapFunction;
	}
	else
	{
		ptrDrawBitmap_F01E8 = GameBitmap::DrawTransparentBitmap_2DE80;
	}
	ptrDrawBitmap_F01EC = ptrDrawBitmap_F01E8;
	ptrDrawBitmap_F01E8 = DrawBitmapToScreenAndBuffer_2BBB0;
	if (gameState_D41A4.showHelp_10)
		ResetHelpItemFlags_88580();
	int actPlayerIndex = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].ActPlayerIndex_0x00e_2BDE_11244;

	uint8_t* help_ScreenBuffer = nullptr;
	if (CommandLineParams.DoTestRenderers()) {
		help_ScreenBuffer = (uint8_t*)malloc(screenWidth_18062C * screenHeight_180624);
		renderer_tests_frame_count++;
	}
	switch (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221)
	{
	case 0:
	case 3:
	case 5:
	case 9:
	case 0xA:
	case 0xD:
		playerEntity = Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
		if (redrawFrame_D41C4 || world_D41A0.m_GameSettings.m_Graphics.m_wViewPortSize != oldViewPortSize_D41C6)
		{
			redrawFrame_D41C4 = 0;
			oldViewPortSize_D41C6 = world_D41A0.m_GameSettings.m_Graphics.m_wViewPortSize;
			if (world_D41A0.m_GameSettings.m_Graphics.m_wViewPortSize < 40)
			{
				if (videoMode_180660 & 1)
					ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, uiBackGroundColorIdx_EB3A8);
				else
					ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, screenWidth_18062C, screenHeight_180624, uiBackGroundColorIdx_EB3A8);
			}
		}
		if (world_D41A0.m_GameSettings.m_Graphics.m_wViewPortSize < 40)
		{
			if (videoMode_180660 & 1)
				ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, uiBackGroundColorIdx_EB3A8);
			else
				ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, screenWidth_18062C, screenHeight_180624, uiBackGroundColorIdx_EB3A8);
		}

		viewPort.SetRenderViewPortSize_40C50(world_D41A0.m_GameSettings.m_Graphics.m_wViewPortSize);
		m_ptrGameRender->DrawWorld_411A0(//draw terrain and particles
			world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].axis_2BDE_11695.x,//position of player
			world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].axis_2BDE_11695.y,//position of player
			world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].rotation__2BDE_11701.yaw,//rotation of player z
			world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].axis_2BDE_11695.z + 128,
			world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].rotation__2BDE_11701.pitch,
			world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].rotation__2BDE_11701.roll,
			world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].rotation__2BDE_11701.fov);

		if (CommandLineParams.DoTestRenderers()) {
			memcpy(help_ScreenBuffer, pdwScreenBuffer_351628, screenWidth_18062C * screenHeight_180624);

			std::string help_buffer_name;
			std::string screenbuffer_buffer_name;
			if (typeid(*m_ptrGameRender) == typeid(GameRenderNG))
			{
				delete m_ptrGameRender;
				m_ptrGameRender = nullptr;
				m_ptrGameRender = (GameRenderInterface*)new GameRenderOriginal();
				help_buffer_name = "ScreenBuffer_HD.bmp";
				screenbuffer_buffer_name = "ScreenBuffer_Original.bmp";
			}
			else
			{
				delete m_ptrGameRender;
				m_ptrGameRender = nullptr;
				m_ptrGameRender = (GameRenderInterface*)new GameRenderNG(viewDistanceScale, fogScale, (multiThreadedRender ? numberOfRenderThreads : 0), assignToSpecificCores);
				help_buffer_name = "ScreenBuffer_Original.bmp";
				screenbuffer_buffer_name = "ScreenBuffer_HD.bmp";
			}

			m_ptrGameRender->DrawWorld_411A0(
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].axis_2BDE_11695.x,//position of player
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].axis_2BDE_11695.y,//position of player
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].rotation__2BDE_11701.yaw,//rotation of player z
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].axis_2BDE_11695.z + 128,
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].rotation__2BDE_11701.pitch,
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].rotation__2BDE_11701.roll,
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].rotation__2BDE_11701.fov);

			WriteBufferToBMP(screenWidth_18062C, screenHeight_180624, *xadatapald0dat2.colorPalette_var28, pdwScreenBuffer_351628, screenbuffer_buffer_name.c_str());
			WriteBufferToBMP(screenWidth_18062C, screenHeight_180624, *xadatapald0dat2.colorPalette_var28, help_ScreenBuffer, help_buffer_name.c_str());

			int difference = 0;
			for (int test_compi = 0; test_compi < screenWidth_18062C * screenHeight_180624; test_compi++) {
				if (pdwScreenBuffer_351628[test_compi] != help_ScreenBuffer[test_compi]) {
					difference++;
				}
			}

			if (difference > 0) {
				std::ostringstream screenBufferName;
				screenBufferName << "Level-" << CommandLineParams.GetSetLevel() << "-Frame-" << renderer_tests_frame_count << "-" << screenbuffer_buffer_name;
				std::ostringstream helpScreenBufferName;
				helpScreenBufferName << "Level-" << CommandLineParams.GetSetLevel() << "-Frame-" << renderer_tests_frame_count << "-" << help_buffer_name;

				renderer_tests[CommandLineParams.GetSetLevel()].differences += difference;
				Logger->error("Differences between HD and Original renderer in frame {0}: {1}", renderer_tests_frame_count, difference);
				WriteBufferToBMP(screenWidth_18062C, screenHeight_180624, *xadatapald0dat2.colorPalette_var28, pdwScreenBuffer_351628, screenBufferName.str().c_str());
				WriteBufferToBMP(screenWidth_18062C, screenHeight_180624, *xadatapald0dat2.colorPalette_var28, help_ScreenBuffer, helpScreenBufferName.str().c_str());
			}

			if (typeid(*m_ptrGameRender) == typeid(GameRenderNG))
			{
				delete m_ptrGameRender;
				m_ptrGameRender = nullptr;
				m_ptrGameRender = (GameRenderInterface*)new GameRenderOriginal();
			}
			else
			{
				delete m_ptrGameRender;
				m_ptrGameRender = nullptr;
				m_ptrGameRender = (GameRenderInterface*)new GameRenderNG(viewDistanceScale, fogScale, (multiThreadedRender ? numberOfRenderThreads : 0), assignToSpecificCores);
			}
		}

		if (playerEntity->life_0x8 < 0)
		{
			int menuIndex = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221;
			if (menuIndex == (int)MenuState::SHOW_IN_GAME_OPTIONS)
			{
				DrawInGameOptionsMenu_30050(scale);
				break;
			}
			else if (menuIndex == 10)
			{
				DrawVolumeSettings_303D0();
			}
			else if (menuIndex == (int)MenuState::SHOW_OK_CANCEL_OPTIONS)
			{
				DrawOkCancelMenu_30A60(6, 6, scale);
				break;
			}
			DrawPauseMenu_2FD90(scale);
		}
		else
		{
			if (webSpritesData_D4188 && playerEntity->playerData_0xA4_164x->mobilizeCounter_0x14E_334)
			{
				//Draw Spiders Web
				actPlayerIndex = DrawSpiderWeb();
			}
			if (world_D41A0.m_GameSettings.m_Display.m_wMiniMap)
			{
				DrawMinimap_63600(//draw minimap
					topBarPosX,
					topBarPosY,
					world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].ActPlayerIndex_0x00e_2BDE_11244 + 1].axis_2BDE_11695.x,
					world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].ActPlayerIndex_0x00e_2BDE_11244 + 1].axis_2BDE_11695.y,
					128 * scale,
					128 * scale,
					world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].ActPlayerIndex_0x00e_2BDE_11244 + 1].rotation__2BDE_11701.yaw,
					256 / scale,
					0);
				DrawMinimapEntites_61880(//draw entites in minimap
					topBarPosX,
					topBarPosY,
					world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].ActPlayerIndex_0x00e_2BDE_11244 + 1].axis_2BDE_11695.x,
					world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].ActPlayerIndex_0x00e_2BDE_11244 + 1].axis_2BDE_11695.y,
					128 * scale,
					128 * scale,
					world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].ActPlayerIndex_0x00e_2BDE_11244 + 1].rotation__2BDE_11701.yaw,
					256 / scale,
					scale);
				DrawMinimapMarks_644F0(
					topBarPosX,
					topBarPosY,
					world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].ActPlayerIndex_0x00e_2BDE_11244 + 1].axis_2BDE_11695.x,
					world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].ActPlayerIndex_0x00e_2BDE_11244 + 1].axis_2BDE_11695.y,
					128 * scale,
					128 * scale,
					world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].ActPlayerIndex_0x00e_2BDE_11244 + 1].rotation__2BDE_11701.yaw,
					256 / scale,
					scale);
			}

			GetFont_6FC50(FontType_D419D);
			if (world_D41A0.m_GameSettings.m_Display.m_wTopBar)
			{

				//Left
				DrawSpellIcon_2E260(
					spellLeftPosX,
					spellIconsPosY,
					Entities_EA3E4[playerEntity->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[playerEntity->playerData_0xA4_164x->str_611.SpellIndexLeft_0x451_1105]],
					false,
					scale);

				//Right
				DrawSpellIcon_2E260(
					spellRightPosX,
					spellIconsPosY,
					Entities_EA3E4[playerEntity->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[playerEntity->playerData_0xA4_164x->str_611.SpellIndexRight_0x453_1107]],
					false,
					scale);

				DrawTopStatusBar_2D710(playerEntity, topBarPosX, topBarPosY, scale);
			}
			switch (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221)
			{
			case (int)MenuState::SHOW_CHAT_MENU:
				DrawChatMenu_2F6B0(scale);
				DrawPauseMenu_2FD90(scale);
				break;
			case (int)MenuState::SHOW_IN_GAME_OPTIONS:
				DrawInGameOptionsMenu_30050(scale);
				break;
			case (int)MenuState::SHOW_VOLUME_OPTIONS:
				DrawVolumeSettings_303D0(scale);
				DrawPauseMenu_2FD90(scale);
				break;
			case (int)MenuState::SHOW_OK_CANCEL_OPTIONS:
				DrawOkCancelMenu_30A60(132, 50, scale);
				break;
			default:
				DrawPauseMenu_2FD90(scale);
				break;
			}
			DrawTextPauseEndOfLevel_2CE30(topBarPosX + (132 * scale), topBarPosY + (50 * scale), scale);
			if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 == (int)MenuState::SHOW_BOTTOM_MENU)
				DrawBottomSpellsMenu_2ECC0();
		}
		break;
	case 6:
	case 7:
	case 8:
	case 0xB:
	case 0xC:
	case 0xE:
		playerEntity = Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
		if (videoMode_180660 & 1)
			ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, uiBackGroundColorIdx_EB3A8);
		else
			ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, screenWidth_18062C, screenHeight_180624, uiBackGroundColorIdx_EB3A8);

		int locViewportPosx;
		int locViewportWidth;
		int locViewportHeight;
		int locMinimapHeight;

		if (videoMode_180660 == 1)
		{
			locViewportPosx = 384;//320x200
			locViewportWidth = 256;
			locViewportHeight = 400;
			locMinimapHeight = 400;
		}
		else
		{
			locViewportPosx = 0.6 * screenWidth_18062C;//bigger than 320x200
			if (locViewportPosx > 384)
				locViewportPosx = 384;
			locViewportWidth = screenWidth_18062C - locViewportPosx;
			locViewportHeight = screenHeight_180624;
			locMinimapHeight = screenHeight_180624;
			if (locMinimapHeight > 400)
				locMinimapHeight = 400;

			if (scale > 1)
			{
				locViewportPosx *= scale;
				locMinimapHeight *= scale;
				locViewportWidth = screenWidth_18062C - locViewportPosx;

				if (locMinimapHeight > screenHeight_180624)
				{
					locMinimapHeight = screenHeight_180624;
				}
			}
		}

		DrawMinimap_63600(
			0,
			0,
			world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].axis_2BDE_11695.x,
			world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].axis_2BDE_11695.y,
			locViewportPosx - 2,
			locMinimapHeight,
			world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].rotation__2BDE_11701.yaw,
			204 / scale,
			1);

		DrawMinimapEntites_61880(
			0,
			0,
			world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].axis_2BDE_11695.x,
			world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].axis_2BDE_11695.y,
			locViewportPosx - 2,
			locMinimapHeight,
			world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].rotation__2BDE_11701.yaw,
			204 / scale,
			scale);

		viewPort.SetRenderViewPortSize_40BF0(locViewportPosx, 0, locViewportWidth, locViewportHeight);

		m_ptrGameRender->DrawWorld_411A0(
			world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].axis_2BDE_11695.x,
			world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].axis_2BDE_11695.y,
			world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].rotation__2BDE_11701.yaw,
			world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].axis_2BDE_11695.z + 128,
			world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].rotation__2BDE_11701.pitch,
			world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].rotation__2BDE_11701.roll,
			world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].rotation__2BDE_11701.fov);

		if (CommandLineParams.DoTestRenderers()) {
			memcpy(help_ScreenBuffer, pdwScreenBuffer_351628, screenWidth_18062C * screenHeight_180624);

			std::string help_buffer_name;
			std::string screenbuffer_buffer_name;
			if (typeid(*m_ptrGameRender) == typeid(GameRenderNG))
			{
				delete m_ptrGameRender;
				m_ptrGameRender = nullptr;
				m_ptrGameRender = (GameRenderInterface*)new GameRenderOriginal();
				help_buffer_name = "ScreenBuffer_HD.bmp";
				screenbuffer_buffer_name = "ScreenBuffer_Original.bmp";
			}
			else
			{
				delete m_ptrGameRender;
				m_ptrGameRender = nullptr;
				m_ptrGameRender = (GameRenderInterface*)new GameRenderNG(viewDistanceScale, fogScale, (multiThreadedRender ? numberOfRenderThreads : 0), assignToSpecificCores);
				help_buffer_name = "ScreenBuffer_Original.bmp";
				screenbuffer_buffer_name = "ScreenBuffer_HD.bmp";
			}

			m_ptrGameRender->DrawWorld_411A0(
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].axis_2BDE_11695.x,//position of player
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].axis_2BDE_11695.y,//position of player
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].rotation__2BDE_11701.yaw,//rotation of player z
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].axis_2BDE_11695.z + 128,
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].rotation__2BDE_11701.pitch,
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].rotation__2BDE_11701.roll,
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].rotation__2BDE_11701.fov);

			WriteBufferToBMP(screenWidth_18062C, screenHeight_180624, *xadatapald0dat2.colorPalette_var28, pdwScreenBuffer_351628, screenbuffer_buffer_name.c_str());
			WriteBufferToBMP(screenWidth_18062C, screenHeight_180624, *xadatapald0dat2.colorPalette_var28, help_ScreenBuffer, help_buffer_name.c_str());

			int difference = 0;
			for (int test_compi = 0; test_compi < screenWidth_18062C * screenHeight_180624; test_compi++) {
				if (pdwScreenBuffer_351628[test_compi] != help_ScreenBuffer[test_compi]) {
					difference++;
				}
			}

			if (difference > 0) {
				renderer_tests[CommandLineParams.GetSetLevel()].differences += difference;
				Logger->error("Differences between HD and Original renderer in frame {0}: {1}", renderer_tests_frame_count, difference);
			}

			if (typeid(*m_ptrGameRender) == typeid(GameRenderNG))
			{
				delete m_ptrGameRender;
				m_ptrGameRender = nullptr;
				m_ptrGameRender = (GameRenderInterface*)new GameRenderOriginal();
			}
			else
			{
				delete m_ptrGameRender;
				m_ptrGameRender = nullptr;
				m_ptrGameRender = (GameRenderInterface*)new GameRenderNG(viewDistanceScale, fogScale, (multiThreadedRender ? numberOfRenderThreads : 0), assignToSpecificCores);
			}
		}

		if (videoMode_180660 & 1)
			SetMouseBoundsScaled_9025C(locViewportPosx, 0, locViewportPosx, locViewportHeight, (unsigned short)(*xadataclrd0dat.colorPalette_var28)[0], 0);
		else
			SetViewportBounds_90374(locViewportPosx, 0, locViewportPosx, locViewportHeight, (unsigned short)(*xadataclrd0dat.colorPalette_var28)[0], 0);
		if (videoMode_180660 & 1)
			SetMouseBoundsScaled_9025C(locViewportPosx - 2, 0, locViewportPosx - 2, locMinimapHeight, (unsigned short)(*xadataclrd0dat.colorPalette_var28)[0], 0);
		else
			SetViewportBounds_90374(locViewportPosx - 2, 0, locViewportPosx - 2, locMinimapHeight, (unsigned short)(*xadataclrd0dat.colorPalette_var28)[0], 0);
		viewPort.SetRenderViewPortSize_40C50(world_D41A0.m_GameSettings.m_Graphics.m_wViewPortSize);
		DrawMinimapMarks_644F0(
			0,
			0,
			world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].axis_2BDE_11695.x,
			world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].axis_2BDE_11695.y,
			locViewportPosx - 2,
			locMinimapHeight,
			world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[actPlayerIndex + 1].rotation__2BDE_11701.yaw,
			204 / scale,
			scale);
		switch (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221)
		{
		case (int)MenuState::SHOW_MAP_SORCERER_SCORES:
			DrawSorcererScores_2D1D0(scale);
			break;
		case (int)MenuState::SHOW_MAP_BOTTOM_MENU:
			DrawPauseMenu_2FD90(scale);
			DrawBottomSpellsMenu_2ECC0();
			break;
		case (int)MenuState::SHOW_MAP_GAME_OPTIONS:
			DrawInGameOptionsMenu_30050(scale);
			break;
		case (int)MenuState::SHOW_MAP_VOLUME_OPTIONS:
			DrawVolumeSettings_303D0();
			DrawPauseMenu_2FD90(scale);
			break;
		case (int)MenuState::SHOW_MAP_OK_CANCEL_OPTIONS:
			DrawOkCancelMenu_30A60(6, 6, scale);
			break;
		default:
			DrawPauseMenu_2FD90(scale);
			break;
		}
		DrawTextPauseEndOfLevel_2CE30(6, 6, scale);
		if (gameState_D41A4.leftSpellPlayerIndex_38400)
			DrawSpellIcon_2E260(
				spellLeftPosX,
				spellIconsPosY,
				Entities_EA3E4[playerEntity->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[playerEntity->playerData_0xA4_164x->str_611.SpellIndexLeft_0x451_1105]],
				false,
				scale);
		if (gameState_D41A4.rightSpellPlayerIndex_38401)
			DrawSpellIcon_2E260(
				spellRightPosX,
				spellIconsPosY,
				Entities_EA3E4[playerEntity->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[playerEntity->playerData_0xA4_164x->str_611.SpellIndexRight_0x453_1107]],
				false,
				scale);
		break;
	default:
		break;
	}
	if (CommandLineParams.DoTestRenderers()) {
		if (help_ScreenBuffer) {
			free(help_ScreenBuffer);
			help_ScreenBuffer = nullptr;
		}
	}
	if (world_D41A0.objectiveBoxCounter_0x36E04)
		DrawCurrentObjectiveTextbox_30630(scale);
	GetFont_6FC50(FontType_D419D);
	if (!(gameState_D41A4.setting_byte1_22 & 4))
		return;

	if (gameState_D41A4.textState_204 == 3)
	{
		if (gameState_D41A4.textDelay_210d <= 0)
			gameState_D41A4.textState_204 = 2;
		else
			gameState_D41A4.textDelay_210d--;// = v26 - 1;
	}
	if (gameState_D41A4.textState_204 == 2)
	{
		gameState_D41A4.textState_204 = 1;
		gameState_D41A4.textLineIndex_214w = 0;
		gameState_D41A4.textDelay_210d = 50;
	}
	if (gameState_D41A4.textState_204 != 1)
		return;

	if (gameState_D41A4.textDelay_210d <= 0)
	{
		while (1)
		{
			if (*off_DB558[gameState_D41A4.textLineIndex_214w] == 33)
				break;
			gameState_D41A4.textLineIndex_214w++;
		}
		gameState_D41A4.textLineIndex_214w++;
		if (*off_DB558[gameState_D41A4.textLineIndex_214w] == 35)
		{
			gameState_D41A4.textState_204 = 3;
			gameState_D41A4.textDelay_210d = 200;
			return;
		}
		gameState_D41A4.textDelay_210d = 50;
		return;
	}
	int height = 380;
	int heightIndex = 0;
	while (*off_DB558[heightIndex + gameState_D41A4.textLineIndex_214w] != 33)
	{
		heightIndex++;
		height -= GetLetterHeight_6FC30();
	}
	int index = 0;
	int actHeight = 8;
	while (heightIndex > 0)
	{
		if (index)
			DrawText_2BC10(off_DB558[index + gameState_D41A4.textLineIndex_214w], 8, actHeight, (*xadataclrd0dat.colorPalette_var28)[0]);
		else
			DrawText_2BC10(off_DB558[gameState_D41A4.textLineIndex_214w], 8, actHeight, (*xadataclrd0dat.colorPalette_var28)[4095]);
		heightIndex--;
		index++;
		actHeight += GetLetterHeight_6FC30();
	}
	gameState_D41A4.textDelay_210d--;
}

int DrawSpiderWeb()
{
	int tilesX, tilesY;

	if (videoMode_180660 & 1)
	{
		tilesX = 4;
		tilesY = 2;
	}
	else
	{
		tilesX = 6;
		tilesY = 4;
	}

	// Pass 1: measure. Row height = tallest tile in the row,
	// web width = widest row, web height = sum of row heights.
	constexpr int kMaxRows = 4;
	int rowHeights[kMaxRows] = {};
	int webWidth = 0;
	int webHeight = 0;

	int index = 1; // sprites are 1-based
	for (int row = 0; row < tilesY; row++)
	{
		int rowWidth = 0;

		for (int col = 0; col < tilesX; col++, index++)
		{
			const auto& tile = webSprites_D4188[index];
			rowWidth += tile.width_4;
			rowHeights[row] = std::max<int>(rowHeights[row], tile.height_5);
		}

		webWidth = (std::max)(webWidth, rowWidth);
		webHeight += rowHeights[row];
	}

	// Largest whole-number scale at which the web fits on screen.
	// Default resolutions keep the original 1x behavior and position.
	int scale = 1;
	int offsetX = 0;
	int offsetY = 0;

	if (videoMode_180660 != 1 && !DefaultResolutions())
	{
		scale = (std::min)(screenWidth_18062C / webWidth, screenHeight_180624 / webHeight);
		scale = (std::max)(scale, 1); // screen smaller than the web: draw 1x

		offsetX = (screenWidth_18062C - webWidth * scale) / 2;
		offsetY = (screenHeight_180624 - webHeight * scale) / 2;
	}

	// Pass 2: draw. Positions are tracked in unscaled web space.
	int tileIndex = 1;
	int y = 0;

	for (int row = 0; row < tilesY; row++)
	{
		int x = 0;

		for (int col = 0; col < tilesX; col++, tileIndex++)
		{
			auto& tile = webSprites_D4188[tileIndex];
			DrawBitmap_2BB40(offsetX + x * scale, offsetY + y * scale, tile, scale);
			x += tile.width_4;
		}

		y += rowHeights[row];
	}

	return tileIndex;
}

//----- (0002CE30) --------------------------------------------------------
void DrawTextPauseEndOfLevel_2CE30(int16_t posX, int16_t posY, uint8_t scale)//20de30
{
	int16_t textPosX;
	int16_t textPosY;
	unsigned __int8 indexedColor; // bl
	int v6; // eax
	//uint8_t* v7; // ebx
	int v7y;
	unsigned __int16 v8; // ax
	//__int16 v9; // ax
	int v10; // eax
	//__int16 v11; // ax
	int v12; // eax
	//__int16 v13; // ax
	__int64 v14; // rax
	unsigned __int8 v15; // al
	int v16; // eax
	//__int16 v17; // ax
	__int64 v18; // rax
	int v19; // eax
	//char v20; // [esp+0h] [ebp-Eh]
	//uint8_t* v21; // [esp+80h] [ebp+72h]
	int v22; // [esp+84h] [ebp+76h]

	textPosY = posY;
	GetFont_6FC50(FontType_D419D);
	//result = (int)x_D41A0_BYTEARRAY_4;
	textPosX = posX;
	indexedColor = (*xadataclrd0dat.colorPalette_var28)[3840];
	if (!gameState_D41A4.pauseToggle_205 && !gameState_D41A4.SelectedMenuItem_38546)
	{
		if (gameState_D41A4.OptionsSettingFlag_24 & GAME_PAUSED)
		{
			DrawText_2BC10(langStrings_E9C4C[425], posX, posY, (*xadataclrd0dat.colorPalette_var28)[240], scale);//Paused!
			textPosX = ((8 * scale) * (strlen((const char*)langStrings_E9C4C[425]) + 2)) + posX;//Paused!
		}

		if (!(gameState_D41A4.setting_byte1_22 & 0x20) && world_D41A0.playerObjectives_0x3659C[world_D41A0.LevelIndex_0xc].substr_3659C.IsLevelEnd_0)
		{
			sprintf(printbuffer, "%s. %s.", langStrings_E9C4C[378], langStrings_E9C4C[379]);//Tasks completed,Fly to the exit point.
			DrawText_2BC10(printbuffer, textPosX, posY, indexedColor, scale);
			textPosX = posX;
			LOWORD(v6) = GetLetterHeight_6FC30() * scale;
			textPosY = v6 + posY;
		}
		v22 = 0;
		//v7 = x_D41A0_BYTEARRAY_0 + 11230;
		v7y = 0;
		while (1)
		{
			//result = (int)x_D41A0_BYTEARRAY_0;
			if (v22 >= world_D41A0.NumberOfPlayers_0xe)
				break;
			v8 = world_D41A0.players_0x2BDE[v7y].endState_0x04f_2C2D_11309 - 1;
			if (v8 <= 4u)
			{
				//v21 = v7x + x_D41A0_BYTEARRAY_0 + 11230 + 28;
				switch (v8)
				{
				case 0u:
					//v9 = *(x_WORD *)(v7x + x_D41A0_BYTEARRAY_0 + 11230 + 77);
					world_D41A0.players_0x2BDE[v7y].CurrentNotificationDuration_0x04d_2C2B_11307--;// = v9 - 1;
					if (world_D41A0.players_0x2BDE[v7y].CurrentNotificationDuration_0x04d_2C2B_11307 + 1 <= 0)
						goto LABEL_24;
					sprintf(printbuffer, "%s %s", world_D41A0.players_0x2BDE[v7y].WizardName_0x39f_2BFA_12157, world_D41A0.players_0x2BDE[v7y].CurrentNotificationText_0x01c_2BFA_11258);
					DrawText_2BC10(printbuffer, textPosX, textPosY, (*xadataclrd0dat.colorPalette_var28)[3840], scale);
					textPosX = posX;
					LOWORD(v10) = GetLetterHeight_6FC30() * scale;
					textPosY += v10;
					break;
				case 1u:
					//v13 = *(x_WORD *)(v7x + x_D41A0_BYTEARRAY_0 + 11230 + 77);
					world_D41A0.players_0x2BDE[v7y].CurrentNotificationDuration_0x04d_2C2B_11307--;
					if (world_D41A0.players_0x2BDE[v7y].CurrentNotificationDuration_0x04d_2C2B_11307 + 1 <= 0)
						goto LABEL_24;
					sprintf(printbuffer, "[%s] %s", world_D41A0.players_0x2BDE[v7y].WizardName_0x39f_2BFA_12157, world_D41A0.players_0x2BDE[v7y].CurrentNotificationText_0x01c_2BFA_11258);
					if (world_D41A0.players_0x2BDE[v7y].CurrentNotificationDuration_0x04d_2C2B_11307 <= 100)
					{
						v15 = (*xadataclrd0dat.colorPalette_var28)[3840];
					}
					else
					{
						v14 = 4080 * gameState_D41A4.colorIndex_121[2];
						v15 = (*xadataclrd0dat.colorPalette_var28)[256 * ((signed int)(v14 - (__CFSHL__(HIDWORD(v14), 8) + (HIDWORD(v14) << 8))) >> 8)];
					}
					DrawText_2BC10(printbuffer, textPosX, textPosY, v15, scale);
					LOWORD(v16) = GetLetterHeight_6FC30() * scale;
					textPosX = posX;
					textPosY += v16;
					break;
				case 2u:
				case 4u:
					//v11 = *(x_WORD *)(v7x + x_D41A0_BYTEARRAY_0 + 11230 + 77);
					world_D41A0.players_0x2BDE[v7y].CurrentNotificationDuration_0x04d_2C2B_11307--;
					if (world_D41A0.players_0x2BDE[v7y].CurrentNotificationDuration_0x04d_2C2B_11307 + 1 <= 0)
						goto LABEL_24;
					if (world_D41A0.players_0x2BDE[v7y].endState_0x04f_2C2D_11309 == 3 || v22 == world_D41A0.LevelIndex_0xc)
					{
						//Draw Selected Spell Name or Spell upgrade notification
						sprintf(printbuffer, "%s", world_D41A0.players_0x2BDE[v7y].CurrentNotificationText_0x01c_2BFA_11258);
						DrawText_2BC10(printbuffer, textPosX, textPosY, (*xadataclrd0dat.colorPalette_var28)[3840], scale);
						textPosX = posX;
						LOWORD(v12) = GetLetterHeight_6FC30() * scale;
						textPosY += v12;
					}
					break;
				case 3u:
					//v17 = *(x_WORD *)(v7x + x_D41A0_BYTEARRAY_0 + 11230 + 77);
					world_D41A0.players_0x2BDE[v7y].CurrentNotificationDuration_0x04d_2C2B_11307--;
					if (world_D41A0.players_0x2BDE[v7y].CurrentNotificationDuration_0x04d_2C2B_11307 + 1 <= 0)
					{
					LABEL_24:
						world_D41A0.players_0x2BDE[v7y].endState_0x04f_2C2D_11309 = 0;
					}
					else
					{
						sprintf(printbuffer, "[%s] %s", world_D41A0.players_0x2BDE[v7y].WizardName_0x39f_2BFA_12157, world_D41A0.players_0x2BDE[v7y].CurrentNotificationText_0x01c_2BFA_11258);
						v18 = 4080 * gameState_D41A4.colorIndex_121[2];
						DrawText_2BC10(printbuffer, textPosX, textPosY, (*xadataclrd0dat.colorPalette_var28)[16 * ((signed int)(v18 - (__CFSHL__(HIDWORD(v18), 8) + (HIDWORD(v18) << 8))) >> 8)]);
						textPosX = posX;
						LOWORD(v19) = GetLetterHeight_6FC30() * scale;
						textPosY += v19;
					}
					break;
				}
			}
			//v7x += 2124;
			v7y++;
			v22++;
		}
	}
	//return result;
}
// 8E3D5: using guessed type x_DWORD sprintf(x_DWORD, const char *, ...);
// D419D: using guessed type char FontType_D419D;
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// E89F0: using guessed type char x_BYTE_E89F0;
// E9800: using guessed type char x_BYTE_E9800;
// EA234: using guessed type int x_DWORD_EA234;
// EA238: using guessed type int x_DWORD_EA238;
// EA2F0: using guessed type int x_DWORD_EA2F0;

//----- (0002D1D0) --------------------------------------------------------
void DrawSorcererScores_2D1D0(uint8_t scale)//20e1d0
{
	signed int v0; // esi
	int v1; // eax
	//uint8_t* v2; // edx
	int v2x;
	int v3; // ebx
	//signed int v4; // edx
	int16_t posY; // ebx
	char v6; // cl
	int v10; // esi
	int blackBarX; // edi
	int v13; // eax
	int v14; // esi
	signed int v15; // eax
	unsigned __int16 v16; // cx
	signed int j; // esi
	int result; // eax
	//char v21; // [esp+0h] [ebp-124h]
	int v22; // [esp+100h] [ebp-24h]
	type_entity_0x6E8E* v23x; // [esp+104h] [ebp-20h]
	//uint8_t* i; // [esp+108h] [ebp-1Ch]
	int ix;
	int v25; // [esp+10Ch] [ebp-18h]
	int barY; // [esp+110h] [ebp-14h]
	int barX; // [esp+114h] [ebp-10h]
	int16_t posX; // [esp+118h] [ebp-Ch]
	uint8_t textColourIdx; // [esp+11Ch] [ebp-8h]
	uint8_t playerColourIdx; // [esp+120h] [ebp-4h]

	int helpWidth = 640;
	int helpHeight;
	if (videoMode_180660 == 1)
		helpHeight = 400;
	else
		helpHeight = 480;
	if (videoMode_180660 != 1)
		if (!DefaultResolutions())
		{
			helpWidth = screenWidth_18062C;
			helpHeight = screenHeight_180624;
		}

	v0 = 0;
	GetFont_6FC50(FontType_D419D);
	v1 = 0;
	//v2 = x_D41A0_BYTEARRAY_0 + 11230;
	v2x = 0;
	while (v0 < world_D41A0.NumberOfPlayers_0xe)
	{
		if (world_D41A0.players_0x2BDE[v2x].playerActive_0x006_2BE4_11236)
			v1++;
		v0++;
		//v2 += 2124;
		v2x++;
	}
	v3 = v1 * ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TILE].height_5 * scale);
	posX = (helpWidth - (v1 * ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TILE_MINI].width_4 * scale) + ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TILE_MINI].width_4) * scale)) / 2;
	posY = (helpHeight - v3) / 2;
	v25 = 0;
	//i=x_D41A0_BYTEARRAY_0 + 11230
	for (ix = 0; ; ix++)
	{
		result = world_D41A0.NumberOfPlayers_0xe;
		if (result <= v25)
			break;
		v6 = world_D41A0.players_0x2BDE[ix].playerActive_0x006_2BE4_11236;
		v23x = Entities_EA3E4[world_D41A0.players_0x2BDE[ix].playerIndex_0x00a_2BE4_11240];
		if (v6 == 1)
		{
			textColourIdx = playersColors_E88E0x[GetTrueWizardNumber_61790(v25)][1];
			playerColourIdx = playersColors_E88E0x[GetTrueWizardNumber_61790(v25)][0];
			DrawBitmap_2BB40(posX, posY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TILE], scale);
			DrawLine_2BC80(
				posX + (4 * scale),
				posY + (4 * scale),
				((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TILE].width_4 * scale) - (8 * scale),
				((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TILE].height_5 * scale) - (8 * scale),
				playerColourIdx);
			v22 = (signed __int16)(posX + (8 * scale));
			DrawText_2BC10(world_D41A0.players_0x2BDE[ix].WizardName_0x39f_2BFA_12157, posX + (8 * scale), posY + (6 * scale), textColourIdx, scale);
			sprintf(printbuffer, "%d", v23x->maxMana_0x8C_140);
			v10 = v25;
			DrawText_2BC10(printbuffer, v22, posY + (20 * scale), textColourIdx, scale);
			if (!v10 && gameState_D41A4.showHelp_10)
			{
				helpPopup_1804B0.scoresPosX_0x96 = posX + (20 * scale);
				helpPopup_1804B0.scoresPosY_0x98 = posY + (30 * scale);
			}
			//HIWORD(v11) = HIWORD(**filearray_2aa18c[6]);
			blackBarX = ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TILE].width_4 * scale) + posX;
			if (gameState_D41A4.showHelp_10 && world_D41A0.NumberOfPlayers_0xe > 1u && !helpPopup_1804B0.wizardNumber_0xa8)
			{
				helpPopup_1804B0.randSeed_0x82 = 9377 * helpPopup_1804B0.randSeed_0x82 + 9439;
				v13 = gameState_D41A4.FrameTimingIndex_26 & 0xFF;
				helpPopup_1804B0.wizardNumber_0xa8 = helpPopup_1804B0.randSeed_0x82 % world_D41A0.NumberOfPlayers_0xe + 1;
				v14 = v13 + helpPopup_1804B0.randSeed_0x82;
				v15 = 1;
				helpPopup_1804B0.randSeed_0x82 = v14;
				//Find next alive player
				while (v15 < 8 && !world_D41A0.players_0x2BDE[v15].playerActive_0x006_2BE4_11236)
				{
					v16 = world_D41A0.NumberOfPlayers_0xe;
					if ((unsigned __int8)++helpPopup_1804B0.wizardNumber_0xa8 >= (signed int)v16)
						helpPopup_1804B0.wizardNumber_0xa8 = 1;
					v15++;
				}
				if ((helpPopup_1804B0.wizardNumber_0xa8 < 0 || helpPopup_1804B0.wizardNumber_0xa8 > world_D41A0.NumberOfPlayers_0xe - 1) ||
					!world_D41A0.players_0x2BDE[helpPopup_1804B0.wizardNumber_0xa8].playerActive_0x006_2BE4_11236)
					helpPopup_1804B0.wizardNumber_0xa8 = 0;
			}
			for (j = 0; j < 8; j++)
			{
				if (world_D41A0.players_0x2BDE[j].playerActive_0x006_2BE4_11236 == 1)
				{
					barX = blackBarX + (4 * scale);
					barY = posY + (4 * scale);
					if (j == v25)
					{
						DrawBitmap_2BB40(blackBarX, posY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TILE_MINI], scale);
						DrawLine_2BC80(
							barX,
							barY,
							((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TILE_MINI].width_4 * scale) - (8 * scale),
							((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TILE_MINI].height_5 * scale) - (8 * scale),
							(*xadataclrd0dat.colorPalette_var28)[0]);
					}
					else
					{
						textColourIdx = playersColors_E88E0x[GetTrueWizardNumber_61790(j)][1];
						playerColourIdx = playersColors_E88E0x[GetTrueWizardNumber_61790(j)][0];
						DrawBitmap_2BB40(blackBarX, posY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TILE_MINI], scale);
						DrawLine_2BC80(
							barX,
							barY,
							((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TILE_MINI].width_4 * scale) - (8 * scale),
							((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TILE_MINI].height_5 * scale) - (8 * scale),
							playerColourIdx);
						sprintf(printbuffer, "%03d", v23x->playerData_0xA4_164x->kills_0x26_38[j]);
						DrawText_2BC10(printbuffer, blackBarX + (8 * scale), posY + (10 * scale), textColourIdx, scale);
						if (gameState_D41A4.showHelp_10 && helpPopup_1804B0.wizardNumber_0xa8 && !v25 && j == (unsigned __int8)helpPopup_1804B0.wizardNumber_0xa8)
						{
							helpPopup_1804B0.scoresBarX_0x9a = blackBarX + (20 * scale);
							helpPopup_1804B0.scoresBarY_0x9c = posY + (20 * scale);
						}
					}
					//HIWORD(v18) = HIWORD(**filearray_2aa18c[6]);
					blackBarX += (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TILE_MINI].width_4 * scale;
				}
			}
			//HIWORD(v19) = HIWORD(**filearray_2aa18c[6]);
			posY += (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TILE_MINI].height_5 * scale;
		}
		v25++;
	}
}
// 8E3D5: using guessed type x_DWORD sprintf(x_DWORD, const char *, ...);
// D419D: using guessed type char FontType_D419D;
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3DC: using guessed type int **filearray_2aa18c[6];
// EA3E4: using guessed type int Entities_EA3E4[];
// 180532: using guessed type int x_DWORD_180532;
// 180546: using guessed type __int16 x_WORD_180546;
// 180548: using guessed type __int16 x_WORD_180548;
// 18054A: using guessed type __int16 x_WORD_18054A;
// 18054C: using guessed type __int16 x_WORD_18054C;
// 180558: using guessed type char x_BYTE_180558;
// 180660: using guessed type __int16 videoMode_180660;

//----- (0002E790) --------------------------------------------------------
void ColorizeScreen_2E790(int posX, int posY, int width, int height, uint8_t color)//20f790
{
	int locPosX = posX;
	int locPosY = posY;
	int locWidth = width;
	int locHeight = height;
	if (videoMode_180660 == 1)
	{
		locPosX = posX / 2;
		locPosY = posY / 2;
		locWidth = width / 2;
		locHeight = height / 2;
	}

	const uint8_t overViewAlpha = (uint8_t)((1.0f - COLOUR_BLEND_ALPHA) * 255.0f + 0.5f);
	for (int indexY = locPosY; indexY < locPosY + locHeight; indexY++)
	{
		for (int indexX = locPosX; indexX < locWidth + locPosX; indexX++)
		{
			const int index = screenWidth_18062C * indexY + indexX;
			if (pdwScreenAlphaBuffer != nullptr && pdwScreenAlphaBuffer[index] == 0)
			{
				pdwScreenBuffer_351628[index] = color;
				pdwScreenAlphaBuffer[index] = overViewAlpha;
			}
			else
				pdwScreenBuffer_351628[index] = ColourLookupTable_F6EE0[COLOUR_BLEND_LOOKUP_OFFSET + 256 * color + pdwScreenBuffer_351628[index]];
		}
	}
}

//----- (0002ECC0) --------------------------------------------------------
void DrawBottomSpellsMenu_2ECC0()//20fcc0
{
	__int16 posIconsY;
	int16_t posIconsX;
	int16_t selectedSpellIndex;
	int16_t posSubMenuSpellX;
	int16_t posIconsXStart;
	int16_t spellIconWidth;
	int16_t spellIconHeight;
	int16_t posSubMenuIconWidth;
	int16_t spellIconIndex;

	int posIconsY2;
	bool drawBitmap;
	type_entity_0x6E8E* spellEntity;
	unsigned __int16 entityIndex;
	bitmap_pos_struct_t bitmap;
	uint8 spellIndex;
	int xPosNext;
	int xPos;
	int xDiff;
	int subSpellPos;
	int16_t posEndX;
	int manaCost;
	__int16 spellIndex2;
	__int16 xAdd2;
	bool canSummon;
	type_entity_0x6E8E* playerEntity = Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
	uint8 color0 = playersColors_E88E0x[GetTrueWizardNumber_61790(playerEntity->playerData_0xA4_164x->playerColorIndex_0x38_56)][0];
	uint8 color1 = playersColors_E88E0x[GetTrueWizardNumber_61790(playerEntity->playerData_0xA4_164x->playerColorIndex_0x38_56)][1];
	gameState_D41A4.spellOnCursor_50 = -1;
	int16_t posX = 0;
	int16_t posY = 0;
	uint8_t scale = 1;
	if (UseInGameUiLayout())
		scale = gameUiScale;
	GetSpellMenuPosition(&posX, &posY);
	posIconsXStart = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[EDGE_PANEL].width_4 * scale;
	spellIconHeight = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL].height_5 * scale;
	posIconsY2 = posY - (2 * spellIconHeight);
	spellIconWidth = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL].width_4 * scale;
	spellIconIndex = 0;
	xAdd2 = spellIconWidth - ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TOP_LEFT_CORNER].width_4 * scale);
	posIconsY = posY - (2 * spellIconHeight);
	uint8_t iconXIdx = 0;
	uint8_t iconYIdx = 0;
	while (iconYIdx < 2)
	{
		if (!iconYIdx)
		{
			//Draw left frame
			ptrDrawBitmap_F01E8(posX, posIconsY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[EDGE_PANEL], scale);
		}
		posIconsX = posIconsXStart;
		//Draw Spell Icons
		iconXIdx = 0;
		while (iconXIdx < 13)
		{
			spellIndex2 = spellIndex_D94FF[spellIconIndex];
			drawBitmap = true;
			if (!SPELLS_BEGIN_BUFFER_str[spellIndex_D94FF[spellIconIndex]].levelCount_0 || (!isCaveLevel_D41B6 && spellIndex2 == 25))
				drawBitmap = false;
			if (!drawBitmap)
			{
				ptrDrawBitmap_F01E8(posX + posIconsX, posIconsY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL], scale);
			}
			else
			{
				spellEntity = Entities_EA3E4[playerEntity->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[spellIndex_D94FF[spellIconIndex]]];
				if (spellEntity > Entities_EA3E4[0])
				{
					int subSpellIndex;
					if (spellIconIndex == playerEntity->playerData_0xA4_164x->str_611.spellIndex_0x458_1112)
					{
						gameState_D41A4.spellOnCursor_50 = spellIconIndex;
						subSpellIndex = playerEntity->playerData_0xA4_164x->str_611.subSpellIndex_0x459_1113;
					}
					else
					{
						subSpellIndex = playerEntity->playerData_0xA4_164x->str_611.selectedSubSpells_0x437_1079x.SpellIndex[spellIndex_D94FF[spellIconIndex]];
					}

					bool skipToLabel43 = false;
					if (SPELLS_BEGIN_BUFFER_str[spellEntity->model_0x40_64].isEnabled_1 & 4)
					{
						if (spellEntity->stage_0x2E_46 > 0 && spellEntity->stage_0x2E_46 < 32 && gameState_D41A4.colorIndex_121[1])
						{
							skipToLabel43 = true;
						}
					}
					if (!skipToLabel43)
					{
						canSummon = false;
						if (!SPELLS_BEGIN_BUFFER_str[spellIndex2].subspell[subSpellIndex].maxManaLimit_A
							|| ((entityIndex = playerEntity->playerData_0xA4_164x->CastleEntityIndex_0x3A_58) != 0
								&& SPELLS_BEGIN_BUFFER_str[spellIndex2].subspell[subSpellIndex].maxManaLimit_A <= Entities_EA3E4[entityIndex]->mana_0x90_144))
						{
							canSummon = true;
						}
						if (canSummon && gameState_D41A4.spellOnCursor_50 == spellIconIndex)
						{
							manaCost = GetSpellManaCost_6D710(playerEntity, spellIndex2, subSpellIndex);
							if (manaCost > 0)
							{
								//Draw mana needed for extra shot for spell
								DrawBitmap_2BB40(posX + posIconsX, posIconsY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TILE_BAR], scale);
								DrawLine_2BC80(posX + posIconsX + (6 * scale), posIconsY + (28 * scale), (36 * scale) * (playerEntity->mana_0x90_144 % manaCost) / manaCost, (4 * scale), color1);

								//Draw mana shot for spell
								int manaPosX = playerEntity->mana_0x90_144 / manaCost;
								for (int x = 0; x < 36 && manaPosX > 0; x += 2)
								{
									int y = 0;
									while (y < (4 * scale) && manaPosX > 0)
									{
										DrawLine_2BC80((x * scale) + posX + posIconsX + (6 * scale), y + posIconsY + (28 * scale), (2 * scale), (2 * scale), color0);
										y += (2 * scale);
										manaPosX--;
									}
								}
							}
						}
						else
						{
							if (canSummon)
								bitmap = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL];
							else
								bitmap = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL2];

							ptrDrawBitmap_F01E8(posX + posIconsX, posIconsY, bitmap, scale);
						}
						if (canSummon)
							DrawBitmap_2BB40(posX + posIconsX, posIconsY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[spellIndex2 + SPELL_FIREBALL_SMALL], scale);
						else
							GameBitmap::DrawTransparentBitmap_2DE80(posX + posIconsX, posIconsY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[spellIndex2 + SPELL_FIREBALL_SMALL], scale);
					}
					//Draw the mouse button marker of a spell favourited with shift + mouse button
					uint8_t bitmapType = playerEntity->playerData_0xA4_164x->str_611.spellSlotTypes_0x3B5_949x.SpellIndex[spellIndex2];
					if (bitmapType == 1)
						GameBitmap::DrawTransparentBitmap_2DE80(posX + posIconsX, posIconsY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TOP_LEFT_CORNER], scale);
					else if (bitmapType == 2)
						GameBitmap::DrawTransparentBitmap_2DE80(posX + posIconsX + xAdd2, posIconsY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_TOP_RIGHT_CORNER], scale);
				}
				else
				{
					ptrDrawBitmap_F01E8(posX + posIconsX, posIconsY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL], scale);
					if (playerEntity->playerData_0xA4_164x->str_611.availableSpells_0x3E9_1001x.SpellIndex[spellIndex2] || playerEntity->playerData_0xA4_164x->str_611.pickedUpSpells_0x403_1027x.SpellIndex[spellIndex2])
					{
						GameBitmap::DrawColourizedBitmap(posX + posIconsX, posIconsY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[spellIndex2 + SPELL_FIREBALL_SMALL], 0xA6u, scale);
					}
				}
			}
			iconXIdx++;
			posIconsX += spellIconWidth;
			spellIconIndex++;
		}
		if (!iconYIdx)
		{
			//Draw right part for frame
			ptrDrawBitmap_F01E8(posX + posIconsX, posIconsY, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[EDGE_PANEL], scale);
		}
		posIconsY += (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL].height_5 * scale;
		iconYIdx++;
	}
	//Frame of selected spell
	DrawBitmap_2BB40(posX + posIconsXStart + spellIconWidth * (playerEntity->playerData_0xA4_164x->str_611.spellIndex_0x458_1112 % 13), playerEntity->playerData_0xA4_164x->str_611.spellIndex_0x458_1112 / 13 * spellIconHeight + posIconsY2, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_FRAME], scale);
	//Draw spell selection
	selectedSpellIndex = gameState_D41A4.spellOnCursor_50;
	if (selectedSpellIndex != -1)
	{
		spellIndex = spellIndex_D94FF[selectedSpellIndex];
		signed __int16 spellIndex3 = playerEntity->playerData_0xA4_164x->str_611.SpellLevels_0x41D_1053z.SpellIndex[spellIndex];
		__int16 xAdd = 3 * ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL2].width_4 * scale);
		__int16 posY2 = posIconsY2 - ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL2].height_5 * scale);
		posSubMenuSpellX = spellIconWidth / 2 + spellIconWidth * (playerEntity->playerData_0xA4_164x->str_611.spellIndex_0x458_1112 % 13) + posIconsXStart - xAdd / 2;
		if (posSubMenuSpellX <= (640 * scale) - xAdd)
		{
			if ((posSubMenuSpellX & 0x8000u) != 0)
				posSubMenuSpellX = 0;
		}
		else
		{
			posSubMenuSpellX = (640 * scale) - xAdd;
		}
		int subSpellIndex2 = 0;
		posSubMenuIconWidth = (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL2].width_4 * scale;
		while (2)
		{
			if (subSpellIndex2 >= 3)
				break;
			int manaPart = 0;
			bool canSubSummon = false;
			if (!SPELLS_BEGIN_BUFFER_str[spellIndex].subspell[subSpellIndex2].maxManaLimit_A
				|| (entityIndex = playerEntity->playerData_0xA4_164x->CastleEntityIndex_0x3A_58) != 0
				&& SPELLS_BEGIN_BUFFER_str[spellIndex].subspell[subSpellIndex2].maxManaLimit_A <= Entities_EA3E4[entityIndex]->mana_0x90_144)
			{
				canSubSummon = true;
				manaPart = playerEntity->mana_0x90_144 / GetSpellManaCost_6D710(playerEntity, spellIndex, subSpellIndex2);
			}
			if (subSpellIndex2 > spellIndex3)
			{
				DrawBitmap_2BB40(posX + posSubMenuSpellX, posY2, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL2], scale);
			}
			else
			{
				int bitmapIndex;
				if (canSubSummon && manaPart)
					bitmapIndex = SPELL_ICON2_PANEL2_WITH_FRAME;
				else
					bitmapIndex = SPELL_ICON2_PANEL2_WITH_FRAME_DARK;
				DrawBitmap_2BB40(posX + posSubMenuSpellX, posY2, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[bitmapIndex], scale);
				DrawBitmap_2BB40(posX + posSubMenuSpellX + 6, posY2 + 10, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[subSpellIndex2 + SPELL_BACKGROUND_NUMBER1], scale);
				int bitmapIndex2 = subSpellIndex2 + 3 * spellIndex + SPELL_SUB_FIREBALL1_SMALL;
				if (canSubSummon)
					DrawBitmap_2BB40(posX + posSubMenuSpellX + (18 * scale), posY2 + (6 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[bitmapIndex2], scale);
				else
					GameBitmap::DrawTransparentBitmap_2DE80(posX + posSubMenuSpellX + (18 * scale), posY2 + (6 * scale), (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[bitmapIndex2], scale);
			}
			if (subSpellIndex2 == playerEntity->playerData_0xA4_164x->str_611.subSpellIndex_0x459_1113)
				DrawBitmap_2BB40(posX + posSubMenuSpellX, posY2, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_GOLD_FRAME], scale);

			if (subSpellIndex2 <= spellIndex3 && subSpellIndex2 < 2)
			{
				if (subSpellIndex2 == spellIndex3)
				{
					if (gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
					{
						xPosNext = SPELLS_BEGIN_BUFFER_str[spellIndex].subspell[subSpellIndex2 + 1].xpos2_0x12;
						xPos = SPELLS_BEGIN_BUFFER_str[spellIndex].subspell[subSpellIndex2].xpos2_0x12;
					}
					else
					{
						xPosNext = SPELLS_BEGIN_BUFFER_str[spellIndex].subspell[subSpellIndex2 + 1].xpos1_E;
						xPos = SPELLS_BEGIN_BUFFER_str[spellIndex].subspell[subSpellIndex2].xpos1_E;
					}

					xDiff = xPosNext - xPos;
					subSpellPos = playerEntity->playerData_0xA4_164x->str_611.SpellExperience_0x263_611x.SpellExperience[spellIndex] + playerEntity->playerData_0xA4_164x->str_611.spellsExperience_0x2CB_715x.at(spellIndex) - xPos;
					if (xDiff > 0)
					{
						posEndX = (54 * scale) * subSpellPos / xDiff;
					}
					else
					{
						posEndX = 0;
					}
				}
				else
				{
					posEndX = (54 * scale);
				}
				//Draw Experience for Spell
				if ((posEndX & 0x8000u) != 0)
					posEndX = 0;
				if (posEndX > (54 * scale))
					posEndX = (54 * scale);
				DrawLine_2BC80(posX + posSubMenuSpellX + (6 * scale), posY2 + (28 * scale), (54 * scale), (2 * scale), (*xadataclrd0dat.colorPalette_var28)[0]);
				if (posEndX)
					DrawLine_2BC80(posX + posSubMenuSpellX + (6 * scale), posY2 + (28 * scale), posEndX, (2 * scale), (*xadataclrd0dat.colorPalette_var28)[3840]);
			}
			subSpellIndex2++;
			posSubMenuSpellX += posSubMenuIconWidth;
			continue;
		}
	}

	if (inputState_18058C.joystickMode_1805C2 == 8
		|| inputState_18058C.joystickMode_1805C2 == 12
		|| inputState_18058C.joystickMode_1805C2 == 13
		|| inputState_18058C.joystickMode_1805C2 == 4
		|| inputState_18058C.joystickMode_1805C2 == 6
		|| inputState_18058C.joystickMode_1805C2 == 11
		|| inputState_18058C.joystickMode_1805C2 == 5)
	{
		DrawBitmap_2BB40(inputState_18058C.mouse_1805B0.x, inputState_18058C.mouse_1805B0.y, (*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[cursorSpriteIndex_D419E]);
	}
	EventDispatcher::I->DispatchEvent(EventType::E_SCENE_CHANGE, Scene::SPELL_MENU);
}

//----- (00054960) --------------------------------------------------------
void DrawLoadingBar_54960()//235960
{
	int16_t posY;
	if (videoMode_180660 & 1)
		posY = 400;
	else
		posY = 480;
	if (videoMode_180660 != 1)
		if (!DefaultResolutions())
			posY = screenHeight_180624;
	SetMousePositionInMemory_5BDC0(426, posY - 72 + 17);
}

//----- (00061880) --------------------------------------------------------
void DrawMinimapEntites_61880(int16_t x, int16_t y, int16_t posX, int16_t posY, uint16_t width, uint16_t height, int16_t yaw, int16_t scaling, uint8_t scale)//242880
{
	//char *result; // eax
	// 0 0 4d80 ffffde80 80 0 0 100 c0 0
	if (world_D41A0.m_GameSettings.m_Display.m_uiScreenSize == 1)
		DrawMinimapEntities_627F0(x, y, posX, posY, width, height, yaw, scaling, scale);
	else
		DrawMinimapEntitiesB_61A00(x, y, posX, posY, width, height, yaw, scaling, scale);
}

//----- (0006FEC0) --------------------------------------------------------
void DrawGameDebugText_6FEC0()//250ec0
{
	//int v0; // edx
	//int result; // eax
	//char v2; // dl
	int v3; // eax
	int v4; // ebx
	int v5; // eax
	int v6; // ebx
	int v7; // eax
	int v8; // ebx
	int v9; // eax
	int v10; // ebx
	int v11; // eax
	int v12; // ebx
	int v13; // eax
	int v14; // ebx
	int v15; // eax
	int v16; // ebx
	int v17; // eax
	int v18; // ebx
	int v19; // eax
	int v20; // ebx
	int v21; // eax
	int v22; // ebx
	int v23; // eax
	int v24; // ebx
	int v25; // eax
	int v26; // ebx
	int v27; // eax
	int v28; // ebx
	int v29; // eax
	//int v30; // edx
	unsigned int v31; // eax
	__int16 v32; // bx
	__int16 v33; // bx
	__int16 v34; // bx
	__int16 v35; // bx
	__int16 v36; // bx
	__int16 v37; // bx
	__int16 v38; // bx
	int v39; // eax
	__int16 v40; // bx
	__int16 v41; // bx
	__int16 v42; // bx
	__int16 v43; // bx
	__int16 v44; // bx
	//int* v45; // edi
	//int v46; // ebx
	//unsigned __int8 v47; // al
	//int v48; // eax
	//int v49; // eax
	__int16 v50; // bx
	__int16 v51; // bx
	__int16 v52; // bx
	__int16 v53; // bx
	__int16 v54; // bx
	__int16 v55; // bx
	__int16 v56; // bx
	__int16 v57; // bx
	__int16 v58; // bx
	__int16 v59; // bx
	__int16 v60; // bx
	__int16 v61; // bx
	__int16 v62; // bx
	__int16 v63; // bx
	//char v64; // [esp+0h] [ebp+7Eh]
	//int i; // [esp+100h] [ebp+17Eh]

	//v0 = 2124 * D41A0_BYTESTR_0.word_0xc;
	//result = v0 + (int)x_D41A0_BYTEARRAY_0;
	//v2 = *(x_BYTE *)(v0 + x_D41A0_BYTEARRAY_0 + 11235);
	//!!!!!!!!!!!!!!!
	if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].ShowDebugTextFlag_0x005_2BE0_11235 & 8)
	{
		GetFont_6FC50(1);
		DrawText_2BC10((char*)"Product name", 320, 0, (*xadataclrd0dat.colorPalette_var28)[3840]);
		LOWORD(v3) = GetLetterHeight_6FC30();
		v4 = v3;
		DrawText_2BC10((char*)"Magic Carpet 2 (Netherworlds)", 320, v3, (*xadataclrd0dat.colorPalette_var28)[15]);
		LOWORD(v5) = GetLetterHeight_6FC30();
		v6 = v5 + v4;
		DrawText_2BC10((char*)"Version number", 320, v6, (*xadataclrd0dat.colorPalette_var28)[3840]);
		LOWORD(v7) = GetLetterHeight_6FC30();
		v8 = v7 + v6;
		DrawText_2BC10(VersionNumber, 320, v8, (*xadataclrd0dat.colorPalette_var28)[15]);
		LOWORD(v9) = GetLetterHeight_6FC30();
		v10 = v9 + v8;
		DrawText_2BC10((char*)"Version date", 320, v10, (*xadataclrd0dat.colorPalette_var28)[3840]);
		LOWORD(v11) = GetLetterHeight_6FC30();
		v12 = v11 + v10;
		sprintf(printbuffer, "%s %s", "03:42:13", "Sep 06 1995");
		DrawText_2BC10(printbuffer, 320, v12, (*xadataclrd0dat.colorPalette_var28)[15]);
		LOWORD(v13) = GetLetterHeight_6FC30();
		v14 = v13 + v12;
		DrawText_2BC10((char*)"Programmer", 320, v14, (*xadataclrd0dat.colorPalette_var28)[3840]);
		LOWORD(v15) = GetLetterHeight_6FC30();
		v16 = v15 + v14;
		DrawText_2BC10((char*)"Bullfrog, Alan Wright.", 320, v16, (*xadataclrd0dat.colorPalette_var28)[15]);
		LOWORD(v17) = GetLetterHeight_6FC30();
		v18 = v17 + v16;
		DrawText_2BC10((char*)"Supplied to", 320, v18, (*xadataclrd0dat.colorPalette_var28)[3840]);
		LOWORD(v19) = GetLetterHeight_6FC30();
		v20 = v19 + v18;
		DrawText_2BC10((char*)"PUBLIC", 320, v20, (*xadataclrd0dat.colorPalette_var28)[15]);
		LOWORD(v21) = GetLetterHeight_6FC30();
		v22 = v21 + v20;
		DrawText_2BC10((char*)"Level Number", 320, v22, (*xadataclrd0dat.colorPalette_var28)[3840]);
		LOWORD(v23) = GetLetterHeight_6FC30();
		v24 = v23 + v22;
		sprintf(printbuffer, "%d", gameState_D41A4.levelnumber_43w);
		DrawText_2BC10((char*)printbuffer, 320, v24, (*xadataclrd0dat.colorPalette_var28)[15]);
		LOWORD(v25) = GetLetterHeight_6FC30();
		v26 = v25 + v24;
		if (gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
		{
			DrawText_2BC10((char*)"Transfer rate:", 320, v26, (*xadataclrd0dat.colorPalette_var28)[3840]);
			LOWORD(v27) = GetLetterHeight_6FC30();
			v28 = v27 + v26;
			sprintf(printbuffer, "%d", gameState_D41A4.transfer_rate_dwordindex_200);
			DrawText_2BC10(printbuffer, 320, v28, (*xadataclrd0dat.colorPalette_var28)[15]);
			LOWORD(v29) = GetLetterHeight_6FC30();
			v26 = v29 + v28;
		}
		//v30 = 2124 * D41A0_BYTESTR_0.word_0xc;
		debugTurn_E9C14 = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].Turn_2BE0_11248;
		v31 = j___clock() / 0x64u;
		x_DWORD_E9C1C = v31;
		if (v31 != debugLastValue_E9C20)
		{
			debugLastValue_E9C20 = v31;
			debugFps_E9C24 = debugTurn_E9C14 - debugLastTurn_E9C18;
			debugLastTurn_E9C18 = debugTurn_E9C14;
		}
		DrawText_2BC10((char*)"FPS", 320, v26, (*xadataclrd0dat.colorPalette_var28)[3840]);
		v32 = GetLetterHeight_6FC30() + v26;
		sprintf(printbuffer, "%d", debugFps_E9C24);
		DrawText_2BC10(printbuffer, 320, v32, (*xadataclrd0dat.colorPalette_var28)[15]);
		v33 = GetLetterHeight_6FC30() + v32;
		DrawText_2BC10((char*)"Sound Number", 320, v33, (*xadataclrd0dat.colorPalette_var28)[3840]);
		v34 = GetLetterHeight_6FC30() + v33;
		sprintf(printbuffer, "%d", SoundNumber_E37B6);
		DrawText_2BC10(printbuffer, 320, v34, (*xadataclrd0dat.colorPalette_var28)[15]);
		v35 = GetLetterHeight_6FC30() + v34;
		DrawText_2BC10((char*)"Game turn", 320, v35, (*xadataclrd0dat.colorPalette_var28)[3840]);
		v36 = GetLetterHeight_6FC30() + v35;
		sprintf(printbuffer, "%d %d", world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].Turn_2BE0_11248, GameTimerTurn_17DB54);
		DrawText_2BC10(printbuffer, 320, v36, (*xadataclrd0dat.colorPalette_var28)[15]);
		v37 = GetLetterHeight_6FC30() + v36;
		DrawText_2BC10((char*)"Thing", 320, v37, (*xadataclrd0dat.colorPalette_var28)[3840]);
		v38 = GetLetterHeight_6FC30() + v37;
		v39 = GetFreeEntityCount_4A810();
		sprintf(printbuffer, "Thing %d, Active %d", 168, 1000 - v39);
		DrawText_2BC10(printbuffer, 320, v38, (*xadataclrd0dat.colorPalette_var28)[15]);
		v40 = GetLetterHeight_6FC30() + v38;
		sprintf(printbuffer, "Carpet %zu", sizeof(type_shadow_D41A0_BYTESTR_0));
		DrawText_2BC10(printbuffer, 320, v40, (*xadataclrd0dat.colorPalette_var28)[15]);
		v41 = GetLetterHeight_6FC30() + v40;
		sprintf(printbuffer, "Tape %d", 38547);
		DrawText_2BC10(printbuffer, 320, v41, (*xadataclrd0dat.colorPalette_var28)[15]);
		v42 = GetLetterHeight_6FC30() + v41;
		sprintf(printbuffer, "Heap %d", gameState_D41A4.dword_0xE6_heapsize_230);
		DrawText_2BC10(printbuffer, 320, v42, (*xadataclrd0dat.colorPalette_var28)[15]);
		v43 = GetLetterHeight_6FC30() + v42;
		//sub_85350();
		DrawText_2BC10((char*)"Memory (Used/Free)", 320, v43, (*xadataclrd0dat.colorPalette_var28)[3840]);
		v44 = GetLetterHeight_6FC30() + v43;
		sprintf(printbuffer, "%d/%d", memUsed_17FEA8, memFree_17FEA4);
		DrawText_2BC10(printbuffer, 320, v44, (*xadataclrd0dat.colorPalette_var28)[15]);
		/*v45 = (int*)&x_DWORD_17ECA0;
		//LOWORD(result) = GetLetterHeight_6FC30();
		v46 = 0;
		for (i = 0; v45; v45 = (int*)v45[2])
		{
			sprintf(printbuffer, "s%7.7d,u%01d", v45[1], *((unsigned __int8*)v45 + 16));
			if (x_DWORD_17E0A4[3 * *((unsigned __int8*)v45 + 17)])
				v47 = (*xadataclrd0dat.colorPalette_var28)[0x000];
			else
				v47 = (*xadataclrd0dat.colorPalette_var28)[3840];
			DrawText_2BC10(printbuffer, i, v46, v47);
			//LOWORD(v48) = GetLetterHeight_6FC30();
			v46 += GetLetterHeight_6FC30();
			//result = 400 - GetLetterHeight_6FC30();
			if ((signed __int16)v46 > 400 - GetLetterHeight_6FC30())
			{
				//LOWORD(v49) = GetLetterWidth_6FC10();
				//result = 12 * v49;
				i += 12 * GetLetterWidth_6FC10();
				v46 = 0;
			}
		}
		*/ //original allocations
	}
	if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].ShowDebugTextFlag_0x005_2BE0_11235 & 0x40)
	{
		v50 = 6 * GetLetterHeight_6FC30();
		DrawText_2BC10((char*)"THING", 10, v50, (*xadataclrd0dat.colorPalette_var28)[3840]);
		v51 = GetLetterHeight_6FC30() + v50;
		sprintf(
			printbuffer,
			"%ld / %ld",
			(long)(intptr_t)gameState_D41A4.nearestEntity_38396,
			(long)(gameState_D41A4.nearestEntity_38396 - world_D41A0.entities_0x6E8E));
		DrawText_2BC10(printbuffer, 10, v51, (*xadataclrd0dat.colorPalette_var28)[255]);
		v52 = GetLetterHeight_6FC30() + v51;
		DrawText_2BC10((char*)"CLASS / MODEL / STATE", 10, v52, (*xadataclrd0dat.colorPalette_var28)[3840]);
		v53 = GetLetterHeight_6FC30() + v52;
		sprintf(
			printbuffer,
			"%hhd / %hhd / %hhu",
			gameState_D41A4.nearestEntity_38396->class_0x3F_63,
			gameState_D41A4.nearestEntity_38396->model_0x40_64,
			gameState_D41A4.nearestEntity_38396->actionIndex_0x45_69);
		DrawText_2BC10(printbuffer, 10, v53, (*xadataclrd0dat.colorPalette_var28)[255]);
		v54 = GetLetterHeight_6FC30() + v53;
		DrawText_2BC10((char*)"LIFE / MAX LIFE", 10, v54, (*xadataclrd0dat.colorPalette_var28)[3840]);
		v55 = GetLetterHeight_6FC30() + v54;
		sprintf(
			printbuffer,
			"%d / %d",
			gameState_D41A4.nearestEntity_38396->life_0x8,
			gameState_D41A4.nearestEntity_38396->maxLife_0x4);
		DrawText_2BC10(printbuffer, 10, v55, (*xadataclrd0dat.colorPalette_var28)[255]);
		v56 = GetLetterHeight_6FC30() + v55;
		DrawText_2BC10((char*)"SPEED ACTUAL / MINIMUM / MAXIMUM", 10, v56, (*xadataclrd0dat.colorPalette_var28)[3840]);
		v57 = GetLetterHeight_6FC30() + v56;
		sprintf(
			printbuffer,
			"%hd / %hd / %hd",
			gameState_D41A4.nearestEntity_38396->actSpeed_0x82_130,
			gameState_D41A4.nearestEntity_38396->minSpeed_0x84_132,
			gameState_D41A4.nearestEntity_38396->maxSpeed_0x86_134);
		DrawText_2BC10(printbuffer, 10, v57, (*xadataclrd0dat.colorPalette_var28)[255]);
		v58 = GetLetterHeight_6FC30() + v57;
		DrawText_2BC10((char*)"ACTUAL X / Y / Z", 10, v58, (*xadataclrd0dat.colorPalette_var28)[3840]);
		v59 = GetLetterHeight_6FC30() + v58;
		sprintf(
			printbuffer,
			"%hu / %hu / %hu",
			gameState_D41A4.nearestEntity_38396->position_0x4C_76.x,
			gameState_D41A4.nearestEntity_38396->position_0x4C_76.y,
			gameState_D41A4.nearestEntity_38396->position_0x4C_76.z);
		DrawText_2BC10(printbuffer, 10, v59, (*xadataclrd0dat.colorPalette_var28)[255]);
		v60 = GetLetterHeight_6FC30() + v59;
		DrawText_2BC10((char*)"ID / WHO OWNS ME", 10, v60, (*xadataclrd0dat.colorPalette_var28)[3840]);
		v61 = GetLetterHeight_6FC30() + v60;
		sprintf(
			printbuffer,
			"%hu / %hu",
			gameState_D41A4.nearestEntity_38396->id_0x1A_26,
			gameState_D41A4.nearestEntity_38396->parentId_0x28_40);
		DrawText_2BC10(printbuffer, 10, v61, (*xadataclrd0dat.colorPalette_var28)[255]);
		v62 = GetLetterHeight_6FC30() + v61;
		DrawText_2BC10((char*)"MANA ACTUAL / MAXIMUM", 10, v62, (*xadataclrd0dat.colorPalette_var28)[3840]);
		v63 = GetLetterHeight_6FC30() + v62;
		sprintf(
			printbuffer,
			"%d / %d",
			gameState_D41A4.nearestEntity_38396->mana_0x90_144,
			gameState_D41A4.nearestEntity_38396->maxMana_0x8C_140);
		DrawText_2BC10(printbuffer, 10, v63, (*xadataclrd0dat.colorPalette_var28)[255]);
		//LOWORD(result) = GetLetterHeight_6FC30();
	}
	if (gameState_D41A4.byteindex_53 == 16273849)
		gameState_D41A4.cdFlags_52 |= 2u;
	//return result;
}
// 8E3D5: using guessed type x_DWORD sprintf(x_DWORD, const char *, ...);
// 98786: using guessed type int /*__fastcall*/ j___clock(x_DWORD, x_DWORD, x_DWORD);
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// E37B6: using guessed type __int16 x_WORD_E37B6_sound_number;
// E890F: using guessed type char x_BYTE_E890F;
// E89FF: using guessed type char x_BYTE_E89FF;
// E9800: using guessed type char x_BYTE_E9800;
// E9C14: using guessed type int debugTurn_E9C14;
// E9C18: using guessed type int debugLastTurn_E9C18;
// E9C1C: using guessed type int x_DWORD_E9C1C;
// E9C20: using guessed type int debugLastValue_E9C20;
// E9C24: using guessed type int debugFps_E9C24;
// 17DB54: using guessed type int GameTimerTurn_17DB54;
// 17E0A4: using guessed type int x_DWORD_17E0A4[];
// 17ECA0: using guessed type int x_DWORD_17ECA0;
// 17FEA4: using guessed type int memFree_17FEA4;
// 17FEA8: using guessed type int memUsed_17FEA8;

//----- (00087090) --------------------------------------------------------
void SetTextBoxMinMaxSizes_87090()//268090
{
	signed int i; // edx
	uint8_t* result; // al
	//uint8_t* v2; // eax
	int v2x;
	int v3; // edx

	memset(&helpPopup_1804B0, 0, 171);
	for (i = 1; i < 120 && !helpPopup_1804B0.helpItemCount_0x66; i++)
	{
		//if (!*(int16_t*)&array_E2A74[30 * i])
		if (!str_E2A74[i].type_0)
			helpPopup_1804B0.helpItemCount_0x66 = i - 1;
	}
	result = (uint8_t*)ReturnTrue_871E0();
	if (result)
	{
		helpPopup_1804B0.initialized_0xa2 = 1;
		helpPopup_1804B0.type_sub_0[0].charWidth_0x10 = 8;
		helpPopup_1804B0.type_sub_0[0].charHeight_0x12 = 14;
		helpPopup_1804B0.type_sub_0[0].maxTextboxHeight2_0xe = 400;
		helpPopup_1804B0.type_sub_0[0].maxPosX_0x16 = 600;
		helpPopup_1804B0.type_sub_0[0].minPosY_0x18 = 80;
		helpPopup_1804B0.type_sub_0[0].maxPosY_0x1a = 260;
		helpPopup_1804B0.type_sub_0[0].minPosX_0x14 = 40;
		helpPopup_1804B0.type_sub_0[1] = helpPopup_1804B0.type_sub_0[0];
		//qmemcpy(&unk_1804B0ar[51], unk_1804B0ar, 0x30u);
		//qmemcpy(&unk_1804B0ar[0x33+0x30], &helpPopup_1804B0.type_sub_0[0].byte_0x30, 3u);
		//v2 = &array_E2A74[30];
		v2x = 0;
		helpPopup_1804B0.type_sub_0[1].maxPosY_0x1a = 370;
		helpPopup_1804B0.type_sub_0[1].minPosX_0x14 = 30;
		helpPopup_1804B0.type_sub_0[1].maxPosX_0x16 = 610;
		helpPopup_1804B0.type_sub_0[1].minPosY_0x18 = 30;
		v3 = 0;
		while (v3 < helpPopup_1804B0.helpItemCount_0x66)
		{
			/* *(int32_t*)&v2[16] = *(int32_t*)&v2[24];
			if (!v2[29])
				v2[29] = v2[28];
			v2 += 30;
			*(uint8_t*)(v2 - 18) = 0;
			v3++;
			*(uint8_t*)(v2 - 10) = 0;*/
			str_E2A74[v2x].counter_16 = str_E2A74[v2x].interval_24;
			if (!str_E2A74[v2x].stage_29)
				str_E2A74[v2x].stage_29 = str_E2A74[v2x].resetStage_28;
			str_E2A74[v2x].entity_12 = 0;
			str_E2A74[v2x].countdown_20 = 0;
			v3++;
			v2x++;
		}
		helpPopup_1804B0.randSeed_0x82 = world_D41A0.rand_0x8;
		/*result = */UpdateHelpItems_88B20();
	}
}

void SetTextBoxMinMaxForSetResolution()
{
	if (!DefaultResolutions())
	{
		const int scale = gameUiScale;
		helpPopup_1804B0.type_sub_0[0].minPosX_0x14 = 40 * scale;
		helpPopup_1804B0.type_sub_0[0].minPosY_0x18 = 80 * scale;
		helpPopup_1804B0.type_sub_0[0].maxPosX_0x16 = screenWidth_18062C - (40 * scale);
		helpPopup_1804B0.type_sub_0[0].maxPosY_0x1a = screenHeight_180624 - (220 * scale);
		helpPopup_1804B0.type_sub_0[0].maxTextboxHeight2_0xe = screenHeight_180624 - 80;

		helpPopup_1804B0.type_sub_0[1] = helpPopup_1804B0.type_sub_0[0];
		helpPopup_1804B0.type_sub_0[1].minPosX_0x14 = 30 * scale;
		helpPopup_1804B0.type_sub_0[1].minPosY_0x18 = 30 * scale;
		helpPopup_1804B0.type_sub_0[1].maxPosX_0x16 = screenWidth_18062C - (30 * scale);
		helpPopup_1804B0.type_sub_0[1].maxPosY_0x1a = screenHeight_180624 - (110 * scale);
	}
	else
	{
		helpPopup_1804B0.type_sub_0[0].maxPosX_0x16 = 600;
		helpPopup_1804B0.type_sub_0[0].maxPosY_0x1a = 260;
		helpPopup_1804B0.type_sub_0[0].maxTextboxHeight2_0xe = 400;

		helpPopup_1804B0.type_sub_0[1] = helpPopup_1804B0.type_sub_0[0];
		helpPopup_1804B0.type_sub_0[1].maxPosX_0x16 = 610;
		helpPopup_1804B0.type_sub_0[1].maxPosY_0x1a = 370;
	}
}

//----- (000871E0) --------------------------------------------------------
char ReturnTrue_871E0()//2681e0
{
	return 1;
}

//----- (000871F0) --------------------------------------------------------
void DrawHelpPopUps_871F0()//2681f0
{
	bool hintType1 = true;

	DrawMenuState_872A0();
	if (helpPopup_1804B0.PopupStatusByte_0x9e & 0x10)
	{
		CloseHelpPopup_87C10();
	}
	else
	{
		if (helpPopup_1804B0.popupFlags_0x9f & 0x20 || helpPopup_1804B0.popupFlags_0x9f & 0x10)
			hintType1 = false;
		if (helpPopup_1804B0.PopupStatusByte_0x9e & 2 || helpPopup_1804B0.popupFlags_0x9f & 0x8)
			hintType1 = true;
		if (hintType1)
		{
			if (helpPopup_1804B0.Index_0x86)
			{
				DrawPopupTextBox_87610();
				UpdateHelpPopupTimeout_87B30();
			}
			else if (--helpPopup_1804B0.blinkCounter_0x72 <= 0)
			{
				SetHelpPopupCoords_87580();
			}
			UpdateHelpItemTimers_87B70();
		}
		else
		{
			if (helpPopup_1804B0.popupFlags_0x9f & 0x10)
			{
				SetHelpPopupLine_87A30();
				return;
			}
			if (helpPopup_1804B0.popupFlags_0x9f & 0x20)
			{
				DrawPauseMenuPopUps_87860();
				return;
			}
		}
	}
}

//----- (000872A0) --------------------------------------------------------
void DrawMenuState_872A0()//2682a0
{
	unsigned __int8 v1; // al
	type_entity_0x6E8E* v2x; // esi
	char v3; // dh
	int v5; // edx
	int v7; // eax
	uint8_t scale = 1;

	if (!DefaultResolutions())
	{
		scale = gameUiScale;
	}

	v1 = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221;
	v2x = Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
	if (v1 < 6u || v1 > 8u)
		helpPopup_1804B0.PopupStatusByte_0x9e &= 0xFA;
	else
		helpPopup_1804B0.PopupStatusByte_0x9e |= 5;
	helpPopup_1804B0.PopupStatusByte_0x9e &= 0xFDu;
	helpPopup_1804B0.popupFlags_0x9f &= 0xC7u;
	v3 = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221;
	if (v3 != 5 && v3 != 8)
	{
		helpPopup_1804B0.blinkPeriod_0x6e = 20;
		helpPopup_1804B0.spellHelpStep_0xa4 = 0;
		helpPopup_1804B0.lastSpellIndex_0xa5 = -1;
	}
	else
	{
		helpPopup_1804B0.PopupStatusByte_0x9e |= 2;
		helpPopup_1804B0.blinkPeriod_0x6e = 6;
		v5 = v2x->playerData_0xA4_164x->str_611.spellIndex_0x458_1112;
		if (v5 != (unsigned __int8)helpPopup_1804B0.lastSpellIndex_0xa5 || v2x->playerData_0xA4_164x->str_611.subSpellIndex_0x459_1113 != helpPopup_1804B0.lastSubSpellIndex_0xa6)
		{
			v7 = spellIndex_D94FF[v2x->playerData_0xA4_164x->str_611.spellIndex_0x458_1112];
			if ((unsigned __int8)helpPopup_1804B0.spellHelpStep_0xa4 > 1u || !v2x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[v7] || !isCaveLevel_D41B6 && v7 == 25)
				CloseHelpPopup_87C10();
			helpPopup_1804B0.spellHelpStep_0xa4 = 0;
			SetSpellHelpPopupCoordinates_88D40(scale);
			helpPopup_1804B0.popupTimer_0x76 = 0;
		}
		helpPopup_1804B0.lastSpellIndex_0xa5 = v2x->playerData_0xA4_164x->str_611.spellIndex_0x458_1112;
		helpPopup_1804B0.lastSubSpellIndex_0xa6 = v2x->playerData_0xA4_164x->str_611.subSpellIndex_0x459_1113;
	}
	if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 == 7)
	{
		helpPopup_1804B0.popupFlags_0x9f |= 8u;
	}
	else if (str_E2A74[helpPopup_1804B0.Index_0x86].type_0 == 12)
	{
		CloseHelpPopup_87C10();
	}
	if (gameState_D41A4.OptionsSettingFlag_24 & GAME_PAUSED)
		helpPopup_1804B0.popupFlags_0x9f |= 0x20u;
	if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 == 3)
		helpPopup_1804B0.popupFlags_0x9f |= 0x10u;
	if (v2x->actSpeed_0x82_130)
		helpPopup_1804B0.uni_0x8a.dword[0] = 0;
	else
		(helpPopup_1804B0.uni_0x8a.dword[0])++;
	helpPopup_1804B0.popupFlags_0x9f &= 0xFBu;
	if (v2x->actionIndex_0x45_69 == 3)
		helpPopup_1804B0.popupFlags_0x9f = helpPopup_1804B0.popupFlags_0x9f | 4;
	helpPopup_1804B0.PopupStatusByte_0x9e &= 0xEF;
	if (world_D41A0.objectiveBoxCounter_0x36E04
		|| gameState_D41A4.byteindex_225
		|| v2x->actionIndex_0x45_69 == 12
		|| v2x->actionIndex_0x45_69 == 11
		|| world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 == 13
		|| world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221 == 14)
	{
		helpPopup_1804B0.blinkCounter_0x72 = helpPopup_1804B0.blinkPeriod_0x6e;
		helpPopup_1804B0.PopupStatusByte_0x9e |= 0x10;
	}
	if (helpPopup_1804B0.Index_0x86)
	{	//adress 268509
		if (str_E2A74[helpPopup_1804B0.Index_0x86].type_0 == 2 || str_E2A74[helpPopup_1804B0.Index_0x86].type_0 == 8)
		{
			if (!(helpPopup_1804B0.popupFlags_0x9f & 0x2)
				|| (str_E2A74[helpPopup_1804B0.Index_0x86].axis_2[0] >= 0)
				&& (str_E2A74[helpPopup_1804B0.Index_0x86].entity_12 == 0
					|| str_E2A74[helpPopup_1804B0.Index_0x86].countdown_20 < 0
					|| str_E2A74[helpPopup_1804B0.Index_0x86].interval_24 & 0x400))
			{
				CloseHelpPopup_87C10();
			}
		}
		if (v2x->actionIndex_0x45_69 == 2)
			CloseHelpPopup_87C10();
	}
}

//----- (00087580) --------------------------------------------------------
void SetHelpPopupCoords_87580()//268580
{
	uint8_t scale = 1;

	if (!DefaultResolutions())
	{
		scale = gameUiScale;
	}

	ClearHelpItemFlags_87BE0();
	if (helpPopup_1804B0.popupFlags_0x9f & 0x4)
	{
		SetHelpPopupTextAndCoords_884D0(91, 1, &str_E2A74[91].axis_2[1], 1, 0, scale);
	}
	else if (helpPopup_1804B0.PopupStatusByte_0x9e & 0x2)
	{
		SetSpellHelpPopupCoordinates_88D40(scale);
	}
	else if (helpPopup_1804B0.popupFlags_0x9f & 0x8)
	{
		SetPlayerScoresHelpPopupTextAndCoords_89360(scale);
	}
	else
	{
		GetHelpPopupTextAndCoords_87CF0(scale);
		int16_t helpIdx = GetHelpPopupIndex_88450();
		SetHelpPopupTextAndCoords_884D0(helpIdx, 1, &str_E2A74[helpIdx].axis_2[1], ((str_E2A74[helpIdx].axis_2[0] & 4) == 0) + 1, 0, scale);
	}
}

//----- (00087610) --------------------------------------------------------
void DrawPopupTextBox_87610()//268610
{
	char* textForDraw; // edi
	signed int v4; // esi
	int posY; // edi
	unsigned __int8 color; // al
	char textBuffer[256]; // [esp+0h] [ebp-80h]

	uint8_t scale = 1;

	if (!DefaultResolutions())
	{
		scale = gameUiScale;
	}

	textForDraw = langStrings_E9C4C[helpPopup_1804B0.uni_0x8a.word[2]];
	if (str_E2A74[helpPopup_1804B0.Index_0x86].axis_2[0] & 0x100 || helpPopup_1804B0.Index_0x86 == 96)
	{
		textForDraw = textBuffer;
		GetHintText_89AC0(textBuffer, helpPopup_1804B0.Index_0x86);
	}
	helpPopup_1804B0.type_sub_0[0].color1_0x30 = (*xadataclrd0dat.colorPalette_var28)[4095];//convert RGB to indexed color
	helpPopup_1804B0.type_sub_0[0].color2_0x31 = (*xadataclrd0dat.colorPalette_var28)[0];
	helpPopup_1804B0.type_sub_0[0].color3_0x32 = (*xadataclrd0dat.colorPalette_var28)[3840];

	switch (helpPopup_1804B0.drawTextType_0xa3)
	{
	case 1: // Draw in the top middle
	{
		if (helpPopup_1804B0.PopupStatusByte_0x9e & 8)
		{
			helpPopup_1804B0.type_sub_0[0].maxTextboxWidth_0x0 = 320;
			helpPopup_1804B0.type_sub_0[0].maxTextboxHeight_0x2 = 80;
			helpPopup_1804B0.type_sub_0[0].maxTextboxWidth2_0xc = 320;

			if (!DefaultResolutions())
			{
				helpPopup_1804B0.type_sub_0[0].maxTextboxWidth_0x0 = screenWidth_18062C / 2;
				helpPopup_1804B0.type_sub_0[0].maxTextboxHeight_0x2 = screenHeight_180624 / 6; // 80 on the original 480 high screen
				helpPopup_1804B0.type_sub_0[0].maxTextboxWidth2_0xc = screenWidth_18062C / 2;
			}

			ComputeTextboxSizesFromTextWords_89420(helpPopup_1804B0.type_sub_0, textForDraw, scale);
			ConstrainTextboxSizes_89520(helpPopup_1804B0.type_sub_0, scale);
			ComputeFrameSizes_89980(helpPopup_1804B0.type_sub_0, scale);
		}
		ColorizeScreen_2E790(
			helpPopup_1804B0.type_sub_0[0].framePosX_0x24,
			helpPopup_1804B0.type_sub_0[0].framePosY_0x26,
			helpPopup_1804B0.type_sub_0[0].frameWidth_0x28,
			helpPopup_1804B0.type_sub_0[0].frameHeight_0x2a,
			helpPopup_1804B0.type_sub_0[0].color2_0x31);
		AlphaRectangle(
			helpPopup_1804B0.type_sub_0[0].framePosX_0x24,
			helpPopup_1804B0.type_sub_0[0].framePosY_0x26,
			helpPopup_1804B0.type_sub_0[0].frameWidth_0x28,
			helpPopup_1804B0.type_sub_0[0].frameHeight_0x2a,
			100
		);
		DrawTextboxText_895D0(helpPopup_1804B0.type_sub_0, textForDraw, scale);
		DrawTextboxFrame_89690(helpPopup_1804B0.type_sub_0, scale);
		helpPopup_1804B0.PopupStatusByte_0x9e &= 0xF7;
		break;
	}
	case 2: // Draws Mini Map, Top Status Bar and Control Help
	{
		helpPopup_1804B0.type_sub_0[0].lineSrcX_0x1c = str_E2A74[helpPopup_1804B0.Index_0x86].axis_2[3];
		helpPopup_1804B0.type_sub_0[0].maxTextboxWidth2_0xc = 220 * scale;
		helpPopup_1804B0.type_sub_0[0].lineSrcY_0x1e = str_E2A74[helpPopup_1804B0.Index_0x86].axis_2[4];
		if (helpPopup_1804B0.PopupStatusByte_0x9e & 0x8)
		{
			ComputeTextboxSizes_89830(helpPopup_1804B0.type_sub_0, scale);//here
			ComputeTextboxSizesFromTextWords_89420(helpPopup_1804B0.type_sub_0, textForDraw, scale);
			ConstrainTextboxSizes_89520(helpPopup_1804B0.type_sub_0, scale);
			ComputeFrameSizes_89980(helpPopup_1804B0.type_sub_0, scale);
		}
		ComputeTextboxLine_898A0(helpPopup_1804B0.type_sub_0);
		ColorizeScreen_2E790(
			helpPopup_1804B0.type_sub_0[0].framePosX_0x24,
			helpPopup_1804B0.type_sub_0[0].framePosY_0x26,
			helpPopup_1804B0.type_sub_0[0].frameWidth_0x28,
			helpPopup_1804B0.type_sub_0[0].frameHeight_0x2a,
			helpPopup_1804B0.type_sub_0[0].color2_0x31);
		AlphaRectangle(
			helpPopup_1804B0.type_sub_0[0].framePosX_0x24,
			helpPopup_1804B0.type_sub_0[0].framePosY_0x26,
			helpPopup_1804B0.type_sub_0[0].frameWidth_0x28,
			helpPopup_1804B0.type_sub_0[0].frameHeight_0x2a,
			100
		);
		DrawTextboxText_895D0(helpPopup_1804B0.type_sub_0, textForDraw, scale);
		DrawTextboxLine_89A30(helpPopup_1804B0.type_sub_0);
		DrawTextboxFrame_89690(helpPopup_1804B0.type_sub_0, scale);
		helpPopup_1804B0.PopupStatusByte_0x9e &= 0xF7;
		break;
	}
	case 3: //Draw Spell Menu Help
	{
		helpPopup_1804B0.type_sub_0[0].lineSrcX_0x1c = str_E2A74[helpPopup_1804B0.Index_0x86].axis_2[3];//3
		helpPopup_1804B0.type_sub_0[0].lineSrcY_0x1e = str_E2A74[helpPopup_1804B0.Index_0x86].axis_2[4];
		ComputeTextboxSizes_89830(helpPopup_1804B0.type_sub_0, scale);
		ComputeTextboxSizesFromTextLines_89920(helpPopup_1804B0.type_sub_0, helpPopup_1804B0.lineCount_0xa0, &helpPopup_1804B0.uni_0x8a.word[2], scale);
		ConstrainTextboxSizes_89520(helpPopup_1804B0.type_sub_0, scale);
		ComputeFrameSizes_89980(helpPopup_1804B0.type_sub_0, scale);
		ComputeTextboxLine_898A0(helpPopup_1804B0.type_sub_0);
		v4 = 0;
		ColorizeScreen_2E790(
			helpPopup_1804B0.type_sub_0[0].framePosX_0x24,
			helpPopup_1804B0.type_sub_0[0].framePosY_0x26,
			helpPopup_1804B0.type_sub_0[0].frameWidth_0x28,
			helpPopup_1804B0.type_sub_0[0].frameHeight_0x2a,
			helpPopup_1804B0.type_sub_0[0].color2_0x31);
		AlphaRectangle(
			helpPopup_1804B0.type_sub_0[0].framePosX_0x24,
			helpPopup_1804B0.type_sub_0[0].framePosY_0x26,
			helpPopup_1804B0.type_sub_0[0].frameWidth_0x28,
			helpPopup_1804B0.type_sub_0[0].frameHeight_0x2a,
			100
		);
		posY = helpPopup_1804B0.type_sub_0[0].textboxPosY_0xa;
		while (v4 < helpPopup_1804B0.lineCount_0xa0)
		{
			if (v4 == helpPopup_1804B0.highlightLine_0xa1)
				color = helpPopup_1804B0.type_sub_0[0].color3_0x32;
			else
				color = helpPopup_1804B0.type_sub_0[0].color1_0x30;
			//FireBall,Rapid Fire,Fire Storm
			DrawText_2BC10(langStrings_E9C4C[helpPopup_1804B0.uni_0x8a.word[2 + v4++]], helpPopup_1804B0.type_sub_0[0].textboxPosX_0x8, posY, color, scale);
			posY += helpPopup_1804B0.type_sub_0[0].charHeight_0x12 * scale;
		}
		DrawTextboxLine_89A30(helpPopup_1804B0.type_sub_0);
		DrawTextboxFrame_89690(helpPopup_1804B0.type_sub_0, scale);
		helpPopup_1804B0.PopupStatusByte_0x9e &= 0xF7;
		break;
	}
	default:
	{
		helpPopup_1804B0.PopupStatusByte_0x9e &= 0xF7;
		break;
	}
	}
}

//----- (00087860) --------------------------------------------------------
void DrawPauseMenuPopUps_87860()//268860
{
	uint8_t scale = 1;

	if (!DefaultResolutions())
	{
		scale = gameUiScale;
	}

	if (helpPopup_1804B0.pauseMenuItem_0xaa >= 0)
	{
		SetPauseMenuCoordinates_87970(scale);
		//Options | Load Level | Save Level | Music Volume | Sound Volume
		helpPopup_1804B0.type_sub_0[1].lineSrcX_0x1c = str_E2A74[(helpPopup_1804B0.pauseMenuItem_0xaa + 86)].axis_2[3];
		helpPopup_1804B0.type_sub_0[1].lineSrcY_0x1e = str_E2A74[(helpPopup_1804B0.pauseMenuItem_0xaa + 86)].axis_2[4];
		helpPopup_1804B0.type_sub_0[1].color1_0x30 = (*xadataclrd0dat.colorPalette_var28)[4095];
		helpPopup_1804B0.type_sub_0[1].maxTextboxWidth2_0xc = 220 * scale;
		helpPopup_1804B0.type_sub_0[1].color2_0x31 = (*xadataclrd0dat.colorPalette_var28)[0];
		ComputeTextboxSizes_89830(&helpPopup_1804B0.type_sub_0[1], scale);
		ComputeTextboxSizesFromTextWords_89420(&helpPopup_1804B0.type_sub_0[1], langStrings_E9C4C[str_E2A74[(helpPopup_1804B0.pauseMenuItem_0xaa + 86)].axis_2[1]], scale);
		ConstrainTextboxSizes_89520(&helpPopup_1804B0.type_sub_0[1], scale);
		ComputeFrameSizes_89980(&helpPopup_1804B0.type_sub_0[1], scale);
		ComputeTextboxLine_898A0(&helpPopup_1804B0.type_sub_0[1]);
		ColorizeScreen_2E790(helpPopup_1804B0.type_sub_0[1].framePosX_0x24, helpPopup_1804B0.type_sub_0[1].framePosY_0x26, helpPopup_1804B0.type_sub_0[1].frameWidth_0x28, helpPopup_1804B0.type_sub_0[1].frameHeight_0x2a, helpPopup_1804B0.type_sub_0[1].color2_0x31);
		DrawTextboxText_895D0(&helpPopup_1804B0.type_sub_0[1], (char*)langStrings_E9C4C[str_E2A74[(helpPopup_1804B0.pauseMenuItem_0xaa + 86)].axis_2[1]], scale);
		DrawTextboxLine_89A30(&helpPopup_1804B0.type_sub_0[1]);
		DrawTextboxFrame_89690(&helpPopup_1804B0.type_sub_0[1], scale);
	}
}

//----- (00087970) --------------------------------------------------------
void SetPauseMenuCoordinates_87970(uint8_t scale)//268970
{
	int16_t height; // [esp+0h] [ebp-10h]
	int16_t width; // [esp+4h] [ebp-Ch]
	int16_t posY; // [esp+8h] [ebp-8h]
	int16_t posX; // [esp+Ch] [ebp-4h]

	GetPauseMenuCoordinates_2FFE0(&posX, &posY, &width, &height, scale);
	str_E2A74[0x56].axis_2[3] = width + posX - (12 * scale);//0x56 tj 0xa14 8
	str_E2A74[0x56].axis_2[4] = posY + (6 * scale);//0x56 tj 0xa14 a
	str_E2A74[0x57].axis_2[3] = width + posX - (12 * scale);//0x57 tj 0xa32 8
	str_E2A74[0x57].axis_2[4] = height + posY + (6 * scale);//0x57 tj 0xa32 a
	str_E2A74[0x58].axis_2[3] = posX + width / 2 - (12 * scale);//0x58 tj 0xa50 8
	str_E2A74[0x58].axis_2[4] = posY + 2 * height + (6 * scale);//0x58 tj 0xa50 a
	str_E2A74[0x59].axis_2[3] = width + posX - (12 * scale);//0x59 tj 0xa6e 8
	str_E2A74[0x59].axis_2[4] = posY + 2 * height + (6 * scale);//0x59 tj 0xa6e a
	str_E2A74[0x5a].axis_2[3] = width + posX - (12 * scale);//0x5a tj 0xa8c 8
	str_E2A74[0x5a].axis_2[4] = posY + 3 * height + (6 * scale);//0x5a tj 0xa8c a
}


//----- (00087A30) --------------------------------------------------------
void SetHelpPopupLine_87A30()//268a30
{
	uint8_t scale = 1;
	if (!DefaultResolutions())
		scale = gameUiScale;

	if (helpPopup_1804B0.keyHelpItem_0xa9 > 0)
	{
		helpPopup_1804B0.type_sub_0[1].lineSrcX_0x1c = str_E2A74[helpPopup_1804B0.keyHelpItem_0xa9].axis_2[3];
		helpPopup_1804B0.type_sub_0[1].lineSrcY_0x1e = str_E2A74[helpPopup_1804B0.keyHelpItem_0xa9].axis_2[4];
		const bool isChatHelp = helpPopup_1804B0.keyHelpItem_0xa9 >= 97 && helpPopup_1804B0.keyHelpItem_0xa9 <= 102;
		int16_t chatPosX = 0;
		int16_t chatPosY = 0;
		if (isChatHelp)
		{
			GetChatMenuCoordinates(&chatPosX, &chatPosY, scale);
			helpPopup_1804B0.type_sub_0[1].lineSrcX_0x1c = chatPosX + (helpPopup_1804B0.type_sub_0[1].lineSrcX_0x1c * scale);
			helpPopup_1804B0.type_sub_0[1].lineSrcY_0x1e = chatPosY + ((helpPopup_1804B0.type_sub_0[1].lineSrcY_0x1e - (400 - CHAT_MENU_HEIGHT)) * scale);
		}
		helpPopup_1804B0.type_sub_0[1].color1_0x30 = (*xadataclrd0dat.colorPalette_var28)[4095];
		helpPopup_1804B0.type_sub_0[1].maxTextboxWidth2_0xc = 220 * scale;
		helpPopup_1804B0.type_sub_0[1].color2_0x31 = (*xadataclrd0dat.colorPalette_var28)[0];
		ComputeTextboxSizes_89830(&helpPopup_1804B0.type_sub_0[1], scale);
		ComputeTextboxSizesFromTextWords_89420(&helpPopup_1804B0.type_sub_0[1], langStrings_E9C4C[str_E2A74[helpPopup_1804B0.keyHelpItem_0xa9].axis_2[1]], scale);
		ConstrainTextboxSizes_89520(&helpPopup_1804B0.type_sub_0[1], scale);
		ComputeFrameSizes_89980(&helpPopup_1804B0.type_sub_0[1], scale);
		if (isChatHelp)
		{
			Type_TextBox_1804B0& textbox = helpPopup_1804B0.type_sub_0[1];
			const int overlap = (textbox.framePosY_0x26 + textbox.frameHeight_0x2a) - (chatPosY - (2 * scale));
			if (overlap > 0)
			{
				textbox.textboxPosY_0xa -= overlap;
				textbox.framePosY_0x26 -= overlap;
				textbox.maxTextboxHeight_0x2 -= overlap;
			}
		}
		ComputeTextboxLine_898A0(&helpPopup_1804B0.type_sub_0[1]);
		ColorizeScreen_2E790(helpPopup_1804B0.type_sub_0[1].framePosX_0x24, helpPopup_1804B0.type_sub_0[1].framePosY_0x26, helpPopup_1804B0.type_sub_0[1].frameWidth_0x28, helpPopup_1804B0.type_sub_0[1].frameHeight_0x2a, helpPopup_1804B0.type_sub_0[1].color2_0x31);
		DrawTextboxText_895D0(&helpPopup_1804B0.type_sub_0[1], (char*)langStrings_E9C4C[str_E2A74[helpPopup_1804B0.keyHelpItem_0xa9].axis_2[1]], scale);
		DrawTextboxLine_89A30(&helpPopup_1804B0.type_sub_0[1]);
		DrawTextboxFrame_89690(&helpPopup_1804B0.type_sub_0[1], scale);
	}
}

//----- (00087B30) --------------------------------------------------------
void UpdateHelpPopupTimeout_87B30()//268b30
{
	if (++helpPopup_1804B0.popupTimer_0x76 >= helpPopup_1804B0.popupTimeout_0x7a || !(str_E2A74[helpPopup_1804B0.Index_0x86].axis_2[0] & 1))
		CloseHelpPopup_87C10();
}

//----- (00087B70) --------------------------------------------------------
void UpdateHelpItemTimers_87B70()//268b70
{
	int indexx;
	int i; // ebx
	int v4; // edx

	indexx = 1;
	for (i = 0; i < helpPopup_1804B0.helpItemCount_0x66; indexx++)
	{
		str_E2A74[indexx].counter_16++;
		v4 = str_E2A74[indexx].counter_16 - str_E2A74[indexx].interval_24;
		if (v4 > 0 && v4 > 2 * str_E2A74[indexx].interval_24)
		{
			str_E2A74[indexx].counter_16 = str_E2A74[indexx].interval_24;
			str_E2A74[indexx].stage_29++;
			if (str_E2A74[indexx].stage_29 < 0)
				str_E2A74[indexx].stage_29 = 0;
			if (str_E2A74[indexx].stage_29 > 5)
				str_E2A74[indexx].stage_29 = 5;
		}
		i++;
	}
}

//----- (00087BE0) --------------------------------------------------------
void ClearHelpItemFlags_87BE0()//268be0
{
	char v2; // bl

	int index = 0;
	for (int i = 0; i < helpPopup_1804B0.helpItemCount_0x66; str_E2A74[index].axis_2[0] = v2 & 0xFE)
	{
		v2 = str_E2A74[index + 1].axis_2[0];
		index++;
		i++;
	}//shift array_E2A74[2] by 1
}

//----- (00087CF0) --------------------------------------------------------
void GetHelpPopupTextAndCoords_87CF0(uint8_t scale)//268cf0
{
	signed int v0; // ecx
	type_entity_0x6E8E* v1x; // esi
	unsigned __int16 v3; // bx
	type_entity_0x6E8E* v5x; // eax
	signed int v6; // ebx
	int v7; // eax
	int v9x;
	bool v13; // zf
	int v14; // eax
	bool v16; // bl
	type_entity_0x6E8E* v17x; // edx
	type_entity_0x6E8E* v18x; // edx
	bool v19; // al
	char v20; // al
	signed int v21; // edx
	int v22x;
	signed int v23; // edx
	int v24x;
	char v25; // ah
	signed int v27; // edx
	int v28x;
	int v29; // [esp+0h] [ebp-28h]
	bool v30; // [esp+8h] [ebp-20h]
	bool v31; // [esp+Ch] [ebp-1Ch]
	bool v32; // [esp+10h] [ebp-18h]
	signed int v33; // [esp+14h] [ebp-14h]
	int v34; // [esp+18h] [ebp-10h]
	uint32_t v35; // [esp+1Ch] [ebp-Ch]
	type_entity_0x6E8E* v36x; // [esp+20h] [ebp-8h]
	signed int v37; // [esp+24h] [ebp-4h]

	// The in-game UI's layout moves the top bar and the spell icons (and their help pointers)
	// from their original positions: the top bar at the top left, the spell icons at the top right
	int topBarOffsetX = 0;
	int topBarOffsetY = 0;
	int spellIconsOffsetX = 0;
	int spellIconsOffsetY = 0;
	if (UseInGameUiLayout())
	{
		int16_t topBarPosX, topBarPosY, spellLeftPosX, spellRightPosX, spellIconsPosY;
		GetTopBarPosition(&topBarPosX, &topBarPosY);
		GetSpellIconsPosition(&spellLeftPosX, &spellRightPosX, &spellIconsPosY);
		topBarOffsetX = topBarPosX;
		topBarOffsetY = topBarPosY;
		spellIconsOffsetX = spellLeftPosX - (screenWidth_18062C - (130 * scale));
		spellIconsOffsetY = spellIconsPosY - (2 * scale);
	}

	v34 = 0;
	v37 = -1;
	v0 = -1;
	v1x = Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
	v3 = v1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58;
	if (v3)
		v5x = Entities_EA3E4[v3];
	else
		v5x = 0;
	v36x = v5x;
	v6 = 0;
	v29 = 0;
	while (v6 < 26)
	{
		v7 = v1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[v6++] != 0;
		v29 += v7;
	}
	if (v29)
	{
		v30 = v1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[2] != 0;
		v32 = v1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[1] != 0;
		v31 = v1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[0] != 0;
		v0 = v1x->playerData_0xA4_164x->str_611.SpellIndexLeft_0x451_1105;
		v37 = v1x->playerData_0xA4_164x->str_611.SpellIndexRight_0x453_1107;
	}
	if (world_D41A0.playerObjectives_0x3659C[world_D41A0.LevelIndex_0xc].substr_3659C.IsLevelEnd_0)
		v33 = -1;
	else
		v33 = world_D41A0.stages_0x3654C[world_D41A0.stages_0x3654C[world_D41A0.LevelIndex_0xc].flags_3654D].stages_3654C_byte0;
	v35 = 1;
	v9x = 1;
	while ((signed int)v35 <= helpPopup_1804B0.helpItemCount_0x66)
	{
		switch (str_E2A74[v9x].type_0)
		{
		case 1:
			if (v35 >= 1 && (v35 <= 2 || v35 == 92))
				goto LABEL_32;
			break;
		case 2:
			if (str_E2A74[v9x].axis_2[0] & 0x18)
				str_E2A74[v9x].axis_2[0] |= 1;
			break;
		case 3:
			if (v36x)
			{
				if (v35 != 35)
					goto LABEL_32;
				if (!v33)
				{
					str_E2A74[v9x].axis_2[0] |= 1u;
					str_E2A74[35].axis_2[3] = ((world_D41A0.terrain_2FECE.manaGoalPercent_0x2FED5 << 6) / 100
						+ (*filearray_2aa18c[filearrayindex_FONTS1DATTAB].posistruct)[40].width_4
						+ 60) * scale + topBarOffsetX;
					str_E2A74[35].axis_2[4] = 38 * scale + topBarOffsetY;
				}
			}
			break;
		case 5:
		case 7:
		case 0xC:
		case 0xD:
			break;
		case 6:
			v13 = (helpPopup_1804B0.PopupStatusByte_0x9e & 2) == 0;
			goto LABEL_31;
		case 8:
			if (str_E2A74[v9x].axis_2[0] & 0x10 || str_E2A74[v9x].axis_2[0] & 0x40)
				str_E2A74[v9x].axis_2[0] |= 1u;
			if (str_E2A74[v9x].axis_2[0] & 0x40)
				v34++;
			break;
		default:
			v13 = v35 == 85;
		LABEL_31:
			if (!v13)
				LABEL_32 :
				str_E2A74[v9x].axis_2[0] |= 1u;
			break;
		}
		v9x++;
		v35++;
	}
	if (helpPopup_1804B0.PopupStatusByte_0x9e & 4)
	{
		//Map screen open
		str_E2A74[2].axis_2[3] = (190 * scale);
		str_E2A74[2].axis_2[4] = (202 * scale);
	}
	else
	{
		//You are here
		str_E2A74[2].axis_2[3] = (64 * scale) + topBarOffsetX;
		str_E2A74[2].axis_2[4] = (66 * scale) + topBarOffsetY;
	}
	if (!v34 && !(helpPopup_1804B0.PopupStatusByte_0x9e & 4))
		str_E2A74[4].axis_2[0] |= 1u;
	if (v1x->maxLife_0x4 >> 2 >= v1x->life_0x8)
		str_E2A74[5].axis_2[0] |= 1u;
	if (v1x->playerData_0xA4_164x->waterCounter_0x262_610)
		str_E2A74[3].axis_2[0] |= 1u;
	str_E2A74[10].axis_2[0] |= 4u;
	str_E2A74[11].axis_2[0] |= 4u;
	str_E2A74[12].axis_2[0] |= 4u;
	if (v29)
	{
		v14 = helpPopup_1804B0.PopupStatusByte_0x9e & 0x40;
		if (helpPopup_1804B0.PopupStatusByte_0x9e & 0x80)
		{
			if (v31)
			{
				str_E2A74[10].axis_2[0] |= 1u;
				if (v0 && v37)
				{
					str_E2A74[14].axis_2[0] |= 1u;
				}
				else
				{
					//Currently selected Spell Help
					str_E2A74[10].axis_2[0] &= 0xFBu;
					if (v0)
						str_E2A74[10].axis_2[3] = 600;
					else
						str_E2A74[10].axis_2[3] = 540;

					if (!DefaultResolutions())
					{
						if (v0)
							str_E2A74[10].axis_2[3] = screenWidth_18062C - (40 * scale) + spellIconsOffsetX;
						else
							str_E2A74[10].axis_2[3] = screenWidth_18062C - (100 * scale) + spellIconsOffsetX;
					}

					str_E2A74[10].axis_2[4] = (32 * scale) + spellIconsOffsetY;
				}
			}
			else
			{
				str_E2A74[7].axis_2[0] |= 1u;
			}
			helpPopup_1804B0.PopupStatusByte_0x9e &= 0x7F;
		}
		if (v14)
		{
			if (v32)
			{
				str_E2A74[11].axis_2[0] |= 1u;
				if (v0 != 1 && v37 != 1)
				{
					str_E2A74[14].axis_2[0] |= 1u;
				}
				else
				{
					//Currently selected Spell Help
					str_E2A74[11].axis_2[0] &= 0xFBu;
					if (v0 == 1)
						str_E2A74[11].axis_2[3] = 540;
					else
						str_E2A74[11].axis_2[3] = 600;

					if (!DefaultResolutions())
					{
						if (v0 == 1)
							str_E2A74[11].axis_2[3] = screenWidth_18062C - (100 * scale) + spellIconsOffsetX;
						else
							str_E2A74[11].axis_2[3] = screenWidth_18062C - (40 * scale) + spellIconsOffsetX;
					}

					str_E2A74[11].axis_2[4] = (32 * scale) + spellIconsOffsetY;
				}
			}
			else
			{
				str_E2A74[9].axis_2[0] |= 1u;
			}
			helpPopup_1804B0.PopupStatusByte_0x9e &= 0xBF;
		}
		if (v30)
		{
			if (!v36x)
			{
				str_E2A74[12].axis_2[0] |= 1u;
				if (v0 != 2 && v37 != 2)
				{
					str_E2A74[13].axis_2[0] |= 1u;
				}
				else
				{
					//Currently selected Spell Help
					str_E2A74[12].axis_2[0] &= 0xFBu;
					if (v0 == 2)
						str_E2A74[12].axis_2[3] = 540;
					else
						str_E2A74[12].axis_2[3] = 600;

					if (!DefaultResolutions())
					{
						if (v0 == 2)
							str_E2A74[12].axis_2[3] = screenWidth_18062C - (100 * scale) + spellIconsOffsetX;
						else
							str_E2A74[12].axis_2[3] = screenWidth_18062C - (40 * scale) + spellIconsOffsetX;
					}

					str_E2A74[12].axis_2[4] = (32 * scale) + spellIconsOffsetY;
				}
			}
		}
		else
		{
			str_E2A74[8].axis_2[0] |= 1u;
		}
	}
	else
	{
		str_E2A74[6].axis_2[0] |= 1u;
	}
	if (v36x)
	{
		if (v1x->mana_0x90_144 >= Entities_EA3E4[v1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[2]]->maxMana_0x8C_140 && v36x->param_0x10_16 < 6)
			str_E2A74[14].axis_2[0] |= 1u;
		if (v1x->playerData_0xA4_164x->castleDestroyedBlink_0x195_405)
			str_E2A74[17].countdown_20 = 100;
		if (str_E2A74[17].countdown_20)
			(str_E2A74[17].countdown_20)--;
		if (str_E2A74[17].countdown_20)
			str_E2A74[17].axis_2[0] |= 1u;
	}
	if (v29)
	{
		if (v0 > -1)
			str_E2A74[38].axis_2[0] |= 1u;
		if (v37 > -1)
			str_E2A74[39].axis_2[0] |= 1u;
		if (v0 > -1)
		{
			//Your current objective is to fly to a point
			str_E2A74[40].axis_2[3] = 540;
			str_E2A74[40].axis_2[0] |= 1u;
			str_E2A74[40].axis_2[4] = (40 * scale) + spellIconsOffsetY;
			str_E2A74[42].axis_2[0] |= 1u;
			str_E2A74[42].axis_2[4] = (16 * scale) + spellIconsOffsetY;
			str_E2A74[42].axis_2[3] = 560;

			if (!DefaultResolutions())
			{
				str_E2A74[40].axis_2[3] = screenWidth_18062C - (100 * scale) + spellIconsOffsetX;
				str_E2A74[42].axis_2[3] = screenWidth_18062C - (80 * scale) + spellIconsOffsetX;
			}
		}
		v16 = 0;
		if (v0 > -1)
		{
			v17x = Entities_EA3E4[v1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[v0]];
			if (v0 == 2)
			{
				if (v1x->mana_0x90_144 < v17x->maxMana_0x8C_140)
					v16 = 1;
			}
			else
			{
				v16 = CanSpellOwnerCast_68D50(v17x, v1x) == 0;
			}
			if (v16)
			{
				str_E2A74[41].axis_2[3] = 540;
				str_E2A74[41].axis_2[4] = (48 * scale) + spellIconsOffsetY;
				str_E2A74[41].axis_2[0] |= 1u;

				if (!DefaultResolutions())
				{
					str_E2A74[41].axis_2[3] = screenWidth_18062C - (100 * scale) + spellIconsOffsetX;
				}
			}
		}
		if (v37 > -1 && !v16)
		{
			v18x = Entities_EA3E4[v1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[v37]];
			v19 = v37 == 2 ? v1x->mana_0x90_144 < v18x->maxMana_0x8C_140 : CanSpellOwnerCast_68D50(v18x, v1x) == 0;
			if (v19)
			{
				str_E2A74[41].axis_2[0] |= 1u;
				str_E2A74[41].axis_2[4] = (48 * scale) + spellIconsOffsetY;
				str_E2A74[41].axis_2[3] = 600;

				if (!DefaultResolutions())
				{
					str_E2A74[41].axis_2[3] = screenWidth_18062C - (40 * scale) + spellIconsOffsetX;
				}
			}
		}
	}
	if (v33 == -1)
	{
		v20 = 1;
		str_E2A74[80].axis_2[0] |= 1u;
		if (str_E2A74[83].axis_2[0] & 0x40)
			str_E2A74[83].axis_2[0] |= 1u;
	}
	else
	{
		v20 = 0;
		switch (v33)
		{
		case 0:
			str_E2A74[72].axis_2[0] |= 1u;
			break;
		case 1:
			str_E2A74[75].axis_2[0] |= 1u;
			break;
		case 2:
			str_E2A74[73].axis_2[0] |= 1u;
			break;
		case 3:
			str_E2A74[77].axis_2[0] |= 1u;
			break;
		case 5:
			str_E2A74[79].axis_2[0] |= 1u;
			break;
		case 7:
			str_E2A74[76].axis_2[0] |= 1u;
			break;
		case 8:
			str_E2A74[78].axis_2[0] |= 1u;
			break;
		case 9:
			str_E2A74[74].axis_2[0] |= 1u;
			break;
		default:
			break;
		}
	}
	if (helpPopup_1804B0.uni_0x8a.dword[0] > 100)
	{
		str_E2A74[15].axis_2[0] |= 1u;
		str_E2A74[16].axis_2[0] |= 1u;
	}
	if (v20)
	{
		v21 = 1;
		v22x = 1;
		while (v21 <= helpPopup_1804B0.helpItemCount_0x66)
		{
			if (str_E2A74[v22x].axis_2[0] & 0x4ff)
				str_E2A74[v22x].axis_2[0] &= 0xFEu;
			v21++;
			v22x++;
		}
	}
	if (helpPopup_1804B0.PopupStatusByte_0x9e & 4)
	{
		v23 = 1;
		v24x = 1;
		while (v23 <= helpPopup_1804B0.helpItemCount_0x66)
		{
			v25 = str_E2A74[v24x].axis_2[0];
			if (v25 & 0x20)
				str_E2A74[v24x].axis_2[0] = v25 & 0xFE;
			v23++;
			v24x++;
		}
	}
	if (!world_D41A0.m_GameSettings.m_Display.m_wMiniMap)
	{
		v27 = 1;
		v28x = 1;
		while (v27 <= helpPopup_1804B0.helpItemCount_0x66)
		{
			if (str_E2A74[v28x].axis_2[0] & 0x2ff)
				str_E2A74[v28x].axis_2[0] &= 0xFEu;
			v27++;
			v28x++;
		}
	}

	// Correct locations for "This is your life" 36 and "This is your Spells Mana" 37
	// "Click on LMB to cast this spell" 38, "Click on RMB to cast this spell" 39
	if (!DefaultResolutions())
	{
		str_E2A74[5].axis_2[3] = 470 * scale + topBarOffsetX;
		str_E2A74[5].axis_2[4] = 16 * scale + topBarOffsetY;
		str_E2A74[29].axis_2[3] = 214 * scale + topBarOffsetX; // Castled mana, current and capacity
		str_E2A74[29].axis_2[4] = 32 * scale + topBarOffsetY;
		str_E2A74[30].axis_2[3] = 214 * scale + topBarOffsetX; // Castle life
		str_E2A74[30].axis_2[4] = 16 * scale + topBarOffsetY;
		str_E2A74[31].axis_2[3] = 130 * scale + topBarOffsetX; // Current size of castle
		str_E2A74[31].axis_2[4] = 16 * scale + topBarOffsetY;
		str_E2A74[32].axis_2[3] = 320 * scale + topBarOffsetX; // Mana held by balloons
		str_E2A74[32].axis_2[4] = 32 * scale + topBarOffsetY;
		str_E2A74[33].axis_2[3] = 260 * scale + topBarOffsetX; // Number of balloons
		str_E2A74[33].axis_2[4] = 16 * scale + topBarOffsetY;
		str_E2A74[34].axis_2[3] = 320 * scale + topBarOffsetX; // Balloons life
		str_E2A74[34].axis_2[4] = 16 * scale + topBarOffsetY;
		str_E2A74[36].axis_2[3] = 470 * scale + topBarOffsetX;
		str_E2A74[36].axis_2[4] = 16 * scale + topBarOffsetY;
		str_E2A74[37].axis_2[3] = 470 * scale + topBarOffsetX;
		str_E2A74[37].axis_2[4] = 32 * scale + topBarOffsetY;
		str_E2A74[38].axis_2[3] = screenWidth_18062C - (100 * scale) + spellIconsOffsetX;
		str_E2A74[38].axis_2[4] = 32 * scale + spellIconsOffsetY;
		str_E2A74[39].axis_2[3] = screenWidth_18062C - (40 * scale) + spellIconsOffsetX;
		str_E2A74[39].axis_2[4] = 32 * scale + spellIconsOffsetY;
	}
}

//----- (00088450) --------------------------------------------------------
int16_t GetHelpPopupIndex_88450()//269450
{
	signed int v0; // ebx
	int v1; // edx
	//uint8_t* v2; // eax
	int v2x;
	int v3; // ecx
	int v4; // edi

	/*uint8_t origbyte2z;
	uint8_t remakebyte2z;
	uint8_t origbyte2y[100];
	uint8_t remakebyte2y[100];
	int remakepos2y[100];
	int comp15 = compare_with_sequence((char*)"00269450", (uint8_t*)unk_1804B0ar, 0x3514b0, debugcounter_GetHelpPopupIndex_88450, 0xb0, &origbyte2z, &remakebyte2z);
	int comp16 = compare_with_sequence_array_E2A74((char*)"00269450-2", (uint8_t*)array_E2A74, 0x2b3a74, debugcounter_GetHelpPopupIndex_88450, 0xc4e, origbyte2y, remakebyte2y, remakepos2y);
	//int comp16 = compare_with_sequence_array_E2A74((char*)"00269450", (uint8_t*)array_E2A74, 0x2b3a74, debugcounter11, 0x1000, origbyte2y, remakebyte2y, remakepos2y);
	//debugcounter_SetHelpPopupCoords_87580++;
	if (comp15 < 0xb0)
		comp15 = comp15;
	if (comp16 < 0xc4e)
		comp16 = comp16;*/

		/*if (debugcounter_GetHelpPopupIndex_88450 >= 0x10)
		{
			//myprintf("i:%d,", debugcounter_GetHelpPopupIndex_88450);
		}*/

	if (helpPopup_1804B0.pendingHelpIndex_0x88)
	{
		v1 = helpPopup_1804B0.pendingHelpIndex_0x88;
		helpPopup_1804B0.pendingHelpIndex_0x88 = 0;
	}
	else
	{
		v0 = 6;
		v1 = 0;
		while (v0 >= 0 && !v1)
		{
			//v2 = &array_E2A74[30];
			v2x = 1;
			v3 = 0;
			v4 = 0;
			while (v3 < helpPopup_1804B0.helpItemCount_0x66)
			{
				if (str_E2A74[v2x].axis_2[0] & 1
					&& v0 == str_E2A74[v2x].stage_29
					&& str_E2A74[v2x].counter_16 - str_E2A74[v2x].interval_24 > 0
					&& str_E2A74[v2x].counter_16 - str_E2A74[v2x].interval_24 > v4
					&& v3 != 0x5b)//fixed
				{
					v1 = v3 + 1;
					v4 = str_E2A74[v2x].counter_16 - str_E2A74[v2x].interval_24;
					//adress 269497
				}
				v3++;
				//v2 += 0x1e;
				v2x++;
			}
			v0--;
		}
	}
	//if (v1 == 0x5c)
		//v1 = v1;//fix
	/*
	//adress 2694bc
	int comp17 = compare_with_sequence((char*)"002694BC", (uint8_t*)unk_1804B0ar, 0x3514b0, debugcounter_GetHelpPopupIndex_88450, 0xb0, &origbyte2z, &remakebyte2z);
	int comp18 = compare_with_sequence_array_E2A74((char*)"002694BC-2", (uint8_t*)array_E2A74, 0x2b3a74, debugcounter_GetHelpPopupIndex_88450, 0xc4e, origbyte2y, remakebyte2y, remakepos2y);
	//int comp16 = compare_with_sequence_array_E2A74((char*)"00269450", (uint8_t*)array_E2A74, 0x2b3a74, debugcounter11, 0x1000, origbyte2y, remakebyte2y, remakepos2y);
	//debugcounter_SetHelpPopupCoords_87580++;
	if (comp17 < 0xb0)
		comp17 = comp17;
	if (comp18 < 0xc4e)
		comp18 = comp18;
	debugcounter_GetHelpPopupIndex_88450++;*/
	return v1;
}
// E2A92: using guessed type void *off_E2A92;
// 180516: using guessed type int x_DWORD_180516;
// 180538: using guessed type __int16 x_WORD_180538;

//----- (000884D0) --------------------------------------------------------
void SetHelpPopupTextAndCoords_884D0(int16_t helpIdx, int16_t a2, int16_t* popupSrcPos, char a4, char a5, uint8_t scale)//2694d0
{
	if (helpIdx)
	{
		helpPopup_1804B0.popupTimeout_0x7a = str_E2A74[helpIdx].axis_2[2];
		helpPopup_1804B0.drawTextType_0xa3 = a4;
		helpPopup_1804B0.Index_0x86 = helpIdx;
		helpPopup_1804B0.PopupStatusByte_0x9e |= 8;
		helpPopup_1804B0.highlightLine_0xa1 = a5;
		helpPopup_1804B0.popupTimer_0x76 = 0;
		helpPopup_1804B0.lineCount_0xa0 = a2;
		for (int i = 0; i < a2; i++)
			helpPopup_1804B0.uni_0x8a.word[i + 2] = popupSrcPos[i];

		str_E2A74[helpIdx].axis_2[0] |= 3;
		if (str_E2A74[helpIdx].axis_2[0] & 8)
		{
			helpPopup_1804B0.PopupStatusByte_0x9e |= 0x20;
		}
		else if (str_E2A74[helpIdx].axis_2[0] & 0x10)
		{
			helpPopup_1804B0.popupFlags_0x9f |= 1u;
		}
	}
}

//----- (00088580) --------------------------------------------------------
void ResetHelpItemFlags_88580()//269580
{
	signed int v0; // edx
	//uint8_t* result; // eax
	int index;

	v0 = 1;
	//for (result = &array_E2A74[30]; v0 <= helpPopup_1804B0.dword_0x66; result += 30)
	for (index = 1; v0 <= helpPopup_1804B0.helpItemCount_0x66; index++)
	{
		//if (*(int16_t*)result >= 2u && (*(int16_t*)result <= 2u || *(int16_t*)result == 8))
		if (str_E2A74[index].type_0 == 2 || str_E2A74[index].type_0 == 8)
		{
			str_E2A74[index].axis_2[0] &= 0xA7u;
			if (str_E2A74[index].entity_12)
			{
				if (str_E2A74[index].axis_2[0] & 2)
					helpPopup_1804B0.popupFlags_0x9f &= 0xFDu;
				else
					str_E2A74[index].entity_12 = 0;
			}
		}
		v0++;
	}
	//return result;
}
// E2A92: using guessed type void *off_E2A92;
// 180516: using guessed type int x_DWORD_180516;
// 18054E: using guessed type __int16 x_WORD_18054E;

//----- (00088B20) --------------------------------------------------------
void UpdateHelpItems_88B20()//269b20
{
	//uint8_t* result; // eax
	signed int v1; // edx
	//uint8_t* v2; // ebx
	int v2x;

	//result = x_D41A0_BYTEARRAY_4;
	if (gameState_D41A4.showHelp_10)
	{
		v1 = 1;
		//v2 = &array_E2A74[30];
		v2x = 1;
		while (v1 <= helpPopup_1804B0.helpItemCount_0x66)
		{
			if (str_E2A74[v2x].type_0 == 7)
			{
				//result = (uint8_t*)(15 * v1);
				str_E2A74[v1].stage_29 = 6;
			}
			v1++;
			//v2 += 30;
			v2x++;
		}
	}
	//return result;
}
// D41A4: using guessed type int x_DWORD_D41A4;
// E2A92: using guessed type void *off_E2A92;
// 180516: using guessed type int x_DWORD_180516;

//----- (00088B60) --------------------------------------------------------
void SetHelpItem41Active_88B60()//269b60
{
	//int result; // eax

	//result = x_D41A0_BYTEARRAY_0[4];
	if (gameState_D41A4.showHelp_10)
		//array_E2A74[0x4eb] = 6;
		str_E2A74[41].stage_29 = 6;
	//return result;
}
// D41A4: using guessed type int x_DWORD_D41A4;
// E2F5F: using guessed type char x_BYTE_E2F5F;

//----- (00088BA0) --------------------------------------------------------
void UpdateObjectiveHelp_88BA0()//269ba0
{
	char result = -1;
	if (gameState_D41A4.showHelp_10)
	{
		if (!world_D41A0.playerObjectives_0x3659C[world_D41A0.LevelIndex_0xc].substr_3659C.IsLevelEnd_0)
			result = world_D41A0.stages_0x3654C[world_D41A0.playerObjectives_0x3659C[world_D41A0.LevelIndex_0xc].substr_3659C.ObjectiveText_1].stages_3654C_byte0;
		if (result == -1)
		{
			helpPopup_1804B0.pendingHelpIndex_0x88 = 80;
		LABEL_15:
			helpPopup_1804B0.blinkPeriod_0x6e = 8;
			CloseHelpPopup_87C10();
		}
		else
		{
			switch (result)
			{
			case 0:
				helpPopup_1804B0.pendingHelpIndex_0x88 = 72;
				helpPopup_1804B0.blinkPeriod_0x6e = 8;
				CloseHelpPopup_87C10();
				break;
			case 1:
				helpPopup_1804B0.pendingHelpIndex_0x88 = 75;
				helpPopup_1804B0.blinkPeriod_0x6e = 8;
				CloseHelpPopup_87C10();
				break;
			case 2:
				helpPopup_1804B0.pendingHelpIndex_0x88 = 73;
				helpPopup_1804B0.blinkPeriod_0x6e = 8;
				CloseHelpPopup_87C10();
				break;
			case 3:
				helpPopup_1804B0.pendingHelpIndex_0x88 = 77;
				helpPopup_1804B0.blinkPeriod_0x6e = 8;
				CloseHelpPopup_87C10();
				break;
			case 5:
				helpPopup_1804B0.pendingHelpIndex_0x88 = 79;
				helpPopup_1804B0.blinkPeriod_0x6e = 8;
				CloseHelpPopup_87C10();
				break;
			case 7:
				helpPopup_1804B0.pendingHelpIndex_0x88 = 76;
				helpPopup_1804B0.blinkPeriod_0x6e = 8;
				CloseHelpPopup_87C10();
				break;
			case 8:
				helpPopup_1804B0.pendingHelpIndex_0x88 = 78;
				helpPopup_1804B0.blinkPeriod_0x6e = 8;
				CloseHelpPopup_87C10();
				break;
			case 9:
				helpPopup_1804B0.pendingHelpIndex_0x88 = 74;
				helpPopup_1804B0.blinkPeriod_0x6e = 8;
				CloseHelpPopup_87C10();
				break;
			default:
				goto LABEL_15;
			}
		}
	}
}

//----- (00088D00) --------------------------------------------------------
void SetHelpPopupIndex93_88D00()//269d00
{
	if (gameState_D41A4.showHelp_10)
		helpPopup_1804B0.pendingHelpIndex_0x88 = 93;
}

//----- (00088D40) --------------------------------------------------------
void SetSpellHelpPopupCoordinates_88D40(uint8_t scale)
{
	int maxScreenHeight;
	int spellPos;
	int posX2;
	__int16 hintText[4];
	int posY2;
	int posY1;
	int sizeFromYEnd;
	int posX1;
	int spellType;
	int sizeFromYEndPlus;
	int spellIndex;
	char typeA;
	bool isStandardSpell;

	type_entity_0x6E8E* locEvent2 = 0;
	char typeB = 0;
	int hintIndex = 0;
	int typeC = 0;
	type_entity_0x6E8E* locEvent1 = Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
	if (locEvent1->playerData_0xA4_164x->CastleEntityIndex_0x3A_58)
		locEvent2 = Entities_EA3E4[locEvent1->playerData_0xA4_164x->CastleEntityIndex_0x3A_58];
	int countHints = 0;
	for (int i = 0; i < 26; i++)
	{
		countHints += (locEvent1->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[i] != 0);
	}
	if (countHints)
	{
		int16_t spellMenuPosX;
		int16_t spellMenuBottomPosY;
		GetSpellMenuPosition(&spellMenuPosX, &spellMenuBottomPosY);
		maxScreenHeight = spellMenuBottomPosY;
		sizeFromYEnd = maxScreenHeight - 2 * ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL].height_5 * scale);
		spellIndex = spellIndex_D94FF[locEvent1->playerData_0xA4_164x->str_611.spellIndex_0x458_1112];
		isStandardSpell = locEvent1->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[spellIndex] != 0;
		if (world_D41A0.terrain_2FECE.MapType != MapType_t::Cave && spellIndex == 25)
			isStandardSpell = false;
		spellType = locEvent1->playerData_0xA4_164x->str_611.SpellLevels_0x41D_1053z.SpellIndex[spellIndex];
		spellPos = locEvent1->playerData_0xA4_164x->str_611.spellIndex_0x458_1112;
		posX1 = ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL].width_4 * scale) / 2 + ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL].width_4 * scale) * (spellPos % 13) + ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[EDGE_PANEL].width_4 * scale);
		sizeFromYEndPlus = sizeFromYEnd - ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL2].height_5 * scale);
		posY1 = ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL].height_5 * scale) / 2 + ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL].height_5 * scale) * (spellPos / 13) + sizeFromYEnd;
		posX2 = posX1 - 3 * ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL2].width_4 * scale) / 2;
		if (posX2 <= (640 * scale) - 3 * ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL2].width_4) * scale)
		{
			if (posX2 < 0)
				posX2 = 0;
		}
		else
		{
			posX2 = (640 * scale) - 3 * ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL2].width_4 * scale);
		}
		bool endLoop = 0;
		int posX3 = posX2 + 3 * ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL2].width_4 * scale) / 2;
		posX3 += spellMenuPosX;
		posX2 += spellMenuPosX;
		posX1 += spellMenuPosX;
		for (int i = 0; i < 8 && !endLoop; i++)
		{
			typeA = 2;
			typeC = 1;
			posY2 = ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL2].height_5 * scale) / 2 + sizeFromYEndPlus;
			switch (helpPopup_1804B0.spellHelpStep_0xa4)
			{
			case 0: //Selected Spell Name
				if (isStandardSpell)
				{
					hintIndex = 85;
					typeA = 3;
					hintText[0] = SPELLS_BEGIN_BUFFER_str[spellIndex].subspell[0].hintText_0x16x;
					hintText[1] = SPELLS_BEGIN_BUFFER_str[spellIndex].subspell[1].hintText_0x16x;
					hintText[2] = SPELLS_BEGIN_BUFFER_str[spellIndex].subspell[2].hintText_0x16x;
					str_E2A74[85].axis_2[3] = posX3;
					str_E2A74[85].axis_2[4] = sizeFromYEndPlus;
					endLoop = true;
					typeC = 3;
					typeB = locEvent1->playerData_0xA4_164x->str_611.subSpellIndex_0x459_1113;
				}
				break;
			case 1: //Spell Experience Points
				if (isStandardSpell)
				{
					hintIndex = SPELL_EXPERIENCE_POINTS;
					hintText[0] = str_E2A74[SPELL_EXPERIENCE_POINTS].axis_2[1];
					endLoop = true;
					str_E2A74[SPELL_EXPERIENCE_POINTS].axis_2[3] = posX2 + (33 * scale);
					if (spellType > 0)
						str_E2A74[SPELL_EXPERIENCE_POINTS].axis_2[3] = posX2 + (33 * scale) + ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL2].width_4 * scale);
					str_E2A74[SPELL_EXPERIENCE_POINTS].axis_2[4] = sizeFromYEndPlus + (29 * scale);
				}
				break;
			case 2: //Can cast spell?
				if (isStandardSpell)
				{
					for (int j = 0; j < 3 && !endLoop; j++)
					{
						if (SPELLS_BEGIN_BUFFER_str[spellIndex].subspell[j].maxManaLimit_A && (!locEvent2 || locEvent2->mana_0x90_144 < SPELLS_BEGIN_BUFFER_str[spellIndex].subspell[j].maxManaLimit_A))
						{
							hintIndex = NOT_ENOUGH_CASTLED_MANA_TO_USE;
							hintText[0] = str_E2A74[NOT_ENOUGH_CASTLED_MANA_TO_USE].axis_2[1];
							str_E2A74[NOT_ENOUGH_CASTLED_MANA_TO_USE].axis_2[3] = posX2 + j * ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL2].width_4 * scale) + ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL2].width_4 * scale) / 2;
							endLoop = true;
							str_E2A74[NOT_ENOUGH_CASTLED_MANA_TO_USE].axis_2[4] = sizeFromYEndPlus + ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL2].height_5 * scale) / 2;
						}
					}
				}
				break;
			case 3:
				if (isStandardSpell && spellType)
				{
					hintIndex = ACHIEVED_SPELL_LEVEL;
					hintText[0] = str_E2A74[ACHIEVED_SPELL_LEVEL].axis_2[1];
					str_E2A74[ACHIEVED_SPELL_LEVEL].axis_2[3] = (((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL2].width_4 * scale) / 2) + posX2 + spellType * ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[163].width_4 * scale);
					endLoop = true;
					str_E2A74[ACHIEVED_SPELL_LEVEL].axis_2[4] = posY2;
				}
				break;
			case 4:
				if (isStandardSpell)
				{
					if (!SPELLS_BEGIN_BUFFER_str[spellIndex].subspell[locEvent1->playerData_0xA4_164x->str_611.subSpellIndex_0x459_1113].maxManaLimit_A || locEvent2 && locEvent2->mana_0x90_144 >= SPELLS_BEGIN_BUFFER_str[spellIndex].subspell[locEvent1->playerData_0xA4_164x->str_611.subSpellIndex_0x459_1113].maxManaLimit_A)
					{
						hintIndex = SPELL_RECHARGE_AND_SHOTS;
						hintText[0] = str_E2A74[SPELL_RECHARGE_AND_SHOTS].axis_2[1];
						str_E2A74[SPELL_RECHARGE_AND_SHOTS].axis_2[3] = posX1;
						str_E2A74[SPELL_RECHARGE_AND_SHOTS].axis_2[4] = posY1 + (((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL].height_5 * scale) - (my_sign32(((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL].height_5 * scale)) * 3)) >> 2;
					}
				}
				break;
			case 5:
				if (isStandardSpell)
				{
					hintIndex = SELECTED_SPELL_LEVEL;
					hintText[0] = str_E2A74[SELECTED_SPELL_LEVEL].axis_2[1];
					str_E2A74[SELECTED_SPELL_LEVEL].axis_2[3] = (((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON2_PANEL2].width_4 * scale) / 2) + posX2 + locEvent1->playerData_0xA4_164x->str_611.subSpellIndex_0x459_1113 * ((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[163].width_4 * scale);
					endLoop = true;
					str_E2A74[SELECTED_SPELL_LEVEL].axis_2[4] = posY2;
				}
				break;
			case 6:
				if (isStandardSpell)
				{
					if (Entities_EA3E4[locEvent1->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[spellIndex]] > Entities_EA3E4[0])
					{
						if (spellIndex == 2)
						{
							if (locEvent1->mana_0x90_144 < Entities_EA3E4[locEvent1->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[spellIndex]]->maxMana_0x8C_140)
								endLoop = true;
						}
						else
						{
							endLoop = !CanSpellOwnerCast_68D50(Entities_EA3E4[locEvent1->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[spellIndex]], locEvent1);
						}
						if (endLoop)
						{
							hintIndex = NOT_ENOUGH_MANA_TO_CAST;
							hintText[0] = str_E2A74[NOT_ENOUGH_MANA_TO_CAST].axis_2[1];
							str_E2A74[NOT_ENOUGH_MANA_TO_CAST].axis_2[3] = posX1;
							str_E2A74[NOT_ENOUGH_MANA_TO_CAST].axis_2[4] = posY1 + (((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL].height_5 * scale) - (my_sign32(((*filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct)[SPELL_ICON_PANEL].height_5 * scale)) * 3)) >> 2;
						}
					}
				}
				break;
			default:
				break;
			}
			if ((unsigned __int8)++helpPopup_1804B0.spellHelpStep_0xa4 > 7u)
				helpPopup_1804B0.spellHelpStep_0xa4 = 0;
		}
		if (hintIndex)
			SetHelpPopupTextAndCoords_884D0(hintIndex, typeC, hintText, typeA, typeB);
	}
}

//----- (00089360) --------------------------------------------------------
void SetPlayerScoresHelpPopupTextAndCoords_89360(uint8_t scale)//26a360
{
	char v0 = 0;
	int16_t helpIdx = 0;
	if (helpPopup_1804B0.showPlayerScores_0xa7)
	{
		helpPopup_1804B0.showPlayerScores_0xa7 = 0;
		if (helpPopup_1804B0.wizardNumber_0xa8 && world_D41A0.NumberOfPlayers_0xe > 1u)
		{
			// Number of times you have killed %s
			str_E2A74[96].axis_2[3] = helpPopup_1804B0.scoresBarX_0x9a;
			helpIdx = 96;
			str_E2A74[96].axis_2[4] = helpPopup_1804B0.scoresBarY_0x9c;
		}
		else
		{
			v0 = 1;
		}
	}
	else
	{
		v0 = 1;
		helpPopup_1804B0.showPlayerScores_0xa7 = 1;
	}
	if (v0)
	{
		// Current spells mana value
		helpIdx = 95;
		str_E2A74[95].axis_2[3] = helpPopup_1804B0.scoresPosX_0x96;
		helpPopup_1804B0.wizardNumber_0xa8 = 0;
		str_E2A74[95].axis_2[4] = helpPopup_1804B0.scoresPosY_0x98;
	}
	if (helpIdx)
	{
		Logger->debug("helpIdx: {}", helpIdx);
		SetHelpPopupTextAndCoords_884D0(helpIdx, 1, &str_E2A74[helpIdx].axis_2[1], 2, 0);
	}
}
