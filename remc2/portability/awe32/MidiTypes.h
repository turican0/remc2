#pragma once
#include <cstdint>
#include <vector>
#include <string>

// One representation of MIDI events for both .mid (SMF) and .xmi input.
// XmiFile and MidiFile both produce a std::vector<MidiEvent> sorted by
// absoluteTick, which the Sequencer then consumes.

enum class MidiEventType : uint8_t
{
    NoteOff,
    NoteOn,
    PolyPressure,
    ControlChange,
    ProgramChange,
    ChannelPressure,
    PitchBend,
    TempoChange,   // meta 0x51 (SMF) / derived from XMI, value in microseconds per quarter note
    EndOfTrack
};

struct MidiEvent
{
    uint32_t absoluteTick = 0;
    MidiEventType type = MidiEventType::NoteOn;
    uint8_t channel = 0;   // 0-15, unused for TempoChange/EndOfTrack
    uint8_t data1 = 0;     // note / controller / program
    uint8_t data2 = 0;     // velocity / controller value
    uint32_t tempoUsPerQuarter = 500000; // valid for TempoChange only
};

// Result of parsing an input file (the same for MidiFile and XmiFile),
// played by the Sequencer.
struct ParsedSequence
{
    std::vector<MidiEvent> events;   // sorted by absoluteTick
    uint16_t ticksPerQuarterNote = 480;
    bool valid = false;
    std::string errorMessage;
};
