//############################################################################
//##                                                                        ##
//##  Miles Sound System                                                    ##
//##                                                                        ##
//##  mssinput.cpp: Digital sound input API module and support routines     ##
//##                                                                        ##
//##  16-bit protected-mode source compatible with MSC 7.0                  ##
//##  32-bit protected-mode source compatible with MSC 9.0                  ##
//##                                                                        ##
//##  Version 1.00 of 8-Dec-98: Initial                                     ##
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

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#define STRICT
#include <windows.h>
#include <mmsystem.h>

#ifdef IS_WIN32
#include <dsound.h>
#endif

#include "mss.h"
#include "imssapi.h"

#include <stdio.h>
#include <stdlib.h>

#ifndef IS_WIN32
#define WAVEFORMATEX PCMWAVEFORMAT
#define LPWAVEFORMATEX LPWAVEFORMAT
#endif

//
// Assembly-language functions
//

extern "C"
{
#ifdef IS_WIN32

#define SETTAG(var,tag) AIL_memcpy(var,tag,4);

#else

#define SETTAG(var,tag) AIL_memcpy(var,tag,4);

#endif
}

//
// Macros to hide DirectSound typecasting
//

#define ASDSBUF(ptr) ((LPDIRECTSOUNDBUFFER)ptr)
#define ASDS(ptr)    ((LPDIRECTSOUND)ptr)

extern C8 AIL_error[256];

#define SERVICE_RATE 200   // Poll buffers at 200 Hz interval

/****************************************************************************/
//
// Flush a DMA half-buffer with silence
//
/****************************************************************************/

static void flush_DMA_buffer(HDIGINPUT dig, //)
                             S32       which)
{
   AIL_memset(dig->wavehdr[which].lpData,
              dig->silence,
              dig->DMA_size);
}

/****************************************************************************/
//
// Callback function to mark input buffer available
//
/****************************************************************************/

void FAR PASCAL WIM_DATA_callback(HWAVEOUT  hWaveIn, //)
                                  UINT      wMsg,
                                  DWORD     dwInstance,
                                  DWORD_PTR dwParam1,
                                  DWORD     dwParam2)
{                   
   LPWAVEHDR current;

   //
   // Return if message not WIM_DATA
   //

   if (wMsg != WIM_DATA)
      {
      return;
      }

   current = (LPWAVEHDR) dwParam1;

   //
   // Clear our "submitted" flag
   //

   current->dwUser &= ~0x80000000;
}

/****************************************************************************/
//
// Callback function to poll for valid data buffers and return them to the
// application
//
/****************************************************************************/

void AILCALLBACK background_callback(UINTa user)
{
   S32 i;

   //
   // Check to see if a new half-buffer is ready to be returned to the app
   //

   HDIGINPUT dig = (HDIGINPUT) user;

   for (i=0; i < N_WAVEIN_BUFFERS; i++)
      {
      if (dig->wavehdr[i].dwFlags & WHDR_DONE)
         {
         break;
         }
      }

   if (i == N_WAVEIN_BUFFERS)
      {
      //
      // No available buffers yet, return
      //

      return;
      }

   //
   // Invoke application callback with buffered data
   //

   if (dig->input_enabled)
      {
      dig->info.callback(dig->wavehdr[i].lpData,
                         dig->info.buffer_size,
                         dig->callback_user);
      }

   //
   // Flush the buffer with silence to prepare for its next cycle
   //

   flush_DMA_buffer(dig, i);

   //
   // If device has not been reset, submit buffer back to waveInAddBuffer
   // function
   //

   dig->wavehdr[i].dwBytesRecorded = 0;

   if (dig->device_active)
      {
      MMRESULT result = waveInAddBuffer(dig->hWaveIn,
                           (LPWAVEHDR) &dig->wavehdr[i],
                                        sizeof(WAVEHDR));

      if (result)
         {
         wsprintf(AIL_error,"waveInAddBuffer() failed, code = 0x%X\n",result);
         }

      //
      // Set flag to indicate we've submitted this buffer
      //

      dig->wavehdr[i].dwUser |= 0x80000000;
      }
}

//############################################################################
//##                                                                        ##
//## Open input device                                                      ##
//##                                                                        ##
//############################################################################

