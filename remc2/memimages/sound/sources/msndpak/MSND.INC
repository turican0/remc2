

ISCOM           equ     1
; ISEXE           equ     not ISCOM

DEF_INT_BIT     equ     4h
PIC2            equ     0A0h
PIC1            equ     020h
EOI             equ     020h
DEF_EOI_REG     equ     PIC2

PLAYBACK	equ	1	;0
MASSAGE 	equ	2	;1
FIXEDFREQ	equ	4	;2
USESTIMER	equ	8	;3
SHARESTIMER	equ	16	;4 True if supports timer sharing.
LOOPEND 	equ	32	;5 Supports looping,and pending sounds.
STEREOPAN	equ	64	;6 supports stereo panning.
STEREOPLAY	equ	128	;7 Supports stereo playback!
AUDIORECORD	equ	256	;8 support digital sound recording.


Struc	SOUNDSPEC

PLAYADR FARPTR	<>	; Address of sound sample to be played.
PLAYLEN dw	?	; Length of sound sample to be played.
ISPLAYING FARPTR <>	 ; Address of our playing sample semephore.
FREQUENCY dw	?	; Frequency to play it at.

	Ends

Macro	IN_TSR
	push	ds
	push	es
	push	si
	push	di
	mov	ax,cs
	mov	ds,ax
	mov	es,ax
	endm

Macro	OUT_TSR
	pop	di
	pop	si
	pop	es
	pop	ds
	endm

Macro	WaitSound
	LOCAL	@@WT
@@WT:	mov	ax,689h
	int	66h
	or	ax,ax
	jnz	@@WT
	endm

Macro	Dally
	jmp	$+2
	endm

PCM_8           equ     8
PCM_16          equ     16
PCM_MONO        equ     1
PCM_STEREO      equ     2


KINT	equ	66h

