//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++                                                                        ++
//++  IBM Audio Interface Library                                           ++
//++                                                                        ++
//++  AIL.H: API function prototypes                                        ++
//++                                                                        ++
//++  Flat-model source compatible with IBM 32-bit ANSI C/C++               ++
//++                                                                        ++
//++  Version 3.00 of 11-Apr-94: Initial version for AIL 3.0                ++
//++          3.01 of 21-Oct-94: Added Watcom/FlashTek support              ++
//++                             AIL_restore_interrupts() changed to void   ++
//++          3.02 of  3-Jan-95: VDI_HDR updated to DDK rev 1.12            ++
//++                             Added AIL_INI structure                    ++
//++                             Added AIL_install_DIG/MDI_INI() functions  ++
//++          3.03 of  9-Jan-95: Added various functions for V3.03 release  ++
//++                             Borland/Zortech support added              ++
//++                                                                        ++
//++  Author: John Miles                                                    ++
//++                                                                        ++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++                                                                        ++
//++  Copyright (C) 1994-1995 Miles Design, Inc.                            ++
//++                                                                        ++
//++  Miles Design, Inc.                                                    ++
//++  8301 Elander Drive                                                    ++
//++  Austin, TX 78750                                                      ++
//++                                                                        ++
//++  (512) 345-2642 / FAX (512) 338-9630 / BBS (512) 454-9990              ++
//++                                                                        ++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef _WIN32

#ifndef AIL_VERSION

#define AIL_VERSION      "3.03"
#define AIL_VERSION_DATE "7-Nov-95"

#endif

//
// Watcom/FlashTek support (for library build)
//

#ifdef INT21
#ifdef __WATCOMC__
#include "x32.h"
#endif
#ifdef __ZTC__
#include "x32.h"
#endif
#endif

#ifndef AIL_H
#define AIL_H

//
// MetaWare support
//

#ifdef __HIGHC__

#define cdecl _CC(_REVERSE_PARMS | _NEAR_CALL)
#pragma Global_aliasing_convention("_%r");
#pragma Align_members(1)

#ifndef FAR_PTR_DEF
#define FAR_PTR_DEF

typedef struct   // Union used to build far pointers under MetaWare
{
   unsigned off;
   unsigned short seg;
}
fp_ovl;

typedef union
{
   _Far void *ptr;
   fp_ovl part;
}
FARPTR;

#endif
#endif

//
// End of MetaWare conditionals
//

#ifdef __cplusplus
extern "C" {
#endif

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

#define DIG_PCM_SIGN             0x0001   // PCM flags
#define DIG_PCM_ORDER            0x0002

#define DIG_PCM_POLARITY         0x0004   // PCM flags used by driver hardware
#define DIG_PCM_SPLIT            0x0008
#define DIG_BUFFER_SERVICE       0x0010
#define DIG_DUAL_DMA             0x0020
#define DIG_RECORDING_SUPPORTED  0x8000

#define AIL3DIG                  0        // .DIG driver
#define AIL3MDI                  1        // .MDI driver

#define DIG_DETECT_8_BIT_ONLY    0x0001   // Detect 8-bit DMA only
#define DIG_DETECT_16_BIT_ONLY   0x0002   // Detect 16-bit DMA only
#define DIG_DETECT_8_AND_16_BITS 0x0003   // Detect both 8- and 16-bit DMA

#define DRV_INIT          0x300     // Functions common to .MDI and .DIG
#define DRV_GET_INFO      0x301     // drivers
#define DRV_SERVE         0x302
#define DRV_PARSE_ENV     0x303
#define DRV_VERIFY_IO     0x304
#define DRV_INIT_DEV      0x305
#define DRV_SHUTDOWN_DEV  0x306

#define DIG_HW_VOLUME     0x400     // .DIG driver functions
#define DIG_START_P_CMD   0x401
#define DIG_STOP_P_REQ    0x402
#define DIG_START_R_CMD   0x403
#define DIG_STOP_R_REQ    0x404
#define DIG_VSE           0x405

#define MDI_HW_VOLUME     0x500     // .MDI driver functions
#define MDI_INIT_INS_MGR  0x501
#define MDI_MIDI_XMIT     0x502
#define MDI_INSTALL_T_SET 0x503
#define MDI_GET_T_STATUS  0x504
#define MDI_PROT_UNPROT_T 0x505
#define MDI_VSE           0x506

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
#define SSD_TEMP          7  // Temporary storage location for general use

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
// MIDI driver types
//

#define MDIDRVRTYPE_GM    0         // General MIDI driver (Roland-compatible)
#define MDIDRVRTYPE_FM_2  1         // 2-operator FM MIDI driver (OPL2)
#define MDIDRVRTYPE_FM_4  2         // 4-operator FM MIDI driver (OPL3)
#define MDIDRVRTYPE_SPKR  3         // Tandy or PC speaker "beep" driver

//
// .INI installation result codes
//

#define AIL_INIT_SUCCESS  0         // Driver installed successfully
#define AIL_NO_INI_FILE   1         // No MDI.INI or DIG.INI file exists
#define AIL_INIT_FAILURE  2         // Driver could not be initialized

//
// General type definitions for portability
//

#ifndef TYPEDEFS
#define TYPEDEFS

typedef unsigned char  UBYTE;
typedef unsigned short UWORD;
typedef unsigned long  ULONG;
// @Note (toml 05-20-96): removed because incompatible with Windows
// these are defined as unsiged in both windows.h and our types.h
// If this breaks AIL, we'll have to explore a better way to
// resolve the conflict
#if 0
typedef          char  BYTE;
typedef signed   short WORD;
#endif
typedef signed   long  LONG;

#endif

typedef void * CALLBACK;            // Generic callback function prototype

typedef ULONG REALFAR;              // Real-mode far pointer type

#ifdef _X32_H_INCLUDED              // Macro to access low mem via seg:off ptr

 #ifndef REALPTR
 #define REALPTR(x) ((void *) (ULONG) ((((ULONG) (x))>>16<<4) + ((x) & 0xffff) \
                    + (ULONG) _x32_zero_base_ptr))
 #endif

