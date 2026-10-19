#include "Emu8000.h"

#include <algorithm>
#include <cmath>
#include <cstdio>

using namespace Emu8000;

namespace
{
    constexpr double kPi = 3.14159265358979323846;

    // Full attenuation range (IFATN 0xFF = 96 dB per the Programmer's Guide).
    constexpr double kFullScaleDb = kAttenMaxDb;

    // The decay/release time tables in the drivers are times for a run over
    // 100 dB - that fits the Programmer's Guide (rate 0x7F = 240 us/dB,
    // table 24 ms; rate 0x01 = 470 ms/dB, table 47513 ms).
    constexpr double kDecayTableSpanDb = 100.0;

    // --- register value -> time conversions ------------------------------
    // [ASM] Derived from the conversion tables in SBAWE32.DRV (the Windows
    // AWE32 MIDI driver): the attack time table at ds:1552 (127 entries) and
    // the decay/release time table at ds:1650 (128 entries), both in
    // milliseconds. The lookup routines sub_2BC0 (attack) and sub_2BF0
    // (decay) establish that
    //   attack rate r = 1..127  ->  attackTable[r-1]
    //   decay  rate r = 0..127  ->  decayTable[r]
    //
    // Both tables are described exactly by one formula: time = base / k(i),
    // where k is a 7-bit "floating" encoding - the first 16 values 1..16, the
    // next 16 values 17..32, and then the step doubles with every group of 16.
    // Verified against both tables byte by byte (0 deviations), so no data
    // has to be copied from the driver.
    int RateDivisor(int index)          // index 0..127
    {
        const int group = (index >> 4) & 7;
        const int m = index & 15;
        return (group == 0) ? (m + 1) : ((m + 17) << (group - 1));
    }

    // ATKHLDV/ATKHLD, bits 6..0. Attack is the total rise time (the envelope
    // is linear in amplitude in this phase). Rate 0 = "never attack" [PG].
    // Rate 1 = 11.88 s, rate 0x7F = 6 ms - exactly the table in the driver.
    double AttackSeconds(int rate)
    {
        if (rate <= 0) return -1.0;      // never
        return 11.878 / RateDivisor(std::min(rate, 127) - 1);
    }

    // DCYSUSV/DCYSUS, bits 6..0. Decay and release use the same register
    // and the same table - on Note Off the driver just rewrites DCYSUSV with
    // bit 15 set.
    //
    // The Programmer's Guide gives the rate as time per dB, not as the total
    // envelope time, so that is how it is computed here too. Returns dB per
    // second; a negative value means "no decay" (rate 0).
    double DecayDbPerSecond(int rate)
    {
        if (rate <= 0) return -1.0;      // no decay
        const double spanSeconds = 47.513 / RateDivisor(std::min(rate, 127) - 1);
        return kDecayTableSpanDb / spanSeconds;
    }

    // ATKHLDV/ATKHLD, bits 14..8: hold in steps of 92 ms, 0x7F = no delay [PG].
    double HoldSeconds(int hold)
    {
        // Chip hold - see kHoldSecPerStepChip (the deviation in run5 was the
        // slower control clock of that run, not a property of the chip).
        return (127 - std::clamp(hold, 0, 127)) * kHoldSecPerStepChip;
    }

    // ENVVOL/ENVVAL/LFO1VAL/LFO2VAL: delay, 0x8000 = no delay,
    // lower values = a growing delay in steps of 725 us [PG].
    double DelaySeconds(uint16_t value)
    {
        const int units = static_cast<int>(kDelayNone) - static_cast<int>(value);
        if (units <= 0) return 0.0;
        return units * kDelaySecPerStep;
    }

    // DCYSUSV bits 14..8: sustain level as attenuation in steps of 0.75 dB,
    // 0x7F = no attenuation, 0 = silence [PG].
    double SustainDb(int level)
    {
        return (0x7F - std::clamp(level, 0, 0x7F)) * kSustainDbPerStep;
    }

    // IFATN bits 7..0: initial attenuation in steps of 0.375 dB, 0xFF = 96 dB [PG].
    double AttenuationDb(int value)
    {
        return std::clamp(value, 0, 255) * kAttenDbPerStep;
    }

    // IFATN bits 15..8: initial filter cutoff.
    //
    // The Programmer's Guide contradicts itself here: it says "in quarter
    // semitones, 0x00 = 125 Hz" and also "0xFF = 8 kHz". Quarter semitones
    // (48 per octave) would give only 4966 Hz at 255. We keep the stated end
    // points, i.e. 125 Hz to 8 kHz over 255 steps (= 42.5 steps per octave),
    // because:
    //   - only then do both stated numbers fit
    //   - the modulation depths are in octaves in the manual, so they convert
    //     the same whatever the steps per octave (see RenderVoice)
    //
    // The manual also says explicitly: "If the Q of the channel is programmed
    // to zero and the filter cutoff to 0xFF, the filter does not alter the
    // signal." With quarter semitones the filter would still cut at 5 kHz and
    // take away highs that are in the reference recordings.
    // Frequency at register 0xFF, derived from the base and the step in cents.
    inline constexpr double kCutoffTopHz =
        Emu8000::kCutoffBaseHz * 30.31287;   // 2^(255*29.3843/1200) = 7717 Hz

    double CutoffOctaves(double cutoffReg, double topHz = kCutoffTopHz,
                         double baseHz = Emu8000::kCutoffBaseHz)
    {
        // how many octaves above the base the register value lies
        const double octavesTotal = std::log2(topHz / baseHz);
        return std::clamp(cutoffReg, 0.0, 255.0) / 255.0 * octavesTotal;
    }

    double CutoffHz(double octavesAboveBase, double baseHz = Emu8000::kCutoffBaseHz)
    {
        return baseHz * std::pow(2.0, octavesAboveBase);
    }

    // TREMFRQ/FM2FRQ2 bits 7..0: LFO frequency in steps of 0.042 Hz,
    // 0xFF = 10.72 Hz [PG]. The series starts at 0.01 Hz, not at zero.
    double LfoHz(int value)
    {
        return 0.01 + std::clamp(value, 0, 255) * kLfoHzPerStep;
    }

    // The chip LFO has a TRIANGLE shape, not a sine. It starts at zero, rises
    // to +1 at a quarter period, back to zero at half and to -1 at three
    // quarters. Transcription of the lfotable of the 86Box reference
    // implementation; a sine here sounded different.
    double LfoTriangle(double phase01)
    {
        double t = phase01 - std::floor(phase01);          // 0..1
        t += 0.25;                                          // offset as in the table
        if (t >= 1.0) t -= 1.0;
        return (t < 0.5) ? (4.0 * t - 1.0) : (3.0 - 4.0 * t);
    }

    double DbToLinear(double db)
    {
        if (db >= kFullScaleDb) return 0.0;
        return std::pow(10.0, -db / 20.0);
    }

    // Filter input attenuation by Q, taken from `filter_atten` in 86Box's
    // snd_emu8k.c (derived there from the awe32faq: the attenuation is about
    // half of Q in dB). In 8.8 fixed point, 65536 = unchanged.
    constexpr int32_t kFilterAtten86[16] = {
        65536, 61869, 57079, 53269, 49145, 44820, 40877, 34792,
        32845, 30653, 28607, 26392, 24630, 22463, 20487, 18470
    };

    // Filter cutoff as 86Box fills it into its coefficient table: it starts
    // at 125 Hz and each of the 256 steps multiplies by 1.016378315
    // (= 42.66 steps per octave).
    double Cutoff86Hz(int index)
    {
        double out = 125.0;
        for (int i = 0; i < std::clamp(index, 0, 255); ++i)
            out *= 1.016378315;
        return out;
    }

    inline int8_t HiSigned(uint16_t w)  { return static_cast<int8_t>(w >> 8); }
    inline int8_t LoSigned(uint16_t w)  { return static_cast<int8_t>(w & 0xFF); }
    inline int    HiByte(uint16_t w)    { return (w >> 8) & 0xFF; }
    inline int    LoByte(uint16_t w)    { return w & 0xFF; }
}

// ===========================================================================
// construction, register array
// ===========================================================================

Emu8000Core::Emu8000Core(uint32_t outputSampleRate)
    : m_outputRate(outputSampleRate ? outputSampleRate : kNativeSampleRate)
{
    // The default presets are those the driver sets: chorus 2 (Chorus 3) and
    // reverb 4 (Hall 2) - see SBAWE32.DRV 0x60FA and 0x612D.
    m_chorus.Init(kNativeSampleRate, Emu8000Fx::kChorusDefault);
    m_reverb.Init(kNativeSampleRate);
    m_reverb.SetPreset(Emu8000Fx::kReverbDefault);
    m_eq.Init(kNativeSampleRate);
    PowerOnInit();
}

uint16_t& Emu8000Core::RegRef(Port p, int reg, int voice)
{
    return m_regs[static_cast<size_t>(p)][reg & 7][voice & 0x1F];
}

uint16_t Emu8000Core::RegVal(Port p, int reg, int voice) const
{
    return m_regs[static_cast<size_t>(p)][reg & 7][voice & 0x1F];
}

// ===========================================================================
// port level - exactly what the driver does (see AWEUTIL sub_10EAC)
// ===========================================================================

void Emu8000Core::SetBasePort(uint16_t sbBasePort)
{
    m_basePort = sbBasePort;
}

