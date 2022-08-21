//############################################################################
//##                                                                        ##
//##  MARS.CPP                                                              ##
//##                                                                        ##
//##  Miles Audio Rendering System high-level API                           ##
//##                                                                        ##
//##  V1.00 of 19-Feb-07: Initial version                                   ##
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

#include <stdlib.h>
#include <assert.h>
#include <math.h>

//#include <paramon.h>
#define debug_printf

#include <windows.h>

#include <stdio.h>
#include <time.h>

#include "mars.h"
#include <imssapi.h>

#define MAX_FILENAME_BYTES 512
#define DEG2RADF 0.017453292F
#define RAD2DEGF 57.29577951F

#ifdef IS_WIN64

  #define PTRFMT "0x%I64X"
  #define SINTFMT "%I64d"
  #define I64FMT "%I64d"
  #define HPFMT "%ld"

#else

  #define PTRFMT "0x%lX"
  #define SINTFMT "%ld"
  #define I64FMT "%I64d"      // TODO: Win32 different from everything else
  #define HPFMT "%ld"

#endif

void _________________________________________________Exported_constants_____________________________________________________(){}

MARS_PROPERTY ENV_N_PLAYING_VOICES           = "Playing voices";          
MARS_PROPERTY ENV_F_LATENCY_MS               = "Aggregate latency (ms)";                 
MARS_PROPERTY ENV_N_CPU_PERCENT              = "Mixer CPU percent";       
MARS_PROPERTY ENV_F_MASTER_VOLUME            = "Master volume";           
MARS_PROPERTY ENV_F_MASTER_VOLUME_DB         = "Master volume dB";        
MARS_PROPERTY ENV_F_MASTER_VOLUME_SCALE      = "Master volume scalar";     
MARS_PROPERTY ENV_RTYPE_REVERB_ROOM_TYPE     = "Room type";                
MARS_PROPERTY ENV_F_REVERB_TIME_SECS         = "Reverb time (seconds)";     
MARS_PROPERTY ENV_F_REVERB_PREDELAY_SECS     = "Reverb predelay (seconds)";  
MARS_PROPERTY ENV_F_REVERB_DAMPING           = "Reverb damping factor";   
MARS_PROPERTY ENV_F_REVERB_MASTER_DRY_VOL    = "Master dry volume";        
MARS_PROPERTY ENV_F_REVERB_MASTER_WET_VOL    = "Master wet volume";        
MARS_PROPERTY ENV_F_REVERB_MASTER_DRY_DB     = "Master dry volume dB";     
MARS_PROPERTY ENV_F_REVERB_MASTER_WET_DB     = "Master wet volume dB";     
MARS_PROPERTY ENV_F_REVERB_MASTER_DRY_SCALE  = "Master dry volume scalar";    
MARS_PROPERTY ENV_F_REVERB_MASTER_WET_SCALE  = "Master wet volume scalar";    
MARS_PROPERTY ENV_F_LISTENER_X_POS           = "3D listener X";                    
MARS_PROPERTY ENV_F_LISTENER_X_FACE          = "3D listener X face";               
MARS_PROPERTY ENV_F_LISTENER_X_UP            = "3D listener X up";                 
MARS_PROPERTY ENV_F_LISTENER_X_VEL           = "3D listener X velocity";           
MARS_PROPERTY ENV_F_LISTENER_Y_POS           = "3D listener Y";                    
MARS_PROPERTY ENV_F_LISTENER_Y_FACE          = "3D listener Y face";               
MARS_PROPERTY ENV_F_LISTENER_Y_UP            = "3D listener Y up";                 
MARS_PROPERTY ENV_F_LISTENER_Y_VEL           = "3D listener Y velocity";           
MARS_PROPERTY ENV_F_LISTENER_Z_POS           = "3D listener Z";                    
MARS_PROPERTY ENV_F_LISTENER_Z_FACE          = "3D listener Z face";               
MARS_PROPERTY ENV_F_LISTENER_Z_UP            = "3D listener Z up";                 
MARS_PROPERTY ENV_F_LISTENER_Z_VEL           = "3D listener Z velocity";           
MARS_PROPERTY ENV_V_LISTENER_POS             = "3D listener position vector";      
MARS_PROPERTY ENV_V_LISTENER_FACE            = "3D listener face vector";          
MARS_PROPERTY ENV_V_LISTENER_UP              = "3D listener up vector";            
MARS_PROPERTY ENV_V_LISTENER_VEL             = "3D listener velocity direction";   
MARS_PROPERTY ENV_F_LISTENER_MAG             = "3D listener velocity magnitude";   
MARS_PROPERTY ENV_F_ROLLOFF                  = "3D rolloff factor";       
MARS_PROPERTY ENV_F_DOPPLER                  = "3D Doppler factor";       
MARS_PROPERTY ENV_F_FALLOFF_POWER            = "3D speaker falloff power";

enum
{
   ENUM_ENV_N_PLAYING_VOICES,         
   ENUM_ENV_F_LATENCY_MS,             
   ENUM_ENV_N_CPU_PERCENT,            
   ENUM_ENV_F_MASTER_VOLUME,          
   ENUM_ENV_F_MASTER_VOLUME_DB,       
   ENUM_ENV_F_MASTER_VOLUME_SCALE,    
   ENUM_ENV_RTYPE_REVERB_ROOM_TYPE,   
   ENUM_ENV_F_REVERB_TIME_SECS,       
   ENUM_ENV_F_REVERB_PREDELAY_SECS,   
   ENUM_ENV_F_REVERB_DAMPING,         
   ENUM_ENV_F_REVERB_MASTER_DRY_VOL,  
   ENUM_ENV_F_REVERB_MASTER_WET_VOL,  
   ENUM_ENV_F_REVERB_MASTER_DRY_DB,   
   ENUM_ENV_F_REVERB_MASTER_WET_DB,   
   ENUM_ENV_F_REVERB_MASTER_DRY_SCALE,
   ENUM_ENV_F_REVERB_MASTER_WET_SCALE,
   ENUM_ENV_F_LISTENER_X_POS,         
   ENUM_ENV_F_LISTENER_X_FACE,        
   ENUM_ENV_F_LISTENER_X_UP,          
   ENUM_ENV_F_LISTENER_X_VEL,         
   ENUM_ENV_F_LISTENER_Y_POS,         
   ENUM_ENV_F_LISTENER_Y_FACE,        
   ENUM_ENV_F_LISTENER_Y_UP,          
   ENUM_ENV_F_LISTENER_Y_VEL,         
   ENUM_ENV_F_LISTENER_Z_POS,         
   ENUM_ENV_F_LISTENER_Z_FACE,        
   ENUM_ENV_F_LISTENER_Z_UP,          
   ENUM_ENV_F_LISTENER_Z_VEL,         
   ENUM_ENV_V_LISTENER_POS,           
   ENUM_ENV_V_LISTENER_FACE,          
   ENUM_ENV_V_LISTENER_UP,            
   ENUM_ENV_V_LISTENER_VEL,           
   ENUM_ENV_F_LISTENER_MAG,           
   ENUM_ENV_F_ROLLOFF,                
   ENUM_ENV_F_DOPPLER,                
   ENUM_ENV_F_FALLOFF_POWER,          
};

const MARS_PROPERTY_INFO ENV_properties[] =
{ 
 { ENV_N_PLAYING_VOICES         , MP_S32,           MPF_READ_ONLY | MPF_UNSAVED },
 { ENV_F_LATENCY_MS             , MP_F32,           MPF_READ_ONLY | MPF_UNSAVED },    
 { ENV_N_CPU_PERCENT            , MP_S32,           MPF_READ_ONLY | MPF_UNSAVED },
 { ENV_F_MASTER_VOLUME          , MP_F32,           MPF_ALTERNATE               },
 { ENV_F_MASTER_VOLUME_DB       , MP_F32,           MPF_ALTERNATE               },
 { ENV_F_MASTER_VOLUME_SCALE    , MP_F32,           0                           },
 { ENV_RTYPE_REVERB_ROOM_TYPE   , MP_ROOM_TYPE,     0                           },  
 { ENV_F_REVERB_TIME_SECS       , MP_F32,           0                           },
 { ENV_F_REVERB_PREDELAY_SECS   , MP_F32,           0                           },
 { ENV_F_REVERB_DAMPING         , MP_F32,           0                           },
 { ENV_F_REVERB_MASTER_DRY_VOL  , MP_F32,           MPF_ALTERNATE               },
 { ENV_F_REVERB_MASTER_WET_VOL  , MP_F32,           MPF_ALTERNATE               },
 { ENV_F_REVERB_MASTER_DRY_DB   , MP_F32,           MPF_ALTERNATE               },
 { ENV_F_REVERB_MASTER_WET_DB   , MP_F32,           MPF_ALTERNATE               },
 { ENV_F_REVERB_MASTER_DRY_SCALE, MP_F32,           0                           },
 { ENV_F_REVERB_MASTER_WET_SCALE, MP_F32,           0                           },
 { ENV_F_LISTENER_X_POS         , MP_F32,           MPF_ALTERNATE               },
 { ENV_F_LISTENER_X_FACE        , MP_F32,           MPF_ALTERNATE               },
 { ENV_F_LISTENER_X_UP          , MP_F32,           MPF_ALTERNATE               },
 { ENV_F_LISTENER_X_VEL         , MP_F32,           MPF_ALTERNATE               },
 { ENV_F_LISTENER_Y_POS         , MP_F32,           MPF_ALTERNATE               },
 { ENV_F_LISTENER_Y_FACE        , MP_F32,           MPF_ALTERNATE               },
 { ENV_F_LISTENER_Y_UP          , MP_F32,           MPF_ALTERNATE               },
 { ENV_F_LISTENER_Y_VEL         , MP_F32,           MPF_ALTERNATE               },
 { ENV_F_LISTENER_Z_POS         , MP_F32,           MPF_ALTERNATE               },
 { ENV_F_LISTENER_Z_FACE        , MP_F32,           MPF_ALTERNATE               },
 { ENV_F_LISTENER_Z_UP          , MP_F32,           MPF_ALTERNATE               },
 { ENV_F_LISTENER_Z_VEL         , MP_F32,           MPF_ALTERNATE               },
 { ENV_V_LISTENER_POS           , MP_VEC3,          0                           },
 { ENV_V_LISTENER_FACE          , MP_VEC3,          0                           },
 { ENV_V_LISTENER_UP            , MP_VEC3,          0                           },
 { ENV_V_LISTENER_VEL           , MP_VEC3,          0                           },
 { ENV_F_LISTENER_MAG           , MP_F32,           0                           },
 { ENV_F_ROLLOFF                , MP_F32,           0                           },
 { ENV_F_DOPPLER                , MP_F32,           0                           },
 { ENV_F_FALLOFF_POWER          , MP_F32,           0                           },
};

MARS_PROPERTY SOUND_STATUS                  = "Playback status";          
MARS_PROPERTY SOUND_SZ_FILENAME             = "Filename";                 
MARS_PROPERTY SOUND_L_FILE_BYTES            = "File size (bytes)";        
MARS_PROPERTY SOUND_N_PLAYBACK_RATE_HZ      = "Playback rate Hz";                  
MARS_PROPERTY SOUND_N_ORIGINAL_RATE_HZ      = "Original sample rate Hz";           
MARS_PROPERTY SOUND_N_SOURCE_CHANNELS       = "Source channels";          
MARS_PROPERTY SOUND_B_IS_STREAM             = "Is streamed";                       
MARS_PROPERTY SOUND_B_IS_AUTO_STREAM        = "Automatic stream service";          
MARS_PROPERTY SOUND_N_STREAM_BUFFER_BYTES   = "Stream buffer (bytes)";    
MARS_PROPERTY SOUND_N_PRIORITY              = "Priority";                          
MARS_PROPERTY SOUND_F_VOL                   = "Volume";                        
MARS_PROPERTY SOUND_F_DB                    = "Volume dB";                         
MARS_PROPERTY SOUND_F_SCALE                 = "Volume scalar";                     
MARS_PROPERTY SOUND_F_LR_PAN                = "Left-right pan";                    
MARS_PROPERTY SOUND_F_FB_PAN                = "Front-back pan";                    
MARS_PROPERTY SOUND_F_CENTER_VOL            = "Center volume";                     
MARS_PROPERTY SOUND_F_CENTER_DB             = "Center volume dB";                  
MARS_PROPERTY SOUND_F_CENTER_SCALE          = "Center volume scalar";              
MARS_PROPERTY SOUND_F_LFE_VOL               = "LFE volume";                        
MARS_PROPERTY SOUND_F_LFE_DB                = "LFE volume dB";                     
MARS_PROPERTY SOUND_F_LFE_SCALE             = "LFE volume scalar";                 
MARS_PROPERTY SOUND_F_WET_VOL               = "Wet level";                         
MARS_PROPERTY SOUND_F_WET_DB                = "Wet level dB";                      
MARS_PROPERTY SOUND_F_WET_SCALE             = "Wet level scalar";                  
MARS_PROPERTY SOUND_F_DRY_VOL               = "Dry level";                         
MARS_PROPERTY SOUND_F_DRY_DB                = "Dry level dB";                      
MARS_PROPERTY SOUND_F_DRY_SCALE             = "Dry level scalar";                  
MARS_PROPERTY SOUND_F_LPF_CUTOFF            = "Built-in LPF cutoff";               
MARS_PROPERTY SOUND_L_INITIAL_OFFSET_BYTES  = "Initial play cursor (byte offset)";
MARS_PROPERTY SOUND_L_PLAYBACK_OFFSET_BYTES = "Play cursor (byte offset)";         
MARS_PROPERTY SOUND_F_PLAYBACK_POSITION_MS  = "Play cursor (msec)";                
MARS_PROPERTY SOUND_L_TOTAL_LEN_BYTES       = "Total length (bytes)";              
MARS_PROPERTY SOUND_F_DURATION_MS           = "Total duration (msec)";             
MARS_PROPERTY SOUND_N_INITIAL_LOOP_COUNT    = "Initial loop count";                        
MARS_PROPERTY SOUND_N_REMAINING_LOOP_COUNT  = "Loops remaining";                   
MARS_PROPERTY SOUND_L_LOOP_START_BYTES      = "Loop start offset (bytes)";         
MARS_PROPERTY SOUND_L_LOOP_END_BYTES        = "Loop end offset (bytes)";           
MARS_PROPERTY SOUND_CB_EOS                  = "End-of-sample callback";            
MARS_PROPERTY SOUND_SINTa_USER              = "User data";                         
MARS_PROPERTY SOUND_F_OCCLUSION             = "Occlusion";                         
MARS_PROPERTY SOUND_F_OBSTRUCTION           = "Obstruction";                       
MARS_PROPERTY SOUND_F_EXCLUSION             = "Exclusion";                         
MARS_PROPERTY SOUND_B_IS_3D                 = "Is 3D-positioned";                  
MARS_PROPERTY SOUND_F_3D_X_POS              = "3D X position";                     
MARS_PROPERTY SOUND_F_3D_X_FACE             = "3D X face";                         
MARS_PROPERTY SOUND_F_3D_X_UP               = "3D X up";                  
MARS_PROPERTY SOUND_F_3D_X_VEL              = "3D X velocity";                     
MARS_PROPERTY SOUND_F_3D_Y_POS              = "3D Y position";                     
MARS_PROPERTY SOUND_F_3D_Y_FACE             = "3D Y face";                         
MARS_PROPERTY SOUND_F_3D_Y_UP               = "3D Y up";                  
MARS_PROPERTY SOUND_F_3D_Y_VEL              = "3D Y velocity";                     
MARS_PROPERTY SOUND_F_3D_Z_POS              = "3D Z position";                     
MARS_PROPERTY SOUND_F_3D_Z_FACE             = "3D Z face";                         
MARS_PROPERTY SOUND_F_3D_Z_UP               = "3D Z up";                  
MARS_PROPERTY SOUND_F_3D_Z_VEL              = "3D Z velocity";                     
MARS_PROPERTY SOUND_V_3D_POS                = "3D position";                       
MARS_PROPERTY SOUND_V_3D_FACE               = "3D face vector";                    
MARS_PROPERTY SOUND_V_3D_UP                 = "3D up vector";             
MARS_PROPERTY SOUND_V_3D_VEL                = "3D normalized velocity";            
MARS_PROPERTY SOUND_F_3D_MAG                = "3D velocity magnitude";             
MARS_PROPERTY SOUND_F_3D_CONE_INNER_DEGS    = "3D cone inner degrees";             
MARS_PROPERTY SOUND_F_3D_CONE_INNER_RADS    = "3D cone inner radians";             
MARS_PROPERTY SOUND_F_3D_CONE_OUTER_DEGS    = "3D cone outer degrees";             
MARS_PROPERTY SOUND_F_3D_CONE_OUTER_RADS    = "3D cone outer radians";             
MARS_PROPERTY SOUND_F_3D_CONE_OUTER_VOL     = "3D cone outer volume";              
MARS_PROPERTY SOUND_F_3D_CONE_OUTER_DB      = "3D cone outer dB";                  
MARS_PROPERTY SOUND_F_3D_CONE_OUTER_SCALE   = "3D cone outer scalar";              
MARS_PROPERTY SOUND_F_3D_MIN_DIST           = "3D minimum distance";               
MARS_PROPERTY SOUND_F_3D_MAX_DIST           = "3D maximum distance";               
MARS_PROPERTY SOUND_CB_3D_FALLOFF           = "3D falloff callback";               
MARS_PROPERTY SOUND_B_3D_AUTO_WET_ATTEN     = "3D wet reverb attenuation";         
MARS_PROPERTY SOUND_L_UNDERLYING_HSAMPLE    = "Underlying HSAMPLE";

