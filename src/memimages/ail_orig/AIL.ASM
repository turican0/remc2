;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
;лл                                                                        лл
;лл   AIL.ASM                                                              лл
;лл                                                                        лл
;лл   IBM Audio Interface Library -- Application Program Interface module  лл
;лл                                                                        лл
;лл   Version 2.00 of 24-Sep-91: Initial V2.X version (derived from V1.02) лл
;лл           2.01 of 02-Dec-91: Round timer period down to reduce drift   лл
;лл           2.02 of 22-Dec-91: register_timer() stack usage adjusted     лл
;лл           2.10 of 07-Mar-92: TAS DIGPAK digital driver support added   лл
;лл           2.11 of 09-Mar-92: AIL_format_sound_buffer/format_VOC_file() лл
;лл                              added for Ad Lib Gold & DIGPAK compliance лл
;лл                              DIGPAK timer-sharing implemented          лл
;лл           2.12 of 12-Aug-92: Initialize timer_value in register_timer  лл
;лл           2.13 of 20-Aug-92: Add RTC INT 70H timer option              лл
;лл           2.14 of 24-Sep-92: Init DP_handle in register_driver()       лл
;лл                              Reset timer_value of first reg'd timer    лл
;лл                                                                        лл
;лл   8086 ASM source compatible with Turbo Assembler v2.0 or later        лл
;лл   C function prototypes in AIL.H                                       лл
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

                MODEL MEDIUM,C          ;Procedures far, data near by default
                LOCALS __               ;Enable local labels with __ prefix
                JUMPS                   ;Enable auto jump sizing

                ;
                ;Configuration equates
                ;

FALSE           equ 0
TRUE            equ -1

DIGPAK          equ TRUE                ;TRUE to assemble DIGPAK support code
DIGPAK_SHARE    equ FALSE               ;FALSE to disable DIGPAK timer sharing

USE_INT8        equ TRUE                ;FALSE to use real-time clock INT 70H
                                        ;(incompatible w/Windows & OS/2)

RTC_FREQ        equ 256                 ;RTC rate default = 256 Hz. 
RTC_FREQ_BITS   equ 1000b               ;(establishes maximum timer frequency)
                                        
                ;
                ;Macros, internal equates
                ;

                INCLUDE ail.inc         ;define driver procedure call numbers
                INCLUDE ail.mac         ;general-use macros

                .CODE

                ;
                ;Process services
                ;                

                PUBLIC AIL_startup
                PUBLIC AIL_shutdown
                PUBLIC AIL_register_timer
                PUBLIC AIL_set_timer_period
                PUBLIC AIL_set_timer_frequency
                PUBLIC AIL_set_timer_divisor
                PUBLIC AIL_interrupt_divisor
                PUBLIC AIL_start_timer
                PUBLIC AIL_start_all_timers
                PUBLIC AIL_stop_timer
                PUBLIC AIL_stop_all_timers
                PUBLIC AIL_release_timer_handle
                PUBLIC AIL_release_all_timers

                ;
                ;Installation services
                ;

                PUBLIC AIL_register_driver
                PUBLIC AIL_release_driver_handle
                PUBLIC AIL_describe_driver
                PUBLIC AIL_detect_device
                PUBLIC AIL_init_driver
                PUBLIC AIL_shutdown_driver
                
                ;
                ;Extended MIDI (XMIDI) performance services
                ;

                PUBLIC AIL_state_table_size
                PUBLIC AIL_register_sequence
                PUBLIC AIL_release_sequence_handle

                PUBLIC AIL_default_timbre_cache_size
                PUBLIC AIL_define_timbre_cache
                PUBLIC AIL_timbre_request
                PUBLIC AIL_install_timbre
                PUBLIC AIL_protect_timbre
                PUBLIC AIL_unprotect_timbre
                PUBLIC AIL_timbre_status

                PUBLIC AIL_start_sequence
                PUBLIC AIL_stop_sequence
                PUBLIC AIL_resume_sequence
                PUBLIC AIL_sequence_status
                PUBLIC AIL_relative_volume
                PUBLIC AIL_relative_tempo
                PUBLIC AIL_set_relative_volume
                PUBLIC AIL_set_relative_tempo
                PUBLIC AIL_beat_count
                PUBLIC AIL_measure_count
                PUBLIC AIL_branch_index

                PUBLIC AIL_controller_value
                PUBLIC AIL_set_controller_value
                PUBLIC AIL_channel_notes
                PUBLIC AIL_send_channel_voice_message
                PUBLIC AIL_send_sysex_message
                PUBLIC AIL_write_display
                PUBLIC AIL_install_callback
                PUBLIC AIL_cancel_callback

                PUBLIC AIL_lock_channel
                PUBLIC AIL_map_sequence_channel
                PUBLIC AIL_true_sequence_channel
                PUBLIC AIL_release_channel

                ;
                ;Digital performance services
                ;

                PUBLIC AIL_index_VOC_block
                PUBLIC AIL_register_sound_buffer
                PUBLIC AIL_format_sound_buffer
                PUBLIC AIL_sound_buffer_status
                PUBLIC AIL_play_VOC_file
                PUBLIC AIL_format_VOC_file
                PUBLIC AIL_VOC_playback_status
                PUBLIC AIL_start_digital_playback
                PUBLIC AIL_stop_digital_playback
                PUBLIC AIL_pause_digital_playback
                PUBLIC AIL_resume_digital_playback
                PUBLIC AIL_set_digital_playback_volume
                PUBLIC AIL_digital_playback_volume
                PUBLIC AIL_set_digital_playback_panpot
                PUBLIC AIL_digital_playback_panpot

                ;
                ;Local data
                ;
                
BIOS_H          equ 16                  ;Handle to BIOS default timer

active_timers   dw ?                    ;# of timers currently registered
timer_busy      dw 0                    ;Reentry flag for INT 8 handler

timer_callback  dd 17 dup (?)           ;Callback function addrs for timers
callback_ds     dw 17 dup (?)           ;Default data segments for callbacks
timer_status    dw 17 dup (?)           ;Status of timers (0=free 1=off 2=on)
timer_elapsed   dd 17 dup (?)           ;Modified DDA error counts for timers
timer_value     dd 17 dup (?)           ;Modified DDA limit values for timers
timer_period    dd ?                    ;Modified DDA increment for timers

bios_callback   dd ?
current_timer   dw ?                    
temp_period     dd ?
PIT_divisor     dw ?

index_base      dd 16 dup (?)           ;Driver table base addresses
assigned_timer  dw 16 dup (?)           ;Timers assigned to drivers
driver_active   dw 16 dup (?)

drvproc         dd ?
cur_drvr        dw ?
rtn_off         dw ?
rtn_seg         dw ?
timer_handle    dw ?

drvr_desc       STRUC
min_API_version dw ?
drvr_type       dw ?
data_suffix     db 4 dup (?)
dev_names       dd ?
def_IO          dw ?
def_IRQ         dw ?
def_DMA         dw ?
def_DRQ         dw ?
svc_rate        dw ?
dsp_size        dw ?
                ENDS

                ALIGN 2
stack_check     db 'Test'              ;(Used for stack overflow checking)
                dw 256 dup (?)         ;512-byte interrupt stack used by
intstack        LABEL WORD             ;default -- can be increased if needed

old_ss          dw ?
old_sp          dw ?

current_rev     dw 211

