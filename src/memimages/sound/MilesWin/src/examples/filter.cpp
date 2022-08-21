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


HDIGDRIVER Digital = NULL;

static char properties[128][128];
static S32 is_boolean[128];
static S32 propertycount = 0;


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
    void * value;

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
      void * value;

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
//## Ask the user what filter they want to test                             ##
//##                                                                        ##
//############################################################################

static HPROVIDER Ask_user_for_filter()
{
  HPROVIDER avail[256];
  C8   FAR *name;
  U32       n = 0;

  printf("Available filter providers:\n\n");

  HPROENUM next = HPROENUM_FIRST;

  //
  // display each filter and save the provider numbers in "avail"
  //

  while (AIL_enumerate_filters( &next,
                                &avail[n],
                                &name ) )
  {
    printf("   %c: %s\n",'a' + n++,name);
  }

  //
  // wait for the user to choose a filter
  //

  for(;;)
  {
    printf("\nEnter choice or ESC to exit: ");

    S32 index = getch();

    if (index == 27)
    {
      exit( 1 );
    }

    printf( "%c\n" ,index);

    if ((U32)(index - 'a') < n)
    {
      return( avail[index-'a'] );
    }
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
  // Offer provider selection menu to user
  //

  filter = Ask_user_for_filter();

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

  Digital = 0; // closed in shutdown  
  AIL_shutdown();
  return( 0 );
}

