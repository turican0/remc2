#pragma once
#include <cstdint>
#include <algorithm>
#include "Awe32Driver.h"

// ---------------------------------------------------------------------------
// Conversion curves MIDI -> attenuation, exactly as in the Creative drivers.
//
// Three consecutive tables of 128 bytes. Verified in THREE independent
// binaries:
//   SBAWE32.MDI (Miles/AIL driver)      0x142D, 0x14AD, 0x152D
//   SBAWE32.DRV (45632 B, WINDRV copy)  ds:0592, ds:0692, ds:0612
//   SBAWE.VXD   (86054 B, the one that really ran during the measurements)
//               kChannelVolumeDb 0x8F10, kExpressionDb 0x8F90,
//               kVelocityDb 0x8E90
//
// Two things to watch:
//  1) In the newer SBAWE.VXD kVelocityDb[0] is **99**, while MDI and WINDRV
//     have 50. The other 127 bytes are the same in all three. We keep 50
//     (two sources of three); velocity 0 is a note-off in MIDI, so it is
//     never used musically. See docs/re-notes/86box_comparison.md 8.
//  2) The SBAWE32.DRV version that really ran during the measurements
//     (45008 B) does **not contain these tables at all** - they moved into
//     the VXD. The `ds:0592` references are for the copy in WINDRV, not for
//     the measured one.
//
// The formula of the resulting attenuation (register IFATN, low byte, unit
// 0.375 dB) is transcribed from the note-on routine of SBAWE32.MDI at
// offset 0x2102:
//
//     if (cc7 <= 10) return 255;                       // silence
//     atten = ( 8*(volDb[cc7] + velDb[velocity])
//               + ((3*(127 - patchAtten)) & ~7) ) / 3;
//     if (atten >= 255) return 255;
//     if (expression < 127)
//         atten += exprDb[expression] * (255 - atten) / 127;
//
// Note that the patch attenuation is added directly in register units
// (127 - value), while volume and velocity are in dB and converted with the
// ratio 8/3 (= 1 / 0.375).
// ---------------------------------------------------------------------------

namespace Awe32Curves
{
    using Awe32::Driver;
    // MDI 0x142D / DRV ds:0592 - expression (CC11)
    inline constexpr uint8_t kExpressionDb[128] = {
        127, 108,  98,  90,  84,  80,  75,  72,  69,  66,  64,  61,  59,  57,  56,  54,
         52,  51,  49,  48,  47,  45,  44,  43,  42,  41,  40,  39,  38,  37,  36,  36,
         35,  34,  33,  33,  32,  31,  30,  30,  29,  29,  28,  27,  27,  26,  26,  25,
         24,  24,  23,  23,  22,  22,  21,  21,  21,  20,  20,  19,  19,  18,  18,  17,
         17,  17,  16,  16,  15,  15,  15,  14,  14,  14,  13,  13,  13,  12,  12,  12,
         11,  11,  11,  10,  10,  10,   9,   9,   9,   9,   8,   8,   8,   7,   7,   7,
          7,   6,   6,   6,   6,   5,   5,   5,   4,   4,   4,   4,   4,   3,   3,   3,
          3,   2,   2,   2,   2,   1,   1,   1,   1,   1,   0,   0,   0,   0,   0,   0,
    };

    // MDI 0x14AD / DRV ds:0692 - channel volume (CC7)
    inline constexpr uint8_t kChannelVolumeDb[128] = {
         99,  99,  99,  99,  99,  99,  99,  99,  99,  99,  99,  43,  41,  40,  39,  38,
         37,  36,  35,  34,  33,  32,  31,  30,  30,  29,  28,  27,  27,  26,  25,  25,
         24,  23,  23,  22,  22,  21,  21,  20,  20,  19,  19,  19,  18,  18,  17,  17,
         17,  16,  16,  16,  15,  15,  15,  14,  14,  14,  14,  13,  13,  13,  12,  12,
         12,  12,  12,  11,  11,  11,  11,  10,  10,  10,  10,   9,   9,   9,   9,   9,
          8,   8,   8,   8,   8,   7,   7,   7,   7,   6,   6,   6,   6,   6,   5,   5,
          5,   5,   5,   4,   4,   4,   4,   4,   3,   3,   3,   3,   3,   2,   2,   2,
          2,   2,   2,   1,   1,   1,   1,   1,   1,   1,   0,   0,   0,   0,   0,   0,
    };

