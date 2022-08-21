//############################################################################
//##                                                                        ##
//##  FILTER.CPP: MSS pipeline filter test bed                              ##
//##                                                                        ##
//##  V1.00 of 23-Aug-98: Initial version                                   ##
//##                                                                        ##
//##  Author: Jeff Roberts                                                  ##
//##                                                                        ##
//############################################################################
//##                                                                        ##
//##  Copyright (C) RAD Game Tools, Inc.                                    ##
//##                                                                        ##
//##  For technical support, contact RAD Game Tools at 425-893-4300.        ##
//##                                                                        ##
//############################################################################

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#include "mss.h"

#include "con_util.i"

// @cdep pre $DefaultsConsoleMSS
// @cdep post $BuildConsoleMSS

#define DIGITALCOUNT 16

#define HW_RATE     44100
#define HW_BITS     16
#define HW_CHANNELS 2

#define FILE2D MSS_DIR_UP "media" MSS_DIR_SEP "welcome.wav"


#define FILTER_NAME "Example filter"


HDIGDRIVER Digital = NULL;

static char properties[128][128];
static S32 is_boolean[128];
static S32 propertycount = 0;

//
// Prototype for our example installable filter's RIB entry point
//

extern "C" 
{
  S32 AILCALL ExampleFilter(HPROVIDER provider_handle, U32 up_down);
}

//############################################################################
//##                                                                        ##
//## Shutdown routine called from an atexit() handler                       ##
//##                                                                        ##
//############################################################################

static void MSS_MAIN_DEF Shutdown( void )
{
  if ( Digital != NULL)
  {
    AIL_close_digital_driver( Digital );
    Digital = 0;
  }

  AIL_shutdown();

  printf("\nFILTER stopped.\n");
}


//############################################################################
//##                                                                        ##
//## Enumerate the filter-level and sample-level properties                 ##
//##                                                                        ##
//############################################################################

static S32 Enum_properties( HPROVIDER filter,
                            HSAMPLE last )
{
  RIB_INTERFACE_ENTRY prop;
  HINTENUM next;

  printf("\nFilter provider properties:\n\n");

  //
  // Dump properties of filter provider
  //

  next = HINTENUM_FIRST;

  while (AIL_enumerate_filter_properties( filter,
                                          &next,
                                          &prop ) )
  {
    void *value;

    AIL_filter_property( filter,
                         prop.entry_name,
                         &value, 0, 0 );

    printf("   %s = %s\n",
            prop.entry_name,
            RIB_type_string( &value, prop.subtype ) );
  }


  //
  // Dump now the sample level-properties
  //

  printf( "_______________________________________________________________________________\n\n" );
  printf( "  Enter       : Start a 2D sound\n\n" );

  printf( "   ?          : Show filter properties\n\n" );
  printf( "  a/z         : Adjust sample property up/down (by 1)\n" );
  printf( "  Shift-a/z   : Adjust sample property up/down (by 100)\n" );
  printf( "  Control-a/z : Adjust sample property up/down (by 0.1)\n" );

  //
  // list all of the sample properties
  //

  propertycount = 0;

  next = HINTENUM_FIRST;

  while (AIL_enumerate_filter_sample_properties( filter,
                                                 &next,
                                                 &prop ) )
  {
    //
    // save the property name
    //

    strcpy( properties[ propertycount ], prop.entry_name );

    is_boolean[ propertycount ] = ( ( prop.subtype & ~RIB_READONLY ) == RIB_BOOL);

    ++propertycount;

    if (last != NULL)
    {
      void *value;

      //
      // print the current property values if they've played a sample
      //

      AIL_filter_sample_property( last,
                                  prop.entry_name,
                                  &value, 0, 0 );

      printf( "   %d          : Select \"%s\" (currently = %s)\n" ,
                  propertycount,
                  prop.entry_name,
                  RIB_type_string( &value, prop.subtype ) );
    }
    else
    {
      printf( "   %d          : Select \"%s\"\n",
                  propertycount,
                  prop.entry_name );
    }
  }

  if ( propertycount > 0 )
  {
    printf( "\n" );
  }

  printf( "  SPACE       : Stop all sounds\n" );
  printf( "  ESC         : Exit\n" );
  printf( "_______________________________________________________________________________\n\n" );

  return( propertycount );
}


