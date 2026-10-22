#include "Synth.h"
#include "I18n.h"
#include "Awe32Rom.h"
#include <cstdio>
#include "Awe32Curves.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>

using Emu8000::Reg;

namespace
{
    constexpr double kPi = 3.14159265358979323846;

    // Length of the substitute sample table (used when no bank is loaded).
    constexpr uint32_t kDefaultWaveLen = 64;

    // Conversion of the logarithmic pitch (register IP) to the linear
    // increment the driver writes to the upper half of PTRX and CPF.
    //
    // Transcribed from SBAWE.VXD, object 1, 0x212E. Computes 2^(ip/4096) in
    // fixed point: the integer part gives a shift, the three top bits of the
    // fraction add 2^(1/2), 2^(1/4) and 2^(1/8) through fractions with the
    // denominator 10000. At the end a multiplication by 1.25 through
    // `esi += esi >> 2`.
    //
    //   0x102E/0x2710 = 0.41420   (2^0.5  - 1 = 0.41421)
    //   0x764 /0x2710 = 0.18920   (2^0.25 - 1 = 0.18921)
    //   0x389 /0x2710 = 0.09050   (2^0.125- 1 = 0.09051)
    uint32_t PitchIncrement(uint16_t ip)
    {
        if (ip == 0xFFFF) return 0xFFFF;

        uint32_t v = 1u << (ip >> 12);
        if (ip & 0x800) v += v * 0x102Eu / 0x2710u;
        if (ip & 0x400) v += v * 0x0764u / 0x2710u;
        if (ip & 0x200) v += v * 0x0389u / 0x2710u;
        v += v >> 2;
        return (v > 0xFFFFu) ? 0xFFFFu : v;
    }

    // AWE32 DOS SDK, midieng noteOn 0x115D..0x11EB. The same steps as
    // PitchIncrement, but in 16-bit code: `imul word` multiplies only the
    // low word of the running value (signed) and `idiv word` gives a 16-bit
    // quotient; the last step is *1.5 (`sar dx,1 / rcr ax,1 / add`) where
    // the VXD has *1.25.
    uint32_t PitchIncrementSdk(uint16_t ip)
    {
        if (ip == 0xFFFF) return 0xFFFF;

        int32_t v = 1 << (ip >> 12);
        auto step = [&v](int32_t k)
        {
            const int32_t prod = static_cast<int32_t>(static_cast<int16_t>(v & 0xFFFF)) * k;
            v += static_cast<int16_t>(prod / 10000);
        };
        if (ip & 0x800) step(0x102E);
        if (ip & 0x400) step(0x0764);
        if (ip & 0x200) step(0x0389);
        v += v >> 1;
        return ((static_cast<uint32_t>(v) >> 16) != 0) ? 0xFFFFu
                                                       : (static_cast<uint32_t>(v) & 0xFFFFu);
    }

    uint16_t MakeDcysusv(int sustain, int rate)
    {
        return static_cast<uint16_t>(((sustain & 0x7F) << 8) | (rate & 0x7F));
    }
    uint16_t MakeAtkhld(int hold, int attack)
    {
        return static_cast<uint16_t>(((hold & 0x7F) << 8) | (attack & 0x7F));
    }
    int AttenDbToUnits(double db)
    {
        return std::clamp(static_cast<int>(std::lround(db / Emu8000::kAttenDbPerStep)), 0, 255);
    }
}

Synth::Synth(uint32_t sampleRate)
    : m_core(sampleRate)
{
    BuildDefaultWaveform();
}

// ---------------------------------------------------------------------------
// Substitute sample in DRAM - one period of a sine in a loop. It goes through
// the whole real path of the chip, only a sine plays instead of real samples.
// ---------------------------------------------------------------------------
void Synth::BuildDefaultWaveform()
{
    if (m_core.DramSize() < kDefaultWaveLen + 8)
        m_core.ResizeDram(kDefaultWaveLen + 8);   // a few extra samples for the interpolation

    int16_t* dram = m_core.DramData();
    for (uint32_t i = 0; i < kDefaultWaveLen + 8; ++i)
    {
        const double phase = 2.0 * kPi * (i % kDefaultWaveLen) / kDefaultWaveLen;
        dram[i] = static_cast<int16_t>(std::sin(phase) * 30000.0);
    }

    m_fallbackStart     = Emu8000::kDramOffset;
    m_fallbackLoopStart = Emu8000::kDramOffset;
    m_fallbackLoopEnd   = Emu8000::kDramOffset + kDefaultWaveLen;
    m_fallbackUnityHz   = static_cast<double>(Emu8000Core::kNativeSampleRate) / kDefaultWaveLen;
}

// ---------------------------------------------------------------------------
// Loading sound data
// ---------------------------------------------------------------------------

bool Synth::LoadWaveRom(const std::string& path, std::string& error)
{
    // A plain image or the one packed by gzip (data.binz).
    std::vector<uint8_t> raw;
    if (!Awe32Rom::Read(path, raw, error)) return false;
    if (raw.size() < 2) { error = StrFormat(_("ROM is empty: %s"), path.c_str()); return false; }

    // A raw dump = 16-bit little-endian samples. The ROM's text header is
    // stored by words (that is why it looks swapped), but the sample data
    // are read directly - see docs/re-notes/rom_vs_sf2.md.
    std::vector<int16_t> rom(raw.size() / 2);
    std::memcpy(rom.data(), raw.data(), rom.size() * 2);

    // The widespread awe32.raw carries one extra word (0x1234) in front of
    // the ROM. A dump taken from a real AWE32 card with AWEDUMP (2026-09-14,
    // AWE32EmuData/rom/awe32rom.bin) starts with 0x0032 and equals awe32.raw
    // shifted by exactly one word (0 differing words out of 524 287).
    // Detect the extra word the same way 86Box does (emu8k_init: "1M" "GM"
    // at words 3 and 4 instead of 2 and 3) and drop it, so address A reads
    // the same word as on the card.
    if (rom.size() > 4 && static_cast<uint16_t>(rom[3]) == 0x314D
                       && static_cast<uint16_t>(rom[4]) == 0x474D)
    {
        rom.erase(rom.begin());
        rom.push_back(0);
    }

    m_core.LoadWaveRom(std::move(rom));
    return true;
}

bool Synth::LoadBank(const std::string& path, std::string& error, bool samplesInRom,
                     int midiBank)
{
    return AddBank(SoundFont::Load(path), error, samplesInRom, midiBank);
}

bool Synth::LoadBankData(std::vector<uint8_t> data, const std::string& name, std::string& error, int midiBank)
{
    return AddBank(SoundFont::LoadData(std::move(data), name), error, false, midiBank);
}

bool Synth::AddBank(SoundFont::Bank bank, std::string& error, bool samplesInRom, int midiBank)
{
    if (!bank.valid) { error = bank.errorMessage; return false; }
    bank.samplesInRom = samplesInRom;

    if (midiBank >= 0)
        for (SoundFont::Preset& p : bank.presets)
            if (p.bank == 0) p.bank = midiBank;

    if (m_nextDramBase == 0)
    {
        const uint32_t reserve =
            (Awe32::IsDosLike(m_core.DriverVariant())) ? kDramReserveDos
                                                           : kDramReserveWin95;
        m_nextDramBase = Emu8000::kDramOffset + reserve;
    }

    LoadedBank lb;
    lb.dramBase = m_nextDramBase;

    if (!samplesInRom && !bank.sampleData.empty())
    {
        // The bank's samples go into DRAM after the banks already loaded -
        // exactly as the driver does when it loads a user bank into the
        // card's RAM.
        const size_t offset = lb.dramBase - Emu8000::kDramOffset;
        const size_t needed = offset + bank.sampleData.size() + 8;
        if (m_core.DramSize() < needed)
        {
            std::vector<int16_t> keep(m_core.DramData(), m_core.DramData() + m_core.DramSize());
            m_core.ResizeDram(needed);
            std::memcpy(m_core.DramData(), keep.data(), keep.size() * sizeof(int16_t));
        }
        std::memcpy(m_core.DramData() + offset, bank.sampleData.data(),
                    bank.sampleData.size() * sizeof(int16_t));

        // The words the driver reserves in front of the first bank are zero
        // on the card (uploads in dos_mdi.trace / bank_synth02s.trace). Our
        // fallback sine used to sit there, and SF1 addresses may point into
        // the reserve: in the MC2 intro every ch2 note starts at 0x200004 and
        // played 46 words of that full-scale sine before its sample. The
        // fallback waveform is only needed when no bank is loaded at all.
        const size_t reserveWords =
            (Awe32::IsDosLike(m_core.DriverVariant())) ? kDramReserveDos : kDramReserveWin95;
        if (offset == reserveWords)
            std::fill(m_core.DramData(), m_core.DramData() + offset, int16_t{0});

        // The Creative drivers do not upload SoundFont 1.0 samples as stored.
        // Every own sample is filtered on the way to the card:
        //   y[n] = clip16(floor((6*x[n] - x[n-1] - x[n+1]) / d))
        // d = 4 for the DOS driver (SBAWE32.MDI), d = 8 for the Win95 driver;
        // the zero padding between samples stays zero. It is a pre-emphasis
        // (unity at DC, +6 dB at Nyquist) - the ROM samples already carry it.
        // Measured on the uploads of the real drivers in 86Box traces:
        //   dos_mdi.trace (Magic Carpet 2, BULLFROG.SBK): bit exact over
        //     [start, end) of every own sample
        //   bank_synth02s.trace (Win95, SYNTH02S.SBK): same kernel with d = 8,
        //     applied over [start, end - 3) - word end - 3 stays as stored
        // Without it every DRAM sound was duller than on the card; the MC2
        // intro sounded wrong everywhere except the belltree from ROM.
        if (bank.version == SoundFont::Version::Sf1)
        {
            const bool dos = (Awe32::IsDosLike(m_core.DriverVariant()));
            const int shift = dos ? 2 : 3;
            const int32_t round = (1 << shift) - 1;
            const int16_t* src = bank.sampleData.data();
            const size_t count = bank.sampleData.size();
            int16_t* dst = m_core.DramData() + offset;
            for (const SoundFont::Sample& s : bank.samples)
            {
                if (s.inRom || s.end <= s.start) continue;
                const size_t endExcl = dos ? s.end : ((s.end >= s.start + 3) ? s.end - 3 : s.start);
                const size_t first = std::min<size_t>(s.start, count);
                const size_t last  = std::min<size_t>(endExcl, count);
                for (size_t i = first; i < last; ++i)
                {
                    const int32_t xm = (i > 0) ? src[i - 1] : 0;
                    const int32_t xp = (i + 1 < count) ? src[i + 1] : 0;
                    const int32_t num = 6 * static_cast<int32_t>(src[i]) - xm - xp;
                    const int32_t y = (num >= 0) ? (num >> shift) : -((-num + round) >> shift);
                    dst[i] = static_cast<int16_t>(std::clamp(y, -32768, 32767));
                }
            }
        }
        m_nextDramBase += static_cast<uint32_t>(bank.sampleData.size() + 8);
    }

    lb.bank = std::make_unique<SoundFont::Bank>(std::move(bank));
    m_banks.push_back(std::move(lb));
    return true;
}

