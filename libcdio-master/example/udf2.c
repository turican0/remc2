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

/* Simple program to show using libudf to extract a file.

   This program can be compiled with either a C or C++ compiler. In 
   the distribution we prefer C++ just to make sure we haven't broken
   things on the C++ side.
 */

/* This is the UDF image. */
#define UDF_IMAGE_PATH "../"
#define UDF_IMAGE "/src2/cd-images/udf/test2.iso"
#define UDF_FILENAME "/parse/cue.L"

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
#include <cdio/udf.h>

#define CEILING(x, y) ((x+(y-1))/y)

#define udf_PATH_DELIMITERS "/\\"

static void 
print_file_info(const udf_dirent_t *p_udf_dirent, const char* psz_dirname)
{
  time_t mod_time = udf_get_modification_time(p_udf_dirent);
  char psz_mode[11]="invalid";
  const char *psz_fname= psz_dirname 
    ? psz_dirname : udf_get_filename(p_udf_dirent);

  /* Print directory attributes*/
  printf("%s ", udf_mode_string(udf_get_posix_filemode(p_udf_dirent),
				psz_mode));
  printf("%4d ", udf_get_link_count(p_udf_dirent));
  printf("%ld ", (long unsigned int) udf_get_file_length(p_udf_dirent));
  printf("%s %s\n",  *psz_fname ? psz_fname : "/", ctime(&mod_time));
}

int
main(int argc, const char *argv[])
{
  udf_t *p_udf;
  char const *psz_udf_image;
  char const *psz_udf_fname;

  if (argc > 1) 
    psz_udf_image = argv[1];
  else 
    psz_udf_image = UDF_IMAGE;

  if (argc > 2) 
    psz_udf_fname = argv[2];
  else 
    psz_udf_fname = UDF_FILENAME;


  p_udf = udf_open (psz_udf_image);
  
  if (NULL == p_udf) {
    fprintf(stderr, "Sorry, couldn't open %s as something using UDF\n", 
	    psz_udf_image);
    return 1;
  } else {
    udf_dirent_t *p_udf_root = udf_get_root(p_udf, true, 0);
    udf_dirent_t *p_udf_file = NULL;
    if (NULL == p_udf_root) {
      fprintf(stderr, "Sorry, couldn't find / in %s\n", 
	      psz_udf_image);
      return 1;
    }
    
    p_udf_file = udf_fopen(p_udf_root, psz_udf_fname);
    if (!p_udf_file) {
      fprintf(stderr, "Sorry, couldn't find %s in %s\n", 
	      psz_udf_fname, psz_udf_image);
      return 2;
      
    }
    print_file_info(p_udf_file, udf_get_filename(p_udf_file));
    {
      long unsigned int i_file_length = udf_get_file_length(p_udf_file);
      char *p_buf;
      unsigned int i_blocks = CEILING(i_file_length, UDF_BLOCKSIZE);
      char fmt_string[100] = {'\0'};
      snprintf(fmt_string, sizeof(fmt_string), "%%%lus", i_file_length);
      
      p_buf = (char *) calloc(sizeof(char), UDF_BLOCKSIZE*i_blocks);
      udf_read_block(p_udf_file, p_buf, i_blocks);
      printf(fmt_string, p_buf);
      free(p_buf);
    }
    udf_dirent_free(p_udf_file);
    udf_dirent_free(p_udf_root);
  }
  
  udf_close(p_udf);
  return 0;
}

