#pragma once
#include <cstdint>
#include <cstddef>
#include <array>
#include <vector>
#include <string>
#include "Emu8000Regs.h"
#include "Emu8000Box.h"
#include "Emu8000Effects.h"
#include "Awe32InitArrays.h"
#include "Awe32Driver.h"

// ---------------------------------------------------------------------------
// Emu8000Core - register-level emulation of the EMU8000 chip (Sound Blaster
// AWE32).
//
// The register map and the initialisation sequence are derived from the
// disassembly of the AWEUTIL.COM driver, see
// docs/re-notes/emu8000_register_map.md.
//
// The class deliberately has TWO interface levels:
//
//   1) Port level (PortOut16/PortIn16) - exactly what a real driver does:
//      OUT to the pointer register + OUT to the data port. This is the path
//      for hooking up a reverse-engineered DOS game that sets the registers
//      itself (see README, use 2).
//
//   2) Register level (WriteReg16/32, Write/Read with the Reg enum) - for
//      our own MIDI player (Synth.cpp), which does not need to simulate the
//      I/O ports.
//
// The chip runs natively at 44100 Hz; RenderBlock can output at another
// rate too (linear resampling), but the internal timing is always 44100.
// ---------------------------------------------------------------------------

class Emu8000Core
{
public:
    static constexpr int kMaxVoices = Emu8000::kMaxVoices;
    static constexpr uint32_t kNativeSampleRate = 44100;

    explicit Emu8000Core(uint32_t outputSampleRate);

    // ---- port level ----------------------------------------------------
    void SetBasePort(uint16_t sbBasePort);          // default 0x220
    uint16_t BasePort() const { return m_basePort; }
    bool OwnsPort(uint16_t port) const;
    void PortOut16(uint16_t port, uint16_t value);
    uint16_t PortIn16(uint16_t port);

    // ---- register level ------------------------------------------------
    void WriteReg16(uint16_t sel, uint16_t value);
    uint16_t ReadReg16(uint16_t sel) const;
    void WriteReg32(uint16_t sel, uint32_t value);   // low word, then high word
    uint32_t ReadReg32(uint16_t sel) const;

    void Write(Emu8000::Reg r, int voice, uint32_t value);
    uint32_t Read(Emu8000::Reg r, int voice) const;
    // Read the way a driver does it: pointer write, then the data port(s)
    // through PortIn16. With the 86Box chip the value comes from the chip
    // itself (current volume target, playback address, ...).
    uint32_t ReadDriver(Emu8000::Reg r, int voice);

    // Initialisation sequence taken from AWEUTIL.COM (sub_12B40 and its
    // subroutines), including the arrays INIT1..INIT4 (Awe32InitArrays.h).
    // Our core reads nothing from the init arrays, but 86Box decodes the
    // reverb and chorus from them, so they must be in the trace - see
    // docs/re-notes/emu8000_register_map.md.
    void PowerOnInit();

    // Which driver family is emulated. Changes eight values in the init
    // arrays INIT3/INIT4 - see Awe32Driver.h. Set before PowerOnInit().
    void SetDriver(Awe32::Driver d) { m_driver = d; }
    Awe32::Driver DriverVariant() const { return m_driver; }

    // ---- sound memory ----------------------------------------------------
    // Addresses in the CCCA/PSST/CSL registers are 24-bit and count samples.
    // The user DRAM starts at Emu8000::kDramOffset; below it lies the card's
    // ROM (reads return 0 unless a ROM image is loaded).
    void ResizeDram(size_t numSamples);
    size_t DramSize() const { return m_dram.size(); }
    int16_t* DramData() { return m_dram.data(); }
    const int16_t* DramData() const { return m_dram.data(); }

    // The card's wave ROM is mapped from address 0. The emulation treats it
    // as an ordinary part of the address space - a voice cannot tell.
    void LoadWaveRom(std::vector<int16_t> rom) { m_rom = std::move(rom); }
    size_t RomSize() const { return m_rom.size(); }

