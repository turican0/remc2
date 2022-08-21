//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   SOUNDFX.H                                                           лл
//лл                                                                       лл
//лл   Sample AIL MIDI sound effects shell header                          лл
//лл                                                                       лл
//лл   V1.00 of 14-Aug-91                                                  лл
//лл                                                                       лл
//лл   C source compatible with Turbo C++ v1.0 or later                    лл
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

typedef int HEFFECT;       // handle to sound effect

#define EFFECT_FREE 0      // values for get_sound_effect_status()
#define EFFECT_PLAYING 1

void init_sfx(void);       // called at application startup
void shutdown_sfx(void);   // called at application shutdown

//
// play_sound_effect(): Plays a single MIDI note "note" lasting 
// "ms_duration" milliseconds on driver "driver," with a MIDI Program 
// Change number "program," a Pan Controller value "panpot," and a 
// relative volume "volume."  Returns a handle to the sound effect 
// in progress "HEFFECT," which may be used with the remaining 
// functions to monitor or interrupt the sound effect.  The "note," 
// "program," "panpot," and "volume" values should be given in 
// standard MIDI units (0-127).
//

HEFFECT play_sound_effect(HDRIVER driver, unsigned program, unsigned note, 
   unsigned volume, unsigned panpot, int ms_duration);

//
// get_sound_effect_status(): Returns the status of a sound effect 
// handle provided by a previous play_sound_effect() call.  
// (See #defines above.)
//

unsigned get_sound_effect_status(HEFFECT effect);

//
// get_sound_effect_channel(): Returns the MIDI channel number 
// allocated for a given sound effect.  This may be used with the 
// AIL_send_channel_voice_message() function to alter the MIDI 
// controllers governing the sound effect.
//

unsigned get_sound_effect_channel(HEFFECT effect);

//
// stop_sound_effect(): Silences a given sound effect, and releases its 
// MIDI channel, without waiting for its duration to expire.
//

void stop_sound_effect(HEFFECT effect);
