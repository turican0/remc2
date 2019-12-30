/*
  SDL_mixer:  An audio mixer library based on the SDL library
  Copyright (C) 1997-2018 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

/* This file supports libADLMIDI music streams */

#include "music_midi_adl.h"

#ifdef MUSIC_MID_ADLMIDI
#include <adlmidi.h>
#include <stdio.h>

/* Global ADLMIDI flags which are applying on initializing of MIDI player with a file */
typedef struct {
    int bank;
    int tremolo;
    int vibrato;
    int scalemod;
    int adlibdrums;
    int log_volumes;
    int volume_model;
    int chips_count;
    int four_op_channels;
    int full_brightness_range;
    int emulator;
    char custom_bank_path[2048];
} AdlMidi_Setup;

static AdlMidi_Setup adlmidi_setup = {
    58, -1, -1, -1, -1, 0, 0, 4, -1, 0, -1, ""
};

static void ADLMIDI_SetDefault(AdlMidi_Setup *setup)
{
    setup->bank        = 58;
    setup->tremolo     = -1;
    setup->vibrato     = -1;
    setup->scalemod    = -1;
    setup->adlibdrums  = -1;
    setup->log_volumes  = 0;
    setup->volume_model = 0;
    setup->chips_count = 4;
    setup->four_op_channels = -1;
    setup->full_brightness_range = 0;
    setup->emulator = -1;
    setup->custom_bank_path[0] = '\0';
}
#endif /* MUSIC_MID_ADLMIDI */

int SDLCALLCC Mix_ADLMIDI_getTotalBanks()
{
    #ifdef MUSIC_MID_ADLMIDI
    return adl_getBanksCount();
    #else
    return 0;
    #endif
}

const char *const * SDLCALLCC Mix_ADLMIDI_getBankNames()
{
    #ifdef MUSIC_MID_ADLMIDI
    return adl_getBankNames();
    #else
    return NULL;
    #endif
}

int SDLCALLCC Mix_ADLMIDI_getBankID()
{
    #ifdef MUSIC_MID_ADLMIDI
    return adlmidi_setup.bank;
    #else
    return 0;
    #endif
}

void SDLCALLCC Mix_ADLMIDI_setBankID(int bnk)
{
    #ifdef MUSIC_MID_ADLMIDI
    adlmidi_setup.bank = bnk;
    #else
    MIX_UNUSED(bnk);
    #endif
}

int SDLCALLCC Mix_ADLMIDI_getTremolo()
{
    return adlmidi_setup.tremolo;
}
void SDLCALLCC Mix_ADLMIDI_setTremolo(int tr)
{
    #ifdef MUSIC_MID_ADLMIDI
    adlmidi_setup.tremolo = tr;
    #else
    MIX_UNUSED(tr);
    #endif
}

int SDLCALLCC Mix_ADLMIDI_getVibrato()
{
    #ifdef MUSIC_MID_ADLMIDI
    return adlmidi_setup.vibrato;
    #else
    return -1;
    #endif
}

void SDLCALLCC Mix_ADLMIDI_setVibrato(int vib)
{
    #ifdef MUSIC_MID_ADLMIDI
    adlmidi_setup.vibrato = vib;
    #else
    MIX_UNUSED(vib);
    #endif
}


int SDLCALLCC Mix_ADLMIDI_getAdLibMode()
{
    #ifdef MUSIC_MID_ADLMIDI
    return adlmidi_setup.adlibdrums;
    #else
    return -1;
    #endif
}

void SDLCALLCC Mix_ADLMIDI_setAdLibMode(int ald)
{
    #ifdef MUSIC_MID_ADLMIDI
    adlmidi_setup.adlibdrums = ald;
    #else
    MIX_UNUSED(ald);
    #endif
}

int SDLCALLCC Mix_ADLMIDI_getScaleMod()
{
    #ifdef MUSIC_MID_ADLMIDI
    return adlmidi_setup.scalemod;
    #else
    return -1;
    #endif
}

void SDLCALLCC Mix_ADLMIDI_setScaleMod(int sc)
{
    #ifdef MUSIC_MID_ADLMIDI
    adlmidi_setup.scalemod = sc;
    #else
    MIX_UNUSED(sc);
    #endif
}

int SDLCALLCC Mix_ADLMIDI_getLogarithmicVolumes()
{
    #ifdef MUSIC_MID_ADLMIDI
    return adlmidi_setup.log_volumes;
    #else
    return -1;
    #endif
}

void SDLCALLCC Mix_ADLMIDI_setLogarithmicVolumes(int vm)
{
    #ifdef MUSIC_MID_ADLMIDI
    adlmidi_setup.log_volumes = vm;
    #else
    MIX_UNUSED(vm);
    #endif
}