    int16_t ReadSample(uint32_t address) const;

    // ---- render ----------------------------------------------------------
    // out = interleaved stereo int16, numFrames frames at the output rate.
    void RenderBlock(int16_t* out, uint32_t numFrames);

    bool IsVoiceActive(int voice) const;

    // Sample interpolation. `Point3` is the one the documentation mentions
    // ("3 Point sample interpolation", Vu, Un-official AWE32 Programming
    // Guide 1995), and it was the default from 2026-09-02: measured against
    // 20 recording/MIDI pairs (tests/tune.py), average score 5.937 against
    // 6.025 for the original cubic (Catmull-Rom).
    // The two variants differ in which three samples they take: `Point3`
    // reads ahead (taps 1,2,3), `Point3c` is symmetric around the playing
    // position (taps 0,1,2) - they are indistinguishable in the measurement
    // (5.9369 vs 5.9370); `Point3` is kept only for the convention
    // "tap(1) = current sample" used by Linear/Cubic.
    // `Sinc` is an eight-point windowed sinc - sharper than all the others.
    // Not a guess: measured on Hi-Octane, where the match with the hardware
    // grows monotonically with the sharpness of the kernel (linear 4.271 ->
    // Point3 4.187 -> Cubic 3.915), and we also lack energy above 6.4 kHz
    // (6.1 % against 19.1 %), so `Sinc` is the default now. Patent
    // US 5,111,727 describes a FIR designed with the Remez algorithm for the
    // G-chip, which is a **sharp** filter too - quadratic Lagrange
    // interpolation (`Point3`) is too soft by comparison.
    enum class Interp { Linear, Cubic, Point3, Point3c, Sinc };
    void SetInterpolation(Interp i) { m_interp = i; }
    // How many points the windowed sinc takes. Eight is the default
    // (measured); more points = a sharper kernel. Used to test whether the
    // remaining deviation is in the interpolation - it is spread evenly over
    // all instruments, which fits an error depending on the sample and the
    // playback rate.
    void SetSincTaps(int n) { m_sincTaps = n; }

    // Scales of the volume envelope time constants. Used for measurement,
    // because the registers match the driver 100 %, but **how fast** the
    // chip reacts to them is our model from the Programmer's Guide. A
    // breakdown of the residual deviation showed that almost half of it is
    // note volume error and that it is largest 50-150 ms after the onset -
    // exactly in the hold/decay phase.
    void SetHoldScale(double x)  { m_holdScale = x; }
    void SetDecayScale(double x) { m_decayScale = x; }
    void SetAttackScale(double x) { m_attackScale = x; }

