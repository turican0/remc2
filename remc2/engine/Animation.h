#pragma once

#ifndef MAIN_ANIMATION
#define MAIN_ANIMATION

#include "ail_sound.h"

#include "Basic.h"
#include "Sound.h"

#pragma pack (1)
typedef struct {//lenght 7
	int16_t startFrame;
	int8_t byte_2;
	int32_t index;
}
type_E17CC_0;
#pragma pack (16)

extern int x_DWORD_17DB54_game_turn2; // weak
extern int x_DWORD_EA3B4; // weak?x_DWORD_E9C4C_langindexbuffer[474]
extern __int16 x_WORD_F42AE; // weak
extern __int16 x_WORD_F42A8; // weak
extern TColor* x_DWORD_E12F4x; // weak
extern int16_t x_WORD_17DB48; // weak
extern char x_BYTE_D41C0; // weak

void PlayInfoFmv(__int16 a1, __int16 a2, type_E17CC_0* a3x, char* path);//257160
void FlvInitSet_473B0();
void sub_1B280_run_intro_sound_events(type_E17CC_0* a1x);
void sub_75DB0();
void sub_75E70();

void sub_loc_1B2E6();
void sub_loc_1B2F7(type_E17CC_0* a2x);
void sub_loc_1B316(uint8_t a1);
void sub_loc_1B334(type_E17CC_0* a2x);
void sub_loc_1B352(type_E17CC_0* a2x);
void sub_loc_1B36C();
void sub_loc_1B37D(type_E17CC_0* a2x);
void sub_loc_1B398(type_E17CC_0* a2x);
void sub_loc_1B3C9(type_E17CC_0* a1x);
void sub_loc_1B413();
void sub_loc_1B424(uint16_t a1, type_E17CC_0* a2x);
void sub_loc_1B450();
void sub_loc_1B461(uint16_t a1, type_E17CC_0* a2x);
void sub_loc_1B4A0(uint16_t a1, type_E17CC_0* a2x);
void sub_loc_1B4DF(uint16_t a1, type_E17CC_0* a2x);
void sub_loc_1B51E(type_E17CC_0* a2x);
void sub_loc_1B54A(uint16_t a1, type_E17CC_0* a2x);
void sub_loc_1B589();
void sub_loc_1B5A7();
void sub_loc_1B5BC(uint16_t a1);
void sub_loc_1B5CB();
//void sub_loc_1B5D7();
void sub_2EC60();
int sub_473E0();

void sub_76300();
int sub_76430();
int sub_76540();
void sub_75CB0();
void sub_9A0FC_wait_to_screen_beam();



#endif //MAIN_ANIMATION