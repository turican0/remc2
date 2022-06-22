//############################################################################
//##                                                                        ##
//##  Miles Sound System                                                    ##
//##                                                                        ##
//##  API.CPP: FLT module for Parametric EQ                                 ##
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

#define FILTER_NAME "Resonator EQ Filter"

// Resonator Defaults
#define  _FX_DEFAULT_CENTER      1000.0F
#define  _FX_DEFAULT_WIDTH       200.0F
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
   _FX_RESONATOR_CENTER,
   _FX_RESONATOR_WIDTH
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

   F32    fMix;
   F32    fCenterFreq;
   F32    fBandWidth;

   F32    fA0;
   F32    fB1;
   F32    fB2;

   F32    fYL1;
   F32    fYL2;
   F32    fYR1;
   F32    fYR2;
};

#include "common.inl"

static void FXCalcParams( SAMPLESTATE FAR * SS, S32 playback_rate )
{
   // get sample rate
   F32 fRate = F32(playback_rate);
   SS->calculated_rate = playback_rate;

   // calculate coefficients
   SS->fB2  =  (F32) AIL_pow(2.71828F, - ( 2.0F * F_PI ) * ( SS->fBandWidth / fRate ));
   SS->fB1  =  ( -4.0F * SS->fB2 ) / ( 1.0F + SS->fB2 ) * \
                        (F32) AIL_cos( 2.0F * F_PI * ( SS->fCenterFreq / fRate ));
   SS->fA0  =  ( 1.0F - SS->fB2 ) * AIL_fsqrt( 1.0F - ( SS->fB1 * SS->fB1 ) / ( 4.0F * SS->fB2 ));
}

static void init_sample( SAMPLESTATE FAR * SS )
{
   //
   // Initialize provider-specific members to their default values
   //

   SS->fMix          =  _FX_MIX_DEFAULT;
   SS->fCenterFreq   =  _FX_DEFAULT_CENTER;
   SS->fBandWidth    =  _FX_DEFAULT_WIDTH;

   // calculate params
   FXCalcParams( SS, SS->driver->dig->DMA_rate );

   // reset sample history
   SS->fYL1     =  0.0F;
   SS->fYL2     =  0.0F;
   SS->fYR1     =  0.0F;
   SS->fYR2     =  0.0F;
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

         // calculate sample
         fOutL    =  SS.fA0 * fInput - SS.fB1 * SS.fYL1 \
                     - SS.fB2 * SS.fYL2;

         // save samples
         SS.fYL2  =  SS.fYL1;
         SS.fYL1  =  fOutL + _FX_DENORMVAL;

         fOutL = ( fOutL * fWetOut ) + ( fInput * fDryOut );

         // get input sample (right)
         fInput   =  (F32)(S16)LE_SWAP16_OFS(source_buffer,2);

         // calculate sample
         fOutR    =  SS.fA0 * fInput - SS.fB1 * SS.fYR1 \
                     - SS.fB2 * SS.fYR2;

         // save samples
         SS.fYR2  =  SS.fYR1;
         SS.fYR1  =  fOutR + _FX_DENORMVAL;

         fOutR = ( fOutR * fWetOut ) + ( fInput * fDryOut );

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

         // calculate sample
         fOutL    =  SS.fA0 * fInput - SS.fB1 * SS.fYL1 \
                     - SS.fB2 * SS.fYL2;

         // save samples
         SS.fYL2  =  SS.fYL1;
         SS.fYL1  =  fOutL + _FX_DENORMVAL;

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
   SAMPLESTATE FAR *SS   = (SAMPLESTATE FAR *) state;
   //HSAMPLE          S    = SS->sample;

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

      case  _FX_RESONATOR_CENTER    :
       if ( before_value ) *(F32 FAR*)before_value = SS->fCenterFreq;
       if ( new_value ) 
       {
         SS->fCenterFreq = *(F32 const FAR*)new_value;
         // clip to valid range
         FX_CLIPRANGE( SS->fCenterFreq, 20.0F, ((F32) SS->driver->dig->DMA_rate) / 2.0F - 1.0F );
         FXCalcParams(SS, SS->driver->dig->DMA_rate);
       }         
       if ( after_value ) *(F32 FAR*)after_value = SS->fCenterFreq;
       return 1;

      case  _FX_RESONATOR_WIDTH    :
       if ( before_value ) *(F32 FAR*)before_value = SS->fBandWidth;
       if ( new_value ) 
       {
         SS->fBandWidth = *(F32 const FAR*)new_value;
         // clip to valid range
         FX_CLIPRANGE( SS->fBandWidth, 20.0F, ((F32) SS->driver->dig->DMA_rate) / 2.0F - 1.0F );
         FXCalcParams(SS, SS->driver->dig->DMA_rate);
       }         
       if ( after_value ) *(F32 FAR*)after_value = SS->fBandWidth;
       return 1;
      }

   return 0;
}

extern "C" S32 ResonMain( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 ResonMain( HPROVIDER provider_handle, U32 up_down )
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
      REG_PR("Mix",      _FX_MIX,      RIB_FLOAT),
      REG_PR("Resonator Center", _FX_RESONATOR_CENTER ,  RIB_FLOAT),
      REG_PR("Resonator Width",  _FX_RESONATOR_WIDTH  ,  RIB_FLOAT),
      };

   if (up_down)
      {
         RIB_register(provider_handle, "MSS pipeline filter", FLT1);
         RIB_register(provider_handle, "MSS pipeline filter", FLT2);
         RIB_register(provider_handle, "MSS pipeline filter", FLT3);

         RIB_register(provider_handle, "Pipeline filter sample services", FLTSMP1);
         RIB_register(provider_handle, "Pipeline filter sample services", FLTSMP2);
      }
   else
      {
         RIB_unregister_all(provider_handle);
      }

   return TRUE;
}
