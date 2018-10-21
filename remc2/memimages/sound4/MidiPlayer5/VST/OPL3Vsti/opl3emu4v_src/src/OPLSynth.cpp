// ==============================================================================
//
// Copyright (c) 1996-2000 Microsoft Corporation.  All rights reserved.
//
// ==============================================================================

#include <math.h>
#include "OPLSynth.h"


BYTE gbVelocityAtten[64] = {
        40, 37, 35, 33, 31, 29, 27, 25, 24, 22, 21, 20, 19, 18, 17, 16,
        16, 15, 14, 14, 13, 13, 12, 12, 11, 11, 10, 10, 9,  9,  8,  8,
        7,  7,  6,  6,  6,  5,  5,  5,  4,  4,  4,  4,  3,  3,  3,  3,
        2,  2,  2,  2,  2,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0 };
/*
                                        o
                                     o
                                   o
                                 o
                               o
                             o
                           o
                         o
                        o
                      o
                     o
                    o
                   o
                  o
                 o
                o
                o
               o
              o
              o
             o
             o
            o
            o
           o
           o
          o
          o
         o
         o
        o
        o
       o
       o
      o
      o
      o
     o
     o
     o
    o
    o
    o
    o
   o
   o
   o
   o
  o
  o
  o
  o
  o
 o
 o
 o
 o
 o
o
o
o
o
o
o
*/

BYTE BCODE gbPercMap[53][2] =
{
   {  0, 35 },
   {  0, 35 },
   {  2, 52 },
   {  3, 48 },
   {  4, 58 },
   {  5, 60 },
   {  6, 47 },
   {  7, 43 },
   {  6, 49 },
   {  9, 43 },
   {  6, 51 },
   { 11, 43 },
   {  6, 54 },
   {  6, 57 },
   { 14, 72 },
   {  6, 60 },
   { 16, 76 },
   { 17, 84 },
   { 18, 36 },
   { 19, 76 },
   { 20, 84 },
   { 21, 83 },
   { 22, 84 },
   { 23, 24 },
   { 16, 77 },
   { 25, 60 },
   { 26, 65 },
   { 27, 59 },
   { 28, 51 },
   { 29, 45 },
   { 30, 71 },
   { 31, 60 },
   { 32, 58 },
   { 33, 53 },
   { 34, 64 },
   { 35, 71 },
   { 36, 61 },
   { 37, 61 },
   { 38, 48 },
   { 39, 48 },
   { 40, 69 },
   { 41, 68 },
   { 42, 63 },
   { 43, 74 },
   { 44, 60 },
   { 45, 80 },
   { 46, 64 },
   { 47, 69 },
   { 48, 73 },
   { 49, 75 },
   { 50, 68 },
   { 51, 48 },
   { 52, 53 }
} ;
#include "patch.h"
BYTE offsetSlot[] =
{
    0, 1, 2, 3, 4, 5,
    8, 9, 10, 11, 12, 13,
    16, 17, 18, 19, 20, 21
};


void
OPLSynth::
WriteMidiData(DWORD dwData)
{
    BYTE    bMsgType,bChannel, bVelocity, bNote;
    DWORD   dwTemp;

    bMsgType = (BYTE) dwData & (BYTE)0xf0;
    bChannel = (BYTE) dwData & (BYTE)0x0f;
    bNote = (BYTE) ((WORD) dwData >> 8) & (BYTE)0x7f;
    bVelocity = (BYTE) (dwData >> 16) & (BYTE)0x7f;
    switch (bMsgType)
    {
        case 0x90:      /* turn key on, or key off if volume == 0 */
            if (bVelocity)
            {
                if (bChannel == DRUMCHANNEL)
                {
					if(bNote>=35 && bNote<88)
					{
						NoteOn((BYTE)(gbPercMap[bNote - 35][0]+35+128),gbPercMap[bNote - 35][1],bChannel,bVelocity,m_iBend[bChannel]);
					}
                }
                else
                {
                    NoteOn((BYTE)m_bPatch[bChannel],bNote,bChannel,bVelocity,m_iBend[bChannel]);
                }
                break;
            } // if bVelocity.
            //NOTE: no break specified here. On an else case we want to continue through and turn key off

        case 0x80:
            /* turn key off */
            //  we don't care what the velocity is on note off
            if (bChannel == DRUMCHANNEL)
			{
				if(bNote>=35 && bNote<88)
				{
					NoteOff((BYTE)(gbPercMap[bNote - 35][0]+35+128),gbPercMap[bNote - 35][1], bChannel, 0);
				}
            }
            else
            {
                NoteOff ((BYTE) m_bPatch[bChannel],bNote, bChannel, m_bSustain[ bChannel ]);
            }
            break;

        case 0xb0:
            /* change control */
            switch (bNote)
            {
               	case 1: // Modulation Wheel
					m_bModWheel[bChannel] = bVelocity;
					break;

			    case 6: // Data Entry (CC98-101 dependent)
					m_bDataEnt[bChannel][1] = bVelocity;
					if (!isNrpnActive[bChannel])
						ProcessDataEntry(bChannel);
					break;

				case 7:
                    /* change channel volume */
                    //ChannelVolume(bChannel,gbVelocityAtten[bVelocity >> 1]);
					 m_curVol[bChannel] = bVelocity;
					ChannelVolume(bChannel, bVelocity);
                    break;

                //case 8: // falco: this is Balance controller... need this?
                case 10:
                    /* change the pan level */
                    SetPan(bChannel, bVelocity);
                    break;
				
				case 11: 
					m_iExpThres[bChannel] = bVelocity;
					/* set expression threshold. should influence bChannel.gbVelocityAtten[curVol>>1] range */
					ChannelVolume(bChannel,m_curVol[bChannel]);
					break;

				case 38:  // Data Entry LSB
		            m_bDataEnt[bChannel][0] = bVelocity;
					if (!isNrpnActive[bChannel])
						ProcessDataEntry(bChannel);
					break;


                case 64:
                    /* Change the sustain level */
                    SetSustain(bChannel, bVelocity);
                    break;

				case 100: // RPN LSB
			    case 101: // RPN MSB
					isNrpnActive[bChannel] = false;
					m_RPN[bChannel][(bNote & 0x01)] = bVelocity;
					//++m_bRPNCount[bChannel];
					break;
				
				case 98: // RPN LSB
			    case 99: // RPN MSB
					isNrpnActive[bChannel] = true;
					break;


				
				case 121:
					/* falco: reset controller */
					m_iExpThres[bChannel] = 127;
					m_iBend[bChannel] = 0;
					m_bModWheel[bChannel] = 0;
                    SetSustain(bChannel, 0);
                    break;
				
				case 120:
				case 123:
				case 124:
				case 125:
				case 126:
				case 127:
					/* falco: All sound/notes off */                                     
                    ChannelNotesOff(bChannel);
					break;                    
                   
            };
            break;

        case 0xc0:
            if (bChannel != DRUMCHANNEL)
            {
               m_bPatch[ bChannel ] = bNote ;

            }
            break;

        case 0xe0:  // pitch bend
            dwTemp = ((WORD)bNote << 0) | ((WORD)bVelocity << 7);
			dwTemp -= 0x2000;
			PitchBend(bChannel, (long)dwTemp);
			break;
    };
    return;
}

