#pragma once
#include <cstdint>
#include <array>
#include <memory>
#include <string>
#include <vector>
#include "Emu8000.h"
#include "SoundFont.h"

// MIDI/MPU-401 interpretation layer over the register-level core Emu8000Core.
//
// The Synth holds no sound state - it translates a MIDI event into exactly
// the EMU8000 register writes the driver on a real card would make (write
// order included - DCYSUSV is written last, because it is what starts the
// envelope engine - see docs/re-notes/emu8000_register_map.md).
//
// Sound sources, exactly as on a real card:
//   - the card's wave ROM (`--rom`), mapped from address 0
//   - the description of the GM bank in ROM (`--rombank`), which only says
//     what lies where in the ROM
//   - user banks (`--sf`), whose samples are loaded into DRAM
//
// Banks are layered: the search starts at the last one loaded, so a user
// bank overrides a GM preset with the same number. Without any bank a
// substitute sine table plays.
class Synth
{
public:
    explicit Synth(uint32_t sampleRate);

    // Wave ROM = a raw dump, 16-bit little-endian samples.
    bool LoadWaveRom(const std::string& path, std::string& error);

    // Loads a bank (.SBK or .SF2). samplesInRom = the bank only describes
    // the contents of the wave ROM (typically 1mgm.sf2 for awe32.raw); its
    // `smpl` is ignored. `midiBank` >= 0 moves all presets of the bank that
    // have bank number 0 to this number. User banks (`.SBK` with their own
    // samples) usually have bank 0 in `phdr`, and the driver assigns them to
    // the user slot on loading - otherwise they would override the GM
    // presets. The drum bank 128 stays where it is.
    bool LoadBank(const std::string& path, std::string& error,
                  bool samplesInRom = false, int midiBank = -1);
    // The same from the bytes of a bank (a driver gets them from its caller).
    bool LoadBankData(std::vector<uint8_t> data, const std::string& name, std::string& error, int midiBank = -1);

    size_t BankCount() const { return m_banks.size(); }
    const SoundFont::Bank& BankAt(size_t i) const { return *m_banks[i].bank; }

    // Prints the first N started voices with their resulting registers.
    void SetVoiceDebug(int count) { m_debugVoices = count; }

    // Record of intermediate values at note-on, named after the **voice
    // parameter block in `SBAWE.VXD`** (EBX points to it, 0x94 B). The
    // columns are named like its fields on purpose, so they can be put next
    // to the output of the driver-structure dump and compared 1:1.
    bool OpenNoteDump(const std::string& path);
    void CloseNoteDump();

    // Bit mask of enabled MIDI channels (bit 0 = channel 1). Used to
    // isolate single tracks while debugging.
    void SetChannelMask(uint16_t mask) { m_channelMask = mask; }

    // Master volume of the AIL sequencer (`AIL_set_XMIDI_master_volume`).
    // Not a driver matter - the driver gets CC7 already multiplied - but
    // without it measurements of games that set their music volume cannot be
    // reproduced. See docs/re-notes/86box_comparison.md 15.8.
    void SetMasterVolume(int v) { m_masterVolume = v; }

    void NoteOn(uint8_t channel, uint8_t note, uint8_t velocity);
    void NoteOff(uint8_t channel, uint8_t note);
    void ChannelPressure(uint8_t channel, uint8_t value);
    void ProgramChange(uint8_t channel, uint8_t program);
    void ControlChange(uint8_t channel, uint8_t controller, uint8_t value);
    void PitchBend(uint8_t channel, int16_t value);

    void RenderBlock(int16_t* out, uint32_t numFrames);

    Emu8000Core& Core() { return m_core; }

    static constexpr int kMaxVoices = Emu8000Core::kMaxVoices;

private:
    struct LoadedBank
    {
        std::unique_ptr<SoundFont::Bank> bank;
        uint32_t dramBase = Emu8000::kDramOffset + 50;
    };

    struct VoiceAlloc
    {
        bool inUse = false;
        bool heldBySustain = false;
        uint8_t channel = 0;
        uint8_t note = 0;
        uint8_t velocity = 0;
        uint8_t releaseRate = 0x40;
        uint8_t releaseModRate = 0;
        uint32_t age = 0;
        int      basePitch = 0;   // IP without pitch bend

