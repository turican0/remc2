/*
  Copyright (C) 2004-2005, 2008, 2010-2014
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

/* This file contains Win32-specific code using the DeviceIoControl
   access method.
*/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_WIN32_CDROM

# if defined (__MINGW64_VERSION_MAJOR)
#  define _NTSRB_ /* Bad things happen if srb.h gets included */
# endif
# include <windows.h>
# ifdef HAVE_DDK_SCSI_H
#  include <ddk/scsi.h>
# endif
# ifdef HAVE_NTDDCDRM_H
#  include <ntddcdrm.h>
# endif
# ifdef HAVE_DDK_NTDDCDRM_H
#  include <ddk/ntddcdrm.h>
# endif
# ifdef HAVE_NTDDSCSI_H
#  include <ntddscsi.h>
# endif
# ifdef HAVE_DDK_NTDDSCSI_H
#  include <ddk/ntddscsi.h>
# endif

#if defined (_WIN32)
#include <windows.h>
#endif

#include <stddef.h>  /* offsetof() macro */
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>

#include <cdio/cdio.h>
#include <cdio/sector.h>
#include <cdio/util.h>
#include <driver/cdio_assert.h>
#include <cdio/mmc.h>
#include "cdio/logging.h"

#if defined (_XBOX)
#define windows_error(loglevel,i_err) {                    \
   cdio_log(loglevel, "Error: file %s: line %d (%s) %ld\n", \
                  __FILE__, __LINE__, __PRETTY_FUNCTION__, i_err); \
}
#else
#define windows_error(loglevel,i_err) {                                 \
  char error_msg[80];                                                   \
  long int count;                                                       \
  count = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,                     \
                        NULL, i_err, MAKELANGID(LANG_NEUTRAL,           \
                                                SUBLANG_DEFAULT),       \
                        error_msg, sizeof(error_msg), NULL);            \
  (count != 0) ?                                                        \
    cdio_log(loglevel, "Error: file %s: line %d (%s)\n\t%s\n",          \
             __FILE__, __LINE__, __FUNCTION__, error_msg)        \
    :                                                                   \
    cdio_log(loglevel, "Error: file %s: line %d (%s) %ld\n",            \
             __FILE__, __LINE__, __FUNCTION__, i_err);           \
}
#endif

#define MAX_ERROR_BUFFER    256
#define MAX_DATA_BUFFER     2048

typedef struct _TRACK_DATA_FULL {
    UCHAR SessionNumber;
    UCHAR Control : 4;
    UCHAR Adr : 4;
    UCHAR TNO;
    UCHAR POINT;  /* Tracknumber (of session?) or lead-out/in (0xA0, 0xA1, 0xA2)  */
    UCHAR Min;  /* Only valid if disctype is CDDA ? */
    UCHAR Sec;  /* Only valid if disctype is CDDA ? */
    UCHAR Frame;  /* Only valid if disctype is CDDA ? */
    UCHAR Zero;  /* Always zero */
    UCHAR PMIN;  /* start min, if POINT is a track; if lead-out/in 0xA0: First Track */
    UCHAR PSEC;
    UCHAR PFRAME;
} TRACK_DATA_FULL, *PTRACK_DATA_FULL;

typedef struct _CDROM_TOC_FULL {
    UCHAR Length[2];
    UCHAR FirstSession;
    UCHAR LastSession;
    TRACK_DATA_FULL TrackData[CDIO_CD_MAX_TRACKS+3];
} CDROM_TOC_FULL, *PCDROM_TOC_FULL;

#define SPT_CDB_LENGTH 32
#define SPT_SENSE_LENGTH 32
#define SPTWB_DATA_LENGTH 512

#ifndef EMPTY_ARRAY_SIZE
#define EMPTY_ARRAY_SIZE 0
#endif


typedef struct {
   SCSI_PASS_THROUGH_DIRECT sptd;
   ULONG Filler; /* Realign buffer to double-word boundary */
   UCHAR ucSenseBuf[SPT_SENSE_LENGTH];
} SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER;

#include <stdio.h>

typedef struct _SCSI_PASS_THROUGH_WITH_BUFFERS {
    SCSI_PASS_THROUGH    Spt;
    ULONG                Filler;   /* realign buffer to double-word boundary */
    UCHAR                ucSenseBuf[SPT_SENSE_LENGTH];
    UCHAR ucDataBuf[EMPTY_ARRAY_SIZE];
} SCSI_PASS_THROUGH_WITH_BUFFERS;

#ifdef HAVE_STDBOOL_H
# include <stdbool.h>
#endif

#include "win32.h"

#define OP_TIMEOUT_MS 60

/*!
  Pause playing CD through analog output

  @param p_cdio the CD object to be acted upon.
*/
driver_return_code_t
audio_pause_win32ioctl (void *p_user_data)
{
  const _img_private_t *p_env = p_user_data;
  DWORD dw_bytes_returned;

  bool b_success =
    DeviceIoControl(p_env->h_device_handle, IOCTL_CDROM_PAUSE_AUDIO,
                    NULL, (DWORD) 0, NULL, 0, &dw_bytes_returned, NULL);

  if ( ! b_success ) {
    windows_error(CDIO_LOG_INFO, GetLastError());
    return DRIVER_OP_ERROR;
  }
  return DRIVER_OP_SUCCESS;
}