#else
#ifdef __BORLANDC__

 #ifndef REALPTR
 #define REALPTR(x) ((void *) (ULONG) ((((ULONG) (x))>>16<<4) + ((x) & 0xffff) \
                    - SEL_base(_DS)))
 #endif

#else

 #ifndef REALPTR
 #define REALPTR(x) ((void *) (ULONG) ((((ULONG) (x))>>16<<4) + ((x) & 0xffff)))
 #endif

#endif
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

#define DIG_SERVICE_RATE          0
#define DEFAULT_DSR               200     // DMA buffer-polling rate = 200 Hz

#define DIG_HARDWARE_SAMPLE_RATE  1
#define DEFAULT_DHSR              NOM_VAL // Use nominal sample rate by default

#define DIG_DMA_RESERVE           2
#define DEFAULT_DDR               32768   // Reserve 32K real-mode mem for DMA

#define DIG_LATENCY               3
#define DEFAULT_DL                100     // Half-buffer size in ms = 100

#define DIG_MIXER_CHANNELS        4
#define DEFAULT_DMC               16      // 16 allocatable SAMPLE structures

#define DIG_DEFAULT_VOLUME        5
#define DEFAULT_DDV               100     // Default sample volume = 100 (0-127)

#define DIG_RESAMPLING_TOLERANCE  6
#define DEFAULT_DRT               655     // Resampling triggered at +/- 1%

#define DIG_USE_STEREO            7
#define DEFAULT_DUS               NO      // Use mono output only

#define DIG_USE_16_BITS           8
#define DEFAULT_DU16              NO      // Use 8-bit output by default

#define DIG_ALLOW_16_BIT_DMA      9
#define DEFAULT_DA16DMA           YES     // OK to use 16-bit DMA if necessary

#define DIG_SS_LOCK               10
#define DEFAULT_DSL               NO      // Don't disable IRQs while mixing

#define MDI_SERVICE_RATE          11
#define DEFAULT_MSR               120     // XMIDI sequencer timing = 120 Hz

#define MDI_SEQUENCES             12
#define DEFAULT_MS                8       // 8 sequence handles/driver

#define MDI_DEFAULT_VOLUME        13
#define DEFAULT_MDV               127     // Default sequence volume = 127 (0-127)

#define MDI_QUANT_ADVANCE         14
#define DEFAULT_MQA               1       // Beat/bar count +1 interval

#define MDI_ALLOW_LOOP_BRANCHING  15
#define DEFAULT_ALB               NO      // Branches cancel XMIDI FOR loops

#define MDI_DEFAULT_BEND_RANGE    16
#define DEFAULT_MDBR              2       // Default pitch-bend range = 2

#define AIL_SCAN_FOR_HARDWARE     17
#define DEFAULT_ASH               YES     // Scan for I/O settings if necessary

#define AIL_ALLOW_VDM_EXECUTION   18
#define DEFAULT_AVE               YES     // Allow Windows "DOS box" execution

#define N_PREFS 19                        // # of preference types

//
// Type definitions
//

typedef LONG     HTIMER;            // Handle to timer

typedef struct                      // I/O parameters structure
{
   WORD IO;
   WORD IRQ;
   WORD DMA_8_bit;
   WORD DMA_16_bit;
   LONG IO_reserved[4];
}
IO_PARMS;

