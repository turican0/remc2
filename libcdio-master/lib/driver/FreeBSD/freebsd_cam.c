/*
  Copyright (C) 2004, 2005, 2008, 2009, 2010, 2012
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
   control of the CD drive via SCSI emulation.
*/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

static const char _rcsid[] = "$Id: freebsd_cam.c,v 1.12 2008/04/21 18:30:20 karl Exp $";

#ifdef HAVE_FREEBSD_CDROM

#include "freebsd.h"
#include <cdio/mmc.h>

/* Default value in seconds we will wait for a command to 
   complete. */
#define DEFAULT_TIMEOUT_MSECS 10000

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

  Return 0 if no error.
 */
int
run_mmc_cmd_freebsd_cam( void *p_user_data, unsigned int i_timeout_ms,
			 unsigned int i_cdb, const mmc_cdb_t *p_cdb, 
			 cdio_mmc_direction_t e_direction, 
			 unsigned int i_buf, /*in/out*/ void *p_buf )
{
  _img_private_t *p_env = p_user_data;
  int   i_status, sense_size;
  int direction = CAM_DEV_QFRZDIS | CAM_PASS_ERR_RECOVER;
  union ccb ccb;

  p_env->gen.scsi_mmc_sense_valid = 0;
  if (!p_env || !p_env->cam) return -2;
    
  memset(&ccb, 0, sizeof(ccb));

  ccb.ccb_h.path_id    = p_env->cam->path_id;
  ccb.ccb_h.target_id  = p_env->cam->target_id;
  ccb.ccb_h.target_lun = p_env->cam->target_lun;
  ccb.ccb_h.timeout    = i_timeout_ms;

  if (SCSI_MMC_DATA_NONE == e_direction)
    i_buf = 0;
  if (!i_buf)
    direction |= CAM_DIR_NONE;
  else
    direction |= (e_direction == SCSI_MMC_DATA_READ)?CAM_DIR_IN : CAM_DIR_OUT;

 
   memcpy(ccb.csio.cdb_io.cdb_bytes, p_cdb->field, i_cdb);
   ccb.csio.cdb_len =
     mmc_get_cmd_len(ccb.csio.cdb_io.cdb_bytes[0]);
   
  cam_fill_csio (&(ccb.csio), 1, NULL, 
		 direction | CAM_DEV_QFRZDIS, MSG_SIMPLE_Q_TAG, p_buf, i_buf, 
 		 sizeof(ccb.csio.sense_data), ccb.csio.cdb_len, 30*1000);

  if (cam_send_ccb(p_env->cam, &ccb) < 0)
    {
      cdio_warn ("transport failed: %s", strerror(errno));
      return -1;
    }
  if ((ccb.ccb_h.status & CAM_STATUS_MASK) == CAM_REQ_CMP)
    {
      return 0;
    }

  /* Record SCSI sense reply for API call mmc_last_cmd_sense().
  */
  sense_size = ccb.csio.sense_len;
  if (sense_size > sizeof(p_env->gen.scsi_mmc_sense))
    sense_size = sizeof(p_env->gen.scsi_mmc_sense);
  memcpy((void *) p_env->gen.scsi_mmc_sense, &ccb.csio.sense_data, sense_size);
  p_env->gen.scsi_mmc_sense_valid = sense_size;

  errno = EIO;
  i_status = ERRCODE(((unsigned char *)&ccb.csio.sense_data));
  if (i_status == 0)
    i_status = -1;
  else
    CREAM_ON_ERRNO(((unsigned char *)&ccb.csio.sense_data));

/* There are many harmless or intentional reasons why to get an SCSI
   error condition. Higher levels should decide whether this is an incident
   or just a normal outcome.

  cdio_warn ("scsi error condition detected : 0x%X", i_status);
*/
  return i_status;
}

