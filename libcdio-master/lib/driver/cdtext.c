/*
  Copyright (C) 2004-2005, 2008, 2011, 2012, 2013 Rocky Bernstein <rocky@gnu.org>
  toc reading routine adapted from cuetools
  Copyright (C) 2003 Svend Sanjay Sorensen <ssorensen@fastmail.fm>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef HAVE_CONFIG_H
# include "config.h"
# define __CDIO_CONFIG_H__ 1
#endif

#include <cdio/cdtext.h>
#include <cdio/logging.h>
#include "cdtext_private.h"
#include <cdio/utf8.h>

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#define _CDTEXT_DBCC
#define MAX_CDTEXT_GENRE_CODE     28
#define MAX_CDTEXT_LANGUAGE_CODE 127

const char *cdtext_field[MAX_CDTEXT_FIELDS] =
{
  "TITLE",
  "PERFORMER",
  "SONGWRITER",
  "COMPOSER",
  "MESSAGE",
  "ARRANGER",
  "ISRC",
  "UPC_EAN",
  "GENRE",
  "DISC_ID",
};

const char *cdtext_genre[MAX_CDTEXT_GENRE_CODE] =
{
  "Not Used",
  "Not Defined",
  "Adult Contemporary",
  "Alternative Rock",
  "Childrens Music",
  "Classical",
  "Contemporary Christian",
  "Country",
  "Dance",
  "Easy Listening",
  "Erotic",
  "Folk",
  "Gospel",
  "Hip Hop",
  "Jazz",
  "Latin",
  "Musical",
  "New Age",
  "Opera",
  "Operetta",
  "Pop Music",
  "Rap",
  "Reggae",
  "Rock Music",
  "Rhythm & Blues",
  "Sound Effects",
  "Spoken Word",
  "World Music"
};

const char *cdtext_language[MAX_CDTEXT_LANGUAGE_CODE] =
{
  "Unknown",
  "Albanian",
  "Breton",
  "Catalan",
  "Croatian",
  "Welsh",
  "Czech",
  "Danish",
  "German",
  "English",
  "Spanish",
  "Esperanto",
  "Estonian",
  "Basque",
  "Faroese",
  "French",
  "Frisian",
  "Irish",
  "Gaelic",
  "Galician",
  "Icelandic",
  "Italian",
  "Lappish",
  "Latin",
  "Latvian",
  "Luxembourgian",
  "Lithuanian",
  "Hungarian",
  "Maltese",
  "Dutch",
  "Norwegian",
  "Occitan",
  "Polish",
  "Portuguese",
  "Romanian",
  "Romansh",
  "Serbian",
  "Slovak",
  "Slovenian",
  "Finnish",
  "Swedish",
  "Turkish",
  "Flemish",
  "Wallon",
  "Zulu",
  "Vietnamese",
  "Uzbek",
  "Urdu",
  "Ukrainian",
  "Thai",
  "Telugu",
  "Tatar",
  "Tamil",
  "Tadzhik",
  "Swahili",
  "SrananTongo",
  "Somali",
  "Sinhalese",
  "Shona",
  "Serbo-croat",
  "Ruthenian",
  "Russian",
  "Russian",
  "Quechua",
  "Pushtu",
  "Punjabi",
  "Persian",
  "Papamiento",
  "Oriya",
  "Nepali",
  "Ndebele",
  "Marathi",
  "Moldavian",
  "Malaysian",
  "Malagasay",
  "Macedonian",
  "Laotian",
  "Korean",
  "Khmer",
  "Kazakh",
  "Kannada",
  "Japanese",
  "Indonesian",
  "Hindi",
  "Hebrew",
  "Hausa",
  "Gurani",
  "Gujurati",
  "Greek",
  "Georgian",
  "Fulani",
  "Dari",
  "Churash",
  "Chinese",
  "Burmese",
  "Bulgarian",
  "Bengali",
  "Bielorussian",
  "Bambora",
  "Azerbaijani",
  "Assamese",
  "Armenian",
  "Arabic",
  "Amharic"
};

/*!
  Return string representation of given field type.
*/
const char *
cdtext_field2str(cdtext_field_t i)
{
  if (i >= MAX_CDTEXT_FIELDS)
    return "INVALID";
  else
    return cdtext_field[i];
}

/*!
  Return string representation of the given genre code.
*/
const char *
cdtext_genre2str(cdtext_genre_t i)
{
  if (i >= MAX_CDTEXT_GENRE_CODE)
    return "INVALID";
  else
    return cdtext_genre[i];
}

