/* -*- C -*-
  Copyright (C) 2009, 2011 Rocky Bernstein <rocky@gnu.org>
  
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
/* Unit test for lib/driver/util.c */
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

#include <cdio/version.h>

int
main(int argc, const char *argv[])
{
    if (0 != strncmp(cdio_version_string, CDIO_VERSION,
		     strlen(CDIO_VERSION))) {
	fprintf(stderr, "CDIO VERSION string mismatch %s vs %s\n",
		cdio_version_string, CDIO_VERSION);
	exit(1);
    }
    if (libcdio_version_num != LIBCDIO_VERSION_NUM) {
	fprintf(stderr, "LIBCDIO_VERSION_NUM value mismatch %d vs %d\n",
		libcdio_version_num, LIBCDIO_VERSION_NUM);
	exit(2);
    }
    exit(0);
}

