#ifndef AIL_SOUND_ACTIVE
#define AIL_SOUND_ACTIVE
#include "../dosbox_files/config.h"

#define MSS_STRUCT struct

typedef MSS_STRUCT                     // VDI interface register structure
{
	Bit16s AX;
	Bit16s BX;
	Bit16s CX;
	Bit16s DX;
	Bit16s SI;
	Bit16s DI;
}
VDI_CALL;

typedef void (* AILTIMERCB)    (Bit32u user);

typedef Bit32s HTIMER;                             // Handle to timer

#define REALFAR unsigned int


typedef MSS_STRUCT                      // I/O parameters structure
{
	/*0*/Bit16s IO;
	/*2*/Bit16s IRQ;
	/*4*/Bit16s DMA_8_bit;
	/*6*/Bit16s DMA_16_bit;
	/*8*/Bit32s IO_reserved[4];
	//24
}
IO_PARMS;

typedef MSS_STRUCT                      // Standard MSS 3.X VDI driver header
{
	/*0*/Bit8s ID[8];
	/*8*/Bit16u driver_version;
	/*10*/Bit16u VDI_HDR_var10;
	/*12*/Bit16u VDI_HDR_var12;
	/*14*/Bit16u num_IO_configurations;//03
	/*16*/void* environment_string;//2c380506	
	//20
	/*22*/IO_PARMS IO;//22
	/*20*/Bit16u scratch0;
	/*46*/Bit16s VDI_HDR_var46;//busy
	/*48*/Bit16u VDI_HDR_var48;//driver_num
	/*50*/Bit16u VDI_HDR_var50;
	/*52*/Bit16u VDI_HDR_var52_this_ISR;
	/*54*/Bit16u VDI_HDR_var54_prev_ISR;
   /*58*/Bit8s scratch[122];           // Shared scratch workspace
   
   Bit8s     dev_name[80];           // Device name (VDI version >= 1.12 only)
}
VDI_HDR;

typedef MSS_STRUCT                      // Standard MSS 3.X VDI driver header
{
	/*0*/Bit8s     ID[6];                  // "AIL3xxx" ID string, followed by ^Z

	/*6*/Bit32u    driver_version;

   /*10*/REALFAR  common_IO_configurations;
   /*14*/Bit16u    num_IO_configurations;

   /*16*/REALFAR  environment_stringx;//not use this is 16bit adress

   /*20*/IO_PARMS IO;

   /*44*/Bit16s     service_rate;

   /*46*/Bit16u    busy;

   /*48*/Bit16u    driver_num;            // Driver number

   /*50*/Bit16u    this_ISR;               // Offset of INT 66H dispatcher
   ///*52*/REALFAR  prev_ISR;               // Pointer to previous INT 66H ISR
   /*52*/Bit16u    prev_ISR;
   /*54*/Bit8u*    VDI_HDR_var54;
   /*56*/Bit8s     scratch[124];           // Shared scratch workspace

   Bit8s     dev_name[80];           // Device name (VDI version >= 1.12 only)
}
VDI_HDRorig;

/*typedef MSS_STRUCT                   // Handle to driver
{
   Bit8u allbuffer[4096];
}
_DIG_DRIVER;*/

typedef void* xHDIGDRIVER;

typedef MSS_STRUCT                   // Handle to driver
{
	/*0*/REALFAR AIL_DRIVER_var0_seg;//remove this! segment must be zero!
/*4*/Bit32u AIL_DRIVER_var1_sel;
/*8*/Bit8u* AIL_DRIVER_var2_buf;
/*12*/Bit32u AIL_DRIVER_var3_size;
/*16*/VDI_HDR* AIL_DRIVER_var4_VHDR;
/*20*/Bit32s AIL_DRIVER_var5;
/*24*/Bit32s AIL_DRIVER_var6;
/*28*/Bit32s AIL_DRIVER_var7;
/*32*/Bit32s AIL_DRIVER_var8;
/*36*/void(*AIL_DRIVER_var9)(xHDIGDRIVER);
/*40*/void* AIL_DRIVER_var10;
/*44*/
}
AIL_DRIVER;

typedef int(*EOS_callback_type)(void);

