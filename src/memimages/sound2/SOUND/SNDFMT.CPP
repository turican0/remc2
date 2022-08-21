////////////////////////////////////////////////////////////////////////
// $Source: x:/prj/tech/libsrc/sound/RCS/sndfmt.cpp $
// $Author: PATMAC $
// $Date: 1998/03/20 12:55:54 $
// $Revision: 1.12 $
//
// (c) 1996 Looking Glass Technologies Inc.
// Pat McElhatton (from JohnB)
//
// Module name: sound formats
// File name: sndfmt.cpp
//
// Description: Implementation of sound file/resource format handling
//
////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <lg.h>
#include <mmsystem.h>
#include <mmreg.h>

#include <dsound.h>
#include <lgsndi.h>
#include <sndfmt.h>

#include <sndvoc.h>
#include <mprintf.h>

static int
openWaveHeader(
               HMMIO          hmmio,
               MMIOINFO       *pMmio,
               void           **ppPCMData,
               uint32         *pPCMLen,
               uint32         *pNSamples,
               sSndAttribs    *pAttribs
               )
{
   MMCKINFO       riff;
   MMCKINFO       chunk;
   WAVEFORMATEX   waveFmt;
   int            nBytes, bytesPerSamp;
   WORD           sampsPerBlock;
   uint32         maxSamples, nBlocks, partialBlockLen;

   // get the next riff chunk
   if(mmioDescend(hmmio,&riff, NULL, 0))
      return TRUE;

   // make sure its a wave file
   if((riff.ckid != FOURCC_RIFF) || (riff.fccType != mmioFOURCC('W','A','V','E')))
      return TRUE;

   /*----------------------------
    * process format chunk
    *---------------------------*/
   // get the format chunk
   chunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
   if (mmioDescend(hmmio, &chunk, &riff, MMIO_FINDCHUNK))
      return TRUE;

   // make sure the chunk is the right size, otherwise we dont know what
   // kind of wave file it is
   if(chunk.cksize < (long)sizeof(PCMWAVEFORMAT))
      return TRUE;

   // now grab the format
   nBytes = mmioRead(hmmio, (HPSTR) &waveFmt, (long) sizeof(waveFmt));
   if ( waveFmt.wFormatTag == WAVE_FORMAT_DVI_ADPCM ) {
      if ( nBytes != sizeof(waveFmt) )
         return TRUE;
      if ( waveFmt.cbSize < sizeof(sampsPerBlock) )
         return TRUE;
      nBytes = mmioRead(hmmio, (HPSTR) &sampsPerBlock, (long) sizeof(sampsPerBlock));
      pAttribs->samplesPerBlock = sampsPerBlock;
   }

   // get back out of the format chunk
   if(mmioAscend(hmmio, &chunk, 0))
      return TRUE;

   /*----------------------------------------
    * process fact chunk, or calc #samples
    *---------------------------------------*/
   // for ADPCM, get the fact chunk, which holds the number of samples
   if ( waveFmt.wFormatTag == WAVE_FORMAT_DVI_ADPCM ) {
      // get the format chunk
      chunk.ckid = mmioFOURCC('f', 'a', 'c', 't');
      // go back to beginning of riff file to handle out of order chunks
      mmioSeek(hmmio, riff.dwDataOffset + 4, SEEK_SET);  
      if (mmioDescend(hmmio, &chunk, &riff, MMIO_FINDCHUNK))
         return TRUE;
      if(chunk.cksize < sizeof(uint32) )
         return TRUE;
      nBytes = mmioRead(hmmio, (HPSTR) pNSamples, sizeof(long) );
      if(mmioAscend(hmmio, &chunk, 0))
         return TRUE;
      pAttribs->dataType = kSndDataIMAADPCM;
   }

   /*----------------------------
    * find data chunk
    *---------------------------*/
   // Find the data subchunk. The current file position should be at 
   // the beginning of the data chunk; however, you should not make 
   // this assumption. Use mmioDescend to locate the data chunk. 
   // go back to beginning of riff file to handle out of order chunks
   mmioSeek(hmmio, riff.dwDataOffset + 4, SEEK_SET);  
   chunk.ckid = mmioFOURCC('d', 'a', 't', 'a'); 
   if (mmioDescend(hmmio, &chunk, &riff, MMIO_FINDCHUNK)) 
      return TRUE;
 
   if (chunk.cksize == 0L)
      return TRUE;
 
   // now we look to see where we are
   if(mmioGetInfo(hmmio, pMmio, 0))
      return TRUE;

   *ppPCMData = (uint8 *)pMmio->pchNext;
	*pPCMLen = chunk.cksize;
   pAttribs->sampleRate = waveFmt.nSamplesPerSec;
   pAttribs->bitsPerSample = waveFmt.wBitsPerSample;
   pAttribs->nChannels = waveFmt.nChannels;
   pAttribs->bytesPerBlock = waveFmt.nBlockAlign;

   switch( waveFmt.wFormatTag ) {

      case WAVE_FORMAT_PCM:
         bytesPerSamp = ((pAttribs->nChannels * pAttribs->bitsPerSample) / 8);
         pAttribs->dataType = kSndDataPCM;
         // for PCM data types, calculate the #sample from #bytes
         *pNSamples = chunk.cksize / bytesPerSamp;
         pAttribs->samplesPerBlock = pAttribs->bytesPerBlock / bytesPerSamp;
         break;

      case WAVE_FORMAT_DVI_ADPCM:
         // the nSamples field in the fact chunk is often (usually!) wrong
         // so if nSamples is larger than the max # of samples in the data
         // chunk, limit nSamples to that
         nBlocks = *pPCMLen / pAttribs->bytesPerBlock;
         partialBlockLen = *pPCMLen % pAttribs->bytesPerBlock;
         maxSamples = nBlocks * pAttribs->samplesPerBlock;
         if ( partialBlockLen >= 4 ) {
            // there is a partial block, account for any samples in it
            // the block has a 4 byte header with 1 sample in it, and
            //   2 samples in each byte after the header
            maxSamples += ( 1 + (2 * (partialBlockLen - 4)) );
         }
         if ( *pNSamples > maxSamples ) {
            *pNSamples = maxSamples;
         }
         break;

      default:
         // we don't handle other formats
         return TRUE;
   }

   pAttribs->numSamples = *pNSamples;
   return FALSE;
}


