/*
  Copyright (C) 2004-2009, 2011-2013
  Rocky Bernstein <rocky@gnu.org>

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

/* This file contains generic implementations of device-driver routines.
*/

#ifdef HAVE_CONFIG_H
# include "config.h"
# define __CDIO_CONFIG_H__ 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <fcntl.h>
#include <limits.h>

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include <cdio/sector.h>
#include <cdio/util.h>
#include <cdio/logging.h>
#include "cdio_assert.h"
#include "cdio_private.h"
#include "_cdio_stdio.h"
#include "filemode.h"

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

/* If available and LFS is enabled, try to use lseek64 */
#if defined(HAVE_LSEEK64) && defined(_FILE_OFFSET_BITS) && (_FILE_OFFSET_BITS == 64)
#if defined(_MSC_VER)
#include <io.h>
#endif
#define CDIO_LSEEK lseek64
#else
#define CDIO_LSEEK lseek
#endif

/*!
  Eject media -- there's nothing to do here. We always return -2.
  Should we also free resources?
 */
int
cdio_generic_unimplemented_eject_media (void *p_user_data) {
  /* Sort of a stub here. Perhaps log a message? */
  return DRIVER_OP_UNSUPPORTED;
}

/*!
  Set the blocksize for subsequent reads.
*/
int
cdio_generic_unimplemented_set_blocksize (void *p_user_data,
                                          uint16_t i_blocksize) {
  /* Sort of a stub here. Perhaps log a message? */
  return DRIVER_OP_UNSUPPORTED;
}

/*!
  Set the drive speed.
*/
int
cdio_generic_unimplemented_set_speed (void *p_user_data, int i_speed) {
  /* Sort of a stub here. Perhaps log a message? */
  return DRIVER_OP_UNSUPPORTED;
}


/*!
  Release and free resources associated with cd.
 */
void
cdio_generic_free (void *p_user_data)
{
  generic_img_private_t *p_env = p_user_data;

  if (NULL == p_env) return;
  if (p_env->source_name) free (p_env->source_name);

  if (NULL != p_env->cdtext) {
      cdtext_destroy(p_env->cdtext);
      free(p_env->cdtext);
      p_env->cdtext = NULL;
  }

  if (p_env->fd >= 0)
    close (p_env->fd);

  if (p_env->scsi_tuple != NULL)
    free (p_env->scsi_tuple);

  free (p_env);
}

/*!
  Initialize CD device.
 */
bool
cdio_generic_init (void *user_data, int open_flags)
{
  generic_img_private_t *p_env = user_data;
  if (p_env->init) {
    cdio_warn ("init called more than once");
    return false;
  }

  p_env->fd = open (p_env->source_name, open_flags, 0);

  if (p_env->fd < 0)
    {
      cdio_warn ("open (%s): %s", p_env->source_name, strerror (errno));
      return false;
    }

  p_env->init = true;
  p_env->toc_init = false;
  p_env->cdtext  = NULL;
  p_env->scsi_tuple  = NULL;
  p_env->b_cdtext_error = false;
  p_env->u_joliet_level = 0;  /* Assume no Joliet extensions initally */
  return true;
}

/*!
   Reads a single form1 sector from cd device into data starting
   from lsn.
 */
driver_return_code_t
cdio_generic_read_form1_sector (void * user_data, void *data, lsn_t lsn)
{
  if (0 > cdio_generic_lseek(user_data, CDIO_CD_FRAMESIZE*lsn, SEEK_SET))
    return DRIVER_OP_ERROR;
  return cdio_generic_read(user_data, data, CDIO_CD_FRAMESIZE);
}

/*!
  Reads into buf the next size bytes.
  Returns -1 on error.
  Is in fact libc's lseek()/lseek64().
*/
off_t
cdio_generic_lseek (void *user_data, off_t offset, int whence)
{
  generic_img_private_t *p_env = user_data;
  return CDIO_LSEEK(p_env->fd, offset, whence);
}

