#pragma once
#include <cstring>

// ---------------------------------------------------------------------------
// The Creative driver variant we emulate.
//
// Comparing against the real drivers running in 86Box showed that Creative
// has two driver families which **deliberately** differ in a few points.
// There is no single right answer, and there is no point in overwriting one
// variant with the other - so both are in the code, selected with
// `--driver`.
//
// All differences are measured, not guessed; details and references to the
// offsets in the disassembly are in docs/re-notes/86box_comparison.md.
//
//   Dos    - `AWEUTIL.COM` (initialisation) + `SBAWE32.MDI` (Miles/AIL, note-on)
//   Win95  - `SBAWE.VXD` 86054 B, the binary against which all 24 registers
//            at note-on are verified on 242 notes (section 11)
//   Sdk    - Creative AWE32 DOS SDK (RAWE32L.LIB, module midieng.c), used by
//            DOSMid and by our AWETEST. Closest relative of `Dos` (same GM
//            table format, same layer reservation), with its own voice
//            allocation; transcribed from the library, checked against the
//            DOSMid trace.
//
// All variants are verified on **all registers** today: `win95` against
// `SBAWE.VXD` (242 notes, MINUET), `dos` against `SBAWE32.MDI` (255 notes,
// Magic Carpet 2) and `sdk` against the DOSMid trace. Both drivers share the
// layout of the layer parameter block - it is the SoundFont generator array
// itself (section 16.2).
//
// Where they differ:
//
// | what | Dos | Win95 | where |
// |---|---|---|---|
// | 8 values in INIT3/INIT4 | AWEUTIL | ALSA == VXD | Awe32InitArrays.h, section 7.1 |
// | `kVelocityDb[0]` | 50 | 99 | Awe32Curves.h, section 8.1 |
// | attenuation formula | MDI `0x2102` | VXD `0x1C54` | Awe32Curves.h, section 10.2 |
// | attenuation +16 for ROM "1MGM" | no | yes | Synth.cpp, section 10.2 |
// | address offset in CCCA | -46 | -4 | StartAddressOffset(), section 16.4 |
// | default `initialAttenuation` | 110 | 127 | SoundFont.cpp, section 16.3 |
// | lower pan limit | `< 0` | `<= 1` | Synth.cpp, section 16.4 |
//
// One uncertainty: whether the tables and attenuation formula of
// `SBAWE32.MDI` apply to `AWEUTIL.COM` we **do not know** - its MIDI engine
// (`/EM:GM`) was never traced. Grouping them as the "Dos" family rests on
// their being used together in DOS, and on `SBAWE32.MDI` and the older
// `SBAWE32.DRV` having byte-identical tables.
// ---------------------------------------------------------------------------

namespace Awe32
{
    enum class Driver
    {
        Dos,     // AWEUTIL.COM + SBAWE32.MDI
        Win95,   // SBAWE.VXD
        Sdk,     // AWE32 DOS SDK (RAWE32L.LIB)
    };

    // `Sdk` shares the `Dos` code path wherever the two do not differ.
    inline constexpr bool IsDosLike(Driver d) { return d == Driver::Dos || d == Driver::Sdk; }

    // Win95 is the default - the whole note-on path is verified against it.
    inline constexpr Driver kDefaultDriver = Driver::Win95;

    // How many words before the sample start the driver starts playback
    // (CCCA). Both families have the very same layer parameter block - the
    // start is at offset 0x76 in both - but subtract a different constant:
    //
    //   SBAWE32.MDI 0x1FF4:  ax:dx = [si+0x76];  sub ax, 0x2e   (46)
    //   SBAWE.VXD   0x1ECF:  eax   = [ebx+0x76]; sub eax, 4
    //   SDK noteOn  0x0DB4:  ax = es:[si+0x76];     sub ax, 5
    //                        (DOSMid trace: +41 words against `dos`)
    //
    // Neither a typo nor our measuring error: against the MDI the difference
    // is exactly 42 words on every note, against the VXD CCCA matches on 242
    // notes.
    inline constexpr int StartAddressOffset(Driver d)
    {
        return (d == Driver::Dos) ? 46 : (d == Driver::Sdk) ? 5 : 4;
    }

    inline const char* DriverName(Driver d)
    {
        return (d == Driver::Dos) ? "dos" : (d == Driver::Sdk) ? "sdk" : "win95";
    }

    // Returns false when the name matches no variant.
    inline bool DriverFromName(const char* name, Driver& out)
    {
        if (name == nullptr) return false;
        if (std::strcmp(name, "dos") == 0)   { out = Driver::Dos;   return true; }
        if (name && std::strcmp(name, "win95") == 0) { out = Driver::Win95; return true; }
        if (name && std::strcmp(name, "sdk") == 0)   { out = Driver::Sdk;   return true; }
        return false;
    }
}
