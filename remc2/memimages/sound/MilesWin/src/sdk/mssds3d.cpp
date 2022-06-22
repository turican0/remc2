//############################################################################
//##                                                                        ##
//##  Miles Sound System                                                    ##
//##                                                                        ##
//##  MSSDS3D.CPP: DirectSound3D voice output filter                        ##
//##                                                                        ##
//##  32-bit protected-mode source compatible with MSC 11.0/Watcom 10.6     ##
//##                                                                        ##
//##  Version 1.00 of 15-Apr-05: Initial, derived from DS3DBASE.I           ##
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

#ifdef EAX3
#define INITGUID
#endif

#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <process.h>

#include <dsound.h>

#define MAX_VOICES       96                  // Hardware: limit to 96 voices, or max supported, whichever is lower 
#define HW_LISTENER_POS  FALSE               // Apply listener translation to samples if FALSE; pass it to the hardware if TRUE

//
// Buffer characteristics determine compromise between dropout immunity, control latency, and 
// control granularity
//

#define BUFF_FRAG_BYTES  2048                // Approx. 23 ms control granularity for 16-bit 44 kHz mono
#define TOTAL_FRAGS      8                   // Total buffer size = 16K
#define FILL_AHEAD_FRAGS 3                   // Fill ~70 ms ahead (for 16-bit 44 kHz mono)
#define FRAG_SERVICE_HZ  200                 // Poll buffers at 5 ms rate

//
// Epsilon value used for FP comparisons with 0
//

#define EPSILON 0.0001F

//
// Provider names
//

#ifdef EAX4
  #define PROVIDER_NAME_STR "Creative Labs EAX 4 (TM)"
  #define PROVIDER_FLAGS_VAL (FPROV_VOICE + FPROV_3D + FPROV_OCCLUSION + FPROV_EAX)
#else
#ifdef EAX3
  #define PROVIDER_NAME_STR "Creative Labs EAX 3 (TM)"
  #define PROVIDER_FLAGS_VAL (FPROV_VOICE + FPROV_3D + FPROV_OCCLUSION + FPROV_EAX)
#else
#ifdef EAX2
  #define PROVIDER_NAME_STR "Creative Labs EAX 2 (TM)"
  #define PROVIDER_FLAGS_VAL (FPROV_VOICE + FPROV_3D + FPROV_OCCLUSION + FPROV_EAX)
#else
#ifdef EAX3D
  #define PROVIDER_NAME_STR "Creative Labs EAX (TM)"
  #define PROVIDER_FLAGS_VAL (FPROV_VOICE + FPROV_3D + FPROV_EAX)
#else
  #define PROVIDER_NAME_STR "DirectSound3D Hardware Support"
  #define PROVIDER_FLAGS_VAL (FPROV_VOICE + FPROV_3D)
#endif
#endif
#endif
#endif

//
// Misc. properties
//

enum SAMPLE_STATE
{
   SS_UNALLOCATED   = -3,
   SS_READY_TO_PLAY = -2,
   SS_PLAYING       = -1,

   //
   // Values from [0,TOTAL_FRAGS) indicate fragment # at which a ->Stop() call should be issued
   // and the sample marked SS_READY_TO_PLAY
   //
};

#ifdef EAX3D

#ifdef EAX3

#include "eax3.h"

#ifdef EAX4
#include "eax4.h"
#endif

DEFINE_GUID(DSPROPSETID_EAX20_ListenerProperties,
    0x306a6a8,
    0xb224,
    0x11d2,
    0x99, 0xe5, 0x0, 0x0, 0xe8, 0xd8, 0xc7, 0x22);

DEFINE_GUID(DSPROPSETID_EAX_ReverbProperties,
    0x4a4e6fc1,
    0xc341,
    0x11d1,
    0xb7, 0x3a, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00);

DEFINE_GUID(DSPROPSETID_EAXBUFFER_ReverbProperties,
    0x4a4e6fc0,
    0xc341,
    0x11d1,
    0xb7, 0x3a, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00);

DEFINE_GUID(DSPROPSETID_EAX20_BufferProperties,
    0x306a6a7,
    0xb224,
    0x11d2,
    0x99, 0xe5, 0x0, 0x0, 0xe8, 0xd8, 0xc7, 0x22);

#define EAXPROP DSPROPSETID_EAX_ListenerProperties
#define EAXBUFPROP DSPROPSETID_EAX_BufferProperties

#elif EAX2

#include "eax2.h"
#define EAXPROP DSPROPSETID_EAX_ListenerProperties
#define EAXBUFPROP DSPROPSETID_EAX_BufferProperties

#else

#include "eax.h"
#define EAXPROP DSPROPSETID_EAX_ReverbProperties
#define EAXBUFPROP DSPROPSETID_EAXBUFFER_ReverbProperties

#endif

#define PSET_SETGET (KSPROPERTY_SUPPORT_GET | KSPROPERTY_SUPPORT_SET)
#define CREVERB_INVALID_VALUE -1.0E+30F
#define CREVERBBUFFER_SETGET (KSPROPERTY_SUPPORT_GET|KSPROPERTY_SUPPORT_SET)
#define CREVERBBUFFER_INVALID_VALUE -1.0E+30F

#endif

#include "mss.h"
#include "imssapi.h"

#include <objbase.h>

#ifdef _DEBUG
  #ifdef IS_WIN64
    #define assert( cond ) if (!(cond)) *((int*)0)=0;
  #else
    #define assert( cond ) if (!(cond)) __asm { int 3 };
  #endif
#else
  #define assert( cond )
#endif

#define DSBCAPS_MUTE3DATMAXDISTANCE 0x00020000

//
// Property tokens
//

enum PROPERTY
{
   //
   // Provider attribs
   //

   PROVIDER_FLAGS,

   //
   // Driver attribs
   //

   MAX_SUPPORTED_SAMPLES,
   MUTE_AT_MAX,
   MINIMUM_SAMPLE_BUFFER_SIZE,

#ifdef EAX3D
   EAX_ENVIRONMENT,
   EAX_EFFECT_VOLUME,
   EAX_DECAY_TIME,
   EAX_DAMPING,
   EAX_ALL_PARAMETERS,
   EAX_PROPERTY_SET,

#ifdef EAX2
   EAX_ENVIRONMENT_SIZE,
   EAX_ENVIRONMENT_DIFFUSION,
   EAX_ROOM,
   EAX_ROOM_HF,
   EAX_DECAY_HF_RATIO,
   EAX_REFLECTIONS,
   EAX_REFLECTIONS_DELAY,
   EAX_REVERB_P,
   EAX_REVERB_DELAY,
   EAX_ROOM_ROLLOFF,
   EAX_AIR_ABSORPTION,
   EAX_FLAGS,

#ifdef EAX3
   EAX_ROOM_LF,
   EAX_DECAY_LF_RATIO,
   EAX_REFLECTIONS_PAN,
   EAX_REVERB_PAN,
   EAX_ECHO_TIME,
   EAX_ECHO_DEPTH,
   EAX_MODULATION_TIME,
   EAX_MODULATION_DEPTH,
   EAX_HF_REFERENCE,
   EAX_LF_REFERENCE,
#endif

#ifdef EAX4
  EAX_SLOT_0,
  EAX_SLOT_1,
  EAX_SLOT_2,
  EAX_SLOT_3,
#endif

#endif
#endif

   //
   // Voice attribs for "Voice filter sample services"
   //

#ifdef EAX3D
   EAX_EFFECT_ALL_PARAMETERS,
   EAX_SAMPLE_PROPERTY_SET,

#ifdef EAX2
   EAX_SAMPLE_DIRECT,
   EAX_SAMPLE_DIRECT_HF,
   EAX_SAMPLE_ROOM,
   EAX_SAMPLE_ROOM_HF,
   EAX_SAMPLE_OBSTRUCTION,
   EAX_SAMPLE_OBSTRUCTION_LF_RATIO,
   EAX_SAMPLE_OCCLUSION,
   EAX_SAMPLE_OCCLUSION_LF_RATIO,
   EAX_SAMPLE_OCCLUSION_ROOM_RATIO,
   EAX_SAMPLE_ROOM_ROLLOFF,
   EAX_SAMPLE_AIR_ABSORPTION,
   EAX_SAMPLE_OUTSIDE_VOLUME_HF,
   EAX_SAMPLE_FLAGS,

#ifdef EAX3
   EAX_SAMPLE_OCCLUSION_DIRECT_RATIO,
   EAX_SAMPLE_EXCLUSION,
   EAX_SAMPLE_EXCLUSION_LF_RATIO,
   EAX_SAMPLE_DOPPLER,
#endif

#ifdef EAX4
  EAX_SAMPLE_SLOT_VOLUMES_P,
#endif

#endif

#endif

   DIRECTSOUND_SAMPLE_BUFFER, 
};

//
// Per-sample filter state descriptor
// One state descriptor is associated with each HSAMPLE
//

struct DSVOICE
{
   //
   // Driver binding
   //

   struct DRIVERSTATE FAR *DRV;        // Owned by this driver

   //
   // HSAMPLE binding (which can change)
   //

   HSAMPLE S;                          // Assigned to this HSAMPLE

   SAMPLE_STATE sample_state;      

   //
   // Secondary buffer state
   //

   volatile S32 lock;                  // no interrupt servicing please

   LPDIRECTSOUNDBUFFER   lpdsb;        // Base DirectSound buffer
   LPDIRECTSOUND3DBUFFER lpds3db;      // Extended DirectSound 3D buffer

#ifdef EAX3D
   LPKSPROPERTYSET lpPropertySet;      // Property set interface for secondary buffer
#endif

   U32 previous_position;              // Previous play cursor position
   U32 previous_write_position;        // Previous write cursor position
   U32 head_frag;                      // Next fragment # to be filled with data
   U32 last_move_time;                 // Last time that the clock moved

   //
   // Source property shadow values
   //
   // In most cases, these are the last values passed to the underlying provider.  In some cases,
   // the values passed to the provider are derived from multiple sources -- e.g., 
   // hardware source_room = wet*exclusion for EAX2 -- so these variables are used only to 
   // monitor the HDIGDRIVER structure for changes that require recalculation of the derived values
   //

   MSSVECTOR3D shadow_position;         
   MSSVECTOR3D shadow_face;             
   MSSVECTOR3D shadow_velocity;         
   S32         shadow_playback_rate;
   F32         shadow_volume;
   F32         shadow_occlusion;
   F32         shadow_obstruction;
   F32         shadow_exclusion;
   F32         shadow_min_dist;
   F32         shadow_max_dist;
   F32         shadow_inner_angle;
   F32         shadow_outer_angle;
   F32         shadow_outer_volume;
   F32         shadow_wet_level;
   F32         shadow_dry_level;
   F32         shadow_master_dry;      // Master wet is a room-level property in EAX, but changes to
   F32         shadow_master_vol;      // master dry/vol need to be applied to all voices for compatibility
   S32         shadow_auto_3D_atten;
   S32         shadow_is_3D;
};

//
// Driver state descriptor
//
// One state descriptor is associated with each HDIGDRIVER
//

struct DRIVERSTATE
{
   //
   // Members common to all voice filter providers
   //

   HDIGDRIVER   dig;                  // Driver with which this descriptor is associated
   SS_STREAM_CB stream_callback;      // Function to call to acquire source data for this driver's voices

   //
   // Members associated with specific filter provider
   //

   LPDIRECTSOUND           lpDS;
   LPDIRECTSOUNDBUFFER     lpDSPRIM;
   LPDIRECTSOUND3DLISTENER lp3DLISTENER;
   DSCAPS                  DSCaps;

   DSVOICE *voices;
   S32      n_voices;

   HTIMER   buffer_timer;

   volatile S32 lock;               // no interrupt servicing please

   //
   // EAX values
   //

#ifdef EAX3D
   LPDIRECTSOUNDBUFFER   lpSecondaryBuffer; // Secondary buffer
   LPDIRECTSOUND3DBUFFER lpDs3dBuffer;      // 3D interface for secondary buffer
   LPKSPROPERTYSET       lpPropertySet;     // Property set interface for secondary buffer
#endif

#ifdef EAX4
   S32 slotvolumes[4];
   S32 sloteffects[4];
#endif

   //
   // Listener property shadow values
   // These are the last values passed to the underlying provider
   //

   MSSVECTOR3D shadow_listen_position;
   MSSVECTOR3D shadow_listen_face;
   MSSVECTOR3D shadow_listen_up;
   MSSVECTOR3D shadow_listen_velocity;
   F32         shadow_listen_doppler_factor;
   F32         shadow_listen_distance_factor;
   F32         shadow_listen_rolloff_factor;

   S32         shadow_room_type;
   F32         shadow_master_wet;     
   F32         shadow_reverb_decay_time_s;    
   F32         shadow_reverb_damping; 
   F32         shadow_reverb_predelay_s;
};

//
// Statics/globals
//

static S32 FLT_started = 0;

static C8 FLT_error_text[256];

static void API_lock(void)
{
   if (AIL_get_preference(AIL_LOCK_PROTECTION))
      AIL_lock();
}

static void API_unlock(void)
{
   if (AIL_get_preference(AIL_LOCK_PROTECTION))
      AIL_unlock();
}

static S32 incnoints(volatile S32 FAR *noi)
{
#ifdef IS_WIN64
   return InterlockedIncrement((LONG *) noi) - 1;  
#else
   __asm
   {
     mov edx,[noi]
     mov eax,1
     lock xadd [edx],eax
   }
#endif
}

static void decnoints(volatile S32 FAR *noi)
{
#ifdef IS_WIN64
   InterlockedDecrement((LONG *) noi);
#else
   __asm
   {
     mov eax,[noi]
     lock dec dword ptr [eax]
   }
#endif
}

static float EAX_GetPredelay(DRIVERSTATE FAR *DRV);
static float EAX_GetMasterWet(DRIVERSTATE FAR *DRV);
static float EAX_GetDamping(DRIVERSTATE FAR *DRV);
static float EAX_GetDecayTime(DRIVERSTATE FAR *DRV);
static void  EAX_SetPredelay(DRIVERSTATE FAR *DRV, F32, S32);
static void  EAX_SetMasterWet(DRIVERSTATE FAR *DRV, F32, S32);
static void  EAX_SetDamping(DRIVERSTATE FAR *DRV, F32, S32);
static void  EAX_SetDecayTime(DRIVERSTATE FAR *DRV, F32, S32);

//############################################################################
//#                                                                          #
//# Critical-section guards                                                  #
//#                                                                          #
//############################################################################

static void CS_enter(volatile S32 FAR *CS)
{
#if 1
   for(;;)
      {   
      U32 v = incnoints(CS);

      if (v == 0)
         {
         break;
         }

      decnoints(CS);
      AIL_delay(0);
      } 
#else
   incnoints(CS);

   AIL_unlock_mutex();

   while (CS != 1)
      {
      Sleep(1);
      }

   AIL_lock_mutex();
#endif
}

static void CS_leave(volatile S32 FAR *CS)
{
  decnoints(CS);
}

//############################################################################
//#                                                                          #
//# Vector math support                                                      #
//#                                                                          #
//############################################################################

#define DEGS_TO_DIAMS(degs) ((degs) / 114.591559F)
#define DIAMS_TO_DEGS(degs) ((degs) * 114.591559F)

static void RAD_vector_invalidate(MSSVECTOR3D *v)
{
   v->x = FLT_MAX;
   v->y = FLT_MAX;
   v->z = FLT_MAX;
}

static S32 __inline RAD_vectors_equal(const MSSVECTOR3D *v1, 
                                      const MSSVECTOR3D *v2)
{
   F32 dx = (v1->x - v2->x); if ((dx < -EPSILON) || (dx > EPSILON)) return 0;
   F32 dy = (v1->y - v2->y); if ((dy < -EPSILON) || (dy > EPSILON)) return 0; 
   F32 dz = (v1->z - v2->z); if ((dz < -EPSILON) || (dz > EPSILON)) return 0; 

   return 1;
}

//############################################################################
//##                                                                        ##
//## Convert linear loudness level to 20log10 (dB) level for volume and pan ##
//## functions                                                              ##
//##                                                                        ##
//## Returns negative dB * 100                                              ##
//##                                                                        ##
//############################################################################

static F32 linear_to_dB(F32 linear_level)
{
  //
  // Ensure extreme values return max/min results
  //

  if (linear_level <= 0.001f)
    return -10000.0F;

  if (linear_level >= (1.0f-EPSILON))
    return 0.0F;

  return( (F32) ( 2000.0f * log10(linear_level) ) );
}

#ifdef EAX3D

#ifdef EAX2

static F32 dB_to_linear(F32 dB)
{
  if ((dB+EPSILON) > 0.0F)
    return 1.0F;
  if ((dB-EPSILON) < -10000.0F)
    return 0.0F;

  return (F32) pow( 10.0f, ( dB / 2000.F ) );
}

static void EAX2_Set_int(DRIVERSTATE FAR                *DRV, //)
                         DSPROPERTY_EAX_LISTENERPROPERTY prop,
                         U32                             val)
{
   API_lock();

   DRV->lpPropertySet->Set(EAXPROP,
                           prop,
                           NULL,
                           0,
                          &val,
                           sizeof(U32));
   API_unlock();
}

static void EAX2_Set_float(DRIVERSTATE FAR                *DRV, //)
                           DSPROPERTY_EAX_LISTENERPROPERTY prop,
                           F32                             val)
{
   API_lock();

   DRV->lpPropertySet->Set( EAXPROP,
                            prop,
                            NULL,
                            0,
                           &val,
                            sizeof(F32));
   API_unlock();
}

