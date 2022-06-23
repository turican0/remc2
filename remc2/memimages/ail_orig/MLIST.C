//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   MLIST.C                                                             лл
//лл                                                                       лл
//лл   Standard MIDI file dump utility                                     лл
//лл                                                                       лл
//лл   V1.00 of 17-Mar-91                                                  лл
//лл   V1.01 of 07-Sep-91: New sysex dump formatting                       лл
//лл   V1.10 of 23-Oct-91: Roland sysex disassembly/                       лл
//лл                       Programmer's Reference features                 лл
//лл   V1.11 of  3-Feb-92: BC 3.0 const declarations fixed                 лл
//лл   V1.12 of 20-Jun-92: Undefined meta-events handled correctly         лл
//лл                                                                       лл
//лл   Project: IBM Audio Interface Library                                лл
//лл    Author: John Miles                                                 лл
//лл                                                                       лл
//лл   C source compatible with Turbo C++ v1.0 or later                    лл
//лл                                                                       лл
//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   mlist.obj: mlist.c gen.h                                            лл
//лл      bcc -ml -c -v mlist.c                                            лл
//лл                                                                       лл
//лл   mlist.exe: mlist.obj gen.lib                                        лл
//лл      tlink @mlist.lls                                                 лл
//лл                                                                       лл
//лл   Contents of MLIST.LLS:                                              лл
//лл     /c /v /x +                                                        лл
//лл     \bc\lib\c0l.obj +                                                 лл
//лл     mlist, +                                                          лл
//лл     mlist.exe, +                                                      лл
//лл     mlist.map, +                                                      лл
//лл     \bc\lib\cl.lib gen.lib                                            лл
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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <alloc.h>
#include <ctype.h>

#include "gen.h"

const char VERSION[] = "1.12";

char *buffer, *bufpnt, *bufend;
unsigned long file_length;
unsigned offset;

unsigned arg_mt, arg_ma, arg_mu;

unsigned char sysex_buffer[256];
                      
/***************************************************************/
#define TIMBRE 0
#define PATCH_TEMP 1
#define RHYTHMS 2
#define PATCH 3
#define SYSTEM 4
#define DISPLAY 5
#define WRITE_REQUEST 6
#define ALL_PARMS_RESET 7

#define T_A 0
#define T_N 1
#define T_C 2
#define T_NONE 3

typedef struct
{
   char area_name[128];
   char parm_name[128];
   unsigned type;
}
addr_desc;


typedef struct
{
   unsigned MSB;
   unsigned KSB;
   unsigned LSB;
}
sysex_addr;

typedef struct
{
   char *name;
   unsigned type;
}
entry;

typedef struct
{
   char *name;
   sysex_addr base;
   unsigned base_size;
   unsigned base_num;
   unsigned group_count;
   const entry *items;
}
Roland_area;

const entry patch_temp[] =
{
   {"TIMBRE GROUP",T_N},
   {"TIMBRE NUMBER",T_N},
   {"KEY SHIFT",T_N},
   {"FINE TUNE",T_N},
   {"BENDER RANGE",T_N},
   {"ASSIGN MODE",T_N},
   {"REVERB SWITCH",T_N},
   {"dummy (ignored if received) (offset $00 $07)",T_N},
   {"OUTPUT LEVEL",T_N},
   {"PANPOT",T_N},
   {"dummy (ignored if received) (offset $00 $0A)",T_N},
   {"dummy (ignored if received) (offset $00 $0B)",T_N},
   {"dummy (ignored if received) (offset $00 $0C)",T_N},
   {"dummy (ignored if received) (offset $00 $0D)",T_N},
   {"dummy (ignored if received) (offset $00 $0E)",T_N},
   {"dummy (ignored if received) (offset $00 $0F)",T_N}
};

const entry patch[] =
{
   {"TIMBRE GROUP",T_N},
   {"TIMBRE NUMBER",T_N},
   {"KEY SHIFT",T_N},
   {"FINE TUNE",T_N},
   {"BENDER RANGE",T_N},
   {"ASSIGN MODE",T_N},
   {"REVERB SWITCH",T_N},
   {"dummy (offset $00 $07)",T_N}
};

const entry rhythm_setup[] =
{
   {"TIMBRE",T_N},
   {"OUTPUT LEVEL",T_N},
   {"PANPOT",T_N},
   {"REVERB SWITCH",T_N}
};

const entry system_area[] =
{
   {"MASTER TUNE",T_N},
   {"REVERB MODE",T_N},
   {"REVERB TIME",T_N},
   {"REVERB LEVEL",T_N},
   {"PARTIAL RESERVE (Part 1)",T_N},
   {"PARTIAL RESERVE (Part 2)",T_N},
   {"PARTIAL RESERVE (Part 3)",T_N},
   {"PARTIAL RESERVE (Part 4)",T_N},
   {"PARTIAL RESERVE (Part 5)",T_N},
   {"PARTIAL RESERVE (Part 6)",T_N},
   {"PARTIAL RESERVE (Part 7)",T_N},
   {"PARTIAL RESERVE (Part 8)",T_N},
   {"PARTIAL RESERVE (Part R)",T_N},
   {"MIDI CHANNEL (Part 1)",T_C},
   {"MIDI CHANNEL (Part 2)",T_C},
   {"MIDI CHANNEL (Part 3)",T_C},
   {"MIDI CHANNEL (Part 4)",T_C},
   {"MIDI CHANNEL (Part 5)",T_C},
   {"MIDI CHANNEL (Part 6)",T_C},
   {"MIDI CHANNEL (Part 7)",T_C},
   {"MIDI CHANNEL (Part 8)",T_C},
   {"MIDI CHANNEL (Part R)",T_C},
   {"MASTER VOLUME",T_N}
};

const entry display[] =
{
   {"MT-32 LCD Display (character 1)",T_A},
   {"MT-32 LCD Display (character 2)",T_A},
   {"MT-32 LCD Display (character 3)",T_A},
   {"MT-32 LCD Display (character 4)",T_A},
   {"MT-32 LCD Display (character 5)",T_A},
   {"MT-32 LCD Display (character 6)",T_A},
   {"MT-32 LCD Display (character 7)",T_A},
   {"MT-32 LCD Display (character 8)",T_A},
   {"MT-32 LCD Display (character 9)",T_A},
   {"MT-32 LCD Display (character 10)",T_A},
   {"MT-32 LCD Display (character 11)",T_A},
   {"MT-32 LCD Display (character 12)",T_A},
   {"MT-32 LCD Display (character 13)",T_A},
   {"MT-32 LCD Display (character 14)",T_A},
   {"MT-32 LCD Display (character 15)",T_A},
   {"MT-32 LCD Display (character 16)",T_A},
   {"MT-32 LCD Display (character 17)",T_A},
   {"MT-32 LCD Display (character 18)",T_A},
   {"MT-32 LCD Display (character 19)",T_A},
   {"MT-32 LCD Display (character 20)",T_A}
};

