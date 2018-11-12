/*
  Copyright (C) 2003, 2004-2005, 2008-2011, 2014
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

/* This file contains FreeBSD-specific code and implements low-level
   control of the CD drive. Culled initially I think from xine's or
   mplayer's FreeBSD code with lots of modifications.
*/

#ifdef HAVE_CONFIG_H
# include "config.h"
# define __CDIO_CONFIG_H__ 1
#endif

#include "freebsd.h"

#ifdef HAVE_FREEBSD_CDROM

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

#include <netinet/in.h>

/* For freebsd_dev_lock() */
#include <sys/file.h>

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif
#ifdef _HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif
#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif

#include <cdio/sector.h>

static lba_t get_track_lba_freebsd(void *p_user_data, track_t i_track);

static access_mode_t
str_to_access_mode_freebsd(const char *psz_access_mode)
{
  const access_mode_t default_access_mode = DEFAULT_FREEBSD_AM;

  if (NULL==psz_access_mode) return default_access_mode;

  if (!strcmp(psz_access_mode, "ioctl"))
    return _AM_IOCTL;
  else if (!strcmp(psz_access_mode, "CAM"))
    return _AM_CAM;
  else if (!strcmp(psz_access_mode, "MMC_RDWR"))
    return _AM_MMC_RDWR;
  else if (!strcmp(psz_access_mode, "MMC_RDWR_EXCL"))
    return _AM_MMC_RDWR_EXCL;
  else {
    cdio_warn ("unknown access type: %s. Default used.",
	       psz_access_mode);
    return default_access_mode;
  }
}

static void
free_freebsd (void *p_obj)
{
  _img_private_t *p_env = p_obj;

  if (NULL == p_env) return;

  if (NULL != p_env->device) free(p_env->device);

  switch (p_env->access_mode) {
    case _AM_CAM:
    case _AM_MMC_RDWR:
    case _AM_MMC_RDWR_EXCL:
      free_freebsd_cam(p_env);
      break;
    case _AM_IOCTL:
      cdio_generic_free(p_obj);
      break;
    case _AM_NONE:
      break;
  }
}

/* Check a drive to see if it is a CD-ROM
   Return 1 if a CD-ROM. 0 if it exists but isn't a CD-ROM drive
   and -1 if no device exists .
*/
static bool
cdio_is_cdrom(char *drive, char *mnttype)
{
  return cdio_is_cdrom_freebsd_ioctl(drive, mnttype);
}

/*!
   Reads i_blocks of audio sectors from cd device into data starting from lsn.
   Returns 0 if no error.
 */
static driver_return_code_t
read_audio_sectors_freebsd (void *p_user_data, void *p_buf, lsn_t i_lsn,
			     unsigned int i_blocks)
{
  _img_private_t *p_env = p_user_data;
  switch (p_env->access_mode) {
    case _AM_CAM:
    case _AM_MMC_RDWR:
    case _AM_MMC_RDWR_EXCL:
      return mmc_read_sectors( p_env->gen.cdio, p_buf, i_lsn,
                                  CDIO_MMC_READ_TYPE_CDDA, i_blocks);
    case _AM_IOCTL:
      return read_audio_sectors_freebsd_ioctl(p_user_data, p_buf, i_lsn,
					      i_blocks);
    case _AM_NONE:
      cdio_info ("access mode not set");
      return DRIVER_OP_ERROR;
  }
  return DRIVER_OP_ERROR;
}

/*!
   Reads a single mode2 sector from cd device into data starting
   from i_lsn. Returns 0 if no error.
 */
static driver_return_code_t
read_mode2_sector_freebsd (void *p_user_data, void *data, lsn_t i_lsn,
			   bool b_form2)
{
  _img_private_t *p_env = p_user_data;

  switch (p_env->access_mode) {
    case _AM_CAM:
    case _AM_MMC_RDWR:
    case _AM_MMC_RDWR_EXCL:
    return read_mode2_sector_freebsd_cam(p_env, data, i_lsn, b_form2);
    case _AM_IOCTL:
      return read_mode2_sector_freebsd_ioctl(p_env, data, i_lsn, b_form2);
    case _AM_NONE:
      cdio_info ("access mode not set");
      return DRIVER_OP_ERROR;
  }
  return DRIVER_OP_ERROR;
}

/*!
   Reads i_blocks of mode2 sectors from cd device into data starting
   from lsn.
 */
static driver_return_code_t
read_mode2_sectors_freebsd (void *p_user_data, void *p_data, lsn_t i_lsn,
			    bool b_form2, unsigned int i_blocks)
{
  _img_private_t *p_env = p_user_data;

  if ( (p_env->access_mode == _AM_CAM ||
	p_env->access_mode == _AM_MMC_RDWR ||
	p_env->access_mode == _AM_MMC_RDWR_EXCL)
       && b_form2 ) {
    /* We have a routine that covers this case without looping. */
    return read_mode2_sectors_freebsd_cam(p_env, p_data, i_lsn, i_blocks);
  } else {
    unsigned int i;
    uint16_t i_blocksize = b_form2 ? M2RAW_SECTOR_SIZE : CDIO_CD_FRAMESIZE;

    /* For each frame, pick out the data part we need */
    for (i = 0; i < i_blocks; i++) {
      int retval = read_mode2_sector_freebsd (p_env,
					       ((char *)p_data) +
					       (i_blocksize * i),
					       i_lsn + i, b_form2);
      if (retval) return retval;
    }
  }
  return DRIVER_OP_SUCCESS;
}

