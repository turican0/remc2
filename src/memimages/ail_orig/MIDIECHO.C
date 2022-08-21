//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   MIDIECHO.C                                                          лл
//лл                                                                       лл
//лл   MIDI receiver / interpreter for AIL drivers                         лл
//лл                                                                       лл
//лл   V2.00 of 22-Oct-91                                                  лл
//лл   V2.01 of 12-Dec-91: New timbre request structure                    лл
//лл   V2.02 of 29-Mar-92: /DO option added                                лл
//лл   V2.03 of 20-Jun-92: GTL files > 64K supported                       лл
//лл   V2.04 of 17-Aug-92: Volume scaling, /NV option added                лл
//лл                                                                       лл
//лл   Project: IBM Audio Interface Library                                лл
//лл    Author: John Miles                                                 лл
//лл                                                                       лл
//лл   C source compatible with Turbo C++ v1.0 or later                    лл
//лл                                                                       лл
//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   midiecho.obj: midiecho.c gen.h ail.h                                лл
//лл      bcc -ml -c -v midiecho.c                                         лл
//лл                                                                       лл
//лл   midiecho.exe: midiecho.obj gen.lib ail.obj                          лл
//лл      tlink @midiecho.lls                                              лл
//лл                                                                       лл
//лл   Contents of MIDIECHO.LLS:                                           лл
//лл     /c /v /x +                                                        лл
//лл     \bc\lib\c0l.obj +                                                 лл
//лл     midiecho ail, +                                                   лл
//лл     midiecho.exe, +                                                   лл
//лл     midiecho.map, +                                                   лл
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

#pragma warn -par
#pragma warn -aus

#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <alloc.h>
#include <string.h>
#include <conio.h>

#include "ail.h"        // Audio Interface Library API function header file
#include "gen.h"        // General DOS and system functions

union REGS inregs,outregs;

const char VERSION[] = "2.04";

unsigned DATA_IO;       // base addr
unsigned STAT_COM;      // base addr+1
unsigned IRQ;           // MPU-401 IRQ

unsigned AIL_IO;        // AIL driver base addr & IRQ
unsigned AIL_IRQ;    

unsigned head,tail;     // incoming MIDI message buffers & pointers
unsigned char queue[4096];    
unsigned char sysex[4096]; 

unsigned cv_message[8]; // outgoing Channel Voice message & data
unsigned cv_bytecnt;
unsigned cv_status;

unsigned driver_vol;    // driver volume %

unsigned orig_PIC;
void interrupt (far *default_isr)();

HDRIVER hdriver;
HTIMER maintain;

char far *GTL;

unsigned dy_opt;
unsigned dr_opt;
unsigned do_opt;
unsigned nv_opt;
int partials,OPL_voices;
int chan_partials[16];
unsigned char far *screen, far *base_addr;

unsigned timbre_bank[16];
unsigned timbre_patch[16];
unsigned RBS[16];

typedef struct                      
   {
      char patch;
      char bank;
      unsigned long offset;
   }
GTL_hdr;

const int init_prg[] = {-1,68,48,95,78,41,3,110,122,-1,-1,-1,-1,-1,-1,-1};

typedef struct
{
   int list[128];
   int part[16];
   int base[16];
}
partial_list;

const partial_list Roland_preset =
{
   {
   4,2,1,3,2,2,1,3,3,3,2,2,3,3,2,2,
   4,2,1,3,2,1,4,2,2,3,2,2,2,2,2,1,
   3,3,3,2,4,4,4,1,3,3,2,2,2,2,2,2,
   4,3,2,3,3,2,3,2,2,3,2,2,2,4,3,4,
   2,1,2,1,3,2,4,2,4,2,3,2,2,3,4,3,
   2,1,3,2,2,2,2,2,3,2,3,2,3,2,2,4,
   3,3,2,1,3,2,4,1,3,2,4,4,2,1,4,3,
   2,1,2,2,2,3,1,2,2,2,4,1,1,4,3,4
   },

   {
   0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0
   },

   {
   0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0
   }

};

