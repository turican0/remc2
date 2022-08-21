;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
;лл                                                                        лл
;лл   YAMAHA.INC                                                           лл
;лл                                                                        лл
;лл   IBM Audio Interface Library XMIDI interpreter for Ad Lib, etc.       лл
;лл   including YMF262 (aka OPL3) 2-and-4-operator FM support              лл
;лл                                                                        лл
;лл   Version 1.00 of 15-Feb-92: Init version (replaces YM3812.INC v1.04)  лл
;лл           1.01 of 20-Jun-92: Pro Audio Spectrum Plus/16 support added  лл
;лл                              Remove REPTs to circumvent TASM bug       лл
;лл                              Reset PAS to Ad Lib mode on shutdown      лл
;лл           1.02 of 13-Nov-92: Detect OPL3 at 2x8, not 2x0               лл
;лл                                                                        лл
;лл   8086 ASM source compatible with Turbo Assembler v2.0 or later        лл
;лл   Author: John Miles                                                   лл
;лл                                                                        лл
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
;лл                                                                        лл
;лл   Copyright (C) 1991, 1992 Miles Design, Inc.                          лл
;лл                                                                        лл
;лл   Miles Design, Inc.                                                   лл
;лл   10926 Jollyville #308                                                лл
;лл   Austin, TX 78759                                                     лл
;лл   (512) 345-2642 / FAX (512) 338-9630 / BBS (512) 454-9990             лл
;лл                                                                        лл
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

                ;
                ;Driver-specific configuration equates
                ;

OSI_ALE         equ FALSE               ;TRUE to assemble OSI TVFX code

MAX_REC_CHAN    equ 10                  ;Max channel recognized by synths
MAX_TRUE_CHAN   equ 9                   ;Max channel available for locking
MIN_TRUE_CHAN   equ 2                   ;Min channel # (1-based)

DEF_SYNTH_VOL   equ 100                 ;Init vol=100%
MAX_TIMBS       equ 192                 ;Max # of timbres in local cache
DEF_TC_SIZE     equ 3584                ;Room for 256 14-byte .BNK timbres

                IFDEF YM3812
NUM_VOICES      equ 9                   ;# of physical voices available
NUM_SLOTS       equ 16                  ;# of virtual voices available
                ELSEIFDEF YMF262
NUM_VOICES      equ 18                  ;# of physical voices available
NUM_4OP_VOICES  equ 6
NUM_SLOTS       equ 20                  ;# of virtual voices available
                ENDIF

VEL_SENS        equ 1                   ;Velocity sensitivity disabled if 0

VEL_TRUE        equ 0                   ;Full velocity sensitivity range if 1
                                        ;(set to 0 to reduce playback noise)
DEF_PITCH_RANGE equ 12                  ;Default pitch wheel range (semitones)

DEF_AV_DEPTH    equ 11000000b           ;Default AM/Vibrato depth
                                        ;bit 7: AM depth 4.8dB if 1, else 1dB
                                        ;bit 6: VIB depth 14c. if 1, else 7c.

                IFDEF YMF262            ;Panpot thresholds for OPL3 voices

R_PAN_THRESH    equ 27                  ;Force right channel if pan <= n
L_PAN_THRESH    equ 100                 ;Force left channel if pan >= n

                IFDEF SBPRO2
LEFT_MASK       equ 11011111b
RIGHT_MASK      equ 11101111b
                ELSEIFDEF PASOPL
LEFT_MASK       equ 11011111b
RIGHT_MASK      equ 11101111b
                ELSE
LEFT_MASK       equ 11101111b
RIGHT_MASK      equ 11011111b
                ENDIF

                ENDIF

CIRC_ASSIGN     equ TRUE                ;FALSE for "old" AIL voice assignment

                ;
                ;Internal equates
                ;

                IFDEF SBPRO1
SBPRO           equ 1
                ENDIF

                IFDEF SBPRO2
SBPRO           equ 1
                ENDIF

                IFDEF PAS
PROAUDIO        equ 1
                ENDIF

                IFDEF PASOPL
PROAUDIO        equ 1
                ENDIF

                ;
                ;Driver Description Table (DDT)
                ;Returned by describe_driver() proc
                ;

DDT             LABEL WORD
min_API_version dw 200                  ;Minimum API version required = 2.00
drvr_type       dw 3                    ;Type 3: XMIDI emulation
data_suffix     LABEL BYTE
                IFDEF YMF262
                db 'OPL',0              ;Supports .OPL Global Timbre file
                ELSE                    ;(backward-compatible w/.AD format)
                db 'AD',0,0             ;Needs .AD Global Timbre file
                ENDIF
device_name_o   dw OFFSET devnames      ;Pointer to list of supported devices
device_name_s   dw ?
default_IO      LABEL WORD              ;Factory default I/O parameters
                IFDEF ADLIBSTD
                dw 388h
                ELSEIFDEF SBSTD
                dw 220h
                ELSEIFDEF PROAUDIO
                dw -1
                ELSEIFDEF SBPRO1
                dw 220h
                ELSEIFDEF SBPRO2
                dw 220h
                ELSEIFDEF ADLIBG
                dw 388h
                ENDIF
default_IRQ     dw -1
default_DMA     dw -1
default_DRQ     dw -1
service_rate    dw QUANT_RATE           ;Request 120 calls/second 
display_size    dw 0                    ;No display

devnames        LABEL BYTE
                IFDEF ADLIBSTD
                db 'Ad Lib(R) Music Synthesizer Card',0
                ELSEIFDEF SBSTD
                db 'Creative Labs Sound Blaster(TM) FM Sound',0
                db 'Media Vision Thunderboard(TM) FM Sound',0
                ELSEIFDEF PAS
                db 'Media Vision Pro Audio Spectrum(TM) 8 FM Sound',0
                ELSEIFDEF PASOPL
                db 'Media Vision Pro Audio Spectrum(TM) Plus/16 FM Sound',0
                ELSEIFDEF SBPRO
                db 'Creative Labs Sound Blaster Pro(TM) FM Sound',0
                ELSEIFDEF ADLIBG
                db 'Ad Lib(R) Gold Music Synthesizer Card',0
                ENDIF
                db 0                    ;0 to end list of device names

                IFDEF YM3812
IOWVAL          equ 42
                IFDEF STEREO
STEREO_3812     equ 1
                ENDIF
                ELSEIFDEF YMF262
IOWVAL          equ 8
                ENDIF

                IF OSI_ALE
                INCLUDE ale.inc
                ENDIF

                ;
                ;Default setup values & internal constants
                ;

freq_table	dw 02b2h,02b4h,02b7h,02b9h,02bch,02beh,02c1h,02c3h,02c6h,02c9h
	dw 02cbh,02ceh,02d0h,02d3h,02d6h,02d8h,02dbh,02ddh,02e0h,02e3h 
	dw 02e5h,02e8h,02ebh,02edh,02f0h,02f3h,02f6h,02f8h,02fbh,02feh 
	dw 0301h,0303h,0306h,0309h,030ch,030fh,0311h,0314h,0317h,031ah 
	dw 031dh,0320h,0323h,0326h,0329h,032bh,032eh,0331h,0334h,0337h 
	dw 033ah,033dh,0340h,0343h,0346h,0349h,034ch,034fh,0352h,0356h 
	dw 0359h,035ch,035fh,0362h,0365h,0368h,036bh,036fh,0372h,0375h 
	dw 0378h,037bh,037fh,0382h,0385h,0388h,038ch,038fh,0392h,0395h 
	dw 0399h,039ch,039fh,03a3h,03a6h,03a9h,03adh,03b0h,03b4h,03b7h 
	dw 03bbh,03beh,03c1h,03c5h,03c8h,03cch,03cfh,03d3h,03d7h,03dah 
	dw 03deh,03e1h,03e5h,03e8h,03ech,03f0h,03f3h,03f7h,03fbh,03feh 
	dw 0fe01h,0fe03h,0fe05h,0fe07h,0fe08h,0fe0ah,0fe0ch,0fe0eh,0fe10h,0fe12h
	dw 0fe14h,0fe16h,0fe18h,0fe1ah,0fe1ch,0fe1eh,0fe20h,0fe21h,0fe23h,0fe25h 
	dw 0fe27h,0fe29h,0fe2bh,0fe2dh,0fe2fh,0fe31h,0fe34h,0fe36h,0fe38h,0fe3ah 
	dw 0fe3ch,0fe3eh,0fe40h,0fe42h,0fe44h,0fe46h,0fe48h,0fe4ah,0fe4ch,0fe4fh 
	dw 0fe51h,0fe53h,0fe55h,0fe57h,0fe59h,0fe5ch,0fe5eh,0fe60h,0fe62h,0fe64h 
	dw 0fe67h,0fe69h,0fe6bh,0fe6dh,0fe6fh,0fe72h,0fe74h,0fe76h,0fe79h,0fe7bh 
	dw 0fe7dh,0fe7fh,0fe82h,0fe84h,0fe86h,0fe89h,0fe8bh,0fe8dh,0fe90h,0fe92h 
	dw 0fe95h,0fe97h,0fe99h,0fe9ch,0fe9eh,0fea1h,0fea3h,0fea5h,0fea8h,0feaah 
	dw 0feadh,0feafh 

note_octave	db 0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1
	db 1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2 
	db 2,2,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4 
	db 4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5 
	db 5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,7 
	db 7,7,7,7,7,7,7,7,7,7,7

note_halftone	db 0,1,2,3,4,5,6,7,8,9,10,11,0,1,2,3,4
	db 5,6,7,8,9,10,11,0,1,2,3,4,5,6,7,8,9
	db 10,11,0,1,2,3,4,5,6,7,8,9,10,11,0,1,2
	db 3,4,5,6,7,8,9,10,11,0,1,2,3,4,5,6,7
	db 8,9,10,11,0,1,2,3,4,5,6,7,8,9,10,11,0
	db 1,2,3,4,5,6,7,8,9,10,11

array0_init     db 00100000b,0,0,01100000b,0,0,0,0,0,0,0,0,0,0,0   ;01-0f
                db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0                 ;10-1f
                db 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1                 ;20-2f
                db 1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0                 ;30-3f
                db 63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63 ;40-4f
                db 63,63,63,63,63,63,0,0,0,0,0,0,0,0,0,0           ;50-5f
                db 255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
                db 255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0     ;70-7f
                db 15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15 ;80-8f
                db 15,15,15,15,15,15,0,0,0,0,0,0,0,0,0,0           ;90-9f
                db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0                 ;a0-af
                db 0,0,0,0,0,0,0,0,0,0,0,0,0,DEF_AV_DEPTH,0,0      ;b0-bf
                db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0                 ;c0-cf
                db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0                 ;d0-df
                db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0                 ;e0-ef
                db 0,0,0,0,0,0                                     ;f0-f5