/*!
   Return the size of the CD in logical block address (LBA) units.
  @return the lsn. On error return CDIO_INVALID_LSN.
 */
static lsn_t
get_disc_last_lsn_freebsd (void *p_obj)
{
  _img_private_t *p_env = p_obj;

  if (!p_env) return CDIO_INVALID_LSN;

  switch (p_env->access_mode) {
    case _AM_CAM:
    case _AM_MMC_RDWR:
    case _AM_MMC_RDWR_EXCL:
      return get_disc_last_lsn_mmc(p_env);
    case _AM_IOCTL:
      return get_disc_last_lsn_freebsd_ioctl(p_env);
    case _AM_NONE:
      cdio_info ("access mode not set");
      return DRIVER_OP_ERROR;
  }
  return DRIVER_OP_ERROR;
}

/*!
  Set the arg "key" with "value" in the source device.
  Currently "source" and "access-mode" are valid keys.
  "source" sets the source device in I/O operations
  "access-mode" sets the the method of CD access

  DRIVER_OP_SUCCESS is returned if no error was found,
  and nonzero if there as an error.
*/
static driver_return_code_t
set_arg_freebsd (void *p_user_data, const char key[], const char value[])
{
  _img_private_t *p_env = p_user_data;

  if (!strcmp (key, "source"))
    {
      if (!value) return DRIVER_OP_ERROR;
      free (p_env->gen.source_name);
      p_env->gen.source_name = strdup (value);
    }
  else if (!strcmp (key, "access-mode"))
    {
      p_env->access_mode = str_to_access_mode_freebsd(value);
      if (p_env->access_mode == _AM_CAM && !p_env->b_cam_init)
	return init_freebsd_cam(p_env)
	  ? DRIVER_OP_SUCCESS : DRIVER_OP_ERROR;
    }
  else return DRIVER_OP_ERROR;

  return DRIVER_OP_SUCCESS;

}

/* Set CD-ROM drive speed */
static int
set_speed_freebsd (void *p_user_data, int i_speed)
{
  const _img_private_t *p_env = p_user_data;

  if (!p_env) return -1;
#ifdef CDRIOCREADSPEED
  i_speed *= 177;
  return ioctl(p_env->gen.fd, CDRIOCREADSPEED, &i_speed);
#else
  return -2;
#endif
}

/*!
  Read and cache the CD's Track Table of Contents and track info.
  Return false if unsuccessful;
*/
static bool
read_toc_freebsd (void *p_user_data)
{
  _img_private_t *p_env = p_user_data;
  track_t i, j;

  /* read TOC header */
  if ( ioctl(p_env->gen.fd, CDIOREADTOCHEADER, &p_env->tochdr) == -1 ) {
    cdio_warn("error in ioctl(CDIOREADTOCHEADER): %s\n", strerror(errno));
    return false;
  }

  p_env->gen.i_first_track = p_env->tochdr.starting_track;
  p_env->gen.i_tracks      = p_env->tochdr.ending_track -
    p_env->gen.i_first_track + 1;

  j=0;
  for (i=p_env->gen.i_first_track; i<=p_env->gen.i_tracks; i++, j++) {
    struct ioc_read_toc_single_entry *p_toc =
      &(p_env->tocent[i-p_env->gen.i_first_track]);
    p_toc->track = i;
    p_toc->address_format = CD_LBA_FORMAT;

    if ( ioctl(p_env->gen.fd, CDIOREADTOCENTRY, p_toc) ) {
      cdio_warn("%s %d: %s\n",
		 "error in ioctl CDROMREADTOCENTRY for track",
		 i, strerror(errno));
      return false;
    }

    set_track_flags(&(p_env->gen.track_flags[i]), p_toc->entry.control);

  }

  p_env->tocent[j].track          = CDIO_CDROM_LEADOUT_TRACK;
  p_env->tocent[j].address_format = CD_LBA_FORMAT;
  if ( ioctl(p_env->gen.fd, CDIOREADTOCENTRY, &(p_env->tocent[j]) ) ){
    cdio_warn("%s: %s\n",
	       "error in ioctl CDROMREADTOCENTRY for leadout track",
	       strerror(errno));
    return false;
  }

  p_env->gen.toc_init = true;
  return true;
}

/*!
  Get the volume of an audio CD.

  @param p_cdio the CD object to be acted upon.
*/
static driver_return_code_t
audio_get_volume_freebsd (void *p_user_data,
			  /*out*/ cdio_audio_volume_t *p_volume)
{

  const _img_private_t *p_env = p_user_data;
  return ioctl(p_env->gen.fd, CDIOCGETVOL, p_volume);
}

/*!
  Pause playing CD through analog output

  @param p_cdio the CD object to be acted upon.
*/
static driver_return_code_t
audio_pause_freebsd (void *p_user_data)
{

  const _img_private_t *p_env = p_user_data;
  return ioctl(p_env->gen.fd, CDIOCPAUSE);
}

