//############################################################################
//##                                                                        ##
//##  MULTIFLT.C - Multiple-stage filter pipeline example                   ##
//##                                                                        ##
//##  V1.00 of 3-Mar-05: Initial, derived from DIGPLAY.C                    ##
//##                                                                        ##
//##   Author: John Miles                                                   ##
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

#include "mss.h"

#include "con_util.i"

// @cdep pre $DefaultsConsoleMSS
// @cdep post $BuildConsoleMSS

//
// Uncomment CAPTURE_FILENAME definition to record output of 
// filter chain to .WAV file for offline inspection
//

#define CAPTURE_FILENAME ".\\multiflt.wav"

#define HW_RATE     44100
#define HW_BITS     16
#define HW_CHANNELS 2

//############################################################################
//##                                                                        ##
//## Digital playback example program                                       ##
//##                                                                        ##
//############################################################################

int MSS_MAIN_DEF main( int argc, char *argv[] )
{
   HDIGDRIVER  dig;
   HSAMPLE     S;
   S32         i;
 
   set_up_console( 0 );
 
   printf( "MULTIFLT - Version " MSS_VERSION "          " MSS_COPYRIGHT " \n" );
   printf( "-------------------------------------------------------------------------------\n\n" );
 
   printf( "This program plays random noise through a five-band parametric equalizer and\n");
   printf( "two additional highpass/lowpass filters, recording the result to a .WAV file\n");
   printf( "with the MSS Capture filter.\n");

#ifdef IS_STATIC           // xbox and dos
    Register_RIB(DSP);  
#endif    
 
   //
   // set the redist directory and statup the system
   //

   AIL_set_redist_directory( MSS_DIR_UP_TWO MSS_REDIST_DIR_NAME );

   AIL_startup();

   //
   // Initialize digital sound system
   //
 
   dig = AIL_open_digital_driver(HW_RATE, HW_BITS, HW_CHANNELS, 0);
 
   if ( dig==0 )
   {
      puts(AIL_last_error());
 
      AIL_shutdown();
      exit(1);
   }
 
   //
   // Install capture filter to record the waveform for offline analysis  
   // if desired
   //
   // The capture filter is not supported under DOS
   //

#ifdef CAPTURE_FILENAME

   HPROVIDER    capture_provider;
   HDRIVERSTATE capture_handle=0;

   if (AIL_find_filter("Capture Filter",&capture_provider))
   {
      AIL_filter_property( capture_provider, 
                           "Filename", 
                           0, CAPTURE_FILENAME, 0 );

      capture_handle = AIL_open_filter(capture_provider,
                                       dig);
   }
#endif

   //
   // Allocate an HSAMPLE to use
   //
 
   S = AIL_allocate_sample_handle(dig);
 
   if (S == NULL)
   {
      puts(AIL_last_error());
      AIL_shutdown();
      exit(1);
   }
 
   //
   // Initialize the sample
   //
 
   AIL_init_sample(S, DIG_F_MONO_16, 0);

   AIL_set_sample_playback_rate (S, HW_RATE);

   //
   // Install highpass/lowpass filters
   //

   HPROVIDER highpass;

   if (!AIL_find_filter("HighPass Filter",&highpass))
   {    
      printf("Error: Could not find highpass filter provider\n");
      AIL_shutdown();
      exit(1);
   }

   F32 hp_cutoff = 500.0F;

   AIL_set_sample_processor       (S, SP_FILTER_0, highpass);
   AIL_sample_stage_property      (S, SP_FILTER_0, "Highpass Cutoff", 0, &hp_cutoff, 0);

   HPROVIDER lowpass;

   if (!AIL_find_filter("LowPass Filter",&lowpass))
   {    
      printf("Error: Could not find lowpass filter provider\n");
      AIL_shutdown();
      exit(1);
   }

   F32 lp_cutoff = 15000.0F;

   AIL_set_sample_processor       (S, SP_FILTER_7, lowpass);
   AIL_sample_stage_property      (S, SP_FILTER_7, "Lowpass Cutoff", 0, &lp_cutoff, 0);

   //
   // Install parametric EQ filters (which become fairly imprecise as their center frequency 
   // approaches Nyquist)
   //

   HPROVIDER parametric_EQ;

   if (!AIL_find_filter("Parametric EQ Filter",&parametric_EQ))
   {    
      printf("Error: Could not find parametric EQ filter provider\n");
      AIL_shutdown();
      exit(1);
   }

   static SAMPLESTAGE stage[]    = { SP_FILTER_1, SP_FILTER_2, SP_FILTER_3, SP_FILTER_4, SP_FILTER_5 };
   static F32 center_frequency[] = { 1000.0F,     5000.0F,     10000.0F,    15000.0F,    20000.0F    };
   static F32 Q[]                = {   0.03F,       0.03F,        0.03F,       0.03F,       0.03F    };
   static F32 gain[]             = {   10.0F,       15.0F,       -12.0F,       18.0F,       12.0F    };

   for (i=0; i < 5; i++)
   {
      AIL_set_sample_processor(S, stage[i], parametric_EQ);

      AIL_sample_stage_property      (S, stage[i], "ParmEQ Center", 0, &center_frequency[i], 0);
      AIL_sample_stage_property      (S, stage[i], "ParmEQ Q",      0, &Q               [i], 0);
      AIL_sample_stage_property      (S, stage[i], "ParmEQ Gain",   0, &gain            [i], 0);
    }

   //
   // Create sample block full of random noise and begin playing it
   //
   // Peak amplitude of noise is 18 dB down from maximum 16-bit range (+/- 4K)
   //
 
   static S16 sample_data[65536];

   for (i=0; i < ARY_CNT(sample_data); i++)
   {
      sample_data[i] = (S16) (((S16) rand() >> 2) - 4096);
   }

   AIL_set_sample_address   (S, sample_data, sizeof(sample_data));
   AIL_set_sample_loop_count(S, 0);

   AIL_start_sample( S );

   //
   // Prompt for keypress
   //
 
   printf( "\nPress any key to halt playback and exit MULTIFLT\n" );
 
   getch();
 
#ifdef CAPTURE_FILENAME
   //
   // Demonstrate how to close a filter -- not strictly necessary here, since they'll
   // be closed automatically at shutdown time
   //

   AIL_close_filter(capture_handle);
#endif

   AIL_shutdown();
 
   printf( "\nMULTIFLT stopped.\n" );
   return( 0 );
}

