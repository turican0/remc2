//############################################################################
//##                                                                        ##
//##  Miles Sound System                                                    ##
//##                                                                        ##
//##  API.CPP: FLT module for Stereo Delay                                  ##
//##                                                                        ##
//##  32-bit protected-mode source compatible with MSC 11.0/Watcom 10.6     ##
//##                                                                        ##
//##  Version 1.00 of 5-Feb-99: Initial                                     ##
//##                                                                        ##
//##  Author: John Miles, Nick Skrepetos                                    ##
//##                                                                        ##
//############################################################################
//##                                                                        ##
//##  Copyright (C) RAD Game Tools, Inc.                                    ##
//##                                                                        ##
//##  Contact RAD Game Tools at 425-893-4300 for technical support.         ##
//##                                                                        ##
//############################################################################

#include "mss.h"
#include "imssapi.h"

#define FILTER_NAME "Delay Filter"

// Stereo Delay Defaults
#define  _FX_ABSMAX_DELAY        10000.0F
#define  _FX_ABSMIN_DELAY        0.005F

#define  _FX_DEFAULT_MAX_DELAY   300.0F
#define  _FX_DEFAULT_DELAYL      200.0F
#define  _FX_DEFAULT_DELAYR      250.0F
#define  _FX_DEFAULT_FEEDBACK    0.5F
#define  _FX_DEFAULT_FEEDFORWARD 0.0F
#define  _FX_DEFAULT_MIX         0.5F

//
// Attribute tokens
//

enum PROP
{
   //
   // Additional filter attribs (beyond standard RIB PROVIDER_ attributes)
   //

   _FX_PROVIDER_FLAGS,

   _FX_MAX_DELAY,
   _FX_MDELAY_DELAYL,
   _FX_MDELAY_DELAYR,
   _FX_MDELAY_FEEDBACK,
   _FX_MDELAY_FEEDFORWARD,
   _FX_MDELAY_MIX
};

//
// Driver state descriptor
//
// One state descriptor is associated with each HDIGDRIVER
//

struct DRIVERSTATE
{
   HDIGDRIVER dig;
};

//
// Per-sample filter state descriptor
//
// One state descriptor is associated with each HSAMPLE
//

struct SAMPLESTATE
{
   //
   // Members common to all pipeline filter providers
   //

   HSAMPLE          sample;   // HSAMPLE with which this descriptor is associated
   DRIVERSTATE FAR *driver;   // Driver with which this descriptor is associated

   //
   // Members associated with specific filter provider
   //

   S32       calculated_rate;

   F32       fMaxDelay;

   F32       fDelayL;
   F32       fDelayR;
   F32       fFeedback;
   F32       fFeedforward;
   F32       fMix;

   // pointers to delay buffers
   F32   *   pfDelayBufferL;
   F32   *   pfDelayBufferR;

   // total bytes in delay buffer, and current index
   U32       dwBufferBytes;
   U32       dwDelayIndexL;
   U32       dwDelayIndexR;
};

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#include "common.inl"

//############################################################################
//#                                                                          #
//# Allocate buffers for specified delay                                     #
//#                                                                          #
//############################################################################

static void FXSetMaxDelay(SAMPLESTATE FAR *SS, //)
                          S32              playback_rate, 
                          F32              delay_ms)
{
   // get sample rate
   SS->calculated_rate = playback_rate;
   SS->fMaxDelay       = delay_ms;

   FX_CLIPRANGE( SS->fMaxDelay, _FX_ABSMIN_DELAY, _FX_ABSMAX_DELAY);
   FX_CLIPRANGE( SS->fDelayL,   _FX_ABSMIN_DELAY, SS->fMaxDelay);
   FX_CLIPRANGE( SS->fDelayR,   _FX_ABSMIN_DELAY, SS->fMaxDelay);

   // convert MS to samples
   U32 dwSamples = (U32)(S32) (( SS->fMaxDelay * F32(playback_rate) ) / 1000.0F);

   // set buffer size plus some padding for overflow safety
   SS->dwBufferBytes = (dwSamples * sizeof(F32)) + 256;

   if (SS->pfDelayBufferL != NULL)
      {
      AIL_mem_free_lock(SS->pfDelayBufferL);
      AIL_mem_free_lock(SS->pfDelayBufferR);
      }

   SS->pfDelayBufferL = (F32 *) AIL_mem_alloc_lock(SS->dwBufferBytes);
   SS->pfDelayBufferR = (F32 *) AIL_mem_alloc_lock(SS->dwBufferBytes);

   // clear buffer
   AIL_memset( SS->pfDelayBufferL, 0, SS->dwBufferBytes );
   AIL_memset( SS->pfDelayBufferR, 0, SS->dwBufferBytes );

   // reset buffer index
   SS->dwDelayIndexL =  0;
   SS->dwDelayIndexR =  0;
}

