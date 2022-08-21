//############################################################################
//##                                                                        ##
//##  Miles Sound System                                                    ##
//##                                                                        ##
//##  MSSSTRM.C: High-level streaming API                                   ##
//##                                                                        ##
//##  Flat-model source compatible with IBM 32-bit ANSI C/C++               ##
//##                                                                        ##
//##  Version 1.00 of 19-Jan-97: Initial                                    ##
//##  Version 1.01 of 11-May-97: Added ADPCM support (Serge Plagnol)        ##
//##  Version 1.10 of 1-Jul-98: Added ASI codec support (John Miles)        ##
//##                                                                        ##
//##  Author: Jeff Roberts                                                  ##
//##                                                                        ##
//############################################################################
//##                                                                        ##
//##  Copyright (C) RAD Game Tools, Inc.                                    ##
//##                                                                        ##
//##  Contact RAD Game Tools at 425-893-4300 for technical support.         ##
//##                                                                        ##
//############################################################################

#ifdef ON_MAC_USE_FSS
#ifdef TARGET_API_MAC_CARBON
  #include <carbon.h>
#else
  #include <Files.h>
#endif
#endif

#if defined(_XENON) || (_XBOX_VER == 200) || defined(_XBOX)
#include <xtl.h>
#elif defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(__NT__) || defined(__WIN32__)
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#define STRICT
#include <windows.h>
#endif

#include <stdlib.h>
#include <stdio.h>

#include "mss.h"
#include "imssapi.h"

#ifdef IS_PS3
#include <sys/timer.h>
#include <sys/ppu_thread.h>
#include <sys/synchronization.h>
#endif

#ifdef IS_MACHO
#include <unistd.h>
#endif

//############################################################################
//##                                                                        ##
//## Locked code                                                            ##
//##                                                                        ##
//############################################################################

static HSTREAM streams=0;
static HTIMER timer=(HTIMER)-1;
static S32 noreenter=0;


#define BUFFERGRANULARITY (4*1024L)

#ifdef IS_DOS

static S32 locked = 0;

#define LOCK(x)   AIL_vmm_lock  (&(x),sizeof(x))
#define UNLOCK(x) AIL_vmm_unlock(&(x),sizeof(x))

void AILSTRM_end(void);

void AILSTRM_start(void)
{
   if (!locked)
      {
      AIL_vmm_lock_range(AILSTRM_start, AILSTRM_end);

      LOCK(streams);
      LOCK(timer);
      LOCK(noreenter);

      locked = 1;
      }
}

#define DOLOCK() AILSTRM_start()

#else

#define DOLOCK()

#endif

static S32 check_buffers( HSTREAM stream, S32 fillup );

#if defined(IS_WIN32API) || defined(IS_MAC) || defined(IS_LINUX) || defined(IS_PS2) || defined(IS_PS3) || defined(IS_WII)

static void handle_asyncs( HSTREAM stream, S32 wait)
{
  U32 amt;

  if (stream->asyncs[0])
  {
    if (MSS_async_status(stream->asyncs[0],wait,&amt))
    {
      if (amt==0)
        stream->error=1;
      stream->asyncs[0]=0;
    }
  }
  if (stream->asyncs[1])
  {
    if (MSS_async_status(stream->asyncs[1],wait,&amt))
    {
      if (amt==0)
        stream->error=1;
      stream->asyncs[1]=0;
    }
  }
  if (stream->asyncs[2])
  {
    if (MSS_async_status(stream->asyncs[2],wait,&amt))
    {
      if (amt==0)
        stream->error=1;
      stream->asyncs[2]=0;
    }
  }
}
#endif

//############################################################################
//##                                                                        ##
//## low level function to handle the buffer loads                          ##
//##                                                                        ##
//############################################################################

static S32 checkforaload(HSTREAM s)
{
  S32 num=-1;

  //
  // Check the async buffer for completion, if in effect
  //


#if defined(IS_WIN32API) || defined(IS_MAC) || defined(IS_LINUX) || defined(IS_PS2) || defined(IS_PS3) || defined(IS_WII)

  if (MSS_async_read)
  {
    handle_asyncs(s,0);
  }

#endif

  if (((s->size1) || (s->alldone==1)) && (s->playcontrol==1) && (s->asyncs[s->buf1]==0)) {

    if (s->preload) {

      AIL_set_sample_address(s->samp,s->bufs[0],s->size1);

      if (s->preloadpos)
      {
        AIL_set_sample_position(s->samp,s->preloadpos);
        s->preloadpos=0;
      }

      AIL_resume_sample(s->samp);

      s->noback=4; // prevents any more background processing
      s->loadedsome=1;

    } else {

      num=AIL_sample_buffer_ready(s->samp);

      if (num!=-1) {

        s->loadedorder[num]=s->loadorder++;
        s->loadedbufstart[num]=s->bufstart[s->buf1];

        if (s->reset_ASI[s->buf1])
        {
          // on a reset block, the true size is stored in the reset_ASI
          s->size1=s->reset_ASI[s->buf1]-1;
          s->reset_ASI[s->buf1]=0;
          AIL_request_EOB_ASI_reset(s->samp,num,s->reset_seek_pos[s->buf1]);
          AIL_load_sample_buffer(s->samp,num,s->bufs[s->buf1],s->size1);
        }
        else
        {
          AIL_load_sample_buffer(s->samp,num,s->bufs[s->buf1],s->size1);
        }

        AIL_start_sample(s->samp);

        s->loadedsome=1;

        if ((s->alldone==1) && (s->size1==0)) {
          s->alldone=2;

          if (s->callback)
            s->docallback=1;
        }

        s->buf1=s->buf2;
        s->buf2=s->buf3;
        if (++s->buf3==3)
          s->buf3=0;
        s->size1=s->size2;
        s->size2=s->size3;
        s->size3=0;

      }

    }

  }

  return(num);
}

//############################################################################
//##                                                                        ##
//## Background sound streaming function                                    ##
//##                                                                        ##
//############################################################################

static void AILLIBCALLBACK background(UINTa junk)
{
  HSTREAM s=streams;
  HSTREAM n;

  junk=junk;

  MSSLockedIncrement(noreenter);

  if (noreenter==1) {
    while (s) {

      MSSLockedIncrementPtr(s->noback);

      if (s->noback==1) {

        #if defined(IS_MAC) || defined(IS_PS2) || defined(IS_WII)

          if (s->autostreaming==2)
          {
            MSSLockedDecrementPtr(s->noback);
            check_buffers( s, 0 );   // check async buffers here, since we have no threads
            MSSLockedIncrementPtr(s->noback);
          }

        #endif

        if (checkforaload(s)!=-1)
          checkforaload(s);

        n =(HSTREAM)s->next;
        MSSLockedDecrementPtr(s->noback);
  
        if (s->docallback)
        {
          if (s->callback)
          {
            if (AIL_sample_status(s->samp)!=SMP_PLAYING) 
            {
              // unlock during the callback
              MSSLockedDecrement(noreenter);
  
              s->docallback=0;
              MSS_do_cb1( (AILSTREAMCB),s->callback,s->samp->driver->callingDS,
                            s);
                            
              // after the callback try to relock
              MSSLockedIncrement(noreenter);
              if (noreenter!=1)
                break;
            }
          }
        }
      }
      else
      {
        n =(HSTREAM)s->next;
        MSSLockedDecrementPtr(s->noback);
      }
      s=n;
    }
  }

  MSSLockedDecrement(noreenter);
}

