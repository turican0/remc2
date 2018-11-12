/*
   "Higher-level" Multimedia Command (MMC) commands which build on
   the "lower-level" commands.

   Copyright (C) 2010-2011, 2014 Rocky Bernstein <rocky@gnu.org>

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
# include "config.h"
# define __CDIO_CONFIG_H__ 1
#endif

#include <cdio/cdio.h>
#include <cdio/mmc_cmds.h>

/**
   Close tray using a MMC START STOP UNIT command.
   @param p_cdio the CD object to be acted upon.
   @return DRIVER_OP_SUCCESS (0) if we got the status.
   return codes are the same as driver_return_code_t
 */
driver_return_code_t
mmc_close_tray(CdIo_t *p_cdio)
{
    return mmc_start_stop_unit(p_cdio, false, false, 0, 0);
}

/**
   Eject using MMC commands. If CD-ROM is "locked" we'll unlock it.
   Command is not "immediate" -- we'll wait for the command to complete.
   For a more general (and lower-level) routine, @see mmc_start_stop_media.

   @param p_cdio the CD object to be acted upon.
   @return DRIVER_OP_SUCCESS (0) if we got the status.
   return codes are the same as driver_return_code_t
*/
driver_return_code_t
mmc_eject_media( const CdIo_t *p_cdio )
{
  int i_status = 0;
  i_status = mmc_prevent_allow_medium_removal(p_cdio, false, false, 0);
  if (0 != i_status) return i_status;

  return mmc_start_stop_unit(p_cdio, true, false, 0, 0);

}

/**
  Detects if a disc (CD or DVD) is erasable or not.

  @param p_user_data the CD object to be acted upon.

  @param b_erasable, if not NULL, on return will be set indicate whether
  the operation was a success (DRIVER_OP_SUCCESS) or if not to some
  other value.

  @return true if the disc is detected as erasable (rewritable), false
otherwise.
 */
/* From Frank Endres: */
driver_return_code_t
mmc_get_disc_erasable(const CdIo_t *p_cdio, bool *b_erasable) {
    uint8_t buf[42] = { 0, };
    driver_return_code_t i_status;

    i_status = mmc_read_disc_information(p_cdio, buf, sizeof(buf),
					 CDIO_MMC_READ_DISC_INFO_STANDARD, 0);

    if (DRIVER_OP_SUCCESS == i_status)
	*b_erasable = ((buf[2] & 0x10) ? true : false);
    else
	*b_erasable = false;
    return i_status;
}

/* From Frank Endres: */
/**
   Detects the disc type using the SCSI-MMC GET CONFIGURATION command.

   @param p_cdio the CD object to be acted upon.

   @param i_status, if not NULL, on return will be set indicate whether
   the operation was a success (DRIVER_OP_SUCCESS) or if not to some
   other value.

   @param p_disctype the disc type set on success.
   @return DRIVER_OP_SUCCESS (0) if we got the status.
   return codes are the same as driver_return_code_t
 */
