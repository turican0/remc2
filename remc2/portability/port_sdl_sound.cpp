#include "port_sdl_sound.h"
#include <adlmidi.h>

/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//The music that will be played
Mix_Music *music = NULL;
Mix_Music* GAME_music[10] = { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };

//The sound effects that will be used
Mix_Chunk *scratch = NULL;
Mix_Chunk *high = NULL;
Mix_Chunk *med = NULL;
Mix_Chunk *low = NULL;

Mix_Chunk gamechunk[32];
HSAMPLE gamechunkHSAMPLE[32];

Bit8u sound_buffer[4][20000];
/*
10
29

128-0
0-0
0-10
0-29
0-80
0-76
0-78
0-78

 channel 1
 2

*/
void test_midi_play(Bit8u* data, Bit8u* header, Bit32s track_number)
{
	Bit8u* acttrack = &header[32 + track_number * 32];
	//int testsize = *(Bit32u*)(&header[32 + (track_number + 1) * 32] + 18) - *(Bit32u*)(acttrack + 18);
	int testsize2 = *(Bit32u*)(acttrack + 26);

	//unsigned char* TranscodeXmiToMid(const unsigned char* pXmiData,	size_t iXmiLength, size_t* pMidLength);
	size_t iXmiLength = testsize2;
	size_t pMidLength;
	Bit8u* outmidi = TranscodeXmiToMid((const Bit8u*)*(Bit32u*)(acttrack + 18), iXmiLength, &pMidLength);
	SDL_RWops* rwmidi = SDL_RWFromMem(outmidi, pMidLength);
#ifdef USE_SDL2
	//Timidity_Init();
	GAME_music[track_number] = Mix_LoadMUSType_RW(rwmidi, MUS_MID, SDL_TRUE);
	//music2 = Mix_LoadMUSType_RW(rwmidi, MIX_MUSIC_TIMIDITY, SDL_TRUE);
#else
	GAME_music[track_number] = Mix_LoadMUS_RW(rwmidi);
#endif
	playmusic2(track_number);
}

void SOUND_start_sequence(Bit32s sequence_num) {
	if (Mix_PlayingMusic() == 0)
	{
		if (Mix_PlayMusic(GAME_music[sequence_num], -1) == -1)
			if (Mix_PausedMusic() == 1)
			{
				Mix_ResumeMusic();
			}
			else
			{
				Mix_PauseMusic();
			}
	}
};

void SOUND_pause_sequence(Bit32s sequence_num) {
	Mix_PauseMusic();
};

void SOUND_stop_sequence(Bit32s sequence_num) {
	Mix_HaltMusic();
};
void SOUND_resume_sequence(Bit32s sequence_num) {
	Mix_ResumeMusic();
};

void SOUND_init_MIDI_sequence(Bit8u* data, Bit8u* header, Bit32s track_number)
{
	Bit8u* acttrack = &header[32 + track_number * 32];
	//int testsize = *(Bit32u*)(&header[32 + (track_number + 1) * 32] + 18) - *(Bit32u*)(acttrack + 18);
	int testsize2 = *(Bit32u*)(acttrack + 26);

	//unsigned char* TranscodeXmiToMid(const unsigned char* pXmiData,	size_t iXmiLength, size_t* pMidLength);
	size_t iXmiLength = testsize2;
	size_t pMidLength;
	Bit8u* outmidi = TranscodeXmiToMid((const Bit8u*)*(Bit32u*)(acttrack + 18), iXmiLength, &pMidLength);
	SDL_RWops* rwmidi = SDL_RWFromMem(outmidi, pMidLength);
#ifdef USE_SDL2
	//Timidity_Init();
	GAME_music[track_number] = Mix_LoadMUSType_RW(rwmidi, MUS_MID, SDL_TRUE);
	//music2 = Mix_LoadMUSType_RW(rwmidi, MIX_MUSIC_TIMIDITY, SDL_TRUE);
#else
	GAME_music[track_number] = Mix_LoadMUS_RW(rwmidi);
#endif
}




