//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   XPLAY.C                                                             лл
//лл                                                                       лл
//лл   Standard XMIDI file performance utility                             лл
//лл                                                                       лл
//лл   V1.00 of 23-Oct-91                                                  лл
//лл   V1.01 of 12-Dec-91: New timbre request structure                    лл
//лл   V1.02 of 20-Dec-91: Register requested sequence only                лл
//лл   V1.03 of  4-Jul-92: Check GTL handle before closing                 лл
//лл                                                                       лл
//лл   Project: IBM Audio Interface Library                                лл
//лл    Author: John Miles                                                 лл
//лл                                                                       лл
//лл   C source compatible with Turbo C++ v1.0 or later                    лл
//лл                                                                       лл
//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   xplay.obj: xplay.c gen.h ail.h                                      лл
//лл      bcc -ml -c -v xplay.c                                            лл
//лл                                                                       лл
//лл   xplay.exe: xplay.obj gen.lib ail.obj                                лл
//лл      tlink @xplay.lls                                                 лл
//лл                                                                       лл
//лл   Contents of XPLAY.LLS:                                              лл
//лл     /c /v /x +                                                        лл
//лл     \bc\lib\c0l.obj +                                                 лл
//лл     xplay ail, +                                                      лл
//лл     xplay.exe, +                                                      лл
//лл     xplay.map, +                                                      лл
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
#include <string.h>

#include "ail.h"        // Audio Interface Library API function header
#include "gen.h"        // General DOS and system functions

const char VERSION[] = "1.03";

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
   HSEQUENCE hseq;
   drvr_desc far *desc;
   FILE *GTL;
   char GTL_filename[32];
   char far *state;
   char far *drvr;
   char far *timb;
   char far *tc_addr;
   unsigned char far *buffer;
   unsigned long state_size;
   unsigned bank,patch,tc_size,seqnum,treq;
   
   if (!strcmp((char far *) 0x000004f0,"XPLAY"))
      {
      printf("You must type 'EXIT' before re-starting XPLAY.\n");
      exit(1);
      }

   printf("\nXPLAY version %s                  Copyright (C) 1991, 1992 Miles Design, Inc.\n",VERSION);
   printf("-------------------------------------------------------------------------------\n\n");

   if (argc < 3)
      {
      printf("This program plays an Extended MIDI (XMIDI) sequence through a \n");
      printf("specified Audio Interface Library V2.0 sound driver.\n\n");
      printf("Usage: XPLAY XMIDI_filename driver_filename [sequence_number]\n");
      exit(1);
      }

   seqnum = 0;
   if (argc == 4) seqnum = val(argv[3],10);

   //
   // Load driver file at seg:0000
   //

   drvr = load_driver(argv[2]); 
   if (drvr==NULL)     
      {
      printf("Driver %s not found\n",argv[2]);
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
         argv[2]);
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
      printf("Driver %s not an XMIDI driver.\n",argv[2]);
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

   buffer = read_file(argv[1],NULL);
   if (buffer == NULL)
      {
      printf("Can't load XMIDI file %s.\n",argv[1]);
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
   // Look up and register desired sequence in XMIDI file, loading
   // timbres if needed
   //

   state = farmalloc(state_size);
   if ((hseq = AIL_register_sequence(hdriver,buffer,seqnum,state,
      NULL)) == -1)
      {
      printf("Sequence %u not present in XMIDI file \"%s\".\n",
         seqnum,argv[1]);
      AIL_shutdown(NULL);
      exit(1);
      }

   while ((treq=AIL_timbre_request(hdriver,hseq)) != 0xffff)
      {
      bank = treq / 256; patch = treq % 256;
                         
      timb = load_global_timbre(GTL,bank,patch);
      if (timb != NULL)
         {
         AIL_install_timbre(hdriver,bank,patch,timb);
         farfree(timb);
         printf("Installed timbre bank %u, patch %u\n",bank,patch);
         }
      else
         {
         printf("Timbre bank %u, patch %u not found ",bank,patch);
         printf("in Global Timbre Library %s\n",GTL_filename);
         AIL_shutdown(NULL);
         exit(1);
         }
      }

   if (GTL != NULL) fclose(GTL);         

   //
   // Start music playback and set flag to prevent user from 
   // launching multiple copies of XPLAY from the DOS shell
   //

   printf("Playing sequence %u from XMIDI file \"%s\" ...\n\n",
      seqnum,argv[1]);

   AIL_start_sequence(hdriver,hseq);

   strcpy((char far *) 0x000004f0,"XPLAY");

   printf("Launching DOS shell.  Type 'EXIT' to stop playback ");
   printf("and exit XPLAY.");
   spawnlp(P_WAIT,"command.com",NULL);

   //
   // Shut down API and all installed drivers; write XMIDI filename 
   // to any front-panel displays
   //

   strcpy((char far *) 0x000004f0,"        ");
   printf("XPLAY stopped.\n");
   AIL_shutdown(argv[1]);
}
