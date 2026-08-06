#include "LevelInit.h"
//----- (00053590) --------------------------------------------------------
void SetLevelId_53590(Type_Level_2FECE* levelData)//234590
{
	D41A0_0.dword_0x4 = levelData->levelID_2FED0;
}

//----- (00056C00) --------------------------------------------------------
void LevelInit_56C00(Type_Level_2FECE* levelData)//237c00
{
	isCaveLevel_D41B6 = 0;
	SPELLS_BEGIN_BUFFER_str[4].subspell[0].life_0x1A = 19;
	SPELLS_BEGIN_BUFFER_str[4].subspell[0].hintText_0x16x = 199;
	SPELLS_BEGIN_BUFFER_str[19].subspell[0].life_0x1A = 19;
	SPELLS_BEGIN_BUFFER_str[19].subspell[0].hintText_0x16x = 245;
	if (levelData->MapType == MapType_t::Day)
	{
		SPELLS_BEGIN_BUFFER_str[4].subspell[0].life_0x1A = 2;
		SPELLS_BEGIN_BUFFER_str[4].subspell[0].hintText_0x16x = 198;
		SPELLS_BEGIN_BUFFER_str[19].subspell[0].life_0x1A = 2;
		SPELLS_BEGIN_BUFFER_str[19].subspell[0].hintText_0x16x = 244;
		D41A0_0.m_GameSettings.str_0x2196.transparency_0x2198 = 0;
		LoadSounds_84300(0);
		CURSOR_SPRITE_INDEX_D419E = 1;
	}
	else if (levelData->MapType == MapType_t::Night)
	{
		D41A0_0.m_GameSettings.str_0x2196.transparency_0x2198 = 0;
		LoadSounds_84300(1u);
		CURSOR_SPRITE_INDEX_D419E = 9;
	}
	else if (levelData->MapType == MapType_t::Cave)
	{
		D41A0_0.m_GameSettings.str_0x2196.transparency_0x2198 = 1;
		isCaveLevel_D41B6 = 1;
		MapBasicHeight_D41B7 = levelData->byte_0x2FED3;
		LoadSounds_84300(2u);
		CURSOR_SPRITE_INDEX_D419E = 10;
	}
	SetDefaultSpells_5C0A0();
	D41A0_0.byte_0x36E02 = 1;
	D41A0_0.word_0x36DFE = 0;
	D41A0_0.word_0x36DFC = 0;
	D41A0_0.str_0x21AE.xxxx_0x21B1 = 0;
	D41A0_0.m_GameSettings.m_Display.xxxx_0x2191 = 0;
	D41A0_0.byte_0x36E0B &= 0xFC;
	x_D41A0_BYTEARRAY_4_struct.SelectedMenuItem_38546 = 0;
	if (x_WORD_180660_VGA_type_resolution == 1)
		FontType_D419D = 1;
	else
		FontType_D419D = 3;
}