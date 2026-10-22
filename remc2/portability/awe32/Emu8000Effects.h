#pragma once
#include <cstdint>
#include <cmath>
#include <vector>
#include <algorithm>

// ---------------------------------------------------------------------------
// Chorus a reverb EMU8000.
//
// NOTE the scope of this part: the routing is register-exact (the send of
// each voice is taken from PTRX bits 15..8 for reverb and from CSL bits
// 31..24 for chorus, before the pan - see the signal diagram in the
// Programmer's Guide), but the ALGORITHM of the two effects itself is not
// exact and cannot be:
//
// The EMU8000 effects processor is a fixed function configured by the arrays
// INIT1..INIT4. Those arrays are raw coefficients of an internal DSP network,
// not named parameters, and replaying them into the emulation makes no sense
// (see docs/re-notes, section 4). The structure of that network cannot be
// read from the drivers - they only feed it data.
//
// The implementation below is therefore a standard chorus (a modulated delay
// line) and reverb (comb + allpass filters) whose parameters are fitted to
// recordings of the real card. It is kept apart on purpose, to make clear
// what is derived from the hardware and what is a substitute.
// ---------------------------------------------------------------------------

namespace Emu8000Fx
{
    // Modulated delay line - one chorus voice.
    class ChorusVoice
    {
    public:
        void Init(uint32_t sampleRate, double baseDelayMs, double depthMs,
                  double rateHz, double phase)
        {
            m_sr = sampleRate;
            m_base = baseDelayMs * sampleRate / 1000.0;
            m_depth = depthMs * sampleRate / 1000.0;
            m_rate = rateHz;
            m_phase = phase;
            m_line.assign(static_cast<size_t>(m_base + m_depth) + 4, 0.0f);
            m_pos = 0;
        }

        float Process(float in, float feedback)
        {
            if (m_line.empty()) return 0.0f;

            const double delay = m_base + m_depth * std::sin(m_phase);
            m_phase += 2.0 * 3.14159265358979323846 * m_rate / m_sr;
            if (m_phase > 2.0 * 3.14159265358979323846)
                m_phase -= 2.0 * 3.14159265358979323846;

            // linear interpolation in the delay line
            const double read = static_cast<double>(m_pos) - delay;
            const double wrapped = read < 0 ? read + m_line.size() : read;
            const size_t i0 = static_cast<size_t>(wrapped) % m_line.size();
            const size_t i1 = (i0 + 1) % m_line.size();
            const float f = static_cast<float>(wrapped - std::floor(wrapped));
            const float out = m_line[i0] + (m_line[i1] - m_line[i0]) * f;

            m_line[m_pos] = in + out * feedback;
            m_pos = (m_pos + 1) % m_line.size();
            return out;
        }

    private:
        std::vector<float> m_line;
        size_t m_pos = 0;
        uint32_t m_sr = 44100;
        double m_base = 0.0, m_depth = 0.0, m_rate = 0.0, m_phase = 0.0;
    };

    // Comb filter with high-frequency damping.
    class Comb
    {
    public:
        void Init(size_t samples) { m_line.assign(samples ? samples : 1, 0.0f); m_pos = 0; m_store = 0.0f; }
        float Process(float in, float feedback, float damp)
        {
            const float out = m_line[m_pos];
            m_store = out * (1.0f - damp) + m_store * damp;
            m_line[m_pos] = in + m_store * feedback;
            m_pos = (m_pos + 1) % m_line.size();
            return out;
        }
    private:
        std::vector<float> m_line;
        size_t m_pos = 0;
        float m_store = 0.0f;
    };

    class AllPass
    {
    public:
        void Init(size_t samples) { m_line.assign(samples ? samples : 1, 0.0f); m_pos = 0; }
        float Process(float in, float gain)
        {
            const float buf = m_line[m_pos];
            const float out = -in + buf;
            m_line[m_pos] = in + buf * gain;
            m_pos = (m_pos + 1) % m_line.size();
            return out;
        }
    private:
        std::vector<float> m_line;
        size_t m_pos = 0;
    };

