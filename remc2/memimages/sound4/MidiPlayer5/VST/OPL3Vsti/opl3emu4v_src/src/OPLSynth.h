// ==============================================================================
//
// Copyright (c) 1996-2000 Microsoft Corporation.  All rights reserved.
//
// ==============================================================================
#include "stdafx.h"
#ifndef OPL3SYNTH_H
#define OPL3SYNTH_H
#include "opl.h"
#include "opl3.h"
typedef unsigned char	BYTE;
typedef unsigned short  WORD;
typedef unsigned long	DWORD,ULONG;
typedef signed long		LONG;

#define NUMPATCHES                      (256)
#define DRUMCHANNEL                     (9)     /* midi channel 10 */


#define BCODE
#define fEnabled TRUE



#define AsULMUL(a, b) ((DWORD)((DWORD)(a) * (DWORD)(b)))
#define AsLSHL(a, b) ((DWORD)((DWORD)(a) << (DWORD)(b)))
#define AsULSHR(a, b) ((DWORD)((DWORD)(a) >> (DWORD)(b)))
#define lin_intp(x, xmin, xmax, ymin, ymax) (ymin+((double)((ymax)-(ymin))*((double)((x)-(xmin))/(double)((xmax)-(xmin)))))

#define AsMemCopy CopyMemory

// indexed FM registers

#define AD_LSI                          (0x000)
#define AD_LSI2                         (0x101)
#define AD_TIMER1                       (0x001)
#define AD_TIMER2                       (0x002)
#define AD_MASK                         (0x004)
#define AD_CONNECTION                   (0x104)
#define AD_NEW                          (0x105)
#define AD_NTS                          (0x008)
#define AD_MULT                         (0x020)
#define AD_MULT2                        (0x120)
#define AD_LEVEL                        (0x040)
#define AD_LEVEL2                       (0x140)
#define AD_AD                           (0x060)
#define AD_AD2                          (0x160)
#define AD_SR                           (0x080)
#define AD_SR2                          (0x180)
#define AD_FNUMBER                      (0x0a0)
#define AD_FNUMBER2                     (0x1a0)
#define AD_BLOCK                        (0x0b0)
#define AD_BLOCK2                       (0x1b0)
#define AD_DRUM                         (0x0bd)
#define AD_FEEDBACK                     (0x0c0)
#define AD_FEEDBACK2                    (0x1c0)
#define AD_WAVE                         (0x0e0)
#define AD_WAVE2                        (0x1e0)

/* transformation of linear velocity value to
        logarithmic attenuation */

/* typedefs for MIDI patches */
#define PATCH_1_4OP             (0) /* use 4-operator patch */
#define PATCH_2_2OP             (1) /* use two 2-operator patches */
#define PATCH_1_2OP             (2) /* use one 2-operator patch */

#define NUM2VOICES   18
#define NUMOPS      2

typedef struct _operStruct {
    BYTE    bAt20;              /* flags which are send to 0x20 on fm */
    BYTE    bAt40;              /* flags seet to 0x40 */
                                /* the note velocity & midi velocity affect total level */
    BYTE    bAt60;              /* flags sent to 0x60 */
    BYTE    bAt80;              /* flags sent to 0x80 */
    BYTE    bAtE0;              /* flags send to 0xe0 */
} operStruct;

typedef struct _noteStruct {
    operStruct op[NUMOPS];      /* operators */
    BYTE    bAtA0[2];           /* send to 0xA0, A3 */
    BYTE    bAtB0[2];           /* send to 0xB0, B3 */
                                /* use in a patch, the block should be 4 to indicate
                                    normal pitch, 3 => octave below, etc. */
    BYTE    bAtC0[2];           /* sent to 0xc0, C3 */
    BYTE    bOp;                /* see PATCH_??? */
    BYTE    bDummy;             /* place holder */
} noteStruct;


typedef struct _patchStruct {
    noteStruct note;            /* note. This is all in the structure at the moment */
} patchStruct;


/* MIDI */

typedef struct _voiceStruct {
   BYTE    bVoiceID;               /* used to identify note allocations */
   BYTE    bNote;                  /* note played */
   BYTE    bChannel;               /* channel played on */
   BYTE    bPatch;                 /* what patch is the note,
                                   drums patch = drum note + 128 */
   BYTE    bOn;                    /* TRUE if note is on, FALSE if off */
   BYTE    bVelocity;              /* velocity */
   BYTE    bBlock[2];              /* value sent to the block */
   BYTE    bSusHeld;               /* turned off, but held on by sustain */
   DWORD   dwTime;                 /* time that was turned on/off;
                                   0 time indicates that its not in use */
  // DWORD   dwOrigPitch[2];         /* original pitch, for pitch bend */
   

   // for EG/LFO
   DWORD  dwStartTime;
   long   dwLFOVal;  
   
} voiceStruct;


/* a bit of tuning information */
#define FSAMP                           (49716.0) //falco (50000.0)     /* sampling frequency */
#define PITCH(x)                        ((DWORD)((x) * (double) (1L << 19) / FSAMP))
                            /* x is the desired frequency,
                                == FNUM at b=1 */
#define EQUAL                           (1.0594630943592952645618252)
#define A                               (440.0)
#define ASHARP                          (A * EQUAL)
#define B                               (ASHARP * EQUAL)
#define C                               (B * EQUAL / 2.0)
#define CSHARP                          (C * EQUAL)
#define D                               (CSHARP * EQUAL)
#define DSHARP                          (D * EQUAL)
#define E                               (DSHARP * EQUAL)
#define F                               (E * EQUAL)
#define FSHARP                          (F * EQUAL)
#define G                               (FSHARP * EQUAL)
#define GSHARP                          (G * EQUAL)