    // Filter tuning parameters. The Programmer's Guide contradicts itself on
    // the cutoff frequencies (quarter semitones vs "0xFF = 8 kHz") and we
    // lacked energy above 3 kHz against the real card, so it has to be
    // measurable.
    //   topHz  - frequency at register value 0xFF
    //   poles  - 1, 2 or 4 (6, 12 or 24 dB per octave)
    void SetFilterTopHz(double hz) { m_filterTopHz = hz; }
    // Frequency at register 0 (now 101.81 Hz). The step per register is
    // derived from --filter-top. Fit on run5 (blocks 7 + 28): 117.8 Hz.
    void SetCutoffBaseHz(double hz) { m_cutoffBaseHz = hz; }
    // How many octaves the filter cutoff drops at Q 15; linear between Q 0
    // and 15. On the hardware the peak moves down with Q, our SVF's does
    // not. Fit on run5: 0.16.
    void SetQCutoffShift(double oct) { m_qCutoffShiftOct = oct; }
    // Base of the filter resonance: 1.0 = the original behaviour, 0.7071 =
    // Butterworth at Q = 0. See the qFactor computation in Emu8000.cpp.
    void SetQBase(double q)       { m_qBase = q; }
    // How strongly the attenuation at the filter input applies, with which
    // the chip pays for the raised resonance (table kFilterAtten). 1 = the
    // whole table, 0 = no attenuation. Values in between are a power, so the
    // scale is linear in dB.
    void SetFilterAtten(double x) { m_filterAtten = x; }
    // How many dB of resonance Q = 15 means. The Programmer's Guide says
    // "about 24 dB", the awe32faq rather 21. The difference shows only on
    // notes with Q > 0, and it is exactly where we had too much energy around
    // 7.7 kHz against the hardware.
    void SetResonanceDb(double db) { m_resonanceDb = db; }
    // `true` = the resonance comes from the measured awe32faq table and
    // changes with the filter cutoff (see kResonanceLowDb/kResonanceHighDb),
    // `false` = a single number `Q * kResonanceMaxDb / 15` as before.
    void SetResonanceCurve(bool on) { m_resonanceCurve = on; }
    // Conversion of the IFATN(15..8) register to the cutoff. `false` = the
    // existing exponential one (125 Hz -> 8 kHz over 255 steps), `true` =
    // linear in Hz per Vu's guide:  f = 100 Hz + register * 31.25 Hz.
    // The difference is large - register 128 gives 1006 Hz against 4100 Hz.
    void SetCutoffLinear(bool on)  { m_cutoffLinear = on; }
    // Filter form. `false` = our TPT (stable up to Nyquist),
    // `true` = exactly what snd_emu8k.c in 86Box does:
    //   - coefficient w0 = sin(2*pi*fc/fs), not tan
    //   - cutoff from the table 125 Hz * 1.016378315^index (42.66 steps per
    //     octave)
    //   - input attenuated by Q with the filter_atten table
    //   - the filter is bypassed only when Q == 0 **and** the whole 16-bit
    //     cutoff is 0xFFFF
    // That last condition is the essential difference: the driver writes
    // cutoff<<8, i.e. 0xFF00, so 86Box filters even at "fully open", while
    // we followed the Programmer's Guide and switched the filter off.
    void SetFilter86Box(bool on)   { m_filter86 = on; }
    // Chamberlin SVF (default, measured on the card - see kChamQ0). `false`
    // selects the previous bilinear TPT, driven by --q-base and --resonance-db.
    void SetFilterCham(bool on)    { m_filterCham = on; }
    // Pan curve. `true` (default) = a plain multiplication as in the chip:
    //   left = pan/255, right = (255-pan)/255
    // `false` = constant-power sin/cos, which we had before. In the centre
    // they differ by 3.01 dB, exactly the flat difference measured against
    // 86Box (3.37-3.41 dB on isolated notes).
    void SetPanLinear(bool on)     { m_panLinear = on; }
    // Should the second (third...) interpolation sample wrap back into the
    // loop? We do it, `snd_emu8k.c` does not - its `EMU8K_READ` reads on
    // linearly past the loop end. Wrapping is "cleaner", but it may be what
    // takes away the high frequencies we lack against the hardware (6.1 %
    // against 19.1 % above 6.4 kHz). The default is wrapping; `--loop-wrap
    // off` turns it off.
    void SetLoopWrap(bool on)      { m_loopWrap = on; }
    void SetFilterPoles(int p)     { m_filterPoles = p; }
    double FilterTopHz() const     { return m_filterTopHz; }
    int    FilterPoles() const     { return m_filterPoles; }

    // Tuning access to the effects - the room size and damping of the reverb
    // are not derived from the hardware (the init arrays are DSP
    // coefficients), so they are verified by measurement against reference
    // recordings.
    void SetReverbRoom(float size, float damp) { m_reverb.SetRoom(size, damp); }
    // A preset forced from outside stops following the INIT registers.
    void SetReverbPreset(int p) { m_revFromRegs = false; m_reverb.SetPreset(p); }
    void SetChorusPreset(int p) { m_choFromRegs = false; m_chorus.SetPreset(p); }
    void SetEffectReturns(float rev, float cho) { m_reverbReturn = rev; m_chorusReturn = cho; }
    // The card's equalizer (bass/treble per INIT3/INIT4). On by default -
    // the card always has it on (SDK and game driver: treble +7.9 dB).
    void SetEqualizer(bool on) { m_eqOn = on; }

