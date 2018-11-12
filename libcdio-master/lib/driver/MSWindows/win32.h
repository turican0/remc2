/*
  Copyright (C) 2004, 2005, 2008, 2014 Rocky Bernstein <rocky@gnu.org>

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

#include <driver/generic.h>

#pragma pack()

typedef struct {
  lsn_t          start_lsn;
  UCHAR          Control : 4;
  UCHAR          Format;
} track_info_t;

typedef enum {
  _AM_NONE,
  _AM_IOCTL,
  _AM_ASPI,
  _AM_MMC_RDWR,
  _AM_MMC_RDWR_EXCL,
} access_mode_t;

typedef struct {
  /* Things common to all drivers like this.
     This must be first. */
  generic_img_private_t gen;

  access_mode_t access_mode;

  /* Some of the more OS specific things. */
    /* Entry info for each track, add 1 for leadout. */
  track_info_t  tocent[CDIO_CD_MAX_TRACKS+1];

  HANDLE h_device_handle; /* device descriptor */
  HMODULE hASPI;
  short i_sid;
  short i_lun;
  long  (*lpSendCommand)( void* );

  bool b_ioctl_init;
  bool b_aspi_init;

} _img_private_t;

/*!
  Pause playing CD through analog output

  @param p_cdio the CD object to be acted upon.
*/
CDIO_EXTERN driver_return_code_t audio_pause_win32ioctl (void *p_user_data);

/*!
  Playing starting at given MSF through analog output

  @param p_cdio the CD object to be acted upon.
*/
CDIO_EXTERN driver_return_code_t audio_play_msf_win32ioctl (void *p_user_data,
						msf_t *p_start_msf,
						msf_t *p_end_msf);
/*!
  Resume playing an audio CD.

  @param p_cdio the CD object to be acted upon.

*/
CDIO_EXTERN driver_return_code_t audio_resume_win32ioctl (void *p_user_data);

/*!
  Get disc type associated with cd object.
*/
CDIO_EXTERN discmode_t get_discmode_win32ioctl (_img_private_t *p_env);

/*!
  Get the volume settings of an audio CD.

  @param p_cdio the CD object to be acted upon.

*/
CDIO_EXTERN driver_return_code_t
audio_get_volume_win32ioctl ( void *p_user_data,
			      /*out*/ cdio_audio_volume_t *p_volume);

/*!
  Read Audio Subchannel information

  @param p_cdio the CD object to be acted upon.

*/
CDIO_EXTERN driver_return_code_t
audio_read_subchannel_win32ioctl (void *p_user_data,
				  cdio_subchannel_t *p_subchannel);

/*!
  Set the volume of an audio CD.

  @param p_cdio the CD object to be acted upon.

*/
CDIO_EXTERN driver_return_code_t
audio_stop_win32ioctl ( void *p_user_data );

/*!
  Set the volume of an audio CD.

  @param p_cdio the CD object to be acted upon.

*/
CDIO_EXTERN driver_return_code_t
audio_set_volume_win32ioctl ( void *p_user_data,
			      cdio_audio_volume_t *p_volume);

/*!
  Close the tray of a CD-ROM

  @param p_user_data the CD object to be acted upon.

*/
CDIO_EXTERN driver_return_code_t close_tray_win32ioctl (const char *psz_win32_drive);

/*!
   Reads an audio device using the DeviceIoControl method into data
   starting from lsn.  Returns 0 if no error.
*/
CDIO_EXTERN int read_audio_sectors_win32ioctl (_img_private_t *p_obj, void *p_data, lsn_t lsn,
				   unsigned int nblocks);
/*!
   Reads a single mode2 sector using the DeviceIoControl method into
   data starting from lsn. Returns 0 if no error.
 */
CDIO_EXTERN int read_mode2_sector_win32ioctl (_img_private_t *p_env, void *p_data,
				  lsn_t lsn, bool b_form2);

/*!
   Reads a single mode1 sector using the DeviceIoControl method into
   data starting from lsn. Returns 0 if no error.
 */
CDIO_EXTERN int read_mode1_sector_win32ioctl (_img_private_t *p_env, void *p_data,
				  lsn_t lsn, bool b_form2);

CDIO_EXTERN const char *is_cdrom_win32ioctl (const char drive_letter);

/*!
  Run a SCSI MMC command.

  env	        private CD structure
  i_timeout_ms  time in milliseconds we will wait for the command
                to complete. If this value is -1, use the default
		time-out value.
  p_buf	        Buffer for data, both sending and receiving
  i_buf	        Size of buffer
  e_direction	direction the transfer is to go.
  cdb	        CDB bytes. All values that are needed should be set on
                input. We'll figure out what the right CDB length should be.

  Return 0 if command completed successfully.
 */
CDIO_EXTERN int run_mmc_cmd_win32ioctl( void *p_user_data,
			    unsigned int i_timeout,
			    unsigned int i_cdb,
			    const mmc_cdb_t * p_cdb,
			    cdio_mmc_direction_t e_direction,
			    unsigned int i_buf, /*in/out*/ void *p_buf );

/*!
  Initialize internal structures for CD device.
 */
CDIO_EXTERN bool init_win32ioctl (_img_private_t *p_env);

/*!
  Read and cache the CD's Track Table of Contents and track info.
  Return true if successful or false if an error.
*/
CDIO_EXTERN bool read_toc_win32ioctl (_img_private_t *p_env);

/*!
  Get the LSN of the first track of the last session of
  on the CD.
 */
CDIO_EXTERN driver_return_code_t
get_last_session_win32ioctl (void *p_user_data,
                             /*out*/ lsn_t *i_last_session);

/*!
  Return the media catalog number MCN.

  Note: string is malloc'd so caller should free() then returned
  string when done with it.

 */
CDIO_EXTERN char *get_mcn_win32ioctl (const _img_private_t *p_env);

/*!
  Return the international standard recording code ISRC.

  Note: string is malloc'd so caller should free() then returned
  string when done with it.

 */
CDIO_EXTERN char *get_track_isrc_win32ioctl (const _img_private_t *p_env,
				 track_t i_track);

/*!
  Return the the kind of drive capabilities of device.

  Note: string is malloc'd so caller should free() then returned
  string when done with it.

 */
CDIO_EXTERN void get_drive_cap_aspi (const _img_private_t *p_env,
			 cdio_drive_read_cap_t  *p_read_cap,
			 cdio_drive_write_cap_t *p_write_cap,
			 cdio_drive_misc_cap_t  *p_misc_cap);

/*!
  Return the the kind of drive capabilities of device.

  Note: string is malloc'd so caller should free() then returned
  string when done with it.

 */
CDIO_EXTERN void get_drive_cap_win32ioctl (const _img_private_t *p_env,
			       cdio_drive_read_cap_t  *p_read_cap,
			       cdio_drive_write_cap_t *p_write_cap,
			       cdio_drive_misc_cap_t  *p_misc_cap);

/*!
  Get the format (XA, DATA, AUDIO) of a track.
*/
CDIO_EXTERN track_format_t get_track_format_win32ioctl(const _img_private_t *p_env,
					   track_t i_track);