bool
init_freebsd_cam (_img_private_t *p_env)
{
  char pass[100];
  
  p_env->cam=NULL;
  memset (&p_env->ccb, 0, sizeof(p_env->ccb));
  p_env->ccb.ccb_h.func_code = XPT_GDEVLIST;

  if (-1 == p_env->gen.fd) 
    p_env->gen.fd = open (p_env->device, O_RDONLY, 0);

  if (p_env->gen.fd < 0)
    {
      cdio_warn ("open (%s): %s", p_env->device, strerror (errno));
      return false;
    }

  if (ioctl(p_env->gen.fd, CDIOCALLOW) == -1) {
      cdio_warn("ioctl(fd, CDIOCALLOW) failed: %s\n", strerror(errno));
  }
  if (ioctl (p_env->gen.fd, CAMGETPASSTHRU, &p_env->ccb) < 0)
    {
      cdio_warn ("open: %s", strerror (errno));
      return false;
    }
  sprintf (pass,"/dev/%.15s%u",
	   p_env->ccb.cgdl.periph_name,
	   p_env->ccb.cgdl.unit_number);
  p_env->cam = cam_open_pass (pass,O_RDWR,NULL);
  if (!p_env->cam) return false;
  
  p_env->gen.init   = true;
  p_env->b_cam_init = true;
  return true;
}

void
free_freebsd_cam (void *user_data)
{
  _img_private_t *p_env = user_data;

  if (NULL == p_env) return;

  if (p_env->gen.fd > 0)
    close (p_env->gen.fd);
  p_env->gen.fd = -1;

  if(p_env->cam)
    cam_close_device(p_env->cam);

  free (p_env);
}

driver_return_code_t
read_mode2_sector_freebsd_cam (_img_private_t *p_env, void *data, lsn_t lsn, 
			       bool b_form2)
{
  if ( b_form2 )
    return read_mode2_sectors_freebsd_cam(p_env, data, lsn, 1);
  else {
    /* Need to pick out the data portion from a mode2 form2 frame */
    char buf[M2RAW_SECTOR_SIZE] = { 0, };
    int retval = read_mode2_sectors_freebsd_cam(p_env, buf, lsn, 1);
    if ( retval ) return retval;
    memcpy (((char *)data), buf + CDIO_CD_SUBHEADER_SIZE, CDIO_CD_FRAMESIZE);
    return DRIVER_OP_SUCCESS;
  }
}

/*!
   Reads nblocks of mode2 sectors from cd device into data starting
   from lsn.
   Returns 0 if no error. 
 */
int
read_mode2_sectors_freebsd_cam (_img_private_t *p_env, void *p_buf, 
				lsn_t lsn, unsigned int nblocks)
{
  mmc_cdb_t cdb = {{0, }};

  bool b_read_10 = false;

  CDIO_MMC_SET_READ_LBA(cdb.field, lsn);
  
  if (b_read_10) {
    int retval;
    
    CDIO_MMC_SET_COMMAND(cdb.field, CDIO_MMC_GPCMD_READ_10);
    CDIO_MMC_SET_READ_LENGTH16(cdb.field, nblocks);
    if ((retval = mmc_set_blocksize (p_env->gen.cdio, M2RAW_SECTOR_SIZE)))
      return retval;
    
    if ((retval = run_mmc_cmd_freebsd_cam (p_env, 0, 
					   mmc_get_cmd_len(cdb.field[0]),
					   &cdb, 
					   SCSI_MMC_DATA_READ,
					   M2RAW_SECTOR_SIZE * nblocks, 
					   p_buf)))
      {
	mmc_set_blocksize (p_env->gen.cdio, CDIO_CD_FRAMESIZE);
	return retval;
      }
    
    return mmc_set_blocksize (p_env->gen.cdio, CDIO_CD_FRAMESIZE);
  } else {
    CDIO_MMC_SET_COMMAND(cdb.field, CDIO_MMC_GPCMD_READ_CD);
    CDIO_MMC_SET_READ_LENGTH24(cdb.field, nblocks);
    cdb.field[1] = 0; /* sector size mode2 */
    cdb.field[9] = 0x58; /* 2336 mode2 */
    return run_mmc_cmd_freebsd_cam (p_env, 0, 
				    mmc_get_cmd_len(cdb.field[0]), 
				    &cdb, 
				    SCSI_MMC_DATA_READ,
				    M2RAW_SECTOR_SIZE * nblocks, p_buf);
    
  }
}

/*!
  Eject media in CD-ROM drive. Return DRIVER_OP_SUCCESS if successful, 
  DRIVER_OP_ERROR on error.
 */