/*!
  Playing starting at given MSF through analog output

  @param p_cdio the CD object to be acted upon.
*/
driver_return_code_t
audio_play_msf_win32ioctl (void *p_user_data, msf_t *p_start_msf,
                           msf_t *p_end_msf)
{
  const _img_private_t *p_env = p_user_data;
  CDROM_PLAY_AUDIO_MSF play;
  DWORD dw_bytes_returned;

  play.StartingM = cdio_from_bcd8(p_start_msf->m);
  play.StartingS = cdio_from_bcd8(p_start_msf->s);
  play.StartingF = cdio_from_bcd8(p_start_msf->f);

  play.EndingM   = cdio_from_bcd8(p_end_msf->m);
  play.EndingS   = cdio_from_bcd8(p_end_msf->s);
  play.EndingF   = cdio_from_bcd8(p_end_msf->f);

  bool b_success =
    DeviceIoControl(p_env->h_device_handle, IOCTL_CDROM_PLAY_AUDIO_MSF,
                    &play, sizeof(play), NULL, 0, &dw_bytes_returned, NULL);

  if ( ! b_success ) {
    windows_error(CDIO_LOG_INFO, GetLastError());
    return DRIVER_OP_ERROR;
  }
  return DRIVER_OP_SUCCESS;

}

/*!
  Read Audio Subchannel information

  @param p_cdio the CD object to be acted upon.

*/
driver_return_code_t
audio_read_subchannel_win32ioctl (void *p_user_data,
                                  cdio_subchannel_t *p_subchannel)
{
  const _img_private_t *p_env = p_user_data;
  DWORD dw_bytes_returned;
  CDROM_SUB_Q_DATA_FORMAT q_data_format;
  SUB_Q_CHANNEL_DATA q_subchannel_data;

  q_data_format.Format = CDIO_SUBCHANNEL_CURRENT_POSITION;
  q_data_format.Track=0; /* Not sure if this has to be set or if so what
                            it should be. */

  if( ! DeviceIoControl( p_env->h_device_handle,
                       IOCTL_CDROM_READ_Q_CHANNEL,
                       &q_data_format, sizeof(q_data_format),
                       &q_subchannel_data, sizeof(q_subchannel_data),
                       &dw_bytes_returned, NULL ) ) {
    windows_error(CDIO_LOG_INFO, GetLastError());
    return DRIVER_OP_ERROR;
  }
  p_subchannel->audio_status =
    q_subchannel_data.CurrentPosition.Header.AudioStatus;
  p_subchannel->track =
    q_subchannel_data.CurrentPosition.TrackNumber;
  p_subchannel->index =
    q_subchannel_data.CurrentPosition.IndexNumber;
  p_subchannel->index =
    q_subchannel_data.CurrentPosition.IndexNumber;
  p_subchannel->address = q_subchannel_data.CurrentPosition.ADR;
  p_subchannel->control = q_subchannel_data.CurrentPosition.Control;

  {
    const UCHAR *abs_addr =
      q_subchannel_data.CurrentPosition.AbsoluteAddress;
    const UCHAR *rel_addr =
      q_subchannel_data.CurrentPosition.TrackRelativeAddress;

    p_subchannel->abs_addr.m = cdio_to_bcd8(abs_addr[1]);
    p_subchannel->abs_addr.s = cdio_to_bcd8(abs_addr[2]);
    p_subchannel->abs_addr.f = cdio_to_bcd8(abs_addr[3]);
    p_subchannel->rel_addr.m = cdio_to_bcd8(rel_addr[1]);
    p_subchannel->rel_addr.s = cdio_to_bcd8(rel_addr[2]);
    p_subchannel->rel_addr.f = cdio_to_bcd8(rel_addr[3]);
  }

  return DRIVER_OP_SUCCESS;
}

/**
  Resume playing an audio CD.

  @param p_user_data the CD object to be acted upon.

*/
driver_return_code_t
audio_resume_win32ioctl (void *p_user_data)
{
  const _img_private_t *p_env = p_user_data;
  DWORD dw_bytes_returned;

  bool b_success =
    DeviceIoControl(p_env->h_device_handle, IOCTL_CDROM_RESUME_AUDIO,
                    NULL, (DWORD) 0, NULL, 0, &dw_bytes_returned, NULL);

  if ( ! b_success ) {
    windows_error(CDIO_LOG_INFO, GetLastError());
    return DRIVER_OP_ERROR;
  }
  return DRIVER_OP_SUCCESS;
}

/**
  Set the volume of an audio CD.

  @param p_user_data pointer to the CD object to be acted upon.
  @param p_volume pointer to the volume levels

*/
driver_return_code_t
audio_set_volume_win32ioctl (void *p_user_data,
                             /*in*/ cdio_audio_volume_t *p_volume)
{
  const _img_private_t *p_env = p_user_data;
  DWORD dw_bytes_returned;

  bool b_success =
    DeviceIoControl(p_env->h_device_handle, IOCTL_CDROM_SET_VOLUME,
                    p_volume, (DWORD) sizeof(cdio_audio_volume_t),
                    NULL, 0, &dw_bytes_returned, NULL);

  if ( ! b_success ) {
    windows_error(CDIO_LOG_INFO, GetLastError());
    return DRIVER_OP_ERROR;
  }
  return DRIVER_OP_SUCCESS;
}

driver_return_code_t
audio_get_volume_win32ioctl (void *p_user_data,
                             /*out*/ cdio_audio_volume_t *p_volume)
{
  const _img_private_t *p_env = p_user_data;
  DWORD dw_bytes_returned;

  bool b_success =
    DeviceIoControl(p_env->h_device_handle, IOCTL_CDROM_GET_VOLUME,
                    NULL, 0,
                    p_volume, (DWORD) sizeof(cdio_audio_volume_t),
                    &dw_bytes_returned, NULL);

  if ( ! b_success ) {
    windows_error(CDIO_LOG_INFO, GetLastError());
    return DRIVER_OP_ERROR;
  }
  return DRIVER_OP_SUCCESS;
}

