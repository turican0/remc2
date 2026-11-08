#include "Mc1Hmi.h"

#include <cstring>
#include <mutex>
#include <string>
#include <vector>

#include "../../remc2/engine/Sound.h"
#include "../../remc2/portability/awe32/Awe32Mdi.h"
#include "../../remc2/portability/port_filesystem.h"
#include "../../remc2/portability/port_sdl_sound.h"

// the driver files of the MSS emulation are those of the MC2 installation (gameFolder of the config)
static std::string SoundDriverPath(const char* file)
{
	return GetSubDirectoryPath(gameFolder.c_str()) + "/SOUND/" + file;
}

// ---- digital -------------------------------------------------------------------------------------------------

static const int kHmiSampleSlots = 32; // HMI mixes up to 32 samples, MC1 walks all of them
static HDIGDRIVER digDriver = nullptr;
static HSAMPLE digSamples[kHmiSampleSlots];
static int32_t digSampleRate = 11025;
static int digSlotCount = 0;

uint16_t sosDIGIInitDriver_64846_64D56(uint16_t sampleRate, uint16_t* driverHandle)
{
	// HMI: the driver of the detected card mixing at the rate of _SOS_DIGI_INIT_DRIVER (2B11h or 5622h)
	// the AIL preferences MC2 sets before AIL_install_DIG_driver_file (26E290), samples of the SB16 in 16 bits
	digSlotCount = maxSimultaniousSounds < kHmiSampleSlots ? maxSimultaniousSounds : kHmiSampleSlots;
	AilSetPreference_91A80(0, 200);
	AilSetPreference_91A80(1, sampleRate);
	AilSetPreference_91A80(3, 100);
	AilSetPreference_91A80(4, digSlotCount + 1);
	AilSetPreference_91A80(5, 127);
	AilSetPreference_91A80(6, 655);
	AilSetPreference_91A80(7, 0);
	AilSetPreference_91A80(8, 1);
	AilSetPreference_91A80(9, 1);
	IO_PARMS io = {};
	io.IO = 0x220;
	io.IRQ = -1;
	io.DMA_8_bit = -1;
	io.DMA_16_bit = -1;
	std::string path = SoundDriverPath("SB16.DIG");
	digDriver = AilInstallDigDriverFile_93330((char*)path.c_str(), &io);
	if (!digDriver)
	{
		Logger->error("MC1 sound: SB16.DIG not installed ({})", path);
		return 1;
	}
	digSampleRate = sampleRate;
	// one SDL_mixer channel per slot as MC2 does (26E290), the slots over the mixer channels stay null
	for (int i = 0; i < digSlotCount; i++)
	{
		digSamples[i] = AilAllocateSampleHandle_93510(digDriver);
		if (!digSamples[i])
			break;
		digSamples[i]->len_4_5[1] = 0;
		digSamples[i]->start_44mhz = nullptr;
		digSamples[i]->channel = i;
		digSamples[i]->id_9 = 0;
	}
	*driverHandle = 0;
	return 0;
}

uint16_t sosDIGIUnInitDriver_64F78_65488(uint16_t /*driverHandle*/, bool /*freeBuffer*/, bool /*closeDriver*/)
{
	if (!digDriver)
		return 0;
	for (int i = 0; i < kHmiSampleSlots; i++)
	{
		if (digSamples[i])
			AilEndSample_93D00(digSamples[i]);
		digSamples[i] = nullptr;
	}
	AilUninstallDigDriver_93480(digDriver);
	digDriver = nullptr;
	return 0;
}

bool sosDIGISampleDone_651F8_65708(uint16_t /*driverHandle*/, uint16_t sampleHandle)
{
	if (sampleHandle >= kHmiSampleSlots)
		return true;
	HSAMPLE sample = digSamples[sampleHandle];
	if (!sample)
		return true; // no mixer channel (maxSimultaniousSounds under 32), sosDIGIStartSample refuses it
	return AilSampleStatus_94010(sample) != 4; // 4 playing
}

