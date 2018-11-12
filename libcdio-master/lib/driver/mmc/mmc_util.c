/* Multimedia Command (MMC) "helper" routines that don't depend
   on anything other than headers.

  Copyright (C) 2010, 2011 Rocky Bernstein <rocky@gnu.org>

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

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

#include <cdio/mmc.h>
#include "mmc_private.h"

/** The below variables are trickery to force enum symbol values to be
    recorded in debug symbol tables. They are used to allow one to refer
    to the enumeration value names in the typedefs above in a debugger
    and debugger expressions
*/
cdio_mmc_feature_t           debug_cdio_mmc_feature;
cdio_mmc_feature_interface_t debug_cdio_mmc_feature_interface;
cdio_mmc_feature_profile_t   debug_cdio_mmc_feature_profile;
cdio_mmc_get_conf_t          debug_cdio_mmc_get_conf;
cdio_mmc_gpcmd_t             debug_cdio_mmc_gpcmd;
cdio_mmc_read_sub_state_t    debug_cdio_mmc_read_sub_state;
cdio_mmc_read_cd_type_t      debug_cdio_mmc_read_cd_type;
cdio_mmc_readtoc_t           debug_cdio_mmc_readtoc;
cdio_mmc_mode_page_t         debug_cdio_mmc_mode_page;

/**
   Maps a mmc_sense_key_t into a string name.
 */
const char mmc_sense_key2str[16][40] = {
    "No Sense",              /**< 0 No specific Sense Key info reported */
    "Recovered Error",       /**< 1 Completed ok with recovery */
    "Not Ready",             /**< 2 */
    "Medium Error",          /**< 3 */
    "Hardware Error",        /**< 4 */
    "Illegal Request",       /**< 5 */
    "Unit Attention",        /**< 6 */
    "Data Protect",          /**< 7 */
    "Blank Check",           /**< 8 */
    "Vendor Specific",       /**< 9 */
    "Copy aborted",          /**< A */
    "Aborted Command",       /**< B */
    "Obsolete",              /**< C */
    "Unknown - 13",          /**< D */
    "Unknown - 14",          /**< E */
    "Unknown - 15",          /**< F */
};

/**
    The maximum value in milliseconds that we will wait on an MMC
    command.  
*/
uint32_t mmc_timeout_ms = MMC_TIMEOUT_DEFAULT;

/**
    The maximum value in milliseconds that we will wait on an MMC read
  command.  
*/
uint32_t mmc_read_timeout_ms = MMC_READ_TIMEOUT_DEFAULT;

/*!
  Return a string containing the name of the audio state as returned from
  the Q_SUBCHANNEL.
 */
const char *
mmc_audio_state2str( uint8_t i_audio_state )
{
    switch(i_audio_state) {
      case CDIO_MMC_READ_SUB_ST_INVALID:
        return "invalid";
      case CDIO_MMC_READ_SUB_ST_PLAY:
        return "playing";
      case CDIO_MMC_READ_SUB_ST_PAUSED:
        return "paused";
      case CDIO_MMC_READ_SUB_ST_COMPLETED:
        return "completed";
      case CDIO_MMC_READ_SUB_ST_ERROR:
        return "error";
      case CDIO_MMC_READ_SUB_ST_NO_STATUS:
        return "no status";
      default:                     
        return "unknown";
    }
}