//############################################################################
//##                                                                        ##
//## Adjust the specified property                                          ##
//##                                                                        ##
//############################################################################

static void Adjust_property( HSAMPLE S, char const* property, F32 adj )
{
  if ( property == 0 )
  {
    printf( "\r           Choose a property first.                     " );
    return;
  }

  if ( S==0 )
  {
    printf( "\r           Play a sound first.                          " );
    return;
  }

  //
  // is this a boolean prop?  if so, we'll have to adjust it a different way
  //

  S32 is_bool = 0;

  for ( S32 i = 0; i < propertycount ; i++ )
   {
   if ( !strcmp( property, properties[ i ] ) )
      {
      is_bool = is_boolean[i];
      break;
      }
   }

  if (is_bool)
   {
   S32 value = (adj < 0.0F) ? 0 : 1;

   //
   // set and get the property
   //

   AIL_filter_sample_property( S, property, 0, &value, &value );

   //
   // and print it
   //

   printf( "\r           %s = %s                     ", property ,value ? "True" : "False");
   }
  else
   {
   //
   // get the current value
   //

   F32 value = 0.0F;

   AIL_filter_sample_property( S, property, &value, 0, 0 );

   //
   // add in the adjustment
   //

   value += adj;

   //
   // now set and get the property
   //

   AIL_filter_sample_property( S, property, 0, &value, &value );

   //
   // and print it
   //

   printf( "\r           %s = %.2f                     ", property ,value );
   }
}


//############################################################################
//##                                                                        ##
//## main()                                                                 ##
//##                                                                        ##
//############################################################################

int MSS_MAIN_DEF main( int argc, char *argv[] )
{
  HPROVIDER      filter;
  HSAMPLE        S[DIGITALCOUNT];
  S32            nsamples;
  U32            nextupdate = 0;

  argc=argc;
  argv=argv;

  set_up_console( 1 );

  printf( "_______________________________________________________________________________\n\n" );
  printf( "MSS Pipeline Filter Test Bed\n");
  printf( "_______________________________________________________________________________\n\n" );

  //
  // set the redist directory and startup the system
  //

#ifdef IS_STATIC           // xbox and dos
    Register_RIB(DSP);    
#endif    

  //
  // Register our example filter implementation (below) with the system
  // This needs to be done *before* calling AIL_startup()!
  //

  filter = RIB_load_static_provider_library(ExampleFilter, "ExampleFilter");   

  AIL_set_redist_directory( MSS_DIR_UP_TWO MSS_REDIST_DIR_NAME );

  AIL_startup();

  atexit( Shutdown );

  //
  // open a digital driver
  //

  Digital = AIL_open_digital_driver( HW_RATE, HW_BITS, HW_CHANNELS, 0 );

  if (Digital == 0)
  {
    printf( "Error opening output device: %s.\n",AIL_last_error());
    exit( 1  );
  }


  //
  // Allocate handles to play normal 2D sound
  //

  for (nsamples=0; nsamples < DIGITALCOUNT; nsamples++)
  {
    S[nsamples] = AIL_allocate_sample_handle( Digital );

    if (S[nsamples] == NULL)
    {
      break;
    }
  }


  //
  // Show provider properties
  //

  S32 maxproperties = Enum_properties( filter, 0 );

  //
  // Load standard test sample file
  //

  void FAR *data = AIL_file_read( FILE2D, 0 );
  U32       size = (U32) AIL_file_size( FILE2D );

  if (data == NULL)
  {
    printf( "Couldn't load %s.\n", FILE2D );
    exit( 1 );
  }

  //
  // starting value
  //

  //F32 value = 100.0F;
  HSAMPLE lastsample = 0;
  char* selectedproperty = 0;

  for(;;)
  {
    //
    // Give other threads a time slice
    //  

    AIL_delay( 1 );  // waits 1/60th of a second

    //
    // update CPU use
    //

    U32 curtime = AIL_ms_count();

    if ( curtime > nextupdate )
    {
      printf( "\rCPU: %d%%", AIL_digital_CPU_percent( Digital ) );
      nextupdate = curtime + 1000;
    }

    //
    // Poll keyboard
    //

    if ( kbhit() )
    {
      S32 i;
      int ch = getch();

      switch (ch)
      {
        //
        // 1-9: Select property to adjust
        //
        case '1': case '2': case '3': case '4': case '5': case '6':
        case '7': case '8': case '9':
          i = ch - '1';
          if (i < maxproperties)
          {
            //
            // update the selected property
            //

            selectedproperty = properties[ i ];
            printf( "\r           %s selected.                  ", selectedproperty );
          }
          break;

        //
        // ESC: Exit
        //
        case 27:
          goto done;

        //
        // SPC: Stop all sounds
        //
        case ' ':
          for (i=0; i < nsamples; i++)
          {
            AIL_end_sample( S[i] );
          }
          break;

        //
        // ?: Dump provider properties
        //
        case '?':
          printf("  ");
          maxproperties = Enum_properties( filter, lastsample );
          break;

        //
        // Enter: Trigger 2D sound
        //
        case 13:
          //
          // find a sample to play
          //

          for (i=0; i < nsamples; i++)
          {
            if (AIL_sample_status( S[i] ) == SMP_DONE)
            {
              AIL_set_named_sample_file( S[i], FILE2D, data, size, 0 );

              AIL_set_sample_loop_count( S[i], 0 );

              AIL_set_sample_processor( S[i], SP_FILTER_0, filter );
               
              if (AIL_sample_processor(S[i], SP_FILTER_0) == 0)
               {
               printf("\n\n%s\n",AIL_last_error());
               exit(1);
               }

              AIL_start_sample( S[i] );

              lastsample= S[i];

              break;
            }
          }
          break;


        //
        // Raise/lower the selected property
        //
        case 'a':
          Adjust_property( lastsample, selectedproperty, 1.0F );
          break;

        case 'z':
          Adjust_property( lastsample, selectedproperty, -1.0F );
          break;

        case 'A':
          Adjust_property( lastsample, selectedproperty, 100.0F );
          break;

        case 'Z':
          Adjust_property( lastsample, selectedproperty, -100.0F );
          break;

        case 1:
          Adjust_property( lastsample, selectedproperty, 0.1F );
          break;

        case 26:
          Adjust_property( lastsample, selectedproperty, -0.1F );
          break;

      }
    }
  }

 done:
  
  AIL_shutdown();
  return( 0 );
}