// ---------------------------------------------------------------------------
// Voice management
// ---------------------------------------------------------------------------

int Synth::AllocateVoice()
{
    for (int i = 0; i < kUsableVoices; ++i)
        if (!m_alloc[i].inUse && !m_alloc[i].heldBySustain && !m_core.IsVoiceActive(i))
            return i;
    for (int i = 0; i < kUsableVoices; ++i)
        if (!m_alloc[i].inUse && !m_alloc[i].heldBySustain)
            return i;

    // All busy - take the oldest, preferably one held only by the pedal.
    int best = 0;
    uint32_t bestAge = 0xFFFFFFFFu;
    bool foundSustained = false;
    for (int i = 0; i < kUsableVoices; ++i)
    {
        const bool sustained = m_alloc[i].heldBySustain;
        if (foundSustained && !sustained) continue;
        if (!foundSustained && sustained) { foundSustained = true; bestAge = 0xFFFFFFFFu; }
        if (m_alloc[i].age < bestAge) { bestAge = m_alloc[i].age; best = i; }
    }
    KillVoice(best);
    return best;
}

void Synth::ReleaseVoice(int voice)
{
    if (Awe32::IsDosLike(m_core.DriverVariant()))
    {
        NoteOffMdi(voice);
        return;
    }
    m_core.Write(Reg::DCYSUSV, voice,
                 Emu8000::kDcysusvRelease | (m_alloc[voice].releaseRate & 0x7F));
    // `SBAWE.VXD` releases **both** envelopes - right after DCYSUSV it sends
    // DCYSUS with its own rate (ReleaseModEnv). Measured in a win95 trace,
    // where the pair DCYSUSV 8029 / DCYSUS 8027 stands at every note-off;
    // hence also the doubled count of DCYSUS writes in the trace_diff census.
    // `SBAWE32.MDI` does not do it.
    if (m_core.DriverVariant() == Awe32::Driver::Win95)
    {
        m_core.Write(Reg::DCYSUS, voice,
                     Emu8000::kDcysusvRelease | (m_alloc[voice].releaseModRate & 0x7F));
        // SBAWE.VXD 0xC0FFA252: loop behind the sample end, CSL = end + 4,
        // PSST = end; the voice counts as finished once CCCA passes the end.
        const uint32_t end = m_alloc[voice].vxdNoteEnd;
        if (end != 0)
        {
            const uint32_t csl = m_core.ReadDriver(Reg::CSL, voice);
            m_core.Write(Reg::CSL, voice, (csl & 0xFF000000u) | (end + 4));
            const uint32_t psst = m_core.ReadDriver(Reg::PSST, voice);
            m_core.Write(Reg::PSST, voice, (psst & 0xFF000000u) | end);
            m_alloc[voice].vxdEndAddr = end;
        }
    }
    m_alloc[voice].inUse = false;
    m_alloc[voice].heldBySustain = false;
    m_alloc[voice].vxdState = 0xFFE;
    m_alloc[voice].vxdRom = false;
}

void Synth::KillVoice(int voice)
{
    m_core.Write(Reg::DCYSUSV, voice, Emu8000::kDcysusvOff);
    m_alloc[voice].inUse = false;
    m_alloc[voice].heldBySustain = false;
    m_alloc[voice].vxdState = 0xFFE;
    m_alloc[voice].vxdRom = false;
}

// ---------------------------------------------------------------------------
// SBAWE32.MDI (family `dos`) voice handling, transcribed from the driver:
// allocation 0x1872, note-off 0x19BE / 0x238A, sustain 0x2576, all notes off
// 0x277C, controller reset 0x2732, volume/expression update 0x2482 / 0x2554,
// modulation update 0x25D6, channel pressure 0x2AAC, pitch bend 0x2AD8.
// The allocation reads the chip (VTFT, DCYSUSV, CCCA), so which voice it
// takes depends on the chip state, as on the card.
// ---------------------------------------------------------------------------

namespace
{
    // IP + bend offset the way SBAWE32.MDI adds them (0x20E0, 0x2B51): above
    // 0xFFFF the result is capped, a negative result wraps to 16 bits.
    uint16_t MdiAddBend(int ip, int offset)
    {
        const int32_t sum = static_cast<int32_t>(static_cast<uint16_t>(ip)) + offset;
        if (sum < 0) return static_cast<uint16_t>(sum);
        return static_cast<uint16_t>(std::min<int32_t>(sum, 0xFFFF));
    }

    // IP + bend offset in the SDK. The bend handler (AWE32PITCHBEND 0x1C4E)
    // clamps to 0..0xFFFF; the note-on (noteOn 0x0C61) only tests the high
    // word of the 32-bit sum, so a negative result becomes 0xFFFF as well.
    uint16_t SdkAddBend(int ip, int offset, bool noteOn)
    {
        const int32_t sum = static_cast<int32_t>(static_cast<uint16_t>(ip)) + offset;
        if (sum > 0xFFFF) return 0xFFFF;
        if (sum < 0) return noteOn ? 0xFFFF : 0;
        return static_cast<uint16_t>(sum);
    }
}

int Synth::AllocateVoiceMdi(uint16_t state)
{
    // Even voices first, then odd ones (voices 30/31 belong to the driver).
    // Score = volume target + 0x200 if assigned + 0x300 if held by the pedal
    // + 0x1000 if not in release. The lowest score wins and a later voice
    // wins a tie; score 0 is taken at once, and so is a voice whose playback
    // address has passed the end set by a note-off loop opening.
    uint32_t best = 0xFFFFFFFFu;
    int chosen = kUsableVoices - 1;
    bool done = false;
    for (int pass = 0; pass < 2 && !done; ++pass)
    {
        for (int v = pass; v < kUsableVoices; v += 2)
        {
            const VoiceAlloc& a = m_alloc[v];
            const uint16_t s = a.mdiState;
            if (s == 0xFFFF || s < 0x1000)
            {
                uint32_t score = m_core.ReadDriver(Reg::VTFT, v) >> 16;
                if (s != 0xFFFF)
                {
                    score += 0x200;
                    if ((s & 0xFF) == 0xFF) score += 0x300;
                }
                if (!(m_core.ReadDriver(Reg::DCYSUSV, v) & Emu8000::kDcysusvRelease))
                    score += 0x1000;
                if (score <= best)
                {
                    best = score;
                    chosen = v;
                    if (score == 0) { done = true; break; }
                }
            }
            if (a.mdiEndAddr != 0
                && (m_core.ReadDriver(Reg::CCCA, v) & Emu8000::kCccaAddressMask) >= a.mdiEndAddr)
            {
                chosen = v;
                done = true;
                break;
            }
        }
    }

    VoiceAlloc& a = m_alloc[chosen];
    a.mdiState = state;
    a.mdiEndAddr = 0;
    a.inUse = false;
    a.heldBySustain = false;
    m_core.Write(Reg::DCYSUSV, chosen, 0x807Fu);
    return chosen;
}

// ---------------------------------------------------------------------------
// AWE32 DOS SDK (family `sdk`) voice allocation, __AWE32ALLOCGCHANNEL in
// midieng.c (RAWE32L.LIB, offset 0x0478). A sibling of the MDI one with
// different details: the release test and the pedal bonus apply only to an
// assigned voice, a free voice is taken at once only when silent, the
// EARLIER voice wins a tie, and when nothing qualifies the note gets no
// voice (-1). The taken voice gets DCYSUSV 0x0080 and VTFT 0x0000FFFF.
// DOSMid trace: voices 0, 2, 4 ... first, DCYSUSV 0080 + VTFT FFFF.
// ---------------------------------------------------------------------------

int Synth::AllocateVoiceSdk(uint16_t state)
{
    uint32_t best = 0xFFFFFFFFu;
    int chosen = -1;
    bool done = false;
    for (int pass = 0; pass < 2 && !done; ++pass)
    {
        for (int v = pass; v < kUsableVoices; v += 2)
        {
            const VoiceAlloc& a = m_alloc[v];
            const uint16_t s = a.mdiState;
            if (s == 0xFFFF || s < 0x1000)
            {
                uint32_t score = m_core.ReadDriver(Reg::VTFT, v) >> 16;
                if (s == 0xFFFF)
                {
                    if (score == 0) { chosen = v; done = true; break; }
                }
                else
                {
                    score += 0x200;
                    if ((s & 0xFF) == 0xFF)
                        score += 0x300;
                    else if (!(m_core.ReadDriver(Reg::DCYSUSV, v) & Emu8000::kDcysusvRelease))
                        score += 0x1300;
                }
                if (score < best) { best = score; chosen = v; }
            }
            if (a.mdiEndAddr != 0
                && (m_core.ReadDriver(Reg::CCCA, v) & Emu8000::kCccaAddressMask) >= a.mdiEndAddr)
            {
                chosen = v;
                done = true;
                break;
            }
        }
    }
    if (chosen < 0) return -1;

    VoiceAlloc& a = m_alloc[chosen];
    a.mdiState = state;
    a.mdiEndAddr = 0;
    a.inUse = false;
    a.heldBySustain = false;
    m_core.Write(Reg::DCYSUSV, chosen, 0x0080u);
    m_core.Write(Reg::VTFT, chosen, 0x0000FFFFu);
    return chosen;
}

// ---------------------------------------------------------------------------
// SBAWE.VXD (family `win95`) voice allocation, transcribed from 0xC0FF9C68.
// ---------------------------------------------------------------------------