//
// extract useful info from RIFF WAVE file image header
//  return TRUE if an error occurs
//
static BOOL
SndCrackWaveHeader(
                   void          *pRezData,
                   uint32        rezLen,
                   void          **ppData,
                   uint32        *pDataLen,
                   uint32        *pNumSamples,
                   sSndAttribs   *pAttribs )
{
   HMMIO      		hmmio;
   MMIOINFO       mmio;
   BOOL           bad;

   // set up to read from a memory file.  This is becuase
   // we have the whole file in memory at this point...
   memset(&mmio, 0, sizeof(MMIOINFO));

   mmio.pIOProc = NULL;
   mmio.fccIOProc = FOURCC_MEM;
   mmio.pchBuffer = (char *) pRezData;
   mmio.cchBuffer = rezLen;

   hmmio = mmioOpen(NULL, &mmio, MMIO_READWRITE);
   if(hmmio==NULL)
      return TRUE;

   // and point to all that good sound data
   bad = openWaveHeader( hmmio, &mmio, ppData, pDataLen,
                         pNumSamples, pAttribs );
   mmioClose(hmmio, 0);

   return bad;
}

//
// extract useful info from VOC file image header
//  return TRUE if an error occurs
//
static BOOL
SndCrackVocHeader(
                  void          *pRezData,
                  uint32        rezLen,
                  void          **ppData,
                  uint32        *pDataLen,
                  uint32        *pNumSamples,
                  sSndAttribs   *pAttribs )
{
   uint8 blockType;
   uint8 *pVoc = (uint8 *) pRezData;
   uint8 *pEndRez = pVoc + rezLen;
   BOOL voiceBlockFound = FALSE;
	uint32 sampleRate;
	int16 bits;
	int16 channels;

   sVocHeader *hdr = (sVocHeader *) pVoc;
   
   sVocDataBlock       *dataBlock;
//   voc_continue_block   *contBlock;
   sVocStereoBlock     *stereoBlock;
   sVocExtendedBlock  *extendedBlock;
   
   pVoc += hdr->offData; // fast forward to data

   while ( !voiceBlockFound ) {
      if ( pVoc >= pEndRez ) {
         // got to the end of VOC without finding a data block
         break;
      }

      blockType = *pVoc; // just read the id off the top.

      switch(blockType)
      {
         case VOC_BLOCK_TERM:
            return TRUE;
         case VOC_BLOCK_DATA:
            dataBlock = (sVocDataBlock *)pVoc;
				
				channels = 1;
				bits = 8;
				sampleRate = 1000000L / (256 - dataBlock->timeConstant);

            *ppData = pVoc + sizeof(sVocDataBlock);
				*pDataLen = VOC_BLOCK_LEN(pVoc) - 2;
				voiceBlockFound = TRUE;
            break;
         case VOC_BLOCK_STEREO:
            stereoBlock = (sVocStereoBlock *)pVoc;
            if(stereoBlock->voiceMode)
            {
               bits = 8;
					channels = 2;
					sampleRate = 
                  128000000L / (65536L - stereoBlock->timeConstant);
            }
            else
            {
               bits = 8;
					channels = 1;
					sampleRate = 
                  256000000L / (65536L - stereoBlock->timeConstant);
            }
            pVoc += VOC_BLOCK_LEN(pVoc) + 4; // go to the end of the block

            *ppData = pVoc + sizeof(sVocDataBlock);
				*pDataLen = VOC_BLOCK_LEN(pVoc) - 2;
            voiceBlockFound = TRUE;
            break;

		 case VOC_BLOCK_EXTENDED:
            extendedBlock = (sVocExtendedBlock *)pVoc;
            *ppData = pVoc + sizeof(sVocExtendedBlock);
				*pDataLen = VOC_BLOCK_LEN(pVoc) - 12;
				sampleRate = extendedBlock->sampleRate;
            channels = extendedBlock->channels;

            if(extendedBlock->format == 0)
					bits = 8;
            else if(extendedBlock->format == 4)
					bits = 16;
            voiceBlockFound = TRUE;
            break;
         default:
            pVoc += VOC_BLOCK_LEN(pVoc) + 4;
            break;
      }
   }  // end while !voiceBlockFound
	
   if ( voiceBlockFound == TRUE ) {
      // fill in sound attributes & samples-in-resource
      pAttribs->nChannels = channels;
      pAttribs->bitsPerSample = bits;
      pAttribs->sampleRate = sampleRate;
      pAttribs->bytesPerBlock = channels * (bits / 8);
      pAttribs->dataType = kSndDataPCM;
      pAttribs->samplesPerBlock = 1;
      *pNumSamples = *pDataLen / pAttribs->bytesPerBlock;
   }
   return !voiceBlockFound;
}

