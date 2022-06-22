#include "mss.h"

#include "imssapi.h"

#if !defined(IS_X86) || !defined(IS_MAC)


static S32 AILCALL MSS_mixer_property (HPROPERTY index, void FAR * before_value, void const FAR * new_value, void FAR * after_value)
{
   switch ( (U32) index )
      {
      case PROVIDER_NAME:
        if (before_value)
        {
          *(char FAR * FAR*)before_value = "Default Miles Mixer";
          return 1;
        }
        break;
        
      case PROVIDER_VERSION:
        if (before_value) 
        {
          *(S32 FAR*)before_value = 0x110;
          return 1;
        }
        break;
   }
   return( 0 );
}

static void AILCALL MSS_mixer_mc_copy ( MSS_BB FAR * build,
                                        S32 n_build_buffers,
                                        void FAR * lpWaveAddr,
                                        S32 hw_format,
#ifdef IS_X86
                                        S32 use_MMX,
#endif
                                        S32 samples_per_buffer,
                                        S32 physical_channels_per_sample )
{
   //
   // Discrete MC_SS_copy() assumes physical == logical channels
   //

   //
   // For each build buffer...
   //

   for (S32 i=0; i < n_build_buffers; i++)
      {
      //
      // Add contents of build buffer to destination channels with clamping
      //
      // In practice, drivers open for two-channel output will use the legacy SS_copy() routine
      //

      MSS_BB *B = &build[i];
#ifdef IS_PS3
         {
         S32 * MSSRESTRICT src  = B->buffer;
         F32 * MSSRESTRICT dest = &((F32 FAR *) lpWaveAddr)[ B->speaker_offset ];

         if (B->chans == 2)
            {
            S32 n = samples_per_buffer * 2;

            for (S32 j=0; j < n; j += 2)
               {
               F32 left  = ( (F32) src[j] ) * (1.0f / ( 2048.0f * 32768.0f ) );
               F32 right = ( (F32) src[j+1] ) * (1.0f / ( 2048.0f * 32768.0f ) );

#if 1
               left = fclip1( left );
               right = fclip1( right );
               dest[0]=left;
               dest[1]=right;
#else
               if      (left <= -1.0f)  dest[0] = -1.0f;
               else if (left >=  1.0f)  dest[0] =  1.0f; 
               else                     dest[0] =  left;

               if      (right <= -1.0f) dest[1] = -1.0f;
               else if (right >=  1.0f) dest[1] =  1.0f; 
               else                     dest[1] =  right;
#endif
               dest += physical_channels_per_sample;
               }
            }
         else
            {
            S32 n = samples_per_buffer;

            for (S32 j=0; j < n; j++)
               {
               F32 mono  = ( (F32) src[j] ) * (1.0f / ( 2048.0f * 32768.0f ) );

#if 1
               mono = fclip1( mono );
               dest[0]=mono;
#else
               if      (mono <= -1.0f)  *dest = -1.0f;
               else if (mono >=  1.0f)  *dest =  1.0f; 
               else                     *dest =  mono;
#endif
               dest += physical_channels_per_sample;
               }
            }
         }
#else
      if (hw_format & DIG_F_16BITS_MASK)
         {
         S32 * MSSRESTRICT src  = B->buffer;
         S16 * MSSRESTRICT dest = &((S16 FAR *) lpWaveAddr)[ B->speaker_offset ];

         if (B->chans == 2)
            {
            S32 n = samples_per_buffer * 2;

            for (S32 j=0; j < n; j += 2)
               {
               S32 left  = src[j]   >> 11;
               S32 right = src[j+1] >> 11;

               if      (left <= -32768)  dest[0] = -32768;
               else if (left >=  32767)  dest[0] =  32767; 
               else                      dest[0] =  (S16) left;

               if      (right <= -32768) dest[1] = -32768;
               else if (right >=  32767) dest[1] =  32767; 
               else                      dest[1] =  (S16) right;

               dest += physical_channels_per_sample;
               }
            }
         else
            {
            S32 n = samples_per_buffer;

            for (S32 j=0; j < n; j++)
               {
               S32 mono = src[j] >> 11;

               if      (mono <= -32768)  *dest = -32768;
               else if (mono >=  32767)  *dest =  32767; 
               else                      *dest =  (S16) mono;

               dest += physical_channels_per_sample;
               }
            }
         }
      else
         {
         S32 * MSSRESTRICT src  = B->buffer;
         U8 *  MSSRESTRICT dest = &((U8 FAR *) lpWaveAddr)[ B->speaker_offset ];

         if (B->chans == 2)
            {
            S32 n = samples_per_buffer * 2;

            for (S32 j=0; j < n; j += 2)
               {
               S32 left  = src[j]   >> 11;
               S32 right = src[j+1] >> 11;

               if      (left <= -32768)  dest[0] = 0;
               else if (left >=  32767)  dest[0] = 255;
               else                      dest[0] = (U8)((left >> 8) ^ 0x80);

               if      (right <= -32768) dest[1] = 0;
               else if (right >=  32767) dest[1] = 255;
               else                      dest[1] = (U8)((right >> 8) ^ 0x80); 

               dest += physical_channels_per_sample;
               }
            }
         else
            {
            S32 n = samples_per_buffer;

            for (S32 j=0; j < n; j++)
               {
               S32 mono = src[j] >> 11;

               if      (mono <= -32768)  *dest = 0;
               else if (mono >=  32767)  *dest = 255;
               else                      *dest = (U8)((mono >> 8) ^ 0x80);

               dest += physical_channels_per_sample;
               }
            }
         }
#endif   
      }
}



