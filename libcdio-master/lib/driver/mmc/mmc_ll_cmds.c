/* 
   Wrappers for specific Multimedia Command (MMC) commands e.g., READ
   DISC, START/STOP UNIT.
   
   Copyright (C) 2010-2012 Rocky Bernstein <rocky@gnu.org>

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

#ifdef HAVE_STDBOOL_H
# include <stdbool.h>
#endif 

#include <cdio/cdio.h>
#include <cdio/mmc_ll_cmds.h>
#include "cdio_private.h"
#include "mmc_cmd_helper.h"

#ifdef HAVE_STRING_H
#include <string.h>
#endif

/**
   Get drive capabilities vis SCSI-MMC GET CONFIGURATION
   @param p_cdio the CD object to be acted upon.
   
   @param p_buf pointer to location to store mode sense information
   @param i_size number of bytes allocated to p_buf
   @param i_return_type value in range 0..2 giving what kind of configuration
   to return:
   
   : 0 Full Header and Full Descriptors; 
   : 1 Feature Headers and those with their Current Bit. 
   : 2 One Feature header and zero or one Feature Descriptors.
   
   @param i_starting_feature_number feature number from which to start
   getting information.
   
   @param i_timeout_ms value in milliseconds to use on timeout. Setting
   to 0 uses the default time-out value stored in
   mmc_timeout_ms.      
   
   @return DRIVER_OP_SUCCESS (0) if we got the status.
   return codes are the same as driver_return_code_t
*/
driver_return_code_t 
mmc_get_configuration(const CdIo_t *p_cdio, void *p_buf, 
                      unsigned int i_size, 
                      unsigned int i_return_type, 
                      unsigned int i_starting_feature_number,
                      unsigned int i_timeout_ms)

{
    MMC_CMD_SETUP(CDIO_MMC_GPCMD_GET_CONFIGURATION);
    CDIO_MMC_SET_READ_LENGTH8(cdb.field, i_size);
    if (0 == i_timeout_ms) i_timeout_ms = mmc_timeout_ms;
    cdb.field[1] = i_return_type & 0x3;
    CDIO_MMC_SET_LEN16(cdb.field, 2, i_starting_feature_number);
    return MMC_RUN_CMD(SCSI_MMC_DATA_READ, i_timeout_ms);
}

/**
   Return results of media event status via SCSI-MMC GET EVENT STATUS
   
   @param p_cdio the CD object to be acted upon.
   
   @param out_buf media status code from operation
   
   @return DRIVER_OP_SUCCESS (0) if we got the status. Return codes
   are the same as driver_return_code_t
 */
driver_return_code_t 
mmc_get_event_status(const CdIo_t *p_cdio, uint8_t out_buf[2])
{
    uint8_t buf[8] = { 0, };
    void   *p_buf  = &buf; 
    const unsigned int i_size = sizeof(buf);
    driver_return_code_t i_status;
    
    MMC_CMD_SETUP_READ16(CDIO_MMC_GPCMD_GET_EVENT_STATUS);
    
    cdb.field[1] = 1;      /* We poll for info */
    cdb.field[4] = 1 << 4; /* We want Media events */
    
    i_status = MMC_RUN_CMD(SCSI_MMC_DATA_READ, mmc_timeout_ms);
    if (i_status == DRIVER_OP_SUCCESS) {
        out_buf[0] = buf[4];
        out_buf[1] = buf[5];
    }
    return i_status;
}
/**
   Run a SCSI-MMC MODE SELECT (10-byte) command
   and put the results in p_buf.

   @param p_cdio the CD object to be acted upon.
   
   @param p_buf pointer to location to store mode sense information
   
   @param i_size number of bytes allocated to p_buf
   
   @param page which "page" of the mode sense command we are interested in
   
   @param i_timeout_ms value in milliseconds to use on timeout. Setting
          to 0 uses the default time-out value stored in
          mmc_timeout_ms.

   @return DRIVER_OP_SUCCESS if we ran the command ok.

 */