    // ----------------------------------------------------------------------
    // Chorus - parameters taken from the preset table in SBAWE32.DRV
    // (ds:0x19A2, 8 presets of 7 words, the default is number 2). Record
    // format:
    //
    //   word 0  feedback      0xE600..0xE6FF, level in the low byte
    //   word 1  delay_offset  in samples at 44100 Hz
    //   word 2  lfo_depth     0xBC00..0xBCFF, depth in the low byte
    //   word 3-4 delay        (dword, goes to HWCF4)
    //   word 5-6 lfo_freq     (dword, goes to HWCF5)
    //
    // From `lfo_freq` of preset 2 being 0x83 one can verify that AWEUTIL
    // uses exactly this preset - it writes HWCF5 = 0x83. The frequency
    // ratios across the presets (109 : 380 : 131 : 91 : 38) match the
    // documented speeds of Chorus 1 / Chorus 2 / Chorus 3 / Feedback /
    // Flanger, which gives a unit of about 0.0073 Hz.
    // ----------------------------------------------------------------------
    struct ChorusPreset
    {
        uint16_t feedback;      // low byte = feedback level
        uint16_t delaySamples;  // delay in samples at 44100 Hz
        uint16_t depth;         // low byte = modulation depth
        uint32_t lfoFreq;       // unit ~0.0073 Hz
    };

    // Exact values from ds:0x19A2 in SBAWE32.DRV.
    inline constexpr ChorusPreset kChorusPresets[8] = {
        { 0xE600, 0x03F6, 0xBC2C, 0x006D },   // 0 Chorus 1
        { 0xE608, 0x031A, 0xBC6E, 0x017C },   // 1 Chorus 2
        { 0xE610, 0x031A, 0xBC84, 0x0083 },   // 2 Chorus 3  (default)
        { 0xE620, 0x0269, 0xBC6E, 0x017C },   // 3 Chorus 4
        { 0xE680, 0x04D3, 0xBCA6, 0x005B },   // 4 Feedback
        { 0xE6E0, 0x044E, 0xBC37, 0x0026 },   // 5 Flanger
        { 0xE600, 0x0B06, 0xBC00, 0x0083 },   // 6 Short Delay
        { 0xE6C0, 0x0B06, 0xBC00, 0x0083 },   // 7 Short Delay + FB
    };
    inline constexpr int kChorusDefault = 2;
    inline constexpr int kReverbDefault = 4;   // Hall 2, see SBAWE32.DRV 0x612D

    // ----------------------------------------------------------------------
    // Recognising the preset from the register state. The chip has no preset
    // register: the drivers configure the effect DSP by writing fixed INIT
    // words (alsa_emu8000_init.c: reverb_parm with the slot order of
    // reverb_cmds, chorus_parm; the same data as SBAWE32.DRV ds:0x1A12 and
    // ds:0x19A2). AWETST25 blocks 22/23 showed that a register replay must
    // follow these writes - the card's reverb and chorus differ strongly per
    // preset, our render did not change at all.
    // ----------------------------------------------------------------------
    struct InitSlot { uint8_t init; uint8_t slot; };   // init = 1..4

    inline constexpr InitSlot kReverbSlots[28] = {
        {1, 0x03}, {1, 0x05}, {4, 0x1F}, {1, 0x07}, {2, 0x14}, {2, 0x16}, {1, 0x0F},
        {1, 0x17}, {1, 0x1F}, {2, 0x07}, {2, 0x0F}, {2, 0x17}, {2, 0x1D}, {2, 0x1F},
        {3, 0x01}, {3, 0x03}, {1, 0x09}, {1, 0x0B}, {1, 0x11}, {1, 0x13}, {1, 0x19},
        {1, 0x1B}, {2, 0x01}, {2, 0x03}, {2, 0x09}, {2, 0x0B}, {2, 0x11}, {2, 0x13},
    };