enum 
{
   ENUM_SOUND_STATUS,                  
   ENUM_SOUND_SZ_FILENAME,             
   ENUM_SOUND_L_FILE_BYTES,            
   ENUM_SOUND_N_PLAYBACK_RATE_HZ,      
   ENUM_SOUND_N_ORIGINAL_RATE_HZ,      
   ENUM_SOUND_N_SOURCE_CHANNELS,       
   ENUM_SOUND_B_IS_STREAM,             
   ENUM_SOUND_B_IS_AUTO_STREAM,        
   ENUM_SOUND_N_STREAM_BUFFER_BYTES,   
   ENUM_SOUND_N_PRIORITY,              
   ENUM_SOUND_F_VOL,                   
   ENUM_SOUND_F_DB,                    
   ENUM_SOUND_F_SCALE,                 
   ENUM_SOUND_F_LR_PAN,                
   ENUM_SOUND_F_FB_PAN,                
   ENUM_SOUND_F_CENTER_VOL,            
   ENUM_SOUND_F_CENTER_DB,             
   ENUM_SOUND_F_CENTER_SCALE,          
   ENUM_SOUND_F_LFE_VOL,               
   ENUM_SOUND_F_LFE_DB,                
   ENUM_SOUND_F_LFE_SCALE,             
   ENUM_SOUND_F_WET_VOL,               
   ENUM_SOUND_F_WET_DB,                
   ENUM_SOUND_F_WET_SCALE,             
   ENUM_SOUND_F_DRY_VOL,               
   ENUM_SOUND_F_DRY_DB,                
   ENUM_SOUND_F_DRY_SCALE,             
   ENUM_SOUND_F_LPF_CUTOFF,            
   ENUM_SOUND_L_INITIAL_OFFSET_BYTES,  
   ENUM_SOUND_L_PLAYBACK_OFFSET_BYTES, 
   ENUM_SOUND_F_PLAYBACK_POSITION_MS,  
   ENUM_SOUND_L_TOTAL_LEN_BYTES,       
   ENUM_SOUND_F_DURATION_MS,           
   ENUM_SOUND_N_INITIAL_LOOP_COUNT,    
   ENUM_SOUND_N_REMAINING_LOOP_COUNT,  
   ENUM_SOUND_L_LOOP_START_BYTES,      
   ENUM_SOUND_L_LOOP_END_BYTES,        
   ENUM_SOUND_CB_EOS,                  
   ENUM_SOUND_SINTa_USER,              
   ENUM_SOUND_F_OCCLUSION,             
   ENUM_SOUND_F_OBSTRUCTION,           
   ENUM_SOUND_F_EXCLUSION,             
   ENUM_SOUND_B_IS_3D,                   
   ENUM_SOUND_F_3D_X_POS,              
   ENUM_SOUND_F_3D_X_FACE,             
   ENUM_SOUND_F_3D_X_UP,               
   ENUM_SOUND_F_3D_X_VEL,              
   ENUM_SOUND_F_3D_Y_POS,              
   ENUM_SOUND_F_3D_Y_FACE,             
   ENUM_SOUND_F_3D_Y_UP,               
   ENUM_SOUND_F_3D_Y_VEL,              
   ENUM_SOUND_F_3D_Z_POS,              
   ENUM_SOUND_F_3D_Z_FACE,             
   ENUM_SOUND_F_3D_Z_UP,               
   ENUM_SOUND_F_3D_Z_VEL,              
   ENUM_SOUND_V_3D_POS,                
   ENUM_SOUND_V_3D_FACE,               
   ENUM_SOUND_V_3D_UP,                 
   ENUM_SOUND_V_3D_VEL,                
   ENUM_SOUND_F_3D_MAG,                
   ENUM_SOUND_F_3D_CONE_INNER_DEGS,    
   ENUM_SOUND_F_3D_CONE_INNER_RADS,    
   ENUM_SOUND_F_3D_CONE_OUTER_DEGS,    
   ENUM_SOUND_F_3D_CONE_OUTER_RADS,    
   ENUM_SOUND_F_3D_CONE_OUTER_VOL,     
   ENUM_SOUND_F_3D_CONE_OUTER_DB,      
   ENUM_SOUND_F_3D_CONE_OUTER_SCALE,   
   ENUM_SOUND_F_3D_MIN_DIST,           
   ENUM_SOUND_F_3D_MAX_DIST,           
   ENUM_SOUND_CB_3D_FALLOFF,           
   ENUM_SOUND_B_3D_AUTO_WET_ATTEN,     
   ENUM_SOUND_L_UNDERLYING_HSAMPLE,
};

const MARS_PROPERTY_INFO SND_properties[] =
{ 
 { SOUND_STATUS                  , MP_SOUND_STATUS,   MPF_READ_ONLY                }, 
 { SOUND_SZ_FILENAME             , MP_SZ,             0                            },
 { SOUND_L_FILE_BYTES            , MP_S64,            MPF_READ_ONLY | MPF_UNSAVED  },
 { SOUND_N_PLAYBACK_RATE_HZ      , MP_S32,            0                            },
 { SOUND_N_ORIGINAL_RATE_HZ      , MP_S32,            MPF_READ_ONLY | MPF_UNSAVED  },
 { SOUND_N_SOURCE_CHANNELS       , MP_S32,            MPF_READ_ONLY | MPF_UNSAVED  },
 { SOUND_B_IS_STREAM             , MP_BOOL,           0                            },
 { SOUND_B_IS_AUTO_STREAM        , MP_BOOL,           0                            },
 { SOUND_N_STREAM_BUFFER_BYTES   , MP_S32,            0                            },
 { SOUND_N_PRIORITY              , MP_S32,            0                            },
 { SOUND_F_VOL                   , MP_F32,            MPF_ALTERNATE                },
 { SOUND_F_DB                    , MP_F32,            MPF_ALTERNATE                },
 { SOUND_F_SCALE                 , MP_F32,            0                            },
 { SOUND_F_LR_PAN                , MP_F32,            0                            },
 { SOUND_F_FB_PAN                , MP_F32,            0                            },
 { SOUND_F_CENTER_VOL            , MP_F32,            MPF_ALTERNATE                },
 { SOUND_F_CENTER_DB             , MP_F32,            MPF_ALTERNATE                },
 { SOUND_F_CENTER_SCALE          , MP_F32,            0                            },
 { SOUND_F_LFE_VOL               , MP_F32,            MPF_ALTERNATE                },
 { SOUND_F_LFE_DB                , MP_F32,            MPF_ALTERNATE                },
 { SOUND_F_LFE_SCALE             , MP_F32,            0                            },
 { SOUND_F_WET_VOL               , MP_F32,            MPF_ALTERNATE                },
 { SOUND_F_WET_DB                , MP_F32,            MPF_ALTERNATE                },
 { SOUND_F_WET_SCALE             , MP_F32,            0                            },
 { SOUND_F_DRY_VOL               , MP_F32,            MPF_ALTERNATE                },
 { SOUND_F_DRY_DB                , MP_F32,            MPF_ALTERNATE                },
 { SOUND_F_DRY_SCALE             , MP_F32,            0                            },
 { SOUND_F_LPF_CUTOFF            , MP_F32,            0                            },
 { SOUND_L_INITIAL_OFFSET_BYTES  , MP_S64,            0                            },
 { SOUND_L_PLAYBACK_OFFSET_BYTES , MP_S64,            0                            },
 { SOUND_F_PLAYBACK_POSITION_MS  , MP_F32,            MPF_ALTERNATE                },
 { SOUND_L_TOTAL_LEN_BYTES       , MP_S64,            MPF_READ_ONLY | MPF_UNSAVED  },
 { SOUND_F_DURATION_MS           , MP_F32,            MPF_READ_ONLY | MPF_UNSAVED  },
 { SOUND_N_INITIAL_LOOP_COUNT    , MP_S32,            0                            },
 { SOUND_N_REMAINING_LOOP_COUNT  , MP_S32,            0                            },
 { SOUND_L_LOOP_START_BYTES      , MP_S64,            0                            },
 { SOUND_L_LOOP_END_BYTES        , MP_S64,            0                            },
 { SOUND_CB_EOS                  , MP_SINTa,          MPF_UNEDITABLE | MPF_UNSAVED },
 { SOUND_SINTa_USER              , MP_SINTa,          0                            },
 { SOUND_F_OCCLUSION             , MP_F32,            0                            },
 { SOUND_F_OBSTRUCTION           , MP_F32,            0                            },
 { SOUND_F_EXCLUSION             , MP_F32,            0                            },
 { SOUND_B_IS_3D                 , MP_BOOL,           0                            },
 { SOUND_F_3D_X_POS              , MP_F32,            MPF_ALTERNATE                },
 { SOUND_F_3D_X_FACE             , MP_F32,            MPF_ALTERNATE                },
 { SOUND_F_3D_X_UP               , MP_F32,            MPF_ALTERNATE                },
 { SOUND_F_3D_X_VEL              , MP_F32,            MPF_ALTERNATE                },
 { SOUND_F_3D_Y_POS              , MP_F32,            MPF_ALTERNATE                },
 { SOUND_F_3D_Y_FACE             , MP_F32,            MPF_ALTERNATE                },
 { SOUND_F_3D_Y_UP               , MP_F32,            MPF_ALTERNATE                },
 { SOUND_F_3D_Y_VEL              , MP_F32,            MPF_ALTERNATE                },
 { SOUND_F_3D_Z_POS              , MP_F32,            MPF_ALTERNATE                },
 { SOUND_F_3D_Z_FACE             , MP_F32,            MPF_ALTERNATE                },
 { SOUND_F_3D_Z_UP               , MP_F32,            MPF_ALTERNATE                },
 { SOUND_F_3D_Z_VEL              , MP_F32,            MPF_ALTERNATE                },
 { SOUND_V_3D_POS                , MP_VEC3,           0                            },
 { SOUND_V_3D_FACE               , MP_VEC3,           0                            },
 { SOUND_V_3D_UP                 , MP_VEC3,           0                            },                      
 { SOUND_V_3D_VEL                , MP_VEC3,           0                            },
 { SOUND_F_3D_MAG                , MP_F32,            0                            },
 { SOUND_F_3D_CONE_INNER_DEGS    , MP_F32,            0                            },                       
 { SOUND_F_3D_CONE_INNER_RADS    , MP_F32,            MPF_ALTERNATE                },                       
 { SOUND_F_3D_CONE_OUTER_DEGS    , MP_F32,            0                            },
 { SOUND_F_3D_CONE_OUTER_RADS    , MP_F32,            MPF_ALTERNATE                },
 { SOUND_F_3D_CONE_OUTER_VOL     , MP_F32,            MPF_ALTERNATE                },
 { SOUND_F_3D_CONE_OUTER_DB      , MP_F32,            MPF_ALTERNATE                },
 { SOUND_F_3D_CONE_OUTER_SCALE   , MP_F32,            0                            },
 { SOUND_F_3D_MIN_DIST           , MP_F32,            0                            },
 { SOUND_F_3D_MAX_DIST           , MP_F32,            0                            },
 { SOUND_CB_3D_FALLOFF           , MP_SINTa,          MPF_UNEDITABLE | MPF_UNSAVED },
 { SOUND_B_3D_AUTO_WET_ATTEN     , MP_BOOL,           0                            },
 { SOUND_L_UNDERLYING_HSAMPLE    , MP_SINTa,          MPF_UNEDITABLE | MPF_UNSAVED | MPF_READ_ONLY },
};                            
                                                             
void _________________________________________________Internal_structures____________________________________________________(){}

struct SNDDEF;
struct SNDINST;
struct MENV;
struct PVOICE;

//
// ENVSTATE contains all data needed to bring an HDIGDRIVER up to date with
// an environment
//

struct ENVSTATE      
{
   HMENV E;

   MARS_ROOM_TYPE room_type;
   BOOL           room_type_valid;

   F32 reverb_time_secs;
   F32 reverb_predelay_secs;
   F32 reverb_damping;

   F32 reverb_master_dry_scalar;
   F32 reverb_master_wet_scalar;

   F32 master_scalar;

   MSSVECTOR3D position;
   MSSVECTOR3D face;
   MSSVECTOR3D up;
   MSSVECTOR3D velocity;
   F32         velocity_mag;

   F32 rolloff_factor;
   F32 doppler_factor;
   F32 falloff_power;

   F32 reverb_wet_scalars[MAX_SPEAKERS];
   F32 reverb_dry_scalars[MAX_SPEAKERS];
};

//
// Environment object
//

struct MENV
{
   HDIGDRIVER DIG;
   S32        n_logical_channels;
   S32        global_reverb_supported;

   MARS_OUTPUT_FORMAT output_format;
   S32                n_physical_voices;
   S32                output_sample_rate;
   U32                MEV_flags;

   ENVSTATE input_properties;
   ENVSTATE current_properties;

   SNDDEF  *first_def;
   S32      n_sound_definitions;

   SNDINST *V;
   S32      n_virtual_voices;

   SNDINST *first_free_inst;
};

//
// SNDSTATE contains all data needed to bring an HSAMPLE or HSTREAM up to date
// with a virtual voice
//
// (Any property settings other than these cannot be changed)
//
// Properties that can't vary, can't be edited, or are always the same for all
// instances belong in the SNDDEF instead.  This includes the filename and everything
// related to the sample's format
//

struct SNDSTATE
{
   SNDDEF *def;

   C8 filename[MAX_FILENAME_BYTES];

   MARS_SOUND_CALLBACK   EOS_CB;
   MARS_FALLOFF_CALLBACK falloff_CB;

   S32  playback_rate;
   BOOL auto_stream_service;

   F32  volume_scalar;
   F32  LR_pan;
   F32  FB_pan;
   F32  center_scalar;
   F32  LFE_scalar;

   F32  wet_scalar;
   F32  dry_scalar;

   F32  LPF_cutoff;           // Valid if occlusion < 0

   S32  loop_count;           // Used to change the loop count of existing instance(s)
   S32  initial_loop_count;   // Used to establish loop count when instance(s) are triggered

   S64  loop_start_bytes;     // Valid if >= 0
   S64  loop_end_bytes;       // Valid if >= 0

   F32  occlusion;            // < 0 to enable LPF_cutoff
   F32  obstruction;
   F32  exclusion;

   //
   // Following values are valid only if is_3D is TRUE
   //

   BOOL        is_3D;

   MSSVECTOR3D position;       
   MSSVECTOR3D face;           
   MSSVECTOR3D up;             
   MSSVECTOR3D velocity;       
   F32         velocity_mag;
               
   F32         inner_angle;    
   F32         outer_angle;    
   F32         outer_scalar;
               
   F32         max_dist;       
   F32         min_dist;

   S32         auto_3D_atten;  

   //
   // Following values are valid only if is_3D is FALSE
   //                              

   F32  logical_channel_levels[MAX_SPEAKERS];

   //
   // Following values are meaningful only for playing instances, although they need to
   // be present here so they can be used to initialize HSTREAMs/HSAMPLEs.  
   //
   // When the HMSOUND is queried, the value returned is actually the 
   // sample position from the most-recently-triggered instance of the HMSOUND
   //

   S64  initial_play_cursor_bytes;  // Always valid, defaults to 0

   S64  play_cursor_bytes;    // Valid if >= 0
   F32  play_cursor_ms;       // Valid if >= 0
};

//
// Sound definition
//

struct SNDDEF
{
   MENV   *E;
   SNDDEF *next_def;
   SNDDEF *prev_def;

   SNDSTATE input_properties;

   S32   priority;
   SINTa user;

   //
   // Following values are used (or obtained) at instance-construction 
   // or file-preload time
   //
   // TODO: Move file images into another layer and refcount them, so they 
   // can be shared by multiple sounds.  (After doing this, do separate 
   // SNDDEF/SNDINST structures still make sense?)
   //

   U32  *file_memory_ptr;     // Length word (limited to 4 GB) followed by rest of file
   void *file_data;
   U32   file_bytes;

   BOOL is_streamed;
   S32  stream_buffer_bytes;

   S32  n_source_channels;    // Mono or stereo
   S32  original_sample_rate; 
   F32  duration_ms;
   S64  duration_bytes;

   //
   // Valid for playing SNDDEFs
   //

   SNDINST *first_inst;
};

//
// Voice instance
//

struct SNDINST
{
   SNDDEF  *D;
   SNDINST *next_inst;
   SNDINST *prev_inst;

   BOOL triggered;
   BOOL has_voice;

   U32  S_status;
   U32  T_status;
   U32  S_play_cursor_bytes;

   SNDSTATE current_properties;

   union
      {
      HSAMPLE S;
      HSTREAM STR;
      };
};

void _________________________________________________Utility_functions______________________________________________________(){}

F32 loudness_to_scalar(F32 loudness) // turns loudness to linear DAC scalar (0.5 = 0.314)
{
   return (F32) AIL_pow(loudness, 10.0F / 6.0F ); 
}

F32 scalar_to_loudness(F32 scalar)  // turns linear DAC scalar into loudness (0.314 = 0.5)
{
   return (F32) AIL_pow(scalar, 6.0F / 10.0F);
}

F32 dB_to_scalar(F32 dB)      // turns dBfs into linear DAC scalar (-6 dB = 0.5F, -12 dB = 0.25F...)
{
   return (F32) AIL_pow(10.0F, dB / 20.0F);
}

F32 scalar_to_dB(F32 scalar)  // turns linear DAC scalar into dB (0.5 = -6 dB, 0.25 = -12 dB, ...)
{
   if (scalar < 0.00001F)
      {
      return -96.0F;
      }

   return 20.0F * AIL_log10(scalar);
}

F32 dB_to_loudness(F32 dB)    // -10 dB = 0.5, -20 dB = 0.25, -30 dB = 0.125 ...
{
   return (F32) AIL_pow(10.0F, dB * 0.03F);
}

F32 loudness_to_dB(F32 loudness) // 0.5 = -10 dB, 0.25 = -20 dB, ...
{
   if (loudness < 0.00001F)
      {
      return -96.0F;
      }

   return (F32) AIL_log10(loudness) / 0.03F;
}

inline BOOL F32_diff(F32 a, F32 b, F32 epsilon=0.0001F)
{
   if (a == b) return FALSE;

   if (a > b) 
      return (a-b) > epsilon;
   else
      return (b-a) > epsilon;
}

inline BOOL VEC3_diff(MSSVECTOR3D &a, MSSVECTOR3D &b, F32 epsilon=0.0001F)
{
   F32 d = (a.x > b.x) ? a.x - b.x : b.x - a.x;
   if (d > epsilon) return TRUE;

   d = (a.y > b.y) ? a.y - b.y : b.y - a.y;
   if (d > epsilon) return TRUE;

   d = (a.z > b.z) ? a.z - b.z : b.z - a.z;
   return (d > epsilon);
}

//***************************************************************************/
//
// set_ENVSTATE_defaults()
//
// Initialize a ENVSTATE to values that reflect the default state of an 
// HDIGDRIVER
//
//***************************************************************************/

void set_ENVSTATE_defaults(HMENV     E, //)
                           ENVSTATE *D)
{
   AIL_memset(D,
              0,
              sizeof(*D));

   D->E = E; 

   D->room_type = MARS_ROOM_GENERIC;

   D->reverb_master_dry_scalar = 1.0F;
   D->reverb_master_wet_scalar = 1.0F;
   D->master_scalar            = 1.0F;

   D->face.x = 0.0F;
   D->face.y = 0.0F;
   D->face.z = 1.0F;

   D->up.x = 0.0F;
   D->up.y = 1.0F;
   D->up.z = 0.0F;

   D->velocity_mag = 1.0F;

   D->rolloff_factor = 1.0F;
   D->doppler_factor = 1.0F;
   D->falloff_power  = 3.0F;

   for (S32 i=0; i < MAX_SPEAKERS; i++)
      {
      D->reverb_wet_scalars[i] = 1.0F;
      D->reverb_dry_scalars[i] = 1.0F;
      }
}

//***************************************************************************/
//
// set_SNDSTATE_defaults()
//
// Initialize a SNDSTATE to valid default values in the absence of a source
// sample file
//
//***************************************************************************/

