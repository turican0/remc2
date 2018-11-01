#ifndef PORT_SDL_SOUND
#define PORT_SDL_SOUND
#include "mctypes.h"
#include "SDL.h"
#include "SDL_mixer_ext.h"
#include "music.h"
#include "mixer.h"
//#include "music_timidity.h"
#include "xmi2mid.h"
#include <string>

#include "../engine/ail_sound.h"
#include "port_filesystem.h"

#define USE_SDL2

extern bool hqsound;
extern bool oggmusic;
extern char oggmusicpath[512];
extern bool oggmusicalternative;

bool init_sound();
bool load_sound_files();
void clean_up_sound();
int playsound1();
int playsound2();
int playsound3();
int playsound4();
void playmusic1();
void stopmusic1();
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
//void test_midi_play(Bit8u* data, Bit8u* header, Bit32s track_number);





#endif //PORT_SDL_SOUND