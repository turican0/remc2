// This is ail3_090.h
//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                        лл
//лл  IBM Audio Interface Library                                           лл
//лл                                                                        лл
//лл  AIL.H: API function prototypes                                        лл
//лл                                                                        лл
//лл  Flat-model source compatible with IBM 32-bit ANSI C/C++               лл
//лл                                                                        лл
//лл  Version 3.00 of 11-Apr-94: Initial version for AIL 3.0                лл
//лл                                                                        лл
//лл  Author: John Miles                                                    лл
//лл                                                                        лл
//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                        лл
//лл  Copyright (C) 1994 Miles Design, Inc.                                 лл
//лл                                                                        лл
//лл  Miles Design, Inc.                                                    лл
//лл  6702 Cat Creek Trail                                                  лл
//лл  Austin, TX 78731                                                      лл
//лл                                                                        лл
//лл  (512) 345-2642 / FAX (512) 346-9596 / BBS (512) 454-9990              лл
//лл                                                                        лл
//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

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

#define DIG_F_MONO_8             0        // PCM data formats
#define DIG_F_MONO_16            1
#define DIG_F_STEREO_8           2
#define DIG_F_STEREO_16          3

#define DIG_PCM_SIGN             0x0001   // PCM flags
#define DIG_PCM_ORDER            0x0002

#define DIG_PCM_POLARITY         0x0004   // PCM flags used by driver hardware
#define DIG_PCM_SPLIT            0x0008
#define DIG_BUFFER_SERVICE       0x0010
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
// SAMPLE.status flag values
//

#define SMP_FREE          0x0001    // Sample is available for allocation

#define SMP_DONE          0x0002    // Sample has finished playing, or has 
                                    // never been started

#define SMP_PLAYING       0x0004    // Sample in "play" mode if 1, "stop" if 0

//
// General type definitions for portability
// 

#ifndef TYPEDEFS
#define TYPEDEFS

typedef unsigned char  UBYTE;
typedef unsigned short UWORD;
typedef unsigned long  ULONG;
typedef          char  BYTE;
typedef signed   short WORD;
typedef signed   long  LONG;

#endif

typedef void * CALLBACK;            // Generic callback function prototype

typedef ULONG REALFAR;              // Real-mode far pointer type

#ifdef __HIGHC__                    // Macro to convert REALFAR to protected *
#else
#ifndef REALPTR
#define REALPTR(x) ((void *) (ULONG) (((x)>>16<<4) + ((x) & 0xffff)))
#endif
#endif

#define MIN 0
#define NOM 1
#define MAX 2

#ifndef YES
#define YES 1
#endif

#ifndef NO
#define NO  0
#endif

#define DIG_SERVICE_RATE          0
#define DEFAULT_DSR               200   // DMA buffer-polling rate = 200 Hz
                                  
#define DIG_HARDWARE_SAMPLE_RATE  1
#define DEFAULT_DHSR              NOM   // Use nominal sample rate by default
                                  
#define DIG_DMA_BUFFER_SIZE       2
#define DEFAULT_DBS               MAX   // Use largest DMA buffer by default
                                  
#define DIG_MIXER_CHANNELS        3     
#define DEFAULT_DMC               16    // # of allocatable SAMPLE structures

#define DIG_DEFAULT_VOLUME        4   
#define DEFAULT_DDV               100   // Default sample volume = 100 (0-127)

#define DIG_RESAMPLING_TOLERANCE  5
#define DEFAULT_DRT               655   // Resampling triggered at +/- 1%
                                  
#define DIG_USE_STEREO            6
#define DEFAULT_DUS               NO    // Use mono output only
                                  
#define DIG_USE_16_BITS           7
#define DEFAULT_DU16              YES   // Use 16-bit output if available
                                  
#define DIG_ALLOW_16_BIT_DMA      8
#define DEFAULT_DA16DMA           YES   // OK to use 16-bit DMA if necessary

#define AIL_SCAN_FOR_HARDWARE     9
#define DEFAULT_ASH               YES   // Scan for I/O settings if necessary

#define N_PREFS 10                      // # of preference types

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
}  
VDI_HDR;