typedef struct                      // Standard AIL 3.X VDI driver header
{
   BYTE     ID[8];                  // "AIL3xxx" ID string, followed by ^Z

   ULONG    driver_version;

   REALFAR  common_IO_configurations;
   UWORD    num_IO_configurations;

   REALFAR  environment_string;

   IO_PARMS IO;

   WORD     service_rate;

   UWORD    busy;

   UWORD    driver_num;             // Driver number

   UWORD    this_ISR;               // Offset of INT 66H dispatcher
   REALFAR  prev_ISR;               // Pointer to previous INT 66H ISR

   BYTE     scratch[128];           // Shared scratch workspace

   BYTE     dev_name[80];           // Device name (VDI version >= 1.12 only)
}
VDI_HDR;

typedef struct
{
   UWORD minimum_physical_sample_rate;
   UWORD nominal_physical_sample_rate;
   UWORD maximum_physical_sample_rate;

   UWORD minimum_DMA_half_buffer_size;
   UWORD maximum_DMA_half_buffer_size;

   ULONG flags;
}
DIG_MODE;

typedef struct
{
   UBYTE    format_supported[16];
   DIG_MODE format_data[16];
}
DIG_DDT;

typedef struct
{
   REALFAR  DMA_buffer_A;
   REALFAR  DMA_buffer_B;
   WORD     active_buffer;
}
DIG_DST;

typedef struct
{
   REALFAR  library_environment;
   REALFAR  GTL_suffix;

   UWORD    num_voices;

   UWORD    max_melodic_channel;
   UWORD    min_melodic_channel;
   UWORD    percussion_channel;
}
MDI_DDT;

typedef struct
{
   BYTE     library_directory[128];
   BYTE     GTL_filename[128];

   BYTE     MIDI_data[512];
}
MDI_DST;

typedef struct                   // Initialization file structure
{
   char     device_name[128];    // Device name
   char     driver_name[128];    // Driver filename
   IO_PARMS IO;                  // I/O parameters for driver
}
AIL_INI;

typedef struct                   // Handle to driver
{
   REALFAR  seg;                 // Seg:off pointer to driver (off=0)
   ULONG    sel;                 // Selector for driver (off=0)
   void    *buf;                 // Protected-mode pointer to driver
   ULONG    size;                // Size of driver image
   VDI_HDR *VHDR;                // Pointer to driver header (same as buf)
   LONG     type;                // AIL3DIG or AIL3MDI (see below)

   LONG     initialized;         // 1 if hardware successfully init'd, else 0

   LONG     PM_ISR;              // -1 if no PM ISR hooked, else IRQ #

   HTIMER   server;              // DRV_SERVE periodic timer, if requested

                                 // Vector to high-level destructor, if any

   void (cdecl *destructor)(void *);

                                 // High-level descriptor (DIG_ or MDI_DRIVER)
   void        *descriptor;
}
AIL_DRIVER;

struct _DIG_DRIVER;

typedef struct _SAMPLE           // Sample instance
{
   struct _DIG_DRIVER *driver;   // Driver for playback

   ULONG    status;              // SMP_ flags: _FREE, _DONE, _PLAYING

   void    *start[2];            // Sample buffer address (W)
   ULONG    len  [2];            // Sample buffer size in bytes (W)
   ULONG    pos  [2];            // Index to next byte (R/W)
   ULONG    done [2];            // Nonzero if buffer with len=0 sent by app

   LONG     current_buffer;      // Buffer # active (0/1)
   LONG     last_buffer;         // Last active buffer (for double-buffering)

   LONG     loop_count;          // # of cycles-1 (0=one-shot, -1=indefinite)

   LONG     format;              // DIG_F format (8/16 bits, mono/stereo)
   ULONG    flags;               // DIG_PCM_SIGN / DIG_PCM_ORDER (stereo only)

   LONG     playback_rate;       // Playback rate in hertz

   LONG     volume;              // Sample volume 0-127
   LONG     pan;                 // Mono panpot/stereo balance (0=L ... 127=R)

   LONG     vol_scale[2][256];   // [left/mono=0,right=1][256] channel volume

   CALLBACK SOB;                 // Start-of-block callback function
   CALLBACK EOB;                 // End-of-buffer callback function
   CALLBACK EOS;                 // End-of-sample callback function

   LONG     user_data  [8];      // Miscellaneous user data
   LONG     system_data[8];      // Miscellaneous system data
}
SAMPLE;

typedef SAMPLE * HSAMPLE;        // Handle to sample