;*****************************************************************************
;*                                                                           *
;* RTC CMOS access                                                           *
;*                                                                           *
;*****************************************************************************

                IF NOT USE_INT8

PIC0_DATA       equ 020h
PIC0_CTRL       equ 021h
PIC1_DATA       equ 0a0h
PIC1_CTRL       equ 0a1h
EOI             equ 020h
ALARMINT        equ 04Ah
RTCINT          equ 070h
CMOS_CONTROL    equ 070h
CMOS_DATA       equ 071h
SRA             equ 00ah
SRB             equ 00bh
SRC             equ 00ch
SRD             equ 00dh

UIP             equ 080h

SET             equ 080h
PIE             equ 040h
AIE	equ 020h
UIE             equ 010h
SQWE	equ 008h
DM              equ 004h
AMPM	equ 002h
DSE             equ 001h

IRQF            equ 080h
PF              equ 040h
AF              equ 020h
UF              equ 010h

VRT             equ 080h

RTC_old_int     dd ?
RTC_active      dw 0

IO_WAIT         MACRO
                jmp $+2
                jmp $+2
                jmp $+2
                ENDM

;*****************************************************************************
read_CMOS       PROC Addr:BYTE
                pushf
                cli

                mov al,[Addr]
                out CMOS_CONTROL,al

                IO_WAIT

                in al,CMOS_DATA

                POP_F
                ret
                ENDP

;*****************************************************************************
write_CMOS      PROC Addr:BYTE,Val:BYTE
                pushf
                cli

                mov al,[Addr]
                out CMOS_CONTROL,al

                IO_WAIT

                mov al,[Val]
                out CMOS_DATA,al

                POP_F
                ret
                ENDP

;*****************************************************************************
RTC_set_freq    PROC Freq:WORD
                USES ds,si,di

                call read_CMOS,SRB
                and al,PIE
                jz __not_set

                mov ax,1
                jmp __out

__not_set:      call read_CMOS,SRA
                and al,0f0h
                mov bx,[Freq]
                and bx,0fh
                or bl,al
                call write_CMOS,SRA,bx

                mov ax,0
__out:          ret
                ENDP

;*****************************************************************************
RTC_enable      PROC
                USES ds,si,di

                call read_CMOS,SRB
	and al,(NOT (PIE AND (UF OR AF)))
                or al,(PIE AND PF)
	call write_CMOS,SRB,ax

                ret
                ENDP

;*****************************************************************************
RTC_disable     PROC                    ;Disable all RTC interrupts
                USES ds,si,di

                call read_CMOS,SRB
	and al,(NOT (PIE AND (UF OR PF OR AF)))
	call write_CMOS,SRB,ax

                ret
                ENDP

;*****************************************************************************
RTC_install     PROC
                USES ds,si,di

                pushf
                cli

                cmp RTC_active,0
                jne __exit

                mov ax,0             
                mov es,ax            
                les ax,es:[RTCINT*4]
                mov WORD PTR RTC_old_int+2,es
                mov WORD PTR RTC_old_int,ax

                mov ax,0             
                mov es,ax            
                lea ax,API_timer
                mov es:[(RTCINT*4)],ax
                mov es:[(RTCINT*4)+2],cs

                call read_CMOS,SRC

                in al,PIC1_CTRL
                and al,11111110b
                out PIC1_CTRL,al

                mov RTC_active,1

__exit:         POP_F
                ret
                ENDP

;*****************************************************************************
RTC_uninstall   PROC
                USES ds,si,di

                pushf
                cli

                cmp RTC_active,0
                je __exit

                in al,PIC1_CTRL
                or al,00000001b
                out PIC1_CTRL,al

                mov ax,0
                mov ds,ax
                les ax,RTC_old_int
                mov ds:[(RTCINT*4)],ax
                mov ds:[(RTCINT*4)+2],es

                mov RTC_active,0

__exit:         POP_F
                ret
                ENDP

                ENDIF

;*****************************************************************************
;*                                                                           *
;* Internal procedures                                                       *
;*                                                                           *
;*****************************************************************************

find_proc       PROC                    ;Return addr of function AX in driver
                                        ;BX (Note: reentrant!)
                cmp bx,16
                jae __bad_handle        ;exit sanely if handle invalid
                shl bx,1                ;(a legitimate action for apps)
                shl bx,1
                les bx,index_base[bx]   ;ES:BX -> driver procedure table
                mov cx,es
                or cx,bx
                jz __bad_handle         ;handle -> unreg'd driver, exit

__find_proc:    mov cx,es:[bx]          ;search for requested function in 
                cmp cx,ax               ;driver procedure table
                je __found
                add bx,4
                cmp cx,-1               
                jne __find_proc

                mov ax,0                ;return 0: function not available
                mov dx,0
                ret

__bad_handle:   mov ax,0                ;return 0: invalid driver handle
                mov dx,0
                ret                                                     

__found:        mov ax,es:[bx+2]        ;get offset from start of driver
                mov dx,es               ;get segment of driver (org = 0)
                ret

                ENDP

;*****************************************************************************
call_driver     PROC                    ;Call function AX in specified driver
                                        ;(Warning: re-entrant procedure!)
                mov bx,sp
                mov bx,ss:[bx+4]        ;get handle

                call find_proc

                cmp ax,0
                jne __do_call
                cmp dx,0
                je __invalid_call

__do_call:      push dx                 ;call driver function via stack 
                push ax
                retf                    

__invalid_call: ret                     ;return DX:AX = 0 if call failed

                ENDP

;*****************************************************************************
API_timer       PROC                    ;API INT 8/INT 70H dispatcher

                inc timer_busy

                cld
                push ax
                push bx
                push cx
                push dx
                push si
                push di
                push bp
                push es
                push ds

                IF NOT USE_INT8
                call read_CMOS C,SRC
                sti
                ENDIF

                cmp timer_busy,1
                jne __exit

                mov old_ss,ss
                mov old_sp,sp

                mov ax,cs               ;switch to internal stack if INT 8
                mov ss,ax               ;in use
                lea sp,intstack

                mov current_timer,0
__for_timer:    mov si,current_timer    ;for timer = 0 to 16
                shl si,1
                cmp timer_status[si],2  ;is timer "running"?
                jne __next_timer        ;no, go on to the next one
                mov ds,callback_ds[si]  ;else load callback's data segment...
                shl si,1
                
                mov ax,WORD PTR timer_elapsed[si]
                mov dx,WORD PTR timer_elapsed[si]+2

                add ax,WORD PTR timer_period
                adc dx,WORD PTR timer_period+2

                cmp dx,WORD PTR timer_value[si]+2
                jb __dec_timer
                ja __timer_tick
                cmp ax,WORD PTR timer_value[si]
                jae __timer_tick

__dec_timer:    mov WORD PTR timer_elapsed[si],ax
                mov WORD PTR timer_elapsed[si]+2,dx
                jmp __next_timer

__timer_tick:   sub ax,WORD PTR timer_value[si]
                sbb dx,WORD PTR timer_value[si]+2

                mov WORD PTR timer_elapsed[si],ax
                mov WORD PTR timer_elapsed[si]+2,dx

                call timer_callback[si] ;DDA timer expired, call timer proc

__next_timer:   inc current_timer       ;(may be externally set to -1 to 
                cmp current_timer,16    ; cancel further callbacks)
                jbe __for_timer

                mov ss,old_ss
                mov sp,old_sp

