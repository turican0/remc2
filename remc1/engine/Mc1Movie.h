#pragma once

#include <cstdint>

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

void WaitMovieFrame_10300_10300();
void ReadMovieFrame_103F0_103F0();
void DrawMovieFrame_104D0_104D0();
void PlayMovie_107C0_107C0(int16_t allowSkip, int16_t redrawText, const Type_Mc1MovieEvent* events);
void MovieEvents_111B0_111B0(const Type_Mc1MovieEvent* events);
void InitInputChange_356E0_35AA0();
int32_t InputChanged_35710_35AD0();
int32_t CheckQuitKey_4B430_4B770();
void WaitSeconds_4B480_4B7C0(void (*callback)(), int32_t seconds);
void PlayIntelIntro_4EFC0_4F300();