// ========================= opl3 specific methods ============================

// ==========================================================================
// AllNotesOff - turn off all notes
// ==========================================================================
void
OPLSynth::
AllNotesOff()
{
    BYTE i;

    for (i = 0; i < NUM2VOICES; i++)
    {
        NoteOff(m_Voice[i].bPatch, m_Voice[i].bNote, m_Voice[i].bChannel, 0);
    }
}

// ==========================================================================
//  void NoteOff
//
//  Description:
//     This turns off a note, including drums with a patch
//     # of the drum note + 128, but the first drum instrument is at MIDI note _35_.
//
//  Parameters:
//     BYTE bPatch
//        MIDI patch
//
//     BYTE bNote
//        MIDI note
//
//     BYTE bChannel
//        MIDI channel
//
//  Return Value:
//     Nothing.
//
//
// ==========================================================================
void
OPLSynth::
NoteOff
(
    BYTE            bPatch,
    BYTE            bNote,
    BYTE            bChannel,
    BYTE            bSustain
)
{

  //patchStruct      *lpPS ;
   WORD             wOffset, wTemp ;

   // Find the note slot
   wTemp = FindFullSlot( bNote, bChannel ) ;

   if (wTemp != 0xffff)
   {
      if (bSustain)
      {
          // This channel is sustained, don't really turn the note off,
          // just flag it.
          //
          m_Voice[ wTemp ].bSusHeld = 1;

          return;
      }

      // get a pointer to the patch
      //lpPS = glpPatch + (BYTE) m_Voice[ wTemp ].bPatch ;

      // shut off the note portion
      // we have the note slot, turn it off.
      wOffset = wTemp;
      if (wTemp >= (NUM2VOICES / 2))
         wOffset += (0x100 - (NUM2VOICES / 2));

      Write(AD_BLOCK + wOffset,
                  (BYTE)(m_Voice[ wTemp ].bBlock[ 0 ] & 0x1f) ) ;

      // Note this...
      m_Voice[ wTemp ].bOn = FALSE ;
      m_Voice[ wTemp ].bBlock[ 0 ] &= 0x1f ;
      m_Voice[ wTemp ].bBlock[ 1 ] &= 0x1f ;
      m_Voice[ wTemp ].dwTime = m_dwCurTime ;
	  m_Voice[ wTemp ].bSusHeld = 0;
   }
}

// ==========================================================================
//  WORD FindFullSlot
//
//  Description:
//     This finds a slot with a specific note, and channel.
//     If it is not found then 0xFFFF is returned.
//
//  Parameters:
//     BYTE bNote
//        MIDI note number
//
//     BYTE bChannel
//        MIDI channel #
//
//  Return Value:
//     WORD
//        note slot #, or 0xFFFF if can't find it
//
//
// ==========================================================================
WORD
OPLSynth::
FindFullSlot
(
    BYTE            bNote,
    BYTE            bChannel
)
{
   WORD  i ;

   for (i = 0; i < NUM2VOICES; i++)
   {
      if ((bChannel == m_Voice[ i ].bChannel)
            && (bNote == m_Voice[ i ].bNote)
            && (m_Voice[ i ].bOn))
      {
            return ( i ) ;
      }
   // couldn't find it
   }
   return ( 0xFFFF ) ;
}