    // MDI 0x152D / DRV ds:0612 - velocity
    inline constexpr uint8_t kVelocityDb[128] = {
         50,  49,  48,  47,  46,  45,  44,  43,  42,  42,  41,  40,  39,  38,  37,  36,
         36,  35,  34,  33,  33,  32,  31,  30,  30,  29,  28,  28,  27,  26,  26,  25,
         25,  24,  24,  23,  22,  22,  21,  21,  20,  20,  19,  19,  19,  18,  18,  17,
         17,  16,  16,  16,  15,  15,  15,  14,  14,  14,  13,  13,  13,  12,  12,  12,
         11,  11,  11,  11,  10,  10,  10,  10,   9,   9,   9,   9,   9,   8,   8,   8,
          8,   8,   7,   7,   7,   7,   7,   6,   6,   6,   6,   6,   6,   5,   5,   5,
          5,   5,   5,   5,   4,   4,   4,   4,   4,   4,   3,   3,   3,   3,   3,   3,
          2,   2,   2,   2,   2,   2,   1,   1,   1,   1,   1,   0,   0,   0,   0,   0,
    };

    // Transcription of the computation in SBAWE32.MDI, offset 0x2102.
    // patchAttenUnits = patch attenuation in register units (0 = none).
    // The velocity table of `SBAWE.VXD` has 99 at index 0, while
    // `SBAWE32.MDI` and the older `SBAWE32.DRV` have 50. The other 127 bytes
    // are the same. Velocity 0 is a note-off in MIDI, so it makes no audible
    // difference, but we keep them apart for fidelity.
    inline int VelocityDb(int velocity, Driver drv)
    {
        velocity = std::clamp(velocity, 0, 127);
        if (velocity == 0 && drv == Driver::Win95) return 99;
        return kVelocityDb[velocity];
    }

    // Formula from `SBAWE32.MDI` offset 0x2102 (Dos family).
    inline int ComputeAttenuationMdi(int cc7, int velocity, int expression,
                                     int patchAttenUnits)
    {
        const int db = kChannelVolumeDb[cc7] + VelocityDb(velocity, Driver::Dos);
        const int patch = (3 * std::clamp(patchAttenUnits, 0, 255)) & ~7;
        int atten = (8 * db + patch) / 3;
        if (atten >= 255) return 255;

        if (expression < 127)
            atten += kExpressionDb[expression] * (255 - atten) / 127;

        return std::clamp(atten, 0, 255);
    }

    // Formula from `SBAWE.VXD` object 1, 0x1C54..0x1CC1 (Win95 family).
    //
    //   sum    = volDb[CC7] + velDb[velocity] + (X + 12) / 24
    //   atten  = globalAtten + sum * 8 / 3               , clipped to 255
    //   if (expression < 127)
    //       atten += (256 - atten) * exprDb[expression] / 128
    //
    // The key is the field `word[esi+0x60]` = X, the **patch attenuation in
    // 1/20 dB**. Read off the instruction trace: for a preset with
    // `initialAttenuation` 107 the driver has 150 there, and 150 * 0.05 dB =
    // 7.5 dB = (127-107) * 0.375 dB. Our `patchAttenUnits` is in register
    // units (0.375 dB), so the conversion is `* 15 / 2`.
    //
    // Three differences from the Dos variant:
    //  1) the patch attenuation is added **into the dB sum before the
    //     conversion** (and truncated by the integer division by 24), not
    //     after it in register units through `(3*p) & ~7`;
    //  2) expression divides by 128 instead of 127 and starts from 256
    //     instead of 255;
    //  3) `[edi+0x10]` is one more, global attenuation - 0 on all 242
    //     measured notes, so it is not modelled yet.
    //
    // Verified: velocity 109 -> velDb 3, CC7 127 -> volDb 0, X 150.
    //   sum = 0 + 3 + (150+12)/24 = 9,  atten = 9*8/3 = 24 = 0x18
    // and after adding 16 for ROM "1MGM" (see Synth.cpp) it comes to 0x28,
    // exactly what the driver wrote.
    inline int ComputeAttenuationVxd(int cc7, int velocity, int expression,
                                     int patchAttenUnits)
    {
        const int patchTwentiethsDb = std::clamp(patchAttenUnits, 0, 255) * 15 / 2;
        const int db = kChannelVolumeDb[cc7] + VelocityDb(velocity, Driver::Win95)
                     + (patchTwentiethsDb + 12) / 24;

        int atten = (db * 8) / 3;
        if (atten >= 255) return 255;

        if (expression < 127)
            atten += (256 - atten) * kExpressionDb[expression] / 128;

        return std::clamp(atten, 0, 255);
    }