const entry write_request[] =
{
   {"Timbre Write (part 1)",T_N},
   {"(Internal) (Offset $01)",T_N},
   {"Timbre Write (part 2)",T_N},
   {"(Internal) (Offset $03)",T_N},
   {"Timbre Write (part 3)",T_N},
   {"(Internal) (Offset $05)",T_N},
   {"Timbre Write (part 4)",T_N},
   {"(Internal) (Offset $07)",T_N},
   {"Timbre Write (part 5)",T_N},
   {"(Internal) (Offset $09)",T_N},
   {"Timbre Write (part 6)",T_N},
   {"(Internal) (Offset $0B)",T_N},
   {"Timbre Write (part 7)",T_N},
   {"(Internal) (Offset $0D)",T_N},
   {"Timbre Write (part 8)",T_N},
   {"(Internal) (Offset $0F)",T_N},
   {"Patch Write (part 1)",T_N},
   {"(Internal) (Offset $01)",T_N},
   {"Patch Write (part 2)",T_N},
   {"(Internal) (Offset $03)",T_N},
   {"Patch Write (part 3)",T_N},
   {"(Internal) (Offset $05)",T_N},
   {"Patch Write (part 4)",T_N},
   {"(Internal) (Offset $07)",T_N},
   {"Patch Write (part 5)",T_N},
   {"(Internal) (Offset $09)",T_N},
   {"Patch Write (part 6)",T_N},
   {"(Internal) (Offset $0B)",T_N},
   {"Patch Write (part 7)",T_N},
   {"(Internal) (Offset $0D)",T_N},
   {"Patch Write (part 8)",T_N},
   {"(Internal) (Offset $0F)",T_N},
};