    // ---- chip variant ----------------------------------------------------
    // `Ours` is our own core (float, tunable filter). `Box86` sends the same
    // port writes to `snd_emu8k.c` from 86Box and takes the sound from there
    // - see Emu8000Box.h. Our core keeps running idle meanwhile, so the voice
    // bookkeeping and thus the register stream stay identical.
    enum class Chip { Ours, Box86 };
    // Onboard DRAM of the 86Box chip in KB (86Box `onboard_ram`); set before
    // UseBox86Chip. The card of the tester has 8 MB, the DOS VM had 512 KB.
    void SetChipRamKb(int kb) { m_chipRamKb = kb; }
    bool UseBox86Chip(const std::string& romPath, std::string& err);
    Chip ChipVariant() const { return m_chip; }
    // How many frames the chip output lags behind (one block with Box86).
    uint32_t ChipLatencyFrames() const;
    int16_t* ChipRam();
    size_t   ChipRamWords() const;

    // ---- port write trace (for comparison with 86Box) --------------------
    // Writes every PortOut16 as "<frame> <port> <value>" on the native
    // 44100 Hz time line. The result can be replayed with emu8k_ref.exe,
    // which runs snd_emu8k.c of 86Box - see docs/TESTING.md.
    bool OpenTrace(const char* path);
    void CloseTrace();

private:
    // Register array: [port][reg][voice], all as 16-bit words.
    // 32-bit registers = the pairs (Data0,Data0Hi) and (Data1,Data1Hi).
    using RegFile = std::array<std::array<std::array<uint16_t, kMaxVoices>,
                                          8>,
                               static_cast<size_t>(Emu8000::Port::Count)>;

    enum class EnvStage { Off, Delay, Attack, Hold, Decay, Sustain, Release };

    struct VoiceState
    {
        // sample playback
        uint32_t address = 0;      // integer part (samples)
        uint32_t frac = 0;         // 16-bit fractional part
        bool     playing = false;
        // Envelope generator engine on (DCYSUSV bit 7 clear), tracked like
        // 86Box env_engine_on: a note starts only on the off -> on transition.
        bool     engineOn = false;

        // volume envelope
        EnvStage volStage = EnvStage::Off;
        double   volDb = 96.0;     // current attenuation in dB (0 = full volume)
        double   volLin = 0.0;     // attack phase 0..1 (amplitude follows kAttackShape)
        double   stageTime = 0.0;  // seconds spent in the current stage

        // modulation envelope
        EnvStage modStage = EnvStage::Off;
        double   modLevel = 0.0;
        double   modStageTime = 0.0;

        // LFO
        double lfo1Phase = 0.0;
        double lfo2Phase = 0.0;
        double lfo1Delay = 0.0;
        double lfo2Delay = 0.0;

        // low-pass filter (topology-preserving SVF, see the note in the .cpp)
        double filtIc1 = 0.0;
        double filtIc2 = 0.0;
        double filtIc3 = 0.0;   // second stage with --filter-poles 4
        double filtIc4 = 0.0;
        double filtLp1 = 0.0;   // one-pole filter with --filter-poles 1
        double filtIc5 = 0.0;   // fifth stage for --filter-mode 86box (FILTER_MOOG)
    };