uint16_t sosDIGIStartSample_6535C_6586C(uint16_t driverHandle, Type_SosStartSample* start)
{
	// HMI plays on the first free slot, the same slot MC1 found before through sosDIGISampleDone
	int slot = 0;
	for (; slot < kHmiSampleSlots; slot++)
		if (sosDIGISampleDone_651F8_65708(driverHandle, slot))
			break;
	if (slot == kHmiSampleSlots || !digSamples[slot])
		return 0xFFFF; // _ERR_NO_SLOTS
	HSAMPLE sample = digSamples[slot];
	AilInitSample_93830(sample);
	// 8-bit unsigned mono at the rate of the driver
	AilSetSampleAddress_93A10(sample, start->sample_0x00_0, start->size_0x08_8);
	AilSetSamplePlaybackRate_93D90(sample, digSampleRate);
	if (start->flags_0x1C_28 & SOS_VOLUME)
		AilSetSampleVolume_93E30(sample, start->volume_0x10_16 * 127 / 0x7FFF);
	// the remc2 emulation hands the loop count to SDL_mixer: 0 plays once, -1 forever
	if (start->flags_0x1C_28 & SOS_LOOPING)
		AilSetSampleLoopCount_93F70(sample, start->loopCount_0x0C_12 == -1 ? -1 : start->loopCount_0x0C_12);
	else
		AilSetSampleLoopCount_93F70(sample, 0);
	AilStartSample_93B50(sample);
	return (uint16_t)slot;
}

uint16_t sosDIGIStopSample_65965_65E75(uint16_t /*driverHandle*/, uint16_t sampleHandle)
{
	if (sampleHandle < kHmiSampleSlots && digSamples[sampleHandle])
		AilEndSample_93D00(digSamples[sampleHandle]);
	return 0;
}

// ---- MIDI ----------------------------------------------------------------------------------------------------

static HMDIDRIVER mdiDriver = nullptr;
static uint16_t channelStealing = 1;

// HMP (HMI NDMF) song
typedef struct {
	const uint8_t* start;    // first event (after the delta)
	const uint8_t* end;
	const uint8_t* position;
	uint32_t wait;           // ticks to the next event
	bool active;
	const uint8_t* loopPosition; // local loop (controllers 116 / 117)
	uint32_t loopWait;
	uint8_t loopCount;
	bool loopSet;
} Type_HmpTrack;

typedef struct {
	bool used;
	bool playing;
	uint32_t tempo; // ticks per second (+38h)
	const uint8_t* data;
	uint32_t trackMap[32];
	std::vector<Type_HmpTrack> tracks;
} Type_HmpSong;

static const int kMaxSongs = 8; // _SOS_MIDI_MAX_SONGS
static Type_HmpSong songs[kMaxSongs];
// the songs are guarded by the mutex of the AWE32 output, HmpService runs under it (one lock, no lock order)

// the variable length numbers of HMP: 7 bits per byte, low bits first, the last byte has bit 7 set
static uint32_t ReadHmpDelta(const uint8_t*& p, const uint8_t* end)
{
	uint32_t value = 0;
	int shift = 0;
	while (p < end)
	{
		uint8_t b = *p++;
		value |= (uint32_t)(b & 0x7F) << shift;
		shift += 7;
		if (b & 0x80)
			break;
	}
	return value;
}

static uint32_t ReadMidiLength(const uint8_t*& p, const uint8_t* end)
{
	uint32_t value = 0;
	while (p < end)
	{
		uint8_t b = *p++;
		value = (value << 7) | (b & 0x7F);
		if (!(b & 0x80))
			break;
	}
	return value;
}

static uint32_t ReadU32(const uint8_t* p)
{
	return p[0] | (p[1] << 8) | (p[2] << 16) | ((uint32_t)p[3] << 24);
}

static void SendMidi(uint8_t status, uint8_t data1, uint8_t data2)
{
	if (mdiDriver)
		AilSendChannelVoiceMessage_98360(mdiDriver, nullptr, status, data1, data2);
}

static void AllNotesOff()
{
	for (int channel = 0; channel < 16; channel++)
	{
		SendMidi(0xB0 | channel, 123, 0);
		SendMidi(0xB0 | channel, 64, 0);
	}
}

// one event of a track; false when the track ended
static bool PlayHmpEvent(Type_HmpTrack& track)
{
	const uint8_t* p = track.position;
	const uint8_t* end = track.end;
	if (p >= end)
		return false;
	uint8_t status = *p++;
	if (status == 0xFF)
	{
		uint8_t type = *p++;
		uint32_t length = ReadMidiLength(p, end);
		p += length;
		track.position = p;
		return type != 0x2F; // end of track
	}
	if (status == 0xF0 || status == 0xF7)
	{
		uint32_t length = ReadMidiLength(p, end);
		track.position = p + length;
		return true;
	}
	uint8_t kind = status & 0xF0;
	uint8_t data1 = *p++;
	uint8_t data2 = 0;
	if (kind != 0xC0 && kind != 0xD0)
		data2 = *p++;
	track.position = p;
	if (kind == 0xB0 && data1 >= 100)
	{
		// the special controllers of HMI are not sent to the device
		switch (data1)
		{
		case 110: // global loop start, the count (0 infinite) - MC1 songs do not use them, the track loop is the same
		case 116: // local loop start
			track.loopPosition = p;
			track.loopCount = data2;
			track.loopSet = true;
			break;
		case 111: // global loop end
		case 117: // local loop end
			if (track.loopSet && (track.loopCount == 0 || --track.loopCount))
				track.position = track.loopPosition;
			break;
		}
		return true;
	}
	SendMidi(status, data1, data2);
	return true;
}