/*!
  Return string representation of the given language code.
*/
const char *
cdtext_lang2str(cdtext_lang_t i)
{
  if (i >= MAX_CDTEXT_LANGUAGE_CODE)
    return "INVALID";
  else
    return cdtext_language[i];
}

/*!
  Free memory associated with the given cdtext_t object.

  @param p_cdtext the CD-TEXT object
*/
void
cdtext_destroy(cdtext_t *p_cdtext)
{
  cdtext_field_t k;
  track_t j;
  int i;

  if (!p_cdtext) return;
  for (i=0; i<CDTEXT_NUM_BLOCKS_MAX; i++) {
    for (j=0; j<CDTEXT_NUM_TRACKS_MAX; j++) {
      for (k=0; k < MAX_CDTEXT_FIELDS; k++) {
        if (p_cdtext->block[i].track[j].field[k]) {
          free(p_cdtext->block[i].track[j].field[k]);
          p_cdtext->block[i].track[j].field[k] = NULL;
        }
      }
    }
  }
}

/*!
  Returns a copy of the return value of cdtext_get_const or NULL.

  Must be freed using cdio_free() when done.
  @see cdtext_get_const
*/
char *
cdtext_get(const cdtext_t *p_cdtext, cdtext_field_t field, track_t track)
{
  const char *ret = cdtext_get_const(p_cdtext, field, track);
  if (NULL == ret)
    return NULL;
  else
    return strdup(ret);
}

/*!
  Returns value of the given field.

  NULL is returned if key is CDTEXT_INVALID or the field is not set.
  Strings are encoded in UTF-8.

  @param p_cdtext the CD-TEXT object
  @param field type of the field to return
  @param track specifies the track, 0 stands for disc
*/
const char *
cdtext_get_const(const cdtext_t *p_cdtext, cdtext_field_t field, track_t track)
{
  if (CDTEXT_FIELD_INVALID == field
      || NULL == p_cdtext
      || CDIO_CD_MAX_TRACKS < track)
    return NULL;

  return p_cdtext->block[p_cdtext->block_i].track[track].field[field];
}

/*!
  Returns the discs genre code.

  @param p_cdtext the CD-TEXT object
*/
cdtext_genre_t
cdtext_get_genre(const cdtext_t *p_cdtext)
{
  if (NULL == p_cdtext)
    return CDTEXT_GENRE_UNUSED;
  return p_cdtext->block[p_cdtext->block_i].genre_code;
}

/*!
  Returns the currently active language.

  @param p_cdtext the CD-TEXT object
*/
cdtext_lang_t
cdtext_get_language(const cdtext_t *p_cdtext)
{
  if (NULL == p_cdtext)
    return CDTEXT_LANGUAGE_UNKNOWN;
  return p_cdtext->block[p_cdtext->block_i].language_code;
}

/*
  Returns a list of available languages or NULL.

  Internally the list is stored in a static array.

  @param p_cdtext the CD-TEXT object
*/
cdtext_lang_t
*cdtext_list_languages(const cdtext_t *p_cdtext)
{
  static cdtext_lang_t avail[CDTEXT_NUM_BLOCKS_MAX];
  int i, j=0;

  if (NULL == p_cdtext)
    return NULL;

  for (i=0; i<CDTEXT_NUM_BLOCKS_MAX; i++)
  {
    avail[i] = CDTEXT_LANGUAGE_UNKNOWN;
    if (CDTEXT_LANGUAGE_UNKNOWN != p_cdtext->block[i].language_code)
      avail[j++] = p_cdtext->block[i].language_code;
  }

  return avail;
}

/*!
  Try to select the given language.
  Select default language if specified is not available or invalid and
  return false.

  @param p_cdtext the CD-TEXT object
  @param language language identifier

  @return true on success, false if language is not available
*/
bool
cdtext_select_language(cdtext_t *p_cdtext, cdtext_lang_t language)
{
  if(NULL == p_cdtext)
    return false;

  if (CDTEXT_LANGUAGE_UNKNOWN != language)
  {
    int i;
    for (i=0; i<CDTEXT_NUM_BLOCKS_MAX; i++) {
      if (language == p_cdtext->block[i].language_code) {
        p_cdtext->block_i = i;
        return true;
      }
    }
  } else {
    p_cdtext->block_i = 0;
  }
  return false;
}

