//############################################################################
//##                                                                        ##
//##  Miles Sound System                                                    ##
//##                                                                        ##
//##  SYNTH.H                                                               ##
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

#ifndef SYNTH_H
#define SYNTH_H

#include <stdio.h>
#include <string.h>

#ifndef IS_PS2
#pragma pack(1)
#endif

#define N_BUILD_BUFFERS      9       // 9 build buffers in chain (0-8)
#define BUILD_LOW_WATER      2       // Synth is falling behind when ring margin falls this low
#define BUILD_HIGH_WATER     6       // DAC is falling behind when ring margin rises this high
#define BUILD_CENTER         4       // Midway point in ring buffer

#define INTERVAL_BUFFER_SIZE 65536   // Interval buffer is >= 200 ms long
#define MQ_SIZE              4096    // 4K message slots in MIDI queue

#ifndef F32
#define F32 float
#endif

#define FOURCC_MLS  mmioFOURCC('M','L','S',' ')
#define FOURCC_FMT  mmioFOURCC('f','m','t',' ')
#define FOURCC_PCM  mmioFOURCC('p','c','m',' ')
#define FOURCC_JUNK mmioFOURCC('j','u','n','k')
#define FOURCC_DATA mmioFOURCC('d','a','t','a')
#define FOURCC_FACT mmioFOURCC('f','a','c','t')
#define FOURCC_INFO mmioFOURCC('I','N','F','O')
#define FOURCC_INAM mmioFOURCC('I','N','A','M')

#define WAVE_FORMAT_PCM         1
#define WAVE_FORMAT_IMA_ADPCM   0x0011

//
// DLL instance handle
//

//
// Single global instance of synthesizer device
//

extern MSS_STRUCT SYNTHDEVICE FAR *DLS;


//
// Misc. definitions
//

MSS_STRUCT CHUNK
{
   FOURCC ckID;
   S32    ckSize;
   FOURCC subID;
   U8     data[1];
};

MSS_STRUCT SUBCHUNK
{
   FOURCC ckID;
   S32    ckSize;
   U8     data[1];
};

//
// .WAV PCM file format chunk
//

typedef MSS_STRUCT
{
   S8   FMT_string[4];
   U32  chunk_size;

   S16  format_tag;
   S16  channels;
   S32  sample_rate;
   S32  average_data_rate;
   S16  alignment;
   S16  bits_per_sample;
}
WAV_FMT;

//
// .WAV IMA ADPCM file format chunk
//

typedef MSS_STRUCT
{
   S8  FMT_string[4];
   U32 chunk_size;

   S16 format_tag;
   S16 channels;
   S32 sample_rate;
   S32 average_data_rate;
   S16 alignment;
   S16 bits_per_sample;
   S16 extra;
   S16 samples_per_block;
}
IMA_FMT;

//
// ASI MPEG Audio file format chunk
//
// This is a proprietary WAV extension, used to wrap DLS sample files
// created by ASI encoders.  (ASI encoders always output raw data, which
// needs a RIFF header in order to be navigable by DLS readers.)
//
// The IMA_FACT and IMA_DATA chunks are reused by the ASI format to
// store the total sample count and data stream, respectively.
//

typedef MSS_STRUCT
{
   S8 FMT_string[4];
   U32 chunk_size;

   S16 format_tag;               // WAVE_FORMAT_UNKNOWN
   S16 channels;                 // # of channels
   S32 sample_rate;              // Sample rate in samples/second
   S32 average_data_rate;        // Stream rate in bytes per second
   S16 alignment;                // Always 1 (actual alignment constraints are determined by ASI decoder)
   S16 bits_per_sample;          // Bits/sample value from encoder output
   S16 extra;                    // Always 4

   C8 original_file_suffix[4];   // Example: "MP3" with zero terminator
}
ASI_FMT;

//
// .WAV FACT chunk
//

typedef MSS_STRUCT
{
   S8  FACT_string[4];
   U32 chunk_size;
   U32 samples;
}
IMA_FACT;

//
// .WAV file data chunk
//

typedef MSS_STRUCT
{
   S8  DATA_string[4];
   U32 chunk_size;
   S8  data[1];
}
IMA_DATA;


//
// .WAV file data chunk
//

