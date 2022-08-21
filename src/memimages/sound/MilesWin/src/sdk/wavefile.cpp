//############################################################################
//##                                                                        ##
//##  Miles Sound System                                                    ##
//##                                                                        ##
//##  WAVEFILE.C: Digital sound API module for digital sound file access    ##
//##                                                                        ##
//##  16-bit protected-mode source compatible with MSC 7.0                  ##
//##  32-bit protected-mode source compatible with MSC 9.0                  ##
//##                                                                        ##
//##  Version 1.00 of 15-Feb-95: Derived from WAILSFIL V1.00                ##
//##          1.10 of 11-May-97: Added IMA ADPCM support (Serge Plagnol)    ##
//##          1.20 of 10-May-02: Massive changes for new reverb, many       ##
//##                             functions moved from mssdig.cpp (JKR)      ##
//##                                                                        ##
//##  Author: John Miles and Jeff Roberts                                   ##
//##                                                                        ##
//############################################################################
//##                                                                        ##
//##  Copyright (C) RAD Game Tools, Inc.                                    ##
//##                                                                        ##
//##  Contact RAD Game Tools at 425-893-4300 for technical support.         ##
//##                                                                        ##
//############################################################################

#include <stdio.h>
#include <string.h>

#include "mss.h"
#include "imssapi.h"

#ifndef IS_WIN32
#include <stdarg.h>
#endif

#include <math.h>

//
// Recognized file types
//

#define FTYP_VOC  0
#define FTYP_WAV  1
#define FTYP_ASI  2

#define MY_PI        3.14159265F
#define MY_TWO_PI   (3.14159265F*2.0F)
#define MSS_EPSILON 0.0001F

#define SPEED_OF_SOUND    0.355F   // Speed of sound in meters per millisecond
#define MAX_DOPPLER_SHIFT 4.00F  
#define MIN_DOPPLER_SHIFT 0.25F

#define DEGS_TO_DIAMS(degs) ((degs) / 114.591559F)
#define DIAMS_TO_DEGS(degs) ((degs) * 114.591559F)

//
// .WAV FACT chunk
//

typedef MSS_STRUCT
{
  S8  FACT_string[ 4 ];
  U32 chunk_size;
  U32 samples;
}
FACT;

//
//
// .VOC terminator block
//

typedef MSS_STRUCT
{
  U8 block_ID;
}
BLK_0;

//
// .VOC voice block
//

typedef MSS_STRUCT
{
  U8 block_ID;
  U8 block_len[ 3 ];
  U8 time_constant;
  U8 pack_method;
}
BLK_1;

//
// .VOC continued voice block
//

typedef MSS_STRUCT
{
   U8 block_ID;
   U8 block_len[ 3 ];
}
BLK_2;

//
// .VOC silence block
//

typedef MSS_STRUCT
{
  U8  block_ID;
  U8  block_len[ 3 ];
  U16 pause_period;
  U8  time_constant;
}
BLK_3;

//
// .VOC marker block
//

typedef MSS_STRUCT
{
  U8  block_ID;
  U8  block_len[ 3 ];
  S16 marker;
}
BLK_4;

//
// .VOC ASCIIZ comment block
//

typedef MSS_STRUCT
{
  U8 block_ID;
  U8 block_len[ 3 ];
  S8 string;
}
BLK_5;

//
// .VOC repeat loop block
//

typedef MSS_STRUCT
{
  U8  block_ID;
  U8  block_len[ 3 ];
  U16 repeat_count;
}
BLK_6;

//
// .VOC end-of-loop block
//

typedef MSS_STRUCT
{
  U8 block_ID;
  U8 block_len[ 3 ];
}
BLK_7;

//
// .VOC extended attribute block
//
// (always followed by block 1)
//

typedef MSS_STRUCT
{
  U8  block_ID;
  U8  block_len[ 3 ];
  U16 time_constant;
  U8  pack_method;
  U8  voice_mode;
}
BLK_8;

//
// .VOC extended voice block
//
// (replaces blocks 1 and 8)
//

typedef MSS_STRUCT
{
  U8  block_ID;
  U8  block_len[ 3 ];
  U32 sample_rate;
  U8  bits_per_sample;
  U8  channels;
  U16 format;
  U8  reserved[ 4 ];
}
BLK_9;

//
// .WAV file headers
//

typedef MSS_STRUCT
{
  S8  RIFF_string[ 4 ];
  U32 chunk_size;
  S8  ID_string[ 4 ];
  U8  data[ 1 ];
}
RIFF;

//
// .WAV PCM file format chunk
//

typedef MSS_STRUCT
{
  S8   FMT_string[ 4 ];
  U32  chunk_size;

  S16  format_tag;
  S16  channels;
  S32  sample_rate;
  S32  average_data_rate;
  S16  alignment;
  S16  bits_per_sample;
  S16  extra;
  S16  samples_per_block;
}
FMT;

//
// .WAV file data chunk
//

typedef MSS_STRUCT
{
  S8  DATA_string[ 4 ];
  U32 chunk_size;
  U8  data[ 1 ];
}
DATA;

extern C8 AIL_error[ 256 ];

#ifdef IS_DOS

//############################################################################
//##                                                                        ##
//## Locked code                                                            ##
//##                                                                        ##
//############################################################################

#define LOCK( x )   AIL_vmm_lock  ( &( x ), sizeof( x ) )
#define UNLOCK( x ) AIL_vmm_unlock( &( x ), sizeof( x ) )

static S32 locked = 0;

void AILSFILE_end( void );

void AILSFILE_start( void )
{
  if ( !locked )
  {
    AIL_vmm_lock_range( AILSFILE_start, AILSFILE_end );

    locked = 1;
  }
}

#define DOLOCK() AILSFILE_start()

#else

#define DOLOCK()

#endif

static void clear_last_sample(HSAMPLE S)
{
   S->left_val         =  0;
   S->right_val        =  0;
   S->last_decomp_left =  65536;
   S->src_fract        =  0;
}

static void init_sample_3D_state(HSAMPLE S)
{
   //
   // Ensure that S3D data is valid even for samples that aren't (yet) under 3D
   // control
   //
   // If we didn't do this, voice filters would need to do a lot more work when swapping
   // voices between samples...
   //

   S->S3D.doppler_valid    = 0;
   S->S3D.doppler_shift    = 1.0F;
   S->S3D.inner_angle      = DEGS_TO_DIAMS(360.0F);
   S->S3D.outer_angle      = DEGS_TO_DIAMS(360.0F);
   S->S3D.outer_volume     = 1.0F;
   S->S3D.cone_enabled     = 0;
   
   S->S3D.max_dist     = 200.0F;
   S->S3D.min_dist     = 1.0F;
   S->S3D.dist_changed = TRUE;

   S->S3D.position.x = 0.0F;
   S->S3D.position.y = 0.0F; 
   S->S3D.position.z = 0.0F; 

   S->S3D.face.x = 1.0F;
   S->S3D.face.y = 0.0F;
   S->S3D.face.z = 0.0F;

   S->S3D.up.x = 0.0F;
   S->S3D.up.y = 1.0F;
   S->S3D.up.z = 0.0F;

   S->S3D.velocity.x = 0.0F; 
   S->S3D.velocity.y = 0.0F; 
   S->S3D.velocity.z = 0.0F; 

   S->S3D.auto_3D_atten = 1;
   S->S3D.atten_3D      = 1.0F;

   S->S3D.owner = S;
   S->S3D.falloff_function = SS_default_falloff_function_callback;
}

//############################################################################
//##                                                                        ##
//## Set sample data format and flags                                       ##
//##                                                                        ##
//## Available formats:                                                     ##
//##                                                                        ##
//##   DIG_F_MONO_8                                                         ##
//##   DIG_F_MONO_16                                                        ##
//##   DIG_F_STEREO_8                                                       ##
//##   DIG_F_STEREO_16                                                      ##
//##   DIG_F_ADPCM_MONO_16                                                  ##
//##   DIG_F_ADPCM_STEREO_16                                                ##
//##                                                                        ##
//## Available flags:                                                       ##
//##                                                                        ##
//##   DIG_PCM_SIGN                                                         ##
//##   DIG_PCM_ORDER (stereo formats only)                                  ##
//##                                                                        ##
//## Note: 16-bit sample data must be in Intel byte order                   ##
//##                                                                        ##
//############################################################################

void AILCALL SS_set_sample_type( HSAMPLE S, S32 format, U32 flags )
{
  if ( S == NULL )
  {
    return;
  }

  if ( ( format != S->format ) ||
       ( flags  != S->flags ) )
  {
    S->format = format;
    S->flags  = flags;

    // Reset ADPCM offset to the end of decode buffer
    // to force a decode buffer refill
    if ( format & DIG_F_ADPCM_MASK )
    {
      S->adpcm.blockleft = 0;
      S->adpcm.extrasamples = 0;
    }
  }
}

void AILCALL SS_update_sample_reverb_state(HSAMPLE S)
{
#ifdef MSS_VFLT_SUPPORTED
  if (S->driver->voice_filter != NULL)
    {
    return;
    }
#endif

  if ( ( S->wet_level > 0.0001f ) && ( S->driver->master_wet > 0.0001f ) && ( S->driver->reverb_decay_time_s > 0.0001f ) )
  {
    S->driver->reverb_off_time_ms = AIL_ms_count() + S->driver->reverb_duration_ms + 100;
    S->driver->reverb_on = 1;
  }
}

void AILCALL SS_update_driver_reverb_state(HDIGDRIVER dig)
{
#ifdef MSS_VFLT_SUPPORTED
  if (dig->voice_filter != NULL)
    {
    return;
    }
#endif

  if ( ( dig->master_wet > 0.0001f ) && ( dig->reverb_decay_time_s > 0.0001f ) )
  {
    dig->reverb_off_time_ms = AIL_ms_count() + dig->reverb_duration_ms + 100;
    dig->reverb_on = 1;
  }
}

static __inline void RAD_vector_cross_product(MSSVECTOR3D       *c,
                                              const MSSVECTOR3D *v1,
                                              const MSSVECTOR3D *v2)
{
  c->x = v1->z * v2->y - v1->y * v2->z;
  c->y = v1->x * v2->z - v1->z * v2->x;
  c->z = v1->y * v2->x - v1->x * v2->y;
}


static __inline void RAD_vector_subtract(MSSVECTOR3D       *d,
                                         const MSSVECTOR3D *v1,
                                         const MSSVECTOR3D *v2)
{
  d->x = v1->x - v2->x;
  d->y = v1->y - v2->y;
  d->z = v1->z - v2->z;
}


static __inline void RAD_vector_normalize(MSSVECTOR3D *v, //)
                                          F32         *mag = NULL)
{
  F32 len = AIL_fsqrt((v->x * v->x) +
                      (v->y * v->y) +
                      (v->z * v->z));

  if (mag != NULL)
   {
   *mag = len;
   }

  if (len < 0.0001F)
  {
    v->x=1.0F;
    v->y=0.0F;
    v->z=0.0F;
  }
  else
  {
    v->x /= len;
    v->y /= len;
    v->z /= len;
  }
}


static __inline F32 RAD_vector_dot_product(const MSSVECTOR3D *v1,
                                    const MSSVECTOR3D *v2)
{
  F32 dot;

  dot  = v1->x * v2->x;
  dot += v1->y * v2->y;
  dot += v1->z * v2->z;

  return( dot );
}

static __inline F32 RAD_vector_length(const MSSVECTOR3D *v)
{
  return AIL_fsqrt((v->x * v->x) +
                   (v->y * v->y) +
                   (v->z * v->z));
}

//############################################################################
//##                                                                        ##
//## Get length of .VOC block                                               ##
//##                                                                        ##
//############################################################################

static U32 AIL_VOC_block_len( void FAR *block )
{
  return( ( * (U32 FAR *) block ) >> 8 );
}


//#define CHECKDENORM
#ifdef CHECKDENORM

static void check_denorm( float * f )
{
  union
  {
    float *f;
    unsigned int *i;
  } both;

  both.f = f;

  if ( *f !=0.0f )
  {
    if ((( both.i[0] >>23 )&255) == 0 )
      MSSBreakPoint();
    if ((( both.i[0] >>23 )&255) == 255 )
      MSSBreakPoint();
  }
}

#else

#define check_denorm( ptr )

#endif

//############################################################################
//##                                                                        ##
//## Terminate playback of .VOC file                                        ##
//##                                                                        ##
//## Invoke application callback function, if any, and release the sample   ##
//## allocated to play this file                                            ##
//##                                                                        ##
//############################################################################

static void AIL_VOC_terminate(HSAMPLE sample)
{
}

//############################################################################
//##                                                                        ##
//## Process .VOC file block                                                ##
//##                                                                        ##
//## Called by .VOC initialization code and as end-of-sample callback       ##
//## function (interrupt-based)                                             ##
//##                                                                        ##
//## If play_flag clear, search for first block after desired marker (if    ##
//## any) and return without playing it                                     ##
//##                                                                        ##
//############################################################################

static void AIL_process_VOC_block(HSAMPLE sample, S32 play_flag)
{
   S32  voice_block;
   void FAR *b;

   voice_block = 0;

   //
   // Loop until voice block is found
   //

   while (!voice_block)
      {
      b = (void FAR *) sample->system_data[VOC_BLK_PTR];

      switch (*(U8 FAR *) b)
         {
         //
         // Terminator block
         //

         case 0:

            //
            // Terminate playback, then return without trying to advance
            // to next block
            //

            AIL_VOC_terminate(sample);

            return;

         //
         // Voice block
         //

         case 1:

            //
            // Skip block if desired marker has not been found
            //

            if (!sample->system_data[VOC_MARKER_FOUND])
               {
               break;
               }

            //
            // Set up sample data and start playback
            //

            AIL_set_sample_address(sample,
                                   AIL_ptr_add(b, sizeof(BLK_1)),
                                   AIL_VOC_block_len(b) - 2);

            AIL_set_sample_playback_rate(sample,(U32)
                             ( (S32)1000000L / (S32)(256 - ((BLK_1 FAR *) b)->time_constant) ) );

            SS_set_sample_type(sample,DIG_F_MONO_8,0);

            if (play_flag)
               AIL_start_sample(sample);

            voice_block = 1;
            break;

         //
         // Marker block
         //

         case 4:

            //
            // Ignore if entire file to be played
            //

            if (sample->system_data[VOC_MARKER] == -1)
               {
               break;
               }

            //
            // If this is the desired marker, set MARKER_FOUND flag --
            // otherwise, clear MARKER_FOUND flag to prevent playback
            // of future voice blocks
            //

            if (sample->system_data[VOC_MARKER] == (S32)
                                                   ((BLK_4 FAR *) b)->marker)
               {
               sample->system_data[VOC_MARKER_FOUND] = 1;
               }
            else
               {
               sample->system_data[VOC_MARKER_FOUND] = 0;
               }

            break;

         //
         // Repeat block
         //

         case 6:

            //
            // Log repeat count and starting address of repeat block
            //

            sample->system_data[VOC_REP_BLK] = (UINTa) b;

            sample->system_data[VOC_N_REPS]  = (UINTa)
                                             ((BLK_6 FAR *) b)->repeat_count;
            break;

         //
         // End repeat block
         //

         case 7:

            //
            // If finite repeat block active, check and decrement repeat
            // count
            //

            if (sample->system_data[VOC_N_REPS] != 0xffff)
               {
               if (sample->system_data[VOC_N_REPS]-- == 0)
                  {
                  break;
                  }
               }

            b = (void FAR *) sample->system_data[VOC_REP_BLK];
            break;

         //
         // Extended attribute block
         // (followed by block 1)
         //

         case 8:

            //
            // Skip block if desired marker has not been found
            //

            if (!sample->system_data[VOC_MARKER_FOUND])
               {
               break;
               }

            //
            // Set up sample data and start playback
            //

            if (((BLK_8 FAR *) b)->voice_mode)
               {
               SS_set_sample_type(sample,DIG_F_STEREO_8,0);

               AIL_set_sample_playback_rate(sample,(U32)
                  ( 128000000 / (((U32)(65536)) - ((BLK_8 FAR *) b)->time_constant)));
               }
            else
               {
               SS_set_sample_type(sample,DIG_F_MONO_8,0);

               AIL_set_sample_playback_rate(sample,(U32)
                  ( 256000000 / (((U32)(65536)) - ((BLK_8 FAR *) b)->time_constant)));
               }

            //
            // Advance to paired voice block (type 1) in .VOC image
            //

            b = (C8 FAR *)AIL_ptr_add(b, AIL_VOC_block_len(b) + 4);

            //
            // Set sample address and size, and start playback
            //

            AIL_set_sample_address(sample,
                                   AIL_ptr_add(b, sizeof(BLK_1)),
                                   AIL_VOC_block_len(b) - 2);

            if (play_flag)
               AIL_start_sample(sample);

            voice_block = 1;
            break;

         //
         // Extended voice block
         //

         case 9:

            //
            // Skip block if desired marker has not been found
            //

            if (!sample->system_data[VOC_MARKER_FOUND])
               {
               break;
               }

            //
            // Set up sample data and start playback
            //

            AIL_set_sample_address(sample,
                                   AIL_ptr_add(b, sizeof(BLK_9)),
                                   AIL_VOC_block_len(b) - 12);

            AIL_set_sample_playback_rate(sample, ((BLK_9 FAR *) b)->sample_rate);

            if ((((BLK_9 FAR *) b)->channels == 1) &&
                (((BLK_9 FAR *) b)->format   == 0))
               {
               SS_set_sample_type(sample,DIG_F_MONO_8,0);
               }
            else if ((((BLK_9 FAR *) b)->channels == 2) &&
                     (((BLK_9 FAR *) b)->format   == 0))
               {
               SS_set_sample_type(sample,DIG_F_STEREO_8,0);
               }
            else if ((((BLK_9 FAR *) b)->channels == 1) &&
                     (((BLK_9 FAR *) b)->format   == 4))
               {
               SS_set_sample_type(sample,DIG_F_MONO_16,DIG_PCM_SIGN);
               }
            else if ((((BLK_9 FAR *) b)->channels == 2) &&
                     (((BLK_9 FAR *) b)->format   == 4))
               {
               SS_set_sample_type(sample,DIG_F_STEREO_16,DIG_PCM_SIGN);
               }

            if (play_flag)
               AIL_start_sample(sample);

            voice_block = 1;
            break;
         }

      //
      // Advance pointer to next block in .VOC image
      //

      sample->system_data[VOC_BLK_PTR] = (SINTa)
                                         AIL_ptr_add(b, AIL_VOC_block_len(b) + 4);
      }
}

//############################################################################
//##                                                                        ##
//## End-of-sample callback handler for .VOC file playback                  ##
//##                                                                        ##
//############################################################################

//static void AILLIBCALLBACK AIL_VOC_EOS( HSAMPLE sample )
//{
//  AIL_process_VOC_block( sample, 1 );
//}


//############################################################################
//##                                                                        ##
//## Create sample instance by parsing .WAV file                            ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_API_WAV_info(void const FAR* data, AILSOUNDINFO FAR* info)
{
   void  FAR *end;
   FMT   FAR *f;
   DATA  FAR *d;
   FACT  FAR *fa;

   DOLOCK();

   if (data==0)
     goto error;

   end=AIL_ptr_add(data,LEU_SWAP32(&((RIFF FAR *) data)->chunk_size));

   if (AIL_strnicmp((char FAR*)(((RIFF FAR *) data)->ID_string),"WAVE",4))
      {
     error:
      AIL_strcpy(AIL_error,"Not a WAV file.");
      return(0);
      }

   //
   // Find mandatory <fmt-ck>
   //

   f = (FMT FAR *)(void FAR*) (((RIFF *) data)->data);

   while (AIL_strnicmp((char FAR*)f->FMT_string,"fmt ",4))
      {
      U32 chunk_size = LEU_SWAP32( &f->chunk_size );
      f = (FMT FAR *) AIL_ptr_add(f, chunk_size + 8 + (chunk_size & 1));
      if (AIL_ptr_dif(f,end)>=0)
        goto error;
      }

   info->format= LE_SWAP16( &f->format_tag );

#ifdef IS_XENON
   //
   // Is this an XMA file?  If so, *f marks an XMAWAVEFORMAT structure
   //

   if (info->format == XWAVE_FORMAT_XMA)
      {
      XXMAWAVEFORMAT *xf = (XXMAWAVEFORMAT *) AIL_ptr_add(f, 8);

      info->rate       = LEU_SWAP32(&xf->XmaStreams[0].SampleRate);
      info->bits       = LE_SWAP16(&xf->BitsPerSample);
      info->channels   =            xf->XmaStreams[0].Channels;
      info->block_size = 2048;

      //
      // XMA data pointer is the beginning of the .WAV image itself
      // We have no way of knowing how many samples are in the file...
      //

      info->initial_ptr = data;
      info->data_ptr    = data;
      info->data_len    = AIL_ptr_dif(data, end);
      info->samples     = 0;
   
      return 1;
      }
#endif

   //
   // Not an XMA file -- must be standard ADPCM or PCM .WAV
   //

   info->rate= LEU_SWAP32( &f->sample_rate );
   info->bits= LE_SWAP16( &f->bits_per_sample );
   info->channels= LE_SWAP16( &f->channels );
   info->block_size= LE_SWAP16( &f->alignment );

   //
   // Find mandatory <data-ck>
   //

   d = (DATA FAR *)(void FAR*) (((RIFF *) data)->data);

   while (AIL_strnicmp((char FAR*)d->DATA_string,"data",4))
      {
      U32 chunk_size = LEU_SWAP32( &d->chunk_size );
      d = (DATA FAR *) AIL_ptr_add( d, chunk_size + 8 + (chunk_size & 1));
      if (AIL_ptr_dif(d,end)>=0)
        goto error;
      }

   info->data_ptr=d->data;
   info->data_len= LEU_SWAP32( &d->chunk_size );

   if ((info->format==WAVE_FORMAT_IMA_ADPCM) && (info->bits==4))
   {

     fa = (FACT FAR *)(void FAR*) (((RIFF *) data)->data);

     while (AIL_strnicmp((char FAR*)fa->FACT_string,"fact",4))
        {
        U32 chunk_size = LEU_SWAP32( &fa->chunk_size );
        fa = (FACT FAR *) AIL_ptr_add( fa, chunk_size + 8 + (chunk_size & 1));
        if (AIL_ptr_dif(fa,end)>=0)
          {

          U32 samples_per_block = 4 << (info->channels/2);
          samples_per_block = 1 + (info->block_size-samples_per_block)*8 / samples_per_block;
          info->samples=((info->data_len+info->block_size-1)/info->block_size)*samples_per_block;

          goto nofact;
          }
       }
       info->samples= LEU_SWAP32( &fa->samples );
     nofact:;
   } else
     info->samples=(info->bits==0)?0:((info->data_len*8)/info->bits);

   info->initial_ptr=info->data_ptr;

   return(1);
}


//############################################################################
//##                                                                        ##
//## Create sample instance by parsing .WAV file                            ##
//##                                                                        ##
//############################################################################

void AIL_process_WAV_image( AILSOUNDINFO FAR const * info, HSAMPLE sample )
{
  S32 format=0;
  S32 sign=0;

  if ( info->channels == 2 )
  {
    format = DIG_F_STEREO_MASK ;
  }

#ifdef IS_XBOX
  if ( info->format == WAVE_FORMAT_XBOX_ADPCM )
  {
    format |= DIG_F_XBOX_ADPCM_MASK;
  }
  else
#endif
  {
    switch( info->bits )
    {
      case 4 :
        format |= DIG_F_ADPCM_MASK ;
      case 16:
        format |= DIG_F_16BITS_MASK ;
        sign   = DIG_PCM_SIGN ;
    }
  }

  SS_set_sample_type( sample, format, sign );

  AIL_set_sample_playback_rate( sample, info->rate );

  //
  // Configure adpcm if required
  //

  if ( format & DIG_F_ADPCM_MASK )
  {
    AIL_set_sample_adpcm_block_size( sample, info->block_size );
  }

  //
  // Configure sample address and length based on DATA chunk
  //

  AIL_set_sample_address( sample, info->data_ptr, info->data_len );
}

//############################################################################
//#                                                                          #
//# Initiate playback of sample, calling driver or voice filter as           #
//# necessary                                                                #
//#                                                                          #
//############################################################################

static void SS_start_playback(HSAMPLE S)
{
#ifdef MSS_VFLT_SUPPORTED
   if (S->voice != NULL)
      {
      HDIGDRIVER dig = S->driver;

      dig->voice_filter->start_sample_voice(dig->voice_filter->driver_state,
                                            S);
      return;
      }
#endif

   SS_start_DIG_driver_playback(S);
}

//############################################################################
//##                                                                        ##
//## Update sample's 3D positional attributes for multichannel output       ##
//##                                                                        ##
//############################################################################

void AILCALL SS_set_speaker_configuration (D3DSTATE        *D3D, //)
                                           MSSVECTOR3D FAR *speaker_positions,
                                           S32              n_channels,
                                           S32              logical_channels_per_sample)
{
   S32 i,spkr;
   
   if (n_channels > logical_channels_per_sample)
      {
      n_channels = logical_channels_per_sample;
      }

   for (i=0; i < n_channels; i++)
      {
      D3D->speaker_positions[i] = speaker_positions[i];
      }

   //
   // Create a default receiver constellation 
   // 
   // This is a set of worldspace points on the unit sphere surrounding the listener, 
   // coincident with directional speakers by default, possibly including a virtual speaker
   // 

   D3D->n_receiver_specs = 0;

   //
   // Assign each speaker to an ambient or directional channel slot
   //
   // Send each directional speaker vector through a 3x3 rotation matrix corresponding 
   // to the listener's orientation
   //
   // Also calculate unweighted centroid (average) of directional speaker locations for
   // virtual-speaker placement calculation
   //

   D3D->n_directional_channels = 0;
   D3D->n_ambient_channels     = 0;

   MSSVECTOR3D V;

   V.x = 0.0F;
   V.y = 0.0F;
   V.z = 0.0F;

   F32 len;

   for (spkr=0; spkr < logical_channels_per_sample; spkr++)   
      {
      //
      // Get unit vector from listener to speaker 
      //

      MSSVECTOR3D P = D3D->speaker_positions[spkr];

      len = RAD_vector_length(&P);

      if (len < MSS_EPSILON)
         {
         //
         // Can't determine direction -- speaker is too close to listener
         //

         D3D->ambient_channels[D3D->n_ambient_channels++] = spkr;
         continue;
         }

      S32 index = D3D->n_directional_channels++;

      D3D->directional_channels[index] = spkr;

      P.x /= len;
      P.y /= len;
      P.z /= len;

      MSSVECTOR3D *C0 = &D3D->listen_cross;    // +X column unit vector
      MSSVECTOR3D *C1 = &D3D->listen_up;       // +Y column unit vector 
      MSSVECTOR3D *C2 = &D3D->listen_face;     // +Z column unit vector 
   
      D3D->listener_to_speaker[index].x = (P.x * C0->x) + (P.y * C1->x) + (P.z * C2->x); 
      D3D->listener_to_speaker[index].y = (P.x * C0->y) + (P.y * C1->y) + (P.z * C2->y); 
      D3D->listener_to_speaker[index].z = (P.x * C0->z) + (P.y * C1->z) + (P.z * C2->z); 

      V.x += D3D->listener_to_speaker[index].x;
      V.y += D3D->listener_to_speaker[index].y;
      V.z += D3D->listener_to_speaker[index].z;

      D3D->receiver_specifications[D3D->n_receiver_specs  ].direction           = D3D->listener_to_speaker[index];
      D3D->receiver_specifications[D3D->n_receiver_specs  ].speaker_index[0]    = index;
      D3D->receiver_specifications[D3D->n_receiver_specs  ].speaker_level[0]    = 1.0F;
      D3D->receiver_specifications[D3D->n_receiver_specs++].n_speakers_affected = 1;
      }

   if (D3D->n_directional_channels > 0)
      {
      V.x /= (F32)D3D->n_directional_channels;
      V.y /= (F32)D3D->n_directional_channels; 
      V.z /= (F32)D3D->n_directional_channels; 

      //
      // See if a virtual directional speaker needs to be created to keep dot products from going
      // past their 180-degree wraparound points
      //

      len = RAD_vector_length(&V);

      if (len > 0.0001F)
         {
         //
         // First, normalize the vector to move it to the surface of the unit sphere
         // Then invert it to move it as far away from the existing speakers as possible
         //

         V.x = -(V.x / len);
         V.y = -(V.y / len);
         V.z = -(V.z / len);

         //
         // If virtual speaker location is less than 90 degrees from any real speaker, we 
         // don't need it
         //

         for (i=0; i < D3D->n_directional_channels; i++)
            {
            F32 cosine = RAD_vector_dot_product(&D3D->listener_to_speaker[i],
                                                &V);
            if (cosine > 0.0F)
               {
               break;
               }
            }

         if (i == D3D->n_directional_channels)
            {
            //
            // Add virtual speaker with channel=-1 to list of directional speakers
            //

            S32 index = D3D->n_directional_channels++;

            D3D->directional_channels[index] = -1;
            D3D->listener_to_speaker [index] = V;

            D3D->receiver_specifications[D3D->n_receiver_specs  ].direction           = D3D->listener_to_speaker[index];
            D3D->receiver_specifications[D3D->n_receiver_specs  ].speaker_index[0]    = index;
            D3D->receiver_specifications[D3D->n_receiver_specs  ].speaker_level[0]    = 1.0F;
            D3D->receiver_specifications[D3D->n_receiver_specs++].n_speakers_affected = 1;
            }
         }
      }
}

