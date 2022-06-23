//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   VTOR.C                                                              лл
//лл                                                                       лл
//лл   .VOC file to raw PCM data conversion utility                        лл
//лл                                                                       лл
//лл   V1.00 of 4-Jan-92                                                   лл
//лл   V1.01 of 14-Jan-92: Stereo .VOC support added                       лл
//лл                                                                       лл
//лл   Project: IBM Audio Interface Library                                лл
//лл    Author: John Miles                                                 лл
//лл                                                                       лл
//лл   C source compatible with Turbo C++ v1.0 or later                    лл
//лл                                                                       лл
//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   vtor.obj: vtor.c gen.h                                              лл
//лл      bcc -ml -c -v vtor.c                                             лл
//лл                                                                       лл
//лл   vtor.exe: vtor.obj gen.lib                                          лл
//лл      tlink @vtor.lls                                                  лл
//лл                                                                       лл
//лл   Contents of VTOR.LLS:                                               лл
//лл     /c /v /x +                                                        лл
//лл     \bc\lib\c0l.obj +                                                 лл
//лл     vtor, +                                                           лл
//лл     vtor.exe, +                                                       лл
//лл     vtor.map, +                                                       лл
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

#include "gen.h"           // General DOS and system functions

const char VERSION[] = "1.01";

char out_fn[128];
char in_fn[128];

typedef struct
{
   char type_desc[0x14];
   unsigned data_blk_offset;
   unsigned version;
   unsigned IDchk;
}
VOC_hdr;

typedef struct
{
   unsigned char type;
   unsigned long len;

}
VOC_blk;

#define BLK_TERMINATOR 0
#define BLK_VOICE_DATA 1
#define BLK_VOICE_CONTINUATION 2
#define BLK_SILENCE 3
#define BLK_MARKER 4
#define BLK_ASCII_TEXT 5
#define BLK_REPEAT_LOOP 6
#define BLK_END_REPEAT_LOOP 7
#define BLK_EXTENDED 8

char ptypes[][32] =
{
   "8-bit raw or unpacked",
   "4-bit packed",
   "2.6-bit packed",
   "2-bit packed"
};

char far buff[32768];

/************************************************************/
void abend(int err, unsigned long info_1, unsigned long info_2)
{
   if (!err) return;

   fprintf(stderr,"Error VR%03u: ",err);

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
         fprintf(stderr,"File \"%s\" not a standard .VOC file",info_1);
         break;
      case 3:
         fprintf(stderr,".VOC file \"%s\" contains blocks with different sample rates",
            info_1);
         break;
      case 4:
         fprintf(stderr,".VOC file \"%s\" uses silence packing",info_1);
         break;
      case 5:
         fprintf(stderr,".VOC file \"%s\" contains an endless or invalid loop",info_1);
         break;
      case 6:
         fprintf(stderr,".VOC file \"%s\" contains no Voice Data blocks",info_1);
         break;
      case 7:
         fprintf(stderr,".VOC file \"%s\" uses multiple packing types",
            info_1);
         break;
      case 8:
         fprintf(stderr,".VOC file \"%s\" uses an unknown packing type",
            info_1);
         break;
      case 9:
         fprintf(stderr,".VOC file \"%s\" contains mixed stereo and mono data",
            info_1);
         break;
      }                      
   
   fprintf(stderr,"\n");
   fcloseall();
   if (strlen(out_fn)) unlink(out_fn);
   exit(err);
}

/***************************************************************/
unsigned VOC_next_blk(FILE *in, VOC_blk far *VB)
{
   VB->type = 0xff;
   VB->len = 0L;

   fread(&VB->type,sizeof(VB->type),1,in);

   if (VB->type != BLK_TERMINATOR)
      {
      fread(&VB->len,3,1,in);
      VB->len &= 0x00ffffffL;
      }

   return VB->type;
}


void FILE_copy_data(FILE *in, FILE *out, unsigned long data_len)
{

   while (data_len > sizeof(buff))
      {
      if (fread(&buff,1,sizeof(buff),in) != sizeof(buff))
         abend(1,5,(unsigned long) in_fn);

      if (fwrite(&buff,1,sizeof(buff),out) != sizeof(buff))
         abend(1,4,(unsigned long) out_fn);

      data_len -= sizeof(buff);
      }

   if (fread(&buff,1,data_len,in) != data_len)
      abend(1,5,(unsigned long) in_fn);

   if (fwrite(&buff,1,data_len,out) != data_len)
      abend(1,4,(unsigned long) out_fn);
}


