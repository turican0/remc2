//############################################################################
//##                                                                        ##
//##  DBTEST.C                                                              ##
//##                                                                        ##
//##  Double-buffered digital audio test facility                           ##
//##                                                                        ##
//##  V1.00 of  2-Jul-94: Initial                                           ##
//##  V1.01 of 20-Nov-94: Use minimum possible buffer size                  ##
//##  V1.02 of 18-Jan-95: Set up driver with .INI file                      ##
//##  V1.03 of  9-Jun-95: Use new HMDIDRIVER/HDIGDRIVER driver handle types ##
//##                      Check driver installation error code              ##
//##  V1.04 of 15-Nov-05: Support MP3 files instead of raw files            ##
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
#include <conio.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <io.h>

#include "mss.h"

#include "con_util.i"

// @cdep pre $DefaultsConsoleMSS
// @cdep post $BuildConsoleMSS

//############################################################################
//##                                                                        ##
//## Check to see if either of the sample's buffers need to be filled with  ##
//## audio data, and, if necessary, refresh it                              ##
//##                                                                        ##
//############################################################################

static void serve_sample( HSAMPLE sample, S8 **buf, S32 size, S32 file )
{
  S32 n,len;

  n = AIL_sample_buffer_ready(sample);

  if (n != -1)
  {
    printf(".");

    //
    // An input buffer is free - read some data into it
    //

    len = _read( file, buf[n], size );

    //
    // Pass it into Miles
    //

    AIL_load_sample_buffer( sample,
                            n,
                            buf[n],
                            len );

    //
    // In this example, we start playback after the first buffer is available
    // This call is ignored for double-buffered samples that are already playing
    //

    AIL_start_sample(sample);
  }
}

//############################################################################
//##                                                                        ##
//## Double-buffering example program                                       ##
//##                                                                        ##
//############################################################################

void MSS_MAIN_DEF main( int argc, char *argv[] )
{
  HDIGDRIVER  dig;
  HSAMPLE     sample;
  HPROVIDER   HP;
  S32         len;
  S32         file;
  S8          *buf[2];
  S32         buffer_size;
  static U8   temp_buffer[AIL_MAX_FILE_HEADER_SIZE];

  setbuf( stdout, NULL );

  printf( "DBTEST - Version " MSS_VERSION "               " MSS_COPYRIGHT " \n" );
  printf( "-------------------------------------------------------------------------------\n\n" );

  printf( "This program plays an .MP3 file using the low-level double-buffering calls.\n\n" );

  if (argc != 2)
  {
    printf( "Usage: DBTEST filename.MP3\n" );
    exit( 1 );
  }

  //
  // Set the redist directory and start MSS
  //

  AIL_set_redist_directory( MSS_DIR_UP_TWO MSS_REDIST_DIR_NAME );

  AIL_startup();

  //
  // Initialize digital sound system
  //

  dig = AIL_open_digital_driver( 44100, 16, 2, 0);

  if (dig == 0)
  {
    printf("%s\n", AIL_last_error() );

    AIL_shutdown();
    exit( 1 );
  }

  //
  // Open the file supplied
  //

  file = _open( argv[1], O_RDONLY | O_BINARY );

  if (file == -1)
  {
    printf( "Could not open file '%s'\n", argv[1] );

    AIL_shutdown();
    exit( 1 );
  }

  //
  // Identify file's type
  //
  // If no specific ASI provider is available for this file type, bail
  //

  HP = RIB_find_file_provider("ASI codec",
                              "Input file types",
                               argv[1]);

  if (HP == 0)
     {
     printf("No ASI provider available for file %s\n", argv[1]);

     AIL_shutdown();
     exit(1);
     }

  //
  // It's an MP3 or other ASI-supported file -- set sample format according to 
  // file name and contents
  //
  // We'll need to load the first 4K of the file's contents to determine the output 
  // data format, then rewind the file to its beginning
  //

  memset(temp_buffer, 0, AIL_MAX_FILE_HEADER_SIZE);

  len = _read(file, temp_buffer, AIL_MAX_FILE_HEADER_SIZE);

  if (!len)
     {
     printf( "Could not read file '%s'\n", argv[1]);

     AIL_shutdown();
     exit(1);
     }

  _lseek(file, 0, SEEK_SET);

  //
  // Allocate an HSAMPLE that uses the specified ASI codec to decode its
  // source data
  //
  // We must first set the address of the sample to the temp header
  // buffer, so the sample parameters (rate, width, channels, etc.) can
  // be determined correctly.  The buffered data is ignored (and
  // should be considered invalid) after AIL_set_sample_processor() returns.
  //
  // AIL_set_sample_processor(DP_ASI_DECODER) will set the sample's format
  // (bit depth, channels, playback rate) to match what the ASI codec reports
  // about the source data.  Since this is a freshly-allocated HSAMPLE, we
  // don't need to call AIL_init_sample() first.
  //
  // (This functionality is pretty similar to the AIL_set_sample_file() function, but
  // that function isn't meant for use with double-buffered HSAMPLEs.)
  // 

  sample = AIL_allocate_sample_handle(dig);

  AIL_set_sample_address(sample,
                         temp_buffer,
                         len);

  AIL_set_sample_processor(sample,
                           DP_ASI_DECODER,
                           HP);

  //
  // Ask Miles for the size of the buffer that we need
  //
  // We'll use worst-case rate and type information for the decompressed data stream here,
  // to make sure the buffers are big enough to handle the (more granular) compressed data
  //

  buffer_size = AIL_minimum_sample_buffer_size(dig, 44100, DIG_F_MONO_16);

  //
  // Allocate two buffers for double-buffering
  //

  buf[0] = AIL_mem_alloc_lock( buffer_size );
  buf[1] = AIL_mem_alloc_lock( buffer_size );

  //
  // Example of application's main event loop
  //
  // Read data from file in buffer_size chunks, passing each chunk to the
  // MSS API when requested
  //

  printf( "Press any key to stop playback " );

  while ( !kbhit() )
  {
    //
    // Give other threads a time slice
    //  

    AIL_delay( 1 );  // waits 1/60th of a second

    //
    // (Process application events here....)
    //

    //
    // Service audio buffers
    //

    serve_sample( sample, buf, buffer_size, file );

    //
    // Exit test loop when final buffer stops playing
    //

    if (AIL_sample_status( sample ) != SMP_PLAYING)
    {
      break;
    }
  }

  if (kbhit())
    getch();

  //
  // Clean up
  //

  AIL_release_sample_handle( sample );

  _close( file );

  AIL_mem_free_lock( buf[0] );
  AIL_mem_free_lock( buf[1] );

  AIL_close_digital_driver( dig );

  AIL_shutdown();

  printf( "\n\nDBTEST stopped.\n" );
}

