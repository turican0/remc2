//############################################################################
//##                                                                        ##
//##  Miles Sound System                                                    ##
//##                                                                        ##
//##  API.CPP: FLT module for Chorus                                        ##
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

#define FILTER_NAME "Chorus Filter"

// Chorus / Flange Buffer
#define  _FX_CHORUS_BUFFER    4096L

// Chorus Defaults
#define  _FX_FEEDBACK_DEFAULT    0.0F
#define  _FX_RATE_DEFAULT        11.0F
#define  _FX_DEPTH_DEFAULT       20.0F
#define  _FX_MIX_DEFAULT         0.5F
#define  _FX_DELAY_DEFAULT       20.0F

//
// Attribute tokens
//

enum PROP
{
   //
   // Additional filter attribs (beyond standard RIB PROVIDER_ attributes)
   //

   _FX_PROVIDER_FLAGS,

   _FX_CHORUS_FEEDBACK,
   _FX_CHORUS_RATE,
   _FX_CHORUS_DEPTH,
   _FX_CHORUS_MIX,
   _FX_CHORUS_DELAY
};

//
// Driver state descriptor
//
// One state descriptor is associated with each HDIGDRIVER
//

struct DRIVERSTATE
{
   //
   // Members common to all pipeline filter providers
   //

   HDIGDRIVER dig;                  // Driver with which this descriptor is associated
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

   F32       fMix;
   F32       fDepth;
   F32       fSweepRate;
   F32       fFeedback;
   F32       fDelay;

   F32 *     pfDelayBuffer;

   S32         wFp;
   S32         wEp1;
   S32         wEp2;
   S32         wStep;
   S32         wMinSweep;
   S32         wMaxSweep;
   S32         wDepth;
   S32         wDelay;
   U32       dwSweep;
   F32       fIFrac;
};

//############################################################################
//#                                                                          #
//# Calculate coefficients and values based on parameter set                 #
//#                                                                          #
//############################################################################

static void  FXCalcParams( SAMPLESTATE FAR * SS, S32 sample_rate )
{
   F32 fRate;

   // get sample rate
   fRate = F32(sample_rate);
   SS->calculated_rate = sample_rate;

   // set parameters
   SS->wStep   =  (int)( SS->fSweepRate * 65.536F );
   SS->wDepth  =  (int)( SS->fDepth * ( fRate / 1000.0F ) );
   SS->wDelay  =  (int)( SS->fDelay * ( fRate / 1000.0F ) );

   // set min/max sweep
   SS->wMaxSweep   =  _FX_CHORUS_BUFFER - 4 - SS->wDelay;
   SS->wMinSweep   =  SS->wMaxSweep   -  SS->wDepth;

   // clip
   if ( SS->wMinSweep < 0 )
      SS->wMinSweep  =  0;

   // set sweep
   SS->dwSweep    =  (( SS->wMinSweep + SS->wMaxSweep ) / 2 ) << 16;

   // set fraction
   SS->fIFrac     =  65536.0F;
}


static void init_sample( SAMPLESTATE FAR * SS )
{
   //
   // Initialize provider-specific members to their default values
   //

   // set defaults
   SS->fFeedback     =  _FX_FEEDBACK_DEFAULT;
   SS->fSweepRate    =  _FX_RATE_DEFAULT;
   SS->fDepth        =  _FX_DEPTH_DEFAULT;
   SS->fMix          =  _FX_MIX_DEFAULT;
   SS->fDelay        =  _FX_DELAY_DEFAULT;

   // allocate delay buffer 
   SS->pfDelayBuffer =  (F32 *)AIL_mem_alloc_lock( _FX_CHORUS_BUFFER * sizeof( F32 ));

   // clear buffer
   AIL_memset( SS->pfDelayBuffer, 0, _FX_CHORUS_BUFFER * sizeof( F32 ));

   // update params
   FXCalcParams( SS, SS->driver->dig->DMA_rate );

   // init pointers
   SS->wFp   =  0;
   SS->wEp1  =  0;
   SS->wEp2  =  0;
}

static void close_sample( SAMPLESTATE FAR * SS )
{
   if (SS->pfDelayBuffer != NULL)
      {
      AIL_mem_free_lock(SS->pfDelayBuffer);
      SS->pfDelayBuffer = NULL;
      }
}

