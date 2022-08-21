;############################################################################
;##                                                                        ##
;##  MSSA32.ASM                                                            ##
;##                                                                        ##
;##  Miles Sound System                                                    ##
;##                                                                        ##
;##  Digital Sound 32-bit and 64-bit flat-model assembly API functions     ##
;##                                                                        ##
;##  Version 2.00 of 11-Jun-98: Initial, derived from DLS1A32.ASM V1.00    ##
;##                                                                        ##
;##  80386 ASM source compatible with Microsoft Assembler v6.12 or later   ##
;##                                                                        ##
;##  Author: John Miles, Jeff Roberts                                      ##
;##                                                                        ##
;############################################################################
;##                                                                        ##
;##  Copyright (C) RAD Game Tools, Inc.                                    ##
;##                                                                        ##
;##  Contact RAD Game Tools at 425-893-4300 for technical support.         ##
;##                                                                        ##
;############################################################################
                                                                  
                OPTION SCOPED           ;Enable local labels

                IFNDEF WIN64

                ;
                ;DOS/Win32 version
                ;

                .586                    ;Enable Pentium instructions
                .MMX                    ;Enable MMX instructions
            
                IFDEF FORNONWIN
                  .MODEL FLAT,C
                ELSE
                IFDEF DPMI
                  .MODEL FLAT,C
                ELSE
                  .MODEL FLAT,STDCALL   ;(RIB functions use __stdcall in Win32)
                ENDIF
                ENDIF

CLEAR_UPPER_32  MACRO regparm                   ;Not needed with 32-bit registers
                ENDM

PTR_SIZE        TEXTEQU <4>
PTR_LABEL       TEXTEQU <DWORD>
PTR_TAG         TEXTEQU <dd>
PTR_EAX         TEXTEQU <eax>
PTR_EBX         TEXTEQU <ebx>
PTR_ECX         TEXTEQU <ecx>
PTR_EDX         TEXTEQU <edx>
PTR_ESI         TEXTEQU <esi>
PTR_EDI         TEXTEQU <edi>
PTR_EBP         TEXTEQU <ebp>
PTR_ESP         TEXTEQU <esp>

STEPTBL_PTR     TEXTEQU <step_whole>

                ELSE

                ;
                ;On entry to Win64 PROCs:
                ;
                ;  rcx = param #1
                ;  rdx = param #2
                ;  r8  = param #3
                ;  r9  = param #4
                ;
                ;  [rsp][0 * 8] = return address
                ;  [rsp][n * 8] = param #n, for all n > 4
                ;

CLEAR_UPPER_32  MACRO parm                      ;Mask arguments against 2^32-1 before using them as 32-bit words! 
                mov r11,00000000ffffffffh
                and parm,r11
                ENDM

ARG1            TEXTEQU <QWORD PTR [r10][1*8]>
ARG2            TEXTEQU <QWORD PTR [r10][2*8]>
ARG3            TEXTEQU <QWORD PTR [r10][3*8]>
ARG4            TEXTEQU <QWORD PTR [r10][4*8]>
ARG5            TEXTEQU <QWORD PTR [r10][5*8]>
ARG6            TEXTEQU <QWORD PTR [r10][6*8]>
ARG7            TEXTEQU <QWORD PTR [r10][7*8]>
ARG8            TEXTEQU <QWORD PTR [r10][8*8]>
ARG9            TEXTEQU <QWORD PTR [r10][9*8]>
ARG10           TEXTEQU <QWORD PTR [r10][10*8]> 
ARG11           TEXTEQU <QWORD PTR [r10][11*8]> 
ARG12           TEXTEQU <QWORD PTR [r10][12*8]> 

ENTER64         MACRO
                mov r10,rsp                     ;R10 is our frame pointer, kept valid throughout the routine
                mov ARG1,rcx                    ;Home the register parameters, so we can access them via text equates
                mov ARG2,rdx
                mov ARG3,r8
                mov ARG4,r9
                push rbx
                push rsi
                push rbp
                push rdi                        ;Note that R12-15 must be preserved as well, if they're ever used!
                ENDM

EXIT64          MACRO
                pop rdi
                pop rbp
                pop rsi
                pop rbx
                ENDM

PTR_SIZE        TEXTEQU <8>
PTR_LABEL       TEXTEQU <QWORD>
PTR_TAG         TEXTEQU <dq>
PTR_EAX         TEXTEQU <rax>
PTR_EBX         TEXTEQU <rbx>
PTR_ECX         TEXTEQU <rcx>
PTR_EDX         TEXTEQU <rdx>
PTR_ESI         TEXTEQU <rsi>
PTR_EDI         TEXTEQU <rdi>
PTR_EBP         TEXTEQU <rbp>
PTR_ESP         TEXTEQU <rsp>

