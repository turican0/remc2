#pragma once
#include <cstdint>

// ---------------------------------------------------------------------------
// EMU8000 register map (Sound Blaster AWE32)
//
// EVERYTHING in this file is derived from the disassembly of the AWEUTIL.COM
// driver, see docs/re-notes/emu8000_register_map.md. There each item says
// whether it is confirmed by the driver code [ASM], taken from the public
// documentation [DOC] or an estimate to be verified [?].
//
// Chip addressing:
//   pointer register (base+0xC02) = (regIndex << 5) | voice
//   data port                     = base + portOffset
//   32-bit register = low word written to the port, high word to port+2
// ---------------------------------------------------------------------------

namespace Emu8000
{
    inline constexpr int kMaxVoices = 32;   // 32 hardware voices

    // -- I/O ports relative to the Sound Blaster base (typically 0x220) ---
    // Derived from sub_10EAC/sub_10F46 in AWEUTIL.COM.
    inline constexpr uint16_t kPortData0    = 0x400; // 0x620 - low word 32bit reg
    inline constexpr uint16_t kPortData0Hi  = 0x402; // 0x622 - high word
    inline constexpr uint16_t kPortData1    = 0x800; // 0xA20 - low word 32bit reg
    inline constexpr uint16_t kPortData1Hi  = 0x802; // 0xA22 - high word / "Data2"
    inline constexpr uint16_t kPortData3    = 0xC00; // 0xE20 - 16-bit registers
    inline constexpr uint16_t kPortPointer  = 0xC02; // 0xE22 - pointer register

    // Order as we use it for the index into the register array.
    enum class Port : int
    {
        Data0   = 0,   // 0x620
        Data0Hi = 1,   // 0x622
        Data1   = 2,   // 0xA20
        Data1Hi = 3,   // 0xA22  ("Data2" in the documentation)
        Data3   = 4,   // 0xE20
        Count   = 5
    };

    // "sel" encoding, exactly as AWEUTIL uses it:
    //   sel = (regIndex << 12) | (portSel << 9) | voice
    // portSel: 2=Data0, 3=Data0Hi, 4=Data1, 5=Data1Hi, 6=Data3
    inline constexpr int PortSelOf(Port p)
    {
        return static_cast<int>(p) + 2;
    }
    inline constexpr uint16_t MakeSel(int regIndex, Port p, int voice)
    {
        return static_cast<uint16_t>((regIndex << 12) | (PortSelOf(p) << 9) | (voice & 0x1F));
    }
    inline constexpr int SelRegIndex(uint16_t sel) { return (sel >> 12) & 7; }
    inline constexpr int SelVoice(uint16_t sel)    { return sel & 0x1F; }

    // Pointer register built from sel (see sub_10EAC).
    inline constexpr uint16_t SelToPointer(uint16_t sel)
    {
        return static_cast<uint16_t>(((sel & 0x7000) >> 7) | (sel & 0x1F));
    }

    // -- Named registers -------------------------------------------------
    // Value = "sel" with voice == 0; the voice number is added on use.
    enum class Reg : uint16_t
    {
        // Data0, 32bit
        CPF     = MakeSel(0, Port::Data0, 0),  // Current Pitch + Fractional address
        PTRX    = MakeSel(1, Port::Data0, 0),  // Pitch Target + Reverb send + aux
        CVCF    = MakeSel(2, Port::Data0, 0),  // Current Volume + Current Filter cutoff
        VTFT    = MakeSel(3, Port::Data0, 0),  // Volume Target + Filter cutoff Target
        Unk0080 = MakeSel(4, Port::Data0, 0),
        Unk0088 = MakeSel(5, Port::Data0, 0),
        PSST    = MakeSel(6, Port::Data0, 0),  // Pan + Loop Start address
        CSL     = MakeSel(7, Port::Data0, 0),  // Chorus send + Loop End address

