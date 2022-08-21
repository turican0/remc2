//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   GLIB.C                                                              лл
//лл                                                                       лл
//лл   Global Timbre Librarian                                             лл
//лл                                                                       лл
//лл   Usage: GLIB catfile                                                 лл
//лл                                                                       лл
//лл   Creates standard Audio Interface Library instrument bankfiles which лл
//лл   contain instruments assigned to the Patch Bank Select/Patch Change  лл
//лл   values embedded in XMIDI data files                                 лл
//лл                                                                       лл
//лл   Version: 1.00 of 05-Oct-91 -- Initial version                       лл
//лл            1.01 of 03-Feb-92 -- Moved header member out of GTL_class  лл
//лл            1.02 of 15-Feb-92 -- Ad Lib Gold OPL3 bankfiles supported  лл
//лл            1.03 of 25-May-92 -- Fixed GTL_write_MTB() mem alloc bug   лл
//лл                                 Added #include <io.h>                 лл
//лл            1.04 of 11-Dec-92 -- Memory allocation for multiple .BNK   лл
//лл                                 files fixed                           лл
//лл            1.05 of 19-May-93 -- Amplitude/pitch vibrato bits switched лл
//лл                                                                       лл
//лл   Project: IBM Audio Interface Library                                лл
//лл    Author: John Miles                                                 лл
//лл                                                                       лл
//лл   C source compatible with Turbo C++ v1.0 or later, large model       лл
//лл                                                                       лл
//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   glib.obj: glib.c gen.h                                              лл
//лл      bcc -ml -c -v glib.c                                             лл
//лл                                                                       лл
//лл   glib.exe: glib.obj gen.lib                                          лл
//лл      tlink @glib.lls                                                  лл
//лл                                                                       лл
//лл   Contents of GLIB.LLS:                                               лл
//лл     /c /v /x +                                                        лл
//лл     \bc\lib\c0l.obj +                                                 лл
//лл     glib, +                                                           лл
//лл     glib.exe, +                                                       лл
//лл     glib.map, +                                                       лл
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

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <alloc.h>
#include <dos.h>
#include <stdlib.h>
#include <io.h>

#include "gen.h"

#pragma warn -par
#pragma warn -aus

#define OSI_ALE 0    // 1 to support Origin Systems Ad Lib Effects, else 0

const char VERSION[] = "1.05";

#define SIG_LEN 28   // Ad Lib Gold instrument maker equates
#define KEY_SIZE 15
#define SIGNATURE "Accomp. Bank, (C) AdLib Inc"

typedef struct
{
   unsigned t;
   unsigned len;
   unsigned comcnt;
   char bytes[256];
}
function;

unsigned char linbuf[2048];

typedef struct
{
   unsigned char patch;
   unsigned char bank;
   unsigned long offset;
}
GTL_hdr;

typedef struct
{
   char *filename;
   FILE *body;
   FILE *out;
   unsigned entries;
   unsigned tpatch;
   unsigned tbank;
   unsigned char *timb;
   unsigned tlen;
}
GTL_class;

GTL_hdr huge header[16384];

typedef struct
{
   char maj_vers;
   char min_vers;
   char signature[6];
   unsigned ins_used;
   unsigned ins_entries;
   unsigned long name_list;
   unsigned long inst_data;
}
bnk1_hdr;

typedef struct    // cf. ACCBNK.C, Marc Savary, Ad Lib Inc.
{
   char signature[SIG_LEN];
   unsigned fileVersion;
   char filler[10];
   unsigned allocIdxEntry;    // # entries in file header
   unsigned usedIdxEntry;     // # entries in use
   long lostSpace;
   /* char index[...] */
   /* char data[...] */
}
bnk2_hdr;

typedef struct    // cf. ACCBNK.C, Marc Savary, Ad Lib Inc.
{
   char O3_sig[3];
   char key[KEY_SIZE-3];
   char used;
   long attrib;
   long dataOffset;
   unsigned blockSize;
   unsigned allocBSize;
}
IndexRec;

typedef struct 
{
   unsigned char mod_avekm;
   unsigned char mod_ksl_tl;
   unsigned char mod_ad;
   unsigned char mod_sr;
   unsigned char mod_ws;

   unsigned char fb_c;

   unsigned char car_avekm;
   unsigned char car_ksl_tl;
   unsigned char car_ad;
   unsigned char car_sr;
   unsigned char car_ws;
}
AD_instrument;

typedef struct
{
   AD_instrument a0;
   AD_instrument a1; // fb_c = don't care
}
OPL3_instrument;

