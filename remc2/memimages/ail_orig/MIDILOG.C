//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   MIDILOG.C                                                           лл
//лл                                                                       лл
//лл   MIDI file event librarian                                           лл
//лл                                                                       лл
//лл   Usage: MIDILOG infile outfile [logfile]                             лл
//лл                                                                       лл
//лл   Provides for the translation of sequencer-accessible MIDI events    лл
//лл   into application-specific events.  Also strips any non-standard     лл
//лл   headers from MIDI files.                                            лл
//лл                                                                       лл
//лл   Version: 1.00 of 04-Jun-91 -- Initial version                       лл
//лл            1.01 of 30-Sep-91 -- MT-32 checksum bug fixed              лл
//лл                                                                       лл
//лл   Project: IBM Audio Interface Library                                лл
//лл    Author: John Miles                                                 лл
//лл                                                                       лл
//лл   C source compatible with Turbo C++ v1.0 or later                    лл
//лл                                                                       лл
//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   midilog.obj: midilog.c gen.h                                        лл
//лл      bcc -ml -c -v midilog.c                                          лл
//лл                                                                       лл
//лл   midilog.exe: midilog.obj gen.lib                                    лл
//лл      tlink @midilog.lls                                               лл
//лл                                                                       лл
//лл   Contents of MIDILOG.LLS:                                            лл
//лл     /c /v /x +                                                        лл
//лл     \bc\lib\c0l.obj +                                                 лл
//лл     midilog, +                                                        лл
//лл     midilog.exe, +                                                    лл
//лл     midilog.map, +                                                    лл
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

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <alloc.h>
#include <dos.h>
#include <stdlib.h>

#include "gen.h"

const char VERSION[] = "1.01";

struct lvalue
{
   int len;
   unsigned char bytes[16];
   unsigned char masks[16];
};

struct rvalue
{
   int len;
   unsigned char bytes[128];
   unsigned char masks[128];
};

struct function
{
   int len;
   unsigned char bytes[128];
   unsigned char masks[128];
};

struct MIDIevent
{
   int running_status;
   int track_continued;
   unsigned char far *org;
   unsigned char far *ptr;

   int len;
   unsigned char far *data;
};

int nspecs;
unsigned long xlat_cnt;
struct lvalue lval[128];
struct rvalue rval[128];
unsigned char linbuf[2048];

#define TOK_EVENT 0
#define TOK_META_EVENT 1
#define TOK_MT32_SYSEX_EVENT 2
#define NTOKS 3

const char *tokens[] = {
                        "event(",
                        "meta_event(",
                        "mt32_sysex_event("
                       };

unsigned char far *buffer,*bufpnt,*bufend;
unsigned long file_len;

