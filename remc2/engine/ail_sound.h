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

//0 pointer to unkstr1 
//#define sam_var2_sample_address_start 2
//#define sam_var3_sample_address_x 3
//#define sam_var4_sample_address_len 4
//#define sam_var5_sample_address_x 5

//10 position of any struct
//#define sam_var12_loop_count 12
//#define sam_var17_volume_pan 17
//#define sam_var15_playback_rate 15

//#define sam_var532_EOS_ptr 523

typedef int(*EOS_callback_type)(void);

typedef MSS_STRUCT _SAMPLE           // Sample instance
{
	//char tag[4];
	 //16 - volume
   Bit32s sam_var[1000];
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
	 Bit16s sam_varxw18 = 0;
	 Bit8s sam_varbx20 = 0;
	 Bit8s sam_varbx21 = 0;
	 EOS_callback_type sam_var532_EOS_ptr;
	 //sam var 533+ -SINTa[8] user data
   Bit8u* sam_var542;
   /*
	0 - char       tag[4];            // HSAM

	1 - HDIGDRIVER driver;            // Driver for playback

	2 - U32      status;              // SMP_ flags: _FREE, _DONE, _PLAYING

	3,4 - void FAR *start[2];           // Sample buffer address (W)
	5,6 - U32       len  [2];           // Sample buffer size in bytes (W)
	7,8 - U32       pos  [2];           // Index to next byte (R/W)
	9,10 - U32       done [2];           // Nonzero if buffer with len=0 sent by app

	11 - U32      src_fract;           // Fractional part of source address
	12 -S32      left_val;            // Mixer source value from end of last buffer
	13 -S32      right_val;           // Mixer source value from end of last buffer

	14 -S32      current_buffer;      // Buffer # active (0/1)
	15- S32      last_buffer;         // Last active buffer (for double-buffering)
	16- S32      starved;             // Buffer stream has run out of data

	t12- S32      loop_count;          // # of cycles-1 (1=one-shot, 0=indefinite)
	S32      loop_start;          // Starting offset of loop block (0=SOF)
	S32      loop_end;            // End offset of loop block (-1=EOF)

	13-S32      format;              // DIG_F format (8/16 bits, mono/stereo)
	14-U32      flags;               // DIG_PCM_SIGN / DIG_PCM_ORDER (stereo only)

	t15 S32      playback_rate;       // Playback rate in hertz

	t16- S32      volume;              // Sample volume 0-127
	t17- S32      pan;                 // Mono panpot/stereo balance (0=L ... 127=R)

	S32      left_scale;          // Left/mono volume scalar 0-2047
	S32      right_scale;         // Right volume scalar 0-2047

	S32      service_type;        // 1 if single-buffered; 2 if streamed

	AILSAMPLECB  SOB;             // Start-of-block callback function
	AILSAMPLECB  EOB;             // End-of-buffer callback function
	AILSAMPLECB  EOS;             // End-of-sample callback function

	S32      user_data  [8];      // Miscellaneous user data
	S32      system_data[8];      // Miscellaneous system data

	ADPCMDATA adpcm;

 #ifdef IS_WINDOWS

	S32      SOB_IsWin32s;        // Is this a Win32s callback
	S32      EOB_IsWin32s;        // Is this a Win32s callback
	S32      EOS_IsWin32s;        // Is this a Win32s callback

	//
	// DirectSound-specific data
	//

	S32      secondary_buffer;    // Secondary buffer index

	S32      service_interval;    // Service sample every n ms
	S32      service_tick;        // Current service countdown value
	S32      buffer_segment_size; // Buffer segment size to fill

	S32      prev_segment;        // Previous segment # (0...n)
	S32      prev_cursor;         // Previous play cursor location

	S32      bytes_remaining;     // # of bytes left to play (if not -1)

	S32      direct_control;      // 1 if app controls buffer, 0 if MSS

 #endif

	S32      doeob;               // Flags to trigger callbacks
	S32      dosob;
	S32      doeos;

	//
	// Sample pipeline stages
	//

	DPINFO   pipeline[N_SAMPLE_STAGES];

	//
	// Reverb parms
	//

	F32      reverb_level;           // Level [0.0, 1.0]
	F32      reverb_reflect_time;    // Reflect time in milliseconds
	F32      reverb_decay_time;      // Decay time [0.1, 20.0]
	S32      base_scale;             // Original 12-bit volume scalar
   */
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





typedef MSS_STRUCT                   // Initialization file structure
{
	Bit8u* buffer;
	Bit8u scratch[19];
}
unkstr2;//lenght 23 mybe identic with _SAMPLE

#endif //AIL_SOUND_
