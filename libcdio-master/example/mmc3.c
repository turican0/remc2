/*
  Copyright (C) 2006, 2008, 2009, 2011, 2017 Rocky Bernstein <rocky@gnu.org>

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

/* Another simple program to show use of SCSI MMC interface. Is
   basically the the libdio scsi_mmc_get_hwinfo() routine.
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
#include <cdio/mmc.h>

/* Set how long to wait for MMC commands to complete */
#define DEFAULT_TIMEOUT_MS 10000

int
main(int argc, const char *argv[])
{
  CdIo_t *p_cdio;
  driver_return_code_t ret;
  driver_id_t driver_id = DRIVER_DEVICE;
  char *psz_drive = NULL;
  bool do_eject = false;
  bool do_close = false;

  if (argc > 1)
    psz_drive = strdup(argv[1]);

  if (!psz_drive) {
    psz_drive = cdio_get_default_device_driver(&driver_id);
    if (!psz_drive) {
      printf("-- Can't find a CD-ROM\n");
     exit(77);
    }
  }

  p_cdio = cdio_open (psz_drive, driver_id);
  if (!p_cdio) {
    printf("-- Can't open %s\n", psz_drive);
    exit(77);
  }

  ret = mmc_get_tray_status(p_cdio);
  switch((int) ret) {
  case 0:
    printf("-- CD-ROM drive %s is closed.\n", psz_drive);
    do_eject = true;
    do_close = true;
    break;
  case 1:
    printf("-- CD-ROM drive %s is open.\n", psz_drive);
    break;
  default:
    printf("-- Error status for drive %s: %s.\n", psz_drive,
	   cdio_driver_errmsg(ret));
    free(psz_drive);
    return 77;
  }

  ret = mmc_get_media_changed(p_cdio);
  switch((int) ret) {
  case 0:
    printf("-- CD-ROM drive %s media not changed since last test.\n",
	   psz_drive);
    break;
  case 1:
    printf("-- CD-ROM drive %s media changed since last test.\n", psz_drive);
    break;
  default:
    printf("-- Error status for drive %s: %s.\n", psz_drive,
	   cdio_driver_errmsg(ret));
    free(psz_drive);
    return 77;
  }

  if (do_eject && argc > 2)
    cdio_eject_media_drive(psz_drive);
  else
    cdio_close_tray(psz_drive, &driver_id);

  ret = mmc_get_tray_status(p_cdio);
  switch((int) ret) {
  case 0:
    printf("-- CD-ROM drive %s is closed.\n", psz_drive);
    break;
  case 1:
    printf("-- CD-ROM drive %s is open.\n", psz_drive);
    break;
  default:
    printf("Error status for drive %s: %s.\n", psz_drive,
	   cdio_driver_errmsg(ret));
    free(psz_drive);
    return 77;
  }

  ret = mmc_get_media_changed(p_cdio);
  switch((int) ret) {
  case 0:
    printf("-- CD-ROM drive %s media not changed since last test.\n",
	   psz_drive);
    break;
  case 1:
    printf("-- CD-ROM drive %s media changed since last test.\n", psz_drive);
    break;
  default:
    printf("-- Error status for drive %s: %s.\n", psz_drive,
	   cdio_driver_errmsg(ret));
    free(psz_drive);
    return 77;
  }

  if (do_close)
    cdio_close_tray(psz_drive, &driver_id);

  free(psz_drive);
  cdio_destroy(p_cdio);

  return 0;
}