static U32 EAX2_Get_int(DRIVERSTATE FAR                *DRV, //)
                        DSPROPERTY_EAX_LISTENERPROPERTY prop)
{
   U32 val=0xffffffff;
   unsigned long r;

   API_lock();

   DRV->lpPropertySet->Get(EAXPROP,
                           prop,
                           NULL,
                           0,
                          &val,
                          sizeof(U32),
                          &r);

   API_unlock();

   return val;
}

static F32 EAX2_Get_float(DRIVERSTATE FAR                *DRV, //)
                          DSPROPERTY_EAX_LISTENERPROPERTY prop)
{
   F32 val=-1.0;
   unsigned long r;

   API_lock();

   DRV->lpPropertySet->Get(EAXPROP,
                           prop,
                           NULL,
                           0,
                          &val,
                           sizeof(F32),
                          &r);
   API_unlock();

   return val;
}


static void EAX2_Sample_Set_int(DSVOICE FAR *V, //)
                                U32          prop,       // to allow OR'ed DSPROPERTY_EAX_LISTENERPROPERTY flags
                                U32          val)
{
   API_lock();

   V->lpPropertySet->Set(EAXBUFPROP,
                         prop,
                         NULL,
                         0,
                        &val,
                         sizeof(U32));

   API_unlock();
}

static void EAX2_Sample_Set_float(DSVOICE FAR                  *V, //)
                                  DSPROPERTY_EAX_BUFFERPROPERTY prop,
                                  F32                           val)
{
   API_lock();

   V->lpPropertySet->Set(EAXBUFPROP,
                         prop,
                         NULL,
                         0,
                        &val,
                         sizeof(F32));

   API_unlock();
}

static U32 EAX2_Sample_Get_int(DSVOICE FAR                  *V, //)
                               DSPROPERTY_EAX_BUFFERPROPERTY prop)
{
   U32 val=0xffffffff;
   unsigned long r;

   API_lock();

   V->lpPropertySet->Get(EAXBUFPROP,
                         prop,
                         NULL,
                         0,
                        &val,
                         sizeof(U32),
                         &r);
   API_unlock();

   return val;
}

static F32 EAX2_Sample_Get_float(DSVOICE FAR                  *V, //)
                                 DSPROPERTY_EAX_BUFFERPROPERTY prop)
{
   F32 val=-1.0f;
   unsigned long r;

   API_lock();

   V->lpPropertySet->Get(EAXBUFPROP,
                         prop,
                         NULL,
                         0,
                        &val,
                         sizeof(F32),
                         &r);

   API_unlock();

   return val;
}

#endif


//----------------------------------------------------------
// void SetEnvironment(unsigned long envId)
//
// DESCRIPTION: Selects a new environment, from the list
//               in EAX.H.  e.g. EAX_ENVIRONMENT_CAVE
//
// PARAMETERS: Environment ID
//
// RETURNS: no return value.
//
//----------------------------------------------------------

static void EAX_SetEnvironment(DRIVERSTATE FAR *DRV, //)
                               unsigned long    envId,
                               S32              take_mutex = TRUE)
{
   if (take_mutex) CS_enter(&DRV->lock);
   API_lock();

   DRV->shadow_room_type = envId;

   DRV->lpPropertySet->Set(EAXPROP,
#ifdef EAX2
                           DSPROPERTY_EAXLISTENER_ENVIRONMENT,
#else
                           DSPROPERTY_EAX_ENVIRONMENT,
#endif
                           NULL,
                           0,
                          &envId,
                           sizeof(unsigned long));

   //
   // Read reverb params and store them in the driver structure 
   // to emulate the core software provider's behavior
   //
   // No other EAX_Set() calls should write to DRV->dig directly!
   // 

   DRV->dig->master_wet          = DRV->shadow_master_wet          = EAX_GetMasterWet(DRV);
   DRV->dig->reverb_decay_time_s = DRV->shadow_reverb_decay_time_s = EAX_GetDecayTime(DRV);
   DRV->dig->reverb_damping      = DRV->shadow_reverb_damping      = EAX_GetDamping(DRV);
   DRV->dig->reverb_predelay_s   = DRV->shadow_reverb_predelay_s   = EAX_GetPredelay(DRV);

   API_unlock();
   if (take_mutex) CS_leave(&DRV->lock);
}

//----------------------------------------------------------
// void SetMasterWet(float volume)
//
// DESCRIPTION: Changes the reverb volume
//
// PARAMETERS: volume value
//
// RETURNS: no return value.
//
//----------------------------------------------------------

static void EAX_SetMasterWet(DRIVERSTATE FAR *DRV, //)
                             F32              volume,
                             S32              take_mutex = TRUE)
{
   if (take_mutex) CS_enter(&DRV->lock);
   API_lock();

   DRV->shadow_master_wet = volume;

#ifdef EAX2
   LONG vol=(LONG)linear_to_dB(volume);

   DRV->lpPropertySet->Set(EAXPROP,
                           DSPROPERTY_EAXLISTENER_ROOM,
                           NULL,
                           0,
                          &vol,
                           sizeof(LONG));

   DRV->lpPropertySet->Set(EAXPROP,
                           DSPROPERTY_EAXLISTENER_ROOMHF,
                           NULL,
                           0,
                          &vol,
                           sizeof(LONG));
#else
   DRV->lpPropertySet->Set(EAXPROP,
                           DSPROPERTY_EAX_VOLUME,
                           NULL,
                           0,
                          &volume,
                           sizeof(float));
#endif

   API_unlock();
   if (take_mutex) CS_leave(&DRV->lock);
}

//----------------------------------------------------------
// void SetDecayTime(float time)
//
// DESCRIPTION: Changes the reverb decay time
//
// PARAMETERS: decay time value
//
// RETURNS: no return value.
//
//----------------------------------------------------------

static void EAX_SetDecayTime(DRIVERSTATE FAR *DRV, //)
                             F32              time,
                             S32              take_mutex = TRUE)
{
   if (take_mutex) CS_enter(&DRV->lock);
   API_lock();

   DRV->shadow_reverb_decay_time_s = time;

   DRV->lpPropertySet->Set(EAXPROP,
#ifdef EAX2
                           DSPROPERTY_EAXLISTENER_DECAYTIME,
#else
                           DSPROPERTY_EAX_DECAYTIME,
#endif
                           NULL,
                           0,
                          &time,
                           sizeof(float));

   API_unlock();
   if (take_mutex) CS_leave(&DRV->lock);
}

//----------------------------------------------------------
// void SetDamping(float damping)
//
// DESCRIPTION: Changes the reverb damping
//
// PARAMETERS: damping value
//
// RETURNS: no return value.
//
//----------------------------------------------------------

static void EAX_SetDamping(DRIVERSTATE FAR *DRV, //)
                           F32              damping,
                           S32              take_mutex = TRUE)
{
   if (take_mutex) CS_enter(&DRV->lock);
   API_lock();

   DRV->shadow_reverb_damping = damping;

   DRV->lpPropertySet->Set(EAXPROP,
#ifdef EAX2
                           DSPROPERTY_EAXLISTENER_DECAYHFRATIO,
#else
                           DSPROPERTY_EAX_DAMPING,
#endif
                           NULL,
                           0,
                          &damping,
                           sizeof(float));

   API_unlock();
   if (take_mutex) CS_leave(&DRV->lock);
}

//----------------------------------------------------------
// void SetPredelay(float damping)
//
// DESCRIPTION: Changes the reverb predelay
//
// PARAMETERS: damping value
//
// RETURNS: no return value.
//
//----------------------------------------------------------

static void EAX_SetPredelay(DRIVERSTATE FAR *DRV, //)
                            float            predelay,
                            S32              take_mutex)
{
#ifdef EAX2
   if (take_mutex) CS_enter(&DRV->lock);
   API_lock();

   DRV->shadow_reverb_predelay_s = predelay;

   DRV->lpPropertySet->Set(EAXPROP,
                           DSPROPERTY_EAXLISTENER_REVERBDELAY,
                           NULL,
                           0,
                          &predelay,
                           sizeof(float));

   API_unlock();
   if (take_mutex) CS_leave(&DRV->lock);
#else
   DRV->shadow_reverb_predelay_s = 0.0F;
#endif
}

//----------------------------------------------------------
// unsigned long GetEnvironment()
//
// DESCRIPTION: Gets the current reverb environment
//
// PARAMETERS: none
//
// RETURNS: Environment ID
//
//----------------------------------------------------------

static U32 EAX_GetEnvironment(DRIVERSTATE FAR *DRV)
{
   unsigned long envId = EAX_ENVIRONMENT_GENERIC;
   unsigned long r;

   API_lock();

   DRV->lpPropertySet->Get(EAXPROP,
#ifdef EAX2
                           DSPROPERTY_EAXLISTENER_ENVIRONMENT,
#else
                           DSPROPERTY_EAX_ENVIRONMENT,
#endif
                           NULL,
                           0,
                          &envId,
                           sizeof(unsigned long),
                          &r);
   API_unlock();

   return envId;
}

//----------------------------------------------------------
// float GetMasterWet()
//
// DESCRIPTION: Gets the current listener reverb volume
//
// PARAMETERS: none
//
// RETURNS: volume value
//
//----------------------------------------------------------

static F32 EAX_GetMasterWet(DRIVERSTATE FAR *DRV)
{
   F32 wet = CREVERB_INVALID_VALUE;
   unsigned long r;

   API_lock();

#ifdef EAX2
   LONG room=20000;

   DRV->lpPropertySet->Get(EAXPROP,
                           DSPROPERTY_EAXLISTENER_ROOM,
                           NULL,
                           0,
                          &room,
                           sizeof(LONG),
                          &r);

   if (room != 20000)
      {
      wet=dB_to_linear((F32) room);
      }
#else
   DRV->lpPropertySet->Get(EAXPROP,
                           DSPROPERTY_EAX_VOLUME,
                           NULL,
                           0,
                          &wet,
                           sizeof(float),
                          &r);
#endif

   API_unlock();

   return wet;
}

//----------------------------------------------------------
// float GetDecayTime()
//
// DESCRIPTION: Gets the current reverb decay time
//
// PARAMETERS: none
//
// RETURNS: decay time value
//
//----------------------------------------------------------

static F32 EAX_GetDecayTime(DRIVERSTATE FAR *DRV)
{
   float time = CREVERB_INVALID_VALUE;
   unsigned long r;

   API_lock();

   DRV->lpPropertySet->Get(EAXPROP,
#ifdef EAX2
                           DSPROPERTY_EAXLISTENER_DECAYTIME,
#else
                           DSPROPERTY_EAX_DECAYTIME,
#endif
                           NULL,
                           0,
                          &time,
                           sizeof(float),
                          &r);
   API_unlock();

   return time;
}

//----------------------------------------------------------
// float GetDamping()
//
// DESCRIPTION: Gets the current reverb damping
//
// PARAMETERS: none
//
// RETURNS: damping value
//
//----------------------------------------------------------

static F32 EAX_GetDamping(DRIVERSTATE FAR *DRV)
{
   float damping = CREVERB_INVALID_VALUE;
   unsigned long r;

   API_lock();

   DRV->lpPropertySet->Get(EAXPROP,
#ifdef EAX2
                           DSPROPERTY_EAXLISTENER_DECAYHFRATIO,
#else
                           DSPROPERTY_EAX_DAMPING,
#endif
                           NULL,
                           0,
                          &damping,
                           sizeof(float),
                          &r);
   API_unlock();

   return damping;
}

//----------------------------------------------------------
// float GetPredelay()
//
// DESCRIPTION: Gets the current reverb predelay
//
// PARAMETERS: none
//
// RETURNS: damping value
//
//----------------------------------------------------------

static F32 EAX_GetPredelay(DRIVERSTATE FAR *DRV)
{
   float predelay = 0.0F;

#ifdef EAX2

   unsigned long r;
   API_lock();

   DRV->lpPropertySet->Get(EAXPROP,
                           DSPROPERTY_EAXLISTENER_REVERBDELAY,
                           NULL,
                           0,
                          &predelay,
                           sizeof(float),
                          &r);
   API_unlock();
#endif

   return predelay;
}


//----------------------------------------------------------
// float SetAll()
//
// DESCRIPTION: Sets all parameters of the current reverb
//
// PARAMETERS: pointer to structure
//
// RETURNS: none
//
//----------------------------------------------------------

static void EAX_SetAll(DRIVERSTATE FAR *DRV, //)
                       void const FAR  *allparameters,
                       S32              take_mutex = TRUE)
{
   if (take_mutex) CS_enter(&DRV->lock);
   API_lock();

   //
   // TODO: Doc that this function does NOT update the shadow registers 
   // and driver reverb vars.  If an application ever sets this preference, it must 
   // continue to do so!
   //

#ifdef EAX2
   DRV->lpPropertySet->Set(EAXPROP,
                           DSPROPERTY_EAXLISTENER_ALLPARAMETERS,
                           NULL,
                           0,
                           (void FAR*)allparameters,
                           sizeof(EAXLISTENERPROPERTIES));
#else
   DRV->lpPropertySet->Set(EAXPROP,
                           DSPROPERTY_EAX_ALL,
                           NULL,
                           0,
                           (void FAR*)allparameters,
                           sizeof(EAX_REVERBPROPERTIES));
#endif

   API_unlock();
   if (take_mutex) CS_leave(&DRV->lock);
}

//----------------------------------------------------------
// float GetAll()
//
// DESCRIPTION: Gets all parameters of the current reverb
//
// PARAMETERS: none
//
// RETURNS: pointer to structure
//
//----------------------------------------------------------

static void FAR * EAX_GetAll(DRIVERSTATE FAR *DRV,int which)
{
#ifdef EAX2
   static EAXLISTENERPROPERTIES allparameters[2];
#else
   static EAX_REVERBPROPERTIES allparameters[2];
#endif
   unsigned long r;

   API_lock();

   DRV->lpPropertySet->Get(EAXPROP,
#ifdef EAX2
                           DSPROPERTY_EAXLISTENER_ALLPARAMETERS,
#else
                           DSPROPERTY_EAX_ALL,
#endif
                           NULL,
                           0,
                          &allparameters[which],
                           sizeof(float),
                          &r);
   API_unlock();

   return &allparameters[which];
}

#if 0
//----------------------------------------------------------
// float GetSampleReverbLevels()
//
// DESCRIPTION: Gets the current per-voice reverb levels
//
// PARAMETERS: none
//
// RETURNS: wet/dry values
//
//----------------------------------------------------------

static void EAX_GetSampleReverbLevels(DSVOICE FAR *V, F32 FAR *wet, F32 FAR *dry)
{
   float W = CREVERBBUFFER_INVALID_VALUE;
   float D = CREVERBBUFFER_INVALID_VALUE;
   unsigned long r;

   API_lock();

#ifdef EAX2
   LONG room = 20000;

   V->lpPropertySet->Get(EAXBUFPROP,
                         DSPROPERTY_EAXBUFFER_ROOM,
                         NULL,
                         0,
                        &room,
                         sizeof(LONG),
                        &r);
   if (room != 20000)
      {
      W = dB_to_linear((F32) room);
      }

   room = 20000;

   V->lpPropertySet->Get(EAXBUFPROP,
                         DSPROPERTY_EAXBUFFER_DIRECT,
                         NULL,
                         0,
                        &room,
                         sizeof(LONG),
                        &r);
   if (room != 20000)
      {
      D = dB_to_linear((F32) room);
      }

#else
   F32 mix;

   V->lpPropertySet->Get(EAXBUFPROP,
                         DSPROPERTY_EAXBUFFER_REVERBMIX,
                         NULL,
                         0,
                        &mix,
                         sizeof(float),
                        &r);

   if (mix < 0.5F) 
      {
      D = 1.0F;
      W = mix * 2.0F;
      }
   else
      {
      D = (1.0F - mix) * 2.0F;
      W = 1.0F;
      }

#endif

   if (wet != NULL) *wet = W;
   if (dry != NULL) *dry = D;

   API_unlock();
}
#endif

//----------------------------------------------------------
// float SetSampleAll()
//
// DESCRIPTION: Sets all parameters of the current sample
//
// PARAMETERS: pointer to structure
//
// RETURNS: none
//
//----------------------------------------------------------

static void EAX_SetSampleAll(DSVOICE FAR     *V, //)
                             void const FAR  *allparameters,
                             S32              take_mutex = TRUE)
{
   if (take_mutex) CS_enter(&V->lock);
   API_lock();

   //
   // TODO: Doc that this function does NOT update the shadow registers 
   // and driver reverb vars.  If an application ever sets this preference, it must 
   // continue to do so!
   //

#ifdef EAX2
   V->lpPropertySet->Set(EAXBUFPROP,
                         DSPROPERTY_EAXBUFFER_ALLPARAMETERS,
                         NULL,
                         0,
                         (void FAR*) allparameters,
                         sizeof(EAXBUFFERPROPERTIES));
#else
   V->lpPropertySet->Set(EAXBUFPROP,
                         DSPROPERTY_EAXBUFFER_ALL,
                         NULL,
                         0,
                         (void FAR*) allparameters,
                         sizeof(EAXBUFFER_REVERBPROPERTIES));
#endif

   API_unlock();
   if (take_mutex) CS_leave(&V->lock);
}

//----------------------------------------------------------
// float GetSampleAll()
//
// DESCRIPTION: Gets all parameters of the current sample
//
// PARAMETERS: none
//
// RETURNS: pointer to structure
//
//----------------------------------------------------------

