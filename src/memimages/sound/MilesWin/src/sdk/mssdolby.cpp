//############################################################################
//##                                                                        ##
//##  Miles Sound System                                                    ##
//##                                                                        ##
//##  DOLBY.CPP: Dolby Surround matrix provider                             ##
//##                                                                        ##
//##  32-bit protected-mode source compatible with MSC 11.0/Watcom 10.6     ##
//##                                                                        ##
//##  Version 1.00 of 4-May-05: Initial                                     ##
//##                                                                        ##
//##  Author: John Miles                                                    ##
//##                                                                        ##
//############################################################################
//##                                                                        ##
//##  Copyright (C) RAD Game Tools, Inc.                                    ##
//##                                                                        ##
//##  Contact RAD Game Tools at 425-893-4300 for technical support.         ##
//##                                                                        ##
//############################################################################

#define INITGUID

#define  diag_printf //AIL_debug_printf

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>

#include "mss.h"
#include "imssapi.h"

//
// Property tokens
//

#define MAX_DELAY 32

enum PROPERTY
{
   //
   // Additional filter attribs (beyond standard RIB PROVIDER_ properties)
   //

   _FX_PROVIDER_FLAGS,
   _FX_DOLBY_DELAY
};

//
// Driver state descriptor
//
// One state descriptor is associated with each HDIGDRIVER
//

struct DRIVERSTATE
{
   HDIGDRIVER dig;                  // Driver with which this descriptor is associated

   S32 buffer[ MAX_DELAY ];
   S32 buffer_pos;
};

//
// Per-sample filter state descriptor
//
// One state descriptor is associated with each HSAMPLE
//

struct SAMPLESTATE
{
};

//
// Globals
//

static S32 FLT_started = 0;

static C8 FLT_error_text[256];

static S32 delay;

//############################################################################
//#                                                                          #
//# Retrieve a standard RIB provider property by index                       #
//#                                                                          #
//############################################################################

static S32 AILCALL PROVIDER_property (HPROPERTY index, void FAR * before_value, void const FAR * new_value, void FAR * after_value)
{
   switch ( index )
      {
      case PROVIDER_NAME:
        if (before_value)
        {
          *(char FAR * FAR*)before_value = "Dolby Surround Matrix Filter";
          return 1;
        }
        break;
        
      case PROVIDER_VERSION:
        if (before_value) 
        {
          *(S32 FAR*)before_value = 0x110;
          return 1;
        }
        break;

      case _FX_PROVIDER_FLAGS: 
        if (before_value) 
        {
          *(S32 FAR*)before_value = FPROV_MATRIX;
          return 1;
        }
        break;

      case _FX_DOLBY_DELAY: 
        if (before_value) 
        {
          *(S32 FAR*)before_value = delay;
          return 1;
        }
        if (new_value)
        {
         delay = *(S32 FAR *) new_value;

         if (delay > MAX_DELAY)
            {
            delay = MAX_DELAY;
            }
        }
        if (after_value) 
        {
          *(S32 FAR*)after_value = delay;
          return 1;
        }
        break;
      }
  return 0;
}

//############################################################################
//#                                                                          #
//# Return FLT filter error text                                             #
//#                                                                          #
//############################################################################

static C8 FAR *       AILCALL FLT_error       (void)
{
   if (FLT_error_text[0]==0)
      {
      return 0;
      }

   return FLT_error_text;
}

//############################################################################
//#                                                                          #
//# Initialize FLT sample filter                                             #
//#                                                                          #
//############################################################################

static FLTRESULT AILCALL FLT_startup     (void)
{
   if (FLT_started++)
      {
      strcpy(FLT_error_text,"Already started");
      return FLT_ALREADY_STARTED;
      }

   //
   // Init static prefs/properties
   //

   FLT_error_text[0] = 0;

   delay = MAX_DELAY;

   return FLT_NOERR;
}

//############################################################################
//#                                                                          #
//# Shut down FLT sample filter                                              #
//#                                                                          #
//############################################################################

static FLTRESULT      AILCALL FLT_shutdown    (void)
{
   if (!FLT_started)
      {
      strcpy(FLT_error_text,"Not initialized");
      return FLT_NOT_INIT;
      }

   --FLT_started;

   return FLT_NOERR;
}

//############################################################################
//#                                                                          #
//# Allocate driver-specific descriptor                                      #
//#                                                                          #
//############################################################################

static HDRIVERSTATE AILCALL FLT_open_driver (HDIGDRIVER dig)

