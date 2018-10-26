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
#include <string.h> /* for strtok() and strtok_s() */

#include "SDL_hints.h"
#include "SDL_log.h"
#include "SDL_timer.h"

#include "SDL_mixer_ext.h"
#include "mixer.h"
#include "music.h"

#include "./codecs/music_cmd.h"
#include "./codecs/music_wav.h"
#include "./codecs/music_mikmod.h"
#include "./codecs/music_modplug.h"
#include "./codecs/music_nativemidi.h"
#include "./codecs/music_fluidsynth.h"
#include "./codecs/music_timidity.h"
#include "./codecs/music_ogg.h"
#include "./codecs/music_opus.h"
#include "./codecs/music_mpg123.h"
#include "./codecs/music_mad.h"
#include "./codecs/music_flac.h"
#include "./codecs/native_midi/native_midi.h"
#include "./codecs/music_gme.h"
#include "./codecs/music_midi_adl.h"
#include "./codecs/music_midi_opn.h"

/* Check to make sure we are building with a new enough SDL */
#if SDL_COMPILEDVERSION < SDL_VERSIONNUM(2, 0, 7)
#error You need SDL 2.0.7 or newer from http://www.libsdl.org
#endif

/* Set this hint to true if you want verbose logging of music interfaces */
#define SDL_MIXER_HINT_DEBUG_MUSIC_INTERFACES \
    "SDL_MIXER_DEBUG_MUSIC_INTERFACES"

char *music_cmd = NULL;
static SDL_bool music_active = SDL_TRUE;
static int music_volume = MIX_MAX_VOLUME;
static Mix_Music * volatile music_playing = NULL;
SDL_AudioSpec music_spec;

struct _Mix_Music {
    Mix_MusicInterface *interface;
    void *context;

    SDL_bool playing;
    Mix_Fading fading;
    int fade_step;
    int fade_steps;

    char music_filename[1024];
};

/* Used to calculate fading steps */
static int ms_per_step;

/* rcg06042009 report available decoders at runtime. */
static const char **music_decoders = NULL;
static int num_decoders = 0;

/* Semicolon-separated SoundFont paths */
static char* soundfont_paths = NULL;

/*  ======== MIDI toggler ======== */
/* MIDI device currently in use */
int mididevice_current = MIDI_ANY;
/* Denies MIDI arguments */
static int mididevice_args_lock = 0;
/*  ======== MIDI toggler END ==== */


/*
 * public domain strtok_r() by Charlie Gordon
 *
 *   from comp.lang.c  9/14/2007
 *
 *      http://groups.google.com/group/comp.lang.c/msg/2ab1ecbb86646684
 *
 *     (Declaration that it's public domain):
 *      http://groups.google.com/group/comp.lang.c/msg/7c7b39328fefab9c
 */
char *Mix_strtok_safe(char *str, const char *delim, char **nextp)
{
    char *ret;
    if (str == NULL) {
        str = *nextp;
    }

    str += strspn(str, delim);
    if (*str == '\0') {
        return NULL;
    }
    ret = str;

    str += strcspn(str, delim);
    if (*str) {
        *str++ = '\0';
    }

    *nextp = str;
    return ret;
}

/* Meta-Tags utiltiy */
void meta_tags_init(Mix_MusicMetaTags *tags)
{
    SDL_memset(tags, 0, sizeof(Mix_MusicMetaTags));
}

void meta_tags_clear(Mix_MusicMetaTags *tags)
{
    size_t i = 0;
    for (i = 0; i < MIX_META_LAST; i++) {
        if (tags->tags[i]) {
            SDL_free(tags->tags[i]);
            tags->tags[i] = NULL;
        }
    }
}

void meta_tags_set(Mix_MusicMetaTags *tags, Mix_MusicMetaTag type, const char *value)
{
    char *out;
    size_t len;

    if (!value) {
        return;
    }
    if (type >= MIX_META_LAST) {
        return;
    }

    len = SDL_strlen(value);
    out = (char *)SDL_malloc(sizeof(char) * len + 1);
    SDL_memset(out, 0, len + 1);
    SDL_strlcpy(out, value, len +1);

    if (tags->tags[type]) {
        SDL_free(tags->tags[type]);
    }

    tags->tags[type] = out;
}

const char *meta_tags_get(Mix_MusicMetaTags *tags, Mix_MusicMetaTag type)
{
    switch (type) {
    case MIX_META_TITLE:
    case MIX_META_ARTIST:
    case MIX_META_ALBUM:
    case MIX_META_COPYRIGHT:
        return tags->tags[type] ? tags->tags[type] : "";
    case MIX_META_LAST:
    default:
        break;
    }
    return "";
}


/* Interfaces for the various music interfaces, ordered by priority */
static Mix_MusicInterface *s_music_interfaces[] =
{
#ifdef MUSIC_CMD
    &Mix_MusicInterface_CMD,
#endif
#ifdef MUSIC_WAV
    &Mix_MusicInterface_WAV,
#endif
#ifdef MUSIC_FLAC
    &Mix_MusicInterface_FLAC,
#endif
#ifdef MUSIC_OGG
    &Mix_MusicInterface_OGG,
#endif
#ifdef MUSIC_OPUS
    &Mix_MusicInterface_Opus,
#endif
#ifdef MUSIC_MP3_MPG123
    &Mix_MusicInterface_MPG123,
#endif
#ifdef MUSIC_GME
    &Mix_MusicInterface_GME,
#endif
#ifdef MUSIC_MID_ADLMIDI
    &Mix_MusicInterface_ADLMIDI,
    &Mix_MusicInterface_ADLIMF,
#endif
#ifdef MUSIC_MID_OPNMIDI
    &Mix_MusicInterface_OPNMIDI,
#endif
#ifdef MUSIC_MP3_MAD
    &Mix_MusicInterface_MAD,
#endif
#ifdef MUSIC_MOD_MODPLUG
    &Mix_MusicInterface_MODPLUG,
#endif
#ifdef MUSIC_MOD_MIKMOD
    &Mix_MusicInterface_MIKMOD,
#endif
#ifdef MUSIC_MID_FLUIDSYNTH
    &Mix_MusicInterface_FLUIDSYNTH,
#endif
#ifdef MUSIC_MID_TIMIDITY
    &Mix_MusicInterface_TIMIDITY,
#endif
#ifdef MUSIC_MID_NATIVE
    &Mix_MusicInterface_NATIVEMIDI,
#endif
};

int get_num_music_interfaces(void)
{
    return SDL_arraysize(s_music_interfaces);
}

Mix_MusicInterface *get_music_interface(int index)
{
    return s_music_interfaces[index];
}

int SDLCALLCC Mix_GetNumMusicDecoders(void)
{
    return(num_decoders);
}

const char * SDLCALLCC Mix_GetMusicDecoder(int index)
{
    if ((index < 0) || (index >= num_decoders)) {
        return NULL;
    }
    return(music_decoders[index]);
}

static void add_music_decoder(const char *decoder)
{
    void *ptr;
    int i;

    /* Check to see if we already have this decoder */
    for (i = 0; i < num_decoders; ++i) {
        if (SDL_strcmp(music_decoders[i], decoder) == 0) {
            return;
        }
    }

    ptr = SDL_realloc((void *)music_decoders, ((size_t)num_decoders + 1) * sizeof (const char *));
    if (ptr == NULL) {
        return;  /* oh well, go on without it. */
    }
    music_decoders = (const char **) ptr;
    music_decoders[num_decoders++] = decoder;
}

/* Local low-level functions prototypes */
static void music_internal_initialize_volume(void);
static void music_internal_volume(Mix_Music *music, int volume);
static int  music_internal_play(Mix_Music *music, int play_count, double position);
static int  music_internal_position(Mix_Music *music, double position);
static SDL_bool music_internal_playing(Mix_Music *music);
static void music_internal_halt(Mix_Music *music);


/* Support for hooking when the music has finished */
static void (SDLCALL *music_finished_hook)(void) = NULL;

void SDLCALLCC Mix_HookMusicFinished(void (SDLCALL *music_finished)(void))
{
    Mix_LockAudio();
    music_finished_hook = music_finished;
    Mix_UnlockAudio();
}

/* Convenience function to fill audio and mix at the specified volume
   This is called from many music player's GetAudio callback.
 */
