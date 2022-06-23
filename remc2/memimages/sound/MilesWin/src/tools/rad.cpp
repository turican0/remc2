//############################################################################
//##                                                                        ##
//##  RAD.CPP                                                               ##
//##                                                                        ##
//##  MSS 8 GDC demo                                                        ##
//##                                                                        ##
//##  V1.00 of 29-Jan-07: Initial version                                   ##
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

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <shlobj.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

//#include <paramon.h>
#define debug_printf

#define sprintf wsprintf
#define vsprintf wvsprintf

#include "mars.h"
#include "radres.h"

typedef S32 BOOL32;
#include "stdtpl.h"

//
// Configuration equates
// 

#define TOP_BORDER_HEIGHT    0        // leave room for transport-control toolbar
#define BOTTOM_BORDER_HEIGHT 16     
#define MIN_WINDOW_X_SIZE    200
#define MIN_WINDOW_Y_SIZE    200

#define N_DEFAULT_VIRTUAL_VOICES 4096

#define TIMER_SERVICE_TREE_MS  100
#define TIMER_SERVICE_WAVE_MS   33
#define TIMER_SERVICE_EXAMMS_MS 50
#define TIMER_SERVICE_PROP_MS   50

#define ICON_MSS 0
#define ICON_BIRD 1
#define ICON_VIOLIN 3
#define ICON_GIRL 4
#define ICON_ROOSTER 5
#define ICON_GLASS 2

//
// File menu
//

#define IDM_NEW_PROJECT   1
#define IDM_OPEN_PROJECT  2
#define IDM_CLOSE_PROJECT 3
#define IDM_SAVE_PROJECT  4
                         
#define IDM_QUIT          5
                         
//
// Help menu
//

#define IDM_USER_GUIDE    600
#define IDM_ABOUT         601

#define IDC_TREEVIEW 2000