        // Data1, 32bit / 16bit
        CCCA    = MakeSel(0, Port::Data1, 0),  // Filter Q + control + Current address
        HWCF    = MakeSel(1, Port::Data1, 0),  // "voice" serves as the register index
        INIT1   = MakeSel(2, Port::Data1, 0),
        INIT3   = MakeSel(3, Port::Data1, 0),
        ENVVOL  = MakeSel(4, Port::Data1, 0),  // delay volume envelope
        DCYSUSV = MakeSel(5, Port::Data1, 0),  // decay/sustain volume envelope
        ENVVAL  = MakeSel(6, Port::Data1, 0),  // delay modulation envelope
        DCYSUS  = MakeSel(7, Port::Data1, 0),  // decay/sustain modulation envelope

        // Data1Hi ("Data2"), 16bit
        HWCF_HI = MakeSel(1, Port::Data1Hi, 0),// SMRD (v26), WC (v27)
        INIT2   = MakeSel(2, Port::Data1Hi, 0),
        INIT4   = MakeSel(3, Port::Data1Hi, 0),
        ATKHLDV = MakeSel(4, Port::Data1Hi, 0),// attack/hold volume envelope
        LFO1VAL = MakeSel(5, Port::Data1Hi, 0),// delay LFO1
        ATKHLD  = MakeSel(6, Port::Data1Hi, 0),// attack/hold modulation envelope
        LFO2VAL = MakeSel(7, Port::Data1Hi, 0),// delay LFO2

        // Data3, 16bit
        IP      = MakeSel(0, Port::Data3, 0),  // Initial Pitch
        IFATN   = MakeSel(1, Port::Data3, 0),  // Initial Filter cutoff + Attenuation
        PEFE    = MakeSel(2, Port::Data3, 0),  // Pitch/Filter envelope amount
        FMMOD   = MakeSel(3, Port::Data3, 0),  // LFO1 -> pitch / filter
        TREMFRQ = MakeSel(4, Port::Data3, 0),  // LFO1 -> volume / LFO1 frequency
        FM2FRQ2 = MakeSel(5, Port::Data3, 0),  // LFO2 -> pitch / LFO2 frequency
        Unk6C   = MakeSel(6, Port::Data3, 0),
        ChipId  = MakeSel(7, Port::Data3, 0),  // read: 0x000C expected
    };

    inline constexpr uint16_t Sel(Reg r, int voice = 0)
    {
        return static_cast<uint16_t>(static_cast<uint16_t>(r) | (voice & 0x1F));
    }

    // Registers at (Data1, reg 1), where the voice number is really the
    // register index. Values confirmed by AWEUTIL's initialisation sequence.
    namespace Hwcf
    {
        inline constexpr int kHWCF4 = 9;
        inline constexpr int kHWCF5 = 10;
        inline constexpr int kHWCF6 = 13;
        inline constexpr int kHWCF7 = 14;
        inline constexpr int kSMALR = 20;  // sample memory address, left read
        inline constexpr int kSMARR = 21;  // ... right read
        inline constexpr int kSMALW = 22;  // ... left write
        inline constexpr int kSMARW = 23;  // ... right write
        inline constexpr int kSMLD  = 26;  // sample memory left data
        inline constexpr int kSMRD  = 26;  // ... right data (on Data1Hi)
        inline constexpr int kWC    = 27;  // wave counter (on Data1Hi)
        inline constexpr int kHWCF1 = 29;
        inline constexpr int kHWCF2 = 30;
        inline constexpr int kHWCF3 = 31;
    }

    // -- Bit meanings ----------------------------------------------------

    // IP (Initial Pitch): 0xE000 = no pitch shift, 0x1000 = one octave.
    // Logarithmic scale. [PG]
    inline constexpr uint16_t kPitchUnity = 0xE000;
    inline constexpr int kPitchPerOctave  = 4096;