__exit:         pop ds
                pop es
                pop bp
                pop di
                pop si
                pop dx
                pop cx
                pop bx

                mov al,20h       
                IF NOT USE_INT8
                out 0a0h,al
                ENDIF
                out 20h,al

                pop ax

                cmp WORD PTR stack_check,'eT'
                jne __stack_fault
                cmp WORD PTR stack_check+2,'ts'
                jne __stack_fault

                dec timer_busy
                iret             

__stack_fault:  sti                     ;(Increase size of internal stack if
                int 3                   ;application breaks or "hangs" here)
                jmp __stack_fault       

                ENDP

;*****************************************************************************
init_DDA_arrays PROC                    ;Initialize timer DDA counters
                USES ds,si,di

                pushf
                cli

                cld
                mov WORD PTR timer_period,-1
                mov WORD PTR timer_period+2,-1
                
                push cs
                pop es
                mov di,OFFSET timer_status
                mov cx,17
                mov ax,0
                rep stosw               ;mark all timer handles "free"

                mov di,OFFSET timer_elapsed
                mov cx,17*2
                rep stosw
                                                              
                mov di,OFFSET timer_value
                mov cx,17*2
                rep stosw

                POP_F
                ret
                ENDP

;*****************************************************************************
bios_caller     PROC                    ;Call old INT8 handler to maintain RTC

                IF USE_INT8
                pushf
                call DWORD PTR bios_callback
                ENDIF

                ret
                ENDP

;*****************************************************************************
hook_timer_process PROC                 ;Take over default BIOS INT 8 handler
                USES ds,si,di

                pushf
                cli
                
                mov ax,0                ;get current INT 8 vector and save it
                mov es,ax               ;as reserved timer function (stopped)
                mov bx,es:[(8*4)]
                mov es,es:[(8*4)+2]

                mov WORD PTR bios_callback,bx
                mov WORD PTR bios_callback+2,es

                mov bx,OFFSET bios_caller
                mov WORD PTR timer_callback[BIOS_H*4],bx
                mov WORD PTR timer_callback[BIOS_H*4]+2,cs

                IF USE_INT8

                mov ax,cs
                mov ds,ax
                mov dx,OFFSET API_timer

                mov ax,0                ;replace default handler with API task
                mov es,ax               ;manager
                mov es:[(8*4)],dx
                mov es:[(8*4)+2],ds

                ELSE

                call RTC_set_freq C,RTC_FREQ_BITS
                call RTC_enable
                call RTC_install

                ENDIF

                POP_F
                ret
                ENDP

;*****************************************************************************
unhook_timer_process PROC               ;Restore default BIOS INT 8 handler
                USES ds,si,di

                pushf
                cli

                mov current_timer,-1    ;disallow any further callbacks

                IF USE_INT8

                mov dx,WORD PTR bios_callback
                mov ds,WORD PTR bios_callback+2

                mov ax,0             
                mov es,ax            
                mov es:[(8*4)],dx
                mov es:[(8*4)+2],ds

                ELSE

                call RTC_uninstall
                call RTC_disable

                ENDIF

                POP_F
                ret
                ENDP

                IF USE_INT8
;*****************************************************************************
set_PIT_divisor PROC Divisor            ;Set 8253 Programmable Interval Timer
                USES ds,si,di           ;to desired IRQ 0 (INT 8) interval

                pushf
                cli

                mov al,36h
                out 43h,al
                mov ax,[Divisor]        ;PIT granularity = 1/1193181 sec.
                mov PIT_divisor,ax
                jmp $+2
                out 40h,al
                mov al,ah
                jmp $+2
                out 40h,al

                POP_F
                ret
                ENDP

;*****************************************************************************
set_PIT_period  PROC Period             ;Set 8253 Programmable Interval Timer
                USES ds,si,di           ;to desired period in microseconds
                   
                mov ax,0                ;special case: no rounding error
                cmp [Period],54925      ;if period=55 msec. BIOS default value
                jae __set_PIT

                mov ax,[Period]
                mov bx,8380             ;PIT granularity = .83809532 uS
                mov cx,10000            ;round down to avoid cumulative error
                mul cx
                div bx

__set_PIT:      call set_PIT_divisor C,ax

                ret
                ENDP

                ENDIF

;*****************************************************************************
ul_divide       PROC Num:DWORD,Den:DWORD
                USES ds,si,di

                mov ax,WORD PTR [Num]
                mov dx,WORD PTR [Num]+2
                mov bx,WORD PTR [Den]
                mov cx,WORD PTR [Den]+2

                or cx,cx
                jne __long_div
                or dx,dx
                je __short_div
                or bx,bx
                je __short_div

__long_div:     mov bp,cx
                mov cx,20h
                xor di,di
                xor si,si
                
__div_loop:     shl ax,1
                rcl dx,1
                rcl si,1
                rcl di,1
                cmp di,bp
                jb __cont_loop
                ja __bump_quot
                cmp si,bx
                jb __cont_loop
__bump_quot:    sub si,bx
                sbb di,bp
                inc ax
__cont_loop:    loop __div_loop
                jmp __end_div

__short_div:    div bx
                xor dx,dx

__end_div:      ret

                ENDP

;*****************************************************************************
program_timers  PROC                    ;Establish timer interrupt rates
                USES ds,si,di           ;based on fastest active timer

                pushf
                cli                     ;non-reentrant, don't interrupt

                cld
                mov WORD PTR temp_period,-1
                mov WORD PTR temp_period+2,-1

                mov si,0
__for_timer:    mov bx,si               ;find fastest active timer....
                shl bx,1
                cmp timer_status[bx],0  ;timer active (registered)?
                je __next_timer         ;no, skip it
                shl bx,1
                mov ax,WORD PTR timer_value[bx]
                mov dx,WORD PTR timer_value[bx]+2

                cmp dx,WORD PTR temp_period+2
                jb __set_temp
                ja __next_timer
                cmp ax,WORD PTR temp_period
                jae __next_timer

__set_temp:     mov WORD PTR temp_period,ax
                mov WORD PTR temp_period+2,dx

__next_timer:   inc si
                cmp si,16               ;(include BIOS reserved timer)
                jbe __for_timer        

                mov ax,WORD PTR temp_period
                mov dx,WORD PTR temp_period+2

                cmp ax,WORD PTR timer_period
                jne __must_reset
                cmp dx,WORD PTR timer_period+2
                je __no_change          ;current rate hasn't changed, exit

__must_reset:   mov current_timer,-1    ;else set new base timer rate
                                        ;(slowest possible base = 54 msec!)
                mov WORD PTR timer_period,ax
                mov WORD PTR timer_period+2,dx  

                IF USE_INT8
                call set_PIT_period C,ax
                ENDIF

                push cs
                pop es
                mov di,OFFSET timer_elapsed
                mov cx,17*2
                mov ax,0                ;reset ALL elapsed counters to 0 uS
                rep stosw

__no_change:    
                POP_F
                ret
                ENDP

;*****************************************************************************
;*                                                                           *
;* Process services                                                          *
;*                                                                           *
;*****************************************************************************

AIL_startup     PROC                    ;Initialize AIL API
                USES ds,si,di

                pushf
                cli

                mov active_timers,0     ;# of registered timers
                mov timer_busy,0        ;timer re-entrancy protection

                IF DIGPAK
                mov DP_registered,0
                ENDIF

                cld
                mov ax,cs
                mov es,ax
                lea di,index_base
                mov cx,16*2
                mov ax,0
                rep stosw
                lea di,assigned_timer
                mov cx,16
                mov ax,-1
                rep stosw
                lea di,driver_active
                mov cx,16
                mov ax,0
                rep stosw

                POP_F    
                ret
                ENDP

