#include "Awe32Mdi.h"

#include "Synth.h"
#include "../port_filesystem.h"

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <cstring>
#include <memory>
#include <thread>
#include <vector>

namespace
{
	void RenderThread();
	void StopRenderThread();

	std::recursive_mutex g_mutex;
	std::unique_ptr<Synth> g_synth;
	std::vector<uint8_t> g_driverImage;
	std::string g_romPath;

	void (*g_service)(void*) = nullptr;
	void* g_serviceArg = nullptr;
	int g_serviceHz = 0;
	int64_t g_untilService = 0;// frames * hz

	// The card is rendered ahead in its own thread; the audio callback only copies the ring buffer,
	// so neither the chip nor the game holding the driver can make it late.
	// The ahead time is the awe32BufferMs setting (SetBufferMs), it has to cover the 4096 frames
	// of the SDL callback and the slow blocks of the chip, a debug build needs more.
	const int kChunkFrames = 512;
	int g_aheadFrames = 8192;
	int g_ringFrames = 16384;
	std::vector<int16_t> g_ring(g_ringFrames * 2);
	uint64_t g_ringRead = 0;
	uint64_t g_ringWrite = 0;
	std::mutex g_ringMutex;
	std::condition_variable g_ringCv;
	std::thread g_renderThread;
	std::atomic<bool> g_renderStop{ false };
	// stops the thread before the objects above are destroyed when the program exits without Shutdown
	struct RenderThreadGuard { ~RenderThreadGuard() { StopRenderThread(); } } g_renderThreadGuard;

	// The card of the MC2 traces (DOS VM) had 512 KB.
	const int kChipRamKb = 512;

	// user bank upload (fn 0x506)
	std::vector<uint32_t> g_bankSizes;
	std::vector<uint8_t> g_bankHeader;
	std::vector<uint8_t> g_bankSamples;
	uint32_t g_sampleOffset = 0;
	uint32_t g_sampleSize = 0;
	uint32_t g_presetOffset = 0;
	uint32_t g_presetSize = 0;
	uint32_t g_fileSize = 0;
	uint16_t g_infoSegment = 0;
	uint16_t g_userBank = 0;

	uint32_t Rd32(const uint8_t* p) { return p[0] | (p[1] << 8) | (p[2] << 16) | ((uint32_t)p[3] << 24); }
	uint16_t Rd16(const uint8_t* p) { return (uint16_t)(p[0] | (p[1] << 8)); }
	void Wr16(uint8_t* p, uint16_t v) { p[0] = (uint8_t)v; p[1] = (uint8_t)(v >> 8); }
	void Wr32(uint8_t* p, uint32_t v) { for (int i = 0; i < 4; i++) p[i] = (uint8_t)(v >> (8 * i)); }

	void CopyDramToChip()
	{
		if (g_synth->Core().ChipVariant() != Emu8000Core::Chip::Box86)
			return;
		if (int16_t* ram = g_synth->Core().ChipRam())
		{
			const size_t n = std::min(g_synth->Core().DramSize(), g_synth->Core().ChipRamWords());
			memcpy(ram, g_synth->Core().DramData(), n * sizeof(int16_t));
		}
	}

	// The first 512 bytes of the SBK: where the samples and the presets lie.
	bool ParseBankHeader(const uint8_t* h, uint32_t fileSize)
	{
		if (memcmp(h, "RIFF", 4) || memcmp(h + 8, "sfbk", 4))
			return false;
		const uint32_t riffEnd = 8 + Rd32(h + 4);
		g_fileSize = fileSize ? fileSize : riffEnd;
		uint32_t o = 12;
		while (o + 12 <= 512)
		{
			const uint32_t size = Rd32(h + o + 4);
			if (!memcmp(h + o, "LIST", 4) && !memcmp(h + o + 8, "sdta", 4))
			{
				const uint32_t end = o + 8 + size;
				for (uint32_t c = o + 12; c + 8 <= 512 && c < end; c += 8 + Rd32(h + c + 4) + (Rd32(h + c + 4) & 1))
					if (!memcmp(h + c, "smpl", 4))
					{
						g_sampleOffset = c + 8;
						g_sampleSize = Rd32(h + c + 4);
						g_presetOffset = end + (size & 1);
						if (g_presetOffset > g_fileSize)
							return false;
						g_presetSize = g_fileSize - g_presetOffset;
						return true;
					}
				return false;
			}
			o += 8 + size + (size & 1);
		}
		return false;
	}

	bool LoadUserBank(const uint8_t* presets, std::string& error)
	{
		std::vector<uint8_t> file(g_fileSize, 0);
		memcpy(file.data(), g_bankHeader.data(), std::min<size_t>(g_sampleOffset, g_bankHeader.size()));
		memcpy(file.data() + g_sampleOffset, g_bankSamples.data(), std::min<size_t>(g_sampleSize, g_bankSamples.size()));
		memcpy(file.data() + g_presetOffset, presets, g_presetSize);
		if (!g_synth->LoadBankData(std::move(file), "user bank", error, g_userBank))
			return false;
		CopyDramToChip();
		const SoundFont::Bank& bank = g_synth->BankAt(g_synth->BankCount() - 1);
		Logger->info("AWE32: user bank loaded, {} presets, {} samples ({} bytes)", bank.presets.size(), bank.samples.size(), g_fileSize);
		return true;
	}
}

