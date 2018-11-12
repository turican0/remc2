/*
  Copyright (C) 2005, 2008, 2009, 2012 Rocky Bernstein <rocky@gnu.org>
  
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
   libcdio.  An optional drive name can be supplied as an argument.
   See also corresponding C program of a similar name.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#define __CDIO_CONFIG_H__ 1
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

#include <cdio++/cdio.hpp>

/* Set up a CD-DA image to test on which is in the libcdio distribution. */
#define CDDA_IMAGE "../../../test/data/cdtext.cue"

static void 
print_cdtext_track_info(CdioCDText *cdtext, track_t i_track) {
  cdtext_field_t i;

  for (i=(cdtext_field_t) 0; i < MAX_CDTEXT_FIELDS; i++) {
    if (cdtext->getConst(i, i_track)) {
      printf("\t%s: %s\n", cdtext->field2str(i),
             cdtext->getConst(i, i_track));
    }
  }
}
    
static void 
print_disc_info(CdioDevice *device)
{
  track_t i_first_track = device->getFirstTrackNum();
  track_t i_tracks = device->getNumTracks();
  track_t i_last_track = i_first_track+i_tracks;
  discmode_t cd_discmode = device->getDiscmode();
  CdioCDText *cdtext = device->getCdtext();
  int i;

  printf("Discmode: %s\n\n", discmode2str[cd_discmode]);

  if (NULL == cdtext)
  {
    printf("No CD-Text found on Disc.\n");
    return;
  }

  /* print available languages */
  {
    cdtext_lang_t *languages;

    printf("CD-Text available in: ");

    languages = cdtext->listLanguages();
    for(i=0; i<8; i++)
      if ( CDTEXT_LANGUAGE_UNKNOWN != languages[i])
          printf("%s ", cdtext->lang2str(languages[i]));
    printf("\n");
  }

  printf("CD-Text for Disc:\n");
  print_cdtext_track_info(cdtext, 0);
  for (i = i_first_track ; i < i_last_track; i++ ) {
    printf("CD-Text for Track %d:\n", i);
    print_cdtext_track_info(cdtext, i);
  }

  delete cdtext;
}

int
main(int argc, const char *argv[])
{
  CdioDevice *device = new CdioDevice;
  const char *psz_drive = (const char *) NULL;

  if (!device->open(CDDA_IMAGE, DRIVER_BINCUE)) {
    printf("Couldn't open " CDDA_IMAGE " with BIN/CUE driver.\n");
  } else {
    print_disc_info(device);
  }

  if (argc > 1) psz_drive = argv[1];

  if (!device->open(psz_drive, DRIVER_DEVICE)) {
    printf("Couldn't find CD\n");
    return 1;
  } else {
    print_disc_info(device);
  }

  delete device;

  return 0;
}
