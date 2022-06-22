;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
;лл                                                                        лл
;лл   SPKR.INC                                                             лл
;лл                                                                        лл
;лл   IBM Audio Interface Library -- XMIDI interpreter for IBM PC 1-voice  лл
;лл                                  and Tandy 3-voice internal speakers   лл
;лл                                                                        лл
;лл   Version 1.00 of 10-Nov-91: Initial version for AIL V2.0 release      лл
;лл           1.01 of 29-Feb-92: Linear pitch/modulation options added     лл
;лл                              PC speaker clicks reduced                 лл
;лл           1.02 of 21-Apr-92: Tandy modulation bug fixed                лл
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

                IFDEF IBMPC
MAX_REC_CHAN    equ 2                   ;Max channel recognized by synths
MAX_TRUE_CHAN   equ 2                   ;Max channel available for locking
NUM_VOICES      equ 1                   ;# of physical voices available
NUM_SLOTS       equ 2                   ;# of virtual voices available
                ELSEIFDEF TANDY
MAX_REC_CHAN    equ 4 
MAX_TRUE_CHAN   equ 4
NUM_VOICES      equ 3
NUM_SLOTS       equ 4
                ENDIF

MIN_TRUE_CHAN   equ 2                   ;Min channel # (1-based)

DEF_SYNTH_VOL   equ 100                 ;Init vol=100%
DEF_TC_SIZE     equ 0                   ;No timbres used
MAX_TIMBS       equ 0                  

VEL_SENS        equ 1                   ;Velocity sensitivity disabled if 0

VEL_TRUE        equ 0                   ;Full velocity sensitivity range if 1
                                        ;(set to 0 to reduce playback noise)
DEF_PITCH_RANGE equ 12                  ;Default pitch wheel range (semitones)

LINEAR_PITCH    equ 1                   ;0 for semitonal pitch wheel response

EMULATE_MOD     equ 1                   ;(Active only w/LINEAR_PITCH = 1)
MOD_RATE        equ 20                  ; FM simulated at 20 Hz
MOD_DEPTH       equ 5                   ; 5 pitch wheel units/modulation unit

                ;
                ;Driver Description Table (DDT)
                ;Returned by describe_driver() proc
                ;

DDT             LABEL WORD
min_API_version dw 200                  ;Minimum API version required = 2.00
drvr_type       dw 3                    ;Type 3: XMIDI emulation
data_suffix     db 4 dup (0)            ;No Global Timbre Library used
device_name_o   dw OFFSET devnames      ;Pointer to list of supported devices
device_name_s   dw ?
default_IO      dw -1                   ;Factory default I/O parameters
default_IRQ     dw -1
default_DMA     dw -1
default_DRQ     dw -1
service_rate    dw QUANT_RATE           ;Request QUANT_RATE calls/second 
display_size    dw 0                    ;No display

devnames        LABEL BYTE
                IFDEF IBMPC
                db 'IBM PC or compatible internal speaker',0
                ELSEIFDEF TANDY
                db 'Tandy 3-voice internal sound',0
                ENDIF
                db 0                    ;0 to end list of device names

                ;
                ;Default setup values & internal constants
                ;

                IFDEF IBMPC

value_l         db 136,181,78,64,65,205,196,61,67,124,42,214
                db 136,181,255,209,32,167,226,30,206,190,242,138
                db 68,65,127,232,144,99,99,143,231,95,1,189
                db 162,160,191,244,72,177,49,199,112,47,254,224
                db 209,208,222,251,36,88,152,227,57,151,255,111
                db 232,104,239,125,17,172,76,241,156,75,255,183
                db 116,52,247,190,136,86,38,248,206,165,127,91
                db 58,26,251,223,196,171,147,124,103,82,63,45

value_h         db 142,134,253,240,226,213,201,190,179,169,160,150
                db 142,134,126,119,113,106,100,95,89,84,79,75
                db 71,67,63,59,56,53,50,47,44,42,40,37
                db 35,33,31,29,28,26,25,23,22,21,19,18
                db 17,16,15,14,14,13,12,11,11,10,9,9
                db 8,8,7,7,7,6,6,5,5,5,4,4
                db 4,4,3,3,3,3,3,2,2,2,2,2
                db 2,2,1,1,1,1,1,1,1,1,1,1
                
                ELSEIFDEF TANDY

