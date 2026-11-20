#pragma once

#include <cstdint>

#include "Mc1Files.h"
#include "Mc1MenuText.h"

// MC1 FLI player (intro\*.dat) with its frame events; the chunk decoders and the player state are shared with
// remc2 (Mc1Shared.h), the frame reading, the timing and the events are MC1's own

#pragma pack(push, 1)
typedef struct { // 7 bytes
	uint16_t frame_0;
	char command_2; // A period, B music bank, E sound bank, L/M/Z music, O/K/P/Q subtitles, R/S/T sound, X stop music
	int16_t value_3;
	uint16_t unused_5;
} Type_Mc1MovieEvent;

typedef struct { // 16 bytes
	uint32_t size_0;
	uint16_t type_4; // F1FAh frame, F100h prefix
	uint16_t chunks_6;
	uint8_t unused_8[8];
} Type_Mc1FrameHeader;
#pragma pack(pop)

extern Type_Mc1FrameHeader frameHeader_AC1A0_AC190;
extern int32_t frameOffset_AC1B0_AC1A0;
extern char frameInfo_AC1B8_AC1A8[256];
extern void (*frameCallback_AC5C4_AC5B4)();
extern int16_t musicLoop_AC5DA_AC5CA; // music + 1 to restart when it ends
extern int16_t skipOnInput_90026_90026;
extern int32_t framePeriod_9ADC4_9ADC4; // timer ticks per frame
extern char moviePath_9ADC8_9ADC8[80];
extern int16_t savedMouseX_B7314_B7304;
extern int16_t savedMouseY_B7316_B7306;
extern int16_t savedKey_B7318_B7308;
extern int32_t inputChanged_AE410_AE400;
extern const Type_Mc1MovieEvent intelEvents_4A1E4_4A524[4];
extern const Type_Mc1MovieEvent logoEvents_4A620_4A960[4];
extern const Type_Mc1MovieEvent titleEvents_4A568_4A8A8[16];
extern uint8_t* titleAnimation_12CB64_12CB54; // intro\title-02.dat / title-04.dat
extern uint8_t* titleLoop_12CB68_12CB58;
extern uint8_t* titleFrame_12CB6C_12CB5C;
extern int16_t titleFrameIndex_12CB70_12CB60;
extern Type_Mc1FileEntry titleFiles_4A0D4_4A414[2];
extern const Type_Mc1MovieEvent introEvents_4A1FC_4A53C[126];
extern uint8_t subtitlesLoaded_9009E_9009E;
extern const char* subtitleText_900A0_900A0; // the text shown, drawn again on a palette change
extern Type_Mc1Font_96884 subtitleFont_AE460_AE450; // data\screens\sfont1
extern uint8_t* subtitleFontTab_B3288_B3278;
extern uint8_t* subtitleFontTabEnd_B328C_B327C;
extern uint8_t* subtitleFontData_B3290_B3280;
extern uint8_t* subtitleArea_B3294_B3284; // the rows of the text under the movie in the work screen
extern uint16_t subtitleAreaSize_B329C_B328C;
extern Type_Mc1FileEntry subtitleFiles_20EF0_20EF0[3];

void WaitMovieFrame_10300_10300();
void ReadMovieFrame_103F0_103F0();
void DrawMovieFrame_104D0_104D0();
void PlayMovie_107C0_107C0(int16_t allowSkip, int16_t redrawText, const Type_Mc1MovieEvent* events);
void MovieEvents_111B0_111B0(const Type_Mc1MovieEvent* events);
void InitInputChange_356E0_35AA0();
int32_t InputChanged_35710_35AD0();
int32_t CheckQuitKey_4B430_4B770();
void WaitSeconds_4B480_4B7C0(void (*callback)(), int32_t seconds);
void PlayMainIntro_4D3B0_4D6F0();
void PlayLogoIntro_4EE70_4F1B0();
void AnimateTitle_4F120_4F460();
void TitleScreen_4F1E0_4F520();
void PlayIntelIntro_4EFC0_4F300();
void InitSubtitles_24AB0_24AB0();
void ShowSubtitle_24B50_24B50(const char* text);
void ClearSubtitles_24B80_24B80();
void FreeSubtitles_24BB0_24BB0();
void SetSubtitleColor_24BF0_24BF0(uint8_t color);