static void FAR * EAX_GetSampleAll(DSVOICE FAR *V, int which )
{
#ifdef EAX2
   static EAXBUFFERPROPERTIES allparameters[2];
#else
   static EAXBUFFER_REVERBPROPERTIES allparameters[2];
#endif
   unsigned long r;

   API_lock();

   V->lpPropertySet->Get(EAXBUFPROP,
#ifdef EAX2
                         DSPROPERTY_EAXBUFFER_ALLPARAMETERS,
#else
                         DSPROPERTY_EAXBUFFER_ALL,
#endif
                         NULL,
                         0,
                        &allparameters[which],
                         sizeof(float),
                        &r);

   API_unlock();

   return &allparameters[which];
}

//############################################################################
//#                                                                          #
//# Destroy EAX-specific objects                                             #
//#                                                                          #
//############################################################################

static void EAX_destroy(DRIVERSTATE FAR *DRV)
{
   API_lock();

   if (DRV->lpSecondaryBuffer != NULL)
      {
      DRV->lpSecondaryBuffer->Release();
      DRV->lpSecondaryBuffer = NULL;
      }

   if (DRV->lpDs3dBuffer != NULL)
      {
      DRV->lpDs3dBuffer->Release();
      DRV->lpDs3dBuffer = NULL;
      }

   if (DRV->lpPropertySet != NULL)
      {
      DRV->lpPropertySet->Release();
      DRV->lpPropertySet = NULL;
      }

   API_unlock();
}

#endif

#ifdef EAX4

static const GUID * fxslots[4] = { &EAXPROPERTYID_EAX40_FXSlot0, &EAXPROPERTYID_EAX40_FXSlot1, &EAXPROPERTYID_EAX40_FXSlot2, &EAXPROPERTYID_EAX40_FXSlot3 };

static const U32 eax_props[]= { EAXAGCCOMPRESSOR_ALLPARAMETERS, EAXAUTOWAH_ALLPARAMETERS, EAXCHORUS_ALLPARAMETERS, 
                                EAXDISTORTION_ALLPARAMETERS, EAXECHO_ALLPARAMETERS, EAXEQUALIZER_ALLPARAMETERS, 
                                EAXFLANGER_ALLPARAMETERS, EAXFREQUENCYSHIFTER_ALLPARAMETERS, EAXVOCALMORPHER_ALLPARAMETERS,
                                EAXPITCHSHIFTER_ALLPARAMETERS, EAXRINGMODULATOR_ALLPARAMETERS, EAXREVERB_ALLPARAMETERS };

static const GUID * eax_guids[]= { &EAX_AGCCOMPRESSOR_EFFECT, &EAX_AUTOWAH_EFFECT, &EAX_CHORUS_EFFECT, 
                                   &EAX_DISTORTION_EFFECT, &EAX_ECHO_EFFECT, &EAX_EQUALIZER_EFFECT, 
                                   &EAX_FLANGER_EFFECT, &EAX_FREQUENCYSHIFTER_EFFECT, &EAX_VOCALMORPHER_EFFECT,
                                   &EAX_PITCHSHIFTER_EFFECT, &EAX_RINGMODULATOR_EFFECT, &EAX_REVERB_EFFECT };

static const U32 eax_struct_sizes[] = { sizeof( EAX_AUTOGAIN ), sizeof( EAX_AUTOWAH ), sizeof( EAX_CHORUS ), 
                                        sizeof( EAX_DISTORTION ), sizeof( EAX_ECHO ), sizeof( EAX_EQUALIZER ),
                                        sizeof( EAX_FLANGER  ), sizeof( EAX_FSHIFTER ), sizeof( EAX_VMORPHER ),
                                        sizeof( EAX_PSHIFTER ), sizeof( EAX_RMODULATOR ), sizeof( EAX_REVERB ) };


static S32 set_eax4(DRIVERSTATE FAR *DRV, //)
                    S32              slot,
                    void const      *value)
{
  U32 effect;
  BOOL v;

  if ( DRV->lpPropertySet == 0 )
  {
    return( 0 );
  }

  effect = ( (U32*) value )[ 0 ];
      
  if ( effect != (U32) DRV->sloteffects[ slot ] )
  {
    EAXFXSLOTPROPERTIES p;

    p.guidLoadEffect = *eax_guids[ effect - 1 ];
    p.lLock = EAXFXSLOT_UNLOCKED;
    p.lVolume = ( (U32*) value )[ 1 ];
    p.ulFlags = ( effect == MSS_EAX_REVERB ) ? EAXFXSLOTFLAGS_ENVIRONMENT : 0;

    API_lock();

    v = SUCCEEDED( DRV->lpPropertySet->Set(*fxslots[ slot ],
                                            EAXFXSLOT_ALLPARAMETERS,
                                            NULL,
                                            0,
                                           &p, 
                                            sizeof( p ) ) );
    
    API_unlock();

    if ( !v )
    {
      return( 0 );
    }

    DRV->slotvolumes[ slot ] = ( (U32*) value )[ 1 ];
    DRV->sloteffects[ slot ] = effect;
  }
  else
  {
    if ( (U32) DRV->slotvolumes[ slot ] != ( (U32*) value )[ 1 ] )
    {
      API_lock();

      v = SUCCEEDED( DRV->lpPropertySet->Set(*fxslots[ slot ],
                                              EAXFXSLOT_VOLUME,
                                              NULL,
                                              0,
                                              ( (U32*) value ) + 1, 4 ) );

      API_unlock();

      if ( !v )
      {
        return( 0 );
      }
  
      DRV->slotvolumes[ slot ] = ( (U32*) value )[ 1 ];
    }
  }

  API_lock();
  v = SUCCEEDED( DRV->lpPropertySet->Set(*fxslots[ slot ],
                                          eax_props[ effect - 1 ],
                                          NULL,
                                          0,
                                          ( (S32*) value ) + 2, eax_struct_sizes[ effect - 1 ] - 8 ) );

  API_unlock();

  return( v ? 1 : 0 );
}

static void *   get_eax4(DRIVERSTATE FAR *DRV, //)
                         S32              slot,
                         int              which)
{
  BOOL v;
  static EAX_REVERB ret[4][2];

  ret[slot][which].Effect = DRV->sloteffects[ slot ];
  ret[slot][which].Volume = DRV->slotvolumes[ slot ];
  
  DWORD size=0;

  API_lock();
  v = SUCCEEDED( DRV->lpPropertySet->Get(*fxslots[ slot ],
                                          eax_props[ DRV->sloteffects[ slot ] ],
                                          NULL,
                                          0,
                                          &ret[slot][which].Environment, eax_struct_sizes[ DRV->sloteffects[ slot ] ] - 8, &size ) );

  API_unlock();

  return( v ? &ret[slot][which] : 0 );
}


static void FAR * get_eax4_sample_volumes(DSVOICE FAR *V, //)
                                          int which )
{
  EAXSOURCESENDPROPERTIES all[ EAX_MAX_FXSLOTS ];
  EAXSOURCEOCCLUSIONSENDPROPERTIES occ[ EAX_MAX_FXSLOTS ];
  EAXACTIVEFXSLOTS active;
  U32 i;
  static EAX_SAMPLE_SLOT_VOLUMES vol[2];

  if ( V->lpPropertySet == 0 )
  {
    return( 0 );
  }

  BOOL v;
  DWORD size1 = 0;
  DWORD size2 = 0;
  DWORD size3 = 0;

  API_lock();
  v = SUCCEEDED( V->lpPropertySet->Get( EAXPROPERTYID_EAX40_Source,
                      EAXSOURCE_ACTIVEFXSLOTID,
                      NULL,0,
                      &active, sizeof( active ), &size1  ) );

  v &= SUCCEEDED( V->lpPropertySet->Get( EAXPROPERTYID_EAX40_Source,
                      EAXSOURCE_SENDPARAMETERS,
                      NULL,0,
                      &all, sizeof( all ), &size2  ) );

  v &= SUCCEEDED( V->lpPropertySet->Get( EAXPROPERTYID_EAX40_Source,
                      EAXSOURCE_OCCLUSIONSENDPARAMETERS,
                      NULL,0,
                      &occ, sizeof( occ ), &size3  ) );
  API_unlock();

  if ( ( !v ) || ( size1 != sizeof( active ) ) || ( size2 != sizeof( all ) ) || ( size3 != sizeof( occ ) ) )
  {
    return( 0 );
  }

  vol[which].NumVolumes = 0;
  for ( i = 0 ; i < EAX_MAX_ACTIVE_FXSLOTS ; i++ )
  {
    U32 slot;

    if ( active.guidActiveFXSlots[ i ] == EAXPROPERTYID_EAX40_FXSlot0 ) slot = 0;
    else if ( active.guidActiveFXSlots[ i ] == EAXPROPERTYID_EAX40_FXSlot1 ) slot = 1;
    else if ( active.guidActiveFXSlots[ i ] == EAXPROPERTYID_EAX40_FXSlot2 ) slot = 2;
    else if ( active.guidActiveFXSlots[ i ] == EAXPROPERTYID_EAX40_FXSlot3 ) slot = 3;
    else 
      continue;

    ++vol[which].NumVolumes;
    vol[which].volumes[ i ].Slot = slot;
    vol[which].volumes[ i ].Send = all[ slot ].lSend;
    vol[which].volumes[ i ].SendHF = all[ slot ].lSendHF;
    vol[which].volumes[ i ].Occlusion = occ[ slot ].lOcclusion;
    vol[which].volumes[ i ].OcclusionLFRatio = occ[ slot ].flOcclusionLFRatio;
    vol[which].volumes[ i ].OcclusionRoomRatio = occ[ slot ].flOcclusionRoomRatio;
    vol[which].volumes[ i ].OcclusionDirectRatio = occ[ slot ].flOcclusionDirectRatio;
  }

  return( &vol[which] );
}

static S32 set_eax4_sample_volumes(DSVOICE FAR *V, //)
                                   void const  *value)
{
  EAXSOURCESENDPROPERTIES all[ EAX_MAX_FXSLOTS ];
  EAXSOURCEOCCLUSIONSENDPROPERTIES occ[ EAX_MAX_FXSLOTS ];
  EAXACTIVEFXSLOTS active;
  BOOL v;
  U32 i;
  EAX_SAMPLE_SLOT_VOLUMES * vol = (EAX_SAMPLE_SLOT_VOLUMES*) value;

  if ( V->lpPropertySet == 0 )
  {
    return( 0 );
  }

  if ( vol->NumVolumes > 2 )
  {
    return( 0 );
  }

  for ( i = 0 ; i < 4 ; i++ )
  {
    all[ i ].guidReceivingFXSlotID = *fxslots[ i ];
    all[ i ].lSend = 0;
    all[ i ].lSendHF = 0;
    occ[ i ].guidReceivingFXSlotID = *fxslots[ i ];
    occ[ i ].lOcclusion = 0;
    occ[ i ].flOcclusionLFRatio = 0.25f;
    occ[ i ].flOcclusionRoomRatio = 1.5f;
    occ[ i ].flOcclusionDirectRatio = 1.0f;
  }

  for ( i = 0 ; i < vol->NumVolumes ; i++ )
  {
    if ( ( vol->volumes[ i ].Slot < 0 ) || ( vol->volumes[ i ].Slot > 3 ) )
    {
      return( 0 );
    }

    all[ vol->volumes[ i ].Slot ].lSend = vol->volumes[ i ].Send;
    all[ vol->volumes[ i ].Slot ].lSendHF = vol->volumes[ i ].SendHF;
    occ[ vol->volumes[ i ].Slot ].lOcclusion = vol->volumes[ i ].Occlusion;
    occ[ vol->volumes[ i ].Slot ].flOcclusionLFRatio = vol->volumes[ i ].OcclusionLFRatio;
    occ[ vol->volumes[ i ].Slot ].flOcclusionRoomRatio = vol->volumes[ i ].OcclusionRoomRatio;
    occ[ vol->volumes[ i ].Slot ].flOcclusionDirectRatio = vol->volumes[ i ].OcclusionDirectRatio;

    active.guidActiveFXSlots[ i ] = *fxslots[ vol->volumes[ i ].Slot ];
  }

  for ( i = vol->NumVolumes ; i < EAX_MAX_ACTIVE_FXSLOTS ; i++ )
  {
    active.guidActiveFXSlots[ i ] = EAX_NULL_GUID;
  }
  
  API_lock();
  v = SUCCEEDED( V->lpPropertySet->Set( EAXPROPERTYID_EAX40_Source,
                      EAXSOURCE_ACTIVEFXSLOTID,
                      NULL,0,
                      &active, sizeof( active ) ) );

  v &= SUCCEEDED( V->lpPropertySet->Set( EAXPROPERTYID_EAX40_Source,
                      EAXSOURCE_SENDPARAMETERS,
                      NULL,0,
                      &all, sizeof( all ) ) );

  v &= SUCCEEDED( V->lpPropertySet->Set( EAXPROPERTYID_EAX40_Source,
                      EAXSOURCE_OCCLUSIONSENDPARAMETERS,
                      NULL,0,
                      &occ, sizeof( occ ) ) );

  API_unlock();

  return( v ? 1 : 0 );
}

#endif //EAX3D

//############################################################################
//##                                                                        ##
//## Lock region of secondary buffer, returning write cursor information    ##
//##                                                                        ##
//############################################################################

static S32 DS_lock_secondary_region(DSVOICE FAR  *V, //)
                                    S32           offset,
                                    S32           size,
                                    void        **p1,
                                    U32          *l1,
                                    void        **p2,
                                    U32          *l2)
{
   HRESULT result;

   //
   // Lock the buffer, returning 0 on failure
   //

   do
      {
      API_lock();

      result = V->lpdsb->Lock(offset,
                              size,
                              p1,
                              (DWORD*)l1,
                              p2,
                              (DWORD*)l2,
                              0);

      API_unlock();

      if (result == DSERR_BUFFERLOST)
         {
         API_lock();
         V->lpdsb->Restore();
         API_unlock();
         }
      }
   while (result == DSERR_BUFFERLOST);

   if (result != DS_OK)
      {
      return 0;
      }

   if ((*l1) + (*l2) != (U32) size)
      {
      return 0;
      }

   return 1;
}

//############################################################################
//##                                                                        ##
//## Unlock region of secondary buffer                                      ##
//##                                                                        ##
//############################################################################

static void DS_unlock_secondary_region(DSVOICE FAR *V, //)
                                       void        *p1,
                                       U32          l1,
                                       void        *p2,
                                       U32          l2)
{
   HRESULT result;

   //
   // Unlock the buffer, returning 0 on failure
   //

   do
      {
      API_lock();

      result = V->lpdsb->Unlock(p1,
                                l1,
                                p2,
                                l2);

      API_unlock();

      if (result == DSERR_BUFFERLOST)
         {
         API_lock();
         V->lpdsb->Restore();
         API_unlock();
         }
      }
   while (result == DSERR_BUFFERLOST);
}

//############################################################################
//##                                                                        ##
//## Start playback of secondary buffer at beginning                        ##
//##                                                                        ##
//############################################################################

static void DS_start_secondary(DSVOICE FAR *V)
{
   HRESULT result;

   do
      {
      API_lock();

      result = V->lpdsb->Play(0,
                              0,
                              DSBPLAY_LOOPING);

      API_unlock();

      if (result == DSERR_BUFFERLOST)
         {
         API_lock();
         V->lpdsb->Restore();
         API_unlock();
         }
      }
   while (result == DSERR_BUFFERLOST);
}

//############################################################################
//##                                                                        ##
//## Stop playback of secondary buffer                                      ##
//##                                                                        ##
//############################################################################

static void DS_stop_secondary(DSVOICE FAR *V)
{
   HRESULT result;

   do
      {
      API_lock();

      result = V->lpdsb->Stop();

      API_unlock();

      if (result == DSERR_BUFFERLOST)
         {
         API_lock();
         V->lpdsb->Restore();
         API_unlock();
         }
      }
   while (result == DSERR_BUFFERLOST);
}

//############################################################################
//##                                                                        ##
//## Rewind secondary buffer to beginning                                   ##
//##                                                                        ##
//############################################################################

static void DS_rewind_secondary(DSVOICE FAR *V)
{
   HRESULT result;

   do
      {
      API_lock();

      result = V->lpdsb->SetCurrentPosition(0);

      API_unlock();

      if (result == DSERR_BUFFERLOST)
         {
         API_lock();
         V->lpdsb->Restore();
         API_unlock();
         }
      }
   while (result == DSERR_BUFFERLOST);

   V->previous_position = 0;
}

//############################################################################
//##                                                                        ##
//## Flush sample's secondary buffer with silence                           ##
//##                                                                        ##
//############################################################################

static void DS_flush_secondary(DSVOICE FAR *V)
{
   DWORD   dwDummy;
   void   *lpDummy;
   DWORD   cnt;
   void   *dest;
   U32     silence;
   HRESULT result;

   //
   // Request lock on entire buffer
   //

   API_lock();

   do
      {
      result = V->lpdsb->Lock(0,
                              BUFF_FRAG_BYTES * TOTAL_FRAGS,
                             &dest,
                             &cnt,
                             &lpDummy,
                             &dwDummy,
                              0);

      if (result == DSERR_BUFFERLOST)
         {
         V->lpdsb->Restore();
         }
      }
   while (result == DSERR_BUFFERLOST);

   if (result != DS_OK)
      {
      API_unlock();
      return;
      }

   if (cnt != (U32) BUFF_FRAG_BYTES * TOTAL_FRAGS)
      {
      API_unlock();
      return;
      }

   //
   // Flush with silence
   //

   silence = 0;

   memset(dest,
          silence,
          cnt);

   //
   // Release lock
   //

   V->lpdsb->Unlock(dest,
                    cnt,
                    lpDummy,
                    dwDummy);

   API_unlock();
}

