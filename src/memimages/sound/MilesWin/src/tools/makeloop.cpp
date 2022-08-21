//############################################################################
//##                                                                        ##
//##  MAKELOOP.CPP                                                          ##
//##                                                                        ##
//##  Command-line utility to generate seamlessly-loopable MP3 and ADPCM    ##
//##  files                                                                 ##
//##                                                                        ##
//##  V1.00 of 23-Oct-06: Initial                                           ##
//##                                                                        ##
//##  Author: John Miles                                                    ##
//##                                                                        ##
//############################################################################
//##                                                                        ##
//##  Copyright (C) RAD Game Tools, Inc.                                    ##
//##                                                                        ##
//##  Contact RAD Game Tools at 425-893-4300 for technical support.         ##
//##                                                                        ##
//############################################################################

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <assert.h>
#include <malloc.h>
#include <dos.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "mss.h"

#define ENCDELAY 576    // Hardwired MP3 encoder delay matches the ENCDELAY equate in LAME's encoder.h

typedef enum OUTPUT_FMT
{
   F_NONE,
   F_MP3,
   F_ADPCM,
};

#ifdef IS_BE

  #define LE_SWAP32(ptr) \
                   ( ( ( ( *((U32 FAR *)(ptr) ) ) << 24 ) ) | \
                     ( ( ( *((U32 FAR *)(ptr) ) ) <<  8 ) & 0x00FF0000 ) | \
                     ( ( ( *((U32 FAR *)(ptr) ) ) >>  8 ) & 0x0000FF00 ) | \
                     ( ( ( *((U32 FAR *)(ptr) ) ) >> 24 ) ) )
  #define MEM_LE_SWAP32(n) *((U32*)n) = LE_SWAP32(n);

#else

  #define LE_SWAP32( ptr ) ( *((U32 FAR *)(ptr) ) )
  #define MEM_LE_SWAP32(n)

#endif

typedef MSS_STRUCT _ADPCMOUT 
{
  U32 riffmark;
  U32 rifflen;
  U32 wavemark;
  U32 fmtmark;
  U32 fmtlen;
  U16 fmttag;
  U16 channels;
  U32 sampersec;
  U32 avepersec;
  U16 blockalign;
  U16 bitspersam;
  S16 extra;
  S16 samples_per_block;
  U32 factmark;
  U32 factlen;
  U32 samples;
  U32 datamark;
  U32 datalen;
} 
ADPCMOUT;

//
// Win32Exec: Create process and return immediately, without doing a
//            WaitForInputIdle() call like WinExec()                
//                                                                  
// Optionally, wait for process to terminate                        
//                                                                  

BOOL Win32Exec(LPSTR lpCmdLine, BOOL bWait)
{
   STARTUPINFO         StartInfo;
   PROCESS_INFORMATION ProcessInfo;
   BOOL                result;
   
   memset(&StartInfo,   0, sizeof(StartInfo));
   memset(&ProcessInfo, 0, sizeof(ProcessInfo));

   StartInfo.cb = sizeof(StartInfo);

   result = CreateProcess(NULL,         // Image name
                          lpCmdLine,    // Command line
                          NULL,         // Process security
                          NULL,         // Thread security
                          FALSE,        // Do not inherit handles
                          0,            // Creation flags
                          NULL,         // Inherit parent environment
                          NULL,         // Keep current working directory
                         &StartInfo,    // Startup info structure
                         &ProcessInfo); // Process info structure

   if (bWait)
      {
      WaitForSingleObject(ProcessInfo.hProcess,
                          INFINITE);
      }

   return result;
}

//***************************************************************************
//
// 2D cubic spline interpolators from Wolberg, Digital Image Warping, p. 293
//
//***************************************************************************

static void tridiag_gen(F64 *A, F64 *B, F64 *C, F64 *D, S32 len)
{
   S32 i;
   F64 b, *F;

   F = (F64 *) AIL_mem_alloc_lock(len * sizeof(F64));

   b = B[0];
   D[0] = D[0] / b;

   for (i=1; i < len; i++)
      {
      F[i] = C[i-1] / b;
      b = B[i] - A[i] * F[i];

      assert(b != 0.0);

      D[i] = (D[i] - D[i-1] * A[i]) / b;
      }

   for (i=len-2; i >= 0; i--)
      {
      D[i] -= (D[i+1] * F[i+1]);
      }

   AIL_mem_free_lock(F);
}

