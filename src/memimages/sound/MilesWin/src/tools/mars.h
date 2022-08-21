//############################################################################
//##                                                                        ##
//##  MARS.H                                                                ##
//##                                                                        ##
//##  Miles Audio Rendering System                                          ##
//##                                                                        ##
//##  jmiles@pop.net                                                        ##
//##                                                                        ##
//############################################################################
//##                                                                        ##
//##  Copyright (C) RAD Game Tools, Inc.                                    ##
//##                                                                        ##
//##  For technical support, contact RAD Game Tools at 425-893-4300.        ##
//##                                                                        ##
//############################################################################

#ifndef MSSMARS_VERSION

#define MSSMARS_VERSION      "7.0s"     // Version string needs to be 4 characters long
#define MSSMARS_MAJOR_VERSION 7
#define MSSMARS_MINOR_VERSION 0
#define MSSMARS_SUB_VERSION   19
#define MSSMARS_VERSION_DATE "04-Oct-06"

#define MSSMARS_COPYRIGHT "Copyright (C) 1991-2006, RAD Game Tools, Inc."

#endif

#ifndef MSSMARS_H
#define MSSMARS_H

#ifndef MSS_H
#include "mss.h"  // TODO: Eventually remove all AIL dependencies
#endif

//
// Constants
//

#define MARS_MAX_PROP_TEXT_LEN 512
#define MARS_MAX_PROP_BIN_LEN  64

//
// MARS objects
//

struct SNDDEF;
struct MENV;

typedef MSS_STRUCT SNDDEF *HMSOUND;   // Handle to sound definition
typedef MSS_STRUCT MENV   *HMENV;     // Handle to sound definition

typedef void (AILCALLBACK *MARS_SOUND_CALLBACK)   (HMSOUND sound);
typedef F32  (AILCALLBACK *MARS_FALLOFF_CALLBACK) (HMSOUND sound, F32 distance, F32 rolloff_factor, F32 min_dist, F32 max_dist);

//
// Sound processor stages
//

typedef enum
{
   MARS_FILTER_0 = 1,             // These values must match SAMPLESTAGE equates
   MARS_FILTER_1,
   MARS_FILTER_2,
   MARS_FILTER_3,
   MARS_FILTER_4,
   MARS_FILTER_5,
   MARS_FILTER_6,
   MARS_FILTER_7,
}
MARS_SAMPLE_STAGE;

//
// Result codes for most API functions
//

typedef enum
{
   MARS_OK = 0,                  // 0
   MARS_DONE,                    // 1
   MARS_GENERAL_ERROR,           // 2
   MARS_NOT_FOUND,               // 3
   MARS_OUT_OF_RANGE,            // 4
   MARS_NOT_WRITABLE,            // 5
   MARS_BAD_PARAM,               // 6 
   MARS_BAD_PROPERTY,            // 7 
   MARS_RESOURCE_UNAVAILABLE,    // 8 
   MARS_OUT_OF_MEMORY,           // 9 
   MARS_DRIVER_FAILURE,          // 10 
   MARS_UNSUPPORTED_FORMAT,      // 11 
   MARS_NOT_DEFERRABLE,          // 12 
   MARS_FILE_NOT_FOUND,          // 13 
   MARS_FILE_NOT_READABLE,       // 14 
   MARS_FILE_NOT_WRITABLE,       // 15 
}
MARS_RESULT;

//
// Output formats
//

typedef enum 
{
   MARS_MONO                = 1,       // For compatibility with S32 channel param
   MARS_STEREO              = 2,
   MARS_USE_SYSTEM_CONFIG   = 0x10,    // Leave space between entries for new variations
   MARS_HEADPHONES          = 0x20,    // with similar quality levels/speaker counts
   MARS_DOLBY_SURROUND      = 0x30,    
   MARS_SRS_CIRCLE_SURROUND = 0x40,    // Enums MUST match MSS_MC_SPEC!
   MARS_40_DTS              = 0x48,
   MARS_40_DISCRETE         = 0x50,
   MARS_51_DTS              = 0x58,
   MARS_51_DISCRETE         = 0x60,
   MARS_61_DISCRETE         = 0x70,
   MARS_71_DISCRETE         = 0x80,
   MARS_81_DISCRETE         = 0x90,
   MARS_DIRECTSOUND3D       = 0xA0,
   MARS_EAX2                = 0xC0,
   MARS_EAX3                = 0xD0,
   MARS_EAX4                = 0xE0
}
MARS_OUTPUT_FORMAT;

//
// Room types
//

