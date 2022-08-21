 
/*****************************************************************************
 *  BASS_VST
 *****************************************************************************
 *
 *  File:       bass_vst_process.c
 *  Authors:    Bjoern Petersen
 *  Purpose:    Processing samples
 *
 *	Version History:
 *	22.04.2006	Created in this form (bp)
 *
 *  (C) Bjoern Petersen Software Design and Development
 *  
 *****************************************************************************
 *
 *	Hint: if the module only processes mono signals, we convert the
 *	data to mono before processing and back to stereo/multichan afterwards.
 *
 *	It is no good idea to call the process routine separatly for
 *	each channel as it may track status information about previous
 *	samples (which will get out of order when processing each channel
 *	separatly)
 *
 *****************************************************************************/



#include "bass_vst_impl.h"



/*****************************************************************************
 *  conversions
 *****************************************************************************/



static void cnvFloatToDouble(const float* fBufStart, double* dBufStart, long numBytes)
{
	// copy backward to allow using the same buffers
	// not that dBuf needs to be two times larger than fBuf
	long numSamples = numBytes / sizeof(float);
	const float* f = &fBufStart[numSamples-1];
	double* d = &dBufStart[numSamples-1];
	while( f >= fBufStart )
	{
		*d = *f;
		d--;
		f--;
	}
}



static void cnvDoubleToFloat(const double* d, float* f, long numBytes)
{
	// copy forward to allow using the same buffers
	long numSamples = numBytes / sizeof(double);
	float* fBufEnd = &f[numSamples];
	while( f < fBufEnd )
	{
		*f = (float)*d;
		f++;
		d++;
	}
}



static void cnvPcm16ToFloat(const signed short* sBufStart, float* fBufStart, long numBytes)
{
	// copy backward to allow using the same buffers
	// not that fBuf needs to be two times larger than sBuf
	long numSamples = numBytes / sizeof(signed short);
	const signed short* s = &sBufStart[numSamples-1];
	float* f = &fBufStart[numSamples-1];
	while( s >= sBufStart )
	{
		*f = (float)(*s) / 32767.0F;
		f--;
		s--;
	}
}



static void cnvFloatToPcm16(const float* fBuf, signed short* sBuf, long numBytes)
{
	// copy forward to allow using the same buffers 
	long numSamples = numBytes / sizeof(float);
	const float* fBufEnd = &fBuf[numSamples];
	float sample;
	while( fBuf < fBufEnd )
	{
		sample = *fBuf * 32767.0F;
		if( sample < -32768.0F ) sample = -32768.0F;
		if( sample >  32767.0F ) sample =  32767.0F;
		*sBuf = (signed short)sample;

		fBuf++;
		sBuf++;
	}
}



static void cnvFloatLLRR_To_Mono(float* bufferL, float* bufferR, long sampleCount, float gain=1.0F)
{
	// To-mono-conversions sums the two channels to the left channel 
	// which will have double gain by default. The right channel is
	// void after conversion.

	float* bufferLEnd = bufferL+sampleCount;

	while( bufferL != bufferLEnd )
	{
		*bufferL = (*bufferL+*bufferR) * gain;
		bufferL++;
		bufferR++;
	}
}



static void cnvFloatLLRR_To_Stereo(float* bufferL, float* bufferR, long sampleCount)
{
	// To-stereo-conversions duplicates the left channel to the right
	// channel and halfs the gain of both channels.
	float* bufferLEnd = bufferL+sampleCount;

	while( bufferL != bufferLEnd )
	{
		*bufferL /= 2;
		*bufferR = *bufferL;
		bufferL++;
		bufferR++;
	}
}



/*****************************************************************************
 *  the needed buffers
 *****************************************************************************/



