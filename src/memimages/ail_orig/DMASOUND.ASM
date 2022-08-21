;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
;лл                                                                        лл
;лл   DMASOUND.ASM                                                         лл
;лл                                                                        лл
;лл   IBM Audio Interface Library                                          лл
;лл                                                                        лл
;лл   Digital sound driver/emulator for Sound Blaster-type audio devices   лл
;лл                                                                        лл
;лл   Version 2.00 of 09-Oct-91: Initial V2.0 version, derived from V1.05  лл
;лл           2.01 of 21-Nov-91: Autodetects Thunderboard as Sound Blaster лл
;лл                              Dual-buffer playback flag checks modified лл
;лл           2.02 of 14-Dec-91: PAS DMA timer pauses/resumes correctly    лл
;лл           2.03 of 18-Dec-91: Standard SB speaker control fixed         лл
;лл           2.04 of 13-Jan-92: Stereo .VOC file support added            лл
;лл           2.05 of 20-Jan-92: I/O timeout w/beep added                  лл
;лл                              Sample rate high byte always cleared      лл
;лл           2.06 of 18-Feb-92: IRQ delay during halt_DMA                 лл
;лл                              sysex_wait() calls replace loops          лл
;лл                              Send >1 bytes during detection            лл
;лл                              Do shutdown only if initialized           лл
;лл           2.07 of  2-Apr-92: Set valid sample rate during detection    лл
;лл           2.10 of  4-Apr-92: Ad Lib Gold support added                 лл
;лл                              AIL_format_VOC_file/_sound_buffer() added лл
;лл                              DMA word count checked in IRQ handlers    лл
;лл           2.11 of 13-May-92: IRQ delay skipped if .VOC playback ended  лл
;лл           2.12 of 29-May-92: PAS default volume reduced                лл
;лл           2.13 of 29-Jun-92: PAS +/16 compatibility issues resolved    лл
;лл                              ALG panning disabled in mono mode         лл
;лл                              SBLASTER silence-packing problems fixed   лл
;лл           2.14 of 17-Aug-92: CHECK_DMAC option added                   лл
;лл           2.15 of 15-Sep-92: WAIT_FALSE_IRQ option added               лл
;лл           2.16 of 14-Nov-92: Alternative fix for unwanted SBlaster     лл
;лл                              IRQs implemented                          лл
;лл                              DMA terminal count of 0 accepted          лл
;лл                                                                        лл
;лл   Author: John Miles                                                   лл
;лл   8086 ASM source compatible with Turbo Assembler v2.0 or later        лл
;лл                                                                        лл
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
;лл                                                                        лл
;лл    Copyright (C) 1991, 1992 Miles Design, Inc.                         лл
;лл                                                                        лл
;лл    Miles Design, Inc.                                                  лл
;лл    10926 Jollyville #308                                               лл
;лл    Austin, TX 78759                                                    лл
;лл    (512) 345-2642 / FAX (512) 338-9630 / BBS (512) 454-9990            лл
;лл                                                                        лл
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

                MODEL MEDIUM,C          ;Procedures far, data near by default
                LOCALS __               ;Enable local labels with __ prefix
                JUMPS                   ;Enable auto jump sizing

                ;
                ;External/configuration equates
                ;

FALSE           equ 0
TRUE            equ -1

PAS_FILTER      equ FALSE               ;TRUE to enable PAS PCM antialiasing
                                        ;(also degrades FM treble response)

CHECK_DMAC      equ TRUE                ;FALSE to inhibit checking for end-
                                        ;of-DMA conditions; may need to be
                                        ;turned off for proper operation with 
                                        ;non-100% IBM-compatible systems and 
                                        ;V86 host software

WAIT_FALSE_IRQ  equ FALSE               ;TRUE to wait 140 ms. after stopping
                                        ;DMA transfers -- original way to     
                                        ;avoid certain Sound Blaster hardware
                                        ;glitches

DAC_STOPPED     equ 0
DAC_PAUSED      equ 1
DAC_PLAYING     equ 2
DAC_DONE        equ 3

                ;
                ;Macros, internal equates
                ;

                INCLUDE ail.mac
                INCLUDE ail.inc

                IFDEF PAS
STEREO          EQU 1
                ENDIF

                IFDEF SBSTD
SBLASTER        EQU 1
                ENDIF

                IFDEF SBPRO
SBLASTER        EQU 1
STEREO          EQU 1
                ENDIF

                IFDEF ADLIBG
STEREO          EQU 1
NEEDS_FORMAT    EQU 1
                ENDIF

                IFDEF PAS
BI_OUTPUTMIXER  equ 00h                 ;PAS equates
BI_L_PCM        equ 06h
BI_R_PCM        equ 0dh
INTRCTLRST      equ 0b89h
AUDIOFILT       equ 0b8ah
INTRCTLR        equ 0b8bh
PCMDATA 	equ 0f88h	      
CROSSCHANNEL    equ 0f8ah
TMRCTLR 	equ 138bh
SAMPLERATE	equ 1388h
SAMPLECNT       equ 1389h
MVState         struc
_sysspkrtmr     db      0               ;   42 System Speaker Timer Address
_systmrctlr	db	0		;   43 System Timer Control Register
_sysspkrreg	db	0		;   61 System Speaker Register
_joystick	db	0		;  201 Joystick Register
_lfmaddr	db	0		;  388 Left  FM Synthesizer Address Register
_lfmdata	db	0		;  389 Left  FM Synthesizer Data Register
_rfmaddr	db	0		;  38A Right FM Synthesizer Address Register
_rfmdata	db	0		;  38B Right FM Synthesizer Data Register
_dfmaddr	db	0		;  788 Dual  FM Synthesizer Address Register
_dfmdata	db	0		;  789 Dual  FM Synthesizer Data Register
                db	0		;      reserved for future use
                db      0               ;      reserved for future use
_audiomixr	db	0		;  B88 Audio Mixer Control Register
_intrctlrst	db	0		;  B89 Interrupt Status Register write
_audiofilt	db	0		;  B8A Audio Filter Control Register
_intrctlr	db	0		;  B8B Interrupt Control Register write
_pcmdata	db	0		;  F88 PCM data I/O register
	db	0		;  F89 reserved for future use
_crosschannel	db	0		;  F8A Cross Channel
	db	0		;  F8B reserved for future use
_samplerate     dw      0               ; 1388 Sample Rate Timer Register
_samplecnt	dw	0		; 1389 Sample Count Register
_spkrtmr	dw	0		; 138A Local Speaker Timer Address
_tmrctlr	db	0		; 138B Local Timer Control Register
_mdirqvect	db	0		; 1788 MIDI IRQ Vector Register
_mdsysctlr	db	0		; 1789 MIDI System Control Register
_mdsysstat	db	0		; 178A MIDI IRQ Status Register
_mdirqclr	db	0		; 178B MIDI IRQ Clear Register
_mdgroup1	db	0		; 1B88 MIDI Group #1 Register
_mdgroup2	db	0		; 1B89 MIDI Group #2 Register
_mdgroup3	db	0		; 1B8A MIDI Group #3 Register
_mdgroup4	db	0		; 1B8B MIDI Group #4 Register
MVState         ends
                ENDIF

VOC_MODE        equ 0                   ;Creative Voice File playback mode
BUF_MODE        equ 1                   ;Dual-buffer DMA playback mode

                .CODE

                dw OFFSET driver_index
                db 'Copyright (C) 1991,1992 Miles Design, Inc.',01ah

driver_index:
                dw AIL_DESC_DRVR,OFFSET describe_driver 
                dw AIL_DET_DEV,OFFSET detect_device   
                dw AIL_INIT_DRVR,OFFSET init_driver     
                dw AIL_SHUTDOWN_DRVR,OFFSET shutdown_driver 
                dw AIL_P_VOC_FILE,OFFSET play_VOC_file       
                dw AIL_START_D_PB,OFFSET start_d_pb      
                dw AIL_STOP_D_PB,OFFSET stop_d_pb       
                dw AIL_PAUSE_D_PB,OFFSET pause_d_pb      
                dw AIL_RESUME_D_PB,OFFSET cont_d_pb       
                dw AIL_VOC_PB_STAT,OFFSET get_VOC_status
                dw AIL_SET_D_PB_VOL,OFFSET set_d_pb_vol    
                dw AIL_D_PB_VOL,OFFSET get_d_pb_vol
                dw AIL_SET_D_PB_PAN,OFFSET set_d_pb_pan
                dw AIL_D_PB_PAN,OFFSET get_d_pb_pan
                dw AIL_INDEX_VOC_BLK,OFFSET index_VOC_blk
                dw AIL_REG_SND_BUFF,OFFSET register_sb
                dw AIL_SND_BUFF_STAT,OFFSET get_sb_status
                IFDEF NEEDS_FORMAT
                dw AIL_F_VOC_FILE,OFFSET format_VOC_file       
                dw AIL_F_SND_BUFF,OFFSET format_sb
                ENDIF
                dw -1

                ;
                ;Driver Description Table (DDT)
                ;Returned by describe_driver() proc
                ;

