//############################################################################
//##                                                                        ##
//##  Miles Sound System                                                    ##
//##                                                                        ##
//##  API.CPP: FLT module for Shelving EQ Filter                            ##
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

#define FILTER_NAME "ShelvingEQ Filter"

// Shelving EQ Defaults
#define  _FX_LSFREQ_DEFAULT      500.0F
#define  _FX_LSGAIN_DEFAULT      -10.0F
#define  _FX_HSFREQ_DEFAULT      4000.0F
#define  _FX_HSGAIN_DEFAULT      2.0F
#define  _FX_MIX_DEFAULT         1.0F

//                                 
// Attribute tokens
//

enum PROP
{
   //
   // Additional filter attribs (beyond standard RIB PROVIDER_ attributes)
   //

   _FX_PROVIDER_FLAGS,

   _FX_MIX,
   _FX_SHELFEQ_LOWFREQ,
   _FX_SHELFEQ_LOWGAIN,
   _FX_SHELFEQ_HIGHFREQ,
   _FX_SHELFEQ_HIGHGAIN
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

   S32          calculated_rate;

   F32          fMix;
   F32          fLowShelfFreq;
   F32          fLowShelfGain;
   F32          fHighShelfFreq;
   F32          fHighShelfGain;

   // low shelf history / coefficients
   F32          fLSXL[ 3 ];
   F32          fLSYL[ 3 ];
   F32          fLSXR[ 3 ];
   F32          fLSYR[ 3 ];
   F32          fLSA[ 3 ];
   F32          fLSB[ 3 ];

   // high shelf history / coefficients
   F32          fHSXL[ 3 ];
   F32          fHSYL[ 3 ];
   F32          fHSXR[ 3 ];
   F32          fHSYR[ 3 ];
   F32          fHSA[ 3 ];
   F32          fHSB[ 3 ];
};

#include "common.inl"

//############################################################################
//#                                                                          #
//# Calculate coefficients and values based on parameter set                 #
//#                                                                          #
//############################################################################

static void  FXCalcParams( SAMPLESTATE FAR * SS, S32 playback_rate )
{
   F32 fRate;
   F32 fSN, fCS;
   F32 fA;
   F32 fAlpha;
   F32 fBeta;
   F32 fS =  1.0F;
   F32 fOmega;
   F32 fQ =  1.0F;

   // get sample rate
   fRate = (F32) playback_rate;
   SS->calculated_rate = playback_rate;

   // compute initial values used to derive coefficients
   fA       =  AIL_fsqrt( AIL_pow( 10.0F,  ( SS->fLowShelfGain / 20.0F )));
   fOmega   =  2.0F * F_PI * ( SS->fLowShelfFreq / fRate );
   fSN      =  (F32) AIL_sin( fOmega );
   fCS      =  (F32) AIL_cos( fOmega );
   fAlpha   =  fSN / ( 2.0F * fQ );
   fBeta    =  AIL_fsqrt( ( ( fA * fA ) + 1.0F ) / fS - ( ( fA - 1.0F ) * ( fA - 1.0F )) );

   // calculate coefficients for low-shelf
   SS->fLSB[ 0 ] =  fA * ( ( fA + 1.0F ) - ( fA - 1.0F ) * fCS + fBeta * fSN );
   SS->fLSB[ 1 ] =  2.0F * fA * ( ( fA - 1.0F ) - ( fA + 1.0F ) * fCS );
   SS->fLSB[ 2 ] =  fA * ( ( fA + 1.0F ) - ( fA - 1.0F ) * fCS - fBeta * fSN );
   SS->fLSA[ 0 ] =  ( fA + 1.0F ) + ( fA - 1.0F ) * fCS + fBeta * fSN;
   SS->fLSA[ 1 ] =  -2.0F * ( ( fA - 1.0F ) + ( fA + 1.0F ) * fCS );
   SS->fLSA[ 2 ] =  ( fA + 1.0F ) + ( fA - 1.0F ) * fCS - fBeta * fSN;

   // compute initial values used to derive coefficients
   fA       =  AIL_fsqrt( AIL_pow( 10.0F,  ( SS->fHighShelfGain / 20.0F )));
   fOmega   =  2.0F * F_PI * ( SS->fHighShelfFreq / fRate );
   fSN      =  (F32) AIL_sin( fOmega );
   fCS      =  (F32) AIL_cos( fOmega );
   fAlpha   =  fSN / ( 2.0F * fQ );
   fBeta    =  AIL_fsqrt( ( ( fA * fA ) + 1.0F ) / fS - ( ( fA - 1.0F ) * ( fA - 1.0F )) );

   // calculate coefficients for high-shelf
   SS->fHSB[ 0 ] =  fA * ( ( fA + 1.0F ) + ( fA - 1.0F ) * fCS + fBeta * fSN );
   SS->fHSB[ 1 ] =  -2.0F * fA * ( ( fA - 1.0F ) + ( fA + 1.0F ) * fCS );
   SS->fHSB[ 2 ] =  fA * ( ( fA + 1.0F ) + ( fA -1.0F ) * fCS - fBeta * fSN );
   SS->fHSA[ 0 ] =  ( fA + 1.0F ) - ( fA - 1.0F ) * fCS + fBeta * fSN;
   SS->fHSA[ 1 ] =  2.0F * ( ( fA - 1.0F ) - ( fA + 1.0F ) * fCS );
   SS->fHSA[ 2 ] =  ( fA + 1.0F ) - ( fA - 1.0F ) * fCS - fBeta * fSN;
}

