//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   MIXDEMO.C                                                           лл
//лл                                                                       лл
//лл   XMIDI multiple-sequence sound effects demo                          лл
//лл                                                                       лл
//лл   V1.00 of 09-Nov-91                                                  лл
//лл   V1.01 of 12-Dec-91: New timbre request structure                    лл
//лл   V1.02 of  3-Dec-92: BC 3.0 const declarations fixed                 лл
//лл   V1.03 of  4-Jul-92: Check GTL handle before closing                 лл
//лл                                                                       лл
//лл   Project: IBM Audio Interface Library                                лл
//лл    Author: John Miles                                                 лл
//лл                                                                       лл
//лл   C source compatible with Turbo C++ v1.0 or later                    лл
//лл                                                                       лл
//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   demo.xmi: backgnd.mid shanty.mid choral.mid                         лл
//лл     midiform demo.xmi backgnd.mid shanty.mid choral.mid               лл
//лл                                                                       лл
//лл   mixdemo.obj: mixdemo.c gen.h ail.h                                  лл
//лл     bcc -ml -c -v mixdemo.c                                           лл
//лл                                                                       лл
//лл   mixdemo.exe: mixdemo.obj gen.lib ail.obj                            лл
//лл     tlink @mixdemo.lls                                                лл
//лл                                                                       лл
//лл   Contents of MIXDEMO.LLS:                                            лл
//лл     /c /v /x +                                                        лл
//лл     \bc\lib\c0l.obj +                                                 лл
//лл     mixdemo ail, +                                                    лл
//лл     mixdemo.exe, +                                                    лл
//лл     mixdemo.map, +                                                    лл
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

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <alloc.h>
#include <string.h>

#include "ail.h"        // Audio Interface Library API function header
#include "gen.h"        // General DOS and system functions

const char VERSION[] = "1.03";

char seq_fn[] = "DEMO.XMI";       // name of XMIDI sequence file

/***************************************************************/

//
// Standard C routine for Global Timbre Library access
//

void far *load_global_timbre(FILE *GTL, unsigned bank, unsigned patch)
{
   unsigned far *timb_ptr;
   static unsigned len;

   static struct                  // GTL file header entry structure
   {
      char patch;
      char bank;
      unsigned long offset;
   }
   GTL_hdr;

   if (GTL==NULL) return NULL;    // if no GTL, return failure

   rewind(GTL);                   // else rewind to GTL header

   do                             // search file for requested timbre
      {
      fread(&GTL_hdr,sizeof(GTL_hdr),1,GTL);    
      if (GTL_hdr.bank == -1) 
         return NULL;             // timbre not found, return NULL
      }
   while ((GTL_hdr.bank != bank) ||
          (GTL_hdr.patch != patch));       

   fseek(GTL,GTL_hdr.offset,SEEK_SET);    
   fread(&len,2,1,GTL);           // timbre found, read its length

   timb_ptr = farmalloc(len);     // allocate memory for timbre ..
   *timb_ptr = len;         
                                  // and load it
   fread((timb_ptr+1),len-2,1,GTL);       
                           
   if (ferror(GTL))               // return NULL if any errors
      return NULL;                // occurred
   else
      return timb_ptr;            // else return pointer to timbre
}
   