typedef enum
{
    MARS_ROOM_GENERIC,                // factory default
    MARS_ROOM_PADDEDCELL,
    MARS_ROOM_ROOM,                   // standard ROOMs
    MARS_ROOM_BATHROOM,
    MARS_ROOM_LIVINGROOM,
    MARS_ROOM_STONEROOM,              // Enums MUST match MSS room types!
    MARS_ROOM_AUDITORIUM,
    MARS_ROOM_CONCERTHALL,
    MARS_ROOM_CAVE,
    MARS_ROOM_ARENA,
    MARS_ROOM_HANGAR,
    MARS_ROOM_CARPETEDHALLWAY,
    MARS_ROOM_HALLWAY,
    MARS_ROOM_STONECORRIDOR,
    MARS_ROOM_ALLEY,
    MARS_ROOM_FOREST,
    MARS_ROOM_CITY,
    MARS_ROOM_MOUNTAINS,
    MARS_ROOM_QUARRY,
    MARS_ROOM_PLAIN,
    MARS_ROOM_PARKINGLOT,
    MARS_ROOM_SEWERPIPE,
    MARS_ROOM_UNDERWATER,
    MARS_ROOM_DRUGGED,
    MARS_ROOM_DIZZY,
    MARS_ROOM_PSYCHOTIC,

    MARS_ROOM_COUNT           // total number of ROOMs
}
MARS_ROOM_TYPE;

//
// MARS_sound_status() flag values
//

#define MARS_PRELOADED 0x00000001     // A valid file image for this sound exists
#define MARS_TRIGGERED 0x00000002     // Most-recently triggered instance for this sound exists (i.e., still has a virtual voice)
#define MARS_PLAYING   0x00000004     // Most-recently-triggered instance has started playing but is not yet finished
#define MARS_PAUSED    0x00000008     // All instances have been manually stopped with MARS_pause()
#define MARS_ENDED     0x00000010     // Most-recently triggered instance has finished playing on its own, or been manually terminated with MARS_end()
#define MARS_VOICED    0x00010000     // Most-recently preloaded instance has a valid physical HSAMPLE

//
// Enumeration initializer
//

typedef SINTa HMARSENUM;

#define MARS_FIRST -1

//
// Property type info
//

typedef enum 
{
   MP_S32,
   MP_S64,
   MP_SINTa,
   MP_F32,
   MP_BOOL,
   MP_VEC3,
   MP_SZ,
   MP_OUTPUT_FORMAT,
   MP_ROOM_TYPE,
   MP_SOUND_STATUS,
}
MARS_PROPERTY_TYPE;

#define MPF_READ_ONLY    0x00000001    // Property can be updated only by the system, not by the API
#define MPF_UNENUMERABLE 0x00000002    // Property should not be returned by enumeration functions
#define MPF_UNEDITABLE   0x00000004    // Property should be hidden (not editable) in RAD or other production tools
#define MPF_ALTERNATE    0x00000008    // Property is an alternate representation of another property, and does not need to be saved/restored
#define MPF_UNSAVED      0x00000010    // Property is otherwise not needed to recreate the sound or environment state

// TODO: prereqs?

typedef struct
{
   const C8          *name;
   MARS_PROPERTY_TYPE type;
   U32                MPF_flags;
}
MARS_PROPERTY_INFO;

#define MARS_PROPERTY_NAME extern const C8 *    // TODO: DLL linkage
#define MARS_PROPERTY             const C8 *

// --------------------------------------------------------
// Global API
// --------------------------------------------------------

MARS_RESULT MARS_startup(void);

MARS_RESULT MARS_shutdown(void);

// --------------------------------------------------------
// Environment/project-level API
// --------------------------------------------------------

//
// Environment creation/destruction
//
// MARS_create_environment(): Create an HMENV handle
//
// MARS_set_output_format(): Associate a sound driver with the environment and
//   set its global properties) based on the specified output format
//
// MARS_load_environment_file(): Applies the global settings from the specified 
//  RAD project file to an existing HMENV environment.  All sound definitions in 
//  the project file are also loaded.  Optionally, any sound definitions that are
//  playing will be restored.
//
// MARS_save_environment_file(): Writes the environment properties to a Global{} clause 
//  in the specified project file, followed by any Sound{} definitions.
//
// MARS_destroy_environment(): Shuts down the underlying HDIGDRIVER and cleans 
//  up resources
//

// TODO: MLE_PRELOAD?
#define MLE_RESUME_PLAYING_SOUNDS   0x00000001      // Re-create playback state of all sound instances

#define MSE_SAVE_PLAYING_SOUNDS     0x00000001      // Save playback state of all sound instances for later resumption
#define MSE_HUMAN_READABLE          0x00000002      // Better for development/diagnostic purposes but files are bigger and slower to process

#define MEV_FORCE_PREFERENCE        0x00000001      // Equivalent to MILES_OPEN_DIGITAL_FLAGS
#define MEV_NEED_HW_3D              0x00000002
#define MEV_NEED_FULL_3D            0x00000004
#define MEV_NEED_LIGHT_3D           0x00000008
#define MEV_NEED_HW_REVERB          0x00000010
#define MEV_NEED_REVERB             0x00000020
#define MEV_USE_IOP_CORE0           0x00000040
#define MEV_RESTORE_SOUND_INSTANCES 0x00000080

