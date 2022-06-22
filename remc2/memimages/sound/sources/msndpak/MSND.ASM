
        LOCALS                  ;; Enable local labels

        IDEAL                   ;; Use Turbo Assembler's IDEAL mode
	JUMPS

        INCLUDE "PROLOGUE.MAC"          ;; common prologue
        include "msnd_dsp.equ"
        include "msnd.inc"


SEGMENT  _TEXT PARA PUBLIC 'CODE'
        ENDS

        ASSUME  CS: _TEXT, DS: _TEXT, SS: NOTHING, ES: NOTHING

SEGMENT _TEXT
IF	ISCOM
	org	100h
ENDIF
START:
	jmp	LoadSound		; Load the digitized sound driver
	db	"DIGPAK",0,13,10        ; Digitized Sound Package Identity String
IDENTIFIER:
;; =============[*][	     1	       2	 3	    ]
;; =============[*][1234567890123456789012345678901234567890]
IF      MSND
        db      "Turtle Beach MultiSound",0,13,10
ENDIF
	db	"The Audio Solution, Copyright (c) 1992",0,13,10
	db	"Written by John W. Ratcliff",0,13,10
IF	ISCOM
	org	200h		; Beginning address of jumps.
	jmp	InstallInterupt 	; Install the interupt vector.
	jmp	DeInstallInterupt	; De-install the interupt.
ENDIF
;; Unique data areas to individual hardware implementations.
_wPort          DW      290h        ; Default I/O address.
_intr_num       DW      10           ; Default is interupt #10
_MsndSeg        dw      0D000h

_bIsrRunning    dw      0
_bTrueInt       db      ?
_bIntBit        db      DEF_INT_BIT             ; IRQ 10
_wEOIreg        dw      DEF_EOI_REG
_wNumBytes      dw      ?
_AudioSeg       dw      ?
_AudioOff       dw      ?
_AudioFlag      dw      0

INDIGPAK	dw	0

;; Set's the DIGPAK semaphore
Macro	SetSemaphore
	mov	[cs:INDIGPAK],1
	endm

;; Clear's the semaphore, and does an IRET
Macro	ClearSemaphoreIRET
	mov	[cs:INDIGPAK],0
	iret
	endm

Macro	ClearSemaphore
	mov	[cs:INDIGPAK],0
	endm


JumpTable       dw      offset  FUNCT1
		dw	offset	FUNCT2
		dw	offset	FUNCT3
		dw	offset	FUNCT4
		dw	offset	FUNCT5
		dw	offset	FUNCT6
		dw	offset	FUNCT7
		dw	offset	FUNCT8
		dw	offset	FUNCT9
		dw	offset	FUNCTA
		dw	offset	FUNCTB
		dw	offset	FUNCTC
		dw	offset	FUNCTD
		dw	offset	FUNCTE
		dw	offset	FUNCTF
		dw	offset	FUNCT10
		dw	offset	FUNCT11
		dw	offset	FUNCT12
		dw	offset	FUNCT13
		dw	offset	FUNCT14
		dw	offset	FUNCT15 ; Set DMA backfill mode.
		dw	offset	FUNCT16 ; Report DMAC count.
		dw	offset	FUNCT17 ; Verify DMA block.
		dw	offset	FUNCT18 ; Set PCM volume.

JumpPtr 	dw	?
_voice_status	dw	0
CallBacks	dw	0	; Callback to application flag.
LABEL		CALLBACK	DWORD	     ; Callback address label.
CallLow 	dw	0	; Low word of callback address.
CallHigh	dw	0	; High word of callback address.
CallDS		dw	0	; Value of DS register at callback time.

DivisorRate	dw	0	; Default divisor rate.

RecordMode	dw	0	; set audio recording flag.
PlayModeChn     dw      PCM_MONO      ; Default play mode is 8 bit PCM.
PlayModeBit     dw      PCM_8
PlayModeRate    dw      11025

;IF      DEBUG
;DebugOffset     dw      0
;ENDIF

OldIntSeg       dw      ?
OldIntOff       dw      ?

;; Data used by Kernel interupt
KJUMP	FARPTR	<>		; Address
OLDIN	FARPTR	<>		; Original interupt vector.
ID      db      'KERN'          ; 4B45524Eh Interupt identifier string.
IND     db      'KR'            ; 4B52h indicates a kernel installed interupt.
Proc	SoundInterupt far
;;; Usage: DS:SI -> point to sound structure to play.
;; FUNCT1  AX = 0688h	 DigPlay
;; FUNCT2  AX = 0689h	 Sound Status
;; FUNCT3  AX = 068Ah	 Massage Audio
;; FUNCT4  AX = 068Bh	 DigPlay2, pre-massaged audio.
;; FUNCT5  AX = 068Ch	 Report audio capabilities.
;; FUNCT6  AX = 068Dh	 Report playback address.
;; FUNCT7  AX = 068Eh	 Set Callback address.
;; FUNCT8  AX = 068Fh	 Stop Sound.
;; FUNCT9  AX = 0690h	 Set Hardware addresses.
;; FUNCTA  AX = 0691h	 Report Current callback address.
;; FUNCTB  AX = 0692h	 Restore hardware vectors.
;; FUNCTC  AX = 0693h	 Set Timer Divisor Sharing Rate
;; FUNCTD  AX = 0694h	 Play preformatted loop
;; FUNCTE  AX = 0695h	 Post Pending Audio
;; FUNCTF  AX = 0696h	 Report Pending Status
;; FUNCT10 AX = 0697h	 Set Stereo Panning value.
;; FUNCT11 AX = 698h	 Set DigPak Play mode.
;; FUNCT12 AX = 699h	 Report Address of pending status flag.
;; FUNCT13 AX = 69Ah	 Set Recording mode 0 off 1 on.
;; FUNCT14 AX = 69Bh	 StopNextLoop
;; FUNCT15 AX = 69Ch	 Set DMA backfill mode.
;; FUNCT16 AX = 69Dh	 Report current DMAC count.
;; FUNCT17 AX = 69Eh	 Verify DMA block.
;; FUNCT18 AX = 69Fh	 Set PCM volume.
	cmp	ax,0688h
	jb	@@CHAIN
	cmp	ax,069Fh
	ja	@@CHAIN

        SetSemaphore
	sti
	sub	ax,0688h

        shl     ax,1
	add	ax,offset JumpTable
	xchg	ax,bx
	mov	bx,[cs:bx]
	xchg	ax,bx
;        int     1 ; debug breakpoint
        mov     [cs:JumpPtr],ax
	jmp	[cs:JumpPtr]	;; Near jump will be modified!!
@@CHAIN:
	cmp	[OLDIN.XPTR.POFF],0
	jne	@@CHAIN
	cmp	[OLDIN.XPTR.PSEG],0
	je	@@IRET
	jmp	[cs:OLDIN.DPTR] 	; Chain to original interrupt vector.
@@IRET:
	ClearSemaphoreIRET
	endp


FUNCT1:
;;**************************************************************************
;:Function #1: DigPlay, Play an 8 bit digitized sound.
;:
;:	  INPUT:  AX = 688h    Command number.
;:		  DS:SI        Point to a sound structure that
;:			       describes the sound effect to be played.
;;**************************************************************************
	PushCREGS

	call	CompleteSound
	call	SetAudio
	call	PlaySound

	PopCREGS
	ClearSemaphoreIRET
FUNCT2:
;;**************************************************************************
;:Function #2: SoundStatus, Check current status of sound driver.
;:
;:	  INPUT:  AX = 689h
;:	  OUTPUT: AX = 0       No sound is playing.
;:		     = 1       Sound effect currently playing.
;;		    DX = 1	 Looping a sound effect
;;		  BX = Version numer, in decimal, times 100, so that 3.00
;;		       would be 300.  Version number begins with version 3.10
;;		       which includes the DigPak semaphore.
;;**************************************************************************
	mov	bx,320	; Return VERSION NUMBER in BX! 3.20
	cmp	[cs:LOOPING],1	; Looping a sample?
	jne	@@REP
	xor	ax,ax
	mov	dx,1		; Return high word looping flag.
	ClearSemaphoreIRET
@@REP:
	mov	ax,[cs:_voice_status]
	xor	dx,dx		; Not looping
	ClearSemaphoreIRET
FUNCT3:
;;**************************************************************************
;:Function #3: MassageAudio, Preformat audio data into ouptut hardware format.
;:
;:	  INPUT:  AX = 68Ah
;:		  DS:SI        Point to address of sound structure.
;;**************************************************************************
	ClearSemaphoreIRET
FUNCT4:
;;**************************************************************************
;:Function #4: DigPlay2, Play preformatted audio data.
;:
;:	  INPUT:  AX = 68Bh
;:		  DS:SI        Point to address of sound structure.
;;**************************************************************************
	PushCREGS

	call	CompleteSound
	call	DoSoundPlay
        mov     [cs:FROMLOOP],0    ; Turn from loop semaphore off.

	PopCREGS
	ClearSemaphoreIRET
FUNCT5:
;;**************************************************************************
;:Function #5: AudioCapabilities, Report capabilities of hardware device.
;:
;:	  INPUT:  AX = 68Ch
;:	  OUTPUT: AX = Bit 0 -> On, supports background playback.
;:				Off, driver only plays as a foreground process.
;:		       Bit 1 -> On, source data is reformatted for output device.
;:				 Off, device handles raw 8 bit unsigned audio.
;:		       Bit 2 -> On, Device plays back at a fixed frequency, but
;:				    the audio driver will downsample input data
;:				    to fit.
;:				Off, device plays back at user specified frequency.
;:				(NOTE: You can still playback an audio sample at
;:				       whatever frequency you wish.  The driver
;:				       will simply downsample the data to fit
;:				       the output hardware.  Currently it does
;:				       not support upsampling though.)
;:		       Bit 3 -> On, this device uses the timer interrupt vector
;:				during sound playback.
;:		  DX = If this device plays back at a fixed frequency the DX
;:		       register will contain that fixed frequency playback rate.
;;**************************************************************************
        xor     ax, ax
        or      ax, STEREOPLAY
        or      ax, PLAYBACK
        or      ax, STEREOPAN
        or      ax, LOOPEND   ;?

        mov     bx,cs
	lea	cx,[IDENTIFIER]
	ClearSemaphoreIRET