typedef struct 
{
   unsigned char sound_mode;
   unsigned char voice_num;
   unsigned char mod_key_scale_lvl;
   unsigned char mod_freq_mult;
   unsigned char mod_feedback;
   unsigned char mod_attack;
   unsigned char mod_sustain_lvl;
   unsigned char mod_sustain_sound;
   unsigned char mod_decay;
   unsigned char mod_release;
   unsigned char mod_out_lvl;
   unsigned char mod_amp_vib;
   unsigned char mod_pitch_vib;
   unsigned char mod_env_scaling;
   unsigned char mod_connection;
   unsigned char car_key_scale_lvl;
   unsigned char car_freq_mult;
   unsigned char car_feedback;
   unsigned char car_attack;
   unsigned char car_sustain_lvl;
   unsigned char car_sustain_sound;
   unsigned char car_decay;
   unsigned char car_release;
   unsigned char car_out_lvl;
   unsigned char car_amp_vib;
   unsigned char car_pitch_vib;
   unsigned char car_env_scaling;
   unsigned char car_connection;
   unsigned char mod_waveform;
   unsigned char car_waveform;
}
BNK1_instrument;     // conventional Ad Lib instrument maker bankfile patch

typedef struct
{
   unsigned char AVEKMMMM;
   unsigned char KKLLLLLL;
   unsigned char AAAADDDD;
   unsigned char SSSSRRRR;
   unsigned char DxxxxWWW;
   unsigned char xxxxxxxx;
}
OPL3_op;

typedef struct
{
   OPL3_op ops[4];
   unsigned char C4xxxFFFC;
   unsigned char xxP24NNN;
   unsigned char TTTTTTTT;
   unsigned char xxxxxxxx;
}
BNK2_instrument;     // Ad Lib Gold instrument maker bankfile patch

typedef struct 
{
   unsigned index;
   unsigned char used;
   char name[9];
}
nsec;

typedef struct
{
   const char *name;
   const int lval_OK,rval_OK,rval_req;
   const unsigned argmax,argmin;
}
tok_class;

const tok_class tokens[] = {
                           "timbre(",1,0,1,2,2,
                           "BNK_file(",1,0,0,-1,1,
                           "BNK_inst(",0,1,0,-1,2,
                           "MTB_file(",0,1,0,-1,1,
                           "BIN_file(",0,1,0,-1,1,
                           "outfile(",1,0,0,-1,1,
#if OSI_ALE
                           "ALE_file(",1,0,0,-1,1,
                           "ALE_inst(",0,1,0,-1,2,
                           "ALE_effect(",0,1,0,-1,1,
#endif
                           };

#define TOK_TIMBRE 0
#define TOK_BNK_FILE 1
#define TOK_BNK_INST 2
#define TOK_MTB_FILE 3
#define TOK_BIN_FILE 4
#define TOK_OUTFILE 5

#if OSI_ALE
#define TOK_ALE_FILE 6
#define TOK_ALE_INST 7
#define TOK_ALE_EFFECT 8
#define NTOKS 9
#else
#define NTOKS 6
#endif

unsigned char *inbuf;
unsigned long data_len;

const char *tempname = "GTLTEMP.$$$";

/************************************************************/
void abend(int err, unsigned long info_1, unsigned long info_2)
{
   if (!err) return;

   fprintf(stderr,"Error GT%03u: ",err,info_1);

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
         fprintf(stderr,"Illegal expression in line %lu",info_1); break;
      case 5:
         fprintf(stderr,"Byte value > 256 in line %lu",info_1); break;
      case 6:
         fprintf(stderr,"Syntax error in line %lu",info_1); break;
      case 7:
         fprintf(stderr,"Expression too long in line %lu",info_1); break;
      case 8:
         fprintf(stderr,"Expected '=' in line %lu",info_1); break;
      case 9:
         fprintf(stderr,"Extra characters in line %lu",info_1); break;
      case 10:
         fprintf(stderr,"Duplicate timbre assignment in line %lu",info_1); break;
      case 11:
         fprintf(stderr,"Too many expression values in line %lu",info_1); break;
      case 12:
         fprintf(stderr,"File \"%s\" not a Big Noise MT-32 Editor/Librarian file",info_1); break;
      case 13:
         fprintf(stderr,"Missing expression(s) in line %lu",info_1); break;
      case 14:
         fprintf(stderr,"File \"%s\" not a recognized Ad Lib-compatible .BNK file",info_1); break;
      case 15:
         fprintf(stderr,"Instrument \"%s\" not found in bankfile",info_1); break;
      case 16:
         fprintf(stderr,"Sound effect \"%s\" not found in library",info_1); break;
      case 17:
         fprintf(stderr,"Too many timbres"); break;
      case 18:
         fprintf(stderr,"DOS error occurred while creating output file"); break;
      case 20:
         fprintf(stderr,"Couldn't read timbre file \"%s\"",info_1); break;
      case 21:
         fprintf(stderr,"Outfile name must be specified before any timbre assignments"); break;
      case 22:
         fprintf(stderr,"\"%s\" not an Ad Lib melodic mode instrument",info_1); break;
      }                  
   fprintf(stderr,"\n");
   fcloseall();
   unlink(tempname);
   exit(err);
}

