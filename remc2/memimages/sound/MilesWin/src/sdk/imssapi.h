#ifndef IMSSAPI_H
#define IMSSAPI_H

// Internal API file

#ifdef IS_PS3
#include "string.h"
#endif

#ifdef IS_WII
#include "string.h"
#include "math.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef IS_PS2
#pragma pack(1)
#endif

#ifdef IS_32
  #if defined(IS_MAC) || defined(IS_LINUX) || defined(IS_PS2) || defined(IS_PS3) || defined(IS_WII)
    #define ASMLINK
    #define ASMPARM
  #else
     #if defined(IS_XENON)
       #define ASMLINK __cdecl
       #define ASMPARM register
     #else
       #define ASMLINK __cdecl
       #define ASMPARM
     #endif
  #endif
#else
  #define ASMLINK WINAPI
  #define ASMPARM
#endif

#ifndef YES
#define YES 1
#endif

#ifndef NO
#define NO  0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE  0
#endif

#ifdef IS_MAC

#if !defined(max)
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#endif
#if !defined(min)
#define min(a,b)  (((a) < (b)) ? (a) : (b))
#endif

#endif

#ifdef IS_WATCOM

#if !defined(max) // Watcom stdlib.h doesn't define these for C++
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#endif
#if !defined(min)
#define min(a,b)  (((a) < (b)) ? (a) : (b))
#endif

#endif


//
// .VOC file header
//

typedef MSS_STRUCT
{
   S8  ID_string[20];

   U16 data_offset;
   U16 version;
   U16 ID_code;
}
VOC;

U32 AILCALL XMI_read_VLN(U8 const FAR* FAR *ptr);
S32 AILCALL XMI_message_size(S32 status);
void const FAR * AILCALL XMI_find_sequence(U8 const FAR *image, S32 sequence);
void AIL_process_WAV_image( AILSOUNDINFO FAR const * info, HSAMPLE sample );

//############################################################################
//##                                                                        ##
//## API function prototypes                                                ##
//##                                                                        ##
//############################################################################

#ifdef IS_XBOX
  extern S32 AILCALL check_hardware_buffer( HSAMPLE S, S32 flags );
  extern void AILCALL hardware_stop( HSAMPLE S );
  extern void AILCALL set_hardware_position( HSAMPLE S );
  extern void AILCALL hardware_spin_until_stopped( HSAMPLE S );
  extern void AILCALL set_hardware_volume( HSAMPLE S );
  extern void AILCALL set_master_hardware_volume( HDIGDRIVER dig );
  extern void AILCALL set_hardware_loop_region( HSAMPLE S );
  extern void AILCALL hardware_3d_init( HSAMPLE S );
  extern void AILCALL set_hardware_low_pass( HSAMPLE S );
#else
  #define check_hardware_buffer( S, flags )
  #define hardware_stop( S )
  #define set_hardware_position( S )
  #define hardware_spin_until_stopped( S )
  #define set_hardware_volume( S )
  #define set_master_hardware_volume( dig )
  #define set_hardware_loop_region( S )
  #define set_hardware_low_pass( S )
#endif

#ifdef IS_WIN32API

#ifdef NTAPI
extern HWND AILCALL AIL_API_HWND                  (void);

void AILEXPORT API_timer (UINT  IDEvent,
                          UINT  uReserved,
                          DWORD dwUser,
                          DWORD dwReserved1,
                          DWORD dwReserved2);

#endif

extern S32    MilesCount;
extern U32 lastapitimerms;

extern void AILCALL win32_sleep( S32 ms );

//#define DEBUGMUTEX
#ifdef DEBUGMUTEX

extern void AILCALL InMilesMutexD(char const * __FILE, int __LINE);
extern void AILCALL OutMilesMutexD(char const * __FILE, int __LINE);

#define InMilesMutex() InMilesMutexD(__FILE__,__LINE__)
#define InMilesMutex() OutMilesMutexD(__FILE__,__LINE__)

#else

extern void AILCALL InMilesMutex(void);
extern void AILCALL OutMilesMutex(void);

#endif

#else

  #ifdef IS_MAC      

    #define InMilesMutex  AIL_lock_mutex
    #define OutMilesMutex AIL_unlock_mutex

    #define TC_NUM 16
    
    typedef void thread_callback( UINTa * data );

    void lock_thread_callback( UINTa * data );
    void unlock_thread_callback( UINTa * data );
    S32 create_thread_callback( UINTa * data, U32 ms_count, thread_callback * callback );
    void free_thread_callback( UINTa * data );
    S32 valid_thread_callback( UINTa * data );
    
  #elif defined(IS_PS3)

    extern void AILCALL InMilesMutex(void);
    extern void AILCALL OutMilesMutex(void);

  #else

    #define InMilesMutex()
    #define OutMilesMutex()

  #endif
#endif

#if defined(IS_WIN32API) || defined(IS_MACHO)
int AILCALL os_create_dir( char const * name );
#endif

#ifdef IS_WIN32API

void AILCALL Set_thread_name( U32 thread_id, char const * thread_name );

extern void AILCALL AIL_LP_lock();

extern void AILCALL AIL_LP_unlock();

#else
  #ifdef IS_MAC
   #if defined(__CARBON__)
    ProcessSerialNumber AILCALL AIL_API_Process(void);
   #endif
  #endif
#endif

// for multi-processor machines

#ifdef IS_WIN32API

#ifdef __RADINDLL__

#ifdef IS_WIN64
long
__stdcall
_InterlockedIncrement(
      long volatile *lpAddend
    );


long
__stdcall
_InterlockedDecrement(
      long volatile *lpAddend
    );
  
  #define MSSLockedIncrement(var) _InterlockedIncrement( (long*)&var ); 
  #define MSSLockedDecrement(var) _InterlockedDecrement( (long*)&var ); 

  #define MSSLockedIncrementPtr(var) _InterlockedIncrement( (long*)&var ); 
  #define MSSLockedDecrementPtr(var) _InterlockedDecrement( (long*)&var ); 
#else
  #define MSSLockedIncrement(var) _asm { lock inc [var] }
  #define MSSLockedDecrement(var) _asm { lock dec [var] }

  static void _inline __MSSLockedIncrementAddr(void * addr)
  {
    _asm
    {
      mov eax,[addr]
      lock inc dword ptr [eax]
    }
  }

  static void _inline __MSSLockedDecrementAddr(void * addr)
  {
    _asm {
      mov eax,[addr]
      lock dec dword ptr [eax]
    }
  }

  #define MSSLockedIncrementPtr(var) __MSSLockedIncrementAddr(&(var))
  #define MSSLockedDecrementPtr(var) __MSSLockedDecrementAddr(&(var))
#endif

#else

#ifdef IS_XENON

long
__stdcall
_InterlockedIncrement(
      long volatile *lpAddend
    );


long
__stdcall
_InterlockedDecrement(
      long volatile *lpAddend
    );

  void __emit(unsigned int opcode);
  #define __lwsync()      __emit(0x7C2004AC)

  #define MSSLockedIncrement(var) { _InterlockedIncrement( (long*)&var ); __lwsync(); }
  #define MSSLockedDecrement(var) { __lwsync(); _InterlockedDecrement( (long*)&var ); }

  #define MSSLockedIncrementPtr(var) { _InterlockedIncrement( (long*)&var ); __lwsync(); }
  #define MSSLockedDecrementPtr(var) { __lwsync(); _InterlockedDecrement( (long*)&var ); }

#else

  #define MSSLockedIncrement(var) (++var)
  #define MSSLockedDecrement(var) (--var)

  #define MSSLockedIncrementPtr(var) (++var)
  #define MSSLockedDecrementPtr(var) (--var)

#endif

#endif

#elif defined( IS_PS3)

  static inline U32 AtomicAdd32(U32 *ea, U32 value)
  {
    uint32_t old, tmp;

    __asm__ volatile(
      "# AtomicAdd32(ea=%[ea],old=%[old],val=%[value],tmp=%[tmp])\n"
      ".loop%=:\n"
      "  lwarx   %[old], 0, %[ea]\n"
      "  add     %[tmp], %[value], %[old]\n"
      "  stwcx.  %[tmp], 0, %[ea]\n"
      "  bne-    .loop%=\n"
      "  lwsync\n"
      : [old]"=&r"(old), [tmp]"=&r"(tmp)
      : [ea]"b"(ea), [value]"r"(value)
      : "cc", "memory");
    return old;
  }

  #define MSSLockedIncrement(var) AtomicAdd32( (U32*) &(var), 1 )
  #define MSSLockedDecrement(var) AtomicAdd32( (U32*) &(var), -1 )

  #define MSSLockedIncrementPtr(var) AtomicAdd32( (U32*) &(var), 1 )
  #define MSSLockedDecrementPtr(var) AtomicAdd32( (U32*) &(var), -1 )

#elif defined(IS_WII)

      #include <revolution/os.h>

      #define MSSLockedIncrement(var) {BOOL i=OSDisableInterrupts(); ++(*((U32*)(&var)));  OSRestoreInterrupts(i);}
      #define MSSLockedDecrement(var) {BOOL i=OSDisableInterrupts(); --(*((U32*)(&var)));  OSRestoreInterrupts(i);}

      #define MSSLockedIncrementPtr(ptr) {BOOL i=OSDisableInterrupts(); ++(*((U32*)(&ptr))); OSRestoreInterrupts(i); }
      #define MSSLockedDecrementPtr(ptr) {BOOL i=OSDisableInterrupts(); --(*((U32*)(&ptr))); OSRestoreInterrupts(i); }

#elif defined(IS_MAC)

  #ifdef IS_X86
  

  static inline void __MSSLockedIncrementAddr(void * addr)
  {
    __asm
    {
      mov eax,[addr]
      lock inc dword ptr [eax]
    }
  }

  static inline void __MSSLockedDecrementAddr(void * addr)
  {
    __asm {
      mov eax,[addr]
      lock dec dword ptr [eax]
    }
  }

  #define MSSLockedIncrement(var) __MSSLockedIncrementAddr((void*)&(var))
  #define MSSLockedDecrement(var) __MSSLockedDecrementAddr((void*)&(var))
  #define MSSLockedIncrementPtr(var) __MSSLockedIncrementAddr((void*)&(var))
  #define MSSLockedDecrementPtr(var) __MSSLockedDecrementAddr((void*)&(var))

  #else

    #include <string.h>

    #ifndef __OPENTRANSPORT__
      extern int OTAtomicAdd32( int toAdd, long *  dest);
    #endif

    #define MSSLockedIncrement(var) OTAtomicAdd32(  1, (long*) &(var) )
    #define MSSLockedDecrement(var) OTAtomicAdd32( -1, (long*) &(var) )

    #define MSSLockedIncrementPtr(var) OTAtomicAdd32(  1, (long*) &(var) )
    #define MSSLockedDecrementPtr(var) OTAtomicAdd32( -1, (long*) &(var) )

  #endif
  
#else
  #define MSSLockedIncrement(var) (++var)
  #define MSSLockedDecrement(var) (--var)

  #define MSSLockedIncrementPtr(var) (++var)
  #define MSSLockedDecrementPtr(var) (--var)
#endif

extern char AIL_redist_directory[260];

extern S32 AILCALL match_suffix(char const FAR* suffix,char const FAR* file_suffix);

#if !defined(IS_X86) || !defined(IS_MAC) // use mix rib on x86 mac
DXDEC S32 AILCALL MIX_RIB_MAIN( HPROVIDER provider_handle, U32 up_down );
#endif

#ifdef IS_STATIC
  DXDEC S32 AILCALL RIB_MAIN_NAME(MP3DEC)( HPROVIDER provider_handle, U32 up_down );
#endif

//
// High-level support services
//

extern void AILCALL AIL_API_startup               (void);
extern void AILCALL AIL_API_shutdown              (void);

extern SINTa AILCALL AIL_API_set_preference        (U32         number,
                                                    SINTa       value);

//
// Low-level support services
//

extern void AILCALL AIL_API_delay                 (S32         intervals);

extern S32  AILCALL AIL_API_background            (void);

extern void AILCALL AIL_API_mem_free_lock         (void   FAR *ptr);

extern void FAR* AILCALL AIL_API_mem_alloc_lock   (UINTa  size);

//
// Process services
//

extern HTIMER AILCALL AIL_API_register_timer      (AILTIMERCB  fn);

extern UINTa  AILCALL AIL_API_set_timer_user      (HTIMER      timer,
                                                   UINTa       user);

extern void   AILCALL AIL_API_set_timer_period    (HTIMER      timer,
                                                   U32         microseconds);

extern void   AILCALL AIL_API_set_timer_frequency (HTIMER      timer,
                                                   U32         hertz) ;

extern void   AILCALL AIL_API_set_timer_divisor   (HTIMER      timer,
                                                   U32         PIT_divisor);

extern void   AILCALL AIL_API_start_timer         (HTIMER      timer) ;
extern void   AILCALL AIL_API_start_all_timers    (void);

extern void   AILCALL AIL_API_stop_timer          (HTIMER      timer);
extern void   AILCALL AIL_API_stop_all_timers     (void);

extern void   AILCALL AIL_API_release_timer_handle(HTIMER      timer);
extern void   AILCALL AIL_API_release_all_timers  (void);

#ifdef IS_STATIC
extern S32 AILCALL MP3DEC_RIB_Main( HPROVIDER provider_handle, U32 up_down );
#endif


extern AIL_file_open_callback MSS_open;
extern AIL_file_close_callback MSS_close;
extern AIL_file_seek_callback MSS_seek;
extern AIL_file_read_callback MSS_read;

#if defined(IS_WIN32API) || defined(IS_MAC) || defined(IS_LINUX) || defined(IS_PS2) || defined(IS_PS3) || defined(IS_WII)
extern AIL_file_async_read_callback MSS_async_read;
extern AIL_file_async_status_callback MSS_async_status;
#endif

extern S32 disk_err;


extern void stream_background(void); // background service for streaming

#if (defined(IS_MAC) && defined( IS_X86 )) || defined( FORNONWIN )

DXDEC S32 AIL_fetch_CB( VOIDFUNC * fetch, UINTa usr, void * dest, S32 bytes, S32 offset );

#define call_fetch_CB( ASI,usr,dest,bytes,offset ) AIL_fetch_CB( ASI->fetch_CB,usr,dest,bytes,offset )

#else

#define call_fetch_CB( ASI,usr,dest,bytes,offset ) ((AILASIFETCHCB)((ASI)->fetch_CB))(usr,dest,bytes,offset)

#endif

extern HPROVIDER find_ASI_provider(const C8 *attrib, const C8 *filename);

#if defined(IS_WIN32API) || defined(IS_MAC) || defined(IS_LINUX) || defined(IS_DOS) || defined(IS_PS2) || defined(IS_PS3) || defined(IS_WII)

//
// M3D services
//

extern HDIGDRIVER AILCALL AIL_API_primary_digital_driver  (HDIGDRIVER new_primary);

extern void       AILCALL AIL_API_set_sample_3D_distances (HSAMPLE   samp,
                                                           F32       max_dist,
                                                           F32       min_dist,
                                                           S32       auto_3D_wet_atten);

extern void       AILCALL AIL_API_sample_3D_distances     (HSAMPLE   samp,
                                                           F32 FAR * max_dist,
                                                           F32 FAR * min_dist,
                                                           S32 FAR * auto_3D_wet_atten);

extern void       AILCALL AIL_API_set_sample_obstruction (HSAMPLE S,
                                                             F32     obstruction);

extern void       AILCALL AIL_API_set_sample_exclusion   (HSAMPLE S,
                                                             F32     exclusion);

extern void       AILCALL AIL_API_set_sample_occlusion   (HSAMPLE S,
                                                             F32     occlusion);

extern void       AILCALL AIL_API_set_sample_3D_cone        (HSAMPLE S,
                                                             F32     inner_angle,
                                                             F32     outer_angle,
                                                             F32     outer_volume);

extern F32        AILCALL AIL_API_sample_obstruction (HSAMPLE S);

extern F32        AILCALL AIL_API_sample_occlusion   (HSAMPLE S);

extern F32        AILCALL AIL_API_sample_exclusion   (HSAMPLE S);

extern void       AILCALL AIL_API_sample_3D_cone        (HSAMPLE  S,
                                                         F32 FAR *inner_angle,
                                                         F32 FAR *outer_angle,
                                                         F32 FAR *outer_volume);

extern  S32      AILCALL AIL_API_room_type                (HDIGDRIVER dig);

extern  void     AILCALL AIL_API_set_room_type            (HDIGDRIVER dig,
                                                           S32        EAX_room_type);

extern  F32      AILCALL AIL_API_3D_rolloff_factor        (HDIGDRIVER dig);

extern  void     AILCALL AIL_API_set_3D_rolloff_factor    (HDIGDRIVER dig,
                                                           F32       factor );

extern  F32      AILCALL AIL_API_3D_doppler_factor        (HDIGDRIVER dig);

extern  void     AILCALL AIL_API_set_3D_doppler_factor    (HDIGDRIVER dig,
                                                           F32        factor );

extern  F32      AILCALL AIL_API_3D_distance_factor       (HDIGDRIVER dig);

extern  void     AILCALL AIL_API_set_3D_distance_factor   (HDIGDRIVER dig,
                                                           F32        factor );