DDT             LABEL WORD
min_API_version dw 200                  ;Minimum API version required = 2.00
driver_type     dw 2                    ;Type 2: Sound Blaster DSP emulation
data_suffix     db 'VOC',0              ;Supports .VOC files directly
device_name_o   dw OFFSET devnames      ;Pointer to list of supported devices
device_name_s   dw ?
default_IO      LABEL WORD              ;Factory default I/O parameters
                IFDEF PAS       
                dw -1                   ;(determined from MVSOUND.SYS)
                ELSEIFDEF ADLIBG
                dw 388h
                ELSE
                dw 220h                 
                ENDIF
default_IRQ     LABEL WORD
                IFDEF SBSTD
                dw 7
                ELSEIFDEF SBPRO
                dw 7                    ;(pre-prod. = 5; prod. = 7)
                ELSEIFDEF PAS
                dw -1                   ;(determined from MVSOUND.SYS)
                ELSEIFDEF ADLIBG
                dw -1                   ;(determined from control regs)
                ENDIF
default_DMA     LABEL WORD
                IFDEF PAS
                dw -1                   ;(determined from MVSOUND.SYS)
                ELSEIFDEF ADLIBG
                dw -1                   ;(determined from control regs)
                ELSE
                dw 1
                ENDIF
default_DRQ     dw -1
service_rate    dw -1                   ;No periodic service required
display_size    dw 0                    ;No display

devnames        LABEL BYTE
                IFDEF SBSTD
                db 'Creative Labs Sound Blaster(TM) Digital Sound',0
                db 'Media Vision Thunderboard(TM) Digital Sound',0
                ELSEIFDEF SBPRO
                db 'Creative Labs Sound Blaster Pro(TM) Digital Sound',0
                ELSEIFDEF PAS
                db 'Media Vision Pro Audio Spectrum(TM) Digital Sound',0
                ELSEIFDEF ADLIBG
                db 'Ad Lib(R) Gold Music Synthesizer Card',0
                ENDIF
                db 0                    ;0 to end list of device names

                ;
                ;Default setup values & internal constants
                ;

default_vol     LABEL WORD
                IFDEF SBPRO
                dw 110
                ELSEIFDEF PAS
                dw 80
                ELSE
                dw 127
                ENDIF

default_pan     dw 64

                IFDEF SBLASTER
pack_opcodes    db 14h,75h,77h,17h      ;type 0-3, init xfer
                db 14h,74h,76h,16h      ;type 0-3, cont xfer
                ENDIF

DMAPAG_offset   db 07h,03h,01h,02h,-1,0bh,09h,0ah

                IFDEF PAS
                IF PAS_FILTER
filter_cutoff   dw 17897,15909,11931,8948,5965,2982
filter_value    db 00001b,00010b,01001b,10001b,11001b,00100b
                ENDIF
                ENDIF

                IFDEF ADLIBG
selected_IRQ    db 3,4,5,7,10,11,12,15

PCM_Hz          dw 44100,22050,11025,7350
ADPCM_Hz        dw 22050,11025,7350,5513

freq_bits       db 00000000b,00001000b,00010000b,00011000b

pack_modes      db 0,1,128,129,4,132

;                  m8 PCM    m4 ADPCM  s8 PCM    s4 ADPCM  m16 PCM   s16 PCM
PRC_0_values    db 01100110b,01100010b,01000110b,00100010b,01100110b,01000110b
PRC_1_values    db 00000000b,00000000b,00100110b,01000010b,00000000b,00100110b

SFC_0_values    db 00000101b,00000101b,10000101b,10000101b,01000101b,11000101b
SFC_1_values    db 00000010b,00000010b,00000011b,00000011b,00000010b,01000011b
                ENDIF

                IFDEF STEREO
pan_graph       db 0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30                      
                db 32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62                
                db 64,66,68,70,72,74,76,78,80,82,84,86,88,90,92,94                
                db 96,98,100,102,104,106,108,110,112,114,116,118,120,122,124,127  
                db 127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127
                db 127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127
                db 127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127
                db 127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127
                ENDIF

sbuffer         STRUC
pack_type       dw ?
sample_rate     dw ?
data            dd ?
len_l           dw ?
len_h           dw ?
sbuffer         ENDS

                ;
                ;Misc. data
                ;

                IFDEF ADLIBG
CTRL_ADDR       dw ?
CTRL_DATA       dw ?
DSP_ADDR        dw ?
DSP_DATA        dw ?

mask_save       dw ?
pack_mode       dw ?
PRC_0_shadow    dw ?
                ENDIF

                IFDEF SBPRO
MIXADDR         dw ?
MIXDATA         dw ?
                ENDIF

                IFDEF SBLASTER
DSP_RESET       dw ?                    ;IO_Addr+06h
DSP_READ        dw ?                    ;+0Ah
DSP_WRITE_STAT  dw ?                    ;+0Ch
DSP_DATA_RDY    dw ?                    ;+0Eh
                ENDIF

DSP_IRQ         dw ?
DSP_DMA         dw ?

playing         dw ?
main_volume     dw ?
panpot          dw ?
block_ptr       dd ?
old_IRQ_o       dw ?
old_IRQ_s       dw ?
packing         dw ?
stereo          dw ?
current_rate    dw ?
pack_byte       dw ?
DMA_ptr         dd ?
DMA_len_l       dw ?
DMA_len_h       dw ?
blk_len         dw ?
iv_status       dw ?
loop_ptr        dd ?
loop_cnt        dw ?
IRQ_confirm     dw ?

buff_data_o     dw 2 dup (?)
buff_data_s     dw 2 dup (?)
buff_len_l      dw 2 dup (?)
buff_len_h      dw 2 dup (?)
buff_pack       dw 2 dup (?)
buff_sample     dw 2 dup (?)
buff_status     dw 2 dup (?)

buffer_mode     dw ?                   
DAC_status      dw ?
current_buffer  dw ?
MV_ftable       dd ?
MV_stable       dd ?

PIC0_val        db ?
PIC1_val        db ?

sample_cnt      dw ?
spkr_status     dw ?
old_freq        dw ?
old_stereo      dw ?

xblk_status     dw ?
xblk_tc         db ?
xblk_pack       db ?

silence_flag    db ?

init_OK         dw 0

;****************************************************************************
;*                                                                          *
;*  Interface primitives                                                    *
;*                                                                          *
;****************************************************************************

                IFDEF SBLASTER

send_timeout    PROC Data:BYTE          ;Returns AX=0 if write failed
                USES ds,si,di
                
                mov cx,200h
                mov dx,DSP_WRITE_STAT
__poll_cts:     in al,dx
                test al,80h
                jz __cts
                loop __poll_cts
                mov ax,0
                ret

__cts:          mov al,[Data]
                out dx,al
                mov ax,-1

                ret
                ENDP

;****************************************************************************
read_timeout    PROC                    ;Returns AX (-1 if no data ready)
                USES ds,si,di

                mov dx,DSP_DATA_RDY
                mov cx,200h
__poll_rdy:     in al,dx
                test al,80h
                jnz __rdy
                loop __poll_rdy
                mov ax,-1
                ret

__rdy:          mov dx,DSP_READ
                in al,dx
                mov ah,0
                ret
                ENDP                                       

;****************************************************************************
send_byte       PROC Data:BYTE          ;Write byte with timeout
                USES ds,si,di

                mov dx,DSP_WRITE_STAT

                mov cx,3
                mov bx,0
__wait_free_1:  sub bx,1
                sbb cx,0
                js __write_byte
                in al,dx
                or al,al
                js __wait_free_1

__write_byte:   mov al,[Data]
                out dx,al

                mov cx,3                ;wait until command received 
                mov bx,0
__wait_free_2:  sub bx,1
                sbb cx,0
                js __exit
                in al,dx
                or al,al
                js __wait_free_2

__exit:         ret
                ENDP

                ELSEIFDEF ADLIBG        ;Ad Lib Gold access routines must 
                                        ;preserve DS,SI,DI!

;****************************************************************************
IO_wait         PROC
                mov cx,500
                mov dx,CTRL_ADDR
__wait:         in al,dx
                and al,01000000b
                loopnz __wait
                ret
                ENDP

;****************************************************************************
enable_ctrl     PROC RegNum             ;Enable access to control chip
                mov dx,CTRL_ADDR   
                mov al,0ffh               
                out dx,al
                ret
                ENDP

;****************************************************************************
disable_ctrl    PROC RegNum             ;Disable access to control chip
                call IO_wait
                mov dx,CTRL_ADDR   
                mov al,0feh               
                out dx,al
                ret
                ENDP

;****************************************************************************
get_ctrl_reg    PROC RegNum             ;Get control chip register value
                call IO_wait
                mov dx,CTRL_ADDR
                mov ax,[RegNum]
                out dx,al
                call IO_wait
                mov dx,CTRL_DATA
                in al,dx
                ret
                ENDP

;****************************************************************************
set_ctrl_reg    PROC RegNum,Val         ;Set control chip register value
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

;****************************************************************************
MMA_wait        PROC                    ;Wait at least 470 nsec.
                mov cx,100
__kill_time:    jmp $+2
                loop __kill_time
                ret
                ENDP

;****************************************************************************
MMA_write       PROC Chan,Reg,Val       ;Write byte to MMA register
                mov ax,[Reg]
                mov dx,DSP_ADDR
                out dx,al
                call MMA_wait
                mov dx,[Chan]
                shl dx,1
                add dx,DSP_DATA
                mov ax,[Val]
                out dx,al
                call MMA_wait
                ret
                ENDP

                ENDIF

