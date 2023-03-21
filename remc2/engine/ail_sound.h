#pragma once
#ifndef AIL_SOUND_ACTIVE
#define AIL_SOUND_ACTIVE

#include <string>
#include <cstdint>

#define MSS_STRUCT struct

typedef MSS_STRUCT                     // VDI interface register structure
{
	int16_t AX;
	int16_t BX;
	int16_t CX;
	int16_t DX;
	int16_t SI;
	int16_t DI;
}
VDI_CALL;



typedef int32_t HTIMER;            // Handle to timer

#define REALFAR unsigned int


typedef MSS_STRUCT                      // I/O parameters structure
{
	/*0*/int IO;
	/*2*/int IRQ;
	/*4*/int DMA_8_bit;
	/*6*/int DMA_16_bit;
	/*8*/int32_t IO_reserved[4];
	//24
}
IO_PARMS;

typedef MSS_STRUCT                      // Standard MSS 3.X VDI driver header
{
	/*0*/int8_t ID_8[8];
	/*8*/uint16_t driver_version_8;
	/*10*/void* VDI_HDR_var10;
	/*12*/uint16_t VDI_HDR_var12;
	/*14*/uint16_t num_IO_configurations_14;//03
	/*16*/uint32_t environment_string_16;//2c380506	
	//20
	/*22*/IO_PARMS IO;//22
	/*20*/uint16_t scratch0;
	/*46*/int16_t VDI_HDR_var46;//busy
	/*48*/uint16_t VDI_HDR_var48;//driver_num
	/*50*/uint16_t VDI_HDR_var50;
	/*52*/uint16_t VDI_HDR_var52_this_ISR;
	/*54*/uint32_t VDI_HDR_var54_prev_ISR;
   /*58*/int8_t scratch[128];           // Shared scratch workspace   
   char dev_name[80];           // Device name (VDI version >= 1.12 only)
   int8_t scratch2[1692];
   char mdiapplendix[4];   
}
VDI_HDR;


struct _DIG_DRIVER;

typedef MSS_STRUCT                   // Handle to driver
{
	/*0*/REALFAR seg_0;//remove this! segment must be zero!
/*4*/uint32_t sel_1;
/*8*/uint8_t* buf_2;
/*12*/uint32_t size_3;
/*16*/VDI_HDR* VHDR_4;
/*20*/int32_t type_5;//type
/*24*/int32_t initialized_6;//initialized
/*28*/int32_t PM_ISR_7;//PM_ISR
/*32*/HTIMER server_8;
/*36*/void (*destructor_9)(void*);
/*40*/void* descriptor_10;
/*44*/
}
AIL_DRIVER;

typedef int(*EOS_callback_type)(void);

const int sample_mark = 100;

typedef MSS_STRUCT _SAMPLE           // Sample instance
{
	//char tag[4];
	 //16 - volume

struct _DIG_DRIVER* driver_0;//0
uint32_t status_1=0;//4
void* start_2_3[2];//8
uint32_t len_4_5[2];
uint32_t pos_6_7[2];            // Index to next byte (R/W)
uint32_t done_8_9[2];            // Nonzero if buffer with len=0 sent by app

int32_t current_buffer_10;      // Buffer # active (0/1)
int32_t last_buffer_11;         // Last active buffer (for double-buffering)

int32_t loop_count_12 = 0;
int32_t format_13 = 0;
uint32_t flags_14 = 0;
int32_t playback_rate_15 = 0;
int32_t volume_16 = 0;
int32_t pan_17 = 0;
int32_t vol_scale_18[2][256];
//uint32_t sam_varw18x[256];
//int8_t sam_varb20 = 0;//maybe sam_varw18x[0][1]?
//int8_t sam_varb21 = 0;//maybe sam_varw18x[0][2]?
//530 CALLBACK SOB
//532 CALLBACK EOB;                 // End-of-buffer callback function
//534 CALLBACK EOS;                 // End-of-sample callback function
EOS_callback_type sam_var532_EOS_ptr;
int32_t sam_var[1000];
   uint8_t* sam_var542;
   int index_sample = -1;

   void* start_44mhz;//8
   uint8_t mark44mark[sample_mark];
   //Mix_Chunk chunk;
}
SAMPLE;
typedef MSS_STRUCT _SAMPLE * HSAMPLE;           // Handle to sample

