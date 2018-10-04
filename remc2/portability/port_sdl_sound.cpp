#include "port_sdl_sound.h"

/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//The music that will be played
Mix_Music *music = NULL;

//The sound effects that will be used
Mix_Chunk *scratch = NULL;
Mix_Chunk *high = NULL;
Mix_Chunk *med = NULL;
Mix_Chunk *low = NULL;

bool init_sound()
{
	//Initialize SDL_mixer
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		return false;
	}
	return true;
}

bool load_sound_files()
{
	//Load the music
	music = Mix_LoadMUS("beat.wav");

	//If there was a problem loading the music
	if (music == NULL)
	{
		return false;
	}

	//Load the sound effects
	scratch = Mix_LoadWAV("scratch.wav");
	high = Mix_LoadWAV("high.wav");
	med = Mix_LoadWAV("medium.wav");
	low = Mix_LoadWAV("low.wav");

	//If there was a problem loading the sound effects
	if ((scratch == NULL) || (high == NULL) || (med == NULL) || (low == NULL))
	{
		return false;
	}

	//If everything loaded fine
	return true;
}

void clean_up_sound()
{
	//Free the sound effects
	Mix_FreeChunk(scratch);
	Mix_FreeChunk(high);
	Mix_FreeChunk(med);
	Mix_FreeChunk(low);

	//Free the music
	Mix_FreeMusic(music);

	//Quit SDL_mixer
	Mix_CloseAudio();
}

int playsound1()
{
	//Play the scratch effect
	if (Mix_PlayChannel(-1, scratch, 0) == -1)
	{
		return 1;
	}
	return 0;
}
int playsound2()
{
	//Play the scratch effect
	if (Mix_PlayChannel(-1, high, 0) == -1)
	{
		return 1;
	}
	return 0;
}
int playsound3()
{
	//Play the scratch effect
	if (Mix_PlayChannel(-1, med, 0) == -1)
	{
		return 1;
	}
	return 0;
}
int playsound4()
{
	//Play the scratch effect
	if (Mix_PlayChannel(-1, low, 0) == -1)
	{
		return 1;
	}
	return 0;
}

void playmusic1()
{
	if (Mix_PlayingMusic() == 0)
	{
		//Play the music
		if (Mix_PlayMusic(music, -1) == -1)
			if (Mix_PausedMusic() == 1)
			{
				//Resume the music
				Mix_ResumeMusic();
			}
		//If the music is playing
			else
			{
				//Pause the music
				Mix_PauseMusic();
			}
	}
}

void stopmusic1()
{
	Mix_HaltMusic();
}
