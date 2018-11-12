/*
  Copyright (C) 2004-2006, 2008, 2012-2013 Rocky Bernstein <rocky@gnu.org>

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

/* Program to read ISO-9660 images. */

#include "util.h"
#include "portable.h"

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#include <cdio/cdio.h>
#include <cdio/iso9660.h>
#include <cdio/udf.h>

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "getopt.h"

#define CEILING(x, y) ((x+(y-1))/y)

/* Used by `main' to communicate with `parse_opt'. And global options
 */
static struct arguments
{
  char          *file_name;
  char          *output_file;
  char          *iso9660_image;
  int            debug_level;
  int            no_header;
  int            ignore;
  int            udf;
} opts;

/* Parse a options. */
static bool
parse_options (int argc, char *argv[])
{

  int opt;

  /* Configuration option codes */
  enum {
    OP_HANDLED = 0,
    OP_VERSION=1,
    OP_USAGE
  };

  static const char helpText[] =
    "Usage: %s [OPTION...]\n"
    "  -d, --debug=INT            Set debugging to LEVEL.\n"
    "  -i, --image=FILE           Read from ISO-9660 image. This option is mandatory\n"
    "  -e, --extract=FILE         Extract FILE from ISO-9660 image. This option is\n"
    "                             mandatory.\n"
    "  -k, --ignore               Ignore read error(s), i.e. keep going\n"
    "  --no-header                Don't display header and copyright (for\n"
    "                             regression testing)\n"
    "  -o, --output-file=FILE     Output file. This option is mandatory.\n"
    "  -U  --udf                  Contents are in UDF format\n"
    "  -V, --version              display version and copyright information and exit\n"
    "\n"
    "Help options:\n"
    "  -?, --help                 Show this help message\n"
    "  --usage                    Display brief usage message\n";

  static const char usageText[] =
    "Usage: %s [-d|--debug INT] [-i|--image FILE] [-e|--extract FILE]\n"
    "        [--no-header] [-o|--output-file FILE]  [-U|--udf]\n"
    "        [-V|--version] [-?|--help] [--usage]\n";

  /* Command-line options */
  static const char* optionsString = "d:i:e:o:VUk?";
  static const struct option optionsTable[] = {
    {"debug",       required_argument, NULL,   'd' },
    {"image",       required_argument, NULL,   'i' },
    {"extract",     required_argument, NULL,   'e' },
    {"no-header",   no_argument, &opts.no_header, 1 },
    {"ignore",      no_argument, &opts.ignore, 'k' },
    {"output-file", required_argument, NULL,   'o' },
    {"udf",         no_argument, &opts.udf,    'U' },
    {"version",     no_argument, NULL,         'V' },

    {"help", no_argument, NULL, '?' },
    {"usage", no_argument, NULL, OP_USAGE },
    { NULL, 0, NULL, 0 }
  };

  program_name = strrchr(argv[0],'/');
  program_name = program_name ? strdup(program_name+1) : strdup(argv[0]);

  while ((opt = getopt_long(argc, argv, optionsString, optionsTable, NULL)) != -1)
    switch (opt)
      {
      case 'd': opts.debug_level = atoi(optarg); break;
      case 'i': opts.iso9660_image = strdup(optarg); break;
      case 'k': opts.ignore        = 1; break;
      case 'e': opts.file_name = strdup(optarg); break;
      case 'o': opts.output_file = strdup(optarg); break;
      case 'U': opts.udf = 1; break;

      case 'V':
        print_version(program_name, CDIO_VERSION, 0, true);
        free(program_name);
        exit (EXIT_SUCCESS);
        break;

      case '?':
        fprintf(stdout, helpText, program_name);
        free(program_name);
        exit(EXIT_INFO);
        break;

      case OP_USAGE:
        fprintf(stderr, usageText, program_name);
        free(program_name);
        exit(EXIT_FAILURE);
        break;

      case OP_HANDLED:
        break;
      }

  if (optind < argc) {
    const char *remaining_arg = argv[optind++];
    if (opts.iso9660_image != NULL) {
      report( stderr, "%s: Source specified as --image %s and as %s\n",
              program_name, opts.iso9660_image, remaining_arg );
      free(program_name);
      exit (EXIT_FAILURE);
    }

    opts.iso9660_image = strdup(remaining_arg);

    if (optind < argc ) {
      report( stderr,
              "%s: use only one unnamed argument for the ISO 9660 "
              "image name\n",
              program_name );
      free(program_name);
      exit (EXIT_FAILURE);
    }
  }

  if (NULL == opts.iso9660_image) {
    report( stderr, "%s: you need to specify an ISO-9660 image name.\n",
            program_name );
    report( stderr, "%s: Use option --image or try --help.\n",
            program_name );
    exit (EXIT_FAILURE);
  }

  if (NULL == opts.file_name) {
    report( stderr, "%s: you need to specify a filename to extract.\n",
            program_name );
    report( stderr, "%s: Use option --extract or try --help.\n",
            program_name );
    exit (EXIT_FAILURE);
  }

  if (NULL == opts.output_file) {
    report( stderr,
            "%s: you need to specify a place write filename extraction to.\n",
             program_name );
    report( stderr, "%s: Use option --output-file or try --help.\n",
            program_name );
    exit (EXIT_FAILURE);
  }

  return true;
}