driver_return_code_t
mmc_mode_select_10(CdIo_t *p_cdio, /*out*/ void *p_buf, unsigned int i_size, 
                   int page, unsigned int i_timeout_ms)
{
    MMC_CMD_SETUP_READ16(CDIO_MMC_GPCMD_MODE_SELECT_10);
    if (0 == i_timeout_ms) i_timeout_ms = mmc_timeout_ms;
    cdb.field[1] = page;
    return MMC_RUN_CMD(SCSI_MMC_DATA_WRITE, i_timeout_ms);
}

/**
   Run a SCSI-MMC MODE SENSE command (10-byte version) 
   and put the results in p_buf 
   @param p_cdio the CD object to be acted upon.
   @param p_buf pointer to location to store mode sense information
   @param i_size number of bytes allocated to p_buf
   @param page which "page" of the mode sense command we are interested in
   @return DRIVER_OP_SUCCESS if we ran the command ok.
*/
driver_return_code_t
mmc_mode_sense_10(CdIo_t *p_cdio, void *p_buf, unsigned int i_size, 
                  unsigned int page)
{
    MMC_CMD_SETUP_READ16(CDIO_MMC_GPCMD_MODE_SENSE_10);
    cdb.field[2] = CDIO_MMC_ALL_PAGES & page;
    return MMC_RUN_CMD(SCSI_MMC_DATA_READ, mmc_timeout_ms);
}

/**
   Run a SCSI-MMC MODE SENSE command (6-byte version) 
   and put the results in p_buf 
   @param p_cdio the CD object to be acted upon.
   @param p_buf pointer to location to store mode sense information
   @param i_size number of bytes allocated to p_buf
   @param page which "page" of the mode sense command we are interested in
   @return DRIVER_OP_SUCCESS if we ran the command ok.
*/
driver_return_code_t
mmc_mode_sense_6(CdIo_t *p_cdio, void *p_buf, unsigned int i_size, int page)
{
    MMC_CMD_SETUP(CDIO_MMC_GPCMD_MODE_SENSE_6);
    cdb.field[4] = i_size;
    
    cdb.field[2] = CDIO_MMC_ALL_PAGES & page;

    return MMC_RUN_CMD(SCSI_MMC_DATA_READ, mmc_timeout_ms);
}

/**
   Request preventing/allowing medium removal on a drive via 
   SCSI-MMC PREVENT/ALLOW MEDIUM REMOVAL.
   
   @param p_cdio the CD object to be acted upon.
   @param b_prevent true of drive locked and false if unlocked
   @param b_persisent make b_prevent state persistent
   
   @return DRIVER_OP_SUCCESS (0) if we got the status.
   return codes are the same as driver_return_code_t
*/
driver_return_code_t 
mmc_prevent_allow_medium_removal(const CdIo_t *p_cdio, 
                                 bool b_persistent, bool b_prevent,
                                 unsigned int i_timeout_ms)
{
    uint8_t buf[8] = { 0, };
    void   *p_buf  = &buf; 
    const unsigned int i_size = 0;
    
    MMC_CMD_SETUP(CDIO_MMC_GPCMD_PREVENT_ALLOW_MEDIUM_REMOVAL);
    if (0 == i_timeout_ms) i_timeout_ms = mmc_timeout_ms;
    if (b_prevent)         cdb.field[4] |= 1;
    if (b_persistent)      cdb.field[4] |= 2;
    
    return MMC_RUN_CMD(SCSI_MMC_DATA_WRITE, i_timeout_ms);
}

/* Maximum blocks to retrieve. Would be nice to customize this based on
   drive capabilities.
*/
#define MAX_CD_READ_BLOCKS 16