void set_SNDSTATE_defaults(SNDDEF   *D, //)
                           SNDSTATE *S)
{
   AIL_memset(S,
              0,
              sizeof(*S));

   S->def = D; 

   S->playback_rate = -1;
   S->volume_scalar = 1.0F;
   S->LR_pan        = 0.5F;
   S->FB_pan        = 0.5F;
   S->center_scalar = 1.0F;
   S->LFE_scalar    = 1.0F;

   S->wet_scalar    = 0.0F;
   S->dry_scalar    = 1.0F;

   S->LPF_cutoff    = 1.0F;

   S->loop_start_bytes = -1;
   S->loop_end_bytes   = -1;

   S->loop_count         = 1;
   S->initial_loop_count = 1;

   S->play_cursor_bytes = -1;
   S->play_cursor_ms    = -1;

   S->occlusion   = -1.0F;
   S->obstruction = 0.0F;
   S->exclusion   = 0.0F;

   S->inner_angle  = 360.0F;
   S->outer_angle  = 360.0F;
   S->outer_scalar = 1.0F;

   S->max_dist = 200.0F;
   S->min_dist = 1.0F;

   S->face.x = 1.0F;
   S->up.y   = 1.0F;

   S->velocity_mag = 1.0F;

   S->auto_3D_atten = TRUE;

   for (S32 i=0; i < MAX_SPEAKERS; i++)
      {
      S->logical_channel_levels[i] = 1.0F;
      }
}

//***************************************************************************/
//
// update_HDIGDRIVER()
//
// Bring an existing HDIGDRIVER up to date with any differences between the
// source and reference ENVSTATEs.  The reference state is updated as well.
// If the reference is NULL, then all HDIGDRIVER parameters are updated
//
//***************************************************************************/

void update_HDIGDRIVER(HDIGDRIVER    dest, //)
                       ENVSTATE     *src,
                       ENVSTATE     *ref)
{
   //
   // TODO: clamp
   // 

   //
   // Global reverb params/room type
   //
   
   if (src->room_type_valid)
      {
      if ((ref == NULL) || (src->room_type != ref->room_type))
         {
         if (ref != NULL)
            {
            ref->room_type = src->room_type;
            }

         AIL_set_room_type(dest, 
                           src->room_type);
         }
      }
   else
      {
      if ((ref == NULL) || F32_diff(src->reverb_time_secs,     ref->reverb_time_secs)
                        || F32_diff(src->reverb_predelay_secs, ref->reverb_predelay_secs)
                        || F32_diff(src->reverb_damping,       ref->reverb_damping))
         {
         if (ref != NULL) 
            {
            ref->reverb_time_secs     = src->reverb_time_secs;
            ref->reverb_predelay_secs = src->reverb_predelay_secs;
            ref->reverb_damping       = src->reverb_damping;
            }

         AIL_set_digital_master_reverb(dest,
                                       src->reverb_time_secs,
                                       src->reverb_predelay_secs,
                                       src->reverb_damping);
         }
      }

   if (ref != NULL)
      {
      ref->room_type_valid = src->room_type_valid;
      }

   //
   // Master wet/dry
   //

   if ((ref == NULL) || F32_diff(src->reverb_master_dry_scalar, ref->reverb_master_dry_scalar) 
                     || F32_diff(src->reverb_master_wet_scalar, ref->reverb_master_wet_scalar))
      {
      if (ref != NULL) 
         {
         ref->reverb_master_dry_scalar = src->reverb_master_dry_scalar;
         ref->reverb_master_wet_scalar = src->reverb_master_wet_scalar;
         }

      AIL_set_digital_master_reverb_levels(dest,
                                           src->reverb_master_dry_scalar,
                                           src->reverb_master_wet_scalar);
      }

   //
   // Master volume
   // 

   if ((ref == NULL) || F32_diff(src->master_scalar, ref->master_scalar))
      {
      if (ref != NULL) 
         {
         ref->master_scalar = src->master_scalar;
         }

      AIL_set_digital_master_volume_level(dest,
                                          src->master_scalar);
      }

   //
   // Rolloff factor
   // 

   if ((ref == NULL) || F32_diff(src->rolloff_factor, ref->rolloff_factor))
      {
      if (ref != NULL) 
         {
         ref->rolloff_factor = src->rolloff_factor;
         }

      AIL_set_3D_rolloff_factor(dest,
                                src->rolloff_factor);
      }

   //
   // Doppler factor
   // 

   if ((ref == NULL) || F32_diff(src->doppler_factor, ref->doppler_factor))
      {
      if (ref != NULL) 
         {
         ref->doppler_factor = src->doppler_factor;
         }

      AIL_set_3D_doppler_factor(dest,
                                src->doppler_factor);
      }

   //
   // Falloff power
   //

   if ((ref == NULL) || F32_diff(src->falloff_power, ref->falloff_power))
      {
      if (ref != NULL) 
         {
         ref->falloff_power = src->falloff_power;
         }

      AIL_set_speaker_configuration(dest,
                                    NULL,
                                    0,
                                    src->falloff_power);
      }

   //
   // Per-speaker wet/dry levels
   //
   // TODO: Select correct MAX_SPEAKERS[] array entries for the speaker indexes
   // valid for the current output format -- it may have changed!
   //

   BOOL dirty = FALSE;

   if (ref == NULL)
      {
      dirty = TRUE;
      }
   else if (!dirty)
      {
      for (S32 i=0; i < src->E->n_logical_channels; i++)
         {
         if (F32_diff(src->reverb_wet_scalars[i], ref->reverb_wet_scalars[i]) ||
             F32_diff(src->reverb_dry_scalars[i], ref->reverb_dry_scalars[i]))
            {
            dirty = TRUE;
            break;
            }
         }
      }

   if (dirty)
      {
      if (ref != NULL) 
         {
         memcpy(ref->reverb_wet_scalars,
                src->reverb_wet_scalars,
                sizeof(src->reverb_wet_scalars[0]) * src->E->n_logical_channels);

         memcpy(ref->reverb_dry_scalars,
                src->reverb_dry_scalars,
                sizeof(src->reverb_dry_scalars[0]) * src->E->n_logical_channels);
         }

      AIL_set_speaker_reverb_levels(dest,
                                    src->reverb_wet_scalars,
                                    src->reverb_dry_scalars,
                                    src->E->n_logical_channels);
      }

   //
   // 3D position
   //

   if ((ref == NULL) || VEC3_diff(src->position, ref->position))
      {
      if (ref != NULL) ref->position = src->position;

      AIL_set_listener_3D_position(dest,
                                   src->position.x,
                                   src->position.y,
                                   src->position.z);
      }

   //
   // Face/up vectors
   // 

   if ((ref == NULL) || VEC3_diff(src->face, ref->face)
                     || VEC3_diff(src->up,   ref->up))
      {
      if (ref != NULL) 
         {
         ref->face = src->face;
         ref->up   = src->up;
         }

      AIL_set_listener_3D_orientation(dest,
                                      src->face.x,
                                      src->face.y,
                                      src->face.z,
                                      src->up.x,
                                      src->up.y,
                                      src->up.z);
      }

   //
   // Velocity
   //

   if ((ref == NULL) || VEC3_diff(src->velocity,     ref->velocity) 
                     || F32_diff (src->velocity_mag, ref->velocity_mag))
      {
      if (ref != NULL) 
         {
         ref->velocity     = src->velocity;
         ref->velocity_mag = src->velocity_mag;
         }

      AIL_set_listener_3D_velocity(dest,
                                   src->velocity.x,
                                   src->velocity.y,
                                   src->velocity.z,
                                   src->velocity_mag);
      }
}

//***************************************************************************/
//
// update_HSAMPLE() 
//
// Bring an existing HSAMPLE up to date with any differences between the source
// and reference SNDSTATEs.  The reference state is also updated.
//
// If the reference is NULL, then all HSAMPLE parameters are updated
//
//***************************************************************************/

void update_HSAMPLE(HSAMPLE    dest, //)
                    SNDSTATE  *src,
                    SNDSTATE  *ref)
{
   //
   // TODO: support streams
   // TODO: clamp
   //

   if (!src->is_3D)     // TODO: if this changes, must reinit sample!
      {
      //
      // Explicit (non-3D controlled) logical channel levels
      //
      // TODO: the format and its speaker indexes may have changed -- add a level 
      // of indirection to make the [MAX_SPEAKERS] arrays format-independent
      //

      BOOL dirty = FALSE;

      if (ref == NULL)
         {
         dirty = TRUE;
         }
      else
         {
         for (S32 i=0; i < src->def->E->n_logical_channels; i++)
            {
            if (F32_diff(src->logical_channel_levels[i], ref->logical_channel_levels[i]))
               {
               dirty = TRUE;
               break;
               }
            }
         }

      if (dirty)
         {
         if (ref != NULL)
            {
            memcpy(ref->logical_channel_levels,
                   src->logical_channel_levels,
                   sizeof(src->logical_channel_levels[0]) * src->def->E->n_logical_channels);
            }

         AIL_set_sample_channel_levels(dest, 
                                       src->logical_channel_levels,
                                       src->def->E->n_logical_channels);
         }
      }
   else
      {
      //
      // 3D position
      //

      if ((ref == NULL) || VEC3_diff(src->position, ref->position))
         {
         if (ref != NULL) ref->position = src->position;

         AIL_set_sample_3D_position(dest,
                                    src->position.x,
                                    src->position.y,
                                    src->position.z);
         }

      //
      // 3D cone
      //

      if ((ref == NULL) ||
           F32_diff(src->inner_angle,  ref->inner_angle) ||
           F32_diff(src->outer_angle,  ref->outer_angle) ||
           F32_diff(src->outer_scalar, ref->outer_scalar))
         {                
         if (ref != NULL)
            {
            ref->inner_angle  = src->inner_angle;
            ref->outer_angle  = src->outer_angle;
            ref->outer_scalar = src->outer_scalar;
            }

         AIL_set_sample_3D_cone(dest, 
                                src->inner_angle, 
                                src->outer_angle, 
                                src->outer_scalar);
         }

      //
      // Distances/auto-atten
      //

      if ((ref == NULL) ||
           F32_diff(src->max_dist,        ref->max_dist) ||
           F32_diff(src->min_dist,        ref->min_dist) ||
                   (src->auto_3D_atten != ref->auto_3D_atten))
         {                
         if (ref != NULL)
            {
            ref->max_dist      = src->max_dist;
            ref->min_dist      = src->min_dist;
            ref->auto_3D_atten = src->auto_3D_atten;
            }

         AIL_set_sample_3D_distances(dest, 
                                     src->max_dist, 
                                     src->min_dist, 
                                     src->auto_3D_atten);
         }

      //
      // Face/up vectors
      // 

      if ((ref == NULL) || VEC3_diff(src->face, ref->face)
                        || VEC3_diff(src->up,   ref->up))
         {
         if (ref != NULL)
            {
            ref->face = src->face;
            ref->up   = src->up;
            }

         AIL_set_sample_3D_orientation(dest,
                                       src->face.x,
                                       src->face.y,
                                       src->face.z,
                                       src->up.x,
                                       src->up.y,
                                       src->up.z);
         }

      //
      // Velocity
      //

      if ((ref == NULL) || VEC3_diff(src->velocity,     ref->velocity) 
                        || F32_diff (src->velocity_mag, ref->velocity_mag))
         {
         if (ref != NULL)
            {
            ref->velocity     = src->velocity;
            ref->velocity_mag = src->velocity_mag;
            }

         AIL_set_sample_3D_velocity(dest,
                                    src->velocity.x,
                                    src->velocity.y,
                                    src->velocity.z,
                                    src->velocity_mag);
         }
      }                                 

   if (ref != NULL)
      {
      ref->def   = src->def;
      ref->is_3D = src->is_3D;
      }

   //
   // Per-sample reverb levels
   //

   if ((ref == NULL) || F32_diff(src->dry_scalar, ref->dry_scalar) 
                     || F32_diff(src->wet_scalar, ref->wet_scalar))
      {
      if (ref != NULL)
         {
         ref->dry_scalar = src->dry_scalar;
         ref->wet_scalar = src->wet_scalar;
         }

      AIL_set_sample_reverb_levels(dest,
                                   src->dry_scalar,
                                   src->wet_scalar);
      }

   //
   // Occlusion is not updated if < 0, because
   // it interferes with LPF
   // 

   if ((ref == NULL) || F32_diff(src->exclusion, ref->exclusion))
      {
      if (ref != NULL)
         {
         ref->exclusion = src->exclusion;
         }

      AIL_set_sample_exclusion(dest, src->exclusion);
      }

   if ((ref == NULL) || F32_diff(src->obstruction, ref->obstruction))
      {
      if (ref != NULL)
         {
         ref->obstruction = src->obstruction;
         }

      AIL_set_sample_obstruction(dest, src->obstruction);
      }

   if ((ref == NULL) || F32_diff(src->occlusion, ref->occlusion))
      {
      if (ref != NULL)
         {
         ref->occlusion = src->occlusion;
         }

      if (src->occlusion >= 0.0F)
         {
         AIL_set_sample_occlusion(dest, src->occlusion);
         }
      }

   //
   // Playback rate
   //

   if ((ref == NULL) || (src->playback_rate != ref->playback_rate))
      {
      if (ref != NULL)
         {
         ref->playback_rate = src->playback_rate;
         }

      AIL_set_sample_playback_rate(dest, src->playback_rate);
      }

   //
   // LPF
   //

   if ((ref == NULL) || F32_diff(src->LPF_cutoff, ref->LPF_cutoff))
      {
      if (ref != NULL)
         {
         ref->LPF_cutoff = src->LPF_cutoff;
         }

      if (src->occlusion < 0.0F)
         {
         AIL_set_sample_low_pass_cut_off(dest, src->LPF_cutoff);
         }
      }

   //
   // Set either mono/stereo or 5.1-specific source volume levels
   //

   if ((ref == NULL) || F32_diff(src->volume_scalar, ref->volume_scalar)
                     || F32_diff(src->LR_pan,        ref->LR_pan)
                     || F32_diff(src->FB_pan,        ref->FB_pan)
                     || F32_diff(src->center_scalar, ref->center_scalar)
                     || F32_diff(src->LFE_scalar,    ref->LFE_scalar))
      {
      if (ref != NULL) // TODO: this should probably support discrete levels
         {
         ref->volume_scalar = src->volume_scalar;
         ref->LR_pan        = src->LR_pan;
         ref->FB_pan        = src->FB_pan;
         ref->center_scalar = src->center_scalar;
         ref->LFE_scalar    = src->LFE_scalar;
         }

#ifdef IS_XBOX
      if (1)
#else
      if ((src->def->E->DIG->channel_spec == MSS_MC_51_DISCRETE) ||     // TODO, new enums
          (src->def->E->DIG->channel_spec == MSS_MC_51_DTS))
#endif   
         {

         AIL_set_sample_51_volume_pan(dest,
                                      src->volume_scalar,
                                      src->LR_pan,
                                      src->FB_pan,
                                      src->center_scalar,
                                      src->LFE_scalar);
         }
      else
         {
         AIL_set_sample_volume_pan(dest,
                                   src->volume_scalar,
                                   src->LR_pan);
         }
      }

   //
   // Loop block
   //

   if ((ref == NULL) || (src->loop_start_bytes != ref->loop_start_bytes) 
                     || (src->loop_end_bytes   != ref->loop_end_bytes))
      {
      if (ref != NULL)
         {
         ref->loop_start_bytes = src->loop_start_bytes;
         ref->loop_end_bytes   = src->loop_end_bytes;
         }

      AIL_set_sample_loop_block(dest,
                          (U32) src->loop_start_bytes,
                          (U32) src->loop_end_bytes);
      }

   //
   // Loop count
   //

   if ((ref == NULL) || (src->loop_count != ref->loop_count))
      {
      if (ref != NULL)
         {
         ref->loop_count = src->loop_count;
         }

      AIL_set_sample_loop_count(dest,
                                src->loop_count);
      }

   //
   // Play cursor (both bytes and msec)
   //

   if ((ref == NULL) || (src->play_cursor_bytes != ref->play_cursor_bytes))
      {
      if (ref != NULL)
         {
         ref->play_cursor_bytes = src->play_cursor_bytes;
         }

      if (src->play_cursor_bytes >= 0)
         {
         AIL_set_sample_position(dest,
                           (U32) src->play_cursor_bytes);
         }
      }

   if ((ref == NULL) || F32_diff(src->play_cursor_ms, ref->play_cursor_ms))
      {
      if (ref != NULL)
         {
         ref->play_cursor_ms = src->play_cursor_ms;
         }

      if (src->play_cursor_ms >= 0.0F)
         {
         AIL_set_sample_ms_position(dest, 
                              (S32) src->play_cursor_ms);
         }
      }

   //
   // Callbacks (TODO: AIL->MARS proxies)
   //

   if ((ref == NULL) || (src->falloff_CB != ref->falloff_CB))
      {
      if (ref != NULL)
         {
         ref->falloff_CB = src->falloff_CB;
         }

      AIL_register_falloff_function_callback(dest,
                                             NULL);
      }

   if ((ref == NULL) || (src->EOS_CB != ref->EOS_CB))
      {
      if (ref != NULL)
         {
         ref->EOS_CB = src->EOS_CB;
         }

      AIL_register_EOS_callback(dest,
                                NULL);
      }

#if 0

   //
   // Install any needed pipeline filters and restore their properties
   //
   // Existing filters are NOT replaced by this function.  Output
   // filter properties are updated, but we assume the filter itself was
   // installed when the HSAMPLE was initialized
   //

   for (S32 n=0; n < dest->n_filter_properties; n++)
      {
      SAMPLESTAGE stage    = dest->filter_stages[n];
      HPROVIDER   provider = dest->filter_providers[n];

      HPROVIDER cur = AIL_sample_processor(src, stage); 

      if ((cur != provider) && (stage != SP_OUTPUT))
         {
         if (cur != 0)
            {
            continue;         // A different provider was already installed in this slot
            }

         AIL_set_sample_processor(src,
                                  stage,
                                  provider);
         }

      RIB_INTERFACE_ENTRY entry;                   // Find property whose token matches the one
      HMARSENUM            next = HPROENUM_FIRST;  // we logged...
      S32                 found = FALSE;

      while (AIL_enumerate_filter_sample_properties(provider,
                                                   &next,
                                                   &entry))
         {
         if (entry.token == dest->filter_property_tokens[n])
            {
            found = TRUE;
            break;
            }
         }

      if (!found) continue;     

      AIL_sample_stage_property(src,               
                                stage,           
                                entry.entry_name,
                                NULL,
                               &dest->filter_property_values[n],
                                NULL);
      }
                               
   //
   // Return previous playback state, so the application can restore it if/when desired
   //

#endif
}

//***************************************************************************/
//
// Allocate a SNDINST structure and link it into the beginning of the SNDDEF's
// instance list.  Returns NULL if no SNDINSTs are in the environment's free list
//
//***************************************************************************/

SNDINST *allocate_SNDINST(SNDDEF *D)
{
   //
   // Unlink first free SNDINST from environment's list
   //

   HMENV E = D->E;

   SNDINST *I = E->first_free_inst;

   if (I == NULL)
      {
      return NULL;
      }

   E->first_free_inst = I->next_inst;

   if (I->next_inst != NULL)
      {
      I->next_inst->prev_inst = NULL;
      }

   //
   // Link it into the SNDDEF's active-instance list
   //

   I->next_inst = D->first_inst;
   assert(I->prev_inst == NULL);  

   if (D->first_inst != NULL)
      {
      D->first_inst->prev_inst = I;
      }

   D->first_inst = I;

   I->D = D;
   I->S = NULL;

   I->triggered = FALSE;
   I->has_voice = FALSE;
   I->S_status  = SMP_DONE;

   return I;
}