/**
  Stop playing an audio CD.

  @param p_user_data the CD object to be acted upon.

*/
driver_return_code_t
audio_stop_win32ioctl (void *p_user_data)
{
  const _img_private_t *p_env = p_user_data;
  DWORD dw_bytes_returned;

  bool b_success =
    DeviceIoControl(p_env->h_device_handle, IOCTL_CDROM_STOP_AUDIO,
                    NULL, (DWORD) 0, NULL, 0, &dw_bytes_returned, NULL);

  if ( ! b_success ) {
    windows_error(CDIO_LOG_INFO, GetLastError());
    return DRIVER_OP_ERROR;
  }
  return DRIVER_OP_SUCCESS;
}

/**
  Close the tray of a CD-ROM

  @param p_user_data the CD object to be acted upon.

*/
driver_return_code_t
close_tray_win32ioctl (const char *psz_win32_drive)
{
#ifdef WIN32
  DWORD dw_bytes_returned;
  DWORD dw_access_flags;

  OSVERSIONINFO ov;
  HANDLE h_device_handle;
  BOOL   b_success;

  memset(&ov,0,sizeof(OSVERSIONINFO));
  ov.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
  GetVersionEx(&ov);

  if((ov.dwPlatformId==VER_PLATFORM_WIN32_NT) &&
     (ov.dwMajorVersion>4))
    dw_access_flags = GENERIC_READ|GENERIC_WRITE;  /* add gen write on W2k/XP */
  else dw_access_flags = GENERIC_READ;

  h_device_handle = CreateFile( psz_win32_drive,
                                dw_access_flags,
                                FILE_SHARE_READ | FILE_SHARE_WRITE,
                                NULL,
                                OPEN_EXISTING,
                                0,
                                NULL );

  if( h_device_handle == INVALID_HANDLE_VALUE ) {
    return DRIVER_OP_ERROR;
  }

  b_success =
    DeviceIoControl(h_device_handle, IOCTL_STORAGE_LOAD_MEDIA2,
                    NULL, (DWORD) 0, NULL, 0, &dw_bytes_returned, NULL);


  CloseHandle(h_device_handle);

  if ( b_success != 0 ) {
    windows_error(CDIO_LOG_INFO, GetLastError());
    return DRIVER_OP_ERROR;
  }
  return DRIVER_OP_SUCCESS;
#else
  return DRIVER_OP_UNSUPPORTED;
#endif
}

/**
  Produce a text composed from the system SCSI address tuple
  "Port,Path,Target,Lun" and store
  it in generic_img_private_t.scsi_tuple.
  To be accessed via cdio_get_arg("scsi-tuple-linux") or ("scsi-tuple").
  Drivers which implement this code have to return 5 valid decimal numbers
  separated by comma, or empty text if no such numbers are available.
  @return   1=success , 0=failure
*/
static int
set_scsi_tuple_win32ioctl(_img_private_t *env)
#ifdef WIN32
{
  char tuple[160];
  char dataBuffer[MAX_DATA_BUFFER];
  PSCSI_ADDRESS scsiAddress = (PSCSI_ADDRESS) dataBuffer;
  ULONG bytesReturned;

  memset(dataBuffer, 0, sizeof(dataBuffer));
  if (DeviceIoControl(env->h_device_handle,
                      IOCTL_SCSI_GET_ADDRESS,
                      NULL,
                      0,
                      dataBuffer,
                      sizeof(dataBuffer),
                      &bytesReturned,
                      FALSE
                      )) {
    snprintf(tuple, sizeof(tuple), "%d,%d,%d,%d",
            scsiAddress->PortNumber,
            scsiAddress->PathId,
            scsiAddress->TargetId,
            scsiAddress->Lun);
    env->gen.scsi_tuple = strdup(tuple);
    return 1;
  } else {
    /* No tuple. */
    env->gen.scsi_tuple = strdup("");
    return 0;
  }
}
#else
{
  env->gen.scsi_tuple = strdup("");
  return 0;
}
#endif

/**
  Run a SCSI MMC command.

  p_user_data   private CD structure
  u_timeout_ms  time in milliseconds we will wait for the command
                to complete. If this value is -1, use the default
                time-out value.
  u_cdb         CDB length
  p_cdb         CDB bytes. All values that are needed should be set on
                input.
  e_direction   direction the transfer is to go.
  p_buf         Buffer for data, both sending and receiving
  u_buf         Size of buffer

  Return DRIVER_OP_SUCCESS if command completed successfully.
 */