int SDLCALLCC Mix_ADLMIDI_getVolumeModel()
{
    #ifdef MUSIC_MID_ADLMIDI
    return adlmidi_setup.volume_model;
    #else
    return -1;
    #endif
}

void SDLCALLCC Mix_ADLMIDI_setVolumeModel(int vm)
{
    #ifdef MUSIC_MID_ADLMIDI
    adlmidi_setup.volume_model = vm;
    if(vm < 0)
        adlmidi_setup.volume_model = 0;
    #else
    MIX_UNUSED(vm);
    #endif
}

int SDLCALLCC Mix_ADLMIDI_getFullRangeBrightness()
{
    #ifdef MUSIC_MID_ADLMIDI
    return adlmidi_setup.full_brightness_range;
    #else
    return -1;
    #endif
}

void SDLCALLCC Mix_ADLMIDI_setFullRangeBrightness(int frb)
{
    #ifdef MUSIC_MID_ADLMIDI
    adlmidi_setup.full_brightness_range = frb;
    #else
    MIX_UNUSED(frb);
    #endif
}

int SDLCALLCC Mix_ADLMIDI_getEmulator()
{
    #ifdef MUSIC_MID_ADLMIDI
    return adlmidi_setup.emulator;
    #else
    return -1;
    #endif
}

void SDLCALLCC Mix_ADLMIDI_setEmulator(int emu)
{
    #ifdef MUSIC_MID_ADLMIDI
    adlmidi_setup.emulator = emu;
    #else
    MIX_UNUSED(emu);
    #endif
}

void SDLCALLCC Mix_ADLMIDI_setSetDefaults()
{
    #ifdef MUSIC_MID_ADLMIDI
    ADLMIDI_SetDefault(&adlmidi_setup);
    #endif
}

void SDLCALLCC Mix_ADLMIDI_setCustomBankFile(const char *bank_wonl_path)
{
    #ifdef MUSIC_MID_ADLMIDI
    if (bank_wonl_path)
        SDL_strlcpy(adlmidi_setup.custom_bank_path, bank_wonl_path, 2048);
    else
        adlmidi_setup.custom_bank_path[0] = '\0';
    #else
    MIX_UNUSED(bank_wonl_path);
    #endif
}

#ifdef MUSIC_MID_ADLMIDI

typedef struct
{
    int play_count;
    struct ADL_MIDIPlayer *adlmidi;
    int volume;

    SDL_AudioStream *stream;
    void *buffer;
    size_t buffer_size;
    Mix_MusicMetaTags tags;
    struct ADLMIDI_AudioFormat sample_format;
} AdlMIDI_Music;

/* Set the volume for a ADLMIDI stream */
static void ADLMIDI_setvolume(void *music_p, int volume)
{
    AdlMIDI_Music *music = (AdlMIDI_Music *)music_p;
    music->volume = (int)(round(128.0 * sqrt(((double)volume) * (1.0 / 128.0))));
}

static void process_args(const char *args, AdlMidi_Setup *setup)
{
    char arg[1024];
    char type = 'x';
    size_t maxlen = 0;
    size_t i, j = 0;
    int value_opened = 0;
    if (args == NULL) {
        return;
    }
    maxlen = SDL_strlen(args);
    if (maxlen == 0) {
        return;
    }

    maxlen += 1;
    ADLMIDI_SetDefault(setup);

    for (i = 0; i < maxlen; i++) {
        char c = args[i];
        if(value_opened == 1) {
            if ((c == ';') || (c == '\0')) {
                int value;
                arg[j] = '\0';
                value = atoi(arg);
                switch(type)
                {
                case 'c':
                    setup->chips_count = value;
                    break;
                case 'f':
                    setup->four_op_channels = value;
                    break;
                case 'b':
                    setup->bank = value;
                    break;
                case 't':
                    setup->tremolo = value;
                    break;
                case 'v':
                    setup->vibrato = value;
                    break;
                case 'a':
                    setup->adlibdrums = value;
                    break;
                case 'm':
                    setup->scalemod = value;
                    break;
                case 'l':
                    setup->volume_model = value;
                    break;
                case 'r':
                    setup->full_brightness_range = value;
                    break;
                case 'e':
                    setup->emulator = value;
                    break;
                case '\0':
                    break;
                default:
                    break;
                }
                value_opened = 0;
            }
            arg[j++] = c;
        } else {
            if (c == '\0') {
                return;
            }
            type = c;
            value_opened = 1;
            j = 0;
        }
    }
}

