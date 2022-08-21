//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   TAIL.C                                                              лл
//лл                                                                       лл
//лл   Wrapper module for AIL application testing/debugging                лл
//лл                                                                       лл
//лл   V1.00 of 29-Jan-92                                                  лл
//лл                                                                       лл
//лл   Project: IBM Audio Interface Library                                лл
//лл    Author: John Miles                                                 лл
//лл                                                                       лл
//лл   C source compatible with Turbo C++ v1.0 or later                    лл
//лл                                                                       лл
//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   tail.obj: tail.c                                                    лл
//лл      bcc -ml -c -v tail.c                                             лл
//лл                                                                       лл
//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   Instructions for use:                                               лл
//лл                                                                       лл
//лл   1) Link TAIL.OBJ with the application, as well as the               лл
//лл      usual AIL.OBJ module.                                            лл
//лл                                                                       лл
//лл   2) Replace EVERY call in the ENTIRE application which               лл
//лл      begins with the prefix "AIL_" with identical calls               лл
//лл      using the prefix "TAIL_".                                        лл
//лл                                                                       лл
//лл      (However, do NOT call any TAIL_ function from a                  лл
//лл      timer callback function or other interrupt                       лл
//лл      handler.)                                                        лл
//лл                                                                       лл
//лл   3) In every application module, replace all                         лл
//лл      directives of the form INCLUDE "AIL.H" with                      лл
//лл      INCLUDE "TAIL.H".                                                лл
//лл                                                                       лл
//лл   4) Execute the application, attempting to reproduce                 лл
//лл      the failure.                                                     лл
//лл                                                                       лл
//лл   5) Stop the application normally as soon as the                     лл
//лл      failure occurs.  The root directory of the                       лл
//лл      C: drive should contain a file called AILxxx.LOG,                лл
//лл      which is actually a time-stamped log of all                      лл
//лл      function calls made to the Audio Interface                       лл
//лл      Library.  (xxx represents a unique number from 000               лл
//лл      to 999, depending on how many AILxxx.LOG files                   лл
//лл      are already in the C: root directory.)                           лл
//лл                                                                       лл
//лл   6) Examine the AILxxx.LOG file for evidence of                      лл
//лл      application-level bugs and inconsistencies.  If                  лл
//лл      you are unable to find the problem by examining                  лл
//лл      the parameters and results of your application's                 лл
//лл      AIL calls, please contact Miles Design at the                    лл
//лл      number below for further assistance.                             лл
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

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <alloc.h>
#include <time.h>
#include <string.h>

#include "ail.h"        // Audio Interface Library API function header

const char TAILVERSION[] = "1.00";

FILE *tscript;

volatile unsigned vhours;
volatile unsigned vmins;
volatile unsigned vsecs;
volatile unsigned vcentisecs;

char *SEQ[] = {"SEQ_STOPPED","SEQ_PLAYING","SEQ_DONE"};
char *DAC[] = {"DAC_STOPPED","DAC_PAUSED","DAC_PLAYING","DAC_DONE"};

int tstarted = 0;

/***********************************************************************/
HTIMER stamp;

void far timestamp_handler(void)
{
   if ((vhours == 99) & (vmins == 59) && (vsecs == 59) && (vcentisecs == 99))
      return;

   if (++vcentisecs == 100)
      {
      vcentisecs = 0;
      if (++vsecs == 60)
         {
         vsecs = 0;
         if (++vmins == 60)
            {
            vmins = 0;
            ++vhours;
            }
         }
      }
}

void init_timestamps(void)
{
   vhours=vmins=vsecs=vcentisecs=0;

   stamp = AIL_register_timer(timestamp_handler);
   AIL_set_timer_frequency(stamp,100L);
   AIL_start_timer(stamp);
}

void shutdown_timestamps(void)
{
   AIL_release_timer_handle(stamp);
}

void write_timestamp(FILE *out)
{
   fprintf(out,"[%.02u:%.02u:%.02u.%.02u]",vhours,vmins,vsecs,vcentisecs);
}

/********************/
/*                  */
/* Process services */
/*                  */
/********************/

