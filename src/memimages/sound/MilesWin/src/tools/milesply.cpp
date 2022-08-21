//############################################################################
//##                                                                        ##
//##  MilesPly.CPP                                                          ##
//##                                                                        ##
//##  MSS 4.0 Tool                                                          ##
//##                                                                        ##
//##  V1.00 of 18-Mar-96: Initial version                                   ##
//##                                                                        ##
//##  C source compatible with Microsoft C v9.0 or later                    ##
//##                                                                        ##
//##   Author: Jeff Roberts                                                 ##
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
#include <zmouse.h>
#include <stdio.h>
#include "mss.h"

#include <string.h>
#include <limits.h>

#ifdef IS_WIN32
#include <commctrl.h>
#else
#include <commdlg.h>
#define memcpy _fmemcpy
#endif

/* @cdep pre
  $if($ispresent($BuildDir,win64),
    $DefaultsWinEXE64EMT
    $requiresbinary(mss64.lib)
  ,
    $DefaultsWinEXE
    $requiresbinary(mss32.lib)
  )
  $requires(milesply.rc)
  $requiresbinary(comdlg32.lib comctl32.lib)
*/

/* @cdep post
  $if($ispresent($BuildDir,win64),
    $BuildWinEXE64EMT
  ,
    $BuildWinEXE
  )
*/


//
// Waveform control globals
//

#define WF_MARGIN           12     
#define WF_DRAG_TOLERANCE   1
#define MAX_WF_RECORD_WIDTH 1600
#define EXTENDED_HEIGHT     128
#define WAVEFORM_DIB_HEIGHT 128

#define N_WF_SCALES       16
static S32 WF_denom [N_WF_SCALES] = { 512, 256, 128, 64, 48, 32, 24, 16, 14, 12, 10, 8, 6, 4, 2, 1 };

static HPEN   wf_trace_pen      = 0;
static HPEN   wf_cursor_pen     = 0;
static HPEN   wf_frame_pen      = 0;
static HPEN   wf_dot_frame_pen  = 0;
static HPEN   wf_loop_begin_pen = 0;
static HPEN   wf_loop_end_pen   = 0;
static HBRUSH wf_background     = 0;
static HFONT  wf_font           = 0;

static POINT point_array [2][MAX_WF_RECORD_WIDTH];
static S32   frame_array    [MAX_WF_RECORD_WIDTH];

static S32 wf_width = 0;
static S32 wf_height = 0;
static S32 waveform_view_valid = 0;

static HDC         wf_hDC;
static HBITMAP     wf_hDefaultBitmap;
static U32        *wf_lpDIBBuffer;
static BITMAPINFO *wf_pbmi;
static HBITMAP     wf_hDIB = NULL;

static S16 * waveform_buffer[2];
static S32   waveform_channels = 0;
static F32   bytes_per_sample = 0.0F;
static U8  * source_addr = NULL;
static SINTa source_bytes = 0;
static S32   output_sample_rate = 0;
static S32   samples_per_frame = 0;
static SINTa n_waveform_samples = 0;
static S32   waveform_scale = 5;
static S32   mouse_x = 0;
static S32   mouse_y = 0;
static S32   mouse_click_x = 0;
static S32   mouse_click_y = 0;
static SINTa mouse_click_pos = 0;
static S32   drag_state = FALSE;
static S32   drag_button = 0;
static S32   last_drag_button = 0;
static S32   begin_loop = -1;
static S32   end_loop = -1;

static S32    is_ASI = 0;
static S32    is_MP3 = 0;
static SINTa *frame_source_offsets;
static S32    n_valid_frames;

static S32 waveform_view_x = 0;
static S32 waveform_view_y = 0;

//
// Other globals
//

static char szAppName[] = "MILESPLY";

static char title[256]="";
static char* laststr;
static HCURSOR norm,waitcur;

static HWND hwnd=0;
static HAUDIO audio=0;
static char filename[MAX_PATH];
static S32 open=0;
static HDIGDRIVER dig;
static HMDIDRIVER mdi;
static HDLSDEVICE dls;
static HDLSFILEID DLSfile;
static void FAR*  dlsdata;
static void FAR*  filedata=0;

static S32 rate=22050;
static S32 bits=16;
static S32 chans=2;

static S32 length;
static S32 audioType;
static SINTa forceDLS=0;

static S32 filtered=1;
static S32 reverb=0;

static HWND cpuTextHwnd;
static HWND scrollHwnd;
static HWND comboHwnd;
static HWND dlsTextHwnd;
static HWND forceDLSHwnd;
static HWND midiTextHwnd;

static char DLSfilename[128];

static S32 X=100;
static S32 Y=100;
static S32 W=0;
static S32 H=0;
static RECT WR;

static UINT rate_adj    [9] = { 0,0,0,1,2,2,3,3,4};
static S32  ind_to_rate [5] = { 11025,16000,22050,32000,44100};

#define btnClose     129
#define scrollBar    132
#define cpuText      134
#define cbFilter     136
#define cbReverb     137
#define pdRate       139
#define cb16bit      135
#define cbStereo     138
#define btnHelp      145
#define btnAbout     146
#define pdForceDLS   141
#define dlsText      143
#define btnBrowseDLS 142
#define midiText     147
#define btnOpen      148
#define btnPP        149

#ifdef IS_WIN32
static char SECTION[]="Miles Sound Player";
#else
static char SECTION[]="Miles 16-bit Sound Player";
#endif

static S32 we_moved=0;

#ifdef IS_WIN32

#define COMBOCOMMANDWORD wParam

#define RADGETSCROLLPOS(hwnd) ((S32)SendMessage(hwnd,TBM_GETPOS,0,0))
#define RADSETSCROLLPOS(hwnd,val) {++we_moved; SendMessage(hwnd,TBM_SETPOS,TRUE,val); --we_moved;}
#define RADSETSCROLLRANGE(hwnd,low,high) SendMessage(hwnd,TBM_SETRANGE,FALSE,MAKELONG(low,high))

#else

#define COMBOCOMMANDWORD lParam

#define RADGETSCROLLPOS(hwnd) ((S32)GetScrollPos(hwnd,SB_CTL))
#define RADSETSCROLLPOS(hwnd,val) {++we_moved; SetScrollPos(hwnd,SB_CTL,val,TRUE); --we_moved;}
#define RADSETSCROLLRANGE(hwnd,low,high) SetScrollRange(hwnd,SB_CTL,low,high,FALSE)

#endif

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

