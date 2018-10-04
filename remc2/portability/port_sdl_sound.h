#ifndef PORT_SDL_SOUND
#define PORT_SDL_SOUND
#include "SDL.h"
#include "SDL_mixer.h"
#include <string>

bool init_sound();
bool load_sound_files();
void clean_up_sound();
int playsound1();
int playsound2();
int playsound3();
int playsound4();
void playmusic1();
void stopmusic1();





#endif //PORT_SDL_SOUND