extern void       AILCALL AIL_API_set_sample_3D_position         (HSAMPLE S,
                                                                  F32     X,
                                                                  F32     Y,
                                                                  F32     Z);

extern void       AILCALL AIL_API_set_sample_3D_velocity         (HSAMPLE S,
                                                                  F32     dX_per_ms,
                                                                  F32     dY_per_ms,
                                                                  F32     dZ_per_ms,
                                                                  F32     magnitude);

extern void       AILCALL AIL_API_set_sample_3D_velocity_vector  (HSAMPLE S,
                                                                  F32     dX_per_ms,
                                                                  F32     dY_per_ms,
                                                                  F32     dZ_per_ms);

extern void       AILCALL AIL_API_set_sample_3D_orientation      (HSAMPLE S,
                                                                  F32     X_face,
                                                                  F32     Y_face,
                                                                  F32     Z_face,
                                                                  F32     X_up,
                                                                  F32     Y_up,
                                                                  F32     Z_up);
                                                                  
extern S32        AILCALL AIL_API_sample_3D_position             (HSAMPLE  S,
                                                                  F32 FAR *X,
                                                                  F32 FAR *Y,
                                                                  F32 FAR *Z);

extern void       AILCALL AIL_API_sample_3D_velocity             (HSAMPLE  S,
                                                                  F32 FAR *dX_per_ms,
                                                                  F32 FAR *dY_per_ms,
                                                                  F32 FAR *dZ_per_ms);

extern void       AILCALL AIL_API_sample_3D_orientation          (HSAMPLE  S,
                                                                  F32 FAR *X_face,
                                                                  F32 FAR *Y_face,
                                                                  F32 FAR *Z_face,
                                                                  F32 FAR *X_up,
                                                                  F32 FAR *Y_up,
                                                                  F32 FAR *Z_up);

extern void       AILCALL AIL_API_update_sample_3D_position      (HSAMPLE S,
                                                                  F32     dt_milliseconds);

extern void       AILCALL AIL_API_set_listener_3D_position(HDIGDRIVER dig,
                                                           F32     X,
                                                           F32     Y,
                                                           F32     Z);

extern void       AILCALL AIL_API_set_listener_3D_velocity(HDIGDRIVER dig,
                                                           F32     dX_per_ms,
                                                           F32     dY_per_ms,
                                                           F32     dZ_per_ms,
                                                           F32     magnitude);

extern void       AILCALL AIL_API_set_listener_3D_velocity_vector  (HDIGDRIVER dig,
                                                                    F32     dX_per_ms,
                                                                    F32     dY_per_ms,
                                                                    F32     dZ_per_ms);

extern void       AILCALL AIL_API_set_listener_3D_orientation      (HDIGDRIVER dig,
                                                                    F32     X_face,
                                                                    F32     Y_face,
                                                                    F32     Z_face,
                                                                    F32     X_up,
                                                                    F32     Y_up,
                                                                    F32     Z_up);

extern void       AILCALL AIL_API_listener_3D_position             (HDIGDRIVER  dig,
                                                                    F32 FAR *X,
                                                                    F32 FAR *Y,
                                                                    F32 FAR *Z);

extern void       AILCALL AIL_API_listener_3D_velocity             (HDIGDRIVER  dig,
                                                                    F32 FAR *dX_per_ms,
                                                                    F32 FAR *dY_per_ms,
                                                                    F32 FAR *dZ_per_ms);

extern void       AILCALL AIL_API_listener_3D_orientation          (HDIGDRIVER  dig,
                                                                    F32 FAR *X_face,
                                                                    F32 FAR *Y_face,
                                                                    F32 FAR *Z_face,
                                                                    F32 FAR *X_up,
                                                                    F32 FAR *Y_up,
                                                                    F32 FAR *Z_up);

extern void       AILCALL AIL_API_update_listener_3D_position      (HDIGDRIVER dig,
                                                                    F32     dt_milliseconds);

extern S32        AILCALL AIL_API_calculate_3D_channel_levels      (HDIGDRIVER       driver, //)
                                                                    F32         FAR *channel_levels,
                                                                    MSSVECTOR3D FAR *src_pos,
                                                                    MSSVECTOR3D FAR *src_face,
                                                                    MSSVECTOR3D FAR *src_up,
                                                                    F32              src_inner_angle,
                                                                    F32              src_outer_angle,
                                                                    F32              src_outer_volume,
                                                                    F32              src_max_dist,
                                                                    F32              src_min_dist,
                                                                    MSSVECTOR3D FAR *listen_pos, 
                                                                    MSSVECTOR3D FAR *listen_face,
                                                                    MSSVECTOR3D FAR *listen_up,  
                                                                    F32              rolloff_factor,
                                                                    MSSVECTOR3D FAR *doppler_velocity,
                                                                    F32         FAR *doppler_shift);

#endif

#if defined(IS_WINDOWS) || defined(IS_MAC) || defined(IS_LINUX) || defined(IS_XBOX) || defined(IS_XENON) || defined(IS_PS2) || defined(IS_PS3) || defined(IS_WII)

extern char MSS_Directory[260];

#if defined(IS_MAC) || defined(IS_LINUX) || defined(IS_XBOX) || defined(IS_XENON) || defined(IS_PS2) || defined(IS_PS3) || defined(IS_WII)

extern HDIGDRIVER AILCALL AIL_API_open_digital_driver( U32 frequency,
                                                       S32 bits,
                                                       S32 channel,
                                                       U32 flags );

extern void AILCALL AIL_API_close_digital_driver( HDIGDRIVER dig );

#else

extern HINSTANCE MSS_Instance;

extern S32 AILCALL AIL_API_waveOutOpen           (HDIGDRIVER   FAR *drvr,
                                                  LPHWAVEOUT   FAR *lphWaveOut,
                                                  LPWAVEFORMAT      lpFormat);

extern void AILCALL AIL_API_waveOutClose         (HDIGDRIVER drvr);

#endif

#if defined(IS_WINDOWS) || defined(IS_MAC) || defined(IS_LINUX)

extern S32 AILCALL AIL_API_digital_handle_release(HDIGDRIVER drvr);

extern S32 AILCALL AIL_API_digital_handle_reacquire
                                                 (HDIGDRIVER drvr);

extern HDIGINPUT AILCALL AIL_API_open_input             (AIL_INPUT_INFO FAR *info);

extern void      AILCALL AIL_API_close_input            (HDIGINPUT         dig);

extern AIL_INPUT_INFO FAR *
                AILCALL AIL_API_get_input_info              (HDIGINPUT         dig);

extern S32       AILCALL AIL_API_set_input_state        (HDIGINPUT         dig,
                                                         S32               enable);

#endif
#endif

#ifdef MSS_FLT_SUPPORTED

void FAR * SS_execute_filter_chain(HSAMPLE  S, //)
                                   void FAR * src,
                                   void FAR * temp_dest,
                                   S32      src_bytes,
                                   S32      is_stereo,
                                   S16 FAR *dest_mono_sample_buffer);

//
// Internal FLT services
//

void FLT_init_list(void);

FLTPROVIDER FAR *FLT_find_provider_instance(HPROVIDER  provider,
                                            HDIGDRIVER dig);

void FLT_disconnect_driver(HDIGDRIVER dig);

void FLT_call_premix_processors(HDIGDRIVER dig);

S32 FLT_call_postmix_processors(HDIGDRIVER dig, void FAR *output_buffer);

//
// FLT services
//

extern S32        AILCALL AIL_API_enumerate_filters  (HPROENUM  FAR *next,
                                                      HPROVIDER FAR *dest,
                                                      C8  FAR * FAR *name);

extern HDRIVERSTATE
                 AILCALL AIL_API_open_filter        (HPROVIDER  lib,
                                                     HDIGDRIVER dig);

extern void       AILCALL AIL_API_close_filter       (HDRIVERSTATE filter);






extern S32        AILCALL AIL_API_enumerate_filter_attributes
                                                (HPROVIDER                  lib,
                                                 HINTENUM FAR *             next,
                                                 RIB_INTERFACE_ENTRY FAR *  dest);

extern void       AILCALL AIL_API_filter_attribute   (HPROVIDER  lib,
                                                      C8 const FAR *   name,
                                                      void FAR * val);

extern void       AILCALL AIL_API_set_filter_preference
                                                (HPROVIDER  lib,
                                                 C8 const FAR *   name,
                                                 void const FAR * val);





extern  S32      AILCALL AIL_API_enumerate_filter_sample_attributes
                                                (HPROVIDER                 lib,
                                                 HINTENUM FAR *            next,
                                                 RIB_INTERFACE_ENTRY FAR * dest);

extern  void     AILCALL AIL_API_filter_sample_attribute
                                                (HSAMPLE      S,
                                                 C8 const FAR *     name,
                                                 void FAR *   val);

extern  void     AILCALL AIL_API_set_filter_sample_preference
                                                (HSAMPLE      S,
                                                 C8 const FAR *     name,
                                                 void const FAR *   val);

#ifdef IS_WIN32API

extern void AILCALL AIL_API_get_DirectSound_info
                                                 (HSAMPLE     S,
                                                  AILLPDIRECTSOUND        *lplpDS,
                                                  AILLPDIRECTSOUNDBUFFER  *lplpDSB);
#endif

#ifdef IS_WINDOWS

extern S32  AILCALL AIL_API_set_DirectSound_HWND
                                                 (HDIGDRIVER     dig,
                                                  HWND           wnd);
#endif

#endif

#ifdef IS_DOS

//
// DOS digital services
//

extern HDIGDRIVER  AILCALL AIL_API_install_DIG_driver_file
                                                 (char const *filename,
                                                  IO_PARMS *IO);

extern S32 AILCALL AIL_API_install_DIG_INI       (HDIGDRIVER *dig);

extern void AILCALL AIL_API_uninstall_DIG_driver (HDIGDRIVER dig);


extern HDIGDRIVER  AILCALL AIL_API_install_DIG_driver_image
                                                 (void const *driver_image,
                                                  U32 size,
                                                  IO_PARMS *IO);
#endif

extern S32  AILCALL AIL_API_enumerate_digital_driver_attributes
                                               (HDIGDRIVER                 dig,
                                                HINTENUM FAR *             next,
                                                RIB_INTERFACE_ENTRY FAR *  dest);

extern void AILCALL AIL_API_digital_driver_attribute   (HDIGDRIVER     dig,
                                                   C8 const FAR * name,
                                                   void FAR *     val);

extern S32  AILCALL AIL_API_enumerate_digital_driver_preferences
                                               (HDIGDRIVER                 dig,
                                                HINTENUM FAR *             next,
                                                RIB_INTERFACE_ENTRY FAR *  dest);

extern void AILCALL AIL_API_set_digital_driver_preference(HDIGDRIVER        dig,
                                                     C8 const FAR *    name,
                                                     void const FAR *  val);

extern S32  AILCALL AIL_API_enumerate_sample_attributes
                                                (HSAMPLE                    S,
                                                 HINTENUM FAR *             next,
                                                 RIB_INTERFACE_ENTRY FAR *  dest);

extern void AILCALL AIL_API_sample_attribute         (HSAMPLE          S,
                                                 C8 const FAR *   name,
                                                 void FAR *       val);

extern S32  AILCALL AIL_API_enumerate_sample_preferences
                                               (HSAMPLE                    S,
                                                HINTENUM FAR *             next,
                                                RIB_INTERFACE_ENTRY FAR *  dest);

extern void AILCALL AIL_API_set_sample_preference (HSAMPLE          S,
                                              C8 const FAR *   name,
                                              void const FAR * val);

extern S32 AILCALL AIL_API_digital_CPU_percent(HDIGDRIVER dig);

extern S32 AILCALL AIL_API_background_CPU_percent(void);

extern S32 AILCALL AIL_API_digital_latency(HDIGDRIVER dig);

extern HSAMPLE AILCALL AIL_API_allocate_sample_handle
                                                  (HDIGDRIVER dig);

extern HSAMPLE AILCALL AIL_API_allocate_file_sample
                                                  (HDIGDRIVER dig,
                                                   void const FAR *file_image,
                                                   S32         block);

extern void  AILCALL AIL_API_set_speaker_configuration
                                                      (HDIGDRIVER       dig,
                                                       MSSVECTOR3D FAR *array,
                                                       S32              n_channels,
                                                       F32              falloff_power);

extern MSSVECTOR3D FAR * 
                    AILCALL AIL_API_speaker_configuration
                                                      (HDIGDRIVER       dig,
                                                       S32         FAR *n_physical_channels,
                                                       S32         FAR *n_logical_channels,
                                                       F32         FAR *falloff_power,
                                                       MSS_MC_SPEC FAR *channel_spec);

extern void  AILCALL AIL_API_set_listener_relative_receiver_array 
                                                      (HDIGDRIVER             dig,
                                                       MSS_RECEIVER_LIST FAR *array,
                                                       S32                    n_receivers);

extern MSS_RECEIVER_LIST FAR * 
                    AILCALL AIL_API_listener_relative_receiver_array 
                                                      (HDIGDRIVER dig,
                                                       S32   FAR *n_receivers);

#ifndef IS_XBOX

extern void         AILCALL AIL_API_set_speaker_reverb_levels
                                                      (HDIGDRIVER dig, 
                                                       F32 FAR   *wet_array,
                                                       F32 FAR   *dry_array,
                                                       S32        n_levels);

extern void         AILCALL AIL_API_speaker_reverb_levels  (HDIGDRIVER     dig,
                                                       S32 FAR       *n_levels,
                                                       F32 FAR * FAR *wet_array,
                                                       F32 FAR * FAR *dry_array);
#endif

extern void AILCALL AIL_API_release_sample_handle (HSAMPLE S);

extern S32 AILCALL AIL_API_set_sample_file        (HSAMPLE S,
                                                   void const FAR *file_image,
                                                   S32     block);

extern S32 AILCALL AIL_API_set_sample_info        (HSAMPLE S,
                                                   AILSOUNDINFO const FAR * info);

extern S32          AILCALL AIL_API_set_named_sample_file (HSAMPLE   S,
                                                           C8 const   FAR *file_type_suffix,
                                                           void const FAR *file_image,
                                                           U32       file_size,
                                                           S32       block);

extern HPROVIDER AILCALL AIL_API_set_sample_processor     (HSAMPLE     S,
                                                           SAMPLESTAGE pipeline_stage,
                                                           HPROVIDER   provider);

extern HPROVIDER AILCALL AIL_API_set_digital_driver_processor
                                                          (HDIGDRIVER  dig,
                                                           DIGDRVSTAGE pipeline_stage,
                                                           HPROVIDER   provider);

extern HPROVIDER AILCALL AIL_API_sample_processor     (HSAMPLE     S,
                                                       SAMPLESTAGE pipeline_stage);

extern HPROVIDER AILCALL AIL_API_digital_driver_processor
                                                      (HDIGDRIVER  dig,
                                                       DIGDRVSTAGE pipeline_stage);

extern void AILCALL AIL_API_set_sample_address    (HSAMPLE S,
                                                   void const FAR *start,
                                                   U32     len);

extern void AILCALL AIL_API_init_sample           (HSAMPLE S,
                                                   S32     format,
                                                   U32     flags);

extern void AILCALL AIL_API_set_sample_adpcm_block_size
                                                  (HSAMPLE S,
                                                   U32     blocksize);

extern void AILCALL AIL_API_start_sample          (HSAMPLE S);

extern void AILCALL AIL_API_stop_sample           (HSAMPLE S);

extern void AILCALL AIL_API_resume_sample         (HSAMPLE S);

extern void AILCALL AIL_API_end_sample            (HSAMPLE S);

extern void AILCALL AIL_API_set_sample_playback_rate
                                                  (HSAMPLE S,
                                                   S32     playback_rate);

extern void AILCALL AIL_API_set_sample_volume_pan (HSAMPLE S,
                                                   F32     volume,
                                                   F32     pan);

extern void AILCALL AIL_API_set_sample_channel_levels (HSAMPLE  S, 
                                                       F32 FAR *level_array,
                                                       S32      n_levels);

extern F32 FAR * AILCALL AIL_API_sample_channel_levels (HSAMPLE  S, 
                                                        S32 FAR *n_levels);

extern void     AILCALL AIL_API_set_sample_51_volume_pan( HSAMPLE S,
                                                          F32     volume,
                                                          F32     pan,
                                                          F32     fb_pan,
                                                          F32     center_level,
                                                          F32     sub_level );

extern void     AILCALL AIL_API_sample_51_volume_pan    ( HSAMPLE S,
                                                          F32 FAR* volume,
                                                          F32 FAR* pan,
                                                          F32 FAR* fb_pan,
                                                          F32 FAR* center_level,
                                                          F32 FAR* sub_level );

extern  void    AILCALL AIL_API_set_sample_51_volume_levels( HSAMPLE S,
                                                             F32     f_left_level,
                                                             F32     f_right_level,
                                                             F32     b_left_level,
                                                             F32     b_right_level,
                                                             F32     center_level,
                                                             F32     sub_level );

extern  void    AILCALL AIL_API_sample_51_volume_levels    ( HSAMPLE S,
                                                             F32 FAR* f_left_level,
                                                             F32 FAR* f_right_level,
                                                             F32 FAR* b_left_level,
                                                             F32 FAR* b_right_level,
                                                             F32 FAR* center_level,
                                                             F32 FAR* sub_level );
