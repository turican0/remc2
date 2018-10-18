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

typedef Bit32s HTIMER;            // Handle to timer

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
	/*0*/Bit8s ID_8[8];
	/*8*/Bit16u driver_version_8;
	/*10*/void* VDI_HDR_var10;
	/*12*/Bit16u VDI_HDR_var12;
	/*14*/Bit16u num_IO_configurations_14;//03
	/*16*/Bit32u environment_string_16;//2c380506	
	//20
	/*22*/IO_PARMS IO;//22
	/*20*/Bit16u scratch0;
	/*46*/Bit16s VDI_HDR_var46;//busy
	/*48*/Bit16u VDI_HDR_var48;//driver_num
	/*50*/Bit16u VDI_HDR_var50;
	/*52*/Bit16u VDI_HDR_var52_this_ISR;
	/*54*/Bit32u VDI_HDR_var54_prev_ISR;
   /*58*/Bit8s scratch[128];           // Shared scratch workspace   
   Bit8s dev_name[80];           // Device name (VDI version >= 1.12 only)
   Bit8s scratch2[1692];
   char mdiapplendix[4];   
}
VDI_HDR;


struct _DIG_DRIVER;

typedef MSS_STRUCT                   // Handle to driver
{
	/*0*/REALFAR seg_0;//remove this! segment must be zero!
/*4*/Bit32u sel_1;
/*8*/void* buf_2;
/*12*/Bit32u size_3;
/*16*/VDI_HDR* VHDR_4;
/*20*/Bit32s type_5;//type
/*24*/Bit32s initialized_6;//initialized
/*28*/Bit32s PM_ISR_7;//PM_ISR
/*32*/HTIMER server_8;
/*36*/void (*destructor_9)(void*);
/*40*/void* descriptor_10;
/*44*/
}
AIL_DRIVER;

typedef int(*EOS_callback_type)(void);


typedef MSS_STRUCT _SAMPLE           // Sample instance
{
	//char tag[4];
	 //16 - volume

struct _DIG_DRIVER* driver_0;
Bit32u status_1;
void* start_2_3[2];
Bit32u len_4_5[2];
Bit32u pos_6_7[2];            // Index to next byte (R/W)
Bit32u done_8_9[2];            // Nonzero if buffer with len=0 sent by app

Bit32s current_buffer_10;      // Buffer # active (0/1)
Bit32s last_buffer_11;         // Last active buffer (for double-buffering)

Bit32s loop_count_12 = 0;
Bit32s format_13 = 0;
Bit32u flags_14 = 0;
Bit32s playback_rate_15 = 0;
Bit32s volume_16 = 0;
Bit32s pan_17 = 0;
Bit32s vol_scale_18[2][256];
//Bit32u sam_varw18x[256];
//Bit8s sam_varb20 = 0;//maybe sam_varw18x[0][1]?
//Bit8s sam_varb21 = 0;//maybe sam_varw18x[0][2]?
//530 CALLBACK SOB
//532 CALLBACK EOB;                 // End-of-buffer callback function
//534 CALLBACK EOS;                 // End-of-sample callback function
EOS_callback_type sam_var532_EOS_ptr;
Bit32s sam_var[1000];
   Bit8u* sam_var542;
}
SAMPLE;
typedef MSS_STRUCT _SAMPLE * HSAMPLE;           // Handle to sample

typedef struct
{
	Bit16u minimum_physical_sample_rate;
	Bit16u nominal_physical_sample_rate;
	Bit16u maximum_physical_sample_rate;

	Bit16u minimum_DMA_half_buffer_size;
	Bit16u maximum_DMA_half_buffer_size;

	Bit32u flags;
}
DIG_MODE;

typedef struct
{
	Bit8u    format_supported[16];
	DIG_MODE format_data[16];
}
DIG_DDT;

typedef struct
{
	void*  DMA_buffer_A;//0
	void*  DMA_buffer_B;//4
	Bit16u     active_buffer;//8
}
DIG_DST;

typedef struct _DIG_DRIVER                    // Initialization file structure
{
	/*0*/AIL_DRIVER* drvr_0;
/*1*/DIG_DDT* DDT_1;
/*2*/DIG_DST* DST_2;
HTIMER timer_3;
 /*2*/Bit32s half_buffer_size_4;
 Bit32s DMA_rate_5;
 Bit32s hw_format_6;
 Bit32u hw_mode_flags_7;
 /*13*/void* DMA_seg_8;
 /*13*/Bit32u DMA_sel_9;
 /*13*/void* DMA_buf_10;
  void* DMA_11_12[2];
  /*13*/Bit16s* buffer_flag_13;
  /*13*/Bit32s last_buffer_14;
  /*13*/Bit32s channels_per_sample_15;
  /*13*/Bit32s bytes_per_channel_16;
  /*13*/Bit32s channels_per_buffer_17;
  /*13*/Bit32s samples_per_buffer_18;
  /*13*/Bit32s build_size_19;
  /*20*/ Bit32s* build_buffer_20;//
  /*13*/Bit32s playing_21;
  /*13*/Bit32s quiet_22;
  /*13*/HSAMPLE samples_23;
  /*13*/Bit32s n_samples_24;
  /*13*/Bit32s n_active_samples_25;
  /*13*/Bit32s master_volume_26;
  /*13*/Bit32u var36_aildrv;
  /*13*/Bit32u var40_aildrv;
  /*13*/Bit32u var46_aildrv;

  ///*28/*/ Bit16s var28_aildrv;
  /*4*/	Bit8u scratch[144];
}
DIG_DRIVER;