/************************************************************/
void show_syntax(void)
{

   printf("This program compiles standard AIL 2.0 Global Timbre Library files containing\n");
   printf("instruments assigned to various MIDI Program Change (patch) values.\n\n");

   printf("Usage: GLIB catfile\n\n");
   printf("where catfile = ASCII catalog listing containing output filenames,\n");
   printf("                timbre assignments, and source data filenames\n\n");
   printf("Catfile values:\n\n");
   printf("     * $nn = hex byte, 1 or 2 digits in length.  Examples: $4,$69,$FE,$FF\n");
   printf("     * nnn = decimal byte, 1-3 digits in length.  Examples: 4,105,254,-1\n");
   printf("     * '...' / \"...\" = string literal value.  Examples: 'DEFAULT.MTB', \"Tuba\"\n");
   printf("     * Character case is not meaningful.\n\n");
   printf("Catfile declarative expressions:\n\n");
   printf("     * outfile ( file ): Specifies the name of a Global Timbre Library file\n");
   printf("       to create.  Must appear in the catfile before any timbres are\n");
   printf("       assigned to the outfile.\n\n");
   printf("     * BNK_file ( file ): Specifies the Ad Lib Instrument Maker bankfile from \n");
   printf("       which Ad Lib and other Yamaha-based instruments should be read.\n");
   printf("       Compatible with both Ad Lib (YM3812) and Ad Lib Gold (OPL3/YMF262)\n");
   printf("       bankfiles.\n\n");

#if OSI_ALE
   printf("     * ALE_file ( file ): Specifies the Ad Lib Effects library file from which\n");
   printf("       Ad Lib time-variant sound-effects are to be read.\n\n");
#endif
   printf("Catfile lvalue expressions:\n\n");
   printf("     * timbre( bank, patch ): Indicates that the next expression (\"rvalue\")\n");
   printf("       specifies the timbre source for the XMIDI Patch Bank Request controller\n");
   printf("       value /bank/, to be accessed via the MIDI Program Change number /patch/.\n\n");
   printf("Catfile rvalue expressions:\n\n");
   printf("     * BNK_inst( string, num ): Refers to the name of an Ad Lib instrument to\n");
   printf("       be found in a previously specified BNK_file.  num specifies the number\n");
   printf("       of semitones by which to transpose notes played with the instrument.  If\n");
   printf("       the instrument is to be played in MIDI channel 10, num specifies its\n");
   printf("       desired absolute MIDI note number.\n\n");

#if OSI_ALE
   printf("     * ALE_inst( string, num ): Refers to the title of an Ad Lib Effects\n");
   printf("       instrument to be found in a previously specified ALE_file. num specifies\n");
   printf("       the number of semitones by which to transpose notes played with the \n");
   printf("       instrument.  If the instrument is to be played in MIDI channel 10, \n");
   printf("       num specifies its desired absolute MIDI note number.\n\n");
   printf("     * ALE_effect( string ): Refers to the title of an Ad Lib Effects\n");
   printf("       sound effect to be found in a previously specified ALE_file.\n\n");
#endif

   printf("     * MTB_file( file ): Refers to the name of a Big Noise MT-32\n");
   printf("       Editor/Librarian file containing 64 timbres.\n\n");
   printf("     * BIN_file( file ): Refers to the name of a binary file containing a\n");
   printf("       single timbre image in the target AIL driver's native format.\n\n");
   printf("Catfile syntax:\n\n");
   printf("     * Use = to separate lvalue and rvalue expressions.\n\n");
   printf("       Example: timbre(1,0) = MTB_file(\"MUSIC.MTB\")\n");
   printf("                timbre(1,64) = MTB_file(\"SOUNDFX.MTB\")\n");
   printf("                timbre(0,1) = BNK_inst(\"AcouPian\",0)\n\n");
   printf("     * Use ; to flag comments.  Every line must contain a single timbre\n");
   printf("       assignment or declaration, a comment by itself, or whitespace.\n");
   printf("       All characters appearing after a ; are ignored.\n\n");
   printf("Sample catfile:\n\n");
   printf("     ;                                                                       \n");
   printf("     ;ULTIMA26.CAT                                                           \n");
   printf("     ;Global Timbre Library assignments for Ultima XXVI                      \n");
   printf("     ;                                                                       \n");
   printf("     ;Usage: C:\>glib ultima26.cat                                         \n");
   printf("     ;                                                                       \n");
   printf("                                                                             \n");
   printf("     outfile(\"ultima26.mt\")                ;create Roland library       \n");
   printf("                                                                          \n");
   printf("     timbre(1,0) = MTB_file(\"roland.mtb\")  ;copy 64 timbres at bank 1, patch 0\n");
   printf("                                                                          \n");
   printf("     outfile(\"ultima26.ad\")                ;create Ad Lib library       \n");
   printf("                                                                          \n");
   printf("     BNK_file(\"adlib.bnk\")                 ;get instruments from ADLIB.BNK\n");
   printf("     timbre(0,86) = BNK_inst(\"Bassoon2\",0)\n");
   printf("     timbre(0,81) = BNK_inst(\"Softsax\",0)\n");
   printf("     timbre(0,77) = BNK_inst(\"Flute2\",12)\n");
   printf("                                                                             \n");
}

