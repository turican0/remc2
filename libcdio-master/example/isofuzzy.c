/*
  Copyright (C) 2005, 2008, 2009, 2017 Rocky Bernstein <rocky@gnu.org>

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

/* Program to show using libiso9660 with fuzzy search to get file info.

   If a single argument is given, it is used as the ISO 9660 image.
   Otherwise we use a compiled-in default ISO 9660 image
   name.
 */

/* This is the BIN we think there is an ISO 9660 image inside of. */
#define ISO9660_IMAGE_PATH "/tmp/"
#define ISO9660_IMAGE ISO9660_IMAGE_PATH "vcd_demo.bin"

/* portable.h has to come first else _FILE_OFFSET_BITS are redefined in
   say opensolaris. */
#include "portable.h"

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#ifdef HAVE_ERRNO_H
#include <errno.h>
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
  CdioISO9660FileList_t *p_entlist;
  CdioListNode_t *entnode;
  char const *psz_fname;
  iso9660_t *p_iso;

  if (argc > 1)
    psz_fname = argv[1];
  else
    psz_fname = ISO9660_IMAGE;

  p_iso = iso9660_open_fuzzy (psz_fname, 5);

  if (NULL == p_iso) {
    fprintf(stderr, "-- Sorry, could not find an ISO 9660 image from %s\n",
	    psz_fname);
    return 1;
  }

  p_entlist = iso9660_ifs_readdir (p_iso, "/");

  /* Iterate over the list of nodes that iso9660_ifs_readdir gives  */

  if (p_entlist) {
    _CDIO_LIST_FOREACH (entnode, p_entlist)
    {
      char filename[4096];
      iso9660_stat_t *p_statbuf =
	(iso9660_stat_t *) _cdio_list_node_data (entnode);
      iso9660_name_translate(p_statbuf->filename, filename);
      printf ("-- /%s\n", filename);
    }

    iso9660_filelist_free(p_entlist);
  }

  iso9660_close(p_iso);
  exit(0);
}
