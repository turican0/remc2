/*
  Copyright (C) 2009 KO Myung-Hun <komh@chollian.net>
  Copyright (C) 2012 Rocky Bernstein <rocky@gnu.org>

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

/* This file contains OS/2-specific code using the DosDevIOCtl
   access method.
*/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_STDBOOL_H
# include <stdbool.h>
#endif 

#include <cdio/cdio.h>
#include <cdio/sector.h>
#include <cdio/util.h>
#include <cdio/mmc.h>
#include <cdio/logging.h>
#include "cdio_assert.h"
#include "cdio_private.h"

#include <string.h>

#ifdef HAVE_OS2_CDROM

#define INCL_DOS
#define INCL_DOSDEVIOCTL
#include <os2safe.h>
#include <os2.h>

#include <ctype.h>


typedef struct {
  lsn_t          lsn_start;
  UCHAR          uc_adr;
  UCHAR          uc_control;
} toc_t;

typedef enum {
  _AM_NONE,
  _AM_OS2,
} access_mode_t;

typedef struct {
  /* Things common to all drivers like this.
     This must be first. */
  generic_img_private_t gen;

  access_mode_t access_mode;

  /* Track information */
  toc_t  toc[CDIO_CD_MAX_TRACKS + 1];   /* 1 more for leadout */
  int    i_first_track;
  int    i_last_track;

  /* Some of the more OS specific things. */
  HFILE h_cd;
  BYTE  uc_drive;
} _img_private_t;

#pragma pack(1)

static track_format_t
get_track_format_os2(const _img_private_t *p_env, track_t i_track);

static bool read_toc_os2 (void *p_user_data);

static int
run_mmc_cmd_os2( void *p_user_data, unsigned int i_timeout_ms,
                 unsigned int i_cdb, const mmc_cdb_t *p_cdb,
                 cdio_mmc_direction_t e_direction,
                 unsigned int i_buf, /*in/out*/ void *p_buf );

/*!
  High memory safe wrapper for DosDevIOCtl()
*/
static APIRET
SafeDosDevIOCtl( HFILE hdevice, ULONG category, ULONG function,
                 PVOID pParams, ULONG cbParamLenMax, PULONG pcbParamLen,
                 PVOID pData, ULONG cbDataLenMax, PULONG pcbDataLen )
{
    PVOID  pParamsLow = NULL;
    PVOID  pDataLow = NULL;
    PULONG pcbParamLenLow = NULL;
    PULONG pcbDataLenLow = NULL;
    ULONG  cbParamLenLow;
    ULONG  cbDataLenLow;

    APIRET rc = 0;

    if( pParams )
        rc = DosAllocMem( &pParamsLow, cbParamLenMax, fALLOC );

    if( rc )
        goto exit_free;

    if( pData )
        rc = DosAllocMem( &pDataLow, cbDataLenMax, fALLOC );

    if( rc )
        goto exit_free;

    if( pParams )
        memcpy( pParamsLow, pParams, cbParamLenMax );

    if( pData )
        memcpy( pDataLow, pData, cbDataLenMax );

    if( pcbParamLen )
    {
        pcbParamLenLow = &cbParamLenLow;
        *pcbParamLenLow = *pcbParamLen;
    }

    if( pcbDataLen )
    {
        pcbDataLenLow = &cbDataLenLow;
        *pcbDataLenLow = *pcbDataLen;
    }

    rc = DosDevIOCtl( hdevice, category, function,
                      pParamsLow, cbParamLenMax, pcbParamLenLow,
                      pDataLow, cbDataLenMax, pcbDataLenLow );

    if( !rc )
    {
        if( pParams )
            memcpy( pParams, pParamsLow, cbParamLenMax );

        if( pData )
            memcpy( pData, pDataLow, cbDataLenMax );

        if( pcbParamLen )
            *pcbParamLen = *pcbParamLenLow;

        if( pcbDataLen )
            *pcbDataLen  = *pcbDataLenLow;
    }

exit_free:
    DosFreeMem( pParamsLow);
    DosFreeMem( pDataLow);

    return rc;
}

#define DosDevIOCtl SafeDosDevIOCtl

/*!
  Set the volume of an audio CD.

  @param p_cdio the CD object to be acted upon.

*/
static driver_return_code_t
audio_get_volume_os2 (void *p_user_data,
                      /*out*/ cdio_audio_volume_t *p_volume)
{
  _img_private_t *p_env = p_user_data;

  struct {
    UCHAR auch_sign[4];
  } s_param = {{'C', 'D', '0', '1'}};

  struct {
    struct {
      BYTE  uc_in_ch;
      BYTE  uc_vol;
    } as_out_ch[4];
  } s_data;

  ULONG ul_param_len;
  ULONG ul_data_len;
  ULONG rc;
  int   i;

  rc = DosDevIOCtl(
          p_env->h_cd, IOCTL_CDROMAUDIO, CDROMAUDIO_GETCHANNEL,
          &s_param, sizeof( s_param ), &ul_param_len,
          &s_data, sizeof( s_data ), &ul_data_len );

  if( rc )
  {
    cdio_warn("audio_get_volume_os2 : DosDevIOCtl(GETCHANNEL) = 0x%lx\n", rc );

    return DRIVER_OP_ERROR;
  }

  for( i = 0; i < 4; i++ )
    p_volume->level[ i ] = s_data.as_out_ch[ i ].uc_vol;

  return DRIVER_OP_SUCCESS;
}

/*!
  Pause playing CD through analog output

  @param p_cdio the CD object to be acted upon.
*/
static driver_return_code_t
audio_pause_os2 (void *p_user_data)
{
  _img_private_t *p_env = p_user_data;

  struct {
    UCHAR auch_sign[4];
  } s_param = {{'C', 'D', '0', '1'}};

  ULONG ul_param_len;
  ULONG ul_data_len;
  ULONG rc;

  rc = DosDevIOCtl(
          p_env->h_cd, IOCTL_CDROMAUDIO, CDROMAUDIO_STOPAUDIO,
          &s_param, sizeof( s_param ), &ul_param_len,
          NULL, 0, &ul_data_len );

  if( rc )
  {
    cdio_warn("audio_pause_os2 : DosDevIOCtl(STOPAUDIO) = 0x%lx\n", rc );

    return DRIVER_OP_ERROR;
  }

  return DRIVER_OP_SUCCESS;
}