static void close_if_open()
{
   //
   // close the library if already open
   //

   if (open) {

     stop_thread();

     if (audio) {
       AIL_quick_unload(audio);
       audio=0;
     }

     if (DLSfile) {
       AIL_DLS_unload(dls,DLSfile);
       DLSfile=0;
       AIL_mem_free_lock(dlsdata);
       dlsdata=0;
     }

     if (filedata) {
       AIL_mem_free_lock(filedata);
       filedata=0;
     }

     AIL_quick_shutdown();
     open=0;
     dls=0;
   }
}


static char FAR* get_filename(char FAR* pathname)
{
   char FAR* f=pathname+lstrlen(pathname)-1;
   while ((f>=pathname) && (*f!='\\') && (*f!=':')) --f;
   return(f+1);
}


static char FAR* extract_filename(char FAR* dest,char FAR* cmdline)
{
  char FAR* s;
  char FAR* e;

  s=cmdline;

  //check for null
  if (s==0) {
    *dest=0;
    return(0);
  }

  //skip whitespace
  while ((*s<=32) && (*s!=0))
    s++;

  //is empty?
  if (*s==0) {
    *dest=0;
    return(0);
  }

  //is it a quoted name?
  if (*s=='"') {
    //scan until we hit the end or the last quote
    e=(++s)+1;
    while ((*e!='"') && (*e!=0))
      e++;

    //copy the string
    memcpy(dest,s,e-s);
    dest[e-s]=0;

    //skip over the end quote
    if (*e=='"')
      ++e;
    goto finish;
  } else {
    //scan until we hit the end or another space
    e=s+1;
    while (*e!=0)
      e++;

    //copy the string
    memcpy(dest,s,e-s);
    dest[e-s]=0;

   finish:
    //skip over any remaining whitespace
    while ((*e<=32) && (*e!=0))
      e++;

    return(*e?e:0);
  }
}


static void handle_filenames(char FAR*cmdline)
{
  char FAR*end;

  // copy the first filename
  end=extract_filename(filename,cmdline);

  if (end) {
    char exe[MAX_PATH];
    char FAR* e;

    GetModuleFileName(0,exe,256);
    e=exe+lstrlen(exe);
    *e++=' ';

    // now launch other instances for multiple files
    while (end) {
      end=extract_filename(e,end);
      if (*e)
        WinExec(exe,SW_SHOW);
    }
  }
}


//
// get a filename to use for output
//


static char* savefilt[]=
 {
  "MP3 files (*.mp3)\0*.mp3\0All files (*.*)\0*.*\0",
  "",
  ""};


/*
//
// function to replace the current extension with a different one
//

static void replace_ext(char FAR* dest, char FAR* ext)
{
  char FAR*s;
  char FAR*e;

  s=get_filename(dest);
  e=s+lstrlen(s)-1;

  while ((e!=s) && (*e!='.'))
    --e;

  if (*e!='.') {
    e=s+lstrlen(s);
    *e='.';
  }
  strcpy(e+1,ext);
}
*/


//
// default output extensions (used in get_outname)
//

static char* savedef[]={"mp3",""};

static char dir[MAX_PATH];

static S32 get_outname(char* dest,char* title)
{
   OPENFILENAME fn;

   memset(&fn, 0, sizeof(fn));

   //
   // setup the save as window
   //

   fn.lStructSize       = sizeof(fn);
   fn.hwndOwner         = hwnd;
   fn.lpstrFilter       = savefilt[0];
   fn.nFilterIndex      = 1;
   fn.lpstrFile         = dest;
   fn.lpstrInitialDir   = NULL;
   fn.nMaxFile          = 256;
   fn.lpstrTitle        = title;
   fn.Flags             = OFN_EXPLORER |
                          OFN_LONGNAMES |
                          OFN_NOCHANGEDIR |
                          OFN_OVERWRITEPROMPT |
                          OFN_PATHMUSTEXIST |
                          OFN_HIDEREADONLY;
   fn.lpstrDefExt       = savedef[0];

  return( GetSaveFileName(&fn)?1:0 );

}

//
// Loop entire file or specified subblock
//

void update_sample_loop_block(void)
{
   if ((audio == NULL) || (audioType != AIL_QUICK_DIGITAL_TYPE))
      {
      return;
      }

   HSAMPLE S = (HSAMPLE) audio->handle;

   if (is_MP3)
      {
      if ((begin_loop != -1) && (end_loop != -1))
         {
         AIL_set_sample_loop_block(S,
                             (S32) frame_source_offsets[begin_loop],
                             (S32) frame_source_offsets[end_loop]);

         EnableWindow( GetDlgItem(hwnd,btnBrowseDLS), TRUE);
         }
      else
         {
         AIL_set_sample_loop_block(S,
                                   0,
                                  -1);

         EnableWindow( GetDlgItem(hwnd,btnBrowseDLS), FALSE);
         }
      }
   else
      {
      if ((begin_loop != -1) && (end_loop != -1))
         {
         AIL_set_sample_loop_block(S,
                                   begin_loop,
                                   end_loop);
         }
      else
         {
         AIL_set_sample_loop_block(S,
                                   0,
                                  -1);
         }
      }
}

//
// change the caption bar status and switch to an hourglass
//

static void show_slow(char* str)
{
  if (*title==0)
    GetWindowText(hwnd,title,256);
  SetWindowText(hwnd,str);
  laststr=str;
  norm=SetCursor(waitcur);
}


//
// hide the caption bar status and hourglass cursor
//

static void hide_slow()
{
  if (*title) {
    SetWindowText(hwnd,title);
    *title=0;
  }
  SetCursor(norm);
}

//
// simple error routines
//

#define show_MSS_error() show_error(AIL_last_error())
static void show_error(char* str)
{
  MessageBox(hwnd,str,"Error",MB_OK|MB_ICONERROR);
}

//
// write out a file with error reporting
//

static S32 file_write(char*name,void const FAR*addr,UINTa size)
{
  S32 result;

  show_slow("Writing...");
  result=AIL_file_write(name,addr,(U32) size);
  hide_slow();

  if (result==0)
    show_MSS_error();

  return(result);
}

/*********************************************************************/
//
// init_logfont_descriptor
//
/*********************************************************************/

#define FE_BOLD   1
#define FE_ITALIC 2

typedef S32 FONT_EFFECTS;

