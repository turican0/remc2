//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   MIDIREC.C                                                           лл
//лл                                                                       лл
//лл   MIDI Format 0 file recorder                                         лл
//лл                                                                       лл
//лл   V1.00 of 04-Sep-91                                                  лл
//лл   V1.10 of 23-Oct-91: SMF sysex recording format                      лл
//лл                                                                       лл
//лл   Project: IBM Audio Interface Library                                лл
//лл    Author: John Miles                                                 лл
//лл                                                                       лл
//лл   C source compatible with Turbo C++ v1.0 or later                    лл
//лл                                                                       лл
//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   midirec.obj: midirec.c gen.h ail.h                                  лл
//лл      bcc -ml -c -v midirec.c                                          лл
//лл                                                                       лл
//лл   midirec.exe: midirec.obj gen.lib ail.obj                            лл
//лл      tlink @midirec.lls                                               лл
//лл                                                                       лл
//лл   Contents of MIDIREC.LLS:                                            лл
//лл     /c /v /x +                                                        лл
//лл     \bc\lib\c0l.obj +                                                 лл
//лл     midirec ail, +                                                    лл
//лл     midirec.exe, +                                                    лл
//лл     midirec.map, +                                                    лл
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
#include <io.h>
#include <conio.h>

#include "gen.h"        // General DOS and system functions
#include "ail.h"        // Include AIL API for timer resources

const char VERSION[] = "1.10";

unsigned DATA_IO;       // base addr
unsigned STAT_COM;      // base addr+1
unsigned IRQ;           // MPU-401 IRQ

#define QLEN 16384
unsigned char far queue[QLEN];
unsigned far deltas[QLEN];
unsigned head,tail;     // incoming MIDI message buffer & pointers

unsigned orig_PIC;
void interrupt (*default_isr)();

HTIMER timer;
FILE *out;

unsigned event_type[256];
unsigned data_bytes[256];
unsigned long delta_time;

unsigned char MIDI_header[] = {'M','T','h','d',0,0,0,6,
   0,0,0,1,0,120,             // Format 0, one track, 120 ticks/quarter note
   'M','T','r','k',0,0,0,15,  // 1st track, initial chunk len = 15
   0,0xff,0x58,4,4,2,0x18,8,  // Time signature: 4/4
   0,0xff,0x51,0x03,0x07,0xa1,0x20};   // Tempo: 500K uS/quarter note

unsigned char end_of_track[] = {0x00,0xff,0x2f,0x00};

typedef struct 
{
   unsigned byte;
   unsigned delta;
}
inbyte;

/***************************************************************/
unsigned send_cmd(unsigned cmd)
{
   unsigned i;

   disable();

   i = 65535;
   while (--i)
      if (!(inportb(STAT_COM) & 0x40)) break;

   if (!i)
      {
      enable();
      return 0;
      }

   outportb(STAT_COM,cmd);

   for (i=65535;i;i--)
      {
      if (inportb(STAT_COM) & 0x80) continue;
      if (inportb(DATA_IO) == 0xfe)
         {
         enable();
         return 1;
         }
      }
             
   enable();
   return 0;
}

/***************************************************************/
void send_byte(unsigned b)
{
   unsigned i;

   i = 65535;
   while (--i)
      if (!(inportb(STAT_COM) & 0x40)) break;

   if (!i)
      return;

   outportb(DATA_IO,b);
}

/***************************************************************/
void init_queue(void)
{
   head = 0;
   tail = 0;
}

unsigned check_queue(void)
{
   return (head!=tail);
}

inbyte read_queue(void)
{
   inbyte temp;
              
   temp.byte = queue[tail];
   temp.delta = deltas[tail];

   ++tail;
   if (tail==QLEN) tail=0;

   return temp;
}

int watch_queue(void)
{
   unsigned n;

   n = queue[tail];

   return n;
}

void write_queue(unsigned n)
{
   queue[head] = n;
   deltas[head] = (delta_time > 65535L) ? 65535:(unsigned) delta_time;
   delta_time = 0L;

   ++head;
   if (head==QLEN) head=0;
}

/***************************************************************/
void interrupt MPU_isr(void)
{
   unsigned in;

   while (!(inportb(STAT_COM) & 0x80))
      {
      in = inportb(DATA_IO);

      send_byte(in);

      if ((in == 0xf0) || (in == 0xf7) || (in < 0xf0))
         write_queue(in);
      }

   outportb(0x20,0x20);
}

/***************************************************************/
void far delta_update(void)
{
   ++delta_time;
}

/***************************************************************/
void write_VLN(unsigned dtime)
{
   int i,n;
   unsigned char bytefield[3];

   bytefield[2] = (dtime & 0x7f);
   bytefield[1] = ((dtime >> 7) & 0x7f) | 0x80;
   bytefield[0] = ((dtime >> 14) & 0x7f) | 0x80;
   
   n=2;
   for (i=0;i<=2;i++)
      if (bytefield[i] & 0x7f)
         {
         n=i;
         break;
         }

   for (i=n;i<=2;i++)
      fputc(bytefield[i],out);
}

void quit(void)
{
   setvect(IRQ+8,default_isr);
   outportb(0x21,orig_PIC);
   fclose(out);
   send_cmd(0xff);
   AIL_shutdown(NULL);
}