    inline constexpr uint16_t kReverbParm[8][28] = {
        { 0xB488, 0xA450, 0x9550, 0x84B5, 0x383A, 0x3EB5, 0x72F4, 0x72A4, 0x7254, 0x7204,
          0x7204, 0x7204, 0x4416, 0x4516, 0xA490, 0xA590, 0x842A, 0x852A, 0x842A, 0x852A,
          0x8429, 0x8529, 0x8429, 0x8529, 0x8428, 0x8528, 0x8428, 0x8528 },   // 0 room 1
        { 0xB488, 0xA458, 0x9558, 0x84B5, 0x383A, 0x3EB5, 0x7284, 0x7254, 0x7224, 0x7224,
          0x7254, 0x7284, 0x4448, 0x4548, 0xA440, 0xA540, 0x842A, 0x852A, 0x842A, 0x852A,
          0x8429, 0x8529, 0x8429, 0x8529, 0x8428, 0x8528, 0x8428, 0x8528 },   // 1 room 2
        { 0xB488, 0xA460, 0x9560, 0x84B5, 0x383A, 0x3EB5, 0x7284, 0x7254, 0x7224, 0x7224,
          0x7254, 0x7284, 0x4416, 0x4516, 0xA490, 0xA590, 0x842C, 0x852C, 0x842C, 0x852C,
          0x842B, 0x852B, 0x842B, 0x852B, 0x842A, 0x852A, 0x842A, 0x852A },   // 2 room 3
        { 0xB488, 0xA470, 0x9570, 0x84B5, 0x383A, 0x3EB5, 0x7284, 0x7254, 0x7224, 0x7224,
          0x7254, 0x7284, 0x4448, 0x4548, 0xA440, 0xA540, 0x842B, 0x852B, 0x842B, 0x852B,
          0x842A, 0x852A, 0x842A, 0x852A, 0x8429, 0x8529, 0x8429, 0x8529 },   // 3 hall 1
        { 0xB488, 0xA470, 0x9570, 0x84B5, 0x383A, 0x3EB5, 0x7254, 0x7234, 0x7224, 0x7254,
          0x7264, 0x7294, 0x44C3, 0x45C3, 0xA404, 0xA504, 0x842A, 0x852A, 0x842A, 0x852A,
          0x8429, 0x8529, 0x8429, 0x8529, 0x8428, 0x8528, 0x8428, 0x8528 },   // 4 hall 2
        { 0xB4FF, 0xA470, 0x9570, 0x84B5, 0x383A, 0x3EB5, 0x7234, 0x7234, 0x7234, 0x7234,
          0x7234, 0x7234, 0x4448, 0x4548, 0xA440, 0xA540, 0x842A, 0x852A, 0x842A, 0x852A,
          0x8429, 0x8529, 0x8429, 0x8529, 0x8428, 0x8528, 0x8428, 0x8528 },   // 5 plate
        { 0xB4FF, 0xA470, 0x9500, 0x84B5, 0x333A, 0x39B5, 0x7204, 0x7204, 0x7204, 0x7204,
          0x7204, 0x72F4, 0x4400, 0x4500, 0xA4FF, 0xA5FF, 0x8420, 0x8520, 0x8420, 0x8520,
          0x8420, 0x8520, 0x8420, 0x8520, 0x8420, 0x8520, 0x8420, 0x8520 },   // 6 delay
        { 0xB4FF, 0xA490, 0x9590, 0x8474, 0x333A, 0x39B5, 0x7204, 0x7204, 0x7204, 0x7204,
          0x7204, 0x72F4, 0x4400, 0x4500, 0xA4FF, 0xA5FF, 0x8420, 0x8520, 0x8420, 0x8520,
          0x8420, 0x8520, 0x8420, 0x8520, 0x8420, 0x8520, 0x8420, 0x8520 },   // 7 panning delay
    };

    // Best matching reverb preset for the 28 words in kReverbSlots order, or
    // -1 when fewer than 24 words match (user-defined data or an update in
    // progress).
    inline int ReverbPresetFromInit(const uint16_t v[28])
    {
        int best = -1, bestHits = 0;
        for (int p = 0; p < 8; ++p)
        {
            int hits = 0;
            for (int i = 0; i < 28; ++i) hits += (v[i] == kReverbParm[p][i]);
            if (hits > bestHits) { bestHits = hits; best = p; }
        }
        return (bestHits >= 24) ? best : -1;
    }