const entry timbre[] =
{
   {"TIMBRE NAME 1",T_A},
   {"TIMBRE NAME 2",T_A},
   {"TIMBRE NAME 3",T_A},
   {"TIMBRE NAME 4",T_A},
   {"TIMBRE NAME 5",T_A},
   {"TIMBRE NAME 6",T_A},
   {"TIMBRE NAME 7",T_A},
   {"TIMBRE NAME 8",T_A},
   {"TIMBRE NAME 9",T_A},
   {"TIMBRE NAME 10",T_A},
   {"Structure of Partial # 1 & 2",T_N},
   {"Structure of Partial # 3 & 4",T_N},
   {"PARTIAL MUTE",T_N},
   {"ENV MODE",T_N},
   {"(Partial #1) WG PITCH COARSE",T_N},
   {"(Partial #1) WG PITCH FINE",T_N},
   {"(Partial #1) WG PITCH KEYFOLLOW",T_N},
   {"(Partial #1) WG PITCH BENDER SW",T_N},
   {"(Partial #1) WG WAVEFORM/PCM BANK",T_N},
   {"(Partial #1) WG PCM WAVE #",T_N},
   {"(Partial #1) WG PULSE WIDTH",T_N},
   {"(Partial #1) WG PW VELO SENS",T_N},
   {"(Partial #1) P-ENV DEPTH",T_N},
   {"(Partial #1) P-ENV VELO SENS",T_N},
   {"(Partial #1) P-ENV TIME KEYF",T_N},
   {"(Partial #1) P-ENV TIME 1",T_N},
   {"(Partial #1) P-ENV TIME 2",T_N},
   {"(Partial #1) P-ENV TIME 3",T_N},
   {"(Partial #1) P-ENV TIME 4",T_N},
   {"(Partial #1) P-ENV LEVEL 0",T_N},
   {"(Partial #1) P-ENV LEVEL 1",T_N},
   {"(Partial #1) P-ENV LEVEL 2",T_N},
   {"(Partial #1) P-ENV SUSTAIN LEVEL",T_N},
   {"(Partial #1) END LEVEL",T_N},
   {"(Partial #1) P-LFO RATE",T_N},
   {"(Partial #1) P-LFO DEPTH",T_N},
   {"(Partial #1) P-LFO MOD SENS",T_N},
   {"(Partial #1) TVF CUTOFF FREQ",T_N},
   {"(Partial #1) TVF RESONANCE",T_N},
   {"(Partial #1) TVF KEYFOLLOW",T_N},
   {"(Partial #1) TVF BIAS POINT/DIR",T_N},
   {"(Partial #1) TVF BIAS LEVEL",T_N},
   {"(Partial #1) TVF ENV DEPTH",T_N},
   {"(Partial #1) TVF ENV VELO SENS",T_N},
   {"(Partial #1) TVF ENV DEPTH KEYF",T_N},
   {"(Partial #1) TVF ENV TIME KEYF",T_N},
   {"(Partial #1) TVF ENV TIME 1",T_N},
   {"(Partial #1) TVF ENV TIME 2",T_N},
   {"(Partial #1) TVF ENV TIME 3",T_N},
   {"(Partial #1) TVF ENV TIME 4",T_N},
   {"(Partial #1) TVF ENV TIME 5",T_N},
   {"(Partial #1) TVF ENV LEVEL 1",T_N},
   {"(Partial #1) TVF ENV LEVEL 2",T_N},
   {"(Partial #1) TVF ENV LEVEL 3",T_N},
   {"(Partial #1) TVF ENV SUSTAIN LEVEL",T_N},
   {"(Partial #1) TVA LEVEL",T_N},
   {"(Partial #1) TVA VELO SENS",T_N},
   {"(Partial #1) TVA BIAS POINT 1",T_N},
   {"(Partial #1) TVA BIAS LEVEL 1",T_N},
   {"(Partial #1) TVA BIAS POINT 2",T_N},
   {"(Partial #1) TVA BIAS LEVEL 2",T_N},
   {"(Partial #1) TVA ENV TIME KEYF",T_N},
   {"(Partial #1) TVA ENV TIME V_FOLLOW",T_N},
   {"(Partial #1) TVA ENV TIME 1",T_N},
   {"(Partial #1) TVA ENV TIME 2",T_N},
   {"(Partial #1) TVA ENV TIME 3",T_N},
   {"(Partial #1) TVA ENV TIME 4",T_N},
   {"(Partial #1) TVA ENV TIME 5",T_N},
   {"(Partial #1) TVA ENV LEVEL 1",T_N},
   {"(Partial #1) TVA ENV LEVEL 2",T_N},
   {"(Partial #1) TVA ENV LEVEL 3",T_N},
   {"(Partial #1) TVA ENV SUSTAIN LEVEL",T_N},
   {"(Partial #2) WG PITCH COARSE",T_N},
   {"(Partial #2) WG PITCH FINE",T_N},
   {"(Partial #2) WG PITCH KEYFOLLOW",T_N},
   {"(Partial #2) WG PITCH BENDER SW",T_N},
   {"(Partial #2) WG WAVEFORM/PCM BANK",T_N},
   {"(Partial #2) WG PCM WAVE #",T_N},
   {"(Partial #2) WG PULSE WIDTH",T_N},
   {"(Partial #2) WG PW VELO SENS",T_N},
   {"(Partial #2) P-ENV DEPTH",T_N},
   {"(Partial #2) P-ENV VELO SENS",T_N},
   {"(Partial #2) P-ENV TIME KEYF",T_N},
   {"(Partial #2) P-ENV TIME 1",T_N},
   {"(Partial #2) P-ENV TIME 2",T_N},
   {"(Partial #2) P-ENV TIME 3",T_N},
   {"(Partial #2) P-ENV TIME 4",T_N},
   {"(Partial #2) P-ENV LEVEL 0",T_N},
   {"(Partial #2) P-ENV LEVEL 1",T_N},
   {"(Partial #2) P-ENV LEVEL 2",T_N},
   {"(Partial #2) P-ENV SUSTAIN LEVEL",T_N},
   {"(Partial #2) END LEVEL",T_N},
   {"(Partial #2) P-LFO RATE",T_N},
   {"(Partial #2) P-LFO DEPTH",T_N},
   {"(Partial #2) P-LFO MOD SENS",T_N},
   {"(Partial #2) TVF CUTOFF FREQ",T_N},
   {"(Partial #2) TVF RESONANCE",T_N},
   {"(Partial #2) TVF KEYFOLLOW",T_N},
   {"(Partial #2) TVF BIAS POINT/DIR",T_N},
   {"(Partial #2) TVF BIAS LEVEL",T_N},
   {"(Partial #2) TVF ENV DEPTH",T_N},
   {"(Partial #2) TVF ENV VELO SENS",T_N},
   {"(Partial #2) TVF ENV DEPTH KEYF",T_N},
   {"(Partial #2) TVF ENV TIME KEYF",T_N},
   {"(Partial #2) TVF ENV TIME 1",T_N},
   {"(Partial #2) TVF ENV TIME 2",T_N},
   {"(Partial #2) TVF ENV TIME 3",T_N},
   {"(Partial #2) TVF ENV TIME 4",T_N},
   {"(Partial #2) TVF ENV TIME 5",T_N},
   {"(Partial #2) TVF ENV LEVEL 1",T_N},
   {"(Partial #2) TVF ENV LEVEL 2",T_N},
   {"(Partial #2) TVF ENV LEVEL 3",T_N},
   {"(Partial #2) TVF ENV SUSTAIN LEVEL",T_N},
   {"(Partial #2) TVA LEVEL",T_N},
   {"(Partial #2) TVA VELO SENS",T_N},
   {"(Partial #2) TVA BIAS POINT 1",T_N},
   {"(Partial #2) TVA BIAS LEVEL 1",T_N},
   {"(Partial #2) TVA BIAS POINT 2",T_N},
   {"(Partial #2) TVA BIAS LEVEL 2",T_N},
   {"(Partial #2) TVA ENV TIME KEYF",T_N},
   {"(Partial #2) TVA ENV TIME V_FOLLOW",T_N},
   {"(Partial #2) TVA ENV TIME 1",T_N},
   {"(Partial #2) TVA ENV TIME 2",T_N},
   {"(Partial #2) TVA ENV TIME 3",T_N},
   {"(Partial #2) TVA ENV TIME 4",T_N},
   {"(Partial #2) TVA ENV TIME 5",T_N},
   {"(Partial #2) TVA ENV LEVEL 1",T_N},
   {"(Partial #2) TVA ENV LEVEL 2",T_N},
   {"(Partial #2) TVA ENV LEVEL 3",T_N},
   {"(Partial #2) TVA ENV SUSTAIN LEVEL",T_N},
   {"(Partial #3) WG PITCH COARSE",T_N},
   {"(Partial #3) WG PITCH FINE",T_N},
   {"(Partial #3) WG PITCH KEYFOLLOW",T_N},
   {"(Partial #3) WG PITCH BENDER SW",T_N},
   {"(Partial #3) WG WAVEFORM/PCM BANK",T_N},
   {"(Partial #3) WG PCM WAVE #",T_N},
   {"(Partial #3) WG PULSE WIDTH",T_N},
   {"(Partial #3) WG PW VELO SENS",T_N},
   {"(Partial #3) P-ENV DEPTH",T_N},
   {"(Partial #3) P-ENV VELO SENS",T_N},
   {"(Partial #3) P-ENV TIME KEYF",T_N},
   {"(Partial #3) P-ENV TIME 1",T_N},
   {"(Partial #3) P-ENV TIME 2",T_N},
   {"(Partial #3) P-ENV TIME 3",T_N},
   {"(Partial #3) P-ENV TIME 4",T_N},
   {"(Partial #3) P-ENV LEVEL 0",T_N},
   {"(Partial #3) P-ENV LEVEL 1",T_N},
   {"(Partial #3) P-ENV LEVEL 2",T_N},
   {"(Partial #3) P-ENV SUSTAIN LEVEL",T_N},
   {"(Partial #3) END LEVEL",T_N},
   {"(Partial #3) P-LFO RATE",T_N},
   {"(Partial #3) P-LFO DEPTH",T_N},
   {"(Partial #3) P-LFO MOD SENS",T_N},
   {"(Partial #3) TVF CUTOFF FREQ",T_N},
   {"(Partial #3) TVF RESONANCE",T_N},
   {"(Partial #3) TVF KEYFOLLOW",T_N},
   {"(Partial #3) TVF BIAS POINT/DIR",T_N},
   {"(Partial #3) TVF BIAS LEVEL",T_N},
   {"(Partial #3) TVF ENV DEPTH",T_N},
   {"(Partial #3) TVF ENV VELO SENS",T_N},
   {"(Partial #3) TVF ENV DEPTH KEYF",T_N},
   {"(Partial #3) TVF ENV TIME KEYF",T_N},
   {"(Partial #3) TVF ENV TIME 1",T_N},
   {"(Partial #3) TVF ENV TIME 2",T_N},
   {"(Partial #3) TVF ENV TIME 3",T_N},
   {"(Partial #3) TVF ENV TIME 4",T_N},
   {"(Partial #3) TVF ENV TIME 5",T_N},
   {"(Partial #3) TVF ENV LEVEL 1",T_N},
   {"(Partial #3) TVF ENV LEVEL 2",T_N},
   {"(Partial #3) TVF ENV LEVEL 3",T_N},
   {"(Partial #3) TVF ENV SUSTAIN LEVEL",T_N},
   {"(Partial #3) TVA LEVEL",T_N},
   {"(Partial #3) TVA VELO SENS",T_N},
   {"(Partial #3) TVA BIAS POINT 1",T_N},
   {"(Partial #3) TVA BIAS LEVEL 1",T_N},
   {"(Partial #3) TVA BIAS POINT 2",T_N},
   {"(Partial #3) TVA BIAS LEVEL 2",T_N},
   {"(Partial #3) TVA ENV TIME KEYF",T_N},
   {"(Partial #3) TVA ENV TIME V_FOLLOW",T_N},
   {"(Partial #3) TVA ENV TIME 1",T_N},
   {"(Partial #3) TVA ENV TIME 2",T_N},
   {"(Partial #3) TVA ENV TIME 3",T_N},
   {"(Partial #3) TVA ENV TIME 4",T_N},
   {"(Partial #3) TVA ENV TIME 5",T_N},
   {"(Partial #3) TVA ENV LEVEL 1",T_N},
   {"(Partial #3) TVA ENV LEVEL 2",T_N},
   {"(Partial #3) TVA ENV LEVEL 3",T_N},
   {"(Partial #3) TVA ENV SUSTAIN LEVEL",T_N},
   {"(Partial #4) WG PITCH COARSE",T_N},
   {"(Partial #4) WG PITCH FINE",T_N},
   {"(Partial #4) WG PITCH KEYFOLLOW",T_N},
   {"(Partial #4) WG PITCH BENDER SW",T_N},
   {"(Partial #4) WG WAVEFORM/PCM BANK",T_N},
   {"(Partial #4) WG PCM WAVE #",T_N},
   {"(Partial #4) WG PULSE WIDTH",T_N},
   {"(Partial #4) WG PW VELO SENS",T_N},
   {"(Partial #4) P-ENV DEPTH",T_N},
   {"(Partial #4) P-ENV VELO SENS",T_N},
   {"(Partial #4) P-ENV TIME KEYF",T_N},
   {"(Partial #4) P-ENV TIME 1",T_N},
   {"(Partial #4) P-ENV TIME 2",T_N},
   {"(Partial #4) P-ENV TIME 3",T_N},
   {"(Partial #4) P-ENV TIME 4",T_N},
   {"(Partial #4) P-ENV LEVEL 0",T_N},
   {"(Partial #4) P-ENV LEVEL 1",T_N},
   {"(Partial #4) P-ENV LEVEL 2",T_N},
   {"(Partial #4) P-ENV SUSTAIN LEVEL",T_N},
   {"(Partial #4) END LEVEL",T_N},
   {"(Partial #4) P-LFO RATE",T_N},
   {"(Partial #4) P-LFO DEPTH",T_N},
   {"(Partial #4) P-LFO MOD SENS",T_N},
   {"(Partial #4) TVF CUTOFF FREQ",T_N},
   {"(Partial #4) TVF RESONANCE",T_N},
   {"(Partial #4) TVF KEYFOLLOW",T_N},
   {"(Partial #4) TVF BIAS POINT/DIR",T_N},
   {"(Partial #4) TVF BIAS LEVEL",T_N},
   {"(Partial #4) TVF ENV DEPTH",T_N},
   {"(Partial #4) TVF ENV VELO SENS",T_N},
   {"(Partial #4) TVF ENV DEPTH KEYF",T_N},
   {"(Partial #4) TVF ENV TIME KEYF",T_N},
   {"(Partial #4) TVF ENV TIME 1",T_N},
   {"(Partial #4) TVF ENV TIME 2",T_N},
   {"(Partial #4) TVF ENV TIME 3",T_N},
   {"(Partial #4) TVF ENV TIME 4",T_N},
   {"(Partial #4) TVF ENV TIME 5",T_N},
   {"(Partial #4) TVF ENV LEVEL 1",T_N},
   {"(Partial #4) TVF ENV LEVEL 2",T_N},
   {"(Partial #4) TVF ENV LEVEL 3",T_N},
   {"(Partial #4) TVF ENV SUSTAIN LEVEL",T_N},
   {"(Partial #4) TVA LEVEL",T_N},
   {"(Partial #4) TVA VELO SENS",T_N},
   {"(Partial #4) TVA BIAS POINT 1",T_N},
   {"(Partial #4) TVA BIAS LEVEL 1",T_N},
   {"(Partial #4) TVA BIAS POINT 2",T_N},
   {"(Partial #4) TVA BIAS LEVEL 2",T_N},
   {"(Partial #4) TVA ENV TIME KEYF",T_N},
   {"(Partial #4) TVA ENV TIME V_FOLLOW",T_N},
   {"(Partial #4) TVA ENV TIME 1",T_N},
   {"(Partial #4) TVA ENV TIME 2",T_N},
   {"(Partial #4) TVA ENV TIME 3",T_N},
   {"(Partial #4) TVA ENV TIME 4",T_N},
   {"(Partial #4) TVA ENV TIME 5",T_N},
   {"(Partial #4) TVA ENV LEVEL 1",T_N},
   {"(Partial #4) TVA ENV LEVEL 2",T_N},
   {"(Partial #4) TVA ENV LEVEL 3",T_N},
   {"(Partial #4) TVA ENV SUSTAIN LEVEL",T_N},
   {"(Unused -- timbre offset $F6)",T_NONE},
   {"(Unused -- timbre offset $F7)",T_NONE},
   {"(Unused -- timbre offset $F8)",T_NONE},
   {"(Unused -- timbre offset $F9)",T_NONE},
   {"(Unused -- timbre offset $FA)",T_NONE},
   {"(Unused -- timbre offset $FB)",T_NONE},
   {"(Unused -- timbre offset $FC)",T_NONE},
   {"(Unused -- timbre offset $FD)",T_NONE},
   {"(Unused -- timbre offset $FE)",T_NONE},
   {"(Unused -- timbre offset $FF)",T_NONE}
};