static void AILCALL MSS_mixer_adpcm_decode( void FAR * dest,
                                            void const FAR * in,
                                            S32 out_len,
                                            S32 in_len,
                                            S32 input_format,
                                            ADPCMDATA FAR *adpcm_data )
{
  if ( input_format & M_SRC_16 )
    if ( input_format & M_SRC_STEREO )
      DecodeADPCM_STEREO(dest,in,out_len,in_len,adpcm_data);
    else
      DecodeADPCM_MONO(dest,in,out_len,in_len,adpcm_data);
  else
    DecodeADPCM_MONO_8(dest,in,out_len,in_len,adpcm_data);
}

#ifdef STANDALONEMIXRIB
DXDEF S32 AILEXPORT RIB_MAIN_NAME(MIX)( HPROVIDER provider_handle, U32 up_down )
#else
DXDEF S32 AILEXPORT MIX_RIB_MAIN( HPROVIDER provider_handle, U32 up_down )
#endif
{
  if (up_down)
  {
    //
    // Initialize default internal mixer RIB
    //

    static const RIB_INTERFACE_ENTRY MIXER[] =
       {
       { RIB_FUNCTION, "PROVIDER_property", (UINTa) &MSS_mixer_property,      RIB_NONE },
       { RIB_FUNCTION, "MIXER_startup",     (UINTa) &MSS_mixer_startup,       RIB_NONE },
       { RIB_FUNCTION, "MIXER_startup",     (UINTa) &MSS_mixer_startup,       RIB_NONE },
       { RIB_FUNCTION, "MIXER_shutdown",    (UINTa) &MSS_mixer_shutdown,      RIB_NONE },
       { RIB_FUNCTION, "MIXER_flush",       (UINTa) &MSS_mixer_flush,         RIB_NONE },
       { RIB_FUNCTION, "MIXER_merge",       (UINTa) &MSS_mixer_merge,         RIB_NONE },
       { RIB_FUNCTION, "MIXER_copy",        (UINTa) &MSS_mixer_copy,          RIB_NONE },
       { RIB_FUNCTION, "MIXER_mc_copy",     (UINTa) &MSS_mixer_mc_copy,       RIB_NONE },
       { RIB_FUNCTION, "MIXER_adpcm_decode",(UINTa) &MSS_mixer_adpcm_decode,  RIB_NONE }
       };

    RIB_register(provider_handle,
                 "MSS mixer services",
                 MIXER);

  }
  else
  {
    RIB_unregister_all(provider_handle);
  }

  return( TRUE );
}

#endif