/*********************************************************************/
//
// Global UI properties and defaults
//
/*********************************************************************/

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT Splitter_OnLButtonDown (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT Splitter_OnLButtonUp   (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT Splitter_OnMouseMove   (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

const char   szAppName[]   = "Miles Sound System 8";
const char   szChildName[] = "RADChild";
HINSTANCE    hInstance;

HMENU hmenu = NULL;
S32   menu_valid = 0;

HWND hwndMain;
HWND hwndTreeView;
HWND hwndPropCtrl;
HWND hwndExamMS;
HWND hwndWaveCtrl;
HWND hwndExam;

HICON RAD_icon;

HCURSOR HC_size_NS;
HCURSOR HC_size_WE;
HCURSOR HC_arrow;

enum PANE_BORDER
{
   BORDER_NONE,
   BORDER_12,
   BORDER_34,
   BORDER_V,
};

HCURSOR current_cursor;         
BOOL    force_cursor;      // NULL if we aren't forcing any particular cursor in WM_SETCURSOR 

C8 INI_path[MAX_PATH];

S32 client_w = 0;
S32 client_h = 0;

S32 window_x = 200;         
S32 window_y = 300;
S32 window_w = 800;
S32 window_h = 600;

S32 splitter_12  = 400;    // Border between PANE_TREE and PANE_TAGLIST
S32 splitter_34  = 300;    // Border between PANE_EDITOR and PANE_HELP
S32 splitter_v   = 400;    // Border between left and right halves of screen
S32 splitter_w = 2;        // Width of splitter border

/*********************************************************************/
//
// Prototypes
//
/*********************************************************************/

#define FE_BOLD   1
#define FE_ITALIC 2

typedef S32 FONT_EFFECTS;

static void init_logfont_descriptor(LOGFONT     *lf, 
                                    C8          *typeface_name,
                                    S32          point_size,
                                    FONT_EFFECTS effects = 0);

struct WAVEVIEW;
WAVEVIEW *global_waveform_viewer;

/*********************************************************************/
//
// Global sound properties and defaults
//
/*********************************************************************/

BOOL    AIL_trace;
HMENV   ME;          // TODO: get rid of this
HMSOUND last_triggered_sound;

struct SOUNDDEF *global_selected_SOUNDDEF;
struct ENVPROP  *global_selected_ENVPROP;
struct SNDPROP  *global_selected_SNDPROP;

void ____________________________________________________Waveform_viewer____________________________________________(){}

#define WF_MARGIN           12
#define WF_DRAG_TOLERANCE   1
#define MAX_WF_RECORD_WIDTH 1600

#define N_WF_SCALES 16
S32 WF_denom [N_WF_SCALES] = { 512, 256, 128, 64, 48, 32, 24, 16, 14, 12, 10, 8, 6, 4, 2, 1 };

S32   mouse_x;
S32   mouse_y;
S32   waveform_scale;


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

static U8 FAR* Source=0;
static UINTa SourceLength=0;
static UINTa SourcePosition=0;

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
    SourcePosition = offset;
  }

  //
  // make sure we don't read too much
  //

  if ((SourcePosition + bytesrequested) > SourceLength)
  {
    bytesrequested = (S32) (SourceLength - SourcePosition);
  }

  //
  // copy the data to the destination
  //

  memcpy( dest, &Source[SourcePosition], bytesrequested );

  //
  // update our position
  //

  SourcePosition += bytesrequested;

  //
  // return how many bytes we actually copied
  //

  return bytesrequested;
}

typedef struct ASITHREAD
{
  S16 * dest[2];
  HASISTREAM stream;
  volatile S32 running;
  volatile S32 stop;
  HANDLE thread;
  ASI_STREAM_PROCESS        ASI_stream_process;
  ASI_STREAM_CLOSE          ASI_stream_close;
  S32 expected_samples_per_channel;

  S32 waveform_channels;
  S32 *n_waveform_samples;
  F32 *bytes_per_sample;
  SINTa source_bytes;

} ASITHREAD;
  
static ASITHREAD oneasi = {0};

static void stop_thread(void)
{
   while ( oneasi.running )
   {
     oneasi.stop = 1;
     Sleep(5);
   }
}

struct WAVEVIEW
{
   HPEN   wf_trace_pen;
   HPEN   wf_cursor_pen;
   HPEN   wf_frame_pen;
   HPEN   wf_dot_frame_pen;
   HPEN   wf_loop_begin_pen;
   HPEN   wf_loop_end_pen;
   HBRUSH wf_background;
   HFONT  wf_font;

   POINT point_array [2][MAX_WF_RECORD_WIDTH];
   S32   frame_array    [MAX_WF_RECORD_WIDTH];

   S32 wf_width;
   S32 wf_height;
   S32 waveform_view_valid;

   HDC         wf_hDC;
   HBITMAP     wf_hDefaultBitmap;
   U32        *wf_lpDIBBuffer;
   BITMAPINFO *wf_pbmi;
   HBITMAP     wf_hDIB;

   S16 * waveform_buffer[2];
   S32   waveform_channels;
   F32   bytes_per_sample;
   U8  * source_addr;
   SINTa source_bytes;
   S32   output_sample_rate;
   S32   samples_per_frame;
   SINTa n_waveform_samples;
   S32   mouse_click_x;
   S32   mouse_click_y;
   SINTa mouse_click_pos;
   S32   drag_state;
   S32   drag_button;
   S32   last_drag_button;
   S32   begin_loop;
   S32   end_loop;

   S32    file_bytes;
   void  *file_data;
   U32   *file_memory_ptr;

   S32    is_ASI;
   S32    is_MP3;
   SINTa *frame_source_offsets;
   S32    n_valid_frames;

   S32 waveform_view_x;
   S32 waveform_view_y;
   
   HWND hwnd;

   static int search_frame_list(const void *keyval, const void *datum)
      {
      SINTa key = *(SINTa *) keyval;

      if (key <  ((SINTa *) datum)[0]) return -1;
      if (key >= ((SINTa *) datum)[1]) return  1;

      return 0;
      }
   
   WAVEVIEW() 
      {
      memset(this, 0, sizeof(*this));  // WARNING: No virtuals allowed

      wf_trace_pen      = 0;
      wf_cursor_pen     = 0;
      wf_frame_pen      = 0;
      wf_dot_frame_pen  = 0;
      wf_loop_begin_pen = 0;
      wf_loop_end_pen   = 0;
      wf_background     = 0;
      wf_font           = 0;

      wf_width = 0;
      wf_height = 0;
      waveform_view_valid = 0;

      wf_hDIB = NULL;

      waveform_channels = 0;
      bytes_per_sample = 0.0F;
      source_addr = NULL;
      source_bytes = 0;
      output_sample_rate = 0;
      samples_per_frame = 0;
      n_waveform_samples = 0;
      waveform_scale = 5;
      mouse_x = 0;
      mouse_y = 0;
      mouse_click_x = 0;
      mouse_click_y = 0;
      mouse_click_pos = 0;
      drag_state = FALSE;
      drag_button = 0;
      last_drag_button = 0;
      begin_loop = -1;
      end_loop = -1;

      is_ASI = 0;
      is_MP3 = 0;

      waveform_view_x = 0;
      waveform_view_y = 0;
      }

   ~WAVEVIEW()
      {
      unload_file();
      destroy_control();
      }

   void build_MP3_frame_list(void)
      {
      n_valid_frames = 0;
   
      MP3_INFO M;
   
      AIL_inspect_MP3(&M,
                       source_addr,
                 (S32) source_bytes);
   
      while (AIL_enumerate_MP3_frames(&M))
         {
         if (n_valid_frames == 0)
            {
            //
            // Most information about an MP3 file is unavailable until the first frame has been
            // encountered and parsed.  Once we've parsed the first frame, we can guess 
            // how many frames are in the file and allocate our frame-offset array
            //
   
            samples_per_frame = M.samples_per_frame;
      
            SINTa n_expected_frames = n_waveform_samples / M.samples_per_frame;
   
            frame_source_offsets = (SINTa *) AIL_mem_alloc_lock((n_expected_frames + 256) * sizeof(SINTa));
            }
   
         frame_source_offsets[n_valid_frames++] = M.byte_offset;
         }
   
      frame_source_offsets[n_valid_frames] = M.next_frame_expected;
   
      //
      // See if this MP3 file has a LAME info block (normally in its first frame)
      //
      // If so, set the default begin/end loop points to the offsets of the 
      // first and last frames in the file that contain nothing but real 
      // (non-silence-padded) audio data
      //
      // This serves as an effective first attempt at finding suitable offsets
      // to loop the entire file.  It's broken out into a separate loop to make it easy
      // to copy into your application verbatim... just pass the frame offsets (M.byte_offset values)
      // at begin_loop and end_loop to the AIL_set_sample_loop_block() or 
      // AIL_set_stream_loop_block() functions.
      //
   
      AIL_inspect_MP3(&M,
                       source_addr,
                 (S32) source_bytes);
   
      S32 sample_cnt = 0;
      S32 n_frames   = 0;
   
      while (AIL_enumerate_MP3_frames(&M))
         {
         S32 first_sample = sample_cnt;
         S32 frame_num    = n_frames;
   
         sample_cnt += M.samples_per_frame;
         ++n_frames;
   
         if ((M.Info_valid) && (M.enc_delay != -1) && (M.enc_padding != -1))
            {
            //
            // Decoder delay is 528 samples; one 1152-sample frame contains the
            // info tag itself
            //
   
            if ((begin_loop == -1) && (first_sample - 528 - 1152 > M.enc_delay))
               {
               begin_loop = frame_num;
               }
   
            if ((end_loop == -1) && (first_sample >= ((M.frame_count * M.samples_per_frame) - 528 - 1152 + M.enc_padding)))
               {
               end_loop = frame_num;
               }
            }
         }
   
      if ((end_loop == -1) && (M.Info_valid))
         {
         end_loop = M.frame_count+1;
         }
      }

   static DWORD WINAPI decompress_thread( LPVOID stuff )
      {
      ASITHREAD * at = (ASITHREAD*) stuff;
   
      S16 * dest[2];
      HASISTREAM stream;
      ASI_STREAM_PROCESS        ASI_stream_process;
      ASI_STREAM_CLOSE          ASI_stream_close;
      S32 expected_samples_per_channel;
      
      S32 len,total_samples=0;
      static S16 buffer[32768];
   
      dest[0] = at->dest[0];
      dest[1] = at->dest[1];
      stream = at->stream;
      ASI_stream_process = at->ASI_stream_process;
      ASI_stream_close = at->ASI_stream_close;
      expected_samples_per_channel = at->expected_samples_per_channel;
   
      while ((len = ASI_stream_process(stream,
                                       buffer,
                                       sizeof(buffer))) != 0)
         {
         S32 n_samples = len / sizeof(S16);
   
         if ( at->stop )
           goto error;
   
         for (S32 i=0; i < at->waveform_channels; i++)
            {
            for (S32 j=0; j < n_samples; j += at->waveform_channels)
               {
               if ((i == 0) && (total_samples++ >= expected_samples_per_channel))
                  {
                  //
                  // This must be a corrupted MP3 file (e.g., the first header specified 
                  // a lower sample rate than the rest)
                  //
                 error:
                  ASI_stream_close(stream);
                  CloseHandle(at->thread);
                  at->running = 0;
                  return 0;
                  }
   
               *dest[i]++ = buffer[j+i];
               }
            }
         }
   
      *at->n_waveform_samples = dest[0] - at->dest[0]; //waveform_buffer[0];
   
      *at->bytes_per_sample = (F32) (F64(at->source_bytes) / F64(*at->n_waveform_samples));
   
      //
      // Return success
      //
   
      ASI_stream_close(stream);
      CloseHandle(at->thread);
      at->running = 0;
//      InvalidateRect(hwnd,0,0);
      return 1;
      }


   S32 build_ASI_waveform_buffer(void  *audio_data,
                                 S32    audio_size,
                                 C8    *file_suffix)
      {
      HPROVIDER ASI;
      HASISTREAM stream;
   
      WAVEOUT FAR *wav = (WAVEOUT FAR *) audio_data;
   
      stop_thread();
   
      Source = (U8 *) audio_data;
   
      if (!memcmp(wav->wavemark, "WAVE", 4 ))
         {
         SourceLength   = wav->datalen;
         SourcePosition = (U8*)wav->data-(U8*)wav;
   
         if (wav->fmttag != WAVE_FORMAT_PCM)
            {
            return 0;
            }
   
         //
         // Find ASI decoder provider which understands the input wave type
         //
   
         ASI = RIB_find_file_dec_provider("ASI codec",
                                          "Input wave tag",
                                           wav->fmttag,
                                          "Output file types",
                                          ".RAW" );
   
         }
      else
         {
         SourceLength   = audio_size;
         SourcePosition = 0;
   
         //
         // Find ASI decoder provider which understands the suffixs
         //
   
         ASI = RIB_find_files_provider("ASI codec",
                                       "Input file types",
                                       file_suffix,
                                       "Output file types",
                                       ".RAW");
         }
   
      if (ASI == 0)
         {
         return 0;
         }
   
      ASI_ERROR                 ASI_error;
      
      ASI_STREAM_OPEN           ASI_stream_open;
      ASI_STREAM_PROCESS        ASI_stream_process;
      ASI_STREAM_SEEK           ASI_stream_seek;
      ASI_STREAM_CLOSE          ASI_stream_close;
      ASI_STREAM_PROPERTY       ASI_stream_property;
   
      HPROPERTY INPUT_BIT_RATE;
      HPROPERTY INPUT_SAMPLE_RATE;
      HPROPERTY INPUT_BITS;
      HPROPERTY INPUT_CHANNELS;
      HPROPERTY OUTPUT_BIT_RATE;
      HPROPERTY OUTPUT_SAMPLE_RATE;
      HPROPERTY OUTPUT_BITS;
      HPROPERTY OUTPUT_CHANNELS;
      HPROPERTY POSITION;
      HPROPERTY PERCENT_DONE;
      HPROPERTY MIN_INPUT_BLOCK_SIZE;
      HPROPERTY RAW_RATE;
      HPROPERTY RAW_BITS;
      HPROPERTY RAW_CHANNELS;
      HPROPERTY REQUESTED_BIT_RATE;
      HPROPERTY REQUESTED_RATE;
      HPROPERTY REQUESTED_BITS;
      HPROPERTY REQUESTED_CHANS;
      HPROPERTY REQUESTED_LAYER;
   
      RIB_INTERFACE_ENTRY ASICODEC[] =
         {
         FN( ASI_error )
         };
   
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
   
      RIB_request( ASI, "ASI codec", ASICODEC );
      RIB_request( ASI, "ASI stream", ASISTR );
   
      //
      // Open stream with codec, registering callback function
      //
   
      source_addr  = &Source[SourcePosition];
      source_bytes =  SourceLength;
   
      stream = ASI_stream_open(0,
                               Stream_CB,
                               (S32) SourceLength);
   
      if (stream == NULL)
         {
         return 0;
         }
      
      //
      // Get waveform characteristics
      //
   
      output_sample_rate = 0;
      waveform_channels  = 0;
   
      ASI_stream_property(stream, OUTPUT_SAMPLE_RATE, &output_sample_rate, 0, 0 );
      ASI_stream_property(stream, OUTPUT_CHANNELS, &waveform_channels, 0, 0 );
   
      if (waveform_channels > 2)
         {
         waveform_channels = 2;
         }
   
      //
      // Generate mono or stereo waveform data for oscilloscope display
      //
      // Note that MP3DEC.ASI emits a significant amount of silent data at 
      // the end of a file (~20K more than SF7's MP3 decoder for a 3-minute 
      // 128kbps file), so we'll add a couple megabytes' worth of padding 
      // to the buffers we allocate to receive the decompressed data.  This also
      // accounts for the fact that MP3 bitrates are not specified all that 
      // precisely.
      //
      // We assume that the uncompressed bit rate corresponds to 16-bit PCM output...
      //
    
      S32 compressed_bit_rate   = 0;
      S32 uncompressed_bit_rate = 0;
   
      ASI_stream_property(stream, INPUT_BIT_RATE,  &compressed_bit_rate,   0, 0 );
      ASI_stream_property(stream, OUTPUT_BIT_RATE, &uncompressed_bit_rate, 0, 0 );
   
      S32 expected_bytes               = (S32) (S64(source_bytes) * S64(uncompressed_bit_rate) / S64(compressed_bit_rate));
      expected_bytes += (expected_bytes / 10);     // Add 10% for extra padding margin
   
      S32 expected_bytes_per_channel   = (expected_bytes / waveform_channels) + 2560000;
      S32 expected_samples_per_channel = expected_bytes_per_channel / sizeof(S16);
   
      S32 i;
      for (i=0; i < waveform_channels; i++)
         {
         waveform_buffer[i] = NULL;
         }
   
      for (i=0; i < waveform_channels; i++)
         {
         waveform_buffer[i] = (S16 *) AIL_mem_alloc_lock(expected_bytes_per_channel);
   
         if (waveform_buffer[i] == NULL)
            {
            for (S32 j=0; j < i; j++)
               {
               AIL_mem_free_lock(waveform_buffer[j]);
               waveform_buffer[j] = NULL;
               }
   
            ASI_stream_close(stream);
            return 0;
            }
   
            memset( waveform_buffer[i], 0, expected_bytes_per_channel );
         }
   
      n_waveform_samples = expected_bytes_per_channel;
   
      bytes_per_sample = (F32) (F64(source_bytes) / F64(n_waveform_samples));
   
      oneasi.dest[0] = waveform_buffer[0];
      oneasi.dest[1] = waveform_buffer[1];
      oneasi.stream = stream;
      oneasi.ASI_stream_process = ASI_stream_process;
      oneasi.ASI_stream_close = ASI_stream_close;
      oneasi.expected_samples_per_channel = expected_samples_per_channel;
      oneasi.stop = 0;
      oneasi.running = 1;
      oneasi.waveform_channels = waveform_channels;
      oneasi.n_waveform_samples = &n_waveform_samples;
      oneasi.bytes_per_sample = &bytes_per_sample;
      oneasi.source_bytes = source_bytes;

      DWORD id;
      oneasi.thread = CreateThread(0,0,decompress_thread,&oneasi,0,&id);
      Sleep(50);
      if ( oneasi.thread == 0 )
      {
        oneasi.running = 0;
      }

      //
      // Thread uses globals, so we have to wait on it 
      // TODO
      //

      while (oneasi.running)
         {
         Sleep(5);
         }
      
      return 1;
      }

   S32 build_PCM_waveform_buffer(void *file_image)
      {
      //
      // Convert non-MP3-based PCM sample to displayable format              
      //
      // Allocate a temporary HSAMPLE so we can query the sample's properties
      // without altering MARS's sample
      //
   
      stop_thread();

      HDIGDRIVER dig;

      MARS_output_driver(ME,     
                        &dig);
   
      HSAMPLE S = AIL_allocate_sample_handle(dig); // TODO: no guarantee this will succeed,
                                                   // probably should use a separate utility HDIGDRIVER
      if (!AIL_set_sample_file(S,
                               file_image,
                              -1))
         {
         AIL_release_sample_handle(S);
         return 0;
         }
   
      source_addr        =  (U8 *) S->start[0];
      source_bytes       =         S->len[0];
      output_sample_rate =         S->playback_rate;
      waveform_channels  =        (S->format & DIG_F_STEREO_MASK) ? 2 : 1;
      bytes_per_sample   = (F32) ((S->format & DIG_F_16BITS_MASK) ? 2 : 1) * waveform_channels;
   
      S16 *dest[2];
   
      if (S->format & DIG_F_ADPCM_MASK)
         {
         //
         // ADPCM
         //
   
         AILSOUNDINFO info;
   
         if (!AIL_WAV_info(file_image,
                          &info))
            {
            AIL_release_sample_handle(S);
            return 0;
            }
                              
         void *WAV_data = NULL;
         U32   WAV_size = 0;
   
         if (!AIL_decompress_ADPCM(&info,
                                   &WAV_data,
                                   &WAV_size))
            {
            AIL_release_sample_handle(S);
            return 0;
            }
   
         HSAMPLE PCM = AIL_allocate_sample_handle(dig);
   
         if (!AIL_set_sample_file(PCM,
                                  WAV_data,
                                 -1))
            {
            AIL_release_sample_handle(PCM);
            AIL_release_sample_handle(S);
            return 0;
            }
   
         S32 PCM_channels  = (PCM->format & DIG_F_STEREO_MASK) ? 2 : 1;
   
         S32 waveform_bytes_per_channel = PCM->len[0] / PCM_channels;
   
         for (S32 i=0; i < PCM_channels; i++)
            {
            waveform_buffer[i] = (S16 *) AIL_mem_alloc_lock(waveform_bytes_per_channel);
            }
   
         dest[0] = waveform_buffer[0];
         dest[1] = waveform_buffer[1];
   
         S16 *buffer = (S16 *) PCM->start[0];
   
         S32 n_samples = PCM->len[0] / sizeof(S16);
   
         for (S32 i=0; i < PCM_channels; i++)
            {
            for (S32 j=0; j < n_samples; j += PCM_channels)
               {
               *dest[i]++ = buffer[j+i];
               }
            }
   
         bytes_per_sample = F32(source_bytes) * PCM_channels / F32(n_samples);
   
         AIL_release_sample_handle(PCM);
         AIL_mem_free_lock(WAV_data);
         }
      else if (S->format & DIG_F_MULTICHANNEL_MASK) 
         {
         //
         // Multichannel .wav
         //
   
         AIL_release_sample_handle(S);
         return 0;
         }
      else if (S->format & DIG_F_16BITS_MASK)
         {
         //
         // 16-bit PCM
         //
   
         SINTa waveform_bytes_per_channel = source_bytes / waveform_channels;
   
         for (S32 i=0; i < waveform_channels; i++)
            {
            waveform_buffer[i] = (S16 *) AIL_mem_alloc_lock(waveform_bytes_per_channel);
            }
   
         dest[0] = waveform_buffer[0];
         dest[1] = waveform_buffer[1];
   
         S16 *buffer = (S16 *) S->start[0];
   
         SINTa n_samples = source_bytes / sizeof(S16);
   
         for (S32 i=0; i < waveform_channels; i++)
            {
            for (S32 j=0; j < n_samples; j += waveform_channels)
               {
               *dest[i]++ = buffer[j+i];
               }
            }
         }
      else
         {
         //
         // 8-bit PCM
         //
   
         SINTa waveform_bytes_per_channel = (source_bytes * 2) / waveform_channels;
   
         for (S32 i=0; i < waveform_channels; i++)
            {
            waveform_buffer[i] = (S16 *) AIL_mem_alloc_lock(waveform_bytes_per_channel);
            }
   
         dest[0] = waveform_buffer[0];
         dest[1] = waveform_buffer[1];
   
         U8 *buffer = (U8 *) S->start[0];
   
         SINTa n_samples = source_bytes / sizeof(U8);
   
         for (S32 i=0; i < waveform_channels; i++)
            {
            for (S32 j=0; j < n_samples; j += waveform_channels)
               {
               *dest[i]++ = (S16(buffer[j+i]) ^ 0x80) << 8;
               }
            }
   
         }
   
      n_waveform_samples = dest[0] - waveform_buffer[0];
   
      AIL_release_sample_handle(S);
      return 1;
      }

   BOOL load_file(C8 *filename)
      {
      n_waveform_samples = 0;
      n_valid_frames     = 0;
      begin_loop = -1;
      end_loop   = -1;
      is_ASI     = FALSE;
      is_MP3     = FALSE;

      file_memory_ptr = (U32 *) AIL_file_read(filename, FILE_READ_WITH_SIZE);

      if (file_memory_ptr == NULL)
         {
         return FALSE;
         }

      file_bytes = *file_memory_ptr;
      file_data  = &(file_memory_ptr[1]);

      S32 file_type = AIL_file_type_named(file_data, filename, file_bytes);

      if (file_type == AILFILETYPE_UNKNOWN)
         {
         return FALSE;
         }

      if (file_type == AILFILETYPE_MPEG_L3_AUDIO)
         {
         is_MP3 = TRUE;
         is_ASI = TRUE;  // TODO: Ogg, others
         }

      if (is_MP3)
         {
         if (build_ASI_waveform_buffer(file_data, file_bytes, ".MP3"))
            {
            build_MP3_frame_list();
            }
         else
            {
            return FALSE;
            }
         }
      else
         {
         if (is_ASI)
            {
            C8 *suffix = strrchr(filename,'.');

            if (!build_ASI_waveform_buffer(file_data, file_bytes, suffix))
               {
               return FALSE;
               }
            }
         else  
            {
            if (!build_PCM_waveform_buffer(file_data))
               {
               return FALSE;
               }
            }
         }

      return TRUE;
      }

   void unload_file(void)
      {
      S32 i=0;
      stop_thread();

      if (file_memory_ptr != NULL)
         {
         AIL_mem_free_lock(file_memory_ptr);
         file_memory_ptr = NULL;
         }

      for (i=0; i < 2; i++)
         {
         if (waveform_buffer[i] != NULL)
            {
            AIL_mem_free_lock(waveform_buffer[i]);
            waveform_buffer[i] = NULL;
            }
         }

      if (frame_source_offsets != NULL)
         {
         AIL_mem_free_lock(frame_source_offsets);
         frame_source_offsets = NULL;
         }

      waveform_view_valid = FALSE;
      }

   void destroy_control(void)
      {
      S32 i=0;
      stop_thread();

      if (wf_hDC != NULL)
         {
         if (wf_hDefaultBitmap != NULL)
            {
            SelectBitmap(wf_hDC, wf_hDefaultBitmap);
            wf_hDefaultBitmap = NULL;
            }

         DeleteDC(wf_hDC);
         wf_hDC = NULL;
         }

      if (wf_hDIB != NULL)
         {
         DeleteObject(wf_hDIB);
         wf_hDIB = NULL;
         }

      if (wf_pbmi != NULL)
         {
         AIL_mem_free_lock(wf_pbmi);
         wf_pbmi = NULL;
         }

      if (wf_trace_pen      != NULL) { DeleteObject(wf_trace_pen);       wf_trace_pen      = NULL; }
      if (wf_cursor_pen     != NULL) { DeleteObject(wf_cursor_pen);      wf_cursor_pen     = NULL; }
      if (wf_background     != NULL) { DeleteObject(wf_background);      wf_background     = NULL; }
      if (wf_font           != NULL) { DeleteObject(wf_font);            wf_font           = NULL; }
      if (wf_frame_pen      != NULL) { DeleteObject(wf_frame_pen);       wf_frame_pen      = NULL; }
      if (wf_dot_frame_pen  != NULL) { DeleteObject(wf_dot_frame_pen);   wf_dot_frame_pen  = NULL; }
      if (wf_loop_begin_pen != NULL) { DeleteObject(wf_loop_begin_pen);  wf_loop_begin_pen = NULL; }
      if (wf_loop_end_pen   != NULL) { DeleteObject(wf_loop_end_pen);    wf_loop_end_pen   = NULL; }

      waveform_view_valid = FALSE;
      }

   void create_control(HWND _hwnd)
      {
      hwnd = _hwnd;

      RECT rect;
      GetClientRect(_hwnd, &rect);

      create_control((rect.right - rect.left) - 0,
                     (rect.bottom - rect.top) - 0);
      }

   void create_control(S32 width, S32 height)
      {
      wf_width  = width;
      wf_height = height;

      //
      // Create graphics resources used by waveform display
      //

      LOGFONT lf;
      init_logfont_descriptor(&lf,
                              "Arial",
                               7);

      wf_font = CreateFontIndirect(&lf);

      wf_trace_pen      = CreatePen(PS_SOLID, 1, RGB(0,   255,  0));
      wf_cursor_pen     = CreatePen(PS_DOT,   1, RGB(255, 255,  0));
      wf_dot_frame_pen  = CreatePen(PS_DOT,   1, RGB(0,   100,  0));
      wf_frame_pen      = CreatePen(PS_SOLID, 1, RGB(0,   100,  0));
      wf_loop_begin_pen = CreatePen(PS_SOLID, 1, RGB(0,   255,  0));
      wf_loop_end_pen   = CreatePen(PS_SOLID, 1, RGB(255,   0,  0));

      wf_background     = CreateSolidBrush(RGB(0,70,0));

      //
      // Create device-independent bitmap of specified size
      //

      wf_pbmi = (BITMAPINFO *) 
             AIL_mem_alloc_lock(sizeof(BITMAPINFOHEADER) + (sizeof (RGBQUAD) * 256));

      memset(wf_pbmi, 
             0, 
             sizeof (BITMAPINFOHEADER) + (sizeof (RGBQUAD) * 256));

      BITMAPINFOHEADER *pbmih = &(wf_pbmi->bmiHeader);

      pbmih->biSize          =  sizeof(*pbmih);
      pbmih->biWidth         =  wf_width;
      pbmih->biHeight        = -wf_height;
      pbmih->biPlanes        =  1;
      pbmih->biBitCount      =  32;
      pbmih->biSizeImage     =  0;
      pbmih->biXPelsPerMeter =  0;
      pbmih->biYPelsPerMeter =  0;
      pbmih->biClrUsed       =  0;
      pbmih->biClrImportant  =  0;
      pbmih->biCompression   =  BI_BITFIELDS;

      *(U32 *) (&(wf_pbmi->bmiColors[0])) = 0xff0000;
      *(U32 *) (&(wf_pbmi->bmiColors[1])) = 0x00ff00;
      *(U32 *) (&(wf_pbmi->bmiColors[2])) = 0x0000ff;

      wf_hDIB = CreateDIBSection(NULL,            // Device context
                                 wf_pbmi,         // BITMAPINFO structure
                                 DIB_RGB_COLORS,  // Color data type
                      (void **) &wf_lpDIBBuffer,  // Address of image map pointer
                                 NULL,            // File
                                 0);              // Bitmap file offset
      //
      // Create display DC and map it to our DIB
      //

      wf_hDC = CreateCompatibleDC(0);
      wf_hDefaultBitmap = SelectBitmap(wf_hDC, wf_hDIB);

      waveform_view_valid = TRUE;
      }

   void update_control(HSAMPLE S, //)
                       S32     wf_x0 = 0,
                       S32     wf_y0 = 0)
      {
      S32 x,ch;
      C8 text[256] = "";

      if ((!waveform_view_valid) || (waveform_buffer[0] == NULL) || (S == NULL) || (is_MP3 && (!n_valid_frames)))
         {
         return;
         }
   
      //
      // Get playback position in decompressed samples relative to start of waveform
      //
      // Because MP3 files are often full of metadata and skipped content, the equivalent output 
      // sample position can't be determined reliably with a simple bit-rate calculation.  We must 
      // binary-search the frame list to see which frame contains the playback position, 
      // and multiply the resulting frame number by 576 or 1152.
      //
      // Each MP3 frame emits samples_per_frame samples into each channel. To obtain the
      // equivalent byte offset in the output stream, sample_pos would be multiplied by 
      // sizeof(S16).
      //
      // If frame_num == n_valid_frames, it means we've already processed the last frame in 
      // the file.  We're in the codec's runoff area.
      //
   
      SINTa in_pos = (SINTa) AIL_sample_position(S);
      S32 sample_pos;
   
      SINTa frame_num = 0;
   
      if (!is_MP3)   
         {
         sample_pos = (S32) (in_pos / bytes_per_sample);
         }
      else
         {
         if (in_pos < frame_source_offsets[0])                                                      
            {
            frame_num = 0;
            }
         else if (in_pos >= frame_source_offsets[n_valid_frames])    // offset[n_valid_frames] = EOF 
            {
            frame_num = n_valid_frames;
            }
         else
            {
            SINTa *result = (SINTa *) bsearch(&in_pos, 
                                               frame_source_offsets, 
                                               n_valid_frames,
                                               sizeof(SINTa),
                                               search_frame_list);
            if (result == NULL)  
               {
               return;  
               }
   
            frame_num = result - frame_source_offsets;
            }
   
         //
         // Lerp to find actual sample position within frame
         //
         // (Normally this isn't too meaningful due to ASI position granularity, but it allows
         // us to move the cursor smoothly during manual dragging operations)
         //
   
         F32 fractional_frame = 0.0F;
   
         if (frame_num < n_valid_frames)
            {
            SINTa oa = frame_source_offsets[frame_num];
            SINTa ob = frame_source_offsets[frame_num+1];     
   
            fractional_frame = F32(in_pos-oa) / F32(ob-oa);
            }
   
         sample_pos = S32((F32(frame_num) + fractional_frame) * F32(samples_per_frame));
         }
   
      //
      // Get # of source data bytes per displayed pixel, used for dragging and non-frame-bound 
      // loop-point positioning
      //
   
      S32 samples_per_pixel = WF_denom[waveform_scale];
   
      F32 pixels_per_frame = 0.0F;
      F32 bytes_per_pixel  = 0.0F;
   
      if (is_MP3)
         {
         pixels_per_frame = F32(samples_per_frame) / F32(samples_per_pixel);
   
         F32 bytes_per_frame = F32(frame_source_offsets[min(1, n_valid_frames-1)] -
                                   frame_source_offsets[0]);
   
         bytes_per_pixel = bytes_per_frame / pixels_per_frame;
         }
      else
         {
         bytes_per_pixel = F32(samples_per_pixel) * bytes_per_sample;
         }
   
      //
      // Draw waveform background to offscreen DIB
      //
   
      S32 DIB_x0     = 0;
      S32 DIB_y0     = 0;
      S32 DIB_x1     = wf_width-1;
      S32 DIB_y1     = wf_height-1;
      S32 DIB_width  = wf_width;
      S32 DIB_height = wf_height;
   
      SetTextColor(wf_hDC, RGB(255,0,255));
      SetBkColor  (wf_hDC, RGB(0, 70, 0));
      SetBkMode   (wf_hDC, TRANSPARENT);
   
      SelectObject(wf_hDC, wf_background);
      SelectObject(wf_hDC, GetStockObject(BLACK_PEN));
      SelectObject(wf_hDC, (wf_font != 0) ? wf_font : GetStockObject(ANSI_VAR_FONT));
   
      Rectangle(wf_hDC, DIB_x0, DIB_y0, DIB_x1+1, DIB_y1+1);
   
      //
      // Cursor position = midscreen if enough sample data exists to both sides to fill window
      //
      // In MP3 mode, leave room at right edge of waveform for EOF frame marker
      //
   
      S32 visible_wf_x0    = DIB_x0 + WF_MARGIN;
      S32 visible_wf_width = DIB_width - (WF_MARGIN * 2);
   
      S32 mid_screen = S32(visible_wf_width / 2); 
      S32 cursor_x   = mid_screen;
   
      SINTa np,sp;
   
      if (is_MP3)
         {
         np = ((n_valid_frames+1) * samples_per_frame) / samples_per_pixel;
         sp = sample_pos                               / samples_per_pixel;
         }
      else
         {
         np = n_waveform_samples / samples_per_pixel;
         sp = sample_pos         / samples_per_pixel;
         }
   
      if (sp + (((visible_wf_width-1) - cursor_x)) >= np)
         {
         cursor_x = (visible_wf_width-1) - (S32) (np - sp);
         }
   
      if ((sp - cursor_x) < 0)
         {
         cursor_x = (S32) sp;
         }
   
      //
      // Fill array with waveform trace points
      // Also record MP3 frame # at each column
      //
      // Displayed waveform points are always sampled at an integer multiple of 
      // samples_per_pixel to avoid unpleasant-looking aliasing effects
      //
   
      S32 max_height = DIB_height / waveform_channels;
   
      S32 base_y  = DIB_y0 + (max_height / 2);
      S32 DIB_step = max_height;
   
      for (ch=0; ch < waveform_channels; ch++)
         {
         S32 index = ((sample_pos / samples_per_pixel) - cursor_x) * samples_per_pixel; 
   
         for (x=0; x < visible_wf_width; x++)
            {
            SINTa wf_index = index;
   
            if (wf_index >= n_waveform_samples) wf_index = n_waveform_samples-1;
            if (wf_index < 0)                   wf_index = 0;
   
            point_array[ch][x].x = x + visible_wf_x0;
            point_array[ch][x].y = ((S32(-waveform_buffer[ch][wf_index]) * max_height) / 65536) + base_y;
   
            if ((ch == 0) && is_MP3)
               {
               frame_array[x] = index / samples_per_frame;
               }
   
            index += samples_per_pixel;
            }
   
         base_y += DIB_step;
         }
   
      //
      // Keyboard/mouse UI for waveform display
      //
   
      S32 app_has_focus     = FALSE;
      S32 mouse_in_waveform = FALSE;
   
      S32 drag_button_clicked  = FALSE;
      S32 drag_button_released = FALSE; 
   
      S32 DIB_mouse_x = mouse_x - wf_x0;
      S32 DIB_mouse_y = mouse_y - wf_y0;
   
      if (GetForegroundWindow() == hwnd)
         {
         app_has_focus = TRUE;
   
         if ((DIB_mouse_x >= DIB_x0) && (DIB_mouse_x <= DIB_x1) &&
             (DIB_mouse_y >= DIB_y0) && (DIB_mouse_y <= DIB_y1))
            {
            mouse_in_waveform = TRUE;
            }
   
         last_drag_button = drag_button;
         drag_button      = ((GetKeyState(VK_LBUTTON) & 0x8000) ? 1 : 0) |
                            ((GetKeyState(VK_RBUTTON) & 0x8000) ? 2 : 0) |
                            ((GetKeyState(VK_MBUTTON) & 0x8000) ? 4 : 0);
   
         drag_button_clicked  = (drag_button) && (!last_drag_button);
         drag_button_released = (last_drag_button) && (!drag_button);
   
         if (drag_button)
            {
            if (mouse_in_waveform)
               {
               if (!last_drag_button)
                  {
                  mouse_click_x   = mouse_x;
                  mouse_click_y   = mouse_y;
                  mouse_click_pos = in_pos;
                  }
   
               if (abs(mouse_x - mouse_click_x) > WF_DRAG_TOLERANCE)
                  {
                  drag_state = TRUE;
                  }
               }
   
            if (drag_state)
               {
               SINTa new_pos = mouse_click_pos - (S32) (((mouse_x - mouse_click_x) * bytes_per_pixel));
   
               if (is_MP3) 
                  {
                  if (new_pos <  frame_source_offsets[0])              new_pos = frame_source_offsets[0];
                  if (new_pos >= frame_source_offsets[n_valid_frames]) new_pos = frame_source_offsets[n_valid_frames]-1;
                  }
               else
                  {
                  //
                  // Allow dragging to one past last sample in PCM file, cf. SoundForge
                  //
   
                  if (new_pos < 0)            new_pos = 0;
                  if (new_pos > source_bytes) new_pos = source_bytes;
                  }
   
               AIL_set_sample_position(S, (S32) new_pos);
               }
            }
   
         if (GetKeyState(VK_HOME) & 0x8000)
            {
            if (is_MP3) 
               {
               AIL_set_sample_position(S, (S32) frame_source_offsets[0]);
               }
            else
               {
               AIL_set_sample_position(S, 0);
               }
            }
         else if (GetKeyState(VK_END) & 0x8000)
            {
            if (is_MP3) 
               {
               AIL_set_sample_position(S, (S32) frame_source_offsets[n_valid_frames]-1);
               }
            else
               {
               //
               // Allow seeking to one past last sample in file, cf. SoundForge
               //
   
               AIL_set_sample_position(S, (S32) source_bytes);
               }
            }
         }
   
      //
      // Draw and hit-test frame markers
      //
                                    
      SetTextColor(wf_hDC, RGB(255,255,255));
      SetTextAlign(wf_hDC, TA_TOP + TA_CENTER + TA_NOUPDATECP);  
   
      S32 mouse_tolerance         = S32(pixels_per_frame / 2) + 1;
      S32 mouse_marker_identified = FALSE;
   
      //
      // For MP3 files, markers exist at columns where the frame number changes
      // Other file types don't have frame markers, but they do support loop markers
      //
      // (Loop points may appear only at frame markers, but may appear anywhere
      // in other digital files)
      //
   
      if (is_MP3)
         {
         //
         // ASI (MP3)
         //
   
         for (x=0; x < visible_wf_width; x++)
            {
            S32 is_new_frame = FALSE;
   
            if ((x > 0) && (frame_array[x] != frame_array[x-1]))
               {
               is_new_frame = TRUE;
               }
   
            if ((x == 0) && (frame_array[x] == 0) && (sp - cursor_x <= 0))
               {
               is_new_frame = TRUE;
               }
   
            if (!is_new_frame)
               {
               continue;
               }
   
            S32 sx = visible_wf_x0 + x;
   
            S32 mouse_over_marker = FALSE;
   
            if ((DIB_mouse_x >= sx-mouse_tolerance) && (DIB_mouse_x <= sx+mouse_tolerance) &&
                (DIB_mouse_y >= DIB_y0)             && (DIB_mouse_y <= DIB_y1))
               {
               if (!drag_state)
                  {
                  if ((AIL_sample_status(S) != SMP_PLAYING) && (!mouse_marker_identified))
                     {
                     mouse_over_marker       = TRUE;
                     mouse_marker_identified = TRUE;
                     }
                  }
               }
   
            S32 frame = frame_array[x];
   
            if (frame > n_valid_frames)
               {
               //
               // Include EOF frame marker
               //
   
               continue;
               }
   
            if ((mouse_over_marker) && (drag_button_released))
               {
               if (last_drag_button & 1)        // Left
                  {
                  if ((begin_loop == frame) || (end_loop == frame))
                     {
                     begin_loop = -1;
                     end_loop   = -1;
                     }
                  else
                     {
                     begin_loop = frame;
                     }
                  }
               else if (last_drag_button & 2)   // Right
                  {
                  if ((begin_loop == frame) || (end_loop == frame))
                     {
                     begin_loop = -1;
                     end_loop   = -1;
                     }
                  else
                     {
                     end_loop = frame;
                     }
                  }
   
               if (end_loop == begin_loop)
                  {
                  begin_loop = -1;
                  end_loop   = -1;
                  }
   
               if ((begin_loop != -1) && 
                   (end_loop   != -1) && 
                   (end_loop < begin_loop))
                  {
                  S32 temp = end_loop;
                  end_loop = begin_loop;
                  begin_loop = temp;
                  }
               }
   
            if (frame == begin_loop)
               {
               SelectObject(wf_hDC, wf_loop_begin_pen);
               }
            else if (frame == end_loop)
               {
               SelectObject(wf_hDC, wf_loop_end_pen);
               }
            else
               {
               SelectObject(wf_hDC, wf_frame_pen);
               }
   
            S32 x0 = sx;
            S32 x1 = sx;
   
            if ((frame == begin_loop) || (frame == end_loop))
               {
               x0--;
               x1++;
               }
   
            if (mouse_over_marker)
               {
               x0--;
               x1++;
               }
   
            for (S32 lx=x0; lx <= x1; lx++)
               {
               MoveToEx(wf_hDC,
                        lx,
                        DIB_y0 + 16,
                        NULL);
   
               LineTo(wf_hDC,
                      lx,
                      DIB_y1 - 16);
               }
   
            if (pixels_per_frame >= 32)
               {
               if (frame == n_valid_frames)
                  {
                  TextOut(wf_hDC, sx, DIB_y1 - 10, "EOF", 3);
                  }
               else
                  {
                  sprintf(text,"%d",(S32) frame_source_offsets[frame]);
                  TextOut(wf_hDC, sx+1, DIB_y1 - 10, text, (S32) strlen(text));
                  }
               }
            }
         }
      else
         {
         //
         // PCM/ADPCM
         //
   
         S32 column_byte      = -1;
         S32 last_column_byte = -1;
   
         for (x=0; x < visible_wf_width; x++)
            {
            S32 sx = visible_wf_x0 + x;
   
            S32 x0 = sx;
            S32 x1 = sx;
   
            //
            // Does this column represent the loop-begin or -end marker?
            //
   
            S32 is_begin_column = FALSE;
            S32 is_end_column   = FALSE;
   
            last_column_byte = column_byte;
            column_byte      = (S32) (in_pos + S32(((x - cursor_x) * samples_per_pixel * bytes_per_sample) + 0.5F));
   
            if (last_column_byte == -1)
               {
               last_column_byte = column_byte; 
               }
   
            F32 ff = ((x == 0) || (x == visible_wf_width-1)) * bytes_per_pixel;
   
            if (begin_loop != -1)
               {
               if (column_byte == begin_loop)                                           is_begin_column = TRUE;
               if ((last_column_byte-ff < begin_loop) && (column_byte+ff > begin_loop)) is_begin_column = TRUE;
               }
   
            if (end_loop != -1)
               {
               if (column_byte == end_loop)                                         is_end_column = TRUE;
               if ((last_column_byte-ff < end_loop) && (column_byte+ff > end_loop)) is_end_column = TRUE;
               }
   
            if (is_begin_column)
               {
               SelectObject(wf_hDC, wf_loop_begin_pen);
               }
   
            if (is_end_column)
               {
               SelectObject(wf_hDC, wf_loop_end_pen);
               }
   
            if (is_begin_column || is_end_column)
               {
               for (S32 lx=x0; lx <= x1; lx++)
                  {
                  MoveToEx(wf_hDC,
                           lx,
                           DIB_y0 + 16,
                           NULL);
   
                  LineTo(wf_hDC,
                         lx,
                         DIB_y1 - 16);
                  }
               }
   
            //
            // Hit-test this column
            //
   
            S32 mouse_in_column = FALSE;
   
            if ((DIB_mouse_x >= sx-mouse_tolerance) && (DIB_mouse_x <= sx+mouse_tolerance) &&
                (DIB_mouse_y >= DIB_y0)             && (DIB_mouse_y <= DIB_y1))
               {
               if (!drag_state)
                  {
                  if ((AIL_sample_status(S) != SMP_PLAYING) && (!mouse_marker_identified))
                     {
                     mouse_in_column         = TRUE;
                     mouse_marker_identified = TRUE;
                     }
                  }
               }
   
            if (mouse_in_column)
               {
               if (drag_button_released)
                  {
                  if (last_drag_button & 1)        // Left
                     {
                     if (is_begin_column || is_end_column)
                        {
                        begin_loop = -1;
                        end_loop   = -1;
                        }
                     else
                        {
                        begin_loop = column_byte;
                        }
                     }
                  else if (last_drag_button & 2)   // Right
                     {
                     if (is_begin_column || is_end_column)
                        {
                        begin_loop = -1;
                        end_loop   = -1;
                        }
                     else
                        {
                        end_loop = column_byte;
                        }
                     }
   
                  if (end_loop == begin_loop)
                     {
                     begin_loop = -1;
                     end_loop   = -1;
                     }
   
                  if ((begin_loop != -1) && 
                      (end_loop   != -1) && 
                      (end_loop < begin_loop))
                     {
                     S32 temp = end_loop;
                     end_loop = begin_loop;
                     begin_loop = temp;
                     }
                  }
   
               if (is_begin_column || is_end_column)
                  {
                  SelectObject(wf_hDC, wf_dot_frame_pen);
                  }
               else
                  {
                  SelectObject(wf_hDC, wf_frame_pen);
                  }
   
               for (S32 lx=x0; lx <= x1; lx++)
                  {
                  MoveToEx(wf_hDC,
                           lx,
                           DIB_y0 + 16,
                           NULL);
   
                  LineTo(wf_hDC,
                         lx,
                         DIB_y1 - 16);
                  }
   
               sprintf(text,"%d",column_byte);
               TextOut(wf_hDC, sx+1, DIB_y1 - 10, text, (S32) strlen(text));
               }
            }
         }
   
      //
      // End drag state *after* hit-testing the markers, so a drag-release operation
      // can be ignored above
      //
      
      if (!drag_button)
         {
         drag_state = FALSE;
         }
   
      //
      // Draw waveform trace after markers
      //
   
      SelectObject(wf_hDC, wf_trace_pen);
   
      for (ch=0; ch < waveform_channels; ch++)
         {
         Polyline(wf_hDC,
                  point_array[ch],
                  visible_wf_width);
         }
   
      //
      // Draw position cursor
      // 
   
      SelectObject(wf_hDC, wf_cursor_pen);
   
      MoveToEx(wf_hDC,
               visible_wf_x0 + cursor_x,
               DIB_y0 + 16,
               NULL);
   
      LineTo(wf_hDC,
             visible_wf_x0 + cursor_x,
             DIB_y1 - 16);
   
      sprintf(text,"%d",in_pos);
   
      SetTextAlign(wf_hDC, TA_TOP + TA_CENTER + TA_NOUPDATECP);  
      SetTextColor(wf_hDC, RGB(255,255,0));
      TextOut(wf_hDC, visible_wf_x0 + cursor_x + 1, DIB_y0 + 2, text, (S32) strlen(text));
   
      //
      // Draw display annotation
      // 
   
      sprintf(text,"1:%d",samples_per_pixel);
   
      SetTextAlign(wf_hDC, TA_TOP + TA_LEFT + TA_NOUPDATECP);  
      SetTextColor(wf_hDC, RGB(255,255,255));
      SetBkMode(wf_hDC, OPAQUE);
      TextOut(wf_hDC, DIB_x1 - 48, DIB_y0 + 2, text, (S32) strlen(text));
   
      if (begin_loop != -1)
         {
         if (is_MP3)
            {
            sprintf(text,"Start: %d",(S32) frame_source_offsets[begin_loop]);
            }
         else
            {
            sprintf(text,"Start: %d",begin_loop);
            }
   
         SetTextColor(wf_hDC, RGB(0,255,0));
         TextOut(wf_hDC, visible_wf_x0 + 8, DIB_y0 + 2, text, (S32) strlen(text));
         }
   
      if (end_loop != -1)
         {
         if (is_MP3)
            {
            sprintf(text,"End: %d",(S32) frame_source_offsets[end_loop]);
            }
         else
            {
            sprintf(text,"End: %d",end_loop);
            }
   
         SetTextColor(wf_hDC, RGB(255,0,0));
         TextOut(wf_hDC, visible_wf_x0 + 12, DIB_y0 + 12, text, (S32) strlen(text));
         }
   
      //
      // Copy waveform display to screen
      //
   
      HDC hdc = GetDC(hwnd);
   
      SetStretchBltMode(hdc, COLORONCOLOR);
   
      StretchDIBits(hdc,            // Destination DC
                    wf_x0,          // Destination X
                    wf_y0,          // Destination Y
                    wf_width,       // Destination (client area) width
                    wf_height,      // Destination (client area) height 
                    0,              // Source X
                    0,              // Source Y
                    wf_width,       // Source (back buffer) width
                    wf_height,      // Source (back buffer) height
                    wf_lpDIBBuffer, // Pointer to source (back buffer)
                    wf_pbmi,        // Bitmap info for back buffer
                    DIB_RGB_COLORS, // Bitmap contains RGB values
                    SRCCOPY);       // Do normal copy with stretching
   
      ReleaseDC(hwnd, hdc);
      }
};

/*********************************************************************/
// 
// Grievous hackery
//
/*********************************************************************/

enum TREENODETYPE
{
   //
   // Root-level entry types
   //

   TNT_PROJECT,

   //
   // Second-level entry types
   //

   TNT_TAGLIST,
   TNT_ENVSETTINGS,
   TNT_SOUNDLIST,

   //
   // Third-level entry types
   //

   TNT_ENVPROP,
   TNT_SOUNDDEF,

   //
   // Fourth-level entry types
   //

   TNT_SNDPROP,
};

struct TREENODE
{
   TREENODETYPE type;
   S32          image_num;
   C8           title[512];
   HTREEITEM    hItem;

   TREENODE() {}

   void add_to_tree(HTREEITEM hParent, BOOL has_children=TRUE)
      {
      TV_ITEM         tvItem;
      TV_INSERTSTRUCT tvInsert;

      memset(&tvItem,   0, sizeof(tvItem));
      memset(&tvInsert, 0, sizeof(tvInsert));

      //fill in the TVITEM structure for this item
      tvItem.mask = TVIF_PARAM | TVIF_TEXT | TVIF_CHILDREN;

      tvItem.mask |= TVIF_IMAGE | TVIF_SELECTEDIMAGE;

      //put the private information in the lParam
      tvItem.lParam = (LPARAM) this;

      //text and images are done on a callback basis
      tvItem.pszText = LPSTR_TEXTCALLBACK;
      tvItem.iImage = tvItem.iSelectedImage = I_IMAGECALLBACK;
      
      //assume we have children
      tvItem.cChildren = has_children;

      //fill in the TV_INSERTSTRUCT structure for this item
      tvInsert.item = tvItem;
      tvInsert.hInsertAfter = TVI_LAST;
      tvInsert.hParent = (hParent == NULL) ? TVI_ROOT : hParent;

      //add the item
      hItem = TreeView_InsertItem(hwndTreeView, &tvInsert);
      }

   void delete_from_tree(void)
      {
      }
};

struct PROPNODE : public TREENODE
{
   MARS_PROPERTY_INFO info;
};

struct TAGLIST : public TREENODE
{
   TAGLIST()
      {
      type      = TNT_TAGLIST;
      image_num = ICON_BIRD;
      strcpy(title,"Tags");
      }
};

struct ENVPROP : public PROPNODE
{
   ENVPROP *prev;
   ENVPROP *next;

   ENVPROP()
      {
      type = TNT_ENVPROP;
      image_num = ICON_BIRD;
      }
};

struct ENVSETTINGS : public PROPNODE
{
   HMENV E;

   LList<ENVPROP> properties;

   ENVSETTINGS()
      {
      type = TNT_ENVSETTINGS;
      image_num = ICON_BIRD;
      strcpy(title,"Global Environment Settings");

      //
      // Create MARS environment and add its settings to the tree view
      //

      MARS_create_environment(&E, N_DEFAULT_VIRTUAL_VOICES);   

      ME = E;  // TODO: use environment from selected tree branch

      MARS_set_output_format(E,                 // TODO: global setting from toolbar
                             MARS_51_DISCRETE, 
                             44100, 
                             64, 
                             MEV_RESTORE_SOUND_INSTANCES);
      }

   ~ENVSETTINGS()
      {
      MARS_destroy_environment(E);
      }

   void add_to_tree(HTREEITEM hParent, BOOL has_children=TRUE)
      {
      TREENODE::add_to_tree(hParent, has_children);

      //
      // Add properties to tree underneath our own entry
      //

      HMARSENUM          next = MARS_FIRST;
      MARS_PROPERTY_INFO info;

      while (MARS_enumerate_environment_properties(E, 
                                                  &next, 
                                                  &info) != MARS_DONE)
         {
         if (info.MPF_flags & (MPF_UNEDITABLE))
            {
            continue;
            }

         ENVPROP *EP = properties.alloc();

         EP->info = info;
         EP->add_to_tree(hItem, FALSE);

         sprintf(EP->title,"%s",info.name);
         }
      }

};

struct SNDPROP : public PROPNODE
{
   SNDPROP *prev;
   SNDPROP *next;

   SNDDEF *D;

   SNDPROP()
      {
      type = TNT_SNDPROP;
      image_num = ICON_MSS;
      }
};

struct SOUNDDEF : public TREENODE
{
   SOUNDDEF *prev;
   SOUNDDEF *next;

   HMSOUND D;
   WAVEVIEW viewer;

   LList<SNDPROP> properties;

   SOUNDDEF()
      {
      D = NULL;
      type = TNT_SOUNDDEF;
      image_num = ICON_MSS;
      }

  ~SOUNDDEF()
      {
      global_waveform_viewer = NULL;
      }

   void create(C8 *filename)
      {
      sprintf(title,"%s",filename);

      MARS_create_sound  (ME, NULL, &D);   
      MARS_set_sound_file(D, filename);
      MARS_preload       (D, TRUE);

      viewer.load_file(filename);
      }

   void play(void)
      {
      MARS_play(D);

      last_triggered_sound = D;

      global_waveform_viewer = &viewer;

      global_waveform_viewer->destroy_control();
      global_waveform_viewer->create_control(hwndWaveCtrl);
      }

   void add_to_tree(HTREEITEM hParent, BOOL has_children=TRUE)
      {
      TREENODE::add_to_tree(hParent, has_children);

      //
      // Add properties to tree underneath our own entry
      //

      HMARSENUM          next = MARS_FIRST;
      MARS_PROPERTY_INFO info;

      while (MARS_enumerate_sound_properties(D, 
                                            &next, 
                                            &info) != MARS_DONE)
         {
         if (info.MPF_flags & (MPF_UNEDITABLE))
            {
            continue;
            }

         SNDPROP *SP = properties.alloc();

         SP->D = D;
         SP->info = info;
         SP->add_to_tree(hItem, FALSE);
         SP->image_num = ICON_GLASS;

         sprintf(SP->title,"%s",info.name);
         }
      }
};

struct SOUNDLIST : public TREENODE
{
   LList<SOUNDDEF> defs;

   SOUNDLIST()
      {
      type = TNT_SOUNDLIST;
      image_num = ICON_VIOLIN;
      strcpy(title,"Sound Definitions");
      }

   void add_to_tree(HTREEITEM hParent, BOOL has_children=TRUE)
      {
      TREENODE::add_to_tree(hParent, has_children);

      //
      // Add some test sounds 
      //

      SOUNDDEF *SD = defs.alloc();
      SD->create("welcome.wav");
      SD->add_to_tree(hItem, TRUE);
      SD->image_num = ICON_GIRL;

      SD = defs.alloc();
      SD->create("ogres.mp3");
      SD->add_to_tree(hItem, TRUE);
      SD->image_num = ICON_ROOSTER;

      SD = defs.alloc();
      SD->create("shot.wav");
      SD->add_to_tree(hItem, TRUE);
      SD->image_num = ICON_GLASS;

      SD = defs.alloc();
      SD->create("music.mp3");
      SD->add_to_tree(hItem, TRUE);
      SD->image_num = ICON_VIOLIN;

      SD = defs.alloc();
      SD->create("DawnBrotherhood.mp3");
      SD->add_to_tree(hItem, TRUE);
      SD->image_num = ICON_VIOLIN;

      SD = defs.alloc();
      SD->create("ShadowRealm.mp3");
      SD->add_to_tree(hItem, TRUE);
      SD->image_num = ICON_VIOLIN;

      TreeView_Expand(hwndTreeView, hItem, TVE_EXPAND);
      }
};

struct PROJECT : public TREENODE
{
   PROJECT *prev;
   PROJECT *next;

   //
   // Contents
   //

   ENVSETTINGS *ENV;
   SOUNDLIST   *SLS;
   TAGLIST     *TLS;

   //
   // Methods
   //

   PROJECT()
      {
      type = TNT_PROJECT;
      strcpy(title,"New Project");
      image_num = 0;

      //
      // Add root-level project to tree view
      //

      add_to_tree(NULL, TRUE);

      //
      // Create outer categories and add them to tree view
      //

      ENV = new ENVSETTINGS;
      SLS = new SOUNDLIST;
      TLS = new TAGLIST;

      ENV->add_to_tree(hItem, TRUE);
      SLS->add_to_tree(hItem, TRUE);
      TLS->add_to_tree(hItem, FALSE);

      TreeView_Expand(hwndTreeView, hItem, TVE_EXPAND);
      }

   ~PROJECT()
      {
      delete ENV;
      delete SLS;
      delete TLS;
      }
};

LList<PROJECT> projects;

void ____________________________________________________Utility_routines___________________________________________(){}

/*********************************************************************/
//
// init_logfont_descriptor
//
/*********************************************************************/

static void init_logfont_descriptor(LOGFONT     *lf, //)
                                    C8          *typeface_name,
                                    S32          point_size,
                                    FONT_EFFECTS effects)
{
   CHARSETINFO csi;

   UINTa curSysCp = GetACP();

   S32 result = TranslateCharsetInfo((DWORD *) curSysCp, &csi, TCI_SRCCODEPAGE);

   memset(lf, 0, sizeof(LOGFONT));

   HDC hSDC = GetDC(NULL);
   lf->lfHeight = -MulDiv(point_size, GetDeviceCaps(hSDC, LOGPIXELSY), 72);
   ReleaseDC(NULL, hSDC);

   lf->lfWidth         = 0;
   lf->lfEscapement    = 0;
   lf->lfOrientation   = 0;
   lf->lfUnderline     = FALSE;
   lf->lfStrikeOut     = FALSE;
   lf->lfCharSet       = csi.ciCharset;
   lf->lfOutPrecision  = OUT_DEFAULT_PRECIS;
   lf->lfClipPrecision = CLIP_DEFAULT_PRECIS;
   lf->lfQuality       = PROOF_QUALITY;
   
   lf->lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

   strcpy(lf->lfFaceName, typeface_name);

   lf->lfWeight =       (effects & FE_BOLD)   ? FW_BOLD : FW_NORMAL;
   lf->lfItalic = (S8) ((effects & FE_ITALIC) ? TRUE    : FALSE);
}

/*********************************************************************/
//
// Error diagnostics
//
/*********************************************************************/

void __cdecl alert_box(C8 *caption, C8 *fmt, ...)
{
   static char work_string[4096];

   if ((fmt == NULL) || (strlen(fmt) > sizeof(work_string)))
      {
      strcpy(work_string, "(String missing or too large)");
      }
   else
      {
      va_list ap;

      va_start(ap, 
               fmt);

      vsprintf(work_string, 
               fmt, 
               ap);

      va_end  (ap);
      }

   if (caption == NULL)
      {
      MessageBox(hwndMain, 
                 work_string,
                "RAD Error", 
                 MB_OK);
      }
   else
      {
      MessageBox(hwndMain, 
                 work_string,
                 caption,
                 MB_OK);
      }
}

void __cdecl show_last_error(C8 *caption = NULL, ...)
{
   LPVOID lpMsgBuf; 

   FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
                 hwndMain,
                 GetLastError(), 
                 0,
        (LPTSTR) &lpMsgBuf, 
                 0, 
                 NULL);

   if (caption == NULL)
      {
      caption = "Error";
      }
   else
      {
      static char work_string[4096];

      va_list ap;

      va_start(ap, 
               caption);

      vsprintf(work_string, 
               caption, 
               ap);

      va_end  (ap);

      caption = work_string;
      }

   MessageBox(NULL, 
    (LPCTSTR) lpMsgBuf, 
              caption,
              MB_OK | MB_ICONINFORMATION); 

   LocalFree(lpMsgBuf);
}

