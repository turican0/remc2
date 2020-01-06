#include "port_sdl_sound.h"
#include <adlmidi.h>

/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

bool debug_first_sound=true;
/*#ifdef DEBUG_SOUND
	debug_first_sound = true;
#endif //DEBUG_SOUND*/

bool hqsound=false;
bool oggmusic=false;
bool oggmusicalternative = false;
char oggmusicpath[512];

//The music that will be played
#ifdef SOUND_SDLMIXER
Mix_Music* GAME_music[20] = { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };
#endif//SOUND_SDLMIXER
#ifdef SOUND_OPENAL
//Mix_Music* GAME_music[20] = { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };
ALint source_state[32];
ALuint alSource[32];      //source
ALuint alSampleSet[32];
#endif//SOUND_OPENAL
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

	//Timidity_Init();
	if (track_number > 20)
	{
		exit(0);
	}
#ifdef SOUND_SDLMIXER
	GAME_music[track_number] = Mix_LoadMUSType_RW(rwmidi, MUS_MID, SDL_TRUE);
#endif//SOUND_SDLMIXER
	//music2 = Mix_LoadMUSType_RW(rwmidi, MIX_MUSIC_TIMIDITY, SDL_TRUE);
	playmusic2(track_number);
}

void SOUND_start_sequence(Bit32s sequence_num) {
	//3 - menu
	//4 - intro
#ifdef SOUND_SDLMIXER
	//volume fix
	if(Mix_VolumeMusic(-1)==0)
		Mix_VolumeMusic(0x7f);
	//volume fix

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
#endif//SOUND_SDLMIXER
};

void SOUND_pause_sequence(Bit32s sequence_num) {
#ifdef SOUND_SDLMIXER
	Mix_PauseMusic();
#endif//SOUND_SDLMIXER
};

void SOUND_stop_sequence(Bit32s sequence_num) {
#ifdef SOUND_SDLMIXER
	Mix_HaltMusic();
#endif//SOUND_SDLMIXER
};
void SOUND_resume_sequence(Bit32s sequence_num) {
#ifdef SOUND_SDLMIXER
	Mix_ResumeMusic();
#endif//SOUND_SDLMIXER
};

void SOUND_set_sequence_volume(Bit32s volume) {
#ifdef SOUND_SDLMIXER
	Mix_VolumeMusic(volume);
#endif//SOUND_SDLMIXER
};

