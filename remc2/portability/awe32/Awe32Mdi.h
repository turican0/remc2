#pragma once
#ifndef AWE32_MDI_H
#define AWE32_MDI_H

#include <cstdint>
#include <mutex>
#include <string>

// The SBAWE32.MDI driver of AIL (Miles) for the game: what the game calls through
// AIL_call_driver goes here, the driver runs the EMU8000 emulation from AWE32Emu
// (driver family "dos" = SBAWE32.MDI, chip snd_emu8k.c from 86Box).
namespace Awe32Mdi
{
	struct Regs
	{
		int16_t AX, BX, CX, DX;
	};

	// DOS conventional memory of the caller: the far pointers of fn 0x506 are real mode segments.
	typedef uint8_t* (*RealPointer)(uint16_t segment, uint16_t offset);
	typedef uint16_t (*RealAlloc)(uint32_t bytes);

	// fn 0x300: the driver image the game read from SOUND/SBAWE32.MDI (it holds the GM bank).
	void Install(const uint8_t* image, size_t size, const std::string& romPath);
	// fn 0x501: init of the instrument manager (chip, ROM, GM bank); false = AX 0.
	bool InitInstruments(std::string& error);
	// fn 0x306
	void Shutdown();
	// fn 0x502: `count` buffered MIDI messages.
	void SendMessages(const uint8_t* data, int count);
	// fn 0x506: AWE32 functions selected by BX (user bank upload).
	void Extension(Regs& regs, RealPointer realPointer, RealAlloc realAlloc);

	// The XMIDI sequencer service of AIL, called `hz` times a second of rendered sound.
	void SetService(void (*service)(void*), void* arg, int hz);
	// Stereo 16-bit frames of the card output at 44100 Hz.
	void Render(int16_t* out, int frames);
	// How far ahead of the audio callback the card is rendered (the music starts this much later).
	void SetBufferMs(int ms);

	bool Ready();
	// Guards the driver and the sequencer between the game, the AIL timers and the audio thread.
	std::recursive_mutex& Mutex();

	const int kSampleRate = 44100;
	const int kMinBufferMs = 100;
	const int kMaxBufferMs = 2000;
}

#endif //AWE32_MDI_H
