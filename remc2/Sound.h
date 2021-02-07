#ifndef MAIN_SOUND
#define MAIN_SOUND

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cstdint>
#include <functional>

#include "stdint.h"

#ifdef _MSC_VER
#include <direct.h>  
#include <io.h>  
#include <windows.h>
#include "portability/dirent-x.h"
#else
#include "dirent.h"
#endif

#include "engine/ail_sound.h"

void /*__fastcall*/ sub_8D290_init_sound(/*char* a1*//*, int a2, int a3*/);
void sub_8D800_sound_proc2();
void sub_8D8F0_sound_proc3_endsample();
void /*__fastcall*/ sub_8D970_init_music(/*char* a1*//*int a1, int a2, char* a3*/);
void sub_8E020_sound_proc14_stopsequence();
int sub_8E0D0(); // weak
void sub_8E160_sound_proc15_startsequence(__int16 track, unsigned __int8 volume);
// x_DWORD sprintf(x_DWORD, const char *, ...); weak
void sub_8E410_sound_proc16_xmidivolume(int32_t master_volume);
int sub_8E470_sound_proc17_volume(int a1);
__int16 sub_8E673();
__int16 sub_8E736();
int sub_8E799();
int sub_8E7B7();
int sub_8E7D5();
void sub_8E7FC();
__int16 sub_8E871();
int sub_8E948();
char sub_8E9EF();
void /*__fastcall*/ sub_8EA7B(int a1, unsigned __int16 a2);
signed int LoadMusic(int channel);
void GetMusicSequenceCount();
char LoadMusicTrack(FILE* file, unsigned __int8 drivernumber);
int sub_8F023(int a1, int a2, unsigned int a3);
int sub_8F0AB(FILE* a1, /*int a2,*/ int a3);
HSAMPLE* sub_8F100_sound_proc19(uint32_t a1, __int16 a2, int a3, int a4, unsigned __int16 a5, char a6, unsigned __int8 a7);
void sub_8F420_sound_proc20(int a1, __int16 a2);
int sub_8F4B0(); // weak
void sub_8F710_sound_proc21(int a1, __int16 a2, signed int a3, unsigned __int8 a4, char a5);
void sub_8F850_sound_proc22();

#endif //MAIN_SOUND