bool Emu8000Core::OwnsPort(uint16_t port) const
{
    const uint16_t off = static_cast<uint16_t>(port - m_basePort);
    return off == kPortData0 || off == kPortData0Hi
        || off == kPortData1 || off == kPortData1Hi
        || off == kPortData3 || off == kPortPointer;
}

bool Emu8000Core::OpenTrace(const char* path)
{
    CloseTrace();
    FILE* f = std::fopen(path, "wb");
    if (!f) return false;
    std::fprintf(f, "# EMU8000 port write trace, 44100 Hz timebase\n");
    std::fprintf(f, "# <frame> <port hex> <value hex>\n");
    m_traceFile = f;
    m_traceFrames = 0;
    return true;
}

void Emu8000Core::CloseTrace()
{
    if (m_traceFile)
    {
        std::fclose(static_cast<FILE*>(m_traceFile));
        m_traceFile = nullptr;
    }
}

void Emu8000Core::PortOut16(uint16_t port, uint16_t value)
{
    if (m_traceFile && !m_traceOff)
        std::fprintf(static_cast<FILE*>(m_traceFile), "%llu %03X %04X\n",
                     static_cast<unsigned long long>(m_traceFrames), port, value);

    // The 86Box chip gets exactly what would go over the bus. Write-backs of
    // the state (UpdateRegistersFromState) are recognised by m_traceOff and
    // are not sent - the chip does those itself.
    if (m_chip == Chip::Box86 && !m_traceOff)
        m_box.PortWrite(port, value);

    const uint16_t off = static_cast<uint16_t>(port - m_basePort);
    if (off == kPortPointer)
    {
        m_pointer = value;
        return;
    }

    Port p;
    switch (off)
    {
    case kPortData0:   p = Port::Data0;   break;
    case kPortData0Hi: p = Port::Data0Hi; break;
    case kPortData1:   p = Port::Data1;   break;
    case kPortData1Hi: p = Port::Data1Hi; break;
    case kPortData3:   p = Port::Data3;   break;
    default: return;
    }

    const int reg   = (m_pointer >> 5) & 7;
    const int voice = m_pointer & 0x1F;
    // 86Box ignores an IFATN write with zero attenuation to a silent voice
    // whose DCYSUSV is 0x0080 and IP is 0 (its patch against clicks from
    // trackers that zero the registers to stop a note). Kept 1:1.
    if (p == Port::Data3 && reg == 1 && (value & 0xFF) == 0 && !m_voices[voice].playing
        && RegVal(Port::Data1, 5, voice) == kDcysusvOff && RegVal(Port::Data3, 0, voice) == 0)
        return;

    RegRef(p, reg, voice) = value;

    // A CCCA write sets the playback address immediately, as in 86Box
    // (emu8k_outw, Data1/Data2 register 0: addr.int_address = ccca & mask).
    // Before this the address was only taken at note-on, while
    // UpdateRegistersFromState wrote the voice's stale position back into
    // CCCA after every rendered block - so an address written a few frames
    // before the note-on (normal in a register replay) was lost and the note
    // started wherever the voice had stopped (AWETST25 block 33: the noise
    // note played ROM from the previous sine's loop).
    if (reg == 0 && (p == Port::Data1 || p == Port::Data1Hi))
        m_voices[voice].address = Read(Reg::CCCA, voice) & kCccaAddressMask;

    // Sample memory upload. SMALW / SMARW (register 1, voices 22 / 23; low
    // word on Data1, address bits 23..16 on Data1Hi) hold the write address;
    // every write to SMLD (Data1) or SMRD (Data1Hi) of register 1, voice 26
    // stores one sample there and advances that address. Without this a
    // register replay of a program that uploads its own samples played
    // silence (AWETST25 block 27, BULLFROG.SBK: presets 0, 3, 4 silent in our
    // render, audible on the card and in the 86Box core). Writes below the
    // DRAM start (the ROM) are ignored, as on the chip.
    if (reg == 1 && voice == Hwcf::kSMLD && (p == Port::Data1 || p == Port::Data1Hi))
    {
        static constexpr size_t kDramMaxWords = 0x1000000u - kDramOffset;
        const int ptr = (p == Port::Data1) ? Hwcf::kSMALW : Hwcf::kSMARW;
        const uint16_t hi = RegVal(Port::Data1Hi, 1, ptr);
        uint32_t addr = RegVal(Port::Data1, 1, ptr) | (static_cast<uint32_t>(hi & 0xFF) << 16);
        if (addr >= kDramOffset && addr - kDramOffset < kDramMaxWords)
        {
            const size_t idx = addr - kDramOffset;
            if (idx >= m_dram.size())
                m_dram.resize(std::min(kDramMaxWords, std::max(idx + 1, m_dram.size() * 2 + 65536)), 0);
            m_dram[idx] = static_cast<int16_t>(value);
        }
        addr = (addr + 1) & 0xFFFFFFu;
        RegRef(Port::Data1, 1, ptr)   = static_cast<uint16_t>(addr & 0xFFFF);
        RegRef(Port::Data1Hi, 1, ptr) = static_cast<uint16_t>((hi & 0xFF00) | (addr >> 16));
    }

    // A write to IP recomputes the target pitch in the upper half of PTRX.
    // The **chip** does that, not the driver - `SBAWE32.MDI` only reads it
    // from there and leaves it (see docs/re-notes/86box_comparison.md 14.3).
    // 86Box has it as `ptrx_pit_target = freqtable[ip] >> 18`, where
    // `freqtable[c] = 2^((c - 0xE000) / 4096) * 2^32`.
    if (p == Port::Data3 && reg == 0)
    {
        // The intermediate value must be 64-bit - for high IP it exceeds 2^32.
        const double ratio = std::pow(2.0, (static_cast<double>(value) - 0xE000) / 4096.0);
        const uint64_t full = static_cast<uint64_t>(ratio * 65536.0 * 65536.0);
        const uint32_t target = (value == 0)
            ? 0u
            : static_cast<uint32_t>(std::min<uint64_t>(full >> 18, 0xFFFFu));
        RegRef(Port::Data0Hi, 1, voice) = static_cast<uint16_t>(target);
    }

    // Per the drivers, the DCYSUSV write is the one that starts the envelope
    // engine ("decay/sustain parameter must be set at last"), so we react to
    // it by changing the voice state.
    // Register side effects on the voice follow 86Box emu8k_outw 1:1:
    //   DCYSUSV (Data1 reg 5): engine on = bit 7 clear. Only the off -> on
    //     transition starts a note: LFOs reset, the volume envelope restarts
    //     when ATKHLDV bit 15 is clear, the mod envelope when ATKHLD bit 15
    //     is clear. A write to a voice that is already on only changes
    //     sustain/decay. Bit 15 = release (applied after a possible start).
    //   ATKHLDV (Data1Hi reg 4) / ATKHLD (Data1Hi reg 6) with bit 15 clear on
    //     a voice whose engine is on: restart that envelope (ATKHLDV also
    //     resets the LFOs).
    //   DCYSUS (Data1 reg 7) bit 15: release of the mod envelope.
    // The sample address comes from the CCCA write (see above); the envelope
    // and filter DSP itself stays our measured model.
    auto& vs = m_voices[voice];
    const auto restartVolEnv = [&]()
    {
        vs.volStage  = EnvStage::Delay;
        vs.volDb     = kFullScaleDb;
        vs.volLin    = 0.0;
        vs.stageTime = 0.0;
    };
    const auto restartModEnv = [&]()
    {
        vs.modStage     = EnvStage::Delay;
        vs.modLevel     = 0.0;
        vs.modStageTime = 0.0;
    };
    const auto resetLfos = [&]()
    {
        vs.lfo1Phase = 0.0;
        vs.lfo2Phase = 0.0;
        vs.lfo1Delay = DelaySeconds(RegVal(Port::Data1Hi, 5, voice));
        vs.lfo2Delay = DelaySeconds(RegVal(Port::Data1Hi, 7, voice));
    };

    if (p == Port::Data1 && reg == 5)
    {
        const bool wasOn = vs.engineOn;
        vs.engineOn = (value & kDcysusvOff) == 0;
        if (!vs.engineOn)
        {
            vs.volStage = EnvStage::Off;
            vs.modStage = EnvStage::Off;
            vs.playing = false;
        }
        else if (!wasOn)
        {
            // note start
            vs.address   = Read(Reg::CCCA, voice) & kCccaAddressMask;
            vs.frac      = 0;
            vs.playing   = true;
            resetLfos();
            if (!(RegVal(Port::Data1Hi, 4, voice) & 0x8000))
                restartVolEnv();
            if (!(RegVal(Port::Data1Hi, 6, voice) & 0x8000))
                restartModEnv();
            // All five filter state variables, not just the first two. Voices
            // are recycled: if energy from the previous note stayed in the
            // second stage (--filter-poles 4) or in the one-pole branch, the
            // start of the new one would ring with it - a click the real chip
            // does not make.
            vs.filtIc1   = 0.0;
            vs.filtIc2   = 0.0;
            vs.filtIc3   = 0.0;
            vs.filtIc4   = 0.0;
            vs.filtLp1   = 0.0;
            vs.filtIc5   = 0.0;
        }
        if (vs.engineOn && (value & kDcysusvRelease))
        {
            if (vs.volStage != EnvStage::Off)
                vs.volStage = EnvStage::Release;
            if (vs.modStage != EnvStage::Off)
                vs.modStage = EnvStage::Release;
            vs.stageTime = 0.0;
            vs.modStageTime = 0.0;
        }
    }
    else if (p == Port::Data1Hi && reg == 4 && !(value & 0x8000) && vs.engineOn)
    {
        resetLfos();
        restartVolEnv();
    }
    else if (p == Port::Data1Hi && reg == 6 && !(value & 0x8000) && vs.engineOn)
    {
        restartModEnv();
    }
    else if (p == Port::Data1 && reg == 7 && (value & 0x8000) && vs.engineOn)
    {
        if (vs.modStage != EnvStage::Off)
            vs.modStage = EnvStage::Release;
        vs.modStageTime = 0.0;
    }
}