MARS_RESULT MARS_create_environment        (HMENV             *environment,
                                            S32                n_virtual_voices);

MARS_RESULT MARS_set_output_format         (HMENV              environment,
                                            MARS_OUTPUT_FORMAT output_channel_spec,
                                            S32                output_sample_rate,
                                            S32                n_physical_voices,
                                            U32                MEV_flags);

MARS_RESULT MARS_load_environment_file     (HMENV       environment,    
                                            C8         *ENV_filename,
                                            U32         MLE_flags);    

MARS_RESULT MARS_save_environment_file     (HMENV       environment,
                                            C8         *ENV_filename,
                                            U32         MSE_flags);

MARS_RESULT MARS_destroy_environment       (HMENV       environment);

//
// MARS_output_driver(): Returns the underlying HDIGDRIVER used to play sounds in a given 
//                         environment, for use with low-level AIL calls
//
// MARS_output_filter(): Returns the HPROVIDER used to provide output-filter services 
//                         (EAX, DS3D, Dolby, SRS...) in a given environment, for use with
//                         low-level AIL calls (we probably won't wrap the output filter 
//                         property-control functions, for example)
//

MARS_RESULT MARS_output_driver (HMENV         environment, 
                                HDIGDRIVER   *driver);

MARS_RESULT MARS_output_filter (HMENV         environment, 
                                HPROVIDER    *provider,
                                S32          *global_reverb_supported);

//
// Environment-level properties
//
// Initial values for these properties can be set in the RAD tool, written to a project file,
// and applied at runtime by MARS_load_environment_settings()
//
// MARS_enumerate_environment_properties(): Returns a list of all environment properties
//
// MARS_environment_property(): Set or retrieve a specified property.  We also want to
//  to define a shortcut macro or inline function for each property name, so people don't have to
//  call an unsafe function with 5 parameters...
// 
// MARS_environment_property_text(): Text-based wrapper for MARS_environment_property()
//
// MARS_update_listener_3D_position(): Same as AIL_update_listener_3D_position()
//
// MARS_defer/commit_environment_settings(): When deferred, the environment's settings
//  are updated with new property values, but they aren't passed to the driver.  
//  The commit function copies any/all altered values to the driver using an AIL_lock() 
//  clause.
//

MARS_PROPERTY_NAME ENV_N_PLAYING_VOICES;          // Read-only property
MARS_PROPERTY_NAME ENV_F_LATENCY_MS;              // Read-only property
MARS_PROPERTY_NAME ENV_N_CPU_PERCENT;             // Read-only property
MARS_PROPERTY_NAME ENV_F_MASTER_VOLUME;           // Master volume in 0-1 "loudness" space
MARS_PROPERTY_NAME ENV_F_MASTER_VOLUME_DB;        // Master volume in dB (0=max)
MARS_PROPERTY_NAME ENV_F_MASTER_VOLUME_SCALE;     // Master volume in linear scalar space
MARS_PROPERTY_NAME ENV_RTYPE_REVERB_ROOM_TYPE;    // Reverb room type
MARS_PROPERTY_NAME ENV_F_REVERB_TIME_SECS;        // Another way to control room parameters
MARS_PROPERTY_NAME ENV_F_REVERB_PREDELAY_SECS;    // ""
MARS_PROPERTY_NAME ENV_F_REVERB_DAMPING;          // ""
MARS_PROPERTY_NAME ENV_F_REVERB_MASTER_DRY_VOL;   // Wet/dry levels, with dB, linear, and loudness
MARS_PROPERTY_NAME ENV_F_REVERB_MASTER_WET_VOL;   // control alternatives
MARS_PROPERTY_NAME ENV_F_REVERB_MASTER_DRY_DB;    // 
MARS_PROPERTY_NAME ENV_F_REVERB_MASTER_WET_DB;    //
MARS_PROPERTY_NAME ENV_F_REVERB_MASTER_DRY_SCALE; // 
MARS_PROPERTY_NAME ENV_F_REVERB_MASTER_WET_SCALE; //
MARS_PROPERTY_NAME ENV_F_LISTENER_X_POS;          // Listener position, orientation, and velocity
MARS_PROPERTY_NAME ENV_F_LISTENER_X_FACE;         //
MARS_PROPERTY_NAME ENV_F_LISTENER_X_UP;           //
MARS_PROPERTY_NAME ENV_F_LISTENER_X_VEL;          //
MARS_PROPERTY_NAME ENV_F_LISTENER_Y_POS;          //
MARS_PROPERTY_NAME ENV_F_LISTENER_Y_FACE;         //
MARS_PROPERTY_NAME ENV_F_LISTENER_Y_UP;           //
MARS_PROPERTY_NAME ENV_F_LISTENER_Y_VEL;          //
MARS_PROPERTY_NAME ENV_F_LISTENER_Z_POS;          //
MARS_PROPERTY_NAME ENV_F_LISTENER_Z_FACE;         //
MARS_PROPERTY_NAME ENV_F_LISTENER_Z_UP;           //
MARS_PROPERTY_NAME ENV_F_LISTENER_Z_VEL;          //
MARS_PROPERTY_NAME ENV_V_LISTENER_POS;            //
MARS_PROPERTY_NAME ENV_V_LISTENER_FACE;           //
MARS_PROPERTY_NAME ENV_V_LISTENER_UP;             //
MARS_PROPERTY_NAME ENV_V_LISTENER_VEL;            //
MARS_PROPERTY_NAME ENV_F_LISTENER_MAG;            //
MARS_PROPERTY_NAME ENV_F_ROLLOFF;                 // Distance rolloff factor
MARS_PROPERTY_NAME ENV_F_DOPPLER;                 // Doppler scale factor
MARS_PROPERTY_NAME ENV_F_FALLOFF_POWER;           // Falloff power

