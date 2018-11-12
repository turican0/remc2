/* -*- C -*-
  Copyright (C) 2009 Thomas Schmitt <scdbackup@gmx.net>
  Copyright (C) 2010-2013 Rocky Bernstein <rocky@gnu.org>

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

/**
   Regression test for MMC commands which don't involve write access.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#define __CDIO_CONFIG_H__ 1
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#if !defined(HAVE_SLEEP)
#  if defined(HAVE_USLEEP)
#     define sleep(s) usleep(1000000*s)
#  elif defined(_WIN32)
#     include <windows.h>
#     define sleep(s) Sleep(1000*s)
#  else
#     define sleep(s) { int i; for(i=0; i<=1000*s; i++); }
#  endif
#endif

#include <cdio/cdio.h>
#include <cdio/logging.h>
#include <cdio/mmc_cmds.h>

#define SKIP_TEST 77

/* gcc may warn if no prototypes are given before function definition */

static int handle_outcome(CdIo_t *p_cdio, driver_return_code_t i_status,
			  int *pi_sense_avail,
			  cdio_mmc_request_sense_t * p_sense_reply,
			  unsigned int i_flag);

static int get_disctype(CdIo_t *p_cdio, bool b_verbose);

static driver_return_code_t get_disc_erasable(const CdIo_t *p_cdio,
					      const char *psz_source,
					      bool verbose);

static int mode_sense(CdIo_t *p_cdio, int *pi_sense_avail,
		      cdio_mmc_request_sense_t *p_sense_reply,
		      unsigned int page_code, unsigned int subpage_code,
		      int i_alloc_len,
		      unsigned char *p_buf, int *pi_size,
		      unsigned int i_flag);

static void print_status_sense(int i_status, int i_sense_valid,
			       cdio_mmc_request_sense_t *p_sense_reply,
			       unsigned int i_flag);

static int test_read(char *psz_drive_path, unsigned int i_flag);

static int test_unit_ready(CdIo_t *p_cdio, int *pi_sense_avail,
			   cdio_mmc_request_sense_t *p_sense_reply,
			   unsigned int i_flag);

static int wait_for_drive(CdIo_t *p_cdio, unsigned int max_tries, bool b_verbose);


/* ------------------------- Helper functions ---------------------------- */

static driver_return_code_t
get_disc_erasable(const CdIo_t *p_cdio, const char *psz_source,
		       bool b_verbose)
{
    driver_return_code_t i_status;
    bool b_erasable;

    i_status = mmc_get_disc_erasable(p_cdio, &b_erasable);
    if (b_verbose && DRIVER_OP_SUCCESS == i_status)
	printf("Disc is %serasable.\n", b_erasable ? "" : "not ");
    return i_status;
}

static int
get_disctype(CdIo_t *p_cdio, bool b_verbose)
{
    cdio_mmc_feature_profile_t disctype;
    driver_return_code_t i_status = mmc_get_disctype(p_cdio, 0, &disctype);
    if (DRIVER_OP_SUCCESS == i_status) {
	if (b_verbose)
	    fprintf(stderr, "test_disctype: profile is %s (0x%X)\n",
		    mmc_feature_profile2str(disctype),
		    disctype);
    }
    return DRIVER_OP_SUCCESS;
}


/**
    @param flag         bit0= verbose
*/
static int
handle_outcome(CdIo_t *p_cdio, driver_return_code_t i_status,
	       int *pi_sense_avail,
	       cdio_mmc_request_sense_t * p_sense_reply,
	       unsigned int i_flag)
{
    cdio_mmc_request_sense_t *p_temp_sense_reply = NULL;
    *pi_sense_avail = mmc_last_cmd_sense(p_cdio, &p_temp_sense_reply);
    print_status_sense(i_status, *pi_sense_avail, p_temp_sense_reply, i_flag & 1);
    if (18 <= *pi_sense_avail)
	memcpy(p_sense_reply, p_temp_sense_reply, sizeof(cdio_mmc_request_sense_t));
    else
	memset(p_sense_reply, 0, sizeof(cdio_mmc_request_sense_t));
    cdio_free(p_temp_sense_reply);
    return i_status;
}


