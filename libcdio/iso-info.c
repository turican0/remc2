/*
  Copyright (C) 2004-2006, 2008, 2012-2014, 2017 Rocky Bernstein
  <rocky@gnu.org>

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
  ISO Info - prints various information about a ISO 9660 image.
*/
#include "getopt.h"
#include "util.h"
#undef err_exit

/*#define err_exit(fmt, args...)                      \
  report (stderr, "%s: "fmt, program_name, ##args); \
  iso9660_close(p_iso);                             \
  if (NULL != program_name) free(program_name);     \
  if (NULL != source_name)  free(source_name);      \
  return(EXIT_FAILURE);
  */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#include <cdio/types.h>
#include <cdio/bytesex.h>
#include <cdio/cdio.h>
#include <cdio/ds.h>
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

#ifdef HAVE_ALLOCA_H
#include <alloca.h>
#endif

#if 0
#define STRONG "\033[1m"
#define NORMAL "\033[0m"
#else
#define STRONG "__________________________________\n"
#define NORMAL ""
#endif

/* TODO: Find a better place from where cd-info can read it too. */
/*
   ECMA-119 allows only a depth of 8 directories. Nobody obeys.
   Rock Ridge allows path length 1023. This would be max depth 512.
*/
#define CDIO_MAX_DIR_RECURSION 512

/* Used by `main' to communicate with `parse_opt'. And global options
 */
static struct arguments
{
  uint32_t       debug_level;
  int            version_only;
  int            silent;
  int            no_header;
  int            no_joliet;
  int            no_xa;
  int            no_rock_ridge;
  int            print_iso9660;
  int            print_udf;
  int            print_iso9660_short;
  int64_t        show_rock_ridge;
} opts;

/* Configuration option codes */
enum {
  OP_HANDLED = 0,

  OP_USAGE,

  /* These are the remaining configuration options */
  OP_VERSION,

};

/* Parse a all options. */
static bool
parse_options (int argc, char *argv[])
{
  int opt;
  int rc = EXIT_FAILURE;

  static const char helpText[] =
    "Usage: %s [OPTION...]\n"
    "  -d, --debug=UINT          Set debugging to LEVEL\n"
    "  -i, --input[=FILE]        Filename to read ISO-9960 image from\n"
    "  -f                        Generate output similar to 'find . -print'\n"
    "  -l, --iso9660             output similar to 'ls -lR' for an ISO 9660 fs\n"
    "  -U, --udf                 output similar to 'ls -lR' for a UDF fs\n"
    "  --no-header               Don't display header and copyright (for regression\n"
#ifdef HAVE_JOLIET
    "  --no-joliet               Don't use Joliet-extension information\n"
#endif /*HAVE_JOLIET*/
    "  --no-rock-ridge           Don't use Rock-Ridge-extension information\n"
    "  --no-xa                   Don't use XA-extension information\n"
    "  -r --show-rock-ridge UINT Show if image uses Rock-Ridge extensions\n"
    "                            A maximum of UINT files will be considered.\n"
    "                            Use 0 for all files.\n"
    "  -q, --quiet               Don't produce warning output\n"
    "  -V, --version            display version and copyright information and exit\n"
    "\n"
    "Help options:\n"
    "  -?, --help             Show this help message\n"
    "  --usage                Display brief usage message\n";

  static const char usageText[] =
    "Usage: %s [-i|--input FILE] [-f] [-l|--iso9660] [-U|--udf]\n"
    "        [--no-header] [--no-joliet] [--no-rock-ridge] [--show-rock-ridge] [--no-xa] [-q|--quiet]\n"
    "        [-d|--debug INT] [-V|--version] [-?|--help] [--usage]\n";

  static const char optionsString[] = "d:i::flUqV?";
  static const struct option optionsTable[] = {
    {"debug",   required_argument, NULL, 'd'},
    {"input",   optional_argument, NULL, 'i'},
    {"iso9660", no_argument,       NULL, 'l'},
    {"udf",     no_argument,       NULL, 'U'},
    {"no-header", no_argument, &opts.no_header, 1 },
#ifdef HAVE_JOLIET
    {"no-joliet", no_argument, &opts.no_joliet, 1 },
#endif /*HAVE_JOLIET*/
    {"no-rock-ridge", no_argument, &opts.no_rock_ridge, 1 },
    {"no-xa", no_argument, &opts.no_xa, 1 },
    {"quiet", no_argument, NULL, 'q'},
    {"show-rock-ridge", required_argument, NULL, 'r' },
    {"version", no_argument, NULL, 'V'},

    {"help", no_argument, NULL, '?' },
    {"usage", no_argument, NULL, OP_USAGE },
    { NULL, 0, NULL, 0 }
  };

  program_name = strrchr(argv[0],'/');
  program_name = program_name ? strdup(program_name+1) : strdup(argv[0]);

  while ((opt = getopt_long(argc, argv, optionsString, optionsTable, NULL)) >= 0) {
    switch (opt)
      {
      case 'd': opts.debug_level = atoi(optarg); break;
      case 'i': if (optarg != NULL) {
	  if (NULL != source_name)  free(source_name);
	  source_name = strdup(optarg);
	  break;
	}
      case 'f': opts.print_iso9660_short = 1; break;
      case 'l': opts.print_iso9660       = 1; break;
      case 'U': opts.print_udf           = 1; break;
      case 'q': opts.silent              = 1; break;
      case 'r': opts.show_rock_ridge = atoll(optarg); break;
      case 'V': opts.version_only        = 1; break;

      case '?':
        fprintf(stdout, helpText, program_name);
	rc = EXIT_INFO;
	goto error_exit;

      case OP_USAGE:
        fprintf(stderr, usageText, program_name);
	rc = EXIT_INFO;
	goto error_exit;

      case OP_HANDLED:
        break;
      }
  }

  if (optind < argc) {
    const char *remaining_arg = argv[optind++];
    if ( optind < argc ) {
      report( stderr, "%s: Source specified in previously %s and %s\n",
              program_name, source_name, remaining_arg );
      goto error_exit;
    }
    if (NULL != source_name)  free(source_name);
    source_name = strdup(remaining_arg);
  }

  return true;
 error_exit:
  free(program_name);
  exit(rc);
}

