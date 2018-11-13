/*
  Copyright (C) 2005, 2006, 2008, 2009 Rocky Bernstein <rocky@gnu.org>

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

/* Simple program to eject a CD-ROM drive door and then close it again.

   If a single argument is given, it is used as the CD-ROM device to
   eject/close. Otherwise a CD-ROM drive will be scanned for.
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
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <cdio/cdio.h>

int
main(int argc, const char *argv[])
{
  driver_return_code_t ret;
  driver_id_t driver_id = DRIVER_DEVICE;
  char *psz_drive = NULL;

  if (argc > 1)
    psz_drive = strdup(argv[1]);

  if (!psz_drive) {
    psz_drive = cdio_get_default_device_driver(&driver_id);
    if (!psz_drive) {
      printf("Can't find a CD-ROM to eject\n");
      exit(1);
    }
  }
  ret = cdio_eject_media_drive(psz_drive);
  switch(ret) {
  case DRIVER_OP_UNSUPPORTED:
    printf("Eject not supported for %s.\n", psz_drive);
    break;
  case DRIVER_OP_SUCCESS:
    printf("CD-ROM drive %s ejected.\n", psz_drive);
    break;
  default:
    printf("Eject of CD-ROM drive %s failed.\n", psz_drive);
    break;
  }

  if (DRIVER_OP_SUCCESS == cdio_close_tray(psz_drive, &driver_id)) {
    printf("Closed tray of CD-ROM drive %s.\n", psz_drive);
  } else {
    printf("Closing tray of CD-ROM drive %s failed.\n", psz_drive);
  }
  free(psz_drive);

  ret = cdio_eject_media_drive(NULL);
  switch(ret) {
  case DRIVER_OP_UNSUPPORTED:
    printf("Eject not supported for default device.\n");
    break;
  case DRIVER_OP_SUCCESS:
    printf("CD-ROM drive ejected for default device.\n");
    break;
  default:
    printf("Eject of CD-ROM drive failed for default device.\n");
    break;
  }

  driver_id = DRIVER_DEVICE;
  if (DRIVER_OP_SUCCESS == cdio_close_tray(NULL, &driver_id)) {
    printf("Closed tray of CD-ROM drive for default disc driver:\n\t%s\n",
	   cdio_driver_describe(driver_id));
  } else {
    printf("Closing tray of CD-ROM drive failed for default "
	   "disc driver:\n\t%s\n", cdio_driver_describe(driver_id));
  }

  return 0;
}
