//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл  BLASTER.C                                                            лл
//лл                                                                       лл
//лл  Demonstrates a technique for determining the address and type        лл
//лл  parameters of a Sound Blaster Pro adapter.  Relies on the BLASTER    лл
//лл  environment variable, which is normally inserted into the end        лл
//лл  user's AUTOEXEC.BAT file by the Sound Blaster installation program.  лл
//лл                                                                       лл
//лл-----------------------------------------------------------------------лл
//лл  Example usage:                                                       лл
//лл                                                                       лл
//лл  C:\>SET BLASTER=A220 I7 D1 T4                                        лл
//лл                                                                       лл
//лл  C:\>blaster                                                          лл
//лл    Sound Blaster type: Sound Blaster Pro (Yamaha YMF262/OPL3 version) лл
//лл           I/O address: 220H                                           лл
//лл                   IRQ: 7                                              лл
//лл                   DMA: 1                                              лл
//лл  Digital sound driver: SBPDIG.ADV                                     лл
//лл       FM sound driver: SBP2FM.ADV                                     лл
//лл                                                                       лл
//лл  C:\>                                                                 лл
//лл-----------------------------------------------------------------------лл
//лл                                                                       лл
//лл  V1.00 of 20-Apr-92                                                   лл
//лл                                                                       лл
//лл  Project: IBM Audio Interface Library                                 лл
//лл   Author: John Miles                                                  лл
//лл                                                                       лл
//лл  C source compatible with Turbo C++ v1.0 or later                     лл
//лл                                                                       лл
//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл  blaster.obj: blaster.c ail.h                                         лл
//лл     bcc -ml -c -v blaster.c                                           лл
//лл                                                                       лл
//лл  blaster.exe: blaster.obj ail.obj                                     лл
//лл     tlink @blaster.lls                                                лл
//лл                                                                       лл
//лл  Contents of BLASTER.LLS:                                             лл
//лл    /c /v /x +                                                         лл
//лл    \bc\lib\c0l.obj +                                                  лл
//лл    blaster ail, +                                                     лл
//лл    blaster.exe, +                                                     лл
//лл    blaster.map, +                                                     лл
//лл    \bc\lib\cl.lib                                                     лл
//лл                                                                       лл
//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл  Copyright (C) 1992 Miles Design, Inc.                                лл
//лл                                                                       лл
//лл  Miles Design, Inc.                                                   лл
//лл  10926 Jollyville #308                                                лл
//лл  Austin, TX 78759                                                     лл
//лл  (512) 345-2642 / FAX (512) 338-9630 / BBS (512) 454-9990             лл
//лл                                                                       лл
//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

#include "ail.h"        // Audio Interface Library API function header

/*************************************************************/
//
// board_ID(): Reads the BLASTER environment variable to determine the type
// and I/O port settings of the user's Sound Blaster adapter.
//
// Returns the names of the FM and digital sound drivers which should be
// used to communicate with the board, as well as an AIL drvr_desc structure
// which contains the correct I/O parameters for the board.
//
// If board_ID() returns 0, no BLASTER environment variable was found.
// Either the user has not installed the Sound Blaster in accordance with
// Creative Labs' instructions, or no Sound Blaster is installed.
//
// You can call board_ID() twice: once to get the name of the driver to
// load, and a second time to insert the correct I/O parameters into the
// drvr_desc structure returned by the driver's AIL_describe_driver()
// function.
//
/*************************************************************/

int board_ID(drvr_desc *desc, char *FM_driver_name,
   char *DSP_driver_name, char *board_name)
{
   int i,j,k,m,d,num,p;
   char *env;
   char string[128];
   static drvr_desc t;
   static int type;
   static char tokens[] =
      {
      "AIDT"
      };
   static int base[] =
      {
      16,10,10,10
      };
   static int *targets[] =
      {
         &t.default_IO,&t.default_IRQ,&t.default_DMA,&type
      };
   static char *FM_driver_names[] =
      {
         "SBFM.ADV",
         "SBP1FM.ADV",
         "SBFM.ADV",
         "SBP2FM.ADV",
      };
   static char *DSP_driver_names[] =
      {
         "SBDIG.ADV",
         "SBPDIG.ADV",
         "SBDIG.ADV",
         "SBPDIG.ADV",
      };
   static char *board_names[] =
      {
         "Sound Blaster V1.5 or earlier",
         "Sound Blaster Pro (Yamaha YM3812 version)",
         "Sound Blaster V2.0",
         "Sound Blaster Pro (Yamaha YMF262/OPL3 version)",
      };

   
   env = getenv("BLASTER");
   if (env==NULL) return 0;

   strncpy(string,env,127);
   if (!strlen(string)) return 0;

   strupr(string);
   t = *desc;

   for (m=0;m<strlen(string);m++)
      {
      if ((m != 0) && (string[m] != ' ')) continue;

      m += (string[m] == ' '); k = string[m];

      for (i=0;i<4;i++)
         if (k==tokens[i])
            {
            p = m + 1;
            num = 0;

            do
               {
               d = string[p++];

               for (j=0;j<base[i];j++)
                  if (toupper(d) == "0123456789ABCDEF"[j])
                     num = (num * base[i]) + j;
               }
            while (isalnum(d));

            *targets[i] = num;
            break;
            }
      }
                     
   *desc = t;

   if (!type) return 0;

   if (type > 4) type=4;

   if (FM_driver_name != NULL)
      strcpy(FM_driver_name,FM_driver_names[type-1]);

   if (DSP_driver_name != NULL)
      strcpy(DSP_driver_name,DSP_driver_names[type-1]);

   if (board_name != NULL)
      strcpy(board_name,board_names[type-1]);

   return type;
}

/*************************************************************/
void main(void)
{
   static drvr_desc blank = {0,0,{0,0,0,0},NULL,0,0,0,0,0,0};
   char DSP_drvr[16];
   char FM_drvr[16];
   char board[64];

   if (!board_ID(&blank,FM_drvr,DSP_drvr,board))
      {
      printf("Sound Blaster not installed, or BLASTER environment\n");
      printf("variable not set.\n");
      exit(1);
      }

   printf("  Sound Blaster type: %s\n",board);
   printf("         I/O address: %XH\n",blank.default_IO);
   printf("                 IRQ: %u\n",blank.default_IRQ);
   printf("                 DMA: %u\n",blank.default_DMA);
   printf("Digital sound driver: %s\n",DSP_drvr);
   printf("     FM sound driver: %s\n",FM_drvr);
}