void callMainsChanged(BASS_VST_PLUGIN* this_, long blockSize)
{
	enterVstCritical(this_);
		this_->aeffect->dispatcher(this_->aeffect, effMainsChanged, 0, 0/*suspend*/, NULL, 0.0);
		this_->aeffect->dispatcher(this_->aeffect, effSetBlockSize, 0, blockSize, NULL, 0.0);
		this_->aeffect->dispatcher(this_->aeffect, effMainsChanged, 0, 1/*resume*/, NULL, 0.0);
	leaveVstCritical(this_);
}



static bool allocChanBuffers(BASS_VST_PLUGIN* this_, long numInputs, long numOutputs, long numBytes)
{
	if( numInputs > MAX_CHANS
	 || numInputs <= 0
	 || numOutputs > MAX_CHANS
	 || numOutputs <= 0 )
	{
		freeChansBuffers(this_);
		return false;
	}

	if( numBytes > this_->bytesPerInOutBuffer
	 || this_->buffersIn[numInputs-1] == NULL
	 || this_->buffersOut[numOutputs-1] == NULL )
	{
		// we allcate silently the double number of bytes to be prepared for double processing ...
		#define BUFFER_HEADROOM_MULT 2
		assert( sizeof(double)==sizeof(float)*BUFFER_HEADROOM_MULT );

		// free previously allocated buffers
		freeChansBuffers(this_);
	
		// allocated the new buffers
		int i;
		for( i = 0; i < numInputs; i++ )
		{ 
			if( (this_->buffersIn[i]=(float*)malloc(numBytes*BUFFER_HEADROOM_MULT)) == NULL )
			{
				freeChansBuffers(this_);
				return false;
			}
		}

		for( i = 0; i < numOutputs; i++ )
		{
			if( (this_->buffersOut[i]=(float*)malloc(numBytes*BUFFER_HEADROOM_MULT)) == NULL )
			{
				freeChansBuffers(this_);
				return false;
			}
		}

		this_->bytesPerInOutBuffer = numBytes;

		this_->effBlockSize = numBytes/sizeof(float)/*sample count*/;
		callMainsChanged(this_, this_->effBlockSize);

		EnterCriticalSection(&s_forwardCritical);
			for( i = 0; i < this_->forwardDataToOtherCnt; i++ )
			{
				BASS_VST_PLUGIN* other_ = refHandle(this_->forwardDataToOtherVstHandles[i]);
					if( other_ && other_->effStartProcessCalled )
						callMainsChanged(other_, this_->effBlockSize);
				unrefHandle(this_->forwardDataToOtherVstHandles[i]);
			}
		LeaveCriticalSection(&s_forwardCritical);
	}

	return true;
}



void freeChansBuffers(BASS_VST_PLUGIN* this_)
{
	if( this_ )
	{
		for( int i = 0; i < MAX_CHANS; i++ )
		{
			if( this_->buffersIn[i] )
			{
				free(this_->buffersIn[i]);
				this_->buffersIn[i] = NULL;
			}

			if( this_->buffersOut[i] )
			{
				free(this_->buffersOut[i]);
				this_->buffersOut[i] = NULL;
			}
		}

		this_->bytesPerInOutBuffer = 0;
	}
}



static bool allocTempBuffer(BASS_VST_PLUGIN* this_, long numBytes)
{
	if( numBytes > this_->bytesTempBuffer )
	{
		freeTempBuffer(this_);

		if( (this_->bufferTemp = (float*)malloc(numBytes)) == NULL )
		{
			freeTempBuffer(this_);
			return false;
		}

		this_->bytesTempBuffer = numBytes;
	}

	return true;
}



void freeTempBuffer(BASS_VST_PLUGIN* this_)
{
	if( this_ )
	{
		if( this_->bufferTemp )
		{
			free(this_->bufferTemp);
			this_->bufferTemp = NULL;
		}

		this_->bytesTempBuffer = 0;
	}
}



/*****************************************************************************
 *  the processing
 *****************************************************************************/



static void clearOutputBuffers(BASS_VST_PLUGIN* this_, long numSamples)
{
	int i;
	for( i = 0; i < MAX_CHANS; i++ )
	{
		if( this_->buffersOut[i] )
			memset(this_->buffersOut[i], 0, numSamples*sizeof(float)*BUFFER_HEADROOM_MULT);
	}
}