void SOUND_init_MIDI_sequence(Bit8u* data, Bit8u* header, Bit32s track_number)
{
	Bit8u* acttrack = &header[32 + track_number * 32];
	//int testsize = *(Bit32u*)(&header[32 + (track_number + 1) * 32] + 18) - *(Bit32u*)(acttrack + 18);
	int testsize2 = *(Bit32u*)(acttrack + 26);

	//unsigned char* TranscodeXmiToMid(const unsigned char* pXmiData,	size_t iXmiLength, size_t* pMidLength);
	size_t iXmiLength = testsize2;
	size_t pMidLength;
	dirsstruct helpdirsstruct;

	if (oggmusic) {
		char buffer1[512]="";
		char buffer2[512] = "";
		char buffer3[512] = "";
		char buffer4[512] = "";
		//if (track_number > 1)track_number = 0;
		if (oggmusicalternative)///&&track_number==4
		{			
			if (track_number == 0)
			{
				sprintf(buffer2, "%salternative\\cave\\", oggmusicpath);
			}
			else if (track_number == 1)
			{
				sprintf(buffer2, "%salternative\\cave\\", oggmusicpath);
			}
			else if (track_number == 2)
			{
				sprintf(buffer2, "%salternative\\cave\\", oggmusicpath);
			}
			else if (track_number == 3)
			{
				sprintf(buffer2, "%salternative\\cave\\", oggmusicpath);
			}
			else if (track_number == 4)
			{
				sprintf(buffer2, "%salternative\\cave\\", oggmusicpath);
			}
			else if (track_number == 5)
			{
				sprintf(buffer2, "%salternative\\cave\\", oggmusicpath);
			}
			else
				sprintf(buffer2, "%salternative\\cave\\", oggmusicpath);

			sprintf(buffer1, "%s", oggmusicpath);

			FixDir(buffer3, buffer2);
			FixDir(buffer4, buffer1);

			helpdirsstruct = getListDir(buffer3);
			if (helpdirsstruct.number > 0)
			{
				int randtrack = rand()%(helpdirsstruct.number + 1);
				if(randtrack==0)sprintf(buffer4, "%smusic%d.ogg", oggmusicpath, track_number);
				else
					sprintf(buffer4, "%s%s", buffer3,helpdirsstruct.dir[randtrack-1]);
			}
			else
				sprintf(buffer4, "%smusic%d.ogg", oggmusicpath, track_number);
		}
		else
			sprintf(buffer4, "%smusic%d.ogg", oggmusicpath, track_number);
#ifdef SOUND_SDLMIXER
		GAME_music[track_number] = Mix_LoadMUS(buffer4);
#endif//SOUND_SDLMIXER
	}
	else
	{
		Bit8u* outmidi = TranscodeXmiToMid((const Bit8u*)*(Bit32u*)(acttrack + 18), iXmiLength, &pMidLength);
		SDL_RWops* rwmidi = SDL_RWFromMem(outmidi, pMidLength);

		//Timidity_Init();
#ifdef SOUND_SDLMIXER
		GAME_music[track_number] = Mix_LoadMUSType_RW(rwmidi, MUS_MID, SDL_TRUE);
#endif//SOUND_SDLMIXER
		//music2 = Mix_LoadMUSType_RW(rwmidi, MIX_MUSIC_TIMIDITY, SDL_TRUE);

	}
#ifdef SOUND_OPENAL
	/*if (!AL_inited)
	{
		//ALSOUND_init();
		AL_inited = true;
	}*/
#endif//SOUND_OPENAL
}




//Mix_Chunk mychunk;

/*bool load_sound_files()
{
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
}*/