typedef struct _DIG_DRIVER          // Handle to digital audio driver
{
   AIL_DRIVER *drvr;                // Base driver descriptor

   DIG_DDT    *DDT;                 // Protected-mode pointer to DDT
   DIG_DST    *DST;                 // Protected-mode pointer to DST

   HTIMER      timer;               // Buffer-polling timer

   LONG        half_buffer_size;    // Size of DMA half-buffer
   LONG        DMA_rate;            // Hardware sample rate
   LONG        hw_format;           // DIG_F code in use
   ULONG       hw_mode_flags;       // DIG_PCM_ flags for mode in use

   REALFAR     DMA_seg;             // Seg:off pointer to DMA buffers (off=0)
   ULONG       DMA_sel;             // Selector for DMA buffers (off=0)
   void       *DMA_buf;             // Protected-mode pointer to DMA buffers

   void       *DMA[2];              // Protected-mode pointers to half-buffers
                                    // (note that DMA[0] may != DMA_buf)

   WORD       *buffer_flag;         // Protected-mode pointer to buffer flag
   LONG        last_buffer;         // Last active buffer flag value in driver

   LONG        channels_per_sample; // # of channels per sample (1 or 2)
   LONG        bytes_per_channel;   // # of bytes per channel (1 or 2)
   LONG        channels_per_buffer; // # of channels per half-buffer
   LONG        samples_per_buffer;  // # of samples per half-buffer

   LONG        build_size;          // # of bytes in build buffer
   LONG       *build_buffer;        // Build buffer (4 * n_samples bytes)

   LONG        playing;             // Playback active if non-zero
   LONG        quiet;               // # of consecutive quiet sample periods

   SAMPLE     *samples;             // Pointer to list of SAMPLEs
   LONG        n_samples;           // # of SAMPLEs

   LONG        n_active_samples;    // # of samples being processed

   LONG        master_volume;       // Master sample volume 0-127

   LONG        system_data[8];      // Miscellaneous system data
}
DIG_DRIVER;

typedef DIG_DRIVER * HDIGDRIVER;    // WAIL-compatible typedef

typedef struct                      // MIDI status log structure
   {
   LONG     program   [NUM_CHANS];  // Program Change
   LONG     pitch_l   [NUM_CHANS];  // Pitch Bend LSB
   LONG     pitch_h   [NUM_CHANS];  // Pitch Bend MSB

   LONG     c_lock    [NUM_CHANS];  // Channel Lock
   LONG     c_prot    [NUM_CHANS];  // Channel Lock Protection
   LONG     c_mute    [NUM_CHANS];  // Channel Mute
   LONG     c_v_prot  [NUM_CHANS];  // Voice Protection
   LONG     bank      [NUM_CHANS];  // Patch Bank Select
   LONG     indirect  [NUM_CHANS];  // ICA indirect controller value
   LONG     callback  [NUM_CHANS];  // Callback Trigger

   LONG     mod       [NUM_CHANS];  // Modulation
   LONG     vol       [NUM_CHANS];  // Volume
   LONG     pan       [NUM_CHANS];  // Panpot
   LONG     exp       [NUM_CHANS];  // Expression
   LONG     sus       [NUM_CHANS];  // Sustain
   LONG     reverb    [NUM_CHANS];  // Reverb
   LONG     chorus    [NUM_CHANS];  // Chorus

   LONG     bend_range[NUM_CHANS];  // Bender Range (data MSB, RPN 0 assumed)
   }
CTRL_LOG;

struct _MDI_DRIVER;

typedef struct                            // XMIDI sequence state table
{
   struct _MDI_DRIVER *driver;            // Driver for playback

   ULONG    status;                       // SEQ_ flags

   void    *TIMB;                         // XMIDI IFF chunk pointers
   void    *RBRN;
   void    *EVNT;

   UBYTE   *EVNT_ptr;                     // Current event pointer

   UBYTE   *ICA;                          // Indirect Controller Array

   CALLBACK prefix_callback;              // XMIDI Callback Prefix handler
   CALLBACK trigger_callback;             // XMIDI Callback Trigger handler
   CALLBACK beat_callback;                // XMIDI beat/bar change handler
   CALLBACK EOS;                          // End-of-sequence callback function

   LONG     loop_count;                   // 0=one-shot, -1=indefinite, ...

   LONG     interval_count;               // # of intervals until next event
   LONG     interval_num;                 // # of intervals since start

   LONG     volume;                       // Sequence volume 0-127
   LONG     volume_target;                // Target sequence volume 0-127
   LONG     volume_accum;                 // Accumulated volume period
   LONG     volume_period;                // Period for volume stepping

   LONG     tempo_percent;                // Relative tempo percentage 0-100
   LONG     tempo_target;                 // Target tempo 0-100
   LONG     tempo_accum;                  // Accumulated tempo period
   LONG     tempo_period;                 // Period for tempo stepping
   LONG     tempo_error;                  // Error counter for tempo DDA

   LONG     beat_count;                   // Sequence playback position
   LONG     measure_count;

   LONG     time_numerator;               // Sequence timing data
   LONG     time_fraction;
   LONG     beat_fraction;
   LONG     time_per_beat;

   void    *FOR_ptrs       [FOR_NEST];    // Loop stack
   LONG     FOR_loop_count [FOR_NEST];

   LONG     chan_map       [NUM_CHANS];   // Physical channel map for sequence

   CTRL_LOG shadow;                       // Controller values for sequence

   LONG     note_count;                   // # of notes "on"

   LONG     note_chan      [MAX_NOTES];   // Channel for queued note (-1=free)
   LONG     note_num       [MAX_NOTES];   // Note # for queued note
   LONG     note_time      [MAX_NOTES];   // Remaining duration in intervals

   LONG     user_data  [8];               // Miscellaneous user data
   LONG     system_data[8];               // Miscellaneous system data
}
SEQUENCE;