const Roland_area areas[] =
{
   {"Timbre Temporary Area (part 1-8)",0x02,0x00,0x00,0xf6,1,1,
      timbre},
   {"Patch Temporary Area (part %u)",0x03,0x00,0x00,0x10,1,8,
      patch_temp},
   {"Rhythm Setup Temporary Area (for Key # %u)",0x03,0x01,0x10,0x04,24,85,
      rhythm_setup},
   {"Timbre Temporary Area (part %u)",0x04,0x00,0x00,0xf6,1,8,
      timbre},
   {"Patch Memory #%u",0x05,0x00,0x00,0x08,1,128,
      patch},
   {"Timbre Memory #%u",0x08,0x00,0x00,0x100,1,64,
      timbre},
   {"System area",0x10,0x00,0x00,0x17,1,1,
      system_area},
   {"Display",0x20,0x00,0x00,0x14,1,1,
      display},
   {"Write Request",0x40,0x00,0x00,0x20,1,1,
      write_request},
   {"All parameters reset",0x7f,0x00,0x00,0x01,1,1,
      NULL}                       
};

/***************************************************************/
sysex_addr calc_offset(sysex_addr base, unsigned offset)
{
   unsigned long addr;
   sysex_addr result;

   addr = ((unsigned long) base.MSB << 14L) |
          ((unsigned long) base.KSB << 7L) |
          ((unsigned long) base.LSB);

   addr += offset;

   result.LSB = addr & 0x7fL;
   result.KSB = (addr >> 7L) & 0x7fL;
   result.MSB = (addr >> 14L) & 0x7fL;

   return result;
}

