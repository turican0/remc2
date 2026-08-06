#pragma once
#ifndef PORT_SDL_SOUND_H
#define PORT_SDL_SOUND_H

//#define SOUND_OPENAL

#include "bitmap_pos_struct.h"
#include "SfxEffectWrapper.h"
#include <SDL2/SDL.h>

#define SOUND_SDLMIXER

#ifdef SOUND_SDLMIXER
	#include <SDL2/SDL_mixer.h>
#endif

#ifdef SOUND_OPENAL
	#include <al.h>
	#include <alc.h>
#endif//SOUND_OPENAL

#include "xmi2mid.h"
#include <time.h>       /* time */
#include <string>

#include "engine/ail_sound.h"
#include "engine/EventDispatcher.h"
#include "engine/GameState.h"
#include "port_filesystem.h"

#pragma pack(push, 1)
typedef struct {//lenght 32 - this is may be format of wav sound file
	int8_t filename_0[18];//first 18 chars - name//30
	uint8_t* wavData_18;//data of wav//18
	int8_t stub_22[4];//22
	int32_t wavSize_26;//24
	int16_t word_30;//28	
}
sub2type_E37A0_sound_buffer2;

typedef struct {//lenght 2072
	int8_t stub[24];
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
}
type_E37A0_sound_buffer2;

//shadow type_E37A0_sound_buffer2
typedef struct {//lenght 32 - this is may be format of wav sound file
	int8_t filename_0[18];//first 18 chars - name//30
	int32_t wavData_18;//data of wav//18
	int8_t stub_22[4];//22
	int32_t wavSize_26;//24
	int16_t word_30;//28
}
shadow_sub2type_E37A0_sound_buffer2;

typedef struct {//lenght 2072
	int8_t stub[24];
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
	int8_t filename_14[18];//first 18 chars - name//30
	int32_t xmiData_0;//data of wav//18
	int8_t stub_4[4];//22
	int32_t xmiSize_8;//24
	int16_t word_12;//28	
}
shadow_sub2type_E3808_music_header;

typedef struct {//lenght 216
	int8_t stub[10];
	int8_t stubb[14];
	shadow_sub2type_E3808_music_header track_10[6];
	//int8_t stubb[14];
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

typedef struct
{
	int Id;
	SDL_TimerID SdlId;
	uint32_t IntervalMs;
	SDL_TimerCallback Callback;
} Mix_Timer;

//shadow shadow_type_E3808_music_header
#pragma pack(pop)

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
extern bool autoShowObjectivesForForeignLanguages;
extern int maxSimultaniousSounds;
extern char speechFolder[512];

extern Mix_Chunk* m_ptrSpeechChunk;
extern int m_ptrSpeechBytesOffSet;

bool init_sound();
void SOUND_GameStateChange(const GameState gameState);
//bool load_sound_files();
void clean_up_sound();
void DeleteWarMusic();
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

int32_t ac_sound_call_driver(AIL_DRIVER* drvr, int32_t fn, VDI_CALL* out);

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
void SOUND_set_sequence_volume(int32_t volume, int32_t  milliseconds);
void SOUND_set_master_volume(int32_t volume);
void SOUND_set_sample_volume_panning(HSAMPLE S, int32_t panning);
void SetSamplePosition(HSAMPLE S, int16_t angle, uint8_t distance);
void SOUND_UPDATE();
void ChannelFinished(int channel);

void SOUND_RegisterTimer(int timerIdx, uint32_t(*callback)(uint32_t));
void SOUND_SetTimerPeriod(int timerIdx, uint32_t intervalMs);
void SOUND_StartTimer(int timerIdx);
void SOUND_StopTimer(int timerIdx);
void SOUND_ChangeSamplePlaybackRate(HSAMPLE S, float percent);

bool PlayCdTrackSegment(uint8_t trackIdx, int32_t startPosSec, int32_t lengthMs);
bool IsCdTrackPlaying();
bool EndPlayingCdTrackSegment();
bool ClearCdTrackSegment();
bool AreCdTracksAvailable();
int GetCdTrackCount();

void WarMusicSetVolume(int32_t volume);

void RegisterEffect(int channel, const Mix_Chunk* chunk, float speed, int frequency, int channels, uint16_t format);
template <typename T> void LoadAudioEffect(int channel, const Mix_Chunk* chunk, float speed, int frequency, int channels, uint16_t format);

//void test_midi_play(uint8_t* data, uint8_t* header, int32_t track_number);
#ifdef SOUND_OPENAL
//void ALSOUND_load_wav(char* alBuffer, long alBufferLen);
void ALSOUND_play(int which, Mix_Chunk* chunk, int loops);
void ALSOUND_init();
#endif//SOUND_OPENAL




#endif //PORT_SDL_SOUND
