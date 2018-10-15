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

Bit8u sound_buffer[4][20000];

bool init_sound()
{	
	//#define MUSIC_MID_FLUIDSYNTH
	//Initialize SDL_mixer
	//Mix_SetSoundFonts("c:\\prenos\\remc2\\sound\\SGM-V2.01.sf2");
	if (Mix_OpenAudio(22050, AUDIO_U8/*MIX_DEFAULT_FORMAT*/, 1, 4096) == -1)//4096
	{
		return false;
	}
	//Mix_SetSoundFonts("c:\\prenos\\remc2\\sound\\SGM-V2.01.sf2");
	//Mix_EachSoundFont("c:\\prenos\\remc2\\sound\\SGM-V2.01.sf2");
	load_sound_files();
	return true;
}


Mix_Chunk mychunk;

bool load_sound_files()
{
	//Load the music
	music = Mix_LoadMUS("c:\\prenos\\remc2\\sound\\Music003.mid");
	//music = Mix_LoadMUS("c:\\prenos\\remc2\\sound\\beat.wav");

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

struct{
	int a;
} common_IO_configurations;

struct {
	int a;
} environment_string;

int num_IO_configurations = 3;
int service_rate = -1;
HSAMPLE last_sample;

Bit32s ac_sound_call_driver(AIL_DRIVER* drvr, Bit32s fn, VDI_CALL* in, VDI_CALL* out)/*AIL_DRIVER *drvr,S32 fn, VDI_CALL*in,VDI_CALL *out)*/ {
	switch (fn) {
	case 0x300: {
		drvr->AIL_DRIVER_var4_VHDR->VDI_HDR_var10 = (int)&common_IO_configurations;
		drvr->AIL_DRIVER_var4_VHDR->num_IO_configurations = num_IO_configurations;
		drvr->AIL_DRIVER_var4_VHDR->environment_string = &environment_string;
		drvr->AIL_DRIVER_var4_VHDR->VDI_HDR_var46 = service_rate;
		/*out->AX = 0;
		out->BX = 0;
		out->CX = 0;
		out->DX = 0;
		out->SI = 0;
		out->DI = 0;*/
		break;
	}
	case 0x301: {
		/*drvr->AIL_DRIVER_var4_VHDR->VDI_HDR_var10 = (int)&common_IO_configurations;
		drvr->AIL_DRIVER_var4_VHDR->num_IO_configurations = num_IO_configurations;
		drvr->AIL_DRIVER_var4_VHDR->environment_string = &environment_string;
		drvr->AIL_DRIVER_var4_VHDR->VDI_HDR_var46 = service_rate;*/
		out->AX = 1;//offset
		out->BX = 2;//offset
		//out->CX = 0x2c38;//segment
		//out->DX = 0x2c38;//segment
		out->SI = 0;
		out->DI = 0;
		break;
	}
	case 0x401: {
		mychunk.abuf=(Bit8u*)last_sample->sam_var2_sample_address_start;
		mychunk.alen = last_sample->sam_var4_sample_address_len;
		mychunk.volume = last_sample->sam_var16_volume;
		Mix_PlayChannel(-1, &mychunk, 0);
		/*if (Mix_PlayingMusic() == 0)
		{
			//Play the music
			if (Mix_PlayMusic(music, -1) == -1)
			{
				return 1;
			}
		}*/
		break;
	}
	}
	printf("drvr:%08X, fn:%08X, in:%08X, out:%08X\n", drvr, fn, in, out);
	return 1;
};

AIL_DRIVER* ac_AIL_API_install_driver(int a1, Bit8u* a2, int a3)/*driver_image,n_bytes*///27f720
{
	//printf("drvr:%08X, fn:%08X, in:%08X, out:%08X\n", drvr, fn, in, out);
	return 0;
}

Bit16u actvect[0x1000];

void ac_set_real_vect(Bit32u vectnum, Bit16u real_ptr)
{
	actvect[vectnum] = real_ptr;
	//66
};

Bit16u ac_get_real_vect(Bit32u vectnum)
{
	return actvect[vectnum];
};