#if defined(IS_WINDOWS) || defined(IS_MAC) || defined(IS_LINUX) || defined(IS_XBOX) || defined(IS_XENON) || defined(IS_PS2) || defined(IS_PS3) || defined(IS_WII)

#ifdef IS_WIN32API

//############################################################################
//##                                                                        ##
//## Background Win32 streaming thread                                      ##
//##                                                                        ##
//############################################################################

static HANDLE threadwait;
extern "C" HANDLE stream_thread_handle=0;
static S32 closing=0;

DWORD WINAPI stream_thread(LPVOID user)
{
  HSTREAM s;

  while (WaitForSingleObject(threadwait,(DWORD) AIL_preference[DIG_DS_FRAGMENT_SIZE])==WAIT_TIMEOUT)
  {
    MSSLockedIncrement(noreenter);
    if (noreenter==1)
    {
      s=streams;

      while ((!closing) && (s))
      {

        if (s->autostreaming==2)
        {
           AIL_API_service_stream(s,0);
        }

        s=(HSTREAM)s->next;
      }

    }

    MSSLockedDecrement(noreenter);
  }

  return(0);
}

#elif defined( IS_PS3 )

extern "C" { sys_ppu_thread_t stream_thread_handle=0; }
static volatile S32 threadexit;
static S32 closing=0;

void stream_thread(uint64_t user)
{
  HSTREAM s;

  while ( 1 ) 
  {
    if ( threadexit) break;
    sys_timer_usleep(1000*AIL_preference[DIG_DS_FRAGMENT_SIZE]);
    if ( threadexit) break;

    MSSLockedIncrement(noreenter);
    if (noreenter==1)
    {
      s=streams;

      while ((!closing) && (s))
      {

        if (s->autostreaming==2)
        {
           AIL_API_service_stream(s,0);
        }

        s=(HSTREAM)s->next;
      }

    }

    MSSLockedDecrement(noreenter);
  }
 done:
  sys_ppu_thread_exit( 0 );
}  


#elif defined( IS_MACHO )

static UINTa thread_buf[ TC_NUM ] = {0};
static S32 closing=0;

static void stream_thread( UINTa * data )
{
  MSSLockedIncrement(noreenter);
  if (noreenter==1)
  {
    HSTREAM s;
  
    s=streams;

    while ((!closing) && (s))
    {

      if (s->autostreaming==2)
      {
        AIL_API_service_stream(s,0);
      }

      s=(HSTREAM)s->next;
    }

  }

  MSSLockedDecrement(noreenter);
}

#endif

//############################################################################
//##                                                                        ##
//## Background sound streaming function                                    ##
//##                                                                        ##
//############################################################################

extern "C"
{

void stream_background(void)  // called by AIL_serve()
{
  HSTREAM s=streams;

  while (s)
  {

    if (s->autostreaming==2)
      AIL_API_service_stream(s,0);

    s=(HSTREAM)s->next;

  }
}

}

#endif

//############################################################################
//##                                                                        ##
//## Open the flags based on the sound type                                 ##
//##                                                                        ##
//############################################################################

static S32 setflags(HSTREAM stream,U32 type,U32 bits, U32 chans)
{
  type = type;
  if (bits==4) {

    stream->totallen&=~((4*chans)-1);
    stream->fileflags=DIG_PCM_SIGN;
#ifdef IS_XBOX
    if ( type == 0x69 )
      stream->filetype=DIG_F_XBOX_ADPCM_MASK;
    else
#endif
      stream->filetype=DIG_F_16BITS_MASK|DIG_F_ADPCM_MASK;

  } else {

     stream->totallen&=~((bits*chans/8)-1);

     if (bits==16) {
       stream->fileflags=DIG_PCM_SIGN;
       stream->filetype=DIG_F_16BITS_MASK;
     } else if (bits==8) {
       stream->fileflags=0;
       stream->filetype=0;
     } else
       return(0);

   }

   if (chans==2)
     stream->filetype|=DIG_F_STEREO_MASK;

   if (stream->filetype&DIG_F_ADPCM_MASK)
   {
     stream->padding = stream->totallen % stream->blocksize ;
     if(stream->padding)
       stream->padding = stream->blocksize - stream->padding ;
   } 
   else 
   {
#ifdef IS_XBOX
     if (stream->filetype&DIG_F_XBOX_ADPCM_MASK)
     {
       stream->blocksize = 36 * chans;
       stream->padding = stream->totallen % stream->blocksize ;
       if(stream->padding)
         stream->padding = stream->blocksize - stream->padding ;
     } 
     else 
#endif
       stream->padding = 0;
   }

   stream->padded = -1 ;

   AIL_set_sample_playback_rate(stream->samp,stream->filerate);
   SS_set_sample_type(stream->samp,stream->filetype,stream->fileflags);
   AIL_set_sample_adpcm_block_size(stream->samp,stream->blocksize);

   return(1);
}


//############################################################################
//##                                                                        ##
//## Open the WAVE file itself                                              ##
//##                                                                        ##
//############################################################################

