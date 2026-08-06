#include "engine/engine_support.h"
#include "port_sdl_sound.h"

#include <adlmidi.h>
#include <iostream>

#if defined(__linux__) || defined(__APPLE__)
    #include <limits>
    #define MAX_PATH PATH_MAX
#endif

/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

bool debug_first_sound=true;

bool hqsound=false;
bool oggmusic=false;
bool oggmusicalternative = false;
char oggmusicFolder[512];
char speechFolder[512];
Mix_Chunk* m_ptrSpeechChunk;
int m_ptrSpeechBytesOffSet;

bool fixspeedsound = false;
bool autoShowObjectivesForForeignLanguages = false;
int maxSimultaniousSounds = 10;

int32_t last_sequence_num = 0;

int lastMusicVolume = -1;
int settingsMusicVolume = 127;
int num_IO_configurations = 3;
int service_rate = -1;
int master_volume = -1;

int music_war_channel_index = maxSimultaniousSounds + 1;
bool warMusicOn = false;

//The music that will be played
#ifdef SOUND_SDLMIXER
Mix_Music* GAME_music[20] = { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };
Mix_Chunk* GAME_music_war = NULL;
#endif//SOUND_SDLMIXER
#ifdef SOUND_OPENAL
//Mix_Music* GAME_music[20] = { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };
ALint source_state[32];
ALuint alSource[32];      //source
ALuint alSampleSet[32];
#endif//SOUND_OPENAL
std::map<int, Mix_Chunk> GameChunks = std::map<int, Mix_Chunk>();
std::map<int, HSAMPLE> GameChunkHSamples = std::map<int, HSAMPLE>();

uint8_t sound_buffer[4][20000];