//############################################################################
//##                                                                        ##
//## Example implementation of a user-written filter                        ##
//##                                                                        ##
//############################################################################

//
// Define various macros used in MSS filters (mostly pertaining to endian 
// conversion and build-buffer access)
//
// These macros are optional, but they can make the FLT_sample_process() function 
// a little easier to write.  If you want to create a modified version of an existing
// MSS filter, the job will be *much* easier if you use these macros.
//
// On some platforms, the implementations of these macros in imssapi.h will provide better
// performance.  However, not all compilers support the inline assembly and intrinsics 
// used in imssapi.h, so these default implementations are provided for compatibility.
//

#define AIL_PTR_ADD(ptr,off) ((void*)(((U8*)(ptr))+(off)))

#ifdef IS_LE

#define AIL_LE_SWAP16( ptr ) ( *((U16 *)(ptr) ) )
#define AIL_LE_SWAP16_OFS( ptr, const_ofs ) ( *((U16 *)(AIL_PTR_ADD(ptr,const_ofs)) ) )
#define AIL_STORE_LE_SWAP16( ptr, val ) { *((U16*)(ptr))=(U16)(val); }

#define WRITE_MONO_SAMPLE( dest, fOut, tmp )                 \
{                                                            \
   (void)(sizeof(tmp));                                      \
   if (fOut > 32767.0F)       *dest++ =  32767;              \
   else if (fOut < -32768.0F) *dest++ = -32768;              \
   else *dest++ = (S16) fOut;                                \
}

#define WRITE_STEREO_SAMPLE( dest, fOutL, fOutR, tmp )       \
{                                                            \
   (void)(sizeof(tmp));                                      \
   if (fOutL > 32767.0F)       *dest =  32767;               \
   else if (fOutL < -32768.0F) *dest = -32768;               \
   else *dest = (S16) fOutL;                                 \
   if (fOutR > 32767.0F)       dest[1] =  32767;             \
   else if (fOutR < -32768.0F) dest[1] = -32768;             \
   else dest[1] = (S16) fOutR;                               \
   dest+=2;                                                  \
}