/*!
  Playing starting at given MSF through analog output

  @param p_cdio the CD object to be acted upon.
*/
static driver_return_code_t
audio_play_msf_freebsd (void *p_user_data, msf_t *p_start_msf,
			msf_t *p_end_msf)
{
  const _img_private_t *p_env = p_user_data;
  struct ioc_play_msf freebsd_play_msf;

  freebsd_play_msf.start_m = cdio_from_bcd8(p_start_msf->m);
  freebsd_play_msf.start_s = cdio_from_bcd8(p_start_msf->s);
  freebsd_play_msf.start_f = cdio_from_bcd8(p_start_msf->f);

  freebsd_play_msf.end_m = cdio_from_bcd8(p_end_msf->m);
  freebsd_play_msf.end_s = cdio_from_bcd8(p_end_msf->s);
  freebsd_play_msf.end_f = cdio_from_bcd8(p_end_msf->f);

  return ioctl(p_env->gen.fd, CDIOCPLAYMSF, &freebsd_play_msf);
}

/*!
  Playing CD through analog output at the desired track and index

  @param p_user_data the CD object to be acted upon.
  @param p_track_index location to start/end.
*/
static driver_return_code_t
audio_play_track_index_freebsd (void *p_user_data,
				cdio_track_index_t *p_track_index)
{
  const _img_private_t *p_env = p_user_data;
  msf_t start_msf;
  msf_t end_msf;
  struct ioc_play_msf freebsd_play_msf;
  lsn_t i_lsn = get_track_lba_freebsd(p_user_data,
				      p_track_index->i_start_track);

  cdio_lsn_to_msf(i_lsn, &start_msf);
  i_lsn = get_track_lba_freebsd(p_user_data, p_track_index->i_end_track);
  cdio_lsn_to_msf(i_lsn, &end_msf);

  freebsd_play_msf.start_m = start_msf.m;
  freebsd_play_msf.start_s = start_msf.s;
  freebsd_play_msf.start_f = start_msf.f;

  freebsd_play_msf.end_m = end_msf.m;
  freebsd_play_msf.end_s = end_msf.s;
  freebsd_play_msf.end_f = end_msf.f;

  return ioctl(p_env->gen.fd, CDIOCPLAYMSF, &freebsd_play_msf);

}

/*!
  Read Audio Subchannel information

  @param p_user_data the CD object to be acted upon.
  @param p_subchannel returned information
*/
#if 1
static driver_return_code_t
audio_read_subchannel_freebsd (void *p_user_data,
			       /*out*/ cdio_subchannel_t *p_subchannel)
{
  const _img_private_t *p_env = p_user_data;
  int i_rc;
  struct cd_sub_channel_info bsdinfo;
  struct ioc_read_subchannel read_subchannel;
  memset(& bsdinfo, 0, sizeof(struct cd_sub_channel_info));
  read_subchannel.address_format = CD_MSF_FORMAT;
  read_subchannel.data_format = CD_CURRENT_POSITION;
  read_subchannel.track = 0;
  read_subchannel.data_len = sizeof(struct cd_sub_channel_info);
  read_subchannel.data = & bsdinfo;
  i_rc = ioctl(p_env->gen.fd, CDIOCREADSUBCHANNEL, &read_subchannel);
  if (0 == i_rc) {
    p_subchannel->audio_status = bsdinfo.header.audio_status;
    p_subchannel->address      = bsdinfo.what.position.addr_type;

    p_subchannel->control      = bsdinfo.what.position.control;
    p_subchannel->track        = bsdinfo.what.position.track_number;
    p_subchannel->index        = bsdinfo.what.position.index_number;

    p_subchannel->abs_addr.m = cdio_to_bcd8 (bsdinfo.what.position.absaddr.msf.minute);
    p_subchannel->abs_addr.s = cdio_to_bcd8 (bsdinfo.what.position.absaddr.msf.second);
    p_subchannel->abs_addr.f = cdio_to_bcd8 (bsdinfo.what.position.absaddr.msf.frame);
    p_subchannel->rel_addr.m = cdio_to_bcd8 (bsdinfo.what.position.reladdr.msf.minute);
    p_subchannel->rel_addr.s = cdio_to_bcd8 (bsdinfo.what.position.reladdr.msf.second);
    p_subchannel->rel_addr.f = cdio_to_bcd8 (bsdinfo.what.position.reladdr.msf.frame);
 }
  return i_rc;
}
#endif

/*!
  Resume playing an audio CD.

  @param p_cdio the CD object to be acted upon.

*/
static driver_return_code_t
audio_resume_freebsd (void *p_user_data)
{
  const _img_private_t *p_env = p_user_data;
  return ioctl(p_env->gen.fd, CDIOCRESUME, 0);
}

/*!
  Set the volume of an audio CD.

  @param p_cdio the CD object to be acted upon.

*/
static driver_return_code_t
audio_set_volume_freebsd (void *p_user_data,
			  cdio_audio_volume_t *p_volume)
{
  const _img_private_t *p_env = p_user_data;
  return ioctl(p_env->gen.fd, CDIOCSETVOL, p_volume);
}

/*!
  Eject media. Return 1 if successful, 0 otherwise.
 */