//********************************************************************/
//
// AIL_trace_callback()
//
//********************************************************************/

void AILCALLBACK AIL_trace_callback(C8 *text,
                                    S32 nest_depth)
{
   if (!AIL_trace)
      {
      return;
      }

   if (nest_depth == 1)
      {
      printf("%s",text);
      }
}

//********************************************************************/
//
// set_current_cursor()
//
//********************************************************************/

void set_current_cursor(HCURSOR hc, //)   
                        BOOL    enforce_during_pumps = FALSE)
{
   current_cursor = hc;
   force_cursor   = enforce_during_pumps;

   SetCursor(hc);

   SetClassLongPtr(hwndMain,
                   GCLP_HCURSOR,
        (LONG_PTR) hc);
}

//********************************************************************/
//
// draw_XOR_bar()
//
//********************************************************************/

void draw_XOR_bar(HWND hwnd, int x, int y, int width, int height)
{
   //
   // GetWindowDC() is window-relative, not client-relative, so the
   // client coords must be translated into window space.  We need to 
   // use a window-relative DC to draw the XOR'ed splitter handle atop the 
   // child windows
   //

   POINT pt; 
   pt.x = x; 
   pt.y = y;

   RECT rect;
   GetWindowRect(hwnd, &rect);
   ClientToScreen(hwnd, &pt);

   x = pt.x - rect.left;
   y = pt.y - rect.top;

   static WORD _dotPatternBmp[8] = 
      { 
      0x00aa, 0x0055, 0x00aa, 0x0055, 
      0x00aa, 0x0055, 0x00aa, 0x0055
      };

   HBITMAP hbm;
   HBRUSH  hbr, hbrushOld;
   HDC     hdc = GetWindowDC(hwnd);

   hbm = CreateBitmap(8, 8, 1, 1, _dotPatternBmp);
   hbr = CreatePatternBrush(hbm);
   
   SetBrushOrgEx(hdc, x, y, 0);
   hbrushOld = (HBRUSH)SelectObject(hdc, hbr);
   
   PatBlt(hdc, x, y, width, height, PATINVERT);
   
   SelectObject(hdc, hbrushOld);

   ReleaseDC(hwnd, hdc);
   DeleteObject(hbr);
   DeleteObject(hbm);
}

