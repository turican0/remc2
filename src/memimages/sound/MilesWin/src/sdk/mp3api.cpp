//############################################################################
//##                                                                        ##
//##  Miles Sound System                                                    ##
//##                                                                        ##
//##  API.CPP: ASI decoder module for MPEG audio                            ##
//##                                                                        ##
//##  16-bit protected-mode source compatible with MSC 7.0                  ##
//##  32-bit protected-mode source compatible with MSC 11.0/Watcom 10.6     ##
//##                                                                        ##
//##  Version 1.00 of 8-Apr-98: Initial                                     ##
//##                                                                        ##
//##  Author: John Miles                                                    ##
//##                                                                        ##
//############################################################################
//##                                                                        ##
//##  Copyright (C) RAD Game Tools, Inc.                                    ##
//##                                                                        ##
//##  Contact RAD Game Tools at 425-893-4300 for technical support.         ##
//##                                                                        ##
//############################################################################

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "mss.h"
#include "mp3dec.h"

S32 ASI_started = 0;

C8 ASI_error_text[256];

//
// Property tokens
//

enum PROPERTY
{
   //
   // Additional decoder props (beyond standard RIB PROVIDER_ properties)
   //

   IN_FTYPES,               // STRING supported input file types
   IN_WTAG,                 // Tag used for this data in a wave file
   OUT_FTYPES,              // STRING supported output file types
   FRAME_SIZE,              // S32 worst-case frame buffer size

   //
   // Generic ASI stream props
   //

   INPUT_BIT_RATE,          // S32 input bit rate
   INPUT_SAMPLE_RATE,       // S32 sample rate of input data
   INPUT_BITS,              // S32 bit width per sample of input data
   INPUT_CHANNELS,          // S32 # of channels in input data

   OUTPUT_BIT_RATE,         // S32 output bit rate
   OUTPUT_SAMPLE_RATE,      // S32 output sample rate
   OUTPUT_BITS,             // S32 bit width per sample of output data
   OUTPUT_CHANNELS,         // S32 # of channels in output data

   OUTPUT_RESERVOIR,        // S32 bytes decoded but not yet requested from ASI_stream_process
   POSITION,                // S32 bytes processed so far
   PERCENT_DONE,            // % percent done
   MIN_INPUT_BLOCK_SIZE,    // S32 minimum block size for input

   //
   // Codec-specific stream props
   //

   MPEG_VERSION,            // S32 MPEG version
   MPEG_LAYER,              // S32 MPEG layer

   //
   // Stream properties
   //

   REQUESTED_RATE,          // S32 requested rate for output data
   REQUESTED_BITS,          // S32 requested bit width for output data
   REQUESTED_CHANS          // S32 requested # of channels for output data
};

//############################################################################
//#                                                                          #
//# Read one frame's worth of audio data into stream buffer                  #
//#                                                                          #
//# Return byte offset of beginning of new data in buffer, or -1 on error    #
//#                                                                          #
//############################################################################

S32 fetch_audio_data(ASISTREAM FAR *STR)
{
   //
   // See if incoming data block will fit in buffer at current write position
   // If not, discard data in lower half of buffer and move newer data down to
   // make room
   //
   // Ensure that 4 bytes of valid overrun data always exists at end of
   // buffer, to avoid page faults in read_bits()
   //
   // Incoming data block is assumed to be smaller than (STREAM_BUFSIZE / 2)!
   // Furthermore, we assume that read and write cursors are in the second
   // half of the buffer by the time an overflow condition occurs.  Two 4K
   // buffer halves will be more than sufficient to guarantee these criteria
   // for all valid MPEG frame sizes.
   //

   S32 half = STREAM_BUFSIZE / 2;

   if (STR->write_cursor + STR->data_size >= (STREAM_BUFSIZE-4))
      {
      AIL_memcpy(STR->audio_buffer,
                &STR->audio_buffer[half],
                 half);

      STR->write_cursor -= half;

      STR->apos -= (half * 8);
      }

   S32 result = call_fetch_CB(STR,
                              STR->user,
                             &STR->audio_buffer[STR->write_cursor],
                              STR->data_size,
                             -1);

   if (result < STR->data_size)
      {
#if 0
      sprintf(ASI_error_text,"Truncated MPEG audio stream, %ld bytes read, %ld expected",
         result, STR->data_size);
      return -1;
#else
   //
   // Reference encoder seems to truncate final frame, so we'll do the padding
   // here...
   //

   AIL_memset(&STR->audio_buffer[result],
               0,
               STR->data_size - result);
#endif
      }

   STR->current_offset += STR->data_size;

   //
   // Advance write cursor and return its original value
   //

   STR->write_cursor += STR->data_size;

   return STR->write_cursor - STR->data_size;
}