uint16_t Emu8000Core::PortIn16(uint16_t port)
{
    const uint16_t value = PortIn16Raw(port);
    // Reads go to the trace as "R <frame> <port> <value>", the format of the
    // 86Box traces, so the driver's decisions can be followed there.
    if (m_traceFile && !m_traceOff)
        std::fprintf(static_cast<FILE*>(m_traceFile), "R %llu %03X %04X\n",
                     static_cast<unsigned long long>(m_traceFrames), port, value);
    return value;
}

uint16_t Emu8000Core::PortIn16Raw(uint16_t port)
{
    // With the 86Box chip every read goes to the chip, like on the bus.
    if (m_chip == Chip::Box86)
        return m_box.PortRead(port);

    const uint16_t off = static_cast<uint16_t>(port - m_basePort);
    if (off == kPortPointer)
    {
        // Drivers wait in loops for bit 12 of the pointer register to toggle
        // (AWEUTIL sub_12A20). We derive it from the wave counter so that the
        // host code does not hang.
        return static_cast<uint16_t>((m_pointer & ~0x1000u)
                                     | ((m_waveCounter & 0x100u) ? 0x1000u : 0u));
    }

    Port p;
    switch (off)
    {
    case kPortData0:   p = Port::Data0;   break;
    case kPortData0Hi: p = Port::Data0Hi; break;
    case kPortData1:   p = Port::Data1;   break;
    case kPortData1Hi: p = Port::Data1Hi; break;
    case kPortData3:   p = Port::Data3;   break;
    default: return 0xFFFF;
    }

    const int reg   = (m_pointer >> 5) & 7;
    const int voice = m_pointer & 0x1F;

    // Detection register - the driver expects the low nibble 0x0C.
    if (p == Port::Data3 && reg == 7 && voice == 0)
        return 0x000C;

    // Wave counter - a free-running counter.
    if (p == Port::Data1Hi && reg == 1 && voice == Hwcf::kWC)
        return static_cast<uint16_t>(m_waveCounter);

    return RegVal(p, reg, voice);
}

// ===========================================================================
// register level
// ===========================================================================

namespace
{
    // sel bits 11..9 -> index into Emu8000::Port; out of range = invalid
    inline bool PortFromSel(uint16_t sel, Port& out)
    {
        const int portSel = (sel >> 9) & 7;
        if (portSel < 2 || portSel > 6) return false;
        out = static_cast<Port>(portSel - 2);
        return true;
    }
}

namespace
{
    uint16_t PortOffset(Port p)
    {
        return p == Port::Data0   ? kPortData0
             : p == Port::Data0Hi ? kPortData0Hi
             : p == Port::Data1   ? kPortData1
             : p == Port::Data1Hi ? kPortData1Hi
                                  : kPortData3;
    }
}

void Emu8000Core::WriteReg16(uint16_t sel, uint16_t value)
{
    Port p;
    if (!PortFromSel(sel, p)) return;
    // Exactly what the driver does: first the pointer, then the data port.
    // That keeps the OpenTrace trace complete and replayable in 86Box.
    PortOut16(static_cast<uint16_t>(m_basePort + kPortPointer), SelToPointer(sel));
    PortOut16(static_cast<uint16_t>(m_basePort + PortOffset(p)), value);
}

uint16_t Emu8000Core::ReadReg16(uint16_t sel) const
{
    Port p;
    if (!PortFromSel(sel, p)) return 0;
    if (p == Port::Data3 && SelRegIndex(sel) == 7 && SelVoice(sel) == 0)
        return 0x000C;
    if (p == Port::Data1Hi && SelRegIndex(sel) == 1 && SelVoice(sel) == Hwcf::kWC)
        return static_cast<uint16_t>(m_waveCounter);
    return RegVal(p, SelRegIndex(sel), SelVoice(sel));
}

void Emu8000Core::WriteReg32(uint16_t sel, uint32_t value)
{
    // A real write sends the low word to the data port and the high word to
    // port+2, which for Data0 means Data0Hi and for Data1 Data1Hi ("Data2").
    Port p;
    if (!PortFromSel(sel, p)) return;

    Port hi;
    if (p == Port::Data0)      hi = Port::Data0Hi;
    else if (p == Port::Data1) hi = Port::Data1Hi;
    else { WriteReg16(sel, static_cast<uint16_t>(value)); return; }

    // The driver (AWEUTIL sub_10F46) sends the low word to `port` and the
    // high word to `port+2`, with one write to the pointer before that.
    PortOut16(static_cast<uint16_t>(m_basePort + kPortPointer), SelToPointer(sel));
    PortOut16(static_cast<uint16_t>(m_basePort + PortOffset(p)),
              static_cast<uint16_t>(value & 0xFFFF));
    PortOut16(static_cast<uint16_t>(m_basePort + PortOffset(hi)),
              static_cast<uint16_t>(value >> 16));
}

uint32_t Emu8000Core::ReadReg32(uint16_t sel) const
{
    Port p;
    if (!PortFromSel(sel, p)) return 0;

    Port hi;
    if (p == Port::Data0)      hi = Port::Data0Hi;
    else if (p == Port::Data1) hi = Port::Data1Hi;
    else return ReadReg16(sel);

    const int reg   = SelRegIndex(sel);
    const int voice = SelVoice(sel);
    return (static_cast<uint32_t>(RegVal(hi, reg, voice)) << 16)
         | static_cast<uint32_t>(RegVal(p, reg, voice));
}

namespace
{
    // Which registers are really 32-bit, i.e. for which port+2 is the upper
    // half of the same register and not a separate register.
    //
    // Data0 (portSel 2): all eight registers are 32-bit.
    // Data1 (portSel 4): only CCCA (reg 0) and HWCF (reg 1) are 32-bit.
    //   For reg 2..7 a completely different register lies at A22h - INIT2,
    //   INIT4, ATKHLDV, LFO1VAL, ATKHLD, LFO2VAL. A 32-bit write to DCYSUSV
    //   would thus clear LFO1VAL. Confirmed against 86Box (snd_emu8k.c, case
    //   0xA00 vs 0xA02).
    bool IsReg32(uint16_t sel)
    {
        const int portSel = (sel >> 9) & 7;
        if (portSel == 2) return true;                       // Data0
        if (portSel == 4) return ((sel >> 12) & 7) <= 1;     // Data1: CCCA, HWCF
        return false;
    }
}

void Emu8000Core::Write(Reg r, int voice, uint32_t value)
{
    const uint16_t sel = Sel(r, voice);
    if (IsReg32(sel)) WriteReg32(sel, value);
    else              WriteReg16(sel, static_cast<uint16_t>(value));
}

uint32_t Emu8000Core::Read(Reg r, int voice) const
{
    const uint16_t sel = Sel(r, voice);
    if (IsReg32(sel)) return ReadReg32(sel);
    return ReadReg16(sel);
}

uint32_t Emu8000Core::ReadDriver(Reg r, int voice)
{
    // As the drivers do it (SBAWE32.MDI 0x17B8 / 0x182C): pointer, then the
    // low word from the data port and, for a 32-bit register, the high word
    // from port + 2.
    const uint16_t sel = Sel(r, voice);
    Port p;
    if (!PortFromSel(sel, p)) return 0;
    PortOut16(static_cast<uint16_t>(m_basePort + kPortPointer), SelToPointer(sel));
    const uint16_t lo = PortIn16(static_cast<uint16_t>(m_basePort + PortOffset(p)));
    if (!IsReg32(sel)) return lo;
    const Port hi = (p == Port::Data0) ? Port::Data0Hi : Port::Data1Hi;
    const uint16_t hw = PortIn16(static_cast<uint16_t>(m_basePort + PortOffset(hi)));
    return (static_cast<uint32_t>(hw) << 16) | lo;
}

// ===========================================================================
// initialisation - transcription of the AWEUTIL.COM sequence (sub_12B40)
// ===========================================================================

// One init array = 128 values in four blocks of 32; each block goes to a
// different register for voices 0..31 (ALSA send_array()).
void Emu8000Core::SendInitArray(const uint16_t* data, const Awe32Init::AltInit* alt)
{
    // Each driver family sends eight values of INIT3/INIT4 differently (see
    // Awe32Driver.h), so they are overwritten for Win95.
    uint16_t buf[128];
    std::copy(data, data + 128, buf);
    if (alt && m_driver == Awe32::Driver::Win95)
        for (int i = 0; i < 8; ++i) buf[alt[i].index] = alt[i].value;

    for (int v = 0; v < 32; ++v) Write(Reg::INIT1, v, buf[v]);
    for (int v = 0; v < 32; ++v) Write(Reg::INIT2, v, buf[32 + v]);
    for (int v = 0; v < 32; ++v) Write(Reg::INIT3, v, buf[64 + v]);
    for (int v = 0; v < 32; ++v) Write(Reg::INIT4, v, buf[96 + v]);
}