    // Chorus preset from INIT3 0x09 (feedback), INIT3 0x0C (delay offset) and
    // INIT4 0x03 (LFO depth); these three already tell all 8 presets apart.
    inline int ChorusPresetFromInit(uint16_t feedback, uint16_t delayOffset, uint16_t depth)
    {
        for (int p = 0; p < 8; ++p)
            if (kChorusPresets[p].feedback == feedback && kChorusPresets[p].delaySamples == delayOffset
                && kChorusPresets[p].depth == depth)
                return p;
        return -1;
    }

    class Chorus
    {
    public:
        void Init(uint32_t sampleRate, int preset = kChorusDefault)
        {
            m_sampleRate = sampleRate;
            SetPreset(preset);
        }

        void SetPreset(int preset)
        {
            const ChorusPreset& p = kChorusPresets[std::clamp(preset, 0, 7)];

            const double delayMs = p.delaySamples * 1000.0 / 44100.0;
            const double depthMs = (p.depth & 0xFF) / 255.0 * 6.0;   // [?] range
            const double rateHz = p.lfoFreq * 0.0073;
            m_feedback = (p.feedback & 0xFF) / 255.0f;

            // The second voice shifted by half a period, for a wide result.
            m_voices[0].Init(m_sampleRate, delayMs, depthMs, rateHz, 0.0);
            m_voices[1].Init(m_sampleRate, delayMs * 1.4, depthMs, rateHz * 0.8, 3.14159);
        }

        void Process(float in, float& outL, float& outR)
        {
            const float a = m_voices[0].Process(in, m_feedback);
            const float b = m_voices[1].Process(in, m_feedback);
            outL = a * 0.7f + b * 0.3f;
            outR = a * 0.3f + b * 0.7f;
        }

    private:
        ChorusVoice m_voices[2];
        uint32_t m_sampleRate = 44100;
        float m_feedback = 0.06f;
    };

    // ----------------------------------------------------------------------
    // Reverb: eight comb filters and four allpasses per channel.
    // The lengths are primes near the classic values, converted to the
    // sample rate of the chip.
    // ----------------------------------------------------------------------
    class Reverb
    {
    public:
        void Init(uint32_t sampleRate)
        {
            static const int kComb[8]   = { 1116, 1188, 1277, 1356, 1422, 1491, 1557, 1617 };
            static const int kAllPass[4] = { 556, 441, 341, 225 };
            const double scale = sampleRate / 44100.0;
            for (int ch = 0; ch < 2; ++ch)
            {
            const int spread = ch ? 23 : 0;   // spread of the right channel
                for (int i = 0; i < 8; ++i)
                    m_comb[ch][i].Init(static_cast<size_t>((kComb[i] + spread) * scale));
                for (int i = 0; i < 4; ++i)
                    m_ap[ch][i].Init(static_cast<size_t>((kAllPass[i] + spread) * scale));
            }
            // Pre-delay line for the delay presets (up to 0.3 s).
            m_sampleRate = sampleRate;
            m_pre.assign(static_cast<size_t>(0.3 * sampleRate) + 1, 0.0f);
            m_prePos = 0;
        }

        void Process(float in, float& outL, float& outR)
        {
            if (m_preLen > 0 && m_preLen <= m_pre.size())
            {
                const float delayed = m_pre[m_prePos];
                m_pre[m_prePos] = in;
                if (++m_prePos >= m_preLen) m_prePos = 0;
                in = delayed;
            }

            // NOTE the gain: a comb filter with feedback f has a DC gain
            // of 1/(1-f). At f = 0.854 that is 6.85x, so without input
            // scaling the reverb amplifies several times and the output
            // clips. Scaling by (1-f) brings the average gain of the comb
            // bank to one, and the amount of effect is then decided only by
            // the send and the return level.
            const float scaled = in * m_inputGain;

            float acc[2] = { 0.0f, 0.0f };
            for (int ch = 0; ch < 2; ++ch)
            {
                for (int i = 0; i < 8; ++i)
                    acc[ch] += m_comb[ch][i].Process(scaled, m_feedback, m_damp);
                acc[ch] *= 0.125f;
                for (int i = 0; i < 4; ++i)
                    acc[ch] = m_ap[ch][i].Process(acc[ch], 0.5f);
            }
            outL = acc[0] * m_outGain;
            outR = acc[1] * m_outGain;
        }