//***************************************************************************/
//
// Return a SNDINST to the environment's free list
//
//***************************************************************************/

void free_SNDINST(SNDINST *I)
{
   //
   // Remove from SNDDEF's list
   //
   
   SNDDEF *D = I->D;

   I->D = NULL;

   if (I == D->first_inst)
      {
      D->first_inst = I->next_inst;
      assert(I->prev_inst == NULL);
      }
   else
      {
      I->prev_inst->next_inst = I->next_inst;
      }
      
   if (I->next_inst != NULL)
      {
      I->next_inst->prev_inst = I->prev_inst;
      }

   //
   // Add to beginning of environment's free list
   //

   HMENV E = D->E;

   I->next_inst = E->first_free_inst;
   I->prev_inst = NULL;
   
   E->first_free_inst = I;   
}

//***************************************************************************/
//
// Release physical voice associated with a sound instance
//
//***************************************************************************/

void release_voice(SNDINST *I, 
                   BOOL     release_HSAMPLE = TRUE)      // (normally TRUE unless you plan to re-use the HSAMPLE)
{
   if (I->has_voice)
      {
      AIL_end_sample(I->S);

      if (release_HSAMPLE)
         {
         AIL_release_sample_handle(I->S);
         I->S = NULL;
         }

      I->has_voice = FALSE;
      }
}

//***************************************************************************/
//
// Try to assign an HSAMPLE or HSTREAM to the specified sound instance, 
// stealing one from a lower-priority sound instance if necessary
//
// The sample's source file must be valid
//
//***************************************************************************/

MARS_RESULT assign_voice(SNDINST *I)
{
   SNDDEF *D = I->D;

   assert(D->input_properties.filename[0]);

   I->S = AIL_allocate_sample_handle(D->E->DIG);  

   if (I->S == NULL)
      {
      //
      // TODO: No voices available -- steal one 
      //
      //   First try to find a SMP_DONE sample
      //
      //   Failing that, steal the voice from an instance of the 
      //   lowest-priority sound, preferring those that are already
      //   in their release phases, or that are beyond the max 3D distance
      //   from the listener
      //

      BOOL voice_donor_found = FALSE;

      SNDDEF *C = D->E->first_def;

      while ((!voice_donor_found) && (C != NULL))
         {
         SNDINST *J = C->first_inst;

         while (J != NULL)
            {
            if (J->has_voice)
               {
               if (AIL_sample_status(J->S) == SMP_DONE)
                  {
                  release_voice(J, FALSE);

                  I->S = J->S;
                  J->S = NULL;

                  voice_donor_found = TRUE;
                  break;
                  }
               }

            J = J->next_inst;
            }

         C = C->next_def;
         }
      }

   if (I->S == NULL)
      {
      I->has_voice = FALSE;
      return MARS_GENERAL_ERROR;
      }

   //
   // Mark instance as untriggered (so it will be started by the next MARS_play() call on this
   // definition)
   //

   I->has_voice = TRUE;
   I->triggered = FALSE;

   //
   // Associate the definition's file with the HSAMPLE
   //

   S32 result = AIL_set_named_sample_file(I->S,
                                          D->input_properties.filename,
                                          D->file_data,
                                          D->file_bytes,
                                          0);

   if (!result)
      {
      return MARS_UNSUPPORTED_FORMAT;
      }

   //
   // If this is the first instance of the sound, update the definition with some 
   // static info about the source HSAMPLE
   //

   if (I->prev_inst == NULL)
      {
      S32 ms = 0;

      AIL_sample_ms_position(I->S,
                            &ms,
                             NULL);

      D->n_source_channels    = (I->S->format & DIG_F_STEREO_MASK) ? 2 : 1;
      D->original_sample_rate = AIL_sample_playback_rate(I->S);
      D->duration_ms          = (F32) ms;
      D->duration_bytes       = I->S->len[0];
      }

   //
   // If the playback-rate property hasn't been overridden by a project-file 
   // setting or API call, initialize it to the data file's property
   //

   if (D->input_properties.playback_rate == -1)
      {
      D->input_properties.playback_rate = D->original_sample_rate;
      }

   //
   // Force initialization of all HSAMPLE properties without comparing them
   // to the current-property set, then copy the input property set to the
   // current set
   //
   // We must do this to force current_properties to match the HSAMPLE state... otherwise,
   // future incremental updates won't work.  TODO: This means that we need to reject any
   // attempt to defer property updates across a MARS_preload() call... do this by failing
   // with a NOT_DEFERRABLE error?
   //

   update_HSAMPLE(I->S, &D->input_properties, NULL);

   I->current_properties = D->input_properties;

   return MARS_OK;
}

//***************************************************************************/
//
// Return address of a property member in an ENVSTATE instance, or NULL
// if the specified property is not part of ENVSTATE
//
//***************************************************************************/

S32 ENVSTATE_property(MARS_PROPERTY        name,
                      MARS_PROPERTY_INFO **info)
{
   //
   // TODO: try address hash first, then string hash
   //

   S32 index;
   S32 n = ARY_CNT(ENV_properties);

   for (index=0; index < n; index++)
      {
      if (!_stricmp(name, ENV_properties[index].name))
         {
         break;
         }
      }

   if (index == n)
      {
      return -1;
      }

   if (info != NULL)
      {
      *info = (MARS_PROPERTY_INFO *) &ENV_properties[index];
      }

   return index;
}

//***************************************************************************/
//
// Return address of a property member in a SNDSTATE instance, or NULL
// if the specified property is not part of SNDSTATE
//
//***************************************************************************/

S32 SNDSTATE_property(MARS_PROPERTY        name,
                      MARS_PROPERTY_INFO **info)
{
   //
   // TODO: try address hash first, then string hash
   //

   S32 index;
   S32 n = ARY_CNT(SND_properties);

   for (index=0; index < n; index++)
      {
      if (!_stricmp(name, SND_properties[index].name))
         {
         break;
         }
      }     

   if (index == n)
      {
      return -1;
      }

   if (info != NULL)
      {
      *info = (MARS_PROPERTY_INFO *) &SND_properties[index];
      }

   return index;
}

//***************************************************************************/
//
// Update environment properties based on API input, if not deferred
//
//***************************************************************************/

void ENV_update_properties(HMENV E)
{
   if (E->DIG == NULL)
      {
      return;
      }

   //
   // TODO: If deferred, return without updating driver
   //

   update_HDIGDRIVER(E->DIG,
                    &E->input_properties,
                    &E->current_properties);
}

//***************************************************************************/
//
// Update sound-definition properties based on API input, if not deferred
//
//***************************************************************************/

void SND_update_properties(HMSOUND D)
{
   //
   // TODO: If deferred, return without updating HSAMPLEs
   //

   //
   // Update underlying filenames
   //
   // This causes instances to lose their existing HSAMPLEs, so we'll 
   // do it before anything else happens
   //

   SNDINST *I = D->first_inst;

   while (I != NULL)
      {
      if (strcmp(D->input_properties.filename, I->current_properties.filename))
         {
         MARS_discard(D);        // TODO: Dec refcount on file in MARS_discard()
         break;                  // TODO: Loop needs to survive MARS_discard()
         }

      I = I->next_inst;
      }

   //
   // Apply any property changes to all HSAMPLEs associated with this
   // sound definition
   //

   I = D->first_inst;

   while (I != NULL)
      {
      if (I->has_voice)
         {
         update_HSAMPLE(I->S,
                       &D->input_properties,
                       &I->current_properties);
         }

      I = I->next_inst;
      }
}

//***************************************************************************/
//
// Store textual representation of a property value
//
//***************************************************************************/

S32 property_to_text(MARS_PROPERTY_TYPE type, 
                     void              *value,
                     C8                *dest)
{
   switch (type)
      {
      case MP_SOUND_STATUS:
         {
         sprintf(dest,"0x%X",*(U32 *) value);
         break;
         }

      case MP_S32:
      case MP_OUTPUT_FORMAT:
      case MP_ROOM_TYPE:
      case MP_BOOL:
         {
         sprintf(dest,"%d",*(S32 *) value);
         break;
         }

      case MP_SINTa:
         {
         sprintf(dest,SINTFMT,*(SINTa *) value);    
         break;
         }

      case MP_S64:
         {
         sprintf(dest,I64FMT,*(S64 *) value);
         break;
         }

      case MP_F32:
         {
         sprintf(dest,"%f",*(F32 *) value);
         break;
         }

      case MP_VEC3:
         {
         MSSVECTOR3D *V = (MSSVECTOR3D *) value;

         sprintf(dest,"%f,%f,%f",V->x,V->y,V->z);
         break;
         }

      case MP_SZ:
         {
         sprintf(dest,"%s",(C8 *) value);
         break;
         }

      default: return FALSE;
      }   

   return TRUE;
}

//***************************************************************************/
//
// Store binary-value representation of a text property 
//
//***************************************************************************/

S32 text_to_property(MARS_PROPERTY_TYPE type, 
                     C8               **value,
                     void              *dest)
{
   S32 len = 0;

   switch (type)
      {
      case MP_OUTPUT_FORMAT:
         {
         sscanf(*value,"0x%X",dest, &len);
         break;
         }

      case MP_S32:
      case MP_ROOM_TYPE:
      case MP_SOUND_STATUS:
      case MP_BOOL:
         {
         sscanf(*value,"%d%n",dest,&len);
         *value += len;
         break;
         }

      case MP_SINTa:
         {
         sscanf(*value,SINTFMT"%n",dest,&len);
         *value += len;
         break;
         }

      case MP_S64:
         {
         sscanf(*value,I64FMT"%n",dest,&len);
         break;
         }

      case MP_F32:
         {
         sscanf(*value,"%f%n",dest,&len);
         break;
         }

      case MP_VEC3:
         {
         MSSVECTOR3D *V = (MSSVECTOR3D *) dest;

         sscanf(*value,"%f,%f,%f%n",&V->x, &V->y, &V->z, &len);
         break;
         }

      case MP_SZ:
         {
         len = strlen(*value);
         strcpy((C8 *) dest,*value);
         break;
         }

      default: return FALSE;
      }   

   *value += len;
   return TRUE;
}

//***************************************************************************/
//
// Return a sound definition with matching tags, beginning search at the
// specified definition
//
//***************************************************************************/

SNDDEF *find_qualified_SNDDEF(HMENV            E, 
                              SNDDEF          *current,
                              C8              *tagset)
{
   if (current == NULL)
      {
      current = E->first_def;
      }

   while (current != NULL)
      {
      /* if qualified */ 
         {
         return current;
         }

      current = current->next_def;
      }

   return NULL;
}

void _________________________________________________Global_API_____________________________________________________________(){}

MARS_RESULT MARS_startup(void)
{
   AIL_startup();
   return MARS_OK;
}

MARS_RESULT MARS_shutdown(void)
{
   // TODO: free environment list

   AIL_shutdown();
   return MARS_OK;
}

void _________________________________________________Environment_objects____________________________________________________(){}

MARS_RESULT MARS_create_environment        (HMENV *environment,
                                            S32    n_virtual_voices)
                                            
{
   if (environment == NULL)
      {
      return MARS_BAD_PARAM;
      }

   HMENV E = (HMENV) AIL_mem_alloc_lock(sizeof(*E));

   if (E == NULL)
      {
      return MARS_OUT_OF_MEMORY;
      }

   memset(E, 0, sizeof(*E));

   set_ENVSTATE_defaults(E,
                        &E->input_properties);

   //
   // Allocate "virtual" voice instances and chain them together in free list
   // 
   
   E->n_virtual_voices = n_virtual_voices;

   E->V = (SNDINST *) AIL_mem_alloc_lock(sizeof(*E->V) * n_virtual_voices);

   if (E->V == NULL)
      {
      MARS_destroy_environment(E);
      return MARS_OUT_OF_MEMORY;
      }

   memset(E->V, 0, sizeof(*E->V) * n_virtual_voices);

   E->first_free_inst = &E->V[0];

   E->V[0].next_inst = &E->V[1];
   E->V[0].prev_inst = NULL;

   S32 i;
   for (i=1; i < n_virtual_voices-1; i++)
      {
      E->V[i].next_inst = &E->V[i+1];
      E->V[i].prev_inst = &E->V[i-1];
      }

   E->V[i].next_inst = NULL;
   E->V[i].prev_inst = &E->V[i-1];

   *environment = E;
   return MARS_OK;
}

MARS_RESULT MARS_set_output_format         (HMENV              E,
                                            MARS_OUTPUT_FORMAT output_channels,
                                            S32                output_sample_rate,
                                            S32                n_physical_voices,
                                            U32                MEV_flags)
{
   if (E == NULL)
      {
      return MARS_BAD_PARAM;
      }   

   //
   // Shut down existing driver, if any
   //

   if (E->DIG != NULL)
      {
      //
      // Either destroy all existing sound instances, or release their physical
      // voices and preserve them for restoration
      //

      SNDDEF *D = E->first_def;

      while (D != NULL)
         {
         if (!(MEV_flags & MEV_RESTORE_SOUND_INSTANCES))
            {
            //
            // Sound instances will be deleted
            //

            MARS_end(D);
            }
         else
            {
            //
            // Sound instances will be left intact and given new physical
            // voices after the driver has been loaded
            //

            SNDINST *I = D->first_inst;

            while (I != NULL)
               {
               I->T_status = I->triggered;

               if (!I->has_voice)
                  {
                  I->S_status = SMP_FREE;
                  }
               else
                  {
                  I->S_status = AIL_sample_status(I->S);
                  assert(I->S_status != SMP_FREE);

                  if ((I->S_status == SMP_PLAYING) ||
                      (I->S_status == SMP_STOPPED))
                     {
                     I->S_play_cursor_bytes = AIL_sample_position(I->S);
                     }

                  release_voice(I, TRUE);
                  }

               I = I->next_inst;
               }
            }

         D = D->next_def;
         }

      AIL_close_digital_driver(E->DIG);
      E->DIG = NULL;
      }

   //
   // Create driver
   //
   // TODO: All digital drivers must use the same # of HSAMPLEs, because
   // it's a global preference
   //

   AIL_set_preference(DIG_MIXER_CHANNELS, n_physical_voices);

   E->DIG = AIL_open_digital_driver(output_sample_rate,
                                    16,
                      (MSS_MC_SPEC) output_channels,
                                    MEV_flags);
                                    
   if (E->DIG == NULL)
      {
      return MARS_DRIVER_FAILURE;
      }

   E->global_reverb_supported = (AIL_room_type(E->DIG) != -1);

   AIL_speaker_configuration(E->DIG,
                             NULL,
                            &E->n_logical_channels,
                             NULL,
            (MSS_MC_SPEC *) &E->output_format);

   E->n_physical_voices  = n_physical_voices;
   E->output_sample_rate = output_sample_rate;
   E->MEV_flags          = MEV_flags;

   //
   // Establish initial driver state, making current and input properties
   // identical
   //

   update_HDIGDRIVER(E->DIG,
                    &E->input_properties, 
                     NULL);

   E->current_properties = E->input_properties;

   //
   // Restore physical voices to previously-stopped or -playing sound instances,
   // if requested
   // 

   if (MEV_flags & MEV_RESTORE_SOUND_INSTANCES)
      {
      SNDDEF *D = E->first_def;

      while (D != NULL)
         {
         SNDINST *I = D->first_inst;

         while (I != NULL)
            {
            I->triggered = I->T_status;

            if ((I->S_status == SMP_PLAYING) ||
                (I->S_status == SMP_STOPPED))
               {
               D->input_properties.play_cursor_bytes = I->S_play_cursor_bytes;

               if (assign_voice(I) == MARS_OK)
                  {
                  if (I->S_status == SMP_PLAYING) 
                     {
                     AIL_resume_sample (I->S);
                     }
                  }
               }

            I = I->next_inst;
            }

         D = D->next_def;
         }
      }

   return MARS_OK;
}

MARS_RESULT MARS_destroy_environment       (HMENV E)
{
   if (E == NULL)
      {
      return MARS_BAD_PARAM;
      }   

   while (E->first_def != NULL)
      {
      MARS_RESULT result = MARS_destroy_sound(E->first_def); 

      if (result != MARS_OK)
         {
         return result;
         }
      }

   if (E->DIG != NULL)
      {
      AIL_close_digital_driver(E->DIG);
      E->DIG = NULL;
      }

   if (E->V != NULL)
      {
      AIL_mem_free_lock(E->V);
      E->V = NULL;
      }

   return MARS_OK;
}

MARS_RESULT MARS_save_environment_file     (HMENV       E,
                                            C8         *ENV_filename,
                                            U32         MSE_flags)
{
   if (E == NULL)
      {
      return MARS_BAD_PARAM;
      }  

   FILE *out = fopen(ENV_filename,"wt");  // TODO

   if (out == NULL)
      {
      return MARS_FILE_NOT_WRITABLE;
      }

   //
   // Dump environment properties
   //

   MARS_PROPERTY_INFO info;
   HMARSENUM next = MARS_FIRST;

   fprintf(out,"[Environment]\n{\n");

   //
   // Write output format settings that will need to be restored
   // before anything else
   //

   fprintf(out,"   Output format = 0x%X,%d,%d,0x%X\n",
      E->output_format,
      E->output_sample_rate,
      E->n_physical_voices,
      E->MEV_flags);

   //
   // Write remaining environment properties
   //

   while (MARS_enumerate_environment_properties(E, 
                                               &next, 
                                               &info) != MARS_DONE)
      {
      C8 data[MARS_MAX_PROP_BIN_LEN];
      C8 text[MARS_MAX_PROP_TEXT_LEN];

      if (MARS_environment_property(E,
                                    info.name,
                                   &data[0],
                                    NULL,
                                    NULL) != MARS_OK)
         {
         continue;
         }

      if (info.MPF_flags & (MPF_UNSAVED | MPF_ALTERNATE))
         {
         continue;
         }

      property_to_text(info.type,
                      &data[0],
                       text);

      fprintf(out,"   %s = %s\n",info.name,text);
      }

   fprintf(out,"}\n\n");

   //
   // Dump properties for all registered sound definitions
   //

   HMARSENUM next_sound = MARS_FIRST;
   SNDDEF *D = NULL;
   
   while (MARS_enumerate_sounds(E,
                                NULL,
                               &next_sound,
                               &D) != MARS_DONE)
      {
      fprintf(out,"[Sound definition]\n{\n");

      while (MARS_enumerate_sound_properties(D,
                                            &next, 
                                            &info) != MARS_DONE)
         {
         C8 data[MARS_MAX_PROP_BIN_LEN];
         C8 text[MARS_MAX_PROP_TEXT_LEN];

         if (MARS_sound_property(D,
                                 info.name,
                                &data[0],
                                 NULL,
                                 NULL) != MARS_OK)
            {
            continue;
            }

         if (info.MPF_flags & (MPF_UNSAVED | MPF_ALTERNATE))
            {
            continue;
            }

         property_to_text(info.type,
                         &data[0],
                          text);

         fprintf(out,"   %s = %s\n",info.name,text);
         }

      fprintf(out,"}\n\n");
      }

   fclose(out);

   return MARS_OK;
}

