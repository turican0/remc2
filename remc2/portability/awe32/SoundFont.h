#pragma once
#include "Awe32Driver.h"
#include <cstdint>
#include <array>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// Generic SoundFont bank loader - handles SoundFont 1.0 (`.SBK`) and SF2 (`.sf2`).
//
// On purpose NOT tailored to any particular bank. The differences of the
// two versions (see docs/re-notes/soundfont1_sbk.md):
//
//   SF1.0                              SF2
//   -----                              ---
//   shdr 16 B (4 dwords)               shdr 46 B incl. name/sr/root key
//   sample names in the `snam` chunk   names inside shdr
//   generators = EMU8000 registers     normalised generators
//     directly, times in milliseconds    (timecents, centibels, cents)
//   addresses ready for the chip       addresses are indices into `smpl`
//
// A sample whose name starts with '*' lies in the card's wave ROM (SF1
// convention); in SF2 the ROM flag in `sfSampleType` means the same. A bank
// may use both sources at once.
// ---------------------------------------------------------------------------

namespace SoundFont
{
    enum class Version { Sf1, Sf2 };

    // Generator numbers per the specification (shared by both versions; SF1
    // just leaves some unused and interprets them differently).
    namespace Gen
    {
        enum : int
        {
            StartAddrsOffset = 0, EndAddrsOffset = 1,
            StartloopAddrsOffset = 2, EndloopAddrsOffset = 3,
            StartAddrsCoarseOffset = 4,
            ModLfoToPitch = 5, VibLfoToPitch = 6, ModEnvToPitch = 7,
            InitialFilterFc = 8, InitialFilterQ = 9,
            ModLfoToFilterFc = 10, ModEnvToFilterFc = 11,
            EndAddrsCoarseOffset = 12, ModLfoToVolume = 13,
            ChorusEffectsSend = 15, ReverbEffectsSend = 16, Pan = 17,
            DelayModLFO = 21, FreqModLFO = 22, DelayVibLFO = 23, FreqVibLFO = 24,
            DelayModEnv = 25, AttackModEnv = 26, HoldModEnv = 27,
            DecayModEnv = 28, SustainModEnv = 29, ReleaseModEnv = 30,
            KeynumToModEnvHold = 31, KeynumToModEnvDecay = 32,
            DelayVolEnv = 33, AttackVolEnv = 34, HoldVolEnv = 35,
            DecayVolEnv = 36, SustainVolEnv = 37, ReleaseVolEnv = 38,
            KeynumToVolEnvHold = 39, KeynumToVolEnvDecay = 40,
            Instrument = 41, KeyRange = 43, VelRange = 44,
            StartloopAddrsCoarseOffset = 45, Keynum = 46, Velocity = 47,
            InitialAttenuation = 48, EndloopAddrsCoarseOffset = 50,
            CoarseTune = 51, FineTune = 52, SampleID = 53, SampleModes = 54,
            // 55 is unused in SF2; in SF1 banks it appears with values like
            // 6000 = the root key in cents. See docs/re-notes.
            Sf1RootPitchCents = 55,
            ScaleTuning = 56, ExclusiveClass = 57, OverridingRootKey = 58,
            Count = 60
        };
    }

    // A set of generators with a "was set" flag (for correct combining of
    // zones and defaults).
    struct GenSet
    {
        std::array<int16_t, Gen::Count> value{};
        std::array<bool, Gen::Count> present{};

        void Set(int op, int16_t v)
        {
            if (op >= 0 && op < Gen::Count) { value[op] = v; present[op] = true; }
        }
        bool Has(int op) const { return op >= 0 && op < Gen::Count && present[op]; }
        int Get(int op, int fallback) const { return Has(op) ? value[op] : fallback; }
        // Combining a preset zone (offsets) over an instrument zone (absolute).
        void AddFrom(const GenSet& other);
        void OverrideFrom(const GenSet& other);
        // Fills in only what is missing - the instrument's value wins.
        void FillFrom(const GenSet& other);
    };

    struct Sample
    {
        std::string name;
        uint32_t start = 0, end = 0, loopStart = 0, loopEnd = 0;
        uint32_t sampleRate = 44100;
        uint8_t  originalKey = 60;
        int8_t   correction = 0;
        bool     inRom = false;
    };