array1_init     db 0,0,0,0,00000001b,0,0,0,0,0,0,0,0,0,0           ;01-0f
                db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0                 ;10-1f
                db 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1                 ;20-2f
                db 1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0                 ;30-3f
                db 63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63 ;40-4f
                db 63,63,63,63,63,63,0,0,0,0,0,0,0,0,0,0           ;50-5f
                db 255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
                db 255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0     ;70-7f
                db 15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15 ;80-8f
                db 15,15,15,15,15,15,0,0,0,0,0,0,0,0,0,0           ;90-9f
                db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0                 ;a0-af
                db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0                 ;b0-bf
                db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0                 ;c0-cf
                db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0                 ;d0-df
                db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0                 ;e0-ef
                db 0,0,0,0,0,0                                     ;f0-f5
                             
vel_graph       db 82,85,88,91,94,97,100,103,106,109,112,115,118,121,124,127

                IFDEF STEREO_3812
pan_graph       db 0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30                      
                db 32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62                
                db 64,66,68,70,72,74,76,78,80,82,84,86,88,90,92,94                
                db 96,98,100,102,104,106,108,110,112,114,116,118,120,122,124,127
                db 127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127
                db 127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127
                db 127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127
                db 127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127
                ENDIF

op_0            db 0,1,2,6,7,8,12,13,14,18,19,20,24,25,26,30,31,32
op_1            db 3,4,5,9,10,11,15,16,17,21,22,23,27,28,29,33,34,35

op_index        db 0,1,2,3,4,5,8,9,10,11,12,13,16,17,18,19,20,21
                db 0,1,2,3,4,5,8,9,10,11,12,13,16,17,18,19,20,21

op_array        db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                db 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1

voice_num       db 0,1,2,3,4,5,6,7,8,0,1,2,3,4,5,6,7,8

voice_array     db 0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1

op4_base        db 1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0
alt_voice       db 3,4,5,0,1,2,-1,-1,-1,12,13,14,9,10,11,-1,-1,-1

                IFDEF YMF262
alt_op_0        db 6,7,8,0,1,2,-1,-1,-1,24,25,26,18,19,20,-1,-1,-1
alt_op_1        db 9,10,11,3,4,5,-1,-1,-1,27,28,29,21,22,23,-1,-1,-1
conn_sel        db 1,2,4,1,2,4,0,0,0,8,16,32,8,16,32,0,0,0

op4_voice       db 0,1,2,9,10,11

carrier_01      db 00b,01b,10b,01b
carrier_23      db 10b,10b,10b,11b
                ENDIF

                ;
                ;Misc. data
                ;

                IFDEF STEREO_3812
LFMADDR 	dw ?	
LFMDATA 	dw ?	
RFMADDR 	dw ?	
RFMDATA 	dw ?	
                ENDIF

                IFDEF SBPRO
MIXADDR         dw ?
MIXDATA         dw ?
                ENDIF

DATA_OUT        dw ?                            ;IO_addr+1
ADDR_STAT       dw ?                            ;IO_addr

                IFDEF ADLIBG                    ;(used during detection)
CTRL_ADDR       dw ?
CTRL_DATA       dw ?
                ENDIF

note_event_l    dw ?                            ;used for LRU counting
note_event_h    dw ?
timb_hist_l     dw MAX_TIMBS dup (?)            ;last note event count for LRU
timb_hist_h     dw MAX_TIMBS dup (?)
timb_offsets    dw MAX_TIMBS dup (?)            ;offsets of resident timbres
timb_bank       db MAX_TIMBS dup (?)            ;GTR bank
timb_num        db MAX_TIMBS dup (?)            ;GTR #
timb_attribs    db MAX_TIMBS dup (?)            ;bit 7=in use 6=protected

cache_base      dd ?                            ;timbre cache base addr
cache_size      dw ?                            ;total cache size in bytes
cache_end       dw ?                            ;offset of first free byte

TV_accum        dw ?                            ;DDA accum for refresh timing
pri_accum       dw ?                            ;DDA accum for priority watch
vol_update      db ?                            ;0 | U_KSLTL

rover_2op       dw ?                            ;circular voice # counters
rover_4op       dw ?

conn_shadow     db ?                            ;OPL3 Connection Select copy

BNK             STRUC           ;.BNK-style timbre definition
B_length        dw ?
B_transpose     db ?
B_mod_AVEKM     db ?            ;op_0 = FM modulator
B_mod_KSLTL     db ?                
B_mod_AD        db ?
B_mod_SR        db ?
B_mod_WS        db ?
B_fb_c          db ?
B_car_AVEKM     db ?            ;op_1 = FM carrier
B_car_KSLTL     db ?                
B_car_AD        db ?
B_car_SR        db ?
B_car_WS        db ?
                ENDS

OPL3BNK         STRUC           ;.BNK-style OPL3 timbre definition
                BNK <>
O_mod_AVEKM     db ?            ;op_2
O_mod_KSLTL     db ?                
O_mod_AD        db ?
O_mod_SR        db ?
O_mod_WS        db ?
O_fb_c          db ?
O_car_AVEKM     db ?            ;op_3
O_car_KSLTL     db ?                
O_car_AD        db ?
O_car_SR        db ?
O_car_WS        db ?
                ENDS

S_timbre_off    dw NUM_SLOTS dup (?)  ;pointer to timbre in local cache
S_timbre_seg    dw NUM_SLOTS dup (?)  ;""
S_duration      dw NUM_SLOTS dup (?)  ;# of TV intervals left in keyon
S_status        db NUM_SLOTS dup (?)  ;2=key off, 1=key on, 0=slot free
S_type          db NUM_SLOTS dup (?)  ;0=BNK, 1=TV inst, 2=TV effect, 3=OPL3
S_voice         db NUM_SLOTS dup (?)  ;YM3812 voice 0-8 or -1 assigned to slot
S_channel       db NUM_SLOTS dup (?)  ;MIDI channel owning slot
S_note          db NUM_SLOTS dup (?)  ;MIDI note # for slot's voice
S_keynum        db NUM_SLOTS dup (?)  ;MIDI key # before RBS translation
S_transpose     db NUM_SLOTS dup (?)  ;MIDI note transposition for slot
S_velocity      db NUM_SLOTS dup (?)  ;keyon velocity for note
S_sustain       db NUM_SLOTS dup (?)  ;note sustained if nonzero
S_update        db NUM_SLOTS dup (?)  ;bit mask for YM3812 register updates

S_KBF_shadow    db NUM_SLOTS dup (?)  ;shadowed KON-BLOCK-FNUM(H) registers
S_BLOCK         db NUM_SLOTS dup (?)  ;KON/BLOCK values
S_FBC           db NUM_SLOTS dup (?)  ;YM3812 multi-purpose registers
S_KSLTL_0       db NUM_SLOTS dup (?)  
S_KSLTL_1       db NUM_SLOTS dup (?)  
S_AVEKM_0       db NUM_SLOTS dup (?)  
S_AVEKM_1       db NUM_SLOTS dup (?)  
S_AD_0          db NUM_SLOTS dup (?)  ;YM3812 envelope registers
S_AD_1          db NUM_SLOTS dup (?)
S_SR_0          db NUM_SLOTS dup (?)
S_SR_1          db NUM_SLOTS dup (?)

S_scale_01      db NUM_SLOTS dup (?)  ;level scaling flags for ops 0-1

                IF NOT OSI_ALE
S_ws_val        dw NUM_SLOTS dup (?)  ;YM3812 registers
S_m1_val        dw NUM_SLOTS dup (?)  ;(declared in ALE.INC if TVFX enabled)
S_m0_val        dw NUM_SLOTS dup (?)
S_fb_val        dw NUM_SLOTS dup (?)
S_p_val         dw NUM_SLOTS dup (?)
S_v1_val        dw NUM_SLOTS dup (?)
S_v0_val        dw NUM_SLOTS dup (?)
S_f_val         dw NUM_SLOTS dup (?)  
                ENDIF

                IFDEF YMF262
S_KSLTL_2       db NUM_SLOTS dup (?)  ;YMF262 registers for operators 3-4
S_KSLTL_3       db NUM_SLOTS dup (?)  
S_AVEKM_2       db NUM_SLOTS dup (?)  
S_AVEKM_3       db NUM_SLOTS dup (?)  
S_AD_2          db NUM_SLOTS dup (?)
S_AD_3          db NUM_SLOTS dup (?)
S_SR_2          db NUM_SLOTS dup (?)
S_SR_3          db NUM_SLOTS dup (?)

S_ws_val_2      dw NUM_SLOTS dup (?)
S_m3_val        dw NUM_SLOTS dup (?)
S_m2_val        dw NUM_SLOTS dup (?)
S_v3_val        dw NUM_SLOTS dup (?)
S_v2_val        dw NUM_SLOTS dup (?)

S_scale_23      db NUM_SLOTS dup (?)  ;level scaling flags for ops 2-3
                ENDIF

FREE            equ 0                 ;S_status[] phase equates
KEYON           equ 1
KEYOFF          equ 2

BNK_INST        equ 0                 ;S_type[] equates
TV_INST         equ 1
TV_EFFECT       equ 2
OPL3_INST       equ 3

U_ALL_REGS      equ 11111001b         ;Bit mask equates for S_update
U_AVEKM         equ 10000000b           
U_KSLTL         equ 01000000b
U_ADSR          equ 00100000b
U_WS            equ 00010000b
U_FBC           equ 00001000b
U_FREQ          equ 00000001b

MIDI_vol        db NUM_CHANS dup (?)  ;volume 
MIDI_pan        db NUM_CHANS dup (?)  ;panpot
MIDI_pitch_l    db NUM_CHANS dup (?)  ;pitchwheel LSB
MIDI_pitch_h    db NUM_CHANS dup (?)  ;pitchwheel MSB
MIDI_express    db NUM_CHANS dup (?)  ;expression 
MIDI_mod        db NUM_CHANS dup (?)  ;modulation 
MIDI_sus        db NUM_CHANS dup (?)  ;HOLD1 pedal 
MIDI_vprot      db NUM_CHANS dup (?)  ;voice protection
MIDI_timbre     db NUM_CHANS dup (?)  ;timbre cache index for each channel
MIDI_bank       db NUM_CHANS dup (?)  ;Patch Bank Select values
MIDI_program    db NUM_CHANS dup (?)  ;program change # / channel

RBS_timbres     db 128 dup (?)        ;RBS timbre offset cache

MIDI_voices     db NUM_CHANS dup (?)  ;# of voices assigned to channel