/**
    @param i_flag bit0= verbose
*/
static void
print_status_sense(int i_status, int i_sense_valid,
		   cdio_mmc_request_sense_t *p_sense_reply,
		   unsigned int i_flag)
{
    if (!(i_flag & 1))
	return;
    printf("return= %d , sense(%d)", i_status, i_sense_valid);
    if (i_sense_valid >= 14)
	printf(":  KEY=%s (%1.1X), ASC= %2.2X , ASCQ= %2.2X",
		mmc_sense_key2str[p_sense_reply->sense_key],
		p_sense_reply->sense_key,
		p_sense_reply->asc,
		p_sense_reply->ascq);
    printf("\n");
}

/* --------------------------- MMC commands ------------------------------ */


/**
   @param flag         bit0= verbose
   @param sense_avail  Number of available sense bytes
   (18 get copied if all 18 exist)

   @param sense_reply  eventual sense bytes
   @return             return value of mmc_run_cmd()
 */
static int
test_unit_ready(CdIo_t *p_cdio,
		int *pi_sense_avail,
		cdio_mmc_request_sense_t *p_sense_reply,
		unsigned int i_flag)
{
  int i_status;
  int old_log_level = cdio_loglevel_default;

  cdio_loglevel_default = CDIO_LOG_WARN;

  if (i_flag & 1)
    printf("test_unit_ready ... ");

  i_status = mmc_test_unit_ready(p_cdio, 0);
  cdio_loglevel_default = old_log_level;

  return handle_outcome(p_cdio, i_status, pi_sense_avail, p_sense_reply,
                             i_flag & 1);
}


/* BARELY OBTRUSIVE: MIGHT RUIN A SIMULTANEOUS BURNING OPERATION ON THE DRIVE */
/**
   Fetch a mode page or a part of it from the drive.
   @param i_alloc_len  The number of bytes to be requested from the drive and to
                       be copied into parameter buf.
                       This has to include the 8 bytes of header and may not
                       be less than 10.
   @param p_buf        Will contain at most alloc_len many bytes. The first 8 are
                       a Mode Parameter Header as of SPC-3 7.4.3, table 240.
                       The further bytes are the mode page, typically as of
                       MMC-5 7.2. There are meanwhile deprecated mode pages which
                       appear only in older versions of MMC.
   @param i_size       Will return the number of actually read bytes resp. the
                       number of available bytes. See flag bit1.
   @param i_flag       bit0= verbose
                       bit1= Peek mode:
                           Reply number of available bytes in *i_size and not
                           the number of actually read bytes.
   @return             return value of mmc_run_cmd(),
                       or other driver_return_code_t
*/
static driver_return_code_t
mode_sense(CdIo_t *p_cdio, int *pi_sense_avail,
		cdio_mmc_request_sense_t *p_sense_reply,
                unsigned int i_page_code, unsigned int subpage_code, int i_alloc_len,
                unsigned char *p_buf, int *pi_size, unsigned int i_flag)
{
  driver_return_code_t i_status;

  if (i_alloc_len < 10)
    return DRIVER_OP_BAD_PARAMETER;

  if (i_flag & 1)
    printf("mode_sense(0x%X, %X, %d) ... ",
	   i_page_code, subpage_code, i_alloc_len);

  i_status = mmc_mode_sense_10(p_cdio, p_buf, i_alloc_len, i_page_code);
  handle_outcome(p_cdio, i_status, pi_sense_avail, p_sense_reply, i_flag & 1);
  if (DRIVER_OP_SUCCESS != i_status)
    return i_status;
  if (i_flag & 2)
    *pi_size = p_buf[9] + 10;                  /* MMC-5 7.2.3 */
  else
    *pi_size = p_buf[0] * 256 + p_buf[1] + 2;  /* SPC-3 7.4.3, table 240 */
  return i_status;
}

