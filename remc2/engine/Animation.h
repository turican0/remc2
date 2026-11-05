#pragma once

#ifndef MAIN_ANIMATION
#define MAIN_ANIMATION

#include "ail_sound.h"

#include "Basic.h"
#include "Sound.h"

#pragma pack(push, 1)
typedef struct {//lenght 7
	int16_t startFrame;//0
	char key_2;//2
	int32_t index;//3
}
Type_SoundEvent_E17CC;

typedef struct {//lenght 12
	uint16_t magic_0;
	uint16_t frames_2;
	uint16_t width_4;
	int16_t frameCount_6;
	int16_t height_8;
	int16_t width_alt_10;
}
Type_17DB40;
#pragma pack(pop)

extern Type_17DB40 fmvHeader_17DB40;//uint8_t unk_17DB40[12]; // weak

extern int skipInput_EA3B4; // weak?langStrings_E9C4C[474]
extern __int16 lastKey_F42AE; // weak
extern __int16 skipDelay_F42A8; // weak
extern TColor* framebuffer_E12F4x; // weak
extern int16_t height_17DB48; // weak
extern char DisplaySubtitles_D41C0; // weak

extern __int16 redrawTextInVideo_E12FC;
extern __int16 soundEventIndex_D4004;
//extern __int16 x_WORD_17DB58;
extern uint16_t ActualKeyframe_17DB60;
extern FILE* fmvFile_17DB38;
//extern uint8_t unk_17DB40[];
extern int16_t LastKeyframe_17DB46;
extern int16_t width_17DB4A;
extern int fileOffset_E1300;
extern __int16 stopPlaybackFlag_17DB5A;
extern __int16 allowSkipVideo_17DB5C;

extern __int16 redrawTextInVideo_E12FC;
extern __int16 soundEventIndex_D4004;
//extern __int16 x_WORD_17DB58;
extern uint16_t ActualKeyframe_17DB60;
extern FILE* fmvFile_17DB38;
//extern uint8_t unk_17DB40[];
extern int16_t LastKeyframe_17DB46;
extern int16_t width_17DB4A;
extern int fileOffset_E1300;
extern __int16 stopPlaybackFlag_17DB5A;
extern __int16 allowSkipVideo_17DB5C;

void PlayInfoFmv(__int16 allowSkip, __int16 redrawText, Type_SoundEvent_E17CC* pSoundEvent, char* path);//257160
void FlvInitSet_473B0();
void PlayIntoSoundEvents_1B280(Type_SoundEvent_E17CC* pSoundEvent);
void ReadFrame_75DB0();
void DrawFrame_75E70();

void ClearSubtitles_2EC60();
int HasSkipInput_473E0();

void ReadIntroPalettes_76260();
void DecodeFlcDeltaChunk_76300();
int DecodeFliLineCompressChunk_76430();
int DecodeFliByteRunChunk_76540();
void WaitForNextAnimationFrame_75CB0();
void WaitToScreenBeam_9A0FC();



#endif //MAIN_ANIMATION