/************************************************************/
void show_syntax(void)
{

   printf("This program translates sequencer-accessible MIDI events into application-\n");
   printf("specific MIDI events, and strips any non-standard headers and trailers from\n");
   printf("MIDI files.\n\n");
   printf("Usage: MIDILOG infile outfile [logfile]\n\n");
   printf("where infile = MIDI file containing header or events to be translated\n");
   printf("     outfile = standard MIDI output file with translated events\n");
   printf("     logfile = list of event translation rules for MIDI file\n\n");
   printf("Logfile values:\n\n");
   printf("     * $nn = hex byte.  Use 'x' to mask hex nibble in comparisons.\n");
   printf("       May be 1 or 2 digits in length.  Examples: $4, $69, $FF\n");
   printf("       Last nibble masked in search expr is restored to all 'x' nibbles in \n");
   printf("       replace expr.  Useful for preserving channel nibbles in replaced MIDI\n");
   printf("       status bytes.\n\n");
   printf("     * nnn = decimal byte.  May be 1-3 digits in length.  Examples: 4,105,255\n\n");
   printf("     * '...' / \"...\" = string literal value.  Examples: 'Test', \"Test\"\n\n");
   printf("     * ? = wildcard value.  Masks entire byte.  Last byte masked in search \n");
   printf("       expr is restored to all ? bytes in replace expr.  Useful for preserving \n");
   printf("       data bytes in replaced MIDI events.\n\n");
   printf("     * Character case is meaningful only in string literal values.\n\n");
   printf("     * Commas must be used to separate values.  Example: $FF,$7F,'Test'\n\n");
   printf("Logfile expressions:\n\n");
   printf("     * event( [val,] ): A MIDI event containing zero or more /val/ values.\n\n");
   printf("     * meta_event( type, [val,] ): Expresses a meta-event of type /type/.\n");
   printf("       Example: meta_event(1,\"Test\") is identical to event($FF,$01,$04,\"Test\")\n\n");
   printf("     * MT32_sysex_event( addr_MSB, addr, addr_LSB, data, [data,] ): Expresses a\n");
   printf("       type DT1 System Exclusive event to write data directly to Roland MT-32-\n");
   printf("       compatible synthesizers.  No nibble masks or wildcards may be used.\n");
   printf("       Example: MT32_sysex_event($20,$00,$00,'This is a test') causes the\n");
   printf("       string 'This is a test' to appear on the MT-32's front-panel display.\n");
   printf("       (For more information, see 'MT-32 MIDI Implementation' pp. 6-10.)\n\n");
   printf("Logfile syntax:\n\n");
   printf("     * Use = to separate search and replace exprs in event translation rules.\n");
   printf("       Example: event($Bx,120,0) = meta_event($01,\"Test\") causes Control Change\n");
   printf("       events which set controller 120 to 0 in any MIDI channel to be replaced\n");
   printf("       with Text meta-events containing the string \"Test\"\n\n");
   printf("     * If more than one event translation rule applies to a given event, \n");
   printf("       MIDILOG will use the first applicable rule to translate the event, \n");
   printf("       ignoring later rules.\n\n");
   printf("     * Use ; to flag comments.  Every line must contain an event translation\n");
   printf("       rule, a comment by itself, or whitespace (blank line).\n");
   printf("       All characters appearing after a ; are ignored.\n\n");
   printf("     * Up to 128 event translation rules may appear in each logfile.  Up to\n");
   printf("       16 bytes may appear in each rule's search expr; up to\n");
   printf("       128 bytes may appear in each rule's replace expr.\n\n");
   printf("Sample logfile:\n\n");
   printf("     ;                                                                       \n");
   printf("     ;SYSTEST.LOG                                                            \n");
   printf("     ;Sample event translation logfile -- turns SEQ.MID into APP.MID         \n");
   printf("     ;                                                                       \n");
   printf("     ;Usage: midilog seq.mid app.mid systest.log                             \n");
   printf("     ;                                                                       \n");
   printf("                                                                             \n");
   printf("     event($Bx,118,0) = meta_event($01,\"Second measure\")                   \n");
   printf("     event($Bx,119,1) = meta_event($02,\"Copyright (C) 1991\")               \n");
   printf("                                                                             \n");
   printf("     event($Bx,120,?) = meta_event($7E,?)                   ;AIL Track Index \n");
   printf("                                                                             \n");
   printf("     event($Bx,117,?) = event($F0,$43,$12,$00,$07,$F7)      ;Generic sysex   \n");
   printf("                                                                             \n");
   printf("     event($Bx,116,?) = MT32_sysex_event($10,$00,$01,1,7,5) ;Set MT-32 reverb\n\n");
}

