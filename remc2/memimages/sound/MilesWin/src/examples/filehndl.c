//############################################################################
//##                                                                        ##
//##  FILEHNDL.C                                                            ##
//##                                                                        ##
//##  Streaming API example program that uses file handles                  ##
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

#include <windows.h>
 
//############################################################################
//##                                                                        ##
//## Streaming example program                                              ##
//##                                                                        ##
//############################################################################

void MSS_MAIN_DEF main( int argc, char *argv[] )
{
  HDIGDRIVER  dig;
  HSTREAM  stream;
  HSAMPLE  HS;
  HANDLE existing_file;
  
  S32 size,mem,i,rate,last_per = 0;

  U32 paused = 0;
  U32 dodelay = 0;

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

  AIL_set_redist_directory( MSS_DIR_UP_TWO "redist" );

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
  // There are two ways to do this -- you can specify a filename directly, or you can
  // stream from within a packed file that's open via an existing handle obtained from 
  // CreateFile() (or the analogous file-open API on your platform)
  //

  {
    existing_file = CreateFile(argv[1],
                               GENERIC_READ,
                               FILE_SHARE_READ,
                               0,
                               OPEN_EXISTING,
                               FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
                               0);

    if (existing_file == INVALID_HANDLE_VALUE)
    {
      stream = 0;
    }
    else
    {
      S32 begin_streaming_at_file_position = 0;    
      S32 length_of_streamed_data          = GetFileSize(existing_file, NULL);

      C8 load_handle[MAX_PATH];

      S32 len = strlen(argv[1]);

      if ((len >= 4) && (!_stricmp(&argv[1][len-4],".mp3")))
      {
        //
        // .MP3 image without a .WAV wrapper
        // (Offset/length parameters are mandatory)
        //

        sprintf(load_handle, 
                "\\\\\\\\%d,%d,%d.mp3",
                existing_file,
                length_of_streamed_data,
                begin_streaming_at_file_position);
      }
      else
      {
        //
        // .WAV image
        // (Offset/length parameters are optional)
        //

        sprintf(load_handle, 
                "\\\\\\\\%d",
                existing_file);
      }

      stream = AIL_open_stream(dig, load_handle, 0);
    }
  }

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

  printf( "Hit Enter to restart the stream.\n"
          "Hit Space to pause and restart the stream.\n"
          "Hit '<' and '>' to skip forwards and backwards in the stream.\n"
          "Hit '[' and ']' to change the playback rate, '\\' to return to normal.\n"
          "Hit ESC to exit.\n\n"
          "Playing a %i byte sound file with %i bytes of RAM.\n\nPress the escape key to stop playback:     ",
          size, mem );

  AIL_start_stream( stream );

  HS = AIL_stream_sample_handle( stream );

  while ( 1 )
  {

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
          if ( i < 5000 )
            i = 5000;
          AIL_set_stream_position( stream, i - 5000 );
          break;

        case '>':
        case '.':
          AIL_set_stream_position( stream, AIL_stream_position( stream ) + 5000 );
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

    if ( AIL_ms_count() > dodelay )
    {
      S32 per = AIL_stream_position( stream ) * 100 / size;

      if ( per != last_per )
      {
        printf( "\b\b\b\b%3i%%", per );
        last_per = per;
      }

      //
      // Service audio buffers
      //

      AIL_service_stream( stream, 0 );


      dodelay = AIL_ms_count() + 250;
    }

  }

done:

  //
  // Clean up
  //

  AIL_close_stream( stream );

  CloseHandle( existing_file );

  AIL_close_digital_driver( dig );

  AIL_shutdown();

  printf( "\n\nSTREAMER stopped.\n" );
}

