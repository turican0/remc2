/*
    Copyright (C) 2001, 2008, 2011 Herbert Valerio Riedel <hvr@gnu.org>

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
#ifdef HAVE_CONFIG_H
#include "config.h"
#define __CDIO_CONFIG_H__ 1
#endif

#include <assert.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#include <math.h>

#include <cdio/iso9660.h>
#include <cdio/types.h>

/* Private headers */
#include "iso9660_private.h"

#define CHECK_SIZEOF(typnam) { \
  printf ("-- checking sizeof (%s) ...", #typnam); \
  if (sizeof (typnam) != (typnam##_SIZEOF)) { \
      printf ("failed!\n==> sizeof (%s) == %d (but should be %d)\n", \
              #typnam, (int)sizeof(typnam), (int)(typnam##_SIZEOF)); \
      fail++; \
  } else { pass++; printf ("ok!\n"); } \
}

#define CHECK_SIZEOF_STRUCT(typnam) { \
  printf ("-- checking sizeof (struct %s) ...", #typnam); \
  if (sizeof (struct typnam) != (struct_##typnam##_SIZEOF)) { \
      printf ("failed!\n==> sizeof (struct %s) == %d (but should be %d)\n", \
              #typnam, (int)sizeof(struct typnam), (int)(struct_##typnam##_SIZEOF)); \
      fail++; \
  } else { pass++; printf ("ok!\n"); } \
}

int main (int argc, const char *argv[])
{
  unsigned fail = 0, pass = 0;

  /* <cdio/types.h> */
  CHECK_SIZEOF(msf_t);

  /* "iso9660_private.h" */
  CHECK_SIZEOF(iso_volume_descriptor_t);
  CHECK_SIZEOF(iso9660_pvd_t);
  CHECK_SIZEOF(iso_path_table_t);
  CHECK_SIZEOF(iso9660_dir_t);

#define iso9660_xa_t_SIZEOF 14

  /* xa.h */
  CHECK_SIZEOF(iso9660_xa_t);

  if (fail)
    return 1;

  return 0;
}