STEPTBL_PTR     TEXTEQU <r11>

                ENDIF

                .DATA

data_start      LABEL BYTE 

DIG_F_16BITS_MASK equ 1                  ;Copy options passed from caller

M_DEST_STEREO   equ 1                   ;Mixer options passed from caller
M_SRC_16        equ 2
M_FILTER        equ 4
M_SRC_STEREO    equ 8
M_ORDER         equ 16
M_RESAMPLE      equ 32
M_VOL_SCALING   equ 64
M_COPY16_NOVOL  equ 128

                ALIGN 16

src_end         PTR_TAG ?
dest_end        PTR_TAG ?
step_whole      PTR_TAG 2 dup (?)

step_fract_save LABEL PTR_LABEL
step_fract      dd ?
                dd ?

operation_save  LABEL PTR_LABEL
operation       dd ?
                dd ?

scale_left_save LABEL PTR_LABEL
scale_left      dd ?
                dd ?

scale_right_save LABEL PTR_LABEL
scale_right     dd ?
                dd ?

playback_ratio_save LABEL PTR_LABEL
playback_ratio  dd ?
                dd ?

divider_l_save  LABEL PTR_LABEL
divider_l       dd ?
                dd ?

divider_r_save  LABEL PTR_LABEL
divider_r       dd ?
                dd ?

cur_l_save      LABEL PTR_LABEL
cur_l           dd ?
                dd ?

cur_r_save      LABEL PTR_LABEL
cur_r           dd ?
                dd ?

cur_r2_save     LABEL PTR_LABEL
cur_r2          dd ?
                dd ?

src_end_early_save LABEL PTR_LABEL
src_end_early   dd ?
                dd ?

cnt_save        LABEL PTR_LABEL
cnt             dd ?
                dd ?

                ;
                ;Table of reciprocals of integers 0-255, in 1:16 format
                ;

                ; // @cdep pre $requires(mssadp.inc)
                INCLUDE mssadp.inc

ADPCMDATA       STRUC
  blocksize     dd ?
  extrasamples  db ?,?,?,?
  blockleft     dd ?
  adpstep       dd ?
  savesrc       PTR_TAG ?
  lsample       dd ? ; the rest of the structure also doubles as a decomp buffer
  destend       PTR_TAG ?
  srcend        PTR_TAG ?
  samplesL      dd ?
  samplesR      dd ?
  moresamples   dw 16 dup (?)
ADPCMDATA       ENDS

data_end        LABEL BYTE

                .CODE MSSMIXER

                ;
                ;RIB-exported functions
                ;

                PUBLIC MSS_mixer_startup
                PUBLIC MSS_mixer_shutdown
                PUBLIC MSS_mixer_flush
                PUBLIC MSS_mixer_merge
                PUBLIC MSS_mixer_copy

                ;
                ;Utility functions (called directly)
                ;

                PUBLIC MSS_MMX_available

                PUBLIC DecodeADPCM_STEREO
                PUBLIC DecodeADPCM_MONO
                PUBLIC DecodeADPCM_MONO_8

                IFDEF DPMI
                  PUBLIC MSSA32_VMM_lock
                ENDIF

code_start      LABEL BYTE


; // @cdep pre $requiresifexists(mssmixer.inc)
include mssmixer.inc

;#############################################################################
;##                                                                         ##
;## Returned bits:                                                          ##
;##                                                                         ##
;## 0x0001: MMX present                                                     ##
;## 0x0002: AMD 3DNow extensions present                                    ##
;## 0x0004: SSE extensions present                                          ##
;##                                                                         ##
;## Returns 0 if no math extensions present                                 ##
;##                                                                         ##
;#############################################################################

               IFDEF WIN64
MSS_MMX_available PROC
                  ENTER64
               ELSE
MSS_MMX_available PROC \
                  USES ebx esi edi ds es
               ENDIF

                ; If bit 21 of the eflags register is changeable, the CPU
                ; supports the "CPUID" instruction -- this includes all P5's,
                ; P6's, some 486s, and all future CPUs
                ; This check is obviously not necessary on any x64 platform...

                IFNDEF WIN64
                pushfd
                pop     eax             ;EAX = original eflags word
                mov     ebx,eax         ;save it in EBX
                xor     eax,1 SHL 21    ;toggle bit 21
                push    eax
                popfd
                pushfd                  ;examine eflags again
                pop     eax
                push    ebx             ;restore original eflags value
                popfd
                mov     esi,0           ;assume result = 0
                cmp     eax,ebx         ;bit changed?
                je      __exit          ;no CPUID, ergo no MMX
                ENDIF

                mov     eax,1           ;else OK to perform CPUID
                cpuid
                test    edx,00800000h   ;test bit 23
                jz      __exit          ;if no MMX, no 3DNow or SSE either

                or      esi,1           ;set bit to indicate MMX present
