/*
  Copyright (C) 2004-2005, 2008, 2011-2013
   Rocky Bernstein <rocky@gnu.org>

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

#ifndef CDIO_DRIVER_CDTEXT_PRIVATE_H_
#define CDIO_DRIVER_CDTEXT_PRIVATE_H_

#include <cdio/types.h>

#ifndef CDIO_CD_MAX_TRACKS
# define CDIO_CD_MAX_TRACKS 99 /* Largest CD track number */
#endif

#define CDTEXT_GET_LEN16(p) (p[0]<<8) + p[1]


typedef enum {
  CDTEXT_LEN_BINARY_MAX     = 9216,
  CDTEXT_LEN_TEXTDATA       = 12,
  CDTEXT_LEN_PACK           = 18,
  CDTEXT_LEN_BLOCKSIZE      = 36,
  CDTEXT_NUM_BLOCKS_MAX     = 8,
  CDTEXT_NUM_TRACKS_MAX     = CDIO_CD_MAX_TRACKS+1, /* +1 for 0th disk track */
  CDTEXT_NUM_BLOCKPACKS_MAX = 255
} cdtext_format_enum_t;

/**
 * From table J.2 - Pack Type Indicator Definitions from
 * Working Draft NCITS XXX T10/1364-D Revision 10G. November 12, 2001.
 */
typedef enum {
  CDTEXT_PACK_TITLE         = 0x80,
  CDTEXT_PACK_PERFORMER     = 0x81,
  CDTEXT_PACK_SONGWRITER    = 0x82,
  CDTEXT_PACK_COMPOSER      = 0x83,
  CDTEXT_PACK_ARRANGER      = 0x84,
  CDTEXT_PACK_MESSAGE       = 0x85,
  CDTEXT_PACK_DISCID        = 0x86,
  CDTEXT_PACK_GENRE         = 0x87,
  CDTEXT_PACK_TOC           = 0x88,
  CDTEXT_PACK_TOC2          = 0x89,
  CDTEXT_PACK_UPC           = 0x8E,
  CDTEXT_PACK_BLOCKSIZE     = 0x8F
} cdtext_packtype_enum_t;


/** CD-Text character encodings */
typedef enum cdtext_charcode_enum_s {
  CDTEXT_CHARCODE_ISO_8859_1 = 0x00, /**< ISO-8859-1 (8 bit), Latin-1 */
  CDTEXT_CHARCODE_ASCII      = 0x01, /**< ASCII (7 bit) */
  CDTEXT_CHARCODE_SHIFT_JIS  = 0x80  /**< Shift_JIS (double byte), JIS X 0208 Appendix 1 */
///* The following were proposed but never implemented anywhere.
// * They are mentioned for completeness here
// *  CDTEXT_CHARCODE_KOREAN     = 0x81, /**< Korean */
// *  CDTEXT_CHARCODE_CHINESE    = 0x82, /**< Mandarin Chinese */
// *  CDTEXT_CHARCODE_UNDEFINED  = 0xFF, /**< everything else */
// */

} cdtext_charcode_enum_t;

/** Structure of CD-TEXT data Packs */
struct cdtext_pack_s
{
  uint8_t  type;
  uint8_t  i_track;
  uint8_t  seq;
  uint8_t  char_pos;  /* character position */
  uint8_t  block;     /* block number 0..7 */
  uint8_t  db_chars;  /* double byte character */
  uint8_t  text[CDTEXT_LEN_TEXTDATA];
  uint8_t  crc[2];
};


/** Structure of of block size information packs */
struct cdtext_blocksize_s
{
  uint8_t charcode;      /* character code */
  uint8_t i_first_track; /* first track number */
  uint8_t i_last_track;  /* last track number */
  uint8_t copyright;     /* 3 CD-TEXT is copyrighted, 0 no copyright on CD-TEXT */
  uint8_t i_packs[16];   /* number of packs of each type
                          * 0 TITLE; 1 PERFORMER; 2 SONGWRITER; 3 COMPOSER;
                          * 4 ARRANGER; 5 MESSAGE; 6 DISCID; 7 GENRE;
                          * 8 TOC; 9 TOC2; 10-12 RESERVED; 13 CLOSED;
                          * 14 UPC_ISRC; 15 BLOCKSIZE */
  uint8_t lastseq[8];    /* last sequence for block 0..7 */
  uint8_t langcode[8];   /* language code for block 0..7 */
};

typedef struct cdtext_pack_s cdtext_pack_t;
typedef struct cdtext_blocksize_s cdtext_blocksize_t;

/*! Structure for CD-TEXT of a track. */
struct cdtext_track_s {
  char *field[MAX_CDTEXT_FIELDS];
};

/*! Structure for CD-TEXT of a block. */
struct cdtext_block_s {
  struct cdtext_track_s track[CDTEXT_NUM_TRACKS_MAX]; /**< 0: disc; 1..99: tracks */
  cdtext_genre_t genre_code;                          /**< genre code of the disc */
  cdtext_lang_t  language_code;                       /**< language of this block */
  bool           copyright;                           /**< CD-TEXT copyright      */
};

/*! Structure for CD-TEXT of a disc.

  @see cdtext_init, cdtext_destroy, cdtext_get, and cdtext_set.
 */
struct cdtext_s {
  struct cdtext_block_s block[CDTEXT_NUM_BLOCKS_MAX]; /**< CD-TEXT for block 0..7 */
  uint8_t  block_i;                                   /**< index of active block  */
};

int cdtext_read_pack (cdtext_pack_t *pack, const uint8_t *data);

/*!
  returns enum of field if key is a CD-Text keyword,
  returns CDTEXT_FIELD_INVALID otherwise.
*/
cdtext_field_t cdtext_is_field (const char *field);

/*!
  returns enum of language if lang is a valid language,
  returns CDTEXT_LANGUAGE_UNKNOWN otherwise.
*/
cdtext_lang_t cdtext_is_language (const char *lang);


#endif /* CDIO_DRIVER_CDTEXT_PRIVATE_H_ */

/*
 * Local variables:
 *  c-file-style: "gnu"
 *  tab-width: 8
 *  indent-tabs-mode: nil
 * End:
 */