MARS_RESULT MARS_enumerate_environment_properties
                                            (HMENV               environment,
                                             HMARSENUM          *next,
                                             MARS_PROPERTY_INFO *dest);

MARS_RESULT MARS_environment_property(HMENV         environment,
                                      MARS_PROPERTY name,
                                      void *        before_value,
                                      void const *  new_value,
                                      void *        after_value,
                                      void *        min_limit = NULL,
                                      void *        max_limit = NULL,
                                      C8 **         description = NULL);

MARS_RESULT MARS_environment_property_text(HMENV         E,
                                           MARS_PROPERTY name,
                                           C8 *          before_value,
                                           C8 const *    new_value,
                                           C8 *          after_value,
                                           void *        min_limit = NULL,
                                           void *        max_limit = NULL,
                                           C8 **         description = NULL);

MARS_RESULT MARS_update_listener_3D_position(HMENV environment,
                                             F32   dt_milliseconds);

MARS_PROPERTY_NAME ENV_F_UPDATE_LISTENER_POS_MS;  // Same as calling AIL_update_listener_3D_position()


MARS_RESULT MARS_defer_environment_settings (HMENV environment);
MARS_RESULT MARS_commit_environment_settings(HMENV environment);

//
// TODO: helper function prototypes
//

//
// The new API lets you specify these speaker labels directly, rather than worrying 
// about how they map into the channel array for the current format...
//

typedef const S32 MARS_SPEAKER;

MARS_SPEAKER MARS_SPEAKER_FRONT_LEFT            = 0;
MARS_SPEAKER MARS_SPEAKER_FRONT_RIGHT           = 1;
MARS_SPEAKER MARS_SPEAKER_FRONT_CENTER          = 2;
MARS_SPEAKER MARS_SPEAKER_LFE                   = 3;
MARS_SPEAKER MARS_SPEAKER_BACK_LEFT             = 4;
MARS_SPEAKER MARS_SPEAKER_BACK_RIGHT            = 5;
MARS_SPEAKER MARS_SPEAKER_FRONT_LEFT_OF_CENTER  = 6;
MARS_SPEAKER MARS_SPEAKER_FRONT_RIGHT_OF_CENTER = 7;
MARS_SPEAKER MARS_SPEAKER_BACK_CENTER           = 8;
MARS_SPEAKER MARS_SPEAKER_SIDE_LEFT             = 9;
MARS_SPEAKER MARS_SPEAKER_SIDE_RIGHT            = 10;
MARS_SPEAKER MARS_SPEAKER_TOP_CENTER            = 11;
MARS_SPEAKER MARS_SPEAKER_TOP_FRONT_LEFT        = 12;
MARS_SPEAKER MARS_SPEAKER_TOP_FRONT_CENTER      = 13;
MARS_SPEAKER MARS_SPEAKER_TOP_FRONT_RIGHT       = 14;
MARS_SPEAKER MARS_SPEAKER_TOP_BACK_LEFT         = 15;
MARS_SPEAKER MARS_SPEAKER_TOP_BACK_CENTER       = 16;
MARS_SPEAKER MARS_SPEAKER_TOP_BACK_RIGHT        = 17;

//
// MARS_speaker_wet_level():
// MARS_speaker_dry_level(): 
// MARS_speaker_wet_scale():
// MARS_speaker_dry_scale(): 
// MARS_speaker_wet_dB():
// MARS_speaker_dry_dB(): Get/set an individual speaker's level in terms of loudness, dB, or amplitude
//
// MARS_speaker_location(): See AIL_set_speaker_location()
//
//

MARS_RESULT MARS_speaker_wet_level (HMENV        environment, 
                                    MARS_SPEAKER speaker,
                                    F32         *before_value,
                                    F32 const   *new_value);

MARS_RESULT MARS_speaker_dry_level (HMENV        environment, 
                                    MARS_SPEAKER speaker,
                                    F32         *before_value,
                                    F32 const   *new_value);

MARS_RESULT MARS_speaker_wet_scale(HMENV        environment, 
                                   MARS_SPEAKER speaker,
                                   F32         *before_value,
                                   F32 const   *new_value);