/*!
  Reads into buf the next size bytes.
  Returns -1 on error.
  Is in fact libc's read().
*/
ssize_t
cdio_generic_read (void *user_data, void *buf, size_t size)
{
  generic_img_private_t *p_env = user_data;
  return read(p_env->fd, buf, size);
}

/*!
  Release and free resources associated with stream or disk image.
*/
void
cdio_generic_stdio_free (void *p_user_data)
{
  generic_img_private_t *p_env = p_user_data;

  if (NULL == p_env) return;
  if (NULL != p_env->source_name)
    free (p_env->source_name);

  if (p_env->data_source)
    cdio_stdio_destroy (p_env->data_source);
}


/*!
  Return true if source_name could be a device containing a CD-ROM.
*/
bool
cdio_is_device_generic(const char *source_name)
{
  struct stat buf;
  if (0 != stat(source_name, &buf)) {
    cdio_warn ("Can't get file status for %s:\n%s", source_name,
                strerror(errno));
    return false;
  }
  return (S_ISBLK(buf.st_mode) || S_ISCHR(buf.st_mode));
}

/*!
  Like above, but don't give a warning device doesn't exist.
*/
bool
cdio_is_device_quiet_generic(const char *source_name)
{
  struct stat buf;
  if (0 != stat(source_name, &buf)) {
    return false;
  }
  return (S_ISBLK(buf.st_mode) || S_ISCHR(buf.st_mode));
}

/*!
  Add/allocate a drive to the end of drives.
  Use cdio_free_device_list() to free this device_list.
*/
void
cdio_add_device_list(char **device_list[], const char *drive,
                     unsigned int *num_drives)
{
  if (NULL != drive) {
    unsigned int j;
    char real_device_1[PATH_MAX];
    char real_device_2[PATH_MAX];
    cdio_realpath(drive, real_device_1);
    /* Check if drive is already in list. */
    for (j=0; j<*num_drives; j++) {
      cdio_realpath((*device_list)[j], real_device_2);
      if (strcmp(real_device_1, real_device_2) == 0) break;
    }

    if (j==*num_drives) {
      /* Drive not in list. Add it. */
      (*num_drives)++;
      *device_list = realloc(*device_list, (*num_drives) * sizeof(char *));
      cdio_debug("Adding drive %s to list of devices", drive);
      (*device_list)[*num_drives-1] = strdup(drive);
      }

  } else {
    (*num_drives)++;
    if (*device_list) {
      *device_list = realloc(*device_list, (*num_drives) * sizeof(char *));
    } else {
      *device_list = malloc((*num_drives) * sizeof(char *));
    }
    cdio_debug("Adding NULL to end of drive list of size %d", (*num_drives)-1);
    (*device_list)[*num_drives-1] = NULL;
  }
}

/*
  Get cdtext information in p_user_data for track i_track.
  For disc information i_track is 0.

  Return the CD-TEXT or NULL if obj is NULL, CD-TEXT information does
  not exist, or we don't know how to get this implemented.
*/
cdtext_t *
get_cdtext_generic (void *p_user_data)
{
  generic_img_private_t *p_env = p_user_data;
  uint8_t *p_cdtext_data = NULL;
  size_t  len;

  if (!p_env) return NULL;

  if (NULL == p_env->cdtext) {
    p_cdtext_data = read_cdtext_generic (p_env);

    if (NULL != p_cdtext_data) {
      len = CDIO_MMC_GET_LEN16(p_cdtext_data)-2;
      p_env->cdtext = cdtext_init();

      if(len <= 0 || 0 != cdtext_data_init (p_env->cdtext, &p_cdtext_data[4], len)) {
        p_env->b_cdtext_error = true;
        cdtext_destroy (p_env->cdtext);
        free(p_env->cdtext);
        p_env->cdtext = NULL;
      }

      free(p_cdtext_data);
    }
  }

  return p_env->cdtext;
}