typedef MSS_STRUCT
{
   S8  DATA_string[4];
   U32 chunk_size;
   U8  data[1];
}
WAV_DATA;

//
// Articulation data
//

MSS_STRUCT CONN
{
   C8 FAR *name;
   U16 val;
};

extern const CONN src[12];

extern const CONN dest[18];

MSS_STRUCT CID
{
   U16 usSource;
   U16 usControl;
   U16 usDestination;
   U16 usTransform;
};

#define CONN_SRC_RPN0 -1
#define CONN_SRC_RPN1 -1
#define CONN_SRC_RPN2 -1

extern const CID CID_list[29];

enum ARTNAME
{
   LFO_FREQUENCY = 0,
   LFO_START_DELAY,
   LFO_ATTENUATION_SCALE,
   LFO_PITCH_SCALE,
   LFO_MODW_TO_ATTENUATION,
   LFO_MODW_TO_PITCH,
   VOL_EG_ATTACK_TIME,
   VOL_EG_DECAY_TIME,
   VOL_EG_SUSTAIN_LEVEL,
   VOL_EG_RELEASE_TIME,
   VOL_EG_VELOCITY_TO_ATTACK,
   VOL_EG_KEY_TO_DECAY,
   PITCH_EG_ATTACK_TIME,
   PITCH_EG_DECAY_TIME,
   PITCH_EG_SUSTAIN_LEVEL,
   PITCH_EG_RELEASE_TIME,
   PITCH_EG_VELOCITY_TO_ATTACK,
   PITCH_EG_KEY_TO_DECAY,
   DEFAULT_PAN,
   EG1_TO_ATTENUATION,
   EG2_TO_PITCH,
   KEY_ON_VELOCITY_TO_ATTENUATION,
   PITCH_WHEEL_TO_PITCH,
   KEY_NUMBER_TO_PITCH,
   MIDI_CONTROLLER_7_TO_ATTENUATION,
   MIDI_CONTROLLER_10_TO_PAN,
   MIDI_CONTROLLER_11_TO_ATTENUATION,
   RPN1_TO_PITCH,
   RPN2_TO_PITCH
};

MSS_STRUCT ARTICULATION
{
   S32 lScale[29];
};

#define IMPLICIT -1                      // This connection is fixed and cannot be overridden
#define ABSOLUTE_ZERO ((S32) 0x80000000) // 32-bit absolute-zero value

#define F_PI  3.141592654F
#define F_2PI 6.283185308F

//
// Default ARTICULATION members used where no articulator values are
// explicitly defined in region
//

extern const ARTICULATION ARTDEFAULT;

//
// Default WSMPL -- used if neither WAVE nor RGN contains a valid WSMP chunk
//

extern const WSMPL WSMPLDEFAULT;

//
// Build buffer descriptor: status = FULL, EMPTY, or index of next chunk
// to be filled
//

#define BUILD_FULL  (-1)
#define BUILD_EMPTY (0)

MSS_STRUCT BUILD_BUFFER
{
   S32 FAR *block;
   S32      status;
};

//
// MIDI channel status descriptor
//

MSS_STRUCT MIDI_CHANNEL
{
   S32 bank_LSB;
   S32 bank_MSB;
   S32 patch;
   S32 instrument_index;

   S8     control[128];
   S32    coarse_tuning;
   F32 fine_tuning;
   F32 pitch_bend_range;
   F32 pitch;
};

MSS_STRUCT MIDI_MESSAGE
{
   U8  status;
   U8  channel;
   U8  data_1;
   U8  data_2;
   S32 instrument_index;
};

//
// DLS voice descriptor
//

#define ATTACK_PHASE  0
#define DECAY_PHASE   1
#define SUSTAIN_PHASE 2
#define RELEASE_PHASE 3

MSS_STRUCT REGION
{
   RGNHEADER           header;
   WSMPL               sample;
   WLOOP               loop;
   WAVELINK            wave;
   ARTICULATION FAR   *connection;
   CONNECTIONLIST FAR *art1;        // Original ART1 chunk data, for diagnostics
};

MSS_STRUCT WAVEDESC
{
   S32       valid;

   S32       format;
   F32    rate;
   void FAR *data;
   S32       len;

   S32       WSMP_valid;
   WSMPL     sample;
   WLOOP     loop;
};

