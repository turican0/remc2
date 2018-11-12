/*
  Copyright (C) 2008, 2010, 2011, 2012 Rocky Bernstein <rocky@gnu.org>
  Copyright (C) 2014 Robert Kausch <robert.kausch@freac.org>

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

/* Changes up to version 0.76 */
/*
 * Copyright (c) 2003
 *      Matthias Drochner.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_STDBOOL_H
# include <stdbool.h>
#endif 

#include <cdio/sector.h>
#include <cdio/util.h>
#include "cdio_assert.h"
#include "cdio_private.h"

#if defined(__NetBSD__) && defined(__i386__)
#define DEFAULT_CDIO_DEVICE "/dev/rcd0d"
#else
#define DEFAULT_CDIO_DEVICE "/dev/rcd0c"
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_NETBSD_CDROM
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/cdio.h>
#include <sys/scsiio.h>

#define TOTAL_TRACKS (_obj->tochdr.ending_track \
                      - _obj->tochdr.starting_track + 1)
#define FIRST_TRACK_NUM (_obj->tochdr.starting_track)

typedef struct {
        generic_img_private_t gen; 

        bool toc_valid;
        struct ioc_toc_header tochdr;
        struct cd_toc_entry tocent[100];

        bool sessionformat_valid;
        int sessionformat[100]; /* format of the session the track is in */
} _img_private_t;

static driver_return_code_t
run_scsi_cmd_netbsd(void *p_user_data, unsigned int i_timeout_ms,
                    unsigned int i_cdb, const mmc_cdb_t *p_cdb, 
                    cdio_mmc_direction_t e_direction, 
                    unsigned int i_buf, void *p_buf )
{
        const _img_private_t *_obj = p_user_data;
        scsireq_t req;

        memset(&req, 0, sizeof(req));
        memcpy(&req.cmd[0], p_cdb, i_cdb);
        req.cmdlen = i_cdb;
        req.datalen = i_buf;
        req.databuf = p_buf;
        req.timeout = i_timeout_ms;
        req.flags = e_direction == SCSI_MMC_DATA_READ ? SCCMD_READ : SCCMD_WRITE;

        if (ioctl(_obj->gen.fd, SCIOCCOMMAND, &req) < 0) {
                perror("SCIOCCOMMAND");
                return -1;
        }
        if (req.retsts != SCCMD_OK) {
                fprintf(stderr, "SCIOCCOMMAND cmd 0x%02x sts %d\n", req.cmd[0], req.retsts);
                return -1;
        }

        return 0;
}

static int
read_audio_sectors_netbsd(void *user_data, void *data, lsn_t lsn,
                          unsigned int nblocks)
{
        scsireq_t req;
        _img_private_t *_obj = user_data;

        memset(&req, 0, sizeof(req));
        req.cmd[0] = 0xbe;
        req.cmd[1] = 0;
        req.cmd[2] = (lsn >> 24) & 0xff;
        req.cmd[3] = (lsn >> 16) & 0xff;
        req.cmd[4] = (lsn >> 8) & 0xff;
        req.cmd[5] = (lsn >> 0) & 0xff;
        req.cmd[6] = (nblocks >> 16) & 0xff;
        req.cmd[7] = (nblocks >> 8) & 0xff;
        req.cmd[8] = (nblocks >> 0) & 0xff;
        req.cmd[9] = 0x78;
        req.cmdlen = 10;

        req.datalen = nblocks * CDIO_CD_FRAMESIZE_RAW; 
        req.databuf = data;
        req.timeout = 10000;
        req.flags = SCCMD_READ;

        if (ioctl(_obj->gen.fd, SCIOCCOMMAND, &req) < 0) {
                perror("SCIOCCOMMAND");
                return 1;
        }
        if (req.retsts != SCCMD_OK) {
                fprintf(stderr, "SCIOCCOMMAND cmd 0xbe sts %d\n", req.retsts);
                return 1;
        }

        return 0;
}

static int
read_mode2_sector_netbsd(void *user_data, void *data, lsn_t lsn, 
                         bool mode2_form2)
{
        scsireq_t req;
        _img_private_t *_obj = user_data;
        char buf[M2RAW_SECTOR_SIZE] = { 0, };

        memset(&req, 0, sizeof(req));
        req.cmd[0] = 0xbe;
        req.cmd[1] = 0;
        req.cmd[2] = (lsn >> 24) & 0xff;
        req.cmd[3] = (lsn >> 16) & 0xff;
        req.cmd[4] = (lsn >> 8) & 0xff;
        req.cmd[5] = (lsn >> 0) & 0xff;
        req.cmd[6] = 0;
        req.cmd[7] = 0;
        req.cmd[8] = 1;
        req.cmd[9] = 0x58; /* subheader + userdata + ECC */
        req.cmdlen = 10;

        req.datalen = M2RAW_SECTOR_SIZE; 
        req.databuf = buf;
        req.timeout = 10000;
        req.flags = SCCMD_READ;

        if (ioctl(_obj->gen.fd, SCIOCCOMMAND, &req) < 0) {
                perror("SCIOCCOMMAND");
                return 1;
        }
        if (req.retsts != SCCMD_OK) {
                fprintf(stderr, "SCIOCCOMMAND cmd 0xbe sts %d\n", req.retsts);
                return 1;
        }

        if (mode2_form2)
                memcpy(data, buf, M2RAW_SECTOR_SIZE);
        else
                memcpy(data, buf + CDIO_CD_SUBHEADER_SIZE, CDIO_CD_FRAMESIZE);

        return 0;
}