        // roomSize 0..1 (reverb length), damp 0..1 (high damping)
        void SetRoom(float roomSize, float damp)
        {
            m_feedback = 0.7f + std::clamp(roomSize, 0.0f, 1.0f) * 0.28f;
            m_damp = std::clamp(damp, 0.0f, 1.0f);
            m_inputGain = 1.0f - m_feedback;
        }

        // Preset 0..7 per the table in SBAWE32.DRV (ds:0x1A12, 8 records of
        // 28 words, the default is number 4). Those 28 words are
        // coefficients of the chip's internal DSP network - they cannot be
        // translated into a topology, so the index gives only the room size
        // and damping. The order matches the standard AWE32 set.
        void SetPreset(int preset)
        {
            // Values fitted to the tester's card, AWETST25 block 22 (internal
            // capture, scratchpad fit_rev.py): level in the first 0.1 s and
            // the tails 0.15-0.6 s and 0.6-1.5 s relative to the dry note, for
            // sends 96 and 255 - 0.4..1.2 dB rms per preset, 0.05 dB for the
            // delay presets. The card has 1-1.5 dB more energy in the first
            // 0.1 s than this comb network gives (presets 1-5, send 96).
            // gain multiplies the output; preDelayMs is the echo delay that
            // the delay presets need (the card adds nothing in the first
            // 0.1 s there).
            struct Room { float size, damp, preDelayMs, gain; };
            static const Room kRooms[8] = {
                { 0.2f, 0.6f,   0.0f, 1.18f },   // 0 Room 1
                { 0.6f, 0.6f,   0.0f, 1.21f },   // 1 Room 2
                { 0.7f, 0.6f,   0.0f, 1.46f },   // 2 Room 3
                { 0.7f, 0.0f,   0.0f, 1.49f },   // 3 Hall 1
                { 0.7f, 0.2f,   0.0f, 1.49f },   // 4 Hall 2   (default)
                { 0.7f, 0.0f,   0.0f, 1.68f },   // 5 Plate
                { 0.0f, 0.0f, 200.0f, 0.38f },   // 6 Delay
                { 0.7f, 0.0f, 150.0f, 0.54f },   // 7 Panning Delay
            };
            const Room& r = kRooms[std::clamp(preset, 0, 7)];
            SetRoom(r.size, r.damp);
            m_outGain = r.gain;
            const size_t len = static_cast<size_t>(r.preDelayMs * m_sampleRate / 1000.0f);
            if (len != m_preLen)
            {
                m_preLen = len;
                m_prePos = 0;
                std::fill(m_pre.begin(), m_pre.end(), 0.0f);
            }
        }

    private:
        Comb m_comb[2][8];
        AllPass m_ap[2][4];
        float m_feedback = 0.84f;
        float m_damp = 0.35f;
        float m_inputGain = 0.16f;   // = 1 - m_feedback, see Process()
        float m_outGain = 1.49f;     // preset 4 until SetPreset is called
        std::vector<float> m_pre;    // pre-delay ring buffer
        size_t m_prePos = 0;
        size_t m_preLen = 0;
        uint32_t m_sampleRate = 44100;
    };

    // -----------------------------------------------------------------------
    // Equalizer (bass / treble) at the chip output.
    //
    // Unlike the reverb and chorus above, this is MEASURED on a real card:
    // AWETST25 at the tester, block 35 - noise from ROM at 1:1, all 12 treble
    // positions at bass 5 and 12 bass positions at treble 5, each against the
    // flat (5,5). Each position is fitted with an RBJ shelf filter of slope
    // S = 0.5; the residual against the measurement is within 0.07 dB rms
    // (treble 10 and 11 within 0.17 and 0.32 dB). The SDK and the game driver
    // set bass 5 and treble 9, which is a shelf of +7.9 dB at f0 2457 Hz
    // (+1.2 dB at 1 kHz, +5.7 at 4 kHz, +7.3 at 8 kHz). Not even 86Box has
    // it.
    // -----------------------------------------------------------------------
    struct EqShelf { double gainDb, f0, S; };