int Synth::AllocateVoiceVxd(uint16_t state)
{
    // Three passes over the voices in steps of three (0, 3, .. 30, then 1, 4,
    // .. 31, then 2, 5, .. 29) - that is the order the driver takes them in
    // (georg_win95.trace). Score of a voice: its volume target, plus 0x200
    // when it is assigned (0x1200 when bit 12 of the state is set), plus
    // 0x300 when it is held by the pedal, otherwise plus 0x1300 when its
    // volume envelope is not in release. The lowest score wins, a later voice
    // wins a tie, a free voice that is already silent is taken at once, and so
    // is a voice whose playback address has passed the end of its sample.
    // While at most two ROM voices are playing, ROM voices are not taken.
    // The flag means "playing a ROM sample now" - it is cleared when the voice
    // is released; kept after the note it made every voice of a ROM bank
    // untouchable and all notes of Georgia after 18 s fell on voice 0.
    int romPlaying = 0;
    for (int v = 0; v < kVxdVoices; ++v)
        if (m_alloc[v].vxdRom && m_alloc[v].vxdState != 0xFFE) ++romPlaying;
    const bool protectRom = (romPlaying <= 2);

    uint32_t best = 0xFFFFFFFFu;
    int cand = -1;
    int chosen = -1;
    for (int pass = 0; pass < 3 && chosen < 0; ++pass)
    {
        for (int v = pass; v < kVxdVoices; v += 3)
        {
            const VoiceAlloc& a = m_alloc[v];
            if (protectRom && a.vxdRom) continue;
            const uint16_t s = a.vxdState;
            if (s < 0x2000)
            {
                uint32_t score = m_core.ReadDriver(Reg::VTFT, v) >> 16;
                bool take = false;
                if (s == 0xFFE)
                {
                    take = (score == 0);
                }
                else
                {
                    score += (s & 0x1000) ? 0x1200u : 0x200u;
                    if ((s & 0xFF) == 0xFF)
                        score += 0x300u;
                    else if (!(m_core.ReadDriver(Reg::DCYSUSV, v) & Emu8000::kDcysusvRelease))
                        score += 0x1300u;
                }
                if (take) { chosen = v; break; }
                if (score <= best) { best = score; cand = v; }
            }
            if (a.vxdEndAddr != 0
                && (m_core.ReadDriver(Reg::CCCA, v) & Emu8000::kCccaAddressMask) >= a.vxdEndAddr)
            {
                chosen = v;
                break;
            }
        }
    }
    if (chosen < 0) chosen = (cand >= 0) ? cand : 0;

    VoiceAlloc& a = m_alloc[chosen];
    a.vxdState = state;
    a.vxdEndAddr = 0;
    a.vxdRom = false;
    a.inUse = false;
    a.heldBySustain = false;
    m_core.Write(Reg::DCYSUSV, chosen, 0x00FFu);
    return chosen;
}

// SBAWE.VXD CC10 (0xC0FFC3F1): every voice of the channel gets a new target
// pan, the same formula and limits as at note-on, and moves towards it one
// register step at a time (0xC0FFC347): the first step at once, the next ones
// from the driver's deferred-call queue. The queue runs every ~5.15 ms -
// relax_win95.trace: 227 frames between the pan steps of a voice in 9669
// cases (189 in the next most common). Each step writes the pan byte of PSST
// and its complement (0xFF for pan 0) to the low byte of PTRX.
namespace { constexpr int32_t kVxdPanStepFrames = 227; }

void Synth::PanVxd(uint8_t channel, uint8_t value)
{
    for (int v = 0; v < kVxdVoices; ++v)
    {
        VoiceAlloc& a = m_alloc[v];
        if ((a.vxdState >> 8) != channel) continue;
        int pan = 0x17F - 2 * (a.vxdPatchPan + static_cast<int>(value));
        if (pan >= 0xFE) pan = 0xFF;
        else if (pan <= 1) pan = 0;
        a.vxdPanTarget = static_cast<uint8_t>(pan);
        PanStepVxd(v);
    }
}

void Synth::PanStepVxd(int voice)
{
    VoiceAlloc& a = m_alloc[voice];
    a.vxdPanTimer = -1;
    if (a.vxdState >= 0x2000 || a.vxdPanCur == a.vxdPanTarget) return;
    a.vxdPanCur = static_cast<uint8_t>(a.vxdPanCur + (a.vxdPanTarget > a.vxdPanCur ? 1 : -1));
    const uint32_t psst = m_core.ReadDriver(Reg::PSST, voice);
    m_core.Write(Reg::PSST, voice, (static_cast<uint32_t>(a.vxdPanCur) << 24) | (psst & 0x00FFFFFFu));
    const uint32_t aux = a.vxdPanCur ? static_cast<uint32_t>((256 - a.vxdPanCur) & 0xFF) : 0xFFu;
    const uint32_t ptrx = m_core.ReadDriver(Reg::PTRX, voice);
    m_core.Write(Reg::PTRX, voice, (ptrx & 0xFFFFFF00u) | aux);
    if (a.vxdPanCur != a.vxdPanTarget) a.vxdPanTimer = kVxdPanStepFrames;
}

// SBAWE.VXD CC1 (0xC0FFC650): FMMOD pitch depth of every voice of the
// channel = patch depth + CC1 / 30 + pressure / 30, capped at 0x7F from
// above; the low byte (filter depth) is read back and kept.
void Synth::UpdateFmmodVxd(uint8_t channel)
{
    const ChannelState& ch = m_channels[channel];
    for (int v = 0; v < kVxdVoices; ++v)
    {
        const VoiceAlloc& a = m_alloc[v];
        if ((a.vxdState >> 8) != channel) continue;
        const int depth = std::min(a.vxdFmmodDepth + ch.modWheel / 30 + ch.vxdPressureDiv30, 0x7F);
        const uint16_t fmmod = static_cast<uint16_t>(m_core.ReadDriver(Reg::FMMOD, v));
        m_core.Write(Reg::FMMOD, v, static_cast<uint16_t>(((depth & 0xFF) << 8) | (fmmod & 0xFF)));
    }
}

// SBAWE.VXD CC7 / CC11 (0xC0FFC47F): the attenuation of every voice of the
// channel is recomputed with the note-on formula (ComputeAttenuationVxd, +16
// for a "1MGM" ROM sample) and written to the low byte of IFATN.
void Synth::UpdateAttenVxd(uint8_t channel)
{
    const ChannelState& ch = m_channels[channel];
    for (int v = 0; v < kVxdVoices; ++v)
    {
        const VoiceAlloc& a = m_alloc[v];
        if ((a.vxdState >> 8) != channel) continue;
        int atten = Awe32Curves::ComputeAttenuation(
            EffectiveChannelVolume(ch.volume), a.velocity, ch.expression,
            a.vxdPatchAtten, Awe32::Driver::Win95);
        if (atten < 255 && a.vxdRom1mgm) atten = std::min(atten + 16, 255);
        const uint16_t ifatn = static_cast<uint16_t>(m_core.ReadDriver(Reg::IFATN, v));
        m_core.Write(Reg::IFATN, v, static_cast<uint16_t>((ifatn & 0xFF00) | (atten & 0xFF)));
    }
}

void Synth::NoteOffMdi(int voice)
{
    // 0x19BE: release both envelopes, modulation first. For a looped sample
    // with a tail after the loop the loop is moved behind the sample end, so
    // the tail plays out; the voice counts as finished once CCCA passes it.
    VoiceAlloc& a = m_alloc[voice];
    a.mdiState = 0xFF00;
    m_core.Write(Reg::DCYSUS,  voice, 0x8000u | a.mdiModRelease);
    m_core.Write(Reg::DCYSUSV, voice, 0x8000u | a.mdiVolRelease);
    if (a.mdiNoteEnd != 0)
    {
        const uint32_t csl = m_core.ReadDriver(Reg::CSL, voice);
        m_core.Write(Reg::CSL, voice, (csl & 0xFF000000u) | (a.mdiNoteEnd + 4));
        const uint32_t psst = m_core.ReadDriver(Reg::PSST, voice);
        m_core.Write(Reg::PSST, voice, (psst & 0xFF000000u) | a.mdiNoteEnd);
        a.mdiEndAddr = a.mdiNoteEnd;
    }
    else
    {
        a.mdiEndAddr = 0;
    }
    a.mdiState = 0xFFFF;
    a.inUse = false;
    a.heldBySustain = false;
}

void Synth::UpdateAttenMdi(uint8_t channel)
{
    // 0x2482 (CC7) / 0x2554 (CC11). The formula is not the note-on one: the
    // patch attenuation enters as * 25 / 80 before the * 8 / 3. The voice test
    // only compares the low nibble of the state's high byte, so free voices
    // (0xFFFF) are rewritten as well when the channel is 15.
    const ChannelState& ch = m_channels[channel];
    const int vol = std::clamp(EffectiveChannelVolume(ch.volume), 0, 127);
    const bool sdk = m_core.DriverVariant() == Awe32::Driver::Sdk;
    for (int v = 0; v < kUsableVoices; ++v)
    {
        const VoiceAlloc& a = m_alloc[v];
        if (((a.mdiState >> 8) & 0x0F) != channel) continue;
        int atten;
        if (vol <= 10)
        {
            atten = 0xFF;
        }
        else
        {
            const uint16_t db = static_cast<uint16_t>(
                Awe32Curves::VelocityDb(a.mdiVelocity, Awe32::Driver::Dos)
                + Awe32Curves::kChannelVolumeDb[vol]);
            const uint16_t patch = static_cast<uint16_t>(
                static_cast<uint16_t>(a.mdiPatchAtten * 0x19) / 0x50);
            const uint16_t sum = static_cast<uint16_t>(patch + db);
            atten = static_cast<uint16_t>(sum << 3) / 3;
            if (sdk)
            {
                // SDK Volume 0x15B2..0x1608: expression from 256 with >> 7,
                // then +16 for a ROM sample (card ROM id "1MGM").
                if (atten > 0xFF)
                    atten = 0xFF;
                else if (ch.expression < 0x7F)
                    atten += static_cast<uint16_t>(Awe32Curves::kExpressionDb[ch.expression] * (0x100 - atten)) >> 7;
                if (a.sdkRom)
                    atten = std::min(atten + 0x10, 0xFF);
            }
            else if (atten >= 0xFF)
                atten = 0xFF;
            else if (ch.expression < 0x7F)
                atten += Awe32Curves::kExpressionDb[ch.expression] * (0xFF - atten) / 0x7F;
        }
        const uint16_t ifatn = static_cast<uint16_t>(m_core.ReadDriver(Reg::IFATN, v));
        m_core.Write(Reg::IFATN, v, static_cast<uint16_t>((ifatn & 0xFF00) | (atten & 0xFF)));
    }
}