//############################################################################
//#                                                                          #
//# Calculate coefficients and values based on parameter set                 #
//#                                                                          #
//############################################################################

static void  FXCalcParams( SAMPLESTATE FAR * SS, S32 playback_rate )
{
}

static void init_sample( SAMPLESTATE FAR * SS )
{
   //
   // Initialize provider-specific members to their default values
   //

   // set default values
   SS->fMaxDelay    =  _FX_DEFAULT_MAX_DELAY;
   SS->fDelayL      =  _FX_DEFAULT_DELAYL;
   SS->fDelayR      =  _FX_DEFAULT_DELAYR;
   SS->fFeedback    =  _FX_DEFAULT_FEEDBACK;
   SS->fFeedforward =  _FX_DEFAULT_FEEDFORWARD;
   SS->fMix         =  _FX_DEFAULT_MIX;

   FXSetMaxDelay(SS, SS->driver->dig->DMA_rate, SS->fMaxDelay);

   // calculate params
   FXCalcParams( SS, SS->driver->dig->DMA_rate );
}

static void close_sample( SAMPLESTATE FAR * SS )
{
   if (SS->pfDelayBufferL != NULL)
      {
      AIL_mem_free_lock(SS->pfDelayBufferL);
      SS->pfDelayBufferL = NULL;
      }

   if (SS->pfDelayBufferR != NULL)
      {
      AIL_mem_free_lock(SS->pfDelayBufferR);
      SS->pfDelayBufferR = NULL;
      }
}

//############################################################################
//#                                                                          #
//#  Process sample data                                                     #
//#                                                                          #
//#  Parameters:                                                             #
//#                                                                          #
//#    state is the sample descriptor.  You can retrieve the HSAMPLE via     #
//#    the state.sample member, if needed.                                   #
//#                                                                          #
//#    source_buffer is a pointer to the a stereo or mono 16-bit sample      #
//#    buffer.                                                               #
//#                                                                          #
//#    n_samples is the number of samples (either mono or stereo) to         #
//#    process.                                                              #
//#                                                                          #
//#    dest_buffer is the destination 16-bit sample buffer.                  #
//#                                                                          #
//#    dest_playback_rate is the hardware sample rate.  Filters must watch   #
//#    for changes in the playback rate and recalculate any dependent        #
//#    parameters.                                                           #
//#                                                                          #
//#    is_stereo says whether the input data is stereo or mono.              #
//#                                                                          #
//############################################################################

