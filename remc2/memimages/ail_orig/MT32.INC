;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
;лл                                                                        лл
;лл   MT32.INC                                                             лл
;лл                                                                        лл
;лл   IBM Audio Interface Library -- Roland MT-32-series MIDI interpreter  лл
;лл                                                                        лл
;лл   Version 1.00 of 27-Sep-91: Initial version for AIL V2.0 release      лл
;лл   Version 1.01 of 12-Mar-92: Bad seq handles rejected by get_request   лл
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

MAX_REC_CHAN    equ 10                  ;Max channel recognized by synths
MAX_TRUE_CHAN   equ 9                   ;Max channel available for locking
MIN_TRUE_CHAN   equ 2                   ;Min channel # (1-based)

SYSEX_SIZE      equ 32                  ;Controller sysex queue size in bytes
SYSEX_Q_CNT     equ 3                   ;# of sysex controller queues
DEF_SYNTH_VOL   equ 90                  ;init vol=90% (avoid MT-32 distortion)
CLEAR_SYNTH     equ TRUE                ;TRUE to reset Roland on init/shutdown
ADJUST_PART_RSV equ TRUE                ;TRUE to set AIL partial reserve

                ;
                ;Driver Description Table (DDT)
                ;Returned by describe_driver() proc
                ;

DDT             LABEL WORD
min_API_version dw 200                  ;Minimum API version required = 2.00
drvr_type       dw 3                    ;Type 3: XMIDI driver
data_suffix     db 'MT',0,0             ;Native data/instrument file suffix
device_name_o   dw OFFSET devnames      ;Pointer to list of supported devices
device_name_s   dw ?
default_IO      LABEL WORD
                IFDEF MPU401
                dw 330h                 ;Factory default I/O parameters
                ELSEIFDEF SBMIDI
                dw 220h
                ENDIF
default_IRQ     dw -1
default_DMA     dw -1
default_DRQ     dw -1
service_rate    dw QUANT_RATE           ;Typically 120 calls/second 
display_size    dw 20                   ;20 char LCD display (MT-32)

devnames        db 'Roland MT-32 or compatible with Roland MPU MIDI Interface',0
                db 'Roland LAPC-1',0
                db 0                    ;0 to end list of device names

                ;
                ;Default setup values & constants
                ;

init_reverb     db 0,3,2                ;reverb mode = 0, time = 3, level = 2
part_chans      db 1,2,3,4,5,6,7,8,9    ;default (0-based) chans for each part
part_rsv        db 3,4,3,4,3,4,3,4,4    ;# of reserved AIL partials/channel

NUM_TIMBS       equ 64                  ;# of RAM timbres

SYSEX_BLK_SIZE  = (FINAL_BYTE_1 - START_MSB_1) + 1
SYSEX_RANGE_BEG = START_MSB_1
SYSEX_RANGE_END = SYSEX_RANGE_BEG + (SYSEX_BLK_SIZE * SYSEX_Q_CNT) - 1

                ;
                ;Misc. data
                ;

patch_bank      db 128 dup (?)

chksum          dw ?
string_buf      db 32 dup (?)

sysex_queues    db SYSEX_SIZE*SYSEX_Q_CNT dup (?)

M_ad            db SYSEX_Q_CNT dup (?)
K_ad            db SYSEX_Q_CNT dup (?)
L_ad            db SYSEX_Q_CNT dup (?)
queue_ptrs      db SYSEX_Q_CNT dup (?)

note_event_l    dw ?                            ;used for LRU counting
note_event_h    dw ?
timb_hist_l     dw NUM_TIMBS dup (?)            ;last note event count for LRU
timb_hist_h     dw NUM_TIMBS dup (?)
timb_bank       db NUM_TIMBS dup (?)            ;GTR bank
timb_num        db NUM_TIMBS dup (?)            ;GTR #
timb_attribs    db NUM_TIMBS dup (?)            ;bit 7=in use 6=protected