IFDEF DPMI

; no sse for dos!

ELSE
                test    edx,02000000h   ;test SSE bit
                jz      __Check3DNow    ;if clear, SSE not supported

                or      esi,4           ;set bit to indicate SSE support
ENDIF

__Check3DNow:
                mov     eax,80000000h   ;query for extended functions
                cpuid                   ;get extended function limit

                cmp     eax,80000000h   ;is 8000_0001h supported?
                jbe     __exit          ;if not, 3DNow not supported

                mov     eax,80000001h   ;extended function 1
                cpuid                   ;call the function

                test    edx,80000000h   ;test bit 31
                jz      __exit          ;if clear, 3DNow not supported

                or      esi,2           ;set bit to indicate 3DNow support

__exit:
                mov     eax,esi         ;return feature bitfield

                IFDEF WIN64
                EXIT64
                ENDIF

                ret

MSS_MMX_available ENDP

;############################################################################

;#############################################################################
;##                                                                         ##
;## Start up mixer services                                                 ##
;##                                                                         ##
;#############################################################################

                IFDEF WIN64
MSS_mixer_startup PROC
                ENTER64
                ELSE
MSS_mixer_startup PROC \
                USES ebx esi edi ds es
                ENDIF

                mov eax,0

                IFDEF WIN64
                EXIT64
                ENDIF

                ret

MSS_mixer_startup ENDP

;#############################################################################
;##                                                                         ##
;## Shut down mixer services                                                ##
;##                                                                         ##
;#############################################################################

                IFDEF WIN64
MSS_mixer_shutdown PROC
                ENTER64
                ELSE
MSS_mixer_shutdown PROC \
                USES ebx esi edi ds es
                ENDIF

                mov eax,0

                IFDEF WIN64
                EXIT64
                ENDIF

                ret

MSS_mixer_shutdown ENDP

;#############################################################################
;##                                                                         ##
;## Flush build buffer with zero-amplitude DC baseline signal               ##
;##                                                                         ##
;#############################################################################

                IFDEF WIN64
MSS_mixer_flush PROC
                ENTER64
                ELSE
MSS_mixer_flush PROC \
                USES esi ecx edi ds es \
                lpDest:PTR,\
                nLen:DWORD,\
                bMMXAvailable:DWORD
                ENDIF

                cld
                mov eax,0

                IFNDEF WIN64

                push ds
                pop es

                mov edi,[lpDest]
                mov ecx,[nLen]

                push ecx
                shr ecx,2
                rep stosd
                
                pop ecx
                and ecx,3
                rep stosb

                ELSE
                
                mov rdi,ARG1
                mov rcx,ARG2
                CLEAR_UPPER_32 rcx

                push rcx
                shr rcx,2
                rep stosd
                
                pop rcx
                and rcx,3
                rep stosb

                EXIT64

                ENDIF

                ret

MSS_mixer_flush ENDP

;#############################################################################
;##                                                                         ##
;## Mix data from PCM source into 32-bit signed build buffer                ##
;##                                                                         ##
;#############################################################################

                IFDEF WIN64

MSS_mixer_merge PROC
                ENTER64

lplpSrc         TEXTEQU <QWORD PTR [r10][1*8]> 
lpSrcFract      TEXTEQU <QWORD PTR [r10][2*8]>    
lpSrcEnd        TEXTEQU <QWORD PTR [r10][3*8]> 
lplpDest        TEXTEQU <QWORD PTR [r10][4*8]> 
lpDestEnd       TEXTEQU <QWORD PTR [r10][5*8]>    
lpLeftVal       TEXTEQU <QWORD PTR [r10][6*8]>    
lpRightVal      TEXTEQU <QWORD PTR [r10][7*8]>    
nPlaybackRatio  TEXTEQU <DWORD PTR [r10][8*8]>       
nScaleLeft      TEXTEQU <DWORD PTR [r10][9*8]>    
nScaleRight     TEXTEQU <DWORD PTR [r10][10*8]>    
nOperation      TEXTEQU <DWORD PTR [r10][11*8]>    
bMMXAvailable   TEXTEQU <DWORD PTR [r10][12*8]>

                ELSE