F32 AILCALLBACK SS_default_falloff_function_callback(HSAMPLE   S, //)
                                                     F32       distance,
                                                     F32       rolloff_factor,
                                                     F32       min_dist,
                                                     F32       max_dist)
{
   //
   // Don't access S here -- it may be NULL (e.g, when called from 
   // AIL_calculate_3D_channel_levels())
   //

   return min_dist / (min_dist + (rolloff_factor * (distance - min_dist)));
}

S32 AILCALL SS_calculate_3D_attenuation(D3DSTATE    FAR *D3D, //)
                                        S3DSTATE    FAR *S3D,
                                        F32 FAR         *distance,
                                        MSSVECTOR3D FAR *listener_to_source,
                                        AILFALLOFFCB     falloff_function)
{
   RAD_vector_subtract(listener_to_source, &S3D->position, &D3D->listen_position);

   *distance = RAD_vector_length(listener_to_source);
 
   //
   // Normalize source-listener vector for cone, speaker-angle, and Doppler calculations
   // 

   RAD_vector_normalize(listener_to_source);

   //
   // Return 0 if sound is muted due to max-distance constraint
   //

   if (D3D->mute_at_max)
      {
      if (*distance > S3D->max_dist)
         {
         S3D->atten_3D = 0.0F;
         return 0;
         }
      }
   else
      {
      //
      // Inhibit further attenuation at max dist
      //
 
      if (*distance > S3D->max_dist)
         {
         *distance = S3D->max_dist;
         }
      }
 
   //
   // Calculate distance attenuation by inverse-square law for sources > min_dist away from
   // the listener
   //
 
   F32 min = S3D->min_dist;
 
   if (min < MSS_EPSILON)
      {
      min = MSS_EPSILON;
      }
 
   S3D->atten_3D = 1.0F;

   if (*distance > (min + MSS_EPSILON))
      {
      //
      // By default, the volume of a sound is attenuated by 6dB at twice 
      // the minimum distance from the listener, 6 dB again at 4X the minimum distance, 
      // and so on
      //
      // This behavior can be overridden via a user falloff callback in cases where the function
      // isn't being called on a 3D hardware driver; otherwise, the default DS3D-style handler
      // is used
      //
      
      if (falloff_function != NULL)
         {
         MSS_do_cb5_with_ret( S3D->atten_3D,
                             (AILFALLOFFCB),
                              falloff_function,
                             (S3D->owner == NULL) ? 0 : S->driver->callingDS,
                              S3D->owner, 
                             *distance,
                              D3D->rolloff_factor,
                              min,
                              S3D->max_dist);
         }
      }

   //
   // Apply cone falloff if enabled
   //

   if (S3D->cone_enabled)
      {
      //
      // Get angle between source's (normalized) face and listener vectors
      //
      // The source's face vector is negated so it can be dotted with the 
      // listener-to-source vector
      //

      MSSVECTOR3D negated_face = S3D->face;
      negated_face.x = -negated_face.x;
      negated_face.y = -negated_face.y;
      negated_face.z = -negated_face.z;

      F32 cosine = RAD_vector_dot_product(&negated_face,
                                           listener_to_source);

      F32 cone_angle;

           if (cosine >  0.9999F) cone_angle = 0.0F; 
      else if (cosine < -0.9999F) cone_angle = MY_PI;
      else                        cone_angle = (F32)AIL_acos(cosine);

      //
      // If angle is less than inner angle, volume is unaltered
      // If angle is greater than outer angle, volume is scaled by outer_volume
      // Otherwise, volume is lerped between volume and volume*outer_volume
      //

      if (cone_angle > S3D->outer_angle)
         {
         S3D->atten_3D *= S3D->outer_volume;
         }
      else
         {
         if (cone_angle >= S3D->inner_angle)
            {
            F32 d = S3D->outer_angle - S3D->inner_angle;

            if (d > MSS_EPSILON)
               {
               // alpha=1.0 when angle=inner (level is scaled by 1.0)
               //       0.0 when angle=outer (level is scaled by outer volume)

               F32 alpha = (S3D->outer_angle - cone_angle) / d;  
               
               F32 scalar = S3D->outer_volume + ((1.0F - S3D->outer_volume) * alpha);

               S3D->atten_3D *= scalar;
               }
            }
         }
      }

   //
   // Return 1 to indicate that sound is not past max distance
   //

   return 1;
}

void SS_calculate_3D_channel_levels(D3DSTATE          FAR *D3D, //)
                                    S32                    logical_channels_per_sample,
                                    S3DSTATE          FAR *S3D,
                                    F32               FAR *channel_levels)
{
   //
   // Get 3D attenuation factor, plus normalized vector from listener to source
   //

   MSSVECTOR3D listener_to_source;
   F32         distance;

   if (!SS_calculate_3D_attenuation(D3D, 
                                    S3D,
                                   &distance,
                                   &listener_to_source,
                                    S3D->falloff_function))
      {
      //
      // Sample is muted due to exceeding max distance with D3D.mute_at_max enabled
      //
      // Note: we now do this even if status==SMP_STOPPED ...
      //

      for (S32 i=0; i < logical_channels_per_sample; i++)
         {
         channel_levels[i] = 0.0F;
         }

      return;
      }

   //
   // Get base volume attenuation
   //
 
   F32 vol = S3D->atten_3D;

   //
   // If source and listener are coincident, divide unscaled 3D volume level linearly among 
   // all channels
   //

   S32 spkr;

   if (distance <= MSS_EPSILON)
      {
      for (spkr=0; spkr < logical_channels_per_sample; spkr++)   
         {
         channel_levels[spkr] = vol * AIL_fsqrt(1.0F / logical_channels_per_sample);
         }
      }
   else
      {
      F32 speaker_contributions[MAX_SPEAKERS];     // Physical speakers, indexed by [0,logical_channels_per_sample-1]
      AIL_memset(speaker_contributions, 0, sizeof(speaker_contributions));

      S32 i;

      //
      // Send proportion of sound to ambient radiators
      //

      F32 frac = AIL_fsqrt(1.0F / logical_channels_per_sample);

      for (i=0; i < D3D->n_ambient_channels; i++)
         {
         spkr = D3D->ambient_channels[i];

         speaker_contributions[spkr] = vol * frac;
         }

      //
      // Record angle between source-listener vector and receiver-listener vector for each
      // receiver point
      //
      // Keep track of the sum of the angles
      //

      F32 reception_angle[MAX_RECEIVER_SPECS];
      F32 theta_sum = 0.0F;

      for (i=0; i < D3D->n_receiver_specs; i++)
         {
         MSS_RECEIVER_LIST *RCVR = &D3D->receiver_specifications[i];

         F32 cosine = RAD_vector_dot_product(&RCVR->direction,
                                             &listener_to_source);

              if (cosine >  0.9999F) reception_angle[i] = MY_PI;
         else if (cosine < -0.9999F) reception_angle[i] = 0.0F;
         else                        reception_angle[i] = MY_PI - (F32) AIL_acos(cosine);   // pi = loud, 0 = quiet 

         theta_sum += reception_angle[i];
         }

      //
      // Perform constant-power pan among all directional channels, including 
      // virtual speaker if present
      //

      for (i=0; i < D3D->n_receiver_specs; i++)
         {
         MSS_RECEIVER_LIST *RCVR = &D3D->receiver_specifications[i];

         F32 src_lvl = AIL_fsqrt(AIL_pow(reception_angle[i], D3D->falloff_power) / theta_sum);          

         for (S32 j=0; j < RCVR->n_speakers_affected; j++)
            {
            spkr = RCVR->speaker_index[j];

            F32 contrib = src_lvl * RCVR->speaker_level[j];

            S32 phys_spkr = D3D->directional_channels[spkr];

            if (phys_spkr == -1)
               {
               //
               // This is a virtual speaker
               //
               // If we don't spread a little of the virtual-speaker's contribution among the other speakers,
               // then mono sources directly behind the listener will drop out entirely.  This calculation
               // doesn't have to be physically-accurate
               //

               for (S32 ii=0; ii < D3D->n_receiver_specs; ii++)
                  {
                  MSS_RECEIVER_LIST *RCVR = &D3D->receiver_specifications[ii];

                  for (S32 ij=0; ij < RCVR->n_speakers_affected; ij++)
                     {
                     S32 phys_spkr = D3D->directional_channels[RCVR->speaker_index[ij]];

                     if (phys_spkr != -1)
                        {
                        speaker_contributions[phys_spkr] += (contrib / (D3D->n_receiver_specs * 2));
                        }
                     }
                  }

               continue;
               }

            speaker_contributions[phys_spkr] += contrib;
            }
         }

      //
      // Final channel level calculation
      //
      // Attenuate by 0-6 dB for falloff powers = 1.0 - 4.0 to help compensate for 
      // volume-boosting effect at higher falloff powers
      //

      F32 falloff_compensation = AIL_fsqrt(D3D->falloff_power);

      for (spkr=0; spkr < logical_channels_per_sample; spkr++)
         {
         F32 lvl = speaker_contributions[spkr] / falloff_compensation;

              if (lvl <= 0.0F) lvl = 0.0F;
         else if (lvl >= 1.0F) lvl = 1.0F;

         channel_levels[spkr] = lvl * vol;
         }
      }

   //
   // Enable Doppler frequency-shifting if relative velocity is nonzero
   //
 
   MSSVECTOR3D movement_dir;
   RAD_vector_subtract(&movement_dir, &S3D->velocity, &D3D->listen_velocity);

   F32 velocity;
   RAD_vector_normalize(&movement_dir, &velocity);

   S3D->doppler_valid = 0;

   if (velocity > MSS_EPSILON)
      {
      //
      // Sign of velocity determined by whether object is approaching
      // source or receding from it
      //

      velocity *= RAD_vector_dot_product(&listener_to_source,
                                         &movement_dir);

      velocity *= D3D->distance_factor * D3D->doppler_factor;

      if (velocity > SPEED_OF_SOUND)
         {
         S3D->doppler_shift = MIN_DOPPLER_SHIFT;
         }
      else if (velocity < -SPEED_OF_SOUND) 
         {
         S3D->doppler_shift = MAX_DOPPLER_SHIFT;
         }
      else
         {
         F32 v = SPEED_OF_SOUND + velocity;

         S3D->doppler_shift = SPEED_OF_SOUND / v;

         if (S3D->doppler_shift > MAX_DOPPLER_SHIFT)      S3D->doppler_shift = MAX_DOPPLER_SHIFT;
         else if (S3D->doppler_shift < MIN_DOPPLER_SHIFT) S3D->doppler_shift = MIN_DOPPLER_SHIFT; 
         }

      S3D->doppler_valid = 1;
      }
}

//############################################################################
//#                                                                          #
//# Save HSAMPLE performance attributes                                      #
//#                                                                          #
//############################################################################

void AILCALL AIL_API_save_sample_attributes(HSAMPLE   S, //)
                                            HSATTRIBS D)
{
   if ((S == NULL) || (D == NULL))
      {
      return;
      }

   memset(D, 0, sizeof(*D));

   //
   // Save all post-initialization performance attributes for samples.  
   // These include filter properties, but not ASI decoder properties or, in general, any 
   // information used to describe the source data at initialization time such as 
   // formats and file names/suffixes
   //
   // This function can be applied to streaming HSAMPLEs, but the streaming and double-buffering
   // states are not saved
   //

   D->is_3D = AIL_sample_3D_position(S,
                                    &D->X_pos,
                                    &D->Y_pos,
                                    &D->Z_pos);

   if (D->is_3D)
      {
      AIL_sample_3D_cone(S, 
                        &D->cone_inner_angle, 
                        &D->cone_outer_angle, 
                        &D->cone_outer_volume);

      AIL_sample_3D_distances(S, 
                             &D->max_distance,
                             &D->min_distance,
                             &D->auto_3D_wet_atten);


      AIL_sample_3D_orientation(S,
                               &D->X_face,
                               &D->Y_face,
                               &D->Z_face,
                               &D->X_up,
                               &D->Y_up,
                               &D->Z_up);

      AIL_sample_3D_velocity(S,
                            &D->X_meters_per_ms,
                            &D->Y_meters_per_ms,
                            &D->Z_meters_per_ms);
      }
   else
      {
      //
      // Read destination channel levels
      //
      // If this is a 3D sample, these values are written by the 3D positioner and
      // don't need to be read or restored
      //

      F32 *levels = AIL_sample_channel_levels(S, &D->n_channel_levels);

      if (D->n_channel_levels > MAX_SPEAKERS) D->n_channel_levels = MAX_SPEAKERS;

      memcpy(D->channel_levels,
             levels,
             sizeof(D->channel_levels[0]) * D->n_channel_levels);
      }

   D->exclusion   = AIL_sample_exclusion(S);
   D->obstruction = AIL_sample_obstruction(S);
   D->occlusion   = AIL_sample_occlusion(S);

   D->loop_count = AIL_sample_loop_count(S);

   AIL_sample_loop_block(S,
                        &D->loop_start_offset,
                        &D->loop_end_offset);

   D->LPF_cutoff = AIL_sample_low_pass_cut_off(S);

   D->playback_rate = AIL_sample_playback_rate(S);

   D->position = AIL_sample_position(S);

   AIL_sample_reverb_levels(S,
                           &D->dry_level,
                           &D->wet_level);

   D->playback_status = AIL_sample_status(S);
   
   //
   // Read either mono/stereo or 5.1-specific source volume levels
   //

#ifdef IS_XBOX
   if (1)
#else
   if ((S->driver->channel_spec == MSS_MC_51_DISCRETE) ||
       (S->driver->channel_spec == MSS_MC_51_DTS))
#endif
      {
      AIL_sample_51_volume_levels(S,
                                 &D->FL_51_level,
                                 &D->FR_51_level,
                                 &D->BL_51_level,
                                 &D->BR_51_level,
                                 &D->C_51_level,
                                 &D->LFE_51_level);
      }
   else
      {
      AIL_sample_volume_levels(S,
                              &D->left_volume,
                              &D->right_volume);
      }

   //
   // Record up to a total of MAX_SAVED_FILTER_PROPERTIES properties from all installed 
   // sample pipeline filters, including any output filter in use
   //
   // Additional filter properties, or those that are read-only, not simple 32-bit scalar
   // types, or are otherwise not restorable, are ignored.  (This means that most EAX 4 
   // structure properties won't be preserved by this function...)
   //

   SAMPLESTAGE stage;
   S32 n = 0;

   for (stage = (SAMPLESTAGE) 0; stage < N_SAMPLE_STAGES; stage = (SAMPLESTAGE) (stage + 1))
      {
      if (((stage < SP_FILTER_0) || (stage > SP_FILTER_7)) && (stage != SP_OUTPUT))
         {
         continue;
         }

      HPROVIDER filter = AIL_sample_processor(S, stage); 

      if (filter == 0)
         {
         continue;
         }

      RIB_INTERFACE_ENTRY entry;

      HINTENUM next = HPROENUM_FIRST;

      while (AIL_enumerate_filter_sample_properties(filter, 
                                                   &next,
                                                   &entry))
         {
         if ((n >= MAX_SAVED_FILTER_PROPERTIES) ||
             (entry.type != RIB_PROPERTY)       ||
             (entry.subtype & RIB_READONLY)     ||
             ((entry.subtype != RIB_DEC)   && (entry.subtype != RIB_HEX)     &&
              (entry.subtype != RIB_FLOAT) && (entry.subtype != RIB_PERCENT) &&
              (entry.subtype != RIB_BOOL)))
            {
            continue;
            }

         U32 value;         

         if (!AIL_sample_stage_property(S,
                                        stage,
                                        entry.entry_name,
                                       &value,
                                        NULL,
                                        NULL))
            {
            continue;
            }

         D->filter_stages         [n] = stage;
         D->filter_providers      [n] = filter;
         D->filter_property_tokens[n] = entry.token;
         D->filter_property_values[n] = value;
         n++;
         }
      }

   D->n_filter_properties = n;
}

//############################################################################
//#                                                                          #
//# Load HSAMPLE performance attributes from a previously-saved record       #
//#                                                                          #
//############################################################################

U32 AILCALL AIL_API_load_sample_attributes(HSAMPLE   S, //)
                                           HSATTRIBS D)
{
   //
   // We don't initialize the HSAMPLE here -- it's assumed the application has already
   // done so
   //

   if ((S == NULL) || (D == NULL))
      {
      return 0;
      }

   if (D->is_3D)
      {
      AIL_set_sample_3D_position(S,
                                 D->X_pos,
                                 D->Y_pos,
                                 D->Z_pos);

      AIL_set_sample_3D_cone(S, 
                             D->cone_inner_angle, 
                             D->cone_outer_angle, 
                             D->cone_outer_volume);

      AIL_set_sample_3D_distances(S, 
                                  D->max_distance,
                                  D->min_distance,
                                  D->auto_3D_wet_atten);
                                 
      AIL_set_sample_3D_orientation(S,
                                    D->X_face,
                                    D->Y_face,
                                    D->Z_face,
                                    D->X_up,
                                    D->Y_up,
                                    D->Z_up);

      AIL_set_sample_3D_velocity_vector(S,
                                        D->X_meters_per_ms,
                                        D->Y_meters_per_ms,
                                        D->Z_meters_per_ms);
      }                                 
   else
      {
      AIL_set_sample_channel_levels(S, 
                                    D->channel_levels,
                                    D->n_channel_levels);
      }

   AIL_set_sample_exclusion  (S, D->exclusion);
   AIL_set_sample_obstruction(S, D->obstruction);
   AIL_set_sample_occlusion  (S, D->occlusion);

   if (S->service_type == 1)     // Don't restore position and looping info for double-buffered samples
      {
      AIL_set_sample_loop_block(S,
                                D->loop_start_offset,
                                D->loop_end_offset);

      AIL_set_sample_loop_count(S, 
                                D->loop_count);

      AIL_set_sample_position(S,  
                              D->position);
      }

   AIL_set_sample_playback_rate(S,
                                D->playback_rate);

   AIL_set_sample_low_pass_cut_off(S,
                                   D->LPF_cutoff);

   AIL_set_sample_reverb_levels(S,
                                D->dry_level,
                                D->wet_level);

   //
   // Set either mono/stereo or 5.1-specific source volume levels
   //

#ifdef IS_XBOX
   if (1)
#else
   if ((S->driver->channel_spec == MSS_MC_51_DISCRETE) || 
       (S->driver->channel_spec == MSS_MC_51_DTS))
#endif
      {
      AIL_set_sample_51_volume_levels(S,
                                      D->FL_51_level,
                                      D->FR_51_level,
                                      D->BL_51_level,
                                      D->BR_51_level,
                                      D->C_51_level,
                                      D->LFE_51_level);
      }
   else
      {
      AIL_set_sample_volume_levels(S,
                                   D->left_volume,
                                   D->right_volume);
      }

   //
   // Install any needed pipeline filters and restore their properties
   //
   // Existing filters are NOT replaced by this function.  Output
   // filter properties are updated, but we assume the filter itself was
   // installed when the HSAMPLE was initialized
   //

   for (S32 n=0; n < D->n_filter_properties; n++)
      {
      SAMPLESTAGE stage    = D->filter_stages[n];
      HPROVIDER   provider = D->filter_providers[n];

      HPROVIDER cur = AIL_sample_processor(S, stage); 

      if ((cur != provider) && (stage != SP_OUTPUT))
         {
         if (cur != 0)
            {
            continue;         // A different provider was already installed in this slot
            }

         AIL_set_sample_processor(S,
                                  stage,
                                  provider);
         }

      RIB_INTERFACE_ENTRY entry;                   // Find property whose token matches the one
      HINTENUM            next  = HPROENUM_FIRST;  // we logged...
      S32                 found = FALSE;

      while (AIL_enumerate_filter_sample_properties(provider,
                                                   &next,
                                                   &entry))
         {
         if (entry.token == D->filter_property_tokens[n])
            {
            found = TRUE;
            break;
            }
         }

      if (!found) continue;     

      AIL_sample_stage_property(S,               
                                stage,           
                                entry.entry_name,
                                NULL,
                               &D->filter_property_values[n],
                                NULL);
      }
                               
   //
   // Return previous playback state, so the application can restore it if/when desired
   //

   return D->playback_status;
}

//############################################################################
//##                                                                        ##
//## Make MSS 3D positioning algorithm available to API                     ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_API_calculate_3D_channel_levels(HDIGDRIVER       driver, //)
                                                F32         FAR *channel_levels,
                                                MSSVECTOR3D FAR *src_pos,
                                                MSSVECTOR3D FAR *src_face,
                                                MSSVECTOR3D FAR *src_up,
                                                F32              src_inner_angle,
                                                F32              src_outer_angle,
                                                F32              src_outer_volume,
                                                F32              src_max_dist,
                                                F32              src_min_dist,
                                                MSSVECTOR3D FAR *listen_pos, 
                                                MSSVECTOR3D FAR *listen_face,
                                                MSSVECTOR3D FAR *listen_up,  
                                                F32              rolloff_factor,
                                                MSSVECTOR3D FAR *doppler_velocity,
                                                F32             *doppler_shift)
{
   if ((driver == NULL) || (driver->logical_channels_per_sample == 0) || (channel_levels == NULL))
      {
      return 0;
      }

   //
   // Reproduce behavior of AIL_set_listener_3D_orientation() with a D3D object that
   // inherits its speaker positions and falloff power (but not its listener transform) 
   // from the driver
   //

   D3DSTATE D3D;
   AIL_memcpy(&D3D, &driver->D3D, sizeof(D3D));

   D3D.rolloff_factor    = rolloff_factor;
   D3D.listen_velocity.x = D3D.listen_velocity.y = D3D.listen_velocity.z = 0.0F;

   D3D.listen_position = *listen_pos;
   D3D.listen_face     = *listen_face;
   D3D.listen_up       = *listen_up;

   RAD_vector_normalize(&D3D.listen_face);
   RAD_vector_normalize(&D3D.listen_up);

   RAD_vector_cross_product(&D3D.listen_cross,   
                            &D3D.listen_face, 
                            &D3D.listen_up);

   RAD_vector_normalize(&D3D.listen_cross);

   SS_set_speaker_configuration(&D3D,
                                 NULL,
                                 0,
                                 driver->logical_channels_per_sample);

   //
   // Create and initialize a source object without an associated HSAMPLE, and declare
   // our static channel-level array
   //
   // For Doppler calculations, we apply the optional Doppler velocity parameter to the source, 
   // leaving the listener velocity at zero
   //

   S3DSTATE S3D;
   AIL_memset(&S3D, 0, sizeof(S3D));

   if (doppler_velocity != NULL)
      {
      S3D.velocity = *doppler_velocity;
      }

   S3D.position = *src_pos;
   S3D.face     = *src_face;
   S3D.up       = *src_up;

   S3D.inner_angle  = src_inner_angle;
   S3D.outer_angle  = src_outer_angle;
   S3D.outer_volume = src_outer_volume;
   S3D.cone_enabled = (src_inner_angle < 360.0F) && (src_outer_volume < 1.0F);

   S3D.max_dist         = src_max_dist;
   S3D.min_dist         = src_min_dist;
   S3D.dist_changed     = TRUE;
   S3D.owner            = NULL;
   S3D.falloff_function = SS_default_falloff_function_callback;

   //
   // Calculate channel levels via 3D positioner
   //

   SS_calculate_3D_channel_levels(&D3D,
                                   driver->logical_channels_per_sample,
                                  &S3D,
                                   channel_levels);

   //
   // Return Doppler scale factor if velocity above minimum epsilon value, else 1.0
   //

   if ((doppler_velocity != NULL) && (doppler_shift != NULL))
      {
      if (S3D.doppler_valid)
         *doppler_shift = S3D.doppler_shift;
      else
         *doppler_shift = 1.0F;  
      }

   return driver->logical_channels_per_sample;
}

//############################################################################
//##                                                                        ##
//## Set parameters of existing HSAMPLE according to info struct            ##
//##                                                                        ##
//## Returns 0 on error, else 1                                             ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_API_set_sample_info(HSAMPLE S, AILSOUNDINFO const FAR * info)
{
  S32 format=0;
  S32 sign=0;
  HPROVIDER HP = 0;

  DOLOCK();

   if ((S==NULL) || (info==NULL))
     return(0);

  if ( info->channels == 2 )
  {
    format = DIG_F_STEREO_MASK;
  }

#ifdef IS_XBOX
  if ( info->format == WAVE_FORMAT_XBOX_ADPCM )
  {
    format |= DIG_F_XBOX_ADPCM_MASK;
  }
  else
#endif
  {
    if (info->format==WAVE_FORMAT_PCM)
    {
      if ( info->bits == 16 ) 
      {
        format |= DIG_F_16BITS_MASK;
        sign   = DIG_PCM_SIGN;
      }
      else if ( info->bits != 8 )
      {
        goto wave_err;
      }
    }
    else if ((info->format==WAVE_FORMAT_IMA_ADPCM) && (info->bits==4))
    {
      format |= DIG_F_ADPCM_MASK;
    }    
    else
    {
      // find asi
      HP = RIB_find_file_dec_provider( "ASI codec",
                                       "Input wave tag",
                                       info->format,
                                       "Output file types",
                                       ".raw" );

      if (!HP)
      {
       wave_err: 
        AIL_set_error("Unsupported wave file format.");
        return 0;
      }
    }
  }

  AIL_init_sample              ( S, format, sign );
  AIL_set_sample_playback_rate ( S, info->rate );

  //
  // Configure adpcm if required
  //

  if ( format & DIG_F_ADPCM_MASK )
  {
    AIL_set_sample_adpcm_block_size( S, info->block_size );
  }

  //
  // Configure sample address and length based on DATA chunk
  //

  AIL_set_sample_address( S, info->data_ptr, info->data_len );

  //
  // Set up to use specified ASI codec to decode data for mixer
  //

  AIL_set_sample_processor(S,
                           SP_ASI_DECODER,
                           HP);

  return( 1 );                           
}

//############################################################################
//##                                                                        ##
//## Set parameters of existing HSAMPLE according to file data              ##
//##                                                                        ##
//## Returns 0 on error, else 1                                             ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_API_set_sample_file(HSAMPLE S, void const FAR *file_image, S32 block)
{
   S32 type;
   AILSOUNDINFO info;

   DOLOCK();

   if ((S==NULL) || (file_image==NULL))
     return(0);

   AIL_init_sample(S, DIG_F_MONO_16, 0);

   //
   // Identify file type
   //
   //

   if (!AIL_strnicmp((char FAR*)(((VOC FAR *) file_image)->ID_string),"Creative",8))
      {
      type = FTYP_VOC;
      }
   else if (!AIL_strnicmp((char FAR*) file_image,"OggS",4))
      {
      //
      // If "Speex" present in first 128 bytes of file image, use .spx decoder
      // Otherwise, we'll assume it's a Vorbis (.ogg) file
      //

      C8 *suffix = ".ogg";

      for (S32 i=0; i < 128; i++)
         {
         if (!AIL_strnicmp((C8 *) AIL_ptr_add(file_image,i), "Speex", 5))
            {
            strcpy(suffix,".spx");
            break;
            }
         }

      HPROVIDER ASI = find_ASI_provider("Input file types",
                                        suffix);
      if ( ASI )
        {
          //
          // Set up to use specified ASI codec to decode data for mixer
          //
  
          AIL_set_sample_processor(S,
                                   SP_ASI_DECODER,
                                   ASI);
          return( 1 );
        }
      else
         {
        AIL_set_error("Unsupported wave file format.");
        return 0;
        }
      }
   else if (AIL_WAV_info(file_image,&info))
      {
      if ( (info.format==WAVE_FORMAT_PCM)
           || ((info.format==WAVE_FORMAT_IMA_ADPCM) && (info.bits==4))
#ifdef IS_XBOX
           || ((info.format==WAVE_FORMAT_XBOX_ADPCM) && (info.bits==4))
#endif
         )
        type = FTYP_WAV;
      else
        {

        // handle asi wave files

        HPROVIDER ASI = RIB_find_file_dec_provider( "ASI codec",
                                                    "Input wave tag",
                                                    info.format,
                                                    "Output file types",
                                                    ".raw" );
        if ( ASI )
        {
          //
          // set sample address and size for encoded data image
          //
  
          AIL_set_sample_address(S,info.data_ptr,info.data_len );
  
          //
          // Set up to use specified ASI codec to decode data for mixer
          //
  
          AIL_set_sample_processor(S,
                                   SP_ASI_DECODER,
                                   ASI);
          return( 1 );
        }

        AIL_set_error("Unsupported wave file format.");
        return 0;
        }
      }
   else
      {
      AIL_set_error("Unknown file type.");
      return 0;
      }

   //
   // Copy file attributes to sample
   //

   switch (type)
      {
      case FTYP_VOC:

         S->system_data[VOC_BLK_PTR]      = (UINTa) AIL_ptr_add(file_image,
                                                 ((VOC FAR *) file_image)->
                                                 data_offset);

         S->system_data[VOC_MARKER]       = block;
         S->system_data[VOC_MARKER_FOUND] = (block == -1);

         AIL_process_VOC_block(S,0);
         break;

      case FTYP_WAV:

         AIL_process_WAV_image(&info,S);
         break;

      default:

         AIL_set_error("Invalid or missing data block.");
         return 0;
      }

   return 1;
}