    inline constexpr EqShelf kEqTreble[12] = {
        { -11.9, 1811, 0.5 }, { -8.5, 1542, 0.5 }, { -5.9, 1313, 0.5 }, { -4.1, 1211, 0.5 },
        {  -1.2, 1031, 0.5 }, {  0.0, 1000, 0.5 }, {  1.9, 1261, 0.5 }, {  3.5, 1671, 0.5 },
        {  5.97, 2231, 0.5 }, { 7.90, 2457, 0.5 }, { 9.69, 2188, 0.5 }, { 11.16, 2231, 0.5 },
    };
    inline constexpr EqShelf kEqBass[12] = {
        { -11.9, 224, 0.5 }, { -8.4, 273, 0.5 }, { -5.9, 321, 0.5 }, { -4.0, 362, 0.5 },
        {  -1.1, 426, 0.5 }, {  0.0, 100, 0.5 }, {  2.0, 500, 0.5 }, {  3.5, 564, 0.5 },
        {   6.0, 636, 0.5 }, {  8.0, 718, 0.5 }, {  9.6, 778, 0.5 }, { 12.0, 914, 0.5 },
    };

    // Values the driver writes to the EQ slots (alsa_emu8000_init.c,
    // bass_parm and treble_parm; slot order see Emu8000Core::UpdateEqualizer).
    inline constexpr uint16_t kEqBassParm[12][2] = {
        {0xD26A, 0xD36A}, {0xD25B, 0xD35B}, {0xD24C, 0xD34C}, {0xD23D, 0xD33D},
        {0xD21F, 0xD31F}, {0xC208, 0xC308}, {0xC219, 0xC319}, {0xC22A, 0xC32A},
        {0xC24C, 0xC34C}, {0xC26E, 0xC36E}, {0xC248, 0xC384}, {0xC26A, 0xC36A},
    };
    inline constexpr uint16_t kEqTrebleParm[12][8] = {
        {0x821E, 0xC26A, 0x031E, 0xC36A, 0x021E, 0xD208, 0x831E, 0xD308},
        {0x821E, 0xC25B, 0x031E, 0xC35B, 0x021E, 0xD208, 0x831E, 0xD308},
        {0x821E, 0xC24C, 0x031E, 0xC34C, 0x021E, 0xD208, 0x831E, 0xD308},
        {0x821E, 0xC23D, 0x031E, 0xC33D, 0x021E, 0xD208, 0x831E, 0xD308},
        {0x821E, 0xC21F, 0x031E, 0xC31F, 0x021E, 0xD208, 0x831E, 0xD308},
        {0x821E, 0xD208, 0x031E, 0xD308, 0x021E, 0xD208, 0x831E, 0xD308},
        {0x821E, 0xD208, 0x031E, 0xD308, 0x021D, 0xD219, 0x831D, 0xD319},
        {0x821E, 0xD208, 0x031E, 0xD308, 0x021C, 0xD22A, 0x831C, 0xD32A},
        {0x821E, 0xD208, 0x031E, 0xD308, 0x021A, 0xD24C, 0x831A, 0xD34C},
        {0x821E, 0xD208, 0x031E, 0xD308, 0x0219, 0xD26E, 0x8319, 0xD36E},
        {0x821D, 0xD219, 0x031D, 0xD319, 0x0219, 0xD26E, 0x8319, 0xD36E},
        {0x821C, 0xD22A, 0x031C, 0xD32A, 0x0219, 0xD26E, 0x8319, 0xD36E},
    };

    // The game driver (SBAWE32 DOS, trace dos97) writes the same values with
    // the nibbles of the low byte swapped (C208 -> C280, D26E -> D2E6). On the
    // card they sound the same within 0.1 dB (block 35), so they are taken as
    // equal.
    inline bool EqWordMatch(uint16_t reg, uint16_t table)
    {
        const uint16_t swapped = static_cast<uint16_t>(
            (reg & 0xFF00) | ((reg & 0x0F) << 4) | ((reg & 0xF0) >> 4));
        return reg == table || swapped == table;
    }