std::recursive_mutex& Awe32Mdi::Mutex()
{
	return g_mutex;
}

bool Awe32Mdi::Ready()
{
	return g_synth != nullptr;
}

void Awe32Mdi::Install(const uint8_t* image, size_t size, const std::string& romPath)
{
	std::lock_guard<std::recursive_mutex> lock(g_mutex);
	g_driverImage.assign(image, image + size);
	g_romPath = romPath;
}

bool Awe32Mdi::InitInstruments(std::string& error)
{
	std::lock_guard<std::recursive_mutex> lock(g_mutex);
	auto synth = std::make_unique<Synth>(kSampleRate);
	if (!synth->LoadWaveRom(g_romPath, error))
		return false;
	synth->Core().SetChipRamKb(kChipRamKb);
	if (!synth->Core().UseBox86Chip(g_romPath, error))
		return false;
	synth->Core().SetDriver(Awe32::Driver::Dos);
	if (!synth->LoadBankData(g_driverImage, "SBAWE32.MDI", error))
		return false;
	g_synth = std::move(synth);
	CopyDramToChip();
	g_synth->Core().PowerOnInit();
	g_synth->SetMasterVolume(127);
	g_untilService = 0;
	Logger->info("AWE32: wave ROM {} ({} samples), GM bank of SBAWE32.MDI {} presets",
		g_romPath, g_synth->Core().RomSize(), g_synth->BankAt(0).presets.size());
	g_ringRead = g_ringWrite = 0;
	g_renderStop = false;
	if (!g_renderThread.joinable())
		g_renderThread = std::thread(RenderThread);
	return true;
}

void Awe32Mdi::Shutdown()
{
	StopRenderThread();
	std::lock_guard<std::recursive_mutex> lock(g_mutex);
	g_service = nullptr;
	g_synth.reset();
}

void Awe32Mdi::SendMessages(const uint8_t* data, int count)
{
	std::lock_guard<std::recursive_mutex> lock(g_mutex);
	if (!g_synth)
		return;
	int pos = 0;
	for (int i = 0; i < count && pos < 512; i++)
	{
		const uint8_t status = data[pos];
		const uint8_t ch = status & 0x0F;
		switch (status & 0xF0)
		{
		case 0x80: g_synth->NoteOff(ch, data[pos + 1]); pos += 3; break;
		case 0x90: g_synth->NoteOn(ch, data[pos + 1], data[pos + 2]); pos += 3; break;
		case 0xA0: pos += 3; break;
		case 0xB0: g_synth->ControlChange(ch, data[pos + 1], data[pos + 2]); pos += 3; break;
		case 0xC0: g_synth->ProgramChange(ch, data[pos + 1]); pos += 2; break;
		case 0xD0: g_synth->ChannelPressure(ch, data[pos + 1]); pos += 2; break;
		case 0xE0: g_synth->PitchBend(ch, (int16_t)(((data[pos + 2] << 7) | data[pos + 1]) - 8192)); pos += 3; break;
		default:
			// a system exclusive message is alone in the buffer (AIL sub_A4FD0)
			Logger->debug("AWE32: MIDI message {:02X} ignored", status);
			return;
		}
	}
}