/************************************************************/
void abend(int err, unsigned long info_1, unsigned long info_2)
{
   if (!err) return;

   fprintf(stderr,"Error ML%03u: ",err);

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
         fprintf(stderr,"Line %lu too long",info_1); break;
      case 3:
         fprintf(stderr,"Missing quotation mark in line %lu",info_1); break;
      case 4:
         fprintf(stderr,"Illegal function in line %lu",info_1); break;
      case 5:
         fprintf(stderr,"Byte value > 256 in line %lu",info_1); break;
      case 6:
         fprintf(stderr,"Expression syntax error in line %lu",info_1); break;
      case 7:
         fprintf(stderr,"> 128 expression values in line %lu",info_1); break;
      case 8:
         fprintf(stderr,"Expected '=' in line %lu",info_1); break;
      case 9:
         fprintf(stderr,"Extra characters in line %lu",info_1); break;
      case 10:
         fprintf(stderr,"Meta-event missing type specifier in line %lu",info_1); break;
      case 11:
         fprintf(stderr,"> 16 bytes in line %lu lvalue expression",info_1); break;
      case 12:
         fprintf(stderr,"File \"%s\" not a standard MIDI file",info_1); break;
      case 13:
         fprintf(stderr,"Empty lvalue in line %lu",info_1); break;
      case 14:
         fprintf(stderr,"Empty system exclusive event in line %lu",info_1); break;
      case 15:
         fprintf(stderr,"System exclusive event too long in line %lu",info_1); break;
      case 16:
         fprintf(stderr,"System exclusive event contains illegal mask in line %lu",info_1); break;
      case 17:
         fprintf(stderr,"> 128 event translation rules in logfile"); break;
      }                  
   
   fprintf(stderr,"\n");
   fcloseall();
   exit(err);
}

/***************************************************************/
unsigned isvalrst(unsigned val)
{
   return ((val >= 0x80) && (val < 0xf0));
}

unsigned get_chr(void)
{
   unsigned val;

   val = (unsigned) (*(unsigned char far *) bufpnt);

   if (FP_OFF(bufpnt) > 0x8000)
      bufpnt = add_ptr(bufpnt,1L);
   else
      ++bufpnt;
   return val;
}

unsigned next_chr(void)
{
   return (unsigned) (*(unsigned char far *) bufpnt);
}

unsigned long get_vln(void)
{
   unsigned long val=0L;
   unsigned i,cnt=4;

   do
      {
      i = get_chr();
      val = (val << 7) | (unsigned long) (i & 0x7f);
      if (!(i & 0x80))
         cnt = 0;
      else
         --cnt;
      }
   while (cnt);

   return val;
}

unsigned long get_ulong(void)
{
   unsigned long val;

   val = (unsigned long) get_chr();
   val = (val << 8) | (unsigned long) get_chr();
   val = (val << 8) | (unsigned long) get_chr();
   val = (val << 8) | (unsigned long) get_chr();

   return val;
}

unsigned long get_24(void)
{
   unsigned long val;

   val = (unsigned long) get_chr();
   val = (val << 8) | (unsigned long) get_chr();
   val = (val << 8) | (unsigned long) get_chr();

   return val;
}   

unsigned get_u(void)
{
   unsigned val;

   val = get_chr();
   val = (val << 8) | get_chr();

   return val;
}

void get_event(struct MIDIevent *dest)
{
   unsigned m,t,status,end_of_track;
   unsigned char far *bufsav;

   bufsav = bufpnt;
   bufpnt = dest->org = dest->ptr;
   end_of_track = 0;
   get_vln();

   dest->data = bufpnt;

   if (next_chr() >= 0x80)
      status = dest->running_status = get_chr();
   else
      if (!isvalrst(dest->running_status))
         do
            status = dest->running_status = get_chr();
         while (status < 0x80);
      else
         status = dest->running_status;

   switch (status & 0xf0)
      {
      case 0x80:
      case 0x90:
      case 0xa0:
      case 0xb0:
      case 0xe0:
         get_chr(); get_chr();
         break;
      case 0xc0:
      case 0xd0:
         get_chr();
         break;
      case 0xf0:
         switch (status)
            {
            case 0xf0:
            case 0xf7:
               t = get_vln();
               while (t--) get_chr();
               break;
            case 0xf1:
            case 0xf3:
               get_chr();
               break;
            case 0xf2:
               get_chr(); get_chr();
               break;
            case 0xff:
               m = get_chr();
               t = get_vln();
               if (m==0x2f) end_of_track = 1;
               while (t--) get_chr();
               break;
            }
         break;
      }

   dest->ptr = bufpnt;
   dest->len = (char huge *) (dest->ptr) - (char huge *) (dest->data);
   bufpnt = bufsav;
   dest->track_continued = !end_of_track;
}