    void RenderNative(float* outL, float* outR, uint32_t numFrames);
    uint16_t PortIn16Raw(uint16_t port);
    void UpdateEqualizer();
    // Follows the reverb/chorus preset written to INIT1..INIT4 (see
    // Emu8000Fx::ReverbPresetFromInit). Disabled for an effect once its
    // preset is forced from outside (SetReverbPreset / SetChorusPreset).
    void UpdateEffectPresets();
    bool m_revFromRegs = true;
    bool m_choFromRegs = true;
    int  m_revDecoded = -1;
    int  m_choDecoded = -1;
    void RenderVoice(int v, float* outL, float* outR, float* sendRev, float* sendCho,
                     uint32_t numFrames);
    void UpdateRegistersFromState(int v);
    void SendInitArray(const uint16_t* data, const Awe32Init::AltInit* alt = nullptr);

    uint16_t& RegRef(Emu8000::Port p, int reg, int voice);
    uint16_t  RegVal(Emu8000::Port p, int reg, int voice) const;

    uint32_t m_outputRate;
    // Measured on all 23 usable pairs: sinc 5.2784, cubic 5.2856, point3
    // 5.3212. The sinc vs. cubic difference comes **only** from Hi-Octane
    // (4.138 and 4.128 against 4.221 and 4.265) - on the rest sinc is ~0.007
    // worse. We take it because Hi-Octane is the cleanest material we have.
    Interp m_interp = Interp::Sinc;
    int    m_sincTaps = 8;
    double m_holdScale   = 1.0;
    double m_decayScale  = 1.0;
    double m_attackScale = 1.0;
    double m_filterTopHz = 8000.0;
    double m_cutoffBaseHz = Emu8000::kCutoffBaseHz;
    double m_qCutoffShiftOct = 0.0;
    double m_qBase       = 1.0;
    double m_filterAtten = 1.0;
    double m_resonanceDb = Emu8000::kResonanceMaxDb;
    bool   m_resonanceCurve = false;
    bool   m_cutoffLinear = false;
    bool   m_filter86     = false;
    bool   m_filterCham   = true;
    bool   m_panLinear    = true;
    bool   m_loopWrap     = true;
    int    m_filterPoles = 2;
    float m_reverbReturn = 1.0f;
    float m_chorusReturn = 0.7f;
    bool  m_eqOn = true;
    uint16_t m_basePort = 0x220;
    uint16_t m_pointer = 0;      // last write to the pointer register
    Awe32::Driver m_driver = Awe32::kDefaultDriver;

    Chip m_chip = Chip::Ours;
    Emu8000Box m_box;
    int  m_chipRamKb = 8192;

    RegFile m_regs{};
    std::array<VoiceState, kMaxVoices> m_voices{};
    std::vector<int16_t> m_dram;
    std::vector<int16_t> m_rom;

    // Wave counter (register WC) - a free-running sample counter. Drivers
    // use it as the time base in wait loops (see AWEUTIL sub_127AE), so we
    // have to tick it, otherwise the driver would hang.
    uint32_t m_waveCounter = 0;

    // Port write trace (see OpenTrace).
    void* m_traceFile = nullptr;   // FILE*, held as void* to keep <cstdio> out of here
    uint64_t m_traceFrames = 0;    // frames rendered at 44100 Hz
    bool m_traceOff = false;

    // RAII switch for writes that must not go into the trace (see
    // UpdateRegistersFromState).
    struct TraceOff
    {
        Emu8000Core& c;
        bool prev;
        explicit TraceOff(Emu8000Core& core) : c(core), prev(core.m_traceOff) { c.m_traceOff = true; }
        ~TraceOff() { c.m_traceOff = prev; }
    };

    // Effect buses. The send is taken from the voice output before the pan
    // (see the signal diagram in the Programmer's Guide), so they are mono.
    Emu8000Fx::Chorus m_chorus;
    Emu8000Fx::Reverb m_reverb;
    Emu8000Fx::Equalizer m_eq;
    std::vector<float> m_sendReverb, m_sendChorus;

    // resampling to the output rate
    std::vector<float> m_nativeL, m_nativeR;
    double m_resamplePos = 0.0;
    float m_lastL = 0.0f, m_lastR = 0.0f;
};