static void ADLMIDI_delete(void *music_p);

static AdlMIDI_Music *ADLMIDI_LoadSongRW(SDL_RWops *src, const char *args)
{
    if(src != NULL)
    {
        void *bytes = 0;
        long filesize = 0;
        int err = 0;
        Sint64 length = 0;
        size_t bytes_l;
        unsigned char byte[1];
        AdlMIDI_Music *music = NULL;
        AdlMidi_Setup setup = adlmidi_setup;
        unsigned short src_format = music_spec.format;

        process_args(args, &setup);

        music = (AdlMIDI_Music *)SDL_calloc(1, sizeof(AdlMIDI_Music));

        switch (music_spec.format)
        {
        case AUDIO_U8:
            music->sample_format.type = ADLMIDI_SampleType_U8;
            music->sample_format.containerSize = sizeof(Uint8);
            music->sample_format.sampleOffset = sizeof(Uint8) * 2;
            break;
        case AUDIO_S8:
            music->sample_format.type = ADLMIDI_SampleType_S8;
            music->sample_format.containerSize = sizeof(Sint8);
            music->sample_format.sampleOffset = sizeof(Sint8) * 2;
            break;
        case AUDIO_S16:
            music->sample_format.type = ADLMIDI_SampleType_S16;
            music->sample_format.containerSize = sizeof(Sint16);
            music->sample_format.sampleOffset = sizeof(Sint16) * 2;
            break;
        case AUDIO_U16:
            music->sample_format.type = ADLMIDI_SampleType_U16;
            music->sample_format.containerSize = sizeof(Uint16);
            music->sample_format.sampleOffset = sizeof(Uint16) * 2;
            break;
        case AUDIO_S32:
            music->sample_format.type = ADLMIDI_SampleType_S32;
            music->sample_format.containerSize = sizeof(Sint32);
            music->sample_format.sampleOffset = sizeof(Sint32) * 2;
            break;
        case AUDIO_F32:
        default:
            music->sample_format.type = ADLMIDI_SampleType_F32;
            music->sample_format.containerSize = sizeof(float);
            music->sample_format.sampleOffset = sizeof(float) * 2;
            src_format = AUDIO_F32;
        }

        music->stream = SDL_NewAudioStream(src_format, 2, music_spec.freq,
                                           music_spec.format, music_spec.channels, music_spec.freq);

        if (!music->stream) {
            ADLMIDI_delete(music);
            return NULL;
        }

        music->buffer_size = music_spec.samples * music->sample_format.containerSize * 2/*channels*/ * music_spec.channels;
        music->buffer = SDL_malloc(music->buffer_size);
        if (!music->buffer) {
            ADLMIDI_delete(music);
            return NULL;
        }

        length = SDL_RWseek(src, 0, RW_SEEK_END);
        if(length < 0)
        {
            Mix_SetError("ADL-MIDI: wrong file\n");
            return NULL;
        }

        SDL_RWseek(src, 0, RW_SEEK_SET);
        bytes = SDL_malloc((size_t)length);

        filesize = 0;
        while((bytes_l = SDL_RWread(src, &byte, sizeof(Uint8), 1)) != 0)
        {
            ((unsigned char *)bytes)[filesize] = byte[0];
            filesize++;
        }

        if(filesize == 0)
        {
            SDL_free(bytes);
            ADLMIDI_delete(music);
            Mix_SetError("ADL-MIDI: wrong file\n");
            return NULL;
        }

        music->adlmidi = adl_init(music_spec.freq);

        adl_setHVibrato(music->adlmidi, setup.vibrato);
        adl_setHTremolo(music->adlmidi, setup.tremolo);
        if(setup.custom_bank_path[0] != '\0')
            err = adl_openBankFile(music->adlmidi, (char*)setup.custom_bank_path);
        else
            err = adl_setBank(music->adlmidi, setup.bank);
        if(err < 0)
        {
            Mix_SetError("ADL-MIDI: %s", adl_errorInfo(music->adlmidi));
            SDL_free(bytes);
            ADLMIDI_delete(music);
            return NULL;
        }

        if (setup.emulator >= 0)
            adl_switchEmulator( music->adlmidi, setup.emulator );
        adl_setScaleModulators(music->adlmidi, setup.scalemod);
        adl_setPercMode(music->adlmidi, setup.adlibdrums);
        adl_setLogarithmicVolumes(music->adlmidi, setup.log_volumes);
        adl_setVolumeRangeModel(music->adlmidi, setup.volume_model);
        adl_setFullRangeBrightness( music->adlmidi, setup.full_brightness_range );
        adl_setNumChips(music->adlmidi, setup.chips_count);
        if (setup.four_op_channels >= 0)
            adl_setNumFourOpsChn(music->adlmidi, setup.four_op_channels);

        err = adl_openData(music->adlmidi, bytes, (unsigned long)filesize);
        SDL_free(bytes);

        if(err != 0)
        {
            Mix_SetError("ADL-MIDI: %s", adl_errorInfo(music->adlmidi));
            ADLMIDI_delete(music);
            return NULL;
        }

        music->volume                 = MIX_MAX_VOLUME;
        meta_tags_init(&music->tags);
        meta_tags_set(&music->tags, MIX_META_TITLE, adl_metaMusicTitle(music->adlmidi));
        meta_tags_set(&music->tags, MIX_META_COPYRIGHT, adl_metaMusicCopyright(music->adlmidi));
        return music;
    }
    return NULL;
}