;*****************************************************************************
AIL_shutdown    PROC SignOff:FAR PTR    ;Quick shutdown of all AIL resources
                USES ds,si,di

                pushf
                cli

                mov cur_drvr,0
__for_slot:     mov si,cur_drvr
                shl si,1
                mov dx,assigned_timer[si]
                shl si,1 
                mov ax,WORD PTR index_base[si]   
                or ax,WORD PTR index_base[si+2]
                jz __next_slot          ;no driver installed, skip slot

                cmp dx,-1
                je __shut_down          ;no timer assigned, continue
                call AIL_release_timer_handle C,dx

__shut_down:    call AIL_shutdown_driver C,cur_drvr,[SignOff]

__next_slot:    inc cur_drvr
                cmp cur_drvr,16
                jne __for_slot

                call AIL_release_all_timers

                POP_F
                ret
                ENDP

;*****************************************************************************
AIL_register_timer PROC Callback:FAR PTR        
                USES ds,si,di

                pushf
                cli

                mov cx,ds               ;save application module's DS segment

                mov bx,0                ;look for a free timer handle....
__find_free:    cmp timer_status[bx],0
                je __found              ;found one
                add bx,2
                cmp bx,32
                jb __find_free
                mov ax,-1
                jmp __return            ;no free timers, return -1

__found:        mov ax,bx               ;yes, set up to return handle
                shr ax,1
                mov timer_status[bx],1  ;turn the new timer "off" (stopped)
                mov callback_ds[bx],cx  ;register timer proc's DS segment
                shl bx,1
                lds si,[Callback]       ;register the timer proc
                mov WORD PTR timer_callback[bx],si
                mov WORD PTR timer_callback+2[bx],ds
                mov WORD PTR timer_value[bx],-1
                mov WORD PTR timer_value[bx]+2,-1
                inc active_timers
                cmp active_timers,1     ;is this the first timer registered?
                jne __return            ;no, just return

                push ax                 ;yes, set up our own interrupt handler

                call init_DDA_arrays    ;init timer countdown values
                mov timer_status[BIOS_H*2],1
                call hook_timer_process ;seize interrupt and register BIOS handler

                IF USE_INT8
                call AIL_set_timer_period C,BIOS_H,54925,0
                call AIL_start_timer C,BIOS_H
                ELSE
                call AIL_set_timer_frequency C,BIOS_H,RTC_FREQ,0
                ENDIF

                pop ax

                mov bx,ax
                shl bx,1
                mov timer_status[bx],1  ;(cleared by init_DDA_arrays)
                shl bx,1
                mov WORD PTR timer_value[bx],-1
                mov WORD PTR timer_value[bx]+2,-1

__return:                               
                POP_F
                ret
                ENDP

;*****************************************************************************
AIL_release_timer_handle PROC Timer
                USES ds,si,di

                pushf
                cli

                mov bx,[Timer]
                cmp bx,-1
                je __return

                shl bx,1
                cmp timer_status[bx],0  ;is the specified timer active?
                je __return             ;no, exit

                mov timer_status[bx],0  ;release the timer's handle
                
                dec active_timers       ;any active timers left?
                jnz __return            ;if not, put the default handler back

                IF USE_INT8
                call set_PIT_divisor C,0
                ENDIF

                call unhook_timer_process             
__return:       
                POP_F
                ret
                ENDP

;*****************************************************************************
AIL_release_all_timers PROC
                USES ds,si,di

                pushf
                cli

                mov si,15               ;free all external timer handles
__release_it:   call AIL_release_timer_handle C,si
                dec si
                jge __release_it

                POP_F
                ret
                ENDP

;*****************************************************************************
AIL_start_timer PROC Timer
                USES ds,si,di

                pushf
                cli

                mov bx,[Timer]
                shl bx,1
                cmp timer_status[bx],1  ;is the specified timer stopped?
                jne __return
                mov timer_status[bx],2  ;yes, start it
__return:       
                POP_F
                ret
                ENDP

;*****************************************************************************
AIL_start_all_timers PROC
                USES ds,si,di

                pushf 
                cli

                mov si,15               ;start all stopped timers
__start_it:     call AIL_start_timer C,si
                dec si
                jge __start_it

                POP_F
                ret
                ENDP

;*****************************************************************************
AIL_stop_timer  PROC Timer
                USES ds,si,di

                pushf
                cli

                mov bx,[Timer]
                shl bx,1
                cmp timer_status[bx],2  ;is the specified timer running?
                jne __return
                mov timer_status[bx],1  ;yes, stop it
__return:       
                POP_F
                ret
                ENDP

;*****************************************************************************
AIL_stop_all_timers PROC
                USES ds,si,di

                pushf
                cli

                mov si,15               ;stop all running timers
__stop_it:      call AIL_stop_timer C,si
                dec si
                jge __stop_it

                POP_F
                ret
                ENDP

;*****************************************************************************
AIL_set_timer_period PROC Timer,uS:DWORD
                USES ds,si,di           ;accepts timer period in microseconds

                pushf
                cli

                mov bx,[Timer]
                shl bx,1
                mov ax,timer_status[bx] ;save timer's status
                push ax
                mov timer_status[bx],1  ;stop timer while calculating...

                shl bx,1
                mov ax,WORD PTR [uS]
                mov dx,WORD PTR [uS]+2
                mov WORD PTR timer_value[bx],ax
                mov WORD PTR timer_value[bx]+2,dx

                mov WORD PTR timer_elapsed[bx],0
                mov WORD PTR timer_elapsed[bx]+2,0

                call program_timers     ;reset base interrupt rate if needed

                pop ax
                mov bx,[Timer]
                shl bx,1
                mov timer_status[bx],ax ;restore timer's former status

                POP_F
                ret
                ENDP

;*****************************************************************************
AIL_set_timer_frequency PROC Timer,Hz:DWORD
                USES ds,si,di           ;accepts timer frequency in Hertz

                pushf
                cli

                call ul_divide C,4240h,000fh,[Hz]
                call AIL_set_timer_period C,[Timer],ax,dx

                POP_F
                ret
                ENDP                                               

;*****************************************************************************
AIL_set_timer_divisor PROC Timer,PIT
                USES ds,si,di           ;accepts PIT register values directly

                pushf
                cli

                cmp [PIT],0             ;special case: 0 wraps to 65536
                jne __nonzero
                mov ax,54925      
                mov dx,0
                jmp __set_AXDX

__nonzero:      mov ax,10000            ;convert to microseconds
                mov bx,11932
                mul [PIT]
                div bx                  ;(accurate to ё.01%)
                mov dx,0                ;(fixes bug in v1.00 release)

__set_AXDX:     call AIL_set_timer_period C,[Timer],ax,dx

                POP_F
                ret
                ENDP

;*****************************************************************************
AIL_interrupt_divisor PROC              ;Get value last used by the API to 
                USES ds,si,di           ;program the PIT chip

                pushf
                cli

                mov ax,PIT_divisor

                POP_F
                ret
                ENDP

;*****************************************************************************
;*                                                                           *
;* Installation services                                                     *
;*                                                                           *
;*****************************************************************************

AIL_register_driver PROC Addr:FAR PTR
                USES ds,si,di

                pushf
                cli
                
                mov cur_drvr,0
