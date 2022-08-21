//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   CLAD.C                                                              лл
//лл                                                                       лл
//лл   Creative Labs / Ad Lib Instrument File Translator                   лл
//лл                                                                       лл
//лл   Usage: CLAD <infile.ins | infile.sbi> <outfile.ins | outfile.sbi>   лл
//лл                                                                       лл
//лл   Provides for the exchange of instrument file data between Ad Lib    лл
//лл   and Creative Labs (Sound Blaster) application programs              лл
//лл                                                                       лл
//лл   V1.00 of 25-Jun-91 -- Initial version                               лл
//лл    1.01 of 19-May-93 -- Amplitude/pitch vibrato bits switched         лл
//лл                                                                       лл
//лл   Project: IBM Audio Interface Library                                лл
//лл    Author: John Miles                                                 лл
//лл                                                                       лл
//лл   C source compatible with Turbo C++ v1.0 or later                    лл
//лл                                                                       лл
//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   clad.obj: clad.c gen.h                                              лл
//лл      bcc -ml -c -v clad.c                                             лл
//лл                                                                       лл
//лл   clad.exe: clad.obj gen.lib                                          лл
//лл      tlink @clad.lls                                                  лл
//лл                                                                       лл
//лл   Contents of CLAD.LLS:                                               лл
//лл     /c /v /x +                                                        лл
//лл     \bc\lib\c0l.obj +                                                 лл
//лл     clad, +                                                           лл
//лл     clad.exe, +                                                       лл
//лл     clad.map, +                                                       лл
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

typedef struct
{
   unsigned char sound_mode;
   unsigned char voice_num;
   unsigned mod_key_scale_lvl;
   unsigned mod_freq_mult;
   unsigned mod_feedback;
   unsigned mod_attack;
   unsigned mod_sustain_lvl;
   unsigned mod_sustain_sound;
   unsigned mod_decay;
   unsigned mod_release;
   unsigned mod_out_lvl;
   unsigned mod_amp_vib;
   unsigned mod_pitch_vib;
   unsigned mod_env_scaling;
   unsigned mod_connection;
   unsigned car_key_scale_lvl;
   unsigned car_freq_mult;
   unsigned car_feedback;
   unsigned car_attack;
   unsigned car_sustain_lvl;
   unsigned car_sustain_sound;
   unsigned car_decay;
   unsigned car_release;
   unsigned car_out_lvl;
   unsigned car_amp_vib;
   unsigned car_pitch_vib;
   unsigned car_env_scaling;
   unsigned car_connection;
   unsigned mod_waveform;
   unsigned car_waveform;
   unsigned char unused[22];
} ins_struct;

typedef struct
{
   unsigned char file_id[4];
   unsigned char ins_name[32];
   unsigned char mod_sound_char;
   unsigned char car_sound_char;
   unsigned char mod_scaling_output;
   unsigned char car_scaling_output;
   unsigned char mod_ad;
   unsigned char car_ad;
   unsigned char mod_sr;
   unsigned char car_sr;
   unsigned char mod_waveform;
   unsigned char car_waveform;
   unsigned char feedback_conn;
   unsigned char sound_mode;
   unsigned char unused[4];
} sbi_struct;

unsigned char *in;
unsigned char outbuf[256];
unsigned out_len;

#define UNDEFINED 0
#define INS_TO_SBI 1
#define SBI_TO_INS 2

unsigned char fix_key_scale[] = {0,2,1,3};

unsigned char sm_si[] = {0,6,7,8,9,10};
unsigned char sm_is[] = {0,0,0,0,0,0,1,2,3,4,5};

char src_fn[128];
char dest_fn[128];

/************************************************************/
void show_syntax(void)
{
   printf("This program translates instrument file data between Ad Lib and Creative\n");
   printf("Labs (Sound Blaster) formats.\n\n");             

   printf("Usage: CLAD <infile.ins | infile.sbi> <outfile.ins | outfile.sbi>\n\n");
   printf("where infile = Creative Labs or Ad Lib standard instrument source file\n");
   printf("     outfile = Creative Labs or Ad Lib standard instrument destination file\n");
}