#if USE_PASSTHROUGH_DIRECT
int
run_mmc_cmd_win32ioctl( void *p_user_data,
                        unsigned int u_timeout_ms,
                        unsigned int u_cdb, const mmc_cdb_t * p_cdb,
                        cdio_mmc_direction_t e_direction,
                        unsigned int u_buf, /*in/out*/ void *p_buf )
{
  _img_private_t *p_env = p_user_data;
  SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER swb;

  BOOL b_success;
  DWORD dw_bytes_returned;
  char dummy_buf[2]; /* Used if we can't use p_buf. See below. */
  int rc = DRIVER_OP_SUCCESS;
  unsigned int u_swb_len =
    sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER);

  p_env->gen.scsi_mmc_sense_valid = 0;
  memset(&swb, 0, u_swb_len);

  swb.sptd.Length  = sizeof(SCSI_PASS_THROUGH_DIRECT);
  swb.sptd.PathId  = 0;      /* SCSI card ID will be filled in
                                automatically */
  swb.sptd.TargetId= 0;      /* SCSI target ID will also be filled in */
  swb.sptd.Lun     = 0;      /* SCSI lun ID will also be filled in */
  swb.sptd.CdbLength         = u_cdb;
  swb.sptd.SenseInfoLength   = sizeof(swb.ucSenseBuf);
  swb.sptd.DataIn            =
    (SCSI_MMC_DATA_READ  == e_direction) ? SCSI_IOCTL_DATA_IN :
    (SCSI_MMC_DATA_WRITE == e_direction) ? SCSI_IOCTL_DATA_OUT :
    SCSI_IOCTL_DATA_UNSPECIFIED;

  /* MS Windows seems to flip out of the size of the buffer is 0 or
     1. For the 1 byte case see: BUG: SCSI Pass Through Fails with
     Invalid User Buffer Error http://support.microsoft.com/kb/259573
     So in those cases we will provide our own.
  */
  if (u_buf <= 1) {
    swb.sptd.DataBuffer         = &dummy_buf;
    swb.sptd.DataTransferLength = 2;
  } else {
    swb.sptd.DataBuffer         = p_buf;
    swb.sptd.DataTransferLength = u_buf;
  }

  swb.sptd.TimeOutValue      = msecs2secs(u_timeout_ms);
  swb.sptd.SenseInfoOffset   =
    offsetof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER, ucSenseBuf);

  p_env->gen.scsi_mmc_sense_valid = 0;
  memcpy(swb.sptd.Cdb, p_cdb, u_cdb);

  /* Send the command to drive */
  b_success = DeviceIoControl(p_env->h_device_handle,
                              IOCTL_SCSI_PASS_THROUGH_DIRECT,
                              (void *)&swb,
                              u_swb_len,
                              &swb,
                              u_swb_len,
                              &dw_bytes_returned,
                              NULL);

  if (u_buf == 1) memcpy(p_buf, &dummy_buf[0], 1);

  if ( 0 == b_success ) {
    long int last_error = GetLastError();
    windows_error(CDIO_LOG_INFO, last_error);
    switch (last_error) {
    case 87:
      rc = DRIVER_OP_BAD_PARAMETER;
      break;
    default:
      rc = DRIVER_OP_ERROR;
    }
  }

#ifdef FIXED_ADDITIONAL_SENSE_BUF
  /* Record SCSI sense reply for API call mmc_last_cmd_sense().
   */
  if (p_swb->ucSenseBuf.additional_sense_len) {
    /* SCSI Primary Command standard
       SPC 4.5.3, Table 26: 252 bytes legal, 263 bytes possible */
    int i_sense_size = p_swb->ucSenseBuf.additional_sense_len + 8;
    if (i_sense_size > sizeof(p_swb->ucSenseBuf)) {
      cdio_warn("Sense size retuned %d is greater buffer size %d\n",
		i_sense_size, sizeof(p_swb->ucSenseBuf));
      sense_size = sizeof(p_swb->ucSenseBuf);
    }
    memcpy((void *) p_env->gen.scsi_mmc_sense, p_swb->ucSenseBuf, i_sense_size);
    p_env->gen.scsi_mmc_sense_valid = sense_size;
    if (DRIVER_OP_SUCCESS == rc)
      rc = DRIVER_OP_MMC_SENSE_DATA;
  }
#endif
  return rc;
}
#else
int
run_mmc_cmd_win32ioctl( void *p_user_data,
                        unsigned int u_timeout_ms,
                        unsigned int u_cdb, const mmc_cdb_t * p_cdb,
                        cdio_mmc_direction_t e_direction,
                        unsigned int u_buf, /*in/out*/ void *p_buf )
{
  _img_private_t *p_env = p_user_data;
  SCSI_PASS_THROUGH_WITH_BUFFERS *p_sptwb;

  BOOL b_success;
  DWORD dw_bytes_returned;
  int rc = DRIVER_OP_SUCCESS;
  unsigned int u_swb_len =
    sizeof(SCSI_PASS_THROUGH_WITH_BUFFERS) + u_buf;

  p_sptwb = malloc(u_swb_len);

  p_env->gen.scsi_mmc_sense_valid = 0;
  memset(p_sptwb, 0, u_swb_len);

  p_sptwb->Spt.Length  = sizeof(SCSI_PASS_THROUGH);
  p_sptwb->Spt.PathId  = 0;      /* SCSI card ID will be filled in
                                   automatically */
  p_sptwb->Spt.TargetId= 0;      /* SCSI target ID will also be filled in */
  p_sptwb->Spt.Lun     = 0;      /* SCSI lun ID will also be filled in */
  p_sptwb->Spt.CdbLength         = u_cdb;
  p_sptwb->Spt.SenseInfoLength   = sizeof(p_sptwb->ucSenseBuf);
  p_sptwb->Spt.DataIn            =
    (SCSI_MMC_DATA_READ  == e_direction) ? SCSI_IOCTL_DATA_IN :
    (SCSI_MMC_DATA_WRITE == e_direction) ? SCSI_IOCTL_DATA_OUT :
    SCSI_IOCTL_DATA_UNSPECIFIED;

  if (SCSI_MMC_DATA_WRITE == e_direction) memcpy(&(p_sptwb->ucDataBuf),
						   p_buf, u_buf);

  p_sptwb->Spt.DataTransferLength= u_buf;
  p_sptwb->Spt.TimeOutValue      = msecs2secs(u_timeout_ms);

  p_sptwb->Spt.DataBufferOffset =
    offsetof(SCSI_PASS_THROUGH_WITH_BUFFERS, ucDataBuf);
  p_sptwb->Spt.SenseInfoOffset =
    offsetof(SCSI_PASS_THROUGH_WITH_BUFFERS, ucSenseBuf);

  memcpy(p_sptwb->Spt.Cdb, p_cdb, u_cdb);

  /*printf("Test 3 Sizeof SCSI_PASS_THROUGH_DIRECT %d\n", sizeof(*p_sptwb));*/
  /* Send the command to drive */
  b_success = DeviceIoControl(p_env->h_device_handle,
                              IOCTL_SCSI_PASS_THROUGH,
                              (void *)p_sptwb,
                              u_swb_len,
                              p_sptwb,
                              u_swb_len,
                              &dw_bytes_returned,
                              NULL);

  if ( 0 == b_success ) {
    char buffer[100];
    snprintf(buffer, sizeof(buffer),
	     "MMC command code: 0x%x\n", p_cdb->field[0]);
    long int last_error = GetLastError();
    windows_error(CDIO_LOG_INFO, last_error);
    cdio_log(CDIO_LOG_INFO, buffer);
    switch (last_error) {
    case 87:
      rc = DRIVER_OP_BAD_PARAMETER;
      break;
    default:
      rc = DRIVER_OP_ERROR;
    }
  }

  memcpy(p_buf, &(p_sptwb->ucDataBuf), u_buf);

  /* Record SCSI sense reply for API call mmc_last_cmd_sense().
   */
  if (p_sptwb->Spt.ScsiStatus && p_sptwb->Spt.SenseInfoLength > 0) {
    int i_sense_size = p_sptwb->Spt.SenseInfoLength;
    if (i_sense_size > sizeof(p_sptwb->ucSenseBuf)) {
      cdio_warn("sense size returned %d is greater buffer size %d\n",
		i_sense_size, sizeof(p_sptwb->ucSenseBuf));
      i_sense_size = sizeof(p_sptwb->ucSenseBuf);
    }
    memcpy((void *) p_env->gen.scsi_mmc_sense, &(p_sptwb->ucSenseBuf),
	   i_sense_size);
    p_env->gen.scsi_mmc_sense_valid = p_sptwb->Spt.SenseInfoLength;
  }
  free(p_sptwb);

  return rc;
}
#endif