//############################################################################
//#                                                                          #
//# Seek next frame header in stream, and write current frame information    #
//# to ASISTREAM structure                                                   #
//#                                                                          #
//############################################################################

S32 seek_frame(ASISTREAM FAR *STR, //)
               S32            offset)
{
   //
   // Fetch data from stream until valid header encountered
   //

   if (offset != -1)
      {
      STR->current_offset = offset;
      }

expect_frame:

  S32 found_layer = -1;
  S32 start_offset = STR->current_offset;
  S32 result;
  U32 seek_data = 0;
  U32 dest_data = 0;

again:

   S32 seek_offset = offset;

   for(;;)
      {
#ifdef IS_LE
      seek_data >>= 8;
#else
      seek_data <<= 8;
#endif

      if ( ( STR->current_offset - start_offset ) > AIL_MAX_FILE_HEADER_SIZE )
      {
        goto not_found;
      }

      result = call_fetch_CB(STR,
                             STR->user,
                             ((U8 FAR*)&dest_data)+3,
                             1,
                             seek_offset);

      if (result < 1)
         {
         //
         // (Not considered an error)
         //

//       strcpy(ASI_error_text,"End of data reached");
         return 0;
         }

      seek_data |= dest_data;

      seek_offset = -1;
      STR->current_offset++;

#ifdef IS_LE
      if ( (seek_data & 0x0000e0ff ) == 0x0000e0ff )
#else
      if ( (seek_data & 0xffe00000 ) == 0xffe00000 )
#endif
         {
         break;
         }

      //
      // Did we just read the first three bytes ("TAG") of an ID3v1 tag?
      //

      static U8 swallow[256];

#ifdef IS_LE
      if ((seek_data & 0xffffff00) == 0x47415400)
#else
      if ((seek_data & 0x00ffffff) == 0x00544147)
#endif
         {
         //
         // Yes -- swallow next 125 bytes
         // This will normally leave us at the end of the file...
         // 

         result = call_fetch_CB(STR,
                                STR->user,
                                swallow,
                                125,
                                seek_offset);

         STR->current_offset += 125;

         offset = -1;
         goto expect_frame;
         }

      //
      // Did we just read the first three bytes ("ID3") of an ID3v2 tag?
      //

#ifdef IS_LE
      if ((seek_data & 0xffffff00) == 0x33444900)
#else
      if ((seek_data & 0x00ffffff) == 0x00494433)
#endif
         {
         //
         // Yes -- read the rest of the tag header, including its length word
         //

         U8 tag[10];

         result = call_fetch_CB(STR,
                                STR->user,
                               &tag[3],
                                7,
                                seek_offset);

         STR->current_offset += 7;

         if (((tag[3] < 0xff) && (tag[4] < 0xff) &&
              (tag[6] < 0x80) && (tag[7] < 0x80) && (tag[8] < 0x80) && (tag[9] < 0x80)))
            {
            //
            // Tag appears to be valid -- swallow it
            //

            S32 size = ((U32) tag[9]) | (((U32) tag[8]) << 7) | (((U32) tag[7]) << 14) | (((U32) tag[6]) << 21);
            S32 skip_len;
   
            if (tag[5] & 0x10)
               {
               skip_len = 10 + size;
               }
            else
               {
               skip_len = size;
               }

            while (skip_len > (S32) sizeof(swallow))
               {
               result = call_fetch_CB(STR,
                                      STR->user,
                                      swallow,
                                      sizeof(swallow),
                                      seek_offset);

               if (result < (S32) sizeof(swallow))
                  {
                  return 0;
                  }

               STR->current_offset += sizeof(swallow);
               skip_len            -= sizeof(swallow);
               }

            if (skip_len > 0)
               {
               result = call_fetch_CB(STR,
                                      STR->user,
                                      swallow,
                                      skip_len,
                                      seek_offset);

               if (result < skip_len)
                  {
                  return 0;
                  }

               STR->current_offset += skip_len;
               }

            //
            // Don't count size of ID3 tag as part of header-seek operation
            //

            offset = -1;
            goto expect_frame;
            }
         }
      }

   //
   // Read rest of header dword
   //

   *((U32 FAR *)(void FAR*)STR->header_buffer) = seek_data;

   //
   // Parse header, first skipping 11-bit sync field
   //

   STR->hpos = 11;

   STR->MPEG25             = !H(1);
   STR->MPEG1              =  H(1);
   STR->layer              =  H(2);
   STR->protection_bit     =  H(1);
   STR->bitrate_index      =  H(4);
   STR->sampling_frequency =  H(2);
   STR->padding_bit        =  H(1);
   STR->private_bit        =  H(1);
   STR->mode               =  H(2);
   STR->mode_extension     =  H(2);
   STR->copyright          =  H(1);
   STR->original           =  H(1);
   STR->emphasis           =  H(2);

   //
   // Perform sanity check on header
   //

   if ((STR->bitrate_index      == 0x0f) ||
       (STR->sampling_frequency == 0x03) )
   {
      //
      // Header contains one or more invalid bitfields, so skip it
      //
      // (Note that this will skip a valid frame if it begins within
      // the first 4 bytes of a false header)
      //

      offset = -1;
      goto again;
   }

   // keep searching if we find a non layer 3 block
   if ( STR->layer!= 1)
   {
     found_layer = STR->layer;
     offset = -1;
     goto again;
   }

   if (!STR->check_valid)
      {
      STR->check_valid = 1;

      STR->check_MPEG25             = STR->MPEG25;
      STR->check_MPEG1              = STR->MPEG1;
      STR->check_layer              = STR->layer;
      STR->check_protection_bit     = STR->protection_bit;
      STR->check_sampling_frequency = STR->sampling_frequency;
      STR->check_mode               = STR->mode;
      STR->check_copyright          = STR->copyright;
      STR->check_original           = STR->original;
      }
   else
      {
      if ((STR->MPEG1              != STR->check_MPEG1)              ||
          (STR->MPEG25             != STR->check_MPEG25)             ||
          (STR->layer              != STR->check_layer)              ||

//          (STR->protection_bit     != STR->check_protection_bit)     ||
//          (STR->mode               != STR->check_mode)               ||
//          (STR->copyright          != STR->check_copyright)          ||
//          (STR->original           != STR->check_original)           ||

          (STR->sampling_frequency != STR->check_sampling_frequency) )
          {

          //
          // Header does not match characteristics of last one found in
          // stream -- skip it
          //
          // (Note that this will skip a valid frame if it begins within
          // the first 4 bytes of a false header)
          //

          STR->check_MPEG25             = STR->MPEG25;
          STR->check_MPEG1              = STR->MPEG1;
          STR->check_layer              = STR->layer;
          STR->check_protection_bit     = STR->protection_bit;
          STR->check_sampling_frequency = STR->sampling_frequency;
          STR->check_mode               = STR->mode;
          STR->check_copyright          = STR->copyright;
          STR->check_original           = STR->original;

          offset = -1;
          goto again;
          }
      }

   //
   // Skip CRC word if present
   //

   STR->header_size = 4;

   if (STR->protection_bit == 0)
      {
      result = call_fetch_CB(STR,
                             STR->user,
                             &STR->header_buffer[4],
                             2,
                             -1);

      if (result < 1)
         {
          not_found:

           if (found_layer == 2)
             {
             strcpy(ASI_error_text,"MPEG Layer 2 files not supported");
             return 0 ; // we don't support layer 2
             }
           else if (found_layer == 3)  // the 3 means layer 1 - cool!
             {
             strcpy(ASI_error_text,"MPEG Layer 1 files not supported");
             return 0 ; // we don't support layer 1
             }
           else  if (found_layer == 0) // reserved layer
             {
             strcpy(ASI_error_text,"Undefined/reserved MPEG layer not supported");
             return 0;
             }

           strcpy(ASI_error_text,"MPEG audio header not found or is badly formatted");
           return 0;
         }

      STR->current_offset += 2;

      STR->hpos += 16;

      STR->header_size = 6;
      }

   STR->frame_function      = L3_frame;
   STR->frame_info_function = L3_frame_info;

   //
   // Call frame info function to fetch stream data
   //

   if (!STR->frame_info_function(STR))
      {
      //
      // End of stream reached, or error occurred
      //

      return 0;
      }

   //
   // Return success
   //

   return 1;
}