int music_pcm_getaudio(void *context, void *data, int bytes, int volume,
                       int (*GetSome)(void *context, void *data, int bytes, SDL_bool *done))
{
    Uint8 *snd = (Uint8 *)data;
    Uint8 *dst;
    int len = bytes;
    SDL_bool done = SDL_FALSE;

    if (volume == MIX_MAX_VOLUME) {
        dst = snd;
    } else {
        dst = SDL_stack_alloc(Uint8, (size_t)bytes);
    }
    while (len > 0 && !done) {
        int consumed = GetSome(context, dst, len, &done);
        if (consumed < 0) {
            break;
        }

        if (volume == MIX_MAX_VOLUME) {
            dst += consumed;
        } else {
            SDL_MixAudioFormat(snd, dst, music_spec.format, (Uint32)consumed, volume);
            snd += consumed;
        }
        len -= consumed;
    }
    if (volume != MIX_MAX_VOLUME) {
        SDL_stack_free(dst);
    }
    return len;
}

/* Mixing function */
void SDLCALL music_mixer(void *udata, Uint8 *stream, int len)
{
    MIX_UNUSED(udata);
    while (music_playing && music_active && len > 0) {
        /* Handle fading */
        if (music_playing->fading != MIX_NO_FADING) {
            if (music_playing->fade_step++ < music_playing->fade_steps) {
                int volume;
                int fade_step = music_playing->fade_step;
                int fade_steps = music_playing->fade_steps;

                if (music_playing->fading == MIX_FADING_OUT) {
                    volume = (music_volume * (fade_steps-fade_step)) / fade_steps;
                } else { /* Fading in */
                    volume = (music_volume * fade_step) / fade_steps;
                }
                music_internal_volume(music_playing, volume);
            } else {
                if (music_playing->fading == MIX_FADING_OUT) {
                    music_internal_halt(music_playing);
                    if (music_finished_hook) {
                        music_finished_hook();
                    }
                    return;
                }
                music_playing->fading = MIX_NO_FADING;
            }
        }

        if (music_playing->interface->GetAudio) {
            int left = music_playing->interface->GetAudio(music_playing->context, stream, len);
            if (left != 0) {
                /* Either an error or finished playing with data left */
                music_playing->playing = SDL_FALSE;
            }
            if (left > 0) {
                stream += (len - left);
                len = left;
            } else {
                len = 0;
            }
        } else {
            len = 0;
        }

        if (!music_internal_playing(music_playing)) {
            music_internal_halt(music_playing);
            if (music_finished_hook) {
                music_finished_hook();
            }
        }
    }
}

/* Load the music interface libraries for a given music type */
SDL_bool load_music_type(Mix_MusicType type)
{
    size_t i;
    int loaded = 0;
    for (i = 0; i < SDL_arraysize(s_music_interfaces); ++i) {
        Mix_MusicInterface *interface = s_music_interfaces[i];
        if (interface->type != type) {
            continue;
        }
        if (!interface->loaded) {
            char hint[64];
            SDL_snprintf(hint, sizeof(hint), "SDL_MIXER_DISABLE_%s", interface->tag);
            if (SDL_GetHintBoolean(hint, SDL_FALSE)) {
                continue;
            }

            if (interface->Load && interface->Load() < 0) {
                if (SDL_GetHintBoolean(SDL_MIXER_HINT_DEBUG_MUSIC_INTERFACES, SDL_FALSE)) {
                    SDL_Log("Couldn't load %s: %s\n", interface->tag, Mix_GetError());
                }
                continue;
            }
            interface->loaded = SDL_TRUE;
        }
        ++loaded;
    }
    return (loaded > 0) ? SDL_TRUE : SDL_FALSE;
}

Mix_MusicAPI get_current_midi_api(int *device)
{
    Mix_MusicAPI target_midi_api = MIX_MUSIC_NATIVEMIDI;
    SDL_bool use_native_midi = SDL_FALSE;
    int midi_device = device ? *device : mididevice_current;

#ifdef MUSIC_MID_NATIVE
    if (SDL_GetHintBoolean("SDL_NATIVE_MUSIC", SDL_FALSE) && native_midi_detect()) {
        use_native_midi = SDL_TRUE;
        target_midi_api = MIX_MUSIC_NATIVEMIDI;
        mididevice_current = MIDI_Native;
    }
#endif

    if (!use_native_midi) {
        switch (midi_device) {
        #ifdef MUSIC_MID_ADLMIDI
        case MIDI_ADLMIDI:
            target_midi_api = MIX_MUSIC_ADLMIDI;
            break;
        #endif
        #ifdef MUSIC_MID_OPNMIDI
        case MIDI_OPNMIDI:
            target_midi_api = MIX_MUSIC_OPNMIDI;
            break;
        #endif
        #ifdef MUSIC_MID_TIMIDITY
        case MIDI_Timidity:
            target_midi_api = MIX_MUSIC_TIMIDITY;
            break;
        #endif
        #ifdef MUSIC_MID_FLUIDSYNTH
        case MIDI_Fluidsynth:
            target_midi_api = MIX_MUSIC_FLUIDSYNTH;
            break;
        #endif
        #ifdef MUSIC_MID_NATIVE
        case MIDI_Native:
            target_midi_api = MIX_MUSIC_NATIVEMIDI;
            break;
        #endif
        default:
            if (device)
                *device = MIDI_ANY;
            else
                mididevice_current = MIDI_ANY;
            break;
        }
    }

    return target_midi_api;
}

int parse_midi_args(const char *args)
{
    char arg[1024];
    char type = 'x';
    size_t maxlen = 0;
    size_t i, j = 0;
    int value_opened = 0;
    int selected_midi_device = -1;

    if (args == NULL) {
        return -1;
    }

    if (mididevice_args_lock) {
        return -1; /* Do nothing as generic MIDI arguments are been locked */
    }

    maxlen = SDL_strlen(args) + 1;

    for (i = 0; i < maxlen; i++) {
        char c = args[i];
        if (value_opened == 1) {
            if ((c == ';') || (c == '\0')) {
                int value;
                arg[j] = '\0';
                value = atoi(arg);
                switch(type) {
                case 's':
                    if ((value >= 0) && value < MIDI_KnuwnDevices)
                        selected_midi_device = value;
                    break;
                case '\0':
                default:
                    break;
                }
                value_opened = 0;
            }
            arg[j++] = c;
        } else {
            if (c == '\0') {
                return selected_midi_device;
            }
            type = c;
            value_opened = 1;
            j = 0;
        }
    }
    return selected_midi_device;
}

/* Open the music interfaces for a given music type */
SDL_bool open_music_type(Mix_MusicType type)
{
    return open_music_type_ex(type, -1);
}

/* Open the music interfaces for a given music type, also select a MIDI library */
SDL_bool open_music_type_ex(Mix_MusicType type, int midi_device)
{
    size_t i;
    int opened = 0;
    SDL_bool use_any_midi = SDL_FALSE;
    Mix_MusicAPI target_midi_api = MIX_MUSIC_NATIVEMIDI;

    if (!music_spec.format) {
        /* Music isn't opened yet */
        return SDL_FALSE;
    }

    if (type == MUS_MID) {
        target_midi_api = get_current_midi_api(&midi_device);
        if (midi_device == MIDI_ANY) {
            use_any_midi = SDL_TRUE;
        }
    }

    for (i = 0; i < SDL_arraysize(s_music_interfaces); ++i) {
        Mix_MusicInterface *interface = s_music_interfaces[i];
        if (!interface->loaded) {
            continue;
        }
        if (type != MUS_NONE && interface->type != type) {
            continue;
        }

        if (interface->type == MUS_MID && !use_any_midi && interface->api != target_midi_api) {
            continue;
        }

        if (!interface->opened) {
            if (interface->Open && interface->Open(&music_spec) < 0) {
                if (SDL_GetHintBoolean(SDL_MIXER_HINT_DEBUG_MUSIC_INTERFACES, SDL_FALSE)) {
                    SDL_Log("Couldn't open %s: %s\n", interface->tag, Mix_GetError());
                }
                continue;
            }
            interface->opened = SDL_TRUE;
            add_music_decoder(interface->tag);
        }
        ++opened;
    }

    if (has_music(MUS_MOD)) {
        add_music_decoder("MOD");
        add_chunk_decoder("MOD");
    }
    if (has_music(MUS_MID)) {
        add_music_decoder("MIDI");
        add_chunk_decoder("MID");
    }
    if (has_music(MUS_OGG)) {
        add_music_decoder("OGG");
        add_chunk_decoder("OGG");
    }
    if (has_music(MUS_OPUS)) {
        add_music_decoder("OPUS");
        add_chunk_decoder("OPUS");
    }
    if (has_music(MUS_MP3)) {
        add_music_decoder("MP3");
        add_chunk_decoder("MP3");
    }
    if (has_music(MUS_FLAC)) {
        add_music_decoder("FLAC");
        add_chunk_decoder("FLAC");
    }

    return (opened > 0) ? SDL_TRUE : SDL_FALSE;
}

