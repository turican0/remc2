//############################################################################
//##                                                                        ##
//##  Miles Sound System                                                    ##
//##                                                                        ##
//##  mssdig.C: Digital Sound module for waveOut and DirectSound            ##
//##                                                                        ##
//##  16-bit protected-mode source compatible with MSC 7.0                  ##
//##  32-bit protected-mode source compatible with MSC 9.0                  ##
//##                                                                        ##
//##  Version 1.00 of 15-Feb-95: Derived from AILSS.C V1.03                 ##
//##          1.01 of 19-Jun-95: Stereo tracks panned for mono output       ##
//##                             Use multiply/shift for 16-bit scaling      ##
//##                             Digital master volume added                ##
//##                             AIL_resume_sample() restarts driver        ##
//##          1.02 of 16-Jul-95: Win95 thread synchronization added         ##
//##          1.03 of 21-Nov-95: API brought up to DOS 3.03C level          ##
//##                             Changed synchronization methods            ##
//##          1.04 of 15-Feb-96: Fixes for optimization and multiple        ##
//##                             16 bit loads (JKR)                         ##
//##          1.05 of 11-Apr-96: Added background thread checking (JKR)     ##
//##          1.06 of 11-May-97: Added IMA ADPCM support (Serge Plagnol)    ##
//##          1.10 of 10-Jun-98: Adapted for use with new mixer, many       ##
//##                             changes (JM)                               ##
//##          1.20 of 10-May-02: Massive changes for new reverb, many       ##
//##                             functions moved to wavefile.cpp (JKR)      ##
//##                                                                        ##
//##  Author: John Miles and Jeff Roberts                                   ##
//##                                                                        ##
//############################################################################
//##                                                                        ##
//##  Copyright (C) RAD Game Tools, Inc.                                    ##
//##                                                                        ##
//##  Contact RAD Game Tools at 425-893-4300 for technical support.         ##
//##                                                                        ##
//############################################################################

#define STRICT
#include <windows.h>
#include <mmsystem.h>
#include <ks.h>
#include <ksmedia.h>

#include "mss.h"
#include "imssapi.h"
          
#include <stdlib.h>

#ifdef IS_WIN32
#define DIRECTSOUND_VERSION 0x300
#include <dsound.h>
#endif

#ifndef IS_WIN32
#define WAVEFORMATEX WAVEFORMAT
#define LPWAVEFORMATEX LPWAVEFORMAT
#endif

#include <math.h>
#include <stdio.h>

#define SETTAG(var,tag) AIL_memcpy(var,tag,4);

//
// Macros to hide DirectSound typecasting
//

#define ASDSBUF(ptr) ((LPDIRECTSOUNDBUFFER)ptr)
#define ASDS(ptr)    ((LPDIRECTSOUND)ptr)

//
// Filter buffer size (in samples) for use with output filters
//
// Output filters with larger half-buffers than this won't work properly with
// pipeline filters!
//

#define MAX_OUTPUT_FILTER_SAMPLES 16384

//
// HDR_USER structure and associated flags
//

typedef struct                      // Descriptor appended to WAVEHDRs
{
   HDIGDRIVER owner;                // DIG_DRIVER associated with this WAVEHDR
   LPWAVEHDR  next;                 // Next WAVEHDR in circular chain
   U32        flags;                // Misc. flags (FORCE_POST, etc.)
}
HDR_USER;

#define HDR_FORCE_POST 0x00000001   // Force this WAVEHDR to be posted

//
// Macro for easy access to WAVEHDR.dwUser member as pointer to HDR_USER
//

#define CURRENT_USER ((HDR_USER FAR *) current->dwUser)

//
// Flag to disable background and foreground callbacks
//

static volatile S32 disable_callbacks = 0;


//
// Flag to keep SS_serve from reentering
//

static volatile S32 SS_servicing = 0;

//
// Primary digital driver (the one used with 3D providers and other
// future single-driver extensions)
//

extern "C"
{
HDIGDRIVER primary_digital_driver = NULL;
}

static void low_serve();

//############################################################################
//##                                                                        ##
//## Boolean macro: is sample 'done' or not yet started?                    ##
//##                                                                        ##
//## Returns TRUE if WHDR_DONE bit is set, or if HDR_FORCE_POST bit is set  ##
//## (used to indicate virgin sample that needs to be posted for the first  ##
//## time)                                                                  ##
//##                                                                        ##
//############################################################################

#define NEED_POST(x) (((x)->dwFlags & WHDR_DONE) || \
                     (((HDR_USER FAR *) (x)->dwUser)->flags & HDR_FORCE_POST))

//############################################################################
//#                                                                          #
//# waveOut emulation routines for DirectSound                               #
//#                                                                          #
//############################################################################

#ifdef IS_WIN32

//
// DirectSound DLL loaded count
//

static S32 DSDLLcount = 0;


//
// DirectSound DLL handle
//

static HINSTANCE DSDLLinstance=0;


//
// Pointer to the DirectSoundCreate function
//

typedef HRESULT (WINAPI *DirectSoundCreateType)(void* lpGUID,void* lpds,void* pUnkOuter);
static DirectSoundCreateType DirectSoundCreatePtr=0;


//
// Pointer to the DirectSoundEnumerate function
//

typedef BOOL (WINAPI *DirectSoundEnumerateType)(LPVOID enumcb,LPVOID context);
static DirectSoundEnumerateType DirectSoundEnumeratePtr=0;

//############################################################################
//##                                                                        ##
//## Load the DirectSound DLL                                               ##
//##                                                                        ##
//############################################################################

S8 DirectSoundLoad()
{
  UINT err;

  if (DSDLLcount++==0) {

    // load the DLL without an error message popping up if it's not there

    OutMilesMutex();
    err=SetErrorMode(0x8000);
    DSDLLinstance=LoadLibrary("DSOUND.DLL");
    SetErrorMode(err);

    Sleep(1);
    InMilesMutex();

    if (DSDLLinstance==0) {
      DSDLLcount--;
      return(0);
    }

    // get the function addresses

    DirectSoundCreatePtr=(DirectSoundCreateType)GetProcAddress(DSDLLinstance,"DirectSoundCreate");
    DirectSoundEnumeratePtr=(DirectSoundEnumerateType)GetProcAddress(DSDLLinstance,"DirectSoundEnumerateA");

    if ((DirectSoundCreatePtr==0) || (DirectSoundEnumeratePtr==0)) {
      FreeLibrary(DSDLLinstance);
      DSDLLcount--;
      return(0);
    }
  }
  return(1);
}


//############################################################################
//##                                                                        ##
//## Unload the DirectSound DLL                                             ##
//##                                                                        ##
//############################################################################

void DirectSoundUnload()
{
  OutMilesMutex();
  if ((DSDLLcount) && (--DSDLLcount==0)) {
    DirectSoundCreatePtr=0;
    FreeLibrary(DSDLLinstance);
    DSDLLinstance=0;
  }
  Sleep(1);
  InMilesMutex();
}


static S32 clear_dsbuf(void* dsbuf, S32 bits)
{
  //
  // fill the buffer with silence and get it looping
  //

  DSBCAPS bcaps;

  AIL_memset(&bcaps,0,sizeof(bcaps));
  bcaps.dwSize = sizeof(bcaps);

  ASDSBUF(dsbuf)->GetCaps(&bcaps);

  void FAR *p1,FAR *p2;
  DWORD l1,l2;

  MMRESULT result;

 again:
  result = ASDSBUF(dsbuf)->Lock(0,
                                bcaps.dwBufferBytes,
                                &p1,
                                &l1,
                                &p2,
                                &l2,
                                0 );

  if (result == DSERR_BUFFERLOST)
  {
    //
    // Restore, and try again later
    //

    if (SUCCEEDED(ASDSBUF(dsbuf)->Restore()))
      goto again;

  }
  else if (result == MMSYSERR_NOERROR)
  {
    //
    // Mix data into locked region
    //

    if ((p1) && (l1))
      AIL_memset(p1,(bits == 16)?0:0x80,l1);

    if ((p2) && (l2))
      AIL_memset(p2,(bits == 16)?0:0x80,l2);

    //
    // Release locked region
    //

    ASDSBUF(dsbuf)->Unlock(p1,
                           l1,
                           p2,
                           l2);

    return(1);
  }
  return(0);
}


static S32 copy_dsbuf(void* destbuf, void * srcbuf)
{
  S32 ret = 0;
  
  //
  // copy a buffer
  //

  DSBCAPS bcaps;

  AIL_memset(&bcaps,0,sizeof(bcaps));
  bcaps.dwSize = sizeof(bcaps);

  ASDSBUF(destbuf)->GetCaps(&bcaps);

  void FAR *sp1,FAR *sp2;
  DWORD sl1,sl2;
  void FAR *dp1,FAR *dp2;
  DWORD dl1,dl2;

  MMRESULT result;

 again:
  result = ASDSBUF(destbuf)->Lock(0,
                                  bcaps.dwBufferBytes,
                                  &dp1,
                                  &dl1,
                                  &dp2,
                                  &dl2,
                                  0 );

  if (result == DSERR_BUFFERLOST)
  {
    //
    // Restore, and try again later
    //

    if (SUCCEEDED(ASDSBUF(destbuf)->Restore()))
      goto again;

  }
  else if (result == MMSYSERR_NOERROR)
  {
   again2:
    result = ASDSBUF(srcbuf)->Lock(0,
                                   bcaps.dwBufferBytes,
                                   &sp1,
                                   &sl1,
                                   &sp2,
                                   &sl2,
                                   0 );

    if (result == DSERR_BUFFERLOST)
    {
      //
      // Restore, and try again later
      //

      if (SUCCEEDED(ASDSBUF(srcbuf)->Restore()))
        goto again2;
    }
    else if (result == MMSYSERR_NOERROR)
    {
      AIL_memcpy( dp1, sp1, dl1 );
      
      ASDSBUF(srcbuf)->Unlock(sp1,
                              sl1,
                              sp2,
                              sl2);
      ret = 1;
    }

    ASDSBUF(destbuf)->Unlock(dp1,
                             dl1,
                             dp2,
                             dl2);
    
  }
  return( ret );
}

//############################################################################
//##                                                                        ##
//## Setup DirectSound based on the settings in the DIGDRIVER structure     ##
//##                                                                        ##
//############################################################################