static int
read_mode2_sectors_netbsd(void *user_data, void *data, lsn_t lsn, 
                          bool mode2_form2, unsigned int nblocks)
{
        int i, res;
        char *buf = data;

        for (i = 0; i < nblocks; i++) {
                res = read_mode2_sector_netbsd(user_data, buf, lsn, mode2_form2);
                if (res)
                        return res;

                buf += (mode2_form2 ? M2RAW_SECTOR_SIZE : CDIO_CD_FRAMESIZE);
                lsn++;
        }

        return 0;
}

static int
set_arg_netbsd(void *user_data, const char key[], const char value[])
{
        _img_private_t *_obj = user_data;

        if (!strcmp(key, "source")) {
                if (!value)
                        return -2;

                free(_obj->gen.source_name);
                _obj->gen.source_name = strdup(value);
        } else if (!strcmp(key, "access-mode")) {
                if (strcmp(value, "READ_CD"))
                        cdio_error("unknown access type: %s ignored.", value);
        } else 
                return -1;

        return 0;
}

static bool
_cdio_read_toc(_img_private_t *_obj) 
{
        int res;
        struct ioc_read_toc_entry req;

        res = ioctl(_obj->gen.fd, CDIOREADTOCHEADER, &_obj->tochdr);
        if (res < 0) {
                cdio_error("error in ioctl(CDIOREADTOCHEADER): %s\n",
                           strerror(errno));
                return false;
        }

        req.address_format = CD_MSF_FORMAT;
        req.starting_track = FIRST_TRACK_NUM;
        req.data_len = (TOTAL_TRACKS + 1) /* leadout! */
                * sizeof(struct cd_toc_entry); 
        req.data = _obj->tocent;

        res = ioctl(_obj->gen.fd, CDIOREADTOCENTRIES, &req);
        if (res < 0) {
                cdio_error("error in ioctl(CDROMREADTOCENTRIES): %s\n",
                           strerror(errno));
                return false;
        }

        _obj->toc_valid = 1;
        return true;
}

static bool
read_toc_netbsd (void *p_user_data) 
{

        return _cdio_read_toc(p_user_data);
}

static int
_cdio_read_discinfo(_img_private_t *_obj)
{
        scsireq_t req;
#define FULLTOCBUF (4 + 1000*11)
        unsigned char buf[FULLTOCBUF] = { 0, };
        int i, j;

        memset(&req, 0, sizeof(req));
        req.cmd[0] = 0x43; /* READ TOC/PMA/ATIP */
        req.cmd[1] = 0x02;
        req.cmd[2] = 0x02; /* full TOC */
        req.cmd[3] = 0;
        req.cmd[4] = 0;
        req.cmd[5] = 0;
        req.cmd[6] = 0;
        req.cmd[7] = FULLTOCBUF / 256;
        req.cmd[8] = FULLTOCBUF % 256;
        req.cmd[9] = 0;
        req.cmdlen = 10;

        req.datalen = FULLTOCBUF; 
        req.databuf = buf;
        req.timeout = 10000;
        req.flags = SCCMD_READ;

        if (ioctl(_obj->gen.fd, SCIOCCOMMAND, &req) < 0) {
                perror("SCIOCCOMMAND");
                return 1;
        }
        if (req.retsts != SCCMD_OK) {
                fprintf(stderr, "SCIOCCOMMAND cmd 0x43 sts %d\n", req.retsts);
                return 1;
        }
#if 1
        printf("discinfo:");
        for (i = 0; i < 4; i++)
                printf(" %02x", buf[i]);
        printf("\n");
        for (i = 0; i < buf[1] - 2; i++) {
                printf(" %02x", buf[i + 4]);
                if (!((i + 1) % 11))
                        printf("\n");
        }
#endif

        for (i = 4; i < req.datalen_used; i += 11) {
                if (buf[i + 3] == 0xa0) { /* POINT */
                        /* XXX: assume entry 0xa1 follows */
                        for (j = buf[i + 8] - 1; j <= buf[i + 11 + 8] - 1; j++)
                                _obj->sessionformat[j] = buf[i + 9];
                }
        }

        _obj->sessionformat_valid = true;
        return 0;
}