static S32 openwavefile(HSTREAM stream, C8 const FAR *filename)
{
   U32 tag,i,pos,type;
   U32 bits,chans,rate;
   S32 tried_id3v2 = 0;

   //
   // Assume stream is uncompressed raw data
   //

   stream->block_oriented = 0;
   stream->using_ASI = 0;

   //
   // Search for ASI codec capable of processing this input file type
   //
   // If no specific provider available for this file type, try the default
   // .WAV file handler
   //

   HPROVIDER HP = RIB_find_file_provider("ASI codec",
                                         "Input file types",
                                          filename);

   if (HP)
      {
      //
      // Set sample format according to file name and contents
      //
      // Load first 4K of file contents to determine the output data format,
      // then rewind the file to its beginning
      //

      #if defined(IS_WII) || defined(IS_PS2) || defined(IS_PS3)
        U8 __attribute__ ((aligned(32))) temp_buffer[AIL_MAX_FILE_HEADER_SIZE];
      #else
        U8 temp_buffer[AIL_MAX_FILE_HEADER_SIZE];
      #endif

      stream->startpos = MSS_seek(stream->fileh,0,AIL_FILE_SEEK_CURRENT);
      stream->totallen = MSS_seek(stream->fileh,0,AIL_FILE_SEEK_END)-stream->startpos;
      MSS_seek(stream->fileh,stream->startpos,AIL_FILE_SEEK_BEGIN);

     domp3:

      i=MSS_read(stream->fileh,
                 temp_buffer,
                 AIL_MAX_FILE_HEADER_SIZE);

      if (!i)
         {
         return 0;
         }

      MSS_seek(stream->fileh,stream->startpos,AIL_FILE_SEEK_BEGIN);

      //
      // Set up to use specified ASI codec to decode data for mixer
      //
      // We must first set the address of the sample to the temp header
      // buffer, so the sample parameters (rate, width, channels, etc.) can
      // be determined correctly.  The buffered data is ignored (and
      // should be considered invalid) after openwavefile() returns.
      //

      AIL_set_sample_address(stream->samp,
                             temp_buffer,
                             i);

      AIL_set_sample_processor(stream->samp,
                               SP_ASI_DECODER,
                               HP);

      //
      // Get handle to this HSAMPLE's decompression pipeline stage
      //


      stream->ASI = &stream->samp->pipeline[SP_ASI_DECODER].TYPE.ASI;

      if (stream->ASI->stream==0)
      {
        if ( !tried_id3v2 )
          goto try_id3v2;
        return( 0 );
      }

      //
      // Log streamed file properties
      //
      // datarate=bytes consumed per second
      // filerate=final output sample rate
      //

      stream->blocksize = (U32)~0 ; stream->ASI->ASI_stream_property(stream->ASI->stream, stream->ASI->MIN_INPUT_BLOCK_SIZE, &stream->blocksize, 0, 0 );

      MSS_seek(stream->fileh,stream->startpos,AIL_FILE_SEEK_BEGIN);

      if (stream->ASI->DATA_START_OFFSET != (UINTa) ~0)
         {
         stream->startpos = (U32)~0; stream->ASI->ASI_stream_property(stream->ASI->stream, stream->ASI->DATA_START_OFFSET, &stream->startpos, 0, 0);
         MSS_seek(stream->fileh,stream->startpos,AIL_FILE_SEEK_BEGIN);

         // NB: in XMA, startpos is now 0x3c (past the wavehdr chunk)
         }

      if (stream->ASI->DATA_LEN != (UINTa) ~0)
         {
         stream->totallen = ~0; stream->ASI->ASI_stream_property(stream->ASI->stream, stream->ASI->DATA_LEN, &stream->totallen,0,0);
         }

      stream->ASI->ASI_stream_seek( stream->ASI->stream, 0 );     // Rewind stream and force output-buffer flush

      if (stream->ASI->STREAM_SEEK_POS != (UINTa) ~0)
         {
         U32 offset = 0;

         stream->ASI->ASI_stream_property(stream->ASI->stream,
                                          stream->ASI->STREAM_SEEK_POS,
                                          0,&offset,&offset);

         if (offset != 0) // if stream does not start immediately after the header for some reason,
            {             // seek to where it does begin
            MSS_seek(stream->fileh,stream->startpos+offset,AIL_FILE_SEEK_BEGIN);
            }
         }

      stream->filerate = -1; stream->ASI->ASI_stream_property(stream->ASI->stream, stream->ASI->OUTPUT_SAMPLE_RATE,&stream->filerate,0,0);
      stream->datarate = (U32)~0; stream->ASI->ASI_stream_property(stream->ASI->stream, stream->ASI->INPUT_BIT_RATE,&stream->datarate,0,0);
      stream->datarate /= 8;

      U32 chans = (U32)~0; stream->ASI->ASI_stream_property(stream->ASI->stream, stream->ASI->OUTPUT_CHANNELS,&chans,0,0);
      U32 bits  = (U32)~0; stream->ASI->ASI_stream_property(stream->ASI->stream, stream->ASI->OUTPUT_BITS,&bits,0,0);

      //
      // Reset sample position to throw away data resulting from temp buffer decode
      //

      stream->samp->pos[0] = 0;
      stream->samp->len[0] = 0;
      stream->samp->pos[1] = 0;
      stream->samp->len[1] = 0;

      if ((stream->datarate==0) || (chans==0))
      {
        if ( !tried_id3v2 )
          goto try_id3v2;
        return( 0 );
      }

      if (bits==16)
         {
         stream->fileflags = DIG_PCM_SIGN;
         stream->filetype  = DIG_F_16BITS_MASK;
         }
      else if (bits==8)
         {
         stream->fileflags = 0;
         stream->filetype  = 0;
         }

      if (chans==2)
         {
         stream->filetype |= DIG_F_STEREO_MASK;
         }

      //
      // Set padding value to ensure that remainder of final block is
      // filled with zeroes
      //

      stream->padding = stream->totallen % stream->blocksize;

      if (stream->padding)
         {
         stream->padding = stream->blocksize - stream->padding;
         }

      stream->padded = -1;

      //
      // Set flag to indicate block-oriented (i.e., compressed) stream
      //

      stream->using_ASI = 1;
      stream->block_oriented = 1;

      return 1;
      }
   else
      {
      //
      // ugly low-level wave file parse with a minimum of i/o
      //

      i=MSS_read(stream->fileh,&tag,4);

      if ( LE_SWAP32( &tag )==0x46464952) {  // 'FFIR'
         i=MSS_read(stream->fileh,&tag,4);
         i=MSS_read(stream->fileh,&tag,4);
         if ( LE_SWAP32( &tag )==0x45564157) {  // 'EVAW'
            i=MSS_read(stream->fileh,&tag,4);
            while ( LE_SWAP32( &tag )!=0x20746d66) {  // ' tmf'
              i=MSS_read(stream->fileh,&tag,4);
              if (i!=4)
                 return(0);
              tag=( LE_SWAP32( &tag )+1)&~1;
              MSS_seek(stream->fileh,tag,AIL_FILE_SEEK_CURRENT);
              i=MSS_read(stream->fileh,&tag,4);
            }
            i=MSS_read(stream->fileh,&pos,4);
            pos=LE_SWAP32( &pos )+ MSS_seek(stream->fileh,0,AIL_FILE_SEEK_CURRENT);

            i=MSS_read(stream->fileh,&type,4);

            MEM_LE_SWAP32( &type );

            chans=type>>16;

            type&=0xffff;
            i=MSS_read(stream->fileh,&rate,4);
            MEM_LE_SWAP32( &rate );

            i=MSS_read(stream->fileh,&tag,4);
            i=MSS_read(stream->fileh,&tag,4);
            MEM_LE_SWAP32( &tag );

            bits=tag>>16;
            stream->blocksize=tag&0xFFFF;

            MSS_seek(stream->fileh,pos,AIL_FILE_SEEK_BEGIN);
            i=MSS_read(stream->fileh,&tag,4);
            while ( LE_SWAP32( &tag )!=0x61746164)   // 'atad'
            {
              i=MSS_read(stream->fileh,&tag,4);
              if (i!=4)
                 return(0);
              tag=( LE_SWAP32( &tag )+1)&~1;
              MSS_seek(stream->fileh,tag,AIL_FILE_SEEK_CURRENT);
              i=MSS_read(stream->fileh,&tag,4);
            }

            i=MSS_read(stream->fileh,&stream->totallen,4);
            MEM_LE_SWAP32( &stream->totallen );
            stream->startpos=MSS_seek(stream->fileh,0,AIL_FILE_SEEK_CURRENT);

            stream->filerate=rate;
            stream->datarate=rate*bits*chans/8;

            if ((bits != 8) && (bits != 16))
               {
               stream->block_oriented = 1;
               }

            if ( (type!=1) 
               && ((type!=0x11) || (bits!=4))
#ifdef IS_XBOX
               && ((type!=0x69) || (bits!=4))
#endif
               )
            {
              HP=RIB_find_file_dec_provider( "ASI codec",
                                             "Input wave tag",
                                             type,
                                             "Output file types",
                                             ".raw");
              if (HP)
                goto domp3;

              return(0);
            }

            return(setflags(stream,type,bits,chans));
            }
         }
         else
         {
           U8 buffer[16];
          
          try_id3v2:
           tried_id3v2 = 1;

          MSS_seek(stream->fileh,stream->startpos,AIL_FILE_SEEK_BEGIN);

          i=MSS_read(stream->fileh,
                     buffer,
                     16 );

          if (!i)
          {
            return 0;
          }

          if ( ( buffer[ 0 ] == 0x49 ) && ( buffer[ 1 ] == 0x44 ) && ( buffer[ 2 ] == 0x33 ) &&
               ( buffer[ 3 ] < 0xff ) && ( buffer[ 4 ] < 0xff ) &&
               ( buffer[ 6 ] < 0x80 ) && ( buffer[ 7 ] < 0x80 ) && ( buffer[ 8 ] < 0x80 ) && ( buffer[ 9 ] < 0x80 ) )
          {
            stream->startpos = 10 + ( (U32) buffer[ 9 ] ) | ( ( (U32) buffer[ 8 ] ) << 7 ) | ( ( (U32) buffer[ 7 ] ) << 14 ) | ( ( (U32) buffer[ 6 ] ) << 21 );
            stream->totallen -= stream->startpos;
            MSS_seek(stream->fileh,stream->startpos,AIL_FILE_SEEK_BEGIN);
            goto domp3;
          }

          return( 0 );
         }
      }
  return(0);
}