int sysex_window(sysex_addr cur, sysex_addr next, sysex_addr test)
{
   unsigned long laddr,haddr,taddr;

   laddr = ((unsigned long) cur.MSB << 14L) |
           ((unsigned long) cur.KSB << 7L) |
           ((unsigned long) cur.LSB);

   haddr = ((unsigned long) next.MSB << 14L) |
           ((unsigned long) next.KSB << 7L) |
           ((unsigned long) next.LSB);

   taddr = ((unsigned long) test.MSB << 14L) |
           ((unsigned long) test.KSB << 7L) |
           ((unsigned long) test.LSB);

   if ((taddr >= laddr) && (taddr < haddr))
      return taddr-laddr;

   return -1;
}

void show_address(unsigned curpos, unsigned dest, sysex_addr cur)
{
   unsigned k;

   printf(" ");
   for (k=curpos;k<dest;k++)
      printf(".");
   printf(" %.2X %.2X %.2X (%.3u %.3u %.3u)\n",cur.MSB,cur.KSB,cur.LSB,
      cur.MSB,cur.KSB,cur.LSB);
}

/***************************************************************/
unsigned describe_addr(sysex_addr addr, addr_desc *desc)
{
   unsigned i,j,m;
   int k;
   sysex_addr cur,next;
   const entry *parms;

   for (i=0;i<sizeof(areas)/sizeof(Roland_area);i++)
      {
      cur = areas[i].base;
      m = areas[i].group_count;

      for (j=0;j<m;j++)
         {
         next = calc_offset(cur,areas[i].base_size);

         k = sysex_window(cur,next,addr);
         if (k != -1)
            {
            if (m > 1)
               sprintf(desc->area_name,areas[i].name,j+areas[i].base_num);
            else
               sprintf(desc->area_name,areas[i].name);

            parms = areas[i].items;
            if (parms != NULL)
               {
               strcpy(desc->parm_name,parms[k].name);
               desc->type = parms[k].type;
               }
            else
               {
               strcpy(desc->parm_name,"");
               desc->type = T_NONE;
               }

            return 1;
            }
            
         cur = calc_offset(cur,areas[i].base_size);
         }
      }

   return 0;
}

/***************************************************************/
void show_MR(unsigned abridged)
{
   unsigned i,gc;
   unsigned j,k,m;
   char *tname;
   sysex_addr group,cur;
   const entry *parms;

   printf("              Roland MT-32 / LAPC-1 Programmer's Reference Guide\n\n");

   printf("Parameter                                               Hex      Decimal\n");
   printf("_______________________________________________________ ________ _____________\n\n");

   for (i=0;i<sizeof(areas)/sizeof(Roland_area);i++)
      {
      cur = areas[i].base;
      gc = areas[i].group_count;

      for (j=0;j<gc;j++)
         {
         tname = areas[i].name;
         k = (gc > 1) ?
            printf(tname,j+areas[i].base_num) : printf(tname);
         if (abridged)
            show_address(k,54,cur);
         else
            printf("\n");
         group = cur;
         cur = calc_offset(cur,areas[i].base_size);
         if (abridged) continue;

         for (m=0;m<areas[i].base_size;m++)
            {
            parms = areas[i].items;
            k = (parms==NULL) ?
               printf("   %s",tname) : printf("   %s",parms[m].name);
            show_address(k,54,group);
            group = calc_offset(group,1);
            }
         }
      if (!abridged)
         {
         putchar(12);
         putchar('\n');
         }
      }

}

/***************************************************************/
unsigned get_chr(void)
{
   unsigned val;

   val = (unsigned) (*(unsigned char *) bufpnt);
   offset = (bufpnt-buffer);

   ++bufpnt;
   return val;
}

unsigned next_chr(void)
{
   return (unsigned) (*(unsigned char *) bufpnt);
}

unsigned long get_vln(void)
{
   unsigned long val=0L;
   unsigned i,cnt=4;

   do
      {
      i = get_chr();
      val = (val << 7) | (unsigned long) (i & 0x7f);
      if (!(i & 0x80))
         cnt = 0;
      else
         --cnt;
      }
   while (cnt);

   return val;
}

unsigned long get_ulong(void)
{
   unsigned long val;

   val = (unsigned long) get_chr();
   val = (val << 8) | (unsigned long) get_chr();
   val = (val << 8) | (unsigned long) get_chr();
   val = (val << 8) | (unsigned long) get_chr();

   return val;
}

unsigned long get_24(void)
{
   unsigned long val;

   val = (unsigned long) get_chr();
   val = (val << 8) | (unsigned long) get_chr();
   val = (val << 8) | (unsigned long) get_chr();

   return val;
}   

unsigned get_u(void)
{
   unsigned val;

   val = get_chr();
   val = (val << 8) | get_chr();

   return val;
}