HDIGINPUT AILCALL AIL_API_open_input (AIL_INPUT_INFO FAR *info)
{
   HDIGINPUT dig;

   //
   // Allocate memory for DIG_INPUT_DRIVER structure
   //

   dig = (HDIGINPUT) AIL_mem_alloc_lock(sizeof(struct _DIG_INPUT_DRIVER));

   if (dig == NULL)
      {
      AIL_set_error("Could not allocate memory for input driver descriptor.");

      return NULL;
      }

   //
   // Explicitly initialize all DIG_INPUT_DRIVER fields to NULL/0
   //

   AIL_memset(dig,
              0,
              sizeof(*dig));

   SETTAG(dig->tag,"HDIN");

   dig->callback_user=info->user_data;

   //
   // Assign info field to descriptor
   //

   dig->info = *info;

   //
   // Assign background timer
   //

   dig->background_timer = AIL_register_timer(background_callback);

   if (dig->background_timer == -1)
      {
      AIL_set_error("Out of timer handles");
      AIL_close_input(dig);

      return 0;
      }

   //
   // Construct WAVEFORMAT structure
   //

   WAVEFORMATEX wf;

#ifdef IS_WIN32
   wf.wFormatTag      = WAVE_FORMAT_PCM;
   wf.nChannels       = (info->hardware_format & DIG_F_STEREO_MASK) ? 2  : 1;
   wf.wBitsPerSample  = (info->hardware_format & DIG_F_16BITS_MASK) ? 16 : 8;
   wf.nSamplesPerSec  = info->hardware_rate;
   wf.nBlockAlign     = wf.nChannels * (wf.wBitsPerSample / 8);
   wf.nAvgBytesPerSec = wf.nBlockAlign * wf.nSamplesPerSec;
   wf.cbSize          = 0;
#else
   wf.wf.wFormatTag      = WAVE_FORMAT_PCM;
   wf.wf.nChannels       = (info->hardware_format & DIG_F_STEREO_MASK) ? 2  : 1;
   wf.wBitsPerSample  = (info->hardware_format & DIG_F_16BITS_MASK) ? 16 : 8;
   wf.wf.nSamplesPerSec  = info->hardware_rate;
   wf.wf.nBlockAlign     = wf.wf.nChannels * (wf.wBitsPerSample / 8);
   wf.wf.nAvgBytesPerSec = wf.wf.nBlockAlign * wf.wf.nSamplesPerSec;
#endif

   //
   // Attempt to open waveIn device
   //

   OutMilesMutex();

   MMRESULT result = waveInOpen((HWAVEIN*)&dig->hWaveIn,
                                (UINT)info->device_ID,
                                (LPWAVEFORMATEX)&wf,
                     (DWORD_PTR) WIM_DATA_callback,
                                 0,
                                 CALLBACK_FUNCTION);

   InMilesMutex();

   if (result)
      {
      wsprintf(AIL_error,"waveInOpen() failed, code = 0x%X\n",result);

      dig->hWaveIn = NULL;
      AIL_close_input(dig);

      return 0;
      }

   //
   // Allocate simulated DMA buffers
   //

   dig->DMA_size = info->buffer_size;

   S32 i;

   for (i=0; i < N_WAVEIN_BUFFERS; i++)
      {
      dig->DMA[i] = AIL_mem_alloc_lock(info->buffer_size);

      if (dig->DMA[i] == NULL)
         {
         AIL_set_error("Insufficient memory for input buffer");

         AIL_close_input(dig);
         return NULL;
         }
      }

   //
   // Prepare waveIn headers
   //

   for (i=0; i < N_WAVEIN_BUFFERS; i++)
      {
      dig->wavehdr[i].lpData          = (LPSTR) dig->DMA[i];
      dig->wavehdr[i].dwBufferLength  = dig->DMA_size;
      dig->wavehdr[i].dwBytesRecorded = 0;
      dig->wavehdr[i].dwUser          = 0;
      dig->wavehdr[i].dwFlags         = 0;
      dig->wavehdr[i].dwLoops         = 0;
      dig->wavehdr[i].lpNext          = NULL;
      dig->wavehdr[i].reserved        = 0;

      result = waveInPrepareHeader(dig->hWaveIn,
                      (LPWAVEHDR) &dig->wavehdr[i],
                                   sizeof(WAVEHDR));


      if (result)
         {
         wsprintf(AIL_error,"waveInPrepareHeader() failed, code = 0x%X\n",result);

         AIL_close_input(dig);
         return NULL;
         }
      }

   //
   // Set silence byte value for current format
   //

   if (wf.wBitsPerSample == 16)
      {
      dig->silence = 0x00;
      }
   else
      {
      dig->silence = 0x80;
      }

   //
   // Flush DMA buffers
   //

   for (i=0; i < N_WAVEIN_BUFFERS; i++)
      {
      flush_DMA_buffer(dig,i);
      }

   //
   // Enable background timer
   //

   AIL_set_timer_frequency(dig->background_timer,         SERVICE_RATE);
   AIL_set_timer_user     (dig->background_timer, (UINTa) dig);
   AIL_start_timer        (dig->background_timer             );

   //
   // Return success
   //

   return dig;
}

