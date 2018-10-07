;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
;лл                                                                        лл
;лл   CAKEPORT.ASM                                                         лл
;лл                                                                        лл
;лл   IBM Audio Interface Library                                          лл
;лл                                                                        лл
;лл   Audio Interface Library driver for Cakewalk Professional version 4.0 лл
;лл                                                                        лл
;лл   Version 1.0 of 08-Aug-91: Initial version                            лл
;лл           2.0 of 02-Sep-91: MPU-401 MIDI receive capability added      лл
;лл                             MIDI.DRV support removed                   лл
;лл                             Dialog box support added                   лл
;лл                             Timer subclass code added                  лл
;лл          2.01 of 10-Dec-91: Timer subclass code fixed for .WRK loading лл
;лл          2.02 of 20-Apr-92: serve_driver() enabled for all drivers     лл
;лл          2.03 of 17-Aug-92: Driver-specific volume scaling added       лл
;лл                                                                        лл
;лл   Author: John Miles                                                   лл
;лл   8086 ASM source compatible with Turbo Assembler v2.0 or later        лл
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
                JUMPS                   ;Auto jump sizing

                INCLUDE ail.inc
                INCLUDE ail.mac

ADDRESS_CRTC    MACRO                   ;Get DX=CRTC address at 3B0h (mono)
                push ax                 ;or 3D0h (color) -- preserves all regs
                mov dx,03cch            
                in al,dx
                and al,1
                shl al,1
                shl al,1
                shl al,1
                shl al,1
                shl al,1
                mov dx,03b0h
                add dl,al
                pop ax
                ENDM

                .CODE

                db 'IPD',10h,00h,82h,00h,00h  

                jmp startup             ;Cakewalk function 0
                jmp shutdown            ;1
                jmp clear_to_send       ;2
                jmp send_bytes          ;3
                jmp data_in_queue       ;4
                jmp read_queue          ;5
                jmp get_device_data     ;6
                      
devname         db 'Audio Interface Library CAKEPORT Driver v2.0',0
                db '   MPU-401: "1" to enable, "2" to disable',0
MIDI_active     dw 1h
                db '             Base port address (hex)',0
def_IO          dw 330h                        
                db '             IRQ number',0
def_IRQ         dw 2h
                db 'AIL driver: "1" to enable, "2" to disable',0
AIL_active      dw 1h
AIL_IO_text     db '             Base port address (hex)',0
def_AIL_IO      dw 388h
AIL_IRQ_text    db '             IRQ number',0
def_AIL_IRQ     dw 7
                db 0

intro_box       LABEL BYTE
                db ' IBM Audio Interface Library '
                db '@@'
                db ' Cakewalk(TM) Extended MIDI Driver '
                db '@@'
                db ' Version 2.03 of '
                db ??date
                db ' '
                db '@@@'
                db ' Copyright (C) 1991, 1992 Miles Design, Inc. '
                db 0

ERR_no_ADV      LABEL BYTE
                db ' AIL ERROR: XMIDI.ADV driver not found @'
                db '@'
                db ' To use an Audio Interface Library XMIDI driver from @'
                db ' within Cakewalk, you must save the desired .ADV @'
                db ' driver file in your Cakewalk driver directory under @'
                db ' the special filename XMIDI.ADV.  For example, to @'
                db ' use the AIL Ad Lib driver with Cakewalk, you should @'
                db ' issue a DOS command similar to the following: @'
                db '@'
                db ' C:\>copy \ail\adlib.adv \cakewalk\xmidi.adv '
                db '@'
                db 0

ERR_bad_tr      LABEL BYTE
                db ' AIL WARNING: Patch ~0 not found in bank ~1 of @'
                db '              Global Timbre Library file ~A @'
                db '@'
                db ' Make sure the correct XMIDI Patch Bank Select @'
                db ' controller (~2) value appears before each MIDI @'
                db ' Patch (Program Change) event.  Also, verify that @'
                db ' each custom timbre you',39,'re using is specified in @'
                db ' the GLIB catfile, and that the ~A file in @'
                db ' your Cakewalk driver directory is up to date. '
                db 0

ERR_bad_rk      LABEL BYTE
                db ' AIL WARNING: Patch for rhythm key #~0 not found in @'
                db '              bank ~1 of Global Timbre Library file @'
                db '              ~A @'
                db '@'
                db ' The AIL driver currently in use emulates the Roland @'
                db ' MT-32 rhythm keys on MIDI channel 10 by playing the @'
                db ' timbres in bank 127 of the Global Timbre Library. @'
                db ' To hear MIDI key ~0 play a rhythm sound, you must @'
                db ' specify a timbre of the form timbre(127,~0) = ... @'
                db ' in the selected synthesizer',39,'s GLIB catfile. '
                db 0

ERR_no_dev      LABEL BYTE
                db ' AIL ERROR: Sound hardware not found @'
                db '@'
                db ' The Audio Interface Library driver could not @'
                db ' locate your sound adapter or synthesizer interface. @'
                db ' You must restart CAKEPRO with the /S option and @'
                db ' choose menu option 3 (MIDI Interface Setup) to @'
                db ' specify the I/O location at which the driver @'
                db ' should address your hardware. '
                db 0

