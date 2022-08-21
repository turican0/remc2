//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   STPLAY.C                                                            лл
//лл                                                                       лл
//лл   Creative Voice File (.VOC) stereo performance                       лл
//лл   utility with dual-buffer playback                                   лл
//лл                                                                       лл
//лл   V2.00 of 09-Oct-91                                                  лл
//лл   V2.01 of 14-Dec-91: Pause/resume added                              лл
//лл   V2.02 of 09-Apr-92: Buffer preformatting added                      лл
//лл                                                                       лл
//лл   Project: IBM Audio Interface Library                                лл
//лл    Author: John Miles                                                 лл
//лл                                                                       лл
//лл   C source compatible with Turbo C++ v1.0 or later                    лл
//лл                                                                       лл
//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   stplay.obj: stplay.c gen.h ail.h                                    лл
//лл      bcc -ml -c -v stplay.c                                           лл
//лл                                                                       лл
//лл   stplay.exe: stplay.obj gen.lib ail.obj                              лл
//лл      tlink @stplay.lls                                                лл
//лл                                                                       лл
//лл   Contents of STPLAY.LLS:                                             лл
//лл     /c /v /x +                                                        лл
//лл     \bc\lib\c0l.obj +                                                 лл
//лл     stplay ail, +                                                     лл
//лл     stplay.exe, +                                                     лл
//лл     stplay.map, +                                                     лл
//лл     \bc\lib\cl.lib gen.lib                                            лл
//лл                                                                       лл
//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   Copyright (C) 1991, 1992 Miles Design, Inc.                         лл
//лл                                                                       лл
//лл   Miles Design, Inc.                                                  лл
//лл   10926 Jollyville #308                                               лл
//лл   Austin, TX 78759                                                    лл
//лл   (512) 345-2642 / FAX (512) 338-9630 / BBS (512) 454-9990            лл
//лл                                                                       лл
//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <alloc.h>
#include <io.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>

#include "ail.h"        // Audio Interface Library API header file
#include "gen.h"        // General DOS and system functions

const char VERSION[] = "2.01";

char far buf1[16384], far buf2[16384];

/*****************************************************************/
void main(int argc, char *argv[])
{
   HDRIVER hdriver;
   FILE *vfile;
   char far *drvr;
   drvr_desc far *desc;
   sound_buff firstblock,tempblock;
   int i,done,bufnum,paused;

   printf("\nSTPLAY version %s                 Copyright (C) 1991, 1992 Miles Design, Inc.\n",VERSION);
   printf("-------------------------------------------------------------------------------\n\n");

   if (argc != 3)
      {
      printf("This program plays a raw 8-bit stereo sample at 22 kHz\n");
      printf("through any Audio Interface Library digital stereo driver.\n\n");
      printf("Usage: STPLAY ST_filename driver_filename\n");
      exit(1);
      }

   AIL_startup();

   //
   // Load, install, describe, and initialize the Sound Blaster-
   // compatible driver
   //

   drvr = load_driver(argv[2]);
   if (drvr == NULL)
      {
      printf("Couldn't load driver.\n");
      AIL_shutdown(NULL);
      exit(1);
      }

   hdriver = AIL_register_driver(drvr);
   if (hdriver==-1)
      {
      printf("Driver %s not compatible with linked API version.\n",
         argv[2]);
      AIL_shutdown(NULL);
      exit(1);
      }

   desc = AIL_describe_driver(hdriver);

   if (desc->drvr_type != DSP_DRVR)
      {
      printf("%s is not a digital sound driver.\n",argv[2]);
      AIL_shutdown(NULL);
      exit(1);
      }

   if (!AIL_detect_device(hdriver,desc->default_IO,desc->default_IRQ,
      desc->default_DMA,desc->default_DRQ))
         {
         printf("Sound hardware not found.\n");
         AIL_shutdown(NULL);
         exit(1);
         }

   AIL_init_driver(hdriver,desc->default_IO,desc->default_IRQ,
      desc->default_DMA,desc->default_DRQ);

   vfile = fopen(argv[1],"rb");
   if (vfile == NULL)
      {
      printf("Couldn't open %s.\n",argv[1]);
      AIL_shutdown(NULL);
      exit(1);
      }

   //
   // Play the block as a series of double-buffered 16K chunks
   //

   printf("U D to increase/decrease volume\n");
   printf("P R to pause/resume playback\n");
   printf("< > to pan left, right\n");
   printf("ESC to stop playback\n\n");

   //
   // Copy sample rate and packing type to working sound buffer 
   // structure
   //
   // Sound Blaster standard sample rate: 256-(1000000-freq in Hz.)
   //

   firstblock.sample_rate = 234;     // 22.050 kHz (stereo 44.1 kHz)
   firstblock.pack_type = 0 | 0x80;  // 8-bit stereo sample
   firstblock.len = filelength(fileno(vfile));
   bufnum = 0;

   tempblock = firstblock;

   //
   // Sample application main loop ...
   //

   done = paused = 0;
   do
      {

      //
      // (Application-specific events here)
      //

      if (kbhit())                                 
         {
         switch (toupper(getch()))
            {
            case 'P':
               AIL_pause_digital_playback(hdriver);
               paused = 1;
               break;

            case 'R':
               AIL_resume_digital_playback(hdriver);
               paused = 0;
               break;

            case 'U':
               i = AIL_digital_playback_volume(hdriver);
               i+=10;
               if (i>127) i=127;
               AIL_set_digital_playback_volume(hdriver,i);
               break;

            case 'D':
               i = AIL_digital_playback_volume(hdriver);
               i-=10;
               if (i<0) i=0;
               AIL_set_digital_playback_volume(hdriver,i);
               break;

            case ',':
            case '<':
               i = AIL_digital_playback_panpot(hdriver);
               i+=10;
               if (i>127) i=127;
               AIL_set_digital_playback_panpot(hdriver,i);
               break;

            case '.':
            case '>':
               i = AIL_digital_playback_panpot(hdriver);
               i-=10;
               if (i<0) i=0;
               AIL_set_digital_playback_panpot(hdriver,i);
               break;

            case 27:
               done=1;
               break;
            }
         }


      //
      // Update sound DMA buffers and ensure sound output is active
      //                          

      if (!paused)
         {
         for (i=0;i<2;i++)
            if ((AIL_sound_buffer_status(hdriver,i) == DAC_DONE)
               && firstblock.len)
               {
               tempblock.len = min(16384L,firstblock.len);
               firstblock.len -= tempblock.len;
               if (!(bufnum ^= 1))
                  {
                  fread(buf1,(unsigned) tempblock.len,1,vfile);
                  tempblock.data = buf1;
                  }
               else
                  {
                  fread(buf2,(unsigned) tempblock.len,1,vfile);
                  tempblock.data = buf2;
                  }

               AIL_register_sound_buffer(hdriver,i,&tempblock);
               AIL_format_sound_buffer(hdriver,&tempblock);
               printf(".");
               }
         AIL_start_digital_playback(hdriver);
         }

      //
      // Playback ends when no bytes are left in the source data and
      // the status of both buffers equals DAC_DONE
      //

      if (!firstblock.len)
         if ((AIL_sound_buffer_status(hdriver,0) == DAC_DONE)
            && (AIL_sound_buffer_status(hdriver,1) == DAC_DONE))
               done = 1;
      }
   while (!done);

   while (kbhit()) getch();

   fclose(vfile);
   printf("\n\nSTPLAY stopped.\n");
   AIL_shutdown(NULL);
}

