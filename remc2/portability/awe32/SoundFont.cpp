#include "SoundFont.h"
#include "I18n.h"
#include "Emu8000Regs.h"
#include "Awe32Driver.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <map>
#include <typeinfo>

namespace SoundFont
{
namespace
{
    uint16_t RdU16(const uint8_t* p) { return static_cast<uint16_t>(p[0] | (p[1] << 8)); }
    uint32_t RdU32(const uint8_t* p)
    {
        return static_cast<uint32_t>(p[0]) | (static_cast<uint32_t>(p[1]) << 8)
             | (static_cast<uint32_t>(p[2]) << 16) | (static_cast<uint32_t>(p[3]) << 24);
    }

    std::string CStr(const uint8_t* p, size_t maxLen)
    {
        size_t n = 0;
        while (n < maxLen && p[n]) ++n;
        std::string s(reinterpret_cast<const char*>(p), n);
        while (!s.empty() && s.back() == ' ') s.pop_back();
        return s;
    }

    struct Chunk { size_t offset = 0; uint32_t size = 0; bool found = false; };

    // Walks the RIFF tree and collects all leaf chunks by name.
    void WalkChunks(const std::vector<uint8_t>& buf, size_t pos, size_t end,
                    std::map<std::string, Chunk>& out)
    {
        while (pos + 8 <= end)
        {
            const std::string id(reinterpret_cast<const char*>(&buf[pos]), 4);
            const uint32_t size = RdU32(&buf[pos + 4]);
            const size_t data = pos + 8;
            if (data > end) break;

            if (id == "LIST" && data + 4 <= end)
            {
                WalkChunks(buf, data + 4, std::min(data + size, end), out);
            }
            else if (!out.count(id))
            {
                Chunk c; c.offset = data; c.size = size; c.found = true;
                out[id] = c;
            }
            pos = data + size + (size & 1);
        }
    }

    // -------------------------------------------------------------------
    // unit conversions
    // -------------------------------------------------------------------

    // Divisor in the 7-bit "floating" encoding of the EMU8000 envelope rates.
    // See docs/re-notes/emu8000_register_map.md, section 5.
    int RateDivisor(int index)
    {
        const int group = (index >> 4) & 7;
        const int m = index & 15;
        return (group == 0) ? (m + 1) : ((m + 17) << (group - 1));
    }

    // Inversion of the driver tables: find the smallest rate whose time is
    // <= the requested one. The same logic as sub_2BC0 / sub_2BF0 in
    // SBAWE32.DRV. The attack time table is in `SBAWE.VXD` at offset
    // **0x09118** - 128 entries of 16 bits, in whole ms.
    // `11878 / RateDivisor(r-1)` reproduces it after rounding **on all 127
    // entries**, so it need not be copied here; it only has to be searched
    // the way the driver does:
    //
    //   the first entry that is **shorter** than the given time (strictly)
    //   zero time -> 0x7F, falling through the loop -> 0x7E
    //
    // Verified on four points from two songs: 0 ms -> 0x7F, 6 ms -> 0x7E
    // (Georgia), 20 ms -> 100 and 1270 ms -> 10 (JUMP, presets `polysynth`
    // and `spolysynth`). Formerly `r-1` was returned against the **exact**
    // times instead of `r` against the rounded ones, which did not matter on
    // Georgia - it never reaches the middle of the table - but on JUMP it
    // made 1008 voices differ.
    int AttackRateFromMs(double ms)
    {
        if (ms <= 0.0) return 0x7F;
        for (int r = 1; r <= 0x7F; ++r)
            if (ms > static_cast<double>(std::lround(11878.0 / RateDivisor(r - 1))))
                return r;
        return 0x7E;
    }
    // When the generator is not in the bank, the default value from the
    // driver's table applies, which comes to 0x7D. Verified on 242 notes of
    // MINUET and on the presets `organ3`, `jazzgtr`, `fretlessbs` and
    // `piano2` in Georgia.
    constexpr int kAttackDefaultRate = 0x7D;
    // The driver picks the entry whose time is **longer than or equal to**
    // the given one, not the first shorter one. Measured with an instruction
    // trace: decay 12600 ms, keynum scaling 183 and note 69 give
    // 12600 - 9*183 = 10953 ms, and the driver wrote rate 4 (table 11878 ms),
    // not 5 (9502 ms).
    int DecayRateFromMs(double ms)
    {
        if (ms <= 0.0) return 0x7F;
        for (int r = 1; r <= 0x7F; ++r)
            if (ms > 47513.0 / RateDivisor(r - 1)) return std::max(r - 1, 0);
        return 0x7F;
    }
    int HoldFromMs(double ms)
    {
        // The driver divides as integers with `idiv`, which **truncates**
        // towards zero (SBAWE32.DRV: `idiv -92; add 0x7F`), so no rounding.
        // Example: hold 630 ms -> 630/92 = 6 -> 121 (0x79), not 120.
        const int steps = static_cast<int>(ms / (Emu8000::kHoldSecPerStep * 1000.0));
        return std::clamp(127 - steps, 0, 127);
    }
    // 1:1 transcription of the branch `C119C116` of `SBAWE.VXD` (object at
    // 0xC1196C74). The jump table of the conversion routine sends exactly
    // four delay generators there: delayModLFO (21), delayVibLFO (23),
    // delayModEnv (25) and delayVolEnv (33) - read from the index table at
    // 0xC119C362 and the address table at 0xC119C2FA.
    //
    //     cmp eax, 0xFFFFD120      ; <= -12000 -> 0x8000, no delay
    //     cmp eax, 0x156C          ; >= 5484   -> 0
    //     add eax, 0x30E4          ; + 12516
    //     shl eax, 0x10 / idiv 1200    ; x in 16.16, division **towards zero**
    //     and edi, 0xFFFF / add edi, 0x10000   ; 1 + fraction
    //     sar eax, 16 / sub cl, al / sar edi, cl
    //     sub esi, edi             ; 0x8000 - result
    //
    // Note: `2^x` here is **not an exponential** but a linear substitute
    // within the octave - `(1 + fraction) << integer part`. In the middle of
    // the octave it overestimates by up to 6 %.
    int DelayFromTimecents(int timecents)
    {
        if (timecents <= -12000)
            return static_cast<int>(Emu8000::kDelayNone);
        if (timecents >= 5484)
            return 0;

        const int q = static_cast<int>(
            (static_cast<int64_t>(timecents + 12516) << 16) / 1200);
        const int value = (0x10000 + (q & 0xFFFF)) >> (16 - (q >> 16));
        const int reg = static_cast<int>(Emu8000::kDelayNone) - value;
        return (reg >= 0) ? reg : 0;
    }

    // The delay register step is **725 microseconds**. Neither an estimate
    // nor a fit - `SFTYPE.H` of the AWE32 SDK says it directly for all four
    // delay fields:
    //
    //     short delayLfo1;   /* delay 0x8000-n*(725us) */
    //     short delayEnv1;   /* delay 0x8000 - n(725us) */
    //
    // SF1 (.SBK) has the times directly in milliseconds, so `n = ms / 0.725`.
    // It fits all three values measured in the driver traces:
    // 20 ms -> 27 steps, 140 ms -> 193, 440 ms -> 606.
    //
    // Beware of a dead end we already took once: the driver **does** have
    // an exponential conversion through timecents (`DelayFromTimecents`
    // below), but that one is for **SF2**. For SF1 it is not called at all -
    // verified with an instruction trace, 3 171 895 instructions in the
    // driver object and zero in its range. Both paths give almost the same
    // (1000/725 = 1.37931 against 2^(12516/1200)/1000 = 1.37957), they differ
    // only in rounding.
    int DelayFromMs(double ms)
    {
        const int steps = static_cast<int>(ms * 1000.0 / 725.0);
        return std::clamp(static_cast<int>(Emu8000::kDelayNone) - steps, 0, 0x8000);
    }