void Synth::UpdateFmmodMdi(uint8_t channel, int value)
{
    // 0x25D6: LFO1 -> pitch depth of the playing voices = pressure / 30 +
    // patch depth + value / 30, capped at 0x7F (no lower limit).
    ChannelState& ch = m_channels[channel];
    ch.mdiModDiv30 = static_cast<uint8_t>(value / 30);
    for (int v = 0; v < kUsableVoices; ++v)
    {
        const VoiceAlloc& a = m_alloc[v];
        const int hi = a.mdiState >> 8;
        if (hi == 0xFF || (hi & 0x0F) != channel) continue;
        int depth = ch.mdiPressureDiv30 + a.mdiFmmodDepth + value / 30;
        if (depth > 0x7F) depth = 0x7F;
        const uint16_t fmmod = static_cast<uint16_t>(m_core.ReadDriver(Reg::FMMOD, v));
        m_core.Write(Reg::FMMOD, v,
                     static_cast<uint16_t>((fmmod & 0x00FF) | ((depth << 8) & 0xFF00)));
    }
}

void Synth::PitchBendMdi(uint8_t channel, int16_t value)
{
    // 0x2AD8: the IP offset is computed once per bend event (16-bit multiply,
    // truncating division) and kept in the channel block; a note-on uses the
    // stored offset. A range of 0 counts as 2.
    ChannelState& ch = m_channels[channel];
    ch.pitchBend = value;
    const int range = ch.pitchBendRangeSemitones ? ch.pitchBendRangeSemitones : 2;
    // The SDK (AWE32PITCHBEND 0x1BF8) divides bend * range by 24 - the
    // exact 4096/12 of the VXD - where the MDI multiplies by 0x155 / 0x2000,
    // one step lower at full bend (681 vs 682).
    const bool sdk = m_core.DriverVariant() == Awe32::Driver::Sdk;
    if (sdk)
    {
        ch.mdiBendOffset = static_cast<int16_t>((static_cast<int32_t>(value) * range) / 24);
    }
    else
    {
        const int32_t product = static_cast<int32_t>(value)
            * static_cast<int16_t>(static_cast<uint16_t>(range * 0x155));
        ch.mdiBendOffset = static_cast<int16_t>(product / 0x2000);
    }
    for (int v = 0; v < kUsableVoices; ++v)
    {
        const VoiceAlloc& a = m_alloc[v];
        const int hi = a.mdiState >> 8;
        if (hi == 0xFF || (hi & 0x0F) != channel) continue;
        m_core.Write(Reg::IP, v, sdk ? SdkAddBend(a.basePitch, ch.mdiBendOffset, false)
                                     : MdiAddBend(a.basePitch, ch.mdiBendOffset));
    }
}

void Synth::SustainMdi(uint8_t channel, uint8_t value)
{
    // 0x2576: releasing the pedal ends the voices a note-off marked as held.
    ChannelState& ch = m_channels[channel];
    if (value >= 0x40)
    {
        ch.sustain = true;
        return;
    }
    ch.sustain = false;
    for (int v = 0; v < kUsableVoices; ++v)
    {
        const uint16_t s = m_alloc[v].mdiState;
        if ((s & 0xFF) == 0xFF && (s >> 8) != 0xFF && ((s >> 8) & 0x0F) == channel)
            NoteOffMdi(v);
    }
}

void Synth::AllNotesOffMdi(uint8_t channel, bool respectSustain)
{
    // 0x277C: CC123 keeps pedal-held notes held, CC120 ends everything.
    const bool hold = respectSustain && m_channels[channel].sustain;
    for (int v = 0; v < kUsableVoices; ++v)
    {
        VoiceAlloc& a = m_alloc[v];
        const int hi = a.mdiState >> 8;
        if (hi == 0xFF || (hi & 0x0F) != channel) continue;
        if (hold) a.mdiState |= 0x00FF;
        else      NoteOffMdi(v);
    }
}

void Synth::ResetControllersMdi(uint8_t channel)
{
    // 0x2732 (CC121).
    ChannelState& ch = m_channels[channel];
    ch.mdiRpnMode = false;
    ch.mdiRpnLsb = 0;
    ch.mdiRpnMsb = 0;
    ch.mdiModDiv30 = 0;
    ch.modWheel = 0;
    SustainMdi(channel, 0);
    ch.expression = 0x7F;
    UpdateAttenMdi(channel);
    PitchBendMdi(channel, 0);
    ch.mdiPressureDiv30 = 0;
    UpdateFmmodMdi(channel, ch.mdiModDiv30 * 30);
}

bool Synth::ControlChangeMdi(uint8_t channel, uint8_t controller, uint8_t value)
{
    // Controller dispatcher 0x27DE; bank, pan, reverb and chorus are only
    // stored there, which the generic path does as well.
    ChannelState& ch = m_channels[channel];
    switch (controller)
    {
    case 1:
        ch.modWheel = value;
        UpdateFmmodMdi(channel, value);
        return true;
    case 6:
        if (ch.mdiRpnMode && ch.mdiRpnLsb == 0 && ch.mdiRpnMsb == 0)
            ch.pitchBendRangeSemitones = value;
        return true;
    case 7:
        ch.volume = value;
        UpdateAttenMdi(channel);
        return true;
    case 11:
        ch.expression = value;
        UpdateAttenMdi(channel);
        return true;
    case 64:
        SustainMdi(channel, value);
        return true;
    case 100:
        ch.mdiRpnMode = true;
        ch.mdiRpnLsb = value;
        return true;
    case 101:
        ch.mdiRpnMode = true;
        ch.mdiRpnMsb = value;
        return true;
    case 120:
        AllNotesOffMdi(channel, false);
        return true;
    case 121:
        ResetControllersMdi(channel);
        return true;
    case 123:
        AllNotesOffMdi(channel, true);
        return true;
    default:
        return false;
    }
}

void Synth::ChannelPressure(uint8_t channel, uint8_t value)
{
    if (channel >= 16) return;
    if (m_core.DriverVariant() == Awe32::Driver::Win95)
    {
        // SBAWE.VXD 0xC0FFCEAE: pressure / 30 is stored and the FMMOD depth
        // of the channel recomputed (0xC0FFC650).
        m_channels[channel].vxdPressureDiv30 = static_cast<uint8_t>(value / 30);
        UpdateFmmodVxd(channel);
        return;
    }
    // SBAWE32.MDI 0x2AAC.
    if (!Awe32::IsDosLike(m_core.DriverVariant())) return;
    ChannelState& ch = m_channels[channel];
    ch.mdiPressureDiv30 = static_cast<uint8_t>(value / 30);
    UpdateFmmodMdi(channel, ch.mdiModDiv30 * 30);
}

// SBAWE.VXD 0xC0FFAC08: every voice with the same channel + preset key and
// the same exclusive class is cut (IFATN 0x00FF, DCYSUSV 0x807F), whatever
// its state - playing, released or held by the pedal.
void Synth::KillExclusiveVxd(uint32_t key, uint8_t cls)
{
    for (int v = 0; v < kVxdVoices; ++v)
    {
        VoiceAlloc& a = m_alloc[v];
        if (a.vxdExclKey != key || a.vxdExclClass != cls) continue;
        m_core.Write(Reg::IFATN, v, 0x00FF);
        m_core.Write(Reg::DCYSUSV, v, 0x807F);
        a.inUse = false;
        a.heldBySustain = false;
        a.vxdState = 0xFFE;
        a.vxdRom = false;
        a.vxdExclKey = 0;
        a.vxdExclClass = 0;
    }
}

void Synth::StartLayers(size_t bankIndex, const std::vector<SoundFont::Region>& regions,
                        uint8_t channel, uint8_t note, uint8_t velocity, uint32_t presetId)
{
    const SoundFont::Bank& b = *m_banks[bankIndex].bank;
    std::vector<SoundFont::VoiceParams> vps;
    vps.reserve(regions.size());
    for (const SoundFont::Region& r : regions)
        vps.push_back(SoundFont::MakeVoiceParams(
            b, r, note, velocity, m_banks[bankIndex].dramBase, kRomPoolBase,
            m_core.DriverVariant()));

    if (Awe32::IsDosLike(m_core.DriverVariant()))
    {
        // SBAWE32.MDI 0x1EB2: every layer gets its voice (reserved, 0xFFFE)
        // before any register of the note is written.
        // The SDK does the same (noteOn 0x0BA3) with its own allocation,
        // which may find no voice - that layer is then not played.
        const bool sdk = m_core.DriverVariant() == Awe32::Driver::Sdk;
        std::vector<int> voices;
        voices.reserve(regions.size());
        for (size_t i = 0; i < regions.size(); ++i)
            voices.push_back(sdk ? AllocateVoiceSdk(0xFFFE) : AllocateVoiceMdi(0xFFFE));
        for (size_t i = 0; i < regions.size(); ++i)
            if (voices[i] >= 0)
                StartVoice(voices[i], channel, note, velocity, vps[i], &b, &regions[i]);
        return;
    }

    const bool vxd = m_core.DriverVariant() == Awe32::Driver::Win95;
    // SBAWE.VXD: the exclusive classes of all layers are resolved before
    // the first voice is taken. Key = channel << 24 | bank file, bank,
    // program (the driver keeps channel << 16 | preset).
    const uint32_t exclKey = (static_cast<uint32_t>(channel) << 24) | (presetId & 0xFFFFFF);
    if (vxd)
        for (const SoundFont::Region& r : regions)
        {
            const uint8_t cls = static_cast<uint8_t>(r.gen.Get(SoundFont::Gen::ExclusiveClass, 0));
            if (cls != 0) KillExclusiveVxd(exclKey, cls);
        }

    // A preset can have several layers per note - each gets a voice.
    // The win95 family selects the voice like SBAWE.VXD (reading the chip),
    // the others by our own rule.
    for (size_t i = 0; i < regions.size(); ++i)
    {
        const int voice = vxd ? AllocateVoiceVxd(0xFFFE) : AllocateVoice();
        StartVoice(voice, channel, note, velocity, vps[i], &b, &regions[i]);
        if (vxd)
        {
            m_alloc[voice].vxdExclKey = exclKey;
            m_alloc[voice].vxdExclClass = static_cast<uint8_t>(
                regions[i].gen.Get(SoundFont::Gen::ExclusiveClass, 0));
        }
    }
}