//############################################################################
//#                                                                          #
//# Flush sample buffers                                                     #
//#                                                                          #
//############################################################################

static void DS_stop_flush_rewind(DSVOICE FAR *V, //)
                                 BOOL         flush_with_silence)
{
   DS_stop_secondary(V);

   if (flush_with_silence)
      {
      DS_flush_secondary(V);
      }

   DS_rewind_secondary(V);
}

//############################################################################
//#                                                                          #
//# Get position                                                             #
//#                                                                          #
//############################################################################

static int DS_get_position( DSVOICE FAR *V, //)
                            DWORD   FAR *play  = NULL, 
                            DWORD   FAR *write = NULL)
{
   for(;;)
      {
      DWORD p1, w1, p2, w2;
    
      // get first position
      if (!(SUCCEEDED ( V->lpdsb->GetCurrentPosition( &p1, &w1))))
         {
         return 0;
         }

      // acts a delay 
      V->lpdsb->GetCurrentPosition( &p2, &w2 );

      // get next position
      V->lpdsb->GetCurrentPosition( &p2, &w2 );

      // make sure it hasn't wrapped oddly (meaning that a backwards read position may have occurred)
      if (((p2 - p1) < 1024) && ((w2 - w1) < 1024))
         {
         if (play  != NULL) *play  = p2;
         if (write != NULL) *write = w2;
         return 1;
         }
      }
}

//############################################################################
//#                                                                          #
//# Update listener state from DRIVERSTATE                                   #
//#                                                                          #
//############################################################################

static void DS_update_listener(DRIVERSTATE FAR *DRV)
{
   S32 settings_changed = 0;

#if HW_LISTENER_POS
   if (!RAD_vectors_equal(&DRV->dig->D3D.listen_position, &DRV->shadow_listen_position))
      {
      DRV->lp3DLISTENER->SetPosition(DRV->dig->D3D.listen_position.x,
                                     DRV->dig->D3D.listen_position.y,
                                     DRV->dig->D3D.listen_position.z,
                                     DS3D_DEFERRED);

      DRV->shadow_listen_position = DRV->dig->D3D.listen_position;
      ++settings_changed;
      }
#endif
   
   if (!RAD_vectors_equal(&DRV->dig->D3D.listen_velocity, &DRV->shadow_listen_velocity))
      {
      DRV->lp3DLISTENER->SetVelocity(DRV->dig->D3D.listen_velocity.x * 1000.0F,
                                     DRV->dig->D3D.listen_velocity.y * 1000.0F,
                                     DRV->dig->D3D.listen_velocity.z * 1000.0F,
                                     DS3D_DEFERRED);

      DRV->shadow_listen_velocity = DRV->dig->D3D.listen_velocity;
      ++settings_changed;
      }

   if ((!RAD_vectors_equal(&DRV->dig->D3D.listen_face, &DRV->shadow_listen_face)) ||
       (!RAD_vectors_equal(&DRV->dig->D3D.listen_up,   &DRV->shadow_listen_up)))
      {
      DRV->lp3DLISTENER->SetOrientation(DRV->dig->D3D.listen_face.x,
                                        DRV->dig->D3D.listen_face.y,
                                        DRV->dig->D3D.listen_face.z,
                                        DRV->dig->D3D.listen_up.x,
                                        DRV->dig->D3D.listen_up.y,
                                        DRV->dig->D3D.listen_up.z,
                                        DS3D_DEFERRED);

      DRV->shadow_listen_face = DRV->dig->D3D.listen_face;
      DRV->shadow_listen_up   = DRV->dig->D3D.listen_up;
      ++settings_changed;
      }
   
   if (fabs(DRV->dig->D3D.doppler_factor - DRV->shadow_listen_doppler_factor) > EPSILON)
      {
      DRV->lp3DLISTENER->SetDopplerFactor(DRV->dig->D3D.doppler_factor,
                                          DS3D_DEFERRED);

      DRV->shadow_listen_doppler_factor = DRV->dig->D3D.doppler_factor;
      ++settings_changed;
      }

   if (fabs(DRV->dig->D3D.distance_factor - DRV->shadow_listen_distance_factor) > EPSILON)
      {
      DRV->lp3DLISTENER->SetDistanceFactor(DRV->dig->D3D.distance_factor,
                                           DS3D_DEFERRED);

      DRV->shadow_listen_distance_factor = DRV->dig->D3D.distance_factor;
      ++settings_changed;
      }

   if (fabs(DRV->dig->D3D.rolloff_factor - DRV->shadow_listen_rolloff_factor) > EPSILON)
      {
      DRV->lp3DLISTENER->SetRolloffFactor(DRV->dig->D3D.rolloff_factor,
                                          DS3D_DEFERRED);

      DRV->shadow_listen_rolloff_factor = DRV->dig->D3D.rolloff_factor;
      ++settings_changed;
      }

   //
   // If any changes were made to listener 3D properties, commit them
   //

   if (settings_changed)
      {
      DRV->lp3DLISTENER->CommitDeferredSettings();
      }

   //
   // Check for updated EAX-specific listener properties
   //

#ifndef EAX3D

   //
   // Indicate no support for EAX room types
   // 

   DRV->dig->room_type = -1;

#else

   if (DRV->lpPropertySet != NULL)
      {
      if (DRV->dig->room_type != DRV->shadow_room_type)
         {
         EAX_SetEnvironment(DRV, DRV->dig->room_type, FALSE);
         }

      if (DRV->dig->master_wet != DRV->shadow_master_wet)
         {
         EAX_SetMasterWet(DRV, DRV->dig->master_wet, FALSE);
         }

      if (DRV->dig->reverb_decay_time_s != DRV->shadow_reverb_decay_time_s)
         {
         EAX_SetDecayTime(DRV, DRV->dig->reverb_decay_time_s, FALSE);
         }

      if (DRV->dig->reverb_predelay_s != DRV->shadow_reverb_predelay_s)
         {
         EAX_SetPredelay(DRV, DRV->dig->reverb_predelay_s, FALSE);
         }

      if (DRV->dig->reverb_damping != DRV->shadow_reverb_damping)
         {
         EAX_SetDamping(DRV, DRV->dig->reverb_damping, FALSE);
         }
      }
#endif
}

//############################################################################
//#                                                                          #
//# Update voice state from HSAMPLE                                          #
//#                                                                          #
//############################################################################

static void DS_update_voice(DSVOICE FAR *V)
{
   HSAMPLE S = V->S;

   if (S == NULL)
      {
      return;
      }

   S32 settings_changed = 0;

   // ------------------------------------------------------------------------------------------
   // Volume and reverb control are related in ways that depend on the EAX version (if any)
   // in use
   // ------------------------------------------------------------------------------------------

#ifdef EAX3D

   BOOL vol = fabs(S->save_volume           - V->shadow_volume     ) > EPSILON;
   BOOL occ = fabs(S->occlusion             - V->shadow_occlusion  ) > EPSILON;
   BOOL obs = fabs(S->obstruction           - V->shadow_obstruction) > EPSILON;
   BOOL exc = fabs(S->exclusion             - V->shadow_exclusion  ) > EPSILON;
   BOOL wet = fabs(S->wet_level             - V->shadow_wet_level  ) > EPSILON;
   BOOL dry = fabs(S->dry_level             - V->shadow_dry_level  ) > EPSILON;
   BOOL mdr = fabs(S->driver->master_dry    - V->shadow_master_dry ) > EPSILON;
   BOOL mvo = fabs(S->driver->master_volume - V->shadow_master_vol ) > EPSILON;
   BOOL aut = (S->S3D.auto_3D_atten != V->shadow_auto_3D_atten);

#if defined(EAX2) || defined(EAX3)

   //
   // Set parameters common to EAX2 and EAX3
   //
   // Deferred EAX parameters do not work on some emulated platforms (C-Media CMI 9880 5.12), so 
   // we'll leave it disabled for now
   // 

   const U32 EAX_DEFERRED = 0;   // DSPROPERTY_EAXBUFFER_DEFERRED;

   if (vol)
      {
      V->shadow_volume = S->save_volume;
      F32 net_volume = linear_to_dB(S->save_volume);
      V->lpdsb->SetVolume((S32) net_volume);
      }

   if (dry || mdr || mvo)
      {
      V->shadow_dry_level  = S->dry_level;
      V->shadow_master_dry = S->driver->master_dry;
      V->shadow_master_vol = S->driver->master_volume;

      LONG val = (LONG) linear_to_dB(S->dry_level * S->driver->master_dry * S->driver->master_volume);

      V->lpPropertySet->Set(EAXBUFPROP,
                            DSPROPERTY_EAXBUFFER_DIRECTHF | EAX_DEFERRED,
                            NULL,
                            0,
                           &val,
                            sizeof(LONG));
    
      V->lpPropertySet->Set(EAXBUFPROP,
                            DSPROPERTY_EAXBUFFER_DIRECT | EAX_DEFERRED,
                            NULL,
                            0,
                           &val,
                            sizeof(LONG));

      settings_changed |= EAX_DEFERRED;
      }

   if (obs)
      {
      V->shadow_obstruction = S->obstruction;

      EAX2_Sample_Set_int(V,
                          DSPROPERTY_EAXBUFFER_OBSTRUCTION | EAX_DEFERRED,
                    (S32) linear_to_dB(1.0F - S->obstruction));

      settings_changed |= EAX_DEFERRED;
      }

   if (occ)
      {
      V->shadow_occlusion = S->occlusion;

      EAX2_Sample_Set_int(V,
                          DSPROPERTY_EAXBUFFER_OCCLUSION | EAX_DEFERRED,
                    (S32) linear_to_dB(1.0F - S->occlusion));

      settings_changed |= EAX_DEFERRED;
      }

   if (aut)
      {
      V->shadow_auto_3D_atten = S->S3D.auto_3D_atten;

      ULONG flags = 0;
      ULONG r     = 0;
                                             
      V->lpPropertySet->Get(EAXBUFPROP,
                            DSPROPERTY_EAXBUFFER_FLAGS,
                            NULL,
                            0,
                           &flags,
                            sizeof(ULONG),
                           &r);

      if (S->S3D.auto_3D_atten)
         {
         flags |= EAXBUFFERFLAGS_ROOMAUTO   |     // Scale both wet and wet-HF with 3D distance/cone direction 
                  EAXBUFFERFLAGS_ROOMHFAUTO |    
                  EAXBUFFERFLAGS_DIRECTHFAUTO;    // Source is more directive at higher frequencies (direct path "brighter" in front)
         }
      else
         {
         flags &= ~(EAXBUFFERFLAGS_ROOMAUTO   |   // Do not scale wet level 
                    EAXBUFFERFLAGS_ROOMHFAUTO |   
                    EAXBUFFERFLAGS_DIRECTHFAUTO); 
         }

      V->lpPropertySet->Set(EAXBUFPROP,
                            DSPROPERTY_EAXBUFFER_FLAGS | EAX_DEFERRED,
                            NULL,
                            0,
                           &flags,
                            sizeof(ULONG));

      settings_changed |= EAX_DEFERRED;
      }

   //
   // In EAX 2, exclusion must be concatenated with wet level
   // In EAX 3 and up, these parameters are independent
   //

   S32 set_final_wet = FALSE;
   F32 final_wet=0.0f;

   if (wet)
      {
      V->shadow_wet_level = S->wet_level;

      final_wet     = S->wet_level;
      set_final_wet = TRUE;
      }

   if (exc)
      {
      V->shadow_exclusion = S->exclusion;

#if defined (EAX3)

      EAX2_Sample_Set_int(V,
                          DSPROPERTY_EAXBUFFER_EXCLUSION | EAX_DEFERRED,
                    (S32) linear_to_dB(1.0F - S->exclusion));

      settings_changed |= EAX_DEFERRED;
#else // EAX2
      final_wet     = S->wet_level * (1.0F - S->exclusion);
      set_final_wet = TRUE;      
#endif
      }

   if (set_final_wet)
      {
      LONG val = (LONG) linear_to_dB(final_wet);

      V->lpPropertySet->Set(EAXBUFPROP,
                            DSPROPERTY_EAXBUFFER_ROOMHF | EAX_DEFERRED,
                            NULL,
                            0,
                           &val,
                            sizeof(LONG));
    
      V->lpPropertySet->Set(EAXBUFPROP,
                            DSPROPERTY_EAXBUFFER_ROOM | EAX_DEFERRED,
                            NULL,
                            0,
                           &val,
                            sizeof(LONG));

      settings_changed |= EAX_DEFERRED;
      }

#else
   //
   // EAX 1 only
   //
   // If wet level is attenuated with distance/cone direction, we must duplicate DS3D's 
   // falloff calculations to obtain the wet-level scalar, since EAX 1 won't do it for us
   //

   if (vol || wet || dry || mdr || mvo || exc || obs || occ || aut)
      {
      V->shadow_volume        = S->save_volume;
      V->shadow_wet_level     = S->wet_level;
      V->shadow_dry_level     = S->dry_level;
      V->shadow_occlusion     = S->occlusion;
      V->shadow_exclusion     = S->exclusion;
      V->shadow_obstruction   = S->obstruction;
      V->shadow_auto_3D_atten = S->S3D.auto_3D_atten;
      V->shadow_master_dry    = S->driver->master_dry;
      V->shadow_master_vol    = S->driver->master_volume;

      F32 wet_level = S->save_volume * S->driver->master_volume                         * S->wet_level * (1.0F - S->exclusion);
      F32 dry_level = S->save_volume * S->driver->master_volume * S->driver->master_dry * S->dry_level * (1.0F - S->obstruction);

      if (S->S3D.auto_3D_atten)
         {
         MSSVECTOR3D listener_to_source;
         F32         distance;

         if (!SS_calculate_3D_attenuation(&S->driver->D3D, 
                                          &S->S3D,
                                          &distance,
                                          &listener_to_source,
                                           SS_default_falloff_function_callback))
            {
            wet_level = 0.0F;
            dry_level = 0.0F;
            }
         else
            {
            wet_level *= S->S3D.atten_3D;
            }
         }

      F32 EAX_reverb_mix      = 0.0F;
      F32 EAX_hardware_volume = 0.0F;

      if ((dry_level > EPSILON) || (wet_level > EPSILON))
         {
         if (dry_level >= wet_level)
            {
            EAX_hardware_volume = dry_level;
            EAX_reverb_mix      = (wet_level / dry_level) * 0.5F;
            }
         else
            {
            EAX_hardware_volume = wet_level;
            EAX_reverb_mix      = 1.0F - ((dry_level / wet_level) * 0.5F);
            }
         }

      V->lpPropertySet->Set(EAXBUFPROP,
                            DSPROPERTY_EAXBUFFER_REVERBMIX,
                            NULL,
                            0,
                           &EAX_reverb_mix,
                            sizeof(float));

      V->lpdsb->SetVolume((S32) linear_to_dB(EAX_hardware_volume));
      }

#endif
#else
   //
   // DS3D hardware (no EAX)
   //
       
   BOOL vol = fabs(S->save_volume           - V->shadow_volume     ) > EPSILON;
   BOOL occ = fabs(S->occlusion             - V->shadow_occlusion  ) > EPSILON;
   BOOL obs = fabs(S->obstruction           - V->shadow_obstruction) > EPSILON;
   BOOL dry = fabs(S->dry_level             - V->shadow_dry_level  ) > EPSILON;
   BOOL mdr = fabs(S->driver->master_dry    - V->shadow_master_dry ) > EPSILON;
   BOOL mvo = fabs(S->driver->master_volume - V->shadow_master_vol ) > EPSILON;

   if (vol || dry || occ || obs || mdr || mvo)
      {
      V->shadow_volume      = S->save_volume;
      V->shadow_dry_level   = S->dry_level;
      V->shadow_occlusion   = S->occlusion;
      V->shadow_obstruction = S->obstruction;
      V->shadow_master_dry  = S->driver->master_dry;
      V->shadow_master_vol  = S->driver->master_volume;

      F32 net_volume = linear_to_dB( S->save_volume
                                   * S->dry_level
                                   * S->driver->master_dry
                                   * S->driver->master_volume
                                   * (1.0F - S->obstruction)
//                                 * (1.0F - S->occlusion)  // Handled by core LPF
                                    );

      V->lpdsb->SetVolume((S32) net_volume);
      }

#endif

   // ------------------------------------------------------------------------------------------
   // Playback rate
   // ------------------------------------------------------------------------------------------

   if (S->playback_rate != V->shadow_playback_rate)
      {
      V->lpdsb->SetFrequency(S->playback_rate);
      V->shadow_playback_rate = S->playback_rate;
      }

   // ------------------------------------------------------------------------------------------
   // 3D/2D mode
   // ------------------------------------------------------------------------------------------

   if (S->is_3D != V->shadow_is_3D)
      {
      if (S->is_3D)
         {
         V->lpds3db->SetMode(DS3DMODE_NORMAL, DS3D_DEFERRED);
         }
      else
         {
         V->lpds3db->SetMode(DS3DMODE_DISABLE, DS3D_DEFERRED);
         }

      V->shadow_is_3D = S->is_3D;
      ++settings_changed;
      }

   // ------------------------------------------------------------------------------------------
   // Technically, the following parameters are valid only for samples under 3D control, 
   // but we must update them anyway to handle voices that are swapped between 3D/non-3D samples
   //
   // This means that S3D members need to be valid even for non-3D-driven samples!
   // ------------------------------------------------------------------------------------------

#if HW_LISTENER_POS
   if (!RAD_vectors_equal(&S->S3D.position, &V->shadow_position))
      {
      V->lpds3db->SetPosition(S->S3D.position.x,
                              S->S3D.position.y,
                              S->S3D.position.z,
                              DS3D_DEFERRED);

      V->shadow_position = S->S3D.position;
      ++settings_changed;
      }
#else

   if ((!RAD_vectors_equal(&V->DRV->dig->D3D.listen_position, &V->DRV->shadow_listen_position))
        ||
       (!RAD_vectors_equal(&S->S3D.position, &V->shadow_position)))
      {
      F32 x = S->S3D.position.x - V->DRV->dig->D3D.listen_position.x;
      F32 y = S->S3D.position.y - V->DRV->dig->D3D.listen_position.y;
      F32 z = S->S3D.position.z - V->DRV->dig->D3D.listen_position.z;

      V->lpds3db->SetPosition(x,
                              y,
                              z,
                              DS3D_DEFERRED);
      
      V->shadow_position = S->S3D.position;
      ++settings_changed;
      }

#endif

   if (!RAD_vectors_equal(&S->S3D.face, &V->shadow_face))
      {
      V->lpds3db->SetConeOrientation(S->S3D.face.x,
                                     S->S3D.face.y,
                                     S->S3D.face.z,
                                     DS3D_DEFERRED);
      V->shadow_face = S->S3D.face;
      ++settings_changed;
      }

   if (!RAD_vectors_equal(&S->S3D.velocity, &V->shadow_velocity))
      {
      V->lpds3db->SetVelocity(S->S3D.velocity.x * 1000.0F,
                              S->S3D.velocity.y * 1000.0F,
                              S->S3D.velocity.z * 1000.0F,
                              DS3D_DEFERRED);

      V->shadow_velocity = S->S3D.velocity;
      ++settings_changed;
      }

   if (S->S3D.dist_changed)
      {
      //
      // Don't check for changes to min/max distance until the API layer has
      // updated both vars
      //

      if (fabs(S->S3D.max_dist - V->shadow_max_dist) > EPSILON)
         {
         V->lpds3db->SetMaxDistance(S->S3D.max_dist, DS3D_DEFERRED);

         V->shadow_max_dist = S->S3D.max_dist;
         ++settings_changed;
         }

      if (fabs(S->S3D.min_dist - V->shadow_min_dist) > EPSILON)
         {
         V->lpds3db->SetMinDistance(S->S3D.min_dist, DS3D_DEFERRED);

         V->shadow_min_dist = S->S3D.min_dist;
         ++settings_changed;
         }

      S->S3D.dist_changed = FALSE;
      }

   if ((fabs(S->S3D.inner_angle - V->shadow_inner_angle) > EPSILON) ||
       (fabs(S->S3D.outer_angle - V->shadow_outer_angle) > EPSILON))
      {
      V->lpds3db->SetConeAngles((S32) DIAMS_TO_DEGS(S->S3D.inner_angle),
                                (S32) DIAMS_TO_DEGS(S->S3D.outer_angle), 
                                      DS3D_DEFERRED);

      V->shadow_inner_angle = S->S3D.inner_angle;
      V->shadow_outer_angle = S->S3D.outer_angle;
      ++settings_changed;
      }

   if (fabs(S->S3D.outer_volume - V->shadow_outer_volume) > EPSILON)
      {
      F32 net_volume = linear_to_dB(S->S3D.outer_volume);

      V->lpds3db->SetConeOutsideVolume((S32) net_volume, DS3D_DEFERRED);
                                                   
      V->shadow_outer_volume = S->S3D.outer_volume;
      ++settings_changed;
      }

   // ------------------------------------------------------------------------------------------
   // If any changes were made to DS3D or EAX voice properties, commit them
   // ------------------------------------------------------------------------------------------

   if (settings_changed)
      {
      V->DRV->lp3DLISTENER->CommitDeferredSettings();
      }
}