static void getYD_gen(F64 *X, F64 *Y, F64 *YD, S32 len)
{
   S32 i;
   F64 h0, h1, r0, r1, *A, *B, *C;

   A = (F64 *) AIL_mem_alloc_lock(len * sizeof(F64));
   B = (F64 *) AIL_mem_alloc_lock(len * sizeof(F64));
   C = (F64 *) AIL_mem_alloc_lock(len * sizeof(F64));

   h0 = X[1] - X[0];
   h1 = X[2] - X[1];
   r0 = (Y[1] - Y[0]) / h0;
   r1 = (Y[2] - Y[1]) / h1;

   B[0] = h1 * (h0 + h1);
   C[0] = (h0 + h1) * (h0 + h1);
   YD[0] = r0 * (3.0 * h0 * h1 + 2.0 * h1 * h1) + r1 * h0 * h0;

   for (i=1; i < len-1; i++)
      {
      h0 = X[i] - X[i-1];
      h1 = X[i+1] - X[i];
      r0 = (Y[i] - Y[i-1]) / h0;
      r1 = (Y[i+1] - Y[i]) / h1;
      A[i] = h1;
      B[i] = 2 * (h0 + h1);
      C[i] = h0;
      YD[i] = 3.0 * (r0 * h1 + r1 * h0);
      }

   A[i] = (h0 + h1) * (h0 + h1);
   B[i] = h0 * (h0 + h1);
   YD[i] = r0 * h1 * h1 + r1 * (3.0 * h0 * h1 + 2.0 * h0 * h0);

   tridiag_gen(A, B, C, YD, len);

   AIL_mem_free_lock(A);
   AIL_mem_free_lock(B);
   AIL_mem_free_lock(C);
}

void ispline_gen(F64 *X1, F64 *Y1, S32 len1, //)
                 F64 *X2, F64 *Y2, S32 len2)
{
   S32 i,j;
   F64 *YD, A0, A1, A2, A3, x, dx, dy, p1, p2, p3;

   YD = (F64 *) AIL_mem_alloc_lock(len1 * sizeof(F64));
   getYD_gen(X1, Y1, YD, len1);

   if (X2[0] < X1[0] || X2[len2-1] > X1[len1-1])
      {
      assert(0);
      }

   p3 = X2[0] - 1;
   for (i=j=0; i < len2; i++)
      {
      p2 = X2[i];

      if (p2 > p3)
         {
         for (; j < len1 && p2 > X1[j]; j++);

         if (p2 < X1[j]) j--;

         p1 = X1[j];
         p3 = X1[j+1];

         dx = 1.0 / (X1[j+1] - X1[j]);
         dy = (Y1[j+1] - Y1[j]) * dx;

         A0 = Y1[j];
         A1 = YD[j];
         A2 = dx * (3.0 * dy - 2.0 * YD[j] - YD[j+1]);
         A3 = dx * dx * (-2.0 * dy + YD[j] + YD[j+1]);
         }

      x = p2 - p1;
      Y2[i] = ((A3 * x + A2) * x + A1) * x + A0;
      }

   AIL_mem_free_lock(YD);
}

static void tridiag(F64 *D, S32 len)
{
   S32 i;
   F64 *C;

   C = (F64 *) AIL_mem_alloc_lock(len * sizeof(F64));

   D[0] = 0.5 * D[0];
   D[1] = (D[1] - D[0]) / 2.0;
   C[1] = 2.0;

   for (i=2; i < len-1; i++)
      {
      C[i] = 1.0 / (4.0 - C[i-1]);
      D[i] = (D[i] - D[i-1]) / (4.0 - C[i]);
      }

   C[i] = 1.0 / (4.0 - C[i-1]);
   D[i] = (D[i] - 4.0*D[i-1]) / (2.0 - 4.0*C[i]);

   for (i=len-2; i >= 0; i--)
      {
      D[i] -= (D[i+1] * C[i+1]);
      }

   AIL_mem_free_lock(C);
}