static int
eject_media_freebsd (void *p_user_data)
{
  _img_private_t *p_env = p_user_data;

  switch (p_env->access_mode) {
    case _AM_CAM:
    case _AM_MMC_RDWR:
    case _AM_MMC_RDWR_EXCL:
      return eject_media_freebsd_cam(p_env);
    case _AM_IOCTL:
      return eject_media_freebsd_ioctl(p_env);
    case _AM_NONE:
      cdio_info ("access mode not set");
      return 0;
  }
  return 0;
}

/*!
  Stop playing an audio CD.

  @param p_user_data the CD object to be acted upon.

*/
static driver_return_code_t
audio_stop_freebsd (void *p_user_data)
{
  const _img_private_t *p_env = p_user_data;
  return ioctl(p_env->gen.fd, CDIOCSTOP);
}

/*!
  Produce a text composed from the system SCSI address tuple according to
  habits of Linux 2.4 and 2.6 :  "Bus,Host,Channel,Target,Lun" and store
  it in generic_img_private_t.scsi_tuple.
  Channel has no meaning on FreeBSD. Expect it to be 0. It is only in
  the text to avoid an unnecessary difference in format.
  Bus and Host will always be the same.
  To be accessed via cdio_get_arg("scsi-tuple-freebsd") or "scsi-tuple".
  For simplicity the FreeBSD driver also replies on "scsi-tuple-linux".
  Drivers which implement this code have to return 5 valid decimal numbers
  separated by comma, or empty text if no such numbers are available.
  @return   1=success , 0=failure
*/
static int
set_scsi_tuple_freebsd(_img_private_t *env)
{
  int bus_no = -1, host_no = -1, channel_no = -1, target_no = -1, lun_no = -1;
  int ret;
  char tuple[160];

  ret = obtain_scsi_adr_freebsd_cam(env->gen.source_name,
                                   &bus_no, &host_no, &channel_no,
                                   &target_no, &lun_no);
  if (ret != 1)
    return 0;
  if (env->gen.scsi_tuple != NULL)
    free (env->gen.scsi_tuple);
  env->gen.scsi_tuple = NULL;
  if (bus_no < 0 || host_no < 0 || channel_no < 0 || target_no < 0 ||
      lun_no < 0) {
    env->gen.scsi_tuple = strdup("");
    return 0;
  }
  sprintf(tuple, "%d,%d,%d,%d,%d",
          bus_no, host_no, channel_no, target_no, lun_no);
  env->gen.scsi_tuple = strdup(tuple);
  return 1;
}

static bool
is_mmc_supported(void *user_data)
{
    _img_private_t *env = user_data;
    switch (env->access_mode) {
      case _AM_IOCTL:
      case _AM_NONE:
	return false;
      case _AM_CAM:
      case _AM_MMC_RDWR:
      case _AM_MMC_RDWR_EXCL:
	return true;
    }
    /* Not reached. */
    return false;
}


/*!
  Return the value associated with the key "arg".
*/
static const char *
get_arg_freebsd (void *user_data, const char key[])
{
  _img_private_t *env = user_data;

  if (!strcmp (key, "source")) {
    return env->gen.source_name;
  } else if (!strcmp (key, "access-mode")) {
    switch (env->access_mode) {
    case _AM_IOCTL:
      return "ioctl";
    case _AM_CAM:
      return "CAM";
    case _AM_MMC_RDWR:
      return "MMC_RDWR";
    case _AM_MMC_RDWR_EXCL:
      return "MMC_RDWR_EXCL";
    case _AM_NONE:
      return "no access method";
    }
  } else if (strcmp (key, "scsi-tuple") == 0) {
    if (env->gen.scsi_tuple == NULL)
      set_scsi_tuple_freebsd(env);
    return env->gen.scsi_tuple;
  } else if (!strcmp (key, "mmc-supported?")) {
    return is_mmc_supported(user_data) ? "true" : "false";
  }
  return NULL;
}

/*!
  Return the media catalog number MCN.

  Note: string is malloc'd so caller should free() then returned
  string when done with it.

  FIXME: This is just a guess.

 */
static char *
get_mcn_freebsd (const void *p_user_data) {
  const _img_private_t *p_env = p_user_data;

  switch (p_env->access_mode) {
    case _AM_CAM:
    case _AM_MMC_RDWR:
    case _AM_MMC_RDWR_EXCL:
      return mmc_get_mcn(p_env->gen.cdio);
    case _AM_IOCTL:
      return mmc_get_mcn(p_env->gen.cdio);
    case _AM_NONE:
      cdio_info ("access mode not set");
      return NULL;
  }
  return NULL;
}

/*!
  Return the international standard recording code ISRC.

  Note: string is malloc'd so caller should free() then returned
  string when done with it.

 */
static char *
get_track_isrc_freebsd (const void *p_user_data,
			track_t i_track) {
  const _img_private_t *p_env = p_user_data;

  switch (p_env->access_mode) {
    case _AM_CAM:
    case _AM_MMC_RDWR:
    case _AM_MMC_RDWR_EXCL:
      return mmc_get_track_isrc(p_env->gen.cdio, i_track);
    case _AM_IOCTL:
      return mmc_get_track_isrc(p_env->gen.cdio, i_track);
    case _AM_NONE:
      cdio_info ("access mode not set");
      return NULL;
  }
  return NULL;
}