MARS_RESULT MARS_speaker_dry_scale(HMENV        environment, 
                                   MARS_SPEAKER speaker,
                                   F32         *before_value,
                                   F32 const   *new_value);

MARS_RESULT MARS_speaker_wet_dB  (HMENV        environment, 
                                  MARS_SPEAKER speaker,
                                  F32         *before_value,
                                  F32 const   *new_value);

MARS_RESULT MARS_speaker_dry_dB  (HMENV        environment, 
                                  MARS_SPEAKER speaker,
                                  F32         *before_value,
                                  F32 const   *new_value);

MARS_RESULT MARS_speaker_location(HMENV         environment, 
                                  MARS_SPEAKER  speaker,
                                  MSSVECTOR3D  *before_value,
                                  MSSVECTOR3D  *new_value,
                                  MSSVECTOR3D  *after_value);

// --------------------------------------------------------
// Sound/voice API
// --------------------------------------------------------

//
// MARS_create_sound(): Create an empty sound definition for a given environment
//
// MARS_copy_sound(): 
// MARS_play_copy():
//
//   Create a sound definition that's identical to an existing one.
//   This is a fairly-lightweight operation, and there are two reasons why you might 
//   want to use it:
//
//    - You want to alter the properties of an HMSOUND without modifying the original
//      properties assigned at authoring time
//
//    - You want to establish a 1:1 correspondence between definitions and playing instances,
//      e.g. so you can stop a sound, perform seeking on it, or poll its status
//
//   If you don't want to modify the copy's properties prior to playing it, you can just call
//   MARS_play_copy().  (MARS_play_copy() with a NULL *copy parameter calls MARS_play() normally.)
//
// MARS_destroy_sound(): Destroy sound definition and all of its internal references.  
//
// MARS_enumerate_sounds(): Enumerate all sound definitions in environment, optionally 
//   qualified by tag(s).  This will include sound definitions that may have been 
//   loaded from the project file, as well as any new definitions created since the 
//   project was loaded
//

MARS_RESULT MARS_create_sound (HMENV    environment,
                               C8      *tagset,
                               HMSOUND *sound); 

MARS_RESULT MARS_copy_sound   (HMSOUND  original,
                               HMSOUND *copy);

MARS_RESULT MARS_play_copy    (HMSOUND  original,
                               HMSOUND *copy);

MARS_RESULT MARS_destroy_sound(HMSOUND sound);

MARS_RESULT MARS_enumerate_sounds(HMENV      environment,
                                  C8        *tagset,
                                  HMARSENUM *next,
                                  HMSOUND   *dest);

//
// MARS_set_sound_file(): Associate a discrete .wav, .mp3, or other file with this
//   sound object.  Only one file may be associated with any given sound definition, but
//   nothing prevents you from creating a sound definition containing nothing but script tags
//   that trigger multiple sounds...
//
//   This also creates the set of attribute tags for the HMSOUND.  (Not all attributes may apply
//   to all file types, so we can't do this at the time the HMSOUND is created.)
//
//   TODO: another version that works with wave library files, and maybe some way to deal 
//   with memory-resident waves?
//
//   TODO: Support multichannel source files by creating HMSOUNDS that can deal with
//   multiple bound HSAMPLES?
//

MARS_RESULT MARS_set_sound_file(HMSOUND sound,
                                C8     *filename);

//
// MARS_preload(): Preloads sound's file or opens stream, optionally forcing an HSAMPLE 
//   to be allocated to make the parameters available for inspection/modification 
//   prior to playback
//

MARS_RESULT MARS_preload(HMSOUND sound,
                         S32     assign_voice);

//
// MARS_discard(): Forces the system to free memory used by sound definition
//   to store RAM-resident files and voices
//

MARS_RESULT MARS_discard(HMSOUND sound);

//
// MARS_play(): Start a playing instance of a sound, loading the file or opening the stream
//   if necessary, and assigning an HSAMPLE if necessary (and possible).  
//
//   The sound is started using the parameters and attributes from the HMSOUND definition.  If
//   you want to override these and change them before starting the sound, or if you want any
//   parameter changes to affect only this particular instance of a playing sound, or if you
//   need to monitor the progress of this sample by querying its status or properties, you can 
//   create a copy of the HMSOUND.  MARS_play_copy() is a convenient way to do this.
//
//   Otherwise, applications can just fire-and-forget the sound without any further 
//   involvement.
// 
// MARS_pause(): Stops playback of any/all instances of a given HMSOUND without releasing their
//               physical voice(s), if any
//
// MARS_resume(): Resumes playback of any/all paused instances of a given HMSOUND
//
// MARS_end(): Kills any/all instances of a given HMSOUND, releasing their virtual and physical
//             voices
//
// MARS_serve_stream(): Same as AIL_service_stream()
//

MARS_RESULT MARS_play(HMSOUND sound);

MARS_RESULT MARS_pause(HMSOUND sound);

