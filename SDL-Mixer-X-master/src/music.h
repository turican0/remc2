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
#include "SDL_mixer_ext.h"


#ifndef MUSIC_H_
#define MUSIC_H_

/* Prevent "Unused" warning on specified object */
#define MIX_UNUSED(object) (void)object

/* math.h doesn't defines round() in C90 standard */
#if !defined(__STDC_VERSION__) || (__STDC_VERSION__ < 199901L)
extern double round(double __x);
#endif


/* Supported music APIs, in order of preference */

typedef enum
{
    MIX_MUSIC_CMD,
    MIX_MUSIC_WAVE,
    MIX_MUSIC_MODPLUG,
    MIX_MUSIC_MIKMOD,
    MIX_MUSIC_FLUIDSYNTH,
    MIX_MUSIC_TIMIDITY,
    MIX_MUSIC_NATIVEMIDI,
    MIX_MUSIC_OGG,
    MIX_MUSIC_MPG123,
    MIX_MUSIC_MAD,
    MIX_MUSIC_SMPEG,
    MIX_MUSIC_FLAC,
    MIX_MUSIC_OPUS,
    MIX_MUSIC_GME,/*MIXER-X*/
    MIX_MUSIC_ADLMIDI,/*MIXER-X*/
    MIX_MUSIC_OPNMIDI,/*MIXER-X*/
    MIX_MUSIC_LAST
} Mix_MusicAPI;


/* MIXER-X: Supported meta-tags */

typedef enum
{
    MIX_META_TITLE,
    MIX_META_ARTIST,
    MIX_META_ALBUM,
    MIX_META_COPYRIGHT,
    MIX_META_LAST
} Mix_MusicMetaTag;


/* MIXER-X: Meta-tags utility structure */

typedef struct {
    char *tags[4];
} Mix_MusicMetaTags;


extern void meta_tags_init(Mix_MusicMetaTags *tags);
extern void meta_tags_clear(Mix_MusicMetaTags *tags);
extern void meta_tags_set(Mix_MusicMetaTags *tags, Mix_MusicMetaTag type, const char *value);
extern const char* meta_tags_get(Mix_MusicMetaTags *tags, Mix_MusicMetaTag type);


/* Music API implementation */

typedef struct
{
    const char *tag;
    Mix_MusicAPI api;
    Mix_MusicType type;
    SDL_bool loaded;
    SDL_bool opened;

    /* Load the library */
    int (*Load)(void);

    /* Initialize for the audio output */
    int (*Open)(const SDL_AudioSpec *spec);

    /* Create a music object from an SDL_RWops stream
     * If the function returns NULL, 'src' will be freed if needed by the caller.
     */
    void *(*CreateFromRW)(SDL_RWops *src, int freesrc);

    /* MIXER-X
     * Create a music object from an SDL_RWops stream
     * If the function returns NULL, 'src' will be freed if needed by the caller.
     * 'extra_settings' will contain a string with extra setup supported by the codec.
     */
    void *(*CreateFromRWex)(SDL_RWops *src, int freesrc, const char *extra_settings);

    /* Create a music object from a file, if SDL_RWops are not supported */
    void *(*CreateFromFile)(const char *file);

    /* MIXER-X
     * Create a music object from a file, if SDL_RWops are not supported.
     * With support of extra settings
     */
    void *(*CreateFromFileEx)(const char *file, const char *extra_settings);

    /* Set the volume */
    void (*SetVolume)(void *music, int volume);

    /* Start playing music from the beginning with an optional loop count */
    int (*Play)(void *music, int play_count);

    /* Returns SDL_TRUE if music is still playing */
    SDL_bool (*IsPlaying)(void *music);

    /* Get music data, returns the number of bytes left */
    int (*GetAudio)(void *music, void *data, int bytes);

    /* Seek to a play position (in seconds) */
    int (*Seek)(void *music, double position);

    /* MIXER-X: Tell a play position (in seconds) */
    double (*Tell)(void *music);

    /* MIXER-X: Tell a total length of a track (in seconds) */
    double (*FullLength)(void *music);

    /* MIXER-X: Tell a loop start position (in seconds) */
    double (*LoopStart)(void *music);

    /* MIXER-X: Tell a loop end position (in seconds) */
    double (*LoopEnd)(void *music);

    /* MIXER-X: Tell a loop length position (in seconds) */
    double (*LoopLength)(void *music);

    /* MIXER-X: Get a meta-tag string if available */
    const char* (*GetMetaTag)(void *music, Mix_MusicMetaTag tag_type);

    /* Pause playing music */
    void (*Pause)(void *music);

    /* Resume playing music */
    void (*Resume)(void *music);

    /* Stop playing music */
    void (*Stop)(void *music);

    /* Delete a music object */
    void (*Delete)(void *music);

    /* Close the library and clean up */
    void (*Close)(void);

    /* Unload the library */
    void (*Unload)(void);

} Mix_MusicInterface;


extern int get_num_music_interfaces(void);
extern Mix_MusicInterface *get_music_interface(int index);
extern Mix_MusicType detect_music_type_from_magic(SDL_RWops *src);
extern SDL_bool load_music_type(Mix_MusicType type);
extern SDL_bool open_music_type(Mix_MusicType type);
extern SDL_bool open_music_type_ex(Mix_MusicType type, int midi_device);
extern SDL_bool has_music(Mix_MusicType type);
extern void open_music(const SDL_AudioSpec *spec);
extern int music_pcm_getaudio(void *context, void *data, int bytes, int volume,
                              int (*GetSome)(void *context, void *data, int bytes, SDL_bool *done));
extern void SDLCALL music_mixer(void *udata, Uint8 *stream, int len);
extern void close_music(void);
extern void unload_music(void);

extern char *music_cmd;
extern SDL_AudioSpec music_spec;
extern int mididevice_current;

/* Saafe strtok implementation */
extern char *Mix_strtok_safe(char *str, const char *delim, char **nextp);

#endif /* MUSIC_H_ */

/* vi: set ts=4 sw=4 expandtab: */