    // CPF (Current Pitch): unlike IP it is a LINEAR increment,
    // 0x4000 = no shift (i.e. increment 1.0). [PG]
    inline constexpr uint16_t kCpfUnity = 0x4000;

    // CCCA: bits 31..28 = Q (0 = no resonance, 15 = about 24 dB),
    // bit 27 always 0, bit 26 = DMA, bit 25 = WR (1 = write), bit 24 = RIGHT. [PG]
    inline constexpr uint32_t kCccaAddressMask = 0x00FFFFFFu;
    inline constexpr int      kCccaQShift      = 28;
    inline constexpr uint32_t kCccaDma         = 0x04000000u;
    inline constexpr uint32_t kCccaDmaWrite    = 0x02000000u;
    inline constexpr uint32_t kCccaDmaRight    = 0x01000000u;
    inline constexpr int      kCccaQMax        = 15;
    // The chip filter is a CHAMBERLIN state-variable filter running at
    // 44.1 kHz. Measured on the tester's card (AWETST25, blocks 6 and 7,
    // 157 notes). Each note's response is taken relative to the same noise at
    // cutoff 255, so the capture path, the EQ and the ROM noise spectrum all
    // cancel. Global fit over all notes at once (rms residual):
    //   Chamberlin 44.1 kHz  0.80 dB  <- with our cutoff map (101.81 Hz, 29.38 c)
    //   Chamberlin 88.2 kHz  1.20 dB
    //   analog 2-pole        1.48 dB
    //   bilinear TPT         2.78 dB  (previous core)
    // Chamberlin has no zero at Nyquist (the card attenuates less in the
    // stopband), and at high cutoffs with low Q its tuning drifts upwards -
    // exactly what the card does. Q = kChamQ0 * 10^(Q_register * kChamDbPerQ / 20).
    inline constexpr double   kChamQ0          = 0.931;
    inline constexpr double   kChamDbPerQ      = 1.175;   // Q15 = 17.6 dB
    // Filter attenuation by Q. Raising Q, the EMU8000's resonant filter also
    // lowers its input - the NRPN documentation says the attenuation is about
    // half of Q in dB (so -6 dB for Q 12 dB). The table is in amplitude,
    // scale 65536. It matches the filter_atten table of the 86Box reference
    // implementation.
    inline constexpr int kFilterAtten[16] = {
        65536, 61869, 57079, 53269, 49145, 44820, 40877, 34792,
        32845, 30653, 28607, 26392, 24630, 22463, 20487, 18470
    };

    inline constexpr double   kResonanceMaxDb  = 24.0;

    // The filter resonance **depends on its cutoff**. Not a guess: the
    // awe32faq has a measured table (copied into 86Box's snd_emu8k.c at
    // `filter_atten` too) that gives each Q a value at a low cutoff (~100 Hz)
    // and at a high one (7-8.5 kHz):
    //
    //   Q     low cutoff   high cutoff   DC attenuation
    //   0        5 dB         flat         -0.0 dB
    //   8       17 dB          7 dB        -6.0 dB
    //   15      28 dB         18 dB       -11.0 dB
    //
    // Our original `Q * 24/15` is one number for both extremes, so with the
    // filter open it overshot the peak by ~6 dB - exactly where we had too
    // much energy against the hardware (7680 Hz, see
    // docs/re-notes/emu8000_tuning.md).
    inline constexpr double kResonanceLowDb[16] = {
         5.0,  6.0,  8.0, 10.0, 11.0, 13.0, 14.0, 16.0,
        17.0, 19.0, 20.0, 22.0, 23.0, 25.0, 26.0, 28.0
    };
    inline constexpr double kResonanceHighDb[16] = {
         0.0,  0.5,  1.0,  2.0,  3.0,  4.0,  5.0,  6.0,
         7.0,  9.0, 10.0, 11.0, 13.0, 15.0, 16.0, 18.0
    };
    // The cutoffs the two columns refer to.
    inline constexpr double kResonanceLowHz  = 100.0;
    inline constexpr double kResonanceHighHz = 7700.0;