typedef struct
{
   UWORD minimum_physical_sample_rate;
   UWORD nominal_physical_sample_rate;
   UWORD maximum_physical_sample_rate;

   UWORD minimum_DMA_half_buffer_size;
   UWORD nominal_DMA_half_buffer_size;
   UWORD maximum_DMA_half_buffer_size;

   ULONG flags;
}
DIG_MODE;

typedef struct
{
   REALFAR  DMA_buffer_area;

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

typedef struct                   // Handle to driver
{
   REALFAR  seg;                 // Seg:off pointer to driver (off=0)
   void    *buf;                 // Protected-mode pointer to driver
   VDI_HDR *VHDR;                // Pointer to driver header (same as buf)
   LONG     type;                // AIL3DIG or AIL3MDI (see below)

   HTIMER   server;              // DRV_SERVE periodic timer, if requested

   void   (*destructor)(void *); // Vector to high-level destructor, if any
   void    *descriptor;          // High-level descriptor (DIG_ or MDI_DRIVER)
}
AIL_DRIVER;

struct _DIG_DRIVER;

typedef struct _SAMPLE           // Sample instance
{
   struct _DIG_DRIVER *driver;   // Driver for playback

   ULONG    status;              // SMP_ flags: _FREE, _DONE, _PLAYING

   void    *start[2];            // Sample buffer address (W)
   ULONG    len[2];              // Sample buffer size in bytes (W)
                     
   ULONG    pos;                 // Index to next byte in current buffer (R/W)
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

typedef struct _DIG_DRIVER
{
   AIL_DRIVER *drvr;                // Base driver descriptor

   DIG_DDT    *DDT;                 // Protected-mode pointer to DDT
   DIG_DST    *DST;                 // Protected-mode pointer to DST

   HTIMER      timer;               // Buffer-polling timer

   LONG        half_buffer_size;    // Size of DMA half-buffer
   LONG        DMA_rate;            // Hardware sample rate
   LONG        hw_format;           // DIG_F code in use
   ULONG       hw_mode_flags;       // DIG_PCM_ flags for mode in use

   void       *DMA[2];              // Protected-mode pointers to DMA buffers
        
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

   SAMPLE     *samples;             // Pointer to list of logical SAMPLEs
   LONG        n_samples;           // # of logical SAMPLEs

   LONG        n_active_samples;    // # of samples being processed
}
DIG_DRIVER;

typedef struct _MDI_DRIVER
{
   AIL_DRIVER *drvr;                // Base driver descriptor

   MDI_DDT *DDT;                    // Protected-mode pointer to DDT
   MDI_DST *DST;                    // Protected-mode pointer to DST
}
MDI_DRIVER;

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

extern LONG AIL_preference[N_PREFS];

//
// AIL fatal error type string
//

extern BYTE AIL_error[256];

//
// High-level support services
//

extern void           AIL_startup(void);
extern void           AIL_shutdown(void);

extern LONG           AIL_set_preference(ULONG number, LONG value);

//
// Low-level support services
//

extern ULONG  cdecl   AIL_get_real_vect             (ULONG       vectnum);
extern void   cdecl   AIL_set_real_vect             (ULONG       vectnum, 
                                                     ULONG       real_ptr);

extern ULONG  cdecl   AIL_disable_interrupts        (void);
extern ULONG  cdecl   AIL_restore_interrupts        (ULONG       FD_register);

extern void   cdecl   AIL_switch_stack              (void       *stack, 
                                                     ULONG       size, 
                                                     ULONG      *SS, 
                                                     void      **ESP, 
                                                     void      **EBP);

extern void   cdecl   AIL_restore_stack             (ULONG       SS, 
                                                     void       *ESP, 
                                                     void       *EBP);

extern LONG   cdecl   AIL_call_driver               (AIL_DRIVER *drvr, 
                                                     LONG        fn,
                                                     VDI_CALL   *in, 
                                                     VDI_CALL   *out);
//                 
// Process services
//

extern HTIMER cdecl   AIL_register_timer            (CALLBACK    callback_fn);

extern ULONG  cdecl   AIL_set_timer_user            (HTIMER      timer, 
                                                     ULONG       user);

extern void   cdecl   AIL_set_timer_period          (HTIMER      timer, 
                                                     ULONG       microseconds);

extern void   cdecl   AIL_set_timer_frequency       (HTIMER      timer, 
                                                     ULONG       hertz);

extern void   cdecl   AIL_set_timer_divisor         (HTIMER      timer, 
                                                     ULONG       PIT_divisor);

extern ULONG  cdecl   AIL_interrupt_divisor         (void);

extern void   cdecl   AIL_start_timer               (HTIMER      timer);
extern void   cdecl   AIL_start_all_timers          (void);

extern void   cdecl   AIL_stop_timer                (HTIMER      timer);
extern void   cdecl   AIL_stop_all_timers           (void);

extern void   cdecl   AIL_release_timer_handle      (HTIMER      timer);
extern void   cdecl   AIL_release_all_timers        (void);

//
// Low-level installation services
//

extern IO_PARMS *     AIL_get_IO_environment        (AIL_DRIVER *drvr);

extern AIL_DRIVER *   AIL_install_driver            (UBYTE      *driver_image, 
                                                     ULONG       n_bytes);

extern void           AIL_uninstall_driver          (AIL_DRIVER *drvr);

extern DIG_DRIVER *   AIL_install_DIG_driver_image  (void       *driver_image,
                                                     ULONG       size,
                                                     IO_PARMS   *IO);
//
// High-level installation services
//

extern DIG_DRIVER *   AIL_install_DIG_driver_file   (BYTE       *filename, 
                                                     IO_PARMS   *IO);

extern void           AIL_uninstall_DIG_driver      (DIG_DRIVER *dig);

//
// SoundStream(TM) high-level digital services
//

extern HSAMPLE        AIL_allocate_sample           (DIG_DRIVER *dig);

extern HSAMPLE        AIL_allocate_file_sample      (DIG_DRIVER *dig, 
                                                     void       *file_image, 
                                                     LONG        block);

extern void           AIL_free_sample               (HSAMPLE     sample);
                                                  
extern void           AIL_init_sample               (HSAMPLE S);

extern void           AIL_set_sample_address        (HSAMPLE S, 
                                                     void   *start, 
                                                     ULONG   len);
                                                  
extern void           AIL_set_sample_type           (HSAMPLE S, 
                                                     LONG    format, 
                                                     ULONG   flags);

extern void           AIL_start_sample              (HSAMPLE S);
extern void           AIL_stop_sample               (HSAMPLE S);
extern void           AIL_resume_sample             (HSAMPLE S);
extern void           AIL_end_sample                (HSAMPLE S);

extern void           AIL_set_sample_playback_rate  (HSAMPLE S, 
                                                     LONG    playback_rate);

extern void           AIL_set_sample_volume         (HSAMPLE S, 
                                                     LONG    volume);

extern void           AIL_set_sample_pan            (HSAMPLE S, 
                                                     LONG    pan);

extern void           AIL_set_sample_loop_count     (HSAMPLE S, 
                                                     LONG    loop_count);

extern ULONG          AIL_sample_status             (HSAMPLE S);
extern LONG           AIL_sample_playback_rate      (HSAMPLE S);
extern LONG           AIL_sample_volume             (HSAMPLE S);
extern LONG           AIL_sample_pan                (HSAMPLE S);
extern LONG           AIL_sample_loop_count         (HSAMPLE S);

//
// SoundStream(TM) low-level digital services
//

extern LONG           AIL_sample_buffer_ready       (HSAMPLE S);

extern void           AIL_load_sample_buffer        (HSAMPLE S,
                                                     ULONG   buff_num,
                                                     void   *buffer,
                                                     ULONG   len);

extern void           AIL_set_sample_position       (HSAMPLE S, 
                                                     ULONG   pos);

extern ULONG          AIL_sample_position           (HSAMPLE S);

extern CALLBACK       AIL_register_SOB_callback     (HSAMPLE S, 
                                                     CALLBACK SOB);

extern CALLBACK       AIL_register_EOB_callback     (HSAMPLE S, 
                                                     CALLBACK EOB);

extern CALLBACK       AIL_register_EOS_callback     (HSAMPLE S, 
                                                     CALLBACK EOS);

extern void           AIL_set_sample_user_data      (HSAMPLE S,
                                                     ULONG   index,
                                                     LONG    value);

extern LONG           AIL_sample_user_data          (HSAMPLE S,
                                                     ULONG   index);

extern LONG           AIL_active_sample_count       (DIG_DRIVER *dig);

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