FUNCT6:
;;**************************************************************************
;:Function #6: ReportSample, Report current playback address.
;:
;:	  INPUT:  AX = 68Dh
;:	  OUTPUT: AX = Current playback address.  Obviously this only
;:		       applies to background drivers.  Note that for some
;:		       drivers this playback address is an aproximation
;:		       and not necessarily the EXACT sample location.
;:		       You can use this service to synchronize
;:		       animation or video effects temporaly with the
;:		       audio output.
;;**************************************************************************
        mov     ax, [cs:_AudioOff]
	ClearSemaphoreIRET
FUNCT7:
;;**************************************************************************
;:Function #7: SetCallBackAddress, sets a user's sound completion
;:		       callback addess.
;:
;:	  INPUT: AX = 068Eh
;:		 BX = Offset portion of far procedure to callback.
;:		 DX = Segment portion of far procedure to callback.
;:		 DS = Data Segment register value to load at callback time.
;:	  OUTPUT: None.
;:
;:		 This function allows the user to specify a callback
;:		 address of a far procedure to be invoked when a sound
;:		 effect has completed being played.  This function is
;:		 disabled by default.  Sending a valid address to this
;:		 function will cause a callback to occur whenever a sound
;:		 sample has completed being played.  The callers DS register
;:		 will be loaded for him at callback time.  Be very careful
;:		 when using this feature.  The application callback procedure
;:		 is being invoked typically during a hardware interupt.
;:		 Your application should spend a small an amount of time
;:		 as possible during this callback.  Remember that the
;:		 callback must be a far procedure.  The sound driver
;:		 preserves ALL registers so your callback function does
;:		 not need to do so.  Do not perform any DOS functions
;:		 during callback time because DOS is not re-entrent.
;:		 Keep in mind that your own application has been interupted
;:		 by the hardware it this point.  Be very careful when making
;:		 assumptions about the state of your application during
;:		 callback time.  Hardware callbacks are generally used
;:		 to communicate sound event information to the application
;:		 or to perform a technique called double-buffering, whereby
;:		 your application immediatly posts another sound effect to
;:		 be played at the exact time that the last sound effect
;:		 has completed.
;:
;:		 WARNING!!! Be sure to turn off hardware callbacks when
;:		 your application leaves!!! Otherwise, harware callbacks
;:		 will be pointing off into memory that no longer contains
;:		 code.	This function is for advanced programmers only.
;;**************************************************************************
	or	bx,bx
	jnz	@@SC1
	or	dx,dx
	jnz	@@SC1
	xor	ax,ax
	mov	[cs:CallBacks],ax		; Callbacks disabled.
	mov	[cs:CallLow],ax 		; Low address.
	mov	[cs:CallHigh],ax
	jmp	@@EXIT
@@SC1:	mov	[cs:CallLow],bx
	mov	[cs:CallHigh],dx
	mov	[cs:CallDS],ds
	mov	[cs:CallBacks],1

@@EXIT:
	ClearSemaphoreIRET
FUNCT8:
;;**************************************************************************
;:Function #8: StopSound, stop currently playing sound.
;:
;:	  INPUT: AX = 68Fh
;:	  OUTPUT: None.
;:
;:		Will cause any currently playing sound effect to be
;:		terminated.
;;**************************************************************************
	call	EndLoop 	; End looped sample if there is one playing
	call	StopSound
	ClearSemaphoreIRET
FUNCT9:
;;**************************************************************************
;:Function #9: SetAudioHardware, set up hardware information.
;:
;:	  INPUT: AX = 690h
;:		 BX = IRQ if device needs one set.
;:		 CX = BASE I/O Address, if device needs one set.
;:		 DX = OTHER, some other possible information the hardware might need.
;:
;:	  OUTPUT: NONE.
;:
;:
;:	  Certain harware can be reconfigured to different IRQ and base
;:	  address settings.  This function call allows the application
;:	  programmer to overide these default settings.  The interpretation
;:	  of these parameters might change from driver to driver.  Currently
;:	  only the SBLASTER (Creative Labs SoundBlaster) driver can be
;:	  reconfigured, upon request of Derek Smart.
;;**************************************************************************

	ClearSemaphoreIRET
FUNCTA:
;;**************************************************************************
;;FUNCTION #10: ReportCallbackAddress
;;
;;	  INPUT: AX = 691h
;;	  OUTPUT: AX:DX -> far pointer to current callback address.
;;		  BX -> original caller's DS register.
;;
;;	  This function should probably never need to be used by your
;;	  application software.  It is provided because the MIDPAK,
;;	  MIDI driver, needs to revector hardware callbacks so that
;;	  it can handle hardware contention problems between digitized
;;	  sound playback and synthesized sound playback.
;;**************************************************************************
	mov	ax,[cs:CallLow]
	mov	dx,[cs:CallHigh]
	mov	bx,[cs:CallDS]
	ClearSemaphoreIRET
FUNCTB:
;;**************************************************************************
;;FUNCTION #11: RestoreHardware
;;
;;	  INPUT: AX = 692h
;;	  OUTPUT:
;;
;;		Put hardware back to initial state.  Invoked by the
;;		DeInstall code.  Not to be called by an application program!
;;**************************************************************************
	push	ds
	push	cs
	pop	ds
        CALLF   RestoreHardISR
	pop	ds
	ClearSemaphoreIRET
FUNCTC:
;;**************************************************************************
;; FUNCTION #12: SetTimerDivsorRate
;;
;;	   INPUT: AX = 693h
;;		  DX = Countdown timer divisor rate, so that timer based
;;		       drivers can service application timer interrupts
;;		       at their previous rate.	Service rate will be an
;;		       aproximation, that is fairly close.  To reset timer
;;		       divisor to default of 18.2 pass a 0 in the DX register.
;;**************************************************************************
	mov	[cs:DivisorRate],dx    ; Set timer divisor rate.
	ClearSemaphoreIRET
FUNCTD:
;;**************************************************************************
;; FUNCTION #13: DigPlayLoop
;;
;;	   INPUT: AX = 694h
;;		  DS:SI ->sound structure, preformated data.
;; Here's the process...
;;	Remember the current callback address.
;;	Set new callback address to US!
;;	Save sound structure.
;;	Call DigPlay.
;;	At call back, keep playing.
;;	This gets done until StopSound is called.
;;	Stop sound checks to see if we need to restore the callback address.
;;	If PlaySound is invoked, and we are currently looping a sound then
;;	stopsound is invoked.
;;**************************************************************************
	PushAll 	; Save all registers.
	push	cs
	pop	es
	lea	di,[LOOPSND]
	mov	cx,SIZE LOOPSND
	rep	movsb
	mov	ax,068Fh	; Stop any currently playing sound.
	int	66h		; do it.
	mov	[cs:LOOPING],1	   ; We are now looping a sound sample.
	mov	ax,cs
	mov	ds,ax
	mov	dx,ax		;
	mov	ax,068Eh
	lea	bx,[LoopBack]	;
	int	66h		; Set loop callback.
	PopAll
	push	cs
	pop	ds
	lea	si,[LOOPSND]
	mov	[word cs:LOOPSOUND],si
	mov	[word cs:LOOPSOUND+2],ds
	mov	[cs:FROMLOOP],1 ; Set from looping semephore
	mov	ax,068Bh	; Do FUNCT4
	jmp	FUNCT4		; Do a DigPlay2

FUNCTE:
;;**************************************************************************
;; FUNCTION #14: PostAudioPending
;;
;;	   INPUT: AX = 695h
;;		  DS:SI ->sound structure, preformated data.
;;	   OUTPUT: AX = 0  Sound was started playing.
;;		   AX = 1  Sound was posted as pending to play.
;;**************************************************************************
	cli    ; Turn off interupts while making this determination.
	mov	ax,[cs:_voice_status]
	or	ax,ax		; Currently playing a sound?
	jnz	@@POST		; yes->try to post pending.
	sti			; We can play it now.
	call	DoSoundPlay	;
	xor	ax,ax		; Return, audio sample is now playing.
	ClearSemaphoreIRET
@@POST: cmp	[cs:PENDING],1	; Already have a pending sound effect?
	jne	@@POST2 	; no, post it for pending play.
	mov	ax,2		; return code of two.
	ClearSemaphoreIRET
@@POST2:mov	[cs:PENDING],1
        call    DoSoundPlay

	ClearSemaphoreIRET
FUNCTF:
;;**************************************************************************
;; FUNCTION #15: PostAudioPending
;;
;;	   INPUT: AX = 696h
;;	  OUTPUT: AX = 0 No sound is playing.
;;		  AX = 1 Sound playing, sound pending.
;;		  AX = 2 Sound playing, no sound pending.
;;**************************************************************************
        cli             ; Clear interrupts while we make this determination.
        mov     ax,[cs:_voice_status]
        or      ax,ax           ; Currently playing a sound?
        jnz     @@POST          ; yes->try to post pending.
	ClearSemaphoreIRET
@@POST: cmp     [cs:PENDING],1  ; Have a sound pending?
        je      @@PEND          ; yes, return pending status.
        mov     ax,1            ; Sound is playing, but no sound is pending.
	ClearSemaphoreIRET
@@PEND:
        mov     ax,2
	ClearSemaphoreIRET
FUNCT10:
;;**************************************************************************
;; FUNCTION #16: SetStereoPan
;;
;;	   INPUT: AX = 697h
;;		  DX = stereo pan value. 0 full volume right.
;;					64 full volume both.
;;				       127 full volume left.
;;	 OUTPUT: AX = 0 command ignored, driver doesn't support stereo panning.
;;		 AX = 1 pan set.
;;**************************************************************************
        cmp     dx, 40h
        jnle    FullLeft

        mov     ax, 1FFh       ; Full Volume Value / 64
        mul     dx

        mov     dx, 7FFFh
        jmp     SetVolumes

FullLeft:
        mov     ax, 7Fh
        sub     dx, ax

        mov     ax, 1FFh
        mul     dx
        mov     dx, ax
        mov     ax, 7FFFh