void insert_explicit_status(unsigned char far *evptr, unsigned char xstat)
{
   unsigned char far *bufsav;

   bufsav = bufpnt;
   bufpnt = evptr;
   get_vln();

   far_memmove(add_ptr(bufpnt,1L),bufpnt,(char huge *) bufend - (char huge *)
      bufpnt + 1L);
   *bufpnt = xstat;
 
   bufpnt = bufsav;
}

/************************************************************/
int expr_val(char *pfn, unsigned *index, struct function *out)
{
   unsigned t;
   int c,i,j,m,n,v,done,csum;
   int qp,qt;
   char *p,*fn;
   static char hex[] = "0123456789ABCDEFX";

   fn = &pfn[*index];

   for (t=0;t<NTOKS;t++)
      {
      i = strlen(tokens[t]);
      if (!strnicmp(fn,tokens[t],i)) break;
      }
   if (t==NTOKS) return 4;

   out->len=0;
   if (t==TOK_META_EVENT)
      {
      out->bytes[0] = 0xff;
      out->masks[0] = 0xff;
      out->len = 2;
      }

   if (t==TOK_MT32_SYSEX_EVENT)
      {
      out->bytes[0] = 0xf0; out->masks[0] = 0xff;  // exclusive status
      out->bytes[1] = 0x00; out->masks[1] = 0xff;  // space for len
      out->bytes[2] = 0x41; out->masks[2] = 0xff;  // Roland ID
      out->bytes[3] = 0x10; out->masks[3] = 0xff;  // MT-32 device ID
      out->bytes[4] = 0x16; out->masks[4] = 0xff;  // MT-32 model ID
      out->bytes[5] = 0x12; out->masks[5] = 0xff;  // MT-32 DT1 command ID
      out->len = 6;
      }

   done=qp=qt=0;
   while (!done)
      {
      c = fn[i];
      
      if (qp)
         {
         if (c==qt)
            qp=0;
         else
            {
            if (out->len == 128) return 7;
            out->bytes[out->len] = c;
            out->masks[out->len] = 0xff;
            ++out->len;
            }
         i++;
         continue;
         }

      if (isdigit(c)) c='0';
      switch (c)
         {
         case '0':
            if (out->len == 128) return 7;
            n = 0;
            while (isdigit(fn[i]))
               {
               n = (n * 10) + (fn[i]-'0');
               if (n >= 256) return 5;
               i++;
               }
            out->bytes[out->len] = n;
            out->masks[out->len] = 0xff;
            ++out->len;
            break;
         case '$':
            if (out->len == 128) return 7;
            ++i;
            n = 0; m = 0xff;
            while ((p=strchr(hex,toupper(fn[i]))) != NULL)
               {
               m = (m*16) & 0xff; n = n*16; v=(p-hex);
               if (v!=16)
                  {
                  m += 0x0f;
                  n += v;
                  }
               if (n >= 256) return 5;
               i++;
               }
            out->bytes[out->len] = n;
            out->masks[out->len] = m;
            ++out->len;
            break;
         case '?':
            if (out->len == 128) return 7;
            i++;
            out->bytes[out->len] = 0;
            out->masks[out->len] = 0;
            ++out->len;
            break;
         case '\'':
         case '\"':
            qt=c;
            qp=1;
            i++;
            break;
         case ')':
            done=1;
            i++;
            break;
         case ',':
            i++;
            break;
         default:
            return 6;
         }
      }

   if (t==TOK_META_EVENT)
      {
      if (out->len==2) return 10;

      out->bytes[1] = out->bytes[2];
      out->masks[1] = out->masks[2];
      out->bytes[2] = out->len - 3;;
      out->masks[2] = 0xff;
      }

   if (t==TOK_MT32_SYSEX_EVENT)
      {
      if (out->len==6) return 14;
      if (out->len > 125) return 15;
      out->bytes[1] = out->len;

      for (j=6,csum=0;j<out->len;j++)
         {
         csum += out->bytes[j];
         if (out->masks[j] != 0xff) return 16;
         }

      csum = 0 - ((csum & 0x7f) - 0x80);

      out->bytes[out->len] = (csum & 0x7f);
      out->masks[out->len] = 0xff;
      ++out->len;
      out->bytes[out->len] = 0xf7;
      out->masks[out->len] = 0xff;
      ++out->len;
      }

   (*index) += i;
   return 0;
}