ERR_no_GTL      LABEL BYTE
                db ' AIL WARNING: Patch ~0, bank ~1 requested but no @'
                db '              Global Timbre Library file available @'
                db '@'
                db ' You should use the GLIB (Global LIBrarian) program @'
                db ' to create a Global Timbre Library file which contains @'
                db ' this timbre.  The Global Timbre Library file for this @'
                db ' synthesizer should be saved in the Cakewalk directory @'
                db ' under the special filename ~A. '
                db 0

ERR_GTL_size    LABEL BYTE
                db ' AIL WARNING: Patch ~0, bank ~1 not resident in first @'
                db '              32K bytes of Global Timbre Library file @'
                db '              ~A @'
                db '@'
                db ' Only the first 32,768 bytes of a Global Timbre Library @'
                db ' file may be loaded by the Cakewalk driver.  You will @'
                db ' need to use the MIDIECHO program instead if this error @'
                db ' occurs. '
                db 0

dummy_XMI       LABEL BYTE
                db 046h,04Fh,052h,04Dh,000h,000h,000h,00Eh,058h,044h,049h
                db 052h,049h,04Eh,046h,04Fh
                db 000h,000h,000h,002h,001h,000h,043h,041h,054h,020h,000h
                db 000h,000h,02Eh,058h,04Dh
                db 049h,044h,046h,04Fh,052h,04Dh,000h,000h,000h,022h,058h
                db 04Dh,049h,044h,045h,056h
                db 04Eh,054h,000h,000h,000h,016h,0FFh,059h,002h,000h,000h
                db 0FFh,051h,003h,009h,027h
                db 0C0h,0FFh,058h,004h,004h,002h,018h,008h,0FFh,02Fh,000h
                db 000h

AIL_fn          db 'XMIDI.ADV',0      ;AIL driver for output
GTL_fn          db 'XMIDI.'           ;AIL Global Timbre Library filename
GTL_suffix_l    dw ?
GTL_suffix_h    dw ?

GTL_seg         dw ?
AIL_vectors     dd ?

desc_addr       dd ?

drvr_desc       STRUC
min_API_version dw ?
drvr_type       dw ?
data_suffix     db 4 dup (?)
dev_names       dd ?
dev_IO          dw ?
dev_IRQ         dw ?
dev_DMA         dw ?
dev_DRQ         dw ?
svc_rate        dw ?
dsp_size        dw ?
                ENDS

t_index         STRUC
num             db ?
bank            db ?
                ENDS

GTL_hdr         STRUC
tag             t_index <>
offset_l        dw ?
offset_h        dw ?
                ENDS

AIL_IO          dw ?
AIL_IRQ         dw ?
AIL_DMA         dw ?
AIL_DRQ         dw ?

cv_status       dw ?
cv_bytecnt      dw ?
cv_data         db 8 dup (?)            ;Channel Voice data stream

timbre_bank     db 16 dup (?)
timbre_num      db 16 dup (?)

init_prg        db -1,68,48,95,78,41,3,110,122,-1,-1,-1,-1,-1,-1,-1
RBS             db 0,0,0,0,0,0,0,0,0,127,0,0,0,0,0,0

default_ISR_o   dw -1
default_ISR_s   dw -1
orig_PIC        dw -1
head            dw 0
tail            dw 0

queue           db 256 dup (?)          ;incoming message queue

app_ds          dw ?                            
app_sp          dw ?
callback        LABEL DWORD
callback_o      dw ?                            
callback_s      dw ?                            
IRQ_busy        dw ?
old_ss          dw ?
old_sp          dw ?

AIL_rate        dw ?                    ;AIL driver service rate*10 (120 Hz)
CW_RATE         EQU 2913                ;Cakewalk default timer rate (291 Hz)
Cake_INT8       dd ?
timer_trapped   dw ?
timer_busy      dw ?
timer_accum     dw ?

CRTC_port       dw ?
dialog_page     dw ?
page_size       dw ?
page_lines      dw ?
page_cols       dw ?
text_seg        dw ?
text_width      db 50 dup (?)
mouse_active    dw ?

driver_vol      dw ?

accums          dw 10 dup (?)
strbuf          db 80 dup (?)

;*****************************************************************************
AIL_base        db 20480 dup (?)        ;reserve 20K for AIL driver
AIL_timbs       db 4096 dup (?)         ;reserve 4K for AIL timbre cache
GTL_base        db 32768 dup (?)        ;reserve 32K for Global Timbre Library
;*****************************************************************************

;*****************************************************************************
init_dialog     PROC                    ;Init text dialog box routines
                USES ds,si,di

                mov dialog_page,1   

                ADDRESS_CRTC
                mov CRTC_port,dx
                mov ax,0b800h
                cmp dx,03d0h
                je __set_scrn_seg
                mov ax,0b000h
__set_scrn_seg: mov text_seg,ax

                mov ax,40h
                mov es,ax
                mov bx,es:[4ah]         ;get # of text cols
                mov page_cols,bx
                mov al,es:[84h]         ;get # of text rows
                mov ah,0
                inc ax
                mov page_lines,ax
                shl bx,1
                mul bx
                mov page_size,ax

                mov ax,24h
                mov bx,-1
                int 33h
                mov ax,0
                cmp bh,-1
                je __set_mouse
                mov ax,1