//------------------------------------------------------------------------
//  void FMNote
//
//  Description:
//     Turns on an FM-synthesizer note.
//
//  Parameters:
//     WORD wNote
//        the note number from 0 to NUMVOICES
//
//     noteStruct *lpSN
//        structure containing information about what
//        is to be played.
//
//  Return Value:
//     Nothing.
//------------------------------------------------------------------------
void
OPLSynth::
FMNote
(
    WORD                wNote,
    noteStruct     *    lpSN
)
{
   WORD            i ;
   WORD            wOffset ;
   operStruct      *lpOS ;

   // write out a note off, just to make sure...

   wOffset = wNote;
   if (wNote >= (NUM2VOICES / 2))
      wOffset += (0x100 - (NUM2VOICES / 2));

   Write(AD_BLOCK + wOffset, 0 ) ;

   // writing the operator information

//   for (i = 0; i < (WORD)((wNote < NUM4VOICES) ? NUMOPS : 2); i++)
   for (i = 0; i < 2; i++)
   {
      lpOS = &lpSN -> op[ i ] ;
      wOffset = gw2OpOffset[ wNote ][ i ] ;
      Write( 0x20 + wOffset, lpOS -> bAt20) ;
      Write( 0x40 + wOffset, lpOS -> bAt40) ;
      Write( 0x60 + wOffset, lpOS -> bAt60) ;
      Write( 0x80 + wOffset, lpOS -> bAt80) ;
      Write( 0xE0 + wOffset, lpOS -> bAtE0) ;

   }

   // write out the voice information
   wOffset = (wNote < 9) ? wNote : (wNote + 0x100 - 9) ;
   Write(0xa0 + wOffset, lpSN -> bAtA0[ 0 ] ) ;
   Write(0xc0 + wOffset, lpSN -> bAtC0[ 0 ] ) ;

   // Note on...
   Write(0xb0 + wOffset,
               (BYTE)(lpSN -> bAtB0[ 0 ] | 0x20) ) ;

} // end of FMNote()

//=======================================================================
//  WORD NoteOn
//
//  Description:
//     This turns on a note, including drums with a patch # of the
//     drum note + 0x80.  The first GM drum instrument is mapped to note 35 instead of zero, though, so
//     we expect 0 as the first drum patch (acoustic kick) if note 35 comes in.
//
//  Parameters:
//     BYTE bPatch
//        MIDI patch
//
//     BYTE bNote
//        MIDI note
//
//     BYTE bChannel
//        MIDI channel
//
//     BYTE bVelocity
//        velocity value
//
//     short iBend
//        current pitch bend from -32768 to 32767
//
//  Return Value:
//     WORD
//        note slot #, or 0xFFFF if it is inaudible
//=======================================================================
void
OPLSynth::
NoteOn
(
    BYTE            bPatch,
    BYTE            bNote,
    BYTE            bChannel,
    BYTE            bVelocity,
    short           iBend
)
{
   WORD             wTemp, i, j ;
   BYTE             /*b4Op,*/ bTemp, bMode, bStereo ;
   patchStruct      *lpPS ;
   DWORD            dwBasicPitch;
   noteStruct       NS ;

   // Get a pointer to the patch
   lpPS = glpPatch + bPatch ;
   // Find out the basic pitch according to our
   // note value.  This may be adjusted because of
   // pitch bends or special qualities for the note.

   dwBasicPitch = gdwPitch[ bNote % 12 ] ;
   bTemp = bNote / (BYTE) 12 ;
   if (bTemp > (BYTE) (60 / 12))
      dwBasicPitch = AsLSHL( dwBasicPitch, (BYTE)(bTemp - (BYTE)(60/12)) ) ;
   else if (bTemp < (BYTE) (60/12))
      dwBasicPitch = AsULSHR( dwBasicPitch, (BYTE)((BYTE) (60/12) - bTemp) ) ;

   // Copy the note information over and modify
   // the total level and pitch according to
   // the velocity, midi volume, and tuning.

   RtlCopyMemory( (LPSTR) &NS, (LPSTR) &lpPS -> note, sizeof( noteStruct ) ) ;
   //b4Op = (BYTE)(NS.bOp != PATCH_1_2OP) ;

   for (j = 0; j < 2; j++)
   {
	  wTemp = MIDINote2FNum(bNote, bChannel, 0);
      NS.bAtA0[ j ] = (BYTE) wTemp ;
      NS.bAtB0[ j ] = (BYTE) 0x20 | (BYTE) (wTemp >> 8) ;
   }
   // Modify level for each operator, but only
   // if they are carrier waves

   bMode = (BYTE) ((NS.bAtC0[ 0 ] & 0x01) * 2 + 4) ;

   for (i = 0; i < 2; i++)
   {
      wTemp = (BYTE)
          CalcVolume(  (BYTE)(NS.op[ i ].bAt40 & (BYTE) 0x3f),
                            bChannel,
                            bVelocity,
                            (BYTE) i,
                            bMode ) ;
      NS.op[ i ].bAt40 = (NS.op[ i ].bAt40 & (BYTE)0xc0) | (BYTE) wTemp ;
   }

   // Do stereo panning, but cutting off a left or
   // right channel if necessary...

   bStereo = CalcStereoMask( bChannel ) ;
   NS.bAtC0[ 0 ] &= bStereo ;

   // Find an empty slot, and use it...
   wTemp = FindEmptySlot( bPatch, bChannel) ;

   FMNote(wTemp, &NS ) ;
   m_Voice[ wTemp ].bNote = bNote ;
   m_Voice[ wTemp ].bChannel = bChannel ;
   m_Voice[ wTemp ].bPatch = bPatch ;
   m_Voice[ wTemp ].bVelocity = bVelocity ;
   m_Voice[ wTemp ].bOn = TRUE ;
   m_Voice[ wTemp ].dwTime = m_dwCurTime++ ;
   //m_Voice[ wTemp ].dwOrigPitch[0] = dwPitch[ 0 ] ;  // not including bend
   //m_Voice[ wTemp ].dwOrigPitch[1] = dwPitch[ 1 ] ;  // not including bend
   m_Voice[ wTemp ].bBlock[0] = NS.bAtB0[ 0 ] ;
   m_Voice[ wTemp ].bBlock[1] = NS.bAtB0[ 1 ] ;
   m_Voice[ wTemp ].bSusHeld = 0;


} // end of NoteOn()