/* Load ADLMIDI stream from an SDL_RWops object */
static void *ADLMIDI_new_RWex(struct SDL_RWops *src, int freesrc, const char *args)
{
    AdlMIDI_Music *adlmidiMusic;

    adlmidiMusic = ADLMIDI_LoadSongRW(src, args);
    if(!adlmidiMusic)
        return NULL;
    if(freesrc)
        SDL_RWclose(src);

    return adlmidiMusic;
}

static void *ADLMIDI_new_RW(struct SDL_RWops *src, int freesrc)
{
    return ADLMIDI_new_RWex(src, freesrc, NULL);
}


/* Start playback of a given Game Music Emulators stream */
static int ADLMIDI_play(void *music_p, int play_counts)
{
    AdlMIDI_Music *music = (AdlMIDI_Music *)music_p;
    adl_positionRewind(music->adlmidi);
    music->play_count = play_counts;
    adl_setLoopEnabled(music->adlmidi, (play_counts < 0));
    return 0;
}

static int ADLMIDI_playSome(void *context, void *data, int bytes, SDL_bool *done)
{
    AdlMIDI_Music *music = (AdlMIDI_Music *)context;
    int filled, gottenLen, amount;

    filled = SDL_AudioStreamGet(music->stream, data, bytes);
    if (filled != 0) {
        return filled;
    }

    if (!music->play_count) {
        /* All done */
        *done = SDL_TRUE;
        return 0;
    }

    /* Align bytes length to correctly capture a stereo input */
    if ((bytes % (int)music->sample_format.sampleOffset) != 0) {
        bytes += ((int)music->sample_format.sampleOffset - (bytes % (int)music->sample_format.sampleOffset));
    }

    gottenLen = adl_playFormat(music->adlmidi, (bytes / (int)music->sample_format.containerSize),
                               (ADL_UInt8*)music->buffer,
                               (ADL_UInt8*)music->buffer + music->sample_format.containerSize,
                               &music->sample_format);

    if (gottenLen <= 0) {
        *done = SDL_TRUE;
        return 0;
    }

    amount = gottenLen * (int)music->sample_format.containerSize;
    if (amount > 0) {
        if (SDL_AudioStreamPut(music->stream, music->buffer, amount) < 0) {
            return -1;
        }
    } else {
        if (music->play_count == 1) {
            music->play_count = 0;
            SDL_AudioStreamFlush(music->stream);
        } else {
            int play_count = -1;
            if (music->play_count > 0) {
                play_count = (music->play_count - 1);
            }
            adl_positionRewind(music->adlmidi);
            music->play_count = play_count;
        }
    }

    return 0;
}

/* Play some of a stream previously started with ADLMIDI_play() */
static int ADLMIDI_playAudio(void *music_p, void *stream, int len)
{
    AdlMIDI_Music *music = (AdlMIDI_Music *)music_p;
    return music_pcm_getaudio(music_p, stream, len, music->volume, ADLMIDI_playSome);
}

/* Close the given Game Music Emulators stream */
static void ADLMIDI_delete(void *music_p)
{
    AdlMIDI_Music *music = (AdlMIDI_Music *)music_p;
    if(music)
    {
        meta_tags_clear(&music->tags);
        if(music->adlmidi) {
            adl_close(music->adlmidi);
        }
        if (music->stream) {
            SDL_FreeAudioStream(music->stream);
        }
        if (music->buffer) {
            SDL_free(music->buffer);
        }
        SDL_free(music);
    }
}

static const char* ADLMIDI_GetMetaTag(void *context, Mix_MusicMetaTag tag_type)
{
    AdlMIDI_Music *music = (AdlMIDI_Music *)context;
    return meta_tags_get(&music->tags, tag_type);
}