//############################################################################
//##                                                                        ##
//## Close input device                                                     ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_close_input     (HDIGINPUT dig)
{
   S32 i;

   if (dig==0)
     return;

   if (dig->background_timer != -1)
      {
      AIL_stop_timer(dig->background_timer);
      AIL_release_timer_handle(dig->background_timer);
      dig->background_timer = -1;
      }

   if (dig->hWaveIn != NULL)
      {
      //
      // Halt wave input
      //

      AIL_set_input_state(dig, 0);

      //
      // Unprepare memory headers
      //

      for (i=0; i < N_WAVEIN_BUFFERS; i++)
         {
         waveInUnprepareHeader(dig->hWaveIn,
                  (LPWAVEHDR) &dig->wavehdr[i],
                               sizeof(WAVEHDR));
         }

      //
      // Close wave driver
      //

      waveInClose(dig->hWaveIn);
      dig->hWaveIn = NULL;
      }

   //
   // Release DMA buffer memory
   //

   for (i=0; i < N_WAVEIN_BUFFERS; i++)
      {
      if (dig->DMA[i] != NULL)
         {
         AIL_mem_free_lock(dig->DMA[i]);
         dig->DMA[i] = NULL;
         }
      }

   AIL_mem_free_lock(dig);
}

//############################################################################
//##                                                                        ##
//## Get input device info                                                  ##
//##                                                                        ##
//############################################################################

AIL_INPUT_INFO FAR * AILCALL AIL_API_get_input_info (HDIGINPUT dig)
{
   return &dig->info;
}

//############################################################################
//##                                                                        ##
//## Enable/disable input                                                   ##
//##                                                                        ##
//############################################################################

S32  AILCALL AIL_API_set_input_state (HDIGINPUT         dig, //)
                                      S32               enable)
{
   S32 i;

   if (dig->input_enabled == enable)
      {
      return dig->input_enabled;
      }

   if (enable)
      {
      //
      // Submit all buffers to waveIn API
      // 

      for (i=0; i < N_WAVEIN_BUFFERS; i++)
         {
         dig->wavehdr[i].dwBytesRecorded = 0;

         MMRESULT result = waveInAddBuffer(dig->hWaveIn, 
                          (LPWAVEHDR) &dig->wavehdr[i],
                                       sizeof(WAVEHDR));

         if (result)
            {
            wsprintf(AIL_error,"waveInAddBuffer() failed, code = 0x%X\n",result);
            return 0;
            }

         dig->wavehdr[i].dwUser |= 0x80000000;
         }

      //
      // Start recording process
      //

      MMRESULT result = waveInStart(dig->hWaveIn);

      if (result)
         {
         wsprintf(AIL_error,"waveInStart() failed, code = 0x%X\n",result);
         return 0;
         }

      //
      // Signal buffers submittable in background
      //

      dig->device_active = 1;

      //
      // Return success
      //

      dig->input_enabled = 1;

      return 1;
      }
   else
      {
      //
      // Reset wave device
      //

      dig->device_active = 0;

      waveInReset(dig->hWaveIn);

      //
      // If we've submitted any buffers, wait up to 1000 milliseconds for
      // them to be marked 'done' by the driver
      //

      U32 t = AIL_ms_count();

      while ((AIL_ms_count() - t) > 1000)
         {
#ifdef IS_WIN32
         Sleep(1);
#endif
         S32 continue_flag = FALSE;

         for (i=0; i < N_WAVEIN_BUFFERS; i++)
            {
            if ((dig->wavehdr[i].dwUser & 0x80000000) && (!(dig->wavehdr[i].dwFlags & WHDR_DONE)))
               {
               continue_flag = TRUE;
               break;
               }
            }

         if (!continue_flag)
            {
            break;
            }
         }

      //
      // Return success
      //

      dig->input_enabled = 0;

      return 1;
      }
}