static void init_logfont_descriptor(LOGFONT     *lf, //)
                                    C8          *typeface_name,
                                    S32          point_size,
                                    FONT_EFFECTS effects = 0)
{
   CHARSETINFO csi;

   UINTa curSysCp = GetACP();

   /*S32 result =*/ TranslateCharsetInfo((DWORD *) curSysCp, &csi, TCI_SRCCODEPAGE);

   memset(lf, 0, sizeof(LOGFONT));

   HDC hSDC = GetDC(NULL);
   lf->lfHeight = -MulDiv(point_size, GetDeviceCaps(hSDC, LOGPIXELSY), 72);
   ReleaseDC(NULL, hSDC);

   lf->lfWidth         = 0;
   lf->lfEscapement    = 0;
   lf->lfOrientation   = 0;
   lf->lfUnderline     = FALSE;
   lf->lfStrikeOut     = FALSE;
   lf->lfCharSet       = (BYTE)csi.ciCharset;
   lf->lfOutPrecision  = OUT_DEFAULT_PRECIS;
   lf->lfClipPrecision = CLIP_DEFAULT_PRECIS;
   lf->lfQuality       = PROOF_QUALITY;
   
   lf->lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

   strcpy(lf->lfFaceName, typeface_name);

   lf->lfWeight =       (effects & FE_BOLD)   ? FW_BOLD : FW_NORMAL;
   lf->lfItalic = (S8) ((effects & FE_ITALIC) ? TRUE    : FALSE);
}

static void set_title(char FAR* pathname)
{
  char title[128];
  lstrcpy(title,"Miles Sound Player - ");
  lstrcat(title,get_filename(pathname));
  SetWindowText(hwnd,title);
}


static void set_device_strings()
{
  char desc[128];

  ShowWindow(GetDlgItem(hwnd,140), FALSE);

  if (audioType==AIL_QUICK_DIGITAL_TYPE){
    lstrcpy(desc,"Using ");
    AIL_digital_configuration(dig,0,0,desc+lstrlen(desc));
  } else if (audioType==AIL_QUICK_XMIDI_TYPE) {
    lstrcpy(desc,"Using the system MIDI device.");
    if (forceDLS==3)
      lstrcpy(desc+lstrlen(desc)-1," with Vortex DLS.");
    else if (forceDLS==4)
      lstrcpy(desc+lstrlen(desc)-1," with SonicVibes DLS.");
  } else if (audioType==AIL_QUICK_DLS_XMIDI_TYPE)
    lstrcpy(desc,"Using the Miles DLS software-synthesizer.");

  SetWindowText(midiTextHwnd,desc);
  ShowWindow(GetDlgItem(hwnd,140), TRUE);
}


static void set_reverb( void )
{
  AIL_quick_set_reverb_levels( audio, 1.0f, ( reverb ) ? 1.0f : 0.0f );
}

//############################################################################
//#                                                                          #
//# Macros to acquire bitfield data of length n from header/side or audio    #
//# buffers, n <= 24                                                         #
//#                                                                          #
//# Bit position 0 is MSB of byte 0                                          #
//#                                                                          #
//# Request for 0 bits is considered valid, returning 0                      #
//#                                                                          #
//############################################################################

#define H(n) (n ? read_bits(header_buffer, &hpos, (n)) : 0)

#ifdef IS_LE

U32 read_bits(U8 FAR *data, S32 *bitpos, S32 n)
{
   U32 val;

#if defined(IS_WIN32) && !defined(IS_WIN64)

   _asm
      {
      mov edx,bitpos
      mov ebx,data

      mov ecx,[edx]
      mov eax,ecx

      and ecx,7
      shr eax,3

      mov eax,[ebx][eax]
      bswap eax

      shl eax,cl

      mov ecx,n
      add [edx],ecx

      mov ecx,32
      sub ecx,n
      shr eax,cl
      mov val,eax
      }

#else

   S32     b = *bitpos;
   U8 FAR *p = &data[b >> 3];

   val = ((U32)(p[3]))        +
        (((U32)(p[2])) << 8)  +
        (((U32)(p[1])) << 16) +
        (((U32)(p[0])) << 24);

   val <<= b & 7;
   val >>= 32 - n;

   *bitpos = b + n;

#endif

   return val;
}

U32 view_bits8(U8 FAR *data, S32 bitpos)
{
   U32 val;

#if defined(IS_WIN32) && !defined(IS_WIN64)

   _asm
      {
      mov ecx,[bitpos]
      mov ebx,[data]
      mov eax,ecx

      and ecx,7
      shr eax,3

      mov ax,[ebx][eax]

      rol ax,cl

      movzx eax,al
      mov val,eax
      }

#else

   U8 FAR *p = &data[bitpos >> 3];

   val = ((U32)(p[1])) + (((U32)(p[0])) << 8);

   val <<= (bitpos & 7) + 16;
   val >>= 24;

#endif

   return val;
}

#else

U32 read_bits(U8 FAR *data, S32 *bitpos, S32 n)
{
   U32 val;
   S32     b = *bitpos;

   val = *(U32*)&data[b>>3];

   val <<= b & 7;
   val >>= 32 - n;

   *bitpos = b + n;

   return val;
}

U32 view_bits8(U8 FAR *data, S32 bitpos)
{
   U32 val;

   U8 FAR *p = &data[bitpos >> 3];

   val = *(U32*)&data[bitpos>>3];

   val <<= bitpos & 7;
   val >>= 32 - 8;

   return val;
}

#endif

//
// Bit rate in bits/second for [MPEG version][value]
//

const S32 MPEG_bit_rate[2][15] =
{
   { 0L,8000L,16000L,24000L,32000L,40000L,48000L,56000L,64000L,80000L,96000L,112000L,128000L,144000L,160000L         }
   ,
   { 0L,32000L,40000L,48000L,56000L,64000L,80000L,96000L,112000L,128000L,160000L,192000L,224000L,256000L,320000L     }
};

//
// Sample rate in samples/second for [MPEG25][MPEG version][value]
//

const S32 MPEG_sample_rate[2][2][4] =
{{
   { 22050L,24000L,16000L,22050L },
   { 44100L,48000L,32000L,44100L }
},
{
   { 11025L,12000L, 8000L,11025L },
   { 44100L,48000L,32000L,44100L }
}};

//############################################################################
//#                                                                          #
//# Get list of MP3 frame offsets                                            #
//#                                                                          #
//############################################################################

static void build_MP3_frame_list(HAUDIO audio)
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

//############################################################################
//##                                                                        ##
//## Use ASI interface to unpack .MP3 (which might have a .WAV wrapper)     ##
//## to raw form for waveform display                                       ## 
//##                                                                        ##
//############################################################################

DWORD WINAPI decompress_thread( LPVOID stuff )
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

      for (S32 i=0; i < waveform_channels; i++)
         {
         for (S32 j=0; j < n_samples; j += waveform_channels)
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

   n_waveform_samples = dest[0] - waveform_buffer[0];

   bytes_per_sample = (F32) (F64(source_bytes) / F64(n_waveform_samples));

   //
   // Return success
   //

   ASI_stream_close(stream);
   CloseHandle(at->thread);
   at->running = 0;
   InvalidateRect(hwnd,0,0);
   return 1;
}

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