chan_timbs      db NUM_CHANS dup (?)            ;indexes used by channels 1-16
                                                ;(-1 if internal/uninit'd)

MIDI_bank       db NUM_CHANS dup (?)            ;Patch Bank Select values
MIDI_program    db NUM_CHANS dup (?)            ;program change # / channel

                IFDEF MPU401
                INCLUDE mpu401.inc      ;Roland MPU-401-compatible interface
                ENDIF

                IFDEF SBMIDI
                INCLUDE sbmidi.inc      ;Sound Blaster MIDI port
                ENDIF

;****************************************************************************
;*                                                                          *
;*  MIDI interpreter and related procedures                                 *
;*                                                                          *
;****************************************************************************

reset_synth     PROC                   
                USES ds,si,di
                pushf                
                cli

                IF CLEAR_SYNTH          ;reset all internal parameters/patches
                call send_sysex_msg C,0,7fh,00h,00h,OFFSET part_chans,cs,1,12
                ENDIF
                                        ;init part/channel assignments
                call send_sysex_msg C,0,10h,00h,0dh,OFFSET part_chans,cs,9,4

                POP_F
                ret
                ENDP

;****************************************************************************
init_synth      PROC
                USES ds,si,di
                pushf           
                cli
                                        ;send adjusted partial reserve table
                IF ADJUST_PART_RSV      ;to avoid dropout on high channel #s
                call send_sysex_msg C,0,10h,00h,04h,OFFSET part_rsv,cs,9,4
                ENDIF                   

                call send_sysex_msg C,0,10h,00h,01h,OFFSET init_reverb,cs,3,4

                mov bx,0
__init_sysex:   mov queue_ptrs[bx],0
                inc bx
                cmp bx,SYSEX_Q_CNT
                jne __init_sysex

                mov note_event_l,0
                mov note_event_h,0

                mov di,0
__init_tcache:  mov timb_attribs[di],00000000b
                inc di
                cmp di,NUM_TIMBS
                jne __init_tcache

                mov di,0
__init_tchans:  mov chan_timbs[di],-1
                mov MIDI_program[di],-1
                mov MIDI_bank[di],0
                inc di
                cmp di,NUM_CHANS
                jne __init_tchans

                mov di,0
__init_patches: mov patch_bank[di],0
                inc di
                cmp di,128
                jne __init_patches

                POP_F
                ret
                ENDP

;****************************************************************************
add_sysex_addr  PROC Addend,MSB:BYTE,KSB:BYTE,LSB:BYTE
                USES ds,si,di                   ;Add value to 21-bit address

                mov al,[LSB]
                mov bl,[KSB]
                mov cl,[MSB]
                mov ah,0
                mov bh,0
                mov ch,0

                add ax,[Addend]

__chk_l:        cmp ax,80h
                jb __chk_k
                sub ax,80h
                inc bx
                jmp __chk_l

__chk_k:        cmp bx,80h
                jb __store_sum
                sub bx,80h
                inc cx
                jmp __chk_k

__store_sum:    mov ah,bl
                mov dl,cl
                mov dh,0
                ret     
                ENDP

;****************************************************************************
send_MIDI_sysex PROC Src:FAR PTR,Type,Len
                USES ds,si,di           ;Send MIDI System Exclusive message
                cld

                cmp [Type],0f0h         ;F0 type?
                jne __get_msg_len       ;no, don't send explicit F0

                call send_byte C,0f0h

__get_msg_len:  lds si,[Src]
                mov cx,[Len]            ;get length
                jcxz __exit

__send_bytes:   push cx
                lodsb
                call send_byte C,ax
                pop cx
                loop __send_bytes

__exit:         ret
                ENDP

;****************************************************************************
sysex_wait      PROC Delay              ;Delay after System Exclusive message
                USES ds,si,di           ;transmissions

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
write_system    PROC Index,Value 
                USES ds,si,di           ;Write value to System area
                                        ;(Warning: No delay!)
                mov ax,[Index]
                lea bx,[Value]  
                call send_sysex_msg C,0,10h,0,ax,bx,ss,1,0      

                ret
                ENDP

;****************************************************************************
write_rhythm_setup PROC Keynum,Offset,Value
                USES ds,si,di           ;Write value to Rhythm Setup area

	mov ax,[Keynum]
	sub ax,24
	shl ax,1
	shl ax,1
	add ax,[Offset]
	call add_sysex_addr C,ax,3,1,16

	mov cl,ah
		
                lea bx,[Value]
                call send_sysex_msg C,0,dx,cx,ax,bx,ss,1,4

                ret
                ENDP

;****************************************************************************
write_patch     PROC Patch,Index,Value,Size
                USES ds,si,di           ;Write byte or word to Patch area
                                 
                mov ax,[Patch]
                shl ax,1
                shl ax,1
                shl ax,1
                add ax,[Index]
                call add_sysex_addr C,ax,5,0,0

	mov cl,ah
		
                lea bx,[Value]
                call send_sysex_msg C,0,dx,cx,ax,bx,ss,[Size],2

__exit:         ret
                ENDP

;****************************************************************************
send_MIDI_message PROC Stat:BYTE,D1:BYTE,D2:BYTE       
                LOCAL op                ;Send MIDI Channel Voice message
                USES ds,si,di           
                NOJUMPS

                mov si,WORD PTR [D1]
                and si,0ffh             ;SI=data 1 / controller #
                mov di,WORD PTR [Stat]
                mov ax,di               
                and di,00fh             ;DI=channel

                and ax,0f0h             ;AX=status
                cmp ax,0b0h             
                je __go_cc
                cmp ax,0c0h
                je __prg_change
                cmp ax,090h             ;Note On (or Note Off)?
                jne __send              

                add note_event_l,1      ;yes, update timbre cache LRU counters
                adc note_event_h,0      
                mov bh,0
                mov bl,chan_timbs[di]
                cmp bl,-1
                je __send       
                shl bx,1
                mov ax,note_event_l
                mov dx,note_event_h
                mov timb_hist_l[bx],ax
                mov timb_hist_h[bx],dx

__send:         mov di,WORD PTR [Stat]
                call send_byte C,di
                call send_byte C,si
                and di,0f0h
                cmp di,0c0h
                je __exit
                cmp di,0d0h
                je __exit
                call send_byte C,WORD PTR [D2]
__exit:         ret

                JUMPS

__go_cc:        jmp __ctrl_change

__prg_change:   mov ax,si               ;log current patch for channel
                mov MIDI_program[di],al
                mov ah,MIDI_bank[di]
                cmp ah,patch_bank[si]   ;changing patch's timbre bank?
                je __set_index          ;no, just log index & send message

                push ax                 ;else send sysex message to associate
                mov al,ah               ;patch with timbre
                call setup_patch C,si,ax
                pop ax

__set_index:    call index_timbre C,ax
                mov chan_timbs[di],al
                jmp __send

__ctrl_change:  cmp si,SYSEX_RANGE_BEG
                jb __send
                cmp si,SYSEX_RANGE_END
                jbe __sysex
                cmp si,PATCH_REVERB
                je __p_rev
                cmp si,PATCH_BENDER
                je __p_b_range
                cmp si,REVERB_MODE
                je __rev_mode
                cmp si,REVERB_TIME
                je __rev_time
                cmp si,REVERB_LEVEL
                je __rev_lvl
                cmp si,PATCH_BANK_SEL
                je __t_bank
	cmp si,RHYTHM_KEY_TIMB
	je __r_t_sel
                cmp si,TIMBRE_PROTECT
                je __t_prot
                
                cmp si,CHAN_LOCK                ;(lowest XMIDI control #)
                jb __send                       
                cmp si,SEQ_INDEX                ;(highest XMIDI control #)
                ja __send                       ;keep XMIDI controls out of 
                jmp __exit                      ;MIDI data stream for speed

__t_prot:       mov bl,chan_timbs[di]
                cmp bl,-1
                je __exit
                mov bh,0
                mov al,timb_attribs[bx]
                and al,10111111b
                cmp [D2],64
                jl __tprot
                or al,01000000b
__tprot:        mov timb_attribs[bx],al
                jmp __exit

__t_bank:       mov al,[D2]
                mov MIDI_bank[di],al
                jmp __exit

__p_rev:        mov al,MIDI_program[di]
                mov ah,0
                cmp al,-1
                je __exit
                push ax
                call write_patch C,ax,6,WORD PTR [D2],1
                or di,0c0h
                call send_byte C,di
                pop ax
                call send_byte C,ax
                jmp __exit

__p_b_range:    mov al,MIDI_program[di]
                mov ah,0
                cmp al,-1
                je __exit
                push ax
                call write_patch C,ax,4,WORD PTR [D2],1
                or di,0c0h
                call send_byte C,di
                pop ax
                call send_byte C,ax
                jmp __exit

__rev_mode:     call write_system C,1,WORD PTR [D2]
                jmp __exit
__rev_time:     call write_system C,2,WORD PTR [D2]
                jmp __exit
__rev_lvl:      call write_system C,3,WORD PTR [D2]
                jmp __exit

__r_t_sel:      mov al,chan_timbs[di]
                cmp al,-1
                je __exit
                mov ah,0
	call write_rhythm_setup C,WORD PTR [D2],0,ax
	jmp __exit

__sysex:        sub si,SYSEX_RANGE_BEG
                mov ax,si
                mov cx,SYSEX_BLK_SIZE
                mov dx,0
                div cx    
                mov op,dx
                mov bx,ax
                mov al,[D2]             ;BX=queue #, AL=data, DX=operation
                cmp dx,0
                je __set_MSB
                cmp dx,1
                je __set_KSB
                cmp dx,2
                je __set_LSB

                mov ax,SYSEX_SIZE       ;BX = queue #
                mul bx
                add ax,OFFSET sysex_queues
                mov di,ax               ;DI = base queue address
                add al,queue_ptrs[bx]
                adc ah,0
                mov si,ax               ;SI = pointer to last byte
                mov al,[D2]
                mov cs:[si],al

                cmp op,3
                je __bump_ptr           ;inc ptr and exit if not final byte

__send_queue:   mov al,queue_ptrs[bx]   ;else send message
                mov ah,0
                inc ax
                mov si,ax               ;SI = len
                xchg di,bx              ;DI = queue #, CS:BX = base address

                call send_sysex_msg C,0,WORD PTR M_ad[di],WORD PTR K_ad[di],\
                     WORD PTR L_ad[di],bx,cs,si,0

                cmp op,3                ;was this an overflow dump?
                jne __address_byte      

                call sysex_wait C,4     ;yes, do an automatic wait of 4
                jmp __overflowed        ;VBL intervals

__address_byte: dec si                  ;SI=len-1
__overflowed:   call add_sysex_addr C,si,WORD PTR M_ad[di],WORD PTR K_ad[di],\
                     WORD PTR L_ad[di]
               
                mov L_ad[di],al         ;add (qlen-1) to sysex start address,
                mov K_ad[di],ah         ;so future Final Byte controllers will
                mov M_ad[di],dl         ;address same byte (unless overflowed)

                mov queue_ptrs[di],0    ;flush queue
                jmp __exit

__bump_ptr:     cmp queue_ptrs[bx],SYSEX_SIZE-1
	jae __send_queue	;dump queue if overflow occurs
                inc queue_ptrs[bx]
                jmp __exit

__set_MSB:      mov M_ad[bx],al
                jmp __exit
__set_KSB:      mov K_ad[bx],al
                jmp __exit
__set_LSB:      mov L_ad[bx],al
                jmp __exit

                ENDP

;****************************************************************************
index_timbre    PROC GNum               ;Get global timbre's local index 0-63 
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
                cmp si,NUM_TIMBS
                jb __find_gnum

                mov si,-1               ;return -1 if timbre not loaded

__found:        mov ax,si
                ret
                ENDP

;****************************************************************************
setup_patch     PROC Patch:BYTE,Bank:BYTE         
                USES ds,si,di           ;Establish patch's timbre setting

                mov bl,[Patch]
                mov bh,0
                mov ah,[Bank]
                mov patch_bank[bx],ah   ;log timbre bank # for patch

                cmp ah,0
                je __bank_0             ;bank 0 = built-in Roland timbres

                mov al,bl
                call index_timbre C,ax  ;see if timbre loaded
                cmp ax,-1
                je __bank_0             ;timbre not loaded, use default

                mov ah,al               ;select TIMBRE NUMBER 0-63
                mov al,2                ;select MEMORY (timbre group 2)
                jmp __send

__bank_0:       mov ah,[Patch]          ;restore default Roland timbre...
                mov al,0
                cmp ah,64               ;set GROUP A if patch # < 64
                sbb al,-1               ;set GROUP B if patch # >= 64
                and ah,63               ;set TIMBRE NUMBER 0-63

__send:         call write_patch C,WORD PTR [Patch],0,ax,2

__exit:         ret                   
                ENDP            

;****************************************************************************
;*                                                                          *
;*  Public (API-accessible) procedures                                      *
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
send_sysex_msg  PROC H,AddrA:BYTE,AddrB:BYTE,AddrC:BYTE,Data:FAR PTR,Size,Wait
                USES ds,si,di           
                pushf
                cli

                call send_byte C,0f0h   ;transmit Roland MT-32 header
                call send_byte C,041h
                call send_byte C,010h
                call send_byte C,016h
                call send_byte C,012h

                call send_byte C,WORD PTR [AddrA]
                call send_byte C,WORD PTR [AddrB]
                call send_byte C,WORD PTR [AddrC]

                mov al,[AddrA]          ;init checksum
                mov ah,0
                add al,[AddrB]
                adc ah,0
                add al,[AddrC]
                adc ah,0
                mov chksum,ax

                les di,[Data]           ;send each message byte
                mov si,[Size]
__send_data:    or si,si
                jz __send_chksum
                dec si
                xor ah,ah
                mov al,es:[di]
                inc di
                add chksum,ax
                call send_byte C,ax
                jmp __send_data

__send_chksum:  mov ax,chksum           
                and ax,7fh
                sub al,80h
                neg al
                and ax,7fh
                call send_byte C,ax     ;send the checksum byte

                call send_byte C,0f7h   ;send EOX terminator

                call sysex_wait C,[Wait]

                POP_F
                ret
                ENDP

;****************************************************************************
write_display   PROC H,String:FAR PTR   ;Write string to display (unless NULL)
                USES ds,si,di
                pushf
                cli

                cld
                lds si,[String]         ;bail out if null string
                mov ax,ds
                cmp ax,0
                je __end_write

                push cs                 ;pad string with spaces
                pop es
                lea di,string_buf
                mov cx,display_size
                mov al,' '
                rep stosb

                lea di,string_buf       ;write up to /display_size/ characters
                mov cx,display_size
__pad_string:   lodsb
                cmp al,0
                je __show_it
                stosb
                loop __pad_string

__show_it:      call send_sysex_msg C,0,20h,00h,00h,\
                     OFFSET string_buf,cs,display_size,4

__end_write:    POP_F
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
                cmp bx,NUM_TIMBS
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
                cmp bx,NUM_TIMBS
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
                cmp ah,0                ;Roland driver reports all bank 0
                je __OK                 ;(default) and bank 127 (rhythm) 
                cmp ah,127              ;timbres present
                je __OK                 

                call index_timbre C,ax

__OK:           inc ax                  ;0 if -1, else local cache index

                POP_F
                ret
                ENDP

;****************************************************************************
get_cache_size  PROC H
                USES ds,si,di
                pushf
                cli

                mov ax,0                ;no resident cache for MT-32

                POP_F
                ret
                ENDP

;****************************************************************************
define_cache    PROC H,Addr:FAR PTR,Size
                USES ds,si,di

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
                cmp ah,0                ;bank 0 reserved for Roland built-in
                je __next_index         ;timbres.... don't request
                cmp ah,127              ;bank 127 reserved for melodic-mode 
                je __next_index         ;rhythm sounds.... don't request
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
install_timbre  PROC H,Bank:BYTE,Num:BYTE,Addr:FAR PTR
                LOCAL sys_seg,sys_dest
                USES ds,si,di
                pushf
                cli

                mov al,[Num]
                mov ah,[Bank]

                cmp ah,0                ;bank 0 reserved for built-in default
                je __set_patch          ;timbres -- restore normal patch
                cmp ah,127              ;bank 127 reserved for melodic mode
                je __exit               ;rhythm sounds -- don't install

                call index_timbre C,ax
                cmp ax,-1
                jne __set_patch         ;timbre already resident, index it

                mov ax,WORD PTR [Addr]
                or ax,WORD PTR [Addr+2]
                jz __exit

                mov si,0                ;else look for an empty timbre slot
__find_free:    test timb_attribs[si],10000000b
                jz __found
                inc si
                cmp si,NUM_TIMBS
                jb __find_free

                mov si,0                ;no free timbre slots, replace least-
                mov bx,0                ;recently-used unprotected timbre
                mov cx,-1
                mov ax,-1
                mov dx,-1
__find_LRU:     test timb_attribs[si],01000000b
                jnz __next_LRU          ;(timbre protected)
                cmp timb_hist_h[bx],dx
                ja __next_LRU
                jb __log_LRU
                cmp timb_hist_l[bx],ax
                ja __next_LRU
__log_LRU:      mov ax,timb_hist_l[bx]
                mov dx,timb_hist_h[bx]
                mov cx,si
__next_LRU:     add bx,2
                inc si
                cmp si,NUM_TIMBS
                jb __find_LRU

                cmp cx,-1               ;if no unprotected timbre slots, exit 
                je __exit               ;without installing new timbre

                mov si,cx               ;else SI = LRU timbre index

__found:        mov bx,si               ;found free/LRU timbre, update its
                shl bx,1                ;timestamp and replace it
                mov ax,note_event_l      
                mov dx,note_event_h     
                add note_event_l,1      
                adc note_event_h,0      
                mov timb_hist_l[bx],ax  
                mov timb_hist_h[bx],dx  

                mov al,[Num]            ;AL=num, AH=bank
                mov ah,[Bank]
                mov timb_num[si],al     ;record global # in slot
                mov timb_bank[si],ah    ;mark timbre "in use/unprotected"
                mov timb_attribs[si],10000000b                  

                cmp WORD PTR [Addr+2],0
                je __exit               ;(NULL pointer = placeholder)

                les di,[Addr]
                add di,2                ;skip timbre length prefix (norm. $F6)

                mov ax,si               ;set up to send timbre dump to Roland
                shl ax,1                ;8,AX,00 = MT-32 address MSB,KSB,LSB

                mov sys_seg,es          ;send common parameter
                mov sys_dest,ax
                call send_sysex_msg C,0,8,sys_dest,0,di,sys_seg,0eh,3

                add di,0eh              ;send partial parameter #1
                call send_sysex_msg C,0,8,sys_dest,0eh,di,sys_seg,3ah,3

                add di,3ah              ;send partial parameter #2
                call send_sysex_msg C,0,8,sys_dest,48h,di,sys_seg,3ah,3

                add di,3ah              ;send partial parameter #3
                inc sys_dest
                call send_sysex_msg C,0,8,sys_dest,2,di,sys_seg,3ah,3

                add di,3ah              ;send partial parameter #4
                call send_sysex_msg C,0,8,sys_dest,3ch,di,sys_seg,3ah,3

__set_patch:    call setup_patch C,WORD PTR [Num],WORD PTR [Bank]

__exit:         POP_F
                ret
                ENDP