int Synth::BankNumberFor(uint8_t channel) const
{
    // Channel 10 (index 9) is drums per GM. SoundFont has them in bank 128.
    if (channel == 9) return kDrumBank;
    return m_channels[channel].bankMsb;
}

int Synth::PitchBendOffset(uint8_t channel) const
{
    const ChannelState& ch = m_channels[channel];
    const long long span =
        static_cast<long long>(ch.pitchBend) * ch.pitchBendRangeSemitones;

    // **The families differ in what they use per semitone.** Both divide as
    // integers only at the end and truncate towards zero, but the constant
    // differs:
    //
    //   win95  4096/12 exactly:  bend * range * 4096 / (8192*12)
    //   dos    341 (truncated):  bend * range * 341 / 8192
    //
    // For win95 it fits eleven measured points (bend, range -> exact ->
    // driver), from Georgia and RELAX:
    //
    //    8064,  2 ->  672.000 ->  672     -768, 12 -> -384.000 -> -384
    //   -4729, 12 -> -2364.50 -> -2364    -682, 12 -> -341.000 -> -341
    //    1280, 12 ->  640.000 ->  640     -512, 12 -> -256.000 -> -256
    //   -1280, 12 -> -640.000 -> -640      176, 12 ->   88.000 ->   88
    //   -1312, 12 -> -656.000 -> -656     8191,  2 ->  682.583 ->  682
    //   -6720,  2 -> -560.000 -> -560
    //
    // For dos the evidence is a full bend down with range 12 in the Magic
    // Carpet 2 intro: the driver wrote an offset of -4092, while 4096/12
    // would give exactly -4096. With 341 the whole intro matches on all 24
    // registers.
    if (Awe32::IsDosLike(m_core.DriverVariant()))
        return static_cast<int>(span * 341 / 8192);
    return static_cast<int>(span * 4096 / (8192LL * 12));
}

// ---------------------------------------------------------------------------
// Starting a voice
// ---------------------------------------------------------------------------

