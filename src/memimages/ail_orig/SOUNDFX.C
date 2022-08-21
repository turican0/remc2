//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   SOUNDFX.C                                                           лл
//лл                                                                       лл
//лл   Sample AIL MIDI sound effects shell                                 лл
//лл                                                                       лл
//лл   V1.00 of 14-Aug-91                                                  лл
//лл                                                                       лл
//лл   Project: IBM Audio Interface Library                                лл
//лл    Author: John Miles                                                 лл
//лл                                                                       лл
//лл   C source compatible with Turbo C++ v1.0 or later                    лл
//лл                                                                       лл
//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   These subroutines, prototyped in soundfx.h,                         лл
//лл   illustrate a practical, efficient technique for the                 лл
//лл   production of MIDI-based sound effects with the                     лл
//лл   Audio Interface Library.                                            лл
//лл                                                                       лл
//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   soundfx.obj: soundfx.c soundfx.h ail.h                              лл
//лл      bcc -ml -c -v soundfx.c                                          лл
//лл                                                                       лл
//лл   Caution: Do not overlay!                                            лл
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

#include "ail.h"        // Audio Interface Library API header file
#include "soundfx.h"    // SOUNDFX function header file

#define NOTE_OFF 0x80   // MIDI message equates
#define NOTE_ON 0x90
#define CTRL_CHG 0xB0
#define  VOLUME 7
#define  PANPOT 10
#define PRG_CHG 0xC0
#define PITCH_WHEEL 0xE0

#define NEFFECTS 8     // Size of sound effects list

HTIMER server;
HDRIVER effect_drivers[NEFFECTS];
unsigned effect_channels[NEFFECTS];
int effect_duration[NEFFECTS];
int effect_handles[NEFFECTS];

unsigned active_effects;

/***************************************************************/
//
// Sound effect timer callback function -- do not call directly
//

void timer_callback(void)
{
   int i;

   //
   // Exit quickly if no sound effect handles are active
   //

   if (!active_effects) return;

   //
   // Subtract 10 ms. from durations of all active sound effects; 
   // stop sound effect when its duration expires by releasing its 
   // channel and handle 
   //
   // (OK to call AIL functions from Process Services callbacks)
   //

   for (i=0;i<NEFFECTS;i++)
      if (effect_handles[i] != -1)
         if ((effect_duration[i] -= 10) <= 0)
            {
            effect_handles[i] = -1;
            AIL_release_channel(effect_drivers[i],effect_channels[i]);
            --active_effects;                              
            }
}

/***************************************************************/
//
// Initialize sound effects shell -- must be called before any
// other functions in this module
//

void init_sfx(void)
{
   int i,j;

   //
   // Mark all sound effect handles "available"; initialize list size
   //

   for (i=0;i<NEFFECTS;i++)
      effect_handles[i] = -1;

   active_effects = 0;

   //
   // Register a timer function; set up for 10-millisecond (100 Hz.) 
   // callback intervals
   //

   server=AIL_register_timer(timer_callback);
   AIL_set_timer_period(server,10000L);
   AIL_start_timer(server);
}

/***************************************************************/
//
// Shut down sound effects shell (normally done before 
// application terminates)
//

void shutdown_sfx(void)
{
   int i;

   //
   // Release sound effect service timer and any locked channels
   //

   AIL_release_timer_handle(server);

   for (i=0;i<NEFFECTS;i++)
      if (effect_handles[i] != -1)
         AIL_release_channel(effect_drivers[i],effect_channels[i]);
}

/***************************************************************/

//
// Start sound effect with specified program change #, note #, MIDI 
// volume & panpot, and duration in milliseconds; return handle to 
// sound effect (required by other functions)
//

HEFFECT play_sound_effect(HDRIVER driver, unsigned program, unsigned
   note, unsigned volume, unsigned panpot, int ms_duration)
   
{
   int i,ch;

   //
   // Find a free sound effect handle; return -1 if no handles 
   // available
   //

   for (i=0;i<NEFFECTS;i++)
      if (effect_handles[i] == -1) break;

   if (i==NEFFECTS) return -1;

   //
   // Lock a MIDI channel for use by the sound effect
   //

   effect_channels[i] = ch = AIL_lock_channel(driver);
   printf("Locked channel %u\n",ch);

   //
   // Initialize housekeeping arrays and all MIDI controllers not set
   // by AIL_lock_channel()
   //

   effect_drivers[i] = driver;
   effect_duration[i] = ms_duration;

   AIL_send_channel_voice_message(driver,PRG_CHG | ch,program,0);
   AIL_send_channel_voice_message(driver,PITCH_WHEEL | ch,0,64);
   AIL_send_channel_voice_message(driver,CTRL_CHG | ch,PANPOT,panpot);
   AIL_send_channel_voice_message(driver,CTRL_CHG | ch,VOLUME,volume);

   //
   // Start the sound effect with a MIDI Note On event
   //

   AIL_send_channel_voice_message(driver,NOTE_ON | ch,note,127);

   //
   // Assign handle to sound effect, increment counter of active sound
   // effects, and return the handle to the caller
   //

   ++active_effects;
   effect_handles[i] = i;

   return i;
}
   
/***************************************************************/
//
// Return: 0 if sound effect expired or unused; 1 if sound effect 
// playing
//

unsigned get_sound_effect_status(HEFFECT effect)
{
   return (effect_handles[effect] != -1);
}
                                    
/***************************************************************/
//
// Return MIDI channel used by sound effect; may be used by 
// application to adjust volume, panpot, pitch, etc.
//

unsigned get_sound_effect_channel(HEFFECT effect)
{
   return effect_channels[effect];
}

/***************************************************************/
//
// Force sound effect off (without waiting for its duration to 
// expire)
//

void stop_sound_effect(HEFFECT effect)
{
   if (get_sound_effect_status(effect)==EFFECT_FREE) return;

   effect_handles[effect] = -1;
   AIL_release_channel(effect_drivers[effect],effect_channels[effect]);
   --active_effects;
}   