/* Jump (seek) to a given position (time is in seconds) */
static int ADLMIDI_jump_to_time(void *music_p, double time)
{
    AdlMIDI_Music *music = (AdlMIDI_Music *)music_p;
    adl_positionSeek(music->adlmidi, time);
    return 0;
}

static double ADLMIDI_currentPosition(void *music_p)
{
    AdlMIDI_Music *music = (AdlMIDI_Music *)music_p;
    if(music)
        return adl_positionTell(music->adlmidi);
    return -1;
}

static double ADLMIDI_songLength(void *music_p)
{
    AdlMIDI_Music *music = (AdlMIDI_Music *)music_p;
    if(music)
        return adl_totalTimeLength(music->adlmidi);
    return -1;
}

static double ADLMIDI_loopStart(void *music_p)
{
    AdlMIDI_Music *music = (AdlMIDI_Music *)music_p;
    if(music)
        return adl_loopStartTime(music->adlmidi);
    return -1;
}

static double ADLMIDI_loopEnd(void *music_p)
{
    AdlMIDI_Music *music = (AdlMIDI_Music *)music_p;
    if(music)
        return adl_loopEndTime(music->adlmidi);
    return -1;
}

static double ADLMIDI_loopLength(void *music_p)
{
    AdlMIDI_Music *music = (AdlMIDI_Music *)music_p;
    if(music)
    {
        double start = adl_loopStartTime(music->adlmidi);
        double end = adl_loopEndTime(music->adlmidi);
        if(start >= 0 && end >= 0)
            return (end - start);
    }
    return -1;
}


Mix_MusicInterface Mix_MusicInterface_ADLMIDI =
{
    "ADLMIDI",
    MIX_MUSIC_ADLMIDI,
    MUS_MID,
    SDL_FALSE,
    SDL_FALSE,

    NULL,   /* Load */
    NULL,   /* Open */
    ADLMIDI_new_RW,
    ADLMIDI_new_RWex,   /* CreateFromRWex [MIXER-X]*/
    NULL,   /* CreateFromFile */
    NULL,   /* CreateFromFileEx [MIXER-X]*/
    ADLMIDI_setvolume,
    ADLMIDI_play,
    NULL,   /* IsPlaying */
    ADLMIDI_playAudio,
    ADLMIDI_jump_to_time,
    ADLMIDI_currentPosition,   /* Tell [MIXER-X]*/
    ADLMIDI_songLength,   /* FullLength [MIXER-X]*/
    ADLMIDI_loopStart,   /* LoopStart [MIXER-X]*/
    ADLMIDI_loopEnd,   /* LoopEnd [MIXER-X]*/
    ADLMIDI_loopLength,   /* LoopLength [MIXER-X]*/
    ADLMIDI_GetMetaTag,   /* GetMetaTag [MIXER-X]*/
    NULL,   /* Pause */
    NULL,   /* Resume */
    NULL,   /* Stop */
    ADLMIDI_delete,
    NULL,   /* Close */
    NULL,   /* Unload */
};

/* Same as Mix_MusicInterface_ADLMIDI. Created to play special music formats separately from off MIDI interfaces */
Mix_MusicInterface Mix_MusicInterface_ADLIMF =
{
    "ADLIMF",
    MIX_MUSIC_ADLMIDI,
    MUS_ADLMIDI,
    SDL_FALSE,
    SDL_FALSE,

    NULL,   /* Load */
    NULL,   /* Open */
    ADLMIDI_new_RW,
    ADLMIDI_new_RWex,   /* CreateFromRWex [MIXER-X]*/
    NULL,   /* CreateFromFile */
    NULL,   /* CreateFromFileEx [MIXER-X]*/
    ADLMIDI_setvolume,
    ADLMIDI_play,
    NULL,   /* IsPlaying */
    ADLMIDI_playAudio,
    ADLMIDI_jump_to_time,
    ADLMIDI_currentPosition,   /* Tell [MIXER-X]*/
    ADLMIDI_songLength,   /* FullLength [MIXER-X]*/
    ADLMIDI_loopStart,   /* LoopStart [MIXER-X]*/
    ADLMIDI_loopEnd,   /* LoopEnd [MIXER-X]*/
    ADLMIDI_loopLength,   /* LoopLength [MIXER-X]*/
    ADLMIDI_GetMetaTag,   /* GetMetaTag [MIXER-X]*/
    NULL,   /* Pause */
    NULL,   /* Resume */
    NULL,   /* Stop */
    ADLMIDI_delete,
    NULL,   /* Close */
    NULL,   /* Unload */
};

#endif /* MUSIC_MID_ADLMIDI */