void clean_up_sound()
{
	/*//Free the sound effects
	Mix_FreeChunk(scratch);
	Mix_FreeChunk(high);
	Mix_FreeChunk(med);
	Mix_FreeChunk(low);*/

	//Free the music
	//Mix_FreeMusic(music);
#ifdef SOUND_SDLMIXER
	for (int i = 0;i < 10;i++)
	{
		//Mix_FreeChunk(GAME_musicmp3[i]);
		Mix_FreeMusic(GAME_music[i]);
	}

	//Quit SDL_mixer
	Mix_CloseAudio();
#endif//SOUND_SDLMIXER
}
/*
int load_music_files() {
	GAME_musicmp3[0] = Mix_LoadMUS("music.mp3");
}*/
/*
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
*/
/*
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
*/
/*
void stopmusic1()
{
	Mix_HaltMusic();
}
*/
void playmusic2(Bit32s track_number)
{
#ifdef SOUND_SDLMIXER
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
#endif//SOUND_SDLMIXER
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

void SOUND_set_master_volume(Bit32s volume) {
	//gamechunk[S->index_sample].volume = volume;
#ifdef SOUND_SDLMIXER
	Mix_Volume(-1, volume);
#endif//SOUND_SDLMIXER
	
	//may be can fix - must analyze

}

void SOUND_set_sample_volume(HSAMPLE S, Bit32s volume) {
#ifdef SOUND_SDLMIXER
	gamechunk[S->index_sample].volume = volume;
	Mix_Volume(S->index_sample, volume);
#endif//SOUND_SDLMIXER
}

void SOUND_start_sample(HSAMPLE S) {
#ifdef SOUND_SDLMIXER
	if (hqsound)
	{
		gamechunk[S->index_sample].abuf = (Bit8u*)S->start_44mhz;
		gamechunk[S->index_sample].alen = S->len_4_5[0] * 4;
		#ifdef DEBUG_SOUND
			if (debug_first_sound) {
				debug_printf("SOUND_start_sample-hq:%08X\n", S->start_44mhz);
				debug_first_sound = false;
			}
		#endif //DEBUG_SOUND		
	}
	else
	{
		#ifdef DEBUG_SOUND
			if (debug_first_sound) {
				debug_printf("SOUND_start_sample:%08X\n", S->start_44mhz);
				debug_first_sound = false;
			}
		#endif //DEBUG_SOUND
		gamechunk[S->index_sample].abuf = (Bit8u*)S->start_2_3[0];
		gamechunk[S->index_sample].alen = S->len_4_5[0];
	}
	
	gamechunk[S->index_sample].volume = S->volume_16;
	gamechunkHSAMPLE[S->index_sample] = S;

	Mix_PlayChannel(S->index_sample, &gamechunk[S->index_sample], 0);
#endif//SOUND_SDLMIXER
#ifdef SOUND_OPENAL
	//sound_load_wav((char*)S->start_44mhz, sizeof(S->start_44mhz));
	if (hqsound)
	{
		gamechunk[S->index_sample].abuf = (Bit8u*)S->start_44mhz;
		gamechunk[S->index_sample].alen = S->len_4_5[0] * 4;
	}
	else
	{
		gamechunk[S->index_sample].abuf = (Bit8u*)S->start_2_3[0];
		gamechunk[S->index_sample].alen = S->len_4_5[0];
	}

	gamechunk[S->index_sample].volume = S->volume_16;
	gamechunkHSAMPLE[S->index_sample] = S;
	ALSOUND_play(S->index_sample,&gamechunk[S->index_sample],0);
#endif//SOUND_OPENAL
};

Bit32u SOUND_sample_status(HSAMPLE S) {
#ifdef SOUND_SDLMIXER
	if (Mix_Playing(S->index_sample)==0)return 2;
#endif//SOUND_SDLMIXER
#ifdef SOUND_OPENAL
	return 2;
#endif//SOUND_SDLMIXER
	return 0;
}

void SOUND_end_sample(HSAMPLE S) {
#ifdef SOUND_SDLMIXER
	Mix_HaltChannel(-1);
#endif//SOUND_SDLMIXER
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
	srand(time(NULL));
#ifdef SOUND_SDLMIXER
	if (hqsound) {
		if (Mix_OpenAudio(44100, AUDIO_S16, 1, 4096) == -1)//4096
		//if (Mix_OpenAudio(44100, AUDIO_S16, 2, 4096) == -1)//4096
		//if (Mix_OpenAudio(11025, AUDIO_S8, 1, 4096) == -1)//4096
		{
			return false;
		}
	}
	else
	{
		if (Mix_OpenAudio(22050, AUDIO_U8/*MIX_DEFAULT_FORMAT*/, 1, 4096) == -1)//4096
		//if (Mix_OpenAudio(11025/*22050*/, AUDIO_U8/*MIX_DEFAULT_FORMAT*/, 1, 4096) == -1)//4096
		{
			return false;
		}
	}

	//Mix_SetSoundFonts("c:\\prenos\\Magic2\\sf2\\TOM-SF2.sf2");
	//load_sound_files();
	/*if(mp3music)
		load_music_files();*/
	/*
Mix_HookMusicFinished(void (SDLCALL *music_finished)(void));
*/
	Mix_ChannelFinished(SOUND_finalize);
#endif//SOUND_SDLMIXER
#ifdef SOUND_OPENAL
	if (hqsound) {
		ALSOUND_init();
	}
	else
	{
		ALSOUND_init();
	}

#endif//SOUND_OPENAL
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

#define TEST_ERROR(_msg)		\
	error = alGetError();		\
	if (error != AL_NO_ERROR) {	\
		fprintf(stderr, _msg "\n");	\
		return;		\
	}
//------------------

void SOUND_UPDATE() {
#ifdef SOUND_OPENAL
	for (int i = 0; i < 32; i++)
	{
		if (source_state[i] == AL_PLAYING)
		{
			alGetSourcei(alSource[i], AL_SOURCE_STATE, &source_state[i]);
			//TEST_ERROR("source state get");
		}
		else
		{
			alDeleteSources(1, &alSource[i]);
			alDeleteBuffers(1, &alSampleSet[i]);
		}
	}
	/*while (source_state == AL_PLAYING) {
		alGetSourcei(alSource, AL_SOURCE_STATE, &source_state);
		TEST_ERROR("source state get");
	}*/
#endif//SOUND_OPENAL
};


#ifdef SOUND_OPENAL
ALCcontext* context;
ALCdevice* device;
const ALCchar* defaultDeviceName;
ALCenum error;

static void list_audio_devices(const ALCchar* devices)
{
	const ALCchar* device = devices, * next = devices + 1;
	size_t len = 0;

	fprintf(stdout, "Devices list:\n");
	fprintf(stdout, "----------\n");
	while (device && *device != '\0' && next && *next != '\0') {
		fprintf(stdout, "%s\n", device);
		len = strlen(device);
		device += (len + 1);
		next += (len + 2);
	}
	fprintf(stdout, "----------\n");
}
void ALSOUND_init()
{
	ALboolean enumeration;
	const ALCchar* devices;
	//const ALCchar* defaultDeviceName = argv[1];
	int ret;
#ifdef LIBAUDIO
	WaveInfo* wave;
#endif
	char* bufferData;
	ALCdevice* device;
	ALvoid* data;
	ALCcontext* context;
	ALsizei size, freq;
	ALenum format;
	//ALuint buffer, source;
	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	ALboolean loop = AL_FALSE;

	//fprintf(stdout, "Using " BACKEND " as audio backend\n");

	enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
	if (enumeration == AL_FALSE)
		fprintf(stderr, "enumeration extension not available\n");

	list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));

	//if (!defaultDeviceName)
	defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);

	device = alcOpenDevice(defaultDeviceName);
	if (!device) {
		fprintf(stderr, "unable to open default device\n");
		return;
	}

	fprintf(stdout, "Device: %s\n", alcGetString(device, ALC_DEVICE_SPECIFIER));

	alGetError();

	context = alcCreateContext(device, NULL);
	if (!alcMakeContextCurrent(context)) {
		fprintf(stderr, "failed to make default context\n");
		return;
	}
	TEST_ERROR("make default context");

	/* set orientation */
	alListener3f(AL_POSITION, 0, 0, 1.0f);
	TEST_ERROR("listener position");
	alListener3f(AL_VELOCITY, 0, 0, 0);
	TEST_ERROR("listener velocity");
	alListenerfv(AL_ORIENTATION, listenerOri);
	TEST_ERROR("listener orientation");
	/*
	for (int i = 0; i < 32; i++)
	{
		alGenSources((ALuint)1, &alSource[i]);
		TEST_ERROR("source generation");

		alSourcef(alSource[i], AL_PITCH, 1);
		TEST_ERROR("source pitch");
		alSourcef(alSource[i], AL_GAIN, 1);
		TEST_ERROR("source gain");
		alSource3f(alSource[i], AL_POSITION, 0, 0, 0);
		TEST_ERROR("source position");
		alSource3f(alSource[i], AL_VELOCITY, 0, 0, 0);
		TEST_ERROR("source velocity");
		alSourcei(alSource[i], AL_LOOPING, AL_FALSE);
		TEST_ERROR("source looping");

		alGenBuffers(1, &alSampleSet[i]);
		TEST_ERROR("buffer generation");
	}*/
}