static void
get_drive_cap_freebsd (const void *p_user_data,
		       cdio_drive_read_cap_t  *p_read_cap,
		       cdio_drive_write_cap_t *p_write_cap,
		       cdio_drive_misc_cap_t  *p_misc_cap)
{
  const _img_private_t *p_env = p_user_data;

  switch (p_env->access_mode) {
    case _AM_CAM:
    case _AM_MMC_RDWR:
    case _AM_MMC_RDWR_EXCL:
      get_drive_cap_mmc (p_user_data, p_read_cap, p_write_cap, p_misc_cap);
    case _AM_IOCTL:
      cdio_info ("get_drive_cap not supported in ioctl access mode");
      return;
    case _AM_NONE:
      cdio_info ("access mode not set");
      return;
  }
}

/*!
  Run a SCSI MMC command.

  p_user_data   internal CD structure.
  i_timeout_ms  time in milliseconds we will wait for the command
                to complete. If this value is -1, use the default
		time-out value.
  i_cdb	        Size of p_cdb
  p_cdb	        CDB bytes.
  e_direction	direction the transfer is to go.
  i_buf	        Size of buffer
  p_buf	        Buffer for data, both sending and receiving
 */
static driver_return_code_t
run_mmc_cmd_freebsd( void *p_user_data, unsigned int i_timeout_ms,
		     unsigned int i_cdb, const mmc_cdb_t *p_cdb,
		     cdio_mmc_direction_t e_direction,
		     unsigned int i_buf, /*in/out*/ void *p_buf )
{
  const _img_private_t *p_env = p_user_data;
  int ret;

  switch (p_env->access_mode) {
    case _AM_CAM:
    case _AM_MMC_RDWR:
    case _AM_MMC_RDWR_EXCL:
      ret = run_mmc_cmd_freebsd_cam( p_user_data, i_timeout_ms, i_cdb, p_cdb,
                                     e_direction, i_buf, p_buf );
      if (ret != 0)
        return DRIVER_OP_ERROR;
      return 0;
    case _AM_IOCTL:
      return DRIVER_OP_UNSUPPORTED;
    case _AM_NONE:
      cdio_info ("access mode not set");
      return DRIVER_OP_ERROR;
  }
  return DRIVER_OP_ERROR;
}

/*!
  Get format of track.

  FIXME: We're just guessing this from the GNU/Linux code.

*/
static track_format_t
get_track_format_freebsd(void *p_user_data, track_t i_track)
{
  _img_private_t *p_env = p_user_data;

  if (!p_env->gen.toc_init) read_toc_freebsd (p_user_data) ;

  if (i_track > TOTAL_TRACKS || i_track == 0)
    return TRACK_FORMAT_ERROR;

  i_track -= FIRST_TRACK_NUM;

  /* This is pretty much copied from the "badly broken" cdrom_count_tracks
     in linux/cdrom.c.
   */
  if (p_env->tocent[i_track].entry.control & CDIO_CDROM_DATA_TRACK) {
    if (p_env->tocent[i_track].address_format == CDIO_CDROM_CDI_TRACK)
      return TRACK_FORMAT_CDI;
    else if (p_env->tocent[i_track].address_format == CDIO_CDROM_XA_TRACK)
      return TRACK_FORMAT_XA;
    else
      return TRACK_FORMAT_DATA;
  } else
    return TRACK_FORMAT_AUDIO;

}

/*!
  Return true if we have XA data (green, mode2 form1) or
  XA data (green, mode2 form2). That is track begins:
  sync - header - subheader
  12     4      -  8

  FIXME: there's gotta be a better design for this and get_track_format?
*/
static bool
get_track_green_freebsd(void *user_data, track_t i_track)
{
  _img_private_t *p_env = user_data;

  if (i_track == CDIO_CDROM_LEADOUT_TRACK) i_track = TOTAL_TRACKS+1;

  if (i_track > TOTAL_TRACKS+1 || i_track == 0)
    return false;

  /* FIXME: Dunno if this is the right way, but it's what
     I was using in cdinfo for a while.
   */
  return ((p_env->tocent[i_track-FIRST_TRACK_NUM].entry.control & 2) != 0);
}

/*!
  Return the starting LSN track number
  i_track in obj.  Track numbers start at 1.
  The "leadout" track is specified either by
  using i_track LEADOUT_TRACK or the total tracks+1.
  CDIO_INVALID_LBA is returned if there is no track entry.
*/
static lba_t
get_track_lba_freebsd(void *user_data, track_t i_track)
{
  _img_private_t *p_env = user_data;

  if (!p_env->gen.toc_init) read_toc_freebsd (p_env) ;

  if (i_track == CDIO_CDROM_LEADOUT_TRACK) i_track = TOTAL_TRACKS+1;

  if (i_track > TOTAL_TRACKS+1 || i_track == 0 || !p_env->gen.toc_init) {
    return CDIO_INVALID_LBA;
  } else {
    return cdio_lsn_to_lba(ntohl(p_env->tocent[i_track-FIRST_TRACK_NUM].entry.addr.lba));
  }
}

