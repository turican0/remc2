//############################################################################
//##                                                                        ##
//##  Miles Sound System                                                    ##
//##                                                                        ##
//##  API.CPP: FLT module for Volume Ramper                                 ##
//##                                                                        ##
//##  32-bit protected-mode source compatible with MSC 11.0/Watcom 10.6     ##
//##                                                                        ##
//##  Version 1.00 of 5-Dec-04: Initial                                     ##
//##                                                                        ##
//##  Author: Jeff Roberts                                                  ##
//##                                                                        ##
//############################################################################
//##                                                                        ##
//##  Copyright (C) RAD Game Tools, Inc.                                    ##
//##                                                                        ##
//##  Contact RAD Game Tools at 425-322-4300 for technical support.         ##
//##                                                                        ##
//############################################################################

#include "mss.h"
#include "imssapi.h"

#define FILTER_NAME "Volume Ramp Filter"

//
// Attribute tokens
//

enum PROP
{
   //
   // Additional filter attribs (beyond standard RIB PROVIDER_ attributes)
   //

   _FX_PROVIDER_FLAGS,

   _FX_RAMP_TIME,
   _FX_RAMP_TO,
   _FX_RAMP_AT
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

   F32    time;

   F32    current_level;
   F32    to_level;
};

#include "common.inl"

static void init_sample( SAMPLESTATE FAR * SS )
{
   //
   // Initialize provider-specific members to their default values
   //

   SS->time          = 0.0F;
   SS->current_level = 1.0F;
   SS->to_level      = 1.0F;
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
   SAMPLESTATE FAR *SS  = (SAMPLESTATE FAR *) state;
   //HSAMPLE          S   = SS->sample;
   //DRIVERSTATE FAR *DRV = SS->driver;

   S32 dwIndex;
   F32 fOutL, fOutR;

   F32 current = SS->current_level;
   F32 to      = SS->to_level;

   if (SS->time <= 0.00001f)
      {
      current = to;
      SS->current_level = to;
      SS->time = 0.0f;
      }
     
   //fast path
   if ( ( current > 0.9999f ) && ( to > 0.9999f ) )
   {
     if ( source_buffer != dest_buffer )
       AIL_memcpy( dest_buffer, source_buffer, n_samples * ( is_stereo ? 4 : 2 ) );
     return;
   }

   fOutL = to - current;

   if ( ( fOutL < 0.0001f ) && ( fOutL > -0.0001f ) )
      {
      if ( is_stereo )
         {
         for ( dwIndex = 0; dwIndex < n_samples; dwIndex++ )
            {
            fOutL = ( (F32) (S16)LE_SWAP16(source_buffer) ) * current;
            fOutR = ( (F32) (S16)LE_SWAP16_OFS(source_buffer,2) ) * current;

            S32 tmp;
            WRITE_STEREO_SAMPLE( dest_buffer, fOutL, fOutR, tmp );

            source_buffer += 2;
            }
         }
      else
         {
         for ( dwIndex = 0; dwIndex < n_samples; dwIndex++ )
            {
            fOutL = ( (F32) (S16)LE_SWAP16(source_buffer) ) * current;
            ++source_buffer;

            // store output
            S32 tmp;
            WRITE_MONO_SAMPLE( dest_buffer, fOutL, tmp );
            }
         }
      }
   else
      {
      F32 adj = (( to - current ) * 1000.0F) / (SS->time * (F32) dest_playback_rate);

      if ( is_stereo )
         {
         if ( adj < 0.0f )
            {
            for ( dwIndex = 0; dwIndex < n_samples; dwIndex++ )
               {
               current += adj;  if ( current < to ) current = to;

               fOutL = ( (F32) (S16)LE_SWAP16(source_buffer) ) * current;
               fOutR = ( (F32) (S16)LE_SWAP16_OFS(source_buffer,2) ) * current;

               S32 tmp;
               WRITE_STEREO_SAMPLE( dest_buffer, fOutL, fOutR, tmp );

               source_buffer += 2;
               }
            }
         else
            {
            for ( dwIndex = 0; dwIndex < n_samples; dwIndex++ )
               {
               current += adj;  if ( current > to ) current = to;

               fOutL = ( (F32) (S16)LE_SWAP16(source_buffer) ) * current;
               fOutR = ( (F32) (S16)LE_SWAP16_OFS(source_buffer,2) ) * current;

               S32 tmp;
               WRITE_STEREO_SAMPLE( dest_buffer, fOutL, fOutR, tmp );

               source_buffer += 2;
               }
            }

         SS->time -= (F32) (n_samples * 500) / dest_playback_rate;
         }
      else
         {
         if ( adj < 0.0f )
            {
            for ( dwIndex = 0; dwIndex < n_samples; dwIndex++ )
               {
               current += adj;  if ( current < to ) current = to;

               fOutL = ( (F32) (S16)LE_SWAP16(source_buffer) ) * current;
               ++source_buffer;

               // store output
               S32 tmp;
               WRITE_MONO_SAMPLE( dest_buffer, fOutL, tmp );
               }
            }
         else
            {
            for ( dwIndex = 0; dwIndex < n_samples; dwIndex++ )
               {
               current += adj;  if ( current > to ) current = to;

               fOutL = ( (F32) (S16)LE_SWAP16(source_buffer) ) * current;
               ++source_buffer;

               // store output
               S32 tmp;
               WRITE_MONO_SAMPLE( dest_buffer, fOutL, tmp );
               }
            }        

         SS->time -= (F32) ( n_samples * 1000 ) / dest_playback_rate;
         }
     
      if ( SS->time < 0.0f )
         {
         SS->time = 0.0f;
         }

      if ( SS->current_level < to )
         {
         if ( current > to )
            SS->current_level = to;
         else
            SS->current_level = current;     
         }
      else
         {
         if ( current < to )
            SS->current_level = to;
         else
            SS->current_level = current;     
         }
      }
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
      case  _FX_RAMP_TIME  :
       if ( before_value ) *(F32 FAR*)before_value = SS->time;
       if ( new_value ) 
       {
         SS->time = *(F32 const FAR*)new_value;
         // clip to valid range
         if ( SS->time <= 0.00001f )
           SS->time = 0.0f;
       }         
       if ( after_value ) *(F32 FAR*)after_value = SS->time;
       return 1;

      case  _FX_RAMP_TO   :
       if ( before_value ) *(F32 FAR*)before_value = SS->to_level;
       if ( new_value ) 
       {
         SS->to_level = *(F32 const FAR*)new_value;
         // clip to valid range
         FX_CLIPRANGE( SS->to_level, 0.0F, 1.0f );
         if ( SS->time <= 0.00001f )
           SS->current_level = SS->to_level;
       }         
       if ( after_value ) *(F32 FAR*)after_value = SS->to_level;
       return 1;

      case  _FX_RAMP_AT   :
       if ( before_value ) 
       {
         *(F32 FAR*)before_value = SS->current_level;
         return 1;
       }
       break;
      }

   return 0;
}

extern "C" S32 RampMain( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 RampMain( HPROVIDER provider_handle, U32 up_down )
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
      REG_PR("Ramp Time",  _FX_RAMP_TIME, RIB_FLOAT),
      REG_PR("Ramp At",    _FX_RAMP_AT,   (RIB_DATA_SUBTYPE) (RIB_FLOAT|RIB_READONLY) ),
      REG_PR("Ramp To",    _FX_RAMP_TO,   RIB_FLOAT),
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

