;############################################################################
;##                                                                        ##
;##  MATH_A.ASM                                                            ##
;##                                                                        ##
;##  Miles Sound System                                                    ##
;##                                                                        ##
;##  IMDCT and polyphase-filter support routines                           ##
;##                                                                        ##
;##  Version 1.00 of 2-Mar-99: Initial, derived from algorithms by         ##
;##                            Byeong Gi Lee, Jeff Tsay, Francois          ##
;##                            Raymond-Boyer, K. Konstantinides, Mikko     ##
;##                            Tommila, et al.                             ##
;##                                                                        ##
;##  80386/AMD K6-3D ASM source compatible with Microsoft Assembler v6.13  ##
;##  or later                                                              ##
;##                                                                        ##
;##  Author: John Miles                                                    ##
;##                                                                        ##
;############################################################################
;##                                                                        ##
;##  Copyright (C) RAD Game Tools, Inc.                                    ##
;##                                                                        ##
;##  Contact RAD Game Tools at 425-893-4300 for technical support.         ##
;##                                                                        ##
;############################################################################

                OPTION SCOPED       ; enable local labels

                IFNDEF WIN64

                .686
                .xmm

                IFDEF AMD
                .K3D                ; enable 3DNow opcodes if building AMD version
                ENDIF

                IFDEF DPMI
                  .MODEL FLAT,C
                ELSE
                  .MODEL FLAT,STDCALL
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

ENTER64         MACRO localsize
                mov r10,rsp                     ;R10 is our frame pointer, kept valid throughout the routine
                mov ARG1,rcx                    ;Home the register parameters, so we can access them via text equates
                mov ARG2,rdx
                mov ARG3,r8
                mov ARG4,r9

                push rbx 
                push rsi 
                push rbp 
                push rdi                        ;Note that R12-15 must be preserved as well, if they're ever used! 

                IFDEF SSE
                movd ebx,xmm6
                movd esi,xmm7
                push rbx
                push rsi
                ENDIF

                mov rbp,rsp                     ;(PUSH RDI decrements RSP by 8, then stores to *RSP, so RBP-(sizeof value) is the first local location)
                sub rsp,localsize
                ENDM

EXIT64          MACRO
                mov rsp,rbp

                IFDEF SSE
                pop rsi
                pop rbx
                movd xmm7,esi
                movd xmm6,ebx
                ENDIF

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

                ENDIF

                .DATA

IFNDEF SSE  ; only dewindow_and_write has an SSE version; in all other cases, the x87 code is used

 IFDEF AMD
amd_data_start LABEL BYTE
public amd_data_start
 ELSE
x86_data_start LABEL BYTE
public x86_data_start
 ENDIF

sin30           dd 0.500000000F         ;sin(pi/6)
cos30           dd 0.866025403F         ;cos(pi/6)

dct3t1          dd 1.931851653F       
dct3t2          dd 0.707106781F
dct3t3          dd 0.517638090F
dct3t4          dd 0.504314480F
dct3t5          dd 0.541196100F
dct3t6          dd 0.630236207F
dct3t7          dd 0.821339815F
dct3t8          dd 1.306562965F
dct3t9          dd 3.830648788F
dct3t10         dd -0.793353340F
dct3t11         dd -0.608761429F
dct3t12         dd -0.923879532F
dct3t13         dd -0.382683432F
dct3t14         dd -0.991444861F
dct3t15         dd -0.130526192F
dct3t16         dd  1.000000000F
dct3t17         dd  0.382683432F
dct3t18         dd  0.608761429F
dct3t19         dd -0.793353340F
dct3t20         dd -0.923879532F
dct3t21         dd -0.991444861F
dct3t22         dd  0.130526192F

dct9t0          dd 1.8793852415718F
dct9t1          dd 1.532088886238F
dct9t2          dd 0.34729635533386F
dct9t3          dd -0.34729635533386F
dct9t4          dd -1.8793852415718F
dct9t5          dd 1.532088886238F
dct9t6          dd -1.532088886238F
dct9t7          dd 0.34729635533386F
dct9t8          dd -1.8793852415718F
dct9t9          dd 1.732050808F
dct9t10         dd 1.9696155060244F
dct9t11         dd 1.2855752193731F
dct9t12         dd 0.68404028665134F
dct9t13         dd 1.2855752193731F
dct9t14         dd -0.68404028665134F
dct9t15         dd 1.9696155060244F
dct9t16         dd 0.68404028665134F
dct9t17         dd 1.9696155060244F
dct9t18         dd -1.2855752193731F
dct9t19         dd 0.707106781F

dct36t0         dd 0.501909918F
dct36t1         dd -0.250238171F
dct36t2         dd -5.731396405F

dct36t3         dd 0.517638090F
dct36t4         dd -0.25215724F
dct36t5         dd -1.915324394F
dct36t6         dd 0.551688959F
dct36t7         dd -0.256069878F
dct36t8         dd -1.155056579F
dct36t9         dd 0.610387294F
dct36t10        dd -0.262132281F
dct36t11        dd -0.831377381F
dct36t12        dd 0.871723397F
dct36t13        dd -0.281845486F
dct36t14        dd -0.541420142F
dct36t15        dd 1.183100792F
dct36t16        dd -0.296422261F
dct36t17        dd -0.465289749F
dct36t18        dd 1.931851653F
dct36t19        dd -0.315118103F
dct36t20        dd -0.410669907F
dct36t21        dd 5.736856623F
dct36t22        dd -0.339085426F
dct36t23        dd -0.370046808F
dct36t24        dd 0.707106781F
dct36t25        dd -0.541196100F
dct36t26        dd -1.306562965F

 IFDEF AMD
amd_data_end LABEL BYTE
public amd_data_end
 ELSE
x86_data_end LABEL BYTE
public x86_data_end
 ENDIF

ENDIF   ; IFNDEF SSE

                .CODE

IFDEF AMD
amd_code_start LABEL BYTE
public amd_code_start
ELSE
 IFDEF SSE
sse_code_start LABEL BYTE
public sse_code_start
 ELSE
x86_code_start LABEL BYTE
public x86_code_start
 ENDIF
ENDIF

fdup            MACRO
                fld st(0)               ;dup top of stack
                ENDM

_FEMMS          MACRO                   ;emit femms on AMD only
                IFDEF AMD
                  femms
                ENDIF
                ENDM

            IFDEF AMD

		        NAME MATHAMD

		        PUBLIC AMD_poly_filter
                PUBLIC AMD_dewindow_and_write
                PUBLIC AMD_IMDCT_3x12
                PUBLIC AMD_IMDCT_1x36
            ELSE
             IFDEF SSE

        		NAME MATHSSE

                ; only dewindow_and_write has an SSE version; in all other cases, the x87 code is used
                PUBLIC SSE_dewindow_and_write

             ELSE

		        NAME MATHX86

                PUBLIC x86_poly_filter
                PUBLIC x86_dewindow_and_write
                PUBLIC x86_IMDCT_3x12
                PUBLIC x86_IMDCT_1x36

             ENDIF
            ENDIF

IFNDEF SSE  ; only dewindow_and_write has an SSE version; in all other cases, the x87 code is used

;#############################################################################
;##                                                                         ##
;## 1x36 IMDCT for window types 0, 1, and 3 (long windows)                  ##
;##                                                                         ##
;#############################################################################

                ;
                ;C = C + B
                ;B = B + A
                ;

CASCADE_ADD     MACRO A,B,C

                IFDEF AMD

                  movd mm0,DWORD PTR [PTR_ESI+(A*4)]
                  movd mm1,DWORD PTR [PTR_ESI+(B*4)]
                  movd mm2,DWORD PTR [PTR_ESI+(C*4)]
                  pfadd mm2,mm1
                  pfadd mm1,mm0
                  movd DWORD PTR [PTR_ESI+(C*4)],mm2
                  movd DWORD PTR [PTR_ESI+(B*4)],mm1

                ELSE

                  fld DWORD PTR [PTR_ESI+(A*4)]
                  fld DWORD PTR [PTR_ESI+(B*4)]
                  fld DWORD PTR [PTR_ESI+(C*4)]
                  fadd st,st(1)
                  fxch st(1)
                  faddp st(2),st
                  fstp DWORD PTR [PTR_ESI+(C*4)]
                  fstp DWORD PTR [PTR_ESI+(B*4)]

                ENDIF

                ENDM

                ;
                ;dest = base + (a*fa) + (b*fb) + (c*fc)
                ;

MAC3            MACRO base,a,b,c,fa,fb,fc,dest

                IFDEF AMD

                  movd mm0,DWORD PTR [PTR_ESI+((a)*4)]
                  movd mm1,DWORD PTR [PTR_ESI+((b)*4)]
                  movd mm2,DWORD PTR [PTR_ESI+((c)*4)]
                  movd mm3,fa
                  movd mm4,fb
                  movd mm5,fc
                  pfmul mm3,mm0
                  pfmul mm4,mm1
                  pfmul mm5,mm2
                  movq mm0,base
                  pfadd mm0,mm3
                  pfadd mm4,mm5
                  pfadd mm0,mm4
                  movd DWORD PTR [PTR_EDI+((dest)*4)],mm0

                ELSE

                  fld DWORD PTR [PTR_ESI+((a)*4)]
                  fld DWORD PTR [PTR_ESI+((b)*4)]
                  fld DWORD PTR [PTR_ESI+((c)*4)]
                  fmul fc
                  fxch st(2)
                  fmul fa
                  fxch st(1)
                  fmul fb
                  fld base
                  faddp st(1),st
                  faddp st(1),st
                  faddp st(1),st
                  fstp DWORD PTR [PTR_EDI+((dest)*4)]

                ENDIF

                ENDM

                ;
                ;9-point IDCT
                ;

IDCT_9          MACRO src,dest

                IFDEF AMD

                  movd mm6,DWORD PTR [PTR_ESI+((src+0)*4)]
                  movd mm7,DWORD PTR [PTR_ESI+((src+12)*4)]
                  pfadd mm6,mm6
                  pfadd mm7,mm6
                  
                  MAC3 mm7,4+src,8+src,16+src,dct9t0,dct9t1,dct9t2,0+dest
                  MAC3 mm7,4+src,8+src,16+src,dct9t3,dct9t4,dct9t5,2+dest
                  MAC3 mm7,4+src,8+src,16+src,dct9t6,dct9t7,dct9t8,3+dest

                  movd mm0,DWORD PTR [PTR_ESI+((src+4)*4)]
                  movd mm1,DWORD PTR [PTR_ESI+((src+8)*4)]
                  movd mm2,DWORD PTR [PTR_ESI+((src+12)*4)]
                  movd mm3,DWORD PTR [PTR_ESI+((src+16)*4)]

                  movq mm4,mm6
                  movd mm5,DWORD PTR [PTR_ESI+((src+0)*4)]
                  pfadd mm4,mm0
                  pfsub mm5,mm0
                  pfsub mm4,mm1
                  pfadd mm5,mm1
                  pfsub mm4,mm2
                  pfsub mm5,mm2
                  pfsub mm4,mm2
                  pfadd mm5,mm3
                  pfsub mm4,mm3
                  movd DWORD PTR [PTR_EDI+((dest+4)*4)],mm5
                  movd DWORD PTR [PTR_EDI+((dest+1)*4)],mm4

                  movd mm0,dct9t9
                  movd mm6,DWORD PTR [PTR_ESI+((src+6)*4)]
                  pfmul mm6,mm0
                  pxor mm7,mm7
                  pfsub mm7,mm6

                  MAC3 mm6,2+src,10+src,14+src,dct9t10,dct9t11,dct9t12,5+dest
                  MAC3 mm7,2+src,10+src,14+src,dct9t13,dct9t14,dct9t15,6+dest
                  MAC3 mm7,2+src,10+src,14+src,dct9t16,dct9t17,dct9t18,7+dest
                  
                  movd mm0,DWORD PTR [PTR_ESI+((src+2)*4)]
                  movd mm1,DWORD PTR [PTR_ESI+((src+10)*4)]
                  movd mm2,DWORD PTR [PTR_ESI+((src+14)*4)]
                  movd mm3,dct9t9
                  pfsub mm0,mm1
                  pfsub mm0,mm2
                  pfmul mm0,mm3
                  movd DWORD PTR [PTR_EDI+((dest+8)*4)],mm0

                ELSE 

                  fld DWORD PTR [PTR_ESI+((src+0)*4)]
                  fadd st,st(0)
                  fst a
                  fadd DWORD PTR [PTR_ESI+((src+12)*4)]
                  fstp b
                  
                  MAC3 b,4+src,8+src,16+src,dct9t0,dct9t1,dct9t2,0+dest
                  MAC3 b,4+src,8+src,16+src,dct9t3,dct9t4,dct9t5,2+dest
                  MAC3 b,4+src,8+src,16+src,dct9t6,dct9t7,dct9t8,3+dest

                  fld a
                  fadd DWORD PTR [PTR_ESI+((src+4)*4)]
                  fsub DWORD PTR [PTR_ESI+((src+8)*4)]
                  fsub DWORD PTR [PTR_ESI+((src+12)*4)]
                  fsub DWORD PTR [PTR_ESI+((src+12)*4)]
                  fsub DWORD PTR [PTR_ESI+((src+16)*4)]
                  fstp DWORD PTR [PTR_EDI+((dest+1)*4)]
                  
                  fld DWORD PTR [PTR_ESI+((src+0)*4)]
                  fsub DWORD PTR [PTR_ESI+((src+4)*4)]
                  fadd DWORD PTR [PTR_ESI+((src+8)*4)]
                  fsub DWORD PTR [PTR_ESI+((src+12)*4)]
                  fadd DWORD PTR [PTR_ESI+((src+16)*4)]
                  fstp DWORD PTR [PTR_EDI+((dest+4)*4)]
                  
                  fld DWORD PTR [PTR_ESI+((src+6)*4)]
                  fmul dct9t9
                  fst a
                  fchs
                  fstp b
                  
                  MAC3 a,2+src,10+src,14+src,dct9t10,dct9t11,dct9t12,5+dest
                  MAC3 b,2+src,10+src,14+src,dct9t13,dct9t14,dct9t15,6+dest
                  MAC3 b,2+src,10+src,14+src,dct9t16,dct9t17,dct9t18,7+dest
                  
                  fld DWORD PTR [PTR_ESI+((src+2)*4)]
                  fsub DWORD PTR [PTR_ESI+((src+10)*4)]
                  fsub DWORD PTR [PTR_ESI+((src+14)*4)]
                  fmul dct9t9
                  fstp DWORD PTR [PTR_EDI+((dest+8)*4)]

                ENDIF

                ENDM

                ;
                ;a = (src1[0] + src2[0])
                ;b = (src1[9] + src2[9]) * f1
                ;
                ;dest1 = (a + b) * f2
                ;dest2 = (a - b) * f3
                ;