/* Initialize the music interfaces with a certain desired audio format */
void open_music(const SDL_AudioSpec *spec)
{
#ifdef MIX_INIT_SOUNDFONT_PATHS
    if (!soundfont_paths) {
        soundfont_paths = SDL_strdup(MIX_INIT_SOUNDFONT_PATHS);
    }
#endif

    /* Load the music interfaces that don't have explicit initialization */
    load_music_type(MUS_CMD);
    load_music_type(MUS_WAV);

    /* Open all the interfaces that are loaded */
    music_spec = *spec;
    open_music_type(MUS_NONE);

    Mix_VolumeMusicStream(NULL, MIX_MAX_VOLUME);

    /* Calculate the number of ms for each callback */
    ms_per_step = (int) (((double)spec->samples * 1000.0) / spec->freq);
}

/* Return SDL_TRUE if the music type is available */
SDL_bool has_music(Mix_MusicType type)
{
    size_t i;
    for (i = 0; i < SDL_arraysize(s_music_interfaces); ++i) {
        Mix_MusicInterface *interface = s_music_interfaces[i];
        if (interface->type != type) {
            continue;
        }
        if (interface->opened) {
            return SDL_TRUE;
        }
    }
    return SDL_FALSE;
}

/*
    XMI and MUS are can be played on ADLMIDI or OPNMIDI only. Yet.
 */
#if defined(MUSIC_MID_ADLMIDI) || defined(MUSIC_MID_OPNMIDI)
static Mix_MusicType xmi_compatible_midi_player()
{
#if defined(MUSIC_MID_ADLMIDI) && defined(MUSIC_MID_OPNMIDI)
    if((mididevice_current != MIDI_ADLMIDI) && (mididevice_current != MIDI_OPNMIDI))
        return MUS_ADLMIDI;
    else
#elif defined(MUSIC_MID_ADLMIDI)
    if (mididevice_next != MIDI_ADLMIDI)
        return MUS_ADLMIDI;
    else
#endif
        return MUS_MID;
}
#endif

#ifdef MUSIC_MID_ADLMIDI
static int detect_imf(SDL_RWops *in, Sint64 start)
{
    Uint16 chunksize, buff, i = 42;
    Uint32 sum1 = 0,  sum2 = 0;
    Uint16 word;

    if(!in)
        return 0;

    SDL_RWseek(in, start, RW_SEEK_SET);

    if (SDL_RWread(in, &word, 1, 2) != 2) {
        SDL_RWseek(in, start, RW_SEEK_SET);
        return 0;
    }
    chunksize = SDL_SwapLE16(word);
    if ((chunksize == 0) || (chunksize & 3)) {
        SDL_RWseek(in, start, RW_SEEK_SET);
        return 0;
    }

    while (i > 0) {
        if (SDL_RWread(in, &word, 1, 2) != 2) {
            SDL_RWseek(in, start, RW_SEEK_SET);
            break;
        }
        buff = SDL_SwapLE16(word);
        sum1 += buff;
        if (SDL_RWread(in, &word, 1, 2) != 2) {
            SDL_RWseek(in, start, RW_SEEK_SET);
            break;
        }
        buff = SDL_SwapLE16(word);
        sum2 += buff;
        i--;
    }
    SDL_RWseek(in, start, RW_SEEK_SET);
    return (sum1 > sum2);
}

static int detect_ea_rsxx(SDL_RWops *in, Sint64 start, Uint8 magic_byte)
{
    int res = SDL_FALSE;
    Uint8 sub_magic[6];

    if (magic_byte != 0x7D)
        return res;

    SDL_RWseek(in, start + 0x6D, RW_SEEK_SET);

    if (SDL_RWread(in, &sub_magic, 1, 6) != 6) {
        SDL_RWseek(in, start, RW_SEEK_SET);
        return 0;
    }

    if (SDL_memcmp(sub_magic, "rsxx}u", 6) == 0)
        res = SDL_TRUE;

    SDL_RWseek(in, start, RW_SEEK_SET);

    return res;
}
#endif

#if defined(MUSIC_MP3_MAD) || defined(MUSIC_MP3_MPG123) || defined(MUSIC_MP3_SMPEG)
static int detect_mp3(Uint8 *magic, SDL_RWops *src, Sint64 start)
{
    Uint32 null = 0;
    Uint8  magic2[5];
    unsigned char byte = 0;
    Sint64 notNullPos = 0;

    SDL_memcpy(magic2, magic, 5);

    if (SDL_strncmp((char *)magic2, "ID3", 3) == 0) {
        SDL_RWseek(src, start, RW_SEEK_SET);
        return 1;
    }

    SDL_RWseek(src, start, RW_SEEK_SET);

    /* If first bytes are zero */
    if (SDL_memcmp(magic2, &null, 4) != 0) {
        goto readHeader;
    }

digMoreBytes:
    {
        /* Find nearest non zero */
        /* Look for FF byte */
        while ((SDL_RWread(src, &byte, 1, 1) == 1) &&
               (byte != 0xFF) &&
               (SDL_RWtell(src) < (start + 10240)))
        {}

        /* with offset -1 byte */
        notNullPos = SDL_RWtell(src) - 1;
        SDL_RWseek(src, notNullPos, RW_SEEK_SET);

        /* If file contains only null bytes */
        if (byte != 0xFF) {
            /* printf("WRONG BYTE\n"); */
            SDL_RWseek(src, start, RW_SEEK_SET);
            return 0;
        }
        if (SDL_RWread(src, magic2, 1, 4) != 4) {
            /* printf("MAGIC WRONG\n"); */
            SDL_RWseek(src, start, RW_SEEK_SET);
            return 0;
        }
    }

readHeader:
    if (
        ((magic2[0] & 0xff) != 0xff) || ((magic2[1] & 0xf0) != 0xf0) || /*  No sync bits */
        ((magic2[2] & 0xf0) == 0x00) || /*  Bitrate is 0 */
        ((magic2[2] & 0xf0) == 0xf0) || /*  Bitrate is 15 */
        ((magic2[2] & 0x0c) == 0x0c) || /*  Frequency is 3 */
        ((magic2[1] & 0x06) == 0x00)   /*  Layer is 4 */
    ) {
        /* printf("WRONG BITS\n"); */
        goto digMoreBytes;
    }

    SDL_RWseek(src, start, RW_SEEK_SET);
    return 1;
}
#endif


