;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
;лл                                                                        лл
;лл   MMASTER.INC                                                          лл
;лл                                                                        лл
;лл   IBM Audio Interface Library -- ASC MediaMaster MIDI interpreter      лл
;лл                                                                        лл
;лл   Version 1.00 of  4-Jun-92: Initial version                           лл
;лл                                                                        лл
;лл   8086 ASM source compatible with Turbo Assembler v2.0 or later        лл
;лл   Author: John Miles                                                   лл
;лл                                                                        лл
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
;лл                                                                        лл
;лл   Copyright (C) 1992 Miles Design, Inc.                                лл
;лл                                                                        лл
;лл   Miles Design, Inc.                                                   лл
;лл   10926 Jollyville #308                                                лл
;лл   Austin, TX 78759                                                     лл
;лл   (512) 345-2642 / FAX (512) 338-9630 / BBS (512) 454-9990             лл
;лл                                                                        лл
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

MOP_PROTEUS     equ     10h
MOP_EXTOUT      equ     32h
MOP_EXTTHRU     equ     02h
MOP_OUTMASK     equ     01h

MIP_EXTIN       equ     01h
MIP_PROTEUS     equ     00h
MIP_INMASK      equ     32h

        include multisnd.equ
                ;
                ;Driver-specific configuration equates
                ;

MAX_REC_CHAN    equ 16                  ;Max channel recognized by synths
;MAX_TRUE_CHAN   equ 9                   ;Max channel available for locking
;MIN_TRUE_CHAN   equ 2                   ;Min channel # (1-based)
MAX_TRUE_CHAN   equ 16                   ;Max channel available for locking
MIN_TRUE_CHAN   equ 1                   ;Min channel # (1-based)

DEF_SYNTH_VOL   equ 50                 ;init vol=100%
CLEAR_SYNTH     equ TRUE                ;TRUE to reset on init/shutdown

                ;
                ;Driver Description Table (DDT)
                ;Returned by describe_driver() proc
                ;

DDT             LABEL WORD
min_API_version dw 200                  ;Minimum API version required = 2.00
drvr_type       dw 3                    ;Type 3: XMIDI driver
data_suffix     db 'MPP',0              ;Native data/instrument file suffix
device_name_o   dw OFFSET devnames      ;Pointer to list of supported devices
device_name_s   dw ?
default_IO      dw 290h                 ;Factory default I/O parameters
default_IRQ     dw 10
default_SMA     dw 0D000h
default_DRQ     dw -1
service_rate    dw QUANT_RATE           ;Typically 120 calls/second
display_size    dw 0

devnames        db 'Turtle Beach MultiSound (c)1993',0
                db 0                    ;0 to end list of device names
                ;
                ;Default setup values & constants
                ;

                ;
                ;Misc. data
                ;

patch_bank      db 128 dup (?)

note_event_l    dw ?                            ;used for LRU counting
note_event_h    dw ?