MARS_RESULT MARS_resume(HMSOUND sound);

MARS_RESULT MARS_end(HMSOUND sound);

MARS_RESULT MARS_serve_stream(HMSOUND sound);

//
// Sound properties
//
// The HMSOUND contains values for each of these properties that serve as initial defaults
// when the sound is played.  These initial defaults are set in the RAD tool.  If you want 
// to alter any of these properties without affecting the original HMSOUND, you can 
// use MARS_copy_sound().
//
// Any altered HMSOUND properties will propagate to any playing instances of the HMSOUND.
//
// MARS_sound_property(): Set or retrieve a specified property.  We also want to
//  to define a shortcut macro or inline function for each property name, so people don't have to
//  call an unsafe function with 5 parameters...
//
//  Note that some properties don't apply to sound definitions at all.  It doesn't make sense to
//  query the status or play-cursor position of an HMSOUND by itself, for instance.  For these 
//  properties, the value for the most-recently-triggered instance will be returned.
//
// MARS_sound_property_text(): Text-based wrapper for MARS_sound_property()
//
// MARS_update_sound_3D_position(): Same as AIL_update_sound_3D_position()
//
// MARS_defer/commit_sound_settings(): When deferred, the sound's settings
//  are updated with new property values, but they aren't used to update the HSAMPLE/HSTREAM.
//  The commit function copies any/all altered values to the HSAMPLE/HSTREAM at once, using 
//  an AIL_lock() clause.
//
// MARS_sound_channel_level()
// MARS_sound_channel_scale()
// MARS_sound_channel_dB(): Individual speaker level control, a la AIL_set_sample_channel_levels()
//
// TODO: Resumption behavior when sound's voice is recovered after theft (one-shot, resumable,
//   time-tracked)
//