/***************************************************************/
void main(int argc, char *argv[])
{
   char outfn[64];
   int argma,argmi;
   int bad,strcnt;
   int i;
   inbyte in;
   unsigned status;
   unsigned expect_bytes;
   unsigned long accum_delta;
   unsigned sysex_active;
   unsigned long flen;
   static unsigned char sysex_buff[1024];
   unsigned sbptr;

   printf("\nMIDIREC version %s                Copyright (C) 1991, 1992 Miles Design, Inc.\n",VERSION);
   printf("-------------------------------------------------------------------------------\n\n");

   argma=argmi=-1;

   bad=strcnt=0;
   for (i=1;i<argc;i++)
      if (argv[i][0] != '/')
         if (strcnt == 1)
            {
            bad=1; break;
            }
         else
            {
            strcpy(outfn,argv[i]);
            strcnt++;
            }
      else
         if (!strnicmp(argv[i],"/MA:",4))
            argma = (int) val(&argv[i][4],16);
         else if (!strnicmp(argv[i],"/MI:",4))
            argmi = (int) val(&argv[i][4],10);
         else
            {
            bad=1; break;
            }

   if (bad || (!strcnt))
      {
      printf("This program writes incoming MIDI data to a specified MIDI Format 0 file.\n\n");
      printf("Usage: MIDIREC filename [/MA:xx] [/MI:xx]\n\n");
      printf("where /MA:xx = MPU-401 MIDI interface I/O address xx (hex)\n");
      printf("      /MI:xx = MPU-401 MIDI interface interrupt number xx\n\n");
      printf("Parameters in brackets [] are optional, and may appear in any order.\n");
      exit(1);
      }

   for (i=0;i<256;i++)
      {
      event_type[i] = 0;
      data_bytes[i] = 0;
      }
   for (i=0x80;i<=0xef;i++)
      {
      event_type[i] = 1;
      data_bytes[i] = 2 - ((i >= 0xc0) && (i <= 0xdf));
      }
   event_type[0xf0] = 2; event_type[0xf7] = 3;

   DATA_IO = 0x330;
   STAT_COM = 0x331;
   IRQ = 2;

   if (argmi != -1) IRQ = argmi;
   if (argma != -1)
      {
      DATA_IO = argma;
      STAT_COM = argma+1;
      }

   if (!send_cmd(0xff))
      if (!send_cmd(0xff))
         {
         printf("Can't initialize MPU-401 MIDI interface.\n");
         exit(1);
         }

   AIL_startup();
   timer=AIL_register_timer(delta_update);
   AIL_set_timer_frequency(timer,240L);

   out=fopen(outfn,"w+b");
   if (out == NULL)
      {
      printf("Can't open %s.\n",outfn);
      AIL_shutdown(NULL);
      exit(1);
      }

   fwrite(MIDI_header,sizeof(MIDI_header),1,out);

   init_queue();

   send_cmd(0x3f);

   disable();
   default_isr = getvect(IRQ+8);
   orig_PIC = inportb(0x21);
   setvect(IRQ+8,MPU_isr);
   outportb(0x21,inportb(0x21) & (~(1 << IRQ)));
   enable();

   printf("Waiting for SysEx or Channel Voice message status...\n\n");

   while (kbhit()) getch();
   while (!kbhit())
      if (check_queue())
         if (event_type[watch_queue()]) break;

   if (kbhit())
      {
      getch();
      printf("Aborted.\n");
      quit();
      unlink(outfn);
      exit(0);
      }

   printf("MIDI recording in progress.  Press any key to stop.\n\n");

   AIL_start_timer(timer);
   delta_time = 0L;
   status = 0;
   accum_delta=0L;
   expect_bytes = 0;
   sysex_active = 0;
   sbptr = 0;

   //
   // Main loop
   //

   while (kbhit()) getch();
   while (!kbhit())
      {
      if (!check_queue()) continue;
      in = read_queue();

      accum_delta += (unsigned long) in.delta;
      if (accum_delta >= 65535L) accum_delta=65535L;

      switch (event_type[in.byte])
         {
         case 0:              // data byte
            if ((sysex_active) && (sbptr < 1024))
               {
               sysex_buff[sbptr++] = in.byte;
               break;
               }

            if (expect_bytes)
               {
               --expect_bytes;
               fputc(in.byte,out);
               break;
               }

         case 1:              // Channel Voice status (or running status)
            sysex_active = 0;
            if (in.byte >= 0x80) status = in.byte;
            if (status < 0x80) break;
            write_VLN(accum_delta);
            accum_delta = 0L;
            fputc(in.byte,out);
            expect_bytes = data_bytes[status] - (in.byte < 0x80);
            break;
         
         case 2:              // System Exclusive begin
            write_VLN(accum_delta);
            accum_delta = 0L;
            fputc(in.byte,out);
            status = 0;
            sbptr = 0;
            sysex_active = 1;
            break;

         case 3:              // System Exclusive end
            if (!sysex_active)
               {
               status = 0;
               break;
               }
            write_VLN(sbptr+1);
            for (i=0;i<sbptr;i++)
               fputc(sysex_buff[i],out);
            fputc(in.byte,out);
            sysex_active = 0;
            break;
         }
      }
   getch();

   fwrite(end_of_track,sizeof(end_of_track),1,out);
   fflush(out);
   flen = filelength(fileno(out)) - sizeof(MIDI_header);
   fseek(out,18L,SEEK_SET);
   flen += 15L;
   flen = wswap(flen);
   fwrite(&flen,4,1,out);

   printf("MIDIREC stopped.\n");
   quit();
   exit(0);
}