typedef DIG_DRIVER * HDIGDRIVER;    // WAIL-compatible typedef


//typedef void* xHMDIDRIVER;

#define MAX_NOTES                32       // Max # of notes "on"
#define FOR_NEST                 4        // # of nested XMIDI FOR loops
#define NUM_CHANS                16       // # of possible MIDI channels

typedef void* xCALLBACK;            // Generic callback function prototype

typedef struct                      // MIDI status log structure
{
	/*0*/Bit32s     program[NUM_CHANS];  // Program Change
	/*64*/Bit32s     pitch_l[NUM_CHANS];  // Pitch Bend LSB
	/*128*/Bit32s     pitch_h[NUM_CHANS];  // Pitch Bend MSB

	/*192*/Bit32s     c_lock[NUM_CHANS];  // Channel Lock
	/*256*/Bit32s     c_prot[NUM_CHANS];  // Channel Lock Protection
	/*320*/Bit32s     c_mute[NUM_CHANS];  // Channel Mute
	/*384*/Bit32s     c_v_prot[NUM_CHANS];  // Voice Protection
	/*448*/Bit32s     bank[NUM_CHANS];  // Patch Bank Select
	/*512*/Bit32s     indirect[NUM_CHANS];  // ICA indirect controller value
	/*576*/Bit32s     callback[NUM_CHANS];  // Callback Trigger

	/*640*/Bit32s     mod[NUM_CHANS];  // Modulation
	/*704*/Bit32s     vol[NUM_CHANS];  // Volume
	/*768*/Bit32s     pan[NUM_CHANS];  // Panpot
	/*832*/Bit32s     exp[NUM_CHANS];  // Expression
	/*896*/Bit32s     sus[NUM_CHANS];  // Sustain
	/*960*/Bit32s     reverb[NUM_CHANS];  // Reverb
	/*1024*/Bit32s     chorus[NUM_CHANS];  // Chorus

	/*1088*/Bit32s     bend_range[NUM_CHANS];  // Bender Range (data MSB, RPN 0 assumed)
	//1152
}
CTRL_LOG;

typedef struct                            // XMIDI sequence state table
{
	/*0*/struct _MDI_DRIVER* driver_0;            // Driver for playback //0

	/*1*/Bit32u status_1;                       // SEQ_ flags

	/*2*/void* TIMB_2;                         // XMIDI IFF chunk pointers
	/*3*/void* RBRN_3;
	/*4*/void* EVNT_4;

	/*5*/Bit8u* EVNT_ptr_5;                     // Current event pointer

	/*6*/Bit8u* ICA_6;                          // Indirect Controller Array

	/*7*/xCALLBACK prefix_callback_7;              // XMIDI Callback Prefix handler
	/*8*/xCALLBACK trigger_callback_8_32;             // XMIDI Callback Trigger handler
	/*9*/xCALLBACK beat_callback_9;                // XMIDI beat/bar change handler
	/*10*/xCALLBACK EOS_10;                          // End-of-sequence callback function

	/*11*/Bit32s     loop_count_11;                   // 0=one-shot, -1=indefinite, ...

	/*12*/Bit32s     interval_count_12;               // # of intervals until next event
	/*13*/Bit32s     interval_num_13;                 // # of intervals since start

	/*14*/Bit32s     volume_14;                       // Sequence volume 0-127
	/*15*/Bit32s     volume_target_15;                // Target sequence volume 0-127
	/*16*/Bit32s     volume_accum_16;                 // Accumulated volume period
	/*17*/Bit32s     volume_period_17;                // Period for volume stepping

	/*18*/Bit32s     tempo_percent_18;                // Relative tempo percentage 0-100
	/*19*/Bit32s     tempo_target_19;                 // Target tempo 0-100
	/*20*/Bit32s     tempo_accum_20;                  // Accumulated tempo period
	/*21*/Bit32s     tempo_period_21;                 // Period for tempo stepping
	/*22*/Bit32s     tempo_error_22;                  // Error counter for tempo DDA

	/*23*/Bit32s     beat_count_23;                   // Sequence playback position
	/*24*/Bit32s     measure_count_24;

	/*25*/Bit32s     time_numerator_25;               // Sequence timing data
	/*26*/Bit32s     time_fraction_26;
	/*27*/Bit32s     beat_fraction_27;
	/*28*/Bit32s     time_per_beat_28;

	/*29*/void* FOR_ptrs[FOR_NEST];    // Loop stack
	/*33*/Bit32s     FOR_loop_count_33[FOR_NEST];

	/*37*/Bit32s     chan_map_37[NUM_CHANS];   // Physical channel map for sequence

	/*53*/CTRL_LOG shadow_53;                       // Controller values for sequence

	/*1205*/Bit32s     note_count;                   // # of notes "on"

	/*1333*/Bit32s     note_chan[MAX_NOTES];   // Channel for queued note (-1=free)
	/*1461*/Bit32s     note_num[MAX_NOTES];   // Note # for queued note
	/*1589*/Bit32s     note_time[MAX_NOTES];   // Remaining duration in intervals

	/*1621*/Bit32s     user_data[8];               // Miscellaneous user data
	/*1653*/Bit32s     system_data[8];               // Miscellaneous system data
	
	Bit32s seq_40;

	Bit32s seq_101[NUM_CHANS];
	Bit32s seq_117[NUM_CHANS];
	Bit32s seq_149[NUM_CHANS];
	Bit32s seq_277[NUM_CHANS];


	
	
	Bit32s seq_341;
	Bit32s seq_342[MAX_NOTES];
	Bit32s seq_374[MAX_NOTES];


}
SEQUENCE;