//############################################################################
//##                                                                        ##
//## Set parameters of existing HSAMPLE according to file data, using       ##
//## the file suffix to specify the file type                               ##
//##                                                                        ##
//## Returns 0 on error, else 1                                             ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_API_set_named_sample_file (HSAMPLE   S, //)
                                           C8   const FAR *file_type_suffix,
                                           void const FAR *file_image,
                                           U32       file_size,
                                           S32       block)
{
   DOLOCK();

   if ((S==NULL) || (file_image==NULL))
     return(0);

   //
   // is this a wacky id3?
   //

   if ( ( ((U8 const FAR* )file_image)[ 0 ] == 0x49 ) && ( ((U8 const FAR* )file_image)[ 1 ] == 0x44 ) && ( ((U8 const FAR* )file_image)[ 2 ] == 0x33 ) &&
        ( ((U8 const FAR* )file_image)[ 3 ] < 0xff ) && ( ((U8 const FAR* )file_image)[ 4 ] < 0xff ) &&
        ( ((U8 const FAR* )file_image)[ 6 ] < 0x80 ) && ( ((U8 const FAR* )file_image)[ 7 ] < 0x80 ) && ( ((U8 const FAR* )file_image)[ 8 ] < 0x80 ) && ( ((U8 const FAR* )file_image)[ 9 ] < 0x80 ) )
   {
     U32 skip = 10 + ( (U32) ((U8 const FAR* )file_image)[ 9 ] ) | ( ( (U32) ((U8 const FAR* )file_image)[ 8 ] ) << 7 ) |
                   ( ( (U32) ((U8 const FAR* )file_image)[ 7 ] ) << 14 ) | ( ( (U32) ((U8 const FAR* )file_image)[ 6 ] ) << 21 );

     file_image = ( (U8 FAR*) file_image ) + skip;
     file_size -= skip;
     file_type_suffix = ".mp3";
   }

   //
   // If file type not specified, pass to .WAV / VOC handler
   //

   if ((file_type_suffix == NULL) ||
       (!AIL_strlen(file_type_suffix)))
      {
      return AIL_set_sample_file(S,
                                 file_image,
                                 block);
      }

   AIL_init_sample(S, DIG_F_MONO_16, 0);

   //
   // Search for ASI codec capable of processing this input file type
   //

   HPROVIDER HP = find_ASI_provider("Input file types",
                                     file_type_suffix);

   //
   // If no specific provider available for this file type, try the default
   // .WAV/.VOC file handler
   //

   if (!HP)
      {
      if (AIL_strlen(file_type_suffix) >= 4)
         {
         if ((!AIL_stricmp(&file_type_suffix[AIL_strlen(file_type_suffix)-4],
                        ".WAV"))
               ||
            (!AIL_stricmp(&file_type_suffix[AIL_strlen(file_type_suffix)-4],
                        ".VOC")))
            {
            return AIL_set_sample_file(S,
                                       file_image,
                                       block);
            }
         }

      //
      // Not a .WAV or .VOC file, fail call
      //

      AIL_set_error( "Couldn't find provider for this file type -- check your AIL_set_redist_directory() call." );

      return 0;
      }


   // reset default filter values

   clear_last_sample(S);

   //
   // Otherwise, set sample address and size for encoded data image
   //

   AIL_set_sample_address(S,
                          file_image,
                          file_size);

   //
   // Set up to use specified ASI codec to decode data for mixer
   //

   AIL_set_sample_processor(S,
                            SP_ASI_DECODER,
                            HP);

   HPROVIDER check = AIL_sample_processor(S, 
                                          SP_ASI_DECODER);

   if (check == 0)
      {
      AIL_set_error("ASI provider for this file type failed to initialize");
      return 0;
      }

   //
   // Return success
   //

   return 1;
}

//############################################################################
//##                                                                        ##
//## Get RIB provider, if any, for specified sample pipeline phase          ##
//##                                                                        ##
//############################################################################

HPROVIDER    AILCALL AIL_API_sample_processor  (HSAMPLE     S, //)
                                                SAMPLESTAGE pipeline_stage)
{
   S32 stage = pipeline_stage;

   if (stage == SAMPLE_ALL_STAGES)
      {
      stage = 0;
      }

   if (S->pipeline[stage].active == 0)
      {
      return 0;
      }

   return S->pipeline[stage].provider;
}

//############################################################################
//##                                                                        ##
//## Set a RIB provider for a specified sample pipeline phase               ##
//##                                                                        ##
//############################################################################

HPROVIDER    AILCALL AIL_API_set_sample_processor  (HSAMPLE     S, //)
                                                    SAMPLESTAGE pipeline_stage,
                                                    HPROVIDER   provider)
{
   DOLOCK();

#ifdef IS_XBOX
   //
   // If format is Xbox ADPCM, we can only use direct mode to play it -- no filtering
   // allowed
   //
   // A new SP_ASI_DECODER can be specified here, since it will actually change the format
   // of the sample
   //
   
   if ((S->format & DIG_F_XBOX_ADPCM_MASK) && (pipeline_stage != SP_ASI_DECODER))
      {
      return 0;
      }

#endif

   S32 stage = pipeline_stage;

   if (stage == SAMPLE_ALL_STAGES)
      {
      stage = 0;
      }

   SPINFO FAR *SP;

   do
      {
      SP = &S->pipeline[stage];

      //
      // If existing stage is being replaced, shut it down first
      //

      if (SP->active)
         {
         switch (stage)
            {
            //
            // Output voice filter
            //

            case SP_OUTPUT:
               break;

            //
            // "MSS mixer services"
            //

            case SP_MERGE:
               break;

            //
            // "ASI codec stream"
            //

            case SP_ASI_DECODER:
               SP->TYPE.ASI.ASI_stream_close(SP->TYPE.ASI.stream);
               break;

            //
            // "MSS pipeline filter"
            //

            case SP_FILTER_0:
            case SP_FILTER_1:
            case SP_FILTER_2:
            case SP_FILTER_3:
            case SP_FILTER_4:
            case SP_FILTER_5:
            case SP_FILTER_6:
            case SP_FILTER_7:

               #ifdef MSS_FLT_SUPPORTED
               SP->TYPE.FLT.provider->close_sample(SP->TYPE.FLT.sample_state);
               SP->TYPE.FLT.sample_state=0;
               --S->n_active_filters;
               #endif
               break;
            }

         SP->active = 0;
         }
      }
   while ((pipeline_stage == SAMPLE_ALL_STAGES) && (++stage < N_SAMPLE_STAGES));

   //
   // Set up to return previous provider
   //

   HPROVIDER previous = SP->provider;

   SP->provider = provider;

   //
   // If no new provider specified, exit normally after shutting down
   // existing provider
   //

   if (!provider)
      {
      return previous;
      }

   //
   // Otherwise, we must exit direct mode on the Xbox (possibly causing a huge glitch if 
   // performed after starting the sample...)
   //

#ifdef IS_XBOX
  S->direct_mode = 0;
#endif

   //
   // Query required attributes from provider interface
   //

   switch (stage)
      {
      //
      // Output voice filter
      //

      case SP_OUTPUT:
         break;

      //
      // "MSS mixer services"
      //

      case SP_MERGE:
         {
         //
         // Get codec properties and store in pipeline-stage data structure
         //

         RIB_INTERFACE_ENTRY MIXER[] =
            {
            { RIB_FUNCTION, "MIXER_merge", (UINTa) &SP->TYPE.MSS_mixer_merge, RIB_NONE },
            };

         RIB_request(SP->provider,"MSS mixer services",MIXER);

         //
         // Indicate installation of mixer module
         //

         SP->active = 1;
         break;
         }

      //
      // "MSS pipeline filter"
      //

      case SP_FILTER_0:
      case SP_FILTER_1:
      case SP_FILTER_2:
      case SP_FILTER_3:
      case SP_FILTER_4:
      case SP_FILTER_5:
      case SP_FILTER_6:
      case SP_FILTER_7:
         {

         #ifdef MSS_FLT_SUPPORTED

         //
         // open the filter if it ain't already opened
         //

         if (!AIL_open_filter(provider, S->driver))
            {
            SP->provider = 0;
            return previous;
            }

         //
         // Find DRIVERSTATE wrapper for this provider and driver
         //

         FLTPROVIDER FAR *F = FLT_find_provider_instance(provider, S->driver);

         SP->TYPE.FLT.provider = F;

         SP->TYPE.FLT.sample_state = 0;

         if ((F==NULL) || (F->open_sample==NULL))
           {
           SP->provider = 0;
           return previous;
           }

         //
         // Create SAMPLESTATE structure for this HSAMPLE and HDRIVERSTATE
         //

         SP->TYPE.FLT.sample_state = F->open_sample(F->driver_state,
                                                    S);

         if (!SP->TYPE.FLT.sample_state)
           {
           SP->provider = 0;
           return previous;
           }

         //
         // Indicate installation of filter module
         //

         SP->active = 1;

         ++S->n_active_filters;

         #endif

         break;
         }

      //
      // "ASI codec stream"
      //

      case SP_ASI_DECODER:
         {
         //
         // Get codec properties and store in pipeline-stage data structure
         // Do this in blocks of <= 4 entries to avoid external lib call warning on PS2 compiler
         //

         ASISTAGE FAR *ASI = &SP->TYPE.ASI;

         RIB_INTERFACE_ENTRY ASISTR1[] =
            {
            { RIB_FUNCTION,   "ASI_stream_property",       (UINTa) &ASI->ASI_stream_property,       RIB_NONE },
            { RIB_FUNCTION,   "ASI_stream_open",           (UINTa) &ASI->ASI_stream_open,           RIB_NONE },
            { RIB_FUNCTION,   "ASI_stream_seek",           (UINTa) &ASI->ASI_stream_seek,           RIB_NONE },
            { RIB_FUNCTION,   "ASI_stream_close",          (UINTa) &ASI->ASI_stream_close,          RIB_NONE }
            };

         RIB_INTERFACE_ENTRY ASISTR2[] =
            {
            { RIB_FUNCTION,   "ASI_stream_process",        (UINTa) &ASI->ASI_stream_process,        RIB_NONE },
            { RIB_PROPERTY,   "Input bit rate",            (UINTa) &ASI->INPUT_BIT_RATE,            RIB_NONE },
            { RIB_PROPERTY,   "Input sample rate",         (UINTa) &ASI->INPUT_SAMPLE_RATE,         RIB_NONE },
            };

         RIB_INTERFACE_ENTRY ASISTR3[] =
            {
            { RIB_PROPERTY,   "Input sample width",        (UINTa) &ASI->INPUT_BITS,                RIB_NONE },
            { RIB_PROPERTY,   "Input channels",            (UINTa) &ASI->INPUT_CHANNELS,            RIB_NONE },
            { RIB_PROPERTY,   "Output bit rate",           (UINTa) &ASI->OUTPUT_BIT_RATE,           RIB_NONE },
            { RIB_PROPERTY,   "Output sample rate",        (UINTa) &ASI->OUTPUT_SAMPLE_RATE,        RIB_NONE },
            };


         RIB_INTERFACE_ENTRY ASISTR4[] =
            {
            { RIB_PROPERTY,   "Output sample width",       (UINTa) &ASI->OUTPUT_BITS,               RIB_NONE },
            { RIB_PROPERTY,   "Output channels",           (UINTa) &ASI->OUTPUT_CHANNELS,           RIB_NONE },
            { RIB_PROPERTY,   "Output reservoir",          (UINTa) &ASI->OUTPUT_RESERVOIR,          RIB_NONE },
            { RIB_PROPERTY,   "Position",                  (UINTa) &ASI->POSITION,                  RIB_NONE },
            };

         RIB_INTERFACE_ENTRY ASISTR5[] =
            {
            { RIB_PROPERTY,   "Percent done",              (UINTa) &ASI->PERCENT_DONE,              RIB_NONE },
            { RIB_PROPERTY,   "Minimum input block size",  (UINTa) &ASI->MIN_INPUT_BLOCK_SIZE,      RIB_NONE },
            { RIB_PROPERTY,   "Raw source sample rate",    (UINTa) &ASI->RAW_RATE,                  RIB_NONE },
            { RIB_PROPERTY,   "Raw source sample width",   (UINTa) &ASI->RAW_BITS,                  RIB_NONE },
            };

         RIB_INTERFACE_ENTRY ASISTR6[] =
            {
            { RIB_PROPERTY,   "Raw source channels",       (UINTa) &ASI->RAW_CHANNELS,              RIB_NONE },
            { RIB_PROPERTY,   "Requested sample rate",     (UINTa) &ASI->REQUESTED_RATE,            RIB_NONE },
            { RIB_PROPERTY,   "Requested sample width",    (UINTa) &ASI->REQUESTED_BITS,            RIB_NONE },
            { RIB_PROPERTY,   "Requested channels",        (UINTa) &ASI->REQUESTED_CHANS,           RIB_NONE }
            };

         RIB_INTERFACE_ENTRY ASISTR7[] =
            {
            { RIB_PROPERTY,   "Stream seek position",      (UINTa) &ASI->STREAM_SEEK_POS,           RIB_NONE },
            { RIB_PROPERTY,   "Data offset",               (UINTa) &ASI->DATA_START_OFFSET,         RIB_NONE },
            { RIB_PROPERTY,   "Data length",               (UINTa) &ASI->DATA_LEN,                  RIB_NONE },
            };

         RIB_request(SP->provider,"ASI stream",ASISTR1);
         RIB_request(SP->provider,"ASI stream",ASISTR2);
         RIB_request(SP->provider,"ASI stream",ASISTR3);
         RIB_request(SP->provider,"ASI stream",ASISTR4);
         RIB_request(SP->provider,"ASI stream",ASISTR5);
         RIB_request(SP->provider,"ASI stream",ASISTR6);
         RIB_request(SP->provider,"ASI stream",ASISTR7);

         //
         // Open stream with codec, registering callback function
         //

         OutMilesMutex();
         ASI->stream = ASI->ASI_stream_open((UINTa) S,
                                                  SP_ASI_DECODER_callback,
                                                  S->len[0]);
         InMilesMutex();

         // jkr - exit on error
         if (ASI->stream==0)
         {
           SP->provider=0;
           return(previous);
         }

         //
         // Request codec output format which matches hardware format
         //

         ASI->ASI_stream_property(ASI->stream,
                                  ASI->REQUESTED_RATE,
                                  0,&S->driver->DMA_rate,0);

         U32 achans = 1 + ((S->driver->hw_format & (DIG_F_STEREO_MASK | DIG_F_MULTICHANNEL_MASK)) != 0);

         ASI->ASI_stream_property(ASI->stream,
                                  ASI->REQUESTED_CHANS,
                                  0,&achans,0);

         U32 abits=8 + (8 * ((S->driver->hw_format & DIG_F_16BITS_MASK) != 0));

         ASI->ASI_stream_property(ASI->stream,
                                  ASI->REQUESTED_BITS,
                                  0,&abits,0);

         //
         // Configure sample type and rate according to codec's actual output
         // format
         //

         U32 nch  = (U32)~0; ASI->ASI_stream_property(ASI->stream, ASI->OUTPUT_CHANNELS,&nch,0,0);
         U32 rate = (U32)~0; ASI->ASI_stream_property(ASI->stream, ASI->OUTPUT_SAMPLE_RATE,&rate,0,0);
         U32 bits = (U32)~0; ASI->ASI_stream_property(ASI->stream, ASI->OUTPUT_BITS,&bits,0,0);

         AIL_set_sample_playback_rate(S, rate);

         if (nch == 2)
            {
            SS_set_sample_type(S,
                              (bits == 16) ? DIG_F_STEREO_16 : DIG_F_STEREO_8,
                              (U32)( (bits == 16) ? DIG_PCM_SIGN    : 0));
            }
         else
            {
            SS_set_sample_type(S,
                              (bits == 16) ? DIG_F_MONO_16   : DIG_F_MONO_8,
                              (U32)( (bits == 16) ? DIG_PCM_SIGN    : 0 ));
            }

         //
         // If ASI codec has identified a data chunk within the sample file image, adjust 
         // our sample address and length parameters accordingly
         //
         // Henceforth, position 0 refers to the start of the data chunk, rather than the 
         // beginning of the file...
         //

         if (S->service_type == 1)
            {
            if (ASI->DATA_START_OFFSET != (UINTa)~0)
               {
               U32 offset = 0; ASI->ASI_stream_property(ASI->stream, ASI->DATA_START_OFFSET,&offset,0,0);
               
               S->start[0] = AIL_ptr_add(S->start[0], offset );
               }

            if (ASI->DATA_LEN != (UINTa)~0)
               {
               ASI->ASI_stream_property(ASI->stream, ASI->DATA_LEN,&S->len[0],0,0);
               }
            }

         //
         // Indicate installation of decoder module
         //

         SP->active = 1;
         break;
         }
      }

   //
   // Return success
   //

   return previous;
}

//############################################################################
//##                                                                        ##
//## Set a RIB provider for a specified driver pipeline phase               ##
//##                                                                        ##
//############################################################################

HPROVIDER    AILCALL AIL_API_digital_driver_processor(HDIGDRIVER  dig, //)
                                                      DIGDRVSTAGE pipeline_stage)
{
   if ( dig == 0 )
     return( 0 );

   S32 stage = pipeline_stage;

   if (stage == DIGDRV_ALL_STAGES)
      {
      stage = 0;
      }

   if (!dig->pipeline[stage].active)
      {
      return 0;
      }

   return dig->pipeline[stage].provider;
}

//############################################################################
//##                                                                        ##
//## Set a RIB provider for a specified driver pipeline phase               ##
//##                                                                        ##
//############################################################################

HPROVIDER    AILCALL AIL_API_set_digital_driver_processor(HDIGDRIVER  dig, //)
                                                          DIGDRVSTAGE pipeline_stage,
                                                          HPROVIDER   provider)
{
   DOLOCK();

   if ( dig == 0 )
     return( 0 );

   S32 stage = pipeline_stage;

   if (stage == DIGDRV_ALL_STAGES)
      {
      stage = 0;
      }

   DPINFO FAR *DP;

   //
   // handle the "default" pipeline stages specially
   //

   if ((pipeline_stage==DP_DEFAULT_FILTER) || (pipeline_stage==DP_DEFAULT_MERGE))
      {
      HPROVIDER previous=dig->pipeline[pipeline_stage].provider;
      dig->pipeline[pipeline_stage].provider=provider;
      return(previous);
      }

   do
      {
      DP = &dig->pipeline[stage];

      //
      // If existing stage is being replaced, shut it down first
      //

      if (DP->active)
         {
         switch (stage)
            {
            //
            // "MSS mixer services"
            //

            case DP_FLUSH:
            case DP_COPY:
            case DP_MC_COPY:
            case DP_ADPCM_DECODE:
               break;
            }

         DP->active = 0;
         }
      }
   while ((pipeline_stage == DIGDRV_ALL_STAGES) && (++stage < N_DIGDRV_STAGES));

   //
   // If no new provider specified, exit normally after shutting down
   // existing provider
   //

   if (!provider)
      {
      return 0;
      }

   //
   // Set up to return previous provider
   //

   HPROVIDER previous = DP->provider;

   DP->provider = provider;

   //
   // Query required attributes from provider interface
   //

   switch (stage)
      {
      //
      // "MSS mixer services"
      //

      case DP_FLUSH:
         {
         //
         // Get codec properties and store in pipeline-stage data structure
         //

         RIB_INTERFACE_ENTRY MIXER[] =
            {
            { RIB_FUNCTION, "MIXER_flush", (UINTa) &DP->TYPE.MSS_mixer_flush, RIB_NONE },
            };

         RIB_request(DP->provider,"MSS mixer services",MIXER);

         //
         // Indicate installation of mixer module
         //

         DP->active = 1;
         break;
         }

      case DP_COPY:
         {
         //
         // Get codec properties and store in pipeline-stage data structure
         //

         RIB_INTERFACE_ENTRY MIXER[] =
            {
            { RIB_FUNCTION, "MIXER_copy",  (UINTa) &DP->TYPE.MSS_mixer_copy,  RIB_NONE }
            };

         RIB_request(DP->provider,"MSS mixer services",MIXER);

         //
         // Indicate installation of mixer module
         //

         DP->active = 1;
         break;
         }

      case DP_MC_COPY:
         {
         //
         // Get codec properties and store in pipeline-stage data structure
         //

         RIB_INTERFACE_ENTRY MIXER[] =
            {
            { RIB_FUNCTION, "MIXER_mc_copy",  (UINTa) &DP->TYPE.MSS_mixer_mc_copy,  RIB_NONE }
            };

         RIB_request(DP->provider,"MSS mixer services",MIXER);

         //
         // Indicate installation of mixer module
         //

         DP->active = 1;
         break;
         }

      case DP_ADPCM_DECODE:
         {
         //
         // Get codec properties and store in pipeline-stage data structure
         //

         RIB_INTERFACE_ENTRY MIXER[] =
            {
            { RIB_FUNCTION, "MIXER_adpcm_decode",  (UINTa) &DP->TYPE.MSS_mixer_adpcm_decode,  RIB_NONE }
            };

         RIB_request(DP->provider,"MSS mixer services",MIXER);

         //
         // Indicate installation of mixer module
         //

         DP->active = 1;
         break;
         }
      }

   //
   // Return success
   //

   return previous;
}

//############################################################################
//##                                                                        ##
//## Runtime profiling                                                      ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_API_digital_CPU_percent(HDIGDRIVER dig)
{
  U32 time;
  S32 diff;

  if (dig==0)
    return(0);

  time=AIL_ms_count();

  diff=(S32)time-(S32)dig->last_ms_polled;
  if (diff<150)
    return(dig->last_percent);

  dig->last_ms_polled=time;

  dig->ms_count+=(dig->us_count/1000);
  dig->us_count=dig->us_count%1000;

  time=(diff)?((dig->ms_count*100)/diff):0;
  dig->ms_count=0;

  dig->last_percent=time;

  if (time>100)
    time=100;

  return((S32)time);
}

#define min(a,b)  (((a) < (b)) ? (a) : (b))

static S32 get_operation(U32 dest_rate,U32 dest_format,U32 src_rate,U32 src_bits, U32 src_chans)
{
  //
  // Set mixer operation code
  //

  S32 op = 0;

  if (dest_format & DIG_F_STEREO_MASK)
     {
     op |= M_DEST_STEREO;
     }

  if (src_chans==2)
     {
     op |= M_SRC_STEREO;
     }

  if (src_bits!=8)
     {
     op |= M_SRC_16;
     }

  //
  // Set M_SRC_SCALE
  //
  // Force M_SRC_SCALE if stereo source mixed into mono output
  //
  // (This routine assumes no volume scaling in use)
  //

  if ((op & M_SRC_STEREO) && (!(op & M_DEST_STEREO)))
     {
     op |= M_VOL_SCALING;
     }

  if (src_rate != dest_rate)
     {
     op |= M_RESAMPLE;
     }

  //
  // Enable filtering if preference set
  //

  if ((AIL_get_preference(DIG_ENABLE_RESAMPLE_FILTER)) && ((src_rate) != dest_rate))
     {
     op |= M_FILTER;
     }

  return(op);
}

//############################################################################
//##                                                                        ##
//## Return how much memory a conversion will require                       ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_API_size_processed_digital_audio(
                                 U32             dest_rate,
                                 U32             dest_format,
                                 S32             num_srcs,
                                 AILMIXINFO const FAR* src)
{
   if ((src==0) || (num_srcs==0))
     return(0);

   // a point is one sample in mono, or two samples in stereo -
   // it is one sample point in time

   S32 i;
   S32 max_src_points=0;

   //calculate the sample points for each input
   for(i=0;i<num_srcs;i++) {
     S32 points=src[i].Info.data_len;
     if (src[i].Info.format==WAVE_FORMAT_IMA_ADPCM)
       points<<=1;    //2 adpcm samples in a byte
     else
       if (src[i].Info.bits!=8)
         points>>=1;  //2 bytes in a 16-bit sample
     if (src[i].Info.channels==2)
       points>>=1;    //2 samples in a stereo point

     // adjust number of points for sample rate differences
#ifdef IS_PS2 
     points = (S32) mult64anddiv(points, dest_rate, src[i].Info.rate);
#else
     points=(S32)(((F64)points*(F64)dest_rate)/((F64)src[i].Info.rate));
#endif

     // keep track of the greatest number of points
     if (points>max_src_points)
       max_src_points=points;
   }

   // figure number of dest points
   S32 dest_point_size=((dest_format&DIG_F_STEREO_MASK)?2:1)*((dest_format&DIG_F_16BITS_MASK)?2:1);

   return( (dest_point_size*max_src_points)+256 );  // 256 for slop

}

//############################################################################
//##                                                                        ##
//## Ask MSS to reset the ASI at the end of the specified buffer            ##
//##                                                                        ##
//############################################################################

DXDEC  void     AILCALL AIL_request_EOB_ASI_reset   (HSAMPLE S,
                                                     U32     buff_num,
                                                     S32     seek_position)
{
  if (S)
  {
   if (buff_num == (U32)~0)  // ~0 = cancel any pending reset request (e.g., when the stream is restarted)
      {
      S->reset_ASI     [0] = S->reset_ASI     [1] = FALSE;
      S->reset_seek_pos[0] = S->reset_seek_pos[1] = 0;
      }
   else
      {
      S->reset_ASI     [buff_num] = TRUE;
      S->reset_seek_pos[buff_num] = seek_position;
      }
  }
}


//############################################################################
//##                                                                        ##
//## Convert data from one sample rate and format to another                ##
//##                                                                        ##
//############################################################################

#define NUMBUILDSAMPLES 2048

