/*
  Copyright (C) 2004, 2005, 2008-2012, 2017
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

/* Sample program to show use of the MMC interface.
   An optional drive name can be supplied as an argument.
   This basically the libdio mmc_get_hwinfo() routine.
   See also corresponding C++ programs.
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
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <cdio/cdio.h>
#include <cdio/mmc.h>
#include <cdio/mmc_cmds.h>

/* Set how long to wait for MMC commands to complete */
#define DEFAULT_TIMEOUT_MS 10000

int
main(int argc, const char *argv[])
{
  CdIo_t *p_cdio;
  const char *psz_drive = NULL;

  if (argc > 1) psz_drive = argv[1];
  p_cdio = cdio_open (psz_drive, DRIVER_UNKNOWN);

  if (!p_cdio) {
    printf("-- Couldn't find CD\n");
    return 77;
  } else {
    int i_status;             /* Result of MMC command */
    mmc_cdb_t cdb = {{0, }};  /* Command Descriptor Buffer */
    char buf[36] = { 0, };    /* Place to hold returned data */

    CDIO_MMC_SET_COMMAND(cdb.field, CDIO_MMC_GPCMD_INQUIRY);
    cdb.field[4] = sizeof(buf);

    i_status = mmc_run_cmd(p_cdio, DEFAULT_TIMEOUT_MS, &cdb,
			   SCSI_MMC_DATA_READ, sizeof(buf), &buf);
    if (i_status == 0) {
	char psz_vendor[CDIO_MMC_HW_VENDOR_LEN+1];
	char psz_model[CDIO_MMC_HW_MODEL_LEN+1];
	char psz_rev[CDIO_MMC_HW_REVISION_LEN+1];

	memcpy(psz_vendor, buf + 8, sizeof(psz_vendor)-1);
	psz_vendor[sizeof(psz_vendor)-1] = '\0';
	memcpy(psz_model,
	       buf + 8 + CDIO_MMC_HW_VENDOR_LEN,
	       sizeof(psz_model)-1);
	psz_model[sizeof(psz_model)-1] = '\0';
	memcpy(psz_rev,
	       buf + 8 + CDIO_MMC_HW_VENDOR_LEN +CDIO_MMC_HW_MODEL_LEN,
	       sizeof(psz_rev)-1);
	psz_rev[sizeof(psz_rev)-1] = '\0';

	printf("-- Vendor: %s\n-- Model: %s\n-- Revision: %s\n",
	       psz_vendor, psz_model, psz_rev);
    } else {
      printf("-- Couldn't get INQUIRY data (vendor, model, and revision).\n");
    }

    {
	bool b_erasable;
	cdio_mmc_feature_profile_t disctype;

	i_status = mmc_get_disc_erasable(p_cdio, &b_erasable);
	if (DRIVER_OP_SUCCESS == i_status)
	    printf("-- Disc is %serasable.\n", b_erasable ? "" : "not ");
	else
	    printf("-- Can't determine if disc is erasable.\n");

	i_status = mmc_get_disctype(p_cdio, 0, &disctype);
	if (DRIVER_OP_SUCCESS == i_status) {
	    printf("-- disc type: profile is %s (0x%X)\n",
		   mmc_feature_profile2str(disctype),
		   disctype);
	}
    }

  }

  cdio_destroy(p_cdio);

  return 0;
}
