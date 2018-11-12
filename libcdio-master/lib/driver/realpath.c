/*
  Copyright (C) 2010, 2011
  Rocky Bernstein <rocky@gnu.org>
  Diego 'Flameeyes' Pettenò 
  Thomas Schmitt

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
  POSIX realpath if that's around, and something like it if not.

  To compile as a standalone program:
  gcc -Wall -g -I../.. -DHAVE_CONFIG_H -DSTANDALONE -o realpath realpath.c
*/
/*
  Make sure we handle:
  - resolve relative links like  /dev/cdrom -> sr2
  - abort on cyclic linking like /dev/cdrom -> /dev/cdrom
*/

#ifdef HAVE_CONFIG_H
# include "config.h"
# define __CDIO_CONFIG_H__ 1
#endif

#include <ctype.h>

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif

#ifdef HAVE_STDIO_H
# include <stdio.h>
#endif

#ifdef HAVE_STRING_H
# include <string.h>
#endif

#ifdef HAVE_UNISTD_H /* readlink */
# include <unistd.h>
#endif

#ifdef HAVE_INTTYPES_H
# include "inttypes.h"
#endif

#ifdef HAVE_LIMITS_H
# include <limits.h>
#endif

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#include <cdio/util.h>

/*!  cdio_realpath() same as POSIX.1-2001 realpath if that's
around. If not we do poor-man's simulation of that behavior.  */
char *cdio_realpath (const char *psz_src_path, char *psz_resolved_path) {

#ifdef HAVE_REALPATH
  psz_resolved_path = realpath(psz_src_path, psz_resolved_path);

#elif defined(HAVE_READLINK)
  char tmp_src[PATH_MAX+1];
  char tmp_dst[PATH_MAX+1];
  char *p_adr;
  int i, len;
  const int loop_limit = 100;

  strcpy(tmp_src, psz_src_path);

  /* FIXME: 
     remove loop and change with stat before and after readlink 
     which looks direct symlink. Rely on errno to figure out other
     non-existent or looped symlinks. 
  */
  for(i = 0; i < loop_limit; i++) {
    len = readlink(tmp_src, tmp_dst, PATH_MAX);
    if (-1 == len) {
	/* Right now we expect a "not symlink" error. However after
	   we put in the stat() suggested above, there won't be a symlink
	   error and we should start reporting what the failure is.
	*/
	break;
    } else {
	tmp_dst[len] = '\0';
	if (tmp_dst[0] != '/') {
	    /* Take care of relative link like  /dev/cdrom -> sr2 */
	    p_adr = strrchr(tmp_src, '/');
	    if (p_adr != NULL) {
		strncpy(p_adr + 1, tmp_dst, PATH_MAX - (p_adr + 1 - tmp_src));
	    } else {
		strncpy(tmp_src, tmp_dst, PATH_MAX);
	    }
	} else {
	    strncpy(tmp_src, tmp_dst, PATH_MAX);
	}
	tmp_src[PATH_MAX - 1] = 0; /* strncpy() does not always set a 0 */
    }
  }
  strncpy(psz_resolved_path, tmp_src, PATH_MAX);
#else
  strncpy(psz_resolved_path, psz_src_path, PATH_MAX);
#endif

  return psz_resolved_path;
  
}

#ifdef STANDALONE
int main(int argc, char **argv)
{
  int i;
  char dest[PATH_MAX];

  if (argc < 2) {
    fprintf(stderr, "Usage: %s  path [path ...]\n", argv[0]);
    fprintf(stderr,
     "       Resolve symbolic links\n");
    exit(1);
  }
  for (i= 1; i < argc; i++) {
    dest[0] = 0;
    cdio_realpath (argv[i], dest);
    printf("%s -> %s\n", argv[i], dest);
  }
  exit(0);
}
#endif