/***************************************************************/
void main(int argc, char *argv[])
{
   FILE *in,*out;
   int bad,strcnt;
   int infile;
   int i,done;
   unsigned hertz,rep_cnt;
   unsigned char sr,pack,type_sr,type_pack;
   unsigned long next_block,rep_pos;
   unsigned xblk_status,xblk_sr;
   unsigned char xblk_pack;
   int xblk_mode,xblk_lastmode;
   VOC_hdr VH;
   VOC_blk VB;

   printf("\nVTOR version %s                   Copyright (C) 1991, 1992 Miles Design, Inc.\n",VERSION);
   printf("-------------------------------------------------------------------------------\n\n");

   strcpy(out_fn,"");
   strcpy(in_fn,"");
   bad=strcnt=0;

   for (i=1;i<argc;i++)
      if (argv[i][0] != '/')
         {
         ++strcnt;
         if (strcnt == 2)
            strcpy(out_fn,argv[i]);
         if (strcnt == 1)
            strcpy(in_fn,argv[i]);
         }
      else
         {
         bad=1; break;
         }

   if (bad || (strcnt != 2))
      {
      printf("This program converts standard Sound Blaster .VOC files to raw PCM\n");
      printf("data samples.  .VOC files to be converted may be of any length \n");
      printf("and must not use silence packing or multiple sample rates.\n\n");

      printf("Usage: VTOR filename.VOC output_filename\n");
      exit(1);
      }

   printf("Converting, please wait ....\n\n");


   in = fopen(in_fn,"rb");
   if (in==NULL) abend(1,3,(unsigned long) in_fn);

   out = fopen(out_fn,"w+b");
   if (out==NULL) abend(1,4,(unsigned long) out_fn);
   
   fread(&VH,sizeof(VOC_hdr),1,in);

   VH.type_desc[0x13] = 0;

   if (stricmp(VH.type_desc,"Creative Voice File"))
      abend(2,(unsigned long) in_fn,0L);

   hertz = 0;
   xblk_status = 0;
   xblk_mode = -1;

   fseek(in,VH.data_blk_offset,SEEK_SET);

   while (VOC_next_blk(in,&VB) != BLK_TERMINATOR)
      {
      next_block = ftell(in) + VB.len;

      switch (VB.type)
         {
         case BLK_VOICE_DATA:
            fread(&sr,sizeof(sr),1,in);              
            fread(&pack,sizeof(pack),1,in);

            if (xblk_status)
               {
               sr = xblk_sr >> 8;

               pack = xblk_pack;
               xblk_status = 0;
               }
            else
               {
               if (xblk_mode == 1)
                  abend(9,(unsigned long) in_fn,0L);

               xblk_mode = 0;
               }

            if (!hertz)
               {
               type_sr = sr;
               type_pack = pack;

               hertz = (unsigned) (1000000L / (256L - (unsigned long) sr));
               }
            else
               if (type_sr != sr)
                  abend(3,(unsigned long) in_fn,0L);
               else if (type_pack != pack)
                  abend(7,(unsigned long) in_fn,0L);

            if (pack >= (sizeof(ptypes) / sizeof(ptypes[0])))
               abend(8,(unsigned long) in_fn,0L);

            FILE_copy_data(in,out,VB.len - 2L);
            break;

         case BLK_VOICE_CONTINUATION:
            FILE_copy_data(in,out,VB.len);
            break;

         case BLK_REPEAT_LOOP:
            fread(&rep_cnt,sizeof(rep_cnt),1,in);
            if ((rep_cnt == 0xffff) || (rep_cnt == 0))
               abend(5,(unsigned long) in_fn,0L);

            rep_pos = ftell(in);
            break;

         case BLK_END_REPEAT_LOOP:
            if (rep_cnt--)
               next_block = rep_pos;
            break;

         case BLK_SILENCE:
            abend(4,(unsigned long) in_fn,0L);
            break;

         case BLK_MARKER:
         case BLK_ASCII_TEXT:
            break;

         case BLK_EXTENDED:
            xblk_lastmode = xblk_mode;

            fread(&xblk_sr,sizeof(xblk_sr),1,in);              
            fread(&xblk_pack,sizeof(xblk_pack),1,in);
            fread(&xblk_mode,sizeof(xblk_mode),1,in);
            xblk_status = 1;

            if (((xblk_lastmode==1) && (xblk_mode==0)) ||
                ((xblk_lastmode==0) && (xblk_mode==1)))
               abend(9,(unsigned long) in_fn,0L);
            break;

         default:
            printf("Warning: skipping unrecognized block type (%u)\n\n",
               VB.type);
         }

      fseek(in,next_block,SEEK_SET);
      }

   fclose(in);
   fclose(out);

   if (!hertz)
      abend(6,(unsigned long) in_fn,0L);

   printf("File \"%s\" created.\n\n",out_fn);
   printf("               Output file size: %lu\n",file_size(out_fn));
   printf("                   Packing type: %s",ptypes[pack]);

   if (xblk_mode)
      printf(" (Stereo)\n");
   else
      printf(" (Monaural)\n");

   printf("           Sample rate in hertz: %u",hertz);

   if (xblk_mode)
      printf(" (%lu bytes/sec)",hertz/2L);
   printf("\n");

   printf("AIL sound_buff.sample_rate byte: %u\n",(unsigned) sr);
   exit(0);
}
