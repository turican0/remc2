;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
;лл                                                                        лл
;лл   AIL.INC                                                              лл
;лл                                                                        лл
;лл   IBM Audio Interface Library assembly equate definitions              лл
;лл                                                                        лл
;лл   Version 1.00 of 24-Sep-91: Initial V2.X version                      лл
;лл           1.01 of 09-Apr-92: AIL_format_VOC_file()/AIL_format_buffer() лл
;лл                              calls added to support Ad Lib Gold        лл
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

SEQ_STOPPED     equ 0 
SEQ_PLAYING     equ 1
SEQ_DONE        equ 2

DAC_STOPPED     equ 0 
DAC_PAUSED      equ 1  
DAC_PLAYING     equ 2
DAC_DONE        equ 3

                        ;
                        ;MIDI and XMIDI controllers recognized by AIL drivers
                        ;

START_MSB_1     equ 32                  ;System Exclusive XMIDI controllers
START_KSB_1     equ 33                  ;(Roland DT1 format)
START_LSB_1     equ 34
DATA_BYTE_1     equ 35
FINAL_BYTE_1    equ 36
START_MSB_2     equ 37       
START_KSB_2     equ 38
START_LSB_2     equ 39
DATA_BYTE_2     equ 40
FINAL_BYTE_2    equ 41
START_MSB_3     equ 42       
START_KSB_3     equ 43
START_LSB_3     equ 44
DATA_BYTE_3     equ 45
FINAL_BYTE_3    equ 46

RHYTHM_KEY_TIMB equ 58                  ;Roland-exclusive XMIDI controllers
PATCH_REVERB    equ 59                  
PATCH_BENDER    equ 60
REVERB_MODE     equ 61
REVERB_TIME     equ 62
REVERB_LEVEL    equ 63

CHAN_LOCK       equ 110                 ;General XMIDI controllers
CHAN_PROTECT    equ 111
VOICE_PROTECT   equ 112
TIMBRE_PROTECT  equ 113
PATCH_BANK_SEL  equ 114
INDIRECT_C_PFX  equ 115
FOR_LOOP        equ 116
NEXT_LOOP       equ 117
CLEAR_BEAT_BAR  equ 118
CALLBACK_TRIG   equ 119
SEQ_INDEX       equ 120

MODULATION      equ 1                   ;MIDI controllers and Channel Mode
PART_VOLUME     equ 7                   ;messages recognized by AIL drivers
PANPOT          equ 10
EXPRESSION      equ 11
SUSTAIN         equ 64
RESET_ALL_CTRLS equ 121
ALL_NOTES_OFF   equ 123

                        ;
                        ;Dynamically linked driver procedure numbers
                        ;

AIL_DESC_DRVR           equ 100
AIL_DET_DEV             equ 101
AIL_INIT_DRVR           equ 102
AIL_SERVE_DRVR          equ 103
AIL_SHUTDOWN_DRVR       equ 104

AIL_INDEX_VOC_BLK       equ 120
AIL_REG_SND_BUFF        equ 121
AIL_SND_BUFF_STAT       equ 122
AIL_P_VOC_FILE          equ 123
AIL_VOC_PB_STAT         equ 124
AIL_START_D_PB          equ 125
AIL_STOP_D_PB           equ 126
AIL_PAUSE_D_PB          equ 127
AIL_RESUME_D_PB         equ 128
AIL_SET_D_PB_VOL        equ 129
AIL_D_PB_VOL            equ 130
AIL_SET_D_PB_PAN        equ 131
AIL_D_PB_PAN            equ 132
AIL_F_VOC_FILE          equ 133
AIL_F_SND_BUFF          equ 134
            
AIL_STATE_TAB_SIZE      equ 150
AIL_REG_SEQ             equ 151
AIL_REL_SEQ_HND         equ 152
AIL_T_CACHE_SIZE        equ 153
AIL_DEFINE_T_CACHE      equ 154
AIL_T_REQ               equ 155
AIL_INSTALL_T           equ 156
AIL_PROTECT_T           equ 157
AIL_UNPROTECT_T         equ 158
AIL_T_STATUS            equ 159

AIL_START_SEQ           equ 170
AIL_STOP_SEQ            equ 171
AIL_RESUME_SEQ          equ 173
AIL_SEQ_STAT            equ 174
AIL_REL_VOL             equ 175
AIL_REL_TEMPO           equ 176
AIL_SET_REL_VOL         equ 177
AIL_SET_REL_TEMPO       equ 178
AIL_BEAT_CNT            equ 179
AIL_BAR_CNT             equ 180
AIL_BRA_INDEX           equ 181

AIL_CON_VAL             equ 182
AIL_SET_CON_VAL         equ 183
AIL_CHAN_NOTES          equ 185
AIL_SEND_CV_MSG         equ 186
AIL_SEND_SYSEX_MSG      equ 187
AIL_WRITE_DISP          equ 188
AIL_INSTALL_CB          equ 189
AIL_CANCEL_CB           equ 190

AIL_LOCK_CHAN           equ 191
AIL_MAP_SEQ_CHAN        equ 192
AIL_RELEASE_CHAN        equ 193
AIL_TRUE_SEQ_CHAN       equ 194