void
OPLSynth::
ProcessDataEntry(BYTE bChannel)
{
   WORD rpn  = (WORD)(m_RPN[bChannel][0])|(m_RPN[bChannel][1] << 7);
       
   //DWORD dwTemp;
   BYTE val = m_bDataEnt[bChannel][1];  

  // if (m_bRPNCount[bChannel] >= 2)
   {
     // m_bRPNCount[bChannel] = 2;

      switch (rpn)
      {
         case (WORD)0x0000:   // Pitch Bend Range extension
            // Calculate base bend value then apply
            ///////////m_iBendRange[bChannel] = (!m_iBendRange[bChannel]) ? 2 : m_iBendRange[bChannel];
            //dwTemp = (m_iBendRange[bChannel]) ? ((long)m_iBend[bChannel] / m_iBendRange[bChannel]) : 0;
            m_iBendRange[bChannel] = val & 0x7f;
            //dwTemp = (dwTemp > 0) ? dwTemp * m_iBendRange[bChannel] : ((long)m_iBend[bChannel] * m_iBendRange[bChannel]);
            //m_iBend[bChannel] = (long) (dwTemp);

            // Update pitch bend
            PitchBend(bChannel, m_iBend[bChannel]);
            break;

        
      }
   }   
         
}



//=======================================================================
// CalcVolume - This calculates the attenuation for an operator.
//
//inputs
//        BYTE    bOrigAtten - original attenuation in 0.75 dB units
//        BYTE    bChannel - MIDI channel
//        BYTE    bVelocity - velocity of the note
//        BYTE    bOper - operator number (from 0 to 3)
//        BYTE    bMode - voice mode (from 0 through 7 for
//                                modulator/carrier selection)
//returns
//        BYTE - new attenuation in 0.75 dB units, maxing out at 0x3f.
//=======================================================================
BYTE
OPLSynth::
CalcVolume(BYTE bOrigAtten,BYTE bChannel,BYTE bVelocity,BYTE bOper,BYTE bMode)
{
    BYTE        bVolume;
    WORD        wTemp;
    WORD        wMin;

    switch (bMode) {
        case 0:
                bVolume = (BYTE)(bOper == 3);
                break;
        case 1:
                bVolume = (BYTE)((bOper == 1) || (bOper == 3));
                break;
        case 2:
                bVolume = (BYTE)((bOper == 0) || (bOper == 3));
                break;
        case 3:
                bVolume = (BYTE)(bOper != 1);
                break;
        case 4:
                bVolume = (BYTE)((bOper == 1) || (bOper == 3));
                break;
        case 5:
                bVolume = (BYTE)(bOper >= 1);
                break;
        case 6:
                bVolume = (BYTE)(bOper <= 2);
                break;
        case 7:
                bVolume = TRUE;
                break;
        default:
                bVolume = FALSE;
                break;
        };
    if (!bVolume)
        return bOrigAtten; /* this is a modulator wave */

    wMin =(m_wSynthAttenL < m_wSynthAttenR) ? m_wSynthAttenL : m_wSynthAttenR;
    wTemp = bOrigAtten +
            ((wMin << 1) +
            m_bChanAtten[bChannel] +
            gbVelocityAtten[bVelocity >> 1]);
    return (wTemp > 0x3f) ? (BYTE) 0x3f : (BYTE) wTemp;
}

// ===========================================================================
// ChannelNotesOff - turn off all notes on a channel
// ===========================================================================
void
OPLSynth::
ChannelNotesOff(BYTE bChannel)
{
    int i;

    for (i = 0; i < NUM2VOICES; i++)
    {
       if ((m_Voice[ i ].bOn) && (m_Voice[ i ].bChannel == bChannel))
       {
          NoteOff(m_Voice[i].bPatch, m_Voice[i].bNote,m_Voice[i].bChannel, 0) ;
       }
    }
}

// ===========================================================================
/* ChannelVolume - set the volume level for an individual channel.
 *
 * inputs
 *      BYTE    bChannel - channel number to change
 *      WORD    wAtten  - attenuation in 1.5 db units
 *
 * returns
 *      none
 */
// ===========================================================================
void
OPLSynth::
ChannelVolume(BYTE bChannel, WORD wAtten)
{
    //m_bChanAtten[bChannel] = (BYTE)wAtten;
	 m_bChanAtten[bChannel] = (BYTE)gbVelocityAtten[(BYTE)(((wAtten & 0x7F) >> 1) * ((float)m_iExpThres[bChannel]/0x7F))];

    SetVolume(bChannel);
}