#endif /* HAVE_FREEBSD_CDROM */

/*!
  Return an array of strings giving possible CD devices.
 */
char **
cdio_get_devices_freebsd (void)
{
#ifndef HAVE_FREEBSD_CDROM
  return NULL;
#else
  char drive[40];
  char **drives = NULL;
  unsigned int num_drives=0;
  bool exists = true, have_cam_drive = false;
  char c;

  /* Scan the system for CD-ROM drives.
  */

#ifdef USE_ETC_FSTAB

  struct fstab *fs;
  setfsent();

  /* Check what's in /etc/fstab... */
  while ( (fs = getfsent()) )
    {
      if (strncmp(fs->fs_spec, "/dev/sr", 7))
	cdio_add_device_list(&drives, fs->fs_spec, &num_drives);
    }

#endif

  /* Scan the system for CD-ROM drives.
     Not always 100% reliable, so use the USE_MNTENT code above first.
  */

  /* Scan SCSI and CAM devices */
  exists = true;
  for ( c='0'; exists && c <='9'; c++ ) {
    sprintf(drive, "/dev/cd%c%s", c, DEVICE_POSTFIX);
    exists = cdio_is_cdrom(drive, NULL);
    if ( exists ) {
      cdio_add_device_list(&drives, drive, &num_drives);
      have_cam_drive = true;
    }
  }

  /* Scan ATAPI devices */

  /* ??? ts 9 Jan 2009
     For now i assume atapicam running if a cdN device was found.
     man atapicam strongly discourages to mix usage of CAM and ATAPI device.
     So on the risk to sabotage systems without atapicam but with real old
     SCSI drives, i list no ATAPI addresses if there was a CAM/SCSI address.

  exists = !have_cam_drive;

     ts 13 Jan 2009
     Regrettably USB drives appear as SCSI drives. We rather need to determine
     whether atapicam runs, or to make pairs of cd and acd.

  */
  exists = true;

  for ( c='0'; exists && c <='9'; c++ ) {
    sprintf(drive, "/dev/acd%c%s", c, DEVICE_POSTFIX);
    exists = cdio_is_cdrom(drive, NULL);
    if ( exists ) {
      cdio_add_device_list(&drives, drive, &num_drives);
    }
  }
  cdio_add_device_list(&drives, NULL, &num_drives);
  return drives;
#endif /*HAVE_FREEBSD_CDROM*/
}

/*!
  Return a string containing the default CD device if none is specified.
 */
char *
cdio_get_default_device_freebsd()
{
#ifndef HAVE_FREEBSD_CDROM
  return NULL;
#else
  char drive[40];
  bool exists=true;
  char c;

  /* Scan the system for CD-ROM drives.
  */

#ifdef USE_ETC_FSTAB

  struct fstab *fs;
  setfsent();

  /* Check what's in /etc/fstab... */
  while ( (fs = getfsent()) )
    {
      if (strncmp(fs->fs_spec, "/dev/sr", 7))
	return strdup(fs->fs_spec);
    }

#endif

  /* Scan the system for CD-ROM drives.
     Not always 100% reliable, so use the USE_MNTENT code above first.
  */

  /* Scan SCSI and CAM devices */
  for ( c='0'; exists && c <='9'; c++ ) {
    sprintf(drive, "/dev/cd%c%s", c, DEVICE_POSTFIX);
    exists = cdio_is_cdrom(drive, NULL);
    if ( exists ) {
      return strdup(drive);
    }
  }

  /* Scan are ATAPI devices */
  exists = true;

  for ( c='0'; exists && c <='9'; c++ ) {
    sprintf(drive, "/dev/acd%c%s", c, DEVICE_POSTFIX);
    exists = cdio_is_cdrom(drive, NULL);
    if ( exists ) {
      return strdup(drive);
    }
  }
  return NULL;
#endif /*HAVE_FREEBSD_CDROM*/
}

/*!
  Close tray on CD-ROM.

  @param psz_device the CD-ROM drive to be closed.

*/
driver_return_code_t
close_tray_freebsd (const char *psz_device)
{
#ifdef HAVE_FREEBSD_CDROM
  int fd = open (psz_device, O_RDONLY|O_NONBLOCK, 0);
  int i_rc;

  if((i_rc = ioctl(fd, CDIOCCLOSE)) != 0) {
    cdio_warn ("ioctl CDIOCCLOSE failed: %s\n", strerror(errno));
    close(fd);
    return DRIVER_OP_ERROR;
  }
  close(fd);
  return DRIVER_OP_SUCCESS;
#else
  return DRIVER_OP_NO_DRIVER;
#endif /*HAVE_FREEBSD_CDROM*/
}

/*! Find out if media has changed since the last call.  @param
  p_user_data the environment of the CD object to be acted upon.
  @return 1 if media has changed since last call, 0 if not. Error
  return codes are the same as driver_return_code_t
   */

#ifdef HAVE_FREEBSD_CDROM
int
get_media_changed_freebsd (const void *p_user_data)
{
  const _img_private_t *p_env = p_user_data;
  int changed = 0 ;

  changed = mmc_get_media_changed( p_env->gen.cdio );

  return ((changed > 0) ? changed : 0);
}