    struct Zone
    {
        GenSet gen;
        int keyLo = 0, keyHi = 127;
        int velLo = 0, velHi = 127;
        int sampleId = -1;     // instrument zone
        int instrument = -1;   // preset zone
    };

    struct Instrument
    {
        std::string name;
        GenSet global;
        std::vector<Zone> zones;
    };

    struct Preset
    {
        std::string name;
        int bank = 0, program = 0;
        GenSet global;
        std::vector<Zone> zones;
    };

    // One layer that is to sound for a given note.
    struct Region
    {
        const Sample* sample = nullptr;
        GenSet gen;
        // SF1: the attenuation **cannot** be taken from the combined GenSet.
        // Each level (instrument zone, preset zone) contributes `127 - v`
        // register units and those add up; the sum of the raw SF1 values
        // would be nonsense. -1 means "not computed" (the bank is SF2).
        int sf1AttenUnits = -1;
    };

    struct Bank
    {
        bool valid = false;
        std::string errorMessage;
        Version version = Version::Sf2;
        std::string name;
        std::string romName;            // INFO/irom - which ROM the bank expects
        // True = the samples of this bank are not in its smpl chunk but in
        // the card's wave ROM. That is how the description of a GM bank (e.g.
        // 1mgm.sf2) is used to play the contents of the real ROM (awe32.raw).
        bool samplesInRom = false;

        std::vector<int16_t> sampleData;  // chunk `smpl`
        std::vector<Sample> samples;
        std::vector<Instrument> instruments;
        std::vector<Preset> presets;

        const Preset* FindPreset(int bank, int program) const;

        // Returns all layers that are to sound for a given note.
        // An empty result = the preset does not exist or does not cover the note.
        std::vector<Region> Select(int bank, int program, int key, int velocity) const;
    };

    Bank Load(const std::string& path);
    // The same from the bytes of the file; `name` is only for messages.
    Bank LoadData(std::vector<uint8_t> buf, const std::string& name);

    // -----------------------------------------------------------------------
    // Conversion of one layer to EMU8000 registers.
    //
    // dramBase = address (in samples) where this bank's `smpl` chunk was loaded.
    // romPoolBase = address of the start of the sound pool in the wave ROM.
    // -----------------------------------------------------------------------
    // Cents to the IP register (`sub_192E` of the driver). The Synth needs
    // it too, when it adds a pitch bend in cents.
    int PitchToIp(int cents);

    struct VoiceParams
    {
        uint32_t ccca = 0;        // Q + control + start address
        uint32_t sampleStart = 0; // raw sample address (without offset)
        int      patchPan = 64;   // patch pan in driver units (64 = centre)
        uint32_t psst = 0;        // pan + loop start
        uint32_t csl = 0;         // chorus send + loop end
        uint16_t ip = 0;          // pitch for the specific note
        // The same pitch in cents, before the conversion to IP - useful when
        // analysing differences against the driver.
        int      ipCents = 0;
        uint16_t ifatn = 0xFF00;     // high byte = cutoff; Synth fills in the low one
        uint8_t  patchAttenUnits = 0;// patch attenuation in 0.375 dB units
        uint16_t pefe = 0, fmmod = 0, tremfrq = 0, fm2frq2 = 0;
        uint16_t envvol = 0x8000, atkhldv = 0x7F7F, dcysusv = 0x7F00;
        uint16_t envval = 0x8000, atkhld = 0x7F7F, dcysus = 0x7F00;
        uint16_t lfo1val = 0x8000, lfo2val = 0x8000;
        uint8_t  reverbSend = 0;
        uint8_t  releaseRate = 0; // for DCYSUSV at Note Off
        uint8_t  releaseModRate = 0; // for DCYSUS at Note Off (win95 only)
        bool     looping = true;
        // Sample end and loop end addresses as the driver keeps them in its
        // patch block (SBAWE32.MDI [si+0x7A] / [si+0x82]); the note-off of the
        // `dos` family opens the loop with them.
        uint32_t sampleEndAddr = 0;
        uint32_t loopEndAddr = 0;
    };

    VoiceParams MakeVoiceParams(const Bank& bank, const Region& region,
                                int key, int velocity,
                                uint32_t dramBase, uint32_t romPoolBase,
                                Awe32::Driver drv = Awe32::kDefaultDriver);
}