/***************************************************************/
int expr_val(char *pfn, unsigned *index, function *out)
{
   unsigned t;
   int c,i,j,m,n,v,done,csum;
   int qp,qt;
   char *p,*fn;
   int neg_val;
   static char hex[] = "0123456789ABCDEF";

   fn = &pfn[*index];

   for (t=0;t<NTOKS;t++)
      {
      i = strlen(tokens[t].name);
      if (!strnicmp(fn,tokens[t].name,i)) break;
      }
   if (t==NTOKS) return 4;

   out->t=t;
   out->len=out->comcnt=0;

   done=qp=qt=neg_val=0;
   while (!done)
      {
      c = fn[i];
      
      if (qp)
         {
         if (c==qt)
            qp=0;
         else
            {
            if (out->len == 256) return 7;
            out->bytes[out->len] = c;
            ++out->len;
            }
         i++;
         continue;
         }

      if (isdigit(c)) c='0';
      switch (c)
         {
         case '0':
            if (out->len == 256) return 7;
            n = 0;
            while (isdigit(fn[i]))
               {
               n = (n * 10) + (fn[i]-'0');
               if (n >= 256) return 5;
               i++;
               }
            if (neg_val)
               {
               neg_val = 0;
               n = (256 - n) & 0xff;
               }
            out->bytes[out->len] = n;
            ++out->len;
            break;
         case '$':
            if (out->len == 256) return 7;
            ++i;
            n = 0;
            while ((p=strchr(hex,toupper(fn[i]))) != NULL)
               {
               n = (n*16) + (p-hex);
               if (n >= 256) return 5;
               i++;
               }
            if (neg_val)
               {
               neg_val = 0;
               n = (256 - n) & 0xff;
               }
            out->bytes[out->len] = n;
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
            out->comcnt++;
            break;
         case '-':
            neg_val = !neg_val;
            i++;
            break;
         default:
            return 6;
         }
      }

   (*index) += i;
   return 0;
}

/***************************************************************/
char *make_str(char *str, function *fn, unsigned resv)
{
   int i;

   for (i=0;i<((fn->len)-resv);i++)
      str[i] = fn->bytes[i];
   str[i] = 0;

   return str;
}

int get_arg(function *fn, unsigned argnum)
{
   return fn->bytes[fn->len - (argnum+1)];
}

/************************************************************/
#if OSI_ALE

unsigned char *ALE_find_instrument(char *AFX, char *ADT, char *title)
{
   unsigned i,icnt;
   char *name;
   char *inst;

   icnt = *(unsigned *) ADT;
   name = ADT+2;
   inst = AFX;

   for (i=1;i<=icnt;i++)
      {
      if (!stricmp(name,title)) break;
      name += (strlen(name) + 1);
      inst += *(unsigned *) inst;
      }

   if (i > icnt) return NULL;

   return inst;
}

#endif

/************************************************************/
unsigned BNK_type(char far *ptr)     
{                                    // 1:old 2:new (OPL3) 0:bad
   if (!strnicmp("ADLIB-",((bnk1_hdr *) ptr)->signature,6))
      return 1;

   if (!stricmp(ptr,SIGNATURE))
      return 2;

   return 0;
}