//********************************************************************/
//
// Set expected RAD.INI path and try to read the file
//
/*********************************************************************/

S32 INI_read(void)
{
   GetModuleFileName(NULL, INI_path, sizeof(INI_path)-1);

   _strlwr(INI_path);

   C8 *exe = strstr(INI_path,"rad.exe");

   if (exe == NULL)
      {
      exe = INI_path;
      }

   strcpy(exe,"rad.ini");

   FILE *options = fopen(INI_path,"rt");

   if (options == NULL)
      {
      alert_box("Warning","Could not find RAD.INI");
      }
   else
      {
      while (1)
         {
         //
         // Read input line
         //

         C8 linbuf[512];

         memset(linbuf,
                0,
                sizeof(linbuf));

         C8 *result = fgets(linbuf, 
                            sizeof(linbuf) - 1, 
                            options);

         if ((result == NULL) || (feof(options)))
            {
            break;
            }

         //
         // Skip blank lines, and kill trailing and leading spaces
         //

         S32 l = strlen(linbuf);

         if ((!l) || (linbuf[0] == ';'))
            {
            continue;
            }

         C8 *lexeme  = linbuf;
         C8 *end     = linbuf;
         S32 leading = 1;

         for (S32 i=0; i < l; i++)
            {
            if (!isspace(linbuf[i]))
               {
               if (leading)
                  {
                  lexeme = &linbuf[i];
                  leading = 0;
                  }

               end = &linbuf[i];
               }
            }

         end[1] = 0;

         if ((leading) || (!strlen(lexeme)))
            {
            continue;
            }

         //
         // Terminate key substring at first space
         //

         C8 *value = strchr(lexeme,' ');

         if (value == NULL)
            {
            value = strchr(lexeme,'\t');
            }

         if (value == NULL)
            {
            continue;
            }

         *value = 0;
         value++;

         //
         // Look for key/value pairs
         // Skip spaces and tabs between key and value
         //
         
         while (1)
            {
            if ((*value == ' ') || (*value == '\t'))
               {
               ++value;
               }
            else
               {
               break;
               }
            }

         if (*value == 0)
            {
            continue;
            }

         if      (!_stricmp(lexeme,"window_x"))    window_x = atoi(value);
         else if (!_stricmp(lexeme,"window_y"))    window_y = atoi(value);
         else if (!_stricmp(lexeme,"window_w"))    window_w = atoi(value);
         else if (!_stricmp(lexeme,"window_h"))    window_h = atoi(value);
         else if (!_stricmp(lexeme,"splitter_12")) splitter_12 = atoi(value);
         else if (!_stricmp(lexeme,"splitter_34")) splitter_34 = atoi(value);
         else if (!_stricmp(lexeme,"splitter_v"))  splitter_v  = atoi(value);
         else alert_box("Warning","Unknown RAD.INI entry '%s'\n",lexeme);
         }

      fclose(options);
      }

   return TRUE;
}

//****************************************************************************
//
// Update .INI file with changed user options, where applicable
//
//****************************************************************************

void INI_update(void)
{
   FILE *read_options = fopen(INI_path,"rt");

   if (read_options == NULL)
      {
      return;
      }

   C8 *write_filename = tmpnam(NULL);

   if (!write_filename[0])
      {
      fclose(read_options);
      return;
      }

   FILE *write_options = fopen(write_filename,"wt");

   if (write_options == NULL)
      {
      fclose(read_options);
      return;
      }

   while (1)
      {
      //
      // Read input line and make a copy
      //

      C8 linbuf[512];

      memset(linbuf,
             0,
             sizeof(linbuf));

      C8 *result = fgets(linbuf, 
                         sizeof(linbuf) - 1, 
                         read_options);

      if ((result == NULL) || (feof(read_options)))
         {
         break;
         }

      C8 original_line[512];
      strcpy(original_line, linbuf);

      //
      // Skip blank lines, and kill trailing and leading spaces
      //

      S32 l = strlen(linbuf);

      if ((!l) || (linbuf[0] == ';'))
         {
         if (fputs(original_line, write_options) == EOF)
            {
            _fcloseall();
            return;
            }

         continue;
         }

      C8 *lexeme  = linbuf;
      C8 *end     = linbuf;
      S32 leading = 1;

      for (S32 i=0; i < l; i++)
         {
         if (!isspace(linbuf[i]))
            {
            if (leading)
               {
               lexeme = &linbuf[i];
               leading = 0;
               }

            end = &linbuf[i];
            }
         }

      end[1] = 0;

      if ((leading) || (!strlen(lexeme)))
         {
         if (fputs(original_line, write_options) == EOF)
            {
            _fcloseall();
            return;
            }

         continue;
         }

      //
      // Terminate key substring at first space
      //

      C8 *value = strchr(lexeme,' ');

      if (value == NULL)
         {
         value = strchr(lexeme,'\t');
         }

      if (value != NULL)
         {
         *value = 0;
         }

      //
      // Write modified values for recognized parameters, passing unrecognized 
      // or inapplicable ones to the new file unchanged
      //

      C8 modified_line[512];
      modified_line[0] = 0;

      if      (!_stricmp(lexeme,"window_x"))    sprintf(modified_line,"window_x %d\n",   window_x);
      else if (!_stricmp(lexeme,"window_y"))    sprintf(modified_line,"window_y %d\n",   window_y);
      else if (!_stricmp(lexeme,"window_w"))    sprintf(modified_line,"window_w %d\n",   window_w);
      else if (!_stricmp(lexeme,"window_h"))    sprintf(modified_line,"window_h %d\n",   window_h);
      else if (!_stricmp(lexeme,"splitter_12")) sprintf(modified_line,"splitter_12 %d\n",splitter_12);
      else if (!_stricmp(lexeme,"splitter_34")) sprintf(modified_line,"splitter_34 %d\n",splitter_34);
      else if (!_stricmp(lexeme,"splitter_v"))  sprintf(modified_line,"splitter_v %d\n", splitter_v);

      if (modified_line[0])
         {
         if (fputs(modified_line, write_options) == EOF)
            {
            _fcloseall();
            return;
            }
         }
      else
         {
         if (fputs(original_line, write_options) == EOF)
            {
            _fcloseall();
            return;
            }
         }
      }

   //
   // Overwrite original .INI file with our modified copy
   //

   _fcloseall();

   CopyFile(write_filename,
            INI_path,
            FALSE);

   DeleteFile(write_filename);
}

//****************************************************************************
//*                                                                          *
//*  Update checked items, etc. in main menu                                 *
//*                                                                          *
//****************************************************************************

void main_menu_update(void)
{
//   CheckMenuItem(hmenu, IDM_HIGH_CONTRAST,  high_contrast  ? MF_CHECKED : MF_UNCHECKED);
}

//****************************************************************************
//*                                                                          *
//*  Initialize main menu                                                    *
//*                                                                          *
//****************************************************************************

void main_menu_init(void)
{
   if (hmenu != NULL)
      {
      SetMenu(hwndMain,NULL);
      DestroyMenu(hmenu);
      }

   hmenu = CreateMenu();

   HMENU pop = CreateMenu();

   AppendMenu(pop, MF_STRING, IDM_NEW_PROJECT,     "&New project... \t l");
   AppendMenu(pop, MF_STRING, IDM_OPEN_PROJECT,         "&Open existing project... \t o");
   AppendMenu(pop, MF_STRING, IDM_CLOSE_PROJECT,  "&Close project");
   AppendMenu(pop, MF_SEPARATOR, 0, NULL);
   AppendMenu(pop, MF_STRING, IDM_CLOSE_PROJECT,  "&Save project... \t s");
   AppendMenu(pop, MF_SEPARATOR, 0, NULL);
   AppendMenu(pop, MF_STRING, IDM_QUIT,         "Quit \t q");

   AppendMenu(hmenu, MF_POPUP, (UINT) pop,      "&File");

   pop = CreateMenu();

   AppendMenu(pop, MF_STRING, IDM_USER_GUIDE,   "&User guide \t F1");
   AppendMenu(pop, MF_SEPARATOR, 0, NULL);
   AppendMenu(pop, MF_STRING, IDM_ABOUT,        "&About RAD");

   AppendMenu(hmenu, MF_POPUP, (UINT) pop,      "&Help");

   SetMenu(hwndMain,hmenu);

   main_menu_update();
   DrawMenuBar(hwndMain);

   menu_valid = TRUE;
}

void global_WM_KEYDOWN (WPARAM wParam)
{
   switch (wParam)
      
      {
      case 'T':
         {
         AIL_trace = !AIL_trace;
         debug_printf("AIL trace %s\n",AIL_trace ? "on" : "off");
         break;
         }

      case 'M': { MARS_set_output_format(ME, MARS_MONO,        44100, 64, MEV_RESTORE_SOUND_INSTANCES); break; }
      case 'S': { MARS_set_output_format(ME, MARS_STEREO,      44100, 64, MEV_RESTORE_SOUND_INSTANCES); break; } 
      case 'Y': { MARS_set_output_format(ME, MARS_51_DISCRETE, 44100, 64, MEV_RESTORE_SOUND_INSTANCES); break; }

      case 'D':
         waveform_scale--;
         if (waveform_scale < 0)            waveform_scale = 0;
         if (waveform_scale >= N_WF_SCALES) waveform_scale = N_WF_SCALES-1;
         break;
         

      case 'U':
         waveform_scale++;
         if (waveform_scale < 0)            waveform_scale = 0;
         if (waveform_scale >= N_WF_SCALES) waveform_scale = N_WF_SCALES-1;
         break;


//      case 'P': 
//         {
//         MARS_play(global_selected_SOUNDDEF->D);
//         break;
//         }

      case 'Q':
         {
         MARS_save_environment_file(ME, "test.txt", 0);
         break;
         }

      case 27:
         SetFocus(hwndMain);
         PostQuitMessage(0);
         break;

//      case ' ':
//         {
//         HMARSENUM next_sound = MARS_FIRST;
//         SNDDEF *D = NULL;
//         
//         while (MARS_enumerate_sounds(ME,
//                                       NULL,
//                                       &next_sound,
//                                       &D) != MARS_DONE)
//            {
//            MARS_end(D);
//            }
//         break;
//         }
//
//      case 'P':
//         {
//         if (hwndTreeView != NULL)
//            {
//            TVITEM tvItem;
//            memset(&tvItem, 0, sizeof(tvItem));
//
//            tvItem.hItem = TreeView_GetSelection(hwndTreeView); 
//            tvItem.mask  = TVIF_PARAM;
//
//            if (TreeView_GetItem(hwndTreeView, &tvItem))
//               {
//               TREENODE *T = (TREENODE *) tvItem.lParam;
//
//               switch (T->type)
//                  {
//                  case TNT_SOUNDDEF:
//                     {
//                     SOUNDDEF *SND = (SOUNDDEF *) T;
//
//                     SND->play();
//                     break;
//                     }
//                  }
//               }
//            }
//
//         break;
//         }
      }
}

void ____________________________________________________Tree______________________________________________________(){}
BOOL Tree_GetRootItems(HWND);
BOOL Tree_GetChildItems(HWND, HTREEITEM);
void Tree_EnumObjects(HWND, HTREEITEM, LPSHELLFOLDER, LPITEMIDLIST);
int CALLBACK Tree_CompareProc(LPARAM, LPARAM, LPARAM);

/******************************************************************************

   Tree_Create

******************************************************************************/

HWND Tree_Create(HINSTANCE hInstance, HWND hwndParent)
{
   //
   // Create tree
   //

   DWORD dwStyle;

   dwStyle =  WS_TABSTOP     | 
              WS_CHILD       | 
              WS_VISIBLE     | 
              TVS_LINESATROOT |
              TVS_HASLINES   | 
              WS_BORDER      |
              TVS_HASBUTTONS | 
              TVS_SHOWSELALWAYS |
              TVS_NOTOOLTIPS |
              0;

   HWND hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,
                              WC_TREEVIEW,
                              NULL,
                              dwStyle,
                              0,
                              0,
                              0,
                              0,
                              hwndParent,
                      (HMENU) IDC_TREEVIEW,
                              hInstance,
                              NULL);

   if (!hwnd) 
      {
      show_last_error("Tree_Create");
      return NULL;
      }

   //
   // Create utility image list
   //

   HIMAGELIST himl = ImageList_Create(16, 
                                      16,
                                      ILC_COLOR24,
                                      256,
                                      0);
   if (himl == NULL)
      {
      return 0;
      }

   //
   // Add MSS icon
   //

   ImageList_AddIcon(himl, RAD_icon);

   //
   // Add temp hack icons
   //

   for (S32 i=0; i < 5; i++)
      {
      HICON hi = ExtractIcon(hInstance,"mars.icl",i); 
      ImageList_AddIcon(himl, hi);
      }

   if (ImageList_GetImageCount(himl) < 6)
      {
      return 0;
      }

   TreeView_SetImageList(hwnd, himl, 0);

   return hwnd;
}

/******************************************************************************

   Tree_Init

******************************************************************************/

void Tree_Init(HWND hwnd)
{  
   TreeView_DeleteAllItems(hwndTreeView);

//   Tree_GetRootItems(hwnd);

   TreeView_SetScrollTime(hwndTreeView, 100);
}

/**************************************************************************

   Tree_Notify()

**************************************************************************/

LRESULT Tree_Notify(HWND hWnd, LPARAM lParam)
{
   LPNMHDR  lpnmh = (LPNMHDR) lParam;

   switch (lpnmh->code)
      {
      case TVN_KEYDOWN:
         {
         NMTVKEYDOWN *ptvkd = (LPNMTVKEYDOWN) lParam;

//         global_WM_KEYDOWN(ptvkd->wVKey);

//         switch (ptvkd->wVKey)
//            {
//            }
         return TRUE;
         break;
         }

      case TVN_GETDISPINFO:
         {
         LPNMTVDISPINFO lpdi = (LPNMTVDISPINFO) lParam;

         TREENODE *TN  = (TREENODE *) lpdi->item.lParam;

         PROJECT  *PRJ    = NULL;
         SOUNDDEF *SND    = NULL;
         TAGLIST  *TLS    = NULL;
         ENVSETTINGS *ENV = NULL;
         SOUNDLIST *SLS   = NULL;
         ENVPROP *EPR     = NULL;
         SNDPROP *SPR     = NULL;

         switch (TN->type)
            {
            case TNT_PROJECT:       PRJ = (PROJECT *)     TN; break;
            case TNT_SOUNDDEF:      SND = (SOUNDDEF *)    TN; break;
            case TNT_TAGLIST:       TLS = (TAGLIST *)     TN; break;
            case TNT_ENVSETTINGS:   ENV = (ENVSETTINGS *) TN; break;
            case TNT_SOUNDLIST:     SLS = (SOUNDLIST *)   TN; break;
            case TNT_ENVPROP:       EPR = (ENVPROP *)     TN; break;
            case TNT_SNDPROP:       SPR = (SNDPROP *)     TN; break;
            default: assert(0);
            }

         // do we need to supply the text?

         if (lpdi->item.mask & TVIF_TEXT)
            {
            switch (TN->type)
               {
               case TNT_PROJECT: 
               case TNT_ENVSETTINGS:
               case TNT_SOUNDLIST:
               case TNT_TAGLIST:
               case TNT_SOUNDDEF:
               case TNT_ENVPROP:
                  {
                  C8 text[MARS_MAX_PROP_TEXT_LEN];

                  if (TN->type == TNT_ENVPROP)
                     {
                     if (MARS_environment_property_text(ME,
                                                        TN->title,
                                                        text,
                                                        NULL,
                                                        NULL) != MARS_OK)
                        {
                        sprintf(lpdi->item.pszText, "%s (Unknown)  ", TN->title);
                        break;
                        }
                     else
                        {
                        sprintf(lpdi->item.pszText, "%s (%s)  ", TN->title, text);
                        }
                     }
                  else
                     {
                     sprintf(lpdi->item.pszText, "%s  ", TN->title);
                     }
                  break;
                  } 

               case TNT_SNDPROP:
                  {
                  C8 text[MARS_MAX_PROP_TEXT_LEN];

                  if (MARS_sound_property_text(SPR->D,
                                               TN->title,
                                               text,
                                               NULL,
                                               NULL) != MARS_OK)
                     {
                     sprintf(lpdi->item.pszText, "%s (Unknown)  ", TN->title);
                     break;
                     }

                  sprintf(lpdi->item.pszText, "%s (%s)  ", TN->title, text);
                  break;
                  }

               default:
                  {
                  sprintf(lpdi->item.pszText,"Bogus item 0x%X",lpdi->item.lParam);
                  break;
                  }
               }
            }

         // do we need to supply the unselected image?

         if (lpdi->item.mask & TVIF_IMAGE)
            {
            lpdi->item.iImage = TN->image_num;
            }

         // do we need to supply the selected image?

         if (lpdi->item.mask & TVIF_SELECTEDIMAGE)
            {
            lpdi->item.iSelectedImage = TN->image_num;
            }

         break;
         }

   case TVN_SELCHANGED:
      {
      LPNMTREEVIEW   pnmtv = (LPNMTREEVIEW) lParam;

      TREENODE *TN  = (TREENODE *) pnmtv->itemNew.lParam;

      PROJECT  *PRJ    = NULL;
      SOUNDDEF *SND    = NULL;
      TAGLIST  *TLS    = NULL;
      ENVSETTINGS *ENV = NULL;
      SOUNDLIST *SLS   = NULL;
      ENVPROP *EPR     = NULL;
      SNDPROP *SPR     = NULL;

      switch (TN->type)
         {
         case TNT_PROJECT:       PRJ = (PROJECT *)     TN; break;
         case TNT_SOUNDDEF:      SND = (SOUNDDEF *)    TN; break;
         case TNT_TAGLIST:       TLS = (TAGLIST *)     TN; break;
         case TNT_ENVSETTINGS:   ENV = (ENVSETTINGS *) TN; break;
         case TNT_SOUNDLIST:     SLS = (SOUNDLIST *)   TN; break;
         case TNT_ENVPROP:       EPR = (ENVPROP *)     TN; break;
         case TNT_SNDPROP:       SPR = (SNDPROP *)     TN; break;
         default: assert(0);
         }

      global_selected_SOUNDDEF = NULL;
      global_selected_ENVPROP  = NULL;
      global_selected_SNDPROP  = NULL;

           if (SND != NULL) global_selected_SOUNDDEF = SND;
      else if (EPR != NULL) global_selected_ENVPROP  = EPR;
      else if (SPR != NULL) global_selected_SNDPROP  = SPR;

      break;
      }

      case TVN_ITEMEXPANDING:
         {
         LPNMTREEVIEW pnmtv = (LPNMTREEVIEW) lParam;
      
         switch (pnmtv->action)
            {
            case TVE_EXPAND:
               {
//             Tree_GetChildItems(hwndTreeView, pnmtv->itemNew.hItem);

//               TREENODE *TN  = (TREENODE *) pnmtv->itemNew.lParam;
//
//               PROJECT  *PRJ    = NULL;
//               SOUNDDEF *SND    = NULL;
//               TAGLIST  *TLS    = NULL;
//               ENVSETTINGS *ENV = NULL;
//               SOUNDLIST *SLS   = NULL;
//               ENVPROP *EPR     = NULL;
//               SNDPROP *SPR     = NULL;
//
//               switch (TN->type)
//                  {
//                  case TNT_PROJECT:       PRJ = (PROJECT *)     TN; break;
//                  case TNT_SOUNDDEF:      SND = (SOUNDDEF *)    TN; break;
//                  case TNT_TAGLIST:       TLS = (TAGLIST *)     TN; break;
//                  case TNT_ENVSETTINGS:   ENV = (ENVSETTINGS *) TN; break;
//                  case TNT_SOUNDLIST:     SLS = (SOUNDLIST *)   TN; break;
//                  case TNT_ENVPROP:       EPR = (ENVPROP *)     TN; break;
//                  case TNT_SNDPROP:       SPR = (SNDPROP *)     TN; break;
//                  default: assert(0);
//                  }

               break;
               }
         
            case TVE_COLLAPSE:
               TreeView_Expand(hwndTreeView, pnmtv->itemNew.hItem, TVE_COLLAPSE); // | TVE_COLLAPSERESET);
               break;
            }
         break;
         }

//      case TVN_DELETEITEM:
//         {
//         LPNMTREEVIEW   pnmtv = (LPNMTREEVIEW)lParam;
//         LPITEMINFO     pItem = (LPITEMINFO)pnmtv->itemOld.lParam;
//         LPMALLOC       pMalloc;
//
//         if (SUCCEEDED(SHGetMalloc(&pMalloc)))
//         {
//         //free up the pidls that we allocated
//         Pidl_Free(pItem->pidlFQ);
//         Pidl_Free(pItem->pidlRel);
//
//         pMalloc->Release();
//         }
//      
//      //this may be NULL if this is the root item
//      if(pItem->pParentFolder)
//         pItem->pParentFolder->Release();
//      
//      GlobalFree((HGLOBAL)pItem);
//      }
//      break;
   
//   case TVN_SELCHANGED:
//      {
//      LPNMTREEVIEW   pnmtv = (LPNMTREEVIEW)lParam;
//      LPITEMINFO pItem = (LPITEMINFO)pnmtv->itemNew.lParam;
//
//      List_DisplayFolder(GetDlgItem(hWnd, IDC_LISTVIEW), pItem);
//      }
//      break;
      }

   return 0;
}