static void getYD(F64 *Y, F64 *YD, S32 len)
{
   S32 i;

   YD[0] = -5.0*Y[0] + 4.0*Y[1] + Y[2];

   for (i=1; i < len-1; i++)
      {
      YD[i] = 3.0 * (Y[i+1] - Y[i-1]);
      }

   YD[len-1] = -Y[len-3] - 4.0*Y[len-2] + 5.0*Y[len-1];

   tridiag(YD,len);
}

void ispline(F64 *Y1, S32 len1, //)
             F64 *Y2, S32 len2)
{
   S32 i,oip;
   F64 *YD, A0=0, A1=0, A2=0, A3=0, x, p, fctr;

   YD = (F64 *) AIL_mem_alloc_lock(len1 * sizeof(F64));
   getYD(Y1,YD,len1);

   oip = -1;
   fctr = F64(len1-2) / (F64) len2; // Original: len1-1 / len2-1, which made ip+1 overflow in A2,A3 calculations

   for (i=0,p=0.5F; i < len2; i++)  // Original: p=0.0, which is bad for symmetry
      {
      S32 ip = (S32) p;

      if (ip != oip)
         {
         oip = ip;

         A0 = Y1[ip];
         A1 = YD[ip];
         A2 = 3.0 * (Y1[ip+1] - Y1[ip]) - 2.0 * YD[ip] - YD[ip+1];
         A3 = -2.0 * (Y1[ip+1] - Y1[ip]) + YD[ip] + YD[ip+1];
         }

      x = p - ip;
      Y2[i] = ((A3 * x + A2) * x + A1) * x + A0;

      p += fctr;
      }

   AIL_mem_free_lock(YD);
}

//************************************************************************************************
//
// main()
//
//************************************************************************************************

void shutdown(void)
{
   AIL_shutdown();
}