/***************************************************************/
unsigned inspect_Roland_sysex(unsigned char *buffer, unsigned len)
{
   unsigned i;
   static char tab[] = "           ";
   char lastname[256];
   sysex_addr target;
   addr_desc desc;

   if (len < 7)
      {
      printf("%sUnrecognized message format",tab);
      return 0;
      }

   if ((buffer[0] != 0x41) || (buffer[1] != 0x10) || (buffer[2] != 0x16))
      {
      printf("%sUnrecognized manufacturer/device code",tab);
      return 0;
      }

   printf("%sManufacturer ID = $41\n",tab);
   printf("%sDevice ID = $10\n",tab);
   printf("%sModel ID = $16\n",tab);
   printf("%sCommand = $%.2X ",tab,buffer[3]);
   switch (buffer[3])
      {
      case 0x11: printf("(Request data 1: RQ1)"); return 0;
      case 0x12: printf("(Data set 1: DT1)"); break;
      case 0x40: printf("(Want to send data: WSD)"); return 0;
      case 0x41: printf("(Request data: RQD)"); return 0;
      case 0x42: printf("(Data set: DAT)"); break;
      case 0x43: printf("(Acknowledge: ACK)"); return 0;
      case 0x45: printf("(End of data: EOD)"); return 0;
      case 0x4e: printf("(Communications error: ERR)"); return 0;
      case 0x4f: printf("(Rejection: RJC)"); return 0;
      default: printf("(Unknown or invalid command ID)"); return 0;
      }

   target.MSB = buffer[4];
   target.KSB = buffer[5];
   target.LSB = buffer[6];

   strcpy(lastname,"xxx");

   for (i=7;i<(len-2);i++)
      {
      if (target.MSB == 0x7f)
         {
         printf("\n%s   All parameters reset",tab);
         target = calc_offset(target,1);
         continue;
         }

      if (!describe_addr(target,&desc))
         {
         printf("\n%s(Undocumented address -- cannot parse message)",tab);
         return 0;
         }

      if (strcmp(lastname,desc.area_name))
         {
         printf("\n%sAddress $%.2X $%.2X $%.2X: %s",tab,target.MSB,target.KSB,
            target.LSB,desc.area_name);
         strcpy(lastname,desc.area_name);
         }

      switch (desc.type)
         {
         case T_NONE:
            break;
         case T_A:
            printf("\n%s   %s",tab,desc.parm_name);
            printf(" '%c'",buffer[i]);
            break;
         case T_N:
            printf("\n%s   %s",tab,desc.parm_name);
            printf(" = %u",buffer[i]);
            break;
         case T_C:
            printf("\n%s   %s",tab,desc.parm_name);
            printf(" = %u",buffer[i]+1);
            break;
         }

      target = calc_offset(target,1);
      }


   printf("\n%sChecksum = $%.2X",tab,buffer[len-2]);

   if (buffer[len-1] != 0xf7)
      {
      printf("\nWARNING: Missing $F7 EOX terminator");
      }

   return 1;
}      