value_l         db 7,7,9,14,6,0,12,10,10,8,0,9
                db 7,7,9,14,6,0,12,10,10,8,0,9
                db 7,7,9,14,6,0,12,10,10,8,0,9
                db 7,7,9,14,6,0,12,10,10,12,15,5
                db 11,3,12,7,3,0,14,13,13,14,15,2
                db 5,9,14,3,9,0,7,14,6,15,7,1
                db 10,4,15,9,4,0,11,7,3,15,11,8
                db 5,2,15,12,10,8,5,3,1,15,13,12

value_h         db 53,50,47,44,42,40,37,35,33,63,60,56
                db 53,50,47,44,42,40,37,35,33,63,60,56
                db 53,50,47,44,42,40,37,35,33,63,60,56
                db 53,50,47,44,42,40,37,35,33,31,29,28
                db 26,25,23,22,21,20,18,17,16,15,14,14
                db 13,12,11,11,10,10,9,8,8,7,7,7
                db 6,6,5,5,5,5,4,4,4,3,3,3
                db 3,3,2,2,2,2,2,2,2,1,1,1

atten           db 15,14,14,14,14,14,14,14,13,13,13,13,13,13,13,13
                db 12,12,12,12,12,12,12,12,11,11,11,11,11,11,11,11
                db 10,10,10,10,10,10,10,10,9,9,9,9,9,9,9,9
                db 8,8,8,8,8,8,8,8,7,7,7,7,7,7,7,7
                db 6,6,6,6,6,6,6,6,5,5,5,5,5,5,5,5
                db 4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3
                db 2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1
                db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

atten_reg       db 090h,0b0h,0d0h
part_reg        db 080h,0a0h,0c0h

                ENDIF
                             
vel_graph       db 82,85,88,91,94,97,100,103,106,109,112,115,118,121,124,127

                ;
                ;Misc. data
                ;

S_status        db NUM_SLOTS dup (?)  ;2=key on, 1=key off, 0=slot free
S_voice         db NUM_SLOTS dup (?)  ;Voice # or -1 assigned to slot
S_channel       db NUM_SLOTS dup (?)  ;MIDI channel owning slot
S_note          db NUM_SLOTS dup (?)  ;MIDI note # for slot's voice
S_keynum        db NUM_SLOTS dup (?)  ;MIDI key # before RBS translation
S_transpose     db NUM_SLOTS dup (?)  ;MIDI note transposition for slot
S_velocity      db NUM_SLOTS dup (?)  ;keyon velocity for note
S_sustain       db NUM_SLOTS dup (?)  ;note sustained if nonzero
S_update        db NUM_SLOTS dup (?)  ;bit mask for register updates

S_BLOCK         db NUM_SLOTS dup (?)  ;KON/BLOCK values
S_atten         db NUM_SLOTS dup (?)  ;Tandy attenuation register setup

S_p_val         dw NUM_SLOTS dup (?)
S_mod_value     dw NUM_SLOTS dup (?)  ;instantaneous pitch  for mod emulation

FREE            equ 0                 ;S_status[] phase equates
KEYON           equ 1
KEYOFF          equ 2

U_ALL_REGS      equ 01000001b         ;Bit mask equates for S_update
U_KSLTL         equ 01000000b
U_FREQ          equ 00000001b

MIDI_vol        db NUM_CHANS dup (?)  ;volume 
MIDI_pitch_l    db NUM_CHANS dup (?)  ;pitchwheel LSB
MIDI_pitch_h    db NUM_CHANS dup (?)  ;pitchwheel MSB
MIDI_express    db NUM_CHANS dup (?)  ;expression 
MIDI_sus        db NUM_CHANS dup (?)  ;HOLD1 pedal 
MIDI_mod        db NUM_CHANS dup (?)  ;modulation
MIDI_vprot      db NUM_CHANS dup (?)  ;voice protection