V_channel       db NUM_VOICES dup (?) ;voice assigned to MIDI channel n or -1

S_V_priority    dw NUM_SLOTS dup (?)  ;adjusted voice priorities

;****************************************************************************
;*                                                                          *
;* I/O routines                                                             *
;*                                                                          *
;****************************************************************************

set_IO_parms    PROC IO_ADDR            ;Set I/O address parms for adapter
                USES ds,si,di           

                IFDEF PAS

                mov LFMADDR,388h          
                mov LFMDATA,389h          
                mov RFMADDR,38ah          
                mov RFMDATA,38bh          
                mov ax,LFMADDR 

                ELSEIFDEF PASOPL

                mov ax,388h

                ELSE

                mov ax,[IO_ADDR]

                ENDIF

                IFDEF SBSTD

                add ax,8             

                ELSEIFDEF SBPRO1

                mov LFMADDR,ax
                inc ax
                mov LFMDATA,ax
                inc ax
                mov RFMADDR,ax
                inc ax
                mov RFMDATA,ax
                inc ax
                mov MIXADDR,ax
                inc ax
                mov MIXDATA,ax
                add ax,3

                ELSEIFDEF SBPRO2

                push ax
                add ax,4
                mov MIXADDR,ax
                inc ax
                mov MIXDATA,ax
                pop ax

                ENDIF

                mov ADDR_STAT,ax
                inc ax
                mov DATA_OUT,ax

                ret
                ENDP

;****************************************************************************

;
;YM3812/YMF262 register access routines must preserve DS, SI, DI!
;

write_register  PROC Operator:BYTE,Base:BYTE,Value:BYTE
                mov bl,[Operator]
                mov bh,0
                mov ah,op_array[bx]
                mov bl,op_index[bx]
                add bl,[Base]
                mov bh,ah
                mov cl,[Value]
                jmp do_update
                ENDP

send_byte       PROC Voice:WORD,Base:BYTE,Data:BYTE
                mov bx,[Voice]
                mov ah,voice_array[bx]
                mov bl,voice_num[bx]
                add bl,[Base]
                mov bh,ah
                mov cl,[Data]
                jmp do_update
                ENDP

do_update:      pop bp                  ;discard stack frame

update_reg      PROC                    ;Write value CL to register BX
                                        ;(preserves BX)
                IFDEF STEREO_3812

                mov al,bl               ;AL=address, CL=data
                mov dx,LFMADDR          ;select left register address
                out dx,al               
                mov dx,RFMADDR          ;select right register address
                out dx,al
                mov dx,LFMADDR          ;delay 3.3 uS

                mov ah,6
__rept_1:       in al,dx
                dec ah
                jne __rept_1

                mov al,cl
                mov dx,LFMDATA          ;write left data
                out dx,al
                mov dx,RFMDATA          ;write right data (same as left)
                out dx,al
                mov dx,LFMADDR          ;delay 23 uS (3.3 for YMF262)

                mov ah,IOWVAL
__rept_2:       in al,dx
                dec ah
                jne __rept_2

                ELSE

                mov dx,ADDR_STAT

                IFDEF YMF262            ;index 2nd array if addr > 256
                add dl,bh               
                adc dh,0
                add dl,bh               
                adc dh,0
                ENDIF

                mov al,bl               ;AL=address, CL=data
                out dx,al               ;select register address

                mov ah,6
__rept_3:       in al,dx
                dec ah
                jne __rept_3

                mov al,cl
                inc dx
                out dx,al
                dec dx

                mov ah,IOWVAL
__rept_4:       in al,dx
                dec ah
                jne __rept_4

                ENDIF
                ret
                ENDP
                
                IFDEF STEREO_3812       ;Access 3812 stereo registers
stereo_register PROC Part:BYTE,Base:BYTE,RLValues:WORD
                mov bl,[Part]
                mov bh,0
                mov al,[Base]
                add al,op_index[bx]
                mov dx,LFMADDR          ;select left register address
                out dx,al               
                mov dx,RFMADDR          ;select right register address
                out dx,al
                mov dx,LFMADDR          ;delay 3.3 uS

                mov ah,6
__rept_1:       in al,dx
                dec ah
                jne __rept_1

                mov dx,LFMDATA          ;write left data
                mov al,BYTE PTR [RLValues+1]
                out dx,al
                mov dx,RFMDATA          ;write right data
                mov al,BYTE PTR [RLValues]
                out dx,al
                mov dx,LFMADDR          ;delay 23 uS

                mov ah,42
__rept_2:       in al,dx
                dec ah
                jne __rept_2

                ret
                ENDP
                ENDIF

read_status     PROC                    ;Read YM3812 status register
                mov dx,ADDR_STAT        
                in al,dx
                mov ah,0
                ret
                ENDP

;****************************************************************************
                IFDEF ADLIBG            

IO_wait         PROC                    ;Wait for clear SB bit (Ad Lib Gold)
                mov cx,500
                mov dx,CTRL_ADDR
__wait:         in al,dx
                and al,01000000b
                loopnz __wait
                ret
                ENDP

get_FM_vol      PROC RegNum             ;Get FM VOLUME register value
                call IO_wait
                mov dx,CTRL_ADDR
                mov ax,[RegNum]
                out dx,al
                call IO_wait
                mov dx,CTRL_DATA
                in al,dx
                ret
                ENDP

set_FM_vol      PROC RegNum,Val         ;Set FM VOLUME register value
                call IO_wait
                mov dx,CTRL_ADDR
                mov ax,[RegNum]
                out dx,al
                call IO_wait
                mov dx,CTRL_DATA
                mov ax,[Val]
                out dx,al
                ret
                ENDP
                ENDIF

detect_device   PROC H,IO_ADDR,IRQ,DMA,DRQ  
                USES ds,si,di             ;Attempt to detect card

                pushf
                cli

                IFDEF ADLIBG              ;if Ad Lib Gold, look for control
                                          ;chip
                mov dx,IO_ADDR
                add dx,2
                mov CTRL_ADDR,dx
                inc dx
                mov CTRL_DATA,dx

                mov dx,CTRL_ADDR          ;attempt to enable control chip
                mov al,0ffh               
                out dx,al

                call get_FM_vol C,9       ;get left volume
                mov si,ax
                call get_FM_vol C,10      ;get right volume
                mov di,ax

                xor si,0101b              ;tweak a few bits
                xor di,1010b

                call set_FM_vol C,9,si    ;write the tweaked values back
                call set_FM_vol C,10,di

                call get_FM_vol C,9       ;see if changes took effect
                cmp ax,si
                mov ax,0                  ;(return failure)
                jne __return
                call get_FM_vol C,10
                cmp ax,di
                mov ax,0                  ;(return failure)
                jne __return

                xor si,0101b              ;control chip found: restore old
                xor di,1010b              ;values & re-enable FM sound

                call set_FM_vol C,9,si
                call set_FM_vol C,10,di

                call IO_wait
                mov dx,CTRL_ADDR          
                mov al,0feh               
                out dx,al

                mov ax,1                  ;return success
                jmp __return

                ELSE                      ;(not Ad Lib Gold card...)

                push DATA_OUT             ;preserve current I/O addresses
                push ADDR_STAT
                IFDEF STEREO_3812
                push LFMADDR
                push RFMADDR
                push LFMDATA
                push RFMDATA
                ENDIF
                IFDEF SBPRO
                push MIXDATA
                push MIXADDR
                ENDIF

                call set_IO_parms C,[IO_ADDR]

                IFDEF SBPRO2              ;do Ad Lib detection at 2x8, NOT 2x0
                add DATA_OUT,8            ;(avoids hangups on standard SB)
                add ADDR_STAT,8
                ENDIF                     

                call detect_Adlib         ;do Ad Lib compatibility test first
                cmp ax,0
                je __exit

                IFDEF SBPRO               ;then look for CT-1345A mixer chip
                mov dx,MIXADDR
                mov al,0ah                ;select Mic Vol control
                out dx,ax
                jmp $+2
                mov dx,MIXDATA
                in al,dx                  ;get original value
                jmp $+2
                mov ah,al                 ;save it
                xor al,110b               ;toggle its bits
                out dx,al                 ;write it back
                jmp $+2
                in al,dx                  ;read/verify changed value
                xor al,110b              
                cmp al,ah
                mov al,ah                 ;put the old value back
                out dx,al
                mov ax,0
                jne __exit 
                mov ax,1                  ;signal card found -- it's a SB PRO
                ENDIF

                IFDEF PROAUDIO            ;if Pro Audio Spectrum, look for 
                mov ax,0bc00h             ;MVSOUND.SYS device driver
                mov bx,03f3fh
                int 2fh                   ;DOS MPX interrupt
                xor bx,cx
                xor bx,dx
                cmp bx,'MV'               ;MediaVision flag
                mov ax,0
                jne __exit
                mov cx,0
                mov ax,0bc03h             ;get function table address
                int 2fh
                cmp ax,'MV'
                mov ax,0
                jne __exit
                cmp cx,10                 ;bail out if no FM Split routine
                jb __exit                 ;present (MVSOUND version < 1.02)
                mov es,dx
                mov di,bx
                mov bx,100
                mov cx,1
                call DWORD PTR es:[di+36] ;set FM Split = stereo mode
                mov ax,1                  ;signal PAS card found
                ENDIF

__exit:         IFDEF SBPRO
                pop MIXADDR
                pop MIXDATA
                ENDIF
                IFDEF STEREO_3812
                pop RFMDATA
                pop LFMDATA
                pop RFMADDR
                pop LFMADDR
                ENDIF
                pop ADDR_STAT
                pop DATA_OUT

                ENDIF                     ;IFDEF ADLIBG

__return:       POP_F                     ;return AX=0 if not found
                ret
                ENDP

                IFNDEF ADLIBG
detect_send     PROC Addr:BYTE,Data:BYTE  ;Write data byte to specified AL reg
                mov dx,ADDR_STAT        
                mov al,[Addr]
                out dx,al                 ;select register address
                mov cx,6
__3_3_us:       in al,dx                  ;delay 3.3 uS
                loop __3_3_us
                mov dx,DATA_OUT
                mov al,[Data]
                out dx,al
                mov dx,ADDR_STAT         
                mov cx,42
__23_us:        in al,dx                  ;delay 23 uS
                loop __23_us
                ret
                ENDP

detect_Adlib    PROC                      ;Detect standard YM3812 timer regs
                USES ds,si,di
                call detect_send C,4,60h  ;reset T1 and T2
                call detect_send C,4,80h  ;reset IRQ
                call read_status
                mov di,ax                 ;save timer status
                call detect_send C,2,0ffh ;set T1 to 0FFh
                call detect_send C,4,21h  ;unmask and start T1
                mov si,200                ;wait 100 uS for timer to count down