void Awe32Mdi::Extension(Regs& regs, RealPointer realPointer, RealAlloc realAlloc)
{
	std::lock_guard<std::recursive_mutex> lock(g_mutex);
	const uint16_t function = (uint16_t)regs.BX;
	uint8_t* params = (function == 0x10) ? nullptr : realPointer((uint16_t)regs.CX, (uint16_t)regs.DX);
	const uint16_t bank = params ? Rd16(params) : 0;
	uint8_t* data = params ? realPointer(Rd16(params + 2), 0) : nullptr;
	regs.AX = 1;
	if (!g_synth || (function != 0x10 && (params == nullptr || data == nullptr)))
		return;
	switch (function)
	{
	case 0x10://DRAM size
		regs.CX = (int16_t)((kChipRamKb * 1024) >> 16);
		regs.DX = 0;
		regs.AX = 0;
		break;
	case 0x11://bank sizes
		g_bankSizes.clear();
		for (int i = 0; i < bank; i++)
			g_bankSizes.push_back(Rd32(data + 4 * i));
		regs.AX = 0;
		break;
	case 0x17://load request: the first 512 bytes of the bank
	{
		g_bankHeader.assign(data, data + 512);
		g_userBank = bank;
		g_bankSamples.clear();
		if (!ParseBankHeader(data, bank < g_bankSizes.size() ? g_bankSizes[bank] : 0))
		{
			Logger->error("AWE32: the user bank is not a SoundFont 1 bank");
			break;
		}
		if (!g_infoSegment)
			g_infoSegment = realAlloc(14);
		uint8_t* info = g_infoSegment ? realPointer(g_infoSegment, 0) : nullptr;
		if (!info)
			break;
		Wr16(info, (uint16_t)((g_sampleSize + 511) / 512));
		Wr32(info + 2, g_sampleOffset);
		Wr32(info + 6, g_presetOffset);
		Wr32(info + 10, g_presetSize);
		regs.CX = (int16_t)g_infoSegment;
		regs.DX = 0;
		regs.AX = 0;
		break;
	}
	case 0x18://512 bytes of the samples
		g_bankSamples.insert(g_bankSamples.end(), data, data + 512);
		regs.AX = 0;
		break;
	case 0x19://the presets
	{
		std::string error;
		if (!LoadUserBank(data, error))
		{
			Logger->error("AWE32: the user bank could not be loaded: {}", error);
			break;
		}
		g_bankSamples.clear();
		regs.AX = 0;
		break;
	}
	case 0x12://release the bank
		regs.AX = 0;
		break;
	default:
		Logger->debug("AWE32: driver function 0x506/{:X} ignored", function);
		break;
	}
}

void Awe32Mdi::SetService(void (*service)(void*), void* arg, int hz)
{
	std::lock_guard<std::recursive_mutex> lock(g_mutex);
	g_service = service;
	g_serviceArg = arg;
	g_serviceHz = hz;
	g_untilService = 0;
}

namespace
{
	// `frames` of the card, the AIL service called on its tick between them
	void RenderFrames(int16_t* out, int frames)
	{
		while (frames > 0)
		{
			std::lock_guard<std::recursive_mutex> lock(g_mutex);
			if (!g_synth)
			{
				memset(out, 0, frames * 2 * sizeof(int16_t));
				return;
			}
			int n = frames;
			if (g_service && g_serviceHz > 0)
			{
				if (g_untilService <= 0)
				{
					g_service(g_serviceArg);
					g_untilService += Awe32Mdi::kSampleRate;
				}
				n = (int)std::min<int64_t>(frames, (g_untilService + g_serviceHz - 1) / g_serviceHz);
				g_untilService -= (int64_t)n * g_serviceHz;
			}
			g_synth->RenderBlock(out, (uint32_t)n);
			out += 2 * n;
			frames -= n;
		}
	}

	void RenderThread()
	{
		std::vector<int16_t> chunk(kChunkFrames * 2);
		while (true)
		{
			{
				std::unique_lock<std::mutex> lock(g_ringMutex);
				g_ringCv.wait(lock, [] { return g_renderStop || g_ringWrite - g_ringRead < (uint64_t)g_aheadFrames; });
				if (g_renderStop)
					return;
			}
			RenderFrames(chunk.data(), kChunkFrames);
			std::lock_guard<std::mutex> lock(g_ringMutex);
			for (int i = 0; i < kChunkFrames; i++)
			{
				const size_t pos = (size_t)((g_ringWrite + i) % g_ringFrames) * 2;
				g_ring[pos] = chunk[2 * i];
				g_ring[pos + 1] = chunk[2 * i + 1];
			}
			g_ringWrite += kChunkFrames;
		}
	}

	void StopRenderThread()
	{
		if (!g_renderThread.joinable())
			return;
		{
			std::lock_guard<std::mutex> lock(g_ringMutex);
			g_renderStop = true;
		}
		g_ringCv.notify_all();
		g_renderThread.join();
	}
}

void Awe32Mdi::SetBufferMs(int ms)
{
	ms = std::clamp(ms, kMinBufferMs, kMaxBufferMs);
	const int frames = (kSampleRate * ms / 1000 + kChunkFrames - 1) / kChunkFrames * kChunkFrames;
	std::lock_guard<std::mutex> lock(g_ringMutex);
	g_aheadFrames = frames;
	g_ringFrames = 2 * frames;
	g_ring.assign((size_t)g_ringFrames * 2, 0);
	g_ringRead = g_ringWrite = 0;
	g_ringCv.notify_one();
}

void Awe32Mdi::Render(int16_t* out, int frames)
{
	std::lock_guard<std::mutex> lock(g_ringMutex);
	const int available = (int)std::min<uint64_t>(g_ringWrite - g_ringRead, (uint64_t)frames);
	for (int i = 0; i < available; i++)
	{
		const size_t pos = (size_t)((g_ringRead + i) % g_ringFrames) * 2;
		out[2 * i] = g_ring[pos];
		out[2 * i + 1] = g_ring[pos + 1];
	}
	g_ringRead += available;
	if (available < frames)
		memset(out + 2 * available, 0, (frames - available) * 2 * sizeof(int16_t));
	g_ringCv.notify_one();
}