static void callProcess(BASS_VST_PLUGIN* this_, BASS_VST_PLUGIN* buffers, long numSamples)
{
	if( this_->effStartProcessCalled )
	{
		// do MIDI processing
		EnterCriticalSection(&this_->midiCritical_);
			if( this_->midiEventsCurr && this_->midiEventsCurr->numEvents )
			{
				this_->aeffect->dispatcher(this_->aeffect, effProcessEvents, 0, 0, this_->midiEventsCurr, 0.0);

				// prepare for the next round ... use the other buffer
				VstEvents* temp = this_->midiEventsCurr; this_->midiEventsCurr = this_->midiEventsPrev; this_->midiEventsPrev = temp;
				if( this_->midiEventsCurr )
					this_->midiEventsCurr->numEvents = 0;
			}
		LeaveCriticalSection(&this_->midiCritical_);

		if(    this_->aeffect->processReplacing
		 && ( (this_->aeffect->flags & effFlagsCanReplacing) || this_->aeffect->__processDeprecated == NULL) )
		{
			// do the normal float processing
			this_->aeffect->processReplacing(this_->aeffect, buffers->buffersIn, buffers->buffersOut, numSamples);
		}
		else if( this_->aeffect->__processDeprecated )
		{
			// do the "old" float processing - better than the overhead for the double replacing
			this_->aeffect->__processDeprecated(this_->aeffect, this_->buffersIn, this_->buffersOut, numSamples);
		}
		else if( canDoubleReplacing(this_) )
		{
			// convert all buffers to double; the output buffer is already emptied incl. the double headroom
			double* doubleIn[MAX_CHANS];
			double* doubleOut[MAX_CHANS];
			int i;
			for( i = 0; i < MAX_CHANS; i++ )
			{
				doubleIn[i] = (double*)buffers->buffersIn[i];
				if( doubleIn[i] )
					cnvFloatToDouble(buffers->buffersIn[i], doubleIn[i], numSamples*sizeof(float));

				doubleOut[i] = (double*)buffers->buffersOut[i]; 
			}

			// do process double replacing
			this_->aeffect->processDoubleReplacing(this_->aeffect, doubleIn, doubleOut, numSamples);

			// convert all buffers back to floats; this is also needed for the input buffers as 
			// callProcess() may be called for several instances of effects (eg. for editor forwarding)
			for( i = 0; i < MAX_CHANS; i++ )
			{
				if( doubleIn[i] )
					cnvDoubleToFloat(doubleIn[i], buffers->buffersIn[i], numSamples*sizeof(double));

				if( doubleOut[i] )
					cnvDoubleToFloat(doubleOut[i], buffers->buffersOut[i], numSamples*sizeof(double));
			}
		}
	}
}



