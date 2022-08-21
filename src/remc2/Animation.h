#pragma once

#ifndef MAIN_ANIMATION
#define MAIN_ANIMATION

#include "ail_sound.h"

#include "Basic.h"
#include "Sound.h"

#pragma pack (1)
typedef struct {//lenght 7
	int16_t startFrame;//0
	char key_2;//2
	int32_t index;//3
}
Type_SoundEvent_E17CC;
#pragma pack (16)

extern int x_DWORD_17DB54_game_turn2; // weak
extern int x_DWORD_EA3B4; // weak?x_DWORD_E9C4C_langindexbuffer[474]
extern __int16 x_WORD_F42AE; // weak
extern __int16 x_WORD_F42A8; // weak
extern TColor* x_DWORD_E12F4x; // weak
extern int16_t x_WORD_17DB48; // weak
extern char x_BYTE_D41C0; // weak

void PlayInfoFmv(__int16 a1, __int16 a2, Type_SoundEvent_E17CC* pSoundEvent, char* path);//257160
void FlvInitSet_473B0();
void PlayIntoSoundEvents_1B280(Type_SoundEvent_E17CC* pSoundEvent);
void sub_75DB0();
void sub_75E70();

void sub_2EC60();
int sub_473E0();

void sub_76300();
int sub_76430();
int sub_76540();
void sub_75CB0();
void sub_9A0FC_wait_to_screen_beam();



#endif //MAIN_ANIMATION