void far cdecl TAIL_startup(void)
{
   FILE *in;
   static char try_fn[] = "C:\\AIL000.LOG";
   unsigned int filenum;
   long elapstime;
   struct tm *adjtime;
   static char loctime[32];

   if (tstarted)
      {
      fprintf(stderr,"*** ERROR: Redundant call to AIL_startup() ***\a\r\n");
      exit(1);
      }

   filenum = 0;
   while ((in = fopen(try_fn,"rb")) != NULL)
      {
      fclose(in);
      ++filenum;
      sprintf(&try_fn[6],"%.03u",filenum);
      try_fn[9] = '.';
      if (filenum == 1000) break;
      }

   tscript = fopen(try_fn,"w+b");

   time(&elapstime);
   adjtime = localtime(&elapstime);
   strcpy(loctime,asctime(adjtime));
   loctime[24] = 0;

   fprintf(tscript,"-------------------------------------------------------------------------------\r\n");
   fprintf(tscript,"Audio Interface Library application usage script generated by TAIL v%s\r\n",TAILVERSION);
   fprintf(tscript,"Start time: %s\r\n",loctime);
   fprintf(tscript,"-------------------------------------------------------------------------------\r\n\r\n");

   AIL_startup();

   init_timestamps();
   write_timestamp(tscript);

   fprintf(tscript," AIL_startup()");

   fprintf(tscript,"\r\n");
   tstarted = 1;
}

void far cdecl TAIL_shutdown(char far *signoff_msg)
{
   if (!tstarted)
      {
      fprintf(stderr,"*** ERROR: AIL_shutdown() called before AIL_startup() ***\a\r\n");
      exit(1);
      }

   write_timestamp(tscript);
   fprintf(tscript," AIL_shutdown(\"%s\")",signoff_msg);

   shutdown_timestamps();
   AIL_shutdown(signoff_msg);

   fprintf(tscript,"\r\n");
   if (tscript != stdout) fclose(tscript);

   tstarted = 0;
}

HTIMER far cdecl TAIL_register_timer(void (far cdecl *callback_fn)())
{
   HTIMER timer;

   write_timestamp(tscript);
   fprintf(tscript," AIL_register_timer(%Fp)",callback_fn);

   timer = AIL_register_timer(callback_fn);

   fprintf(tscript," = %d\r\n",timer);

   return timer;
}

void far cdecl TAIL_set_timer_period(HTIMER timer, unsigned long microseconds)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_set_timer_period(%d,%lu)",timer,microseconds);

   AIL_set_timer_period(timer,microseconds);

   fprintf(tscript,"\r\n");
}

void far cdecl TAIL_set_timer_frequency(HTIMER timer, unsigned long hertz)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_set_timer_frequency(%d,%lu)",timer,hertz);

   AIL_set_timer_frequency(timer,hertz);

   fprintf(tscript,"\r\n");
}

void far cdecl TAIL_set_timer_divisor(HTIMER timer, unsigned PIT_divisor)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_set_timer_divisor(%d,%u)",timer,PIT_divisor);

   AIL_set_timer_divisor(timer,PIT_divisor);

   fprintf(tscript,"\r\n");
}

unsigned far cdecl TAIL_interrupt_divisor(void)
{
   unsigned result;

   write_timestamp(tscript);
   fprintf(tscript," AIL_interrupt_divisor()");

   result = AIL_interrupt_divisor();

   fprintf(tscript," = %u\r\n",result);

   return result;
}

void far cdecl TAIL_start_timer(HTIMER timer)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_start_timer(%d)",timer);

   AIL_start_timer(timer);

   fprintf(tscript,"\r\n");
}

void far cdecl TAIL_start_all_timers(void)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_start_all_timers()");

   AIL_start_all_timers();

   fprintf(tscript,"\r\n");
}

void far cdecl TAIL_stop_timer(HTIMER timer)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_stop_timer(%d)",timer);

   AIL_stop_timer(timer);

   fprintf(tscript,"\r\n");
}

void far cdecl TAIL_stop_all_timers(void)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_stop_all_timers()");

   AIL_stop_all_timers();

   fprintf(tscript,"\r\n");
}