S32 setup_directsound(HDIGDRIVER dig)
{
   DSBUFFERDESC pdsbdesc;
   S32          setformat;
   DSCAPS       caps;
   U32          using_default;
   WAVEFORMATEX form;
   U32          requested_priority;

   //
   // Get window handle for application owning this thread
   //

   if ((dig->dsHwnd==0) || (!IsWindow((HWND)dig->dsHwnd)))
      {
      dig->dsHwnd=(UINTa) AIL_HWND();
      }

   //
   // First, set cooperative level to DSSCL_NORMAL, to determine default
   // format of primary buffer
   //
   // (If, however, we're mixing into the primary buffer, we've got to
   // go ahead and request DSSCL_WRITEPRIMARY access now)
   //
   // If this is a multichannel configuration, we don't care anything about the default
   // format of the primary buffer.  It's probably meaningless (WDM).  Go ahead 
   // and set the format without checking
   //

   if (AIL_preference[DIG_DS_USE_PRIMARY])
      {
      setformat = !dig->DS_use_default_format;
      requested_priority = DSSCL_WRITEPRIMARY;
      }
   else
      {
      setformat = 0;

      if (dig->wformat.wf.wFormatTag == WAVE_FORMAT_EXTENSIBLE)
         {
         requested_priority = DSSCL_PRIORITY;
         }
      else
         {
         requested_priority = DSSCL_NORMAL;
         }
      }

   //
   // Iterate until primary buffer object created with desired format
   //

   for(;;)
      {
      //
      // Release previously-created objects
      //

      if (dig->lppdsb)
         {
         ASDSBUF(dig->lppdsb)->Release();
         dig->lppdsb = NULL;
         }

      if (dig->pDS)
         {
         ASDS(dig->pDS)->Release();
         dig->pDS = NULL;
         }

      //
      // Attempt to instantiate DirectSound object
      //
      // If DirectSound object cannot be instantiated, fail
      //

      MMRESULT res;

      OutMilesMutex();
      DirectSoundCreateType DS_CREATE = (DirectSoundCreateType) AIL_preference[DIG_DS_CREATION_HANDLER];
      InMilesMutex();

      if (DS_CREATE == NULL)
         {
         //
         // No specific DirectSoundCreate() handler provided -- use
         // default
         //

         OutMilesMutex();
         if (!SUCCEEDED((res=DirectSoundCreatePtr((LPGUID) dig->guid,
                                    (LPDIRECTSOUND FAR *) &dig->pDS,
                                                           NULL))))
         {
            InMilesMutex();
            AIL_set_error("DirectSoundCreate() failed.");
            return 0;
          }
          InMilesMutex();
         }
      else
      {
         //
         // Call application-provided DirectSoundCreate() handler (e.g., for
         // Aureal 3D)
         //

         OutMilesMutex();
         if (!SUCCEEDED((res=DS_CREATE((LPGUID) dig->guid,
                         (LPDIRECTSOUND FAR *) &dig->pDS,
                                                NULL))))
         {
            InMilesMutex();
            AIL_set_error("DirectSoundCreate() custom handler failed.");
            return 0;
         }
         InMilesMutex();
      }

      //
      // Figure out if we're running in emulation mode
      //

      dig->emulated_ds = 0;

      AIL_memset(&caps,0,sizeof(caps));
      caps.dwSize = sizeof(caps);

      if (ASDS(dig->pDS)->GetCaps(&caps) == DS_OK)
         {
         dig->emulated_ds = (caps.dwFlags & DSCAPS_EMULDRIVER) ? 1 : 0;
         }

      //
      // If DS is emulated, we can't request WRITEPRIMARY access
      //

      if ((dig->emulated_ds) && (requested_priority == DSSCL_WRITEPRIMARY))
         {
         requested_priority = DSSCL_PRIORITY;
         }

      //
      // Set requested priority level
      //

      OutMilesMutex();
      if (!SUCCEEDED(ASDS(dig->pDS)->SetCooperativeLevel(
           (HWND)dig->dsHwnd,
                 requested_priority)))
         {
         ASDS(dig->pDS)->Release();
         InMilesMutex();
         AIL_set_error("SetCooperativeLevel() failed.");
         return 0;
         }
      InMilesMutex();

      dig->ds_priority=requested_priority;

      //
      // Set up DSBUFFERDESC structure for primary buffer
      //
      // lpwxFormat must be NULL for primary buffers at setup time
      //

      AIL_memset(&pdsbdesc, 0, sizeof(DSBUFFERDESC));

      pdsbdesc.dwSize  = sizeof(DSBUFFERDESC);
      pdsbdesc.dwFlags = DSBCAPS_PRIMARYBUFFER;

      if (AIL_preference[DIG_DS_DSBCAPS_CTRL3D])
         {
         pdsbdesc.dwFlags |= DSBCAPS_CTRL3D;
         }

      pdsbdesc.dwBufferBytes = 0;
      pdsbdesc.lpwfxFormat   = NULL;

      //
      // Try to create primary sound buffer
      //

      OutMilesMutex();
      if (!SUCCEEDED(ASDS(dig->pDS)->CreateSoundBuffer(&pdsbdesc,
                                                      (LPDIRECTSOUNDBUFFER FAR *) &dig->lppdsb,
                                                       NULL)))
         {
         ASDS(dig->pDS)->Release();
         InMilesMutex();
         AIL_set_error("CreateSoundBuffer() failed on primary buffer.");
         return 0;
         }
      InMilesMutex();

      //
      // Set format of buffer, if requested
      // This will have no effect on WDM drivers!
      //

      if (setformat)
         {
         if (dig->wformat.wf.wFormatTag != WAVE_FORMAT_EXTENSIBLE)
            {
            dig->wformat.wf.wFormatTag = WAVE_FORMAT_PCM;
            }

         dig->wformat.wf.nAvgBytesPerSec  = dig->wformat.wf.nSamplesPerSec *
                                            dig->wformat.wf.nBlockAlign;

         OutMilesMutex();
         if (!SUCCEEDED(ASDSBUF(dig->lppdsb)->SetFormat((LPWAVEFORMATEX)&dig->wformat)))
            {
            ASDS(dig->pDS)->Release();
            InMilesMutex();
            AIL_set_error("SetFormat() failed on primary buffer.");
            return 0;
            }
         InMilesMutex();
         }

      //
      // If this is a multichannel setup, we're done -- exit without trying to read/reuse the
      // primary buffer format
      //

      if (dig->wformat.wf.wFormatTag == WAVE_FORMAT_EXTENSIBLE)
         {
         break;
         }

      //
      // Get format of primary sound buffer
      //

      ASDSBUF(dig->lppdsb)->GetFormat( &form, sizeof(form), NULL);

      using_default = dig->DS_use_default_format;

      if ((form.nChannels      == dig->wformat.wf.nChannels)      &&
          (form.nSamplesPerSec == dig->wformat.wf.nSamplesPerSec) &&
          (form.nBlockAlign    == dig->wformat.wf.nBlockAlign)    &&
          (form.wBitsPerSample == dig->wformat.wBitsPerSample)    &&
          (form.wFormatTag     == dig->wformat.wf.wFormatTag))
         {
         using_default = 1;
         }

      //
      // If we can use the current format, or if a specific format was just
      // set, exit from the loop
      //

      if ((using_default) || (setformat))
         {
         dig->wformat.wf.nChannels        = form.nChannels;
         dig->wformat.wf.nSamplesPerSec   = form.nSamplesPerSec;
         dig->wformat.wf.nBlockAlign      = form.nBlockAlign;
         dig->wformat.wBitsPerSample      = form.wBitsPerSample;
         dig->wformat.wf.wFormatTag       = form.wFormatTag;

         dig->wformat.wf.wFormatTag       = WAVE_FORMAT_PCM;
         dig->wformat.wf.nBlockAlign      = ((dig->wformat.wf.nChannels   == 2 ) + 1) *
                                            ((dig->wformat.wBitsPerSample == 16) + 1);

         dig->wformat.wf.nAvgBytesPerSec  = dig->wformat.wf.nSamplesPerSec *
                                            dig->wformat.wf.nBlockAlign;
         break;
         }

      //
      // Otherwise, set flag to try specified format, and continue
      //

      setformat = 1;
      requested_priority = DSSCL_PRIORITY;
      }

     //
     // fill the buffer with silence and get it looping
     //

     if (requested_priority == DSSCL_WRITEPRIMARY)
       clear_dsbuf(dig->lppdsb,dig->wformat.wBitsPerSample);

     //
     // Set primary buffer to "play"
     //
     OutMilesMutex();
     ASDSBUF(dig->lppdsb)->Play( 0, 0, DSBPLAY_LOOPING);
     InMilesMutex();

   //
   // Return success
   //

   return 1;
}

//############################################################################
//#                                                                          #
//#  Translate system-specific speaker configuration word to closest MSS     #
//#  value                                                                   #
//#                                                                          #
//############################################################################

S32 AILCALL get_system_speaker_configuration(MSS_MC_SPEC *channel_spec)
{
   //
   // Create temporary DirectSound object and use it to query the speaker configuration
   //

   if (!DirectSoundLoad())
      {
      AIL_set_error("Could not load DirectSound to query speaker configuration");
      return 0;
      }

   LPDIRECTSOUND pDS;

   if (!SUCCEEDED(DirectSoundCreatePtr(NULL,
                                      &pDS,
                                       NULL)))
      {
      DirectSoundUnload();
      AIL_set_error("DirectSoundCreate() failed in get_system_speaker_configuration()");
      return 0;
      }

   DWORD config = 0;
   HRESULT result = pDS->GetSpeakerConfig(&config);

   pDS->Release();
   DirectSoundUnload();

   if (!SUCCEEDED(result))
      {
      AIL_set_error("GetSpeakerConfig() failed");
      return 0;
      }

   //
   // Default channel spec == normal stereo if the configuration can't be determined
   //

   *channel_spec = MSS_MC_STEREO;

   switch (DSSPEAKER_CONFIG(config))       // Must mask off geometry byte before comparing
      {
      case DSSPEAKER_DIRECTOUT : *channel_spec = MSS_MC_STEREO;              break; // Not clear what should be done with this
      case DSSPEAKER_HEADPHONE : *channel_spec = MSS_MC_HEADPHONES;          break;
      case DSSPEAKER_MONO      : *channel_spec = MSS_MC_MONO;                break;
      case DSSPEAKER_QUAD      : *channel_spec = MSS_MC_40_DISCRETE;         break;
      case DSSPEAKER_STEREO    : *channel_spec = MSS_MC_STEREO;              break;
      case DSSPEAKER_SURROUND:
         {
         // 
         // For generic "surround" configurations, we prefer SRS, then Dolby, then 
         // fall back to stereo if neither of those providers are available
         //
              if (AIL_find_filter("SRS Circle Surround 6.1 Matrix Filter", NULL)) *channel_spec = MSS_MC_SRS_CIRCLE_SURROUND; 
         else if (AIL_find_filter("Dolby Surround Matrix Filter", NULL))          *channel_spec = MSS_MC_DOLBY_SURROUND;
         else                                                                     *channel_spec = MSS_MC_STEREO;
         break;
         } 
      case DSSPEAKER_5POINT1   : *channel_spec = MSS_MC_51_DISCRETE;         break;
      case DSSPEAKER_7POINT1   : *channel_spec = MSS_MC_71_DISCRETE;         break;
      }

   return 1;
}

//############################################################################
//##                                                                        ##
//## Set the DirectSound HWND to use                                        ##
//##                                                                        ##
//############################################################################

extern "C" extern HWND MSShWnd;

S32 AILCALL AIL_API_set_DirectSound_HWND(HDIGDRIVER dig, HWND wnd)
{
   if (dig == NULL)
      {
      return 0;
      }

   dig->dsHwnd = (UINTa) wnd;

   if (dig->pDS)
   {
     OutMilesMutex();
     if (!SUCCEEDED(ASDS(dig->pDS)->SetCooperativeLevel(
                                      (HWND)dig->dsHwnd,
                                      dig->ds_priority)))
     {
       InMilesMutex();
       AIL_set_error("SetCooperativeLevel() failed.");
       return 0;
     }
     InMilesMutex();
   }

   MSShWnd=wnd;

   return 1;
}

//############################################################################
//##                                                                        ##
//##  Get DirectSound driver and secondary buffer associated with sample    ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_\
get_DirectSound_info(HSAMPLE              S,//)
                     AILLPDIRECTSOUND       *lplpDS,
                     AILLPDIRECTSOUNDBUFFER *lplpDSB)
{
   HDIGDRIVER dig;

   if (S == NULL)
      {
      if (DIG_first == NULL)
         {
         return;
         }

      //
      // Return first driver's DirectSound and primary buffer objects
      // if no specific sample handle provided
      //

      if (lplpDS != NULL)
         {
         *lplpDS = (LPDIRECTSOUND) DIG_first->pDS;
         }

      if (lplpDSB != NULL)
         {
         *lplpDSB = ASDSBUF(DIG_first->lppdsb);
         }

      return;
      }

   //
   // Get driver and buffer associated with sample
   //

   dig = S->driver;

   //
   // Return requested values
   //

   if (lplpDS != NULL)
      {
      *lplpDS = (LPDIRECTSOUND) dig->pDS;
      }

   if (lplpDSB != NULL)
      {
      *lplpDSB = ASDSBUF(dig->lppdsb);
      }
}

void AILCALL AIL_LP_lock()
{
  if (AIL_preference[AIL_LOCK_PROTECTION])
  {
    AIL_lock();
  }
}


void AILCALL AIL_LP_unlock()
{
  if (AIL_preference[AIL_LOCK_PROTECTION])
  {
    AIL_unlock();
  }
}


// ------------------------------------------------------------------
// DS_shutdown
// ------------------------------------------------------------------

void DS_shutdown(HDIGDRIVER dig)
{
   S32 rel;
   AIL_LP_lock();    // Lock to avoid DirectSound deadlocks with SS_serve

   MSSLockedIncrement(disable_callbacks);

   rel=dig->released;

   dig->playing=0;
   dig->released=1;
   dig->hWaveOut=0;

   if (!rel)
      {
      //
      // Release secondary buffer, if created
      //

      if (dig->DS_sec_buff != NULL)
         {
         ASDSBUF(dig->DS_sec_buff)->Release();
         dig->DS_sec_buff = NULL;
         }

      //
      // Release the primary buffer
      //

      if (dig->lppdsb != NULL)
         {
         ASDSBUF(dig->lppdsb)->Release();
         dig->lppdsb = NULL;
         }

      //
      // Shut down DirectSound driver
      //

      if (dig->pDS != NULL)
         {
         ASDS(dig->pDS)->Release();
         dig->pDS = NULL;
         }
      }

   //
   // Unload DS DLL
   //

   DirectSoundUnload();

   MSSLockedDecrement(disable_callbacks);

   AIL_LP_unlock();
}

// ------------------------------------------------------------------
// DIG_waveOutPrepareHeader
// ------------------------------------------------------------------

MMRESULT DIG_waveOutPrepareHeader(HDIGDRIVER dig, //)
                                  HWAVEOUT   hwo,
                                  LPWAVEHDR  pwh,
                                  UINT       cbwh)
{
   if (AIL_preference[DIG_USE_WAVEOUT])
      {
      return waveOutPrepareHeader(hwo, pwh, cbwh);
      }

   return MMSYSERR_NOERROR;
}

// ------------------------------------------------------------------
// DIG_waveOutUnprepareHeader
// ------------------------------------------------------------------

MMRESULT DIG_waveOutUnprepareHeader(HDIGDRIVER dig, //)
                                    HWAVEOUT  hwo,
                                    LPWAVEHDR pwh,
                                    UINT      cbwh)
{
   if (AIL_preference[DIG_USE_WAVEOUT])
      {
      return waveOutUnprepareHeader(hwo, pwh, cbwh);
      }

   return MMSYSERR_NOERROR;
}

// ------------------------------------------------------------------
// DIG_waveOutGetDevCaps
// ------------------------------------------------------------------

MMRESULT DIG_waveOutGetDevCaps(HDIGDRIVER dig, //)
                               UINT          uDeviceID,
                               LPWAVEOUTCAPS lpCaps,
                               UINT          wSize)
{
   if (AIL_preference[DIG_USE_WAVEOUT])
      {
      return waveOutGetDevCaps(uDeviceID, lpCaps, wSize);
      }

   AIL_strcpy(lpCaps->szPname, dig->emulated_ds?"Emulated DirectSound - MSS Mixer":"DirectSound - MSS Mixer");

   return MMSYSERR_NOERROR;
}