S32 AILCALL AIL_API_process_digital_audio(
                                 void FAR       *dest_buffer,
                                 S32             dest_buffer_size,
                                 U32             dest_rate,
                                 U32             dest_format,
                                 S32             num_srcs,
                                 AILMIXINFO FAR* src)
{

   DOLOCK();
   UINTa nbytes_written=0;

   if ((src==0) || (dest_buffer==0) || (num_srcs==0))
     return(0);

   S32 adpcm_size=0;
   S16 FAR*adpcm_temp_buffer=0;

   S32 dest_chan=((dest_format&DIG_F_STEREO_MASK)?2:1);

   //
   // Get driver mixer providers
   //

   MIXER_FLUSH MIXER_flush;
   MIXER_MERGE MIXER_merge;
   MIXER_COPY  MIXER_copy;
   MIXER_ADPCM_DECODE  MIXER_adpcm_decode;

   HPROVIDER HP;


   RIB_enumerate_providers("MSS mixer services",
                            NULL,
                           &HP);

   RIB_INTERFACE_ENTRY MIXER[] =
      {
      FN(MIXER_merge),
      FN(MIXER_flush),
      FN(MIXER_copy),
      FN(MIXER_adpcm_decode)
      };

   RIB_request(HP,"MSS mixer services",MIXER);


   S32 i;

   //calculate all of the operations
   S32 operations[256];
   for(i=0;i<num_srcs;i++)
     operations[i]=get_operation(dest_rate,dest_format,src[i].Info.rate,src[i].Info.bits,src[i].Info.channels);

  keepconverting:

   // a point is one sample in mono, or two samples in stereo -
   // it is one sample point in time

   S32 max_src_points=0;

   //calculate the sample points for each input
   for(i=0;i<num_srcs;i++) {
     S32 points=src[i].Info.data_len;
     if (src[i].Info.format==WAVE_FORMAT_IMA_ADPCM)
       points<<=1; //2 adpcm samples in a byte
     else
       if (src[i].Info.bits!=8)
         points>>=1;  //2 bytes in a 16-bit sample
     if (src[i].Info.channels==2)
       points>>=1;  //2 samples in a stereo point

     // adjust number of points for sample rate differences
#ifdef IS_PS2   
     points = (S32) mult64anddiv(points, dest_rate, src[i].Info.rate);
#else
     points=(S32)(((F64)points*(F64)dest_rate)/((F64)src[i].Info.rate));
#endif

     // keep track of the greatest number of points
     if (points>max_src_points)
       max_src_points=points;
   }

   // figure number of dest points
   S32 dest_point_size=dest_chan*((dest_format&DIG_F_16BITS_MASK)?2:1);
   S32 dest_points=dest_buffer_size/dest_point_size;

   //limit our outputs to the maximum number of inputs
   if (max_src_points<dest_points)
     dest_points=max_src_points;

   //just exit if we don't have any mixing to do
   if (dest_points==0)
     return (S32) nbytes_written;


   // Init persistent variables for all passes
   for(i=0;i<num_srcs;i++) {
     if ((src[i].mss_adpcm.blocksize!=src[i].Info.block_size) || (src[i].Info.initial_ptr==src[i].Info.data_ptr)) {
       src[i].mss_adpcm.blocksize    = src[i].Info.block_size;
       src[i].mss_adpcm.blockleft    = 0;
       src[i].mss_adpcm.extrasamples = 0;
       src[i].src_fract=0;
       src[i].left_val=0;
       src[i].right_val=0;
     }
   }

   //ok, get down to the mixing
   U8 FAR* dest=(U8 FAR*)dest_buffer;

   S32 build_buffer[NUMBUILDSAMPLES];
   S32 build_points=NUMBUILDSAMPLES/dest_chan;

   while (dest_points) {

     //figure out how many points we're going to do this pass
     S32 points=build_points;
     if (points>dest_points)
       points=dest_points;

     S32 build_size=points*sizeof(S32)*dest_chan;

     //clear the output buffer
     MIXER_flush( build_buffer,
                  build_size
#ifdef IS_X86
                  ,(S32) AIL_get_preference(AIL_ENABLE_MMX_SUPPORT)
#endif
                  );

     SINTa max_built_size=0;

     //now loop through the srcs mixing in a piece of each src's buffer
     for(i=0;i<num_srcs;i++) {

       //do we have anything to mix?
       if (src[i].Info.data_len==0)
         continue;

       void const FAR* mix_ptr;
       S32 mix_size;

       // if we're dealing with ADPCM, we have to decompress into a temp buffer first
       if (src[i].Info.format==WAVE_FORMAT_IMA_ADPCM) {

         void const FAR* src_orig=src[i].Info.data_ptr;

#ifdef IS_PS2 
         mix_size = 2 * (S32) mult64anddiv(points, src[i].Info.rate, dest_rate) * src[i].Info.channels;
#else
         mix_size=2*((S32)(((((F64)points)*((F64)src[i].Info.rate))/(F64)dest_rate)+0.5F))*src[i].Info.channels;
#endif

         //make sure the buffer is big enough
         if (mix_size>adpcm_size) {
           if (adpcm_temp_buffer)
             AIL_mem_free_lock(adpcm_temp_buffer);
           adpcm_size=mix_size;
           adpcm_temp_buffer=(S16 FAR*)AIL_mem_alloc_lock(adpcm_size);
           if (adpcm_temp_buffer==0)
             return(0);
         }

         S16 FAR* temp=adpcm_temp_buffer;

         MIXER_adpcm_decode(&temp, &src[i].Info.data_ptr, mix_size, src[i].Info.data_len, 
                            operations[i], &src[i].mss_adpcm );

         U32 src_moved=(U32) AIL_ptr_dif(src[i].Info.data_ptr,src_orig);
         src[i].Info.data_len=(src[i].Info.data_len<=src_moved)?0:(src[i].Info.data_len-src_moved);

         mix_ptr=adpcm_temp_buffer;

         //don't worry about the fractional bit with adpcm
         src[i].src_fract=0;

       } else {

         mix_ptr=src[i].Info.data_ptr;
         mix_size=src[i].Info.data_len;

       }


       //
       // Call mixer provider
       //

       #ifdef IS_32

         void const FAR* mix_orig=mix_ptr;
         S32 FAR* build=build_buffer;

         InMilesMutex();

         MIXER_merge(&mix_ptr,
                     &src[i].src_fract,
                      AIL_ptr_add( mix_ptr, mix_size),
                     &build,
                      AIL_ptr_add( build, build_size),
                     &src[i].left_val,
                     &src[i].right_val,
#ifdef IS_PS2
                      (S32) mult64anddiv(src[i].Info.rate, 65536, dest_rate),
#else
                      (S32)((((F64) src[i].Info.rate) * 65536.0F) / (F64) dest_rate),
#endif
                      2048,
                      2048,
                      operations[i]
#ifdef IS_X86
                      ,(S32) AIL_get_preference(AIL_ENABLE_MMX_SUPPORT)
#endif
                      );

         OutMilesMutex();

         S32 built_size=(S32) AIL_ptr_dif(build,build_buffer);
         U32 src_moved=(U32) AIL_ptr_dif(mix_ptr,mix_orig);

       #else

         U32 src_offset  = LOWORD(mix_ptr);
         U32 dest_offset = LOWORD(build_buffer);

         MIXER_merge(HIWORD(mix_ptr),
                     HIWORD(build_buffer),
                    &src[i].src_fract,
                    &src_offset,
                    &dest_offset,
                     src_offset+mix_size,
                     dest_offset+build_size,
                    &src[i].left_val,
                    &src[i].right_val,
#ifdef IS_PS2
                     (S32) mult64anddiv(src[i].Info.rate, 65536, dest_rate),
#else
                     (S32)((((F64) src[i].Info.rate) * 65536.0F) / (F64) dest_rate),  
#endif
                     (2048 << 16) | 2048,
                     operations[i]);

         U32 src_moved=(src_offset - LOWORD(mix_ptr));
         S32 built_size=(dest_offset - LOWORD(build_buffer));

       #endif

       // don't adjust the pointers for adpcm (already done)
       if (!(src[i].Info.format==WAVE_FORMAT_IMA_ADPCM)) {
         src[i].Info.data_ptr = AIL_ptr_add(src[i].Info.data_ptr, src_moved);
         src[i].Info.data_len=(src[i].Info.data_len<=src_moved)?0:(src[i].Info.data_len-src_moved);
       }

       if (built_size>max_built_size)
         max_built_size=built_size;

     }

     if (max_built_size==0)
       break;

     //
     // Copy from the build buffer into dest
     //
     // We can't use the RIB provider on PS2, because of the
     // 256-sample left/right interleaved funkiness
     //

#ifdef IS_PS2
     PS2_noninterleaved_mixer_copy(build_buffer,
                                   max_built_size,
                                   dest,
                                   dest_format);
#else
     MIXER_copy( build_buffer,
           (S32) max_built_size,
                 dest,
                 dest_format
#ifdef IS_X86
                 ,(S32) AIL_get_preference(AIL_ENABLE_MMX_SUPPORT)
#else
#ifdef IS_BE
                 ,0
#endif
#endif
                 );
#endif

     dest=(U8 FAR*)AIL_ptr_add(dest,points*dest_point_size);
     dest_points-=points;
   }

   SINTa written=(SINTa) AIL_ptr_dif(dest,dest_buffer);
   nbytes_written+=written;
   dest_buffer=dest;
   dest_buffer_size-=(S32) written;

   if (dest_buffer_size)
     goto keepconverting;

   if (adpcm_temp_buffer)
     AIL_mem_free_lock(adpcm_temp_buffer);

   return (S32) nbytes_written;
}

//
// ASCII alternate redist directory
//

char AIL_redist_directory[260]="";

//############################################################################
//##                                                                        ##
//## Set the alternate MSS driver directory                                 ##
//##                                                                        ##
//############################################################################

extern "C"
char FAR*  AILEXPORT AIL_set_redist_directory(char const FAR* dir)
{
   U32 len;

   if ( ( dir == NULL ) || ( ( dir[ 0 ] == '.' ) && ( dir[ 1 ] == 0 ) ) )
      *AIL_redist_directory=0;
   else
      {
#if defined(IS_MAC) && !defined(IS_MACHO)
      AIL_strcpy(AIL_redist_directory,dir);

      len=AIL_strlen(AIL_redist_directory);
      if (len)
      {
        if (AIL_redist_directory[len-1]!=':')
        {
          AIL_redist_directory[len]=':';
          AIL_redist_directory[len+1]=0;
        }
      }
#else
      AIL_strcpy(AIL_redist_directory,dir);
      len=(S32) AIL_strlen(AIL_redist_directory);
      if (len)
         {
         if ((AIL_redist_directory[len-1]!='/') && (AIL_redist_directory[len-1]!='\\') && (AIL_redist_directory[len-1]!=':'))
            {
            AIL_redist_directory[len]='/';
            AIL_redist_directory[len+1]=0;
            }
         }
#endif
      }

   return (char FAR*) (AIL_redist_directory);
}


//############################################################################
//##                                                                        ##
//## Return number of actively playing samples for given driver             ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_API_active_sample_count( HDIGDRIVER dig )
{
#ifdef IS_XBOX
  return( dig->n_active_samples );
#else
  S32 i,n;

  if ( dig == 0 )
  {
    return( 0 );
  }

  n = 0;

  for ( i = 0 ; i < dig->n_samples ; i++ )
  {
    if ( ( dig->sample_status[ i ] == SMP_PLAYING ) || ( dig->sample_status[ i ] == SMP_PLAYINGBUTRELEASED ) )
    {
      ++n;
    }
  }

  return( n );
#endif
}

//############################################################################
//##                                                                        ##
//## Allocate a SAMPLE structure for use with a given driver                ##
//##                                                                        ##
//############################################################################

HSAMPLE AILCALL AIL_API_allocate_sample_handle( HDIGDRIVER dig )
{
  S32    i;
  HSAMPLE S;

  //
  // Exit if driver invalid
  //

  if ( dig == NULL )
  {
    return( 0 );
  }

  //
  // Lock timer services to prevent reentry
  //

  AIL_lock();

  //
  // Look for an unallocated sample structure
  //

  for ( i = 0 ; i < dig->n_samples ; i++ )
  {
    if ( dig->sample_status[ i ] == SMP_FREE )
      break;
  }

  //
  // If all structures in use, return NULL
  //

  if ( i == dig->n_samples )
  {
    AIL_set_error( "Out of sample handles." );

    AIL_unlock();
    return( 0 );
  }

  S = &dig->samples[ i ];

  //
  // Initialize sample to default (SMP_DONE) status with nominal
  // sample attributes
  //

  AIL_init_sample( S, DIG_F_MONO_16, 0 );

  //
  // If a voice filter is installed on this driver, call it to assign a voice to the 
  // HSAMPLE
  //
  // Failure means the sample handle is re-released
  //

#ifdef MSS_VFLT_SUPPORTED
  if (dig->voice_filter != NULL)
    {
    if (!dig->voice_filter->assign_sample_voice(dig->voice_filter->driver_state,
                                                S))
      {
      S->driver->sample_status[S->index] = SMP_FREE;
      AIL_set_error( "Hardware voice assignment failed." );

      AIL_unlock();
      return 0;
      }

    AIL_set_sample_processor(S,
                             SP_OUTPUT,
                             dig->voice_filter->provider);
    }
#endif

  AIL_unlock();
  return( S );
}

//############################################################################
//##                                                                        ##
//## Free a SAMPLE structure for later allocation                           ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_release_sample_handle( HSAMPLE S )
{
  if ( S == NULL )
  {
    return;
  }

  hardware_stop( S );

  S->driver->sample_status[S->index] = SMP_FREE;

  //
  // Shut down any installed pipeline providers
  //

#ifdef MSS_VFLT_SUPPORTED
  if (S->driver->voice_filter != NULL)
    {
    S->driver->voice_filter->release_sample_voice(S->driver->voice_filter->driver_state, 
                                                  S);
    }
#endif

  AIL_set_sample_processor( S,
                            SAMPLE_ALL_STAGES,
                            0 );

  hardware_spin_until_stopped( S );
}

//############################################################################
//##                                                                        ##
//## Initialize a SAMPLE structure to baseline values                       ##
//##                                                                        ##
//## Sample is allocated (not free), done playing, and stopped              ##
//##                                                                        ##
//## Beginning with MSS V7, sample source data format must be specified     ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_init_sample( HSAMPLE S, S32 format, U32 flags )
{
  if ( S == NULL )
  {
    return;
  }

  hardware_stop( S );

  //
  // Set status to FREE while manipulating vars, to keep callback thread
  // from reading invalid data
  //

  S->driver->sample_status[S->index] = SMP_FREE;

  //
  // Shut down any previously-active pipeline providers
  //   and init pipeline stages
  //

  AIL_set_sample_processor( S,
                            SAMPLE_ALL_STAGES,
                            0 );

  //
  // Revert to 2D-only control
  //

  S->is_3D = FALSE;
  init_sample_3D_state(S);

  //
  // Initialize sample vars
  //

#ifdef IS_XBOX
  S->cleared_bufs = 0;
  S->cleared_bytes = 0;
  S->need_more_zeroes = 0;
#endif

  S->start[ 0 ]     =  NULL;
  S->len  [ 0 ]     =  0;
  S->pos  [ 0 ]     =  0;
  S->done [ 0 ]     =  0;
  S->reset_ASI[ 0 ] =  0;
  S->reset_seek_pos[0] = 0;

  S->start[ 1 ]     =  NULL;
  S->len  [ 1 ]     =  0;
  S->pos  [ 1 ]     =  0;
  S->done [ 1 ]     =  1;
  S->reset_ASI[ 1 ] =  0;
  S->reset_seek_pos[1] = 0;

  S->exhaust_ASI    =  0;

  clear_last_sample(S);

  S->current_buffer =  0;
  S->last_buffer    = -2;
  S->starved        =  1;

  S->loop_count      =  1;
  S->loop_start      =  0;
  S->loop_end        = -1;
  S->orig_loop_count =  1;
  S->orig_loop_start =  0;
  S->orig_loop_end   = -1;

  #ifndef IS_XBOX
  S->format         =  format;
  S->flags          =  flags;
  S->playback_rate  =  11025;
  #endif

  //
  // Mark as single-buffered sample
  //
  
  S->service_type = 1;

  // 50% of the default volume to each speaker
  AIL_API_set_sample_volume_pan( S, 1.0f, 0.5f );

  S->dry_level     = 1.0F;
  S->wet_level     = 0.0F;
  S->exclusion     = 0.0F;
  S->obstruction   = 0.0F;
  S->occlusion     = 0.0F;

  S->lp.on = 0;
  S->cutoff_param = 1.0F;
  S->calculated_cut = -1.0F;

  // Set default ADPCM data
  S->adpcm.blocksize    = 256;
  S->adpcm.blockleft    = 0;
  S->adpcm.extrasamples = 0;

  S->SOB =  NULL;
  S->EOB =  NULL;
  S->EOS =  NULL;
  S->dosob = 0;
  S->doeob = 0;
  S->doeos = 0;

  //
  // Init pipeline stages
  //

  S32 i;

  for (i = 0 ; i < N_SAMPLE_STAGES ; i++ )
  {
    S->pipeline[ i ].active = 0;
  }

  S->n_active_filters = 0;

  //
  // Select default mixer merge provider
  //

  AIL_set_sample_processor( S,
                            SP_MERGE,
                            S->driver->pipeline[ DP_DEFAULT_MERGE ].provider );

  AIL_set_sample_processor( S,
                            SP_FILTER_0,
                            S->driver->pipeline[ DP_DEFAULT_FILTER ].provider );

  //
  // Channel levels = full scale
  //

  for (i=0; i < S->driver->logical_channels_per_sample; i++)
   {
   S->user_channel_levels   [i] = 1.0F;
   S->auto_3D_channel_levels[i] = 1.0F;
   }

  //
  // Previous-interval volume scalars = 0
  //

  for (i=0; i < MAX_SPEAKERS; i++)
   {
   S->cur_scale[i] = 0;
   }

  //
  // Mark sample initialized
  //

  S->driver->sample_status[S->index] =  SMP_DONE;

  #ifdef IS_XBOX
  S->direct_mode = 1;

  hardware_spin_until_stopped( S );
  hardware_3d_init( S );

  #endif
}

//############################################################################
//##                                                                        ##
//## Get status of sample                                                   ##
//##                                                                        ##
//############################################################################

U32 AILCALL AIL_API_sample_status( HSAMPLE S )
{
  if ( S == NULL )
  {
    return( 0 );
  }

  return( S->driver->sample_status[S->index] );
}

//############################################################################
//##                                                                        ##
//## Set adpcm sample block size                                            ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_sample_adpcm_block_size( HSAMPLE S, U32 blocksize )
{
  if ( S )
  {
    S->adpcm.blocksize = blocksize;
  }
}

//############################################################################
//##                                                                        ##
//## Set starting address and length of sample                              ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_sample_address( HSAMPLE S, void const FAR *start, U32 len )
{
  if ( S == NULL )
  {
    return;
  }

  S->start[ 0 ] = start;
  S->len[ 0 ]   = len;

  S->start[ 1 ] = NULL;
  S->len[ 1 ]   = 0;

  //
  // Mark as single-buffered sample
  //
  
  S->service_type = 1;
}

//############################################################################
//##                                                                        ##
//## Get sample playback rate in hertz                                      ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_API_sample_playback_rate( HSAMPLE S )
{
  if ( S == NULL)
  {
    return 0;
  }

  return( S->playback_rate );
}

//############################################################################
//##                                                                        ##
//## Set sample playback rate in hertz                                      ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_sample_playback_rate( HSAMPLE S, S32 playback_rate )
{
  if ( S )
  {
    if ( playback_rate > 0 )
    {
      S->playback_rate = playback_rate;

      if (S->lp.on)
         {
         AIL_set_sample_low_pass_cut_off(S, S->cutoff_param);
         }

//      set_hardware_rate( S );    // We now set this asynchronously on Xbox so Doppler will work in software
    }
  }
}

//############################################################################
//##                                                                        ##
//## Get sample volume levels                                               ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_sample_volume_levels( HSAMPLE S, F32 * left_level, F32 * right_level )
{
  if (S == NULL)
  {
    return;
  }

  if ( left_level )
  {
    *left_level = S->left_volume;
  }
  if ( right_level )
  {
    *right_level = S->right_volume;
  }
}

//############################################################################
//##                                                                        ##
//## Set sample volume levels                                               ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_sample_volume_levels( HSAMPLE S, F32 left_level, F32 right_level )
{
  if ( S == NULL )
  {
    return;
  }

  S->left_volume = left_level;      // These are already linear scalars
  S->right_volume = right_level;

  // build some reasonable values for reporting
  if ( left_level > 0.0001f )
  {
    F32 ratio = (F32) AIL_pow( right_level / left_level, 10.0f / 3.0f );
    S->save_pan = ratio / ( ratio + 1 );
    if ( S->save_pan < 0.0001f )
      S->save_volume = left_level;
    else
      S->save_volume = right_level * (F32) AIL_pow( S->save_pan, -0.3f );
  }
  else
  {
    if ( right_level > 0.0001f )
    {
      S->save_pan = 1.0f;
      S->save_volume = right_level;
    }
    else
    {
      S->save_pan = 0.5f;
      S->save_volume = 0.0f;
    }
  }

  S->leftb_volume = S->left_volume;
  S->rightb_volume = S->right_volume;
  S->center_volume = S->save_volume;
  S->low_volume = S->save_volume;
  S->save_fb_pan = 0.5f;

  set_hardware_volume( S );
}

//############################################################################
//##                                                                        ##
//## Get sample reverb levels                                               ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_sample_reverb_levels(HSAMPLE S, 
                                          F32    *dry_level, 
                                          F32    *wet_level)
{
   if ( S == NULL )
      {
      if (dry_level)
         {
         *dry_level = 0.0F;
         }

      if (wet_level)
         {
         *wet_level = 0.0F;
         }

      return;
      }

   if (dry_level)
      {
      *dry_level = S->dry_level;
      }

   if (wet_level)
      {
      *wet_level = S->wet_level;
      }
}

//############################################################################
//##                                                                        ##
//## Set sample reverb levels                                               ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_sample_reverb_levels(HSAMPLE S, 
                                              F32     dry_level, 
                                              F32     wet_level)
{
   if ( S == NULL )
      {
      return;
      }

   S->dry_level = dry_level;
   S->wet_level = wet_level;
  
   if (S->driver->sample_status[S->index] == SMP_PLAYING)
      {
      SS_update_sample_reverb_state(S);
      }

   set_hardware_volume(S);
}


//############################################################################
//##                                                                        ##
//## Set sample volume and panpot / stereo balance (0=left, 1=right)        ##
//##                                                                        ##
//## Value determines panning position for mono samples, and balance for    ##
//## stereo samples                                                         ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_sample_volume_pan( HSAMPLE S, F32 volume, F32 pan )
{
  if ( S == NULL )
  {
    return;
  }

  S->save_volume = (F32) AIL_pow( volume, 10.0f / 6.0f ); // turns 0.5F into linear DAC scalar for -10 dB, .25F into -20 dB...
  S->save_pan = pan;

  if ( pan == 0.5f )
  {
    S->left_volume = S->save_volume * 0.812252196f;    
    S->right_volume = S->save_volume * 0.812252196f;
  }
  else
  {
    S->left_volume = S->save_volume * (F32) AIL_pow( 1.0f - pan, 0.3f );
    S->right_volume = S->save_volume * (F32) AIL_pow( pan, 0.3f );
  }

  #if defined(IS_XBOX) || defined(IS_XENON)
    // 0.812252196f is the front/back perfectly balance pan
  S->left_volume = S->left_volume * 0.812252196f;
  S->right_volume = S->right_volume * 0.812252196f;
  #endif

  S->leftb_volume = S->left_volume;
  S->rightb_volume = S->right_volume;
  S->center_volume = S->save_volume;
  S->low_volume = S->save_volume;

  S->save_fb_pan = 0.5f;
  S->save_center = 1.0f;
  S->save_low = 1.0f;

  set_hardware_volume( S );
}

//############################################################################
//##                                                                        ##
//## Get sample volume and panpot / stereo balance (0=left, 1=right)        ##
//##                                                                        ##
//## Value determines panning position for mono samples, and balance for    ##
//## stereo samples                                                         ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_sample_volume_pan( HSAMPLE S, F32 FAR * volume, F32 FAR * pan )
{
  if ( S == NULL )
  {
    return;
  }

  if (volume)
  {
    *volume = (F32) AIL_pow( S->save_volume, 6.0f / 10.0f );    // turns 0.3F (linear scalar for 10 dB down) into 0.5F
  }

  if (pan)
  {
    *pan = S->save_pan;
  }
}

//############################################################################
//##                                                                        ##
//## Set sample channel level scalars                                       ##
//##                                                                        ##
//############################################################################

void         AILCALL AIL_API_set_sample_channel_levels (HSAMPLE  S, //)
                                                        F32 FAR *level_array,
                                                        S32      n_levels)
{
  if ( S == NULL )
  {
    return;
  }

  if (n_levels > S->driver->logical_channels_per_sample)
   {
   n_levels = S->driver->logical_channels_per_sample;
   }

  for (S32 i=0; i < n_levels; i++)
     {
     S->user_channel_levels[i] = level_array[i];
     }
}

//############################################################################
//##                                                                        ##
//## Get sample channel level scalars                                       ##
//##                                                                        ##
//############################################################################

F32 FAR *     AILCALL AIL_API_sample_channel_levels (HSAMPLE  S, //)
                                                     S32 FAR *n_levels)
{
  if ( S == NULL )
  {
    return NULL;
  }

  if (n_levels != NULL)
   {
   *n_levels = S->driver->logical_channels_per_sample;
   }
   
   return S->user_channel_levels;
}

//############################################################################
//##                                                                        ##
//## Set listener-relative source locations used for multispeaker pan       ##
//## calculations                                                           ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_listener_relative_receiver_array (HDIGDRIVER             dig, //)
                                                           MSS_RECEIVER_LIST FAR *array,
                                                           S32                    n_receivers)
{
   if (dig == NULL)
      {
      return;
      }

   if (n_receivers > MAX_RECEIVER_SPECS)
      {
      n_receivers = MAX_RECEIVER_SPECS;
      }

   dig->D3D.n_receiver_specs = n_receivers;

   if (n_receivers)
      {
      AIL_memcpy(dig->D3D.receiver_specifications,
                 array,
                 sizeof(MSS_RECEIVER_LIST) * n_receivers);
      }
}

//############################################################################
//##                                                                        ##
//## Get listener-relative speaker positions (common to all drivers,        ##
//## whether they use the information or not                                ##
//##                                                                        ##
//############################################################################

MSS_RECEIVER_LIST FAR * AILCALL AIL_API_listener_relative_receiver_array (HDIGDRIVER dig, //)
                                                                          S32 FAR   *n_receivers)
{
  if (n_receivers != NULL)
   {
   *n_receivers = dig->D3D.n_receiver_specs;
   }
   
   return dig->D3D.receiver_specifications;
}

#ifndef IS_XBOX

//############################################################################
//##                                                                        ##
//## Set reverb response at each speaker (common to all drivers,            ##
//## whether they use the information or not)                               ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_speaker_reverb_levels (HDIGDRIVER dig, //)
                                                F32 FAR   *wet_array,
                                                F32 FAR   *dry_array,
                                                S32        n_levels)
{
   if (dig == NULL)
      {
      return;
      }

   //
   // Initialize to defaults if no level array provided
   //

   if (wet_array == NULL)
      {
      for (S32 i=0; i < dig->logical_channels_per_sample; i++)
         {
         dig->D3D.speaker_wet_reverb_response[i] = 1.0F;
         }
      }

   if (dry_array == NULL)
      {
      for (S32 i=0; i < dig->logical_channels_per_sample; i++)
         {
         dig->D3D.speaker_dry_reverb_response[i] = 1.0F;
         }
      }

   if (n_levels > dig->logical_channels_per_sample)
      {
      n_levels = dig->logical_channels_per_sample;
      }

   for (S32 i=0; i < n_levels; i++)
      {
      if (wet_array != NULL) dig->D3D.speaker_wet_reverb_response[i] = wet_array[i];
      if (dry_array != NULL) dig->D3D.speaker_dry_reverb_response[i] = dry_array[i];
      }
}

//############################################################################
//##                                                                        ##
//## Set reverb response at each speaker (common to all drivers,            ##
//## whether they use the information or not)                               ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_speaker_reverb_levels (HDIGDRIVER     dig, //)
                                            S32 FAR *      n_levels,
                                            F32 FAR * FAR *wet_array,
                                            F32 FAR * FAR *dry_array)
{
   if (dig == NULL)
      {
      return;
      }

   if (n_levels != NULL)
      {
      *n_levels = dig->logical_channels_per_sample;
      }

   if (wet_array != NULL)
      {
      *wet_array = dig->D3D.speaker_wet_reverb_response;
      }
   
   if (dry_array != NULL)
      {
      *dry_array = dig->D3D.speaker_dry_reverb_response;
      }
}

#endif // !XBox

//############################################################################
//##                                                                        ##
//## Set listener-relative speaker positions (common to all drivers,        ##
//## whether they use the information or not)                               ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_speaker_configuration (HDIGDRIVER       dig, //)
                                                MSSVECTOR3D FAR *speaker_positions,
                                                S32              n_channels,
                                                F32              falloff_power)
{
   if ((dig == NULL) || (dig->logical_channels_per_sample == 0))
      {
      return;
      }

   //
   // Caller does not have to submit locations for all (or any) speakers -- any 
   // speakers beyond n_channels retain their current positions
   //
   // To change the falloff power by itself, pass NULL and 0 for 
   // speaker_positions and n_channels when calling this function
   //

   dig->D3D.falloff_power = falloff_power;

   SS_set_speaker_configuration(&dig->D3D,
                                 speaker_positions,
                                 n_channels,
                                 dig->logical_channels_per_sample);
}

//############################################################################
//##                                                                        ##
//## Get listener-relative speaker positions (common to all drivers,        ##
//## whether they use the information or not                                ##
//##                                                                        ##
//############################################################################

MSSVECTOR3D FAR * AILCALL AIL_API_speaker_configuration (HDIGDRIVER         dig, //)
                                                         S32 FAR           *n_physical_channels,
                                                         S32 FAR           *n_logical_channels,
                                                         F32 FAR           *falloff_power,
                                                         MSS_MC_SPEC FAR   *channel_spec)

{
   if ( dig == NULL )
      {
      return NULL;
      }

   if (n_physical_channels != NULL)
      {
      *n_physical_channels = dig->physical_channels_per_sample;
      }
                          
   if (n_logical_channels != NULL)
      {
      *n_logical_channels = dig->logical_channels_per_sample;
      }

   if (falloff_power != NULL)
      {
      *falloff_power = dig->D3D.falloff_power;
      }

   if (channel_spec != NULL)
      {
      *channel_spec = dig->channel_spec;
      }
   
   return dig->D3D.speaker_positions;
}