driver_return_code_t
mmc_get_disctype( const CdIo_t *p_cdio, unsigned int i_timeout_ms,
                  cdio_mmc_feature_profile_t *p_disctype)
{
    uint8_t buf[500] = { 0, };
    driver_return_code_t i_status;

    if (0 == i_timeout_ms) i_timeout_ms = mmc_timeout_ms;
    i_status = mmc_get_configuration(p_cdio, &buf, sizeof(buf),
				     CDIO_MMC_GET_CONF_ALL_FEATURES,
				     0, i_timeout_ms);

    if (DRIVER_OP_SUCCESS == i_status) {
	uint8_t *p, *q;
	uint8_t profiles_list_length;
	uint16_t profile_number;
	bool profile_active;

	/* there is always a profile list feature listed at the
	   first place of the features list */
	p = buf + 8;
	profiles_list_length = p[3];
	q = p+4;
	*p_disctype = CDIO_MMC_FEATURE_PROF_NON_CONFORM;

	while ((CDIO_MMC_FEATURE_PROF_NON_CONFORM == *p_disctype) &&
	       (q < p + profiles_list_length)) {
	    profile_number = CDIO_MMC_GET_LEN16(q);
	    profile_active = q[2] & 0x01;

	    if (profile_active)
		switch (profile_number) {
		  case CDIO_MMC_FEATURE_PROF_CD_ROM:
		  case CDIO_MMC_FEATURE_PROF_CD_R:
		  case CDIO_MMC_FEATURE_PROF_CD_RW:
		  case CDIO_MMC_FEATURE_PROF_DVD_ROM:
		  case CDIO_MMC_FEATURE_PROF_DVD_R_SEQ:
		  case CDIO_MMC_FEATURE_PROF_DVD_RAM:
		  case CDIO_MMC_FEATURE_PROF_DVD_RW_RO:
		  case CDIO_MMC_FEATURE_PROF_DVD_RW_SEQ:
		  case CDIO_MMC_FEATURE_PROF_DVD_R_DL_SEQ:
		  case CDIO_MMC_FEATURE_PROF_DVD_R_DL_JR:
		  case CDIO_MMC_FEATURE_PROF_DVD_PRW:
		  case CDIO_MMC_FEATURE_PROF_DVD_PR:
		  case CDIO_MMC_FEATURE_PROF_DVD_PRW_DL:
		  case CDIO_MMC_FEATURE_PROF_DVD_PR_DL:
		  case CDIO_MMC_FEATURE_PROF_BD_ROM:
		  case CDIO_MMC_FEATURE_PROF_BD_SEQ:
		  case CDIO_MMC_FEATURE_PROF_BD_R_RANDOM:
		  case CDIO_MMC_FEATURE_PROF_BD_RE:
		  case CDIO_MMC_FEATURE_PROF_HD_DVD_ROM:
		  case CDIO_MMC_FEATURE_PROF_HD_DVD_R:
		  case CDIO_MMC_FEATURE_PROF_HD_DVD_RAM:
		    *p_disctype = (cdio_mmc_feature_profile_t) profile_number;
		    break;
		}
	    q += 4;
	}
    }
    return i_status;
}

/**
   Run a SCSI-MMC MMC MODE SENSE command (6- or 10-byte version)
   and put the results in p_buf
   @param p_cdio the CD object to be acted upon.
   @param p_buf pointer to location to store mode sense information
   @param i_size number of bytes allocated to p_buf
   @param page which "page" of the mode sense command we are interested in
   @return DRIVER_OP_SUCCESS if we ran the command ok.
*/
driver_return_code_t
mmc_mode_sense(CdIo_t *p_cdio, /*out*/ void *p_buf, unsigned int i_size,
	       int page)
{
  /* We used to make a choice as to which routine we'd use based
     cdio_have_atapi(). But since that calls this in its determination,
     we had an infinite recursion. So we can't use cdio_have_atapi()
     (until we put in better capability checks.)
   */
    if ( DRIVER_OP_SUCCESS == mmc_mode_sense_6(p_cdio, p_buf, i_size, page) )
	return DRIVER_OP_SUCCESS;
    return mmc_mode_sense_10(p_cdio, p_buf, i_size, page);
}

/**
  Set the drive speed in CD-ROM speed units.

  @param p_cdio	   CD structure set by cdio_open().
  @param i_drive_speed   speed in CD-ROM speed units. Note this
                         not Kbytes/sec as would be used in the MMC spec or
	                 in mmc_set_speed(). To convert CD-ROM speed units
		         to Kbs, multiply the number by 176 (for raw data)
		         and by 150 (for filesystem data). On many CD-ROM
		         drives, specifying a value too large will result
		         in using the fastest speed. GNU/Linux ioctl
			 treats <= 0 as max speed, so we'll do that here
			 as well.

  @return the drive speed if greater than 0. -1 if we had an error. is -2
  returned if this is not implemented for the current driver.

   @see cdio_set_speed and mmc_set_speed
*/
driver_return_code_t
mmc_set_drive_speed( const CdIo_t *p_cdio, int i_drive_speed )
{
	if (i_drive_speed <= 0)
		i_drive_speed = 0xffff;
	else
		i_drive_speed *= 176;

    return mmc_set_speed(p_cdio, i_drive_speed, 0);
}