// ===========================================================================
//  void SetVolume
//
//  Description:
//     This should be called if a volume level has changed.
//     This will adjust the levels of all the playing voices.
//
//  Parameters:
//     BYTE bChannel
//        channel # of 0xFF for all channels
//
//  Return Value:
//     Nothing.
//
// ===========================================================================
void
OPLSynth::
SetVolume
(
    BYTE   bChannel
)
{
   WORD            i, j, wTemp, wOffset ;
   noteStruct      *lpPS ;
   BYTE            bMode, bStereo ;

   // Make sure that we are actually open...
   if (!glpPatch)
      return ;

   // Loop through all the notes looking for the right
   // channel.  Anything with the right channel gets
   // its pitch bent.
   for (i = 0; i < NUM2VOICES; i++)
   {
      if ((m_Voice[ i ].bChannel == bChannel) || (bChannel == 0xff))
      {
         // Get a pointer to the patch
         lpPS = &(glpPatch + m_Voice[ i ].bPatch) -> note ;

         // Modify level for each operator, IF they are carrier waves...
         bMode = (BYTE) ( (lpPS->bAtC0[0] & 0x01) * 2 + 4);

         for (j = 0; j < 2; j++)
         {
            wTemp = (BYTE) CalcVolume(
               (BYTE) (lpPS -> op[j].bAt40 & (BYTE) 0x3f),
               m_Voice[i].bChannel, m_Voice[i].bVelocity,
               (BYTE) j,            bMode ) ;

            // Write new value.
            wOffset = gw2OpOffset[ i ][ j ] ;
            Write(
               0x40 + wOffset,
               (BYTE) ((lpPS -> op[j].bAt40 & (BYTE)0xc0) | (BYTE) wTemp) ) ;
         }

         // Do stereo pan, but cut left or right channel if needed.
         bStereo = CalcStereoMask( m_Voice[ i ].bChannel ) ;
         wOffset = i;
         if (i >= (NUM2VOICES / 2))
             wOffset += (0x100 - (NUM2VOICES / 2));
         Write(0xc0 + wOffset, (BYTE)(lpPS -> bAtC0[ 0 ] & bStereo) ) ;
      }
   }
} // end of SetVolume

// ===========================================================================
// SetPan - set the left-right pan position.
//
// inputs
//      BYTE    bChannel - channel number to alter
//      BYTE    bPan     - 0-47 for left, 81-127 for right, or somewhere in the middle.
//
// returns - none
//
//  As a side note, I think it's odd that (since 64 = CENTER, 127 = RIGHT and 0 = LEFT)
//  there are 63 intermediate gradations for the left side, but 62 for the right.
// ===========================================================================
void
OPLSynth::
SetPan(BYTE bChannel, BYTE bPan)
{
    /* change the pan level */
    if (bPan > (64 + 16))
            m_bStereoMask[bChannel] = 0xef;      /* let only right channel through */
    else if (bPan < (64 - 16))
            m_bStereoMask[bChannel] = 0xdf;      /* let only left channel through */
    else
            m_bStereoMask[bChannel] = 0xff;      /* let both channels */

    /* change any curently playing patches */
    SetVolume(bChannel);
}

// ===========================================================================
//  void MIDINote2FNum
//
//  Description:
//     Obtains FNumber from MIDI note + current bend values
//     Special thanks to ValleyBell for MidiPlay sources for adaption
//
//  Parameters:
//     double note   - MIDI note number (promoted from BYTE)
//     BYTE bChannel - channel
//     long dwLFOVal - current magnitude of the modulation waveform
//
//  Return Value:
//     ((WORD) BlockVal << 10) | (WORD) keyVal;
// ===========================================================================
WORD
OPLSynth::
MIDINote2FNum(double note, BYTE bChannel, long dwLFOVal)
{
	double freqVal, curNote;
	signed char BlockVal;
	WORD keyVal;
//	signed long CurPitch;
	
   /*TODO: keep for later, may add other features */
	//CurPitch = //MMstTuning + TempMid->TunePb + TempMid->Pitch + TempMid->ModPb;
	   

	curNote = (double)(note + (m_iBend[bChannel] * m_iBendRange[bChannel] + dwLFOVal) / 8192.0); //Note + CurPitch / 8192.0;
	freqVal = 440.0 * pow(2.0, (curNote - 69) / 12.0);
	//BlockVal = ((signed short int)CurNote / 12) - 1;
	BlockVal = ((signed short int)(curNote + 6) / 12) - 2;
	if (BlockVal < 0x00)
		BlockVal = 0x00;
	else if (BlockVal > 0x07)
		BlockVal = 0x07;
	//KeyVal = (unsigned short int)(FreqVal * pow(2, 20 - BlockVal) / CHIP_RATE + 0.5);
	keyVal = (WORD)(freqVal * (1ULL << (20 - BlockVal)) / FSAMP + 0.5);
	if (keyVal > 0x03FF)
		keyVal = 0x03FF;
	
	return (BlockVal << 10) | keyVal;	// << (8+2)
}



// ===========================================================================
//  void PitchBend
//
//  Description:
//     This pitch bends a channel.
//
//  Parameters:
//     BYTE bChannel
//        channel
//
//     short iBend
//        values from -32768 to 32767, being -2 to +2 half steps
//
//  Return Value:
//     Nothing.
// ===========================================================================
void
OPLSynth::
PitchBend
(
    BYTE        bChannel,
    short        iBend
)
{
   WORD   i, wTemp[ 2 ], wOffset, j ;
//   DWORD  dwNew ;

   // Remember the current bend value
   m_iBend[ bChannel ] = iBend;

   // Loop through all the notes looking for
   // the correct channel.  Anything with the
   // correct channel gets its pitch bent...
   for (i = 0; i < NUM2VOICES; i++)
   {
      if (m_Voice[ i ].bChannel == bChannel/* && (m_Voice [ i ].bOn || m_bSustain[bChannel])*/)
      {
         j = 0 ;
         //wTemp[ j ] = CalcFAndB( CalcBend( m_Voice[ i ].dwOrigPitch[ j ], iBend ) ) ;
         wTemp[ j ] = MIDINote2FNum(m_Voice[ i ].bNote, bChannel, 0 );
         m_Voice[ i ].bBlock[ j ] =
            (m_Voice[ i ].bBlock[ j ] & (BYTE) 0xe0) |
            (BYTE) (wTemp[ j ] >> 8) ;

         wOffset = i;
         if (i >= (NUM2VOICES / 2))
            wOffset += (0x100 - (NUM2VOICES / 2));


         Write(AD_BLOCK + wOffset, m_Voice[ i ].bBlock[ 0 ] ) ;
         Write(AD_FNUMBER + wOffset, (BYTE) wTemp[ 0 ] ) ;
	  }
   }
} // end of PitchBend