MSS_mixer_merge PROC \
                USES ebx ecx esi edi ds es \
                lplpSrc:PTR,\
                lpSrcFract:PTR,\
                lpSrcEnd:PTR,\
                lplpDest:PTR,\
                lpDestEnd:PTR,\
                lpLeftVal:PTR,\
                lpRightVal:PTR,\
                nPlaybackRatio:DWORD,\
                nScaleLeft:DWORD,\
                nScaleRight:DWORD,\
                nOperation:DWORD,\
                bMMXAvailable:DWORD

                ENDIF

                ;
                ;Save static values against reentry from foreground 
                ;

                push src_end
                push dest_end
                push step_whole[0]
                push step_whole[1]
                push step_fract_save
                push operation_save
                push scale_left_save
                push scale_right_save
                push playback_ratio_save
                push divider_l_save
                push divider_r_save
                push cur_l_save
                push cur_r_save 
                push cur_r2_save 
                push src_end_early_save 
                push cnt_save 

                ;
                ;Configure step tables for frequency resampling, and adjust
                ;for sample size in bytes
                ;

                mov eax,nPlaybackRatio
                mov ebx,eax
                shr ebx,16      ;EBX = whole part
                shl eax,16      ;EAX = fract part

                mov step_fract,eax

                mov eax,ebx     ;EAX = base step when fract does not overflow
                inc ebx         ;EBX = base+1 step when fract overflows

                test nOperation,M_SRC_STEREO
                jz __chk_16_bits

                shl eax,1
                shl ebx,1

__chk_16_bits:  test nOperation,M_SRC_16
                jz __write_step

                shl eax,1       ;multiply whole step size by # of bytes/sample
                shl ebx,1

__write_step:   CLEAR_UPPER_32 PTR_EBX
                CLEAR_UPPER_32 PTR_EAX
   
                mov step_whole[0*PTR_SIZE],PTR_EBX
                mov step_whole[1*PTR_SIZE],PTR_EAX

                ;
                ;Register usage throughout all loops:
                ;
                ;EAX->scratch
                ;EBX->scratch
                ;ECX->dest end (or scratch)
                ;EDX->src fraction (or scratch)
                ;ESI->src
                ;EDI->dest
                ;
                ;Non-filtered and downsample-filtered loops maintain the
                ;source address fraction at 0:32-bit precision.  Upsample-
                ;filtered loops use 16:16-bit precision, to avoid processing
                ;skipped samples twice.  The fractional component is always
                ;returned to the caller at 16:16 precision to allow smooth
                ;transitions between the two loop types.
                ;

                mov PTR_EBX,lpSrcFract
                mov edx,[PTR_EBX]

__load_src:     mov PTR_EBX,lpLeftVal
                mov eax,[ebx]
                mov cur_l,eax

                mov PTR_EBX,lpRightVal
                mov eax,[ebx]
                mov cur_r,eax

                mov PTR_EBX,lplpSrc
                mov PTR_ESI,[PTR_EBX]
                cmp PTR_ESI,lpSrcEnd
                mov PTR_EDI,0
                jae __exit

                mov PTR_EBX,lplpDest
                mov PTR_EDI,[PTR_EBX]
                mov PTR_ECX,lpDestEnd
                cmp PTR_EDI,PTR_ECX
                jae __exit

                ;
                ;Make parameters accessible to macros
                ;

                mov eax,nScaleLeft
                mov ebx,nScaleRight
                mov scale_left,eax
                mov scale_right,ebx

                mov PTR_EAX,lpSrcEnd
                mov PTR_EBX,lpDestEnd
                mov src_end,PTR_EAX
                mov dest_end,PTR_EBX

                mov eax,nPlaybackRatio
                mov playback_ratio,eax

                mov eax,nOperation
                mov operation,eax

                ;
                ;Call merge handler
                ;

                IFDEF WIN64

                CLEAR_UPPER_32 rax
                shl rax,3              ;* sizeof(pointer)

                lea r11,vector_table   ;LEA hackery needed because of spurious LNK2017 error
                add rax,r11

                lea r11,step_whole     

                call QWORD PTR [rax]

                ELSE

                call cs:vector_table[PTR_EAX * PTR_SIZE]

                ENDIF

                ;
                ;Update application src/dest pointers before returning
                ;