Mix_MusicType detect_music_type_from_magic(SDL_RWops *src)
{
    Uint8 magic[25];

    Sint64 start = SDL_RWtell(src);
    if (SDL_RWread(src, magic, 1, 24) != 24) {
        Mix_SetError("Couldn't read from RWops");
        return MUS_NONE;
    }
    SDL_RWseek(src, start, RW_SEEK_SET);
    magic[24]       = '\0';

    /* Drop out some known but not supported file types (Archives, etc.) */
    if (SDL_memcmp(magic, "PK\x03\x04", 3) == 0) {
        return MUS_NONE;
    }
    if (SDL_memcmp(magic, "\x37\x7A\xBC\xAF\x27\x1C", 6) == 0) {
        return MUS_NONE;
    }

    /* Ogg Vorbis files have the magic four bytes "OggS" */
    if (SDL_memcmp(magic, "OggS", 4) == 0) {
        return MUS_OGG;
    }

    /* FLAC files have the magic four bytes "fLaC" */
    if (SDL_memcmp(magic, "fLaC", 4) == 0) {
        return MUS_FLAC;
    }

    /* MIDI files have the magic four bytes "MThd" */
    if (SDL_memcmp(magic, "MThd", 4) == 0) {
        return MUS_MID;
    }
    /* RIFF MIDI files have the magic four bytes "RIFF" and then "RMID" */
    if ((SDL_memcmp(magic, "RIFF", 4) == 0) && (SDL_memcmp(magic + 8, "RMID", 4) == 0)) {
        return MUS_MID;
    }
    #if defined(MUSIC_MID_ADLMIDI) || defined(MUSIC_MID_OPNMIDI)
    if (SDL_memcmp(magic, "MUS\x1A", 4) == 0) {
        return xmi_compatible_midi_player();
    }
    if ((SDL_memcmp(magic, "FORM", 4) == 0) && (SDL_memcmp(magic + 8, "XDIR", 4) == 0)) {
        return xmi_compatible_midi_player();
    }
    #endif

    /* WAVE files have the magic four bytes "RIFF"
           AIFF files have the magic 12 bytes "FORM" XXXX "AIFF" */
    if (((SDL_memcmp(magic, "RIFF", 4) == 0) && (SDL_memcmp((magic + 8), "WAVE", 4) == 0)) ||
       ((SDL_memcmp(magic, "FORM", 4) == 0) && (SDL_memcmp((magic + 8), "XDIR", 4) != 0))) {
        return MUS_WAV;
    }

    if (SDL_memcmp(magic, "GMF\x1", 4) == 0) {
        return MUS_ADLMIDI;
    }
    if (SDL_memcmp(magic, "CTMF", 4) == 0) {
        return MUS_ADLMIDI;
    }

    if (SDL_memcmp(magic, "ID3", 3) == 0 ||
        (magic[0] == 0xFF && (magic[1] & 0xFE) == 0xFA)) {
        return MUS_MP3;
    }

    /* GME Specific files */
    if (SDL_memcmp(magic, "ZXAY", 4) == 0)
        return MUS_GME;
    if (SDL_memcmp(magic, "GBS\x01", 4) == 0)
        return MUS_GME;
    if (SDL_memcmp(magic, "GYMX", 4) == 0)
        return MUS_GME;
    if (SDL_memcmp(magic, "HESM", 4) == 0)
        return MUS_GME;
    if (SDL_memcmp(magic, "KSCC", 4) == 0)
        return MUS_GME;
    if (SDL_memcmp(magic, "KSSX", 4) == 0)
        return MUS_GME;
    if (SDL_memcmp(magic, "NESM", 4) == 0)
        return MUS_GME;
    if (SDL_memcmp(magic, "NSFE", 4) == 0)
        return MUS_GME;
    if (SDL_memcmp(magic, "SAP\x0D", 4) == 0)
        return MUS_GME;
    if (SDL_memcmp(magic, "SNES", 4) == 0)
        return MUS_GME;
    if (SDL_memcmp(magic, "Vgm ", 4) == 0)
        return MUS_GME;
    if (SDL_memcmp(magic, "\x1f\x8b", 2) == 0)
        return MUS_GME;

    /* Detect some module files */
    if (SDL_memcmp(magic, "Extended Module", 15) == 0)
        return MUS_MOD;
    if (SDL_memcmp(magic, "ASYLUM Music Format V", 22) == 0)
        return MUS_MOD;
    if (SDL_memcmp(magic, "Extreme", 7) == 0)
        return MUS_MOD;
    if (SDL_memcmp(magic, "IMPM", 4) == 0)
        return MUS_MOD;
    if (SDL_memcmp(magic, "DBM0", 4) == 0)
        return MUS_MOD;
    if (SDL_memcmp(magic, "DDMF", 4) == 0)
        return MUS_MOD;
    /*  SMF files have the magic four bytes "RIFF" */
    if ((SDL_memcmp(magic, "RIFF", 4) == 0) &&
       (SDL_memcmp(magic + 8,  "DSMF", 4) == 0) &&
       (SDL_memcmp(magic + 12, "SONG", 4) == 0))
        return MUS_MOD;
    if (SDL_memcmp(magic, "MAS_UTrack_V00", 14) == 0)
        return MUS_MOD;
    if (SDL_memcmp(magic, "GF1PATCH110", 11) == 0)
        return MUS_MOD;
    if (SDL_memcmp(magic, "FAR=", 4) == 0)
        return MUS_MOD;
    if (SDL_memcmp(magic, "MTM", 3) == 0)
        return MUS_MOD;
    if (SDL_memcmp(magic, "MMD", 3) == 0)
        return MUS_MOD;
    if (SDL_memcmp(magic, "PSM\x20", 4) == 0)
        return MUS_MOD;
    if (SDL_memcmp(magic, "PTMF", 4) == 0)
        return MUS_MOD;
    if (SDL_memcmp(magic, "MT20", 4) == 0)
        return MUS_MOD;
    if (SDL_memcmp(magic, "OKTA", 4) == 0)
        return MUS_MOD;
    if (SDL_memcmp(magic, "JN", 2) == 0)
        return MUS_MOD;
    if (SDL_memcmp(magic, "if", 2) == 0)
        return MUS_MOD;

    #if defined(MUSIC_MP3_MAD) || defined(MUSIC_MP3_MPG123) || defined(MUSIC_MP3_SMPEG)
    /* Detect MP3 format [needs scanning of bigger part of the file] */
    if (detect_mp3(magic, src, start)) {
        return MUS_MP3;
    }
    #endif

    #ifdef MUSIC_MID_ADLMIDI
    /* Detect id Software Music Format file */
    if (detect_imf(src, start)) {
        return MUS_ADLMIDI;
    }
    /* Detect EA MUS (RSXX) format */
    if (detect_ea_rsxx(src, start, magic[0])) {
        return MUS_ADLMIDI;
    }
    #endif

      /* Reset position to zero! */
    SDL_RWseek(src, start, RW_SEEK_SET);

    /* Assume MOD format.
     *
     * Apparently there is no way to check if the file is really a MOD,
     * or there are too many formats supported by MikMod/ModPlug, or
     * MikMod/ModPlug does this check by itself. */
    return MUS_MOD;
}

static Mix_MusicType detect_music_type(SDL_RWops *src)
{
    Uint8 magic[12];
    Mix_MusicType t;

    if (SDL_RWread(src, magic, 1, 12) != 12) {
        Mix_SetError("Couldn't read first 12 bytes of audio data");
        return MUS_NONE;
    }
    SDL_RWseek(src, -12, RW_SEEK_CUR);

    /* WAVE files have the magic four bytes "RIFF"
       AIFF files have the magic 12 bytes "FORM" XXXX "AIFF" */
    if (((SDL_memcmp(magic, "RIFF", 4) == 0) && (SDL_memcmp((magic+8), "WAVE", 4) == 0)) ||
       ((SDL_memcmp(magic, "FORM", 4) == 0) && (SDL_memcmp((magic+8), "XDIR", 4) != 0))) {
        return MUS_WAV;
    }
    t = detect_music_type_from_magic(src);
    if (t == MUS_OGG) {
        Sint64 pos = SDL_RWtell(src);
        SDL_RWseek(src, 28, RW_SEEK_CUR);
        SDL_RWread(src, magic, 1, 8);
        SDL_RWseek(src, pos, RW_SEEK_SET);
        if (SDL_memcmp(magic, "OpusHead", 8) == 0) {
            return MUS_OPUS;
        }
    }
    return t;
}

/*
 * Split path and arguments after "|" character.
 */
static int split_path_and_params(const char *path, char **file, char **args)
{
    char *music_file, *music_args;
    size_t i, j, file_str_len, buffer_len,
    args_div = 0,
    srclen = SDL_strlen(path);

    file_str_len = SDL_strlen(path);
    buffer_len = file_str_len + 1;
    music_file = (char *)SDL_malloc(buffer_len);
    music_args = (char *)SDL_malloc(buffer_len);
    SDL_memset(music_file, 0, buffer_len);
    SDL_memset(music_args, 0, buffer_len);
    SDL_strlcpy(music_file, path, buffer_len);
    #ifdef _WIN32
    if (music_file) {
        size_t i = 0;
        for (i = 0; music_file[i] != '\0'; i++) {
            if (music_file[i] == '\\') {
                music_file[i] = '/';
            }
        }
    }
    #endif
    *file = music_file;
    *args = music_args;

    for (i = 0; i < srclen; i++) {
        if (music_file[i] == '|') {
            if (i == 0) {
                Mix_SetError("Empty filename!");
                return 0;
            }
            args_div = i;
        }
    }
    if (args_div != 0) {
        music_file[args_div] = '\0';
        for (j = 0, i = (args_div + 1); i <= srclen; i++, j++) {
            music_args[j] = music_file[i];
        }
        music_args[j] = '\0';
    }

    return 1;
}