void Synth::StartVoice(int voice, uint8_t channel, uint8_t note, uint8_t velocity,
                       const SoundFont::VoiceParams& vp,
                       const SoundFont::Bank* bank, const SoundFont::Region* region)
{
    const ChannelState& ch = m_channels[channel];

    // Attenuation: an exact transcription of the formula from the note-on
    // routine of SBAWE32.MDI (0x2102), including the conversion tables for
    // CC7, velocity and CC11. See Awe32Curves.h.
    const Awe32::Driver drv = m_core.DriverVariant();
    int atten = Awe32Curves::ComputeAttenuation(
        EffectiveChannelVolume(ch.volume), velocity, ch.expression,
        vp.patchAttenUnits, drv);

    // When the bank refers to the ROM "1MGM", the driver adds 16 units
    // (= 6 dB) to the attenuation. Measured with an instruction trace in
    // SBAWE.VXD (object 1, 0x1CCB): `cmp dword ptr [edi+0x158E], 0x4D474D31`
    // - that is ASCII "1MGM" reversed - and then `add ecx, 0x10` clipped to
    // 0xFF. In the trace ecx went from 0x18 to 0x28 exactly here.
    //
    // The driver also conditions it on a byte flag (`cmp byte ptr [eax], 0`).
    // That flag is **"does the sample lie in ROM?"**. Swapping the bank in
    // the guest revealed it: when SYNTH02S.SBK (with its own samples in DRAM)
    // is loaded instead of SYNTHGM.SBK (describes only the ROM), the driver
    // does **not** add the 16 units - on all 242 notes of MINUET its
    // attenuation was exactly 16 lower than ours and the target volume thus
    // double (16 units = 6 dB = factor 2).
    //
    // It makes physical sense too: the samples in the wave ROM are 6 dB
    // louder than what the driver itself loads into DRAM.
    const bool sampleInRom = region && region->sample
                          && (region->sample->inRom || bank->samplesInRom);
    // The SDK has the same rule (noteOn 0x0E82: sample address below
    // 0x200000 and the card's ROM id "1MGM"); the ROM id comes from the card,
    // not from the bank, and every AWE32 has that ROM.
    if (sampleInRom && ((drv == Awe32::Driver::Win95 && bank && bank->romName == "1MGM")
                        || drv == Awe32::Driver::Sdk))
        atten = std::min(atten + 16, 255);

    // Velocity affects the filter cutoff too - quieter notes are darker.
    // Transcribed from SBAWE32.DRV, offset 0x021E:
    //
    //     if (channel != 9 && attackRate < 0x7D)
    //         cutoff = (cutoff * max(velocity, 0x46) + 0x40) / 0x7F;
    //
    // Drums (channel 9) have their own branch and the filter is not adjusted
    // this way for them. **The families differ here**, and for a while we
    // had both the same (per the VXD), which did not fit DOS:
    //
    //   SBAWE32.DRV 0x021E (dos):    (cutoff * v + 0x40) / 0x7F
    //   SBAWE.VXD   0x1CF6 (win95):  (cutoff * v + 0xA0) >> 7
    //
    // The difference shows only at the top: for cutoff 255 and velocity 127
    // DOS gives 255 (32449/127 = 255.5), while the VXD gives 254
    // (32545/128 = 254.3). Measured on two notes of channel 5 in the Magic
    // Carpet 2 intro - the driver had 0xFF, we had 0xFE.
    int cutoff = (vp.ifatn >> 8) & 0xFF;
    const int attackRate = vp.atkhldv & Emu8000::kAtkhldAttackMask;
    if (channel != 9 && attackRate < 0x7D)
    {
        const int v = std::max<int>(velocity, 0x46);
        // The SDK (noteOn 0x0D10) has the VXD form.
        cutoff = (drv == Awe32::Driver::Dos) ? (cutoff * v + 0x40) / 0x7F
                                             : ((cutoff * v + 0xA0) >> 7);
        cutoff = std::clamp(cutoff, 0, 255);
    }

    const uint16_t ifatn = static_cast<uint16_t>((cutoff << 8) | atten);

    // Pan: `0x17F - 2*(bankPan + CC10)` with two limits. Transcribed from
    // both drivers, which have it literally the same except for the lower
    // limit (CC10 64 = centre; in the EMU8000 0 = right):
    //   SBAWE32.MDI 0x22B6:  ax = 0x17F; cx = [bx+6] + [si+0x22]; cx += cx;
    //                        ax -= cx;  if (ax >= 0xFE) ax = 0xFF;
    //                        if (negative) ax = 0
    //   SBAWE.VXD obj 1, 0x4253: the same formula, only the lower limit is
    //                        `if (ax <= 1) ax = 0`
    // Formerly the finished register value from the bank was added to an
    // offset from CC10 here; it came out the same only because the `pan`
    // generator is missing in both measured banks and the default 64 gives
    // the same result.
    int pan = 0x17F - 2 * (vp.patchPan + static_cast<int>(ch.pan));
    if (pan >= 0xFE) pan = 0xFF;
    else if (pan < (drv == Awe32::Driver::Dos ? 0 : 2)) pan = 0;   // SDK 0x0F49 = VXD
    const uint32_t psst = (static_cast<uint32_t>(pan) << Emu8000::kPanShift)
                        | (vp.psst & Emu8000::kLoopAddressMask);

    // Chorus and reverb: the driver first **scales the channel value from
    // CC93/CC91 to 90 %** and only then adds the value from the bank
    // (clipped to 255). Both families have the scaling literally the same:
    //
    //   SBAWE32.MDI 0x242A (reverb) and 0x244C (chorus):
    //       mov ax, 0x5a; mul si; mov cx, 0x64; div cx  -> [channel+4] / [+5]
    //   SBAWE.VXD obj 1, 0x314D (reverb) and 0x3174 (chorus):
    //       imul eax, eax, 0x5a; mov ecx, 0x64; div ecx -> [channel+0x447] / [+0x448]
    //
    // The sum form is read from the MDI (0x2290 reverb, 0x230A chorus:
    // `al = [bx+4]; add ax, [si+0x20]` and clipping to 0xFF). For the VXD we
    // do not have it directly from the code - MINUET.MID sends no CC91 or
    // CC93, so that measurement does not decide it - but the stored value is
    // prepared the same way there.
    const int chChorus = ch.chorusSend * 90 / 100;
    const int chorus = std::clamp(
        static_cast<int>((vp.csl >> Emu8000::kChorusShift) & 0xFF) + chChorus, 0, 255);
    const uint32_t csl = (static_cast<uint32_t>(chorus) << Emu8000::kChorusShift)
                       | (vp.csl & Emu8000::kLoopAddressMask);
    const int chReverb = ch.reverbSend * 90 / 100;
    const int reverb = std::clamp(static_cast<int>(vp.reverbSend) + chReverb, 0, 255);

    // `dos`: SBAWE32.MDI adds the offset stored at the last bend event
    // (see PitchBendMdi), capped above and wrapping below.
    const int pitch = (drv == Awe32::Driver::Sdk) ? SdkAddBend(vp.ip, ch.mdiBendOffset, true)
        : (drv == Awe32::Driver::Dos) ? MdiAddBend(vp.ip, ch.mdiBendOffset)
        : std::clamp(vp.ip + PitchBendOffset(channel), 0, 65535);

    // The modulation wheel adds LFO1 depth to the pitch. The `SBAWE.VXD`
    // handler of CC1 (0x34A4) divides the value **by thirty** and adds the
    // result to the depth from the patch; the sum is clipped to 0x7F and goes
    // to the high byte of FMMOD:
    //
    //     mov ecx, 0x1E / div ecx      ; CC1 / 30 -> 0..4
    //     add ebp, edx                 ; + depth from the patch
    //     cmp ebp, 0x7F / shl ebp, 8
    //
    // Measured on RELAX: the driver had 01, 02 and 04 where we had zero.
    // `dos` (SBAWE32.MDI 0x2224): CC1 / 30 + channel pressure / 30 + patch
    // depth, capped at 0x7F from above only.
    // `win95` (SBAWE.VXD 0xC0FFB1D5 / 0xC0FFBE0B): CC1 / 30 + pressure / 30
    // + patch depth, capped above; the low byte is ORed in sign-extended, so
    // a negative filter depth turns the high byte into 0xFF.
    const int modDepth = (Awe32::IsDosLike(drv))
        ? std::min(static_cast<int>(static_cast<int8_t>((vp.fmmod >> 8) & 0xFF))
                       + ch.mdiModDiv30 + ch.mdiPressureDiv30, 0x7F)
        : std::clamp(
              static_cast<int>(static_cast<int8_t>((vp.fmmod >> 8) & 0xFF))
                  + ch.modWheel / 30 + ch.vxdPressureDiv30, -128, 0x7F);
    uint16_t fmmod = static_cast<uint16_t>(
        ((static_cast<uint8_t>(modDepth)) << 8) | (vp.fmmod & 0xFF));
    if (drv == Awe32::Driver::Win95 && (vp.fmmod & 0x80))
        fmmod |= 0xFF00;

    const uint32_t reverbByte =
        static_cast<uint32_t>(std::clamp(reverb, 0, 255)) << Emu8000::kReverbShift;
    // The low byte of PTRX is an auxiliary pan. `SBAWE.VXD` puts 256 - pan
    // there (writes 0x81 at pan 0x7F), measured in the voice block at +0x24.
    // `SBAWE32.MDI` leaves **zero** there - measured on 341 notes of Magic
    // Carpet 2, where PSST carries pan 0x0F and PTRX has the low byte 0x00.
    // The SDK (noteOn 0x0F5B: pan ? -pan : 0xFF) has the same byte.
    const uint32_t panAux = (drv != Awe32::Driver::Dos)
        ? static_cast<uint32_t>(std::clamp(256 - pan, 0, 255))
        : 0u;
    // The start address is shifted by a constant depending on the driver
    // family.
    const uint32_t ccca = (vp.ccca & ~Emu8000::kCccaAddressMask)
        | ((vp.sampleStart - Awe32::StartAddressOffset(drv))
           & Emu8000::kCccaAddressMask);

    if (drv == Awe32::Driver::Win95)
    {
        // The exact write sequence of `SBAWE.VXD`, read from a win95 trace
        // (`tests/voice_seq.py --note N`). Verified to be the same for all
        // notes and voices - see docs/re-notes/driver_note_on.md.
        //
        // The upper half of PTRX (and likewise CPF) is not the logarithmic
        // IP but a **linear increment** - transcribed from SBAWE.VXD
        // (object 1, 0x212E).
        // `SBAWE.VXD` 0x2099 (modulation) and 0x219C (volume): when the
        // attack is at maximum and there is no delay, the constant 0xBFFF is
        // sent to the port instead of the computed delay. The original value
        // stays in the parameter block, so we do not change it either.
        const bool volInstant = (vp.atkhldv & 0x7F) == 0x7F && vp.envvol >= 0x8000;
        const bool modInstant = (vp.atkhld  & 0x7F) == 0x7F && vp.envval >= 0x8000;
        const uint32_t envvolReg = volInstant ? 0xBFFFu : vp.envvol;
        const uint32_t envvalReg = modInstant ? 0xBFFFu : vp.envval;
        const uint32_t increment = PitchIncrement(static_cast<uint16_t>(pitch));
        const uint32_t filterTarget = static_cast<uint32_t>(cutoff) << 8;

        // Silencing: the driver writes 0x00FF, not 0x0080. VTFT goes twice.
        m_core.Write(Reg::DCYSUSV, voice, 0x00FFu);
        m_core.Write(Reg::VTFT,    voice, 0x0000FFFFu);
        m_core.Write(Reg::VTFT,    voice, 0x0000FFFFu);
        m_core.Write(Reg::CVCF,    voice, 0x0000FFFFu);

        m_core.Write(Reg::ATKHLDV, voice, vp.atkhldv);
        m_core.Write(Reg::LFO1VAL, voice, vp.lfo1val);
        m_core.Write(Reg::ATKHLD,  voice, vp.atkhld);
        m_core.Write(Reg::DCYSUS,  voice, vp.dcysus);
        m_core.Write(Reg::LFO2VAL, voice, vp.lfo2val);
        m_core.Write(Reg::IP,      voice, static_cast<uint16_t>(pitch));
        m_core.Write(Reg::IFATN,   voice, ifatn);
        m_core.Write(Reg::PEFE,    voice, vp.pefe);
        m_core.Write(Reg::FMMOD,   voice, fmmod);
        m_core.Write(Reg::TREMFRQ, voice, vp.tremfrq);
        m_core.Write(Reg::FM2FRQ2, voice, vp.fm2frq2);
        m_core.Write(Reg::ENVVAL,  voice, envvalReg);
        m_core.Write(Reg::ENVVOL,  voice, envvolReg);

        // Cleared before the addresses; the real values come here only at
        // the end.
        m_core.Write(Reg::PTRX, voice, 0u);
        m_core.Write(Reg::CPF,  voice, 0u);
        m_core.Write(Reg::PSST, voice, psst);
        m_core.Write(Reg::CSL,  voice, csl);
        // CCCA twice: first without Q in the high byte, then with it. When Q
        // is zero both writes are the same - which matches the trace too.
        m_core.Write(Reg::CCCA, voice, ccca & 0x00FFFFFFu);
        // The driver clears Z1/Z2 (Data0 registers 5 and 4) for every note.
        // What exactly they mean we do not know; 86Box keeps them only as a
        // stored word.
        m_core.Write(Reg::Unk0088, voice, 0u);   // Z1
        m_core.Write(Reg::Unk0080, voice, 0u);   // Z2
        m_core.Write(Reg::CCCA, voice, ccca);

        // When the envelope has neither attack nor delay, the driver does not
        // ramp the voice up from zero but sets the target volume right away -
        // into the upper half of VTFT and CVCF. The condition and the table
        // are from `SBAWE.VXD` 0x219C..0x21EF, see Awe32Curves.h. Measured on
        // 844 notes of Georgia.
        const uint32_t volTarget =
            volInstant ? Awe32Curves::VolumeTarget(atten) : 0u;
        const uint32_t vtft = (volTarget << 16) | filterTarget;
        m_core.Write(Reg::VTFT, voice, vtft);
        m_core.Write(Reg::CVCF, voice, vtft);
        m_core.Write(Reg::PTRX, voice, (increment << 16) | reverbByte | panAux);
        m_core.Write(Reg::CPF,  voice, increment << 16);
    }
    else if (drv == Awe32::Driver::Sdk)
    {
        // AWE32 DOS SDK, midieng noteOn 0x0FB3..0x1349 - the VXD sequence
        // without the leading DCYSUSV/VTFT/CVCF (the allocation wrote
        // DCYSUSV 0x0080 and VTFT already) and with CVCF after ENVVOL.
        // Instant attack (0x10CF / 0x11EE): envelope delay 0xBFFF, and for
        // the modulation envelope the targets already include it - pitch
        // += PEFE pitch << 4 (16-bit, overflow -> 0xFFFF), filter = cutoff
        // + PEFE filter clamped to 0..255.
        const bool volInstant = (vp.atkhldv & 0x7F) == 0x7F && vp.envvol >= 0x8000;
        const bool modInstant = (vp.atkhld  & 0x7F) == 0x7F && vp.envval >= 0x8000;
        const uint32_t envvolReg = volInstant ? 0xBFFFu : vp.envvol;
        const uint32_t envvalReg = modInstant ? 0xBFFFu : vp.envval;

        uint32_t targetPitch = static_cast<uint32_t>(pitch) & 0xFFFFu;
        int filter = cutoff;
        if (modInstant)
        {
            const int16_t depth = static_cast<int16_t>(
                static_cast<int16_t>(static_cast<int8_t>((vp.pefe >> 8) & 0xFF)) << 4);
            uint32_t lo = static_cast<uint32_t>(static_cast<uint16_t>(depth)) + targetPitch;
            const uint32_t hi = ((depth < 0) ? 0xFFFFu : 0u) + (lo >> 16);
            lo &= 0xFFFFu;
            targetPitch = ((hi & 0xFFFFu) != 0) ? 0xFFFFu : lo;
            filter = std::clamp(cutoff + static_cast<int8_t>(vp.pefe & 0xFF), 0, 255);
        }
        const uint32_t increment = PitchIncrementSdk(static_cast<uint16_t>(targetPitch));
        const uint32_t volTarget = volInstant ? Awe32Curves::VolumeTarget(atten) : 0u;
        const uint32_t vtft = (volTarget << 16) | (static_cast<uint32_t>(filter) << 8);

        m_core.Write(Reg::VTFT,    voice, 0x0000FFFFu);
        m_core.Write(Reg::ATKHLDV, voice, vp.atkhldv);
        m_core.Write(Reg::LFO1VAL, voice, vp.lfo1val);
        m_core.Write(Reg::ATKHLD,  voice, vp.atkhld);
        m_core.Write(Reg::DCYSUS,  voice, vp.dcysus);
        m_core.Write(Reg::LFO2VAL, voice, vp.lfo2val);
        m_core.Write(Reg::IP,      voice, static_cast<uint16_t>(pitch));
        m_core.Write(Reg::IFATN,   voice, ifatn);
        m_core.Write(Reg::PEFE,    voice, vp.pefe);
        m_core.Write(Reg::FMMOD,   voice, fmmod);
        m_core.Write(Reg::TREMFRQ, voice, vp.tremfrq);
        m_core.Write(Reg::FM2FRQ2, voice, vp.fm2frq2);
        m_core.Write(Reg::ENVVAL,  voice, envvalReg);
        m_core.Write(Reg::ENVVOL,  voice, envvolReg);
        m_core.Write(Reg::CVCF,    voice, 0x0000FFFFu);
        m_core.Write(Reg::PTRX,    voice, 0u);
        m_core.Write(Reg::CPF,     voice, 0u);
        m_core.Write(Reg::PSST,    voice, psst);
        m_core.Write(Reg::CSL,     voice, csl);
        m_core.Write(Reg::CCCA,    voice, ccca & 0x00FFFFFFu);
        m_core.Write(Reg::Unk0088, voice, 0u);   // Z1
        m_core.Write(Reg::Unk0080, voice, 0u);   // Z2
        m_core.Write(Reg::CCCA,    voice, ccca);
        m_core.Write(Reg::VTFT,    voice, vtft);
        m_core.Write(Reg::CVCF,    voice, vtft);
        m_core.Write(Reg::PTRX,    voice, (increment << 16) | reverbByte | panAux);
        m_core.Write(Reg::CPF,     voice, increment << 16);
    }
    else
    {
        // `SBAWE32.MDI` note-on 0x1F12..0x2363, write order as in the driver
        // (confirmed on the game trace dos_mdi.trace, 260 notes of the MC2
        // intro): unit pitch, voice off, volume target 0, envelopes, pitch and
        // modulation, PTRX read-modify-write (the chip has already derived the
        // pitch target from IP, only the reverb byte is replaced), addresses.
        m_core.Write(Reg::IP,      voice, 0xE000u);
        m_core.Write(Reg::DCYSUSV, voice, Emu8000::kDcysusvOff);
        m_core.Write(Reg::VTFT,    voice, 0x0000FFFFu);
        m_core.Write(Reg::ENVVOL,  voice, vp.envvol);
        m_core.Write(Reg::ATKHLDV, voice, vp.atkhldv);
        m_core.Write(Reg::ENVVAL,  voice, vp.envval);
        m_core.Write(Reg::ATKHLD,  voice, vp.atkhld);
        m_core.Write(Reg::DCYSUS,  voice, vp.dcysus);
        m_core.Write(Reg::IP,      voice, static_cast<uint16_t>(pitch));
        m_core.Write(Reg::IFATN,   voice, ifatn);
        m_core.Write(Reg::LFO1VAL, voice, vp.lfo1val);
        m_core.Write(Reg::LFO2VAL, voice, vp.lfo2val);
        m_core.Write(Reg::PEFE,    voice, vp.pefe);
        m_core.Write(Reg::FMMOD,   voice, fmmod);
        m_core.Write(Reg::TREMFRQ, voice, vp.tremfrq);
        m_core.Write(Reg::FM2FRQ2, voice, vp.fm2frq2);
        const uint32_t cur = m_core.ReadDriver(Reg::PTRX, voice);
        m_core.Write(Reg::PTRX, voice, (cur & 0xFFFF00FFu) | reverbByte);
        m_core.Write(Reg::PSST, voice, psst);
        m_core.Write(Reg::CSL,  voice, csl);
        m_core.Write(Reg::CCCA, voice, ccca);
    }

    m_core.Write(Reg::DCYSUSV, voice, vp.dcysusv);   // starts the note

    VoiceAlloc& a = m_alloc[voice];
    a = VoiceAlloc{};
    a.inUse = true;
    a.channel = channel;
    a.note = note;
    a.velocity = velocity;
    a.releaseRate = vp.releaseRate ? vp.releaseRate : 0x40;
    a.releaseModRate = vp.releaseModRate;
    a.age = ++m_ageCounter;
    a.basePitch = vp.ip;
    if (Awe32::IsDosLike(drv))
    {
        // Voice block of SBAWE32.MDI (0x20A4..0x20DD, 0x2366). A looped
        // sample with at least 0x14 words after the loop end gets a note-off
        // loop opening at its end (0x2006..0x2060).
        a.mdiState      = static_cast<uint16_t>((channel << 8) | note);
        a.mdiVelocity   = velocity;
        a.mdiPatchAtten = vp.patchAttenUnits;
        a.sdkRom        = sampleInRom;
        a.mdiModRelease = vp.releaseModRate;
        a.mdiVolRelease = vp.releaseRate;
        a.mdiFmmodDepth = static_cast<int8_t>((vp.fmmod >> 8) & 0xFF);
        a.mdiNoteEnd    = (vp.looping && (vp.sampleEndAddr - vp.loopEndAddr) >= 0x14u)
                        ? vp.sampleEndAddr + 4 : 0u;
        a.mdiEndAddr    = 0;
    }
    else
    {
        a.vxdState = static_cast<uint16_t>((channel << 8) | note);
        a.vxdRom = sampleInRom;
        // SBAWE.VXD 0xC0FFB0C8: a sample without a loop gets the end for the
        // CCCA test (+0x0A) already at note-on, sample end + 4 - such a voice
        // is free for the next note as soon as it has played out. A looped
        // sample gets it only when the loop is opened at note-off (0xC0FFA29B).
        a.vxdEndAddr = vp.looping ? 0u : vp.sampleEndAddr + 4;
        a.vxdPatchPan = static_cast<uint8_t>(vp.patchPan);
        a.vxdFmmodDepth = static_cast<int8_t>((vp.fmmod >> 8) & 0xFF);
        a.vxdPatchAtten = vp.patchAttenUnits;
        a.vxdRom1mgm = bank && bank->romName == "1MGM" && sampleInRom;
        // Same rule as SBAWE32.MDI: a looped sample with at least 0x14 words
        // after the loop end gets its loop moved behind the end at note-off
        // (block +0x06 = sample end + 4, relax/georgia traces: PSST 983F, CSL 9843).
        a.vxdNoteEnd = (vp.looping && (vp.sampleEndAddr - vp.loopEndAddr) >= 0x14u)
                     ? vp.sampleEndAddr + 4 : 0u;
        a.vxdPanTarget = a.vxdPanCur = static_cast<uint8_t>(pan);
        a.vxdPanTimer = -1;
    }

    if (m_noteDump)
    {
        // The column order and names follow the block fields of `SBAWE.VXD`
        // (offsets in brackets), so it can be put next to patch_struct.py.
        std::fprintf(static_cast<FILE*>(m_noteDump),
                     "%d,%d,%d,%d,%04X,%04X,%04X,%04X,%04X,%04X,%04X,%04X,%04X,%04X,%06X,%06X,%06X,%d,%d,%d\n",
                     static_cast<int>(channel), static_cast<int>(note),
                     static_cast<int>(velocity), voice,
                     static_cast<unsigned>((vp.ccca >> Emu8000::kCccaQShift) & 0xF), // 0x12 Q
                     static_cast<unsigned>(reverb & 0xFF),                           // 0x20
                     static_cast<unsigned>(panAux & 0xFF),                           // 0x24
                     static_cast<unsigned>(atten & 0xFF),                            // 0x26
                     static_cast<unsigned>(cutoff & 0xFF),
                     static_cast<unsigned>(vp.envval),                               // 0x32
                     static_cast<unsigned>(vp.atkhld & 0x7F),                        // 0x34
                     static_cast<unsigned>(vp.envvol),                               // 0x42
                     static_cast<unsigned>(vp.atkhldv & 0x7F),                       // 0x44
                     static_cast<unsigned>((vp.atkhldv >> 8) & 0x7F),                // 0x48
                     static_cast<unsigned>(pitch),
                     static_cast<unsigned>(ccca & Emu8000::kCccaAddressMask),
                     static_cast<unsigned>(vp.csl & Emu8000::kLoopAddressMask),
                     // The pitch bend is already included in the final IP;
                     // analysing differences against the driver needs its
                     // inputs and its contribution alone too.
                     static_cast<int>(ch.pitchBend),
                     static_cast<int>(ch.pitchBendRangeSemitones),
                     PitchBendOffset(channel));
    }

    if (m_debugVoices > 0)
    {
        --m_debugVoices;
        const bool rom = bank && region && region->sample
                      && (region->sample->inRom || bank->samplesInRom);
        std::cout << "  voice " << voice
                  << " ch" << static_cast<int>(channel)
                  << " prog" << static_cast<int>(m_channels[channel].program)
                  << " note " << static_cast<int>(note)
                  << " vel " << static_cast<int>(velocity)
                  << " | " << (region && region->sample ? region->sample->name
                                                        : std::string("(substitute sine)"))
                  << (rom ? " [ROM]" : "")
                  << std::hex
                  << " | addr " << (ccca & Emu8000::kCccaAddressMask)
                  << " loop " << (vp.psst & Emu8000::kLoopAddressMask)
                  << ".." << (vp.csl & Emu8000::kLoopAddressMask)
                  << " IP " << vp.ip << " IFATN " << ifatn
                  << " ATKHLDV " << vp.atkhldv << " DCYSUSV " << vp.dcysusv
                  << std::dec << "\n";
    }
}

