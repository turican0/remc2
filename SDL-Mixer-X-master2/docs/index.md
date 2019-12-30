# SDL Mixer X
A fork of [SDL_mixer](http://www.libsdl.org/projects/SDL_mixer/) library

# Description
SDL_Mixer is a sample multi-channel audio mixer library.
It supports any number of simultaneously playing channels of 16 bit stereo audio,
plus a single channel of music, mixed by the popular FLAC, MikMod MOD, ModPlug,
Timidity MIDI, FluidSynth, Ogg Vorbis, libMAD, and SMPEG MP3 libraries.

SDL Mixer X - is an extended fork made by Vitaly Novichkov "Wohlstand" in
13 January 2015. SDL_mixer is a quick and stable high-quality audio library,
however, it has own bunch of deffects such as broken resampling, incorrect
playback of WAV files, inability to choose MIDI backend in runtime,
inability to customize Timidty patches path, No support for cross-fade
and parallel streams playing (has only one musical stream. Using of very
long Chunks is ineffectively), etc. The goal of this fork is resolving those
issues, providing more extended functionality than was originally,
and providing support for more supported audio formats.

## New features of SDL Mixer X in comparison to original SDL_mixer
* Added much more music formats (Such a game music emulators liks NSF, VGM, HES, GBM, etc. playing via GME library, also XMI, MUS, and IMF playing via [ADLMIDI](https://github.com/Wohlstand/libADLMIDI) library)
* Added support of the loop points in the OGG files (via <u>LOOPSTART</u> and <u>LOOPEND</u> (or <u>LOOPLENGHT</u>) meta-tags) (Later was also added into original SDL_mixer).
* In the Modplug module enabled internal loops (tracker musics with internal loops are will be looped rightly)
* Better MIDI support:
  * Added ability to choose any of available MIDI backends in runtime
  * Added ability to append custom config path for Timidity synthesizer, then no more limit to default patches set
  * Forked version now has [ADLMIDI](https://github.com/Wohlstand/libADLMIDI) midi sequences together with Native MIDI, Timidity and Fluidsynth. ADLMIDI is [OPL-Synth](http://wohlsoft.ru/pgewiki/FM_Synthesis) Emulation based MIDI player. Unlike to other MIDI synthesizers are using in SDL Mixer X (except of Native MIDI), ADLMIDI is completely standalone software synthesizer which never requires any external sound fonts or banks to work.
  * Also the [OPNMIDI](https://github.com/Wohlstand/libOPNMIDI) was added which a MIDI player through emulator of YM2612 chip which was widely used on Sega Megadrive/Genesis game console. Also is fully standalone like ADLMIDI.
* Added new API functions
  * Ability to redefine Timidity patches path. So, patches folders are can be stored in any place!
  * Added functions to retrieve some meta-tags: Title, Artist, Album, Copyright
  * Added ADLMIDI Extra functions: Change bank ID, enable/disable high-level tremolo, enable/disable high-level vibrato, enable/disable scalable modulation, Set path to custom bank file (You can use [this editor](https://github.com/Wohlstand/OPL3BankEditor) to create or edit them)
  * Added OPNMIDI Extra function: Set path to custom bank file (You can use [this editor](https://github.com/Wohlstand/OPN2BankEditor) to create or edit them)
* Own re-sampling implementation which a workaround to glitches caused with inaccurate re-sampler implementation from SDL2 (anyway, recent versions of SDL2 now has much better resampler than was before).
* Added support of [extra arguments](http://wohlsoft.ru/pgewiki/SDL_Mixer_X#Path_arguments) in the tail of the file path, passed into Mix_LoadMUS function.
* Added ability to build shared library in the <u>stdcall</u> mode with static linking of libSDL on Windows to use it as independent audio library with other languages like VB6 or .NET.
* QMake and CMake building systems in use

## Requirements
* Fresh [SDL2 library](https://hg.libsdl.org/SDL/)
* Complete set of audio codecs from [this repository](https://github.com/WohlSoft/AudioCodecs)
* Building system on your taste:
  * QMake from Qt 5.x
  * CMake >= 2.8

# How to build

## With CMake
With this way all dependencies will be automatically downloaded and compiled
```bash
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr/local/ -DDOWNLOAD_AUDIO_CODECS_DEPENDENCY=ON -DDOWNLOAD_SDL2_DEPENDENCY=ON ..
make -j 4 #where 4 - set number of CPU cores you have
make install
```

# How to use library

## Include
The API is fully compatible with original SDL Mixer with exception of new added functions and different header name
```cpp
#include <SDL2/SDL_mixer_ext.h>
```

## Linking (Linux)

### Dynamically
```bash
gcc playmus.c -o playmus -I/usr/local/include -L/usr/local/lib -lSDL2_mixer_ext -lSDL2 -lstdc++
```

### Statically
To get it linked you must also link dependencies of SDL Mixer X library itself and also dependencies of SDL2 too
```
gcc playmus.c -o playmus -I/usr/local/include -L/usr/local/lib -Wl,-Bstatic -lSDL2_mixer_ext -lFLAC -lopusfile -lopus -lvorbisfile -lvorbis -logg -lmad -lid3tag -lmodplug -lADLMIDI -lOPNMIDI -ltimidity -lgme -lzlib -lSDL2 -Wl,-Bdynamic -lpthread -lm -ldl -static-libgcc -lstdc++
```

# Documentation
* [Full documentation](SDL_mixer_ext.html)
* [PGE-Wiki description page](http://wohlsoft.ru/pgewiki/SDL_Mixer_X)