__wait_100_uS:  call read_status
                dec si
                jnz __wait_100_uS
                mov si,ax                 ;save timer status
                call detect_send C,4,60h  ;reset T1 and T2
                call detect_send C,4,80h  ;reset IRQ
                and si,0e0h               ;mask off undefined bits
                and di,0e0h
                mov ax,0                  ;assume board not detected
                cmp di,0
                jne __return              ;initial timer value not 0, exit
                cmp si,0c0h
                jne __return              ;timer didn't overflow, exit
                mov ax,1                  ;else Ad Lib-compatible board exists
__return:       ret
                ENDP
                ENDIF

;****************************************************************************
reset_synth     PROC                      ;Init regs & register map 
                USES ds,si,di

                pushf                  
                cli

                IFDEF YMF262
                mov bx,105h
                mov cl,00000001b          ;set OPL3 NEW bit
                call update_reg

                mov bx,104h               ;init OPL3 to 18 2-op voices
                mov cl,0
                call update_reg
                mov conn_shadow,0
                ENDIF

                mov bx,1h
__init_0:       mov cl,array0_init-1[bx]
                call update_reg
                inc bx
                cmp bx,0f5h
                jbe __init_0

                IFDEF YMF262
                mov bx,101h
__init_1:       mov cl,array1_init-101h[bx]
                call update_reg
                inc bx
                cmp bx,1f5h
                jbe __init_1
                ENDIF

                POP_F
                ret
                ENDP

;****************************************************************************
shutdown_synth  PROC                      ;Establish Ad Lib compatibility mode
                USES ds,si,di

                pushf
                cli

                IFDEF YMF262
                mov bx,105h
                mov cl,0                  ;clear OPL3 NEW bit
                call update_reg
                ENDIF

                IFDEF PAS                 ;reset 3812-based card to Ad Lib
                mov ax,0bc07h             ;function 7: get state table entries
                int 2fh
                or bl,10000000b           ;clear FM split (bit 7)
                mov al,bl                 ;...and write to mixer port
                mov dx,0b88h
                out dx,al
                ENDIF

                POP_F
                ret
                ENDP

;****************************************************************************
;*                                                                          *
;*  Timbre cache management / related API calls                             *
;*                                                                          *
;****************************************************************************

index_timbre    PROC GNum               ;Get global timbre's local index 
                USES ds,si,di

                mov si,0
                mov ax,[GNum]   
__find_gnum:    test timb_attribs[si],10000000b
                jz __find_next          ;(timbre unused)
                cmp timb_bank[si],ah
                jne __find_next
                cmp timb_num[si],al
                je __found
__find_next:    inc si
                cmp si,MAX_TIMBS
                jb __find_gnum

                mov si,-1               ;return -1 if timbre not loaded

__found:        mov ax,si
                ret
                ENDP

;****************************************************************************
protect_timbre  PROC H,Bank:BYTE,Num:BYTE
                USES ds,si,di           ;Protect a timbre from replacement
                pushf
                cli

                mov al,[Num]
                mov ah,[Bank]

                cmp ax,-1
                je __prot_all

                call index_timbre C,ax
                cmp ax,-1
                je __exit               ;timbre not loaded, can't protect it

                mov bx,ax
                or timb_attribs[bx],01000000b
                jmp __exit

__prot_all:     mov bx,0
__prot_timb:    or timb_attribs[bx],01000000b
                inc bx
                cmp bx,MAX_TIMBS
                jb __prot_timb

__exit:         POP_F
                ret
                ENDP

;****************************************************************************
unprotect_timbre PROC H,Bank:BYTE,Num:BYTE            
                USES ds,si,di           ;Allow a timbre to be replaced
                pushf 
                cli

                mov al,[Num]
                mov ah,[Bank]

                cmp ax,-1
                je __unprot_all

                call index_timbre C,ax
                cmp ax,-1
                je __exit               ;timbre not loaded, can't unprotect it

                mov bx,ax
                and timb_attribs[bx],10111111b
                jmp __exit

__unprot_all:   mov bx,0
__unprot_timb:  and timb_attribs[bx],10111111b
                inc bx
                cmp bx,MAX_TIMBS
                jb __unprot_timb

__exit:         POP_F
                ret
                ENDP

;****************************************************************************
timbre_status   PROC H,Bank:BYTE,Num:BYTE            
                USES ds,si,di           ;Return 0 if timbre not resident
                pushf 
                cli

                mov al,[Num]
                mov ah,[Bank]

                call index_timbre C,ax
                cmp ax,-1
                je __exit               ;not resident, inc AX to return 0

                mov si,ax               ;else return offset+1 in local cache
                shl si,1
                mov ax,timb_offsets[si] ;(for diagnostic purposes)

__exit:         inc ax

                POP_F
                ret
                ENDP

;****************************************************************************
get_cache_size  PROC H
                USES ds,si,di
                pushf
                cli

                mov ax,DEF_TC_SIZE

                POP_F
                ret
                ENDP

;****************************************************************************
define_cache    PROC H,Addr:FAR PTR,Size
                USES ds,si,di
                pushf
                cli

                les di,[Addr]
                FAR_TO_HUGE es,di
                mov WORD PTR cache_base,di
                mov WORD PTR cache_base+2,es

                mov ax,[Size]
                mov cache_size,ax

                mov cache_end,0

                POP_F
                ret
                ENDP

;****************************************************************************
get_request     PROC H,Sequence
                USES ds,si,di
                pushf
                cli

                mov si,[Sequence]
                cmp si,-1
                je __no_request
                lds si,sequence_state[si]

                cmp WORD PTR [si].TIMB+2,0
                je __no_request         ;no requested timbres, exit

                lds si,[si].TIMB        ;make sure TIMB chunk is present
                cmp [si],'IT'
                jne __no_request        ;if not, no requests are possible
                cmp [si+2],'BM'
                jne __no_request

                add si,8
                mov di,[si]             ;get TIMB.cnt
__chk_index:    add si,2
                mov ax,[si]
                call index_timbre C,[si]
                cmp ax,-1               ;timbre in local cache?
                je __request            ;no, request it
__next_index:   dec di
                jne __chk_index
                jmp __no_request        ;all requested timbres loaded, exit

__request:      mov ax,[si]             ;else return request: AL=num, AH=bank
                jmp __exit

__no_request:   mov ax,-1
                
__exit:         POP_F
                ret
                ENDP

;****************************************************************************
delete_LRU      PROC                    ;Excise LRU timbre from cache, adjust
                LOCAL index,tsize,toff  ;links & references
                USES ds,si,di           
                cld

                mov index,-1
                mov ax,-1           
                mov bx,0                
                mov dx,-1
                mov si,0                
__find_LRU:     test timb_attribs[si],10000000b
                jz __next_LRU           ;(timbre not installed)
                test timb_attribs[si],01000000b
                jnz __next_LRU          ;(timbre protected)
                cmp timb_hist_h[bx],dx
                ja __next_LRU           ;(timbre not least-recently-used)
                jb __log_LRU
                cmp timb_hist_l[bx],ax
                ja __next_LRU
__log_LRU:      mov ax,timb_hist_l[bx]
                mov dx,timb_hist_h[bx]
                mov index,si
__next_LRU:     add bx,2
                inc si
                cmp si,MAX_TIMBS
                jb __find_LRU

                cmp index,-1        
                je __exit

                mov si,index            ;SI = LRU timbre index
                shl si,1
                mov ax,timb_offsets[si]
                mov toff,ax
                les di,cache_base
                add di,ax               ;ES:DI -> timbre to delete
                mov si,es:[di]
                mov tsize,si
                add si,di               ;ES:SI -> next timbre in cache

                mov cx,WORD PTR cache_base
                add cx,cache_end
                sub cx,si               ;CX = size of area to move down 

                push es
                pop ds
                REP_MOVSB

                mov di,index            ;clear attributes of deleted timbre
                mov timb_attribs[di],00000000b
                mov ax,tsize            ;adjust end-of-cache pointer
                sub cache_end,ax                

                mov di,0                ;invalidate channel references to
                mov bh,0                ;the deleted timbre
__for_chan:     mov bl,MIDI_timbre[di]  
                cmp bl,-1
                je __next_chan
                cmp bx,index
                jne __next_chan
                mov MIDI_timbre[di],-1
__next_chan:    inc di
                cmp di,NUM_CHANS
                jne __for_chan
                mov di,0
__for_RBS:      mov bl,RBS_timbres[di]
                cmp bx,index
                jne __next_RBS
                mov RBS_timbres[di],-1
__next_RBS:     inc di
                cmp di,SIZE RBS_timbres
                jne __for_RBS

                mov di,0                ;adjust offset pointers for all
                mov bx,0                ;timbres after the deleted timbre
__for_index:    test timb_attribs[bx],10000000b
                jz __next_index
                mov ax,timb_offsets[di] 
                cmp ax,toff
                jbe __next_index
                sub ax,tsize
                mov timb_offsets[di],ax
__next_index:   add di,2
                inc bx
                cmp bx,MAX_TIMBS
                jb __for_index

                mov ax,WORD PTR cache_base
                add toff,ax
                mov di,0                ;adjust pointers to any active timbres
                mov si,0                
__for_slot:     cmp S_status[si],FREE
                je __next_slot
                mov ax,S_timbre_off[di]
                cmp ax,toff
                jb __next_slot
                jne __move_down
                call release_voice C,si
                mov S_status[si],FREE
                jmp __next_slot
__move_down:    sub ax,tsize
                mov S_timbre_off[di],ax
__next_slot:    add di,2
                inc si
                cmp si,NUM_SLOTS
                jne __for_slot

__exit:         ret
                ENDP

;****************************************************************************
install_timbre  PROC H,Bank:BYTE,Num:BYTE,Addr:FAR PTR
                LOCAL index
                USES ds,si,di
                pushf
                cli
                cld

                mov al,[Num]
                mov ah,[Bank]
                call index_timbre C,ax
                mov index,ax
                cmp ax,-1
                jne __set_patch         ;if timbre already resident, index it

                mov ax,WORD PTR [Addr]
                or ax,WORD PTR [Addr+2]
                jz __exit

__get_space:    mov di,0                ;find first available timbre slot
__find_index:   test timb_attribs[di],10000000b
                jz __get_size           ;found it....
                inc di
                cmp di,MAX_TIMBS
                jne __find_index
                jmp __kill_LRU          ;else kill a timbre to free up a slot

__get_size:     lds si,[Addr]           
                mov bx,[si]             ;get new timbre size
                add bx,cache_end
                cmp bx,cache_size       ;enough free space in local cache?
                jbe __install           ;yes, install it