typedef SEQUENCE * HSEQUENCE;       // Handle to sequence

typedef struct _MDI_DRIVER          // Handle to XMIDI driver
{
   AIL_DRIVER *drvr;                // Base driver descriptor

   MDI_DDT    *DDT;                 // Protected-mode pointer to DDT
   MDI_DST    *DST;                 // Protected-mode pointer to DST

   HTIMER      timer;               // XMIDI quantization timer
   LONG        interval_time;       // XMIDI quantization timer interval in uS

   LONG        disable;             // > 0 to disable XMIDI service

   SEQUENCE   *sequences;           // Pointer to list of SEQUENCEs
   LONG        n_sequences;         // # of SEQUENCEs

   LONG        lock  [NUM_CHANS];   // 1 if locked, 2 if protected, else 0
   HSEQUENCE   locker[NUM_CHANS];   // HSEQUENCE which locked channel
   HSEQUENCE   owner [NUM_CHANS];   // HSEQUENCE which owned locked channel
   HSEQUENCE   user  [NUM_CHANS];   // Last sequence to use channel
   LONG        state [NUM_CHANS];   // Lock state prior to being locked

   LONG        notes [NUM_CHANS];   // # of active notes in channel

   CALLBACK    event_trap;          // MIDI event trap callback function
   CALLBACK    timbre_trap;         // MIDI timbre request callback function

   LONG        message_count;       // MIDI message count
   LONG        offset;              // MIDI buffer offset

   LONG        master_volume;       // Master XMIDI note volume 0-127

   LONG        system_data[8];      // Miscellaneous system data
}
MDI_DRIVER;

typedef MDI_DRIVER * HMDIDRIVER;    // WAIL-compatible typedef

typedef struct                      // XMIDI TIMB IFF chunk
   {
   BYTE  name[4];

   UBYTE msb;
   UBYTE lsb;
   UBYTE lsb2;
   UBYTE lsb3;

   UWORD n_entries;

   UWORD timbre[1];
   }
TIMB_chunk;

typedef struct                      // XMIDI RBRN IFF entry
   {
   WORD  bnum;
   ULONG offset;
   }
RBRN_entry;

typedef struct                      // Wave library entry
{
   LONG  bank;                      // XMIDI bank, MIDI patch for sample
   LONG  patch;

   LONG  root_key;                  // Root MIDI note # for sample (or -1)

   ULONG file_offset;               // Offset of wave data from start-of-file
   ULONG size;                      // Size of wave sample in bytes

   LONG  format;                    // DIG_F format (8/16 bits, mono/stereo)
   ULONG flags;                     // DIG_PCM_SIGN / DIG_PCM_ORDER (stereo)
   LONG  playback_rate;             // Playback rate in hertz
}
WAVE_ENTRY;

typedef struct                      // Virtual "wave synthesizer" descriptor
{
   HMDIDRIVER  mdi;                 // MIDI driver for use with synthesizer
   HDIGDRIVER  dig;                 // Digital driver for use with synthesizer

   WAVE_ENTRY *library;             // Pointer to wave library

   CALLBACK   *prev_event_fn;       // Previous MIDI event trap function
   CALLBACK   *prev_timb_fn;        // Previous timbre request trap function

   CTRL_LOG    controls;            // MIDI controller states

   WAVE_ENTRY *wave [NUM_CHANS];    // Pointer to WAVE_ENTRY for each channel

   HSAMPLE     S    [MAX_W_VOICES]; // List of HSAMPLE voices
   LONG        n_voices;            // Actual # of voices allocated to synth

   LONG        chan [MAX_W_VOICES]; // MIDI channel for each voice, or -1
   LONG        note [MAX_W_VOICES]; // MIDI note number for voice
   LONG        root [MAX_W_VOICES]; // MIDI root note for voice
   LONG        rate [MAX_W_VOICES]; // Playback rate for voice
   LONG        vel  [MAX_W_VOICES]; // MIDI note velocity for voice
   ULONG       time [MAX_W_VOICES]; // Timestamp for voice

   ULONG       event;               // Event counter for LRU timestamps
}
WAVE_SYNTH;