// ------------------------------------------------------------------
// DIG_waveOutGetID
// ------------------------------------------------------------------

MMRESULT DIG_waveOutGetID(HDIGDRIVER dig, //)
                          HWAVEOUT hWaveOut,
                          PUINT    lpuDeviceID)
{
   if (AIL_preference[DIG_USE_WAVEOUT])
      {
      return waveOutGetID(hWaveOut, lpuDeviceID);
      }

   *lpuDeviceID = 0;

   return MMSYSERR_NOERROR;
}

static int GoodGetCurrentPosition( LPDIRECTSOUNDBUFFER b, DWORD * play, DWORD * write )
{

  for(;;)
  {
    DWORD p1, w1, p2, w2;

    // get first position
    if (!(SUCCEEDED ( b->GetCurrentPosition( &p1, &w1 ) ) ) )
    {
      return( 0 );
    }

    // acts a delay
    b->GetCurrentPosition( &p2, &w2 );

    // get next position
    b->GetCurrentPosition( &p2, &w2 );

    // make sure it hasn't wrapped oddly (meaning that a backwards read position may have occurred)
    if ( ( ( p2 - p1 ) < 1024 ) && ( ( w2 - w1 ) < 1024 ) )
    {
      *play = p2;
      *write = w2;
      return( 1 );
    }
  }
}


//
// creates a Directsound secondary buffer
//

static LPDIRECTSOUNDBUFFER create_ds_buf( HDIGDRIVER dig )
{
   LPDIRECTSOUNDBUFFER buf;
   
   //
   // Allocate secondary buffer configured for specified format
   //

   DSBUFFERDESC dsbdesc;

   AIL_memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));

   dsbdesc.dwSize           =  sizeof(DSBUFFERDESC);

   dsbdesc.dwFlags          =  DSBCAPS_GLOBALFOCUS   |
                               DSBCAPS_GETCURRENTPOSITION2 ;

//                               DSBCAPS_CTRLFREQUENCY |
//                               DSBCAPS_CTRLPAN       |
//                               DSBCAPS_CTRLVOLUME;

   dsbdesc.dwBufferBytes    =  dig->DS_buffer_size;
   dsbdesc.lpwfxFormat      = (LPWAVEFORMATEX) &dig->wformat;

   //
   // Try to create secondary sound buffer
   //

   OutMilesMutex();
   if (!(SUCCEEDED (ASDS(dig->pDS)->CreateSoundBuffer(&dsbdesc,
                                                      &buf,
                                                      NULL))))
   {
     buf = 0;

   }
   InMilesMutex();

   return( buf );
}


// ------------------------------------------------------------------
// DIG_waveOutOpen
// ------------------------------------------------------------------

MMRESULT DIG_waveOutOpen(HDIGDRIVER      dig, //)
                         LPHWAVEOUT      lphWaveOut,
                         LPWAVEFORMATEX  lpFormat,
                         DWORD_PTR       dwCallback,
                         DWORD           dwCallbackInstance,
                         DWORD           fdwOpen)
{
   if (AIL_preference[DIG_USE_WAVEOUT])
      {
      OutMilesMutex();
      MMRESULT res = waveOutOpen(lphWaveOut, 
                         (DWORD) AIL_preference[DIG_PREFERRED_WO_DEVICE],
                                 lpFormat, 
                                 dwCallback, 
                                 dwCallbackInstance, 
                                 fdwOpen);
      InMilesMutex();      
      return(res);
      }

   //
   // Fail if already open
   //

   if (dig->DS_initialized)
      {
      AIL_set_error("DirectSound already initialized.");
      return MMSYSERR_ERROR;
      }

   AIL_LP_lock();    // Lock to avoid DirectSound deadlocks with SS_serve

   //
   // Init DS vars
   //

   dig->DS_sec_buff = NULL;
   dig->DS_out_buff = NULL;
   dig->lppdsb = NULL;
   dig->pDS    = NULL;

   //
   // Load the DirectSound dll
   //

   if (!DirectSoundLoad())
      {
      AIL_set_error("The DirectSound DLL could not be loaded.");
      AIL_LP_unlock();

      return MMSYSERR_ERROR;
      }

   //
   // If WAVE_MAPPER is passed in, convert to zero GUID
   // (Behavior retained for robustness even though it's a separate preference now...)
   //

   UINTa dwDeviceID = AIL_preference[DIG_PREFERRED_DS_DEVICE];

   if ((dwDeviceID==WAVE_MAPPER) || (dwDeviceID==(WAVE_MAPPER&0xffff)))
     dwDeviceID=0;

   dig->guid = dwDeviceID;

   //
   // Set up DirectSound, setting primary buffer to specified format
   //

   if (!setup_directsound(dig))
      {
      DS_shutdown(dig);
      AIL_LP_unlock();
      return MMSYSERR_ERROR;
      }
      
   if (((LPPCMWAVEFORMAT) lpFormat)->wf.nChannels == 0)
      {
      //
      // This driver is being opened only to serve an external output filter -- we 
      // don't want access to the primary or secondary buffer
      //

      dig->DS_frag_cnt     = 0;
      dig->DS_frag_size    = 0;
      dig->DS_buffer_size  = 0;
      dig->request_reset   = 0;
      dig->playing         = 0;

      dig->DS_initialized  = 1;

      AIL_LP_unlock();
      return MMSYSERR_NOERROR;
      }

   if ((!AIL_preference[DIG_DS_USE_PRIMARY]) || (dig->emulated_ds))
      {
      //
      // Calculate size of secondary buffer based on preferences
      //

      dig->DS_frag_cnt = (S32) AIL_preference[DIG_DS_FRAGMENT_CNT];

      // make sure we have at least triple the mix ahead
      if (dig->DS_frag_cnt<((S32) AIL_preference[DIG_DS_MIX_FRAGMENT_CNT]*3))
        dig->DS_frag_cnt = (S32) AIL_preference[DIG_DS_MIX_FRAGMENT_CNT]*3;

      dig->DS_frag_size = ((dig->wformat.wf.nAvgBytesPerSec *
                           (S32) AIL_preference[DIG_DS_FRAGMENT_SIZE]) / 1000L);

      //hack to give emulated mode a little more breathing room
      if (dig->emulated_ds)
        dig->DS_frag_size*=2;

      // Round fragment size up to multiple of sample block size

      S32 n = dig->wformat.wf.nBlockAlign;
      dig->DS_frag_size = ((dig->DS_frag_size + (n-1)) / n) * n;

      dig->DS_buffer_size = dig->DS_frag_size * dig->DS_frag_cnt;

      dig->DS_sec_buff = create_ds_buf( dig );
      if ( dig->DS_sec_buff == 0 )
      {
         DS_shutdown(dig);
         AIL_set_error("Could not create secondary buffer");
         AIL_LP_unlock();
         return MMSYSERR_ERROR;
      }
      
      //
      // Start it
      //

      dig->DS_out_buff = dig->DS_sec_buff;

      SAMPLE temp;
      temp.driver = dig;

      SS_start_DIG_driver_playback(&temp);
      }
   else
      {
      //
      // Set up fragment info for writing to primary buffer
      //

      dig->DS_out_buff = dig->lppdsb;

      DSBCAPS caps;

      AIL_memset(&caps,0,sizeof(caps));
      caps.dwSize = sizeof(caps);

      ASDSBUF(dig->DS_out_buff)->GetCaps(&caps);

      dig->DS_buffer_size = caps.dwBufferBytes;

      //
      // First, approximate the fragment size by the requested fragment
      // playback period
      //

      dig->DS_frag_size = (dig->wformat.wf.nAvgBytesPerSec *
                           (S32) AIL_preference[DIG_DS_FRAGMENT_SIZE]) / 1000L;

      dig->DS_frag_cnt = dig->DS_buffer_size / dig->DS_frag_size;

      //
      // Accept the next larger fragment size which evenly divides the
      // primary buffer, and which consists of an integral number of
      // samples
      //
      // We can't use fewer than 2 fragments
      //

      while (dig->DS_frag_cnt >= 2)
         {
         dig->DS_frag_size = dig->DS_buffer_size / dig->DS_frag_cnt;

         if (dig->DS_frag_size & (dig->wformat.wf.nBlockAlign - 1))
            {
            --dig->DS_frag_cnt;
            continue;
            }

         if ((dig->DS_frag_size * dig->DS_frag_cnt) != dig->DS_buffer_size)
            {
            --dig->DS_frag_cnt;
            continue;
            }

         break;
         }

      if (dig->DS_frag_cnt < 2)
         {
         dig->DS_frag_cnt = 2;
         dig->DS_frag_size = dig->DS_buffer_size / 2;
         }
      }

   //
   // Get initial play position
   //

   DWORD p,w;

   if (!GoodGetCurrentPosition(ASDSBUF(dig->DS_out_buff),&p,&w))
      {
      DS_shutdown(dig);

      AIL_set_error("Could not get buffer position");
      AIL_LP_unlock();
      return MMSYSERR_ERROR;
      }

   dig->DS_last_frag  = (w / dig->DS_frag_size)+1;
   dig->DS_last_write = dig->DS_last_frag;

   dig->DS_skip_time = (S32) AIL_preference[DIG_DS_FRAGMENT_CNT] *
                       (S32) AIL_preference[DIG_DS_FRAGMENT_SIZE];

   //
   // Return OK
   //

   dig->DS_initialized = 1;

   AIL_LP_unlock();
   return MMSYSERR_NOERROR;
}

// ------------------------------------------------------------------
// DIG_waveOutClose
// ------------------------------------------------------------------

MMRESULT DIG_waveOutClose(HDIGDRIVER dig, HWAVEOUT hwo)
{
   if (AIL_preference[DIG_USE_WAVEOUT])
      {
      MMRESULT result=waveOutClose(hwo);
      return result;
      }

   if (!dig->DS_initialized)
      {
      return MMSYSERR_ERROR;
      }

   DS_shutdown(dig);

   dig->DS_initialized = 0;

   return MMSYSERR_NOERROR;
}

// ------------------------------------------------------------------
// DIG_waveOutReset
// ------------------------------------------------------------------

MMRESULT DIG_waveOutReset(HDIGDRIVER dig, HWAVEOUT hWaveOut)
{
   MMRESULT result;

   if (dig->physical_channels_per_sample == 0)
      {
      return MMSYSERR_NOERROR;
      }

   if (AIL_preference[DIG_USE_WAVEOUT])
      {
      result=waveOutReset(hWaveOut);
      dig->wom_done_buffers=0;
      return result;
      }

   AIL_LP_lock();    // Lock to avoid DirectSound deadlocks with SS_serve

   do
      {
      result = ASDSBUF(dig->DS_out_buff)->Stop();

      if (result == DSERR_BUFFERLOST)
         {
         ASDSBUF(dig->DS_out_buff)->Restore();
         OutMilesMutex();
         Sleep(10);
         InMilesMutex();
         }
      }
   while (result == DSERR_BUFFERLOST);

   AIL_LP_unlock();

   return MMSYSERR_NOERROR;
}

// ------------------------------------------------------------------
// DIG_waveOutWrite
// ------------------------------------------------------------------

MMRESULT DIG_waveOutWrite(HDIGDRIVER dig,  //)
                          HWAVEOUT   hwo,
                          LPWAVEHDR  pwh,
                          UINT       cbwh)
{
   if (AIL_preference[DIG_USE_WAVEOUT])
      {
      MMRESULT result=waveOutWrite(hwo, pwh, cbwh);
      return(result);
      }

   return MMSYSERR_NOERROR;
}

#else

#define DIG_waveOutClose(a,b) waveOutClose(b)
#define DIG_waveOutGetDevCaps(a,b,c,d) waveOutGetDevCaps(b,c,d)
#define DIG_waveOutGetID(a,b,c) waveOutGetID(b,c)
#define DIG_waveOutOpen(a,b,c,d,e,f,g) waveOutOpen(b,c,d,e,f,g)
#define DIG_waveOutPrepareHeader(a,b,c,d) waveOutPrepareHeader(b,c,d)
#define DIG_waveOutReset(a,b) waveOutReset(b)
#define DIG_waveOutUnprepareHeader(a,b,c,d) waveOutUnprepareHeader(b,c,d)
#define DIG_waveOutWrite(a,b,c,d) waveOutWrite(b,c,d)

#endif

//############################################################################
//##                                                                        ##
//## Start driver-based DMA buffer playback                                 ##
//##                                                                        ##
//############################################################################