typedef MSS_STRUCT _SAMPLE           // Sample instance
{
	//char tag[4];
	 //16 - volume

   Bit32s sam_var[1000];
xHDIGDRIVER sam_var0;
Bit8u* sam_var2_sample_address_start = 0;
Bit32u sam_var3_sample_address_start2 = 0;
Bit32u sam_var4_sample_address_len = 0;
Bit32u sam_var5_sample_address_len2 = 0;
Bit32s sam_var12_loop_count = 0;
Bit32s sam_var13_format = 0;
Bit32u sam_var14_flags = 0;
Bit32s sam_var15_playback_rate = 0;
Bit32s sam_var16_volume = 0;
Bit32s sam_var17_volume_pan = 0;
Bit16s sam_varw18 = 0;
Bit8s sam_varb20 = 0;
Bit8s sam_varb21 = 0;
EOS_callback_type sam_var532_EOS_ptr;

   Bit8u* sam_var542;
   /*Bit32s sam_var2;
   Bit32s sam_var3;
   Bit32s sam_var4;
   Bit32s sam_var5;
   Bit32s sam_var6;
   Bit32s sam_var7;
   Bit32s sam_var8;
   Bit32s sam_var9;
   Bit32s sam_var10;
   Bit32s sam_var11;
   Bit32s sam_var12;
   Bit32s sam_var13;
   Bit32s sam_var14;
   Bit32s sam_var15;
   Bit32s sam_var16;
   Bit32s sam_var17;
   Bit32s sam_var18;
   Bit32s sam_var19;
   Bit32s sam_var20;
   Bit32s sam_var21;
   Bit32s sam_var22;
   Bit32s sam_var23;
   Bit32s sam_var24;
   Bit32s sam_var25;
   Bit32s sam_var26;
   Bit32s sam_var27;
   Bit32s sam_var28;
   Bit32s sam_var29;
   Bit32s sam_var274;
   Bit32s sam_var530;
   Bit32s sam_var531;
   Bit32s sam_var532;
	char scratch[400];*/
	/*char       tag[4];            // HSAM

	HDIGDRIVER driver;            // Driver for playback

	S32        index;             // Numeric index of this sample

	void const FAR *start[2];     // Sample buffer address (W)
	U32       len[2];           // Sample buffer size in bytes (W)
	U32       pos[2];           // Index to next byte (R/W)
	U32       done[2];           // Nonzero if buffer with len=0 sent by app
	S32       reset_ASI[2];      // Reset the ASI decoder at the end of the buffer
	S32       reset_seek_pos[2];  // New destination offset in stream source data, for ASI codecs that care
	S32       exhaust_ASI;        // Are we prolonging the buffer lifetime until ASI output is exhausted?

	U32      src_fract;           // Fractional part of source address
	S32      left_val;            // Mixer source value from end of last buffer
	S32      right_val;           // Mixer source value from end of last buffer

	S32      current_buffer;      // Buffer # active (0/1)
	S32      last_buffer;         // Last active buffer (for double-buffering)
	S32      starved;             // Buffer stream has run out of data

	S32      loop_count;          // # of cycles-1 (1=one-shot, 0=indefinite)
	S32      loop_start;          // Starting offset of loop block (0=SOF)
	S32      loop_end;            // End offset of loop block (-1=EOF)
	S32      orig_loop_count;     // Original loop properties specified by app, before any
	S32      orig_loop_start;     // alignment constraints
	S32      orig_loop_end;

	S32      format;              // DIG_F format (8/16 bits, mono/stereo)
	U32      flags;               // DIG_PCM_SIGN / DIG_PCM_ORDER (stereo only)

	S32      playback_rate;       // Playback rate in hertz

	F32      save_volume;         // Sample volume 0-1.0
	F32      save_pan;            // Mono panpot/stereo balance (0=L ... 1.0=R)

	F32      left_volume;         // Left/mono volume 0 to 1.0
	F32      right_volume;        // Right volume 0 to 1.0

	F32      wet_level;           // reverb level 0 to 1.0
	F32      dry_level;           // non-reverb level 0 to 1.0

	F32      obstruction;
	F32      occlusion;
	F32      exclusion;

	F32      auto_3D_channel_levels[MAX_SPEAKERS]; // Channel levels set by 3D positioner (always 1.0 if not 3D-positioned)
	F32      user_channel_levels[MAX_SPEAKERS]; // Channel levels set by AIL_set_sample_channel_levels() [driver->logical_channels]
	S32      cur_scale[MAX_SPEAKERS];           // Calculated 11-bit volume scale factors for current/previous mixing interval
	S32      prev_scale[MAX_SPEAKERS];          // (These are all indexed by build buffer*2, not speaker indexes!)
	S32      ramps_left[MAX_SPEAKERS];

	LOWPASS_INFO lp;              // low pass info
	F32      cutoff_param;
	F32      calculated_cut;
	S32      service_type;        // 1 if single-buffered; 2 if streamed

	AILSAMPLECB  SOB;             // Start-of-block callback function
	AILSAMPLECB  EOB;             // End-of-buffer callback function
	AILSAMPLECB  EOS;             // End-of-sample callback function

	SINTa    user_data[8];      // Miscellaneous user data
	SINTa    system_data[8];      // Miscellaneous system data

	ADPCMDATA adpcm;

	S32      last_decomp_left;    // last sample in an asi or adpcm buffer
	S32      last_decomp_right;   // last sample in an asi or adpcm buffer

	S32      doeob;               // Flags to trigger callbacks
	S32      dosob;
	S32      doeos;

	//
	// Sample pipeline stages
	//

	SPINFO   pipeline[N_SAMPLE_STAGES];
	S32      n_active_filters;    // # of SP_FILTER_n stages active

	//
	// 3D-related state for all platforms (including Xbox)
	//

	S32      is_3D;               // TRUE if channel levels are derived automatically from 3D positional state, FALSE if they're controlled manually
	S3DSTATE S3D;                 // Software version applies 3D positioning only if is_3D == TRUE, but output filters always use it

 #ifdef MSS_VFLT_SUPPORTED
	void FAR *voice;              // Optional object used by output filter to store per-sample information such as DS3D buffers
 #endif

	//
	// Platform-specific members
	//

 #ifdef IS_XBOX
	AILLPDIRECTSOUNDBUFFER pDSB;

	S32 hw_rate;
	S32 hw_channels;
	S32 hw_bits;
	S32 direct_mode;

	STAGE_BUFFER * ds_staging_buffer;
	S32 waiting_for_which_half;
	S32 cleared_bufs;
	S32 cleared_bytes;
	S32 need_more_zeroes;
	S32 ds_stage_size;

	F32 shadow_FL;
	F32 shadow_FR;
	F32 shadow_FC;
	F32 shadow_LF;
	F32 shadow_BL;
	F32 shadow_BR;

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
	S32         shadow_auto_3D_atten;
	S32         shadow_is_3D;

 #endif

	F32 leftb_volume;         // Left/mono volume 0 to 1.0 (back)
	F32 rightb_volume;        // Right volume 0 to 1.0 (back)
	F32 center_volume;        // Center volume 0 to 1.0
	F32 low_volume;           // Low volume 0 to 1.0
	F32 save_fb_pan;          // Sample volume 0-1.0
	F32 save_center;          // saved center level
	F32 save_low;             // saved sub level

 #ifdef IS_WINDOWS

	//
	// DirectSound-specific data
	//

	S32      service_interval;    // Service sample every n ms
	S32      service_tick;        // Current service countdown value
	S32      buffer_segment_size; // Buffer segment size to fill

	S32      prev_segment;        // Previous segment # (0...n)
	S32      prev_cursor;         // Previous play cursor location

	S32      bytes_remaining;     // # of bytes left to play (if not -1)

	S32      direct_control;      // 1 if app controls buffer, 0 if MSS

 #endif*/
}
SAMPLE;
typedef MSS_STRUCT _SAMPLE * HSAMPLE;           // Handle to sample