// one tick of a song (the HMI song handler runs at the song tempo)
static void PlayHmpTick(Type_HmpSong& song)
{
	for (Type_HmpTrack& track : song.tracks)
	{
		if (!track.active)
			continue;
		while (track.active && track.wait == 0)
		{
			if (!PlayHmpEvent(track))
			{
				track.active = false;
				break;
			}
			if (track.position >= track.end)
			{
				track.active = false;
				break;
			}
			track.wait = ReadHmpDelta(track.position, track.end);
		}
		if (track.active)
			track.wait--;
	}
}

static void HmpService(void* /*arg*/)
{
	std::lock_guard<std::recursive_mutex> lock(Awe32Mdi::Mutex());
	for (Type_HmpSong& song : songs)
		if (song.used && song.playing)
		{
			PlayHmpTick(song);
			bool active = false;
			for (const Type_HmpTrack& track : song.tracks)
				active |= track.active;
			if (!active)
				song.playing = false;
		}
}

uint16_t sosMIDIInitDriver_6035F_6086F(uint16_t deviceId, uint16_t* driverHandle)
{
	// port: MC1 plays its music only through the AWE32 (_MIDI_AWE32), here the SBAWE32.MDI of MSS
	if (deviceId != SOS_MIDI_AWE32 || !awe32Music)
		return 1;
	// the AIL preferences MC2 sets before AIL_install_MDI_driver_file (26E6F0): service 120 Hz, volume 127
	AilSetPreference_91A80(11, 120);
	AilSetPreference_91A80(12, 1);
	AilSetPreference_91A80(13, 127);
	AilSetPreference_91A80(14, 1);
	AilSetPreference_91A80(15, 0);
	AilSetPreference_91A80(16, 12);
	IO_PARMS io = {};
	io.IO = 0x620;
	io.IRQ = -1;
	io.DMA_8_bit = -1;
	io.DMA_16_bit = -1;
	std::string path = SoundDriverPath("SBAWE32.MDI");
	mdiDriver = AilInstakkMidiDriverFile_95850((char*)path.c_str(), &io);
	if (!mdiDriver)
	{
		Logger->error("MC1 music: SBAWE32.MDI not installed ({})", path);
		return 1;
	}
	// the songs tick at their tempo in the AWE32 output (instead of the XMIDI service of MSS, MC1 has no XMI)
	Awe32Mdi::SetService(HmpService, nullptr, 120);
	*driverHandle = 0;
	return 0;
}

uint16_t sosMIDIUnInitDriver_6080C_60D1C(uint16_t /*driverHandle*/, bool /*closeDriver*/)
{
	if (!mdiDriver)
		return 0;
	{
		std::lock_guard<std::recursive_mutex> lock(Awe32Mdi::Mutex());
		for (Type_HmpSong& song : songs)
			song = Type_HmpSong();
	}
	Awe32Mdi::SetService(nullptr, nullptr, 120);
	AilUninstallMidiDriver_959A0(mdiDriver);
	mdiDriver = nullptr;
	return 0;
}

uint16_t sosMIDIResetDriver_6091D_60E2D(uint16_t /*driverHandle*/)
{
	AllNotesOff();
	for (int channel = 0; channel < 16; channel++)
		SendMidi(0xB0 | channel, 121, 0); // reset all controllers
	return 0;
}