/* CDIO logging routines */

static void
_log_handler (cdio_log_level_t level, const char message[])
{
  if (level == CDIO_LOG_DEBUG && opts.debug_level < 2)
    return;

  if (level == CDIO_LOG_INFO  && opts.debug_level < 1)
    return;

  if (level == CDIO_LOG_WARN  && opts.silent)
    return;

  if (level == CDIO_LOG_ERROR) {
    // print an error like default, but *don't* exit.
    fprintf (stderr, "**ERROR: %s\n", message);
    fflush (stderr);
    return;
  }


  gl_default_cdio_log_handler (level, message);
}

static void
print_iso9660_recurse (iso9660_t *p_iso, const char psz_path[],
		       unsigned int rec_counter)
{
  CdioISO9660FileList_t *entlist;
  CdioISO9660DirList_t *p_dirlist = iso9660_dirlist_new();
  CdioListNode_t *entnode;
  uint8_t i_joliet_level = iso9660_ifs_get_joliet_level(p_iso);
  char *translated_name = (char *) alloca(4096);
  size_t translated_name_size = 4096;
  entlist = iso9660_ifs_readdir (p_iso, psz_path);

  if (opts.print_iso9660) {
    printf ("%s:\n", psz_path);
  }

  if (NULL == entlist) {
    iso9660_dirlist_free(p_dirlist);
    report( stderr, "Error getting above directory information\n" );
    return;
  }

  rec_counter++;
  if (rec_counter > CDIO_MAX_DIR_RECURSION) {
    iso9660_dirlist_free(p_dirlist);
    iso9660_filelist_free(entlist);
    report( stderr,
            "Directory recursion too deep. ISO most probably damaged.\n" );
    return;
  }

  /* Iterate over files in this directory */

  _CDIO_LIST_FOREACH (entnode, entlist)
    {
      iso9660_stat_t *p_statbuf = _cdio_list_node_data (entnode);
      char *psz_iso_name = p_statbuf->filename;
      char _fullname[4096] = { 0, };
      if (strlen(psz_iso_name) >= translated_name_size) {
         translated_name_size = strlen(psz_iso_name)+1;
       }

      if (yep != p_statbuf->rr.b3_rock || 1 == opts.no_rock_ridge) {
        iso9660_name_translate_ext(psz_iso_name, translated_name,
                                   i_joliet_level);
        snprintf (_fullname, sizeof (_fullname), "%s%s", psz_path,
                  translated_name);
      } else {
        snprintf (_fullname, sizeof (_fullname), "%s%s", psz_path,
                  psz_iso_name);
      }

      strncat (_fullname, "/", sizeof(_fullname) - strlen(_fullname) - 1);

      if (p_statbuf->type == _STAT_DIR
          && strcmp (psz_iso_name, ".")
          && strcmp (psz_iso_name, ".."))
        _cdio_list_append (p_dirlist, strdup (_fullname));

      if (opts.print_iso9660) {
        print_fs_attrs(p_statbuf,
                       0 == opts.no_rock_ridge,
                       iso9660_ifs_is_xa(p_iso) && 0 == opts.no_xa,
                       psz_iso_name, translated_name);
      } else {
        if ( strcmp (psz_iso_name, ".") && strcmp (psz_iso_name, ".."))
          printf("%9u %s%s\n", (unsigned int) p_statbuf->size, psz_path,
                 yep == p_statbuf->rr.b3_rock
                 ? psz_iso_name : translated_name);
      }
    }

  iso9660_filelist_free(entlist);

  if (opts.print_iso9660) {
    printf ("\n");
  }

  /* Now recurse over the directories. */

  _CDIO_LIST_FOREACH (entnode, p_dirlist)
    {
      char *_fullname = _cdio_list_node_data (entnode);

      print_iso9660_recurse (p_iso, _fullname, rec_counter);
    }

  iso9660_dirlist_free(p_dirlist);
}