    // Transcription of `sub_192E` from `SBAWE.VXD` (obj 1, 0x192E) - cents ->
    // IP register:
    //
    //     esi = cents + 0x41A0        ; 16800, so that everything is positive
    //     edi = esi / 0x4B0           ; 1200 -> octave, clipped to 15
    //     edx = esi % 0x4B0           ; remainder in cents
    //     IP  = (edi << 12) | (edx*3 + (edx*31)/75)
    //
    // `3 + 31/75` is exactly `4096/1200`, so the formula itself has no
    // distortion. The difference against our earlier
    // `kPitchUnity + log2(...)*4096` came from **integer division**: the
    // driver computes in whole cents and truncates, while we carried the
    // whole chain in doubles. On Georgia that was +-1 on 67 notes.
    int PitchFromCents(double cents)
    {
        int v = static_cast<int>(std::lround(cents)) + 0x41A0;
        if (v < 0) v = 0;
        int oct = v / 1200;
        if (oct > 15) oct = 15;
        const int rem = v % 1200;
        return (oct << 12) | (rem * 3 + (rem * 31) / 75);
    }

    double TimecentsToMs(int tc) { return std::pow(2.0, tc / 1200.0) * 1000.0; }

    // Absolute cents (SF2) -> IFATN units (quarter semitones from 125 Hz).
    int FilterFcFromAbsCents(int cents)
    {
        // Must give the same register as the SF1 path, otherwise the same bank
        // sounds different in the two formats. Exactly that happened to us:
        // `SYNTHGM.SBK` gave IFATN `dc30` (cutoff 220) for the piano and
        // `SYNTHGM.SF2` `f542` (cutoff 245), because the step here was 25
        // cents instead of 29.3843 and the base 125 Hz instead of 101.81 Hz.
        // The SF1 value is the right one - it matches the real driver on 242
        // notes.
        return std::clamp(static_cast<int>(std::lround(
            (cents - Emu8000::kCutoffBaseCents) / Emu8000::kCutoffCentsStep)), 0, 255);
    }

    int8_t ClampS8(int v) { return static_cast<int8_t>(std::clamp(v, -128, 127)); }
    uint8_t ClampU8(int v) { return static_cast<uint8_t>(std::clamp(v, 0, 255)); }
}

// ===========================================================================
// GenSet
// ===========================================================================

void GenSet::AddFrom(const GenSet& other)
{
    for (int i = 0; i < Gen::Count; ++i)
        if (other.present[i])
        {
            if (present[i]) value[i] = static_cast<int16_t>(value[i] + other.value[i]);
            else            { value[i] = other.value[i]; present[i] = true; }
        }
}

void GenSet::FillFrom(const GenSet& other)
{
    for (int i = 0; i < Gen::Count; ++i)
        if (other.present[i] && !present[i])
        { value[i] = other.value[i]; present[i] = true; }
}

void GenSet::OverrideFrom(const GenSet& other)
{
    for (int i = 0; i < Gen::Count; ++i)
        if (other.present[i]) { value[i] = other.value[i]; present[i] = true; }
}

// ===========================================================================
// loading a bank
// ===========================================================================

// ===========================================================================
// GM bank compiled into SBAWE32.MDI
// ===========================================================================
//
// The DOS driver does not read SYNTHGM.SBK: the GM presets for the wave ROM
// are compiled into SBAWE32.MDI itself (Magic Carpet 2 loads only
// BULLFROG.SBK). Layout, found by comparison with SYNTHGM.SBK (2026-09-16,
// MC2 copy 36 880 B: presets @0x4BF0, igen @0x5C08, shdr @0x8686):
//
//   preset table  word pairs (id, pbag index), id = program | bank << 8,
//                 terminated by id 0xFF80 whose index = number of bags
//   pbag          word generator index per bag, +1 end entry = count
//   pgen          3 B per generator: op, amount16
//   (word alignment)
//   inst          word ibag index per instrument, +1 end entry
//   ibag          word generator index per bag, +1 end entry
//   igen          3 B per generator
//   (word alignment)
//   shdr          SoundFont 1.0 sample headers, 16 B each
//
// Part of the generators is stored already converted to driver/register
// units. Per op the SBK -> MDI mapping over all 129 presets is a function
// (mdi_units.py): x2 for 8/10/11/13/22/24, >>3 for 9, register delay for
// 21/23/25/33, attack rate for 26/34, hold for 27, decay rate for 28/30/38,
// sustain *4/3 for 29/37; the rest (including 35/36, which the driver key
// scales in ms at note-on) is stored as in the SBK. A few values are real
// data differences from SYNTHGM.SBK (e.g. decayVolEnv 2200 -> 1080/1430,
// some key ranges, one sample end address).
//
// The loader rebuilds an SF1 bank: each converted value is replaced by the
// smallest SF1 value that the conversions in MakeVoiceParams map to exactly
// that register value, so the registers equal the driver's tables and the
// rest of the pipeline stays the SF1 path.
namespace
{
    uint16_t Rd16(const std::vector<uint8_t>& d, size_t p)
    {
        return (p + 1 < d.size()) ? static_cast<uint16_t>(d[p] | (d[p + 1] << 8)) : 0;
    }

    void Wr16(std::vector<uint8_t>& o, uint16_t v) { o.push_back(v & 0xFF); o.push_back(v >> 8); }
    void Wr32(std::vector<uint8_t>& o, uint32_t v) { Wr16(o, v & 0xFFFF); Wr16(o, v >> 16); }

    void WrChunk(std::vector<uint8_t>& o, const char* id, const std::vector<uint8_t>& body)
    {
        o.insert(o.end(), id, id + 4);
        Wr32(o, static_cast<uint32_t>(body.size()));
        o.insert(o.end(), body.begin(), body.end());
        if (body.size() & 1) o.push_back(0);
    }

    void WrList(std::vector<uint8_t>& o, const char* type, const std::vector<uint8_t>& body)
    {
        std::vector<uint8_t> b(type, type + 4);
        b.insert(b.end(), body.begin(), body.end());
        WrChunk(o, "LIST", b);
    }

    // Smallest SF1 value v in [lo, hi] with f(v) == target; returns false when
    // the conversion cannot produce the target. The inverse of each
    // conversion is tabulated once (keyed by the conversion's address).
    template <typename F>
    bool Preimage(int target, int lo, int hi, F f, int& out)
    {
        static std::map<std::pair<int, int>, std::map<int, int>> cache;
        const int tag = static_cast<int>(typeid(F).hash_code() & 0x7FFFFFFF);
        auto& inv = cache[{tag, lo}];
        if (inv.empty())
            for (int v = hi; v >= lo; --v)
                inv[f(v)] = v;                   // descending: smallest v wins
        const auto it = inv.find(target);
        if (it == inv.end()) return false;
        out = it->second;
        return true;
    }