//############################################################################
//##                                                                        ##
//## Set sample 5.1 volume and panpot / stereo balance (0=left, 1=right)    ##
//##                                                                        ##
//## Value determines panning position for mono samples, and balance for    ##
//## stereo samples                                                         ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_sample_51_volume_pan( HSAMPLE S, F32 volume, F32 pan, F32 fb_pan, F32 center_level, F32 sub_level )
{
  if ( S == NULL )
  {
    return;
  }

#ifndef IS_XBOX
  if ((S->driver->channel_spec != MSS_MC_51_DISCRETE) && 
      (S->driver->channel_spec != MSS_MC_51_DTS))
     {
     return;
     }
#endif

  F32 left, right, front, back;

  S->save_volume = (F32) AIL_pow( volume, 10.0f / 6.0f );
  S->save_pan = pan;
  S->save_fb_pan = fb_pan;
  S->save_center = center_level;
  S->save_low = sub_level;

  if ( fb_pan == 0.5f )
  {
    front = 0.812252196f;
    back = 0.812252196f;
  }
  else
  {
    front = (F32) AIL_pow( 1.0f - fb_pan, 0.3f );
    back = (F32) AIL_pow( fb_pan, 0.3f );
  }

  if ( pan != fb_pan )
  {
    if ( pan == 0.5f )
    {
      left = S->save_volume * 0.812252196f;
      right = S->save_volume * 0.812252196f;
    }
    else
    {
      left = S->save_volume * (F32) AIL_pow( 1.0f - pan, 0.3f );
      right = S->save_volume * (F32) AIL_pow( pan, 0.3f );
    }
  }
  else
  {
    left = S->save_volume * front;
    right = S->save_volume * back;
  }

  S->left_volume = left * front;
  S->right_volume = right * front;

  S->leftb_volume = left * back;
  S->rightb_volume = right * back;

  S->center_volume = S->save_volume * center_level;
  S->low_volume = S->save_volume * sub_level;

  set_hardware_volume( S );
}


//############################################################################
//##                                                                        ##
//## Get sample 5.1 volume and panpot / stereo balance (0=left, 1=right)    ##
//##                                                                        ##
//## Value determines panning position for mono samples, and balance for    ##
//## stereo samples                                                         ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_sample_51_volume_pan( HSAMPLE S, F32 FAR * volume, F32 FAR * lr_pan, F32 FAR * fb_pan, F32 FAR * center_level, F32 FAR * sub_level )
{
  if ( S == NULL )
  {
    return;
  }

#ifndef IS_XBOX
  if ((S->driver->channel_spec != MSS_MC_51_DISCRETE) && 
      (S->driver->channel_spec != MSS_MC_51_DTS))
     {
     return;
     }
#endif

  if ( volume )
  {
    *volume = (F32) AIL_pow( S->save_volume, 6.0f / 10.0f );
  }

  if ( lr_pan )
  {
    *lr_pan = S->save_pan;
  }

  if ( fb_pan )
  {
    *fb_pan = S->save_fb_pan;
  }

  if ( center_level )
  {
    *center_level = S->save_center;
  }

  if ( sub_level )
  {
    *sub_level = S->save_low;
  }
}

//############################################################################
//##                                                                        ##
//## Set sample volume levels                                               ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_sample_51_volume_levels( HSAMPLE S, F32 f_left_level, F32 f_right_level, F32 b_left_level, F32 b_right_level, F32 center_level, F32 sub_level )
{
  if ( S == NULL )
  {
    return;
  }

#ifndef IS_XBOX
  if ((S->driver->channel_spec != MSS_MC_51_DISCRETE) && 
      (S->driver->channel_spec != MSS_MC_51_DTS))
     {
     return;
     }
#endif

  S->left_volume = f_left_level;
  S->right_volume = f_right_level;

  S->leftb_volume = b_left_level;
  S->rightb_volume = b_right_level;
  S->center_volume = center_level;
  S->low_volume = sub_level;

  // build some reasonable values for reporting
  if ( f_left_level > 0.0001f )
  {
    F32 ratio = (F32) AIL_pow( f_right_level / f_left_level, 10.0f / 3.0f );
    S->save_pan = ratio / ( ratio + 1 );

    ratio = (F32) AIL_pow( b_left_level / f_left_level, 10.0f / 3.0f );
    S->save_fb_pan = ratio / ( ratio + 1 );

    S->save_volume = ( S->save_pan > 0.0001f ) ?
                       ( ( S->save_fb_pan > 0.0001f ) ?
                         ( b_right_level * (F32) AIL_pow( S->save_pan * S->save_fb_pan, -0.3f ) ) :
                         ( f_right_level * (F32) AIL_pow( S->save_pan, -0.3f ) )
                        ) :
                        ( ( S->save_fb_pan > 0.0001f ) ?
                          ( b_left_level * (F32) AIL_pow( S->save_fb_pan, -0.3f ) ) :
                          f_left_level
                        );
  }
  else
  {
    if ( b_left_level > 0.0001f )
    {
      F32 ratio = (F32) AIL_pow( b_right_level / b_left_level, 10.0f / 3.0f );
      S->save_pan = ratio / ( ratio + 1 );

      ratio = (F32) AIL_pow( f_left_level / b_left_level, 10.0f / 3.0f );
      S->save_fb_pan = 1.0f - ( ratio / ( ratio + 1 ) );

      S->save_volume = ( S->save_pan > 0.0001f ) ?
                         ( ( S->save_fb_pan > 0.0001f ) ?
                           ( b_right_level * (F32) AIL_pow( S->save_pan * S->save_fb_pan, -0.3f ) ) :
                           ( f_right_level * (F32) AIL_pow( S->save_pan, -0.3f ) )
                          ) :
                          ( ( S->save_fb_pan > 0.0001f ) ?
                            ( b_left_level * (F32) AIL_pow( S->save_fb_pan, -0.3f ) ) :
                            f_left_level
                          );
    }
    else
    {
      if ( f_right_level > 0.0001f )
      {
        F32 ratio = (F32) AIL_pow( b_right_level / f_right_level, 10.0f / 3.0f );
        S->save_fb_pan = ratio / ( ratio + 1 );
        S->save_pan = 1.0f;

        S->save_volume = ( S->save_fb_pan > 0.0001f ) ?
                         ( b_right_level * (F32) AIL_pow( S->save_fb_pan, -0.3f ) ) :
                         f_right_level;
      }
      else
      {
        if ( b_right_level > 0.0001f )
        {
          S->save_pan = 1.0f;
          S->save_fb_pan = 1.0f;
          S->save_volume = b_right_level;
        }
        else
        {
          S->save_pan = 0.5f;
          S->save_fb_pan = 0.5f;
          S->save_volume = 0;
        }
      }
    }
  }

  if ( S->save_volume > 0.0001f )
  {
    S->save_center = center_level / S->save_volume;
    S->save_low = sub_level / S->save_volume;
  }
  else
  {
    S->save_center = 0.0f;
    S->save_low = 0.0f;
  }

  set_hardware_volume( S );
}

//############################################################################
//##                                                                        ##
//## Get sample 5.1 volume and panpot / stereo balance (0=left, 1=right)    ##
//##                                                                        ##
//## Value determines panning position for mono samples, and balance for    ##
//## stereo samples                                                         ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_sample_51_volume_levels( HSAMPLE S, F32 FAR * f_left_level, F32 FAR * f_right_level, F32 FAR * b_left_level, F32 FAR * b_right_level, F32 FAR * center_level, F32 FAR * sub_level )
{
  if ( S == NULL )
  {
    return;
  }

#ifndef IS_XBOX
  if ((S->driver->channel_spec != MSS_MC_51_DISCRETE) && 
      (S->driver->channel_spec != MSS_MC_51_DTS))
     {
     return;
     }
#endif

  if ( f_left_level )
  {
    *f_left_level = S->left_volume;
  }

  if ( f_right_level )
  {
    *f_right_level = S->right_volume;
  }

  if ( b_left_level )
  {
    *b_left_level = S->leftb_volume;
  }

  if ( b_right_level )
  {
    *b_right_level = S->rightb_volume;
  }

  if ( center_level )
  {
    *center_level = S->center_volume;
  }

  if ( sub_level )
  {
    *sub_level = S->low_volume;
  }
}

//############################################################################
//##                                                                        ##
//## Return # of bytes per sample                                           ##
//##                                                                        ##
//############################################################################

U32 SS_granularity( U32 format )
{
  switch ( format )
  {
    case DIG_F_MONO_16:
    case DIG_F_STEREO_8:
      return( 2 );

    case DIG_F_STEREO_16:
      return( 4 );

    default:
      return( 1 );
  }
}

//############################################################################
//##                                                                        ##
//## Get the granularity of sample type (1=8/m,2=8/s,16/m,4=16/m, or adpcm) ##
//##                                                                        ##
//## Value is returned in bytes                                             ##
//##                                                                        ##
//############################################################################

U32 AILCALL AIL_API_sample_granularity( HSAMPLE S )
{
  if ( S == NULL )
  {
    return( 0 );
  }

  if ( S->pipeline[ SP_ASI_DECODER ].active )
  {
    //
    // All ASI compressed formats are assumed to have 1-byte granularity --
    // the ASI layer will handle any granularity issues associated with
    // seeks, etc. internally
    //

    return( 1 );
  }

#ifdef IS_XBOX
  if ( S->format & DIG_F_XBOX_ADPCM_MASK )
  {
    return( 36 * ( ( S->format & DIG_F_STEREO_MASK ) ? 2 : 1 ) );
  }
#endif

  if ( S->format & DIG_F_ADPCM_MASK )
  {
    return( S->adpcm.blocksize );
  }

  return( SS_granularity( S->format ) );
}

//############################################################################
//##                                                                        ##
//## Get position of next sample block to be transferred                    ##
//##                                                                        ##
//## Value is returned in bytes relative to start of sample, and may be     ##
//## treated as the sample's approximate playback position                  ##
//##                                                                        ##
//############################################################################

U32 AILCALL AIL_API_sample_position( HSAMPLE S )
{
  if ( S == NULL )
  {
    return( 0 );
  }

  check_hardware_buffer( S, 1 );

  return( S->pos[ S->current_buffer ] );
}

//############################################################################
//##                                                                        ##
//## Set position of next sample block to be transferred                    ##
//##                                                                        ##
//## Value is specified in bytes relative to start of sample, and will      ##
//## determine the precise location of the next block of data which is      ##
//## fetched from the sample                                                ##
//##                                                                        ##
//## AIL_set_sample_position(0) "rewinds" the sample to its beginning       ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_sample_position( HSAMPLE S, U32 pos )
{
  U32 gran;

  if ( S == NULL )
  {
    return;
  }

  if (pos > S->len[S->current_buffer]) pos = S->len[S->current_buffer];

  gran = AIL_sample_granularity( S );
  #ifdef IS_XBOX
  gran = ( gran + 3 ) & ~3;
  #endif

  pos = ( ( pos + ( gran / 2 ) ) / gran ) * gran;

  // Reset ADPCM offset to the end of decode buffer
  // to force a decode buffer refill

  if ( S->format & DIG_F_ADPCM_MASK )
  {
    S->adpcm.blockleft = 0;
    S->adpcm.extrasamples = 0;
  }

  //
  // If this is an ASI-compressed format, perform a seek with offset -1
  // to allow the decoder to reinitialize its state
  //
  // We could also implement this by seeking to the specified offset, but
  // we need to change the position here anyway so it will be reported
  // correctly in subsequent AIL_set_sample_position() calls.  Most ASI
  // decoders don't care about the seek position -- they only need
  // to be informed when we change it
  //

  if (S->pipeline[ SP_ASI_DECODER ].active)
  {
    ASISTAGE *ASI = &S->pipeline[ SP_ASI_DECODER ].TYPE.ASI;
    ASI->ASI_stream_seek( ASI->stream, -1 );

    if ((S->service_type == 1) && (ASI->STREAM_SEEK_POS != (UINTa) -1))
      {
      ASI->ASI_stream_property(ASI->stream,
                               ASI->STREAM_SEEK_POS,
                               0,&pos,&pos);
      }
  }

  if ( S->driver->sample_status[S->index] == SMP_PLAYING )
  {
    check_hardware_buffer( S, 0 );
  }

  if ( pos < S->len[S->current_buffer] )
  {
  S->pos[ S->current_buffer ] = pos;
  }

  set_hardware_position( S );
}

//############################################################################
//##                                                                        ##
//## Get number of sample loops remaining                                   ##
//##                                                                        ##
//## 1 indicates that the sample is on its last iteration                   ##
//## 0 indicates that the sample is looping indefinitely                    ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_API_sample_loop_count( HSAMPLE S )
{
  if ( S == NULL )
  {
    return -1;
  }

  return( S->loop_count );
}

//############################################################################
//##                                                                        ##
//## Set sample loop count                                                  ##
//##                                                                        ##
//##  1: Single iteration, no looping                                       ##
//##  0: Loop indefinitely                                                  ##
//##  n: Play sample n times                                                ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_sample_loop_count(HSAMPLE S, S32 loop_count)
{
  if (S == NULL)
  {
    return;
  }

  S->loop_count      = loop_count;
  S->orig_loop_count = loop_count;
}

//############################################################################
//##                                                                        ##
//## Set optional subblock for looping                                      ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_sample_loop_block( HSAMPLE S, //)
                                            S32     loop_start_offset,
                                            S32     loop_end_offset )
{
  U32 gran;

  if ( S == NULL )
  {
    return;
  }

  S->orig_loop_start = loop_start_offset;
  S->orig_loop_end   = loop_end_offset;

  gran = AIL_sample_granularity( S );
  #ifdef IS_XBOX
  gran = ( gran + 3 ) & ~3;
  #endif

  S->loop_start = ( ( loop_start_offset + ( gran / 2 ) ) / gran ) * gran;

  if ( loop_end_offset == -1 )
    S->loop_end = -1;
  else
    S->loop_end = ( ( loop_end_offset + ( gran / 2 ) ) / gran ) * gran;

  set_hardware_loop_region( S );
}

//############################################################################
//##                                                                        ##
//## Return optional subblock boundaries                                    ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_API_sample_loop_block( HSAMPLE S, //)
                                       S32    *loop_start_offset,
                                       S32    *loop_end_offset )
{
  if ( S == NULL )
  {
    return 0;
  }

  if (loop_start_offset) *loop_start_offset = S->orig_loop_start;
  if (loop_end_offset)   *loop_end_offset   = S->orig_loop_end;

  return S->orig_loop_count;
}

//############################################################################
//##                                                                        ##
//## Set master volume for all samples                                      ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_digital_master_volume_level( HDIGDRIVER dig, F32 master_volume )
{
  if ( dig == 0 )
    return;

  dig->master_volume = master_volume;

  set_master_hardware_volume( dig );
}

//############################################################################
//##                                                                        ##
//## Return current master digital volume setting                           ##
//##                                                                        ##
//############################################################################

F32 AILCALL AIL_API_digital_master_volume_level(HDIGDRIVER dig)
{
  if ( dig == 0 )
    return( 0.0f );
    
  return( dig->master_volume );
}


//############################################################################
//##                                                                        ##
//## Get driver reverb levels                                               ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_digital_master_reverb_levels( HDIGDRIVER dig, F32 FAR* dry_level, F32 FAR * wet_level )
{
  if (dig == NULL)
  {
    return;
  }

  if ( dry_level )
  {
    *dry_level = dig->master_dry;
  }
  if ( wet_level )
  {
    *wet_level = dig->master_wet;
  }
}

//############################################################################
//##                                                                        ##
//## Set driver reverb levels                                               ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_digital_master_reverb_levels( HDIGDRIVER dig, F32 dry_level, F32 wet_level )
{
  if (dig == NULL)
  {
    return;
  }

  dig->master_dry = dry_level;
  dig->master_wet = wet_level;

#ifndef IS_XBOX

  SS_update_driver_reverb_state(dig);

#endif

  set_master_hardware_volume( dig );
}

#ifndef IS_XBOX

#define REVERB_DELAYS 6

static const F32 ReverbDelayTimes[ REVERB_DELAYS ] = { 0.0297F, 0.0371F, 0.0411F, 0.0437F, 0.005F, 0.0017F };
static const F32 ReverbAllPass[ 2 ] = { 0.0968535f, 0.032924f };

S32 AILCALL AIL_allocate_reverb_buffers( HDIGDRIVER dig )
{
  U32 mem;
  U32 del_sizes[ REVERB_DELAYS ];
  U32 del_sizes_pad[ REVERB_DELAYS ];
  U32 i;
  F32 rate = (F32) dig->DMA_rate;

  if (dig->hw_format & DIG_F_OUTPUT_FILTER_IN_USE )
   {
   // Configurations with no build buffers (e.g., those with voice filters) don't do reverb
   return 1;
   }

  // calculate the size of the reverb and predelay buffer sizes
  // This is always a mono buffer, so we'll begin with build_size / (mono or stereo build-buffer channel count)

  S32 reverb_channels_per_sample = (dig->logical_channels_per_sample == 1) ? 1 : 2;

  dig->reverb_fragment_size = ( 2 * dig->build_size / reverb_channels_per_sample ); //2* because reverb uses S32 samples

  i = (U32) (S32) ( ( ( ( (F32) AIL_preference[DIG_MAX_PREDELAY_MS] ) / 1000.0f ) * rate ) * 4.0f );

  i = ( ( i + dig->reverb_fragment_size - 1 ) / dig->reverb_fragment_size );

  dig->reverb_buffer_size = i * dig->reverb_fragment_size;

  mem = ( dig->reverb_buffer_size + 15 ) & ~15;

  // calculate the sizes for the delay lines

  for( i = 0 ; i < REVERB_DELAYS ; i++ )
  {
    del_sizes[ i ] = ( (U32) (S32) ( ( rate * ReverbDelayTimes[ i ] ) /*+ 0.9999f*/ ) ) * 4;
    del_sizes_pad[ i ] = ( del_sizes[ i ] + 15 ) & ~15;
    mem += del_sizes_pad[ i ];
  }

  // now allocate the entire buffer
  dig->reverb_total_size = mem;
  dig->reverb_build_buffer = (S32 FAR *) AIL_mem_alloc_lock( mem );

  if ( dig->reverb_build_buffer == 0 )
  {
    return( 0 );
  }

  // clear everything to zeros
  AIL_memset( dig->reverb_build_buffer, 0, mem );

  // set the two reverb offsets
  dig->reverb_head = 0;
  dig->reverb_tail = 0;

  // set all of the delay line pointers
  dig->ri.c.start0 = (F32 FAR*) ( ( (U8 FAR*) dig->reverb_build_buffer ) + ( ( dig->reverb_buffer_size + 15 ) & ~15 ) );
  dig->ri.c.start1 = (F32 FAR*) ( ( (U8 FAR*) dig->ri.c.start0 ) + del_sizes_pad[ 0 ] );
  dig->ri.c.start2 = (F32 FAR*) ( ( (U8 FAR*) dig->ri.c.start1 ) + del_sizes_pad[ 1 ] );
  dig->ri.c.start3 = (F32 FAR*) ( ( (U8 FAR*) dig->ri.c.start2 ) + del_sizes_pad[ 2 ] );
  dig->ri.c.start4 = (F32 FAR*) ( ( (U8 FAR*) dig->ri.c.start3 ) + del_sizes_pad[ 3 ] );
  dig->ri.c.start5 = (F32 FAR*) ( ( (U8 FAR*) dig->ri.c.start4 ) + del_sizes_pad[ 4 ] );

  dig->ri.c.end0 = (F32 FAR*) ( ( (U8 FAR*) dig->ri.c.start0 ) + del_sizes[ 0 ] );
  dig->ri.c.end1 = (F32 FAR*) ( ( (U8 FAR*) dig->ri.c.start1 ) + del_sizes[ 1 ] );
  dig->ri.c.end2 = (F32 FAR*) ( ( (U8 FAR*) dig->ri.c.start2 ) + del_sizes[ 2 ] );
  dig->ri.c.end3 = (F32 FAR*) ( ( (U8 FAR*) dig->ri.c.start3 ) + del_sizes[ 3 ] );
  dig->ri.c.end4 = (F32 FAR*) ( ( (U8 FAR*) dig->ri.c.start4 ) + del_sizes[ 4 ] );
  dig->ri.c.end5 = (F32 FAR*) ( ( (U8 FAR*) dig->ri.c.start5 ) + del_sizes[ 5 ] );

  dig->ri.u.address0 = dig->ri.c.start0;
  dig->ri.u.address1 = dig->ri.c.start1;
  dig->ri.u.address2 = dig->ri.c.start2;
  dig->ri.u.address3 = dig->ri.c.start3;
  dig->ri.u.address4 = dig->ri.c.start4;
  dig->ri.u.address5 = dig->ri.c.start5;

  dig->ri.u.X0 = 0;
  dig->ri.u.X1 = 0;
  dig->ri.u.Y0 = 0;
  dig->ri.u.Y1 = 0;

  return( 1 );
}


static void calc_constants( REVERB_INFO FAR * ri, F32 time, F32 damping )
{
  F32 C;

  ri->c.C0 = (F32) AIL_pow( 0.001f, ReverbDelayTimes[ 0 ] /  time );
  ri->c.C1 = (F32) AIL_pow( 0.001f, ReverbDelayTimes[ 1 ] /  time );
  ri->c.C2 = (F32) AIL_pow( 0.001f, ReverbDelayTimes[ 2 ] /  time );
  ri->c.C3 = (F32) AIL_pow( 0.001f, ReverbDelayTimes[ 3 ] /  time );
  ri->c.C4 = (F32) AIL_pow( 0.001f, ReverbDelayTimes[ 4 ] /  ReverbAllPass[ 0 ] );
  ri->c.C5 = (F32) AIL_pow( 0.001f, ReverbDelayTimes[ 5 ] /  ReverbAllPass[ 1 ] );

  // calculate new coeffs
  if ( damping < 0.0001f )
  {
    ri->c.A = 0.0f;
    ri->c.B0 = 0.0f;
    ri->c.B1 = 0.0f;
  }
  else
  {
    C =  1.0f / (F32) AIL_tan( MY_PI * damping * 0.125f );

    // calculate coefficients
    ri->c.A =  1.0f / ( 1.0f + AIL_fsqrt( 2.0f ) * C + C * C );

    // calculate coefficients
    ri->c.B0 = 2.0f * ( 1.0f - C * C ) * ri->c.A;
    ri->c.B1 = ( 1.0f - AIL_fsqrt( 2.0F ) * C + C * C ) * ri->c.A;
  }

  check_denorm( &ri->c.C0 );
  check_denorm( &ri->c.C1 );
  check_denorm( &ri->c.C2 );
  check_denorm( &ri->c.C3 );
  check_denorm( &ri->c.C4 );
  check_denorm( &ri->c.C5 );
  check_denorm( &ri->c.A );
  check_denorm( &ri->c.B0 );
  check_denorm( &ri->c.B1 );
}

#ifdef NOQIFIST
static __inline S32 f2i(F32 val)
{
  S32 retval;
  __asm
  {
    fld [val]
    fistp [retval]
  }
  return(retval);
}
#else
#define f2i(val) ((S32)(val))
#endif

static void do_reverb(HDIGDRIVER dig)
{
   //
   // Advance head (input) pointer
   // 

   dig->reverb_head += dig->reverb_fragment_size;

   if (dig->reverb_head >= dig->reverb_buffer_size)
      {
      dig->reverb_head = 0;
      }

   //
   // Get region to process, and advance tail (output) pointer
   //

   U32 bytes   = 0;
   U32 remnant = 0;

   void FAR *src = ( (U8*) dig->reverb_build_buffer ) + dig->reverb_tail;

   S32 amount_left = dig->reverb_buffer_size - dig->reverb_tail;

   if (dig->reverb_fragment_size >= amount_left)
      {
      //
      // Do last part of source buffer followed by first part
      //

      bytes            = amount_left;
      remnant          = dig->reverb_fragment_size - amount_left;
      dig->reverb_tail = remnant;
      }
   else
      {
      //
      // Do the entire source buffer
      // 

      bytes             = dig->reverb_fragment_size;
      remnant           = 0;
      dig->reverb_tail += dig->reverb_fragment_size;
      }

   //
   // Up to 2 iterations may be necessary to handle buffer wrap
   //

   S32 d = 0;

   while (bytes > 0)
      {
      U32 num = bytes / sizeof(S32);
      S32 FAR *s = (S32 FAR *) src;
    
      REVERB_INFO ri;
      AIL_memcpy(&ri, &dig->ri, sizeof(ri));
    
      while (num--)
         {
         F32 temp, U1, U2;
         F32 comb_sum;
         F32 samp, lp_samp;
        
         samp = ( (F32) *s++ ) * ri.c.A;
         check_denorm( &samp );
        
         // calculate sample
         lp_samp = samp + ri.u.X0 + ri.u.X0 + ri.u.X1
                   - ri.c.B0 * ri.u.Y0
                   - ri.c.B1 * ri.u.Y1 + ( 1.192092896e-06F * 2.0f );
        
         check_denorm( &lp_samp );
        
         // save sample history
         ri.u.X1 = ri.u.X0;
         ri.u.X0 = samp;
         ri.u.Y1 = ri.u.Y0;
         ri.u.Y0 = lp_samp;
        
         // process sum of comb filters
         comb_sum = *ri.u.address0 + \
                    *ri.u.address1 + \
                    *ri.u.address2 + \
                    *ri.u.address3;
        
         check_denorm( &comb_sum );
        
         // process coef for each comb filter
        
         *ri.u.address0 = ri.c.C0 * ( *ri.u.address0 ) + lp_samp;
         *ri.u.address1 = ri.c.C1 * ( *ri.u.address1 ) + lp_samp;
         *ri.u.address2 = ri.c.C2 * ( *ri.u.address2 ) + lp_samp;
         *ri.u.address3 = ri.c.C3 * ( *ri.u.address3 ) + lp_samp;
        
         check_denorm( ri.u.address0 );
         check_denorm( ri.u.address1 );
         check_denorm( ri.u.address2 );
         check_denorm( ri.u.address3 );
        
         // advance inputs
         ++ri.u.address0;
         ++ri.u.address1;
         ++ri.u.address2;
         ++ri.u.address3;
        
         // process clip for buffer wrap
         if ( ri.u.address0 >= ri.c.end0 ) ri.u.address0 = ri.c.start0;
         if ( ri.u.address1 >= ri.c.end1 ) ri.u.address1 = ri.c.start1;
         if ( ri.u.address2 >= ri.c.end2 ) ri.u.address2 = ri.c.start2;
         if ( ri.u.address3 >= ri.c.end3 ) ri.u.address3 = ri.c.start3;
        
         // get allpass value
         U1 = *ri.u.address4;
         check_denorm(ri.u.address4);
         check_denorm(ri.u.address5);
        
         // all pass
         temp = ri.c.C4 * U1 + comb_sum;
        
         *ri.u.address4 = temp;
         ++ri.u.address4;
        
         check_denorm(ri.u.address4);
        
         // adjust
         U1 -= ri.c.C4 * temp;
         U2 = *ri.u.address5;
        
         // all pass
         temp = ri.c.C5 * U2 + ( U1 * 0.25f );
         *ri.u.address5 = temp;
         ++ri.u.address5;
        
         check_denorm(ri.u.address5);
        
         // compute left
         F32 left;
        
         left = U2 - ri.c.C5 * temp;
         check_denorm(&left);
        
         // process clip for buffer wrap
         if ( ri.u.address4 >= ri.c.end4 )
           ri.u.address4 = ri.c.start4;
        
         if ( ri.u.address5 >= ri.c.end5 )
           ri.u.address5 = ri.c.start5;
        
         //
         // Add new reverb output sample to all build buffers
         //

         for (S32 i=0; i < dig->n_build_buffers; i++)
            {
            MSS_BB *B = &dig->build[i];

            if (B->chans == 2)
               {
               F32 right = U2 - ri.c.C5 * ( ri.c.C5 * U2 + ( U1 * 0.15f ) );
               check_denorm(&right);

               B->buffer[(d * 2)    ] += f2i(left  * dig->D3D.speaker_wet_reverb_response[B->speaker_offset    ]);
               B->buffer[(d * 2) + 1] += f2i(right * dig->D3D.speaker_wet_reverb_response[B->speaker_offset + 1]);
               }
            else
               {
               B->buffer[d] += f2i(left * dig->D3D.speaker_wet_reverb_response[B->speaker_offset]);
               }
            }

         //
         // Advance to next destination sample
         //

         ++d;
         } 
        
      dig->ri.u = ri.u;

      bytes   = remnant;
      remnant = 0;

      if (bytes > 0)
         {
         src = dig->reverb_build_buffer;
         }
      }
}

