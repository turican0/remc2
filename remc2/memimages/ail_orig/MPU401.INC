;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
;лл                                                                        лл
;лл   MPU401.INC                                                           лл
;лл                                                                        лл
;лл   IBM Audio Interface Library -- Roland MPU-401 I/O routines           лл
;лл                                                                        лл
;лл   Version 1.00 of 27-Sep-91: Initial version for AIL V2.0 release      лл
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

DATA_IO         dw ?                    ;IO_addr
STAT_COM        dw ?                    ;IO_addr+1

;****************************************************************************
set_IO_parms    PROC IO_ADDR,IRQ,DMA,DRQ
                USES ds,si,di

                mov ax,[IO_ADDR]
                mov DATA_IO,ax
                inc ax
                mov STAT_COM,ax

                ret
                ENDP

;****************************************************************************
detect_device   PROC H,IO_ADDR,IRQ,DMA,DRQ  ;Attempt to detect MPU-401 card
                USES ds,si,di    
                pushf
                cli

                push DATA_IO              ;preserve current I/O address
                push STAT_COM

                call set_IO_parms C,[IO_ADDR],[IRQ],[DMA],[DRQ]

                call reset_interface      ;attempt to reset card

                pop STAT_COM
                pop DATA_IO

                POP_F                     ;return AX=0 if not found
                ret
                ENDP

;****************************************************************************
send_byte       PROC DataByte:BYTE

                mov dx,STAT_COM
__wait_cts:     in al,dx
                test al,40h
                je __write_it
                test al,80h
                jne __wait_cts
                sti                     ;if MIDI byte incoming, give receiving
                jmp $+2                 ;process a chance to catch it ...
                jmp $+2                 ;prevents lost data and/or hangups
                jmp $+2                 ;during Cakewalk and MIDIECHO
                mov dx,DATA_IO
                in al,dx
                mov dx,STAT_COM
                jmp __wait_cts

__write_it:     mov dx,DATA_IO
                mov al,[DataByte]
                out dx,al

                ret
                ENDP

;****************************************************************************
send_cmd        PROC CmdByte:BYTE

                pushf
                cli
                mov dx,STAT_COM
                mov cx,-1
__wait_cts_1:   in al,dx
                test al,40h
                jz __cts_1
                loop __wait_cts_1
                jmp __exit_bad

__cts_1:        mov al,[CmdByte]
                out dx,al

                mov cx,-1
__wait_ack:     mov dx,STAT_COM
                in al,dx
                test al,80h
                jnz __next_loop
                mov dx,DATA_IO
                in al,dx
                cmp al,0feh
                je __exit_OK
__next_loop:    loop __wait_ack

__exit_bad:     mov ax,0
                jmp __exit
__exit_OK:      mov ax,1
__exit:         POP_F
                ret
                ENDP

;****************************************************************************
reset_interface PROC                    ;Returns 1 if MPU interface available
                USES ds,si,di
                pushf                   ;don't allow interruption
                cli

                call send_cmd C,0ffh    ;reset MPU-401 interface
                or ax,ax
                jne __exit_reset        ;(worked the first time)

                call send_cmd C,0ffh    ;try again in case UART mode active

__exit_reset:   POP_F
                ret
                ENDP

;****************************************************************************
init_interface  PROC                    ;Init MPU-401-compatible interface
                USES ds,si,di
                pushf                   ;don't allow interruption
                cli

                call send_cmd C,03fh    ;assert UART mode

                POP_F
                ret

;****************************************************************************
                ENDP