//############################################################################
//#                                                                          #
//# Copy data from HSAMPLE source to DS3D secondary buffer                   #
//#                                                                          #
//############################################################################

static void DS_stream_to_buffer(DSVOICE FAR *V)
{  
   void *dest;
   U32   len;
   void *dest2;
   U32   len2;

   //
   // Lock segment at head of fragment list
   //

   if (!DS_lock_secondary_region(V,
                                 V->head_frag * BUFF_FRAG_BYTES,
                                 BUFF_FRAG_BYTES,
                                &dest,
                                &len,
                                &dest2,
                                &len2))
      {
      return;
      }
   
   assert(dest2 == NULL);
   assert(len2  == 0);

   S32 out_pos = V->DRV->dig->stream_callback(V->S,
                                  (S16 FAR *) dest,
                                              len);

   //
   // If SS_stream_to_buffer() did not fill the entire fragment, then we either have an
   // ended sample or a starved stream.  Fill the remainder of the fragment with silence
   //

   S32 remnant = len - out_pos;

   if (remnant > 0)
      {
      AIL_memset(&((U8 FAR *) dest)[out_pos],
                  0,
                  remnant);
      }

   DS_unlock_secondary_region(V,
                              dest,
                              len,
                              dest2,
                              len2);

   //
   // Bump fragment head pointer
   //

   V->head_frag = (V->head_frag + 1) % TOTAL_FRAGS;
}

//############################################################################
//#                                                                          #
//# Write silence to head fragment                                           #
//#                                                                          #
//############################################################################

static void DS_write_silent_fragment(DSVOICE FAR *V)
{  
   void *dest;
   U32   len;
   void *dest2;
   U32   len2;

   //
   // Lock segment at head of fragment list
   //

   if (!DS_lock_secondary_region(V,
                                 V->head_frag * BUFF_FRAG_BYTES,
                                 BUFF_FRAG_BYTES,
                                &dest,
                                &len,
                                &dest2,
                                &len2))
      {
      return;
      }
   
   assert(dest2 == NULL);
   assert(len2  == 0);

   AIL_memset(dest,
              0,
              len);

   DS_unlock_secondary_region(V,
                              dest,
                              len,
                              dest2,
                              len2);

   //
   // Bump fragment head pointer
   //

   V->head_frag = (V->head_frag + 1) % TOTAL_FRAGS;
}

//############################################################################
//#                                                                          #
//# Callback function to simulate IDirectSoundNotify interface (broken on    #
//# some drivers / DirectSound3D providers)                                  #
//#                                                                          #
//############################################################################

static void AILCALLBACK notify_timer(UINTa user)
{
   S32 i;

   DRIVERSTATE FAR *DRV = (DRIVERSTATE FAR *) user;

   API_lock();

   //
   // Log profiling information
   //

   U32 start_us = AIL_us_count();

   //
   // Lock out foreground calls that may affect driver state
   //

   CS_enter(&DRV->lock);

   if (DRV->n_voices == 0)
      {
      //
      // Exit if we're in the last timer tick after the hardware has been shut down
      //

      CS_leave(&DRV->lock);

      API_unlock();
      return;
      }

   //
   // Check for updated listener properties
   //

   DS_update_listener(DRV);

   //
   // For all voices associated with driver...
   // 

   for (i=0; i < DRV->n_voices; i++)
      {
      //
      // Skip voices that aren't either associated with active HSAMPLEs, or
      // in their shutdown phase
      //

      DSVOICE FAR *V = &DRV->voices[i];

      U32 L = incnoints(&V->lock);

      if (L != 0)
         {
         decnoints(&V->lock);
         continue;
         }

      HSAMPLE S = V->S;

      if ((S == NULL)                           ||
          (V->sample_state == SS_READY_TO_PLAY) || 
          (V->lpdsb == NULL))
         {
         decnoints(&V->lock);
         continue;
         }
      
      //
      // Check for updated voice properties if the sample is actively playing
      //
      // Even if the sample is done, stopped, or deallocated entirely, we must continue to 
      // play its voice data until all of it has reached the DAC
      //

      if (S->driver->sample_status[S->index] == SMP_PLAYING)
         {
         DS_update_voice(V);
         }

      //
      // Get voice's play/write cursors
      //

      DWORD p,w;

      if (!DS_get_position(V, &p, &w))
         {
         decnoints(&V->lock);
         continue;
         }

      //
      // Deal with buggy Creative drivers that stop playing for no reason
      //

      U32 timer = AIL_ms_count();

      if ((p != V->previous_position) || (w != V->previous_write_position))
         {
         V->last_move_time          = timer;
         V->previous_write_position = w;
         }
      else
         {         
         // if neither of the positions have moved in 128 ms - figure we
         //   hit the Creative bug and reset playback
         if ((timer - V->last_move_time) > 128)
            {
            V->lpdsb->Play(0,0,0);
            V->lpdsb->Stop();
            V->lpdsb->Play(0,0,DSBPLAY_LOOPING);

            V->last_move_time = timer;
            }
         }

      //
      // Has play cursor entered the first silent fragment written at the end of a sample?
      // If so, stop the secondary buffer and mark it available for reuse
      //
      // Otherwise, write data FILL_AHEAD_FRAGS ahead of the play cursor
      //

      S32 play_frag = p / BUFF_FRAG_BYTES;

      if (play_frag == V->sample_state)
         {
         DS_stop_secondary(V);
         V->sample_state = SS_READY_TO_PLAY;
         }
      else
         {
         //
         // These loops can write multiple fragments if necessary, e.g. in case of 
         // timer starvation
         //

         U32 tail_frag = (play_frag + FILL_AHEAD_FRAGS) % TOTAL_FRAGS;

         if (S->driver->sample_status[S->index] == SMP_PLAYING)
            {
            while (V->head_frag != tail_frag)
               {
               DS_stream_to_buffer(V);
               }
            }
         else
            {
            //
            // Sample has stopped playing -- write a silent block and schedule a ->Stop() call to
            // take place as soon as we detect that the play cursor has entered it
            //

            if (V->sample_state == SS_PLAYING)
               {
               V->sample_state = (SAMPLE_STATE) V->head_frag;
               }

            while (V->head_frag != tail_frag)
               {
               DS_write_silent_fragment(V);
               }
            }
         }

      V->previous_position = p;
      decnoints(&V->lock);
      }

#if !HW_LISTENER_POS

   //
   // Voices may have been updated to reflect listener movement,
   // so make sure our simulated listener-shadow position is up to date
   //

   DRV->shadow_listen_position = DRV->dig->D3D.listen_position;
#endif

   //
   // Keep the profiling information
   //

   HDIGDRIVER dig = DRV->dig;

   U32 end_us = AIL_us_count();

   start_us = end_us - start_us;

   dig->us_count += start_us;

   if (dig->us_count > 10000000) 
      {
      dig->ms_count += (dig->us_count / 1000);
      dig->us_count = dig->us_count % 1000;
      }

   //
   // Release driver and API locks
   //

   CS_leave(&DRV->lock);

   API_unlock();
}

//############################################################################
//#                                                                          #
//# Acquire hardware resources                                               #
//#                                                                          #
//############################################################################

static S32 acquire_hardware(DRIVERSTATE FAR *DRV, //)
                            S32              max_voices)
{
   //
   // Get DirectSound object and primary buffer pointers from MSS
   //
   // Fail if MSS is not using DirectSound
   //

   DRV->lpDS     = NULL;
   DRV->lpDSPRIM = NULL;

   AIL_get_DirectSound_info(          NULL,
      (AILLPDIRECTSOUND FAR *)       &DRV->lpDS,
      (AILLPDIRECTSOUNDBUFFER FAR *) &DRV->lpDSPRIM);

   if (DRV->lpDS == NULL)
      {
      strcpy(FLT_error_text,"3D provider requires AIL_USE_WAVEOUT==NO");
      return 0;
      }

   //
   // Get # of buffers available in hardware, and set our initial sample-
   // allocation limit to this value
   //

   DRV->DSCaps.dwSize = sizeof(DRV->DSCaps);

   API_lock();

   if (!(SUCCEEDED(DRV->lpDS->GetCaps(&DRV->DSCaps))))
      {
      API_unlock();
      strcpy(FLT_error_text,"lpDS->GetCaps() failed");
      return 0;
      }

   API_unlock();

   S32 n_voices = max(DRV->DSCaps.dwMaxHw3DAllBuffers, 
                      DRV->DSCaps.dwFreeHw3DAllBuffers);

   n_voices = min(max_voices, n_voices);

#ifdef EAX3D
   //
   // EAX: Subtract one secondary buffer for our internal use
   //

   if (n_voices)
      {
      --n_voices;
      }
#endif

   //
   // Bomb out if no samples available
   //

   if (!n_voices)
      {
      strcpy(FLT_error_text,"Provider could not be initialized -- no 3D voices available");
      return 0;
      }

   //
   // Create listener object
   //

   API_lock();

   if (FAILED(DRV->lpDSPRIM->QueryInterface(IID_IDirectSound3DListener,
                                (LPVOID *) &DRV->lp3DLISTENER)))
      {
      strcpy(FLT_error_text,"Failed to create DS3D listener object");

      API_unlock();
      return 0;
      }

   API_unlock();

#ifdef EAX3D

   //
   // EAX: Create dummy secondary buffer to support global reverb object,
   // and get property set
   //
   // (code from Creative's EAXTEST.CPP and .PDF file)
   //

   API_lock();

   DRV->lpSecondaryBuffer = NULL;
   DRV->lpDs3dBuffer      = NULL;
   DRV->lpPropertySet     = NULL;

   WAVEFORMATEX   pcmOut;           // Format of the wave for secondary buffer if we need to make one.
   DSBUFFERDESC   dsbdSecondary;    // description for creating secondary buffer if we need to make one.

   // we don't have a secondary to work with so we will create one.
   ZeroMemory( &dsbdSecondary, sizeof(DSBUFFERDESC));
   ZeroMemory( &pcmOut, sizeof(WAVEFORMATEX));
   // any format should do I just say 11kHz 16 bit mono
   pcmOut.wFormatTag = WAVE_FORMAT_PCM;
   pcmOut.nChannels = 1;
   pcmOut.nSamplesPerSec = 11025;
   pcmOut.nAvgBytesPerSec = 22050;
   pcmOut.nBlockAlign = 2;
   pcmOut.wBitsPerSample = 16;
   pcmOut.cbSize = 0;
   // size is just arbitary but not too small as I have seen problems with single sample buffers...
   dsbdSecondary.dwSize = sizeof(DSBUFFERDESC);
   dsbdSecondary.dwBufferBytes = 1024;
   dsbdSecondary.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_STATIC;
   dsbdSecondary.lpwfxFormat = &pcmOut;

   DWORD support = 0;

   if ((DRV->lpDS->CreateSoundBuffer(&dsbdSecondary, &DRV->lpSecondaryBuffer, NULL) != DS_OK)
       ||
       (DRV->lpSecondaryBuffer->QueryInterface(IID_IDirectSound3DBuffer, (LPVOID *) &DRV->lpDs3dBuffer) != DS_OK)
       ||
       (DRV->lpDs3dBuffer->QueryInterface(IID_IKsPropertySet, (LPVOID *) &DRV->lpPropertySet) != DS_OK)
       ||
#ifdef EAX4
       (DRV->lpPropertySet->QuerySupport(EAXPROPERTYID_EAX40_Context,
         EAXCONTEXT_ALLPARAMETERS,
#else
       (DRV->lpPropertySet->QuerySupport(EAXPROP,
#ifdef EAX2
         DSPROPERTY_EAXLISTENER_ALLPARAMETERS,
#else
         DSPROPERTY_EAX_ALL,
#endif
#endif
         &support) != DS_OK)
       ||
       ((support & PSET_SETGET) != PSET_SETGET))
      {
      EAX_destroy(DRV);

      if (DRV->lp3DLISTENER)
         {
         DRV->lp3DLISTENER->Release();
         DRV->lp3DLISTENER = NULL;
         }

      API_unlock();

      strcpy(FLT_error_text,"Unable to initialize EAX support");
      return 0;
      }

   API_unlock();

#endif

   //
   // Invalidate listener properties
   //

   RAD_vector_invalidate(&DRV->shadow_listen_position);
   RAD_vector_invalidate(&DRV->shadow_listen_face);
   RAD_vector_invalidate(&DRV->shadow_listen_up);
   RAD_vector_invalidate(&DRV->shadow_listen_velocity);

   DRV->shadow_listen_doppler_factor  = FLT_MAX;
   DRV->shadow_listen_distance_factor = FLT_MAX;
   DRV->shadow_listen_rolloff_factor  = FLT_MAX;

   DRV->shadow_room_type            = -1;
   DRV->shadow_master_wet           = FLT_MAX;     
   DRV->shadow_reverb_decay_time_s  = FLT_MAX;    
   DRV->shadow_reverb_damping       = FLT_MAX; 
   DRV->shadow_reverb_predelay_s    = FLT_MAX;

   DS_update_listener(DRV);

   //
   // Register and start buffer-notification timer
   //

   DRV->buffer_timer = AIL_register_timer(notify_timer);

   AIL_set_timer_user(DRV->buffer_timer, (UINTa) DRV);

   AIL_set_timer_frequency(DRV->buffer_timer, FRAG_SERVICE_HZ);

   AIL_start_timer(DRV->buffer_timer);

   //
   // Return success
   //

   return n_voices;
}

//############################################################################
//#                                                                          #
//# Release hardware resources                                               #
//#                                                                          #
//############################################################################

static void release_hardware(DRIVERSTATE FAR *DRV)
{
   AIL_stop_timer          (DRV->buffer_timer);
   AIL_release_timer_handle(DRV->buffer_timer);

   DRV->buffer_timer = 0;

   //
   // Release voice objects
   //

   API_lock();

   CS_enter(&DRV->lock);

   for (S32 i=0; i < DRV->n_voices; i++)
      {
      DSVOICE FAR *V = &DRV->voices[i];

      CS_enter(&V->lock);

      if (V->lpds3db != NULL)
         {
         V->lpds3db->Release();
         V->lpds3db = NULL;
         }

      if (V->lpdsb != NULL)
         {
         V->lpdsb->Release();
         V->lpdsb = NULL;
         }

#ifdef EAX3D
      if (V->lpPropertySet != NULL)
         {
         V->lpPropertySet->Release();
         V->lpPropertySet = NULL;
         }
#endif

      CS_leave(&V->lock);
      }

   DRV->n_voices = 0;

   //
   // Release EAX objects
   //

#ifdef EAX3D
   EAX_destroy(DRV);
#endif

   //
   // Release the listener
   //

   if (DRV->lp3DLISTENER)
      {
      DRV->lp3DLISTENER->Release();
      DRV->lp3DLISTENER = NULL;
      }

   CS_leave(&DRV->lock);

   API_unlock();
}

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
          *(char FAR * FAR*)before_value = PROVIDER_NAME_STR;
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

      case PROVIDER_FLAGS: 
        if (before_value) 
        {
          *(S32 FAR*)before_value = PROVIDER_FLAGS_VAL;
          return 1;
        }
        break;
      }
  return 0;
}