/**
  Get disc type associated with cd object.
*/
static discmode_t
dvd_discmode_win32ioctl (_img_private_t *p_env)
{
  discmode_t discmode=CDIO_DISC_MODE_NO_INFO;
  driver_return_code_t rc;

  /* See if this is a DVD. */
  cdio_dvd_struct_t dvd;  /* DVD READ STRUCT for layer 0. */

  dvd.physical.type = CDIO_DVD_STRUCT_PHYSICAL;
  dvd.physical.layer_num = 0;

  rc = mmc_get_dvd_struct_physical_private (p_env, &run_mmc_cmd_win32ioctl,
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


/**
  Get disc type associated with cd object.
*/
discmode_t
get_discmode_win32ioctl (_img_private_t *p_env)
{
  track_t i_track;
  discmode_t discmode;

  if (!p_env) return CDIO_DISC_MODE_ERROR;

  discmode = dvd_discmode_win32ioctl(p_env);

  if (CDIO_DISC_MODE_NO_INFO != discmode) return discmode;

  if (!p_env->gen.toc_init) read_toc_win32ioctl (p_env);

  if (!p_env->gen.toc_init) return CDIO_DISC_MODE_ERROR;

  for (i_track = p_env->gen.i_first_track;
       i_track < p_env->gen.i_first_track + p_env->gen.i_tracks ;
       i_track ++) {
    track_format_t track_fmt=get_track_format_win32ioctl(p_env, i_track);

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

/*
  Returns a string that can be used in a CreateFile call if
  c_drive letter is a character. If not NULL is returned.
 */

const char *
is_cdrom_win32ioctl(const char c_drive_letter)
{
#ifdef _XBOX
  char sz_win32_drive_full[] = "\\\\.\\X:";
  sz_win32_drive_full[4] = c_drive_letter;
  return strdup(sz_win32_drive_full);
#else
  UINT uDriveType;
  char sz_win32_drive[4];

  sz_win32_drive[0]= c_drive_letter;
  sz_win32_drive[1]=':';
  sz_win32_drive[2]='\\';
  sz_win32_drive[3]='\0';

  uDriveType = GetDriveType(sz_win32_drive);

  switch(uDriveType) {
  case DRIVE_CDROM: {
    char sz_win32_drive_full[] = "\\\\.\\X:";
    sz_win32_drive_full[4] = c_drive_letter;
    return strdup(sz_win32_drive_full);
  }
  default:
    cdio_debug("Drive %c is not a CD-ROM", c_drive_letter);
    return NULL;
  }
#endif
}

/**
   Reads an audio device using the DeviceIoControl method into data
   starting from lsn.  Returns 0 if no error.
 */
driver_return_code_t
read_audio_sectors_win32ioctl (_img_private_t *p_env, void *data, lsn_t lsn,
                               unsigned int nblocks)
{
  DWORD dw_bytes_returned;
  RAW_READ_INFO cdrom_raw;

  /* Initialize CDROM_RAW_READ structure */
  cdrom_raw.DiskOffset.QuadPart = (long long) CDIO_CD_FRAMESIZE_RAW * lsn;
  cdrom_raw.SectorCount = nblocks;
  cdrom_raw.TrackMode = CDDA;

  if( DeviceIoControl( p_env->h_device_handle,
                       IOCTL_CDROM_RAW_READ, &cdrom_raw,
                       sizeof(RAW_READ_INFO), data,
                       CDIO_CD_FRAMESIZE_RAW * nblocks,
                       &dw_bytes_returned, NULL ) == 0 ) {
    cdio_info("Error reading audio-mode lsn %lu\n)",
                (long unsigned int) lsn);
    windows_error(CDIO_LOG_INFO, GetLastError());
    return DRIVER_OP_ERROR;
  }
  return DRIVER_OP_SUCCESS;
}

/**
   Reads a single raw sector using the DeviceIoControl method into
   data starting from lsn. Returns 0 if no error.
 */
static int
read_raw_sector (_img_private_t *p_env, void *p_buf, lsn_t lsn)
{
  mmc_cdb_t cdb = {{0, }};

  /* ReadCD CDB12 command.  The values were taken from MMC1 draft paper. */
  CDIO_MMC_SET_COMMAND      (cdb.field, CDIO_MMC_GPCMD_READ_CD);
  CDIO_MMC_SET_READ_LBA     (cdb.field, lsn);
  CDIO_MMC_SET_READ_LENGTH24(cdb.field, 1);

  cdb.field[9]=0xF8;  /* Raw read, 2352 bytes per sector */

  return run_mmc_cmd_win32ioctl(p_env, OP_TIMEOUT_MS,
                                mmc_get_cmd_len(cdb.field[0]),
                                &cdb, SCSI_MMC_DATA_READ,
                                CDIO_CD_FRAMESIZE_RAW, p_buf);
}

/**
   Reads a single mode2 sector using the DeviceIoControl method into
   data starting from lsn. Returns 0 if no error.
 */
int
read_mode2_sector_win32ioctl (_img_private_t *p_env, void *p_data,
                              lsn_t lsn, bool b_form2)
{
  char buf[CDIO_CD_FRAMESIZE_RAW] = { 0, };
  int ret = read_raw_sector (p_env, buf, lsn);

  if ( 0 != ret) return ret;

  memcpy (p_data,
          buf + CDIO_CD_SYNC_SIZE + CDIO_CD_XA_HEADER,
          b_form2 ? M2RAW_SECTOR_SIZE: CDIO_CD_FRAMESIZE);

  return 0;

}

/**
   Reads a single mode2 sector using the DeviceIoControl method into
   data starting from lsn. Returns 0 if no error.
 */
int
read_mode1_sector_win32ioctl (_img_private_t *env, void *data,
                              lsn_t lsn, bool b_form2)
{
  char buf[CDIO_CD_FRAMESIZE_RAW] = { 0, };
  int ret = read_raw_sector (env, buf, lsn);

  if ( 0 != ret) return ret;

  memcpy (data,
          buf + CDIO_CD_SYNC_SIZE+CDIO_CD_HEADER_SIZE,
          b_form2 ? M2RAW_SECTOR_SIZE: CDIO_CD_FRAMESIZE);

  return 0;

}

/**
  Initialize internal structures for CD device.
 */
bool
init_win32ioctl (_img_private_t *env)
{
#ifdef WIN32
  OSVERSIONINFO ov;
#endif

#ifdef _XBOX
  ANSI_STRING filename;
  OBJECT_ATTRIBUTES attributes;
  IO_STATUS_BLOCK status;
  HANDLE hDevice;
  NTSTATUS error;
#else
  unsigned int len=strlen(env->gen.source_name);
  char psz_win32_drive[7];
  DWORD dw_access_flags;
#endif

  cdio_debug("using winNT/2K/XP ioctl layer");

#ifdef WIN32
  memset(&ov,0,sizeof(OSVERSIONINFO));
  ov.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
  GetVersionEx(&ov);

  if((ov.dwPlatformId==VER_PLATFORM_WIN32_NT) &&
     (ov.dwMajorVersion>4))
    dw_access_flags = GENERIC_READ|GENERIC_WRITE;  /* add gen write on W2k/XP */
  else dw_access_flags = GENERIC_READ;
#endif

  if (cdio_is_device_win32(env->gen.source_name))
  {
#ifdef _XBOX
    //  Use XBOX cdrom, no matter what drive letter is given.
    RtlInitAnsiString(&filename,"\\Device\\Cdrom0");
    InitializeObjectAttributes(&attributes, &filename, OBJ_CASE_INSENSITIVE,
                               NULL);
    error = NtCreateFile( &hDevice,
                          GENERIC_READ |SYNCHRONIZE | FILE_READ_ATTRIBUTES,
                          &attributes,
                          &status,
                          NULL,
                          0,
                          FILE_SHARE_READ,
                          FILE_OPEN,
                          FILE_NON_DIRECTORY_FILE
                          | FILE_SYNCHRONOUS_IO_NONALERT );

    if (!NT_SUCCESS(error))
    {
          return false;
    }
    env->h_device_handle = hDevice;
#else
    snprintf( psz_win32_drive, sizeof(psz_win32_drive),
                                      "\\\\.\\%c:",
                                      env->gen.source_name[len-2] );

    env->h_device_handle = CreateFile( psz_win32_drive,
                                       dw_access_flags,
                                       FILE_SHARE_READ | FILE_SHARE_WRITE,
                                       NULL,
                                       OPEN_EXISTING,
                                       0,
                                       NULL );

    if( env->h_device_handle == INVALID_HANDLE_VALUE )
    {
          /* No good. try toggle write. */
          dw_access_flags ^= GENERIC_WRITE;
          env->h_device_handle = CreateFile( psz_win32_drive,
                                             dw_access_flags,
                                             FILE_SHARE_READ,
                                             NULL,
                                             OPEN_EXISTING,
                                             0,
                                             NULL );
          if (env->h_device_handle == NULL)
                return false;
    }
#endif
    env->b_ioctl_init = true;
    set_scsi_tuple_win32ioctl(env);
    return true;
  }
  return false;
}

/**
  Read and cache the CD's Track Table of Contents and track info.
  via a SCSI MMC READ_TOC (FULTOC).  Return true if successful or
  false if an error.
*/
static bool
read_fulltoc_win32mmc (_img_private_t *p_env)
{
  mmc_cdb_t  cdb = {{0, }};
  CDROM_TOC_FULL  cdrom_toc_full;
  int             i_status, i, j;
  int             i_track_format = 0;
  int             i_seen_flag;

  /* Operation code */
  CDIO_MMC_SET_COMMAND(cdb.field, CDIO_MMC_GPCMD_READ_TOC);

  cdb.field[1] = 0x00;

  /* Format */
  cdb.field[2] = CDIO_MMC_READTOC_FMT_FULTOC;

  memset(&cdrom_toc_full, 0, sizeof(cdrom_toc_full));

  /* Setup to read header, to get length of data */
  CDIO_MMC_SET_READ_LENGTH16(cdb.field, sizeof(cdrom_toc_full));

  i_status = run_mmc_cmd_win32ioctl (p_env, 1000*60*3,
                                     mmc_get_cmd_len(cdb.field[0]),
                                     &cdb, SCSI_MMC_DATA_READ,
                                     sizeof(cdrom_toc_full), &cdrom_toc_full);

  if ( 0 != i_status ) {
    cdio_debug ("SCSI MMC READ_TOC failed\n");
    return false;
  }

  i_seen_flag=0;
  for( i = 0 ; i <= CDIO_CD_MAX_TRACKS+3; i++ ) {

    if ( 0xA0 == cdrom_toc_full.TrackData[i].POINT ) {
      /* First track number */
      p_env->gen.i_first_track = cdrom_toc_full.TrackData[i].PMIN;
      i_track_format = cdrom_toc_full.TrackData[i].PSEC;
      i_seen_flag|=0x01;
    }

    if ( 0xA1 == cdrom_toc_full.TrackData[i].POINT ) {
      /* Last track number */
      p_env->gen.i_tracks =
        cdrom_toc_full.TrackData[i].PMIN - p_env->gen.i_first_track + 1;
      i_seen_flag|=0x02;
    }

    j = cdrom_toc_full.TrackData[i].POINT;
    if ( 0xA2 ==  j ) {
      /* Start position of the lead out */
      p_env->tocent[ p_env->gen.i_tracks ].start_lsn =
        cdio_lba_to_lsn(
                        cdio_msf3_to_lba(
                                         cdrom_toc_full.TrackData[i].PMIN,
                                         cdrom_toc_full.TrackData[i].PSEC,
                                         cdrom_toc_full.TrackData[i].PFRAME
                                         )
                        );
      p_env->tocent[ p_env->gen.i_tracks ].Control
        = cdrom_toc_full.TrackData[i].Control;
      p_env->tocent[ p_env->gen.i_tracks ].Format  = i_track_format;
      i_seen_flag|=0x04;
    }

    if (cdrom_toc_full.TrackData[i].POINT > 0
        && cdrom_toc_full.TrackData[i].POINT <= p_env->gen.i_tracks) {
      p_env->tocent[j-1].start_lsn =
        cdio_lba_to_lsn(
                        cdio_msf3_to_lba(
                                         cdrom_toc_full.TrackData[i].PMIN,
                                         cdrom_toc_full.TrackData[i].PSEC,
                                         cdrom_toc_full.TrackData[i].PFRAME
                                         )
                        );
      p_env->tocent[j-1].Control =
        cdrom_toc_full.TrackData[i].Control;
      p_env->tocent[j-1].Format  = i_track_format;

      set_track_flags(&(p_env->gen.track_flags[j]),
                      p_env->tocent[j-1].Control);

      cdio_debug("p_sectors: %i, %lu", i,
                 (unsigned long int) (p_env->tocent[i].start_lsn));

      if (cdrom_toc_full.TrackData[i].POINT == p_env->gen.i_tracks)
        i_seen_flag|=0x08;
    }

    if ( 0x0F == i_seen_flag ) break;
  }
  if ( 0x0F == i_seen_flag ) {
    p_env->gen.toc_init = true;
    return true;
  }
  return false;
}

/**
  Read and cache the CD's Track Table of Contents and track info.
  Return true if successful or false if an error.
*/
bool
read_toc_win32ioctl (_img_private_t *p_env)
{
  CDROM_TOC    cdrom_toc;
  DWORD        dw_bytes_returned;
  unsigned int i, j;
  bool         b_fulltoc_first;  /* Do we do fulltoc or DeviceIoControl
                                    first? */
  if ( ! p_env ) return false;

  /*
     The MMC5 spec says:
       For media other than CD, information may be fabricated in order
                                            ^^^ ^^
       to emulate a CD structure for the specific media.

     There is no requirement though that it *has* to and some DVD
     drives like one by Thompson for XBOX don't support a
     IOCTL_CDROM_READ_TOC for DVD's. So if we have a DVD we should not
     prefer getting the TOC via MMC.

     But on the other hand in GNU/Linux it is reported that using the
     TOC via MMC gives better information such as for CD DATA Form 2 (used
     in SVCDs). So if we *don't* have a DVD I think we want to try MMC
     first.

     Is this complicated enough? I could be wrong...

   */
  b_fulltoc_first = (CDIO_DISC_MODE_NO_INFO == dvd_discmode_win32ioctl(p_env));

  if ( b_fulltoc_first && read_fulltoc_win32mmc(p_env) ) return true;

  /* SCSI-MMC READ_TOC (FULTOC) read failed or we don't want to try it
     initiaily.  Try reading TOC via DeviceIoControl... */

  if( DeviceIoControl( p_env->h_device_handle,
                       IOCTL_CDROM_READ_TOC,
                       NULL, 0, &cdrom_toc, sizeof(CDROM_TOC),
                       &dw_bytes_returned, NULL ) == 0 ) {
    cdio_log_level_t loglevel = b_fulltoc_first
      ? CDIO_LOG_WARN : CDIO_LOG_DEBUG;


    cdio_log(loglevel, "could not read TOC");
    windows_error(loglevel, GetLastError());

#ifdef RUN_MMC_CMD_WIN32IOCTL_FULLY_FIXED
    /* rocky: there is some brokenness in run_mmc_cmd_win32ioctl. So until that
       is fixed, not running the below will mitigate that somewhat. */
    if ( !b_fulltoc_first && read_fulltoc_win32mmc(p_env) ) return true;
#endif
    return false;
  }

  p_env->gen.i_first_track = cdrom_toc.FirstTrack;
  p_env->gen.i_tracks  = cdrom_toc.LastTrack - cdrom_toc.FirstTrack + 1;

  j = p_env->gen.i_first_track;
  for( i = 0 ; i <= p_env->gen.i_tracks ; i++, j++ ) {
    p_env->tocent[ i ].start_lsn =
      cdio_lba_to_lsn(
                      cdio_msf3_to_lba( cdrom_toc.TrackData[i].Address[1],
                                        cdrom_toc.TrackData[i].Address[2],
                                        cdrom_toc.TrackData[i].Address[3] )
                      );
    p_env->tocent[i].Control   = cdrom_toc.TrackData[i].Control;
    p_env->tocent[i].Format    = cdrom_toc.TrackData[i].Adr;

    p_env->gen.track_flags[j].preemphasis =
      p_env->tocent[i].Control & 0x1
      ? CDIO_TRACK_FLAG_TRUE : CDIO_TRACK_FLAG_FALSE;

    p_env->gen.track_flags[j].copy_permit =
      p_env->tocent[i].Control & 0x2
      ? CDIO_TRACK_FLAG_TRUE : CDIO_TRACK_FLAG_FALSE;

    p_env->gen.track_flags[j].channels =
      p_env->tocent[i].Control & 0x8 ? 4 : 2;


    cdio_debug("p_sectors: %i, %lu", i,
               (unsigned long int) (p_env->tocent[i].start_lsn));
  }
  p_env->gen.toc_init = true;
  return true;
}

/**
  Get the LSN of the first track of the last session of
  on the CD.
 */
driver_return_code_t
get_last_session_win32ioctl (void *p_user_data,
                             /*out*/ lsn_t *i_last_session)
{
  const _img_private_t *p_env = p_user_data;
  DWORD dw_bytes_returned;
  CDROM_TOC_SESSION_DATA session;

  bool b_success =
    DeviceIoControl(p_env->h_device_handle, IOCTL_CDROM_GET_LAST_SESSION,
                    NULL, (DWORD) 0, &session, sizeof(session), &dw_bytes_returned, NULL);

  if ( ! b_success ) {
    windows_error(CDIO_LOG_INFO, GetLastError());
    return DRIVER_OP_ERROR;
  }

  *i_last_session = (session.TrackData[0].Address[0] << 24) |
    (session.TrackData[0].Address[1] << 16) |
    (session.TrackData[0].Address[2] << 8) |
    (session.TrackData[0].Address[3]);

  return DRIVER_OP_SUCCESS;
}

/**
  Return the media catalog number MCN.

  Note: string is malloc'd so caller should free() then returned
  string when done with it.

 */
char *
get_mcn_win32ioctl (const _img_private_t *p_env) {

  DWORD dw_bytes_returned;
  SUB_Q_MEDIA_CATALOG_NUMBER mcn;
  CDROM_SUB_Q_DATA_FORMAT q_data_format;

  memset( &mcn, 0, sizeof(mcn) );

  q_data_format.Format = CDIO_SUBCHANNEL_MEDIA_CATALOG;

  /* MSDN info on CDROM_SUB_Q_DATA_FORMAT says if Format is set to
     get MCN, track must be set 0.
   */
  q_data_format.Track=0;

  if( ! DeviceIoControl( p_env->h_device_handle,
                       IOCTL_CDROM_READ_Q_CHANNEL,
                       &q_data_format, sizeof(q_data_format),
                       &mcn, sizeof(mcn),
                       &dw_bytes_returned, NULL ) ) {
    cdio_warn( "could not read Q Channel at track %d", 1);
  } else if (mcn.Mcval)
    return strdup((const char *) mcn.MediaCatalog);
  return NULL;
}

/**
  Return the international standard recording code ISRC.

  Note: string is malloc'd so caller should free() then returned
  string when done with it.

 */
char *
get_track_isrc_win32ioctl (const _img_private_t *p_env, track_t i_track) {

  DWORD dw_bytes_returned;
  SUB_Q_TRACK_ISRC isrc;
  CDROM_SUB_Q_DATA_FORMAT q_data_format;

  memset( &isrc, 0, sizeof(isrc) );

  q_data_format.Format = CDIO_SUBCHANNEL_TRACK_ISRC;
  q_data_format.Track  = i_track;

  if( ! DeviceIoControl( p_env->h_device_handle,
                       IOCTL_CDROM_READ_Q_CHANNEL,
                       &q_data_format, sizeof(q_data_format),
                       &isrc, sizeof(isrc),
                       &dw_bytes_returned, NULL ) ) {
    cdio_warn( "could not read Q Channel at track %d", 1);
  } else if (isrc.Tcval)
    return strdup((const char *) isrc.TrackIsrc);
  return NULL;
}

/**
  Get the format (XA, DATA, AUDIO) of a track.
*/
track_format_t
get_track_format_win32ioctl(const _img_private_t *env, track_t i_track)
{
  /* This is pretty much copied from the "badly broken" cdrom_count_tracks
     in linux/cdrom.c.
  */

  if (env->tocent[i_track - env->gen.i_first_track].Control & 0x04) {
    if (env->tocent[i_track - env->gen.i_first_track].Format == 0x10)
      return TRACK_FORMAT_CDI;
    else if (env->tocent[i_track - env->gen.i_first_track].Format == 0x20)
      return TRACK_FORMAT_XA;
    else
      return TRACK_FORMAT_DATA;
  } else
    return TRACK_FORMAT_AUDIO;
}

#endif /*HAVE_WIN32_CDROM*/