//
// get header info from sound resource (mem-resident sound file image)
// return TRUE if failure occurs
//
BOOL
SndCrackRezHeader(
                  void          *pRezData,
                  uint32        rezLen,
                  void          **ppData,
                  uint32        *pDataLen,
                  uint32        *pNumSamples,
                  sSndAttribs   *pAttribs )
{
   char *buf = (char *) pRezData;
   BOOL bad;

   // determine the file image type (WAVE or VOC)
   // extract sound attribs from rez header
   // find start of audio data
   if ( (strncmp( buf, "RIFF", 4) == 0 )
        && (strncmp( buf + 8, "WAVE", 4) == 0) ) {
      // sound resource is a wave file image
      bad = SndCrackWaveHeader( pRezData, rezLen, ppData, pDataLen,
                                 pNumSamples, pAttribs );
      if ( bad ) mprintf( "SndCrackWaveHeader returned error\n");
      TLOG3("SndCrackWaveHeader %ld bytes, %ld samples %d badFlag",
           rezLen, *pNumSamples, bad );
   } else if ( strncmp( buf, "Creative Voice File", 19 ) == 0 ) {
      // sound resource is a voc file image
      //TBD!
      bad = SndCrackVocHeader( pRezData, rezLen, ppData, pDataLen,
                               pNumSamples, pAttribs );
      if ( bad ) mprintf( "SndCrackVocHeader returned error\n");
      TLOG3("SndCrackVocHeader %ld bytes, %ld samples %d badFlag",
           rezLen, *pNumSamples, bad );
   } else {
      // Error - Rez is not a recognized sound file image type
      mprintf("Unrecognizable sound file type\n");
      bad = TRUE;
   }

   return bad;
}