/**************************************************************************

   Tree_GetRootItems

**************************************************************************/

BOOL Tree_GetRootItems(HWND hwnd)
{
   TV_ITEM           tvItem;
   TV_INSERTSTRUCT   tvInsert;
   HTREEITEM         hParentItem;

   memset(&tvItem, 0, sizeof(tvItem));
   memset(&tvInsert, 0, sizeof(tvInsert));

   tvItem.mask = TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN;

   //put the private information in the lParam
   tvItem.lParam = 0;

   //text and images are done on a callback basis
   tvItem.pszText = LPSTR_TEXTCALLBACK;
   tvItem.iImage = tvItem.iSelectedImage = I_IMAGECALLBACK;
   
   //assume the root has children
   tvItem.cChildren = TRUE;

   //fill in the TV_INSERTSTRUCT structure for this item
   tvInsert.item = tvItem;
   tvInsert.hInsertAfter = TVI_LAST;
   tvInsert.hParent = TVI_ROOT;

   //add the item
   HTREEITEM hRootItem = TreeView_InsertItem(hwnd, &tvInsert);

   for (S32 i=0; i < 12; i++)
      {
      //fill in the TVITEM structure for this item
      tvItem.mask = TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN;

      //put the private information in the lParam
      tvItem.lParam = i+1;

      //text and images are done on a callback basis
      tvItem.pszText = LPSTR_TEXTCALLBACK;
      tvItem.iImage = tvItem.iSelectedImage = I_IMAGECALLBACK;
      
      //assume the desktop has children
      tvItem.cChildren = TRUE;

      //fill in the TV_INSERTSTRUCT structure for this item
      tvInsert.item = tvItem;
      tvInsert.hInsertAfter = TVI_LAST;
      tvInsert.hParent = hRootItem;

      //add the item
      hParentItem = TreeView_InsertItem(hwnd, &tvInsert);

      // create 4 children for each root item

      for (S32 j=0; j < 4; j++)
         {
         tvItem.mask = TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN;

         //put the private information in the lParam
         tvItem.lParam = j+1;

         //text and images are done on a callback basis
         tvItem.pszText = LPSTR_TEXTCALLBACK;
         tvItem.iImage = tvItem.iSelectedImage = I_IMAGECALLBACK;
         
         //assume this level has no children
         tvItem.cChildren = FALSE;

         //fill in the TV_INSERTSTRUCT structure for this item
         tvInsert.item = tvItem;
         tvInsert.hInsertAfter = TVI_LAST;
         tvInsert.hParent = hParentItem;

         //add the item
         TreeView_InsertItem(hwnd, &tvInsert);
         }

      //go ahead and expand root item
      }

   TreeView_Expand(hwnd, hRootItem, TVE_EXPAND);

   return TRUE;
}

/**************************************************************************

   Tree_GetChildItems

**************************************************************************/

BOOL Tree_GetChildItems(HWND hwnd, HTREEITEM hParentItem)
{
   return FALSE;
//   TVITEM         tvItem;
//   LPITEMINFO     pItem;
//   HRESULT        hr;
//   HCURSOR        hCursor;
//   TV_SORTCB      tvSort;
//
//   //get the parent item's pidl
//   ZeroMemory(&tvItem, sizeof(tvItem));
//   tvItem.mask = TVIF_PARAM;
//   tvItem.hItem = hParentItem;
//
//   if(!TreeView_GetItem(hwnd, &tvItem))
//   return FALSE;
//
////change the cursor
//hCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
//
////turn redawing off in the TreeView. This will speed things up as we add items
//SendMessage(hwnd, WM_SETREDRAW, FALSE, 0);
//
//pItem = (LPITEMINFO)tvItem.lParam;
//
////if the parent folder is NULL, then we are at the root of the namespace, so the parent of this item is the desktop folder
//if(!pItem->pParentFolder)
//   {
//   hr = SHGetDesktopFolder(&pParentFolder);
//   }
////otherwise we need to get the IShellFolder for this item
//else
//   {
//   hr = pItem->pParentFolder->BindToObject(pItem->pidlRel, NULL, IID_IShellFolder, (LPVOID*)&pParentFolder);
//   }
//
//if(FAILED(hr))
//   return FALSE;
//
//Tree_EnumObjects(hwnd, hParentItem, pParentFolder, pItem->pidlFQ);
//
////sort the new items
//tvSort.hParent = hParentItem;
//tvSort.lpfnCompare = Tree_CompareProc;
//tvSort.lParam = 0;
//TreeView_SortChildrenCB(hwnd, &tvSort, 0);
//
////turn redawing back on in the TreeView
//SendMessage(hwnd, WM_SETREDRAW, TRUE, 0);
////UpdateWindow(hwnd);
//
//pParentFolder->Release();
//
//SetCursor(hCursor);
//
//return TRUE;
}

/**************************************************************************

   Tree_EnumObjects

**************************************************************************/

void Tree_EnumObjects(  HWND hwnd, 
                        HTREEITEM hParentItem, 
                        LPSHELLFOLDER pParentFolder, 
                        LPITEMIDLIST pidlParent)
{
//   LPENUMIDLIST   pEnum;
//   
//   if (SUCCEEDED(pParentFolder->EnumObjects(NULL, SHCONTF_FOLDERS, &pEnum)))
//   {
//   LPITEMIDLIST   pidlTemp;
//   DWORD          dwFetched = 1;
//   LPITEMINFO     pItem;
//
//   //enumerate the item's PIDLs
//   while(S_OK == (pEnum->Next(1, &pidlTemp, &dwFetched)) && dwFetched)
//      {
//      TVITEM         tvItem;
//      TVINSERTSTRUCT tvInsert;
//      DWORD          dwAttribs;
//
//      ZeroMemory(&tvItem, sizeof(tvItem));
//      
//      //fill in the TV_ITEM structure for this item
//      tvItem.mask = TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN;
//
//      //AddRef the parent folder so it's pointer stays valid
//      pParentFolder->AddRef();
//
//      //put the private information in the lParam
//      pItem = (LPITEMINFO)GlobalAlloc(GPTR, sizeof(ITEMINFO));
//      
//      pItem->pidlRel = pidlTemp;
//      pItem->pidlFQ = Pidl_Concatenate(pidlParent, pidlTemp);
//      
//      pItem->pParentFolder = pParentFolder;
//      tvItem.lParam = (LPARAM)pItem;
//
//      //text and images are done on a callback basis
//      tvItem.pszText = LPSTR_TEXTCALLBACK;
//      tvItem.iImage = tvItem.iSelectedImage = I_IMAGECALLBACK;
//
//      //determine if the item has children
//      dwAttribs = SFGAO_HASSUBFOLDER | SFGAO_FOLDER | SFGAO_DISPLAYATTRMASK | SFGAO_CANRENAME;
//      pParentFolder->GetAttributesOf(1, (LPCITEMIDLIST*)&pidlTemp, &dwAttribs);
//      tvItem.cChildren = (dwAttribs & SFGAO_HASSUBFOLDER);
//
//      //determine if the item is shared
//      if(dwAttribs & SFGAO_SHARE)
//         {
//         tvItem.mask |= TVIF_STATE;
//         tvItem.stateMask |= TVIS_OVERLAYMASK;
//         tvItem.state |= INDEXTOOVERLAYMASK(1); //1 is the index for the shared overlay image
//         }
//      
//      //fill in the TV_INSERTSTRUCT structure for this item
//      tvInsert.item = tvItem;
//      tvInsert.hInsertAfter = TVI_LAST;
//      tvInsert.hParent = hParentItem;
//
//      TreeView_InsertItem(hwnd, &tvInsert);
//
//      dwFetched = 0;
//      }
//
//   pEnum->Release();
//   }
}

///**************************************************************************
//
//   Tree_CompareProc
//
//**************************************************************************/
//
//int CALLBACK Tree_CompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
//{
//UNREFERENCED_PARAMETER(lParamSort);
//
//LPITEMINFO  pItem1 = (LPITEMINFO)lParam1;
//LPITEMINFO  pItem2 = (LPITEMINFO)lParam2;
//HRESULT     hr;
//
//hr = pItem1->pParentFolder->CompareIDs(   0,
//                                          pItem1->pidlRel,
//                                          pItem2->pidlRel);
//
//if(FAILED(hr))
//   return 0;
//
//return (short)SCODE_CODE(GetScode(hr));
//}

/**************************************************************************

   Tree_DoItemMenu

**************************************************************************/

//void Tree_DoItemMenu(HWND hwnd, HTREEITEM hItem, LPPOINT pptScreen)
//{
//TVITEM   tvItem;
//
//ZeroMemory(&tvItem, sizeof(tvItem));
//tvItem.mask = TVIF_PARAM;
//tvItem.hItem = hItem;
//
//if(TreeView_GetItem(hwnd, &tvItem))
//   {
//   HWND           hwndParent = GetParent(hwnd);
//   HRESULT        hr;
//   LPITEMINFO     pInfo = (LPITEMINFO)tvItem.lParam;
//   IContextMenu   *pcm;
//   IShellFolder   *psfFolder = pInfo->pParentFolder;
//
//   if(!psfFolder)
//      {
//      SHGetDesktopFolder(&psfFolder);
//      }
//   else
//      {
//      psfFolder->AddRef();
//      }
//
//   if(psfFolder)
//      {
//      hr = psfFolder->GetUIObjectOf(   hwndParent, 
//                                       1, 
//                                       (LPCITEMIDLIST*)&pInfo->pidlRel, 
//                                       IID_IContextMenu, 
//                                       NULL, 
//                                       (LPVOID*)&pcm);
//
//      if(SUCCEEDED(hr))
//         {
//         HMENU hPopup;
//
//         hPopup = CreatePopupMenu();
//         if(hPopup)
//            {
//            hr = pcm->QueryContextMenu(hPopup, 0, 1, 0x7fff, CMF_NORMAL | CMF_EXPLORE);
//
//            if(SUCCEEDED(hr))
//               {
//               pcm->QueryInterface(IID_IContextMenu2, (LPVOID*)&g_pcm2);
//
//               UINT  idCmd;
//
//               idCmd = TrackPopupMenu( hPopup, 
//                                       TPM_LEFTALIGN | TPM_RETURNCMD | TPM_RIGHTBUTTON, 
//                                       pptScreen->x, 
//                                       pptScreen->y,
//                                       0,
//                                       hwndParent,
//                                       NULL);
//            
//               if(g_pcm2)
//                  {
//                  g_pcm2->Release();
//                  g_pcm2 = NULL;
//                  }
//
//               if(idCmd)
//                  {
//                  CMINVOKECOMMANDINFO  cmi;
//                  cmi.cbSize = sizeof(CMINVOKECOMMANDINFO);
//                  cmi.fMask = 0;
//                  cmi.hwnd = hwndParent;
//                  cmi.lpVerb = (LPCSTR)(INT_PTR)(idCmd - 1);
//                  cmi.lpParameters = NULL;
//                  cmi.lpDirectory = NULL;
//                  cmi.nShow = SW_SHOWNORMAL;
//                  cmi.dwHotKey = 0;
//                  cmi.hIcon = NULL;
//                  hr = pcm->InvokeCommand(&cmi);
//                  }
//               }
//            }
//      
//         pcm->Release();
//         }
//
//      psfFolder->Release();
//      }
//   }
//}

///**************************************************************************
//
//   Tree_DoContextMenu
//
//**************************************************************************/
//
//void Tree_DoContextMenu(HWND hwnd, LPPOINT pptScreen)
//{
//TVHITTESTINFO  tvhti;
//
//tvhti.pt = *pptScreen;
//ScreenToClient(hwnd, &tvhti.pt);
//
//tvhti.flags = LVHT_NOWHERE;
//
//TreeView_HitTest(hwnd, &tvhti);
//
//if(TVHT_ONITEM & tvhti.flags)
//   {
//   Tree_DoItemMenu(hwnd, tvhti.hItem , pptScreen);
//   }
//else
//   {
//   //the tree doesn't support background menus
//   }
//}

void ____________________________________________________EXAMMS_code_______________________________________________(){}

#define MAX_3D_SOUNDS   4

#define EPSILON       0.0001F
#define PI            3.14159265358979F
#define FASTSPEED    (400.0F / 3600.0F)  // (400 km/hour)
#define SLOWSPEED    (40.0F / 3600.0F)   // (40 km/hour)
#define CIRCLERADIUS  100.0F
#define STRAIGHTDIST  200.0F
#define CIRCLE       (2.0F * PI)

#define SOUNDSIZE     4     
#define LISTENERSIZE  12
#define CRATESIZE     12
#define WALLHEIGHT    48
#define WALLWIDTH     8

#define CLIENT_LISTENER_X       447
#define CLIENT_LISTENER_Y       162
#define CLIENT_LISTENER_RANGE_X 125
#define CLIENT_LISTENER_RANGE_Y 157

//
// Combo box entries
//

struct COMBO_ENTRY
{
   C8         *name;   
   MARS_OUTPUT_FORMAT channel_spec;
};

COMBO_ENTRY combo_entries[] = 
{
   { "Use Windows speaker configuration", MARS_USE_SYSTEM_CONFIG   },
   { "Mono",                              MARS_MONO                },
   { "Stereo headphones",                 MARS_HEADPHONES          },
   { "Stereo speakers",                   MARS_STEREO              },
   { "Dolby Surround",                    MARS_DOLBY_SURROUND      },
   { "SRS Circle Surround",               MARS_SRS_CIRCLE_SURROUND },
   { "4.0 discrete",                      MARS_40_DISCRETE         },
   { "5.1 discrete",                      MARS_51_DISCRETE         },
   { "6.1 discrete",                      MARS_61_DISCRETE         },
   { "7.1 discrete",                      MARS_71_DISCRETE         },
   { "8.1 discrete",                      MARS_81_DISCRETE         }
 /*  { "DirectSound 3D hardware support",   MARS_DIRECTSOUND3D       },
   { "Creative Labs EAX 2 (TM)",          MARS_EAX2                },
   { "Creative Labs EAX 3 (TM)",          MARS_EAX3                },
   { "Creative Labs EAX 4 (TM)",          MARS_EAX4                } */,
};

//
// Display environment
//

typedef enum SNDMOVE
{
   AUTO_ROTATE,
   AUTO_BACK_AND_FORTH,
   MANUAL_SOURCE_PLACEMENT,
   MANUAL_LISTENER_ORIENTATION,
   INVALID,
};

struct ENV
{
   HWND    env;
   RECT    envrect;

   S32     envwidth;
   S32     soundadjx;
   S32     soundadjy;
   HBRUSH  old;
   HBRUSH  listbrush;
   HBRUSH  cratebrush;
   HBRUSH  wallbrush;
   HBRUSH  soundbrush[MAX_3D_SOUNDS];
   HBITMAP cbm;
   HDC     envdc;

   BOOL    obstruction_crates;
   BOOL    occlusion_walls;
   BOOL    looped;

   BOOL    auto_3D_atten;
   BOOL    use_cone;

   SNDMOVE movement_type;
   F32     requested_falloff;
   S32     requested_room_type;

   F32     listener_bearing_x;
   F32     listener_bearing_y;
   F32     listener_bearing_z;

   S32     pitch_slider;
   F32     speed_slider;
   F32     wet_slider;
   F32     dry_slider;
};

//
// Driver-related configuration state
//

struct CFG
{
   BOOL reverb_supported;
};

//
// Control state for a given 3D sound
//

struct SND3D
{
   //
   // File image copied into sample slot when slot is played
   //

//   void *loaded_sample_address;
//   S32   loaded_sample_file_size;
//   C8    loaded_sample_filename[MAX_PATH];

   //
   // HSAMPLE assigned to slot, if any
   //

//   HSAMPLE S;

   SOUNDDEF *SND;

   //
   // Maintain the sound state separately from the HSAMPLE so the HSAMPLE can be
   // reallocated on the fly
   //

   BOOL triggered;
   BOOL playing;

   S32  base_sample_rate;

   S32  loop_count;
   S32  play_cursor;

   F32 x;
   F32 y;
   F32 z;
   F32 face_x;
   F32 face_y;
   F32 face_z;

   F32 inner_cone_angle;
   F32 outer_cone_angle;
   F32 outer_cone_vol;

   F32     offset;
   F32     cone_axis;
   SNDMOVE movement_type;

   S32     playback_rate;

   F32     occlusion;
   F32     obstruction;
   F32     wet;
   F32     dry;
   BOOL    auto_3D_wet_atten;

   BOOL    use_cone;
   BOOL    looped;

   F32     dx_dt;
   F32     dy_dt;
   F32     dz_dt;

   //
   // Housekeeping
   //

   S32 last_time_ms;
};

//
// Colors for sound brushes
//

U32 sound_colors[MAX_3D_SOUNDS] = { RGB(0x6a,0x52,0x00),
                                    RGB(0xf0,0x00,0x00), 
                                    RGB(0xfe,0xad,0x0c),
                                    RGB(0xff,0xf7,0x21) };

//
// Sound/environment globals
// 

SND3D sounds[MAX_3D_SOUNDS];

ENV *E = NULL;
CFG *current_configuration = NULL;

HWND combo;

void       *loaded_sample_address = NULL;
S32         loaded_sample_file_size = 0;
C8          loaded_sample_filename[MAX_PATH];

//############################################################################
//#                                                                          #
//#  Enable or disable a dialog window control                               #
//#                                                                          #
//############################################################################

static void UI_enable_control(WORD control, BOOL state)
{
   EnableWindow(GetDlgItem(hwndExamMS,control), state);
}

//############################################################################
//#                                                                          #
//#  Check or uncheck a dialog window control                                #
//#                                                                          #
//############################################################################

static void UI_check_control(WORD control, BOOL state)
{
   Button_SetCheck(GetDlgItem(hwndExamMS,control), state );
}

//############################################################################
//#                                                                          #
//#  Enable controls common to all configurations                            #
//#                                                                          #
//############################################################################

static void UI_enable_base_controls(void)
{
   UI_enable_control(sldSpeed,     TRUE);
   UI_enable_control(sldPitch,     TRUE);
   UI_enable_control(txtPitch,     TRUE);
   UI_enable_control(radAroundx,   TRUE);
   UI_enable_control(radBackForth, TRUE);
   UI_enable_control(radManualRot, TRUE);
   UI_enable_control(radManualLst, TRUE);
   UI_enable_control(btnStop,      TRUE);
   UI_enable_control(btnPlay,      TRUE);
   UI_enable_control(radFalloff,   TRUE);
   UI_enable_control(radF1,        TRUE);
   UI_enable_control(radF2,        TRUE);
   UI_enable_control(radF3,        TRUE);
   UI_enable_control(radF4,        TRUE);
   UI_enable_control(chkCone,      TRUE);
   UI_enable_control(chkOcclude,   TRUE);
   UI_enable_control(chkObstruct,  TRUE);
   UI_enable_control(chkLoop,      TRUE);
   UI_enable_control(txtSpeed,     TRUE);
   UI_enable_control(txtCurrent,   TRUE);
}

//############################################################################
//#                                                                          #
//#  Disable all controls except configuration selector                      #
//#                                                                          #
//############################################################################

static void UI_disable_all_controls(void)
{
   UI_enable_control(sldSpeed,     FALSE);
   UI_enable_control(sldPitch,     FALSE);
   UI_enable_control(txtPitch,     FALSE);
   UI_enable_control(radAroundx,   FALSE);
   UI_enable_control(radBackForth, FALSE);
   UI_enable_control(radManualRot, FALSE);
   UI_enable_control(radManualLst, FALSE);
   UI_enable_control(radNone,      FALSE);
   UI_enable_control(radBath,      FALSE);
   UI_enable_control(radMountains, FALSE);
   UI_enable_control(radHallway,   FALSE);
   UI_enable_control(btnStop,      FALSE);
   UI_enable_control(btnPlay,      FALSE);
   UI_enable_control(radFalloff,   FALSE);
   UI_enable_control(radF1,        FALSE);
   UI_enable_control(radF2,        FALSE);
   UI_enable_control(radF3,        FALSE);
   UI_enable_control(radF4,        FALSE);
   UI_enable_control(chkCone,      FALSE);
   UI_enable_control(chkOcclude,   FALSE);
   UI_enable_control(chkObstruct,  FALSE);
   UI_enable_control(chkLoop,      FALSE);
   UI_enable_control(txtDry,       FALSE);
   UI_enable_control(txtWet,       FALSE);
   UI_enable_control(sldDry,       FALSE);
   UI_enable_control(sldWet,       FALSE);
   UI_enable_control(chk3DAtten,   FALSE);
   UI_enable_control(txtSpeed,     FALSE);
   UI_enable_control(txtRevEnv,    FALSE);
   UI_enable_control(txtCurrent,   FALSE);
   UI_enable_control(txtRevMix,    FALSE);
}