__kill_LRU:     call delete_LRU         ;get rid of old timbres until the
                jmp __get_space         ;new timbre can be installed

__install:      xchg cache_end,bx

                mov index,di            ;save index of new timbre

                mov al,[Num]            ;AL=num, AH=bank
                mov ah,[Bank]
                mov timb_num[di],al     ;record global # in slot
                mov timb_bank[di],ah    ;mark timbre "in use/unprotected"
                mov timb_attribs[di],10000000b                  
                shl di,1                ;update timbre's timestamp
                mov ax,note_event_l     
                mov dx,note_event_h     
                add note_event_l,1      
                adc note_event_h,0      
                mov timb_hist_l[di],ax  
                mov timb_hist_h[di],dx  
                mov timb_offsets[di],bx

                les di,cache_base       ;DS:SI -> new timbre source addr
                add di,bx               ;ES:DI -> cache address for new timbre
                mov cx,[si]             ;CX = size of new timbre
                REP_MOVSB

__set_patch:    mov al,[Num]                                     
                mov ah,[Bank]
                mov bx,index
                mov di,0                ;for all channels...
__set_requests: cmp MIDI_program[di],al ; if timbre was requested in channel,
                jne __next_req          ; update channel's timbre index
                cmp MIDI_bank[di],ah
                jne __next_req
                mov MIDI_timbre[di],bl
__next_req:     inc di
                cmp di,NUM_CHANS
                jne __set_requests

__exit:         POP_F
                ret
                ENDP

;****************************************************************************
;*                                                                          *
;*  MIDI interpreter and related procedures                                 *
;*                                                                          *
;****************************************************************************

init_synth      PROC                    ;Init MIDI synthesizer emulation
                USES ds,si,di
                pushf           
                cli
                cld

                mov note_event_l,0
                mov note_event_h,0

                mov di,0
__init_timbres: mov timb_attribs[di],00000000b
                inc di
                cmp di,MAX_TIMBS
                jne __init_timbres

                mov di,0
__init_chans:   mov MIDI_timbre[di],-1
                mov MIDI_voices[di],0
                mov MIDI_program[di],-1
                mov MIDI_bank[di],0
                inc di
                cmp di,NUM_CHANS
                jne __init_chans

                mov di,0
__init_slots:   mov S_status[di],FREE
                inc di
                cmp di,NUM_SLOTS
                jne __init_slots

                mov di,0
__init_voices:  mov V_channel[di],-1
                inc di
                cmp di,NUM_VOICES
                jne __init_voices

                push cs
                pop es
                lea di,RBS_timbres
                mov cx,SIZE RBS_timbres
                mov al,-1
                rep stosb 

                mov TV_accum,0
                mov pri_accum,0
                mov vol_update,0
                mov rover_2op,-1
                mov rover_4op,-1

                POP_F
                ret
                ENDP

;****************************************************************************
assign_voice    PROC Slot               ;Allocate hardware voice to slot
                USES ds,si,di
                
                IFDEF YMF262
                mov si,[Slot]           
                cmp S_type[si],OPL3_INST
                je __OPL3
                ENDIF

                mov dx,-1               ;try to find an unassigned voice
                mov bx,rover_2op
