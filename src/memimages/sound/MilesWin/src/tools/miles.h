//############################################################################
//##                                                                        ##
//##  MILES.H                                                               ##
//##                                                                        ##
//##  Miles Sound System 8 HLAPI                                            ##
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

#ifndef MILES_VERSION

#define MILES_VERSION      "8.0a"     // Version string needs to be 4 characters long
#define MILES_MAJOR_VERSION 8
#define MILES_MINOR_VERSION 0
#define MILES_SUB_VERSION   1
#define MILES_VERSION_DATE "23-Mar-07"

#define MILES_COPYRIGHT "Copyright (C) 1991-2007, RAD Game Tools, Inc."

#endif

#ifndef MILES_H
#define MILES_H

#include <radbase.h>

#ifdef __cplusplus
extern "C" {
#define DEFPARM(x) = x              // Default parameters are supported when compiled as .cpp
#define RADDEFSTART extern "C" {
#define RADDEFEND }
#else
#define DEFPARM(x)                  // Default parameters are ignored when compiled as .c
#define RADDEFSTART
#define RADDEFEND
#endif

#define MILES_CALLBACK RADLINK

//
// Misc. constant definitions
//

#define MAX_DRVRS                  16     // Max. # of simultaneous environments
#define MAX_TIMERS                 16     // Max. # of simultaneous timers (TODO)
#ifdef __RADWIN__
#define MAX_SPEAKERS               18     // Up to 18 hardware output channels supported
#elif defined(__RADPS3__)
#define MAX_SPEAKERS               8      // Up to 8 hardware output channels on PS3
#else
#define MAX_SPEAKERS               6      // Up to 6 hardware output channels supported on other platforms
#endif
#define MAX_RECEIVER_SPECS         32     // Up to 32 receiver point specifications

#define MILES_MAX_FILE_HEADER_SIZE 8192   // Initialization of handles based on files requires at least 
                                          // 8K of data or the entire file image, whichever is less,
                                          // to determine sample format
RADSTRUCT _DIG_DRIVER;
typedef RADSTRUCT _DIG_DRIVER * HDIGDRIVER;    // Handle to digital driver
typedef RADSTRUCT _SAMPLE * HSAMPLE;           // Handle to sample

typedef RADSTRUCT _MILES_VEC3
{
  F32 x;
  F32 y;
  F32 z;
} MILES_VEC3;

#ifndef RIB_H        // Subset of RIB.H contents needed for low-level calls

typedef U32 HPROVIDER;

typedef U32 HPROENUM;
#define HPROENUM_FIRST 0

typedef enum
{
   RIB_FUNCTION = 0,
   RIB_PROPERTY       // Property: read-only or read-write data type
}
RIB_ENTRY_TYPE;

typedef enum
{
   RIB_NONE = 0, // No type
   RIB_CUSTOM,   // Used for pointers to application-specific structures
   RIB_DEC,      // Used for 32-bit integer values to be reported in decimal
   RIB_HEX,      // Used for 32-bit integer values to be reported in hex
   RIB_FLOAT,    // Used for 32-bit single-precision FP values
   RIB_PERCENT,  // Used for 32-bit single-precision FP values to be reported as percentages
   RIB_BOOL,     // Used for Boolean-constrained integer values to be reported as TRUE or FALSE
   RIB_STRING,   // Used for pointers to null-terminated ASCII strings
   RIB_READONLY = 0x80000000  // Property is read-only
}
RIB_DATA_SUBTYPE;

typedef RADSTRUCT
{
   RIB_ENTRY_TYPE   type;        // See list above
   char            *entry_name;  // Name of desired function or property
   UINTa            token;       // Function pointer or property token
   RIB_DATA_SUBTYPE subtype;     // Property subtype
}
RIB_INTERFACE_ENTRY;

#endif

//
// Constants
//

#define MILES_MAX_PROP_TEXT_LEN 512
#define MILES_MAX_PROP_BIN_LEN  64

//
// MILES objects
//

//struct SNDDEF;
//struct MENV;
//
//typedef RADSTRUCT SNDDEF *HMSOUND; 
//typedef RADSTRUCT MENV   *HMENV;   

typedef U64 MILES_HANDLE;              // Polymorphic alias for all 64-bit handles

typedef U64 MILES_SYSTEM;              // Handle to global system
typedef U64 MILES_DRIVER;              // Handle to output driver
typedef U64 MILES_ENVIRONMENT;         // Handle to global environment
typedef U64 MILES_SOUND_DEFINITION;    // Handle to sound definition

typedef U64 MILES_SOUND_INSTANCE;      // Handle to sound instance (separate handle space from above)

typedef void (MILES_CALLBACK *MILES_SOUND_CALLBACK)   (MILES_SOUND_INSTANCE sound);
typedef F32  (MILES_CALLBACK *MILES_FALLOFF_CALLBACK) (MILES_SOUND_INSTANCE sound, F32 distance, F32 rolloff_factor, F32 min_dist, F32 max_dist);

//
// Sound processor stages
//

typedef enum
{
   SAMPLE_FILTER_0 = 1,             // These values must match SAMPLESTAGE equates
   SAMPLE_FILTER_1,
   SAMPLE_FILTER_2,
   SAMPLE_FILTER_3,
   SAMPLE_FILTER_4,
   SAMPLE_FILTER_5,
   SAMPLE_FILTER_6,
   SAMPLE_FILTER_7,
}
MILES_SAMPLE_STAGE;

//
// Result codes for MilesError()
//

typedef enum
{
   MILES_OK = 0,                  // 0
   MILES_DONE,                    // 1
   MILES_GENERAL_ERROR,           // 2
   MILES_NOT_FOUND,               // 3
   MILES_OUT_OF_RANGE,            // 4
   MILES_NOT_WRITABLE,            // 5
   MILES_BAD_PARAM,               // 6 
   MILES_BAD_PROPERTY,            // 7 
   MILES_RESOURCE_UNAVAILABLE,    // 8 
   MILES_OUT_OF_MEMORY,           // 9 
   MILES_DRIVER_FAILURE,          // 10 
   MILES_UNSUPPORTED_FORMAT,      // 11 
   MILES_NOT_DEFERRABLE,          // 12 
   MILES_FILE_NOT_FOUND,          // 13 
   MILES_FILE_NOT_READABLE,       // 14 
   MILES_FILE_NOT_WRITABLE,       // 15 
}
MILES_RESULT;

//
// Output formats
//

typedef enum 
{
   MILES_MONO                = 1,       // For compatibility with S32 channel param
   MILES_STEREO              = 2,
   MILES_USE_SYSTEM_CONFIG   = 0x10,    // Leave space between entries for new variations
   MILES_HEADPHONES          = 0x20,    // with similar quality levels/speaker counts
   MILES_DOLBY_SURROUND      = 0x30,    
   MILES_SRS_CIRCLE_SURROUND = 0x40,    // Enums MUST match MSS_MC_SPEC!
   MILES_40_DTS              = 0x48,
   MILES_40_DISCRETE         = 0x50,
   MILES_51_DTS              = 0x58,
   MILES_51_DISCRETE         = 0x60,
   MILES_61_DISCRETE         = 0x70,
   MILES_71_DISCRETE         = 0x80,
   MILES_81_DISCRETE         = 0x90,
   MILES_DIRECTSOUND3D       = 0xA0,
   MILES_EAX2                = 0xC0,
   MILES_EAX3                = 0xD0,
   MILES_EAX4                = 0xE0
}
MILES_OUTPUT_FORMAT;

//
// Room types
//

typedef enum
{
    MILES_ROOM_GENERIC,                // factory default
    MILES_ROOM_PADDEDCELL,
    MILES_ROOM_RM,                   // standard RMs
    MILES_ROOM_BATHRM,
    MILES_ROOM_LIVINGRM,
    MILES_ROOM_STONERM,              // Enums MUST match MSS RM types!
    MILES_ROOM_AUDITORIUM,
    MILES_ROOM_CONCERTHALL,
    MILES_ROOM_CAVE,
    MILES_ROOM_ARENA,
    MILES_ROOM_HANGAR,
    MILES_ROOM_CARPETEDHALLWAY,
    MILES_ROOM_HALLWAY,
    MILES_ROOM_STONECORRIDOR,
    MILES_ROOM_ALLEY,
    MILES_ROOM_FOREST,
    MILES_ROOM_CITY,
    MILES_ROOM_MOUNTAINS,
    MILES_ROOM_QUARRY,
    MILES_ROOM_PLAIN,
    MILES_ROOM_PARKINGLOT,
    MILES_ROOM_SEWERPIPE,
    MILES_ROOM_UNDERWATER,
    MILES_ROOM_DRUGGED,
    MILES_ROOM_DIZZY,
    MILES_ROOM_PSYCHOTIC,

    MILES_ROOM_COUNT           // total number of MILES_ROOMs
}
MILES_ROOM_TYPE;

//
// MilesSoundStatus() flag values
//

#define HSOUND_PRELOADED 0x00000001     // A valid file image for this sound exists
#define HSOUND_TRIGGERED 0x00000002     // Most-recently triggered instance for this sound exists (i.e., still has a virtual voice)
#define HSOUND_PLAYING   0x00000004     // Most-recently-triggered instance has started playing but is not yet finished
#define HSOUND_PAUSED    0x00000008     // All instances have been manually stopped with MilesPause()
#define HSOUND_ENDED     0x00000010     // Most-recently triggered instance has finished playing on its own, or been manually terminated with MilesEnd()
#define HSOUND_VOICED    0x00010000     // Most-recently preloaded instance has a valid physical HSAMPLE

//
// Enumeration initializer
//

typedef SINTa HMILESENUM;

#define MILES_FIRST -1

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
MILES_PROPERTY_TYPE;

#define MPF_READ_ONLY    0x00000001    // Property can be updated only by the system, not by the API
#define MPF_UNENUMERABLE 0x00000002    // Property should not be returned by enumeration functions
#define MPF_UNEDITABLE   0x00000004    // Property should be hidden (not editable) in RAD or other production tools
#define MPF_ALTERNATE    0x00000008    // Property is an alternate representation of another property, and does not need to be saved/restored
#define MPF_UNSAVED      0x00000010    // Property is otherwise not needed to recreate the sound or environment state

// TODO: prereqs?

typedef struct
{
   const char         *name;
   MILES_PROPERTY_TYPE type;
   U32                 MPF_flags;
}
MILES_PROPERTY_INFO;

#define MILES_PROPERTY_NAME extern const char *    // TODO: DLL linkage
#define MILES_PROPERTY             const char *

// --------------------------------------------------------
// Global API
// --------------------------------------------------------

MILES_SYSTEM MilesStartup(void);

// --------------------------------------------------------
// Generic API applying to handles of all types
// --------------------------------------------------------

//typedef U64 MILES_HANDLE;              // Polymorphic alias for all 64-bit handles
//
//typedef U64 MILES_SYSTEM;              // Handle to global system
//typedef U64 MILES_DRIVER;              // Handle to output driver
//typedef U64 MILES_ENVIRONMENT;         // Handle to global environment
//typedef U64 MILES_SOUND_DEFINITION;    // Handle to sound definition
//
//typedef U64 MILES_SOUND_INSTANCE;      // Handle to sound instance (separate handle space from above)

MILES_RESULT MilesLock       (MILES_HANDLE handle);
MILES_RESULT MilesUnlock     (MILES_HANDLE handle);
MILES_RESULT MilesRelease    (MILES_HANDLE handle);

MILES_RESULT MilesError      (MILES_HANDLE handle);

// MILES_TYPE   MilesHandleType (MILES_HANDLE handle);

MILES_RESULT MilesSerialize  (MILES_HANDLE handle,
                              char        *dest_buffer,
                              S32         &buffer_bytes);

// unserialize, describe

// --------------------------------------------------------
// Driver API
//
// A driver may not be bound to more than one environment in the
// current system; this will probably change
// --------------------------------------------------------

#define MDR_FORCE_PREFERENCE        0x00000001      // Equivalent to MILES_OPEN_DIGITAL_FLAGS
#define MDR_NEED_HW_3D              0x00000002
#define MDR_NEED_FULL_3D            0x00000004
#define MDR_NEED_LIGHT_3D           0x00000008
#define MDR_NEED_HW_REVERB          0x00000010
#define MDR_NEED_REVERB             0x00000020
#define MDR_USE_IOP_CORE0           0x00000040
#define MDR_RESTORE_SOUND_INSTANCES 0x00000080

MILES_DRIVER MilesOpenDriver  (MILES_OUTPUT_FORMAT output_channel_spec,    // MilesRelease() when done
                               S32                 output_sample_rate,
                               S32                 bits_per_channel,
                               S32                 n_physical_voices,
                               U32                 MDR_flags);


//
// MilesOutputDriver(): Returns the underlying HDIGDRIVER used to play sounds in a given 
//                        environment, for use with low-level AIL calls
//
// MilesOutputFilter(): Returns the HPROVIDER used to provide output-filter services 
//                        (EAX, DS3D, Dolby, SRS...) in a given environment, for use with
//                        low-level AIL calls (we probably won't wrap the output filter 
//                        property-control functions, for example)
//

MILES_RESULT MilesOutputDriver (MILES_DRIVER  environment, 
                                HDIGDRIVER   *driver);

MILES_RESULT MilesOutputFilter (MILES_DRIVER  driver, 
                                HPROVIDER    *provider,
                                S32          *global_reverb_supported);

// --------------------------------------------------------
// Environment/project-level API
// --------------------------------------------------------


MILES_ENVIRONMENT MilesCreateEnvironment   (S32 n_virtual_voices);

MILES_RESULT      MilesSetOutputDriver     (MILES_ENVIRONMENT   environment,
                                            MILES_DRIVER        driver);

//
// Environment-level properties
//
// Initial values for these properties can be set in the RAD tool, written to a project file,
// and applied at runtime by MilesLoadEnvironmentSettings()
//
// MilesEnumerateEnvironmentProperties(): Returns a list of all environment properties
//
// MilesEnvironmentProperty(): Set or retrieve a specified property.  We also want to
//  to define a shortcut macro or inline function for each property name, so people don't have to
//  call an unsafe function with 8 parameters...
// 
// MilesEnvironmentPropertyText(): Text-based wrapper for MilesEnvironmentProperty()
//
// MilesUpdateListener3DPosition(): Same as AIL_update_listener_3D_position()
//
// MilesDefer/CommitEnvironmentSettings(): When deferred, the environment's settings
//  are updated with new property values, but they aren't passed to the driver.  
//  The commit function copies any/all altered values to the driver at once,
//  using an AIL_lock() clause
//

MILES_PROPERTY_NAME ENV_SZ_TAGSET;                 // String containing list of tags for environment
MILES_PROPERTY_NAME ENV_N_PLAYING_VOICES;          // Read-only property
MILES_PROPERTY_NAME ENV_F_LATENCY_MS;              // Read-only property
MILES_PROPERTY_NAME ENV_N_CPU_PERCENT;             // Read-only property
MILES_PROPERTY_NAME ENV_F_MASTER_VOLUME;           // Master volume in 0-1 "loudness" space
MILES_PROPERTY_NAME ENV_F_MASTER_VOLUME_DB;        // Master volume in dB (0=max)
MILES_PROPERTY_NAME ENV_F_MASTER_VOLUME_SCALE;     // Master volume in linear scalar space
MILES_PROPERTY_NAME ENV_RTYPE_REVERB_ROOM_TYPE;    // Reverb room type
MILES_PROPERTY_NAME ENV_F_REVERB_TIME_SECS;        // Another way to control room parameters
MILES_PROPERTY_NAME ENV_F_REVERB_PREDELAY_SECS;    // ""
MILES_PROPERTY_NAME ENV_F_REVERB_DAMPING;          // ""
MILES_PROPERTY_NAME ENV_F_REVERB_MASTER_DRY_VOL;   // Wet/dry levels, with dB, linear, and loudness
MILES_PROPERTY_NAME ENV_F_REVERB_MASTER_WET_VOL;   // control alternatives
MILES_PROPERTY_NAME ENV_F_REVERB_MASTER_DRY_DB;    // 
MILES_PROPERTY_NAME ENV_F_REVERB_MASTER_WET_DB;    //
MILES_PROPERTY_NAME ENV_F_REVERB_MASTER_DRY_SCALE; // 
MILES_PROPERTY_NAME ENV_F_REVERB_MASTER_WET_SCALE; //
MILES_PROPERTY_NAME ENV_F_LISTENER_X_POS;          // Listener position, orientation, and velocity
MILES_PROPERTY_NAME ENV_F_LISTENER_X_FACE;         //
MILES_PROPERTY_NAME ENV_F_LISTENER_X_UP;           //
MILES_PROPERTY_NAME ENV_F_LISTENER_X_VEL;          //
MILES_PROPERTY_NAME ENV_F_LISTENER_Y_POS;          //
MILES_PROPERTY_NAME ENV_F_LISTENER_Y_FACE;         //
MILES_PROPERTY_NAME ENV_F_LISTENER_Y_UP;           //
MILES_PROPERTY_NAME ENV_F_LISTENER_Y_VEL;          //
MILES_PROPERTY_NAME ENV_F_LISTENER_Z_POS;          //
MILES_PROPERTY_NAME ENV_F_LISTENER_Z_FACE;         //
MILES_PROPERTY_NAME ENV_F_LISTENER_Z_UP;           //
MILES_PROPERTY_NAME ENV_F_LISTENER_Z_VEL;          //
MILES_PROPERTY_NAME ENV_V_LISTENER_POS;            //
MILES_PROPERTY_NAME ENV_V_LISTENER_FACE;           //
MILES_PROPERTY_NAME ENV_V_LISTENER_UP;             //
MILES_PROPERTY_NAME ENV_V_LISTENER_VEL;            //
MILES_PROPERTY_NAME ENV_F_LISTENER_MAG;            //
MILES_PROPERTY_NAME ENV_F_ROLLOFF;                 // Distance rolloff factor
MILES_PROPERTY_NAME ENV_F_DOPPLER;                 // Doppler scale factor
MILES_PROPERTY_NAME ENV_F_FALLOFF_POWER;           // Falloff power

#if 0 // TODO

MILES_RESULT MilesEnumerateEnvironmentProperties
                                            (HMENV               environment,
                                             HMILESENUM         *next,
                                             MILES_PROPERTY_INFO *dest);

MILES_RESULT MilesEnvironmentProperty(HMENV          environment,
                                      MILES_PROPERTY name,
                                      void *         before_value,
                                      void const *   new_value   DEFPARM(NULL),
                                      void *         after_value DEFPARM(NULL),
                                      void *         min_limit   DEFPARM(NULL),
                                      void *         max_limit   DEFPARM(NULL),
                                      char **        description DEFPARM(NULL));

MILES_RESULT MilesEnvironmentPropertyText(HMENV          E,
                                          MILES_PROPERTY name,
                                          char *         before_value,
                                          char const *   new_value   DEFPARM(NULL),
                                          char *         after_value DEFPARM(NULL),
                                          void *         min_limit   DEFPARM(NULL),
                                          void *         max_limit   DEFPARM(NULL),
                                          char **        description DEFPARM(NULL));

MILES_RESULT MilesUpdateListener3DPosition(HMENV environment,
                                           F32   dt_milliseconds);

MILES_PROPERTY_NAME ENV_F_UPDATE_LISTENER_POS_MS;  // Same as calling AIL_update_listener_3D_position()

MILES_RESULT MilesDeferEnvironmentSettings  (HMENV environment);
MILES_RESULT MilesCommitEnvironmentSettings (HMENV environment);

//
// TODO: helper function prototypes
//

//
// The new API lets you specify these speaker labels directly, rather than worrying 
// about how they map into the channel array for the current format...
//

typedef const S32 MILES_SPEAKER;

MILES_SPEAKER MILES_SPEAKER_FRONT_LEFT            = 0;
MILES_SPEAKER MILES_SPEAKER_FRONT_RIGHT           = 1;
MILES_SPEAKER MILES_SPEAKER_FRONT_CENTER          = 2;
MILES_SPEAKER MILES_SPEAKER_LFE                   = 3;
MILES_SPEAKER MILES_SPEAKER_BACK_LEFT             = 4;
MILES_SPEAKER MILES_SPEAKER_BACK_RIGHT            = 5;
MILES_SPEAKER MILES_SPEAKER_FRONT_LEFT_OF_CENTER  = 6;
MILES_SPEAKER MILES_SPEAKER_FRONT_RIGHT_OF_CENTER = 7;
MILES_SPEAKER MILES_SPEAKER_BACK_CENTER           = 8;
MILES_SPEAKER MILES_SPEAKER_SIDE_LEFT             = 9;
MILES_SPEAKER MILES_SPEAKER_SIDE_RIGHT            = 10;
MILES_SPEAKER MILES_SPEAKER_TOP_CENTER            = 11;
MILES_SPEAKER MILES_SPEAKER_TOP_FRONT_LEFT        = 12;
MILES_SPEAKER MILES_SPEAKER_TOP_FRONT_CENTER      = 13;
MILES_SPEAKER MILES_SPEAKER_TOP_FRONT_RIGHT       = 14;
MILES_SPEAKER MILES_SPEAKER_TOP_BACK_LEFT         = 15;
MILES_SPEAKER MILES_SPEAKER_TOP_BACK_CENTER       = 16;
MILES_SPEAKER MILES_SPEAKER_TOP_BACK_RIGHT        = 17;

//
// MilesSpeakerWetLevel():
// MilesSpeakerDryLevel(): 
// MilesSpeakerWetScale():
// MilesSpeakerDryScale(): 
// MilesSpeakerWetdB():
// MilesSpeakerDrydB(): Get/set an individual speaker's level in terms of loudness, dB, or amplitude
//
// MilesSpeakerLocation(): See AIL_set_speaker_location()
//
//

MILES_RESULT MilesSpeakerWetLevel (HMENV        environment, 
                                   MILES_SPEAKER speaker,
                                   F32         *before_value,
                                   F32 const   *new_value DEFPARM(NULL));

MILES_RESULT MilesSpeakerDryLevel (HMENV        environment, 
                                   MILES_SPEAKER speaker,
                                   F32         *before_value,
                                   F32 const   *new_value DEFPARM(NULL));

MILES_RESULT MilesSpeakerWetScale(HMENV        environment, 
                                  MILES_SPEAKER speaker,
                                  F32         *before_value,
                                  F32 const   *new_value DEFPARM(NULL));

MILES_RESULT MilesSpeakerDryScale(HMENV        environment, 
                                  MILES_SPEAKER speaker,
                                  F32         *before_value,
                                  F32 const   *new_value DEFPARM(NULL));

MILES_RESULT MilesSpeakerWetdB  (HMENV        environment, 
                                  MILES_SPEAKER speaker,
                                  F32         *before_value,
                                  F32 const   *new_value DEFPARM(NULL));

MILES_RESULT MilesSpeakerDrydB  (HMENV        environment, 
                                  MILES_SPEAKER speaker,
                                  F32         *before_value,
                                  F32 const   *new_value DEFPARM(NULL));

MILES_RESULT MilesSpeakerLocation(HMENV         environment, 
                                  MILES_SPEAKER speaker,
                                  MILES_VEC3   *before_value,
                                  MILES_VEC3   *new_value   DEFPARM(NULL),
                                  MILES_VEC3   *after_value DEFPARM(NULL));

// --------------------------------------------------------
// Sound/voice API
// --------------------------------------------------------

//
// MilesCreateSound(): Create an empty sound definition for a given environment
//
// MilesPlay(): Initiate playback of an HSOUND definition, optionally spawning a copy of
//   it to allow its properties to be polled/controlled independently of any other instances
//   of the sound
//
//   The sound is started using the parameters and attributes from the HMSOUND definition.  If
//   you want to override these without affecting any current or future playing instances, 
//   you must use MilesCopySound() to create a copy of the HMSOUND first.  If you only want to
//   poll the sound instance or control it after playback begins, you can simply provide a
//   destination HMSOUND pointer to MilesPlay().  Otherwise, you just call MilesPlay() to 
//   fire-and-forget the sound instance without any further interaction.
//
// MilesCopySound(): Create a sound definition that's identical to an existing one.  Used
//   when you want to set new properties for an HMSOUND definition *before* it starts to
//   play
//
// MilesDestroySound(): Destroy the sound definition and all of its internal references.  The
//   environment will clean up its own HMSOUNDs at destruction time, but the function must be
//   called explicitly for any HMSOUND copies obtained from MilesPlay() or MilesCopySound()
//
// MilesEnumerateSounds(): Enumerate all sound definitions in environment, optionally 
//   qualified by tag(s).  This will include sound definitions that may have been 
//   loaded from the project file, as well as any new definitions created since the 
//   project was loaded
//
// MilesSound(): Shortcut function that returns the first HMSOUND whose tagset property
//   matches the specified string; handy when composing simple fire-and-forget calls 
//   like MilesPlay(MilesSound("AK-47"));
//

MILES_RESULT MilesCreateSound  (HMENV    environment,
                                HMSOUND *sound);

MILES_RESULT MilesCopySound    (HMSOUND  original,
                                HMSOUND *copy);

MILES_RESULT MilesPlay         (HMSOUND  sound,
                                HMSOUND *copy DEFPARM(NULL));

MILES_RESULT MilesDestroySound (HMSOUND sound);

MILES_RESULT MilesEnumerateSounds
                               (HMENV       environment,
                                HMILESENUM *next,
                                HMSOUND    *dest,
                                char       *tagset DEFPARM(NULL));

HMSOUND      MilesSound        (HMENV      environment,
                                char      *tagset);
//
// MilesSetSoundFile(): Associate a discrete .wav, .mp3, .ogg, or other file with this
//   sound object.  Only one file may be associated with any given sound definition, but
//   nothing prevents you from creating a sound definition containing nothing but script tags
//   that trigger multiple HMSOUNDs...
//
//   This is also what adds most of the attributes to the HMSOUND.  (Not all 
//   attributes may apply to all file types...)
//
//   TODO: other versions that work with wave libraries created by RAD tool, and 
//     some way to deal with memory-resident wave images
//
//   TODO: Support multichannel source files by creating HMSOUNDS that can deal with
//     multiple bound HSAMPLES? (Multichannel streams would need the files to be interleaved 
//     offline...)
//

MILES_RESULT MilesSetSoundFile(HMSOUND sound,
                               char   *filename);

//
// MilesPreload(): Preloads the sound's file or opens its stream, optionally forcing 
//   an HSAMPLE to be allocated to make the parameters available for inspection/modification 
//   prior to playback
//

MILES_RESULT MilesPreload(HMSOUND sound,
                          BOOL    assign_voice);

//
// MilesDiscard(): Forces the system to free memory used by sound definition
//   to store RAM-resident files and voices
//

MILES_RESULT MilesDiscard(HMSOUND sound);

// 
// MilesPause(): Stops playback of any/all instances of a given HMSOUND without releasing their
//               physical voice(s), if any
//
// MilesResume(): Resumes playback of any/all paused instances of a given HMSOUND
//
// MilesEnd(): Kills any/all instances of a given HMSOUND, releasing their virtual and physical
//             voices
//
// MilesServe(): Same as AIL_service_stream()
//

MILES_RESULT MilesPause(HMSOUND sound);

MILES_RESULT MilesResume(HMSOUND sound);

MILES_RESULT MilesEnd(HMSOUND sound);

MILES_RESULT MilesServe(HMSOUND sound);

//
// Sound properties
//
// The HMSOUND contains values for each of these properties that serve as initial defaults
// when the sound is played.  These initial defaults are set in the RAD tool.  If you want 
// to alter any of these properties without affecting the original HMSOUND, you can 
// use Milescopy_sound().
//
// Any altered HMSOUND properties will propagate to all playing instances of the HMSOUND.  If
//   this isn't what you want, you should play a copy of the HMSOUND instead
//
// MilesSoundProperty(): Set or retrieve a specified property.  We also want to
//  to define a shortcut macro or inline function for each property name, so people don't have to
//  call an unsafe function with 5 parameters...
//
//  Note that some properties don't apply to sound definitions at all.  It doesn't make sense to
//  query the status or play-cursor position of an HMSOUND by itself, for instance.  For these 
//  properties, the value for the most-recently-triggered instance will be returned
//
// MilesSoundPropertyText(): Text-based wrapper for MilesSoundProperty()
//
// MilesUpdateSound3DPosition(): Same as AIL_update_sound_3D_position()
//
// MilesDefer/CommitSoundSettings(): When deferred, the sound's settings
//  are updated with new property values, but they aren't used to update the HSAMPLE/HSTREAM.
//  The commit function copies any/all altered values to the HSAMPLE/HSTREAM at once, using 
//  an AIL_lock() clause
//
// MilesSoundChannelLevel()
// MilesSoundChannelScale()
// MilesSoundChanneldB(): Individual speaker level control, a la AIL_set_sample_channel_levels()
//
// TODO: Resumption behavior when sound's voice is recovered after theft (one-shot, resumable,
//   time-tracked)
//

MILES_PROPERTY_NAME SOUND_SZ_TAGSET;               // String containing list of tags for sound
MILES_PROPERTY_NAME SOUND_STATUS;                  // Playback status
MILES_PROPERTY_NAME SOUND_SZ_FILENAME;             // Source filename
MILES_PROPERTY_NAME SOUND_L_FILE_BYTES;            // Size of source filename in bytes
MILES_PROPERTY_NAME SOUND_N_PLAYBACK_RATE_HZ;      // Playback rate
MILES_PROPERTY_NAME SOUND_N_ORIGINAL_RATE_HZ;      // Read-only property
MILES_PROPERTY_NAME SOUND_N_SOURCE_CHANNELS;       // Read-only property
MILES_PROPERTY_NAME SOUND_B_IS_STREAM;             // Whether it's streaming or static
MILES_PROPERTY_NAME SOUND_B_IS_AUTO_STREAM;        // If it's a stream, is it serviced automatically?
MILES_PROPERTY_NAME SOUND_N_STREAM_BUFFER_BYTES;   // If it's a stream, how big is the buffer? (-1=default)
MILES_PROPERTY_NAME SOUND_N_PRIORITY;              // Priority level
MILES_PROPERTY_NAME SOUND_F_VOL;                   // Base volume for this HSOUND in dB, linear amplitude, 
MILES_PROPERTY_NAME SOUND_F_DB;                    //  and loudness
MILES_PROPERTY_NAME SOUND_F_SCALE;                 // 
MILES_PROPERTY_NAME SOUND_F_LR_PAN;                // Left-right pan
MILES_PROPERTY_NAME SOUND_F_FB_PAN;                // Front-back pan
MILES_PROPERTY_NAME SOUND_F_CENTER_VOL;            // Center and LFE volume
MILES_PROPERTY_NAME SOUND_F_CENTER_DB;             // 
MILES_PROPERTY_NAME SOUND_F_CENTER_SCALE;          // 
MILES_PROPERTY_NAME SOUND_F_LFE_VOL;               //
MILES_PROPERTY_NAME SOUND_F_LFE_DB;                // 
MILES_PROPERTY_NAME SOUND_F_LFE_SCALE;             // 
MILES_PROPERTY_NAME SOUND_F_WET_VOL;               // Wet and dry volume for built-in reverb
MILES_PROPERTY_NAME SOUND_F_WET_DB;                // 
MILES_PROPERTY_NAME SOUND_F_WET_SCALE;             // 
MILES_PROPERTY_NAME SOUND_F_DRY_VOL;               //
MILES_PROPERTY_NAME SOUND_F_DRY_DB;                // 
MILES_PROPERTY_NAME SOUND_F_DRY_SCALE;             // 
MILES_PROPERTY_NAME SOUND_F_LPF_CUTOFF;            // Built-in LPF cutoff (as fraction of Nyquist)
MILES_PROPERTY_NAME SOUND_L_PLAYBACK_OFFSET_BYTES; // Play cursor in bytes and ms
MILES_PROPERTY_NAME SOUND_F_PLAYBACK_POSITION_MS;  //
MILES_PROPERTY_NAME SOUND_L_TOTAL_LEN_BYTES;       // Total length in bytes and ms
MILES_PROPERTY_NAME SOUND_F_DURATION_MS;           //
MILES_PROPERTY_NAME SOUND_N_INITIAL_LOOP_COUNT;    // Original loop count to apply to triggered sounds
MILES_PROPERTY_NAME SOUND_N_REMAINING_LOOP_COUNT;  // Loops remaining (readable/writable)
MILES_PROPERTY_NAME SOUND_L_LOOP_START_BYTES;      // Loop boundaries in bytes
MILES_PROPERTY_NAME SOUND_L_LOOP_END_BYTES;        // 
MILES_PROPERTY_NAME SOUND_CB_EOS;                  // End-of-sound callback handler
MILES_PROPERTY_NAME SOUND_SINTa_USER;              // User data field
MILES_PROPERTY_NAME SOUND_F_OCCLUSION;             // Occlusion  
MILES_PROPERTY_NAME SOUND_F_OBSTRUCTION;           // Obstruction
MILES_PROPERTY_NAME SOUND_F_EXCLUSION;             // Exclusion  
MILES_PROPERTY_NAME SOUND_B_IS_3D;                 // TRUE if this sound is 3D-positioned
MILES_PROPERTY_NAME SOUND_F_3D_X_POS;              // 3D position, orientation, velocity as X,Y,Z 
MILES_PROPERTY_NAME SOUND_F_3D_X_FACE;             //
MILES_PROPERTY_NAME SOUND_F_3D_X_UP;               //
MILES_PROPERTY_NAME SOUND_F_3D_X_VEL;              //
MILES_PROPERTY_NAME SOUND_F_3D_Y_POS;              //
MILES_PROPERTY_NAME SOUND_F_3D_Y_FACE;             //
MILES_PROPERTY_NAME SOUND_F_3D_Y_UP;               //
MILES_PROPERTY_NAME SOUND_F_3D_Y_VEL;              //
MILES_PROPERTY_NAME SOUND_F_3D_Z_POS;              //
MILES_PROPERTY_NAME SOUND_F_3D_Z_FACE;             //
MILES_PROPERTY_NAME SOUND_F_3D_Z_UP;               //
MILES_PROPERTY_NAME SOUND_F_3D_Z_VEL;              //
MILES_PROPERTY_NAME SOUND_V_3D_POS;                // 3D position, orientation, velocity as VEC3
MILES_PROPERTY_NAME SOUND_V_3D_FACE;               //
MILES_PROPERTY_NAME SOUND_V_3D_UP;                 //
MILES_PROPERTY_NAME SOUND_V_3D_VEL;                //
MILES_PROPERTY_NAME SOUND_F_3D_MAG;                //
MILES_PROPERTY_NAME SOUND_F_3D_CONE_INNER_DEGS;    // 3D cone inner angle, outer angle, volume
MILES_PROPERTY_NAME SOUND_F_3D_CONE_INNER_RADS;    //
MILES_PROPERTY_NAME SOUND_F_3D_CONE_OUTER_DEGS;    //
MILES_PROPERTY_NAME SOUND_F_3D_CONE_OUTER_RADS;    //
MILES_PROPERTY_NAME SOUND_F_3D_CONE_OUTER_VOL;     //
MILES_PROPERTY_NAME SOUND_F_3D_CONE_OUTER_DB;      //
MILES_PROPERTY_NAME SOUND_F_3D_CONE_OUTER_SCALE;   //
MILES_PROPERTY_NAME SOUND_F_3D_MIN_DIST;           // 3D min/max distance
MILES_PROPERTY_NAME SOUND_F_3D_MAX_DIST;           //
MILES_PROPERTY_NAME SOUND_CB_3D_FALLOFF;           // 3D falloff-function callback handler
MILES_PROPERTY_NAME SOUND_B_3D_AUTO_WET_ATTEN;     // 3D auto wet atten 
MILES_PROPERTY_NAME SOUND_L_UNDERLYING_HSAMPLE;    // Temporary for demo/test

//MILES_RESULT MilesEnumerateSoundProperties
//                                            (HMSOUND              sound,
//                                             HMILESENUM          *next,
//                                             MILES_PROPERTY_INFO *dest);
//
//MILES_RESULT MilesSoundProperty (HMSOUND        sound,
//                                 MILES_PROPERTY name,
//                                 void *         before_value DEFPARM(NULL),
//                                 void const *   new_value    DEFPARM(NULL),
//                                 void *         after_value  DEFPARM(NULL),
//                                 void *         min_limit    DEFPARM(NULL),
//                                 void *         max_limit    DEFPARM(NULL),
//                                 char **        description  DEFPARM(NULL));
//
//MILES_RESULT MilesSoundPropertyText (HMSOUND        sound,
//                                     MILES_PROPERTY name,
//                                     char *         before_value DEFPARM(NULL),
//                                     char const *   new_value    DEFPARM(NULL),
//                                     char *         after_value  DEFPARM(NULL),
//                                     void *         min_limit    DEFPARM(NULL),
//                                     void *         max_limit    DEFPARM(NULL),
//                                     char **        description  DEFPARM(NULL));

MILES_RESULT MilesUpdateSound3DPosition (MILES_SOUND_INSTANCE sound,
                                         F32                  dt_milliseconds);

MILES_RESULT MilesSoundChannelLevel (MILES_SOUND_INSTANCE       sound,
                                     MILES_SPEAKER channel,
                                     F32          *before_value,
                                     F32 const    *new_value DEFPARM(NULL));

MILES_RESULT MilesSoundChannelScale (MILES_SOUND_INSTANCE       sound,
                                     MILES_SPEAKER channel,
                                     F32          *before_value,
                                     F32 const    *new_value DEFPARM(NULL));

MILES_RESULT MilesSoundChanneldB   (MILES_SOUND_INSTANCE       sound,
                                    MILES_SPEAKER channel,
                                    F32          *before_value,
                                    F32 const    *new_value DEFPARM(NULL));

MILES_RESULT MilesDeferSoundSettings  (MILES_SOUND_INSTANCE sound);
MILES_RESULT MilesCommitSoundSettings (MILES_SOUND_INSTANCE sound);

//
// Filter pipeline API
//
// TODO: Set of property-helper functions for all filters we ship
//

MILES_RESULT MilesEnumerateFilters (HMENV      environment,
                                    HPROENUM  *next,
                                    char     **filter_name);

MILES_RESULT MilesInstallSoundFilter(MILES_SOUND_INSTANCE            sound,
                                     MILES_SAMPLE_STAGE stage,
                                     char              *filter_name);

MILES_RESULT MilesSoundFilter       (MILES_SOUND_INSTANCE            sound,
                                     MILES_SAMPLE_STAGE stage,
                                     char             **filter_name);

MILES_RESULT MilesEnumerateFilterProperties (HMENV                environment,
                                             char                *filter_name,
                                             HMILESENUM          *next,
                                             RIB_INTERFACE_ENTRY *dest);

MILES_RESULT MilesSoundFilterProperty(MILES_SOUND_INSTANCE            sound,
                                      MILES_SAMPLE_STAGE stage,
                                      char              *property_name DEFPARM(NULL),
                                      void *             before_value  DEFPARM(NULL),
                                      void const *       new_value     DEFPARM(NULL),
                                      void *             after_value   DEFPARM(NULL),
                                      void *             min_limit     DEFPARM(NULL),
                                      void *             max_limit     DEFPARM(NULL),
                                      char **            description   DEFPARM(NULL));

//
// Tag API
// 
// MilesAddTag(): Add tag(s) to the environment, making them available for enumeration and
//   assignment to individual sounds
//
// MilesRemoveTag(): Remove tag(s) registered with the environment.  References to the
//   tag(s) will also be removed from any HSOUNDs
//
// MilesAddSoundTag(): Add tag(s) to the sound.  Any tag(s) that have not yet been registered 
//   in the HMSOUND's environment will be added to the environment as well
//
// MilesRemoveSoundTag(): Remove tag(s) from this sound definition (NULL to remove all).  Tags are
//   not automatically removed from the sound's environment, even when no other sounds use them
//
// MilesEnumerateTags(): Enumerates all tag(s) associated with sound definition or
//   environment
//
// MilesSoundHasTag(): Returns TRUE if the HSOUND has _all_ of the specified tag(s)
//
// MilesSetTagScript(): Associates a script with the specified tag(s).  The script will
//   execute for all sounds with these tag(s).  Only one script can be associated with any given
//   tag, but any number of script-bearing tags may be associated with any sound definition
// 

MILES_RESULT MilesAddTag   (HMENV  environment,
                            char  *tagset);

MILES_RESULT MilesRemoveTag (HMENV  environment,
                             char  *tagset);

MILES_RESULT MilesAddSoundTag (HMSOUND sound,
                               char   *tagset);

MILES_RESULT MilesRemoveSoundTag(HMSOUND sound,
                                 char   *tagset);

MILES_RESULT MilesEnumerateTags(HMENV       environment,
                                HMSOUND     sound,
                                HMILESENUM *next,
                                char       *tag);

BOOL         MilesSoundHasTag (HMSOUND sound,
                               char   *tagset);

MILES_RESULT MilesSetTagScript(HMENV environment,
                               char *tagset,
                               char *script);

//
// Examples of property helper functions 
// C++ files get type checking via inline functions; C files get macro suckiness
//

#if 0

#ifdef __cplusplus
   inline U32 MilesSoundStatus(MILES_SOUND_INSTANCE sound)
      {
      U32 temp = 0;
      MilesSoundProperty(sound, SOUND_STATUS, &temp, NULL, NULL);
      return temp;
      }
#else  // TODO: any way to create a unique temporary so the macro can return a value directly? token-paste "temp"+the line #, maybe?
   #define MilesSoundStatus(sound,status) MilesSoundProperty(sound, SOUND_STATUS, status, NULL, NULL);
#endif

#ifdef __cplusplus
   inline S32 MilesSoundPlaybackRateHz(MILES_SOUND_INSTANCE sound)
      {
      S32 temp = 0;
      return MilesSoundProperty(sound, SOUND_N_PLAYBACK_RATE_HZ, &temp, NULL, NULL);
      return temp;
      }
#else  // TODO: any way to create a unique temporary so the macro can return a value directly? token-paste "temp"+the line #, maybe?
   #define MilesSoundPlaybackRateHz(sound,playback_rate) MilesSoundProperty(sound, SOUND_N_PLAYBACK_RATE_HZ, (playback_rate_Hz), NULL, NULL);
#endif

#ifdef __cplusplus
   inline MILES_RESULT MilesSetSoundPlaybackRateHz(MILES_SOUND_INSTANCE sound, U32 playback_rate_Hz)
      {
      return MilesSoundProperty(sound, SOUND_N_PLAYBACK_RATE_HZ, NULL, &playback_rate_Hz, NULL);
      }
#else 
   #define MilesSetSoundPlaybackRateHz(sound,playback_rate) { U32 temp = (playback_rate); MilesSoundProperty(sound, SOUND_N_PLAYBACK_RATE_HZ, NULL, &temp, NULL); }
#endif

#endif // end TODO

//
// ...
//

#endif

#ifdef __cplusplus
}
#endif

#endif