static const char*
get_access_mode(const char *psz_source)
{
    char *psz_src;
    if (!psz_source) {
	psz_src = cdio_get_default_device_freebsd();
    } else {
	psz_src = strdup(psz_source);
    }

    if (psz_src) {
	if (!(strncmp(psz_src, "/dev/acd", 8)))
	    return "ioctl";
	else {
	    char devname[256];
	    int  bytes = readlink(psz_src, devname, 255);

	    if (bytes > 0) {
		devname[bytes]=0;
		if (!(strncmp(devname, "acd", 3)))
		    return "ioctl";
	    }
	}
    }
    return "CAM";
}


/* Lock the inode associated to dev_fd and the inode associated to devname.
   Return OS errno, number of pass device of dev_fd, locked fd to devname,
   error message.
   A return value of > 0 means success, <= 0 means failure.
*/
static int freebsd_dev_lock(int dev_fd, char *devname,
   int *os_errno, int *pass_dev_no, int *lock_fd, char msg[4096],
   int flag)
{
  int lock_denied = 0, fd_stbuf_valid, name_stbuf_valid, i, pass_l = 100;
  int max_retry = 3, tries;
  struct stat fd_stbuf, name_stbuf;
  char pass_name[16], *lock_name;

  *os_errno = 0;
  *pass_dev_no = -1;
  *lock_fd = -1;
  msg[0] = 0;

  fd_stbuf_valid = !fstat(dev_fd, &fd_stbuf);

  /* Try to find name of pass device by inode number */
  lock_name = (char *) "effective device";
  if(fd_stbuf_valid) {
    for (i = 0; i < pass_l; i++) {
      sprintf(pass_name, "/dev/pass%d", i);
      if (stat(pass_name, &name_stbuf) != -1)
        if(fd_stbuf.st_ino == name_stbuf.st_ino &&
           fd_stbuf.st_dev == name_stbuf.st_dev)
    break;
    }
    if (i < pass_l) {
      lock_name = pass_name;
      *pass_dev_no = i;
    }
  }

  name_stbuf_valid = !stat(devname, &name_stbuf);
  for (tries= 0; tries <= max_retry; tries++) {
    lock_denied = flock(dev_fd, LOCK_EX | LOCK_NB);
    *os_errno = errno;
    if (lock_denied) {
      if (errno == EAGAIN && tries < max_retry) {
        /* <<< debugging
        fprintf(stderr, "\nlibcdio_DEBUG: EAGAIN , tries= %d\n", tries);
        */
        usleep(2000000);
  continue;
      }
      sprintf(msg,
        "Device busy. flock(LOCK_EX) failed on %s of %s",
        strlen(lock_name) > 2000 || *pass_dev_no < 0 ?
               "pass device" : lock_name,
        strlen(devname) > 2000 ? "drive" : devname);
      return 0;
    }
  break;
  }

/*
  fprintf(stderr, "libburn_DEBUG: flock obtained on %s of %s\n",
                  lock_name, devname);
*/

  /* Eventually lock the official device node too */
  if (fd_stbuf_valid && name_stbuf_valid &&
    (fd_stbuf.st_ino != name_stbuf.st_ino ||
     fd_stbuf.st_dev != name_stbuf.st_dev)) {

    *lock_fd = open(devname, O_RDONLY);
    if (*lock_fd == 0) {
      close(*lock_fd);
      *lock_fd = -1;
    } if (*lock_fd > 0) {
      for (tries = 0; tries <= max_retry; tries++) {
        lock_denied = flock(*lock_fd, LOCK_EX | LOCK_NB);
        if (lock_denied) {
          if (errno == EAGAIN && tries < max_retry) {
            usleep(2000000);
      continue;
          }
          close(*lock_fd);
          *lock_fd = -1;
          sprintf(msg, "Device busy. flock(LOCK_EX) failed on %s",
                  strlen(devname) > 4000 ? "drive" : devname);
          return 0;
        }
      break;
      }
    }

/*
    fprintf(stderr, "libburn_DEBUG: flock obtained on %s\n",
        devname);
*/

  }
  return 1;
}

#endif /*HAVE_FREEBSD_CDROM*/

/*!
  Initialization routine. This is the only thing that doesn't
  get called via a function pointer. In fact *we* are the
  ones to set that up.
 */
CdIo *
cdio_open_freebsd (const char *psz_source_name)
{
  return cdio_open_am_freebsd(psz_source_name, NULL);
}


/*!
  Initialization routine. This is the only thing that doesn't
  get called via a function pointer. In fact *we* are the
  ones to set that up.
 */