//############################################################################
//#                                                                          #
//# Set control startup defaults                                             #
//#                                                                          #
//############################################################################

static void UI_set_control_defaults()
{
   HWND slider = GetDlgItem(hwndExamMS,sldSpeed);
   SendMessage(slider, TBM_SETRANGE, TRUE, MAKELONG( 0, 128 ) );
   SendMessage(slider, TBM_SETPOS, TRUE, 64 );

   slider = GetDlgItem(hwndExamMS,sldSpeed);
   SendMessage(slider, TBM_SETRANGE, TRUE, MAKELONG( 0, 128 ) );
   SendMessage(slider, TBM_SETPOS, TRUE, 64 );

   slider = GetDlgItem(hwndExamMS,sldWet);
   SendMessage(slider, TBM_SETRANGE, TRUE, MAKELONG( 0, 128 ) );
   SendMessage(slider, TBM_SETPOS, TRUE, 128 );

   slider = GetDlgItem(hwndExamMS,sldDry);
   SendMessage(slider, TBM_SETRANGE, TRUE, MAKELONG( 0, 128 ) );
   SendMessage(slider, TBM_SETPOS, TRUE, 128 );

   slider = GetDlgItem(hwndExamMS,sldPitch);
   SendMessage(slider, TBM_SETRANGE, TRUE, MAKELONG( 0, 128 ) );
   SendMessage(slider, TBM_SETPOS, TRUE, 64 );

   UI_check_control(radF1,0);
   UI_check_control(radF2,0);
   UI_check_control(radF3,1);
   UI_check_control(radF4,0);

   UI_check_control(radNone,1);
   UI_check_control(radMountains,0);
   UI_check_control(radBath,0);
   UI_check_control(radHallway,0);

   UI_check_control(radAroundx,1);
   UI_check_control(radNone,1);

   UI_check_control(chkLoop, 1);
   UI_check_control(chk3DAtten, 1);
}

//############################################################################
//#                                                                          #
//#  Read a slider's position                                                #
//#                                                                          #
//############################################################################

static S32 UI_get_slider_value(S32 slider_ID)
{
   HWND slider = GetDlgItem(hwndExamMS,slider_ID);

   return (S32) SendMessage(slider,TBM_GETPOS,0,0);
}

//############################################################################
//#                                                                          #
//# Create environment for display/control of demo objects                   #
//#                                                                          #
//############################################################################

ENV *ENV_create(void)
{
   ENV *E = (ENV *) AIL_mem_alloc_lock(sizeof(ENV));

   if (E == NULL)
      {
      return NULL;
      }

   memset(E,
          0,
          sizeof(ENV));

   E->env = GetDlgItem(hwndExamMS, txtCurrent);
   GetWindowRect(E->env, &E->envrect);

   E->envrect.right  -= (E->envrect.left + 12);
   E->envrect.bottom -= (E->envrect.top  + 10);
   E->envrect.left    = 12;
   E->envrect.top     = 20;

   E->envwidth  = E->envrect.right - E->envrect.left - (SOUNDSIZE * 7);
   E->soundadjx = 12 + (E->envwidth / 2) + (SOUNDSIZE * 3);
   E->soundadjy = 20 + ((E->envrect.bottom - E->envrect.top - (SOUNDSIZE * 4)) / 2) + ((SOUNDSIZE * 3) / 2);

   HDC dc = GetDC(E->env);

   E->envdc = CreateCompatibleDC(dc);

   E->cbm = CreateCompatibleBitmap(dc,               
                                   E->envrect.right, 
                                   E->envrect.bottom);

   SelectObject(E->envdc, E->cbm);

   ReleaseDC(E->env, dc);

   E->listbrush  = CreateSolidBrush(RGB(255,128,128));
   E->cratebrush = CreateSolidBrush(RGB(208,208,0));
   E->wallbrush  = CreateSolidBrush(RGB(228,0,64));

   for (S32 i=0; i < MAX_3D_SOUNDS; i++)
      {
      E->soundbrush[i] = CreateSolidBrush(sound_colors[i]);
      }

   E->movement_type = AUTO_ROTATE;

   E->listener_bearing_x = 0.0F;
   E->listener_bearing_y = 0.0F;
   E->listener_bearing_z = 1.0F;

   return E;
}

//############################################################################
//#                                                                          #
//# Destroy renderable environment                                           #
//#                                                                          #
//############################################################################

void ENV_destroy(ENV *E)
{
   if (E->envdc != NULL)
      {
      DeleteDC(E->envdc);
      E->envdc = NULL;   
      }

   if (E->cbm != NULL)
      {
      DeleteObject(E->cbm);
      E->cbm = NULL;   
      }

   if (E->listbrush != NULL)
      {
      DeleteObject(E->listbrush);
      E->listbrush = NULL;
      }

   if (E->cratebrush != NULL)
      {
      DeleteObject(E->cratebrush);
      E->cratebrush = NULL;
      }

   if (E->wallbrush != NULL)
      {
      DeleteObject(E->wallbrush);
      E->wallbrush = NULL;
      }

   for (S32 i=0; i < MAX_3D_SOUNDS; i++)
      {
      if (E->soundbrush[i] != NULL)
         {
         DeleteObject(E->soundbrush[i]);
         E->soundbrush[i] = NULL;
         }
      }
}

//############################################################################
//#                                                                          #
//# Begin rendering an environment                                           #
//#                                                                          #
//############################################################################

void ENV_begin_frame(ENV *E)
{
   //
   // Read UI controls for this frame
   //

   E->dry_slider   = ((F32) UI_get_slider_value(sldDry))   / 128.0F;  
   E->wet_slider   = ((F32) UI_get_slider_value(sldWet))   / 128.0F;
   E->speed_slider = ((F32) UI_get_slider_value(sldSpeed)) / 128.0F;  
   E->pitch_slider =        UI_get_slider_value(sldPitch);

   E->obstruction_crates = (SendMessage(GetDlgItem(hwndExamMS,chkObstruct), BM_GETCHECK, 0, 0) == 0) ? FALSE : TRUE;
   E->occlusion_walls    = (SendMessage(GetDlgItem(hwndExamMS,chkOcclude),  BM_GETCHECK, 0, 0) == 0) ? FALSE : TRUE;
   E->looped             = (SendMessage(GetDlgItem(hwndExamMS,chkLoop),     BM_GETCHECK, 0, 0) == 0) ? FALSE : TRUE;   
   E->use_cone           = (SendMessage(GetDlgItem(hwndExamMS,chkCone),     BM_GETCHECK, 0, 0) == 0) ? FALSE : TRUE;   
   E->auto_3D_atten      = (SendMessage(GetDlgItem(hwndExamMS,chk3DAtten),  BM_GETCHECK, 0, 0) == 0) ? FALSE : TRUE;  

   if (SendMessage(GetDlgItem(hwndExamMS,radAroundx), BM_GETCHECK,0,0) != 0)
      {
      E->movement_type = AUTO_ROTATE;
      }
   else if (SendMessage(GetDlgItem(hwndExamMS,radBackForth), BM_GETCHECK,0,0) != 0)
      {
      E->movement_type = AUTO_BACK_AND_FORTH;
      }
   else if (SendMessage(GetDlgItem(hwndExamMS,radManualRot), BM_GETCHECK,0,0) != 0)
      {
      E->movement_type = MANUAL_SOURCE_PLACEMENT;
      }
   else
      {
      E->movement_type = MANUAL_LISTENER_ORIENTATION;
      }

   if (SendMessage(GetDlgItem(hwndExamMS,radF1), BM_GETCHECK,0,0) != 0)
      {
      E->requested_falloff = 1.0F;
      }
   else if (SendMessage(GetDlgItem(hwndExamMS,radF2), BM_GETCHECK,0,0) != 0)
      {
      E->requested_falloff = 2.0F;
      }
   else if (SendMessage(GetDlgItem(hwndExamMS,radF3), BM_GETCHECK,0,0) != 0)
      {
      E->requested_falloff = 3.0F;
      }
   else
      {
      E->requested_falloff = 4.0F;
      }

   if (SendMessage(GetDlgItem(hwndExamMS,radBath), BM_GETCHECK,0,0) != 0)
      {
      E->requested_room_type = ENVIRONMENT_BATHROOM;
      }
   else if (SendMessage(GetDlgItem(hwndExamMS,radMountains), BM_GETCHECK,0,0) != 0)
      {
      E->requested_room_type = ENVIRONMENT_MOUNTAINS;
      }
   else if (SendMessage(GetDlgItem(hwndExamMS,radHallway), BM_GETCHECK,0,0) != 0)
      {
      E->requested_room_type = ENVIRONMENT_HALLWAY;
      }
   else
      {
      E->requested_room_type = ENVIRONMENT_GENERIC;
      }

   //
   // Allow user to change the listener's facing direction
   //

   if (E->movement_type == MANUAL_LISTENER_ORIENTATION)
      {
      //
      // Check for left-click in environment window, and get its bearing to the
      // listener
      //

      if ((GetForegroundWindow() == hwndMain) &&
          (GetKeyState(VK_LBUTTON) & 0x8000))
         {
         POINT screen_ul;
         RECT rect;
         GetClientRect(hwndExamMS, &rect);
         screen_ul.x = rect.left; 
         screen_ul.y = rect.top; 
         ClientToScreen(hwndExamMS, &screen_ul); 

         POINT screen_cursor;
         GetCursorPos(&screen_cursor);  

         F32 x = F32((screen_cursor.x - screen_ul.x) - CLIENT_LISTENER_X);
         F32 y = F32((screen_cursor.y - screen_ul.y) - CLIENT_LISTENER_Y);

         if ((x >= -CLIENT_LISTENER_RANGE_X) && (x < CLIENT_LISTENER_RANGE_X) &&
             (y >= -CLIENT_LISTENER_RANGE_Y) && (y < CLIENT_LISTENER_RANGE_Y))
            {
            //
            // We have a valid bearing -- use it to generate a facing vector
            // (which does not have to be normalized before passing to MSS)
            //
            // Screen X/Y in left-handed coords = +X/-Z
            //

            E->listener_bearing_x =  x;
            E->listener_bearing_y =  0.0F;
            E->listener_bearing_z = -y;
            }
         }
      }

   //
   // Set the listener's facing direction
   //
   // The listener's up-vector in this example is always = +Y
   //

   MSSVECTOR3D up   = {0.0F, 1.0F, 0.0F};
   MSSVECTOR3D face = {E->listener_bearing_x, E->listener_bearing_y, E->listener_bearing_z};

   MARS_environment_property(ME,
                             ENV_V_LISTENER_FACE,
                             NULL,
                            &face,
                             NULL,
                             NULL,
                             NULL,
                             NULL);

   MARS_environment_property(ME,
                             ENV_V_LISTENER_UP,
                             NULL,
                            &up,
                             NULL,
                             NULL,
                             NULL,
                             NULL);


   //
   // Set up to begin rendering
   // 
   
   E->old = (HBRUSH) SelectObject(E->envdc, 
                                  E->listbrush);

   FillRect(E->envdc,
           &E->envrect,
           (HBRUSH) (COLOR_WINDOW+1));
}

//############################################################################
//#                                                                          #
//# End environment render                                                   #
//#                                                                          #
//############################################################################

void ENV_end_frame(ENV *E)
{
   SelectObject(E->envdc, 
                E->old);

   HDC dc = GetDC(E->env);

   if (dc)
      {
      //
      // Report total CPU usage
      //

      C8 text[MARS_MAX_PROP_TEXT_LEN];

      if (MARS_environment_property_text(ME,
                                         ENV_N_CPU_PERCENT,
                                         text,
                                         NULL,
                                         NULL) == MARS_OK)
         {
         static C8 profile[32];

         wsprintf(profile, "CPU: %s%%", text);

         SelectObject(dc, GetStockObject(ANSI_VAR_FONT));

         TextOut(E->envdc, 
                 E->envrect.left   + 4, 
                 E->envrect.bottom - 16,
                 profile, 
           (S32) strlen(profile));
         }

      //
      // Copy environment to window
      //

      BitBlt(dc,
             E->envrect.left,
             E->envrect.top,
             E->envrect.right  - E->envrect.left,
             E->envrect.bottom - E->envrect.top,
             E->envdc,
             E->envrect.left,
             E->envrect.top,
             SRCCOPY);

      ReleaseDC(E->env, dc);
      }
}

//############################################################################
//#                                                                          #
//# Render current environment                                               #
//#                                                                          #
//############################################################################

void ENV_render_environment(ENV *E)
{
   //
   // Draw listener's "nose" at its current orientation
   //

   F32 t = atan2(E->listener_bearing_x,
                 E->listener_bearing_z);

   while (t > CIRCLE) t -= CIRCLE;
   while (t < 0.0F)   t += CIRCLE;

   POINT n[3],nose[3];

   S32 L = LISTENERSIZE-1;

   n[0].x = - L / 3;
   n[0].y = - L;
   n[1].x = 0;
   n[1].y = - ((L * 5) / 3);
   n[2].x = + L / 3;
   n[2].y = - L;

   for (S32 i=0; i < 3; i++)
      {
      nose[i].x = S32(n[i].x * cos(t)) - S32(n[i].y * sin(t)) + E->soundadjx;
      nose[i].y = S32(n[i].y * cos(t)) + S32(n[i].x * sin(t)) + E->soundadjy; 
      }

   Polygon(E->envdc,
           nose,
           3);

   Ellipse(E->envdc,
           E->soundadjx - LISTENERSIZE,
           E->soundadjy - LISTENERSIZE,
           E->soundadjx + LISTENERSIZE,
           E->soundadjy + LISTENERSIZE);

   //
   // Draw obstacles
   //

   RECT r;

   if (E->obstruction_crates)
      {
      r.left   = E->soundadjx -  CRATESIZE;
      r.right  = E->soundadjx +  CRATESIZE;
      r.top    = E->soundadjy - (CRATESIZE * 7);
      r.bottom = E->soundadjy - (CRATESIZE * 5);

      FillRect(E->envdc,
              &r,
               E->cratebrush);

      r.top    = E->soundadjy + (CRATESIZE * 4);
      r.bottom = E->soundadjy + (CRATESIZE * 6);

      FillRect(E->envdc,
              &r,
               E->cratebrush);
      }

   if (E->occlusion_walls)
      {
      if (E->movement_type == AUTO_BACK_AND_FORTH)
         {
         r.left   = E->soundadjx - WALLHEIGHT;
         r.right  = E->soundadjx + WALLHEIGHT;
         r.top    = E->soundadjy - (CRATESIZE * 9);
         r.bottom = E->soundadjy - (CRATESIZE * 9) + WALLWIDTH;

         FillRect(E->envdc,
                 &r,
                  E->wallbrush);

         r.top    = E->soundadjy + (CRATESIZE * 7);
         r.bottom = E->soundadjy + (CRATESIZE * 7) + WALLWIDTH;

         FillRect(E->envdc,
                 &r,
                  E->wallbrush);
         }
      else
         {
         r.left   = E->soundadjx - (CRATESIZE * 6) - WALLWIDTH;
         r.right  = E->soundadjx - (CRATESIZE * 6);
         r.top    = E->soundadjy - WALLHEIGHT;
         r.bottom = E->soundadjy + WALLHEIGHT;

         FillRect(E->envdc,
                 &r,
                  E->wallbrush);
         
         r.left   = E->soundadjx + (CRATESIZE * 6);
         r.right  = E->soundadjx + (CRATESIZE * 6) + WALLWIDTH;

         FillRect(E->envdc,
                 &r,
                  E->wallbrush);
         }
      }
}

//############################################################################
//#                                                                          #
//# Reset SND3D position and offset                                          #
//#                                                                          #
//############################################################################

void SND3D_reset_movement(SND3D *S)
{
   if (S->movement_type == AUTO_ROTATE)
      {
      S->x             =  25.0F;
      S->y             =   0.0F;
      S->z             =   0.0F;
      S->offset        =   0.0F;
      S->cone_axis     =   0.0F;
      }
   else if (S->movement_type == AUTO_BACK_AND_FORTH)
      {
      S->x             =   0.0F;
      S->y             =   0.0F;
      S->z             =  75.0F;
      S->offset        =  -1.0F;
      S->cone_axis     =   0.0F;
      }
}

//############################################################################
//#                                                                          #
//# Apply timestep to SND3D object                                           #
//#                                                                          #
//############################################################################
 
void SND3D_update(SND3D *S)
{
   //
   // Get elapsed time since last update
   //

   S32 cur_time_ms = AIL_ms_count();

   S32 dT_ms = cur_time_ms - S->last_time_ms;

   S->last_time_ms = cur_time_ms;

   //
   // Get relative velocity from slider and use it to scale the timestep
   //

   F32 i = (E->speed_slider * (FASTSPEED - SLOWSPEED)) + SLOWSPEED;
   i = i * (F32) dT_ms;

   //
   // Read other UI controls that affect the sample
   //

   S->auto_3D_wet_atten = E->auto_3D_atten;

   //
   // Handle change in movement type
   //

   if (S->movement_type != E->movement_type)
      {
      S->movement_type = E->movement_type;

      SND3D_reset_movement(S);
      }

   //
   // Update position, orientation, occlusion, and obstruction
   //
   // To calculate obstruction and occlusion in a 3d game, you would
   //   usually ray cast a spread of rays out from the sound source
   //   and see how many hit the player's current sector.
   //
   // Here, we simply do it closed form, since the crates and walls
   //   aren't moving.
   //

   S->obstruction = 0.0F;
   S->occlusion   = 0.0F;

   F32 prev_x = S->x;
   F32 prev_y = S->y;
   F32 prev_z = S->z;

   if (E->movement_type == AUTO_BACK_AND_FORTH)
      {
      S->z += (S->offset * i);

      if ( S->z > STRAIGHTDIST )
         S->offset = -1.0F;
      else if ( S->z < -(STRAIGHTDIST))
         S->offset = 1.0F;

      if (E->obstruction_crates)
         {
         if ( ( S->z >= 156.5F ) || ( S->z <= -136.5F ) )
            {
            S->obstruction = 0.50F;
            }
         else
            {
            if ( ( S->z >= 115.5F ) && ( S->z <= 156.5F ) )
               S->obstruction = 0.50F * ( ( S->z - 115.5F ) / 41.0F );
            else if ( ( S->z >= -135.5F ) && ( S->z <= -95.5F ) )
               S->obstruction = 0.50F * ( - ( S->z + 95.5F ) / 41.0F );
            }
         }

      if ( E->occlusion_walls )
         {
         if ( ( S->z >= 198.5F ) || ( S->z <= -175.5F ) )
            {
            S->occlusion = 1.0F;
            }
         else
            {
            if ( ( S->z>= 173.0F ) && ( S->z <= 198.5F ) )
               S->occlusion = ( S->z - 173.0F ) / 25.0F;
            else if ( ( S->z >= -175.0F ) && ( S->z <= -150.0F ) )
               S->occlusion = ( -( S->z + 150.0F ) / 25.0F );
            }
         }
      }
   else 
      {
      if (E->movement_type == AUTO_ROTATE)
         {
         S->offset += (i / CIRCLERADIUS);
         }
      else if (E->movement_type == MANUAL_SOURCE_PLACEMENT)
         {
         //
         // Check for left-click in environment window, and get its bearing to the
         // listener
         //

         if ((GetForegroundWindow() == hwndMain) && 
             (GetKeyState(VK_LBUTTON) & 0x8000))
            {
            POINT screen_ul;
            RECT rect;
            GetClientRect(hwndExamMS, &rect);
            screen_ul.x = rect.left; 
            screen_ul.y = rect.top; 
            ClientToScreen(hwndExamMS, &screen_ul); 

            POINT screen_cursor;
            GetCursorPos(&screen_cursor);  

            F32 x = F32((screen_cursor.x - screen_ul.x) - CLIENT_LISTENER_X);
            F32 y = F32((screen_cursor.y - screen_ul.y) - CLIENT_LISTENER_Y);

            if ((x >= -CLIENT_LISTENER_RANGE_X) && (x < CLIENT_LISTENER_RANGE_X) &&
                (y >= -CLIENT_LISTENER_RANGE_Y) && (y < CLIENT_LISTENER_RANGE_Y))
               {
               S->offset = atan2(-y,x);
               }
            }
         }

      while (S->offset > CIRCLE) S->offset -= CIRCLE;
      while (S->offset < 0.0F)   S->offset += CIRCLE;

      S->x = (F32) (CIRCLERADIUS * cos( S->offset ));
      S->z = (F32) (CIRCLERADIUS * sin( S->offset ));

      if (E->obstruction_crates)
         {
         if ( ( ( S->offset >= 1.43F ) && ( S->offset <= 1.72F ) ) || 
              ( ( S->offset >= 4.51F ) && ( S->offset <= 4.90F ) ) )
            {
            S->obstruction = 0.90F;
            }
         else
            {
            if ( ( S->offset >= 1.38F ) && ( S->offset <= 1.43F ) )
               S->obstruction = 0.90F * ( ( S->offset - 1.38F ) / 0.05F );
            else if ( ( S->offset >= 1.72F ) && ( S->offset <= 1.77F ) )
               S->obstruction = 0.90F * ( 1.0F - ( ( S->offset - 1.72F ) / 0.05F ) );
            else if ( ( S->offset >= 4.46F ) && ( S->offset <= 4.51F ) )
               S->obstruction = 0.90F * ( ( S->offset - 4.46F ) / 0.05F );
            else if ( ( S->offset >= 4.90F ) && ( S->offset <= 4.95F ) )
               S->obstruction = 0.90F * ( 1.0F - ( ( S->offset - 4.90F ) / 0.05F ) );
            }
         }

      if (E->occlusion_walls)
         {
         if ( ( ( S->offset >= 2.76F ) && ( S->offset <= 3.51F ) ) || 
                ( S->offset <= 0.40F ) || ( S->offset >= 5.89F ) )
            {
            S->occlusion = 1.0F;
            }
         else
            {
            if ( ( S->offset >= 0.40F ) && ( S->offset <= 0.81F ) )
               S->occlusion = 1.0F - ( ( S->offset - 0.40F ) / 0.42F );
            else if ( ( S->offset >= 2.34F ) && ( S->offset <= 2.76F ) )
               S->occlusion = ( S->offset - 2.34F ) / 0.42F;
            else if ( ( S->offset >= 3.51F ) && ( S->offset <= 3.93F ) )
               S->occlusion = 1.0F - ( ( S->offset - 3.51F ) / 0.42F );
            else if ( ( S->offset >= 5.47F ) && ( S->offset <= 5.89F ) )
               S->occlusion = ( S->offset - 5.47F ) / 0.42F;
            }
         }
      } 

   //
   // Calculate velocity (for Doppler shift)
   //
   // We'll set this to zero if the source is moving in a circle -- when you are moving in a
   // circle, there is angular velocity, but no velocity away or towards the listener
   //

   if (E->movement_type == AUTO_ROTATE)
      {
      S->dx_dt = 0.0F;
      S->dy_dt = 0.0F;
      S->dz_dt = 0.0F;
      }
   else
      {
      S->dx_dt = (S->x - prev_x) / dT_ms;
      S->dy_dt = (S->y - prev_y) / dT_ms;
      S->dz_dt = (S->z - prev_z) / dT_ms;
      }

   //
   // Update global wet/dry levels and playback rate scale
   //

   S->wet = E->wet_slider;
   S->dry = E->dry_slider;

   S->playback_rate = E->pitch_slider;

   //
   // Update orientation
   //
   // If a directional cone is in use, rotate it
   // If not, the sound source faces the listener at the origin
   //

   S->face_y = -S->y;

   S->use_cone = E->use_cone;

   if (S->use_cone)
      {
      S->cone_axis += (CIRCLE / 45.0F);

      if (S->cone_axis > CIRCLE)
         {
         S->cone_axis -= CIRCLE;
         }

      S->face_x = (F32) (CIRCLERADIUS * cos(S->cone_axis));
      S->face_z = (F32) (CIRCLERADIUS * sin(S->cone_axis));

      S->inner_cone_angle = 90.0F;
      S->outer_cone_angle = 120.0F;
      S->outer_cone_vol   = 0.05F;
      }
   else
      {
      S->face_x = -S->x;
      S->face_z = -S->z;

      S->inner_cone_angle = 360.0F;
      S->outer_cone_angle = 360.0F;
      S->outer_cone_vol   = 1.0F;
      }

   //
   // Maintain looping status
   //

   if (E->looped)
      {
      S->loop_count = 0;
      }
   else
      {
      S->loop_count = 1;
      }
}