SetVolumes:
        push    ds                      ; Set up our SMA seg
        mov     ax, [cs:_MsndSeg]
        mov     ds, ax
        assume  ds:SRAM_BANK_0

        mov     [ds:wCurrPlayVolLeft], ax
        mov     [ds:wCurrPlayVolRight], dx
        mov     [ds:wCurrMIDIVolLeft], ax
        mov     [ds:wCurrMIDIVolRight], dx

        pop     ds
        assume  ds:nothing

        mov     ax, 1
	ClearSemaphoreIRET

FUNCT11:
;;**************************************************************************
;; FUNCTION #17: SetPlayMode
;;
;;	   INPUT: AX = 698h
;;		  DX = Play Mode function.
;;			  DX = 0 -> 8 bit PCM
;;			     = 1 -> 8 bit Stereo PCM (left/right)
;;			     = 2 -> 16 bit PCM
;;			     = 3 -> 16 bit PCM stereo.
;;
;;	 OUTPUT: AX = 1 -> mode set.
;;		 AX = 0 -> mode not supported by this driver.
;;
;;**************************************************************************

        mov     [cs:PlayModeChn],1
        mov     [cs:PlayModeBit],8
        cmp     dx, 0
        je      done
        mov     [cs:PlayModeChn],2
        cmp     dx, 1
        je      done
        mov     [cs:PlayModeChn],1
        mov     [cs:PlayModeBit],16
        cmp     dx, 3
        je      done
        mov     [cs:PlayModeChn],2
        cmp     dx, 4
        je done

        xor     ax, ax
done:   mov     ax,1            ; Set!
	ClearSemaphoreIRET

FUNCT12:
;;**************************************************************************
;; FUNCTION #18: Report Address of Pending Flag
;;
;;	   INPUT: AX = 699h
;;
;;	 OUTPUT: AX:DX -> form far address of pending status flag.
;;
;;**************************************************************************
	mov	dx,cs		; Code segment.
	lea	ax,[PENDING]	; Address of pending flag.
	lea	bx,[INDIGPAK]	; Address of semaphore address.
	ClearSemaphoreIRET

FUNCT13:
;;**************************************************************************
;; FUNCTION #19: Set audio recording mode.
;;
;;	   INPUT: AX = 69Ah
;;		  DX = 0 turn audio recording ON.
;;		     = 1 turn audio recording OFF.
;;
;;	 OUTPUT: AX = 0 sound driver doesn't support audio recording.
;;		 AX = 1 audio recording mode is set.
;;
;;**************************************************************************
;        mov     [cs:RecordMode],dx
;        mov     ax,1
        xor     ax,ax           ; Not YET!!
	ClearSemaphoreIRET
FUNCT14:
;;**************************************************************************
;; FUNCTION #20: StopNextLoop
;;
;;	   INPUT: AX = 69Bh
;;
;;	   OUTPUT: NOTHING, Stop Looped sample, next time around.
;;
;;**************************************************************************
	mov	[cs:CallBacks],0
	mov	[cs:LOOPING],0
	ClearSemaphoreIRET
FUNCT15:
;;**************************************************************************
;; FUNCTION #21: Set DMA back fill mode.
;;
;;	   INPUT: AX = 69Ch
;;		  BX = backfill mode 0 means turn it off.
;;		       and a 1 means to turn it off.
;;
;;	   OUTPUT: AX = 1 -> back fill mode set.
;;			0 -> driver doesn't support DMA backfill.
;;
;;**************************************************************************
	xor	ax,ax		; Not a backfill driver.
	ClearSemaphoreIRET
FUNCT16:
;;**************************************************************************
;; FUNCTION #22: Report current DMAC count.
;;
;;	   INPUT: AX = 69Dh
;;
;;	   OUTPUT: AX = Current DMAC count.
;;
;;**************************************************************************
	ClearSemaphoreIRET
FUNCT17:
;;**************************************************************************
;; FUNCTION #23: Verify DMA block, check to see if it crosses a 64k page
;;		 boundary for the user.
;;
;;	   INPUT: AX = 69Eh
;;		  ES:BX -> address of sound.
;;		  CX	-> length of sound effect.
;;
;;	   OUTPUT: AX = 1 Block is ok, DOESN'T cross 64k bounadary.
;;		   AX = 0 block failed, DOES cross 64k boundary.
;;
;;**************************************************************************
	ClearSemaphoreIRET
FUNCT18:
;;**************************************************************************
;; FUNCTION #24: Set PCM volume.
;;
;;	   INPUT: AX = 69Eh
;;		  BX = Left channel volume (or both if mono) 0-256
;;		  CX = Right channel volume (or both if mono) 0-256
;;
;;	   OUTPUT: AX = 1 Volume set
;;		   AX = 0 Device doesn't support volume setting.
;;
;;**************************************************************************
        mov     ax, 0
	ClearSemaphoreIRET

Proc	PlayPending	far
	cmp	[PENDING],1  ; Pending?
	jne	@@not
	mov	[PENDING],0
	mov	[cs:CallBacks],0	; No longer have one pending..
	lea	si,[PENDSND]	; Address of pending sound.
	call	DoSoundPlay	; Do a sound play call.
	ret
@@not:
	mov	[cs:CallBacks],0	; Disable callbacks.
	ret
	endp

Proc	DoSoundPlay	near
	PushCREGS
	call	SetAudio
	call	PlaySound
	PopCREGS
	ret
	endp


Proc	CheckCallBack	near
	cmp	[cs:CallBacks],0	; Callbacks enabled?
	je	@@GOUT		; no, exit.
	PushAll 		; Save all registers
	mov	ds,[cs:CallDS]	; Get DS register.
	call	[cs:CallBack]	; far call to application.
	PopAll			; Restore all registers.
@@GOUT:
	ret
	endp


FROMLOOP	dw	0
SAVECALLBACK	dd	?	; Saved callback address.
SAVECALLDS	dw	?
LOOPING 	dw	0	; True if we were looping.

LOOPSOUND	dd	?
LOOPSND 	SOUNDSPEC	<>

PENDING 	dw	0	; True, when second sound sample is pending.
PENDSND         SOUNDSPEC <>    ; Sound structure of pending sound.

Proc	LoopBack	far
	mov	ax,068Bh	; Play preformated data.
	mov	[cs:FROMLOOP],1
	lds	si,[LOOPSOUND]	;
	int	66h		; Start playing the sound again.
	ret
	endp

Proc    SetAudio        near
	mov	[ds:(SOUNDSPEC ptr si).ISPLAYING.XPTR.POFF],offset _voice_status
	mov	[ds:(SOUNDSPEC ptr si).ISPLAYING.XPTR.PSEG],cs
	les	bx,[ds:(SOUNDSPEC ptr si).PLAYADR.DPTR]
	mov	cx,[ds:(SOUNDSPEC ptr si).PLAYLEN]
	mov	dx,[ds:(SOUNDSPEC si).FREQUENCY]

        push    ds                      ; Set up our SMA seg
        mov     ax, [cs:_MsndSeg]
        mov     ds, ax
        assume  ds:SRAM_BANK_0

        push    bx                      ; save offset of sample address

	mov	bx, offset wDAPQbuff
        mov     ax, [cs:PlayModeBit]
        mov     [ds:(DAQueueDataStruc ptr bx).wDAQD_SampleSize], ax
        mov     ax, [cs:PlayModeChn]
        mov     [ds:(DAQueueDataStruc ptr bx).wDAQD_Channels], ax
        mov     [ds:(DAQueueDataStruc ptr bx).wDAQD_SampleRate], dx
;        mov     [ds:(DAQueueDataStruc ptr bx).wDAQD_SampleRate], 11025

	add	bx, DAQD_STRUC_SIZE*2
        mov     ax, [cs:PlayModeBit]
        mov     [ds:(DAQueueDataStruc ptr bx).wDAQD_SampleSize], ax
        mov     ax, [cs:PlayModeChn]
        mov     [ds:(DAQueueDataStruc ptr bx).wDAQD_Channels], ax
        mov     [ds:(DAQueueDataStruc ptr bx).wDAQD_SampleRate], dx
;        mov     [ds:(DAQueueDataStruc ptr bx).wDAQD_SampleRate], 11025

	add	bx, DAQD_STRUC_SIZE*2
        mov     ax, [cs:PlayModeBit]
        mov     [ds:(DAQueueDataStruc ptr bx).wDAQD_SampleSize], ax
        mov     ax, [cs:PlayModeChn]
        mov     [ds:(DAQueueDataStruc ptr bx).wDAQD_Channels], ax
        mov     [ds:(DAQueueDataStruc ptr bx).wDAQD_SampleRate], dx
;        mov     [ds:(DAQueueDataStruc ptr bx).wDAQD_SampleRate], 11025

        pop     bx
        pop     ds

	push	cs
	pop	ds		; DS = Code group.
        assume  ds:_TEXT
	ret
	endp

Proc	EndLoop near
	cmp	[cs:LOOPING],0	   ; Are we doing a loop?
	je	@@YES		; no, skip it.
	PushAll
	push	cs
	pop	ds
	mov	[CallBacks],0	; Disable callbacks.
	call	StopSound
	PopAll
	mov	[cs:LOOPING],0	   ; No longer looping.

@@YES:
	ret
	endp

Proc	CompleteSound	near
	cmp	[cs:FROMLOOP],1    ; In loop callback?
	je	@@YES
	call	EndLoop 	; don't wait for loop to complete, end it!
@@YES:
@@WT:	cmp	[cs:_voice_status],0	 ; Wait until last sound completed.
	jne	@@WT
	ret
	endp


; This proc can be used for debug messages.  When called, the value of
; AX is written into the DSP shared RAM area.
;IF      DEBUG
;proc    DoutAX  near
;        push    ax
;        push    bx
;        push    es
;
;        mov     bx, [cs:_MsndSeg]
;        mov     es, bx
;         assume  es:SRAM_BANK_0
;
;        mov     bx, [cs:DebugOffset]
;        add     bx, 7060h
;        mov     [es:(word ptr bx)], ax
;        mov     bx, [cs:DebugOffset]
;        add     bx, 2
;        cmp     bx, 1FFh
;        jng     dont_do_it
;        xor     bx, bx
;dont_do_it:
;        mov     [cs:DebugOffset], bx
;        pop     es
;        pop     bx
;        pop     ax
;        assume  es:nothing
;        ret
;        endp
;ENDIF

ORG_INT_ADDX    dd      ?       ; Original IRQ address.

