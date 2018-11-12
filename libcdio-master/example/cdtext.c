/*
  Copyright (C) 2004, 2005, 2006, 2008, 2009, 2011, 2012
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

/* Simple program to list CD-Text info of a Compact Disc using
   libcdio.  See also corresponding C++ programs of similar names. */

#define EXAMPLE_CUE_FILE "../test/data/cdtext.cue"
#define EXAMPLE_PREF_LANG CDTEXT_LANGUAGE_GERMAN

#ifdef HAVE_CONFIG_H
#include "config.h"
#define __CDIO_CONFIG_H__ 1
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

#include <cdio/cdio.h>

static void 
print_cdtext_track_info(cdtext_t *cdtext, track_t i_track) {
    cdtext_field_t i;
    
    for (i=0; i < MAX_CDTEXT_FIELDS; i++) {
        if (cdtext_get_const(cdtext, i, i_track)) {
            printf("-- \t%s: %s\n", cdtext_field2str(i),
                   cdtext_get_const(cdtext, i, i_track));
        }
    }
    
}

static void 
print_disc_info(CdIo_t *p_cdio) {
    track_t i_first_track = cdio_get_first_track_num (p_cdio);
    track_t i_tracks = cdio_get_num_tracks (p_cdio);
    track_t i_last_track = i_first_track + i_tracks;
    discmode_t cd_discmode = cdio_get_discmode (p_cdio);
    cdtext_t *cdtext = cdio_get_cdtext (p_cdio);
    int i;
    
    printf("-- Discmode: %s\n\n", discmode2str[cd_discmode]);
    
    if (NULL == cdtext)
    {
        printf("-- No CD-Text found on Disc.\n");
        return;
    }

    /* print available languages */
    {
        cdtext_lang_t *languages;

        printf("-- CD-Text available in: ");

        languages = cdtext_list_languages(cdtext);
        for(i=0; i<8; i++)
            if ( CDTEXT_LANGUAGE_UNKNOWN != languages[i])
                printf("%s ", cdtext_lang2str(languages[i]));
        printf("\n");
    }

    /* select language */
    if(cdtext_select_language(cdtext, EXAMPLE_PREF_LANG)) {
        printf("-- %s selected.\n", cdtext_lang2str (EXAMPLE_PREF_LANG));
    } else {
        printf("-- '%s' is not available. Using '%s'\n",
               cdtext_lang2str (EXAMPLE_PREF_LANG),
               cdtext_lang2str (cdtext_get_language (cdtext)));
    }

    /* print cd-text */
    printf("-- CD-Text for Disc:\n");
    print_cdtext_track_info(cdtext, 0);
    for (i=i_first_track ; i < i_last_track; i++ ) {
        printf("-- CD-Text for Track %d:\n", i);
        print_cdtext_track_info(cdtext, i);
    }
}

int
main(int argc, const char *argv[])
{
    CdIo_t *p_cdio;
    
    /* read CD-Text from a bin/cue (CDRWIN) image */
    p_cdio = cdio_open(EXAMPLE_CUE_FILE, DRIVER_BINCUE);
    if (NULL == p_cdio) {
        printf("Couldn't open %s with BIN/CUE driver.\n", 
               EXAMPLE_CUE_FILE);
    } else {
        print_disc_info(p_cdio);
        cdio_destroy(p_cdio);
    }
    
    /* read CD-Text from default device */
    p_cdio = cdio_open (NULL, DRIVER_DEVICE);
    if (NULL == p_cdio) {
        printf("Couldn't find CD\n");
        return 77;
    } else {
        print_disc_info(p_cdio);
        cdio_destroy(p_cdio);
    }
    
    return 0;
}