MSS_STRUCT VOICE
{
   S32                  active;
   S32                  key;  
   S32                  release_request;

   MSS_STRUCT WAVEDESC FAR *wave;

   MIDI_MESSAGE         trigger;
   MSS_STRUCT REGION FAR   *region;

   S32                  mixer_operation;

   S32                  play_cursor;
   U32                  src_fract;
   S32                  left_val;
   S32                  right_val;

   S32                  loop_start;
   S32                  loop_end;
   S32                  loop_size;

   F32               static_pitch;
   F32               static_atten;

   S32                  BPS;

   S32                  LFO_holdoff;
   F32               LFO_radians_per_interval;
   F32               LFO_phase_accumulator;
   F32               LFO_atten_scale;
   F32               LFO_pitch_scale;
   F32               LFO_CC1_to_atten;
   F32               LFO_CC1_to_pitch;

   S32                  EG1_active;
   S32                  EG1_phase;
   F32               EG1_scale;
   F32               EG1_atten;
   F32               EG1_attack_dB_per_interval;
   F32               EG1_lin_scale;
   S32                  EG1_attack_intervals;
   S32                  EG1_attack_time;
   F32               EG1_decay_dB_per_interval;
   F32               EG1_sustain_atten;
   F32               EG1_release_dB_per_interval;
   S32                  EG1_release_intervals;

   S32                  EG2_active;
   S32                  EG2_phase;
   F32               EG2_scale;
   F32               EG2_pitch;
   F32               EG2_attack_per_interval;
   S32                  EG2_attack_intervals;
   F32               EG2_decay_per_interval;
   F32               EG2_sustain_pitch;
   F32               EG2_release_per_interval;
   S32                  EG2_release_intervals;

   S32                  default_pan;
};

//
// DLS synthesizer type definitions
//

MSS_STRUCT SYNTHDEVICE
{
   HMDIDRIVER    mdi;

   U32           output_format;
   F32        output_sample_rate;
   S32           channels_per_sample;
   S32           bytes_per_channel;
   S32           bytes_per_sample;

   S32           stream_buffer_size;
   UINTa         user;
   AILDLSPCB     stream_poll_CB;
   AILDLSLCB     stream_lock_CB;
   AILDLSUCB     stream_unlock_CB;

   F32           service_rate_uS;
   U32           last_interval_us_count;
   U32           last_total;
   HTIMER        timer_handle;

   S32           bytes_to_write;

   BUILD_BUFFER  build[N_BUILD_BUFFERS];
   S32           bytes_per_buffer;

   S32          *interval_buffer;
               
   S32 FAR * FAR *reverb_buffers;
   S32            n_reverb_buffers;

   S32               build_queue_head;
   S32               build_queue_tail;
   S32               buffers_filled;

   AILEVENTCB    prev_event_fn;
   AILTIMBRECB   prev_timb_fn; 

   MIDI_CHANNEL  channel[16];

   MIDI_MESSAGE  MIDI_queue[MQ_SIZE];
   S32           MIDI_queue_head;
   S32           MIDI_queue_tail;

   VOICE    FAR *voice_list;
   S32           n_voices;

   U32           use_MMX;

   S32           enabled;

   S32           time_bias;
   U32           ms_count;
   U32           us_count;
   U32           last_ms_polled;
   U32           last_percent;

   MIXER_FLUSH   mixer_flush;
   MIXER_MERGE   mixer_merge;
   MIXER_COPY    mixer_copy;
};

MSS_STRUCT INSTRUMENT
{
   INSTRUMENT FAR *next;
   INSTRUMENT FAR *prev;
   INSTRUMENT FAR *next_in_set;
   INSTRUMENT FAR *prev_in_set;
   S32         set_key;
   S32         entry_num;

   S32                 DLS_file;    // DLS file containing instrument

   INSTHEADER          header;      // Bank/patch#/region count

   ARTICULATION   FAR *art_list;    // Articulation set(s)
   S32                 art_cnt;     // # of articulation set(s)

   REGION         FAR *region_list; // Instrument definitions by keyboard region
};

// ---------------------------------------------------------------------------
// Definitions/prototypes
// ---------------------------------------------------------------------------