/*!
  Playing CD through analog output at the given MSF.

  @param p_cdio the CD object to be acted upon.
*/
static driver_return_code_t
audio_play_msf_os2 (void *p_user_data, msf_t *p_start_msf, msf_t *p_end_msf)
{
  _img_private_t *p_env = p_user_data;

  struct {
    UCHAR auch_sign[4];
    BYTE  uc_access_mode;
    BYTE  uc_start_msf_f;
    BYTE  uc_start_msf_s;
    BYTE  uc_start_msf_m;
    BYTE  uc_start_msf_reserved;
    BYTE  uc_end_msf_f;
    BYTE  uc_end_msf_s;
    BYTE  uc_end_msf_m;
    BYTE  uc_end_msf_reserved;
  } s_param = {
    .auch_sign = {'C', 'D', '0', '1'},
    .uc_access_mode = 01,                /* use MSF format */
  };

  ULONG ul_param_len;
  ULONG ul_data_len;
  ULONG rc;

  s_param.uc_start_msf_m = cdio_from_bcd8(p_start_msf->m);
  s_param.uc_start_msf_s = cdio_from_bcd8(p_start_msf->s);
  s_param.uc_start_msf_f = cdio_from_bcd8(p_start_msf->f);

  s_param.uc_end_msf_m   = cdio_from_bcd8(p_end_msf->m);
  s_param.uc_end_msf_s   = cdio_from_bcd8(p_end_msf->s);
  s_param.uc_end_msf_f   = cdio_from_bcd8(p_end_msf->f);

  rc = DosDevIOCtl(
          p_env->h_cd, IOCTL_CDROMAUDIO, CDROMAUDIO_PLAYAUDIO,
          &s_param, sizeof( s_param ), &ul_param_len,
          NULL, 0, &ul_data_len );

  if( rc )
  {
    cdio_warn("audio_play_msf_os2 : DosDevIOCtl(PLAYAUDIO) = 0x%lx\n", rc );

    return DRIVER_OP_ERROR;
  }

  return DRIVER_OP_SUCCESS;
}

/*!
  Read Audio Subchannel information

  @param p_cdio the CD object to be acted upon.

*/
static driver_return_code_t
audio_read_subchannel_os2 (void *p_user_data,
                            cdio_subchannel_t *p_subchannel)
{
  _img_private_t *p_env = p_user_data;

  struct {
    UCHAR auch_sign[4];
  } s_param = {{'C', 'D', '0', '1'}};

  struct {
    BYTE uc_control_and_adr;
    BYTE uc_track_number;                /* in BCD */
    BYTE uc_index;                       /* in BCD */
    BYTE uc_running_time_in_track_m;
    BYTE uc_running_time_in_track_s;
    BYTE uc_running_time_in_track_f;
    BYTE uc_reserved;
    BYTE uc_running_time_on_disk_m;
    BYTE uc_running_time_on_disk_s;
    BYTE uc_running_time_on_disk_f;
  } s_data_subchannel_q;

  struct {
    USHORT  us_audio_status_bits;
    ULONG   ul_start_msf;
    ULONG   ul_end_msf;
  } s_data_audio_status;

  ULONG ul_data_device_status;
  ULONG ul_param_len;
  ULONG ul_data_len;
  ULONG rc;

  rc = DosDevIOCtl(
          p_env->h_cd, IOCTL_CDROMAUDIO, CDROMAUDIO_GETSUBCHANNELQ,
          &s_param, sizeof( s_param ), &ul_param_len,
          &s_data_subchannel_q, sizeof( s_data_subchannel_q ), &ul_data_len );

  if( rc )
  {
    cdio_warn("audio_read_subchannel_os2 : DosDevIOCtl(GETSUBCHANNELQ) = 0x%lx\n", rc );

    return DRIVER_OP_ERROR;
  }

  rc = DosDevIOCtl(
          p_env->h_cd, IOCTL_CDROMAUDIO, CDROMAUDIO_GETAUDIOSTATUS,
          &s_param, sizeof( s_param ), &ul_param_len,
          &s_data_audio_status, sizeof( s_data_audio_status ), &ul_data_len );

  if( rc )
  {
    cdio_warn("audio_read_subchannel_os2 : DosDevIOCtl(GETAUDIOSTATUS) = 0x%lx\n", rc );

    return DRIVER_OP_ERROR;
  }

  rc = DosDevIOCtl(
          p_env->h_cd, IOCTL_CDROMDISK, CDROMDISK_DEVICESTATUS,
          &s_param, sizeof( s_param ), &ul_param_len,
          &ul_data_device_status, sizeof( ul_data_device_status ), &ul_data_len );

  if( rc )
  {
    cdio_warn("audio_read_subchannel_os2 : DosDevIOCtl(DEVICESTATUS) = 0x%lx\n", rc );

    return DRIVER_OP_ERROR;
  }

  p_subchannel->track = cdio_from_bcd8(s_data_subchannel_q.uc_track_number);
  p_subchannel->index = cdio_from_bcd8(s_data_subchannel_q.uc_index);

  p_subchannel->abs_addr.m = cdio_to_bcd8(s_data_subchannel_q.uc_running_time_on_disk_m);
  p_subchannel->abs_addr.s = cdio_to_bcd8(s_data_subchannel_q.uc_running_time_on_disk_s);
  p_subchannel->abs_addr.f = cdio_to_bcd8(s_data_subchannel_q.uc_running_time_on_disk_f);
  p_subchannel->rel_addr.m = cdio_to_bcd8(s_data_subchannel_q.uc_running_time_in_track_m);
  p_subchannel->rel_addr.s = cdio_to_bcd8(s_data_subchannel_q.uc_running_time_in_track_s);
  p_subchannel->rel_addr.f = cdio_to_bcd8(s_data_subchannel_q.uc_running_time_in_track_f);

  p_subchannel->address =   s_data_subchannel_q.uc_control_and_adr        & 0x0F;
  p_subchannel->control = ( s_data_subchannel_q.uc_control_and_adr >> 4 ) & 0x0F;

  if( ul_data_device_status & 0x1000 )
    p_subchannel->audio_status = CDIO_MMC_READ_SUB_ST_PLAY;
  else if( s_data_audio_status.us_audio_status_bits & 1 )
    p_subchannel->audio_status = CDIO_MMC_READ_SUB_ST_PAUSED;
  else if( s_data_audio_status.ul_start_msf == 0 &&
           s_data_audio_status.ul_end_msf == 0 )
    p_subchannel->audio_status = CDIO_MMC_READ_SUB_ST_NO_STATUS;
  else
    p_subchannel->audio_status = CDIO_MMC_READ_SUB_ST_COMPLETED;

  return DRIVER_OP_SUCCESS;
}

  /*!
    Resume playing an audio CD.

    @param p_cdio the CD object to be acted upon.

  */
