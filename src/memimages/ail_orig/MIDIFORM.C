//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   MIDIFORM.C                                                          лл
//лл                                                                       лл
//лл   Extended MIDI (XMIDI) file conversion utility                       лл
//лл                                                                       лл
//лл   V1.00 of 15-Sep-91                                                  лл
//лл   V1.01 of 27-Nov-91: MAX_EVLEN increased to 384                      лл
//лл   V1.02 of  7-Feb-92: Input line comments allowed                     лл
//лл                       Quant math precision increased                  лл
//лл   V1.03 of 18-Dec-92: Branch points logged after index controller     лл
//лл                       events                                          лл
//лл   V1.04 of 22-Jan-93: Longer delta-time events handled                лл
//лл   V1.05 of 16-Feb-93: Rhythm-note callback option added               лл
//лл                                                                       лл
//лл   Project: IBM Audio Interface Library                                лл
//лл    Author: John Miles                                                 лл
//лл                                                                       лл
//лл   C source compatible with Turbo C++ v1.0 or later                    лл
//лл                                                                       лл
//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   midiform.obj: midiform.c gen.h                                      лл
//лл      bcc -ml -c -v midiform.c                                         лл
//лл                                                                       лл
//лл   midiform.exe: midiform.obj gen.lib                                  лл
//лл      tlink @midiform.lls                                              лл
//лл                                                                       лл
//лл   Contents of MIDIFORM.LLS:                                           лл
//лл     /c /v /x +                                                        лл
//лл     \bc\lib\c0l.obj +                                                 лл
//лл     midiform, +                                                       лл
//лл     midiform.exe, +                                                   лл
//лл     midiform.map, +                                                   лл
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

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <alloc.h>
#include <string.h>
#include <io.h>
#include <conio.h>
#include <ctype.h>

#include "gen.h"           // General DOS and system functions

const char VERSION[] = "1.05";

#define DEFAULT_QUAN 120   // Default quantization rate in hertz
#define MAX_TRKS 64        // Max. # of tracks in MIDI input file
#define MAX_EVLEN 384      // Max. length of MIDI event in bytes
#define MAX_TIMB 16384     // Max. # of timbre requests in MIDI file
#define MAX_RBRN 128       // Max. # of unique branch targets in MIDI file
#define MAX_NOTES 32       // Max. # of notes simultaneously "on" in MIDI file
#define CHAN_CNT 16        // # of MIDI channels

#define AIL_BRANCH_PT 120  // AIL XMIDI controller for branch points
#define AIL_TIMB_BNK  114  // AIL XMIDI controller: Timbre Bank Select
#define AIL_CALLBACK  119  // AIL XMIDI controller: Callback Trigger

#define EV_NOTE_OFF 0x80   // Standard MIDI file event types
#define EV_NOTE_ON 0x90
#define EV_POLY_PRESS 0xa0
#define EV_CONTROL 0xb0
#define EV_PROGRAM 0xc0
#define EV_CHAN_PRESS 0xd0
#define EV_PITCH 0xe0
#define EV_SYSEX 0xf0
#define EV_ESC 0xf7
#define EV_META 0xff
#define EV_INVALID 0x00

#define META_EOT 0x2f      // Standard MIDI meta-event types
#define META_TRK_NAME 0x03
#define META_INS_NAME 0x04
#define META_TEMPO 0x51

#define MAX_ULONG 4294967295L

typedef struct 
{
   unsigned bnum;
   unsigned long offset;
}
RBRN_entry;

typedef struct
{
   unsigned char t_num;
   unsigned char t_bank;
}
TIMB_entry;

typedef struct
{
   unsigned quantization;
}
AILH_block;

typedef struct
{
   unsigned cnt;
   RBRN_entry brn[MAX_RBRN];
}
RBRN_block;

typedef struct
{
   unsigned cnt;
   TIMB_entry tbr[MAX_TIMB];
}
TIMB_block;

typedef struct
{
   unsigned long len;
   unsigned long avail;
   unsigned char far *name;
   unsigned char far *base;
   unsigned char far *ptr;
}
IFF_block;

typedef struct
{
   unsigned char far *seq_fn;
   unsigned char far *base;
   unsigned long seq_len;

   unsigned format;
   unsigned ntrks;
   unsigned division;
   unsigned long tick_time;
   unsigned char event[MAX_EVLEN];
   unsigned long event_len;
   unsigned long event_time;
   unsigned event_chan;

   unsigned event_trk;
   unsigned char far *trk_ptr[MAX_TRKS];
   unsigned char status[MAX_TRKS];
   unsigned char trk_active[MAX_TRKS];
   unsigned long pending_delta[MAX_TRKS];
}               
MIDI;

