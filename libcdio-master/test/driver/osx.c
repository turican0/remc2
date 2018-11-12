/* -*- C -*-
  Copyright (C) 2009, 2011, 2014 Rocky Bernstein <rocky@gnu.org>
  
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
   Unit test for lib/driver/osx.c
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
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <cdio/cdio.h>
#include <cdio/logging.h>

int
main(int argc, const char *argv[])
{
  CdIo_t *p_cdio;
  char **ppsz_drives=NULL;
  int n=0;

  cdio_loglevel_default = (argc > 1) ? CDIO_LOG_DEBUG : CDIO_LOG_INFO;
  /* snprintf(psz_nrgfile, sizeof(psz_nrgfile)-1,
	     "%s/%s", TEST_DIR, cue_file[i]);
  */
  if (!cdio_have_driver(DRIVER_OSX)) return(77);
  ppsz_drives = cdio_get_devices(DRIVER_DEVICE);
  if (!ppsz_drives) {
      printf("Can't find a CD-ROM drive. Skipping test.\n");
      exit(77);
  }
  
  do {
    p_cdio = cdio_open_osx(ppsz_drives[n]);
    if (p_cdio) {
      const char *psz_source = cdio_get_arg(p_cdio, "source");
      const char *psz_access_mode = cdio_get_arg(p_cdio, "access-mode");
      discmode_t  discmode = cdio_get_discmode(p_cdio);
      if (0 != strncmp(psz_source, ppsz_drives[0],
		       strlen(ppsz_drives[0]))) {
	  fprintf(stderr, 
		  "Got %s; should get back %s, the name we opened.\n",
		  psz_source, ppsz_drives[0]);
	  exit(1);
      }
      if (0 != strncmp(psz_access_mode, "OS X", strlen("OS X"))) {
	  fprintf(stderr,
		  "Got %s; Should get back %s, the access mode requested.\n",
		  psz_access_mode, "OS X");
	  exit(2);
      }
      if (CDIO_DISC_MODE_ERROR == discmode) {
	  fprintf(stderr,
		  "Error getting disc mode for device %s.\n",
		  ppsz_drives[n]);
	  exit(3);
      }
    }

    cdio_destroy(p_cdio);
  }
  while (ppsz_drives[++n] != NULL);

  cdio_free_device_list(ppsz_drives);

  return 0;
}