/**
   Issue a MMC READ_CD command.
   
   @param p_cdio  object to read from 
   
   @param p_buf1   Place to store data. The caller should ensure that 
   p_buf1 can hold at least i_blocksize * i_blocks  bytes.
   
   @param i_lsn   sector to read 
   
   @param expected_sector_type restricts reading to a specific CD
   sector type.  Only 3 bits with values 1-5 are used:
   0 all sector types
   1 CD-DA sectors only 
   2 Mode 1 sectors only
   3 Mode 2 formless sectors only. Note in contrast to all other
   values an MMC CD-ROM is not required to support this mode.
   4 Mode 2 Form 1 sectors only
   5 Mode 2 Form 2 sectors only
   
   @param b_digital_audio_play Control error concealment when the
   data being read is CD-DA.  If the data being read is not CD-DA,
   this parameter is ignored.  If the data being read is CD-DA and
   DAP is false zero, then the user data returned should not be
   modified by flaw obscuring mechanisms such as audio data mute and
   interpolate.  If the data being read is CD-DA and DAP is true,
   then the user data returned should be modified by flaw obscuring
   mechanisms such as audio data mute and interpolate.  
   
   b_sync_header return the sync header (which will probably have
   the same value as CDIO_SECTOR_SYNC_HEADER of size
   CDIO_CD_SYNC_SIZE).
   
   @param header_codes Header Codes refer to the sector header and
   the sub-header that is present in mode 2 formed sectors: 
   
   0 No header information is returned.  
   1 The 4-byte sector header of data sectors is be returned, 
   2 The 8-byte sector sub-header of mode 2 formed sectors is
   returned.  
   3 Both sector header and sub-header (12 bytes) is returned.  
   The Header preceeds the rest of the bytes (e.g. user-data bytes) 
   that might get returned.
   
   @param b_user_data  Return user data if true. 
   
   For CD-DA, the User Data is CDIO_CD_FRAMESIZE_RAW bytes.
   
   For Mode 1, The User Data is ISO_BLOCKSIZE bytes beginning at
   offset CDIO_CD_HEADER_SIZE+CDIO_CD_SUBHEADER_SIZE.
   
   For Mode 2 formless, The User Data is M2RAW_SECTOR_SIZE bytes
   beginning at offset CDIO_CD_HEADER_SIZE+CDIO_CD_SUBHEADER_SIZE.
   
   For data Mode 2, form 1, User Data is ISO_BLOCKSIZE bytes beginning at
   offset CDIO_CD_XA_SYNC_HEADER.
   
   For data Mode 2, form 2, User Data is 2 324 bytes beginning at
   offset CDIO_CD_XA_SYNC_HEADER.
   
   @param b_sync 
   
   @param b_edc_ecc true if we return EDC/ECC error detection/correction bits.
   
   The presence and size of EDC redundancy or ECC parity is defined
   according to sector type: 
   
   CD-DA sectors have neither EDC redundancy nor ECC parity.  
   
   Data Mode 1 sectors have 288 bytes of EDC redundancy, Pad, and
   ECC parity beginning at offset 2064.
   
   Data Mode 2 formless sectors have neither EDC redundancy nor ECC
   parity
   
   Data Mode 2 form 1 sectors have 280 bytes of EDC redundancy and
   ECC parity beginning at offset 2072
   
   Data Mode 2 form 2 sectors optionally have 4 bytes of EDC
   redundancy beginning at offset 2348.
   
   @param c2_error_information If true associate a bit with each
   sector for C2 error The resulting bit field is ordered exactly as
   the main channel bytes.  Each 8-bit boundary defines a byte of
   flag bits.
   
   @param subchannel_selection subchannel-selection bits
   
   0  No Sub-channel data shall be returned.  (0 bytes)
   1  RAW P-W Sub-channel data shall be returned.  (96 byte)
   2  Formatted Q sub-channel data shall be transferred (16 bytes)
   3  Reserved     
   4  Corrected and de-interleaved R-W sub-channel (96 bytes)
   5-7  Reserved
   
   @param i_blocksize size of the a block expected to be returned
   
   @param i_blocks number of blocks expected to be returned.

   @return DRIVER_OP_SUCCESS if we ran the command ok.
*/
driver_return_code_t
mmc_read_cd(const CdIo_t *p_cdio, void *p_buf1, lsn_t i_lsn, 
            int read_sector_type, bool b_digital_audio_play,
            bool b_sync, uint8_t header_codes, bool b_user_data, 
            bool b_edc_ecc, uint8_t c2_error_information, 
            uint8_t subchannel_selection, uint16_t i_blocksize, 
            uint32_t i_blocks)
{
    void *p_buf = p_buf1;
    uint8_t cdb9 = 0;
    const unsigned int i_timeout = mmc_timeout_ms * (MAX_CD_READ_BLOCKS/2);

    MMC_CMD_SETUP(CDIO_MMC_GPCMD_READ_CD);

    /* Catch what may be a common bug. */
    if (NULL == p_buf) return DRIVER_OP_BAD_POINTER;

    CDIO_MMC_SET_READ_TYPE(cdb.field, read_sector_type);
    if (b_digital_audio_play) cdb.field[1] |= 0x2;
    
    if (b_sync)      cdb9 |= 128;
    if (b_user_data) cdb9 |=  16;
    if (b_edc_ecc)   cdb9 |=   8;
    cdb9 |= (header_codes & 3)         << 5;
    cdb9 |= (c2_error_information & 3) << 1;
    cdb.field[9]  = cdb9;
    cdb.field[10] = (subchannel_selection & 7);
    
  {
      unsigned int j = 0;
      int i_status = DRIVER_OP_SUCCESS;
      
      while (i_blocks > 0) {
          const unsigned i_blocks2 = (i_blocks > MAX_CD_READ_BLOCKS) 
              ? MAX_CD_READ_BLOCKS : i_blocks;
          
          const unsigned int i_size = i_blocksize * i_blocks2;
          
          p_buf = ((char *)p_buf1 ) + (j * i_blocksize);
          CDIO_MMC_SET_READ_LBA     (cdb.field, (i_lsn+j));
          CDIO_MMC_SET_READ_LENGTH24(cdb.field, i_blocks2);
          
          i_status = MMC_RUN_CMD(SCSI_MMC_DATA_READ, i_timeout);
          
          if (i_status) return i_status;
          
          i_blocks -= i_blocks2;
          j += i_blocks2;
      }
      return i_status;
  }
}

