#pragma once

#include <cstdint>
#include <cstddef>
#include <string>

// ---------------------------------------------------------------------------
// Emu8000Box - wrapper around `snd_emu8k.c` from 86Box (src/86box, with the
// AWE32Emu changes measured on a real card). The DOS test VM builds the same
// code from its own copy in the 86Box tree; chipcheck.py (against
// emu8k_ref.exe, built from that tree) shows whether the two still agree.
//
// Timing. 86Box drives the chip in blocks of `WTBUFLEN` = 980 frames: all
// writes are applied first, each at its own offset in the block, then one
// call of `emu8k_update()` renders the whole block. Our sequencer renders
// frame by frame instead. Stepping the chip sample by sample does not work -
// upstream ran the effects only when `num_active > 0`, evaluated once per
// call, so the chorus would drift apart.
//
// The solution is a **delay of one block**: writes are queued and the sound
// comes from the previous, already finished block. The latency is exactly
// 980 frames (kLatencyFrames) and constant - it is simply cut off at the
// output.
// ---------------------------------------------------------------------------
class Emu8000Box
{
public:
    static constexpr int kBlockFrames   = 980;   // WTBUFLEN = 44100/45
    static constexpr int kLatencyFrames = kBlockFrames;

    Emu8000Box();
    ~Emu8000Box();
    Emu8000Box(const Emu8000Box&) = delete;
    Emu8000Box& operator=(const Emu8000Box&) = delete;

    // romPath has to point to the 1 MB raw dump of the wave ROM (awe32.raw) -
    // the chip loads it itself through rom_fopen, exactly as in 86Box.
    bool Init(const std::string& romPath, uint16_t basePort, int ramKb, std::string& err);
    bool Ready() const { return m_ready; }

    // Sound DRAM of the chip. The bank samples are loaded into it with
    // memcpy, as with our own core - see Synth::LoadBank.
    int16_t* Ram();
    size_t   RamWords() const;

    void PortWrite(uint16_t port, uint16_t value, bool isByte = false);

    // Port read, as a driver does it. Pending writes of the current block are
    // applied first and the chip is brought up to the current frame, so the
    // value is the chip state at this moment (86Box: every outw runs
    // emu8k_update, and a driver always writes the pointer before a read).
    uint16_t PortRead(uint16_t port);

    // One frame at 44100 Hz. The first kLatencyFrames calls return silence.
    void RenderFrame(int32_t& l, int32_t& r);

private:
    void FlushBlock();

    struct Impl;
    Impl* m_impl = nullptr;
    bool  m_ready = false;
};