void Emu8000Core::PowerOnInit()
{
    m_regs = RegFile{};
    m_voices = {};
    m_pointer = 0;
    m_waveCounter = 0;

    // step 2-4: HWCF1/2/3
    WriteReg16(MakeSel(1, Port::Data1, Hwcf::kHWCF1), 0x0059);
    WriteReg16(MakeSel(1, Port::Data1, Hwcf::kHWCF2), 0x0020);
    WriteReg16(MakeSel(1, Port::Data1, Hwcf::kHWCF3), 0x0004);

    // step 5 (sub_126E8): 16-bit registers of all voices
    for (int v = 0; v < kMaxVoices; ++v)
    {
        Write(Reg::DCYSUSV, v, kDcysusvOff);
        Write(Reg::ATKHLD,  v, 0);
        Write(Reg::DCYSUS,  v, 0);
        Write(Reg::IP,      v, 0);
        Write(Reg::IFATN,   v, 0xFF00);
        Write(Reg::PEFE,    v, 0);
        Write(Reg::FMMOD,   v, 0);
        Write(Reg::TREMFRQ, v, 0x0018);
        Write(Reg::FM2FRQ2, v, 0x0018);
        Write(Reg::Unk6C,   v, 0);
        Write(Reg::LFO2VAL, v, 0);
        Write(Reg::LFO1VAL, v, 0);
        Write(Reg::ATKHLDV, v, 0);
        Write(Reg::ENVVOL,  v, 0);
        Write(Reg::ENVVAL,  v, 0);
    }

    // step 6 (sub_127AE): 32-bit registers of all voices.
    // VTFT and CVCF get 0x0000FFFF, not 0xFFFFFFFF - the upper half is the
    // volume (0 = silence), the lower the filter cutoff (0xFFFF = fully
    // open). SBAWE32.DRV does the same (sub_1320).
    for (int v = 0; v < kMaxVoices; ++v)
    {
        Write(Reg::PTRX,    v, 0);
        Write(Reg::VTFT,    v, 0x0000FFFFu);
        Write(Reg::PSST,    v, 0);
        Write(Reg::CSL,     v, 0);
        Write(Reg::CPF,     v, 0);
        Write(Reg::CVCF,    v, 0x0000FFFFu);
        Write(Reg::CCCA,    v, 0);
        Write(Reg::Unk0088, v, 0);
        Write(Reg::Unk0080, v, 0);
    }

    // step 7 (sub_1288C): SMALR/SMARR/SMALW + init arrays.
    //
    // The init arrays INIT1..INIT4 are coefficients of the internal DSP. Our
    // emulation does not use them - they are only stored in the register
    // array - but 86Box reads the reverb and chorus parameters from them, so
    // they must be sent for the OpenTrace trace to be replayable. The order
    // follows ALSA init_arrays().
    WriteReg16(MakeSel(1, Port::Data1, Hwcf::kSMALR), 0);
    WriteReg16(MakeSel(1, Port::Data1, Hwcf::kSMARR), 0);
    WriteReg16(MakeSel(1, Port::Data1, Hwcf::kSMALW), 0);
    // AWEUTIL writes SMARR a second time, not SMARW like the Linux driver.
    // Measured from a real run, see docs/re-notes/86box_comparison.md.
    WriteReg16(MakeSel(1, Port::Data1, Hwcf::kSMARR), 0);

    SendInitArray(Awe32Init::kInit1);
    SendInitArray(Awe32Init::kInit2);
    SendInitArray(Awe32Init::kInit3, Awe32Init::kAltInit3Sbawe);

    WriteReg32(MakeSel(1, Port::Data1, Hwcf::kHWCF4), 0x00000000u);
    WriteReg32(MakeSel(1, Port::Data1, Hwcf::kHWCF5), 0x00000083u);
    WriteReg32(MakeSel(1, Port::Data1, Hwcf::kHWCF6), 0x00008000u);
    WriteReg32(MakeSel(1, Port::Data1, Hwcf::kHWCF7), 0x00000000u);

    SendInitArray(Awe32Init::kInit4, Awe32Init::kAltInit4Sbawe);

    // step 8 (sub_12A20): voices 30 and 31 serve as "DRAM refresh" channels.
    // Watch the `cwd` in AWEUTIL: 0xFFE0 is sign-extended to 0xFFFFFFE0,
    // while elsewhere the upper half is cleared through `xor dx,dx`.
    Write(Reg::PSST, 30, 0xFFFFFFE0u);
    Write(Reg::CSL,  30, 0x00FFFFE8u);
    Write(Reg::PTRX, 30, 0x00000000u);
    Write(Reg::CPF,  30, 0x00000000u);
    Write(Reg::CCCA, 30, 0x00FFFFE3u);
    Write(Reg::PSST, 31, 0x00FFFFF0u);
    Write(Reg::CSL,  31, 0x00FFFFF8u);
    Write(Reg::PTRX, 31, 0x000000FFu);
    Write(Reg::CPF,  31, 0x00008000u);
    Write(Reg::CCCA, 31, 0x00FFFFF3u);

    // These two writes are described in docs/re-notes/emu8000_register_map.md
    // ("pointer=003Eh ... Data0+2=4828h, pointer=003Ch, Data1=0"), but only
    // the comparison with the real AWEUTIL in 86Box showed that they really
    // are in the initialisation and where they belong. Data1 reg 1 voice 28
    // is the undocumented register HWCF.
    Write(Reg::PTRX, 30, 0x48280000u);
    WriteReg16(MakeSel(1, Port::Data1, 28), 0x0000);

    // AWEUTIL's `cwd` fills in the upper half here too (0xFFFFFFFF), but BOTH
    // Creative drivers write 0x0000FFFF - the target volume of voices 30/31
    // is thus zero (g_win95_c.trace, dos_mdi.trace and mc2_full.trace have
    // VTFT FFFF / VTFT^ 0000). It matters: voice selection in SBAWE.VXD
    // scores by VTFT^, so with 0xFFFF it would never touch voices 30 and 31.
    Write(Reg::VTFT, 30, 0x0000FFFFu);
    Write(Reg::VTFT, 31, 0x0000FFFFu);

    // step 9
    // SBAWE32.MDI (and AWEUTIL) end with HWCF3 = 0x0004, SBAWE.VXD with
    // 0x0006 (g_win95_c/relax_win95/jump_win95.trace vs dos_mdi/mc2_full.trace).
    // The extra bit fits the VXD taking voices 30 and 31 for notes.
    WriteReg16(MakeSel(1, Port::Data1, Hwcf::kHWCF3),
               (m_driver == Awe32::Driver::Win95) ? 0x0006 : 0x0004);

    // SBAWE32.MDI initialises the voices again when the game loads it and
    // ends every voice with DCYSUS = DCYSUSV = 0x807F (0x398C..0x399F in the
    // per-voice loop at 0x3912). Its voice allocation reads DCYSUSV back and
    // an idle voice must show the release bit, otherwise every voice scores
    // 0x1000 and the last odd voice wins instead of voice 0.
    if (Awe32::IsDosLike(m_driver))
    {
        for (int v = 0; v < kMaxVoices; ++v)
        {
            Write(Reg::DCYSUS,  v, 0x807Fu);
            Write(Reg::DCYSUSV, v, 0x807Fu);
        }
    }
}

// ===========================================================================
// sound memory
// ===========================================================================

void Emu8000Core::ResizeDram(size_t numSamples)
{
    m_dram.assign(numSamples, 0);
}

int16_t Emu8000Core::ReadSample(uint32_t address) const
{
    if (address < kDramOffset)
    {
        // The card's wave ROM. If none is loaded, the address reads silence.
        return (address < m_rom.size()) ? m_rom[address] : 0;
    }
    const size_t idx = address - kDramOffset;
    return (idx < m_dram.size()) ? m_dram[idx] : 0;
}

bool Emu8000Core::IsVoiceActive(int voice) const
{
    if (voice < 0 || voice >= kMaxVoices) return false;
    return m_voices[voice].volStage != EnvStage::Off;
}

// ===========================================================================
// render
// ===========================================================================