//############################################################################
//##                                                                        ##
//## Set driver reverb settings                                             ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_digital_master_reverb(HDIGDRIVER dig, //)
                                               F32        reverb_decay_time, 
                                               F32        reverb_predelay, 
                                               F32        reverb_damping)
{
   if (dig == NULL)
      {
      return;
      }
 
   dig->reverb_decay_time_s = reverb_decay_time;
   dig->reverb_predelay_s   = reverb_predelay;
   dig->reverb_damping      = reverb_damping;
 
   //
   // Calculate software reverb filter parameters if applicable
   //

#ifdef MSS_VFLT_SUPPORTED
   if (dig->voice_filter == NULL)
#endif
      {
      dig->reverb_duration_ms = (S32) ((reverb_decay_time + reverb_predelay) * 1000.0F);

      SS_update_driver_reverb_state(dig);

      //
      // Tail (output) pointer is <predelay> behind head (incoming) pointer
      //

      S32 bytes = ((S32) (reverb_predelay * (F32) dig->DMA_rate)) * sizeof(S32);

      dig->reverb_tail = dig->reverb_head - bytes;

      if (dig->reverb_tail < 0)
         {
         dig->reverb_tail += dig->reverb_buffer_size;
         }

      calc_constants( &dig->ri, dig->reverb_decay_time_s, dig->reverb_damping );
      }
}

//############################################################################
//##                                                                        ##
//## Get driver reverb settings                                             ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_digital_master_reverb( HDIGDRIVER dig, F32 FAR * reverb_time, F32 FAR* reverb_predelay, F32 FAR* reverb_damping )
{
  if ( dig == NULL )
  {
    return;
  }

  if ( reverb_time )
  {
    *reverb_time = dig->reverb_decay_time_s;
  }
  if ( reverb_predelay )
  {
    *reverb_predelay = dig->reverb_predelay_s;
  }
  if ( reverb_damping )                      
  {
    *reverb_damping = dig->reverb_damping;
  }
}

#endif

//############################################################################
//##                                                                        ##
//## Lowpass helper functions                                               ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_sample_low_pass_cut_off( HSAMPLE S,
                                                  F32     cutoff )
{
  if ( S )
  {
  S->cutoff_param = cutoff;

    if ( ( cutoff < 0.0f ) || ( cutoff >= 0.999f ) )
    {
      S->lp.on = 0;
      S->lp.cutoff = 1.0f;
      S->calculated_cut = -1.0F;
    }
    else
    {
#ifndef IS_XBOX
      F32 A;

      F32 cut = (cutoff * ((F32) S->playback_rate)) / ((F32) S->driver->DMA_rate);
      if (cut > 0.999F) cut = 0.999F;

      if (cut != S->calculated_cut)
         {
         S->calculated_cut = cut;

         if ( cut < 0.0001f )
            {
            A = 0;
            S->lp.c.A =  0;
            S->lp.c.B0 = 0;
            S->lp.c.B1 = 0;
            }
         else
            {
            // calculate new coeffs
            F32 C =  1.0F / (F32) AIL_tan( MY_PI * cut * 0.5f );
           
            // calculate coefficients
            A = 4096.0F / ( 1.0F + ( AIL_fsqrt( 2.0F ) * C ) + C * C );
            S->lp.c.A =  (S32) A;
           
            // calculate coefficients
            S->lp.c.B0 =  (S32)( 2.0F * ( 1.0F - C * C ) * A );
            S->lp.c.B1 =  (S32)( ( 1.0F - ( AIL_fsqrt( 2.0F ) * C ) + C * C ) * A );
            }

         if ( S->lp.on == 0 )
            {
            S->lp.u.XL0 = 0;
            S->lp.u.XL1 = 0;
            S->lp.u.YL0 = 0;
            S->lp.u.YL1 = 0;
            S->lp.u.XR0 = 0;
            S->lp.u.XR1 = 0;
            S->lp.u.YR0 = 0;
            S->lp.u.YR1 = 0;
            }
         else
            {
            S->lp.u.XL0 = (S32) ( ( (F32) S->lp.u.YL0 ) * A );
            S->lp.u.XL1 = (S32) ( ( (F32) S->lp.u.YL1 ) * A );
            S->lp.u.XR0 = (S32) ( ( (F32) S->lp.u.YR0 ) * A );
            S->lp.u.XR1 = (S32) ( ( (F32) S->lp.u.YR1 ) * A );
            }
         }

#endif     
      S->lp.cutoff = cutoff;
      S->lp.on = 1;
    }

    set_hardware_low_pass( S );
  }
}

F32 AILCALL AIL_API_sample_low_pass_cut_off(HSAMPLE S)
{
  if ( ( S ) && ( S->lp.on ) )
  {
    return( S->lp.cutoff );
  }
  else
  {
    return( 1.0f );
  }
}

//############################################################################
//##                                                                        ##
//## Start playback of sample from beginning                                ##
//##                                                                        ##
//## At a minimum, sample must first have been initialized with prior calls ##
//## to AIL_init_sample() and AIL_set_sample_address() or                   ##
//## AIL_load_sample_buffer()                                               ##
//##                                                                        ##
//## Playback will begin at the next DMA half-buffer transition             ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_start_sample( HSAMPLE S )
{
  if ( S == NULL )
  {
    return;
  }

  //
  // Make sure sample has been allocated
  //

  if ( S->driver->sample_status[S->index] == SMP_FREE )
  {
    return;
  }

  //
  // If this is a double-buffered sample that's already playing, just return without
  // rewinding it
  //

  if (((S->driver->sample_status[S->index] == SMP_PLAYING) || (S->driver->sample_status[S->index] == SMP_PLAYINGBUTRELEASED))
       &&
      (S->service_type == 2))
   {
   return;
   }

  //
  // Make sure valid sample data exists
  //

  if ( ( S->len  [ S->current_buffer ] == 0 ) ||
       ( S->start[ S->current_buffer ] == NULL ) )
  {
    return;
  }

  // Clear residuals
  clear_last_sample(S);

  // Reset ADPCM data
  S->adpcm.blockleft = 0;
  S->adpcm.extrasamples = 0;

  //
  // Rewind sample to beginning
  //

  S->pos[ S->current_buffer ] = 0;

  // Flush and reset ASI decoder, if any
  if (S->pipeline[ SP_ASI_DECODER ].active)
  {
    ASISTAGE *ASI = &S->pipeline[ SP_ASI_DECODER ].TYPE.ASI;

    ASI->ASI_stream_seek( ASI->stream, -1 );

    if ((S->service_type == 1) && (ASI->STREAM_SEEK_POS != (UINTa) -1))
      {
      ASI->ASI_stream_property(ASI->stream,
                               ASI->STREAM_SEEK_POS,
                               0,&S->pos[ S->current_buffer ], &S->pos[ S->current_buffer ]);
      }
  }

  //
  // Mark as single-buffered sample
  //
  // (Disabled as of V7.0 -- AIL_start_sample() is not only compatible with double-buffering now, but
  // required)
  //

#if 0
  S->service_type = 1;
#endif

  #if !defined(IS_DOS) && !defined(IS_XBOX) && !defined(IS_XENON) && !defined(IS_PS2) && !defined(IS_PS3) && !defined(IS_WII)
  if ( S->driver->released )
  {
     S->driver->sample_status[S->index] = SMP_PLAYINGBUTRELEASED;
  }
  else
  #endif
  {
    //
    // start reverb if we need to
    //

    SS_update_sample_reverb_state(S);

    //
    // Set 'playing' status
    //

    S->driver->sample_status[S->index] = SMP_PLAYING;

    //
    // If sample's driver is not already transmitting data, start it
    //

    SS_start_playback( S );
  }
}

//############################################################################
//##                                                                        ##
//## Stop playback of sample                                                ##
//##                                                                        ##
//## Sample playback may be resumed with AIL_resume_sample(), or restarted  ##
//## from the beginning with AIL_start_sample()                             ##
//##                                                                        ##
//## Playback will stop at the next DMA half-buffer transition              ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_stop_sample( HSAMPLE S )
{
  if ( S == NULL )
  {
    return;
  }

  #ifdef IS_XBOX

  if ( S->driver->sample_status[S->index] == SMP_PLAYING )
  {
    check_hardware_buffer( S, 1 );
  }

  hardware_stop( S );
  #endif

  //
  // Make sure sample is currently playing
  //

  if ( S->driver->sample_status[S->index] != SMP_PLAYING )
  {
    // on the xbox, if the sound is stopped and they call stop again, do the spin
    hardware_spin_until_stopped( S );
    return;
  }

  //
  // Mask 'playing' status
  //

  S->driver->sample_status[S->index] = SMP_STOPPED;
}

//############################################################################
//##                                                                        ##
//## Resume playback of sample from current position                        ##
//##                                                                        ##
//## Playback will resume at the next DMA half-buffer transition            ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_resume_sample( HSAMPLE S )
{
  if ( S == NULL )
  {
    return;
  }

  //
  // See if sample has been previously stopped
  //

  if ( S->driver->sample_status[S->index] == SMP_STOPPED )
  {
    goto startsound;
  }
  else
  {
    if ( S->driver->sample_status[S->index] == SMP_DONE )
    {
      //
      // this means they called resume before start (good for non-zero
      //   start offsets)
      //

      //
      // Make sure valid sample data exists
      //

      if ( ( S->len  [ S->current_buffer ] == 0 ) ||
           ( S->start[ S->current_buffer ] == NULL ) )
      {
        return;
      }

     startsound:

      // Clear residuals
      clear_last_sample(S);

      #if !defined(IS_DOS) && !defined(IS_XBOX) && !defined(IS_XENON) && !defined(IS_PS2) && !defined(IS_PS3) && !defined(IS_WII)
      if ( S->driver->released )
      {
        S->driver->sample_status[S->index] = SMP_PLAYINGBUTRELEASED;
      }
      else
      #endif
      {
        if ( ( S->service_type == 2) && ( S->last_buffer < 0 ) )
          return;

        //
        // start reverb if we need to
        //

        SS_update_sample_reverb_state(S);

        //
        // Set 'playing' status
        //

        S->driver->sample_status[S->index] = SMP_PLAYING;

        //
        // If sample's driver is not already transmitting data, start it
        //

        SS_start_playback( S );
      }
    }
  }
}

//############################################################################
//##                                                                        ##
//## Terminate playback of sample, setting sample status to SMP_DONE        ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_end_sample(HSAMPLE S)
{
  if ( S == NULL )
  {
    return;
  }

  hardware_stop( S );

  //
  // Make sure sample has been allocated
  //

  if ( S->driver->sample_status[S->index] == SMP_FREE )
  {
    return;
  }

  hardware_spin_until_stopped( S );

  //
  // If sample is not already done, halt it and invoke its end-of-buffer
  // and end-of-sample callback functions
  //

  if ( S->driver->sample_status[S->index] != SMP_DONE )
  {
    //
    // Set 'done' status
    //

    S->driver->sample_status[S->index] = SMP_DONE;
    S->starved = 1;

    //
    // Call EOB and EOS functions
    //

    if ( S->EOB != NULL )
    {
      MSS_do_cb1( (AILSAMPLECB) ,S->EOB,S->driver->callingDS,
                   S);
    }

    if ( S->EOS != NULL )
    {
      MSS_do_cb1( (AILSAMPLECB) ,S->EOS,S->driver->callingDS,
                  S);
    }
  }
}


//############################################################################
//##                                                                        ##
//## Set start-of-block (SOB) callback function for sample                  ##
//##                                                                        ##
//## Callback function will be invoked prior to playback of each sample     ##
//## data block, whose position within the sample can be determined by      ##
//## calling AIL_sample_position()                                          ##
//##                                                                        ##
//## The rate at which this function is called is determined by the DMA     ##
//## half-buffer transition period for the driver in use; e.g., a 1K half-  ##
//## buffer being played at 22 kHz will trigger start-of-block callbacks    ##
//## at a rate of 22 per second                                             ##
//##                                                                        ##
//## This function returns the sample's previous SOB callback handler       ##
//## address, or NULL if no SOB callback handler was registered             ##
//##                                                                        ##
//############################################################################

AILSAMPLECB AILCALL AIL_API_register_SOB_callback( HSAMPLE S, AILSAMPLECB SOB )
{
  AILSAMPLECB old;

  if ( S == NULL )
  {
    return( 0 );
  }

  old = S->SOB;

  S->SOB = SOB;

  return( old );
}

//############################################################################
//##                                                                        ##
//## Set end-of-buffer (EOB) callback function for sample                   ##
//##                                                                        ##
//## Callback function will be invoked when either of two sample buffers    ##
//## finish playing                                                         ##
//##                                                                        ##
//## When not double-buffering, the end-of-sample (EOS) callback, if any,   ##
//## will be triggered immediately after the end of buffer 0                ##
//##                                                                        ##
//## This function returns the sample's previous EOB callback handler       ##
//## address, or NULL if no EOB callback handler was registered             ##
//##                                                                        ##
//############################################################################

AILSAMPLECB AILCALL AIL_API_register_EOB_callback( HSAMPLE S, AILSAMPLECB EOB )
{
  AILSAMPLECB old;

  if ( S == NULL )
  {
    return( 0 );
  }

  old = S->EOB;

  S->EOB = EOB;

  return( old );
}

//############################################################################
//##                                                                        ##
//## Set 3D distance falloff function callback for sample                   ##
//##                                                                        ##
//## This function returns the sample's previous falloff callback handler   ##
//## address, which will be the internal default handler if no previous     ##
//## user falloff function has been registered                              ##
//##                                                                        ##
//############################################################################

AILFALLOFFCB AILCALL AIL_API_register_falloff_function_callback( HSAMPLE S, AILFALLOFFCB falloff_cb )
{
  AILFALLOFFCB old;

  if ( S == NULL )
  {
    return( 0 );
  }

  if (falloff_cb == NULL)
   {
   falloff_cb = SS_default_falloff_function_callback;
   }

  old = S->S3D.falloff_function;

  S->S3D.falloff_function = falloff_cb;

  return( old );
}

//############################################################################
//##                                                                        ##
//## Set end-of-sample (EOS) callback function for sample                   ##
//##                                                                        ##
//## Callback function will be invoked when all valid sample buffers have   ##
//## finished playing                                                       ##
//##                                                                        ##
//## When not double-buffering, the end-of-sample (EOS) callback will be    ##
//## triggered immediately after the end of buffer 0                        ##
//##                                                                        ##
//## This function returns the sample's previous EOS callback handler       ##
//## address, or NULL if no EOS callback handler was registered             ##
//##                                                                        ##
//############################################################################

AILSAMPLECB AILCALL AIL_API_register_EOS_callback( HSAMPLE S, AILSAMPLECB EOS )
{
  AILSAMPLECB old;

  if ( S == NULL )
  {
    return( 0 );
  }

  old = S->EOS;

  S->EOS = EOS;

  return( old );
}

//############################################################################
//##                                                                        ##
//## Set sample user data value at specified index                          ##
//##                                                                        ##
//## Any desired value may be stored at one of eight user data words        ##
//## associated with a given SAMPLE                                         ##
//##                                                                        ##
//## Callback functions may access the user data array at interrupt time    ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_sample_user_data( HSAMPLE S, U32 index, SINTa value )
{
  if ( S == NULL )
  {
    return;
  }

  S->user_data[ index ] = value;
}

//############################################################################
//##                                                                        ##
//## Get sample user data value at specified index                          ##
//##                                                                        ##
//## Any desired value may be stored at one of eight user data words        ##
//## associated with a given SAMPLE                                         ##
//##                                                                        ##
//## Callback functions may access the user data array at interrupt time    ##
//##                                                                        ##
//############################################################################

SINTa AILCALL AIL_API_sample_user_data( HSAMPLE S, U32 index )
{
  if ( S == NULL )
  {
    return 0;
  }

  return( S->user_data[ index ] );
}

static S32 nibbles_per_sample( S32 format )
{
  switch ( format )
  {
    case DIG_F_ADPCM_MONO_16:
      return( 1 );
    case DIG_F_ADPCM_STEREO_16:
    case DIG_F_MONO_8:
      return( 2 );
    case DIG_F_STEREO_8:
    case DIG_F_MONO_16:
      return( 4 );
    default:
      return( 8 );
  }
}


//############################################################################
//##                                                                        ##
//## Return minimum buffer size for dual-buffer playback                    ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_API_minimum_sample_buffer_size( HDIGDRIVER dig, //)
                                                S32        playback_rate,
                                                S32        format )
{
#ifdef IS_XBOX

  return( dig->build_size * 2 );

#else

#ifdef MSS_VFLT_SUPPORTED
   if (dig->voice_filter != NULL)
      {
      S32 val = 65536;     // Default value guaranteed to be sufficient if driver fails to implement minimum-size attribute

      AIL_output_filter_driver_property(dig,
                                        "Minimum sample buffer size",
                                        &val,0,0);

      return val;
      }
#endif

  S32  app_nibbles_per_sample;
  S32  hw_nibbles_per_sample;
  S32  n;

  //
  // Get # of application nibbles per sample unit
  //

  app_nibbles_per_sample = nibbles_per_sample( format );

  //
  // Get # of hardware nibbles per sample unit
  // This will be used to determine the rate at which the (physical) hardware consumes the source (logical) data
  //

  hw_nibbles_per_sample = dig->logical_channels_per_sample * dig->bytes_per_channel * 2;

  //
  // Multiply size of hardware half-buffer by ratio of logical-to-physical
  // sample size
  //

  n = dig->hardware_buffer_size * app_nibbles_per_sample / hw_nibbles_per_sample ;

  //
  // Scale n by resampling ratio
  //

  n = n * playback_rate / dig->DMA_rate;

  //
  // Scale n by 2X resampling tolerance to provide safety margin
  //

  n = n + ( ( n * (S32) AIL_preference[ DIG_RESAMPLING_TOLERANCE ] ) / 32768 );

  //
  // If DMA rate is not 1X, 2X, or 4X times playback rate, round buffer
  // size up 1 sample to avoid possible truncation errors
  //

  if ( ( dig->DMA_rate != ( 1 * playback_rate ) ) &&
       ( dig->DMA_rate != ( 2 * playback_rate ) ) &&
       ( dig->DMA_rate != ( 4 * playback_rate ) ) )
  {
    n += 4;
  }

  //
  // Round n up to nearest multiple of 256 bytes
  //

  n = ( n + 255 ) & ~255;

  //
  // Return size *3 (*4 in Win32) to ensure enough data is
  // available for initial buffer fill
  //

#ifndef IS_DOS

#if defined(IS_WIN32)
  if ( AIL_preference[ DIG_USE_WAVEOUT ] )
  {
    n = n * 4;
  }
  else
#endif
#if defined(IS_WIN32)
  {
    // get the number of fragments to mix ahead
    S32 DS_frag_mix_ahead = (S32) AIL_preference[ DIG_DS_MIX_FRAGMENT_CNT ];
    if ( DS_frag_mix_ahead >= dig->DS_frag_cnt )
    {
      DS_frag_mix_ahead = dig->DS_frag_cnt - 1;
    }

    n = n * DS_frag_mix_ahead;
  }
#elif defined(IS_PS3) || defined(IS_WII)
  {
    // get the number of fragments to mix ahead
    S32 DS_frag_mix_ahead = (S32) AIL_preference[ DIG_DS_MIX_FRAGMENT_CNT ];
    if ( DS_frag_mix_ahead >= dig->snd_frag_cnt )
    {
      DS_frag_mix_ahead = dig->snd_frag_cnt - 1;
    }

    n = n * DS_frag_mix_ahead;
  }
#else
#if defined(IS_MAC)
  n = n * AIL_preference[ DIG_SM_MIX_FRAGMENT_CNT ] / 2;
#else
  n = n * 3;
#endif
#endif
#endif

 return( n );
#endif
}

//############################################################################
//##                                                                        ##
//## Set address and length for one of two double-buffered sample buffers   ##
//##                                                                        ##
//## Start playback of sample if not already in progress                    ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_load_sample_buffer(HSAMPLE S, S32 buff_num, void const FAR *buffer, U32 len)
{
  if (S == NULL)
  {
    return;
  }

#ifdef IS_XBOX
  S->direct_mode = 0;
#endif

  S->done [buff_num] = (len == 0);
  S->start[buff_num] = buffer;
  S->len  [buff_num] = len;
  S->pos  [buff_num] = 0;

  S->starved = 0;

  if (len)
  {
    if (S->driver->sample_status[S->index] != SMP_PLAYING)
    {
      //
      // Mark as double-buffered sample
      //

      S->service_type = 2;

#if 0
      //
      // Don't start driver playback until both buffers primed with
      // stream data
      //
      // (As of V7.0, we no longer start playback automatically at all)
      //

      if (S->last_buffer < 0)
      {
        return;
      }
// #endif

      //
      // Set 'playing' status
      //

      #if !defined(IS_DOS) && !defined(IS_XBOX)
      if (S->driver->released)
      {
        S->driver->sample_status[S->index] = SMP_PLAYINGBUTRELEASED;
      }
      else
      #endif
      {
        //
        // start reverb if we need to
        //

        SS_update_sample_reverb_state(S);

        S->driver->sample_status[S->index] = SMP_PLAYING;

        //
        // Start driver playback
        //

        SS_start_playback(S);
      }
#endif
    }
  }
}

//############################################################################
//##                                                                        ##
//## Get double-buffer playback status for sample                           ##
//##                                                                        ##
//##  0: Buffer 0 is ready to be filled (buffer 1 or neither buffer playing)##
//##  1: Buffer 1 is ready to be filled (buffer 0 playing)                  ##
//## -1: Both buffers are already full                                      ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_API_sample_buffer_ready( HSAMPLE S )
{
  if ( S == NULL )
  {
    return -1;
  }

  //
  // For first two calls after sample initialization, return 0 and 1,
  // respectively
  //
  // This allows the application to "prime" the buffers for continued
  // playback
  //

  switch ( S->last_buffer )
  {
    case -2:
      //
      // First call after AIL_init_sample() must clear second buffer's
      // "done" flag to permit buffer-switching
      //

      S->done[ 1 ] = 0;

      //
      // Set up to load buffer 0 this call, and "bootstrap" buffer 1 at
      // next call
      //

      S->last_buffer = -1;
      return( 0 );

    case -1:
      //
      // Return 1 to force load of second buffer immediately
      // Subsequent calls should operate on alternating buffers
      //

      S->last_buffer = S->current_buffer;
      return( 1 );
  }

  //
  // If buffer has not switched since last call, return -1
  //

  if ( S->last_buffer == S->current_buffer )
  {
    return( -1 );
  }

  //
  // New current_buffer exists -- set last_buffer equal to
  // current_buffer and return exhausted buffer
  //

  S->last_buffer = S->current_buffer;

  return( S->current_buffer ^ 1 );
}


//############################################################################
//##                                                                        ##
//## Get information about status of streaming buffer pair                  ##
//##                                                                        ##
//############################################################################

S32  AILCALL AIL_API_sample_buffer_info( HSAMPLE    S, //)
                                         U32     FAR* pos0,
                                         U32     FAR* len0,
                                         U32     FAR* pos1,
                                         U32     FAR* len1 )
{
  if ( S == 0 )
  {
    if ( pos0 ) *pos0 = 0;
    if ( len0 ) *len0 = 0;
    if ( pos1 ) *pos1 = 0;
    if ( len1 ) *len1 = 0;
    return( 0 );
  }
  
  if ( pos0 != NULL )
  {
    *pos0 = S->pos[ 0 ];
  }

  if ( pos1 != NULL )
  {
    *pos1 = S->pos[ 1 ];
  }

  if ( len0 != NULL )
  {
    *len0 = S->len[ 0 ];
  }

  if ( len1 != NULL )
  {
    *len1 = S->len[ 1 ];
  }

  return( S->starved );
}

//############################################################################
//##                                                                        ##
//## Get size and current position of sample in milliseconds                ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_sample_ms_position( HSAMPLE  S, //)
                                         S32 FAR *total_milliseconds,
                                         S32 FAR *current_milliseconds )
{
  U32 datarate;

  if ( S == NULL )
  {
    return;
  }

  S32 effective_rate = S->playback_rate;

  if (S->is_3D && S->S3D.doppler_valid)
   {
   effective_rate = (S32) (((F32) (S->playback_rate)) * S->S3D.doppler_shift);
   }

  //
  // Get data rate in bytes per second
  //

  if ( S->format & DIG_F_ADPCM_MASK )
  {
    //
    // ADPCM compression
    //

    U32 samples_per_block = 4 << ( ( S->format & DIG_F_STEREO_MASK ) ? 1 : 0 );
    samples_per_block = 1 + ( S->adpcm.blocksize - samples_per_block ) * 8 / samples_per_block;

    datarate = ( effective_rate * S->adpcm.blocksize ) / samples_per_block;
  }
  else
  {
    if ( S->pipeline[ SP_ASI_DECODER ].active )
    {
      //
      // ASI compression
      //

      ASISTAGE *ASI = &S->pipeline[ SP_ASI_DECODER ].TYPE.ASI;

      datarate = (U32)~0; ASI->ASI_stream_property( ASI->stream,
                                                    ASI->INPUT_BIT_RATE, &datarate,0,0 );
      datarate /= 8;
    }
    else
    {
      //
      // Straight PCM
      //

      datarate = ( effective_rate * nibbles_per_sample( S->format ) ) / 2;
    }
  }

  //
  // Return requested values
  //

  if ( total_milliseconds )
  {
    *total_milliseconds = (S32) ( ( (float) S->len[ S->current_buffer ] * 1000.0F ) / (float) datarate );
  }

  if ( current_milliseconds )
  {
    *current_milliseconds = (S32) ( ( (float) S->pos[ S->current_buffer ] * 1000.0F ) / (float) datarate );
  }
}

//############################################################################
//##                                                                        ##
//## Seek to a specified millisecond within a sample                        ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_sample_ms_position( HSAMPLE S, //)
                                             S32     milliseconds )
{
  U32 datarate;

  if ( S == NULL )
  {
    return;
  }

  S32 effective_rate = S->playback_rate;

  if (S->is_3D && S->S3D.doppler_valid)
   {
   effective_rate = (S32) (((F32) (S->playback_rate)) * S->S3D.doppler_shift);
   }

  //
  // Get data rate in bytes per second
  //

  if ( S->format & DIG_F_ADPCM_MASK )
  {
    //
    // ADPCM compression
    //

    U32 samples_per_block = 4 << ( ( S->format & DIG_F_STEREO_MASK ) ? 1 : 0 );
    samples_per_block = 1 + ( S->adpcm.blocksize - samples_per_block ) * 8 / samples_per_block;

    datarate = ( effective_rate * S->adpcm.blocksize ) / samples_per_block;
  }
  else
  {
    if ( S->pipeline[ SP_ASI_DECODER ].active )
    {
      //
      // ASI compression
      //

      ASISTAGE *ASI = &S->pipeline[ SP_ASI_DECODER ].TYPE.ASI;

      datarate = (U32)~0; ASI->ASI_stream_property( ASI->stream,
                                                    ASI->INPUT_BIT_RATE, &datarate,0,0 );
      datarate /= 8;
    }
    else
    {
      //
      // Straight PCM
      //

      datarate = ( effective_rate * nibbles_per_sample( S->format ) ) / 2;
    }
  }

  //
  // Set requested position
  //

  AIL_set_sample_position( S, (U32)(S32) ( ( (float) datarate * (float) milliseconds ) / 1000.0F ) );
}

//############################################################################
//##                                                                        ##
//## ASI callback routine to fetch encoded source data from sample          ##
//##                                                                        ##
//## Used by both MSSWO.CPP and MSSDS.CPP                                   ##
//##                                                                        ##
//############################################################################