void
   OPLSynth::
   LFOUpdate(BYTE bVoice)
{
   WORD  wTemp, wOffset;
   long  newLFOVal = m_Voice[bVoice].dwLFOVal;         
   DWORD timeDiff = m_dwCurSample-m_Voice[bVoice].dwStartTime;
   double timeLapse = 0.00025*3.14159265358979323846*(double)(timeDiff),
          noteDiff = 0;
   
         

   // 100Hz sine wave with half semitone magnitude by default
   // (mod*32)sin((1/100)*FSAMP * curSample)
   if (m_bModWheel[m_Voice[bVoice].bChannel] > 0)
      newLFOVal = (long)floor(0.5 + (m_bModWheel[m_Voice[bVoice].bChannel]*32)*sin(timeLapse));

   // Linear envelope generator hack  (TODO: improve)
   wTemp = gbPercMap[m_Voice[bVoice].bPatch-128][2] & 0xFF;
   
   if (newLFOVal == m_Voice[bVoice].dwLFOVal)
      return;

   m_Voice[bVoice].dwLFOVal = newLFOVal;   

   wTemp = MIDINote2FNum((noteDiff)?noteDiff:m_Voice[ bVoice ].bNote,
      m_Voice[bVoice].bChannel, newLFOVal);
   m_Voice[ bVoice ].bBlock[ 0 ] =
      (m_Voice[ bVoice ].bBlock[ 0 ] & (BYTE) 0xe0) |
      (BYTE) (wTemp >> 8) ;

   wOffset = bVoice;
   if (bVoice >= (NUM2VOICES / 2))
      wOffset += (0x100 - (NUM2VOICES / 2));

   // Depends if voice is enabled or not
   if (m_Voice[bVoice].bOn == FALSE)
      wTemp &= ~(1<<5);

   Write(AD_BLOCK + wOffset, m_Voice[ bVoice ].bBlock[ 0 ] ) ;
   Write(AD_FNUMBER + wOffset, (BYTE) wTemp ) ;
   
} // end of PitchBend



// ===========================================================================
//  CalcStereoMask - This calculates the stereo mask.
//
//  inputs
//            BYTE  bChannel - MIDI channel
//  returns
//            BYTE  mask (for register 0xc0-c8) for eliminating the
//                  left/right/both channels
// ===========================================================================
BYTE
OPLSynth::
CalcStereoMask(BYTE bChannel)
{
    WORD        wLeft, wRight;

    /* figure out the basic levels of the 2 channels */
    wLeft = (m_wSynthAttenL << 1) + m_bChanAtten[bChannel];
    wRight = (m_wSynthAttenR << 1) + m_bChanAtten[bChannel];

    /* if both are too quiet then mask to nothing */
    if ((wLeft > 0x3f) && (wRight > 0x3f))
        return 0xcf;

    /* if one channel is significantly quieter than the other than
        eliminate it */
    if ((wLeft + 8) < wRight)
        return (BYTE)(0xef & m_bStereoMask[bChannel]);   /* right is too quiet so eliminate */
    else if ((wRight + 8) < wLeft)
        return (BYTE)(0xdf & m_bStereoMask[bChannel]);   /* left too quiet so eliminate */
    else
        return (BYTE)(m_bStereoMask[bChannel]);  /* use both channels */
}

//------------------------------------------------------------------------
//  WORD FindEmptySlot
//
//  Description:
//     This finds an empty note-slot for a MIDI voice.
//     If there are no empty slots then this looks for the oldest
//     off note.  If this doesn't work then it looks for the oldest
//     on-note of the same patch.  If all notes are still on then
//     this finds the oldests turned-on-note.
//
//  Parameters:
//     BYTE bPatch
//        MIDI patch that will replace it.
//
//  Return Value:
//     WORD
//        note slot #
//
//
//------------------------------------------------------------------------
WORD
OPLSynth::
FindEmptySlot(BYTE bPatch, BYTE bChannel)
{
   BYTE   bChnVoiceCnt = 0;
   WORD   i, foundOldestOff, foundOldestCurCh, foundOldestOn;
   DWORD  dwOldestOff, dwOldestCurCh, dwOldestOn ;

   // Now, look for a slot of the oldest off-note
   dwOldestOff = 0xffffffff ;
   dwOldestCurCh = 0xffffffff ;
   dwOldestOn = 0xffffffff ;
   foundOldestOff = 0xffff ;
   foundOldestCurCh = 0xffff;
   foundOldestOn = 0xffff;

   for (i = 0; i < NUM2VOICES; i++)
   {
      if (!m_Voice[ i ].dwTime)
         return ( i ) ;

      if (!m_Voice[ i ].bOn && m_Voice[ i ].dwTime < dwOldestOff)
      {
         dwOldestOff = m_Voice[ i ].dwTime ;
         foundOldestOff = i ;
      }

      if ((m_Voice[ i ].bOn || m_Voice[ i ].bSusHeld) && m_Voice[ i ].bChannel == bChannel && m_Voice[ i ].dwTime < dwOldestCurCh)
      {
         dwOldestCurCh = m_Voice[ i ].dwTime;
         foundOldestCurCh = i;
         ++bChnVoiceCnt;
      }

      if (m_Voice[ i ].dwTime < dwOldestOn)
      {
         dwOldestOn = m_Voice[ i ].dwTime;
         foundOldestOn = i;
      }
   }    

   if (foundOldestOff != 0xffff)
      return ( foundOldestOff ) ;

   if (foundOldestCurCh != 0xffff && bChnVoiceCnt > 3)
      return ( foundOldestCurCh ) ;

   return foundOldestOn;
   
} // end of FindEmptySlot()