#else

//
// Big-endian implementations must swap the output bytes, since mixer build
// buffers are always LE
//

#define AIL_LE_SWAP16(ptr) \
                 ( ( U16 ) \
                   ( ( ( ( *((U16 *)(ptr) ) ) << 8 ) ) | \
                     ( ( ( *((U16 *)(ptr) ) ) >> 8 ) ) ) \
                 )

#define AIL_LE_SWAP16_OFS(ptr,const_ofs) \
                 ( ( U16 ) \
                   ( ( ( ( *((U16 *)(AIL_PTR_ADD(ptr,const_ofs)) ) ) << 8 ) ) | \
                     ( ( ( *((U16 *)(AIL_PTR_ADD(ptr,const_ofs)) ) ) >> 8 ) ) ) \
                 )

#define AIL_STORE_LE_SWAP16( ptr, val )  { *((U16*)ptr) =
                   ( ( ( ((U16)(val) ) ) <<  8 ) ) | \
                   ( ( ( ((U16)(val) ) ) >>  8 ) ) ); }

#define WRITE_MONO_SAMPLE( dest, fOut, tmp )                         \
{                                                                    \
   (void)(sizeof(tmp));                                              \
   if (fOut > 32767.0F)       AIL_STORE_LE_SWAP16( dest, 32767 );    \
   else if (fOut < -32768.0F) AIL_STORE_LE_SWAP16( dest, -32768 );   \
   else AIL_STORE_LE_SWAP16( dest, (S16) fOut );                     \
   ++dest;                                                           \
}

#define WRITE_STEREO_SAMPLE( dest, fOutL, fOutR, tmp )               \
{                                                                    \
   (void)(sizeof(tmp));                                              \
   if (fOutL > 32767.0F)       AIL_STORE_LE_SWAP16( dest, 32767 );   \
   else if (fOutL < -32768.0F) AIL_STORE_LE_SWAP16( dest, -32768 );  \
   else AIL_STORE_LE_SWAP16( dest, (S16) fOutL );                    \
   ++dest;                                                           \
   if (fOutR > 32767.0F)       AIL_STORE_LE_SWAP16( dest, 32767 );   \
   else if (fOutR < -32768.0F) AIL_STORE_LE_SWAP16( dest, -32768 );  \
   else AIL_STORE_LE_SWAP16( dest, (S16) fOutR );                    \
   ++dest;                                                           \
}

#endif

//
// Filter definitions and parameter defaults
//

#define _FX_MIX_DEFAULT 1.0F     // Wet/dry mix defaults to 1.0 (full wet)

//
// Filter's attribute tokens 
// (beyond the default PROVIDER_NAME and PROVIDER_VERSION tags)
//

enum PROP
{
   _FX_PROVIDER_FLAGS,

   _FX_MIX,       // Wet/dry mix (as implemented by most filters...)
};

//
// Driver state descriptor
// One state descriptor is associated with each HDIGDRIVER
//

struct DRIVERSTATE
{
   //
   // Members common to all pipeline filter providers
   //

   HDIGDRIVER dig;

   //
   // Members associated with specific filter provider, if any
   //
};

//
// Per-sample filter state descriptor
// One state descriptor is associated with each HSAMPLE
//

struct SAMPLESTATE
{
   //
   // Members common to all pipeline filter providers
   //

   HSAMPLE          sample;   // HSAMPLE with which this descriptor is associated
   DRIVERSTATE *driver;   // Driver with which this descriptor is associated

   //
   // Members associated with specific filter provider, if any
   //

   F32 fMix;                  // Wet/dry mix
};

static S32 FLT_started = 0;
static C8  FLT_error_text[256];

// -------------------------------------------------------------
// Retrieve a standard RIB provider attribute by index                     
// -------------------------------------------------------------