//############################################################################
//#                                                                          #
//# Synchronize MSS sample with SND3D state                                  #
//#                                                                          #
//############################################################################

BOOL SND3D_update_HSAMPLE(SND3D *S)
{
   CFG *C = current_configuration;

   if ((C == NULL))
      {
      //
      // No driver loaded
      //

      return FALSE;
      }

   if (S->SND == NULL)
      {
      return TRUE;
      }

   S32 is_3D = TRUE;
   MSSVECTOR3D pos = { S->x, S->y, S->z };

   MARS_sound_property(S->SND->D, SOUND_B_IS_3D,  NULL, &is_3D, NULL, NULL,NULL,NULL);
   MARS_sound_property(S->SND->D, SOUND_V_3D_POS, NULL, &pos,   NULL, NULL,NULL,NULL);

   MSSVECTOR3D face = { S->face_x, S->face_y, S->face_z };
   MSSVECTOR3D up   = { 0.0F,      1.0F,      0.0F      };

   MARS_sound_property(S->SND->D, SOUND_V_3D_FACE, NULL, &face, NULL, NULL,NULL,NULL);
   MARS_sound_property(S->SND->D, SOUND_V_3D_UP,   NULL, &up,   NULL, NULL,NULL,NULL);

   MSSVECTOR3D vel = { S->dx_dt, S->dy_dt, S->dz_dt };
   MARS_sound_property(S->SND->D, SOUND_V_3D_VEL,  NULL, &vel,  NULL, NULL,NULL,NULL);

   MARS_sound_property(S->SND->D, SOUND_F_3D_CONE_INNER_DEGS,   NULL, &S->inner_cone_angle, NULL, NULL,NULL,NULL);
   MARS_sound_property(S->SND->D, SOUND_F_3D_CONE_OUTER_DEGS,   NULL, &S->outer_cone_angle, NULL, NULL,NULL,NULL);
   MARS_sound_property(S->SND->D, SOUND_F_3D_CONE_OUTER_SCALE,  NULL, &S->outer_cone_vol,   NULL, NULL,NULL,NULL);

   F32 max_dist = STRAIGHTDIST + 10;
   F32 min_dist = 40;
   
   MARS_sound_property(S->SND->D, SOUND_F_3D_MAX_DIST, NULL, &max_dist, NULL, NULL,NULL,NULL);
   MARS_sound_property(S->SND->D, SOUND_F_3D_MIN_DIST, NULL, &min_dist, NULL, NULL,NULL,NULL);

   MARS_sound_property(S->SND->D, SOUND_B_3D_AUTO_WET_ATTEN,  NULL, &S->auto_3D_wet_atten, NULL, NULL,NULL,NULL);

   //
   // Set non-3D-specific parameters
   //

   // TODO: looping is awkward because of state mismatch
   MARS_sound_property(S->SND->D, SOUND_N_INITIAL_LOOP_COUNT,   NULL, &S->loop_count, NULL, NULL,NULL,NULL);
   MARS_sound_property(S->SND->D, SOUND_N_REMAINING_LOOP_COUNT, NULL, &S->loop_count, NULL, NULL,NULL,NULL);

   MARS_sound_property(S->SND->D, SOUND_F_OCCLUSION,   NULL, &S->occlusion,   NULL, NULL,NULL,NULL);
   MARS_sound_property(S->SND->D, SOUND_F_OBSTRUCTION, NULL, &S->obstruction, NULL, NULL,NULL,NULL);

   MARS_sound_property(S->SND->D, SOUND_F_WET_SCALE, NULL, &S->wet, NULL, NULL,NULL,NULL);
   MARS_sound_property(S->SND->D, SOUND_F_DRY_SCALE, NULL, &S->dry, NULL, NULL,NULL,NULL);
   
   //
   // Adjust playback rate up or down with slider
   // 

   MARS_sound_property(S->SND->D, SOUND_N_ORIGINAL_RATE_HZ, &S->base_sample_rate, NULL, NULL, NULL,NULL,NULL);

   S32 half_rate = S->base_sample_rate / 2;

   S32 min_rate = S->base_sample_rate - half_rate;
   S32 max_rate = S->base_sample_rate + half_rate;

   S32 new_rate =  min_rate + (((max_rate - min_rate) * S->playback_rate) / 128);

   MARS_sound_property(S->SND->D, SOUND_N_PLAYBACK_RATE_HZ, NULL, &new_rate, NULL, NULL,NULL,NULL);

   //
   // Trigger sample, and maintain our view of its playback state
   //

   if (S->triggered)
      {
      S->triggered = FALSE;

      S->SND->play();
      }

   U32 status = 0;
   MARS_sound_property(S->SND->D, SOUND_STATUS, &status, NULL, NULL, NULL,NULL,NULL);

   S->playing = (status & MARS_PLAYING);

   return TRUE;
}

//############################################################################
//#                                                                          #
//# Render SND3D representation to environment display                       #
//#                                                                          #
//############################################################################

void SND3D_render(SND3D *S, //)
                  ENV   *E)
{
   //
   // Render playing samples only
   //

   if (!S->playing)
      {
      return;
      }

   //
   // Calculate screenspace position and length of cone-direction pointer
   //

   F32 length = (F32) sqrt((S->face_x * S->face_x) + (S->face_z * S->face_z));

   if (length <= 0.0001F)
      {
      length = 1.0F;
      }

   F32 f,r;

   if (E->movement_type == AUTO_BACK_AND_FORTH)
      {
      r = STRAIGHTDIST * 2.0F;
      f = 20.0F;
      }                                                                                          
   else
      {
      r = CIRCLERADIUS * 2.0F;
      f = 10.0F;
      }

   S32 x  = (S32) ((  S->x * (F32) E->envwidth)                                / r);
   S32 y  = (S32) (( -S->z * (F32) E->envwidth)                                / r);
   S32 dx = (S32) ((( S->x + (( S->face_x / length) * f)) * (F32) E->envwidth) / r);
   S32 dy = (S32) (((-S->z + ((-S->face_z / length) * f)) * (F32) E->envwidth) / r);

   //
   // Draw sound representation
   //

   MoveToEx(E->envdc,
            E->soundadjx + x,
            E->soundadjy + y,
            0);

   LineTo(E->envdc,
          E->soundadjx + dx,  
          E->soundadjy + dy);

   Ellipse(E->envdc,
           E->soundadjx + x - SOUNDSIZE,
           E->soundadjy + y - SOUNDSIZE,
           E->soundadjx + x + SOUNDSIZE,
           E->soundadjy + y + SOUNDSIZE);
}

//############################################################################
//#                                                                          #
//# Destroy existing configuration                                           #
//#                                                                          #
//############################################################################

void CFG_destroy(CFG *C)
{
   //
   // Disable UI
   //

   UI_disable_all_controls();

//   //
//   // Free all SND3D HSAMPLEs
//   //
//
//   for (S32 i=0; i < MAX_3D_SOUNDS; i++)
//      {
//      SND3D *S = &sounds[i];
//
//      if (S->S != NULL)
//         {
//         AIL_release_sample_handle(S->S);
//         S->S = NULL;
//         }
//      }
//
//   //
//   // Unload driver
//   //
//
//   if (C->DIG3D != NULL)
//      {
//      AIL_close_digital_driver(C->DIG3D);
//      C->DIG3D = NULL;
//      }
//
//   AIL_mem_free_lock(C);
}

//############################################################################
//#                                                                          #
//# Create new configuration based on selected combo-box entry               #
//#                                                                          #
//############################################################################

CFG *CFG_create(S32 index)
{
   //
   // Allocate and zero a new CFG object
   //

   CFG *C = (CFG *) AIL_mem_alloc_lock(sizeof(CFG));

   if (C == NULL)
      {
      return NULL;
      }

   memset(C,
          0,
          sizeof(CFG));

   //
   // Load a digital driver
   //

//   C->DIG3D = AIL_open_digital_driver(44100,
//                                      16,
//                                      combo_entries[index].channel_spec,
//                                      0);
//
//   if (C->DIG3D == NULL)
//      {
//      MessageBox(0,AIL_last_error(),"Error",MB_OK);
//
//      CFG_destroy(C);
//      return NULL;
//      }

   MARS_set_output_format(ME, 
                          combo_entries[index].channel_spec,
                          44100, 
                          64, 
                          MEV_RESTORE_SOUND_INSTANCES);

   //
   // See if we're running under an EAX-compatible provider
   // (i.e., not DS3D hardware or another output filter that doesn't support reverb)
   //
   // The default room type will be -1 on these providers
   //

   HPROVIDER PRO = 0;
   MARS_output_filter(ME, &PRO, &C->reverb_supported);

   S32 using_output_filter = (PRO != 0);

   if (C->reverb_supported)       // TODO
      {
      MARS_ROOM_TYPE fuck=MARS_ROOM_BATHROOM;

      MARS_environment_property(ME,
                                ENV_RTYPE_REVERB_ROOM_TYPE,
                                NULL,
                               &fuck,
                                NULL,
                                NULL,
                                NULL,
                                NULL);

      fuck = MARS_ROOM_GENERIC;

      MARS_environment_property(ME,
                                ENV_RTYPE_REVERB_ROOM_TYPE,
                                NULL,
                               &fuck,
                                NULL,
                                NULL,
                                NULL,
                                NULL);


      }


   //
   // Enable/disable the reverb-property controls accordingly
   //

   UI_enable_base_controls();

   UI_enable_control(txtRevEnv,    C->reverb_supported);
   UI_enable_control(radNone,      C->reverb_supported);
   UI_enable_control(radBath,      C->reverb_supported);
   UI_enable_control(radMountains, C->reverb_supported);
   UI_enable_control(radHallway,   C->reverb_supported);
   
   UI_enable_control(txtRevMix,    C->reverb_supported);
   UI_enable_control(txtDry,       C->reverb_supported);
   UI_enable_control(txtWet,       C->reverb_supported);
   UI_enable_control(sldWet,       C->reverb_supported);
   UI_enable_control(sldDry,       C->reverb_supported);
   UI_enable_control(chk3DAtten,   C->reverb_supported);

   UI_enable_control(radFalloff, !using_output_filter);
   UI_enable_control(radF1, !using_output_filter);
   UI_enable_control(radF2, !using_output_filter);
   UI_enable_control(radF3, !using_output_filter);
   UI_enable_control(radF4, !using_output_filter);

//   //
//   // Invalidate all samples, and retrigger any that were playing
//   //
//
//   for (S32 i=0; i < MAX_3D_SOUNDS; i++)
//      {
//      SND3D *S = &sounds[i];
//
//      S->data_registered = FALSE;
//
//      if (S->playing)
//         {
//         S->triggered = TRUE;
//         }
//      }

   return C;
}

//############################################################################
//#                                                                          #
//#  Stop all playing sounds                                                 #
//#                                                                          #
//############################################################################

void CFG_stop_all_sounds(CFG *C)
{
   for (S32 i=0; i < MAX_3D_SOUNDS; i++)
      {
      SND3D *S = &sounds[i];

      if (S->SND != NULL)
         {
         MARS_end(S->SND->D);
         S->SND = NULL;
         S->playing = FALSE;
         }
      }
}

////############################################################################
////#                                                                          #
////# Trigger 3D-positioned sound                                              #
////#                                                                          #
////############################################################################
//
//void CFG_play_current_sample(CFG *C)
//{
//   //
//   // Find a stopped sample
//   //
//
//   S32 i;
//
//   for (i=0; i < MAX_3D_SOUNDS; i++)
//      {
//      SND3D *S = &sounds[i];
//
//      if (!S->playing)
//         {
//         break;
//         }
//      }
//
//   if (i == MAX_3D_SOUNDS)
//      {
//      return;
//      }
//
//   SND3D *S = &sounds[i];
//
//   //
//   // Make a copy of the currently-loaded sound file image, freeing any old one first
//   //
//
//   if (S->loaded_sample_address != NULL)
//      {
//      AIL_mem_free_lock(S->loaded_sample_address);
//      S->loaded_sample_address = NULL;
//      }
//
//   S->loaded_sample_address   = AIL_mem_alloc_lock(loaded_sample_file_size);
//   S->loaded_sample_file_size = loaded_sample_file_size;
//
//   memcpy(S->loaded_sample_address,
//          loaded_sample_address,
//          loaded_sample_file_size);
//
//   strcpy(S->loaded_sample_filename,
//          loaded_sample_filename);
//
//   //
//   // Reposition sample's physical location and play cursor
//   //
//
//   SND3D_reset_movement(S);
//
//   S->play_cursor = 0;
//
//   //
//   // Arrange for sample's data to be passed to MSS and played
//   //
//
//   S->data_registered = FALSE;
//   S->triggered       = TRUE;
//}
//

//############################################################################
//#                                                                          #
//# Trigger 3D-positioned SOUNDDEF and arrange to control it                 #
//#                                                                          #
//############################################################################

void play_SND(SOUNDDEF *SND)
{
   //
   // Find a stopped sample
   //

   S32 i;

   for (i=0; i < MAX_3D_SOUNDS; i++)
      {
      SND3D *S = &sounds[i];

      if (!S->playing)
         {
         break;
         }
      }

   if (i == MAX_3D_SOUNDS)
      {
      return;
      }

   SND3D *S = &sounds[i];

   S->SND = SND;

   //
   // Reposition sample's physical location and play cursor
   //

   SND3D_reset_movement(S);

   S->play_cursor = 0;

   //
   // Arrange for sample's data to be passed to MSS and played
   //

   S->triggered       = TRUE;
}

//############################################################################
//#                                                                          #
//# Update current configuration                                             #
//#                                                                          #
//############################################################################

void CFG_update(CFG *C)
{
   //
   // This is called from WM_TIMER.  We don't want it re-entered from within an error dialog's 
   // internal message pump
   //

   static S32 reentry_flag = 0;

   if (reentry_flag)
      {
      return;
      }

   ++reentry_flag;

   //
   // Set falloff parameter
   //

   MARS_environment_property(ME,
                             ENV_F_FALLOFF_POWER,
                             NULL,
                            &E->requested_falloff,
                             NULL,
                             NULL,
                             NULL,
                             NULL);
   
   //
   // See if a new room type has been requested
   //
   
   MARS_environment_property(ME,
                             ENV_RTYPE_REVERB_ROOM_TYPE,
                             NULL,
                            &E->requested_room_type,
                             NULL,
                             NULL,
                             NULL,
                             NULL);

//   if (AIL_room_type(C->DIG3D) != E->requested_room_type)
//      {
//      AIL_set_room_type(C->DIG3D,
//                        E->requested_room_type);
//      }

   //
   // Update environment based on UI controls
   //

   ENV_begin_frame(E);

   //
   // Render environment (listener, walls, etc.)
   //

   ENV_render_environment(E);

   //
   // Service individual sound instances
   //

   for (S32 i=0; i < MAX_3D_SOUNDS; i++)
      {
      SND3D *S = &sounds[i];

      //
      // Update SND3D based on UI controls
      //

      SND3D_update(S);

      //
      // Bring HSAMPLE up to date with SND3D state
      //

      if (!SND3D_update_HSAMPLE(S))
         {
         CFG_destroy(C);

         --reentry_flag;
         return;
         }

      //
      // Render SND3D state into environment
      //

      SelectObject(E->envdc, 
                   E->soundbrush[i]);

      SND3D_render(S, E);
      }

   //
   // Render result and exit
   //

   ENV_end_frame(E);

   --reentry_flag;
}

//############################################################################
//#                                                                          #
//#  Main window procedure                                                   #
//#                                                                          #
//############################################################################

BOOL CALLBACK ExamDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
   switch (Message)
      {
      case WM_COMMAND:
         {
         switch (LOWORD(wParam))
            {
            case cboxConfig:
               {
               if (HIWORD(wParam) == CBN_SELENDOK)
                  {
                  S32 selected = ComboBox_GetCurSel(combo) - 1;

                  if (selected >= 0)
                     {
//                     if (current_configuration != NULL)
//                        {
//                        CFG_destroy(current_configuration);
//                        current_configuration = NULL;
//                        }

                     current_configuration = CFG_create(selected);
                     }
                  }

                break;
                }

            case btnPlay:
               {
               if (hwndTreeView == NULL)
                  {
                  break;
                  }

               TVITEM tvItem;
               memset(&tvItem, 0, sizeof(tvItem));

               tvItem.hItem = TreeView_GetSelection(hwndTreeView); 
               tvItem.mask  = TVIF_PARAM;

               if (!TreeView_GetItem(hwndTreeView, &tvItem))
                  {
                  break;
                  }

               TREENODE *T = (TREENODE *) tvItem.lParam;

               if (T->type != TNT_SOUNDDEF)
                  {
                  break;
                  }

               SOUNDDEF *SND = (SOUNDDEF *) T;

               //
               // Kill any other instances of this sound definition -- we can
               // only deal with one at a time in the tree control!
               //

               MARS_end(SND->D);

               if (current_configuration != NULL)
                  {
                 // SND->play();
                  play_SND(SND);
                  }

               break;
               }

            case btnStop:
               {
               if (current_configuration != NULL)
                  {
                  CFG_stop_all_sounds(current_configuration);
                  }

               break;
               }
            }

         return TRUE;
         }

      case WM_TIMER:
         {
         if (current_configuration != NULL)
            {
            CFG_update(current_configuration);
            }
         break;
         return TRUE;
         }

      default:
         return FALSE;
      }

   return TRUE;
}

void ____________________________________________________Menu_commands_____________________________________________(){}

void CMD_new_project(void)
{
   if (ME == NULL)      // only one allowed for now
      {
      projects.alloc();
      }
}

void CMD_quit(void)
{
   PostQuitMessage(0);
}

void ____________________________________________________Window_management_________________________________________(){}

//****************************************************************************
//
// Maintain globally-accessible position and sizes of main window and
// all child windows, adjusting child window sizes based on client 
// area of main window
//
//****************************************************************************