/***************************************************************/
int parse_chunk(void)
{                          // returns 0 if last chunk
   unsigned h,i,j,l,m,n,p,s,k,v;
   char chunk_title[5];
   unsigned chunk_len,chunk_end;
   static unsigned last_chunk = 0;
   unsigned division;
   unsigned tracks_left;
   unsigned status,running_status,channel;
   unsigned end_of_track;
   unsigned long delta_time;
   unsigned long r,t;
   unsigned con_num,con_val;
   static const char bs[][4] = {"MSB","LSB"};
   static const char stat[][4] = {"off","on"};
   static const int pwr_2[] = {0,2,4,8,16,32,64,128,256};
   unsigned bn;

   for (i=0;i<4;i++) chunk_title[i] = get_chr(); chunk_title[i] = 0;
   chunk_len = (unsigned) get_ulong();
   chunk_end = offset + chunk_len + 1;

   printf("Offset $%.4X: Chunk %s, length $%.4X\n",offset-7,chunk_title,chunk_len);

   if (!stricmp(chunk_title,"MThd"))
      {
      printf("    MIDI file format: %u\n",get_u());
      printf("    # of tracks: %u\n",tracks_left = get_u());
      division = get_u();
      if (division & 0x8000)
         {
         printf("    SMPTE division: %u frames per second, %u ticks per frame\n",
            256 - ((division & 0xff00) >> 8),division & 0xff);
         }
      else
         {
         printf("    MIDI division: %u delta-time ticks per quarter-note\n",division);
         }
      }

   if (stricmp(chunk_title,"MTrk"))
      {
      bufpnt = buffer + chunk_end;
      return (!last_chunk);          // unknown chunk type
      }

   // else parse MTrk chunk ...

   running_status = 0;
   end_of_track = 0;

   do
      {
      delta_time = get_vln();

      printf("    DTime %.4lu: ",delta_time);

      status = 0;

      if (next_chr() >= 0x80)
         status = running_status = get_chr();
      else
         status = running_status;

      if ((status >= 0x80) && (status < 0xf0))
         {
         channel = status & 0x0f;      // channel voice message $8x-$Ex
         status  = status & 0xf0;
         printf("Ch %.2u ",channel+1);
         }

      switch (status)
         {
         case 0x80:        // note off
            k = get_chr(); v = get_chr();
            printf("note %.3u off, release velocity %.3u",k,v);
            break;
         case 0x90:        // note on
            k = get_chr(); v = get_chr();
            if (v == 0)
               printf("note %.3u off",k);
            else
               printf("note %.3u on, attack velocity %.3u",k,v);
            break;
         case 0xa0:        // polyphonic key pressure
            k = get_chr(); v = get_chr();
            printf("polyphonic key pressure %.3u applied to note %.3u",v,k);
            break;
         case 0xb0:        // control change or channel mode message
            con_num = get_chr();
            con_val = get_chr();

            bn = 0;

            switch (con_num)
               {
               case 1:
                  printf("modulation controller %s = %.3u",bs[bn],con_val);
                  break;
               case 2:
                  printf("breath controller %s = %.3u",bs[bn],con_val);
                  break;         
               case 4:
                  printf("foot controller %s = %.3u",bs[bn],con_val);
                  break;
               case 5:
                  printf("portamento time %s = %.3u",bs[bn],con_val);
                  break;
               case 6:
                  printf("data entry %s = %.3u",bs[bn],con_val);
                  break;
               case 7:
                  printf("main volume %s = %.3u",bs[bn],con_val);
                  break;
               case 8:
                  printf("balance controller %s = %.3u",bs[bn],con_val);
                  break;
               case 10:
                  printf("pan controller %s = %.3u",bs[bn],con_val);
                  break;
               case 11:
                  printf("expression controller %s = %.3u",bs[bn],con_val);
                  break;
               case 16:
                  printf("general purpose controller #1 %s = %.3u",bs[bn],con_val);
                  break;
               case 17:
                  printf("general purpose controller #2 %s = %.3u",bs[bn],con_val);
                  break;
               case 18:
                  printf("general purpose controller #3 %s = %.3u",bs[bn],con_val);
                  break;
               case 19:
                  printf("general purpose controller #4 %s = %.3u",bs[bn],con_val);
                  break;

               case 32:
                  printf("AIL sysex start address MSB (queue 0) %.3u",con_val);
                  break;
               case 33:
                  printf("AIL sysex start address KSB (queue 0) %.3u",con_val);
                  break;
               case 34:
                  printf("AIL sysex start address LSB (queue 0) %.3u",con_val);
                  break;
               case 35:
                  printf("AIL sysex data byte (queue 0) %.3u",con_val);
                  break;
               case 36:
                  printf("AIL final sysex data byte (queue 0) %.3u",con_val);
                  break;
               case 37:
                  printf("AIL sysex start address MSB (queue 1) %.3u",con_val);
                  break;
               case 38:
                  printf("AIL sysex start address KSB (queue 1) %.3u",con_val);
                  break;
               case 39:
                  printf("AIL sysex start address LSB (queue 1) %.3u",con_val);
                  break;
               case 40:
                  printf("AIL sysex data byte (queue 1) %.3u",con_val);
                  break;
               case 41:
                  printf("AIL final sysex data byte (queue 1) %.3u",con_val);
                  break;
               case 42:
                  printf("AIL sysex start address MSB (queue 2) %.3u",con_val);
                  break;
               case 43:
                  printf("AIL sysex start address KSB (queue 2) %.3u",con_val);
                  break;
               case 44:
                  printf("AIL sysex start address LSB (queue 2) %.3u",con_val);
                  break;
               case 45:
                  printf("AIL sysex data byte (queue 2) %.3u",con_val);
                  break;
               case 46:
                  printf("AIL final sysex data byte (queue 2) %.3u",con_val);
                  break;

               case 58:
                  printf("AIL rhythm setup timbre %.3u",con_val);
                  break;
               case 59:
                  printf("AIL MT-32 patch reverb switch %.3u",con_val);
                  break;
               case 60:
                  printf("AIL MT-32 patch bender range %.3u",con_val);
                  break;
               case 61:
                  printf("AIL MT-32 reverb mode %.3u",con_val);
                  break;
               case 62:
                  printf("AIL MT-32 reverb time %.3u",con_val);
                  break;
               case 63:
                  printf("AIL MT-32 reverb level %.3u",con_val);
                  break;

               case 64:
                  printf("damper pedal (sustain) = %s",stat[con_val > 63]);
                  break;
               case 65:
                  printf("portamento = %s",stat[con_val > 63]);
                  break;
               case 66:
                  printf("sostenuto %s",stat[con_val > 63]);
                  break;
               case 67:
                  printf("soft pedal = %s",stat[con_val > 63]);
                  break;
               case 69:
                  printf("hold 2 = %s",stat[con_val > 63]);
                  break;
               case 80:
                  printf("general purpose controller #5 = %.3u",con_val);
                  break;
               case 81:
                  printf("general purpose controller #6 = %.3u",con_val);
                  break;
               case 82:
                  printf("general purpose controller #7 = %.3u",con_val);
                  break;
               case 83:
                  printf("general purpose controller #8 = %.3u",con_val);
                  break;
               case 91:
                  printf("external effects depth = %.3u",con_val);
                  break;
               case 92:
                  printf("tremolo depth = %.3u",con_val);
                  break;
               case 93:
                  printf("chorus depth = %.3u",con_val);
                  break;
               case 94:
                  printf("celeste (detune) depth = %.3u",con_val);
                  break;
               case 95:
                  printf("phaser depth = %.3u",con_val);
                  break;

               case 96:
                  printf("data increment");
                  break;
               case 97:
                  printf("data decrement");
                  break;

               case 98:
                  printf("non-registered parameter LSB %.3u selected",con_val);
                  break;
               case 99:
                  printf("non-registered parameter MSB %.3u selected",con_val);
                  break;
               case 100:
                  printf("registered parameter LSB %.3u selected",con_val);
                  break;
               case 101:
                  printf("registered parameter MSB %.3u selected",con_val);
                  break;

               case 110:
                  printf("AIL channel lock/release %.3u",con_val);
                  break;
               case 111:
                  printf("AIL channel lock protection %.3u",con_val);
                  break;
               case 112:
                  printf("AIL voice protection %.3u",con_val);
                  break;
               case 113:
                  printf("AIL timbre protection %.3u",con_val);
                  break;
               case 114:
                  printf("AIL patch bank select, bank %.3u",con_val);
                  break;
               case 115:
                  printf("AIL indirect controller prefix, array[%.3u]",con_val);
                  break;
               case 116:
                  printf("AIL loop controller: FOR loop = 1 to %.3u",con_val);
                  break;
               case 117:
                  printf("AIL loop controller: NEXT/BREAK value %.3u",con_val);
                  break;
               case 118:
                  printf("AIL clear beat / measure count controller");
                  break;
               case 119:
                  printf("AIL callback trigger controller %.3u",con_val);
                  break;
               case 120:
                  printf("AIL sequence index controller %.3u",con_val);
                  break;

               case 121:
                  printf("reset all controllers");
                  break;
               case 122:
                  printf("local control %s",stat[con_val > 63]);
                  break;
               case 123:
                  printf("all notes off");
                  break;
               case 124:
                  printf("omni mode off");
                  break;
               case 125:
                  printf("omni mode on");
                  break;
               case 126:
                  printf("mono mode on, %.3u channels",con_val);
                  break;
               case 127:
                  printf("poly mode on");
                  break;

               default:
                  if (con_num > 127)
                     printf("MIDI file syntax error: data value %.3u",con_num);
                  else
                     printf("undefined controller #%.3u, value = %.3u",
                     con_num,con_val);
                  break;
               }
            break;

         case 0xc0:        // program change
            printf("program change to %.3u",get_chr());
            break;
         case 0xd0:        // channel pressure
            printf("channel pressure %.3u applied",get_chr());
            break;
         case 0xe0:        // pitch bend change
            l = get_chr(); h = get_chr();
            printf("pitch wheel set to %.5u",h*128+l);
            break;

         case 0xf0:        // system exclusive message
            printf("system exclusive message, type $F0\n");
            t = get_vln();

            if (arg_mt)
               {
               for (i=0;i<t;i++) sysex_buffer[i] = get_chr();
               if (inspect_Roland_sysex(sysex_buffer,t)) break;
               printf("\n           (Restart MLIST without /MT option to view hex dump)");
               break;
               }

            printf("        ");
            i = 0;
            while (t)
               {
               t--;                                        
               printf("%.2X ",get_chr());
               if ((++i == 8) && (t)) { printf("\n        "); i=0;}
               }
            break;

         case 0xf7:        // EOX or secondary SysEX packet
            printf("system exclusive message, type $F7\n");
            t = get_vln();

            printf("        ");
            i = 0;
            while (t)
               {
               t--;                                        
               printf("%.2X ",get_chr());
               if ((++i == 8) && (t)) { printf("\n        "); i=0;}
               }
            break;
            
         case 0xf1:       // SysCom: MTC quarter-frame
            i = get_chr(); 
            printf("MTC quarter-frame message type $%.2X",i);
            break;
         case 0xf2:       // SysCom: song position pointer
            l = get_chr(); h = get_chr();
            printf("song position pointer = %.5u",h*128+l);
            break;
         case 0xf3:       // SysCom: song select
            s = get_chr();
            printf("song select = %.3u",s);
            break;
         case 0xf6:       // SysCom: tune request
            printf("tune request");
            break;

         case 0xff:       // meta-event
            printf("meta-event: ");
            m = get_chr();
            t = get_vln();
            switch (m)
               {
               case 0x00:
                  printf("sequence number %lu",get_u());
                  t -= 2;
                  while (t--) get_chr();
                  break;
               case 0x01:
                  printf("text \"");
                  while (t--) printf("%c",get_chr());
                  printf("\"");
                  break;
               case 0x02:
                  printf("copyright notice \"");
                  while (t--) printf("%c",get_chr());
                  printf("\"");
                  break;
               case 0x03:
                  printf("sequence/track name \"");
                  while (t--) printf("%c",get_chr());
                  printf("\"");
                  break;
               case 0x04:
                  printf("instrument name \"");
                  while (t--) printf("%c",get_chr());
                  printf("\"");
                  break;
               case 0x05:
                  printf("lyric \"");
                  while (t--) printf("%c",get_chr());
                  printf("\"");
                  break;
               case 0x06:
                  printf("marker \"");
                  while (t--) printf("%c",get_chr());
                  printf("\"");
                  break;
               case 0x07:
                  printf("cue point \"");
                  while (t--) printf("%c",get_chr());
                  printf("\"");
                  break;
               case 0x20:
                  printf("MIDI channel prefix %.3u",get_chr());
                  t -= 1;
                  while (t--) get_chr();
                  break;
               case 0x2f:
                  printf("end of track");
                  while (t--) get_chr();
                  end_of_track = 1;
                  break;
               case 0x51:
                  printf("tempo in uS/MIDI quarter-note = %lu",
                     get_24());
                  t -= 3;
                  while (t--) get_chr();
                  break;
               case 0x54:
                  i=get_chr(); j=get_chr(); k=get_chr(); l=get_chr();
                  m=get_chr();
                  printf("SMPTE offset %.3u:%.3u:%.3u %.3u:%.3u",i,j,k,l,m);
                  t -= 5;
                  while (t--) get_chr();
                  break;
               case 0x58:
                  i=get_chr(); j=get_chr(); k=get_chr(); l=get_chr();
                  printf("time signature %u/%u, c = %.3u, b = %.3u",
                     i,pwr_2[j],k,l);
                  t -= 4;
                  while (t--) get_chr();
                  break;
               case 0x59:
                  i=get_chr(); j=get_chr();
                  printf("key signature %u %u",i,j);
                  t -= 2;
                  while (t--) get_chr();
                  break;
               case 0x7c:
                  printf("AIL reset beat / measure counter (obsolete) ");
                  while (t--) printf("%u ",get_chr());
                  break;
               case 0x7d:
                  i=get_chr();
                  printf("AIL callback trigger %u (obsolete) ",i);
                  t-=1;
                  while (t--) printf("%u ",get_chr());
                  break;
               case 0x7e:
                  i=get_chr();
                  printf("AIL track index %u (obsolete) ",i);
                  t-=1;
                  while (t--) printf("%u ",get_chr());
                  break;
               case 0x7f:
                  printf("%lu bytes of sequencer-specific meta-event data",t);
                  while (t--) get_chr();
                  break;

               default:
                  printf("undefined event code $%.2X",m);
                  while (t--) get_chr();
                  break;
               }
            break;

         default:         // undefined or illegal
            printf("Undefined or illegal status byte $%.2X",status);
            break;
         }

      printf("\n");
      }
   while (!end_of_track);
                                                    
   if (!(--tracks_left)) last_chunk = 1;

   bufpnt = buffer + chunk_end;
   return (!last_chunk);
}