void CALLBACK doEffectProcess(HDSP dspHandle, DWORD channelHandle, void* buffer__, DWORD bufferBytes__, USERPTR vstHandle__)
{
	DWORD				vstHandle = (DWORD)vstHandle__;
	BASS_CHANNELINFO	channelInfo;
	int					i;
	long				requiredInputs;
	long				requiredOutputs;

	float*				floatBuffer;
	long				numSamples;
	bool				cnvPcm2Float;
	bool				cnvMonoToStereo = false;

	BASS_VST_PLUGIN* this_ = refHandle(vstHandle);
	if( this_ == NULL || channelHandle != this_->channelHandle || dspHandle != this_->dspHandle || buffer__ == NULL || bufferBytes__ <= 0 )
		goto Cleanup; // error already logged

	// get the channel information
	if( !BASS_ChannelGetInfo(channelHandle, &channelInfo)
	 ||  channelInfo.chans <= 0 )
		goto Cleanup;

	requiredInputs = this_->aeffect->numInputs;
	if( (long)channelInfo.chans > requiredInputs )
		requiredInputs = channelInfo.chans;

	requiredOutputs = this_->aeffect->numOutputs;
	if( (long)channelInfo.chans > requiredOutputs )
		requiredOutputs = channelInfo.chans;

	// get the data as floats.
	// this is not lossy.
	cnvPcm2Float = ((channelInfo.flags&BASS_SAMPLE_FLOAT)==0 && (this_->type==VSTinstrument || BASS_GetConfig(BASS_CONFIG_FLOATDSP)==0));
	if( cnvPcm2Float )
	{
		if( channelInfo.flags & BASS_SAMPLE_8BITS )
			goto Cleanup; // can't and won't do this

		if( !allocTempBuffer(this_, bufferBytes__*2) )
			goto Cleanup;
		cnvPcm16ToFloat((signed short*)buffer__, this_->bufferTemp, bufferBytes__);

		floatBuffer = this_->bufferTemp;
		numSamples = (bufferBytes__ / sizeof(signed short)) / channelInfo.chans;
	}
	else
	{
		floatBuffer = (float*)buffer__;
		numSamples = (bufferBytes__ / sizeof(float)) / channelInfo.chans;
	}

	if( numSamples <= 0 )
		goto Cleanup;

	// copy the given LRLRLR buffer to the VST LLLRRR buffers
	// this is not lossy
	if( !allocChanBuffers(this_, requiredInputs, requiredOutputs, numSamples*sizeof(float)) )
		goto Cleanup;
	
	{
		long chans = channelInfo.chans, c = 0;
		float* buffer = (float*)floatBuffer;
		float* end = &buffer[numSamples * chans];
		float** in = this_->buffersIn;
		i = 0;
		while( buffer < end )
		{
			in[c][i] = *buffer;
			buffer ++;
			c++;
			if( c == chans )
			{
				c = 0;
				i++;
			}
		}

		for( c = chans; c < requiredInputs; c++ )
			memset(in[c], 0, numSamples * sizeof(float));
	}

	// special mono-processing effect handling
	if(   this_->aeffect->numInputs == 1
	 &&   this_->aeffect->numOutputs <= 2
	 &&   channelInfo.chans > 1
	 && !(this_->createFlags&BASS_VST_KEEP_CHANS) )
	{
		cnvFloatLLRR_To_Mono(this_->buffersIn[0], this_->buffersIn[1], numSamples,
			this_->aeffect->numOutputs == 1? 1.0F : 0.5F);
		
		if( this_->aeffect->numOutputs == 1 )
			cnvMonoToStereo = true;
	}

	// empty the output buffers and process
	// (normally this is needed only for process() and not for processReplacing();
	// however, this has to be done even in processReplacing() since some VSTIs
	// (most notably those from Steinberg... hehe) obviously don't implement 
	// processReplacing() as a separate function but rather use process())
	enterVstCritical(this_);
		if( !this_->doBypass )
		{
			this_->vstTimeInfo.samplePos += numSamples;
			if( this_->vstTimeInfo.samplePos < 0.0 )
				this_->vstTimeInfo.samplePos = 0.0;

			EnterCriticalSection(&s_forwardCritical);
				for( i = 0; i < this_->forwardDataToOtherCnt; i++ )
				{
					clearOutputBuffers(this_, numSamples);
					BASS_VST_PLUGIN* other_ = refHandle(this_->forwardDataToOtherVstHandles[i]);
						if( other_ )
						{
							if( tryEnterVstCritical(other_) )
							{
								callProcess(other_, this_/*buffers to use*/, numSamples);
								leaveVstCritical(other_);
							}
						}
					unrefHandle(this_->forwardDataToOtherVstHandles[i]);
				}
			LeaveCriticalSection(&s_forwardCritical);

			// the "real" sound processing (the one above is only for the editors to get data)
			clearOutputBuffers(this_, numSamples);
			callProcess(this_, this_/*buffers to use*/, numSamples);

			// special mono-processing effect handling
			if( cnvMonoToStereo )
			{
				cnvFloatLLRR_To_Stereo(this_->buffersOut[0], this_->buffersOut[1], numSamples);
			}

			// convert the returned data back to our channel representation (LLLLLRRRRR to LRLRLRLR)
			// this is not lossy
			{
				long chans = channelInfo.chans, c = 0;
				float* buffer = (float*)floatBuffer;
				float* end = &buffer[numSamples * chans];
				float** out = this_->buffersOut;
				i = 0;
				while( buffer < end )
				{
					*buffer = out[c][i];
					buffer++;
					c++;
					if( c == chans )
					{
						c = 0;
						i++;
					}
				}		
			}

			// convert the data back to PCM, if needed
			// this is lossy
			if( cnvPcm2Float )
			{
				cnvFloatToPcm16(floatBuffer, (signed short*)buffer__, numSamples * sizeof(float) * channelInfo.chans);
			}
		}
	leaveVstCritical(this_);
	
	// done
Cleanup:
	unrefHandle(vstHandle);
}