/*!
  Get disc type associated with cd object.
*/
discmode_t
get_discmode_generic (void *p_user_data )
{
  generic_img_private_t *p_env = p_user_data;

  /* See if this is a DVD. */
  cdio_dvd_struct_t dvd;  /* DVD READ STRUCT for layer 0. */

  dvd.physical.type = CDIO_DVD_STRUCT_PHYSICAL;
  dvd.physical.layer_num = 0;
  if (0 == mmc_get_dvd_struct_physical (p_env->cdio, &dvd)) {
    switch(dvd.physical.layer[0].book_type) {
    case CDIO_DVD_BOOK_DVD_ROM:    return CDIO_DISC_MODE_DVD_ROM;
    case CDIO_DVD_BOOK_DVD_RAM:    return CDIO_DISC_MODE_DVD_RAM;
    case CDIO_DVD_BOOK_DVD_R:      return CDIO_DISC_MODE_DVD_R;
    case CDIO_DVD_BOOK_DVD_RW:     return CDIO_DISC_MODE_DVD_RW;
    case CDIO_DVD_BOOK_HD_DVD_ROM: return CDIO_DISC_MODE_HD_DVD_ROM;
    case CDIO_DVD_BOOK_HD_DVD_RAM: return CDIO_DISC_MODE_HD_DVD_RAM;
    case CDIO_DVD_BOOK_HD_DVD_R:   return CDIO_DISC_MODE_HD_DVD_R;
    case CDIO_DVD_BOOK_DVD_PR:     return CDIO_DISC_MODE_DVD_PR;
    case CDIO_DVD_BOOK_DVD_PRW:    return CDIO_DISC_MODE_DVD_PRW;
    case CDIO_DVD_BOOK_DVD_PRW_DL: return CDIO_DISC_MODE_DVD_PRW_DL;
    case CDIO_DVD_BOOK_DVD_PR_DL:  return CDIO_DISC_MODE_DVD_PR_DL;
    default: return CDIO_DISC_MODE_DVD_OTHER;
    }
  }

  return get_discmode_cd_generic(p_user_data);
}

/*!
  Get disc type associated with cd object.
*/
discmode_t
get_discmode_cd_generic (void *p_user_data )
{
  generic_img_private_t *p_env = p_user_data;
  track_t i_track;
  discmode_t discmode=CDIO_DISC_MODE_NO_INFO;

  if (!p_env->toc_init)
    p_env->cdio->op.read_toc (p_user_data);

  if (!p_env->toc_init)
    return CDIO_DISC_MODE_NO_INFO;

  for (i_track = p_env->i_first_track;
       i_track < p_env->i_first_track + p_env->i_tracks ;
       i_track ++) {
    track_format_t track_fmt =
      p_env->cdio->op.get_track_format(p_env, i_track);

    switch(track_fmt) {
    case TRACK_FORMAT_AUDIO:
      switch(discmode) {
        case CDIO_DISC_MODE_NO_INFO:
          discmode = CDIO_DISC_MODE_CD_DA;
          break;
        case CDIO_DISC_MODE_CD_DA:
        case CDIO_DISC_MODE_CD_MIXED:
        case CDIO_DISC_MODE_ERROR:
          /* No change*/
          break;
      default:
          discmode = CDIO_DISC_MODE_CD_MIXED;
      }
      break;
    case TRACK_FORMAT_XA:
      switch(discmode) {
        case CDIO_DISC_MODE_NO_INFO:
          discmode = CDIO_DISC_MODE_CD_XA;
          break;
        case CDIO_DISC_MODE_CD_XA:
        case CDIO_DISC_MODE_CD_MIXED:
        case CDIO_DISC_MODE_ERROR:
          /* No change*/
          break;
      default:
        discmode = CDIO_DISC_MODE_CD_MIXED;
      }
      break;
    case TRACK_FORMAT_CDI:
    case TRACK_FORMAT_DATA:
      switch(discmode) {
        case CDIO_DISC_MODE_NO_INFO:
          discmode = CDIO_DISC_MODE_CD_DATA;
          break;
        case CDIO_DISC_MODE_CD_DATA:
        case CDIO_DISC_MODE_CD_MIXED:
        case CDIO_DISC_MODE_ERROR:
          /* No change*/
          break;
      default:
        discmode = CDIO_DISC_MODE_CD_MIXED;
      }
      break;
    case TRACK_FORMAT_ERROR:
    default:
      discmode = CDIO_DISC_MODE_ERROR;
    }
  }
  return discmode;
}