/***************************************************************/
void main(int argc, char *argv[])
{
   HDRIVER hdriver;
   HSEQUENCE hseq[8];
   char far *state[8];
   char GTL_filename[32];
   unsigned char far *buffer;
   unsigned long state_size;
   drvr_desc far *desc;
   char far *drvr;
   char far *timb;
   int seqnum;
   FILE *GTL;
   unsigned i,ch,tc_size;
   char far *tc_addr;
   unsigned treq;

   printf("\nMIXDEMO version %s                Copyright (C) 1991, 1992 Miles Design, Inc.\n",VERSION);
   printf("-------------------------------------------------------------------------------\n\n");

   if (argc != 2)
      {
      printf("This program demonstrates the Audio Interface Library's ");
      printf("multiple-sequence\nExtended MIDI playback features.\n\n");
      printf("Usage: MIXDEMO driver_filename\n");
      exit(1);
      }

   //
   // Load driver file at seg:0000
   //

   drvr = load_driver(argv[1]); 
   if (drvr==NULL)     
      {
      printf("Driver %s not found\n",argv[1]);
      exit(1);
      }

   //
   // Initialize API before calling any Library functions
   //

   AIL_startup();

   //
   // Register the driver with the API
   //

   hdriver = AIL_register_driver(drvr);
   if (hdriver==-1)
      {
      printf("Driver %s not compatible with linked API version.\n",
         argv[1]);
      AIL_shutdown(NULL);
      exit(1);
      }

   //
   // Get driver type and factory default I/O parameters; exit if
   // driver is not capable of interpreting MIDI files
   //

   desc = AIL_describe_driver(hdriver);

   if (desc->drvr_type != XMIDI_DRVR)
      {
      printf("Driver %s not an XMIDI driver.\n",argv[1]);
      AIL_shutdown(NULL);
      exit(1);
      }

   //
   // Verify presence of driver's sound hardware and prepare 
   // driver/hardware for use
   //

   if (!AIL_detect_device(hdriver,desc->default_IO,desc->default_IRQ,
      desc->default_DMA,desc->default_DRQ))
         {
         printf("Sound hardware not found.\n");
         AIL_shutdown(NULL);
         exit(1);
         }

   AIL_init_driver(hdriver,desc->default_IO,desc->default_IRQ,
      desc->default_DMA,desc->default_DRQ);

   state_size = AIL_state_table_size(hdriver);

   //
   // Load XMIDI data file
   //

   buffer = read_file(seq_fn,NULL);
   if (buffer == NULL)
      {
      printf("Can't load XMIDI file %s.\n",seq_fn);
      AIL_shutdown(NULL);
      exit(1);
      }

   //
   // Get name of Global Timbre Library file by appending suffix 
   // supplied by XMIDI driver to GTL filename prefix "SAMPLE."
   //

   strcpy(GTL_filename,"SAMPLE.");
   strcat(GTL_filename,desc->data_suffix);

   //
   // Set up local timbre cache; open Global Timbre Library file
   //

   tc_size = AIL_default_timbre_cache_size(hdriver);
   if (tc_size)
      {
      tc_addr = farmalloc((unsigned long) tc_size);
      AIL_define_timbre_cache(hdriver,tc_addr,tc_size);
      }

   GTL = fopen(GTL_filename,"rb");

   //
   // Register all sequences in XMIDI file (up to 8 allowed), loading
   // new timbres as needed
   //

   for (i=0;i<8;i++)
      {
      state[i] = farmalloc(state_size);
      if ((hseq[i] = AIL_register_sequence(hdriver,buffer,i,state[i],
         NULL)) == -1)
         {
         farfree(state[i]);
         break;
         }

      while ((treq=AIL_timbre_request(hdriver,hseq[i])) != 0xffff)
         {
         timb = load_global_timbre(GTL,treq/256,treq%256);
         if (timb != NULL)
            {
            AIL_install_timbre(hdriver,treq/256,treq%256,timb);
            farfree(timb);
            }
         else
            {
            printf("Timbre bank %u, patch %u not found ",
               treq/256,treq%256);
            printf("in Global Timbre Library %s\n",GTL_filename);
            AIL_shutdown(NULL);
            exit(1);
            }
         }
      }

   if (GTL != NULL) fclose(GTL);         
   printf("Sequences 0-%u registered.\n\n",i-1);

   //
   // Show menu, start sequences at user's request
   //

   printf("Options: [1] to start main sequence (BACKGND.MID)\n");
   printf("         [2] to start sequence #1   (SHANTY.MID)\n");
   printf("         [3] to start sequence #2   (CHORAL.MID)\n");
   printf("       [ESC] to quit\n\n");

   while ((ch=getch()) != 27)
      {
      switch (ch)
         {
         case '1':
         case '2':
         case '3':
            i = ch - '1';
            AIL_start_sequence(hdriver,hseq[i]);
            break;
         }
      }

   //
   // Shut down API and all installed drivers; write signoff message
   // to any front-panel displays
   //

   printf("MIXDEMO stopped.\n");
   AIL_shutdown("MIXDEMO stopped.");
}