void far cdecl TAIL_release_timer_handle(HTIMER timer)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_release_timer_handle(%d)",timer);

   AIL_release_timer_handle(timer);

   fprintf(tscript,"\r\n");
}

void far cdecl TAIL_release_all_timers(void)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_release_all_timers()");

   AIL_release_all_timers();

   fprintf(tscript,"\r\n");
}


/*************************/
/*                       */
/* Installation services */
/*                       */
/*************************/

HDRIVER far cdecl TAIL_register_driver(void far *driver_base_addr)
{
   HDRIVER result;

   write_timestamp(tscript);
   fprintf(tscript," AIL_register_driver(%Fp)",driver_base_addr);

   result = AIL_register_driver(driver_base_addr);

   fprintf(tscript," = %d\r\n",result);

   return result;
}

void far cdecl TAIL_release_driver_handle(HDRIVER driver)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_release_driver_handle(%d)",driver);

   AIL_release_driver_handle(driver);

   fprintf(tscript,"\r\n");
}

drvr_desc far * cdecl far TAIL_describe_driver(HDRIVER driver)
{
   drvr_desc far *t;

   write_timestamp(tscript);
   fprintf(tscript," AIL_describe_driver(%d)",driver);

   t = AIL_describe_driver(driver);

   fprintf(tscript," = %Fp",t);
   fprintf(tscript," -> {%u,%u,\"%s\",%Fp,%XH,%d,%d,%d,%d,%u}",
      t->min_API_version,t->drvr_type,&t->data_suffix[0],t->dev_name_table,
      t->default_IO,t->default_IRQ,t->default_DMA,t->default_DRQ,
      t->service_rate,t->display_size);

   fprintf(tscript,"\r\n");

   return t;
}

unsigned far cdecl TAIL_detect_device(HDRIVER driver, unsigned IO_addr, 
    unsigned IRQ, unsigned DMA, unsigned DRQ)
{
   unsigned result;

   write_timestamp(tscript);
   fprintf(tscript," AIL_detect_device(%d,%XH,%d,%d,%d)",driver,IO_addr,
      IRQ,DMA,DRQ);

   result = AIL_detect_device(driver,IO_addr,IRQ,DMA,DRQ);

   fprintf(tscript," = %u",result);
   fprintf(tscript,"\r\n");

   return result;
}

void far cdecl TAIL_init_driver(HDRIVER driver, unsigned IO_addr, 
    unsigned IRQ, unsigned DMA, unsigned DRQ)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_init_driver(%d,%XH,%d,%d,%d)",driver,IO_addr,
      IRQ,DMA,DRQ);

   AIL_init_driver(driver,IO_addr,IRQ,DMA,DRQ);

   fprintf(tscript,"\r\n");
}

void far cdecl TAIL_shutdown_driver(HDRIVER driver, char far *signoff_msg)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_shutdown_driver(%d,\"%s\")",driver,signoff_msg);

   AIL_shutdown_driver(driver,signoff_msg);

   fprintf(tscript,"\r\n");
}

/********************************/
/*                              */
/* Digital performance services */
/*                              */
/********************************/

unsigned far cdecl TAIL_index_VOC_block(HDRIVER driver, void far *VOC_file, unsigned 
   block_marker, sound_buff far *buff)
{
   unsigned result;

   write_timestamp(tscript);
   fprintf(tscript," AIL_index_VOC_block(%d,%Fp,%u,%Fp)",
      driver,VOC_file,block_marker,buff);

   result = AIL_index_VOC_block(driver,VOC_file,block_marker,buff);

   fprintf(tscript," = %u",result);
   fprintf(tscript,"\r\n");

   return result;
}

void far cdecl TAIL_register_sound_buffer(HDRIVER driver, unsigned buffer_num,
   sound_buff far *buff)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_register_sound_buffer(%d,%u,%Fp -> {%u,%u,%Fp,%lu})",
      driver,buffer_num,buff,buff->pack_type,buff->sample_rate,buff->data,
      buff->len);

   AIL_register_sound_buffer(driver,buffer_num,buff);

   fprintf(tscript,"\r\n");
}