__set_mouse:    mov mouse_active,ax

                ret
                ENDP

;*****************************************************************************
copy_page       PROC Src,Dest           ;src|dest=0 for main page, else page #
                USES ds,si,di
                cld

                mov dx,CRTC_port        ;wait for vsync leading to
                add dx,10               ;avoid flicker
__vsync:	in al,dx
	test al,8
	jnz __vsync
__not_vsync:    in al,dx
                test al,8
                jz __not_vsync

                mov ds,text_seg
                mov es,text_seg

                mov ax,page_size
                mul [Src]
                mov si,ax
                mov ax,page_size
                mul [Dest]
                mov di,ax

                mov cx,page_size
                rep movsw

                ret
                ENDP

;*****************************************************************************
bascalc         PROC HTab,VTab,Page
                USES ds,si,di

                mov ax,page_cols
                shl ax,1
                mul [VTab]
                mov bx,[HTab]
                shl bx,1
                add bx,ax       

                mov ax,page_size
                mul [Page]
                add bx,ax

                mov ax,bx
                mov dx,text_seg

                ret                     ;AX=offset DX=segment
                ENDP

;*****************************************************************************
dialog_escape   PROC EscCode:BYTE       ;Dialog escape string to buffer
                USES ds,si,di

                mov al,[EscCode]
                cmp al,'A'              ;'A': XMIDI GTL filename
                je __GTL_fn

                sub al,'0'              ;'0' - '9': Numeric accumulator
                mov ah,0
                mov di,ax
                shl di,1
                call decstr C,OFFSET strbuf,cs,accums[di]
                jmp __exit

__GTL_fn:       push cs
                pop ds
                lea si,GTL_fn
                jmp __fetch_str

__fetch_str:    lea di,strbuf
                push cs
                pop es
__strcpy:       lodsb
                stosb
                cmp al,0
                jne __strcpy

__exit:         ret
                ENDP

;*****************************************************************************
decstr          PROC Buf:FAR PTR,Num    ;decimal ASCII to string buffer
                LOCAL accum,lzero
                USES ds,si,di
                cld

                les di,[Buf]
                mov ax,[Num]
                mov accum,ax
                mov lzero,0

                mov cx,10000
__div_loop:     mov ax,accum
                mov dx,0
                div cx
                mov accum,dx
                add al,'0'

                cmp al,'0'
                jne __write_digit
                cmp lzero,0
                je __next_digit

__write_digit:  mov lzero,1
                stosb

__next_digit:   mov ax,cx
                mov dx,0
                mov bx,10
                div bx
                mov cx,ax
                cmp ax,0
                jne __div_loop

                cmp lzero,0
                jne __end_string
                mov al,'0'
                stosb

__end_string:   mov BYTE PTR es:[di],0

                ret
                ENDP

;*****************************************************************************
hide_mouse      PROC                    ;hide mouse if mouse in use
                USES ds,si,di

                cmp mouse_active,0
                je __exit

                mov ax,2                
                int 33h

__exit:         ret
                ENDP

;*****************************************************************************
show_mouse      PROC                    ;show mouse if mouse in use
                USES ds,si,di

                cmp mouse_active,0
                je __exit

                mov ax,1
                int 33h

__exit:         ret
                ENDP


;*****************************************************************************
open_dialog     PROC TextStr,BColor:BYTE,FColor:BYTE,CJustify
                LOCAL width,height,text_top,text_left
                LOCAL box_top,box_left,box_right,box_bottom
                USES ds,si,di
                cld

                call hide_mouse
                call copy_page C,0,dialog_page

                push cs
                pop ds
                mov si,[TextStr]

                mov width,0
                mov bx,0
__scan_line:    mov cx,0
__scan_txt:     lodsb
                mov text_width[bx],cl
                cmp al,0
                je __end_txt
                cmp al,'@'
                je __end_line
                cmp al,'~'
                je __escape
                inc cx
__bump_width:   cmp cx,width
                jb __scan_txt
                mov width,cx
                jmp __scan_txt
__end_line:     inc bx
                jmp __scan_line
__escape:       lodsb
                push cx
                push bx
                call dialog_escape C,ax
                pop bx
                pop cx
                mov di,-1
__find_strlen:  inc di
                cmp strbuf[di],0
                jne __find_strlen
                add cx,di
                jmp __bump_width
__end_txt:      inc bx
                mov height,bx

                mov ax,page_lines
                sub ax,height
                shr ax,1
                mov text_top,ax
                sub ax,1
                mov box_top,ax
                add ax,height
                add ax,1
                mov box_bottom,ax
                mov ax,page_cols
                sub ax,width
                shr ax,1
                mov text_left,ax
                sub ax,1
                mov box_left,ax
                add ax,width
                add ax,1
                mov box_right,ax

                mov si,box_left
                mov di,box_top