int MSS_MAIN_DEF main( int argc, char *argv[] )
{
   printf("MAKELOOP - Version " MSS_VERSION "          " MSS_COPYRIGHT " \n");
   printf("-------------------------------------------------------------------------------\n\n");
  
   printf("This program creates loopable .MP3 and IMA ADPCM .WAV files by decimating the\n"
          "input .WAV or .MP3 data to an integral number of frames and applying any \n"
          "required overlap-window precompensation.\n\n"
          "To create .MP3 files, the LAME command-line encoder must be accessible in the \n"
          "current path.\n\n");

   //
   // Parse args
   //

   C8        *input_filename    = NULL;
   C8        *output_filename   = NULL;
   C8        *lame_args         = NULL;
   F32        silence_threshold = 0.0F;
   S32        bitrate_kbps      = 128;
   S32        sample_adjust     = LONG_MAX;
   S32        update_rate       = FALSE;
   S32        error             = 0;

   for (S32 i=1; i < argc; i++)
      {
      C8 arg[MAX_PATH];
      strcpy(arg,argv[i]);
      _strlwr(arg);  

      if (arg[0] == '-') arg[0] = '/';

           if (!strncmp(arg, "/s", 2)) sscanf(arg,"/s%f",&silence_threshold);  
      else if (!strncmp(arg, "/b", 2)) sscanf(arg,"/b%d",&bitrate_kbps);
      else if (!strncmp(arg, "/a", 2)) sscanf(arg,"/a%d",&sample_adjust);
      else if (!strncmp(arg, "/l", 2)) lame_args = &argv[i][2];
      else if (!strncmp(arg, "/r", 2)) update_rate = TRUE;
      else if (arg[0] != '/')
         {
              if (input_filename  == NULL) input_filename = argv[i];
         else if (output_filename == NULL) output_filename = argv[i];
         else error = 1;
         }
      else
         {
         error = 1;
         }
      }

   if ((error) || (output_filename == NULL))
      {
      printf(
      "Usage: makeloop input_filename output_filename [/snn] [/bnnn] [/annn] [/r]\n"
      "                [/l\"...\"]\n\n"
      "where:       input_filename is a .WAV (recommended) or .MP3 input file\n\n"
      "             output_filename is the target .MP3 or ADPCM .WAV filename\n\n"
      "             /snn  is an optional silence-truncation threshold in dBFS\n"
      "                   (example: /s48 truncates all data <= -48 dB from full\n"
      "                   scale at beginning and end of input file)\n\n"
      "             /bnnn is the bit rate in kbps for MP3 encoding (default = 128)\n\n"
      "             /annn adjusts (by subtraction) the precise number of samples \n"
      "                   encoded (default = 1 for MP3, 0 for ADPCM output)\n\n"
      "             /r    causes MAKELOOP to update its ADPCM .WAV output file with\n"
      "                   the exact sample rate resulting from decimation.  (For\n"
      "                   optimum runtime performance and compatibility, the output\n"
      "                   file normally inherits the input file's sample rate, \n"
      "                   resulting in a slight pitch shift.)\n\n"
      "             /l\"...\" is an optional set of switches for LAME, enclosed in \n"
      "                   double quotes, that will be used instead of MAKELOOP's\n"
      "                   default argument set.\n");
      exit(1);
      }

   OUTPUT_FMT dest_format = F_NONE;

   S32 ol = (S32) strlen(output_filename);

   if (ol >= 3)
      {
      C8 *suffix = &output_filename[ol-3];

      if (!_stricmp(suffix,"MP3")) 
         {
         dest_format = F_MP3;
         if (sample_adjust == LONG_MAX) sample_adjust = 1;
         }
      else if (!_stricmp(suffix,"WAV")) 
         {
         dest_format = F_ADPCM;
         if (sample_adjust == LONG_MAX) sample_adjust = 0;
         }
      }

   if (dest_format == F_NONE)
      {
      printf("Error: Output file format must be .WAV (ADPCM) or .MP3\n");
      exit(1);
      }

   //
   // Set the redist directory and start MSS
   //
 
   AIL_set_redist_directory( MSS_DIR_UP_TWO MSS_REDIST_DIR_NAME );
 
   AIL_startup();
   atexit(shutdown);
 
   //
   // Initialize digital sound system for functions that need an HDIGDRIVER
   //
 
   HDIGDRIVER dig = AIL_open_digital_driver( 44100, 16, 2, 0);
 
   if (dig == NULL)
      {
      printf("%s\n",AIL_last_error());
      exit(1);
      }
 
   //
   // Load file and convert to PCM .WAV image if it isn't already one
   //
 
   printf( "Loading %s\n", input_filename);
 
   U32 *ptr = (U32 *) AIL_file_read(input_filename, FILE_READ_WITH_SIZE );
 
   if (ptr == NULL)
      {
      printf("File '%s' not found\n", input_filename);
      exit(1);
      }
 
   void *allocated_WAV_ptr = ptr;

   S32 type = AIL_file_type_named(ptr+1,input_filename,ptr[0]);
 
   void *WAV = NULL;
   U32   WAV_bytes = 0;

   switch (type) 
      {
      default:
         {
         printf("Unsupported input file type (%d)\n",type);
         exit(1);
         }

      case AILFILETYPE_PCM_WAV:
         {
         printf("Processing PCM .WAV input...\n");

         WAV       = (void *) &ptr[1];
         WAV_bytes = ptr[0];
         break;
         }

      case AILFILETYPE_ADPCM_WAV:
         {
         AILSOUNDINFO info;
         AIL_WAV_info(ptr+1, &info);

         printf("ADPCM channels = %d, block size = %d\n",info.channels,info.block_size);

         S32 result = AIL_decompress_ADPCM(&info, 
                                           &WAV, 
                                           &WAV_bytes);

         if (!result)
            {
            printf("ADPCM decompression failed: %s\n",AIL_last_error());
       
            exit(1);
            }

         AIL_mem_free_lock(ptr);
         allocated_WAV_ptr = WAV;
         break;
         }

      case AILFILETYPE_MPEG_L3_AUDIO:
         {
         printf("WARNING: Recompressing .MP3 data can cause audible loss.  PCM input recommended instead.\nDecompressing...\n");

         S32 result = AIL_decompress_ASI(&ptr[1],     // TODO: returns garbage with asynja.mp3? 
                                          ptr[0],
                                         ".mp3",
                                         &WAV,
                                         &WAV_bytes,
                                          NULL);

         if (!result)      
            {
            printf(".MP3 decompression failed: %s\n",AIL_last_error());
            exit(1);
            }
         
         AIL_mem_free_lock(ptr);
         allocated_WAV_ptr = WAV;
         break;
         }
      }

   //
   // Extract raw PCM pointer from .WAV header
   //

   AILSOUNDINFO info;
   AIL_WAV_info(WAV, &info);
   
   info.samples /= info.channels;      // We consider one "sample" to be a mono or stereo sample

   if ((info.bits != 16) || (info.format != WAVE_FORMAT_PCM))
      {
      printf("Error: 16-bit data required\n");
      exit(1);
      }

   printf("Processing 16-bit PCM data: %d bytes, %d channel(s), %d total samples, %d Hz\n",
      info.data_len,
      info.channels,
      info.samples,
      info.rate);

   if (info.data_len != (info.samples * info.channels * sizeof(S16)))
      {
      printf("Error: corrupt or unsupported .WAV image\n");
      exit(1);
      }
   
   //
   // Target sample modulus is based on the sample rate for both ADPCM and MP3 formats
   //

   S32 samples_per_frame     = 0;
   S32 ADPCM_bytes_per_frame = 0;

   switch (dest_format)
      {
      case F_ADPCM:
         {
         ADPCM_bytes_per_frame = 256 << (info.channels / 2);

         if (info.rate > 11025)
            {
            ADPCM_bytes_per_frame *= info.rate / 11025;  // e.g., 512 bytes for 22050 Hz mono
            }

         //
         // One (mono or stereo) PCM sample/predictor DWORD exists at the beginning of 
         // each IMA/DVI ADPCM frame, followed by as many remaining ADPCM nibbles
         // as will fit in ADPCM_bytes_per_frame-sizeof(PCM_sample) bytes.  This will always 
         // result in an odd number of samples per block
         //

         S32 reserved_bytes = 4 << (info.channels / 2);
         samples_per_frame = 1 + ((ADPCM_bytes_per_frame-reserved_bytes) * 8 / reserved_bytes);

         printf("Target = %d samples per ADPCM block\n",samples_per_frame);
         break;
         }

      case F_MP3:
         {
         if ((info.rate == 44100) || 
             (info.rate == 48000) || 
             (info.rate == 32000))
            {
            samples_per_frame = 1152;           // MPEG1
            }
         else if ((info.rate == 22050) ||
                  (info.rate == 24000) ||
                  (info.rate == 16000))
            {
            samples_per_frame = 576;            // MPEG2
            }
         else if ((info.rate == 11025) ||
                  (info.rate == 12000) ||
                  (info.rate == 8000))
            {
            samples_per_frame = 576;            // MPEG2.5
            }
      
         if (samples_per_frame != 1152)
            {
            //
            // For now, we generate only MPEG1 data (TODO: test at other rates)
            //
      
            printf("Error: Unsupported sample rate -- must be 44.1 kHz, 48 kHz, or 32 kHz\n");
            exit(1);
            }
         break;
         }
      }

   if (info.samples < (U32) samples_per_frame)
      {
      printf("Error: file too small to process (< %d samples)\n",samples_per_frame);
      exit(1);
      }

   //
   // Apply optional silence clipping
   //

   if (silence_threshold != 0.0F)
      {
      S32 val = S32(32768.0 / pow(10.0, silence_threshold / 20.0));

      printf("Truncating silent PCM values below %.03f dBfs [-%d,%d]... \n",silence_threshold,val,val);
   
      S32 first_audible_sample = -1;
      S32 last_audible_sample  = 0;

      for (S32 s=0; s < (S32) info.samples; s++)
         {
         for (S32 c=0; c < info.channels; c++)
            {
            S16 *S = &((S16 *) info.data_ptr)[c];

            S32 sv = (S32) S[s * info.channels];

            S32 silent = ((sv < val) && (sv > -val));

            if (!silent) 
               {
               last_audible_sample = s;

               if (first_audible_sample == -1)
                  {
                  first_audible_sample = s;
                  }
               }
            }
         }

      if ((first_audible_sample == -1) || 
         ((last_audible_sample - first_audible_sample) < samples_per_frame))
         {
         printf("ERROR: Insufficient audible data in source file\n");
         exit(1);
         }

      S32 beg_trunc   = first_audible_sample;
      S32 end_trunc   = info.samples - last_audible_sample;
      S32 total_trunc = beg_trunc + end_trunc;

      printf("%d silent sample(s) removed from beginning of file, %d removed from end\n",
         beg_trunc,
         end_trunc);

      info.data_ptr  = &((U8 *) info.data_ptr)[first_audible_sample * sizeof(S16) * info.channels];
      info.data_len -= (total_trunc * sizeof(S16) * info.channels);
      info.samples  -=  total_trunc;
      }

   //
   // See if resampling is needed
   //
   // Note that we encode one less than the target # of samples by default, for MP3 data.  
   // Either the resampler sounds a bit better with an odd number of samples, or LAME does... 
   // it's not clear which
   //

   S32 target_frames  = info.samples / samples_per_frame;
   S32 target_samples = target_frames * samples_per_frame;

   target_samples -= sample_adjust;

   S16 *src = (S16 *) info.data_ptr;
   S32 src_bytes  = info.data_len;

   S32 dest_rate = 0;

   if (target_samples == (S32) info.samples)
      {
      printf("(Source data is already a multiple of %d samples -- no resampling needed)\n",samples_per_frame);
      dest_rate = info.rate;
      }
   else
      {
      printf("Decimating input to occupy %d frames (%d samples)...\n",target_frames,target_samples);
      
      S32  dest_bytes = target_samples * info.channels * sizeof(S16);
      S16 *dest       = (S16 *) AIL_mem_alloc_lock(dest_bytes);

      if (dest == NULL)
         {
         printf("Error: out of memory\n");
         exit(1);
         }
   
      F64 frac_rate = F64(dest_bytes) * F64(info.rate) / F64(src_bytes);

      printf("Resampling %d samples/sec to %.02lf samples/sec\n",
         info.rate,
         frac_rate);

      dest_rate = S32(frac_rate + 0.5F);

      //
      // Use cubic spline to perform decimation and smooth the resulting curve
      //

      F64 *SS = (F64 *) AIL_mem_alloc_lock(info.samples   * sizeof(F64));
      F64 *SD = (F64 *) AIL_mem_alloc_lock(target_samples * sizeof(F64));

      for (S32 c=0; c < info.channels; c++)
         {
         S32 s;
         S16 *S = &src[c];
         S16 *D = &dest[c];

         for (s=0; s < (S32) info.samples; s++)
            {
            SS[s] = ((F64) (S32) *S) / 32768.0;
            S += info.channels;
            }

         ispline(SS, info.samples, 
                 SD, target_samples);

         for (s=0; s < target_samples; s++)
            {
            F64 V = SD[s];

                 if (V >=  1.0) *D = 32767;
            else if (V <= -1.0) *D = -32768;
            else                *D = (S16) (S32) (V * 32767.0);

            D += info.channels;
            }
         }

      AIL_mem_free_lock(SS);
      AIL_mem_free_lock(SD);

      AIL_mem_free_lock(allocated_WAV_ptr);
      WAV = NULL;

      src = dest;
      src_bytes = target_samples * info.channels * sizeof(S16);

      allocated_WAV_ptr = dest;
      }

   if (dest_format == F_ADPCM)
      {
      //
      // Prepend a copy of the final frame's worth of samples to the PCM data being encoded,
      // so that the ADPCM step initializer will be continuous at the loop point
      //

      S32 overlap_bytes = samples_per_frame * info.channels * sizeof(S16); 
      U8 *overlap_ptr   = &((U8 *) src)[src_bytes - overlap_bytes];

      S32 S_bytes = overlap_bytes + src_bytes; 
      U8 *S = (U8 *) AIL_mem_alloc_lock(S_bytes);

      memcpy(S, 
             overlap_ptr,
             overlap_bytes);

      memcpy(&S[overlap_bytes],
              src,
              src_bytes);

      //
      // Encode the file
      //

      AILSOUNDINFO I;
      I.format     = WAVE_FORMAT_PCM;
      I.data_ptr   = S;
      I.data_len   = S_bytes;
      I.rate       = update_rate ? dest_rate : info.rate;
      I.bits       = info.bits;
      I.channels   = info.channels;
      I.samples    = target_samples + samples_per_frame;
      I.block_size = 1;

      void *wav = NULL;
      U32 wavesize = 0;

      if (!AIL_compress_ADPCM(&I,
                              &wav,
                              &wavesize))
         {
         printf("ERROR: AIL_compress_ADPCM() failed (%s)\n",AIL_last_error());
         exit(1);
         }

      //
      // Adjust the .WAV image to remove the first block
      //

      ADPCMOUT *adp = (ADPCMOUT *) wav;

      adp->samples = target_samples;
      adp->datalen = ADPCM_bytes_per_frame * target_frames;
      adp->rifflen = wavesize - 8 - ADPCM_bytes_per_frame;

      memmove(((U8 *) wav) + sizeof(ADPCMOUT),
              ((U8 *) wav) + sizeof(ADPCMOUT) + ADPCM_bytes_per_frame,
              wavesize - sizeof(ADPCMOUT) - ADPCM_bytes_per_frame);

      MEM_LE_SWAP32( &adp->rifflen );
      MEM_LE_SWAP32( &adp->samples );
      MEM_LE_SWAP32( &adp->datalen );

      //
      // Save ADPCM .WAV image
      //

      FILE *target = fopen(output_filename,"w+b");

      if (target == NULL)
         {
         printf("Error: could not open target file %s",output_filename);
         exit(1);
         }

      if (fwrite(wav,
                 wavesize,
                 1,
                 target) != 1)
         {
         printf("Error: could not write to target file, disk full?");
         exit(1);
         }

      fclose(target);

      AIL_mem_free_lock(wav);
      AIL_mem_free_lock(S);
      }
   else
      {
      //
      // Prepare to save PCM frames as raw temporary file
      //

      C8 rawtemp_name[MAX_PATH] = "c:\\msstemp.raw";
      C8 path_buffer [MAX_PATH] = "";

      GetTempPath(sizeof(path_buffer)-1,
                  path_buffer);

      GetTempFileName(path_buffer,
                     "MSS",
                      0,
                      rawtemp_name);

      strcat(rawtemp_name,".raw");

      printf("Creating temporary PCM file %s...\n",rawtemp_name);

      FILE *tempfile = fopen(rawtemp_name,"w+b");

      if (tempfile == NULL)
         {
         printf("Error: could not open temporary file %s",rawtemp_name);
         exit(1);
         }

      //
      // Ensure that the first valid data frame of the file will be entirely silent,
      // by writing samples_per_frame-ENCDELAY zeroed samples
      //

      S32 silent_bytes = (samples_per_frame - ENCDELAY) * info.channels * sizeof(S16);

      S16 *silence = (S16 *) alloca(silent_bytes);
      memset(silence, 0, silent_bytes);

      if (fwrite(silence,silent_bytes,1,tempfile) != 1)
         {
         printf("Error: could not write to temporary file %s, disk full?",rawtemp_name);
         _unlink(rawtemp_name);
         exit(1);
         }

      //
      // Next, write a copy of the last frame in front of the file, creating a 
      // "predecessor" frame that will match the overlap-window contents 
      // when the file loops
      //

      S32 overlap_bytes = samples_per_frame * info.channels * sizeof(S16);
      U8 *overlap_ptr   = &((U8 *) src)[src_bytes - overlap_bytes];

      if (fwrite(overlap_ptr,overlap_bytes,1,tempfile) != 1)
         {
         printf("Error: could not write to temporary file %s, disk full?",rawtemp_name);
         _unlink(rawtemp_name);
         exit(1);
         }

      //
      // Finally, write resampled PCM file image
      //

      if (fwrite(src,src_bytes,1,tempfile) != 1)
         {
         printf("Error: could not write to temporary file %s, disk full?",rawtemp_name);
         _unlink(rawtemp_name);
         exit(1);
         }

      fclose(tempfile);

      AIL_mem_free_lock(allocated_WAV_ptr);

      //
      // Launch LAME to encode tempfile
      //

      C8 mp3temp_name[MAX_PATH] = "c:\\msstemp.mp3";

      GetTempPath(sizeof(path_buffer)-1,
                  path_buffer);

      GetTempFileName(path_buffer,
                     "MSS",
                      0,
                      mp3temp_name);

      strcat(mp3temp_name,".mp3");

      C8 lame_command[MAX_PATH*2];

      if (lame_args != NULL)
         {
         sprintf(lame_command,"lame %s %s %s",
                 lame_args,
                 rawtemp_name,
                 mp3temp_name);
         }
      else
         {
         sprintf(lame_command,"lame -x -r -s%f -b%d --cbr -T --nores -h %s %s %s",
          ((F32) info.rate) / 1000.0F,
                 bitrate_kbps,
                 (info.channels == 1) ? "-mm" : "",
                 rawtemp_name,
                 mp3temp_name);
         }

      printf("Executing \"%s\"...\n\n",lame_command);

      if (!Win32Exec(lame_command,TRUE))
         {
         printf("Error: could not launch LAME -- is it in your path?");
         _unlink(rawtemp_name);
         _unlink(mp3temp_name);
         exit(1);
         }

      printf("\nInspecting temporary MP3 file... ");

      //
      // Load MP3 tempfile and save the loopable portion
      //
      // The first frame is the Info/Xing tag (which we don't actually need), the second 
      // frame is the silent one we wrote to hide the encoder delay, and the third is the 
      // copy of the final frame we encoded to anticipate the overlap window's contents 
      // during looping.  The file's final frame is always silent (based on LAME's 
      // current behavior).  So we'll save the fourth through the penultimate frames as 
      // the final "loopable" file...
      //

      _unlink(rawtemp_name);

      U32 *file = (U32 *) AIL_file_read(mp3temp_name, FILE_READ_WITH_SIZE);

      if (file == NULL)
         {
         printf("\n%s\n",AIL_last_error());
         _unlink(mp3temp_name);
         exit(1);
         }

      _unlink(mp3temp_name);

      S32 image_bytes = (S32)   file[0];
      U8 *image       = (U8 *) &file[1];

      MP3_INFO M;

      AIL_inspect_MP3(&M,
                       image,
                       image_bytes);

      S32 *frame_offsets = (S32 *) AIL_mem_alloc_lock((target_frames+16) * sizeof(S32));

      if (frame_offsets == NULL)
         {
         printf("\nError: Out of memory\n");
         exit(1);
         }

      S32 n_frames = 0;

      while (AIL_enumerate_MP3_frames(&M))
         {
         if (n_frames >= target_frames+16)
            {
            printf("\nError: File has way too many frames\n");
            exit(1);
            }

         frame_offsets[n_frames++] = M.byte_offset;
         }

      printf("Found %d frames (%d expected)\n",n_frames,target_frames+4);

      if (n_frames != target_frames+4)
         {
         printf("WARNING: File may not loop properly due to this discrepancy!\n");
         }

      S32 start_offset = frame_offsets[3];
      S32 end_offset   = frame_offsets[n_frames-1];

      printf("Saving frames 4-%d (offsets %d-%d) to %s...\n",n_frames-1,start_offset,end_offset,output_filename);

      S32 save_bytes = end_offset - start_offset;

      FILE *target = fopen(output_filename,"w+b");

      if (target == NULL)
         {
         printf("Error: could not open target file %s",output_filename);
         exit(1);
         }

      if (fwrite(&image[start_offset], 
                  save_bytes,
                  1,
                  target) != 1)
         {
         printf("Error: could not write to target file, disk full?");
         exit(1);
         }

      fclose(target);

      AIL_mem_free_lock(file);
      }

   //
   // Shut down driver, digital services, and process services
   //
 
   printf( "\nMAKELOOP done.\n");
 
   return 0;
}

// @cdep pre $DefaultsConsoleMSS
// @cdep post $BuildConsoleMSS

