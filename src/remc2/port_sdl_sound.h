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

#include "ail_sound.h"
#include "port_filesystem.h"

#pragma pack (1)
typedef struct {//lenght 32 - this is may be format of wav sound file
	uint8_t* wavData_0;//data of wav//18
	int8_t stub_4[4];//22
	int32_t wavSize_8;//24
	int16_t word_12;//28
	int8_t filename_14[18];//first 18 chars - name//30
}
sub2type_E37A0_sound_buffer2;

typedef struct {//lenght 2072
	int8_t stub[10];
	sub2type_E37A0_sound_buffer2 wavs_10[96];
}
sub1type_E37A0_sound_buffer2;

typedef struct {//lenght 3100
	int8_t byte_0;
	int8_t byte_1;
	int8_t byte_2;
	int8_t byte_3;
	int8_t byte_4;
	int8_t byte_5;
	int8_t byte_6;
	int8_t byte_7;
	sub1type_E37A0_sound_buffer2 str_8;//2072 lenght
	int8_t next_str[10];
}
type_E37A0_sound_buffer2;

//shadow type_E37A0_sound_buffer2
typedef struct {//lenght 32 - this is may be format of wav sound file
	int32_t wavData_0;//data of wav//18
	int8_t stub_4[4];//22
	int32_t wavSize_8;//24
	int16_t word_12;//28
	int8_t filename_14[18];//first 18 chars - name//30
}
shadow_sub2type_E37A0_sound_buffer2;

typedef struct {//lenght 2072
	int8_t stub[10];
	shadow_sub2type_E37A0_sound_buffer2 wavs_10[96];
}
shadow_sub1type_E37A0_sound_buffer2;

typedef struct {//lenght 3100
	int8_t byte_0;
	int8_t byte_1;
	int8_t byte_2;
	int8_t byte_3;
	int8_t byte_4;
	int8_t byte_5;
	int8_t byte_6;
	int8_t byte_7;
	shadow_sub1type_E37A0_sound_buffer2 str_8;//2072 lenght
	int8_t next_str[10];
}
shadow_type_E37A0_sound_buffer2;
//shadow type_E37A0_sound_buffer2

typedef struct {//lenght 16*6=96
	int32_t dword_0;
	int32_t dword_4;
	int32_t sizeBytes_8;
	int32_t dword_12;
}
type_v8;



typedef struct {//lenght 32 - this is may be format of wav sound file
	uint8_t* xmiData_0;//data of wav//18
	int8_t stub_4[4];//22
	int32_t xmiSize_8;//24
	int16_t word_12;//28
	int8_t filename_14[18];//first 18 chars - name//30
}
sub2type_E3808_music_header;

typedef struct {//lenght 216
	int8_t stub[10];
	sub2type_E3808_music_header track_10[6];
	int8_t stubb[14];
}
sub1type_E3808_music_header;

typedef struct {//lenght 224
	int8_t byte_0;
	int8_t byte_1;
	int8_t byte_2;
	int8_t byte_3;
	int8_t byte_4;
	int8_t byte_5;
	int8_t byte_6;
	int8_t byte_7;
	sub1type_E3808_music_header str_8;//216 lenght
}
type_E3808_music_header;
/*
typedef struct {//lenght 80656
	int8_t byte_0;
	int8_t byte_1;
	int8_t byte_2;
	int8_t byte_3;
	int8_t byte_4;
	int8_t byte_5;
	int8_t byte_6;
	int8_t byte_7;
	int8_t data_8[];
}
type_E3810_music_data;*/

//shadow shadow_type_E3808_music_header
typedef struct {//lenght 32 - this is may be format of wav sound file
	int32_t xmiData_0;//data of wav//18
	int8_t stub_4[4];//22
	int32_t xmiSize_8;//24
	int16_t word_12;//28
	int8_t filename_14[18];//first 18 chars - name//30
}
shadow_sub2type_E3808_music_header;

typedef struct {//lenght 216
	int8_t stub[10];
	shadow_sub2type_E3808_music_header track_10[6];
	int8_t stubb[14];
}
shadow_sub1type_E3808_music_header;

typedef struct {//lenght 224
	int8_t byte_0;
	int8_t byte_1;
	int8_t byte_2;
	int8_t byte_3;
	int8_t byte_4;
	int8_t byte_5;
	int8_t byte_6;
	int8_t byte_7;
	shadow_sub1type_E3808_music_header str_8;//216 lenght
}
shadow_type_E3808_music_header;
//shadow shadow_type_E3808_music_header
#pragma pack (16)

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
void playmusic2(int32_t track_number);
extern uint8_t sound_buffer[4][20000];
//extern HSAMPLE last_sample;

int32_t ac_sound_call_driver(AIL_DRIVER* drvr, int32_t fn, VDI_CALL* in, VDI_CALL* out);

void ac_set_real_vect(uint32_t vectnum, uint16_t real_ptr);
uint16_t ac_get_real_vect(uint32_t vectnum);
AIL_DRIVER* ac_AIL_API_install_driver(int a1, uint8_t* a2, int a3);

void SOUND_init_MIDI_sequence(uint8_t* datax, type_E3808_music_header* headerx, int32_t track_number);

void SOUND_start_sample(HSAMPLE S);
void SOUND_end_sample(HSAMPLE S);

void SOUND_start_sequence(int32_t sequence_num);
void SOUND_pause_sequence(int32_t sequence_num);
void SOUND_stop_sequence(int32_t sequence_num);
void SOUND_resume_sequence(int32_t sequence_num);
uint32_t SOUND_sample_status(HSAMPLE S);

void SOUND_set_sample_volume(HSAMPLE S, int32_t volume);
void SOUND_set_sequence_volume(int32_t volume);
void SOUND_set_master_volume(int32_t volume);
void SOUND_UPDATE();
//void test_midi_play(uint8_t* data, uint8_t* header, int32_t track_number);
#ifdef SOUND_OPENAL
//void ALSOUND_load_wav(char* alBuffer, long alBufferLen);
void ALSOUND_play(int which, Mix_Chunk* chunk, int loops);
void ALSOUND_init();
#endif//SOUND_OPENAL




#endif //PORT_SDL_SOUND