/**
  Request information about et drive capabilities vis SCSI-MMC READ
  DISC INFORMATION
  
  @param p_cdio the CD object to be acted upon.

  @param p_buf pointer to location to store mode sense information
  
  @param i_size number of bytes allocated to p_buf
   
  @param data_type kind of information to retrieve.

  @return DRIVER_OP_SUCCESS (0) if we got the status.
 */
driver_return_code_t 
mmc_read_disc_information(const CdIo_t *p_cdio, /*out*/ void *p_buf,
                          unsigned int i_size, 
                          cdio_mmc_read_disc_info_datatype_t data_type,
                          unsigned int i_timeout_ms)
{
    MMC_CMD_SETUP(CDIO_MMC_GPCMD_READ_DISC_INFO);
    CDIO_MMC_SET_READ_LENGTH8(cdb.field, i_size);
    if (0 == i_timeout_ms) i_timeout_ms = mmc_timeout_ms;
    cdb.field[1] = data_type & 0x7;
    return MMC_RUN_CMD(SCSI_MMC_DATA_READ, i_timeout_ms);
}

/**
   Set the drive speed in K bytes per second using SCSI-MMC SET SPEED.
   .
   
   @param p_cdio        CD structure set by cdio_open().
   @param i_Kbs_speed   speed in K bytes per second. Note this is 
                        not in standard CD-ROM speed units, e.g.
                        1x, 4x, 16x as it is in cdio_set_speed.
                        To convert CD-ROM speed units to Kbs,
                        multiply the number by 176 (for raw data)
                        and by 150 (for filesystem data). 
                        Also note that ATAPI specs say that a value
                        less than 176 will result in an error.
                        On many CD-ROM drives,
                        specifying a value too large will result in using
                        the fastest speed.

    @return the drive speed if greater than 0. -1 if we had an
    error. is -2 returned if this is not implemented for the current
    driver.

    @see cdio_set_speed and mmc_set_drive_speed

   @return DRIVER_OP_SUCCESS if we ran the command ok.
  */
