/*
  Copyright (C) 2012 Pete Batard <pete@akeo.ie>
  Based on samples copyright (c) 2003-2011, 2017 Rocky Bernstein <rocky@gnu.org>

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

/* Extract the full contents of either an UDF or ISO9660 image file.
   TODO: timestamp preservation, file permissions, Unicode
 */

/* To handle files > 2 GB, we may need the Large File Support settings
   defined in config.h. Comes first, as stdio.h depends on it. */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#if defined(_WIN32)
#include <direct.h>
#else
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#define _mkdir(a) mkdir(a, S_IRWXU)
#endif

#if !defined(HAVE_SNPRINTF)
/* Fallback to unsafe 'sprintf' */
#define snprintf(str, size, format, ...) sprintf(str, format, __VA_ARGS__)
#endif

#include <cdio/cdio.h>
#include <cdio/logging.h>
#include <cdio/iso9660.h>
#include <cdio/udf.h>

#ifndef MIN
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#define print_vd_info(title, fn)     \
  if (fn(p_iso, &psz_str)) {         \
    printf(title ": %s\n", psz_str); \
  }                                  \
  free(psz_str);                     \
  psz_str = NULL;

static const char *psz_extract_dir;
static uint8_t i_joliet_level = 0;

static void log_handler (cdio_log_level_t level, const char *message)
{
  switch(level) {
  case CDIO_LOG_DEBUG:
  case CDIO_LOG_INFO:
    return;
  default:
    printf("cdio %d message: %s\n", level, message);
  }
}

static int udf_extract_files(udf_t *p_udf, udf_dirent_t *p_udf_dirent, const char *psz_path)
{
  FILE *fd = NULL;
  int i_length;
  char* psz_fullpath;
  const char* psz_basename;
  udf_dirent_t *p_udf_dirent2;
  uint8_t buf[UDF_BLOCKSIZE];
  int64_t i_read, i_file_length;

  if ((p_udf_dirent == NULL) || (psz_path == NULL))
    return 1;

  while (udf_readdir(p_udf_dirent)) {
    psz_basename = udf_get_filename(p_udf_dirent);
    i_length = 3 + strlen(psz_path) + strlen(psz_basename) + strlen(psz_extract_dir);
    psz_fullpath = (char*)calloc(sizeof(char), i_length);
    if (psz_fullpath == NULL) {
      fprintf(stderr, "Error allocating file name\n");
      goto out;
    }
    i_length = snprintf(psz_fullpath, i_length, "%s%s/%s", psz_extract_dir, psz_path, psz_basename);
    if (i_length < 0) {
      goto out;
    }
    printf("-- Extracting: %s\n", psz_fullpath);
    if (udf_is_dir(p_udf_dirent)) {
      int rc = _mkdir(psz_fullpath);
      if (0 == rc) {
	p_udf_dirent2 = udf_opendir(p_udf_dirent);
	if (p_udf_dirent2 != NULL) {
	  if (udf_extract_files(p_udf, p_udf_dirent2,
				&psz_fullpath[strlen(psz_extract_dir)]))
	    goto out;
	}
      } else if (-1 == rc) {
        fprintf(stderr, "  Unable to create make directory %s:\n%s\n",
		psz_fullpath, strerror(errno));
      } else {
        fprintf(stderr, "  Unable to create make directory %s;(rc=%d)\n",
		psz_fullpath, rc);
      }
    } else {
      fd = fopen(psz_fullpath, "wb");
      if (fd == NULL) {
        fprintf(stderr, "  Unable to create file %s\n", psz_fullpath);
        goto out;
      }
      i_file_length = udf_get_file_length(p_udf_dirent);
      while (i_file_length > 0) {
        memset(buf, 0, UDF_BLOCKSIZE);
        i_read = udf_read_block(p_udf_dirent, buf, 1);
        if (i_read < 0) {
          fprintf(stderr, "  Error reading UDF file %s\n", &psz_fullpath[strlen(psz_extract_dir)]);
          goto out;
        }
        fwrite(buf, (size_t)MIN(i_file_length, i_read), 1, fd);
        if (ferror(fd)) {
          fprintf(stderr, "  Error writing file %s: %s\n", psz_fullpath,
                  strerror(errno));
          goto out;
        }
        i_file_length -= i_read;
      }
      fclose(fd);
      fd = NULL;
    }
    free(psz_fullpath);
  }
  return 0;

out:
  if (fd != NULL)
    fclose(fd);
  free(psz_fullpath);
  return 1;
}