//############################################################################
//##                                                                        ##
//## Open a streaming file                                                  ##
//##                                                                        ##
//############################################################################

HSTREAM  AILCALL AIL_API_open_stream(HDIGDRIVER dig, char const * filename, S32 stream_mem)
{
   HSTREAM stream;
   S32 mini;

   DOLOCK();
   if ((dig==0) || (filename==0))
     return(0);

   //
   // Allocate memory for STREAM structure
   //

   stream = (HSTREAM) AIL_mem_alloc_lock(sizeof(*stream));

   if (stream == 0)
      {
      AIL_set_error("Out of memory.");
      return 0;
      }

   AIL_memset(stream, 0, sizeof(*stream));

   OutMilesMutex();

   #ifdef ON_MAC_USE_FSS
   
   MSS_FILE file;
   file.file_type=0;
   file.file=filename;
   if (!MSS_open(&file,&stream->fileh))
     stream->fileh=(U32)-1;
   
   #else
   
   if (!MSS_open(filename,&stream->fileh))
     stream->fileh=(U32)-1;
   
   #endif
   
   InMilesMutex();

   if (stream->fileh==(U32)-1)
      {
      AIL_set_error("Unable to open file.");
   error:
      AIL_mem_free_lock(stream);
      return(0);
      }

   stream->samp=AIL_allocate_sample_handle(dig);

   if (stream->samp==0)
      {
      AIL_set_error("Unable to obtain a sample handle.");
   errorclose:
      if (stream->fileh!=(U32)-1)
        MSS_close(stream->fileh);
      goto error;
      }

   AIL_init_sample(stream->samp, DIG_F_MONO_16, 0);   // temporarily establish a valid format

   OutMilesMutex();

   if (!openwavefile(stream,filename))
      {
      InMilesMutex();
      AIL_set_error("Error getting sound format.");
     releaseclose:
      AIL_release_sample_handle(stream->samp);
      goto errorclose;
      }

   InMilesMutex();

   //
   // figure bufsize and do mallocs
   //

   mini=AIL_minimum_sample_buffer_size(dig, stream->filerate, stream->filetype);

   if (stream_mem==0) {
     stream->bufsize=stream->datarate/3;
     if (stream->bufsize<mini)
       stream->bufsize=mini;
     stream->bufsize=(stream->bufsize+(BUFFERGRANULARITY-1))&~(BUFFERGRANULARITY-1);
   } else
     stream->bufsize=((stream_mem/3)+(BUFFERGRANULARITY-1))&~(BUFFERGRANULARITY-1);

   if (stream->bufsize==0)
     stream->bufsize=BUFFERGRANULARITY;

   if ((S32)(stream->totallen-(stream->bufsize*3))<(S32)(BUFFERGRANULARITY*3)) {

     stream->primeamount=stream->bufsize=stream->totallen;
     stream->bufs[0]=(U8*)AIL_mem_alloc_lock(stream->totallen);
     if (stream->bufs[0]==0)
       goto allocclose;

     stream->preload=1;

   } else {

     stream->primeamount=mini*2;
     if (stream->primeamount<(stream->bufsize*2))
       stream->primeamount=stream->bufsize*2;

     stream->bufs[0]=(U8*)AIL_mem_alloc_lock(stream->bufsize+64);
     if (stream->bufs[0]==0) {
      allocclose:
       AIL_set_error("Out of memory.");
       goto releaseclose;
     }

     stream->bufs[1]=(U8*)AIL_mem_alloc_lock(stream->bufsize+64);
     if (stream->bufs[1]==0) {
       AIL_mem_free_lock(stream->bufs[0]);
       goto allocclose;
     }

     stream->bufs[2]=(U8*)AIL_mem_alloc_lock(stream->bufsize+64);
     if (stream->bufs[2]==0) {
      freeclose:
       AIL_mem_free_lock(stream->bufs[0]);
       AIL_mem_free_lock(stream->bufs[1]);
       goto allocclose;
     }
   }


#if !defined(IS_DOS)
   
   //
   // setup the buffering callback
   //

   AIL_register_EOB_callback(stream->samp,(AILSAMPLECB)background);
   stream->samp->system_data[STR_HSTREAM] = (UINTa)stream;

   if (MSS_async_read)
     stream->readsize=stream->bufsize;
   else

#endif
     stream->readsize=stream->bufsize/2;

   stream->sublen=0x7fffffff;
   stream->substart=0;

   stream->buf2=1;
   stream->buf3=2;
   stream->loopsleft=1;
   stream->readatleast=stream->primeamount;
   
   #if defined(IS_WIN32API) || defined(IS_LINUX) || defined(IS_MACHO) || defined(IS_PS3)
   stream->autostreaming = 1;
   #elif defined(IS_MAC) || defined(IS_PS2) || defined(IS_WII)
   if ( MSS_async_read )
     stream->autostreaming = 1;
   #endif

   //
   // insert this stream into the linked list
   //

   if (streams==0) {
     //
     // start background timer;
     //

     timer=AIL_register_timer( background );
     if (timer==(HTIMER)-1) {
       AIL_set_error("Out of timer handles.");
       AIL_mem_free_lock(stream->bufs[2]);
       goto freeclose;
     }

     AIL_set_timer_frequency(timer,16);

     AIL_start_timer(timer);

#ifdef IS_WIN32API

     // create the I/O thread
     if (stream_thread_handle==0)
     {
       threadwait=CreateEvent(0,TRUE,0,0);

       stream_thread_handle=CreateThread(0,65536,stream_thread,0,0,(LPDWORD)&mini);
     
#if !defined(IS_XBOX)
       Set_thread_name( mini, "Miles Stream" );
#endif

     }

#elif defined( IS_PS3 )
  if ( stream_thread_handle==0)
  {
    threadexit=0;
    sys_ppu_thread_create( &stream_thread_handle, stream_thread, 0, 651, 32*1024, SYS_PPU_THREAD_CREATE_JOINABLE, "Miles stream Thread");
  }
  
#elif defined( IS_MACHO )

  if ( ! valid_thread_callback( thread_buf ) )
  {
    create_thread_callback( thread_buf, AIL_preference[DIG_SM_FRAGMENT_SIZE], stream_thread );
    unlock_thread_callback( thread_buf );
  }

#endif

     streams=stream;

   } else {
     stream->next=(void*)streams;
     streams=stream;
   }

   return(stream);
}

