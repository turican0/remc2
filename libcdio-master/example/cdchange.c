/*
  Copyright (C) 2005-2013
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

/* Test media changed */
#ifdef HAVE_CONFIG_H
# include "config.h"
# define __CDIO_CONFIG_H__ 1
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif
#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif
#ifdef _WIN32
#include <windows.h>
#undef sleep
#define sleep(s) Sleep(1000*s)
#endif

#include <cdio/cdio.h>

int
main(int argc, const char *argv[])
{
  CdIo_t *p_cdio;
  long int i_sleep = 30;
  if (argc > 1) {
    p_cdio = cdio_open (argv[1], DRIVER_DEVICE);
    if (argc > 2) {
      errno = 0;
      i_sleep = strtol(argv[2], (char **)NULL, 10);
      if ( (LONG_MIN == i_sleep || LONG_MAX == i_sleep) && errno != 0 ) {
	printf("Invalid sleep parameter %s\n", argv[2]);
	printf("Error reported back from strtol: %s\n", strerror(errno));
	return 2;
      }
    }
  } else {
    p_cdio = cdio_open (NULL, DRIVER_DEVICE);
  }

  if (NULL == p_cdio) {
    printf("Couldn't find a driver.. leaving.\n");
    return 1;
  }

  if (cdio_get_media_changed(p_cdio))
    printf("Initial media status: changed\n");
  else
    printf("Initial media status: not changed\n");

  printf("Giving you %ld seconds to change CD if you want to do so.\n",
	 i_sleep);
  sleep(i_sleep);
  if (cdio_get_media_changed(p_cdio))
    printf("Media status: changed\n");
  else
    printf("Media status: not changed\n");

  cdio_destroy(p_cdio);
  return 0;
}
