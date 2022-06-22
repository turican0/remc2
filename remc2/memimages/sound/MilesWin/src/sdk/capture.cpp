//############################################################################
//##                                                                        ##
//##  Miles Sound System                                                    ##
//##                                                                        ##
//##  API.CPP: FLT module for capturing output data to a wave file          ##
//##                                                                        ##
//##  32-bit protected-mode source compatible with MSC 11.0/Watcom 10.6     ##
//##                                                                        ##
//##  Version 1.00 of 5-Feb-99: Initial                                     ##
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


//############################################################################
//
// How to use this filter:
//
//   1) Get filter's HPROVIDER via enumeration or AIL_find_filter("Capture Filter") 
//   2) Set the filter-level preference "Filename" to the output filename
//   3) Associate the filter with the driver with AIL_open_filter()
//
//############################################################################


#if defined(_XENON) || (_XBOX_VER == 200) || defined(_XBOX)
#include <xtl.h>
#elif defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(__NT__) || defined(__WIN32__)
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#define STRICT
#include <windows.h>
#endif

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "mss.h"
#include "imssapi.h"

#if defined( IS_WIN32API ) && !defined(FORNONWIN)

#define CAPTURE_BUFFER_SIZE 65536*16     // 1 meg capture buffer by default

//
// Attribute tokens
//

enum PROP
{
   _FX_PROVIDER_FLAGS,

  FILENAME,
  SERVICE_THREAD,
  RECORD_ENABLED,
};

//
// Driver state descriptor
//
// One state descriptor is associated with each HDIGDRIVER
//

struct DRIVERSTATE
{
   //
   // Members common to all pipeline filter providers
   //

   HDIGDRIVER       dig;                  // Driver with which this descriptor is associated

   S32 FAR         *build_buffer;         // Location and size of driver's mixer output buffer
   S32              build_buffer_size;

   //
   // Members associated with specific filter provider
   //

   U8 FAR         *capture_buffer;        // start of capture buffer
   U8 FAR         *capture_end;           // end of capture buffer
   S32             capture_buffer_size;

   U8 FAR         *current_head;          // writing data out of here
   U8 FAR         *current_tail;          // adding data here

   S32            capturing;
   S32            rate;
   S32            format;
   HANDLE         fhand;
   U32            total_written;
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
   // Members associated with specific filter provider (none)
   //

};

//
// Globals
//

static S32 FLT_started = 0;

static C8 FLT_error_text[256];

static char filename[256]="default.wav";
static char oldfilename[256]="default.wav";

static S32 enabled = 0;

static HANDLE threadhandle,threadwait;

//############################################################################
//#                                                                          #
//# Retrieve a standard RIB provider attribute by index                      #
//#                                                                          #
//############################################################################

static S32 AILCALL PROVIDER_property (HPROPERTY index, void FAR * before_value, void const FAR * new_value, void FAR * after_value)
{
   switch ( /* (PROP) */ index )
      {
      case PROVIDER_NAME:
        if (before_value)
        {
          *(char FAR * FAR*)before_value = "Capture Filter";
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
          *(S32 FAR*)before_value = FPROV_ON_POSTMIX;
          return 1;
        }
        break;

      case RECORD_ENABLED:
        if (before_value) 
        {
          *(S32 FAR*)before_value = enabled;
        }
        if (new_value)
         {
         enabled = *(S32 FAR *) new_value;
         }
       if (after_value)
         {
          *(S32 FAR*)after_value = enabled;
         }

        return 1;

     case SERVICE_THREAD:
        if (before_value) 
        {
          *(SINTa FAR*) before_value = (SINTa) threadhandle;
          return 1;
        }
        break;

     case FILENAME:
       if (new_value)
       {
          AIL_strcpy(oldfilename,filename);
          AIL_strcpy(filename,(char const FAR *)new_value);
       }
       if (before_value) *(char FAR * FAR*)before_value = oldfilename;
       if (after_value) *(char FAR * FAR*)before_value = filename;
       return 1;

      }
  return 0;
}

//############################################################################
//#                                                                          #
//# Function to write out a wave file header                                 #
//#                                                                          #
//############################################################################