void AILCALL SS_start_DIG_driver_playback(HSAMPLE S)
{
   LPWAVEHDR current;
   S32       i;
   HDIGDRIVER dig;

   dig = S->driver;

   //
   // Always clear any reset request when starting playback
   //

   dig->request_reset = 0;

   //
   // Return if playback already active or output filter in use
   //

   if ((dig->playing) || (dig->physical_channels_per_sample == 0))
      {
      return;
      }

   //
   // Set playing flag
   //

   dig->playing = 1;

   if (AIL_preference[DIG_USE_WAVEOUT])
      {
      //
      // Set up to force all buffers to be posted
      //

      current = dig->first;

      for (i=0; i < dig->n_buffers; i++)
         {
         CURRENT_USER->flags |= HDR_FORCE_POST;

         current = CURRENT_USER->next;
         }

      //
      // Manually force foreground timer service to begin playing sound
      // immediately
      //

      if (AIL_bkgnd_flag == 0)
         {
         low_serve();
         }
      }
   else
      {
#ifdef IS_WIN32
      AIL_LP_lock();    // Lock to avoid DirectSound deadlocks with SS_serve

      MMRESULT result;

      clear_dsbuf(dig->DS_out_buff,dig->wformat.wBitsPerSample);

      do
         {
         result = ASDSBUF(dig->DS_out_buff)->Play(0,
                                                  0,
                                                  DSBPLAY_LOOPING);

         if (result == DSERR_BUFFERLOST)
            {
            ASDSBUF(dig->DS_out_buff)->Restore();
            OutMilesMutex();
            Sleep(10);
            InMilesMutex();
            }
         }
      while (result == DSERR_BUFFERLOST);

      if (result)
         {
         AIL_set_error("Could not start output buffer");
         }

      AIL_LP_unlock();
#endif
      }
}

//############################################################################
//##                                                                        ##
//## Stop driver-based DMA buffer playback                                  ##
//##                                                                        ##
//## Warning: if !dig->reset_works, do NOT call this function except at     ##
//## shutdown time!                                                         ##
//##                                                                        ##
//############################################################################

static void SS_stop_DIG_driver_playback(HDIGDRIVER dig)
{
   if (!dig->playing)
      {
      return;
      }

   //
   // Stop playback ASAP and return all buffers
   //
   // If we're not in background mode, we can reset immediately --
   // otherwise, post a request for the next foreground timer call
   //

   if (!AIL_background())
      {
      dig->playing = 0;
      DIG_waveOutReset(dig,dig->hWaveOut);
      }
   else
      {
      dig->request_reset = 1;
      }
}

//############################################################################
//##                                                                        ##
//## Fill output buffer with mixed data and/or silence                      ##
//##                                                                        ##
//############################################################################

static void SS_fill(HDIGDRIVER dig, void FAR *lpData)
{
   S32        cnt,n;
   HSAMPLE    S;
   U32       *stat;

   U32 start_us = AIL_us_count();

   //
   // Flush build buffer with silence
   //

   SS_flush(dig);

   //
   // Merge active samples (if any) into build buffer
   //

   cnt = 0;

   for (n = dig->n_samples,S = &dig->samples[0],stat = &dig->sample_status[0]; n; --n,++S,++stat)
      {
      //
      // Skip sample if stopped, finished, or not allocated
      //

      if (*stat != SMP_PLAYING)
         {
         continue;
         }

      ++cnt;

      //
      // Convert sample to 16-bit signed format and mix with
      // contents of build buffer
      //
      // Note that SS_stream_and_mix() may invoke user callback functions
      // which may free or otherwise alter the sample being merged
      //
      // If ASI codec is in use, buffer maintenance can take place within
      // either SS_stream_and_mix() or the ASI fetch callback
      //

      SS_stream_and_mix(S);
      }

   //
   // Set number of active samples
   //

   dig->n_active_samples = cnt;

   //
   // Copy build buffer contents to DMA buffer
   //

   SS_copy(dig, lpData);

   //
   // If no samples active for two consecutive interrupts (three in Win32),
   // request DMA halt
   //

   if (dig->n_active_samples)
      {
      dig->quiet = 0;
      }
   else
      {
#ifdef IS_WIN32
      if (dig->quiet++ == 3)
#else
      if (dig->quiet++ == 2)
#endif
         {
         if (dig->reset_works)
            {
            //
            // Don't stop the DirectSound secondary buffer -- we only need to
            // stop the waveOut buffer to reduce subsequent sample latency,
            // which isn't a problem with DirectSound
            //

            if (AIL_preference[DIG_USE_WAVEOUT])
               {
               SS_stop_DIG_driver_playback(dig);
               }
            }
         }
      }

   //
   // keep the profiling information
   //

   U32 end_us=AIL_us_count();

   start_us=end_us-start_us;

   dig->us_count+=start_us;
   if (dig->us_count>10000000) {
     dig->ms_count+=(dig->us_count/1000);
     dig->us_count=dig->us_count%1000;
   }
}


//############################################################################
//##                                                                        ##
//## Foreground callback function to keep buffer chain in queue             ##
//##                                                                        ##
//## Causes all DONE WAVEHDRS in chain to be sent to the driver, in order,  ##
//## via waveOutWrite()                                                     ##
//##                                                                        ##
//############################################################################

static void SS_foreground_service()
{
   HDIGDRIVER dig;
   LPWAVEHDR  current,start;

   //
   // Return immediately if callbacks disabled
   //

   if (disable_callbacks)
      {
      return;
      }

   //
   // Skip foreground processing if DirectSound in use
   //

   if (!AIL_preference[DIG_USE_WAVEOUT])
   {
     return;
   }

   MSSLockedIncrement(SS_servicing);

   if (SS_servicing==1)
   {

     //
     // Disallow timer service
     //

     AIL_lock();

     //
     // Iterate digital driver list
     //

     dig = DIG_first;

     while (dig != NULL)
        {
        //
        // Perform waveOutReset(), if requested
        //

        if (dig->request_reset)
           {
           dig->request_reset = 0;
           dig->playing       = 0;

           DIG_waveOutReset(dig,dig->hWaveOut);
           }

        //
        // Skip drivers which are not playing
        //

        if (!dig->playing)
           {
           dig = dig->next;
           continue;
           }

        //
        // Begin search at first queue entry
        //

        current = dig->first;

        //
        // Because buffer list is circular and contains two subqueues of
        // entries, the subqueue of DONE buffers begins immediately
        // "after" the subqueue of pending buffers, if any.
        //
        // First, locate the subgroup of pending (posted but unplayed) buffers.
        // If no pending buffers exist, then the entire queue is full of
        // DONE buffers that need to be reposted.
        //

        start = current;

        while (NEED_POST(current))
           {
           current = CURRENT_USER->next;

           if (current == start)
              {
              break;
              }
           }

        //
        // If "starvation" occurred, or if this is an initial call to start
        // playback, re-prime queue from beginning
        //

        if (NEED_POST(current))
           {
           //
           // Prime queue buffers
           //

           start = dig->first;
           SS_fill(dig, start->lpData);

           start = ((HDR_USER FAR *) start->dwUser)->next;
           SS_fill(dig, start->lpData);

           start = ((HDR_USER FAR *) start->dwUser)->next;
           SS_fill(dig, start->lpData);

  #ifdef IS_WIN32

           start = ((HDR_USER FAR *) start->dwUser)->next;
           SS_fill(dig, start->lpData);

  #endif
           current = dig->first;
           }

        //
        // If we've hit the subqueue of pending buffers, find the first
        // DONE buffer after it and start posting.
        //
        // If no pending buffers found, fall through and start posting.
        //

        start = current;

        while (!NEED_POST(current))
           {
           current = CURRENT_USER->next;

           if (current == start)
              {
              break;
              }
           }

        //
        // Post buffers until starting point reached or already-pending buffer
        // encountered
        //

        start = current;

        while NEED_POST(current)
           {
           //
           // Clear "virgin" bit
           //

           CURRENT_USER->flags &= ~HDR_FORCE_POST;

           //
           // Do waveOutWrite() call to post this header
           //

           DIG_waveOutWrite(dig,
                            dig->hWaveOut,
                            current,
                            sizeof(WAVEHDR));

           //
           // Iterate through rest of "done" chain
           //

           current = CURRENT_USER->next;

           if (current == start)
              {
              break;
              }
           }

        //
        // Move on to next HDIGDRIVER
        //

        dig = dig->next;
        }

     AIL_unlock();
  }

  MSSLockedDecrement(SS_servicing);
}

//############################################################################
//##                                                                        ##
//## SetTimer timer to periodically call the foreground servicing routine   ##
//##                                                                        ##
//############################################################################

extern "C" void stream_background(void); // background service for streaming

void AILEXPORT Timer_foreground_service(HWND  hwnd,       //)
                                    UINT  message,
                                    UINT  nIDEvent,
                                    DWORD dwTime)
{
   Only16Push32s();

   InMilesMutex();
   low_serve();
   OutMilesMutex();

   Only16Pop32s();
}

#ifdef IS_WIN32

extern "C"
{
static U32 checkforeground=0;
}

#else