chan_timbs      db NUM_CHANS dup (?)            ;indexes used by channels 1-16
                                                ;(-1 if internal/uninit'd)

MIDI_bank       db NUM_CHANS dup (?)            ;Patch Bank Select values
MIDI_program    db NUM_CHANS dup (?)            ;program change # / channel

ECSR            dw ?                            ;Emulation Control/Status Reg
EDR             dw ?                            ;Emulation Data Reg

;****************************************************************************
;*                                                                          *
;*  MediaMaster 6850 I/O routines                                           *
;*                                                                          *
;****************************************************************************

set_IO_parms    PROC IO_ADDR,IRQ,DMA,DRQ
                USES ds,si,di


        ret
        ENDP

;****************************************************************************
detect_device   PROC H,IO_ADDR,IRQ,DMA,DRQ

                mov ax,1

                ret
                ENDP

;****************************************************************************
send_byte       PROC DataByte:BYTE

;        int     1              ; Used for Turbo Debug break point
        push    bx
        push    ax
        push    dx


        mov     al, [DataByte]
        mov     ah, 0

        push    ds
        mov     bx, default_SMA
        mov     ds, bx
        assume  ds:SRAM_BANK_0

        mov     bx, [ds:wMODQ_Tail]             ; curr/next tail
	mov	dx, bx				;*mof* save it for later
	inc	bx				; new next tail

        cmp     bx, [ds:wMODQ_Size]                  ; time to wrap ?
	jbe	modpw_2				; nope

	xor	bx, bx				; back to the start
modpw_2:

modpw_4:
        cmp     bx, [ds:wMODQ_Head]                  ; will buffer be full ?
	je	modpw_4				; yep, wait for space

;*mof*  xchg    bx, [ds:wMODQ_Tail]                  ; update new tail / get prev
;        mov     [ds:wMODQ_Tail], bx                  ; update new tail
        push    bx
	mov	bx, dx				; get entry tail

        add     bx, [ds:wMODQ_Start]                 ; add-in DSP based start addr
	sub	bx, DSP_BASE_ADDR		; normalize it
	shl	bx, 1				; make a byte-wise word-ptr addr
	mov	word ptr ds:[bx], ax		; store the byte (word)

        pop     bx
        mov     [ds:wMODQ_Tail], bx                  ; update new tail

        cmp     dx, [ds:wMODQ_Head]                  ; was buffer empty ?
	pop	ds				; (restore DATA seg ref)
        assume  ds:nothing
	jne	modpw_6				; nope (bne), skip kick start

	mov	al, HDEX_MIDI_OUT_START		; tell DSP to check buffer
	call	dspEx				;  only when necessary

modpw_6:
        pop     dx
        pop     ax
        pop     bx


        ret
        ENDP


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; dspEx : Write a command / exception byte to the DSP.
;
; AL = The command byte to be written.
;
; comm : Carry set on exit if it times out.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

proc    dspEx   near                                     ; void (CF = error)

        push    ds
        push    dx
	push	cx
	push	ax			; stack byte to write

        mov     dx, [default_IO]
	add	dl, HP_CVR		; point at command port

	cli				;--- no ints till after dsp talk fini

	mov	ah, 10			; outer timeout loop counter
	xor	cx, cx			; inner timeout loop counter
dspex1:
	in	al, dx			; get current command status
	test	al, mask HPCVR_HC	; last command complete ?
	jz	dspex2			; yep, get going

        loop	dspex1			; loop if not timed out
	dec	ah			; outer loop
	jnz	dspex1			; keep looping if still not timed-out

	pop	ax			; dump the data

	push	es
	pop	es

	stc				; set carry to show time out
	jmp	short dspex3

dspex2:
	pop	ax			; get the data byte
	out	dx, al			; write it (same port)
	clc				; no error

dspex3:
	sti				;--- ints safe again

	pop	cx
	pop	dx
        pop     ds
        assume  ds:nothing
	ret

        endp




;****************************************************************************
reset_interface PROC
                USES ds,si,di
;                pushf
;                cli
;                mov dx,ECSR
;                mov al,3                ;reset port
;                mov cx,8
;__send_reset:   out dx,al
;                loop __send_reset
;
;                call sysex_wait C,1
;
;                mov dx,ECSR
;                mov al,15h              ;normal operation, IRQ's disabled
;                out dx,al
;
;                POP_F
                ret
                ENDP

;****************************************************************************
;*                                                                          *
;*  MIDI interpreter and related procedures                                 *
;*                                                                          *
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
reset_synth     PROC
;                USES ds,si,di
;                pushf
;                cli
;
;                POP_F
                ret
                ENDP

;****************************************************************************
init_synth      PROC
                USES ds,si,di
                pushf
                cli

                int     1 ; debug int
                mov note_event_l,0
                mov note_event_h,0

                mov di,0
__init_tchans:  mov MIDI_program[di],-1
                mov MIDI_bank[di],0
                inc di
                cmp di,NUM_CHANS
                jne __init_tchans

                mov di,0
__init_patches: mov patch_bank[di],0
                inc di
                cmp di,128
                jne __init_patches

; Added 7-19-1993
; This section will set the initial volume of the proteus

                call send_byte C,0F0h
                call send_byte C,018h
                call send_byte C,004h
                call send_byte C,000h
                call send_byte C,014h
                call send_byte C,03Fh   ; init volume value
                call send_byte C,0F7h
                call sysex_wait C,10h

                POP_F
                ret
                ENDP

;****************************************************************************
send_MIDI_message PROC Stat:BYTE,D1:BYTE,D2:BYTE
                USES ds,si,di           ;Send MIDI Channel Voice message
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
;       our midimapper +1 octave for the drum track
;                cmp     di, 09h         ; is it channel 10?
;                jne     __NotDrums      ; no, we don't care
;
;                add     [D2], 0Ch       ; yes, use our MIDI mapper +1 octave
;
;__NotDrums:
;
                add note_event_l,1      ;yes, update timbre cache LRU counters
                adc note_event_h,0

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

__prg_change:   cmp     di, 09h         ; is it channel 10?
                jne     __send        ; no, we don't care

                jmp __exit

__ctrl_change:  cmp si,PATCH_BANK_SEL
                je __t_bank

                cmp si,CHAN_LOCK                ;(lowest XMIDI control #)
                jb __send
                cmp si,SEQ_INDEX                ;(highest XMIDI control #)
                ja __send                       ;keep XMIDI controls out of
                jmp __exit                      ;MIDI data stream for speed

__t_bank:       mov al,[D2]
                mov MIDI_bank[di],al
                jmp __exit

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

                ret
                ENDP

;****************************************************************************
write_display   PROC H,String:FAR PTR   ;Write string to display (unless NULL)

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
 ;               pushf
 ;               cli
 ;
 ;               POP_F
                ret
                ENDP

;****************************************************************************
unprotect_timbre PROC H,Bank:BYTE,Num:BYTE
                USES ds,si,di           ;Allow a timbre to be replaced
 ;               pushf
 ;               cli
 ;
 ;               POP_F
 ;               ret
                ENDP

;****************************************************************************
timbre_status   PROC H,Bank:BYTE,Num:BYTE
                USES ds,si,di           ;Return 0 if timbre not resident
  ;              pushf
  ;              cli
  ;
  ;              POP_F
                ret
                ENDP

;****************************************************************************
get_cache_size  PROC H
                USES ds,si,di
                pushf
                cli

                mov ax,0                ;no resident cache

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

                mov ax,-1

                POP_F
                ret
                ENDP

;****************************************************************************
install_timbre  PROC H,Bank:BYTE,Num:BYTE,Addr:FAR PTR
                LOCAL sys_seg,sys_dest
                USES ds,si,di
   ;             pushf
   ;             cli
   ;
   ;             POP_F
                ret
                ENDP


                                  