//############################################################################
//#                                                                          #
//# Retrieve a standard RIB provider property by index                       #
//#                                                                          #
//############################################################################

static S32 AILCALL PROVIDER_property(HPROPERTY index, void FAR * before_value, void const FAR * new_value, void FAR * after_value)
{
   switch (index)
      {
      case PROVIDER_NAME:
        if (before_value)
        {
          *(char FAR * FAR*)before_value = "MSS MPEG Layer 3 Audio Decoder";
          return 1;
        }
        break;
        
      case PROVIDER_VERSION:
        if (before_value) 
        {
          *(S32 FAR*)before_value = 0x110;
          return 1;
        }
        break;

      case IN_FTYPES:
        if (before_value)
        {
          *(char FAR * FAR*)before_value = "MPEG Layer 3 audio files\0*.MP3\0";
          return 1;
        }
        break;
        
      case IN_WTAG:
        if (before_value)
        {
          *(U32 FAR*)before_value = 85;
          return 1;
        }
        break;
        
      case OUT_FTYPES:
        if (before_value)
        {
          *(char FAR * FAR*)before_value = "Raw PCM files\0*.RAW\0";
          return 1;
        }
        break;
        
      case FRAME_SIZE:
        if (before_value)
        {
          *(S32 FAR*)before_value = (STREAM_BUFSIZE / 2);
          return 1;
        }
        break;
        
      }

   return 0;
}