extern void AILCALL AIL_API_set_sample_volume_levels(HSAMPLE S,
                                                     F32     left_level,
                                                     F32     right_level);

extern void AILCALL AIL_API_set_sample_reverb_levels(HSAMPLE S,
                                                     F32     dry_level,
                                                     F32     wet_level);

extern void AILCALL AIL_API_set_sample_low_pass_cut_off(HSAMPLE S,
                                                        F32     cut_off);

extern void AILCALL AIL_API_set_sample_loop_count (HSAMPLE S,
                                                   S32     loop_count);

extern void AILCALL AIL_API_set_sample_loop_block (HSAMPLE S,
                                                   S32     loop_start_offset,
                                                   S32     loop_end_offset);

extern S32  AILCALL AIL_API_sample_loop_block     (HSAMPLE S,
                                                   S32    *loop_start_offset,
                                                   S32    *loop_end_offset);

extern U32 AILCALL AIL_API_sample_status          (HSAMPLE S);

extern S32 AILCALL AIL_API_sample_playback_rate   (HSAMPLE S);

extern void AILCALL AIL_API_sample_volume_pan     (HSAMPLE S,
                                                   F32 FAR* volume,
                                                   F32 FAR* pan);

extern void AILCALL AIL_API_sample_volume_levels(HSAMPLE S,
                                                 F32 FAR * left_level,
                                                 F32 FAR * right_level);

extern void AILCALL AIL_API_sample_reverb_levels(HSAMPLE S,
                                                 F32 FAR * dry_level,
                                                 F32 FAR * wet_level);

extern F32 AILCALL AIL_API_sample_low_pass_cut_off(HSAMPLE S);

extern S32 AILCALL AIL_API_sample_loop_count      (HSAMPLE S);

extern void AILCALL AIL_API_set_digital_master_volume_level
                                                  (HDIGDRIVER dig,
                                                   F32         master_volume);

extern F32 AILCALL AIL_API_digital_master_volume_level  (HDIGDRIVER dig);

extern void AILCALL AIL_API_set_digital_master_volume_levels(HDIGDRIVER dig,
                                                     F32     left_level,
                                                     F32     right_level);

extern void AILCALL AIL_API_set_digital_master_reverb_levels(HDIGDRIVER dig,
                                                     F32     dry_level,
                                                     F32     wet_level);

extern void AILCALL AIL_API_set_digital_master_room_type(HDIGDRIVER dig,
                                                         S32        room_type);

extern void AILCALL AIL_API_digital_master_reverb_levels(HDIGDRIVER dig,
                                                         F32 FAR * dry_level,
                                                         F32 FAR * wet_level);

extern void AILCALL AIL_API_set_digital_master_reverb(HDIGDRIVER dig,
                                                      F32     reverb_time,
                                                      F32     reverb_predelay,
                                                      F32     reverb_damping);

extern void AILCALL AIL_API_digital_master_reverb(HDIGDRIVER dig,
                                                  F32 FAR *  reverb_time,
                                                  F32 FAR *  reverb_predelay,
                                                  F32 FAR *  reverb_damping);

extern S32 AILCALL AIL_API_minimum_sample_buffer_size
                                                  (HDIGDRIVER dig,
                                                   S32         playback_rate,
                                                   S32         format);

extern S32 AILCALL AIL_API_sample_buffer_ready    (HSAMPLE S);

extern void AILCALL AIL_API_load_sample_buffer    (HSAMPLE S,
                                                   S32    buff_num,
                                                   void const FAR *buffer,
                                                   U32    len);

extern S32 AILCALL AIL_API_sample_buffer_info     (HSAMPLE S,
                                                   U32    FAR *pos0,
                                                   U32    FAR *len0,
                                                   U32    FAR *pos1,
                                                   U32    FAR *len1);

extern U32 AILCALL AIL_API_sample_granularity     (HSAMPLE S);

extern void AILCALL AIL_API_set_sample_position   (HSAMPLE S,
                                                   U32     pos);

extern U32 AILCALL AIL_API_sample_position        (HSAMPLE S);

extern AILSAMPLECB AILCALL AIL_API_register_SOB_callback
                                                  (HSAMPLE S,
                                                   AILSAMPLECB SOB);

extern AILSAMPLECB AILCALL AIL_API_register_EOB_callback
                                                  (HSAMPLE S,
                                                   AILSAMPLECB EOB);

extern AILSAMPLECB AILCALL AIL_API_register_EOS_callback
                                                  (HSAMPLE S,
                                                   AILSAMPLECB EOS);

extern AILFALLOFFCB AILCALL AIL_API_register_falloff_function_callback
                                                  (HSAMPLE S,
                                                   AILFALLOFFCB falloff_cb);

extern void AILCALL AIL_API_set_sample_user_data  (HSAMPLE S,
                                                   U32     index,
                                                   SINTa   value);

extern SINTa AILCALL AIL_API_sample_user_data       (HSAMPLE S,
                                                     U32     index);

extern S32 AILCALL AIL_API_active_sample_count    (HDIGDRIVER dig);

extern void AILCALL AIL_API_digital_configuration (HDIGDRIVER dig,
                                                   S32   FAR *rate,
                                                   S32   FAR *format,
                                                   char  FAR *config);

//
// High-level XMIDI services
//

#if defined(IS_WINDOWS) || defined(IS_MAC) || defined(IS_LINUX) || defined(IS_XBOX) || defined(IS_XENON) || defined(IS_PS2) || defined(IS_PS3) || defined(IS_WII)

#if defined(IS_MAC) || defined(IS_LINUX) || defined(IS_XBOX) || defined(IS_XENON) || defined(IS_PS2) || defined(IS_PS3) || defined(IS_WII)

extern HMDIDRIVER AILCALL AIL_API_open_XMIDI_driver( U32 flags );

extern void AILCALL AIL_API_close_XMIDI_driver( HMDIDRIVER mdi );

#else

extern S32 AILCALL AIL_API_midiOutOpen           (HMDIDRIVER FAR *drvr,
                                                   LPHMIDIOUT FAR *lphMidiOut,
                                                   U32            dwDeviceID);

extern void AILCALL AIL_API_midiOutClose          (HMDIDRIVER mdi);

#endif

extern S32 AILCALL AIL_API_MIDI_handle_release    (HMDIDRIVER mdi);

extern S32 AILCALL AIL_API_MIDI_handle_reacquire  (HMDIDRIVER mdi);

#else

//
// DOS XMIDI services
//

extern HMDIDRIVER  AILCALL AIL_API_install_MDI_driver_file
                                                    (char const *filename,
                                                     IO_PARMS *IO);

extern S32 AILCALL AIL_API_install_MDI_INI          (HMDIDRIVER *mdi);

extern void AILCALL AIL_API_uninstall_MDI_driver    (HMDIDRIVER mdi);

extern HMDIDRIVER  AILCALL AIL_API_install_MDI_driver_image
                                                    (void const *driver_image,
                                                     U32 size,
                                                     IO_PARMS *IO);

extern S32 AILCALL AIL_API_MDI_driver_type          (HMDIDRIVER mdi);

extern void AILCALL AIL_API_set_GTL_filename_prefix (char const *prefix);

extern S32 AILCALL AIL_API_timbre_status            (HMDIDRIVER mdi,
                                                     S32 bank,
                                                     S32 patch);

extern S32 AILCALL AIL_API_install_timbre           (HMDIDRIVER mdi,
                                                     S32 bank,
                                                     S32 patch);

extern void AILCALL AIL_API_protect_timbre          (HMDIDRIVER mdi,
                                                     S32 bank,
                                                     S32 patch);

extern void AILCALL AIL_API_unprotect_timbre        (HMDIDRIVER mdi,
                                                     S32 bank,
                                                     S32 patch);

#endif

extern HSEQUENCE AILCALL AIL_API_allocate_sequence_handle
                                                  (HMDIDRIVER mdi);

extern void AILCALL AIL_API_release_sequence_handle
                                                  (HSEQUENCE S);

extern S32 AILCALL AIL_API_init_sequence          (HSEQUENCE S,
                                                   void const FAR *start,
                                                   S32       sequence_num);

extern void AILCALL AIL_API_start_sequence        (HSEQUENCE S);
extern void AILCALL AIL_API_stop_sequence         (HSEQUENCE S);
extern void AILCALL AIL_API_resume_sequence       (HSEQUENCE S);
extern void AILCALL AIL_API_end_sequence          (HSEQUENCE S);

extern void AILCALL AIL_API_set_sequence_tempo    (HSEQUENCE S,
                                                   S32       tempo,
                                                   S32       milliseconds);

extern void AILCALL AIL_API_set_sequence_volume   (HSEQUENCE S,
                                                   S32       volume,
                                                   S32       milliseconds);

extern void AILCALL AIL_API_set_sequence_loop_count
                                                  (HSEQUENCE S,
                                                   S32       loop_count);

extern U32 AILCALL AIL_API_sequence_status        (HSEQUENCE S);

extern S32 AILCALL AIL_API_sequence_tempo         (HSEQUENCE S);
extern S32 AILCALL AIL_API_sequence_volume        (HSEQUENCE S);
extern S32 AILCALL AIL_API_sequence_loop_count    (HSEQUENCE S);

extern void AILCALL AIL_API_set_XMIDI_master_volume
                                                  (HMDIDRIVER mdi,
                                                   S32        master_volume);

extern S32 AILCALL AIL_API_XMIDI_master_volume    (HMDIDRIVER mdi);

//
// Low-level XMIDI services
//

extern S32 AILCALL AIL_API_active_sequence_count  (HMDIDRIVER mdi);

extern S32 AILCALL AIL_API_controller_value       (HSEQUENCE S,
                                                   S32       channel,
                                                   S32       controller_num);

extern S32 AILCALL AIL_API_channel_notes          (HSEQUENCE S,
                                                   S32       channel);

extern void AILCALL AIL_API_sequence_position     (HSEQUENCE S,
                                                   S32       FAR *beat,
                                                   S32       FAR *measure);

extern void AILCALL AIL_API_branch_index          (HSEQUENCE S,
                                                   U32       marker);

extern AILPREFIXCB AILCALL AIL_API_register_prefix_callback
                                                  (HSEQUENCE S,
                                                   AILPREFIXCB callback);

extern AILTRIGGERCB AILCALL AIL_API_register_trigger_callback
                                                  (HSEQUENCE S,
                                                   AILTRIGGERCB callback);

extern AILSEQUENCECB AILCALL AIL_API_register_sequence_callback
                                                  (HSEQUENCE S,
                                                   AILSEQUENCECB callback);

extern AILEVENTCB AILCALL AIL_API_register_event_callback
                                                  (HMDIDRIVER mdi,
                                                   AILEVENTCB callback);

extern AILBEATCB AILCALL AIL_API_register_beat_callback
                                                  (HSEQUENCE  S,
                                                   AILBEATCB callback);

extern AILTIMBRECB AILCALL AIL_API_register_timbre_callback
                                                  (HMDIDRIVER mdi,
                                                   AILTIMBRECB callback);

extern void AILCALL AIL_API_set_sequence_user_data(HSEQUENCE S,
                                                   U32       index,
                                                   SINTa     value);

extern SINTa AILCALL AIL_API_sequence_user_data     (HSEQUENCE S,
                                                     U32       index);

extern void AILCALL AIL_API_register_ICA_array    (HSEQUENCE S,
                                                   U8        FAR *array);

extern S32 AILCALL AIL_API_lock_channel           (HMDIDRIVER mdi);

extern void AILCALL AIL_API_release_channel       (HMDIDRIVER mdi,
                                                   S32        channel);

extern void AILCALL AIL_API_map_sequence_channel  (HSEQUENCE S,
                                                   S32       seq_channel,
                                                   S32       new_channel);

extern S32 AILCALL AIL_API_true_sequence_channel  (HSEQUENCE S,
                                                   S32       seq_channel);

extern void AILCALL AIL_API_send_channel_voice_message
                                                  (HMDIDRIVER mdi,
                                                   HSEQUENCE  S,
                                                   S32        status,
                                                   S32        data_1,
                                                   S32        data_2);

extern void AILCALL AIL_API_send_sysex_message    (HMDIDRIVER mdi,
                                                   void const FAR *buffer);

extern HWAVESYNTH
       AILCALL AIL_API_create_wave_synthesizer    (HDIGDRIVER dig,
                                                   HMDIDRIVER mdi,
                                                   void const FAR *wave_lib,
                                                   S32        polyphony);

extern void AILCALL AIL_API_destroy_wave_synthesizer(HWAVESYNTH W);

extern S32 AILCALL AIL_API_MIDI_to_XMI            (void const FAR* MIDI,
                                                   U32       MIDI_size,
                                                   void FAR* FAR*XMIDI,
                                                   U32  FAR* XMIDI_size,
                                                   S32       flags);

#if defined(IS_WIN32)

extern S32 AILCALL AIL_API_list_MIDI              (void const FAR*  MIDI,
                                                   U32        MIDI_size,
                                                   char FAR*  FAR*list,
                                                   U32  FAR*  list_size,
                                                   S32        flags);

extern S32 AILCALL AIL_API_list_DLS               (void const FAR* DLS,
                                                   char FAR* FAR*list,
                                                   U32  FAR* list_size,
                                                   S32       flags,
                                                   C8   FAR* title);

#endif

extern char FAR* AILCALL AIL_API_last_error       ( void );

extern void AILCALL AIL_API_set_error             ( char const FAR* error_msg );

extern S32 AILCALL AIL_API_file_error             (void);

extern S32 AILCALL AIL_API_file_size              (char const FAR   *filename);

extern void FAR * AILCALL AIL_API_file_read       (char const FAR   *filename,
                                                   void FAR *dest);

extern S32 AILCALL AIL_API_file_write             (char const FAR   *filename,
                                                   void const FAR *buf,
                                                   U32       len);

extern S32 AILCALL AIL_API_WAV_file_write         (char const FAR   *filename,
                                                   void const FAR *buf,
                                                   U32       len,
                                                   S32       rate,
                                                   S32       format);

#ifdef ON_MAC_USE_FSS

extern int AILCALL AIL_get_FSSpec(char FAR *filename,FSSpec * spec);

extern int AILCALL AIL_create_and_get_FSSpec(char const FAR *filename,FSSpec * spec, U32 type, U32 creator );

extern S32 AILCALL AIL_API_file_fss_size          (FSSpec const FAR   *filename);

extern void FAR * AILCALL AIL_API_file_fss_read   (FSSpec const FAR   *filename,
                                                   void FAR *dest);

extern S32 AILCALL AIL_API_file_fss_write         (FSSpec const FAR   *filename,
                                                   void const FAR *buf,
                                                   U32       len);

extern S32 AILCALL AIL_API_file_fss_attrib_write  (FSSpec const FAR   *filename,
                                                   void const FAR *buf,
                                                   U32       len,
                                                   U32       type,
                                                   U32       creator);

extern S32 AILCALL AIL_API_WAV_file_fss_write     (FSSpec const FAR   *filename,
                                                   void const FAR *buf,
                                                   U32       len,
                                                   S32       rate,
                                                   S32       format);

#endif

extern void AILCALL AIL_API_serve                 (void);


#if !defined(IS_XBOX) && !defined(IS_XENON) && !defined(IS_PS2) && !defined(IS_PS3) && !defined(IS_WII)

extern HREDBOOK AILCALL AIL_API_redbook_open      (U32 which);

#ifdef IS_MAC
extern HREDBOOK AILCALL AIL_API_redbook_open_volume(char const* drive);
#else
extern HREDBOOK AILCALL AIL_API_redbook_open_drive(S32 drive);
#endif

extern void AILCALL AIL_API_redbook_close         (HREDBOOK hand);

extern void AILCALL AIL_API_redbook_eject         (HREDBOOK hand);

extern void AILCALL AIL_API_redbook_retract       (HREDBOOK hand);

extern U32 AILCALL AIL_API_redbook_status         (HREDBOOK hand);

extern U32 AILCALL AIL_API_redbook_tracks         (HREDBOOK hand);

extern U32 AILCALL AIL_API_redbook_track          (HREDBOOK hand);

extern void AILCALL AIL_API_redbook_track_info    (HREDBOOK hand,
                                                   U32 tracknum,
                                                   U32 FAR* startmsec,
                                                   U32 FAR* endmsec);

extern U32 AILCALL AIL_API_redbook_id             (HREDBOOK hand);

extern U32 AILCALL AIL_API_redbook_position       (HREDBOOK hand);

extern U32 AILCALL AIL_API_redbook_play           (HREDBOOK hand,
                                                   U32 startmsec,
                                                   U32 endmsec);

extern U32 AILCALL AIL_API_redbook_stop           (HREDBOOK hand);

extern U32 AILCALL AIL_API_redbook_pause          (HREDBOOK hand);

extern U32 AILCALL AIL_API_redbook_resume         (HREDBOOK hand);

extern F32 AILCALL AIL_API_redbook_volume_level   (HREDBOOK hand);

extern F32 AILCALL AIL_API_redbook_set_volume_level(HREDBOOK hand,
                                                    F32 volume);

#endif

extern S32 AILCALL AIL_API_quick_startup          (S32         use_digital,
                                                   S32         use_MIDI,
                                                   U32         output_rate,
                                                   S32         output_bits,
                                                   S32         output_channels);