const partial_list YM3812_default =
{
   {
   2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
   2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
   2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
   2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
   2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
   2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
   2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
   2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2
   },

   {
   0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0
   },

   {
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
   }
};

partial_list OPL3_default =
{
   {
   2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
   2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
   2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
   2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
   2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
   2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
   2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
   2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2
   },

   {
   0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0
   },

   {
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
   }
};


partial_list pmap;

char g1[] = "ЩЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЛ";
char g2[] = "КњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњњК";
char g3[] = "ШЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭМ";

char attribs[3][80];

#define FRAME_ATTR 0x1f
#define OK_ATTR 0x1a
#define BAD_ATTR 0x1c
#define WARN_ATTR 0x1e

#define BAR_SIZE 77

#define BAR_CHAR 219
#define PEAK_CHAR 7
#define BLANK_CHAR 250
#define T_MARK_CHAR 209
#define B_MARK_CHAR 207

#define AIL_PBS 114
#define PART_VOL 7

#define NOTE_OFF 0x80
#define NOTE_ON 0x90
#define CTRL_CHG 0xb0
#define PRG_CHG 0xc0

unsigned char dummy_XMI[] =
{
   0x46,0x4F,0x52,0x4D,0x00,0x00,0x00,0x0E,
   0x58,0x44,0x49,0x52,0x49,0x4E,0x46,0x4F,
   0x00,0x00,0x00,0x02,0x01,0x00,0x43,0x41,
   0x54,0x20,0x00,0x00,0x00,0x2E,0x58,0x4D,
   0x49,0x44,0x46,0x4F,0x52,0x4D,0x00,0x00,
   0x00,0x22,0x58,0x4D,0x49,0x44,0x45,0x56,
   0x4E,0x54,0x00,0x00,0x00,0x16,0xFF,0x59,
   0x02,0x00,0x00,0xFF,0x51,0x03,0x09,0x27,
   0xC0,0xFF,0x58,0x04,0x04,0x02,0x18,0x08,
   0xFF,0x2F,0x00,0x00
};   

#define callback

/***************************************************************/
unsigned send_cmd(unsigned cmd)
{
   unsigned i;

   disable();

   i = 65535U;
   while (--i)
      if (!(inportb(STAT_COM) & 0x40)) break;

   if (!i)
      {
      enable();
      return 0;
      }

   outportb(STAT_COM,cmd);

   for (i=65535U;i;i--)
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

   i = 65535U;
   while (--i)
      if (!(inportb(STAT_COM) & 0x40)) break;

   if (!i)
      return;

   outportb(DATA_IO,b);
}


/***************************************************************/
void locate(int x, int y)
{
   inregs.h.ah = 0x0f;
   int86(0x10,&inregs,&outregs);

   inregs.h.ah = 0x02;
   inregs.h.dh = y; inregs.h.dl = x; inregs.h.bh = outregs.h.bh;
   int86(0x10,&inregs,&outregs);
}

int curpos_x(void)
{
   inregs.h.ah = 0x0f;
   int86(0x10,&inregs,&outregs);

   inregs.h.bh = outregs.h.bh; inregs.h.ah = 0x03;
   int86(0x10,&inregs,&outregs);

   return outregs.h.dl;
}

int curpos_y(void)
{
   inregs.h.ah = 0x0f;
   int86(0x10,&inregs,&outregs);

   inregs.h.bh = outregs.h.bh; inregs.h.ah = 0x03;
   int86(0x10,&inregs,&outregs);

   return outregs.h.dh;
}

/***************************************************************/
char far *locate_timbre(unsigned b, unsigned n)
{
   char far *G_ptr;
   GTL_hdr GHDR;

   if (GTL==NULL) return NULL;

   G_ptr = GTL;

   do
      {
      GHDR = *(GTL_hdr far *) G_ptr;
      G_ptr = add_ptr(G_ptr,sizeof(GHDR));
      if (GHDR.bank == -1)        
         return NULL;
      }
   while ((GHDR.bank != b) ||
          (GHDR.patch != n));

   return add_ptr(GTL,GHDR.offset);
}