//############################################################################
//##                                                                        ##
//## Close a streaming file                                                 ##
//##                                                                        ##
//############################################################################

void  AILCALL AIL_API_close_stream(HSTREAM stream)
{
  if (stream)
  {
    //
    // lock out the streaming thread
    //

    MSSLockedIncrement(noreenter);
#if defined(IS_WIN32API) || defined(IS_PS3) || defined( IS_MACHO )
    MSSLockedIncrement(closing);
#endif

    if (stream->samp)
    {
      HSAMPLE samp=stream->samp;
      stream->samp=0;
      AIL_release_sample_handle(samp);
    }

    //
    // remove from the linked list
    //

    if (streams==stream)
      streams=(HSTREAM)streams->next;
    else
    {
      HSTREAM s=streams;
      while (s)
      {
        if (s->next==(void*)stream)
        {
          s->next=stream->next;
          break;
        }
        s=(HSTREAM)s->next;
      }
    }

    if (streams==0)
    {
      AIL_release_timer_handle(timer);
      timer=0;
 
      // (thread is closed in shutdown)
    }
#ifdef IS_WIN32API

    // if the background thread is running, synch up
    if (noreenter!=1)
    {
      OutMilesMutex();

      // wait until background thread syncs up (if there are still threads playing)
      while (noreenter!=1)
      {
        Sleep(1);
      }

      InMilesMutex();
    }

#elif defined( IS_PS3 )

    // if the background thread is running, synch up
    if (noreenter!=1)
    {
      OutMilesMutex();

      // wait until background thread syncs up (if there are still threads playing)
      while (noreenter!=1)
      {
        sys_timer_usleep(500);
      }

      InMilesMutex();
    }

#elif defined( IS_MACHO )

    // if the background thread is running, synch up
    if (noreenter!=1)
    {
      OutMilesMutex();

      // wait until background thread syncs up (if there are still threads playing)
      while (noreenter!=1)
      {
        usleep(500);
      }

      InMilesMutex();
    }

#endif

#if defined(IS_WIN32API) || defined(IS_PS3) || defined(IS_MACHO)
    MSSLockedDecrement(closing);
#endif
    MSSLockedDecrement(noreenter);
    
    //
    // Wait for the async I/O to finish...
    //

#if defined(IS_WIN32API) || defined(IS_MAC) || defined(IS_LINUX) || defined(IS_PS2) || defined(IS_PS3) || defined(IS_WII)

    if (MSS_async_read)
    {
      handle_asyncs(stream,1);
    }

#endif

    if (stream->bufs[0])
      AIL_mem_free_lock(stream->bufs[0]);

    if (stream->bufs[1])
      AIL_mem_free_lock(stream->bufs[1]);

    if (stream->bufs[2])
      AIL_mem_free_lock(stream->bufs[2]);

    if (stream->fileh!=(U32)-1)
    {
      OutMilesMutex();
      MSS_close(stream->fileh);
      InMilesMutex();
    }

    AIL_memset(stream,0,sizeof(*stream));
    AIL_mem_free_lock(stream);

  }
}


//############################################################################
//##                                                                        ##
//## Gets the status of a streaming file                                    ##
//##                                                                        ##
//############################################################################

S32  AILCALL AIL_API_stream_status(HSTREAM stream)
{
  if (stream)
  {
    if (stream->error==0)
    {
      if ((stream->playcontrol==1) && (!stream->loadedsome))
        return( SMP_PLAYING );
      else
        return( AIL_sample_status(stream->samp) );
    }
  }
  return(-1);
}

//############################################################################
//##                                                                        ##
//## low-level buffer reading                                               ##
//##                                                                        ##
//############################################################################

static S32 doread(HSTREAM stream,S32 cur, S32* size)
{
  U32 amt,read;

  amt=stream->totallen-stream->totalread;
  if (amt > (U32)(stream->readsize))
    amt=stream->readsize;
  read=stream->bufsize-*size;
  if (amt>read)
    amt=read;

  read=stream->sublen-stream->totalread;
  if (amt > read)
    amt=read;

  if (amt)
  {
#if defined(IS_WIN32API) || defined(IS_MAC) || defined(IS_LINUX) || defined(IS_PS2) || defined(IS_PS3) || defined(IS_WII)
    if (MSS_async_read)
    {
      //
      // Only launch one async buffer per buffer
      //
      U32 align_to_next_2k = ( stream->totalread+stream->startpos ) & 2047;
      if ( ( align_to_next_2k < amt ) && ( (amt-align_to_next_2k) > 2048 ) )
        amt -= align_to_next_2k;
      stream->asyncs[cur]=MSS_async_read(stream->fileh,stream->bufs[cur]+*size,amt);
      read=amt;
    }
    else
#endif
    {
      read=MSS_read(stream->fileh,stream->bufs[cur]+*size,amt);
    }
  }
  else
    read=0;

  if ((*size)==0)
    stream->bufstart[cur]=stream->totalread;

  stream->totalread+=read;
  *size+=read;

  if (read!=amt)
    stream->error=1;

  //
  // are we at the end of the subblock loop?
  //

  if (stream->sublen==(S32)stream->totalread)
  {
    if (!stream->block_oriented) //filetype&DIG_F_ADPCM_MASK))
      goto endofdata;

    if (stream->padded==-1)
      stream->padded=0;

    amt = stream->subpadding-stream->padded;
    if (amt > (U32)(stream->bufsize-*size))
      amt = (stream->bufsize-*size);

    AIL_memset(stream->bufs[cur]+*size, 0, amt) ;
    read+=amt;
    *size+=amt;
    stream->padded+=amt;

    if(stream->padded==stream->subpadding)  //end of padding?
    {
     endofdata:
      
      if (stream->loopsleft)
        --stream->loopsleft;

      if (stream->loopsleft==1)
        stream->sublen=0x7fffffff;
      
      stream->padded=-1;
      stream->totalread=stream->substart;

      MSS_seek(stream->fileh,stream->substart+stream->startpos,AIL_FILE_SEEK_BEGIN);

      //
      // If looping back, flush the ASI output buffer
      //

      if (stream->using_ASI)
      {
        // save the true size in reset_ASI
        stream->reset_ASI[cur]=(*size)+1;
        stream->reset_seek_pos[cur] = stream->substart;
        // mark the buffer as full
        *size=stream->bufsize;
      }
    }

  }

  //
  // are we at the end of the file?
  //

  if (stream->totallen==(S32)stream->totalread) {
    if (stream->loopsleft==1)
      stream->alldone=1;
    else {

      if (!(stream->block_oriented)) //filetype&DIG_F_ADPCM_MASK))
        goto endofsubloop;

      if (stream->padded==-1)
        stream->padded=0;

      amt = stream->padding-stream->padded;
      if (amt > (U32)(stream->bufsize-*size))
        amt = (stream->bufsize-*size);

      AIL_memset(stream->bufs[cur]+*size, 0, amt) ;
      read+=amt;
      *size+=amt;
      stream->padded+=amt;

      if(stream->padded==stream->padding) { //end of padding?

       endofsubloop:
        if(stream->loopsleft)
          --stream->loopsleft;

        stream->padded=-1;
        stream->totalread=0;
        MSS_seek(stream->fileh,stream->startpos,AIL_FILE_SEEK_BEGIN);

        //
        // If looping back, flush the ASI output buffer
        //

        if (stream->using_ASI)
        {
          // save the true size in reset_ASI
          stream->reset_ASI[cur]=(*size)+1;
          stream->reset_seek_pos[cur] = 0;
          // mark the buffer as full
          *size=stream->bufsize;
        }
      }

    }
  }

  return(read);
}