extern void AILCALL AIL_API_quick_shutdown        (void);

extern void AILCALL AIL_API_quick_handles         (HDIGDRIVER FAR* pdig,
                                                   HMDIDRIVER FAR* pmdi,
                                                   HDLSDEVICE FAR* pdls);

extern HAUDIO AILCALL AIL_API_quick_load          (char const FAR *filename);

#ifdef ON_MAC_USE_FSS

extern HAUDIO AILCALL AIL_API_quick_fss_load      (FSSpec const FAR *filename);

#endif

extern HAUDIO AILCALL AIL_API_quick_load_mem      (void const FAR *mem,
                                                   U32    size);

extern HAUDIO AILCALL AIL_API_quick_load_named_mem(void const FAR *mem,
                                                   char const FAR *filename,
                                                   U32    size);

extern void AILCALL AIL_API_quick_unload          (HAUDIO      audio);

extern S32 AILCALL AIL_API_quick_play             (HAUDIO      audio,
                                                   U32         loop_count);

extern void AILCALL AIL_API_quick_halt            (HAUDIO      audio);

extern S32 AILCALL AIL_API_quick_status           (HAUDIO      audio);

#ifdef ON_MAC_USE_FSS

extern HAUDIO AILCALL AIL_API_quick_fss_load_and_play (FSSpec const FAR *filename,
                                                   U32         loop_count,
                                                   S32         wait_request);
#endif

extern HAUDIO AILCALL AIL_API_quick_load_and_play (char const FAR *filename,
                                                   U32         loop_count,
                                                   S32         wait_request);

extern void AILCALL AIL_API_quick_set_speed       (HAUDIO      audio,
                                                   S32         speed);

extern void AILCALL AIL_API_quick_set_volume      (HAUDIO      audio,
                                                   F32         volume,
                                                   F32         extravol);

extern void AILCALL AIL_API_quick_set_reverb_levels(HAUDIO  audio,
                                                    F32     dry_level,
                                                    F32     wet_level);

extern void AILCALL AIL_API_quick_set_low_pass_cut_off(HAUDIO  audio,
                                                       F32     cut_off);

extern HAUDIO AILCALL AIL_API_quick_copy          (HAUDIO hand);

extern void AILCALL AIL_API_quick_set_ms_position (HAUDIO audio,
                                                   S32 milliseconds);

extern S32 AILCALL AIL_API_quick_ms_position      (HAUDIO audio);

extern S32 AILCALL AIL_API_quick_ms_length        (HAUDIO audio);

extern S32 AILCALL AIL_API_quick_type             (HAUDIO audio);

//
// High-level streaming services
//

void AILSTRM_shutdown(HDIGDRIVER driver);

extern HSTREAM AILCALL AIL_API_open_stream        (HDIGDRIVER dig,
                                                   char const FAR* filename,
                                                   S32 stream_mem);

extern void AILCALL AIL_API_close_stream          (HSTREAM stream);

extern S32 AILCALL AIL_API_service_stream         (HSTREAM stream,
                                                   S32 fillup);

extern void AILCALL AIL_API_start_stream          (HSTREAM stream);

extern void AILCALL AIL_API_pause_stream          (HSTREAM stream,
                                                   S32 onoff);

extern void AILCALL AIL_API_set_stream_loop_block (HSTREAM S,
                                                   S32       loop_start_offset,
                                                   S32       loop_end_offset);

extern S32 AILCALL AIL_API_stream_loop_count      (HSTREAM stream);

extern void AILCALL AIL_API_set_stream_loop_count (HSTREAM stream,
                                                   S32 count);

extern S32 AILCALL AIL_API_stream_status          (HSTREAM stream);

extern void AILCALL AIL_API_set_stream_position   (HSTREAM stream,
                                                   S32 offset);

extern S32 AILCALL AIL_API_stream_position        (HSTREAM stream);

extern void AILCALL AIL_API_stream_info           (HSTREAM stream,
                                                   S32 FAR* datarate,
                                                   S32 FAR* sndtype,
                                                   S32 FAR* length,
                                                   S32 FAR* memory);

#if defined(IS_WINDOWS) || defined(IS_MAC) || defined(IS_LINUX) || defined(IS_XBOX) || defined(IS_XENON) || defined(IS_PS2) || defined(IS_PS3) || defined(IS_WII)

extern void AILCALL AIL_API_auto_service_stream   (HSTREAM stream,
                                                   S32 onoff);

#endif

extern AILSTREAMCB AILCALL AIL_API_register_stream_callback
                                                  (HSTREAM stream,
                                                   AILSTREAMCB callback);

extern void AILCALL AIL_API_set_stream_user_data  (HSTREAM S,
                                                   U32     index,
                                                   SINTa   value);

extern SINTa AILCALL AIL_API_stream_user_data       (HSTREAM S,
                                                     U32     index);

extern S32 AILCALL AIL_API_size_processed_digital_audio(
                                 U32             dest_rate,
                                 U32             dest_format,
                                 S32             num_srcs,
                                 AILMIXINFO const FAR* src);

extern S32 AILCALL AIL_API_process_digital_audio(
                                 void FAR       *dest_buffer,
                                 S32             dest_buffer_size,
                                 U32             dest_rate,
                                 U32             dest_format,
                                 S32             num_srcs,
                                 AILMIXINFO FAR* src);

extern HDLSDEVICE AILCALL AIL_API_DLS_open        (HMDIDRIVER mdi,
                                                   HDIGDRIVER dig,
#ifdef IS_STATIC
                                                   AILSTATICDLS const FAR* dls,
#else
                                                   char const FAR* libname,
#endif
                                                   U32 flags,
                                                   U32 rate,
                                                   S32 bits,
                                                   S32 channels);


extern void AILCALL AIL_API_DLS_close             (HDLSDEVICE dls,
                                                   U32 flags);

extern HDLSFILEID AILCALL AIL_API_DLS_load_file   (HDLSDEVICE dls,
                                                   char const FAR* filename,
                                                   U32 flags);

extern HDLSFILEID AILCALL AIL_API_DLS_load_memory (HDLSDEVICE dls,
                                                   void const FAR* memfile,
                                                   U32 flags);

extern void AILCALL AIL_API_DLS_unload            (HDLSDEVICE dls,
                                                   HDLSFILEID dlsid);

extern void AILCALL AIL_API_DLS_compact           (HDLSDEVICE dls);

extern void AILEXPORT AIL_API_DLS_set_reverb_levels(HDLSDEVICE dls,
                                                    F32   dry_level,
                                                    F32   wet_level);

extern void AILEXPORT AIL_API_DLS_get_reverb_levels(HDLSDEVICE dls,
                                                    F32 FAR*    dry_level,
                                                    F32 FAR*    wet_level);

extern void AILCALL AIL_API_DLS_get_info          (HDLSDEVICE dls,
                                                   AILDLSINFO FAR* info,
                                                   S32 FAR* PercentCPU);

extern S32 AILCALL AIL_API_extract_DLS            (void const FAR*source_image, //)
                                                   U32      source_size,
                                                   void FAR * FAR *XMI_output_data,
                                                   U32  FAR       *XMI_output_size,
                                                   void FAR * FAR *DLS_output_data,
                                                   U32  FAR       *DLS_output_size,
                                                   AILLENGTHYCB    callback);

extern void AILCALL AIL_API_set_sequence_ms_position
                                                  (HSEQUENCE S, //)
                                                   S32       milliseconds);

extern void AILCALL AIL_API_sequence_ms_position  (HSEQUENCE S, //)
                                                   S32 FAR *total_milliseconds,
                                                   S32 FAR *current_milliseconds);

extern void AILCALL AIL_API_set_sample_ms_position(HSAMPLE S, //)
                                                   S32       milliseconds);

extern void AILCALL AIL_API_sample_ms_position    (HSAMPLE S, //)
                                                   S32 FAR    *total_milliseconds,
                                                   S32 FAR   *current_milliseconds);

extern void AILCALL AIL_API_set_stream_ms_position(HSTREAM   S, //)
                                                   S32       milliseconds);

extern void AILCALL AIL_API_stream_ms_position    (HSTREAM   S, //)
                                                   S32 FAR  *total_milliseconds,
                                                   S32 FAR  *current_milliseconds);

extern void AILCALL AIL_API_save_sample_attributes(HSAMPLE   S,
                                                   HSATTRIBS D);

extern U32 AILCALL AIL_API_load_sample_attributes(HSAMPLE   S,
                                                  HSATTRIBS D);

extern S32 AILCALL AIL_API_WAV_info               (void const FAR* data,
                                                   AILSOUNDINFO FAR* info);

extern S32 AILCALL AIL_API_compress_ADPCM         (AILSOUNDINFO const FAR* info, //)
                                                   void FAR* FAR* outdata, U32 FAR* outsize);

extern S32 AILCALL AIL_API_decompress_ADPCM       (AILSOUNDINFO const FAR* info,
                                                   void FAR* FAR* outdata,
                                                   U32 FAR* outsize);

extern S32 AILCALL AIL_API_file_type              (void const FAR* data,
                                                   U32 size);

extern S32 AILCALL AIL_API_file_type_named        (void const FAR* data,
                                                   char const FAR* filename,
                                                   U32 size);

extern S32 AILCALL AIL_API_find_DLS               (void const FAR* data,
                                                   U32 size,
                                                   void FAR* FAR*xmi,
                                                   U32 FAR* xmisize,
                                                   void FAR* FAR*dls,
                                                   U32 FAR* dlssize);

#ifdef IS_DOS

//
// Low-level support services
//

extern REALFAR AILCALL AIL_API_get_real_vect          (U32 vectnum);

extern void    AILCALL AIL_API_set_real_vect          (U32 vectnum,
                                                       REALFAR real_ptr);

extern void    AILCALL AIL_API_set_USE16_ISR          (S32 IRQ,
                                                       REALFAR real_base,
                                                       U32 ISR_offset);

extern void    AILCALL AIL_API_restore_USE16_ISR      (S32 IRQ);

extern U32     AILCALL AIL_API_disable_interrupts     (void);

extern void    AILCALL AIL_API_restore_interrupts     (U32 FD_register);

extern S32     AILCALL AIL_API_call_driver            (AIL_DRIVER *drvr,
                                                       S32 fn,
                                                       VDI_CALL *in,
                                                       VDI_CALL *out);

extern S32     AILCALL AIL_API_read_INI               (AIL_INI *INI,
                                                       char *filename);

//
// Low-level installation services
//

extern IO_PARMS*   AILCALL AIL_API_get_IO_environment (AIL_DRIVER *drvr);

extern AIL_DRIVER* AILCALL AIL_API_install_driver     (U8 const *driver_image,
 U32 n_bytes);

extern void        AILCALL AIL_API_uninstall_driver   (AIL_DRIVER *drvr);

#endif

//
// Internal MSS mixer RIB calls
//

extern U32  AILCALL MSS_MMX_available (void);

extern void AILCALL MSS_mixer_startup  (void);
extern void AILCALL MSS_mixer_shutdown (void);

extern void AILCALL MSS_mixer_flush(S32 FAR *dest,
                                    S32      len
#ifdef IS_X86
                                   ,U32             MMX_available
#endif
                                    );

extern void AILCALL MSS_mixer_merge(void const FAR * FAR *src,
                                    U32        FAR *src_fract,
                                    void FAR       *src_end,
                                    S32  FAR * FAR *dest,
                                    void FAR       *dest_end,
                                    S32  FAR       *left_val,
                                    S32  FAR       *right_val,
                                    S32             playback_ratio,
                                    S32             scale_left,
                                    S32             scale_right,
                                    U32             operation
#ifdef IS_X86
                                   ,U32             MMX_available
#endif
                                    );

extern void AILCALL MSS_mixer_copy(void const FAR *src,
                                   S32       src_len,
                                   void FAR *dest,
                                   U32       operation
#ifdef IS_BE
                                   ,S32       big_endian_output
#else
#ifdef IS_PS2
                                   ,U32       IOP_overwrite
#else
#ifdef IS_X86
                                   ,U32       MMX_available
#endif
#endif
#endif
                                   );

extern void AILCALL SS_set_speaker_configuration (D3DSTATE        *D3D, 
                                                  MSSVECTOR3D FAR *speaker_positions,
                                                  S32              n_channels,
                                                  S32              logical_channels_per_sample);

extern void SS_calculate_3D_channel_levels(D3DSTATE          FAR *D3D, 
                                            S32                    logical_channels_per_sample,
                                            S3DSTATE          FAR *S3D,
                                            F32               FAR *channel_levels);

extern S32 AILCALL SS_calculate_3D_attenuation(D3DSTATE    FAR  *D3D, //)
                                               S3DSTATE    FAR  *S3D,
                                               F32 FAR          *distance,
                                               MSSVECTOR3D FAR  *listener_to_source,
                                               AILFALLOFFCB      falloff_function);

extern void AILCALL SS_update_driver_reverb_state(HDIGDRIVER dig);

extern void AILCALL SS_update_sample_reverb_state(HSAMPLE S);

extern void AILCALL SS_flush (HDIGDRIVER dig);

extern void AILCALL SS_copy (HDIGDRIVER dig, void FAR *lpWaveAddr);

extern F32 AILCALLBACK SS_default_falloff_function_callback(HSAMPLE   S, //)
                                                            F32       distance,
                                                            F32       rolloff_factor,
                                                            F32       min_dist,
                                                            F32       max_dist);
extern S32  SS_merge (HSAMPLE  S, //)
                      S32      in_len,
                      S32      out_pos,
                      S16 FAR *dest_mono_sample_buffer,
                      S32      dest_buffer_size);

extern S32 AILCALLBACK SS_stream_to_buffer(HSAMPLE  S,
                                           S16 FAR *dest_mono_sample_buffer,
                                           S32      dest_buffer_size);

extern void AILCALL SS_stream_and_mix(HSAMPLE S);

extern void AILCALL SS_start_DIG_driver_playback(HSAMPLE S);

extern void AILCALL SS_set_sample_type( HSAMPLE S, S32 format, U32 flags );

extern void AILCALL AIL_apply_reverb( HDIGDRIVER dig );

extern S32 AILCALL AIL_allocate_reverb_buffers( HDIGDRIVER dig );

extern void AILCALL AIL_apply_lowpass( void FAR* dest, void const FAR* src, void const FAR* src_end, LOWPASS_INFO FAR* lp_in, S32 op );

#ifdef IS_WIN32
extern S32 AILCALL get_system_speaker_configuration(MSS_MC_SPEC *channel_spec);
#endif



#ifdef IS_BE

  #ifdef IS_PPC

    #ifdef IS_XENON
      #define LE_SWAP32( ptr ) __loadwordbytereverse(0,ptr)

      #define STORE_LE_SWAP32( ptr, val )  __storewordbytereverse ( (U32)(val), 0, ptr )
    #else
    #ifdef IS_PS3
      #define LE_SWAP32( ptr ) ld_le32(ptr)
                                                   // the char* is not required, but works around a GCC bug  
      #define STORE_LE_SWAP32( ptr, val )  st_le32( (char*)(ptr), (U32)(val) )
    #else
      #define LE_SWAP32( ptr ) __lwbrx(ptr,0)

      #define STORE_LE_SWAP32( ptr, val )  __stwbrx( (U32)(val), ptr, 0 )
    #endif
    #endif

  #else

    #define LE_SWAP32(ptr) \
                     ( ( ( ( *((U32 FAR *)(ptr) ) ) << 24 ) ) | \
                       ( ( ( *((U32 FAR *)(ptr) ) ) <<  8 ) & 0x00FF0000 ) | \
                       ( ( ( *((U32 FAR *)(ptr) ) ) >>  8 ) & 0x0000FF00 ) | \
                       ( ( ( *((U32 FAR *)(ptr) ) ) >> 24 ) ) )

    #define STORE_LE_SWAP32( ptr, val )  { *((U32 FAR*)ptr) =
                     ( ( ( ( ((U32)(val) ) ) << 24 ) ) | \
                       ( ( ( ((U32)(val) ) ) <<  8 ) & 0x00FF0000 ) | \
                       ( ( ( ((U32)(val) ) ) >>  8 ) & 0x0000FF00 ) | \
                       ( ( ( ((U32)(val) ) ) >> 24 ) ) );  }

  #endif

  #ifdef IS_PPC

    #ifdef IS_XENON
      unsigned short      __loadshortbytereverse  (int offset, const void * base);
      unsigned long       __loadwordbytereverse   (int offset, const void * base);

      void            __storeshortbytereverse (unsigned short val, int offset, void * base);
      void            __storewordbytereverse  (unsigned int   val, int offset, void * base);

      #define LE_SWAP16( ptr ) __loadshortbytereverse (0,(S16 *)ptr)

      #define LE_SWAP16_OFS( ptr, const_ofs ) __loadshortbytereverse (const_ofs,(S16 *)ptr)

      #define STORE_LE_SWAP16( ptr, val )  __storeshortbytereverse ( (U16)(val), 0, (S16 *)ptr )

      #define STORE_LE_SWAP16_OFS( ptr, val, const_ofs )  __storeshortbytereverse ( (U16)(val), const_ofs, (S16 *)ptr )
    #else
    #ifdef IS_PS3

#define __fsel( outf, cmp, inf1, inf2 ) \
   __asm__ __volatile__ ("fsel %0,%1,%2,%3" : "=f" (outf) : "f" (cmp), "f" (inf1), "f" (inf2));