;****************************************************************************
reset_DSP       PROC                    ;Returns AX=0 if failure
                USES ds,si,di

                IFDEF PAS

                mov ax,1

                ELSEIFDEF ADLIBG

                call MMA_write C,0,9,10000000b
                call MMA_write C,0,9,01110110b

                call MMA_write C,1,9,10000000b
                call MMA_write C,1,9,01110110b

                mov ax,01110110b
                mov PRC_0_shadow,ax
                
                mov ax,1

                ELSEIFDEF SBLASTER

                mov dx,DSP_RESET        ;assert reset
                mov al,1
                out dx,al

                mov cx,20
__wait:         in al,dx                ;wait > 3 uS
                loop __wait

                mov al,0                ;drop reset
                out dx,al

                mov si,10h              ;try 16 times
__try_read:     call read_timeout
                cmp ax,0aah
                je __exit               ;(reset succeeded)
                dec si
                jnz __try_read

                mov ax,0                ;return 0 if failed

                ENDIF

__exit:         ret                   
                ENDP

;****************************************************************************
;*                                                                          *
;*  Internal procedures                                                     *
;*                                                                          *
;****************************************************************************

sub_ptr         PROC Off1,Seg1,Off2,Seg2        
                USES ds,si,di           ;Return DX:AX = ptr 2 - ptr 1

                mov ax,[Seg2]
                mov dx,0
                shl ax,1
                rcl dx,1
                shl ax,1
                rcl dx,1
                shl ax,1
                rcl dx,1
                shl ax,1
                rcl dx,1
                add ax,[Off2]
                adc dx,0

                mov bx,[Seg1]
                mov cx,0
                shl bx,1
                rcl cx,1
                shl bx,1
                rcl cx,1
                shl bx,1
                rcl cx,1
                shl bx,1
                rcl cx,1
                add bx,[Off1]
                adc cx,0

                sub ax,bx
                sbb dx,cx

                ret
                ENDP

;****************************************************************************
block_type      PROC                    ;Return AX=current block type
                USES ds,si,di

                lds si,block_ptr
                lodsb
                mov ah,0

                ret
                ENDP

;****************************************************************************
set_xblk        PROC                    ;Set extended .VOC block parms
                USES ds,si,di

                lds si,block_ptr        
                cmp BYTE PTR [si],8
                jne __exit              ;(not an extended block)

                mov al,[si+5]           ;get extended voice parameters
                mov xblk_tc,al          ;high byte of TC = normal sample rate

                mov ax,[si+6]           ;get pack (AL) and mode (AH)
                cmp ah,1                ;stereo?
                jne __set_pack

                or al,80h               ;yes, make pack byte negative

__set_pack:     mov xblk_pack,al

                mov xblk_status,1       ;flag extended block override

__exit:         ret
                ENDP

;****************************************************************************
marker_num      PROC                    ;Return AX=block's marker #
                USES ds,si,di

                lds si,block_ptr
                cmp BYTE PTR [si],4
                mov ax,-1       
                jne __exit              ;(not a marker block)
                mov ax,[si+4]           ;return marker #

__exit:         ret
                ENDP

;****************************************************************************
                IFDEF SBLASTER
DAC_spkr_on     PROC    
                USES ds,si,di

                cmp spkr_status,1
                je __exit               ;already on, exit
                mov spkr_status,1

                call send_byte C,0d1h
                call sysex_wait C,8     ;112 ms delay (SB Dev Kit, p. 14-11)

__exit:         ret
                ENDP

;****************************************************************************
DAC_spkr_off    PROC    
                USES ds,si,di

                cmp spkr_status,0
                je __exit               ;already off, exit
                mov spkr_status,0

                call halt_DMA C,1
                call send_byte C,0d3h
                call sysex_wait C,16    ;224 ms delay (SB Dev Kit, p. 14-11)

__exit:         ret
                ENDP
                ENDIF

;****************************************************************************
continue_DMA    PROC
                USES ds,si,di

                IFDEF PAS

                les di,MV_stable
                mov al,es:[di]._audiofilt
                or al,01000000b
                mov dx,AUDIOFILT
                out dx,al               ;start the transfer
                mov es:[di]._audiofilt,al

                ELSEIFDEF ADLIBG

                mov ax,PRC_0_shadow
                or ax,00000001b         ;set GO bit
                call MMA_write C,0,9,ax

                ELSEIFDEF SBLASTER

                call send_byte C,0d4h

                ENDIF

                mov playing,1

                ret
                ENDP

;****************************************************************************
halt_DMA        PROC WaitOpt
                USES ds,si,di

                IFDEF PAS

                les di,MV_stable
                mov al,es:[di]._audiofilt
                and al,10111111b
                mov dx,AUDIOFILT
                out dx,al               ;suspend the transfer
                mov es:[di]._audiofilt,al

                ELSEIFDEF ADLIBG

                mov ax,PRC_0_shadow
                and ax,11111110b        ;clear GO bit
                call MMA_write C,0,9,ax

                ELSEIFDEF SBLASTER

                IF NOT WAIT_FALSE_IRQ   ;"new" way to avoid SB glitches
                
                pushf                   ;save i-flag status

                mov dx,DSP_WRITE_STAT   ;register busy flag
                mov cx,7
                mov bx,0

__wait_busy:    sub bx,1
                sbb cx,0
                js __send_halt

                sti                     ;wait for busy status, abort if 
                jmp $+2                 ;playback ends by itself
                jmp $+2
                jmp $+2
                cmp playing,0
                je __not_playing
                cli

                in al,dx
                or al,al
                jns __wait_busy         

                mov cx,32768            ;wait for free edge
__wait_free:    dec cx
                jz __send_halt
                in al,dx
                or al,al
                js __wait_free

__send_halt:    mov al,0d0h             ;send halt DMA opcode
                out dx,al

__not_playing:  POP_F                   ;recover i-flag

                ELSE

                call send_byte C,0d0h

                cmp WaitOpt,0           ;optional wait to absorb dummy IRQ
                je __exit
                cmp iv_status,0
                jne __exit

                lea ax,IRQ_test         ;enable EOD interrupts from DSP
                call IRQ_set_vect C,ax,cs
                pushf
                sti
                call sysex_wait C,10    ;(140 milliseconds)
                POP_F
                call IRQ_rest_vect

                ENDIF                   ;NOT WAIT_FALSE_IRQ

                ENDIF                   ;DEF SBLASTER

__exit:         mov playing,0
                ret
                ENDP

;****************************************************************************
                IFNDEF SBLASTER         
match_constant  PROC TAddr,TSize,N      ;Get entry index for nearest match to
                USES ds,si,di           ;constant N

                cld
                mov si,[TAddr]
                mov bx,0ffffh
                mov cx,0
                mov di,0

__abs_delta:    lods WORD PTR cs:[si]
                sub ax,[N]
                cwd
                xor ax,dx
                sub ax,dx

                cmp ax,bx
                ja __next
                
                mov bx,ax
                mov di,cx

__next:         inc cx
                cmp cx,[TSize]
                jne __abs_delta

                mov ax,di

                ret
                ENDP
                ENDIF

;****************************************************************************
set_sample_rate PROC SB_Rate,Stereo     ;Establish DSP sample rate
                LOCAL freq
                USES ds,si,di

                pushf
                cli                     ;make sure IRQ's are off

                mov ax,[SB_Rate]        ;f in Hz. = 1E6 / (256 - SB_Rate)
                mov ah,0
                mov bx,256
                sub bx,ax
                mov dx,0fh
                mov ax,4240h
                div bx
                mov freq,ax

                IFDEF PAS

                mov ax,1
                mov cx,freq
                mov bx,[Stereo]

                cmp cx,old_freq         ;avoid clicks by sending
                jne __new_parms         ;new settings only when changed
                cmp bx,old_stereo
                je __exit

__new_parms:    mov old_freq,cx
                mov old_stereo,bx

	mov dx,PCMDATA          ;silence PCM output
	mov al,80h
	out dx,al

                mov ax,34dch
                mov dx,12h
                div freq
                mov cx,ax
                                                
                mov al,00110110b        ;timer 0, square wave, binary mode
                mov dx,TMRCTLR
                out dx,al
                mov dx,SAMPLERATE
                mov al,cl
                out dx,al
                jmp $+2
                mov al,ch
                out dx,al

                IF PAS_FILTER
                lea ax,filter_cutoff    ;select filter for freq in Hz. / 2
                mov cx,freq
                shr cx,1
                call match_constant C,ax,6,cx
                mov si,ax               ;SI = index into filter_value

                les di,MV_stable
                mov al,es:[di]._audiofilt
                and al,11100000b
                or al,filter_value[si]
                mov dx,AUDIOFILT
                out dx,al
                mov es:[di]._audiofilt,al
                ENDIF

                ELSEIFDEF ADLIBG

                mov ax,1
                mov cx,freq
                mov bx,[Stereo]

                cmp cx,old_freq         ;avoid Ad Lib Gold clicks by sending
                jne __new_parms         ;new settings only when changed
                cmp bx,old_stereo
                je __exit

__new_parms:    mov old_freq,cx
                mov old_stereo,bx

                mov ax,pack_byte
                and ax,10000111b
                mov si,0
__find_pack:    cmp pack_modes[si],al
                je __pack_found
                inc si
                cmp si,6
                jne __find_pack
                mov si,0
__pack_found:   mov pack_mode,si

                lea ax,ADPCM_Hz
                cmp si,1
                je __find_freq
                cmp si,3
                je __find_freq
                lea ax,PCM_Hz