/************************************************************/
BNK2_instrument *BNK2_find_instrument(bnk2_hdr *bhdr, char *title)
{
   IndexRec *ns;
   unsigned n;

   for (n=0;n<bhdr->allocIdxEntry;n++)
      {
      ns = add_ptr(bhdr,sizeof(bnk2_hdr)+(n * sizeof(IndexRec)));
      if (!ns->used) continue;

      if (!stricmp(title,ns->key))
         return add_ptr(bhdr,ns->dataOffset);
      }

   return NULL;
}

/************************************************************/
BNK1_instrument *BNK1_find_instrument(bnk1_hdr *bhdr, char *title)
{
   nsec *ns;
   unsigned n;

   for (n=0;n<bhdr->ins_entries;n++)
      {
      ns = add_ptr(bhdr,bhdr->name_list + (n * sizeof(nsec)));
      if (!ns->used) continue;

      if (!stricmp(title,ns->name))
         return add_ptr(bhdr,bhdr->inst_data + (ns->index *
            sizeof(BNK1_instrument)));
      }

   return NULL;
}

/************************************************************/
OPL3_instrument *BNK2_to_OPL3(BNK2_instrument *src)
{
   int i;
   static OPL3_instrument dest;

   for (i=sizeof(dest)-1;i>=0;i--)
      *(((char *) &dest)+i) = 0;

   dest.a0.mod_avekm = src->ops[0].AVEKMMMM;
   dest.a0.mod_ksl_tl = src->ops[0].KKLLLLLL;
   dest.a0.mod_ad = src->ops[0].AAAADDDD;
   dest.a0.mod_sr = src->ops[0].SSSSRRRR;
   dest.a0.mod_ws = src->ops[0].DxxxxWWW & 0x07;

   dest.a0.fb_c = src->C4xxxFFFC & 0x8f;

   dest.a0.car_avekm = src->ops[1].AVEKMMMM;
   dest.a0.car_ksl_tl = src->ops[1].KKLLLLLL;
   dest.a0.car_ad = src->ops[1].AAAADDDD;
   dest.a0.car_sr = src->ops[1].SSSSRRRR;
   dest.a0.car_ws = src->ops[1].DxxxxWWW & 0x07;

   dest.a1.mod_avekm = src->ops[2].AVEKMMMM;
   dest.a1.mod_ksl_tl = src->ops[2].KKLLLLLL;
   dest.a1.mod_ad = src->ops[2].AAAADDDD;
   dest.a1.mod_sr = src->ops[2].SSSSRRRR;
   dest.a1.mod_ws = src->ops[2].DxxxxWWW & 0x07;

   dest.a1.fb_c = 0;

   dest.a1.car_avekm = src->ops[3].AVEKMMMM;
   dest.a1.car_ksl_tl = src->ops[3].KKLLLLLL;
   dest.a1.car_ad = src->ops[3].AAAADDDD;
   dest.a1.car_sr = src->ops[3].SSSSRRRR;
   dest.a1.car_ws = src->ops[3].DxxxxWWW & 0x07;

   return &dest;
}

AD_instrument *BNK2_to_AD(BNK2_instrument *src)
{
   int i;
   static AD_instrument dest;

   for (i=sizeof(dest)-1;i>=0;i--)
      *(((char *) &dest)+i) = 0;

   dest.mod_avekm = src->ops[0].AVEKMMMM;
   dest.mod_ksl_tl = src->ops[0].KKLLLLLL;
   dest.mod_ad = src->ops[0].AAAADDDD;
   dest.mod_sr = src->ops[0].SSSSRRRR;
   dest.mod_ws = src->ops[0].DxxxxWWW & 0x07;

   dest.fb_c = src->C4xxxFFFC & 0x0f;

   dest.car_avekm = src->ops[1].AVEKMMMM;
   dest.car_ksl_tl = src->ops[1].KKLLLLLL;
   dest.car_ad = src->ops[1].AAAADDDD;
   dest.car_sr = src->ops[1].SSSSRRRR;
   dest.car_ws = src->ops[1].DxxxxWWW & 0x07;

   return &dest;
}