DWORD CALLBACK doInstrumentProcess(HSTREAM vstHandle, void* buffer, DWORD bufferBytes, USERPTR user /*not used for VST instruments*/)
{
	// check for common errors and init the buffer to silence (needed if processReplacing() is not available)
	if( bufferBytes <= 0 || buffer == NULL )
		return 0;
	memset(buffer, 0, bufferBytes);

	// now, we can do the same processing as for VST effects :-)
	doEffectProcess(0, vstHandle, buffer, bufferBytes, (USERPTR)vstHandle);

	return bufferBytes;
}




bool openProcess(BASS_VST_PLUGIN* this_, BASS_VST_PLUGIN* info_)
{
	// really not yet opened?
	if( this_->effStartProcessCalled )
	{
		return false;
	}

	// do we have a BASS handle?
	if( info_->channelHandle == 0 )
	{
		return false;
	}

	// get information about the channel
	BASS_CHANNELINFO channelInfo;
	if( !BASS_ChannelGetInfo(info_->channelHandle, &channelInfo) )
	{
		return false; // error already logged
	}

	enterVstCritical(this_);

		// connect the inputs and outputs
		int i;
		for( i = 0; i < info_->aeffect->numInputs; i++ )
		{
			this_->aeffect->dispatcher(this_->aeffect, __effConnectInputDeprecated, i, 
				i<(int)channelInfo.chans? 1/*connect*/ : 0/*disconnect*/, NULL, 0.0);
		}

		for( i = 0; i < info_->aeffect->numOutputs; i++ )
		{
			this_->aeffect->dispatcher(this_->aeffect, __effConnectOutputDeprecated, i, 
				i<(int)channelInfo.chans? 1/*connect*/ : 0/*disconnect*/, NULL, 0.0);
		}

		// notify the plugin that we will start processing
		this_->aeffect->dispatcher(this_->aeffect, effStartProcess, 0, 0, NULL, 0.0);
		this_->effStartProcessCalled = true;

	leaveVstCritical(this_);

	return true;
}


bool closeProcess(BASS_VST_PLUGIN* this_ /*same as info*/)
{
	// really opened?
	if( !this_->effStartProcessCalled )
	{
		return false;
	}

	enterVstCritical(this_);

		// notify the plugin that we will stop processing
		this_->aeffect->dispatcher(this_->aeffect, effStopProcess, 0, 0, NULL, 0.0);
		this_->effStartProcessCalled = false;

		// disconnect the inputs and outputs
		int i;
		for( i = 0; i < this_->aeffect->numInputs; i++ )
		{
			this_->aeffect->dispatcher(this_->aeffect, __effConnectInputDeprecated, i, 
				0/*disconnect*/, NULL, 0.0);
		}

		for( i = 0; i < this_->aeffect->numOutputs; i++ )
		{
			this_->aeffect->dispatcher(this_->aeffect, __effConnectOutputDeprecated, i, 
				0/*disconnect*/, NULL, 0.0);
		}

	leaveVstCritical(this_);

	return true;
}