#define ARYSIZE(x) ((S32) (sizeof((x)) / sizeof((x)[0])))

//
// Instrument list manager class
//

#define INS_SET_DIM    1
#define INS_BLOCK_SIZE 128

MSS_STRUCT InsListMgr
{
public:
   S32 current_size;
   INSTRUMENT FAR*used;
   INSTRUMENT FAR*avail;
   INSTRUMENT FAR*table[INS_SET_DIM];
   INSTRUMENT FAR*array;
};

extern InsListMgr FAR *instrument_list;

extern void INS_init(INSTRUMENT FAR *INS, void const FAR *data);
extern void INS_cleanup(INSTRUMENT FAR *INS);
extern S32 INS_is_match(INSTRUMENT FAR *INS, void FAR *data);
extern U32 INS_set(void const FAR *data);

extern void ILM_construct(InsListMgr FAR *ILM);
extern void ILM_dealloc(InsListMgr FAR *ILM, INSTRUMENT FAR *entry);
extern void ILM_dealloc(InsListMgr FAR *ILM, S32 entry_num);
extern void ILM_cleanup(InsListMgr FAR *ILM);
extern void ILM_destroy(InsListMgr FAR *ILM);
extern INSTRUMENT FAR *ILM_lookup(InsListMgr FAR *ILM, void FAR *seed);
extern INSTRUMENT FAR *ILM_alloc(InsListMgr FAR *ILM, void const FAR *seed=NULL);

MSS_STRUCT DLS_FILE
{
   DLS_FILE FAR *next;
   DLS_FILE FAR *prev;
   DLS_FILE FAR *next_in_set;
   DLS_FILE FAR *prev_in_set;
   S32       set_key;
   S32       entry_num;

   //
   // File data
   //

   C8        filename[256];
   void const FAR *image;

   //
   // COLH data
   //

   S32 cInstruments;

   //
   // PTBL data
   //

   POOLTABLE FAR *ptbl;
   POOLCUE   FAR *cues;

   //
   // WAVE data
   //

   WAVEDESC FAR *WAVE_list;
};

extern void DF_init(DLS_FILE FAR *DF, void const FAR *data);
extern void DF_cleanup(DLS_FILE FAR *DF);
extern S32 DF_is_match(DLS_FILE FAR *DF, void const FAR *data);
extern U32 DF_set(void const FAR *data);

//
// File list manager
//

#define FILE_SET_DIM    1
#define FILE_BLOCK_SIZE 4

MSS_STRUCT FileListMgr
{
   S32 current_size;    
   DLS_FILE FAR*used;           
   DLS_FILE FAR*avail;
   DLS_FILE FAR*table[FILE_SET_DIM];
   DLS_FILE FAR*array;
};

extern void FLM_construct(FileListMgr *FLM);
extern void FLM_dealloc(FileListMgr *FLM, DLS_FILE FAR *entry);
extern void FLM_dealloc(FileListMgr *FLM, S32 entry_num);
extern void FLM_destroy(FileListMgr *FLM);
extern void FLM_cleanup(FileListMgr *FLM);
extern DLS_FILE FAR *FLM_lookup(FileListMgr *FLM, void const FAR *seed);
extern DLS_FILE FAR *FLM_alloc(FileListMgr *FLM, void const FAR *seed=NULL);

extern FileListMgr FAR *file_list;

extern S32 InstrumentsInit();
extern void InstrumentsDeinit();

extern S32 DLSFILE_parse(void const FAR *data, DLS_FILE FAR * FAR *file_var, C8 const FAR *lpFileName, U32 flags);

extern S32 DLS_init     (SINTa FAR *lpdwHandle,
                         S32        dwFlags,
                         HMDIDRIVER MIDI_driver,
                         S32        output_format,
                         S32        output_sample_rate,
                         S32        buffer_size,
                         UINTa      user,
                         AILDLSPCB  buffer_poll_CB,
                         AILDLSLCB  buffer_lock_CB,
                         AILDLSUCB  buffer_unlock_CB);

extern S32 DLS_shutdown (SINTa      dwDLSHandle,
                         S32        dwFlags);

#ifndef IS_PS2
#pragma pack()
#endif

#endif

