//############################################################################
//##                                                                        ##
//##  Miles Sound System                                                    ##
//##                                                                        ##
//##  API.CPP: FLT module for Ring/Amplutide Modulator                      ##
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

#define FILTER_NAME "Ring Modulator Filter"

// Ringmodulator
#define  _FX_FREQ_DEFAULT        16.0f
#define  _FX_PHASE_LEFT          0
#define  _FX_PHASE_RIGHT         4
#define  _FX_MIX_DEFAULT         0.5f

//
// Attribute tokens
//

enum PROP
{
   //
   // Additional filter attribs (beyond standard RIB PROVIDER_ attributes)
   //

   _FX_PROVIDER_FLAGS,

   _FX_RINGMOD_FREQ,
   _FX_RINGMOD_PHASE_LEFT,
   _FX_RINGMOD_PHASE_RIGHT,
   _FX_RINGMOD_MIX
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

   F32    fFrequency;
   F32    fMix;

   // sample index
   F32    fPhaseFreq;
   F32    fPhaseAdd;
   S32    iPhaseAdd;
   F32    fPhaseL, fPhaseR;
   S32    iPhaseL, iPhaseR;
};

#include "common.inl"

#define MAXLUF 16.0f
#define MAXLU 16

static F32 sintable[ MAXLU + 1 ];
static S32 do_init = 1;

//############################################################################
//#                                                                          #
//# Calculate coefficients and values based on parameter set                 #
//#                                                                          #
//############################################################################

static void  FXCalcParams( SAMPLESTATE FAR * SS, S32 sample_rate )
{
   F32 fPhaseAdd, fRate = F32(sample_rate);
   SS->calculated_rate = sample_rate;

   fPhaseAdd = ( MAXLUF * SS->fFrequency / fRate );

   SS->iPhaseAdd = (S32) AIL_floor(fPhaseAdd);
   SS->fPhaseAdd = fPhaseAdd - (F32)SS->iPhaseAdd;
}

#ifdef IS_MAC
#define sinf sin
#endif