__find_handle:  mov si,cur_drvr
                shl si,1
                shl si,1
                mov ax,WORD PTR index_base[si]
                or ax,WORD PTR index_base[si+2]
                je __found
                inc cur_drvr
                cmp cur_drvr,16
                jne __find_handle
                mov ax,-1               ;return -1 if no free handles
                jmp __return

__found:        les di,[Addr]           ;get driver base address

                IF DIGPAK
                cmp es:[di+3],'ID'
                jne __check_ADV         ;check for 'DIGPAK' ID string
                cmp es:[di+5],'PG'
                jne __check_ADV
                cmp es:[di+7],'KA'
                je __register_DP        ;if it's a DIGPAK driver, register the
                ENDIF                   ;Integrated Driver Interface instead

__check_ADV:    mov ax,-1               ;else check for copyright string to
                cmp es:[di+2],'oC'      ;avoid calling non-AIL drivers
                jne __return
                cmp es:[di+4],'yp'
                jne __return

                add di,es:[di]          ;skip copyright message text
                mov WORD PTR index_base[si],di
                mov WORD PTR index_base[si+2],es

                call AIL_describe_driver C,cur_drvr

                mov es,dx               ;check API version compatibility
                mov di,ax
                or dx,ax
                mov ax,-1
                je __return             ;return -1 if description call failed

                mov dx,es:[di].min_API_version
                cmp dx,current_rev
                ja __return             ;return -1 if API out of date

__valid_handle: mov ax,cur_drvr         ;else return AX=new driver handle

__return:       POP_F
                ret

                IF DIGPAK
__register_DP:  mov ax,-1               ;Register a DIGPAK driver from The
                cmp DP_registered,0     ;Audio Solution, Inc.
                jne __return
                mov DP_registered,1     ;(multiple DP drivers not supported)

                mov ax,cur_drvr         
                mov DP_handle,ax

                mov ax,es
                sub ax,10h              ;get entry point for .COM driver
                add di,100h             ;(ES:0 -> AX-10H:100h)

                mov WORD PTR DP_base+2,ax 
                mov WORD PTR DP_base,di   

                mov WORD PTR index_base[si],OFFSET DP_IDI_index
                mov WORD PTR index_base[si+2],cs

                mov DP_installed,0      ;flag driver not yet installed
                jmp __valid_handle      ;return valid handle to DIGPAK IDI
                ENDIF

                ENDP

;*****************************************************************************
AIL_release_driver_handle PROC H
                USES ds,si,di

                pushf
                cli

                mov bx,[H]
                cmp bx,16
                jae __exit              ;exit cleanly if invalid handle passed
                shl bx,1
                shl bx,1
                mov WORD PTR index_base[bx],0
                mov WORD PTR index_base[bx+2],0

                IF DIGPAK               
                cmp DP_registered,0     ;Release DIGPAK driver handle
                je __exit
                mov ax,[H]
                cmp ax,DP_handle
                jne __exit
                mov DP_registered,0
                ENDIF
        
__exit:         POP_F
                ret
                ENDP

;*****************************************************************************
AIL_describe_driver PROC HDrvr

                push SEG AIL_interrupt_divisor
                push OFFSET AIL_interrupt_divisor
                push [HDrvr]
                mov ax,AIL_DESC_DRVR
                call call_driver
                add sp,6
                ret
                ENDP

;*****************************************************************************
AIL_detect_device PROC

                mov ax,AIL_DET_DEV
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_init_driver PROC HDrvr,IO,IRQ,DMA,DRQ
                USES ds,si,di

                pushf
                cli

                cmp [HDrvr],16
                jae __return            ;exit cleanly if invalid handle passed

                mov timer_handle,-1

                call AIL_describe_driver C,[HDrvr]

                mov es,dx
                mov di,ax
                mov si,es:[di].svc_rate ;get desired service rate
                cmp si,-1
                je __do_init            ;(no timer service requested)

                mov ax,AIL_SERVE_DRVR 
                mov bx,[HDrvr]
                call find_proc

                mov bx,ax
                or bx,dx
                jz __do_init            ;(no driver service proc)

                mov es,dx
                mov bx,ax               ;ES:BX = serve_driver() address

                call AIL_register_timer C,bx,es
                mov bx,[HDrvr]
                shl bx,1           
                mov assigned_timer[bx],ax
                mov timer_handle,ax

                call AIL_set_timer_frequency C,ax,si,0

__do_init:      push DRQ
                push DMA
                push IRQ
                push IO
                push HDrvr
                mov ax,AIL_INIT_DRVR
                call call_driver
                add sp,10

                mov bx,[HDrvr]
                shl bx,1
                mov driver_active[bx],1

                cmp timer_handle,-1
                je __return
                call AIL_start_timer C,timer_handle

__return:       POP_F
                ret

                ENDP

;*****************************************************************************
AIL_shutdown_driver PROC

                mov bx,sp
                mov bx,ss:[bx+4]        ;get handle
                cmp bx,16
                jae __exit              ;exit cleanly if invalid handle passed

                shl bx,1
                mov dx,0
                xchg driver_active[bx],dx
                cmp dx,0
                je __exit               ;driver never initialized, exit
                mov dx,assigned_timer[bx]
                cmp dx,-1
                je __shut_down          ;no timer assigned, continue
                call AIL_release_timer_handle C,dx

__shut_down:    mov ax,AIL_SHUTDOWN_DRVR
                jmp call_driver

__exit:         ret
                ENDP

;*****************************************************************************
;*                                                                           *
;* Performance services                                                      *
;*                                                                           *
;*****************************************************************************

AIL_index_VOC_block PROC

                mov ax,AIL_INDEX_VOC_BLK  
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_register_sound_buffer PROC

                mov ax,AIL_REG_SND_BUFF  
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_format_sound_buffer PROC

                mov ax,AIL_F_SND_BUFF  
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_sound_buffer_status PROC

                mov ax,AIL_SND_BUFF_STAT 
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_play_VOC_file  PROC

                mov ax,AIL_P_VOC_FILE 
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_format_VOC_file PROC

                mov ax,AIL_F_VOC_FILE 
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_VOC_playback_status PROC

                mov ax,AIL_VOC_PB_STAT   
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_start_digital_playback PROC

                mov ax,AIL_START_D_PB    
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_stop_digital_playback PROC

                mov ax,AIL_STOP_D_PB     
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_pause_digital_playback PROC

                mov ax,AIL_PAUSE_D_PB    
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_resume_digital_playback PROC

                mov ax,AIL_RESUME_D_PB   
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_set_digital_playback_volume PROC

                mov ax,AIL_SET_D_PB_VOL  
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_digital_playback_volume PROC

                mov ax,AIL_D_PB_VOL      
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_set_digital_playback_panpot PROC

                mov ax,AIL_SET_D_PB_PAN  
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_digital_playback_panpot PROC

                mov ax,AIL_D_PB_PAN      
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_state_table_size PROC

                mov ax,AIL_STATE_TAB_SIZE
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_register_sequence PROC

                mov ax,AIL_REG_SEQ       
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_release_sequence_handle PROC

                mov ax,AIL_REL_SEQ_HND   
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_default_timbre_cache_size PROC

                mov ax,AIL_T_CACHE_SIZE  
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_define_timbre_cache PROC

                mov ax,AIL_DEFINE_T_CACHE
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_timbre_request PROC

                mov ax,AIL_T_REQ         
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_install_timbre PROC

                mov ax,AIL_INSTALL_T     
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_protect_timbre PROC

                mov ax,AIL_PROTECT_T     
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_unprotect_timbre PROC

                mov ax,AIL_UNPROTECT_T   
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_timbre_status  PROC

                mov ax,AIL_T_STATUS
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_start_sequence PROC

                mov ax,AIL_START_SEQ     
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_stop_sequence  PROC

                mov ax,AIL_STOP_SEQ      
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_resume_sequence PROC

                mov ax,AIL_RESUME_SEQ    
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_sequence_status PROC

                mov ax,AIL_SEQ_STAT      
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_relative_volume PROC

                mov ax,AIL_REL_VOL       
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_relative_tempo PROC

                mov ax,AIL_REL_TEMPO
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_set_relative_volume PROC

                mov ax,AIL_SET_REL_VOL       
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_set_relative_tempo PROC

                mov ax,AIL_SET_REL_TEMPO  
                jmp call_driver
                        
                ENDP

