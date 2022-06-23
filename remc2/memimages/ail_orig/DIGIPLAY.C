//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   DIGIPLAY.C                                                          лл
//лл                                                                       лл
//лл   Creative Voice File (.VOC) performance utility                      лл
//лл   with dual-buffer playback                                           лл
//лл                                                                       лл
//лл   Project: IBM Audio Interface Library                                лл
//лл    Author: John Miles                                                 лл
//лл                                                                       лл
//лл   V2.00 of 09-Oct-91                                                  лл
//лл   V2.01 of 09-Apr-92: Preformatting call added                        лл
//лл                                                                       лл
//лл   C source compatible with Turbo C++ v1.0 or later                    лл
//лл                                                                       лл
//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   digiplay.obj: digiplay.c gen.h ail.h                                лл
//лл      bcc -ml -c -v digiplay.c                                         лл
//лл                                                                       лл
//лл   digiplay.exe: digiplay.obj gen.lib ail.obj                          лл
//лл      tlink @digiplay.lls                                              лл
//лл                                                                       лл
//лл   Contents of DIGIPLAY.LLS:                                           лл
//лл     /c /v /x +                                                        лл
//лл     \bc\lib\c0l.obj +                                                 лл
//лл     digiplay ail, +                                                   лл
//лл     digiplay.exe, +                                                   лл
//лл     digiplay.map, +                                                   лл
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
#include <conio.h>

#include "ail.h"        // Audio Interface Library API header file
#include "gen.h"        // General DOS and system functions

const char VERSION[] = "2.01";

/***********************************************************/
void main(int argc, char *argv[])
{
   HDRIVER hdriver;
   char far *vfile;
   char far *drvr;
   drvr_desc *desc;
   sound_buff firstblock,tempblock;
   int i,done;

   printf("\nDIGIPLAY version %s               Copyright (C) 1991, 1992 Miles Design, Inc.\n",VERSION);
   printf("-------------------------------------------------------------------------------\n\n");

   if (argc != 3)
      {
      printf("This program plays the first Voice Block from a Creative Voice File (.VOC)\n");
      printf("through any Audio Interface Library digital driver.\n\n");
      printf("Usage: DIGIPLAY VOC_filename driver_filename\n");
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

   //
   // Read the entire .VOC file into memory
   //
   // (Not necessary when double-buffering, but done for simplicity)
   //

   vfile = read_file(argv[1],NULL);
   if (vfile == NULL)
      {
      printf("Couldn't load %s.\n",argv[1]);
      AIL_shutdown(NULL);
      exit(1);
      }

   //
   // Get the address and size of the first Voice Data block in the
   // target .VOC file
   //

   AIL_index_VOC_block(hdriver,vfile,-1,&firstblock);

   //
   // Play the block as a series of double-buffered 16K chunks
   //
   // For this example, we disregard the fact that the entire file is
   // in memory, and simulate the use of registered sound buffers to 
   // maintain continuous sound output as if the buffered chunks were
   // actually loaded on demand from disk or extra memory
   //

   printf("Press any key to stop playback ");

   //
   // Copy sample rate and packing type to working sound buffer 
   // structure
   //

   tempblock = firstblock;

   //
   // Sample application main loop ...
   //

   done = 0;
   do
      {

      //
      // (Application-specific events here)
      //

      //
      // Update sound DMA buffers and ensure sound output is active
      //                          

      for (i=0;i<2;i++)
         if ((AIL_sound_buffer_status(hdriver,i) == DAC_DONE)
            && firstblock.len)
            {
            tempblock.data = firstblock.data;
            tempblock.len = min(16384L,firstblock.len);
            firstblock.len -= tempblock.len;
            firstblock.data = (void far *) ((char huge *)
               firstblock.data + tempblock.len);
            AIL_format_sound_buffer(hdriver,&tempblock);
            AIL_register_sound_buffer(hdriver,i,&tempblock);
            printf(".");
            }
      AIL_start_digital_playback(hdriver);

      if (kbhit()) done = 1;

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

   printf("\n\nDIGIPLAY stopped.\n");
   AIL_shutdown(NULL);
}