COMBINE_ADD     MACRO src1,src2,f1,f2,f3,dest1,dest2

                IFDEF AMD

                  movd mm0,DWORD PTR [PTR_EDI+(src1*4)]
                  movd mm1,DWORD PTR [PTR_EDI+((src1+9)*4)]
                  movd mm2,DWORD PTR [PTR_EDI+(src2*4)]
                  movd mm3,DWORD PTR [PTR_EDI+((src2+9)*4)]

                  movd mm4,f1
                  movd mm5,f2
                  movd mm6,f3

                  pfadd mm1,mm3
                  pfadd mm0,mm2
                  pfmul mm1,mm4

                  movq mm2,mm0
                  pfsub mm0,mm1
                  pfadd mm2,mm1
                  pfmul mm0,mm6
                  pfmul mm2,mm5
                  movd DWORD PTR [PTR_EBX+(dest2*4)],mm0
                  movd DWORD PTR [PTR_EBX+(dest1*4)],mm2
                  
                ELSE

                  fld DWORD PTR [PTR_EDI+((src1+9)*4)]
                  fadd DWORD PTR [PTR_EDI+((src2+9)*4)]
                  fld DWORD PTR [PTR_EDI+(src1*4)]
                  fadd DWORD PTR [PTR_EDI+(src2*4)]
                  fxch st(1)
                  fmul f1
                         
                  fld st(1)
                  fadd st,st(1)
                  fmul f2
                  fstp DWORD PTR [PTR_EBX+(dest1*4)]
                  
                  fsubp st(1),st
                  fmul f3
                  fstp DWORD PTR [PTR_EBX+(dest2*4)]

                ENDIF

                ENDM

                ;
                ;a = (src1[0] - src2[0])
                ;b = (src1[9] - src2[9]) * f1
                ;
                ;dest1 = (a + b) * f2
                ;dest2 = (a - b) * f3
                ;

COMBINE_SUB     MACRO src1,src2,f1,f2,f3,dest1,dest2

                IFDEF AMD

                  movd mm0,DWORD PTR [PTR_EDI+(src1*4)]
                  movd mm1,DWORD PTR [PTR_EDI+((src1+9)*4)]
                  movd mm2,DWORD PTR [PTR_EDI+(src2*4)]
                  movd mm3,DWORD PTR [PTR_EDI+((src2+9)*4)]

                  movd mm4,f1
                  movd mm5,f2
                  movd mm6,f3

                  pfsub mm1,mm3
                  pfsub mm0,mm2
                  pfmul mm1,mm4

                  movq mm2,mm0
                  pfsub mm0,mm1
                  pfadd mm2,mm1
                  pfmul mm0,mm6
                  pfmul mm2,mm5
                  movd DWORD PTR [PTR_EBX+(dest2*4)],mm0
                  movd DWORD PTR [PTR_EBX+(dest1*4)],mm2
                  
                ELSE

                  fld DWORD PTR [PTR_EDI+((src1+9)*4)]
                  fsub DWORD PTR [PTR_EDI+((src2+9)*4)]
                  fld DWORD PTR [PTR_EDI+(src1*4)]
                  fsub DWORD PTR [PTR_EDI+(src2*4)]
                  fxch st(1)
                  fmul f1
                         
                  fld st(1)
                  fadd st,st(1)
                  fmul f2
                  fstp DWORD PTR [PTR_EBX+(dest1*4)]
                  
                  fsubp st(1),st
                  fmul f3
                  fstp DWORD PTR [PTR_EBX+(dest2*4)]

                ENDIF
                  
                ENDM

                ;
                ;A = (-B * C) + D
                ;E = ( F * G) - H
                ;

WINDOW_0110     MACRO A,B,C,D,E,F,G,H

                IFDEF AMD

                  movd mm0,DWORD PTR [PTR_EBX+(B*4)]
                  movd mm1,DWORD PTR [PTR_ESI+(C*4)]
                  movd mm2,DWORD PTR [PTR_ECX+(D*4)]
                  movd mm3,DWORD PTR [PTR_EBX+(F*4)]
                  movd mm4,DWORD PTR [PTR_ESI+(G*4)]
                  movd mm5,DWORD PTR [PTR_ECX+(H*4)]
                  pfsubr mm0,mm7
                  pfmul mm3,mm4
                  pfmul mm0,mm1
                  pfsub mm3,mm5
                  pfadd mm0,mm2
                  movd DWORD PTR [PTR_EDI+(E*4)],mm3
                  movd DWORD PTR [PTR_EDI+(A*4)],mm0

                ELSE

                  fld DWORD PTR [PTR_EBX+(F*4)]
                  fld DWORD PTR [PTR_EBX+(B*4)]
                  fchs
                  fmul DWORD PTR [PTR_ESI+(C*4)]
                  fadd DWORD PTR [PTR_ECX+(D*4)]
                  fxch st(1)

                  fmul DWORD PTR [PTR_ESI+(G*4)]
                  fsub DWORD PTR [PTR_ECX+(H*4)]
                  fstp DWORD PTR [PTR_EDI+(E*4)]
                  fstp DWORD PTR [PTR_EDI+(A*4)]
                  
                ENDIF

                ENDM

                ;
                ;WINDOW_0100: A = (-B * C) + D
                ;             E = (-F * G) - H
                ;

WINDOW_0100     MACRO A,B,C,D,E,F,G,H

                IFDEF AMD

                  movd mm0,DWORD PTR [PTR_EBX+(B*4)]
                  movd mm1,DWORD PTR [PTR_ESI+(C*4)]
                  movd mm2,DWORD PTR [PTR_ECX+(D*4)]
                  movd mm3,DWORD PTR [PTR_EBX+(F*4)]
                  movd mm4,DWORD PTR [PTR_ESI+(G*4)]
                  movd mm5,DWORD PTR [PTR_ECX+(H*4)]
                  pfsubr mm3,mm7
                  pfsubr mm0,mm7
                  pfmul mm3,mm4
                  pfmul mm0,mm1
                  pfsub mm3,mm5
                  pfadd mm0,mm2
                  movd DWORD PTR [PTR_EDI+(E*4)],mm3
                  movd DWORD PTR [PTR_EDI+(A*4)],mm0

                ELSE

                  fld DWORD PTR [PTR_EBX+(F*4)]
                  fld DWORD PTR [PTR_EBX+(B*4)]
                  fchs
                  fmul DWORD PTR [PTR_ESI+(C*4)]
                  fadd DWORD PTR [PTR_ECX+(D*4)]
                  fxch st(1)
                  
                  fchs
                  fmul DWORD PTR [PTR_ESI+(G*4)]
                  fsub DWORD PTR [PTR_ECX+(H*4)]
                  fstp DWORD PTR [PTR_EDI+(E*4)]
                  fstp DWORD PTR [PTR_EDI+(A*4)]

                ENDIF

                ENDM

                ;
                ;WINDOW_1100: A = ( B * C) + D
                ;             E = (-F * G) - H
                ;

WINDOW_1100     MACRO A,B,C,D,E,F,G,H

                IFDEF AMD

                  movd mm0,DWORD PTR [PTR_EBX+(B*4)]
                  movd mm1,DWORD PTR [PTR_ESI+(C*4)]
                  movd mm2,DWORD PTR [PTR_ECX+(D*4)]
                  movd mm3,DWORD PTR [PTR_EBX+(F*4)]
                  movd mm4,DWORD PTR [PTR_ESI+(G*4)]
                  pfsubr mm3,mm7
                  movd mm5,DWORD PTR [PTR_ECX+(H*4)]
                  pfmul mm3,mm4
                  pfmul mm0,mm1
                  pfsub mm3,mm5
                  pfadd mm0,mm2
                  movd DWORD PTR [PTR_EDI+(E*4)],mm3
                  movd DWORD PTR [PTR_EDI+(A*4)],mm0

                ELSE

                  fld DWORD PTR [PTR_EBX+(F*4)]
                  fld DWORD PTR [PTR_EBX+(B*4)]
                  fmul DWORD PTR [PTR_ESI+(C*4)]
                  fadd DWORD PTR [PTR_ECX+(D*4)]
                  fxch st(1)

                  fchs
                  fmul DWORD PTR [PTR_ESI+(G*4)]
                  fsub DWORD PTR [PTR_ECX+(H*4)]
                  fstp DWORD PTR [PTR_EDI+(E*4)]
                  fstp DWORD PTR [PTR_EDI+(A*4)]

                ENDIF

                ENDM

                ;
                ;WINDOW_0101: A = (-B * C) + D
                ;             E = (-F * G) + H
                ;

WINDOW_0101     MACRO A,B,C,D,E,F,G,H

                IFDEF AMD

                  movd mm0,DWORD PTR [PTR_EBX+(B*4)]
                  movd mm1,DWORD PTR [PTR_ESI+(C*4)]
                  movd mm2,DWORD PTR [PTR_ECX+(D*4)]
                  movd mm3,DWORD PTR [PTR_EBX+(F*4)]
                  movd mm4,DWORD PTR [PTR_ESI+(G*4)]
                  movd mm5,DWORD PTR [PTR_ECX+(H*4)]
                  pfsubr mm3,mm7
                  pfsubr mm0,mm7
                  pfmul mm3,mm4
                  pfmul mm0,mm1
                  pfadd mm3,mm5
                  pfadd mm0,mm2
                  movd DWORD PTR [PTR_EDI+(E*4)],mm3
                  movd DWORD PTR [PTR_EDI+(A*4)],mm0

                ELSE

                  fld DWORD PTR [PTR_EBX+(F*4)]
                  fld DWORD PTR [PTR_EBX+(B*4)]
                  fchs
                  fmul DWORD PTR [PTR_ESI+(C*4)]
                  fadd DWORD PTR [PTR_ECX+(D*4)]
                  fxch st(1)
                 
                  fchs
                  fmul DWORD PTR [PTR_ESI+(G*4)]
                  fadd DWORD PTR [PTR_ECX+(H*4)]
                  fstp DWORD PTR [PTR_EDI+(E*4)]
                  fstp DWORD PTR [PTR_EDI+(A*4)]

                ENDIF

                ENDM

                ;
                ;WINDOW_0111: A = (-B * C) + D
                ;             E = ( F * G) + H
                ;

WINDOW_0111     MACRO A,B,C,D,E,F,G,H

                IFDEF AMD

                  movd mm0,DWORD PTR [PTR_EBX+(B*4)]
                  movd mm1,DWORD PTR [PTR_ESI+(C*4)]
                  movd mm2,DWORD PTR [PTR_ECX+(D*4)]
                  movd mm3,DWORD PTR [PTR_EBX+(F*4)]
                  movd mm4,DWORD PTR [PTR_ESI+(G*4)]
                  movd mm5,DWORD PTR [PTR_ECX+(H*4)]
                  pfsubr mm0,mm7
                  pfmul mm3,mm4
                  pfmul mm0,mm1
                  pfadd mm3,mm5
                  pfadd mm0,mm2
                  movd DWORD PTR [PTR_EDI+(E*4)],mm3
                  movd DWORD PTR [PTR_EDI+(A*4)],mm0

                ELSE

                  fld DWORD PTR [PTR_EBX+(F*4)]
                  fld DWORD PTR [PTR_EBX+(B*4)]
                  fchs
                  fmul DWORD PTR [PTR_ESI+(C*4)]
                  fadd DWORD PTR [PTR_ECX+(D*4)]
                  fxch st(1)
                
                  fmul DWORD PTR [PTR_ESI+(G*4)]
                  fadd DWORD PTR [PTR_ECX+(H*4)]
                  fstp DWORD PTR [PTR_EDI+(E*4)]
                  fstp DWORD PTR [PTR_EDI+(A*4)]

                ENDIF

                ENDM

                ;
                ;WINDOW_1111: A = ( B * C) + D
                ;             E = ( F * G) + H
                ;

WINDOW_1111     MACRO A,B,C,D,E,F,G,H

                IFDEF AMD

                  movd mm0,DWORD PTR [PTR_EBX+(B*4)]
                  movd mm1,DWORD PTR [PTR_ESI+(C*4)]
                  movd mm2,DWORD PTR [PTR_ECX+(D*4)]
                  movd mm3,DWORD PTR [PTR_EBX+(F*4)]
                  movd mm4,DWORD PTR [PTR_ESI+(G*4)]
                  movd mm5,DWORD PTR [PTR_ECX+(H*4)]
                  pfmul mm3,mm4
                  pfmul mm0,mm1
                  pfadd mm3,mm5
                  pfadd mm0,mm2
                  movd DWORD PTR [PTR_EDI+(E*4)],mm3
                  movd DWORD PTR [PTR_EDI+(A*4)],mm0

                ELSE

                  fld DWORD PTR [PTR_EBX+(F*4)]
                  fld DWORD PTR [PTR_EBX+(B*4)]
                  fmul DWORD PTR [PTR_ESI+(C*4)]
                  fadd DWORD PTR [PTR_ECX+(D*4)]
                  fxch st(1)

                  fmul DWORD PTR [PTR_ESI+(G*4)]
                  fadd DWORD PTR [PTR_ECX+(H*4)]
                  fstp DWORD PTR [PTR_EDI+(E*4)]
                  fstp DWORD PTR [PTR_EDI+(A*4)]

                ENDIF

                ENDM

                ;
                ;p1 = m1*m2
                ;p2 = m3*m4
                ;

MULT_PAIR       MACRO m1,m2,p1,m3,m4,p2

                IFDEF AMD

                  movd mm0,DWORD PTR m1
                  movd mm1,DWORD PTR m2
                  movd mm2,DWORD PTR m3
                  movd mm3,DWORD PTR m4
                  pfmul mm0,mm1
                  pfmul mm2,mm3
                  movd DWORD PTR p1,mm0
                  movd DWORD PTR p2,mm2

                ELSE

                  fld DWORD PTR m1
                  fld DWORD PTR m3
                  fmul DWORD PTR m4
                  fxch st(1)
                  fmul DWORD PTR m2
                  fxch st(1)
                  fstp DWORD PTR p2
                  fstp DWORD PTR p1

                ENDIF
                
                ENDM

                ;
                ;p1 = m1*m2
                ;

MULT            MACRO m1,m2,p1

                IFDEF AMD

                  movd mm0,m1
                  movd mm1,m2
                  pfmul mm0,mm1
                  movd p1,mm0

                ELSE

                  fld DWORD PTR m1
                  fmul DWORD PTR m2
                  fstp DWORD PTR p1

                ENDIF

                ENDM

                IFDEF AMD
                  IFDEF WIN64