__search_free:  inc dx
                cmp dx,NUM_VOICES
                je __seize_voice        ;(# of active slots > # of voices)
                inc bx
                IF CIRC_ASSIGN
                cmp bx,NUM_VOICES
                jne __chk_free
                mov bx,0
__chk_free:     mov rover_2op,bx
                ENDIF
                cmp V_channel[bx],-1   
                jne __search_free

                mov S_voice[si],bl      ;found free voice, assign it to slot
                mov di,bx
                mov bl,S_channel[si]
                inc MIDI_voices[bx]
                mov V_channel[di],bl

__update:       mov S_update[si],U_ALL_REGS
                call update_voice C,si  ;update the hardware
                ret

__seize_voice:  call update_priority    ;assign voice based on priority search
                ret                     

                IFDEF YMF262
__OPL3:         mov dx,-1               ;assigning 4-op voice...
                mov di,rover_4op
                mov bh,0
__OPL3_search:  inc dx                  ;try to find unassigned eligible pair
                cmp dx,NUM_4OP_VOICES
                je __seize_voice
                inc di
                IF CIRC_ASSIGN
                cmp di,NUM_4OP_VOICES
                jne __OPL3_free
                mov di,0
__OPL3_free:    mov rover_4op,di
                ENDIF
                mov bl,op4_voice[di]
                cmp V_channel[bx],-1
                jne __OPL3_search
                cmp V_channel[bx+3],-1
                jne __OPL3_search

                mov S_voice[si],bl      
                mov di,bx
                mov bl,S_channel[si]
                inc MIDI_voices[bx]
                mov V_channel[di],bl
                mov V_channel[di+3],bl
                jmp __update
                ENDIF

                ENDP

;****************************************************************************
release_voice   PROC Slot               ;Release slot's voice
                USES ds,si,di

                mov si,[Slot]

                cmp S_voice[si],-1   
                je __exit            

                and S_BLOCK[si],11011111b
                or S_update[si],U_FREQ  ;force KON = 0...

                call update_voice C,si  ;...silence any note...

                mov bh,0                ;...and deallocate the voice
                mov bl,S_channel[si]     
                dec MIDI_voices[bx]
                mov bl,S_voice[si]

                cmp S_type[si],OPL3_INST
                jne __free_chan
                mov V_channel[bx+3],-1  ;free both "halves" of 4-op OPL3
__free_chan:    mov V_channel[bx],-1    ;voice

                mov S_voice[si],-1

                cmp S_type[si],OPL3_INST
                je __release            ;release any .BNK slots deallocated...
                cmp S_type[si],BNK_INST
                jne __exit              
__release:      mov S_status[si],FREE   ;otherwise, slot remains active

__exit:         ret
                ENDP
                    
;****************************************************************************
update_voice    PROC Slot               ;Update hardware regs for slot
                USES ds,si,di
                LOCAL voice,voice0,voice1,vol:BYTE,lvol:BYTE,rvol:BYTE
                LOCAL f_num
                LOCAL array,update_save:BYTE
                LOCAL scale_flags:BYTE
                LOCAL m0_val,m1_val,AVEKM_0:BYTE,AVEKM_1:BYTE
                LOCAL v0_val,v1_val,KSLTL_0:BYTE,KSLTL_1:BYTE
                LOCAL AD_0:BYTE,AD_1:BYTE,SR_0:BYTE,SR_1:BYTE
                LOCAL ws_val,fb_val,FBC:BYTE

                mov si,[Slot]           ;update only requested parameters for
                                        ;speed
                cmp S_voice[si],-1
                je __exit               ;no hardware voice assigned, exit

                test S_update[si],U_KSLTL
                jz __chk_type           ;skip level calculations if not needed

                mov di,WORD PTR S_channel[si]
                and di,0fh              ;DI = MIDI channel

                mov al,MIDI_vol[di]
                mul MIDI_express[di]
                shl ax,1                ;(AX*2)/256 = AX/128 ї AX/127
                mov al,ah
                cmp al,1
                sbb al,-1               ;(error = 1/127 units; round up if !0)
                mul S_velocity[si]
                shl ax,1
                mov al,ah
                cmp al,1
                sbb al,-1               ;(error = 1/127 units; round up if !0)
                mov vol,al              ;AX=composite (vol+expression) volume

                IFDEF STEREO_3812       ;interpret panpot for stereo cards
                mov cx,ax
                mov bh,0
                mov bl,MIDI_pan[di]
                mov al,pan_graph[bx]
                mul cl                  ;calculate left-channel volume
                shl ax,1                ;(AX*2)/256 = AX/128 ї AX/127
                mov al,ah
                cmp al,1
                sbb al,-1               ;(error = 1/127 units; round up if !0)
                mov lvol,al             ;set left volume
                sub bx,127
                neg bx
                mov al,pan_graph[bx]
                mul cl                  ;calculate right-channel volume
                shl ax,1                ;(AX*2)/256 = AX/128 ї AX/127
                mov al,ah
                cmp al,1
                sbb al,-1               ;(error = 1/127 units; round up if !0)
                mov rvol,al             ;set right volume
                ENDIF

__chk_type:     mov ax,0
                cmp S_type[si],OPL3_INST
                jne __set_array
                mov ax,1                ;write 2nd array first if OPL3 4-op
__set_array:    mov array,ax

                IFDEF YMF262            ;update OPL3 CONN SEL register
                mov di,WORD PTR S_voice[si]
                and di,0ffh             ;DI = voice
                mov cl,conn_shadow
                mov dl,conn_sel[di]
                or ax,ax
                jz __2_op

                or cl,dl                ;turning on 4-op voice...
                cmp cl,conn_shadow      ;CS changed (replacing 2 2-op voices?)
                je __do_arrays          ;no, 4-op connection is already valid
                mov conn_shadow,cl      ;else enable 4-op connection select
                mov bx,104h
                call update_reg
                jmp __do_arrays
                
__2_op:         not dl                  ;turning on 2-op voice...
                and cl,dl
                cmp cl,conn_shadow      ;CS changed (replacing 1 4-op voice?)
                je __do_arrays          ;no, voice already in 2-op mode
                mov conn_shadow,cl      ;else disable 4-op connection select
                mov bx,104h
                call update_reg         
                                        ;mute "other half" of old 4-op voice
                mov al,alt_op_0[di]     ;set modulator release = immed
                call write_register C,ax,080h,0fh

                mov al,alt_op_1[di]     ;set carrier release = immed
                call write_register C,ax,080h,0fh

                mov al,alt_voice[di]    ;set KON = 0 to force note off
                mov ah,0
                call send_byte C,ax,0b0h,0
                ENDIF

__do_arrays:    cmp array,0
                je __array_0

                IFDEF YMF262
                mov ah,0
                mov bh,0
                mov bl,S_voice[si]
                add bx,3                ;index 2nd operator pair at v+3
                mov voice,bx
                mov al,op_0[bx]
                mov voice0,ax           ;"modulator" operator cell (2nd pair)
                mov al,op_1[bx]
                mov voice1,ax           ;"carrier" operator cell (2nd pair)

                mov bx,si               ;set op0 and op1 parms for 2nd pair...
                mov ax,S_m2_val[bx][si]
                mov m0_val,ax
                mov ax,S_m3_val[bx][si]
                mov m1_val,ax
                mov al,S_AVEKM_2[si]
                mov AVEKM_0,al
                mov al,S_AVEKM_3[si]
                mov AVEKM_1,al
                mov ax,S_v2_val[bx][si]
                mov v0_val,ax
                mov ax,S_v3_val[bx][si]
                mov v1_val,ax
                mov al,S_KSLTL_2[si]
                mov KSLTL_0,al
                mov al,S_KSLTL_3[si]
                mov KSLTL_1,al
                mov al,S_AD_2[si]
                mov AD_0,al
                mov al,S_AD_3[si]
                mov AD_1,al
                mov al,S_SR_2[si]
                mov SR_0,al
                mov al,S_SR_3[si]
                mov SR_1,al
                mov fb_val,0
                mov ax,S_ws_val_2[bx][si]
                mov ws_val,ax
                mov al,S_FBC[si]
                shr al,1
                mov FBC,al
                mov al,S_scale_23[si]
                mov scale_flags,al

                mov al,S_update[si]
                mov update_save,al      ;save update flags for 2nd op pair
                ENDIF

__do_array:     test S_update[si],U_AVEKM
                jnz __go_AVEKM
__AVEKM_done:   test S_update[si],U_KSLTL
                jnz __go_KSLTL
__KSLTL_done:   test S_update[si],U_ADSR
                jnz __go_ADSR
__ADSR_done:    test S_update[si],U_WS
                jnz __go_WS
__WS_done:      test S_update[si],U_FBC
                jnz __go_FBC
__FBC_done:     test S_update[si],U_FREQ
                jnz __go_FREQ           ;(update FREQ's KeyOn bit last)
__FREQ_done:
                cmp array,0
                je __exit
                mov array,0

                mov al,update_save
                mov S_update[si],al

__array_0:      mov ah,0
                mov bh,0
                mov bl,S_voice[si]
                mov voice,bx            ;voice #
                mov al,op_0[bx]
                mov voice0,ax           ;"modulator" operator cell
                mov al,op_1[bx]
                mov voice1,ax           ;"carrier" operator cell

                mov bx,si
                mov ax,S_m0_val[bx][si]
                mov m0_val,ax
                mov ax,S_m1_val[bx][si]
                mov m1_val,ax
                mov al,S_AVEKM_0[si]
                mov AVEKM_0,al
                mov al,S_AVEKM_1[si]
                mov AVEKM_1,al
                mov ax,S_v0_val[bx][si]
                mov v0_val,ax
                mov ax,S_v1_val[bx][si]
                mov v1_val,ax
                mov al,S_KSLTL_0[si]
                mov KSLTL_0,al
                mov al,S_KSLTL_1[si]
                mov KSLTL_1,al
                mov al,S_AD_0[si]
                mov AD_0,al
                mov al,S_AD_1[si]
                mov AD_1,al
                mov al,S_SR_0[si]
                mov SR_0,al
                mov al,S_SR_1[si]
                mov SR_1,al
                mov ax,S_fb_val[bx][si]
                mov fb_val,ax
                mov ax,S_ws_val[bx][si]
                mov ws_val,ax
                mov al,S_FBC[si]
                mov FBC,al
                mov al,S_scale_01[si]
                mov scale_flags,al
                jmp __do_array

__exit:         ret

__go_AVEKM:     jmp __AVEKM
__go_KSLTL:     jmp __KSLTL
__go_ADSR:      jmp __ADSR
__go_WS:        jmp __WS
__go_FBC:       jmp __FBC
__go_FREQ:      jmp __FREQ

;----------------------------------------------------------------------------
__AVEKM:        mov di,WORD PTR S_channel[si]
                and di,0fh     

                mov al,MIDI_mod[di]
                mov di,01000000b        ;set bit 6 (Vibrato) if MIDI
                cmp al,64               ;Modulation controller >= 64
                jge __set_mod
                mov di,00000000b

__set_mod:      mov ax,m0_val           ;get multiplier 0 value
                mov cl,4
                shr ax,cl
                mov al,ah               ;divide by 4096
                or ax,di                ;merge with AM modulation bit
                or al,AVEKM_0           ;merge with EG/KSR/VIB bits
                call write_register C,voice0,20h,ax

                mov ax,m1_val           ;get multiplier 1 value
                mov cl,4
                shr ax,cl
                mov al,ah               ;divide by 4096
                or ax,di                ;merge with AM modulation bit
                or al,AVEKM_1           ;merge with EG/KSR/VIB bits
                call write_register C,voice1,20h,ax

                and S_update[si],NOT U_AVEKM
                jmp __AVEKM_done

;----------------------------------------------------------------------------
__KSLTL:        mov bx,v0_val           ;get modulator volume level
                shr bx,1
                shr bx,1
                mov bl,bh               ;divide by 1024 (BL=BH=0-63)
                
                IFDEF STEREO_3812
                test scale_flags,01b    ;additive operator?
                jz __mod_vol            ;no, don't scale level
                mov cl,127              ;else scale by composite volume factor
                mov al,bh
                mul lvol
                div cl
                mov bh,al
                mov al,bl
                mul rvol
                div cl
                mov bl,al
__mod_vol:      not bx                  ;BL=right vol, BH=left vol         
                and bx,3f3fh            ;convert volume to attenuation
                mov al,KSLTL_0          ;merge with KSL bits
                mov ah,al
                or bx,ax
                call stereo_register C,voice0,40h,bx
                ELSE
                test scale_flags,01b    ;additive operator?
                jz __mod_vol            ;no, don't scale level
                mov cl,127              ;else scale by composite volume factor
                mov al,bh
                mul vol
                div cl
                mov bl,al
__mod_vol:      not bl                  ;BL=monaural vol
                and bl,3fh              ;convert volume to attenuation
                or bl,KSLTL_0           ;merge with KSL bits
                call write_register C,voice0,40h,bx
                ENDIF

                mov bx,v1_val           ;get carrier volume level
                shr bx,1
                shr bx,1
                mov bl,bh               ;divide by 1024 (BL=BH=0-63)
                
                IFDEF STEREO_3812
                test scale_flags,10b    ;additive operator?
                jz __car_vol            ;no, don't scale level
                mov cl,127              ;else scale by composite volume factor
                mov al,bh
                mul lvol
                div cl
                mov bh,al
                mov al,bl
                mul rvol
                div cl
                mov bl,al
__car_vol:      not bx                  ;BL=right vol, BH=left vol         
                and bx,3f3fh            ;convert volume to attenuation
                mov al,KSLTL_1          ;merge with KSL bits
                mov ah,al
                or bx,ax
                call stereo_register C,voice1,40h,bx
                ELSE
                test scale_flags,10b    ;additive operator?
                jz __car_vol            ;no, don't scale level
                mov cl,127              ;else scale by composite volume factor
                mov al,bh
                mul vol
                div cl
                mov bl,al
__car_vol:      not bl                  ;BL=monaural vol
                and bl,3fh              ;convert volume to attenuation
                or bl,KSLTL_1           ;merge with KSL bits
                call write_register C,voice1,40h,bx
                ENDIF

                and S_update[si],NOT U_KSLTL
                jmp __KSLTL_done

;----------------------------------------------------------------------------
__ADSR:         mov al,AD_0
                call write_register C,voice0,60h,ax

                mov al,AD_1
                call write_register C,voice1,60h,ax

                mov al,SR_0
                call write_register C,voice0,80h,ax

                mov al,SR_1
                call write_register C,voice1,80h,ax

                and S_update[si],NOT U_ADSR
                jmp __ADSR_done

;----------------------------------------------------------------------------
__WS:           mov al,BYTE PTR ws_val
                call write_register C,voice1,0e0h,ax

                mov al,BYTE PTR ws_val+1
                call write_register C,voice0,0e0h,ax

                and S_update[si],NOT U_WS
                jmp __WS_done

;----------------------------------------------------------------------------
__FBC:          mov ax,fb_val           ;get feedback
                mov cl,4                ;divide by 4096
                shr ax,cl
                and ah,00001110b        ;merge bits 1-3 with C bit
                mov al,FBC
                and al,00000001b
                or al,ah

                IFDEF YMF262
                or al,00110000b         ;assume center channel
                mov di,WORD PTR S_channel[si]
                and di,0fh             
                mov bl,MIDI_pan[di]     ;get panpot for voice's MIDI channel
                cmp bl,R_PAN_THRESH  
                jbe __right
                cmp bl,L_PAN_THRESH
                jb __write_FBC
                and al,LEFT_MASK         ;>= left threshold, mute right channel
                jmp __write_FBC
__right:        and al,RIGHT_MASK        ;<= right threshold, mute left channel
                ENDIF

__write_FBC:    call send_byte C,voice,0c0h,ax

                and S_update[si],NOT U_FBC
                jmp __FBC_done

;----------------------------------------------------------------------------
__FREQ:         cmp array,1
                je __end_freq           ;(OPL3 freq not used in 2nd array)
                
                mov al,S_type[si]       ;if BNK instrument or TVFX instrument,
                cmp al,TV_EFFECT        ;note #/pitch bend determine freq...
                jne __MIDI_freq
                jmp __TV_effect         ;else TV freq/blk vals determine freq

__MIDI_freq:    test S_BLOCK[si],00100000b
                jnz __key_on            ;KON bit = 1; turn note on

                mov al,S_KBF_shadow[si] ;retrieve voice's last KBF value
                and al,11011111b        ;set KON = 0
                call send_byte C,voice,0b0h,ax  
                jmp __end_freq          ;turn the note off immediately

__key_on:       mov bl,S_channel[si]    ;get signed pitch bend value
                mov bh,0                
                mov al,MIDI_pitch_h[bx]
                mov ah,0
                mov cx,7
                shl ax,cl
                or al,MIDI_pitch_l[bx]  
                sub ax,2000h            

                mov cx,5                ;divide by 0x20, preserving sign
                sar ax,cl               ;(range now +0x100 to -0x100)

                mov cl,DEF_PITCH_RANGE  ;normally 12 (+0xc00 to -0xc00)
                mov ch,0
                imul cx                 

                mov bl,S_note[si]       ;get key # 12-108
                mov bh,0

                mov cx,ax               ;transpose it
                mov al,S_transpose[si]
                cbw
                add bx,ax
                mov ax,cx

                sub bx,24               ;normalize to 0-95

__norm_1:       add bx,12
                cmp bx,0
                jl __norm_1
                add bx,12
__norm_2:       sub bx,12
                cmp bx,95
                jg __norm_2

                add ah,bl               ;add computed note offset
                add ax,8                ;add 1/32 to round to 1/16

                mov cx,4                ;derive true note #
                sar ax,cl               ;(expressed in 1/16 halftones)

                sub ax,(12*16)
__norm_3:       add ax,(12*16)
                cmp ax,0
                jl __norm_3
                add ax,(12*16)
__norm_4:       sub ax,(12*16)
                cmp ax,(96*16)-1
                jg __norm_4

                mov di,ax
                mov cx,4
                shr di,cl               ;divide by 16 to get physical note #
                mov dx,di               ;DX = halftone/octave index   
                mov bl,note_halftone[di]
                mov bh,0
                mov di,bx               ;DI = halftone value within octave

                mov cx,5                ;derive table row
                shl di,cl
                shl ax,1
                and ax,11111b
                add di,ax               ;derive table index

                mov ax,freq_table[di]

                mov di,dx
                mov bl,note_octave[di]
                dec bl
                or ax,ax
                jge __bump_block
                inc bl
__bump_block:   or bl,bl
                jge __set_M_freq
                inc bl
                sar ax,1

__set_M_freq:   shl bl,1                ;merge F-NUM(H) with block #
                shl bl,1
                and ah,00000011b
                or ah,bl
                mov f_num,ax
                jmp __set_freq

__TV_effect:    mov bx,si               ;set frequency for TV sound effect
                mov ax,S_f_val[bx][si]
                mov cl,6
                shr ax,cl               ;F-NUMBER = TV freq val = 0-1023
                mov f_num,ax

__set_freq:     mov al,BYTE PTR f_num   ;set F-NUMBER(L)
                call send_byte C,voice,0a0h,ax  

                mov al,BYTE PTR f_num+1 ;get F-NUMBER(H)
                or al,S_BLOCK[si]       ;merge with KON/BLOCK bits

                mov S_KBF_shadow[si],al ;track value for quick Note Off later
                call send_byte C,voice,0b0h,ax 

__end_freq:     and S_update[si],NOT U_FREQ
                jmp __FREQ_done

                ENDP

;****************************************************************************
update_priority PROC                    ;Maintain synthesizer voice priority
                LOCAL slot_cnt,low_p,high_p,low_4_p
                LOCAL needful
                USES ds,si,di

                mov slot_cnt,0          ;zero active slot count

                mov si,-1              
__get_priority: inc si                  ;build adjusted priority table and
                cmp si,NUM_SLOTS        ;reallocate voices if necessary
                je __sort_p_list
                cmp S_status[si],FREE
                je __get_priority

                inc slot_cnt            ;slot active, bump count

                mov bx,si
                mov di,WORD PTR S_channel[si]
                and di,0fh              ;DI = slot's MIDI channel
                mov ax,0ffffh           
                cmp MIDI_vprot[di],64   ;priority = max if voice protection on
                jge __adj_priority       
                mov ax,S_p_val[bx][si]  
__adj_priority: mov cl,MIDI_voices[di]  ;AX = slot's base priority        
                mov ch,0
                sub ax,cx               ;priority -= # of voices in channel
                jnc __set_priority
                mov ax,0
__set_priority: mov S_V_priority[bx][si],ax
                jmp __get_priority

__sort_p_list:  mov ax,0                ;set AX = unvoiced highest priority                
                mov dx,-1               ;set DX = voiced lowest priority
                mov cx,-1               ;set CX = voiced 4-op lowest priority
                mov si,-1               
__for_slot:     inc si                  
                cmp si,NUM_SLOTS
                je __reassign
                cmp S_status[si],FREE
                je __for_slot
                mov bx,si
                mov di,S_V_priority[bx][si]
                mov bl,S_voice[si]
                cmp bl,-1      
                jne __chk_low_4
                cmp di,ax               
                jb __for_slot         
                mov ax,di              
                mov high_p,si           ;highest-priority unvoiced slot
                jmp __for_slot
__chk_low_4:    cmp op4_base[bx],0
                je __chk_low
                cmp di,cx
                ja __chk_low
                mov cx,di
                mov low_4_p,si          ;lowest-priority voiced 4-op slot
__chk_low:      cmp di,dx               
                ja __for_slot
                mov dx,di               
                mov low_p,si            ;lowest-priority voiced slot
                jmp __for_slot

__reassign:     cmp ax,dx               ;highest unvoiced < lowest voiced?
                jb __exit               ;yes, we're done

                cmp ax,0                ;if highest unvoiced priority = 0 (or 
                je __exit               ;none), bail out

                mov si,low_p            ;else steal voice from compatible slot

                IFDEF YMF262
                mov bx,high_p           ;is new voice a 4-op voice?
                cmp S_type[bx],OPL3_INST
                jne __rob_voice
                mov si,low_4_p          ;yes, must rob from 4-op slot
                cmp S_type[si],OPL3_INST
                je __rob_voice          ;4-op seizes 4-op, continue

                mov al,alt_voice[si]    ;4-op seizes 2 2-op voices...
                mov di,-1               
__kill_alt:     inc di                  ;find slot which owns 2nd half's
                cmp di,NUM_SLOTS        ;2-op voice (if any) and release it
                je __rob_voice
                cmp S_status[di],FREE
                je __kill_alt
                cmp S_voice[di],al
                jne __kill_alt

                call release_voice C,di
                ENDIF

__rob_voice:    mov bh,0
                mov bl,S_voice[si]    

                push bx
                call release_voice C,si
                pop bx

                mov si,high_p
                mov S_voice[si],bl
                mov di,bx
                mov bl,S_channel[si]
                inc MIDI_voices[bx]
                mov V_channel[di],bl
                cmp S_type[si],OPL3_INST
                jne __do_update
                mov V_channel[di+3],bl

__do_update:    mov S_update[si],U_ALL_REGS
                call update_voice C,si  ;update the hardware

                dec slot_cnt
                jnz __sort_p_list       ;keep sorting until priorities valid

__exit:         ret
                ENDP

;****************************************************************************
                IFDEF YMF262
OPL_phase       PROC Slot                   ;Set up 4-op slot parameters
                USES ds,si,di

                call BNK_phase C,[Slot]

                mov si,[Slot]
                mov bx,si
                shl bx,1
                mov di,S_timbre_off[bx]
                mov ds,S_timbre_seg[bx]

                mov S_type[si],OPL3_INST

                mov al,[di].B_fb_c
                and ax,10000000b
                mov cl,6
                shr ax,cl
                or S_FBC[si],al             ;move 2nd connection bit to S_FBC

                push bx                     ;set scaling flags for additive
                mov bl,S_FBC[si]            ;operators, based on voice's
                mov bh,0                    ;algorithm (0-3)
                mov al,carrier_01[bx]       
                mov S_scale_01[si],al
                mov al,carrier_23[bx]
                mov S_scale_23[si],al
                pop bx

                mov al,0                    ;copy key scale/total level values
                mov ah,[di].O_mod_KSLTL
                mov dl,ah
                and dl,11000000b
                mov S_KSLTL_2[si],dl
                not ah
                and ah,00111111b
                shl ax,1
                shl ax,1
                mov S_v2_val[bx],ax

                mov al,0                    
                mov ah,[di].O_car_KSLTL
                mov dl,ah
                and dl,11000000b
                mov S_KSLTL_3[si],dl
                not ah
                and ah,00111111b
                shl ax,1
                shl ax,1
                mov S_v3_val[bx],ax

                mov al,[di].O_mod_AVEKM     ;copy AM/Vib/EG/KSR/Multi bits
                mov ah,al
                and al,11110000b            
                mov S_AVEKM_2[si],al
                mov al,0
                mov cl,4
                shl ax,cl
                mov S_m2_val[bx],ax

                mov al,[di].O_car_AVEKM     
                mov ah,al
                and al,11110000b            
                mov S_AVEKM_3[si],al
                mov al,0
                mov cl,4
                shl ax,cl
                mov S_m3_val[bx],ax

                mov al,[di].O_mod_AD        ;copy envelope parms
                mov S_AD_2[si],al           
                mov al,[di].O_mod_SR
                mov S_SR_2[si],al

                mov al,[di].O_car_AD
                mov S_AD_3[si],al
                mov al,[di].O_car_SR
                mov S_SR_3[si],al

                mov al,[di].O_car_WS        ;copy wave select values
                mov ah,[di].O_mod_WS
                mov S_ws_val_2[bx],ax

                ret
                ENDP
                ENDIF

;****************************************************************************
BNK_phase       PROC Slot                   ;Set up BNK slot parameters
                USES ds,si,di

                mov si,[Slot]
                mov bx,si              
                shl bx,1
                mov di,S_timbre_off[bx]
                mov ds,S_timbre_seg[bx]
                
                mov S_BLOCK[si],00100000b   ;set KON, clear BLOCK mask

                mov S_type[si],BNK_INST
                mov S_duration[bx],-1

                mov S_p_val[bx],32767       ;BNK instrument priority = average

                mov al,0
                mov ah,[di].B_fb_c          ;copy feedback/connection values
                mov dl,ah
                and dl,00000001b
                mov S_FBC[si],dl
                mov cl,4
                shl ax,cl
                mov S_fb_val[bx],ax

                mov al,0                    ;copy key scale/total level values
                mov ah,[di].B_mod_KSLTL
                mov dl,ah
                and dl,11000000b
                mov S_KSLTL_0[si],dl
                not ah
                and ah,00111111b
                shl ax,1
                shl ax,1
                mov S_v0_val[bx],ax

                mov al,0                    
                mov ah,[di].B_car_KSLTL
                mov dl,ah
                and dl,11000000b
                mov S_KSLTL_1[si],dl
                not ah
                and ah,00111111b
                shl ax,1
                shl ax,1
                mov S_v1_val[bx],ax

                mov al,[di].B_mod_AVEKM     ;copy AM/Vib/EG/KSR/Multi bits
                mov ah,al
                and al,11110000b            
                mov S_AVEKM_0[si],al
                mov al,0
                mov cl,4
                shl ax,cl
                mov S_m0_val[bx],ax

                mov al,[di].B_car_AVEKM     
                mov ah,al
                and al,11110000b            
                mov S_AVEKM_1[si],al
                mov al,0
                mov cl,4
                shl ax,cl
                mov S_m1_val[bx],ax

                mov al,[di].B_mod_AD        ;copy envelope parms
                mov S_AD_0[si],al           
                mov al,[di].B_mod_SR
                mov S_SR_0[si],al

                mov al,[di].B_car_AD
                mov S_AD_1[si],al
                mov al,[di].B_car_SR
                mov S_SR_1[si],al

                mov al,[di].B_car_WS        ;copy wave select values
                mov ah,[di].B_mod_WS
                mov S_ws_val[bx],ax

                mov al,S_FBC[si]            ;get C-bit (1=additive, 0=FM)
                or al,10b                   ;(2-op carrier always scaled)
                mov S_scale_01[si],al

                mov S_update[si],U_ALL_REGS ;flag all registers "dirty"

                ret
                ENDP

;****************************************************************************
note_off        PROC Chan:BYTE,Note:BYTE
                USES ds,si,di           ;Turn MIDI note off

                mov si,-1               ;find all slots in which note is
__next_slot:    mov al,[Note]           ;playing
                mov bl,[Chan]
__find_note:    inc si
                cmp si,NUM_SLOTS
                je __exit
                cmp S_status[si],KEYON  
                jne __find_note
                cmp S_keynum[si],al
                jne __find_note
                cmp S_channel[si],bl
                jne __find_note

                mov bh,0
                cmp MIDI_sus[bx],64
                jge __sustained
                
                cmp S_type[si],OPL3_INST
                je __release_it
                cmp S_type[si],BNK_INST
                jne __TV_note_off
                
__release_it:   call release_voice C,si ;release the slot's voice
                mov S_status[si],FREE

                IFDEF TV_switch_voice   ;if TVFX installed...
                call TV_switch_voice    ;see if a TV slot needs voice
                ENDIF
                jmp __next_slot

__TV_note_off:  mov bx,si
                mov S_duration[bx][si],1    
                jmp __next_slot         ;set duration to last cycle

__sustained:    mov S_sustain[si],1
                jmp __next_slot

__exit:         ret

                ENDP

;****************************************************************************
note_on         PROC Chan,Note,Velocity ;Turn MIDI note on
                USES ds,si,di

                mov di,[Chan]     

                mov bh,0
                mov bl,MIDI_timbre[di]  ;get timbre used in channel

                cmp di,9                ;channel under RBS control?
                jne __set_timbre        ;no, BX=timbre cache index

                mov si,[Note]           ;else see if RBS entry is valid for
                mov bl,RBS_timbres[si]  ;this note
                cmp bl,-1
                jne __set_timbre        ;it's valid -- use it

                mov ax,[Note]           ;it's not -- validate RBS entry first
                mov ah,127              ;bank 127 reserved for RBS timbres
                call index_timbre C,ax
                mov bx,ax
                mov RBS_timbres[si],bl

__set_timbre:   cmp bl,-1
                je __exit               ;timbre not loaded, exit
                
                shl bx,1
                lds di,cache_base
                add di,timb_offsets[bx] ;else get address of timbre

                add note_event_l,1      ;update timbre cache LRU counters
                adc note_event_h,0      
                mov ax,note_event_l
                mov dx,note_event_h
                mov timb_hist_l[bx],ax
                mov timb_hist_h[bx],dx
                    
                mov si,0                
__find_slot:    cmp S_status[si],FREE
                je __slot_found
                inc si                  ;find a free virtual voice slot
                cmp si,NUM_SLOTS
                jne __find_slot
                jmp __exit              ;exit if no virtual voice available

__slot_found:   mov ax,[Chan]           ;establish MIDI channel
                mov S_channel[si],al    

                mov dx,[Note]
                mov S_keynum[si],dl     ;save MIDI key #

                mov al,0                ;establish MIDI note/transposition
                mov cl,[di+2]
                mov bx,[Chan]
                cmp bx,9                ;(for all channels except 10)
                je __set_n_txp
                mov al,cl
                mov cl,dl
__set_n_txp:    mov S_note[si],cl
                mov S_transpose[si],al

                IF VEL_SENS             
                mov ax,[Velocity]       ;establish note velocity
                IF NOT VEL_TRUE              
                shr al,1
                shr al,1
                shr al,1
                lea bx,vel_graph        ;scale back velocity sensitivity to 
                xlat cs:[bx]            ;reduce perceived playback noise
                ENDIF
                ELSE
                mov al,127              ;default velocity = 127
                ENDIF
                mov S_velocity[si],al

                mov bx,si               ;copy timbre address
                shl bx,1
                mov S_timbre_off[bx],di
                mov S_timbre_seg[bx],ds

                mov S_status[si],KEYON  ;flag note "on" in slot

                mov S_sustain[si],0     ;init sustained flag

                mov ax,[di]             ;derive timbre type based on size
                cmp ax,SIZE OPL3BNK
                je __OPL3BNK
                cmp ax,SIZE BNK
                jne __TVFX_timbre

__BNK:          call BNK_phase C,si     ;set up BNK timbre in slot
                jmp __get_voice

__OPL3BNK:      IFDEF YMF262
                call OPL_phase C,si
                ENDIF
                jmp __get_voice

__TVFX_timbre:  IFDEF TV_phase          ;if TVFX enabled...
                call TV_phase C,si      ;set up TVFX timbre in slot
                ELSE
                jmp __exit
                ENDIF

__get_voice:    mov S_voice[si],-1
                call assign_voice C,si  ;assign hardware voice to slot

__exit:         ret
                ENDP

;****************************************************************************
release_sustain PROC Chan:BYTE
                USES ds,si,di

                mov si,0                
__release_sus:  cmp S_status[si],FREE   
                je __next_sus
                mov al,[Chan]
                cmp S_channel[si],al
                jne __next_sus
                cmp S_sustain[si],0
                je __next_sus
                call note_off C,di,WORD PTR S_note[si]
__next_sus:     inc si
                cmp si,NUM_SLOTS
                jne __release_sus

                ret
                ENDP

;****************************************************************************
send_MIDI_message PROC Stat:BYTE,D1:BYTE,D2:BYTE       
                USES ds,si,di           ;Send MIDI Channel Voice message

                mov si,WORD PTR [D1]
                and si,0ffh             ;SI=data 1 / controller #
                mov di,WORD PTR [Stat]
                mov ax,di               
                and di,00fh             ;DI=channel
                and ax,0f0h             ;AX=status
                mov ch,0                ;CX=data byte 2
                mov cl,[D2]             

                cmp ax,0b0h             
                je __ctrl_change
                cmp ax,0c0h
                je __program
                cmp ax,0e0h
                je __pitch
                cmp ax,080h
                je __note_off
                cmp ax,090h
                jne __exit

                cmp di,MIN_TRUE_CHAN-1
                jb __exit
                cmp di,MAX_REC_CHAN-1
                ja __exit

                jcxz __note_off         ;implicit Note Off if velocity==0

                call note_on C,di,si,cx
                ret

__note_off:     call note_off C,di,si
__exit:         ret

__pitch:        mov ax,si
                mov MIDI_pitch_l[di],al
                mov MIDI_pitch_h[di],cl
                mov al,U_FREQ
                jmp __flag_updates

__ctrl_change:  cmp si,PATCH_BANK_SEL
                je __t_bank
                cmp si,VOICE_PROTECT
                je __vprot
                cmp si,TIMBRE_PROTECT
                jne __MIDI

                mov bl,MIDI_timbre[di]
                cmp bl,-1
                je __exit
                mov bh,0
                mov al,timb_attribs[bx]
                and al,10111111b
                cmp cl,64
                jl __tprot
                or al,01000000b
__tprot:        mov timb_attribs[bx],al
                jmp __exit

__program:      mov ax,si
                mov MIDI_program[di],al
                mov ah,MIDI_bank[di]
                call index_timbre C,ax
                mov MIDI_timbre[di],al  ;record timbre # used by channel
                jmp __exit              ;(-1 if timbre not in local cache)

__t_bank:       mov MIDI_bank[di],cl
                jmp __exit

__vprot:        mov MIDI_vprot[di],cl
                jmp __exit

__MIDI:         mov al,U_AVEKM          ;Emulate MIDI controllers
                lea bx,MIDI_mod         
                cmp si,MODULATION
                je __MIDI_set

                mov al,U_KSLTL
                lea bx,MIDI_vol
                cmp si,PART_VOLUME
                je __MIDI_set
                lea bx,MIDI_express
                cmp si,EXPRESSION
                je __MIDI_set

                IFDEF YMF262            ;FBC registers control stereo on OPL3
                mov al,U_FBC
                ENDIF

                lea bx,MIDI_pan
                cmp si,PANPOT
                je __MIDI_set

                cmp si,SUSTAIN
                je __MIDI_sus
                cmp si,RESET_ALL_CTRLS
                je __MIDI_rac
                cmp si,ALL_NOTES_OFF
                je __MIDI_ano
                jmp __exit             

__MIDI_set:     mov cs:[bx][di],cl      ;save shadowed controller value
                
__flag_updates: mov bx,di
                mov si,0                ;mark appropriate voice parameters
__flag_slot:    cmp S_status[si],FREE   ;as "changed"
                je __flag_next
                cmp S_channel[si],bl
                jne __flag_next
                or S_update[si],al
                push ax
                push bx
                call update_voice C,si  ;update the hardware registers
                pop bx
                pop ax
__flag_next:    inc si
                cmp si,NUM_SLOTS
                jne __flag_slot
                jmp __exit

__MIDI_sus:     mov MIDI_sus[di],cl     ;log sustain value
                cmp cl,64               ;releasing sustain controller?
                jge __exit
                                          
                call release_sustain C,di 
                jmp __exit              ;yes, turn off any sustained notes 

__MIDI_ano:     mov si,0                ;turn off all notes playing in channel
__chk_note:     cmp S_status[si],KEYON
                jne __next_ano
                mov bx,di
                cmp S_channel[si],bl
                jne __next_ano
                call note_off C,di,WORD PTR S_note[si]
__next_ano:     inc si
                cmp si,NUM_SLOTS
                jne __chk_note
                jmp __exit

__MIDI_rac:     mov MIDI_sus[di],0
                call release_sustain C,di
                mov MIDI_mod[di],0      ;emulate Roland LAPC-1 RAC message
                mov MIDI_express[di],127
                mov MIDI_pitch_l[di],DEF_PITCH_L
                mov MIDI_pitch_h[di],DEF_PITCH_H
                mov al,U_AVEKM OR U_KSLTL OR U_FREQ
                jmp __flag_updates

                ENDP

;****************************************************************************
;*                                                                          *
;*  Miscellaneous public (API-accessible) procedures                        *
;*                                                                          *
;****************************************************************************

describe_driver PROC H,IntRateProc:FAR PTR    
                USES ds,si,di           ;Return far ptr to DDT
                pushf
                cli

                mov dx,cs
                mov device_name_s,dx
                lea ax,DDT

                POP_F
                ret
                ENDP

;****************************************************************************
send_cv_msg     PROC H,Stat,D1,D2       ;Send an explicit Channel Voice msg
                USES ds,si,di
                pushf
                cli

                call send_MIDI_message C,[Stat],[D1],[D2]

                POP_F
                ret
                ENDP

;****************************************************************************
send_sysex_msg  PROC H,AddrA:BYTE,AddrB:BYTE,AddrC:BYTE,Data:FAR PTR,Size,Wait

                ret
                ENDP

;****************************************************************************
write_display   PROC H,String:FAR PTR   ;Write string to display (unless NULL)

                ret
                ENDP