/**
   Watch drive by test unit ready loop until ready, no media or timeout.
   @param b_verbose verbose
   @return     1= unit ready , 0= error , -1= severe failure, 2 = no media
*/
static int
wait_for_drive(CdIo_t *p_cdio, unsigned int i_max_tries, bool b_verbose)
{
  int i_ret, i, i_sense_avail;
  cdio_mmc_request_sense_t sense_reply;

  memset(&sense_reply, 0, sizeof(sense_reply));
  for (i = 0; i < i_max_tries; i++) {
    i_ret = test_unit_ready(p_cdio, &i_sense_avail, &sense_reply, b_verbose);
    if (i_ret == 0) /* Unit is ready */
      return 1;
    if (i_sense_avail < 18)
      return -1;
    if (2 == sense_reply.sense_key && 0x04 == sense_reply.asc) {

      /* Not ready */;

    } else if (6 == sense_reply.sense_key && 0x28 == sense_reply.asc &&
               0 == sense_reply.ascq) {

      /* Media change notice = try again */;

    } else if (2 == sense_reply.sense_key && 0x3a == sense_reply.asc) {
      /* Medium not present */;
        return 2;
    } else if (0 == sense_reply.sense_key && 0 == sense_reply.asc) {

      /* Error with no sense */;

      return -1;
  break;
    } else {

      /* Other error */;

      return 0;
    }
    sleep(1);
  }
  fprintf(stderr, "wait_for_drive: Drive not ready after %d retries\n",
          i_max_tries);
  return -1;
}

/**
   This function bundles tests for the read capability to perform MMC
   commands and detecting the sense reply of MMC commands, which
   indicates error causes or important drive events.

   @param drive_path  a drive address suitable for cdio_open_am()
   @param flag        bit0= verbose
   @return            0= no severe failure
                      else an proposal for an exit() value is returned
*/
static int
test_read(char *psz_drive_path, unsigned int i_flag)
{
    int sense_avail = 0, i_ret, i_sense_valid, i_size, alloc_len = 10;
    bool b_verbose = !!(i_flag & 1);
    int old_log_level = cdio_loglevel_default;
    cdio_mmc_request_sense_t sense_reply;
    unsigned char buf[10];
    CdIo_t *p_cdio;
    const char *scsi_tuple;


    p_cdio = cdio_open(psz_drive_path, DRIVER_DEVICE);
    if (!p_cdio)
	i_ret = SKIP_TEST - 16;

    /* The further code produces some intentional failures which should not be
       reported by mmc_run_cmd() as INFO messages.
    */
    cdio_loglevel_default = CDIO_LOG_WARN;

    /* Test availability of info for cdrecord style adresses .
     */
    scsi_tuple = cdio_get_arg(p_cdio, "scsi-tuple");
    if (scsi_tuple == NULL) {
	fprintf(stderr, "Error: cdio_get_arg(\"scsi-tuple\") returns NULL.\n");
	i_ret = 6; goto ex;
    } else if (i_flag & 1)
	printf("Drive '%s' has cdio_get_arg(\"scsi-tuple\") = '%s'\n",
	       psz_drive_path, scsi_tuple);

    /* Test availability of sense reply in case of unready drive.
       E.g. if the tray is already ejected.
    */
    i_ret = test_unit_ready(p_cdio, &sense_avail, &sense_reply, b_verbose);
    if (i_ret != 0 && sense_avail < 18) {
	fprintf(stderr,
		"Error: Drive not ready. Only %d sense bytes. Expected >= 18.\n",
		sense_avail);
	i_ret = 2; goto ex;
    }

    /* Cause sense reply failure by requesting inappropriate mode page 3Eh */
    i_ret = mode_sense(p_cdio, &sense_avail, &sense_reply,
			    0x3e, 0, alloc_len, buf, &i_size, b_verbose);
    if (i_ret != 0 && sense_avail < 18) {

	fprintf(stderr,
		"Error: Deliberately illegal command yields only %d sense bytes. Expected >= 18.\n",
		sense_avail);
	i_ret = 3; goto ex;
    } else  {
	if (sense_reply.sense_key != 5)  {
	    fprintf(stderr,
		    "Error: Sense key should be 5, got %d\n",
		    sense_reply.sense_key);
	    i_ret = 3; goto ex;
	} else if (sense_reply.asc != 0x24)  {
	    fprintf(stderr,
		    "Error: Sense code should be 24, got %d\n",
		    sense_reply.asc);
	    i_ret = 4; goto ex;
	} else if (sense_reply.ascq != 0)  {
	    fprintf(stderr,
		    "Error: Sense code should be 24, got %d\n",
		    sense_reply.ascq);
	    i_ret = 4; goto ex;
	}
    }

    /* How are we, finally ? */
    i_ret = test_unit_ready(p_cdio, &i_sense_valid, &sense_reply, b_verbose);
    if ((i_flag & 1) && 0 != i_ret && 2 == sense_reply.sense_key &&
	0x3a == sense_reply.asc)
	fprintf(stderr, "test_unit_ready: Note: No loaded media detected.\n");

    /* Call mmc_read with a null pointer and check that we get
       the right return code. */

    if (DRIVER_OP_BAD_POINTER !=
	(i_ret = mmc_read_cd(p_cdio,
			     NULL, /* wrong! should be a buffer. */
			     200,  CDIO_MMC_READ_TYPE_ANY,
			     false, false, 0, true, false, false,
			     1, 2448, 1))) {
	fprintf(stderr,
		"mmc_read_cd: expecting bad pointer return, got %d\n",
		i_ret);
    } else {
	    i_ret = 0;
    }

  ex:;
    cdio_loglevel_default = old_log_level;
    cdio_destroy(p_cdio);
    return i_ret;
}