static void write_header(DRIVERSTATE* DRV)
{
   WAVEOUT wo;
   DWORD wrote;

   AIL_memcpy(&wo.riffmark,"RIFF",4);
   STORE_LE_SWAP32(&wo.rifflen, DRV->total_written+sizeof(WAVEOUT)-8);

   AIL_memcpy(&wo.wavemark,"WAVE",4);
   AIL_memcpy(&wo.fmtmark,"fmt ",4);
   STORE_LE_SWAP32(&wo.fmtlen,16);
   STORE_LE_SWAP16(&wo.fmttag,WAVE_FORMAT_PCM);
   STORE_LE_SWAP16(&wo.channels,(S16)((DRV->format&(DIG_F_STEREO_MASK | DIG_F_MULTICHANNEL_MASK))?2:1));
   STORE_LE_SWAP32(&wo.sampersec,DRV->rate);
   STORE_LE_SWAP16(&wo.bitspersam,(S16)((DRV->format&DIG_F_16BITS_MASK)?16:8));
   STORE_LE_SWAP16(&wo.blockalign,(S16)(((S32)wo.bitspersam*(S32)wo.channels) / 8));
   STORE_LE_SWAP32(&wo.avepersec,(DRV->rate *(S32)wo.bitspersam*(S32)wo.channels) / 8);
   AIL_memcpy(&wo.datamark,"data",4);
   STORE_LE_SWAP32(&wo.datalen,DRV->total_written);

   SetFilePointer(DRV->fhand,0,0,FILE_BEGIN);
   WriteFile(DRV->fhand,&wo,sizeof(wo),&wrote,0);
}


//############################################################################
//#                                                                          #
//# Function to dump data accumulated in the capture buffer                  #
//#                                                                          #
//############################################################################

static S32 in_do_output=0;

static void do_output(DRIVERSTATE* DRV)
{
  MSSLockedIncrement(in_do_output);

  while (in_do_output!=1)
    Sleep(1);

  while (DRV->current_tail!=DRV->current_head) {
    UINTa writeamt;
    DWORD wrote;

    writeamt=(DRV->current_tail<DRV->current_head)?(DRV->capture_end-DRV->current_head):(DRV->current_tail-DRV->current_head);
    if (writeamt>32768)
      writeamt=32768;

    WriteFile(DRV->fhand,DRV->current_head,(DWORD) writeamt,&wrote,0);

    Sleep(1);

    DRV->current_head+=writeamt;
    if (DRV->current_head>=DRV->capture_end)
      DRV->current_head-=DRV->capture_buffer_size;

    DRV->total_written+=wrote;
  }

  MSSLockedDecrement(in_do_output);
}


//############################################################################
//#                                                                          #
//# Return FLT filter error text                                             #
//#                                                                          #
//############################################################################