{
   DRIVERSTATE FAR *DRV;

   DRV = (DRIVERSTATE *) AIL_mem_alloc_lock(sizeof(DRIVERSTATE));

   if (DRV == NULL)
      {
      strcpy(FLT_error_text,"Out of memory");
      return 0;
      }

   AIL_memset(DRV,
              0,
              sizeof(DRIVERSTATE));

   //
   // Initialize generic members
   //

   DRV->dig = dig;

   DRV->buffer_pos = 0;

   //
   // Return descriptor address cast to handle
   //

   return (HSAMPLESTATE) DRV;
}

//############################################################################
//#                                                                          #
//# Close filter driver instance                                             #
//#                                                                          #
//############################################################################

static FLTRESULT     AILCALL FLT_close_driver (HDRIVERSTATE state)
{
   DRIVERSTATE FAR *DRV = (DRIVERSTATE FAR *) state;

   AIL_mem_free_lock(DRV);

   return FLT_NOERR;
}

//############################################################################
//#                                                                          #
//#  Perform any needed processing before per-sample mixing begins           #
//#                                                                          #
//#  Called after the build buffer has been flushed prior to the mixing      #
//#  phase, but before any samples have been mixed into it                   #
//#                                                                          #
//############################################################################

static void AILCALL FLT_premix_process (HDRIVERSTATE driver)
{
}

//############################################################################
//#                                                                          #
//#  Process data after mixing                                               #
//#                                                                          #
//#  Called after all samples have been mixed into the 32-bit build buffer,  #
//#  prior to copying the build-buffer contents to the driver's output       #
//#  buffer                                                                  #
//#                                                                          #
//############################################################################

static S32 AILCALL FLT_postmix_process (HDRIVERSTATE driver, //)
                                   void FAR *   output_buffer)
{
   DRIVERSTATE FAR *DRV = (DRIVERSTATE FAR *) driver;

   HDIGDRIVER dig = DRV->dig;

   //
   // Following code assumes one stereo build buffer (LF/RF) and one mono build buffer
   // (CS).  Any other configuration will fail!
   //

   S32 m;
   S32 n_samples = dig->samples_per_buffer;

#ifdef IS_PS2
   S32 s;

   //
   // PS2 version writes Dolby-encoded output to 16:11 working buffer
   // without clamping
   //

   static S32 working_buffer[1024] __attribute__((aligned (16))); 

   S32 *dest = working_buffer;

   for (m=0; m < n_samples; m++)
      {
      s = m << 1;

      // get input samples
      S32 l = dig->build[0].buffer [s  ];
      S32 r = dig->build[0].buffer [s+1]; 
      S32 b = dig->build[1].buffer [m];

      S32 db = DRV->buffer[DRV->buffer_pos];
      DRV->buffer[DRV->buffer_pos++] = b;

      if ( DRV->buffer_pos >= delay) DRV->buffer_pos = 0; 

      // store output
      dest[0] = (l + db) / 2;
      dest[1] = (r - db) / 2;

      dest += 2;
      }

   //
   // Convert to 16-bit format, clamp, apply 256-sample interleave, and write/add to
   // output buffer
   //

   MSS_mixer_copy(working_buffer,
                  4096,
                  output_buffer,
                  0,
                  dig->IOP_overwrite);

#elif defined(IS_WII)

   // wii stores left and the right in separate buffers (right pointer is passed in
   //   as the first word of the left buffer)

  #define clip2( v1, v2 )    \
  subi v1, v1, 32767       ; \
  subi v2, v2, 32767       ; \
  srawi temp1, v1, 31      ; \
  srawi temp2, v2, 31      ; \
  and v1, v1, temp1        ; \
  and v2, v2, temp2        ; \
  add v1, v1, val65535     ; \
  add v2, v2, val65535     ; \
  srawi temp1, v1, 31      ; \
  srawi temp2, v2, 31      ; \
  andc v1, v1, temp1       ; \
  andc v2, v2, temp2       ; \
  subi v1, v1, 32768       ; \
  subi v2, v2, 32768       ; \


   U32 *dest = (U32 *) output_buffer; // 16-bit left samples 
   U32 *dest_r = (U32 *) dest[0];     // 16-bit right samples 
   S32 *src =dig->build[0].buffer;
   S32 *src_b =dig->build[1].buffer;
    
   register U32 val65535 = 65535;

   for (m=(n_samples/2); m; m--)
   {
      // get input samples
      register S32 sl = src[0];
      register S32 sr = src[1]; 
      register S32 sl2 = src[2];
      register S32 sr2 = src[3]; 
      register S32 b = src_b[0];
      register S32 b2 = src_b[1];
      register U32 temp1;
      register U32 temp2;

      sl >>= 11;
      sr >>= 11;
      sl2 >>= 11;
      sr2 >>= 11;
      b >>= 11;
      b2 >>= 11;

      src += 4;
      src_b += 2;
      
      __asm
      {
        clip2( sl, sr );
        clip2( sl2, sr2 );
        clip2( b, b2 );
      }

      S32 db = DRV->buffer[DRV->buffer_pos];
      DRV->buffer[DRV->buffer_pos++] = b;
      if ( DRV->buffer_pos >= delay) DRV->buffer_pos = 0; 
      S32 db2 = DRV->buffer[DRV->buffer_pos];
      DRV->buffer[DRV->buffer_pos++] = b2;
      if ( DRV->buffer_pos >= delay) DRV->buffer_pos = 0; 

      sl = (sl + db) / 2;
      sr = (sr - db) / 2;

      sl2 = (sl2 + db2) / 2;
      sr2 = (sr2 - db2) / 2;

      // store output
      *dest++ = ( sl << 16 ) | ( sl2 & 0xffff );
      *dest_r++ = ( sr << 16 ) | ( sr2 & 0xffff );
   }
#else
   U32 *dest = (U32 *) output_buffer; // 16-bit stereo samples in LR order

   S32 *src =dig->build[0].buffer;
   S32 *src_b =dig->build[1].buffer;

   for (m=n_samples;m; m--)
      {
      // get input samples
      S32 l = src[0];
      S32 r = src[1];
      S32 b = src_b[0];

      src += 2;
      ++src_b;

      l >>= 11;
      r >>= 11;
      b >>= 11;

      if      (l < -32768) l = -32768;
      else if (l > 32767) l = 32767;

      if      (r < -32768) r = -32768;
      else if (r > 32767) r = 32767;

      if      (b < -32768) b = -32768;
      else if (b > 32767) b = 32767;

      S32 db = DRV->buffer[DRV->buffer_pos];
      DRV->buffer[DRV->buffer_pos++] = b;

      if ( DRV->buffer_pos >= delay) DRV->buffer_pos = 0; 

      l = (l + db) / 2;
      r = (r - db) / 2;

#ifdef IS_BE
      *dest++ = ( l << 16 ) | ( r & 0xffff );
#else
      *dest++ = ( r << 16 ) | ( l & 0xffff );
#endif      
      }
#endif

   //
   // Inhibit rest of SS_copy() 
   //

   return 1;
}