static void
print_iso9660_fs (iso9660_t *iso)
{
  print_iso9660_recurse (iso, "/", 0);
}

static void
print_udf_file_info(const udf_dirent_t *p_udf_dirent,
                    const char* psz_dirname,
                    const char *psz_filename)
{
  time_t mod_time = udf_get_modification_time(p_udf_dirent);
  char date_str[30];
  char psz_mode[11]="invalid";
  const char *psz_fname= psz_filename
    ? psz_filename : udf_get_filename(p_udf_dirent);

  if (!opts.print_iso9660) {
    if (strcmp(psz_dirname, ".") != 0) {
      printf("%9lu ", (long unsigned int) udf_get_file_length(p_udf_dirent));
      printf("/%s", psz_dirname);
      printf("%s\n", *psz_fname ? psz_fname : "/");
    }
    return;
  }
  /* Print directory attributes*/
  printf("%s ", udf_mode_string(udf_get_posix_filemode(p_udf_dirent),
                                psz_mode));
  /* fake uid/gid */
  printf("0 0 ");
  printf("%3d ", udf_get_link_count(p_udf_dirent));
  printf("%9lu ", (long unsigned int) udf_get_file_length(p_udf_dirent));
  strftime_s(date_str, sizeof(date_str), "%b %d %Y %H:%M:%S ", localtime_s(&mod_time));
  printf("%s %s", date_str, *psz_fname ? psz_fname : "/");
  printf("\n");
}

static void
list_udf_files(udf_t *p_udf, udf_dirent_t *p_udf_dirent, const char *psz_path)
{
  if (!p_udf_dirent) return;

  if (opts.print_iso9660) {
    printf ("\n/%s:\n", psz_path);
  }
  print_udf_file_info(p_udf_dirent, psz_path, ".");

  while (udf_readdir(p_udf_dirent)) {

    if (udf_is_dir(p_udf_dirent)) {

      udf_dirent_t *p_udf_dirent2 = udf_opendir(p_udf_dirent);
      if (p_udf_dirent2) {
        const char *psz_dirname = udf_get_filename(p_udf_dirent);
        const unsigned int i_newlen=2 + strlen(psz_path) + strlen(psz_dirname);
        char *psz_newpath = calloc(1, sizeof(char)*i_newlen);

        snprintf(psz_newpath, i_newlen, "%s%s/", psz_path, psz_dirname);
        list_udf_files(p_udf, p_udf_dirent2, psz_newpath);
        free(psz_newpath);
      }
    } else {
      print_udf_file_info(p_udf_dirent, psz_path, NULL);
    }
  }
}