typedef struct
{
   unsigned long q_int;
   long double DDA_sum;
   unsigned long interval;
   unsigned long delta;
   unsigned char note_chan[MAX_NOTES];
   unsigned char note_num[MAX_NOTES];
   unsigned long note_intvl[MAX_NOTES];
   unsigned char far *note_next[MAX_NOTES];
   unsigned char timb_BNK[CHAN_CNT];
   unsigned rbs_BNK[CHAN_CNT];
   AILH_block AILH;
   RBRN_block RBRN;
   TIMB_block TIMB;
   IFF_block EVNT;
}
XMIDI;

char *tmp_fn = "MFXXXXXX";
char out_fn[128];
union REGS inregs,outregs;
unsigned c_opt;

/************************************************************/
void abend(int err, unsigned long info_1, unsigned long info_2)
{
   if (!err) return;

   fprintf(stderr,"Error MF%03u: ",err);

   switch (err)
      {
      case 1:
         switch (info_1)
            {
            case 1:
               fprintf(stderr,"I/O fault"); break;
            case 2:
               fprintf(stderr,"Insufficient free memory"); break;
            case 3:
               fprintf(stderr,"File \"%s\" not found",info_2); break;
            case 4:
               fprintf(stderr,"Can't write to file \"%s\"",info_2); break;
            case 5:
               fprintf(stderr,"Can't read from file \"%s\"",info_2); break;
            case 6:
               fprintf(stderr,"Disk full"); break;
            default:
               fprintf(stderr,"Undefined disk error"); break;
            }
         break;
      case 2:
         fprintf(stderr,"File \"%s\" not a standard MIDI file",info_1); break;
      case 3:
         fprintf(stderr,"MIDI event > %u bytes long in file \"%s\"",
            MAX_EVLEN,info_1);
         break;
      case 4:
         fprintf(stderr,"Illegal MIDI status byte in file \"%s\"",info_1);
         break;
      case 5:
         fprintf(stderr,"Illegal MIDI event in file \"%s\"",info_1); break;
      case 6:
         fprintf(stderr,"> %u tracks in MIDI file \"%s\"",MAX_TRKS,info_1);
         break;
      case 7:
         fprintf(stderr,"Insufficient memory to convert \"%s\"",info_1);
         break;
      case 8:
         fprintf(stderr,"Unpaired MIDI note-on event in file \"%s\"",
            info_1);
         break;
      case 9:
         fprintf(stderr,"> %u simultaneous notes in file \"%s\"",
            MAX_NOTES,info_1);
         break;
      case 10:
         fprintf(stderr,"> %u branch point controllers in file \"%s\"",
            MAX_RBRN,info_1);
         break;
      case 11:
         fprintf(stderr,"> %u timbre request controller pairs in file \"%s\"",
            MAX_TIMB,info_1);
         break;
      case 12:
         fprintf(stderr,"Duplicate branch point controller in ");
         fprintf(stderr,"file \"%s\"",info_1);
         break;
      }                      
   
   fprintf(stderr,"\n");
   fcloseall();
   if (strlen(tmp_fn)) unlink(tmp_fn);
   if (strlen(out_fn)) unlink(out_fn);
   exit(err);
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
FILE *IFF_create_file(char *filename)
{
   static FILE *out;

   out = fopen(filename,"w+b");

   return out;
}

/***************************************************************/
int IFF_append_CAT(FILE *out, char *CAT_type)
{
   static unsigned long len = 0L;

   fputs("CAT ",out);
   fwrite(&len,4,1,out);
   fputs(CAT_type,out);
   fflush(out);

   return (!ferror(out));
}

/***************************************************************/
void IFF_close_CAT(FILE *out, unsigned long len_off)
{
   static unsigned long len;

   fflush(out);
   len = wswap(ftell(out)-len_off+4L);
   fseek(out,len_off-8L,SEEK_SET);
   fwrite(&len,4,1,out);
}

/***************************************************************/
void IFF_close_file(FILE *out)
{
   fclose(out);
}

/***************************************************************/
unsigned IFF_construct(IFF_block *BLK)
{
   BLK->ptr = norm(BLK->base);
   BLK->len = 0L;

   return 0L;
}

/***************************************************************/
int IFF_write_block(FILE *out, IFF_block *BLK)
{
   unsigned long len,blen;
   unsigned char far *ptr;

   ptr = BLK->base;
   len = BLK->len;

   blen = wswap(len + (len & 1L));
   fputs(BLK->name,out);
   fwrite(&blen,4,1,out);

   blen = len;
   while (len--)
      {
      fputc(*ptr,out);
      ptr = add_ptr(ptr,1L);
      }

   if (blen & 1L) fputc(0,out);

   return (!ferror(out));
}

/***************************************************************/
int IFF_append_FORM(FILE *out, char *FORM_type, FILE *in)
{
   static char buff[512];
   unsigned long len,blen;

   fseek(in,0L,SEEK_SET);
   fflush(in);

   len = filelength(fileno(in));

   blen = wswap(len + (len & 1L) + 4L);

   fputs("FORM",out);
   fwrite(&blen,4,1,out);

   fputs(FORM_type,out);
   
   blen = len;
   while (len > 512L)
      {
      fread(buff,512,1,in);
      fwrite(buff,512,1,out);
      len -= 512L;
      }
   fread(buff,len,1,in);
   fwrite(buff,len,1,out);

   if (blen & 1L) fputc(0,out);

   return (!ferror(out));
}

/***************************************************************/
void IFF_put_byte(unsigned val, IFF_block *BLK)
{
   *BLK->ptr = (unsigned char) val;
   BLK->ptr = add_ptr(BLK->ptr,1L);
   BLK->len++;
}

/***************************************************************/
void IFF_put_vln(unsigned long val, IFF_block *BLK)
{
   unsigned i,n,cnt;
   unsigned char bytefield[4];

   bytefield[3] = (val & 0x7fL);
   bytefield[2] = ((val >> 7) & 0x7fL) | 0x80;
   bytefield[1] = ((val >> 14) & 0x7fL) | 0x80;
   bytefield[0] = ((val >> 21) & 0x7fL) | 0x80;
   
   n=3;
   for (i=0;i<=3;i++)
      if (bytefield[i] & 0x7f)
         {
         n=i;
         break;
         }

   for (i=n;i<=3;i++)
      {
      *BLK->ptr = bytefield[i];
      BLK->ptr = add_ptr(BLK->ptr,1L);
      BLK->len++;
      }
}

/***************************************************************/
unsigned MIDI_get_chr(MIDI *MIDI, unsigned trk)
{
   unsigned val;
   unsigned char far *ptr = MIDI->trk_ptr[trk];

   val = (unsigned) *ptr;
   ptr = add_ptr(ptr,1L);

   MIDI->trk_ptr[trk] = ptr;

   return val;
}

unsigned MIDI_next_chr(MIDI *MIDI, unsigned trk)
{
   unsigned val;

   val = (unsigned) *MIDI->trk_ptr[trk]; 

   return val;
}

unsigned long MIDI_get_vln(MIDI *MIDI, unsigned trk)
{
   unsigned long val=0L;
   unsigned i,cnt=4;

   do
      {
      i = MIDI_get_chr(MIDI,trk);
      val = (val << 7) | (unsigned long) (i & 0x7f);
      if (!(i & 0x80))
         cnt = 0;
      else
         --cnt;
      }
   while (cnt);

   return val;
}

unsigned MIDI_vln_size(unsigned long val)
{
   unsigned cnt=0;

   do
      {
      cnt++;
      val >>= 7;
      }
   while (val);

   return cnt;
}

unsigned MIDI_put_vln(unsigned long val, unsigned char far *ptr)
{
   unsigned i,n,cnt;
   unsigned char bytefield[4];

   bytefield[3] = (val & 0x7fL);
   bytefield[2] = ((val >> 7) & 0x7fL) | 0x80;
   bytefield[1] = ((val >> 14) & 0x7fL) | 0x80;
   bytefield[0] = ((val >> 21) & 0x7fL) | 0x80;
   
   n=3;
   for (i=0;i<=3;i++)
      if (bytefield[i] & 0x7f)
         {
         n=i;
         break;
         }

   cnt=0;
   for (i=n;i<=3;i++)
      {
      *ptr = bytefield[i];
      ptr = add_ptr(ptr,1L);
      ++cnt;
      }

   return cnt;
}

unsigned long MIDI_get_32(MIDI *MIDI, unsigned trk)
{
   unsigned long val;

   val = (unsigned long) MIDI_get_chr(MIDI,trk);
   val = (val << 8) | (unsigned long) MIDI_get_chr(MIDI,trk);
   val = (val << 8) | (unsigned long) MIDI_get_chr(MIDI,trk);
   val = (val << 8) | (unsigned long) MIDI_get_chr(MIDI,trk);

   return val;
}

unsigned long MIDI_get_24(MIDI *MIDI, unsigned trk)
{
   unsigned long val;

   val = (unsigned long) MIDI_get_chr(MIDI,trk);
   val = (val << 8) | (unsigned long) MIDI_get_chr(MIDI,trk);
   val = (val << 8) | (unsigned long) MIDI_get_chr(MIDI,trk);

   return val;
}   

unsigned MIDI_get_16(MIDI *MIDI, unsigned trk)
{
   unsigned val;

   val = MIDI_get_chr(MIDI,trk);
   val = (val << 8) | MIDI_get_chr(MIDI,trk);

   return val;
}

/***************************************************************/
unsigned MIDI_construct(MIDI far *MIDI)
{
   unsigned long chunk_len;
   unsigned trk,bad;
   unsigned char far *src;
   unsigned long len;

   src = norm(MIDI->base);
   len = MIDI->seq_len;
   bad=1;
   while (len-- >= 4L)
      {
      if (!strnicmp(src,"MThd",4))
         {
         bad=0;
         break;
         }
      src = add_ptr(src,1L);
      };
   if (bad) return 2;

   chunk_len = wswap(*(unsigned long far *) (src+4));

   MIDI->ntrks = bswap(*(unsigned far *) (src+10));
   if (MIDI->ntrks > MAX_TRKS) return 6;

   MIDI->format = bswap(*(unsigned far *) (src+8));
   MIDI->division = bswap(*(unsigned far *) (src+12));

   MIDI->tick_time = (50000000L) / (unsigned long) MIDI->division;

   MIDI->event_time = 0L;
   MIDI->event_trk = MIDI->ntrks - 1;

   src = add_ptr(src,chunk_len+8L);

   trk = 0;
   do
      {
      chunk_len = wswap(*(unsigned long far *) (src+4));
      if (!strnicmp(src,"MTrk",4))
         {
         MIDI->trk_ptr[trk] = add_ptr(src,8L);
         MIDI->status[trk] = 0;
         MIDI->trk_active[trk] = 1;
         MIDI->pending_delta[trk] = MIDI_get_vln(MIDI,trk);
         trk++;                       
         }
      src = add_ptr(src,chunk_len+8L);
      }
   while (trk < MIDI->ntrks);

   return 0;
}

void MIDI_destroy(MIDI far *MIDI)
{
   farfree(MIDI->base);
}

/***************************************************************/
unsigned MIDI_event_type(MIDI far *MIDI)
{
   switch (MIDI->event[0] & 0xf0)
      {
      case EV_NOTE_OFF:
         return EV_NOTE_OFF;
      case EV_NOTE_ON:
         return (MIDI->event[2]) ? EV_NOTE_ON : EV_NOTE_OFF;
      case EV_POLY_PRESS:
         return EV_POLY_PRESS;
      case EV_CONTROL:
         return EV_CONTROL;
      case EV_PROGRAM:
         return EV_PROGRAM;
      case EV_CHAN_PRESS:
         return EV_CHAN_PRESS;
      case EV_PITCH:
         return EV_PITCH;
      case EV_SYSEX:
         switch (MIDI->event[0])
            {
            case EV_SYSEX: return EV_SYSEX;
            case EV_ESC: return EV_SYSEX;
            case EV_META: return EV_META;
            }
      default:
         return EV_INVALID;
      }
}

/***************************************************************/
unsigned MIDI_get_event(MIDI far *MIDI, int trk)
{
   int type;
   unsigned long cnt,len;
   unsigned char far *temp;

   if (MIDI_next_chr(MIDI,trk) >= 0x80)
      MIDI->status[trk] = MIDI_get_chr(MIDI,trk);

   if (MIDI->status[trk] < 0x80)
      return 5;

   MIDI->event_len = 0;
   MIDI->event[MIDI->event_len++] = MIDI->status[trk];

   switch (MIDI->status[trk] & 0xf0)
      {
      case EV_NOTE_OFF:
      case EV_NOTE_ON:
      case EV_POLY_PRESS:
      case EV_CONTROL:
      case EV_PITCH:
         MIDI->event[MIDI->event_len++] = MIDI_get_chr(MIDI,trk);
      case EV_PROGRAM:
      case EV_CHAN_PRESS:
         MIDI->event[MIDI->event_len++] = MIDI_get_chr(MIDI,trk);
         break;

      case EV_SYSEX:
         switch (MIDI->status[trk])
            {
            case EV_META:
               MIDI->event[MIDI->event_len++] = type = MIDI_get_chr(MIDI,trk);
               switch (type)
                  {
                  case META_EOT:
                     MIDI->trk_active[trk] = 0;
                     break;
                  case META_TEMPO:
                     temp = MIDI->trk_ptr[trk];
                     MIDI_get_vln(MIDI,trk);
                     MIDI->tick_time = (100L * MIDI_get_24(MIDI,trk)) /
                        (unsigned long) MIDI->division;
                     MIDI->trk_ptr[trk] = temp;
                     break;
                  }
            case EV_SYSEX:
            case EV_ESC:
               len = MIDI_get_vln(MIDI,trk);
               MIDI->event_len += MIDI_put_vln(len,
                  &(MIDI->event[MIDI->event_len]));

               for (cnt=0L;cnt<len;cnt++)
                  {
                  if (MIDI->event_len >= MAX_EVLEN)
                     return 3;
                  MIDI->event[MIDI->event_len++] = MIDI_get_chr(MIDI,trk);
                  }
               break;

            default:
               return 4;
            }
      }

   MIDI->pending_delta[trk] = MIDI_get_vln(MIDI,trk);
   MIDI->event_chan = MIDI->event[0] & 0x0f;
   return 0;
}

/***************************************************************/
unsigned MIDI_get_next_event(MIDI far *MIDI)
{
   unsigned long event_delta,min_delta;
   int trk,new_trk,trk_cnt;

   new_trk = -1;
   trk = MIDI->event_trk;
   trk_cnt = MIDI->ntrks;
   min_delta = MAX_ULONG;

   do
      {
      if (MIDI->trk_active[trk])
         {
         event_delta = MIDI->pending_delta[trk];
         if (event_delta <= min_delta)
            {
            min_delta = event_delta;
            new_trk = trk;        
            }
         }
      if (trk-- == 0)
         trk = MIDI->ntrks-1;
      }
   while (--trk_cnt);

   if (new_trk == -1) return 0;

   MIDI->event_trk = new_trk;
   MIDI->event_time = min_delta;

   for (trk=0;trk<MIDI->ntrks;trk++)
      if (MIDI->trk_active[trk])
         MIDI->pending_delta[trk] -= min_delta;

   abend(MIDI_get_event(MIDI,new_trk),(unsigned long) MIDI->seq_fn,0L);

   return 1;
}

/***************************************************************/
unsigned XMIDI_construct(XMIDI *XMIDI)
{
   unsigned i;

   XMIDI->RBRN.cnt = 0;
   XMIDI->TIMB.cnt = 0;

   XMIDI->q_int = (100000000L / (unsigned long) XMIDI->AILH.quantization);
   XMIDI->DDA_sum = 0L;
   XMIDI->interval = 0L;
   XMIDI->delta = 0L;

   for (i=0;i<MAX_NOTES;i++)
      XMIDI->note_chan[i] = 255;

   for (i=0;i<CHAN_CNT;i++)
      {
      XMIDI->rbs_BNK[i] = 0;
      XMIDI->timb_BNK[i] = 0;
      }

   XMIDI->rbs_BNK[9] = 127;

   XMIDI->EVNT.base = farmalloc(XMIDI->EVNT.avail);
   IFF_construct(&XMIDI->EVNT);

   return 0;
}

void XMIDI_destroy(XMIDI *XMIDI)
{
   farfree(XMIDI->EVNT.base);
}

/***************************************************************/
void XMIDI_accum_interval(XMIDI *XMIDI,MIDI *MIDI)
{
   XMIDI->DDA_sum += (long double) MIDI->event_time *
                     (long double) MIDI->tick_time;
   while (XMIDI->DDA_sum >= (long double) XMIDI->q_int)
      {
      XMIDI->DDA_sum -= (long double) XMIDI->q_int;
      XMIDI->interval++;
      XMIDI->delta++;
      }
}

/***************************************************************/
void XMIDI_write_interval(XMIDI *XMIDI)
{
   while (XMIDI->delta > 127L)
      {
      IFF_put_byte(127,&XMIDI->EVNT);
      XMIDI->delta -= 127L;
      }

   if (XMIDI->delta)
      IFF_put_byte((unsigned) XMIDI->delta, &XMIDI->EVNT);

   XMIDI->delta = 0L;
}

/***************************************************************/
void XMIDI_put_MIDI_event(XMIDI *XMIDI, MIDI *MIDI)
{
   unsigned i;

   if (XMIDI->delta) XMIDI_write_interval(XMIDI);

   for (i=0;i<MIDI->event_len;i++)
      IFF_put_byte(MIDI->event[i],&XMIDI->EVNT);
}

/***************************************************************/
void XMIDI_put_explicit_event(XMIDI *XMIDI, unsigned char *ev, unsigned len)
{
   unsigned i;

   if (XMIDI->delta) XMIDI_write_interval(XMIDI);

   for (i=0;i<len;i++)
      IFF_put_byte(ev[i],&XMIDI->EVNT);
}

/***************************************************************/
unsigned XMIDI_log_branch(XMIDI *XMIDI, MIDI *MIDI)
{
   unsigned i,b=XMIDI->RBRN.cnt;

   if (b >= MAX_RBRN) return 10;
   
   for (i=0;i<b;i++)
      if (XMIDI->RBRN.brn[i].bnum == MIDI->event[2]) return 12;

   XMIDI->RBRN.brn[b].offset = ptr_dif(XMIDI->EVNT.ptr,XMIDI->EVNT.base);
   XMIDI->RBRN.brn[b].bnum = MIDI->event[2];

   XMIDI->RBRN.cnt++;
   return 0;
}

/***************************************************************/
unsigned XMIDI_log_timbre_request(XMIDI *XMIDI, MIDI *MIDI)
{
   unsigned i,ch,val,t=XMIDI->TIMB.cnt;

   ch = MIDI->event_chan;

   if (t >= MAX_TIMB) return 11;

   switch (MIDI_event_type(MIDI))
      {
      case EV_NOTE_ON:
         val = XMIDI->rbs_BNK[ch];
         for (i=0;i<t;i++)
            if ((XMIDI->TIMB.tbr[i].t_bank == val) &&
               (XMIDI->TIMB.tbr[i].t_num == MIDI->event[1])) break;
         if (i == t)
            {
            XMIDI->TIMB.tbr[t].t_bank = val;
            XMIDI->TIMB.tbr[t].t_num = MIDI->event[1];
            XMIDI->TIMB.cnt++;
            }
         break;
      case EV_CONTROL:
         switch (MIDI->event[1])
            {
            case AIL_TIMB_BNK:
               XMIDI->timb_BNK[ch] = MIDI->event[2];
               break;
            }
         break;
      case EV_PROGRAM:
         for (i=0;i<t;i++)
            if ((XMIDI->TIMB.tbr[i].t_bank == XMIDI->timb_BNK[ch]) &&
               (XMIDI->TIMB.tbr[i].t_num == MIDI->event[1])) break;
         if (i == t)
            {
            XMIDI->TIMB.tbr[t].t_bank = XMIDI->timb_BNK[ch];
            XMIDI->TIMB.tbr[t].t_num = MIDI->event[1];
            XMIDI->TIMB.cnt++;
            }
         break;

      }

   return 0;
}

/***************************************************************/
void XMIDI_note_off(XMIDI *XMIDI,MIDI *MIDI)
{
   unsigned i,j;
   unsigned long duration,offset,len;
   unsigned char far *src, far *dest;
   unsigned channel,note;

   channel = MIDI->event_chan;
   note = MIDI->event[1];

   for (i=0;i<MAX_NOTES;i++)
      if ((XMIDI->note_chan[i] == channel) && (XMIDI->note_num[i] == note))
         {
         XMIDI->note_chan[i] = 255;

         duration = XMIDI->interval - XMIDI->note_intvl[i];
         offset = (unsigned long) MIDI_vln_size(duration) - 1L;

         if (offset)
            {
            dest = add_ptr(XMIDI->note_next[i],offset);
            src = XMIDI->note_next[i];
            len = ptr_dif(XMIDI->EVNT.ptr,XMIDI->note_next[i]);
            far_memmove(dest,src,len);
            XMIDI->EVNT.ptr = add_ptr(XMIDI->EVNT.ptr,offset);
            XMIDI->EVNT.len += offset;

            for (j=0;j<MAX_NOTES;j++)
               if (XMIDI->note_chan[j] != 255)
                  if (ptr_dif(XMIDI->note_next[j],src) >= 0L)
                     XMIDI->note_next[j] =
                        add_ptr(XMIDI->note_next[j],offset);

            for (j=0;j<XMIDI->RBRN.cnt;j++)
               if (ptr_dif(add_ptr(XMIDI->EVNT.base,
                  XMIDI->RBRN.brn[j].offset),src) >= 0L)
                     XMIDI->RBRN.brn[j].offset += offset;
            }

         MIDI_put_vln(duration,add_ptr(XMIDI->note_next[i],-1L));
         }
}

/***************************************************************/
unsigned XMIDI_note_on(XMIDI *XMIDI, MIDI *MIDI)
{
   unsigned i;
   unsigned char cb_event[3] = { 0xb9,119,0 };

   if ((c_opt) && (MIDI->event_chan == 9))
      {
      cb_event[2] = MIDI->event[1];
      XMIDI_put_explicit_event(XMIDI,cb_event,3);
      }

   XMIDI_put_MIDI_event(XMIDI,MIDI);
   IFF_put_byte(0x00,&XMIDI->EVNT);

   for (i=0;i<MAX_NOTES;i++)
      if (XMIDI->note_chan[i] == 255)
         {
         XMIDI->note_chan[i] = MIDI->event_chan;
         XMIDI->note_num[i] = MIDI->event[1];
         XMIDI->note_intvl[i] = XMIDI->interval;
         XMIDI->note_next[i] = XMIDI->EVNT.ptr;
         break;
         }

   return (i==MAX_NOTES);
}

/***************************************************************/
unsigned XMIDI_verify_notes(XMIDI *XMIDI)
{
   unsigned i;
   
   for (i=0;i<MAX_NOTES;i++)
      if (XMIDI->note_chan[i] != 255)
         return 8;

   return 0;
}

/***************************************************************/
unsigned XMIDI_IFF_write_blocks(XMIDI *XMIDI, FILE *out)
{
   static IFF_block far IFF_TIMB;
   static IFF_block far IFF_RBRN;

   IFF_TIMB.name = "TIMB";
   IFF_TIMB.base = (unsigned char far *) &XMIDI->TIMB; 
   IFF_TIMB.len = sizeof(TIMB_block) -
      ((MAX_TIMB - XMIDI->TIMB.cnt) * sizeof(TIMB_entry));

   IFF_RBRN.name = "RBRN";
   IFF_RBRN.base = (unsigned char far *) &XMIDI->RBRN;
   IFF_RBRN.len = sizeof(RBRN_block) -
      ((MAX_RBRN - XMIDI->RBRN.cnt) * sizeof(RBRN_entry));

   if (XMIDI->TIMB.cnt)
      if (!IFF_write_block(out,&IFF_TIMB)) return 1;

   if (XMIDI->RBRN.cnt)
      if (!IFF_write_block(out,&IFF_RBRN)) return 1;

   if (!IFF_write_block(out,&XMIDI->EVNT)) return 1;
   
   return 0;
}   

/***************************************************************/
void XMIDI_compile(char *src_fn, FILE *out, unsigned quant)
{
   static MIDI far MIDI;
   static XMIDI far XMIDI;

   MIDI.seq_fn = src_fn;
   MIDI.seq_len = file_size(MIDI.seq_fn);
   MIDI.base = read_file(MIDI.seq_fn,NULL);
   if (MIDI.base == NULL)
      abend(1,get_disk_error(),(unsigned long) MIDI.seq_fn);
   abend(MIDI_construct(&MIDI),(unsigned long) MIDI.seq_fn,0L);

   XMIDI.EVNT.avail = farcoreleft() - 16384L;
   XMIDI.EVNT.name = "EVNT";
   XMIDI.AILH.quantization = quant;
   XMIDI_construct(&XMIDI);

   while (MIDI_get_next_event(&MIDI))
      {
      if ((XMIDI.EVNT.avail - XMIDI.EVNT.len) < (MAX_EVLEN + 16))
         abend(7,(unsigned long) MIDI.seq_fn,0L);

      XMIDI_accum_interval(&XMIDI,&MIDI);

      switch (MIDI_event_type(&MIDI))
         {
         case EV_NOTE_ON:
            if (XMIDI.rbs_BNK[MIDI.event_chan]) 
               abend(XMIDI_log_timbre_request(&XMIDI,&MIDI),
                  (unsigned long) MIDI.seq_fn,0L);
            if (XMIDI_note_on(&XMIDI,&MIDI))
               abend(9,(unsigned long) MIDI.seq_fn,0L);
            break;

         case EV_NOTE_OFF:
            XMIDI_note_off(&XMIDI,&MIDI);
            break;

         case EV_CONTROL:
            XMIDI_put_MIDI_event(&XMIDI,&MIDI);
            switch (MIDI.event[1])
               {
               case AIL_BRANCH_PT:
                  abend(XMIDI_log_branch(&XMIDI,&MIDI),
                     (unsigned long) MIDI.seq_fn,0L);
                  break;
               case AIL_TIMB_BNK:
                  abend(XMIDI_log_timbre_request(&XMIDI,&MIDI),
                     (unsigned long) MIDI.seq_fn,0L);
                  break;
               }
            break;

         case EV_PROGRAM:
            abend(XMIDI_log_timbre_request(&XMIDI,&MIDI),
               (unsigned long) MIDI.seq_fn,0L);
            XMIDI_put_MIDI_event(&XMIDI,&MIDI);
            break;

         case EV_META:
            switch (MIDI.event[1])
               {
               case META_EOT:
               case META_TRK_NAME:
               case META_INS_NAME:
                  break;
               default:
                  XMIDI_put_MIDI_event(&XMIDI,&MIDI);
               }
            break;

         case EV_INVALID:
            break;

         default:
            XMIDI_put_MIDI_event(&XMIDI,&MIDI);
         }
      }

   XMIDI_write_interval(&XMIDI);
   IFF_put_byte(EV_META,&XMIDI.EVNT);
   IFF_put_byte(META_EOT,&XMIDI.EVNT);
   IFF_put_byte(0x00,&XMIDI.EVNT);

   abend(XMIDI_verify_notes(&XMIDI),(unsigned long) MIDI.seq_fn,0L);

   abend(XMIDI_IFF_write_blocks(&XMIDI,out),6L,0L);

   XMIDI_destroy(&XMIDI);
   MIDI_destroy(&MIDI);
}

/***************************************************************/
void main(int argc, char *argv[])
{
   int bad,strcnt;
   int infile;
   int i,done,seq_cnt;
   int x,y;
   unsigned quant;
   static char seq_fn[128];
   static char buff[5];
   FILE *tmp, *XMID;
   unsigned long info_len;
   unsigned long catlen_off,seqcnt_off;

   printf("\nMIDIFORM version %s               Copyright (C) 1991, 1992 Miles Design, Inc.\n",VERSION);
   printf("-------------------------------------------------------------------------------\n\n");

   strcpy(out_fn,"");
   bad=strcnt=infile=0;
   quant=DEFAULT_QUAN;
   c_opt = 0;

   for (i=1;i<argc;i++)
      if (argv[i][0] != '/')
         {
         ++strcnt;
         if (strcnt == 1)
            {
            strcpy(out_fn,argv[i]);
            }
         if (strcnt == 2)
            infile = i;
         }
      else
         if (!strnicmp(argv[i],"/Q:",3))
            {
            if (!infile)
               quant = (int) val((char far *)&argv[i][3],10);
            }
         else if (!strnicmp(argv[i],"/C",2))
            {
            c_opt = 1;
            }
         else
            {
            bad=1; break;
            }

   if (bad || (!strcnt))
      {
      printf("This program converts Standard MIDI Format 0 or Format 1 sequence files to\n");
      printf("the XMIDI format used by the Audio Interface Library Version 2.X drivers.\n\n");

      printf("Usage: MIDIFORM [/Q:nn] [/C] output_file [[input_file ...] | [< rspfile]]\n\n");

      printf("where /Q:nn = quantization rate in hertz (default=%u Hz.)\n",DEFAULT_QUAN);
      printf("      /C    = insert callback triggers before rhythm key notes\n");
      printf("    rspfile = list of newline-separated MIDI sequence files for input\n");
      exit(1);
      }

   if (file_size(out_fn) > 12L)
      {
      tmp = fopen(out_fn,"rb");
      fread(buff,4,1,tmp); buff[4] = 0;
      fclose(tmp);
      if (strcmp(buff,"FORM") && (strcmp(buff,"CAT ")))
         {
         printf("WARNING: Non-IFF file \"%s\" will be overwritten.\n",
            out_fn);
         printf("Continue? (Y/N) \a");
         if (toupper(getche()) == 'Y')
            printf("\n\n");
         else
            {
            printf("\n\nAborted\n");
            exit(1);
            }
         }
      }

   if (infile)
      printf("Converting, please wait ....\n\n");

   XMID = IFF_create_file(out_fn);
   if (XMID==NULL) abend(1,4L,(unsigned long) out_fn);

   seq_cnt = -1;

   mktemp(tmp_fn);
   tmp = fopen(tmp_fn,"w+b");
   if (tmp==NULL) abend(1,4L,(unsigned long) tmp_fn);
   fputs("INFO",tmp);
   info_len = wswap(2L);
   fwrite(&info_len,4L,1,tmp);
   fwrite(&seq_cnt,2L,1,tmp);
   fseek(tmp,0L,SEEK_SET);
   IFF_append_FORM(XMID,"XDIR",tmp);
   fflush(XMID);
   seqcnt_off = ftell(XMID);
   fclose(tmp);

   IFF_append_CAT(XMID,"XMID");
   catlen_off = ftell(XMID);

   done=seq_cnt=0;
   while (!done)
      {
      strcpy(seq_fn,"");
      if (infile)
         if (infile >= argc)
            done=1;
         else
            strcpy(seq_fn,argv[infile++]);
      else
         {
         locate(0,curpos_y());
         printf("Sequence filename (Enter to end): ");
         x = curpos_x();
         if (gets(seq_fn) == NULL) done=1;

         for (i=0;i<strlen(seq_fn);i++)
            if (seq_fn[i] == ';') seq_fn[i] = 0;

         if (!strlen(seq_fn)) done=1;

         if (curpos_x() == x) printf("%s\n",seq_fn);
         if (done)
            {
            locate(x,curpos_y() - 1);
            printf("Done\n");
            }
         }

      if (done) continue;

      if (!strnicmp(seq_fn,"/Q:",3))
         {
         quant = (int) val((char far *)&seq_fn[3],10);
         continue;
         }
      else if (!strnicmp(seq_fn,"/C",2))
         {
         c_opt = 1;
         continue;
         }

      mktemp(tmp_fn);
      tmp = fopen(tmp_fn,"w+b");
      if (tmp==NULL) abend(1,4L,(unsigned long) tmp_fn);

      XMIDI_compile(seq_fn,tmp,quant);
      fseek(tmp,0L,SEEK_SET);
      IFF_append_FORM(XMID,"XMID",tmp);

      fclose(tmp);
      unlink(tmp_fn);
      ++seq_cnt;
      }

   IFF_close_CAT(XMID,catlen_off);

   fseek(XMID,seqcnt_off-2L,SEEK_SET);
   fwrite(&seq_cnt,2L,1,XMID);

   IFF_close_file(XMID);
   if (!seq_cnt) unlink(out_fn);

   printf("%u sequence(s) converted.\n",seq_cnt);
   exit(0);
}