AMD_IMDCT_1x36    PROC
                  ENTER64 152

lpIn              TEXTEQU <QWORD PTR [r10][1*8]> 
dwSB              TEXTEQU <DWORD PTR [r10][2*8]>    
lpResult          TEXTEQU <QWORD PTR [r10][3*8]>    
lpSave            TEXTEQU <QWORD PTR [r10][4*8]>    
lpWindow          TEXTEQU <QWORD PTR [r10][5*8]>    

                  ELSE

AMD_IMDCT_1x36    PROC \
                  USES PTR_EBX PTR_ESI PTR_EDI \
                  lpIn:PTR,\
                  dwSB:DWORD,\
                  lpResult:PTR,\
                  lpSave:PTR,\
                  lpWindow:PTR

                  ENDIF

                ELSE
                  IFDEF WIN64

x86_IMDCT_1x36    PROC
                  ENTER64 152

lpIn              TEXTEQU <QWORD PTR [r10][1*8]> 
dwSB              TEXTEQU <DWORD PTR [r10][2*8]>    
lpResult          TEXTEQU <QWORD PTR [r10][3*8]>    
lpSave            TEXTEQU <QWORD PTR [r10][4*8]>    
lpWindow          TEXTEQU <QWORD PTR [r10][5*8]>    

                  ELSE

x86_IMDCT_1x36    PROC \
                  USES PTR_EBX PTR_ESI PTR_EDI \
                  lpIn:PTR,\
                  dwSB:DWORD,\
                  lpResult:PTR,\
                  lpSave:PTR,\
                  lpWindow:PTR

                  ENDIF
                ENDIF

                IFDEF WIN64
a                 equ DWORD PTR [PTR_EBP-4]
b                 equ DWORD PTR [PTR_EBP-8]
temp1             equ DWORD PTR [PTR_EBP-80]
temp2             equ DWORD PTR [PTR_EBP-152]
                ELSE
                  LOCAL a
                  LOCAL b
                  LOCAL temp1[18]
                  LOCAL temp2[18]
                ENDIF

                mov PTR_ESI,lpIn
                lea PTR_EDI,temp1
                lea PTR_EBX,temp2

                _FEMMS

                CASCADE_ADD 15,16,17
                CASCADE_ADD 13,14,15
                CASCADE_ADD 11,12,13
                CASCADE_ADD 9,10,11
                CASCADE_ADD 7,8,9
                CASCADE_ADD 5,6,7
                CASCADE_ADD 3,4,5
                CASCADE_ADD 1,2,3

                IFDEF AMD

                  movd mm0,DWORD PTR [PTR_ESI+(1*4)]
                  movd mm1,DWORD PTR [PTR_ESI+(0*4)]
                  pfadd mm0,mm1
                  movd DWORD PTR [PTR_ESI+(1*4)],mm0

                ELSE

                  fld DWORD PTR [PTR_ESI+(1*4)]
                  fadd DWORD PTR [PTR_ESI+(0*4)]
                  fstp DWORD PTR [PTR_ESI+(1*4)]

                ENDIF

                CASCADE_ADD 13,15,17
                CASCADE_ADD 9,11,13
                CASCADE_ADD 5,7,9
                CASCADE_ADD 1,3,5

                IDCT_9 0,0
                IDCT_9 1,9

                COMBINE_ADD 0,5, dct36t0,dct36t1,dct36t2,    0,17
                COMBINE_ADD 1,8, dct36t3,dct36t4,dct36t5,    1,16
                COMBINE_ADD 2,6, dct36t6,dct36t7,dct36t8,    2,15
                COMBINE_ADD 3,7, dct36t9,dct36t10,dct36t11,  3,14

                COMBINE_SUB 3,7, dct36t12,dct36t13,dct36t14, 5,12
                COMBINE_SUB 2,6, dct36t15,dct36t16,dct36t17, 6,11
                COMBINE_SUB 1,8, dct36t18,dct36t19,dct36t20, 7,10
                COMBINE_SUB 0,5, dct36t21,dct36t22,dct36t23, 8,9

                IFDEF AMD

                  movd mm0,dct36t24
                  movd mm1,dct36t25
                  movd mm2,dct36t26

                  movd mm3,DWORD PTR [PTR_EDI+(13*4)]
                  movd mm4,DWORD PTR [PTR_EDI+(4*4)]

                  pfmul mm3,mm0
                  movq mm5,mm4

                  pfadd mm4,mm3
                  pfsub mm5,mm3
                  pfmul mm4,mm1
                  pfmul mm5,mm2

                  movd DWORD PTR [PTR_EBX+(4*4)],mm4
                  movd DWORD PTR [PTR_EBX+(13*4)],mm5

                  pxor mm7,mm7          ;(used to change sign in window routines below)

                ELSE

                  fld DWORD PTR [PTR_EDI+(13*4)]
                  fmul dct36t24
                  fstp b
                         
                  fld DWORD PTR [PTR_EDI+(4*4)]
                  fadd b
                  fmul dct36t25
                  fstp DWORD PTR [PTR_EBX+(4*4)]
                  
                  fld DWORD PTR [PTR_EDI+(4*4)]
                  fsub b
                  fmul dct36t26
                  fstp DWORD PTR [PTR_EBX+(13*4)]

                ENDIF

                mov PTR_ESI,lpWindow
                mov PTR_EDI,lpResult
                mov PTR_ECX,lpSave

                test dwSB,1
                jz __even_window

                WINDOW_0110 0,9,0,0,1,10,1,1
                WINDOW_0110 2,11,2,2,3,12,3,3
                WINDOW_0110 4,13,4,4,5,14,5,5
                WINDOW_0110 6,15,6,6,7,16,7,7
                WINDOW_0100 8,17,8,8,9,17,9,9
                WINDOW_1100 10,16,10,10,11,15,11,11
                WINDOW_1100 12,14,12,12,13,13,13,13
                WINDOW_1100 14,12,14,14,15,11,15,15
                WINDOW_1100 16,10,16,16,17,9,17,17
                jmp __save

__even_window:  WINDOW_0101 0,9,0,0,1,10,1,1
                WINDOW_0101 2,11,2,2,3,12,3,3
                WINDOW_0101 4,13,4,4,5,14,5,5
                WINDOW_0101 6,15,6,6,7,16,7,7
                WINDOW_0111 8,17,8,8,9,17,9,9
                WINDOW_1111 10,16,10,10,11,15,11,11
                WINDOW_1111 12,14,12,12,13,13,13,13
                WINDOW_1111 14,12,14,14,15,11,15,15
                WINDOW_1111 16,10,16,16,17,9,17,17

__save:         MULT_PAIR [PTR_EBX+(8*4)], [PTR_ESI+(18*4)], [PTR_ECX+(0*4)],\
                          [PTR_EBX+(7*4)], [PTR_ESI+(19*4)], [PTR_ECX+(1*4)]
                MULT_PAIR [PTR_EBX+(6*4)], [PTR_ESI+(20*4)], [PTR_ECX+(2*4)],\
                          [PTR_EBX+(5*4)], [PTR_ESI+(21*4)], [PTR_ECX+(3*4)]
                MULT_PAIR [PTR_EBX+(4*4)], [PTR_ESI+(22*4)], [PTR_ECX+(4*4)],\
                          [PTR_EBX+(3*4)], [PTR_ESI+(23*4)], [PTR_ECX+(5*4)]
                MULT_PAIR [PTR_EBX+(2*4)], [PTR_ESI+(24*4)], [PTR_ECX+(6*4)],\
                          [PTR_EBX+(1*4)], [PTR_ESI+(25*4)], [PTR_ECX+(7*4)]
                MULT_PAIR [PTR_EBX+(0*4)], [PTR_ESI+(26*4)], [PTR_ECX+(8*4)],\
                          [PTR_EBX+(0*4)], [PTR_ESI+(27*4)], [PTR_ECX+(9*4)]
                MULT_PAIR [PTR_EBX+(1*4)], [PTR_ESI+(28*4)], [PTR_ECX+(10*4)],\
                          [PTR_EBX+(2*4)], [PTR_ESI+(29*4)], [PTR_ECX+(11*4)]
                MULT_PAIR [PTR_EBX+(3*4)], [PTR_ESI+(30*4)], [PTR_ECX+(12*4)],\
                          [PTR_EBX+(4*4)], [PTR_ESI+(31*4)], [PTR_ECX+(13*4)]
                MULT_PAIR [PTR_EBX+(5*4)], [PTR_ESI+(32*4)], [PTR_ECX+(14*4)],\
                          [PTR_EBX+(6*4)], [PTR_ESI+(33*4)], [PTR_ECX+(15*4)]
                MULT_PAIR [PTR_EBX+(7*4)], [PTR_ESI+(34*4)], [PTR_ECX+(16*4)],\
                          [PTR_EBX+(8*4)], [PTR_ESI+(35*4)], [PTR_ECX+(17*4)]

                _FEMMS

                IFDEF WIN64
                EXIT64
                ENDIF
                ret

                IFDEF AMD
AMD_IMDCT_1x36    ENDP
                ELSE
x86_IMDCT_1x36    ENDP
                ENDIF

;#############################################################################
;##                                                                         ##
;## 3x12 IMDCT for window type 2 (short windows)                            ##
;##                                                                         ##
;#############################################################################

                ;
                ;dest += src
                ;

ACCUM           MACRO dest,src

                IFDEF AMD

                  movd mm0,DWORD PTR dest
                  movd mm1,DWORD PTR src
                  pfadd mm0,mm1
                  movd DWORD PTR dest,mm0

                ELSE

                  fld DWORD PTR dest
                  fadd DWORD PTR src
                  fstp DWORD PTR dest

                ENDIF

                ENDM

                ;
                ;dest1 += src1
                ;dest2 += src2
                ;

ACCUM_PAIR      MACRO dest1,src1,dest2,src2

                IFDEF AMD

                  movd mm0,DWORD PTR dest1
                  movd mm1,DWORD PTR src1
                  movd mm2,DWORD PTR dest2
                  movd mm3,DWORD PTR src2
                  pfadd mm0,mm1
                  pfadd mm2,mm3
                  movd DWORD PTR dest1,mm0
                  movd DWORD PTR dest2,mm2

                ELSE

                  fld DWORD PTR dest1
                  fld DWORD PTR dest2
                  fadd DWORD PTR src2
                  fxch st(1)
                  fadd DWORD PTR src1
                  fxch st(1)
                  fstp DWORD PTR dest2
                  fstp DWORD PTR dest1

                ENDIF

                ENDM

                ;
                ;t1 = in2 * cos(pi/6)
                ;t2 = in1 * sin(pi/6)
                ;t3 = in3 + t2;
                ;out1 = in3 - in1;
                ;out2 = t3 + t1;
                ;out3 = t3 - t1;
                ;

IDCT_3          MACRO in1,in2,in3,out1,out2,out3

                IFDEF AMD

                  movd mm3,DWORD PTR in3
                  movd mm2,DWORD PTR in2
                  movd mm1,DWORD PTR in1
                  movq mm5,mm3
                  movd mm6,sin30
                  movd mm7,cos30
                  pfsub mm5,mm1
                  pfmul mm1,mm6 ;mm1=t2
                  pfmul mm2,mm7 ;mm2=t1
                  pfadd mm3,mm1 ;mm3=t3
                  movq mm0,mm2
                  pfadd mm2,mm3
                  pfsub mm3,mm0
                  movd DWORD PTR out1,mm5
                  movd DWORD PTR out2,mm2
                  movd DWORD PTR out3,mm3

                ELSE

                  fld DWORD PTR in1
                  fld DWORD PTR in2
                  fmul cos30
                  fxch st(1)
                  fmul sin30
                  fld DWORD PTR in3
                  faddp st(1),st
                  fdup
                  fld DWORD PTR in3
                  fsub DWORD PTR in1
                  fstp DWORD PTR out1
                  fadd st,st(2)   
                  fstp DWORD PTR out2
                  fsubrp st(1),st
                  fstp DWORD PTR out3

                ENDIF

                ENDM

                IFDEF AMD
                  IFDEF WIN64

AMD_IMDCT_3x12    PROC
                  ENTER64 216

lpIn              TEXTEQU <QWORD PTR [r10][1*8]> 
dwSB              TEXTEQU <DWORD PTR [r10][2*8]>    
lpResult          TEXTEQU <QWORD PTR [r10][3*8]>    
lpSave            TEXTEQU <QWORD PTR [r10][4*8]>    

                  ELSE

AMD_IMDCT_3x12    PROC \
                  USES PTR_EBX PTR_ESI PTR_EDI \
                  lpIn:PTR,\
                  dwSB:DWORD,\
                  lpResult:PTR,\
                  lpSave:PTR

                  ENDIF

                ELSE

                  IFDEF WIN64

x86_IMDCT_3x12    PROC
                  ENTER64 216

lpIn              TEXTEQU <QWORD PTR [r10][1*8]> 
dwSB              TEXTEQU <DWORD PTR [r10][2*8]>    
lpResult          TEXTEQU <QWORD PTR [r10][3*8]>    
lpSave            TEXTEQU <QWORD PTR [r10][4*8]>    

                  ELSE

x86_IMDCT_3x12    PROC \
                  USES PTR_EBX PTR_ESI PTR_EDI \
                  lpIn:PTR,\
                  dwSB:DWORD,\
                  lpResult:PTR,\
                  lpSave:PTR

                  ENDIF
                ENDIF

                IFDEF WIN64
loop_end          equ QWORD PTR [PTR_EBP-8]
temp              equ DWORD PTR [PTR_EBP-72]
output            equ DWORD PTR [PTR_EBP-216]
                ELSE
                  LOCAL loop_end
                  LOCAL temp[16]
                  LOCAL output[36]
                ENDIF

                fldz
                I = 0
                REPT 17

                  fst QWORD PTR output[I]

                I = I + 8
                ENDM
                fstp QWORD PTR output[I]

                _FEMMS

                mov PTR_ESI,lpIn
                lea PTR_EDI,output
                
                lea PTR_EAX,[PTR_ESI][18 * SIZE DWORD]
                mov loop_end,PTR_EAX