extern "C"
{
HTASK Win16_thread_task=0;

S32 Win16_pump_mess=0;

//############################################################################
//##                                                                        ##
//## SetTimer timer to periodically call the foreground servicing routine   ##
//##                                                                        ##
//############################################################################

void AILEXPORT Timer_othertask_service(HWND  hwnd,       //)
                                       UINT  message,
                                       UINT  nIDEvent,
                                       DWORD dwTime)
{
   Only16Push32s();

   SS_foreground_service();

   Only16Pop32s();
}

//############################################################################
//##                                                                        ##
//## Called from the other task to do background processing                 ##
//##                                                                        ##
//############################################################################

WORD AILEXPORT SS_Win16_thread()
{
  MSG msg;

  DWORD timer = (DWORD) SetTimer((HWND) NULL,0,75, Timer_othertask_service);

  Win16_thread_task=GetCurrentTask();

  while (GetMessage(&msg,0,0,0)) {
    switch (msg.message) {
      case WM_USER+230:
        PostQuitMessage(0);
        continue;
      case WM_USER+300:
        --Win16_pump_mess;
        break;
    }
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  KillTimer((HWND) NULL, (UINT) timer);

  return msg.message;
}

}
#endif

//############################################################################
//##                                                                        ##
//## Timer callback function to mix data into output buffers                ##
//##                                                                        ##
//## Called from callback handler under Win16 to improve latency;           ##
//## called from timeSetEvent() callback thread under Win32 to avoid        ##
//## deadlocks                                                              ##
//##                                                                        ##
//## If DirectSound is being used to emulate waveOut functionality, this    ##
//## routine fills the next available DirectSound buffer fragment.          ##
//## Otherwise, it writes the data to a waveOut buffer which is several     ##
//## buffers ahead of the one most recently returned.                       ##
//##                                                                        ##
//############################################################################

void AILLIBCALLBACK SS_serve(HDIGDRIVER dig)
{
   LPWAVEHDR  current;
   S32        i;

   //
   // Return immediately if callbacks disabled or driver not actively playing
   //
   // dig->playing will never be TRUE when an output filter is in use
   //

   if (disable_callbacks)
      {
      return;
      }

   if (!dig->playing)
      {
      return;
      }

   //
   // Increment background count so callback functions will run in
   // background
   //

   MSSLockedIncrement(SS_servicing);

   if (SS_servicing==1)
   {

      MSSLockedIncrement(AIL_bkgnd_flag);

      if (AIL_preference[DIG_USE_WAVEOUT])
         {
         //
         // Loop through all returned buffers (if any)
         //
         // WARNING: SS_WOM_DONE() may add entry to circular list asynchronously!
         //

         while (dig->return_tail != dig->return_head)
            {
            //
            // Fetch WAVEHDR at tail pointer
            //

            i = dig->return_tail;

            current = dig->return_list[i];

            i = (i + 1) % dig->n_buffers;

            dig->return_tail = i;

            //
            // Fill subsequent output buffer with mixed audio
            //

            current = CURRENT_USER->next;
            current = CURRENT_USER->next;
            current = CURRENT_USER->next;

      #ifdef IS_WIN32
            current = CURRENT_USER->next;
      #endif

            SS_fill(dig, current->lpData);
            }
         }
      else
         {
   #ifdef IS_WIN32
         //
         // Get current buffer fragment
         //

         DWORD p,w;

         AIL_LP_lock();
         dig->position_error = GoodGetCurrentPosition(ASDSBUF(dig->DS_out_buff),&p,&w) ? 0 : 1;
         AIL_LP_unlock();
         
         S32 timer=AIL_ms_count();

         if ( ( p != dig->last_ds_play ) || ( w != dig->last_ds_write ) )
         {
           dig->last_ds_play = p;
           dig->last_ds_write = w;
           dig->last_ds_move = timeGetTime();
         }
         else
         {         
           U32 tgt = timeGetTime();
           
           // if neither of the positions have moved in 128 ms - figure we
           //   hit the Creative bug and reset playback
           if ( ( tgt - dig->last_ds_move ) > 128 )
           {
             AIL_LP_lock();
             ASDSBUF(dig->DS_out_buff)->Stop();
  
             LPDIRECTSOUNDBUFFER new_buf;
             new_buf = create_ds_buf( dig );
             if ( new_buf )
             {
               // lock both and copy
               copy_dsbuf(new_buf, dig->DS_sec_buff);

               ASDSBUF(dig->DS_sec_buff)->Release();
               dig->DS_sec_buff = new_buf;
               dig->DS_out_buff = new_buf;
               ASDSBUF(dig->DS_out_buff)->SetCurrentPosition(p);
             }
             
             ASDSBUF(dig->DS_out_buff)->Play(0,0,DSBPLAY_LOOPING);
             AIL_LP_unlock();
             
             {
               char buf[256]; wsprintf(buf,"Warning: DirectSound playback reset due to non-moving playback cursor (buggy sound driver) time: %i play: %i write: %i.\n",timer,p,w);
               OutputDebugString( buf );
             }
             
             dig->last_ds_move = tgt;
           }
         }
             

         S32 cur_frag = (w / dig->DS_frag_size)+1;
         S32 frags_filled;


         // check for a wrap - we let DS_last_frag grow beyond the end of the buffer and only clip once
         //   once the cur_frag value wraps around
         if (cur_frag<dig->DS_last_write)
         {
           dig->DS_last_frag%=dig->DS_frag_cnt;
         }
         dig->DS_last_write=cur_frag;


         // has it been so long that we dropped out (or did our cur_frag move beyond last fragment)
         if (((timer-dig->DS_last_timer)>dig->DS_skip_time) || (dig->DS_last_frag<cur_frag))
         {
           // bad news - we dropped out
           frags_filled=0;
           dig->DS_last_frag=cur_frag;
         }
         else
         {
           // compute the number of currently filled frags
           frags_filled=dig->DS_last_frag-cur_frag;
         }
         dig->DS_last_timer=timer;

         // get the number of fragments to mix ahead
         S32 DS_frag_mix_ahead = (S32) AIL_preference[DIG_DS_MIX_FRAGMENT_CNT];

         // if we're emulated, automatically mix ahead further
         if ( dig->emulated_ds )
           DS_frag_mix_ahead *= 2;

         // the maximum we can write is right up to before the play cursor
         S32 frags_available;
         
         // get the fragment that we're about to fill at
         S32 fill_frag = (dig->DS_last_frag + dig->DS_frag_cnt) % dig->DS_frag_cnt;
         // get the play cursor fragment
         S32 cur_play_frag = ( p / dig->DS_frag_size );

         // now calculate how many fragments from the fill_frag to the play cursor
         if ( cur_play_frag >= fill_frag )
         {
           frags_available = cur_play_frag - fill_frag;
         }
         else
         {
           frags_available = dig->DS_frag_cnt - fill_frag + cur_play_frag;
         }

         if ( ( DS_frag_mix_ahead - frags_filled ) > frags_available)
           DS_frag_mix_ahead = frags_filled + frags_available;
          
         //
         // Keep at least minimum number of fragments ahead
         //

         while (frags_filled<DS_frag_mix_ahead)
            {
            fill_frag = (dig->DS_last_frag + dig->DS_frag_cnt) % dig->DS_frag_cnt;
            //
            // Calculate offset and size of fragment to lock
            //

            S32 offset = fill_frag * dig->DS_frag_size;
            S32 size   = dig->DS_frag_size;

            //
            // Lock the buffer
            //

            void FAR *p1,FAR *p2;
            DWORD l1,l2;

            MMRESULT result;

            AIL_LP_lock();
            result = ASDSBUF(dig->DS_out_buff)->Lock(offset,
                                                     size,
                                                    &p1,
                                                    &l1,
                                                    &p2,
                                                    &l2,
                                                     0);
            AIL_LP_unlock();

            if (result == DSERR_BUFFERLOST)
               {
               //
               // Restore, and try again later
               //

               AIL_LP_lock();
               ASDSBUF(dig->DS_out_buff)->Restore();
               AIL_LP_unlock();
               }
            else if (result == MMSYSERR_NOERROR)
               {
               //
               // Mix data into locked region
               //

               SS_fill(dig, p1);

               //
               // Release locked region
               //

               AIL_LP_lock();
               ASDSBUF(dig->DS_out_buff)->Unlock(p1,
                                                 l1,
                                                 p2,
                                                 l2);
               AIL_LP_unlock();
               }

            // we don't wrap this variable until cur_frag wraps
            dig->DS_last_frag = dig->DS_last_frag + 1;
            ++frags_filled;
            }
   #endif

         }

      //
      // Clear entry flags and return
      //

      MSSLockedDecrement(AIL_bkgnd_flag);

   }
   MSSLockedDecrement(SS_servicing);

#ifdef IS_WIN32

   if ((checkforeground++&7)==0)
   {
     SS_foreground_service();
   }

#endif
}


//############################################################################
//##                                                                        ##
//## Background callback function for output buffer return                  ##
//##                                                                        ##
//## Not called if DirectSound in use                                       ##
//##                                                                        ##
//############################################################################

void

#ifndef IS_WIN32
__export
#endif

FAR PASCAL SS_WOM_DONE(HWAVEOUT  hWaveOut, //)
                       UINT      wMsg,
                       DWORD     dwInstance,
                       DWORD_PTR dwParam1,
                       DWORD     dwParam2)
{
   LPWAVEHDR  current;
   HDIGDRIVER dig;
   U32        i;

   //
   // Return if message not WOM_DONE
   //

   if (wMsg != MM_WOM_DONE)
      {
      return;
      }

   //
   // Return if no HDR_USER information available
   //

   current = (LPWAVEHDR) dwParam1;

   if (current->dwUser == 0)
   {
      return;
   }

   dig = CURRENT_USER->owner;

   if ( dig->no_wom_done )
   {
     return;
   }

   Only16Push32s();

   //
   // We want the WOM_DONE callback function to behave like a DOS interrupt
   // handler; i.e., the application thread, once interrupted, should not
   // execute again until the callback function has returned.  So we'll
   // suspend the application thread (the one that called AIL_startup())
   // until we're done.
   //

   AIL_lock();

   MSSLockedIncrement(SS_servicing);

   if (SS_servicing!=1)
   {
     dig->wom_done_buffers++;
   }
   else
   {
     InMilesMutex();

  #ifdef IS_WIN32
     if (AIL_preference[AIL_LOCK_PROTECTION])
       if (SuspendThread(hAppThread) == 0xffffffff)
          {
          goto justexit;
          }
  #endif

     //
     // Return if not playing
     //

     if (!dig->playing)
        {
        goto resumethreadandexit;
        }

     dig->wom_done_buffers++;

     while ( dig->wom_done_buffers )
     {
       dig->wom_done_buffers--;

       //
       // Add buffer to circular list at head pointer
       //
       // Temporary variable "i" is used below because list is scanned
       // asynchronously by SS_serve.  A fault could occur if the compiler
       // stores the incremented head pointer before taking its modulus.
       //

       i = dig->return_head;

       dig->return_list[i] = current;

       i = (i + 1) % dig->n_buffers;

       //
       // New head entry becomes readable by SS_serve after atomic write below
       //

       dig->return_head = i;
     }

     //
     // Win16 requires an extra buffer of lead time if the queue is not
     // serviced within the WOM_DONE callback -- so we'll explicitly call
     // the SS_serve function to mix the buffer if Win32 is not in use
     //

  #ifndef IS_WIN32
     SS_serve(dig);
  #endif

     //
     // Resume foreground thread and return
     //

    resumethreadandexit:
  #ifdef IS_WIN32
     if (AIL_preference[AIL_LOCK_PROTECTION])
       ResumeThread(hAppThread);
  #endif
    justexit:

     OutMilesMutex();
   }

   MSSLockedDecrement(SS_servicing);
   AIL_unlock();

   Only16Pop32s();
}

//############################################################################
//##                                                                        ##
//## Initialize Windows waveOut driver and allocate output buffers          ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_API_waveOutOpen(HDIGDRIVER  FAR *drvr,       //)
                                LPHWAVEOUT  FAR *lphWaveOut,
                                LPWAVEFORMAT     lpFormat)
{
   S32                i;
   U32                test_sample;
   HDIGDRIVER         dig;
   U32                result;
   volatile LPWAVEHDR current,temp;
   LPWAVEHDR FAR     *prev_next;
   DWORD              ticks;
   static char        string[128];

   //
   // Fail if not supported PCM format
   //

   if (lpFormat)
      {
      if (lpFormat->wFormatTag == WAVE_FORMAT_PCM)
         {
         if ((((LPPCMWAVEFORMAT) lpFormat)->wf.nChannels < 1) ||
             (((LPPCMWAVEFORMAT) lpFormat)->wf.nChannels > 2) ||

            ((((LPPCMWAVEFORMAT) lpFormat)->wBitsPerSample != 8 ) &&
             (((LPPCMWAVEFORMAT) lpFormat)->wBitsPerSample != 16)))
            {
            AIL_set_error("Unsupported PCM data format.");

            return MMSYSERR_ERROR;
            }
         }
      else if (lpFormat->wFormatTag == WAVE_FORMAT_EXTENSIBLE)
         {
         if ((((LPPCMWAVEFORMAT) lpFormat)->wBitsPerSample != 8 ) &&
             (((LPPCMWAVEFORMAT) lpFormat)->wBitsPerSample != 16))
            {
            AIL_set_error("Unsupported multichannel PCM data format.");

            return MMSYSERR_ERROR;
            }
         }
      else
         {
         AIL_set_error("Non-PCM wave data type not supported.");
         return MMSYSERR_ERROR;
         }
      }

   //
   // Allocate memory for DIG_DRIVER structure
   //

   dig = (HDIGDRIVER) AIL_mem_alloc_lock(sizeof(struct _DIG_DRIVER));

   if (dig == NULL)
      {
      AIL_set_error("Could not allocate memory for driver descriptor.");

      return MMSYSERR_NOMEM;
      }

   //
   // Explicitly initialize all DIG_DRIVER fields to NULL/0
   //

   AIL_memset(dig,
          0,
          sizeof(*dig));

   SETTAG(dig->tag,"HDIG");

   *drvr = dig;

   //
   // Set reasonable 3D listener defaults
   //

   dig->room_type   = 0;
   dig->D3D.mute_at_max = 1;

   dig->D3D.rolloff_factor  = 1.0F;
   dig->D3D.doppler_factor  = 1.0F;
   dig->D3D.distance_factor = 1.0F;
   
   dig->D3D.listen_position.x = 0.0F;
   dig->D3D.listen_position.y = 0.0F;
   dig->D3D.listen_position.z = 0.0F;

   dig->D3D.listen_face.x = 0.0F;
   dig->D3D.listen_face.y = 0.0F;
   dig->D3D.listen_face.z = 1.0F;

   dig->D3D.listen_up.x = 0.0F;
   dig->D3D.listen_up.y = 1.0F;
   dig->D3D.listen_up.z = 0.0F;

   dig->D3D.listen_cross.x = 1.0F;
   dig->D3D.listen_cross.y = 0.0F;
   dig->D3D.listen_cross.z = 0.0F;

   dig->D3D.listen_velocity.x = 0.0F;
   dig->D3D.listen_velocity.y = 0.0F;
   dig->D3D.listen_velocity.z = 0.0F;

   //
   // Check for MMX support if enabled
   //

   dig->use_MMX = AIL_MMX_available();

   //
   // Attempt to open wave output device
   //

   dig->DS_use_default_format = 0;

   if (lpFormat)
      {
      //
      // Is this a WAVE_FORMAT_EXTENSIBLE struct?  If so, enable multichannel mode
      //

      if (lpFormat->wFormatTag == WAVE_FORMAT_PCM)
         {
         AIL_memcpy(&dig->wformat,lpFormat,sizeof(PCMWAVEFORMAT));
         }
      else if (lpFormat->wFormatTag == WAVE_FORMAT_EXTENSIBLE)
         {
         AIL_memcpy(&dig->wformat,lpFormat,sizeof(WAVEFORMATEXTENSIBLE));
         }
      }
   else
      {
      //
      // Choose a default format for waveOut, and set flag to force use of
      // default primary format for DirectSound
      //

      dig->wformat.wf.wFormatTag     = WAVE_FORMAT_PCM;
      dig->wformat.wf.nChannels      = 2;
      dig->wformat.wf.nSamplesPerSec = 22050;
      dig->wformat.wf.nBlockAlign    = 2;
      dig->wformat.wBitsPerSample    = 8;
      dig->wformat.wf.nAvgBytesPerSec = 44100;

      dig->DS_use_default_format = 1;
      }

   //
   // Physical and logical channel counts may differ if DPL or other matrix
   // format is in use.  For these formats, the caller must stash the logical channel
   // count in the second byte of dig->wformat.wf.nChannels
   //

   dig->physical_channels_per_sample = dig->wformat.wf.nChannels & 0xff;
   dig->logical_channels_per_sample  = dig->wformat.wf.nChannels >> 8;

   dig->wformat.wf.nChannels = (WORD) dig->physical_channels_per_sample;

   if (dig->logical_channels_per_sample == 0)
      {
      dig->logical_channels_per_sample = dig->physical_channels_per_sample;
      }

   result = DIG_waveOutOpen( dig,
                            &dig->hWaveOut,
       (WAVEFORMATEX FAR *) &dig->wformat,
                 (DWORD_PTR) SS_WOM_DONE,
                             0,
                             CALLBACK_FUNCTION);

   if (result)
      {
      AIL_set_error("waveOutOpen() failed.");

      AIL_mem_free_lock(dig);

      return result;
      }

   if (lphWaveOut)
      {
      *lphWaveOut = &dig->hWaveOut;
      }

   //
   // Some Windows drivers may not properly implement waveOutReset().  Allow
   // end users to insert a [MSS] / Reset=0 statement in WIN.INI to keep
   // MSS from calling waveOutReset() if trouble occurs.
   //

   //
   // Assume waveOutReset() does work...
   //

   dig->reset_works = 1;

   //
   // "Reset=1" in WIN.INI forces use of waveOutReset()
   // "Reset=0" in WIN.INI disables use of waveOutReset()
   //

   GetProfileString("MSS",
                    "Reset",
                    "None",
                     string, sizeof(string));

   if (!AIL_strnicmp(string, "1", 1))
      {
      dig->reset_works = 1;
      }

   if (!AIL_strnicmp(string, "0", 1))
      {
      dig->reset_works = 0;
      }

   //
   // Set sample rate and size values and calling params
   //

   #ifndef IS_WIN32
     MSSGetTaskInfo(&dig->callingCT,&dig->callingDS);
   #endif

   AIL_memset(dig->build,
              0,
              sizeof(dig->build));

   if (dig->physical_channels_per_sample == 0)
      {
      dig->hw_format            = DIG_F_OUTPUT_FILTER_IN_USE;
      dig->hw_mode_flags        = 0;
      dig->DMA_rate             = 0;
      dig->bytes_per_channel    = 0;
      dig->hardware_buffer_size = 0;
      dig->samples_per_buffer   = MAX_OUTPUT_FILTER_SAMPLES;
      dig->n_build_buffers      = 0;
      dig->build_size           = 0;
      }
   else
      {
      dig->DMA_rate          = dig->wformat.wf.nSamplesPerSec;
      dig->bytes_per_channel = dig->wformat.wBitsPerSample / 8;

      if (dig->bytes_per_channel == 1)
         {
         test_sample = 0x80808080;

         dig->hw_mode_flags = 0;

         if (dig->physical_channels_per_sample == 1)
            {
            dig->hw_format = DIG_F_MONO_8;
            }
         else
            {
            if (dig->physical_channels_per_sample > 2)
               {
               dig->hw_format = DIG_F_MULTICHANNEL_8;
               }
            else
               {
               dig->hw_format = DIG_F_STEREO_8;
               }
            }
         }
      else
         {
         test_sample = 0;

         dig->hw_mode_flags = DIG_PCM_SIGN;

         if (dig->physical_channels_per_sample == 1)
            {
            dig->hw_format = DIG_F_MONO_16;
            }
         else
            {
            if (dig->physical_channels_per_sample > 2)
               {
               dig->hw_format = DIG_F_MULTICHANNEL_16;
               }
            else
               {
               dig->hw_format = DIG_F_STEREO_16;
               }
            }
         }

      if (AIL_preference[DIG_USE_WAVEOUT])
         {
         //
         // Determine build buffer size by measuring actual playback duration
         // of sample known to be smaller than DMA buffer (i.e., force silence
         // padding)
         //

         current = (LPWAVEHDR) AIL_mem_alloc_lock(sizeof(WAVEHDR));

         if (current == NULL)
            {
            AIL_set_error("Could not allocate memory for test WAVEHDR.");

            DIG_waveOutClose(dig,dig->hWaveOut);
            AIL_mem_free_lock(dig);

            return MMSYSERR_NOMEM;
            }

         current->lpData         = (LPSTR) &test_sample;
         current->dwBufferLength =          4;
         current->dwFlags        =          0;
         current->dwLoops        =          0;
         current->dwUser         =          0;

         DIG_waveOutPrepareHeader(dig,
                                  dig->hWaveOut,
                                  current,
                                  sizeof(WAVEHDR));

         //
         // Disable timer callbacks
         //

         MSSLockedIncrement(disable_callbacks);

         dig->no_wom_done = 1;

         DIG_waveOutWrite(dig,
                          dig->hWaveOut,
                          current,
                          sizeof(WAVEHDR));

         S32 no_done_error = 0;

         ticks = AIL_ms_count();
         while (!(current->dwFlags & WHDR_DONE))
         {
           if ( ( AIL_ms_count() - ticks ) > 750 )
           {
             no_done_error = 1;
             break;
           }
         }

#ifdef IS_WIN32
         OutMilesMutex();
         Sleep(1);
#endif

         DIG_waveOutReset(dig, dig->hWaveOut);

         ticks = AIL_ms_count();

         DIG_waveOutWrite(dig,
                          dig->hWaveOut,
                          current,
                          sizeof(WAVEHDR));

         //
         // Profile the buffer
         //

         if ( no_done_error == 0 )
         {
           while (!(current->dwFlags & WHDR_DONE))
           {
             if ( ( AIL_ms_count() - ticks ) > 750 )
             {
               no_done_error = 1;
               break;
             }
           }
         }

         ticks = AIL_ms_count() - ticks;

         // if way long time, setup a reasonable default
#ifdef IS_WIN32
         if (ticks>75)
           ticks=75;
#else
         if (ticks>100)
           ticks=100;
#endif
         else
         {
           // if impossibly small time, setup a reasonable start
           if (ticks<20)
             ticks=20;
         }

         DIG_waveOutUnprepareHeader(dig,
                                    dig->hWaveOut,
                                    current,
                                    sizeof(WAVEHDR));

         //
         // Give WOM_DONE callback thread a chance to execute before permitting
         // callback processing again
         //

         DIG_waveOutReset(dig, dig->hWaveOut);

#ifdef IS_WIN32
         Sleep(1);
         InMilesMutex();
#endif

         dig->no_wom_done = 0;

         MSSLockedDecrement(disable_callbacks);

         AIL_mem_free_lock(current);

         if ( no_done_error )
         {
           AIL_set_error("Broken waveOut driver - 'buffer done' flag never set.");
           goto err;
         }

         S32 min_chain_element_size = ( ( dig->wformat.wf.nAvgBytesPerSec * (S32) AIL_preference[DIG_MIN_CHAIN_ELEMENT_TIME] ) / 1000 ) & ~1023;

         if ( min_chain_element_size > (S32) AIL_preference[DIG_MAX_CHAIN_ELEMENT_SIZE] )
         {
           min_chain_element_size = (S32) AIL_preference[DIG_MAX_CHAIN_ELEMENT_SIZE];
         }

         do
            {
            //
            // Add 5 ms to duration in milliseconds to provide safety margin
            //
            // Ensure that build buffers are at least DIG_MIN_CHAIN_ELEMENT_SIZE
            // bytes in size
            //

            ticks += 5;

            //
            // Calculate size of DMA buffer in samples
            //
            // Ensure that buffer is multiple of 4 samples in size
            //

            dig->samples_per_buffer  = dig->wformat.wf.nSamplesPerSec * ticks / 1000L;

            dig->samples_per_buffer  = (dig->samples_per_buffer + 3) & ~3;

            S32 physical_channels_per_buffer = dig->samples_per_buffer * dig->physical_channels_per_sample;

            dig->hardware_buffer_size = physical_channels_per_buffer * dig->bytes_per_channel;
            }
         while (dig->hardware_buffer_size < min_chain_element_size );
         }
      else
         {
         //
         // Use DSound to emulate waveOut -- exact buffer size is already known
         //

         dig->hardware_buffer_size = dig->DS_frag_size;

         S32 physical_channels_per_buffer = dig->hardware_buffer_size / dig->bytes_per_channel;

         dig->samples_per_buffer = physical_channels_per_buffer / dig->physical_channels_per_sample;
         }

      //
      // Allocate build buffer(s) for each *logical* channel, plus three working buffers for FLT chain
      //
      // The build buffers correspond 1:1 to the output DMA buffers, except when a matrix format is in use
      //

       //
       // channels/2 stereo build buffers (plus one leftover mono build buffer, if necessary)
       //

       dig->n_build_buffers = ( dig->logical_channels_per_sample + 1 ) / 2;

      S32 nch = dig->logical_channels_per_sample;

      S32 spkr_chan = 0;

      for (i=0; i < dig->n_build_buffers; i++)
         {
         dig->build[i].chans  = (nch > 1) ? 2 : 1;
         dig->build[i].bytes  = sizeof(S32) * dig->samples_per_buffer * dig->build[i].chans;
         dig->build[i].buffer = (S32 FAR *) AIL_mem_alloc_lock(dig->build[i].bytes);

         dig->build[i].speaker_offset = spkr_chan;
         spkr_chan += dig->build[i].chans;

         if (dig->build[i].buffer == NULL)
            {
            AIL_set_error("Could not allocate build buffer.");

free_build:
            for (S32 j=0; j < (sizeof(dig->build) / sizeof(dig->build[0])); j++)
               {
               if (dig->build[j].buffer != NULL)
                  {
                  AIL_mem_free_lock(dig->build[j].buffer);
                  dig->build[j].buffer = NULL;
                  }
               }
err:
            DIG_waveOutClose(dig, dig->hWaveOut);
            AIL_mem_free_lock(dig);

            return MMSYSERR_ERROR;
            }

         nch -= dig->build[i].chans;
         }

      }

   for (i=0; i < EXTRA_BUILD_BUFFERS; i++)
      {
      S32 j = MAX_SPEAKERS + i;

      //
      // Working build buffers always have space allocated for 2 channels, even
      // though they may be used as mono buffers by the filter chain
      //

      dig->build[j].chans  = 2;
      dig->build[j].bytes  = sizeof(S16) * dig->samples_per_buffer * dig->build[j].chans;
      dig->build[j].buffer = (S32 FAR *) AIL_mem_alloc_lock(dig->build[j].bytes);

      if (dig->build[j].buffer == NULL)
         {
         AIL_set_error("Could not allocate build buffer.");
         goto free_build;
         }
      }

   //
   // Build size is a byte offset in a simulated S16 (not S32) dest-sample mixer loop
   //

   dig->build_size = dig->build[0].bytes>>1;  

   if (AIL_allocate_reverb_buffers( dig ) == 0 )
   {
      AIL_set_error("Could not allocate reverb build buffer.");

      goto free_build;
   }

   //
   // Initialize default speaker-position array
   //
   // Positions assume left-handed coordinate frame with identity =
   // face = +Z, right = +X, up = +Y
   //

#define RSQRTF 0.707106781F

   MSSVECTOR3D SPKR[MAX_SPEAKERS];  // Listener-relative speaker locations

   AIL_memset(SPKR,
              0,
              sizeof(SPKR));

   switch (dig->logical_channels_per_sample)
      {
      case 1:
         {
         SPKR[0].x = 0.0F;   SPKR[0].y = 0.0F;   SPKR[0].z = 1.0F;  
         break;
         }

      case 2:     // Must call via AIL_open_digital_driver() to distinguish stereo speakers (Z=1/sqrt(2)) from headphones (Z=0)
         {
#if 1
         SPKR[0].x = -RSQRTF; SPKR[0].y = 0.0F;   SPKR[0].z = RSQRTF;
         SPKR[1].x =  RSQRTF; SPKR[1].y = 0.0F;   SPKR[1].z = RSQRTF;
#else
         SPKR[0].x = -1.0F;   SPKR[0].y = 0.0F;   SPKR[0].z = 0.0F;  
         SPKR[1].x =  1.0F;   SPKR[1].y = 0.0F;   SPKR[1].z = 0.0F;  
#endif
         break;
         }

      case 3:     // Used for Dolby Surround
         {
         SPKR[0].x = -RSQRTF; SPKR[0].y = 0.0F;   SPKR[0].z =  RSQRTF;
         SPKR[1].x =  RSQRTF; SPKR[1].y = 0.0F;   SPKR[1].z =  RSQRTF;
         SPKR[2].x =  0.0F;   SPKR[2].y = 0.0F;   SPKR[2].z = -1.0F;  
         break;
         }

      case 4:
         {
         SPKR[0].x = -RSQRTF; SPKR[0].y = 0.0F;   SPKR[0].z =  RSQRTF;
         SPKR[1].x =  RSQRTF; SPKR[1].y = 0.0F;   SPKR[1].z =  RSQRTF;
         SPKR[2].x = -RSQRTF; SPKR[2].y = 0.0F;   SPKR[2].z = -RSQRTF;
         SPKR[3].x =  RSQRTF; SPKR[3].y = 0.0F;   SPKR[3].z = -RSQRTF;
         break;
         }

      case 6:
         {
         SPKR[0].x = -RSQRTF; SPKR[0].y = 0.0F;   SPKR[0].z =  RSQRTF;
         SPKR[1].x =  RSQRTF; SPKR[1].y = 0.0F;   SPKR[1].z =  RSQRTF;
         SPKR[2].x =  0.0F;   SPKR[2].y = 0.0F;   SPKR[2].z =  1.0F;  
         SPKR[3].x =  0.0F;   SPKR[3].y = 0.0F;   SPKR[3].z =  0.0F;  
         SPKR[4].x = -RSQRTF; SPKR[4].y = 0.0F;   SPKR[4].z = -RSQRTF;
         SPKR[5].x =  RSQRTF; SPKR[5].y = 0.0F;   SPKR[5].z = -RSQRTF;
         break;
         }

      case 7:
         {
         SPKR[0].x = -RSQRTF; SPKR[0].y = 0.0F;   SPKR[0].z =  RSQRTF;
         SPKR[1].x =  RSQRTF; SPKR[1].y = 0.0F;   SPKR[1].z =  RSQRTF;
         SPKR[2].x =  0.0F;   SPKR[2].y = 0.0F;   SPKR[2].z =  1.0F;  
         SPKR[3].x =  0.0F;   SPKR[3].y = 0.0F;   SPKR[3].z =  0.0F;  
         SPKR[4].x = -RSQRTF; SPKR[4].y = 0.0F;   SPKR[4].z = -RSQRTF;
         SPKR[5].x =  RSQRTF; SPKR[5].y = 0.0F;   SPKR[5].z = -RSQRTF;
         SPKR[6].x =  0.0F;   SPKR[6].y = 0.0F;   SPKR[6].z = -1.0F;  
         break;
         }

      case 8:
         {
         SPKR[0].x = -RSQRTF; SPKR[0].y = 0.0F;   SPKR[0].z =  RSQRTF;
         SPKR[1].x =  RSQRTF; SPKR[1].y = 0.0F;   SPKR[1].z =  RSQRTF;
         SPKR[2].x =  0.0F;   SPKR[2].y = 0.0F;   SPKR[2].z =  1.0F;  
         SPKR[3].x =  0.0F;   SPKR[3].y = 0.0F;   SPKR[3].z =  0.0F;  
         SPKR[4].x = -RSQRTF; SPKR[4].y = 0.0F;   SPKR[4].z = -RSQRTF;
         SPKR[5].x =  RSQRTF; SPKR[5].y = 0.0F;   SPKR[5].z = -RSQRTF;
         SPKR[6].x = -1.0F;   SPKR[6].y = 0.0F;   SPKR[6].z =  0.0F;  
         SPKR[7].x =  1.0F;   SPKR[7].y = 0.0F;   SPKR[7].z =  0.0F;  
         break;
         }

      case 9:
         {
         SPKR[0].x = -RSQRTF; SPKR[0].y = 0.0F;   SPKR[0].z =  RSQRTF;
         SPKR[1].x =  RSQRTF; SPKR[1].y = 0.0F;   SPKR[1].z =  RSQRTF;
         SPKR[2].x =  0.0F;   SPKR[2].y = 0.0F;   SPKR[2].z =  1.0F;  
         SPKR[3].x =  0.0F;   SPKR[3].y = 0.0F;   SPKR[3].z =  0.0F;  
         SPKR[4].x = -RSQRTF; SPKR[4].y = 0.0F;   SPKR[4].z = -RSQRTF;
         SPKR[5].x =  RSQRTF; SPKR[5].y = 0.0F;   SPKR[5].z = -RSQRTF;
         SPKR[6].x =  0.0F;   SPKR[6].y = 0.0F;   SPKR[6].z = -1.0F;  
         SPKR[7].x = -1.0F;   SPKR[7].y = 0.0F;   SPKR[7].z =  0.0F;  
         SPKR[8].x =  1.0F;   SPKR[8].y = 0.0F;   SPKR[8].z =  0.0F;  
         break;
         }
      }

   //
   // Set default falloff power and reverb response
   //

   AIL_set_speaker_configuration(dig, 
                                 SPKR, 
                                 dig->logical_channels_per_sample, 
                                 3.0F);

   AIL_set_speaker_reverb_levels(dig, 
                                 NULL,
                                 NULL,
                                 0);

   //
   // Allocate physical SAMPLE structures for driver
   //

   dig->n_samples        = (S32) AIL_get_preference(DIG_MIXER_CHANNELS);
   dig->n_active_samples = 0;

   dig->master_volume    = 1.0F;
   dig->master_dry       = 1.0F;
   dig->master_wet       = 1.0F;

   dig->samples = (HSAMPLE) AIL_mem_alloc_lock(sizeof(struct _SAMPLE) * dig->n_samples);

   if (dig->samples == NULL)
      {
free_reverb:
      AIL_set_error("Could not allocate SAMPLE structures.");

      AIL_mem_free_lock(dig->reverb_build_buffer);
      goto free_build;
      }

   dig->sample_status = (U32 *) AIL_mem_alloc_lock(sizeof(U32) * dig->n_samples);

   if (dig->sample_status == NULL)
      {
      AIL_mem_free_lock((void FAR *) dig->samples);
      goto free_reverb;
      }

   AIL_memset(dig->samples,
              0,
              sizeof(struct _SAMPLE) * dig->n_samples);

   for (i=0; i < dig->n_samples; i++)
      {
      SETTAG(dig->samples[i].tag,"HSAM");

      dig->sample_status[i]  = SMP_FREE;
      dig->samples[i].index  = i;
      dig->samples[i].driver = dig;
      dig->samples[i].is_3D  = FALSE;
      }

   //
   // Allocate waveOut-specific data structures
   // These are not needed for DirectSound support
   //

   if (AIL_preference[DIG_USE_WAVEOUT])
      {
      //
      // Allocate and page-lock approximately DIG_OUTPUT_BUFFER_SIZE bytes'
      // worth of output WAVEHDR buffers -- or at least 4 buffers
      //

      dig->n_buffers = ((S32) AIL_get_preference(DIG_OUTPUT_BUFFER_SIZE) /
                        dig->hardware_buffer_size);

      if (dig->n_buffers < 4)
         {
         dig->n_buffers = 4;
         }

      //
      // Allocate and page-lock circular "return list" used to
      // communicate between SS_WOM_DONE() and SS_serve
      //

      dig->return_list = (LPWAVEHDR FAR *) AIL_mem_alloc_lock(sizeof(LPWAVEHDR) * dig->n_buffers);

      dig->return_head = 0;
      dig->return_tail = 0;

      //
      // Allocate circular chain of WAVEHDR structures and their associated
      // HDR_USER structures
      //

      prev_next = &dig->first;

      for (i=0; i < dig->n_buffers; i++)
         {
         current = (LPWAVEHDR) AIL_mem_alloc_lock(sizeof(WAVEHDR));

         *prev_next = current;

         if (current == NULL)
            {
            AIL_set_error("Could not allocate WAVEHDR structures.");

            i = -1;
            break;
            }

         current->dwUser = (UINTa) AIL_mem_alloc_lock(sizeof(HDR_USER));

         if (current->dwUser == 0)
            {
            AIL_set_error("Could not allocate HDR_USER structures.");

            i = -1;
            break;
            }

         CURRENT_USER->owner = dig;

         prev_next = &(CURRENT_USER->next);
         }

      //
      // Exit if memory allocation error occurred
      //

      if (i == -1)
         {
         current = dig->first;

         while (current != NULL)
            {
            if (current->dwUser == 0)
               {
               AIL_mem_free_lock(current);
               break;
               }

            temp = CURRENT_USER->next;

            AIL_mem_free_lock(CURRENT_USER);
            AIL_mem_free_lock(current);

            current = temp;
            }

         AIL_mem_free_lock((void FAR *) dig->return_list);
         AIL_mem_free_lock((void FAR *) dig->reverb_build_buffer);
         AIL_mem_free_lock((void FAR *) dig->samples);
         AIL_mem_free_lock((void FAR *) dig->sample_status);
         goto free_build;
         }

      //
      // Otherwise, make list circular
      //

      *prev_next = dig->first;

      //
      // Allocate buffer memory and page-lock it with waveOutPrepareHdr()
      //
      // Init flag bytes to WHDR_DONE, to force foreground timer to perform
      // waveOutWrite calls
      //
      // Bit 0 of dwBufferLength is used as "virgin" bit to force posting of
      // newly-initialized buffers
      //

      current = dig->first;

      for (i=0; i < dig->n_buffers; i++)
         {
         current->lpData = (C8 FAR *) AIL_mem_alloc_lock(dig->hardware_buffer_size);

         if (current->lpData == NULL)
            {
            AIL_set_error("Could not allocate output buffer.");

            i = -1;
            break;
            }

         current->dwBufferLength = dig->hardware_buffer_size;
         current->dwFlags        = 0;
         current->dwLoops        = 0;

         DIG_waveOutPrepareHeader(dig,
                                 dig->hWaveOut,
                                 current,
                                 sizeof(WAVEHDR));

         current = CURRENT_USER->next;
         }

      //
      // Exit if memory allocation error occurred
      //

      if (i == -1)
         {
         current = dig->first;

         for (i=0; i < dig->n_buffers; i++)
            {
            if (current->lpData != NULL)
               {
               DIG_waveOutUnprepareHeader(dig,
                                          dig->hWaveOut,
                                          current,
                                          sizeof(WAVEHDR));

               AIL_mem_free_lock(current->lpData);
               }

            temp = CURRENT_USER->next;

            AIL_mem_free_lock(CURRENT_USER);
            AIL_mem_free_lock(current);

            current = temp;
            }

         AIL_mem_free_lock((void FAR *) dig->return_list);
         AIL_mem_free_lock((void FAR *) dig->reverb_build_buffer);
         AIL_mem_free_lock((void FAR *) dig->samples);
         AIL_mem_free_lock((void FAR *) dig->sample_status);

         goto free_build;
         }
      }

      //
      // Link HDIGDRIVER into chain used by timers
      //

      if (DIG_first!=NULL)
        {
        dig->next=DIG_first;
        }
      else
        {

        dig->next = NULL;

#ifdef IS_WIN32
        AIL_primary_digital_driver(dig);
#endif

      //
      // If this is the first driver initialized, set up
      // foreground timer to maintain output buffer queue
      //

      dig->foreground_timer = (DWORD) SetTimer((HWND) NULL,
                                              0,
                                              50,
                                              (TIMERPROC) Timer_foreground_service);

      //
      // If timer allocation failed, release all resources and return
      // error
      //

      if (dig->foreground_timer == 0)
         {
#ifdef IS_WIN32
         if (DIG_first == NULL)
            {
            AIL_release_timer_handle(dig->backgroundtimer);
            }
#endif

         if (AIL_preference[DIG_USE_WAVEOUT])
            {
            current = dig->first;

            for (i=0; i < dig->n_buffers; i++)
               {
               DIG_waveOutUnprepareHeader(dig,
                                          dig->hWaveOut,
                                          current,
                                          sizeof(WAVEHDR));

               AIL_mem_free_lock(current->lpData);

               temp = CURRENT_USER->next;

               AIL_mem_free_lock(CURRENT_USER);
               AIL_mem_free_lock(current);

               current = temp;
               }

            AIL_mem_free_lock((void FAR *) dig->return_list);
            }

         AIL_mem_free_lock((void FAR *) dig->reverb_build_buffer);
         AIL_mem_free_lock((void FAR *) dig->samples);
         AIL_mem_free_lock((void FAR *) dig->sample_status);
         goto free_build;
         }
      }

   DIG_first = dig;

   //
   // Background timer used only for Win32 -- SS_serve called
   // explicitly from SS_WOM_DONE if Win16 is in use, for improved
   // latency performance
   //

#ifdef IS_WIN32

   dig->backgroundtimer = AIL_register_timer((AILTIMERCB)SS_serve);

   //
   // If timer allocation failed, release all resources and return
   // error
   //

   if (dig->backgroundtimer == -1)
      {
      AIL_set_error("Out of timer handles.");

      if (AIL_preference[DIG_USE_WAVEOUT])
         {
         current = dig->first;

         for (i=0; i < dig->n_buffers; i++)
            {
            DIG_waveOutUnprepareHeader(dig,
                                       dig->hWaveOut,
                                       current,
                                       sizeof(WAVEHDR));

            AIL_mem_free_lock(current->lpData);

            temp = CURRENT_USER->next;

            AIL_mem_free_lock(CURRENT_USER);
            AIL_mem_free_lock(current);

            current = temp;
            }

         AIL_mem_free_lock((void FAR *) dig->return_list);
         }

      KillTimer((HWND) NULL, (UINT) dig->foreground_timer);
      AIL_mem_free_lock((void FAR *) dig->reverb_build_buffer);
      AIL_mem_free_lock((void FAR *) dig->samples);
      AIL_mem_free_lock((void FAR *) dig->sample_status);
      goto free_build;
      }
#endif

   //
   // Init driver pipeline stages
   //

   for (i=0; i < N_DIGDRV_STAGES; i++)
      {
      dig->pipeline[i].active = 0;
      }

   //
   // Select default mixer flush/copy providers
   //
   // (These won't be used if an output filter is present)
   //

   HPROVIDER HP;

   RIB_enumerate_providers("MSS mixer services",
                            NULL,
                           &HP);

   AIL_set_digital_driver_processor(dig,
                                    DP_DEFAULT_FILTER,
                                    0);

   AIL_set_digital_driver_processor(dig,
                                    DP_DEFAULT_MERGE,
                                    HP);

   AIL_set_digital_driver_processor(dig,
                                    DP_FLUSH,
                                    HP);

   AIL_set_digital_driver_processor(dig,
                                    DP_COPY,
                                    HP);

   AIL_set_digital_driver_processor(dig,
                                    DP_MC_COPY,
                                    HP);

   AIL_set_digital_driver_processor(dig,
                                    DP_ADPCM_DECODE,
                                    HP);

   //
   // Start 100 Hz background service timer to process WOM_DONE events
   //

#ifdef IS_WIN32

   AIL_set_timer_frequency(dig->backgroundtimer,         1000 / (S32) AIL_preference[DIG_DS_FRAGMENT_SIZE]);
   AIL_set_timer_user     (dig->backgroundtimer, (UINTa) dig);
   AIL_start_timer        (dig->backgroundtimer             );

#else

  if (dig->next == NULL) {
    char name[128];    // load the background task exe
    char str[32];
    U16 show[2];
    struct _lp {
       WORD   segEnv;                  /* child environment  */
       LPSTR  lpszCmdLine;             /* child command tail */
       LPWORD lpwShow;                 /* how to show child  */
       LPWORD lpwReserved;             /* must be NULL       */
    } lp;

    lp.segEnv=0;
    lp.lpszCmdLine=(LPSTR)str;
    show[0]=2;
    show[1]=SW_SHOWNORMAL;
    lp.lpwShow=show;
    lp.lpwReserved=0;

    str[0]=(U8)AIL_sprintf(str+1,"!%li",(U32)(LPVOID)SS_Win16_thread);

    AIL_strcpy(name,MSS_Directory);
    AIL_strcat(name,"\\");

#ifdef MSSXTRA
    AIL_strcat(name,"MSSBX16.TSK");  // set up the name of the task
#else
    AIL_strcat(name,"MSSB16.TSK");  // set up the name of the task
#endif
    {
      WORD err=SetErrorMode(0x8000);
      if ((U16)LoadModule(name,&lp)<32) {  // Start the background task

        //now try in the redist directory
        if (*AIL_redist_directory)
        {
          if ((AIL_redist_directory[0]=='\\') || (AIL_redist_directory[1]==':'))
          {
            AIL_strcpy(name,AIL_redist_directory);
            AIL_strcat(name,"\\");
          }
          else
          {
            // relative path
            AIL_strcpy(name,MSS_Directory);
            AIL_strcat(name,"\\");
            AIL_strcat(name,AIL_redist_directory);
          }

          #ifdef MSSXTRA
            AIL_strcat(name,"MSSBX16.TSK");  // set up the name of the task
          #else
            AIL_strcat(name,"MSSB16.TSK");  // set up the name of the task
          #endif

          if ((U16)LoadModule(name,&lp)>=32)  // Start the background task
            goto cont;
        }
        OutputDebugString("Could not load MSS background task: ");
        OutputDebugString(name);
        OutputDebugString("\n");
      }
     cont:
      SetErrorMode(err);
    }
  }

#endif

   //
   // Return normally
   //

   return 0;
}