MARS_PROPERTY_NAME SOUND_STATUS;                  // Playback status
MARS_PROPERTY_NAME SOUND_SZ_FILENAME;             // Source filename
MARS_PROPERTY_NAME SOUND_L_FILE_BYTES;            // Size of source filename in bytes
MARS_PROPERTY_NAME SOUND_N_PLAYBACK_RATE_HZ;      // Playback rate
MARS_PROPERTY_NAME SOUND_N_ORIGINAL_RATE_HZ;      // Read-only property
MARS_PROPERTY_NAME SOUND_N_SOURCE_CHANNELS;       // Read-only property
MARS_PROPERTY_NAME SOUND_B_IS_STREAM;             // Whether it's streaming or static
MARS_PROPERTY_NAME SOUND_B_IS_AUTO_STREAM;        // If it's a stream, is it serviced automatically?
MARS_PROPERTY_NAME SOUND_N_STREAM_BUFFER_BYTES;   // If it's a stream, how big is the buffer? (-1=default)
MARS_PROPERTY_NAME SOUND_N_PRIORITY;              // Priority level
MARS_PROPERTY_NAME SOUND_F_VOL;                   // Base volume for this HSOUND in dB, linear amplitude, 
MARS_PROPERTY_NAME SOUND_F_DB;                    //  and loudness
MARS_PROPERTY_NAME SOUND_F_SCALE;                 // 
MARS_PROPERTY_NAME SOUND_F_LR_PAN;                // Left-right pan
MARS_PROPERTY_NAME SOUND_F_FB_PAN;                // Front-back pan
MARS_PROPERTY_NAME SOUND_F_CENTER_VOL;            // Center and LFE volume
MARS_PROPERTY_NAME SOUND_F_CENTER_DB;             // 
MARS_PROPERTY_NAME SOUND_F_CENTER_SCALE;          // 
MARS_PROPERTY_NAME SOUND_F_LFE_VOL;               //
MARS_PROPERTY_NAME SOUND_F_LFE_DB;                // 
MARS_PROPERTY_NAME SOUND_F_LFE_SCALE;             // 
MARS_PROPERTY_NAME SOUND_F_WET_VOL;               // Wet and dry volume for built-in reverb
MARS_PROPERTY_NAME SOUND_F_WET_DB;                // 
MARS_PROPERTY_NAME SOUND_F_WET_SCALE;             // 
MARS_PROPERTY_NAME SOUND_F_DRY_VOL;               //
MARS_PROPERTY_NAME SOUND_F_DRY_DB;                // 
MARS_PROPERTY_NAME SOUND_F_DRY_SCALE;             // 
MARS_PROPERTY_NAME SOUND_F_LPF_CUTOFF;            // Built-in LPF cutoff (as fraction of Nyquist)
MARS_PROPERTY_NAME SOUND_L_PLAYBACK_OFFSET_BYTES; // Play cursor in bytes and ms
MARS_PROPERTY_NAME SOUND_F_PLAYBACK_POSITION_MS;  //
MARS_PROPERTY_NAME SOUND_L_TOTAL_LEN_BYTES;       // Total length in bytes and ms
MARS_PROPERTY_NAME SOUND_F_DURATION_MS;           //
MARS_PROPERTY_NAME SOUND_N_INITIAL_LOOP_COUNT;    // Original loop count to apply to triggered sounds
MARS_PROPERTY_NAME SOUND_N_REMAINING_LOOP_COUNT;  // Loops remaining (readable/writable)
MARS_PROPERTY_NAME SOUND_L_LOOP_START_BYTES;      // Loop boundaries in bytes
MARS_PROPERTY_NAME SOUND_L_LOOP_END_BYTES;        // 
MARS_PROPERTY_NAME SOUND_CB_EOS;                  // End-of-sound callback handler
MARS_PROPERTY_NAME SOUND_SINTa_USER;              // User data field
MARS_PROPERTY_NAME SOUND_F_OCCLUSION;             // Occlusion  
MARS_PROPERTY_NAME SOUND_F_OBSTRUCTION;           // Obstruction
MARS_PROPERTY_NAME SOUND_F_EXCLUSION;             // Exclusion  
MARS_PROPERTY_NAME SOUND_B_IS_3D;                 // TRUE if this sound is 3D-positioned
MARS_PROPERTY_NAME SOUND_F_3D_X_POS;              // 3D position, orientation, velocity as X,Y,Z 
MARS_PROPERTY_NAME SOUND_F_3D_X_FACE;             //
MARS_PROPERTY_NAME SOUND_F_3D_X_UP;               //
MARS_PROPERTY_NAME SOUND_F_3D_X_VEL;              //
MARS_PROPERTY_NAME SOUND_F_3D_Y_POS;              //
MARS_PROPERTY_NAME SOUND_F_3D_Y_FACE;             //
MARS_PROPERTY_NAME SOUND_F_3D_Y_UP;               //
MARS_PROPERTY_NAME SOUND_F_3D_Y_VEL;              //
MARS_PROPERTY_NAME SOUND_F_3D_Z_POS;              //
MARS_PROPERTY_NAME SOUND_F_3D_Z_FACE;             //
MARS_PROPERTY_NAME SOUND_F_3D_Z_UP;               //
MARS_PROPERTY_NAME SOUND_F_3D_Z_VEL;              //
MARS_PROPERTY_NAME SOUND_V_3D_POS;                // 3D position, orientation, velocity as VEC3
MARS_PROPERTY_NAME SOUND_V_3D_FACE;               //
MARS_PROPERTY_NAME SOUND_V_3D_UP;                 //
MARS_PROPERTY_NAME SOUND_V_3D_VEL;                //
MARS_PROPERTY_NAME SOUND_F_3D_MAG;                //
MARS_PROPERTY_NAME SOUND_F_3D_CONE_INNER_DEGS;    // 3D cone inner angle, outer angle, volume
MARS_PROPERTY_NAME SOUND_F_3D_CONE_INNER_RADS;    //
MARS_PROPERTY_NAME SOUND_F_3D_CONE_OUTER_DEGS;    //
MARS_PROPERTY_NAME SOUND_F_3D_CONE_OUTER_RADS;    //
MARS_PROPERTY_NAME SOUND_F_3D_CONE_OUTER_VOL;     //
MARS_PROPERTY_NAME SOUND_F_3D_CONE_OUTER_DB;      //
MARS_PROPERTY_NAME SOUND_F_3D_CONE_OUTER_SCALE;   //
MARS_PROPERTY_NAME SOUND_F_3D_MIN_DIST;           // 3D min/max distance
MARS_PROPERTY_NAME SOUND_F_3D_MAX_DIST;           //
MARS_PROPERTY_NAME SOUND_CB_3D_FALLOFF;           // 3D falloff-function callback handler
MARS_PROPERTY_NAME SOUND_B_3D_AUTO_WET_ATTEN;     // 3D auto wet atten 
MARS_PROPERTY_NAME SOUND_L_UNDERLYING_HSAMPLE;    // Temporary for demo/test

MARS_RESULT MARS_enumerate_sound_properties
                                            (HMSOUND             sound,
                                             HMARSENUM          *next,
                                             MARS_PROPERTY_INFO *dest);

MARS_RESULT MARS_sound_property(HMSOUND       sound,
                                MARS_PROPERTY name,
                                void *        before_value,
                                void const *  new_value,
                                void *        after_value,
                                void *        min_limit = NULL,
                                void *        max_limit = NULL,
                                C8 **         description = NULL);

MARS_RESULT MARS_sound_property_text(HMSOUND       D,
                                     MARS_PROPERTY name,
                                     C8 *          before_value,
                                     C8 const *    new_value,
                                     C8 *          after_value,
                                     void *        min_limit = NULL,
                                     void *        max_limit = NULL,
                                     C8 **         description = NULL);

MARS_RESULT MARS_update_sound_3D_position(HMSOUND sound,
                                          F32     dt_milliseconds);

MARS_RESULT MARS_sound_channel_level(HMSOUND      sound,
                                     MARS_SPEAKER channel,
                                     F32         *before_value,
                                     F32 const   *new_value);