static void init_sample( SAMPLESTATE FAR * SS )
{
   //
   // Initialize provider-specific members to their default values
   //

   // init default values
   SS->fFrequency    =  _FX_FREQ_DEFAULT;
   SS->iPhaseL       =  _FX_PHASE_LEFT;
   SS->iPhaseR       =  _FX_PHASE_RIGHT;
   SS->fPhaseL       =  0;
   SS->fPhaseR       =  0;
   SS->fMix          =  _FX_MIX_DEFAULT;

   // update params
   FXCalcParams( SS, SS->driver->dig->DMA_rate );

   if (do_init)
   {
     S32 i;
     do_init = 0;
     for ( i = 0 ; i < ( MAXLU + 1 ) ; i++ )
     {
       sintable[ i ] = (F32) sinf( ( 2.0f * F_PI * (F32) i ) / MAXLUF );
     }
   }
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

   S32      dwIndex;
   F32      fInput;
   F32      fOutL,fOutR;
   F32      fDryOut;
   F32      fWetOut;

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

   // check if mono or stereo
   if ( is_stereo )
   {
      // mix into build buffer
      for( dwIndex = 0; dwIndex < n_samples; dwIndex++ )
      {
         // get input sample (left)
         fInput   =  (F32)(S16)LE_SWAP16(source_buffer);

         // modulate input   fracpor * ( n - f ) + f
         fOutL   = fInput * 
                   ( SS.fPhaseL * ( sintable[ SS.iPhaseL + 1 ] - sintable[ SS.iPhaseL ] ) +
                     sintable[ SS.iPhaseL ] );

         SS.iPhaseL += SS.iPhaseAdd;
         SS.fPhaseL += SS.fPhaseAdd;
         if ( SS.fPhaseL >= 1.0f )
         {
           SS.fPhaseL -= 1.0f;
           ++SS.iPhaseL;
         }
         SS.iPhaseL &= ( MAXLU-1 );

         // left
         fOutL  =  ( fInput * fDryOut ) +
                    ( fOutL * fWetOut );

         // get input sample (right)
         fInput   =  (F32)(S16)LE_SWAP16_OFS(source_buffer,2);

         SS.iPhaseR += SS.iPhaseAdd;
         SS.fPhaseR += SS.fPhaseAdd;
         if ( SS.fPhaseR >= 1.0f )
         {
           SS.fPhaseR -= 1.0f;
           ++SS.iPhaseR;
         }
         SS.iPhaseR &= ( MAXLU-1 );

         // modulate input   fracpor * ( n - f ) + f
         fOutR   = fInput * 
                   ( SS.fPhaseR * ( sintable[ SS.iPhaseR + 1 ] - sintable[ SS.iPhaseR ] ) +
                     sintable[ SS.iPhaseR ] );
         // right
         fOutR   =  ( fInput * fDryOut ) +
                     ( fOutR * fWetOut );

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

         // modulate input   fracpor * ( n - f ) + f
         fOutL   = fInput * 
                   ( SS.fPhaseL * ( sintable[ SS.iPhaseL + 1 ] - sintable[ SS.iPhaseL ] ) +
                     sintable[ SS.iPhaseL ] );

         SS.iPhaseL += SS.iPhaseAdd;
         SS.fPhaseL += SS.fPhaseAdd;
         if ( SS.fPhaseL >= 1.0f )
         {
           SS.fPhaseL -= 1.0f;
           ++SS.iPhaseL;
         }
         SS.iPhaseL &= ( MAXLU-1 );

         // left
         fOutL  =  ( fInput * fDryOut ) +
                    ( fOutL * fWetOut );

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

   F32 phase;

   // determine preference
   switch (property)
      {
      //
      // Preferences
      //
      case  _FX_RINGMOD_FREQ   :
       if ( before_value ) *(F32 FAR*)before_value = SS->fFrequency;
       if ( new_value ) 
       {
         SS->fFrequency = *(F32 const FAR*)new_value;
         // clip to valid range
         FX_CLIPRANGE( SS->fFrequency, 20.0F, ((F32) SS->driver->dig->DMA_rate) / 2.0F - 1.0F );
         FXCalcParams(SS, SS->driver->dig->DMA_rate);
       }         
       if ( after_value ) *(F32 FAR*)after_value = SS->fFrequency;
       return 1;

      case  _FX_RINGMOD_PHASE_LEFT   :
       if ( before_value ) 
       {
         phase = ( (F32) SS->iPhaseL ) + SS->fPhaseL;
         phase = ( phase / MAXLUF ) * 2.0f * F_PI;
         *(F32 FAR*)before_value = phase;
       }
       
       if ( new_value ) 
       {
         phase = *(F32 const FAR*)new_value;
         // clip width to valid range
         FX_CLIPRANGE( phase, 0.0f, 2.0F*F_PI );
         phase = ( phase / ( 2.0f * F_PI ) ) * MAXLUF;
         SS->iPhaseL = (S32) AIL_floor(phase);
         SS->fPhaseL = phase - (F32)SS->iPhaseL;

         FXCalcParams(SS, SS->driver->dig->DMA_rate);
       }         
       
       if ( after_value ) 
       {
         phase = ( (F32) SS->iPhaseL ) + SS->fPhaseL;
         phase = ( phase / MAXLUF ) * 2.0f * F_PI;
         *(F32 FAR*)after_value = phase;
       }
       return 1;

      case  _FX_RINGMOD_PHASE_RIGHT   :
       if ( before_value ) 
       {
         phase = ( (F32) SS->iPhaseR ) + SS->fPhaseR;
         phase = ( phase / MAXLUF ) * 2.0f * F_PI;
         *(F32 FAR*)before_value = phase;
       }
       
       if ( new_value ) 
       {
         phase = *(F32 const FAR*)new_value;
         // clip width to valid range
         FX_CLIPRANGE( phase, 0.0f, 2.0F*F_PI );
         phase = ( phase / ( 2.0f * F_PI ) ) * MAXLUF;
         SS->iPhaseR = (S32) AIL_floor(phase);
         SS->fPhaseR = phase - (F32)SS->iPhaseR;

         FXCalcParams(SS, SS->driver->dig->DMA_rate);
       }         
       
       if ( after_value ) 
       {
         phase = ( (F32) SS->iPhaseR ) + SS->fPhaseR;
         phase = ( phase / MAXLUF ) * 2.0f * F_PI;
         *(F32 FAR*)after_value = phase;
       }
       return 1;

      case  _FX_RINGMOD_MIX   :
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

extern "C" S32 RingModMain( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 RingModMain( HPROVIDER provider_handle, U32 up_down )
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
      REG_PR("Ring Mod Freq",        _FX_RINGMOD_FREQ, RIB_FLOAT),
      REG_PR("Ring Mod Phase Left",  _FX_RINGMOD_PHASE_LEFT, RIB_FLOAT),
      REG_PR("Ring Mod Phase Right", _FX_RINGMOD_PHASE_RIGHT, RIB_FLOAT),
      REG_PR("Mix",                  _FX_RINGMOD_MIX,  RIB_FLOAT),
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