//############################################################################
//#                                                                          #
//# RIB loader hook                                                          #
//#                                                                          #
//############################################################################

DXDEF S32 AILEXPORT RIB_MAIN_NAME(DolbySurround)( HPROVIDER provider_handle, U32 up_down )
{
   const RIB_INTERFACE_ENTRY FLT1[] =
      {
      REG_FN(PROVIDER_property),
      REG_PR("Name",    PROVIDER_NAME,     (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_STRING)),
      REG_PR("Version", PROVIDER_VERSION,  (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_HEX)),
      REG_PR("Flags",  _FX_PROVIDER_FLAGS, (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_HEX)),
      };

   const RIB_INTERFACE_ENTRY FLT2[] =
      {
      REG_PR("Rear Delay", _FX_DOLBY_DELAY, RIB_DEC),
      REG_FN(FLT_startup),
      REG_FN(FLT_error),
      REG_FN(FLT_shutdown),
      };

   const RIB_INTERFACE_ENTRY FLT3[] =
      {
      REG_FN(FLT_open_driver),
      REG_FN(FLT_close_driver),
      REG_FN(FLT_premix_process),
      REG_FN(FLT_postmix_process),
      };

   if (up_down)
      {
      RIB_register(provider_handle, "MSS pipeline filter", FLT1);
      RIB_register(provider_handle, "MSS pipeline filter", FLT2);
      RIB_register(provider_handle, "MSS pipeline filter", FLT3);
      }
   else
      {
      RIB_unregister_all(provider_handle);
      }

   return TRUE;
}

/* @cdep pre

  $if($ispresent($BuildDir,win64),
    $DefaultsDLL64EMT
    $set(outname,$clipfilename($outname)\mss64dolby.flt)
    $requiresbinary(mss64.lib)
    $requires(mss.rc)
  ,
  $if($ispresent($BuildDir,wii),
    $DefaultsWiiLIB 
  ,
  $if($ispresent($BuildDir,ps2),
    $DefaultsPSTwoLIB
  ,
    $DefaultsDLL
    $set(outname,$extension($outname,flt))
    $requires(mss.rc)
    $requires(ribdll.c)
  )
  )
  )

*/

/* @cdep post 

  $if($ispresent($BuildDir,win64),
    $BuildDLL64EMT( , -base:0x23000000)
  ,
  $if($ispresent($BuildDir,wii),
    $BuildWiiLib
  ,
  $if($ispresent($BuildDir,ps2),
    $BuildPSTwoLib
  ,
    $BuildDLL( , -base:0x23000000)
  )
  )
  )
*/