MARS_RESULT MARS_sound_channel_scale(HMSOUND      sound,
                                     MARS_SPEAKER channel,
                                     F32         *before_value,
                                     F32 const   *new_value);

MARS_RESULT MARS_sound_channel_dB  (HMSOUND      sound,
                                    MARS_SPEAKER channel,
                                    F32         *before_value,
                                    F32 const   *new_value);

MARS_RESULT MARS_defer_sound_settings (HMSOUND sound);
MARS_RESULT MARS_commit_sound_settings(HMSOUND sound);

//
// Filter pipeline API
//

MARS_RESULT MARS_enumerate_filters (HMENV      environment,
                                    HPROENUM  *next,
                                    C8       **filter_name);

MARS_RESULT MARS_install_sound_filter(HMSOUND           sound,
                                      MARS_SAMPLE_STAGE stage,
                                      C8               *filter_name);

MARS_RESULT MARS_sound_filter        (HMSOUND           sound,
                                      MARS_SAMPLE_STAGE stage,
                                      C8              **filter_name);

MARS_RESULT MARS_enumerate_filter_properties(HMENV                environment,
                                             C8                  *filter_name,
                                             HMARSENUM           *next,
                                             RIB_INTERFACE_ENTRY *dest);

MARS_RESULT MARS_sound_filter_property(HMSOUND           sound,
                                       MARS_SAMPLE_STAGE stage,
                                       C8               *property_name,
                                       void *            before_value,
                                       void const *      new_value,
                                       void *            after_value);

//
// Tag API
// 
// MARS_add_tag(): Add tag(s) to the environment, making them available for enumeration and
//   assignment to individual sounds.  
//
// MARS_remove_tag(): Remove tag(s) registered with the environment.  References to the
//   tag(s) will also be removed from any HSOUNDs.
//
// MARS_add_sound_tag(): Add tag(s) to the sound.  Any tag(s) that have not yet been registered 
//   in the HMSOUND's environment will be added to the environment as well.
//
// MARS_remove_sound_tag(): Remove tag(s) from this sound definition (NULL to remove all).  Tags are
//   not automatically removed from the sound's environment, even when no other sounds use them.
//
// MARS_enumerate_tags(): Enumerates all tag(s) associated with sound definition
//
// MARS_sound_has_tag(): Returns TRUE if the HSOUND has _all_ of the specified tag(s)
//
// MARS_set_tag_script(): Associates a script with the specified tag(s).  The script will
//   execute for all sounds with these tag(s).  Only one script can be associated with any given
//   tag, but any number of script-bearing tags may be associated with any sound definition.
// 

MARS_RESULT MARS_add_tag   (HMENV  environment,
                            C8    *tagset);

MARS_RESULT MARS_remove_tag(HMENV  environment,
                            C8    *tagset);

MARS_RESULT MARS_add_sound_tag(HMSOUND sound,
                               C8     *tagset);

MARS_RESULT MARS_remove_sound_tag(HMSOUND sound,
                                  C8     *tagset);

MARS_RESULT MARS_enumerate_tags(HMENV      environment,
                                HMSOUND    sound,
                                HMARSENUM *next,
                                C8        *tag);

S32        MARS_sound_has_tag (HMSOUND sound,
                               C8     *tagset);

MARS_RESULT MARS_set_tag_script(HMENV environment,
                                C8   *tagset,
                                C8   *script);

//
// Helper functions
//

#ifdef __cplusplus
   inline MARS_RESULT MARS_sound_status(HMSOUND sound, U32 *status)
      {
      return MARS_sound_property(sound, SOUND_STATUS, status, NULL, NULL);
      }
#else 
   #define MARS_sound_status(sound,status) MARS_sound_property(sound, SOUND_STATUS, status, NULL, NULL);
#endif

#ifdef __cplusplus
   inline MARS_RESULT MARS_sound_playback_rate_Hz(HMSOUND sound, U32 *playback_rate_Hz)
      {
      return MARS_sound_property(sound, SOUND_N_PLAYBACK_RATE_HZ, playback_rate_Hz, NULL, NULL);
      }
#else 
   #define MARS_sound_playback_rate(sound,playback_rate) MARS_sound_property(sound, SOUND_N_PLAYBACK_RATE_HZ, (playback_rate_Hz), NULL, NULL);
#endif

#ifdef __cplusplus
   inline MARS_RESULT MARS_set_sound_playback_rate_Hz(HMSOUND sound, U32 playback_rate_Hz)
      {
      return MARS_sound_property(sound, SOUND_N_PLAYBACK_RATE_HZ, NULL, &playback_rate_Hz, NULL);
      }
#else 
   #define MARS_set_sound_playback_rate(sound,playback_rate) { U32 temp = (playback_rate); MARS_sound_property(sound, SOUND_N_PLAYBACK_RATE_HZ, NULL, &temp, NULL); }
#endif


#endif