static S32 AILCALL PROVIDER_property (HPROPERTY index, void *before_value, void const *new_value, void *after_value)
{
   switch ( index )
      {
      case PROVIDER_NAME:
        if (before_value)
        {
          *(char **)before_value = FILTER_NAME;
          return 1;
        }
        break;
        
      case PROVIDER_VERSION:
        if (before_value) 
        {
          *(S32*)before_value = 0x110;               // Filter API revision level (always 1.10 for now)
          return 1;
        }
        break;

      case _FX_PROVIDER_FLAGS: 
        if (before_value) 
        {
          *(S32*)before_value = FPROV_ON_SAMPLES;    // Enable filter to be listed by
          return 1;                                      // AIL_enumerate_filters() (use 0 here otherwise)
        }
        break;
      }
  return 0;
}

// -------------------------------------------------------------
// Return filter global error text
// -------------------------------------------------------------

static C8 *       AILCALL FLT_error       (void)
{
   if (FLT_error_text[0]==0)
      {
      return 0;
      }

   return FLT_error_text;
}

// -------------------------------------------------------------
// Initialize the filter module
// -------------------------------------------------------------

static FLTRESULT AILCALL FLT_startup (void)
{
   if (FLT_started++)
      {
      strcpy(FLT_error_text,"Already started");
      return FLT_ALREADY_STARTED;
      }

   //
   // Init static prefs/attributes
   //

   FLT_error_text[0] = 0;

   return FLT_NOERR;
}

// -------------------------------------------------------------
// Shut down filter
// -------------------------------------------------------------

static FLTRESULT AILCALL FLT_shutdown (void)
{
   if (!FLT_started)
      {
      strcpy(FLT_error_text,"Not initialized");
      return FLT_NOT_INIT;
      }

   --FLT_started;

   return FLT_NOERR;
}

// -------------------------------------------------------------
// Bind filter to driver              
// -------------------------------------------------------------

static HDRIVERSTATE AILCALL FLT_open_driver (HDIGDRIVER  dig)
{
   DRIVERSTATE *DRV = (DRIVERSTATE *) AIL_mem_alloc_lock(sizeof(DRIVERSTATE));

   if (DRV == NULL)
      {
      strcpy(FLT_error_text,"Out of memory");
      return 0;
      }

   memset(DRV,
          0,
          sizeof(DRIVERSTATE));

   DRV->dig = dig;

   //
   // Perform any filter-specific initialization here
   //

   //
   // Return descriptor address cast to handle
   //

   return (HSAMPLESTATE) DRV;
}

// -------------------------------------------------------------
// Close filter-driver binding
// -------------------------------------------------------------

static FLTRESULT AILCALL FLT_close_driver (HDRIVERSTATE state)
{
   DRIVERSTATE *DRV = (DRIVERSTATE *) state;

   AIL_mem_free_lock(DRV);

   return FLT_NOERR;
}

// -------------------------------------------------------------
// Perform any needed processing before per-sample mixing begins     
//                                                                   
// Called after the build buffer has been flushed prior to the mixing
// phase, but before any samples have been mixed into it             
// -------------------------------------------------------------

static void AILCALL FLT_premix_process (HDRIVERSTATE driver)
{
}

// -------------------------------------------------------------
// Process data after mixing                                             
//                                                                       
// Called after all samples have been mixed into the 32-bit build buffer,
// prior to copying the build-buffer contents to the driver's output     
// buffer                                                                
//                                                                       
// This function may optionally handle the task of filling the driver's  
// output buffer.  If it does, it should return a nonzero value to       
// indicate that it has done so                                          
// -------------------------------------------------------------

static S32 AILCALL FLT_postmix_process (HDRIVERSTATE driver, //)
                                        void    *output_buffer)
{
   return 0;
}

// -------------------------------------------------------------
// Bind filter to specified sample
// (The filter already has a valid binding to the HSAMPLE's driver)
// -------------------------------------------------------------

static HSAMPLESTATE AILCALL FLTSMP_open_sample (HDRIVERSTATE driver, //)
                                                HSAMPLE      S)
{
   //
   // Allocate our SAMPLESTATE descriptor
   //

   SAMPLESTATE *SS = (SAMPLESTATE *) AIL_mem_alloc_lock(sizeof(SAMPLESTATE));

   if (SS == NULL)
      {
      strcpy(FLT_error_text,"Out of memory");
      return 0;
      }

   memset(SS,
          0,
          sizeof(SAMPLESTATE));

   SS->driver = (DRIVERSTATE *) driver;
   SS->sample = S;

   //
   // Perform any filter-specific initialization here
   //

   SS->fMix = _FX_MIX_DEFAULT;
   
   //
   // Return descriptor address cast to handle
   //

   return (HSAMPLESTATE) SS;
}