/**
  On input a MODE_SENSE command was issued and we have the results
  in p. We interpret this and return a bit mask set according to the 
  capabilities.
*/
void
mmc_get_drive_cap_buf(const uint8_t *p,
                      /*out*/ cdio_drive_read_cap_t  *p_read_cap,
                      /*out*/ cdio_drive_write_cap_t *p_write_cap,
                      /*out*/ cdio_drive_misc_cap_t  *p_misc_cap)
{
  /* Reader */
  if (p[2] & 0x01) *p_read_cap  |= CDIO_DRIVE_CAP_READ_CD_R;
  if (p[2] & 0x02) *p_read_cap  |= CDIO_DRIVE_CAP_READ_CD_RW;
  if (p[2] & 0x08) *p_read_cap  |= CDIO_DRIVE_CAP_READ_DVD_ROM;
  if (p[4] & 0x01) *p_read_cap  |= CDIO_DRIVE_CAP_READ_AUDIO;
  if (p[4] & 0x10) *p_read_cap  |= CDIO_DRIVE_CAP_READ_MODE2_FORM1;
  if (p[4] & 0x20) *p_read_cap  |= CDIO_DRIVE_CAP_READ_MODE2_FORM2;
  if (p[5] & 0x01) *p_read_cap  |= CDIO_DRIVE_CAP_READ_CD_DA;
  if (p[5] & 0x10) *p_read_cap  |= CDIO_DRIVE_CAP_READ_C2_ERRS;
  if (p[5] & 0x20) *p_read_cap  |= CDIO_DRIVE_CAP_READ_ISRC;
  if (p[5] & 0x40) *p_read_cap  |= CDIO_DRIVE_CAP_READ_MCN;
  
  /* Writer */
  if (p[3] & 0x01) *p_write_cap |= CDIO_DRIVE_CAP_WRITE_CD_R;
  if (p[3] & 0x02) *p_write_cap |= CDIO_DRIVE_CAP_WRITE_CD_RW;
  if (p[3] & 0x10) *p_write_cap |= CDIO_DRIVE_CAP_WRITE_DVD_R;
  if (p[3] & 0x20) *p_write_cap |= CDIO_DRIVE_CAP_WRITE_DVD_RAM;
  if (p[4] & 0x80) *p_misc_cap  |= CDIO_DRIVE_CAP_WRITE_BURN_PROOF;

  /* Misc */
  if (p[4] & 0x40) *p_misc_cap  |= CDIO_DRIVE_CAP_MISC_MULTI_SESSION;
  if (p[6] & 0x01) *p_misc_cap  |= CDIO_DRIVE_CAP_MISC_LOCK;
  if (p[6] & 0x08) *p_misc_cap  |= CDIO_DRIVE_CAP_MISC_EJECT;
  if (p[6] >> 5 != 0) 
    *p_misc_cap |= CDIO_DRIVE_CAP_MISC_CLOSE_TRAY;
}

/**
   Return a string containing the name of the given feature
*/
const char *
mmc_feature2str( int i_feature )
{
  switch(i_feature) {
  case CDIO_MMC_FEATURE_PROFILE_LIST:
    return "Profile List";
  case CDIO_MMC_FEATURE_CORE: 
    return "Core";
  case CDIO_MMC_FEATURE_MORPHING:
    return "Morphing" ;
  case CDIO_MMC_FEATURE_REMOVABLE_MEDIUM:
    return "Removable Medium";
  case CDIO_MMC_FEATURE_WRITE_PROTECT:
    return "Write Protect";
  case CDIO_MMC_FEATURE_RANDOM_READABLE:
    return "Random Readable";
  case CDIO_MMC_FEATURE_MULTI_READ:
    return "Multi-Read";
  case CDIO_MMC_FEATURE_CD_READ:
    return "CD Read";
  case CDIO_MMC_FEATURE_DVD_READ:
    return "DVD Read";
  case CDIO_MMC_FEATURE_RANDOM_WRITABLE:
    return "Random Writable";
  case CDIO_MMC_FEATURE_INCR_WRITE:
    return "Incremental Streaming Writable";
  case CDIO_MMC_FEATURE_SECTOR_ERASE:
    return "Sector Erasable";
  case CDIO_MMC_FEATURE_FORMATABLE:
    return "Formattable";
  case CDIO_MMC_FEATURE_DEFECT_MGMT:
    return "Management Ability of the Logical Unit/media system "
      "to provide an apparently defect-free space.";
  case CDIO_MMC_FEATURE_WRITE_ONCE:
    return "Write Once";
  case CDIO_MMC_FEATURE_RESTRICT_OVERW:
    return "Restricted Overwrite";
  case CDIO_MMC_FEATURE_CD_RW_CAV:
    return "CD-RW CAV Write";
  case CDIO_MMC_FEATURE_MRW:
    return "MRW";
  case CDIO_MMC_FEATURE_ENHANCED_DEFECT:
    return "Enhanced Defect Reporting";
  case CDIO_MMC_FEATURE_DVD_PRW:
    return "DVD+RW";
  case CDIO_MMC_FEATURE_DVD_PR:
    return "DVD+R";
  case CDIO_MMC_FEATURE_RIGID_RES_OVERW:
    return "Rigid Restricted Overwrite";
  case CDIO_MMC_FEATURE_CD_TAO:
    return "CD Track at Once";
  case CDIO_MMC_FEATURE_CD_SAO:
    return "CD Mastering (Session at Once)";
  case CDIO_MMC_FEATURE_DVD_R_RW_WRITE:
    return "DVD-R/RW Write";
  case CDIO_MMC_FEATURE_CD_RW_MEDIA_WRITE:
    return "CD-RW Media Write Support";
  case CDIO_MMC_FEATURE_DVD_PR_2_LAYER:
    return "DVD+R Double Layer";
  case CDIO_MMC_FEATURE_POWER_MGMT:
    return "Initiator- and Device-directed Power Management";
  case CDIO_MMC_FEATURE_CDDA_EXT_PLAY:
    return "CD Audio External Play";
  case CDIO_MMC_FEATURE_MCODE_UPGRADE:
    return "Ability for the device to accept new microcode via the interface";
  case CDIO_MMC_FEATURE_TIME_OUT:
    return "Ability to respond to all commands within a specific time";
  case CDIO_MMC_FEATURE_DVD_CSS:
    return "Ability to perform DVD CSS/CPPM authentication via RPC";
  case CDIO_MMC_FEATURE_RT_STREAMING:
    return "Ability to read and write using Initiator requested performance"
      " parameters";
  case CDIO_MMC_FEATURE_LU_SN:
    return "The Logical Unit Unique Identifier";
  default: 
    {
      static char buf[100];
      if ( 0 != (i_feature & 0xFF00) ) {
        snprintf( buf, sizeof(buf),
                 "Vendor-specific code %x", i_feature );
      } else {
        snprintf( buf, sizeof(buf),
                 "Unknown code %x", i_feature );
      }
      return buf;
    }
  }
}