__draw_box:     call bascalc C,si,di,0
                mov es,dx
                mov bx,ax
                cmp di,box_top
                je __top_char
                cmp di,box_bottom
                je __btm_char
                cmp si,box_left
                je __side_char
                cmp si,box_right
                je __side_char
                mov al,' '
                jmp __put_char
__side_char:    mov al,'К'
                jmp __put_char
__top_char:     mov al,'Щ'
                cmp si,box_left
                je __put_char
                mov al,'Л'
                cmp si,box_right
                je __put_char
                mov al,'Э'
                jmp __put_char
__btm_char:     mov al,'Ш'
                cmp si,box_left
                je __put_char
                mov al,'М'
                cmp si,box_right
                je __put_char
                mov al,'Э'
__put_char:     mov es:[bx],al
                mov al,[BColor]
                shl al,1
                shl al,1
                shl al,1
                shl al,1
                or al,[FColor]
                mov es:[bx+1],al
                inc si
                cmp si,box_right
                jbe __draw_box
                mov si,box_left
                inc di
                cmp di,box_bottom
                jbe __draw_box
                
                mov di,0
                mov si,[TextStr]
__for_line:     call bascalc C,text_left,text_top,0
                mov bx,ax
                mov es,dx
                cmp [CJustify],0
                je __for_char
                mov bx,width
                sub bl,text_width[di]
                and bx,11111110b
                add bx,ax
__for_char:     lodsb
                cmp al,0
                je __text_done
                cmp al,'@'
                je __newline
                cmp al,'~'
                je __prt_escape
                mov es:[bx],al
                add bx,2
                jmp __for_char
__newline:      inc di
                inc text_top
                jmp __for_line
__prt_escape:   lodsb
                push di
                push si
                push bx
                push es
                call dialog_escape C,ax
                pop es
                pop bx
                mov si,-1
__dump_str:     inc si
                mov al,strbuf[si]
                cmp al,0
                je __end_esc
                mov es:[bx],al
                add bx,2
                jmp __dump_str
__end_esc:      pop si
                pop di
                jmp __for_char

__text_done:    call show_mouse

                inc dialog_page
                ret                     
                ENDP

;*****************************************************************************
close_dialog    PROC
                USES ds,si,di

                dec dialog_page

                call hide_mouse
                call copy_page C,dialog_page,0
                call show_mouse

                ret
                ENDP

;*****************************************************************************
dialog_prompt   PROC Timeout
                LOCAL ticks,curval
                USES ds,si,di

                mov ticks,0
                mov curval,0

__wait:         cmp [Timeout],-1
                je __chk_kbd
                mov ax,40h
                mov es,ax
                mov ax,es:[6ch]
                cmp ax,curval
                je __chk_kbd
                mov curval,ax
                inc ticks               ;tick cnt +/- 1
                mov ax,ticks
                cmp ax,[Timeout]
                je __exit

__chk_kbd:      mov ax,1100h            ;key struck?
                int 16h
                jz __chk_mouse
                
                mov ax,0h               ;yes, clear keystroke and return
                int 16h
                jmp __exit

__chk_mouse:    cmp mouse_active,0
                je __wait

                mov ax,3                ;mouse clicked?
                int 33h
                and bx,111b
                jz __wait

                push bx                 ;yes, wait for release and return
__clear_mouse:  mov ax,3
                int 33h
                and bx,111b
                jnz __clear_mouse
                pop ax

__exit:         ret                     ;return AX=ASC key, mouse code (0-7),
                ENDP                    ;or # of ticks in [Timeout]

;*****************************************************************************
send_cmd        PROC CmdByte

                pushf
                cli

                mov dx,def_IO
                inc dx
                mov cx,-1
__wait_cts_1:   in al,dx
                test al,40h
                jz __cts_1
                loop __wait_cts_1
                jmp __exit_bad

__cts_1:        mov ax,[CmdByte]
                out dx,al

                mov cx,-1
__wait_ack:     in al,dx
                test al,80h
                jnz __next_loop
                dec dx
                in al,dx
                inc dx
                cmp al,0feh
                je __exit_OK
__next_loop:    loop __wait_ack

__exit_bad:     mov ax,0
                jmp __exit
__exit_OK:      mov ax,1
__exit:         POP_F
                ret
                ENDP

;*****************************************************************************
report_error    PROC ErrTxt
                USES ds,si,di

                call open_dialog C,[ErrTxt],4,15,0
                call dialog_prompt C,-1
                call close_dialog

                ret
                ENDP

;*****************************************************************************
load_file       PROC Filename:FAR PTR,Addr,MaxLen
                LOCAL file_seg
                USES ds,si,di           ;Addr: offset of file space in CS

                mov ax,cs
                mov dx,0
                REPT 4
                shl ax,1
                rcl dx,1
                ENDM
                add ax,[Addr]
                adc dx,0
                REPT 4
                shr dx,1
                rcr ax,1
                ENDM
                inc ax
                mov file_seg,ax

                lds dx,[Filename]
                mov ax,3d00h
                int 21h
                jc __error
                push ax

                mov bx,ax
                mov ah,3fh
                mov cx,[MaxLen]
                mov dx,0
                mov ds,file_seg
                int 21h

                pop bx
                mov ah,3eh
                int 21h

                mov ax,file_seg
                ret                     ;return segment address of file

