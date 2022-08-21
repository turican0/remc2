//############################################################################
//##                                                                        ##
//##  Miles Sound System                                                    ##
//##                                                                        ##
//##  API.CPP: FLT module for Phaser Filter                                 ##
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

#define FILTER_NAME "Phaser Filter"

// Phaser defaults
#define  _FX_FEEDBACK_DEFAULT    0.8F
#define  _FX_RATE_DEFAULT        2.0F
#define  _FX_RANGE_DEFAULT       4.0F
#define  _FX_BASEFREQ_DEFAULT    200.0F
#define  _FX_MIX_DEFAULT         0.8F

//
// Attribute tokens
//

enum PROP
{
   //
   // Additional filter attribs (beyond standard RIB PROVIDER_ attributes)
   //

   _FX_PROVIDER_FLAGS,

   _FX_PHASER_FEEDBACK,
   _FX_PHASER_RATE,
   _FX_PHASER_RANGE,
   _FX_PHASER_BASEFREQ,
   _FX_PHASER_MIX
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

   S32    calculated_rate;

   F32    fFeedback;
   F32    fRate;
   F32    fRange;
   F32    fBaseFreq;
   F32    fMix;

   F32    fLX1;
   F32    fLY1;
   F32    fLX2;
   F32    fLY2;
   F32    fLX3;
   F32    fLY3;
   F32    fLX4;
   F32    fLY4;

   F32    fWP;
   F32    fSweepMin;
   F32    fSweepRange;
   F32    fSweepMax;
   F32    fSweepRate;
   F32    fSweepRateInv;
   F32    fSweepFrac;
};

#include "common.inl"

//############################################################################
//#                                                                          #
//# Calculate coefficients and values based on parameter set                 #
//#                                                                          #
//############################################################################

static void FXCalcParams( SAMPLESTATE FAR * SS, S32 playback_rate )
{
   // get sample rate
   F32 fRate = F32(playback_rate);
   SS->calculated_rate = playback_rate;

   // calculate parameters for sweeping filters
   SS->fWP  =  ( F_PI * SS->fBaseFreq ) / fRate;

   // set minimum sweep
   SS->fSweepMin     =  SS->fWP;

   // calculate range
   SS->fSweepRange   =  (F32) AIL_pow( 2.0F, SS->fRange );

   // set maximum sweep
   SS->fSweepMax     =  ( F_PI * SS->fBaseFreq * SS->fSweepRange ) / fRate;

   // set sweep rate
   SS->fSweepRate    =  (F32) AIL_pow( SS->fSweepRange, SS->fRate / ( fRate / 2.0F ));
   SS->fSweepRateInv = 1.0F / SS->fSweepRate;
   
   // set fractional sweep
   SS->fSweepFrac    =  SS->fSweepRate;
}


static void init_sample( SAMPLESTATE FAR * SS )
{
   //
   // Initialize provider-specific members to their default values
   //

   SS->fFeedback     =  _FX_FEEDBACK_DEFAULT;
   SS->fRate         =  _FX_RATE_DEFAULT;
   SS->fRange        =  _FX_RANGE_DEFAULT;
   SS->fBaseFreq     =  _FX_BASEFREQ_DEFAULT;
   SS->fMix          =  _FX_MIX_DEFAULT;

   // calculate params
   FXCalcParams( SS, SS->driver->dig->DMA_rate );

   // reset all history values
   SS->fLX1    =  0.0F;
   SS->fLY1    =  0.0F;
   SS->fLX2    =  0.0F;
   SS->fLY2    =  0.0F;
   SS->fLX3    =  0.0F;
   SS->fLY3    =  0.0F;
   SS->fLX4    =  0.0F;
   SS->fLY4    =  0.0F;
}

