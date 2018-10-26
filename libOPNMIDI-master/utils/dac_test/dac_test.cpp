#include <signal.h>
#include <deque>
#include <cstdio>
#include <cstdio>
#include <cstring>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <nuked_opn2.h>
typedef NukedOPN2 CurrentOPN2;

class MutexType
{
    SDL_mutex *mut;
public:
    MutexType() : mut(SDL_CreateMutex()) { }
    ~MutexType()
    {
        SDL_DestroyMutex(mut);
    }
    void Lock()
    {
        SDL_mutexP(mut);
    }
    void Unlock()
    {
        SDL_mutexV(mut);
    }
};

typedef std::deque<int16_t> AudioBuff;
static AudioBuff g_audioBuffer;
static MutexType g_audioBuffer_lock;

static void SDL_AudioCallbackX(void *, Uint8 *stream, int len)
{
    SDL_LockAudio();
    short *target = reinterpret_cast<int16_t*>(stream);
    g_audioBuffer_lock.Lock();
    size_t ate = size_t(len) / 2; // number of shorts
    if(ate > g_audioBuffer.size()) ate = g_audioBuffer.size();
    for(size_t a = 0; a < ate; ++a)
        target[a] = g_audioBuffer[a];
    g_audioBuffer.erase(g_audioBuffer.begin(), g_audioBuffer.begin() + AudioBuff::difference_type(ate));
    g_audioBuffer_lock.Unlock();
    SDL_UnlockAudio();
}

static int stop = 0;
static void sighandler(int dum)
{
    if((dum == SIGINT)
        || (dum == SIGTERM)
    #ifndef _WIN32
        || (dum == SIGHUP)
    #endif
    )
        stop = 1;
}


int main(int argc, char **argv)
{
    if(argc < 2)
    {
        std::printf("Testing the DAC of YM2612 chip emulator!\n"
                    "\n"
                    "Syntax: %s <path to raw mono unsigned 8bit PCM with 44100 rate>\n"
                    "\n", argv[0]);
        return 1;
    }
    // How long is SDL buffer, in seconds?
    // The smaller the value, the more often SDL_AudioCallBack()
    // is called.
    const double AudioBufferLength = 0.08;
    // How much do WE buffer, in seconds? The smaller the value,
    // the more prone to sound chopping we are.
    const double OurHeadRoomLength = 0.1;
    // The lag between visual content and audio content equals
    // the sum of these two buffers.

    int sampleRate = 44100;

    SDL_AudioSpec spec;
    SDL_AudioSpec obtained;
    spec.freq     = sampleRate;
    spec.format   = AUDIO_S16SYS;
    spec.channels = 2;
    spec.samples  = Uint16((double)spec.freq * AudioBufferLength);
    spec.callback = SDL_AudioCallbackX;

    // Set up SDL
    if(SDL_OpenAudio(&spec, &obtained) < 0)
    {
        std::fprintf(stderr, "\nERROR: Couldn't open audio: %s\n\n", SDL_GetError());
        //return 1;
    }
    if(spec.samples != obtained.samples || spec.freq != obtained.freq)
    {
        sampleRate = obtained.freq;
        std::fprintf(stderr, " - Audio wanted (samples=%u,rate=%u,channels=%u);\n"
                             " - Audio obtained (samples=%u,rate=%u,channels=%u)\n",
                     spec.samples,    spec.freq,    spec.channels,
                     obtained.samples, obtained.freq, obtained.channels);
    }

    CurrentOPN2 opn;
    opn.writeReg(0, 0x22, 0x00);  //push current LFO state
    opn.writeReg(0, 0x27, 0x00);  //set Channel 3 normal mode
    opn.writeReg(0, 0x2B, 0x80);  //Enable DAC (7'th bit is 1)
    opn.writeReg(1, 0xB4 + 2, 0xC0);  //set pan all channels

    FILE *stream = std::fopen(argv[1], "rb");
    if(!stream)
    {
        std::fprintf(stderr, "Ooops... I cant' open this damned file! (%s)\n", argv[1]);
        return 2;
    }

    signal(SIGINT, sighandler);
    signal(SIGTERM, sighandler);
    #ifndef _WIN32
    signal(SIGHUP, sighandler);
    #endif

    uint8_t buffIn[2048];
    int16_t buff[4096];
    size_t pos = 0;
    std::memset(buff, 0, sizeof(buff));

    SDL_PauseAudio(0);
    while(!stop)
    {
        size_t got_mono;
        size_t got;

        got_mono = std::fread(buffIn, 1, 2048, stream);
        got = got_mono * 2;

        for(size_t i = 0; i < got_mono; i++)
        {
            int16_t frame[2];
            pos++;
            opn.writeReg(0, 0x2A, buffIn[i]);
            opn.nativeGenerate(frame);
            buff[(i * 2) + 0] = frame[0];
            buff[(i * 2) + 1] = frame[1];
        }

        std::printf("Sample position: %lu           \r", (unsigned long)pos);
        std::fflush(stdout);

        if(got_mono < 2048)
        {
            std::fseek(stream, 0, SEEK_SET);
            pos = 0;
        }

        g_audioBuffer_lock.Lock();
        size_t pos = g_audioBuffer.size();
        g_audioBuffer.resize(pos + got);
        for(size_t p = 0; p < got; ++p)
            g_audioBuffer[pos + p] = buff[p];
        g_audioBuffer_lock.Unlock();

        const SDL_AudioSpec &spec = obtained;
        while(g_audioBuffer.size() > static_cast<size_t>(spec.samples + (spec.freq * 2) * OurHeadRoomLength))
        {
            SDL_Delay(1);
        }
    }

    std::fclose(stream);

    SDL_CloseAudio();

    return 0;
}