std::vector<Mix_Timer> Timers = std::vector<Mix_Timer>();
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
void test_midi_play(uint8_t*  /*data*/, uint8_t* header, int32_t track_number)
{
	uint8_t* acttrack = &header[32 + track_number * 32];
	//int testsize = *(uint32_t*)(&header[32 + (track_number + 1) * 32] + 18) - *(uint32_t*)(acttrack + 18);
	int testsize2 = *(uint32_t*)(acttrack + 26);

	//unsigned char* TranscodeXmiToMid(const unsigned char* pXmiData,	size_t iXmiLength, size_t* pMidLength);
	size_t iXmiLength = testsize2;
	size_t pMidLength;
	uint8_t* outmidi = TranscodeXmiToMid((const uint8_t*)*(uint32_t*)(acttrack + 18), iXmiLength, &pMidLength);
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

void SOUND_start_sequence(int32_t sequence_num) {
	if (unitTests)return;
	//3 - menu
	//4 - intro
#ifdef SOUND_SDLMIXER

	if (oggmusic && !GAME_music_war)
	{
		std::string oggmusicPath = GetSubDirectoryPath(oggmusicFolder);
		char selectedTrackPath[512] = "";

		sprintf(selectedTrackPath, "%s/00%d*danger.ogg", oggmusicPath.c_str(), sequence_num);
		_finddata_t musicFile;
		auto hFile = my_findfirst(selectedTrackPath, &musicFile);

		if (hFile != -1)
		{
			sprintf(selectedTrackPath, "%s/%s", oggmusicPath.c_str(), musicFile.name);
			GAME_music_war = Mix_LoadWAV(selectedTrackPath);
			warMusicOn = true;
		}
		else
			warMusicOn = false;
	}

	last_sequence_num = sequence_num;
	//volume fix
	if (lastMusicVolume == -1)
	{
		SOUND_set_sequence_volume(0x64, 0);
	}
	if (lastMusicVolume != settingsMusicVolume)
	{
		SOUND_set_sequence_volume(settingsMusicVolume, 0);
	}
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
	if(warMusicOn)
	{
		if (Mix_Playing(music_war_channel_index) == 0) {
			Mix_VolumeChunk(GAME_music_war, 0);
			Mix_Volume(music_war_channel_index, 0);
			if (Mix_PlayChannel(music_war_channel_index, GAME_music_war, -1) == -1) {
				if (Mix_Paused(music_war_channel_index) == 1) {
					Mix_Resume(music_war_channel_index);
				}
				else {
					Mix_Pause(music_war_channel_index);
				}
			}
		}
	}
#endif//SOUND_SDLMIXER
};

void WarMusicSetVolume(int32_t volume) {
#ifdef SOUND_SDLMIXER
	Mix_Volume(music_war_channel_index, ((volume * settingsMusicVolume) / 127));
	Mix_VolumeChunk(GAME_music_war, 128);
#endif//SOUND_SDLMIXER
}

void SOUND_pause_sequence(int32_t  /*sequence_num*/) {
	if (unitTests)return;
#ifdef SOUND_SDLMIXER
	Mix_PauseMusic();
	if (warMusicOn) {
		Mix_Pause(music_war_channel_index);
	}
#endif//SOUND_SDLMIXER
};

void SOUND_stop_sequence(int32_t  /*sequence_num*/) {
	if (unitTests)return;
#ifdef SOUND_SDLMIXER
	Mix_HaltMusic();
	if (warMusicOn) {
		Mix_HaltChannel(music_war_channel_index);
	}
#endif//SOUND_SDLMIXER
};
void SOUND_resume_sequence(int32_t  /*sequence_num*/) {
	if (unitTests)return;
#ifdef SOUND_SDLMIXER
	Mix_ResumeMusic();
	if (warMusicOn) {
		Mix_Resume(music_war_channel_index);
	}
#endif//SOUND_SDLMIXER
};

void SOUND_set_sequence_volume(int32_t volume, int32_t  milliseconds) {
	if (unitTests)return;
#ifdef SOUND_SDLMIXER
#ifndef __linux__
	if ((milliseconds > 0) && (volume == 0))
	{
		if (GAME_music[last_sequence_num])
		{
			double position = Mix_GetMusicPosition(GAME_music[last_sequence_num]);
			if (position != 0)
			{
				Mix_FadeOutMusic(milliseconds);
				Mix_SetMusicPosition(position);
			}
		}
	}
	else if ((milliseconds > 0) && (lastMusicVolume == 0))
	{
		if (GAME_music[last_sequence_num])
		{
			double position = Mix_GetMusicPosition(GAME_music[last_sequence_num]);
			if (position != 0)
			{
				Mix_FadeInMusicPos(GAME_music[last_sequence_num], 1, milliseconds, position);
			}
		}
	}
	else
#endif //__linux__
		Mix_VolumeMusic(volume);
	lastMusicVolume = volume;
	if (milliseconds == 0)
		settingsMusicVolume = volume;
#endif//SOUND_SDLMIXER
}

void SOUND_init_MIDI_sequence(uint8_t*  /*datax*/, type_E3808_music_header* headerx, int32_t track_number)
{
	if (unitTests)return;
	//uint8_t* acttrack = &header[32 + track_number * 32];
	uint8_t* acttrack = headerx->str_8.track_10[track_number].xmiData_0;
	//int testsize = *(uint32_t*)(&header[32 + (track_number + 1) * 32] + 18) - *(uint32_t*)(acttrack + 18);
	int testsize2 = *(uint32_t*)(acttrack + 26);

	//we can translate datax from xmi to mid and play(with bad quality or slow midi emulators), at now but we use ogg samples
	//unsigned char* TranscodeXmiToMid(const unsigned char* pXmiData,	size_t iXmiLength, size_t* pMidLength);
	size_t iXmiLength = testsize2;
	size_t pMidLength;
	dirsstruct helpdirsstruct;

	if (oggmusic) {

		std::string oggmusicPath = GetSubDirectoryPath(oggmusicFolder);
		char alternativeMusicPath[512] = "";
		char selectedTrackPath[512] = "";
		//if (track_number > 1)track_number = 0;
		if (oggmusicalternative)///&&track_number==4
		{
			if (track_number == 0)
			{
				sprintf(alternativeMusicPath, "%s/alternative/day", oggmusicPath.c_str());
			}
			else if (track_number == 1)
			{
				sprintf(alternativeMusicPath, "%s/alternative/night", oggmusicPath.c_str());
			}
			else if (track_number == 2)
			{
				sprintf(alternativeMusicPath, "%s/alternative/cave", oggmusicPath.c_str());
			}
			else if (track_number == 3)
			{
				sprintf(alternativeMusicPath, "%s/alternative/cave", oggmusicPath.c_str());
			}
			else if (track_number == 4)
			{
				sprintf(alternativeMusicPath, "%s/alternative/cave", oggmusicPath.c_str());
			}
			else if (track_number == 5)
			{
				sprintf(alternativeMusicPath, "%s/alternative/cave", oggmusicPath.c_str());
			}
			else
			{
				sprintf(alternativeMusicPath, "%s/alternative/cave", oggmusicPath.c_str());
			}

			helpdirsstruct = getListDir(alternativeMusicPath);

			if (helpdirsstruct.number > 0)
			{
				int randtrack = rand()%(helpdirsstruct.number + 1);
				if(randtrack==0)
					sprintf(selectedTrackPath, "%s/music%d.ogg", oggmusicPath.c_str(), track_number);
				else
					sprintf(selectedTrackPath, "%s/%s", alternativeMusicPath ,helpdirsstruct.dir[randtrack-1]);
			}
			else
				sprintf(selectedTrackPath, "%s/music%d.ogg", oggmusicPath.c_str(), track_number);
		}
		else
		{
			sprintf(selectedTrackPath, "%s/00%d*.ogg", oggmusicPath.c_str(), track_number);
			_finddata_t musicFile;
			auto hFile = my_findfirst(selectedTrackPath, &musicFile);
			if (hFile < 0)
			{
				Logger->error("Track NOT found error: {}", Mix_GetError());
				return;
			}
			sprintf(selectedTrackPath, "%s/%s", oggmusicPath.c_str(), musicFile.name);
		}
#ifdef SOUND_SDLMIXER
		GAME_music[track_number] = Mix_LoadMUS(selectedTrackPath);
		if (!GAME_music[track_number]) {
			Logger->error("Mix_LoadMUS() error: {}", Mix_GetError());
		}
#endif//SOUND_SDLMIXER
	}
	else
	{
		uint8_t* outmidi = TranscodeXmiToMid(/*(const uint8_t*)*(uint32_t*)(*/acttrack/* + 18)*/, iXmiLength, &pMidLength);
		SDL_RWops* rwmidi = SDL_RWFromMem(outmidi, pMidLength);

		//Timidity_Init();
#ifdef SOUND_SDLMIXER
		GAME_music[track_number] = Mix_LoadMUSType_RW(rwmidi, MUS_MID, SDL_TRUE);
		if (!GAME_music[track_number]) {
			Logger->error("Mix_LoadMUSType_RW() error: {}", Mix_GetError());
		}
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
	if (unitTests)return;
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
	DeleteWarMusic();

#endif//SOUND_SDLMIXER
}

void DeleteWarMusic()
{
	if (GAME_music_war)
	{
		Mix_FreeChunk(GAME_music_war);
		GAME_music_war = nullptr;
	}
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
void playmusic2(int32_t track_number)
{
	if (unitTests)return;
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

int32_t ac_sound_call_driver(AIL_DRIVER* drvr, int32_t fn, VDI_CALL* out) {
	switch (fn) {
	case 0x300: {//AIL_API_install_driver
		drvr->VHDR_4->VDI_HDR_var10 = (void*)&common_IO_configurations;
		drvr->VHDR_4->num_IO_configurations_14 = num_IO_configurations;
		drvr->VHDR_4->environment_string_16 = environment_string.a;
		drvr->VHDR_4->VDI_HDR_var46 = service_rate;
		break;
	}
	case 0x301: {//AIL_API_install_DIG_driver_file/AIL_API_install_MDI_driver_file		
		out->AX = 1;//offset
		out->BX = 2;//offset
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
		/*		mychunk.abuf=(uint8_t*)last_sample->start_2_3[0];
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
	return 1;
};

void SOUND_set_master_volume(int32_t volume) {
	//gamechunk[S->channel].volume = volume;
#ifdef SOUND_SDLMIXER
	master_volume = volume;

	for (int i = 0; i < maxSimultaniousSounds; i++)
	{
		Mix_Volume(i, (int)((GameChunks[i].volume * master_volume) / 127));
	}
#endif//SOUND_SDLMIXER

	//may be can fix - must analyze

}

void SOUND_set_sample_volume(HSAMPLE S, int32_t volume) {
#ifdef SOUND_SDLMIXER
	if (master_volume == -1)
		master_volume = 127;

	GameChunks[S->channel].volume = volume;
	Mix_Volume(S->channel, (int)((GameChunks[S->channel].volume * master_volume) / 127));
#endif//SOUND_SDLMIXER
}

void SOUND_set_sample_volume_panning(HSAMPLE S, int32_t panning) {
	
	auto left = uint8_t(255.0f * ((float)(127 - panning) / 127));// * masterPercent);
	auto right = uint8_t(255.0f * ((float)panning / 127));// * masterPercent);

	Mix_SetPanning(S->channel, left, right);
}

void SetSamplePosition(HSAMPLE S, int16_t angle, uint8_t distance) 
{
	Mix_SetPosition(S->channel, angle, distance);
}

void SOUND_start_sample(HSAMPLE S) {
	if (unitTests)return;
#ifdef SOUND_SDLMIXER

	if (hqsound)
	{
		/*
		// load sample.wav in to sample
		uint8_t* presample = malloc(S->len_4_5[0] * 4 + 10);
		Mix_Chunk* sample;
		sample = Mix_LoadWAV_RW(presample, 0);
		if (!sample) {
			printf("Mix_LoadWAV_RW: %s\n", Mix_GetError());
			// handle error
		}
		*/

		//44100, AUDIO_S16, 2, 4096
		//src/dst
		/*SDL_AudioCVT cvt;
		SDL_BuildAudioCVT(&cvt, AUDIO_U8, 1, S->playback_rate_15, AUDIO_S16, 2, 44100);
		SDL_assert(cvt.needed); // obviously, this one is always needed.
		cvt.len = S->len_4_5[0];// 1024 * 2 * 4;  // 1024 stereo float32 sample frames.
		cvt.buf = (Uint8*)S->start_2_3[0];//(Uint8*)SDL_malloc(cvt.len * cvt.len_mult);
		// read your float32 data into cvt.buf here.
		SDL_ConvertAudio(&cvt);*/

		GameChunks[S->channel].abuf = /*sample->abuf;//*/ (uint8_t*)S->start_44mhz;
		if (fixspeedsound)
			GameChunks[S->channel].alen = /*sample->alen;//*/S->len_4_5[0] * 16;
		else
			GameChunks[S->channel].alen = /*sample->alen;//*/S->len_4_5[0] * 8;
			if (debug_first_sound) {
				Logger->trace("SOUND_start_sample-hq:{}", S->start_44mhz);
				debug_first_sound = false;
			}
	}
	else
	{
		if (debug_first_sound) {
			Logger->trace("SOUND_start_sample:{}", S->start_44mhz);
			debug_first_sound = false;
		}
		GameChunks[S->channel].abuf = (uint8_t*)S->start_2_3[0];
		GameChunks[S->channel].alen = S->len_4_5[0];
	}
	
	GameChunks[S->channel].volume = S->volume_16;
	GameChunkHSamples[S->channel] = S;

	float percentage = (float)S->playback_rate_15 / (float)22050;
	if (percentage != 1.0f)
		SOUND_ChangeSamplePlaybackRate(S, percentage);

	Mix_PlayChannel(S->channel, &GameChunks[S->channel], S->loop_count_12);
	Mix_ChannelFinished(ChannelFinished);

#endif//SOUND_SDLMIXER
#ifdef SOUND_OPENAL
	//sound_load_wav((char*)S->start_44mhz, sizeof(S->start_44mhz));
	if (hqsound)
	{
		gamechunk[S->channel].abuf = (uint8_t*)S->start_44mhz;
		gamechunk[S->channel].alen = S->len_4_5[0] * 4;
	}
	else
	{
		gamechunk[S->channel].abuf = (uint8_t*)S->start_2_3[0];
		gamechunk[S->channel].alen = S->len_4_5[0];
	}

	gamechunk[S->channel].volume = S->volume_16;
	gamechunkHSAMPLE[S->channel] = S;
	ALSOUND_play(S->channel,&gamechunk[S->channel],0);
#endif//SOUND_OPENAL
};

void ChannelFinished(int channel)
{
	if (GameChunkHSamples.count(channel) > 0)
	{
		GameChunkHSamples[channel]->status_1 = 2;
	}

	if (channel == maxSimultaniousSounds && !IsCdTrackPlaying())
	{
		ClearCdTrackSegment();
	}
}

uint32_t SOUND_sample_status(HSAMPLE S) {
	if (unitTests)return 0;
	
#ifdef SOUND_SDLMIXER
	if (Mix_Playing(S->channel)==0) return 2;
#endif//SOUND_SDLMIXER
#ifdef SOUND_OPENAL
	return 2;
#endif//SOUND_SDLMIXER
	return 0;
}

void SOUND_RegisterTimer(int timerIdx, uint32_t(*callback)(uint32_t))
{
	auto timer = Mix_Timer();
	timer.Id = timerIdx;
	timer.Callback = (SDL_TimerCallback)callback;
	Timers.push_back(timer);
}

void SOUND_SetTimerPeriod(int timerIdx, uint32_t intervalMs)
{
	for (int i = 0; i < Timers.size(); i++)
	{
		if (Timers[i].Id == timerIdx)
		{
			Timers[i].IntervalMs = intervalMs;
			break;
		}
	}
}

void SOUND_StartTimer(int timerIdx)
{
	for (int i = 0; i < Timers.size(); i++)
	{
		if (Timers[i].Id == timerIdx)
		{
			Timers[i].SdlId = SDL_AddTimer(Timers[i].IntervalMs, Timers[i].Callback, nullptr);
			break;
		}
	}
}

void SOUND_StopTimer(int timerIdx)
{
	int idxToDelete = -1;
	for (int i = 0; i < Timers.size(); i++)
	{
		if (Timers[i].Id == timerIdx)
		{
			SDL_RemoveTimer(Timers[i].SdlId);
			idxToDelete = i;
			break;
		}
	}
	if (idxToDelete > -1)
		Timers.erase(Timers.begin() + idxToDelete);
}

void SOUND_end_sample(HSAMPLE S) {
#ifdef SOUND_SDLMIXER
	Mix_HaltChannel(S->channel);

	if (GameChunks.count(S->channel) == 0)
		return;

	GameChunks.erase(S->channel);
	GameChunkHSamples.erase(S->channel);

#endif//SOUND_SDLMIXER
};

void SOUND_finalize(int channel) {

	if (GameChunks.count(channel) == 0)
		return;

	HSAMPLE S = GameChunkHSamples[channel];
	if (S)
	{
		S->channel = -1;

		if (S->status_1 != 1)
		{
			if (S->status_1 != 2)
			{
				S->status_1 = 2;
			}
		}
		S->vol_scale_18[0][0] = 0;
		S->id_9 = 0;
		S->flags_14 = 0;
		S->vol_scale_18[0][2] = 0;
		S->vol_scale_18[0][3] = 0;
	}
}

int run();

bool init_sound()
{
	ActiveAudioEffects.resize(maxSimultaniousSounds);

	std::function<void(GameState)> callback = SOUND_GameStateChange;
	EventDispatcher::I->RegisterEvent(new Event<GameState>(EventType::E_GAME_STATE_CHANGE, callback));

	//run();
	//#define MUSIC_MID_FLUIDSYNTH
	//Initialize SDL_mixer
	srand(time(NULL));
#ifdef SOUND_SDLMIXER
	if (hqsound) {
		if (Mix_OpenAudio(44100, AUDIO_S16, 2, 4096) == -1)//4096
		//if (Mix_OpenAudio(44100, AUDIO_S16, 2, 4096) == -1)//4096
		//if (Mix_OpenAudio(11025, AUDIO_S8, 1, 4096) == -1)//4096
		{
			return false;
		}
	}
	else
	{
		if (Mix_OpenAudio(22050, AUDIO_U8/*MIX_DEFAULT_FORMAT*/, 2, 4096) == -1)//4096
		//if (Mix_OpenAudio(11025/*22050*/, AUDIO_U8/*MIX_DEFAULT_FORMAT*/, 1, 4096) == -1)//4096
		{
			return false;
		}
	}

	Mix_AllocateChannels(maxSimultaniousSounds + 1 + 1);

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

void SOUND_GameStateChange(const GameState gameState)
{
	if (gameState == GameState::GAMEPLAY_ENDED)
	{
		warMusicOn = false;
		DeleteWarMusic();
	}
}

void SOUND_ChangeSamplePlaybackRate(HSAMPLE S, float percent)
{
	if (hqsound)
		RegisterEffect(S->channel, &GameChunks[S->channel], percent, 44100, 2, AUDIO_S16);
	else
		RegisterEffect(S->channel, &GameChunks[S->channel], percent, 22050, 2, AUDIO_U8);
}

void RegisterEffect(int channel, const Mix_Chunk* chunk, float speed, int frequency, int channels, uint16_t format)
{
	Logger->debug("Attempting to register effect on channel {}", channel);
	if (ActiveAudioEffects[channel].effect != nullptr) { return; }

	switch (format)
	{
		case AUDIO_U16: LoadAudioEffect<uint16_t>(channel, chunk, speed, frequency, channels, format); break;
		case AUDIO_S16: LoadAudioEffect<int16_t>(channel, chunk, speed, frequency, channels, format); break;
		case AUDIO_S32: LoadAudioEffect<int32_t>(channel, chunk, speed, frequency, channels, format); break;
		case AUDIO_F32: LoadAudioEffect<float>(channel, chunk, speed, frequency, channels, format); break;
	}
}

template <typename T>
void LoadAudioEffect(int channel, const Mix_Chunk* chunk, float speed, int frequency, int channels, uint16_t format)
{
	ActiveAudioEffects[channel].effect = new SfxEffectWrapper<T>(chunk, speed, frequency, channels, format);
	Mix_RegisterEffect(channel, SfxEffectWrapper<T>::EffectModifierCallback, SfxEffectWrapper<T>::EffectDoneCallback, nullptr);
}

bool PlayCdTrackSegment(uint8_t trackIdx, int32_t startPosMs, int32_t lengthMs)
{
	try
	{
		double startPosSec = startPosMs / 1000;
		m_ptrSpeechBytesOffSet = (44100 * startPosSec * 16 * 2) / 8;
		char speechPath[512];
		sprintf(speechPath, "%s/TRACK%02d.WAV", GetSubDirectoryPath(speechFolder).c_str(), trackIdx);
		m_ptrSpeechChunk = Mix_LoadWAV(speechPath);
		m_ptrSpeechChunk->volume = (uint8_t)master_volume;
		m_ptrSpeechChunk->abuf = m_ptrSpeechChunk->abuf + m_ptrSpeechBytesOffSet;
		m_ptrSpeechChunk->alen = m_ptrSpeechChunk->alen - m_ptrSpeechBytesOffSet;
		Mix_HaltChannel(maxSimultaniousSounds);
		if (Mix_PlayChannelTimed(maxSimultaniousSounds, m_ptrSpeechChunk, 0, lengthMs) < 0)
		{
			fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
			return false;
		}

		return true;
	}
	catch (std::exception ex)
	{
		return false;
	}
}

bool IsCdTrackPlaying()
{
	return Mix_Playing(maxSimultaniousSounds) == 1;
}

bool EndPlayingCdTrackSegment()
{
	if (IsCdTrackPlaying())
	{
		auto success = Mix_HaltChannel(maxSimultaniousSounds) == 0;
		if (success)
			success = ClearCdTrackSegment();

		return success;
	}
	return true;
}

bool ClearCdTrackSegment()
{
	if (m_ptrSpeechChunk != nullptr)
	{
		m_ptrSpeechChunk->abuf = m_ptrSpeechChunk->abuf - m_ptrSpeechBytesOffSet;
		m_ptrSpeechChunk->alen = m_ptrSpeechChunk->alen + m_ptrSpeechBytesOffSet;
		Mix_FreeChunk(m_ptrSpeechChunk);
		m_ptrSpeechChunk = nullptr;
		return true;
	}
	return true;
}

bool AreCdTracksAvailable()
{
	return GetCdTrackCount() > 0;
}

int GetCdTrackCount()
{
	char speechPath[512];
	int count = 0;
	try
	{
		for (int i = 1; i < 1000; i++)
		{
			sprintf(speechPath, "%s/TRACK%02d.WAV", GetSubDirectoryPath(speechFolder).c_str(), i);

			if (FILE* file = fopen(speechPath, "r")) {
				fclose(file);
				count++;
			}
			else
			{
				break;
			}
		}
	}
	catch (std::exception ex)
	{
		return count;
	}
	return count;
}

AIL_DRIVER* ac_AIL_API_install_driver(int  /*a1*/, uint8_t*  /*a2*/, int  /*a3*/)/*driver_image,n_bytes*///27f720
{
	//printf("drvr:%08X, fn:%08X, in:%08X, out:%08X\n", drvr, fn, in, out);
	return 0;
}

uint16_t actvect[4096];

void ac_set_real_vect(uint32_t vectnum, uint16_t real_ptr)
{
	actvect[vectnum] = real_ptr;
	//66
};

uint16_t ac_get_real_vect(uint32_t vectnum)
{
	return actvect[vectnum];
};

void test_music()
{
}

void my_audio_callback(void *midi_player, uint8_t *stream, int len);

/* variable declarations */
static uint32_t is_playing = 0; /* remaining length of the sample we have to play */
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
	if (Mix_OpenAudio(spec.freq, spec.format, spec.channels, spec.samples) < 0)
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
		Mix_CloseAudio();
		adl_close(midi_player);
		return 1;
	}

	is_playing = 1;
	/* Start playing */
#if SDL_MIXER_VERSION_ATLEAST(2, 8, 0)
	Mix_PauseAudio(0);
#else
	// Mix_PauseAudio not in SDL2_mixer < 2.8.0
	Mix_Resume(-1);
	Mix_ResumeMusic();
#endif

	Logger->info("Playing... Hit Ctrl+C to quit!");

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
void my_audio_callback(void *midi_player, uint8_t *stream, int len)
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
	SDL_memcpy(stream, (uint8_t*)buffer, samples_count * 2);
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
	//Once you�ve finished don�t forget to clean memoryand release OpenAL contextand device

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