MARS_RESULT MARS_load_environment_file  (HMENV  E, 
                                         C8    *ENV_filename,
                                         U32    MLE_flags)
{
   enum FILE_SECTION 
      {
      FSEC_NONE,
      FSEC_ENV,
      FSEC_SNDDEF
      };

   FILE_SECTION section = FSEC_NONE;

   if (E == NULL)
      {
      return MARS_BAD_PARAM;
      }  

   FILE *infile = fopen(ENV_filename,"rt");  // TODO

   if (infile == NULL)
      {
      return MARS_FILE_NOT_READABLE;
      }

   MARS_RESULT result      = MARS_OK;
   SNDDEF *D               = NULL;
   S32 output_format_valid = FALSE;

   while (1)
      {
      //
      // Read input line
      //

      C8 linbuf[MARS_MAX_PROP_TEXT_LEN*2];

      memset(linbuf,
             0,
             sizeof(linbuf));

      C8 *r = fgets(linbuf, 
                    sizeof(linbuf) - 1, 
                    infile);

      if ((r == NULL) || (feof(infile)))
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
      // Check for categories
      //

      if (!strcmp(lexeme,"[Environment]"))
         {
         section = FSEC_ENV; 

         output_format_valid = FALSE;
         continue;
         }

      if (!strcmp(lexeme,"[Sound definition]"))
         {
         section = FSEC_SNDDEF; 

         D = NULL;
         continue;
         }

      if (!strcmp(lexeme,"}"))
         {
         section = FSEC_NONE;
         continue;
         }

      if (section == FSEC_NONE)
         {
         continue;
         }

      //
      // Terminate key substring at first '=' sign and remove
      // spaces to either side
      //

      C8 *value = strchr(lexeme,'=');

      if (value == NULL)
         {
         continue;
         }

      *value = 0;
      C8 *key = value;

      while (isspace(*++value));
      while ((key > lexeme) && isspace(*--key)) *key = 0;

      key = lexeme;

      //
      // Restore environment and sound properties
      //
      
      if (section == FSEC_ENV)
         {
         if (!output_format_valid)
            {
            if (!strcmp(key,"Output format"))
               {
               MARS_OUTPUT_FORMAT output_format      = MARS_STEREO;
               S32                output_sample_rate = 44100;
               S32                n_physical_voices  = 0;
               U32                MEV_flags          = 0;

               sscanf(value,"0x%X,%d,%d,0x%X\n",
                  &output_format,
                  &output_sample_rate,
                  &n_physical_voices,
                  &MEV_flags);

               result = MARS_set_output_format(E,
                                               output_format,
                                               output_sample_rate,
                                               n_physical_voices,
                                               MEV_flags);

               if (result != MARS_OK)
                  {
                  return result;
                  }

               output_format_valid = TRUE;
               }

            continue;
            }

         result = MARS_environment_property_text(E,
                                                 key,
                                                 NULL,
                                                 value,
                                                 NULL);

         if ((result != MARS_OK) && (result != MARS_NOT_WRITABLE))
            {
            return result;
            }
         }
      else if (section == FSEC_SNDDEF)
         {
         //
         // TODO: Ensure that tags are saved first, so they can be used to
         // create the sound definition
         //

         if (D == NULL)
            {
            result = MARS_create_sound(E,
                                       NULL,   // TODO
                                      &D);

            if (result != MARS_OK)
               {
               return result;
               }
            }

         result = MARS_sound_property_text(D,
                                           key,
                                           NULL,
                                           value,
                                           NULL);

         if ((result != MARS_OK) && (result != MARS_NOT_WRITABLE))
            {
            return result;
            }
         }
      }

   fclose(infile);

   return MARS_OK;

}

MARS_RESULT MARS_output_driver (HMENV         E, 
                                HDIGDRIVER   *driver)
{
   if ((E == NULL) || (driver == NULL))
      {
      return MARS_BAD_PARAM;
      }

   *driver = E->DIG;

   return MARS_OK;
}

MARS_RESULT MARS_output_filter (HMENV         E,
                                HPROVIDER    *provider,
                                S32          *global_reverb_supported)
{
   if (E == NULL)
      {
      return MARS_BAD_PARAM;
      }

   if (provider != NULL) *provider = AIL_digital_output_filter(E->DIG);
   if (global_reverb_supported != NULL) *global_reverb_supported = E->global_reverb_supported;

   return MARS_OK;
}

void _________________________________________________Environment_properties____________________________________________________(){}

MARS_RESULT MARS_enumerate_environment_properties
                                            (HMENV                E,
                                             HMARSENUM           *next,
                                             MARS_PROPERTY_INFO  *dest)
{
   if ((E == NULL) || (next == NULL) || (dest == NULL))
      {
      return MARS_BAD_PARAM;
      }

   S32 index = *next;

   if (index == MARS_FIRST)
      {
      index = 0;
      }

   if (index >= ARY_CNT(ENV_properties))
      {
      *next = 0;
      return MARS_DONE;
      }

   *dest = ENV_properties[index];
   index++;

   *next = index;
   return MARS_OK;
}

MARS_RESULT MARS_environment_property(HMENV         E,
                                      MARS_PROPERTY name,
                                      void *        before_value,
                                      void const *  new_value,
                                      void *        after_value,
                                      void *        min_limit,
                                      void *        max_limit,
                                      C8 **         description)