/************************************************************/
void parse_logfile(char *filename)
{
   FILE *in;
   unsigned i,j,k,l,chr,line;
   int qp,qt;
   struct function lv,rv;

   in = fopen(filename,"rt");
   if (in==NULL) abend(1,3,(unsigned long) filename);

   line=0;
   while (fgets(linbuf,2048,in) != NULL)
      {
      ++line;
      l = strlen(linbuf);
      if (l==2048) abend(2,line,0);
      
      for (i=j=qp=0;i<l;i++)
         {                                
         if (qp)
            {
            if (linbuf[i] == qt) qp = 0;
            }
         else 
            {
            if (isspace(linbuf[i])) continue;
            if (linbuf[i] == ';') break;

            if ((linbuf[i] == '\'') || (linbuf[i] == '\"'))
               {
               qp = 1; qt = linbuf[i];
               }
            }
         linbuf[j++] = linbuf[i];
         }
      linbuf[j] = 0; l=j;

      if (qp) abend(3,line,0);
      if (!l) continue;

      if (nspecs >= 128) abend(17,0,0);

      chr = 0;
      abend(expr_val(linbuf,&chr,&lv),line,0);
      if (linbuf[chr++] != '=') abend(8,line,0);
      if (lv.len > 16) abend(11,line,0);
      if (lv.len == 0) abend(13,line,0);
      abend(expr_val(linbuf,&chr,&rv),line,1);
      if (linbuf[chr]) abend(9,line,0);

      lval[nspecs].len = lv.len;
      for (i=0;i<lv.len;i++)
         {
         lval[nspecs].bytes[i] = lv.bytes[i];
         lval[nspecs].masks[i] = lv.masks[i];
         }

      rval[nspecs].len = rv.len;
      for (i=0;i<rv.len;i++)
         {
         rval[nspecs].bytes[i] = rv.bytes[i];
         rval[nspecs].masks[i] = rv.masks[i];
         }
      ++nspecs;
      }

   if (ferror(in)) abend(1,5,(unsigned long) filename);

   fclose(in);
}

/************************************************************/
void replace_event(struct MIDIevent *event, unsigned long far *chunk_len_addr)
{
   unsigned match,xstat,orig_stat,new_stat,rv_index,mval,nval;
   unsigned i,j,k,l,m,s;
   unsigned long chunk_len;
   struct rvalue rv;
   struct MIDIevent temp;
   long dsize;

   chunk_len = wswap(*chunk_len_addr);

   for (s=0;s<nspecs;s++)
      {
      orig_stat = event->running_status;

      if ((orig_stat & lval[s].masks[0]) != lval[s].bytes[0])
         continue;
      xstat = (orig_stat == event->data[0]);

      for (j=xstat,k=1,l=lval[s].len-1;l;j++,k++,l--)
         if ((event->data[j] & lval[s].masks[k]) != lval[s].bytes[k])
            break;
      if (l) continue;
      ++xlat_cnt;

      m = lval[s].masks[0];
      if (m != 0xff)
         if (m == 0)
            mval = orig_stat;
         else
            nval = orig_stat & (~m);
      for (j=xstat,k=1,l=lval[s].len-1;l;j++,k++,l--)
         {
         m = lval[s].masks[k];
         if (m != 0xff)
            if (m == 0)
               mval = event->data[j++];
            else
               nval = event->data[j++] & (~m);
         }
      nval |= (nval & 0x0f)? (nval << 4) : (nval >> 4);
      rv = rval[s];
      for (i=0;i<rv.len;i++)
         {
         m = rv.masks[i];
         if (m != 0xff)
            if (m == 0)
               rv.bytes[i] = mval;
            else
               rv.bytes[i] |= (nval & (~m));
         }
      new_stat = (rv.len)? rv.bytes[0] : 0;

      if ((event->track_continued) && (isvalrst(orig_stat)) && (orig_stat
         != new_stat))
         {
         temp = *event;
         get_event(&temp);
         if (temp.running_status != temp.data[0])
            {
            insert_explicit_status(event->ptr,orig_stat);
            bufend = add_ptr(bufend,1L);
            ++chunk_len;
            }
         }

      rv_index = (orig_stat == new_stat) && (!xstat);
      rv.len -= rv_index;

      if (!rv.len)      
         {
         dsize = (char huge *) event->org - (char huge *) event->ptr;
         far_memmove(event->org,event->ptr,(char huge *) bufend -
            (char huge *) (event->ptr) + 1L);
         }
      else
         {
         dsize = rv.len - event->len;
         far_memmove(add_ptr(event->data,rv.len),event->ptr,(char huge *) 
            bufend - (char huge *) (event->ptr) + 1L);
         far_memmove(event->data,&rv.bytes[rv_index],rv.len);
         }

      event->ptr = add_ptr(event->ptr,dsize);
      event->len = rv.len;
      event->running_status = new_stat;
      bufend = add_ptr(bufend,dsize);
      chunk_len += dsize;

      break;
      }

   *chunk_len_addr = wswap(chunk_len);
}