static void AILCALL FLTSMP_sample_process(HSAMPLESTATE state,  //)
                                            S16  FAR * MSSRESTRICT  source_buffer,
                                            S16  FAR * MSSRESTRICT  dest_buffer,
                                            S32          n_samples,
                                            S32          dest_playback_rate,
                                            S32          is_stereo)
{
   SAMPLESTATE FAR *SSp  = (SAMPLESTATE FAR *) state;
   SAMPLESTATE SS;
   AIL_memcpy(&SS, SSp, sizeof(SS));
   //HSAMPLE          S   = SS.sample;
   //DRIVERSTATE FAR *DRV = SS.driver;

   S32 dwIndex;
   F32 fInput;
   F32 fOutL,fOutR;
   F32 delOutL,delOutR;
   F32 fDryOut;
   F32 fWetOut;

   if (dest_playback_rate != SS.calculated_rate)
      {
      // This filter doesn't support changes in the dest playback rate...
      FXCalcParams(&SS, dest_playback_rate);
      }

   F32 fRate = (F32) dest_playback_rate;

   U32 dwDelayLengthL =  (U32) (S32) (( SS.fDelayL * fRate ) / 1000.0F);
   U32 dwDelayLengthR =  (U32) (S32) (( SS.fDelayR * fRate ) / 1000.0F);

   // set wet/dry mix
   fDryOut  =  1.0F - SS.fMix;
   fWetOut  =  SS.fMix;

   //fast path
   if ( fDryOut > 0.999f )
   {
     if ( source_buffer != dest_buffer )
       AIL_memcpy( dest_buffer, source_buffer, n_samples * ( is_stereo ? 4 : 2 ) );
     return;
   }

   // check if mono or stereo
   if ( is_stereo )
   {
      // mix into build buffer
      for( dwIndex = 0; dwIndex < n_samples; dwIndex++ )
      {
         // get input sample (left)
         fInput   =  (F32)(S16)LE_SWAP16(source_buffer);

         // apply feedforward
         delOutL = SS.pfDelayBufferL[ SS.dwDelayIndexL ] + (fInput * -SS.fFeedforward) + _FX_DENORMVAL;

         // set output
         fOutL    =  ( fInput * fDryOut ) + ( delOutL * fWetOut );

         // calculate output sample
         SS.pfDelayBufferL[ SS.dwDelayIndexL ]  =  fInput + ( delOutL * SS.fFeedback ) + _FX_DENORMVAL;

         // advance index
         if ( ++SS.dwDelayIndexL >= dwDelayLengthL )
            SS.dwDelayIndexL =  0;

         // get input sample (right)
         fInput   =  (F32)(S16)LE_SWAP16_OFS(source_buffer,2);

         // apply feedforward
         delOutR = SS.pfDelayBufferR[ SS.dwDelayIndexR ] + (fInput * -SS.fFeedforward) + _FX_DENORMVAL;

         // set output
         fOutR    =  ( fInput * fDryOut ) + ( delOutR * fWetOut );

         // calculate output sample
         SS.pfDelayBufferR[ SS.dwDelayIndexR ]  =  fInput + ( delOutR * SS.fFeedback ) + _FX_DENORMVAL;

         // advance index
         if ( ++SS.dwDelayIndexR >= dwDelayLengthR )
            SS.dwDelayIndexR =  0;

         // store output

         S32 tmp;
         WRITE_STEREO_SAMPLE( dest_buffer, fOutL, fOutR, tmp );

         source_buffer += 2;
      }
   }
   else
   {
      for( dwIndex = 0; dwIndex < n_samples; dwIndex++ )
      {
         // get input sample
         fInput   =  (F32)(S16)LE_SWAP16(source_buffer);
         ++source_buffer;

         // apply feedforward
         delOutL = SS.pfDelayBufferL[ SS.dwDelayIndexL ] + (fInput * -SS.fFeedforward) + _FX_DENORMVAL;

         // set output
         fOutL  =  ( fInput * fDryOut ) + ( delOutL * fWetOut );

         // calculate output sample
         SS.pfDelayBufferL[ SS.dwDelayIndexL ]  =  fInput + ( delOutL * SS.fFeedback ) + _FX_DENORMVAL;

         // advance index
         if ( ++SS.dwDelayIndexL >= dwDelayLengthL )
            SS.dwDelayIndexL  =  0;

         // store output
         S32 tmp;
         WRITE_MONO_SAMPLE( dest_buffer, fOutL, tmp );
      }
   }
   AIL_memcpy(SSp, &SS, sizeof(*SSp));
}


//############################################################################
//#                                                                          #
//# Set sample preference value, returning previous setting                  #
//#                                                                          #
//############################################################################

static S32 AILCALL FLTSMP_sample_property(HSAMPLESTATE state, HPROPERTY property,void FAR * before_value, void const FAR * new_value, void FAR * after_value)
{
   SAMPLESTATE FAR *SS  = (SAMPLESTATE FAR *) state;
   //HSAMPLE          S   = SS->sample;

   // determine preference
   switch (property)
      {
      //
      // Preferences
      //
      case  _FX_MAX_DELAY :
       if ( before_value ) *(F32 FAR*)before_value = SS->fMaxDelay;
       if ( new_value ) 
       {
         SS->fMaxDelay = *(F32 const FAR*)new_value;
         // clip to valid range
         FXSetMaxDelay(SS, SS->driver->dig->DMA_rate, SS->fMaxDelay);
         FXCalcParams(SS, SS->driver->dig->DMA_rate);
       }         
       if ( after_value ) *(F32 FAR*)after_value = SS->fMaxDelay;
       return 1;

      case  _FX_MDELAY_DELAYL     :
       if ( before_value ) *(F32 FAR*)before_value = SS->fDelayL;
       if ( new_value ) 
       {
         SS->fDelayL = *(F32 const FAR*)new_value;
         // clip to valid range
         FX_CLIPRANGE( SS->fDelayL, _FX_ABSMIN_DELAY, SS->fMaxDelay);
         FXCalcParams(SS, SS->driver->dig->DMA_rate);
       }         
       if ( after_value ) *(F32 FAR*)after_value = SS->fDelayL;
       return 1;

      case  _FX_MDELAY_DELAYR     :
       if ( before_value ) *(F32 FAR*)before_value = SS->fDelayR;
       if ( new_value ) 
       {
         SS->fDelayR = *(F32 const FAR*)new_value;
         // clip to valid range
         FX_CLIPRANGE( SS->fDelayR, _FX_ABSMIN_DELAY, SS->fMaxDelay);
         FXCalcParams(SS, SS->driver->dig->DMA_rate);
       }         
       if ( after_value ) *(F32 FAR*)after_value = SS->fDelayR;
       return 1;

      case  _FX_MDELAY_FEEDBACK     :
       if ( before_value ) *(F32 FAR*)before_value = SS->fFeedback;
       if ( new_value ) 
       {
         SS->fFeedback = *(F32 const FAR*)new_value;
         // clip to valid range
         FX_CLIPRANGE( SS->fFeedback, 0.0F, 1.0F );
       }         
       if ( after_value ) *(F32 FAR*)after_value = SS->fFeedback;
       return 1;

      case  _FX_MDELAY_FEEDFORWARD    :
       if ( before_value ) *(F32 FAR*)before_value = SS->fFeedforward;
       if ( new_value ) 
       {
         SS->fFeedforward = *(F32 const FAR*)new_value;
         // clip to valid range
         FX_CLIPRANGE( SS->fFeedforward, 0.0F, 1.0F );
       }         
       if ( after_value ) *(F32 FAR*)after_value = SS->fFeedforward;
       return 1;

      case  _FX_MDELAY_MIX       :
       if ( before_value ) *(F32 FAR*)before_value = SS->fMix;
       if ( new_value ) 
       {
         SS->fMix = *(F32 const FAR*)new_value;
         // clip to valid range
         FX_CLIPRANGE( SS->fMix, 0.0F, 1.0F );
       }         
       if ( after_value ) *(F32 FAR*)after_value = SS->fMix;
       return 1;
      }


   return 0;
}