unsigned far cdecl TAIL_sound_buffer_status(HDRIVER driver, unsigned buffer_num)
{
   unsigned result;

   write_timestamp(tscript);
   fprintf(tscript," AIL_sound_buffer_status(%d,%u)",driver,buffer_num);

   result = AIL_sound_buffer_status(driver,buffer_num);

   fprintf(tscript," = %u (%s)",result,DAC[result]);
   fprintf(tscript,"\r\n");
   return result;
}


void far cdecl TAIL_play_VOC_file(HDRIVER driver, void far *VOC_file, int
   block_marker)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_play_VOC_file(%d,%Fp,%d)",driver,VOC_file,
      block_marker);

   AIL_play_VOC_file(driver,VOC_file,block_marker);

   fprintf(tscript,"\r\n");
}

unsigned far cdecl TAIL_VOC_playback_status(HDRIVER driver)
{
   unsigned result;

   write_timestamp(tscript);
   fprintf(tscript," AIL_VOC_playback_status(%d)",driver);

   result = AIL_VOC_playback_status(driver);

   fprintf(tscript," = %u (%s)",result,DAC[result]);
   fprintf(tscript,"\r\n");
   return result;
}


void far cdecl TAIL_start_digital_playback(HDRIVER driver)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_start_digital_playback(%d)",driver);

   AIL_start_digital_playback(driver);

   fprintf(tscript,"\r\n");
}

void far cdecl TAIL_stop_digital_playback(HDRIVER driver)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_stop_digital_playback(%d)",driver);

   AIL_stop_digital_playback(driver);

   fprintf(tscript,"\r\n");
}

void far cdecl TAIL_pause_digital_playback(HDRIVER driver)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_pause_digital_playback(%d)",driver);

   AIL_pause_digital_playback(driver);

   fprintf(tscript,"\r\n");
}

void far cdecl TAIL_resume_digital_playback(HDRIVER driver)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_resume_digital_playback(%d)",driver);

   AIL_resume_digital_playback(driver);

   fprintf(tscript,"\r\n");
}

void far cdecl TAIL_set_digital_playback_volume(HDRIVER driver, unsigned volume)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_set_digital_playback_volume(%d,%u)",driver,volume);

   AIL_set_digital_playback_volume(driver,volume);

   fprintf(tscript,"\r\n");
}

unsigned far cdecl TAIL_digital_playback_volume(HDRIVER driver)
{
   unsigned result;

   write_timestamp(tscript);
   fprintf(tscript," AIL_digital_playback_volume(%d)",driver);

   result = AIL_digital_playback_volume(driver);

   fprintf(tscript," = %u",result);
   fprintf(tscript,"\r\n");
   return result;
}

void far cdecl TAIL_set_digital_playback_panpot(HDRIVER driver, unsigned panpot)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_set_digital_playback_panpot(%d,%u)",driver,panpot);

   AIL_set_digital_playback_panpot(driver,panpot);

   fprintf(tscript,"\r\n");
}

unsigned far cdecl TAIL_digital_playback_panpot(HDRIVER driver)
{
   unsigned result;

   write_timestamp(tscript);
   fprintf(tscript," AIL_digital_playback_panpot(%d)",driver);

   result = AIL_digital_playback_panpot(driver);

   fprintf(tscript," = %u",result);
   fprintf(tscript,"\r\n");
   return result;
}

/******************************/
/*                            */
/* XMIDI performance services */
/*                            */
/******************************/

unsigned far cdecl TAIL_state_table_size(HDRIVER driver)
{
   unsigned result;

   write_timestamp(tscript);
   fprintf(tscript," AIL_state_table_size(%d)",driver);

   result = AIL_state_table_size(driver);

   fprintf(tscript," = %u",result);
   fprintf(tscript,"\r\n");
   return result;
}