static driver_return_code_t
audio_resume_os2 (void *p_user_data)
{
  _img_private_t *p_env = p_user_data;

  struct {
    UCHAR auch_sign[4];
  } s_param = {{'C', 'D', '0', '1'}};

  ULONG ul_param_len;
  ULONG ul_data_len;
  ULONG rc;

  rc = DosDevIOCtl(
          p_env->h_cd, IOCTL_CDROMAUDIO, CDROMAUDIO_RESUMEAUDIO,
          &s_param, sizeof( s_param ), &ul_param_len,
          NULL, 0, &ul_data_len );

  if( rc )
  {
    cdio_warn("audio_resume_os2 : DosDevIOCtl(RESUMEAUDIO) = 0x%lx\n", rc );

    return DRIVER_OP_ERROR;
  }

  return DRIVER_OP_SUCCESS;
}

/*!
  Set the volume of an audio CD.

  @param p_cdio the CD object to be acted upon.

*/
static driver_return_code_t
audio_set_volume_os2 ( void *p_user_data, cdio_audio_volume_t *p_volume)
{
  _img_private_t *p_env = p_user_data;

  struct {
    UCHAR auch_sign[4];
  } s_param = {{'C', 'D', '0', '1'}};

  struct {
    struct {
      BYTE  uc_in_ch;
      BYTE  uc_vol;
    } as_out_ch[4];
  } s_data;

  ULONG ul_param_len;
  ULONG ul_data_len;
  ULONG rc;
  int   i;

  /* first retrive current input ch. */
  rc = DosDevIOCtl(
          p_env->h_cd, IOCTL_CDROMAUDIO, CDROMAUDIO_GETCHANNEL,
          &s_param, sizeof( s_param ), &ul_param_len,
          &s_data, sizeof( s_data ), &ul_data_len );

  if( rc )
  {
    cdio_warn("audio_set_volume_os2 : DosDevIOCtl(GETCHANNEL) = 0x%lx\n", rc );

    return DRIVER_OP_ERROR;
  }

  for( i = 0; i < 4; i++ )
    s_data.as_out_ch[ i ].uc_vol = p_volume->level[ i ];

  /* now set volumes */
  rc = DosDevIOCtl(
          p_env->h_cd, IOCTL_CDROMAUDIO, CDROMAUDIO_SETCHANNELCTRL,
          &s_param, sizeof( s_param ), &ul_param_len,
          &s_data, sizeof( s_data ), &ul_data_len );

  if( rc )
  {
    cdio_warn("audio_set_volume_os2 : DosDevIOCtl(SETCHANNELCTRL) = 0x%lx\n", rc );

    return DRIVER_OP_ERROR;
  }

  return DRIVER_OP_SUCCESS;
}

static driver_return_code_t
audio_stop_os2 (void *p_user_data)
{
  _img_private_t *p_env = p_user_data;

  struct {
    UCHAR auch_sign[4];
  } s_param = {{'C', 'D', '0', '1'}};

  ULONG ul_param_len;
  ULONG ul_data_len;
  ULONG rc;

  rc = DosDevIOCtl(
          p_env->h_cd, IOCTL_CDROMAUDIO, CDROMAUDIO_STOPAUDIO,
          &s_param, sizeof( s_param ), &ul_param_len,
          NULL, 0, &ul_data_len );

  if( rc )
  {
    cdio_warn("audio_stop_os2 : DosDevIOCtl(STOPAUDIO) = 0x%lx\n", rc );

    return DRIVER_OP_ERROR;
  }

  return DRIVER_OP_SUCCESS;
}

/*!
  Get disc type associated with cd object.
*/
static discmode_t
dvd_discmode_os2 (_img_private_t *p_env)
{
  discmode_t discmode=CDIO_DISC_MODE_NO_INFO;
  driver_return_code_t rc;

  /* See if this is a DVD. */
  cdio_dvd_struct_t dvd;  /* DVD READ STRUCT for layer 0. */

  dvd.physical.type = CDIO_DVD_STRUCT_PHYSICAL;
  dvd.physical.layer_num = 0;

  rc = mmc_get_dvd_struct_physical_private (p_env, &run_mmc_cmd_os2,
                                            &dvd);

  if (DRIVER_OP_SUCCESS == rc) {
    switch(dvd.physical.layer[0].book_type) {
    case CDIO_DVD_BOOK_DVD_ROM:  return CDIO_DISC_MODE_DVD_ROM;
    case CDIO_DVD_BOOK_DVD_RAM:  return CDIO_DISC_MODE_DVD_RAM;
    case CDIO_DVD_BOOK_DVD_R:    return CDIO_DISC_MODE_DVD_R;
    case CDIO_DVD_BOOK_DVD_RW:   return CDIO_DISC_MODE_DVD_RW;
    case CDIO_DVD_BOOK_DVD_PR:   return CDIO_DISC_MODE_DVD_PR;
    case CDIO_DVD_BOOK_DVD_PRW:  return CDIO_DISC_MODE_DVD_PRW;
    default: return CDIO_DISC_MODE_DVD_OTHER;
    }
  }
  return discmode;
}