__for_DCT:      ACCUM_PAIR [PTR_ESI+(5*4)], [PTR_ESI+(4*4)], [PTR_ESI+(4*4)], [PTR_ESI+(3*4)]
                ACCUM_PAIR [PTR_ESI+(3*4)], [PTR_ESI+(2*4)], [PTR_ESI+(2*4)], [PTR_ESI+(1*4)]
                ACCUM_PAIR [PTR_ESI+(1*4)], [PTR_ESI+(0*4)], [PTR_ESI+(5*4)], [PTR_ESI+(3*4)]
                ACCUM      [PTR_ESI+(3*4)], [PTR_ESI+(1*4)]

                IDCT_3 [PTR_ESI+(4*4)], [PTR_ESI+(2*4)], [PTR_ESI+(0*4)], temp[1*4], temp[0*4], temp[2*4]
                IDCT_3 [PTR_ESI+(5*4)], [PTR_ESI+(3*4)], [PTR_ESI+(1*4)], temp[4*4], temp[5*4], temp[3*4]

                MULT_PAIR temp[3*4],dct3t1,temp[3*4],temp[4*4],dct3t2,temp[4*4]
                MULT      temp[5*4],dct3t3,temp[5*4]

                IFDEF AMD

                  movd mm0,temp[0*4]
                  movd mm1,temp[1*4]
                  movd mm2,temp[2*4]
                  movd mm6,temp[5*4]
                  movd mm7,temp[4*4]
                  movq mm3,mm0
                  movq mm4,mm1
                  movq mm5,mm2
                  pfadd mm0,mm6
                  pfsub mm3,mm6
                  movd temp[0*4],mm0
                  movd temp[5*4],mm3
                  pfadd mm1,mm7
                  pfsub mm4,mm7
                  movd mm6,temp[3*4]
                  movd temp[1*4],mm1
                  movd temp[4*4],mm4
                  pfadd mm2,mm6
                  pfsub mm5,mm6
                  movd temp[2*4],mm2
                  movd temp[3*4],mm5

                ELSE

                  fld DWORD PTR temp[0*4]
                  fdup
                  fadd DWORD PTR temp[5*4]
                  fstp DWORD PTR temp[0*4]
                  fsub DWORD PTR temp[5*4]
                  fstp DWORD PTR temp[5*4]
                  
                  fld DWORD PTR temp[1*4]
                  fdup
                  fadd DWORD PTR temp[4*4]
                  fstp DWORD PTR temp[1*4]
                  fsub DWORD PTR temp[4*4]
                  fstp DWORD PTR temp[4*4]
                  
                  fld DWORD PTR temp[2*4]
                  fdup
                  fadd DWORD PTR temp[3*4]
                  fstp DWORD PTR temp[2*4]
                  fsub DWORD PTR temp[3*4]
                  fstp DWORD PTR temp[3*4]

                ENDIF

                MULT_PAIR temp[0*4],dct3t4,temp[0*4],temp[1*4],dct3t5,temp[1*4]
                MULT_PAIR temp[2*4],dct3t6,temp[2*4],temp[3*4],dct3t7,temp[3*4]
                MULT_PAIR temp[4*4],dct3t8,temp[4*4],temp[5*4],dct3t9,temp[5*4]

                MULT_PAIR dct3t10,temp[0*4],temp[8*4],dct3t11,temp[0*4],temp[9*4]
                MULT_PAIR dct3t12,temp[1*4],temp[7*4],dct3t13,temp[1*4],temp[10*4]
                MULT_PAIR dct3t14,temp[2*4],temp[6*4],dct3t15,temp[2*4],temp[11*4]
                MULT_PAIR dct3t16,temp[3*4],temp[0*4],dct3t17,temp[4*4],temp[1*4] 
                MULT_PAIR dct3t18,temp[5*4],temp[2*4],dct3t19,temp[5*4],temp[3*4] 
                MULT_PAIR dct3t20,temp[4*4],temp[4*4],dct3t21,temp[0*4],temp[5*4] 
                MULT      dct3t22,temp[0*4],temp[0*4]

                ACCUM_PAIR [PTR_EDI+(6*4)], temp[0*4], [PTR_EDI+(7*4)], temp[1*4]
                ACCUM_PAIR [PTR_EDI+(8*4)], temp[2*4], [PTR_EDI+(9*4)], temp[3*4]
                ACCUM_PAIR [PTR_EDI+(10*4)],temp[4*4], [PTR_EDI+(11*4)],temp[5*4]
                ACCUM_PAIR [PTR_EDI+(12*4)],temp[6*4], [PTR_EDI+(13*4)],temp[7*4]
                ACCUM_PAIR [PTR_EDI+(14*4)],temp[8*4], [PTR_EDI+(15*4)],temp[9*4]
                ACCUM_PAIR [PTR_EDI+(16*4)],temp[10*4],[PTR_EDI+(17*4)],temp[11*4]

                add PTR_ESI,6 * SIZE DWORD
                add PTR_EDI,6 * SIZE DWORD
                cmp PTR_ESI,loop_end
                jne __for_DCT

                _FEMMS

                lea PTR_ESI,output
                mov PTR_EDI,lpResult
                mov PTR_EBX,lpSave

                lea PTR_EAX,[PTR_ESI][18 * SIZE DWORD]
                mov loop_end,PTR_EAX

                test dwSB,1
                jz __even_overlap

__odd_overlap:  
                I = 0
                REPT 9
                
                  fld DWORD PTR [PTR_ESI+I]
                  fadd DWORD PTR [PTR_EBX+I]
                  fstp DWORD PTR [PTR_EDI+I]
                
                  fld DWORD PTR [PTR_ESI+SIZE DWORD+I]
                  fchs
                  fsub DWORD PTR [PTR_EBX+SIZE DWORD+I]
                  fstp DWORD PTR [PTR_EDI+SIZE DWORD+I]

                I = I + 8
                ENDM
                jmp __overlap_done

__even_overlap: 
                I = 0
                REPT 18

                  fld DWORD PTR [PTR_ESI+I]
                  fadd DWORD PTR [PTR_EBX+I]
                  fstp DWORD PTR [PTR_EDI+I]

                I = I + 4
                ENDM

__overlap_done: add PTR_EBX,18 * SIZE DWORD
                add PTR_ESI,18 * SIZE DWORD
                add PTR_EDI,18 * SIZE DWORD

                I = 0
                REPT 18

                  fld DWORD PTR [PTR_ESI+I]
                  fstp DWORD PTR [PTR_EBX+I-(18 * SIZE DWORD)]

                I = I + 4
                ENDM
                IFDEF WIN64
                EXIT64
                ENDIF
                ret

                IFDEF AMD
AMD_IMDCT_3x12    ENDP
                ELSE
x86_IMDCT_3x12    ENDP
                ENDIF

;#############################################################################
;##                                                                         ##
;## IDCT reordering and butterflies for polyphase filter                    ##
;##                                                                         ##
;#############################################################################

                ;
                ;B = (A - C) * D; d1 = A + C;
                ;F = (E - G) * H; d2 = E + G;
                ;
                ;Separation between successive A,C,E,G elements determined
                ;by stride -- 18 in the case of the initial r[][18] array,
                ;1 in the case of the intermediate arrays
                ;

                IFDEF AMD

REORD_PAIR      MACRO stride,dest1,A,B,C,D,dest2,E,F,G,H

                movd mm1,DWORD PTR [PTR_ESI+(A*stride*4)]
                movd mm3,DWORD PTR [PTR_ESI+(C*stride*4)]
                movd mm4,DWORD PTR [PTR_EBX+(D*4)]
                movq mm2,mm1
                pfsub mm1,mm3

                movd mm5,DWORD PTR [PTR_ESI+(C*stride*4)]
                movd mm6,DWORD PTR [PTR_ESI+(G*stride*4)]

                pfmul mm1,mm4
                pfadd mm2,mm5

                movd DWORD PTR [PTR_EDI+(B*4)],mm1
                movd DWORD PTR [PTR_EDI+(dest1*4)],mm2
                
                movd mm1,DWORD PTR [PTR_ESI+(E*stride*4)]
                movd mm7,DWORD PTR [PTR_EBX+(H*4)]
                movq mm2,mm1
                pfsub mm1,mm6
                movd mm0,DWORD PTR [PTR_ESI+(G*stride*4)]

                pfmul mm1,mm7
                pfadd mm2,mm0

                movd DWORD PTR [PTR_EDI+(F*4)],mm1
                movd DWORD PTR [PTR_EDI+(dest2*4)],mm2

                ENDM

                ELSE

REORD_PAIR      MACRO stride,dest1,A,B,C,D,dest2,E,F,G,H

                fld DWORD PTR [PTR_ESI+(E*stride*4)]    
                fdup
                fld DWORD PTR [PTR_ESI+(A*stride*4)]    
                fdup                            

                fsub DWORD PTR [PTR_ESI+(C*stride*4)]   
                fmul DWORD PTR [PTR_EBX+(D*4)]
                fxch st(2)
                fsub DWORD PTR [PTR_ESI+(G*stride*4)]   
                fmul DWORD PTR [PTR_EBX+(H*4)]
                fxch st(2)                     

                fstp DWORD PTR [PTR_EDI+(B*4)]
                fadd DWORD PTR [PTR_ESI+(C*stride*4)]
                fstp DWORD PTR [PTR_EDI+(dest1*4)]

                fstp DWORD PTR [PTR_EDI+(F*4)]
                fadd DWORD PTR [PTR_ESI+(G*stride*4)]
                fstp DWORD PTR [PTR_EDI+(dest2*4)]

                ENDM

                ENDIF

                IFDEF AMD
                  IFDEF WIN64

AMD_poly_filter   PROC
                  ENTER64 192

lpSrc             TEXTEQU <QWORD PTR [r10][1*8]> 
lpBArray          TEXTEQU <QWORD PTR [r10][2*8]>    
dwPhase           TEXTEQU <DWORD PTR [r10][3*8]>    
lpOut1            TEXTEQU <QWORD PTR [r10][4*8]>    
lpOut2            TEXTEQU <QWORD PTR [r10][5*8]>    

                  ELSE

AMD_poly_filter   PROC \
                  USES PTR_EBX PTR_ESI PTR_EDI \
                  lpSrc:PTR,\
                  lpBArray:PTR,\
                  dwPhase:DWORD,\
                  lpOut1:PTR,\
                  lpOut2:PTR

                  ENDIF

                ELSE

                  IFDEF WIN64

x86_poly_filter   PROC
                  ENTER64 192

lpSrc             TEXTEQU <QWORD PTR [r10][1*8]> 
lpBArray          TEXTEQU <QWORD PTR [r10][2*8]>    
dwPhase           TEXTEQU <DWORD PTR [r10][3*8]>    
lpOut1            TEXTEQU <QWORD PTR [r10][4*8]>    
lpOut2            TEXTEQU <QWORD PTR [r10][5*8]>    

                  ELSE

x86_poly_filter   PROC \
                  USES PTR_EBX PTR_ESI PTR_EDI \
                  lpSrc:PTR,\
                  lpBArray:PTR,\
                  dwPhase:DWORD,\
                  lpOut1:PTR,\
                  lpOut2:PTR

                  ENDIF
                ENDIF

                IFDEF WIN64