static int 
eject_media_netbsd(void *user_data) {

        _img_private_t *_obj = user_data;
        int fd, res, ret = 0;

        fd = open(_obj->gen.source_name, O_RDONLY|O_NONBLOCK);
        if (fd < 0)
                return 2;

        res = ioctl(fd, CDIOCALLOW);
        if (res < 0) {
                cdio_error("ioctl(fd, CDIOCALLOW) failed: %s\n",
                           strerror(errno));
                /* go on... */
        }
        res = ioctl(fd, CDIOCEJECT);
        if (res < 0) {
                cdio_error("ioctl(CDIOCEJECT) failed: %s\n",
                           strerror(errno));
                ret = 1;
        }

        close(fd);
        return ret;
}

/*!
  Return the value associated with the key "arg".
*/
static const char *
get_arg_netbsd(void *user_data, const char key[])
{
        _img_private_t *_obj = user_data;

        if (!strcmp(key, "source")) {
                return _obj->gen.source_name;
        } else if (!strcmp(key, "access-mode")) {
                return "READ_CD";
        } else if (!strcmp (key, "mmc-supported?")) {
            return "true" ;
        }

        return NULL;
}

static track_t
get_first_track_num_netbsd(void *user_data) 
{
        _img_private_t *_obj = user_data;
        int res;
  
        if (!_obj->toc_valid) {
                res = _cdio_read_toc(_obj);
                if (!res)
                        return CDIO_INVALID_TRACK;
        }

        return FIRST_TRACK_NUM;
}

static track_t
get_num_tracks_netbsd(void *user_data) 
{
        _img_private_t *_obj = user_data;
        int res;
  
        if (!_obj->toc_valid) {
                res = _cdio_read_toc(_obj);
                if (!res)
                        return CDIO_INVALID_TRACK;
        }

        return TOTAL_TRACKS;
}