/* Load a music file */
Mix_Music * SDLCALLCC Mix_LoadMUS(const char *file)
{
    size_t i;
    void *context;
    char *ext;
    Mix_MusicType type;
    SDL_RWops *src;
    Mix_Music *ret = NULL;
    char *music_file = NULL;
    char *music_args = NULL;

    /* ========== Path arguments ========== */
    if (!file) {
        Mix_SetError("Null filename!");
        return NULL;
    }
    if (split_path_and_params(file, &music_file, &music_args) == 0) {
        SDL_free(music_file);
        SDL_free(music_args);
        Mix_SetError("Invalid path argument syntax!");
        return NULL;
    }
    /* Use path with stripped down arguments after stick | sign*/
    file = music_file;
    /* ========== Path arguments END ====== */

    for (i = 0; i < SDL_arraysize(s_music_interfaces); ++i) {
        Mix_MusicInterface *interface = s_music_interfaces[i];
        if (!interface->opened || (!interface->CreateFromFile && !interface->CreateFromFileEx)) {
            continue;
        }

        if (interface->type == MUS_MID) {
            Mix_MusicAPI target_midi_api = 0;
            SDL_bool use_any_midi = SDL_FALSE;
            int midi_device = parse_midi_args(music_args);
            if (midi_device < 0) {
                music_args[0] = '\0';
                midi_device = mididevice_current;
            }
            target_midi_api = get_current_midi_api(&midi_device);
            if (midi_device == MIDI_ANY) {
                use_any_midi = SDL_TRUE;
            }
            if (!use_any_midi && interface->api != target_midi_api) {
                continue;
            }
        }

        if (interface->CreateFromFileEx) {
            context = interface->CreateFromFileEx(file, music_args);
        } else {
            context = interface->CreateFromFile(file);
        }

        if (context) {
            /* Allocate memory for the music structure */
            Mix_Music *music = (Mix_Music *)SDL_calloc(1, sizeof(Mix_Music));
            if (music == NULL) {
                Mix_SetError("Out of memory");
                return NULL;
            }
            music->interface = interface;
            music->context = context;
            SDL_strlcpy(music->music_filename, (SDL_strstr(music_file, "/")) ? (SDL_strrchr(music_file, '/') + 1) : music_file, 1024);
            SDL_free(music_file);
            SDL_free(music_args);
            return music;
        }
    }

    src = SDL_RWFromFile(file, "rb");
    if (src == NULL) {
        Mix_SetError("Couldn't open '%s'", file);
        SDL_free(music_file);
        SDL_free(music_args);
        return NULL;
    }

    /* Use the extension as a first guess on the file type */
    type = MUS_NONE;
    ext = SDL_strrchr(file, '.');
    if (ext) {
        ++ext; /* skip the dot in the extension */
        if (SDL_strcasecmp(ext, "WAV") == 0) {
            type = MUS_WAV;
        } else if (SDL_strcasecmp(ext, "MID") == 0 ||
                    SDL_strcasecmp(ext, "MIDI") == 0 ||
                    SDL_strcasecmp(ext, "RMI") == 0 ||
                    SDL_strcasecmp(ext, "KAR") == 0) {
            type = MUS_MID;
        } else if (SDL_strcasecmp(ext, "OGG") == 0) {
            type = MUS_OGG;
        } else if (SDL_strcasecmp(ext, "OPUS") == 0) {
            type = MUS_OPUS;
        } else if (SDL_strcasecmp(ext, "FLAC") == 0) {
            type = MUS_FLAC;
        } else  if (SDL_strcasecmp(ext, "MPG") == 0 ||
                     SDL_strcasecmp(ext, "MPEG") == 0 ||
                     SDL_strcasecmp(ext, "MP3") == 0 ||
                     SDL_strcasecmp(ext, "MAD") == 0) {
            type = MUS_MP3;
        } else if (SDL_strcasecmp(ext, "669") == 0 ||
                    SDL_strcasecmp(ext, "AMF") == 0 ||
                    SDL_strcasecmp(ext, "AMS") == 0 ||
                    SDL_strcasecmp(ext, "DBM") == 0 ||
                    SDL_strcasecmp(ext, "DSM") == 0 ||
                    SDL_strcasecmp(ext, "FAR") == 0 ||
                    SDL_strcasecmp(ext, "IT") == 0 ||
                    SDL_strcasecmp(ext, "MED") == 0 ||
                    SDL_strcasecmp(ext, "MDL") == 0 ||
                    SDL_strcasecmp(ext, "MOD") == 0 ||
                    SDL_strcasecmp(ext, "MOL") == 0 ||
                    SDL_strcasecmp(ext, "MTM") == 0 ||
                    SDL_strcasecmp(ext, "NST") == 0 ||
                    SDL_strcasecmp(ext, "OKT") == 0 ||
                    SDL_strcasecmp(ext, "PTM") == 0 ||
                    SDL_strcasecmp(ext, "S3M") == 0 ||
                    SDL_strcasecmp(ext, "STM") == 0 ||
                    SDL_strcasecmp(ext, "ULT") == 0 ||
                    SDL_strcasecmp(ext, "UMX") == 0 ||
                    SDL_strcasecmp(ext, "WOW") == 0 ||
                    SDL_strcasecmp(ext, "XM") == 0) {
            type = MUS_MOD;
        } else if (SDL_strcasecmp(ext, "SPC") == 0 ||
                   SDL_strcasecmp(ext, "VGM") == 0 ||
                   SDL_strcasecmp(ext, "HES") == 0 ||
                   SDL_strcasecmp(ext, "NSF") == 0 ||
                   SDL_strcasecmp(ext, "NSFE") == 0) {
            type = MUS_GME;
        }
    }
    ret = Mix_LoadMUSType_RW_ARG(src, type, SDL_TRUE, music_args);
    if (ret) {
        SDL_strlcpy(ret->music_filename, (SDL_strstr(music_file, "/")) ? (SDL_strrchr(music_file, '/') + 1) : music_file, 1023);
    }
    SDL_free(music_file);
    SDL_free(music_args);
    return ret;
}

Mix_Music * SDLCALLCC Mix_LoadMUS_RW(SDL_RWops *src, int freesrc)
{
    return Mix_LoadMUSType_RW(src, MUS_NONE, freesrc);
}

Mix_Music *SDLCALLCC Mix_LoadMUS_RW_ARG(SDL_RWops *src, int freesrc, const char *args)
{
    return Mix_LoadMUSType_RW_ARG(src, MUS_NONE, freesrc, args);
}

Mix_Music *SDLCALLCC Mix_LoadMUS_RW_GME(SDL_RWops *src, int freesrc, int trackID)
{
    char music_args[25];
    music_args[0] = '\0';
    SDL_snprintf(music_args, 25, "%i", trackID);
    return Mix_LoadMUSType_RW_ARG(src, MUS_NONE, freesrc, music_args);
}

Mix_Music * SDLCALLCC Mix_LoadMUSType_RW(SDL_RWops *src, Mix_MusicType type, int freesrc)
{
    return Mix_LoadMUSType_RW_ARG(src, type, freesrc, "");
}