/***************************************************************/
unsigned setup_timbre(unsigned chan)
{
   int b,n;
   char far *tptr;

   b = timbre_bank[chan];
   n = timbre_patch[chan];

   if (n==-1) return 0;

   if (AIL_timbre_status(hdriver,b,n) != 0) return 0;

   tptr = locate_timbre(b,n);

   if (tptr == NULL)
      if (dy_opt || dr_opt || do_opt)
         putch(7);
      else
         {
         printf("Warning: Timbre bank %u, number %u not found in ",b,n);
         printf("Global Timbre Library\n");
         }
   else
      AIL_install_timbre(hdriver,b,n,tptr);
   
   return 1;
}

/***************************************************************/
unsigned get_channel_partials(unsigned chan)
{
   int b,n;
   char far *tptr;
   static unsigned nbits[] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};

   if ((n=pmap.base[chan]) != 0) return n;

   if (!pmap.part[chan]) return 0;

   b = timbre_bank[chan];
   n = timbre_patch[chan];

   if (n==-1) return 0;

   if (dy_opt)
      return 2;

   if (do_opt)
      return (*locate_timbre(b,n) == 25) ? 4:2;

   if (dr_opt)
      {
      if (b == 0) return pmap.list[n];

      tptr = locate_timbre(b,n);
      if (tptr == NULL) return 0;

      return nbits[tptr[0x0c]]; 
      }


   return 0;
}

/***************************************************************/
unsigned intercept_timbre_request(unsigned status, unsigned d1, unsigned d2)
{
   unsigned ch = status & 0x0f;
   unsigned val;

   val = d2;

   switch (status & 0xf0)
      {
      case NOTE_ON:
         if ((!d2) || (!RBS[ch])) break;
         timbre_patch[ch] = d1;
         timbre_bank[ch] = RBS[ch];
         setup_timbre(ch);
         chan_partials[ch] = get_channel_partials(ch);
         break;

      case PRG_CHG:
         timbre_patch[ch] = d1;
         setup_timbre(ch);
         chan_partials[ch] = get_channel_partials(ch);
         break;

      case CTRL_CHG:
         switch (d1)
            {
            case AIL_PBS:
               timbre_bank[ch] = d2;
               break;

            case PART_VOL:
               val = (d2 * driver_vol) / 100;
               break;
            }
         break;
      }

   return val;
}

/***************************************************************/
void callback reset_peak(void)
{
   unsigned i;

   for (i=0;i<BAR_SIZE;i++)
      if (base_addr[i*2] == PEAK_CHAR)
         base_addr[i*2] = BLANK_CHAR;
}

/***************************************************************/
void set_graph_attribs(char attribs[3][80])
{
   unsigned j,t,i;
   unsigned char far *l1 = base_addr - 2 - 160;
   unsigned char far *l2 = base_addr - 2;
   unsigned char far *l3 = base_addr - 2 + 160;

   for (i=0;i<BAR_SIZE+2;i++)
      {
      j=2*i+1;
      t=l1[j]; l1[j] = attribs[0][i]; attribs[0][i] = t;
      t=l2[j]; l2[j] = attribs[1][i]; attribs[1][i] = t;
      t=l3[j]; l3[j] = attribs[2][i]; attribs[2][i] = t;
      }
}

/***************************************************************/
void show_bar(unsigned pcnt)
{
   unsigned i,j;

   for (i=0;i<pcnt;i++)
      base_addr[i*2] = BAR_CHAR;

   for (j=i;j<BAR_SIZE;j++)
      if (base_addr[j*2] == BAR_CHAR)
         base_addr[j*2] = PEAK_CHAR;

   if (do_opt)
      {
      if (OPL_voices > 6)
         {
         for (j=0;j<BAR_SIZE;j++)
            if (base_addr[j*2+1] == OK_ATTR)
               base_addr[j*2+1] = WARN_ATTR;
         }
      else
         {
         for (j=0;j<BAR_SIZE;j++)
            if (base_addr[j*2+1] == WARN_ATTR)
               base_addr[j*2+1] = OK_ATTR;
         }
      }
}