//############################################################################
//##                                                                        ##
//## Shut down Windows waveOut driver and free output buffers               ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_waveOutClose(HDIGDRIVER dig)
{
   HDIGDRIVER cur,prev;
   LPWAVEHDR  current,temp;
   S32        i;

   if ((dig == NULL) || (DIG_first == NULL))
      {
      return;
      }

   AILSTRM_shutdown(dig);

   MSSLockedIncrement( SS_servicing );

   while ( SS_servicing != 1 )
   {
     Sleep( 1 );
   }

   InMilesMutex();

   //
   // Disable callback processing
   //

   MSSLockedIncrement(disable_callbacks);

   dig->no_wom_done = 1;

   //
   // Stop playback
   //

   if (!dig->released)
     SS_stop_DIG_driver_playback(dig);

   //
   // Release any open sample handles (to ensure that pipeline resources
   // are deallocated properly)
   //

   for (i=0; i < dig->n_samples; i++)
      {
      if (dig->sample_status[i] != SMP_FREE)
         {
         AIL_release_sample_handle(&dig->samples[i]);
         }
      }


#ifdef IS_WIN32

   //
   // Release any filters associated with this driver
   //

   FLT_disconnect_driver(dig);

   //
   // Kill buffer maintenance timer
   //

    AIL_release_timer_handle(dig->backgroundtimer);

#endif

   //
   // Unlink from foreground service chain
   //

   if (dig == DIG_first)
      {
      DIG_first = dig->next;

      if (DIG_first == NULL)
         {
         //
         // If no more drivers left, kill foreground timer
         //

#ifndef IS_WIN32

         if (Win16_thread_task) {
           PostAppMessage(Win16_thread_task,WM_USER+230,0,0);
           DirectedYield(Win16_thread_task);  // run the task specifically
           Win16_thread_task=0;
         }

#endif
         KillTimer((HWND) NULL, (UINT) dig->foreground_timer);
         }
      }
   else
      {
      prev = DIG_first;
      cur  = DIG_first->next;

      while (cur != dig)
         {
         if (cur == NULL)
            {
            MSSLockedDecrement(disable_callbacks);
            return;
            }

         prev = cur;
         cur  = cur->next;
         }

      prev->next = cur->next;
      }

   //
   // Reset driver to force return of all structures
   //
   // In Win32, ensure that all buffer-return threads have a chance to run
   //

#ifdef IS_WIN32
   OutMilesMutex();
#endif

   if (!dig->released)
     DIG_waveOutReset(dig,dig->hWaveOut);

#ifdef IS_WIN32
   Sleep(1);
   InMilesMutex();
#endif

   //
   // Release all resources allocated by this driver
   //

   if (AIL_preference[DIG_USE_WAVEOUT])
      {
      current = dig->first;

      for (i=0; i < dig->n_buffers; i++)
         {
         if (!dig->released)
         DIG_waveOutUnprepareHeader(dig,
                                    dig->hWaveOut,
                                    current,
                                    sizeof(WAVEHDR));

         AIL_mem_free_lock(current->lpData);

         temp = CURRENT_USER->next;

         AIL_mem_free_lock(CURRENT_USER);
         AIL_mem_free_lock(current);

         current = temp;
         }
      }

   //
   // Close driver and free resources
   //

   if (!dig->released)
     DIG_waveOutClose(dig,dig->hWaveOut);

   if (AIL_preference[DIG_USE_WAVEOUT])
      {
      AIL_mem_free_lock((void FAR *) dig->return_list);
      }

   S32 j;

   AIL_mem_free_lock((void FAR *) dig->samples);
   AIL_mem_free_lock((void FAR *) dig->sample_status);

   for (j=0; j < (sizeof(dig->build) / sizeof(dig->build[0])); j++)
      {
      if (dig->build[j].buffer != NULL)
         {
         AIL_mem_free_lock(dig->build[j].buffer);
         dig->build[j].buffer = NULL;
         }
      }

   AIL_mem_free_lock(dig->reverb_build_buffer);

   MSSLockedDecrement( SS_servicing );

   MSSLockedDecrement(disable_callbacks);

#ifdef IS_WIN32
   OutMilesMutex();
   Sleep(1);
#endif

   AIL_mem_free_lock(dig);
}