/*!  
  Get format of track. 
*/
static track_format_t
get_track_format_netbsd(void *user_data, track_t track_num) 
{
        _img_private_t *_obj = user_data;
        int res;
  
        if (!_obj->toc_valid) {
                res = _cdio_read_toc(_obj);
                if (!res)
                        return CDIO_INVALID_TRACK;
        }

        if (track_num > TOTAL_TRACKS || track_num == 0)
                return TRACK_FORMAT_ERROR;

        if (_obj->tocent[track_num - 1].control & 0x04) {
                if (!_obj->sessionformat_valid) {
                        res = _cdio_read_discinfo(_obj);
                        if (res)
                                return CDIO_INVALID_TRACK;
                }

                if (_obj->sessionformat[track_num - 1] == 0x10)
                        return TRACK_FORMAT_CDI;
                else if (_obj->sessionformat[track_num - 1] == 0x20) 
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
get_track_green_netbsd(void *user_data, track_t track_num) 
{

        return (get_track_format_netbsd(user_data, track_num)
                == TRACK_FORMAT_XA);
}

/*!  
  Return the starting MSF (minutes/secs/frames) for track number
  track_num in obj.  Track numbers usually start at something 
  greater than 0, usually 1.

  The "leadout" track is specified either by
  using i_track LEADOUT_TRACK or the total tracks+1.
  False is returned if there is no track entry.
*/
static bool
get_track_msf_netbsd(void *user_data, track_t track_num, msf_t *msf)
{
        _img_private_t *_obj = user_data;
        int res;

        if (!msf)
                return false;

        if (!_obj->toc_valid) {
                res = _cdio_read_toc(_obj);
                if (!res)
                        return CDIO_INVALID_TRACK;
        }

        if (track_num == CDIO_CDROM_LEADOUT_TRACK)
                track_num = TOTAL_TRACKS + 1;

        if (track_num > TOTAL_TRACKS + 1 || track_num == 0)
                return false;

        msf->m = cdio_to_bcd8(_obj->tocent[track_num - 1].addr.msf.minute);
        msf->s = cdio_to_bcd8(_obj->tocent[track_num - 1].addr.msf.second);
        msf->f = cdio_to_bcd8(_obj->tocent[track_num - 1].addr.msf.frame);

        return true;
}

/*!
   Return the size of the CD in logical block address (LBA) units.
   @return the lsn. On error return CDIO_INVALID_LSN.

   Also note that in one at least one test the corresponding MMC gives
   a different answer, so there may be some disagreement about what is in
   fact the last lsn.
 */
static lsn_t
get_disc_last_lsn_netbsd(void *user_data) 
{
        msf_t msf;
  
        get_track_msf_netbsd(user_data, CDIO_CDROM_LEADOUT_TRACK, &msf);

        return (((msf.m * 60) + msf.s) * CDIO_CD_FRAMES_PER_SEC + msf.f);
}
#endif /* HAVE_NETBSD_CDROM */

char **
cdio_get_devices_netbsd (void)
{
#ifndef HAVE_NETBSD_CDROM
  return NULL;
#else
  return NULL;
#endif /* HAVE_NETBSD_CDROM */
}

/*!
  Return a string containing the default CD device.
 */
char *
cdio_get_default_device_netbsd()
{
  return strdup(DEFAULT_CDIO_DEVICE);
}

/*!
  Close tray on CD-ROM.
  
  @param psz_device the CD-ROM drive to be closed.
  
*/
driver_return_code_t 
close_tray_netbsd (const char *psz_device)
{
#ifdef HAVE_NETBSD_CDROM
  return DRIVER_OP_UNSUPPORTED;
#else
  return DRIVER_OP_NO_DRIVER;
#endif
}

#ifdef HAVE_NETBSD_CDROM
static cdio_funcs_t _funcs = {
  .audio_read_subchannel = audio_read_subchannel_mmc,
  .eject_media           = eject_media_netbsd,
  .free                  = cdio_generic_free,
  .get_arg               = get_arg_netbsd,
  .get_blocksize         = get_blocksize_mmc,
  .get_cdtext            = get_cdtext_generic,
  .get_cdtext_raw        = read_cdtext_generic,
  .get_default_device    = cdio_get_default_device_netbsd,
  .get_devices           = cdio_get_devices_netbsd,
  .get_disc_last_lsn     = get_disc_last_lsn_netbsd,
  .get_discmode          = get_discmode_generic,
  .get_drive_cap         = get_drive_cap_mmc,
  .get_first_track_num   = get_first_track_num_netbsd,
  .get_hwinfo            = NULL,
  .get_mcn               = get_mcn_mmc,
  .get_num_tracks        = get_num_tracks_netbsd,
  .get_track_channels    = get_track_channels_generic,
  .get_track_copy_permit = get_track_copy_permit_generic,
  .get_track_format      = get_track_format_netbsd,
  .get_track_green       = get_track_green_netbsd,
  .get_track_lba         = NULL, /* This could be implemented if need be. */
  .get_track_preemphasis = get_track_preemphasis_generic,
  .get_track_msf         = get_track_msf_netbsd,
  .get_track_isrc        = get_track_isrc_mmc,
  .lseek                 = cdio_generic_lseek,
  .read                  = cdio_generic_read,
  .read_audio_sectors    = read_audio_sectors_netbsd,
  .read_data_sectors     = read_data_sectors_generic,
  .read_mode2_sector     = read_mode2_sector_netbsd,
  .read_mode2_sectors    = read_mode2_sectors_netbsd,
  .read_toc              = read_toc_netbsd,
#if 1
  .run_mmc_cmd           = run_scsi_cmd_netbsd,
#endif
  .set_arg               = set_arg_netbsd,
};
#endif /*HAVE_NETBSD_CDROM*/

/*!
  Initialization routine. This is the only thing that doesn't
  get called via a function pointer. In fact *we* are the
  ones to set that up.
 */
CdIo_t *
cdio_open_netbsd(const char *source_name)
{
#ifdef HAVE_NETBSD_CDROM
    CdIo_t *ret;
    _img_private_t *_data;
    
    _data = calloc(1, sizeof(_img_private_t));

    _data->gen.init = false;
    _data->toc_valid = false;
    _data->sessionformat_valid = false;
    _data->gen.fd = -1;
    _data->gen.b_cdtext_error = false;
    
    set_arg_netbsd(_data, "source",
                   (source_name ? source_name : DEFAULT_CDIO_DEVICE));
    
    if (source_name && !cdio_is_device_generic(source_name))
        return (NULL);
    
    ret = cdio_new(&_data->gen, &_funcs);
    if (ret == NULL) return NULL;
    
    ret->driver_id = DRIVER_NETBSD;

    if (cdio_generic_init(_data, O_RDONLY)) {
        return ret;
    } else {
        cdio_generic_free(_data);
        free(ret);
        return NULL;
    }

#else 
  return NULL;
#endif /* HAVE_NETBSD_CDROM */

}

/*!
  Initialization routine. This is the only thing that doesn't
  get called via a function pointer. In fact *we* are the
  ones to set that up.
 */
CdIo_t *
cdio_open_am_netbsd(const char *source_name, const char *am)
{
  return (cdio_open_netbsd(source_name));
}

bool
cdio_have_netbsd (void)
{
#ifdef HAVE_NETBSD_CDROM
  return true;
#else 
  return false;
#endif /* HAVE_NETBSD_CDROM */
}


/* 
 * Local variables:
 *  c-file-style: "gnu"
 *  tab-width: 8
 *  indent-tabs-mode: nil
 * End:
 */