__find_freq:    cmp bx,0
                je __lookup
                shr cx,1                        ;sample rate /= 2 for stereo

__lookup:       call match_constant C,ax,4,cx
                mov di,ax

                call MMA_write C,0,9,10000000b  ;reset both FIFOs
                call MMA_write C,1,9,10000000b

                call MMA_write C,0,11,0         ;write 4 dummy bytes to
                call MMA_write C,0,11,0         ;allow proper FIFO DMA 
                call MMA_write C,0,11,0         ;initialization
                call MMA_write C,0,11,0

                mov al,freq_bits[di]
                or al,PRC_0_values[si]
                mov PRC_0_shadow,ax
                call MMA_write C,0,9,ax

                mov al,freq_bits[di]
                or al,PRC_1_values[si]
                call MMA_write C,1,9,ax

                mov al,SFC_0_values[si]
                call MMA_write C,0,12,ax

                mov al,SFC_1_values[si]
                call MMA_write C,1,12,ax

                ELSEIFDEF SBLASTER

                call send_byte C,40h   
                call send_byte C,[SB_Rate]

                ENDIF

__exit:         POP_F
                ret
                ENDP

;****************************************************************************
set_volume      PROC                    ;Establish output lvl w/vol, pan
                USES ds,si,di

                IFDEF STEREO

                mov si,127
                sub si,panpot
                mov al,pan_graph[si]
                mul BYTE PTR main_volume
                mov di,ax               ;DI = right volume 0-16129
                mov si,panpot
                mov al,pan_graph[si]
                mul BYTE PTR main_volume
                mov si,ax               ;SI = left volume 0-16129

                IFDEF SBPRO

                mov ax,di
                mov cx,10
                shr ax,cl
                mov bh,al               ;right volume 0-15
                mov ax,si
                mov cx,6
                shr ax,cl
                and ax,11110000b
                mov bl,al               ;left volume 0-15
                mov dx,MIXADDR
                mov al,4                ;select voice volume register
                out dx,al
                jmp $+2
                mov dx,MIXDATA
                or bl,bh
                mov al,bl
                out dx,al             

                ELSEIFDEF PAS

                mov cx,161
                mov ax,di
                mov dx,0
                div cx                  ;right volume 0-100 (sic)
                push ax                 
                mov ax,si
                mov dx,0
                div cx                  ;left volume 0-100
                mov bx,ax
                mov cx,BI_OUTPUTMIXER
                mov dx,BI_L_PCM
                les di,MV_ftable
                call dword ptr es:[di+0]
                pop bx
                mov cx,BI_OUTPUTMIXER
                mov dx,BI_R_PCM
                les di,MV_ftable
                call dword ptr es:[di+0]

                ELSEIFDEF ADLIBG

                cmp stereo,0            ;ALG panning works only in stereo mode
                jne __set_vol
                add si,di
                shr si,1
                mov di,si

__set_vol:      mov cx,6
                shr di,cl               ;right volume 0-252
                call MMA_write C,0,10,di

                mov cx,6
                shr si,cl               ;left volume 0-252
                call MMA_write C,1,10,si

                ENDIF

                ENDIF                   ;IFDEF STEREO

                IFDEF SBLASTER
                call DAC_spkr_on
                ENDIF

__exit:         ret
                ENDP

;****************************************************************************
IRQ_set_vect    PROC Handler:FAR PTR    ;Install DMA IRQ handler
                USES ds,si,di

                pushf                   ;avoid interruption
                cli

                cmp iv_status,0         ;avoid redundant settings
                jne __exit

                mov bx,DSP_IRQ          ;index interrupt vector for IRQ
                cmp bx,8
                jb __calc_vect
                add bx,60h              ;index slave PIC vectors if IRQ > 7
__calc_vect:    add bx,8
                shl bx,1
                shl bx,1

                mov ax,0                ;save old handler address, install
                mov ds,ax               ;new handler
                les di,[bx]
                mov old_IRQ_s,es
                mov old_IRQ_o,di
                les di,[Handler]
                mov [bx],di
                mov [bx+2],es

                mov cx,DSP_IRQ          ;enable hardware interrupts from DSP
                mov bx,1
                shl bx,cl
                not bx
                in al,0a1h
                mov PIC1_val,al
                and al,bh
                out 0a1h,al
                in al,21h
                mov PIC0_val,al
                and al,bl
                out 21h,al

                IFDEF PAS
                mov dx,INTRCTLRST
                out dx,al
                jmp $+2
                in al,dx
                mov dx,INTRCTLR
                in al,dx
                or al,00001000b         ;enable IRQs on sample buffer empty
                out dx,al
                ENDIF

                mov iv_status,1

__exit:         POP_F
                ret    
                ENDP

;****************************************************************************
IRQ_rest_vect   PROC
                USES ds,si,di

                pushf                   ;avoid interruption
                cli

                cmp iv_status,1         ;avoid redundant settings
                jne __exit

                IFDEF PAS
                mov dx,INTRCTLRST
                out dx,al
                jmp $+2
                in al,dx
                mov dx,INTRCTLR
                in al,dx
                and al,11110111b        ;disable IRQs on sample buffer empty
                out dx,al
                ENDIF

                mov cx,DSP_IRQ          ;stop hardware interrupts from DSP
                mov bx,1
                shl bx,cl
                in al,0a1h
                or al,bh
                and al,PIC1_val         ;don't kill any interrupts that were
                out 0a1h,al             ;initially active
                in al,21h
                or al,bl
                and al,PIC0_val
                out 21h,al

                mov bx,DSP_IRQ          ;index interrupt vector for IRQ
                cmp bx,8
                jb __calc_vect
                add bx,60h              ;index slave PIC vectors if IRQ > 7
__calc_vect:    add bx,8
                shl bx,1
                shl bx,1

                mov ax,0                ;restore old handler address
                mov ds,ax             
                mov ax,old_IRQ_o
                mov dx,old_IRQ_s
                mov [bx],ax
                mov [bx+2],dx

                mov iv_status,0

__exit:         POP_F
                ret
                ENDP

;****************************************************************************
DMAC_word_cnt   PROC                    ;Return DMA word count
                USES ds,si,di

                pushf
                cli

                cmp silence_flag,0      ;if silence packing was used, return
                mov silence_flag,0      ;0ffffh (transfer done)
                mov ax,0ffffh
                jne __exit              

                mov dx,DSP_DMA
                shl dx,1
                add dx,1                
                in al,dx                ;DMAnCNT: Channel n Word Count
                mov bl,al
                in al,dx
                mov ah,al
                mov al,bl

__exit:         POP_F
                ret
                ENDP

;****************************************************************************
program_DMAC    PROC Addr,Page,Len      ;Program 8237 DMAC for DAC transfer
                USES ds,si,di

                pushf                   ;make sure interrupts are off
                cli

                IFDEF PAS
                les di,MV_stable
                mov al,es:[di]._crosschannel
                mov dx,CROSSCHANNEL
                or al,10000000b         ;secure the DMA channel
                mov es:[di]._crosschannel,al
                out dx,al
                ENDIF

                mov ax,DSP_DMA
                or ax,4h                ;DMASET: Set bit (mask or request)
                out 0ah,al   

                mov bx,DSP_DMA
                mov dx,80h              ;DMAPAG: Base address
                add dl,DMAPAG_offset[bx]
                mov ax,[Page]
                out dx,al

                mov al,0
                out 0ch,al              ;DMACLFF: Clear Byte Pointer Flip Flop
                
                mov dx,DSP_DMA
                shl dx,1
                mov ax,[Addr]
                out dx,al               ;DMAnADR: Channel n Current Address
                mov al,ah
                out dx,al

                mov dx,DSP_DMA
                shl dx,1
                add dx,1
                mov ax,[Len]
                out dx,al               ;DMAnCNT: Channel n Word Count
                mov al,ah
                out dx,al

                mov ax,48h             
                or ax,DSP_DMA
                out 0bh,al              ;DMAMD: Write Mode Register

                mov ax,DSP_DMA
                or ax,0h                ;DMARST: Reset bit (mask or request)
                out 0ah,al   

                POP_F
                ret
                ENDP

;****************************************************************************
                IFDEF SBLASTER

IRQ_test        PROC                    ;DMA IRQ handler for IRQ detection

                push dx
                push ax

                mov dx,DSP_DATA_RDY
                in al,dx                ;acknowledge the interrupt

                mov IRQ_confirm,1       ;flag interrupt received OK
                mov playing,0

                mov al,20h              ;send EOI to PIC
                cmp DSP_IRQ,8           ;clear PIC1 if IRQ >= 8
                jb __master
                out 0a0h,al
__master:       out 20h,al

__exit:         pop ax
                pop dx

                iret
                ENDP

                ENDIF

;****************************************************************************
IRQ_play_VOC    PROC                    ;DMA IRQ handler for .VOC file output
                     
                push ax
                push bx
                push cx
                push dx
                push si
                push di
                push bp
                push ds
                push es
                cld

                IFDEF ADLIBG
                mov dx,DSP_ADDR
                in al,dx
                and al,00000001b        ;FIF0 interrupt?
                jz __EOI                ;no, exit
                ENDIF

                IF CHECK_DMAC
                cmp DSP_IRQ,7           ;(spurious IRQs occur only on IRQ 7)
                jne __DMAC_OK

                call DMAC_word_cnt      ;see if DMA transfer is truly over
                cmp ax,0
                je __DMAC_OK            ;(can occur during packed transfers)
                cmp ax,0ffffh
                jne __EOI
