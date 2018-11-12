/* -*- C -*-
  Copyright (C) 2009, 2010, 2012 Rocky Bernstein <rocky@gnu.org>
  
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
   Unit test for lib/driver/gnu_linux.c
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
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

#include "helper.h"

int
main(int argc, const char *argv[])
{
  CdIo_t *p_cdio;
  char **ppsz_drives=NULL;
  
  cdio_log_set_handler(log_handler);
  cdio_loglevel_default = (argc > 1) ? CDIO_LOG_DEBUG : CDIO_LOG_INFO;
  /* snprintf(psz_nrgfile, sizeof(psz_nrgfile)-1,
             "%s/%s", TEST_DIR, cue_file[i]);
  */
  if (!cdio_have_driver(DRIVER_LINUX)) return(77);
  ppsz_drives = cdio_get_devices(DRIVER_DEVICE);
  if (!ppsz_drives) {
      printf("Can't find a CD-ROM drive. Skipping test.\n");
      exit(77);
  }
  
  p_cdio = cdio_open_linux(ppsz_drives[0]);
  if (p_cdio) {
      const char *psz_source = NULL, *psz_scsi_tuple;
      lsn_t lsn;
      
      reset_counts();
      lsn = cdio_get_track_lsn(p_cdio, CDIO_CD_MAX_TRACKS+1);
      assert_equal_int(CDIO_INVALID_LSN, lsn, 
		       "cdio_get_track_lsn with too large of a track number");
      reset_counts();
      
      check_get_arg_source(p_cdio, ppsz_drives[0]);
      check_mmc_supported(p_cdio, 3);
  
      psz_scsi_tuple = cdio_get_arg(p_cdio, "scsi-tuple");
      if (psz_scsi_tuple == NULL) {
	  fprintf(stderr, "cdio_get_arg(\"scsi-tuple\") returns NULL.\n");
	  cdio_destroy(p_cdio);
	  exit(3);
      }
      if (cdio_loglevel_default == CDIO_LOG_DEBUG)
	  printf("Drive '%s' has cdio_get_arg(\"scsi-tuple\") = '%s'\n",
		 psz_source, psz_scsi_tuple);
      cdio_destroy(p_cdio);
  }
  
  p_cdio = cdio_open_am_linux(ppsz_drives[0], "MMC_RDWR");
  if (p_cdio) {
      check_access_mode(p_cdio, "MMC_RDWR");
  }

  cdio_destroy(p_cdio);
  cdio_free_device_list(ppsz_drives);

  return 0;
}