/************************************************************/
void abend(int err, unsigned long info_1, unsigned long info_2)
{
   if (!err) return;

   fprintf(stderr,"Error CA%03u-%lu ",err,info_1);

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
         fprintf(stderr,"Illegal file conversion requested"); break;
      }                  
   
   fprintf(stderr,"\n");
   fcloseall();
   exit(err);
}

/************************************************************/
void sbi_to_ins(sbi_struct *sbi, ins_struct *ins)
{
   int i;

   for (i=sizeof(ins_struct)-1;i>=0;i--)
      *(((char *) ins)+i) = 0;

   ins->mod_freq_mult = sbi->mod_sound_char & 0x0f;
   ins->mod_env_scaling = ((sbi->mod_sound_char & 0x10) != 0);
   ins->mod_sustain_sound = ((sbi->mod_sound_char & 0x20) != 0);
   ins->mod_amp_vib = ((sbi->mod_sound_char & 0x80) != 0);
   ins->mod_pitch_vib = ((sbi->mod_sound_char & 0x40) != 0);
   
   ins->car_freq_mult = sbi->car_sound_char & 0x0f;
   ins->car_env_scaling = ((sbi->car_sound_char & 0x10) != 0);
   ins->car_sustain_sound = ((sbi->car_sound_char & 0x20) != 0);
   ins->car_amp_vib = ((sbi->car_sound_char & 0x80) != 0);
   ins->car_pitch_vib = ((sbi->car_sound_char & 0x40) != 0);

   ins->mod_out_lvl = sbi->mod_scaling_output & 0x3f;
   ins->mod_key_scale_lvl = fix_key_scale[sbi->mod_scaling_output >> 6];

   ins->car_out_lvl = sbi->car_scaling_output & 0x3f;
   ins->car_key_scale_lvl = fix_key_scale[sbi->car_scaling_output >> 6];

   ins->mod_attack = sbi->mod_ad >> 4;
   ins->mod_decay = sbi->mod_ad & 0x0f;
   ins->mod_sustain_lvl = sbi->mod_sr >> 4;
   ins->mod_release = sbi->mod_sr & 0x0f;

   ins->car_attack = sbi->car_ad >> 4;  
   ins->car_decay = sbi->car_ad & 0x0f;
   ins->car_sustain_lvl = sbi->car_sr >> 4;
   ins->car_release = sbi->car_sr & 0x0f;

   ins->mod_feedback = (sbi->feedback_conn >> 1) & 0x07;
   ins->car_feedback = (sbi->feedback_conn >> 1) & 0x07;

   ins->mod_connection = (sbi->feedback_conn & 1) ^ 1;
   ins->car_connection = (sbi->feedback_conn & 1) ^ 1;

   ins->mod_waveform = sbi->mod_waveform;
   ins->car_waveform = sbi->car_waveform;

   ins->voice_num = sm_si[sbi->sound_mode];
   ins->sound_mode = (sbi->sound_mode != 0);
}