Proc	PlaySound	near
;; CX ->number of bytes in sound sample.
;; ES:BX -> far address of sound sample to be played.
;; DX ->rate to play at.

        push    ax
        push    dx
        mov     [cs:_AudioSeg], es
        mov     [cs:_AudioOff], bx
        mov     [cs:_wNumBytes], cx
        mov     ax, [cs:_MsndSeg]
        mov     es, ax
        assume  es:SRAM_BANK_0

SecondLoad:
        mov     cx, [cs:_wNumBytes]
        mov     dx, [es:wDAPQ_Tail]     ; Tail in DX
        push    dx                      ; Save original tail
        add     dx, DAQD_STRUC_SIZE     ; Advance Tail

        mov     ax, [es:wDAPQ_Size]
        cmp     dx, ax                  ; Time to wrap?
	jbe	NoWrap
        xor     dx, dx                  ; Yes, Wrap the queue

NoWrap:
        cmp     cx, DAP_BUFF_SIZE
	jbe	SizeOK
        mov     cx, DAP_BUFF_SIZE      ; This is bogus, deal with it later

SizeOK:
        cld
        pop     ax                      ; pop the original tail
        mov     si, [cs:_AudioOff]
        mov     bx, offset wDAPQbuff    ; point to data struct

        shl     ax, 1
        add     bx, ax

        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Size], cx
        sub     [cs:_wNumBytes], cx
        mov     ax, [es:(DAQueueDataStruc ptr bx).wDAQD_Start] ; dsp based buffer start addr
        sub     ax, DSP_BASE_ADDR               ; lose his base (rel bank 0)
        shl     ax, 1                           ; now buff start in our domain

        mov     di, ax
        push    ds
        mov     ax, [cs:_AudioSeg]
        mov     ds, ax
        assume  ds:nothing

        shr     cx, 1           ; trying word movs
        rep movsw               ; DS:SI -> ES:DI

        mov     [cs:_AudioOff], si
        mov     [es:wDAPQ_Tail], dx
        pop     ds

; used to check for size, now assuming less than our buffers
        cmp     [cs:_AudioFlag], 1
        je      done_load
        cmp     [cs:_wNumBytes], 0
        je      done_load
        mov     [cs:_AudioFlag], 1
        jmp     SecondLoad

done_load:
        mov     [cs:_AudioFlag], 0
        mov     al, HDEX_PLAY_START
        call    dspEx

        mov     [cs:_voice_status], 1

        pop     dx
        pop     ax
        ret

        endp


Proc    StopSound       near

        mov     al, HDEX_PLAY_STOP
        call    dspEx
	ret
	endp


Proc	DoCallBacks	near
	cmp	[cs:CallBacks],0
	je	@@GOUT
	PushAll 		; Save all registers
	mov	ds,[cs:CallDS]	; Get DS register.
	call	[cs:CallBack]	; far call to application.
	PopAll			; Restore all registers.
@@GOUT: ret
	endp


; MultiSound DSP specific calls

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; dspRead : Read a worde from the DSP (TXM, TXH).
;
; rdesc : If time out occurs before the DSP is ready then the carry flag
;         is set and no value is returned otherwise the carry flag is cleared
;         and the data values are in AX.
;
; comm : The timeout value is very machine dependent.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

proc    dspRead near                             ; word

	push	cx
	push	dx

        mov     dx, [_wPort]              ; get the base port address
	add	dl, HP_ISR		; point at status port

	xor	ah, ah			; outer loop time out value
	xor	cx, cx			; inner loop

dsprd_wait_rxdrf:

	in      al, dx			; fetch status port
	test	al, mask HPISR_RXDF	; wait for Recv Data Register Full
	jnz	dsprd_rxdrf		; yep, got some

	loop	dsprd_wait_rxdrf	; loop till timeout
	dec	ah
	jnz	dsprd_wait_rxdrf	; loop till timeout

	push	es
	pop	es

        stc
	jmp	short dsprd_fini

dsprd_rxdrf:

	add	dl, HP_RXH - HP_ISR	; point at DSP rx high data port
	in	ax, dx			; get data bytes (RXH->AL, RXM->AH)

	clc

dsprd_fini:

	pop	dx
	pop	cx
	ret

        endp


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; dspWrite : Write TXL byte to the DSP.
;
; AL = The byte to be written.
;
; comm : Carry set on exit if it times out.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

proc    dspWrite        near                             ; void (CF = error)

	push	dx
	push	cx

	push	ax			; stack byte to write

        mov     dx, [_wPort]
	add	dl, HP_ISR		; point at int status port

	mov	ah, 10			; outer timeout loop counter
	xor	cx, cx			; inner timeout loop counter
dspwr1:
	in	al, dx			; get current status
	test	al, mask HPISR_TRDY	; tx ready ?
	jnz	dspwr2			; yep, get going

	loop	dspwr1			; loop if not timed out
	dec	ah			; outer loop
	jnz	dspwr1			; keep looping if still not timed-out

	pop	ax			; dump the data

	push	es
	pop	es

	stc				; set carry to show time out
	jmp	short dspwr3

dspwr2:
	pop	ax			; get the data byte
	add	dx, HP_TXL - HP_ISR	; point at TX low
	out	dx, al			; write it
	clc				; no error

dspwr3:
	pop	cx
	pop	dx
	ret

        endp

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

        mov     ax, cs
        mov     ds, ax
        assume  ds:_TEXT

  ; what is our data segment

        mov     dx, [_wPort]
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


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; dspExAux : Write a Aux command / exception byte to the DSP.
;
; AL = The aux command byte to be written.
;
; comm : Carry set on exit if it times out.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

proc    dspExAux        near                             ; void (CF = error)

	cli				;--- no ints till after dsp talk fini

	call	dspWrite		; put aux req # in TXL
	jc	dspexa_9		; trouble, bail

	mov	al, HDEX_AUX_REQ	; request aux command
	call	dspEx			; (cf is in order)
					; ("dspEx" unlocks ints before exit)
dspexa_9:
	sti				;--- ints safe again
        ret

        endp


proc    smaReadWord     near                            ; word

						; passed : BX = addr to read
						; rtn'd  : AX = data read
						; trashed: nothing
	push	ds
        mov     ds, [_MsndSeg]
        assume  ds:SRAM_BANK_0

        mov     ax, [ds:(word ptr bx)]          ; read word from shared mem

	pop	ds
        assume  ds:_TEXT
	ret
        endp


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; dspISR : This function services interrupts from the DSP.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


proc    dspISR  far
	SetSemaphore
        assume  es:nothing, ds:nothing

        push    ax
        push    dx
        push    es              ; Save registers that are used
	push	ds
	push	di
	push	si
	push	cx
	push	bx
	mov	ax,cs
	mov	ds,ax		; Establish data addressability.

	cld				; we must make sure dir is fwd

	; set up local DS & shared memory ES

        push    cs
        pop     ds
        assume  ds:_TEXT                ; our local fixed data area

        cmp     [_bIsrRunning], 0         ; is ISR running ?
	je	isr_not_nested		; nope

        mov     dx, [_wPort]            ; base dsp / host port
	add	dl, HP_RXL		; must read low to clear dsp
	in	al, dx			; now he can write again, hence int again

	; send EOI to both PICs if needed

        mov     dx, DEF_EOI_REG
	mov     al, EOI			; issue EOI to 8259
	out     dx, al

	cmp	dl, 20h			; are we on cascaded controller ?
	je	isr_re_no_cas		; nope, 1st one

	out     20h, al			; send end of int to controller 1

isr_re_no_cas:
	pop	bx
	pop	cx
	pop	si
	pop	di
	pop	ds
	pop	es
        pop     dx
        pop     ax
	sti				;--- "iret" won't do it
	ClearSemaphoreIRET


isr_not_nested:

        mov     es, [_MsndSeg]            ; prep ES
        assume  es:SRAM_BANK_0          ; bank 0 is visible for sure (by design)

        mov     [_bIsrRunning], not 0     ; set ISR running

	; acknowledge the interrupt from the DSP

isr_q_loop:

        mov     bx, [es:wDSPQ_Head]          ; get my current dsp msg ptr
        cmp     bx, [es:wDSPQ_Tail]          ; is the queue empty ?
	je	isr_q_empty		; yep, bye-bye

	inc	bx			; point at next queue slot
        cmp     bx, [es:wDSPQ_Size]          ; time to wrap ?
	jbe	isr_q_ready		; nope, skip

	xor	bx, bx			; point back at start

isr_q_ready:

        xchg    bx, [es:wDSPQ_Head]          ; update new queue head / get prev

        add     bx, [es:wDSPQ_Start]         ; dsp based start
	sub	bx, DSP_BASE_ADDR	; lose his base
	shl	bx, 1			; now in our domain

        mov     ax, [es:(word ptr bx)]  ; get the queue data

        cmp     ah, HIMT_PLAY_DONE
        jne     isr_q_loop

        call    himPlayDone             ; AH has msg type, AL has msg data
	jmp	isr_q_loop		; loop till dsp msg queue is empty

isr_q_empty:

	; allow DSP to generate more interrupts

        mov     dx, [_wPort]              ; base dsp / host port
	add	dl, HP_RXL		; must read low to clear dsp
	in	al, dx			; now he can write again, hence int again

	; send EOI to both PICs if needed

        mov     dx, DEF_EOI_REG
	mov     al, EOI			; issue EOI to 8259
	out     dx, al

	cmp	dl, 20h			; are we on cascaded controller ?
	je	isr_no_cas		; nope, 1st one

	out     20h, al			; send end of int to controller 1

isr_no_cas:

        mov     [_bIsrRunning], 0         ; ISR not running

	pop	bx
	pop	cx
	pop	si
	pop	di
	pop	ds
	pop	es
        pop     dx
        pop     ax

        assume  es:nothing, ds:nothing
	ClearSemaphoreIRET
        endp

proc    himPlayDone     near
        push    es
        push    bx
        push    dx
        push    ax

        mov     bx, [cs:_MsndSeg]
        mov     es, bx
        assume  es:SRAM_BANK_0

        mov     ax, [cs:PENDING]
        cmp     ax, 1
        jne     @@NoPending
;        push    ax
;        mov     ax, 0FFFFh
;        call    DoutAX
;        pop     ax
        mov     [cs:PENDING], 0
        jmp     EndPlay

