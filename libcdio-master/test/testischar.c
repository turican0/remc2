/*
  Copyright (C) 2001, 2008 Herbert Valerio Riedel <hvr@gnu.org>

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
/* Tests ISO9660 character sets. */
#ifdef HAVE_CONFIG_H
#include "config.h"
#define __CDIO_CONFIG_H__ 1
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

#include <cdio/iso9660.h>

int
main (int argc, const char *argv[])
{
  int i, j;

  printf ("--  ");

  for (j = 0; j < 0x10; j++)
    printf (" %1.1x", j);

  printf (" |");

  for (j = 0; j < 0x10; j++)
    printf (" %1.1x", j);

  printf ("\n-- ");

  for (i = 0; i < 0x10; i++)
    {

      printf ("%1.1x ", i);

      for (j = 0; j < 0x10; j++)
	{
	  int c = (j << 4) + i;

	  printf (" %c", iso9660_is_dchar (c) ? c : ' ');
	}

      printf (" |");

      for (j = 0; j < 0x10; j++)
	{
	  int c = (j << 4) + i;

	  printf (" %c", iso9660_isachar (c) ? c : ' ');
	}
      
      printf ("\n-- ");
    }
  printf ("\n");

  return 0;
}