Mix_Music * SDLCALLCC Mix_LoadMUSType_RW_ARG(SDL_RWops *src, Mix_MusicType type, int freesrc, const char *args)
{
    size_t i;
    void *context;
    Sint64 start;
    int midi_device = mididevice_current;

    if (!src) {
        Mix_SetError("RWops pointer is NULL");
        return NULL;
    }
    start = SDL_RWtell(src);

    /* If the caller wants auto-detection, figure out what kind of file
     * this is. */
    if (type == MUS_NONE) {
        if ((type = detect_music_type(src)) == MUS_NONE) {
            /* Don't call Mix_SetError() since detect_music_type() does that. */
            if (freesrc) {
                SDL_RWclose(src);
            }
            return NULL;
        }
    }

    if (type == MUS_MID) {
        midi_device = parse_midi_args(args);
        if (midi_device < 0) {
            args = NULL;
            midi_device = mididevice_current;
        }
    }

    Mix_ClearError();

    if (load_music_type(type) && open_music_type_ex(type, midi_device)) {
        Mix_MusicAPI target_midi_api = get_current_midi_api(&midi_device);
        SDL_bool use_any_midi = SDL_FALSE;
        if (midi_device == MIDI_ANY) {
            use_any_midi = SDL_TRUE;
        }
        for (i = 0; i < SDL_arraysize(s_music_interfaces); ++i) {
            Mix_MusicInterface *interface = s_music_interfaces[i];
            if (!interface->opened || type != interface->type ||
                (!interface->CreateFromRW && !interface->CreateFromRWex)) {
                continue;
            }
            if (interface->type == MUS_MID && !use_any_midi && interface->api != target_midi_api) {
                continue;
            }

            if (interface->CreateFromRWex) {
                context = interface->CreateFromRWex(src, freesrc, args);
            } else {
                context = interface->CreateFromRW(src, freesrc);
            }

            if (context) {
                /* Allocate memory for the music structure */
                Mix_Music *music = (Mix_Music *)SDL_calloc(1, sizeof(Mix_Music));
                if (music == NULL) {
                    interface->Delete(context);
                    Mix_SetError("Out of memory");
                    return NULL;
                }
                music->interface = interface;
                music->context = context;

                if (SDL_GetHintBoolean(SDL_MIXER_HINT_DEBUG_MUSIC_INTERFACES, SDL_FALSE)) {
                    SDL_Log("Loaded music with %s\n", interface->tag);
                }
                return music;
            }

            /* Reset the stream for the next decoder */
            SDL_RWseek(src, start, RW_SEEK_SET);
        }
    }

    if (!*Mix_GetError()) {
        Mix_SetError("Unrecognized audio format");
    }
    if (freesrc) {
        SDL_RWclose(src);
    } else {
        SDL_RWseek(src, start, RW_SEEK_SET);
    }
    return NULL;
}

/* Free a music chunk previously loaded */
void SDLCALLCC Mix_FreeMusic(Mix_Music *music)
{
    if (music) {
        /* Stop the music if it's currently playing */
        Mix_LockAudio();
        if (music == music_playing) {
            /* Wait for any fade out to finish */
            while (music->fading == MIX_FADING_OUT) {
                Mix_UnlockAudio();
                SDL_Delay(100);
                Mix_LockAudio();
            }
            if (music == music_playing) {
                music_internal_halt(music_playing);
            }
        }
        Mix_UnlockAudio();

        music->interface->Delete(music->context);
        SDL_free(music);
    }
}

/* Find out the music format of a mixer music, or the currently playing
   music, if 'music' is NULL.
*/
Mix_MusicType SDLCALLCC Mix_GetMusicType(const Mix_Music *music)
{
    Mix_MusicType type = MUS_NONE;

    if (music) {
        type = music->interface->type;
    } else {
        Mix_LockAudio();
        if (music_playing) {
            type = music_playing->interface->type;
        }
        Mix_UnlockAudio();
    }
    return(type);
}

static const char * get_music_tag_internal(const Mix_Music *music, Mix_MusicMetaTag tag_type)
{
    const char *tag = "";

    Mix_LockAudio();
    if (music && music->interface->GetMetaTag) {
        tag = music->interface->GetMetaTag(music->context, tag_type);
    } else if (music_playing && music_playing->interface->GetMetaTag) {
        tag = music_playing->interface->GetMetaTag(music_playing->context, tag_type);
    } else {
        Mix_SetError("Music isn't playing");
    }
    Mix_UnlockAudio();
    return tag;
}

const char *SDLCALLCC Mix_GetMusicTitleTag(const Mix_Music *music)
{
    return get_music_tag_internal(music, MIX_META_TITLE);
}

/* Get music title from meta-tag if possible */
const char *SDLCALLCC Mix_GetMusicTitle(const Mix_Music *music)
{
    const char *tag = Mix_GetMusicTitleTag(music);
    if (SDL_strlen(tag) > 0) {
        return tag;
    }
    if (music) {
        return music->music_filename;
    }
    if (music_playing) {
        return music_playing->music_filename;
    }
    return "";
}

const char *SDLCALLCC Mix_GetMusicArtistTag(const Mix_Music *music)
{
    return get_music_tag_internal(music, MIX_META_ARTIST);
}

const char *SDLCALLCC Mix_GetMusicAlbumTag(const Mix_Music *music)
{
    return get_music_tag_internal(music, MIX_META_ALBUM);
}

const char *SDLCALLCC Mix_GetMusicCopyrightTag(const Mix_Music *music)
{
    return get_music_tag_internal(music, MIX_META_COPYRIGHT);
}

/* Play a music chunk.  Returns 0, or -1 if there was an error.
 */
static int music_internal_play(Mix_Music *music, int play_count, double position)
{
    int retval = 0;

#if defined(__MACOSX__) && defined(MID_MUSIC_NATIVE)
    /* This fixes a bug with native MIDI on Mac OS X, where you
       can't really stop and restart MIDI from the audio callback.
    */
    if (music == music_playing && music->api == MIX_MUSIC_NATIVEMIDI) {
        /* Just a seek suffices to restart playing */
        music_internal_position(position);
        return 0;
    }
#endif

    /* Note the music we're playing */
    if (music_playing) {
        music_internal_halt(music_playing);
    }
    music_playing = music;
    music_playing->playing = SDL_TRUE;

    /* Set the initial volume */
    music_internal_initialize_volume();

    /* Set up for playback */
    retval = music->interface->Play(music->context, play_count);

    /* Set the playback position, note any errors if an offset is used */
    if (retval == 0) {
        if (position > 0.0) {
            if (music_internal_position(music_playing, position) < 0) {
                Mix_SetError("Position not implemented for music type");
                retval = -1;
            }
        } else {
            music_internal_position(music_playing, 0.0);
        }
    }

    /* If the setup failed, we're not playing any music anymore */
    if (retval < 0) {
        music->playing = SDL_FALSE;
        music_playing = NULL;
    }
    return(retval);
}

int SDLCALLCC Mix_FadeInMusicPos(Mix_Music *music, int loops, int ms, double position)
{
    int retval;

    if (ms_per_step == 0) {
        SDL_SetError("Audio device hasn't been opened");
        return(-1);
    }

    /* Don't play null pointers :-) */
    if (music == NULL) {
        Mix_SetError("music parameter was NULL");
        return(-1);
    }

    /* Setup the data */
    if (ms) {
        music->fading = MIX_FADING_IN;
    } else {
        music->fading = MIX_NO_FADING;
    }
    music->fade_step = 0;
    music->fade_steps = ms/ms_per_step;

    /* Play the puppy */
    Mix_LockAudio();
    /* If the current music is fading out, wait for the fade to complete */
    while (music_playing && (music_playing->fading == MIX_FADING_OUT)) {
        Mix_UnlockAudio();
        SDL_Delay(100);
        Mix_LockAudio();
    }
    if (loops == 0) {
        /* Loop is the number of times to play the audio */
        loops = 1;
    }
    retval = music_internal_play(music, loops, position);
    /* Set music as active */
    music_active = (retval == 0);
    Mix_UnlockAudio();

    return(retval);
}
int SDLCALLCC Mix_FadeInMusic(Mix_Music *music, int loops, int ms)
{
    return Mix_FadeInMusicPos(music, loops, ms, 0.0);
}
int SDLCALLCC Mix_PlayMusic(Mix_Music *music, int loops)
{
    return Mix_FadeInMusicPos(music, loops, 0, 0.0);
}