    // Converts one generator from driver units back to an SF1 value.
    // Returns the number of values that had no exact preimage (0 or 1).
    int MdiToSf1(int op, uint16_t raw, int16_t& out)
    {
        const int s = static_cast<int16_t>(raw);
        int v = s;
        bool ok = true;
        switch (op)
        {
        case Gen::InitialFilterFc:                 // cutoff = v * 2
            ok = Preimage(raw, 0, 127, [](int x) { return std::clamp(x * 2, 0, 255); }, v);
            break;
        case Gen::ModLfoToFilterFc: case Gen::ModEnvToFilterFc: case Gen::ModLfoToVolume:
            ok = Preimage(s, -128, 127, [](int x) { return static_cast<int>(ClampS8(x * 2)); }, v);
            break;
        case Gen::FreqModLFO: case Gen::FreqVibLFO: // (v * 2) & 0xFF
            ok = Preimage(raw & 0xFF, 0, 255, [](int x) { return (x * 2) & 0xFF; }, v);
            break;
        case Gen::InitialFilterQ:                  // q = v >> 3
            ok = Preimage(raw, 0, 127, [](int x) { return x >> 3; }, v);
            break;
        case Gen::DelayModLFO: case Gen::DelayVibLFO: case Gen::DelayModEnv: case Gen::DelayVolEnv:
            ok = Preimage(raw, 0, 65535, [](int x) { return DelayFromMs(x); }, v);
            break;
        case Gen::AttackModEnv: case Gen::AttackVolEnv:
            ok = Preimage(raw, 0, 65535, [](int x) { return AttackRateFromMs(x); }, v);
            break;
        case Gen::HoldModEnv:
            ok = Preimage(raw, 0, 65535, [](int x) { return HoldFromMs(x); }, v);
            break;
        case Gen::DecayModEnv: case Gen::ReleaseModEnv: case Gen::ReleaseVolEnv:
            ok = Preimage(raw, 0, 65535, [](int x) { return DecayRateFromMs(x); }, v);
            break;
        case Gen::SustainModEnv: case Gen::SustainVolEnv:
            ok = Preimage(raw, 0, 255, [](int x) { return std::clamp(x * 4 / 3, 0, 0x7F); }, v);
            break;
        default:
            break;
        }
        out = static_cast<int16_t>(v);
        return ok ? 0 : 1;
    }

