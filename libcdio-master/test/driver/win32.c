/* -*- C -*-
  Copyright (C) 2009, 2010, 2011 Rocky Bernstein <rocky@gnu.org>
  
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
   Unit test for lib/driver/MSWindows/win32.c
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
  
  cdio_loglevel_default = (argc > 1) ? CDIO_LOG_DEBUG : CDIO_LOG_INFO;
  /* snprintf(psz_nrgfile, sizeof(psz_nrgfile)-1,
	     "%s/%s", TEST_DIR, cue_file[i]);
  */
  if (!cdio_have_driver(DRIVER_WIN32)) return(77);
  ppsz_drives = cdio_get_devices(DRIVER_DEVICE);
  if (!ppsz_drives) {
      printf("Can't find a CD-ROM drive. Skipping test.\n");
      exit(77);
  }
  
  p_cdio = cdio_open_win32(ppsz_drives[0]);
  if (p_cdio) {
      const char *psz_source = cdio_get_arg(p_cdio, "source");
      const char *psz_scsi_tuple;
      const char *psz_access_mode = cdio_get_arg(p_cdio, "access-mode");
      if (0 != strncmp(psz_source, ppsz_drives[0],
		       strlen(ppsz_drives[0]))) {
	  fprintf(stderr, 
		  "Got %s; should get back %s, the name we opened.\n",
		  psz_source, ppsz_drives[0]);
	  cdio_destroy(p_cdio);
	  exit(1);
      }

      if (0 == strncmp(psz_access_mode, "ioctl", strlen("ioctl"))) {
	psz_scsi_tuple = cdio_get_arg(p_cdio, "scsi-tuple");
	if (psz_scsi_tuple == NULL) {
	  fprintf(stderr, "cdio_get_arg(\"scsi-tuple\") returns NULL.\n");
	  cdio_destroy(p_cdio);
	  exit(3);
	}
	if (cdio_loglevel_default == CDIO_LOG_DEBUG)
	  printf("Drive '%s' has cdio_get_arg(\"scsi-tuple\") = '%s'\n",
		 psz_source, psz_scsi_tuple);
      }

  }
  
  cdio_destroy(p_cdio);
  p_cdio = cdio_open_am_win32(ppsz_drives[0], "ASPI");
  if (p_cdio) {
      const char *psz_access_mode = cdio_get_arg(p_cdio, "access-mode");
      
      if (0 != strncmp(psz_access_mode, "ASPI", strlen("ASPI"))) {
	  fprintf(stderr,
		  "Got %s; Should get back %s, the access mode requested.\n",
		  psz_access_mode, "ASPI");
	  exit(2);
      }
  }

  p_cdio = cdio_open_am_win32(ppsz_drives[0], "ioctl");
  if (p_cdio) {
      const char *psz_access_mode = cdio_get_arg(p_cdio, "access-mode");
      const char *psz_response = cdio_get_arg(p_cdio, "mmc-supported?");
      
      if (0 != strncmp(psz_access_mode, "ioctl", strlen("ioctl"))) {
	  fprintf(stderr,
		  "Got %s; Should get back %s, the access mode requested.\n",
		  psz_access_mode, "ioctl");
	  exit(3);
      }

      if ( psz_response == NULL || 
	   ((0 != strncmp("true", psz_response, sizeof("true")))) ) {
	fprintf(stderr, 
		"cdio_get_arg(\"mmc-supported?\") should return \"true\"; got: %s.\n",
		psz_response);
	exit(4);
    }

  }

  cdio_destroy(p_cdio);
  cdio_free_device_list(ppsz_drives);

  return 0;
}