//############################################################################
//#                                                                          #
//# Return ASI decoder error text                                            #
//#                                                                          #
//############################################################################

static C8 FAR *       AILCALL ASI_error       (void)
{
   if (ASI_error_text[0]==0)
      {
      return NULL;
      }

   return ASI_error_text;
}


//############################################################################
//#                                                                          #
//# Initialize ASI stream decoder                                            #
//#                                                                          #
//############################################################################

static ASIRESULT AILCALL ASI_startup     (void)
{
    if (ASI_started++)
      {
      strcpy(ASI_error_text,"Already started");
      return ASI_ALREADY_STARTED;
      }

   //
   // Init static prefs/peroperties
   //

    ASI_error_text[0] = 0;

   //
   // Init layer-specific data
   //

    L3_init();
 
   return ASI_NOERR;
}

//############################################################################
//#                                                                          #
//# Shut down ASI stream decoder                                             #
//#                                                                          #
//############################################################################

static ASIRESULT      AILCALL ASI_shutdown    (void)
{
   if (!ASI_started)
      {
      strcpy(ASI_error_text,"Not initialized");
      return ASI_NOT_INIT;
      }

   --ASI_started;

   //
   // Destroy layer-specific data
   //

   L3_destroy();

   return ASI_NOERR;
}

//############################################################################
//#                                                                          #
//# Open a stream, returning handle to stream                                #
//#                                                                          #
//############################################################################