void Emu8000Core::RenderVoice(int v, float* outL, float* outR,
                              float* sendRev, float* sendCho, uint32_t numFrames)
{
    VoiceState& vs = m_voices[v];
    if (vs.volStage == EnvStage::Off) return;

    const uint16_t atkhldv = RegVal(Port::Data1Hi, 4, v);
    const uint16_t dcysusv = RegVal(Port::Data1,   5, v);
    const uint16_t envvol  = RegVal(Port::Data1,   4, v);
    const uint16_t atkhld  = RegVal(Port::Data1Hi, 6, v);
    const uint16_t dcysus  = RegVal(Port::Data1,   7, v);
    const uint16_t envval  = RegVal(Port::Data1,   6, v);
    const uint16_t ifatn   = RegVal(Port::Data3,   1, v);
    const uint16_t pefe    = RegVal(Port::Data3,   2, v);
    const uint16_t fmmod   = RegVal(Port::Data3,   3, v);
    const uint16_t tremfrq = RegVal(Port::Data3,   4, v);
    const uint16_t fm2frq2 = RegVal(Port::Data3,   5, v);
    const uint16_t ipReg   = RegVal(Port::Data3,   0, v);

    const uint32_t ccca = Read(Reg::CCCA, v);
    const uint32_t psst = Read(Reg::PSST, v);
    const uint32_t csl  = Read(Reg::CSL,  v);

    const uint32_t loopStart = psst & kLoopAddressMask;
    const uint32_t loopEnd   = csl  & kLoopAddressMask;
    const int      panReg    = static_cast<int>(psst >> kPanShift) & 0xFF;
    const int      filterQ   = static_cast<int>(ccca >> kCccaQShift) & 0x0F;

    // Effect sends: reverb from PTRX bits 15..8, chorus from CSL bits 31..24.
    const float revSend = ((Read(Reg::PTRX, v) >> kReverbShift) & 0xFF) / 255.0f;
    const float choSend = static_cast<float>((csl >> kChorusShift) & 0xFF) / 255.0f;

    // Envelope constants (the rate registers usually do not change at run
    // time, so converting them once per block is enough).
    const double volDelay   = DelaySeconds(envvol);
    // The scales default to 1.0; they are for measurement, see SetHoldScale.
    const double volAttackRaw = AttackSeconds(atkhldv & kAtkhldAttackMask);
    const double volAttack  = (volAttackRaw < 0.0) ? volAttackRaw
                                                   : volAttackRaw * m_attackScale;
    const double volHold    = HoldSeconds((atkhldv & kAtkhldHoldMask) >> 8)
                            * m_holdScale;
    const double volDecayRaw = DecayDbPerSecond(dcysusv & kDcysusvRateMask);
    const double volDecayDb = (volDecayRaw < 0.0) ? volDecayRaw
                                                  : volDecayRaw * m_decayScale;
    const double volSustain = SustainDb((dcysusv & kDcysusvSustainMask) >> 8);

    const double modDelay   = DelaySeconds(envval);
    const double modAttack  = AttackSeconds(atkhld & kAtkhldAttackMask);
    const double modHold    = HoldSeconds((atkhld & kAtkhldHoldMask) >> 8);
    const double modDecayDb = DecayDbPerSecond(dcysus & kDcysusvRateMask);
    const double modSustain = 1.0 - SustainDb((dcysus & kDcysusvSustainMask) >> 8) / kFullScaleDb;

    const double initialAtten = AttenuationDb(LoByte(ifatn));
    const double initialCutoff = static_cast<double>(HiByte(ifatn));

    const double lfo1Hz = LfoHz(LoByte(tremfrq));
    const double lfo2Hz = LfoHz(LoByte(fm2frq2));

    const double dt = 1.0 / kNativeSampleRate;

    // Pan: PSST bits 31..24, where 0 = fully RIGHT and 0xFF = fully LEFT [PG].
    const double panNorm = panReg / 255.0;   // 0 = right, 1 = left
    // The chip has the pan as a **plain multiplication**, not a
    // constant-power curve: snd_emu8k.c does  vol_l = psst_pan,
    // vol_r = 255 - psst_pan  and divides both by 256. In the pan centre each
    // channel thus gets half (-6 dB), while sin/cos gives 0.7071 (-3 dB)
    // there. That difference of 3.01 dB matches exactly the flat offset
    // measured against 86Box. Constant power stays as an option
    // (`--pan power`) for comparison.
    const float gainL = m_panLinear
        ? static_cast<float>(panNorm)
        : static_cast<float>(std::sin(panNorm * kPi * 0.5));
    const float gainR = m_panLinear
        ? static_cast<float>(1.0 - panNorm)
        : static_cast<float>(std::cos(panNorm * kPi * 0.5));

    // Filter resonance: CCCA bits 31..28, 0 = no resonance,
    // 15 = about 24 dB of resonance [PG].
    const double resonanceDb = filterQ * (m_resonanceDb / kCccaQMax);
    // The base from which the resonance rises. Originally there was
    //     max(0.7071, pow(10, res/20))
    // but `pow` is always >= 1 for res >= 0, so 0.7071 never applied and
    // Q = 0 gave Q = 1.0, i.e. a ~1.25 dB bump at the cutoff. If Q = 0 is to
    // mean "no resonance", the base is 0.7071 (Butterworth) and the
    // resonance **multiplies**. The default 1.0 keeps the original behaviour
    // so that both can be measured.
    const double qFactor = m_qBase * std::pow(10.0, resonanceDb / 20.0);
    // Manual: with Q = 0 and the filter fully open the signal is not changed
    // at all.
    const bool bypassFilter = (filterQ == 0);
    // The chip pays for raising Q with an attenuation at the filter input
    // (see kFilterAtten). Without it resonant patches play much louder than
    // they should.
    const double filterAttenRaw =
        kFilterAtten[std::clamp(filterQ, 0, 15)] / 65536.0;
    // `m_filterAtten` is a power, so 0.5 means half the attenuation in dB
    // and 0 none. Used to measure whether the chip really attenuates the
    // whole band or only levels the peak at the cutoff - see
    // docs/re-notes/emu8000_tuning.md.
    const double filterInputGain =
        (m_filterAtten == 1.0) ? filterAttenRaw
                               : std::pow(filterAttenRaw, m_filterAtten);

    for (uint32_t i = 0; i < numFrames; ++i)
    {
        // ---- volume envelope ------------------------------------------
        switch (vs.volStage)
        {
        case EnvStage::Delay:
            vs.volDb = kFullScaleDb;
            if ((vs.stageTime += dt) >= volDelay) { vs.stageTime = 0.0; vs.volStage = EnvStage::Attack; }
            break;
        case EnvStage::Attack:
            // Attack is linear in amplitude, decay/release in dB - SoundFont
            // defines it that way too and it matches the EMU8000 behaviour.
            if (volAttack > 0.0) vs.volLin += dt / volAttack;
            if (vs.volLin >= 1.0) { vs.volLin = 1.0; vs.stageTime = 0.0; vs.volStage = EnvStage::Hold; }
            {
                // volLin is the attack PHASE (0..1 of the attack time). The
                // amplitude follows the shape measured on the tester's card
                // (AWETST25 block 8, rates 0x04..0x24, internal capture and
                // line-out agree within 0.05): silent up to ~0.1 T, roughly
                // linear to ~0.75 at 0.8 T, faster to 1.0 at 1.0 T. Values
                // are amplitude / level at 1.0 T, one point per 0.05 T. The
                // small overshoot after the attack (+3..6 % up to ~1.2 T)
                // is not modelled.
                static constexpr double kAttackShape[21] = {
                    0.000, 0.000, 0.005, 0.058, 0.116, 0.203, 0.280, 0.326,
                    0.372, 0.433, 0.493, 0.537, 0.580, 0.621, 0.662, 0.703,
                    0.744, 0.807, 0.899, 0.947, 1.000 };
                const double pos = std::clamp(vs.volLin, 0.0, 1.0) * 20.0;
                const int idx = std::min(static_cast<int>(pos), 19);
                const double amp = kAttackShape[idx]
                                 + (kAttackShape[idx + 1] - kAttackShape[idx]) * (pos - idx);
                vs.volDb = (amp > 0.0) ? -20.0 * std::log10(amp) : kFullScaleDb;
            }
            break;
        case EnvStage::Hold:
            vs.volDb = 0.0;
            if ((vs.stageTime += dt) >= volHold) { vs.stageTime = 0.0; vs.volStage = EnvStage::Decay; }
            break;
        case EnvStage::Decay:
            if (volDecayDb < 0.0) { vs.volStage = EnvStage::Sustain; break; }
            vs.volDb += volDecayDb * dt;
            if (vs.volDb >= volSustain) { vs.volDb = volSustain; vs.volStage = EnvStage::Sustain; }
            break;
        case EnvStage::Sustain:
            vs.volDb = volSustain;
            break;
        case EnvStage::Release:
            // Rate 0 = "no decay"; on a real chip the voice would keep
            // sounding, but the driver always writes a non-zero release rate
            // on Note Off.
            //
            // Release falls to the SUSTAIN level, not to silence. The driver
            // writes sustain 0 at note-off (Synth::ReleaseVoice), so in music
            // it is the same - but on the card a note with sustain 0x7F does
            // not fall at all. Measured 2026-09-09, block 12 of the tester's
            // recording.
            vs.volDb += (volDecayDb < 0.0 ? kFullScaleDb : volDecayDb) * dt;
            if (volSustain >= kFullScaleDb - kSustainDbPerStep)
            {
                // Sustain 0 - what the driver writes at every note-off. The
                // behaviour must stay exactly as before, otherwise the moment
                // the voice is freed shifts, and with it the whole voice
                // allocation.
                if (vs.volDb >= kFullScaleDb)
                {
                    vs.volDb = kFullScaleDb;
                    vs.volStage = EnvStage::Off;
                    vs.playing = false;
                    return;
                }
            }
            else if (vs.volDb >= volSustain)
            {
                // Higher sustain: the level stops there and the note holds.
                // Measured on the card with block 12, where AWETEST wrote
                // sustain 0x7F and the note did not fall at all.
                vs.volDb = volSustain;
            }
            break;
        default:
            return;
        }

        // ---- modulation envelope --------------------------------------
        switch (vs.modStage)
        {
        case EnvStage::Delay:
            if ((vs.modStageTime += dt) >= modDelay) { vs.modStageTime = 0.0; vs.modStage = EnvStage::Attack; }
            break;
        case EnvStage::Attack:
            if (modAttack > 0.0)
            {
                // The card's mod envelope attack is strongly convex (AWETST25
                // block 30: noise through the filter, PEFE 0x7F, cutoff 64,
                // rates 0x10..0x3C). The level read back from the brightness
                // is ~0.45 at 0.05 T and ~0.76 at 0.1 T for every rate (above
                // ~0.7 the measurement saturates); a linear ramp gives 0.05
                // and 0.1. Modelled as 1 - (1 - t/T)^13.5. modStageTime is
                // zero on entering the stage and serves as the attack phase.
                static constexpr double kModAttackExp = 13.5;
                vs.modStageTime += dt;
                const double x = std::min(vs.modStageTime / modAttack, 1.0);
                vs.modLevel = 1.0 - std::pow(1.0 - x, kModAttackExp);
            }
            if (vs.modLevel >= 1.0) { vs.modLevel = 1.0; vs.modStageTime = 0.0; vs.modStage = EnvStage::Hold; }
            break;
        case EnvStage::Hold:
            if ((vs.modStageTime += dt) >= modHold) { vs.modStageTime = 0.0; vs.modStage = EnvStage::Decay; }
            break;
        case EnvStage::Decay:
            if (modDecayDb < 0.0) { vs.modStage = EnvStage::Sustain; break; }
            vs.modLevel -= (modDecayDb / kFullScaleDb) * dt;
            if (vs.modLevel <= modSustain) { vs.modLevel = modSustain; vs.modStage = EnvStage::Sustain; }
            break;
        case EnvStage::Release:
            if (modDecayDb > 0.0)
                vs.modLevel = std::max(0.0, vs.modLevel - (modDecayDb / kFullScaleDb) * dt);
            break;
        default:
            break;
        }

        // ---- LFO -------------------------------------------------------
        double lfo1 = 0.0, lfo2 = 0.0;
        if (vs.lfo1Delay > 0.0) vs.lfo1Delay -= dt;
        else { lfo1 = LfoTriangle(vs.lfo1Phase); vs.lfo1Phase += lfo1Hz * dt; }
        if (vs.lfo2Delay > 0.0) vs.lfo2Delay -= dt;
        else { lfo2 = LfoTriangle(vs.lfo2Phase); vs.lfo2Phase += lfo2Hz * dt; }
        if (vs.lfo1Phase >= 1.0) vs.lfo1Phase -= 1.0;
        if (vs.lfo2Phase >= 1.0) vs.lfo2Phase -= 1.0;

        // ---- pitch -----------------------------------------------------
        // Depths per the Programmer's Guide: 0x7F = full positive depth,
        // 0x80 = full negative. All three are +-1 octave.
        constexpr double kOct = kPitchPerOctave / 127.0;
        double pitch = static_cast<double>(ipReg);
        pitch += vs.modLevel * HiSigned(pefe)    * kOct * kPefePitchOctaves;
        pitch += lfo1        * HiSigned(fmmod)   * kOct * kFmmodPitchOctaves;
        pitch += lfo2        * HiSigned(fm2frq2) * kOct * kFm2PitchOctaves;

        const double increment = std::pow(2.0,
            (pitch - static_cast<double>(kPitchUnity)) / static_cast<double>(kPitchPerOctave));

        // ---- sample ----------------------------------------------------
        float sample = 0.0f;
        if (vs.playing)
        {
            // "the actual audio location is the point 1 word higher than this
            // value due to interpolator offset" [PG] - applies to CCCA and to
            // both loop ends.
            //
            // The second interpolation sample has to wrap back into the loop.
            // Without that, data PAST the loop was read at its end, which
            // made a discontinuity on every pass - a periodic click and
            // broadband noise in the high frequencies.
            // The offset may be negative too - the windowed sinc is symmetric
            // around the playing position, so it reaches before the current
            // sample as well.
            auto tap = [&](int offset) -> double
            {
                int64_t a = static_cast<int64_t>(vs.address) + offset;
                if (a < 0) a = 0;
                uint32_t ua = static_cast<uint32_t>(a);
                if (m_loopWrap && loopEnd > loopStart)
                    while (ua > loopEnd) ua -= (loopEnd - loopStart);
                return ReadSample(ua) / 32768.0;
            };

            const double f = vs.frac / 65536.0;
            if (m_interp == Interp::Cubic)
            {
                // Catmull-Rom over four points, like the 86Box reference
                // implementation. The points are 0,1,2,3 (not -1..2) because
                // of the one-word interpolator offset.
                const double d0 = tap(0), d1 = tap(1), d2 = tap(2), d3 = tap(3);
                const double c0 = -0.5 * f * f * f + f * f - 0.5 * f;
                const double c1 =  1.5 * f * f * f - 2.5 * f * f + 1.0;
                const double c2 = -1.5 * f * f * f + 2.0 * f * f + 0.5 * f;
                const double c3 =  0.5 * f * f * f - 0.5 * f * f;
                sample = static_cast<float>(d0 * c0 + d1 * c1 + d2 * c2 + d3 * c3);
            }
            else if (m_interp == Interp::Sinc)
            {
                // Windowed sinc (Blackman window) over `m_sincTaps` points.
                // The playing position lies between taps 1 and 2 (the
                // interpolator offset of one word), so the kernel is taken
                // symmetrically around it: -2 to 5 for eight points, -6 to 9
                // for sixteen.
                const int    half = m_sincTaps / 2;
                const int    lo   = -(half - 2);
                const int    hi   = half + 1;
                const double sirka = static_cast<double>(m_sincTaps - 1);
                const double x = 1.0 + f;
                double acc = 0.0, norm = 0.0;
                for (int i = lo; i <= hi; ++i)
                {
                    const double d = x - static_cast<double>(i);
                    double w;
                    if (std::abs(d) < 1e-9)
                    {
                        w = 1.0;
                    }
                    else
                    {
                        const double pd = kPi * d;
                        // Blackman window over the whole kernel width
                        const double t = (d + sirka * 0.5) / sirka;
                        const double bw = 0.42 - 0.5 * std::cos(2.0 * kPi * t)
                                        + 0.08 * std::cos(4.0 * kPi * t);
                        w = std::sin(pd) / pd * bw;
                    }
                    acc += tap(i) * w;
                    norm += w;
                }
                sample = static_cast<float>(norm > 1e-9 ? acc / norm : acc);
            }
            else if (m_interp == Interp::Point3 || m_interp == Interp::Point3c)
            {
                // Quadratic over three points (Lagrange). The AWE32
                // documentation mentions "3 Point sample interpolation" for
                // the chip, so this is closer to the real thing than linear
                // or Catmull-Rom.
                //
                // The playing position lies between the second and third tap
                // (the interpolator offset of one word), so for the forward
                // variant these are taps 1,2,3 and f runs from 0 to 1 between
                // taps 1 and 2.
                const int b = (m_interp == Interp::Point3) ? 1 : 0;
                const double a0 = tap(b), a1 = tap(b + 1), a2 = tap(b + 2);
                const double g = (m_interp == Interp::Point3) ? f : (f + 1.0);
                const double l0 = 0.5 * (g - 1.0) * (g - 2.0);
                const double l1 = -g * (g - 2.0);
                const double l2 = 0.5 * g * (g - 1.0);
                sample = static_cast<float>(a0 * l0 + a1 * l1 + a2 * l2);
            }
            else
            {
                const double s0 = tap(1), s1 = tap(2);
                sample = static_cast<float>(s0 + (s1 - s0) * f);
            }

            const uint64_t step = static_cast<uint64_t>(increment * 65536.0);
            uint64_t pos = (static_cast<uint64_t>(vs.address) << 16) | vs.frac;
            pos += step;
            vs.address = static_cast<uint32_t>(pos >> 16);
            vs.frac = static_cast<uint32_t>(pos & 0xFFFF);

            if (loopEnd > loopStart && vs.address >= loopEnd)
                vs.address -= (loopEnd - loopStart);
        }

        // ---- filter ---------------------------------------------------
        // Base from the register. Two mappings, because the sources disagree:
        // the Programmer's Guide gives the end points 125 Hz and 8 kHz (and
        // contradicts itself with "in quarter semitones"), Vu's guide says
        // directly
        //     f = 100 Hz + register * 31.25 Hz,
        // i.e. **linear in Hz**. At register 128 that is a factor of four.
        // In both cases the modulations are in octaves, as the manual gives
        // them: PEFE lo +-6 octaves, FMMOD lo +-3 octaves.
        const double baseHz = m_cutoffLinear
            ? (kCutoffLinearBaseHz + initialCutoff * kCutoffLinearStepHz)
            : CutoffHz(CutoffOctaves(initialCutoff, m_filterTopHz, m_cutoffBaseHz),
                       m_cutoffBaseHz);
        double octaves = 0.0;
        octaves += vs.modLevel * LoSigned(pefe)  / 127.0 * kPefeFilterOctaves;
        octaves += lfo1        * LoSigned(fmmod) / 127.0 * kFmmodFilterOctaves;
        // Cutoff shift with growing Q (tuning, default 0). A joint fit of
        // blocks 7 and 28 of run5 gives 0.16 octave down at Q 15 - see
        // SetQCutoffShift.
        if (m_qCutoffShiftOct != 0.0)
            octaves -= m_qCutoffShiftOct * filterQ / 15.0;

        double filtered;
        const double filterIn = sample * filterInputGain;

        if (m_filter86)
        {
            // Exactly what snd_emu8k.c does - the FILTER_MOOG branch, which
            // is really active in that file (FILTER_INITIAL, which we ported
            // before, is commented out with #if 0 there and never runs; the
            // first attempt therefore improved nothing in the measurement).
            // A four-stage cascade of one-pole filters with feedback (Moog
            // ladder). The filter is bypassed only with Q == 0 **and** all
            // 16 bits of the cutoff at 0xFFFF - the driver writes cutoff<<8,
            // so that never happens and the filter runs even at "fully
            // open".
            const int qidx = std::clamp(filterQ, 0, 15);
            const int cidx = std::clamp(static_cast<int>(initialCutoff), 0, 255);
            const uint16_t ctoff16 = static_cast<uint16_t>(cidx << 8);
            if (qidx == 0 && ctoff16 == 0xFFFF)
            {
                filtered = sample;
            }
            else
            {
                const double fc = Cutoff86Hz(cidx) * std::pow(2.0, octaves);
                // Lower clamp at the register-0 cutoff - see the TPT branch below.
                const double w0 = std::sin(2.0 * kPi
                                           * std::clamp(fc, Cutoff86Hz(0), kNativeSampleRate * 0.49)
                                           / kNativeSampleRate);
                const double qFactor86 = 1.0 - w0;
                const double p = w0 + 0.8 * w0 * qFactor86;
                const double coef0 = p;
                const double coef1 = p + p - 1.0;
                const double resonance = (1.0 - std::pow(2.0, -qidx * 24.0 / 90.0)) * 0.8;
                const double coef2 = resonance
                    * (1.0 + 0.5 * qFactor86 * (w0 + 5.6 * qFactor86 * qFactor86));

                // We work in normalised units (full scale = 1.0), which is
                // algebraically the same as the fixed-point <<8/>>24 in 86Box
                // - just without the rounding loss. The "double range" of the
                // clipping there corresponds to 2.0 here.
                auto clip2 = [](double v) { return std::clamp(v, -2.0, 2.0); };

                const double x = sample - coef2 * vs.filtIc5;
                const double t1 = vs.filtIc2;
                vs.filtIc2 = clip2((x + vs.filtIc1) * coef0 - vs.filtIc2 * coef1);
                const double t2 = vs.filtIc3;
                vs.filtIc3 = clip2((vs.filtIc2 + t1) * coef0 - vs.filtIc3 * coef1);
                const double t3 = vs.filtIc4;
                vs.filtIc4 = clip2((vs.filtIc3 + t2) * coef0 - vs.filtIc4 * coef1);
                vs.filtIc5 = clip2((vs.filtIc4 + t3) * coef0 - vs.filtIc5 * coef1);
                vs.filtIc1 = clip2(x);
                filtered = vs.filtIc5;
            }
        }
        else if (bypassFilter && initialCutoff >= 255.0 - 1e-9 && octaves >= -1e-9)
        {
            // "If the Q of the channel is programmed to zero and the filter
            // cutoff to 0xFF, the filter does not alter the signal." [PG]
            filtered = sample;   // Q=0 and fully open: unchanged
        }
        else if (m_filterCham)
        {
            // Chamberlin SVF (Dattorro form): L += F*B; H = in - L - q*B; B += F*H.
            // Cutoff comes from our map with the same lower clamp (register 0)
            // as the TPT branch below. F is capped at 1 (fc = fs/6): the fit of
            // the card sits there too, and with this damping the filter is
            // nearly flat across the band at F = 1 - which is why the card
            // barely filters at the highest cutoffs.
            const double cutoffFloorHz = m_cutoffLinear ? kCutoffLinearBaseHz
                                                        : m_cutoffBaseHz;
            const double cutoffHz = std::clamp(baseHz * std::pow(2.0, octaves),
                                               cutoffFloorHz, kNativeSampleRate / 6.0);
            const double F = 2.0 * std::sin(kPi * cutoffHz / kNativeSampleRate);
            const double qd = 1.0 / (kChamQ0 * std::pow(10.0, filterQ * kChamDbPerQ / 20.0));
            vs.filtIc1 += F * vs.filtIc2;                         // low-pass state
            const double hp = filterIn - vs.filtIc1 - qd * vs.filtIc2;
            vs.filtIc2 += F * hp;                                 // band-pass state
            filtered = vs.filtIc1;
        }
        else
        {
            // The lower clamp is the cutoff of REGISTER 0, not 20 Hz. Measured
            // on the card (run5 block 28, PEFE -128..-64 at cutoff 128): closing
            // further no longer changes the level, the plateau sits at the
            // register-0 cutoff (fit 1.16 dB; with a 20 Hz clamp 10.36 dB).
            // 86Box clamps `filtercut` to the register range too. The upper
            // clamp is unchanged - it cannot be seen in the card measurements.
            const double cutoffFloorHz = m_cutoffLinear ? kCutoffLinearBaseHz
                                                        : m_cutoffBaseHz;
            const double cutoffHz = std::clamp(baseHz * std::pow(2.0, octaves),
                                               cutoffFloorHz, kNativeSampleRate * 0.49);

            // Topology-preserving state variable filter. The Chamberlin
            // variant goes unstable at higher cutoffs (the condition
            // f + 1/Q < 2 no longer holds at 4 kHz and Q=0.707); this one is
            // stable up to Nyquist.
            const double g = std::tan(kPi * cutoffHz / kNativeSampleRate);
            // With the awe32faq curve the resonance is computed only here,
            // because it depends on the **current** filter cutoff (i.e. on
            // the modulation too).
            double qNow = qFactor;
            // Only for Q > 0. The "Coeff 0" row of the awe32faq table gives
            // 5 dB at a low cutoff, but "Flat" in the second column - that is
            // hardly resonance when Q is zero. Measured: when that row was
            // applied, 779 notes of DANCE.MID got a resonance of 2.6 dB they
            // did not have before, and both Dance recordings got worse
            // (4.122 -> 4.168).
            if (m_resonanceCurve && filterQ > 0)
            {
                const int qi = std::clamp(filterQ, 0, 15);
                const double t = std::clamp(
                    std::log2(cutoffHz / Emu8000::kResonanceLowHz)
                        / std::log2(Emu8000::kResonanceHighHz
                                    / Emu8000::kResonanceLowHz), 0.0, 1.0);
                const double db = Emu8000::kResonanceLowDb[qi] * (1.0 - t)
                                + Emu8000::kResonanceHighDb[qi] * t;
                qNow = m_qBase * std::pow(10.0, db / 20.0);
            }
            const double k = 1.0 / qNow;
            const double a1 = 1.0 / (1.0 + g * (g + k));
            const double a2 = g * a1;
            const double a3 = g * a2;

            if (m_filterPoles == 1)
            {
                // one-pole (6 dB/oct) - to compare how steep a filter the
                // real card actually has
                const double a = g / (1.0 + g);
                vs.filtLp1 += a * (filterIn - vs.filtLp1);
                filtered = vs.filtLp1;
            }
            else
            {
                const double v3 = filterIn - vs.filtIc2;
                const double v1 = a1 * vs.filtIc1 + a2 * v3;
                const double v2 = vs.filtIc2 + a2 * vs.filtIc1 + a3 * v3;
                vs.filtIc1 = 2.0 * v1 - vs.filtIc1;
                vs.filtIc2 = 2.0 * v2 - vs.filtIc2;
                filtered = v2;   // low-pass output

                if (m_filterPoles >= 4)
                {
                    // a second identical stage in cascade = 24 dB per octave
                    const double w3 = filtered - vs.filtIc4;
                    const double w1 = a1 * vs.filtIc3 + a2 * w3;
                    const double w2 = vs.filtIc4 + a2 * vs.filtIc3 + a3 * w3;
                    vs.filtIc3 = 2.0 * w1 - vs.filtIc3;
                    vs.filtIc4 = 2.0 * w2 - vs.filtIc4;
                    filtered = w2;
                }
            }
        }

        // ---- volume -----------------------------------------------------
        // Tremolo: TREMFRQ bits 15..8. Only attenuation, and only in the half
        // period where lfo * depth < 0 - see kTremoloChipMaxDb (formerly
        // +-6 dB around zero, which amplified in the other half period).
        double db = vs.volDb + initialAtten;
        db += std::max(0.0, -lfo1 * HiSigned(tremfrq)) * (kTremoloChipMaxDb / 127.0);
        const double gain = DbToLinear(db);

        const float out = static_cast<float>(filtered * gain);
        outL[i] += out * gainL;
        outR[i] += out * gainR;

        // The sends come from the voice output before the pan, i.e. mono.
        sendRev[i] += out * revSend;
        sendCho[i] += out * choSend;
    }
}