__DMAC_OK:
                ENDIF

                IFDEF PAS

                mov dx,INTRCTLRST
                in al,dx                ;acknowledge the interrupt
                test al,00001000b
                jz __EOI                ;it wasn't caused by our board, exit
                out dx,al

                les di,MV_stable
                mov al,es:[di]._crosschannel
                mov dx,CROSSCHANNEL
                and al,10111111b        ;kill the PCM engine until re-done
                mov es:[di]._crosschannel,al
                out dx,al

                ELSEIFDEF SBLASTER

                mov dx,DSP_DATA_RDY
                in al,dx                ;acknowledge the interrupt

                ENDIF

                cmp playing,0
                je __EOI
                mov playing,0

                IFDEF PAS
                call halt_DMA C,0
                ENDIF

                mov ax,DMA_len_l        ;at end of block?
                or ax,DMA_len_h
                jz __end_of_block

                call xfer_chunk         ;no, send next chunk
                jmp __EOI

__end_of_block: call IRQ_rest_vect      ;else go on to next block in chain
                call next_block         
                call process_block

__EOI:          mov al,20h              ;send EOI to PIC
                cmp DSP_IRQ,8           ;clear PIC1 if IRQ >= 8
                jb __master
                out 0a0h,al
__master:       out 20h,al

__exit:         pop es
                pop ds
                pop bp
                pop di
                pop si
                pop dx
                pop cx
                pop bx
                pop ax

                iret
                ENDP

;****************************************************************************
IRQ_play_buffer PROC                    ;DMA IRQ handler for double-buffering
                     
                push ax
                push bx
                push cx
                push dx
                push si
                push di
                push bp
                push ds
                push es
                cld

                IFDEF ADLIBG
                mov dx,DSP_ADDR
                in al,dx
                and al,00000001b        ;FIF0 interrupt?
                jz __EOI                ;no, exit
                ENDIF

                IF CHECK_DMAC
                cmp DSP_IRQ,7           ;(spurious IRQs occur only on IRQ 7)
                jne __DMAC_OK

                call DMAC_word_cnt      ;see if DMA transfer is truly over
                cmp ax,0
                je __DMAC_OK            ;(can occur during packed transfers)
                cmp ax,0ffffh
                jne __EOI
__DMAC_OK:
                ENDIF

                IFDEF PAS

                mov dx,INTRCTLRST
                in al,dx                ;acknowledge the interrupt
                test al,00001000b
                jz __EOI                ;it wasn't caused by our board, exit
                out dx,al

                les di,MV_stable
                mov al,es:[di]._crosschannel
                mov dx,CROSSCHANNEL
                and al,10111111b        ;kill the PCM engine until re-done
                mov es:[di]._crosschannel,al
                out dx,al

                ELSEIFDEF SBLASTER

                mov dx,DSP_DATA_RDY
                in al,dx                ;acknowledge the interrupt

                ENDIF

                cmp playing,0
                je __EOI
                mov playing,0

                IFDEF PAS
                call halt_DMA C,0
                ENDIF

                mov ax,DMA_len_l        ;at end of block?
                or ax,DMA_len_h
                jz __end_of_block
                     
                call xfer_chunk         ;no, send next chunk
                jmp __EOI

__end_of_block: call IRQ_rest_vect      ;else look for an unplayed buffer...

                mov bx,current_buffer
                mov buff_status[bx],DAC_DONE

                call next_buffer
                cmp ax,-1
                je __EOI                ;no buffers left, terminate playback
                call process_buffer C,ax

__EOI:          mov al,20h              ;send EOI to PIC
                cmp DSP_IRQ,8           ;clear PIC1 if IRQ >= 8
                jb __master
                out 0a0h,al
__master:       out 20h,al

__exit:         pop es
                pop ds
                pop bp
                pop di
                pop si
                pop dx
                pop cx
                pop bx
                pop ax

                iret
                ENDP

;****************************************************************************
hardware_xfer   PROC                    ;Program hardware to send chunk
                USES ds,si,di

                IFDEF PAS

                mov cx,blk_len
                jcxz __exit
                add cx,1                ;1-65535; 0=65536

                mov al,01110100b        ;program sample buffer counter
                mov dx,TMRCTLR
                out dx,al
                mov dx,SAMPLECNT
                mov al,cl
                out dx,al
                jmp $+2
                mov al,ch
                out dx,al

                les di,MV_stable        ;reset PCM state machine
                mov ah,es:[di]._crosschannel
                and ah,00001111b
                mov al,10110000b
                cmp stereo,0
                je __set_xchan
                mov al,10010000b
__set_xchan:    or al,ah
                mov dx,CROSSCHANNEL
                out dx,al
                jmp $+2
                or al,01000000b
                out dx,al
                mov es:[di]._crosschannel,al

                mov al,es:[di]._audiofilt
                or al,11000000b
                mov dx,AUDIOFILT
                out dx,al               ;start the transfer
                mov es:[di]._audiofilt,al

                ELSEIFDEF ADLIBG

                mov ax,PRC_0_shadow
                or ax,00000001b         ;set GO bit
                call MMA_write C,0,9,ax

                ELSEIFDEF SBLASTER

                mov bx,packing          ;program SB DSP to transfer data
                call send_byte C,WORD PTR pack_opcodes[bx]

                mov ax,blk_len
                and ax,0ffh
                call send_byte C,ax

                mov ax,blk_len
                and ax,0ff00h
                xchg al,ah
                call send_byte C,ax

                ENDIF

                mov playing,1

__exit:         ret
                ENDP    

;****************************************************************************
xfer_chunk      PROC                    ;Get addr, size of next chunk; send it
                USES ds,si,di

                lds si,DMA_ptr
                FAR_TO_HUGE ds,si       ;DS:SI = start of data to send
                mov di,ds
                and di,0f000h  
                add di,1000h            ;DI:0000 = start of next physical page
                                        
                call sub_ptr C,si,ds,0,di
                sub ax,1
                sbb dx,0
                mov blk_len,ax          ;AX = # of bytes left in page -1

                mov ax,DMA_len_l        ;set AX:DX = total # of bytes left -1
                mov dx,DMA_len_h
                sub ax,1
                sbb dx,0                
                cmp dx,0                ;> 64K?
                ja __len_valid          ;yes, send rest of current page only
                cmp ax,blk_len          ;> # of bytes left in page?
                ja __len_valid          ;yes, send rest of current page only

                mov blk_len,ax          ;else send all remaining data

__len_valid:    mov ax,ds               ;program DMA controller with chunk len
                mov dx,0                ;and addr
                shl ax,1
                rcl dx,1
                shl ax,1
                rcl dx,1
                shl ax,1
                rcl dx,1
                shl ax,1
                rcl dx,1
                add ax,si
                adc dx,0
                call program_DMAC C,ax,dx,blk_len

                IFDEF SBPRO
                mov dx,MIXADDR
                mov al,0eh              ;select DNFI/VSTC flag set
                out dx,al
                jmp $+2
                mov dx,MIXDATA
                mov ax,stereo
                out dx,al               ;Set stereo/mono mode; filtering = ON
                ENDIF

                call hardware_xfer
                
                lds si,DMA_ptr          ;add len of chunk +1 to DMA pointer
                ADD_PTR blk_len,0,ds,si
                ADD_PTR 1,0,ds,si
                mov WORD PTR DMA_ptr,si
                mov WORD PTR DMA_ptr+2,ds

                mov ax,DMA_len_l        ;subtract len of transmitted chunk +1
                mov dx,DMA_len_h
                sub ax,blk_len
                sbb dx,0
                sub ax,1
                sbb dx,0
                mov DMA_len_h,dx
                mov DMA_len_l,ax

                cmp packing,4           ;did we just send an initial chunk?
                jae __exit              
                add packing,4           ;yes, switch to "continue" opcode set

__exit:         ret                     ;return DX:AX = remaining bytes 
                ENDP

;****************************************************************************
DMA_transfer    PROC Addr:FAR PTR,LenL,LenH
                USES ds,si,di           ;Set up DMA transfer, send first chunk

                lds si,[Addr]           
                mov ax,[LenL]
                mov dx,[LenH]

                mov WORD PTR DMA_ptr,si
                mov WORD PTR DMA_ptr+2,ds
                mov DMA_len_l,ax
                mov DMA_len_h,dx

                call xfer_chunk

                ret
                ENDP

;****************************************************************************
next_block      PROC                    ;Index next block in voice data
                USES ds,si,di

                lds si,block_ptr
                inc si                  ;skip block type
                lodsw                   
                mov dl,[si]
                mov dh,0                ;blk len: AL=LSB, AH=KSB, DL=MSB, DH=0
                inc si

                ADD_PTR ax,dx,ds,si     ;point to next block

                mov WORD PTR block_ptr,si
                mov WORD PTR block_ptr+2,ds

                ret
                ENDP

;****************************************************************************
process_block   PROC                    ;Process current block in voice data
                USES ds,si,di           ;(May be called from IRQ handler)