@@NoPending:
;        push    ax
;        mov     ax, [cs:_wNumBytes]
;        call    DoutAX
;        pop     ax
        mov     cx, [cs:_wNumBytes]     ; get remaining bytes
        cmp     cx, 0
        jne     ContinueCheck
        mov     [cs:_voice_status], 0

;RM     ADDED 7/14/92
;       fixed problem of invoking the callback before the audio has
;       completed.

        cli                     ; no ints untill this is determined
        mov     dx, [es:wDAPQ_Head]
        sti
        cmp     dx, [es:wDAPQ_Tail]
        jne     NoCallBack
        call    CheckCallBack           ; Check callbacks.
NoCallBack:
;****RM

        jmp     EndPlay

ContinueCheck:
        mov     dx, [es:wDAPQ_Tail]     ; Tail in DX
        push    dx                      ; Save original tail
        add     dx, DAQD_STRUC_SIZE     ; Advance Tail

        mov     ax, [es:wDAPQ_Size]
        cmp     dx, ax                  ; Time to wrap?
	jbe	NoWrap3
        xor     dx, dx                  ; Yes, Wrap the queue

NoWrap3:
        cmp     cx, DAP_BUFF_SIZE
	jbe	SizeOK3
        mov     cx, DAP_BUFF_SIZE

SizeOK3:
        cld
        pop     ax

        mov     bx, offset wDAPQbuff    ; point to data struct

        shl     ax, 1
        add     bx, ax

        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Size], cx
        sub     [cs:_wNumBytes], cx
        mov     ax, [es:(DAQueueDataStruc ptr bx).wDAQD_Start] ; dsp based buffer start addr
        sub     ax, DSP_BASE_ADDR               ; lose his base (rel bank 0)
        shl     ax, 1                           ; now buff start in our domain

        mov     di, ax

        push    ds
        mov     ax, [cs:_AudioSeg]
        mov     ds, ax
        assume  ds:nothing
        mov     si, [cs:_AudioOff]

        shr     cx, 1                   ; check carry for odd # of bytes
        rep     movsw
        jnc     StoreAudPtr
        movsb
;        rep movsb                       ; DS:SI -> ES:DI
StoreAudPtr:
        mov     [cs:_AudioOff], si
        mov     [es:wDAPQ_Tail], dx
        pop     ds

        mov     al, HDEX_PLAY_START
        call    dspEx

EndPlay:
        pop     ax
        pop     dx
        pop     bx
        pop     es
        ret
        endp



proc    RestoreHardISR  far
	sti				;--- make sure
        push    dx
        push    ax
        push    cx

        mov     dx, [_wPort]              ; get the base port address
	add	dl, HP_DSPR		; point at DSP reset port
	mov	al, HPDSPRESET_ON	; want to reset him
	out	dx, al			; dsp is now in reset
	mov	cx, 100
	loop	$
	mov	al, HPDSPRESET_OFF
	out	dx, al			; dsp is now not in reset
	loop	$

; force HREQ line low (active), therefore IRQx line high (requesting/disabled)
; which means that IRQx will not change state; from high (req) to high (disabled)

					; make sure int line (HREQ) is active
        mov     dx, [_wPort]              ; base port addr
	add	dl, HP_ICR		; point at int control reg
	in	al, dx			; get it
	or	al, mask HPICR_TREQ	; enable tx empty int so hreq is active
	and	al, not mask HPICR_RREQ	; turn off rx ints so we don't get hosed-down
	out	dx, al			; generate the int / IRQx = HIGH

	mov	al, HPIRQ_NONE		; want to disable ints
	add	dl, HP_IRQM - HP_ICR	; point at interrupt mapping port
	out	dx, al			; irq is now disabled


; disable tx int source on dsp host port

	cli				;--- ints locked

	add	dl, HP_ICR - HP_IRQM	; point at the int control reg
	in	al, dx			; get current state
	and	al, not mask HPICR_TREQ	; turn off tx int generation
	out	dx, al			; hreq now inactive, IRQx would be low


; disable our interrupt on the PIC

        mov     dx, [_wEOIreg]            ; get 8259 base addr
	inc	dx			; point at mask reg
        in	al, dx			; get current mask bits
        mov     ah, [_bIntBit]            ; get irq bit for our hw int
	test	al, ah			; is irq already disabled ?
	jnz	disable3		; yep, skip "out" (as per MicroSoft)

        or	al, ah			; make a new mask w/ our irq bit
        out	dx, al			; disable our int
disable3:

	sti				;--- ints ok again

; restore the previous interrupt vector

        mov     al, [_bTrueInt]           ; get the true x86 int level we use
        push	ds
        mov     ds, [OldIntSeg]
        mov     dx, [OldIntOff]

        assume  ds:nothing
        mov	ah, 25H			; set interrupt vector
        int	21H			; reset the old vector
        pop	ds
        assume  ds:_TEXT

disable9:
        pop     cx
        pop     ax
        pop     dx

        ret
        endp



LABEL	SUICIDE byte		;; Where to delete ourselves from memory
hard    db      "MultiSound not detected.",13,10,'$'
msg0A   db      "MultiSound failed in reset",13,10,'$'
msg0B   db      "MultiSound failed interrupt installation",13,10,'$'

msg0    db      "MultiSound DIGPAK Driver"
	db	" - Copyright (c) 1992, The Audio Solution 3.20",13,10
	db	" Portions Copyright, Turtle Beach Systems",13,10,'$'

msg1	db	"The Sound Driver is already resident.",13,10,'$'
msg1a   db      "The Sound Driver is resident, through MIDPAK.",13,10,'$'
msg1b   db      "A Sound Driver cannot be loaded on top of MIDPAK.  Unload MIDPAK first.",13,10,'$'
msg2	db	"Unable to install Sound Driver interupt vector",13,10,'$'
msg3    db      "Invalid command line",13,10,'$'
msg4	db	"Sound Driver isn't in memory",13,10,'$'
msg5	db	"Sound Driver unloaded",13,10,'$'
msg5a	db	"Sound Driver can't be unloaded, unload MIDPAK first.",13,10,'$'
param   dw      4 dup(?)        ;; Used for parameter passing.
Installed	dw	0

        _bMemMap        db HPMEM_D000
        _bWaitState     db HPWAITSTATE_0
        _bBitMode       db HPBITMODE_16
        _bIntMap        db HPIRQ_10

DSP_INIT_SIZE   equ     512
wDspInitSize	dw	DSP_INIT_SIZE

label   bDspInitStart   byte
        include "msdspini.asm"

label   bDspPermStart   byte
label   wDspPermWords   word
        include "msdsperm.asm"
wDspPermSize	dw	$-wDspPermWords


Proc    LoadSound near
	mov	ax,cs			;;
	mov	ds,ax			;; establish data segment
        assume  ds:_TEXT
;IF      ISEXE
;        sub     ax,10h                  ;;
;ENDIF
	mov	es,ax			;; point ES to PSP

	call	CheckIn
	mov	[Installed],ax		;; Save in installed flag.
	call	ParseCommandLine	;; Build a command line.
	cmp	[_argc],0
	je	NoArg
	cmp	[_argc],1
	jne	@@BC
	mov	bx,[_argv]
	mov	al,[bx]
	cmp	al,'u'
	je	ULOAD
	cmp	al,'U'
	je	ULOAD
@@BC:
	Message msg3			;; Invalid command line
        DOSTerminate

ULOAD:	mov	ax,[Installed]
	or	ax,ax
	jnz	DOU
	Message msg4			;; wasn't loaded.
	DOSTerminate			;; Terminate with message.
DOU:	cmp	ax,2
	jne	@@OKU
	Message msg5a
	DOSTerminate
@@OKU:
	CALLF	DeInstallInterupt
	Message msg5			;; Display message
	DOSTerminate			;; terminate

NoArg:	or	ax,ax			;; Already loaded?
	jz	@@DOLO			;; no->load it.
	cmp	ax,2
	jne	@@TEMP
	Message msg1a
	DOSTerminate
@@TEMP: cmp	ax,3
	jne	@@TEMPA
	jmp short @@DOLO
	Message msg1b
	DOSTerminate
@@TEMPA:
	Message msg1			;; message
	DOSTerminate			;;
@@DOLO: CALLF	InstallInterupt
	or	ax,ax			;; Was there an error?
	jz	@@HOP1			;; no->continue
        cmp     ax, 1
        jne     @@Not_a_one
        Message msg2                    ;; display the error message
        Message msg0A    ; Hardware error message if there is one.
        jmp     @@Terminer
@@Not_a_one:
        Message msg2                    ;; display the error message
        Message msg0B    ; Hardware error message if there is one.

@@Terminer:
	DOSTerminate			;; exit to dos
@@HOP1:
;;; The Kernel is now installed.
;;; Announce the Kernel's presence.
	Message msg0
        DosTSR  SUICIDE         ;; Terminate ourselves bud.
	endp

Proc	InstallInterupt 	far
	IN_TSR

        call    HardwareInit    ;; Initialize hardware.
	or	ax,ax		;; Error initializing hardware?
	jnz	@@OUT

	mov	[param],KINT		;; The interupt kernel is going into.
	mov	[param+2],offset SoundInterupt ;; offset of interupt routine
	mov	[param+4],cs		;; Our code segment.
	PushEA	param			;; push the address of the parameter list
	call	InstallInt		;; Install the interupt.
	add	sp,2			;; clean up stack
@@OUT:

	OUT_TSR
	ret
	endp

Proc	DeInstallInterupt	far
	IN_TSR
	mov	[param],KINT		;; Interupt requested to be unloaded.
	PushEA	param			;; pass parameter.
	call	UnLoad			;; Unload it
	add	sp,2			;; clean up stack
	OUT_TSR
	ret
	endp


Proc	CheckIn near
	push	ds		; Save ds register.
	push	si

	mov	si,66h*4h	; get vector number
        xor     ax,ax           ; zero
        mov     ds,ax           ; point it there
	lds	si,[ds:si]	; get address of interupt vector
        or      si,si           ; zero?
        jz      @@CIOUT         ; exit if zero
        sub     si,6            ; point back to identifier

	cmp	[word si],'IM'  ; Midi driver?
	jne	@@NEX
	cmp	[word si+2],'ID'  ; full midi driver identity string?
	jne	@@NEX