void Emu8000Core::UpdateRegistersFromState(int v)
{
    // So that host code which reads the registers (e.g. a reverse-engineered
    // game) sees a meaningful current state.
    //
    // This does not belong in the trace - these are write-backs of the core
    // state, not driver actions, and there would be 32 of them per sample.
    // 86Box computes the same values itself.
    const TraceOff noTrace(*this);

    const VoiceState& vs = m_voices[v];
    const uint32_t ccca = Read(Reg::CCCA, v);
    Write(Reg::CCCA, v, (ccca & ~kCccaAddressMask) | (vs.address & kCccaAddressMask));

    const uint16_t ifatn = RegVal(Port::Data3, 1, v);
    const double gain = DbToLinear(vs.volDb + AttenuationDb(LoByte(ifatn)));
    const uint16_t curVol = static_cast<uint16_t>(std::clamp(gain, 0.0, 1.0) * 65535.0);
    RegRef(Port::Data0Hi, 2, v) = curVol;   // CVCF hi16 = current volume
    // VTFT hi16 = volume target. The `dos` driver reads it to pick a voice,
    // so a silent voice must read 0 (86Box: vtft_vol_target).
    RegRef(Port::Data0Hi, 3, v) = (vs.volStage == EnvStage::Off) ? uint16_t{0} : curVol;

    // CPF: the upper half is a LINEAR increment (0x4000 = 1.0), the lower is
    // the fractional part of the address [PG]. IP, by contrast, is
    // logarithmic.
    const double increment = std::pow(2.0,
        (static_cast<double>(RegVal(Port::Data3, 0, v)) - kPitchUnity) / kPitchPerOctave);
    const uint16_t cpfHi = static_cast<uint16_t>(
        std::clamp(increment * kCpfUnity, 0.0, 65535.0));
    RegRef(Port::Data0Hi, 0, v) = cpfHi;
    RegRef(Port::Data0,   0, v) = static_cast<uint16_t>(vs.frac);
}