static inline float fclip1(float sample )
{
  float ret;

  __fsel( ret, sample + 1.0f, sample, -1.0f );
  __fsel( ret, sample - 1.0f, 1.0f, ret );
  return( ret );
}

      static __inline__ U16 ld_le16(const S16 *addr)
      {
        U16 val;
        __asm__ __volatile__ ("lhbrx %0,0,%1" : "=r" (val) : "r" (addr));
        return val;
      }

      static __inline__ U16 ld_le16_ofs(const S16 *addr, U64 const_ofs )
      {
        U16 val;
        __asm__ __volatile__ ("lhbrx %0,%1,%2" : "=r" (val) : "b" (const_ofs), "r" (addr));
        return val;
      }

      static __inline__ void st_le16(S16 *addr, const U16 val)
      {
        __asm__ __volatile__ ("sthbrx %1,0,%2" : "=m" (*addr) : "r" (val), "r" (addr));
      }

      static __inline__ void st_le16_ofs(S16 *addr, U64 const_ofs, const U16 val)
      {
        __asm__ __volatile__ ("sthbrx %2,%1,%3" : "=m" (*addr) : "b" (const_ofs), "r" (val), "r" (addr));
      }

      static __inline__ U32 ld_le32(const void *addr)
      {
        U32 val;

        __asm__ __volatile__ ("lwbrx %0,0,%1" : "=r" (val) : "r" ((U32*)addr) );
        return val;
      }

/*      static __inline__ void st_le32_ofs(void *addr, U64 const_ofs, const U32 val)
      {
        __asm__ __volatile__ ("stwbrx %2,%1,%3" : "=m" (*(U32*)addr) : "b" (const_ofs), "r" (val), "r" ((U32*)addr));
      }*/

      static __inline__ void st_le32(void *addr, const U32 val)
      {                                                        //todo, weird hacks to make this work with GCC
        __asm__ __volatile__ ("stwbrx %1,%3,%2" : "=m" (*(U32*)addr) : "r" (val), "r" ((U32*)addr), "O"(0) );
      }

      #define LE_SWAP16( ptr ) ld_le16 ((S16 *)ptr)

      #define LE_SWAP16_OFS( ptr, const_ofs ) ld_le16_ofs ((S16 *)ptr, const_ofs)

      #define STORE_LE_SWAP16( ptr, val )  st_le16( (S16 *)ptr, (U16)(val) )

      #define STORE_LE_SWAP16_OFS( ptr, val, const_ofs ) st_le16_ofs( (S16 *)ptr, const_ofs, (U16)(val) )
    #else
      #define LE_SWAP16( ptr ) __lhbrx((S16 *)ptr,0)

      #define LE_SWAP16_OFS( ptr, const_ofs ) __lhbrx((S16 *)ptr,const_ofs)

      #define STORE_LE_SWAP16( ptr, val )  __sthbrx( (U16)(val), (S16 *)ptr, 0 )

      #define STORE_LE_SWAP16_OFS( ptr, val, const_ofs )  __sthbrx( (U16)(val), (S16 *)ptr, const_ofs )
   #endif
   #endif


  #else

    #define LE_SWAP16(ptr) \
                     ( ( U16 ) \
                       ( ( ( ( *((U16 FAR *)(ptr) ) ) << 8 ) ) | \
                         ( ( ( *((U16 FAR *)(ptr) ) ) >> 8 ) ) ) \
                     )

    #define LE_SWAP16_OFS(ptr,const_ofs) \
                     ( ( U16 ) \
                       ( ( ( ( *((U16 FAR *)(AIL_ptr_add(ptr,const_ofs)) ) ) << 8 ) ) | \
                         ( ( ( *((U16 FAR *)(AIL_ptr_add(ptr,const_ofs)) ) ) >> 8 ) ) ) \
                     )

    #define STORE_LE_SWAP16( ptr, val )  { *((U16 FAR*)ptr) =
                       ( ( ( ((U16)(val) ) ) <<  8 ) ) | \
                       ( ( ( ((U16)(val) ) ) >>  8 ) ) ); }

  #endif

  #define BE_SWAP32( ptr ) ( *((U32 FAR *)(ptr) ) )
  #define BE_SWAP16( ptr ) ( *((U16 FAR *)(ptr) ) )

  #define MEM_LE_SWAP32(n) *((U32*)n) = LE_SWAP32(n);
  #define MEM_LE_SWAP16(n) *((U16*)n) = (U16) LE_SWAP16(n);

  #define MEM_BE_SWAP32(n)
  #define MEM_BE_SWAP16(n)

  // unaligned versions
  #define BEU_SWAP32( ptr ) ( *((U32 FAR *)(ptr) ) )
  #define BEU_SWAP16( ptr ) ( *((U16 FAR *)(ptr) ) )

  #define LEU_SWAP32(ptr) LE_SWAP32(ptr)
  #define LEU_SWAP16(ptr) LE_SWAP16(ptr)

  #define STORE_LEU_SWAP32( ptr, val )  STORE_LE_SWAP32( ptr, val )
  #define STORE_LEU_SWAP16( ptr, val )  STORE_LE_SWAP16( ptr, val )

  #define MEM_LEU_SWAP32(n) *((U32*)n) = LEU_SWAP32(n);
  #define MEM_LEU_SWAP16(n) *((U32*)n) = LEU_SWAP32(n);

#else

  #define LE_SWAP32( ptr ) ( *((U32 FAR *)(ptr) ) )
  #define LE_SWAP16( ptr ) ( *((U16 FAR *)(ptr) ) )

  #define LE_SWAP16_OFS( ptr, const_ofs ) ( *((U16 FAR *)(AIL_ptr_add(ptr,const_ofs)) ) )

  #define STORE_LE_SWAP32( ptr, val )  { *((U32 FAR*)(ptr))=(U32)(val); }
  #define STORE_LE_SWAP16( ptr, val )  { *((U16 FAR*)(ptr))=(U16)(val); }
  #define STORE_LE_SWAP16_OFS( ptr, val, const_ofs )  { *((U16 FAR*)((((U8*)ptr)+const_ofs)))=(U16)(val); }

  #define BE_SWAP32(ptr) \
                     ( ( ( ( *((U32 FAR *)(ptr) ) ) << 24 ) ) | \
                       ( ( ( *((U32 FAR *)(ptr) ) ) <<  8 ) & 0x00FF0000 ) | \
                       ( ( ( *((U32 FAR *)(ptr) ) ) >>  8 ) & 0x0000FF00 ) | \
                       ( ( ( *((U32 FAR *)(ptr) ) ) >> 24 ) ) )

  #define BE_SWAP16(ptr) \
                     ( ( U16 ) \
                       ( ( ( ( *((U16 FAR *)(ptr) ) ) << 8 ) ) | \
                         ( ( ( *((U16 FAR *)(ptr) ) ) >> 8 ) ) ) \
                     )

#ifdef IS_PS2

  #define putunaligned4(temp, s ) \
  __asm__("swl %0,3+%1" : : "r"(temp), "o"(*(U32*)(s)));  \
  __asm__("swr %0,%1" : : "r"(temp), "o"(*(U32*)(s)) ); 

  #define getunaligned4(temp, s ) \
  __asm__("lwl %0,3+%1" : "=r"(temp) : "o"(*(U32*)(s)));  \
  __asm__("lwr %0,%1" : "+r"(temp) : "o"(*(U32*)(s)) ); 

  static inline U32 LEU_SWAP32( void * ptr )
  {
    U32 ret;
    getunaligned4( ret, ptr );
    return( ret );
  }

  static inline U32 BEU_SWAP32( void * ptr )
  {
    U32 ret;

    getunaligned4( ret, ptr );
    ret = ( ret << 24 ) | ( ( ret << 8 ) & 0x00ff0000 ) | ( ( ret >> 8 ) & 0x0000ff00 ) | ( ret >> 24 );
    return( ret );
  }

  #define LEU_SWAP16(ptr) \
                     ( ( U16 ) \
                       ( ( ( ( ((U32)(((U8 FAR *)(ptr))[1]) ) ) << 8 ) ) | \
                         ( ( ( ((U32)(((U8 FAR *)(ptr))[0]) ) ) ) ) ) \
                     )
  #define BEU_SWAP16(ptr) \
                     ( ( U16 ) \
                       ( ( ( ( ((U32)(((U8 FAR *)(ptr))[0]) ) ) << 8 ) ) | \
                         ( ( ( ((U32)(((U8 FAR *)(ptr))[1]) ) ) ) ) ) \
                     )

  #define STORE_LEU_SWAP32( ptr, val )  { register U32 __v = (U32)val; putunaligned4( __v, ptr ); }
  #define STORE_LEU_SWAP16( ptr, val )  { register U16 __v = (U16)val; ((U8 FAR*)(ptr))[0]=(U8)val; ((U8 FAR*)(ptr))[1]=(U8)(val>>8);}

#else

  #define LEU_SWAP32(ptr) LE_SWAP32(ptr)
  #define LEU_SWAP16(ptr) LE_SWAP16(ptr)

  #define BEU_SWAP32(ptr) BE_SWAP32(ptr)
  #define BEU_SWAP16(ptr) BE_SWAP16(ptr)

  #define STORE_LEU_SWAP32( ptr, val )  STORE_LE_SWAP32( ptr, val )
  #define STORE_LEU_SWAP16( ptr, val )  STORE_LE_SWAP16( ptr, val )

#endif

  #define MEM_LEU_SWAP32(n)
  #define MEM_LEU_SWAP16(n)

  #define MEM_LE_SWAP32(n)
  #define MEM_LE_SWAP16(n)

  #define MEM_BE_SWAP32(n) *((U32*)n) = BE_SWAP32(n);
  #define MEM_BE_SWAP16(n) *((U16*)n) = BE_SWAP16(n);

#endif



#if defined(IS_MAC) || defined(IS_WII)

#ifdef IS_X86

static inline U32 mult64addsubandshift( U32 mt1, U32 mt2, U32 addv, U32 subv, U32 shift )
{
  U32 retv;
  __asm
  {
    mov eax,[mt1]
    mov ecx,[mt2]
    mul ecx
    add eax,[addv]
    adc edx,0
    sub eax,[subv]
    sbb edx,0
    mov ecx,[shift]
    shrd eax,edx,cl
    mov [retv], eax
  }
  return( retv );
}

static inline U32 mult64anddiv(U32 a,U32 b, U32 c)
{
  U32 retv;
  __asm
  {
    mov eax,[a]
    mov ecx,[b]
    mul ecx
    mov ecx,[c]
    div ecx
    mov [retv], eax
  }

  return( retv );
}

static inline U32 shift64addsubanddiv( U32 val, U32 shift,U32 addv, U32 subv, U32 divv)
{
  U32 retv;
  __asm
  {
    xor edx, edx
    mov eax, [val]
    mov ecx, [shift]
    shld edx, eax, cl
    shl eax, cl
    add eax, [addv]
    adc edx, 0
    sub eax, [subv]
    sbb edx, 0
    mov ecx, [divv]
    idiv ecx
    mov [retv], eax
  }
  return( retv );
}


#else

//
// These three dudes help deal with u64s in hi,lo format
//

#define mul64hilo(_hi,_lo,_mt1,_mt2)    \
  __asm                                 \
  {                                     \
    mulhwu _hi, _mt1, _mt2;             \
    mullw _lo, _mt1, _mt2               \
  }                                     \


#define imul64hilo(_hi,_lo,_mt1,_mt2)    \
  __asm                                 \
  {                                     \
    mulhw _hi, _mt1, _mt2;             \
    mullw _lo, _mt1, _mt2               \
  }                                     \


#define add64hilo(_hi,_lo,_hisub,_losub)    \
  __asm                                     \
  {                                         \
    addc _lo, _losub, _lo;                  \
    adde _hi, _hisub, _hi                   \
  }                                         \


#define sub64hilo(_hi,_lo,_hisub,_losub)    \
  __asm                                     \
  {                                         \
    subfc _lo, _losub, _lo;                 \
    subfe _hi, _hisub, _hi                  \
  }                                         \

#define carry_check( q, r, c, d ) \
  if ( r < c )                    \
  {                               \
    --q;                          \
    r += d;                       \
    if ( r >= d )                 \
    {                             \
      if ( r < c )                \
      {                           \
        --q;                      \
        r += d;                   \
      }                           \
    }                             \
  }                             


static inline U32 div64with16( U32 nlo, U32 nhi, U32 d )
{
  U32 dlo, dhi;
  U32 rlo, rhi;
  U32 qhi, qlo;
  U32 carry;
  
  dhi = d >> 16;
  dlo = d & 0xffff;
  
  qhi = nhi / dhi;
  rhi = nhi % dhi;
  
  carry = qhi * dlo;
  
  rhi = ( rhi << 16 ) | ( nlo >> 16 );
  
  carry_check( qhi, rhi, carry, d );
  rhi -= carry;
    
  qlo = rhi / dhi;
  rlo = rhi % dhi;
  carry = qlo * dlo;
 
  qhi <<= 16;
    
  rlo = ( rlo << 16 ) |  ( nlo & 0xffff );
  carry_check( qlo, rlo, carry, d );

//  rlo -= carry; 

  return( qhi | qlo );   
}

static U32 inline mult64anddiv( register U32 a, register U32 b,U32 d )
{
/*  register U32 hi, lo;
  register U32 mt1, mt2, d;

  mt1=m1;
  mt2=m2;
  d=dv;

  mul64hilo( hi, lo, mt1, mt2 );

  return( div64(hi,lo,d) );
*/  
  U32 lz;
  register U32 nhi, nlo;

  mul64hilo( nhi, nlo, a, b );

  if ( ( d & ( d - 1 ) ) == 0 )
  {
    lz = (U32) __cntlzw( d );

    // Shift for powers of 2. 
    return( ( nhi << ( lz + 1 ) ) | ( nlo >> ( 31 - lz ) ) );
  }

  if ( nhi == 0 )
  {
    return( nlo / d );
  }

  lz = (U32) __cntlzw( d );

  d <<= lz;
  nhi = ( nhi << lz ) + ( nlo >> ( 32 - lz ) );
  nlo <<= lz;

  return( div64with16( nlo, nhi, d ) );
  
}

static U32 inline mult64andshift( U32 m1,U32 m2,U32 shift )
{
  register U32 hi, lo;
  register U32 mt1, mt2;

  mt1=m1;
  mt2=m2;

  mul64hilo( hi, lo, mt1, mt2 );

  return( ( hi << (32 - shift ) ) + ( lo >> shift ) );
}

static U32 inline mult64addsubandshift( U32 m1, U32 m2, U32 av, U32 sv, U32 shift )
{
  register U32 hi, lo;
  register U32 mt1, mt2;
  register U32 addv;
  register U32 subv;
  register U32 zero = 0;

  mt1=m1;
  mt2=m2;
  addv=av;
  subv=sv;

  mul64hilo(hi,lo,mt1,mt2);
  add64hilo( hi, lo, zero, addv );
  sub64hilo( hi, lo, zero, subv );

  return( ( hi << (32 - shift ) ) + ( lo >> shift ) );
}

static U32 __inline shift64addsubanddiv( U32 val, U32 shift,U32 av, U32 sv, U32 divv)
{
  register U32 hi, lo;
  register U32 addv;
  register U32 subv;
  register U32 d;
  register U32 zero = 0;
  U32 lz;

  addv=av;
  subv=sv;
  d=divv;

  hi = val >> ( 32 - shift );
  lo = val << shift;
  add64hilo( hi, lo, zero, addv );
  sub64hilo( hi, lo, zero, subv );

  if ( hi & 0x80000000 )
  {
    register U32 ihi = hi;
    register U32 ilo = lo;
    hi = lo = 0;
    sub64hilo( hi, lo, ihi, ilo );
    
    if ( ( d & ( d - 1 ) ) == 0 )
    {
      lz = (U32) __cntlzw( d );

      // Shift for powers of 2. 
      return( (U32) -(S32) ( ( hi << ( lz + 1 ) ) | ( lo >> ( 31 - lz ) ) ) );
    }

    if ( hi == 0 )
    {
      return( (U32) -(S32) ( lo / d ) );
    }

    return( (U32) -(S32) div64with16( lo,hi,d ) );
  }  
    
  if ( ( d & ( d - 1 ) ) == 0 )
  {
    lz = (U32) __cntlzw( d );

    // Shift for powers of 2. 
    return( ( hi << ( lz + 1 ) ) | ( lo >> ( 31 - lz ) ) );
  }

  if ( hi == 0 )
  {
    return( lo / d );
  }

  return( div64with16( lo,hi,d ) );
}

#define WRITE_MONO_SAMPLE( dest, fOut, tmp )                   \
{                                                              \
   (void)(sizeof(tmp));                                        \
   if (fOut > 32767.0F)       STORE_LE_SWAP16( dest, 32767 );  \
   else if (fOut < -32768.0F) STORE_LE_SWAP16( dest, -32768 ); \
   else STORE_LE_SWAP16( dest, (S16) fOut );                   \
   ++dest;                                                     \
}