;; Ok, a MIDI driver is loaded at this address.
	mov	ax,701h 	; Digitized Sound capabilities request.
	int	66h		; Request.
	or	ax,ax		; digitized sound driver available?
	jnz	@@INMID 	; yes, report that to the caller.
	mov	ax,3		; Not available, but mid pak is in!
	jmp short @@EXT 	; exit with return code.
@@INMID:
	mov	ax,2		; Sound driver resident, through MIDPAK.
	jmp short @@EXT
@@NEX:
	cmp	[word si],454Bh ; equal?
        jne     @@CIOUT         ; exit if not equal
	cmp	[word si+2],4E52h    ; equal?
        jne     @@CIOUT
@@OK:	mov	ax,1
@@EXT:
	pop	si
	pop	ds
	ret
@@CIOUT: xor	ax,ax		; Zero return code.
	jmp short @@EXT
	endp



Proc	InstallINT near
	ARG	DATA:WORD
;; Usage: IntallINT(&parms)
;; offset 0: interupt
;;        2: offset of interupt code
;;        4: segment of interupt code
	PENTER	0
	PushCREGS

	mov	bx,[DATA]	; Get address of parameter table
        mov     ax,[bx]         ; get the interupt vector.
        mov     di,ax           ; save interupt vector into DI as well
        mov     si,[bx+2]       ; get offset
        mov     ds,[bx+4]       ; get segment.
        mov     ah,35h          ; Get interupt vector
        int     21h             ; Do DOS call to get vector.
	mov	[ds:si-10],bx	; Save the old offset.
	mov	[ds:si-8],es	; Save the old segment
        cld
        xor     ax,ax
        mov     es,ax
	ShiftL	di,2		;
        mov     ax,si           ; get offset.
        cli
        stosw
        mov     ax,ds           ; code segment
        stosw                   ; store it.
        sti
        xor     ax,ax           ; Success

	PopCREGS
	PLEAVE
	ret
	endp

Proc	UnLoad near
	ARG	DATA:WORD
;; Usage: UnLoad(&vector)
;; Returns: AX = 0 success
;           AX nonzero, couldn't unload interupt vector.
	PENTER	0
	PushCREGS

	mov	ax,68Fh ; Stop sound playback!
	int	KINT	; Invoke interrupt.
	WaitSound
	mov	ax,692h ; Deinstall hardware vectors.
	int	KINT

	mov	bx,[DATA]	; get address of interupt vector
        mov     bx,[bx]         ; get the interupt vector.
        mov     dx,bx           ; put it into DX as well
	ShiftL	bx,2		;
        xor     ax,ax
        mov     ds,ax           ; Segment zero
	lds	si,[ds:bx]	; get address of interupt vector
        or      si,si           ; zero?
        jz      @@UOUT          ; exit if zero
	cmp	[WORD ds:si-2],524Bh ;'KR' Is this a kernel installed interupt?
        push    ds              ; save DS
        mov     ax,dx           ; Get interupt vector.
        mov     ah,25h          ; Do DOS 25h set interupt vector.
	mov	dx,[ds:si-10]	 ; get old offset
	mov	ds,[ds:si-8]	; get old segment
        int     21h             ; set interupt vector.
        pop     ax              ; get back segment of program.
;IF      ISEXE
;        sub     ax,10h          ; point to program's PSP
;ENDIF
        mov     es,ax
	push	es
	mov	es,[es:2Ch]	; Environment space.
	mov	ah,49h
	int	21h		; Free it up.
	pop	es
        mov     ah,49h          ; free memory.
        int     21h             ; free up the memory used by us.

@@EXIT: PopCREGS
	PLEAVE
        ret
@@UOUT: mov     ax,1
        jmp short @@EXIT
	endp

;; This procedure parses the command line and builds an array of
;; pointers to each argument.  Arguments are seperated by space's.
;; these spaces get replaced by zero bytes.
_argc	 dw	 0	 ; The argument count
_argv	dw	16 dup(0) ; Up to 16 arguments.
command db	128 dup(?)
Proc	ParseCommandLine	near
	mov	[_argc],0
	cmp	[byte es:80h],2
	jb	@@END
	xor	cx,cx
	mov	cl,[es:80h]	; Get length.
	SwapSegs
	dec	cx		; Less one
	lea	di,[command]
	mov	si,82h
	rep	movsb
	push	cs
	pop	ds
	lea	di,[_argv]	; Argument list.
	lea	si,[command]	; Start address.
@@SET:	inc	[_argc] 	; Increment argument counter.
	mov	ax,si		; Base argument addres.
	stosw
@@NEX:	lodsb			; Get characters until we hit space of eol
	cmp	al,32
	jne	@@NEX2
	mov	[byte ds:si-1],0 ; Turn space into a zero byte.
	jmp short @@SET
@@NEX2:
	cmp	al,13
	je	@@END1
	or	al,al
	jnz	@@NEX		; Keep skipping to next arg.
@@END1: mov	[byte ds:si-1],0	; Zero byte terminate last arg
@@END:
	ret
	endp

;;************************************************************************
;; Unique harware init code.
;;************************************************************************
Proc    HardwareInit    far

        push    es
        mov     ax, [_MsndSeg]             ; point ES to our SMA d000
        mov     es,ax
        assume  es:SRAM_BANK_0
        xor     bx,bx

;         int     1       ; debug breakpoint
        mov     [_bMemMap], HPMEM_B000
        cmp     ax, 0B000h
        je      GotTheMemReg

        mov     [_bMemMap], HPMEM_C800
        cmp     ax, 0C800h
        je      GotTheMemReg

        mov     [_bMemMap], HPMEM_D000
        cmp     ax, 0D000h
        je      GotTheMemReg

        mov     [_bMemMap], HPMEM_D800
        cmp     ax, 0D800h
        je      GotTheMemReg

        mov     [_bMemMap], HPMEM_E000
        cmp     ax, 0E000h
        je      GotTheMemReg

        mov     [_bMemMap], HPMEM_E800
        cmp     ax, 0E800h
        je      GotTheMemReg

; from here to there is new stuff
        jmp     initERROR


GotTheMemReg:
        mov     ax, [_intr_num]             ; get the interrupt number

        mov     [_bIntMap], HPIRQ_5
        cmp     ax, 05h
        je      GotTheIntReg

        mov     [_bIntMap], HPIRQ_7
        cmp     ax, 07h
        je      GotTheIntReg

        mov     [_bIntMap], HPIRQ_9
        cmp     ax, 09h
        je      GotTheIntReg

        mov     [_bIntMap], HPIRQ_10
        cmp     ax, 0Ah
        je      GotTheIntReg

        mov     [_bIntMap], HPIRQ_11
        cmp     ax, 0Bh
        je      GotTheIntReg

        mov     [_bIntMap], HPIRQ_12
        cmp     ax, 0Ch
        je      GotTheIntReg

        mov     [_bIntMap], HPIRQ_15
        cmp     ax, 0Fh
        je      GotTheIntReg

        jmp     initERROR

GotTheIntReg:
; here is there

        mov     dx, [_wPort]              ; get the base port address
        add	dl, HP_DSPR		; point at DSP reset port
	mov	al, HPDSPRESET_ON	; want to reset him
        out	dx, al			; dsp is now in reset (he can't hurt us)

        add     dl, (HP_MEMM - HP_DSPR) ; point at memory mapping port
        mov     al, [_bMemMap]          ; get desired shared memory segment map
	out	dx, al			; memory is now mapped

	add	dl, HP_BLKS - HP_MEMM	; point at bank select port
	mov	al, HPBLKSEL_0		; want to start w/ bank #0
	out	dx, al			; bank 0 is now selected

        add     dl, (HP_WAIT - HP_BLKS) ; point at wait state select port
        mov     al, [_bWaitState]       ; get desired wait states
	out	dx, al			; wait states are now as requested

        add     dl, (HP_BITM - HP_WAIT) ; point at bit mode select port
        mov     al, [_bBitMode]         ; get desired bit mode: 8 / 16
	mov	cl, 3			; move D3 into D0 position
	shr	al, cl			; 8 => 1, 16 => 0 (for D0)
	out	dx, al			; bus mode is now as requested


        cld                                     ; make sure
        mov     dx, [_wPort]                    ; Zero all the SMA
	add	dl, HP_BLKS
	mov	al, HPBLKSEL_1			; bank 1
	out	dx, al
	xor	di, di
	mov	cx, SRAM_BANK_SIZE/2
	xor	ax, ax
	rep	stosw

	mov	al, HPBLKSEL_0			; bank 0
	out	dx, al
	xor	di, di
	mov	cx, SRAM_BANK_SIZE/2
	rep	stosw

	push	ds				; prep to init sma - bank 0
        mov     ax, [_MsndSeg]
        mov     ds, ax
        assume  ds:SRAM_BANK_0

; init digital audio playback job queue

	mov	ax, (offset wDAPQbuff) + (DSP_BASE_ADDR SHL 1)
	shr	ax, 1
        mov     [wDAPQ_Start], ax
        mov     [wDAPQ_Size], DAPQ_BUFF_SIZE-1
;*z*	mov	wDAPQ_Head, 0
;*z*	mov	wDAPQ_Tail, 0


; init digital audio record job queue

	mov	ax, (offset wDARQbuff) + (DSP_BASE_ADDR SHL 1)
	shr	ax, 1
        mov     [wDARQ_Start], ax
        mov     [wDARQ_Size], DARQ_BUFF_SIZE-1
;*z*	mov	wDARQ_Head, 0
;*z*	mov	wDARQ_Tail, 0


; init midi output data queue

	mov	ax, (offset wMODQbuff) + (DSP_BASE_ADDR SHL 1)
	shr	ax, 1
        mov     [wMODQ_Start], ax
        mov     [wMODQ_Size], MODQ_BUFF_SIZE-1
;*z*	mov	wMODQ_Head, 0
;*z*	mov	wMODQ_Tail, 0


; init midi input data queue

	mov	ax, (offset wMIDQbuff) + (DSP_BASE_ADDR SHL 1)
	shr	ax, 1
        mov     [wMIDQ_Start], ax
        mov     [wMIDQ_Size], MIDQ_BUFF_SIZE-1
;*z*	mov	wMIDQ_Head, 0
;*z*	mov	wMIDQ_Tail, 0


; init dsp->host message queue

        mov     ax, (offset wDSPQbuff) + (DSP_BASE_ADDR SHL 1)
	shr	ax, 1
        mov     [wDSPQ_Start], ax
        mov     [wDSPQ_Size], DSPQ_BUFF_SIZE-1
;*z*	mov	wDSPQ_Head, 0
;*z*	mov	wDSPQ_Tail, 0


; init DA play job queue data

	mov	bx, offset wDAPQbuff
        mov     ax, offset bDAPbuff1Start
        shr     ax, 1
        add     ax, DSP_BASE_ADDR
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Start], ax
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Size], DAP_BUFF_1_SIZE                ; buffer start & size
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Format], 1                            ; PCM data format
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_SampleSize], 8                        ; 8 bit
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Channels], 1                          ; mono
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_SampleRate], 11025                    ; 44.1kHz
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_IntMsg], (HIMT_PLAY_DONE*100h) + 0    ; int time msg
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Flags], 1                             ; flag buffer #1

	add	bx, DAQD_STRUC_SIZE*2
        mov     ax, offset bDAPbuff2Start
        shr     ax, 1
        add     ax, DSP_BASE_ADDR
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Start], ax
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Size], DAP_BUFF_2_SIZE                ; buffer start & size
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Format], 1                            ; PCM data format
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_SampleSize], 8                        ; 8 bit
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Channels], 1                          ; mono
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_SampleRate], 11025                    ; 44.1kHz
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_IntMsg], (HIMT_PLAY_DONE*100h) + 1    ; int msg
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Flags], 2                             ; flag buffer #2

	add	bx, DAQD_STRUC_SIZE*2
        mov     ax, offset bDAPbuff3Start
        shr     ax, 1
        add     ax, DSP_BASE_ADDR
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Start], ax
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Size], DAP_BUFF_3_SIZE                ; buffer start & size
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Format], 1                            ; PCM data format
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_SampleSize], 8                        ; 8 bit
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Channels], 1                          ; mono
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_SampleRate], 11025                    ; 44.1kHz
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_IntMsg], (HIMT_PLAY_DONE*100h) + 2    ; int msg
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Flags], 3                             ; flag buffer #3