    // AWE32 DOS SDK, midieng noteOn 0x0C7F..0x0CE9 (family Sdk). Same three
    // tables as MDI; the patch attenuation goes into the dB sum as
    // units * 25 / 80 (truncated), and the expression step is
    // `mul cx / shr ax, 7` from 256.
    inline int ComputeAttenuationSdk(int cc7, int velocity, int expression,
                                     int patchAttenUnits)
    {
        const int db = kChannelVolumeDb[cc7] + VelocityDb(velocity, Driver::Sdk)
                     + std::clamp(patchAttenUnits, 0, 255) * 25 / 80;
        int atten = (db * 8) / 3;
        if (atten > 255) return 255;

        if (expression < 127)
            atten += (kExpressionDb[expression] * (256 - atten)) >> 7;

        return std::clamp(atten, 0, 255);
    }

    inline int ComputeAttenuation(int cc7, int velocity, int expression,
                                  int patchAttenUnits, Driver drv)
    {
        cc7 = std::clamp(cc7, 0, 127);
        velocity = std::clamp(velocity, 0, 127);
        expression = std::clamp(expression, 0, 127);

        // Both families mute the channel when the volume is very low.
        if (cc7 <= 10) return 255;

        if (drv == Driver::Sdk)
            return ComputeAttenuationSdk(cc7, velocity, expression, patchAttenUnits);
        return (drv == Driver::Win95)
            ? ComputeAttenuationVxd(cc7, velocity, expression, patchAttenUnits)
            : ComputeAttenuationMdi(cc7, velocity, expression, patchAttenUnits);
    }
    // ---- attenuation -> linear amplitude (target volume of the voice) ---
    //
    // `SBAWE.VXD` obj 1, 0x21BF:
    //
    //     mov si, word [edx*2 + 0x409010]   ; table[atten & 15]
    //     ...
    //     sar eax, 4                        ; atten >> 4
    //     shr si, cl                        ; mantissa >> (atten >> 4)
    //
    // So 6 dB per shift and 16 steps in between, which makes 0.3763 dB per
    // unit. It was found by back-computing the bounds of each of the 16
    // entries from 844 notes of Georgia; in the whole binary **one** place
    // satisfies them.
    //
    // Offsets: in the file the table is at **0x8DB0** (verified by searching
    // for the sixteen words in the binary), the static disassembler shows it
    // at 0x409010 and at run time it is at 0xC10001BC. An earlier comment
    // claimed 0x09010 in the file - that was the linear address passed off
    // as a file offset; the LE object does not start at the start of the
    // file.
    //
    // Careful: this is not 86Box's `attentable` (which has a step of exactly
    // 0.375 dB and starts at 65535), so no smooth formula fits it.
    inline constexpr uint16_t kAttenToAmp16[16] = {
        60096, 57544, 55104, 52768, 50528, 48392, 46336, 44376,
        42488, 40688, 38960, 37312, 35728, 34216, 32768, 31376
    };

    // Fifteen words that **precede** the table in the binary
    // (0x8D92..0x8DAE). The driver reads them when the attenuation is
    // negative: it makes the index as `atten % 16` truncated towards zero, so
    // the index is negative too and reaches before the table. It is the end
    // of another table and the last three words are zeros, so an
    // attenuation of -1 to -3 gives silence.
    inline constexpr uint16_t kAttenBeforeTable[15] = {
        1542, 1286, 1285, 1028, 1028, 772, 771, 515, 514, 258, 257, 257, 0, 0, 0
    };

    // Transcription of `SBAWE.VXD` obj 1, 0xC0FFB36B..0xC0FFB398 at run time:
    //
    //     movsx eax, word [ebx+0x26]   ; atten, **signed**
    //     cdq / xor / sub / and 0xF / xor / sub   ; index = atten % 16 towards zero
    //     mov si, word [edx*2 + table]
    //     cdq / and edx,0xF / add / sar eax,4     ; shift = atten / 16 towards zero
    //     mov cl, al / shr si, cl
    //
    // Both divisions truncate **towards zero**, not down - hence `%` and `/`
    // in C++, not `& 15` and `>> 4`. For an attenuation >= 0 it is the same,
    // for a negative one it is not.
    //
    // Measured: on Georgia, JUMP and RELAX (14 931 notes) the attenuation is
    // always 16..255, and `ComputeAttenuation*` clips it to 0..255 anyway,
    // so the negative branch does not occur today. It is here to match the
    // driver, not for the sound.
    inline uint16_t VolumeTarget(int attenUnits)
    {
        const int index = attenUnits % 16;
        const int shift = attenUnits / 16;
        const uint16_t base = (index >= 0) ? kAttenToAmp16[index]
                                           : kAttenBeforeTable[15 + index];

        // `shr si, cl` takes only the low byte of the shift and the CPU masks
        // the count to five bits; a shift of 16 and more gives zero in a
        // 16-bit register.
        const unsigned count =
            static_cast<unsigned>(static_cast<uint8_t>(shift)) & 31u;
        return (count >= 16u) ? uint16_t{0}
                              : static_cast<uint16_t>(base >> count);
    }

}