;*****************************************************************************
AIL_beat_count     PROC

                mov ax,AIL_BEAT_CNT      
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_measure_count  PROC

                mov ax,AIL_BAR_CNT       
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_branch_index   PROC

                mov ax,AIL_BRA_INDEX  
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_controller_value PROC

                mov ax,AIL_CON_VAL       
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_set_controller_value PROC

                mov ax,AIL_SET_CON_VAL   
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_channel_notes  PROC

                mov ax,AIL_CHAN_NOTES    
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_send_channel_voice_message PROC

                mov ax,AIL_SEND_CV_MSG   
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_send_sysex_message PROC

                mov ax,AIL_SEND_SYSEX_MSG
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_write_display  PROC

                mov ax,AIL_WRITE_DISP    
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_install_callback PROC

                mov ax,AIL_INSTALL_CB    
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_cancel_callback PROC

                mov ax,AIL_CANCEL_CB     
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_lock_channel   PROC

                mov ax,AIL_LOCK_CHAN     
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_map_sequence_channel PROC

                mov ax,AIL_MAP_SEQ_CHAN  
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_release_channel PROC

                mov ax,AIL_RELEASE_CHAN  
                jmp call_driver

                ENDP

;*****************************************************************************
AIL_true_sequence_channel PROC

                mov ax,AIL_TRUE_SEQ_CHAN  
                jmp call_driver

                ENDP

;*****************************************************************************
;*                                                                           *
;* DIGPAK Integrated Driver Interface                                        *
;*                                                                           *
;* Simulated AIL driver shell allows access to The Audio Solution's DIGPAK   *
;* driver loaded as .COM file                                                *
;*                                                                           *
;*****************************************************************************

                IF DIGPAK

DP_IDI_index:   dw AIL_DESC_DRVR,OFFSET DP_describe_driver 
                dw AIL_DET_DEV,OFFSET DP_detect_device   
                dw AIL_INIT_DRVR,OFFSET DP_init_driver     
                dw AIL_SHUTDOWN_DRVR,OFFSET DP_shutdown_driver 
                dw AIL_P_VOC_FILE,OFFSET DP_play_VOC_file       
                dw AIL_START_D_PB,OFFSET DP_start_d_pb      
                dw AIL_STOP_D_PB,OFFSET DP_stop_d_pb       
                dw AIL_VOC_PB_STAT,OFFSET DP_get_VOC_status
                dw AIL_INDEX_VOC_BLK,OFFSET DP_index_VOC_blk
                dw AIL_REG_SND_BUFF,OFFSET DP_register_sb
                dw AIL_SND_BUFF_STAT,OFFSET DP_get_sb_status
                dw AIL_F_VOC_FILE,OFFSET DP_format_VOC_file
                dw AIL_F_SND_BUFF,OFFSET DP_format_sb
                dw -1

BKGND_PLAY      equ 00000001b           ;Function 068CH attributes
MODIFIES_DATA   equ 00000010b
DOWNSAMPLES     equ 00000100b
USES_TICK       equ 00001000b
SHARES_TIMER    equ 00010000b

BLK_SIZE        equ 32768               ;max. block size for INT 66 calls

DP_DDT          LABEL WORD              ;Simulated AIL device description
                dw 210                  ;Minimum API version required = 2.10
                dw 2                    ;Type 2: Sound Blaster DSP emulation
                db 'VOC',0              ;Supports .VOC files directly
                dw OFFSET DP_name       ;Pointer to list of supported devices
DP_devname_s    dw ?
                dw -1                   ;TAS drivers responsible for handling
                dw -1                   ;I/O parms
                dw -1
                dw -1
DP_uses_INT8    dw ?                    ;0 if driver stops music; -1 if OK
                dw 0                    ;No display

DP_name         db 80 dup (?)

DP_call         LABEL DWORD
call_off        dw ?
call_seg        dw ?

DP_handle       dw ?
DP_registered   dw ?
DP_installed    dw ?
DP_base         dd ?
DP_attribs      dw ?

VOC_MODE        equ 0                   ;Creative Voice File playback mode
BUF_MODE        equ 1                   ;Dual-buffer DMA playback mode

sbuffer         STRUC                   ;AIL sound buffer structure
pack_type       dw ?
sample_rate     dw ?
data            dd ?
len_l           dw ?
len_h           dw ?
                ENDS

DP_sndstruc     STRUC                   ;DIGPAK sound sample structure
sndptr          dd ?
sndlen          dw ?
sndstat         dd ?
sndfreq         dw ?
                ENDS

DPS             DP_sndstruc <>
DPT             DP_sndstruc <>

buff_data_o     dw 2 dup (?)
buff_data_s     dw 2 dup (?)
buff_len_l      dw 2 dup (?)
buff_len_h      dw 2 dup (?)
buff_sample     dw 2 dup (?)
buff_status     dw 2 dup (?)

buffer_mode     dw ?                   
DAC_status      dw ?
current_buffer  dw ?
block_ptr       dd ?
loop_ptr        dd ?
loop_cnt        dw ?
DMA_ptr         dd ?
DMA_len_l       dw ?
DMA_len_h       dw ?
packing         dw ?
playing_flag    dw ?

DP_timer        dw ?
AIL_hooked      dw ?

;****************************************************************************
;*                                                                          *
;*  DP IDI internal procedures                                              *
;*                                                                          *
;****************************************************************************

AIL_unhook      PROC
                USES ds,si,di

                IF USE_INT8

                test DP_attribs,USES_TICK
                jz __exit

                cmp active_timers,0
                je __exit

                cmp AIL_hooked,0
                je __exit

                IF DIGPAK_SHARE
                test DP_attribs,SHARES_TIMER
                jnz __share
                ENDIF

                call unhook_timer_process

                mov AIL_hooked,0
                jmp __exit

__share:        mov ax,693h             ;function #12: ShareTimer
                mov dx,PIT_divisor     
                int 66h

                ENDIF

__exit:         ret
                ENDP

;****************************************************************************
AIL_hook        PROC
                USES ds,si,di

                IF USE_INT8

                test DP_attribs,USES_TICK
                jz __exit

                IF DIGPAK_SHARE
                test DP_attribs,SHARES_TIMER
                jnz __exit
                ENDIF

                cmp active_timers,0
                je __exit

                cmp AIL_hooked,0
                jne __exit

                call AIL_interrupt_divisor
                call set_PIT_divisor C,ax

                call hook_timer_process
                mov AIL_hooked,1

                ENDIF