/************************************************************/
AD_instrument *BNK1_to_AD(BNK1_instrument *src)
{
   int i;
   static AD_instrument dest;

   for (i=sizeof(dest)-1;i>=0;i--)
      *(((char *) &dest)+i) = 0;

   dest.car_avekm =
      (src->car_freq_mult & 0x0f) |
      (src->car_env_scaling ? 0x10:0x00) |
      (src->car_sustain_sound ? 0x20:0x00) |
      (src->car_amp_vib ? 0x80:0x00) |
      (src->car_pitch_vib ? 0x40:0x00);
   
   dest.car_ksl_tl =
      (src->car_out_lvl & 0x3f) |
      (src->car_key_scale_lvl << 6);

   dest.car_ad =
      (src->car_decay & 0x0f) |
     ((src->car_attack & 0x0f) << 4);

   dest.car_sr =
      (src->car_release & 0x0f) |
     ((src->car_sustain_lvl & 0x0f) << 4);

   dest.car_ws = src->car_waveform;

   dest.mod_avekm =
      (src->mod_freq_mult & 0x0f) |
      (src->mod_env_scaling ? 0x10:0x00) |
      (src->mod_sustain_sound ? 0x20:0x00) |
      (src->mod_amp_vib ? 0x80:0x00) |
      (src->mod_pitch_vib ? 0x40:0x00);
   
   dest.mod_ksl_tl =
      (src->mod_out_lvl & 0x3f) |
      (src->mod_key_scale_lvl << 6);

   dest.mod_ad =
      (src->mod_decay & 0x0f) |
     ((src->mod_attack & 0x0f) << 4);

   dest.mod_sr =
      (src->mod_release & 0x0f) |
     ((src->mod_sustain_lvl & 0x0f) << 4);

   dest.mod_ws = src->mod_waveform;

   dest.fb_c =
      ((src->mod_feedback & 7) << 1) |
      ((src->mod_connection & 1) ^ 1);

   return &dest;
}

/***************************************************************/
unsigned GTL_create(GTL_class *GTL)
{
   GTL->entries = 0;
   header[0].offset = 0L;

   if ((GTL->body = fopen(tempname,"w+b")) == NULL)
      return 18;

   return 0;
}

/************************************************************/
unsigned GTL_write_timbre(GTL_class *GTL)
{
   unsigned i = GTL->entries;
   unsigned t;

   if (i>=16384) return 17;

   t = GTL->tlen + 2;

   header[i].bank = GTL->tbank;
   header[i].patch = GTL->tpatch;
   header[i+1].offset = header[i].offset + (unsigned long) t;

   fwrite(&t,2,1,GTL->body);
   fwrite(GTL->timb,GTL->tlen,1,GTL->body);

   if (ferror(GTL->body)) return 18;

   ++GTL->entries;
   return 0;
}

/************************************************************/
unsigned GTL_close(GTL_class *GTL)
{
   unsigned i;
   unsigned long hsize;
   unsigned long len;
   static char far buff[512];

   fclose(GTL->body);

   if ((GTL->out = fopen(GTL->filename,"w+b")) == NULL)
      return 18;

   hsize = ((unsigned long) sizeof(GTL_hdr) * (unsigned long) GTL->entries) +
      2L;

   for (i=0;i<GTL->entries;i++)
      {
      header[i].offset += hsize;
      fwrite(&header[i],sizeof(GTL_hdr),1,GTL->out);
      }

   i=0xffff;
   fwrite(&i,2,1,GTL->out);

   if ((GTL->body = fopen(tempname,"rb")) == NULL)
      return 18;

   len = filelength(fileno(GTL->body));

   while (len > 512L)
      {
      fread(buff,512,1,GTL->body);
      fwrite(buff,512,1,GTL->out);
      len -= 512L;
      }
   fread(buff,len,1,GTL->body);
   fwrite(buff,len,1,GTL->out);

   if (ferror(GTL->body)) return 18;
   if (ferror(GTL->out)) return 18;

   fclose(GTL->body);
   fclose(GTL->out);
   unlink(tempname);

   printf("Global Timbre Library \"%s\" created.\n",GTL->filename);

   return 0;
}

/************************************************************/
unsigned GTL_write_BIN(GTL_class *GTL, char *filename)
{
   unsigned err;

   GTL->tlen = file_size(filename);
   if ((GTL->timb = read_file(filename,NULL)) == NULL) return 20;

   err = GTL_write_timbre(GTL);

   farfree(GTL->timb);

   return err;
}

/************************************************************/
unsigned GTL_write_MTB(GTL_class *GTL, char *filename)
{
   unsigned i;
   unsigned err;
   char far *GTL_base;

   if ((GTL->timb = read_file(filename,NULL)) == NULL) return 20;
   GTL->tlen = 0xf6;
   GTL_base = GTL->timb;

   for (i=0;i<64;i++)
      {
      if ((err = GTL_write_timbre(GTL)) != 0) break;
      ++GTL->tpatch;   
      if (GTL->tpatch == 128)
         {
         GTL->tpatch = 0;
         ++GTL->tbank;
         }
      GTL->timb = add_ptr(GTL->timb,GTL->tlen);
      }

   farfree(GTL_base);

   return err;
}

