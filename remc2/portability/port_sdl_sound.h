#ifndef PORT_SDL_SOUND
#define PORT_SDL_SOUND
#include "mctypes.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include <string>

#include "../engine/ail_sound.h"

bool init_sound();
bool load_sound_files();
void clean_up_sound();
int playsound1();
int playsound2();
int playsound3();
int playsound4();
void playmusic1();
void stopmusic1();

void ac_sound_call_driver(Bit32s* drvr, Bit32s fn, VDI_CALL* in, VDI_CALL* out);





#endif //PORT_SDL_SOUND