static int iso_extract_files(iso9660_t* p_iso, const char *psz_path)
{
  FILE *fd = NULL;
  int i_length, r = 1;
  char psz_fullpath[4096], *psz_basename;
  const char *psz_iso_name = &psz_fullpath[strlen(psz_extract_dir)];
  unsigned char buf[ISO_BLOCKSIZE];
  CdioListNode_t *p_entnode;
  iso9660_stat_t *p_statbuf;
  CdioISO9660FileList_t* p_entlist;
  size_t i;
  lsn_t lsn;
  int64_t i_file_length;

  if ((p_iso == NULL) || (psz_path == NULL))
    return 1;

  i_length = snprintf(psz_fullpath, sizeof(psz_fullpath), "%s%s/", psz_extract_dir, psz_path);
  if (i_length < 0)
    return 1;
  psz_basename = &psz_fullpath[i_length];

  p_entlist = iso9660_ifs_readdir(p_iso, psz_path);
  if (!p_entlist) {
    printf("Could not access %s\n", psz_path);
    return 1;
  }

  _CDIO_LIST_FOREACH (p_entnode, p_entlist) {
    p_statbuf = (iso9660_stat_t*) _cdio_list_node_data(p_entnode);
    /* Eliminate . and .. entries */
    if ( (strcmp(p_statbuf->filename, ".") == 0)
      || (strcmp(p_statbuf->filename, "..") == 0) )
      continue;
    iso9660_name_translate_ext(p_statbuf->filename, psz_basename, i_joliet_level);
    if (p_statbuf->type == _STAT_DIR) {
      _mkdir(psz_fullpath);
      if (iso_extract_files(p_iso, psz_iso_name))
        goto out;
    } else {
      printf("Extracting: %s\n", psz_fullpath);
      fd = fopen(psz_fullpath, "wb");
      if (fd == NULL) {
        fprintf(stderr, "  Unable to create file\n");
        goto out;
      }
      i_file_length = p_statbuf->size;
      for (i = 0; i_file_length > 0; i++) {
        memset(buf, 0, ISO_BLOCKSIZE);
        lsn = p_statbuf->lsn + i;
        if (iso9660_iso_seek_read(p_iso, buf, lsn, 1) != ISO_BLOCKSIZE) {
          fprintf(stderr, "  Error reading ISO9660 file %s at LSN %lu\n",
            psz_iso_name, (long unsigned int)lsn);
          goto out;
        }
        fwrite(buf, (size_t)MIN(i_file_length, ISO_BLOCKSIZE), 1, fd);
        if (ferror(fd)) {
	  fprintf(stderr, "  Error writing file %s: %s\n", psz_iso_name,
		  strerror(errno));
          goto out;
        }
        i_file_length -= ISO_BLOCKSIZE;
      }
      fclose(fd);
      fd = NULL;
    }
  }
  r = 0;

out:
  if (fd != NULL)
    fclose(fd);
  iso9660_filelist_free(p_entlist);
  return r;
}

int main(int argc, char** argv)
{
  iso9660_t* p_iso = NULL;
  udf_t* p_udf = NULL;
  udf_dirent_t* p_udf_root;
  char *psz_str = NULL;
  char vol_id[UDF_VOLID_SIZE] = "";
  char volset_id[UDF_VOLSET_ID_SIZE+1] = "";
  int r = 0;

  cdio_log_set_handler (log_handler);

  if (argc < 3) {
    fprintf(stderr, "Usage: extract <iso_image> <extraction_dir>\n");
    return 1;
  }

  /* Warn if LFS doesn't appear to be enabled */
  if (sizeof(off_t) < 8) {
    fprintf(stderr, "INFO: Large File Support not detected (required for files >2GB)\n");
  }

  psz_extract_dir = argv[2];
  if (_mkdir(psz_extract_dir) == 0) {
    printf("-- Creating directory: %s\n", psz_extract_dir);
  } else if (errno != EEXIST) {
    fprintf(stderr, "Unable to create extraction directory %s\n", psz_extract_dir);
    return 1;
  }

  /* First try to open as UDF - fallback to ISO if it failed */
  p_udf = udf_open(argv[1]);
  if (p_udf == NULL)
    goto try_iso;

  p_udf_root = udf_get_root(p_udf, true, 0);
  if (p_udf_root == NULL) {
    fprintf(stderr, "Couldn't locate UDF root directory\n");
    goto out;
  }
  vol_id[0] = 0; volset_id[0] = 0;

  /* Show basic UDF Volume info */
  if (udf_get_volume_id(p_udf, vol_id, sizeof(vol_id)) > 0)
    printf("-- Volume id: %s\n", vol_id);
  if (udf_get_volume_id(p_udf, volset_id, sizeof(volset_id)) >0 ) {
    volset_id[UDF_VOLSET_ID_SIZE]='\0';
    printf("-- Volume set id: %s\n", volset_id);
  }
  printf("-- Partition number: %d\n", udf_get_part_number(p_udf));

  /* Recursively extract files */
  r = udf_extract_files(p_udf, p_udf_root, "");

  goto out;

try_iso:
  p_iso = iso9660_open_ext(argv[1], ISO_EXTENSION_ALL);
  if (p_iso == NULL) {
    fprintf(stderr, "Unable to open image '%s'.\n", argv[1]);
    r = 1;
    goto out;
  }
  i_joliet_level = iso9660_ifs_get_joliet_level(p_iso);

  /* Show basic ISO9660 info from the Primary Volume Descriptor. */
  print_vd_info("Application", iso9660_ifs_get_application_id);
  print_vd_info("Preparer   ", iso9660_ifs_get_preparer_id);
  print_vd_info("Publisher  ", iso9660_ifs_get_publisher_id);
  print_vd_info("System     ", iso9660_ifs_get_system_id);
  print_vd_info("Volume     ", iso9660_ifs_get_volume_id);
  print_vd_info("Volume Set ", iso9660_ifs_get_volumeset_id);

  r = iso_extract_files(p_iso, "");

out:
  if (p_iso != NULL)
    iso9660_close(p_iso);
  if (p_udf != NULL)
    udf_close(p_udf);

  return r;
}