S32 AILCALLBACK SP_ASI_DECODER_callback(UINTa     user, //)
                                        void FAR *dest,
                                        S32       bytes_requested,
                                        S32       offset)
{
   HSAMPLE S = (HSAMPLE) user;

   S32 total = 0;

   S32 n = S->current_buffer;

   if (offset > (S32) S->len[n])
      {
      offset = S->len[n];     // Needed to make XMA ASI work with double-buffers whose len < the total RIFF len
      }

   if (offset != -1)
      {
      S->pos[n] = offset;
      }

   if (S->exhaust_ASI)
      {
      //
      // We decremented S->pos by one byte in SS_merge() to keep it from reaching S->len 
      // until all buffered data was retrieved from the ASI codec.  Now, since we're in the decoder
      // callback, all of the buffered output data has been used up and it's time to fetch 
      // some more source data if possible.  Undo the decrement here to let the sample 
      // terminate normally, or switch its buffers, without feeding the byte at len-1 to the ASI codec.
      //

      S->exhaust_ASI = 0;
      S->pos[n] = S->len[n];
      }

   //
   // Fulfill as much of request as possible from current buffer
   //

   S32 amount = bytes_requested;

   if ((S->pos[n] + amount) > S->len[n])
      {
      amount = S->len[n] - S->pos[n];
      }

#if 0
   //
   // We *should* be doing a loop-iteration test here, but seeking within an ASI data-fetch 
   // callback is unlikely to work except under very constrained conditions that we can't 
   // enforce here.  Consequently, the output stream of an ASI codec may overshoot the end 
   // of a loop on occasion.
   //

   if (S->loop_count != 1)
      {
      if ((S->loop_end != -1) && ((U32) S->loop_end < S->len[n]))
         {
         S32 loop_left = S->loop_end - S->pos[n];

         if (amount > loop_left)
            {
            amount = loop_left;
            }
         }
      }
#endif

   AIL_memcpy(dest,
              AIL_ptr_add(S->start[n], S->pos[n]),
              amount);

   dest = AIL_ptr_add(dest, amount);

   S->pos[n] += amount;

   total += amount;
   bytes_requested -= amount;

   //
   // If end of buffer reached with samples left to fetch, try to
   // switch buffers or execute a loop iteration
   //

   if (bytes_requested > 0)
      {
      if (S->EOB != NULL)
         ++S->doeob;

      if (S->service_type == 1)
         {
#if 0
         if (S->loop_count != 1)
            {
            S->pos[S->current_buffer] = S->loop_start;

            //
            // Clear ASI filter history
            // (see comment above)
            //

            ASISTAGE *ASI = &S->pipeline[SP_ASI_DECODER].TYPE.ASI;
            ASI->ASI_stream_seek(ASI->stream, -2);

            //
            // Decrement loop count if not infinite
            //
    
            if (S->loop_count != 0)
               {
               --S->loop_count;
               }

            goto try_again;
            }
#endif
         }
      else
         {
         // reset the ASI, if requested
         if (S->reset_ASI[n])
         {
           // return without switching buffers if we were requested to do a reset
           return(total);
         }

         n ^= 1;

         //
         // If explicit 0-length buffer was posted, fall through to allow
         // the sample to terminate
         //

         if (S->done[n])
            {
            return total;
            }
         else
            {
            //
            // If alternate buffer not yet initialized, break out of loop
            // to allow application to supply data as soon as it can
            //

            if (!S->len[n])
               {
               S->starved = 1;
               return total;
               }

            //
            // If alternate buffer already played, break out of loop to
            // allow application to refresh it as soon as possible
            //

            if (S->pos[n])
               {
               S->starved = 1;
               return total;
               }

            //
            // Otherwise, alternate buffer is ready to play -- switch to
            // it and keep filling output buffer
            //

            S->current_buffer = n;

            amount = bytes_requested;

            if ((S->pos[n] + amount) > S->len[n])
               {
               amount = S->len[n] - S->pos[n];
               }

            AIL_memcpy(dest,
                       AIL_ptr_add(S->start[n], S->pos[n]),
                       amount);

            S->pos[n] += amount;

            total += amount;
            }
         }
      }

   return total;
}

#if (defined(IS_MAC) && defined( IS_X86 )) || defined( FORNONWIN )

DXDEF __declspec(naked) S32 AIL_fetch_CB( VOIDFUNC * fetch, UINTa usr, void * dest, S32 bytes, S32 offset )
{
  __asm
  {
    __asm mov eax,16
    __asm sub esp,64
    __asm add eax,esp
    __asm and eax,0xfffffff0
    __asm movups xmm0,[esp+64+4+4]
    __asm movaps [eax],xmm0
    __asm mov dword ptr [eax+16],esp
    __asm mov edx,dword ptr [esp+64+4]
    __asm mov esp,eax
    __asm call edx
    __asm mov esp,dword ptr [esp+16]
    __asm add esp,64
    __asm ret
  }
}

#endif

//############################################################################
//#                                                                          #
//# Add audio data to build and reverb buffers with optional level scaling   #
//#                                                                          #
//# This routine is aware of reverb and multichannel build buffers, but does #
//# not do resampling or format conversion like the "real" mixer             #
//#                                                                          #
//# It can replicate mono data to a stereo build buffer, but this capability #
//# is not currently needed                                                  #
//#                                                                          #
//############################################################################
                    
static void MC_mix (HSAMPLE     S,                      //)  
             S16 FAR    * MSSRESTRICT ssrc,         
             S32         src_bytes,
             MSS_BB FAR *dest_buffers, 
             S32         n_dest_buffers,
             S32 FAR    * MSSRESTRICT reverb_dest,
             S32         reverb_channel_scale,
             S32         dest_offset,
             S32         src_stereo )
{
  void FAR * MSSRESTRICT src_end;
  S32 i;
  
  src_end = AIL_ptr_add( ssrc, src_bytes );

  if ( reverb_dest )
  {
    if ( reverb_channel_scale )
    {
      void FAR * MSSRESTRICT src;
      void FAR * dest;
      void FAR * MSSRESTRICT dest_end;
      S32 op;
      S32 lrtemp = 0;
      U32 temp = 0;

      op = M_SRC_16;
      if ( src_stereo )
        op |= M_SRC_STEREO;

      if ( reverb_channel_scale < 2048 )
        op |= M_VOL_SCALING;

      src = ssrc;         

      if ( src_stereo )
        dest = AIL_ptr_add( reverb_dest, dest_offset );  // 16s to 32m
      else
        dest = AIL_ptr_add( reverb_dest, dest_offset * 2 ); // 16m to 32m

      dest_end = AIL_ptr_add( reverb_dest, S->driver->reverb_fragment_size );

      S->pipeline[SP_MERGE].TYPE.MSS_mixer_merge( (void const **)&src,
                                                  &temp,
                                                  src_end,
                        (S32 FAR * FAR *)(void FAR*) &dest,
                                                  dest_end,
                                                  &lrtemp,
                                                  &lrtemp,
                                                  65536,
                                                  reverb_channel_scale,
                                                  reverb_channel_scale,
                                                  op
#ifdef IS_X86
                                                  ,S->driver->use_MMX
#endif           
                                                  );
    }
  }

  S32 ramp = ( src_stereo ) ? ( src_bytes >> 2 ) : ( src_bytes >> 1 );

  for ( i = 0 ; i < n_dest_buffers ; i++ )
  {
    void FAR * MSSRESTRICT src;
    void FAR * dest;
    void FAR * MSSRESTRICT dest_end;
    S32 op;
    S32 left, right, oleft, oright, lrtemp = 0;
    U32 temp = 0;

    src = ssrc;
    dest_end = AIL_ptr_add( dest_buffers[i].buffer, dest_buffers[i].bytes );
    
    op = M_SRC_16;

    left   = S->cur_scale [ i * 2 ];
    oleft  = S->prev_scale[ i * 2 ];
    right  = S->cur_scale [ i * 2 + 1 ];
    oright = S->prev_scale[ i * 2 + 1 ];

    if ( ( ( left != oleft ) || ( right != oright ) ) && ( S->ramps_left[i] == 0 ) )
    {
      // volume has changed and we aren't ramping, so start a new ramp period
      S->ramps_left[i]=(S32) AIL_preference[DIG_LEVEL_RAMP_SAMPLES];
    }

    S32 lv = oleft << 16;
    S32 rv = oright << 16;

    S32 r = ramp;

    if ( r > S->ramps_left[i] )
    {
      r = S->ramps_left[i];
    }
    
    if ( src_stereo )
    {
      op |= M_SRC_STEREO;

      if( dest_buffers[ i ].chans > 1 ) 
      {
        op |= M_DEST_STEREO;
        dest = AIL_ptr_add( dest_buffers[i].buffer, dest_offset * 2 ); // 16s to 32s

        if (((left != oleft) || (right != oright)) && (r))
        {
          S32 ladj = ( ( left - oleft ) << 16 ) / S->ramps_left[i];
          S32 radj = ( ( right - oright ) << 16 ) / S->ramps_left[i];
          S->ramps_left[i] -= r;

          while ( r ) 
          {
            ((S32*)dest)[0] += ( ( (S32) ( (S16) LE_SWAP16( src ) ) ) * ( lv >> 16 ) );
            ((S32*)dest)[1] += ( ( (S32) ( (S16) LE_SWAP16_OFS( src, 2 ) ) ) * ( rv >> 16 ) );
            lv += ladj;
            rv += radj;
            
            dest = AIL_ptr_add( dest, 8 );
            src = AIL_ptr_add( src, 4 );
            --r;
          }
        }
      }
      else
      {
        dest = AIL_ptr_add( dest_buffers[i].buffer, dest_offset );  // 16s to 32m

        if (((left != oleft) || (right != oright)) && (r))
        {
          S32 ladj = ( ( left - oleft ) << 16 ) / S->ramps_left[i];
          S32 radj = ( ( right - oright ) << 16 ) / S->ramps_left[i];
          S->ramps_left[i] -= r;

          while ( r ) 
          {
            ((S32*)dest)[0] += ( ( (S32) ( (S16) LE_SWAP16( src ) ) ) * ( lv >> 16 ) ) +
                               ( ( (S32) ( (S16) LE_SWAP16_OFS( src, 2 ) ) ) * ( rv >> 16 ) );
            lv += ladj;
            rv += radj;
            
            dest = AIL_ptr_add( dest, 4 );
            src = AIL_ptr_add( src, 4 );
            --r;
          }
        }
      }
    }
    else
    {
      if( dest_buffers[ i ].chans > 1 ) 
      {
        op |= M_DEST_STEREO;
        dest = AIL_ptr_add( dest_buffers[i].buffer, dest_offset * 4 ); // 16m to 32s

        if (((left != oleft) || (right != oright)) && (r))
        {
          S32 ladj = ( ( left - oleft ) << 16 ) / S->ramps_left[i];
          S32 radj = ( ( right - oright ) << 16 ) / S->ramps_left[i];

          S->ramps_left[i] -= r;

          while ( r ) 
          {
            S32 s = ( (S32) ( (S16) LE_SWAP16( src ) ) );
            
            ((S32*)dest)[0] += ( s * ( lv >> 16 ) );
            ((S32*)dest)[1] += ( s * ( rv >> 16 ) );
            lv += ladj;
            rv += radj;
            
            dest = AIL_ptr_add( dest, 8 );
            src = AIL_ptr_add( src, 2 );
            --r;
          }
        }
      }
      else
      {
        dest = AIL_ptr_add( dest_buffers[i].buffer, dest_offset * 2 ); // 16m to 32m

        if ((left != oleft) && (r))
        {
          S32 ladj = ( ( left - oleft ) << 16 ) / S->ramps_left[i];
          S->ramps_left[i] -= r;
          while ( r ) 
          {
            ((S32*)dest)[0] += ( ( (S32) ( (S16) LE_SWAP16( src ) ) ) * ( lv >> 16 ) );
            lv += ladj;
            
            dest = AIL_ptr_add( dest, 4 );
            src = AIL_ptr_add( src, 2 );
            --r;
          }
        }
      }
    }

    if (S->ramps_left[i])
      {
      //
      // We may not have made it all the way to cur_scale in this sub-interval, so 
      // keep track of how far we got to avoid discontinuities in the next call    
      //

      left  = lv >> 16;                    
      right = rv >> 16;                    
      S->cur_scale[ i * 2     ] = left;    
      S->cur_scale[ i * 2 + 1 ] = right;   
      }

    if ( left | right )
    {
      if ( ( left < 2048 ) || ( right < 2048 ) ) 
        op |= M_VOL_SCALING;

      S->pipeline[SP_MERGE].TYPE.MSS_mixer_merge( (void const **)&src,
                                                  &temp,
                                                   src_end,
                     (S32 FAR * FAR *)(void FAR*) &dest,
                                                   dest_end,
                                                  &lrtemp,
                                                  &lrtemp,
                                                   65536,
                                                   left,
                                                   right,
                                                   op
#ifdef IS_X86
                                                   ,S->driver->use_MMX
#endif           
                                                   );
    }
  }
}


#ifdef MSS_FLT_SUPPORTED

//############################################################################
//#                                                                          #
//# Apply filter chain to data at specified location in build buffer FLT_A   #
//#                                                                          #
//############################################################################

void FAR * SS_execute_filter_chain(HSAMPLE  S, //)
                                   void FAR * src,
                                   void FAR * temp_dest,
                                   S32      src_bytes,
                                   S32      is_stereo,
                                   S16 FAR *dest_mono_sample_buffer)
{
   HDIGDRIVER dig = S->driver;

   S32 n_samples = (S32) ( src_bytes / sizeof(S16) );

   if ( is_stereo )
     n_samples >>= 1;

   //
   // Effective sample rate for filters = DMA output rate (which is S->playback_rate if
   // a voice filter is in use)
   //

   S32 sample_rate = (dest_mono_sample_buffer == NULL) ? dig->DMA_rate : S->playback_rate;

   //
   // Build linear list of filters to execute
   // 

   SAMPLESTAGE stage,filter_list[8];
   S32 i,n = 0;

   for (stage = SP_FILTER_0; stage <= SP_FILTER_7; stage = (SAMPLESTAGE) (stage + 1))
      {
      if (S->pipeline[stage].active)
         {
         filter_list[n++] = stage;
         }
      }

   if (n == 0)
      {
      return 0;     // Shouldn't happen (data won't get copied to the output buffer)
      }

   //
   // Execute all filters in list
   //
   // Don't apply volume scaling here -- it will be applied by a later MC_mix() call, or by the voice output
   // filter itself
   //

   for ( i = 0; i < n; )
      {
      stage = filter_list[i];

      FLTPROVIDER FAR *F = S->pipeline[stage].TYPE.FLT.provider;
      HSAMPLESTATE state = S->pipeline[stage].TYPE.FLT.sample_state;

      ++i;

      // save out new dest in the return pointer
      void * dest = ( ( dest_mono_sample_buffer ) && ( i == n ) ) ? dest_mono_sample_buffer : temp_dest;

      // if we are using an output filter and we are on the last buffer, filter right into
      //   the dest_mono_sample_buffer
      F->sample_process(state,
                        src,
                        dest,        // usually will be the same value as src
                        n_samples,
                        sample_rate,
                        is_stereo );

      src = dest;
      }

   return src;
}

#endif


//############################################################################
//##                                                                        ##
//## Flush mixer buffer                                                     ##
//##                                                                        ##
//############################################################################

#ifndef IS_XBOX

void AILCALL SS_flush (HDIGDRIVER dig)
{
   //
   // Initialize the build buffer by flushing with 0
   //

   for (S32 i=0; i < dig->n_build_buffers; i++)
      {
      dig->pipeline[DP_FLUSH].TYPE.MSS_mixer_flush(dig->build[i].buffer,
                                                   dig->build[i].bytes
#ifdef IS_X86
                                                  ,dig->use_MMX
#endif
                                                   );
      }
/*                                 
#ifdef MSS_FLT_SUPPORTED
      dig->pipeline[DP_FLUSH].TYPE.MSS_mixer_flush(dig->build[FLT_A].buffer,
                                                   dig->build[FLT_A].bytes
#ifdef IS_X86
                                                  ,dig->use_MMX
#endif
                                                   );

      dig->pipeline[DP_FLUSH].TYPE.MSS_mixer_flush(dig->build[FLT_B].buffer,
                                                   dig->build[FLT_B].bytes
#ifdef IS_X86
                                                  ,dig->use_MMX
#endif
                                                   );

#endif
*/
   if ( dig->reverb_on )
   {
     {
       // clear the reverb buffer
       dig->pipeline[DP_FLUSH].TYPE.MSS_mixer_flush((S32 FAR *)AIL_ptr_add( dig->reverb_build_buffer, dig->reverb_head ),
                                                    dig->reverb_fragment_size
#ifdef IS_X86
                                                    ,dig->use_MMX
#endif
                                                    );
     }

     if ( AIL_ms_count() >= dig->reverb_off_time_ms )
     {
       dig->reverb_on = 0;
     }

   }

#ifdef MSS_FLT_SUPPORTED

   //
   // Invoke installable filters
   //

   FLT_call_premix_processors(dig);

#endif
}

//############################################################################
//##                                                                        ##
//## Merge source data into output buffer by calling processor and mixer    ##
//## handlers                                                               ##
//##                                                                        ##
//## Optionally, processed source data may be downconverted to mono         ##
//## and written to a specified 16-bit destination buffer for the benefit   ##
//## of per-sample voice filters.  Reverb, volume-scaling, and resampling   ##
//## are unsupported by this path.                                          ##
//##                                                                        ##
//############################################################################