__exit:         mov PTR_EBX,lplpSrc
                mov [PTR_EBX],PTR_ESI

                mov PTR_EBX,lplpDest
                mov [PTR_EBX],PTR_EDI

                mov PTR_EBX,lpSrcFract
                mov [PTR_EBX],edx

                mov eax,cur_l
                mov PTR_EBX,lpLeftVal
                mov [PTR_EBX],eax

                mov eax,cur_r
                mov PTR_EBX,lpRightVal
                mov [PTR_EBX],eax

                pop cnt_save 
                pop src_end_early_save 
                pop cur_r2_save 
                pop cur_r_save 
                pop cur_l_save
                pop divider_r_save
                pop divider_l_save
                pop playback_ratio_save
                pop scale_right_save
                pop scale_left_save
                pop operation_save
                pop step_fract_save
                pop step_whole[1]
                pop step_whole[0]
                pop dest_end
                pop src_end

                IFDEF WIN64
                EXIT64
                ENDIF

                ret

MSS_mixer_merge ENDP

;#############################################################################
;##                                                                         ##
;## Clip and copy build buffer data to output sample buffer                 ##
;##                                                                         ##
;#############################################################################

                IFDEF WIN64

MSS_mixer_copy  PROC
                ENTER64

lpSrc           TEXTEQU <QWORD PTR [r10][1*8]>
nSrcLen         TEXTEQU <QWORD PTR [r10][2*8]>
lpDest          TEXTEQU <QWORD PTR [r10][3*8]>
nOperation      TEXTEQU <DWORD PTR [r10][4*8]>
bMMXAvailable   TEXTEQU <DWORD PTR [r10][5*8]>

                ELSE

MSS_mixer_copy  PROC \
                USES ebx ecx esi edi ds es \
                lpSrc:PTR,\
                nSrcLen:DWORD,\
                lpDest:PTR,\
                nOperation:DWORD,\
                bMMXAvailable:DWORD

                ENDIF

                ;
                ;ESI->src
                ;EDI->dest
                ;ECX->src end
                ;

                mov PTR_ESI,lpSrc
                mov PTR_EDI,lpDest
                mov PTR_ECX,nSrcLen
                CLEAR_UPPER_32 PTR_ECX

                cmp PTR_ECX,0
                je __exit
                add PTR_ECX,PTR_ESI

                ;
                ;If MMX requested, do as many blocks as possible
                ;before falling through to x86 remnant case
                ;

                cmp bMMXAvailable,0
                je __x86

                mov PTR_EAX,nSrcLen ;get # of source bytes
                CLEAR_UPPER_32 PTR_EAX
                shr eax,4           ;divide by 128-bit MMX block size
                jz __x86            ;if < 1 block, fall through to remnant

                test nOperation,DIG_F_16BITS_MASK
                jz __do_8_MMX

                ;
                ;MMX clip and copy to 16-bit stream
                ;

__do_16_MMX:    movq mm0,[PTR_ESI]      ;read first 2 source DWORDs
                psrad mm0,11        ;SAR DWORD pair by 11

                movq mm1,[PTR_ESI+8]    ;read next 2 source DWORDs
                psrad mm1,11        ;SAR DWORD pair by 11

                packssdw mm0,mm1    ;merge DWORDs into 4 WORDs w/saturation
                movq [PTR_EDI],mm0      ;write 4 samples to output stream

                add PTR_EDI,8
                add PTR_ESI,16
                dec eax
                jnz __do_16_MMX

                emms                ;exit MMX mode
                jmp __x86

                ;
                ;MMX clip and copy to 8-bit stream
                ;

__do_8_MMX:     mov ebx,80808080h   ;prepare XOR constant for 
                movd mm2,ebx        ;signed-to-unsigned conversion

__8_MMX_loop:   movq mm0,[PTR_ESI]      ;read first 2 source DWORDs
                psrad mm0,11        ;SAR DWORD pair by 11

                movq mm1,[PTR_ESI+8]    ;read next 2 source DWORDs
                psrad mm1,11        ;SAR DWORD pair by 11

                packssdw mm0,mm1    ;merge DWORDs into 4 WORDs w/saturation

                psraw mm0,8         ;convert to bytes
                packsswb mm0,mm0

                pxor mm0,mm2        ;convert to unsigned bytes
                movd DWORD PTR [PTR_EDI],mm0   ;write 4 samples to output stream

                add PTR_EDI,4
                add PTR_ESI,16
                dec eax
                jnz __8_MMX_loop

                emms                ;exit MMX mode

                ;
                ;Non-MMX version also handles remnant case
                ;

__x86:          cmp PTR_ESI,PTR_ECX
                jae __exit
                
                test nOperation,DIG_F_16BITS_MASK
                jz __copy_8

                ;
                ;X86 clip and copy to 16-bit stream
                ;