__error:        mov ax,0
                ret

                ENDP

;*****************************************************************************
find_proc       PROC                    ;Return DX:AX -> function AX
                les bx,AIL_vectors      ;ES:BX -> driver procedure table

__find_proc:    mov cx,es:[bx]          ;search for requested function in 
                cmp cx,ax               ;driver procedure table
                je __found
                add bx,4
                cmp cx,-1               
                jne __find_proc

                mov ax,0                ;return 0: function not available
                mov dx,0
                ret

__found:        mov ax,es:[bx+2]        ;get offset from start of driver
                mov dx,es               ;get segment of driver (org = 0)
                ret

                ENDP

;*****************************************************************************
AIL             PROC                    ;Call function AX in AIL driver
                                        ;(Warning: re-entrant procedure!)
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
AIL_describe_driver PROC

                mov ax,AIL_DESC_DRVR
                jmp AIL

                ENDP

;*****************************************************************************
AIL_detect_device PROC

                mov ax,AIL_DET_DEV
                jmp AIL

                ENDP

;*****************************************************************************
AIL_init_driver PROC

                mov ax,AIL_INIT_DRVR
                jmp AIL

                ENDP

;*****************************************************************************
AIL_define_timbre_cache PROC

                mov ax,AIL_DEFINE_T_CACHE
                jmp AIL

                ENDP

;*****************************************************************************
AIL_install_timbre PROC

                mov ax,AIL_INSTALL_T
                jmp AIL

                ENDP

;*****************************************************************************
AIL_timbre_status PROC

                mov ax,AIL_T_STATUS
                jmp AIL

                ENDP

;*****************************************************************************
AIL_shutdown_driver PROC

                mov ax,AIL_SHUTDOWN_DRVR
                jmp AIL

                ENDP

;*****************************************************************************
AIL_send_channel_voice_message PROC

                mov ax,AIL_SEND_CV_MSG
                jmp AIL

                ENDP

;*****************************************************************************
AIL_serve_driver PROC

                mov ax,AIL_SERVE_DRVR
                jmp AIL

                ENDP

;*****************************************************************************
AIL_register_sequence PROC

                mov ax,AIL_REG_SEQ
                jmp AIL

                ENDP

;*****************************************************************************
AIL_relative_volume PROC

                mov ax,AIL_REL_VOL
                jmp AIL

                ENDP

;*****************************************************************************
AIL_release_sequence_handle PROC

                mov ax,AIL_REL_SEQ_HND
                jmp AIL

                ENDP

;*****************************************************************************
AIL_INT8        PROC                    ;Perform serve_driver() calls to AIL
                                        ;driver for possible TVFX maintenance
                cmp timer_busy,0     
                je __no_reentry
                jmp __serve_Cake

__no_reentry:   mov timer_busy,1        ;avoid re-entry or undesirable calls

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

                mov ax,AIL_rate
                add timer_accum,ax
                cmp timer_accum,CW_RATE
                jb __restore_regs
                sub timer_accum,CW_RATE

                call AIL_serve_driver
                         
__restore_regs: pop ds
                pop es
                pop bp
                pop di
                pop si
                pop dx
                pop cx
                pop bx
                pop ax

                mov timer_busy,0

__serve_Cake:   pushf
                call [Cake_INT8]

                iret
                ENDP

;*****************************************************************************
setup_timbre    PROC Bank,Num
                USES ds,si,di

                call AIL_timbre_status C,0,[Bank],[Num]
                cmp ax,0
                jne __exit              ;timbre already installed or not 
                                        ;needed by synthesizer, exit
                mov ax,[Num]
                mov accums[0],ax
                mov ax,[Bank]
                mov accums[2],ax
                mov accums[4],PATCH_BANK_SEL

                cmp GTL_seg,0   
                jne __GTL_valid
                call report_error C,OFFSET ERR_no_GTL
                jmp __exit

__GTL_valid:    mov ds,GTL_seg
                mov si,0

                mov bl,BYTE PTR [Num]
                mov bh,BYTE PTR [Bank]

__chk_entry:    mov al,[si].tag.num
                mov ah,[si].tag.bank

                cmp ah,-1               
                je __not_found          ;end of GTL reached, timbre not found

                cmp bx,ax
                je __timbre_found       ;bank and num match, timbre found

                add si,SIZE GTL_hdr
                jmp __chk_entry

__timbre_found: cmp [si].offset_h,0
                jne __GTL_too_big       ;timbre not in resident GTL image

                mov si,[si].offset_l

                mov ax,si               ;DS:AX -> timbre
                add ax,[si]             ;add timbre length
                cmp ax,SIZE GTL_base
                ja __GTL_too_big        ;timbre not in resident GTL image

                call AIL_install_timbre C,0,[Bank],[Num],si,ds

__exit:         ret

__not_found:    cmp bh,127
                je __bad_rk
                call report_error C,OFFSET ERR_bad_tr
                jmp __exit
__bad_rk:       call report_error C,OFFSET ERR_bad_rk
                jmp __exit

__GTL_too_big:  call report_error C,OFFSET ERR_GTL_size
                jmp __exit

                ENDP