#define WRITE_STEREO_SAMPLE( dest, fOutL, fOutR, tmp )           \
{                                                                \
   (void)(sizeof(tmp));                                          \
   if (fOutL > 32767.0F)       STORE_LE_SWAP16( dest, 32767 );   \
   else if (fOutL < -32768.0F) STORE_LE_SWAP16( dest, -32768 );  \
   else STORE_LE_SWAP16( dest, (S16) fOutL );                    \
   ++dest;                                                       \
   if (fOutR > 32767.0F)       STORE_LE_SWAP16( dest, 32767 );   \
   else if (fOutR < -32768.0F) STORE_LE_SWAP16( dest, -32768 );  \
   else STORE_LE_SWAP16( dest, (S16) fOutR );                    \
   ++dest;                                                       \
}

#endif

#else

#if ( defined(IS_X86) && defined(IS_WIN32API) ) || defined(IS_LINUX)

//
// Macros to aid in writing to build buffers
//

#ifdef IS_WIN64

#define WRITE_MONO_SAMPLE( dest, fOut, tmp )                 \
{                                                            \
   (void)(sizeof(tmp));                                      \
   if (fOut > 32767.0F)       *dest++ =  32767;              \
   else if (fOut < -32768.0F) *dest++ = -32768;              \
   else *dest++ = (S16) fOut;                                \
}

#define WRITE_STEREO_SAMPLE( dest, fOutL, fOutR, tmp )       \
{                                                            \
   (void)(sizeof(tmp));                                      \
   if (fOutL > 32767.0F)       *dest =  32767;               \
   else if (fOutL < -32768.0F) *dest = -32768;               \
   else *dest = (S16) fOutL;                                 \
   if (fOutR > 32767.0F)       dest[1] =  32767;             \
   else if (fOutR < -32768.0F) dest[1] = -32768;             \
   else dest[1] = (S16) fOutR;                               \
   dest+=2;                                                  \
}

#else

#define WRITE_STEREO_SAMPLE( dest_buffer, fOutL, fOutR, tmp ) WRITE_STEREO_SAMPLE_L1( dest_buffer, fOutL, fOutR, tmp, __LINE__ )
#define WRITE_STEREO_SAMPLE_L1( dest_buffer, fOutL, fOutR, tmp, line ) WRITE_STEREO_SAMPLE_L2( dest_buffer, fOutL, fOutR, tmp, line )

#define WRITE_STEREO_SAMPLE_L2( dest_buffer, fOutL, fOutR, tmp, line ) \
__asm                                                       \
{                                                           \
  __asm mov edx,[dest_buffer]                               \
  __asm fld [fOutR]                                         \
  __asm fld [fOutL]                                         \
  __asm fistp dword ptr [edx]                               \
  __asm fistp dword ptr [tmp]                               \
                                                            \
  __asm mov eax,dword ptr [edx]                             \
  __asm cmp eax,32767                                       \
  __asm jg highl##line                                      \
  __asm cmp eax,-32768                                      \
  __asm jl lowl##line                                             \
                                                            \
 __asm cont##line:                                                \
  __asm mov eax,[tmp]                                       \
  __asm cmp eax,32767                                       \
  __asm jg highr##line                                            \
  __asm cmp eax,-32768                                      \
  __asm jl lowr##line                                             \
  __asm mov word ptr [edx+2],ax                             \
  __asm jmp done##line                                            \
                                                            \
  __asm highl##line:                                              \
  __asm mov word ptr [edx],32767                            \
  __asm jmp cont##line                                            \
  __asm lowl##line:                                               \
  __asm mov word ptr [edx],-32768                           \
  __asm jmp cont##line                                            \
                                                            \
  __asm highr##line:                                              \
  __asm mov word ptr [edx+2],32767                          \
  __asm jmp done##line                                            \
  __asm lowr##line:                                               \
  __asm mov word ptr [edx+2],-32768                         \
                                                            \
  __asm done##line:                                               \
  __asm add edx, 4                                          \
  __asm mov [dest_buffer],edx                               \
}                                                           \


#define WRITE_MONO_SAMPLE( dest_buffer, fOut, tmp ) WRITE_MONO_SAMPLE_L1( dest_buffer, fOut, tmp, __LINE__ )
#define WRITE_MONO_SAMPLE_L1( dest_buffer, fOut, tmp, line ) WRITE_MONO_SAMPLE_L2( dest_buffer, fOut, tmp, line )

#define WRITE_MONO_SAMPLE_L2( dest_buffer, fOut, tmp, line )\
__asm                                                       \
{                                                           \
  __asm mov edx,[dest_buffer]                               \
  __asm fld [fOut]                                          \
  __asm fistp dword ptr [tmp]                               \
                                                            \
  __asm mov eax,[tmp]                                       \
  __asm cmp eax,32767                                       \
  __asm jg higha##line                                      \
  __asm cmp eax,-32768                                      \
  __asm jl lowa##line                                       \
  __asm mov word ptr [edx],ax                               \
  __asm jmp donea##line                                     \
                                                            \
  __asm higha##line:                                        \
  __asm mov word ptr [edx],32767                            \
  __asm jmp donea##line                                     \
  __asm lowa##line:                                         \
  __asm mov word ptr [edx],-32768                           \
                                                            \
  __asm donea##line:                                        \
  __asm add edx, 2                                          \
  __asm mov [dest_buffer],edx                               \
}

#endif

static U32 __inline mult64addsubandshift( U32 mt1, U32 mt2, U32 addv, U32 subv, U32 shift )
{
  U32 retv;

#ifdef __GNUC__
  unsigned long long value;
  value = (unsigned long long)mt1 * mt2;
  value += addv;
  value -= subv;
  value = (signed long long )value >> (signed long long )shift;
  retv = (U32)value;
#else
#ifdef IS_WIN64
  U64 value;
  value = (U64) mt1 * mt2;
  value += addv;
  value -= subv;
  value = (S64) value >> (S64) shift;
  retv = (U32) value;
#else
  __asm
  {
    mov eax,[mt1]
    mov ecx,[mt2]
    mul ecx
    add eax,[addv]
    adc edx,0
    sub eax,[subv]
    sbb edx,0
    mov ecx,[shift]
    shrd eax,edx,cl
    mov [retv], eax
  }
#endif
#endif
  return( retv );
}

static U32 __inline mult64anddiv(U32 a,U32 b, U32 c)
{
  U32 retv;

#ifdef __GNUC__
  unsigned long long value;
  value = (unsigned long long)a * b;
  value /= c;
  retv = (U32)value;
#else
#ifdef IS_WIN64
  U64 value;
  value = (U64) a * b;
  value /= c;
  retv = (U32) value;
#else
  __asm
  {
    mov eax,[a]
    mov ecx,[b]
    mul ecx
    mov ecx,[c]
    div ecx
    mov [retv], eax
  }
#endif
#endif
  return( retv );
}

static U32 __inline shift64addsubanddiv( U32 val, U32 shift,U32 addv, U32 subv, U32 divv)
{
  U32 retv;
#ifdef __GNUC__
  unsigned long long value;
  value = (unsigned long long)val;
  value <<= shift;
  value += addv;
  value -= subv;
  value = (signed long long ) value / (signed long long ) divv;
  retv = (U32)value;
#else
#ifdef IS_WIN64
  U64 value;
  value = (U64) val;
  value <<= shift;
  value += addv;
  value -= subv;
  value = (S64) value / (S64) divv;
  retv = (U32) value;
#else
  __asm
  {
    xor edx, edx
    mov eax, [val]
    mov ecx, [shift]
    shld edx, eax, cl
    shl eax, cl
    add eax, [addv]
    adc edx, 0
    sub eax, [subv]
    sbb edx, 0
    mov ecx, [divv]
    idiv ecx
    mov [retv], eax
  }
#endif
#endif
  return( retv );
}

#else

#ifdef __WATCOMC__

#define sinf sin

U32 mult64anddiv(U32 mt1,U32 mt2,U32 d);
#pragma aux mult64anddiv = "mul ecx" "div ebx" parm [eax] [ecx] [ebx] modify [EDX eax];

U32 mult64addsubandshift(U32 mt1,U32 mt2,U32 addv, U32 subvv, U32 shift);
#pragma aux mult64addsubandshift = "mul ebx" "add eax,esi" "adc edx,0" "sub eax,edi" "sbb edx,0" "shrd eax,edx,cl" parm [eax] [ebx] [esi] [edi] [ecx] modify [EDX eax ecx];

U32 shift64addsubanddiv(U32 val,U32 shift,U32 addv, U32 subv, U32 divv);
#pragma aux shift64addsubanddiv = "xor edx,edx" "shld edx,eax,cl" "shl eax,cl" "add eax,esi" "adc edx,0" "sub eax,edi" "sbb edx,0" "idiv ebx" parm [eax] [ecx] [esi] [edi] [ebx] modify [EDX eax];

#define WRITE_STEREO_SAMPLE( dest_buffer, fOutL, fOutR, tmp ) dest_buffer = WRITE_STEREO_SAMPLE_2( dest_buffer, fOutL, fOutR, &tmp )
S16 * WRITE_STEREO_SAMPLE_2( void * dest_buffer, float fOutL, float fOutR, void * tmp );
#pragma aux WRITE_STEREO_SAMPLE_2 =                    \
  "fistp dword ptr [edx]"                              \
  "fistp dword ptr [esi]"                              \
                                                       \
  "mov eax,dword ptr [edx]"                            \
  "cmp eax,32767"                                      \
  "jg highl"                                           \
  "cmp eax,-32768"                                     \
  "jl lowl"                                            \
                                                       \
 "cont:"                                               \
  "mov eax,[esi]"                                      \
  "cmp eax,32767"                                      \
  "jg highr"                                           \
  "cmp eax,-32768"                                     \
  "jl lowr"                                            \
  "mov word ptr [edx+2],ax"                            \
  "jmp done"                                           \
                                                       \
  "highl:"                                             \
  "mov word ptr [edx],32767"                           \
  "jmp cont"                                           \
  "lowl:"                                              \
  "mov word ptr [edx],-32768"                          \
  "jmp cont"                                           \
                                                       \
  "highr:"                                             \
  "mov word ptr [edx+2],32767"                         \
  "jmp done"                                           \
  "lowr:"                                              \
  "mov word ptr [edx+2],-32768"                        \
                                                       \
  "done:"                                              \
  "add edx, 4"                                         \
parm [edx] [8087] [8087] [esi] value [edx] modify [EDX eax 8087];


#define WRITE_MONO_SAMPLE( dest_buffer, fOut, tmp ) dest_buffer = WRITE_MONO_SAMPLE_2( dest_buffer, fOut, &tmp )
S16 * WRITE_MONO_SAMPLE_2( void * dest_buffer, float fOut, void * tmp );
#pragma aux WRITE_MONO_SAMPLE_2 =                      \
  "fistp dword ptr [esi]"                              \
                                                       \
  "mov eax,[esi]"                                      \
  "cmp eax,32767"                                      \
  "jg highr"                                           \
  "cmp eax,-32768"                                     \
  "jl lowr"                                            \
  "mov word ptr [edx],ax"                              \
  "jmp done"                                           \
                                                       \
  "highr:"                                             \
  "mov word ptr [edx],32767"                           \
  "jmp done"                                           \
  "lowr:"                                              \
  "mov word ptr [edx],-32768"                          \
                                                       \
  "done:"                                              \
  "add edx, 2"                                         \
parm [edx] [8087] [esi] value [edx] modify [EDX eax 8087];

#else

#if defined( IS_XENON ) 

#define toU64(v) ((U64)((U32)(v)))
#define toS64(v) ((S64)((S32)(v)))

#define mult64anddiv(mt1,mt2,d) ((U32)((toU64(mt1)*toU64(mt2))/toU64(d)))

#define mult64addsubandshift(mt1,mt2,addv,subv,d) ((U32)((((S64)((toU64(mt1)*toU64(mt2))+toU64(addv)))-toS64(subv))>>toS64(d)))

#define shift64addsubanddiv(val,shift,addv,subv,divv) ((U32)(((S64)((toU64(val)<<toU64(shift))+toU64(addv)-toU64(subv)))/toS64(divv)))

#define WRITE_MONO_SAMPLE( dest, fOut, tmp )                   \
{                                                              \
   (void)(sizeof(tmp));                                        \
   if (fOut > 32767.0F)       STORE_LE_SWAP16( dest, 32767 );  \
   else if (fOut < -32768.0F) STORE_LE_SWAP16( dest, -32768 ); \
   else STORE_LE_SWAP16( dest, (S16) fOut );                   \
   ++dest;                                                     \
}

#define WRITE_STEREO_SAMPLE( dest, fOutL, fOutR, tmp )           \
{                                                                \
   (void)(sizeof(tmp));                                          \
   if (fOutL > 32767.0F)       STORE_LE_SWAP16( dest, 32767 );   \
   else if (fOutL < -32768.0F) STORE_LE_SWAP16( dest, -32768 );  \
   else STORE_LE_SWAP16( dest, (S16) fOutL );                    \
   ++dest;                                                       \
   if (fOutR > 32767.0F)       STORE_LE_SWAP16( dest, 32767 );   \
   else if (fOutR < -32768.0F) STORE_LE_SWAP16( dest, -32768 );  \
   else STORE_LE_SWAP16( dest, (S16) fOutR );                    \
   ++dest;                                                       \
}


#else

#if defined( IS_PS3 ) 

#define toU64(v) ((U64)((U32)(v)))
#define toS64(v) ((S64)((S32)(v)))

#define mult64anddiv(mt1,mt2,d) ((U32)((toU64(mt1)*toU64(mt2))/toU64(d)))

#define mult64addsubandshift(mt1,mt2,addv,subv,d) ((U32)((((S64)((toU64(mt1)*toU64(mt2))+toU64(addv)))-toS64(subv))>>toS64(d)))

#define shift64addsubanddiv(val,shift,addv,subv,divv) ((U32)(((S64)((toU64(val)<<toU64(shift))+toU64(addv)-toU64(subv)))/toS64(divv)))

static inline float fclip32K(float sample )
{
  float ret;

  __fsel( ret, sample + 32767.0f, sample, -32767.0f );
  __fsel( ret, sample - 32767.0f, 32767.0f, ret );
  return( ret );
}

#define WRITE_MONO_SAMPLE( dest, fOut, tmp )                   \
{                                                              \
   (void)(sizeof(tmp));                                        \
   STORE_LE_SWAP16( dest, (S16) fclip32K(fOut) );              \
   ++dest;                                                     \
}

#define WRITE_STEREO_SAMPLE( dest, fOutL, fOutR, tmp )           \
{                                                                \
   (void)(sizeof(tmp));                                          \
   STORE_LE_SWAP16( dest, (S16) fclip32K(fOutL) );               \
   STORE_LE_SWAP16_OFS( dest, (S16) fclip32K(fOutR), 2 );        \
   dest += 2;                                                    \
}

#else

#if defined (IS_PS2)

#define toU64(v) ((U64)((U32)(v)))
#define toS64(v) ((S64)((S32)(v)))

#define mult64addsubandshift(mt1,mt2,addv,subv,d) ((U32)((((S64)((toU64(mt1)*toU64(mt2))+toU64(addv)))-toS64(subv))>>toS64(d)))

#define count_leading_zeros_minus_one(count, x) \
  __asm__ ("plzcw %0,%1"              \
     : "=r" (count)                   \
     : "r" (x))

#define carry_check( q, r, c, d ) \
  if ( r < c )                    \
  {                               \
    --q;                          \
    r += d;                       \
    if ( r >= d )                 \
    {                             \
      if ( r < c )                \
      {                           \
        --q;                      \
        r += d;                   \
      }                           \
    }                             \
  }                             

inline static U32 div64with16( U32 nlo, U32 nhi, U32 d )
{
  U32 dlo, dhi;
  U32 rlo, rhi;
  U32 qhi, qlo;
  U32 carry;
  
  dhi = d >> 16;
  dlo = d & 0xffff;
  
  qhi = nhi / dhi;
  rhi = nhi % dhi;
  
  carry = qhi * dlo;
  
  rhi = ( rhi << 16 ) | ( nlo >> 16 );
  
  carry_check( qhi, rhi, carry, d );
  rhi -= carry;
    
  qlo = rhi / dhi;
  rlo = rhi % dhi;
  carry = qlo * dlo;
 
  qhi <<= 16;
    
  rlo = ( rlo << 16 ) |  ( nlo & 0xffff );
  carry_check( qlo, rlo, carry, d );

//  rlo -= carry; 

  return( qhi | qlo );   
}
        
inline static U32 shift64addsubanddiv( U32 val, U32 shift,U32 av, U32 sv, U32 divv)
{
  register U32 hi, lo;
  register U32 addv;
  register U32 subv;
  register U32 d;
  register U32 zero = 0;
  U32 lz;
  S64 hilo;

  addv=av;
  subv=sv;
  d=divv;

  hilo = ((S64) val) << shift;

  hilo = hilo + (S64) av;
  hilo = hilo - (S64) sv;

  if ( hilo < 0 )
  {
    if ( ( d & ( d - 1 ) ) == 0 )
    {
      lz = d >> 1;
      count_leading_zeros_minus_one( lz, lz );

      // Shift for powers of 2. 
      return( (U32)(S32) ( hilo >> ( 31 - lz ) ) );
    }

    hilo = -hilo;
    
    if ( hilo <= 0xffffffff )
    {
      return( (U32) -(S32)( ( ( U32 ) hilo ) / d ) );
    }
 
    return( (U32)-(S32)div64with16( (U32) hilo, (U32) ( hilo >> 32 ), d ) );
  }

  if ( ( d & ( d - 1 ) ) == 0 )
  {
    lz = d >> 1;                              // shift right here so we'll work with signed vals!
    count_leading_zeros_minus_one( lz, lz );  // e.g., 0x10000 = 0x0E, when there are actually 15 leading zeroes...

    // Shift for powers of 2. 
    return( (U32) ( hilo >> ( 31 - lz ) ) );
  }

  if ( hilo <= 0xffffffff )
  {
    return( ( ( ( U32 ) hilo ) / d ) );
  }

  return( div64with16( (U32) hilo, (U32) ( hilo >> 32 ), d ) );
}