static HASISTREAM AILCALL ASI_stream_open (UINTa         user,  //)
                                           AILASIFETCHCB fetch_CB,
                                           U32           total_size)
{
   //
   // Allocate ASISTREAM descriptor
   //
   // Make sure it's paragraph aligned (even in DEBUGALLOC mode) for the benefit 
   // of our SIMD code...
   //

   ASISTREAM FAR *STR = (ASISTREAM FAR *) AIL_mem_alloc_lock((sizeof(ASISTREAM) + 15) & ~15);

   if (STR == NULL)
      {
      strcpy(ASI_error_text,"Out of memory");
      return 0;
      }

   AIL_memset(STR, 0, sizeof(ASISTREAM));

   //
   // Init prefs
   //

   STR->requested_rate = -1;
   STR->requested_bits = -1;
   STR->requested_chans = -1;

   //
   // Copy params to descriptor fields
   //

   STR->user            = user;
   STR->fetch_CB        = (VOIDFUNC FAR*)fetch_CB;
   STR->total_size      = total_size;

   //
   // Alloc frame buffer
   //

   STR->frame_buffer = (U8 FAR *) AIL_mem_alloc_lock(FRAME_BUFSIZE);

   if (STR->frame_buffer == NULL)
      {
      strcpy(ASI_error_text,"Out of memory");
      AIL_mem_free_lock(STR);
      return 0;
      }

   //
   // Initialize input buffer
   //

   STR->write_cursor = 0;
   STR->apos         = 0;

   //
   // Initialize output buffer
   //

   STR->frame_size = 0;
   STR->output_cursor = 0;

   //
   // Decode first frame header to obtain stream information
   //

   if (!seek_frame(STR,0))
      {
      strcpy(ASI_error_text,"seek_frame() failed -- no valid frames?");
      AIL_mem_free_lock(STR);
      return 0;
      }

   //
   // Force rewind to beginning of stream when first frame decoded
   //

   STR->seek_param = 0;

   AIL_memset(STR->s,0,sizeof(STR->s));
   AIL_memset(STR->u,0,sizeof(STR->u));

   STR->u_start[0] = STR->u_start[1] = 0;
   STR->u_div  [0] = STR->u_div  [1] = 0;

   //
   // Return descriptor address cast to handle
   //

   return (HASISTREAM) STR;
}

//############################################################################
//#                                                                          #
//# Close stream, freeing handle and all internally-allocated resources      #
//#                                                                          #
//############################################################################

static ASIRESULT      AILCALL ASI_stream_close(HASISTREAM stream)
{
   ASISTREAM FAR *STR = (ASISTREAM FAR *) stream;

   if (STR->frame_buffer != NULL)
      {
      AIL_mem_free_lock(STR->frame_buffer);
      STR->frame_buffer = NULL;
      }

   AIL_mem_free_lock(STR);

   return ASI_NOERR;
}

//############################################################################
//#                                                                          #
//# Decode data from stream, returning # of bytes actually decoded           #
//#                                                                          #
//############################################################################

static S32 AILCALL ASI_stream_process (HASISTREAM  stream, //)
                                         void FAR   *buffer,
                                         S32         request_size)
{
   ASISTREAM FAR *STR = (ASISTREAM FAR *) stream;

   //
   // Keep track of # of bytes originally requested
   //

   S32 original_request = request_size;

   //
   // Init buffer output offset
   //

   S32 write_cursor = 0;

   U8 FAR *dest = (U8 FAR *) buffer;

   //
   // If any data from last frame remains in buffer, copy it first
   //
   // Otherwise fetch and decode frame data until request size satisfied
   //

   while (request_size > 0)
      {
      //
      // Copy as much data as possible from currently-buffered frame
      //

      if (STR->output_cursor < STR->frame_size)
         {
         S32 avail = STR->frame_size - STR->output_cursor;

         if (avail > request_size)
            {
            avail = request_size;
            }

         AIL_memcpy(&dest[write_cursor],
                    &STR->frame_buffer[STR->output_cursor],
                     avail);

         STR->output_cursor += avail;
         write_cursor       += avail;
         request_size       -= avail;
         }

      //
      // Exit from loop if request completely fulfilled by existing buffer
      // contents
      //

      if (!request_size)
         {
         break;
         }

      //
      // Else initialize output frame buffer and fetch next frame
      //

      STR->frame_size = 0;
      STR->output_cursor = 0;

      //
      // Seek next frame based on current offset
      //

      if (!seek_frame(STR,STR->seek_param))
         {
         //
         // End of stream reached, or error occurred
         //

         break;
         }

      //
      // Set offset parameter to -1 for all subsequent frames, to allow
      // application to perform continuous streaming without explicit seeks
      //

      STR->seek_param = -1;

      //
      // Call frame-processing function
      //

      S32 result = STR->frame_function(STR);

      if (result == 0)
         {
         //
         // End of stream reached, or error occurred
         //

         break;
         }

      if (result == 2)
         {
         //
         // Insufficient data was available to process frame (usually because
         // we dumped the bit reservoir during a seek) -- output a silent frame
         //

         S32 bytes_to_clear = (32 * 18 * STR->nch * sizeof(S16));

         for (S32 gr=0; gr < STR->ngr; gr++)
            {
            AIL_memset(&STR->frame_buffer[STR->frame_size],
                        0,
                        bytes_to_clear);

            STR->frame_size += bytes_to_clear;
            }
         }
      }

   //
   // If source stream exhausted, pad with zeroes to end of buffer
   //

   if (request_size > 0)
      {
      AIL_memset(&dest[write_cursor],
                  0,
                  request_size);
      }

   //
   // Return # of bytes fetched from source stream
   //

   return original_request - request_size;
}

