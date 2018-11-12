/*
  Copyright (C) 2013
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

/* Simple program to show using libiso9660 how to see if a file
   has Rock-Ridge Extensions
 */

/* Set up an ISO-9660 image to test on which is in the libcdio
   distribution. */
#define ISO9660_IMAGE_PATH "../"
#define ISO9660_IMAGE ISO9660_IMAGE_PATH "test/data/copying-rr.iso"

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
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include <cdio/cdio.h>
#include <cdio/iso9660.h>

int
main(int argc, const char *argv[])
{
  char const *psz_fname;
  iso9660_t *p_iso;

  if (argc > 1)
    psz_fname = argv[1];
  else
    psz_fname = ISO9660_IMAGE;

  p_iso = iso9660_open (psz_fname);

  if (NULL == p_iso) {
    fprintf(stderr, "Sorry, couldn't open %s as an ISO-9660 image\n",
	    psz_fname);
    return 1;
  }

  switch (iso9660_have_rr(p_iso, 100)) {
    case yep:
      printf("-- Have Rock Ridge\n");
      break;
    case nope:
      printf("-- Don't have Rock Ridge\n");
      break;
    case dunno:
      printf("-- Possibly don't have Rock Ridge\n");
      break;
    }

  iso9660_close(p_iso);
  return 0;
}