    // Builds an SF1 RIFF image from the tables in SBAWE32.MDI.
    bool BuildSbkFromMdi(const std::vector<uint8_t>& d, std::vector<uint8_t>& riff,
                         std::string& error, int& unmapped)
    {
        // Preset table: find the 0xFF80 terminator preceded by pairs with
        // increasing bag indices.
        size_t ptab = 0, pend = 0;
        for (size_t t = 4; t + 4 <= d.size() && !pend; t += 1)
        {
            if (Rd16(d, t) != 0xFF80) continue;
            size_t p = t;
            int n = 0;
            // id high byte is the MIDI bank (0 or 0x80 for the drum kit),
            // low byte a program < 128
            while (p >= 4 && ((Rd16(d, p - 4) >> 8) == 0 || (Rd16(d, p - 4) >> 8) == 0x80)
                   && (Rd16(d, p - 4) & 0xFF) < 128 && Rd16(d, p - 2) < Rd16(d, p + 2))
            {
                p -= 4;
                ++n;
            }
            if (n > 50) { ptab = p; pend = t; }
        }
        if (!pend) { error = _("SBAWE32.MDI: preset table not found"); return false; }

        const size_t nPresets = (pend - ptab) / 4;
        const uint16_t nPbag = Rd16(d, pend + 2);
        size_t pos = pend + 4;
        std::vector<uint16_t> pbag(nPbag + 1u);
        for (auto& x : pbag) { x = Rd16(d, pos); pos += 2; }
        const size_t pgenAt = pos;
        const size_t nPgen = pbag.back();
        pos = pgenAt + 3 * nPgen;
        pos += pos & 1;
        std::vector<uint16_t> inst;
        inst.push_back(Rd16(d, pos));
        pos += 2;
        while (pos + 2 <= d.size() && Rd16(d, pos) >= inst.back())
        {
            inst.push_back(Rd16(d, pos));
            pos += 2;
        }
        std::vector<uint16_t> ibag(inst.back() + 1u);
        for (auto& x : ibag) { x = Rd16(d, pos); pos += 2; }
        const size_t igenAt = pos;
        const size_t nIgen = ibag.back();
        size_t shdrAt = igenAt + 3 * nIgen;
        shdrAt += shdrAt & 1;
        if (shdrAt > d.size()) { error = _("SBAWE32.MDI: tables exceed the file"); return false; }

        auto gens = [&](size_t at, size_t count, int& maxSample)
        {
            std::vector<uint8_t> o;
            for (size_t i = 0; i < count; ++i)
            {
                const int op = d[at + 3 * i];
                const uint16_t raw = Rd16(d, at + 3 * i + 1);
                int16_t v = static_cast<int16_t>(raw);
                if (op != Gen::KeyRange && op != Gen::VelRange)
                    unmapped += MdiToSf1(op, raw, v);
                if (op == Gen::SampleID) maxSample = std::max(maxSample, static_cast<int>(raw));
                Wr16(o, static_cast<uint16_t>(op));
                Wr16(o, static_cast<uint16_t>(v));
            }
            Wr32(o, 0);                              // terminal record
            return o;
        };
        int maxSample = -1, dummy = -1;
        const std::vector<uint8_t> pgenB = gens(pgenAt, nPgen, dummy);
        const std::vector<uint8_t> igenB = gens(igenAt, nIgen, maxSample);

        const size_t nSamples = static_cast<size_t>(maxSample + 1);
        if (shdrAt + 16 * nSamples > d.size()) { error = _("SBAWE32.MDI: sample table truncated"); return false; }

        std::vector<uint8_t> phdrB, pbagB, instB, ibagB, shdrB, snamB, pmodB(10, 0), imodB(10, 0);
        auto name20 = [](std::vector<uint8_t>& o, const std::string& s)
        {
            for (size_t i = 0; i < 20; ++i) o.push_back(i < s.size() ? static_cast<uint8_t>(s[i]) : 0);
        };
        for (size_t k = 0; k <= nPresets; ++k)
        {
            const uint16_t id = (k < nPresets) ? Rd16(d, ptab + 4 * k) : 0;
            const uint16_t bag = (k < nPresets) ? Rd16(d, ptab + 4 * k + 2) : nPbag;
            name20(phdrB, (k < nPresets) ? ("MDI " + std::to_string(id >> 8) + ":" + std::to_string(id & 0xFF)) : "EOP");
            Wr16(phdrB, id & 0xFF);
            Wr16(phdrB, id >> 8);
            Wr16(phdrB, bag);
            Wr32(phdrB, 0); Wr32(phdrB, 0); Wr32(phdrB, 0);
        }
        for (uint16_t x : pbag) { Wr16(pbagB, x); Wr16(pbagB, 0); }
        for (size_t i = 0; i < inst.size(); ++i)
        {
            name20(instB, (i + 1 < inst.size()) ? ("MDI inst " + std::to_string(i)) : "EOI");
            Wr16(instB, inst[i]);
        }
        for (uint16_t x : ibag) { Wr16(ibagB, x); Wr16(ibagB, 0); }
        shdrB.assign(d.begin() + shdrAt, d.begin() + shdrAt + 16 * nSamples);
        for (size_t i = 0; i < nSamples; ++i) name20(snamB, "ROM " + std::to_string(i));

        std::vector<uint8_t> info, ifil, pdta, sdta;
        Wr16(ifil, 1); Wr16(ifil, 0);
        WrChunk(info, "ifil", ifil);
        const std::string nm = "SBAWE32.MDI GM";
        WrChunk(info, "INAM", std::vector<uint8_t>(nm.c_str(), nm.c_str() + nm.size() + 1));
        std::vector<uint8_t> sdtaBody;
        WrChunk(sdtaBody, "snam", snamB);
        WrChunk(pdta, "phdr", phdrB); WrChunk(pdta, "pbag", pbagB); WrChunk(pdta, "pmod", pmodB);
        WrChunk(pdta, "pgen", pgenB); WrChunk(pdta, "inst", instB); WrChunk(pdta, "ibag", ibagB);
        WrChunk(pdta, "imod", imodB); WrChunk(pdta, "igen", igenB); WrChunk(pdta, "shdr", shdrB);

        std::vector<uint8_t> body{'s', 'f', 'b', 'k'};
        WrList(body, "INFO", info);
        WrList(body, "sdta", sdtaBody);
        WrList(body, "pdta", pdta);
        riff.clear();
        riff.insert(riff.end(), {'R', 'I', 'F', 'F'});
        Wr32(riff, static_cast<uint32_t>(body.size()));
        riff.insert(riff.end(), body.begin(), body.end());
        return true;
    }
}

Bank Load(const std::string& path)
{
    Bank bank;

    std::ifstream file(path, std::ios::binary);
    if (!file) { bank.errorMessage = StrFormat(_("Cannot open file: %s"), path.c_str()); return bank; }

    std::vector<uint8_t> buf((std::istreambuf_iterator<char>(file)),
                              std::istreambuf_iterator<char>());
    return LoadData(std::move(buf), path);
}

Bank LoadData(std::vector<uint8_t> buf, const std::string& name)
{
    Bank bank;

    if (buf.size() >= 7 && std::memcmp(buf.data(), "AIL3MDI", 7) == 0)
    {
        std::vector<uint8_t> riff;
        int unmapped = 0;
        if (!BuildSbkFromMdi(buf, riff, bank.errorMessage, unmapped)) return bank;
        if (unmapped)
            std::fprintf(stderr, _("Warning: %s: %d MDI values without an exact SF1 counterpart\n"),
                         name.c_str(), unmapped);
        buf.swap(riff);
    }
    if (buf.size() < 12 || std::memcmp(buf.data(), "RIFF", 4) != 0)
    {
        bank.errorMessage = _("Missing RIFF header");
        return bank;
    }

    std::map<std::string, Chunk> c;
    const size_t riffEnd = std::min<size_t>(buf.size(), 8 + RdU32(&buf[4]));
    WalkChunks(buf, 12, riffEnd, c);

    auto need = [&](const char* id) -> const Chunk*
    {
        auto it = c.find(id);
        return (it != c.end() && it->second.found) ? &it->second : nullptr;
    };

    const Chunk* ifil = need("ifil");
    if (!ifil || ifil->size < 2) { bank.errorMessage = _("Missing chunk ifil"); return bank; }
    const uint16_t major = RdU16(&buf[ifil->offset]);
    bank.version = (major < 2) ? Version::Sf1 : Version::Sf2;

    if (const Chunk* n = need("INAM")) bank.name = CStr(&buf[n->offset], n->size);
    if (const Chunk* r = need("irom")) bank.romName = CStr(&buf[r->offset], r->size);

    // ---- sample data ----
    if (const Chunk* smpl = need("smpl"))
    {
        bank.sampleData.resize(smpl->size / 2);
        std::memcpy(bank.sampleData.data(), &buf[smpl->offset], bank.sampleData.size() * 2);
    }

    // ---- sample headers ----
    const Chunk* shdr = need("shdr");
    if (!shdr) { bank.errorMessage = _("Missing chunk shdr"); return bank; }

    if (bank.version == Version::Sf1)
    {
        // SF1: 16 B per record, names in a separate chunk snam (20 B).
        const Chunk* snam = need("snam");
        const uint32_t count = shdr->size / 16;
        bank.samples.reserve(count);

        // Which samples lie in the wave ROM. The SF1 sample header has no
        // flag for it. An asterisk in the name (`*BellTree`) is only a habit
        // of banks saved through SFSTORE.DLL (BULLFROG.SBK) - Creative does
        // not mark its banks that way: SYNTHGS.SBK and SYNTHMT.SBK start with
        // 153 ROM samples without an asterisk (exactly the ROM table of
        // SYNTHGM.SBK, start and loop addresses match) and 71 of them have an
        // address smaller than the length of `smpl`, so they cannot be told
        // by the address either. Formerly they were therefore played from
        // DRAM at nonsense addresses.
        //
        // The layout does hold, though: the ROM samples always come first and
        // the bank's own samples after them start at address 0. Verified on
        // all 44 SBKs we have (2026-09-13). If no sample starts at 0, the
        // asterisk rule stays.
        uint32_t firstOwn = count;
        for (uint32_t i = 0; i < count; ++i)
            if (RdU32(&buf[shdr->offset + i * 16]) == 0) { firstOwn = i; break; }
        const bool byPosition = firstOwn < count;
        for (uint32_t i = 0; i < count; ++i)
        {
            const uint8_t* p = &buf[shdr->offset + i * 16];
            Sample s;
            s.start     = RdU32(p);
            s.end       = RdU32(p + 4);
            s.loopStart = RdU32(p + 8);
            s.loopEnd   = RdU32(p + 12);
            if (snam && (i + 1) * 20 <= snam->size)
                s.name = CStr(&buf[snam->offset + i * 20], 20);
            // A bank without any `smpl` chunk (e.g. SYNTHGM.SBK - the E-mu
            // description of the GM bank) describes only the ROM contents, so
            // all its samples are in ROM. Otherwise the position before the
            // bank's own samples decides.
            s.inRom = bank.sampleData.empty()
                   || (byPosition ? (i < firstOwn)
                                  : (!s.name.empty() && s.name[0] == '*'));
            // The SF1 sample header has neither a sample rate nor a root
            // note - the EMU8000 runs natively at 44100 Hz and the root note
            // comes from the generators (OverridingRootKey /
            // Sf1RootPitchCents).
            s.sampleRate = 44100;   // EMU8000 native clock
            s.originalKey = 60;
            bank.samples.push_back(std::move(s));
        }
    }
    else
    {
        const uint32_t count = shdr->size / 46;
        bank.samples.reserve(count);
        for (uint32_t i = 0; i < count; ++i)
        {
            const uint8_t* p = &buf[shdr->offset + i * 46];
            Sample s;
            s.name       = CStr(p, 20);
            s.start      = RdU32(p + 20);
            s.end        = RdU32(p + 24);
            s.loopStart  = RdU32(p + 28);
            s.loopEnd    = RdU32(p + 32);
            s.sampleRate = RdU32(p + 36);
            s.originalKey = p[40];
            s.correction  = static_cast<int8_t>(p[41]);
            const uint16_t type = RdU16(p + 44);
            s.inRom = (type & 0x8000) != 0 || (!s.name.empty() && s.name[0] == '*');
            if (s.sampleRate == 0) s.sampleRate = 44100;
            bank.samples.push_back(std::move(s));
        }
        if (!bank.samples.empty() && bank.samples.back().name == "EOS")
            bank.samples.pop_back();
    }

    // ---- bag / gen / instruments / presets ----
    const Chunk* pbag = need("pbag"); const Chunk* pgen = need("pgen");
    const Chunk* inst = need("inst"); const Chunk* ibag = need("ibag");
    const Chunk* igen = need("igen"); const Chunk* phdr = need("phdr");
    if (!pbag || !pgen || !inst || !ibag || !igen || !phdr)
    {
        bank.errorMessage = _("One of the chunks phdr/pbag/pgen/inst/ibag/igen is missing");
        return bank;
    }

    auto genAt = [&](const Chunk* ch, uint32_t i, int& op, int16_t& val)
    {
        const uint8_t* p = &buf[ch->offset + i * 4];
        op = RdU16(p);
        val = static_cast<int16_t>(RdU16(p + 2));
    };
    auto bagGenIndex = [&](const Chunk* ch, uint32_t i) -> uint32_t
    {
        return RdU16(&buf[ch->offset + i * 4]);
    };

    // Splits the zones of a bag into "global" (without terminator) and normal.
    auto readZones = [&](const Chunk* bagCh, const Chunk* genCh,
                          uint32_t bagFirst, uint32_t bagLast,
                          int terminator, GenSet& global, std::vector<Zone>& zones)
    {
        const uint32_t nGen = genCh->size / 4;
        for (uint32_t b = bagFirst; b < bagLast; ++b)
        {
            const uint32_t g0 = bagGenIndex(bagCh, b);
            const uint32_t g1 = bagGenIndex(bagCh, b + 1);
            Zone z;
            bool hasTerminator = false;
            for (uint32_t g = g0; g < g1 && g < nGen; ++g)
            {
                int op; int16_t val;
                genAt(genCh, g, op, val);
                if (op == Gen::KeyRange)
                {
                    const uint16_t raw = static_cast<uint16_t>(val);
                    z.keyLo = raw & 0xFF; z.keyHi = (raw >> 8) & 0xFF;
                }
                else if (op == Gen::VelRange)
                {
                    const uint16_t raw = static_cast<uint16_t>(val);
                    z.velLo = raw & 0xFF; z.velHi = (raw >> 8) & 0xFF;
                }
                else if (op == terminator)
                {
                    hasTerminator = true;
                    if (terminator == Gen::SampleID) z.sampleId = static_cast<uint16_t>(val);
                    else                             z.instrument = static_cast<uint16_t>(val);
                }
                z.gen.Set(op, val);
            }
            // A zone without terminator in the first place = the global zone.
            if (!hasTerminator)
            {
                if (b == bagFirst) global = z.gen;
                continue;
            }
            zones.push_back(std::move(z));
        }
    };

    // instruments
    const uint32_t nInst = inst->size / 22;
    for (uint32_t i = 0; i + 1 < nInst; ++i)
    {
        const uint8_t* p = &buf[inst->offset + i * 22];
        Instrument in;
        in.name = CStr(p, 20);
        const uint32_t b0 = RdU16(p + 20);
        const uint32_t b1 = RdU16(p + 22 + 20);
        readZones(ibag, igen, b0, b1, Gen::SampleID, in.global, in.zones);
        bank.instruments.push_back(std::move(in));
    }

    // presets
    const uint32_t nPreset = phdr->size / 38;
    for (uint32_t i = 0; i + 1 < nPreset; ++i)
    {
        const uint8_t* p = &buf[phdr->offset + i * 38];
        Preset pr;
        pr.name    = CStr(p, 20);
        pr.program = RdU16(p + 20);
        pr.bank    = RdU16(p + 22);
        const uint32_t b0 = RdU16(p + 24);
        const uint32_t b1 = RdU16(p + 38 + 24);
        readZones(pbag, pgen, b0, b1, Gen::Instrument, pr.global, pr.zones);
        bank.presets.push_back(std::move(pr));
    }

    bank.valid = true;
    return bank;
}

// ===========================================================================
// zone selection
// ===========================================================================

const Preset* Bank::FindPreset(int bankNum, int program) const
{
    for (const Preset& p : presets)
        if (p.bank == bankNum && p.program == program) return &p;
    return nullptr;
}

std::vector<Region> Bank::Select(int bankNum, int program, int key, int velocity) const
{
    std::vector<Region> out;

    // No fallback to bank 0 - the caller takes care of that only after it
    // asked ALL loaded banks for the exact bank. Otherwise a user bank with
    // preset 0 would override, say, the GM drums in bank 128.
    const Preset* preset = FindPreset(bankNum, program);
    if (!preset) return out;

    for (const Zone& pz : preset->zones)
    {
        if (key < pz.keyLo || key > pz.keyHi) continue;
        if (velocity < pz.velLo || velocity > pz.velHi) continue;
        if (pz.instrument < 0 || pz.instrument >= static_cast<int>(instruments.size())) continue;

        const Instrument& in = instruments[pz.instrument];
        for (const Zone& iz : in.zones)
        {
            if (key < iz.keyLo || key > iz.keyHi) continue;
            if (velocity < iz.velLo || velocity > iz.velHi) continue;
            if (iz.sampleId < 0 || iz.sampleId >= static_cast<int>(samples.size())) continue;

            Region r;
            r.sample = &samples[iz.sampleId];

            // Combining order per the specification: the global instrument
            // zone, then the instrument zone (both absolute), and finally the
            // preset zones as offsets.
            r.gen = in.global;
            r.gen.OverrideFrom(iz.gen);
            GenSet presetGen = preset->global;
            presetGen.OverrideFrom(pz.gen);

            // The SF1 attenuation must be summed only in register units,
            // otherwise `AddFrom` would add the raw values (e.g. drum zone
            // 121 + preset 127 = 248) and `127 - 248` would drop to zero.
            // Measured on Georgia: on drums the driver has exactly
            // `127 - zone atten` more than we had (zone 121 -> +6, zone 112
            // -> +15, ...).
            if (version == Version::Sf1)
            {
                int units = 0;
                bool any = false;
                if (r.gen.Has(Gen::InitialAttenuation))
                { units += 127 - r.gen.value[Gen::InitialAttenuation]; any = true; }
                if (presetGen.Has(Gen::InitialAttenuation))
                { units += 127 - presetGen.value[Gen::InitialAttenuation]; any = true; }
                r.sf1AttenUnits = any ? units : -1;
            }

            // The preset zone only **fills in** what the instrument zone
            // lacks - it is not added. Formerly we added per the SF2
            // specification, but the driver does not do that.
            //
            // Measured on CRAZY: channel 4 plays program 97 (SeaShore ->
            // Soundtrack), which has `coarseTune` on both levels - 1 on the
            // preset and 3 on the instrument. Adding gave us 4, the driver
            // uses 3 and plays a semitone lower; all 58 notes of that
            // channel were off. It showed because two layers of the same
            // tone had offsets of -341 and -342, i.e. unequal - the
            // signature of an offset in **cents before the conversion**,
            // not of a constant in IP units.
            //
            // After the change CRAZY matches 32/32 and nothing else got
            // worse: Georgia, JUMP, RELAX and MINUET stay at 32/32, Magic
            // Carpet 2 at 24/24 - over 22 000 notes in total.
            //
            // The attenuation is an exception, handled separately above,
            // because the driver adds it only in register units.
            r.gen.FillFrom(presetGen);

            out.push_back(std::move(r));
        }
    }
    return out;
}

// ===========================================================================
// conversion to registers
// ===========================================================================

VoiceParams MakeVoiceParams(const Bank& bank, const Region& region,
                            int key, int velocity,
                            uint32_t dramBase, uint32_t romPoolBase,
                            Awe32::Driver drv)
{
    using namespace Emu8000;
    VoiceParams vp;
    const GenSet& g = region.gen;
    const Sample& s = *region.sample;
    const bool sf1 = (bank.version == Version::Sf1);

    // ---- addresses -------------------------------------------------------
    // A sample lies in ROM either because the bank marks it so (asterisk in
    // the name / flag in shdr) or because the whole bank describes the ROM.
    const bool inRom = s.inRom || bank.samplesInRom;
    const uint32_t base = inRom ? romPoolBase : dramBase;
    uint32_t start, loopStart, loopEnd;
    if (sf1)
    {
        // SF1 stores ready-made addresses for the chip (including the
        // interpolator correction) - ROM samples use them directly, the
        // bank's own samples are only moved to where the bank was loaded.
        const uint32_t off = inRom ? 0 : dramBase;
        // The zone offsets must be added here too. That was missing and it
        // showed on the sample `organwave` in the preset Organ 3, whose zone
        // has both `startloopAddrsOffset -1` and `endloopAddrsOffset -1`: the
        // driver wrote PSST F146 and CSL F17D, we F147 and F17E (232 notes of
        // Georgia).
        start     = s.start + off
                  + g.Get(Gen::StartAddrsOffset, 0)
                  + 32768u * g.Get(Gen::StartAddrsCoarseOffset, 0);
        loopStart = s.loopStart + off
                  + g.Get(Gen::StartloopAddrsOffset, 0)
                  + 32768u * g.Get(Gen::StartloopAddrsCoarseOffset, 0);
        loopEnd   = s.loopEnd + off
                  + g.Get(Gen::EndloopAddrsOffset, 0)
                  + 32768u * g.Get(Gen::EndloopAddrsCoarseOffset, 0);
    }
    else
    {
        // SF2 stores indices; the correction -1/-2/-3 matches how Creative
        // addresses the same samples in its own SF1 bank (see docs/re-notes).
        start     = base + s.start + g.Get(Gen::StartAddrsOffset, 0)
                    + 32768u * g.Get(Gen::StartAddrsCoarseOffset, 0) - 1;
        loopStart = base + s.loopStart + g.Get(Gen::StartloopAddrsOffset, 0)
                    + 32768u * g.Get(Gen::StartloopAddrsCoarseOffset, 0) - 2;
        loopEnd   = base + s.loopEnd + g.Get(Gen::EndloopAddrsOffset, 0)
                    + 32768u * g.Get(Gen::EndloopAddrsCoarseOffset, 0) - 3;
    }

    // The default is **0** = a one-shot sample, not a loop. Not a guess:
    // both drivers have a table of generator defaults which is copied into
    // the layer's parameter block before the bank is applied, and in both
    // there is a zero at the position of generator 54.
    //   SBAWE32.MDI 0x16AD (0x43 words, copied to 0x1CD2)
    //   SBAWE.VXD   obj 1, 0x6D60 (the same table, the same values)
    // It shows on the Magic Carpet 2 bank: the presets LOOP2 and LOOP3 have
    // no sampleModes and the driver really puts their loop past the sample.
    vp.sampleEndAddr = (sf1 && inRom) ? s.end : (base + s.end);
    vp.loopEndAddr   = loopEnd;
    const int sampleModes = g.Get(Gen::SampleModes, 0);
    vp.looping = (sampleModes & 1) != 0;
    if (!vp.looping)
    {
        // The EMU8000 can only loop, it has no "one-shot" mode. The driver
        // solves it by putting the loop into the silence PAST the sample -
        // the format appends 46 zero samples after every sample (in
        // 1mgm.sf2 the gap between the end of one and the start of the next
        // is exactly 46).
        //
        // The specific offsets +4 and +8 are transcribed from SBAWE32.DRV
        // (0x02E4):
        //     loopStart = end + 4;  loopEnd = end + 8;
        const uint32_t end = (sf1 && inRom) ? s.end : (base + s.end);
        loopStart = end + 4;
        loopEnd   = end + 8;
    }

    // ---- Q + address -> CCCA --------------------------------------------
    int q;
    // SF1 has `initialFilterQ` 0..127, the register 0..15. Measured on
    // Georgia against `SBAWE.VXD` (3331 notes, three different values in
    // `SYNTHGM.SBK`):
    //
    //     SF1 12 -> 1,  SF1 50 -> 6,  SF1 79 -> 9
    //
    // The original `v * 15 / 127` with truncation gave 5 for 50 and
    // diverged on 669 notes (preset "Piano 2"). A shift by three bits fits
    // all three points and is also what a 16-bit driver would most likely
    // do (`shr ax, 3`).
    //
    // Note: `lround(v * 15 / 127.0)` fits the same three points too. A note
    // with `initialFilterQ` 6, 14 or 22 would tell them apart - there the
    // two variants differ. None of our traces has one yet.
    if (sf1) q = g.Get(Gen::InitialFilterQ, 0) >> 3;
    else     q = static_cast<int>(std::lround(g.Get(Gen::InitialFilterQ, 0) / 10.0
                                              / kResonanceMaxDb * kCccaQMax));
    q = std::clamp(q, 0, kCccaQMax);
    // CCCA gets the start address reduced by a constant which **differs**
    // between the two driver families - see Awe32::StartAddressOffset().
    // The raw address is therefore carried separately and combined only in
    // Synth.
    vp.sampleStart = start;
    vp.ccca = (static_cast<uint32_t>(q) << kCccaQShift)
            | ((start - Awe32::StartAddressOffset(Awe32::kDefaultDriver))
               & kCccaAddressMask);

    // ---- pan + loop start -> PSST ---------------------------------------
    // The patch pan is kept in driver units (0..127, 64 = centre, default
    // from the table of generator defaults). It is converted to the register
    // only in Synth, because it is added to CC10 there. The register form
    // below is only for the substitute voice and for the dump.
    if (sf1)
        vp.patchPan = g.Get(Gen::Pan, 64);
    else
        // SF2: -500 = fully left, +500 = fully right; the EMU8000 is reversed.
        vp.patchPan = std::clamp(64 + static_cast<int>(
            std::lround(g.Get(Gen::Pan, 0) * 127.0 / 1000.0)), 0, 127);
    const int pan = 0x17F - 2 * (vp.patchPan + 64);
    vp.psst = (static_cast<uint32_t>(ClampU8(pan)) << kPanShift)
            | (loopStart & kLoopAddressMask);

    // ---- chorus send + loop end -> CSL ----------------------------------
    const int chorus = sf1 ? g.Get(Gen::ChorusEffectsSend, 0)
                           : static_cast<int>(std::lround(g.Get(Gen::ChorusEffectsSend, 0) * 255.0 / 1000.0));
    // The driver adds 1 to the loop end - but **only in the loop branch**.
    // For a one-shot sample it puts the loop at `end+4 .. end+8` and adds no
    // one:
    //   SBAWE32.MDI 0x2019 (loop: `add ax, 1`) vs 0x208B (one-shot: `add ax, 8`)
    //   SBAWE.VXD   obj 1, 0x1EE7 `inc eax` is also only in the loop branch
    // Measured on 52 drum notes of Magic Carpet 2, where our CSL came out
    // exactly 1 higher than the driver's.
    vp.csl = (static_cast<uint32_t>(ClampU8(chorus)) << kChorusShift)
           | ((loopEnd + (vp.looping ? 1u : 0u)) & kLoopAddressMask);

    // The driver's default reverb send is 28: SYNTHGM.SBK has no
    // reverbEffectsSend and MINUET sends no CC91, yet the driver writes 0x1C
    // to the high byte of the low word of PTRX on all 242 notes.
    vp.reverbSend = ClampU8(sf1 ? g.Get(Gen::ReverbEffectsSend, 28)
                                : static_cast<int>(std::lround(g.Get(Gen::ReverbEffectsSend, 0) * 255.0 / 1000.0)));

    // ---- pitch -> IP -----------------------------------------------------
    int rootKey = g.Get(Gen::OverridingRootKey, -1);
    if (rootKey < 0) rootKey = s.originalKey;
    double cents = 0.0;
    if (sf1 && g.Has(Gen::Sf1RootPitchCents))
        cents = -(g.value[Gen::Sf1RootPitchCents] - rootKey * 100.0);
    cents += s.correction;
    cents += g.Get(Gen::CoarseTune, 0) * 100.0;
    cents += g.Get(Gen::FineTune, 0);

    // `scaleTuning` is **not in percent** as in SF2. The driver only tests
    // for equality with one and then **halves** the whole result -
    // transcribed from `SBAWE.VXD` (object at 0xC0FF7BE0),
    // C0FFAF54..C0FFAF87:
    //
    //     ecx = keynum - rootKey + coarseTune
    //     ecx = (ecx + 60) * 100 - samplePitch + fineTune
    //     cmp word [esi+0x70], 1        ; scaleTuning
    //     jne next
    //         eax = ecx; cdq; sub eax,edx; sar eax,1   ; divide by 2 towards zero
    //
    // The field names are from `SFTYPE.H` of the AWE32 SDK (0x6E
    // samplePitch, 0x70 scaleTuning, 0x74 rootKey).
    //
    // Measured: preset 122 SeaShore in SYNTHGM.SBK has `scaleTuning 1`, and
    // those were the last four mismatching notes of RELAX. For note 69 with
    // samplePitch 8781 it gives (69-60+60)*100 - 8781 = -1881, half is
    // -940 - exactly what the driver wrote.
    double pitchCents = (key - rootKey) * 100.0 + cents;
    if (g.Get(Gen::ScaleTuning, 100) == 1)
        pitchCents = static_cast<int>(pitchCents) / 2;

    // The sample rate is converted to cents separately - the driver has this
    // component already baked into `gen55`, we keep it in the sample
    // header.
    const double centsTotal = pitchCents
        + std::log2(s.sampleRate / static_cast<double>(44100)) * 1200.0;
    vp.ipCents = static_cast<int>(centsTotal);
    vp.ip = static_cast<uint16_t>(std::clamp(PitchFromCents(centsTotal), 0, 65535));

    // ---- patch attenuation and filter -> IFATN ----------------------------
    // The attenuation is assembled only in the Synth layer by the formula
    // transcribed from the driver (see Awe32Curves.h) - here we only convert
    // the patch attenuation to register units (0.375 dB per unit, 0 = none).
    if (sf1)
    {
        // SF1: 0..127, where 127 = no attenuation. The driver computes
        // 0x7F - v and adds the result directly in register units.
        // The default **differs** between the families: in the table of
        // generator defaults SBAWE32.MDI (0x16AD+0x60) has 110, while
        // SBAWE.VXD (obj 1, 0x6D60+0x60) has 127.
        const int dflt = (Awe32::IsDosLike(drv)) ? 110 : 127;
        const int units = (region.sf1AttenUnits >= 0) ? region.sf1AttenUnits
                                                      : (127 - dflt);
        vp.patchAttenUnits = static_cast<uint8_t>(std::clamp(units, 0, 255));
    }
    else
    {
        // SF2: centibels (0.1 dB) -> units of 0.375 dB.
        vp.patchAttenUnits = static_cast<uint8_t>(std::clamp(
            static_cast<int>(std::lround(g.Get(Gen::InitialAttenuation, 0)
                                         / 10.0 / kAttenDbPerStep)), 0, 255));
    }

    // SF1 stores initialFilterFc as 0..127, the IFATN register has an
    // 8-bit cutoff. The conversion is **plain doubling**; if the generator is
    // missing, the default 255 from the driver table applies (MDI 0x16AD,
    // VXD 0x6D60).
    //
    // Measured on Georgia against `SBAWE.VXD`:
    //
    //     SF1 52 -> 104,  SF1 97 -> 194,  SF1 127 -> 254,  missing -> 255
    //
    // Formerly `v * 255 / 127` was computed here, so that 127 gave 255. That
    // fix was grafted onto a wrong measurement: preset 52 'Choir Aahs' of
    // Magic Carpet 2, where the driver wrote cutoff 255, **has no
    // `initialFilterFc`** - so it was the default value, not a conversion of
    // the number 127. A real 127 appeared only with the preset "Piano 2" on
    // Georgia and gave 254.
    int cutoff;
    if (!g.Has(Gen::InitialFilterFc)) cutoff = 255;
    else if (sf1)                     cutoff = std::clamp<int>(
                                          g.value[Gen::InitialFilterFc] * 2, 0, 255);
    else                              cutoff = FilterFcFromAbsCents(g.value[Gen::InitialFilterFc]);

    // Synth fills in the low byte (attenuation) from the driver curves.
    vp.ifatn = static_cast<uint16_t>(cutoff << 8);

    // ---- modulation ------------------------------------------------------
    // `sf1Scale` is the multiplier for SF1: some generators have a 7-bit
    // range in SBK, while the register is 8-bit, so the driver doubles the
    // value. Measured on Georgia against `SBAWE.VXD`, 3331 notes, **without
    // a single exception**:
    //
    //     modEnvToFilterFc  3F -> 7E,  01 -> 02   (1410 notes)
    //     modLfoToFilterFc  08 -> 10              (478 notes)
    //     modLfoToVolume    23 -> 46              (712 notes)
    //     freqModLFO        12 -> 24              (824 notes)
    //     freqVibLFO        2C -> 58              (595 notes)
    //
    // Pitches (`modEnvToPitch`, `modLfoToPitch`, `vibLfoToPitch`), on the
    // other hand, are **not doubled** - `vibLfoToPitch` matches 03 and FF on
    // 595 notes and `modLfoToPitch` the value 01 on 111 notes, so a
    // multiplication would break the match there. The dividing line is thus
    // pitch/filter, not SF1/SF2.
    auto modAmount = [&](int op, double sf2Scale, int sf1Scale = 1) -> int8_t
    {
        if (!g.Has(op)) return 0;
        return sf1 ? ClampS8(g.value[op] * sf1Scale)
                   : ClampS8(static_cast<int>(std::lround(g.value[op] / sf2Scale)));
    };

    // SF2 gives the depths in cents; the EMU8000 has +-1 octave at +-127,
    // i.e. 1200 cents / 127 steps = 9.45 cents per step. The filter has +-6
    // octaves (PEFE) or +-3 octaves (FMMOD), i.e. 56.7 or 28.3 cents per
    // step.
    const double kPitchCentsPerStep  = 1200.0 * kPefePitchOctaves / 127.0;
    const double kPefeFcCentsPerStep = 1200.0 * kPefeFilterOctaves / 127.0;
    const double kFmmodFcCentsPerStep = 1200.0 * kFmmodFilterOctaves / 127.0;
    const double kTremCbPerStep      = kTremoloMaxDb * 10.0 / 127.0;

    vp.pefe = static_cast<uint16_t>(
        (static_cast<uint8_t>(modAmount(Gen::ModEnvToPitch, kPitchCentsPerStep)) << 8)
        | static_cast<uint8_t>(modAmount(Gen::ModEnvToFilterFc, kPefeFcCentsPerStep, 2)));

    // When the generator is missing, the driver uses LFO1 frequency = 128.
    // Measured in the voice block at +0x2C on all 242 notes; SYNTHGM.SBK has
    // no freqModLFO for the piano, yet the driver writes TREMFRQ = 0x0080.
    // A present frequency is doubled like the depths above; a missing
    // generator, however, means the register value 128 directly, not 64x2.
    const int lfo1Freq = sf1 ? (g.Has(Gen::FreqModLFO)
                                    ? ((g.value[Gen::FreqModLFO] * 2) & 0xFF)
                                    : 128)
                             : std::clamp<int>(static_cast<int>(std::lround(
                                   8.176 * std::pow(2.0, g.Get(Gen::FreqModLFO, 0) / 1200.0)
                                   / kLfoHzPerStep)), 0, 255);
    // Note: the driver **lets the result overflow the byte**, it does not
    // clip it. Measured on RELAX: `freqVibLFO 132` -> 264 -> written 0x08,
    // we had given 0xFF.
    const int lfo2Freq = sf1 ? (g.Has(Gen::FreqVibLFO)
                                    ? ((g.value[Gen::FreqVibLFO] * 2) & 0xFF)
                                    : 0)
                             : std::clamp<int>(static_cast<int>(std::lround(
                                   8.176 * std::pow(2.0, g.Get(Gen::FreqVibLFO, 0) / 1200.0)
                                   / kLfoHzPerStep)), 0, 255);

    vp.fmmod = static_cast<uint16_t>(
        (static_cast<uint8_t>(modAmount(Gen::ModLfoToPitch, kPitchCentsPerStep)) << 8)
        | static_cast<uint8_t>(modAmount(Gen::ModLfoToFilterFc, kFmmodFcCentsPerStep, 2)));
    vp.tremfrq = static_cast<uint16_t>(
        (static_cast<uint8_t>(modAmount(Gen::ModLfoToVolume, kTremCbPerStep, 2)) << 8) | lfo1Freq);
    vp.fm2frq2 = static_cast<uint16_t>(
        (static_cast<uint8_t>(modAmount(Gen::VibLfoToPitch, kPitchCentsPerStep)) << 8) | lfo2Freq);

    // ---- envelopes -------------------------------------------------------
    auto timeMs = [&](int op, double defaultMs) -> double
    {
        if (!g.Has(op)) return defaultMs;
        return sf1 ? static_cast<double>(static_cast<uint16_t>(g.value[op]))
                   : TimecentsToMs(g.value[op]);
    };
    auto sustainReg = [&](int op) -> int
    {
        // A missing generator means sustain 0 (decay to silence), not 0x7F.
        // Measured in the voice block at +0x4A (DCYSUSV) and +0x3A (DCYSUS)
        // on all 242 notes; for the piano decaying is right.
        if (!g.Has(op)) return 0;
        // SF1: the sustain level in whole decibels above silence, the
        // register has steps of 0.75 dB - so the ratio is 4/3, not 1:1.
        //
        // Programmer's Guide on DCYSUSV: "bits 14-8 are the volume envelope
        // sustain level in 0.75dB increments, with 0x7f being no
        // attenuation". The conversion is verified on a bank we have in both
        // formats: `SYNTHGM.SBK` (SF1) and `SYNTHGM.SF2` of the DOS SDK
        // describe the same presets, so SF1 -> centibels can be read off:
        //
        //     SF1  99 -> 0 cB    -> register 127     (99*4/3 = 132, clipped)
        //     SF1  93 -> 23 cB   -> register 123.9   (124.0)
        //     SF1  92 -> 34 cB   -> register 122.5   (122.7)
        //     SF1  90 -> 55 cB   -> register 119.7   (120.0)
        //     SF1  87 -> 86 cB   -> register 115.5   (116.0)
        //
        // It fits the measurement too: preset 52 'Choir Aahs' has sustain 99
        // in the bank and the driver wrote 0x7F, while we sent 0x63
        // (= 99 raw).
        if (sf1) return std::clamp<int>(g.value[op] * 4 / 3, 0, 0x7F);
        // SF2: centibels of attenuation, 0 = full level
        return std::clamp(127 - static_cast<int>(std::lround(
            g.value[op] / 10.0 / kSustainDbPerStep)), 0, 127);
    };

    // Dependence of the envelope on the note number. Transcribed from
    // SBAWE32.DRV (0x0278):
    //     hold  += (60 - key) * keynumToHold        (non-negative)
    //     decay -= (key - 60) * keynumToDecay       (non-negative)
    // In SF2 the generators are in timecents per key, so the adjustment is
    // made before the conversion to milliseconds; for SF1 directly in ms as
    // in the driver.
    auto keyScaled = [&](int timeOp, int keyOp, bool subtract) -> double
    {
        const int amount = g.Get(keyOp, 0);
        if (amount == 0) return timeMs(timeOp, 0.0);
        if (sf1)
        {
            const double base = timeMs(timeOp, 0.0);
            const double delta = (subtract ? (key - 60) : (60 - key)) * amount;
            return std::max(0.0, subtract ? base - delta : base + delta);
        }
        const int tc = g.Has(timeOp) ? g.value[timeOp] : -12000;
        const int adj = tc + (subtract ? -(key - 60) : (60 - key)) * amount;
        return TimecentsToMs(adj);
    };

    // SF2 has the delay directly in timecents, so it goes straight into the
    // driver routine. SF1 has milliseconds - see DelayFromMs for how they
    // convert.
    auto delayReg = [&](int op) -> uint16_t
    {
        if (sf1)
            return static_cast<uint16_t>(DelayFromMs(timeMs(op, 0.0)));
        return static_cast<uint16_t>(
            DelayFromTimecents(g.Has(op) ? g.value[op] : -12000));
    };

    vp.envvol  = delayReg(Gen::DelayVolEnv);
    vp.atkhldv = static_cast<uint16_t>(
        (HoldFromMs(keyScaled(Gen::HoldVolEnv, Gen::KeynumToVolEnvHold, false)) << 8)
        | (g.Has(Gen::AttackVolEnv)
               ? AttackRateFromMs(timeMs(Gen::AttackVolEnv, 0.0))
               : kAttackDefaultRate));
    vp.dcysusv = static_cast<uint16_t>(
        (sustainReg(Gen::SustainVolEnv) << 8)
        | DecayRateFromMs(keyScaled(Gen::DecayVolEnv, Gen::KeynumToVolEnvDecay, true)));
    vp.releaseRate = static_cast<uint8_t>(DecayRateFromMs(timeMs(Gen::ReleaseVolEnv, 0.0)));

    vp.envval = delayReg(Gen::DelayModEnv);
    vp.atkhld = static_cast<uint16_t>(
        (HoldFromMs(keyScaled(Gen::HoldModEnv, Gen::KeynumToModEnvHold, false)) << 8)
        | (g.Has(Gen::AttackModEnv)
               ? AttackRateFromMs(timeMs(Gen::AttackModEnv, 0.0))
               : kAttackDefaultRate));
    vp.dcysus = static_cast<uint16_t>(
        (sustainReg(Gen::SustainModEnv) << 8)
        | DecayRateFromMs(keyScaled(Gen::DecayModEnv, Gen::KeynumToModEnvDecay, true)));
    vp.releaseModRate = static_cast<uint8_t>(
        DecayRateFromMs(timeMs(Gen::ReleaseModEnv, 0.0)));

    // When the attack comes out at maximum (0x7F, i.e. instant), the driver
    // writes **0xBFFF** to the corresponding delay register instead of
    // 0x8000. It does not affect the sound - bit 15 means "no delay" and the
    // lower 15 bits are then ignored (`ENVVOL_TO_EMU_SAMPLES`) - but it is in
    // the trace.
    //
    // Measured on Georgia: 844 notes of the preset `shonkytonk` (second
    // layer of Honky-Tonk, `attackVolEnv 0`) and drums. It corresponds to
    // the branch at SBAWE32.DRV 0x0206, only the listing has 0xB7FF there -
    // the measured value is 0xBFFF and applies outside channel 9 too.
    // Note: 0xBFFF for ENVVOL/ENVVAL is **not** substituted here. The driver
    // keeps the computed delay in the parameter block and sends the constant
    // only to the port (`SBAWE.VXD` 0x21AB: `push 0xBFFF`). We keep it the
    // same so the block can be compared 1:1 - see Synth::NoteOn and
    // tests/patch_cmp.py.

    vp.lfo1val = delayReg(Gen::DelayModLFO);
    vp.lfo2val = delayReg(Gen::DelayVibLFO);

    return vp;
}

int PitchToIp(int cents)
{
    return std::clamp(PitchFromCents(static_cast<double>(cents)),
                      0, 65535);
}

} // namespace SoundFont