static S32 check_buffers( HSTREAM stream, S32 fillup )
{
  U32 pos0,len0,pos1,len1;
  int donext=0;
  int donext1=0;
  S32 amt;
  S32 tamt=0;

  MSSLockedIncrementPtr(stream->noback);

   if (stream->noback!=1)
      {
      MSSLockedDecrementPtr(stream->noback);
      return 0;
      }

  if ( AIL_API_sample_buffer_info(stream->samp,&pos0,&len0,&pos1,&len1) )
  {
    fillup = 1;  // if we stall, completely reload the buffer
  }

   MSSLockedDecrementPtr(stream->noback);

  if ((pos0==len0) && (pos1==len1)) {

    stream->readatleast=stream->primeamount;
    donext=1;
    donext1=1;

  } else if ((pos0==len0) || (pos1==len1)) {

    stream->readatleast=stream->primeamount;
    donext=1;

  }

  if (stream->asyncs[stream->buf1]==0)
  {
    do {
      amt=doread(stream,stream->buf1,&stream->size1);
      if (stream->error)
        return(-1);
      tamt+=amt;
    } while ((amt) && (stream->asyncs[stream->buf1]==0) && ((tamt<stream->readatleast) || (fillup)));
  }

  if ((donext) && (stream->asyncs[stream->buf2]==0)) {
    do {
      amt=doread(stream,stream->buf2,&stream->size2);
      if (stream->error)
        return(-1);
      tamt+=amt;
    } while ((amt) && (stream->asyncs[stream->buf2]==0) && ((tamt<stream->readatleast) || (fillup)));
  }

  if ((donext1) && (stream->asyncs[stream->buf3]==0)) {
    do {
      amt=doread(stream,stream->buf3,&stream->size3);
      if (stream->error)
        return(-1);
      tamt+=amt;
    } while ((amt) && (stream->asyncs[stream->buf3]==0) && ((tamt<stream->readatleast) || (fillup)));
  }

  stream->readatleast=0;

  if ((stream->preload) && (stream->fileh!=(U32)-1)) {
    #if defined(IS_WINDOWS) || defined(IS_MAC) || defined(IS_LINUX) || defined(IS_XBOX) || defined(IS_XENON) || defined(IS_PS2) || defined(IS_PS3) || defined(IS_WII)
    stream->autostreaming=0;
    // leave the movie open if we will be waiting on an async IO
    if (MSS_async_read == 0)
    #endif
    {
      MSS_close(stream->fileh);
      stream->fileh=(U32)-1;
    }
  }

  #if defined(IS_WINDOWS) || defined(IS_MAC) || defined(IS_LINUX) || defined(IS_XBOX) || defined(IS_XENON) || defined(IS_PS2) || defined(IS_PS3) || defined(IS_WII)
  if (stream->autostreaming==1) // switch to full autostreaming when
    stream->autostreaming=2;    //    started (from standby auto)
  #endif
  
  return( tamt );
}

//############################################################################
//##                                                                        ##
//## Service a streaming file                                               ##
//##                                                                        ##
//############################################################################

static S32 noreenterserve=0;

S32  AILCALL AIL_API_service_stream(HSTREAM stream, S32 fillup)
{
  S32 tamt=0;

  if ((stream) && (stream->samp))
  {
    MSSLockedIncrement(noreenterserve);

    if (noreenterserve==1) {

      if ((stream->error==0) && (stream->alldone==0) && (stream->samp))
         {
         tamt = check_buffers( stream, fillup );

         MSSLockedDecrement(noreenterserve);

         return(tamt);
         }

    }

    MSSLockedDecrement(noreenterserve);

  }

  return(-1);
}


// align position for a seek
static S32 stream_align(HSTREAM stream,S32 offset)
{
  if (stream->using_ASI)
   {
   return offset;
   }

  if (stream->block_oriented)
  {
    S32 aligned ;

    aligned = (offset / stream->blocksize) * stream->blocksize;

    if((offset - aligned) > ((S32)stream->blocksize/2))
    {
      aligned += stream->blocksize ;
    }

    offset = aligned ;
  }
  else
  {
    offset = offset & ~((((stream->filetype&DIG_F_16BITS_MASK)?2:1)*((stream->filetype&DIG_F_STEREO_MASK)?2:1))-1);
  }

  if (offset>stream->totallen)
  {
    offset=stream->totallen;
  }

  return( offset );
}


//############################################################################
//##                                                                        ##
//## Set the position of a streaming file                                   ##
//##                                                                        ##
//############################################################################