#include "common.inl"

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
   F32 fInputL, fInputR;
   F32 fOutL, fOutR;
   F32 fDryOut;
   F32 fWetOut;

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
         fInputL   =  (F32)(S16)LE_SWAP16(source_buffer);
         fInputR   =  (F32)(S16)LE_SWAP16_OFS(source_buffer,2);

         // calculate interpolation
         fOutL    =  ( (F32)SS.pfDelayBuffer[ SS.wEp1 ] * (F32)( SS.dwSweep >> 16 ) +
                        (F32)SS.pfDelayBuffer[ SS.wEp2 ] * ( SS.fIFrac - (F32)( SS.dwSweep >> 16 ))) /
                        SS.fIFrac;

         // set output sample
         SS.pfDelayBuffer[ SS.wFp ]  =  (F32)( fInputL + fInputR + ( fOutL * SS.fFeedback ) + _FX_DENORMVAL);

         // calculate new output
         fOutR  =  ( fOutL * fWetOut ) + ( fInputR * fDryOut );
         fOutL  =  ( fOutL * fWetOut ) + ( fInputL * fDryOut );

         // update pointers
         SS.wFp   =  ( SS.wFp + 1 ) & ( _FX_CHORUS_BUFFER - 1 );

         // adjust sweep
         SS.dwSweep  += SS.wStep;

         // calculate buffer positions
         SS.wEp1  =  ( SS.wFp + ( SS.dwSweep >> 16 ) ) & ( _FX_CHORUS_BUFFER - 1 );
         SS.wEp2  =  ( SS.wEp1 - 1 ) & ( _FX_CHORUS_BUFFER - 1 );

         // check for sweep reversal
         if ( (S32)SS.dwSweep >> 16 >= SS.wMaxSweep )
            SS.wStep =  -SS.wStep;
         else
            if ( (S32)SS.dwSweep >> 16 <= SS.wMinSweep )
               SS.wStep =  -SS.wStep;

         // store output
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
         fInputL   =  (F32)(S16)LE_SWAP16(source_buffer);
         ++source_buffer;

         // calculate interpolation
         fOutL    =  ( (F32)SS.pfDelayBuffer[ SS.wEp1 ] * (F32)( SS.dwSweep >> 16 ) +
                        (F32)SS.pfDelayBuffer[ SS.wEp2 ] * ( SS.fIFrac - (F32)( SS.dwSweep >> 16 ))) /
                        SS.fIFrac;

         // set output sample
         SS.pfDelayBuffer[ SS.wFp ]  =  (F32)( fInputL + ( fOutL * SS.fFeedback ) + _FX_DENORMVAL);

         // calculate new output
         fOutL  =  ( fOutL * fWetOut ) + ( fInputL * fDryOut );

         // update pointers
         SS.wFp   =  ( SS.wFp + 1 ) & ( _FX_CHORUS_BUFFER - 1 );

         // adjust sweep
         SS.dwSweep  += SS.wStep;

         // calculate buffer positions
         SS.wEp1  =  ( SS.wFp + ( SS.dwSweep >> 16 ) ) & ( _FX_CHORUS_BUFFER - 1 );
         SS.wEp2  =  ( SS.wEp1 - 1 ) & ( _FX_CHORUS_BUFFER - 1 );

         // check for sweep reversal
         if ( (S32)SS.dwSweep >> 16 >= SS.wMaxSweep )
            SS.wStep =  -SS.wStep;
         else
            if ( (S32)SS.dwSweep >> 16 <= SS.wMinSweep )
               SS.wStep =  -SS.wStep;

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
      case  _FX_CHORUS_MIX       :
       if ( before_value ) *(F32 FAR*)before_value = SS->fMix;
       if ( new_value ) 
       {
         SS->fMix = *(F32 const FAR*)new_value;
         // clip to valid range
         FX_CLIPRANGE( SS->fMix, 0.0F, 1.0F );
       }         
       if ( after_value ) *(F32 FAR*)after_value = SS->fMix;
       return 1;

      case  _FX_CHORUS_DEPTH        :
       if ( before_value ) *(F32 FAR*)before_value = SS->fDepth;
       if ( new_value ) 
       {
         SS->fDepth = *(F32 const FAR*)new_value;
         // clip to valid range
         FX_CLIPRANGE( SS->fDepth, 1.0F, 1000.0F );
         FXCalcParams( SS, SS->calculated_rate);
       }         
       if ( after_value ) *(F32 FAR*)after_value = SS->fDepth;
       return 1;

      case  _FX_CHORUS_RATE        :
       if ( before_value ) *(F32 FAR*)before_value = SS->fSweepRate;
       if ( new_value ) 
       {
         SS->fSweepRate = *(F32 const FAR*)new_value;
         // clip to valid range
         FX_CLIPRANGE( SS->fSweepRate, 0.01F, 50.0F );
         FXCalcParams( SS, SS->calculated_rate);
       }         
       if ( after_value ) *(F32 FAR*)after_value = SS->fSweepRate;
       return 1;

      case  _FX_CHORUS_FEEDBACK     :
       if ( before_value ) *(F32 FAR*)before_value = SS->fFeedback;
       if ( new_value ) 
       {
         SS->fFeedback = *(F32 const FAR*)new_value;
         // clip to valid range
         FX_CLIPRANGE( SS->fFeedback, -0.9F, 0.9F );
       }         
       if ( after_value ) *(F32 FAR*)after_value = SS->fFeedback;
       return 1;

      case  _FX_CHORUS_DELAY     :
       if ( before_value ) *(F32 FAR*)before_value = SS->fDelay;
       if ( new_value ) 
       {
         SS->fDelay = *(F32 const FAR*)new_value;
         // clip to valid range
         FX_CLIPRANGE( SS->fDelay, 0.0F, 200.0F );
         FXCalcParams( SS, SS->calculated_rate);
       }         
       if ( after_value ) *(F32 FAR*)after_value = SS->fDelay;
       return 1;
      }

   return 0; 
}

extern "C" S32 ChorusMain( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 ChorusMain( HPROVIDER provider_handle, U32 up_down )
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
      REG_FN(FLTSMP_sample_property),
      REG_PR("Mix",             _FX_CHORUS_MIX,      RIB_FLOAT),
      REG_PR("Chorus Depth",    _FX_CHORUS_DEPTH,    RIB_FLOAT),
      REG_PR("Chorus Rate",     _FX_CHORUS_RATE,     RIB_FLOAT),
      };

   const RIB_INTERFACE_ENTRY FLTSMP3[] =
      {
      REG_PR("Chorus Feedback", _FX_CHORUS_FEEDBACK, RIB_FLOAT),
      REG_PR("Chorus Delay",    _FX_CHORUS_DELAY,    RIB_FLOAT),
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