/* Set the playing music position */
int music_internal_position(Mix_Music *music, double position)
{
    if (music->interface->Seek) {
        return music->interface->Seek(music->context, position);
    }
    return -1;
}
int SDLCALLCC Mix_SetMusicStreamPosition(Mix_Music *music, double position)
{
    int retval;

    Mix_LockAudio();
    if (music) {
        retval = music_internal_position(music, position);
        if (retval < 0) {
            Mix_SetError("Position not implemented for music type");
        }
    } else if (music_playing) {
        retval = music_internal_position(music_playing, position);
        if (retval < 0) {
            Mix_SetError("Position not implemented for music type");
        }
    } else {
        Mix_SetError("Music isn't playing");
        retval = -1;
    }
    Mix_UnlockAudio();

    return(retval);
}

/* Deprecated call, kept for ABI compatibility */
int SDLCALLCC Mix_SetMusicPosition(double position)
{
    return Mix_SetMusicStreamPosition(NULL, position);
}

/* Set the playing music position */
static double music_internal_position_get(Mix_Music *music)
{
    if (music->interface->Tell) {
        return music->interface->Tell(music->context);
    }
    return -1;
}
double SDLCALLCC Mix_GetMusicPosition(Mix_Music *music)
{
    double retval;

    Mix_LockAudio();
    if (music) {
        retval = music_internal_position_get(music);
    } else if (music_playing) {
        retval = music_internal_position_get(music_playing);
    } else {
        Mix_SetError("Music isn't playing");
        retval = -1.0;
    }
    Mix_UnlockAudio();

    return(retval);
}

/* Get total playing music position */
static double music_internal_position_total(Mix_Music *music)
{
    if (music->interface->FullLength) {
        return music->interface->FullLength(music->context);
    }
    return -1;
}
double SDLCALLCC Mix_GetMusicTotalTime(Mix_Music *music)
{
    double retval;

    Mix_LockAudio();
    if (music) {
        retval = music_internal_position_total(music);
    } else if (music_playing) {
        retval = music_internal_position_total(music_playing);
    } else {
        Mix_SetError("Music isn't playing");
        retval = -1.0;
    }
    Mix_UnlockAudio();

    return(retval);
}

/* Get Loop start position */
static double music_internal_loop_start(Mix_Music *music)
{
    if (music->interface->LoopStart) {
        return music->interface->LoopStart(music->context);
    }
    return -1;
}
double SDLCALLCC Mix_GetMusicLoopStartTime(Mix_Music *music)
{
    double retval;

    Mix_LockAudio();
    if (music) {
        retval = music_internal_loop_start(music);
    } else if (music_playing) {
        retval = music_internal_loop_start(music_playing);
    } else {
        Mix_SetError("Music isn't playing");
        retval = -1.0;
    }
    Mix_UnlockAudio();

    return(retval);
}

/* Get Loop end position */
static double music_internal_loop_end(Mix_Music *music)
{
    if (music->interface->LoopEnd) {
        return music->interface->LoopEnd(music->context);
    }
    return -1;
}
double SDLCALLCC Mix_GetMusicLoopEndTime(Mix_Music *music)
{
    double retval;

    Mix_LockAudio();
    if (music) {
        retval = music_internal_loop_end(music);
    } else if (music_playing) {
        retval = music_internal_loop_end(music_playing);
    } else {
        Mix_SetError("Music isn't playing");
        retval = -1.0;
    }
    Mix_UnlockAudio();

    return(retval);
}

/* Get Loop end position */
static double music_internal_loop_length(Mix_Music *music)
{
    if (music->interface->LoopLength) {
        return music->interface->LoopLength(music->context);
    }
    return -1;
}
double SDLCALLCC Mix_GetMusicLoopLengthTime(Mix_Music *music)
{
    double retval;

    Mix_LockAudio();
    if (music) {
        retval = music_internal_loop_length(music);
    } else if (music_playing) {
        retval = music_internal_loop_length(music_playing);
    } else {
        Mix_SetError("Music isn't playing");
        retval = -1.0;
    }
    Mix_UnlockAudio();

    return(retval);
}



/* Set the music's initial volume */
static void music_internal_initialize_volume(void)
{
    if (music_playing->fading == MIX_FADING_IN) {
        music_internal_volume(music_playing, 0);
    } else {
        music_internal_volume(music_playing, music_volume);
    }
}

/* Set the music volume */
static void music_internal_volume(Mix_Music *music, int volume)
{
    if (music->interface->SetVolume) {
        music->interface->SetVolume(music->context, volume);
    }
}
int SDLCALLCC Mix_VolumeMusicStream(Mix_Music *music, int volume)
{
    int prev_volume;

    /*TODO: put here "music->interface->GetVolume(music->context)" when implement*/
    prev_volume = music_volume;
    if (volume < 0) {
        return prev_volume;
    }
    if (volume > SDL_MIX_MAXVOLUME) {
        volume = SDL_MIX_MAXVOLUME;
    }
    music_volume = volume;
    Mix_LockAudio();
    if (music) {
        music_internal_volume(music, volume);
    } else if (music_playing) {
        music_internal_volume(music_playing, volume);
    }
    Mix_UnlockAudio();
    return(prev_volume);
}
int SDLCALLCC Mix_VolumeMusic(int volume)
{
    return Mix_VolumeMusicStream(NULL, volume);
}

/* Halt playing of music */
static void music_internal_halt(Mix_Music *music)
{
    if (music->interface->Stop) {
        music->interface->Stop(music->context);
    }

    music->playing = SDL_FALSE;
    music->fading = MIX_NO_FADING;
    if (music == music_playing) {
        music_playing = NULL;
    }
    /*TODO: For MultiMusic case, scan the "playing streams" array and remove the element from it*/
}
int SDLCALLCC Mix_HaltMusicStream(Mix_Music *music)
{
    Mix_LockAudio();
    if (music) {
        music_internal_halt(music);
        /* TODO: For MultiMusic: Implement the hook which allows to identify which music stream was halted
        if (music_stream_finished_hook) {
            music_stream_finished_hook(music);
        }
        */
    } else if (music_playing) {
        music_internal_halt(music_playing);
        if (music_finished_hook) {
            music_finished_hook();
        }
    }
    Mix_UnlockAudio();

    return(0);
}
int SDLCALLCC Mix_HaltMusic(void)
{
    return Mix_HaltMusicStream(NULL);
}

/* Progressively stop the music */
int SDLCALLCC Mix_FadeOutMusicStream(Mix_Music *music, int ms)
{
    int retval = 0;

    if (music == NULL) {
        music = music_playing;
    }

    if (ms_per_step == 0) {
        SDL_SetError("Audio device hasn't been opened");
        return 0;
    }

    if (ms <= 0) {  /* just halt immediately. */
        Mix_HaltMusicStream(music);
        return 1;
    }

    Mix_LockAudio();
    if (music) {
        int fade_steps = (ms + ms_per_step - 1) / ms_per_step;
        if (music->fading == MIX_NO_FADING) {
            music->fade_step = 0;
        } else {
            int step;
            int old_fade_steps = music->fade_steps;
            if (music->fading == MIX_FADING_OUT) {
                step = music->fade_step;
            } else {
                step = old_fade_steps - music->fade_step + 1;
            }
            music->fade_step = (step * fade_steps) / old_fade_steps;
        }
        music->fading = MIX_FADING_OUT;
        music->fade_steps = fade_steps;
        retval = 1;
    }
    Mix_UnlockAudio();

    return(retval);
}
int SDLCALLCC Mix_FadeOutMusic(int ms)
{
    return Mix_FadeOutMusicStream(NULL, ms);
}

Mix_Fading SDLCALLCC Mix_FadingMusicStream(Mix_Music *music)
{
    Mix_Fading fading = MIX_NO_FADING;

    Mix_LockAudio();
    if (music) {
        fading = music->fading;
    } else if (music_playing) {
        fading = music_playing->fading;
    }
    Mix_UnlockAudio();

    return(fading);
}
Mix_Fading SDLCALLCC Mix_FadingMusic(void)
{
    return Mix_FadingMusicStream(NULL);
}

/* Pause/Resume the music stream */
void SDLCALLCC Mix_PauseMusicStream(Mix_Music *music)
{
    Mix_LockAudio();
    if (music) {
        if (music->interface->Pause) {
            music->interface->Pause(music->context);
        }
    } else if (music_playing) {
        if (music_playing->interface->Pause) {
            music_playing->interface->Pause(music_playing->context);
        }
    }
    if (music == music_playing || music == NULL) {
        music_active = SDL_FALSE;
    }
    Mix_UnlockAudio();
}
void SDLCALLCC Mix_PauseMusic(void)
{
    return Mix_PauseMusicStream(NULL);
}