/*!
  Return the number of of the first track.
  CDIO_INVALID_TRACK is returned on error.
*/
track_t
get_first_track_num_generic(void *p_user_data)
{
  const generic_img_private_t *p_env = p_user_data;

  if (!p_env->toc_init)
    p_env->cdio->op.read_toc (p_user_data);

  return p_env->toc_init ? p_env->i_first_track : CDIO_INVALID_TRACK;
}


/*!
  Return the number of tracks in the current medium.
*/
track_t
get_num_tracks_generic(void *p_user_data)
{
  generic_img_private_t *p_env = p_user_data;

  if (!p_env->toc_init)
    p_env->cdio->op.read_toc (p_user_data);

  return p_env->toc_init ? p_env->i_tracks : CDIO_INVALID_TRACK;
}


/*!
  Read CD-Text information for a CdIo_t object .

  return pointer to raw cdtext on success,
  NULL on failure
  free when done and not NULL
*/
uint8_t *
read_cdtext_generic(void *p_env)
{
  generic_img_private_t *p_user_data = p_env;
  return mmc_read_cdtext_private (p_user_data,
                                  p_user_data->cdio->op.run_mmc_cmd
                                  );
}

/*! Return number of channels in track: 2 or 4; -2 if not
  implemented or -1 for error.
  Not meaningful if track is not an audio track.
*/
int
get_track_channels_generic(const void *p_user_data, track_t i_track)
{
  const generic_img_private_t *p_env = p_user_data;
  return p_env->track_flags[i_track].channels;
}

/*! Return 1 if copy is permitted on the track, 0 if not, or -1 for error.
  Is this meaningful if not an audio track?
*/
track_flag_t
get_track_copy_permit_generic(void *p_user_data, track_t i_track)
{
  const generic_img_private_t *p_env = p_user_data;
  return p_env->track_flags[i_track].copy_permit;
}

/*! Return 1 if track has pre-emphasis, 0 if not, or -1 for error.
  Is this meaningful if not an audio track?

  pre-emphasis is a non linear frequency response.
*/
track_flag_t
get_track_preemphasis_generic(const void *p_user_data, track_t i_track)
{
  const generic_img_private_t *p_env = p_user_data;
  return p_env->track_flags[i_track].preemphasis;
}

void
set_track_flags(track_flags_t *p_track_flag, uint8_t i_flag)
{
  p_track_flag->preemphasis = ( i_flag & CDIO_TRACK_FLAG_PRE_EMPHASIS )
    ? CDIO_TRACK_FLAG_TRUE : CDIO_TRACK_FLAG_FALSE;

  p_track_flag->copy_permit = ( i_flag & CDIO_TRACK_FLAG_COPY_PERMITTED )
    ? CDIO_TRACK_FLAG_TRUE : CDIO_TRACK_FLAG_FALSE;

  p_track_flag->channels = ( i_flag & CDIO_TRACK_FLAG_FOUR_CHANNEL_AUDIO )
    ? 4 : 2;
}

driver_return_code_t
read_data_sectors_generic (void *p_user_data, void *p_buf, lsn_t i_lsn,
                           uint16_t i_blocksize, uint32_t i_blocks)
{
  int rc;
  if (0 > cdio_generic_lseek(p_user_data, i_blocksize*i_lsn, SEEK_SET))
    return DRIVER_OP_ERROR;
  rc = cdio_generic_read(p_user_data, p_buf, i_blocksize*i_blocks);
  if (rc > 0) return DRIVER_OP_SUCCESS;
  return DRIVER_OP_ERROR;
}


/*
 * Local variables:
 *  c-file-style: "gnu"
 *  tab-width: 8
 *  indent-tabs-mode: nil
 * End:
 */