/*!
  Initialize a new cdtext structure.

  When the structure is no longer needed, release the
  resources using cdtext_delete.

*/
cdtext_t
*cdtext_init(void)
{
  cdtext_field_t k;
  track_t j;
  int i;
  cdtext_t *p_cdtext;

  p_cdtext = (cdtext_t *) malloc(sizeof(struct cdtext_s));

  for (i=0; i<CDTEXT_NUM_BLOCKS_MAX; i++) {
    for (j=0; j<CDTEXT_NUM_TRACKS_MAX; j++) {
      for (k=0; k < MAX_CDTEXT_FIELDS; k++) {
        p_cdtext->block[i].track[j].field[k] = NULL;
      }
    }
    p_cdtext->block[i].genre_code = CDTEXT_GENRE_UNUSED;
    p_cdtext->block[i].language_code = CDTEXT_LANGUAGE_UNKNOWN;
  }

  p_cdtext->block_i = 0;

  return p_cdtext;
}

/*!
  Returns associated cdtext_field_t if field is a CD-TEXT keyword.

  Internal function.

  @param key key to test

  @return CDTEXT_INVALID if the given keyword is invalid
*/
cdtext_field_t
cdtext_is_field (const char *key)
{
  unsigned int i;

  for (i = 0; i < MAX_CDTEXT_FIELDS ; i++)
    if (0 == strcmp(cdtext_field[i], key)) {
      return i;
    }
  return CDTEXT_FIELD_INVALID;
}

/*!
  Returns associated cdtext_lang_t if argument is a supported language.

  Internal function.

  @param lang language to test

  @return CDTEXT_LANGUAGE_UNKNOWN if language is not supported
*/
cdtext_lang_t
cdtext_is_language(const char *lang)
{
  unsigned int i;

  for (i = 0; i < MAX_CDTEXT_LANGUAGE_CODE; i++)
    if (0 == strcmp(cdtext_language[i], lang)) {
      return i;
    }
  return CDTEXT_LANGUAGE_UNKNOWN;
}

/*!
  Sets the given field at the given track to the given value.

  Recodes to UTF-8 if charset is not NULL.

  @param p_cdtext the CD-TEXT object
  @param key field to set
  @param value value to set
  @param track track to work on
  @param charset charset to convert from
 */
void
cdtext_set(cdtext_t *p_cdtext, cdtext_field_t key, const uint8_t *value,
           track_t track, const char *charset)
{
  if (NULL == value || key == CDTEXT_FIELD_INVALID
      || CDIO_CD_MAX_TRACKS < track)
    return;

  /* free old memory */
  if (p_cdtext->block[p_cdtext->block_i].track[track].field[key])
    free(p_cdtext->block[p_cdtext->block_i].track[track].field[key]);

  /* recode to UTF-8 */
  if (NULL != charset) {
    cdio_utf8_t *utf8_str = NULL;
    cdio_charset_to_utf8((const char*) value, strlen((const char*)value),
                        &utf8_str, charset);
    p_cdtext->block[p_cdtext->block_i].track[track].field[key] = (char *)utf8_str;
  } else
    p_cdtext->block[p_cdtext->block_i].track[track].field[key] = strdup((const char *)value);
}

