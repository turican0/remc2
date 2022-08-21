//############################################################################
//##                                                                        ##
//##  RECORD.C                                                              ##
//##                                                                        ##
//##  Recording API example program                                         ##
//##                                                                        ##
//##  V1.00 of  2-Aug-02: Initial                                           ##
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
#include <string.h>

#include "mss.h"

#include "con_util.i"

// @cdep pre $DefaultsConsoleMSS
// @cdep post $BuildConsoleMSS

static void * input_buf = 0;
static U32 total_size = 0;
static U32 current_size = 0;

// callback to save the sound into a buffer

static void AILCALLBACK handle_sound( void const * data, S32 bytes, UINTa user_data )
{
  S32 left = (S32) ( total_size - current_size );

  if ( left < bytes )
  {
    bytes = left;
  }

  memcpy( ( (U8*) input_buf ) + current_size, data, (size_t) bytes );
  current_size += bytes;
}

//############################################################################
//##                                                                        ##
//## Recording example program                                              ##
//##                                                                        ##
//############################################################################

int MSS_MAIN_DEF main( int argc, char *argv[] )
{
  HDIGDRIVER  dig;
  HDIGINPUT record;
  HSAMPLE sample;
  S32 recording;
  S32 playing;
  U32 old_time;

  set_up_console( 1 );

  printf( "RECORD - Version " MSS_VERSION "            " MSS_COPYRIGHT " \n" );
  printf( "-------------------------------------------------------------------------------\n\n" );

  printf( "This program records a sample and then plays it back.\n\n" );

  //
  // set the redist directory and statup the system
  //

  AIL_set_redist_directory( MSS_DIR_UP_TWO MSS_REDIST_DIR_NAME );

  AIL_startup();

  //
  // open a digital driver
  //

  dig = AIL_open_digital_driver( 44100, 16, 2, 0 );

  if ( dig == 0 )
  {
    printf( AIL_last_error() );
    exit( 1 );
  }

  sample = AIL_allocate_sample_handle( dig );

  if ( sample == 0 )
  {
    printf( AIL_last_error() );
    exit( 2 );
  }

  AIL_init_sample( sample, DIG_F_MONO_16, 0 );
  AIL_set_sample_playback_rate( sample, 44100 );

  //
  // allocate input buffer
  //

  total_size = 44100 * 2 * 10;  // max 10 seconds worth

  input_buf = AIL_mem_alloc_lock( total_size );

  if ( input_buf == 0 )
  {
    printf( AIL_last_error() );
    exit( 3 );
  }

  //
  // open the record handle
  //

  {
    AIL_INPUT_INFO info;
#ifdef IS_MAC
    info.device_ID = 0;
#else
    info.device_ID = WAVE_MAPPER;
#endif
    info.hardware_format = DIG_F_MONO_16;
    info.hardware_rate = 44100;
    info.buffer_size = 8192;
    info.callback = handle_sound;

    record = AIL_open_input( &info );

    if ( record == 0 )
    {
      printf( AIL_last_error() );
      exit( 4 );
    }
  }

  recording = 0;
  playing = 0;
  old_time = 0;

  printf( "Hit Space to stop and start recording.\n"
          "Hit Enter to play the recorded sound.\n"
          "Hit ESC to exit.\n\n" );

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
    // process key strokes
    //

    if ( kbhit( ) )
    {
      switch ( getch() )
      {
        case 27:
          goto done;

        case ' ':
          // start/stop recording
          if ( recording )
          {
            AIL_set_input_state( record, 0 );
            recording = 0;
          }
          else
          {
            AIL_stop_sample( sample );

            current_size = 0;
            AIL_set_input_state( record, 1 );
            recording = 1;
          }
          break;

        case 13:
          // stop recording
          if ( recording )
          {
            AIL_set_input_state( record, 0 );
            recording = 0;
          }

          AIL_set_sample_address( sample, input_buf, current_size );
          AIL_start_sample( sample );
          playing = 1;
          break;
      }
    }

    //
    // display the status (but only every 250 ms)
    //

    if ( ( AIL_ms_count() - old_time ) > 250 )
    {
      old_time = AIL_ms_count();

      if ( recording )
      {
        printf( "\rRecording %i bytes.                                ", current_size );
      }
      else
      {
        if ( playing )
        {
          if ( AIL_sample_status( sample ) == SMP_PLAYING )
          {
            printf( "\rPlaying %i bytes.                                  ", AIL_sample_position( sample ) );
          }
          else
          {
            playing = 0;
          }
        }
        else
        {
          printf( "\rNot recording (%i bytes currently recorded).", current_size );
        }
      }
    }
  }

 done:

  //
  // Clean up
  //

  AIL_close_input( record );

  //
  // Shut down driver, digital services, and process services
  //

  AIL_close_digital_driver( dig );

  AIL_shutdown();

  printf( "\n\nRECORD stopped.\n" );

  return( 0 );
}