extern "C" S32 DelayMain( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 DelayMain( HPROVIDER provider_handle, U32 up_down )
{
   const RIB_INTERFACE_ENTRY FLT1[] =
      {
      REG_FN(PROVIDER_property),
      REG_PR("Name",                     PROVIDER_NAME,        (RIB_DATA_SUBTYPE) (RIB_STRING|RIB_READONLY)),
      REG_PR("Version",                  PROVIDER_VERSION,     (RIB_DATA_SUBTYPE) (RIB_HEX|RIB_READONLY)),
      REG_PR("Flags",                   _FX_PROVIDER_FLAGS,    (RIB_DATA_SUBTYPE) (RIB_HEX|RIB_READONLY)),
      };

   const RIB_INTERFACE_ENTRY FLT2[] =
      {
      REG_FN(FLT_startup),
      REG_FN(FLT_error),
      REG_FN(FLT_shutdown),
      REG_FN(FLT_open_driver),
      };

   const RIB_INTERFACE_ENTRY FLT3[] =
      {
      REG_FN(FLT_close_driver),
      REG_FN(FLT_premix_process),
      REG_FN(FLT_postmix_process),
      };

   const RIB_INTERFACE_ENTRY FLTSMP1[] =
      {
      REG_FN(FLTSMP_open_sample),
      REG_FN(FLTSMP_close_sample),
      REG_FN(FLTSMP_sample_process),
      REG_FN(FLTSMP_sample_property),
      };

   const RIB_INTERFACE_ENTRY FLTSMP2[] =
      {
      REG_PR("Delay TimeL",       _FX_MDELAY_DELAYL  ,      RIB_FLOAT),
      REG_PR("Delay TimeR",       _FX_MDELAY_DELAYR  ,      RIB_FLOAT),
      REG_PR("Delay Feedback",    _FX_MDELAY_FEEDBACK  ,    RIB_FLOAT),
      REG_PR("Delay Feedforward", _FX_MDELAY_FEEDFORWARD ,  RIB_FLOAT),
      };

   const RIB_INTERFACE_ENTRY FLTSMP3[] =
      {
      REG_PR("Max Delay Time",    _FX_MAX_DELAY ,           RIB_FLOAT),
      REG_PR("Mix",               _FX_MDELAY_MIX  ,         RIB_FLOAT),
      };

   if (up_down)
      {
         RIB_register(provider_handle, "MSS pipeline filter", FLT1);
         RIB_register(provider_handle, "MSS pipeline filter", FLT2);
         RIB_register(provider_handle, "MSS pipeline filter", FLT3);

         RIB_register(provider_handle, "Pipeline filter sample services", FLTSMP1);
         RIB_register(provider_handle, "Pipeline filter sample services", FLTSMP2);
         RIB_register(provider_handle, "Pipeline filter sample services", FLTSMP3);
      }
   else
      {
         RIB_unregister_all(provider_handle);
      }

   return TRUE;
}