typedef struct
{
	uint16_t minimum_physical_sample_rate;
	uint16_t nominal_physical_sample_rate;
	uint16_t maximum_physical_sample_rate;

	uint16_t minimum_DMA_half_buffer_size;
	uint16_t maximum_DMA_half_buffer_size;

	uint32_t flags;
}
DIG_MODE;

typedef struct
{
	uint8_t    format_supported[16];
	DIG_MODE format_data[16];
}
DIG_DDT;

typedef struct
{
	void*  DMA_buffer_A;//0
	void*  DMA_buffer_B;//4
	uint16_t active_buffer;//8
}
DIG_DST;

typedef struct _DIG_DRIVER                    // Initialization file structure
{
	/*0*/AIL_DRIVER* drvr_0;
/*1*/DIG_DDT* DDT_1;
/*2*/DIG_DST* DST_2;
HTIMER timer_3;
 /*2*/int32_t half_buffer_size_4;
 int32_t DMA_rate_5;
 int32_t hw_format_6;
 uint32_t hw_mode_flags_7;
 /*13*/int32_t DMA_seg_8;
 /*13*/uint32_t DMA_sel_9;
 /*13*/void* DMA_buf_10;
  void* DMA_11_12[2];
  /*13*/int16_t buffer_flag_13;
  /*13*/int32_t last_buffer_14;
  /*13*/int32_t channels_per_sample_15;
  /*13*/int32_t bytes_per_channel_16;
  /*13*/int32_t channels_per_buffer_17;
  /*13*/int32_t samples_per_buffer_18;
  /*13*/int32_t build_size_19;
  /*20*/ int32_t* build_buffer_20;//
  /*13*/int32_t playing_21;
  /*13*/int32_t quiet_22;
  /*13*/HSAMPLE samples_23;
  /*13*/int32_t n_samples_24;
  /*13*/int32_t n_active_samples_25;
  /*13*/int32_t master_volume_26;
  /*13*/uint32_t var36_aildrv;
  /*13*/uint32_t var40_aildrv;
  /*13*/uint32_t var46_aildrv;

  ///*28/*/ int16_t var28_aildrv;
  /*4*/	uint8_t scratch[144];
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
	/*53 0*/int32_t     program[NUM_CHANS];  // Program Change
	/*69 16*/int32_t     pitch_l[NUM_CHANS];  // Pitch Bend LSB
	/*85 32*/int32_t     pitch_h[NUM_CHANS];  // Pitch Bend MSB

	/*101 48*/int32_t     c_lock[NUM_CHANS];  // Channel Lock
	/*117 64*/int32_t     c_prot[NUM_CHANS];  // Channel Lock Protection
	/*133 80*/int32_t     c_mute[NUM_CHANS];  // Channel Mute
	/*149 96*/int32_t     c_v_prot[NUM_CHANS];  // Voice Protection
	/*165 112*/int32_t     bank[NUM_CHANS];  // Patch Bank Select
	/*181 128*/int32_t     indirect[NUM_CHANS];  // ICA indirect controller value
	/*197 144*/int32_t     callback[NUM_CHANS];  // Callback Trigger

	/*213 160*/int32_t     mod[NUM_CHANS];  // Modulation
	/*229 176*/int32_t     vol[NUM_CHANS];  // Volume
	/*245 192*/int32_t     pan[NUM_CHANS];  // Panpot
	/*261 208*/int32_t     exp[NUM_CHANS];  // Expression
	/*277 224*/int32_t     sus[NUM_CHANS];  // Sustain
	/*393 240*/int32_t     reverb[NUM_CHANS];  // Reverb
	/*309 256*/int32_t     chorus[NUM_CHANS];  // Chorus

	/*325 272*/int32_t     bend_range[NUM_CHANS];  // Bender Range (data MSB, RPN 0 assumed)
	//341 288
}
CTRL_LOG;