temp1             equ [PTR_EBP-128]
temp2             equ [PTR_EBP-192]
                ELSE
                  LOCAL temp1[32]
                  LOCAL temp2[16]
                ENDIF

                _FEMMS

                mov eax,dwPhase
                CLEAR_UPPER_32 PTR_EAX
                mov PTR_ESI,lpSrc
                lea PTR_EDI,temp1
                mov PTR_EBX,lpBArray
                lea PTR_ESI,[PTR_ESI][PTR_EAX*4]

                REORD_PAIR 18, 0,0,16,31,1,    1,1,17,30,3
                REORD_PAIR 18, 3,2,19,29,5,    2,3,18,28,7
                REORD_PAIR 18, 6,4,22,27,9,    7,5,23,26,11
                REORD_PAIR 18, 5,6,21,25,13,   4,7,20,24,15
                REORD_PAIR 18, 12,8,28,23,17,  13,9,29,22,19
                REORD_PAIR 18, 15,10,31,21,21, 14,11,30,20,23
                REORD_PAIR 18, 10,12,26,19,25, 11,13,27,18,27
                REORD_PAIR 18, 9,14,25,17,29,  8,15,24,16,31

                lea PTR_EDI,temp2
                lea PTR_ESI,temp1

                REORD_PAIR 1, 0,0,8,8,2,     1,1,9,9,6
                REORD_PAIR 1, 2,2,10,10,14,  3,3,11,11,10
                REORD_PAIR 1, 4,4,12,12,30,  5,5,13,13,26
                REORD_PAIR 1, 6,6,14,14,18,  7,7,15,15,22

                lea PTR_EDI,temp1
                lea PTR_ESI,temp2

                REORD_PAIR 1, 0,0,4,4,4,     1,1,5,5,12
                REORD_PAIR 1, 2,2,6,6,28,    3,3,7,7,20
                REORD_PAIR 1, 8,8,12,12,4,   9,9,13,13,12
                REORD_PAIR 1, 10,10,14,14,   28,11,11,15,15,20

                lea PTR_EDI,temp2
                lea PTR_ESI,temp1

                REORD_PAIR 1, 0,0,2,2,8,     1,1,3,3,24
                REORD_PAIR 1, 4,4,6,6,8,     5,5,7,7,24
                REORD_PAIR 1, 8,8,10,10,8,   9,9,11,11,24
                REORD_PAIR 1, 12,12,14,14,8, 13,13,15,15,24

                IFDEF AMD

                  movd mm7,DWORD PTR [PTR_EBX+(16*4)]
                  movd mm0,DWORD PTR [PTR_EDI+(0*4)]
                  movd mm1,DWORD PTR [PTR_EDI+(1*4)]
                  pxor mm2,mm2
                  movd mm3,DWORD PTR [PTR_EDI+(0*4)]
                  pfsub mm2,mm0
                  movd mm4,DWORD PTR [PTR_EDI+(1*4)]
                  pfsub mm2,mm1
                  pfsub mm3,mm4
                  pfadd mm2,mm2
                  pfmul mm3,mm7
                  movd DWORD PTR [PTR_ESI+(0*4)],mm2
                  movd DWORD PTR [PTR_ESI+(1*4)],mm3

                  movd mm0,DWORD PTR [PTR_EDI+(2*4)]
                  movd mm1,DWORD PTR [PTR_EDI+(3*4)]
                  movq mm2,mm0
                  pfadd mm0,mm1
                  movd mm3,DWORD PTR [PTR_EDI+(4*4)]
                  movd DWORD PTR [PTR_ESI+(2*4)],mm0
                  pfsub mm2,mm1
                  movd mm4,DWORD PTR [PTR_EDI+(5*4)]
                  pfmul mm2,mm7
                  movq mm5,mm3
                  pfsub mm2,mm0
                  pfadd mm3,mm4
                  movd DWORD PTR [PTR_ESI+(3*4)],mm2
                  movd DWORD PTR [PTR_ESI+(4*4)],mm3

                  movd mm0,DWORD PTR [PTR_EDI+(6*4)]
                  pfsub mm5,mm4
                  movd mm1,DWORD PTR [PTR_EDI+(7*4)]
                  pfmul mm5,mm7
                  pfadd mm0,mm1
                  pfadd mm5,mm3
                  movd DWORD PTR [PTR_ESI+(5*4)],mm5
                  movd DWORD PTR [PTR_ESI+(6*4)],mm0

                  movd mm0,DWORD PTR [PTR_EDI+(8*4)]
                  movd mm1,DWORD PTR [PTR_EDI+(9*4)]
                  movq mm2,mm0
                  movd mm5,DWORD PTR [PTR_EDI+(6*4)]
                  pfadd mm0,mm1
                  movd mm6,DWORD PTR [PTR_EDI+(7*4)]
                  movd DWORD PTR [PTR_ESI+(8*4)],mm0
                  pfsub mm2,mm1
                  movd mm4,DWORD PTR [PTR_ESI+(5*4)]
                  pfmul mm2,mm7
                  pfsub mm5,mm6
                  movd DWORD PTR [PTR_ESI+(9*4)],mm2
                  pfmul mm5,mm7
                  pfadd mm2,mm0
                  pfsub mm5,mm4
                  movd mm3,DWORD PTR [PTR_EDI+(10*4)]
                  movd mm4,DWORD PTR [PTR_EDI+(11*4)]
                  pfsub mm3,mm4
                  movd DWORD PTR [PTR_ESI+(7*4)],mm5
                  pfmul mm3,mm7
                  pfadd mm3,mm2
                  movd DWORD PTR [PTR_ESI+(11*4)],mm3

                  movd mm0,DWORD PTR [PTR_EDI+(10*4)]
                  movd mm1,DWORD PTR [PTR_EDI+(11*4)]
                  movd mm2,DWORD PTR [PTR_EDI+(12*4)]
                  movd mm3,DWORD PTR [PTR_EDI+(13*4)]
                  movq mm4,mm2
                  pfadd mm2,mm3
                  pfadd mm0,mm1
                  movd DWORD PTR [PTR_ESI+(12*4)],mm2
                  pfsub mm4,mm3
                  movd DWORD PTR [PTR_ESI+(10*4)],mm0
                  pfmul mm4,mm7
                  pfadd mm4,mm2
                  movd mm5,DWORD PTR [PTR_ESI+(8*4)]
                  movd mm6,DWORD PTR [PTR_ESI+(9*4)]
                  pfsub mm4,mm5
                  pfsub mm4,mm6
                  movd DWORD PTR [PTR_ESI+(13*4)],mm4

                  movd mm0,DWORD PTR [PTR_EDI+(14*4)]
                  movd mm1,DWORD PTR [PTR_EDI+(15*4)]
                  movd mm2,DWORD PTR [PTR_ESI+(8*4)]
                  movd mm3,DWORD PTR [PTR_ESI+(10*4)]
                  movd mm5,DWORD PTR [PTR_ESI+(11*4)]
                  movq mm4,mm0
                  pfadd mm0,mm1
                  pfsub mm4,mm1
                  pfsub mm0,mm2
                  pfmul mm4,mm7
                  pfsub mm0,mm3
                  pfsub mm4,mm5
                  movd DWORD PTR [PTR_ESI+(14*4)],mm0
                  movd DWORD PTR [PTR_ESI+(15*4)],mm4

                  mov PTR_EDI,lpOut1

                  movd mm0,DWORD PTR [PTR_ESI+(1*4)]
                  movd mm1,DWORD PTR [PTR_ESI+(9*4)]
                  movd mm2,DWORD PTR [PTR_ESI+(14*4)]
                  movd DWORD PTR [PTR_EDI+(0*16*4)],mm0
                  pfsub mm1,mm2
                  movd DWORD PTR [PTR_EDI+(2*16*4)],mm1

                  movd mm0,DWORD PTR [PTR_ESI+(5*4)]
                  movd mm1,DWORD PTR [PTR_ESI+(13*4)]
                  movd mm2,DWORD PTR [PTR_ESI+(6*4)]
                  movd mm3,DWORD PTR [PTR_ESI+(10*4)]
                  pfsub mm0,mm2
                  pfsub mm1,mm3
                  movd DWORD PTR [PTR_EDI+(4*16*4)],mm0
                  movd DWORD PTR [PTR_EDI+(6*16*4)],mm1

                  movd mm0,DWORD PTR [PTR_ESI+(3*4)]
                  movd mm1,DWORD PTR [PTR_ESI+(11*4)]
                  movd mm2,DWORD PTR [PTR_ESI+(7*4)]
                  movd mm3,DWORD PTR [PTR_ESI+(8*4)]
                  movd mm4,DWORD PTR [PTR_ESI+(9*4)]
                  pfsub mm1,mm3
                  movd mm5,DWORD PTR [PTR_ESI+(13*4)]
                  pfsub mm1,mm4
                  movd DWORD PTR [PTR_EDI+(8*16*4)],mm0
                  pfsub mm1,mm5
                  movd DWORD PTR [PTR_EDI+(12*16*4)],mm2
                  movd DWORD PTR [PTR_EDI+(10*16*4)],mm1

                  movd mm0,DWORD PTR [PTR_ESI+(15*4)]
                  movd mm1,DWORD PTR [PTR_ESI+(0*4)]
                  movd DWORD PTR [PTR_EDI+(14*16*4)],mm0

                  mov PTR_EDI,lpOut2

                  pxor mm6,mm6
                  movd DWORD PTR [PTR_EDI+(16*16*4)],mm1

                  movd mm0,DWORD PTR [PTR_ESI+(8*4)]
                  movd mm3,DWORD PTR [PTR_ESI+(12*4)]
                  movq mm2,mm0
                  movq mm4,mm0
                  movd mm1,DWORD PTR [PTR_ESI+(4*4)]
                  pfsubr mm0,mm6
                  pfsubr mm1,mm6
                  movd DWORD PTR [PTR_EDI+(14*16*4)],mm0

                  pfsub mm2,mm3
                  movd mm0,DWORD PTR [PTR_ESI+(2*4)]
                  movd DWORD PTR [PTR_EDI+(10*16*4)],mm2
                  pfsubr mm0,mm6

                  movd mm5,DWORD PTR [PTR_ESI+(10*4)]
                  pfsub mm3,mm4
                  movd DWORD PTR [PTR_EDI+(8*16*4)],mm0
                  pfsub mm3,mm5
                  movd DWORD PTR [PTR_EDI+(12*16*4)],mm1
                  movd DWORD PTR [PTR_EDI+(6*16*4)],mm3

                  movd mm0,DWORD PTR [PTR_ESI+(4*4)]
                  movd mm1,DWORD PTR [PTR_ESI+(6*4)]
                  movd mm2,DWORD PTR [PTR_ESI+(14*4)]
                  movd mm3,DWORD PTR [PTR_ESI+(1*4)]
                  pfsub mm0,mm1
                  pfsubr mm2,mm6
                  pfsubr mm3,mm6
                  movd DWORD PTR [PTR_EDI+(4*16*4)],mm0
                  movd DWORD PTR [PTR_EDI+(2*16*4)],mm2
                  movd DWORD PTR [PTR_EDI+(0*16*4)],mm3

                ELSE

                  fld DWORD PTR [PTR_EDI+(0*4)]
                  fchs
                  fsub DWORD PTR [PTR_EDI+(1*4)]
                  fadd st,st(0)
                  fstp DWORD PTR [PTR_ESI+(0*4)]

                  fld DWORD PTR [PTR_EDI+(0*4)]
                  fsub DWORD PTR [PTR_EDI+(1*4)]
                  fmul DWORD PTR [PTR_EBX+(16*4)]
                  fstp DWORD PTR [PTR_ESI+(1*4)]

                  fld DWORD PTR [PTR_EDI+(2*4)]
                  fadd DWORD PTR [PTR_EDI+(3*4)]
                  fld DWORD PTR [PTR_EDI+(2*4)]
                  fsub DWORD PTR [PTR_EDI+(3*4)]
                  fmul DWORD PTR [PTR_EBX+(16*4)]
                  fsub st,st(1)
                  fstp DWORD PTR [PTR_ESI+(3*4)]
                  fstp DWORD PTR [PTR_ESI+(2*4)]

                  fld DWORD PTR [PTR_EDI+(4*4)]
                  fadd DWORD PTR [PTR_EDI+(5*4)]
                  fld DWORD PTR [PTR_EDI+(4*4)]
                  fsub DWORD PTR [PTR_EDI+(5*4)]
                  fmul DWORD PTR [PTR_EBX+(16*4)]
                  fadd st,st(1)
                  fstp DWORD PTR [PTR_ESI+(5*4)]
                  fstp DWORD PTR [PTR_ESI+(4*4)]

                  fld DWORD PTR [PTR_EDI+(6*4)]
                  fadd DWORD PTR [PTR_EDI+(7*4)]
                  fstp DWORD PTR [PTR_ESI+(6*4)]

                  fld DWORD PTR [PTR_EDI+(6*4)]
                  fsub DWORD PTR [PTR_EDI+(7*4)]
                  fmul DWORD PTR [PTR_EBX+(16*4)]
                  fsub DWORD PTR [PTR_ESI+(5*4)]
                  fstp DWORD PTR [PTR_ESI+(7*4)]

                  fld DWORD PTR [PTR_EDI+(8*4)]
                  fadd DWORD PTR [PTR_EDI+(9*4)]
                  fst DWORD PTR [PTR_ESI+(8*4)]
                  fld DWORD PTR [PTR_EDI+(8*4)]
                  fsub DWORD PTR [PTR_EDI+(9*4)]
                  fmul DWORD PTR [PTR_EBX+(16*4)]
                  fst DWORD PTR [PTR_ESI+(9*4)]
                  faddp st(1),st
                  fld DWORD PTR [PTR_EDI+(10*4)]
                  fsub DWORD PTR [PTR_EDI+(11*4)]
                  fmul DWORD PTR [PTR_EBX+(16*4)]
                  faddp st(1),st
                  fstp DWORD PTR [PTR_ESI+(11*4)]

                  fld DWORD PTR [PTR_EDI+(10*4)]
                  fadd DWORD PTR [PTR_EDI+(11*4)]
                  fstp DWORD PTR [PTR_ESI+(10*4)]

                  fld DWORD PTR [PTR_EDI+(12*4)]
                  fadd DWORD PTR [PTR_EDI+(13*4)]
                  fstp DWORD PTR [PTR_ESI+(12*4)]
                  fld DWORD PTR [PTR_EDI+(12*4)]
                  fsub DWORD PTR [PTR_EDI+(13*4)]
                  fmul DWORD PTR [PTR_EBX+(16*4)]
                  fadd DWORD PTR [PTR_ESI+(12*4)]
                  fsub DWORD PTR [PTR_ESI+(8*4)]
                  fsub DWORD PTR [PTR_ESI+(9*4)]
                  fstp DWORD PTR [PTR_ESI+(13*4)]

                  fld DWORD PTR [PTR_EDI+(14*4)]
                  fadd DWORD PTR [PTR_EDI+(15*4)]
                  fsub DWORD PTR [PTR_ESI+(8*4)]
                  fsub DWORD PTR [PTR_ESI+(10*4)]
                  fstp DWORD PTR [PTR_ESI+(14*4)]

                  fld DWORD PTR [PTR_EDI+(14*4)]
                  fsub DWORD PTR [PTR_EDI+(15*4)]
                  fmul DWORD PTR [PTR_EBX+(16*4)]
                  fsub DWORD PTR [PTR_ESI+(11*4)]
                  fstp DWORD PTR [PTR_ESI+(15*4)]

                  mov PTR_EDI,lpOut1

                  fld DWORD PTR [PTR_ESI+(1*4)]
                  fstp DWORD PTR [PTR_EDI+(0*16*4)]

                  fld DWORD PTR [PTR_ESI+(9*4)]
                  fsub DWORD PTR [PTR_ESI+(14*4)]
                  fstp DWORD PTR [PTR_EDI+(2*16*4)]

                  fld DWORD PTR [PTR_ESI+(5*4)]
                  fsub DWORD PTR [PTR_ESI+(6*4)]
                  fstp DWORD PTR [PTR_EDI+(4*16*4)]

                  fld DWORD PTR [PTR_ESI+(13*4)]
                  fsub DWORD PTR [PTR_ESI+(10*4)]
                  fstp DWORD PTR [PTR_EDI+(6*16*4)]

                  fld DWORD PTR [PTR_ESI+(3*4)]
                  fstp DWORD PTR [PTR_EDI+(8*16*4)]

                  fld DWORD PTR [PTR_ESI+(11*4)]
                  fsub DWORD PTR [PTR_ESI+(8*4)]
                  fsub DWORD PTR [PTR_ESI+(9*4)]
                  fsub DWORD PTR [PTR_ESI+(13*4)]
                  fstp DWORD PTR [PTR_EDI+(10*16*4)]

                  fld DWORD PTR [PTR_ESI+(7*4)]
                  fstp DWORD PTR [PTR_EDI+(12*16*4)]

                  fld DWORD PTR [PTR_ESI+(15*4)]
                  fstp DWORD PTR [PTR_EDI+(14*16*4)]

                  mov PTR_EDI,lpOut2

                  fld DWORD PTR [PTR_ESI+(0*4)]
                  fstp DWORD PTR [PTR_EDI+(16*16*4)]

                  fld DWORD PTR [PTR_ESI+(8*4)]
                  fchs
                  fstp DWORD PTR [PTR_EDI+(14*16*4)]

                  fld DWORD PTR [PTR_ESI+(4*4)]
                  fchs
                  fstp DWORD PTR [PTR_EDI+(12*16*4)]

                  fld DWORD PTR [PTR_ESI+(8*4)]
                  fsub DWORD PTR [PTR_ESI+(12*4)]
                  fstp DWORD PTR [PTR_EDI+(10*16*4)]

                  fld DWORD PTR [PTR_ESI+(2*4)]
                  fchs
                  fstp DWORD PTR [PTR_EDI+(8*16*4)]

                  fld DWORD PTR [PTR_ESI+(12*4)]
                  fsub DWORD PTR [PTR_ESI+(8*4)]
                  fsub DWORD PTR [PTR_ESI+(10*4)]
                  fstp DWORD PTR [PTR_EDI+(6*16*4)]

                  fld DWORD PTR [PTR_ESI+(4*4)]
                  fsub DWORD PTR [PTR_ESI+(6*4)]
                  fstp DWORD PTR [PTR_EDI+(4*16*4)]

                  fld DWORD PTR [PTR_ESI+(14*4)]
                  fchs
                  fstp DWORD PTR [PTR_EDI+(2*16*4)]

                  fld DWORD PTR [PTR_ESI+(1*4)]
                  fchs
                  fstp DWORD PTR [PTR_EDI+(0*16*4)]

                ENDIF

                lea PTR_EDI,temp2
                lea PTR_ESI,temp1

                REORD_PAIR 1, 0,16,8,24,2,   1,17,9,25,6
                REORD_PAIR 1, 2,18,10,26,14, 3,19,11,27,10
                REORD_PAIR 1, 4,20,12,28,30, 5,21,13,29,26
                REORD_PAIR 1, 6,22,14,30,18, 7,23,15,31,22

                lea PTR_EDI,temp1
                lea PTR_ESI,temp2

                REORD_PAIR 1, 16,0,20,4,4,    17,1,21,5,12
                REORD_PAIR 1, 18,2,22,6,28,   19,3,23,7,20
                REORD_PAIR 1, 24,8,28,12,4,   25,9,29,13,12
                REORD_PAIR 1, 26,10,30,14,28, 27,11,31,15,20

                lea PTR_EDI,temp2
                lea PTR_ESI,temp1

                REORD_PAIR 1, 0,16,2,18,8, 1,17,3,19,24
                REORD_PAIR 1, 4,20,6,22,8, 5,21,7,23,24
                REORD_PAIR 1, 8,24,10,26,8, 9,25,11,27,24
                REORD_PAIR 1, 12,28,14,30,8, 13,29,15,31,24

                _FEMMS

                fld DWORD PTR [PTR_EDI+(0*4)]
                fadd DWORD PTR [PTR_EDI+(1*4)]
                fstp DWORD PTR [PTR_ESI+(16*4)]

                fld DWORD PTR [PTR_EDI+(0*4)]
                fsub DWORD PTR [PTR_EDI+(1*4)]
                fmul DWORD PTR [PTR_EBX+(16*4)]
                fstp DWORD PTR [PTR_ESI+(17*4)]

                fld DWORD PTR [PTR_EDI+(2*4)]
                fadd DWORD PTR [PTR_EDI+(3*4)]
                fstp DWORD PTR [PTR_ESI+(18*4)]

                fld DWORD PTR [PTR_EDI+(2*4)]
                fsub DWORD PTR [PTR_EDI+(3*4)]
                fmul DWORD PTR [PTR_EBX+(16*4)]
                fstp DWORD PTR [PTR_ESI+(19*4)]

                fld DWORD PTR [PTR_EDI+(4*4)]
                fadd DWORD PTR [PTR_EDI+(5*4)]
                fadd DWORD PTR [PTR_ESI+(16*4)]
                fstp DWORD PTR [PTR_ESI+(20*4)]

                fld DWORD PTR [PTR_EDI+(4*4)]
                fsub DWORD PTR [PTR_EDI+(5*4)]
                fmul DWORD PTR [PTR_EBX+(16*4)]
                fadd DWORD PTR [PTR_ESI+(17*4)]
                fstp DWORD PTR [PTR_ESI+(21*4)]
                
                fld DWORD PTR [PTR_EDI+(6*4)]
                fadd DWORD PTR [PTR_EDI+(7*4)]
                fadd DWORD PTR [PTR_ESI+(16*4)]
                fadd DWORD PTR [PTR_ESI+(18*4)]
                fstp DWORD PTR [PTR_ESI+(22*4)]

                fld DWORD PTR [PTR_EDI+(6*4)]
                fsub DWORD PTR [PTR_EDI+(7*4)]
                fmul DWORD PTR [PTR_EBX+(16*4)]
                fadd DWORD PTR [PTR_ESI+(16*4)]
                fadd DWORD PTR [PTR_ESI+(17*4)]
                fadd DWORD PTR [PTR_ESI+(19*4)]
                fstp DWORD PTR [PTR_ESI+(23*4)]

                fld DWORD PTR [PTR_EDI+(8*4)]
                fadd DWORD PTR [PTR_EDI+(9*4)]
                fstp DWORD PTR [PTR_ESI+(24*4)]

                fld DWORD PTR [PTR_EDI+(8*4)]
                fsub DWORD PTR [PTR_EDI+(9*4)]
                fmul DWORD PTR [PTR_EBX+(16*4)]
                fstp DWORD PTR [PTR_ESI+(25*4)]

                fld DWORD PTR [PTR_EDI+(10*4)]
                fadd DWORD PTR [PTR_EDI+(11*4)]
                fadd DWORD PTR [PTR_ESI+(24*4)]
                fstp DWORD PTR [PTR_ESI+(26*4)]

                fld DWORD PTR [PTR_EDI+(10*4)]
                fsub DWORD PTR [PTR_EDI+(11*4)]
                fmul DWORD PTR [PTR_EBX+(16*4)]
                fadd DWORD PTR [PTR_ESI+(24*4)]
                fadd DWORD PTR [PTR_ESI+(25*4)]
                fstp DWORD PTR [PTR_ESI+(27*4)]

                fld DWORD PTR [PTR_EDI+(12*4)]
                fadd DWORD PTR [PTR_EDI+(13*4)]
                fsub DWORD PTR [PTR_ESI+(20*4)]
                fstp DWORD PTR [PTR_ESI+(28*4)]

                fld DWORD PTR [PTR_EDI+(12*4)]
                fsub DWORD PTR [PTR_EDI+(13*4)]
                fmul DWORD PTR [PTR_EBX+(16*4)]
                fadd DWORD PTR [PTR_ESI+(28*4)]
                fsub DWORD PTR [PTR_ESI+(21*4)]
                fstp DWORD PTR [PTR_ESI+(29*4)]

                fld DWORD PTR [PTR_EDI+(14*4)]
                fadd DWORD PTR [PTR_EDI+(15*4)]
                fsub DWORD PTR [PTR_ESI+(22*4)]
                fstp DWORD PTR [PTR_ESI+(30*4)]

                fld DWORD PTR [PTR_EDI+(14*4)]
                fsub DWORD PTR [PTR_EDI+(15*4)]
                fmul DWORD PTR [PTR_EBX+(16*4)]
                fsub DWORD PTR [PTR_ESI+(23*4)]
                fstp DWORD PTR [PTR_ESI+(31*4)]

                mov PTR_EDI,lpOut2

                fld DWORD PTR [PTR_ESI+(30*4)]
                fchs
                fstp DWORD PTR [PTR_EDI+(1*16*4)]

                fld DWORD PTR [PTR_ESI+(26*4)]
                fsub DWORD PTR [PTR_ESI+(22*4)]
                fstp DWORD PTR [PTR_EDI+(3*16*4)]

                fld DWORD PTR [PTR_ESI+(18*4)]
                fadd DWORD PTR [PTR_ESI+(20*4)]
                fsub DWORD PTR [PTR_ESI+(26*4)]
                fstp DWORD PTR [PTR_EDI+(5*16*4)]

                fld DWORD PTR [PTR_ESI+(28*4)]
                fsub DWORD PTR [PTR_ESI+(18*4)]
                fstp DWORD PTR [PTR_EDI+(7*16*4)]

                fld DWORD PTR [PTR_ESI+(28*4)]
                fchs
                fstp DWORD PTR [PTR_EDI+(9*16*4)]

                fld DWORD PTR [PTR_ESI+(24*4)]
                fsub DWORD PTR [PTR_ESI+(20*4)]
                fstp DWORD PTR [PTR_EDI+(11*16*4)]

                fld DWORD PTR [PTR_ESI+(16*4)]
                fsub DWORD PTR [PTR_ESI+(24*4)]
                fstp DWORD PTR [PTR_EDI+(13*16*4)]

                fld DWORD PTR [PTR_ESI+(16*4)]
                fchs
                fstp DWORD PTR [PTR_EDI+(15*16*4)]

                mov PTR_EDI,lpOut1

                fld DWORD PTR [PTR_ESI+(31*4)]
                fstp DWORD PTR [PTR_EDI+(15*16*4)]

                fld DWORD PTR [PTR_ESI+(23*4)]
                fsub DWORD PTR [PTR_ESI+(27*4)]
                fstp DWORD PTR [PTR_EDI+(13*16*4)]

                fld DWORD PTR [PTR_ESI+(27*4)]
                fsub DWORD PTR [PTR_ESI+(19*4)]
                fsub DWORD PTR [PTR_ESI+(20*4)]
                fsub DWORD PTR [PTR_ESI+(21*4)]
                fstp DWORD PTR [PTR_EDI+(11*16*4)]

                fld DWORD PTR [PTR_ESI+(19*4)]
                fsub DWORD PTR [PTR_ESI+(29*4)]
                fstp DWORD PTR [PTR_EDI+(9*16*4)]

                fld DWORD PTR [PTR_ESI+(29*4)]
                fsub DWORD PTR [PTR_ESI+(18*4)]
                fstp DWORD PTR [PTR_EDI+(7*16*4)]
                
                fld DWORD PTR [PTR_ESI+(18*4)]
                fadd DWORD PTR [PTR_ESI+(20*4)]
                fadd DWORD PTR [PTR_ESI+(21*4)]
                fsub DWORD PTR [PTR_ESI+(25*4)]
                fsub DWORD PTR [PTR_ESI+(26*4)]
                fstp DWORD PTR [PTR_EDI+(5*16*4)]

                fld DWORD PTR [PTR_ESI+(26*4)]
                fadd DWORD PTR [PTR_ESI+(25*4)]
                fsub DWORD PTR [PTR_ESI+(17*4)]
                fsub DWORD PTR [PTR_ESI+(22*4)]
                fstp DWORD PTR [PTR_EDI+(3*16*4)]

                fld DWORD PTR [PTR_ESI+(17*4)]
                fsub DWORD PTR [PTR_ESI+(30*4)]
                fstp DWORD PTR [PTR_EDI+(1*16*4)]

                IFDEF WIN64
                EXIT64
                ENDIF
                ret

                IFDEF AMD
