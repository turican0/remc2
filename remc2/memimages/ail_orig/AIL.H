/*ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   IBM Audio Interface Library                                         лл
//лл                                                                       лл
//лл   AIL.H: C API function prototypes                                    лл
//лл                                                                       лл
//лл   Source compatible with Borland/Microsoft C/C++                      лл
//лл                                                                       лл
//лл   AIL.H release 2.03                                                  лл
//лл   For use with API version 2.11                                       лл
//лл                                                                       лл
//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   Copyright (C) 1991, 1992 Miles Design, Inc.                         лл
//лл                                                                       лл
//лл   Miles Design, Inc.                                                  лл
//лл   10926 Jollyville #308                                               лл
//лл   Austin, TX 78759                                                    лл
//лл   (512) 345-2642 / FAX (512) 338-9630 / BBS (512) 454-9990            лл
//лл                                                                       лл
//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
*/

#ifndef AIL_H
#define AIL_H

#define SEQ_STOPPED 0       /* equates for AIL_sequence_status()         */
#define SEQ_PLAYING 1
#define SEQ_DONE 2

#define DAC_STOPPED 0       /* equates for AIL_sound_buffer_status()     */
#define DAC_PAUSED 1        /*             AIL_VOC_playback_status()     */
#define DAC_PLAYING 2
#define DAC_DONE 3

#define XMIDI_DRVR 3        /* equates for drvr_desc.drvr_type values    */
#define DSP_DRVR 2

typedef int HTIMER;         /* handle to timer                           */
typedef int HDRIVER;        /* handle to driver                          */
typedef int HSEQUENCE;      /* handle to XMIDI sequence                  */

typedef struct
{
   unsigned min_API_version;
   unsigned drvr_type;
   char data_suffix[4];
   void far *dev_name_table;
   int default_IO;
   int default_IRQ;
   int default_DMA;
   int default_DRQ;
   int service_rate;
   unsigned display_size;
}  
drvr_desc;

typedef struct
{
   unsigned pack_type;
   unsigned sample_rate;
   void far *data;
   unsigned long len;
}
sound_buff;