static void close_sample( SAMPLESTATE FAR * SS )
{
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
   F32 fX1, fCoef, fCoefn;
   F32 fDryOut, fWetOut;

   if (dest_playback_rate != SS.calculated_rate)
      {
      FXCalcParams(&SS, dest_playback_rate);
      }

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

   // calculate coef
   fCoef    =  ( 1.0F - SS.fWP ) / ( 1.0F + SS.fWP );

   // check if mono or stereo
   if ( is_stereo )
   {
      // mix into build buffer
      for( dwIndex = 0; dwIndex < n_samples; dwIndex++ )
      {
         // get input sample (left)
         fInput   =  (F32)(S16)LE_SWAP16(source_buffer);

         // get next coef

         // adjust frequency of sweep filter
         SS.fWP  *= SS.fSweepFrac;

         // check sweep ranges
         if ( SS.fWP > SS.fSweepMax )
            SS.fSweepFrac =  SS.fSweepRateInv;
         else
         {
            // check for minimum
            if ( SS.fWP < SS.fSweepMin )
               SS.fSweepFrac =  SS.fSweepRate;
         }

         // calculate next coef
         fCoefn    =  ( 1.0F - SS.fWP ) / ( 1.0F + SS.fWP );

         // process
         fX1   =  fInput + ( SS.fFeedback * SS.fLY4 );

         // do process all filters
         SS.fLY1 =  fCoef * ( SS.fLY1 + fX1 ) - SS.fLX1;
         SS.fLX1 =  fX1;
         SS.fLY2 =  fCoef * ( SS.fLY2 + SS.fLY1 ) - SS.fLX2;
         SS.fLX2 =  SS.fLY1;
         SS.fLY3 =  fCoef * ( SS.fLY3 + SS.fLY2 ) - SS.fLX3;
         SS.fLX3 =  SS.fLY2;
         SS.fLY4 =  fCoef * ( SS.fLY4 + SS.fLY3 ) - SS.fLX4;
         SS.fLX4 =  SS.fLY3;

         // calculate final output
         fOutL  =  SS.fLY4 * fWetOut + fInput * fDryOut;

         // get input sample (right)
         fInput   =  (F32)(S16)LE_SWAP16_OFS(source_buffer,2);

         // process
         fX1   =  fInput + ( SS.fFeedback * SS.fLY4 );

         // do process all filters
         SS.fLY1 =  fCoef * ( SS.fLY1 + fX1 ) - SS.fLX1;
         SS.fLX1 =  fX1;
         SS.fLY2 =  fCoef * ( SS.fLY2 + SS.fLY1 ) - SS.fLX2;
         SS.fLX2 =  SS.fLY1;
         SS.fLY3 =  fCoef * ( SS.fLY3 + SS.fLY2 ) - SS.fLX3;
         SS.fLX3 =  SS.fLY2;
         SS.fLY4 =  fCoef * ( SS.fLY4 + SS.fLY3 ) - SS.fLX4;
         SS.fLX4 =  SS.fLY3;

         // calculate final output
         fOutR  =  SS.fLY4 * fWetOut + fInput * fDryOut;

         S32 tmp;
         WRITE_STEREO_SAMPLE( dest_buffer, fOutL, fOutR, tmp );

         source_buffer += 2;
         fCoef = fCoefn;
      }
   }
   else
   {
      // mix into build buffer
      for( dwIndex = 0; dwIndex < n_samples; dwIndex++ )
      {
         // get input sample
         fInput   =  (F32)(S16)LE_SWAP16(source_buffer);
         ++source_buffer;

         // get next coef

         // adjust frequency of sweep filter
         SS.fWP  *= SS.fSweepFrac;

         // check sweep ranges
         if ( SS.fWP > SS.fSweepMax )
            SS.fSweepFrac =  SS.fSweepRateInv;
         else
         {
            // check for minimum
            if ( SS.fWP < SS.fSweepMin )
               SS.fSweepFrac =  SS.fSweepRate;
         }

         // calculate next coef
         fCoefn    =  ( 1.0F - SS.fWP ) / ( 1.0F + SS.fWP );

         // process
         fX1   =  fInput + ( SS.fFeedback * SS.fLY4 );

         // do process all filters
         SS.fLY1 =  fCoef * ( SS.fLY1 + fX1 ) - SS.fLX1;
         SS.fLX1 =  fX1;
         SS.fLY2 =  fCoef * ( SS.fLY2 + SS.fLY1 ) - SS.fLX2;
         SS.fLX2 =  SS.fLY1;
         SS.fLY3 =  fCoef * ( SS.fLY3 + SS.fLY2 ) - SS.fLX3;
         SS.fLX3 =  SS.fLY2;
         SS.fLY4 =  fCoef * ( SS.fLY4 + SS.fLY3 ) - SS.fLX4;
         SS.fLX4 =  SS.fLY3;

         // calculate final output
         fOutL  =  SS.fLY4 * fWetOut + fInput * fDryOut;

         // store output
         S32 tmp;
         WRITE_MONO_SAMPLE( dest_buffer, fOutL, tmp );

         fCoef = fCoefn;
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
      case  _FX_PHASER_FEEDBACK  :
       if ( before_value ) *(F32 FAR*)before_value = SS->fMix;
       if ( new_value ) 
       {
         SS->fFeedback = *(F32 const FAR*)new_value;
         // clip to valid range
         FX_CLIPRANGE( SS->fMix, 0.0F, 1.0F );
       }         
       if ( after_value ) *(F32 FAR*)after_value = SS->fFeedback;
       return 1;

      case  _FX_PHASER_RATE      :
       if ( before_value ) *(F32 FAR*)before_value = SS->fRate;
       if ( new_value ) 
       {
         SS->fRate = *(F32 const FAR*)new_value;
         // clip to valid range
         FX_CLIPRANGE( SS->fRate, 0.01F, 4.0f );
         FXCalcParams(SS, SS->driver->dig->DMA_rate);
       }         
       if ( after_value ) *(F32 FAR*)after_value = SS->fRate;
       return 1;

      case  _FX_PHASER_RANGE     :
       if ( before_value ) *(F32 FAR*)before_value = SS->fRange;
       if ( new_value ) 
       {
         SS->fRange = *(F32 const FAR*)new_value;
         // clip to valid range
         FX_CLIPRANGE( SS->fRange, 0.01F, 4.0f );
         FXCalcParams(SS, SS->driver->dig->DMA_rate);
       }         
       if ( after_value ) *(F32 FAR*)after_value = SS->fRange;
       return 1;

      case  _FX_PHASER_BASEFREQ  :
       if ( before_value ) *(F32 FAR*)before_value = SS->fBaseFreq;
       if ( new_value ) 
       {
         SS->fBaseFreq = *(F32 const FAR*)new_value;
         // clip to valid range
         FX_CLIPRANGE( SS->fBaseFreq, 20.0F, ((F32) SS->driver->dig->DMA_rate) / 2.0F - 1.0F );
         FXCalcParams(SS, SS->driver->dig->DMA_rate);
       }         
       if ( after_value ) *(F32 FAR*)after_value = SS->fBaseFreq;
       return 1;

      case  _FX_PHASER_MIX       :
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

extern "C" S32 PhaserMain( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 PhaserMain( HPROVIDER provider_handle, U32 up_down )
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
      REG_PR("Phaser Feedback",      _FX_PHASER_FEEDBACK,    RIB_FLOAT),
      REG_PR("Phaser Rate",          _FX_PHASER_RATE,        RIB_FLOAT),
      REG_PR("Phaser Range",         _FX_PHASER_RANGE,       RIB_FLOAT),
      REG_PR("Phaser BaseFreq",      _FX_PHASER_BASEFREQ,    RIB_FLOAT),
      };

   const RIB_INTERFACE_ENTRY FLTSMP3[] =
      {
      REG_PR("Mix",                  _FX_PHASER_MIX,         RIB_FLOAT),
      };

   if (up_down)
      {
         RIB_register(provider_handle, "MSS pipeline filter",  FLT1);
         RIB_register(provider_handle, "MSS pipeline filter",  FLT2);
         RIB_register(provider_handle, "MSS pipeline filter",  FLT3);

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