AMD_poly_filter   ENDP
                ELSE
x86_poly_filter   ENDP
                ENDIF


ENDIF   ; IFNDEF SSE


;#############################################################################
;##                                                                         ##
;## Apply inverse window and write sample data                              ##
;##                                                                         ##
;#############################################################################

;=============================================================================

; Macros

; start a sum, SSE only
; does four samples in parallel

MAC_INIT_PAIR_SSE   MACRO f1,f2

        movaps  xmm0,[f1]
        movaps  xmm1,[f1][16*4]
        movaps  xmm2,[f1][32*4]
        movaps  xmm3,[f1][48*4]

        movups  xmm4,[f2]
        movups  xmm5,[f2][32*4]
        movups  xmm6,[f2][64*4]
        movups  xmm7,[f2][96*4]

        mulps   xmm0,xmm4
        mulps   xmm1,xmm5
        mulps   xmm2,xmm6
        mulps   xmm3,xmm7

        ENDM

; start a swapped sum (add 0, 1, 2, 3 to 3, 2, 1, 0), SSE only
; does four samples in parallel

MAC_INIT_PAIR_SWAPPED_4_SSE MACRO f1,f2

        movups  xmm4,f2
        movups  xmm5,f2[-32*4]
        movups  xmm6,f2[-64*4]
        movups  xmm7,f2[-96*4]

        movaps  xmm0,f1
        movaps  xmm1,f1[-16*4]
        movaps  xmm2,f1[-32*4]
        movaps  xmm3,f1[-48*4]

        shufps  xmm4,xmm4,01Bh
        shufps  xmm5,xmm5,01Bh
        shufps  xmm6,xmm6,01Bh
        shufps  xmm7,xmm7,01Bh

        mulps   xmm0,xmm4
        mulps   xmm1,xmm5
        mulps   xmm2,xmm6
        mulps   xmm3,xmm7

        ENDM

; start a swapped sum (add 0, 1, 2, 3 to 3, 2, 1, 0), SSE only
; does two samples in parallel

MAC_INIT_PAIR_SWAPPED_2_SSE MACRO f1,f2

        movups  xmm4,f2
        movups  xmm5,f2[-32*4]
        movaps  xmm0,f1
        movaps  xmm1,f1[-16*4]
        shufps  xmm4,xmm4,01Bh
        shufps  xmm5,xmm5,01Bh
        mulps   xmm0,xmm4
        mulps   xmm1,xmm5

        ENDM

; start a swapped sum (add 0, 1, 2, 3 to 3, 2, 1, 0), SSE only
; does one sample

MAC_INIT_PAIR_SWAPPED_1_SSE MACRO f1,f2

        movups  xmm4,f2
        movaps  xmm0,f1
        shufps  xmm4,xmm4,01Bh
        mulps   xmm0,xmm4

        ENDM

; start a sum

MAC_INIT_PAIR   MACRO f1,f2,f3,f4

    IFDEF AMD   ;-------------
    
        movd    mm1,DWORD PTR f1
        movd    mm2,DWORD PTR f2
        movd    mm3,DWORD PTR f3
        movd    mm4,DWORD PTR f4
        pfmul   mm1,mm2
        pfmul   mm3,mm4

    ELSE
     IFDEF SSE  ;-------------

        movss   xmm0,DWORD PTR f1
        mulss   xmm0,DWORD PTR f2

        movss   xmm1,DWORD PTR f3
        mulss   xmm1,DWORD PTR f4

     ELSE       ;-------------

        fld     DWORD PTR f3
        fmul    DWORD PTR f4

        fld     DWORD PTR f1
        fmul    DWORD PTR f2

     ENDIF      ;-------------
    ENDIF

        ENDM

; continue an ongoing sum, SSE only
; does four samples in parallel

MAC_PAIR_SSE    MACRO f1,f2

        movaps  xmm4,f1
        movaps  xmm5,f1[16*4]
        movups  xmm6,f2
        movups  xmm7,f2[32*4]
        mulps   xmm4,xmm6
        mulps   xmm5,xmm7
        addps   xmm0,xmm4
        addps   xmm1,xmm5
    
        movaps  xmm4,f1[32*4]
        movaps  xmm5,f1[48*4]
        movups  xmm6,f2[64*4]
        movups  xmm7,f2[96*4]
        mulps   xmm4,xmm6
        mulps   xmm5,xmm7
        addps   xmm2,xmm4
        addps   xmm3,xmm5

        ENDM

; continues an ongoing swapped sum (add 0, 1, 2, 3 to 3, 2, 1, 0), SSE only
; does four samples in parallel

MAC_PAIR_SWAPPED_4_SSE  MACRO f1,f2

        movups  xmm5,f2
        movups  xmm7,f2[-32*4]
        movaps  xmm4,f1
        movaps  xmm6,f1[-16*4]
        shufps  xmm5,xmm5,01Bh
        shufps  xmm7,xmm7,01Bh
        mulps   xmm4,xmm5
        mulps   xmm6,xmm7
        addps   xmm0,xmm4
        addps   xmm1,xmm6

        movups  xmm5,f2[-64*4]
        movups  xmm7,f2[-96*4]
        movaps  xmm4,f1[-32*4]
        movaps  xmm6,f1[-48*4]
        shufps  xmm5,xmm5,01Bh
        shufps  xmm7,xmm7,01Bh
        mulps   xmm4,xmm5
        mulps   xmm6,xmm7
        addps   xmm2,xmm4
        addps   xmm3,xmm6

        ENDM