driver_return_code_t
eject_media_freebsd_cam (_img_private_t *p_env) 
{
  int i_status;
  mmc_cdb_t cdb = {{0, }};
  uint8_t buf[1];
  
  CDIO_MMC_SET_COMMAND(cdb.field, CDIO_MMC_GPCMD_ALLOW_MEDIUM_REMOVAL);

  i_status = run_mmc_cmd_freebsd_cam (p_env, DEFAULT_TIMEOUT_MSECS,
				      mmc_get_cmd_len(cdb.field[0]), 
				      &cdb, SCSI_MMC_DATA_WRITE, 0, &buf);
  if (i_status) return i_status;
  
  CDIO_MMC_SET_COMMAND(cdb.field, CDIO_MMC_GPCMD_START_STOP);
  cdb.field[4] = 1;
  i_status = run_mmc_cmd_freebsd_cam (p_env, DEFAULT_TIMEOUT_MSECS,
				 mmc_get_cmd_len(cdb.field[0]), &cdb, 
				 SCSI_MMC_DATA_WRITE, 0, &buf);
  if (i_status) return i_status;
  
  CDIO_MMC_SET_COMMAND(cdb.field, CDIO_MMC_GPCMD_START_STOP);
  cdb.field[4] = 2; /* eject */

  return run_mmc_cmd_freebsd_cam (p_env, DEFAULT_TIMEOUT_MSECS,
				  mmc_get_cmd_len(cdb.field[0]), 
				  &cdb, 
				  SCSI_MMC_DATA_WRITE, 0, &buf);
}



/* This is a CAM based device enumerator.
   Currently its only purpose is to eventually obtain the info needed for
    cdio_get_arg("scsi-tuple")
*/
/* Stemming from code in libburn/sg-freebsd.c ,
   originally contributed by Alexander Nedotsukov <bland@FreeBSD.org>,
   without copyright claim to libburn in October 2006.
   Contributed by libburn and adapted to libcdio without copyright claim
   in January 2010.
*/

struct burn_drive_enumeration_state {
  int fd;
  union ccb ccb;
  unsigned int i;
  int skip_device;
};
typedef struct burn_drive_enumeration_state *burn_drive_enumerator_t;


/* Some helper functions for scsi_give_next_adr() */

static int sg_init_enumerator(burn_drive_enumerator_t *idx_)
{
  struct burn_drive_enumeration_state *idx;
  int bufsize;

  idx = malloc(sizeof(*idx));
  if (idx == NULL) {
    cdio_warn("cannot malloc memory for CAM based drive enumerator");
    return -1;
  }
  idx->skip_device = 0;

  if ((idx->fd = open(XPT_DEVICE, O_RDWR)) == -1) {
    cdio_warn("could not open %s (errno = %d  \"%s\")",
              XPT_DEVICE, errno, strerror(errno));
    free(idx);
    idx = NULL;
    return -1;
  }

  memset(&(idx->ccb), 0, sizeof(union ccb));

  idx->ccb.ccb_h.path_id = CAM_XPT_PATH_ID;
  idx->ccb.ccb_h.target_id = CAM_TARGET_WILDCARD;
  idx->ccb.ccb_h.target_lun = CAM_LUN_WILDCARD;

  idx->ccb.ccb_h.func_code = XPT_DEV_MATCH;
  bufsize = sizeof(struct dev_match_result) * 100;
  idx->ccb.cdm.match_buf_len = bufsize;
  idx->ccb.cdm.matches = (struct dev_match_result *)malloc(bufsize);
  if (idx->ccb.cdm.matches == NULL) {
    cdio_warn("cannot malloc memory for CAM enumerator matches");
    close(idx->fd);
    free(idx);
    return -1;
  }
  idx->ccb.cdm.num_matches = 0;
  idx->i = idx->ccb.cdm.num_matches; /* to trigger buffer load */

  /*
   * We fetch all nodes, since we display most of them in the default
   * case, and all in the verbose case.
   */
  idx->ccb.cdm.num_patterns = 0;
  idx->ccb.cdm.pattern_buf_len = 0;

  *idx_ = idx;

  return 1; 
}


static void sg_destroy_enumerator(burn_drive_enumerator_t *idx_)
{
  struct burn_drive_enumeration_state *idx = *idx_;

  if(idx->fd != -1)
    close(idx->fd);

  free(idx->ccb.cdm.matches);
  free(idx);

  *idx_ = NULL;
}


