# libOPNMIDI
libOPNMIDI is a free MIDI to WAV conversion library with OPN2 (YM2612) emulation

OPNMIDI Library: Copyright (c) 2017-2018 Vitaly Novichkov <admin@wohlnet.ru>

Library is based on core of the [libADLMIDI](https://github.com/Wohlstand/libADLMIDI), a MIDI playing library with OPL3 emulation.

* Semaphore-CI: [![Build Status](https://semaphoreci.com/api/v1/wohlstand/libopnmidi/branches/master/badge.svg)](https://semaphoreci.com/wohlstand/libopnmidi)
* AppVeyor CI: [![Build status](https://ci.appveyor.com/api/projects/status/98m4ltr1swyg7s5y?svg=true)](https://ci.appveyor.com/project/Wohlstand/libopnmidi)

# Tested on platforms
* Linux GCC 4.8, 4.9, 5.4 / CLang
* Mac OS X CLang (Xcode 7.x)
* Windows MinGW 4.9.x, 5.2
* Android NDK 12b/13

# Key features
* OPN2 emulation
* Customizable bank of FM patches (You have to use the [bank editor](https://github.com/Wohlstand/OPN2BankEditor) to create own sound bank)
* Stereo sound
* Number of simulated OPN2 chips can be specified as 1-100 (maximum channels 600!)
* Pan (binary panning, i.e. left/right side on/off)
* Pitch-bender with adjustable range
* Vibrato that responds to RPN/NRPN parameters
* Sustain (a.k.a. Pedal hold) and Sostenuto enable/disable
* MIDI and RMI file support
* Real-Time MIDI API suppor
* MIDI and RMI file support
* loopStart / loopEnd tag support (Final Fantasy VII)
* 111-th controller based loop start (RPG-Maker)
* Use automatic arpeggio with chords to relieve channel pressure
* Support for multiple concurrent MIDI synthesizers (per-track device/port select FF 09 message), can be used to overcome 16 channel limit
* Partial support for GS and XG standards (having more instruments than in one 128:128 GM set and ability to use multiple channels for percussion purposes, and a support for some GS/XG exclusive controllers)
* CC74 "Brightness" affects a modulator scale (to simulate frequency cut-off on WT synths)
* Portamento support (CC5, CC37, and CC65)
* SysEx support that supports some generic, GS, and XG features
* Full-panning stereo option (works for emulators only)

# How to build
To build libOPNMIDI you need to use CMake:

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
```

# License
The library is licensed under in it's parts LGPL 2.1+, GPL v2+, GPL v3+, and MIT.
* Nuked OPN2 emulator is licensed under LGPL v2.1+.
* GENS 2.10 emulator is licensed under LGPL v2.1+.
* MAME YM2612 emulator is licensed under GPL v2+.
* Genesis Plus GX emulator is licensed under GPL v2+.
* Chip interfaces are licensed under LGPL v2.1+.
* File Reader class and MIDI Sequencer is licensed under MIT.
* Other parts of library are licensed under GPLv3+.

## Available CMake options
* **CMAKE_PREFIX_PATH** - destinition folder where libOPNMIDI will be installed. On Linux it is /usr/local/ by default.
* **CMAKE_BUILD_TYPE** - Build types: **Debug** or **Release**
* **WITH_MIDIPLAY** - (ON/OFF, default OFF) Build demo MIDI player (Requires SDL2 and also pthread on Windows with MinGW)
* **WITH_VLC_PLUGIN** - (ON/OFF, default OFF) Compile VLC plugin. For now, works on Linux and VLC version 2.2.2. Support for newer VLC versions and other platforms comming soon!
* **WITH_MIDI_SEQUENCER** - (ON/OFF, default ON) Enable built-in MIDI sequencer to play loaded MIDI files. When you will disable MIDI sequencer, Real-Time functions only will work. Use this option when you are making MIDI plugin or real-time MIDI driver.
* **USE_MAME_EMULATOR** - (ON/OFF, default ON) Enable support for MAME YM2612 emulator. Well-accurate and fast on slow devices.
* **USE_NUKED_EMULATOR** - (ON/OFF, default ON) Enable support for Nuked OPN2 emulator. Very accurate, however, requires a very powerful CPU. *Is not recommended for mobile devices!*.
* **USE_GENS_EMULATOR** - (ON/OFF, default ON) Enable support for GENS 2.10 emulator. Very outdated and inaccurate, but fastest.
* **WITH_MUS_SUPPORT** - (ON/OFF, default ON) Enable support for DMX MUS format in built-in MIDI sequencer.
* **WITH_XMI_SUPPORT** - (ON/OFF, default ON) Enable support for AIL XMI format in built-in MIDI sequencer.
* **WITH_UNIT_TESTS** - (ON/OFF, default OFF) Also compile unit-tests of internal features.

* **libOPNMIDI_STATIC** - (ON/OFF, default ON) Build static library
* **libOPNMIDI_SHARED** - (ON/OFF, default OFF) Build shared library


## You also can build library manually:
You need to make in the any IDE a library project and put into it next files
(or include those files into subfolder of your exist project instead if you want to use it statically):

### Useful macros
* `BWMIDI_DISABLE_XMI_SUPPORT` - Disables XMI to MIDI converter
* `BWMIDI_DISABLE_MUS_SUPPORT` - Disables MUS to MIDI converter
* `OPNMIDI_DISABLE_MIDI_SEQUENCER` - Completely disables built-in MIDI sequencer.
* `OPNMIDI_USE_LEGACY_EMULATOR` - Enables Gens 2.10 YM2612 emulator to be used. Nuked OPN2 is used by default if macro is not defined.

### Public header (include)
* opnmidi.h    - Library API, use it to control library

### Internal code (src)
* opnbank.h    - bank structures definition
* opnmidi_private.hpp - header of internal private APIs

* opnmidi.cpp   - code of library

* opnmidi_load.cpp	- Source of file loading and parsing processing
* opnmidi_midiplay.cpp	- MIDI event sequencer
* opnmidi_opn2.cpp	- OPN2 chips manager
* opnmidi_private.cpp	- some internal functions sources

* opnmidi_bankmap.h - MIDI bank hash table
* opnmidi_bankmap.tcc - MIDI bank hash table (Implementation)
* opnmidi_cvt.hpp - Instrument conversion template
* opnmidi_ptr.hpp - Custom implementations of smart pointers for C++98
* file_reader.hpp - Generic file and memory reader

* `chips/opn_chip_base.h`   - Header of base class over all emulation cores
* `chips/opn_chip_base.tcc` - Code of base class over all emulation cores

* chips/gens_opn2.h  - Header of emulator frontent over Gens 2.10 emulator
* chips/gens_opn2.cpp  - Code of emulator frontent over Gens 2.10 emulator
* chips/gens/Ym2612_ChipEmu.h  - Gens 2.10 OPN2 Emulation header
* chips/gens/Ym2612_ChipEmu.cpp   - Code of Gens 2.10 OPN2 emulator by Stéphane Dallongeville, improved by Shay Green

* chips/mame_opn2.h  - Header of emulator frontent over MAME YM2612 emulator
* chips/mame_opn2.cpp  - Code of emulator frontent over MAME YM2612 emulator
* chips/mame/mame_ym2612fm.h  - MAME YM2612 Emulation header
* chips/mame/mame_ym2612fm.cpp   - Code of MAME YM2612 emulator by Stéphane Dallongeville, improved by Shay Green

* chips/nuked_opn2.h  - Header of emulator frontent over Nuked OPN2 emulator
* chips/nuked_opn2.cpp  - Code of emulator frontent over Nuked OPN2 emulator
* chips/nuked/ym3438.h  - Nuked OPN2 Emulation header
* chips/nuked/ym3438.cpp   - Code of Nuked OPN2 emulator by Stéphane Dallongeville, improved by Shay Green

* wopn/*        - WOPN bank format library

#### MIDI Sequencer
To remove MIDI Sequecer, define `OPNMIDI_DISABLE_MIDI_SEQUENCER` macro and remove all those files
* adlmidi_sequencer.cpp	- MIDI Sequencer related source
* cvt_mus2mid.hpp - MUS2MID converter source (define `BWMIDI_DISABLE_MUS_SUPPORT` macro to remove MUS support)
* cvt_xmi2mid.hpp - XMI2MID converter source (define `BWMIDI_DISABLE_XMI_SUPPORT` macro to remove XMI support)
* fraction.hpp  - Fraction number handling (Used by Sequencer only)
* midi_sequencer.h	- MIDI Sequencer C bindings
* midi_sequencer.hpp	- MIDI Sequencer C++ declaration
* midi_sequencer_impl.hpp	- MIDI Sequencer C++ implementation (must be once included into one of CPP together with interfaces initializations)

**Important**: Please use GENS emulator for on mobile or any non-power devices because it requires very small CPU power. Nuked OPN2 emulator is very accurate (compared to real OPN2 chip), however, it requires a VERY POWERFUL device even for a single chip emulation and is a high probability that your device will lag and playback will be dirty and choppy.

# Working demos

* [PGE MusPlay for Win32](http://wohlsoft.ru/docs/_laboratory/_Builds/win32/bin-w32/_packed/pge-musplay-dev-win32.zip) and [Win64](http://wohlsoft.ru/docs/_laboratory/_Builds/win32/bin-w64/_packed/pge-musplay-dev-win64.zip) (also available for other platforms as part of [PGE Project](https://github.com/WohlSoft/PGE-Project)) - a little music player which uses SDL Mixer X library (fork of the SDL Mixer 2.0) which has embedded libOPNMIDI to play MIDI files independently from operating system's settings and drivers. <br>(source code of player can be find [here](https://github.com/WohlSoft/PGE-Project/tree/master/MusicPlayer) and source code of SDL Mixer X [here](https://github.com/WohlSoft/SDL-Mixer-X/))
* [OPNMIDI Player for Android](https://github.com/Wohlstand/OPNMIDI-Player-Java/) - a little MIDI-player for Android which uses libOPNMIDI to play MIDI files and provides flexible GUI with ability to change bank, flags, number of emulated chips, etc.

# Changelog
## 1.4.0   2018-10-01
 * Implemented a full support for Portamento! (Thanks to [Jean Pierre Cimalando](https://github.com/jpcima) for a work!)
 * Added support for SysEx event handling! (Thanks to [Jean Pierre Cimalando](https://github.com/jpcima) for a work!)
 * Added support for GS way of custom drum channels (through SysEx events)
 * Ignore some NRPN events and lsb bank number when using GS standard (after catching of GS Reset SysEx call)
 * Added support for CC66-Sostenuto controller (Pedal hold of currently-pressed notes only while CC64 holds also all next notes)
 * Added support for CC67-SoftPedal controller (SoftPedal lowers the volume of notes played)
 * Resolved a trouble which sometimes makes a junk noise sound and unnecessary overuse of chip channels
 * Volume models support taken from libADLMIDI has been adapted to OPN2's chip speficis
 * Fixed inability to play high notes due physical tone frequency out of range on the OPN2 chip
 * Added support for full-panning stereo option

## 1.3.0   2018-06-19
 * Optimizing the MIDI banks management system for MultiBanks (Thanks to [Jean Pierre Cimalando](https://github.com/jpcima) for a work!)
 * Fixed incorrect initial MIDI tempo when MIDI file doesn't includes the tempo event
 * Fixed an incorrect processing of auto-flags
 * MAME YM2612 now results a more accurate sound as internal using of native sample rate makes more correct sound generation
 * Channel and Note Aftertouch features are now supported correctly! Aftertouch is the tremolo / vibrato, NOT A VOLUME!
 * Added optional HQ resampler for Nuked OPL3 emulators which does usage of Zita-Resampler library (Thanks to [Jean Pierre Cimalando](https://github.com/jpcima) for a work!)

## 1.2.0   2018-04-24
 * Added ability to disable MUS and XMI converters
 * Added ability to disable embedded MIDI sequencer to use library as RealTime synthesizer only or use any custom MIDI sequencer plugins.
 * Fixed blank instruments fallback in multi-bank support. When using non-zero bank, if instrument is blank, then, instrument will be taken from a root (I.e. zero bank).
 * Added support for real-time switching the emulator
 * Added support for MAME YM2612 Emulator
 * Added support for CC-120 - "All sound off" on the MIDI channel
 * Changed logic of CC-74 Brightness to affect sound only between 0 and 64 like real XG synthesizers. Ability to turn on a full-ranged brightness (to use full 0...127 range) is kept.
 * Added support for different output sample formats (PCM8, PCM8U, PCM16, PCM16U, PCM32, PCM32U, Float32, and Float64) (Thanks to [Jean Pierre Cimalando](https://github.com/jpcima) for a work!)
 * Reworked MIDI channels management to avoid any memory reallocations while music processing for a hard real time. (Thanks to [Jean Pierre Cimalando](https://github.com/jpcima) for a work!)

## 1.1.0   2018-01-21
* First stable release
* Contains all features are made in libADLMIDI 1.3.1

## 1.0.0-beta   2017-05-06
* First experimental release