typedef WAVE_SYNTH * HWAVE;         // Handle to virtual wave synthesizer

typedef struct                      // VDI interface register structure
{
   WORD AX;
   WORD BX;
   WORD CX;
   WORD DX;
   WORD SI;
   WORD DI;
}
VDI_CALL;

//
// Global preference array
//

extern LONG           AIL_preference   [N_PREFS];

//
// AIL fatal error type string
//

extern BYTE           AIL_error        [256];

//
// Last IO_PARMS structure used to attempt device detection
//

extern IO_PARMS       AIL_last_IO_attempt;

//
// High-level support services
//

extern void    cdecl  AIL_startup                   (void);
extern void    cdecl  AIL_shutdown                  (void);

extern LONG    cdecl  AIL_set_preference            (ULONG       number,
                                                     LONG        value);
//
// Low-level support services
//

extern void    cdecl  AIL_lock                      (void);
extern void    cdecl  AIL_unlock                    (void);

extern REALFAR cdecl  AIL_get_real_vect             (ULONG       vectnum);
extern void    cdecl  AIL_set_real_vect             (ULONG       vectnum,
                                                     REALFAR     real_ptr);

extern void    cdecl  AIL_set_USE16_ISR             (LONG        IRQ,
                                                     REALFAR     real_base,
                                                     ULONG       ISR_offset);

extern void    cdecl  AIL_restore_USE16_ISR         (LONG        IRQ);

extern ULONG   cdecl  AIL_disable_interrupts        (void);
extern void    cdecl  AIL_restore_interrupts        (ULONG       FD_register);

extern void    cdecl  AIL_switch_stack              (void       *stack,
                                                     ULONG       size,
                                                     ULONG      *SS,
                                                     void      **ESP,
                                                     void      **EBP);

extern void    cdecl  AIL_restore_stack             (ULONG       SS,
                                                     void       *ESP,
                                                     void       *EBP);

extern LONG    cdecl  AIL_call_driver               (AIL_DRIVER *drvr,
                                                     LONG        fn,
                                                     VDI_CALL   *in,
                                                     VDI_CALL   *out);

extern void    cdecl  AIL_delay                     (LONG        intervals);

extern LONG    cdecl  AIL_background                (void);

extern LONG    cdecl  AIL_read_INI                  (AIL_INI    *INI,
                                                     BYTE       *filename);

//
// Process services
//

extern HTIMER  cdecl  AIL_register_timer            (CALLBACK    callback_fn);

extern ULONG   cdecl  AIL_set_timer_user            (HTIMER      timer,
                                                     ULONG       user);

extern void    cdecl  AIL_set_timer_period          (HTIMER      timer,
                                                     ULONG       microseconds);

extern void    cdecl  AIL_set_timer_frequency       (HTIMER      timer,
                                                     ULONG       hertz);

extern void    cdecl  AIL_set_timer_divisor         (HTIMER      timer,
                                                     ULONG       PIT_divisor);

extern ULONG   cdecl  AIL_interrupt_divisor         (void);

extern void    cdecl  AIL_start_timer               (HTIMER      timer);
extern void    cdecl  AIL_start_all_timers          (void);

extern void    cdecl  AIL_stop_timer                (HTIMER      timer);
extern void    cdecl  AIL_stop_all_timers           (void);

extern void    cdecl  AIL_release_timer_handle      (HTIMER      timer);
extern void    cdecl  AIL_release_all_timers        (void);

//
// Low-level installation services
//

extern IO_PARMS *   cdecl AIL_get_IO_environment    (AIL_DRIVER *drvr);

extern AIL_DRIVER * cdecl AIL_install_driver        (UBYTE      *driver_image,
                                                     ULONG       n_bytes);

extern void         cdecl AIL_uninstall_driver      (AIL_DRIVER *drvr);

//
// SoundStream(TM) high-level digital services
//

extern LONG         cdecl AIL_install_DIG_INI       (HDIGDRIVER *dig);

extern HDIGDRIVER   cdecl AIL_install_DIG_driver_file
                                                    (BYTE       *filename,
                                                     IO_PARMS   *IO);

extern void         cdecl AIL_uninstall_DIG_driver  (HDIGDRIVER  dig);

extern HSAMPLE      cdecl AIL_allocate_sample_handle
                                                    (HDIGDRIVER  dig);

extern HSAMPLE      cdecl AIL_allocate_file_sample  (HDIGDRIVER  dig,
                                                     void       *file_image,
                                                     LONG        block);

extern void         cdecl AIL_release_sample_handle (HSAMPLE S);

extern void         cdecl AIL_init_sample           (HSAMPLE S);

extern LONG         cdecl AIL_set_sample_file       (HSAMPLE S,
                                                     void   *file_image,
                                                     LONG    block);