//############################################################################
//#                                                                          #
//# Get driver-level property value, returning previous setting              #
//#                                                                          #
//############################################################################

static S32 AILCALL VDRV_force_update (HDRIVERSTATE driver)
{
   API_lock();

   DRIVERSTATE FAR *DRV = (DRIVERSTATE FAR *) driver;

   CS_enter(&DRV->lock);

   DS_update_listener(DRV);

   CS_leave(&DRV->lock);

   API_unlock();

   return 1;
}

//############################################################################
//#                                                                          #
//#  Assign hardware voice to HSAMPLE, if not already done                   #
//#                                                                          #
//############################################################################

static S32 AILCALL VFLT_assign_sample_voice (HDRIVERSTATE driver, //)
                                               HSAMPLE      S)
{
   DRIVERSTATE FAR *DRV = (DRIVERSTATE FAR *) driver;

   //
   // Called by AIL_allocate_sample_handle()
   //
   // Return values:
   //
   // 0: Voice resources could not be allocated
   // 1: Existing voice was reused
   // 2: New voice was allocated
   //

   API_lock();

   //
   // Keep sample's current voice if it has one
   //

   if (S->voice != NULL)
      {
      API_unlock();
      return 1;
      }

   //
   // Find slot for new voice 
   // Note that n_voices == # of available voices at startup time 
   // (*or* reacquisition time, which may be smaller)
   //

   DSVOICE FAR *V = NULL;

   S32 i;
   for (i=0; i < DRV->n_voices; i++)
      {
      V = &DRV->voices[i];

      if (V->lpdsb == NULL)
         {
         break;
         }
      }

   if (i == DRV->n_voices)
      {
      //
      // No available hardware voices
      //

      API_unlock();
      return 0;    
      }

   //
   // Initialize voice slot, including shadowed hardware state
   //

   memset(V, 0, sizeof(*V));

   V->shadow_playback_rate = -1;
   V->shadow_volume        = FLT_MAX;
   V->shadow_exclusion     = FLT_MAX;
   V->shadow_obstruction   = FLT_MAX;
   V->shadow_occlusion     = FLT_MAX;
   V->shadow_min_dist      = FLT_MAX;
   V->shadow_max_dist      = FLT_MAX;
   V->shadow_inner_angle   = FLT_MAX;
   V->shadow_outer_angle   = FLT_MAX;
   V->shadow_outer_volume  = FLT_MAX;
   V->shadow_wet_level     = FLT_MAX;
   V->shadow_dry_level     = FLT_MAX;
   V->shadow_master_dry    = FLT_MAX;
   V->shadow_master_vol    = FLT_MAX;
   V->shadow_auto_3D_atten = -1;
   V->shadow_is_3D         = -1;

   RAD_vector_invalidate(&V->shadow_position);
   RAD_vector_invalidate(&V->shadow_face);
   RAD_vector_invalidate(&V->shadow_velocity);

   //
   // Set up WAVEFORMATEX structure
   //
   // All DS3D voices are 16-bit mono.  Stereo truncation and 8-to-16-bit upconversion are performed
   // at write time
   //

   WAVEFORMATEX wf;

   wf.wFormatTag      = WAVE_FORMAT_PCM;
   wf.nChannels       = 1;
   wf.nSamplesPerSec  = S->playback_rate;
   wf.nAvgBytesPerSec = S->playback_rate * 1 * 2;
   wf.nBlockAlign     = (U16) (1 * 2);
   wf.wBitsPerSample  = (S16) 2 * 8;
   wf.cbSize          = 0;

   //
   // Set up DSBUFFERDESC structure
   //

   DSBUFFERDESC dsbdesc;

   memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));

   dsbdesc.dwSize           =  sizeof(DSBUFFERDESC);

   dsbdesc.dwFlags          =  DSBCAPS_GLOBALFOCUS         |
                               DSBCAPS_CTRL3D              |
                               DSBCAPS_GETCURRENTPOSITION2 |
                               ( ( DRV->dig->D3D.mute_at_max ) ? DSBCAPS_MUTE3DATMAXDISTANCE : 0 ) |
                               DSBCAPS_LOCHARDWARE |
                               DSBCAPS_CTRLVOLUME  |
                               DSBCAPS_CTRLFREQUENCY;

   dsbdesc.dwBufferBytes    =  (BUFF_FRAG_BYTES * TOTAL_FRAGS);
   dsbdesc.lpwfxFormat      = &wf;

   //
   // Create DirectSound buffer (streaming emitter) object
   //

   if (!(SUCCEEDED (DRV->lpDS->CreateSoundBuffer(&dsbdesc,
                                                 &V->lpdsb,
                                                  NULL))))

      {
      AIL_set_error("Could not create secondary DS buffer");

      V->lpdsb = NULL;

      API_unlock();
      return 0;
      }

   //
   // Get the DirectSound3D interface for this buffer
   //

   HRESULT res = V->lpdsb->QueryInterface(IID_IDirectSound3DBuffer,
                              (LPVOID *) &V->lpds3db);

   if (!(SUCCEEDED(res)))
      {
      AIL_set_error("Could not obtain IDirectSound3DBuffer interface");

      V->lpdsb->Release();
      V->lpdsb = NULL;

      API_unlock();
      return 0;
      }

#ifdef EAX3D

   //
   // Get the sound-source property set for this buffer, and make sure
   // it supports the reverb-mix property
   //

   if (FAILED(V->lpds3db->QueryInterface(IID_IKsPropertySet,
                              (void **) &V->lpPropertySet)))
      {
      V->lpPropertySet = NULL;
      }
   else
      {
      DWORD support = 0;

#ifdef EAX2
#define BUFFERPROPSNEEDED DSPROPERTY_EAXBUFFER_ALLPARAMETERS
#else
#define BUFFERPROPSNEEDED DSPROPERTY_EAXBUFFER_REVERBMIX
#endif
      if (FAILED(V->lpPropertySet->QuerySupport(EAXBUFPROP,
                                                BUFFERPROPSNEEDED,
                                               &support))
                 ||
                 ((support & CREVERBBUFFER_SETGET) != CREVERBBUFFER_SETGET))
         {
         V->lpPropertySet->Release();
         V->lpPropertySet = NULL;
         }
      }
#endif

   //
   // Flush newly-allocated voice with silence
   //

   DS_flush_secondary(V);

   //
   // Assign voice to sample
   // This assignment survives future calls to AIL_allocate_sample_handle() / AIL_release_sample_handle()
   //

   S->voice = V;
   V->S     = S;

   V->DRV = DRV;

   V->sample_state = SS_READY_TO_PLAY;

   //
   // Return success
   //

   API_unlock();
   return 2;
}

//############################################################################
//#                                                                          #
//#  Deallocate an HSAMPLE's hardware voice, if it has one                   #
//#                                                                          #
//############################################################################

static void AILCALL VFLT_release_sample_voice (HDRIVERSTATE driver, //)
                                                 HSAMPLE      S)
{
   //
   // Called by AIL_release_sample_handle()
   //
   // We don't release the sample's voice here -- it is reused whenever possible,
   // and released only at shutdown time
   //
}

//############################################################################
//#                                                                          #
//#  Initiate playback of an HSAMPLE via its hardware voice                  #
//#                                                                          #
//############################################################################

static S32 AILCALL VFLT_start_sample_voice (HDRIVERSTATE driver, //)
                                              HSAMPLE      S)
{
   API_lock();

   DSVOICE FAR *V = (DSVOICE FAR *) S->voice;

   if (V == NULL)
      {
      API_unlock();
      return 0;
      }

   CS_enter(&V->lock);

   if (V->sample_state != SS_READY_TO_PLAY)
      {
      //
      // Sample is being restarted before all of the source data copied to its voice buffer
      // has been played.  Try to find another sample whose secondary buffer has already stopped,
      // and swap voices with it
      //
      // OK for this to fail -- if no other samples' voices are idle, we'll just restart the 
      // existing one
      //

      for (S32 i=0; i < V->DRV->dig->n_samples; i++)
         {
         HSAMPLE NS = &V->DRV->dig->samples[i];

         if (NS == S)
            {
            continue;
            }

         DSVOICE FAR *N = (DSVOICE FAR *) NS->voice;

         if (N == NULL)
            {
            continue;
            }

         assert(N != V);

         CS_enter(&N->lock);

         if (N->sample_state == SS_READY_TO_PLAY)
            {
            NS->voice = V;
            S->voice  = N;

            N->S = S;
            V->S = NS;

            CS_leave(&V->lock);
            V = N;

            break;
            }

         CS_leave(&N->lock);
         }
      }

   //
   // Initialize sample voice (without flushing with silence)
   //

   DS_stop_flush_rewind(V, FALSE);

   //
   // Update listener and voice parameters based on HSAMPLE state
   //

#if !HW_LISTENER_POS
   //
   // Make sure the voice's position is updated.  Necessary if the source HSAMPLE is fixed and the
   // listener was moved while another voice was assigned
   //

   RAD_vector_invalidate(&V->shadow_position);
#endif

   DS_update_listener(V->DRV);
   DS_update_voice(V);

   //
   // Write first FILL_AHEAD_FRAGS fragments, leaving V->head_frag at next
   // fragment
   //

   V->head_frag = 0;

   for (S32 i=0; i < FILL_AHEAD_FRAGS; i++)
      {
      DS_stream_to_buffer(V);
      }

   DS_start_secondary(V);
   V->sample_state = SS_PLAYING;

   CS_leave(&V->lock);

   API_unlock();
   return 1;
}

//############################################################################
//#                                                                          #
//# Return FLT filter error text                                             #
//#                                                                          #
//############################################################################

static C8 FAR *       AILCALL VFLT_error       (void)
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

static FLTRESULT AILCALL VFLT_startup     (void)
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

   return FLT_NOERR;
}

//############################################################################
//#                                                                          #
//# Shut down FLT sample filter                                              #
//#                                                                          #
//############################################################################

static FLTRESULT      AILCALL VFLT_shutdown    (void)
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

static HDRIVERSTATE AILCALL VFLT_open_driver (HDIGDRIVER dig)
{
   S32 i;
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
   // Establish default room type for this provider
   //

   DRV->dig = dig;

#ifdef EAX3D
   dig->room_type = EAX_ENVIRONMENT_GENERIC;
#else
   dig->room_type = -1;
#endif

   //
   // Open the hardware
   //

   DRV->n_voices = acquire_hardware(DRV,
                                    MAX_VOICES);

   if (!DRV->n_voices)
      {
      AIL_mem_free_lock(DRV);
      return 0;
      }

   //
   // Allocate and zero voice structures
   //

   S32 vsize = DRV->n_voices * sizeof(DSVOICE);

   DRV->voices = (DSVOICE FAR *) AIL_mem_alloc_lock(vsize);

   if (DRV->voices == NULL)
      {
      strcpy(FLT_error_text,"Out of memory");
      return 0;
      }

   AIL_memset(DRV->voices, 
              0,
              vsize);

   //
   // Zero SAMPLE voice assignments
   //

   for (i=0; i < DRV->n_voices; i++)
      {
      DSVOICE FAR *V = &DRV->voices[i];

      V->sample_state = SS_UNALLOCATED;
      }

   for (i=0; i < DRV->dig->n_samples; i++)
      {
      DRV->dig->samples[i].voice = NULL;
      }

   //
   // Return descriptor address cast to handle
   //

   return (HDRIVERSTATE) DRV;
}

//############################################################################
//#                                                                          #
//# Close filter driver instance                                             #
//#                                                                          #
//############################################################################

static FLTRESULT     AILCALL VFLT_close_driver (HDRIVERSTATE state)
{
   DRIVERSTATE FAR *DRV = (DRIVERSTATE FAR *) state;

   release_hardware(DRV);

   for (S32 i=0; i < DRV->dig->n_samples; i++)
      {
      DRV->dig->samples[i].voice = NULL;
      }

   AIL_mem_free_lock(DRV->voices);
   DRV->voices = NULL;

   AIL_mem_free_lock(DRV);

   return FLT_NOERR;
}

//############################################################################
//#                                                                          #
//# Set driver-level preference value, returning previous setting            #
//#                                                                          #
//############################################################################