int
mmc_set_speed(const CdIo_t *p_cdio, int i_Kbs_speed, unsigned int i_timeout_ms)

{
    uint8_t buf[14] = { 0, };
    void * p_buf = &buf;
    const unsigned int i_size = sizeof(buf);
    MMC_CMD_SETUP(CDIO_MMC_GPCMD_SET_SPEED);

    if (0 == i_timeout_ms) i_timeout_ms = mmc_timeout_ms;

    /* If the requested speed is less than 1x 176 kb/s this command
       will return an error - it's part of the ATAPI specs. Therefore, 
       test and stop early. */
    
    if ( i_Kbs_speed < 176 ) return -1;
    
    CDIO_MMC_SET_LEN16(cdb.field, 2, i_Kbs_speed);
    /* Some drives like the Creative 24x CDRW require one to set a
       nonzero write speed or else one gets an error back.  Some
       specifications have setting the value 0xfffff indicate setting to
       the maximum allowable speed.
    */
    CDIO_MMC_SET_LEN16(cdb.field, 4, 0xffff);
    return MMC_RUN_CMD(SCSI_MMC_DATA_WRITE, i_timeout_ms);
}

/**
   Load or Unload media using a SCSI-MMC START/STOP UNIT command. 
   
   @param p_cdio  the CD object to be acted upon.
   @param b_eject eject if true and close tray if false
   @param b_immediate wait or don't wait for operation to complete
   @param power_condition Set CD-ROM to idle/standby/sleep. If nonzero,
          eject/load is ignored, so set to 0 if you want to eject or load.
    
    @see mmc_eject_media or mmc_close_tray
*/
driver_return_code_t
mmc_start_stop_unit(const CdIo_t *p_cdio, bool b_eject, bool b_immediate,
                    uint8_t power_condition, unsigned int i_timeout_ms)
{
  uint8_t buf[1];
  void * p_buf = &buf;
  const unsigned int i_size = 0;

  MMC_CMD_SETUP_READ16(CDIO_MMC_GPCMD_START_STOP_UNIT);

  if (b_immediate) cdb.field[1] |= 1;

  if (power_condition) 
    cdb.field[4] = power_condition << 4;
  else {
    if (b_eject) 
      cdb.field[4] = 2; /* eject */
    else 
      cdb.field[4] = 3; /* close tray for tray-type */
  }

  return MMC_RUN_CMD(SCSI_MMC_DATA_WRITE, mmc_timeout_ms);
}

/**
   Check if drive is ready using SCSI-MMC TEST UNIT READY command. 
   
   @param p_cdio  the CD object to be acted upon.
    
   @return DRIVER_OP_SUCCESS if we ran the command ok.
 */
driver_return_code_t
mmc_test_unit_ready(const CdIo_t *p_cdio, unsigned int i_timeout_ms)
{
    const unsigned int i_size = 0;
    void  * p_buf = NULL;
    MMC_CMD_SETUP_READ16(CDIO_MMC_GPCMD_TEST_UNIT_READY);

    if (0 == i_timeout_ms) i_timeout_ms = mmc_timeout_ms;
    return MMC_RUN_CMD(SCSI_MMC_DATA_NONE, i_timeout_ms);
}