void Synth::StartFallbackVoice(int voice, uint8_t channel, uint8_t note, uint8_t velocity)
{
    SoundFont::VoiceParams vp;
    vp.sampleStart = m_fallbackStart;
    vp.ccca = m_fallbackStart & Emu8000::kCccaAddressMask;
    vp.psst = (128u << Emu8000::kPanShift) | (m_fallbackLoopStart & Emu8000::kLoopAddressMask);
    vp.csl  = m_fallbackLoopEnd & Emu8000::kLoopAddressMask;

    const double freqHz = 440.0 * std::pow(2.0, (note - 69) / 12.0);
    const double octaves = std::log2(freqHz / m_fallbackUnityHz);
    vp.ip = static_cast<uint16_t>(std::clamp(
        Emu8000::kPitchUnity + octaves * Emu8000::kPitchPerOctave, 0.0, 65535.0));

    const double vel = std::max(1, static_cast<int>(velocity)) / 127.0;
    vp.ifatn = static_cast<uint16_t>((0xFF << 8) | AttenDbToUnits(-40.0 * std::log10(vel)));
    vp.envvol = 0x8000; vp.atkhldv = MakeAtkhld(0x7F, 0x7F); vp.dcysusv = MakeDcysusv(0x7F, 0);
    vp.envval = 0x8000; vp.atkhld  = MakeAtkhld(0x7F, 0x7F); vp.dcysus  = MakeDcysusv(0x7F, 0);
    vp.lfo1val = vp.lfo2val = 0x8000;
    vp.releaseRate = 0x40;

    StartVoice(voice, channel, note, velocity, vp, nullptr, nullptr);
}

// ---------------------------------------------------------------------------
// MIDI interface
// ---------------------------------------------------------------------------

