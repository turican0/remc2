/* -*- C -*-
  Copyright (C) 2004, 2006, 2008, 2010, 2011, 2012, 2017
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

/*
   Example demonstrating the parsing of raw CD-Text files
   Output adapted from cd-info.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#define __CDIO_CONFIG_H__ 1
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#include <cdio/cdio.h>

#define CDTEXT_LEN_BINARY_MAX 9216

static void
print_cdtext_track_info(cdtext_t *p_cdtext, track_t i_track, const char *psz_msg) {

  if (NULL != p_cdtext) {
    cdtext_field_t i;

    printf("%s\n", psz_msg);

    for (i=0; i < MAX_CDTEXT_FIELDS; i++) {
      if (cdtext_get_const(p_cdtext, i, i_track)) {
        printf("\t%s: %s\n", cdtext_field2str(i), cdtext_get_const(p_cdtext, i, i_track));
      }
    }
  }
}

static void
print_cdtext_info(cdtext_t *p_cdtext) {
  track_t i_first_track = 0;
  track_t i_last_track = 0;
  cdtext_lang_t *languages;
  cdtext_genre_t genre;

  int i, j;

  languages = cdtext_list_languages(p_cdtext);
  for(i=0; i<8; i++)
    if ( CDTEXT_LANGUAGE_UNKNOWN != languages[i]
         && cdtext_select_language(p_cdtext, languages[i]))
    {
      printf("\nLanguage %d '%s':\n", i, cdtext_lang2str(languages[i]));

      print_cdtext_track_info(p_cdtext, 0, "CD-TEXT for Disc:");
      genre = cdtext_get_genre(p_cdtext);
      if ( CDTEXT_GENRE_UNUSED != genre)
        printf("\tGENRE_CODE: %d (%s)\n", genre, cdtext_genre2str(genre));

      i_first_track = cdtext_get_first_track(p_cdtext);
      i_last_track = cdtext_get_last_track(p_cdtext);

      for ( j = i_first_track ; j <= i_last_track; j++ ) {
        char msg[50];
        sprintf(msg, "CD-TEXT for Track %2d:", j);
        print_cdtext_track_info(p_cdtext, j, msg);
      }
    }
}

static cdtext_t *
read_cdtext(const char *path) {
  FILE *fp;
  size_t size;
  uint8_t cdt_data[CDTEXT_LEN_BINARY_MAX+4];
  cdtext_t *cdt;

  fp = fopen(path, "rb");
  if (NULL == fp) {
    fprintf(stderr, "could not open file `%s'\n", path);
    exit(3);
  }

  size = fread(cdt_data, sizeof(uint8_t), sizeof(cdt_data), fp);
  fclose(fp);

  if (size < 5) {
    fprintf(stderr, "file `%s' is too small to contain CD-TEXT\n", path);
    exit(1);
  }

  /* Truncate header when it is too large. The standard is ambiguous here*/
  if (cdt_data[0] > 0x80) {
    size -= 4;
  }

  /* ignore trailing 0 */
  if (1 == size % 18)
    size -= 1;

  /* init cdtext */
  cdt = cdtext_init ();
  if(0 != cdtext_data_init(cdt, cdt_data, size)) {
    fprintf(stderr, "failed to parse CD-Text file `%s'\n", path);
    free(cdt);
    exit(2);
  }

  return cdt;
}

int
main(int argc, const char *argv[]) {
  cdtext_t *cdt;

  char *cdt_path = NULL;

  if (argc > 1) {
    cdt_path = (char *) argv[1];
  }else{
    fprintf(stderr, "no CD-Text file given\n");
    exit(77);
  }

  cdt = read_cdtext(cdt_path);
  print_cdtext_info(cdt);
  free(cdt);

  return 0;
}