__do_block:     call block_type
                cmp ax,0                ;terminator?
                je __terminate
                cmp ax,1                ;new voice block?
                je __new_voice
                cmp ax,2                ;continued voice block?
                je __cont_voice
                cmp ax,3                ;silence period?
                je __silence
                cmp ax,4                ;marker (end of data?)
                je __terminate
                cmp ax,6                ;beginning of repeat loop?
                je __rept_loop
                cmp ax,7                ;end of repeat loop?
                je __end_loop
                cmp ax,8                ;extended block type?
                je __extended
                jmp __skip_block        ;else unrecognized block type, skip it

__extended:     call set_xblk
                jmp __skip_block

__terminate:    mov DAC_status,DAC_DONE
                jmp __exit

__skip_block:   call next_block
                jmp __do_block

__rept_loop:    lds si,block_ptr
                mov ax,[si+4]
                mov loop_cnt,ax
                call next_block
                lds si,block_ptr
                mov WORD PTR loop_ptr,si
                mov WORD PTR loop_ptr+2,ds
                jmp __do_block

__end_loop:     cmp loop_cnt,0
                je __skip_block
                lds si,loop_ptr
                mov WORD PTR block_ptr,si
                mov WORD PTR block_ptr+2,ds
                cmp loop_cnt,0ffffh
                je __do_block
                dec loop_cnt
                jmp __do_block