typedef MSS_STRUCT                   // Initialization file structure
{
	/*0*/AIL_DRIVER* var0_aildrv;
/*1*/Bit8u* var1_aildrv;
/*2*/Bit8u* var2_aildrv;
 Bit32u var3_aildrv;
 /*2*/Bit32s var4_aildrv;
 Bit32u var5_aildrv;
 Bit32u var6_aildrv;
 Bit32u var7_aildrv;
 /*13*/Bit32u var8_aildrv;
 /*13*/Bit32u var9_aildrv;
 /*13*/Bit32u var10_aildrv;
  Bit32u var11_aildrv;
  Bit32u var12_aildrv;
  /*13*/Bit8u* var13_aildrv;
  /*13*/Bit32u var14_aildrv;
  /*13*/Bit32u var15_aildrv;
  /*13*/Bit32u var16_aildrv;
  /*13*/Bit32u var17_aildrv;
  /*13*/Bit32u var18_aildrv;
  /*13*/Bit32u var19_aildrv;
  /*20*/ Bit8u* var20_aildrv_dig_drv;
  /*13*/Bit32u var21_aildrv;
  /*13*/Bit32u var22_aildrv;
  /*13*/HSAMPLE var23_aildrvx;
  /*13*/Bit32u var24_aildrv;
  /*13*/Bit32u var25_aildrv;
  /*13*/Bit32u var26_aildrv;
  /*13*/Bit32u var36_aildrv;
  /*13*/Bit32u var40_aildrv;
  /*13*/Bit32u var46_aildrv;

  ///*28/*/ Bit16s var28_aildrv;
  /*4*/	Bit8u scratch[144];
}
_DIG_DRIVER;

typedef _DIG_DRIVER* HDIGDRIVER;    // Handle to digital driver

