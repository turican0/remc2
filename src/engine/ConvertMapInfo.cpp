#include "ConvertMapInfo.h"
void DecompressLevel_2FECE(Type_CompressedLevel_2FECE* from, Type_Level_2FECE* to) {
	to->word_2FECE = from->word_2FECE;
	to->levelID_2FED0 = from->levelID_2FED0;
	to->byte_0x2FED2 = from->byte_0x2FED2;
	to->byte_0x2FED3 = from->byte_0x2FED3;
	to->MapType = (from->MapType == 2) ? MapType_t::Cave : (from->MapType == 1) ? MapType_t::Night : MapType_t::Day;
	to->word_0x2FED5 = from->word_0x2FED5;
	to->word_0x2FED7 = from->word_0x2FED7;
	for (int i = 0; i < 8; i++)to->player_0x2FED9[i] = from->array_0x2FED9[i];
	for (int i = 0; i < 4; i++)to->stubb[i] = from->stubb[i];
	to->seed_0x2FEE5 = from->word_0x2FEE5;
	for (int i = 0; i < 2; i++)to->stubb2[i] = from->stubb2[i];
	to->offset_0x2FEE9 = from->word_0x2FEE9;
	for (int i = 0; i < 2; i++)to->stubc[i] = from->stubc[i];
	to->raise_0x2FEED = from->word_0x2FEED;
	for (int i = 0; i < 2; i++)to->stubc2[i] = from->stubc2[i];
	to->gnarl_0x2FEF1 = from->word_0x2FEF1;
	for (int i = 0; i < 2; i++)to->stubc3[i] = from->stubc3[i];
	to->river_0x2FEF5 = from->word_0x2FEF5;
	to->lriver_0x2FEF9 = from->word_0x2FEF9;
	for (int i = 0; i < 2; i++)to->stube[i] = from->stube[i];
	to->source_0x2FEFD = from->word_0x2FEFD;
	for (int i = 0; i < 2; i++)to->stubf[i] = from->stubf[i];
	to->snLin_0x2FF01 = from->word_0x2FF01;
	for (int i = 0; i < 2; i++)to->stubg[i] = from->stubg[i];
	to->snFlt_0x2FF05 = from->word_0x2FF05;
	for (int i = 0; i < 2; i++)to->stubh[i] = from->stubh[i];
	to->bhLin_0x2FF09 = from->word_0x2FF09;
	for (int i = 0; i < 2; i++)to->stubi[i] = from->stubi[i];
	to->bhFlt_0x2FF0D = from->word_0x2FF0D;
	for (int i = 0; i < 2; i++)to->stubj[i] = from->stubj[i];
	to->rkSte_0x2FF11 = from->word_0x2FF11;
	for (int i = 0; i < 1022; i++)to->stubk[i] = from->stubk[i];
	for (int i = 0; i < 1200; i++)to->entity_0x30311[i] = from->array_0x30311[i];
	to->next_0x360D1 = from->next_0x360D1;
	for (int i = 0; i < 8; i++)to->WizardMapSettings_0x360D2[i] = from->WizardMapSettings_0x360D2[i];
	for (int i = 0; i < 8; i++)to->stages_0x36442[i] = from->str_0x36442[i];
	for (int i = 0; i < 11; i++)
	{
		to->StageVars_0x3647A[i].index_0x3647A_0 = from->array_0x3647A[i].str_0x3647A_byte_0;
		to->StageVars_0x3647A[i].stage_0x3647A_1 = from->array_0x3647A[i].str_0x3647A_byte_1;
		to->StageVars_0x3647A[i].str_0x3647A_2 = from->array_0x3647A[i].str_0x3647A_2;
		to->StageVars_0x3647A[i].str_0x3647C_4.axis = from->array_0x3647A[i].str_0x3647C_4.axis;
	}
}

