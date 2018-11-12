/*
   Copyright (C) 2010, 2012 Rocky Bernstein <rocky@gnu.org>

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

#ifndef CDIO_DRIVER_MMC_CMD_HELPER_H_
#define CDIO_DRIVER_MMC_CMD_HELPER_H_

/* Boilerplate initialization code to setup running MMC command.  We
   assume variables 'p_cdio', 'p_buf', and 'i_size' are previously
   defined.  It does the following:

   1. Defines a cdb variable, 
   2  Checks to see if we have a cdio object and can run an MMC command
   3. zeros the buffer (p_buf) using i_size.
   4. Sets up the command field of cdb to passed in value mmc_cmd.
*/
#define MMC_CMD_SETUP(mmc_cmd)                                          \
    mmc_cdb_t cdb = {{0, }};                                            \
                                                                        \
    if ( ! p_cdio ) return DRIVER_OP_UNINIT;                            \
    if ( ! p_cdio->op.run_mmc_cmd ) return DRIVER_OP_UNSUPPORTED;       \
                                                                        \
    CDIO_MMC_SET_COMMAND(cdb.field, mmc_cmd)                            

/* Boilerplate initialization code to setup running MMC read command
   needs to set the cdb 16-bit length field. See above
   comment for MMC_CMD_SETUP.
*/
#define MMC_CMD_SETUP_READ16(mmc_cmd)                                   \
    MMC_CMD_SETUP(mmc_cmd);                                             \
                                                                        \
    /* Setup to read header, to get length of data */                   \
    CDIO_MMC_SET_READ_LENGTH16(cdb.field, i_size)

/* Boilerplate code to run a MMC command. 

   We assume variables 'p_cdio', 'mmc_timeout_ms', 'cdb', 'i_size' and
   'p_buf' are defined previously.

   'direction' is the SCSI direction (read, write, none) of the
   command.  
*/
#define MMC_RUN_CMD(direction, i_timeout)                               \
    p_cdio->op.run_mmc_cmd(p_cdio->env,                                 \
        i_timeout,                                                      \
        mmc_get_cmd_len(cdb.field[0]),                                  \
        &cdb,                                                           \
        direction, i_size, p_buf)

#endif /* CDIO_DRIVER_MMC_CMD_HELPER_H_ */