/************************************************************/
void ins_to_sbi(ins_struct *ins, sbi_struct *sbi)
{
   int i;

   for (i=sizeof(sbi_struct)-1;i>=0;i--)
      *(((char *) sbi)+i) = 0;

   strcpy(sbi->file_id,"SBI");
   sbi->file_id[3] = 0x1a;

   sbi->mod_sound_char =
      (ins->mod_freq_mult & 0x0f) |
      (ins->mod_env_scaling ? 0x10:0x00) |
      (ins->mod_sustain_sound ? 0x20:0x00) |
      (ins->mod_amp_vib ? 0x80:0x00) |
      (ins->mod_pitch_vib ? 0x40:0x00);

   sbi->car_sound_char =
      (ins->car_freq_mult & 0x0f) |
      (ins->car_env_scaling ? 0x10:0x00) |
      (ins->car_sustain_sound ? 0x20:0x00) |
      (ins->car_amp_vib ? 0x80:0x00) |
      (ins->car_pitch_vib ? 0x40:0x00);

   sbi->mod_scaling_output =
      (ins->mod_out_lvl & 0x3f) |
      fix_key_scale[ins->mod_key_scale_lvl << 6];

   sbi->car_scaling_output =
      (ins->car_out_lvl & 0x3f) |
      fix_key_scale[ins->car_key_scale_lvl << 6];

   sbi->mod_ad =
      (ins->mod_decay & 0x0f) |
     ((ins->mod_attack & 0x0f) << 4);

   sbi->car_ad =
      (ins->car_decay & 0x0f) |
     ((ins->car_attack & 0x0f) << 4);

   sbi->mod_sr =
      (ins->mod_release & 0x0f) |
     ((ins->mod_sustain_lvl & 0x0f) << 4);

   sbi->car_sr =
      (ins->car_release & 0x0f) |
     ((ins->car_sustain_lvl & 0x0f) << 4);

   sbi->mod_waveform = ins->mod_waveform;
   sbi->car_waveform = ins->car_waveform;

   sbi->feedback_conn =
     ((ins->mod_feedback & 7) << 1);

   sbi->feedback_conn |= ((ins->mod_connection & 1) ^ 1);

   sbi->sound_mode = sm_is[ins->voice_num];
}

/************************************************************/
void main(int argc, char *argv[])
{
   int direction;
   int i;
   unsigned long in_len, out_len;

   printf("\nCLAD version %s                   Copyright (C) 1991, 1992 Miles Design, Inc.\n",VERSION);
   printf("-------------------------------------------------------------------------------\n\n");

   if (argc != 3)
      {
      show_syntax();
      exit(1);
      }

   for (i=0;i<strlen(argv[1]);i++)
      {
      src_fn[i] = argv[1][i]; 
      if (src_fn[i] == '.') break;
      }
   src_fn[i+1] = 0;
   for (i=strlen(argv[1])-1;i>=0;i--)
      if (argv[1][i] == '.')
         {
         strcat(src_fn,&argv[1][i+1]);
         break;
         }

   for (i=0;i<strlen(argv[2]);i++)
      {
      dest_fn[i] = argv[2][i]; 
      if (dest_fn[i] == '.') break;
      }
   dest_fn[i+1] = 0;
   for (i=strlen(argv[2])-1;i>=0;i--)
      if (argv[2][i] == '.')
         {
         strcat(dest_fn,&argv[2][i+1]);
         break;
         }

   direction = UNDEFINED;
   if ((!stricmp(&src_fn[strlen(src_fn)-4],".INS")) &&
      (!stricmp(&dest_fn[strlen(dest_fn)-4],".SBI")))
         direction = INS_TO_SBI;
   if ((!stricmp(&src_fn[strlen(src_fn)-4],".SBI")) &&
      (!stricmp(&dest_fn[strlen(dest_fn)-4],".INS")))
         direction = SBI_TO_INS;
   if (direction == UNDEFINED) abend(2,NULL,NULL);

   printf("Converting %s to %s ....\n\n",src_fn,dest_fn);

   if ((in=read_file(src_fn,NULL))==NULL)
      abend(1,get_disk_error(),(unsigned long) src_fn);

   switch (direction)
      {
      case INS_TO_SBI:
         ins_to_sbi((ins_struct *) in,(sbi_struct *) outbuf);
         out_len = sizeof(sbi_struct);
         break;
      case SBI_TO_INS:
         sbi_to_ins((sbi_struct *) in,(ins_struct *) outbuf);
         out_len = sizeof(ins_struct);
         break;
      }

   printf("Writing %lu bytes\n",out_len);
   if (!write_file(dest_fn,outbuf,out_len))
      abend(1,get_disk_error(),(unsigned long) dest_fn);

   exit(0);
}