/* operator offset location */
static WORD BCODE gw2OpOffset[ NUM2VOICES ][ 2 ] =
   {
     { 0x000,0x003 },
     { 0x001,0x004 },
     { 0x002,0x005 },
     { 0x008,0x00b },
     { 0x009,0x00c },
     { 0x00a,0x00d },
     { 0x010,0x013 },
     { 0x011,0x014 },
     { 0x012,0x015 },

     { 0x100,0x103 },
     { 0x101,0x104 },
     { 0x102,0x105 },
     { 0x108,0x10b },
     { 0x109,0x10c },
     { 0x10a,0x10d },
     { 0x110,0x113 },
     { 0x111,0x114 },
     { 0x112,0x115 },
   } ;

/* pitch values, from middle c, to octave above it */
static DWORD BCODE gdwPitch[12] = {
        PITCH(C), PITCH(CSHARP), PITCH(D), PITCH(DSHARP),
        PITCH(E), PITCH(F), PITCH(FSHARP), PITCH(G),
        PITCH(GSHARP), PITCH(A), PITCH(ASHARP), PITCH(B)};

class OPLSynth
{
private:
    opl3_chip   nuked_Miniport;
	OPL			dosbox_Miniport;
	patchStruct *glpPatch;

    // midi stuff
    voiceStruct m_Voice[NUM2VOICES];  /* info on what voice is where */
    DWORD m_dwCurTime;    /* for note on/off */
	DWORD   m_dwCurSample;      /* for software eg/lfo generators */
    /* volume */
    WORD    m_wSynthAttenL;        /* in 1.5dB steps */
    WORD    m_wSynthAttenR;        /* in 1.5dB steps */	
	//BYTE    m_bRPNCount[16];

    /* support for volume property */
   // LONG    m_MinVolValue;      // Minimum value for volume controller
   // LONG    m_MaxVolValue;      // Maximum value for volume controller
   // ULONG   m_VolStepDelta;     // Correlation between controller and actual decibels
   // LONG    m_SavedVolValue[2]; // Saved value for volume controller

    /* channel volumes */
    BYTE    m_bChanAtten[16];       /* attenuation of each channel, in .75 db steps */
    BYTE    m_bStereoMask[16];              /* mask for left/right for stereo midi files */

    LONG    m_iBend[16];    /* bend for each channel */
	BYTE    m_bDataEnt[16][2]; /* Data Entry MSB/LSB */
	BYTE    m_iBendRange[16];  /* Bend range as dictated by CC100=0, CC101=0,CC6=n, where n= +/- range of semitones */
	BYTE    m_bModWheel[16];   /* Modulation wheel setting */
    BYTE    m_iExpThres[16];   /* 0 to 127 expression value */
	BYTE    m_curVol[16];      /* Volume control */

	BYTE    m_bPatch[16];   /* patch number mapped to */
    BYTE    m_bSustain[16];   /* Is sustain in effect on this channel? */
	BYTE    m_RPN[16][2]; 
	
	bool    isNrpnActive[16]; //falco: either RPN OR NRPN can be active! We do not support NRPN, but false settings can occur if we do not check NRPN status since both use the same data entry.
	
	bool    useNuked; // use Nuked OPL3 emulator engine;
	
	int     sampleRate;

    void ChannelVolume(BYTE bChannel, WORD wAtten);
    void SetPan(BYTE bChannel, BYTE bPan);
    void PitchBend(BYTE bChannel, short iBend);
    void NoteOn(BYTE bPatch,BYTE bNote, BYTE bChannel, BYTE bVelocity,short iBend);
    void NoteOff (BYTE bPatch,BYTE bNote, BYTE bChannel, BYTE bSustain);
    void AllNotesOff(void);
    void ChannelNotesOff(BYTE bChannel);
    WORD FindFullSlot(BYTE bNote, BYTE bChannel);
	void ProcessDataEntry(BYTE bChannel);
    //WORD CalcFAndB (DWORD dwPitch);
	WORD MIDINote2FNum(double note, BYTE bChannel, long dwLFOVal);
    //DWORD CalcBend (DWORD dwOrig, short iBend);
    BYTE CalcVolume (BYTE bOrigAtten, BYTE bChannel,BYTE bVelocity, BYTE bOper, BYTE bMode);
    BYTE CalcStereoMask (BYTE bChannel);
    WORD FindEmptySlot(BYTE bPatch, BYTE bChannel);
    void SetVolume(BYTE bChannel);
    void FMNote(WORD wNote, noteStruct *lpSN);
    void SetSustain(BYTE bChannel, BYTE bSusLevel);
    void LFOUpdate(BYTE bVoice);
	void Write(Bit16u reg, Bit8u v);	
	void Generate(Bit16s *sndptr, Bit32u numsamples);
public:	
    bool Init(int samplerate);
    void WriteMidiData(DWORD dwData);	
    void BoardReset(void);
    void GetSample(short *samplem, int len);
    void PlaySysex(Bit8u *bufpos, DWORD len);
	void SetPatch_Sbi(Bit8u bankNum, Bit8u patchNum, Bit8u *buf, DWORD len);
	void SetBank(int bankNum);
	int  GetActiveVoiceCount(void);
	void SetVolume(WORD volume_atten);
	BYTE GetPatch(BYTE channel);
	void UseNukedOPL3(bool value);

};
#endif