#ifdef __cplusplus
extern "C" {
#endif

/********************/
/*                  */
/* Process services */
/*                  */
/********************/

void far cdecl AIL_startup(void);
void far cdecl AIL_shutdown(char far *signoff_msg);
HTIMER far cdecl AIL_register_timer(void (far cdecl *callback_fn)());
void far cdecl AIL_set_timer_period(HTIMER timer, unsigned long microseconds);
void far cdecl AIL_set_timer_frequency(HTIMER timer, unsigned long hertz);
void far cdecl AIL_set_timer_divisor(HTIMER timer, unsigned PIT_divisor);
unsigned far cdecl AIL_interrupt_divisor(void);
void far cdecl AIL_start_timer(HTIMER timer);
void far cdecl AIL_start_all_timers(void);
void far cdecl AIL_stop_timer(HTIMER timer);
void far cdecl AIL_stop_all_timers(void);
void far cdecl AIL_release_timer_handle(HTIMER timer);
void far cdecl AIL_release_all_timers(void);

/*************************/
/*                       */
/* Installation services */
/*                       */
/*************************/

HDRIVER far cdecl AIL_register_driver(void far *driver_base_addr);
void far cdecl AIL_release_driver_handle(HDRIVER driver);
drvr_desc far * cdecl far AIL_describe_driver(HDRIVER driver);
unsigned far cdecl AIL_detect_device(HDRIVER driver, unsigned IO_addr, 
    unsigned IRQ, unsigned DMA, unsigned DRQ);
void far cdecl AIL_init_driver(HDRIVER driver, unsigned IO_addr, 
    unsigned IRQ, unsigned DMA, unsigned DRQ);
void far cdecl AIL_shutdown_driver(HDRIVER driver, char far *signoff_msg);
                    
/********************************/
/*                              */
/* Digital performance services */
/*                              */
/********************************/

unsigned far cdecl AIL_index_VOC_block(HDRIVER driver, void far *VOC_file, unsigned 
   block_marker, sound_buff far *buff);
void far cdecl AIL_register_sound_buffer(HDRIVER driver, unsigned buffer_num,
   sound_buff far *buff);
void far cdecl AIL_format_sound_buffer(HDRIVER driver, sound_buff far *buff);
unsigned far cdecl AIL_sound_buffer_status(HDRIVER driver, unsigned buffer_num);
void far cdecl AIL_format_VOC_file(HDRIVER driver, void far *VOC_file, int
   block_marker);
void far cdecl AIL_play_VOC_file(HDRIVER driver, void far *VOC_file, int
   block_marker);
unsigned far cdecl AIL_VOC_playback_status(HDRIVER driver);

void far cdecl AIL_start_digital_playback(HDRIVER driver);
void far cdecl AIL_stop_digital_playback(HDRIVER driver);
void far cdecl AIL_pause_digital_playback(HDRIVER driver);
void far cdecl AIL_resume_digital_playback(HDRIVER driver);
void far cdecl AIL_set_digital_playback_volume(HDRIVER driver, unsigned volume);
unsigned far cdecl AIL_digital_playback_volume(HDRIVER driver);
void far cdecl AIL_set_digital_playback_panpot(HDRIVER driver, unsigned panpot);
unsigned far cdecl AIL_digital_playback_panpot(HDRIVER driver);

/******************************/
/*                            */
/* XMIDI performance services */
/*                            */
/******************************/

unsigned far cdecl AIL_state_table_size(HDRIVER driver);
HSEQUENCE far cdecl AIL_register_sequence(HDRIVER driver, void far *FORM_XMID,
   unsigned sequence_num, void far *state_table, void far *controller_table);
void far cdecl AIL_release_sequence_handle(HDRIVER driver, HSEQUENCE sequence);

unsigned far cdecl AIL_default_timbre_cache_size(HDRIVER driver);
void far cdecl AIL_define_timbre_cache(HDRIVER driver, void far *cache_addr,
   unsigned cache_size);                     
unsigned far cdecl AIL_timbre_request(HDRIVER driver, HSEQUENCE sequence);
unsigned far cdecl AIL_timbre_status(HDRIVER driver, int bank, int patch);
void far cdecl AIL_install_timbre(HDRIVER driver, int bank, int patch, 
   void far *src_addr);
void far cdecl AIL_protect_timbre(HDRIVER driver, int bank, int patch);
void far cdecl AIL_unprotect_timbre(HDRIVER driver, int bank, int patch);

void far cdecl AIL_start_sequence(HDRIVER driver, HSEQUENCE sequence);
void far cdecl AIL_stop_sequence(HDRIVER driver, HSEQUENCE sequence);
void far cdecl AIL_resume_sequence(HDRIVER driver, HSEQUENCE sequence);
unsigned far cdecl AIL_sequence_status(HDRIVER driver, HSEQUENCE sequence);
unsigned far cdecl AIL_relative_volume(HDRIVER driver, HSEQUENCE sequence);
unsigned far cdecl AIL_relative_tempo(HDRIVER driver, HSEQUENCE sequence);
void far cdecl AIL_set_relative_volume(HDRIVER driver, HSEQUENCE sequence,
   unsigned percent, unsigned milliseconds);
void far cdecl AIL_set_relative_tempo(HDRIVER driver, HSEQUENCE sequence,
   unsigned percent, unsigned milliseconds);
int far cdecl AIL_controller_value(HDRIVER driver, HSEQUENCE sequence,
   unsigned channel, unsigned controller_num);
void far cdecl AIL_set_controller_value(HDRIVER driver, HSEQUENCE sequence,
   unsigned channel, unsigned controller_num, unsigned value);
unsigned far cdecl AIL_channel_notes(HDRIVER driver, HSEQUENCE sequence,
   unsigned channel);
unsigned far cdecl AIL_beat_count(HDRIVER driver, HSEQUENCE sequence);
unsigned far cdecl AIL_measure_count(HDRIVER driver, HSEQUENCE sequence);
void far cdecl AIL_branch_index(HDRIVER driver, HSEQUENCE sequence, 
   unsigned marker_number);

void far cdecl AIL_send_channel_voice_message(HDRIVER driver, unsigned status, 
   unsigned data_1, unsigned data_2);
void far cdecl AIL_send_sysex_message(HDRIVER driver, unsigned addr_a, 
   unsigned addr_b, unsigned addr_c, void far *data, unsigned size, 
   unsigned delay);
void far cdecl AIL_write_display(HDRIVER driver, char far *string);
void far cdecl AIL_install_callback(HDRIVER driver, 
   void (far cdecl *callback_fn)());
void far cdecl AIL_cancel_callback(HDRIVER driver);

unsigned far cdecl AIL_lock_channel(HDRIVER driver);
void far cdecl AIL_map_sequence_channel(HDRIVER driver, HSEQUENCE sequence,
   unsigned sequence_channel, unsigned physical_channel);
unsigned far cdecl AIL_true_sequence_channel(HDRIVER driver, HSEQUENCE sequence,
   unsigned sequence_channel);
void far cdecl AIL_release_channel(HDRIVER driver, unsigned channel);

#ifdef __cplusplus
}
#endif
   
#endif
