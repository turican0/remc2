#ifndef PORT_SDL_SOUND
#define PORT_SDL_SOUND

//#define SOUND_OPENAL

#include "mctypes.h"

#ifdef _MSC_VER
	#define SOUND_SDLMIXER
	#include "SDL.h"
#else
	#define NO_ADLMIDI
    #include "SDL2/SDL.h"
#endif

#ifdef SOUND_SDLMIXER
	#include "SDL_mixer_ext.h"
	#include "music.h"
	#include "mixer.h"
#endif//SOUND_SDLMIXER

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
extern Bit8u sound_buffer[4][20000];
//extern HSAMPLE last_sample;

Bit32s ac_sound_call_driver(AIL_DRIVER* drvr, Bit32s fn, VDI_CALL* in, VDI_CALL* out);

void ac_set_real_vect(Bit32u vectnum, Bit16u real_ptr);
Bit16u ac_get_real_vect(Bit32u vectnum);
AIL_DRIVER* ac_AIL_API_install_driver(int a1, Bit8u* a2, int a3);

void SOUND_init_MIDI_sequence(Bit8u* data, Bit8u* header, Bit32s track_number);

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
//void test_midi_play(Bit8u* data, Bit8u* header, Bit32s track_number);
#ifdef SOUND_OPENAL
//void ALSOUND_load_wav(char* alBuffer, long alBufferLen);
void ALSOUND_play(int which, Mix_Chunk* chunk, int loops);
void ALSOUND_init();
#endif//SOUND_OPENAL




#endif //PORT_SDL_SOUND