//------------------------------------------------------------------------
//  WORD SetSustain
//
//  Description:
//     Set the sustain controller on the current channel.
//
//  Parameters:
//     BYTE bSusLevel
//        The new sustain level
//
//
//------------------------------------------------------------------------
void
OPLSynth::
SetSustain(BYTE bChannel,BYTE bSusLevel)
{
   WORD i, wOffset;

   if (m_bSustain[ bChannel ] && !bSusLevel)
   {
      // Sustain has just been turned off for this channel
      // Go through and turn off all notes that are being held for sustain
      //
      for (i = 0; i < NUM2VOICES; i++)
      {
         if ((bChannel == m_Voice[ i ].bChannel) &&
            m_Voice[ i ].bSusHeld)
         {
            // this is not guaranteed to cut repeated sustained notes
            //NoteOff(m_Voice[i].bPatch, m_Voice[i].bNote, m_Voice[i].bChannel, 0);

            wOffset = i;
            if (i >= (NUM2VOICES / 2))
               wOffset += (0x100 - (NUM2VOICES / 2));

            Write(AD_BLOCK + wOffset,
               (BYTE)(m_Voice[ i ].bBlock[ 0 ] & 0x1f) ) ;

            // Note this...
            m_Voice[ i ].bOn = FALSE ;
            m_Voice[ i ].bBlock[ 0 ] &= 0x1f ;
            m_Voice[ i ].bBlock[ 1 ] &= 0x1f ;
            m_Voice[ i ].dwTime = m_dwCurTime ;
            m_Voice[ i ].bSusHeld = FALSE ;
         }
      }
   }
   m_bSustain[ bChannel ] = bSusLevel;
}

void
OPLSynth::
BoardReset()
{
    BYTE i;
	
	AllNotesOff();

     /* start attenuations at -3 dB, which is 90 MIDI level */
    for (i = 0; i < 16; i++)
    {
        m_bChanAtten[i] = 4;
        m_bStereoMask[i] = 0xff;
		m_iExpThres[i] = 127;    // CC11 = max
		m_curVol[i] = 100;       // CC7 = 100

		m_iBend[i] = 0;          // central value
		m_iBendRange[i] = 2;     // -/+2 semitones   
		isNrpnActive[i] = false; //RPN has precedence over NRPN

		m_bPatch[i] = 0;		
		m_bModWheel[i] = 0;    
		//m_bRPNCount[i] = 0;
		memset(m_RPN[i], 0, sizeof(WORD));     
		memset(m_bDataEnt[i], 2, sizeof(WORD)); 
		SetSustain(i, 0);

    };

	/* ---- silence the chip -------- */

    /* tell the FM chip to use 4-operator mode, and
    fill in any other random variables */
    Write(AD_NEW, 0x01);
    Write(AD_MASK, 0x60);
    Write(AD_CONNECTION, 0x00);
    Write(AD_NTS, 0x00);

    /* turn off the drums, and use high vibrato/modulation */
    Write(AD_DRUM, 0xc0);

    /* turn off all the oscillators */
    for (i = 0; i <= 0x15; i++)
    {
        if ((i & 0x07) <= 0x05)
        {
            Write(AD_LEVEL + i, 0x3f);
            Write(AD_LEVEL2 + i, 0x3f);
        }
    };

    /* turn off all the voices */
    for (i = 0; i <= 0x08; i++)
    {
        Write(AD_BLOCK + i, 0x00);
        Write(AD_BLOCK2 + i, 0x00);
    };
}
bool
OPLSynth::
Init(int samplerate)
{
    sampleRate = samplerate;
	glpPatch = FatglpPatch;
    // init some members    
	m_dwCurTime = 1;    /* for note on/off */
    /* volume */
    m_wSynthAttenL = 0;        /* in 1.5dB steps */
    m_wSynthAttenR = 0;        /* in 1.5dB steps */

   // m_MinVolValue  = 0xFFD0C000;    //  minimum -47.25(dB) * 0x10000
   // m_MaxVolValue  = 0x00000000;    //  maximum  0    (dB) * 0x10000
   // m_VolStepDelta = 0x0000C000;    //  steps of 0.75 (dB) * 0x10000
   
	useNuked = false;

	dosbox_Miniport.adlib_init(samplerate);	
	OPL3_Reset(&nuked_Miniport, samplerate);  
	
	BoardReset();

	return true;
}