CdIo *
cdio_open_am_freebsd (const char *psz_orig_source_name,
		      const char *psz_access_mode)
{

#ifdef HAVE_FREEBSD_CDROM
  CdIo *ret;
  _img_private_t *_data;
  char *psz_source_name;
  int open_access_mode;  /* Access mode passed to cdio_generic_init. */

  if (!psz_access_mode)
      psz_access_mode = get_access_mode(psz_orig_source_name);

  cdio_funcs_t _funcs = {
    .audio_get_volume       = audio_get_volume_freebsd,
    .audio_pause            = audio_pause_freebsd,
    .audio_play_msf         = audio_play_msf_freebsd,
    .audio_play_track_index = audio_play_track_index_freebsd,
    .audio_read_subchannel  = audio_read_subchannel_freebsd,
    .audio_resume           = audio_resume_freebsd,
    .audio_set_volume       = audio_set_volume_freebsd,
    .audio_stop             = audio_stop_freebsd,
    .eject_media            = eject_media_freebsd,
    .free                   = free_freebsd,
    .get_arg                = get_arg_freebsd,
    .get_blocksize          = get_blocksize_mmc,
    .get_cdtext             = get_cdtext_generic,
    .get_cdtext_raw         = read_cdtext_generic,
    .get_default_device     = cdio_get_default_device_freebsd,
    .get_devices            = cdio_get_devices_freebsd,
    .get_disc_last_lsn      = get_disc_last_lsn_freebsd,
    .get_discmode           = get_discmode_generic,
    .get_drive_cap          = get_drive_cap_freebsd,
    .get_first_track_num    = get_first_track_num_generic,
    .get_media_changed      = get_media_changed_freebsd,
    .get_mcn                = get_mcn_freebsd,
    .get_num_tracks         = get_num_tracks_generic,
    .get_track_channels     = get_track_channels_generic,
    .get_track_copy_permit  = get_track_copy_permit_generic,
    .get_track_format       = get_track_format_freebsd,
    .get_track_green        = get_track_green_freebsd,
    .get_track_lba          = get_track_lba_freebsd,
    .get_track_preemphasis  = get_track_preemphasis_generic,
    .get_track_msf          = NULL,
    .get_track_isrc         = get_track_isrc_freebsd,
    .lseek                  = cdio_generic_lseek,
    .read                   = cdio_generic_read,
    .read_audio_sectors     = read_audio_sectors_freebsd,
    .read_data_sectors      = read_data_sectors_mmc,
    .read_mode2_sector      = read_mode2_sector_freebsd,
    .read_mode2_sectors     = read_mode2_sectors_freebsd,
    .read_toc               = read_toc_freebsd,
    .run_mmc_cmd            = run_mmc_cmd_freebsd,
    .set_arg                = set_arg_freebsd,
    .set_blocksize          = set_blocksize_mmc,
    .set_speed              = set_speed_freebsd,
  };

  _data                     = calloc(1, sizeof (_img_private_t));
  _data->access_mode        = str_to_access_mode_freebsd(psz_access_mode);
  _data->gen.init           = false;
  _data->gen.fd             = -1;
  _data->gen.toc_init       = false;
  _data->gen.b_cdtext_error = false;

  if (NULL == psz_orig_source_name) {
    psz_source_name=cdio_get_default_device_freebsd();
    if (NULL == psz_source_name) return NULL;
    _data->device  = psz_source_name;
    set_arg_freebsd(_data, "source", psz_source_name);
  } else {
    if (cdio_is_device_generic(psz_orig_source_name)) {
      set_arg_freebsd(_data, "source", psz_orig_source_name);
      _data->device  = strdup(psz_orig_source_name);
    } else {
      /* The below would be okay if all device drivers worked this way. */
#if 0
      cdio_info ("source %s is a not a device", psz_orig_source_name);
#endif
      free(_data);
      return NULL;
    }
  }

  ret = cdio_new ((void *)_data, &_funcs);
  if (ret == NULL) return NULL;

  open_access_mode = 0;
  if (_AM_MMC_RDWR == _data->access_mode) {
    open_access_mode |= O_RDWR;
  } else if (_AM_MMC_RDWR_EXCL == _data->access_mode) {
    open_access_mode |= O_RDWR;
  } else {
    open_access_mode |= O_RDONLY;
  }
/*
  fprintf(stderr,
      "libcdio_DEBUG: am = %d (MMC_RDWR_EXCL = %d), open = %d (O_RDWR = %d)\n",
      _data->access_mode, _AM_MMC_RDWR_EXCL, open_access_mode, O_RDWR);
*/

  if (cdio_generic_init(_data, open_access_mode)) {
    if (_AM_MMC_RDWR_EXCL == _data->access_mode) {
      int os_errno, pass_dev_no = -1, flock_fd = -1, lock_result;
      char msg[4096];

      lock_result = freebsd_dev_lock(_data->gen.fd, _data->gen.source_name,
                                   &os_errno, &pass_dev_no, &flock_fd, msg, 0);
      if (lock_result <= 0) {
        cdio_warn ("%s", msg);
        cdio_generic_free (_data);
        return NULL;
      }
      /* One should rather keep this fd open until _data->gen.fd gets closed.
         It eventually locks a device sibling of _data->gen.source_name.
      */
      if (flock_fd > 0)
        close(flock_fd);
    }

    if ( _data->access_mode == _AM_IOCTL ) {
      return ret;
    } else {
      if (init_freebsd_cam(_data))
	return ret;
      else {
	cdio_generic_free (_data);
	return NULL;
      }
    }
  } else {
    cdio_generic_free (_data);
    return NULL;
  }

#else
  return NULL;
#endif /* HAVE_FREEBSD_CDROM */

}

bool
cdio_have_freebsd (void)
{
#ifdef HAVE_FREEBSD_CDROM
  return true;
#else
  return false;
#endif /* HAVE_FREEBSD_CDROM */
}