;*****************************************************************************
intercept_timbre_request PROC Stat,D1,D2
                USES ds,si,di

                mov di,[Stat]
                mov dx,di
                mov ax,[D1]             ;AX = 1st data byte
                mov ah,0
                mov cx,[D2]             ;CX = 2nd data byte
                mov ch,0
                and dx,0f0h             ;DX = status
                and di,00fh             ;DI = channel #

                cmp dx,90h              ;NOTE ON
                je __note_on

                cmp dx,0b0h             ;CONTROL CHANGE
                je __ctrl_chg

                cmp dx,0c0h             ;PROGRAM CHANGE
                je __prg_chg
                jmp __exit

__note_on:      jcxz __exit

                mov bl,RBS[di]
                mov bh,0
                or bx,bx
                je __exit
                call setup_timbre C,bx,ax
                jmp __exit

__prg_chg:      mov timbre_num[di],al
                mov ah,0
                mov bl,timbre_bank[di]
                mov bh,0
                call setup_timbre C,bx,ax
                jmp __exit

__ctrl_chg:     cmp ax,PART_VOLUME
                je __volume
                cmp ax,PATCH_BANK_SEL
                je __PBS
                jmp __exit

__volume:       mov ax,driver_vol       ;pass scaled volume value
                mov bx,100
                mul cx
                div bx
                ret

__PBS:          mov timbre_bank[di],cl

__exit:         mov ax,[D2]             ;(normally pass D2 unchanged)
                ret
                ENDP

;*****************************************************************************
put_MIDI_byte   PROC Data:BYTE          ;Send CV messages to AIL driver
                USES ds,si,di

                mov al,[Data]
                mov ah,0

                cmp ax,80h              ;status or data?
                jb __data
                cmp ax,0f0h             ;channel voice status?
                jb __new_status

                cmp ax,0f8h             ;system real-time?
                jae __exit              ;yes, ignore

                mov cv_bytecnt,0
                mov cv_status,0         ;else clear running status
                jmp __exit              ;...and exit

__new_status:   mov cv_status,ax        
                mov cv_bytecnt,0
                jmp __exit

__data:         mov dx,cv_status
                cmp dx,80h
                jb __exit               ;invalid status, ignore incoming data

                mov bx,cv_bytecnt       ;write data byte to xmit queue
                cmp bx,8
                jae __new_status
                mov cv_data[bx],al
                inc cv_bytecnt          

                and dx,0f0h             ;mask out channel number
                mov cx,1               
                cmp dx,0c0h             ;Program Change?
                je __chk_complete       ;yes, 1 data byte needed
                cmp dx,0d0h             ;Channel Pressure?
                je __chk_complete       ;yes, 1 data byte needed
                mov cx,2                ;else 2 data bytes needed to xmit

__chk_complete: cmp cv_bytecnt,cx
                jb __exit               ;message not complete, exit

                mov cv_bytecnt,0        ;else send it to the AIL driver

                mov cx,cv_status
                mov ch,0
                mov ax,cx
                and ax,0fh
                sub ax,09h
                jbe __norm_channel

                and cl,0f0h             ;translate chs. 11-16 to 2-7
                or cl,al                ;for AIL locked-channel simulation

__norm_channel: mov al,cv_data[0]
                mov ah,0
                mov si,ax
                mov al,cv_data[1]
                mov ah,0
                mov di,ax

                inc timer_busy
                inc IRQ_busy            ;avoid callback function re-entry
                push cx               
                call intercept_timbre_request C,cx,si,di
                mov di,ax

                pop cx
                call AIL_send_channel_voice_message C,0,cx,si,di
                dec IRQ_busy
                dec timer_busy

__exit:         ret
                ENDP

;*****************************************************************************
queue_byte      PROC Data
                USES ds,si,di

                pushf
                cli

                mov ax,[Data]
                mov di,head
                mov queue[di],al
                inc head
                cmp head,256
                jne __exit
                mov head,0

__exit:         POP_F
                ret
                ENDP

;*****************************************************************************
stuff_byte      PROC                    ;Read byte, insert into queue
                USES ds,si,di

                cmp IRQ_busy,0
                jne __EOI
                mov IRQ_busy,1

                mov al,20h
                out 20h,al
                sti

__notify:       mov ds,app_ds
                mov es,app_ds
                mov old_ss,ss
                mov old_sp,sp
                mov ax,app_sp
                or ax,ax
                jz __callback
                mov ss,app_ds
                mov sp,ax
__callback:     clc
                call [callback]
                mov ss,old_ss
                mov sp,old_sp
                mov ax,tail
                cmp ax,head
                jne __notify

                mov IRQ_busy,0
                jmp __exit

__EOI:          mov al,20h
                out 20h,al

__exit:         ret
                ENDP

;*****************************************************************************
MPU_ISR         PROC                    ;Interrupt service routine for MPU-401
                
                push dx
                push ax
                cld

                mov dx,def_IO
                inc dx
                in al,dx
                test al,80h
                jnz __EOI