{
   if (E == NULL)
      {
      return MARS_BAD_PARAM;
      }

   //
   // Get property index from string (TODO: hash)
   //

   MARS_PROPERTY_INFO *info = NULL;

   S32 index = ENVSTATE_property(name,
                                &info);

   if (index == -1)
      {
      return MARS_BAD_PARAM;
      }

   if ((new_value != NULL) && (info->MPF_flags & MPF_READ_ONLY))
      {
      return MARS_NOT_WRITABLE;
      }

   HDIGDRIVER DIG =  E->DIG;
   ENVSTATE  *EB  = &E->input_properties;    // Used for "before_value" results
   ENVSTATE  *EA  = &E->current_properties;  // Used for "after_value" results

   switch (index)
      {
      default: return MARS_BAD_PARAM;

      case ENUM_ENV_N_PLAYING_VOICES:
         {
         if (DIG == NULL)       return MARS_RESOURCE_UNAVAILABLE;

         S32 n = AIL_active_sample_count(DIG);

         if (before_value != NULL) *(S32 *) before_value = n;
         if (after_value  != NULL) *(S32 *) after_value  = n;
         if (min_limit != NULL)   *(S32 *) min_limit = 0;
         if (max_limit != NULL)   *(S32 *) max_limit = E->n_physical_voices;
         if (description != NULL) *(C8 **) description = "Number of physical voices (HSAMPLES) currently in use (Read-only)";
         return MARS_OK;
         }

      case ENUM_ENV_F_LATENCY_MS:
         {
         if (DIG == NULL)       return MARS_RESOURCE_UNAVAILABLE;

         F32 n = (F32) AIL_digital_latency(DIG);

         if (before_value != NULL) *(F32 *) before_value = n;
         if (after_value  != NULL) *(F32 *) after_value = n;
         if (min_limit != NULL)    *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)    *(F32 *) max_limit = 1000.0F;
         if (description != NULL)  *(C8 **) description = "Anticipated overall latency in milliseconds, including both MSS and driver overhead where available (Read-only)";
         return MARS_OK;
         }

      case ENUM_ENV_N_CPU_PERCENT:
         {
         if (DIG == NULL)       return MARS_RESOURCE_UNAVAILABLE;

         S32 n = AIL_digital_CPU_percent(DIG);

         n--; if (n < 0) n=0;

         if (before_value != NULL) *(S32 *) before_value = n;
         if (after_value  != NULL) *(S32 *) after_value = n;
         if (min_limit != NULL)    *(S32 *) min_limit = 0;
         if (max_limit != NULL)    *(S32 *) max_limit = 100;
         if (description != NULL)  *(C8 **) description = "Total utilization of CPU core(s) associated with mixing, decompression, and other background tasks (Read-only)";
         return MARS_OK;
         }

      case ENUM_ENV_F_MASTER_VOLUME :
         {
         if (before_value != NULL) *(F32 *) before_value = scalar_to_loudness(EB->master_scalar);

         if (new_value != NULL)
            {
            EB->master_scalar = loudness_to_scalar(*(F32 *) new_value);
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(F32 *) after_value = scalar_to_loudness(EA->master_scalar);
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Master volume for all sounds played in the current environment, based on a relative-loudness scale";
         return MARS_OK;
         }

      case ENUM_ENV_F_MASTER_VOLUME_DB:
         {
         if (before_value != NULL) *(F32 *) before_value = scalar_to_dB(EB->master_scalar);

         if (new_value != NULL)
            {
            EB->master_scalar = dB_to_scalar(*(F32 *) new_value);
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(F32 *) after_value = scalar_to_dB(EA->master_scalar);
         if (min_limit != NULL)   *(F32 *) min_limit = -96.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 0.0F;
         if (description != NULL) *(C8 **) description = "Master volume for all sounds played in the current environment (dB relative to full scale)";
         return MARS_OK;
         }

      case ENUM_ENV_F_MASTER_VOLUME_SCALE :
         {
         if (before_value != NULL) *(F32 *) before_value = EB->master_scalar;

         if (new_value != NULL)
            {
            EB->master_scalar = *(F32 *) new_value;
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(F32 *) after_value = EA->master_scalar;
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Master volume for all sounds played in the current environment (linear DAC scalar)";
         return MARS_OK;
         }

      case ENUM_ENV_RTYPE_REVERB_ROOM_TYPE:
         {
         if (before_value != NULL) *(MARS_ROOM_TYPE *) before_value = EB->room_type;

         if (new_value != NULL)
            {
            EB->room_type       = *(MARS_ROOM_TYPE *) new_value;
            EB->room_type_valid = TRUE;
            ENV_update_properties(E);     // TODO: log time/predelay/damping vals for this room type
            }

         if (after_value != NULL) *(MARS_ROOM_TYPE *) after_value = EA->room_type;
         if (min_limit != NULL)   *(S32 *) min_limit = MARS_ROOM_GENERIC;
         if (max_limit != NULL)   *(S32 *) max_limit = MARS_ROOM_COUNT + MARS_ROOM_GENERIC - 1;
         if (description != NULL) *(C8 **) description = "A value of type MARS_ROOM_TYPE that specifies a particular combination of global reverb decay time, predelay, and damping preset values";
         return MARS_OK;
         }

      case ENUM_ENV_F_REVERB_TIME_SECS:
         {
         if (before_value != NULL) *(F32 *) before_value = EB->reverb_time_secs;

         if (new_value != NULL)
            {
            EB->reverb_time_secs = *(F32 *) new_value;
            EB->room_type_valid  = FALSE;
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(F32 *) after_value = EA->reverb_time_secs;
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 3.0F;
         if (description != NULL) *(C8 **) description = "Global reverb decay time in seconds";
         return MARS_OK;
         }

      case ENUM_ENV_F_REVERB_PREDELAY_SECS:
         {
         if (before_value != NULL) *(F32 *) before_value = EB->reverb_predelay_secs;

         if (new_value != NULL)
            {
            EB->reverb_predelay_secs = *(F32 *) new_value;
            EB->room_type_valid  = FALSE;
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(F32 *) after_value = EA->reverb_predelay_secs;
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 3.0F;
         if (description != NULL) *(C8 **) description = "Global reverb predelay time in seconds";
         return MARS_OK;
         }

      case ENUM_ENV_F_REVERB_DAMPING:
         {
         if (before_value != NULL) *(F32 *) before_value = EB->reverb_damping;

         if (new_value != NULL)
            {
            EB->reverb_damping  = *(F32 *) new_value;
            EB->room_type_valid = FALSE;
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(F32 *) after_value = EA->reverb_damping;
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Global reverb damping factor";
         return MARS_OK;
         }

      case ENUM_ENV_F_REVERB_MASTER_DRY_VOL:
         {
         if (before_value != NULL) *(F32 *) before_value = scalar_to_loudness(EB->reverb_master_dry_scalar);

         if (new_value != NULL)
            {
            EB->reverb_master_dry_scalar = loudness_to_scalar(*(F32 *) new_value);
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(F32 *) after_value = scalar_to_loudness(EA->reverb_master_dry_scalar);
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Global reverb dry-path volume, based on a relative-loudness scale";
         return MARS_OK;
         }

      case ENUM_ENV_F_REVERB_MASTER_WET_VOL:
         {
         if (before_value != NULL) *(F32 *) before_value = scalar_to_loudness(EB->reverb_master_wet_scalar);

         if (new_value != NULL)
            {
            EB->reverb_master_wet_scalar = loudness_to_scalar(*(F32 *) new_value);
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(F32 *) after_value = scalar_to_loudness(EA->reverb_master_wet_scalar);
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Global reverb wet-path volume, based on a relative-loudness scale";
         return MARS_OK;
         }

      case ENUM_ENV_F_REVERB_MASTER_DRY_DB:
         {
         if (before_value != NULL) *(F32 *) before_value = scalar_to_dB(EB->reverb_master_dry_scalar);

         if (new_value != NULL)
            {
            EB->reverb_master_dry_scalar = dB_to_scalar(*(F32 *) new_value);
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(F32 *) after_value = scalar_to_dB(EA->reverb_master_dry_scalar);
         if (min_limit != NULL)   *(F32 *) min_limit = -96.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 0.0F;
         if (description != NULL) *(C8 **) description = "Global reverb dry-path volume (dB relative to full scale)";
         return MARS_OK;
         }

      case ENUM_ENV_F_REVERB_MASTER_WET_DB:
         {
         if (before_value != NULL) *(F32 *) before_value = scalar_to_dB(EB->reverb_master_wet_scalar);

         if (new_value != NULL)
            {
            EB->reverb_master_wet_scalar = dB_to_scalar(*(F32 *) new_value);
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(F32 *) after_value = scalar_to_dB(EA->reverb_master_wet_scalar);
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Global reverb wet-path volume (linear DAC scalar)";
         return MARS_OK;
         }

      case ENUM_ENV_F_REVERB_MASTER_DRY_SCALE:
         {
         if (before_value != NULL) *(F32 *) before_value = EB->reverb_master_dry_scalar;

         if (new_value != NULL)
            {
            EB->reverb_master_dry_scalar = *(F32 *) new_value;
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(F32 *) after_value = EA->reverb_master_dry_scalar;
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Global reverb dry-path volume (linear DAC scalar)";
         return MARS_OK;
         }

      case ENUM_ENV_F_REVERB_MASTER_WET_SCALE:
         {
         if (before_value != NULL) *(F32 *) before_value = EB->reverb_master_wet_scalar;

         if (new_value != NULL)
            {
            EB->reverb_master_wet_scalar = *(F32 *) new_value;
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(F32 *) after_value = EA->reverb_master_wet_scalar;
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Global reverb wet-path volume (linear DAC scalar)";
         return MARS_OK;
         }

      case ENUM_ENV_F_LISTENER_X_POS:
         {
         if (before_value != NULL) *(F32 *) before_value = EB->position.x;

         if (new_value != NULL)
            {
            EB->position.x = *(F32 *) new_value;
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(F32 *) after_value = EA->position.x;
         if (min_limit != NULL)   *(F32 *) min_limit = -500.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 500.0F;
         if (description != NULL) *(C8 **) description = "Listener X position";
         return MARS_OK;
         }

      case ENUM_ENV_F_LISTENER_X_FACE :
         {
         if (before_value != NULL) *(F32 *) before_value = EB->face.x;

         if (new_value != NULL)
            {
            EB->face.x = *(F32 *) new_value;
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(F32 *) after_value = EA->face.x;
         if (min_limit != NULL)   *(F32 *) min_limit = -1.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Listener X facing direction";
         return MARS_OK;
         }

      case ENUM_ENV_F_LISTENER_X_UP :
         {
         if (before_value != NULL) *(F32 *) before_value = EB->up.x;

         if (new_value != NULL)
            {
            EB->up.x = *(F32 *) new_value;
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(F32 *) after_value = EA->up.x;
         if (min_limit != NULL)   *(F32 *) min_limit = -1.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Listener X up vector";
         return MARS_OK;
         }

      case ENUM_ENV_F_LISTENER_X_VEL:
         {
         if (before_value != NULL) *(F32 *) before_value = EB->velocity.x;

         if (new_value != NULL)
            {
            EB->velocity.x = *(F32 *) new_value;
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(F32 *) after_value = EA->velocity.x;
         if (min_limit != NULL)   *(F32 *) min_limit = -1.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Listener X velocity";
         return MARS_OK;
         }

      case ENUM_ENV_F_LISTENER_Y_POS:
         {
         if (before_value != NULL) *(F32 *) before_value = EB->position.y;

         if (new_value != NULL)
            {
            EB->position.y = *(F32 *) new_value;
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(F32 *) after_value = EA->position.y;
         if (min_limit != NULL)   *(F32 *) min_limit = -1.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Listener Y position";
         return MARS_OK;
         }

      case ENUM_ENV_F_LISTENER_Y_FACE :
         {
         if (before_value != NULL) *(F32 *) before_value = EB->face.y;

         if (new_value != NULL)
            {
            EB->face.y = *(F32 *) new_value;
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(F32 *) after_value = EA->face.y;
         if (min_limit != NULL)   *(F32 *) min_limit = -1.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Listener Y facing direction";
         return MARS_OK;
         }

      case ENUM_ENV_F_LISTENER_Y_UP :
         {
         if (before_value != NULL) *(F32 *) before_value = EB->up.y;

         if (new_value != NULL)
            {
            EB->up.y = *(F32 *) new_value;
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(F32 *) after_value = EA->up.y;
         if (min_limit != NULL)   *(F32 *) min_limit = -1.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Listener Y up vector";
         return MARS_OK;
         }

      case ENUM_ENV_F_LISTENER_Y_VEL:
         {
         if (before_value != NULL) *(F32 *) before_value = EB->velocity.y;

         if (new_value != NULL)
            {
            EB->velocity.y = *(F32 *) new_value;
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(F32 *) after_value = EA->velocity.y;
         if (min_limit != NULL)   *(F32 *) min_limit = -1.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Listener Y velocity";
         return MARS_OK;
         }

      case ENUM_ENV_F_LISTENER_Z_POS:
         {
         if (before_value != NULL) *(F32 *) before_value = EB->position.z;

         if (new_value != NULL)
            {
            EB->position.z = *(F32 *) new_value;
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(F32 *) after_value = EA->position.z;
         if (min_limit != NULL)   *(F32 *) min_limit = -1.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Listener Z position";
         return MARS_OK;
         }

      case ENUM_ENV_F_LISTENER_Z_FACE :
         {
         if (before_value != NULL) *(F32 *) before_value = EB->face.z;

         if (new_value != NULL)
            {
            EB->face.z = *(F32 *) new_value;
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(F32 *) after_value = EA->face.z;
         if (min_limit != NULL)   *(F32 *) min_limit = -1.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Listener Z facing direction";
         return MARS_OK;
         }

      case ENUM_ENV_F_LISTENER_Z_UP :
         {
         if (before_value != NULL) *(F32 *) before_value = EB->up.z;

         if (new_value != NULL)
            {
            EB->up.z = *(F32 *) new_value;
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(F32 *) after_value = EA->up.z;
         if (min_limit != NULL)   *(F32 *) min_limit = -1.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Listener Z up vector";
         return MARS_OK;
         }

      case ENUM_ENV_F_LISTENER_Z_VEL:
         {
         if (before_value != NULL) *(F32 *) before_value = EB->velocity.z;

         if (new_value != NULL)
            {
            EB->velocity.z = *(F32 *) new_value;
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(F32 *) after_value = EA->velocity.z;
         if (min_limit != NULL)   *(F32 *) min_limit = -1.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Listener Z velocity";
         return MARS_OK;
         }

      case ENUM_ENV_V_LISTENER_POS:
         {
         if (before_value != NULL) *(MSSVECTOR3D *) before_value = EB->position;

         if (new_value != NULL)
            {
            EB->position = *(MSSVECTOR3D *) new_value;
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(MSSVECTOR3D *) after_value = EA->position;
         if (description != NULL) *(C8 **) description = "Listener 3D position vector";
         return MARS_OK;
         }

      case ENUM_ENV_V_LISTENER_FACE:
         {
         if (before_value != NULL) *(MSSVECTOR3D *) before_value = EB->position;

         if (new_value != NULL)
            {
            EB->position = *(MSSVECTOR3D *) new_value;
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(MSSVECTOR3D *) after_value = EA->position;
         if (description != NULL) *(C8 **) description = "Listener 3D facing vector";
         return MARS_OK;
         }

      case ENUM_ENV_V_LISTENER_UP:
         {
         if (before_value != NULL) *(MSSVECTOR3D *) before_value = EB->up;

         if (new_value != NULL)
            {
            EB->up = *(MSSVECTOR3D *) new_value;
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(MSSVECTOR3D *) after_value = EA->up;
         if (description != NULL) *(C8 **) description = "Listener 3D up vector";
         return MARS_OK;
         }

      case ENUM_ENV_V_LISTENER_VEL:
         {
         if (before_value != NULL) *(MSSVECTOR3D *) before_value = EB->velocity;

         if (new_value != NULL)
            {
            EB->velocity = *(MSSVECTOR3D *) new_value;
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(MSSVECTOR3D *) after_value = EA->velocity;
         if (description != NULL) *(C8 **) description = "Listener 3D velocity (direction)";
         return MARS_OK;
         }

      case ENUM_ENV_F_LISTENER_MAG:
         {
         if (before_value != NULL) *(F32 *) before_value = EB->velocity_mag;

         if (new_value != NULL)
            {
            EB->velocity_mag = *(F32 *) new_value;
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(F32 *) after_value = EA->velocity_mag;
         if (description != NULL) *(C8 **) description = "Listener 3D velocity (magnitude)";
         return MARS_OK;
         }

      case ENUM_ENV_F_ROLLOFF :
         {
         if (before_value != NULL) *(F32 *) before_value = EB->rolloff_factor;

         if (new_value != NULL)
            {
            EB->rolloff_factor = *(F32 *) new_value;
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(F32 *) after_value = EA->rolloff_factor;
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Listener 3D distance-rolloff factor";
         return MARS_OK;
         }

      case ENUM_ENV_F_DOPPLER :
         {
         if (before_value != NULL) *(F32 *) before_value = EB->doppler_factor;

         if (new_value != NULL)
            {
            EB->doppler_factor = *(F32 *) new_value;
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(F32 *) after_value = EA->doppler_factor;
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Doppler-effect scale factor for the current environment";
         return MARS_OK;
         }

      case ENUM_ENV_F_FALLOFF_POWER :
         {
         if (before_value != NULL) *(F32 *) before_value = EB->falloff_power;

         if (new_value != NULL)
            {
            EB->falloff_power = *(F32 *) new_value;
            ENV_update_properties(E);
            }

         if (after_value != NULL) *(F32 *) after_value = EA->falloff_power;
         if (min_limit != NULL)   *(F32 *) min_limit = 1.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 5.0F;
         if (description != NULL) *(C8 **) description = "Speaker directionality factor. Higher values yield more-dramatic 3D spatialization, while lower values improve level flatness";
         return MARS_OK;
         }
      }
   
   return MARS_OK;
}

MARS_RESULT MARS_environment_property_text(HMENV         E,
                                           MARS_PROPERTY name,
                                           C8 *          before_value,
                                           C8 const *    new_value,
                                           C8 *          after_value,
                                           void *        min_limit,
                                           void *        max_limit,
                                           C8 **         description)

{
   if (E == NULL)
      {
      return MARS_BAD_PARAM;
      }

   //
   // Get property index from string (TODO: hash)
   // (Also redundant with the actual property-set call below)
   //

   MARS_PROPERTY_INFO *info;

   S32 index = ENVSTATE_property(name,
                                &info);
   if (index == -1)
      {
      return MARS_BAD_PROPERTY;
      }

   C8 new_bin[MARS_MAX_PROP_BIN_LEN];

   if (new_value != NULL)
      {
      if (!text_to_property(info->type,
                   (C8 **) &new_value,
                           &new_bin))
         {
         return MARS_BAD_PROPERTY;
         }
      }

   C8 before_bin[MARS_MAX_PROP_BIN_LEN];
   C8 after_bin [MARS_MAX_PROP_BIN_LEN];

   MARS_RESULT result = MARS_environment_property(E,
                                                  info->name,
                                                  (before_value == NULL) ? NULL : before_bin,
                                                  (new_value == NULL)    ? NULL : new_bin,
                                                  (after_value == NULL)  ? NULL : after_bin,
                                                  min_limit,
                                                  max_limit,
                                                  description);
   if (result != MARS_OK)
      {
      return result;
      }

   if (before_value != NULL)
      {
      if (!property_to_text(info->type,
                            before_bin,
                            before_value))
         {
         return MARS_BAD_PROPERTY;
         }
      }

   if (after_value != NULL)
      {
      if (!property_to_text(info->type,
                            after_bin,
                            after_value))
         {
         return MARS_BAD_PROPERTY;
         }
      }

   return MARS_OK;      
}

MARS_RESULT MARS_defer_environment_settings (HMENV environment)
{
   return MARS_OK;
}

MARS_RESULT MARS_commit_environment_settings(HMENV environment)
{
   return MARS_OK;
}

MARS_RESULT MARS_speaker_wet_level(HMENV        environment, 
                                   MARS_SPEAKER speaker,
                                   F32         *before_value,
                                   F32 const   *new_value)
{
   return MARS_OK;
}

MARS_RESULT MARS_speaker_dry_level(HMENV        environment, 
                                   MARS_SPEAKER speaker,
                                   F32         *before_value,
                                   F32 const   *new_value)
{
   return MARS_OK;
}

MARS_RESULT MARS_speaker_wet_scale (HMENV        environment, 
                                    MARS_SPEAKER speaker,
                                    F32         *before_value,
                                    F32 const   *new_value)
{
   return MARS_OK;
}

MARS_RESULT MARS_speaker_dry_scale (HMENV        environment, 
                                    MARS_SPEAKER speaker,
                                    F32         *before_value,
                                    F32 const   *new_value)
{
   return MARS_OK;
}

MARS_RESULT MARS_speaker_wet_dB  (HMENV        environment, 
                                  MARS_SPEAKER speaker,
                                  F32         *before_value,
                                  F32 const   *new_value)
{
   return MARS_OK;
}

MARS_RESULT MARS_speaker_dry_dB  (HMENV        environment, 
                                  MARS_SPEAKER speaker,
                                  F32         *before_value,
                                  F32 const   *new_value)
{
   return MARS_OK;
}

MARS_RESULT MARS_speaker_location(HMENV         environment, 
                                  MARS_SPEAKER  speaker,
                                  MSSVECTOR3D  *before_value,
                                  MSSVECTOR3D  *new_value,
                                  MSSVECTOR3D  *after_value)
{
   return MARS_OK;
}

void _________________________________________________Sound_objects____________________________________________________(){}

MARS_RESULT MARS_create_sound (HMENV    E,
                               C8      *tagset,
                               HMSOUND *sound)
{
   if ((E == NULL) || (sound == NULL))
      {
      return MARS_BAD_PARAM;
      }

   // TODO: Allocate these from a growable pool, not one at a time
   SNDDEF *D = (SNDDEF *) AIL_mem_alloc_lock(sizeof(SNDDEF));

   if (D == NULL)
      {
      return MARS_OUT_OF_MEMORY;
      }

   memset(D, 0, sizeof(SNDDEF));

   D->next_def = E->first_def;
   D->prev_def = NULL;
   D->E        = E;

   if (E->first_def != NULL)
      {
      E->first_def->prev_def = D;
      }

   E->first_def = D;

   //
   // Establish default initial state for future instances
   //

   set_SNDSTATE_defaults(D, 
                        &D->input_properties);

   *sound = D;

   return MARS_OK;
}

MARS_RESULT MARS_destroy_sound(HMSOUND D)
{
   if (D == NULL)
      {
      return MARS_BAD_PARAM;
      }

   HMENV E = D->E;

   //
   // Stop the sound and free all of its instances and file data
   //

   MARS_RESULT result = MARS_discard(D);

   if (result != MARS_OK)
      {
      return result;
      }

   //
   // Unlink from environment pool
   //

   if (D->prev_def == NULL)
      {
      E->first_def = D->next_def;
      }
   else
      {
      D->prev_def->next_def = D->next_def;
      }

   if (D->next_def != NULL)
      {
      D->next_def->prev_def = D->prev_def;
      }

   //
   // Free file image
   //

   if (D->file_memory_ptr != NULL)
      {
      AIL_mem_free_lock(D->file_memory_ptr);
      D->file_memory_ptr = NULL;
      }

   AIL_mem_free_lock(D);

   return MARS_OK;
}

MARS_RESULT MARS_copy_sound   (HMSOUND  original,
                               HMSOUND *copy)
{
   return MARS_OK;
}                                 

MARS_RESULT MARS_enumerate_sounds(HMENV      E,
                                  C8        *tagset,
                                  HMARSENUM *next,
                                  HMSOUND   *dest)
{
   if ((E == NULL) || (next == NULL) || (dest == NULL))
      {
      return MARS_BAD_PARAM;
      }

   if (*next == NULL)
      {
      //
      // Most-recently-returned definition was the last one
      //

      return MARS_DONE; 
      }

   if (*next == MARS_FIRST)
      {
      *next = NULL;
      }

   SNDDEF *current = find_qualified_SNDDEF(E, (SNDDEF *) *next, tagset); 

   if (current == NULL)
      {
      //
      // No (further) definitions qualify
      // 

      return MARS_DONE;
      }

   // 
   // We found one -- return it, setting *next to the next definition in the list
   //

   *dest = current;
   *next = (HMARSENUM) current->next_def;

   return MARS_OK;
}

MARS_RESULT MARS_set_sound_file(HMSOUND D,            // TODO, replace with general helper function
                                C8     *filename)
{
   return MARS_sound_property_text(D,
                                   SOUND_SZ_FILENAME,
                                   NULL,
                                   filename,
                                   NULL);
}

MARS_RESULT MARS_preload(HMSOUND D,
                         S32     assign)
{
   if (D == NULL)
      {
      return MARS_BAD_PARAM;
      }

   if (!D->input_properties.filename[0])
      {
      return MARS_FILE_NOT_FOUND;
      }

   if (D->file_memory_ptr == NULL)
      {
      //
      // Load the file, if we haven't already done so
      //

      D->file_memory_ptr = (U32 *) AIL_file_read(D->input_properties.filename, FILE_READ_WITH_SIZE);

      if (D->file_memory_ptr == NULL)
         {
         switch (AIL_file_error())
            {
            case AIL_OUT_OF_MEMORY:  return MARS_OUT_OF_MEMORY;
            case AIL_FILE_NOT_FOUND: return MARS_FILE_NOT_FOUND;
            case AIL_CANT_READ_FILE: return MARS_FILE_NOT_READABLE;
            default:                 return MARS_GENERAL_ERROR;
            }
         }

      D->file_bytes = *D->file_memory_ptr;
      D->file_data  = &(D->file_memory_ptr[1]);
      }

   //
   // Create a new, untriggered instance of the specified sound if requested
   // (but only if we don't already have an unplayed instance at the head of the list)
   //

   SNDINST *I = D->first_inst;

   BOOL need_new_instance = FALSE;

   if ((I == NULL) || (I->triggered))
      {
      need_new_instance = TRUE;
      }

   if (assign && need_new_instance)
      {
      //
      // Try to allocate a new sound instance, including both a virtual 
      // and physical voice
      //
      // We may need an HSAMPLE/HSTREAM here (as opposed to MARS_play() time) to make
      // the file's properties available for inspection/modification before
      // playback begins
      //

      I = allocate_SNDINST(D);

      if (I == NULL)
         {
         return MARS_RESOURCE_UNAVAILABLE;
         }

      MARS_RESULT result = assign_voice(I);

      if (result != MARS_OK)
         {
         return result;
         }
      }

   return MARS_OK;
}

MARS_RESULT MARS_discard(HMSOUND D)
{
   //
   // Stop all voice instances and release their resources
   //

   MARS_end(D);

   //
   // Discard any existing file data
   // 

   if (D->file_memory_ptr != NULL)
      {
      AIL_mem_free_lock(D->file_memory_ptr);
      D->file_memory_ptr = NULL;

      D->file_data  = NULL;
      D->file_bytes = 0;
      }

   return MARS_OK;
}

void _________________________________________________Sound_properties________________________________________________(){}

MARS_RESULT MARS_enumerate_sound_properties (HMSOUND             D,   
                                             HMARSENUM          *next,
                                             MARS_PROPERTY_INFO *dest)
{
   if ((D == NULL) || (next == NULL) || (dest == NULL))
      {
      return MARS_BAD_PARAM;
      }

   S32 index = *next;

   if (index == MARS_FIRST)
      {
      index = 0;
      }

   if (index >= ARY_CNT(SND_properties))
      {
      *next = 0;
      return MARS_DONE;
      }

   *dest = SND_properties[index];
   index++;

   *next = index;
   return MARS_OK;
}

MARS_RESULT MARS_sound_property(HMSOUND       D,
                                MARS_PROPERTY name,
                                void *        before_value,
                                void const *  new_value,
                                void *        after_value,
                                void *        min_limit,
                                void *        max_limit,
                                C8 **         description)  // TODO: Move to sample info, replace with void *default_value param
{
   if (D == NULL)
      {
      return MARS_BAD_PARAM;
      }

   //
   // Get property index from string (TODO: hash)
   //

   MARS_PROPERTY_INFO *info = NULL;

   S32 index = SNDSTATE_property(name,
                                &info);

   if (index == -1)
      {
      return MARS_BAD_PARAM;
      }

   if ((new_value != NULL) && (info->MPF_flags & MPF_READ_ONLY))
      {
      return MARS_NOT_WRITABLE;
      }

   //
   // Get HSAMPLE corresponding to first instance of this sound definition,
   // if any
   //
   // This will be the HSAMPLE used to query dynamic properties 
   // (status, play cursor, etc.)
   // 

   HSAMPLE S = NULL;

   SNDSTATE *DB  = &D->input_properties;    // Used for "before_value" results
   SNDSTATE *DA  = DB;                      // Used for "after_value" results 

   SNDINST *I = D->first_inst;
 
   if ((I != NULL) && (I->has_voice))
      {
      S  =  I->S;
      DA = &I->current_properties;
      }

   switch (index)
      {
      case ENUM_SOUND_STATUS:
         {
         U32 status = 0;

         if (D->file_data != NULL)          status |= MARS_PRELOADED;
         if (S != NULL)                     status |= MARS_VOICED;
         if ((I != NULL) && (I->triggered)) status |= MARS_TRIGGERED;
         
         if (S != NULL)
            {
            U32 t = AIL_sample_status(S);

                 if (t == SMP_DONE)    status |= MARS_ENDED;
            else if (t == SMP_PLAYING) status |= MARS_PLAYING;
            else if (t == SMP_STOPPED) status |= MARS_PAUSED;
            }

         if (before_value != NULL) *(S32 *) before_value = status;
         if (after_value  != NULL) *(S32 *) after_value  = status;
         if (description != NULL) *(C8 **) description = "The playback status of the sound or stream (Read-only)";
         return MARS_OK;
         }

      case ENUM_SOUND_SZ_FILENAME:
         {
         if (before_value != NULL) strcpy((C8 *) before_value, DB->filename);

         if (new_value != NULL)
            {
            strcpy(DB->filename, (C8 *) new_value);
            SND_update_properties(D);
            }

         if (after_value != NULL) strcpy((C8 *) after_value, DA->filename);
         if (description != NULL) *(C8 **) description = "The sound or stream's filename";
         return MARS_OK;
         }

      case ENUM_SOUND_L_FILE_BYTES:
         {
         if (before_value != NULL) *(S64 *) before_value = D->file_bytes;
         if (after_value != NULL)  *(S64 *) after_value  = D->file_bytes;
         if (min_limit != NULL)   *(S64 *) min_limit = 0;
         if (max_limit != NULL)   *(S64 *) max_limit = 0x7fffffff;
         if (description != NULL) *(C8 **) description = "The size of the sound file in bytes, including any headers and metadata (Read-only)";

         return MARS_OK;
         }
                  
      case ENUM_SOUND_N_PLAYBACK_RATE_HZ:
         {
         if (before_value != NULL) *(S32 *) before_value = DB->playback_rate;

         if (new_value != NULL)
            {
            DB->playback_rate = *(S32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(S32 *) after_value  = DA->playback_rate;
         if (min_limit != NULL)   *(S32 *) min_limit = 1000;
         if (max_limit != NULL)   *(S32 *) max_limit = 192000;
         if (description != NULL) *(C8 **) description = "The rate, in samples per second, at which samples will be fetched from the sound data at playback time";
         return MARS_OK;
         }

      case ENUM_SOUND_N_ORIGINAL_RATE_HZ:
         {
         if (before_value != NULL) *(S32 *) before_value = D->original_sample_rate;
         if (after_value  != NULL) *(S32 *) after_value  = D->original_sample_rate;
         if (min_limit != NULL)    *(S32 *) min_limit = 1000;
         if (max_limit != NULL)    *(S32 *) max_limit = 192000;
         if (description != NULL)  *(C8 **) description = "The intended playback rate, in samples per second, which was specified when the file was authored (Read-only)";
         return MARS_OK;
         }

      case ENUM_SOUND_N_SOURCE_CHANNELS:
         {
         if (before_value != NULL) *(S32 *) before_value = D->n_source_channels;
         if (after_value  != NULL) *(S32 *) after_value  = D->n_source_channels;
         if (min_limit != NULL)    *(S32 *) min_limit = 1;
         if (max_limit != NULL)    *(S32 *) max_limit = 2;
         if (description != NULL)  *(C8 **) description = "The number of channels per sample in the sound file; typically 1 for mono or 2 for stereo. (Read-only)";
         return MARS_OK;
         }

      case ENUM_SOUND_B_IS_STREAM:
         {
         if (before_value != NULL) *(S32 *) before_value = D->is_streamed;

         if (new_value != NULL)
            {
            D->is_streamed = *(S32 *) new_value; // TODO: applies to future instances only...?:
            }

         if (after_value != NULL)  *(S32 *) after_value  = D->is_streamed;
         if (min_limit != NULL)    *(S32 *) min_limit = 0;
         if (max_limit != NULL)    *(S32 *) max_limit = 1;
         if (description != NULL)  *(C8 **) description = "0 if this is a memory-resident sound; 1 if it's streamed";
         return MARS_OK;
         }

      case ENUM_SOUND_B_IS_AUTO_STREAM:
         {
         if (before_value != NULL) *(S32 *) before_value = DB->auto_stream_service;

         if (new_value != NULL)
            {
            DB->auto_stream_service = *(S32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL)  *(S32 *) after_value  = DA->auto_stream_service;
         if (min_limit != NULL)    *(S32 *) min_limit = 0;
         if (max_limit != NULL)    *(S32 *) max_limit = 1;
         if (description != NULL)  *(C8 **) description = "Nonzero if a streaming sound will be serviced automatically; zero if the application intends to manage stream service by itself";
         return MARS_OK;
         }

      case ENUM_SOUND_N_STREAM_BUFFER_BYTES:
         {
         if (before_value != NULL) *(S32 *) before_value = D->stream_buffer_bytes;

         if (new_value != NULL)
            {
            D->stream_buffer_bytes = *(S32 *) new_value;   // TODO: applies to future instances only...?
            }

         if (after_value != NULL)  *(S32 *) after_value  = D->stream_buffer_bytes;
         if (min_limit != NULL)    *(S32 *) min_limit = 1024;
         if (max_limit != NULL)    *(S32 *) max_limit = 256000;    // TODO: Derive
         if (description != NULL)  *(C8 **) description = "The size of the internally-allocated buffer associated with a streaming sample.  Larger buffers are less prone to dropouts during periods of heavy system load";
         return MARS_OK;
         }

      case ENUM_SOUND_N_PRIORITY:
         {
         if (before_value != NULL) *(S32 *) before_value = D->priority;

         if (new_value != NULL)
            {
            D->priority = *(S32 *) new_value;   // TODO: deal with this
            }

         if (after_value != NULL)  *(S32 *) after_value  = D->priority;
         if (min_limit != NULL)    *(S32 *) min_limit = 0;
         if (max_limit != NULL)    *(S32 *) max_limit = 255;
         if (description != NULL)  *(C8 **) description = "When necessary, sounds can 'steal' voice-related resources from other sounds of equal or lesser priority";
         return MARS_OK;
         }

      case ENUM_SOUND_F_VOL:
         {
         if (before_value != NULL) *(F32 *) before_value = scalar_to_loudness(DB->volume_scalar);

         if (new_value != NULL)
            {
            DB->volume_scalar = loudness_to_scalar(*(F32 *) new_value);
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = scalar_to_loudness(DA->volume_scalar);
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Overall sound volume, based on a relative-loudness scale";
         return MARS_OK;
         }

      case ENUM_SOUND_F_DB:
         {
         if (before_value != NULL) *(F32 *) before_value = scalar_to_dB(DB->volume_scalar);

         if (new_value != NULL)
            {
            DB->volume_scalar = dB_to_scalar(*(F32 *) new_value);
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = scalar_to_dB(DA->volume_scalar);
         if (min_limit != NULL)   *(F32 *) min_limit = -96.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 0.0F;
         if (description != NULL) *(C8 **) description = "Overall sound volume (dB relative to full scale)";
         return MARS_OK;
         }

      case ENUM_SOUND_F_SCALE:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->volume_scalar;

         if (new_value != NULL)
            {
            DB->volume_scalar = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->volume_scalar;
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Overall sound volume, expressed as a linear DAC-voltage scale factor";
         return MARS_OK;
         }

      case ENUM_SOUND_F_LR_PAN:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->LR_pan;

         if (new_value != NULL)
            {
            DB->LR_pan = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->LR_pan;
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Constant-power pan value ranging from 0.0 (hard left) to 1.0 (hard right)";
         return MARS_OK;
         }

      case ENUM_SOUND_F_FB_PAN:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->FB_pan;

         if (new_value != NULL)
            {
            DB->FB_pan = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->FB_pan;
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Constant-power pan value ranging from 0.0 (front) to 1.0 (rear)";
         return MARS_OK;
         }

      case ENUM_SOUND_F_CENTER_VOL:
         {
         if (before_value != NULL) *(F32 *) before_value = scalar_to_loudness(DB->center_scalar);

         if (new_value != NULL)
            {
            DB->center_scalar = loudness_to_scalar(*(F32 *) new_value);
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = scalar_to_loudness(DA->center_scalar);
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Center-channel volume, based on a relative-loudness scale";
         return MARS_OK;
         }

      case ENUM_SOUND_F_CENTER_DB:
         {
         if (before_value != NULL) *(F32 *) before_value = scalar_to_dB(DB->center_scalar);

         if (new_value != NULL)
            {
            DB->center_scalar = dB_to_scalar(*(F32 *) new_value);
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = scalar_to_dB(DA->center_scalar);
         if (min_limit != NULL)   *(F32 *) min_limit = -96.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 0.0F;
         if (description != NULL) *(C8 **) description = "Center-channel volume (dB relative to full scale)";
         return MARS_OK;
         }

      case ENUM_SOUND_F_CENTER_SCALE:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->center_scalar;

         if (new_value != NULL)
            {
            DB->center_scalar = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->center_scalar;
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Center-channel volume, expressed as a linear DAC-voltage scale factor";
         return MARS_OK;
         }

      case ENUM_SOUND_F_LFE_VOL:
         {
         if (before_value != NULL) *(F32 *) before_value = scalar_to_loudness(DB->LFE_scalar);

         if (new_value != NULL)
            {
            DB->LFE_scalar = loudness_to_scalar(*(F32 *) new_value);
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = scalar_to_loudness(DA->LFE_scalar);
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Low-frequency effects (LFE) volume, based on a relative-loudness scale";
         return MARS_OK;
         }

      case ENUM_SOUND_F_LFE_DB:
         {
         if (before_value != NULL) *(F32 *) before_value = scalar_to_dB(DB->LFE_scalar);

         if (new_value != NULL)
            {
            DB->LFE_scalar = dB_to_scalar(*(F32 *) new_value);
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = scalar_to_dB(DA->LFE_scalar);
         if (min_limit != NULL)   *(F32 *) min_limit = -96.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 0.0F;
         if (description != NULL) *(C8 **) description = "Low-frequency effects (LFE) volume (dB relative to full scale)";
         return MARS_OK;
         }

      case ENUM_SOUND_F_LFE_SCALE:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->LFE_scalar;

         if (new_value != NULL)
            {
            DB->LFE_scalar = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->LFE_scalar;
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Low-frequency effects (LFE) volume, expressed as a linear DAC-voltage scale factor";
         return MARS_OK;
         }

      case ENUM_SOUND_F_WET_VOL:
         {
         if (before_value != NULL) *(F32 *) before_value = scalar_to_loudness(DB->wet_scalar);

         if (new_value != NULL)
            {
            DB->wet_scalar = loudness_to_scalar(*(F32 *) new_value);
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = scalar_to_loudness(DA->wet_scalar);
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Sound-specific wet level for global reverb mix, based on a relative-loudness scale";
         return MARS_OK;
         }

      case ENUM_SOUND_F_WET_DB:
         {
         if (before_value != NULL) *(F32 *) before_value = scalar_to_dB(DB->wet_scalar);

         if (new_value != NULL)
            {
            DB->wet_scalar = dB_to_scalar(*(F32 *) new_value);
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = scalar_to_dB(DA->wet_scalar);
         if (min_limit != NULL)   *(F32 *) min_limit = -96.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 0.0F;
         if (description != NULL) *(C8 **) description = "Sound-specific wet level for global reverb mix (dB relative to full scale)";
         return MARS_OK;
         }

      case ENUM_SOUND_F_WET_SCALE:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->wet_scalar;

         if (new_value != NULL)
            {
            DB->wet_scalar = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->wet_scalar;
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Sound-specific wet level for global reverb, expressed as a linear DAC-voltage scale factor";
         return MARS_OK;
         }

      case ENUM_SOUND_F_DRY_VOL:
         {
         if (before_value != NULL) *(F32 *) before_value = scalar_to_loudness(DB->dry_scalar);

         if (new_value != NULL)
            {
            DB->dry_scalar = loudness_to_scalar(*(F32 *) new_value);
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = scalar_to_loudness(DA->dry_scalar);
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Sound-specific dry level for global reverb mix, based on a relative-loudness scale";
         return MARS_OK;
         }
            
      case ENUM_SOUND_F_DRY_DB:
         {
         if (before_value != NULL) *(F32 *) before_value = scalar_to_dB(DB->dry_scalar);

         if (new_value != NULL)
            {
            DB->dry_scalar = dB_to_scalar(*(F32 *) new_value);
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = scalar_to_dB(DA->dry_scalar);
         if (min_limit != NULL)   *(F32 *) min_limit = -96.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 0.0F;
         if (description != NULL) *(C8 **) description = "Sound-specific dry level for global reverb mix (dB relative to full scale)";
         return MARS_OK;
         }

      case ENUM_SOUND_F_DRY_SCALE:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->dry_scalar;

         if (new_value != NULL)
            {
            DB->dry_scalar = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->dry_scalar;
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Sound-specific dry level for global reverb, expressed as a linear DAC-voltage scale factor";
         return MARS_OK;
         }

      case ENUM_SOUND_F_LPF_CUTOFF:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->LPF_cutoff;

         if (new_value != NULL)
            {
            DB->LPF_cutoff = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->LPF_cutoff;
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Cutoff frequency for the built-in low-pass filter, expressed as a fraction of the sound's Nyquist frequency.  A value of 1.0 disables the internal low-pass filter.  If occlusion is >= 0.0, the LPF cutoff parameter will have no effect";
         return MARS_OK;
         }

      case ENUM_SOUND_L_INITIAL_OFFSET_BYTES:
         {
         if (before_value != NULL) *(S64 *) before_value = DB->initial_play_cursor_bytes;

         if (new_value != NULL)
            {
            DB->initial_play_cursor_bytes = *(S64 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(S64 *) after_value = DA->initial_play_cursor_bytes;
         if (min_limit != NULL)   *(S64 *) min_limit = 0;
         if (max_limit != NULL)   *(S64 *) max_limit = D->duration_bytes;
         if (description != NULL) *(C8 **) description = "The desired default play-cursor position for this sound.  All newly-triggered instances of the sound will begin playing at this byte offset";
         return MARS_OK;
         }

      case ENUM_SOUND_L_PLAYBACK_OFFSET_BYTES:
         {
         if (before_value != NULL)
            {
            S64 pos = 0;
            if (S != NULL) pos = AIL_sample_position(S);
            *(S64 *) before_value = pos;
            }

         if (new_value != NULL)
            {
            DB->play_cursor_bytes = *(S64 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL)
            {
            S64 pos = 0;
            if (S != NULL) pos = AIL_sample_position(S);
            *(S64 *) after_value = pos;
            }

         if (min_limit != NULL)   *(S64 *) min_limit = 0;
         if (max_limit != NULL)   *(S64 *) max_limit = D->duration_bytes;
         if (description != NULL) *(C8 **) description = "The current play-cursor position in bytes for this sound.  This value applies only to already-playing sounds.  It can be read or modified at any time after a sound has been triggered";
         return MARS_OK;
         }

      case ENUM_SOUND_F_PLAYBACK_POSITION_MS:
         {
         if (before_value != NULL)
            {
            S32 pos = 0;
            if (S != NULL) AIL_sample_ms_position(S, NULL, &pos);
            *(F32 *) before_value = (F32) pos;
            }

         if (new_value != NULL)
            {
            DB->play_cursor_ms = (*(F32 *) new_value) + 0.5F;
            SND_update_properties(D);
            }

         if (after_value != NULL)
            {
            S32 pos = 0;
            if (S != NULL) AIL_sample_ms_position(S, NULL, &pos);
            *(F32 *) after_value = (F32) pos;
            }

         if (min_limit != NULL)   *(F32 *) min_limit = 0;
         if (max_limit != NULL)   *(F32 *) max_limit = D->duration_ms;
         if (description != NULL) *(C8 **) description = "The current play-cursor position in milliseconds for this sound.  This value applies only to already-playing sounds.  It can be read or modified at any time after a sound has been triggered"; 
         return MARS_OK;
         }

      case ENUM_SOUND_L_TOTAL_LEN_BYTES:
         {
         if (before_value != NULL) *(S64 *) before_value = D->duration_bytes;
         if (after_value != NULL)  *(S64 *) after_value  = D->duration_bytes;
         if (description != NULL)  *(C8 **) description = "The total length of the sound's audio data, excluding headers and metadata, in bytes (Read-only)";
         return MARS_OK;
         }

      case ENUM_SOUND_F_DURATION_MS:
         {
         if (before_value != NULL) *(F32 *) before_value = D->duration_ms;
         if (after_value != NULL)  *(F32 *) after_value  = D->duration_ms;
         if (description != NULL)  *(C8 **) description = "The total length of the sound's audio data, excluding headers and metadata, in milliseconds (Read-only)";
         return MARS_OK;
         }

      case ENUM_SOUND_N_INITIAL_LOOP_COUNT:
         {
         if (before_value != NULL) *(S32 *) before_value = DB->initial_loop_count;

         if (new_value != NULL)
            {
            DB->initial_loop_count = *(S32 *) new_value; // (takes effect only when starting a new sound)
            }

         if (after_value != NULL) *(S32 *) after_value  = DA->initial_loop_count;
         if (min_limit != NULL)   *(S32 *) min_limit = 0;
         if (max_limit != NULL)   *(S32 *) max_limit = 99999;
         if (description != NULL) *(C8 **) description = "The desired default loop count for this sound (0=indefinite, 1=single-shot).  All newly-triggered instances of the sound will loop this many times";
         return MARS_OK;
         }

      case ENUM_SOUND_N_REMAINING_LOOP_COUNT:
         {
         if (before_value != NULL)
            {
            S32 pos = 0;
            if (S != NULL) pos = AIL_sample_loop_count(S);
            *(S32 *) before_value = pos;
            }

         if (new_value != NULL)
            {
            DB->loop_count = *(S32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL)
            {
            S32 pos = 0;
            if (S != NULL) pos = AIL_sample_loop_count(S);
            *(S32 *) after_value = pos;
            }

         if (min_limit != NULL)   *(S32 *) min_limit = 0;
         if (max_limit != NULL)   *(S32 *) max_limit = 99999;
         if (description != NULL) *(C8 **) description = "The number of loop iterations remaining to be played (0=indefinite).  This value applies only to already-playing sounds.  It can be read or modified at any time after a sound has been triggered";
         return MARS_OK;
         }

      case ENUM_SOUND_L_LOOP_START_BYTES:
         {
         if (before_value != NULL) *(S64 *) before_value = DB->loop_start_bytes;

         if (new_value != NULL)
            {
            DB->loop_start_bytes = *(S64 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL)  *(S64 *) after_value  = DA->loop_start_bytes;
         if (min_limit != NULL)   *(S64 *) min_limit = -1;
         if (max_limit != NULL)   *(S64 *) max_limit = D->duration_bytes;
         if (description != NULL) *(C8 **) description = "The play-cursor position in bytes at the start of the sound's looped (sustain) portion, or -1 if no specific loop block is desired";
         return MARS_OK;
         }

      case ENUM_SOUND_L_LOOP_END_BYTES:
         {
         if (before_value != NULL) *(S64 *) before_value = DB->loop_end_bytes;

         if (new_value != NULL)
            {
            DB->loop_end_bytes = *(S64 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL)  *(S64 *) after_value  = DA->loop_end_bytes;
         if (min_limit != NULL)   *(S64 *) min_limit = -1;
         if (max_limit != NULL)   *(S64 *) max_limit = D->duration_bytes;
         if (description != NULL) *(C8 **) description = "The play-cursor position in bytes at the end of the sound's looped (sustain) portion, or -1 if no specific loop block is desired";
         return MARS_OK;
         }

      case ENUM_SOUND_CB_EOS:
         {
         if (before_value != NULL) *(MARS_SOUND_CALLBACK *) before_value = DB->EOS_CB;

         if (new_value != NULL)
            {
            DB->EOS_CB = *(MARS_SOUND_CALLBACK *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL)  *(MARS_SOUND_CALLBACK *) after_value  = DA->EOS_CB;
         return MARS_OK;
         }

      case ENUM_SOUND_SINTa_USER:
         {
         if (before_value != NULL) *(SINTa *) before_value = D->user;
         if (new_value != NULL)    D->user = *(SINTa *) new_value;
         if (after_value != NULL)  *(SINTa *) after_value  = D->user;
         if (min_limit != NULL)   *(SINTa *) min_limit = 0;
         if (max_limit != NULL)   *(SINTa *) max_limit = 0xffffffff;
         if (description != NULL) *(C8 **) description = "A general user-data field that can be used for any desired purpose.  It is guaranteed to be large enough to contain a pointer on all supported platforms";
         return MARS_OK;
         }

      case ENUM_SOUND_F_OCCLUSION:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->occlusion;

         if (new_value != NULL)
            {
            DB->occlusion = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->occlusion;
         if (min_limit != NULL)   *(F32 *) min_limit = -1.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "An inverse low-pass function that simulates substantial obstacles to wave propagation (0.0=unoccluded, 1.0=fully-occluded).  Note that the built-in LPF is usable only if occlusion is < 0.0 (disabled)";
         return MARS_OK;
         }

      case ENUM_SOUND_F_OBSTRUCTION:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->obstruction;

         if (new_value != NULL)
            {
            DB->obstruction = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->obstruction;
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Acts as an additional attenuation factor in the global reverb's dry path, in order to simulate a blocked or obstructed direct path (0.0=unobstructed, 1.0=dry path 100% obstructed)";
         return MARS_OK;
         }

      case ENUM_SOUND_F_EXCLUSION:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->exclusion;

         if (new_value != NULL)
            {
            DB->exclusion = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->exclusion;
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Exclusion is the opposite of obstruction, acting as an attenuator in the global reverb's wet path.  (0.0=wet path unexcluded, 1.0=completely excluded)";
         return MARS_OK;
         }

      case ENUM_SOUND_B_IS_3D:
         {
         if (before_value != NULL) *(S32 *) before_value = DB->is_3D;

         if (new_value != NULL)
            {
            DB->is_3D = *(S32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(S32 *) after_value = DA->is_3D;
         if (min_limit != NULL)   *(S32 *) min_limit = 0;
         if (max_limit != NULL)   *(S32 *) max_limit = 1;
         if (description != NULL) *(C8 **) description = "If nonzero, this sound's output-channel levels will be determined by its listener-relative position and other 3D-related attributes.  Changes to this value affect only subsequently-triggered sound instances";
         return MARS_OK;
         }

      case ENUM_SOUND_F_3D_X_POS:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->position.x;

         if (new_value != NULL)
            {
            DB->position.x = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->position.x;
         if (min_limit != NULL)   *(F32 *) min_limit = -500.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 500.0F;
         if (description != NULL) *(C8 **) description = "Sound X position";
         return MARS_OK;
         }

      case ENUM_SOUND_F_3D_X_FACE:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->face.x;

         if (new_value != NULL)
            {
            DB->face.x = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->face.x;
         if (min_limit != NULL)   *(F32 *) min_limit = -1.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Sound X facing direction";
         return MARS_OK;
         }

      case ENUM_SOUND_F_3D_X_UP:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->up.x;

         if (new_value != NULL)
            {
            DB->up.x = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->up.x;
         if (min_limit != NULL)   *(F32 *) min_limit = -1.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Sound X up vector";
         return MARS_OK;
         }

      case ENUM_SOUND_F_3D_X_VEL:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->velocity.x;

         if (new_value != NULL)
            {
            DB->velocity.x = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->velocity.x;
         if (min_limit != NULL)   *(F32 *) min_limit = -1.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Sound X velocity";
         return MARS_OK;
         }

      case ENUM_SOUND_F_3D_Y_POS:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->position.y;

         if (new_value != NULL)
            {
            DB->position.y = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->position.y;
         if (min_limit != NULL)   *(F32 *) min_limit = -500.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 500.0F;
         if (description != NULL) *(C8 **) description = "Sound Y position";
         return MARS_OK;
         }

      case ENUM_SOUND_F_3D_Y_FACE:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->face.y;

         if (new_value != NULL)
            {
            DB->face.y = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->face.y;
         if (min_limit != NULL)   *(F32 *) min_limit = -1.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Sound Y facing direction";
         return MARS_OK;
         }

      case ENUM_SOUND_F_3D_Y_UP:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->up.y;

         if (new_value != NULL)
            {
            DB->up.y = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->up.y;
         if (min_limit != NULL)   *(F32 *) min_limit = -1.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Sound Y up vector";
         return MARS_OK;
         }

      case ENUM_SOUND_F_3D_Y_VEL:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->velocity.y;

         if (new_value != NULL)
            {
            DB->velocity.y = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->velocity.y;
         if (min_limit != NULL)   *(F32 *) min_limit = -1.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Sound Y velocity";
         return MARS_OK;
         }

      case ENUM_SOUND_F_3D_Z_POS:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->position.z;

         if (new_value != NULL)
            {
            DB->position.z = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->position.z;
         if (min_limit != NULL)   *(F32 *) min_limit = -500.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 500.0F;
         if (description != NULL) *(C8 **) description = "Sound Z position";
         return MARS_OK;
         }

      case ENUM_SOUND_F_3D_Z_FACE:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->face.z;

         if (new_value != NULL)
            {
            DB->face.z = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->face.z;
         if (min_limit != NULL)   *(F32 *) min_limit = -1.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Sound Z facing direction";
         return MARS_OK;
         }

      case ENUM_SOUND_F_3D_Z_UP:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->up.z;

         if (new_value != NULL)
            {
            DB->up.z = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->up.z;
         if (min_limit != NULL)   *(F32 *) min_limit = -1.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Sound Z up vector";
         return MARS_OK;
         }

      case ENUM_SOUND_F_3D_Z_VEL:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->velocity.z;

         if (new_value != NULL)
            {
            DB->velocity.z = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->velocity.z;
         if (min_limit != NULL)   *(F32 *) min_limit = -1.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Sound Z velocity";
         return MARS_OK;
         }

      case ENUM_SOUND_V_3D_POS:
         {
         if (before_value != NULL) *(MSSVECTOR3D *) before_value = DB->position;

         if (new_value != NULL)
            {
            DB->position = *(MSSVECTOR3D *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(MSSVECTOR3D *) after_value = DA->position;
         if (description != NULL) *(C8 **) description = "Sound 3D position vector";
         return MARS_OK;
         }

      case ENUM_SOUND_V_3D_FACE:
         {
         if (before_value != NULL) *(MSSVECTOR3D *) before_value = DB->face;

         if (new_value != NULL)
            {
            DB->face = *(MSSVECTOR3D *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(MSSVECTOR3D *) after_value = DA->face;
         if (description != NULL) *(C8 **) description = "Sound 3D facing vector";
         return MARS_OK;
         }

      case ENUM_SOUND_V_3D_UP:
         {
         if (before_value != NULL) *(MSSVECTOR3D *) before_value = DB->up;

         if (new_value != NULL)
            {
            DB->up = *(MSSVECTOR3D *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(MSSVECTOR3D *) after_value = DA->up;
         if (description != NULL) *(C8 **) description = "Sound 3D up vector";
         return MARS_OK;
         }

      case ENUM_SOUND_V_3D_VEL:
         {
         if (before_value != NULL) *(MSSVECTOR3D *) before_value = DB->velocity;

         if (new_value != NULL)
            {
            DB->velocity = *(MSSVECTOR3D *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(MSSVECTOR3D *) after_value = DA->velocity;
         if (description != NULL) *(C8 **) description = "Sound 3D velocity (direction)";
         return MARS_OK;
         }

      case ENUM_SOUND_F_3D_MAG:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->velocity_mag;

         if (new_value != NULL)
            {
            DB->velocity_mag = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->velocity_mag;
         if (description != NULL) *(C8 **) description = "Sound 3D velocity (magnitude)";
         return MARS_OK;
         }

      case ENUM_SOUND_F_3D_CONE_INNER_DEGS:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->inner_angle;

         if (new_value != NULL)
            {
            DB->inner_angle = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->inner_angle;
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 360.0F;
         if (description != NULL) *(C8 **) description = "Inner angle in degrees of 3D sound-emission cone.  Within this angle, no cone attenuation takes place.  Source-listener bearings between the inner and outer angle result in linear volume falloff.  By default both angles are 360 degrees, disabling cone attenuation";
         return MARS_OK;
         }

      case ENUM_SOUND_F_3D_CONE_INNER_RADS:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->inner_angle * DEG2RADF;

         if (new_value != NULL)
            {
            DB->inner_angle = *(F32 *) new_value * RAD2DEGF;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->inner_angle * DEG2RADF;
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 2.0F*3.1415926535F;
         if (description != NULL) *(C8 **) description = "Inner angle in radians of 3D sound-emission cone.  Within this angle, no cone attenuation takes place.  Source-listener bearings between the inner and outer angle result in linear volume falloff.  By default both angles are 2*pi radians, disabling cone attenuation";
         return MARS_OK;
         }

      case ENUM_SOUND_F_3D_CONE_OUTER_DEGS:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->outer_angle;

         if (new_value != NULL)
            {
            DB->outer_angle = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->outer_angle;
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 360.0F;
         if (description != NULL) *(C8 **) description = "Outer angle in degrees of 3D sound-emission cone.  When the listener is outside this angle, the cone's outer volume property determines the attenuation.  By default both inner and outer angles are 360 degrees, disabling cone attenuation";
         return MARS_OK;
         }

      case ENUM_SOUND_F_3D_CONE_OUTER_RADS:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->outer_angle * DEG2RADF;

         if (new_value != NULL)
            {
            DB->outer_angle = *(F32 *) new_value * RAD2DEGF;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->outer_angle * DEG2RADF;
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 2.0F*3.1415926535F;
         if (description != NULL) *(C8 **) description = "Outer angle in radians of 3D sound-emission cone.  When the listener is outside this angle, the cone's outer volume property determines the attenuation.  By default both inner and outer angles are 2*pi radians, disabling cone attenuation";
         return MARS_OK;
         }

      case ENUM_SOUND_F_3D_CONE_OUTER_VOL:
         {
         if (before_value != NULL) *(F32 *) before_value = scalar_to_loudness(DB->outer_scalar);

         if (new_value != NULL)
            {
            DB->outer_scalar = loudness_to_scalar(*(F32 *) new_value);
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = scalar_to_loudness(DA->outer_scalar);
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Volume level effective beyond the 3D cone's outer angle, based on a relative-loudness scale";
         return MARS_OK;
         }

      case ENUM_SOUND_F_3D_CONE_OUTER_DB:
         {
         if (before_value != NULL) *(F32 *) before_value = scalar_to_dB(DB->outer_scalar);

         if (new_value != NULL)
            {
            DB->outer_scalar = dB_to_scalar(*(F32 *) new_value);
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = scalar_to_dB(DA->outer_scalar);
         if (min_limit != NULL)   *(F32 *) min_limit = -96.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 0.0F;
         if (description != NULL) *(C8 **) description = "Volume level effective beyond the 3D cone's outer angle (dB relative to full scale)";
         return MARS_OK;
         }

      case ENUM_SOUND_F_3D_CONE_OUTER_SCALE:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->outer_scalar;

         if (new_value != NULL)
            {
            DB->outer_scalar = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->outer_scalar;
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1.0F;
         if (description != NULL) *(C8 **) description = "Volume level effective beyond the 3D cone's outer angle, expressed as a linear DAC-voltage scale factor";
         return MARS_OK;
         }

      case ENUM_SOUND_F_3D_MIN_DIST:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->min_dist;

         if (new_value != NULL)
            {
            DB->min_dist = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->min_dist;
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1000.0F;
         if (description != NULL) *(C8 **) description = "When the listener is within the sound's minimum-distance zone, no 3D distance attenuation is applied to the sound";
         return MARS_OK;
         }

      case ENUM_SOUND_F_3D_MAX_DIST:
         {
         if (before_value != NULL) *(F32 *) before_value = DB->max_dist;

         if (new_value != NULL)
            {
            DB->max_dist = *(F32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL) *(F32 *) after_value = DA->max_dist;
         if (min_limit != NULL)   *(F32 *) min_limit = 0.0F;
         if (max_limit != NULL)   *(F32 *) max_limit = 1000.0F;
         if (description != NULL) *(C8 **) description = "When the listener is outside a given sound's maximum-distance boundary, the sound will be muted automatically by the system";
         return MARS_OK;
         }

      case ENUM_SOUND_CB_3D_FALLOFF:
         {
         if (before_value != NULL) *(MARS_FALLOFF_CALLBACK *) before_value = DB->falloff_CB;

         if (new_value != NULL)
            {
            DB->falloff_CB = *(MARS_FALLOFF_CALLBACK *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL)  *(MARS_FALLOFF_CALLBACK *) after_value  = DA->falloff_CB;
         return MARS_OK;
         }

      case ENUM_SOUND_B_3D_AUTO_WET_ATTEN:
         {
         if (before_value != NULL) *(S32 *) before_value = DB->auto_3D_atten;

         if (new_value != NULL)
            {
            DB->auto_3D_atten = *(S32 *) new_value;
            SND_update_properties(D);
            }

         if (after_value != NULL)  *(S32 *) after_value = DA->auto_3D_atten;
         if (min_limit != NULL)   *(S32 *) min_limit = 0;
         if (max_limit != NULL)   *(S32 *) max_limit = 1;
         if (description != NULL) *(C8 **) description = "If this value is nonzero for a 3D-positioned sound, its wet-path contribution to the global reverb will be attenuated with increasing distance from the listener.  Otherwise, the wet path is not subject to distance-based attenuation";
         return MARS_OK;
         }

      case ENUM_SOUND_L_UNDERLYING_HSAMPLE:
         {
         if (S == NULL) 
            {
            if (before_value != NULL) *(SINTa *) before_value = (SINTa) 0;
            if (after_value != NULL)  *(SINTa *) after_value  = (SINTa) 0;
            }
         else
            {
            if (before_value != NULL) *(SINTa *) before_value = (SINTa) S;
            if (after_value != NULL)  *(SINTa *) after_value  = (SINTa) S;
            }
         return MARS_OK;
         }
      }

   return MARS_OK;
}

MARS_RESULT MARS_sound_property_text(HMSOUND       D,
                                     MARS_PROPERTY name,
                                     C8 *          before_value,
                                     C8 const *    new_value,
                                     C8 *          after_value,
                                     void *        min_limit,
                                     void *        max_limit,
                                     C8 **         description)

{
   if (D == NULL)
      {
      return MARS_BAD_PARAM;
      }

   //
   // Get property index from string (TODO: hash)
   // (Also redundant with the actual property-set call below)
   //

   MARS_PROPERTY_INFO *info;

   S32 index = SNDSTATE_property(name,
                                &info);
   if (index == -1)
      {
      return MARS_BAD_PROPERTY;
      }

   C8 new_bin[MARS_MAX_PROP_BIN_LEN];

   if (new_value != NULL)
      {
      if (!text_to_property(info->type,
                   (C8 **) &new_value,
                           &new_bin))
         {
         return MARS_BAD_PROPERTY;
         }
      }

   C8 before_bin[MARS_MAX_PROP_BIN_LEN];
   C8 after_bin [MARS_MAX_PROP_BIN_LEN];

   MARS_RESULT result = MARS_sound_property(D,
                                            info->name,
                                            (before_value == NULL) ? NULL : before_bin,
                                            (new_value == NULL)    ? NULL : new_bin,
                                            (after_value == NULL)  ? NULL : after_bin,
                                            min_limit,
                                            max_limit,
                                            description);
   if (result != MARS_OK)
      {
      return result;
      }

   if (before_value != NULL)
      {
      if (!property_to_text(info->type,
                            before_bin,
                            before_value))
         {
         return MARS_BAD_PROPERTY;
         }
      }

   if (after_value != NULL)
      {
      if (!property_to_text(info->type,
                            after_bin,
                            after_value))
         {
         return MARS_BAD_PROPERTY;
         }
      }

   return MARS_OK;      
}

MARS_RESULT MARS_sound_channel_level(HMSOUND      sound,
                                     MARS_SPEAKER channel,
                                     F32         *before_value,
                                     F32 const   *new_value)
{
   return MARS_OK;
}

MARS_RESULT MARS_sound_channel_scale(HMSOUND      sound,
                                     MARS_SPEAKER channel,
                                     F32         *before_value,
                                     F32 const   *new_value)
{
   return MARS_OK;
}

MARS_RESULT MARS_sound_channel_dB  (HMSOUND      sound,
                                    MARS_SPEAKER channel,
                                    F32         *before_value,
                                    F32 const   *new_value)
{
   return MARS_OK;
}

MARS_RESULT MARS_defer_sound_settings (HMSOUND sound)
{
   return MARS_OK;
}

MARS_RESULT MARS_commit_sound_settings(HMSOUND sound)
{
   return MARS_OK;
}

void _________________________________________________Filters________________________________________________________(){}

MARS_RESULT MARS_enumerate_filters (HMENV      environment,
                                    HPROENUM  *next,
                                    C8       **filter_name)
{
   return MARS_OK;
}

MARS_RESULT MARS_install_sound_filter(HMSOUND           sound,
                                      MARS_SAMPLE_STAGE stage,
                                      C8               *filter_name)
{
   return MARS_OK;
}

MARS_RESULT MARS_sound_filter        (HMSOUND           sound,
                                      MARS_SAMPLE_STAGE stage,
                                      C8              **filter_name)
{
   return MARS_OK;
}

MARS_RESULT MARS_enumerate_filter_properties(HMENV                environment,
                                             C8                  *filter_name,
                                             HMARSENUM           *next,
                                             RIB_INTERFACE_ENTRY *dest)
{
   return MARS_OK;
}

MARS_RESULT MARS_sound_filter_property(HMSOUND           sound,
                                       MARS_SAMPLE_STAGE stage,
                                       C8               *property_name,
                                       void *            before_value,
                                       void const *      new_value,
                                       void *            after_value)
{
   return MARS_OK;
}


void _________________________________________________Transport_controls_______________________________________________(){}

MARS_RESULT MARS_play(HMSOUND D)
{
   if (D == NULL)
      {
      return MARS_BAD_PARAM;
      }

   if (!D->input_properties.filename[0])
      {
      return MARS_FILE_NOT_FOUND;
      }

   //
   // Load the data and create a new SNDINST, if not already done
   //

   MARS_RESULT result = MARS_preload(D, TRUE);

   if (result != MARS_OK)
      {
      return result;
      }

   //
   // Start any untriggered instances of this sound definition that have a
   // valid HSAMPLE
   //
   // Undocumented hack: you can set the initial play cursor to -1 to
   // keep the result of a previous time-based seek
   //

   SNDINST *I = D->first_inst;

   while (I != NULL)
      {
      if (!I->triggered)
         {
         I->triggered = TRUE;

         if (D->input_properties.initial_play_cursor_bytes >= 0)
            {
            AIL_set_sample_position(I->S, (S32) D->input_properties.initial_play_cursor_bytes);
            }

         AIL_set_sample_loop_count(I->S, D->input_properties.initial_loop_count);
         AIL_resume_sample(I->S);
         }

      I = I->next_inst;
      }

   return MARS_OK;
}

MARS_RESULT MARS_pause(HMSOUND D)
{
   //
   // Pause any playing samples/streams of this type
   //

   SNDINST *I = D->first_inst;

   while (I != NULL)
      {
      if (I->has_voice)
         {
         AIL_stop_sample(I->S);
         }

      I = I->next_inst;
      }

   return MARS_OK;
}

MARS_RESULT MARS_resume(HMSOUND D)
{
   //
   // Restart any paused samples/streams of this type
   //

   SNDINST *I = D->first_inst;

   while (I != NULL)
      {
      if (I->has_voice && (AIL_sample_status(I->S) == SMP_STOPPED))
         {
         AIL_resume_sample (I->S);
         }

      I = I->next_inst;
      }

   return MARS_OK;
}

MARS_RESULT MARS_end(HMSOUND D)
{
   //
   // Free any assigned physical and virtual voices
   //

   while (D->first_inst != NULL)
      {
      release_voice(D->first_inst, TRUE);
      free_SNDINST (D->first_inst);
      }

   return MARS_OK;
}

MARS_RESULT MARS_serve_stream(HMSOUND sound)
{
   return MARS_OK;
}

void _________________________________________________Tags____________________________________________________________(){}

MARS_RESULT MARS_add_tag   (HMENV  environment,
                            C8    *tagset)
{
   return MARS_OK;
}

MARS_RESULT MARS_remove_tag(HMENV  environment,
                            C8    *tagset)
{
   return MARS_OK;
}

MARS_RESULT MARS_add_sound_tag(HMSOUND sound,
                               C8     *tagset)
{
   return MARS_OK;
}

MARS_RESULT MARS_remove_sound_tag(HMSOUND sound,
                                  C8     *tagset)
{
   return MARS_OK;
}

MARS_RESULT MARS_enumerate_tags(HMENV      environment,
                                HMSOUND    sound,
                                HMARSENUM *next,
                                C8        *tag)
{
   return MARS_OK;
}

S32        MARS_sound_has_tag(HMSOUND sound,
                              C8     *tagset)
{
   return FALSE;
}

MARS_RESULT MARS_set_tag_script(HMENV environment,
                                C8   *tagset,
                                C8   *script)
{
   return MARS_OK;
}