    // PSST: bits 31..24 = pan, NOTE 0 = fully right, 0xFF = fully left. [PG]
    // CSL:  bits 31..24 = chorus send (0 = none, 0xFF = maximum). [PG]
    inline constexpr uint32_t kLoopAddressMask = 0x00FFFFFFu;
    inline constexpr int      kPanShift        = 24;
    inline constexpr int      kChorusShift     = 24;

    // PTRX: bits 31..16 = pitch target, 15..8 = reverb send, 7..0 = aux. [PG]
    inline constexpr int kReverbShift = 8;

    // DCYSUSV / DCYSUS [PG]:
    //   bit 15    = 0 writes decay, 1 writes release
    //   bits 14-8 = sustain level in 0.75 dB steps (0x7F = none, 0 = silence)
    //   bit 7     = envelope generator off (always 0 for DCYSUS)
    //   bits 6-0  = encoded decay/release rate (0 = no decay)
    inline constexpr uint16_t kDcysusvRelease     = 0x8000;
    inline constexpr uint16_t kDcysusvSustainMask = 0x7F00;
    inline constexpr uint16_t kDcysusvOff         = 0x0080;
    inline constexpr uint16_t kDcysusvRateMask    = 0x007F;
    inline constexpr double   kSustainDbPerStep   = 0.75;

    // ATKHLDV / ATKHLD [PG]:
    //   bit 15    = 0 starts the attack
    //   bits 14-8 = hold in 92 ms steps (0x7F = no delay, 0 = 11.68 s)
    //   bit 7     = always 0
    //   bits 6-0  = encoded attack (0 = never, 1 = 11.88 s, 0x7F = 6 ms)
    inline constexpr uint16_t kAtkhldHoldMask   = 0x7F00;
    inline constexpr uint16_t kAtkhldAttackMask = 0x007F;
    inline constexpr double   kHoldSecPerStep   = 0.092;
    // How long the chip REALLY holds - separate from the bank conversion
    // above (SBAWE32.DRV `idiv -92`), which has to stay for the registers to
    // match.
    //
    // Run5 (AWETST05, block 9) gave 102.74 ms per step, BUT in that run the
    // whole control part of the chip ran ~9-10 % slow: LFO1 2.448 Hz instead
    // of 2.698 (block 28), and divided by that clock the hold (1.013), decay,
    // attack and ENVVOL return to [PG]. The old recording ver3 gave LFO1 and
    // ENVVOL equal to [PG] within 0.1 %. The hold +11.7 % was a property of
    // run5 (the card or the initialisation of AWETEST v05), not the chip.
    //
    // AWETST25 at the tester (2026-09-12, nominal clock: LFO1 0.9955x,
    // ENVVOL 1.007x): a regression of the hold end over 28 steps
    // (0x7D..0x61) gives 93.1 ms per step with a residual under 1 ms. That is
    // 4096 samples (92.88 ms) - what 86Box has too - not the 92 ms of the
    // Programmer's Guide. The bank conversion still uses 92 (idiv -92 in the
    // driver), the chip counts 4096 samples.
    inline constexpr double   kHoldSecPerStepChip = 4096.0 / 44100.0;