void Emu8000Core::RenderNative(float* outL, float* outR, uint32_t numFrames)
{
    std::fill(outL, outL + numFrames, 0.0f);
    std::fill(outR, outR + numFrames, 0.0f);

    m_sendReverb.assign(numFrames, 0.0f);
    m_sendChorus.assign(numFrames, 0.0f);

    for (int v = 0; v < kMaxVoices; ++v)
    {
        RenderVoice(v, outL, outR, m_sendReverb.data(), m_sendChorus.data(), numFrames);
        UpdateRegistersFromState(v);
    }

    // Chorus goes to the output and also feeds the reverb, as in the signal
    // diagram of the chip.
    //
    // Return levels. The reverb normalises its gain itself (see
    // Emu8000Effects.h), so the amount of effect is decided only by the send
    // from the register. The values are empirical (fitted to recordings), not
    // derived from the hardware.
    const float kReverbReturn = m_reverbReturn;
    const float kChorusReturn = m_chorusReturn;

    // The effect presets follow the INIT words the driver wrote.
    UpdateEffectPresets();

    for (uint32_t i = 0; i < numFrames; ++i)
    {
        float cl, cr;
        m_chorus.Process(m_sendChorus[i], cl, cr);
        cl *= kChorusReturn;
        cr *= kChorusReturn;
        outL[i] += cl;
        outR[i] += cr;
        m_sendReverb[i] += (cl + cr) * 0.5f;

        float rl, rr;
        m_reverb.Process(m_sendReverb[i], rl, rr);
        outL[i] += rl * kReverbReturn;
        outR[i] += rr * kReverbReturn;
    }

    // The equalizer comes after the effects, on the whole chip output
    // (Programmer's Guide: bass/treble at the DSP output). The position comes
    // from the INIT3/INIT4 registers, so it also fits another driver or a
    // game that changes the EQ.
    if (m_eqOn)
    {
        UpdateEqualizer();
        for (uint32_t i = 0; i < numFrames; ++i)
            m_eq.Process(outL[i], outR[i]);
    }

    m_waveCounter += numFrames;
    m_traceFrames += numFrames;
}

