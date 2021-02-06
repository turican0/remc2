#pragma once
#ifndef PORT_SDL_SOUND_H
#define PORT_SDL_SOUND_H

//#define SOUND_OPENAL

#include "mctypes.h"

#define SOUND_SDLMIXER

#ifdef _MSC_VER
	#include "SDL.h"
#ifdef SOUND_SDLMIXER
	#include "SDL_mixer_ext.h"
	#include "music.h"
	#include "mixer.h"
#endif
#else
    #include "SDL2/SDL.h"
#ifdef SOUND_SDLMIXER
	#include "SDL2/SDL_mixer.h"
#endif
#endif

#ifdef SOUND_OPENAL
	#include <al.h>
	#include <alc.h>
#endif//SOUND_OPENAL

//#include "music_timidity.h"
#include "xmi2mid.h"
#include <time.h>       /* time */
#include <string>

#include "../engine/ail_sound.h"
#include "port_filesystem.h"

#ifndef SOUND_SDLMIXER
typedef struct {
	int allocated;
	Uint8* abuf;
	Uint32 alen;
	Uint8 volume;       /* Per-sample volume, 0-128 */
} Mix_Chunk;
#endif//SOUND_SDLMIXER

#define USE_SDL2

#define DEBUG_SOUND
extern bool debug_first_sound;

extern bool hqsound;
extern bool oggmusic;
extern char oggmusicFolder[512];
extern bool oggmusicalternative;
extern bool fixspeedsound;

bool init_sound();
//bool load_sound_files();
void clean_up_sound();
/*int playsound1();
int playsound2();
int playsound3();
int playsound4();
void playmusic1();
void stopmusic1();
*/
void playmusic2(Bit32s track_number);
extern uint8_t sound_buffer[4][20000];
//extern HSAMPLE last_sample;

Bit32s ac_sound_call_driver(AIL_DRIVER* drvr, Bit32s fn, VDI_CALL* in, VDI_CALL* out);

void ac_set_real_vect(Bit32u vectnum, Bit16u real_ptr);
Bit16u ac_get_real_vect(Bit32u vectnum);
AIL_DRIVER* ac_AIL_API_install_driver(int a1, uint8_t* a2, int a3);

void SOUND_init_MIDI_sequence(uint8_t* data, uint8_t* header, Bit32s track_number);

void SOUND_start_sample(HSAMPLE S);
void SOUND_end_sample(HSAMPLE S);

void SOUND_start_sequence(Bit32s sequence_num);
void SOUND_pause_sequence(Bit32s sequence_num);
void SOUND_stop_sequence(Bit32s sequence_num);
void SOUND_resume_sequence(Bit32s sequence_num);
Bit32u SOUND_sample_status(HSAMPLE S);

void SOUND_set_sample_volume(HSAMPLE S, Bit32s volume);
void SOUND_set_sequence_volume(Bit32s volume);
void SOUND_set_master_volume(Bit32s volume);
void SOUND_UPDATE();
//void test_midi_play(uint8_t* data, uint8_t* header, Bit32s track_number);
#ifdef SOUND_OPENAL
//void ALSOUND_load_wav(char* alBuffer, long alBufferLen);
void ALSOUND_play(int which, Mix_Chunk* chunk, int loops);
void ALSOUND_init();
#endif//SOUND_OPENAL




#endif //PORT_SDL_SOUND