; continues an ongoing swapped sum (add 0, 1, 2, 3 to 3, 2, 1, 0), SSE only
; does two samples in parallel

MAC_PAIR_SWAPPED_2_SSE  MACRO f1,f2

        movups  xmm5,f2
        movups  xmm7,f2[-32*4]
        movaps  xmm4,f1
        movaps  xmm6,f1[-16*4]
        shufps  xmm5,xmm5,01Bh
        shufps  xmm7,xmm7,01Bh
        mulps   xmm4,xmm5
        mulps   xmm6,xmm7
        addps   xmm0,xmm4
        addps   xmm1,xmm6

        ENDM

; continues an ongoing swapped sum (add 0, 1, 2, 3 to 3, 2, 1, 0), SSE only
; does one sample

MAC_PAIR_SWAPPED_1_SSE  MACRO f1,f2

        movups  xmm5,f2
        movaps  xmm4,f1
        shufps  xmm5,xmm5,01Bh
        mulps   xmm4,xmm5
        addps   xmm0,xmm4

        ENDM

; continues an ongoing sum

MAC_PAIR        MACRO f1,f2,f3,f4

    IFDEF AMD   ;-------------

        movd    mm0,DWORD PTR f1
        movd    mm5,DWORD PTR f2
        movd    mm6,DWORD PTR f3
        movd    mm7,DWORD PTR f4
        pfmul   mm0,mm5
        pfmul   mm6,mm7
        pfadd   mm1,mm0
        pfadd   mm3,mm6

    ELSE
     IFDEF SSE  ;-------------

        movss   xmm4,DWORD PTR f1
        movss   xmm5,DWORD PTR f3

        mulss   xmm4,DWORD PTR f2
        mulss   xmm5,DWORD PTR f4

        addss   xmm0,xmm4
        addss   xmm1,xmm5

     ELSE       ;-------------

        fld     DWORD PTR f1
        fld     DWORD PTR f3

        fmul    DWORD PTR f4
        fxch    st(1)
        fmul    DWORD PTR f2

        faddp   st(2),st
        faddp   st(2),st

     ENDIF      ;-------------
    ENDIF   

        ENDM

; finishes an ongoing sum, SSE only
; does four samples in parallel
; stores the 16-bit clamped signed integer results to *EDI, *(EDI+dwSampleStep), *(EDI+dwSampleStep*2), *(EDI+dwSampleStep*3)
; advances EDI by dwSampleStep*4

MAC_END_SUM_SSE MACRO      ; TODO: EAX

        movaps  xmm4,xmm1
        movaps  xmm7,xmm3
        movhlps xmm1,xmm0       ; 13 12 03 02
        movhlps xmm3,xmm2
        movlhps xmm0,xmm4       ; 11 10 01 00
        movlhps xmm2,xmm7

        addps   xmm0,xmm1       ; 13+11 12+10 03+01 02+00
        addps   xmm2,xmm3
        shufps  xmm0,xmm0,0D8h  ; 13+11 03+01 12+10 02+00
        shufps  xmm2,xmm2,0D8h
        movhlps xmm1,xmm0       ; XXXXX XXXXX 13+11 03+01
        movhlps xmm3,xmm2
        addps   xmm0,xmm1       ; XXXXX XXXXX sum1  sum0
        addps   xmm2,xmm3

        cvtps2pi mm0,xmm0
        cvtps2pi mm2,xmm2
        packssdw mm0,mm0
        packssdw mm2,mm2

        movd    ecx,mm0
        mov     WORD PTR [PTR_EDI],cx
        shr     ecx,16
        mov     WORD PTR [PTR_EDI+PTR_EAX],cx
        lea     PTR_EDI,[PTR_EDI + PTR_EAX*2]

        movd    ecx,mm2
        mov     WORD PTR [PTR_EDI],cx
        shr     ecx,16
        mov     WORD PTR [PTR_EDI+PTR_EAX],cx
        lea     PTR_EDI,[PTR_EDI + PTR_EAX*2]

        ENDM

; finishes an ongoing sum
; stores the 16-bit clamped signed integer result to *EDI,
; advances EDI by dwSampleStep

MAC_END_SUM     MACRO
        LOCAL __t1,__t2,__c1,__c2,__done

    IFDEF AMD   ;-------------

        pfadd   mm1,mm3
        pf2id   mm1,mm1
        packssdw mm1,mm1
        movd    ecx,mm1
        mov     [PTR_EDI],cx
        add     PTR_EDI,PTR_EAX

    ELSE
     IFDEF SSE  ;-------------

        addss   xmm0,xmm1
        cvtss2si ecx,xmm0

     ELSE       ;-------------

        faddp   st(1),st
        fistp   DWORD PTR temp
        mov     ecx,temp

     ENDIF      ;-------------

        cmp     ecx,32767
        jg      __c1
        cmp     ecx,-32768
        jl      __c2
__t1:
        mov     [PTR_EDI],cx
        add     PTR_EDI,PTR_EAX
        jmp     __done

__c1:
        mov     ecx,32767
        jmp     __t1
__c2:
        mov     ecx,-32768
        jmp     __t1

__done:

    ENDIF   ;-----------------

        ENDM

; finishes an ongoing swapped sum by taking a difference of the two partial sums (sum0 - sum1), SSE only
; does four samples in parallel
; stores the 16-bit clamped signed integer results to *EDI, *(EDI+dwSampleStep), *(EDI+dwSampleStep*2), *(EDI+dwSampleStep*3)
; advances EDI by dwSampleStep*4

MAC_END_DIF_ODD_4_SSE   MACRO

        movaps  xmm4,xmm1
        movaps  xmm5,xmm3
        movhlps xmm1,xmm0       ; 13 12 03 02
        movhlps xmm3,xmm2       ; 13 12 03 02
        movlhps xmm0,xmm4       ; 11 10 01 00
        movlhps xmm2,xmm5       ; 11 10 01 00
        addps   xmm0,xmm1       ; 13+11 12+10 03+01 02+00
        addps   xmm2,xmm3       ; 13+11 12+10 03+01 02+00
        shufps  xmm0,xmm0,0D8h  ; 13+11 03+01 12+10 02+00
        shufps  xmm2,xmm2,0D8h  ; 13+11 03+01 12+10 02+00
        movhlps xmm1,xmm0       ; XXXXX XXXXX 13+11 03+01
        movhlps xmm3,xmm2       ; XXXXX XXXXX 13+11 03+01
        subps   xmm1,xmm0       ; XXXXX XXXXX dif1  dif0
        subps   xmm3,xmm2       ; XXXXX XXXXX dif1  dif0
        
        cvtps2pi mm0,xmm1       ; integer result in mm4
        cvtps2pi mm1,xmm3       ; integer result in mm4
        packssdw mm0,mm0
        packssdw mm1,mm1

        movd    ecx,mm0
        mov     [PTR_EDI],cx
        shr     ecx,16
        mov     [PTR_EDI + PTR_EAX],cx
        lea     PTR_EDI,[PTR_EDI + PTR_EAX*2]

        movd    ecx,mm1
        mov     [PTR_EDI],cx
        shr     ecx,16
        mov     [PTR_EDI + PTR_EAX],cx
        lea     PTR_EDI,[PTR_EDI + PTR_EAX*2]

        ENDM

; finishes an ongoing swapped sum by taking a difference of the two partial sums (sum0 - sum1), SSE only
; does two samples in parallel
; stores the 16-bit clamped signed integer results to *EDI, *(EDI+dwSampleStep)
; advances EDI by dwSampleStep*2

MAC_END_DIF_ODD_2_SSE   MACRO

        movaps  xmm4,xmm1
        movhlps xmm1,xmm0       ; 13 12 03 02
        movlhps xmm0,xmm4       ; 11 10 01 00
        addps   xmm0,xmm1       ; 13+11 12+10 03+01 02+00
        shufps  xmm0,xmm0,0D8h  ; 13+11 03+01 12+10 02+00
        movhlps xmm1,xmm0       ; XXXXX XXXXX 13+11 03+01
        subps   xmm1,xmm0       ; XXXXX XXXXX dif1  dif0
        
        cvtps2pi mm0,xmm1       ; integer result in mm4
        packssdw mm0,mm0

        movd    ecx,mm0
        mov     [PTR_EDI],cx
        shr     ecx,16
        mov     [PTR_EDI + PTR_EAX],cx
        lea     PTR_EDI,[PTR_EDI + PTR_EAX*2]

        ENDM

; finishes an ongoing swapped sum by taking a difference of the two partial sums (sum0 - sum1), SSE only
; does one sample
; stores the 16-bit clamped signed integer result to *EDI
; advances EDI by dwSampleStep

MAC_END_DIF_ODD_1_SSE   MACRO

        movhlps xmm4,xmm0
        addps   xmm0,xmm4       ; don't care, don't care, result 1 + result 3, result 0 + result 2
        movaps  xmm4,xmm0       ; result 0 + result 2
        shufps  xmm0,xmm0,001h  ; result 1 + result 3
        subss   xmm0,xmm4       ; (result 1 + result 3) - (result 0 + result 2)
        
        cvtps2pi mm0,xmm0       ; integer result in MM0
        packssdw mm0,mm0

        movd    ecx,mm0

        mov     [PTR_EDI],cx
        add     PTR_EDI,PTR_EAX

        ENDM

; finishes an ongoing swapped sum by taking a difference of the two partial sums (sum1 - sum0), SSE only
; does four samples in parallel
; stores the 16-bit clamped signed integer results to *EDI, *(EDI+dwSampleStep), *(EDI+dwSampleStep*2), *(EDI+dwSampleStep*3)
; advances EDI by dwSampleStep*4

MAC_END_DIF_EVEN_4_SSE  MACRO

        movaps  xmm4,xmm1
        movaps  xmm5,xmm3
        movhlps xmm1,xmm0       ; 13 12 03 02
        movhlps xmm3,xmm2       ; 13 12 03 02
        movlhps xmm0,xmm4       ; 11 10 01 00
        movlhps xmm2,xmm5       ; 11 10 01 00
        addps   xmm0,xmm1       ; 13+11 12+10 03+01 02+00
        addps   xmm2,xmm3       ; 13+11 12+10 03+01 02+00
        shufps  xmm0,xmm0,0D8h  ; 13+11 03+01 12+10 02+00
        shufps  xmm2,xmm2,0D8h  ; 13+11 03+01 12+10 02+00
        movhlps xmm1,xmm0       ; XXXXX XXXXX 13+11 03+01
        movhlps xmm3,xmm2       ; XXXXX XXXXX 13+11 03+01
        subps   xmm0,xmm1       ; XXXXX XXXXX dif1  dif0
        subps   xmm2,xmm3       ; XXXXX XXXXX dif1  dif0

        cvtps2pi mm0,xmm0       ; integer result in mm4
        cvtps2pi mm1,xmm2       ; integer result in mm4
        packssdw mm0,mm0
        packssdw mm1,mm1

        movd    ecx,mm0
        mov     [PTR_EDI],cx
        shr     ecx,16
        mov     [PTR_EDI + PTR_EAX],cx
        lea     PTR_EDI,[PTR_EDI + PTR_EAX*2]

        movd    ecx,mm1
        mov     [PTR_EDI],cx
        shr     ecx,16
        mov     [PTR_EDI + PTR_EAX],cx
        lea     PTR_EDI,[PTR_EDI + PTR_EAX*2]

        ENDM

; finishes an ongoing swapped sum by taking a difference of the two partial sums (sum1 - sum0), SSE only
; does two samples in parallel
; stores the 16-bit clamped signed integer results to *EDI, *(EDI+dwSampleStep)
; advances EDI by dwSampleStep*2

MAC_END_DIF_EVEN_2_SSE  MACRO

        movaps  xmm4,xmm1
        movhlps xmm1,xmm0       ; 13 12 03 02
        movlhps xmm0,xmm4       ; 11 10 01 00
        addps   xmm0,xmm1       ; 13+11 12+10 03+01 02+00
        shufps  xmm0,xmm0,0D8h  ; 13+11 03+01 12+10 02+00
        movhlps xmm1,xmm0       ; XXXXX XXXXX 13+11 03+01
        subps   xmm0,xmm1       ; XXXXX XXXXX dif1  dif0

        cvtps2pi mm0,xmm0       ; integer result in mm4
        packssdw mm0,mm0

        movd    ecx,mm0
        mov     [PTR_EDI],cx
        shr     ecx,16
        mov     [PTR_EDI + PTR_EAX],cx
        lea     PTR_EDI,[PTR_EDI + PTR_EAX*2]

        ENDM

; finishes an ongoing swapped sum by taking a difference of the two partial sums (sum1 - sum0), SSE only
; does one sample
; stores the 16-bit clamped signed integer result to *EDI
; advances EDI by dwSampleStep

MAC_END_DIF_EVEN_1_SSE  MACRO

        movhlps xmm4,xmm0
        addps   xmm0,xmm4       ; don't care, don't care, result 1 + result 3, result 0 + result 2
        movaps  xmm4,xmm0       ; result 0 + result 2
        shufps  xmm0,xmm0,001h  ; result 1 + result 3
        subss   xmm4,xmm0       ; (result 0 + result 2) - (result 1 + result 3)

        cvtps2pi mm0,xmm4       ; integer result in MM0
        packssdw mm0,mm0

        movd    ecx,mm0

        mov     [PTR_EDI],cx
        add     PTR_EDI,PTR_EAX

        ENDM

; finishes an ongoing sum by taking a difference of the two partial sums
; stores the 16-bit clamped signed integer result to *EDI
; advances EDI by dwSampleStep

MAC_END_DIF     MACRO
        LOCAL __t1,__c1,__c2,__done

    IFDEF AMD   ;-------------

        pfsub   mm1,mm3
        pf2id   mm1,mm1
        packssdw mm1,mm1
        movd    ecx,mm1
        mov     [PTR_EDI],cx
        add     PTR_EDI,PTR_EAX

    ELSE
     IFDEF SSE  ;-------------

        subss   xmm0,xmm1
        cvtss2si ecx,xmm0

     ELSE       ;-------------

        fsubrp  st(1),st
        fistp   DWORD PTR temp
        mov     ecx,temp

     ENDIF      ;-------------

        cmp     ecx,32767
        jg      __c1
        cmp     ecx,-32768
        jl      __c2