__silence:      IFDEF PAS

                jmp __skip_block

                ELSEIFDEF ADLIBG

                jmp __skip_block

                ELSEIFDEF SBLASTER

                mov DMA_len_l,0
                mov DMA_len_h,0
                lea ax,IRQ_play_VOC     ;enable EOD interrupts from DSP
                call IRQ_set_vect C,ax,cs
                lds si,block_ptr        ;generate silent period

                call set_sample_rate C,[si+6],stereo
                call send_byte C,80h    ;(undoc'd DSP opcode used by C. Labs!)
                call send_byte C,[si+4]
                call send_byte C,[si+5]

                mov playing,1
                mov silence_flag,1      ;set up to skip DMA test
                jmp __exit

                ENDIF

__cont_voice:   call set_sample_rate C,current_rate,stereo

                lds si,block_ptr        ;continue output from new voice block
                lea ax,IRQ_play_VOC     ;enable EOD interrupts from DSP
                call IRQ_set_vect C,ax,cs
                mov ax,[si+1]
                mov dl,[si+3]
                mov dh,0                ;DX:AX = voice len
                ADD_PTR 4,0,ds,si       ;DS:SI -> start-of-data
                call DMA_transfer C,si,ds,ax,dx
                jmp __exit

__new_voice:    lds si,block_ptr        ;initiate output from new voice block
                mov bl,[si+4]
                mov al,[si+5]
                mov bh,0
                mov ah,0

                cmp xblk_status,0       ;previous extended block overrides
                je __use_vd             ;data block values
                mov al,xblk_pack
                mov bl,xblk_tc
                mov xblk_status,0

__use_vd:       mov pack_byte,ax
                mov packing,ax
                and packing,7fh
                and ax,80h
                mov cx,6
                shr ax,cl
                and ax,10b
                mov stereo,ax

                mov current_rate,bx

                call set_sample_rate C,current_rate,stereo

                lea ax,IRQ_play_VOC     ;enable EOD interrupts from DSP
                call IRQ_set_vect C,ax,cs
                mov ax,[si+1]
                mov dl,[si+3]
                mov dh,0                
                sub ax,2                
                sbb dx,0                ;DX:AX = voice len
                ADD_PTR 6,0,ds,si       ;DS:SI -> start-of-data
                call DMA_transfer C,si,ds,ax,dx

__exit:         ret
                ENDP

;****************************************************************************
next_buffer     PROC                    ;Find a registered, unplayed buffer
                USES ds,si,di

                mov ax,0                ;buffer 0 registered?
                cmp buff_status[0*2],DAC_STOPPED
                je __return             ;yes, return its handle
                mov ax,1                ;buffer 1 registered?
                cmp buff_status[1*2],DAC_STOPPED
                je __return             ;yes, return its handle

                mov DAC_status,DAC_DONE ;else signal playback complete and
                mov ax,-1               ;return AX=-1

__return:       ret
                ENDP
                     
;****************************************************************************
process_buffer  PROC Buf                ;Play specified buffer
                USES ds,si,di

                mov si,[Buf]            ;get buffer handle 
                shl si,1                ;derive index

                mov buff_status[si],DAC_PLAYING
                mov current_buffer,si   ;save index to playing buffer

                mov ax,buff_pack[si]
                mov pack_byte,ax
                mov packing,ax
                and packing,7fh
                and ax,80h
                mov cx,6
                shr ax,cl
                and ax,10b
                mov stereo,ax

                call set_sample_rate C,buff_sample[si],ax

                lea ax,IRQ_play_buffer  ;enable EOD interrupts from DSP
                call IRQ_set_vect C,ax,cs

                call DMA_transfer C,buff_data_o[si],buff_data_s[si],\
                     buff_len_l[si],buff_len_h[si]

__exit:         ret
                ENDP

;****************************************************************************
sysex_wait      PROC Delay              ;Machine-independent delay
                USES ds,si,di          

                mov ax,40h              ;wait n VBL periods (14 ms/period min, 
                mov ds,ax               ;requires CGA/EGA/VGA/XGA video)

                mov dx,ds:[63h]         ;get CRTC Address register location
                add dl,6                ;get CRTC Status register location

                mov cx,[Delay]
                jcxz __exit

__sync_1:       in al,dx            
                test al,8
                jz __sync_1             

__sync_2:       in al,dx
                test al,8
                jnz __sync_2

                loop __sync_1

__exit:         ret
                ENDP

;****************************************************************************
                IFDEF NEEDS_FORMAT      ;(if PCM format not SB-compatible)

format_block    PROC BufPtr:DWORD,Len:DWORD
                USES ds,si,di

                les di,[BufPtr]

                mov cx,WORD PTR [Len]   ;set CX = # of 16-byte chunks
                mov dx,WORD PTR [Len+2]
                shr dx,1
                rcr cx,1
                shr dx,1
                rcr cx,1
                shr dx,1
                rcr cx,1
                shr dx,1
                rcr cx,1
                jcxz __remain

__for_chunk:
                sub BYTE PTR es:[di],80h
                sub BYTE PTR es:[di+1],80h
                sub BYTE PTR es:[di+2],80h
                sub BYTE PTR es:[di+3],80h
                sub BYTE PTR es:[di+4],80h
                sub BYTE PTR es:[di+5],80h
                sub BYTE PTR es:[di+6],80h
                sub BYTE PTR es:[di+7],80h
                sub BYTE PTR es:[di+8],80h
                sub BYTE PTR es:[di+9],80h
                sub BYTE PTR es:[di+10],80h
                sub BYTE PTR es:[di+11],80h
                sub BYTE PTR es:[di+12],80h
                sub BYTE PTR es:[di+13],80h
                sub BYTE PTR es:[di+14],80h
                sub BYTE PTR es:[di+15],80h

                ADD_PTR 16,0,es,di
                loop __for_chunk

__remain:       mov cx,WORD PTR [Len]   ;format remaining 0-15 bytes
                and cx,0fh
                jz __exit

__rem_loop:     sub BYTE PTR es:[di],80h
                inc di
                loop __rem_loop

__exit:         ret
                ENDP

                ENDIF

;****************************************************************************
;*                                                                          *
;*  Public (API-accessible) procedures                                      *
;*                                                                          *
;****************************************************************************

describe_driver PROC H                  ;Return far ptr to DDT
                USES ds,si,di

                pushf
                cli

                mov dx,cs
                mov device_name_s,dx
                lea ax,DDT

                POP_F
                ret
                ENDP

;****************************************************************************
shutdown_driver PROC H,SignOff:FAR PTR
                USES ds,si,di

                pushf
                cli

                cmp init_OK,0
                je __exit

                IFDEF SBLASTER
                call DAC_spkr_off
                ENDIF

                call stop_d_pb

                IFDEF PAS

                les di,MV_stable
                mov al,es:[di]._crosschannel
                mov dx,CROSSCHANNEL      ;disable DRQs from PAS
                and al,00111111b         ;and disable PCM state machine
                mov es:[di]._crosschannel,al
                out dx,al

                ELSEIFDEF ADLIBG

                call enable_ctrl
                call set_ctrl_reg C,13h,mask_save
                call disable_ctrl

                ENDIF

                call reset_DSP

                mov init_OK,0

__exit:         POP_F
                ret
                ENDP

;****************************************************************************
set_d_pb_pan    PROC H,Pan              ;Set digital playback panpot 0-127
                USES ds,si,di           

                pushf
                cli

                mov ax,[Pan]
                mov panpot,ax

                call set_volume 

                POP_F
                ret
                ENDP

;****************************************************************************
get_d_pb_pan    PROC H                  ;Get digital playback panpot 0-127
                USES ds,si,di           

                pushf
                cli

                mov ax,panpot

                POP_F
                ret
                ENDP

;****************************************************************************
set_d_pb_vol    PROC H,Vol              ;Set digital playback volume 0-127        
                USES ds,si,di           ;(0=off; anything else=on)

                pushf
                cli

                mov ax,[Vol]
                mov main_volume,ax

                call set_volume

                POP_F
                ret
                ENDP

;****************************************************************************
get_d_pb_vol    PROC H                  ;Get digital playback volume 0-127        
                USES ds,si,di          

                pushf
                cli

                mov ax,main_volume

                POP_F
                ret
                ENDP

;****************************************************************************
detect_device   PROC H,IO_ADDR,IRQ,DMA,DRQ  ;Check for presence of supported
                USES ds,si,di               ;device

                pushf
                cli

                IFDEF SBLASTER
                push DSP_RESET       
                push DSP_READ        
                push DSP_WRITE_STAT  
                push DSP_DATA_RDY    
                push DSP_IRQ
                push DSP_DMA
                ENDIF

                IFDEF SBPRO
                push MIXDATA
                push MIXADDR
                ENDIF

                IFDEF ADLIBG
                push CTRL_ADDR
                push CTRL_DATA
                ENDIF

                mov spkr_status,-1
                mov iv_status,0

                IFDEF ADLIBG              ;ALG, check for control chip

                mov dx,IO_ADDR
                add dx,2
                mov CTRL_ADDR,dx
                inc dx
                mov CTRL_DATA,dx

                call enable_ctrl

                call get_ctrl_reg C,9     ;get left volume
                mov si,ax
                call get_ctrl_reg C,10    ;get right volume
                mov di,ax

                xor si,0101b              ;tweak a few bits
                xor di,1010b

                call set_ctrl_reg C,9,si  ;write the tweaked values back
                call set_ctrl_reg C,10,di

                call get_ctrl_reg C,9     ;see if changes took effect
                cmp ax,si
                mov ax,0                  ;(return failure)
                jne __exit
                call get_ctrl_reg C,10
                cmp ax,di
                mov ax,0                  ;(return failure)
                jne __exit

                xor si,0101b              ;control chip found: restore old
                xor di,1010b              ;values & re-enable FM sound

                call set_ctrl_reg C,9,si
                call set_ctrl_reg C,10,di

                call disable_ctrl
                mov ax,1                  ;return success
                jmp __exit

                ELSEIFDEF PAS

                mov ax,0bc00h             ;PAS, look for MVSOUND.SYS driver
                mov bx,03f3fh
                int 2fh                   ;DOS MPX interrupt
                xor bx,cx
                xor bx,dx
                cmp bx,'MV'               ;MediaVision flag
                mov ax,0
                jne __exit

                mov ax,0bc03h             ;get driver function table address
                int 2fh
                mov WORD PTR MV_ftable,bx
                mov WORD PTR MV_ftable+2,dx

                mov ax,0bc02h             ;get pointer to state table
                int 2fh
                mov WORD PTR MV_stable,bx
                mov WORD PTR MV_stable+2,dx

                mov ax,1
                jmp __exit

                ELSEIFDEF SBLASTER        ;SB / SBPRO, detect SB DSP chip

                mov ax,IO_ADDR
                IFDEF SBPRO
                add ax,4
                mov MIXADDR,ax
                add ax,1
                mov MIXDATA,ax
                add ax,1
                ELSE
                add ax,6
                ENDIF
                mov DSP_RESET,ax
                add ax,4
                mov DSP_READ,ax
                add ax,2
                mov DSP_WRITE_STAT,ax
                add ax,2
                mov DSP_DATA_RDY,ax
                mov ax,IRQ
                mov DSP_IRQ,ax
                mov ax,DMA
                mov DSP_DMA,ax

                call send_timeout C,0d3h  ;turn speaker off and let the new 
                call sysex_wait C,16      ;setting take effect

                call reset_DSP              
                or ax,ax
                jz __exit                 ;(reset failed)

                IFDEF SBPRO               ;look for CT-1345A mixer chip
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
                ENDIF

                pushf                     ;prepare for IRQ/DMA test....
                sti                       ;output a few bytes of data to 
                mov IRQ_confirm,0         ;trigger EOD IRQ on selected line
                mov packing,0
                mov pack_byte,0
                mov stereo,0
                mov xblk_status,0
                mov si,1                  ;assume success
                call set_sample_rate C,166,0
                lea ax,IRQ_test           ;enable EOD interrupts from DSP
                call IRQ_set_vect C,ax,cs
                call DMA_transfer C,0,0,4,0
                mov di,34                 ;wait typ. 500 milliseconds
__poll_confirm: call sysex_wait C,1
                cmp IRQ_confirm,1         ;EOD interrupt occurred?
                je __end_IRQ_test         ;yes, device IRQ valid
                dec di
                jnz __poll_confirm
                mov si,0                  ;IRQ handler never called -- failed
__end_IRQ_test: call IRQ_rest_vect
                POP_F
                mov ax,si

                ENDIF                     ;ELSEIFDEF SBLASTER

__exit:         IFDEF ADLIBG
                pop CTRL_DATA
                pop CTRL_ADDR
                ENDIF

                IFDEF SBPRO
                pop MIXADDR
                pop MIXDATA
                ENDIF

                IFDEF SBLASTER
                pop DSP_DMA
                pop DSP_IRQ
                pop DSP_DATA_RDY
                pop DSP_WRITE_STAT
                pop DSP_READ
                pop DSP_RESET
                ENDIF

                POP_F                     ;return AX=0 if not found
                ret
                ENDP

;****************************************************************************
init_driver     PROC H,IO_ADDR,IRQ,DMA,DRQ  
                USES ds,si,di

                pushf
                cli

                mov ax,IO_ADDR

                IFDEF SBPRO
                add ax,4
                mov MIXADDR,ax
                add ax,1
                mov MIXDATA,ax
                add ax,1
                ELSEIFDEF SBSTD
                add ax,6
                ENDIF

                IFDEF SBLASTER

                mov DSP_RESET,ax
                add ax,4
                mov DSP_READ,ax
                add ax,2
                mov DSP_WRITE_STAT,ax
                add ax,2
                mov DSP_DATA_RDY,ax

                mov ax,IRQ
                mov DSP_IRQ,ax
                mov ax,DMA
                mov DSP_DMA,ax

                ELSEIFDEF ADLIBG

                mov ax,IO_ADDR
                add ax,2
                mov CTRL_ADDR,ax          ;set I/O parms for control chip
                inc ax
                mov CTRL_DATA,ax
                inc ax
                mov DSP_ADDR,ax           ;set I/O parms for sampling channels
                inc ax
                mov DSP_DATA,ax

                ENDIF                     ;IFDEF SBLASTER

                mov spkr_status,-1
                mov iv_status,0
                mov xblk_status,0
                mov silence_flag,0
                mov old_freq,-1
                mov old_stereo,-1
                mov playing,0

                call detect_device C,0,IO_ADDR,IRQ,DMA,DRQ
                or ax,ax
                jz __exit_init            ;verify device, establish addresses

                IFDEF PAS
                                          ;get current DMA and IRQ settings
                mov ax,0bc04h             ;and enable DRQ's
                int 2fh                
                mov DSP_DMA,bx
                mov DSP_IRQ,cx

                les di,MV_stable
                mov al,es:[di]._crosschannel
                mov dx,CROSSCHANNEL
                or al,10000000b
                mov es:[di]._crosschannel,al
                out dx,al

                ELSEIFDEF ADLIBG

                call enable_ctrl
                call get_ctrl_reg C,11h   ;(Audio Selection)
                and ax,11111100b          ;set filters to playback mode
                call set_ctrl_reg C,11h,ax

                call get_ctrl_reg C,13h   ;(Audio IRQ/DMA Select - Channel 0)
                mov mask_save,ax
                mov si,ax
                or ax,10001000b           ;(DEN0 | AEN)
                call set_ctrl_reg C,13h,ax
                call disable_ctrl

                mov ax,si
                and ax,01110000b          ;isolate DMA SEL 0 bits
                mov cx,4
                shr ax,cl
                mov DSP_DMA,ax            ;record DMA channel in use

                and si,00000111b          ;isolate INT SEL A bits
                mov al,selected_IRQ[si]
                mov DSP_IRQ,ax            ;record IRQ line in use

                call reset_DSP

                ENDIF

                mov ax,default_pan
                mov panpot,ax
                mov ax,default_vol
                mov main_volume,ax
                call set_volume

                mov loop_cnt,0
                mov DAC_status,DAC_STOPPED
                mov buffer_mode,BUF_MODE

                mov buff_status[0*2],DAC_DONE
                mov buff_status[1*2],DAC_DONE

                mov init_OK,1

                mov ax,1
__exit_init:    POP_F
                ret
                ENDP

;****************************************************************************
index_VOC_blk   PROC H,File:FAR PTR,Block,SBuf:FAR PTR
                USES ds,si,di
                LOCAL x_status,x_pack:BYTE,x_tc:BYTE

                pushf
                cli
                cld

                mov x_status,0

                lds si,[File]
                mov ax,[si+14h]         ;get offset of data block
                ADD_PTR ax,0,ds,si

                mov bx,[Block]

__get_type:     mov al,[si]             ;get block type
                mov ah,0
                cmp ax,0                ;terminator block?
                je __exit               ;yes, return AX=0 (block not found)

                cmp ax,8                ;extended voice data?
                jne __chk_voice         ;no

                mov al,[si+5]           ;get extended voice parameters
                mov x_tc,al             ;high byte of TC = normal sample rate
                mov ax,[si+6]           ;get pack (AL) and mode (AH)
                cmp ah,1                ;stereo?
                jne __set_pack
                or al,80h               ;yes, make pack byte negative
__set_pack:     mov x_pack,al
                mov x_status,1          ;flag extended block override
                jmp __next_blk

__chk_voice:    cmp ax,1                ;voice data block?
                jne __chk_marker        ;no

                cmp bx,-1               ;marker found (or disregarded)?
                je __vblk_found         ;yes, use this voice data block
                jmp __next_blk          ;no, keep looking

__chk_marker:   cmp ax,4                ;marker block?
                jne __next_blk          ;no, keep looking

                cmp bx,[si+4]           ;yes, compare marker numbers
                jne __next_blk

                mov bx,-1               ;marker found, use next voice block

__next_blk:     inc si
                lodsw
                mov dl,[si]
                mov dh,0                ;blk len: AL=LSB, AH=KSB, DL=MSB, DH=0
                inc si

                ADD_PTR ax,dx,ds,si     ;point to next block
                jmp __get_type

__vblk_found:   les di,[SBuf]           ;get pointer to output structure

                mov bl,[si+4]           ;copy sampling rate
                mov al,[si+5]           ;copy packing type
                mov bh,0
                mov ah,0

                cmp x_status,0          ;previous extended block overrides
                je __use_vd             ;data block values
                mov al,x_pack
                mov bl,x_tc
                mov x_status,0

__use_vd:       mov es:[di].sample_rate,bx
                mov es:[di].pack_type,ax

                mov ax,[si+1]           ;copy voice data length
                mov dl,[si+3]
                mov dh,0
                sub ax,2
                sbb dx,0
                mov es:[di].len_l,ax
                mov es:[di].len_h,dx

                mov dx,ds               ;copy pointer to voice data
                mov ax,si
                ADD_PTR 6,0,dx,ax       
                mov WORD PTR es:[di].data,ax
                mov WORD PTR es:[di].data+2,dx

                mov ax,1

__exit:         POP_F
                ret
                ENDP

;****************************************************************************
register_sb     PROC H,BufNum,SBuf:FAR PTR
                USES ds,si,di

                pushf
                cli

                cmp buffer_mode,VOC_MODE        
                jne __get_bufnum        ;not in VOC mode, proceed
                call stop_d_pb C,0      ;else stop VOC file output first
                mov buffer_mode,BUF_MODE

__get_bufnum:   mov di,[BufNum]         ;get buffer #0-1
                shl di,1

                lds si,[SBuf]           ;copy structure data to buffer 
                mov ax,[si].pack_type   ;descriptor fields
                mov buff_pack[di],ax
                mov ax,[si].sample_rate
                mov buff_sample[di],ax

                les bx,[si].data
                mov buff_data_o[di],bx
                mov buff_data_s[di],es

                mov ax,[si].len_l
                mov buff_len_l[di],ax
                mov ax,[si].len_h
                mov buff_len_h[di],ax
                
                mov buff_status[di],DAC_STOPPED

__exit:         POP_F                  
                ret
                ENDP

;****************************************************************************
get_sb_status   PROC H,HBuffer
                USES ds,si,di

                pushf
                cli

                mov bx,[HBuffer]
                shl bx,1
                mov ax,buff_status[bx]

                POP_F
                ret
                ENDP

;****************************************************************************
play_VOC_file   PROC H,File:FAR PTR,Block
                LOCAL block_file:DWORD
                USES ds,si,di

                pushf
                cli

                mov xblk_status,0

                call stop_d_pb C,0      ;assert VOC mode
                mov buffer_mode,VOC_MODE

                les di,[File]
                mov WORD PTR block_file,di
                mov WORD PTR block_file+2,es

                mov DAC_status,DAC_DONE 
                      
                lds si,block_file
                mov ax,[si+14h]         ;get offset of data block
                ADD_PTR ax,0,ds,si
                mov WORD PTR block_ptr,si
                mov WORD PTR block_ptr+2,ds
                
                cmp [Block],-1          ;play 1st block if no marker specified
                je __do_it

__find_blk:     call block_type 
                cmp ax,0                ;terminator block?
                je __exit               ;yes, exit (block not found)
                call set_xblk
                call marker_num         ;get marker # (or -1 if non-marker)
                mov si,ax
                call next_block
                cmp si,[Block]
                jne __find_blk

__do_it:        mov DAC_status,DAC_STOPPED         
                                        ;return w/block_ptr -> 1st file block
__exit:         POP_F                       
                ret

                ENDP

;****************************************************************************
                IFDEF NEEDS_FORMAT

format_VOC_file PROC H,File:FAR PTR,Block
                LOCAL block_file:DWORD  ;leave interrupts enabled; this might
                LOCAL pack:BYTE         ;take awhile
                USES ds,si,di           
               
                mov pack,-1

                les di,[File]
                mov WORD PTR block_file,di
                mov WORD PTR block_file+2,es

                lds si,block_file
                mov ax,[si+14h]         ;get offset of data block
                ADD_PTR ax,0,ds,si
                mov WORD PTR block_ptr,si
                mov WORD PTR block_ptr+2,ds
                
                cmp [Block],-1          ;format 1st blk if no marker specified
                je __preform_blk

__form_find:    call block_type 
                cmp ax,0                ;terminator block?
                je __exit               ;yes, exit (block not found)
                call marker_num         ;get marker # (or -1 if non-marker)
                mov si,ax
                call next_block
                cmp si,[Block]
                jne __form_find
           
__preform_blk:  call block_type
                cmp ax,0
                je __exit
                cmp ax,1
                jne __not_vdata

                lds si,block_ptr
                mov al,[si+5]
                and al,0fh
                mov pack,al
                mov ax,[si+1]
                mov dx,[si+3]
                mov dh,0
                sub ax,2                ;voice data len = BLKLEN - 2
                sbb dx,0
                add si,6
                jmp __preform

__not_vdata:    cmp ax,2
                jne __preform_next

                lds si,block_ptr
                mov ax,[si+1]
                mov dx,[si+3]
                mov dh,0
                add si,4

__preform:      cmp pack,0              ;preformat only 8-bit PCM data
                jne __preform_next
                call format_block C,si,ds,ax,dx

__preform_next: call next_block
                jmp __preform_blk

__exit:         ret
                ENDP

;****************************************************************************
format_sb       PROC H,SBuf:FAR PTR
                USES ds,si,di           
                                        
                lds si,[SBuf]           

                mov ax,[si].pack_type
                and ax,0fh
                jnz __exit              ;format only 8-bit PCM data

                les bx,[si].data

                mov ax,[si].len_l
                mov dx,[si].len_h

                call format_block C,bx,es,ax,dx

__exit:         ret
                ENDP
                ENDIF

;****************************************************************************
start_d_pb      PROC H
                USES ds,si,di

                pushf
                cli

                cmp buffer_mode,VOC_MODE
                je __voc_mode           ;start Creative Voice File playback

                cmp DAC_status,DAC_PLAYING
                je __exit               ;bail out if already playing

                call next_buffer        ;start dual-buffer playback                
                cmp ax,-1
                je __exit               ;no buffers registered, exit
                mov DAC_status,DAC_PLAYING

                mov old_freq,-1
                mov old_stereo,-1

                call process_buffer C,ax
                jmp __exit

__voc_mode:     cmp DAC_status,DAC_STOPPED
                jne __exit
                
                mov old_freq,-1
                mov old_stereo,-1

                mov DAC_status,DAC_PLAYING
                call process_block

__exit:         POP_F
                ret
                ENDP

;****************************************************************************
stop_d_pb       PROC H
                USES ds,si,di

                pushf
                cli

                mov si,1
                cmp buffer_mode,VOC_MODE
                jne __stop_DMA
                cmp DAC_status,DAC_PLAYING      
                je __stop_DMA                   ;if in .VOC mode and no longer
                mov si,0                        ;playing, skip the IRQ delay

__stop_DMA:     mov DAC_status,DAC_STOPPED
                call IRQ_rest_vect              ;
                call halt_DMA C,si              ;(switched in V2.06)

                mov buff_status[0*2],DAC_DONE
                mov buff_status[1*2],DAC_DONE

                POP_F
                ret
                ENDP

;****************************************************************************
pause_d_pb      PROC H
                USES ds,si,di

                pushf
                cli

                cmp DAC_status,DAC_PLAYING
                jne __exit              ;(not playing)
                mov DAC_status,DAC_PAUSED

                IFDEF PAS
                mov dx,INTRCTLRST       ;disable IRQs on sample buffer empty
                out dx,al
                jmp $+2
                in al,dx
                mov dx,INTRCTLR
                in al,dx
                and al,11110111b        
                out dx,al
                ENDIF

                call halt_DMA C,1

__exit:         POP_F
                ret
                ENDP

;****************************************************************************
cont_d_pb       PROC H
                USES ds,si,di

                pushf
                cli

                cmp DAC_status,DAC_PAUSED
                jne __exit              ;(not paused)
                mov DAC_status,DAC_PLAYING

                IFDEF PAS
                mov dx,INTRCTLRST       ;enable IRQs on sample buffer empty
                out dx,al
                jmp $+2
                in al,dx
                mov dx,INTRCTLR
                in al,dx
                or al,00001000b         
                out dx,al
                ENDIF

                call continue_DMA

__exit:         POP_F
                ret
                ENDP

;****************************************************************************
get_VOC_status  PROC H
                USES ds,si,di

                pushf
                cli

                mov ax,DAC_status

                POP_F
                ret
                ENDP

;****************************************************************************
          	END