//############################################################################
//#                                                                          #
//# Restart stream decoding process at new offset                            #
//#                                                                          #
//# Positive offset: Same as -1 below, but value will be treated as new      #
//#     stream position for the next data-fetch callback and subsequent      #
//#     position property queries                                            #
//#                                                                          #
//# -1: Clear output filter and bit reservoir, and discard both              #
//#     input and output buffers without specifying a new offset.            #
//#     Used for performing explicit asynchronous seeks when the caller      #
//#     doesn't care about updating the ASI codec's position property        #
//#                                                                          #
//# -2: Clear bit reservoir and input buffer, but do not discard accumulated #
//#     output data, filter state, or overlap contents.  Used for looping    #
//#                                                                          #
//############################################################################

static ASIRESULT AILCALL ASI_stream_seek (HASISTREAM stream, //)
                                            S32        stream_offset)
{
   ASISTREAM FAR *STR = (ASISTREAM FAR *) stream;

   //
   // Initialize input buffer
   //

   STR->write_cursor = 0;
   STR->apos         = 0;

   //
   // Initialize output buffer
   //

   if (stream_offset != -2)
      {
      STR->frame_size = 0;
      STR->output_cursor = 0;

      AIL_memset(STR->s, 0, sizeof(STR->s));
      AIL_memset(STR->u, 0, sizeof(STR->u));

      STR->u_start[0] = STR->u_start[1] = 0;
      STR->u_div  [0] = STR->u_div  [1] = 0;
      }
   else
      {
      stream_offset=-1;
      }

   //
   // Set up to resume frame processing at specified seek position
   //
   // -1 causes current position to be maintained
   //

   STR->seek_param = stream_offset;

   //
   // Return success
   //

   return ASI_NOERR;
}

#define max(a,b)  (((a) > (b)) ? (a) : (b))

//############################################################################
//#                                                                          #
//# Retrieve an ASI stream property value by index                           #
//#                                                                          #
//############################################################################