/*!
  Get disc type associated with the cd object.
*/
static discmode_t
get_discmode_os2(void *p_user_data)
{
  _img_private_t *p_env = p_user_data;

  track_t i_track;
  discmode_t discmode;

  if (!p_env) return CDIO_DISC_MODE_ERROR;

  discmode = dvd_discmode_os2(p_env);

  if (CDIO_DISC_MODE_NO_INFO != discmode) return discmode;

  if (!p_env->gen.toc_init) read_toc_os2 (p_env);

  if (!p_env->gen.toc_init) return CDIO_DISC_MODE_ERROR;

  for (i_track = p_env->gen.i_first_track;
       i_track < p_env->gen.i_first_track + p_env->gen.i_tracks ;
       i_track ++) {
    track_format_t track_fmt=get_track_format_os2(p_env, i_track);

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

#define CDROMDISK_EXECMD    0x7A

/* 0, if transfer data to device,  1, if transfer data from device */
#define EX_DIRECTION_IN     0x0001
/* 0, if don't check playing audio, 1, if device plays audio return error */
#define EX_PLAYING_CHK      0x0002

/*!
  Run a SCSI MMC command.

  env           private CD structure
  i_timeout_ms  time in milliseconds we will wait for the command
                to complete. If this value is -1, use the default
                time-out value.
  p_buf         Buffer for data, both sending and receiving
  i_buf         Size of buffer
  e_direction   direction the transfer is to go.
  cdb           CDB bytes. All values that are needed should be set on
                input. We'll figure out what the right CDB length should be.

  Return 0 if command completed successfully.
 */
static int
run_mmc_cmd_os2( void *p_user_data, unsigned int i_timeout_ms,
                 unsigned int i_cdb, const mmc_cdb_t *p_cdb,
                 cdio_mmc_direction_t e_direction,
                 unsigned int i_buf, /*in/out*/ void *p_buf )
{
  _img_private_t *p_env = p_user_data;

  struct {
    UCHAR        auch_sign[4];       // 'CD01'
    USHORT       us_data_length;     // length of the Data Packet
    USHORT       us_cmd_length;      // length of the Command Buffer
    USHORT       us_flags;           // flags
    BYTE         auc_cmd_buffer[16]; // Command Buffer for SCSI command
  } s_param = {
    .auch_sign = {'C', 'D', '0', '1'},
  };

  ULONG ul_param_len;
  ULONG ul_data_len;
  ULONG rc;

  s_param.us_data_length = i_buf;
  s_param.us_cmd_length = i_cdb;
  s_param.us_flags =
    ( e_direction == SCSI_MMC_DATA_READ ) ? EX_DIRECTION_IN : 0;

  memcpy( s_param.auc_cmd_buffer, p_cdb, i_cdb );

  rc = DosDevIOCtl(
          p_env->h_cd, IOCTL_CDROMDISK, CDROMDISK_EXECMD,
          &s_param, sizeof( s_param ), &ul_param_len,
          p_buf, i_buf, &ul_data_len );

  if( rc )
  {
    cdio_warn("run_mmc_cmd_os2 : DosDevIOCtl(EXECMD) = 0x%lx\n", rc );

    return DRIVER_OP_ERROR;
  }

  return DRIVER_OP_SUCCESS;
}

/*!
  Initialize CD device.
 */
static bool
init_os2 (void *p_user_data)
{
  _img_private_t *p_env = p_user_data;

  ULONG   ul_action;
  ULONG   rc;

  if (p_env->gen.init) {
    cdio_warn ("init called more than once");
    return false;
  }

  /* Initializations */
  p_env->h_cd = 0;

  rc = DosOpen((PSZ)p_env->gen.source_name, &p_env->h_cd,
               &ul_action, 0, FILE_NORMAL,
               OPEN_ACTION_OPEN_IF_EXISTS | OPEN_ACTION_FAIL_IF_NEW,
               OPEN_ACCESS_READONLY | OPEN_SHARE_DENYNONE | OPEN_FLAGS_DASD,
               NULL );
  if( rc )
  {
    cdio_warn("init_os2 : DosOpen(%s) = %ld\n", p_env->gen.source_name, rc );

    return false;
  }

  p_env->uc_drive = toupper( p_env->gen.source_name[ 0 ]) - 'A';

  p_env->gen.init           = true;
  p_env->gen.toc_init       = false;
  p_env->gen.fd             = p_env->h_cd;

  return true;
}

/*!
  Release and free resources associated with cd.
 */
static void
free_os2 (void *p_user_data)
{
  _img_private_t *p_env = p_user_data;

  if( !p_env )
    return;

  free (p_env->gen.source_name);

  if( p_env->h_cd )
    DosClose( p_env->h_cd );

  free (p_env);

}

/*!
   Reads i_blocks of audio sectors from cd device into p_data starting
   from i_lsn.
   Returns DRIVER_OP_SUCCESS if no error.
 */
static int
read_audio_sectors_os2 (void *p_user_data, void *p_buf, lsn_t i_lsn,
                        unsigned int i_blocks)
{
  _img_private_t *p_env = p_user_data;

  struct
  {
    UCHAR   auch_sign[ 4 ];
    BYTE    uc_addr_mode;
    USHORT  us_sectors;
    ULONG   ul_start_sector;
    BYTE    uc_reserved;
    BYTE    uc_interleaved_size;
  } s_param = {
    .auch_sign = {'C', 'D', '0', '1'},
    .uc_addr_mode = 0,                  /* use LBA format */
  };

  ULONG ul_param_len;
  ULONG ul_data_len;
  ULONG rc;

  s_param.us_sectors = i_blocks;
  s_param.ul_start_sector = i_lsn;

  rc = DosDevIOCtl(
          p_env->h_cd, IOCTL_CDROMDISK, CDROMDISK_READLONG,
          &s_param, sizeof( s_param ), &ul_param_len,
          p_buf, CDIO_CD_FRAMESIZE_RAW * i_blocks, &ul_data_len );

  if( rc )
  {
    cdio_warn("read_audio_sectors_os2 : DosDevIOCtl(READLONG) = 0x%lx\n", rc );

    return DRIVER_OP_ERROR;
  }

  return DRIVER_OP_SUCCESS;
}

/*!
   Reads a single raw sector using the DosDevIOCtl method into
   data starting from lsn. Returns 0 if no error.
 */
static int
read_raw_sector (_img_private_t *p_env, void *p_buf, lsn_t lsn)
{
  struct
  {
    UCHAR   auch_sign[ 4 ];
    BYTE    uc_addr_mode;
    USHORT  us_sectors;
    ULONG   ul_start_sector;
    BYTE    uc_reserved;
    BYTE    uc_interleaved_size;
  } s_param = {
    .auch_sign = {'C', 'D', '0', '1'},
    .uc_addr_mode = 0,                  /* use LBA format */
    .us_sectors = 1,
  };

  ULONG ul_param_len;
  ULONG ul_data_len;
  ULONG rc;

  s_param.ul_start_sector = lsn;

  rc = DosDevIOCtl(
          p_env->h_cd, IOCTL_CDROMDISK, CDROMDISK_READLONG,
          &s_param, sizeof( s_param ), &ul_param_len,
          p_buf, CDIO_CD_FRAMESIZE_RAW, &ul_data_len );

  if( rc )
  {
    cdio_warn("read_raw_sector : DosDevIOCtl(READLONG) = 0x%lx\n", rc );

    return DRIVER_OP_ERROR;
  }

  return DRIVER_OP_SUCCESS;
}

/*!
   Reads a single mode1 sector from cd device into data starting from
   lsn. Returns 0 if no error.
 */
static int
read_mode1_sector_os2 (void *p_user_data, void *p_buf, lsn_t lsn,
                         bool b_form2)
{
  _img_private_t *p_env = p_user_data;
  char buf[CDIO_CD_FRAMESIZE_RAW] = { 0, };
  int ret = read_raw_sector (p_env, buf, lsn);

  if ( 0 != ret) return ret;

  memcpy (p_buf,
          buf + CDIO_CD_SYNC_SIZE+CDIO_CD_HEADER_SIZE,
          b_form2 ? M2RAW_SECTOR_SIZE: CDIO_CD_FRAMESIZE);

  return 0;
}

/*!
   Reads nblocks of mode1 sectors from cd device into data starting
   from lsn.
   Returns 0 if no error.
 */
static int
read_mode1_sectors_os2 (void *p_user_data, void *p_buf, lsn_t lsn,
                        bool b_form2, unsigned int nblocks)
{
  _img_private_t *p_env = p_user_data;
  int i;
  int retval;

  for (i = 0; i < nblocks; i++) {
    if (b_form2) {
      retval = read_mode1_sector_os2 (
                 p_env, ((char *)p_buf) +
                 (M2RAW_SECTOR_SIZE * i),
                 lsn + i, true);
      if ( retval ) return retval;
    } else {
      char buf[M2RAW_SECTOR_SIZE] = { 0, };
      if ( (retval = read_mode1_sector_os2 (p_env, buf, lsn + i, false)) )
        return retval;

      memcpy (((char *)p_buf) + (CDIO_CD_FRAMESIZE * i),
              buf, CDIO_CD_FRAMESIZE);
    }
  }
  return 0;
}

/*!
   Reads a single mode2 sector from cd device into data starting
   from lsn. Returns 0 if no error.
 */
static int
read_mode2_sector_os2 (void *p_user_data, void *data, lsn_t lsn,
                       bool b_form2)
{
  _img_private_t *p_env = p_user_data;
  char buf[CDIO_CD_FRAMESIZE_RAW] = { 0, };
  int ret = read_raw_sector (p_env, buf, lsn);

  if ( 0 != ret) return ret;

  memcpy (data,
          buf + CDIO_CD_SYNC_SIZE + CDIO_CD_XA_HEADER,
          b_form2 ? M2RAW_SECTOR_SIZE: CDIO_CD_FRAMESIZE);

  return 0;
}

/*!
   Reads nblocks of mode2 sectors from cd device into data starting
   from lsn.
   Returns 0 if no error.
 */
static int
read_mode2_sectors_os2 (void *p_user_data, void *data, lsn_t lsn,
                        bool b_form2, unsigned int i_blocks)
{
  int i;
  int retval;
  unsigned int blocksize = b_form2 ? M2RAW_SECTOR_SIZE : CDIO_CD_FRAMESIZE;

  for (i = 0; i < i_blocks; i++) {
    if ( (retval = read_mode2_sector_os2 (p_user_data,
                        ((char *)data) + (blocksize * i),
                        lsn + i, b_form2)) )
      return retval;
  }
  return 0;
}

/*!
   Return the size of the CD in logical block address (LBA) units.
 */
static lsn_t
get_disc_last_lsn_os2 (void *p_user_data)
{
  _img_private_t *p_env = p_user_data;

  struct {
    UCHAR auch_sign[4];
  } s_param = {{'C', 'D', '0', '1'}};

  ULONG ul_data_volume_size;
  ULONG ul_param_len;
  ULONG ul_data_len;
  ULONG rc;

  rc = DosDevIOCtl(
          p_env->h_cd, IOCTL_CDROMDISK, CDROMDISK_GETVOLUMESIZE,
          &s_param, sizeof( s_param ), &ul_param_len,
          &ul_data_volume_size, sizeof( ul_data_volume_size ), &ul_data_len );

  if( rc )
  {
    cdio_warn("get_disc_last_lsn_os2 : DosDevIOCtl(GETVOLUMESIZE) = 0x%lx\n", rc );

    return CDIO_INVALID_LSN;
  }

  return ul_data_volume_size;
}

/*!
  Set the key "arg" to "value" in source device.
*/
static int
set_arg_os2 (void *p_user_data, const char key[], const char value[])
{
  _img_private_t *p_env = p_user_data;

  if (!strcmp (key, "source"))
  {
    if (!value)
      return DRIVER_OP_ERROR;

    free (p_env->gen.source_name);
    p_env->gen.source_name = strdup (value);
  }
  else if (!strcmp (key, "access-mode"))
  {
    if (!strcmp(value, "OS2"))
      p_env->access_mode = _AM_OS2;
    else
      cdio_warn ("unknown access type: %s. ignored.", value);
  }
  else
    return DRIVER_OP_ERROR;

  return DRIVER_OP_SUCCESS;
}

/*!
  Read and cache the CD's Track Table of Contents and track info.
  Return true if successful or false if an error.
*/
static bool
read_toc_os2 (void *p_user_data)
{
  _img_private_t *p_env = p_user_data;

  struct {
    UCHAR auch_sign[4];
  } s_param_disk = {{'C', 'D', '0', '1'}};

  struct {
    BYTE  uc_first_track;
    BYTE  uc_last_track;
    ULONG ul_lead_out_addr;         /* in MSF */
  } s_data_disk;

  struct {
    UCHAR auch_sign[4];
    BYTE  uc_track;
  } s_param_track = {
    .auch_sign = {'C', 'D', '0', '1'},
  };

  struct {
    ULONG ul_track_addr;            /* in MSF */
    BYTE  uc_control_and_adr;
  } s_data_track;

  ULONG ul_param_len;
  ULONG ul_data_len;
  ULONG rc;
  int   i_track;

  rc = DosDevIOCtl(
          p_env->h_cd, IOCTL_CDROMAUDIO, CDROMAUDIO_GETAUDIODISK,
          &s_param_disk, sizeof( s_param_disk ), &ul_param_len,
          &s_data_disk, sizeof( s_data_disk ), &ul_data_len );

  if( rc )
  {
    cdio_warn("read_toc_os2 : DosDevIOCtl(GETAUDIODISK) = 0x%lx\n", rc );

    return false;
  }

  p_env->gen.i_first_track = s_data_disk.uc_first_track;
  p_env->gen.i_tracks  = s_data_disk.uc_last_track - s_data_disk.uc_first_track + 1;

  p_env->i_first_track = s_data_disk.uc_first_track;
  p_env->i_last_track = s_data_disk.uc_last_track;

  for( i_track = p_env->i_first_track; i_track <= p_env->i_last_track; i_track++ )
  {
    s_param_track.uc_track = i_track;

    rc = DosDevIOCtl(
            p_env->h_cd, IOCTL_CDROMAUDIO, CDROMAUDIO_GETAUDIOTRACK,
            &s_param_track, sizeof( s_param_track ), &ul_param_len,
            &s_data_track, sizeof( s_data_track ), &ul_data_len );

    if( rc )
    {
      cdio_warn("read_toc_os2 : DosDevIOCtl(GETAUDIOTRACK) = 0x%lx\n", rc );

      return false;
    }

    p_env->toc[i_track].lsn_start =
      cdio_lba_to_lsn( cdio_msf3_to_lba(
        ( s_data_track.ul_track_addr >> 16 ) & 0xFF,
        ( s_data_track.ul_track_addr >>  8 ) & 0xFF,
          s_data_track.ul_track_addr         & 0xFF ));

    p_env->toc[i_track].uc_adr     =   s_data_track.uc_control_and_adr        & 0x0F;
    p_env->toc[i_track].uc_control = ( s_data_track.uc_control_and_adr >> 4 ) & 0x0F;

    p_env->gen.track_flags[i_track].preemphasis =
      p_env->toc[i_track].uc_control & 0x01
      ? CDIO_TRACK_FLAG_TRUE : CDIO_TRACK_FLAG_FALSE;

    p_env->gen.track_flags[i_track].copy_permit =
      p_env->toc[i_track].uc_control & 0x02
      ? CDIO_TRACK_FLAG_TRUE : CDIO_TRACK_FLAG_FALSE;

    p_env->gen.track_flags[i_track].channels =
      p_env->toc[i_track].uc_control & 0x08 ? 4 : 2;
  }

  /* store lead out info */
  p_env->toc[p_env->i_last_track + 1].lsn_start =
    cdio_lba_to_lsn( cdio_msf3_to_lba(
      ( s_data_disk.ul_lead_out_addr >> 16 ) & 0xFF,
      ( s_data_disk.ul_lead_out_addr >>  8 ) & 0xFF,
        s_data_disk.ul_lead_out_addr         & 0xFF ));

  p_env->gen.toc_init = true;

  return true;
}

/*!
  Eject media.
 */
static driver_return_code_t
eject_media_os2 (void *p_user_data)
{
  _img_private_t *p_env = p_user_data;

  struct {
    BYTE uc_cmd_info;
    BYTE uc_drive;
  } s_param;

  ULONG ul_param_len;
  ULONG ul_data_len;
  ULONG rc;

  s_param.uc_cmd_info = 2;
  s_param.uc_drive    = p_env->uc_drive;

  rc = DosDevIOCtl(
             ( HFILE )-1, IOCTL_DISK, DSK_UNLOCKEJECTMEDIA,
             &s_param, sizeof( s_param ), &ul_param_len,
             NULL, 0, &ul_data_len );

  if( rc )
  {
    cdio_warn("eject_media_os2 : DosDevIOCtl(UNLOCKEJECTMEDIA) = 0x%lx\n", rc );

    return DRIVER_OP_ERROR;
  }

  return DRIVER_OP_SUCCESS;
}

/*!
  Return the value associated with the key "arg".
*/
static const char *
get_arg_os2 (void *p_user_data, const char key[])
{
  _img_private_t *p_env = p_user_data;

  if (!strcmp (key, "source")) {
    return p_env->gen.source_name;
  } else if (!strcmp (key, "access-mode")) {
    switch (p_env->access_mode) {
      case _AM_OS2:
        return "OS2";
      case _AM_NONE:
        return "no access method";
    }
  }
  return NULL;
}

/*!
  Return the media catalog number MCN.

  Note: string is malloc'd so caller should free() then returned
  string when done with it.

 */
static char *
_cdio_get_mcn (const void *p_user_data) {
  const _img_private_t *p_env = p_user_data;
  return mmc_get_mcn( p_env->gen.cdio );
}

/*!
  Return the international standard recording code ISRC.

  Note: string is malloc'd so caller should free() then returned
  string when done with it.

 */
static char *
_cdio_get_track_isrc (const void *p_user_data, track_t i_track) {
  const _img_private_t *p_env = p_user_data;
  return mmc_get_track_isrc( p_env->gen.cdio, i_track );
}

/*!
  Get the format (XA, DATA, AUDIO) of a track.
*/
static track_format_t
get_track_format_os2(const _img_private_t *p_env, track_t i_track)
{
  /* This is pretty much copied from the "badly broken" cdrom_count_tracks
     in linux/cdrom.c.
  */

  if (p_env->toc[i_track].uc_control & 0x04) {
    if (p_env->toc[i_track].uc_adr == 0x10)
      return TRACK_FORMAT_CDI;
    else if (p_env->toc[i_track].uc_adr == 0x20)
      return TRACK_FORMAT_XA;
    else
      return TRACK_FORMAT_DATA;
  } else
    return TRACK_FORMAT_AUDIO;
}

/*!
  Get format of track.
*/
static track_format_t
_cdio_get_track_format(void *p_obj, track_t i_track)
{
  _img_private_t *p_env = p_obj;

  if ( !p_env )
    return TRACK_FORMAT_ERROR;

  if (!p_env->gen.toc_init)
    if (!read_toc_os2 (p_env))
      return TRACK_FORMAT_ERROR;

  if ( i_track < p_env->gen.i_first_track
       || i_track >= p_env->gen.i_tracks + p_env->gen.i_first_track )
    return TRACK_FORMAT_ERROR;

  return get_track_format_os2(p_env, i_track);
}

/*!
  Return true if we have XA data (green, mode2 form1) or
  XA data (green, mode2 form2). That is track begins:
  sync - header - subheader
  12     4      -  8

  FIXME: there's gotta be a better design for this and get_track_format?
*/
static bool
_cdio_get_track_green(void *p_obj, track_t i_track)
{
  _img_private_t *p_env = p_obj;

  switch (_cdio_get_track_format(p_env, i_track)) {
    case TRACK_FORMAT_XA:
      return true;
    case TRACK_FORMAT_ERROR:
    case TRACK_FORMAT_CDI:
    case TRACK_FORMAT_AUDIO:
      return false;
    case TRACK_FORMAT_DATA:
    default:
      break;
  }

  /* FIXME: Dunno if this is the right way, but it's what
     I was using in cd-info for a while.
   */
  return ((p_env->toc[i_track].uc_control & 2) != 0);
}

/*!
  Return the starting MSF (minutes/secs/frames) for track number
  i_tracks in obj.  Track numbers start at 1.
  The "leadout" track is specified either by
  using i_tracks LEADOUT_TRACK or the total tracks+1.
  False is returned if there is no track entry.
*/
static bool
_cdio_get_track_msf(void *p_user_data, track_t i_tracks, msf_t *p_msf)
{
  _img_private_t *p_env = p_user_data;

  if (!p_msf)
    return false;

  if (!p_env->gen.toc_init)
    if (!read_toc_os2 (p_env))
      return false;

  if (i_tracks == CDIO_CDROM_LEADOUT_TRACK)
    i_tracks = p_env->gen.i_tracks+1;

  if (i_tracks > p_env->gen.i_tracks+1 || i_tracks == 0) {
    return false;
  } else {
    cdio_lsn_to_msf(p_env->toc[i_tracks].lsn_start, p_msf);
    return true;
  }
}

#endif /* HAVE_OS2_CDROM */

/*!
  Return an array of strings giving possible CD devices.
 */
char **
cdio_get_devices_os2 (void)
{
#ifndef HAVE_OS2_CDROM
  return NULL;
#else
  char **drives = NULL;
  unsigned int num_drives=0;

  struct {
    BYTE uc_cmd_info;
    BYTE uc_drive;
  } s_param;

  struct {
    struct {
      USHORT  us_bytes_per_sector;
      BYTE    uc_sectors_per_cluster;
      USHORT  us_reserved_sectors;
      BYTE    uc_number_of_fats;
      USHORT  us_root_dir_entries;
      USHORT  us_total_sectors;
      BYTE    uc_media_descriptor;
      USHORT  us_sectors_per_fat;
      USHORT  us_sectors_per_track;
      USHORT  us_number_of_heads;
      ULONG   ul_hidden_sectors;
      ULONG   ul_large_total_sectors;
      BYTE    auc_reserved[6];
    } s_ebpb;

    USHORT  us_cylinders;
    BYTE    uc_dev_type;
    USHORT  us_dev_attr;
  } s_data;

  ULONG ul_param_len;
  ULONG ul_data_len;
  UCHAR uc_drive;
  char  sz_drive_str[ 3 ] = "X:";
  ULONG rc;

  /* Scan the system for CD-ROM drives.
  */

  for( uc_drive = 0; uc_drive < 26; uc_drive++ )
  {
    s_param.uc_cmd_info = 0;
    s_param.uc_drive = uc_drive;

    rc = DosDevIOCtl(
          ( HFILE )-1, IOCTL_DISK, DSK_GETDEVICEPARAMS,
          &s_param, sizeof( s_param ), &ul_param_len,
          &s_data, sizeof( s_data ), &ul_data_len );

    if( rc )
      continue;

    switch( s_data.s_ebpb.uc_media_descriptor )
    {
      case       4  :   /* CD-R                          */
      case 128 + 4  :   /* CD-R    but cannot be written */
      case       5  :   /* CD-ROM                        */
      case 128 + 5  :   /* CD-ROM  but cannot be written */
      case       6  :   /* DVD-ROM                       */
      case 128 + 6  :   /* DVD-ROM but cannot be written */
      case       7  :   /* DVD-RAM                       */
      case 128 + 7  :   /* DVD-RAM but cannot be written */
      case       8  :   /* CD-RW                         */
      case 128 + 8  :   /* CD-RW   but cannot be written */
      case       9  :   /* DVD-R                         */
      case 128 + 9  :   /* DVD-R   but cannot be written */
      case       10 :   /* DVD-RW                        */
      case 128 + 10 :   /* DVD-RW  but cannot be written */
      case       11 :   /* DVD+RW                        */
      case 128 + 11 :   /* DVD+RW  but cannot be written */
        sz_drive_str[0] = 'A' + uc_drive;
        cdio_add_device_list(&drives, strdup(sz_drive_str), &num_drives);
        break;
    }
  }

  cdio_add_device_list(&drives, NULL, &num_drives);
  return drives;
#endif /*HAVE_OS2_CDROM*/
}

#define IOCTL_CDROMDISK2        0x82

#define CDROMDISK2_DRIVELETTERS 0x60

/*!
  Return a string containing the default CD device if none is specified.
  if CdIo is NULL (we haven't initialized a specific device driver),
  then find a suitable one and return the default device for that.

  NULL is returned if we couldn't get a default device.
*/
char *
cdio_get_default_device_os2(void)
{
#ifdef HAVE_OS2_CDROM
  struct {
    USHORT us_drive_count;
    USHORT us_drive_first;
  } s_drive_letters;

  HFILE h_cd2;
  ULONG ul_action;
  ULONG ul_param_len;
  ULONG ul_data_len;
  char  sz_drive_str[ 3 ] = "X:";
  ULONG rc;

  rc = DosOpen((PSZ)"CD-ROM2$", &h_cd2, &ul_action, 0, FILE_NORMAL,
               OPEN_ACTION_OPEN_IF_EXISTS | OPEN_ACTION_FAIL_IF_NEW,
               OPEN_ACCESS_READONLY | OPEN_SHARE_DENYNONE,
               NULL );

  if( rc )
  {
    cdio_warn("cdio_get_default_device_os2 : DosOpen(CD-ROM2$) = %ld\n", rc );

    return NULL;
  }

  rc = DosDevIOCtl(
          h_cd2, IOCTL_CDROMDISK2, CDROMDISK2_DRIVELETTERS,
          NULL, 0, &ul_param_len,
          &s_drive_letters, sizeof( s_drive_letters ), &ul_data_len );

  DosClose( h_cd2 );

  if( rc )
  {
    cdio_warn("cdio_get_default_device_os2 : DosDevIOCtl(DRIVELETTERS) = 0x%lx\n", rc );

    return NULL;
  }

  if( s_drive_letters.us_drive_count == 0 )
    return NULL;

  sz_drive_str[0] = 'A' + s_drive_letters.us_drive_first;

  return strdup( sz_drive_str );
#else
  return NULL;
#endif
}

/*!
  Return true if source_name could be a device containing a CD-ROM.
*/
bool
cdio_is_device_os2(const char *source_name)
{
  if (!source_name)
    return false;

#ifdef HAVE_OS2_CDROM
  return (isalpha(source_name[0]) &&
          source_name[1] == ':' &&
          source_name[2] == '\0');
#else
  return false;
#endif
}

/*!
  Close tray on CD-ROM.

  @param p_user_data the CD object to be acted upon.

*/
driver_return_code_t
close_tray_os2 (const char *psz_os2_drive)
{
#ifdef HAVE_OS2_CDROM
  struct {
    BYTE uc_cmd_info;
    BYTE uc_drive;
  } s_param;

  ULONG ul_param_len;
  ULONG ul_data_len;
  ULONG rc;

  s_param.uc_cmd_info = 3;
  s_param.uc_drive    = toupper(psz_os2_drive[0]) - 'A';

  rc = DosDevIOCtl(
             ( HFILE )-1, IOCTL_DISK, DSK_UNLOCKEJECTMEDIA,
             &s_param, sizeof( s_param ), &ul_param_len,
             NULL, 0, &ul_data_len );

  if( rc && rc != 99 /* device in use */ )
  {
    cdio_warn("close_tray_os2 : DosDevIOCtl(UNLOCKEJECTMEDIA) = 0x%lx\n", rc );

    return DRIVER_OP_ERROR;
  }

  return DRIVER_OP_SUCCESS;
#else
  return DRIVER_OP_UNSUPPORTED;
#endif /* HAVE_OS2_CDROM */
}

/*!
  Initialization routine. This is the only thing that doesn't
  get called via a function pointer. In fact *we* are the
  ones to set that up.
 */
CdIo_t *
cdio_open_os2 (const char *psz_orig_source)
{

#ifdef HAVE_OS2_CDROM
  CdIo_t *ret;
  _img_private_t *_data;
  char *psz_source;

  cdio_funcs_t _funcs;

  memset( &_funcs, 0, sizeof(_funcs) );

  _funcs.audio_get_volume       = audio_get_volume_os2;
  _funcs.audio_pause            = audio_pause_os2;
  _funcs.audio_play_msf         = audio_play_msf_os2;
#if 0
  _funcs.audio_play_track_index = audio_play_track_index_os2;
#endif
  _funcs.audio_read_subchannel  = audio_read_subchannel_os2;
  _funcs.audio_resume           = audio_resume_os2;
  _funcs.audio_set_volume       = audio_set_volume_os2;
  _funcs.audio_stop             = audio_stop_os2;
  _funcs.eject_media            = eject_media_os2;
  _funcs.free                   = free_os2;
  _funcs.get_arg                = get_arg_os2;
#if 0
  _funcs.get_blocksize          = get_blocksize_os2;
#endif
  _funcs.get_cdtext             = get_cdtext_generic;
  _funcs.get_cdtext_raw         = read_cdtext_generic;
  _funcs.get_default_device     = cdio_get_default_device_os2;
  _funcs.get_devices            = cdio_get_devices_os2;
  _funcs.get_disc_last_lsn      = get_disc_last_lsn_os2;
  _funcs.get_discmode           = get_discmode_os2;
  _funcs.get_drive_cap          = get_drive_cap_mmc;
  _funcs.get_first_track_num    = get_first_track_num_generic;
  _funcs.get_hwinfo             = NULL;
#if 0
  _funcs.get_last_session       = get_last_session_os2;
#endif
  _funcs.get_media_changed      = get_media_changed_mmc;
  _funcs.get_mcn                = _cdio_get_mcn;
  _funcs.get_num_tracks         = get_num_tracks_generic;
  _funcs.get_track_channels     = get_track_channels_generic;
  _funcs.get_track_copy_permit  = get_track_copy_permit_generic;
  _funcs.get_track_format       = _cdio_get_track_format;
  _funcs.get_track_green        = _cdio_get_track_green;
  _funcs.get_track_lba          = NULL; /* This could be done if need be. */
#if 0
  _funcs.get_track_pregap_lba   = get_track_pregap_lba_os2;
#endif
  _funcs.get_track_isrc         = _cdio_get_track_isrc;
  _funcs.get_track_msf          = _cdio_get_track_msf;
  _funcs.get_track_preemphasis  = get_track_preemphasis_generic;
  _funcs.lseek                  = cdio_generic_lseek;
  _funcs.read                   = cdio_generic_read;
  _funcs.read_audio_sectors     = read_audio_sectors_os2;
  _funcs.read_data_sectors      = read_data_sectors_mmc;
  _funcs.read_mode1_sector      = read_mode1_sector_os2;
  _funcs.read_mode1_sectors     = read_mode1_sectors_os2;
  _funcs.read_mode2_sector      = read_mode2_sector_os2;
  _funcs.read_mode2_sectors     = read_mode2_sectors_os2;
  _funcs.read_toc               = read_toc_os2;
  _funcs.run_mmc_cmd            = run_mmc_cmd_os2;
  _funcs.set_arg                = set_arg_os2;
  _funcs.set_blocksize          = set_blocksize_mmc;
  _funcs.set_speed              = set_drive_speed_mmc;

  _data                 = calloc(1, sizeof (_img_private_t));
  _data->access_mode    = _AM_OS2;
  _data->gen.b_cdtext_error = false;
  _data->gen.init       = false;
  _data->gen.fd         = -1;

  if (NULL == psz_orig_source) {
    psz_source=cdio_get_default_device_os2();
    if (NULL == psz_source) return NULL;
    set_arg_os2(_data, "source", psz_source);
    free(psz_source);
  } else {
    if (cdio_is_device_os2(psz_orig_source))
      set_arg_os2(_data, "source", psz_orig_source);
    else {
      /* The below would be okay if all device drivers worked this way. */
#if 0
      cdio_info ("source %s is a not a device", psz_orig_source);
#endif
      free(_data);
      return NULL;
    }
  }

  ret = cdio_new ((void *)_data, &_funcs);
  if (ret == NULL) return NULL;

  ret->driver_id = DRIVER_OS2;

  if (init_os2(_data))
    return ret;
  else {
    free_os2 (_data);
    free(ret);
    return NULL;
  }
#else
  return NULL;
#endif /* HAVE_OS2_CDROM */

}

/*!
  Initialization routine. This is the only thing that doesn't
  get called via a function pointer. In fact *we* are the
  ones to set that up.
 */
CdIo_t *
cdio_open_am_os2 (const char *psz_source_name, const char *psz_access_mode)
{

  if (psz_access_mode != NULL)
    cdio_warn ("there is only one access mode for OS/2. Arg %s ignored",
               psz_access_mode);
  return cdio_open_os2(psz_source_name);
}

bool
cdio_have_os2 (void)
{
#ifdef HAVE_OS2_CDROM
  return true;
#else
  return false;
#endif /* HAVE_OS2_CDROM */
}
