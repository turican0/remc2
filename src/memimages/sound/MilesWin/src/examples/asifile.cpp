//############################################################################
//##                                                                        ##
//##  ASIFILE.CPP                                                           ##
//##                                                                        ##
//##  V1.00 of 5-Jul-98: Initial version                                    ##
//##                                                                        ##
//##  Author: John Miles                                                    ##
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

#include "mss.h"

#include "con_util.i"

#pragma pack(1)

// @cdep pre $DefaultsConsoleMSS
// @cdep post $BuildConsoleMSS

struct WAVEOUT
{
  U32 riffmark;
  U32 rifflen;
  char wavemark[4];
  U32 fmtmark;
  U32 fmtlen;
  U16 fmttag;
  U16 channels;
  U32 sampersec;
  U32 avepersec;
  U16 blockalign;
  S16 bitspersam;
  U32 datamark;
  U32 datalen;
  C8  data[1];
};

#pragma pack()

// ---------------------------------------------------------------------------
// enum_attribs()
// ---------------------------------------------------------------------------

static void Enum_properties( HPROVIDER ASI,
                             ASI_STREAM_PROPERTY ASI_stream_property,
                             HASISTREAM stream,
                             RIB_ENTRY_TYPE type )
{
  HINTENUM next;
  RIB_INTERFACE_ENTRY attrib;

  //
  // enumerate the properties
  //

  next = HINTENUM_FIRST;

  while (RIB_enumerate_interface( ASI,
                                  "ASI stream",
                                  type,
                                  &next,
                                  &attrib ) )
  {
    void * value;
    ASI_stream_property( stream, attrib.token, &value, 0, 0 );
    printf("   %s = %s\n",
               attrib.entry_name,
               RIB_type_string( &value,
               attrib.subtype) );
  }
}


// ---------------------------------------------------------------------------
// stream_CB()
// ---------------------------------------------------------------------------

static U8 FAR* Source=0;
static U32 SourceLength=0;
static U32 SourcePosition=0;

static S32 AILCALLBACK Stream_CB( UINTa     user,
                                  void FAR *dest,
                                  S32       bytesrequested,
                                  S32       offset )
{
  user=user;
 
  //
  // new position, if so, change the source position
  //
  
  if (offset != -1)
  {
    SourcePosition = (U32) offset;
  }

  //
  // make sure we don't read too much
  //

  if ((SourcePosition + bytesrequested) > SourceLength)
  {
    bytesrequested = (S32) ( SourceLength - SourcePosition );
  }

  //
  // copy the data to the destination
  //

  memcpy( dest, &Source[SourcePosition], (size_t) bytesrequested );

  //
  // update our position
  //

  SourcePosition += bytesrequested;

  //
  // return how many bytes we actually copied
  //

  return bytesrequested;
}

// ---------------------------------------------------------------------------
// shutdown
// ---------------------------------------------------------------------------

static void MSS_MAIN_DEF Shutdown(void)
{
  AIL_shutdown();

  printf( "\nASIFILE stopped.\n" );
}


// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------