// -------------------------------------------------------------
// Close filter-sample binding
// -------------------------------------------------------------

static FLTRESULT AILCALL FLTSMP_close_sample (HSAMPLESTATE state)
{
   SAMPLESTATE *SS = (SAMPLESTATE *) state;

   //
   // Perform any filter-specific cleanup here
   //

   AIL_mem_free_lock(SS);

   return FLT_NOERR;
}

// -------------------------------------------------------------
//  Process sample data immediately prior to mixing
//
//  This is the kernel routine for all MSS filters that need
//  to modify or inspect outgoing data on a per-HSAMPLE basis
//                                                                        
//  Parameters:                                                           
//                                                                        
//    state is the sample descriptor.  You can retrieve the HSAMPLE via   
//    the state.sample member, if needed.                                 
//                                                                        
//    source_buffer is a pointer to a stereo or mono 16-bit sample    
//    buffer.                                                             
//                                  
//    dest_buffer is the destination 16-bit sample buffer.                
//                                      
//    n_samples is the number of samples (either mono or stereo) to       
//    process.                                                            
//                                                                        
//    dest_playback_rate is the hardware sample rate.  Filters must watch 
//    for changes in the playback rate and recalculate any dependent      
//    parameters.  (This will never change when running on software-mixed 
//    drivers, but if you're using a hardware-mixed output option like 
//    EAX/DirectSound3D, the hardware sample rate has to follow the 
//    sample's playback rate!)
//                                                                        
//    is_stereo says whether the input data is stereo or mono.            
//
// -------------------------------------------------------------


static void AILCALL FLTSMP_sample_process(HSAMPLESTATE       state,  //)
                                          S16  *MSSRESTRICT  source_buffer,
                                          S16  *MSSRESTRICT  dest_buffer,
                                          S32                n_samples,
                                          S32                dest_playback_rate,
                                          S32                is_stereo)
{
   SAMPLESTATE *SSp  = (SAMPLESTATE *) state;
   SAMPLESTATE SS;
   memcpy(&SS, SSp, sizeof(SS));   // Copy to stack for faster access on certain platforms
   //HSAMPLE      S   = SS.sample;
   //DRIVERSTATE *DRV = SS.driver;

   S32 dwIndex;
   F32 fInput;
   F32 fOutL,fOutR;

   F32 fDryOut  =  1.0F - SS.fMix;
   F32 fWetOut  =  SS.fMix;

   //
   // Fast path used when wet mix near zero
   //

   if ( fDryOut > 0.999f )
   {
     if ( source_buffer != dest_buffer )
       memcpy( dest_buffer, source_buffer, n_samples * ( is_stereo ? 4 : 2 ) );
     return;
   }

   //
   // (This is a good place to watch for changes in dest_playback_rate
   // and update any precalculated filter coefficients accordingly... see lowpass.cpp,
   // for example)
   //

   if ( is_stereo )
   {
      // Copy filtered data into stereo build buffer
      for( dwIndex = 0; dwIndex < n_samples; dwIndex++ )
      {
         // Read little-endian PCM input sample (left) as signed, single-precision float [-32K,32K]
         fInput   =  (F32) (S16) AIL_LE_SWAP16(source_buffer);

         // Calculate left output sample (no processing performed in this example)
         fOutL    =  fInput;

         // Apply wet/dry mix to left output sample
         fOutL = ( fOutL * fWetOut ) + ( fInput * fDryOut );

         // Read little-endian PCM input sample (right) as signed, single-precision float [-32K,32K]
         fInput   =  (F32) (S16) AIL_LE_SWAP16_OFS(source_buffer,2);

         // Calculate right output sample (no processing performed in this example)
         fOutR    =  fInput;

         // Apply wet/dry mix to right output sample
         fOutR = ( fOutR * fWetOut ) + ( fInput * fDryOut );

         // Convert output back to S16, clamp it, and write it to the build buffer 
         // using platform-specific macro
         S32 tmp;
         WRITE_STEREO_SAMPLE( dest_buffer, fOutL, fOutR, tmp );

         source_buffer += 2;
      }
   }
   else
   {
      // Copy filtered data into mono build buffer
      for( dwIndex = 0; dwIndex < n_samples; dwIndex++ )
      {
         // Read little-endian PCM input sample as signed, single-precision float [-32K,32K]
         fInput   =  (F32) (S16) AIL_LE_SWAP16(source_buffer);
         ++source_buffer;

         // Calculate output sample (no processing performed in this example)
         fOutL    =  fInput;

         // Apply wet/dry mix
         fOutL = ( fOutL * fWetOut ) + ( fInput * fDryOut );

         // Convert output back to S16, clamp it, and write it to the build buffer 
         // using platform-specific macro
         S32 tmp;
         WRITE_MONO_SAMPLE( dest_buffer, fOutL, tmp );
      }
   }

   memcpy(SSp, &SS, sizeof(*SSp));  // Update the sample state structure with any changes 

   //
   // Note: to communicate with the application, you can use the AIL_sample_user_data() function
   // to retrieve the address of a callback function, data buffer, etc... or just create a 
   // filter property to pass the address explicitly.  Filters dedicated to data analysis (as opposed
   // to manipulation) may need to do this.
   //
   // If you choose to call back into your application here, that callback function must make *no*
   // calls to MSS or otherwise alter its state!
   //
}     