static int sg_next_enumeration_buffer(burn_drive_enumerator_t *idx_)
{
  struct burn_drive_enumeration_state *idx = *idx_;

  /*
   * We do the ioctl multiple times if necessary, in case there are
   * more than 100 nodes in the EDT.
   */
  if (ioctl(idx->fd, CAMIOCOMMAND, &(idx->ccb)) == -1) {
    cdio_warn("error sending CAMIOCOMMAND ioctl, (errno = %d  \"%s\")",
              errno, strerror(errno));
    return -1;
  }

  if ((idx->ccb.ccb_h.status != CAM_REQ_CMP)
      || ((idx->ccb.cdm.status != CAM_DEV_MATCH_LAST)
           && (idx->ccb.cdm.status != CAM_DEV_MATCH_MORE))) {
    cdio_warn("got CAM error %#x, CDM error %d\n",
              idx->ccb.ccb_h.status, idx->ccb.cdm.status);
    return -1;
  }
  return 1;
}


/** Returns the next index object state and the next enumerated drive address.
    @param idx An opaque handle. Make no own theories about it.
    @param adr Takes the reply
    @param adr_size Gives maximum size of reply including final 0
    @param initialize  1 = start new,
     0 = continue, use no other values for now
    -1 = finish
    @return 1 = reply is a valid address , 0 = no further address available
     -1 = severe error (e.g. adr_size too small)
*/
/* This would be the public interface of the enumerator.
   In libcdio it is private for now.
*/
static
int give_next_adr_freebsd_cam(burn_drive_enumerator_t *idx_,
                              char adr[], int adr_size, int initialize)
{
  struct burn_drive_enumeration_state *idx;
  int ret;

  if (initialize == 1) {
    ret = sg_init_enumerator(idx_);
    if (ret<=0)
      return ret;
  } else if (initialize == -1) {
    sg_destroy_enumerator(idx_);
    return 0;
  }
    
  idx = *idx_;

  do {
    if (idx->i >= idx->ccb.cdm.num_matches) { 
      ret = sg_next_enumeration_buffer(idx_);
      if (ret<=0)
        return -1;
      idx->i = 0;
    } else
      (idx->i)++;

    while (idx->i < idx->ccb.cdm.num_matches) {
      switch (idx->ccb.cdm.matches[idx->i].type) {
      case DEV_MATCH_BUS:
        break;
      case DEV_MATCH_DEVICE: {
        struct device_match_result* result;

        result = &(idx->ccb.cdm.matches[idx->i].result.device_result);
        if (result->flags & DEV_RESULT_UNCONFIGURED)
          idx->skip_device = 1;
        else
          idx->skip_device = 0;
        break;
      }
      case DEV_MATCH_PERIPH: {
        struct periph_match_result* result;

        result = &(idx->ccb.cdm.matches[idx->i].result.periph_result);

        /* A specialized CD drive enumerator would have to test for
             strcmp(result->periph_name, "cd") != 0
           rather than
             strcmp(result->periph_name, "pass") == 0
        */
        if (idx->skip_device ||
            strcmp(result->periph_name, "pass") == 0)
          break;

       ret = snprintf(adr, adr_size, "/dev/%s%d",
                       result->periph_name, result->unit_number);
        if(ret >= adr_size)
          return -1;

        /* Found next enumerable address */
        return 1;

      }
      default:
        /* fprintf(stderr, "unknown match type\n"); */
        break;
      }
      (idx->i)++;
    }
  } while ((idx->ccb.ccb_h.status == CAM_REQ_CMP)
           && (idx->ccb.cdm.status == CAM_DEV_MATCH_MORE));

  return 0;
}


/** Try to obtain SCSI address tuple of path.
    @return  1 is success , 0 is failure
*/
int obtain_scsi_adr_freebsd_cam(char *path,
                                int *bus_no, int *host_no, int *channel_no,
                                int *target_no, int *lun_no)
{
  burn_drive_enumerator_t idx;
  int ret;
  char buf[64];
  struct periph_match_result* result;

  ret = sg_init_enumerator(&idx);
  if (ret <= 0)
          return 0;
  while(1) {
    ret = give_next_adr_freebsd_cam(&idx, buf, sizeof(buf), 0);
    if (ret <= 0)
      break;
    if (strcmp(path, buf) == 0) {
      result = &(idx->ccb.cdm.matches[idx->i].result.periph_result);
      *bus_no = result->path_id;
      *host_no = result->path_id;
      *channel_no = 0;
      *target_no = result->target_id;
      *lun_no = result->target_lun;
      sg_destroy_enumerator(&idx);
      return 1;
    }
  }
  sg_destroy_enumerator(&idx);
  return (0);
}

#endif /* HAVE_FREEBSD_CDROM */
