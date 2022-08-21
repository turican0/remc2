//############################################################################
//##                                                                        ##
//##  Miles Sound System                                                    ##
//##                                                                        ##
//##  WAIL.H: API function prototypes                                       ##
//##                                                                        ##
//##  16-bit protected-mode source compatible with MSC 7.0                  ##
//##                                                                        ##
//##  Version 1.00 of 15-Feb-95: Initial, derived from AIL.H V3.02          ##
//##          1.01 of 19-Jun-95: Added various functions for V3.03 release  ##
//##          1.02 of 22-Nov-95: C++ typedef problem fixed, declspecs added ##
//##          1.03 of 15-Feb-96: Changes for 16 bit callbacks and multiple  ##
//##                             16 bit DLL loads (JKR)                     ##
//##                                                                        ##
//##  Author: John Miles                                                    ##
//##                                                                        ##
//############################################################################
//##                                                                        ##
//##  Copyright (C) 1994-1996 Miles Design, Inc.                            ##
//##                                                                        ##
//##  Contact RAD Software at 801-322-4300 for technical support.           ##
//##                                                                        ##
//############################################################################

#ifndef WAIL_VERSION

#define WAIL_VERSION      "3.50"
#define WAIL_VERSION_DATE "22-Mar-96"

#endif

#ifndef WAIL_H
#define WAIL_H

#include <windows.h>
#include <mmsystem.h>

#ifdef WIN32
  #define IS_WIN32 1
#endif

#ifdef _WIN32
  #define IS_WIN32 1
#endif