extern void         cdecl AIL_set_sample_address    (HSAMPLE S,
                                                     void   *start,
                                                     ULONG   len);

extern void         cdecl AIL_set_sample_type       (HSAMPLE S,
                                                     LONG    format,
                                                     ULONG   flags);

extern void         cdecl AIL_start_sample          (HSAMPLE S);
extern void         cdecl AIL_stop_sample           (HSAMPLE S);
extern void         cdecl AIL_resume_sample         (HSAMPLE S);
extern void         cdecl AIL_end_sample            (HSAMPLE S);

extern void         cdecl AIL_set_sample_playback_rate
                                                    (HSAMPLE S,
                                                     LONG    playback_rate);

extern void         cdecl AIL_set_sample_volume     (HSAMPLE S,
                                                     LONG    volume);

extern void         cdecl AIL_set_sample_pan        (HSAMPLE S,
                                                     LONG    pan);

extern void         cdecl AIL_set_sample_loop_count (HSAMPLE S,
                                                     LONG    loop_count);

extern ULONG        cdecl AIL_sample_status         (HSAMPLE S);
extern LONG         cdecl AIL_sample_playback_rate  (HSAMPLE S);
extern LONG         cdecl AIL_sample_volume         (HSAMPLE S);
extern LONG         cdecl AIL_sample_pan            (HSAMPLE S);
extern LONG         cdecl AIL_sample_loop_count     (HSAMPLE S);

extern void         cdecl AIL_set_digital_master_volume
                                                    (HDIGDRIVER dig,
                                                     LONG       master_volume);

extern LONG         cdecl AIL_digital_master_volume (HDIGDRIVER dig);

//
// SoundStream(TM) low-level digital services
//

extern HDIGDRIVER   cdecl AIL_install_DIG_driver_image
                                                    (void     *driver_image,
                                                     ULONG     size,
                                                     IO_PARMS *IO);

extern LONG     cdecl AIL_minimum_sample_buffer_size(HDIGDRIVER  dig,
                                                     LONG        playback_rate,
                                                     LONG        format);

extern LONG     cdecl AIL_sample_buffer_ready       (HSAMPLE S);

extern void     cdecl AIL_load_sample_buffer        (HSAMPLE S,
                                                     ULONG   buff_num,
                                                     void   *buffer,
                                                     ULONG   len);

extern void     cdecl AIL_set_sample_position       (HSAMPLE S,
                                                     ULONG   pos);

extern ULONG    cdecl AIL_sample_position           (HSAMPLE S);

extern CALLBACK cdecl AIL_register_SOB_callback     (HSAMPLE S,
                                                     CALLBACK SOB);

extern CALLBACK cdecl AIL_register_EOB_callback     (HSAMPLE S,
                                                     CALLBACK EOB);

extern CALLBACK cdecl AIL_register_EOS_callback     (HSAMPLE S,
                                                     CALLBACK EOS);

extern CALLBACK cdecl AIL_register_EOF_callback     (HSAMPLE S,
                                                     CALLBACK EOFILE);

extern void     cdecl AIL_set_sample_user_data      (HSAMPLE S,
                                                     ULONG   index,
                                                     LONG    value);

extern LONG     cdecl AIL_sample_user_data          (HSAMPLE S,
                                                     ULONG   index);

extern LONG     cdecl AIL_active_sample_count       (HDIGDRIVER  dig);

//
// High-level XMIDI services
//

extern LONG         cdecl AIL_install_MDI_INI       (HMDIDRIVER *mdi);

extern HMDIDRIVER   cdecl AIL_install_MDI_driver_file
                                                    (BYTE       *filename,
                                                     IO_PARMS   *IO);

extern void         cdecl AIL_uninstall_MDI_driver  (HMDIDRIVER  mdi);

extern HSEQUENCE    cdecl AIL_allocate_sequence_handle
                                                    (HMDIDRIVER  mdi);

extern void         cdecl AIL_release_sequence_handle
                                                    (HSEQUENCE S);

extern LONG         cdecl AIL_init_sequence         (HSEQUENCE S,
                                                     void     *start,
                                                     LONG      sequence_num);

extern void         cdecl AIL_start_sequence        (HSEQUENCE S);
extern void         cdecl AIL_stop_sequence         (HSEQUENCE S);
extern void         cdecl AIL_resume_sequence       (HSEQUENCE S);
extern void         cdecl AIL_end_sequence          (HSEQUENCE S);

extern void         cdecl AIL_set_sequence_tempo    (HSEQUENCE S,
                                                     LONG      tempo,
                                                     LONG      milliseconds);

extern void         cdecl AIL_set_sequence_volume   (HSEQUENCE S,
                                                     LONG      volume,
                                                     LONG      milliseconds);