        // SBAWE32.MDI voice block (0x0BC6 + voice * 0x14), family `dos`.
        // State: 0xFFFF free, 0xFFFE reserved while a note is set up,
        // (channel << 8) | note playing, low byte 0xFF = held by the pedal.
        uint16_t mdiState = 0xFFFF;
        uint32_t mdiNoteEnd = 0;     // +0x0A: sample end + 4 for the note-off loop opening
        uint32_t mdiEndAddr = 0;     // +0x0E: voice is finished once CCCA passes this
        uint8_t  mdiVelocity = 0;    // +0x02
        uint8_t  mdiPatchAtten = 0;  // +0x03 (as 0x7F - field = attenuation units)
        bool     sdkRom = false;     // SDK: the voice plays a ROM sample (+16 attenuation)
        uint8_t  mdiModRelease = 0;  // +0x04
        uint8_t  mdiVolRelease = 0;  // +0x05
        int8_t   mdiFmmodDepth = 0;  // +0x06

        // SBAWE.VXD voice block (object 1, 0x42 + voice * 0x20), family
        // `win95`. State: 0xFFE free, 0xFFFE reserved while a note is set up,
        // (channel << 8) | note playing, low byte 0xFF = held by the pedal.
        uint16_t vxdState = 0xFFE;
        uint32_t vxdEndAddr = 0;     // +0x0A: finished once CCCA passes it
        bool     vxdRom = false;     // +0x0D: the sample lies in the wave ROM
        uint8_t  vxdPatchPan = 64;   // +0x12: pan of the patch (driver units)
        uint8_t  vxdPanTarget = 0;   // +0x18: pan register value to glide to
        uint8_t  vxdPanCur = 0;      // +0x19: pan register value written last
        int32_t  vxdPanTimer = -1;   // frames to the next glide step, -1 = none
        int8_t   vxdFmmodDepth = 0;  // +0x14: FMMOD pitch depth of the patch
        uint8_t  vxdPatchAtten = 0;  // patch attenuation (register units)
        bool     vxdRom1mgm = false; // +16 attenuation for a "1MGM" ROM sample
        uint32_t vxdNoteEnd = 0;     // +0x06: sample end for the note-off loop opening
        uint32_t vxdExclKey = 0;     // +0x1C: channel and preset of the note
        uint8_t  vxdExclClass = 0;   // +0x13: exclusive class of the layer
    };

    struct ChannelState
    {
        uint8_t program = 0;
        uint8_t modWheel = 0;   // CC1, see Synth::NoteOn
        uint8_t bankMsb = 0;         // CC0
        uint8_t bankLsb = 0;         // CC32
        uint8_t volume = 100;        // CC7
        uint8_t expression = 127;    // CC11
        uint8_t pan = 64;            // CC10
        uint8_t reverbSend = 0;      // CC91
        uint8_t chorusSend = 0;      // CC93
        bool sustain = false;        // CC64
        int16_t pitchBend = 0;
        uint8_t pitchBendRangeSemitones = 2;
        // Selected RPN (CC101 high, CC100 low byte). 0x7F/0x7F is "none" -
        // after it data entry sets nothing.
        uint8_t rpnMsb = 0x7F;
        uint8_t rpnLsb = 0x7F;

        // SBAWE32.MDI channel block (0x0E46 + channel * 0x1C), family `dos`.
        uint8_t mdiModDiv30 = 0;       // +0x09: CC1 / 30
        uint8_t mdiPressureDiv30 = 0;  // +0x0A: channel pressure / 30
        // SBAWE.VXD channel block (0x442 + channel * 0x24), family `win95`.
        uint8_t vxdPressureDiv30 = 0;  // +0x0B: channel pressure / 30
        int16_t mdiBendOffset = 0;     // +0x0C: IP offset computed at the last bend
        bool    mdiRpnMode = false;    // +0x14 == 0x100 after CC100 / CC101
        uint8_t mdiRpnLsb = 0;         // +0x16
        uint8_t mdiRpnMsb = 0;         // +0x17
    };

    int  EffectiveChannelVolume(int cc7) const
    {
        return (m_masterVolume >= 127) ? cc7 : cc7 * m_masterVolume / 127;
    }