inline static U32 mult64anddiv( U32 a, U32 b, U32 d )
{
  U32 lz;
  U64 n;
  
  n = (U64) a * (U64) b;

  if ( ( d & ( d - 1 ) ) == 0 )
  {
    lz = d >> 1;
    count_leading_zeros_minus_one( lz, lz );

    // Shift for powers of 2. 
    return( (U32) ( n >> ( 31 - lz ) ) );
  }

  if ( ( n >> 32 ) == 0 )
  {
    return( ( (U32) n ) / d );
  }

  lz = d >> 1;
  count_leading_zeros_minus_one( lz, lz );

  d = d << lz;
  n = n << lz;

  return( div64with16( (U32) n, (U32) ( n >> 32 ), d ) );
}

inline static U32 mult64andshift( U32 a, U32 b, U32 c ) 
{
  return( ( (U64) a * (U64) b ) >> c );
}

#else
U32 mult64addsubandshift(U32 mt1,U32 mt2,U32 addv, U32 subv, U32 shift);
U32 shift64addsubanddiv(U32 val,U32 shift,U32 addv, U32 subv, U32 divv);
#endif

#if defined ( IS_PS2 ) 


#define WRITE_MONO_SAMPLE( dest, fOut, tmp )                 \
{                                                            \
   (void)(sizeof(tmp));                                      \
   if (fOut > 32767.0F)       *dest++ =  32767;              \
   else if (fOut < -32768.0F) *dest++ = -32768;              \
   else *dest++ = (S16) fOut;                                \
}

#define WRITE_STEREO_SAMPLE( dest, fOutL, fOutR, tmp )       \
{                                                            \
   (void)(sizeof(tmp));                                      \
   if (fOutL > 32767.0F)       *dest =  32767;               \
   else if (fOutL < -32768.0F) *dest = -32768;               \
   else *dest = (S16) fOutL;                                 \
   if (fOutR > 32767.0F)       dest[1] =  32767;             \
   else if (fOutR < -32768.0F) dest[1] = -32768;             \
   else dest[1] = (S16) fOutR;                               \
   dest+=2;                                                  \
}

#endif

#endif

#endif

#endif

#endif

#endif


//
// low-level utility memory file routines
//

typedef MSS_STRUCT _MEMDUMP
{
  void FAR* buffer[1024]; // up to 64 MB
  U8 FAR* current;
  U32 size;
  U32 totalsize;
  U32 curbufnum;
  U32 lastbufnum;
  U32 curpos;
  S32 error;
  S32 expandable;
  U32 eachbuf;
} MEMDUMP;

typedef MSS_STRUCT _MEMDUMP FAR* HMEMDUMP;

HMEMDUMP AIL_mem_open(void FAR* addr,U32 size);

HMEMDUMP AIL_mem_create(void);

S32 AIL_mem_close(HMEMDUMP m, void FAR* FAR* buf, U32 FAR* size);

U32
#if !defined(IS_MAC) && !defined(IS_LINUX) && !defined(IS_PS2) && !defined(IS_PS3) && !defined(IS_WII)
__cdecl
#endif
AIL_mem_printf(HMEMDUMP m, char const FAR* fmt, ...);

U32 AIL_mem_printc(HMEMDUMP m, char c);

U32 AIL_mem_prints(HMEMDUMP m, char const FAR* s);

U32 AIL_mem_write(HMEMDUMP m, void const FAR* s, U32 bytes);

U32 AIL_mem_read(HMEMDUMP m, void FAR* s, U32 bytes);

U32 AIL_mem_seek(HMEMDUMP m, U32 pos);

U32 AIL_mem_size(HMEMDUMP m);

U32 AIL_mem_pos(HMEMDUMP m);

S32 AIL_mem_error(HMEMDUMP m);


#define DIG_PROCESS_BUFFER_SIZE 2048

extern U8 FAR *ASI_mem_src_ptr;
extern S32     ASI_mem_src_len;
extern S32     ASI_mem_src_pos;

extern S32 AILCALLBACK ASI_mem_stream_CB(UINTa     user, //)
                                         void FAR *dest,
                                         S32       bytes_requested,
                                         S32       offset);

extern S32 AILCALL AIL_time_write(void);

#ifdef IS_DOS

//
// Low-level file i/o routines
//

U32  cdecl AIL_lowfopen(char const * dest,U32 acc);
U32  cdecl AIL_fcreate(char const * dest);
U32  cdecl AIL_fappend(char const * dest);
U32  cdecl AIL_ferase(char const * dest);
U32  cdecl AIL_fread(U32 hand,void* dest,U32 bytes);
U32  cdecl AIL_fwrite(U32 hand,void const * dest,U32 bytes);
U32  cdecl AIL_lowfseekbegin(U32 hand,U32 pos);
U32  cdecl AIL_lowfseekcur(U32 hand,U32 pos);
U32  cdecl AIL_lowfseekend(U32 hand,U32 pos);
void cdecl AIL_lowfclose(U32 hand);

void AILCALL AIL_memmove(void* dest,void const * source,U32 size);

//
// Stack vs. register mapping routines
//

DXDEC char* __pascal (*AIL_getenv) (char* vari);
DXDEC void __pascal (*AIL_int386) (U32 intnum,void* inr, void* outr);
DXDEC S32 __cdecl (*AIL_sprintf) (char FAR *dest, char const FAR *fmt, ...);
DXDEC S32 __cdecl (*AIL_fprintf) (U32 hand, char const FAR *fmt, ...);

//
// Low-level inline routines for watcom
//

#ifdef __WATCOMC__

S32 AIL_abs(S32 ab);
#pragma aux AIL_abs = "test eax,eax" "jge skip" "neg eax" "skip:" parm [eax];

void AIL_memset(void* dest,S8 value,U32 len);
#pragma aux AIL_memset = "mov ah,al" "mov bx,ax" "shl eax,16" "mov ax,bx" "mov bl,cl" "shr ecx,2" "and bl,3" "rep stosd" "mov cl,bl" "rep stosb" parm [EDI] [AL] [ECX] modify [EAX EDX EBX ECX EDI];

void AIL_memcpy(void* dest,void const * source,U32 size);
#pragma aux AIL_memcpy = "mov bl,cl" "shr ecx,2" "and bl,3" "rep movsd" "mov cl,bl" "rep movsb" parm [EDI] [ESI] [ECX] modify [EBX ECX EDI ESI];

char* AIL_strcpy(void* dest,void const * source);
#pragma aux AIL_strcpy = "mov edx,edi" "lp:" "mov al,[esi]" "inc esi" "mov [edi],al" "inc edi" "cmp al,0" "jne lp" parm [EDI] [ESI] modify [EAX EDX EDI ESI] value [EDX];

U32 AIL_strlen(void const * dest);
#pragma aux AIL_strlen = "mov ecx,0xffffffff" "xor eax,eax" "repne scasb" "not ecx" "dec ecx" parm [EDI] modify [EAX ECX EDI] value [ECX];

char* AIL_strcat(void* dest,void const * source);
#pragma aux AIL_strcat = "cld" "mov ecx,0xffffffff" "mov edx,edi" "xor eax,eax" "repne scasb" "dec edi" "lp:" "lodsb" "stosb" "test al,0xff" "jnz lp" \
  parm [EDI] [ESI] modify [EAX ECX EDI ESI] value [EDX];

S8 AIL_memcmp(void const * s1,void const * s2,U32 len);
#pragma aux AIL_memcmp = "cld" "rep cmpsb" "setne al" "jbe end" "neg al" "end:"  parm [EDI] [ESI] [ECX] modify [ECX EDI ESI];

S8 AIL_strcmp(void const * s1,void const * s2);
#pragma aux AIL_strcmp = "lp:" "mov al,[esi]" "mov ah,[edi]" "cmp al,ah" "jne set" "cmp al,0" "je set" "inc esi" "inc edi" "jmp lp" "set:" "setne al" "jbe end" "neg al" "end:" \
   parm [EDI] [ESI] modify [EAX EDI ESI];

S8 AIL_stricmp(void const * s1,void const * s2);
#pragma aux AIL_stricmp = "lp:" "mov al,[esi]" "mov ah,[edi]" "cmp al,'a'" "jb c1" "cmp al,'z'" "ja c1" "sub al,32" "c1:" "cmp ah,'a'" "jb c2" "cmp ah,'z'" "ja c2" "sub ah,32" "c2:" "cmp al,ah" "jne set" "cmp al,0" "je set" \
   "inc esi" "inc edi" "jmp lp" "set:" "setne al" "jbe end" "neg al" "end:" \
   parm [EDI] [ESI] modify [EAX EDI ESI];

S8 AIL_strnicmp(char const * s1, char const * s2,U32 len);
#pragma aux AIL_strnicmp = "lp:" "mov al,[esi]" "mov ah,[edi]" "cmp al,'a'" "jb c1" "cmp al,'z'" "ja c1" "sub al,32" "c1:" "cmp ah,'a'" "jb c2" "cmp ah,'z'" "ja c2" "sub ah,32" "c2:" "cmp al,ah" "jne set" "cmp al,0" "je set" \
   "dec ecx" "jz set" "inc esi" "inc edi" "jmp lp" "set:" "setne al" "jbe end" "neg al" "end:" \
   parm [EDI] [ESI] [ECX] modify [EAX ECX EDI ESI];

U32 AIL_atoi(void const * dest);
 #pragma aux AIL_atoi = "cld" "xor ecx,ecx" "xor ebx,ebx" "xor edi,edi" "lodsb" "cmp al,45" "jne skip2" "mov edi,1" "jmp skip" "lp:" "mov eax,10" "mul ecx" "lea ecx,[eax+ebx]" \
 "skip:" "lodsb" "skip2:" "cmp al,0x39" "ja dne" "cmp al,0x30" "jb dne" "mov bl,al" "sub bl,0x30" "jmp lp" "dne:" "test edi,1" "jz pos" "neg ecx" "pos:" \
 parm [ESI] modify [EAX EBX EDX EDI ESI] value [ecx];

void MarkEBXAsChanged();
#pragma aux MarkEBXAsChanged = "" modify [ebx];

#else

#include "stdlib.h"
#include "string.h"

#define AIL_abs abs
#define AIL_memset memset
#define AIL_memcpy memcpy
#define AIL_strcpy strcpy
#define AIL_strlen strlen
#define AIL_strcat strcat
#define AIL_memcmp memcmp
#define AIL_strcmp strcmp
#define AIL_stricmp stricmp
#define AIL_strnicmp strnicmp
#define AIL_atoi(str) ((U32)atoi(str))
#define MarkEBXAsChanged()

#endif

#define AIL_ptr_add(ptr,off) ((void*)(((U8*)(ptr))+(off)))

#define AIL_ptr_inc_clone_ref(ptr, off) *((void**)ptr)=AIL_ptr_add(*((void**)ptr),off);

#define AIL_ptr_dif(p1,p2) ((SINTa)((SINTa) p1) - ((SINTa) p2))

#define AIL_ptr_alloc_clone(x) ((void*)x)

#define AIL_ptr_free_clone(x)

#define AIL_ptr_fixup_clone(x)

#define AIL_ptr_lt(x,y) (((UINTa)x) < ((UINTa)y))

#define AIL_ptr_ge(x,y) (((UINTa)x) >= ((UINTa)y))

#define AIL_ptr_eq(x,y) (((UINTa)x) == ((UINTa)y))

#define AIL_ptr_ne(x,y) (((UINTa)x) != ((UINTa)y))

#define AIL_ptr_lin_addr(x) ((UINTa)(x))

#else

#if defined(IS_LINUX)

#include <string.h>

#define AIL_abs abs
#define AIL_memset memset
#define AIL_memcpy memcpy
#define AIL_memmove memmove
#define AIL_strcpy strcpy
#define AIL_strlen strlen
#define AIL_strcat strcat
#define AIL_memcmp memcmp
#define AIL_strcmp strcmp
#define AIL_stricmp strcasecmp
#define AIL_strnicmp strncasecmp

#define AIL_ptr_add(ptr,off) ((void*)(((U8*)(ptr))+(off)))

#ifdef __cplusplus
}
  template <typename T> inline T AILptradd( T ptr, U32 off ) { return( T(AIL_ptr_add( ptr, off )) ); }
  #define AIL_ptr_inc_clone(x,y) x=AILptradd(x,y)
extern "C" {
#endif

#define AIL_ptr_dif(p1,p2) ((SINTa)((SINTa) p1) - ((SINTa) p2))

#define AIL_ptr_alloc_clone(x) ((void*)x)

#define AIL_ptr_free_clone(x)

#define AIL_ptr_fixup_clone(x)

#define AIL_ptr_lt(x,y) (((UINTa)x) < ((UINTa)y))

#define AIL_ptr_ge(x,y) (((UINTa)x) >= ((UINTa)y))

#define AIL_ptr_eq(x,y) (((UINTa)x) == ((UINTa)y))

#define AIL_ptr_ne(x,y) (((UINTa)x) != ((UINTa)y))

#define AIL_ptr_lin_addr(x) ((UINTa)(x))

DXDEC S32  AIL_sprintf            (char FAR *dest,
                                   char const FAR *fmt, ...);

#else

#ifdef IS_PS2
#include "string.h"
#else
S32           AIL_stricmp(const char *s1, const char *s2);
S32           AIL_strnicmp( const char *s1,  const char *s2, U32 maxlen);
#endif

#ifdef IS_MAC

////////
//
// Macintosh
//
////////

#include <string.h>

#define AIL_memcpy memcpy

#define AIL_memmove memmove

#define AIL_strcpy strcpy

#define AIL_strcmp strcmp

#define AIL_strlen strlen

#define AIL_strcat strcat

#define AIL_memset memset

#define AIL_ptr_add(ptr,off) ((void*)(((U8*)(ptr))+(off)))

#define AIL_ptr_dif(p1,p2) ((SINTa)((SINTa) p1) - ((SINTa) p2))

#define AIL_ptr_alloc_clone(x) ((void*)x)

#define AIL_ptr_free_clone(x)

#ifdef __cplusplus
}
  template <typename T> inline T AILptradd( T ptr, U32 off ) { return( T(AIL_ptr_add( ptr, off )) ); }
  #define AIL_ptr_inc_clone(x,y) x=AILptradd(x,y)
extern "C" {
#endif

#define AIL_ptr_fixup_clone(x)

#define AIL_ptr_lt(x,y) (((UINTa)x) < ((UINTa)y))

#define AIL_ptr_ge(x,y) (((UINTa)x) >= ((UINTa)y))

#define AIL_ptr_eq(x,y) (((UINTa)x) == ((UINTa)y))

#define AIL_ptr_ne(x,y) (((UINTa)x) != ((UINTa)y))

#define AIL_ptr_lin_addr(x) ((UINTa)(x))

#define AIL_abs abs
#define AIL_memcmp memcmp

DXDEC S32  AIL_sprintf            (char FAR *dest,
                                   char const FAR *fmt, ...);

#else

#ifdef IS_WIN32API

#define AIL_abs abs

#ifdef IS_XENON

typedef void *PVOID;
typedef int                 INT;
typedef  unsigned long ULONG_PTR;
typedef ULONG_PTR SIZE_T;

PVOID
__stdcall
XMemSet(
         PVOID                       pDest,
          INT                         c,
          SIZE_T                      count
    );

PVOID
__stdcall
XMemCpy(
         PVOID                       pDest,
          const void*                 pSrc,
          SIZE_T                      count
    );

char * __cdecl strcpy( char *_Dest,  const char * _Source);

#define AIL_memcpy XMemCpy
#define AIL_memset XMemSet

#else

#ifdef IS_XBOX
  typedef __w64 unsigned int   size_t;
  void *  __cdecl memcpy(void *, const void *, size_t);
  void *  __cdecl memset(void *, int, size_t);
  char *  __cdecl strcpy(char *, const char *);
#elif defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(__NT__) || defined(__WIN32__)
  #include <string.h>
  void *  __cdecl memcpy(void *, const void *, size_t);
  void *  __cdecl memset(void *, int, size_t);
  char *  __cdecl strcpy(char *, const char *);
#endif

#define AIL_memcpy memcpy
#define AIL_memset memset

#endif

#define AIL_memmove memmove

#define AIL_memcmp memcmp

#define AIL_strcpy strcpy

#define AIL_strcmp strcmp

#define AIL_strlen strlen

#define AIL_strcat strcat

#define AIL_ptr_add(ptr,off) ((void*)(((U8*)(ptr))+(off)))

#define AIL_ptr_dif(p1,p2) ((SINTa)((SINTa) p1) - ((SINTa) p2))

#define AIL_ptr_alloc_clone(x) ((void*)x)

#define AIL_ptr_free_clone(x)

#ifdef __cplusplus
}
  template <typename T> inline T AILptradd( T ptr, U32 off ) { return( T(AIL_ptr_add( ptr, off )) ); }
  #define AIL_ptr_inc_clone(x,y) x=AILptradd(x,y)