//############################################################################
//##                                                                        ##
//## Temporarily release the Windows HWAVEOUT device                        ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_API_digital_handle_release(HDIGDRIVER dig)
{
   LPWAVEHDR  current;
   S32        i;

   if (dig == NULL)
      {
      dig = primary_digital_driver;

      if (dig == NULL)
         {
         dig = DIG_first;
         }
      }

   if ((dig == NULL) || (DIG_first == NULL))
      {
      return(0);
      }

   if (dig->voice_filter)
      {
      //
      // Not supported if a voice output filter is in use (we can't be sure we'll get all our
      // HSAMPLE voices back)
      //

      return 0;
      }

   if (dig->released)
     return(1);

   //
   // Disable callback processing
   //

   MSSLockedIncrement(disable_callbacks);

   dig->no_wom_done = 1;

   //
   // Stop playback
   //

   SS_stop_DIG_driver_playback(dig);

   //
   // Reset driver to force return of all structures
   //
   // In Win32, ensure that all buffer-return threads have a chance to run
   //

#ifdef IS_WIN32
   OutMilesMutex();
#endif

   DIG_waveOutReset(dig,dig->hWaveOut);

#ifdef IS_WIN32
   Sleep(1);
   InMilesMutex();
#endif

   //
   // Unprepare the headers
   //

   for (i=0; i < dig->n_samples; i++)
      {
      if (dig->sample_status[i] == SMP_PLAYING)
         {
         dig->sample_status[i]=SMP_PLAYINGBUTRELEASED;
         }
      }

   if (AIL_preference[DIG_USE_WAVEOUT])
      {
      current = dig->first;

      for (i=0; i < dig->n_buffers; i++)
         {
         DIG_waveOutUnprepareHeader(dig,
                                    dig->hWaveOut,
                                    current,
                                    sizeof(WAVEHDR));

         current = CURRENT_USER->next;
         }
      }

   //
   // Close driver
   //

   DIG_waveOutClose(dig,dig->hWaveOut);

   dig->playing=0;

   dig->released=1;

   dig->hWaveOut=0;

   dig->no_wom_done = 0;

   MSSLockedDecrement(disable_callbacks);

#ifdef IS_WIN32
   OutMilesMutex();
   Sleep(1);
   InMilesMutex();
#endif

   return(1);
}