void Synth::NoteOn(uint8_t channel, uint8_t note, uint8_t velocity)
{
    if (channel >= 16 || note > 127) return;
    if (!((m_channelMask >> channel) & 1)) return;
    if (velocity == 0) { NoteOff(channel, note); return; }

    const int bankNum = BankNumberFor(channel);
    const int program = m_channels[channel].program;

    // Search order. Each step is tried over ALL loaded banks before moving
    // to the next - otherwise a user bank with preset 0 would override the
    // GM drums just because it was loaded later.
    //
    //   1) exact bank + program
    //   2) for drums also bank 128, program 0 = the "Standard" kit.
    //      A GM drum bank often has only the basic kit while the song sends
    //      another program number; without this step the whole drum part
    //      would be replaced by a melodic preset from bank 0.
    //   3) bank 0 with the same program - but **only on melodic channels**.
    // On the drum channel the driver does not touch bank 0: when it does not
    // find the kit, it takes the first preset of the bank right away.
    // Measured on RELAX.SBK (32 presets 0..31, no drum bank) with the song
    // RELAX_VX, where channel 9 has program 16: the driver played preset 0
    // (0x20000C) on all 1849 notes, while we took the melodic preset 16 from
    // bank 0.
    int chain[3];
    int chainLen = 0;
    chain[chainLen++] = bankNum;
    const bool drums = (bankNum == kDrumBank);
    if (drums) chain[chainLen++] = -1;          // marker for (128, program 0)
    if (bankNum != 0 && !drums) chain[chainLen++] = 0;

    for (int pass = 0; pass < chainLen; ++pass)
    {
        const bool standardKit = (chain[pass] == -1);
        const int wantBank = standardKit ? kDrumBank : chain[pass];
        const int wantProgram = standardKit ? 0 : program;

        // Within one pass the search starts at the last loaded bank - a user
        // bank overrides a GM preset with the same number.
        for (size_t i = m_banks.size(); i-- > 0; )
        {
            const SoundFont::Bank& b = *m_banks[i].bank;
            const std::vector<SoundFont::Region> regions =
                b.Select(wantBank, wantProgram, note, velocity);
            if (regions.empty()) continue;

            StartLayers(i, regions, channel, note, velocity,
                        (static_cast<uint32_t>(i) << 16) | (wantBank << 8) | wantProgram);
            return;
        }
    }

    // When the program is not in the bank, the driver takes the **first
    // preset of the bank**, not some substitute of its own. Measured on
    // RELAX.SBK (32 vocal presets 0..31) played with the song RELAX_VX, which
    // uses GM programs up to 122: for all the missing ones the driver played
    // the sample at 0x20000C, i.e. preset 0. We played the substitute sine
    // from `kDramOffset` instead, which showed in CCCA as 0x1FFFFC.
    for (size_t i = m_banks.size(); i-- > 0; )
    {
        const SoundFont::Bank& b = *m_banks[i].bank;
        const std::vector<SoundFont::Region> regions =
            b.Select(0, 0, note, velocity);
        if (regions.empty()) continue;
        StartLayers(i, regions, channel, note, velocity, static_cast<uint32_t>(i) << 16);
        return;
    }

    // Only when the bank has no preset 0 either - then it is a bank without
    // usable content and the substitute sample plays.
    const Awe32::Driver drv = m_core.DriverVariant();
    const int fallback = (drv == Awe32::Driver::Sdk) ? AllocateVoiceSdk(0xFFFE)
                       : (drv == Awe32::Driver::Dos) ? AllocateVoiceMdi(0xFFFE)
                       : AllocateVoice();
    if (fallback >= 0)
        StartFallbackVoice(fallback, channel, note, velocity);
}

void Synth::NoteOff(uint8_t channel, uint8_t note)
{
    if (channel >= 16) return;
    if (Awe32::IsDosLike(m_core.DriverVariant()))
    {
        // SBAWE32.MDI 0x238A: every layer of the key on the channel; with the
        // pedal down the voice is only marked as held (low byte 0xFF).
        const uint16_t key = static_cast<uint16_t>((channel << 8) | note);
        for (int v = 0; v < kUsableVoices; ++v)
        {
            VoiceAlloc& a = m_alloc[v];
            if ((a.mdiState >> 8) == 0xFF || (a.mdiState & 0x0FFF) != key) continue;
            if (m_channels[channel].sustain) a.mdiState |= 0x00FF;
            else                             NoteOffMdi(v);
        }
        return;
    }
    for (int i = 0; i < NoteVoices(); ++i)
    {
        VoiceAlloc& a = m_alloc[i];
        if (!a.inUse || a.heldBySustain) continue;
        if (a.channel != channel || a.note != note) continue;

        if (m_channels[channel].sustain)
        {
            a.heldBySustain = true;
            a.vxdState = static_cast<uint16_t>((a.vxdState & 0xFF00u) | 0xFFu);
        }
        else                             ReleaseVoice(i);
    }
}

void Synth::ProgramChange(uint8_t channel, uint8_t program)
{
    if (channel >= 16) return;
    m_channels[channel].program = program;
}

void Synth::RefreshChannel(uint8_t channel)
{
    const int bend = PitchBendOffset(channel);
    const bool vxd = m_core.DriverVariant() == Awe32::Driver::Win95;
    for (int i = 0; i < NoteVoices(); ++i)
    {
        VoiceAlloc& a = m_alloc[i];
        if (vxd)
        {
            // SBAWE.VXD matches the high byte of the voice state only: a free
            // voice (0x0FFE) belongs to channel 15, so bends there move the
            // stored IP of every free voice (relax_win95.trace, 43.26 s).
            if ((a.vxdState >> 8) != channel) continue;
        }
        else if ((!a.inUse && !a.heldBySustain) || a.channel != channel) continue;

        const uint16_t pitch = static_cast<uint16_t>(std::clamp(a.basePitch + bend, 0, 65535));
        // IP only. PTRX is **not written** - the chip computes the target
        // pitch in its upper half from the IP write itself (see
        // Emu8000Core::PortOut16). The real driver does it that way too: a
        // win95 trace has IP 4418 times, exactly as we do, but no extra PTRX
        // writes. Formerly `pitch << 16` was written here, which is the
        // logarithmic IP - but the upper half of PTRX is a linear increment,
        // so it overwrote the correct value computed by the chip.
        m_core.Write(Reg::IP, i, pitch);
    }
}

void Synth::ControlChange(uint8_t channel, uint8_t controller, uint8_t value)
{
    if (channel >= 16) return;
    if (Awe32::IsDosLike(m_core.DriverVariant())
        && ControlChangeMdi(channel, controller, value))
        return;
    ChannelState& ch = m_channels[channel];

    switch (controller)
    {
    case 0:  ch.bankMsb = value; break;
    case 1:
        ch.modWheel = value;
        if (m_core.DriverVariant() == Awe32::Driver::Win95) UpdateFmmodVxd(channel);
        break;
    case 32: ch.bankLsb = value; break;
    case 7:
        ch.volume = value;
        if (m_core.DriverVariant() == Awe32::Driver::Win95) UpdateAttenVxd(channel);
        break;
    case 10:
        if (m_core.DriverVariant() == Awe32::Driver::Win95 && ch.pan != value)
        {
            ch.pan = value;
            PanVxd(channel, value);
        }
        ch.pan = value;
        break;
    case 11:
        ch.expression = value;
        if (m_core.DriverVariant() == Awe32::Driver::Win95) UpdateAttenVxd(channel);
        break;
    case 91: ch.reverbSend = value; break;
    case 93: ch.chorusSend = value; break;

    // The pitch bend range is set through RPN 0,0. Without it we had two
    // semitones hard-wired, the MIDI default - but the Miles driver in the
    // game uses **twelve**, and it showed: four notes on ch6 in the Magic
    // Carpet 2 intro had IP 10 semitones higher, because a full bend down
    // lies on them (2 semitones for us instead of 12).
    case 101: ch.rpnMsb = value; break;
    case 100: ch.rpnLsb = value; break;
    case 6:
        if (ch.rpnMsb == 0 && ch.rpnLsb == 0)
            ch.pitchBendRangeSemitones = value;
        break;
    // The fine part of the range (cents) does not fit into the register
    // anyway - the driver computes in whole semitones, so we just accept it.
    case 38: break;

    case 64:
    {
        const bool wasOn = ch.sustain;
        ch.sustain = value >= 64;
        if (wasOn && !ch.sustain)
            for (int i = 0; i < NoteVoices(); ++i)
                if (m_alloc[i].heldBySustain && m_alloc[i].channel == channel)
                    ReleaseVoice(i);
        break;
    }

    case 121:
        // SBAWE.VXD 0xC0FFC8ED: CC1 = 0 without a voice update, pedal off,
        // expression 127 (IFATN), bend to the centre (IP), pressure 0 (FMMOD).
        if (m_core.DriverVariant() != Awe32::Driver::Win95) break;
        ch.modWheel = 0;
        ControlChange(channel, 64, 0);
        ControlChange(channel, 11, 127);
        PitchBend(channel, 0);
        ChannelPressure(channel, 0);
        break;

    case 120:
        for (int i = 0; i < NoteVoices(); ++i)
            if (m_alloc[i].channel == channel) KillVoice(i);
        break;

    case 123:
        for (int i = 0; i < NoteVoices(); ++i)
            if (m_alloc[i].inUse && m_alloc[i].channel == channel) ReleaseVoice(i);
        break;

    default:
        break;
    }
}

void Synth::PitchBend(uint8_t channel, int16_t value)
{
    if (channel >= 16) return;
    if (Awe32::IsDosLike(m_core.DriverVariant()))
    {
        PitchBendMdi(channel, value);
        return;
    }
    m_channels[channel].pitchBend = value;
    RefreshChannel(channel);
}

void Synth::RenderBlock(int16_t* out, uint32_t numFrames)
{
    if (m_core.DriverVariant() == Awe32::Driver::Win95)
        for (int v = 0; v < kVxdVoices; ++v)
        {
            VoiceAlloc& a = m_alloc[v];
            if (a.vxdPanTimer < 0) continue;
            a.vxdPanTimer -= static_cast<int32_t>(numFrames);
            if (a.vxdPanTimer <= 0) PanStepVxd(v);
        }
    m_core.RenderBlock(out, numFrames);
}

bool Synth::OpenNoteDump(const std::string& path)
{
    CloseNoteDump();
    FILE* f = std::fopen(path.c_str(), "wb");
    if (!f) return false;
    std::fprintf(f, "ch,note,vel,voice,Q,reverb,panAux,atten,cutoff,"
                    "envvalDelay,modAttack,envvolDelay,volAttack,volHold,ip,ccca,loopEnd,bend,bendRange,bendOffset\n");
    m_noteDump = f;
    return true;
}

void Synth::CloseNoteDump()
{
    if (m_noteDump) { std::fclose(static_cast<FILE*>(m_noteDump)); m_noteDump = nullptr; }
}