static S32 AILCALL VDRV_driver_property(HDRIVERSTATE driver, HPROPERTY property,void FAR * before_value, void const FAR * new_value, void FAR * after_value)
{
   DRIVERSTATE FAR *DRV = (DRIVERSTATE FAR *) driver;

   switch ((PROPERTY) property )
      {
      case MAX_SUPPORTED_SAMPLES: 
        if (before_value) 
        {
          *(S32 FAR*)before_value = DRV->n_voices;
          return 1;
        }
        break;

      case MINIMUM_SAMPLE_BUFFER_SIZE:  
        if (before_value) 
        {
          *(S32 FAR*)before_value = BUFF_FRAG_BYTES * (FILL_AHEAD_FRAGS + 1);    // +1 fragment for safety margin
          return 1;
        }
        break;

      case MUTE_AT_MAX:
       if ( before_value ) *(S32 FAR*)before_value = DRV->dig->D3D.mute_at_max;
       if ( new_value ) DRV->dig->D3D.mute_at_max = *(S32 FAR*)new_value;
       if ( after_value ) *(S32 FAR*)after_value = DRV->dig->D3D.mute_at_max;
       return 1;

#ifdef EAX3D

      //
      // Preferences
      //

      case EAX_ENVIRONMENT:
         if (DRV->lpPropertySet)
         {
           if ( before_value ) *(S32 FAR*)before_value = EAX_GetEnvironment(DRV);
           if ( new_value ) EAX_SetEnvironment(DRV,DRV->dig->room_type = *(U32 const FAR*)new_value );
           if ( after_value ) *(S32 FAR*)after_value = EAX_GetEnvironment(DRV);
           return 1;
         }
         break;

      case EAX_EFFECT_VOLUME:
         if (DRV->lpPropertySet)
         {
           if ( before_value ) *(F32 FAR*)before_value = EAX_GetMasterWet(DRV);
           if ( new_value ) EAX_SetMasterWet(DRV,DRV->dig->master_wet = *(F32 const FAR*)new_value );
           if ( after_value ) *(F32 FAR*)after_value = EAX_GetMasterWet(DRV);
           return 1;
         }
         break;

      case EAX_DECAY_TIME:
         if (DRV->lpPropertySet)
         {
           if ( before_value ) *(F32 FAR*)before_value = EAX_GetDecayTime(DRV);
           if ( new_value ) EAX_SetDecayTime(DRV,DRV->dig->reverb_decay_time_s = *(F32 const FAR*)new_value );
           if ( after_value ) *(F32 FAR*)after_value = EAX_GetDecayTime(DRV);
           return 1;
         }
         break;

      case EAX_DAMPING:
         if (DRV->lpPropertySet)
         {
           if ( before_value ) *(F32 FAR*)before_value = EAX_GetDamping(DRV);
           if ( new_value ) EAX_SetDamping(DRV,DRV->dig->reverb_damping = *(F32 const FAR*)new_value );
           if ( after_value ) *(F32 FAR*)after_value = EAX_GetDamping(DRV);
           return 1;
         }
         break;

      case EAX_ALL_PARAMETERS:
         if (DRV->lpPropertySet)
         {
           if ( before_value ) *(void FAR* FAR*)before_value = EAX_GetAll(DRV,0);
           if ( new_value ) EAX_SetAll(DRV,new_value );
           if ( after_value ) *(void FAR* FAR*)after_value = EAX_GetAll(DRV,1);
           return 1;
         }
         break;

#ifdef EAX2
      case EAX_ENVIRONMENT_SIZE:
       if ( DRV->lpPropertySet )
       {
         if ( before_value ) *(F32 FAR*)before_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_ENVIRONMENTSIZE);
         if ( new_value ) EAX2_Set_float(DRV,DSPROPERTY_EAXLISTENER_ENVIRONMENTSIZE,*(F32 const FAR*)new_value );
         if ( after_value ) *(F32 FAR*)after_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_ENVIRONMENTSIZE);
         return 1;
       }
       break;

      case EAX_ENVIRONMENT_DIFFUSION:
       if ( DRV->lpPropertySet )
       {
         if ( before_value ) *(F32 FAR*)before_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_ENVIRONMENTDIFFUSION);
         if ( new_value ) EAX2_Set_float(DRV,DSPROPERTY_EAXLISTENER_ENVIRONMENTDIFFUSION,*(F32 const FAR*)new_value );
         if ( after_value ) *(F32 FAR*)after_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_ENVIRONMENTDIFFUSION);
         return 1;
       }
       break;

      case EAX_ROOM:
       if ( DRV->lpPropertySet )
       {
         if ( before_value ) *(S32 FAR*)before_value = EAX2_Get_int(DRV,DSPROPERTY_EAXLISTENER_ROOM);
         if ( new_value ) EAX2_Set_int(DRV,DSPROPERTY_EAXLISTENER_ROOM,*(S32 const FAR*)new_value );
         if ( after_value ) *(S32 FAR*)after_value = EAX2_Get_int(DRV,DSPROPERTY_EAXLISTENER_ROOM);
         return 1;
       }
       break;

      case EAX_ROOM_HF:
       if ( DRV->lpPropertySet )
       {
         if ( before_value ) *(S32 FAR*)before_value = EAX2_Get_int(DRV,DSPROPERTY_EAXLISTENER_ROOMHF);
         if ( new_value ) EAX2_Set_int(DRV,DSPROPERTY_EAXLISTENER_ROOMHF,*(S32 const FAR*)new_value );
         if ( after_value ) *(S32 FAR*)after_value = EAX2_Get_int(DRV,DSPROPERTY_EAXLISTENER_ROOMHF);
         return 1;
       }
       break;

      case EAX_DECAY_HF_RATIO:
       if ( DRV->lpPropertySet )
       {
         if ( before_value ) *(F32 FAR*)before_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_DECAYHFRATIO);
         if ( new_value ) EAX2_Set_float(DRV,DSPROPERTY_EAXLISTENER_DECAYHFRATIO,*(F32 const FAR*)new_value );
         if ( after_value ) *(F32 FAR*)after_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_DECAYHFRATIO);
         return 1;
       }
       break;

      case EAX_REFLECTIONS:
       if ( DRV->lpPropertySet )
       {
         if ( before_value ) *(S32 FAR*)before_value = EAX2_Get_int(DRV,DSPROPERTY_EAXLISTENER_REFLECTIONS);
         if ( new_value ) EAX2_Set_int(DRV,DSPROPERTY_EAXLISTENER_REFLECTIONS,*(S32 const FAR*)new_value );
         if ( after_value ) *(S32 FAR*)after_value = EAX2_Get_int(DRV,DSPROPERTY_EAXLISTENER_REFLECTIONS);
         return 1;
       }
       break;

      case EAX_REFLECTIONS_DELAY:
       if ( DRV->lpPropertySet )
       {
         if ( before_value ) *(F32 FAR*)before_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_REFLECTIONSDELAY);
         if ( new_value ) EAX2_Set_float(DRV,DSPROPERTY_EAXLISTENER_REFLECTIONSDELAY,*(F32 const FAR*)new_value );
         if ( after_value ) *(F32 FAR*)after_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_REFLECTIONSDELAY);
         return 1;
       }
       break;

      case EAX_REVERB_P:
       if ( DRV->lpPropertySet )
       {
         if ( before_value ) *(S32 FAR*)before_value = EAX2_Get_int(DRV,DSPROPERTY_EAXLISTENER_REVERB);
         if ( new_value ) EAX2_Set_int(DRV,DSPROPERTY_EAXLISTENER_REVERB,*(S32 const FAR*)new_value );
         if ( after_value ) *(S32 FAR*)after_value = EAX2_Get_int(DRV,DSPROPERTY_EAXLISTENER_REVERB);
         return 1;
       }
       break;

      case EAX_REVERB_DELAY:
       if ( DRV->lpPropertySet )
       {
         if ( before_value ) *(F32 FAR*)before_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_REVERBDELAY);
         if ( new_value ) EAX2_Set_float(DRV,DSPROPERTY_EAXLISTENER_REVERBDELAY,*(F32 const FAR*)new_value );
         if ( after_value ) *(F32 FAR*)after_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_REVERBDELAY);
         return 1;
       }
       break;

      case EAX_ROOM_ROLLOFF:
       if ( DRV->lpPropertySet )
       {
         if ( before_value ) *(F32 FAR*)before_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_ROOMROLLOFFFACTOR);
         if ( new_value ) EAX2_Set_float(DRV,DSPROPERTY_EAXLISTENER_ROOMROLLOFFFACTOR,*(F32 const FAR*)new_value );
         if ( after_value ) *(F32 FAR*)after_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_ROOMROLLOFFFACTOR);
         return 1;
       }
       break;

      case EAX_AIR_ABSORPTION:
       if ( DRV->lpPropertySet )
       {
         if ( before_value ) *(F32 FAR*)before_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_AIRABSORPTIONHF);
         if ( new_value ) EAX2_Set_float(DRV,DSPROPERTY_EAXLISTENER_AIRABSORPTIONHF,*(F32 const FAR*)new_value );
         if ( after_value ) *(F32 FAR*)after_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_AIRABSORPTIONHF);
         return 1;
       }
       break;

      case EAX_FLAGS:
       if ( DRV->lpPropertySet )
       {
         if ( before_value ) *(S32 FAR*)before_value = EAX2_Get_int(DRV,DSPROPERTY_EAXLISTENER_FLAGS);
         if ( new_value ) EAX2_Set_int(DRV,DSPROPERTY_EAXLISTENER_FLAGS,*(S32 const FAR*)new_value );
         if ( after_value ) *(S32 FAR*)after_value = EAX2_Get_int(DRV,DSPROPERTY_EAXLISTENER_FLAGS);
         return 1;
       }
       break;


#ifdef EAX3
      case EAX_ROOM_LF:
       if ( DRV->lpPropertySet )
       {
         if ( before_value ) *(S32 FAR*)before_value = EAX2_Get_int(DRV,DSPROPERTY_EAXLISTENER_ROOMLF);
         if ( new_value ) EAX2_Set_int(DRV,DSPROPERTY_EAXLISTENER_ROOMLF,*(S32 const FAR*)new_value );
         if ( after_value ) *(S32 FAR*)after_value = EAX2_Get_int(DRV,DSPROPERTY_EAXLISTENER_ROOMLF);
         return 1;
       }
       break;

      case EAX_DECAY_LF_RATIO:
       if ( DRV->lpPropertySet )
       {
         if ( before_value ) *(F32 FAR*)before_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_DECAYLFRATIO);
         if ( new_value ) EAX2_Set_float(DRV,DSPROPERTY_EAXLISTENER_DECAYLFRATIO,*(F32 const FAR*)new_value );
         if ( after_value ) *(F32 FAR*)after_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_DECAYLFRATIO);
         return 1;
       }
       break;

      case EAX_REFLECTIONS_PAN:
       if ( DRV->lpPropertySet )
       {
         if ( before_value ) *(F32 FAR*)before_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_REFLECTIONSPAN);
         if ( new_value ) EAX2_Set_float(DRV,DSPROPERTY_EAXLISTENER_REFLECTIONSPAN,*(F32 const FAR*)new_value );
         if ( after_value ) *(F32 FAR*)after_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_REFLECTIONSPAN);
         return 1;
       }
       break;

      case EAX_REVERB_PAN:
       if ( DRV->lpPropertySet )
       {
         if ( before_value ) *(F32 FAR*)before_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_REVERBPAN);
         if ( new_value ) EAX2_Set_float(DRV,DSPROPERTY_EAXLISTENER_REVERBPAN,*(F32 const FAR*)new_value );
         if ( after_value ) *(F32 FAR*)after_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_REVERBPAN);
         return 1;
       }
       break;

      case EAX_ECHO_TIME: 
       if ( DRV->lpPropertySet )
       {
         if ( before_value ) *(F32 FAR*)before_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_ECHOTIME);
         if ( new_value ) EAX2_Set_float(DRV,DSPROPERTY_EAXLISTENER_ECHOTIME,*(F32 const FAR*)new_value );
         if ( after_value ) *(F32 FAR*)after_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_ECHOTIME);
         return 1;
       }
       break;

      case EAX_ECHO_DEPTH:
       if ( DRV->lpPropertySet )
       {
         if ( before_value ) *(F32 FAR*)before_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_ECHODEPTH);
         if ( new_value ) EAX2_Set_float(DRV,DSPROPERTY_EAXLISTENER_ECHODEPTH,*(F32 const FAR*)new_value );
         if ( after_value ) *(F32 FAR*)after_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_ECHODEPTH);
         return 1;
       }
       break;

      case EAX_MODULATION_TIME:
       if ( DRV->lpPropertySet )
       {
         if ( before_value ) *(F32 FAR*)before_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_MODULATIONTIME);
         if ( new_value ) EAX2_Set_float(DRV,DSPROPERTY_EAXLISTENER_MODULATIONTIME,*(F32 const FAR*)new_value );
         if ( after_value ) *(F32 FAR*)after_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_MODULATIONTIME);
         return 1;
       }
       break;

      case EAX_MODULATION_DEPTH:
       if ( DRV->lpPropertySet )
       {
         if ( before_value ) *(F32 FAR*)before_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_MODULATIONDEPTH);
         if ( new_value ) EAX2_Set_float(DRV,DSPROPERTY_EAXLISTENER_MODULATIONDEPTH,*(F32 const FAR*)new_value );
         if ( after_value ) *(F32 FAR*)after_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_MODULATIONDEPTH);
         return 1;
       }
       break;

      case EAX_HF_REFERENCE:
       if ( DRV->lpPropertySet )
       {
         if ( before_value ) *(F32 FAR*)before_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_HFREFERENCE);
         if ( new_value ) EAX2_Set_float(DRV,DSPROPERTY_EAXLISTENER_HFREFERENCE,*(F32 const FAR*)new_value );
         if ( after_value ) *(F32 FAR*)after_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_HFREFERENCE);
         return 1;
       }
       break;

      case EAX_LF_REFERENCE:
       if ( DRV->lpPropertySet )
       {
         if ( before_value ) *(F32 FAR*)before_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_LFREFERENCE);
         if ( new_value ) EAX2_Set_float(DRV,DSPROPERTY_EAXLISTENER_LFREFERENCE,*(F32 const FAR*)new_value );
         if ( after_value ) *(F32 FAR*)after_value = EAX2_Get_float(DRV,DSPROPERTY_EAXLISTENER_LFREFERENCE);
         return 1;
       }
       break;


#ifdef EAX4
      case EAX_SLOT_0:
       if ( DRV->lpPropertySet )
       {
         if ( before_value ) *(void FAR* FAR*)before_value = get_eax4(DRV,0,0);
         if ( new_value ) if ( set_eax4(DRV, 0, new_value ) == 0 ) return 0;
         if ( after_value ) *(void FAR* FAR*)after_value = get_eax4(DRV,0,1);
         return 1;
       }
       break;

      case EAX_SLOT_1:
       if ( DRV->lpPropertySet )
       {
         if ( before_value ) *(void FAR* FAR*)before_value = get_eax4(DRV,1,0);
         if ( new_value ) if ( set_eax4(DRV, 1, new_value ) == 0 ) return 0;
         if ( after_value ) *(void FAR* FAR*)after_value = get_eax4(DRV,1,1);
         return 1;
       }
       break;

      case EAX_SLOT_2:
       if ( DRV->lpPropertySet )
       {
         if ( before_value ) *(void FAR* FAR*)before_value = get_eax4(DRV,2,0);
         if ( new_value ) if ( set_eax4(DRV, 2, new_value ) == 0 ) return 0;
         if ( after_value ) *(void FAR* FAR*)after_value = get_eax4(DRV,2,1);
         return 1;
       }
       break;

      case EAX_SLOT_3:
       if ( DRV->lpPropertySet )
       {
         if ( before_value ) *(void FAR* FAR*)before_value = get_eax4(DRV,3,0);
         if ( new_value ) if ( set_eax4(DRV, 3, new_value ) == 0 ) return 0;
         if ( after_value ) *(void FAR* FAR*)after_value = get_eax4(DRV,3,1);
         return 1;
       }
       break;
#endif

#endif

#endif

#endif
      }

   return 0;
}

//############################################################################
//#                                                                          #
//# Retrieve a sample property by index                                      #
//#                                                                          #
//############################################################################

