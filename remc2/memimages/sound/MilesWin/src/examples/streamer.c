//############################################################################
//##                                                                        ##
//##  STREAMER.C                                                            ##
//##                                                                        ##
//##  Streaming API example program                                         ##
//##                                                                        ##
//##  V1.00 of  2-Jul-94: Initial                                           ##
//##                                                                        ##
//##   Author: Jeff Roberts                                                 ##
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

#include "mss.h"

#include "con_util.i"

// @cdep pre $DefaultsConsoleMSS
// @cdep post $BuildConsoleMSS

//############################################################################
//##                                                                        ##
//## Streaming example program                                              ##
//##                                                                        ##
//############################################################################

int MSS_MAIN_DEF main( int argc, char *argv[] )
{
  HDIGDRIVER  dig;
  HSTREAM  stream;
  HSAMPLE  HS;
  S32 size,advance,mem,i,rate,last_per = -1;

  S32 paused = 0;
  U32 lasttime = 0;

  set_up_console( 0 );

  printf( "STREAMER - Version " MSS_VERSION "          " MSS_COPYRIGHT " \n" );
  printf( "-------------------------------------------------------------------------------\n\n" );

  printf( "This program streams a digital audio MP3 or WAV file with the streaming API.\n\n" );

  if (argc < 2)
  {
    printf( "Enter name of file to play (use '" MSS_DIR_UP "media" MSS_DIR_SEP "Music.MP3', for example): " );
    get_args( &argc, &argv );
  }

  //
  // Set the redist directory and start up MSS
  //

  AIL_set_redist_directory( MSS_DIR_UP_TWO MSS_REDIST_DIR_NAME );

  AIL_startup();

  //
  // Open a digital driver
  //

  dig = AIL_open_digital_driver( 44100, 16, 2, 0 );

  if ( dig == 0 )
  {
    printf( AIL_last_error() );
    exit( 1 );
  }

  //
  // Open the stream handle
  //

  stream = AIL_open_stream( dig, argv[1], 0 );

  if ( stream == 0 )
  {
    printf( "Could not open file '%s': %s\n", argv[1] , AIL_last_error());

    AIL_shutdown();
    exit( 1 );
  }

  //
  // Loop the stream forever
  //

  AIL_set_stream_loop_count( stream, 0 );

  AIL_stream_info( stream, &rate, 0, &size, &mem );

  advance = size / 20; if ( advance < 500 ) advance = 500;

  printf( "Hit Enter to restart the stream.\n"
          "Hit Space to pause and restart the stream.\n"
          "Hit '<' and '>' to skip forwards and backwards in the stream.\n"
          "Hit '[' and ']' to change the playback rate, '\\' to return to normal.\n"
          "Hit ESC to exit.\n\n"
          "Playing a %i byte sound file with %i bytes of RAM.\n\n",
          size, mem );

  AIL_start_stream( stream );

  HS = AIL_stream_sample_handle( stream );

  for(;;)
  {
    //
    // Give other threads a time slice
    //  

    AIL_delay( 1 );  // waits 1/60th of a second

    //
    // (You could process other application events here....)
    //


    //
    // Process keystrokes
    //

    if ( kbhit() )
    {
      switch ( getch() )
      {
        case 27:
          goto done;

        case ' ':
          AIL_pause_stream( stream, paused ^= 1 );
          break;

        case 13:
          paused=0;
          AIL_start_stream( stream );
          break;

        case '<':
        case ',':
          i = AIL_stream_position( stream );
          if ( i < advance )
            i = advance;
          AIL_set_stream_position( stream, i - advance );
          break;

        case '>':
        case '.':
          AIL_set_stream_position( stream, AIL_stream_position( stream ) + advance );
          break;

        case '[':
          i=AIL_sample_playback_rate( HS )-1000;
          if ( i < ( rate / 2 ) )
            i = rate / 2;
          AIL_set_sample_playback_rate( HS , i );
          break;

        case ']':
          i=AIL_sample_playback_rate( HS ) + 1000;
          if (i > ( rate * 2 ) )
            i = rate * 2;
          AIL_set_sample_playback_rate( HS, i );
          break;

        case '\\':
          AIL_set_sample_playback_rate( HS, rate );
          break;
      }
    }

    //
    // Show the percentage complete and service audio every 250 ms
    //

    if ( ( AIL_ms_count() - lasttime ) > 250 )
    {
      S32 per = (S32) ( ( (F32) AIL_stream_position( stream ) * 100.0f) / (F32) size );

      if ( per != last_per )
      {
        printf( "\rPercent played: %3i%%", per );
        last_per = per;
      }

      //
      // Service audio buffers
      //

      AIL_service_stream( stream, 0 );

      lasttime = AIL_ms_count();
    }

  }

done:

  //
  // Clean up
  //

  AIL_close_stream( stream );

  AIL_close_digital_driver( dig );

  AIL_shutdown();

  printf( "\n\nSTREAMER stopped.\n" );

  return( 0 );
}