HSEQUENCE far cdecl TAIL_register_sequence(HDRIVER driver, void far *FORM_XMID,
   unsigned sequence_num, void far *state_table, void far *controller_table)
{
   HSEQUENCE result;

   write_timestamp(tscript);
   fprintf(tscript," AIL_register_sequence(%d,%Fp,%u,%Fp,%Fp)",driver,
      FORM_XMID,sequence_num,state_table,controller_table);

   result = AIL_register_sequence(driver,FORM_XMID,sequence_num,state_table,
      controller_table);

   fprintf(tscript," = %d",result);
   fprintf(tscript,"\r\n");
   return result;
}

void far cdecl TAIL_release_sequence_handle(HDRIVER driver, HSEQUENCE sequence)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_release_sequence_handle(%d,%d)",driver,sequence);

   AIL_release_sequence_handle(driver,sequence);

   fprintf(tscript,"\r\n");
}

unsigned far cdecl TAIL_default_timbre_cache_size(HDRIVER driver)
{
   unsigned result;

   write_timestamp(tscript);
   fprintf(tscript," AIL_default_timbre_cache_size(%d)",driver);

   result = AIL_default_timbre_cache_size(driver);

   fprintf(tscript," = %u",result);
   fprintf(tscript,"\r\n");
   return result;
}

void far cdecl TAIL_define_timbre_cache(HDRIVER driver, void far *cache_addr,
   unsigned cache_size)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_define_timbre_cache(%d,%Fp,%u)",driver,cache_addr,
      cache_size);

   AIL_define_timbre_cache(driver,cache_addr,cache_size);

   fprintf(tscript,"\r\n");
}

unsigned far cdecl TAIL_timbre_request(HDRIVER driver, HSEQUENCE sequence)
{
   unsigned result;

   write_timestamp(tscript);
   fprintf(tscript," AIL_timbre_request(%d,%d)",driver,sequence);

   result = AIL_timbre_request(driver,sequence);

   fprintf(tscript," = %u (bank %d, patch %d)",result,result/256,result%256);
   fprintf(tscript,"\r\n");
   return result;
}

unsigned far cdecl TAIL_timbre_status(HDRIVER driver, int bank, int patch)
{
   unsigned result;

   write_timestamp(tscript);
   fprintf(tscript," AIL_timbre_status(%d,%d,%d)",driver,bank,patch);

   result = AIL_timbre_status(driver,bank,patch);

   fprintf(tscript," = %u",result);
   fprintf(tscript,"\r\n");
   return result;
}

void far cdecl TAIL_install_timbre(HDRIVER driver, int bank, int patch, 
   void far *src_addr)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_install_timbre(%d,%d,%d,%Fp)",driver,bank,patch,
      src_addr);

   AIL_install_timbre(driver,bank,patch,src_addr);

   fprintf(tscript,"\r\n");
}

void far cdecl TAIL_protect_timbre(HDRIVER driver, int bank, int patch)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_protect_timbre(%d,%d,%d)",driver,bank,patch);

   AIL_protect_timbre(driver,bank,patch);

   fprintf(tscript,"\r\n");
}

void far cdecl TAIL_unprotect_timbre(HDRIVER driver, int bank, int patch)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_unprotect_timbre(%d,%d,%d)",driver,bank,patch);

   AIL_unprotect_timbre(driver,bank,patch);

   fprintf(tscript,"\r\n");
}

void far cdecl TAIL_start_sequence(HDRIVER driver, HSEQUENCE sequence)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_start_sequence(%d,%d)",driver,sequence);

   AIL_start_sequence(driver,sequence);

   fprintf(tscript,"\r\n");
}

void far cdecl TAIL_stop_sequence(HDRIVER driver, HSEQUENCE sequence)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_stop_sequence(%d,%d)",driver,sequence);

   AIL_stop_sequence(driver,sequence);

   fprintf(tscript,"\r\n");
}

void far cdecl TAIL_resume_sequence(HDRIVER driver, HSEQUENCE sequence)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_resume_sequence(%d,%d)",driver,sequence);

   AIL_resume_sequence(driver,sequence);

   fprintf(tscript,"\r\n");
}