/*!
  Read a binary CD-TEXT and fill a cdtext struct.

  @param p_cdtext the CD-TEXT object
  @param wdata the data
  @param i_data size of wdata

  @returns 0 on success, non-zero on failure
*/
int
cdtext_data_init(cdtext_t *p_cdtext, uint8_t *wdata, size_t i_data)
{
  uint8_t       *p_data;
  int           j;
  uint8_t       buffer[256];
  int           i_buf = 0;
  int           i_block;
  int           i_seq = 0;
  int           i;
  cdtext_blocksize_t blocksize;
  char          *charset = NULL;

  memset( buffer, 0, sizeof(buffer) );

  p_data = wdata;
  if (i_data < CDTEXT_LEN_PACK || 0 != i_data % CDTEXT_LEN_PACK) {
    cdio_warn("CD-Text size is too small or not a multiple of pack size");
    return -1;
  }

#if 0
  for(i=0; i < i_data; i++)
    printf("%0x%c", wdata[i], ((i+1) % 18 == 0 ? '\n' : ' '));
#endif


  /* Iterate over blocks */
  i_block = -1;
  while(i_data > 0) {
    cdtext_pack_t pack;
    cdtext_read_pack(&pack, p_data);

    if (i_block != pack.block || i_seq != pack.seq) {
      cdtext_pack_t tpack;
      i_block = pack.block;
      if (i_block >= CDTEXT_NUM_BLOCKS_MAX) {
        cdio_warn("CD-TEXT: Invalid blocknumber %d.\n", i_block);
        return -1;
      }
      p_cdtext->block_i = i_block;
      i_seq = 0;
      memset( &blocksize, 0, CDTEXT_LEN_BLOCKSIZE);

      /* first read block size information for sanity checks and encoding */
      for(i=0; i <= i_data-CDTEXT_LEN_PACK; i+=CDTEXT_LEN_PACK) {

        if (p_data[i+0] == CDTEXT_PACK_BLOCKSIZE) {
          cdtext_read_pack(&tpack, p_data+i);
          switch (tpack.i_track) {
            case 0:
              blocksize.charcode      = tpack.text[0];
              blocksize.i_first_track = tpack.text[1];
              blocksize.i_last_track  = tpack.text[2];
              blocksize.copyright     = tpack.text[3];
              blocksize.i_packs[0]    = tpack.text[4];
              blocksize.i_packs[1]    = tpack.text[5];
              blocksize.i_packs[2]    = tpack.text[6];
              blocksize.i_packs[3]    = tpack.text[7];
              blocksize.i_packs[4]    = tpack.text[8];
              blocksize.i_packs[5]    = tpack.text[9];
              blocksize.i_packs[6]    = tpack.text[10];
              blocksize.i_packs[7]    = tpack.text[11];
              break;
            case 1:
              blocksize.i_packs[8]    = tpack.text[0];
              blocksize.i_packs[9]    = tpack.text[1];
              blocksize.i_packs[10]   = tpack.text[2];
              blocksize.i_packs[11]   = tpack.text[3];
              blocksize.i_packs[12]   = tpack.text[4];
              blocksize.i_packs[13]   = tpack.text[5];
              blocksize.i_packs[14]   = tpack.text[6];
              blocksize.i_packs[15]   = tpack.text[7];
              blocksize.lastseq[0]    = tpack.text[8];
              blocksize.lastseq[1]    = tpack.text[9];
              blocksize.lastseq[2]    = tpack.text[10];
              blocksize.lastseq[3]    = tpack.text[11];
              break;
            case 2:
              blocksize.lastseq[4]    = tpack.text[0];
              blocksize.lastseq[5]    = tpack.text[1];
              blocksize.lastseq[6]    = tpack.text[2];
              blocksize.lastseq[7]    = tpack.text[3];
              blocksize.langcode[0]   = tpack.text[4];
              blocksize.langcode[1]   = tpack.text[5];
              blocksize.langcode[2]   = tpack.text[6];
              blocksize.langcode[3]   = tpack.text[7];
              blocksize.langcode[4]   = tpack.text[8];
              blocksize.langcode[5]   = tpack.text[9];
              blocksize.langcode[6]   = tpack.text[10];
              blocksize.langcode[7]   = tpack.text[11];
              break;
          }
        }
      }

      if(blocksize.i_packs[15] == 3) {
        /* if there were 3 BLOCKSIZE packs */
        /* set copyright */
        p_cdtext->block[i_block].copyright = (0x03 == (blocksize.copyright & 0x03));

        /* set Language */
        if(blocksize.langcode[i_block] <= 0x7f)
          p_cdtext->block[i_block].language_code = blocksize.langcode[i_block];

        /* determine encoding */
        switch (blocksize.charcode){
          case CDTEXT_CHARCODE_ISO_8859_1:
            /* default */
            charset = (char *) "ISO-8859-1";
            break;
          case CDTEXT_CHARCODE_ASCII:
            charset = (char *) "ASCII";
            break;
          case CDTEXT_CHARCODE_SHIFT_JIS:
            charset = (char *) "SHIFT_JIS";
            break;
        }
      } else {
        cdio_warn("CD-TEXT: No blocksize information available for block %d.\n", i_block);
        return -1;
      }

    }

    cdtext_read_pack(&pack, p_data);

#ifndef _CDTEXT_DBCC
    if ( pack.db_chars ) {
      cdio_warn("CD-TEXT: Double-byte characters not supported");
      return -1;
    }
#endif

    /* read text packs first */
    j = 0;
    switch (pack.type) {
      case CDTEXT_PACK_GENRE:
        /* If pack.text starts with an unprintable character, it is likely to be the genre_code.
         * While the specification requires the first GENRE pack to start with the 2 byte genre code,
         * it is not specific about the following ones. */
        if (pack.text[0] <= 31) {
          j = 2;
          if (CDTEXT_GENRE_UNUSED == p_cdtext->block[i_block].genre_code)
            p_cdtext->block[i_block].genre_code = CDTEXT_GET_LEN16(pack.text);
        }
      case CDTEXT_PACK_TITLE:
      case CDTEXT_PACK_PERFORMER:
      case CDTEXT_PACK_SONGWRITER:
      case CDTEXT_PACK_COMPOSER:
      case CDTEXT_PACK_ARRANGER:
      case CDTEXT_PACK_MESSAGE:
      case CDTEXT_PACK_DISCID:
      case CDTEXT_PACK_UPC:
        while (j < CDTEXT_LEN_TEXTDATA) {
          /* not terminated */
          if (pack.text[j] != 0 || (pack.db_chars && pack.text[j+1] != 0)) {
            buffer[i_buf++] = pack.text[j];
            if(pack.db_chars)
              buffer[i_buf++] = pack.text[j+1];
          } else if(i_buf > 1) {
            buffer[i_buf++] = 0;
            if(pack.db_chars)
              buffer[i_buf++] = 0;

            switch (pack.type) {
              case CDTEXT_PACK_TITLE:
                cdtext_set(p_cdtext, CDTEXT_FIELD_TITLE, buffer, pack.i_track, charset);
                break;
              case CDTEXT_PACK_PERFORMER:
                cdtext_set(p_cdtext, CDTEXT_FIELD_PERFORMER, buffer, pack.i_track, charset);
                break;
              case CDTEXT_PACK_SONGWRITER:
                cdtext_set(p_cdtext, CDTEXT_FIELD_SONGWRITER, buffer, pack.i_track, charset);
                break;
              case CDTEXT_PACK_COMPOSER:
                cdtext_set(p_cdtext, CDTEXT_FIELD_COMPOSER, buffer, pack.i_track, charset);
                break;
              case CDTEXT_PACK_ARRANGER:
                cdtext_set(p_cdtext, CDTEXT_FIELD_ARRANGER, buffer, pack.i_track, charset);
                break;
              case CDTEXT_PACK_MESSAGE:
                cdtext_set(p_cdtext, CDTEXT_FIELD_MESSAGE, buffer, pack.i_track, charset);
                break;
              case CDTEXT_PACK_DISCID:
                if (pack.i_track == 0)
                  cdtext_set(p_cdtext, CDTEXT_FIELD_DISCID, buffer, pack.i_track, NULL);
                break;
              case CDTEXT_PACK_GENRE:
                cdtext_set(p_cdtext, CDTEXT_FIELD_GENRE, buffer, pack.i_track, "ASCII");
                break;
              case CDTEXT_PACK_UPC:
                if (pack.i_track == 0)
                  cdtext_set(p_cdtext, CDTEXT_FIELD_UPC_EAN, buffer, pack.i_track, "ASCII");
                else
                  cdtext_set(p_cdtext, CDTEXT_FIELD_ISRC, buffer, pack.i_track, "ISO-8859-1");
                break;
            }
            i_buf = 0;

          }
          if (pack.db_chars)
            j+=2;
          else
            j+=1;
        }
        break;
    }
    /* This would be the right place to parse TOC and TOC2 fields. */

    i_seq++;
    i_data-=CDTEXT_LEN_PACK;
    p_data+=CDTEXT_LEN_PACK;
  } /* end of while loop */

  p_cdtext->block_i = 0;
  return 0;
}


/*!
  Fills cdtext_pack_t with information read from p_data

  @param p_pack out
  @param p_data in
*/
int
cdtext_read_pack(cdtext_pack_t *p_pack, const uint8_t *p_data) {
  p_pack->type     = p_data[0];
  p_pack->i_track  = p_data[1];
  p_pack->seq      = p_data[2];
  p_pack->char_pos = p_data[3]        & 0x0F;
  p_pack->block    = (p_data[3] >> 4) & 0x07;
  p_pack->db_chars = (p_data[3] >> 7) & 0x01;
  p_pack->text[0]  = p_data[4];
  p_pack->text[1]  = p_data[5];
  p_pack->text[2]  = p_data[6];
  p_pack->text[3]  = p_data[7];
  p_pack->text[4]  = p_data[8];
  p_pack->text[5]  = p_data[9];
  p_pack->text[6]  = p_data[10];
  p_pack->text[7]  = p_data[11];
  p_pack->text[8]  = p_data[12];
  p_pack->text[9]  = p_data[13];
  p_pack->text[10] = p_data[14];
  p_pack->text[11] = p_data[15];
  p_pack->crc[0]   = p_data[16];
  p_pack->crc[1]   = p_data[17];

  return 0;
}