//Mix_Chunk mychunk;

bool load_sound_files()
{
	/*//Load the music
	music = Mix_LoadMUS("c:\\prenos\\remc2\\sound\\Music003.mid");
	//music = Mix_LoadMUS("c:\\prenos\\remc2\\sound\\beat.wav");

	//If there was a problem loading the music
	if (music == NULL)
	{
		return false;
	}*/

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

void playmusic2(Bit32s track_number)
{
	if (Mix_PlayingMusic() == 0)
	{
		//Play the music
		if (Mix_PlayMusic(GAME_music[track_number], -1) == -1)
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

struct {
	int a;
} common_IO_configurations;

struct {
	int a;
} environment_string;

int num_IO_configurations = 3;
int service_rate = -1;
//HSAMPLE last_sample;

Bit32s ac_sound_call_driver(AIL_DRIVER* drvr, Bit32s fn, VDI_CALL* in, VDI_CALL* out)/*AIL_DRIVER *drvr,S32 fn, VDI_CALL*in,VDI_CALL *out)*/ {
	switch (fn) {
	case 0x300: {//AIL_API_install_driver
		drvr->VHDR_4->VDI_HDR_var10 = (void*)&common_IO_configurations;
		drvr->VHDR_4->num_IO_configurations_14 = num_IO_configurations;
		drvr->VHDR_4->environment_string_16 = (Bit32u)&environment_string;
		drvr->VHDR_4->VDI_HDR_var46 = service_rate;
		/*out->AX = 0;
		out->BX = 0;
		out->CX = 0;
		out->DX = 0;
		out->SI = 0;
		out->DI = 0;*/
		break;
	}
	case 0x301: {//AIL_API_install_DIG_driver_file/AIL_API_install_MDI_driver_file
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
	case 0x304: {//AIL_API_install_DIG_driver_file/AIL_API_install_MDI_driver_file
		break;
	}
	case 0x305: {//AIL_API_install_DIG_driver_file/AIL_API_install_MDI_driver_file
		break;
	}
	case 0x306: {//AIL_API_uninstall_driver/AIL_API_uninstall_MDI_driver_file
		break;
	}
	case 0x401: {
		/*		mychunk.abuf=(Bit8u*)last_sample->start_2_3[0];
				mychunk.alen = last_sample->len_4_5[0];
				mychunk.volume = last_sample->volume_16;
				//mychunk.allocated = 0;

				#ifdef USE_SDL2
						Mix_PlayChannel(-1, &mychunk, 0);
				#else
						Mix_PlayChannel(-1, &mychunk, 0);
				#endif*/
		break;
	}
	case 0x501: {//AIL_API_install_MDI_INI
		break;
	}
	case 0x502: {//AIL_API_install_MDI_INI
		break;
	}
	default:
	{
		break;
	}
	}
	//printf("drvr:%08X, fn:%08X, in:%08X, out:%08X\n", drvr, fn, in, out);
	return 1;
};

void SOUND_start_sample(HSAMPLE S) {
	gamechunk[S->index_sample].abuf = (Bit8u*)S->start_2_3[0];
	gamechunk[S->index_sample].alen = S->len_4_5[0];
	gamechunk[S->index_sample].volume = S->volume_16;
	gamechunkHSAMPLE[S->index_sample] = S;
	Mix_PlayChannel(S->index_sample, &gamechunk[S->index_sample], 0);
};

void SOUND_end_sample(HSAMPLE S) {
	Mix_HaltChannel(-1);
};

void SOUND_finalize(int channel) {
	HSAMPLE S = gamechunkHSAMPLE[channel];
	if (S)
	{
		if (S->status_1 != 1)
		{
			if (S->status_1 != 2)
			{
				S->status_1 = 2;
			}
		}
		S->vol_scale_18[0][0] = 0;
		S->flags_14 = 0;
		S->vol_scale_18[0][2] = 0;
		S->vol_scale_18[0][3] = 0;
	}
}

int run();

bool init_sound()
{
	//run();
	//#define MUSIC_MID_FLUIDSYNTH
	//Initialize SDL_mixer
	if (Mix_OpenAudio(11025/*22050*/, AUDIO_U8/*MIX_DEFAULT_FORMAT*/, 1, 4096) == -1)//4096
	{
		return false;
	}
	//Mix_SetSoundFonts("c:\\prenos\\Magic2\\sf2\\TOM-SF2.sf2");
	load_sound_files();
	/*
Mix_HookMusicFinished(void (SDLCALL *music_finished)(void));
*/
	Mix_ChannelFinished(SOUND_finalize);
	return true;
}

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

void test_music()
{
}

void my_audio_callback(void *midi_player, Uint8 *stream, int len);

/* variable declarations */
static Uint32 is_playing = 0; /* remaining length of the sample we have to play */
static short buffer[4096]; /* Audio buffer */

int run()
{
	/* local variables */
	static SDL_AudioSpec            spec; /* the specs of our piece of music */
	static struct ADL_MIDIPlayer    *midi_player = NULL; /* Instance of ADLMIDI player */
	static const char               *music_path = NULL; /* Path to music file */

	music_path = "c:\\prenos\\remc2\\remc2\\memimages\\midi\\Music001.mid";

	/* Initialize SDL.*/
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
		return 1;

	spec.freq = 44100;
	spec.format = AUDIO_S16SYS;
	spec.channels = 2;
	spec.samples = 2048;

	/* Initialize ADLMIDI */
	midi_player = adl_init(spec.freq);
	if (!midi_player)
	{
		fprintf(stderr, "Couldn't initialize ADLMIDI: %s\n", adl_errorString());
		return 1;
	}

	/* set the callback function */
	spec.callback = my_audio_callback;
	/* set ADLMIDI's descriptor as userdata to use it for sound generation */
	spec.userdata = midi_player;

	/* Open the audio device */
	if (SDL_OpenAudio(&spec, NULL) < 0)
	{
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		return 1;
	}

	/* Optionally Setup ADLMIDI as you want */

	/* Set using of embedded bank by ID */
	/*adl_setBank(midi_player, 68);*/

	/* Set using of custom bank (WOPL format) loaded from a file */
	/*adl_openBankFile(midi_player, "/home/vitaly/Yandex.Disk/??????/Wolfinstein.wopl");*/

	/* Open the MIDI (or MUS, IMF or CMF) file to play */
	if (adl_openFile(midi_player, music_path) < 0)
	{
		fprintf(stderr, "Couldn't open music file: %s\n", adl_errorInfo(midi_player));
		SDL_CloseAudio();
		adl_close(midi_player);
		return 1;
	}

	is_playing = 1;
	/* Start playing */
	SDL_PauseAudio(0);

	printf("Playing... Hit Ctrl+C to quit!\n");

	/* wait until we're don't playing */
	while (is_playing)
	{
		SDL_Delay(100);
	}

	/* shut everything down */
	SDL_CloseAudio();
	adl_close(midi_player);

	return 0;
}

/*
 audio callback function
 here you have to copy the data of your audio buffer into the
 requesting audio buffer (stream)
 you should only copy as much as the requested length (len)
*/
void my_audio_callback(void *midi_player, Uint8 *stream, int len)
{
	struct ADL_MIDIPlayer* p = (struct ADL_MIDIPlayer*)midi_player;

	/* Convert bytes length into total count of samples in all channels */
	int samples_count = len / 2;

	/* Take some samples from the ADLMIDI */
	samples_count = adl_play(p, samples_count, (short*)buffer);

	if (samples_count <= 0)
	{
		is_playing = 0;
		SDL_memset(stream, 0, len);
		return;
	}

	/* Send buffer to the audio device */
	SDL_memcpy(stream, (Uint8*)buffer, samples_count * 2);
}