    // Determines the bass and treble position from the values of the ten
    // slots. What matches no table row (e.g. before initialisation) keeps its
    // previous value.
    inline void EqIndexFromInit(const uint16_t* v, int& bass, int& treble)
    {
        for (int i = 0; i < 12; ++i)
            if (EqWordMatch(v[0], kEqBassParm[i][0]) && EqWordMatch(v[1], kEqBassParm[i][1]))
            { bass = i; break; }
        for (int i = 0; i < 12; ++i)
        {
            bool ok = true;
            for (int k = 0; k < 8 && ok; ++k)
                ok = EqWordMatch(v[2 + k], kEqTrebleParm[i][k]);
            if (ok) { treble = i; break; }
        }
    }

    class Equalizer
    {
    public:
        void Init(double sampleRate)
        {
            m_fs = sampleRate;
            m_bass = m_treble = -1;
            Set(5, 9);
        }

        void Set(int bass, int treble)
        {
            bass = std::clamp(bass, 0, 11);
            treble = std::clamp(treble, 0, 11);
            if (bass == m_bass && treble == m_treble) return;
            m_bass = bass;
            m_treble = treble;
            for (int c = 0; c < 2; ++c)
            {
                Design(m_low[c],  kEqBass[bass],     false);
                Design(m_high[c], kEqTreble[treble], true);
            }
        }

        int Bass() const   { return m_bass; }
        int Treble() const { return m_treble; }

        void Process(float& l, float& r)
        {
            l = static_cast<float>(m_high[0].Run(m_low[0].Run(l)));
            r = static_cast<float>(m_high[1].Run(m_low[1].Run(r)));
        }

    private:
        struct Biquad
        {
            double b0 = 1, b1 = 0, b2 = 0, a1 = 0, a2 = 0;
            double z1 = 0, z2 = 0;
            double Run(double x)
            {
                const double y = b0 * x + z1;
                z1 = b1 * x - a1 * y + z2;
                z2 = b2 * x - a2 * y;
                return y;
            }
        };

        // RBJ Audio EQ Cookbook, shelf with slope S. The filter state is not
        // reset when the position changes, so that the change does not click.
        void Design(Biquad& q, const EqShelf& s, bool high)
        {
            const double A = std::pow(10.0, s.gainDb / 40.0);
            const double w0 = 2.0 * 3.14159265358979323846 * s.f0 / m_fs;
            const double cw = std::cos(w0);
            const double alpha = std::sin(w0) / 2.0
                * std::sqrt((A + 1.0 / A) * (1.0 / s.S - 1.0) + 2.0);
            const double sq = 2.0 * std::sqrt(A) * alpha;
            double b0, b1, b2, a0, a1, a2;
            if (high)
            {
                b0 = A * ((A + 1) + (A - 1) * cw + sq);
                b1 = -2 * A * ((A - 1) + (A + 1) * cw);
                b2 = A * ((A + 1) + (A - 1) * cw - sq);
                a0 = (A + 1) - (A - 1) * cw + sq;
                a1 = 2 * ((A - 1) - (A + 1) * cw);
                a2 = (A + 1) - (A - 1) * cw - sq;
            }
            else
            {
                b0 = A * ((A + 1) - (A - 1) * cw + sq);
                b1 = 2 * A * ((A - 1) - (A + 1) * cw);
                b2 = A * ((A + 1) - (A - 1) * cw - sq);
                a0 = (A + 1) + (A - 1) * cw + sq;
                a1 = -2 * ((A - 1) + (A + 1) * cw);
                a2 = (A + 1) + (A - 1) * cw - sq;
            }
            q.b0 = b0 / a0; q.b1 = b1 / a0; q.b2 = b2 / a0;
            q.a1 = a1 / a0; q.a2 = a2 / a0;
        }

        double m_fs = 44100.0;
        int m_bass = -1, m_treble = -1;
        Biquad m_low[2], m_high[2];
    };
}