void update_window_position(void)
{
   RECT rect;
   GetWindowRect(hwndMain, &rect);

   window_x = rect.left;
   window_y = rect.top;
   window_w = rect.right  - rect.left;
   window_h = rect.bottom - rect.top;
}

void update_window_layout(S32 new_client_width  = -1,     
                          S32 new_client_height = -1)
{
   RECT presize;
   GetClientRect(hwndMain, &presize);

   if (new_client_width == -1)  new_client_width  = presize.right;
   if (new_client_height == -1) new_client_height = presize.bottom;

   client_w = new_client_width;
   client_h = new_client_height;

   assert(hwndTreeView == GetDlgItem(hwndMain, IDC_TREEVIEW));

   MoveWindow(hwndTreeView,
              0, 
              TOP_BORDER_HEIGHT, 
              splitter_v,
              splitter_12 - TOP_BORDER_HEIGHT, 
              TRUE);

   MoveWindow(hwndPropCtrl, 
              0, 
              splitter_12 + splitter_w, 
              splitter_v,
              new_client_height - splitter_12 - splitter_w - BOTTOM_BORDER_HEIGHT, 
              TRUE);

   MoveWindow(hwndExamMS, 
              splitter_v + splitter_w, 
              TOP_BORDER_HEIGHT, 
              new_client_width - splitter_v - splitter_w,
              splitter_34 - TOP_BORDER_HEIGHT, 
              TRUE);

   MoveWindow(hwndWaveCtrl, 
              splitter_v + splitter_w,
              splitter_34 + splitter_w, 
              new_client_width - splitter_v - splitter_w,
              new_client_height - splitter_34 - splitter_w - BOTTOM_BORDER_HEIGHT, 
              TRUE);

   update_window_position();

   //
   // Update waveform viewer, if any
   //

   if (global_waveform_viewer != NULL)
      {
      global_waveform_viewer->destroy_control();
      global_waveform_viewer->create_control(hwndWaveCtrl);
      }

}

BOOL CALLBACK PropCtrlDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
   switch (Message)
      {
      case WM_TIMER:
         {
         switch (wParam)
            {
            case 1: 
               {
               //
               // If current SNDPROP and ENVPROP are both NULL, leave the current text
               // alone
               //

               C8 text_value[MARS_MAX_PROP_TEXT_LEN];
               
               C8 *help = NULL;

               MARS_PROPERTY_INFO *info = NULL;

               if (global_selected_ENVPROP != NULL) 
                  {
                  info = &global_selected_ENVPROP->info;

                  if (MARS_environment_property_text(ME,
                                                     info->name,
                                                     text_value,
                                                     NULL,
                                                     NULL,
                                                     NULL,
                                                     NULL,
                                                    &help) != MARS_OK)
                     {
                     break;
                     }
                   }
               else if (global_selected_SNDPROP != NULL) 
                  {
                  info = &global_selected_SNDPROP->info;

                  if (MARS_sound_property_text(global_selected_SNDPROP->D,
                                               info->name,
                                               text_value,
                                               NULL,
                                               NULL,
                                               NULL,
                                               NULL,
                                              &help) != MARS_OK)
                     {
                     break;
                     }
                  }
               else 
                  {
                  break;
                  }
               
               //
               // Get existing text caption and box contents and see if they've
               // changed, so we don't update the window unnecessarily
               //

               C8 current[512];
               BOOL new_prop_sel = FALSE;

               GetDlgItemText(hwnd, IDC_CTRL_HELP, current, sizeof(current));

               if (strcmp(current, info->name))
                  {
                  new_prop_sel = TRUE;
                  }
               else
                  {
                  GetDlgItemText(hwnd, IDC_CTRL_BOX, current, sizeof(current));

                  if (strcmp(current, help))
                     {
                     new_prop_sel = TRUE;
                     }
                  }

               if (new_prop_sel) 
                  {
                  SetDlgItemText(hwnd, IDC_CTRL_HELP, info->name);
                  SetDlgItemText(hwnd, IDC_CTRL_BOX,  help);
                  }

               break;
               }
            }
         return TRUE;
         }

      case WM_COMMAND:
         {
         }

      default:
         return FALSE;
      }

   return TRUE;
}

LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   switch (msg)
      {
//      case WM_MOUSEMOVE:
//         {
//         S32 x = LOWORD(lParam);
//         S32 y = HIWORD(lParam);
//
//         mouse_x = x;
//         mouse_y = y;
//         break;
//         }

      case WM_MOUSEWHEEL:
         {
         S32 zDelta = S32(wParam) >> 16;

         if (zDelta > 1)
            {
            waveform_scale++;
            }
         else
            {
            waveform_scale--;
            }

         if (waveform_scale < 0)            waveform_scale = 0;
         if (waveform_scale >= N_WF_SCALES) waveform_scale = N_WF_SCALES-1;
         break;
         }
      }

   return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   switch (msg)
      {
      case WM_CREATE:
         {
         hwndTreeView = Tree_Create(hInstance,
                                    hwnd);

         hwndPropCtrl = CreateDialog(hInstance,
                                     MAKEINTRESOURCE(IDD_CONTROL),
                                     hwnd,
                                     PropCtrlDlgProc);
      
         hwndExamMS = CreateDialog(hInstance,
                                   MAKEINTRESOURCE(IDD_EXAMMS),
                                   hwnd,
                                   ExamDlgProc);

         hwndWaveCtrl = CreateWindowEx(WS_EX_CLIENTEDGE,
                                       szChildName,
                                       "", 
                                       WS_VISIBLE | WS_CHILD,
                                       0,
                                       0,
                                       0,
                                       0,
                                       hwnd, 
                                       0, 
                                       hInstance, 
                                       0);      

         Tree_Init(hwndTreeView);

         ShowWindow(hwndTreeView, TRUE);
         ShowWindow(hwndPropCtrl, TRUE);
         ShowWindow(hwndExamMS,   TRUE);
         ShowWindow(hwndWaveCtrl, TRUE);

         CMD_new_project();
         return 0;
         }

      case WM_DESTROY:
         {
         SetFocus(hwndMain);
         PostQuitMessage(0);
         return 0;
         }

      case WM_TIMER:
         {
         switch (wParam)
            {
            case 1: 
               {
               InvalidateRect(hwndTreeView, NULL, FALSE); 
               break;
               }

            case 2:
               {
               if ((global_waveform_viewer != NULL) && (last_triggered_sound != NULL))
                  {
                  if (global_waveform_viewer->waveform_view_valid)
                     {
                     HSAMPLE S;

                     MARS_sound_property(last_triggered_sound,
                                         SOUND_L_UNDERLYING_HSAMPLE,
                                        &S,
                                         NULL,
                                         NULL);

                     global_waveform_viewer->update_control(S);
                     }
                  }
               break;
               }
            }
         break;
         }

      case WM_NOTIFY:
         {
         LPNMHDR pnmh = (LPNMHDR) lParam;

         switch (pnmh->idFrom)
            {
            case IDC_TREEVIEW:
               {
               return Tree_Notify(hwnd, lParam);
               }
            }

         return 0;
         }

      case WM_CONTEXTMENU:
         {
         POINT ptScreen;
         POINT ptClient;

         ptScreen.x = GET_X_LPARAM(lParam);
         ptScreen.y = GET_Y_LPARAM(lParam);
         ptClient = ptScreen;
         ScreenToClient(hwnd, &ptClient);

         HWND hwndOver = ChildWindowFromPoint(hwnd, ptClient);

         if (GetDlgItem(hwnd, IDC_TREEVIEW) == hwndOver)
            {
//            Tree_DoContextMenu(hwndOver, &ptScreen);
            }

         break;
         }

      case WM_SETCURSOR:
         {
         if (force_cursor)
            {
            SetCursor(current_cursor);
            return TRUE;
            }

         break;
         }

      case WM_GETMINMAXINFO:
         {
         MINMAXINFO *M = (MINMAXINFO *) lParam;
         M->ptMinTrackSize.x = MIN_WINDOW_X_SIZE;
         M->ptMinTrackSize.y = MIN_WINDOW_Y_SIZE;
         break;
         }

      case WM_MOVE:
         {
         update_window_position();
         return 0;
         }

      case WM_SIZE:
         {
         S32 nw = LOWORD(lParam);
         S32 nh = HIWORD(lParam);

         //
         // Maintain proportions of child panes (if this isn't the initial update)
         //
         // Don't try to auto-size the child panes until the menu has been constructed, because
         // it affects the effective client-area size
         //

         if (client_w && menu_valid)
            {
            F64 f12 = ((F64) splitter_12) / ((F64) client_h);
            F64 f34 = ((F64) splitter_34) / ((F64) client_h);
            F64 fv  = ((F64) splitter_v)  / ((F64) client_w);

            splitter_12 = (S32) ((f12 * ((F64) nh)) + 0.5);
            splitter_34 = (S32) ((f34 * ((F64) nh)) + 0.5);
            splitter_v  = (S32) ((fv  * ((F64) nw)) + 0.5);
            }

         update_window_layout(nw, nh);
         return 0;
         }

      case WM_PAINT:
         {
         HDC DC = GetDC(hwnd);

         RECT rect;
         GetClientRect(hwnd, &rect);
         rect.left = rect.right - 16;
         rect.top  = rect.bottom - 16;

         DrawFrameControl(DC,
                         &rect,
                          DFC_SCROLL,
                          DFCS_SCROLLSIZEGRIP);

         ReleaseDC(hwnd,DC);
         break;
         }

      case WM_CLOSE:
         DestroyWindow(hwnd);
         return 0;

#if 1
      case WM_LBUTTONDOWN:

         Splitter_OnLButtonDown(hwnd, msg, wParam, lParam);
         return 0;

      case WM_LBUTTONUP:

         Splitter_OnLButtonUp(hwnd, msg, wParam, lParam);
         return 0;

      case WM_MOUSEMOVE:

         Splitter_OnMouseMove(hwnd, msg, wParam, lParam);
         return 0;
#endif

      case WM_CHAR:

         switch (wParam)
            {
            case 14:    // ^N
               {
               CMD_new_project();
               break;
               }

            break;
            }

//      case WM_KEYDOWN:
//
//         global_WM_KEYDOWN(wParam);
//         break;

      case WM_COMMAND:
         {
         switch (wParam)
            {
            case IDM_NEW_PROJECT:
               {
               CMD_new_project();
               break;
               }

            case IDM_QUIT:
               PostQuitMessage(0);
               break;

            case IDM_CLOSE_PROJECT:
               {
               // which project is currently selected?
               break;
               }
            }
         }

      default:
         break;
      }

   return DefWindowProc(hwnd, msg, wParam, lParam);
}

//****************************************************************************
//
// Message handlers for splitter windows
//
// These are called on the main HWND for mouse events that didn't occur 
// inside a valid child window client area (i.e., only for the splitter borders)
//
//****************************************************************************

static PANE_BORDER mouse_over_border = BORDER_NONE;
static int  oldy = -1;
static int  oldx = -1;
static BOOL fMoved = FALSE;
static BOOL fDragMode = FALSE;

PANE_BORDER Splitter_IdentifyBorder(S32 x, S32 y)
{          
   S32 b = splitter_w / 2;
   S32 h = x - (splitter_v + b);
   S32 dy = (h < 0) ? abs(y - (splitter_12 + b)) : abs(y - (splitter_34 + b)); 
   S32 dx = abs(h);

   if (dx < dy)
      {
      if (dx > 3)
         {
         return BORDER_NONE;
         }

      return BORDER_V;
      }
   else        
      {        
      if (dy > 3)
         {
         return BORDER_NONE;
         }

      if (h < 0)
         {
         return BORDER_12;
         }
      else
         {
         return BORDER_34;
         }
      }

   return BORDER_NONE;
}

LRESULT Splitter_OnLButtonDown(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
   if (fDragMode == TRUE)
      {
      return 0;
      }

   fDragMode = TRUE;

   S32 x = LOWORD(lParam);
   S32 y = HIWORD(lParam);

   mouse_over_border = Splitter_IdentifyBorder(x,y);

   SetCapture(hwnd);    // Must capture mouse or the main HWND won't receive all the WM_MOUSEMOVEs

   RECT cr;
   GetClientRect(hwnd, &cr);

   switch (mouse_over_border)
      {
      case BORDER_12:
         draw_XOR_bar(hwnd, 0, y - 2, splitter_v-1, 4);
         break;

      case BORDER_34:
         draw_XOR_bar(hwnd, splitter_v + 2, y - 2, cr.right - (splitter_v + 2) - 1, 4);
         splitter_34 = y;
         break;

      case BORDER_V:
         draw_XOR_bar(hwnd, x - 2, TOP_BORDER_HEIGHT, 4, cr.bottom - (TOP_BORDER_HEIGHT + BOTTOM_BORDER_HEIGHT));
         break;
      }

   oldx = x;
   oldy = y;
      
   return 0;
}


LRESULT Splitter_OnLButtonUp(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
   if (fDragMode == FALSE)
      {
      return 0;
      }

   fDragMode = FALSE;

   S32 x = LOWORD(lParam);
   S32 y = HIWORD(lParam);

   RECT cr;
   GetClientRect(hwnd, &cr);

   switch (mouse_over_border)
      {
      case BORDER_12:
         draw_XOR_bar(hwnd, 0, oldy - 2, splitter_v-1, 4);
         splitter_12 = y;
         break;

      case BORDER_34:
         draw_XOR_bar(hwnd, splitter_v + 2, oldy - 2, cr.right - (splitter_v + 2) - 1, 4);
         splitter_34 = y;
         break;

      case BORDER_V:
         draw_XOR_bar(hwnd, oldx - 2, TOP_BORDER_HEIGHT, 4, cr.bottom - (TOP_BORDER_HEIGHT + BOTTOM_BORDER_HEIGHT));
         splitter_v = x;
         break;
      }

   update_window_layout();

   ReleaseCapture();

   set_current_cursor(HC_arrow);
   return 0;
}

LRESULT Splitter_OnMouseMove(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
   S32 x = LOWORD(lParam);
   S32 y = HIWORD(lParam);

   if (fDragMode == FALSE) 
      {
      switch (Splitter_IdentifyBorder(x,y))
         {
         case BORDER_12:
         case BORDER_34:
            set_current_cursor(HC_size_NS);
            break;

         case BORDER_V:
            set_current_cursor(HC_size_WE);
            break;

         default:
            set_current_cursor(HC_arrow);
            break;
         }

      return 0;
      }

   RECT cr;
   GetClientRect(hwnd, &cr);

   if (((x != oldx) || (y != oldy)) && wParam & MK_LBUTTON)
      {
      switch (mouse_over_border)
         {
         case BORDER_12:
            draw_XOR_bar(hwnd, 0, oldy - 2, splitter_v-1, 4);
            draw_XOR_bar(hwnd, 0, y    - 2, splitter_v-1, 4);
            break;

         case BORDER_34:
            draw_XOR_bar(hwnd, splitter_v + 2, oldy - 2, cr.right - (splitter_v + 2) - 1, 4);
            draw_XOR_bar(hwnd, splitter_v + 2, y    - 2, cr.right - (splitter_v + 2) - 1, 4);
            break;

         case BORDER_V:
            draw_XOR_bar(hwnd, oldx - 2, TOP_BORDER_HEIGHT, 4, cr.bottom - (TOP_BORDER_HEIGHT + BOTTOM_BORDER_HEIGHT));
            draw_XOR_bar(hwnd, x - 2,    TOP_BORDER_HEIGHT, 4, cr.bottom - (TOP_BORDER_HEIGHT + BOTTOM_BORDER_HEIGHT));
            break;
         }

      oldx = x;
      oldy = y;
      }

   return 0;
}

void ____________________________________________________Main_______________________________________________________(){}

//****************************************************************************
//
// Exit handlers
//
// These routines handle exits under different conditions (exit() call, 
// user request via GUI, etc.)
//
//****************************************************************************

static int exit_handler_active = 0;

void WINAPI WinClean(void)
{
   if (exit_handler_active)
      {
      return;
      }

   exit_handler_active = 1;

   //
   // Update our .INI file
   //

   INI_update();

   //
   // Shut down libraries
   // 

   if (E != NULL)
      {
      ENV_destroy(E);
      }

   if (ME != NULL)
      {
      MARS_destroy_environment(ME);
      ME = NULL;
      }

//   AIL_register_trace_callback(NULL, 0);
   MARS_shutdown();

   CoUninitialize();
}

void WINAPI WinExit(void)
{
   if (!exit_handler_active)
      {
      WinClean();
      }

   exit(0);
}

void AppExit(void)
{
   if (!exit_handler_active)
      {
      WinClean();
      }

   return;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nShowCmd)
{
   WNDCLASSEX   wc,cc;
   MSG         msg;

   hInstance = hInst;

   InitCommonControls();
   CoInitialize(NULL);

   //
   // Read RAD.INI to obtain window sizes and positions
   //

   if (!INI_read())
      {
      exit(1);
      }

   //
   // Start MSS
   //

   MARS_startup();

//   AIL_register_trace_callback(AIL_trace_callback, 1);

   //
   // Register exit handlers
   //

   atexit(AppExit);

   //
   // Load needed GDI resources and register our window classes
   //

   RAD_icon   = LoadIcon   (hInst, MAKEINTRESOURCE(IDI_ICON)); 

   HC_size_WE = LoadCursor(NULL, IDC_SIZEWE); 
   HC_size_NS = LoadCursor(NULL, IDC_SIZENS); 
   HC_arrow   = LoadCursor (NULL, IDC_ARROW); 

   force_cursor   = FALSE;
   current_cursor = HC_arrow;

   memset(&cc, 0, sizeof(cc));
   cc.cbSize        = sizeof(cc);
   cc.style         = 0;
   cc.lpfnWndProc   = ChildWndProc;
   cc.cbClsExtra    = 0;
   cc.cbWndExtra    = 0;
   cc.hInstance     = hInst;
   cc.hIcon         = RAD_icon;
   cc.hCursor       = HC_arrow;
//   cc.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
   cc.hbrBackground = (HBRUSH)(COLOR_3DFACE+1);
   cc.lpszMenuName  = 0;
   cc.lpszClassName = szChildName;

   RegisterClassEx (&cc);

   memset(&wc, 0, sizeof(wc));
   wc.cbSize        = sizeof(wc);
   wc.style         = 0;
   wc.lpfnWndProc   = MainWndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = hInst;
   wc.hIcon         = RAD_icon;
   wc.hCursor       = HC_arrow;
   wc.hbrBackground = (HBRUSH)(COLOR_3DFACE+1);
   wc.lpszMenuName  = 0;
   wc.lpszClassName = szAppName;

   RegisterClassEx(&wc);

   //
   // Create the main window. This window will host the child windows and dialogs
   //

   hwndMain = CreateWindowEx(0,                  // extended style (not needed)
                             szAppName,          // window class name
                             szAppName,          // window caption
                             WS_OVERLAPPEDWINDOW |
                             WS_CLIPCHILDREN,    // window style
                             window_x,             // initial x position
                             window_y,             // initial y position
                             window_w,             // initial x size
                             window_h,             // initial y size
                             NULL,               // parent window handle
                             NULL,               // use window class menu
                             hInst,              // program instance handle
                             NULL);              // creation parameters

   ShowWindow(hwndMain, nShowCmd);

   main_menu_init();

   //
   // Init legacy EXAMMS stuff
   //
   // First copy configuration choices to the dropdown list...
   //
   
   combo = GetDlgItem(hwndExamMS, cboxConfig);

   ComboBox_AddString(combo,"Choose a configuration...");

   S32 n_configs = sizeof(combo_entries) / sizeof(combo_entries[0]);

   for (S32 i=0; i < n_configs; i++)
      {
      ComboBox_AddString(combo,
                         combo_entries[i].name);
      }

   ComboBox_SetCurSel(combo,0);

   //
   // Disable all controls except the configuration selector, and set defaults
   //

   UI_disable_all_controls();

   UI_set_control_defaults();

   //
   // Create bitmap environment
   //

   E = ENV_create();

   //
   // Initialize 3D sample slots
   //

   for (S32 i=0; i < MAX_3D_SOUNDS; i++)
      {
      SND3D *S = &sounds[i];

      memset(S,
             0,
             sizeof(*S));

      S->movement_type = INVALID;

      S->last_time_ms = AIL_ms_count();
      }

   //
   // Set timers and enter message loop
   //

   HACCEL hAccels = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELS));

   SetTimer(hwndMain,     1, TIMER_SERVICE_TREE_MS,   NULL);
   SetTimer(hwndMain,     2, TIMER_SERVICE_WAVE_MS,   NULL);
   SetTimer(hwndExamMS,   1, TIMER_SERVICE_EXAMMS_MS, NULL);
   SetTimer(hwndPropCtrl, 1, TIMER_SERVICE_PROP_MS,   NULL);

   while (GetMessage(&msg, NULL, 0, 0))
      {
      if (msg.message == WM_KEYDOWN)
         {
         global_WM_KEYDOWN(msg.wParam);
         }
//      else if (msg.message == WM_MOUSEWHEEL)
//         {
//         S32 zDelta = S32(msg.wParam) >> 16;
//
//         if (zDelta > 1)
//            {
//            waveform_scale++;
//            }
//         else
//            {
//            waveform_scale--;
//            }
//
//         if (waveform_scale < 0)            waveform_scale = 0;
//         if (waveform_scale >= N_WF_SCALES) waveform_scale = N_WF_SCALES-1;
//         }

      if (!TranslateAccelerator(hwndMain, hAccels, &msg))
//      if (!IsWindow(hwndExamMS) || !IsDialogMessage(hwndExamMS, &msg))
         {
         TranslateMessage(&msg);
         DispatchMessage(&msg);
         }
      }

   return 0;
}