MIDI_voices     db NUM_CHANS dup (?)  ;# of voices assigned to channel

V_channel       db NUM_VOICES dup (?) ;voice assigned to MIDI channel n or -1

S_V_priority    dw NUM_SLOTS dup (?)  ;adjusted voice priorities

MOD_accum       dw ?                  ;DDA accumulator for modulation timing
MOD_sign        dw ?                  ;sign (+/-) for modulation

last_val_l      db ?
last_val_h      db ?

;****************************************************************************
;*                                                                          *
;* I/O routines                                                             *
;*                                                                          *
;****************************************************************************

;****************************************************************************
detect_device   PROC H,IO_ADDR,IRQ,DMA,DRQ  
                USES ds,si,di           ;Attempt to detect hardware

                pushf
                cli
                cld

                IFDEF IBMPC

                mov ax,1                ;IBM speaker always present

                ELSEIFDEF TANDY

                mov ax,0f000h           ;search PC BIOS for string 'Tandy'
                mov es,ax
                mov di,0
                mov cx,65535
                mov al,'T'
__search_T:     repne scasb
                jne __not_found
                cmp WORD PTR es:[di],'na'
                jne __search_T
                cmp WORD PTR es:[di+2],'yd'
                jne __search_T
                jmp __exit              ;found, assume Tandy sound available
__not_found:    mov ax,0              

                ENDIF

__exit:         POP_F
                ret
                ENDP

;****************************************************************************
reset_synth     PROC                    ;Init hardware regs
                USES ds,si,di

                pushf                  
                cli

                IFDEF IBMPC

                in al,61h
                and al,11111100b
                out 61h,al              ;turn the speaker off

                ELSEIFDEF TANDY
                        
                mov al,9fh              ;turn off any active sounds
                out 0c0h,al
                mov al,0bfh
                out 0c0h,al
                mov al,0dfh
                out 0c0h,al

                mov al,68h              ;enable internal speaker
                out 61h,al

                ENDIF

                POP_F
                ret
                ENDP

;****************************************************************************
;*                                                                          *
;*  Timbre cache management / related API calls                             *
;*                                                                          *
;****************************************************************************

;****************************************************************************
protect_timbre  PROC H,Bank:BYTE,Num:BYTE

                ret
                ENDP

;****************************************************************************
unprotect_timbre PROC H,Bank:BYTE,Num:BYTE            

                ret
                ENDP

;****************************************************************************
timbre_status   PROC H,Bank:BYTE,Num:BYTE            
                USES ds,si,di           ;Return 0 if timbre not resident
                pushf 
                cli

                mov ax,1                ;(no timbres needed)

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

                ret
                ENDP

;****************************************************************************
get_request     PROC H,Sequence
                USES ds,si,di
                pushf
                cli

                mov ax,-1
                
                POP_F
                ret
                ENDP

;****************************************************************************
install_timbre  PROC H,Bank:BYTE,Num:BYTE,Addr:FAR PTR

                ret
                ENDP

;****************************************************************************
;*                                                                          *
;*  MIDI interpreter and related procedures                                 *
;*                                                                          *
;****************************************************************************

                IF EMULATE_MOD
serve_synth     PROC                    ;Update simulated modulation         
                USES ds,si,di           ;Called once per quantization interval

                add MOD_accum,MOD_RATE
                cmp MOD_accum,QUANT_RATE
                jae __do_M_period
                ret

__do_M_period:  sub MOD_accum,QUANT_RATE

                xor MOD_sign,1

                mov si,0                ;mark appropriate voice parameters
__flag_slot:    cmp S_status[si],FREE   ;as "changed"
                je __flag_next
                mov bl,S_channel[si]
                mov bh,0
                mov al,MOD_DEPTH
                mul MIDI_mod[bx]
                cmp MOD_sign,0
                je __store
                neg ax
__store:        mov bx,si
                mov S_mod_value[si][bx],ax
                or S_update[si],U_FREQ
                call update_voice C,si  ;update the hardware registers