/***************************************************************/
void main(int argc, char *argv[])
{
   unsigned long l;
   unsigned bad,strcnt,i;
   static char filename[256];

   printf("\n");

   arg_mt=arg_ma=arg_mu=0;

   bad=strcnt=0;
   for (i=1;i<argc;i++)
      if (argv[i][0] != '/')
         if (strcnt == 1)
            {
            bad=1; break;
            }
         else
            {
            strcpy(filename,argv[i]);
            strcnt++;
            }
      else
         if (!strnicmp(argv[i],"/MT",3))
            arg_mt = 1;
         else if (!strnicmp(argv[i],"/MA",3))
            arg_ma = 1;
         else if (!strnicmp(argv[i],"/MU",3))
            arg_mu = 1;
         else
            {
            bad=1; break;
            }

   if (!(arg_ma | arg_mu))
      {
      printf("MLIST version %s                  Copyright (C) 1991, 1992 Miles Design, Inc.\n",VERSION);
      printf("-------------------------------------------------------------------------------\n\n");
      }

   if (bad || ((!strcnt) && (!(arg_ma | arg_mu))))
      {
      printf("This program displays the contents of a standard MIDI file.\n\n");
      printf("Usage: MLIST [/MT] filename | /MA | /MU\n\n");
      printf("where /MT = Display System Exclusive messages in Roland MT-32/LAPC-1 format\n");
      printf("      /MA = Print abridged Roland MT-32/LAPC-1 Programmer's Reference to\n");
      printf("            standard output device\n");
      printf("      /MU = Print unabridged Roland MT-32/LAPC-1 Programmer's Reference to\n");
      printf("            standard output device (WARNING: Requires over 300 pages)\n");
      printf("\nParameters in brackets [] are optional, and may appear in any order.\n");
      exit(1);
      }

   if (arg_ma)
      {
      show_MR(1);
      exit(0);
      }

   if (arg_mu)
      {
      show_MR(0);
      exit(0);
      }

   buffer = bufpnt = read_file(filename,NULL);
   if (buffer == NULL)
      {
      printf("Can't load %s.\n",filename);
      exit(1);
      }

   l = file_size(filename); bufend = buffer + l;
   do
      {
      if ((toupper(*bufpnt) == 'M') &&
      (toupper(*(bufpnt+1)) == 'T') &&
      (toupper(*(bufpnt+2)) == 'H') &&
      (toupper(*(bufpnt+3)) == 'D')) break;
      bufpnt++;
      }
   while (--l);

   if (!l)
      {
      printf("Not a standard MIDI file!\n");
      exit(1);
      }

   while (parse_chunk());

   exit(0);
}