ALenum alFormatBuffer = AL_FORMAT_MONO16/*AL_FORMAT_MONO16*/;    //buffer format
ALsizei alFreqBuffer = 44100;       //frequency
//ALint source_state;
/*void ALSOUND_load_wav(char* alBuffer,long alBufferLen)
{
	//Now for the second part, loading a wav file.You have to open a file, fill buffers with dataand then attach it to a source.

	//char* alBuffer;             //data for the buffer
	
	//long       alBufferLen;        //bit depth
	//ALboolean    alLoop;         //loop


	//load the wave file
	//alutLoadWAVFile("my_music.wav", alFormatBuffer, (void**) alBuffer, (unsigned int*) alBufferLen, alFreqBuffer);//, alLoop);

	//create a source
	alGenSources(1, &alSource);

	//create  buffer
	alGenBuffers(1, &alSampleSet);

	//put the data into our sampleset buffer
	alBufferData(alSampleSet, alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);

	//assign the buffer to this source
	alSourcei(alSource, AL_BUFFER, alSampleSet);

	//release the data
	//alutUnloadWAV(alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);
}*/
void ALSOUND_play(int which, Mix_Chunk* mixchunk, int loops)
{
	/*
	//Once the sound is loaded we can play it.To do this we use alSourcePlay.

	alSourcei(alSource, AL_LOOPING, AL_TRUE);

	//play
	alSourcePlay(alSource);

	//alSourcePlay(source);

	//to stop
	//alSourceStop(alSource);
	*/
	/*
	alSourcePlay(): Play, replay, or resume a source.
	alSourceStop(): Stop one or more sources.
	alSourceRewind(): Rewind a source (set the playback position to the beginning).
	alSourcePause(): Pause a source.
	*/
	//alutLoadWAVFile("test.wav", &format, &data, &size, &freq, &loop);
	//TEST_ERROR("loading wav file");
	
	//----
	//delete
	//alDeleteSources(1, &alSource);
	//delete our buffer
	//alDeleteBuffers(1, &alSampleSet);
    //----
	alGenSources((ALuint)1, &alSource[which]);
	TEST_ERROR("source generation");

	alSourcef(alSource[which], AL_PITCH, 1);
	TEST_ERROR("source pitch");
	alSourcef(alSource[which], AL_GAIN, 1);
	TEST_ERROR("source gain");
	alSource3f(alSource[which], AL_POSITION, 0, 0, 0);
	TEST_ERROR("source position");
	alSource3f(alSource[which], AL_VELOCITY, 0, 0, 0);
	TEST_ERROR("source velocity");
	alSourcei(alSource[which], AL_LOOPING, AL_FALSE);
	TEST_ERROR("source looping");

	alGenBuffers(1, &alSampleSet[which]);
	TEST_ERROR("buffer generation");
	//------


	alBufferData(alSampleSet[which], alFormatBuffer, mixchunk->abuf, mixchunk->alen, alFreqBuffer);
	TEST_ERROR("buffer copy");

	alSourcei(alSource[which], AL_BUFFER, alSampleSet[which]);
	TEST_ERROR("buffer binding");

	alSourcePlay(alSource[which]);
	TEST_ERROR("source playing");

	alGetSourcei(alSource[which], AL_SOURCE_STATE, &source_state[which]);
	TEST_ERROR("source state get");
	/*while (source_state == AL_PLAYING) {
		alGetSourcei(alSource, AL_SOURCE_STATE, &source_state);
		TEST_ERROR("source state get");
	}

	alDeleteSources(1, &alSource);
	alDeleteBuffers(1, &alSampleSet);*/
}