__copy_16:      mov eax,[PTR_ESI]

                add PTR_EDI,2
                sar eax,11
                add PTR_ESI,4

                cmp eax,32767
                jg __clip_H16
                cmp eax,-32768
                jl __clip_L16

                mov [PTR_EDI-2],ax
                cmp PTR_ESI,PTR_ECX
                jb __copy_16
                jmp __exit

__clip_H16:     mov WORD PTR [PTR_EDI-2],32767
                cmp PTR_ESI,PTR_ECX
                jb __copy_16
                jmp __exit

__clip_L16:     mov WORD PTR [PTR_EDI-2],-32768
                cmp PTR_ESI,PTR_ECX
                jb __copy_16
                jmp __exit

                ;
                ;X86 clip and copy to 8-bit stream
                ;

__copy_8:       mov eax,[PTR_ESI]

                inc PTR_EDI
                sar eax,11
                add PTR_ESI,4

                cmp eax,32767
                jg __clip_H8
                cmp eax,-32768
                jl __clip_L8

                xor eax,8000h

                mov [PTR_EDI-1],ah
                cmp PTR_ESI,PTR_ECX
                jb __copy_8
                jmp __exit

__clip_H8:      mov BYTE PTR [PTR_EDI-1],0ffh
                cmp PTR_ESI,PTR_ECX
                jb __copy_8
                jmp __exit

__clip_L8:      mov BYTE PTR [PTR_EDI-1],0
                cmp PTR_ESI,PTR_ECX
                jb __copy_8
                jmp __exit

__exit:         
                IFDEF WIN64
                EXIT64
                ENDIF

                ret

MSS_mixer_copy  ENDP

;#############################################################################
DECODE_ADPCM	MACRO Flg

               IFDEF WIN64

               ;
               ;Win64 version
               ;

IF (Flg AND 2)
DecodeADPCM_MONO_8 PROC
ELSE
IF (Flg AND 1)
DecodeADPCM_STEREO PROC
ELSE
DecodeADPCM_MONO PROC
ENDIF
ENDIF
               ENTER64

destaddrptr    TEXTEQU <QWORD PTR [r10][1*8]>
srcaddrptr     TEXTEQU <QWORD PTR [r10][2*8]>
destlen        TEXTEQU <QWORD PTR [r10][3*8]>
srclen         TEXTEQU <QWORD PTR [r10][4*8]>
adpcmptr       TEXTEQU <QWORD PTR [r10][5*8]>

               ELSE

               ;
               ;Win32 version
               ;

IF (Flg AND 2)
DecodeADPCM_MONO_8 PROC C
ELSE
IF (Flg AND 1)
DecodeADPCM_STEREO PROC C
ELSE
DecodeADPCM_MONO PROC C
ENDIF
ENDIF

destaddrptr    equ [ebp+28+0]
srcaddrptr     equ [ebp+28+4]
destlen        equ [ebp+28+8]
srclen         equ [ebp+28+12]
adpcmptr       equ [ebp+28+16]

               ENDIF

               IFNDEF WIN64

               push PTR_EBX
               push PTR_ECX
               push PTR_EDX
               push PTR_ESI
               push PTR_EDI
               push PTR_EBP
             
               mov ebp,esp

STEPW_TBL      TEXTEQU <OFFSET stepw>
NEWAD_TBL      TEXTEQU <OFFSET newadpcm>

               ELSE

               CLEAR_UPPER_32 destlen
               CLEAR_UPPER_32 srclen

               lea r8,stepw            ;LEA hackery needed because of spurious LNK2017 error 
               lea r9,newadpcm

STEPW_TBL      TEXTEQU <r8>
NEWAD_TBL      TEXTEQU <r9>

               ENDIF

               mov PTR_EDI,destaddrptr
               mov PTR_EBX,srcaddrptr
               mov PTR_EDI,[PTR_EDI]
               mov PTR_EBX,[PTR_EBX]
               mov PTR_ESI,PTR_EDI
               mov PTR_ECX,PTR_EBX
               add PTR_ESI,destlen
               add PTR_ECX,srclen
               mov PTR_EBP,adpcmptr
               mov [PTR_EBP.ADPCMDATA.destend],PTR_ESI
               mov [PTR_EBP.ADPCMDATA.srcend],PTR_ECX
               
               cmp [PTR_EBP.ADPCMDATA.extrasamples],0
               ja  __doextrasamps

               cmp [PTR_EBP.ADPCMDATA.blockleft],0
               ja __doadword