__exit:         ret
                ENDP

;****************************************************************************
sub_ptr         PROC Off1,Seg1,Off2,Seg2        
                USES ds,si,di           ;Return DX:AX = ptr 2 - ptr 1

                mov ax,[Seg2]
                mov dx,0
                REPT 4
                shl ax,1
                rcl dx,1
                ENDM
                add ax,[Off2]
                adc dx,0

                mov bx,[Seg1]
                mov cx,0
                REPT 4
                shl bx,1
                rcl cx,1
                ENDM
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
get_sample_rate PROC SB_Rate            ;Establish DSP sample rate
                USES ds,si,di           

                mov bx,[SB_Rate]
                mov bh,0

                mov ax,256              ;f in Hz. = 1E6 / (256 - SB_Rate)
                sub ax,bx
                call ul_divide C,4240h,0fh,ax,0

                ret                     ;return DX:AX=sample rate in hertz
                ENDP

;****************************************************************************
DP_preformat    PROC SampRate,BufPtr:DWORD,Len:DWORD
                USES ds,si,di

                test DP_attribs,MODIFIES_DATA
                jz __exit               ;preformatting unnecessary, exit

                mov WORD PTR DPT.sndstat,OFFSET playing_flag
                mov WORD PTR DPT.sndstat+2,cs

                mov ax,[SampRate]
                mov DPT.sndfreq,ax

                lds si,[BufPtr]

__while_gt_BLK: mov WORD PTR DPT.sndptr,si
                mov WORD PTR DPT.sndptr+2,ds

                cmp WORD PTR [Len+2],0
                ja __gt_BLK
                cmp WORD PTR [Len],BLK_SIZE
                jbe __remainder
                   
__gt_BLK:       mov DPT.sndlen,BLK_SIZE

                push ds
                push si
                push cs
                pop ds
                lea si,DPT
                mov ax,68ah             ;function #3: MassageAudio
                int 66h
                pop si
                pop ds

                ADD_PTR BLK_SIZE,0,ds,si

                sub WORD PTR [Len],BLK_SIZE
                sbb WORD PTR [Len+2],0
                jmp __while_gt_BLK

__remainder:    mov cx,WORD PTR [Len]
                jcxz __exit
                
                mov DPT.sndlen,cx

                push cs
                pop ds
                lea si,DPT
                mov ax,68ah
                int 66h

__exit:         ret
                ENDP
                                        
;****************************************************************************
CB_set_vect     PROC Handler:FAR PTR    ;Install simulated DMA IRQ handler
                USES ds,si,di

                mov ax,cs
                mov ds,ax
                mov bx,WORD PTR [Handler]
                mov dx,WORD PTR [Handler+2]
                mov ax,68eh
                int 66h                 ;DP function 7: SetCallBack Address

__exit:         ret
                ENDP

;****************************************************************************
CB_play_VOC     PROC                    ;Callback handler for .VOC file output
                     
                cld

                call AIL_hook           ;restore AIL service

                mov ax,DMA_len_l        ;at end of block?
                or ax,DMA_len_h
                jz __end_of_block

                call xfer_chunk         ;no, send next chunk
                jmp __exit

__end_of_block: call next_block         ;else go on to next block in chain
                call process_block

__exit:         ret
                ENDP

;****************************************************************************
CB_play_buffer  PROC                    ;Callback handler for double-buffering
                     
                cld

                call AIL_hook           ;restore AIL service

                mov ax,DMA_len_l        ;at end of block?
                or ax,DMA_len_h
                jz __end_of_block
                     
                call xfer_chunk         ;no, send next chunk
                jmp __exit

__end_of_block: mov bx,current_buffer   ;else look for an unplayed buffer...
                mov buff_status[bx],DAC_DONE

                call next_buffer
                cmp ax,-1
                je __exit               ;no buffers left, terminate playback

                call process_buffer C,ax

__exit:         ret
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
process_block   PROC                    ;Process current block in voice data
                USES ds,si,di           ;(May be called from IRQ handler)

__do_block:     call block_type
                cmp ax,0                ;terminator?
                je __terminate
                cmp ax,1                ;new voice block?
                je __new_voice
                cmp ax,2                ;continued voice block?
                je __cont_voice
                cmp ax,4                ;marker (end of data?)
                je __terminate
                cmp ax,6                ;beginning of repeat loop?
                je __rept_loop
                cmp ax,7                ;end of repeat loop?
                je __end_loop
                jmp __skip_block        ;else unrecognized block type, skip it

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

__cont_voice:   lds si,block_ptr       ;continue output from new voice block
                lea ax,CB_play_VOC     ;enable EOD interrupts from DSP
                call CB_set_vect C,ax,cs
                mov ax,[si+1]
                mov dl,[si+3]
                mov dh,0               ;DX:AX = voice len
                ADD_PTR 4,0,ds,si      ;DS:SI -> start-of-data
                call DMA_transfer C,si,ds,ax,dx
                jmp __exit

__new_voice:    lds si,block_ptr       ;initiate output from new voice block 

                call get_sample_rate C,[si+4]
                mov DPS.sndfreq,ax     

                lea ax,CB_play_VOC     ;enable EOD interrupts from DSP
                call CB_set_vect C,ax,cs
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
process_buffer  PROC Buf                ;Play specified buffer
                USES ds,si,di

                mov si,[Buf]            ;get buffer handle 
                shl si,1                ;derive index

                mov buff_status[si],DAC_PLAYING
                mov current_buffer,si   ;save index to playing buffer

                call get_sample_rate C,buff_sample[si]
                mov DPS.sndfreq,ax

                lea ax,CB_play_buffer  ;enable EOD interrupts from DSP
                call CB_set_vect C,ax,cs

                call DMA_transfer C,buff_data_o[si],buff_data_s[si],\
                     buff_len_l[si],buff_len_h[si]

                ret
                ENDP

;****************************************************************************
DMA_transfer    PROC Addr:FAR PTR,LenL,LenH
                USES ds,si,di           ;Set up simulated DMA transfer,
                                        ;and send first "chunk"
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
xfer_chunk      PROC                    ;Get addr, size of next chunk; send it
                USES ds,si,di
                LOCAL blk_len

                mov ax,BLK_SIZE
                cmp DMA_len_h,0
                ja __set_size
                cmp DMA_len_l,ax
                ja __set_size
                mov ax,DMA_len_l
__set_size:     mov blk_len,ax          ;blk_len = # of bytes

                mov DPS.sndlen,ax

                mov WORD PTR DPS.sndstat,OFFSET playing_flag
                mov WORD PTR DPS.sndstat+2,cs

                lds si,DMA_ptr
                FAR_TO_HUGE ds,si       ;DS:SI = start of data to send

                mov WORD PTR DPS.sndptr,si
                mov WORD PTR DPS.sndptr+2,ds

                ADD_PTR blk_len,0,ds,si ;add len of chunk to "DMA pointer"
                mov WORD PTR DMA_ptr,si
                mov WORD PTR DMA_ptr+2,ds

                mov ax,DMA_len_l        ;subtract len of transmitted chunk
                mov dx,DMA_len_h
                sub ax,blk_len
                sbb dx,0
                mov DMA_len_h,dx
                mov DMA_len_l,ax

                call AIL_unhook

                mov ax,cs
                mov ds,ax
                lea si,DPS
                mov ax,688h             ;DP function 1: DigPlay
                test DP_attribs,MODIFIES_DATA
                jz __DigPlay
                mov ax,68bh             ;DP function 4: DigPlay2