/* ---------------------------      main       ----------------------------- */

int
main(int argc, const char *argv[])
{
  CdIo_t *p_cdio;
  char **ppsz_drives     = NULL;
  const char *psz_source = NULL;
  int i_ret;
  int exitrc = 0;
  bool b_verbose = (argc > 1);
  driver_return_code_t i_status;

  cdio_loglevel_default = b_verbose ? CDIO_LOG_DEBUG : CDIO_LOG_WARN;

  if (0 !=
      strncmp("TEST UNIT READY", mmc_cmd2str(CDIO_MMC_GPCMD_TEST_UNIT_READY),
	      sizeof("TEST UNIT READY"))) {
      printf("Expecting mmc_cmd2str to give 'TEST UNIT READY' for code 0x%x\n",
	     CDIO_MMC_GPCMD_TEST_UNIT_READY);
      exit(1);
  }

  ppsz_drives = cdio_get_devices(DRIVER_DEVICE);
  if (!ppsz_drives) {
    printf("Can't find a CD-ROM drive. Skipping test.\n");
    exit(SKIP_TEST);
  }

  p_cdio = cdio_open(ppsz_drives[0], DRIVER_DEVICE);

  if (p_cdio) {
    const char *psz_have_mmc = cdio_get_arg(p_cdio, "mmc-supported?");

    psz_source = cdio_get_arg(p_cdio, "source");
    if (0 != strncmp(psz_source, ppsz_drives[0],
                     strlen(ppsz_drives[0]))) {
      fprintf(stderr,
              "Got %s; should get back %s, the name we opened.\n",
              psz_source, ppsz_drives[0]);
      exit(1);
    }

    i_ret = wait_for_drive(p_cdio, 30, b_verbose);
    if (0 >= i_ret) {
	fprintf(stderr, "Wait for drive error\n");
	exit(2);
    } else {
	if (1 == i_ret)  {
	    i_status = get_disc_erasable(p_cdio, psz_source, b_verbose);
	    if (DRIVER_OP_SUCCESS != i_status) {
		printf("Got status %d back from get_disc_erasable(%s)\n",
		       i_status, psz_source);
	    }
	    i_status = get_disctype(p_cdio, b_verbose);

	    if ( psz_have_mmc
		 && 0 == strncmp("true", psz_have_mmc, sizeof("true")) ) {

		/* Test the MMC enhancements of version 0.83 in december 2009 */
		i_ret = test_read(ppsz_drives[0],
				  cdio_loglevel_default == CDIO_LOG_DEBUG);
		if (0 != i_ret) exit(i_ret + 16);
	    }
	} else if (2 == i_ret && b_verbose)
	    printf("Drive is empty... skipping remaining tests.\n");
    }
    cdio_destroy(p_cdio);
  } else {
    fprintf(stderr, "cdio_open('%s') failed\n", ppsz_drives[0]);
    exit(2);
  }

  cdio_free_device_list(ppsz_drives);

  return exitrc;
}