typedef struct                            // XMIDI sequence state table
{
	/*0*/struct _MDI_DRIVER* driver_0;            // Driver for playback //0

	/*1*/uint32_t status_1;                       // SEQ_ flags

	/*2*/void* TIMB_2;                         // XMIDI IFF chunk pointers
	/*3*/uint8_t* RBRN_3;
	/*4*/void* EVNT_4;

	/*5*/uint8_t* EVNT_ptr_5;                     // Current event pointer

	/*6*/uint8_t* ICA_6;                          // Indirect Controller Array

	/*7*/xCALLBACK prefix_callback_7;              // XMIDI Callback Prefix handler
	/*8*/xCALLBACK trigger_callback_8_32;             // XMIDI Callback Trigger handler
	/*9*/xCALLBACK beat_callback_9;                // XMIDI beat/bar change handler
	/*10*/xCALLBACK EOS_10;                          // End-of-sequence callback function

	/*11*/int32_t     loop_count_11;                   // 0=one-shot, -1=indefinite, ...

	/*12*/int32_t     interval_count_12;               // # of intervals until next event
	/*13*/int32_t     interval_num_13;                 // # of intervals since start

	/*14*/int32_t     volume_14;                       // Sequence volume 0-127
	/*15*/int32_t     volume_target_15;                // Target sequence volume 0-127
	/*16*/int32_t     volume_accum_16;                 // Accumulated volume period
	/*17*/int32_t     volume_period_17;                // Period for volume stepping

	/*18*/int32_t     tempo_percent_18;                // Relative tempo percentage 0-100
	/*19*/int32_t     tempo_target_19;                 // Target tempo 0-100
	/*20*/int32_t     tempo_accum_20;                  // Accumulated tempo period
	/*21*/int32_t     tempo_period_21;                 // Period for tempo stepping
	/*22*/int32_t     tempo_error_22;                  // Error counter for tempo DDA

	/*23*/int32_t     beat_count_23;                   // Sequence playback position
	/*24*/int32_t     measure_count_24;

	/*25*/int32_t     time_numerator_25;               // Sequence timing data
	/*26*/int32_t     time_fraction_26;
	/*27*/int32_t     beat_fraction_27;
	/*28*/int32_t     time_per_beat_28;

	/*29*/uint8_t* FOR_ptrs[FOR_NEST];    // Loop stack
	/*33*/int32_t     FOR_loop_count_33[FOR_NEST];

	/*37*/int32_t     chan_map_37[NUM_CHANS];   // Physical channel map for sequence

	/*53*/CTRL_LOG shadow_53;                       // Controller values for sequence

	/*341*/int32_t     note_count;                   // # of notes "on"

	/*342*/int32_t     note_chan[MAX_NOTES];   // Channel for queued note (-1=free)
	/*374*/int32_t     note_num[MAX_NOTES];   // Note # for queued note
	/*406*/int32_t     note_time[MAX_NOTES];   // Remaining duration in intervals

	/*438*/int32_t     user_data[8];               // Miscellaneous user data
	/*446*/int32_t     system_data[8];               // Miscellaneous system data
	
	int32_t seq_40;

	int32_t seq_101[NUM_CHANS];
	int32_t seq_117[NUM_CHANS];
	int32_t seq_149[NUM_CHANS];
	int32_t seq_277[NUM_CHANS];


	
	
	int32_t seq_341;
	int32_t seq_342[MAX_NOTES];
	int32_t seq_374[MAX_NOTES];


	int32_t sequence_num;

}
SEQUENCE;

typedef SEQUENCE * HSEQUENCE;       // Handle to sequence

typedef struct
{
	void*  library_environment;
	void*  GTL_suffix;

	uint16_t    num_voices;

	uint16_t    max_melodic_channel;
	uint16_t    min_melodic_channel;
	uint16_t    percussion_channel;
}
MDI_DDT;