static void
init(void)
{
  opts.debug_level   = 0;
  opts.ignore        = 0;
  opts.file_name     = NULL;
  opts.output_file   = NULL;
  opts.iso9660_image = NULL;
}

static int read_iso_file(const char *iso_name, const char *src,
                         FILE *outfd, size_t *bytes_written)
{
  iso9660_stat_t *statbuf;
  int i;
  iso9660_t *iso;

  iso = iso9660_open (iso_name);

  if (NULL == iso) {
    report(stderr,
           "%s: Sorry, couldn't open ISO-9660 image file '%s'.\n",
           program_name, src);
    return 1;
  }

  statbuf = iso9660_ifs_stat_translate (iso, src);

  if (NULL == statbuf)
    {
      report(stderr,
             "%s: Could not get ISO-9660 file information out of %s"
             " for file %s.\n",
             program_name, iso_name, src);
      report(stderr,
             "%s: iso-info may be able to show the contents of %s.\n",
             program_name, iso_name);
      return 2;
    }


  /* Copy the blocks from the ISO-9660 filesystem to the local filesystem. */
  for (i = 0; i < statbuf->size; i += ISO_BLOCKSIZE)
    {
      char buf[ISO_BLOCKSIZE];

      memset (buf, 0, ISO_BLOCKSIZE);

      if ( ISO_BLOCKSIZE != iso9660_iso_seek_read (iso, buf, statbuf->lsn
                                                   + (i / ISO_BLOCKSIZE),
                                                   1) )
      {
        report(stderr, "Error reading ISO 9660 file at lsn %lu\n",
               (long unsigned int) statbuf->lsn + (i / ISO_BLOCKSIZE));
        if (!opts.ignore) return 4;
      }


      fwrite (buf, ISO_BLOCKSIZE, 1, outfd);

      if (ferror (outfd))
        {
          perror ("fwrite()");
          return 5;
        }
    }
  iso9660_close(iso);

  *bytes_written = statbuf->size;
  return 0;
}

static int read_udf_file(const char *iso_name, const char *src,
                         FILE *outfd, size_t *bytes_written)
{
  udf_t *p_udf;

  p_udf = udf_open (iso_name);

  if (NULL == p_udf) {
    fprintf(stderr, "Sorry, couldn't open %s as something using UDF\n",
            iso_name);
    return 1;
  } else {
    udf_dirent_t *p_udf_root = udf_get_root(p_udf, true, 0);
    udf_dirent_t *p_udf_file = NULL;
    if (NULL == p_udf_root) {
      fprintf(stderr, "Sorry, couldn't find / in %s\n",
              iso_name);
      return 1;
    }

    p_udf_file = udf_fopen(p_udf_root, src);
    if (!p_udf_file) {
      fprintf(stderr, "Sorry, couldn't find %s in %s\n",
              src, iso_name);
      udf_dirent_free(p_udf_root);
      return 2;

    }

    {
      uint64_t i_file_length = udf_get_file_length(p_udf_file);
      const unsigned int i_blocks = (unsigned int) CEILING(i_file_length, UDF_BLOCKSIZE);
      unsigned int i;
      for (i = 0; i < i_blocks ; i++) {
        char buf[UDF_BLOCKSIZE] = {'\0',};
        ssize_t i_read = udf_read_block(p_udf_file, buf, 1);

        if ( i_read < 0 ) {
          fprintf(stderr, "Error reading UDF file %s at block %u\n",
                  src, i);
	  free(p_udf_file);
	  udf_dirent_free(p_udf_root);
          return 4;
        }

        fwrite (buf, i_read, 1, outfd);

        if (ferror (outfd)) {
          perror ("fwrite()");
	  free(p_udf_file);
	  udf_dirent_free(p_udf_root);
          return 5;
        }
      }

      free(p_udf_file);
      udf_dirent_free(p_udf_root);
      udf_close(p_udf);
      *bytes_written = i_file_length;
    }
  }
  return 0;
}

int
main(int argc, char *argv[])
{
  FILE *outfd;
  int ret;
  size_t bytes_written = 0;

  init();

  /* Parse our arguments; every option seen by `parse_opt' will
     be reflected in `arguments'. */
  if (!parse_options(argc, argv)) {
    report(stderr,
           "error while parsing command line - try --help\n");
    return 2;
  }

  if (!(outfd = fopen (opts.output_file, "wb")))
    {
      report(stderr,
             "%s: Could not open %s for writing: %s\n",
             program_name, opts.output_file, strerror(errno));
      return 3;
    }

  if (opts.udf) {
      ret = read_udf_file (opts.iso9660_image, opts.file_name,
			   outfd, &bytes_written);
  } else  {
      ret = read_iso_file (opts.iso9660_image, opts.file_name,
                           outfd, &bytes_written);
  }
  if (ret != 0) return ret;

  fflush (outfd);

  /* Make sure the file size has the exact same byte size. Without the
     truncate below, the file will a multiple of ISO_BLOCKSIZE.
   */
  if (ftruncate (fileno (outfd), bytes_written))
    perror ("ftruncate()");

  fclose (outfd);
  return 0;
}