__startblock:
               IF (Flg AND 1)
             
               mov eax,[PTR_EBX] ;get the first sample and step size
               mov esi,[PTR_EBP.ADPCMDATA.blocksize]
               ror eax,16
               sub esi,8
             
               mov edx,[PTR_EBX+4]
               mov [PTR_EBP.ADPCMDATA.blockleft],esi
               xchg ax,dx
             
               ror eax,16
               add PTR_EBX,8
             
               shl edx,8
               mov [PTR_EDI],eax
             
               add PTR_EDI,4
             
               ELSE
             
               IF (Flg AND 2)
             
               mov edx,[PTR_EBX] ;get the first sample and step size
               mov eax,edx
               mov esi,[PTR_EBP.ADPCMDATA.blocksize]
               add dh,080h
               add PTR_EBX,4
               mov [PTR_EDI],dh
               sub esi,4
               inc PTR_EDI
               shr edx,8
               mov [PTR_EBP.ADPCMDATA.blockleft],esi
               and edx,0ffffh
             
               ELSE
             
               mov edx,[PTR_EBX] ;get the first sample and step size
               mov eax,edx
               mov esi,[PTR_EBP.ADPCMDATA.blocksize]
               mov [PTR_EDI],dx
               add PTR_EBX,4
               shr edx,8
               sub esi,4
               add PTR_EDI,2
               mov [PTR_EBP.ADPCMDATA.blockleft],esi
               and edx,0ffffh
             
               ENDIF
               ENDIF

__contsamps:
               cmp PTR_EBX,[PTR_EBP.ADPCMDATA.srcend]
               jae __srcend

__donextdword:

               mov ecx,[PTR_EBX]
             
               IF (Flg AND 1)
               add PTR_EBX,8
               mov dl,cl
               mov [PTR_EBP.ADPCMDATA.savesrc],PTR_EBX
               shl dl,4
               mov ebx,[PTR_EBX-4]                       ;ebx is not a pointer anymore?
               sub [PTR_EBP.ADPCMDATA.blockleft],8
               ror edx,16
               mov [PTR_EBP.ADPCMDATA.extrasamples],8
               mov dl,bl
               shl dl,4
               ror edx,20
               ELSE
               mov dl,cl
               add PTR_EBX,4
               mov [PTR_EBP.ADPCMDATA.extrasamples],8
               shl dl,4
               sub [PTR_EBP.ADPCMDATA.blockleft],4
               ENDIF

__expanddword:
             
               cmp PTR_EDI,[PTR_EBP.ADPCMDATA.destend]
               jae __destend
             
               IF (Flg AND 1)
               mov PTR_ESI,0fffh
               and esi,edx
               mov dl,cl
               add PTR_ESI,NEWAD_TBL
               mov dh,[PTR_ESI]
               sub PTR_ESI,NEWAD_TBL
               shl PTR_ESI,1
               add PTR_ESI,STEPW_TBL
               test cl,8
               mov si,[PTR_ESI]
               jnz __minusL
             
               add si,ax
               cmp si,ax
               jge __handleL
             
               mov ax,32767
               jmp __fixedsampleL

__minusL:
               add si,ax
               cmp si,ax
               jg __toolowL

__handleL:
               mov ax,si
__fixedsampleL:
               ror edx,16
               mov PTR_ESI,0fffh
               ror eax,16
               and esi,edx
               mov dl,bl
               add PTR_ESI,NEWAD_TBL
               mov dh,[PTR_ESI]
               sub PTR_ESI,NEWAD_TBL
               shl PTR_ESI,1
               add PTR_ESI,STEPW_TBL
               test bl,8
               mov si,[PTR_ESI]
               jnz __minusR
             
               add si,ax
               cmp si,ax
               jge __handleR
             
               mov ax,32767
               jmp __fixedsampleR

__minusR:
               add si,ax
               cmp si,ax
               jg __toolowR

__handleR:
               mov ax,si
__fixedsampleR:
               ror edx,20
               ror eax,16
               shr ecx,4
               mov [PTR_EDI],eax
             
               shr ebx,4
               add PTR_EDI,4
             
               ELSE

               shr edx,4
             
               mov esi,edx
               movzx edx,cl
               CLEAR_UPPER_32 PTR_ESI
               add PTR_ESI,NEWAD_TBL
               mov dh,[PTR_ESI]
               sub PTR_ESI,NEWAD_TBL
               shl PTR_ESI,1
               add PTR_ESI,STEPW_TBL
               test cl,8
               movzx esi,WORD PTR [PTR_ESI]
               jnz __minus

               add si,ax
               cmp si,ax
               jge __handle
               mov ax,32767
               jmp __fixedsample
                           
__minus:
               add si,ax
               cmp si,ax
               jg __toolow

__handle:
               mov ax,si