// -------------------------------------------------------------
// Set sample preference value, returning previous setting   
// -------------------------------------------------------------

static S32 AILCALL FLTSMP_sample_property(HSAMPLESTATE state, HPROPERTY property, void *before_value, void const *new_value, void *after_value)
{
   SAMPLESTATE *SS  = (SAMPLESTATE *) state;
   //HSAMPLE      S   = SS->sample;

   switch (property)
      {
      //
      // Preferences
      //

      case _FX_MIX:

         if (before_value) 
            {
            *(F32 *) before_value = SS->fMix;
            }

         if (new_value) 
            {
            SS->fMix = *(F32 const*)new_value;

            if (SS->fMix > 1.0F) SS->fMix = 1.0F;     // Clamp wet/dry mix at [0.0, 1.0]
            if (SS->fMix < 0.0F) SS->fMix = 0.0F;
            }         

         if (after_value) 
            {
            *(F32*) after_value = SS->fMix;
            }

         return 1;
      }

   return 0;
}

// -------------------------------------------------------------
// Filter registration function
//
// This function must be prototyped exactly as it appears below, and passed to 
// Register_RIB() (or the equivalent RIB_load_static_provider_library() function)
// prior to any attempt to use the filter
// -------------------------------------------------------------

extern "C" S32 AILCALL ExampleFilter( HPROVIDER provider_handle, 
                                      U32       up_down)
{
   const RIB_INTERFACE_ENTRY FLT1[] =
      {
      REG_FN(PROVIDER_property),
      REG_PR("Name",      PROVIDER_NAME,        (RIB_DATA_SUBTYPE) (RIB_STRING | RIB_READONLY)),
      REG_PR("Version",   PROVIDER_VERSION,     (RIB_DATA_SUBTYPE) (RIB_HEX    | RIB_READONLY)),
      REG_PR("Flags",    _FX_PROVIDER_FLAGS,    (RIB_DATA_SUBTYPE) (RIB_HEX    | RIB_READONLY)),
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
      REG_PR("Mix", _FX_MIX, RIB_FLOAT),    // Wet/dry mix, 0-1
      };

   if (up_down)
      {
      RIB_register(provider_handle, "MSS pipeline filter", FLT1); // Broken up into multiple structs
      RIB_register(provider_handle, "MSS pipeline filter", FLT2); // with <= 4 members to avoid triggering
      RIB_register(provider_handle, "MSS pipeline filter", FLT3); // runtime calls on SN Systems' PS2 compiler

      RIB_register(provider_handle, "Pipeline filter sample services", FLTSMP1);
      RIB_register(provider_handle, "Pipeline filter sample services", FLTSMP2);
      }
   else
      {
      RIB_unregister_all(provider_handle);
      }

   return 1;
}