__DigPlay:      int 66h

                ret              
                ENDP

;****************************************************************************
;*                                                                          *
;*  Public (API-accessible) procedures                                      *
;*                                                                          *
;****************************************************************************

DP_describe_driver PROC H               ;Return far ptr to DDT
                USES ds,si,di

                cmp DP_installed,0
                jne __get_desc

                les di,DP_base
                mov call_seg,es
                add di,100h             ;offset to installation call
                mov call_off,di
                call [DP_call]          ;enable INT 66H interface

                mov ax,68ch             ;DP function 5: AudioCapabilities
                int 66h
                mov DP_attribs,ax          

                mov ax,-1               
                test DP_attribs,USES_TICK
                jz __set_int_svc        ;let application know if music will
                mov ax,0                ;be stopped by digitized output
__set_int_svc:  mov DP_uses_INT8,ax

                mov DP_installed,1

__get_desc:     les di,DP_base          ;copy DIGPAK device description string
                mov si,0                ;to local data area and return as 
                mov cx,78               ;AIL device name list
__copy_name:    mov al,es:[di+12]
                cmp al,32
                jb __end_string
                mov DP_name[si],al
                inc si
                inc di
                loop __copy_name
__end_string:   mov DP_name[si],0       ;add 2 0-bytes to end device name list
                mov DP_name[si+1],0     

                mov dx,cs
                mov DP_devname_s,dx
                lea ax,DP_DDT

                ret
                ENDP

;****************************************************************************
DP_detect_device PROC H,IO_ADDR,IRQ,DMA,DRQ
                USES ds,si,di    

                mov ax,1                ;assume DIGPAK device exists

                ret
                ENDP

;****************************************************************************
DP_init_driver  PROC H,IO_ADDR,IRQ,DMA,DRQ  
                USES ds,si,di

                call AIL_register_timer C,0,0
                mov DP_timer,ax         ;register dummy timer

                mov AIL_hooked,1        ;assume AIL timers currently enabled

                call DP_describe_driver C,0

                mov loop_cnt,0
                mov DAC_status,DAC_STOPPED
                mov buffer_mode,BUF_MODE

                mov buff_status[0*2],DAC_DONE
                mov buff_status[1*2],DAC_DONE

                ret
                ENDP

;****************************************************************************
DP_shutdown_driver PROC H,SignOff:FAR PTR
                USES ds,si,di

                call DP_stop_d_pb

                les di,DP_base
                mov call_seg,es
                add di,103h             ;offset to removal call
                mov call_off,di
                call [DP_call]

                call AIL_release_timer_handle C,[DP_timer]

                ret
                ENDP

;****************************************************************************
DP_index_VOC_blk PROC H,File:FAR PTR,Block,SBuf:FAR PTR
                USES ds,si,di

                lds si,[File]
                mov ax,[si+14h]         ;get offset of data block
                ADD_PTR ax,0,ds,si

                mov bx,[Block]

__get_type:     mov al,[si]             ;get block type
                mov ah,0
                cmp ax,0                ;terminator block?
                je __exit               ;yes, return AX=0 (block not found)

                cmp ax,1                ;voice data block?
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

                mov al,[si+4]           ;copy sampling rate
                mov ah,0
                mov es:[di].sample_rate,ax

                mov al,[si+5]           ;copy packing type
                mov ah,0
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

__exit:         ret
                ENDP

;****************************************************************************
DP_register_sb  PROC H,BufNum,SBuf:FAR PTR
                USES ds,si,di

                cmp buffer_mode,VOC_MODE        
                jne __get_bufnum        ;not in VOC mode, proceed
                call DP_stop_d_pb C,0   ;else stop VOC file output first
                mov buffer_mode,BUF_MODE

__get_bufnum:   mov di,[BufNum]         ;get buffer #0-1
                shl di,1

                lds si,[SBuf]           ;copy structure data to buffer 
                mov ax,[si].sample_rate ;descriptor fields
                mov buff_sample[di],ax

                les bx,[si].data
                mov buff_data_o[di],bx
                mov buff_data_s[di],es

                mov ax,[si].len_l
                mov buff_len_l[di],ax
                mov ax,[si].len_h
                mov buff_len_h[di],ax
                
                mov buff_status[di],DAC_STOPPED

                ret
                ENDP

;****************************************************************************
DP_get_sb_status PROC H,HBuffer
                USES ds,si,di

                mov bx,[HBuffer]
                shl bx,1
                mov ax,buff_status[bx]

                ret
                ENDP

;****************************************************************************
DP_play_VOC_file PROC H,File:FAR PTR,Block
                LOCAL block_file:DWORD
                USES ds,si,di
                   
                call DP_stop_d_pb C,0   ;assert VOC mode
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
                call marker_num         ;get marker # (or -1 if non-marker)
                mov si,ax
                call next_block
                cmp si,[Block]
                jne __find_blk

__do_it:        mov DAC_status,DAC_STOPPED         
                                        ;return w/block_ptr -> 1st file block
__exit:         ret
                ENDP

;****************************************************************************
DP_format_VOC_file PROC H,File:FAR PTR,Block
                LOCAL block_file:DWORD  ;leave interrupts enabled; this might
                LOCAL pack:BYTE         ;take awhile
                LOCAL freq:WORD
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
                call get_sample_rate C,[si+4]
                mov freq,ax

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

                call DP_preformat,freq,si,ds,ax,dx

__preform_next: call next_block
                jmp __preform_blk

__exit:         ret
                ENDP

;****************************************************************************
DP_format_sb    PROC H,SBuf:FAR PTR
                USES ds,si,di           
                LOCAL ssize:DWORD
                                        
                lds si,[SBuf]           
                mov ax,[si].len_l
                mov dx,[si].len_h
                mov WORD PTR ssize,ax
                mov WORD PTR ssize+2,dx

                call get_sample_rate C,[si].sample_rate
                call DP_preformat C,ax,[si].data,ssize

                ret
                ENDP

;****************************************************************************
DP_start_d_pb   PROC H
                USES ds,si,di

                cmp buffer_mode,VOC_MODE
                je __voc_mode           ;start Creative Voice File playback

                cmp DAC_status,DAC_PLAYING
                je __exit               ;bail out if already playing

                call next_buffer        ;start dual-buffer playback                
                cmp ax,-1
                je __exit               ;no buffers registered, exit

                mov DAC_status,DAC_PLAYING

                call process_buffer C,ax
                jmp __exit

__voc_mode:     cmp DAC_status,DAC_STOPPED
                jne __exit
                
                mov DAC_status,DAC_PLAYING

                call process_block

__exit:         ret
                ENDP

;****************************************************************************
DP_stop_d_pb    PROC H
                USES ds,si,di

                mov DAC_status,DAC_STOPPED

                call CB_set_vect C,0,0  ;remove callback trap

                mov ax,68fh             ;DP function 8: StopSound
                int 66h

                mov buff_status[0*2],DAC_DONE
                mov buff_status[1*2],DAC_DONE

                call AIL_hook           ;restore AIL service

                ret
                ENDP

;****************************************************************************
DP_get_VOC_status PROC H
                USES ds,si,di

                mov ax,DAC_status

                ret
                ENDP

                ENDIF                   ;end IF DIGPAK

;*****************************************************************************
                END