void  AILCALL AIL_API_set_stream_position(HSTREAM stream,S32 offset)
{
  if (stream) {

    stream->padded = -1 ;

    offset=stream_align(stream,offset);

    if ((stream->using_ASI) && (stream->ASI->STREAM_SEEK_POS != (UINTa) ~0))
       {
       //
       // Tell codec our seek position (and give it a chance to align it)
       //
       // Note that offset 0 is at the end of a WAVEHDR, ID3 tag, or other
       // metadata at the beginning of the stream file image
       //

       stream->ASI->ASI_stream_property(stream->ASI->stream,
                                        stream->ASI->STREAM_SEEK_POS,
                                        0,&offset,&offset);
       }


    // if we skip past the final block, then don't pad
    if (offset>=stream->totallen)
    {
      stream->padded = stream->padding;
    }

    //
    // ASI codecs do not require block alignment on seeks (but they may
    // implement it internally by skipping data until the start of
    // the next block)
    //

    if (stream->preload) {

      if (stream->loadedsome)
        AIL_set_sample_position(stream->samp,offset);
      else
        stream->preloadpos=offset;

    } else {

      MSSLockedIncrementPtr(stream->noback);

#if defined(IS_WIN32API)
      while (stream->noback != 1)
      {
        Sleep(1);
      }
#endif

#if defined(IS_PS3)
      while (stream->noback != 1)
      {
        sys_timer_usleep(500);
      }
#endif

#if defined(IS_MACHO)
      while (stream->noback != 1)
      {
        usleep(500);
      }
#endif

      //
      // If stream has been started, re-initialize it to
      // reset buffer state
      //
      // Save and restore pipeline providers, temporarily clearing provider
      // list to keep AIL_init_sample() from closing active sample handles
      //
      // If ASI codec in use, force frame resync at next read operation
      //

      if (stream->loadedsome)
         {
         SMPATTRIBS save;
         AIL_save_sample_attributes(stream->samp, &save);

         SPINFO pipeline_save[N_SAMPLE_STAGES];
         S32 n_active_filters = stream->samp->n_active_filters;

         AIL_memcpy(pipeline_save,
                    stream->samp->pipeline,
                    sizeof(pipeline_save));

         AIL_memset(stream->samp->pipeline,
                    0,
                    sizeof(stream->samp->pipeline));

         AIL_end_sample(stream->samp);
         AIL_init_sample(stream->samp,stream->filetype,stream->fileflags);

         AIL_memcpy(stream->samp->pipeline,
                    pipeline_save,
                    sizeof(pipeline_save));
         stream->samp->n_active_filters = n_active_filters;

         AIL_load_sample_attributes(stream->samp, &save);
         AIL_set_sample_adpcm_block_size(stream->samp,stream->blocksize);

         if (stream->using_ASI)
            {
            stream->ASI->ASI_stream_seek(stream->ASI->stream, -1);
            }
         }

      stream->totalread=offset;

      MSS_seek(stream->fileh,stream->startpos+offset,AIL_FILE_SEEK_BEGIN);

      //
      // Wait for the async I/O to finish...
      //

#if defined(IS_WIN32API) || defined(IS_MAC) || defined(IS_LINUX) || defined(IS_PS2) || defined(IS_PS3) || defined(IS_WII)

      if (MSS_async_read)
      {
        handle_asyncs(stream,1);
      }

#endif

      stream->buf1=0;
      stream->buf2=1;
      stream->buf3=2;
      stream->size1=0;
      stream->size2=0;
      stream->size3=0;
      stream->readatleast=stream->primeamount;
      stream->loadedsome=0;
      if (stream->playcontrol!=1)
        stream->playcontrol=0;
      stream->alldone=0;

      MSSLockedDecrementPtr(stream->noback);
    }
  }
}

//############################################################################
//##                                                                        ##
//## Get the position of a streaming file                                   ##
//##                                                                        ##
//############################################################################
S32  AILCALL AIL_API_stream_position(HSTREAM stream)
{
  U32 pos0,len0,pos1,len1;
  U32 lo0, lo1, lbs0, lbs1;
  S32 ret;

  if (stream) {
    if (stream->error==0) {

      if (stream->preload)
        return( AIL_sample_position(stream->samp) );

      // lock every out and then record the current buffer settings
      MSSLockedIncrementPtr(stream->noback);

#ifdef IS_WIN32API
      while (stream->noback != 1)
      {
        Sleep(1);
      }
#endif

#if defined(IS_PS3)
      while (stream->noback != 1)
      {
        sys_timer_usleep(500);
      }
#endif


#if defined(IS_MACHO)
      while (stream->noback != 1)
      {
        usleep(500);
      }
#endif

      lo0=stream->loadedorder[0];
      lo1=stream->loadedorder[1];
      lbs0=stream->loadedbufstart[0];
      lbs1=stream->loadedbufstart[1];
      AIL_sample_buffer_info(stream->samp,&pos0,&len0,&pos1,&len1);

      MSSLockedDecrementPtr(stream->noback);

      // are we on the first buffer or the second?
      if ( lo0 < lo1 )
      {
        // if we've used this buffer and the next one has data in it...
        if ( ( pos0 == len0 ) && ( pos1 != len1 ) )
          goto buf2;

        ret = lbs0 + pos0;
      }
      else
      {
       buf2: 
        ret = lbs1 + pos1;
      }

      if ( ret > stream->totallen )
        ret = ret % stream->totallen;

      return( ret );
    
    }
  }

  return(-1);
}

//############################################################################
//##                                                                        ##
//## Start the playback of a streaming file                                 ##
//##                                                                        ##
//############################################################################

void  AILCALL AIL_API_start_stream(HSTREAM stream)
{
  if (stream) {

    #if defined(IS_WINDOWS) || defined(IS_MAC) || defined(IS_LINUX) || defined(IS_XBOX) || defined(IS_XENON) || defined(IS_PS2) || defined(IS_PS3) || defined(IS_WII)
    if (stream->autostreaming==1)      // switch to full autostreaming when
      stream->autostreaming=2;         //    started (from standby auto)
    #endif

    AIL_request_EOB_ASI_reset(stream->samp,(U32)~0,0);  // Cancel any pending reset requests

    if (stream->preload) {

      if (stream->loadedsome)
        AIL_start_sample(stream->samp);

    } else {
      if (stream->loadedsome) {
        AIL_stop_sample(stream->samp);
        AIL_set_stream_position(stream,0);
      }
      OutMilesMutex();
      background(0);
      InMilesMutex();
    }

    stream->playcontrol=1;
    OutMilesMutex();
    background(0);
    InMilesMutex();
  }
}


//############################################################################
//##                                                                        ##
//## Pause the playback of a streaming file                                 ##
//##                                                                        ##
//############################################################################

void  AILCALL AIL_API_pause_stream(HSTREAM stream, S32 onoff)
{
  if (stream)
    if (onoff) {

      //
      // turn on pause
      //

      if (AIL_stream_status(stream)==SMP_PLAYING) {
        AIL_stop_sample(stream->samp);
        stream->playcontrol|=24; // 8 = stopped, 16=stopped the sample
      } else
        stream->playcontrol|=8;

    } else {

      //
      // turn off pause
      //

      if ((stream->playcontrol&7)==0)
        AIL_start_stream(stream);
      else {
        if (stream->playcontrol&16)
          AIL_resume_sample(stream->samp);
        stream->playcontrol&=~24;
        OutMilesMutex();
        background(0);
        InMilesMutex();
      }

   }
}

//############################################################################
//##                                                                        ##
//## Changes the loop count of a streaming file                             ##
//##                                                                        ##
//############################################################################

void  AILCALL AIL_API_set_stream_loop_count(HSTREAM stream, S32 count)
{

  if (stream)
    if (stream->preload)
      AIL_set_sample_loop_count(stream->samp,count);
    else
      stream->loopsleft=count;

}


//############################################################################
//##                                                                        ##
//## Changes the loop sub block of a streaming file                         ##
//##                                                                        ##
//############################################################################

void  AILCALL AIL_API_set_stream_loop_block(HSTREAM stream,
                                            S32     loop_start_offset,
                                            S32     loop_end_offset)
{

  if (stream)
    if (stream->preload)
      AIL_set_sample_loop_block(stream->samp,loop_start_offset,loop_end_offset);
    else
    {
      if ((loop_start_offset==0) && (loop_end_offset==-1))
      {
        stream->substart=0;
        stream->sublen=0x7fffffff;
      }
      else
      {
        stream->substart=stream_align(stream,loop_start_offset);
        if (loop_end_offset==-1)
          loop_end_offset=stream->totallen;
        stream->sublen=stream_align(stream,loop_end_offset);
      }

      if (stream->filetype&DIG_F_ADPCM_MASK)
      {
        stream->subpadding = stream->sublen % stream->blocksize ;
        if(stream->subpadding)
          stream->subpadding = stream->blocksize - stream->subpadding ;
      } else
        stream->subpadding = 0 ;

    }

}