__flag_next:    inc si
                cmp si,NUM_SLOTS
                jne __flag_slot

                ret
                ENDP
                ENDIF

;****************************************************************************
init_synth      PROC                    ;Init MIDI synthesizer emulation
                USES ds,si,di
                pushf           
                cli
                cld

                mov di,0
__init_chans:   mov MIDI_voices[di],0
                inc di
                cmp di,NUM_CHANS
                jne __init_chans

                mov di,0
__init_slots:   mov S_status[di],FREE
                mov bx,di
                mov S_mod_value[di][bx],0
                inc di
                cmp di,NUM_SLOTS
                jne __init_slots

                mov di,0
__init_voices:  mov V_channel[di],-1
                inc di
                cmp di,NUM_VOICES
                jne __init_voices

                mov MOD_accum,0
                mov MOD_sign,0

                mov last_val_l,-1
                mov last_val_h,-1

                POP_F
                ret
                ENDP

;****************************************************************************
assign_voice    PROC Slot               ;Allocate hardware voice to slot
                USES ds,si,di
                
                mov bx,-1               ;try to find an unassigned voice
__search_free:  inc bx
                cmp bx,NUM_VOICES
                je __seize_voice        ;(# of active slots > # of voices)
                cmp V_channel[bx],-1   
                jne __search_free

                mov si,[Slot]           ;found free voice, assign it to slot
                mov S_voice[si],bl
                mov di,bx
                mov bl,S_channel[si]
                inc MIDI_voices[bx]
                mov V_channel[di],bl

                mov S_update[si],U_ALL_REGS
                call update_voice C,si  ;update the hardware
                ret

__seize_voice:  call update_priority    ;assign voice based on priority search
                ret                     
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
                mov V_channel[bx],-1

                mov S_voice[si],-1

__exit:         ret
                ENDP
                    
;****************************************************************************
VAL_SHIFT       MACRO
                IFDEF TANDY

                mov cl,ah
                mov ch,0
                shl cx,1
                shl cx,1
                shl cx,1
                shl cx,1
                or cl,al
                mov ax,cx

                ENDIF
                ENDM

RECOMBINE       MACRO
                IFDEF TANDY

                mov cx,ax
                shr cx,1
                shr cx,1
                shr cx,1
                shr cx,1
                mov ah,cl
                and al,0fh

                ENDIF
                ENDM

update_voice    PROC Slot               ;Update hardware regs for slot
                USES ds,si,di
                LOCAL voice,vol:BYTE
                LOCAL f_num
                LOCAL val_l:BYTE,val_h:BYTE
                LOCAL pitch,notenum,mid_val
                LOCAL MOD_value
                NOJUMPS                                                   

                mov si,[Slot]           ;update only requested parameters for
                                        ;speed
                mov bl,S_voice[si]
                cmp bl,-1               
                je __exit               ;no hardware voice assigned, exit

                mov bh,0
                mov ah,0
                mov voice,bx            ;voice #

                test S_update[si],U_KSLTL
                jnz __go_KSLTL
__KSLTL_done:   test S_update[si],U_FREQ
                jnz __go_FREQ           ;(update FREQ's KeyOn bit last)
__FREQ_done:
__exit:         ret

__go_KSLTL:     jmp __KSLTL
__go_FREQ:      jmp __FREQ

;----------------------------------------------------------------------------
__KSLTL:        IFDEF IBMPC

                ELSEIFDEF TANDY

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

                mov ah,0
                mov bx,ax               ;BX=composite (vol+expression) volume

                mov al,atten[bx]
                mov S_atten[si],al

                or S_update[si],U_FREQ  ;signal need to update keyon volume

                ENDIF

                and S_update[si],NOT U_KSLTL
                jmp __KSLTL_done

;----------------------------------------------------------------------------
__FREQ:         test S_BLOCK[si],00100000b
                jnz __key_on            ;KON bit = 1; turn note on

                IFDEF IBMPC

                in al,61h               ;turn IBM speaker off
                and al,11111100b
                out 61h,al

                ELSEIFDEF TANDY

                mov bx,voice
                mov al,atten_reg[bx]
                or al,0fh               ;set maximum Tandy attenuation
                out 0c0h,al

                ENDIF
                jmp __end_freq

__key_on:       mov bl,S_channel[si]    ;get pitch bend word value
                mov bh,0
                mov al,MIDI_pitch_h[bx]
                mov ah,0
                mov cx,7
                shl ax,cl
                or al,MIDI_pitch_l[bx]
                mov pitch,ax            ;store it

                mov bl,S_note[si]       ;get key # 12-108
                mov bh,0
                mov al,S_transpose[si]
                cbw
                add bx,ax
                sub bx,24               ;normalize to 0-95
__norm_1:       add bx,12
                cmp bx,0
                jl __norm_1
                add bx,12
__norm_2:       sub bx,12
                cmp bx,95
                jg __norm_2
                mov notenum,bx          ;store it

                IF LINEAR_PITCH

                push si
                push di

                shl si,1
                mov ax,S_mod_value[si]
                mov MOD_value,ax

                mov bx,notenum          ;get upper and lower note value bounds
                sub bx,DEF_PITCH_RANGE
                jg __lin_1
                mov bx,0
__lin_1:        mov al,value_l[bx]
                mov ah,value_h[bx]
                VAL_SHIFT
                mov si,ax               ;SI=low value

                mov bx,notenum
                add bx,DEF_PITCH_RANGE
                cmp bx,95
                jb __lin_2
                mov bx,95
__lin_2:        mov al,value_l[bx]
                mov ah,value_h[bx]
                VAL_SHIFT
                mov di,ax               ;DI=high value

                mov bx,notenum
                mov al,value_l[bx]
                mov ah,value_h[bx]      
                VAL_SHIFT
                mov dx,ax               ;DX=0 value
                mov mid_val,dx          

                mov ax,pitch
                sub ax,2000h            ;E000=min 0=none 1FFF=max
                add ax,MOD_value
                cmp ax,0
                jl __neg
                sub di,dx
                mov dx,di               ;positive bend: DX = maximum 
                jmp __set
__neg:          sub dx,si               ;negative bend: DX = maximum 

__set:          imul dx
                mov cx,2000h
                idiv cx

                add ax,mid_val
                RECOMBINE
                mov val_l,al
                mov val_h,ah

                pop di
                pop si

                ELSE
                mov ax,pitch
                sub ax,1fffh

                mov cx,5                ;divide by 0x20, preserving sign
                sar ax,cl               ;(range now +0x100 to -0x0FF)

                mov cl,DEF_PITCH_RANGE
                mov ch,0                ;normally 12 (+0xc00 to -0xBF4)
                imul cx                 

                add ah,BYTE PTR notenum ;add computed note offset * 256
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

                shr ax,1
                shr ax,1
                shr ax,1
                shr ax,1
                mov bx,ax          

                mov al,value_l[bx]
                mov val_l,al
                mov al,value_h[bx]
                mov val_h,al

                ENDIF                   ;LINEAR_PITCH

                IFDEF IBMPC

                mov bl,val_l            ;avoid redundant timer resets to
                mov bh,val_h            ;reduce clicks
                cmp bl,last_val_l
                jne __set_freq
                cmp bh,last_val_h
                je __on
__set_freq:     mov al,0b6h             ;program speaker frequency on Timer 2
                out 43h,al
                jmp $+2
                mov al,bl
                out 42h,al
                jmp $+2
                mov al,bh
                out 42h,al
                jmp $+2
                mov last_val_l,bl
                mov last_val_h,bh
__on:           in al,61h
                or al,00000011b
                out 61h,al              ;turn the PC speaker on

                ELSEIFDEF TANDY

                mov di,voice            ;set Tandy freq, keyon level
                mov al,part_reg[di]
                or al,val_l
                out 0c0h,al
                jmp $+2
                mov al,val_h
                out 0c0h,al
                jmp $+2
                mov al,atten_reg[di]
                or al,S_atten[si]
                out 0c0h,al

                ENDIF                   ;TANDY

__end_freq:     and S_update[si],NOT U_FREQ
                jmp __FREQ_done

                JUMPS
                ENDP

;****************************************************************************
update_priority PROC                    ;Maintain synthesizer voice priority
                LOCAL vcount,low_p,high_p
                USES ds,si,di

                mov dx,0                ;zero active slot count

                mov si,-1              
__get_priority: inc si                  ;build adjusted priority table and
                cmp si,NUM_SLOTS        ;reallocate voices if necessary
                je __chk_demand
                cmp S_status[si],FREE
                je __get_priority

                inc dx                  ;slot active, bump count
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

__chk_demand:   cmp dx,NUM_VOICES       ;more active slots than voices?
                jbe __exit              ;no, exit

                mov vcount,dx

__sort_p_list:  mov ax,0                ;set AX = unvoiced highest priority                
                mov dx,-1               ;set DX = voiced lowest priority
                mov si,-1               
__for_slot:     inc si                  
                cmp si,NUM_SLOTS
                je __reassign
                cmp S_status[si],FREE
                je __for_slot
                mov bx,si
                mov di,S_V_priority[bx][si]
                cmp S_voice[si],-1      
                jne __chk_low           
                cmp di,ax               
                jb __for_slot         
                mov ax,di              
                mov high_p,si           ;highest-priority unvoiced slot index
                jmp __for_slot
__chk_low:      cmp di,dx               
                ja __for_slot
                mov dx,di               
                mov low_p,si            ;lowest-priority voiced slot index
                jmp __for_slot

__reassign:     cmp ax,dx               ;highest unvoiced < lowest voiced?
                jb __exit               ;yes, we're done

                mov si,low_p
                mov bh,0
                mov bl,S_voice[si]    

                push bx
                call release_voice C,si
                mov S_status[si],FREE   ;release any slots deallocated...
                pop bx               

                mov si,high_p
                mov S_voice[si],bl
                mov di,bx
                mov bl,S_channel[si]
                inc MIDI_voices[bx]
                mov V_channel[di],bl

                mov S_update[si],U_ALL_REGS
                call update_voice C,si  ;update the hardware

                dec vcount
                jnz __sort_p_list       ;keep sorting until priorities valid

__exit:         ret
                ENDP


;****************************************************************************
slot_phase      PROC Slot                   ;Set up slot parameters
                USES ds,si,di

                mov si,[Slot]
                mov bx,si              
                shl bx,1
                
                mov S_BLOCK[si],00100000b   ;set KON, clear BLOCK mask

                mov S_p_val[bx],32767       ;instrument priority = average

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
                
                call release_voice C,si ;release the slot's voice
                mov S_status[si],FREE
                jmp __next_slot

__sustained:    mov S_sustain[si],1
                jmp __next_slot

__exit:         ret

                ENDP

;****************************************************************************
note_on         PROC Chan,Note,Velocity ;Turn MIDI note on
                USES ds,si,di

                mov di,[Chan]     

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

                mov S_note[si],dl
                mov S_transpose[si],0   ;(no transposition)

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

                mov S_status[si],KEYON  ;flag note "on" in slot

                mov S_sustain[si],0     ;init sustained flag

                call slot_phase C,si    ;set up BNK timbre in slot

                mov S_voice[si],-1

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

__ctrl_change:  cmp si,VOICE_PROTECT
                je __vprot
                jmp __MIDI

__vprot:        mov MIDI_vprot[di],cl
                jmp __exit

__MIDI:         mov al,0
                lea bx,MIDI_mod
                cmp si,MODULATION
                je __MIDI_set
                mov al,U_KSLTL          ;Emulate MIDI controllers
                lea bx,MIDI_vol
                cmp si,PART_VOLUME
                je __MIDI_set
                lea bx,MIDI_express
                cmp si,EXPRESSION
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
                mov MIDI_mod[di],0
                mov MIDI_express[di],127 ;emulate Roland LAPC-1 RAC message
                mov MIDI_pitch_l[di],DEF_PITCH_L
                mov MIDI_pitch_h[di],DEF_PITCH_H
                mov al,U_KSLTL OR U_FREQ
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