; init DA record job queue data

	mov	bx, offset wDARQbuff
        mov     ax, offset bDARbuff1Start
        shr     ax, 1
        add     ax, (DSP_BASE_ADDR+DSP_BANK_BASE)
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Start], ax
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Size], DAR_BUFF_1_SIZE                ; buffer start & size
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Format], 1                            ; PCM data format
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_SampleSize], 16                       ; 16 bit
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Channels], 2                          ; stereo
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_SampleRate], 44100                    ; 44.1kHz
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_IntMsg], (HIMT_RECORD_DONE*100h) + 0  ; int time msg
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Flags], 1                             ; flag buffer #1

	add	bx, DAQD_STRUC_SIZE*2
        mov     ax, offset bDARbuff2Start
        shr     ax, 1
        add     ax, (DSP_BASE_ADDR+DSP_BANK_BASE)
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Start], ax
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Size], DAR_BUFF_2_SIZE                ; buffer start & size
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Format], 1                            ; PCM data format
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_SampleSize], 16                       ; 16 bit
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Channels], 2                          ; stereo
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_SampleRate], 44100                    ; 44.1kHz
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_IntMsg], (HIMT_RECORD_DONE*100h) + 1  ; int msg
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Flags], 2                             ; flag buffer #2

	add	bx, DAQD_STRUC_SIZE*2
        mov     ax, offset bDARbuff3Start
        shr     ax, 1
        add     ax, (DSP_BASE_ADDR+DSP_BANK_BASE)
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Start], ax
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Size], DAR_BUFF_3_SIZE                ; buffer start & size
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Format], 1                            ; PCM data format
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_SampleSize], 16                       ; 16 bit
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Channels], 2                          ; stereo
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_SampleRate], 44100                    ; 44.1kHz
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_IntMsg], (HIMT_RECORD_DONE*100h) + 2  ; int msg
        mov     [es:(DAQueueDataStruc ptr bx).wDAQD_Flags], 3                             ; flag buffer #3


; init common data

;*z*	mov	wCurrPlayBytes,   0		; # of bytes played in current play job
;*z*	mov	wCurrRecordBytes, 0		; # of bytes recorded in current record job
        mov     [es:wCurrPlayVolLeft],  1FFFh        ; current play output volume - left
        mov     [es:wCurrPlayVolRight], 1FFFh        ; current play output volume - right
;*z*	mov	wCurrInVolLeft,    0000h	; current in mixer volume - left
;*z*	mov	wCurrInVolRight,   0000h	; current in mixer volume - right
        mov     [wCurrMIDIVolLeft],  1FFFh        ; current MIDI output volume - left
        mov     [wCurrMIDIVolRight], 1FFFh        ; current MIDI output volume - right
;*z*	mov	word ptr dwCurrPlayPitch+0, 0000h ; current play pitch: MSW = int, LSW = frac
        mov     [word ptr dwCurrPlayPitch+2], 0001h ; current play pitch: MSW = int, LSW = frac
;*z*	mov	word ptr dwCurrPlayRate+0,  0000h ; current play rate: MSW = int, LSW = frac
        mov     [word ptr dwCurrPlayRate],  0001h ; current play rate: MSW = int, LSW = frac
;*z*	mov	wCurrMIDIIOPatch, 0		; current MIDI I/O patch setup

        mov     [wCurrPlayFormat],           1    ; current playback data format
        mov     [wCurrPlaySampleSize],       8    ;    "       "     sample size in bits
        mov     [wCurrPlayChannels],         1    ;    "       "     # of channels (1=mono)
        mov     [wCurrPlaySampleRate],   11025    ;    "       "     data output rate

        mov     [wCurrRecordFormat],         1    ; current recorder data format
        mov     [wCurrRecordSampleSize],     8    ;    "       "     sample size in bits
        mov     [wCurrRecordChannels],       1    ;    "       "     # of channels (1=mono)
        mov     [wCurrRecordSampleRate], 11025    ;    "       "     data output rate

;*z*	mov	wCurrDSPStatusFlags,  0		; current DSP  status flags:
;*z*	mov	wCurrHostStatusFlags, 0		; current Host status flags:

;*z*	mov	wCurrInputTagBits, 0		; current A/D clipping tag bits
;*z*	mov	wCurrLeftPeak,  0		; current left  peak
;*z*	mov	wCurrRightPeak, 0		; current right peak

;*z*	mov	wExtDSPbits,  0			; external DSP  hw bits
;*z*	mov	wExtHostbits, 0			; external Host hw bits

;*z*	mov	bInPotPosLeft,  0		; input  pot left  setting (0->FF)
;*z*	mov	bInPotPosRight,	0		; input  pot right setting (0->FF)

;*z*	mov	bAuxPotPosLeft,  0		; aux/CD pot left  setting (0->FF)
;*z*	mov	bAuxPotPosRight, 0		; aux/CD pot right setting (0->FF)

;*z*	mov	bInPotMaxLeft,  0		; input  pot left  setting (0->FF)
;*z*	mov	bInPotMaxRight, 0		; input  pot right setting (0->FF)

;*z*	mov	bAuxPotMaxLeft,  0		; aux/CD pot left  setting (0->FF)
;*z*	mov	bAuxPotMaxRight, 0		; aux/CD pot right setting (0->FF)

;*z*	mov	bInPotMaxMethod,  0		;*nu* input  pot max method
;*z*	mov	bAuxPotMaxMethod, 0		;*nu* aux/CD pot max method
						;*nu* 0=off, 1=var, 2=2Vrms, 3=0.5Vrms, 4=fut.

;*z*	mov	wCurrMastVolLeft,  0000h	; master left  mixer volume scalar
;*z*	mov	wCurrMastVolRight, 0000h	; master right mixer volume scalar

        mov     [wCalFreqAtoD], 44100             ; calibrate freq for a/d

;*z*	mov	wCurrAuxVolLeft,  0000h		; aux left  mixer volume
;*z*	mov	wCurrAuxVolRight, 0000h		; aux right mixer volume

        pop     ds
        pop     es

        assume  ds:_TEXT, es:_TEXT

        call    dspReset
        push    bx
        mov     bx, 1
        cmp     ax, 0
        jne     initERROR
        call    InitHardInt
        mov     bx, 2
        cmp     ax, 0
        je      InitDone

initERROR:
        mov     ax,bx                     ; DSP reset failed
        pop     bx
        ret

InitDone:
        xor     ax, ax
        pop     bx
        ret

        endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; void dspReset : Reset the DSP, upload new code, etc.
;
; rdesc: If the reset is completed successfully then the carry
;       flag is cleared and 0 is returned in AX.  If an error
;       occurs, carry is set and an error code is returned in AX.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

proc    dspReset        far                      ; assumed: bank 0 is selected

	cld				; make sure

;	reset DSP
					; the dsp is already in reset
					;  from the call to smaAllocate
        mov     dx, [_wPort]              ; get the base port address
	add	dl, HP_DSPR		; point at DSP reset port
	mov	al, HPDSPRESET_ON	; want to reset him
	out	dx, al			; dsp is now in reset


;	move relocatable dsp code into shared ram
        push    es                      ; set es to board
        mov     ax, [_MsndSeg]
        mov     es, ax
        assume  es:SRAM_BANK_0

	push	ds
	mov	ax, cs
	mov	ds, ax
        assume  ds:_TEXT                ; dsp code is in "init" code seg

        lea     si, [bDspPermStart]       ; ds:si points to front of src buffer
	xor	di, di			; dest sma (4000h rel. to dsp addr scheme)
        mov     cx, [wDspPermSize]        ; put the # bytes to move in counter
	jcxz	dsprst_1		; none to move
	push	cx			; save for compare
	push	di			; (dito)
	push	si			; (dito)
	rep	movsb			; slam them in (slow, but easy)
	pop	si			; ds:si points to front of buffer 1
	pop	di			; es:di points to front of buffer 2
	pop	cx			; put the # bytes to cmpr in counter
	repe	cmpsb			; check integrity of move
	je	dsprst_1		; no problem

	pop	ds
	jmp	dsprst_err

dsprst_1:
	pop	ds
        assume  ds:_TEXT