static void init_sample( SAMPLESTATE FAR * SS )
{
   //
   // Initialize provider-specific members to their default values
   //

   // init values
   SS->fMix             =  _FX_MIX_DEFAULT;
   SS->fLowShelfFreq    =  _FX_LSFREQ_DEFAULT;
   SS->fLowShelfGain    =  _FX_LSGAIN_DEFAULT;
   SS->fHighShelfFreq   =  _FX_HSFREQ_DEFAULT;
   SS->fHighShelfGain   =  _FX_HSGAIN_DEFAULT;

   // reset sample history
   SS->fLSXL[ 0 ] =  0.0F;
   SS->fLSXL[ 1 ] =  0.0F;
   SS->fLSXL[ 2 ] =  0.0F;
   SS->fLSYL[ 0 ] =  0.0F;
   SS->fLSYL[ 1 ] =  0.0F;
   SS->fLSYL[ 2 ] =  0.0F;
   SS->fLSXR[ 0 ] =  0.0F;
   SS->fLSXR[ 1 ] =  0.0F;
   SS->fLSXR[ 2 ] =  0.0F;
   SS->fLSYR[ 0 ] =  0.0F;
   SS->fLSYR[ 1 ] =  0.0F;
   SS->fLSYR[ 2 ] =  0.0F;
   SS->fHSXL[ 0 ] =  0.0F;
   SS->fHSXL[ 1 ] =  0.0F;
   SS->fHSXL[ 2 ] =  0.0F;
   SS->fHSYL[ 0 ] =  0.0F;
   SS->fHSYL[ 1 ] =  0.0F;
   SS->fHSYL[ 2 ] =  0.0F;
   SS->fHSXR[ 0 ] =  0.0F;
   SS->fHSXR[ 1 ] =  0.0F;
   SS->fHSXR[ 2 ] =  0.0F;
   SS->fHSYR[ 0 ] =  0.0F;
   SS->fHSYR[ 1 ] =  0.0F;
   SS->fHSYR[ 2 ] =  0.0F;

   // update params
   FXCalcParams( SS, SS->driver->dig->DMA_rate );
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
   F32 fHSInput;

   // set wet/dry mix
   F32 fDryOut  =  1.0F - SS.fMix;
   F32 fWetOut  =  SS.fMix;

   //fast path
   if ( fDryOut > 0.999f )
   {
     if ( source_buffer != dest_buffer )
       AIL_memcpy( dest_buffer, source_buffer, n_samples * ( is_stereo ? 4 : 2 ) );
     return;
   }

   if (dest_playback_rate != SS.calculated_rate)
      {
      FXCalcParams(&SS, dest_playback_rate);
      }

   // check if mono or stereo
   if ( is_stereo )
   {
      // mix into build buffer
      for( dwIndex = 0; dwIndex < n_samples; dwIndex++ )
      {
         // get input sample (left)
         fInput   =  (F32)(S16)LE_SWAP16(source_buffer);

         // calculate output
         fHSInput =  ( SS.fLSB[ 0 ] / SS.fLSA[ 0 ] ) * fInput + \
                     ( SS.fLSB[ 1 ] / SS.fLSA[ 0 ] ) * SS.fLSXL[ 0 ] + \
                     ( SS.fLSB[ 2 ] / SS.fLSA[ 0 ] ) * SS.fLSXL[ 1 ] - \
                     ( SS.fLSA[ 1 ] / SS.fLSA[ 0 ] ) * SS.fLSYL[ 0 ] - \
                     ( SS.fLSA[ 2 ] / SS.fLSA[ 0 ] ) * SS.fLSYL[ 1 ];

         // save history
         SS.fLSXL[ 1 ] =  SS.fLSXL[ 0 ];
         SS.fLSXL[ 0 ] =  fInput;
         SS.fLSYL[ 1 ] =  SS.fLSYL[ 0 ];
         SS.fLSYL[ 0 ] =  fHSInput + _FX_DENORMVAL;

         // calculate output
         fOutL =  ( SS.fHSB[ 0 ] / SS.fHSA[ 0 ] ) * fHSInput + \
                  ( SS.fHSB[ 1 ] / SS.fHSA[ 0 ] ) * SS.fHSXL[ 0 ] + \
                  ( SS.fHSB[ 2 ] / SS.fHSA[ 0 ] ) * SS.fHSXL[ 1 ] - \
                  ( SS.fHSA[ 1 ] / SS.fHSA[ 0 ] ) * SS.fHSYL[ 0 ] - \
                  ( SS.fHSA[ 2 ] / SS.fHSA[ 0 ] ) * SS.fHSYL[ 1 ];

         // save history
         SS.fHSXL[ 1 ] =  SS.fHSXL[ 0 ];
         SS.fHSXL[ 0 ] =  fHSInput;
         SS.fHSYL[ 1 ] =  SS.fHSYL[ 0 ];
         SS.fHSYL[ 0 ] =  fOutL + _FX_DENORMVAL;

         fOutL = ( fOutL * fWetOut ) + ( fInput * fDryOut );

         // get input sample (right)
         fInput   =  (F32)(S16)LE_SWAP16_OFS(source_buffer,2);

         // calculate output
         fHSInput =  ( SS.fLSB[ 0 ] / SS.fLSA[ 0 ] ) * fInput + \
                     ( SS.fLSB[ 1 ] / SS.fLSA[ 0 ] ) * SS.fLSXR[ 0 ] + \
                     ( SS.fLSB[ 2 ] / SS.fLSA[ 0 ] ) * SS.fLSXR[ 1 ] - \
                     ( SS.fLSA[ 1 ] / SS.fLSA[ 0 ] ) * SS.fLSYR[ 0 ] - \
                     ( SS.fLSA[ 2 ] / SS.fLSA[ 0 ] ) * SS.fLSYR[ 1 ];

         // save history
         SS.fLSXR[ 1 ] =  SS.fLSXR[ 0 ];
         SS.fLSXR[ 0 ] =  fInput;
         SS.fLSYR[ 1 ] =  SS.fLSYR[ 0 ];
         SS.fLSYR[ 0 ] =  fHSInput + _FX_DENORMVAL;

         // calculate output
         fOutR  =  ( SS.fHSB[ 0 ] / SS.fHSA[ 0 ] ) * fHSInput + \
                   ( SS.fHSB[ 1 ] / SS.fHSA[ 0 ] ) * SS.fHSXR[ 0 ] + \
                   ( SS.fHSB[ 2 ] / SS.fHSA[ 0 ] ) * SS.fHSXR[ 1 ] - \
                   ( SS.fHSA[ 1 ] / SS.fHSA[ 0 ] ) * SS.fHSYR[ 0 ] - \
                   ( SS.fHSA[ 2 ] / SS.fHSA[ 0 ] ) * SS.fHSYR[ 1 ];

         // save history
         SS.fHSXR[ 1 ] =  SS.fHSXR[ 0 ];
         SS.fHSXR[ 0 ] =  fHSInput;
         SS.fHSYR[ 1 ] =  SS.fHSYR[ 0 ];
         SS.fHSYR[ 0 ] =  fOutR + _FX_DENORMVAL;

         fOutR = ( fOutR * fWetOut ) + ( fInput * fDryOut );

         // write output
         S32 tmp;
         WRITE_STEREO_SAMPLE( dest_buffer, fOutL, fOutR, tmp );

         source_buffer += 2;
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

         // calculate output
         fHSInput =  ( SS.fLSB[ 0 ] / SS.fLSA[ 0 ] ) * fInput + \
                     ( SS.fLSB[ 1 ] / SS.fLSA[ 0 ] ) * SS.fLSXL[ 0 ] + \
                     ( SS.fLSB[ 2 ] / SS.fLSA[ 0 ] ) * SS.fLSXL[ 1 ] - \
                     ( SS.fLSA[ 1 ] / SS.fLSA[ 0 ] ) * SS.fLSYL[ 0 ] - \
                     ( SS.fLSA[ 2 ] / SS.fLSA[ 0 ] ) * SS.fLSYL[ 1 ];

         // save history
         SS.fLSXL[ 1 ] =  SS.fLSXL[ 0 ];
         SS.fLSXL[ 0 ] =  fInput;
         SS.fLSYL[ 1 ] =  SS.fLSYL[ 0 ];
         SS.fLSYL[ 0 ] =  fHSInput + _FX_DENORMVAL;

         // calculate output
         fOutL =  ( SS.fHSB[ 0 ] / SS.fHSA[ 0 ] ) * fHSInput + \
                  ( SS.fHSB[ 1 ] / SS.fHSA[ 0 ] ) * SS.fHSXL[ 0 ] + \
                  ( SS.fHSB[ 2 ] / SS.fHSA[ 0 ] ) * SS.fHSXL[ 1 ] - \
                  ( SS.fHSA[ 1 ] / SS.fHSA[ 0 ] ) * SS.fHSYL[ 0 ] - \
                  ( SS.fHSA[ 2 ] / SS.fHSA[ 0 ] ) * SS.fHSYL[ 1 ];

         // save history
         SS.fHSXL[ 1 ] =  SS.fHSXL[ 0 ];
         SS.fHSXL[ 0 ] =  fHSInput;
         SS.fHSYL[ 1 ] =  SS.fHSYL[ 0 ];
         SS.fHSYL[ 0 ] =  fOutL + _FX_DENORMVAL;

         fOutL = ( fOutL * fWetOut ) + ( fInput * fDryOut );

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

      case  _FX_MIX       :
       if ( before_value ) *(F32 FAR*)before_value = SS->fMix;
       if ( new_value ) 
       {
         SS->fMix = *(F32 const FAR*)new_value;
         // clip to valid range
         FX_CLIPRANGE( SS->fMix, 0.0F, 1.0F );
       }         
       if ( after_value ) *(F32 FAR*)after_value = SS->fMix;
       return 1;

      case  _FX_SHELFEQ_LOWFREQ   :
       if ( before_value ) *(F32 FAR*)before_value = SS->fLowShelfFreq;
       if ( new_value ) 
       {
         SS->fLowShelfFreq = *(F32 const FAR*)new_value;
         // clip to valid range
         FX_CLIPRANGE( SS->fLowShelfFreq, 20.0F, ((F32) SS->driver->dig->DMA_rate) / 2.0F - 1.0F );
         FXCalcParams(SS, SS->driver->dig->DMA_rate);
       }         
       if ( after_value ) *(F32 FAR*)after_value = SS->fLowShelfFreq;
       return 1;

      case  _FX_SHELFEQ_LOWGAIN   :
       if ( before_value ) *(F32 FAR*)before_value = SS->fLowShelfGain;
       if ( new_value ) 
       {
         SS->fLowShelfGain = *(F32 const FAR*)new_value;
         // clip to valid range
         FX_CLIPRANGE( SS->fLowShelfGain, -20.0F, 20.0f );
         FXCalcParams(SS, SS->driver->dig->DMA_rate);
       }         
       if ( after_value ) *(F32 FAR*)after_value = SS->fLowShelfGain;
       return 1;

      case  _FX_SHELFEQ_HIGHFREQ   :
       if ( before_value ) *(F32 FAR*)before_value = SS->fHighShelfFreq;
       if ( new_value ) 
       {
         SS->fHighShelfFreq = *(F32 const FAR*)new_value;
         // clip to valid range
         FX_CLIPRANGE( SS->fHighShelfFreq, 20.0F, ((F32) SS->driver->dig->DMA_rate) / 2.0F - 1.0F );
         FXCalcParams(SS, SS->driver->dig->DMA_rate);
       }         
       if ( after_value ) *(F32 FAR*)after_value = SS->fHighShelfFreq;
       return 1;

      case  _FX_SHELFEQ_HIGHGAIN   :
       if ( before_value ) *(F32 FAR*)before_value = SS->fHighShelfGain;
       if ( new_value ) 
       {
         SS->fHighShelfGain = *(F32 const FAR*)new_value;
         // clip to valid range
         FX_CLIPRANGE( SS->fHighShelfGain, -20.0F, 20.0f );
         FXCalcParams(SS, SS->driver->dig->DMA_rate);
       }         
       if ( after_value ) *(F32 FAR*)after_value = SS->fHighShelfGain;
       return 1;

      }

   return 0;
}

extern "C" S32 ShelfEqMain( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 ShelfEqMain( HPROVIDER provider_handle, U32 up_down )
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
      REG_PR("Mix",                  _FX_MIX,              RIB_FLOAT),
      REG_PR("ShelvingEQ LowFreq",   _FX_SHELFEQ_LOWFREQ,  RIB_FLOAT),
      REG_PR("ShelvingEQ LowGain",   _FX_SHELFEQ_LOWGAIN,  RIB_FLOAT),
      REG_PR("ShelvingEQ HighFreq",  _FX_SHELFEQ_HIGHFREQ, RIB_FLOAT),
      };

   const RIB_INTERFACE_ENTRY FLTSMP3[] =
      {
      REG_PR("ShelvingEQ HighGain",  _FX_SHELFEQ_HIGHGAIN, RIB_FLOAT),
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