    bool AddBank(SoundFont::Bank bank, std::string& error, bool samplesInRom, int midiBank);
    void BuildDefaultWaveform();
    int  AllocateVoice();
    void ReleaseVoice(int voice);
    void KillVoice(int voice);
    void RefreshChannel(uint8_t channel);
    int  BankNumberFor(uint8_t channel) const;
    int  PitchBendOffset(uint8_t channel) const;
    void StartVoice(int voice, uint8_t channel, uint8_t note, uint8_t velocity,
                    const SoundFont::VoiceParams& vp,
                    const SoundFont::Bank* bank, const SoundFont::Region* region);
    void StartFallbackVoice(int voice, uint8_t channel, uint8_t note, uint8_t velocity);
    void KillExclusiveVxd(uint32_t key, uint8_t cls);
    void StartLayers(size_t bankIndex, const std::vector<SoundFont::Region>& regions,
                     uint8_t channel, uint8_t note, uint8_t velocity, uint32_t presetId = 0);

    // Family `dos` (SBAWE32.MDI), see Synth.cpp.
    int  AllocateVoiceMdi(uint16_t state);
    int  AllocateVoiceSdk(uint16_t state);
    int  AllocateVoiceVxd(uint16_t state);
    void PanVxd(uint8_t channel, uint8_t value);
    void PanStepVxd(int voice);
    void UpdateFmmodVxd(uint8_t channel);
    void UpdateAttenVxd(uint8_t channel);
    void NoteOffMdi(int voice);
    void UpdateAttenMdi(uint8_t channel);
    void UpdateFmmodMdi(uint8_t channel, int value);
    void PitchBendMdi(uint8_t channel, int16_t value);
    void SustainMdi(uint8_t channel, uint8_t value);
    void AllNotesOffMdi(uint8_t channel, bool respectSustain);
    void ResetControllersMdi(uint8_t channel);
    bool ControlChangeMdi(uint8_t channel, uint8_t controller, uint8_t value);

    Emu8000Core m_core;
    std::vector<LoadedBank> m_banks;
    // The first sample does not start right at the start of DRAM. The
    // driver leaves 50 words before it: CCCA points 46 words before the
    // sample start (see Awe32::StartAddressOffset) and without a reserve it
    // would still point into the ROM. Measured against SBAWE32.MDI - its
    // first sample in DRAM starts at 0x200032 and ours did at 0x200000; on
    // all 52 drum notes of Magic Carpet 2 the difference was exactly 50.
    // **The families differ.** With SBAWE32.MDI the first sample starts at
    // 0x200032 (reserve 50), with SBAWE.VXD 34 lower - reserve 16. Measured
    // by swapping the bank in the guest: with SYNTH02S.SBK instead of
    // SYNTHGM.SBK the driver had CCCA, PSST and CSL exactly 34 lower than
    // ours on all 242 notes of MINUET.
    static constexpr uint32_t kDramReserveDos   = 50;
    static constexpr uint32_t kDramReserveWin95 = 16;
    uint32_t m_nextDramBase = 0;   // computed when the first bank is loaded
    int m_debugVoices = 0;
    void* m_noteDump = nullptr;   // FILE*
    uint16_t m_channelMask = 0xFFFF;
    int      m_masterVolume = 127;

    // Substitute sample when no bank covers the note.
    uint32_t m_fallbackStart = 0, m_fallbackLoopStart = 0, m_fallbackLoopEnd = 0;
    double   m_fallbackUnityHz = 0.0;

    std::array<VoiceAlloc, kMaxVoices> m_alloc{};
    std::array<ChannelState, 16> m_channels{};
    uint32_t m_ageCounter = 0;

    // The driver takes voices 30 and 31 for the DRAM refresh; 30 remain for
    // notes. SBAWE.VXD uses them too, though (a Georgia win95 trace has
    // note-ons on v30 and v31), so the `win95` family takes all 32.
    static constexpr int kUsableVoices = 30;
    static constexpr int kVxdVoices = 32;
    // Voices the driver family hands out: 32 for win95, 30 otherwise.
    int NoteVoices() const
    {
        return (m_core.DriverVariant() == Awe32::Driver::Win95) ? kVxdVoices : kUsableVoices;
    }
    // The sound data of the wave ROM starts at this word (see docs/re-notes).
    static constexpr uint32_t kRomPoolBase = 495;
    // Drum bank number per the GM/SoundFont convention.
    static constexpr int kDrumBank = 128;
};