/**
 Return a string containing the name of the given feature profile.
 */
const char *
mmc_feature_profile2str( int i_feature_profile )
{
  switch(i_feature_profile) {
  case CDIO_MMC_FEATURE_PROF_NON_REMOVABLE:
    return "Non-removable";
  case CDIO_MMC_FEATURE_PROF_REMOVABLE:
    return "disk Re-writable; with removable media";
  case CDIO_MMC_FEATURE_PROF_MO_ERASABLE:
    return "Erasable Magneto-Optical disk with sector erase capability";
  case CDIO_MMC_FEATURE_PROF_MO_WRITE_ONCE:
    return "Write Once Magneto-Optical write once";
  case CDIO_MMC_FEATURE_PROF_AS_MO:
    return "Advance Storage Magneto-Optical";
  case CDIO_MMC_FEATURE_PROF_CD_ROM:
    return "Read only Compact Disc capable";
  case CDIO_MMC_FEATURE_PROF_CD_R:
    return "Write once Compact Disc capable";
  case CDIO_MMC_FEATURE_PROF_CD_RW:
    return "CD-RW Re-writable Compact Disc capable";
  case CDIO_MMC_FEATURE_PROF_DVD_ROM:
    return "Read only DVD";
  case CDIO_MMC_FEATURE_PROF_DVD_R_SEQ:
    return "Re-recordable DVD using Sequential recording";
  case CDIO_MMC_FEATURE_PROF_DVD_RAM:
    return "Re-writable DVD";
  case CDIO_MMC_FEATURE_PROF_DVD_RW_RO:
    return "Re-recordable DVD using Restricted Overwrite";
  case CDIO_MMC_FEATURE_PROF_DVD_RW_SEQ:
    return "Re-recordable DVD using Sequential Recording";
  case CDIO_MMC_FEATURE_PROF_DVD_R_DL_SEQ:
    return "DVD-R - Double-Layer Sequential Recording";
  case CDIO_MMC_FEATURE_PROF_DVD_R_DL_JR:
    return "DVD-R - Double-layer Jump Recording";
  case CDIO_MMC_FEATURE_PROF_DVD_PRW:
    return "DVD+RW - DVD Rewritable";
  case CDIO_MMC_FEATURE_RIGID_RES_OVERW:
    return "Rigid Restricted Overwrite";
  case CDIO_MMC_FEATURE_PROF_DVD_PR:
    return "DVD+R - DVD Recordable";
  case CDIO_MMC_FEATURE_PROF_DDCD_ROM:
    return "Read only DDCD";
  case CDIO_MMC_FEATURE_PROF_DDCD_R:
    return "DDCD-R Write only DDCD";
  case CDIO_MMC_FEATURE_PROF_DDCD_RW:
    return "Re-Write only DDCD";
  case CDIO_MMC_FEATURE_PROF_DVD_PRW_DL:
    return "DVD+RW - Double Layer";
  case CDIO_MMC_FEATURE_PROF_DVD_PR_DL:
    return "DVD+R Double Layer - DVD Recordable Double Layer";
  case CDIO_MMC_FEATURE_PROF_BD_ROM:
    return "Blu Ray BD-ROM";
  case CDIO_MMC_FEATURE_PROF_BD_SEQ:
    return "Blu Ray BD-R sequential recording";
  case CDIO_MMC_FEATURE_PROF_BD_R_RANDOM:
    return "Blu Ray BD-R random recording";
  case CDIO_MMC_FEATURE_PROF_BD_RE:
    return "Blu Ray BD-RE";
  case CDIO_MMC_FEATURE_PROF_HD_DVD_ROM:
    return "HD-DVD-ROM";
  case CDIO_MMC_FEATURE_PROF_HD_DVD_R:
      return "HD-DVD-R";
  case CDIO_MMC_FEATURE_PROF_HD_DVD_RAM:
    return "HD-DVD-RAM";
  case CDIO_MMC_FEATURE_PROF_NON_CONFORM:
    return "The Logical Unit does not conform to any Profile";
  default: 
    {
      static char buf[100];
      snprintf(buf, sizeof(buf), "Unknown Profile %x", i_feature_profile);
      return buf;
    }
  }
}