extern "C" {
#endif

#define AIL_ptr_fixup_clone(x)

#define AIL_ptr_lt(x,y) (((UINTa)x) < ((UINTa)y))

#define AIL_ptr_ge(x,y) (((UINTa)x) >= ((UINTa)y))

#define AIL_ptr_eq(x,y) (((UINTa)x) == ((UINTa)y))

#define AIL_ptr_ne(x,y) (((UINTa)x) != ((UINTa)y))

#define AIL_ptr_lin_addr(x) ((UINTa)(x))

#else

#ifdef IS_PS2

#include "eetypes.h"

typedef MSS_STRUCT _PS2FPUSTATE
{
   U32 regs[32];
   U32 acc;
   U32 ctrl;
}
PS2FPUSTATE;

typedef MSS_STRUCT _PS2VU0STATE
{
   U128 regs[32];
   U128 acc;
   U32  status_flag;
   U32  MAC_flag;
}
PS2VU0STATE;

extern MSS_RESOURCE_POLICY AILCALL AIL_API_set_scratchpad_policy (MSS_RESOURCE_POLICY policy);
extern MSS_RESOURCE_POLICY AILCALL AIL_API_scratchpad_policy     (void);

extern void PS2_noninterleaved_mixer_copy(S32 FAR  *src,
                                          S32       src_bytes,
                                          void FAR *dest,
                                          U32       output_format);

extern void PS2_own_scratchpad(void);
extern void PS2_release_scratchpad(void);

void ASMLINK PS2_save_FPU_state    (PS2FPUSTATE *dest);
void ASMLINK PS2_restore_FPU_state (PS2FPUSTATE *src);

void ASMLINK PS2_save_VU0_state   (PS2VU0STATE *dest);
void ASMLINK PS2_restore_VU0_state(PS2VU0STATE *src);

extern void AIL_memcpy( void * dest, void const * src, U32 bytes );
extern void AIL_memset( void * dest, U8 val, U32 bytes );

#define AIL_memmove memmove

#define AIL_memcmp memcmp

#define AIL_strcpy strcpy

#define AIL_strcmp strcmp

#define AIL_stricmp stricmp

#define AIL_strnicmp strnicmp

#define AIL_strlen strlen

#define AIL_strcat strcat

#define AIL_ptr_add(ptr,off) ((void*)(((U8*)(ptr))+(off)))

#define AIL_ptr_dif(p1,p2) ((SINTa)((SINTa) p1) - ((SINTa) p2))

#define AIL_ptr_alloc_clone(x) ((void*)x)

#define AIL_ptr_free_clone(x)

#ifdef __cplusplus
}
  template <typename T> inline T AILptradd( T ptr, U32 off ) { return( T(AIL_ptr_add( ptr, off )) ); }
  #define AIL_ptr_inc_clone(x,y) x=AILptradd(x,y)
extern "C" {
#endif

#define AIL_ptr_fixup_clone(x)

#define AIL_ptr_lt(x,y) (((UINTa)x) < ((UINTa)y))

#define AIL_ptr_ge(x,y) (((UINTa)x) >= ((UINTa)y))

#define AIL_ptr_eq(x,y) (((UINTa)x) == ((UINTa)y))

#define AIL_ptr_ne(x,y) (((UINTa)x) != ((UINTa)y))

#define AIL_ptr_lin_addr(x) ((UINTa)(x))

static inline S32 AIL_abs(S32 v) { return( ( v >=0 ) ? v : -v ); }                         
                         
#else

#ifdef IS_PS3

#define AIL_memmove memmove

#define AIL_memcpy memcpy

#define AIL_memset memset

#define AIL_memcmp memcmp

#define AIL_strcpy strcpy

#define AIL_strcmp strcmp

#define AIL_stricmp strcasecmp_ascii

#define AIL_strnicmp strncasecmp_ascii

#define AIL_strlen strlen

#define AIL_strcat strcat

#define AIL_ptr_add(ptr,off) ((void*)(((U8*)(ptr))+(off)))

#define AIL_ptr_dif(p1,p2) ((SINTa)((SINTa) p1) - ((SINTa) p2))

#define AIL_ptr_alloc_clone(x) ((void*)x)

#define AIL_ptr_free_clone(x)

#ifdef __cplusplus
}
  template <typename T> inline T AILptradd( T ptr, U32 off ) { return( T(AIL_ptr_add( ptr, off )) ); }
  #define AIL_ptr_inc_clone(x,y) x=AILptradd(x,y)
extern "C" {
#endif

#define AIL_ptr_fixup_clone(x)

#define AIL_ptr_lt(x,y) (((UINTa)x) < ((UINTa)y))

#define AIL_ptr_ge(x,y) (((UINTa)x) >= ((UINTa)y))

#define AIL_ptr_eq(x,y) (((UINTa)x) == ((UINTa)y))

#define AIL_ptr_ne(x,y) (((UINTa)x) != ((UINTa)y))

#define AIL_ptr_lin_addr(x) ((UINTa)(x))

static inline S32 AIL_abs(S32 v) { return( ( v >=0 ) ? v : -v ); }                         

#else

#ifdef IS_WII

#define AIL_memcpy memcpy

#define AIL_memmove memmove

#define AIL_strcpy strcpy

#define AIL_strcmp strcmp

#define AIL_strlen strlen

#define AIL_strcat strcat

#define AIL_memset memset

#define AIL_ptr_add(ptr,off) ((void*)(((U8*)(ptr))+(off)))

#define AIL_ptr_dif(p1,p2) ((SINTa)((SINTa) p1) - ((SINTa) p2))

#define AIL_ptr_alloc_clone(x) ((void*)x)

#define AIL_ptr_free_clone(x)

#ifdef __cplusplus
}
  template <typename T> inline T AILptradd( T ptr, U32 off ) { return( T(AIL_ptr_add( ptr, off )) ); }
  #define AIL_ptr_inc_clone(x,y) x=AILptradd(x,y)
extern "C" {
#endif

#define AIL_ptr_fixup_clone(x)

#define AIL_ptr_lt(x,y) (((UINTa)x) < ((UINTa)y))

#define AIL_ptr_ge(x,y) (((UINTa)x) >= ((UINTa)y))

#define AIL_ptr_eq(x,y) (((UINTa)x) == ((UINTa)y))

#define AIL_ptr_ne(x,y) (((UINTa)x) != ((UINTa)y))

#define AIL_ptr_lin_addr(x) ((UINTa)(x))

#ifndef __cplusplus
int abs( int );
#endif

#define AIL_abs abs
#define AIL_memcmp memcmp

DXDEC S32  AIL_sprintf            (char FAR *dest,
                                   char const FAR *fmt, ...);

#else

//
// Win16
//

#define AIL_memcpy hmemcpy

extern void AIL_memmove    (void FAR *dest,
                            void const FAR *src,
                            S32       cnt);

#define AIL_strcpy lstrcpy

#define AIL_strcmp lstrcmp

#define AIL_strlen lstrlen

#define AIL_strcat lstrcat

extern void AILCALL AIL_memset     (void FAR *dest,
                                   S32       ch,
                                   S32       cnt);


BOOL DPMISetSelectorLimit (UINT selector, DWORD dwLimit);

#define AIL_ptr_add(ptr,off) ((void FAR *) (((U8 _huge *) ptr) + off))

extern S32 AIL_ptr_dif(void const FAR *p1, void const FAR*p2);

extern void FAR *AIL_ptr_alloc_clone(void const FAR *src);

#define AIL_ptr_free_clone(src) (FreeSelector(HIWORD(src)))

extern void AIL_ptr_inc_clone_ref(void FAR * FAR *ptr, U32 offset);
#define AIL_ptr_inc_clone(x,y) AIL_ptr_inc_clone_ref((void FAR * FAR *) &(x),y)

extern void AIL_apply_fixup_clone_ref(void FAR * FAR *src);
#define AIL_ptr_fixup_clone(x) if (LOWORD(x) > 32767) AIL_apply_fixup_clone_ref((void FAR * FAR *) &x)

#define AIL_ptr_lt(x,y) (AIL_ptr_dif(x,y)<0)
#define AIL_ptr_ge(x,y) (AIL_ptr_dif(x,y)>=0)
#define AIL_ptr_eq(x,y) (AIL_ptr_dif(x,y)==0)
#define AIL_ptr_ne(x,y) (AIL_ptr_dif(x,y)!=0)

#define AIL_ptr_lin_addr(x) (GetSelectorBase(HIWORD(x)) + LOWORD(x))

#endif

#endif

#endif

#endif

DXDEC S32 
#if !defined(IS_PS2) && !defined(IS_PS3) && !defined(IS_WII)
_cdecl 
#endif
AIL_sprintf (char FAR *dest, char const FAR *fmt, ...);

#endif

#endif

#endif

#ifndef AIL_ptr_inc_clone
#define AIL_ptr_inc_clone(x,y) AIL_ptr_inc_clone_ref((void FAR * FAR *)(void FAR*) &(x),y)
#endif
#define AIL_ptr_from_clone(clone,hptr) (AIL_ptr_add(hptr,AIL_ptr_dif(clone,hptr)))


#ifdef IS_32

#define Only16Push32s()

#define Only16Pop32s()

#define MSS_do_cb1(type,addr,ds,param1) \
  (addr)(param1)

#define MSS_do_cb3(type,addr,ds,param1,param2,param3) \
  (addr)(param1,param2,param3)

#define MSS_do_cb4(type,addr,ds,param1,param2,param3,param4) \
  (addr)(param1,param2,param3,param4)

#define MSS_do_cb1_with_ret(ret,type,addr,ds,param1) \
  ret = (addr)(param1)

#define MSS_do_cb3_with_ret(ret,type,addr,ds,param1,param2,param3) \
  ret = (addr)(param1,param2,param3)

#define MSS_do_cb4_with_ret(ret,type,addr,ds,param1,param2,param3,param4) \
  ret = (addr)(param1,param2,param3,param4)

#define MSS_do_cb5_with_ret(ret,type,addr,ds,param1,param2,param3,param4,param5) \
  ret = (addr)(param1,param2,param3,param4,param5)

#else

extern void __cdecl FAR Only16Push32s();

extern void __cdecl FAR Only16Pop32s();

extern void FAR MSSGetTaskInfo(S32 FAR* ct, S32 FAR* cds);

extern void __cdecl FAR MSSCall16();

extern void __cdecl FAR SetupMSSCall16(void FAR* addr,U16 ds);

#define MSS_do_cb1(type,addr,ds,param1)                         \
  {                                                             \
    SetupMSSCall16((void FAR*)type addr,(U16)(ds));             \
    (type &MSSCall16)(param1);                                  \
  }

#define MSS_do_cb3(type,addr,ds,param1,param2,param3)           \
  {                                                             \
    SetupMSSCall16((void FAR*)type addr,(U16)(ds));             \
    (type &MSSCall16)(param1,param2,param3);                    \
  }

#define MSS_do_cb4(type,addr,ds,param1,param2,param3,param4)    \
  {                                                             \
    SetupMSSCall16((void FAR*)type addr,(U16)(ds));             \
    (type &MSSCall16)(param1,param2,param3,param4);             \
  }

#define MSS_do_cb1_with_ret(ret,type,addr,ds,param1)  \
  {                                                             \
    SetupMSSCall16((void FAR*)type addr,(U16)(ds));             \
    ret=(type &MSSCall16)(param1);                              \
  }

#define MSS_do_cb3_with_ret(ret,type,addr,ds,param1,param2,param3) \
  {                                                                          \
    SetupMSSCall16((void FAR*)type addr,(U16)(ds));                          \
    ret=(type &MSSCall16)(param1,param2,param3);                             \
  }

#define MSS_do_cb4_with_ret(ret,type,addr,ds,param1,param2,param3,param4) \
  {                                                                                        \
    SetupMSSCall16((void FAR*)type addr,(U16)(ds));                                        \
    ret=(type &MSSCall16)(param1,param2,param3,param4);                                    \
  }

#define MSS_do_cb5_with_ret(ret,type,addr,ds,param1,param2,param3,param4,param5) \
  {                                                                                        \
    SetupMSSCall16((void FAR*)type addr,(U16)(ds));                                        \
    ret=(type &MSSCall16)(param1,param2,param3,param4,param5);                             \
  }

#endif

#ifdef IS_XENON

#define  XWAVE_FORMAT_XMA 0x0165

typedef struct XXMASTREAMFORMAT
{
    U32   PsuedoBytesPerSec;  // Used by encoder
    U32   SampleRate;         // Sample rate for the stream.
    U32   LoopStart;          // Loop start offset (in bits).
    U32   LoopEnd;            // Loop end offset (in bits).

    // Format for SubframeData: eeee ssss.
    // e: Subframe number of loop end point [0,3].
    // s: Number of subframes to skip before decoding and outputting at the loop start point [1,4].

    U8    SubframeData;       // Data for decoding subframes.  See above.
    U8    Channels;           // Number of channels in the stream (1 or 2).
    U16   ChannelMask;        // Channel assignments for the channels in the stream (same as
                                // lower 16 bits of dwChannelMask in WAVEFORMATEXTENSIBLE).
} XXMASTREAMFORMAT;

typedef struct XXMAWAVEFORMAT
{
    U16            FormatTag;     // Audio format type (always WAVE_FORMAT_XMA).
    U16            BitsPerSample; // Bit depth (currently required to be 16).
    U16            EncodeOptions; // Options for XMA encoder/decoder.
    U16            LargestSkip;   // Largest skip used in interleaving streams.
    U16            NumStreams;    // Number of interleaved audio streams.
    U8             LoopCount;     // Number of loop repetitions (255 == infinite).
    U8             Version;       // Version of the encoder that generated this.
    XXMASTREAMFORMAT XmaStreams[1]; // Format info for each stream (can grow based on wNumStreams).
} XXMAWAVEFORMAT;

#endif

extern U32 SS_granularity(U32 format);

extern S32 MC_open_output_filter(C8        *name, //)
                                 HDIGDRIVER driver,
                                 S32        is_matrix_filter);

//
// Prototypes for ADPCM decode routines
//

extern void ASMLINK DecodeADPCM_STEREO( ASMPARM void FAR       *out, 
                                 ASMPARM void const FAR *in,
                                 ASMPARM S32             out_len, 
                                 ASMPARM S32             in_len,
                                 ASMPARM ADPCMDATA FAR  *adpcm_data);

extern void ASMLINK DecodeADPCM_MONO( ASMPARM void FAR       *out, 
                               ASMPARM void const FAR *in,
                               ASMPARM S32             out_len, 
                               ASMPARM S32             in_len,
                               ASMPARM ADPCMDATA FAR  *adpcm_data);

extern void ASMLINK DecodeADPCM_MONO_8( ASMPARM void FAR       *out, 
                                 ASMPARM void const FAR *in,
                                 ASMPARM S32             out_len, 
                                 ASMPARM S32             in_len,
                                 ASMPARM ADPCMDATA FAR  *adpcm_data);

#ifdef IS_DOS

extern void cdecl AILSSA_DMA_copy(U32 half_buff,
                                  void FAR *DMA,
                                  U32 hw_mode_flags,
                                  U32 hw_format,
                                  U32 n_active_samples,
                                  void FAR *build_buffer,
                                  U32 samples_per_buffer,
                                  U32 buffer_size);

#endif

/*
#ifdef IS_PPC

void ASMLINK DecodeADPCM_MONO( register void FAR*out, register void const FAR *in,
                               register S32 out_len, register S32 in_len,
                               register ADPCMDATA FAR*adpcm_data);

void ASMLINK DecodeADPCM_MONO_8( register void FAR*out, register void const FAR *in,
                                  register S32 out_len, register S32 in_len,
                                  register ADPCMDATA FAR*adpcm_data);

void stereo_mc_merge( void * src, void * dest, S32 cnt, S32 m0, S32 m1 );

#else

#endif
*/

#ifndef IS_PS2
#pragma pack(1)
#endif

typedef MSS_STRUCT _ADPCMOUT {
  U32 riffmark;
  U32 rifflen;
  U32 wavemark;
  U32 fmtmark;
  U32 fmtlen;
  U16 fmttag;
  U16 channels;
  U32 sampersec;
  U32 avepersec;
  U16 blockalign;
  U16 bitspersam;
  S16 extra;
  S16 samples_per_block;
  U32 factmark;
  U32 factlen;
  U32 samples;
  U32 datamark;
  U32 datalen;
} ADPCMOUT;

typedef MSS_STRUCT _WAVEOUT {
  U32 riffmark;
  U32 rifflen;
  U32 wavemark;
  U32 fmtmark;
  U32 fmtlen;
  U16 fmttag;
  U16 channels;
  U32 sampersec;
  U32 avepersec;
  U16 blockalign;
  U16 bitspersam;
  U32 datamark;
  U32 datalen;
} WAVEOUT;


#ifndef IS_PS2
#pragma pack()
#endif

#ifdef __cplusplus
}
#endif

#endif