    // IFATN [PG]: bits 15-8 = initial filter cutoff in quarter semitones
    // from 125 Hz, bits 7-0 = attenuation in 0.375 dB steps (0xFF = 96 dB).
    inline constexpr double kAttenDbPerStep  = 0.375;
    inline constexpr double kAttenMaxDb      = 96.0;
    // Filter cutoff at register value 0: **101.81 Hz**, and one register
    // step is 29.3843 cents (40.84 steps per octave), so register 0xFF comes
    // to 7717 Hz.
    //
    // Not an estimate from conflicting documentation. `SYNTHGM.SBK` (SF1)
    // and `SYNTHGM.SF2` of the DOS SDK describe the same presets, only in
    // different units - the conversion can be read off the pair directly:
    //
    //     SF1   4 -> 4602 cents       SF1  89 ->  9617
    //     SF1  54 -> 7552             SF1 101 -> 10325
    //     SF1  60 -> 7906             SF1 105 -> 10561
    //
    // The spacings come to exactly 59.0 cents per SF1 step (2950/50, 354/6,
    // 1711/29, 708/12, 236/4). Extrapolating to SF1 0 gives 4366 cents =
    // 101.81 Hz - the "100 Hz" the comment in 86Box gives, too. The register
    // is twice SF1 (more precisely x255/127), so one register step is
    // 59 * 127/255 = 29.3843 cents.
    //
    // Special case: **SF1 127 has the value 14400 cents in SF2** (33 kHz),
    // which is no point on that line but "filter wide open".
    inline constexpr double kCutoffBaseHz     = 101.81;

    // Linear cutoff mapping per Vu's guide (Un-official AWE32 Programming
    // Guide, 1995):  f = 100 Hz + register * 31.25 Hz, i.e. 0 -> 100 Hz and
    // 255 -> 8068.75 Hz. Enabled with SetCutoffLinear().
    constexpr double kCutoffLinearBaseHz = 100.0;
    constexpr double kCutoffLinearStepHz = 31.25;
    inline constexpr double kCutoffBaseCents  = 4366.0;
    inline constexpr double kCutoffCentsStep  = 29.3843;
    inline constexpr int    kCutoffPerOctave = 48;    // quarter semitones

    // ENVVOL / ENVVAL / LFO1VAL / LFO2VAL [PG]: 0x8000 = no delay,
    // lower values = a growing delay in steps of 725 us.
    inline constexpr uint16_t kDelayNone      = 0x8000;
    inline constexpr double   kDelaySecPerStep = 0.000725;

    // Modulation depths [PG]. All are signed bytes, 0x7F being the full
    // positive depth and 0x80 the full negative one.
    inline constexpr double kPefePitchOctaves  = 1.0;   // PEFE hi:  env -> pitch
    inline constexpr double kPefeFilterOctaves = 6.0;   // PEFE lo:  env -> filter
    inline constexpr double kFmmodPitchOctaves = 1.0;   // FMMOD hi: LFO1 vibrato
    inline constexpr double kFmmodFilterOctaves = 3.0;  // FMMOD lo: LFO1 -> filter
    // Measured on a real card (ver3.wav, block 17, 5 steps): the swing is
    // 0.0945 dB peak to peak per unit of depth, i.e. 6 dB at full depth.
    // "+-12 dB" in the Programmer's Guide means 12 dB IN TOTAL, not +-12 -
    // we had twice that.
    inline constexpr double kTremoloMaxDb      = 6.0;   // TREMFRQ hi: LFO1 tremolo
    // Tremolo on the CHIP (AWETST25 at the tester, block 17 and the probe
    // in block 1, Hilbert and 4 ms envelope): the card only ATTENUATES. In the
    // LFO half period where lfo * depth is negative it falls to -10.3 dB at
    // depth 112 (i.e. ~12 dB at 127) and back; in the other half period it
    // stays at 0 dB. The sign of the depth only swaps which half period that
    // is. kTremoloMaxDb above stays for the SF2 -> register conversion, which
    // is verified against the driver.
    inline constexpr double kTremoloChipMaxDb  = 12.0;
    inline constexpr double kFm2PitchOctaves   = 1.0;   // FM2FRQ2 hi: LFO2 vibrato
    inline constexpr double kLfoHzPerStep      = 0.042; // 0xFF = 10.72 Hz

    // Address space of the sound memory. The card's ROM lies below
    // 0x200000, the user DRAM starts at 0x200000 (addresses are in samples,
    // not bytes).
    inline constexpr uint32_t kDramOffset = 0x200000u;

}