/***************************************************************/
void count_partial_usage(unsigned status, unsigned d1, unsigned d2)
{
   unsigned stat = status & 0xf0;
   unsigned chan = status & 0x0f;

   if ((stat == NOTE_ON) && (d2))
      {
      partials += chan_partials[chan];
      if (chan_partials[chan] == 4) ++OPL_voices;
      }
   else if ((stat == NOTE_OFF) || ((stat == NOTE_ON) && (!d2)))
      {
      partials -= chan_partials[chan];
      if (chan_partials[chan] == 4) --OPL_voices;
      }
   else return;

   if (partials > BAR_SIZE)
      show_bar(BAR_SIZE);
   else if (partials < 0)
      {
      partials=0;
      OPL_voices=0;
      show_bar(0);
      }
   else
      show_bar(partials);
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

int read_queue(void)
{
   unsigned n;

   if (tail==head) return -1;

   n = queue[tail++];
   if (tail==sizeof(queue)) tail=0;

   return n;
}

void write_queue(unsigned n)
{
   queue[head++] = n;
   if (head==sizeof(queue)) head=0;
}

/***************************************************************/
void init_message(void)
{
   cv_status = 0;
   cv_bytecnt = 0;
}

void write_message_byte(unsigned n)
{
   static unsigned sysex_ptr = 0;
   unsigned i,status,channel;

   if (n == 0xf0)
      {
      sysex[0] = 0xf0;
      sysex_ptr = 1;
      return;
      }

   if (sysex_ptr)
      {
      if (n == 0xf7)
         {
         sysex[sysex_ptr] = 0xf7;
         for (i=0;i<=sysex_ptr;i++)
            send_byte(sysex[i]);
         sysex_ptr = 0;
         }
      else
         {
         sysex[sysex_ptr] = n;
         ++sysex_ptr;
         }
      return;
      }

   if (n >= 0x80)
      {
      if (n < 0xf8)
         {
         cv_bytecnt = 0;

         status = n & 0xf0;
         if (status >= 0xf0) status = 0;

         channel = n & 0x0f;
         if (channel > 9) channel -= 9;

         cv_status = status | channel;
         }
      return;
      }
   
   if (cv_bytecnt == 8) cv_bytecnt = 0;

   if (cv_status >= 0x80) cv_message[cv_bytecnt++] = n;
 
   if (cv_bytecnt >= (2 - ((cv_status & 0xe0) == 0xc0)))
      {
      cv_bytecnt = 0;

      cv_message[1] =
         intercept_timbre_request(cv_status,cv_message[0],cv_message[1]);

      count_partial_usage(cv_status,cv_message[0],cv_message[1]);

      AIL_send_channel_voice_message(hdriver,cv_status,cv_message[0],
         cv_message[1]);
      }
}     

/***************************************************************/
void interrupt MPU_isr(void)
{
   while (!(inportb(STAT_COM) & 0x80))
      write_queue(inportb(DATA_IO));
       
   outportb(0x20,0x20);
}

/***************************************************************/
void main(int argc, char *argv[])
{
   drvr_desc far *desc;
   char far *drvr;
   char far *s;
   HSEQUENCE hs;
   char fnames[2][64];
   int arga,argi,argma,argmi;
   int bad,strcnt;
   int lim,i;
   char far *tc_addr;
   unsigned tc_size;
   unsigned base_x,base_y;

   screen = (unsigned char far *)
      (((*(int far *)0x00000463L) == 0x3b4) ? 0xb0000000L : 0xb8000000L);

   printf("\nMIDIECHO version %s               Copyright (C) 1991, 1992 Miles Design, Inc.\n",VERSION);
   printf("-------------------------------------------------------------------------------\n\n");

   arga=argi=argma=argmi=-1;
   fnames[0][0] = fnames[1][0] = 0;

   dy_opt=dr_opt=do_opt=nv_opt=0;
   bad=strcnt=0;
   for (i=1;i<argc;i++)
      if (argv[i][0] != '/')
         if (strcnt == 2)
            {
            bad=1; break;
            }
         else
            strcpy(fnames[strcnt++],argv[i]);
      else
         if (!strnicmp(argv[i],"/MA:",4))
            argma = (int) val(&argv[i][4],16);
         else if (!strnicmp(argv[i],"/MI:",4))
            argmi = (int) val(&argv[i][4],10);
         else if (!strnicmp(argv[i],"/A:",3))
            arga = (int) val(&argv[i][3],16);
         else if (!strnicmp(argv[i],"/I:",3))
            argi = (int) val(&argv[i][3],10);
         else if (!strnicmp(argv[i],"/DY",3))
            dy_opt = 1;
         else if (!strnicmp(argv[i],"/DR",3))
            dr_opt = 1;
         else if (!strnicmp(argv[i],"/DO",3))
            do_opt = 1;
         else if (!strnicmp(argv[i],"/NV",3))
            nv_opt = 1;
         else
            {
            bad=1; break;
            }

   if (bad || (strcnt == 0))
      {
      printf("This program echoes incoming MIDI data to a specified Audio Interface Library\n");
      printf("sound driver.\n\n");
      printf("Usage: MIDIECHO driver_filename [instrument_filename] [/MA:xx] [/MI:xx] [/A:xx]\n");
      printf("       [/I:xx] [/DY | /DO | /DR] [/NV]\n\n");
      printf("where /MA:xx = MPU-401 MIDI interface I/O address xx (hex)\n");
      printf("      /MI:xx = MPU-401 MIDI interface interrupt number xx\n");
      printf("       /A:xx = Audio Interface Library driver I/O address xx (hex)\n");
      printf("       /I:xx = Audio Interface Library driver interrupt number\n");
      printf("         /DY = Display Yamaha YM3812 partial usage (2 partials/note)\n");
      printf("         /DO = Display Yamaha YMF262 (OPL3) partial usage (2/4 partials/note\n");
      printf("         /DR = Display Roland MT-32 partial usage (1-4 partials/note)\n");
      printf("         /NV = Do not scale volume values to simulate runtime XMIDI playback\n");
      printf("\nParameters in brackets [] are optional, and may appear in any order.\n");
      exit(1);
      }

   AIL_startup();

   drvr = load_driver(fnames[0]);
   if (drvr==NULL)     
      {
      printf("Driver %s not found\n",fnames[0]);
      exit(1);
      }
   hdriver = AIL_register_driver(drvr);

   desc = AIL_describe_driver(hdriver);
   if (desc->drvr_type != XMIDI_DRVR)
      {
      printf("%s is not an Extended MIDI driver.\n",fnames[0]);
      AIL_shutdown(NULL);
      exit(1);
      }

   DATA_IO = 0x330;
   STAT_COM = 0x331;
   IRQ = 2;
   AIL_IRQ = desc->default_IRQ;
   AIL_IO = desc->default_IO;

   if (argi != -1) AIL_IRQ = argi;
   if (arga != -1) AIL_IO = arga;
   if (argmi != -1) IRQ = argmi;
   if (argma != -1)
      {
      DATA_IO = argma;
      STAT_COM = argma+1;
      }

   if (!AIL_detect_device(hdriver,AIL_IO,AIL_IRQ,
      desc->default_DMA,desc->default_DRQ))
         {
         printf("AIL sound hardware not found.\n");
         AIL_shutdown(NULL);
         exit(1);
         }

   AIL_init_driver(hdriver,AIL_IO,AIL_IRQ,
      desc->default_DMA,desc->default_DRQ);

   if (nv_opt)
      driver_vol = 100;
   else
      {
      s = farmalloc(AIL_state_table_size(hdriver));
      hs = AIL_register_sequence(hdriver,dummy_XMI,0,s,NULL);
      driver_vol = AIL_relative_volume(hdriver,hs);
      AIL_release_sequence_handle(hdriver,hs);
      farfree(s);
      }

   tc_size = AIL_default_timbre_cache_size(hdriver);
   if (tc_size)
      {
      tc_addr = farmalloc((unsigned long) tc_size);
      AIL_define_timbre_cache(hdriver,tc_addr,tc_size);
      }

   GTL = NULL;
   if (fnames[1][0] != 0)
      {
      printf("Loading Global Timbre Library file %s ....\n\n",fnames[1]);
      GTL = read_file(fnames[1],NULL);
      if (GTL == NULL)
         {
         printf("Can't load %s.\n",fnames[1]);
         AIL_shutdown(NULL);
         exit(1);
         }
      }

   if (!send_cmd(0xff))
      if (!send_cmd(0xff))
         {
         printf("Can't initialize MPU-401 MIDI interface.\n");
         AIL_shutdown(NULL);
         exit(1);
         }

   init_message();
   init_queue();

   send_cmd(0x3f);      // init UART mode for reception/xmission

   disable();
   default_isr = getvect(IRQ+8);
   orig_PIC = inportb(0x21);
   setvect(IRQ+8,MPU_isr);
   outportb(0x21,inportb(0x21) & (~(1 << IRQ)));
   enable();

   if (dy_opt || dr_opt || do_opt)
      {
      if (dy_opt)
         {
         lim = 18;
         pmap = YM3812_default;
         printf("Yamaha YM3812 melodic-mode partial usage (%u max):\n\n",lim);
         }
      else if (dr_opt)
         {
         lim = 32;
         pmap = Roland_preset;
         printf("Roland MT-32 partial usage (%u max):\n\n",lim);
         }
      else if (do_opt)
         {
         lim = 36;
         pmap = OPL3_default;
         printf("Yamaha YMF262 (OPL3) melodic-mode partial usage (%u max):\n\n",lim);
         }

      g1[lim] = T_MARK_CHAR;
      g3[lim] = B_MARK_CHAR;

      for (i=0;i<3;i++)
         attribs[i][0] = attribs[i][BAR_SIZE+1] = FRAME_ATTR;

      for (i=1;i<BAR_SIZE+1;i++)
         {
         attribs[0][i] = FRAME_ATTR;
         attribs[1][i] = (i > lim) ? BAD_ATTR:OK_ATTR;
         attribs[2][i] = FRAME_ATTR;
         }

      printf("%s\n",g1);
      printf("%s\n",g2);
      printf("%s\n\n",g3);

      maintain = AIL_register_timer(reset_peak);
      AIL_set_timer_period(maintain,2000000L);
      AIL_start_timer(maintain);
      }

   printf("MIDI performance in progress.  Press any key to stop.\n\n");


   if (dy_opt || dr_opt || do_opt)
      {
      base_x = 1; base_y = curpos_y() - 5;
      base_addr = screen + (160 * base_y) + (2 * base_x);
      partials = OPL_voices = 0;
      set_graph_attribs(attribs);
      }

   for (i=0;i<16;i++)
      {
      RBS[i] = (i == 9)? 127:0;
      timbre_bank[i] = 0;
      timbre_patch[i] = init_prg[i];
      setup_timbre(i);
      chan_partials[i] = get_channel_partials(i);
      }

   //
   // Main loop
   //

   while (kbhit()) getch();
   while (!kbhit())
      if (check_queue()) write_message_byte(read_queue());
   getch();

   send_cmd(0xff);      // reset MPU-401

   setvect(IRQ+8,default_isr);
   outportb(0x21,orig_PIC);

   printf("MIDIECHO stopped.\n");
   AIL_shutdown("MIDIECHO stopped.");
}