void
OPLSynth::
SetPatch_Sbi(Bit8u bankNum, Bit8u patchNum, Bit8u *buf, DWORD len)
{
   if (len != 11) return; 
   
   switch (bankNum)
   {
	   case 0:
		   FatglpPatch[patchNum].note.op[0].bAt20 = buf[0];
		   FatglpPatch[patchNum].note.op[1].bAt20 = buf[1];
		   FatglpPatch[patchNum].note.op[0].bAt40 = buf[2];
		   FatglpPatch[patchNum].note.op[1].bAt40 = buf[3];
		   FatglpPatch[patchNum].note.op[0].bAt60 = buf[4];
		   FatglpPatch[patchNum].note.op[1].bAt60 = buf[5];
		   FatglpPatch[patchNum].note.op[0].bAt80 = buf[6];
		   FatglpPatch[patchNum].note.op[1].bAt80 = buf[7];
		   FatglpPatch[patchNum].note.op[0].bAtE0 = buf[8];
		   FatglpPatch[patchNum].note.op[1].bAtE0 = buf[9];
		   FatglpPatch[patchNum].note.bAtC0[0] = (buf[10] & 0x0F) | 0x30;

		   break;
	   case 1: 
		   MauiglpPatch[patchNum].note.op[0].bAt20 = buf[0];
		   MauiglpPatch[patchNum].note.op[1].bAt20 = buf[1];
		   MauiglpPatch[patchNum].note.op[0].bAt40 = buf[2];
		   MauiglpPatch[patchNum].note.op[1].bAt40 = buf[3];
		   MauiglpPatch[patchNum].note.op[0].bAt60 = buf[4];
		   MauiglpPatch[patchNum].note.op[1].bAt60 = buf[5];
		   MauiglpPatch[patchNum].note.op[0].bAt80 = buf[6];
		   MauiglpPatch[patchNum].note.op[1].bAt80 = buf[7];
		   MauiglpPatch[patchNum].note.op[0].bAtE0 = buf[8];
		   MauiglpPatch[patchNum].note.op[1].bAtE0 = buf[9];
		   MauiglpPatch[patchNum].note.bAtC0[0] = (buf[10] & 0x0F) | 0x30;

		   break;
	   case 2: 
		   FmsglpPatch[patchNum].note.op[0].bAt20 = buf[0];
		   FmsglpPatch[patchNum].note.op[1].bAt20 = buf[1];
		   FmsglpPatch[patchNum].note.op[0].bAt40 = buf[2];
		   FmsglpPatch[patchNum].note.op[1].bAt40 = buf[3];
		   FmsglpPatch[patchNum].note.op[0].bAt60 = buf[4];
		   FmsglpPatch[patchNum].note.op[1].bAt60 = buf[5];
		   FmsglpPatch[patchNum].note.op[0].bAt80 = buf[6];
		   FmsglpPatch[patchNum].note.op[1].bAt80 = buf[7];
		   FmsglpPatch[patchNum].note.op[0].bAtE0 = buf[8];
		   FmsglpPatch[patchNum].note.op[1].bAtE0 = buf[9];
		   FmsglpPatch[patchNum].note.bAtC0[0] = (buf[10] & 0x0F) | 0x30;
	
		   break;
	   
	   default:	
		   break;

   }

   
  
}

void
OPLSynth::
SetBank(int bankNum)
{
	switch (bankNum)
	{
		case 0: 
		glpPatch = FatglpPatch;
		break;
		
		case 1:
		glpPatch = MauiglpPatch;
	    break;

	    case 2:
		glpPatch = FmsglpPatch;
        break;
		
		default:
		glpPatch = FatglpPatch;
		break;
	}
	AllNotesOff();
}

int
OPLSynth::
GetActiveVoiceCount()
{
	BYTE i;
	BYTE count = 0;

	for (i = 0; i < NUM2VOICES; i++) 
	{
		if (m_Voice[i].bSusHeld || m_Voice[i].bOn)
			count++;
	}
	
	return count;
}

void
OPLSynth::
SetVolume(WORD volume_atten)
{
	BYTE i;

	m_wSynthAttenL = volume_atten;        
    m_wSynthAttenR = volume_atten; 
	
	for (i = 0; i <= 15; i++) 
	{
		SetVolume(i);
	}
}

BYTE
OPLSynth::
GetPatch(BYTE channel)
{
    BYTE result = 0;

	if (channel < 16)
	result = m_bPatch[channel];

	return result;
}

void
OPLSynth::
PlaySysex(Bit8u *bufpos, DWORD len)
{
   bool isResetSysex = false;
   const BYTE resetArray[6] = {0xF0, 0x7E, 0x7F, 0x09, 0x01, 0xF7};
   
   // Ignore Sysex Continued or non-standard sysex events
   if (bufpos[0] != 0xF0 || bufpos[len-1] != 0xF7)
      return;

   // Check reset
   if (len == 6  && memcmp(&resetArray[0], bufpos, 6) == 0)
      isResetSysex = true;
   
   if (isResetSysex)
   {
	  AllNotesOff();
	  BoardReset();
   }
  
}


void 
OPLSynth::
GetSample(short *sample, int len)
{
   BYTE i;
   
   for (i = 0; i < NUM2VOICES; i++)
   {
      // Only execute once initialized
      if (m_Voice[i].bChannel != (BYTE)~0)
         LFOUpdate(i);
   }      

   m_dwCurSample += len;

   Generate(sample, len);
}

void 
OPLSynth::
Write(Bit16u reg, Bit8u v)
{
	OPL3_WriteReg(&nuked_Miniport, reg, v);
	dosbox_Miniport.adlib_write( reg, v);
}

void 
OPLSynth::
Generate(Bit16s *sndptr, Bit32u numsamples)
{
	if (useNuked)
		OPL3_GenerateStream(&nuked_Miniport, sndptr, numsamples);
	else
		dosbox_Miniport.adlib_getsample(sndptr, numsamples);

}
void 
OPLSynth::
UseNukedOPL3(bool value) 
{
	if (value != useNuked)
	{
		AllNotesOff();
		OPL3_Reset(&nuked_Miniport, sampleRate);	
		useNuked = value;
		AllNotesOff();
	}
	
}