__read_byte:    mov dx,def_IO
                in al,dx
                call queue_byte C,ax

                mov dx,def_IO
                inc dx
                in al,dx
                test al,80h
                jz __read_byte

                push bx
                push cx
                push es
                call stuff_byte C
                pop es
                pop cx 
                pop bx
                jmp __exit

__EOI:          mov al,20h
                out 20h,al

__exit:         pop ax
                pop dx

                iret
                ENDP

;*****************************************************************************
                ;
                ;void startup(void far *callback, int stack_base, int stack)
                ;            

startup         PROC

                push bp
                mov bp,sp

                call init_dialog

                call open_dialog C,OFFSET intro_box,3,0,1
                call dialog_prompt C,60
                call close_dialog

                mov ax,ds
                mov app_ds,ax                 
                mov ax,[bp+06h]                   
                mov dx,[bp+08h]                   
                mov callback_o,ax                 
                mov callback_s,dx                 
                mov ax,[bp+0Ah]                   
                add ax,[bp+0Ch]                   
                mov app_sp,ax                 

                mov timer_trapped,0
                mov timer_busy,0

                xor MIDI_active,2
                xor AIL_active,2

                mov default_ISR_o,-1    ;initialize MPU input queue
                mov default_ISR_s,-1
                mov orig_PIC,-1
                mov head,0
                mov tail,0
                mov IRQ_busy,0

                cmp AIL_active,0
                je __chk_MIDI

                mov di,0
__init_timbs:   mov timbre_bank[di],0
                mov timbre_num[di],0
                inc di
                cmp di,16
                jne __init_timbs

                lea ax,AIL_base
                lea bx,AIL_fn
                call load_file C,bx,cs,ax,SIZE AIL_base
                cmp ax,0
                jne __loaded

                call report_error C,OFFSET ERR_no_ADV
                mov ax,0
                jmp __exit

__loaded:       mov es,ax
                mov bx,es:[0]
                mov WORD PTR AIL_vectors,bx
                mov WORD PTR AIL_vectors+2,ax

                call AIL_describe_driver C,0
                mov es,dx
                mov bx,ax
                mov ax,def_AIL_IO
                mov AIL_IO,ax
                mov ax,def_AIL_IRQ
                mov AIL_IRQ,ax
                mov ax,es:[bx].dev_DMA
                mov AIL_DMA,ax
                mov ax,es:[bx].dev_DRQ
                mov AIL_DRQ,ax
                mov ax,es:[bx].svc_rate
                cmp ax,-1
                je __set_svc_rate
                mov dx,10
                mul dx
__set_svc_rate: mov AIL_rate,ax

                mov ax,WORD PTR es:[bx].data_suffix
                mov GTL_suffix_l,ax
                mov ax,WORD PTR es:[bx].data_suffix+2
                mov GTL_suffix_h,ax

                call AIL_detect_device C,0,AIL_IO,AIL_IRQ,AIL_DMA,AIL_DRQ
                cmp ax,0
                jne __AIL_found

                call report_error C,OFFSET ERR_no_dev
                mov ax,0
                jmp __exit

__AIL_found:    call AIL_init_driver C,0,AIL_IO,AIL_IRQ,AIL_DMA,AIL_DRQ

                call AIL_register_sequence C,0,OFFSET dummy_XMI,cs,0,\
                        OFFSET AIL_timbs,cs,0,0
                push ax
                call AIL_relative_volume C,0,ax
                mov driver_vol,ax

                pop ax
                call AIL_release_sequence_handle C,0,ax

                mov cv_status,0         ;invalidate running status
                mov cv_bytecnt,0        ;clear data stream

                lea ax,GTL_base
                lea bx,GTL_fn
                call load_file C,bx,cs,ax,SIZE GTL_base
                mov GTL_seg,ax

                call AIL_define_timbre_cache C,0,OFFSET AIL_timbs,cs,\
                     SIZE AIL_timbs

                mov di,0
__setup_timbs:  mov al,timbre_bank[di]
                mov ah,0
                mov bl,init_prg[di]
                mov bh,0
                cmp bl,-1
                je __next_timb
                call setup_timbre C,ax,bx
__next_timb:    inc di
                cmp di,16
                jne __setup_timbs

__chk_MIDI:     mov ax,1
                cmp MIDI_active,0
                je __exit

                call send_cmd C,0ffh
                or ax,ax
                jne __init_OK
                call send_cmd C,0ffh
                or ax,ax
                je __exit

__init_OK:      call send_cmd C,03fh    ;enable UART mode

                push ds
                push si
                pushf                   ;install MPU input handler, enable
                cli                     ;incoming interrupts
                mov ax,0
                mov es,ax
                mov bx,def_IRQ
                add bx,8
                shl bx,1
                shl bx,1
                lds si,es:[bx]
                mov default_ISR_o,si
                mov default_ISR_s,ds
                lea si,MPU_ISR
                mov es:[bx],si
                mov es:[bx+2],cs
                in al,21h
                mov orig_PIC,ax
                mov bx,1
                mov cx,def_IRQ
                shl bx,cl
                not bx
                and ax,bx               ;enable IRQs from MPU-401
                out 21h,al
                jmp $+2
                POP_F
                pop si
                pop ds

                mov ax,1

__exit:         pop bp
                ret
                ENDP