void SDLCALLCC Mix_ResumeMusicStream(Mix_Music *music)
{
    Mix_LockAudio();
    if (music) {
        if (music->interface->Resume) {
            music->interface->Resume(music->context);
        }
    } else if (music_playing) {
        if (music_playing->interface->Resume) {
            music_playing->interface->Resume(music_playing->context);
        }
    }
    if (music == music_playing || music == NULL) {
        music_active = SDL_TRUE;
    }
    Mix_UnlockAudio();
}
void SDLCALLCC Mix_ResumeMusic(void)
{
    return Mix_ResumeMusicStream(NULL);
}


void SDLCALLCC Mix_RewindMusicStream(Mix_Music *music)
{
    Mix_SetMusicStreamPosition(music, 0.0);
}
void SDLCALLCC Mix_RewindMusic(void)
{
    Mix_SetMusicStreamPosition(NULL, 0.0);
}

int SDLCALLCC Mix_PausedMusicStream(Mix_Music *music)
{
    MIX_UNUSED(music);
    /* TODO: For MultiMusic: Implement "IsPaused" call
    int isPaused = SDL_FALSE;
    if (music->interface->IsPaused) {
        isPaused = music->interface->IsPaused(music->context);
    }
    */
    return (music_active == SDL_FALSE);/*isPaused;*/
}

int SDLCALLCC Mix_PausedMusic(void)
{
    return (music_active == SDL_FALSE);
}

/* Check the status of the music */
static SDL_bool music_internal_playing(Mix_Music *music)
{
    if (!music) {
        return SDL_FALSE;
    }

    if (music->interface->IsPlaying) {
        music->playing = music->interface->IsPlaying(music->context);
    }
    return music->playing;
}
int SDLCALLCC Mix_PlayingMusicStream(Mix_Music *music)
{
    SDL_bool playing;

    Mix_LockAudio();
    if (music) {
        playing = music_internal_playing(music);
    } else {
        playing = music_internal_playing(music_playing);
    }
    Mix_UnlockAudio();

    return playing ? 1 : 0;
}
/* Deprecated call, kept for ABI compatibility */
int SDLCALLCC Mix_PlayingMusic(void)
{
    return Mix_PlayingMusicStream(NULL);
}

/* Set the external music playback command */
int SDLCALLCC Mix_SetMusicCMD(const char *command)
{
    Mix_HaltMusicStream(music_playing);
    /*TODO: For MultiMusic: Stop all music streams here*/
    if (music_cmd) {
        SDL_free(music_cmd);
        music_cmd = NULL;
    }
    if (command) {
        size_t length = SDL_strlen(command) + 1;
        music_cmd = (char *)SDL_malloc(length);
        if (music_cmd == NULL) {
            return SDL_OutOfMemory();
        }
        SDL_memcpy(music_cmd, command, length);
    }
    return 0;
}

int SDLCALLCC Mix_SetSynchroValue(int i)
{
    (void)i;
    /* Not supported by any players at this time */
    return(-1);
}

int SDLCALLCC Mix_GetSynchroValue(void)
{
    /* Not supported by any players at this time */
    return(-1);
}


/* Uninitialize the music interfaces */
void close_music(void)
{
    size_t i;

    Mix_HaltMusicStream(music_playing);

    for (i = 0; i < SDL_arraysize(s_music_interfaces); ++i) {
        Mix_MusicInterface *interface = s_music_interfaces[i];
        if (!interface || !interface->opened) {
            continue;
        }

        if (interface->Close) {
            interface->Close();
        }
        interface->opened = SDL_FALSE;
    }

    if (soundfont_paths) {
        SDL_free(soundfont_paths);
        soundfont_paths = NULL;
    }

    /* rcg06042009 report available decoders at runtime. */
    if (music_decoders) {
        SDL_free((void *)music_decoders);
        music_decoders = NULL;
    }
    num_decoders = 0;

    ms_per_step = 0;
}

/* Unload the music interface libraries */
void unload_music(void)
{
    size_t i;
    for (i = 0; i < SDL_arraysize(s_music_interfaces); ++i) {
        Mix_MusicInterface *interface = s_music_interfaces[i];
        if (!interface || !interface->loaded) {
            continue;
        }

        if (interface->Unload) {
            interface->Unload();
        }
        interface->loaded = SDL_FALSE;
    }
}

int SDLCALLCC Mix_SetSoundFonts(const char *paths)
{
    if (soundfont_paths) {
        SDL_free(soundfont_paths);
        soundfont_paths = NULL;
    }

    if (paths) {
        if (!(soundfont_paths = SDL_strdup(paths))) {
            Mix_SetError("Insufficient memory to set SoundFonts");
            return 0;
        }
    }
    return 1;
}

const char* SDLCALLCC Mix_GetSoundFonts(void)
{
    const char *env_paths = SDL_getenv("SDL_SOUNDFONTS");
    SDL_bool force_env_paths = SDL_GetHintBoolean("SDL_FORCE_SOUNDFONTS", SDL_FALSE);
    if (force_env_paths && (!env_paths || !*env_paths)) {
        force_env_paths = SDL_FALSE;
    }
    if (soundfont_paths && *soundfont_paths && !force_env_paths) {
        return soundfont_paths;
    }
    if (env_paths) {
        return env_paths;
    }

    /* We don't have any sound fonts set programmatically or in the environment
       Time to start guessing where they might be...
     */
    {
        static char *s_soundfont_paths[] = {
            "/usr/share/sounds/sf2/FluidR3_GM.sf2"  /* Remember to add ',' here */
        };
        unsigned i;

        for (i = 0; i < SDL_arraysize(s_soundfont_paths); ++i) {
            SDL_RWops *rwops = SDL_RWFromFile(s_soundfont_paths[i], "rb");
            if (rwops) {
                SDL_RWclose(rwops);
                return s_soundfont_paths[i];
            }
        }
    }
    return NULL;
}

int SDLCALLCC Mix_EachSoundFont(int (SDLCALL *function)(const char*, void*), void *data)
{
    char *context, *path, *paths;
    const char* cpaths = Mix_GetSoundFonts();
    int soundfonts_found = 0;

    if (!cpaths) {
        Mix_SetError("No SoundFonts have been requested");
        return 0;
    }

    if (!(paths = SDL_strdup(cpaths))) {
        Mix_SetError("Insufficient memory to iterate over SoundFonts");
        return 0;
    }

#if defined(_WIN32)
#define SEPARATOR ";"
#else
#define SEPARATOR ":;"
#endif
    for (path = Mix_strtok_safe(paths, SEPARATOR, &context);
         path;
         path = Mix_strtok_safe(NULL, SEPARATOR, &context))
    {
        if (!function(path, data)) {
            continue;
        } else {
            soundfonts_found++;
        }
    }
#undef SEPARATOR

    SDL_free(paths);
    if (soundfonts_found > 0)
        return 1;
    else
        return 0;
}


int SDLCALLCC Mix_GetMidiPlayer()
{
    return mididevice_current;
}

int SDLCALLCC Mix_GetNextMidiPlayer()
{
    return mididevice_current;
}

int SDLCALLCC Mix_SetMidiPlayer(int player)
{
    switch (player) {
        #ifdef MUSIC_MID_ADLMIDI
    case MIDI_ADLMIDI:
        #endif
        #ifdef MUSIC_MID_OPNMIDI
    case MIDI_OPNMIDI:
        #endif
        #ifdef MUSIC_MID_TIMIDITY
    case MIDI_Timidity:
        #endif
        #ifdef MUSIC_MID_NATIVE
    case MIDI_Native:
        #endif
        #ifdef MUSIC_MID_FLUIDSYNTH
    case MIDI_Fluidsynth:
        #endif
        mididevice_current = player;
        return 0;
    default:
        Mix_SetError("Unknown MIDI Device");
        return -1;
    }
}

void SDLCALLCC Mix_SetLockMIDIArgs(int lock_midiargs)
{
    mididevice_args_lock = lock_midiargs;
}

/* vi: set ts=4 sw=4 expandtab: */