void Emu8000Core::UpdateEqualizer()
{
    // Slot order as in snd_emu8000_update_equalizer (alsa_emu8000_init.c):
    // INIT4 0x01, 0x11 (bass), INIT3 0x11, 0x13, 0x1B, INIT4 0x07, 0x0B, 0x0D,
    // 0x17, 0x19 (treble).
    const uint16_t v[10] = {
        RegVal(Port::Data1Hi, 3, 0x01), RegVal(Port::Data1Hi, 3, 0x11),
        RegVal(Port::Data1,   3, 0x11), RegVal(Port::Data1,   3, 0x13),
        RegVal(Port::Data1,   3, 0x1B), RegVal(Port::Data1Hi, 3, 0x07),
        RegVal(Port::Data1Hi, 3, 0x0B), RegVal(Port::Data1Hi, 3, 0x0D),
        RegVal(Port::Data1Hi, 3, 0x17), RegVal(Port::Data1Hi, 3, 0x19),
    };
    int bass = m_eq.Bass(), treble = m_eq.Treble();
    Emu8000Fx::EqIndexFromInit(v, bass, treble);
    m_eq.Set(bass, treble);
}

void Emu8000Core::UpdateEffectPresets()
{
    // INIT1/INIT2 are register 2, INIT3/INIT4 register 3; the odd ones are
    // written through Data1, the even ones through Data2 (Data1Hi here).
    const auto init = [this](int n, int slot) {
        return RegVal((n % 2) ? Port::Data1 : Port::Data1Hi, (n <= 2) ? 2 : 3, slot);
    };
    if (m_revFromRegs)
    {
        uint16_t v[28];
        for (int i = 0; i < 28; ++i)
            v[i] = init(Emu8000Fx::kReverbSlots[i].init, Emu8000Fx::kReverbSlots[i].slot);
        const int p = Emu8000Fx::ReverbPresetFromInit(v);
        if (p >= 0 && p != m_revDecoded) { m_revDecoded = p; m_reverb.SetPreset(p); }
    }
    if (m_choFromRegs)
    {
        const int p = Emu8000Fx::ChorusPresetFromInit(init(3, 0x09), init(3, 0x0C), init(4, 0x03));
        if (p >= 0 && p != m_choDecoded) { m_choDecoded = p; m_chorus.SetPreset(p); }
    }
}

bool Emu8000Core::UseBox86Chip(const std::string& romPath, std::string& err)
{
    // 86Box allocates the DRAM itself; the size decides where uploads wrap.
    if (!m_box.Init(romPath, m_basePort + 0x400, m_chipRamKb, err))
        return false;
    m_chip = Chip::Box86;
    return true;
}

uint32_t Emu8000Core::ChipLatencyFrames() const
{
    return (m_chip == Chip::Box86) ? Emu8000Box::kLatencyFrames : 0u;
}

int16_t* Emu8000Core::ChipRam()
{
    return m_box.Ram();
}

size_t Emu8000Core::ChipRamWords() const
{
    return m_box.RamWords();
}

void Emu8000Core::RenderBlock(int16_t* out, uint32_t numFrames)
{
    if (numFrames == 0) return;

    auto emit = [&](uint32_t i, float l, float r)
    {
        const float sl = std::clamp(l, -1.0f, 1.0f);
        const float sr = std::clamp(r, -1.0f, 1.0f);
        out[i * 2 + 0] = static_cast<int16_t>(sl * 32767.0f);
        out[i * 2 + 1] = static_cast<int16_t>(sr * 32767.0f);
    };

    if (m_outputRate == kNativeSampleRate)
    {
        m_nativeL.resize(numFrames);
        m_nativeR.resize(numFrames);
        RenderNative(m_nativeL.data(), m_nativeR.data(), numFrames);
        if (m_chip == Chip::Box86)
        {
            // 86Box gives int32 directly and clips it to int16 - no conversion
            // through float, so the result can be compared byte by byte with
            // emu8k_ref.exe.
            for (uint32_t i = 0; i < numFrames; ++i)
            {
                int32_t l = 0;
                int32_t r = 0;
                m_box.RenderFrame(l, r);
                out[i * 2 + 0] = static_cast<int16_t>(std::clamp(l, -32768, 32767));
                out[i * 2 + 1] = static_cast<int16_t>(std::clamp(r, -32768, 32767));
            }
            return;
        }
        for (uint32_t i = 0; i < numFrames; ++i)
            emit(i, m_nativeL[i], m_nativeR[i]);
        return;
    }

    // Linear resampling from the native 44100 Hz to the output rate.
    // m_nativeL/R serve as a carry buffer between calls.
    const double ratio = static_cast<double>(kNativeSampleRate) / m_outputRate;
    const size_t needed = static_cast<size_t>(
        std::ceil(m_resamplePos + ratio * numFrames)) + 2;

    if (m_nativeL.size() < needed)
    {
        const size_t have = m_nativeL.size();
        m_nativeL.resize(needed);
        m_nativeR.resize(needed);
        RenderNative(m_nativeL.data() + have, m_nativeR.data() + have,
                     static_cast<uint32_t>(needed - have));
    }

    for (uint32_t i = 0; i < numFrames; ++i)
    {
        const double q = m_resamplePos + ratio * i;
        const size_t i0 = static_cast<size_t>(q);
        const float f = static_cast<float>(q - i0);
        const float l = m_nativeL[i0] + (m_nativeL[i0 + 1] - m_nativeL[i0]) * f;
        const float r = m_nativeR[i0] + (m_nativeR[i0 + 1] - m_nativeR[i0]) * f;
        emit(i, l, r);
    }

    m_resamplePos += ratio * numFrames;
    const size_t consumed = static_cast<size_t>(m_resamplePos);
    if (consumed > 0)
    {
        m_nativeL.erase(m_nativeL.begin(), m_nativeL.begin() + consumed);
        m_nativeR.erase(m_nativeR.begin(), m_nativeR.begin() + consumed);
        m_resamplePos -= static_cast<double>(consumed);
    }
}
