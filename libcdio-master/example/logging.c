/*
  Copyright (C) 2013 Rocky Bernstein <rocky@gnu.org>

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

/* Simple program to show how to set log verbosity and how to write a
   custom log handler.

   Invoke with an argument level, e.g.
      logging debug
   or with a number like:
      logging 1 # same as debug
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
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif
#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <cdio/cdio.h>
#include <cdio/cd_types.h>
#include <cdio/logging.h>


/* Here is an example of a custom log handler. */
static void
custom_log_handler (cdio_log_level_t level, const char *message)
{
  switch(level) {
  case CDIO_LOG_DEBUG:
    printf("-- custom debug message: %s\n", message);
    return;
  case CDIO_LOG_INFO:
    printf("-- custom info message: %s\n", message);
    return;
  case CDIO_LOG_WARN:
    printf("-- custom warning message: %s\n", message);
    return;
  case CDIO_LOG_ERROR:
    printf("-- custom error message: %s\n", message);
    return;
  case CDIO_LOG_ASSERT:
    printf("-- custom assertion message: %s\n", message);
    return;
  default:
    printf("custom level %d message: %s\n", level, message);
  }
}

static long
parse_loglevel_arg(const char *ps_loglevel) {
    long loglevel;
    if (strncmp(ps_loglevel, "debug", strlen("debug")) == 0) {
	loglevel = CDIO_LOG_DEBUG;
    } else if (strncmp(ps_loglevel, "info", strlen("info")) == 0) {
	loglevel = CDIO_LOG_INFO;
    } else if (strncmp(ps_loglevel, "warn", strlen("warn")) == 0) {
	loglevel = CDIO_LOG_WARN;
    } else if (strncmp(ps_loglevel, "error", strlen("error")) == 0) {
	loglevel = CDIO_LOG_ERROR;
    } else if (strncmp(ps_loglevel, "assert", strlen("assert")) == 0) {
	loglevel = CDIO_LOG_ASSERT;
    } else {
        char *endptr;
	errno = 0;    /* To distinguish success/failure after call */
	loglevel = strtol(ps_loglevel, &endptr, 10);

	/* Check for various possible errors, in parsing the verbosity
	 * level. */
	if ((errno == ERANGE &&
	     (loglevel == LONG_MAX || loglevel == LONG_MIN))
		|| (errno != 0 && loglevel == 0)) {
	    perror("strtol conversion error of loglevel");
	    exit(EXIT_FAILURE);
	}

	if (endptr == ps_loglevel) {
	    fprintf(stderr, "No digits were found\n");
	    exit(EXIT_FAILURE);
	}
    }
    return loglevel;
}

static void
print_drives(void)
{
    char **ppsz_cd_drives=NULL, **c;
    /* Print out a list of CD-drives with the above set log level. */
    ppsz_cd_drives = cdio_get_devices(DRIVER_DEVICE);
    if (NULL != ppsz_cd_drives)
	for( c = ppsz_cd_drives; *c != NULL; c++ ) {
	    printf("-- Drive %s\n", *c);
	}
    cdio_free_device_list(ppsz_cd_drives);
}

int
main(int argc, const char *argv[])
{
    /* Set the log level to the warning verbosity. */
    cdio_loglevel_default = CDIO_LOG_WARN;

    /* Parse command arguemnts */
    if (argc > 2) {
	printf("Usage:\n%s <verbosity-level>\n"
	       "Shows drives.\n"
	       "A verbosity level is a name like 'debug', 'info', 'warn,'\n"
	       "or a number greater than 0.\n",
	       argv[0]);
	exit(EXIT_FAILURE);
    }

    if (argc == 2) {
	/* Set cdio's verbosity level. */
        cdio_loglevel_default = parse_loglevel_arg(argv[1]);
    }

    print_drives();

    /* Do the same thing again but with a custom log handler. */
    cdio_log_set_handler (custom_log_handler);

    print_drives();
    return 0;
}