static S32     AILCALL ASI_stream_property (HASISTREAM stream, //)
                                            HPROPERTY  property,
                                            void FAR * before_value,
                                            void const FAR* new_value,
                                            void FAR * after_value)
{
   ASISTREAM FAR *STR = (ASISTREAM FAR *) stream;

   //
   // Sample rate in samples/second for [MPEG25][MPEG version][value]
   //

   const S32 sample_rate[2][2][4] =
   {{
      { 22050L,24000L,16000L,22050L },
      { 44100L,48000L,32000L,44100L }
   },
   {
      { 11025L,12000L, 8000L,11025L },
      { 44100L,48000L,32000L,44100L }
   }};

   switch (property)
      {
      //
      // Properties
      //

      case INPUT_BIT_RATE:
        if (before_value) 
        {
          *(S32 FAR*)before_value = STR->bit_rate;
          return 1;
        }
        break;

      case INPUT_SAMPLE_RATE:
        if (before_value) 
        {
          *(S32 FAR*)before_value = sample_rate[STR->MPEG25][STR->MPEG1][STR->sampling_frequency];
          return 1;
        }
        break;

      case INPUT_BITS:
        if (before_value) 
        {
          *(S32 FAR*)before_value = 16 / max(1,((sample_rate[STR->MPEG25][STR->MPEG1][STR->sampling_frequency] * 16 * STR->nch) / STR->bit_rate));
          return 1;
        }
        break;

      case INPUT_CHANNELS:
        if (before_value) 
        {
          *(S32 FAR*)before_value = STR->nch;
          return 1;
        }
        break;

      case OUTPUT_BIT_RATE:   
        if (before_value) 
        {
          *(S32 FAR*)before_value = sample_rate[STR->MPEG25][STR->MPEG1][STR->sampling_frequency] * 16 * STR->nch;
          return 1;
        }
        break;

      case OUTPUT_SAMPLE_RATE: 
        if (before_value) 
        {
          *(S32 FAR*)before_value = sample_rate[STR->MPEG25][STR->MPEG1][STR->sampling_frequency];
          return 1;
        }
        break;

      case OUTPUT_BITS: 
        if (before_value) 
        {
          *(S32 FAR*)before_value = 16;
          return 1;
        }
        break;

      case OUTPUT_CHANNELS:
        if (before_value) 
        {
          *(S32 FAR*)before_value = STR->nch;
          return 1;
        }
        break;

      case OUTPUT_RESERVOIR: 
        if (before_value) 
        {
          *(S32 FAR*)before_value = STR->frame_size - STR->output_cursor;
          return 1;
        }
        break;

      case POSITION:   
        if (before_value) 
        {
          *(S32 FAR*)before_value = STR->current_offset;
          return 1;
        }
        break;

      case MPEG_VERSION: 
        if (before_value) 
        {
          *(S32 FAR*)before_value = 2 - STR->MPEG1;
          return 1;
        }
        break;

      case MPEG_LAYER: 
        if (before_value) 
        {
          *(S32 FAR*)before_value = 4 - STR->layer;
          return 1;
        }
        break;

      case MIN_INPUT_BLOCK_SIZE: 
        if (before_value) 
        {
          *(S32 FAR*)before_value = (STREAM_BUFSIZE / 2);
          return 1;
        }
        break;

      case PERCENT_DONE:
        if (before_value) 
        {
         if ((STR->current_offset == 0) || (STR->total_size == 0))
         {
            *(F32 FAR*)before_value = 0.0f;
         }
         else
         {
            *(F32 FAR*)before_value = F32(STR->current_offset) * 100.0F / F32(STR->total_size);
         }
         return 1;
        }
        break;

      case REQUESTED_RATE:
        if (before_value) 
        {
          *(S32 FAR*)before_value = STR->requested_rate;
        }
        if (new_value)
        {
          STR->requested_rate  = *(S32 FAR*)new_value;
        }
        if (after_value) 
        {
          *(S32 FAR*)after_value = STR->requested_rate;
        }
        return 1;

      case REQUESTED_BITS:
        if (before_value) 
        {
          *(S32 FAR*)before_value = STR->requested_bits;
        }
        if (new_value)
        {
          STR->requested_bits  = *(S32 FAR*)new_value;
        }
        if (after_value) 
        {
          *(S32 FAR*)after_value = STR->requested_bits;
        }
        return 1;
  
      case REQUESTED_CHANS:
        if (before_value) 
        {
          *(S32 FAR*)before_value = STR->requested_chans;
        }
        if (new_value)
        {
          STR->requested_chans  = *(S32 FAR*)new_value;
        }
        if (after_value) 
        {
          *(S32 FAR*)after_value = STR->requested_chans;
        }
        return 1;
      }

   return 0;
}