;	reset proteus / time boot-up
;       Don't reset the proteus, it will trash the presets

	add	dl, HP_PROR - HP_DSPR	; point at Proteus reset port
;        mov     al, HPPRORESET_ON       ; want to reset him
;        out     dx, al                  ; Proteus is now in reset

;        mov     cx, 100                 ; wait for reset to take hold
;        loop    $

;        mov     al, HPPRORESET_OFF      ; let the midi fly
;        out     dx, al                  ; Proteus is now out-of-reset

;	do board & chip level junk

        mov     cx, 8                   ; 8 bits in ext regs
        xor     ah, ah                  ; clean slate to start
	add	dl, HPR_BLRC - HP_PROR	; point at 1st ext host bit

dsprst_tc_loop:
	in	al, dx			; get the bit
	shr	al, 1			; put it in the CF
	rcl	ah, 1			; put it in next bit pos
	inc	dx			; next i/o
	loop	dsprst_tc_loop		; loop till done

        mov     [bExtHostBits], ah        ; update common memory

	sub	dx, 8			; point back at R/C bit

;	time board level

dsprst_bl_on:				; wait for r/c network to discharge
	in	al, dx			; get port state
	test	al, 1			; r/c bit still on ?
	loopnz	dsprst_bl_on		; yep - loop
	jnz	dsprst_bl_rdy		; must be a classic (bli = 0)

	loop	$			; drain out the rest
	loop	$			; (this loop is smaller)

	cli				;--- no ints till timing over

	call	SnapTimer		; get adjustment reference ticks
	mov	bx, ax			; save them

	in	al, dx			; do the "in" thing
	mov	bl, al			; (dito)

	call	SnapTimer		; get "in" ticks
	mov	si, ax			; save them
	call	SnapTimer		; get 2nd ref ticks

	sub	bx, si			; compute "in" time
	sub	si, ax			; compute single snap-shot time
	add	si, bx

	mov	al, HPDSPRESET_OFF	; let the cap charge begin
	add	dl, HP_DSPR - HPR_BLRC	; point at dsp reset port
	out	dx, al			; dsp is now out-of-reset
	add	dl, HPR_BLRC - HP_DSPR	; point at board level r/c bit

	call	SnapTimer		; get loop reference ticks
	mov	di, ax			; save count

dsprst_bl_off:
	in	al, dx			; get r/c bit
	mov	bl, al			; save state

	call	SnapTimer		; get current ticks
	mov	cx, di			; get ref ticks
	sub	cx, ax			; compute elapsed ticks
	cmp	cx, (0FFFFh - 1000)	; close to the edge ?
	jae	dsprst_bl_rdy		; yep, must be a classic (bli = 0)

	test	bl, 1			; did voltage trip yet ?
	jz	dsprst_bl_off		; nope, keep looping

dsprst_bl_calc:
;***	sub	cx, si			; adjust elasped ticks

	mov	al, 1			; assume level 1
	cmp	cx, 20			; short discharge
	jbe	dsprst_bl_rdy		; yep, too short (classic)

	cmp	cx, 607			; board level 1 (1st prod) ?
	jbe	dsprst_bl_save		; yep, save it

	inc	al			; must be level 2

dsprst_bl_save:
        mov     [bBoardLevel], al         ; save board level index

dsprst_bl_rdy:

	sti				;--- ints ok again
	add	dl, HP_DSPR - HPR_BLRC	; point at dsp reset port
	mov	al, HPDSPRESET_OFF	; make sure dsp is running
	out	dx, al			; dsp is now out-of-reset (for sure)
					; (really he's waiting for the upload)

;	wait for DSP to settle down after lifting reset

	mov	cx, 100
	loop	$			; zzzzzzzzzzzzz

	add	dl, HP_ISR - HP_DSPR	; point at host port int status reg

dsprst_wait_loop:
	in	al, dx			; read current isr contents
	cmp	al, 06h			; this MUST be the default contents
	loopne	dsprst_wait_loop	; wait till it is
	jne	dsprst_err		; timed-out


; force HREQ line low (active), therefore IRQx line high (requesting/disabled)
; which means that IRQx will not change state; from high (req) to high (disabled)
; and also will not generate an int from high (disabled) to low (enabled)

        add     dl, HP_ICR - HP_ISR     ; point at int control reg
	in	al, dx			; get it
	or	al, mask HPICR_TREQ	; enable tx empty int so hreq is active
	and	al, not mask HPICR_RREQ	; turn off rx ints so we don't get hosed-down
	out	dx, al			; now hreq is active
        mov     al, [_bIntMap]          ; get requested IRQ # map
	add	dl, HP_IRQM - HP_ICR	; point at interrupt mapping port
	out	dx, al			; IRQ # is now set (and IRQx is high, but ok)

	add	dl, HP_ICR - HP_IRQM	; point at the int control reg
	in	al, dx			; get current state
	and	al, not mask HPICR_TREQ	; turn off tx int generation
	out	dx, al			; hreq now inactive, IRQx is low

	add	dl, HP_ISR - HP_ICR	; point at host port int status reg

	push	ds
	mov	ax, cs
	mov	ds, ax
        assume  ds:_TEXT                ; dsp code is in "init" code seg

        lea     si, [bDspInitStart]       ; ds:si points to front of buffer
        mov     cx, [wDspInitSize]        ; put the max words in loop counter

dsprst_mov_data:
	xor	ah, ah			; timeout counter

dsprst_wait_txdre:
        in      al, dx			; fetch status port
	test	al, mask HPISR_TXDE	; wait for transmit Data Register Empty
        jz      dsprst_txdrne		; oops, it's not

	add	dl, HP_TXH - HP_ISR	; get the port to send the data to

        cld                             ; clear direction
        lodsb
        out     dx, al
        inc     dl
        lodsb
        out     dx, al
        inc     dl
        lodsb
        out     dx, al

;        outsb                              ; send high byte
;        inc     dl                      ; point to mid byte
;        outsb                           ; send mid byte
;        inc     dl                      ; point to low byte
;        outsb                           ; send low byte
        add     dl, HP_ISR - HP_TXL     ; restore status port addr
	loop	dsprst_mov_data		; keep going till all data moved up

	pop	ds
        assume  ds:_TEXT                ; return to fixed data seg

	add	dl, HP_RXL - HP_ISR	; point at rx data - low byte
	in	al, dx			; clear rx data port(s)

	add	dl, HP_CVR - HP_RXL	;* point at cmd vect reg
	in	al, dx			;* fetch value

dsprst_2:				; wait for dsp to give us a green light
	mov	ax, 50			; outer loop count (was 10)
dsprst_4:
        cmp     [es:(word ptr 0000h)], 0  ; is he ready to go yet ?
	je	dsprst_6		; he is ready!
	loop	dsprst_4		; not ready, then wait

	dec	ax			; outer loop expired ?
	jnz	dsprst_4		; nope, keep going

	jmp	short dsprst_hosed_err	; dsp is hosed-down

dsprst_6:
        pop     es
        assume  es:nothing
        xor	ax, ax			; success (cf=0)
        ret

dsprst_txdrne:

        dec     ah                      ; one less wait time, keep waiting ?
	jnz	dsprst_wait_txdre	; yep, back in there

	pop	ds
        assume  ds:_TEXT                ; return to fixed data seg

dsprst_err:
        pop     es
        assume  es:_TEXT
        inc     ax
	stc				; failure (cf=1)
	ret

dsprst_hosed_err:
	jmp	dsprst_err

        endp


; Snap timer proc used for DSP init timing issues

proc    SnapTimer       near            ; rtn'd  : AX = timer ticks

	xor	al, al
	out	43h, al			; freeze timer
	jmp	$+2
	in	al, 40h			; read it
	mov	ah, al
	in	al, 40h			; read & thaw it
	xchg	al, ah
        ret

        endp


proc    InitHardInt     far

; hook the interrupt we want to use
        xor     ax, ax
        mov     ax, [_intr_num]       ; get the true x86 int level we use
        cmp     al, 7
        jg      SecondPIC

        add     al, 8                   ; we are on the first PIC
        mov     [_bTrueInt], al
        mov     [_wEOIreg], PIC1
        sub     al, 8

        mov     cl, al
        mov     al, 1
        shl     al, cl
        mov     [_bIntBit], al

        jmp     Proceed

SecondPIC:
        add     al, 68h
        mov     [_bTrueInt], al
        mov     [_wEOIreg], PIC2
        sub     al, 68h

        sub     al, 8
        mov     cl, al
        mov     al, 1
        shl     al, cl
        mov     [_bIntBit], al
Proceed:
        push    bx

        mov     ah, 35H                 ; get interrupt vector
        int	21H			; get the current vector in ES:BX
        mov     [OldIntSeg], es
        mov     [OldIntOff], bx           ; save the old vector

        pop     bx

; set vector to our isr

        mov     al, [_bTrueInt]           ; get the true x86 int level we use
        push    ds
        push    cs
        pop     ds
        assume  ds:_TEXT

        mov     dx, offset dspISR
        mov	ah, 25H			; set interrupt vector
        int	21h			; set the new vector
        pop     ds                      ; Why was this here
        assume  ds:_TEXT

; enable our irq on the proper PIC

	cli				;--- ints locked
        mov     dx, [_wEOIreg]            ; get 8259 base addr
	inc	dx			; point at mask reg
	in	al, dx			; get current mask bits

;        bIntBit = (BYTE)(1 << bInt);
;        is bintbit correct?
;        mov     ah, 1
;        mov     cl, [_intr_num]
;        sub     cl, 8
;        shl     ah, cl
        mov     ah, [_bIntBit]            ; get irq bit for our hw int

	test	al, ah			; is irq already enabled ?
	jz	enable3			; yep, skip "out" (as per MicroSoft)

	not	ah			; need a zero to enable int
	and	al, ah			; make a new mask
	out	dx, al			; enable our int
enable3:

; tell dsp to int us on rx data

        mov     dx, [_wPort]              ; base port addr
	add	dl, HP_ICR		; get the the int control reg
	in	al, dx
	or      al, mask HPICR_RREQ	; enable the rx int
	out	dx, al

	sti				;--- ints ok again

enab9:
	xor	ax, ax			; no error
enab99:

        ret
        endp

        ENDS

	end	START