int MSS_MAIN_DEF main( int argc, char *argv[] )
{
  HPROVIDER ASI;
  HASISTREAM stream;

  set_up_console( 0 );

  printf( "_______________________________________________________________________________\n\n" );
  printf( "ASI file test bed\n" );
  printf( "_______________________________________________________________________________\n\n" );

  if (argc < 2)
  {
    printf( "Usage asifile infile [outfile]\n\n" );
    printf( "If infile is a .WAV or .MP3 file, source parms will automatically be determined\n" );
    printf( "If infile is a .RAW file, ASIFILE.CPP must be edited to supply parameters\n" );

    printf( "\nEnter filenames: ");
    get_args( &argc, &argv );
  }

  //
  // set the redist directory and start up the system
  //

  #ifdef IS_STATIC // xbox and dos
    Register_RIB(MP3Dec);
  #endif    

  AIL_set_redist_directory( MSS_DIR_UP_TWO MSS_REDIST_DIR_NAME );

  AIL_startup();
  
  atexit( Shutdown );

  //
  // get the filenames to use
  //

  char infile[256];
  char outfile[256];

  strcpy( infile, argv[1] );
  strcpy( outfile, (argc==3)?argv[2]:"null.raw" );

  printf( "Loading input file, please wait...\n" );

  //
  // Load source data
  //

  Source = (U8 FAR *) AIL_file_read( infile, 0 );

  if (Source == NULL)
  {
    printf( AIL_last_error() );
    exit( 1 );
  }

  WAVEOUT FAR *wav = (WAVEOUT FAR *) Source;

  //
  // Parameters to be used for raw file input...
  //

  UINTa source_rate     = 44100;
  UINTa source_bits     = 16;
  UINTa source_channels = 2;

  if ( !memcmp( wav->wavemark, "WAVE", 4 ))
  {
    //
    // this is a wave file

    SourceLength = wav->datalen;
    SourcePosition = (U32) ((U8*)wav->data-(U8*)wav);

    if (wav->fmttag != WAVE_FORMAT_PCM)
    {
      printf( "Cannot process compressed wave files.\n" );
      exit( 1 );
    }

    source_rate     = wav->sampersec;
    source_bits     = wav->bitspersam;
    source_channels = wav->channels;

    printf( "Done.  Source identified as .WAV file, " );
    printf( "%d Hz %d-bit %s\n\n",
            source_rate,
            source_bits,
            (source_channels == 2) ? "stereo" : "mono" );

    //
    // Find ASI decoder provider which understands the input wave type
    //

    ASI = RIB_find_file_dec_provider( "ASI codec",
                                      "Input wave tag",
                                      wav->fmttag,
                                      "Output file types",
                                      outfile );

  }
  else
  {
    printf( "Done.  Source is not a wave file.\n\n" );

    SourceLength = (U32) AIL_file_size(infile);
    SourcePosition = 0;

    //
    // Find ASI decoder provider which understands the suffixes
    //

    ASI = RIB_find_files_provider( "ASI codec",
                                   "Input file types",
                                   infile,
                                   "Output file types",
                                   outfile );


  }

  if (ASI == 0)
  {
    printf( "No provider available for specified file types.\n" );
    exit( 1 );
  }

  //
  // function pointers to ASI functions
  //

  ASI_ERROR                 ASI_error;

  ASI_STREAM_OPEN           ASI_stream_open;
  ASI_STREAM_PROCESS        ASI_stream_process;
  ASI_STREAM_SEEK           ASI_stream_seek;
  ASI_STREAM_CLOSE          ASI_stream_close;
  ASI_STREAM_PROPERTY       ASI_stream_property;

  //
  // data values to ASI preferences
  //

  HPROPERTY   INPUT_BIT_RATE;
  HPROPERTY   INPUT_SAMPLE_RATE;
  HPROPERTY   INPUT_BITS;
  HPROPERTY   INPUT_CHANNELS;
  HPROPERTY   OUTPUT_BIT_RATE;
  HPROPERTY   OUTPUT_SAMPLE_RATE;
  HPROPERTY   OUTPUT_BITS;
  HPROPERTY   OUTPUT_CHANNELS;
  HPROPERTY   POSITION;
  HPROPERTY   PERCENT_DONE;
  HPROPERTY   MIN_INPUT_BLOCK_SIZE;
  HPROPERTY   RAW_RATE;
  HPROPERTY   RAW_BITS;
  HPROPERTY   RAW_CHANNELS;
  HPROPERTY   REQUESTED_BIT_RATE;
  HPROPERTY   REQUESTED_RATE;
  HPROPERTY   REQUESTED_BITS;
  HPROPERTY   REQUESTED_CHANS;
  HPROPERTY   REQUESTED_LAYER;

  //
  // structure used to lookup the codec functions
  //

  RIB_INTERFACE_ENTRY ASICODEC[] =
  {
    FN( ASI_error )
  };

  //
  // structure used to lookup the stream functions and the stream preferences
  //

  RIB_INTERFACE_ENTRY ASISTR[] =
  {
    FN( ASI_stream_property ),
    FN( ASI_stream_open ),
    FN( ASI_stream_seek ),
    FN( ASI_stream_close ),
    FN( ASI_stream_process ),

    PR( "Input bit rate",           INPUT_BIT_RATE ),
    PR( "Input sample rate",        INPUT_SAMPLE_RATE ),
    PR( "Input sample width",       INPUT_BITS ),
    PR( "Input channels",           INPUT_CHANNELS ),
    PR( "Output bit rate",          OUTPUT_BIT_RATE ),
    PR( "Output sample rate",       OUTPUT_SAMPLE_RATE ),
    PR( "Output sample width",      OUTPUT_BITS ),
    PR( "Output channels",          OUTPUT_CHANNELS ),
    PR( "Position",                 POSITION ),
    PR( "Percent done",             PERCENT_DONE ),
    PR( "Minimum input block size", MIN_INPUT_BLOCK_SIZE ),
    PR( "Raw source sample rate",   RAW_RATE ),
    PR( "Raw source sample width",  RAW_BITS ),
    PR( "Raw source channels",      RAW_CHANNELS ),
    PR( "Requested bit rate",       REQUESTED_BIT_RATE ),
    PR( "Requested sample rate",    REQUESTED_RATE ),
    PR( "Requested sample width",   REQUESTED_BITS ),
    PR( "Requested channels",       REQUESTED_CHANS ),
    PR( "Layer",                    REQUESTED_LAYER )
  };

  //
  // the function calls to do the actual lookups
  //

  RIB_request( ASI, "ASI codec", ASICODEC );
  RIB_request( ASI, "ASI stream", ASISTR );

  //
  // Open stream with codec, registering callback function
  //

  stream = ASI_stream_open( 0,
                            Stream_CB,
                            SourceLength );

  if (stream == NULL)
  {
    printf( "Could not open stream (%s)\n", ASI_error() );
  }

  //
  // Set stream parameters
  //

  UINTa req_layer = 3;

  ASI_stream_property( stream,
                       REQUESTED_LAYER,
                       0,&req_layer,0 );

  ASI_stream_property( stream,
                       RAW_RATE,
                       0,&source_rate,0 );

  ASI_stream_property( stream,
                       RAW_BITS,
                       0,&source_bits,0 );

  ASI_stream_property( stream,
                       RAW_CHANNELS,
                       0,&source_channels,0 );

  //
  // open the output file (if one is specified)
  //

  FILE* file = 0;

  if (argc == 3)
  {
    file = fopen( outfile, "wb" );

    if (file == 0)
    {
      printf( "Could not open output file\n" );
      exit( 1 );
    }
  }

  S32 len;

  U8* buffer = (U8*)AIL_mem_alloc_lock(32768);
  if ( buffer == 0 )
  {
     printf( "Out of memory.\n" );
     exit( 1 );
  }

  printf( "Converting, please wait...\n" );
  U32 timer=AIL_ms_count();

  //
  // Keep reading the file until done
  //

  S32 tick = AIL_ms_count();

  while ((len = ASI_stream_process( stream,
                                    buffer,
                                    32768 ) ) != 0)
  {
    if (file != 0)
    {
    fwrite( buffer, 1, (size_t) len, file );
    }

    //
    // print the percentage once per second
    //

    S32 now = AIL_ms_count();

    if ((now - tick) > 1000)
      {
      tick = now;

      if ( PERCENT_DONE != (UINTa) -1 )
         {
         F32 percent;
         if ( ASI_stream_property( stream, PERCENT_DONE, &percent, 0, 0 ) )
            printf( "\r%s", RIB_type_string( &percent, RIB_PERCENT ) );
         }

      if (kbhit())
         {
         getch();
         break;
         }
      }
  }

  //
  // close the file
  //

  if (file != 0)
  {
    fclose( file );
  }

  //
  // finished - display the total time
  //

  timer = AIL_ms_count()-timer;

  printf( "\n\nDone.  Total time converting (in milliseconds): %d.\n",timer );

  //
  // Show properties
  //

  Enum_properties( ASI, ASI_stream_property, stream, RIB_PROPERTY );

  //
  // Exit w/success
  //

  ASI_stream_close( stream );

  if ( ASI_error() != NULL )
  {
    printf( "Error: %s\n", ASI_error() );
  }

  return( 0 );
}