bool
mmc_is_disctype_bd (cdio_mmc_feature_profile_t disctype) {
    switch (disctype) {
      case CDIO_MMC_FEATURE_PROF_BD_ROM:
      case CDIO_MMC_FEATURE_PROF_BD_SEQ:
      case CDIO_MMC_FEATURE_PROF_BD_R_RANDOM:
      case CDIO_MMC_FEATURE_PROF_BD_RE:
        return true;
      default:
        return false;
    }
}

bool 
mmc_is_disctype_cdrom (cdio_mmc_feature_profile_t disctype) {
    switch (disctype) {
      case CDIO_MMC_FEATURE_PROF_CD_ROM:
      case CDIO_MMC_FEATURE_PROF_CD_R:
      case CDIO_MMC_FEATURE_PROF_CD_RW:
        return true;
      default:
        return false;
    }
}


bool
mmc_is_disctype_dvd (cdio_mmc_feature_profile_t disctype) {
    switch (disctype) {
      case CDIO_MMC_FEATURE_PROF_DVD_ROM:
      case CDIO_MMC_FEATURE_PROF_DVD_RAM:
      case CDIO_MMC_FEATURE_PROF_DVD_R_SEQ:
      case CDIO_MMC_FEATURE_PROF_DVD_RW_RO:
      case CDIO_MMC_FEATURE_PROF_DVD_RW_SEQ:
      case CDIO_MMC_FEATURE_PROF_DVD_R_DL_SEQ:
      case CDIO_MMC_FEATURE_PROF_DVD_R_DL_JR:
      case CDIO_MMC_FEATURE_PROF_DVD_PRW:
      case CDIO_MMC_FEATURE_PROF_DVD_PR:
      case CDIO_MMC_FEATURE_PROF_DVD_PRW_DL:
      case CDIO_MMC_FEATURE_PROF_DVD_PR_DL:
        return true;
      default:
        return false;
    }
}

bool
mmc_is_disctype_hd_dvd (cdio_mmc_feature_profile_t disctype) {
    switch (disctype) {
      case CDIO_MMC_FEATURE_PROF_HD_DVD_ROM:
      case CDIO_MMC_FEATURE_PROF_HD_DVD_R:
      case CDIO_MMC_FEATURE_PROF_HD_DVD_RAM:
        return true;
      default:
        return false;
    }
}


bool
mmc_is_disctype_overwritable (cdio_mmc_feature_profile_t disctype) {
    switch (disctype) {
      case CDIO_MMC_FEATURE_PROF_DVD_RW_RO:
      case CDIO_MMC_FEATURE_PROF_DVD_R_DL_JR:
      case CDIO_MMC_FEATURE_PROF_DVD_PRW:
      case CDIO_MMC_FEATURE_PROF_DVD_PRW_DL:
      case CDIO_MMC_FEATURE_PROF_BD_R_RANDOM: /* pseudo-overwritable */
      case CDIO_MMC_FEATURE_PROF_BD_RE:
      case CDIO_MMC_FEATURE_PROF_HD_DVD_RAM:
        return true;
      default:
        return false;
    }
}


bool
mmc_is_disctype_rewritable (cdio_mmc_feature_profile_t disctype) { 
    /* discs that need blanking before re-use */
    if (mmc_is_disctype_overwritable (disctype))
        return true;
    
    switch (disctype) {
      case CDIO_MMC_FEATURE_PROF_CD_RW:
      case CDIO_MMC_FEATURE_PROF_DVD_RW_SEQ:
      case CDIO_MMC_FEATURE_PROF_BD_SEQ:
        return true;
      default:
        return false;
    }
}