__t1:
        mov     [PTR_EDI],cx
        add     PTR_EDI,PTR_EAX
        jmp     __done

__c1:
        mov     ecx,32767
        jmp     __t1
__c2:
        mov     ecx,-32768
        jmp     __t1

__done:

    ENDIF   ;-----------------

        ENDM

;=============================================================================

        IFDEF AMD
                  IFDEF WIN64

AMD_dewindow_and_write   PROC
                         ENTER64 8

lpU                      TEXTEQU <QWORD PTR [r10][1*8]> 
lpDewindow               TEXTEQU <QWORD PTR [r10][2*8]>    
dwStart                  TEXTEQU <DWORD PTR [r10][3*8]>    
lpSamples                TEXTEQU <QWORD PTR [r10][4*8]>    
dwSampleStep             TEXTEQU <DWORD PTR [r10][5*8]>    
dwDiv                    TEXTEQU <DWORD PTR [r10][6*8]>    
                  ELSE

AMD_dewindow_and_write PROC \
        USES PTR_EBX PTR_ESI PTR_EDI    \
        lpU:PTR,            \
        lpDewindow:PTR,     \
        dwStart:DWORD,      \
        lpSamples:PTR,      \
        dwSampleStep:DWORD, \
        dwDiv:DWORD
                  ENDIF

        ELSE
         IFDEF SSE

                  IFDEF WIN64

SSE_dewindow_and_write   PROC
                         ENTER64 8

lpU                      TEXTEQU <QWORD PTR [r10][1*8]> 
lpDewindow               TEXTEQU <QWORD PTR [r10][2*8]>    
dwStart                  TEXTEQU <DWORD PTR [r10][3*8]>    
lpSamples                TEXTEQU <QWORD PTR [r10][4*8]>    
dwSampleStep             TEXTEQU <DWORD PTR [r10][5*8]>    
dwDiv                    TEXTEQU <DWORD PTR [r10][6*8]>    
                  ELSE

SSE_dewindow_and_write PROC \
        USES PTR_EBX PTR_ESI PTR_EDI    \
        lpU:PTR,            \
        lpDewindow:PTR,     \
        dwStart:DWORD,      \
        lpSamples:PTR,      \
        dwSampleStep:DWORD, \
        dwDiv:DWORD
                  ENDIF

         ELSE

                  IFDEF WIN64

x86_dewindow_and_write   PROC
                         ENTER64 8

lpU                      TEXTEQU <QWORD PTR [r10][1*8]> 
lpDewindow               TEXTEQU <QWORD PTR [r10][2*8]>    
dwStart                  TEXTEQU <DWORD PTR [r10][3*8]>    
lpSamples                TEXTEQU <QWORD PTR [r10][4*8]>    
dwSampleStep             TEXTEQU <DWORD PTR [r10][5*8]>    
dwDiv                    TEXTEQU <DWORD PTR [r10][6*8]>    
                  ELSE

x86_dewindow_and_write PROC \
        USES PTR_EBX PTR_ESI PTR_EDI    \
        lpU:PTR,            \
        lpDewindow:PTR,     \
        dwStart:DWORD,      \
        lpSamples:PTR,      \
        dwSampleStep:DWORD, \
        dwDiv:DWORD
                  ENDIF
         ENDIF
        ENDIF

        IFDEF WIN64
temp      equ [PTR_EBP-8]
        ELSE
          LOCAL temp
        ENDIF

        mov PTR_ESI,lpU
        mov PTR_EDI,lpSamples

        IFDEF WIN64
        movsxd rax,dwStart
        neg rax
        ELSE
        mov eax,dwStart
        neg eax
        ENDIF

        mov PTR_EBX,lpDewindow
        lea PTR_EBX,[PTR_EBX + PTR_EAX*4 + 16*4]    ; adjust up by 16-dwStart input values

        IFDEF WIN64
        movsxd rax,dwSampleStep
        ELSE
        mov eax,dwSampleStep
        ENDIF

        ; EAX = dwSampleStep
        ; EBX = dewindow (24.8)
        ; ECX = scratch
        ; EDX = used for loop termination addresses
        ; ESI = u        (8.24)
        ; EDI = output samples (S16)

        _FEMMS

        ;First 16 samples

        lea PTR_EDX,[PTR_ESI][16*16*4]          ; end address - we'll do 16 samples, each using a
                                        ; block of 16 input values from each stream

__for_loops:

    IFDEF SSE   ;------------------------------------------------

        MAC_INIT_PAIR_SSE   PTR_ESI,PTR_EBX
        CNT = 16

        REPT 3

            MAC_PAIR_SSE  [PTR_ESI+CNT],[PTR_EBX+CNT]
            CNT = CNT + 16

        ENDM

        add PTR_EBX,32*4*4
        add PTR_ESI,16*4*4

        MAC_END_SUM_SSE

    ELSE    ; non-SSE -------------------------------------------

        MAC_INIT_PAIR  [PTR_ESI],[PTR_EBX],[PTR_ESI+4],[PTR_EBX+4]
        CNT = 8

        REPT 7

            MAC_PAIR  [PTR_ESI+CNT],[PTR_EBX+CNT],[PTR_ESI+CNT+4],[PTR_EBX+CNT+4]
            CNT = CNT + 8

        ENDM

        add PTR_EBX,32*4
        add PTR_ESI,16*4

        MAC_END_SUM

    ENDIF   ;----------------------------------------------------

        cmp PTR_ESI,PTR_EDX
        jne __for_loops

        test dwDiv,1
        jz __even_segment

        ; odd segment, 17th sample

        MAC_INIT_PAIR [PTR_ESI],[PTR_EBX],[PTR_ESI+8],[PTR_EBX+8]
        CNT = 16

        REPT 3

            MAC_PAIR [PTR_ESI+CNT],[PTR_EBX+CNT],[PTR_ESI+CNT+8],[PTR_EBX+CNT+8]
            CNT = CNT + 16

        ENDM

        MAC_END_SUM

        IFDEF WIN64
        movsxd PTR_ECX,dwStart
        ELSE
        mov ecx,dwStart
        ENDIF
        lea PTR_EBX,[PTR_EBX][PTR_ECX*8][-48*4]

        ; odd segment, last 15 samples

    IFDEF SSE   ;------------------------------------------------

        ; do first 12 samples 4 samples at a time

        lea edx,[PTR_ESI][-12*16*4]

__odd_loops:

        ASCEND = 0*4
        DESCEND = 15*4
        MAC_INIT_PAIR_SWAPPED_4_SSE [PTR_ESI+ASCEND-16*4],[PTR_EBX+DESCEND-12]

        REPT 3

            DESCEND = DESCEND - 16
            ASCEND = ASCEND + 16
            MAC_PAIR_SWAPPED_4_SSE    [PTR_ESI+ASCEND-16*4],[PTR_EBX+DESCEND-12]

        ENDM

        sub PTR_ESI,16*4*4
        sub PTR_EBX,32*4*4

        MAC_END_DIF_ODD_4_SSE

        cmp PTR_ESI,PTR_EDX
        jne __odd_loops

        ; do next 2 samples 2 samples at a time

        ASCEND = 0*4
        DESCEND = 15*4
        MAC_INIT_PAIR_SWAPPED_2_SSE [PTR_ESI+ASCEND-16*4],[PTR_EBX+DESCEND-12]

        REPT 3

            DESCEND = DESCEND - 16
            ASCEND = ASCEND + 16
            MAC_PAIR_SWAPPED_2_SSE    [PTR_ESI+ASCEND-16*4],[PTR_EBX+DESCEND-12]

        ENDM

        MAC_END_DIF_ODD_2_SSE

        ; do last sample

        ASCEND = 0*4
        DESCEND = 15*4
        MAC_INIT_PAIR_SWAPPED_1_SSE   [PTR_ESI+ASCEND-16*4-16*4*2],[PTR_EBX+DESCEND-12-32*4*2]

        REPT 3

            DESCEND = DESCEND - 16
            ASCEND = ASCEND + 16
            MAC_PAIR_SWAPPED_1_SSE  [PTR_ESI+ASCEND-16*4-16*4*2],[PTR_EBX+DESCEND-12-32*4*2]

        ENDM

        MAC_END_DIF_ODD_1_SSE

    ELSE    ; non-SSE -------------------------------------------

        lea edx,[PTR_ESI][-15*16*4]

__odd_loops:

        ASCEND = 0*4
        DESCEND = 15*4

        MAC_INIT_PAIR [PTR_ESI+ASCEND+4-16*4],[PTR_EBX+DESCEND-4],[PTR_ESI+ASCEND-16*4],[PTR_EBX+DESCEND]

        REPT 7

            DESCEND = DESCEND - 8
            ASCEND = ASCEND + 8

            MAC_PAIR [PTR_ESI+ASCEND+4-16*4],[PTR_EBX+DESCEND-4],[PTR_ESI+ASCEND-16*4],[PTR_EBX+DESCEND]

        ENDM

        sub PTR_ESI,16*4
        sub PTR_EBX,32*4

        MAC_END_DIF

        cmp PTR_ESI,PTR_EDX
        jne __odd_loops

    ENDIF   ;----------------------------------------------------

        jmp __exit

__even_segment:
        ; even segment, 17th sample

        MAC_INIT_PAIR [PTR_ESI+4],[PTR_EBX+4],[PTR_ESI+12],[PTR_EBX+12]
        CNT = 16

        REPT 3

            MAC_PAIR [PTR_ESI+CNT+4],[PTR_EBX+CNT+4],[PTR_ESI+CNT+12],[PTR_EBX+CNT+12]
            CNT = CNT + 16

        ENDM

        MAC_END_SUM

        IFDEF WIN64
        movsxd PTR_ECX,dwStart
        ELSE
        mov ecx,dwStart
        ENDIF

        lea PTR_EBX,[PTR_EBX][PTR_ECX*8][-48*4]

        ; even segment, last 15 samples

    IFDEF SSE   ;------------------------------------------------

        ; do first 12 samples 4 samples at a time

        lea edx,[PTR_ESI][-12*16*4]

__even_loops:

        ASCEND = 0*4
        DESCEND = 15*4
        MAC_INIT_PAIR_SWAPPED_4_SSE [PTR_ESI+ASCEND-16*4],[PTR_EBX+DESCEND-12]

        REPT 3

            DESCEND = DESCEND - 16
            ASCEND = ASCEND + 16
            MAC_PAIR_SWAPPED_4_SSE    [PTR_ESI+ASCEND-16*4],[PTR_EBX+DESCEND-12]

        ENDM

        sub PTR_ESI,16*4*4
        sub PTR_EBX,32*4*4

        MAC_END_DIF_EVEN_4_SSE

        cmp PTR_ESI,PTR_EDX
        jne __even_loops

        ; do next 2 samples 2 samples at a time

        ASCEND = 0*4
        DESCEND = 15*4
        MAC_INIT_PAIR_SWAPPED_2_SSE [PTR_ESI+ASCEND-16*4],[PTR_EBX+DESCEND-12]

        REPT 3

            DESCEND = DESCEND - 16
            ASCEND = ASCEND + 16
            MAC_PAIR_SWAPPED_2_SSE    [PTR_ESI+ASCEND-16*4],[PTR_EBX+DESCEND-12]

        ENDM

        MAC_END_DIF_EVEN_2_SSE

        ; do last sample

        ASCEND = 0*4
        DESCEND = 15*4
        MAC_INIT_PAIR_SWAPPED_1_SSE   [PTR_ESI+ASCEND-16*4-16*4*2],[PTR_EBX+DESCEND-12-32*4*2]

        REPT 3

            DESCEND = DESCEND - 16
            ASCEND = ASCEND + 16
            MAC_PAIR_SWAPPED_1_SSE  [PTR_ESI+ASCEND-16*4-16*4*2],[PTR_EBX+DESCEND-12-32*4*2]

        ENDM

        MAC_END_DIF_EVEN_1_SSE

    ELSE    ; non-SSE -------------------------------------------

        lea edx,[PTR_ESI][-15*16*4]

__even_loops:

        ASCEND = 0*4
        DESCEND = 15*4
        MAC_INIT_PAIR [PTR_ESI+ASCEND-16*4],[PTR_EBX+DESCEND],[PTR_ESI+ASCEND+4-16*4],[PTR_EBX+DESCEND-4]

        REPT 7

            DESCEND = DESCEND - 8
            ASCEND = ASCEND + 8
            MAC_PAIR [PTR_ESI+ASCEND-16*4],[PTR_EBX+DESCEND],[PTR_ESI+ASCEND+4-16*4],[PTR_EBX+DESCEND-4]

        ENDM

        sub PTR_ESI,16*4
        sub PTR_EBX,32*4

        MAC_END_DIF

        cmp PTR_ESI,PTR_EDX
        jne __even_loops

    ENDIF   ;----------------------------------------------------

__exit:
        _FEMMS

        IFDEF SSE
        emms
        ENDIF

        IFDEF WIN64
        EXIT64
        ENDIF
        ret

        IFDEF AMD
AMD_dewindow_and_write ENDP
        ELSE
         IFDEF SSE
SSE_dewindow_and_write ENDP
         ELSE
x86_dewindow_and_write ENDP
         ENDIF
        ENDIF

db 13,10,13,10
db 'Miles Sound System',13,10
db 'Copyright (C) 1991-2007 RAD Game Tools, Inc.',13,10,13,10

        ;
        ;End of locked code
        ;

IFDEF AMD
amd_code_end LABEL BYTE
public amd_code_end
ELSE
 IFDEF SSE
sse_code_end LABEL BYTE
public sse_code_end
 ELSE
x86_code_end LABEL BYTE
public x86_code_end
 ENDIF
ENDIF

;#############################################################################
;##                                                                         ##
;## Lock all code and data in AILSSA module                                 ##
;##                                                                         ##
;#############################################################################

        IFDEF AMD

        EXTRN x86_data_start:BYTE,x86_data_end:BYTE,x86_code_start:BYTE,x86_code_end:BYTE

		IFDEF DPMI

AIL_vmm_lock_range PROTO C,P1:NEAR PTR,P2:NEAR PTR

MATHA_VMM_lock  PROC C \
        USES PTR_EBX PTR_ESI PTR_EDI ds es

        invoke AIL_vmm_lock_range,OFFSET amd_data_start,OFFSET amd_data_end
        invoke AIL_vmm_lock_range,OFFSET amd_code_start,OFFSET amd_code_end

        invoke AIL_vmm_lock_range,OFFSET x86_data_start,OFFSET x86_data_end
        invoke AIL_vmm_lock_range,OFFSET x86_code_start,OFFSET x86_code_end

        ret

MATHA_VMM_lock  ENDP

        ENDIF

        ENDIF

        END
