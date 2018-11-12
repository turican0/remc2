/* -*- C -*-
  Copyright (C) 2012 Rocky Bernstein <rocky@gnu.org>
  
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
   Unit test for cdda-like things lib/driver/disc.c
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
  char *mcn = cdio_get_mcn(NULL);
  
  cdio_log_set_handler(log_handler);
  cdio_loglevel_default = (argc > 1) ? CDIO_LOG_DEBUG : CDIO_LOG_INFO;

  /* Had a bug in not testing for null p_cdio with cdio_get_mcn. */
  mcn = cdio_get_mcn(NULL);
  if (mcn != NULL) {
      printf("Should have gotten NULL for MCN for cdio object\n");
      return 1;
  }
  
  return 0;
}