static int
print_udf_fs (void)
{
  udf_t *p_udf;

  p_udf = udf_open (source_name);

  if (NULL == p_udf) {
    fprintf(stderr, "Sorry, couldn't open %s as something using UDF\n",
            source_name);
    return 1;
  } else {
    udf_dirent_t *p_udf_root = udf_get_root(p_udf, true, 0);
    if (NULL == p_udf_root) {
      fprintf(stderr, "Sorry, couldn't find / in %s\n",
              source_name);
      return 1;
    }

    list_udf_files(p_udf, p_udf_root, "");
  }
  udf_close(p_udf);
  return 0;
}


/* Initialize global variables. */
static void
init(void)
{
  gl_default_cdio_log_handler = cdio_log_set_handler (_log_handler);

  /* Default option values. */
  opts.silent              = false;
  opts.no_header           = false;
  opts.no_joliet           = 0;
  opts.no_rock_ridge       = 0;
  opts.no_xa               = 0;
  opts.debug_level         = 0;
  opts.print_iso9660       = 0;
  opts.print_iso9660_short = 0;
  opts.show_rock_ridge     = -1;
}

#define print_vd_info(title, fn)          \
  if (fn(p_iso, &psz_str)) {              \
    printf(title ": %s\n", psz_str);      \
  }                                       \
  free(psz_str);                          \
  psz_str = NULL;

/* ------------------------------------------------------------------------ */

int
main(int argc, char *argv[])
{

  iso9660_t           *p_iso=NULL;
  iso_extension_mask_t iso_extension_mask = ISO_EXTENSION_ALL;

  init();

  /* Parse our arguments; every option seen by `parse_opt' will
     be reflected in `arguments'. */
  parse_options(argc, argv);

  print_version(program_name, CDIO_VERSION, opts.no_header, opts.version_only);

  if (opts.debug_level == 3) {
    cdio_loglevel_default = CDIO_LOG_INFO;
  } else if (opts.debug_level >= 4) {
    cdio_loglevel_default = CDIO_LOG_DEBUG;
  }

  if (source_name==NULL) {
    err_exit("No input device given/found%s\n", "");
  }

  if (opts.no_joliet) {
    iso_extension_mask &= ~ISO_EXTENSION_JOLIET;
  }

  p_iso = iso9660_open_ext (source_name, iso_extension_mask);

  if (p_iso==NULL) {
    err_exit("Error in opening ISO-9660 image%s\n", "");
  }

  if (opts.silent == 0) {
    char *psz_str = NULL;
    uint8_t u_joliet_level = iso9660_ifs_get_joliet_level(p_iso);

    printf(STRONG "ISO 9660 image: %s\n", source_name);
    print_vd_info("Application ", iso9660_ifs_get_application_id);
    print_vd_info("Preparer    ", iso9660_ifs_get_preparer_id);
    print_vd_info("Publisher   ", iso9660_ifs_get_publisher_id);
    print_vd_info("System      ", iso9660_ifs_get_system_id);
    print_vd_info("Volume      ", iso9660_ifs_get_volume_id);
    print_vd_info("Volume Set  ", iso9660_ifs_get_volumeset_id);

    if (opts.show_rock_ridge >= 0) {
      switch (iso9660_have_rr(p_iso, (uint64_t) opts.show_rock_ridge)) {
      case yep:
	printf("Rock Ridge  : yes\n");
	break;
      case nope:
	printf("Rock Ridge  : no\n");
	break;
      case dunno:
	printf("Rock Ridge  : possibly not\n");
      }
    }

    if (u_joliet_level == 0) {
	printf("No Joliet extensions\n");
    } else {
	printf("Joliet Level: %u\n", u_joliet_level);
    }

  }

  if (opts.print_iso9660 || opts.print_iso9660_short) {
      printf(STRONG "ISO-9660 Information\n" NORMAL);
      if (opts.print_iso9660 && opts.print_iso9660_short) {
          printf("Note: both -f and -l options given -- "
                 "-l (long listing) takes precidence\n");
      }
      print_iso9660_fs(p_iso);
  } else if (opts.print_udf) {
      print_udf_fs();
  }


  free(source_name);
  iso9660_close(p_iso);
  /* Not reached:*/
  free(program_name);
  return(EXIT_SUCCESS);
}