/************************************************************/
void main(int argc, char *argv[])
{
   unsigned i,done,trk,ntrks;
   unsigned long l,chunk_len;
   char far *chunk_beg;
   unsigned long far *chunk_len_addr;
   char chunk_title[8];
   struct MIDIevent event;
   unsigned long bsize;

   printf("\nMIDILOG version %s                Copyright (C) 1991, 1992 Miles Design, Inc.\n",VERSION);
   printf("-------------------------------------------------------------------------------\n\n");

   if ((argc != 3) && (argc != 4))
      {
      show_syntax();
      exit(1);
      }

   nspecs=0;
   xlat_cnt=0L;
   if (argc == 4) parse_logfile(argv[3]);

   printf("Available memory %lu\n\n",bsize=farcoreleft()-16384L);

   if (argc == 4) printf("Translating ... ");

   buffer = read_file(argv[1],farmalloc(bsize));
   if (buffer==NULL)
      abend(1,get_disk_error(),(unsigned long) argv[1]);

   file_len = file_size(argv[1]);
   bufend = add_ptr(buffer,file_len);

   // find the MIDI header chunk (deleting any proprietary headers)

   for (l=file_len-4L;l;l--)
      {
      if ((toupper(*buffer) == 'M') &&
      (toupper(*(buffer+1)) == 'T') &&
      (toupper(*(buffer+2)) == 'H') &&
      (toupper(*(buffer+3)) == 'D')) break;
      buffer = add_ptr(buffer,1L);
      }
   if (!l) abend(12,(unsigned long) argv[1],0);

   bufpnt = buffer;

   // read # of track chunks from MIDI header, index the first track chunk

   for (i=0;i<4;i++) get_chr();
   chunk_len = get_ulong();
   chunk_beg = bufpnt;
   get_u();
   ntrks = get_u();
   bufpnt = add_ptr(chunk_beg,chunk_len);

   // scan each track for logged events, ignoring non-MTrk chunks

   for (trk=0;trk<ntrks;trk++)
      {
      for (i=0;i<4;i++) chunk_title[i] = get_chr(); chunk_title[i] = 0;
      chunk_len_addr = (unsigned long far *) bufpnt;
      get_ulong();
      chunk_beg = bufpnt;

      if (stricmp(chunk_title,"MTrk"))
         {
         bufpnt = add_ptr(chunk_beg,wswap(*chunk_len_addr));
         trk--;
         continue;
         }

      event.running_status = 0;
      event.ptr = bufpnt;

      do
         {
         get_event(&event);
         replace_event(&event,chunk_len_addr);
         }
      while (event.track_continued);

      bufpnt = add_ptr(chunk_beg,wswap(*chunk_len_addr));
      }

   if (argc == 4) printf("%lu event(s) translated\n\n",xlat_cnt);

   file_len = (char huge *) bufpnt - (char huge *) buffer;
   printf("Writing %lu bytes\n",file_len);
   if (!write_file(argv[2],buffer,file_len))
      abend(1,get_disk_error(),(unsigned long) argv[2]);

   exit(0);
}


