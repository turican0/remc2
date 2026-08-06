#pragma once

#include <SDL2/SDL_mixer.h>
#include <vector>

struct AudioBundle
{
	void* effect = nullptr;
	bool finished = false;
};

static std::vector<AudioBundle> ActiveAudioEffects;

template <typename AudioFormat>
struct SfxEffectWrapper
{
	AudioFormat* chunkData;
	float speed, position;
	int duration, chunkSize;
	bool handled = false;
	int frequency;
	int channels;
	uint16_t format;

	SfxEffectWrapper(const Mix_Chunk* chunk, const float speed, const int frequency, const int channels, const uint16_t format) :
		chunkData((AudioFormat*)chunk->abuf),
		speed(speed),
		frequency(frequency),
		channels(channels),
		format(format),
		duration(ComputeChunkLengthMilliSec(chunk->alen, frequency, format, channels)),
		chunkSize(chunk->alen / FormatSampleSize(format)),
		position(0)
	{
	}

	~SfxEffectWrapper()
	{
		//tty::Log("DELETED");
	}

	uint16_t FormatSampleSize(uint16_t format)
	{ 
		return (format & 0xFF) / 8; 
	}

	int ComputeChunkLengthMilliSec(int chunkSize, int frequency, uint16_t format, int channels)
	{
		const uint32_t points = chunkSize / FormatSampleSize(format);
		const uint32_t frames = (points / channels);
		return ((frames * 1000) / frequency);
	}

	void ModifyPlaybackSpeed(int mixChannel, void* stream, int length)
	{
		AudioFormat* buffer = (AudioFormat*)stream;
		const int bufferSize = length / sizeof(AudioFormat);

		if (duration < 1)
			ActiveAudioEffects[mixChannel].finished = true;

		if (position < duration)
		{
			const float delta = 1000.0f / frequency,
				vdelta = delta * speed;
			if (!handled)
			{
				//handled = true;
				for (int i = 0; i < bufferSize; i += channels)
				{
					const int j = i / channels;
					const float x = position + j * vdelta;
					const int k = floor(x / delta);
					const float prop = (x / delta) - k;

					for (int c = 0; c < channels; c++)
					{
						if (k * channels + channels - 1 < chunkSize)
						{
							AudioFormat v0 = chunkData[(k * channels + c) % chunkSize],
								v1 = chunkData[((k + 1) * channels + c) % chunkSize];
							buffer[i + c] = v0 + prop * (v1 - v0);
						}
						else
						{
							buffer[i + c] = 0;
						}
					}
				}
			}
			position += (bufferSize / channels) * vdelta;
		}
		else
		{
			//Clear buffer and finish
			for (int i = 0; i < bufferSize; i++) { buffer[i] = 0; }
			ActiveAudioEffects[mixChannel].finished = true;
		}
	}

	static void EffectModifierCallback(int channel, void* stream, int length, void* userData)
	{
		((SfxEffectWrapper*)ActiveAudioEffects[channel].effect)->ModifyPlaybackSpeed(channel, stream, length);
	}

	static void EffectDoneCallback(int channel, void* userData)
	{
		delete (SfxEffectWrapper*)ActiveAudioEffects[channel].effect;
		ActiveAudioEffects[channel].effect = nullptr;
	}
};