static C8 FAR *       AILCALL FLT_error       (void)
{
   if (!AIL_strlen(FLT_error_text))
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
   // Init static prefs/attributes
   //

   FLT_error_text[0]            = 0;

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
//# Background thread that does the writing to disk                          #
//#                                                                          #
//############################################################################

DWORD WINAPI write_thread(LPVOID user)
{
  DRIVERSTATE* DRV=(DRIVERSTATE*)user;

  while (WaitForSingleObject(threadwait,300)==WAIT_TIMEOUT) {

    if (DRV->capturing)
      do_output(DRV);

  }

  return(0);
}

//############################################################################
//#                                                                          #
//# Allocate driver-specific descriptor                                      #
//#                                                                          #
//############################################################################

static HDRIVERSTATE AILCALL FLT_open_driver (HDIGDRIVER dig)

{
   U32 id;
   DRIVERSTATE FAR *DRV;

   if (*filename==0) {
     strcpy(FLT_error_text,"NULL filename");
     return(0);
   }

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
   // Filter captures contents of build buffer 0 (front left/right) by default
   //

   DRV->dig               = dig;
   DRV->build_buffer      = dig->build[0].buffer;
   DRV->build_buffer_size = dig->build[0].bytes;

   //
   // Initialize provider-specific members to their default values
   //

   DRV->capturing=0;

   DRV->capture_buffer_size=CAPTURE_BUFFER_SIZE;

   DRV->capture_buffer = (U8 FAR *) AIL_mem_alloc_lock(DRV->capture_buffer_size);
   if (DRV->capture_buffer == NULL)
      {
      strcpy(FLT_error_text,"Could not allocate capture buffer");
      AIL_mem_free_lock(DRV);
      return 0;
      }

   DRV->capture_end=DRV->capture_buffer+DRV->capture_buffer_size;

   DRV->current_head=DRV->capture_buffer;
   DRV->current_tail=DRV->capture_buffer;

   AIL_digital_configuration(dig,&DRV->rate,&DRV->format,0);

   //
   // Create thread to perform output
   // TODO: Make a single thread perform output writes for multiple drivers
   // (and make filenames non-global, too)
   //

   threadwait=CreateEvent(0,TRUE,0,0);
   if (threadwait==0) {
     strcpy(FLT_error_text,"Could not create thread event");
     goto err;
   }

   threadhandle=CreateThread(0,0,write_thread,(LPVOID)DRV,0,(LPDWORD)&id);
   if (threadhandle==0) {
     strcpy(FLT_error_text,"Could not create IO thread");
     goto err;
   }

   DRV->fhand=CreateFile(filename,
                        GENERIC_WRITE,
                        0,
                        NULL,
                        CREATE_ALWAYS,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL);

   if (DRV->fhand==INVALID_HANDLE_VALUE) {
     strcpy(FLT_error_text,"Could not create file");
    err:
     AIL_mem_free_lock(DRV->capture_buffer);
     AIL_mem_free_lock(DRV);
     return(NULL);
   }

   DRV->current_head=DRV->capture_buffer;
   DRV->current_tail=DRV->capture_buffer;
   DRV->total_written=0;

   write_header(DRV);

   DRV->capturing=1;

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

   // shut down the thread
   SetEvent(threadwait);
   WaitForSingleObject(threadhandle,INFINITE);
   CloseHandle(threadwait);
   CloseHandle(threadhandle);

   //turn off capturing
   if (DRV->capturing) {
     DRV->capturing=0;
     do_output(DRV);
     write_header(DRV);
     CloseHandle(DRV->fhand);
   }

   if (DRV->capture_buffer != NULL)
      {
      AIL_mem_free_lock(DRV->capture_buffer);
      DRV->capture_buffer = NULL;
      }

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

void AILCALL FLT_premix_process (HDRIVERSTATE driver)
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

S32 AILCALL FLT_postmix_process (HDRIVERSTATE driver, //)
                                   void FAR *   output_buffer)
{
   if (!enabled)
      {
      return 0;
      }

   DRIVERSTATE FAR *DRV = (DRIVERSTATE FAR *) driver;

   S32 FAR *src =  DRV->build_buffer;

   UINTa i = DRV->build_buffer_size / sizeof(S32);

   //calculate how much space we have left
   UINTa leftincap=(UINTa)DRV->current_head;
   if (leftincap==(UINTa)DRV->current_tail)
     leftincap=DRV->capture_buffer_size;
   else if (leftincap>(UINTa)DRV->current_tail)
     leftincap=(leftincap-(UINTa)DRV->current_tail)-2;
   else
     leftincap=((DRV->capture_buffer_size+leftincap)-(UINTa)DRV->current_tail)-2;

   if (DRV->format&DIG_F_16BITS_MASK) {
     // convert to 16-bit output

     leftincap/=2;

     if (i>leftincap)
       i=leftincap;

     while (i--) {
        S32 val;

        val=(*src++>>11);

        if (val>32767)
          val=32767;
        else if (val<-32768)
          val=-32768;

        STORE_LE_SWAP16((S16*)DRV->current_tail,(S16)val);

        DRV->current_tail+=2;
        if (DRV->current_tail>=DRV->capture_end)
          DRV->current_tail-=DRV->capture_buffer_size;
      }
   } else {
     // convert to 8-bit output
     if (i>leftincap)
       i=leftincap;

     while (i--) {
        S32 val;

        val=(*src++>>19);

        if (val>127)
          val=127;
        else if (val<-128)
          val=-128;

        *DRV->current_tail++=((U8)val)+128;
        if (DRV->current_tail>=DRV->capture_end)
          DRV->current_tail-=DRV->capture_buffer_size;
     }
   }

   return 0;
}

extern "C" S32 CaptureMain( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 CaptureMain( HPROVIDER provider_handle, U32 up_down )
{
   const RIB_INTERFACE_ENTRY FLT[] =
      {
      REG_FN(PROVIDER_property),
      REG_PR("Name",                     PROVIDER_NAME,        (RIB_DATA_SUBTYPE) (RIB_STRING|RIB_READONLY)),
      REG_PR("Version",                  PROVIDER_VERSION,     (RIB_DATA_SUBTYPE) (RIB_HEX|RIB_READONLY)),
      REG_PR("Flags",                   _FX_PROVIDER_FLAGS,    (RIB_DATA_SUBTYPE) (RIB_HEX|RIB_READONLY)),

      REG_FN(FLT_startup),
      REG_FN(FLT_error),
      REG_FN(FLT_shutdown),

      REG_FN(FLT_open_driver),
      REG_FN(FLT_close_driver),

      REG_FN(FLT_premix_process),
      REG_FN(FLT_postmix_process),

      REG_PR("Filename", FILENAME, RIB_STRING),
      REG_PR("Service thread", SERVICE_THREAD, (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_HEX)),
      REG_PR("Enabled", RECORD_ENABLED, (RIB_DATA_SUBTYPE) (RIB_BOOL)),
      };

   if (up_down)
      {
      RIB_register(provider_handle,
                  "MSS pipeline filter",
                   FLT);
      }
   else
      {
      RIB_unregister_all(provider_handle);
      }

   return TRUE;
}

#endif
