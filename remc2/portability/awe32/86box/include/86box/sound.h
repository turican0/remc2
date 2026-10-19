#ifndef EMU_SOUND_H
#define EMU_SOUND_H
/* Harness stub. Values copied verbatim from upstream src/include/86box/sound.h. */
#define FREQ_44100  44100
#define FREQ_48000  48000
#define FREQ_49716  49716

#define SOUND_FREQ  FREQ_48000
#define SOUNDBUFLEN (SOUND_FREQ / 50)

#define MUSIC_FREQ  FREQ_49716
#define MUSICBUFLEN (MUSIC_FREQ / 36)

#define WT_FREQ     FREQ_44100
#define WTBUFLEN    (WT_FREQ / 45)

extern int music_pos_global;
extern int wavetable_pos_global;
#endif