//############################################################################
//##                                                                        ##
//## Reacquire the Windows HWAVEOUT device                                  ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_API_digital_handle_reacquire(HDIGDRIVER dig)
{
   LPWAVEHDR  current;
   U32 result;
   S32 i;
   S32 playing=0;

   if (dig == NULL)
      {
      dig = primary_digital_driver;

      if (dig == NULL)
         {
         dig = DIG_first;
         }
      }

   if ((dig == NULL) || (DIG_first == NULL))
      {
      return(0);
      }

   if (dig->voice_filter)
      {
      //
      // Not supported if a voice output filter is in use (we can't be sure we'll get all our
      // HSAMPLE voices back)
      //

      return 0;
      }

   if (!dig->released)
     return(1);

   //
   // Disable callback processing
   //

   MSSLockedIncrement(disable_callbacks);

   result = DIG_waveOutOpen( dig,
                            &dig->hWaveOut,
           (LPWAVEFORMATEX) &dig->wformat,
                 (DWORD_PTR) SS_WOM_DONE,
                             0,
                             CALLBACK_FUNCTION);

   if (result)
      {
      AIL_set_error("waveOutOpen() failed.");

      MSSLockedDecrement(disable_callbacks);

      return(0);
      }

   //
   // Reprepare the headers
   //

   for (i=0; i < dig->n_samples; i++)
      {
      if (dig->sample_status[i] == SMP_PLAYINGBUTRELEASED)
         {
         //
         // start reverb if we need to
         //

         SS_update_sample_reverb_state(&dig->samples[i]);

         dig->sample_status[i]=SMP_PLAYING;
         playing=1;
         }
      }

   if (AIL_preference[DIG_USE_WAVEOUT])
      {
      current = dig->first;

      for (i=0; i < dig->n_buffers; i++)
         {
         DIG_waveOutPrepareHeader(dig,
                                 dig->hWaveOut,
                                 current,
                                 sizeof(WAVEHDR));

         current = CURRENT_USER->next;
         }
      }

  dig->released=0;

  if (playing)
    SS_start_DIG_driver_playback(dig->samples);

   MSSLockedDecrement(disable_callbacks);

  return(1);
}

//############################################################################
//##                                                                        ##
//## Externally-callable service function for foreground timer              ##
//##                                                                        ##
//############################################################################

static S32 only_one_serve=0;
static U32 last_serve=0;

static void low_serve()
{
   last_serve=AIL_ms_count();

   MSSLockedIncrement(only_one_serve);

   if (only_one_serve==1)
   {
     //
     // If Win32 in use, call foreground service only if DirectSound not in use
     //

     #ifdef IS_WIN32
     if (AIL_preference[DIG_USE_WAVEOUT] == YES)
     #endif
     {
       SS_foreground_service();
     }

     OutMilesMutex();
     stream_background();
     InMilesMutex();

   }

   MSSLockedDecrement(only_one_serve);

   // check all the buffer status's
   HDIGDRIVER dig=DIG_first;
   while (dig)
   {
     SS_serve(dig);
     dig=dig->next;
   }
}


void AILCALL AIL_API_serve()
{
  U32 t=AIL_ms_count();
  if ((t-last_serve)>(U32)AIL_preference[DIG_DS_FRAGMENT_SIZE])
  {
    InMilesMutex();
    low_serve();
    OutMilesMutex();
  }
}


//############################################################################
//##                                                                        ##
//## Get digital driver configuration                                       ##
//##                                                                        ##
//############################################################################

void     AILCALL AIL_API_digital_configuration     (HDIGDRIVER dig, //)
                                            S32    FAR*rate,
                                            S32    FAR*format,
                                            char   FAR*config)
{
   UINT id;
   WAVEOUTCAPS CAPS;

   if (dig==NULL)
     return;

   if (rate != NULL)
      {
      *rate = dig->DMA_rate;
      }

   if (format != NULL)
      {
      *format = dig->hw_format;
      }
   if (config != NULL)
      {

      DIG_waveOutGetID(dig,dig->hWaveOut,&id);
      DIG_waveOutGetDevCaps(dig,id, &CAPS, sizeof(CAPS));

      *config=0;

      if (AIL_preference[DIG_USE_WAVEOUT])
         {
         lstrcat(config,"WaveOut - ");
         }

      lstrcat(config,CAPS.szPname);
      }
}

S32 AILCALL AIL_API_digital_latency(HDIGDRIVER dig)
{
  if (dig)
  {
    if (AIL_preference[DIG_USE_WAVEOUT])
    {
      if (dig->playing)
      {
        return( 10+mult64anddiv(dig->hardware_buffer_size*
#ifdef IS_WIN32
                4
#else
                3
#endif
                ,1000,dig->wformat.wf.nAvgBytesPerSec));

      }
      else
      {
        return(10);
      }
    }
    else
    {
      if (dig->emulated_ds)
      {
        return( 200+mult64anddiv(dig->DS_frag_size,1000,dig->wformat.wf.nAvgBytesPerSec)*
                    (S32) AIL_preference[DIG_DS_MIX_FRAGMENT_CNT] );
      }
      else
      {
        if (AIL_preference[DIG_DS_USE_PRIMARY])
        {
          return( mult64anddiv(dig->DS_frag_size,1000,dig->wformat.wf.nAvgBytesPerSec)*
                  (S32) AIL_preference[DIG_DS_MIX_FRAGMENT_CNT] );
        }
        else
        {
          return( 20+mult64anddiv(dig->DS_frag_size,1000,dig->wformat.wf.nAvgBytesPerSec)*
                     (S32) AIL_preference[DIG_DS_MIX_FRAGMENT_CNT] );
        }
      }
    }
  }
  return(0);
}