extern void         cdecl AIL_set_sequence_loop_count
                                                    (HSEQUENCE S,
                                                     LONG      loop_count);

extern ULONG        cdecl AIL_sequence_status       (HSEQUENCE S);

extern LONG         cdecl AIL_sequence_tempo        (HSEQUENCE S);
extern LONG         cdecl AIL_sequence_volume       (HSEQUENCE S);
extern LONG         cdecl AIL_sequence_loop_count   (HSEQUENCE S);

extern void         cdecl AIL_set_XMIDI_master_volume
                                                    (HMDIDRIVER mdi,
                                                     LONG       master_volume);

extern LONG         cdecl AIL_XMIDI_master_volume   (HMDIDRIVER mdi);


//
// Low-level XMIDI services
//

extern HMDIDRIVER   cdecl AIL_install_MDI_driver_image
                                                    (void       *driver_image,
                                                     ULONG       size,
                                                     IO_PARMS   *IO);

extern LONG     cdecl AIL_MDI_driver_type           (HMDIDRIVER  mdi);

extern void     cdecl AIL_set_GTL_filename_prefix   (BYTE       *prefix);

extern LONG     cdecl AIL_timbre_status             (HMDIDRIVER  mdi,
                                                     LONG        bank,
                                                     LONG        patch);

extern LONG     cdecl AIL_install_timbre            (HMDIDRIVER  mdi,
                                                     LONG        bank,
                                                     LONG        patch);

extern void     cdecl AIL_protect_timbre            (HMDIDRIVER  mdi,
                                                     LONG        bank,
                                                     LONG        patch);

extern void     cdecl AIL_unprotect_timbre          (HMDIDRIVER  mdi,
                                                     LONG        bank,
                                                     LONG        patch);

extern LONG     cdecl AIL_active_sequence_count     (HMDIDRIVER  mdi);

extern LONG     cdecl AIL_controller_value          (HSEQUENCE S,
                                                     LONG      channel,
                                                     LONG      controller_num);

extern LONG     cdecl AIL_channel_notes             (HSEQUENCE S,
                                                     LONG      channel);

extern void     cdecl AIL_sequence_position         (HSEQUENCE S,
                                                     LONG     *beat,
                                                     LONG     *measure);

extern void     cdecl AIL_branch_index              (HSEQUENCE S,
                                                     ULONG     marker);

extern CALLBACK cdecl AIL_register_prefix_callback  (HSEQUENCE S,
                                                     CALLBACK  callback);

extern CALLBACK cdecl AIL_register_trigger_callback (HSEQUENCE S,
                                                     CALLBACK  callback);

extern CALLBACK cdecl AIL_register_sequence_callback
                                                    (HSEQUENCE S,
                                                     CALLBACK  callback);

extern CALLBACK cdecl AIL_register_beat_callback    (HSEQUENCE   S,
                                                     CALLBACK    callback);

extern CALLBACK cdecl AIL_register_event_callback   (HMDIDRIVER  mdi,
                                                     CALLBACK    callback);

extern CALLBACK cdecl AIL_register_timbre_callback  (HMDIDRIVER  mdi,
                                                     CALLBACK    callback);

extern void     cdecl AIL_set_sequence_user_data    (HSEQUENCE S,
                                                     ULONG     index,
                                                     LONG      value);

extern LONG     cdecl AIL_sequence_user_data        (HSEQUENCE S,
                                                     ULONG     index);

extern void     cdecl AIL_register_ICA_array        (HSEQUENCE S,
                                                     BYTE     *array);

extern LONG     cdecl AIL_lock_channel              (HMDIDRIVER  mdi);

extern void     cdecl AIL_release_channel           (HMDIDRIVER  mdi,
                                                     LONG        channel);

extern void     cdecl AIL_map_sequence_channel      (HSEQUENCE S,
                                                     LONG      seq_channel,
                                                     LONG      new_channel);

extern LONG     cdecl AIL_true_sequence_channel     (HSEQUENCE S,
                                                     LONG      seq_channel);

extern void     cdecl AIL_send_channel_voice_message
                                                    (HMDIDRIVER  mdi,
                                                     HSEQUENCE   S,
                                                     LONG        status,
                                                     LONG        data_1,
                                                     LONG        data_2);

extern void     cdecl AIL_send_sysex_message        (HMDIDRIVER  mdi,
                                                     void       *buffer);

extern HWAVE    cdecl AIL_create_wave_synthesizer   (HDIGDRIVER  dig,
                                                     HMDIDRIVER  mdi,
                                                     void       *wave_lib,
                                                     LONG        polyphony);

extern void     cdecl AIL_destroy_wave_synthesizer  (HWAVE W);

#ifdef __cplusplus
}
#endif

//
// MetaWare support
//

#ifdef __HIGHC__
#pragma Global_aliasing_convention();
#endif

#endif

#endif