typedef MSS_STRUCT                   // Handle to driver
{
   REALFAR  seg;                 // Seg:off pointer to driver (off=0)
   Bit32u    sel;                   // Selector for driver (off=0)
   void*	buf;                 // Protected-mode pointer to driver
   Bit32u    size;                  // Size of driver image
   VDI_HDR *VHDR;                // Pointer to driver header (same as buf)
   Bit32s     type;                 // AIL3DIG or AIL3MDI (see below)
   
   Bit32s     initialized;          // 1 if hardware successfully init'd, else 0

   Bit32s     PM_ISR;               // -1 if no PM ISR hooked, else IRQ #

   HTIMER   server;              // DRV_SERVE periodic timer, if requested

								 // Vector to high-level destructor, if any

   void (*destructor)(HDIGDRIVER);

   // High-level descriptor (DIG_ or MDI_DRIVER)
void        *descriptor;
}
AIL_DRIVERx;



typedef MSS_STRUCT                   // Initialization file structure
{
   char     device_name[128];    // Device name
   char     driver_name[128];    // Driver filename
   IO_PARMS IO;                  // I/O parameters for driver
}
AIL_INI;


/*
//0 pointer to unkstr1 
#define sam_var2_sample_address_start 2
#define sam_var3_sample_address_x 3//start2
#define sam_var4_sample_address_len 4
#define sam_var5_sample_address_x 5//len2

//10 index(subadress) of sound
#define sam_var12_loop_count 12
#define sam_var16_volume 16
#define sam_var17_volume_pan 17
#define sam_var60_playback_rate 60

#define sam_var532_EOS_ptr 523
*/
//only for reference
struct SoundDriver                   // Handle to driver
{
	/* 00 */ uint32_t seg;                 // Seg:off pointer to driver (off=0)
	/* 04 */ uint32_t sel;                 // Selector for driver (off=0)
	/* 08 */ void	   *buf;                 // Protected-mode pointer to driver
	/* 0c */ uint32_t size;                // Size of driver image
	/* 10 */ void	   *VHDR;                // Pointer to driver header (same as buf)
	/* 14 */ int32_t  type;                // AIL3DIG or AIL3MDI (see below)
	/* 18 */ int32_t  initialized;         // 1 if hardware successfully init'd, else 0
	/* 1c */ int32_t  PM_ISR;              // -1 if no PM ISR hooked, else IRQ #
	/* 20 */ int32_t  server;              // DRV_SERVE periodic timer, if requested
	/* 24 */ void(*destructor)(void *); // Vector to high-level destructor, if any
	/* 28 */ void    *descriptor; // High-level descriptor (DIG_ or MDI_DRIVER)
}/* __attribute__((packed))*/;
struct SoundSample
{
  /*  00 */ SoundDriver *driver;            // Driver for playback
/*  04 */ uint32_t     status;            // SMP_ flags: _FREE, _DONE, _PLAYING
/*  08 */ void        *start[2];          // Sample buffer address (W)
/*  10 */ uint32_t     len[2];            // Sample buffer size in bytes (W)
/*  18 */ uint32_t     pos[2];            // Index to next byte (R/W)
/*  20 */ uint32_t     done[2];           // Nonzero if buffer with len=0 sent by app
/*  28 */ int32_t      current_buffer;    // Buffer # active (0/1)
/*  2c */ int32_t      last_buffer;       // Last active buffer (for double-buffering)
/*  30 */ int32_t      loop_count;        // 0-inf, 1-1
/*  34 */ int32_t      format;            // DIG_F format (8/16 bits, mono/stereo)
/*  38 */ uint32_t     flags;             // DIG_PCM_SIGN / DIG_PCM_ORDER (stereo only)
/*  3c */ int32_t      playback_rate;     // Playback rate in hertz
/*  40 */ int32_t      volume;            // Sample volume 0-127
/*  44 */ int32_t      pan;               // Mono panpot/stereo balance (0=L ... 127=R)
/*  48 */ int32_t      vol_scale[2][256]; // [left/mono=0,right=1][256] channel volume
/* ... */ void	*SOB;               // Start-of-block callback function
/* ... */ void	*EOB;               // End-of-buffer callback function
/* ... */ void        *EOS;               // End-of-sample callback function
/* ... */ int32_t      user_data[8];      // Miscellaneous user data
/* ... */ int32_t      system_data[8];    // Miscellaneous system data
}/* __attribute__((packed))*/;
//only for reference




typedef MSS_STRUCT                   // Initialization file structure
{
	Bit8u* buffer;
	Bit8u scratch[19];
}
unkstr2;//lenght 23 mybe identic with _SAMPLE

#endif //AIL_SOUND_