/************************************************************/
unsigned GTL_write_BNK1(GTL_class *GTL, char *title, bnk1_hdr *BNK, int txp)
{
   unsigned err;
   BNK1_instrument *BNK1;

   struct 
      {
      char transpose;
      AD_instrument AIL;
      }
   GTL_entry;

   BNK1 = BNK1_find_instrument(BNK,title);
   if (BNK1 == NULL) return 15;
   if (BNK1->voice_num) return 22;

   GTL_entry.AIL = *BNK1_to_AD(BNK1);
   GTL_entry.transpose = txp & 0xff;

   GTL->tlen = sizeof(GTL_entry);
   GTL->timb = (unsigned char *) &GTL_entry;

   return GTL_write_timbre(GTL);
}

/************************************************************/
unsigned GTL_write_BNK2(GTL_class *GTL, char *title, bnk2_hdr *BNK, int txp)
{
   unsigned err;
   BNK2_instrument *BNK2;

   struct 
      {
      char transpose;
      OPL3_instrument AIL;
      }
   OPL3_entry;

   struct 
      {
      char transpose;
      AD_instrument AIL;
      }
   AD_entry;

   BNK2 = BNK2_find_instrument(BNK,title);
   if (BNK2 == NULL) return 15;
   if (BNK2->xxP24NNN & 0x20) return 22;

   if (BNK2->xxP24NNN & 0x08)
      {     
      OPL3_entry.AIL = *BNK2_to_OPL3(BNK2);
      OPL3_entry.transpose = ((txp & 0xff) + (BNK2->TTTTTTTT)) & 0xff;
      GTL->tlen = sizeof(OPL3_entry);
      GTL->timb = (unsigned char *) &OPL3_entry;

      if (!strnicmp(".AD",&GTL->filename[strlen(GTL->filename)-3],3))
         {
         fprintf(stderr,"Warning: YMF262 timbre \"%s\" copied ",title);
         fprintf(stderr,"to YM3812 GTL file \"%s\"\n",GTL->filename);
         }
      }
   else
      {    
      AD_entry.AIL = *BNK2_to_AD(BNK2);
      AD_entry.transpose = ((txp & 0xff) + (BNK2->TTTTTTTT)) & 0xff;
      GTL->tlen = sizeof(AD_entry);
      GTL->timb = (unsigned char *) &AD_entry;
      }

   return GTL_write_timbre(GTL);
}

/************************************************************/
#if OSI_ALE

unsigned GTL_write_ALE(GTL_class *GTL, char *title, unsigned char *AFX,
   unsigned char *ADT, int tt)
{
   unsigned i = GTL->entries;
   unsigned t;
   unsigned err;
   unsigned char type,transpose;
   char *ALE;

   if (i>=16384) return 17;

   type = (tt >> 8) + 1;     
   transpose = tt & 0xff;

   ALE = ALE_find_instrument(AFX,ADT,title);
   if (ALE == NULL) return 16;

   GTL->tlen = (*(unsigned *) ALE)-2;
   GTL->timb = ALE+2;
   t = GTL->tlen + 4;

   header[i].bank = GTL->tbank;
   header[i].patch = GTL->tpatch;
   header[i+1].offset = header[i].offset + (unsigned long) t;

   fwrite(&t,2,1,GTL->body);
   fwrite(&transpose,1,1,GTL->body);
   fwrite(&type,1,1,GTL->body);
   fwrite(GTL->timb,GTL->tlen,1,GTL->body);

   if (ferror(GTL->body)) return 18;

   ++GTL->entries;
   return 0;
}

#endif