typedef SEQUENCE * HSEQUENCE;       // Handle to sequence

typedef struct
{
	void*  library_environment;
	void*  GTL_suffix;

	Bit16u    num_voices;

	Bit16u    max_melodic_channel;
	Bit16u    min_melodic_channel;
	Bit16u    percussion_channel;
}
MDI_DDT;

typedef struct
{
	Bit8s     library_directory[128];
	Bit8s     GTL_filename[128];

	Bit8s     MIDI_data[512];
}
MDI_DST;

typedef struct _MDI_DRIVER          // Handle to XMIDI driver
{
 AIL_DRIVER *drvr_0;                // Base driver descriptor
 MDI_DDT* DDT_1;
 MDI_DST* DST_2;
 HTIMER timer_3;
 Bit32s interval_time_4;
 Bit32s disable_5;
 HSEQUENCE sequences_6;
 Bit32u n_sequences_7;

 /*8*/Bit32s        lock[NUM_CHANS];   // 1 if locked, 2 if protected, else 0
 /*9*/HSEQUENCE   locker[NUM_CHANS];   // HSEQUENCE which locked channel
 /*10*/HSEQUENCE   owner[NUM_CHANS];   // HSEQUENCE which owned locked channel
 /*11*/HSEQUENCE   user[NUM_CHANS];   // Last sequence to use channel
 /*11*/Bit32s        state[NUM_CHANS];   // Lock state prior to being locked

 /*11*/Bit32s        notes[NUM_CHANS];   // # of active notes in channel

 /*11*/xCALLBACK    event_trap;          // MIDI event trap callback function
 /*11*/xCALLBACK    timbre_trap;         // MIDI timbre request callback function

 /*11*/Bit32s        message_count;       // MIDI message count
 /*11*/Bit32s        offset;              // MIDI buffer offset

 /*11*/Bit32s        master_volume;       // Master XMIDI note volume 0-127

 /*11*/Bit32s        system_data[8];      // Miscellaneous system data


 ///*13*/Bit32u var8_aildrv;
 ///*13*/Bit32u var9_aildrv;
 ///*13*/Bit32u var10_aildrv;
  //Bit32u var11_aildrv;
  //Bit32u var12_aildrv;
  ///*13*/Bit8u* var13_aildrv;
  ///*13*/Bit32u var14_aildrv;
  ///*13*/Bit32u var15_aildrv;
  ///*13*/Bit32u var16_aildrv;
  ///*13*/Bit32u var17_aildrv;
  ///*13*/Bit32u var18_aildrv;
  ///*13*/Bit32u var19_aildrv;
  ///*20*/ Bit8u* var20_aildrv_dig_drv;
  ///*13*/Bit32u var21_aildrv;
  ///*13*/Bit32u var22_aildrv;
  ///*13*/HSAMPLE var23_aildrvx;
  ///*13*/Bit32u var24_aildrv;
  ///*13*/Bit32u var25_aildrv;
  ///*13*/Bit32u var26_aildrv;
  ///*13*/Bit32u var36_aildrv;
  ///*13*/Bit32u var40_aildrv;
  ///*13*/Bit32u var46_aildrv;

  ///*28/*/ Bit16s var28_aildrv;
 Bit16s var104_aildrv;
 Bit16s var105_aildrv;
 Bit16s var106_aildrv;
 Bit16s var107_aildrv;
 Bit16s var108_aildrv;
  /*4*/	Bit8u scratch[144];
}
MDI_DRIVER;

typedef MDI_DRIVER* HMDIDRIVER;    // Handle to digital driver

typedef MSS_STRUCT                   // Initialization file structure
{
   char     device_name[128];    // Device name
   char     driver_name[128];    // Driver filename
   IO_PARMS IO;                  // I/O parameters for driver
}
AIL_INI;
#endif //AIL_SOUND_