S32 SS_merge (HSAMPLE  S, //)
              S32      in_len,
              S32      out_pos,
              S16 FAR *dest_mono_sample_buffer,
              S32      dest_buffer_size)
{
   //
   // Set mixer operation code
   //

   S32 op                = 0;
   S32 src_sample_shift  = 0;
   S32 dest_sample_shift = 1;
   U32 out_pos_shift     = 0;

   HDIGDRIVER dig = S->driver;

   if (dest_mono_sample_buffer == NULL)
      {
      if (dig->hw_format & (DIG_F_STEREO_MASK | DIG_F_MULTICHANNEL_MASK))
        {
           if (S->format & DIG_F_STEREO_MASK)
           {
           op |= M_DEST_STEREO;
           dest_sample_shift += 1;

           //
           // Reverse left/right channels if DIG_PCM_ORDER specified
           //

           if (S->flags & DIG_PCM_ORDER)
              {
              op |= M_ORDER;
              }

           }
           else
           {
             dest_buffer_size >>= 1;
             out_pos >>= 1;
             out_pos_shift = 1;
           }
        }
      }

   if (S->format & DIG_F_STEREO_MASK)
      {
      op |= M_SRC_STEREO;
      src_sample_shift += 1;
      }

   if ((S->format & DIG_F_16BITS_MASK) || (S->format & DIG_F_ADPCM_MASK) || (S->pipeline[SP_ASI_DECODER].active) )
      {
      op |= M_SRC_16;
      src_sample_shift += 1;
      }

   //
   // Calculate sampling fraction
   //

   S32 playback_ratio;
  
   if (dest_mono_sample_buffer != NULL)
      {
      playback_ratio = 65536;
      S->src_fract   = 0;
      }
   else
      {
      F32 rate_shift;

      if (S->is_3D && S->S3D.doppler_valid)
         {
         rate_shift = S->S3D.doppler_shift;
         }
      else
         {
         rate_shift = 1.0F;
         }

      playback_ratio = (S32) ((((F32) S->playback_rate) * rate_shift * 65536.0F) /
                                (F32) dig->DMA_rate);

      if ((playback_ratio > (65536 + AIL_preference[DIG_RESAMPLING_TOLERANCE])) ||
          (playback_ratio < (65536 - AIL_preference[DIG_RESAMPLING_TOLERANCE])))
         {
         op |= M_RESAMPLE;

         //
         // Enable filtering if preference set
         //

         if (AIL_preference[ DIG_ENABLE_RESAMPLE_FILTER ])
            {
            op |= M_FILTER;
            }
         }
      else
         {
         playback_ratio = 65536;
         S->src_fract   = 0x20000000;  //flag to say last buffer was non-filter
         }
      }

  //
  // Calculate # of bytes needed for this loop
  //
  // This calculation takes resampling into account, as well as the
  // difference in sample sizes between the output of the decode stage
  // ("source sample") and the mixer build buffer.  The resulting sample
  // count reflects the amount of data needed to fill the build buffer
  // from its current out_pos write position.
  //

  S32 needed_dest = dest_buffer_size - out_pos;

  S32 needed_src = mult64addsubandshift( ( needed_dest >> dest_sample_shift ), playback_ratio,
                                          0xffff, 65536 - ( S->src_fract & 0xffff ), 16
                                       ) << src_sample_shift;

  if ( S->src_fract & 0xc0000000 )
    needed_src -= ( ( ( playback_ratio >> 16 ) - ( ( S->src_fract & ~0xc0000000 ) >> 16 ) ) << src_sample_shift );
  else
    needed_src += ( 1 << src_sample_shift);

  if ( needed_src <= 0 )
    needed_src = ( 1 << src_sample_shift );

  S32 using_process = 0;

  U8 process_buffer_start[ DIG_PROCESS_BUFFER_SIZE + 16 + 15 ];

  //
  // align our temp pointer
  //

  U8 FAR * process_buffer = (U8 FAR*) ( ( ( (UINTa) process_buffer_start ) + 15 ) & ~15 );

  //
  // If ASI or ADPCM compression is in use, unpack data into
  // temporary buffer
  //

  void const FAR *src;
  void FAR       *src_end;
  ASISTAGE   FAR *ASI = NULL; 

  U8 *process_ptr = process_buffer;

  if (S->pipeline[SP_ASI_DECODER].active)
  {
    S32 src_adjust = 0;
    
    //
    // ASI decoder in use -- source data must be obtained from
    // SP_ASI_DECODER_callback() via ASI_stream_process()
    //
    // ok, if we have a sample left over from the last decompressed buffer, 
    //   then we need to use it first
    if ( S->last_decomp_left != 65536 )
    {
      // we don't store these specifically little endian because we are
      //   just storing exactly what we read earlier (that is, on big endian
      //   machines, last_decomp_left/right are byte swapped in the value)   
      ( ( S16 FAR* ) process_buffer)[ 0 ] = (S16)S->last_decomp_left;
      ( ( S16 FAR* ) process_buffer)[ 1 ] = (S16)S->last_decomp_right;

      src_adjust = ( 1 << src_sample_shift ); 
      process_ptr += ( ( op & M_SRC_STEREO ) ? 4 : 2 );
    }

    S32 decode_src = needed_src - src_adjust;

    if ( decode_src > 0 )
    {
      ASI = &S->pipeline[SP_ASI_DECODER].TYPE.ASI; 

      needed_src = ASI->ASI_stream_process(ASI->stream,
                                           process_ptr,
                                         ( decode_src > DIG_PROCESS_BUFFER_SIZE ) ? DIG_PROCESS_BUFFER_SIZE : decode_src );
      needed_src += src_adjust;
    }                                       

   //
   // Don't let position cursor hit the end of the input buffer if there's still some
   // decoded data in the ASI output buffer
   //

   if (needed_src != 0) 
      {
      S32 n = S->current_buffer;

      if ((S->pos[n] == S->len[n]) && (ASI->OUTPUT_RESERVOIR != (UINTa) -1))
         {
         S32 left = ~0;

         ASI->ASI_stream_property(ASI->stream, 
                                  ASI->OUTPUT_RESERVOIR,&left,0,0);

         if (left > 0)
            {
            S->pos[n] = S->len[n] - 1;
            S->exhaust_ASI = 1;
            }
         }
      }
   else
      {
       // if no data was output and if we have to reset the ASI,
       //   don't advance the output pointer, because we'll just call this
       //   routine again immediately (after the ASI has been reset out at
       //   the upper layer).
       
       if ( S->reset_ASI[S->current_buffer])
         {
         return 0;
         }
      }

    using_process = 1;
    src = process_buffer;
    src_end = AIL_ptr_add( src, needed_src );
  }
  else
  {
    //
    // No ASI decoder in use -- source data is available in its entirety
    // from the current buffer
    //

    src = AIL_ptr_add(S->start[S->current_buffer],
                      S->pos  [S->current_buffer]);

    src_end = AIL_ptr_add(src,
                          in_len);

    //
    // If ADPCM compression in use, decompress the data into the decode
    // buffer
    //

    if (S->format & DIG_F_ADPCM_MASK)
    {
      U32 decode_src = needed_src;

      // ok, if we have a sample left over from the last decompressed buffer, 
      //   then we need to use it first
      if ( S->last_decomp_left != 65536 )
      {
        // we don't store these specifically little endian because we are
        //   just storing exactly what we read earlier (that is, on big endian
        //   machines, last_decomp_left/right are byte swapped in the value)   
        ( ( S16 FAR* ) process_buffer)[ 0 ] = (S16)S->last_decomp_left;
        ( ( S16 FAR* ) process_buffer)[ 1 ] = (S16)S->last_decomp_right;

        decode_src -= ( 1 << src_sample_shift );
        process_ptr += ( ( op & M_SRC_STEREO ) ? 4 : 2 );
      }
      
      if ( decode_src > 0)
      {
        void const FAR *in  = src;
        void FAR *out       = process_ptr;

        //
        // Decode block of data from source sample to decode buffer
        //
        // Size of block to decode is determined by decoder
        // given smaller amount of available and needed data
        //

        S->driver->pipeline[DP_ADPCM_DECODE].TYPE.MSS_mixer_adpcm_decode(
                               &out, &in,
                             ( decode_src > DIG_PROCESS_BUFFER_SIZE ) ? DIG_PROCESS_BUFFER_SIZE : decode_src,
                               in_len, op, &S->adpcm);

        //
        // Update source sample position index
        //
        // Note: In Win16, *in and *out are normalized upon return from
        // the ADPCM decoder.  Their selectors may not be the same as the
        // original selectors passed to *in and *out.  Since we are writing
        // to the (small) decode buffer, *out will not be renormalized in
        // this application -- but *in often will.
        //

        using_process = 1;
        src = process_buffer;
        src_end = out;

        needed_src = (S32) AIL_ptr_dif( src_end, src );

        S->pos[S->current_buffer] = (S32) AIL_ptr_dif(in,
                                                      S->start[S->current_buffer]);
     }
    }
    else
    {
      if ( needed_src <= 0 )
      {
        //
        // we always need at least one input sample, but only *advance* the
        // source pointer if we are about to wrap around to the next one
        //

        needed_src = ( 1 << src_sample_shift );
        if ( ( S->src_fract & 0xffff ) + playback_ratio >= 65536 )
          goto uncomp_advance;
      }
      else
      {
uncomp_advance:

        //
        // clamp to the end of the buffer
        //

        if ( needed_src > in_len )
          needed_src = in_len;

        if ( S->lp.on )
        {
          if ( needed_src > DIG_PROCESS_BUFFER_SIZE )
          {
            needed_src = DIG_PROCESS_BUFFER_SIZE;
          }
        }
      }

      src_end = AIL_ptr_add( src, needed_src );
    }
  }

  //
  // If empty source or dest range, return build buffer full
  //

  if ( ( needed_src <= 0 ) || ( needed_dest <= 0 ) )
  {
    // we return buffer full so that the filling routine that calls
    //   ss_merge exits (otherwise it just keeps calling)

    if ((dest_mono_sample_buffer != NULL) && (needed_dest != 0))
      {
      void FAR *dest = AIL_ptr_add(dest_mono_sample_buffer, out_pos);

      AIL_memset(dest,
                 0,
                 needed_dest);
      }

    return dest_buffer_size << out_pos_shift;
  }

  //
  // Calc new needed_dest based on our src size (which may have changed
  //   due to input buffer size and/or compression
  //

  S32 dest_from_src = shift64addsubanddiv( needed_src >> src_sample_shift, 
                                           16,
                                           65536 - ( S->src_fract & 0xffff ), 
                                           playback_ratio >> 1,
                                           playback_ratio ) << dest_sample_shift;

  if ( dest_from_src < needed_dest )
    needed_dest = ( dest_from_src ) ? dest_from_src : ( 1 << dest_sample_shift );

   // ====================================================================================
   // Get reverb and per-channel dry destination levels, and see if zero-volume optimizations are
   // possible
   // ====================================================================================

   S32 zero_volume = FALSE;

   F32 SC[8];
   S32 S_mask = 1;

   if (dest_mono_sample_buffer == NULL)
      {
      F32 dry_scale = dig->master_volume * dig->master_dry * S->dry_level * (1.0F - S->obstruction);

      S32 z = 0;

      SC[0] = S->left_volume  * dry_scale;
      SC[1] = S->right_volume * dry_scale;

      if ((dig->channel_spec == MSS_MC_51_DISCRETE) || 
          (dig->channel_spec == MSS_MC_51_DTS))
         {
         //
         // Use additional 5.1-specific "shortcut" volume scalars
         //

         SC[2] = S->center_volume * dry_scale;
         SC[3] = S->low_volume    * dry_scale;
         SC[4] = S->leftb_volume  * dry_scale;
         SC[5] = S->rightb_volume * dry_scale;
         S_mask = 7;
         }

      for (S32 b = 0; b < dig->n_build_buffers; b++)   
         {
         MSS_BB *B = &dig->build[b];

         S32 index = b*2;

         F32 left_scale  = SC[(index)   & S_mask] * dig->D3D.speaker_dry_reverb_response[index];   
         F32 right_scale = SC[(index+1) & S_mask] * dig->D3D.speaker_dry_reverb_response[index+1];

         //
         // Set up previous-scale values for volume-scaled writes to this build buffer
         // on behalf of the currently-processed HSAMPLE
         //

         S->prev_scale[index]   = S->cur_scale[index];
         S->prev_scale[index+1] = S->cur_scale[index+1];

         //
         // Calculate volume scalars used for writes to each build buffer
         //
         // All build buffers are stereo except the last, which is mono if the output
         // format is mono or if it's a remnant buffer in an odd multichannel 
         // configuration (e.g., 6.1)
         //

         if (B->chans == 1)
            {
            S->cur_scale[index]   = (S32) ((2048.0F * left_scale * S->user_channel_levels[index] * S->auto_3D_channel_levels[index]) + 0.5F);
            S->cur_scale[index+1] = 0;
            
            if (S->cur_scale[index] >= 2048) S->cur_scale[index] = 2048;

            if (S->cur_scale[index] == 0)
               {
               if ((S->wet_level < MSS_EPSILON) || (dig->master_wet < MSS_EPSILON))
                  {
                  z++;
                  }
               }
            }
         else
            {
            S->cur_scale[index]   = (S32) ((2048.0F * left_scale  * S->user_channel_levels[index  ] * S->auto_3D_channel_levels[index  ]) + 0.5F);
            S->cur_scale[index+1] = (S32) ((2048.0F * right_scale * S->user_channel_levels[index+1] * S->auto_3D_channel_levels[index+1]) + 0.5F);
            
            if (S->cur_scale[index]   >= 2048) S->cur_scale[index]   = 2048;
            if (S->cur_scale[index+1] >= 2048) S->cur_scale[index+1] = 2048;

            if ((S->cur_scale[index] == 0) && (S->cur_scale[index+1] == 0))
               {
               if ((S->wet_level < MSS_EPSILON) || (dig->master_wet < MSS_EPSILON))
                  {
                  z++;
                  }
               }
            }
         }

      if (z == dig->n_build_buffers)
         {
         zero_volume = TRUE;
         }
      }

   // ====================================================================================
   // Get output processing options
   // ====================================================================================
  
   S32 reverb_enabled  = 0;
   S32 filters_enabled = 0;

   S32 FAR  *reverb_dest          = NULL;
   S32       reverb_channel_scale = 0;

#ifdef MSS_REVERB_SUPPORTED

   F32 reverb_scale = dig->master_volume * 
                      dig->master_wet    * 
                      S->save_volume     * 
                      S->wet_level       * 
                      (1.0F - S->exclusion);

   if (S->S3D.auto_3D_atten)
      {
      reverb_scale *= S->S3D.atten_3D;
      }

   S32 zero_reverb_volume = FALSE;
   U32 reverb_timer       = 0;

   if (dest_mono_sample_buffer == NULL)
      {
      if (reverb_scale < 0.0001F)
         {
         zero_reverb_volume = TRUE;
         }

      if ((dig->reverb_on) && (!zero_reverb_volume))
         {
         reverb_enabled = 1;

         reverb_channel_scale = (S32) ((2048.0F * reverb_scale) + 0.5F); 
         if (reverb_channel_scale >= 2048) reverb_channel_scale = 2048;

         //
         // Calculate destination address in global mono reverb buffer
         //

         reverb_dest = (S32 FAR *) AIL_ptr_add(dig->reverb_build_buffer, dig->reverb_head);

         // advance the time when the reverb should turn off

         reverb_timer = AIL_ms_count(); 

         if (((U32) (dig->reverb_off_time_ms - reverb_timer)) < (dig->reverb_duration_ms + 100))
            {
            dig->reverb_off_time_ms = reverb_timer + dig->reverb_duration_ms + 100;
            }
         }
      }
#endif

#ifdef MSS_FLT_SUPPORTED
   if ((S->n_active_filters > 0) || (S->lp.on))
      {
      filters_enabled = 1;
      }
#endif

   // ====================================================================================
   // Call mixer provider
   // ====================================================================================

   const void FAR *mixer_input = src;

   if (zero_volume)
      {
      // --------------------------------------------------------------------------------
      // Fast path #1: volume is zero, so just advance the dest pointer (don't mix)
      // --------------------------------------------------------------------------------

      clear_last_sample(S);

      src = src_end;

      out_pos += needed_dest;
      }
   else
      {
      // ------------------------------------------------------------------------------------
      // Initialize last-value history for filters properly following a non-resampled
      // mixer frame
      // ------------------------------------------------------------------------------------

      if ((S->src_fract == 0x20000000) && (op & M_FILTER))
         {
         S->src_fract = 0;

         if (S->format & DIG_F_16BITS_MASK)
            {
            if ( S->format & DIG_F_STEREO_MASK )
               {
               if (S->flags & DIG_PCM_ORDER)
                  {
                  S->left_val   = (S16) LE_SWAP16(&( (S16 FAR*) src )[ 1 ]);
                  S->right_val  = (S16) LE_SWAP16(&( (S16 FAR*) src )[ 0 ]);
                  }
               else
                  {
                  S->left_val   = (S16) LE_SWAP16(&( (S16 FAR*) src )[ 0 ]);
                  S->right_val  = (S16) LE_SWAP16(&( (S16 FAR*) src )[ 1 ]);
                  }
               }
            else
               {
               S->left_val  = (S16) LE_SWAP16(&( (S16 FAR*) src )[ 0 ]);
               S->right_val = S->left_val;
               }
            }
         else
            {
            if ( S->format & DIG_F_STEREO_MASK )
               {
               if (S->flags & DIG_PCM_ORDER)
                  {
                  S->left_val  = ( (U8 FAR*) src )[ 1 ] - 128;
                  S->right_val = ( (U8 FAR*) src )[ 0 ] - 128;
                  }
               else
                  {
                  S->left_val  = ( (U8 FAR*) src )[ 0 ] - 128;
                  S->right_val = ( (U8 FAR*) src )[ 1 ] - 128;
                  }
               }
            else
               {
               S->left_val  = ( (U8 FAR*) src )[ 0 ] - 128;
               S->right_val = S->left_val;
               }
            }
         }

      //
      // See if we need to downmix a stereo source to mono for 3D positioning
      //

      S32 need_downmix = (S->is_3D && (op & M_SRC_STEREO) && (op & M_DEST_STEREO));

      if (need_downmix)
         {
         op &= ~M_DEST_STEREO;   // force conversion to mono with downmix/6-dB cut
         needed_dest /= 2;       // only generate half as much data, so we can replicate it back to stereo
         }

#ifdef MSS_FLT_SUPPORTED
      if ( (!filters_enabled) && ( dest_mono_sample_buffer ) )
         {
         // --------------------------------------------------------------------------------
         // Fast path #2: no filters into a hardware buffer (direct copy)
         // --------------------------------------------------------------------------------

          const void FAR *dest = AIL_ptr_add(dest_mono_sample_buffer, out_pos);
          void FAR *dest_end   = AIL_ptr_add(dest_mono_sample_buffer, out_pos + needed_dest);

          S->pipeline[SP_MERGE].TYPE.MSS_mixer_merge(&src,
                                                     &S->src_fract,
                                                      src_end,
                        (S32 FAR * FAR *)(void FAR*) &dest,
                                                      dest_end,
                                                     &S->left_val,
                                                     &S->right_val,
                                                      playback_ratio,
                                                      0,
                                                      0,
                                                      op | M_COPY16_NOVOL
#ifdef IS_X86
                                                     ,dig->use_MMX
#endif           
                                                      );

          out_pos = (S32) AIL_ptr_dif(dest, dest_mono_sample_buffer);
          }
         else
#endif
         {
         // --------------------------------------------------------------------------------
         // "Full" path: internal reverb engine and/or outboard pipeline filters are 
         // potentially enabled
         // --------------------------------------------------------------------------------

         //
         // First, call mixer to resample data if we need to resample or format convert,
         //   and then filter it if necessary
         //

         S32 orig_out_pos = out_pos;

         S32 mix_bytes;
         void FAR *mixer_output;

         if ( ( ( ( op & ( M_DEST_STEREO | M_SRC_STEREO ) ) == ( M_DEST_STEREO | M_SRC_STEREO ) ) ||
                ( ( op & ( M_DEST_STEREO | M_SRC_STEREO ) ) == 0 ) ) &&
             ( op & M_SRC_16 ) && ( ! ( op & (M_RESAMPLE | M_ORDER) ) ) )
         {
           mixer_output = (void FAR *) src;

           mix_bytes = (S32) AIL_ptr_dif( src_end, src );
           if ( mix_bytes > needed_dest )
           {
             mix_bytes = needed_dest;
             src = AIL_ptr_add( src, needed_dest );
           }
           else
           {
             src = src_end;
           }
         }
         else
         {
#ifdef MSS_FLT_SUPPORTED
           void FAR *dest     = AIL_ptr_add( dig->build[FLT_A].buffer, out_pos );
#else
           void FAR *dest     = AIL_ptr_add( dig->build[0].buffer, out_pos );
#endif
           void FAR *dest_end = AIL_ptr_add( dest, needed_dest);

           mixer_output = dest;

           S->pipeline[SP_MERGE].TYPE.MSS_mixer_merge(&src,
                                                      &S->src_fract,
                                                       src_end,
                         (S32 FAR * FAR *)(void FAR*) &dest,
                                                       dest_end,
                                                      &S->left_val,
                                                      &S->right_val,
                                                       playback_ratio,
                                                       0,
                                                       0,
                                                       op | M_COPY16_NOVOL
#ifdef IS_X86
                                                      ,dig->use_MMX
#endif           
                                                       );
        
           mix_bytes = (S32) AIL_ptr_dif(dest, mixer_output);
         }

         //
         // Did we force a downmix from stereo to mono for 3D positioning?  If so, 
         // convert the output data back to stereo by replicating the monaural
         // dest samples
         //

         if (need_downmix)
            {
            S16 *S = &((S16 *) mixer_output)[mix_bytes/2];
            S16 *D = &((S16 *) mixer_output)[mix_bytes]; 
            mix_bytes *= 2;

            while (D > mixer_output)
               {
               S16 val = *--S;

               *--D = val;
               *--D = val;
               }

            op |= M_DEST_STEREO;
            }

         //
         // Advance output position by number of bytes initially written by mixer to dest
         //
         
         out_pos += mix_bytes;

         //
         // Next, apply filtering on platforms that support it
         //

         if (S->lp.on)
            {
            S32 n_samples = mix_bytes / sizeof(S16);
            S32 is_stereo = (op & M_DEST_STEREO);

            if (is_stereo)
               {
               n_samples >>= 1;
               }

            S16 FAR *d = (S16 FAR *) ((dest_mono_sample_buffer != NULL) && (S->n_active_filters == 0)) ? 
               (S16 FAR *) AIL_ptr_add( dest_mono_sample_buffer, orig_out_pos ) 
                  : 
               (S16 FAR *) AIL_ptr_add( dig->build[FLT_A].buffer, orig_out_pos );

            S16 const FAR *s = (S16 const FAR *) mixer_output;

            mixer_output = d;

            LOWPASS_INFO lp = S->lp;

            if (is_stereo)
               {
               for (S32 dwIndex = 0; dwIndex < n_samples; dwIndex++)
                  {
                  register S32 in_s, out_s;

                  in_s = ( (S32) (S16) LE_SWAP16( s ) ) * lp.c.A;

                  out_s = ( in_s + lp.u.XL0 + lp.u.XL0 + lp.u.XL1
                            - lp.c.B0 * lp.u.YL0
                            - lp.c.B1 * lp.u.YL1 ) >> 12;
                  
                  lp.u.XL1 = lp.u.XL0;
                  lp.u.XL0 = in_s;
                  lp.u.YL1 = lp.u.YL0;
                  lp.u.YL0 = out_s;
               
                  if ( out_s < -32768 )
                    out_s = -32768;
                  else if ( out_s > 32767 )
                    out_s = 32767;

                  STORE_LE_SWAP16( d, (S16) out_s );
                  
                  in_s = ( (S32) (S16) LE_SWAP16_OFS( s, 2 ) ) * lp.c.A;
                  
                  out_s = ( in_s + lp.u.XR0 + lp.u.XR0 + lp.u.XR1
                            - lp.c.B0 * lp.u.YR0
                            - lp.c.B1 * lp.u.YR1 ) >> 12;
                  
                  lp.u.XR1 = lp.u.XR0;
                  lp.u.XR0 = in_s;
                  lp.u.YR1 = lp.u.YR0;
                  lp.u.YR0 = out_s;
                  
                  if ( out_s < -32768 )
                    out_s = -32768;
                  else if ( out_s > 32767 )
                    out_s = 32767;

                  STORE_LE_SWAP16_OFS( d, (S16) out_s, 2 );

                  s += 2;
                  d += 2;
                  } 
               }
            else
               {
               for (S32 dwIndex = 0; dwIndex < n_samples; dwIndex++)
                  {
                  register S32 in_s, out_s;
                  
                  in_s = ( (S32) (S16) LE_SWAP16(s) ) * lp.c.A;
                  
                  out_s = ( in_s + lp.u.XL0 + lp.u.XL0 + lp.u.XL1
                            - lp.c.B0 * lp.u.YL0
                            - lp.c.B1 * lp.u.YL1 ) >> 12;
                  
                  lp.u.XL1 = lp.u.XL0;
                  lp.u.XL0 = in_s;
                  lp.u.YL1 = lp.u.YL0;
                  lp.u.YL0 = out_s;
                  
                  if ( out_s < -32768 )
                    out_s = -32768;
                  else if ( out_s > 32767 )
                    out_s = 32767;

                  STORE_LE_SWAP16( d, (S16) out_s );

                  ++s;
                  ++d;
                  }
               }

            S->lp.u = lp.u;
            }

#ifdef MSS_FLT_SUPPORTED
         if (S->n_active_filters > 0)
            {
            mixer_output = SS_execute_filter_chain( S,
                                                    mixer_output,
                                                    AIL_ptr_add( dig->build[FLT_A].buffer, orig_out_pos ),
                                                    mix_bytes,
                                                    op & M_DEST_STEREO,
                                                    (dest_mono_sample_buffer == NULL) ? NULL : (S16 FAR *) AIL_ptr_add( dest_mono_sample_buffer, orig_out_pos));
            }
#endif        

         if (dest_mono_sample_buffer == NULL)
            {
            //
            // Perform level scaling and copy the data to the reverb/build buffers
            //                      

            MC_mix(S,
                   (S16 FAR*) mixer_output,           // src
                   mix_bytes,                         // src_bytes
                   dig->build,                        // dest_buffers
                   dig->n_build_buffers,              // n_dest_buffers
                   reverb_dest,                       // reverb_dest
                   reverb_channel_scale,              // reverb_channel_scale
                   orig_out_pos,                      // dest offset
                   op & M_DEST_STEREO );
            }
         }
      }

   //
   // Update source sample position based on amount actually consumed by mixer, if the
   // sample is uncompressed
   //

   if (!using_process)
      {
      S->pos[ S->current_buffer ] += (S32) AIL_ptr_dif(src, mixer_input);
      }
   else
      {
      //
      // We are using the process buffer -- either we used all of it, or
      // we didn't use the very last sample.  if we didn't use the last
      // one, then save it (since the process_buffer is only on the stack)
      //

      if (src < src_end)
         {
         // we don't store these specifically little endian because we are
         //   just storing exactly what we read earlier (that is, on big endian
         //   machines, last_decomp_left/right are byte swapped in the value)   

         if ( S->format & DIG_F_STEREO_MASK )
            {
              S->last_decomp_left  =  ( (S16 FAR*) src )[ 0 ];
              S->last_decomp_right  =  ( (S16 FAR*) src )[ 1 ];
            }
         else
            {
            S->last_decomp_left  =  ( (S16 FAR*) src )[ 0 ];
            S->last_decomp_right =  S->last_decomp_left;
            }
         }
      else
         {
         S->last_decomp_left = 65536;
         }
      }

  //
  // Return updated build buffer output offset
  //

  return( out_pos << out_pos_shift );
}


//############################################################################
//##                                                                        ##
//## Copy mixer buffer to DMA buffer                                        ##
//##                                                                        ##
//############################################################################

void AILCALL SS_copy (HDIGDRIVER dig, void FAR *lpWaveAddr)
{
#ifdef IS_32
   //
   // Add reverb buffer contents, if any, to build buffers
   //

   if ( dig->reverb_on )
      {
      do_reverb( dig );
      }

#endif

#ifdef MSS_FLT_SUPPORTED

   //
   // Invoke installable filters
   //
   // If any filter's postmix processor performed the buffer copy, we're done
   //

   if (FLT_call_postmix_processors(dig, lpWaveAddr))
      {
      return;
      }

#endif


#ifdef IS_PS3
    if (dig->logical_channels_per_sample <= dig->physical_channels_per_sample) 
      {
      dig->pipeline[DP_MC_COPY].TYPE.MSS_mixer_mc_copy( dig->build,
                                                        dig->n_build_buffers,
                                                        lpWaveAddr,
                                                        dig->hw_format,
#ifdef IS_X86
                                                        dig->use_MMX,
#endif
                                                        dig->samples_per_buffer,
                                                        dig->physical_channels_per_sample );
      }
#else

#ifdef IS_DOS
   //
   // If DIG_DUAL_DMA or DIG_PCM_SPLIT modes are in use for legacy cards
   // like the RAP-10 and GUS, we must use the old AILSSA_DMA_copy routine
   // instead of the new RIB-based provider.  These modes will not be
   // supported in future RIB providers.
   //
   // Note that this means these cards won't sound right with a matrix processor 
   // that doesn't support DIG_PCM_SPLIT and DIG_DUAL_DMA
   //
 
   if (dig->hw_mode_flags & (DIG_PCM_SPLIT | DIG_DUAL_DMA))
      {
      AILSSA_DMA_copy( ( lpWaveAddr == dig->DMA[ 0 ] ) ? 0 : 1,
                      &dig->DMA[0],
                       dig->hw_mode_flags,
                       dig->hw_format,
                       dig->n_active_samples,
                       dig->build[0].buffer,
                       dig->samples_per_buffer,
                       dig->hardware_buffer_size);
      }
   else
#endif
   {
#ifndef IS_WII    
   //
   // If discrete multichannel or matrix build buffers in use, call separate version
   //

   if (dig->logical_channels_per_sample > 2)
      {
      if (dig->logical_channels_per_sample <= dig->physical_channels_per_sample) 
         {
          dig->pipeline[DP_MC_COPY].TYPE.MSS_mixer_mc_copy( dig->build,
                                                            dig->n_build_buffers,
                                                            lpWaveAddr,
                                                            dig->hw_format,
#ifdef IS_X86
                                                            dig->use_MMX,
#endif
                                                            dig->samples_per_buffer,
                                                            dig->physical_channels_per_sample );
         }
      else
         {
         //
         // We shouldn't get here -- a matrix processor should have returned 1 from 
         // FLT_call_postmix_processors() above
         //
         }
      }
   else
#endif
      {
      //
      // Copy 1- or 2-channel build buffer contents directly to output buffer
      //
      // Multichannel applications will use the MC version of SS_copy() instead, so we only need
      // to support the first build buffer here.
      //

      dig->pipeline[DP_COPY].TYPE.MSS_mixer_copy(dig->build[0].buffer, 
                                                 dig->build[0].bytes,
                                                 lpWaveAddr,
                                                 dig->hw_format
#ifdef IS_X86
                                                ,dig->use_MMX
#else
#ifdef IS_PS2
                                                ,dig->IOP_overwrite
#else
#ifdef IS_BE
                                                ,1      // Use big-endian output
#endif
#endif
#endif
                                                     );
      }
   }
#endif
}

#endif   // !XBOX

//############################################################################
//##                                                                        ##
//## Copy data from source sample to build buffer, with mixing and ASI      ##
//## decompression                                                          ##
//##                                                                        ##
//############################################################################

void AILCALL SS_stream_and_mix(HSAMPLE S)
{
#ifdef IS_XBOX
  S32 dest_buffer_size = S->ds_stage_size;
#else
  S32 dest_buffer_size = S->driver->build_size;
#endif

   SS_stream_to_buffer(S,
                       NULL,
                       dest_buffer_size);
}

//############################################################################
//##                                                                        ##
//## Copy data from source sample to specified target buffer with ASI       ##
//## decompression                                                          ##
//##                                                                        ##
//############################################################################

S32 AILCALLBACK SS_stream_to_buffer(HSAMPLE  S, //)
                                    S16 FAR *dest_mono_sample_buffer,
                                    S32      dest_buffer_size)
{
  S32 out_pos;
  S32 in_len;
  S32 next_buffer;

  S->dosob=0;
  S->doeob=0;
  S->doeos=0;

  //
  // Bail if source stream ended in previous call
  //

  if (S->driver->sample_status[S->index] != SMP_PLAYING)
  {
    return 0;
  }

  //
  // In 3D mode, the channel levels are controlled by 3D positioning logic
  // This may affect channel levels (but not volume/pan!), playback rate, reverb/filtering, 
  // etc.
  //
  // We don't do level control here if a voice filter is in use, or if Xbox is in use (done in DS_serve
  // so that hardware voices will be positioned in software mode)
  //

   for (S32 i=0; i < S->driver->logical_channels_per_sample; i++)
      {
      S->auto_3D_channel_levels[i] = 1.0F;
      }

#ifndef IS_XBOX
   if (S->driver->logical_channels_per_sample > 0)
      {
      if (S->is_3D)
         {
         SS_calculate_3D_channel_levels(&S->driver->D3D,
                                         S->driver->logical_channels_per_sample,
                                        &S->S3D,
                                         S->auto_3D_channel_levels); 
         }
      }
#endif

  //
  // Init output buffer position to beginning of build buffer
  //            

  out_pos = 0;

  //
  // Merge source data into output buffer until source exhausted or output full
  //
  // Loop can be exited under the following conditions:
  //
  // 1) Output buffer full (normal condition)
  //
  // 2) Source sample ended (normal condition)
  //
  // 3) Source stream "starved" (abnormal condition)
  //

  while (out_pos < dest_buffer_size)
  {
    //
    // Set input len = size of source block to merge, based on
    // size and playback position of source sample
    //

    in_len = S->len[S->current_buffer] - S->pos[S->current_buffer];

    //
    // Initial block may terminate before end of source buffer, if loop
    // endpoint has been declared
    //

    if (S->loop_count != 1)
    {
      if ((S->loop_end != -1) && ((U32) S->loop_end < S->len[S->current_buffer]))
      {
        in_len = S->loop_end - S->pos[S->current_buffer];
      }
    }

    //
    // If no input data left, check for buffer switching and loop iteration
    //
    // (Note: in_len may fall below 0 if source data is undersampled!)
    //

    if (in_len <= 0)
    {
      //
      // Set end-of-buffer function, if any, to signal end of loop block
      // or buffer
      //
      // Note that this function will be called repeatedly if buffer
      // starvation occurs
      //

      if (S->EOB != NULL)
        ++S->doeob;

      // reset the ASI, if requested
      if (S->reset_ASI[S->current_buffer])
      {
        // if requested, do a reset now that the ASI decoder has the data
        if ( S->pipeline[SP_ASI_DECODER].active )
        {
          ASISTAGE *ASI = &S->pipeline[SP_ASI_DECODER].TYPE.ASI;
          ASI->ASI_stream_seek(ASI->stream, -2);

          if (ASI->STREAM_SEEK_POS != (UINTa) -1)
            {
            ASI->ASI_stream_property(ASI->stream,
                                     ASI->STREAM_SEEK_POS,
                                     0,&S->reset_seek_pos[ S->current_buffer ], &S->reset_seek_pos[ S->current_buffer ]);
            }

        }
        S->reset_ASI[S->current_buffer]=0;
      }

      //
      // If looping active, go back to beginning of loop to fetch next
      // source data block
      //

      if (S->loop_count != 1)
      {
        //
        // Reset source sample position to beginning of loop
        //

        S->pos[S->current_buffer] = S->loop_start;

        //
        // Reset ADPCM offset to the end of decode buffer
        // to force a decode buffer refill
        //

        if (S->format & DIG_F_ADPCM_MASK)
        {
          S->adpcm.blockleft = 0;
          S->adpcm.extrasamples = 0;
        }

        //
        // Reset ASI state, if applicable
        //

        if (S->pipeline[SP_ASI_DECODER].active)
        {
          ASISTAGE *ASI = &S->pipeline[SP_ASI_DECODER].TYPE.ASI;
          ASI->ASI_stream_seek(ASI->stream, -2);

          if ((S->service_type == 1) && (ASI->STREAM_SEEK_POS != (UINTa) -1))
            {
            ASI->ASI_stream_property(ASI->stream,
                                     ASI->STREAM_SEEK_POS,
                                     0,&S->pos[ S->current_buffer ], &S->pos[ S->current_buffer ]);
            }
        }

        //
        // Decrement loop count if not infinite
        //

        if (S->loop_count != 0)
        {
          --S->loop_count;
        }

        //
        // Recalculate size and address of source block
        //

        continue;
      }


      //
      // If streaming sample and alternate buffer available,
      // set up to fetch next source data block from it
      //
      // Typically, buffer-switching takes place in the ASI fetch
      // callback function if an ASI decoder is in use.  We must
      // still be able to switch here, though, in case the callback
      // function returns at the end of its current buffer.
      //

      if (S->service_type == 2)
      {

        next_buffer = S->current_buffer ^ 1;

        //
        // If explicit 0-length buffer was posted, fall through to allow
        // the sample to terminate
        //

        if (!S->done[next_buffer])
        {
          //
          // If alternate buffer not yet initialized, break out of loop
          // to allow application to supply data as soon as it can
          //

          if (!S->len[next_buffer])
          {
            goto starved;
          }

          //
          // If alternate buffer already played, break out of loop to
          // allow application to refresh it as soon as possible
          //

          if (S->pos[next_buffer])
          {
           starved:
            S->starved=1;

#ifdef IS_XBOX
            // once we starve, we fill the rest of this buffer with silence
            //   and then we need to clear the next two as well
            if ( out_pos == 0 )
              ++S->cleared_bufs;

            if ( S->cleared_bufs <= 2 )
            {
            S32 backfill_bytes = dest_buffer_size - out_pos;

            S->cleared_bytes += backfill_bytes;

              // fill rest to zero
              AIL_memset( AIL_ptr_add( S->ds_staging_buffer->data,
                                      ( dest_buffer_size & -S->waiting_for_which_half ) +
                                      out_pos ),
                          ( S->format & (DIG_F_16BITS_MASK | DIG_F_XBOX_ADPCM_MASK | DIG_F_ADPCM_MASK) ) ? 0 : 0x80,
                          backfill_bytes );
            }
#endif
            goto docbs;
          }

          //
          // Otherwise, alternate buffer is ready to play -- switch to
          // it and keep filling output buffer
          //

          S->current_buffer = next_buffer;
          continue;
        }
      }

      //
      // If new input data is still not available after looping
      // and/or switching buffers, terminate the sample
      //

#ifdef IS_XBOX
      // once we're done, we fill the rest of this buffer with silence
      //   and then we need to clear the next two as well
      if ( out_pos == 0 )
        ++S->cleared_bufs;

      S32 backfill_bytes = dest_buffer_size - out_pos;

      S->cleared_bytes += backfill_bytes;

      // fill rest to zero
      AIL_memset( AIL_ptr_add( S->ds_staging_buffer->data,
                              ( dest_buffer_size & -S->waiting_for_which_half ) +
                              out_pos ),
                  ( S->format & (DIG_F_16BITS_MASK | DIG_F_XBOX_ADPCM_MASK | DIG_F_ADPCM_MASK) ) ? 0 : 0x80,
                  backfill_bytes );

      if ( S->cleared_bufs < 2 )
        goto docbs;
#endif

      S->driver->sample_status[S->index] = SMP_DONE;
      S->starved = 1;

      if (S->EOS != NULL)
         ++S->doeos;

      goto docbs;
    }

    //
    // Set to perform start-of-block callback
    //

    if (S->SOB != NULL)
       ++S->dosob;

    //
    // Merge source block with contents of output buffer, updating output
    // buffer position counter
    //
    // Note that buffer switching may also take place within this routine if
    // an ASI decoder is in use!
    //

    out_pos = SS_merge(S,                       // sample handle
                       in_len,                  // max len of source block to copy
                       out_pos,                 // dest position in build buffer 
                       dest_mono_sample_buffer, // optional output-filter buffer
                       dest_buffer_size);       // total size of output build buffer

  }

docbs:

  //
  // make the callbacks after everything's been merged
  //

  while (S->dosob--)
  {
    MSS_do_cb1( (AILSAMPLECB) ,S->SOB,S->driver->callingDS,
    S);
  }

  while (S->doeob--)
  {
    MSS_do_cb1( (AILSAMPLECB) ,S->EOB,S->driver->callingDS,
    S);
  }

  while (S->doeos--)
  {
    MSS_do_cb1( (AILSAMPLECB) ,S->EOS,S->driver->callingDS,
    S);
  }

  return out_pos;
}

#ifdef IS_DOS

//############################################################################
//##                                                                        ##
//## End of locked code                                                     ##
//##                                                                        ##
//############################################################################

void AILSFILE_end(void)
{
   if (locked)
      {
      AIL_vmm_unlock_range(AILSFILE_start, AILSFILE_end);

      locked = 0;
      }
}

#endif