static S32 AILCALL VSMP_sample_property(HSAMPLE S, HPROPERTY property,void FAR * before_value, void const FAR * new_value, void FAR * after_value)
{
   DSVOICE FAR *V = (DSVOICE FAR *) S->voice;

   if (V == NULL)
      {
      return -1;
      }

   switch (property)
      {
      case DIRECTSOUND_SAMPLE_BUFFER:  
        if (before_value) 
        {
          *(LPDIRECTSOUND3DBUFFER FAR*)before_value = V->lpds3db;
          return 1;
        }
        break;

        
#ifdef EAX3D

      case EAX_PROPERTY_SET:
        if (before_value) 
        {
          *(LPKSPROPERTYSET FAR*)before_value = V->lpPropertySet;
          return 1;
        }
        break;

      case EAX_EFFECT_ALL_PARAMETERS: 
       if ( V->lpPropertySet )
       {
         if ( before_value ) *(void FAR* FAR*)before_value = EAX_GetSampleAll(V,0);
         if ( new_value ) EAX_SetSampleAll(V,new_value);
         if ( after_value ) *(void FAR* FAR*)after_value = EAX_GetSampleAll(V,1);
         return 1;
       }
       break;
 
#ifdef EAX2
      case EAX_SAMPLE_DIRECT:
       if ( V->lpPropertySet )
       {
         if ( before_value ) *(S32 FAR*)before_value = EAX2_Sample_Get_int(V,DSPROPERTY_EAXBUFFER_DIRECT);
         if ( new_value ) EAX2_Sample_Set_int(V,DSPROPERTY_EAXBUFFER_DIRECT,*(S32 const FAR*)new_value );
         if ( after_value ) *(S32 FAR*)after_value = EAX2_Sample_Get_int(V,DSPROPERTY_EAXBUFFER_DIRECT);
         return 1;
       }
       break;
       
      case EAX_SAMPLE_DIRECT_HF:
       if ( V->lpPropertySet )
       {
         if ( before_value ) *(S32 FAR*)before_value = EAX2_Sample_Get_int(V,DSPROPERTY_EAXBUFFER_DIRECTHF);
         if ( new_value ) EAX2_Sample_Set_int(V,DSPROPERTY_EAXBUFFER_DIRECTHF,*(S32 const FAR*)new_value );
         if ( after_value ) *(S32 FAR*)after_value = EAX2_Sample_Get_int(V,DSPROPERTY_EAXBUFFER_DIRECTHF);
         return 1;
       }
       break;
       
      case EAX_SAMPLE_ROOM: 
       if ( V->lpPropertySet )
       {
         if ( before_value ) *(S32 FAR*)before_value = EAX2_Sample_Get_int(V,DSPROPERTY_EAXBUFFER_ROOM);
         if ( new_value ) EAX2_Sample_Set_int(V,DSPROPERTY_EAXBUFFER_ROOM,*(S32 const FAR*)new_value );
         if ( after_value ) *(S32 FAR*)after_value = EAX2_Sample_Get_int(V,DSPROPERTY_EAXBUFFER_ROOM);
         return 1;
       }
       break;
       
      case EAX_SAMPLE_ROOM_HF:
       if ( V->lpPropertySet )
       {
         if ( before_value ) *(S32 FAR*)before_value = EAX2_Sample_Get_int(V,DSPROPERTY_EAXBUFFER_ROOMHF);
         if ( new_value ) EAX2_Sample_Set_int(V,DSPROPERTY_EAXBUFFER_ROOMHF,*(S32 const FAR*)new_value );
         if ( after_value ) *(S32 FAR*)after_value = EAX2_Sample_Get_int(V,DSPROPERTY_EAXBUFFER_ROOMHF);
         return 1;
       }
       break;
       
      case EAX_SAMPLE_OBSTRUCTION:
       if ( V->lpPropertySet )
       {
         if ( before_value ) *(S32 FAR*)before_value = EAX2_Sample_Get_int(V,DSPROPERTY_EAXBUFFER_OBSTRUCTION);
         if ( new_value ) EAX2_Sample_Set_int(V,DSPROPERTY_EAXBUFFER_OBSTRUCTION,*(S32 const FAR*)new_value );
         if ( after_value ) *(S32 FAR*)after_value = EAX2_Sample_Get_int(V,DSPROPERTY_EAXBUFFER_OBSTRUCTION);
         return 1;
       }
       break;
       
      case EAX_SAMPLE_OBSTRUCTION_LF_RATIO:
       if ( V->lpPropertySet )
       {
         if ( before_value ) *(F32 FAR*)before_value = EAX2_Sample_Get_float(V,DSPROPERTY_EAXBUFFER_OBSTRUCTIONLFRATIO);
         if ( new_value ) EAX2_Sample_Set_float(V,DSPROPERTY_EAXBUFFER_OBSTRUCTIONLFRATIO,*(F32 const FAR*)new_value );
         if ( after_value ) *(F32 FAR*)after_value = EAX2_Sample_Get_float(V,DSPROPERTY_EAXBUFFER_OBSTRUCTIONLFRATIO);
         return 1;
       }
       break;
       
      case EAX_SAMPLE_OCCLUSION:
       if ( V->lpPropertySet )
       {
         if ( before_value ) *(S32 FAR*)before_value = EAX2_Sample_Get_int(V,DSPROPERTY_EAXBUFFER_OCCLUSION);
         if ( new_value ) EAX2_Sample_Set_int(V,DSPROPERTY_EAXBUFFER_OCCLUSION,*(S32 const FAR*)new_value );
         if ( after_value ) *(S32 FAR*)after_value = EAX2_Sample_Get_int(V,DSPROPERTY_EAXBUFFER_ROOM);
         return 1;
       }
       break;
       
      case EAX_SAMPLE_OCCLUSION_LF_RATIO:  
       if ( V->lpPropertySet )
       {
         if ( before_value ) *(F32 FAR*)before_value = EAX2_Sample_Get_float(V,DSPROPERTY_EAXBUFFER_OCCLUSIONLFRATIO);
         if ( new_value ) EAX2_Sample_Set_float(V,DSPROPERTY_EAXBUFFER_OCCLUSIONLFRATIO,*(F32 const FAR*)new_value );
         if ( after_value ) *(F32 FAR*)after_value = EAX2_Sample_Get_float(V,DSPROPERTY_EAXBUFFER_OCCLUSIONLFRATIO);
         return 1;
       }
       break;
       
      case EAX_SAMPLE_OCCLUSION_ROOM_RATIO:
       if ( V->lpPropertySet )
       {
         if ( before_value ) *(F32 FAR*)before_value = EAX2_Sample_Get_float(V,DSPROPERTY_EAXBUFFER_OCCLUSIONROOMRATIO);
         if ( new_value ) EAX2_Sample_Set_float(V,DSPROPERTY_EAXBUFFER_OCCLUSIONROOMRATIO,*(F32 const FAR*)new_value );
         if ( after_value ) *(F32 FAR*)after_value = EAX2_Sample_Get_float(V,DSPROPERTY_EAXBUFFER_OCCLUSIONROOMRATIO);
         return 1;
       }
       break;
       
      case EAX_SAMPLE_ROOM_ROLLOFF:
       if ( V->lpPropertySet )
       {
         if ( before_value ) *(F32 FAR*)before_value = EAX2_Sample_Get_float(V,DSPROPERTY_EAXBUFFER_ROOMROLLOFFFACTOR);
         if ( new_value ) EAX2_Sample_Set_float(V,DSPROPERTY_EAXBUFFER_ROOMROLLOFFFACTOR,*(F32 const FAR*)new_value );
         if ( after_value ) *(F32 FAR*)after_value = EAX2_Sample_Get_float(V,DSPROPERTY_EAXBUFFER_ROOMROLLOFFFACTOR);
         return 1;
       }
       break;
       
      case EAX_SAMPLE_AIR_ABSORPTION: 
       if ( V->lpPropertySet )
       {
         if ( before_value ) *(F32 FAR*)before_value = EAX2_Sample_Get_float(V,DSPROPERTY_EAXBUFFER_AIRABSORPTIONFACTOR);
         if ( new_value ) EAX2_Sample_Set_float(V,DSPROPERTY_EAXBUFFER_AIRABSORPTIONFACTOR,*(F32 const FAR*)new_value );
         if ( after_value ) *(F32 FAR*)after_value = EAX2_Sample_Get_float(V,DSPROPERTY_EAXBUFFER_AIRABSORPTIONFACTOR);
         return 1;
       }
       break;
       
      case EAX_SAMPLE_OUTSIDE_VOLUME_HF:
       if ( V->lpPropertySet )
       {
         if ( before_value ) *(S32 FAR*)before_value = EAX2_Sample_Get_int(V,DSPROPERTY_EAXBUFFER_OUTSIDEVOLUMEHF);
         if ( new_value ) EAX2_Sample_Set_int(V,DSPROPERTY_EAXBUFFER_OUTSIDEVOLUMEHF,*(S32 const FAR*)new_value );
         if ( after_value ) *(S32 FAR*)after_value = EAX2_Sample_Get_int(V,DSPROPERTY_EAXBUFFER_OUTSIDEVOLUMEHF);
         return 1;
       }
       break;
       
      case EAX_SAMPLE_FLAGS:
       if ( V->lpPropertySet )
       {
         if ( before_value ) *(S32 FAR*)before_value = EAX2_Sample_Get_int(V,DSPROPERTY_EAXBUFFER_FLAGS);
         if ( new_value ) EAX2_Sample_Set_int(V,DSPROPERTY_EAXBUFFER_FLAGS,*(S32 const FAR*)new_value );
         if ( after_value ) *(S32 FAR*)after_value = EAX2_Sample_Get_int(V,DSPROPERTY_EAXBUFFER_FLAGS);
         return 1;
       }
       break;
       

#ifdef EAX3
      case EAX_SAMPLE_OCCLUSION_DIRECT_RATIO:
       if ( V->lpPropertySet )
       {
         if ( before_value ) *(F32 FAR*)before_value = EAX2_Sample_Get_float(V,DSPROPERTY_EAXBUFFER_OCCLUSIONDIRECTRATIO);
         if ( new_value ) EAX2_Sample_Set_float(V,DSPROPERTY_EAXBUFFER_OCCLUSIONDIRECTRATIO,*(F32 const FAR*)new_value );
         if ( after_value ) *(F32 FAR*)after_value = EAX2_Sample_Get_float(V,DSPROPERTY_EAXBUFFER_OCCLUSIONDIRECTRATIO);
         return 1;
       }
       break;
       
      case EAX_SAMPLE_EXCLUSION:
       if ( V->lpPropertySet )
       {
         if ( before_value ) *(S32 FAR*)before_value = EAX2_Sample_Get_int(V,DSPROPERTY_EAXBUFFER_EXCLUSION);
         if ( new_value ) EAX2_Sample_Set_int(V,DSPROPERTY_EAXBUFFER_EXCLUSION,*(S32 const FAR*)new_value );
         if ( after_value ) *(S32 FAR*)after_value = EAX2_Sample_Get_int(V,DSPROPERTY_EAXBUFFER_EXCLUSION);
         return 1;
       }
       break;
       
      case EAX_SAMPLE_EXCLUSION_LF_RATIO:
       if ( V->lpPropertySet )
       {
         if ( before_value ) *(F32 FAR*)before_value = EAX2_Sample_Get_float(V,DSPROPERTY_EAXBUFFER_EXCLUSIONLFRATIO);
         if ( new_value ) EAX2_Sample_Set_float(V,DSPROPERTY_EAXBUFFER_EXCLUSIONLFRATIO,*(F32 const FAR*)new_value );
         if ( after_value ) *(F32 FAR*)after_value = EAX2_Sample_Get_float(V,DSPROPERTY_EAXBUFFER_EXCLUSIONLFRATIO);
         return 1;
       }
       break;
       
      case EAX_SAMPLE_DOPPLER:
       if ( V->lpPropertySet )
       {
         if ( before_value ) *(F32 FAR*)before_value = EAX2_Sample_Get_float(V,DSPROPERTY_EAXBUFFER_DOPPLERFACTOR);
         if ( new_value ) EAX2_Sample_Set_float(V,DSPROPERTY_EAXBUFFER_DOPPLERFACTOR,*(F32 const FAR*)new_value );
         if ( after_value ) *(F32 FAR*)after_value = EAX2_Sample_Get_float(V,DSPROPERTY_EAXBUFFER_DOPPLERFACTOR);
         return 1;
       }
       break;

#ifdef EAX4
      case EAX_SAMPLE_SLOT_VOLUMES_P:
       if ( V->lpPropertySet )
       {
         if ( before_value ) *(void FAR* FAR*)before_value = get_eax4_sample_volumes( V, 0 );
         if ( new_value ) set_eax4_sample_volumes(V,new_value);
         if ( after_value ) *(void FAR* FAR*)after_value = get_eax4_sample_volumes( V, 1 );
         return 1;
       }
       break;
#endif       
#endif
#endif
#endif

      }

   return 0;
}


//############################################################################
//#                                                                          #
//# DLLMain registers RIB API at load time                                   #
//#                                                                          #
//############################################################################

#ifdef EAX4
extern "C" S32 EAX4Main( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 EAX4Main( HPROVIDER provider_handle, U32 up_down )
#else
#ifdef EAX3
extern "C" S32 EAX3Main( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 EAX3Main( HPROVIDER provider_handle, U32 up_down )
#else
#ifdef EAX2
extern "C" S32 EAX2Main( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 EAX2Main( HPROVIDER provider_handle, U32 up_down )
#else
#ifdef EAX3D
extern "C" S32 EAXMain( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 EAXMain( HPROVIDER provider_handle, U32 up_down )
#else
extern "C" S32 hwMain( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 hwMain( HPROVIDER provider_handle, U32 up_down )
#endif
#endif
#endif
#endif
{
   const RIB_INTERFACE_ENTRY VFLT[] =
      {
      REG_FN(PROVIDER_property),
      REG_PR("Name",                        PROVIDER_NAME,              (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_STRING)),
      REG_PR("Version",                     PROVIDER_VERSION,           (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_HEX)),
      REG_PR("Flags",                       PROVIDER_FLAGS,             (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_HEX)),

      REG_FN(VFLT_startup),
      REG_FN(VFLT_error),
      REG_FN(VFLT_shutdown),

      REG_FN(VFLT_open_driver),
      REG_FN(VFLT_close_driver),

      REG_FN(VFLT_assign_sample_voice),
      REG_FN(VFLT_release_sample_voice),
      REG_FN(VFLT_start_sample_voice),
      };

   const RIB_INTERFACE_ENTRY VDRV[] =
      {
      REG_PR("Maximum supported samples",   MAX_SUPPORTED_SAMPLES,      (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_DEC)),
      REG_PR("Minimum sample buffer size",  MINIMUM_SAMPLE_BUFFER_SIZE, (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_DEC)),
      REG_PR("Mute at maximum",             MUTE_AT_MAX,                RIB_DEC),

#ifdef EAX3D
      REG_PR("EAX property set",            EAX_PROPERTY_SET,           (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_DEC)),

      REG_PR("EAX environment selection",   EAX_ENVIRONMENT,            RIB_DEC),
      REG_PR("EAX effect volume",           EAX_EFFECT_VOLUME,          RIB_FLOAT),
      REG_PR("EAX decay time",              EAX_DECAY_TIME,             RIB_FLOAT),
      REG_PR("EAX damping",                 EAX_DAMPING,                RIB_FLOAT),
      REG_PR("EAX all parameters",          EAX_ALL_PARAMETERS,         RIB_DEC),

#ifdef EAX2
      REG_PR("EAX2 environment size",       EAX_ENVIRONMENT_SIZE,       RIB_FLOAT),
      REG_PR("EAX2 environment diffusion",  EAX_ENVIRONMENT_DIFFUSION,  RIB_FLOAT),
      REG_PR("EAX2 room",                   EAX_ROOM,                   RIB_DEC),
      REG_PR("EAX2 room HF",                EAX_ROOM_HF,                RIB_DEC),
      REG_PR("EAX2 decay HF ratio",         EAX_DECAY_HF_RATIO,         RIB_FLOAT),
      REG_PR("EAX2 reflections",            EAX_REFLECTIONS,            RIB_DEC),
      REG_PR("EAX2 reflections delay",      EAX_REFLECTIONS_DELAY,      RIB_FLOAT),
      REG_PR("EAX2 reverb",                 EAX_REVERB_P,               RIB_DEC),
      REG_PR("EAX2 reverb delay",           EAX_REVERB_DELAY,           RIB_FLOAT),
      REG_PR("EAX2 room rolloff",           EAX_ROOM_ROLLOFF,           RIB_FLOAT),
      REG_PR("EAX2 air absorption",         EAX_AIR_ABSORPTION,         RIB_FLOAT),
      REG_PR("EAX2 flags",                  EAX_FLAGS,                  RIB_DEC),

#ifdef EAX3
      REG_PR("EAX3 room LF",                EAX_ROOM_LF,                RIB_DEC),
      REG_PR("EAX3 decay LF ratio",         EAX_DECAY_LF_RATIO,         RIB_FLOAT),
      REG_PR("EAX3 reflections pan",        EAX_REFLECTIONS_PAN,        RIB_FLOAT),
      REG_PR("EAX3 reverb pan",             EAX_REVERB_PAN,             RIB_FLOAT),
      REG_PR("EAX3 echo time",              EAX_ECHO_TIME,              RIB_FLOAT),
      REG_PR("EAX3 echo depth",             EAX_ECHO_DEPTH,             RIB_FLOAT),
      REG_PR("EAX3 modulation time",        EAX_MODULATION_TIME,        RIB_FLOAT),
      REG_PR("EAX3 modulation depth",       EAX_MODULATION_DEPTH,       RIB_FLOAT),
      REG_PR("EAX3 HF reference",           EAX_HF_REFERENCE,           RIB_FLOAT),
      REG_PR("EAX3 LF reference",           EAX_LF_REFERENCE,           RIB_FLOAT),
#endif

#ifdef EAX4
      REG_PR("EAX4 Effects Slot 0",             EAX_SLOT_0,          RIB_DEC),
      REG_PR("EAX4 Effects Slot 1",             EAX_SLOT_1,          RIB_DEC),
      REG_PR("EAX4 Effects Slot 2",             EAX_SLOT_2,          RIB_DEC),
      REG_PR("EAX4 Effects Slot 3",             EAX_SLOT_3,          RIB_DEC),
 #endif
#endif
#endif
      REG_FN(VDRV_driver_property),
      REG_FN(VDRV_force_update),
      };

   const RIB_INTERFACE_ENTRY VSMP[] =
      {
#ifdef EAX3D
      REG_PR("EAX sample all parameters",        EAX_EFFECT_ALL_PARAMETERS,       RIB_DEC),

      REG_PR("EAX sample property set",          EAX_PROPERTY_SET,                (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_DEC)),

#ifdef EAX2
      REG_PR("EAX2 sample direct",               EAX_SAMPLE_DIRECT,               RIB_DEC  ),
      REG_PR("EAX2 sample direct HF",            EAX_SAMPLE_DIRECT_HF,            RIB_DEC  ),
      REG_PR("EAX2 sample room",                 EAX_SAMPLE_ROOM,                 RIB_DEC  ),
      REG_PR("EAX2 sample room HF",              EAX_SAMPLE_ROOM_HF,              RIB_DEC  ),
      REG_PR("EAX2 sample obstruction",          EAX_SAMPLE_OBSTRUCTION,          RIB_DEC  ),
      REG_PR("EAX2 sample obstruction LF ratio", EAX_SAMPLE_OBSTRUCTION_LF_RATIO, RIB_FLOAT),
      REG_PR("EAX2 sample occlusion",            EAX_SAMPLE_OCCLUSION,            RIB_DEC  ),
      REG_PR("EAX2 sample occlusion LF ratio",   EAX_SAMPLE_OCCLUSION_LF_RATIO,   RIB_FLOAT),
      REG_PR("EAX2 sample occlusion room ratio", EAX_SAMPLE_OCCLUSION_ROOM_RATIO, RIB_FLOAT),
      REG_PR("EAX2 sample room rolloff",         EAX_SAMPLE_ROOM_ROLLOFF,         RIB_FLOAT),
      REG_PR("EAX2 sample air absorption",       EAX_SAMPLE_AIR_ABSORPTION,       RIB_FLOAT),
      REG_PR("EAX2 sample outside volume HF",    EAX_SAMPLE_OUTSIDE_VOLUME_HF,    RIB_DEC  ),
      REG_PR("EAX2 sample flags",                EAX_SAMPLE_FLAGS,                RIB_DEC  ),

#ifdef EAX3
      REG_PR("EAX3 sample occlusion direct ratio", EAX_SAMPLE_OCCLUSION_DIRECT_RATIO, RIB_FLOAT),
      REG_PR("EAX3 sample exclusion",            EAX_SAMPLE_EXCLUSION,              RIB_DEC  ),
      REG_PR("EAX3 sample exclusion LF ratio",   EAX_SAMPLE_EXCLUSION_LF_RATIO,     RIB_FLOAT),
      REG_PR("EAX3 sample doppler factor",       EAX_SAMPLE_DOPPLER,                RIB_FLOAT),
#endif

#ifdef EAX4
      REG_PR("EAX4 slot volumes",                EAX_SAMPLE_SLOT_VOLUMES_P,         RIB_DEC),
#endif

#endif

#endif

      REG_PR("DirectSound sample buffer",        DIRECTSOUND_SAMPLE_BUFFER,         (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_DEC)),

      REG_FN(VSMP_sample_property),
   };

   if (up_down)
      {
      RIB_register(provider_handle,
                  "MSS voice filter",
                   VFLT);

      RIB_register(provider_handle,
                  "Voice filter driver services",
                   VDRV);

      RIB_register(provider_handle,
                  "Voice filter sample services",
                   VSMP);
      } 
   else 
      {
      RIB_unregister_all(provider_handle);
      }

   return TRUE;
}

#ifdef EAX4
extern "C" S32 EAX3Main( HPROVIDER provider_handle, U32 up_down );
extern "C" S32 EAX2Main( HPROVIDER provider_handle, U32 up_down );
//extern "C" S32 EAXMain( HPROVIDER provider_handle, U32 up_down );

DXDEF S32 AILCALL RIB_MAIN_NAME(EAX)( HPROVIDER provider_handle, U32 up_down )
{
  static HPROVIDER EAXPH, EAX2PH, EAX3PH;

  if ( up_down )
  {
    EAX3PH = RIB_alloc_provider_handle(0);
    EAX2PH = RIB_alloc_provider_handle(0);
    EAXPH = RIB_alloc_provider_handle(0);
  }

  EAX4Main( provider_handle, up_down );
  EAX3Main( EAX3PH, up_down );
  EAX2Main( EAX2PH, up_down );
//  EAXMain( EAXPH, up_down );

  return( TRUE );
}

#else 
#ifndef EAX3D     // non-EAX case 

extern "C" S32 hwMain( HPROVIDER provider_handle, U32 up_down );

DXDEF S32 AILEXPORT RIB_Main( HPROVIDER provider_handle, U32 up_down )
{
  hwMain( provider_handle, up_down );
  return( TRUE );
}

#endif
#endif