/************************************************************/
void compile_GTL(char *cat_fn)
{
   function lv,rv;
   unsigned i,j,k,l,chr,line,banktype;
   FILE *cat;
   int qp,qt;
   static GTL_class far GTL;
   static char far GTL_fname[512];
   static char far buff[512];
   static char far buff2[512];
   void *BNK;
   bnk1_hdr *BNK1;
   bnk2_hdr *BNK2;
   char far *ADT, far *AFX;

   strcpy(GTL_fname,"");
   BNK=NULL;
   BNK1=NULL;
   BNK2=NULL;
   AFX=ADT=NULL;

   cat = fopen(cat_fn,"rt");
   if (cat==NULL) abend(1,3,(unsigned long) cat_fn);

   line=0;
   while (fgets(linbuf,2048,cat) != NULL)
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

      chr = 0;
      abend(expr_val(linbuf,&chr,&lv),line,0);
      if (!tokens[lv.t].lval_OK) abend(6,line,0);
      if ((lv.len < tokens[lv.t].argmin) ||
          (lv.comcnt < (tokens[lv.t].argmin-1))) abend(13,line,0);
      if (lv.len > tokens[lv.t].argmax) abend(11,line,0);
      if (tokens[lv.t].rval_req)
         {
         if (linbuf[chr++] != '=') abend(8,line,0);
         abend(expr_val(linbuf,&chr,&rv),line,0);
         if (!tokens[rv.t].rval_OK) abend(6,line,0);
         if ((rv.len < tokens[rv.t].argmin) ||
            (rv.comcnt < (tokens[rv.t].argmin-1))) abend(13,line,0);
         if (rv.len > tokens[rv.t].argmax) abend(11,line,0);
         }
      if (linbuf[chr]) abend(9,line,0);
      
      switch (lv.t)
         {
         case TOK_TIMBRE:
            if (!strlen(GTL_fname)) abend(21,0L,0L);
            GTL.tbank = lv.bytes[0];
            GTL.tpatch = lv.bytes[1];
            switch (rv.t)
               {
               case TOK_MTB_FILE:
                  make_str(buff,&rv,0);
                  abend(GTL_write_MTB(&GTL,buff),(unsigned long) buff,0L);
                  break;
               case TOK_BIN_FILE:
                  make_str(buff,&rv,0);
                  abend(GTL_write_BIN(&GTL,buff),(unsigned long) buff,0L);
                  break;
               case TOK_BNK_INST:
                  make_str(buff,&rv,1);
                  if (banktype == 1)
                     abend(GTL_write_BNK1(&GTL,buff,BNK1,get_arg(&rv,0)),
                        (unsigned long) buff, 0L);
                  else
                     abend(GTL_write_BNK2(&GTL,buff,BNK2,get_arg(&rv,0)),
                        (unsigned long) buff, 0L);
                  break;
#if OSI_ALE
               case TOK_ALE_INST:
                  make_str(buff,&rv,1);
                  abend(GTL_write_ALE(&GTL,buff,AFX,ADT,get_arg(&rv,0)&0xff),
                     (unsigned long) buff, 0L);
                  break;
               case TOK_ALE_EFFECT:
                  make_str(buff,&rv,0);
                  abend(GTL_write_ALE(&GTL,buff,AFX,ADT,0x100),
                     (unsigned long) buff, 0L);
                  break;
#endif
               }
            break;
         case TOK_BNK_FILE:
            if (BNK!=NULL)
               {
               farfree(BNK);
               BNK = NULL;
               }
            make_str(buff,&lv,0);
            BNK = read_file(buff,NULL);
            BNK1 = BNK;
            BNK2 = BNK;
            if (BNK==NULL) abend(20,(unsigned long) buff,0L);
            if (!(banktype=BNK_type(BNK)) != 0)
               abend(14,(unsigned long) buff,0L);
            break;
#if OSI_ALE
         case TOK_ALE_FILE:
            if (ADT!=NULL)
               {
               farfree(ADT);
               ADT = NULL;
               }
            if (AFX!=NULL)
               {
               farfree(AFX);
               AFX = NULL;
               }
            make_str(buff,&lv,0);
            strcpy(buff2,buff);
            strcpy(&buff2[strlen(buff2)-4],".ADT");
            AFX = read_file(buff,NULL);
            ADT = read_file(buff2,NULL);
            if ((AFX==NULL) || (ADT==NULL))
               abend(20,(unsigned long) buff,0L);
            break;
#endif
         case TOK_OUTFILE:
            if (BNK!=NULL)
               {
               farfree(BNK);
               BNK = NULL;
               }
            if (strlen(GTL.filename))
               abend(GTL_close(&GTL),0L,0L);
            GTL.filename = make_str(GTL_fname,&lv,0);
            abend(GTL_create(&GTL),0L,0L);
            break;
         }
      }

   if (BNK!=NULL)
      {
      farfree(BNK);
      BNK = NULL;
      }

   if (ADT!=NULL)
      {
      farfree(ADT);
      ADT = NULL;
      }

   if (AFX!=NULL)
      {
      farfree(AFX);
      AFX = NULL;
      }

   if (strlen(GTL.filename))
      abend(GTL_close(&GTL),0L,0L);

   if (ferror(cat)) abend(1,5,(unsigned long) cat_fn);
   fclose(cat);
}                                                                

/************************************************************/
void main(int argc, char *argv[])
{
   unsigned i,n;

   printf("\nGLIB version %s                   Copyright (C) 1991, 1992 Miles Design, Inc.\n",VERSION);
   printf("-------------------------------------------------------------------------------\n\n");

   if (argc != 2)
      {
      show_syntax();
      exit(1);
      }

   compile_GTL(argv[1]);

   exit(0);
}