unsigned far cdecl TAIL_sequence_status(HDRIVER driver, HSEQUENCE sequence)
{
   unsigned result;

   write_timestamp(tscript);
   fprintf(tscript," AIL_sequence_status(%d,%d)",driver,sequence);

   result = AIL_sequence_status(driver,sequence);

   fprintf(tscript," = %u (%s)",result,SEQ[result]);
   fprintf(tscript,"\r\n");
   return result;
}

unsigned far cdecl TAIL_relative_volume(HDRIVER driver, HSEQUENCE sequence)
{
   unsigned result;

   write_timestamp(tscript);
   fprintf(tscript," AIL_relative_volume(%d,%d)",driver,sequence);

   result = AIL_relative_volume(driver,sequence);

   fprintf(tscript," = %u%%",result);
   fprintf(tscript,"\r\n");
   return result;
}

unsigned far cdecl TAIL_relative_tempo(HDRIVER driver, HSEQUENCE sequence)
{
   unsigned result;

   write_timestamp(tscript);
   fprintf(tscript," AIL_relative_tempo(%d,%d)",driver,sequence);

   result = AIL_relative_tempo(driver,sequence);

   fprintf(tscript," = %u%%",result);
   fprintf(tscript,"\r\n");
   return result;
}

void far cdecl TAIL_set_relative_volume(HDRIVER driver, HSEQUENCE sequence,
   unsigned percent, unsigned milliseconds)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_set_relative_volume(%d,%d,%u%%,%u)",driver,sequence,
      percent,milliseconds);

   AIL_set_relative_volume(driver,sequence,percent,milliseconds);

   fprintf(tscript,"\r\n");
}

void far cdecl TAIL_set_relative_tempo(HDRIVER driver, HSEQUENCE sequence,
   unsigned percent, unsigned milliseconds)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_set_relative_tempo(%d,%d,%u%%,%u)",driver,sequence,
      percent,milliseconds);

   AIL_set_relative_tempo(driver,sequence,percent,milliseconds);

   fprintf(tscript,"\r\n");
}

int far cdecl TAIL_controller_value(HDRIVER driver, HSEQUENCE sequence,
   unsigned channel, unsigned controller_num)
{
   int result;

   write_timestamp(tscript);
   fprintf(tscript," AIL_controller_value(%d,%d,%u,%u)",driver,sequence,
      channel,controller_num);

   result = AIL_controller_value(driver,sequence,channel,controller_num);

   fprintf(tscript," = %d",result);
   fprintf(tscript,"\r\n");
   return result;
}

void far cdecl TAIL_set_controller_value(HDRIVER driver, HSEQUENCE sequence,
   unsigned channel, unsigned controller_num, unsigned value)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_set_controller_value(%d,%d,%u,%u,%u)",driver,
      sequence,channel,controller_num,value);

   AIL_set_controller_value(driver,sequence,channel,controller_num,value);

   fprintf(tscript,"\r\n");
}

unsigned far cdecl TAIL_channel_notes(HDRIVER driver, HSEQUENCE sequence,
   unsigned channel)
{
   unsigned result;

   write_timestamp(tscript);
   fprintf(tscript," AIL_channel_notes(%d,%d,%u)",driver,sequence,channel);

   result = AIL_channel_notes(driver,sequence,channel);

   fprintf(tscript," = %u",result);
   fprintf(tscript,"\r\n");
   return result;
}

unsigned far cdecl TAIL_beat_count(HDRIVER driver, HSEQUENCE sequence)
{
   unsigned result;

   write_timestamp(tscript);
   fprintf(tscript," AIL_beat_count(%d,%d)",driver,sequence);

   result = AIL_beat_count(driver,sequence);

   fprintf(tscript," = %u",result);
   fprintf(tscript,"\r\n");
   return result;
}

unsigned far cdecl TAIL_measure_count(HDRIVER driver, HSEQUENCE sequence)
{
   unsigned result;

   write_timestamp(tscript);
   fprintf(tscript," AIL_measure_count(%d,%d)",driver,sequence);

   result = AIL_measure_count(driver,sequence);

   fprintf(tscript," = %u",result);
   fprintf(tscript,"\r\n");
   return result;
}