//############################################################################
//##                                                                        ##
//## Gets the remaining loop count of a streaming file                      ##
//##                                                                        ##
//############################################################################

S32  AILCALL AIL_API_stream_loop_count(HSTREAM stream)
{

  if (stream)
    if (stream->preload)
      return( AIL_sample_loop_count(stream->samp) );
    else
      return(stream->loopsleft);
  return(-1);

}


//############################################################################
//##                                                                        ##
//## Gets buffer info for the streaming file                                ##
//##                                                                        ##
//############################################################################

void  AILCALL AIL_API_stream_info(HSTREAM stream, S32* datarate, S32* sndtype, S32* length, S32* memory)
{
  if (stream) {

    if (datarate)
      *datarate=stream->filerate;
    if (sndtype)
      *sndtype=stream->filetype|((stream->using_ASI)?DIG_F_USING_ASI:0);
    if (length)
      *length=stream->totallen;
    if (memory)
      *memory=stream->bufsize*((stream->preload)?1:3);
  }

}


static void AILLIBCALLBACK preload_callback_broker(HSAMPLE samp)
{
  HSTREAM s=(HSTREAM)samp->system_data[STR_HSTREAM];

  if (s)
    MSS_do_cb1( (AILSTREAMCB),s->callback,s->samp->driver->callingDS,
                 s);
}

//############################################################################
//##                                                                        ##
//## Sets the end of stream callback                                        ##
//##                                                                        ##
//############################################################################

AILSTREAMCB  AILCALL AIL_API_register_stream_callback(HSTREAM stream, AILSTREAMCB callback)
{
  AILSTREAMCB cb=(AILSTREAMCB)-1;

  if (stream) {
    cb=stream->callback;

    if (stream->preload)
    {
      if ( callback )
        AIL_register_EOS_callback(stream->samp,preload_callback_broker);
      else
        AIL_register_EOS_callback(stream->samp,0);
    }

    stream->callback=callback;
  }

  return(cb);
}

//############################################################################
//##                                                                        ##
//## Sets the user data values                                              ##
//##                                                                        ##
//############################################################################

void  AILCALL AIL_API_set_stream_user_data(HSTREAM stream, U32 index, SINTa value)
{
  if (stream)
    stream->user_data[index]=value;
}


//############################################################################
//##                                                                        ##
//## Gets the user data values                                              ##
//##                                                                        ##
//############################################################################

SINTa  AILCALL AIL_API_stream_user_data(HSTREAM stream, U32 index)
{
  if (stream)
    return(stream->user_data[index]);

  return(0);
}

#if defined(IS_WINDOWS) || defined(IS_MAC) || defined(IS_LINUX) || defined(IS_XBOX) || defined(IS_XENON) || defined(IS_PS2) || defined(IS_PS3) || defined(IS_WII)

//############################################################################
//##                                                                        ##
//## Turns auto serving on off                                              ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_auto_service_stream(HSTREAM stream, S32 onoff)
{
  if (stream)
#if defined(IS_MAC) || defined(IS_PS2) || defined(IS_WII)
    stream->autostreaming=((onoff) && (MSS_async_read))?2:0;
#else
    stream->autostreaming=(onoff)?2:0;
#endif
}

#endif

//############################################################################
//##                                                                        ##
//## Get size and current position of stream in milliseconds                ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_stream_ms_position(HSTREAM    S, //)
                                S32 FAR   *total_milliseconds,
                                S32 FAR   *current_milliseconds)
{
   U32 datarate;

   if (S) 
      {
      if (S->filetype & DIG_F_ADPCM_MASK)
         {
         U32 samples_per_block = 4 << ((S->filetype&DIG_F_STEREO_MASK)?1:0);

         samples_per_block = 1 + (S->blocksize-samples_per_block)*8 / samples_per_block;

         datarate=(S->filerate * S->blocksize)/samples_per_block;
         }
      else
         {
         if (S->block_oriented)
            {
            datarate = S->datarate;
            }
         else
            {
            datarate = S->filerate *
               ((S->filetype&DIG_F_STEREO_MASK)?2:1) *
               ((S->filetype&DIG_F_16BITS_MASK)?2:1);
            }
         }

      if (total_milliseconds)
         *total_milliseconds=(datarate==0)?1:((S32)(((float)S->totallen*1000.0F)/(float)datarate));

      if (current_milliseconds)
         *current_milliseconds=(datarate==0)?1:((S32)(((float)AIL_stream_position(S)*1000.0F)/(float)datarate));
      }
}

//############################################################################
//##                                                                        ##
//## Seek to a specified millisecond within a stream                        ##
//##                                                                        ##
//############################################################################

void AILCALL AIL_API_set_stream_ms_position  (HSTREAM   S, //)
                                      S32       milliseconds)
{
   U32 datarate;

   if (S) 
      {
      if (S->filetype & DIG_F_ADPCM_MASK) 
         {
         U32 samples_per_block = 4 << ((S->filetype&DIG_F_STEREO_MASK)?1:0);

         samples_per_block = 1 + (S->blocksize-samples_per_block)*8 / samples_per_block;

         datarate=(S->filerate * S->blocksize)/samples_per_block;

         }
      else
         {
         if (S->block_oriented)
            {
            datarate = S->datarate;
            }
         else
            {
            datarate = S->filerate *
               ((S->filetype&DIG_F_STEREO_MASK)?2:1) *
               ((S->filetype&DIG_F_16BITS_MASK)?2:1);
            }
         }

      AIL_set_stream_position(S, (S32)(((float)datarate*(float)milliseconds)/1000.0F));
      }
}

void AILSTRM_shutdown(HDIGDRIVER driver)
{
   HSTREAM s;

   s=streams;
   
   while (s)
   {
     HSTREAM cur=s;
     s=(HSTREAM)(s->next);
     if (cur->samp)
     {
       if (cur->samp->driver==driver)
       {
         cur->samp=0;  // the driver shutdown that is calling this function will close the HSAMPLE
         AIL_close_stream(cur);
       }
     }
   }

#ifdef IS_WIN32API
   if (streams==0)
   {
     if (stream_thread_handle)
     {
       SetEvent(threadwait);
       WaitForSingleObject(stream_thread_handle,INFINITE);
       CloseHandle(threadwait);
       CloseHandle(stream_thread_handle);
       stream_thread_handle=0;
     }
   }
#endif

#ifdef IS_PS3
   if (streams==0)
   {
     if (stream_thread_handle)
     {
       uint64_t exitcode;

       threadexit = 1;
       sys_ppu_thread_join( stream_thread_handle, &exitcode);
       stream_thread_handle=0;

       threadexit = 0;
     }
   }
#endif

#ifdef IS_MACHO
   if (streams==0)
   {
     if (valid_thread_callback( thread_buf ) )
     {
       free_thread_callback( thread_buf );
     }
   }
#endif

}

#ifdef IS_DOS

//############################################################################
//##                                                                        ##
//## End of locked code                                                     ##
//##                                                                        ##
//############################################################################

void AILSTRM_end(void)
{
   //
   // close all of the open streams
   //

   while (streams)
     AIL_close_stream(streams);

   if (locked)
      {
      AIL_vmm_unlock_range(AILSTRM_start, AILSTRM_end);

      locked = 0;
      }
}

#endif