__fixedsample:
               IF (Flg AND 2)
             
               ;8-bit mono
               add ah,080h
               shr ecx,4
               mov [PTR_EDI],ah
               inc PTR_EDI
               sub ah,080h

               ELSE

               ;16-bit mono
               shr ecx,4
               mov [PTR_EDI],ax
               add PTR_EDI,2

               ENDIF

               ENDIF

               dec [PTR_EBP.ADPCMDATA.extrasamples]
               jnz __expanddword
             
               IF (Flg AND 1)
               mov PTR_EBX,[PTR_EBP.ADPCMDATA.savesrc]
               ENDIF
             
               cmp PTR_EDI,[PTR_EBP.ADPCMDATA.destend]
               jae __destendloop
             
               IF (Flg AND 1)
               shl edx,4
               ENDIF
             
               cmp [PTR_EBP.ADPCMDATA.blockleft],0
               jne __contsamps

               cmp PTR_EBX,[PTR_EBP.ADPCMDATA.srcend]
               jb __startblock

__destendtop:
__srcend:
               mov [PTR_EBP.ADPCMDATA.extrasamples],0
               mov [PTR_EBP.ADPCMDATA.adpstep],edx
               jmp __done

__destend:
               mov [PTR_EBP.ADPCMDATA.samplesL],ecx
               IF (Flg AND 1)
               mov [PTR_EBP.ADPCMDATA.samplesR],ebx
               mov PTR_EBX,[PTR_EBP.ADPCMDATA.savesrc]
               ENDIF

__destendloop:
               mov [PTR_EBP.ADPCMDATA.lsample],eax
               mov [PTR_EBP.ADPCMDATA.adpstep],edx

__done:
               IFNDEF WIN64
               mov ebp,esp
               ENDIF

               mov PTR_ESI,destaddrptr
               mov PTR_ECX,srcaddrptr
               mov [PTR_ESI],PTR_EDI
               mov [PTR_ECX],PTR_EBX
             
               IFDEF WIN64
               EXIT64
               ELSE
               pop PTR_EBP
               pop PTR_EDI
               pop PTR_ESI
               pop PTR_EDX
               pop PTR_ECX
               pop PTR_EBX
               ENDIF

               ret

               IF (Flg AND 1)

__toolowL:
               mov ax,-32768
               jmp __fixedsampleL

__toolowR:
               mov ax,-32768
               jmp __fixedsampleR

               ELSE

__toolow:
               mov ax,-32768
               jmp __fixedsample

               ENDIF

__doadword:
               mov edx,[PTR_EBP.ADPCMDATA.adpstep]
               IF (Flg AND 1)
               shl edx,4
               ENDIF

               mov eax,[PTR_EBP.ADPCMDATA.lsample]
               jmp __donextdword

__doextrasamps:

               mov ecx,[PTR_EBP.ADPCMDATA.samplesL]
               IF (Flg AND 1)
               mov [PTR_EBP.ADPCMDATA.savesrc],PTR_EBX
               mov ebx,[PTR_EBP.ADPCMDATA.samplesR]
               ENDIF
             
               mov eax,[PTR_EBP.ADPCMDATA.lsample]
               mov edx,[PTR_EBP.ADPCMDATA.adpstep]
               jmp __expanddword

IF (Flg AND 2)
DecodeADPCM_MONO_8 ENDP
ELSE
IF (Flg AND 1)
DecodeADPCM_STEREO ENDP
ELSE
DecodeADPCM_MONO ENDP
ENDIF
ENDIF

               ENDM

;#############################################################################
DECODE_ADPCM 0	; Mono adpcm decode

DECODE_ADPCM 1	; Stereo adpcm decode

DECODE_ADPCM 2	; Mono, 8-bit adpcm decode


db 13,10,13,10
db 'Miles Sound System',13,10
db 'Copyright (C) 1991-2007 RAD Game Tools, Inc.',13,10,13,10

                ;
                ;End of locked code
                ;

code_end        LABEL BYTE

;#############################################################################
;##                                                                         ##
;## Lock all code and data in AILSSA module                                 ##
;##                                                                         ##
;#############################################################################

                IFDEF DPMI

AIL_vmm_lock_range PROTO C,P1:NEAR PTR,P2:NEAR PTR

MSSA32_VMM_lock PROC C \
                USES ebx esi edi ds es

                invoke AIL_vmm_lock_range,OFFSET data_start,OFFSET data_end
                invoke AIL_vmm_lock_range,OFFSET code_start,OFFSET code_end

                ret

MSSA32_VMM_lock ENDP

                ENDIF

                END


