/*
  Copyright (C) 2011
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

/* http://sourceforge.net/projects/discid/ using libcdio:

   a small program to calculate the disc-ID for audio compact
   discs. The disc-ID can be used to query info (tracks, title,
   interpret) about compact audio discs from a CDDB-Server.
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#define __CDIO_CONFIG_H__ 1
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include <cdio/cdio.h>

static int cddb_sum(int n)
{
  int result = 0;
  while (n > 0)
  {
    result += n % 10;
    n /= 10;
  }

  return result;
}

int
main(int argc, const char *argv[])
{
  CdIo_t *p_cdio = cdio_open (NULL, DRIVER_UNKNOWN);
  track_t i_track;
  track_t i_tracks;
  unsigned n = 0;

  if (NULL == p_cdio) {
    printf("Couldn't find a driver.. leaving.\n");
    return 1;
  }
  
  i_tracks = cdio_get_num_tracks(p_cdio);
  for (i_track = 1; i_track <= i_tracks; ++i_track) {
      lba_t lba = cdio_get_track_lba(p_cdio, i_track);
      n += cddb_sum(lba / CDIO_CD_FRAMES_PER_SEC);
  }
  { 
      unsigned start_sec = 
	  cdio_get_track_lba(p_cdio, 1) /  CDIO_CD_FRAMES_PER_SEC;
      unsigned leadout_sec = 
	  cdio_get_track_lba(p_cdio, CDIO_CDROM_LEADOUT_TRACK) 
	  / CDIO_CD_FRAMES_PER_SEC;
      unsigned total = leadout_sec - start_sec;
      unsigned id = ((n % 0xff) << 24 | total << 8 | i_tracks);

      printf("%08X %d", id, i_tracks);
      for (i_track = 1; i_track <= i_tracks; ++i_track) {
	  lba_t lba = cdio_get_track_lba(p_cdio, i_track);
	  printf(" %ld", (long) lba);
      }
      printf(" %ld\n", (long) cdio_get_track_lba(p_cdio, 
						 CDIO_CDROM_LEADOUT_TRACK)
	     / CDIO_CD_FRAMES_PER_SEC);
  }

  cdio_destroy(p_cdio);
  return 0;
}