void far cdecl TAIL_branch_index(HDRIVER driver, HSEQUENCE sequence, 
   unsigned marker_number)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_branch_index(%d,%d,%u)",driver,sequence,marker_number);

   AIL_branch_index(driver,sequence,marker_number);

   fprintf(tscript,"\r\n");
}


void far cdecl TAIL_send_channel_voice_message(HDRIVER driver, unsigned status, 
   unsigned data_1, unsigned data_2)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_send_channel_voice_message(%d,$%.02X,$%.02X,$%.02X)",
      driver,status,data_1,data_2);
      
   AIL_send_channel_voice_message(driver,status,data_1,data_2);

   fprintf(tscript,"\r\n");
}

void far cdecl TAIL_send_sysex_message(HDRIVER driver, unsigned addr_a, 
   unsigned addr_b, unsigned addr_c, void far *data, unsigned size, 
   unsigned delay)
{
   unsigned i,n;

   write_timestamp(tscript);
   fprintf(tscript," AIL_send_sysex_message(%d,$%.02X,$%.02X,$%.02X,%Fp,%u,%u)",
      driver,addr_a,addr_b,addr_c,data,size,delay);

   fprintf(tscript,"\r\n");

   AIL_send_sysex_message(driver,addr_a,addr_b,addr_c,data,size,delay);

   n = 0;
   while (size > 8)
      {
      fprintf(tscript,"     ");
      for (i=0;i<8;i++)
         {
         fprintf(tscript,"%.02X ",(unsigned) (*((unsigned char far *) data+n)));
         n++;
         }
      fprintf(tscript,"\r\n");
      size -= 8;
      };

   fprintf(tscript,"     ");
   while (size--)
      {
      fprintf(tscript,"%.02X ",(unsigned) (*((unsigned char far *) data+n)));
      n++;
      }

   fprintf(tscript,"\r\n");
}

void far cdecl TAIL_write_display(HDRIVER driver, char far *string)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_write_display(%d,\"%s\")",driver,string);

   AIL_write_display(driver,string);

   fprintf(tscript,"\r\n");
}

void far cdecl TAIL_install_callback(HDRIVER driver, 
   void (far cdecl *callback_fn)())
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_install_callback(%d,%Fp)",driver,callback_fn);

   AIL_install_callback(driver,callback_fn);

   fprintf(tscript,"\r\n");
}

void far cdecl TAIL_cancel_callback(HDRIVER driver)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_cancel_callback(%d)",driver);

   AIL_cancel_callback(driver);

   fprintf(tscript,"\r\n");
}

unsigned far cdecl TAIL_lock_channel(HDRIVER driver)
{
   unsigned result;

   write_timestamp(tscript);
   fprintf(tscript," AIL_lock_channel(%d)",driver);

   result = AIL_lock_channel(driver);

   fprintf(tscript," = %u",result);
   fprintf(tscript,"\r\n");
   return result;
}

void far cdecl TAIL_map_sequence_channel(HDRIVER driver, HSEQUENCE sequence,
   unsigned sequence_channel, unsigned physical_channel)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_map_sequence_channel(%d,%d,%u,%u)",driver,sequence,
      sequence_channel,physical_channel);

   AIL_map_sequence_channel(driver,sequence,sequence_channel,
      physical_channel);

   fprintf(tscript,"\r\n");
}

unsigned far cdecl TAIL_true_sequence_channel(HDRIVER driver, HSEQUENCE sequence,
   unsigned sequence_channel)
{
   unsigned result;

   write_timestamp(tscript);
   fprintf(tscript," AIL_true_sequence_channel(%d,%d,%u)",driver,sequence,
      sequence_channel);

   result = AIL_true_sequence_channel(driver,sequence,sequence_channel);

   fprintf(tscript," = %u",result);
   fprintf(tscript,"\r\n");
   return result;
}

void far cdecl TAIL_release_channel(HDRIVER driver, unsigned channel)
{
   write_timestamp(tscript);
   fprintf(tscript," AIL_release_channel(%d,%u)",driver,channel);

   AIL_release_channel(driver,channel);

   fprintf(tscript,"\r\n");
}