void ALSOUND_delete()
{
	//Once you’ve finished don’t forget to clean memoryand release OpenAL contextand device

	//alDeleteSources(1, &alSource);

	//delete our buffer
	//alDeleteBuffers(1, &alSampleSet);

	context = alcGetCurrentContext();

	//Get device for active context
	device = alcGetContextsDevice(context);

	//Disable context
	alcMakeContextCurrent(NULL);

	//Release context(s)
	alcDestroyContext(context);

	//Close device
	alcCloseDevice(device);
}


/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdbool.h>

#include <AL/al.h>
#include <AL/alc.h>
*/
/*
#ifdef LIBAUDIO
#include <audio/wave.h>
#define BACKEND	"libaudio"
#else
#include <AL/alut.h>
#define BACKEND "alut"
#endif
*/



	
static inline ALenum to_al_format(short channels, short samples)
{
	bool stereo = (channels > 1);

	switch (samples) {
	case 16:
		if (stereo)
			return AL_FORMAT_STEREO16;
		else
			return AL_FORMAT_MONO16;
	case 8:
		if (stereo)
			return AL_FORMAT_STEREO8;
		else
			return AL_FORMAT_MONO8;
	default:
		return -1;
	}
}
#endif//SOUND_OPENAL
/*
int main(int argc, char** argv)
{
	ALboolean enumeration;
	const ALCchar* devices;
	const ALCchar* defaultDeviceName = argv[1];
	int ret;
#ifdef LIBAUDIO
	WaveInfo* wave;
#endif
	char* bufferData;
	ALCdevice* device;
	ALvoid* data;
	ALCcontext* context;
	ALsizei size, freq;
	ALenum format;
	ALuint buffer, source;
	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	ALboolean loop = AL_FALSE;
	ALCenum error;
	ALint source_state;

	//fprintf(stdout, "Using " BACKEND " as audio backend\n");

	enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
	if (enumeration == AL_FALSE)
		fprintf(stderr, "enumeration extension not available\n");

	list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));

	if (!defaultDeviceName)
		defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);

	device = alcOpenDevice(defaultDeviceName);
	if (!device) {
		fprintf(stderr, "unable to open default device\n");
		return -1;
	}

	fprintf(stdout, "Device: %s\n", alcGetString(device, ALC_DEVICE_SPECIFIER));

	alGetError();

	context = alcCreateContext(device, NULL);
	if (!alcMakeContextCurrent(context)) {
		fprintf(stderr, "failed to make default context\n");
		return -1;
	}
	TEST_ERROR("make default context");

	alListener3f(AL_POSITION, 0, 0, 1.0f);
	TEST_ERROR("listener position");
	alListener3f(AL_VELOCITY, 0, 0, 0);
	TEST_ERROR("listener velocity");
	alListenerfv(AL_ORIENTATION, listenerOri);
	TEST_ERROR("listener orientation");

	alGenSources((ALuint)1, &source);
	TEST_ERROR("source generation");

	alSourcef(source, AL_PITCH, 1);
	TEST_ERROR("source pitch");
	alSourcef(source, AL_GAIN, 1);
	TEST_ERROR("source gain");
	alSource3f(source, AL_POSITION, 0, 0, 0);
	TEST_ERROR("source position");
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	TEST_ERROR("source velocity");
	alSourcei(source, AL_LOOPING, AL_FALSE);
	TEST_ERROR("source looping");

	alGenBuffers(1, &buffer);
	TEST_ERROR("buffer generation");

#ifdef LIBAUDIO
	wave = WaveOpenFileForReading("test.wav");
	if (!wave) {
		fprintf(stderr, "failed to read wave file\n");
		return -1;
	}

	ret = WaveSeekFile(0, wave);
	if (ret) {
		fprintf(stderr, "failed to seek wave file\n");
		return -1;
	}

	bufferData = malloc(wave->dataSize);
	if (!bufferData) {
		perror("malloc");
		return -1;
	}

	ret = WaveReadFile(bufferData, wave->dataSize, wave);
	if (ret != wave->dataSize) {
		fprintf(stderr, "short read: %d, want: %d\n", ret, wave->dataSize);
		return -1;
	}

	alBufferData(buffer, to_al_format(wave->channels, wave->bitsPerSample),
		bufferData, wave->dataSize, wave->sampleRate);
	TEST_ERROR("failed to load buffer data");
#else
	//alutLoadWAVFile("test.wav", &format, &data, &size, &freq, &loop);
	TEST_ERROR("loading wav file");

	alBufferData(buffer, format, data, size, freq);
	TEST_ERROR("buffer copy");
#endif

	alSourcei(source, AL_BUFFER, buffer);
	TEST_ERROR("buffer binding");

	alSourcePlay(source);
	TEST_ERROR("source playing");

	alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	TEST_ERROR("source state get");
	while (source_state == AL_PLAYING) {
		alGetSourcei(source, AL_SOURCE_STATE, &source_state);
		TEST_ERROR("source state get");
	}

	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);

	return 0;
}
*/