typedef struct
{
	std::string library_directory;
	std::string GTL_filename;

	int8_t       MIDI_data[512];
}
MDI_DST;

typedef struct _MDI_DRIVER          // Handle to XMIDI driver
{
 AIL_DRIVER *drvr_0;                // Base driver descriptor
 MDI_DDT* DDT_1;
 MDI_DST* DST_2;
 HTIMER timer_3;
 int32_t interval_time_4;
 int32_t disable_5;
 HSEQUENCE sequences_6;
 uint32_t n_sequences_7;

 /*8*/int32_t        lock[NUM_CHANS];   // 1 if locked, 2 if protected, else 0
 /*24*/HSEQUENCE   locker[NUM_CHANS];   // HSEQUENCE which locked channel
 /*40*/HSEQUENCE   owner[NUM_CHANS];   // HSEQUENCE which owned locked channel
 /*56*/HSEQUENCE   user[NUM_CHANS];   // Last sequence to use channel
 /*72*/int32_t        state[NUM_CHANS];   // Lock state prior to being locked

 /*88*/int32_t        notes[NUM_CHANS];   // # of active notes in channel

 /*104*/xCALLBACK    event_trap;          // MIDI event trap callback function
 /*105*/xCALLBACK    timbre_trap;         // MIDI timbre request callback function

 /*106*/int32_t        message_count;       // MIDI message count
 /*107*/int32_t        offset;              // MIDI buffer offset

 /*108*/int32_t        master_volume;       // Master XMIDI note volume 0-127

 /*109*/int32_t        system_data[8];      // Miscellaneous system data


 ///*13*/uint32_t var8_aildrv;
 ///*13*/uint32_t var9_aildrv;
 ///*13*/uint32_t var10_aildrv;
  //uint32_t var11_aildrv;
  //uint32_t var12_aildrv;
  ///*13*/uint8_t* var13_aildrv;
  ///*13*/uint32_t var14_aildrv;
  ///*13*/uint32_t var15_aildrv;
  ///*13*/uint32_t var16_aildrv;
  ///*13*/uint32_t var17_aildrv;
  ///*13*/uint32_t var18_aildrv;
  ///*13*/uint32_t var19_aildrv;
  ///*20*/ uint8_t* var20_aildrv_dig_drv;
  ///*13*/uint32_t var21_aildrv;
  ///*13*/uint32_t var22_aildrv;
  /*13*/HSEQUENCE var23_aildrvx[NUM_CHANS];
  ///*13*/uint32_t var24_aildrv;
  ///*13*/uint32_t var25_aildrv;
  ///*13*/uint32_t var26_aildrv;
  ///*13*/uint32_t var36_aildrv;
  ///*13*/uint32_t var40_aildrv;
  ///*13*/uint32_t var46_aildrv;

  ///*28/*/ int16_t var28_aildrv;
 int16_t var104_aildrv;
 int16_t var105_aildrv;
 int16_t var106_aildrv;
 int16_t var107_aildrv;
 int16_t var108_aildrv;
  /*4*/	uint8_t scratch[144];
}
MDI_DRIVER;

typedef MDI_DRIVER* HMDIDRIVER;    // Handle to digital driver

typedef void(*AILTIMERCB)    (HMDIDRIVER user);

typedef MSS_STRUCT                   // Initialization file structure
{
   char     device_name[128];    // Device name
   char     driver_name[128];    // Driver filename
   char     driver_path[260];    // Full Driver path
   IO_PARMS IO;                  // I/O parameters for driver
}
AIL_INI;
/*
typedef struct _XMIDISUB
{
	uint8_t trackname[18];//0
	uint8_t* data_buffer_4b;//18
	uint16_t var5b;//22
	uint32_t var6;//24
	//uint32_t var7;//28
}
XMIDISUBHEADER;

typedef struct _XMIDIHEADER
{
	uint8_t sign[4];//RNC
	uint32_t size;
	uint8_t x[32-8];
	XMIDISUBHEADER track[6];
}
XMIDIHEADER;*/

#endif //AIL_SOUND_