DXDEF S32 AILEXPORT RIB_MAIN_NAME(MP3Dec)( HPROVIDER provider_handle, U32 up_down )
{
    const RIB_INTERFACE_ENTRY ASI1[] =
      {
      REG_FN(PROVIDER_property),
      REG_PR("Name",                 PROVIDER_NAME,    (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_STRING)),
      REG_PR("Version",              PROVIDER_VERSION, (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_HEX)),
      REG_PR("Input file types",     IN_FTYPES,        (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_STRING)),
      };

    const RIB_INTERFACE_ENTRY ASI2[] =
      {
      REG_PR("Input wave tag",       IN_WTAG,    (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_DEC)),
      REG_PR("Output file types",    OUT_FTYPES, (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_STRING)),
      REG_PR("Maximum frame size",   FRAME_SIZE, (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_DEC)),
      };

    const RIB_INTERFACE_ENTRY ASI3[] =
      {
      REG_FN(ASI_startup),
      REG_FN(ASI_error),
      REG_FN(ASI_shutdown),
      };

    const RIB_INTERFACE_ENTRY ASISTR1[] =
      {
      REG_FN(ASI_stream_open),
      REG_FN(ASI_stream_process),
      REG_FN(ASI_stream_property),
      };

    const RIB_INTERFACE_ENTRY ASISTR2[] =
      {
      REG_FN(ASI_stream_seek),
      REG_FN(ASI_stream_close),
      REG_PR("Input bit rate",           INPUT_BIT_RATE,       (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_DEC)),
      REG_PR("Input sample rate",        INPUT_SAMPLE_RATE,    (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_DEC)),
      };

    const RIB_INTERFACE_ENTRY ASISTR3[] =
      {
      REG_PR("Input sample width",       INPUT_BITS,           (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_DEC)),
      REG_PR("Input channels",           INPUT_CHANNELS,       (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_DEC)),
      REG_PR("Output bit rate",          OUTPUT_BIT_RATE,      (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_DEC)),
      REG_PR("Output sample rate",       OUTPUT_SAMPLE_RATE,   (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_DEC)),
      };

    const RIB_INTERFACE_ENTRY ASISTR4[] =
      {
      REG_PR("Output sample width",      OUTPUT_BITS,          (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_DEC)),
      REG_PR("Output channels",          OUTPUT_CHANNELS,      (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_DEC)),
      REG_PR("Output reservoir",         OUTPUT_RESERVOIR,     (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_DEC)),
      REG_PR("Position",                 POSITION,             (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_DEC)),
      };

    const RIB_INTERFACE_ENTRY ASISTR5[] =
      {
      REG_PR("Percent done",             PERCENT_DONE,         (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_PERCENT)),
      REG_PR("Minimum input block size", MIN_INPUT_BLOCK_SIZE, (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_DEC)),
      REG_PR("MPEG version",             MPEG_VERSION,         (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_DEC)),
      REG_PR("MPEG layer",               MPEG_LAYER,           (RIB_DATA_SUBTYPE) (RIB_READONLY|RIB_DEC)),
      };

    const RIB_INTERFACE_ENTRY ASISTR6[] =
      {
      REG_PR("Requested sample rate",    REQUESTED_RATE,       RIB_DEC),
      REG_PR("Requested bit width",      REQUESTED_BITS,       RIB_DEC),
      REG_PR("Requested # of channels",  REQUESTED_CHANS,      RIB_DEC)
      };

    if (up_down)
     {
         RIB_register(provider_handle, "ASI codec", ASI1);
         RIB_register(provider_handle, "ASI codec", ASI2);
         RIB_register(provider_handle, "ASI codec", ASI3);
 
         RIB_register(provider_handle, "ASI stream", ASISTR1);
         RIB_register(provider_handle, "ASI stream", ASISTR2);
         RIB_register(provider_handle, "ASI stream", ASISTR3);
         RIB_register(provider_handle, "ASI stream", ASISTR4);
         RIB_register(provider_handle, "ASI stream", ASISTR5);
         RIB_register(provider_handle, "ASI stream", ASISTR6);
      }
    else
      {
         RIB_unregister_all(provider_handle);
      }

   return TRUE;
}