#ifdef __cplusplus
extern "C" {
#endif

//
// If compiling WAIL32 library, use __declspec(dllexport) for both 
// declarations and definitions
//
// If compiling WAIL32 application, use __declspec(dllimport) for
// declarations -- definitions don't matter
//
// If compiling WAIL16 library or application, use "extern" in declarations 
// and nothing in definitions
//

#ifdef IS_WIN32

  #define AILEXPORT WINAPI

  #ifdef BUILD_WAIL
    #define DXDEC __declspec(dllexport)
    #define DXDEF __declspec(dllexport)
  #else
    #define DXDEC __declspec(dllimport)
  #endif

#else

  #define AILEXPORT __export WINAPI

  #define DXDEC  extern
  #define DXDEF

#endif

typedef LPVOID AILLPDIRECTSOUND;
typedef LPVOID AILLPDIRECTSOUNDBUFFER;

//
// Misc. constant definitions  
//

#define MAX_DRVRS                16       // Max. # of simultaneous drivers
#define MAX_TIMERS               16       // Max. # of simultaneous timers
#define MAX_NOTES                32       // Max # of notes "on" 
#define FOR_NEST                 4        // # of nested XMIDI FOR loops
#define NUM_CHANS                16       // # of possible MIDI channels
#define MAX_W_VOICES             16       // Max virtual wave synth voice cnt
#define MAX_W_ENTRIES            512      // 512 wave library entries max.

#define MIN_CHAN                 ( 1-1)   // Min channel recognized (0-based)
#define MAX_CHAN                 (16-1)   // Max channel recognized
#define MIN_LOCK_CHAN            ( 2-1)   // Min channel available for locking
#define MAX_LOCK_CHAN            ( 9-1)   // Max channel available for locking
#define PERCUSS_CHAN             (10-1)   // Percussion channel (no locking)

#define DIG_F_MONO_8             0        // PCM data formats
#define DIG_F_MONO_16            1
#define DIG_F_STEREO_8           2
#define DIG_F_STEREO_16          3

#define DIG_F_16BITS_MASK        1
#define DIG_F_STEREO_MASK        2

#define DIG_PCM_SIGN             0x0001   // PCM flags
#define DIG_PCM_ORDER            0x0002

#define DIG_PCM_POLARITY         0x0004   // PCM flags used by driver hardware
#define DIG_PCM_SPLIT            0x0008
#define DIG_BUFFER_SERVICE       0x0010
#define DIG_DUAL_DMA             0x0020   
#define DIG_RECORDING_SUPPORTED  0x8000

//
// Non-specific XMIDI/MIDI controllers and event types
//

#define CHAN_MUTE         107
#define CALLBACK_PFX      108
#define SEQ_BRANCH        109
#define CHAN_LOCK         110
#define CHAN_PROTECT      111
#define VOICE_PROTECT     112
#define TIMBRE_PROTECT    113
#define PATCH_BANK_SEL    114
#define INDIRECT_C_PFX    115
#define FOR_LOOP          116
#define NEXT_LOOP         117
#define CLEAR_BEAT_BAR    118
#define CALLBACK_TRIG     119
#define SEQ_INDEX         120

#define MODULATION        1
#define DATA_MSB          6
#define PART_VOLUME       7
#define PANPOT            10
#define EXPRESSION        11
#define DATA_LSB          38
#define SUSTAIN           64
#define REVERB            91
#define CHORUS            93
#define RPN_LSB           100
#define RPN_MSB           101
#define ALL_NOTES_OFF     123

#define EV_NOTE_OFF       0x80
#define EV_NOTE_ON        0x90
#define EV_POLY_PRESS     0xa0
#define EV_CONTROL        0xb0
#define EV_PROGRAM        0xc0
#define EV_CHAN_PRESS     0xd0
#define EV_PITCH          0xe0
#define EV_SYSEX          0xf0
#define EV_ESC            0xf7
#define EV_META           0xff

#define META_EOT          0x2f
#define META_TEMPO        0x51
#define META_TIME_SIG     0x58

//
// SAMPLE.system_data[] usage
//

#define SSD_EOD_CALLBACK  0  // Application end-of-data callback if not NULL
#define VOC_BLK_PTR       1  // Pointer to current block                                   
#define VOC_REP_BLK       2  // Pointer to beginning of repeat loop block                  
#define VOC_N_REPS        3  // # of iterations left in repeat loop
#define VOC_MARKER        4  // Marker to search for, or -1 if all
#define VOC_MARKER_FOUND  5  // Desired marker found if 1, else 0
#define SSD_RELEASE       6  // Release sample handle upon termination if >0
#define SSD_EOD_CB_WIN32S 7  // Application end-of-data callback is in Win32s

//
// Timer status values
//

#define AILT_FREE         0         // Timer handle is free for allocation
#define AILT_STOPPED      1         // Timer is stopped
#define AILT_RUNNING      2         // Timer is running

//
// SAMPLE.status flag values
//

#define SMP_FREE          0x0001    // Sample is available for allocation

#define SMP_DONE          0x0002    // Sample has finished playing, or has 
                                    // never been started

#define SMP_PLAYING       0x0004    // Sample is playing

#define SMP_STOPPED       0x0008    // Sample has been stopped

//
// SEQUENCE.status flag values
//

#define SEQ_FREE          0x0001    // Sequence is available for allocation

#define SEQ_DONE          0x0002    // Sequence has finished playing, or has 
                                    // never been started

#define SEQ_PLAYING       0x0004    // Sequence is playing

#define SEQ_STOPPED       0x0008    // Sequence has been stopped

//
// AIL_set_direct_buffer_control() command values
//

#define AILDS_RELINQUISH  0         // App returns control of secondary buffer
#define AILDS_SEIZE       1         // App takes control of secondary buffer
#define AILDS_SEIZE_LOOP  2         // App wishes to loop the secondary buffer

//
// General type definitions for portability
// 

#ifndef TYPEDEFS
#define TYPEDEFS

typedef unsigned char  U8   ;
typedef unsigned short U16  ;
typedef unsigned long  U32  ;
typedef          char  S8  ;
typedef signed   short S16 ;
typedef signed   long  S32 ;

typedef U32      REALFAR;           // Real-mode far pointer (16:16)

#endif

#ifndef FILE_ERRS
#define FILE_ERRS

#define WAIL_NO_ERROR        0
#define WAIL_IO_ERROR        1
#define WAIL_OUT_OF_MEMORY   2
#define WAIL_FILE_NOT_FOUND  3
#define WAIL_CANT_WRITE_FILE 4
#define WAIL_CANT_READ_FILE  5
#define WAIL_DISK_FULL       6

#endif

#define MIN_VAL 0
#define NOM_VAL 1
#define MAX_VAL 2

#ifndef YES
#define YES 1
#endif

#ifndef NO
#define NO  0
#endif

//
// Preference names and default values
//

#define DIG_MIXER_CHANNELS         0     
#define DEFAULT_DMC                16     // 16 allocatable SAMPLE structures

#define DIG_DEFAULT_VOLUME         1   
#define DEFAULT_DDV                100    // Default sample volume = 100 (0-127)

#define DIG_RESAMPLING_TOLERANCE   2
#define DEFAULT_DRT                655    // Resampling triggered at +/- 1%

#define DIG_OUTPUT_BUFFER_SIZE     3    
#define DEFAULT_DOBS               49152  // 48K output buffer size
                                  
#define MDI_SERVICE_RATE           4
#define DEFAULT_MSR                120    // XMIDI sequencer timing = 120 Hz

#define MDI_SEQUENCES              5
#define DEFAULT_MS                 8      // 8 sequence handles/driver

#define MDI_DEFAULT_VOLUME         6
#define DEFAULT_MDV                127    // Default sequence volume = 127 (0-127)

#define MDI_QUANT_ADVANCE          7
#define DEFAULT_MQA                1      // Beat/bar count +1 interval

#define MDI_ALLOW_LOOP_BRANCHING   8
#define DEFAULT_ALB                NO     // Branches cancel XMIDI FOR loops

#define MDI_DEFAULT_BEND_RANGE     9
#define DEFAULT_MDBR               2      // Default pitch-bend range = 2

#define MDI_SYSEX_BUFFER_SIZE      10   
#define DEFAULT_MSBS               256    // Default sysex buffer = 512 bytes

#define AIL_MM_PERIOD              11
#define DEFAULT_AMP                5      // Default MM timer period = 5 msec.

#define AIL_TIMERS                 12
#define DEFAULT_AT                 16     // 16 allocatable HTIMER handles

#define DIG_MIN_CHAIN_ELEMENT_SIZE 13
#define DEFAULT_MCES               2048   // 2048 bytes/waveOut buffer 

#define DIG_DS_SECONDARY_SIZE      14    
#define DEFAULT_DDSS               32768  // Must be 2^n -- use 32K by default

#define DIG_USE_WAVEOUT            15
#define DEFAULT_DUW                NO     // Use DirectSound by default

#define DIG_DS_SAMPLE_CEILING      16
#define DEFAULT_DDSC               44100  // Allow up to 44 kHz samples

#define AIL_LOCK_PROTECTION        17     
#define DEFAULT_ALP                YES    // Suspend foreground thread by default

#define MDI_DOUBLE_NOTE_OFF        18
#define DEFAULT_MDNO               NO     // For stuck notes on SB daughterboards

#define AIL_BOOST_TIMER_PRIORITY   19
#define DEFAULT_ABTP               NO     // Leave timer unboosted

#define AIL_WIN32S_CALLBACK_SIZE   20
#define DEFAULT_WCS                4096   // Size of callback data in bytes

#define N_PREFS 21                        // # of preference types

//
// Type definitions
//

struct _DIG_DRIVER;

struct _MDI_DRIVER;

typedef struct _DIG_DRIVER FAR * HDIGDRIVER;    // Handle to digital driver

typedef struct _MDI_DRIVER FAR * HMDIDRIVER;    // Handle to XMIDI driver

typedef struct _SAMPLE FAR * HSAMPLE;           // Handle to sample

typedef struct _SEQUENCE FAR * HSEQUENCE;       // Handle to sequence

typedef S32 HTIMER;                             // Handle to timer


//
// Function pointer types
//

typedef void (AILEXPORT FAR* AILTIMERCB)    (U32 user);

typedef void (AILEXPORT FAR* AILSAMPLECB)   (HSAMPLE sample);

typedef S32  (AILEXPORT FAR* AILEVENTCB)    (HMDIDRIVER hmi,HSEQUENCE seq,S32 status,S32 data_1,S32 data_2);

typedef S32  (AILEXPORT FAR* AILTIMBRECB)   (HMDIDRIVER hmi,S32 bank,S32 patch);

typedef S32  (AILEXPORT FAR* AILPREFIXCB)   (HSEQUENCE seq,S32 log,S32 data);

typedef void (AILEXPORT FAR* AILTRIGGERCB)  (HSEQUENCE seq,S32 log,S32 data);

typedef void (AILEXPORT FAR* AILBEATCB)     (HMDIDRIVER hmi,HSEQUENCE seq,S32 beat,S32 measure);

typedef void (AILEXPORT FAR* AILSEQUENCECB) (HSEQUENCE seq);



struct _AILTIMER                 // Timer instance
{
   U32      status;
   AILTIMERCB callback;
   U32      user;

   S32      elapsed;
   S32      value;
   S32      callingCT;           // Calling EXE's task number (16 bit only)
   S32      callingDS;           // Calling EXE's DS (used in 16 bit only)
   S32      IsWin32s;            // Is this a Win32s callback
};

struct _SAMPLE                   // Sample instance
{
   char     tag[4];              // HSAM

   HDIGDRIVER driver;            // Driver for playback

   U32      status;              // SMP_ flags: _FREE, _DONE, _PLAYING

   void FAR *start[2];           // Sample buffer address (W)
   U32       len  [2];           // Sample buffer size in bytes (W)
   U32       pos  [2];           // Index to next byte (R/W)
   U32       done [2];           // Nonzero if buffer with len=0 sent by app

   S32      current_buffer;      // Buffer # active (0/1)
   S32      last_buffer;         // Last active buffer (for double-buffering)
   S32      starved;             // Buffer stream has run out of data

   S32      loop_count;          // # of cycles-1 (1=one-shot, 0=indefinite)
   S32      loop_start;          // Starting offset of loop block (0=SOF)
   S32      loop_end;            // End offset of loop block (-1=EOF)
          
   S32      format;              // DIG_F format (8/16 bits, mono/stereo)
   U32      flags;               // DIG_PCM_SIGN / DIG_PCM_ORDER (stereo only)
                                 
   S32      playback_rate;       // Playback rate in hertz
                                 
   S32      volume;              // Sample volume 0-127
   S32      pan;                 // Mono panpot/stereo balance (0=L ... 127=R)

   S32      vol_scale[2][256];   // [left/mono=0,right=1][256] channel volume

   AILSAMPLECB  SOB;             // Start-of-block callback function
   AILSAMPLECB  EOB;             // End-of-buffer callback function
   AILSAMPLECB  EOS;             // End-of-sample callback function

   S32      SOB_IsWin32s;        // Is this a Win32s callback
   S32      EOB_IsWin32s;        // Is this a Win32s callback
   S32      EOS_IsWin32s;        // Is this a Win32s callback

   S32      user_data  [8];      // Miscellaneous user data
   S32      system_data[8];      // Miscellaneous system data

   //
   // DirectSound-specific data
   //

   S32      secondary_buffer;    // Secondary buffer index

   S32      service_interval;    // Service sample every n ms 
   S32      service_tick;        // Current service countdown value
   S32      service_type;        // 1 if single-buffered; 2 if streamed
   S32      buffer_segment_size; // Buffer segment size to fill

   S32      prev_segment;        // Previous segment # (0...n)
   S32      prev_cursor;         // Previous play cursor location

   S32      bytes_remaining;     // # of bytes left to play (if not -1)

   S32      direct_control;      // 1 if app controls buffer, 0 if WAIL
};

typedef struct _DIG_DRIVER          // Handle to digital audio driver
{
   char     tag[4];                 // HDIG

   //
   // waveOut-specific interface data
   //
   
   HWAVEOUT    hWaveOut;            // Wave output driver

   U32         reset_works;         // TRUE if OK to do waveOutReset
   U32         request_reset;       // If nonzero, do waveOutReset ASAP

   LPWAVEHDR   first;               // Pointer to first WAVEHDR in chain
   S32         n_buffers;           // # of output WAVEHDRs in chain

   LPWAVEHDR volatile FAR *return_list; // Circular list of returned WAVEHDRs
   S32       volatile      return_head; // Head of WAVEHDR list (insertion point)
   S32       volatile      return_tail; // Tail of WAVEHDR list (retrieval point)

   S32         build_size;          // # of bytes in build buffer
   S32    FAR *build_buffer;        // Build buffer (4 * samples_per_buffer)

   S32         quiet;               // # of consecutive quiet sample periods

   S32         n_active_samples;    // # of samples being processed

   //
   // DirectSound-specific interface data
   //

   U32                    guid;        // The guid id of the ds driver      
   AILLPDIRECTSOUND       lpDS;        // DirectSound output driver

   AILLPDIRECTSOUNDBUFFER FAR * lpbufflist;   // List of pointers to secondary buffers
   HSAMPLE         FAR *samp_list;      // HSAMPLE associated with each buffer
   S32             FAR *sec_format;     // DIG_F_ format for secondary buffer
   S32                  max_buffs;      // Max. allowable # of secondary buffers

   //
   // Output format data
   //

   S32         DMA_rate;            // Hardware sample rate
   S32         hw_format;           // DIG_F code in use
   U32         hw_mode_flags;       // DIG_PCM_ flags for mode in use

   S32         channels_per_sample; // # of channels per sample (1 or 2)
   S32         bytes_per_channel;   // # of bytes per channel (1 or 2)
   S32         channels_per_buffer; // # of channels per half-buffer
   S32         samples_per_buffer;  // # of samples per half-buffer

   //
   // Input data
   //

   HSAMPLE     samples;             // Pointer to list of SAMPLEs
   S32         n_samples;           // # of SAMPLEs

   S32         master_volume;       // Master sample volume 0-127

   //
   // Misc. data
   //

   S32         buffer_size;         // Size of each output buffer

   S32         playing;             // Playback active if non-zero

   S32         system_data[8];      // Miscellaneous system data

   U32         foreground_timer;    // Foreground timer handle
   HTIMER      background_timer;    // Background timer handle

   HDIGDRIVER  next;                // Pointer to next HDIGDRIVER in use
   S32      callingCT;           // Calling EXE's task number (16 bit only)
   S32      callingDS;           // Calling EXE's DS (used in 16 bit only)
}
DIG_DRIVER;

typedef struct                      // MIDI status log structure    
   {
   S32      program   [NUM_CHANS];  // Program Change
   S32      pitch_l   [NUM_CHANS];  // Pitch Bend LSB
   S32      pitch_h   [NUM_CHANS];  // Pitch Bend MSB

   S32      c_lock    [NUM_CHANS];  // Channel Lock
   S32      c_prot    [NUM_CHANS];  // Channel Lock Protection
   S32      c_mute    [NUM_CHANS];  // Channel Mute
   S32      c_v_prot  [NUM_CHANS];  // Voice Protection
   S32      bank      [NUM_CHANS];  // Patch Bank Select
   S32      indirect  [NUM_CHANS];  // ICA indirect controller value
   S32      callback  [NUM_CHANS];  // Callback Trigger

   S32      mod       [NUM_CHANS];  // Modulation
   S32      vol       [NUM_CHANS];  // Volume
   S32      pan       [NUM_CHANS];  // Panpot
   S32      exp       [NUM_CHANS];  // Expression
   S32      sus       [NUM_CHANS];  // Sustain
   S32      reverb    [NUM_CHANS];  // Reverb
   S32      chorus    [NUM_CHANS];  // Chorus

   S32      bend_range[NUM_CHANS];  // Bender Range (data MSB, RPN 0 assumed)
   }
CTRL_LOG;

struct _SEQUENCE                          // XMIDI sequence state table
{
   char     tag[4];                       // HSEQ

   HMDIDRIVER driver;                     // Driver for playback

   U32      status;                       // SEQ_ flags

   void    FAR *TIMB;                     // XMIDI IFF chunk pointers
   void    FAR *RBRN;           
   void    FAR *EVNT;           

   U8      FAR *EVNT_ptr;                 // Current event pointer

   U8      FAR *ICA;                      // Indirect Controller Array

   AILPREFIXCB   prefix_callback;         // XMIDI Callback Prefix handler
   AILTRIGGERCB  trigger_callback;        // XMIDI Callback Trigger handler
   AILBEATCB     beat_callback;           // XMIDI beat/bar change handler
   AILSEQUENCECB EOS;                     // End-of-sequence callback function

   S32      PREFIX_IsWin32s;              // Is this a Win32s callback
   S32      TRIGGER_IsWin32s;             // Is this a Win32s callback
   S32      BEAT_IsWin32s;                // Is this a Win32s callback
   S32      EOS_IsWin32s;                 // Is this a Win32s callback

   S32      loop_count;                   // 0=one-shot, -1=indefinite, ...

   S32      interval_count;               // # of intervals until next event
   S32      interval_num;                 // # of intervals since start

   S32      volume;                       // Sequence volume 0-127
   S32      volume_target;                // Target sequence volume 0-127
   S32      volume_accum;                 // Accumulated volume period
   S32      volume_period;                // Period for volume stepping

   S32      tempo_percent;                // Relative tempo percentage 0-100
   S32      tempo_target;                 // Target tempo 0-100
   S32      tempo_accum;                  // Accumulated tempo period
   S32      tempo_period;                 // Period for tempo stepping
   S32      tempo_error;                  // Error counter for tempo DDA

   S32      beat_count;                   // Sequence playback position
   S32      measure_count;  

   S32      time_numerator;               // Sequence timing data
   S32      time_fraction;  
   S32      beat_fraction;  
   S32      time_per_beat;

   void FAR *FOR_ptrs      [FOR_NEST];    // Loop stack
   S32      FOR_loop_count [FOR_NEST];

   S32      chan_map       [NUM_CHANS];   // Physical channel map for sequence

   CTRL_LOG shadow;                       // Controller values for sequence

   S32      note_count;                   // # of notes "on"
                          
   S32      note_chan      [MAX_NOTES];   // Channel for queued note (-1=free)
   S32      note_num       [MAX_NOTES];   // Note # for queued note
   S32      note_time      [MAX_NOTES];   // Remaining duration in intervals

   S32      user_data  [8];               // Miscellaneous user data
   S32      system_data[8];               // Miscellaneous system data
};

typedef struct _MDI_DRIVER          // Handle to XMIDI driver
{
   char     tag[4];                 // HMDI

   HMIDIOUT    hMidiOut;            // MIDI output driver

   HTIMER      timer;               // XMIDI quantization timer
   S32         interval_time;       // XMIDI quantization timer interval in uS

   S32         disable;             // > 0 to disable XMIDI service

   HSEQUENCE   sequences;           // Pointer to list of SEQUENCEs
   S32         n_sequences;         // # of SEQUENCEs

   S32         lock  [NUM_CHANS];   // 1 if locked, 2 if protected, else 0
   HSEQUENCE   locker[NUM_CHANS];   // HSEQUENCE which locked channel
   HSEQUENCE   owner [NUM_CHANS];   // HSEQUENCE which owned locked channel
   HSEQUENCE   user  [NUM_CHANS];   // Last sequence to use channel
   S32         state [NUM_CHANS];   // Lock state prior to being locked

   S32         notes [NUM_CHANS];   // # of active notes in channel

   AILEVENTCB  event_trap;          // MIDI event trap callback function
   AILTIMBRECB timbre_trap;         // Timbre request callback function

   S32      EVENT_IsWin32s;         // Is this a Win32s callback
   S32      TIMBRE_IsWin32s;        // Is this a Win32s callback

   S32         master_volume;       // Master XMIDI note volume 0-127

   S32         system_data[8];      // Miscellaneous system data

   MIDIHDR FAR *mhdr;               // SysEx header
   U8      FAR *sysdata;            // SysEx buffer

   HMDIDRIVER  next;                // Pointer to next HMDIDRIVER in use
   S32      callingCT;              // Calling EXE's task number (16 bit only)
   S32      callingDS;              // Calling EXE's DS (used in 16 bit only)
}
MDI_DRIVER;

typedef struct                      // XMIDI TIMB IFF chunk
   {
   S8    name[4];

   U8    msb;
   U8    lsb;
   U8    lsb2;
   U8    lsb3;

   U16   n_entries;

   U16   timbre[1];
   }
TIMB_chunk;

typedef struct                      // XMIDI RBRN IFF entry
   {
   S16   bnum;
   U32   offset;
   }
RBRN_entry;

typedef struct                      // Wave library entry
{
   S32   bank;                      // XMIDI bank, MIDI patch for sample
   S32   patch;                     

   S32   root_key;                  // Root MIDI note # for sample (or -1)

   U32   file_offset;               // Offset of wave data from start-of-file
   U32   size;                      // Size of wave sample in bytes

   S32   format;                    // DIG_F format (8/16 bits, mono/stereo)
   U32   flags;                     // DIG_PCM_SIGN / DIG_PCM_ORDER (stereo)
   S32   playback_rate;             // Playback rate in hertz
}
WAVE_ENTRY;

typedef struct                      // Virtual "wave synthesizer" descriptor
{
   HMDIDRIVER mdi;                  // MIDI driver for use with synthesizer
   HDIGDRIVER dig;                  // Digital driver for use with synthesizer

   WAVE_ENTRY FAR *library;         // Pointer to wave library

   AILEVENTCB  prev_event_fn;       // Previous MIDI event trap function
   AILTIMBRECB prev_timb_fn;        // Previous timbre request trap function

   CTRL_LOG    controls;            // MIDI controller states

   WAVE_ENTRY FAR *wave [NUM_CHANS];// Pointer to WAVE_ENTRY for each channel

   HSAMPLE     S    [MAX_W_VOICES]; // List of HSAMPLE voices
   S32         n_voices;            // Actual # of voices allocated to synth

   S32         chan [MAX_W_VOICES]; // MIDI channel for each voice, or -1
   S32         note [MAX_W_VOICES]; // MIDI note number for voice
   S32         root [MAX_W_VOICES]; // MIDI root note for voice
   S32         rate [MAX_W_VOICES]; // Playback rate for voice
   S32         vel  [MAX_W_VOICES]; // MIDI note velocity for voice
   U32         time [MAX_W_VOICES]; // Timestamp for voice

   U32         event;               // Event counter for LRU timestamps
}
WAVE_SYNTH;

typedef WAVE_SYNTH FAR * HWAVESYNTH;// Handle to virtual wave synthesizer

//
// Handle to thread which called AIL_startup()
// 
// This thread is suspended by WAIL callback threads, to simulate DOS-style
// interrupt handler behavior
// 

#ifdef IS_WIN32

DXDEC extern HANDLE hAppThread;

#endif

//
// Background flag for timers
// 

extern volatile S32            AIL_bkgnd_flag;

//
// Global preference array
//

extern S32            AIL_preference   [N_PREFS];

//
// AIL fatal error type string
//
                                      
extern S8             AIL_error        [256];

//
// DIG_DRIVER list
//

extern HDIGDRIVER     DIG_first;

//
// MDI_DRIVER list
//

extern HMDIDRIVER     MDI_first;

//
// Miscellaneous system services
//

DXDEC  void FAR * WINAPI MEM_alloc_lock   (U32       size);
DXDEC  void       WINAPI MEM_free_lock    (void FAR *ptr);

DXDEC  S32        WINAPI FILE_error       (void);

DXDEC  S32        WINAPI FILE_size        (S8 FAR   *filename);

DXDEC  void FAR * WINAPI FILE_read        (S8 FAR   *filename, 
                                           void FAR *dest);

DXDEC  S32        WINAPI FILE_write       (S8 FAR   *filename, 
                                           void FAR *buf, 
                                           U32       len);

extern void FAR * PTR_add          (void FAR *ptr, 
                                           U32       offset);

extern S32        PTR_dif          (void FAR *p1,  
                                           void FAR *p2);

extern S32        STR_nicmp        (void FAR *s1,
                                           void FAR *s2,
                                           U32       count);

extern U8 FAR *   STR_str          (void FAR *s1, 
                                           void FAR *s2);

extern void       MEM_cpy          (void FAR *s1, 
                                           void FAR *s2, 
                                           U32       count);

typedef struct
{
   U8 FAR *Buffer;
   U8 FAR *Buffer32;
   U8 FAR *Base32;
   S32     StartO;
   S32     EndO;
   S32     Size;
   S32     CurLeft;
}
WIN32S_CALLBACKS;


#ifdef IS_WIN32

#define Only16Push32s()

#define Only16Pop32s()

#define WAIL_do_cb1(type,addr,ds,win32sflag,param1) \
  (addr)(param1)

#define WAIL_do_cb3(type,addr,ds,win32sflag,param1,param2,param3) \
  (addr)(param1,param2,param3)

#define WAIL_do_cb4(type,addr,ds,win32sflag,param1,param2,param3,param4) \
  (addr)(param1,param2,param3,param4)

#define WAIL_do_cb1_with_ret(ret,type,addr,ds,win32sflag,win32sret,param1) \
  ret = (addr)(param1)

#define WAIL_do_cb3_with_ret(ret,type,addr,ds,win32sflag,win32sret,param1,param2,param3) \
  ret = (addr)(param1,param2,param3)

#define WAIL_do_cb5_with_ret(ret,type,addr,ds,win32sflag,win32sret,param1,param2,param3,param4,param5) \
  ret = (addr)(param1,param2,param3,param4,param5)

#else  

extern U8 FromWin32sCB;

extern void __cdecl FAR Only16Push32s();

extern void __cdecl FAR Only16Pop32s();

extern void FAR WAILGetTaskInfo(S32 FAR* ct, S32 FAR* cds);

extern void __cdecl FAR WAILCall16();

extern void __cdecl FAR SetupWAILCall16(void FAR* addr,U16 ds);

extern void Win32sMakeCB();

extern void Win32sAddCB1(void FAR* addr, U32 val);

extern void Win32sAddCB3(void FAR* addr, U32 val1, U32 val2, U32 val3);

extern void Win32sAddCB4(void FAR* addr, U32 val1, U32 val2, U32 val3, U32 val4);

extern void Win32sAddCB5(void FAR* addr, U32 val1, U32 val2, U32 val3, U32 val4, U32 val5);

#define CheckWin32sCB(param) \
  if (FromWin32sCB) {        \
    param=1;                 \
    Win32sMakeCB();          \
  } else                     \
    param=0;               

#define WAIL_do_cb1(type,addr,ds,win32sflag,param1)                         \
  if (win32sflag)                                                           \
    Win32sAddCB1(addr,(U32)param1);                                         \
  else {                                                                    \
    SetupWAILCall16((void FAR*)type addr,(U16)(ds));                        \
    (type &WAILCall16)(param1);                                             \
  }

#define WAIL_do_cb3(type,addr,ds,win32sflag,param1,param2,param3)           \
  if (win32sflag)                                                           \
    Win32sAddCB3(addr,(U32)param1,(U32)param2,(U32)param3);                 \
  else {                                                                    \
    SetupWAILCall16((void FAR*)type addr,(U16)(ds));                        \
    (type &WAILCall16)(param1,param2,param3);                               \
  }

#define WAIL_do_cb4(type,addr,ds,win32sflag,param1,param2,param3,param4)    \
  if (win32sflag)                                                           \
    Win32sAddCB4(addr,(U32)param1,(U32)param2,(U32)param3,(U32)param4);     \
  else {                                                                    \
    SetupWAILCall16((void FAR*)type addr,(U16)(ds));                        \
    (type &WAILCall16)(param1,param2,param3,param4);                        \
  }

#define WAIL_do_cb1_with_ret(ret,type,addr,ds,win32sflag,win32sret,param1)  \
  if (win32sflag) {                                                         \
    Win32sAddCB1(addr,(U32)param1);                                         \
    ret=win32sret;                                                          \
  } else {                                                                  \
    SetupWAILCall16((void FAR*)type addr,(U16)(ds));                        \
    ret=(type &WAILCall16)(param1);                                         \
  }

#define WAIL_do_cb3_with_ret(ret,type,addr,ds,win32sflag,win32sret,param1,param2,param3) \
  if (win32sflag) {                                                         \
    Win32sAddCB3(addr,(U32)param1,(U32)param2,(U32)param3);                 \
    ret=win32sret;                                                          \
  } else {                                                                  \
    SetupWAILCall16((void FAR*)type addr,(U16)(ds));                        \
    ret=(type &WAILCall16)(param1,param2,param3);                           \
  }                                                                   

#define WAIL_do_cb5_with_ret(ret,type,addr,ds,win32sflag,win32sret,param1,param2,param3,param4,param5) \
  if (win32sflag) {                                                         \
    Win32sAddCB5(addr,(U32)param1,(U32)param2,(U32)param3,(U32)param4,(U32)param5); \
    ret=win32sret;                                                          \
  } else {                                                                  \
    SetupWAILCall16((void FAR*)type addr,(U16)(ds));                        \
    ret=(type &WAILCall16)(param1,param2,param3,param4,param5);             \
  }

#endif


//
// Quick-integration service functions and data types
//

typedef struct
{
   void FAR *data;
   S32  len;

   S32  type;
   void FAR *handle;

   S32  status;

   void FAR* next;
}
AUDIO_TYPE;

#define QSTAT_DONE          1       // Data has finished playing
#define QSTAT_LOADED        2       // Data has been loaded, but not yet played
#define QSTAT_PLAYING       3       // Data is currently playing

typedef AUDIO_TYPE FAR * HAUDIO;        // Generic handle to any audio data type


DXDEC S32     WINAPI AIL_quick_startup             (S32         use_digital, 
                                                    S32         use_MIDI,
                                                    U32         output_rate,
                                                    S32         output_bits,
                                                    S32         output_channels);

DXDEC void    WINAPI AIL_quick_shutdown            (void);

DXDEC HAUDIO  WINAPI AIL_quick_load                (char   FAR *filename);
DXDEC void    WINAPI AIL_quick_unload              (HAUDIO      audio);

DXDEC S32     WINAPI AIL_quick_play                (HAUDIO      audio,
                                                    U32         loop_count);

DXDEC void    WINAPI AIL_quick_halt                (HAUDIO      audio);

DXDEC S32     WINAPI AIL_quick_status              (HAUDIO      audio);

DXDEC HAUDIO  WINAPI AIL_quick_load_and_play       (char   FAR *filename, 
                                                    U32         loop_count,
                                                    S32         wait_request);

//
// High-level support services
//

DXDEC  S32     WINAPI  AIL_startup                   (void);
DXDEC  void    WINAPI  AIL_shutdown                  (void);

DXDEC  S32     WINAPI  AIL_get_preference            (U32         number);

DXDEC  S32     WINAPI  AIL_set_preference            (U32         number, 
                                                      S32          value);

DXDEC  LPSTR   WINAPI  AIL_last_error                (void);

DXDEC  void    WINAPI  AIL_set_error                 (LPSTR error_msg);

//
// Low-level support services
//

DXDEC  void    WINAPI  AIL_lock                      (void);
DXDEC  void    WINAPI  AIL_unlock                    (void);

DXDEC  void    WINAPI  AIL_delay                     (S32         intervals);

DXDEC  S32     WINAPI  AIL_background                (void);

//                 
// Process services
//

DXDEC  HTIMER  WINAPI  AIL_register_timer            (AILTIMERCB  fn);

DXDEC  U32     WINAPI  AIL_set_timer_user            (HTIMER      timer, 
                                                      U32         user);

DXDEC  void    WINAPI  AIL_set_timer_period          (HTIMER      timer, 
                                                      U32         microseconds);

DXDEC  void    WINAPI  AIL_set_timer_frequency       (HTIMER      timer, 
                                                      U32         hertz);

DXDEC  void    WINAPI  AIL_set_timer_divisor         (HTIMER      timer, 
                                                      U32         PIT_divisor);

DXDEC  void    WINAPI  AIL_start_timer               (HTIMER      timer);
DXDEC  void    WINAPI  AIL_start_all_timers          (void);

DXDEC  void    WINAPI  AIL_stop_timer                (HTIMER      timer);
DXDEC  void    WINAPI  AIL_stop_all_timers           (void);

DXDEC  void    WINAPI  AIL_release_timer_handle      (HTIMER      timer);
DXDEC  void    WINAPI  AIL_release_all_timers        (void);

//
// high-level digital services
//


DXDEC  S32          WINAPI AIL_waveOutOpen           (HDIGDRIVER   FAR *drvr,
                                                      LPHWAVEOUT   FAR *lphWaveOut,
                                                      U32             wDeviceID,
                                                      LPWAVEFORMAT      lpFormat);

DXDEC  void         WINAPI AIL_waveOutClose          (HDIGDRIVER drvr);

DXDEC  void         WINAPI AIL_serve();

DXDEC  HSAMPLE      WINAPI AIL_allocate_sample_handle 
                                                    (HDIGDRIVER dig);

DXDEC  HSAMPLE      WINAPI AIL_allocate_file_sample  (HDIGDRIVER dig, 
                                                     void       FAR *file_image, 
                                                     S32         block);

DXDEC  void         WINAPI AIL_release_sample_handle (HSAMPLE S);
                                                 
DXDEC  void         WINAPI AIL_init_sample           (HSAMPLE S);

DXDEC  S32          WINAPI AIL_set_sample_file       (HSAMPLE S,
                                                     void   FAR *file_image,
                                                     S32     block);

DXDEC  void         WINAPI AIL_set_sample_address    (HSAMPLE S, 
                                                     void   FAR *start, 
                                                     U32     len);
                                                 
DXDEC  void         WINAPI AIL_set_sample_type       (HSAMPLE S, 
                                                     S32     format, 
                                                     U32     flags);

DXDEC  void         WINAPI AIL_start_sample          (HSAMPLE S);
DXDEC  void         WINAPI AIL_stop_sample           (HSAMPLE S);
DXDEC  void         WINAPI AIL_resume_sample         (HSAMPLE S);
DXDEC  void         WINAPI AIL_end_sample            (HSAMPLE S);

DXDEC  void         WINAPI AIL_set_sample_playback_rate 
                                                     (HSAMPLE S, 
                                                     S32     playback_rate);

DXDEC  void         WINAPI AIL_set_sample_volume     (HSAMPLE S, 
                                                     S32     volume);

DXDEC  void         WINAPI AIL_set_sample_pan        (HSAMPLE S, 
                                                     S32     pan);

DXDEC  void         WINAPI AIL_set_sample_loop_count (HSAMPLE S, 
                                                     S32     loop_count);

DXDEC  void         WINAPI AIL_set_sample_loop_block (HSAMPLE S,
                                                     S32     loop_start_offset,
                                                     S32     loop_end_offset);

DXDEC  U32          WINAPI AIL_sample_status         (HSAMPLE S);
DXDEC  S32          WINAPI AIL_sample_playback_rate  (HSAMPLE S);
DXDEC  S32          WINAPI AIL_sample_volume         (HSAMPLE S);
DXDEC  S32          WINAPI AIL_sample_pan            (HSAMPLE S);
DXDEC  S32          WINAPI AIL_sample_loop_count     (HSAMPLE S);

DXDEC  void         WINAPI AIL_set_digital_master_volume
                                                     (HDIGDRIVER dig, 
                                                     S32         master_volume);

DXDEC  S32          WINAPI AIL_digital_master_volume (HDIGDRIVER dig);

//
// low-level digital services
//

DXDEC  S32      WINAPI AIL_minimum_sample_buffer_size(HDIGDRIVER dig,
                                                     S32         playback_rate,
                                                     S32         format);       

DXDEC  S32      WINAPI AIL_sample_buffer_ready      (HSAMPLE S);

DXDEC  void     WINAPI AIL_load_sample_buffer       (HSAMPLE S,
                                                     U32     buff_num,
                                                     void   FAR *buffer,
                                                     U32     len);

DXDEC  S32      WINAPI AIL_sample_buffer_info      (HSAMPLE S, //)
                                                    U32     FAR *pos0,
                                                    U32     FAR *len0,
                                                    U32     FAR *pos1,
                                                    U32     FAR *len1);

DXDEC  void     WINAPI AIL_set_sample_position      (HSAMPLE S, 
                                                     U32     pos);

DXDEC  U32      WINAPI AIL_sample_position          (HSAMPLE S);

DXDEC  AILSAMPLECB WINAPI AIL_register_SOB_callback
                                                    (HSAMPLE S,
                                                     AILSAMPLECB SOB);

DXDEC  AILSAMPLECB WINAPI AIL_register_EOB_callback
                                                    (HSAMPLE S,
                                                     AILSAMPLECB EOB);

DXDEC  AILSAMPLECB WINAPI AIL_register_EOS_callback
                                                    (HSAMPLE S,
                                                     AILSAMPLECB EOS);

DXDEC  AILSAMPLECB WINAPI AIL_register_EOF_callback
                                                    (HSAMPLE S,
                                                     AILSAMPLECB EOFILE);

DXDEC  void     WINAPI AIL_set_sample_user_data   (HSAMPLE S,
                                                     U32     index,
                                                     S32     value);

DXDEC  S32      WINAPI AIL_sample_user_data       (HSAMPLE S,
                                                     U32     index);

DXDEC  S32      WINAPI AIL_active_sample_count    (HDIGDRIVER dig);

DXDEC  void     WINAPI AIL_digital_configuration  (HDIGDRIVER dig,
                                                      S32   FAR *rate,
                                                      S32   FAR *format,
                                                      char  FAR *string);

DXDEC  S32      WINAPI AIL_set_direct_buffer_control (HSAMPLE S,
                                                     U32      command);

DXDEC  void     WINAPI AIL_get_DirectSound_info  (HSAMPLE              S,
                                              AILLPDIRECTSOUND    *lplpDS,
                                              AILLPDIRECTSOUNDBUFFER *lplpDSB);
//
// High-level XMIDI services
//

DXDEC  S32          WINAPI AIL_midiOutOpen(HMDIDRIVER FAR *drvr,
                                               LPHMIDIOUT FAR *lphMidiOut,
                                               U32           dwDeviceID);

DXDEC  void         WINAPI AIL_midiOutClose      (HMDIDRIVER mdi);



DXDEC  HSEQUENCE    WINAPI AIL_allocate_sequence_handle 
                                                     (HMDIDRIVER mdi);

DXDEC  void         WINAPI AIL_release_sequence_handle 
                                                     (HSEQUENCE S);
                                                 
DXDEC  S32          WINAPI AIL_init_sequence         (HSEQUENCE S,
                                                     void     FAR *start,
                                                     S32       sequence_num);

DXDEC  void         WINAPI AIL_start_sequence        (HSEQUENCE S);
DXDEC  void         WINAPI AIL_stop_sequence         (HSEQUENCE S);
DXDEC  void         WINAPI AIL_resume_sequence       (HSEQUENCE S);
DXDEC  void         WINAPI AIL_end_sequence          (HSEQUENCE S);

DXDEC  void         WINAPI AIL_set_sequence_tempo    (HSEQUENCE S, 
                                                     S32       tempo,
                                                     S32       milliseconds);

DXDEC  void         WINAPI AIL_set_sequence_volume   (HSEQUENCE S, 
                                                     S32       volume,
                                                     S32       milliseconds);

DXDEC  void         WINAPI AIL_set_sequence_loop_count
                                                     (HSEQUENCE S, 
                                                     S32       loop_count);

DXDEC  U32          WINAPI AIL_sequence_status       (HSEQUENCE S);
                                                   
DXDEC  S32          WINAPI AIL_sequence_tempo        (HSEQUENCE S);
DXDEC  S32          WINAPI AIL_sequence_volume       (HSEQUENCE S);
DXDEC  S32          WINAPI AIL_sequence_loop_count   (HSEQUENCE S);

DXDEC  void         WINAPI AIL_set_XMIDI_master_volume
                                                     (HMDIDRIVER mdi, 
                                                     S32         master_volume);

DXDEC  S32          WINAPI AIL_XMIDI_master_volume   (HMDIDRIVER mdi);

//
// Low-level XMIDI services
//

DXDEC  S32      WINAPI AIL_active_sequence_count     (HMDIDRIVER mdi);

DXDEC  S32      WINAPI AIL_controller_value          (HSEQUENCE S,
                                                      S32       channel,
                                                      S32       controller_num);

DXDEC  S32      WINAPI AIL_channel_notes             (HSEQUENCE S,
                                                      S32       channel);

DXDEC  void     WINAPI AIL_sequence_position         (HSEQUENCE S,
                                                      S32      FAR *beat,
                                                      S32      FAR *measure);

DXDEC  void     WINAPI AIL_branch_index              (HSEQUENCE  S,
                                                      U32        marker);

DXDEC  AILPREFIXCB WINAPI AIL_register_prefix_callback
                                                     (HSEQUENCE  S,
                                                      AILPREFIXCB callback);
                                                     
DXDEC  AILTRIGGERCB WINAPI AIL_register_trigger_callback
                                                     (HSEQUENCE  S,
                                                      AILTRIGGERCB callback);
                                                     
DXDEC  AILSEQUENCECB WINAPI AIL_register_sequence_callback 
                                                     (HSEQUENCE  S,
                                                      AILSEQUENCECB callback);
                                                     
DXDEC  AILBEATCB WINAPI AIL_register_beat_callback   (HSEQUENCE  S,
                                                      AILBEATCB callback);

DXDEC  AILEVENTCB WINAPI AIL_register_event_callback (HMDIDRIVER mdi,
                                                      AILEVENTCB callback);
                                                     
DXDEC  AILTIMBRECB WINAPI AIL_register_timbre_callback
                                                     (HMDIDRIVER mdi,
                                                      AILTIMBRECB callback);

DXDEC  void     WINAPI AIL_set_sequence_user_data    (HSEQUENCE S,
                                                      U32       index,
                                                      S32       value);

DXDEC  S32      WINAPI AIL_sequence_user_data        (HSEQUENCE S,
                                                      U32       index);

DXDEC  void     WINAPI AIL_register_ICA_array        (HSEQUENCE S,
                                                      S8       FAR *array);

DXDEC  S32      WINAPI AIL_lock_channel              (HMDIDRIVER mdi);

DXDEC  void     WINAPI AIL_release_channel           (HMDIDRIVER mdi,
                                                      S32         channel);

DXDEC  void     WINAPI AIL_map_sequence_channel      (HSEQUENCE S,
                                                      S32       seq_channel,
                                                      S32       new_channel);

DXDEC  S32      WINAPI AIL_true_sequence_channel     (HSEQUENCE S,
                                                      S32       seq_channel);

DXDEC  void     WINAPI AIL_send_channel_voice_message
                                                     (HMDIDRIVER  mdi,
                                                      HSEQUENCE   S,
                                                      S32         status,
                                                      S32         data_1,
                                                      S32         data_2);

DXDEC  void     WINAPI AIL_send_sysex_message        (HMDIDRIVER mdi,
                                                     void       FAR *buffer);

DXDEC  HWAVESYNTH 
                WINAPI AIL_create_wave_synthesizer   (HDIGDRIVER dig,
                                                     HMDIDRIVER mdi,
                                                     void       FAR *wave_lib,
                                                     S32         polyphony);

DXDEC  void     WINAPI AIL_destroy_wave_synthesizer  (HWAVESYNTH W);

typedef struct _REDBOOK {
  U32 DeviceID;
  U32 paused;
  U32 pausedsec;
  U32 lastendsec;
} REDBOOK;

typedef struct _REDBOOK FAR* HREDBOOK;

#define REDBOOK_ERROR    0
#define REDBOOK_PLAYING  1
#define REDBOOK_PAUSED   2
#define REDBOOK_STOPPED  3


DXDEC  HREDBOOK   WINAPI AIL_redbook_open(U32 which);

DXDEC  void       WINAPI AIL_redbook_close(HREDBOOK hand);

DXDEC  void       WINAPI AIL_redbook_eject(HREDBOOK hand);

DXDEC  U32        WINAPI AIL_redbook_status(HREDBOOK hand);

DXDEC  U32        WINAPI AIL_redbook_tracks(HREDBOOK hand);

DXDEC  void       WINAPI AIL_redbook_track_info(HREDBOOK hand,U32 tracknum,
                                                     U32 FAR* startmsec,U32 FAR* endmsec);

DXDEC  U32        WINAPI AIL_redbook_id(HREDBOOK hand);

DXDEC  U32        WINAPI AIL_redbook_position(HREDBOOK hand);

DXDEC  U32        WINAPI AIL_redbook_play(HREDBOOK hand,U32 startmsec, U32 endmsec);

DXDEC  U32        WINAPI AIL_redbook_stop(HREDBOOK hand);

DXDEC  U32        WINAPI AIL_redbook_pause(HREDBOOK hand);

DXDEC  U32        WINAPI AIL_redbook_resume(HREDBOOK hand);


#define AIL_ms_count timeGetTime

#ifdef __cplusplus
}
#endif

#endif