;*****************************************************************************
                ;
                ;void shutdown(void)
                ;

shutdown        PROC
       
                cmp AIL_active,0
                je __chk_MIDI
                call AIL_shutdown_driver C,0,0,0

__chk_MIDI:     cmp MIDI_active,0
                je __exit

                call send_cmd C,0ffh    ;reset the MPU-401 interface

                mov dx,default_ISR_s
                cmp dx,-1
                je __exit
                mov ax,default_ISR_o
                pushf                   ;replace old IRQ handler
                cli                   
                mov cx,0
                mov es,cx
                mov bx,def_IRQ
                add bx,8
                shl bx,1
                shl bx,1
                mov es:[bx],ax
                mov es:[bx+2],dx
                mov ax,orig_PIC
                out 21h,al
                jmp $+2
                POP_F
                                        
__exit:         ret
                ENDP

;*****************************************************************************
                ;
                ;int clear_to_send(void)
                ;

clear_to_send   PROC

                mov ax,0
                cmp MIDI_active,0
                je __exit

                mov dx,def_IO
                inc dx
                in al,dx
                test al,40h
                mov ax,0
                jnz __exit
                mov ax,1

__exit:         ret 
                ENDP
                               
;*****************************************************************************
                ;
                ;void send_bytes(int byte1, int byte2, ...)
                ;

send_bytes      PROC

                cmp AIL_active,0
                je __exit

                cmp timer_trapped,0     ;INT 8 vector trapped yet?
                je __trap_timer         ;no, do it now...

                mov ax,0                ;timer was trapped; is it still OK?
                mov es,ax
                cmp es:[8*4],OFFSET AIL_INT8
                jne __trap_timer        ;no, trap it again
                mov ax,cs
                cmp es:[8*4+2],ax
                jne __trap_timer

__send_byte:    push bp                 ;send each message byte to the                
                mov bp,sp               ;AIL driver's MIDI data stream
                push si                
                mov si,0008
__send_AIL:     mov al,[bp+si]                   
                call put_MIDI_byte C,ax
                add si,0002                      
                test WORD PTR [bp+si],8000h
                je __send_AIL
                pop si
                pop bp

__exit:         ret

__trap_timer:   pushf                   ;trap 291.3Hz. Cakewalk timer tick
                cli                     ;to provide TVFX driver service
                cmp AIL_rate,-1         
                je __trapped            ;(no service required)

                mov ax,0
                mov es,ax
                mov ax,es:[8*4]
                mov dx,es:[8*4+2]
                mov WORD PTR Cake_INT8,ax
                mov WORD PTR Cake_INT8+2,dx

                mov es:[8*4],OFFSET AIL_INT8
                mov es:[8*4+2],cs

__trapped:      mov timer_trapped,1
                POP_F
                jmp __send_byte

                ENDP

;*****************************************************************************
                ;
                ;int data_in_queue(void)
                ;

data_in_queue   PROC                    ;Return 1 if any data in queue

                mov ax,0
                cmp MIDI_active,0
                je __exit

                mov ax,head
                xor ax,tail
                je __exit
                mov ax,1

__exit:         ret
                ENDP
                
;*****************************************************************************
                ;
                ;int read_queue(void)
                ;

read_queue      PROC                    ;Return next byte from queue

                mov ax,-1
                cmp MIDI_active,0
                je __exit

                mov bx,tail
                cmp bx,head
                je __exit

                mov al,queue[bx]
                mov ah,0

                inc tail
                cmp tail,256
                jne __exit
                mov tail,0

__exit:         ret
                ENDP

;*****************************************************************************
                ;
                ;void get_device_data(char far *data)
                ;

get_device_data PROC                
                push bp             
                mov bp,sp                        

                call init_dialog

                lea ax,AIL_base         ;get default I/O parameters 
                lea bx,AIL_fn           ;for current AIL.ADV driver
                call load_file C,bx,cs,ax,SIZE AIL_base
                cmp ax,0
                jne __desc_drvr

                call report_error C,OFFSET ERR_no_ADV
                jmp __chk_MIDI

__desc_drvr:    mov es,ax
                mov bx,es:[0]
                mov WORD PTR AIL_vectors,bx
                mov WORD PTR AIL_vectors+2,ax
                call AIL_describe_driver C,0

                mov es,dx
                mov bx,ax
                mov ax,' '                      ;assume IO used by AIL driver
                cmp es:[bx].dev_IO,-1
                jne __set_IO                    ;...if not, terminate text to
                mov ax,0                        ;truncate setup menu
__set_IO:       mov AIL_IO_text,al

                mov ax,' '                      ;assume IRQ used by AIL driver
                cmp es:[bx].dev_IRQ,-1
                jne __set_IRQ                   ;...if not, terminate text to
                mov ax,0                        ;truncate setup menu
__set_IRQ:      mov AIL_IRQ_text,al

__chk_MIDI:     les bx,[bp+06]                   
                mov ax,OFFSET devname
                mov es:[bx],ax                   
                mov es:[bx+02],cs                
                mov sp,bp                        
                pop bp                           
                ret                                
                ENDP

;*****************************************************************************
                END
