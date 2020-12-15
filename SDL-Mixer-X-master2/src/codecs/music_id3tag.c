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

#include "music_id3tag.h"
#include "mixer.h"
#ifdef MUSIC_MP3_ID3TAG

#include "id3tag.h"

static void parse_frame(struct id3_tag  *tag, const char *tag_key, Mix_MusicMetaTags *out_tags, Mix_MusicMetaTag tag_type)
{
    struct id3_frame *pFrame;
    pFrame = id3_tag_findframe(tag, tag_key, 0);

    if (pFrame != NULL) {
        union id3_field field = pFrame->fields[1];
        id3_ucs4_t const *pTemp = NULL;
        id3_utf8_t *pStrUtf8 = NULL;

        if (field.type == ID3_FIELD_TYPE_STRING) {
            pTemp = id3_field_getstring(&field);
        } else if (field.type == ID3_FIELD_TYPE_STRINGLIST) {
            pTemp = id3_field_getstrings(&field, 0);
        }

        if(pTemp != NULL) {
            pStrUtf8 = id3_ucs4_utf8duplicate(pTemp);
            meta_tags_set(out_tags, tag_type, (const char*)pStrUtf8);
            free(pStrUtf8);
        }
    }
}

static void parse_xxx_frame(struct id3_tag  *tag, const char *arg, Mix_MusicMetaTags *out_tags, Mix_MusicMetaTag tag_type)
{
    struct id3_frame *pFrame;
    pFrame = id3_tag_findframe(tag, "TXXX", 0);

    if (pFrame != NULL) {
        unsigned int i, found = 0;

        for (i = 1; i < pFrame->nfields; i++) {
            union id3_field field = pFrame->fields[i];
            id3_ucs4_t const *pTemp = NULL;
            id3_utf8_t *pStrUtf8 = NULL;

            if (field.type == ID3_FIELD_TYPE_STRING) {
                pTemp = id3_field_getstring(&field);
            } else if (field.type == ID3_FIELD_TYPE_STRINGLIST) {
                pTemp = id3_field_getstrings(&field, 0);
            }

            if (pTemp != NULL) {
                pStrUtf8 = id3_ucs4_utf8duplicate(pTemp);
                if (found) {
                    meta_tags_set(out_tags, tag_type, (const char*)pStrUtf8);
                    found = 1;
                } else {
                    if (SDL_strcmp((const char*)pStrUtf8, arg) == 0) {
                        found = 1;
                    }
                }
                free(pStrUtf8);
            }
        }
    }
}

Sint64 id3tag_fetchTags(Mix_MusicMetaTags *out_tags, SDL_RWops *src)
{
    struct id3_file *tags;
    Sint64 src_begin_pos = 0;

    tags = id3_file_from_rwops(src, ID3_FILE_MODE_READONLY);

    if (tags) {
        struct id3_tag  *tag = id3_file_tag(tags);

        /* Attempt to escape a bug of MAD that causes a junk begin on some MP3 files */
        src_begin_pos = (Sint64)tag->paddedsize;

        /* Search for given frame by frame id */
        parse_frame(tag, ID3_FRAME_TITLE, out_tags, MIX_META_TITLE);
        parse_frame(tag, ID3_FRAME_ARTIST, out_tags, MIX_META_ARTIST);
        parse_frame(tag, ID3_FRAME_ALBUM, out_tags, MIX_META_ALBUM);
        parse_frame(tag, "TCOP", out_tags, MIX_META_COPYRIGHT);
        parse_xxx_frame(tag, "Copyright message", out_tags, MIX_META_COPYRIGHT);
        id3_file_close(tags);

        SDL_RWseek(src, src_begin_pos, RW_SEEK_SET);
    }

    return src_begin_pos;
}

Sint64 id3tag_fetchTagsFromMemory(Mix_MusicMetaTags *out_tags, Uint8 *data, size_t length)
{
    SDL_RWops *src = SDL_RWFromConstMem(data, (int)length);
    if (src) {
        Sint64 ret = id3tag_fetchTags(out_tags, src);
        SDL_RWclose(src);
        return ret;
    }
    return 0;
}

#else
Sint64 id3tag_fetchTags(Mix_MusicMetaTags *out_tags, SDL_RWops *src)
{
    (void)out_tags;
    (void)src;
    return 0;
}

Sint64 id3tag_fetchTagsFromMemory(Mix_MusicMetaTags *out_tags, Uint8 *data, size_t length)
{
    (void)out_tags;
    (void)data;
    (void)length;
    return 0;
}
#endif