static void SetupSongTracks(Type_HmpSong& song)
{
	const uint8_t* data = song.data;
	uint32_t trackCount = ReadU32(data + 0x30);
	song.tempo = ReadU32(data + 0x38);
	song.tracks.clear();
	const uint8_t* p = data + 0x308;
	for (uint32_t index = 0; index < trackCount; index++)
	{
		uint32_t length = ReadU32(p + 4);
		Type_HmpTrack track = {};
		track.start = p + 12;
		track.end = p + length;
		track.active = true;
		// 5F0ED: the track map of the application, 0FFh = the map of the file ([32][5] device IDs at 80h)
		if (index < 32 && song.trackMap[index] == SOS_MIDI_MAP_TRACK)
		{
			const uint8_t* map = data + 0x80 + index * 20;
			bool found = false;
			for (int k = 0; k < 5 && ReadU32(map + k * 4) && !found; k++)
			{
				uint32_t device = ReadU32(map + k * 4);
				// A000 also plays on the MPU-401 and the AWE32 (5F1C3); the only driver here is the AWE32
				if (device == 0xA000 || device == SOS_MIDI_AWE32)
					found = true;
			}
			if (ReadU32(map) != 0 && !found)
				track.active = false; // no driver for the track
		}
		if (track.active)
		{
			track.position = track.start;
			track.wait = ReadHmpDelta(track.position, track.end);
		}
		song.tracks.push_back(track);
		p += length;
	}
}

uint16_t sosMIDIInitSong_5EE22_5F332(Type_SosMidiInitSong* initSong, const uint32_t* trackMap, uint16_t* songHandle)
{
	std::lock_guard<std::recursive_mutex> lock(Awe32Mdi::Mutex());
	int handle = 0;
	while (handle < kMaxSongs && songs[handle].used)
		handle++;
	if (handle == kMaxSongs)
		return 1;
	const uint8_t* data = initSong->songData_0x00_0;
	if (!data || memcmp(data, "HMIMIDIP", 8))
		return 1;
	Type_HmpSong& song = songs[handle];
	song = Type_HmpSong();
	song.data = data;
	memcpy(song.trackMap, trackMap, sizeof(song.trackMap));
	SetupSongTracks(song);
	song.used = true;
	*songHandle = (uint16_t)handle;
	return 0;
}

uint16_t sosMIDIResetSong_5F529_5FA39(uint16_t songHandle, Type_SosMidiInitSong* initSong)
{
	std::lock_guard<std::recursive_mutex> lock(Awe32Mdi::Mutex());
	if (songHandle >= kMaxSongs || !songs[songHandle].used)
		return 1;
	Type_HmpSong& song = songs[songHandle];
	song.data = initSong->songData_0x00_0;
	song.playing = false;
	SetupSongTracks(song);
	return 0;
}

uint16_t sosMIDIUnInitSong_5F355_5F865(uint16_t songHandle)
{
	std::lock_guard<std::recursive_mutex> lock(Awe32Mdi::Mutex());
	if (songHandle < kMaxSongs)
		songs[songHandle] = Type_HmpSong();
	return 0;
}

uint16_t sosMIDIStartSong_5F398_5F8A8(uint16_t songHandle)
{
	std::lock_guard<std::recursive_mutex> lock(Awe32Mdi::Mutex());
	if (songHandle >= kMaxSongs || !songs[songHandle].used)
		return 1;
	// sosTIMERRegisterEvent with the tempo of the song (+38h), one tick per call
	Awe32Mdi::SetService(HmpService, nullptr, songs[songHandle].tempo ? songs[songHandle].tempo : 120);
	songs[songHandle].playing = true;
	return 0;
}

uint16_t sosMIDIStopSong_5F420_5F930(uint16_t songHandle)
{
	std::lock_guard<std::recursive_mutex> lock(Awe32Mdi::Mutex());
	if (songHandle < kMaxSongs)
		songs[songHandle].playing = false;
	AllNotesOff();
	return 0;
}

bool sosMIDISongDone_5D7F0_5DD00(uint16_t songHandle)
{
	std::lock_guard<std::recursive_mutex> lock(Awe32Mdi::Mutex());
	if (songHandle >= kMaxSongs)
		return true;
	return !songs[songHandle].playing;
}

uint16_t sosMIDIEnableChannelStealing_5EDD6_5F2E6(uint16_t enable)
{
	// channel stealing shares the channels between several drivers, MC1 has one
	uint16_t old = channelStealing;
	channelStealing = enable;
	return old;
}

// ---- timer ---------------------------------------------------------------------------------------------------

uint16_t sosTIMERRegisterEvent_5D97B_5DE8B(uint16_t rate, uint32_t (*event)(uint32_t), uint16_t* timerHandle)
{
	int index = AilRegisterTimer_92600(event);
	if (index == -1)
		return 1;
	AilSetTimerFrequency_92930(index, rate);
	AilStartTimer_92BA0(index);
	*timerHandle = (uint16_t)index;
	return 0;
}

void ReleaseTimerEvent(uint16_t timerHandle)
{
	AilReleaseTimer_92DC0(timerHandle);
}