static S32 build_ASI_waveform_buffer(HAUDIO audio, //)
                                     C8    *file_suffix)
{
   HPROVIDER ASI;
   HASISTREAM stream;

   WAVEOUT FAR *wav = (WAVEOUT FAR *) audio->data;

   stop_thread();

   Source = (U8 *) audio->data;

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
      SourceLength   = audio->size;
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

   DWORD id;
   oneasi.thread = CreateThread(0,0,decompress_thread,&oneasi,0,&id);
   Sleep(50);
   if ( oneasi.thread == 0 )
   {
     oneasi.running = 0;
   }
   
   return 1;
}

//############################################################################
//##                                                                        ##
//## Convert non-MP3-based PCM sample to displayable format                 ##
//##                                                                        ##
//############################################################################

static S32 build_PCM_waveform_buffer(HAUDIO audio)
{
   //
   // Allocate a temporary HSAMPLE so we can query the sample's properties
   //
   // (Necessary because the quick API doesn't assign an HSAMPLE to an HAUDIO
   // until it's played)
   //

   stop_thread();

   HSAMPLE S = AIL_allocate_sample_handle(dig);

   if (!AIL_set_sample_file(S,
                            audio->data,
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

      if (!AIL_WAV_info(audio->data,
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

//############################################################################
//##                                                                        ##
//## bsearch() worker function for waveform display                         ##
//##                                                                        ##
//############################################################################

static int search_frame_list(const void *keyval, const void *datum)
{
   SINTa key = *(SINTa *) keyval;

   if (key <  ((SINTa *) datum)[0]) return -1;
   if (key >= ((SINTa *) datum)[1]) return  1;

   return 0;
}

//############################################################################
//##                                                                        ##
//## Create waveform control                                                ##
//##                                                                        ##
//############################################################################

static void WF_control_create(S32 width, //)
                              S32 height)
{
   n_waveform_samples = 0;
   n_valid_frames     = 0;

   wf_width  = width;
   wf_height = height;

   if (is_MP3)
      {
      show_slow("Compiling frame data...");

      if (build_ASI_waveform_buffer(audio,".MP3"))
         {
         build_MP3_frame_list(audio);
         }
      else
         {
         hide_slow();
         return;
         }

      hide_slow();
      }
   else
      {
      show_slow("Acquiring digital audio waveform...");

      if (is_ASI)
         {
         C8 *suffix = strrchr(filename,'.');

         if (!build_ASI_waveform_buffer(audio, suffix))
            {
            hide_slow();
            return;
            }
         }
      else  
         {
         if (!build_PCM_waveform_buffer(audio))
            {
            hide_slow();
            return;
            }
         }

      hide_slow();
      }

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

//############################################################################
//##                                                                        ##
//## Destroy existing waveform control, if any                              ##
//##                                                                        ##
//############################################################################

static void WF_control_destroy(void)
{
   S32 i=0;

   stop_thread();

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

//############################################################################
//##                                                                        ##
//## Update waveform display for digital files                              ##
//##                                                                        ##
//############################################################################

static void WF_control_update(S32 wf_x0, //)
                              S32 wf_y0)
{
   S32 x,ch;
   C8 text[256] = "";

   if ((!waveform_view_valid) || (waveform_buffer[0] == NULL) || (audio == NULL) || (is_MP3 && (!n_valid_frames)))
      {
      return;
      }

   if (audioType != AIL_QUICK_DIGITAL_TYPE)
      {
      return;
      }

   HSAMPLE S = (HSAMPLE) audio->handle;

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

   if (is_MP3)
      {
      if ((begin_loop != -1) && (end_loop != -1))
         {
         EnableWindow( GetDlgItem(hwnd,btnBrowseDLS), TRUE);
         }
      else
         {
         EnableWindow( GetDlgItem(hwnd,btnBrowseDLS), FALSE);
         }
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


//############################################################################
//##                                                                        ##
//## Get a sound filename                                                   ##
//##                                                                        ##
//############################################################################

S32 get_sound_file()
{
   OPENFILENAME fn;

   memset(&fn, 0, sizeof(fn));

   fn.lStructSize       = sizeof(fn);
   fn.hwndOwner         = hwnd;
   fn.lpstrFilter       = "Sound files (*.mss;*.xmi;*.mid;*.rmi;*.wav;*.mp?)\0*.mid;*.mss;*.rmi;*.xmi;*.wav;*.mp?\0All files\0*.*\0";
   fn.nFilterIndex      = 1;
   fn.lpstrFile         = filename;
   fn.nMaxFile          = 126;
   fn.lpstrTitle        = "Select Sound File...";
   fn.Flags             = OFN_FILEMUSTEXIST |
#ifdef IS_WIN32
                          OFN_EXPLORER | OFN_LONGNAMES |
#endif
                          OFN_NOCHANGEDIR |
                          OFN_PATHMUSTEXIST |
                          OFN_HIDEREADONLY;
   fn.lpstrDefExt       = "xmi";

   return( GetOpenFileName(&fn)?1:0 );

}

static void open_file(S32 save_position)
{
   S32 usedig;
   S32 usemidi;
   S32 type;

   S32 pos=((open) && (audio) && (save_position))?AIL_quick_ms_position(audio):0;

   close_if_open();

  loadagain:

   set_title("Loading...");

   filedata=AIL_file_read(filename,FILE_READ_WITH_SIZE);
   if (filedata==0) {
    err:
     MessageBox(hwnd,AIL_last_error(),"Error opening file.",MB_OK);

     if (get_sound_file())
       goto loadagain;
     else {
       DestroyWindow(hwnd);
       return;
     }
   }

   //
   // get type of file
   //

   type=AIL_file_type_named(((U32 FAR*)filedata)+1,filename,*((U32 FAR*)filedata));

   switch (type) {
     case AILFILETYPE_PCM_WAV:
     case AILFILETYPE_ADPCM_WAV:
     case AILFILETYPE_VOC:
     case AILFILETYPE_MPEG_L3_AUDIO:
     case AILFILETYPE_OGG_VORBIS:
     case AILFILETYPE_OGG_SPEEX:
     case AILFILETYPE_V12_VOICE:
     case AILFILETYPE_V24_VOICE:
     case AILFILETYPE_V29_VOICE:
     case AILFILETYPE_S8_VOICE:
     case AILFILETYPE_S16_VOICE:
     case AILFILETYPE_S32_VOICE:
       usedig=1;
       usemidi=0;
       break;
     case AILFILETYPE_MIDI:
     case AILFILETYPE_XMIDI:
       if (forceDLS>=2)
         goto usedls;
       usemidi=1;
       usedig=0;
       break;
     case AILFILETYPE_XMIDI_DLS:
     case AILFILETYPE_XMIDI_MLS:
      usedls:
       usedig=0;
       if (forceDLS==3)
         usemidi=AIL_QUICK_MIDI_AND_VORTEX_DLS;
       else if (forceDLS==4)
         usemidi=AIL_QUICK_MIDI_AND_SONICVIBES_DLS;
       else {
         if (forceDLS==1)
           usemidi=1;
         else {
           usemidi=AIL_QUICK_DLS_ONLY;
           usedig=1;
         }
       }
       break;
     default:
       AIL_set_error("Unplayable file type.");
       AIL_mem_free_lock(filedata);
       filedata=0;
       goto err;
   }

   //
   // Open MSS
   //

   if (AIL_quick_startup(usedig,usemidi,rate,bits,chans) == 0) {

     MessageBox(hwnd,AIL_last_error(),"Couldn't open MSS.",MB_OK);

     // if we were using a custom MIDI mode, flip back normal and try again
     if ((forceDLS!=0) && (usemidi)) {
       forceDLS=0;
       SendMessage(forceDLSHwnd,CB_SETCURSEL,(UINT)forceDLS,0);
       AIL_mem_free_lock(filedata);
       goto loadagain;
     }

     DestroyWindow(hwnd);

   } else {

     //
     // mark the library as open and get the handles
     //

     open=1;
     AIL_quick_handles(&dig,&mdi,&dls);

     //
     // Set the reverb settings
     //

     if (dig)  
      {
      AIL_set_room_type( dig, EAX_ENVIRONMENT_HALLWAY ); 
      }

tryagain:

      WF_control_destroy();

      begin_loop = -1;
      end_loop   = -1;
      is_ASI     = FALSE;
      is_MP3     = FALSE;

     //
     // try to load the file
     //

     audio=AIL_quick_load_named_mem( ((U32 FAR*)filedata)+1,filename,*((U32 FAR*)filedata));

     if (audio) {

       audioType=AIL_quick_type(audio);

       //
       // Try to create a waveform control if it's a digital file
       //

      if (audioType == AIL_QUICK_MPEG_DIGITAL_TYPE)
         {
         SetWindowText(GetDlgItem(hwnd,btnBrowseDLS),"Save portion");
         ShowWindow  ( dlsTextHwnd, FALSE);

         audioType=AIL_QUICK_DIGITAL_TYPE;
         is_ASI = TRUE;
         is_MP3 = TRUE;
         }
      else
         {
         SetWindowText(GetDlgItem(hwnd,btnBrowseDLS),"GM DLS file:");
         ShowWindow(dlsTextHwnd, TRUE);

         if ( (audioType == AIL_QUICK_OGG_VORBIS_TYPE) || 
              (audioType == AIL_QUICK_OGG_SPEEX_TYPE)  ||
              (audioType == AIL_QUICK_S8_VOICE_TYPE)   || 
              (audioType == AIL_QUICK_S16_VOICE_TYPE)  || 
              (audioType == AIL_QUICK_S32_VOICE_TYPE)  || 
              (audioType == AIL_QUICK_V12_VOICE_TYPE)  || 
              (audioType == AIL_QUICK_V24_VOICE_TYPE)  || 
              (audioType == AIL_QUICK_V29_VOICE_TYPE) )
            {
            audioType=AIL_QUICK_DIGITAL_TYPE;
            is_ASI = TRUE;
            is_MP3 = FALSE;
            }
         }

      if (audioType == AIL_QUICK_DIGITAL_TYPE)
         {
         RECT rect;
         GetClientRect(hwnd, &rect);

         WF_control_create((rect.right - rect.left) - 16,
                           WAVEFORM_DIB_HEIGHT);
         }

     //
     // Expand window if we're showing a waveform view
     //

      if (waveform_view_valid)
         {
         SetWindowPos(hwnd, 
                      0,
                      (UINT) X,
                      (UINT) Y,
                      W,
                      H + EXTENDED_HEIGHT + 8,
                      SWP_NOCOPYBITS | SWP_NOZORDER);

         RECT rect;
         GetClientRect(hwnd, &rect);

         waveform_view_x = rect.left + 8;
         waveform_view_y = rect.bottom - 8 - EXTENDED_HEIGHT;
         }
      else
         {
         SetWindowPos(hwnd, 
                      0,
                      (UINT) X,
                      (UINT) Y,
                      W,
                      H,
                      SWP_NOCOPYBITS | SWP_NOZORDER);
         }

       //
       // Play/pause button works for digital types only
       //

       EnableWindow( GetDlgItem(hwnd,btnPP), (audioType == AIL_QUICK_DIGITAL_TYPE));

       //
       // did the user force DLS even for general MIDI files?
       //

       if ((audioType==AIL_QUICK_XMIDI_TYPE) && (dls) && (forceDLS>=2) && (*DLSfilename)) {

         U32 size;

         dlsdata=AIL_file_read(DLSfilename,0);
         size=AIL_file_size(DLSfilename);

         if (dlsdata==0) {

          dlserr:
           MessageBox(hwnd,AIL_last_error(),"Error opening DLS file.",MB_OK);

         } else {

           if (AIL_file_type(dlsdata,size)!=AILFILETYPE_DLS) {
             void FAR*uncomp;

             if (AIL_extract_DLS(dlsdata,size,0,0,&uncomp,0,0)==0) {
               AIL_mem_free_lock(dlsdata);
               goto dlserr;
             }

             AIL_mem_free_lock(dlsdata);
             dlsdata=uncomp;
           }

           DLSfile=AIL_DLS_load_memory(dls,dlsdata,0);
           if (DLSfile==0) {
             AIL_mem_free_lock(dlsdata);
             goto dlserr;
           } else
             audioType=AIL_QUICK_DLS_XMIDI_TYPE;
         }
       }

       //
       // if we aren't using the soft synth, change the file type
       //

       if ((audioType==AIL_QUICK_DLS_XMIDI_TYPE) && (forceDLS!=2) && ((forceDLS!=0) || (dls==0)))
         audioType=AIL_QUICK_XMIDI_TYPE;

       //
       // if no dls, then disable the inappropriate controls
       //

       if (audioType==AIL_QUICK_XMIDI_TYPE)
         SetWindowText(cpuTextHwnd,"CPU: SysMon");

       EnableWindow( GetDlgItem(hwnd,cbFilter), (audioType==AIL_QUICK_XMIDI_TYPE)?0:1 );
       EnableWindow( GetDlgItem(hwnd,cbReverb), (audioType==AIL_QUICK_XMIDI_TYPE)?0:1 );

       EnableWindow( GetDlgItem(hwnd,cb16bit), (audioType==AIL_QUICK_XMIDI_TYPE)?0:1 );
       EnableWindow( GetDlgItem(hwnd,cbStereo), (audioType==AIL_QUICK_XMIDI_TYPE)?0:1 );
       EnableWindow( comboHwnd, (audioType==AIL_QUICK_XMIDI_TYPE)?0:1 );

       EnableWindow( forceDLSHwnd, (audioType==AIL_QUICK_DIGITAL_TYPE)?0:1 );
       EnableWindow( dlsTextHwnd, ((audioType!=AIL_QUICK_DIGITAL_TYPE) && (forceDLS>=2))?1:0 );
       EnableWindow( GetDlgItem(hwnd,btnBrowseDLS), ((audioType!=AIL_QUICK_DIGITAL_TYPE) && (forceDLS>=2))?1:0 );

       //
       // Set device strings
       //

       set_device_strings();

       //
       // set the initial reverb
       //

       set_reverb();

       //
       // start the playback
       //

       update_sample_loop_block();
       AIL_quick_play(audio,0);

       set_title(filename);

       length=AIL_quick_ms_length( audio );

       if (pos)
         AIL_quick_set_ms_position(audio,pos);

     } else {

       MessageBox(hwnd,AIL_last_error(),"Error opening file.",MB_OK);

       if (get_sound_file())
         goto tryagain;
       else
         DestroyWindow(hwnd);

     }

   }

   if (*DLSfilename) {
     SetWindowText(dlsTextHwnd,get_filename(DLSfilename));
   } else
     SetWindowText(dlsTextHwnd,"None selected.");
}

//############################################################################
//##                                                                        ##
//## Get a DLS filename                                                     ##
//##                                                                        ##
//############################################################################

void get_DLS_file()
{
   OPENFILENAME fn;
   char         fn_buff[128];

   memset(&fn, 0, sizeof(fn));
   lstrcpy(fn_buff,DLSfilename);

   fn.lStructSize       = sizeof(fn);
   fn.hwndOwner         = hwnd;
   fn.lpstrFilter       = "Downloadable sample files (*.dls;*.mls)\0*.dls;*.mls\0All files\0*.*\0";
   fn.nFilterIndex      = 1;
   fn.lpstrFile         = fn_buff;
   fn.nMaxFile          = sizeof(fn_buff);
   fn.lpstrTitle        = "Select DLS File...";
   fn.Flags             = OFN_FILEMUSTEXIST |
#ifdef IS_WIN32
                          OFN_EXPLORER | OFN_LONGNAMES |
#endif
                          OFN_NOCHANGEDIR |
                          OFN_PATHMUSTEXIST |
                          OFN_HIDEREADONLY;
   fn.lpstrDefExt       = "dls";


   if (GetOpenFileName(&fn))
      {
      lstrcpy(DLSfilename,fn_buff);
      open_file(1);
      }
}

//############################################################################
//##                                                                        ##
//## Show the help window                                                   ##
//##                                                                        ##
//############################################################################

void show_help()
{
   MessageBox(hwnd,

   "Thank you for using RAD Game Tools!\n\n"
   "The Miles player can play MP3, XMIDI, MIDI, or wave files. Simply choose\n"
   "the file you'd like to play, or drop the file onto the player icon.\n\n"
   "Controls:\n"
   "\tCPU - how much CPU time the DLS synthesizer is using.\n\n"
   "\tFilter - this lets you turn on and off sound filtering in the DLS\n"
   "\tsoftware synthesizer and the digital mixer. Filtering takes a little\n"
   "\textra CPU time (so little that it should usually just be left on).\n\n"
   "\tReverb - this lets you turn on and off reverb in the DLS software\n"
   "\tsynthesizer and digital mixer. Reverb takes extra CPU time.\n\n"
   "\tRate - this lets you set the rate to use for digital output (both\n"
   "\tfor wave file and the DLS soft-synth). The faster rates require\n"
   "\tmore CPU time for the DLS software synthesizer.\n\n"
   "\t16-bit - this lets you set the output quality to 16-bit. 16-bit\n"
   "\tsound is a little slower for the software synthesizer to build.\n\n"
   "\tStereo - this lets you set the output to stereo. Stereo sound is\n"
   "\tslower for the software synthesizer to build.\n\n"
   "\tMIDI Device - this option lets you choose the MIDI output\n"
   "\tdevice.  Your choices are: 1) auto-choose, that is, play\n"
   "\tplain MIDI files on the MIDI hardware, and DLS MIDI files\n"
   "\ton the software synthesizer;  2) always use the MIDI\n"
   "\thardware; 3) always use the software synthesizer; 4) use the\n"
   "\tAureal Vortex DLS hardware; and, 4) use the S3 SonicVibes\n"
   "\tDLS hardware.\n\n"
   "\tIf you choose to always use the soft-synth, then you must\n"
   "\talso choose a DLS file for playing plain MIDI files. You can\n"
   "\tobtain a Roland (TM) General MIDI DLS file (called GM.DLS)\n"
   "\tby downloading Microsoft's 'Interactive Music Control'\n"
   "\t(available on the Microsoft web site)."
   "\n\n"
   "\tLoops within digital audio files (PCM, ADPCM, and MP3) may be\n"
   "\tauditioned in the waveform display.  Use the Play/Pause button\n"
   "\tto stop playback, then left-click in the waveform display to\n"
   "\tplace a green marker that designates the beginning of a loop.\n"
   "\tRight-click to place the red end-of-loop marker.  An existing \n"
   "\tloop may be removed by clicking on one of its markers.\n"
   "\tThe displayed loop boundaries are suitable for passing directly\n"
   "\tto AIL_set_sample_loop_block().\n\n"
   "\tYou may also drag the waveform display -- or zoom it with the\n"
   "\tmouse wheel -- without affecting any existing marker positions.\n\n"
   "\tNote that loop points in MP3 files may be placed only at frame\n"
   "\tboundaries.  For best results, we recommend creating looped MP3\n"
   "\tfiles with an encoder that allows the bit reservoir to be disabled,\n"
   "\tsuch as LAME with the --nores option."

   ,"Help...", MB_OK);
}


//############################################################################
//##                                                                        ##
//## Show the about window                                                  ##
//##                                                                        ##
//############################################################################

void show_about()
{
   char text[1024];
   char version[8];
   AIL_MSS_version(version,8);
   lstrcpy(text,"Version ");
   lstrcat(text,version);
   lstrcat(text," " MSS_COPYRIGHT "\n\n"
                "Miles Sound System XMIDI, MIDI, MPEG and wave player.");

   if (dig) {
     lstrcat(text,"\n\nDigital driver: ");
     AIL_digital_configuration(dig,0,0,text+lstrlen(text));
   }

   lstrcat(text,"\n\nFor questions or comments, please contact RAD Game Tools at:\n\n"
               "\tRAD Game Tools\n"
               "\t401 Park Place - Suite 103\n"
               "\tKirkland, WA  98033\n"
               "\t425-893-4300\n"
               "\tFAX: 425-609-2463\n\n"
               "\tWeb: http://www.radgametools.com\n"
               "\tE-mail: sales3@radgametools.com");
   MessageBox(hwnd,text,"About the Miles Sound Player...", MB_OK);
}

//############################################################################
//##                                                                        ##
//## Main window procedure                                                  ##
//##                                                                        ##
//############################################################################

LRESULT AILEXPORT WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   char buffer[128];
   S32 i;

   switch (message)
      {
      case WM_COMMAND:

        switch (LOWORD(wParam))
        {
          case btnOpen:
            if (get_sound_file())
              open_file(0);
            break;

          case btnPP:
            {
            if (audio != NULL)
               {
               HSAMPLE S = (HSAMPLE) audio->handle;

               if (AIL_sample_status(S) == SMP_PLAYING)
                  {
                  AIL_stop_sample(S);
                  }
               else
                  {
                  update_sample_loop_block();
                  AIL_resume_sample(S);
                  }
               }
            break;
            }

          case btnHelp:
            show_help();
            break;

          case btnAbout:
            show_about();
            break;

          case btnBrowseDLS:
            
            if ((is_MP3) && (begin_loop != -1) && (end_loop != -1))
               {
               char each[MAX_PATH] = "";

               if (get_outname(each,"Enter the filename of the new .MP3 file to save")) 
                  {
                  file_write(each, 
                            &((U8 *) audio->data)[frame_source_offsets[begin_loop]], 
                             frame_source_offsets[end_loop] - frame_source_offsets[begin_loop]);
                  }
               }
            else
               {
               get_DLS_file();
               }
            break;

          case IDCANCEL:
          case btnClose:
            DestroyWindow(hwnd);
            break;

          case cb16bit:
            bits=(bits==8)?16:8;
            open_file(1);
            break;

          case cbStereo:
            chans=(chans==1)?2:1;
            open_file(1);
            break;

          case cbFilter:
            filtered=!filtered;
            AIL_set_preference(DLS_ENABLE_FILTERING,filtered);
            AIL_set_preference(DIG_ENABLE_RESAMPLE_FILTER,filtered);
            break;

          case cbReverb:
            reverb=!reverb;
            set_reverb();
            break;

          case pdForceDLS:
            if (HIWORD(COMBOCOMMANDWORD) == CBN_SELENDOK) {
              forceDLS=SendMessage(forceDLSHwnd,CB_GETCURSEL,0,0);
              EnableWindow( dlsTextHwnd, (forceDLS==2)?1:0 );
              EnableWindow( GetDlgItem(hwnd,btnBrowseDLS), (forceDLS==2)?1:0 );
              if (forceDLS==2) {
                if (*DLSfilename)
                  open_file(1);
              } else
                open_file(1);
            }
            break;

          case pdRate:
            if (HIWORD(COMBOCOMMANDWORD) == CBN_SELENDOK) {
              i=ind_to_rate[SendMessage(comboHwnd,CB_GETCURSEL,0,0)];
              if (i!=rate) {
                rate=i;
                open_file(1);
              }
            }
            break;

        }
        return 0;

      case WM_MOUSEMOVE:
         {
         S32 x = LOWORD(lParam);
         S32 y = HIWORD(lParam);

         mouse_x = x;
         mouse_y = y;
         break;
         }

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

      case WM_HSCROLL:
#ifdef IS_WIN32
        if (!we_moved)
          AIL_quick_set_ms_position(audio,(RADGETSCROLLPOS(scrollHwnd)*length)/100L);
#else
        {  // have to handle 16-bit scroll bars manually
        S32 n=RADGETSCROLLPOS(scrollHwnd);
        switch (LOWORD(wParam)) {
          case SB_THUMBPOSITION:
          case SB_THUMBTRACK:
            n=LOWORD(lParam);
            break;
          case SB_BOTTOM:
          case SB_PAGEDOWN:
          case SB_LINEDOWN:
            n++;
            break;
          case SB_TOP:
          case SB_PAGEUP:
          case SB_LINEUP:
            n--;
            break;
        }

        if (n<0) n=0; else if (n>100) n=100;

        RADSETSCROLLPOS(scrollHwnd,(int)n);
        AIL_quick_set_ms_position(audio,(n*length)/100L);
        }
#endif
        break;

      case WM_TIMER:

        //
        // update the cpu hit
        //

//        update_waveform();

        if ((dls) && (audioType==AIL_QUICK_DLS_XMIDI_TYPE)) {
          AIL_DLS_get_info(dls,0,&i);
          wsprintf(buffer,"CPU: %i%%",i);
          SetWindowText(cpuTextHwnd,buffer);
        } else
          if ((dig) && (audioType==AIL_QUICK_DIGITAL_TYPE)) {
            wsprintf(buffer,"CPU: %i%%",AIL_digital_CPU_percent(dig));
            SetWindowText(cpuTextHwnd,buffer);
          }

        //
        // update the position
        //

        if (audio) {
          i=AIL_quick_ms_position(audio);
          if (length)
            RADSETSCROLLPOS(scrollHwnd,(UINT)(i*100L/length));
        }
        break;

      case WM_WINDOWPOSCHANGED:
        if (!IsIconic(hwnd)) {
          X=((LPWINDOWPOS)lParam)->x;
          Y=((LPWINDOWPOS)lParam)->y;
        }
        break;

      case WM_SETFOCUS:    // deal with the focus in this weird dialog-window
        SetFocus(GetDlgItem(hwnd,btnClose));
        break;

      case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

#ifndef IS_WIN32
      case WM_ENDSESSION:   // for 16-bit windows
        close_if_open();
        break;

      case WM_CTLCOLOR:
        if (HIWORD(lParam)!=CTLCOLOR_LISTBOX) {
          SetBkColor((HDC)wParam,RGB(192,192,192));
          return((LRESULT)GetStockObject(LTGRAY_BRUSH));
        } else
          break;
#endif

      case WM_PAINT:
         {
         WF_control_update(waveform_view_x, waveform_view_y);
         break;
         }
      }

   return DefWindowProc(hwnd,message,wParam,lParam);
}


static void WriteProfileInt(char FAR* sec,char FAR* key,U32 value)
{
  char buf[16];
  wsprintf(buf,"%i",value);
  WriteProfileString(sec,key,buf);
}

//############################################################################
//##                                                                        ##
//## WinMain()                                                              ##
//##                                                                        ##
//############################################################################

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpszCmdLine, int nCmdShow)
{
   MSG      msg;
   WNDCLASS wndclass;

   waitcur=LoadCursor(0,IDC_WAIT);

   //
   // set the redist directory and statup the system
   //

   AIL_set_redist_directory( MSS_DIR_UP_TWO MSS_REDIST_DIR_NAME );

   AIL_startup();

   //
   // create the window class
   //

   if (!hPrevInstance)
      {
      wndclass.lpszClassName = szAppName;
      wndclass.lpfnWndProc   = (WNDPROC) WndProc;
      wndclass.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
      wndclass.hInstance     = hInstance;
      wndclass.hIcon         = LoadIcon(hInstance,szAppName);
      wndclass.hCursor       = LoadCursor(0,IDC_ARROW);
#ifdef _WIN32
      wndclass.hbrBackground = (HBRUSH)(COLOR_3DFACE+1);
#else
      wndclass.hbrBackground = GetStockObject(LTGRAY_BRUSH);
#endif
      wndclass.cbClsExtra    = 0;
      wndclass.cbWndExtra    = DLGWINDOWEXTRA;
      wndclass.lpszMenuName  = NULL;

      RegisterClass(&wndclass);
      }

   hwnd = CreateDialog(hInstance,szAppName,0,NULL);

   if (hwnd==0) {
     MessageBox(0,"Couldn't create dialog box.","Error",MB_OK);
     AIL_quick_shutdown();
     return(0);
   }

   //
   // read the INI file
   //

   filtered=GetProfileInt(SECTION,"Filtered",(UINT)filtered);
   reverb=GetProfileInt(SECTION,"Reverb",(UINT)reverb);
   rate=GetProfileInt(SECTION,"Rate",(UINT)rate);
   bits=GetProfileInt(SECTION,"Bits",(UINT)bits);
   chans=GetProfileInt(SECTION,"Channels",(UINT)chans);
   forceDLS=GetProfileInt(SECTION,"ForceDLS",(UINT)forceDLS);
   GetProfileString(SECTION,"DLSFilename","",DLSfilename,128);
   X=GetProfileInt(SECTION,"X",(UINT)X);
   Y=GetProfileInt(SECTION,"Y",(UINT)Y);


   //
   // Main message loop
   //

   SetWindowPos(hwnd,0,(UINT)X,(UINT)Y,0,0,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOSIZE);
   ShowWindow(hwnd,nCmdShow);

   GetWindowRect(hwnd, &WR);
   W = WR.right  - WR.left + 1;
   H = WR.bottom - WR.top  + 1;

#ifdef IS_WIN32
   InitCommonControls();
#endif

   //
   // copy the filename and if blank, ask for one
   //

   handle_filenames(lpszCmdLine);

   if (*filename==0) {
     get_sound_file();
     if (*filename==0)
       return(0);
   }


   //
   // setup the dialog
   //

   cpuTextHwnd=GetDlgItem(hwnd,cpuText);
   scrollHwnd=GetDlgItem(hwnd,scrollBar);
   comboHwnd=GetDlgItem(hwnd,pdRate);
   dlsTextHwnd=GetDlgItem(hwnd,dlsText);
   forceDLSHwnd=GetDlgItem(hwnd,pdForceDLS);
   midiTextHwnd=GetDlgItem(hwnd,midiText);

   SendMessage(comboHwnd,CB_ADDSTRING,0,(UINTa)(char FAR*)"44 Khz");
   SendMessage(comboHwnd,CB_ADDSTRING,0,(UINTa)(char FAR*)"32 Khz");
   SendMessage(comboHwnd,CB_ADDSTRING,0,(UINTa)(char FAR*)"22 Khz");
   SendMessage(comboHwnd,CB_ADDSTRING,0,(UINTa)(char FAR*)"16 Khz");
   SendMessage(comboHwnd,CB_ADDSTRING,0,(UINTa)(char FAR*)"11 Khz");
   SendMessage(comboHwnd,CB_SETCURSEL,rate_adj[rate/5000],0);

   SendMessage(forceDLSHwnd,CB_ADDSTRING,0,(UINTa)(char FAR*)"Auto-choose MIDI device.");
   SendMessage(forceDLSHwnd,CB_ADDSTRING,0,(UINTa)(char FAR*)"Use MIDI hardware.");
   SendMessage(forceDLSHwnd,CB_ADDSTRING,0,(UINTa)(char FAR*)"Use Miles software synth.");
   SendMessage(forceDLSHwnd,CB_ADDSTRING,0,(UINTa)(char FAR*)"Use Vortex DLS hardware.");
   SendMessage(forceDLSHwnd,CB_ADDSTRING,0,(UINTa)(char FAR*)"Use SonicVibes DLS hardware.");
   SendMessage(forceDLSHwnd,CB_SETCURSEL,(UINTa)forceDLS,0);

   if (filtered)
     SendMessage(GetDlgItem(hwnd,cbFilter),BM_SETCHECK,1,0);

   if (reverb)
     SendMessage(GetDlgItem(hwnd,cbReverb),BM_SETCHECK,1,0);

   if (bits==16)
     SendMessage(GetDlgItem(hwnd,cb16bit),BM_SETCHECK,1,0);

   if (chans==2)
     SendMessage(GetDlgItem(hwnd,cbStereo),BM_SETCHECK,1,0);

   if (forceDLS!=2) {
     EnableWindow( dlsTextHwnd, 0 );
     EnableWindow( GetDlgItem(hwnd,btnBrowseDLS), 0 );
   }

   RADSETSCROLLRANGE(scrollHwnd,0,100);

   UpdateWindow(hwnd);

   //
   // open the system and the MIDI file
   //

   open_file(0);

   //
   // create a timer to update the position and CPU usage
   //

   SetTimer(hwnd,1,250,0);

   //
   // start the message loop
   //

   SetFocus(GetDlgItem(hwnd,btnClose));

   while (GetMessage(&msg, 0, 0, 0)) {
     if (!IsDialogMessage(hwnd,&msg)) {
       TranslateMessage(&msg);
       DispatchMessage(&msg);
     }

     Sleep(10);
     WF_control_update(waveform_view_x, waveform_view_y);

   }

   WF_control_destroy();

   //
   // write the INI file
   //

   WriteProfileInt(SECTION,"Filtered",filtered);
   WriteProfileInt(SECTION,"Reverb",reverb);
   WriteProfileInt(SECTION,"Rate",rate);
   WriteProfileInt(SECTION,"Bits",bits);
   WriteProfileInt(SECTION,"Channels",chans);
   WriteProfileInt(SECTION,"ForceDLS",(U32) forceDLS);
   WriteProfileString(SECTION,"DLSFilename",DLSfilename);
   WriteProfileString(SECTION,"DLSFilename",DLSfilename);
   WriteProfileInt(SECTION,"X",X);
   WriteProfileInt(SECTION,"Y",Y);

   close_if_open();

   AIL_shutdown();

   return (int) msg.wParam;
}

